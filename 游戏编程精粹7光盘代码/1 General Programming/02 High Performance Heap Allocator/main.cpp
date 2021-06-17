// Copyright (c) 2007, Dimitar Lazarov, Luxoflux
// dimitar.lazarov@usa.net

#include "hpha.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <new>

allocator gAllocator;

const unsigned N = 1024*512;
const size_t MIN_SIZE_LOG2 = 1;
const size_t MIN_SIZE = 1 << MIN_SIZE_LOG2;
const size_t MAX_SIZE_LOG2 = 12;
const size_t MAX_SIZE = 1 << MAX_SIZE_LOG2;
const size_t MAX_ALIGNMENT_LOG2 = 7;
const size_t MAX_ALIGNMENT = 1 << MAX_ALIGNMENT_LOG2;

struct test_record {
	void* ptr;
	size_t size;
	size_t alignment;
	size_t _padding;
} tr[N];

size_t rand_size() {
	float r = float(rand()) / RAND_MAX;
	return MIN_SIZE + (size_t)((MAX_SIZE - MIN_SIZE) * powf(r, 8.0f));
}

size_t rand_alignment() {
	float r = float(rand()) / RAND_MAX;
	return 1 << (size_t)(MAX_ALIGNMENT_LOG2 * r);
}

void benchmark1() {
	clock_t start, finish;

	printf("\t\t\t\t\t\tHPHA\t\tDEFAULT\n");

	printf("TEST ALLOC/FREE:");
	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		tr[i].ptr = gAllocator.alloc(size);
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		gAllocator.free(tr[j].ptr);
		tr[j].ptr = tr[i].ptr;
	}
	finish = clock();
	printf("\t\t\t\t(%f)", (double)(finish - start) / CLOCKS_PER_SEC);

	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		tr[i].ptr = malloc(size);
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		free(tr[j].ptr);
		tr[j].ptr = tr[i].ptr;
	}
	finish = clock();
	printf("\t(%f)\n", (double)(finish - start) / CLOCKS_PER_SEC);

	printf("TEST ALLOC/FREE plus SIZE:");
	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		tr[i].ptr = gAllocator.alloc(size);
		tr[i].size = size;
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		gAllocator.free(tr[j].ptr, tr[j].size);
		tr[j].ptr = tr[i].ptr;
		tr[j].size = tr[i].size;
	}
	finish = clock();
	printf("\t\t\t(%f)", (double)(finish - start) / CLOCKS_PER_SEC);

	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		tr[i].ptr = malloc(size);
		tr[i].size = size;
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		free(tr[j].ptr);
		tr[j].ptr = tr[i].ptr;
		tr[j].size = tr[i].size;
	}
	finish = clock();
	printf("\t(%f)\n", (double)(finish - start) / CLOCKS_PER_SEC);

	printf("TEST ALLOC/FREE with ALIGNMENT:");
	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		size_t alignment = rand_alignment();
		tr[i].ptr = gAllocator.alloc(size, alignment);
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		gAllocator.free(tr[j].ptr);
		tr[j].ptr = tr[i].ptr;
	}
	finish = clock();
	printf("\t\t\t(%f)", (double)(finish - start) / CLOCKS_PER_SEC);

	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		size_t alignment = rand_alignment();
		tr[i].ptr = _aligned_malloc(size, alignment);
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		_aligned_free(tr[j].ptr);
		tr[j].ptr = tr[i].ptr;
	}
	finish = clock();
	printf("\t(%f)\n", (double)(finish - start) / CLOCKS_PER_SEC);

	printf("TEST ALLOC/FREE with ALIGNMENT plus SIZE:");
	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		size_t alignment = rand_alignment();
		tr[i].ptr = gAllocator.alloc(size, alignment);
		tr[i].size = size;
		tr[i].alignment = alignment;
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		gAllocator.free(tr[j].ptr, tr[j].size, tr[j].alignment);
		tr[j].ptr = tr[i].ptr;
		tr[j].size = tr[i].size;
		tr[j].alignment = tr[i].alignment;
	}
	finish = clock();
	printf("\t(%f)", (double)(finish - start) / CLOCKS_PER_SEC);

	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		size_t alignment = rand_alignment();
		tr[i].ptr = _aligned_malloc(size, alignment);
		tr[i].size = size;
		tr[i].alignment = alignment;
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		_aligned_free(tr[j].ptr);
		tr[j].ptr = tr[i].ptr;
		tr[j].size = tr[i].size;
		tr[j].alignment = tr[i].alignment;
	}
	finish = clock();
	printf("\t(%f)\n", (double)(finish - start) / CLOCKS_PER_SEC);

	printf("TEST REALLOC:");
	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		tr[i].ptr = gAllocator.realloc(NULL, size);
	}
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		tr[i].ptr = gAllocator.realloc(tr[i].ptr, size);
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		gAllocator.realloc(tr[j].ptr, 0);
		tr[j].ptr = tr[i].ptr;
	}
	finish = clock();
	printf("\t\t\t\t\t(%f)", (double)(finish - start) / CLOCKS_PER_SEC);

	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		tr[i].ptr = realloc(NULL, size);
	}
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		tr[i].ptr = realloc(tr[i].ptr, size);
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		realloc(tr[j].ptr, 0);
		tr[j].ptr = tr[i].ptr;
	}
	finish = clock();
	printf("\t(%f)\n", (double)(finish - start) / CLOCKS_PER_SEC);

	printf("TEST REALLOC with ALIGNMENT:");
	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		size_t alignment = rand_alignment();
		tr[i].ptr = gAllocator.realloc(NULL, size, alignment);
	}
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		size_t alignment = rand_alignment();
		tr[i].ptr = gAllocator.realloc(tr[i].ptr, size, alignment);
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		gAllocator.realloc(tr[j].ptr, 0, 0);
		tr[j].ptr = tr[i].ptr;
	}
	finish = clock();
	printf("\t\t\t(%f)", (double)(finish - start) / CLOCKS_PER_SEC);

	srand(1234);
	start = clock();
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		size_t alignment = rand_alignment();
		tr[i].ptr = _aligned_realloc(NULL, size, alignment);
	}
	for (unsigned i = 0; i < N; i++) {
		size_t size = rand_size();
		size_t alignment = rand_alignment();
		tr[i].ptr = _aligned_realloc(tr[i].ptr, size, alignment);
	}
	for (unsigned i = 0; i < N; i++) {
		unsigned j = i + rand() % (N - i);
		_aligned_realloc(tr[j].ptr, 0, 0);
		tr[j].ptr = tr[i].ptr;
	}
	finish = clock();
	printf("\t(%f)\n", (double)(finish - start) / CLOCKS_PER_SEC);
}

// for the benchmark to work properly we have to switch off global new/delete overrides
// another reason why it's sucha a PITA to work with
//#define USE_GLOBAL_NEW_DELETE_OVERRIDE
#ifdef USE_GLOBAL_NEW_DELETE_OVERRIDE
// GLOBAL NEW/DELETE OVERRIDES
// PROS:
// 1) integrates in 5 seconds! watch out for nothrow_t versions
// CONS: 
// 1) no-no for middleware
// 2) no way to get "delete" with size parameter
// 3) tricky with DLLs
// 4) no way to get alignment information
// 5) everything uses the allocator even stuff you don't want to 
//   (seriously only if I get 5c every time I catch a middleware memory leak ...)
inline void* operator new(size_t size) {
	return gAllocator.alloc(size);
}
inline void* operator new(size_t size, const std::nothrow_t&) {
	return gAllocator.alloc(size);
}
inline void operator delete(void* ptr) {
	gAllocator.free(ptr);
}
inline void operator delete(void* ptr, const std::nothrow_t&) {
	gAllocator.free(ptr);
}
#endif //USE_GLOBAL_NEW_DELETE_OVERRIDE

// PER CLASS OPERATOR NEW/DELETE
// just inherit to use
// PROS:
// 1) more control over allocations
// 2) "delete" with size parameter
// CONS:
// 1) have to inherit or copy/paste operators
// 2) try doing placement new, nothrow new, any extra parameter new!
// 3) how does C++ know the size on "delete" ... even with virtual multiple inheritance
//    secret return value from top level dtor? extra vtable entry? special virtual delete operator?
//    only if we had general access to it that would be great! typeid().size() ftw?!
struct operator_new_delete {
	void* operator new(size_t size) {
		return gAllocator.alloc(size);
	}
	void operator delete(void* ptr, size_t size) {
		gAllocator.free(ptr, size);
	}
};

// CUSTOM NEW/DELETE TEMPLATE FUNCTIONS
// PROS: 
// 1) super flexible, "Just works"
// 2) inlines like a champion
// 3) has access to both alignment and size information
// CONS:
// 1) needs dynamic_cast to properly work
// 2) damn that's a lot of code! But it's all needed.

// special tagged placement new so if a ctor throws we can correctly free the memory
struct custom_tag {};
inline void* operator new(size_t, void* ptr, custom_tag) {
	return ptr;
}
inline void operator delete(void* ptr, void*, custom_tag) {
	gAllocator.free(ptr);
}

// support for up to 4 ctor parameters
// with C++0x and variadic templates this can be simplified to 1 function
template<class T> 
inline T* custom_new() {
	void* p = gAllocator.alloc(sizeof(T));
	return new (p, custom_tag()) T();
}
template<class T, class P1> 
inline T* custom_new(const P1& p1) {
	void* p = gAllocator.alloc(sizeof(T));
	return new (p, custom_tag()) T(p1);
}
template<class T, class P1, class P2> 
inline T* custom_new(const P1& p1, const P2& p2) {
	void* p = gAllocator.alloc(sizeof(T));
	return new (p, custom_tag()) T(p1, p2);
}
template<class T, class P1, class P2, class P3> 
inline T* custom_new(const P1& p1, const P2& p2, const P3& p3) {
	void* p = gAllocator.alloc(sizeof(T));
	return new (p, custom_tag()) T(p1, p2, p3);
}
template<class T, class P1, class P2, class P3, class P4> 
inline T* custom_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4) {
	void* p = gAllocator.alloc(sizeof(T));
	return new (p, custom_tag()) T(p1, p2, p3, p4);
}

// helper template to guess whether a class is polymorphic or not
template<class T> struct is_polymorphic_helper : public T {
	virtual ~is_polymorphic_helper();
};
template<class T> struct is_polymorphic { enum { value = sizeof(T) == sizeof(is_polymorphic_helper<T>) }; };
template<class T> struct is_polymorphic<T*> { enum {value = false}; };
template<class T> struct is_polymorphic<const T*> { enum {value = false}; };
template<class T> struct is_polymorphic<volatile T*> { enum {value = false}; };
template<class T> struct is_polymorphic<volatile const T*> { enum {value = false}; };
template<> struct is_polymorphic<bool> { enum {value = false}; };
template<> struct is_polymorphic<wchar_t> { enum {value = false}; };
template<> struct is_polymorphic<char> { enum {value = false}; };
template<> struct is_polymorphic<unsigned char> { enum {value = false}; };
template<> struct is_polymorphic<signed char> { enum {value = false}; };
template<> struct is_polymorphic<short> { enum {value = false}; };
template<> struct is_polymorphic<unsigned short> { enum {value = false}; };
template<> struct is_polymorphic<int> { enum {value = false}; };
template<> struct is_polymorphic<unsigned int> { enum {value = false}; };
template<> struct is_polymorphic<long> { enum {value = false}; };
template<> struct is_polymorphic<unsigned long> { enum {value = false}; };
template<> struct is_polymorphic<long long> { enum {value = false}; };
template<> struct is_polymorphic<unsigned long long> { enum {value = false}; };
template<> struct is_polymorphic<float> { enum {value = false}; };
template<> struct is_polymorphic<double> { enum {value = false}; };
template<> struct is_polymorphic<long double> { enum {value = false}; };

// dispatch the delete to the correct free function
// for polymorhic objects we need to use dynamic_cast to find the pointer to the master object
// it seems a waste especially because the dtor actually knows it but unfortunately this is the best we can do
// SMALL GOTCHA: dynamic_cast must be called before the dtor because the vtbl gets destroyed by the dtor
template<bool> struct polymorphic_tag {};
template<class T> 
inline void custom_delete(T* ptr, polymorphic_tag<true>) {
	if (ptr) {
		void* basePtr = dynamic_cast<void*>(ptr);
		ptr->~T();
		gAllocator.free(basePtr);
	}
}
template<class T> 
inline void custom_delete(T* ptr, polymorphic_tag<false>) {
	if (ptr) {
		ptr->~T();
		gAllocator.free(ptr, sizeof(T));
	}
}
template<class T> 
inline void custom_delete(T* ptr) {
	custom_delete(ptr, polymorphic_tag<is_polymorphic<T>::value>());
}

void benchmark2() {
	clock_t start, finish;

	printf("TEST CUSTOM NEW/DELETE:");
	start = clock();
	for (int i = 0; i < N; i++) {
		int* pi1 = custom_new<int>(i);
		custom_delete(pi1);
		int* pi2 = custom_new<int>(i);
		custom_delete(pi2);
		int* pi3 = custom_new<int>(i);
		custom_delete(pi3);
		int* pi4 = custom_new<int>(i);
		custom_delete(pi4);
	}
	finish = clock();
	printf("\t\t\t\t(%f)", (double)(finish - start) / CLOCKS_PER_SEC);

	start = clock();
	for (int i = 0; i < N; i++) {
		int* pi1 = new int(i);
		delete pi1;
		int* pi2 = new int(i);
		delete pi2;
		int* pi3 = new int(i);
		delete pi3;
		int* pi4 = new int(i);
		delete pi4;
	}
	finish = clock();
	printf("\t(%f)\n", (double)(finish - start) / CLOCKS_PER_SEC);
}

int main() {
	benchmark1();
	benchmark2();

	return 0;
}
