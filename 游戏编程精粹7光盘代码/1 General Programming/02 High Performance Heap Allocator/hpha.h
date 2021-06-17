// Copyright (c) 2007, Dimitar Lazarov, Luxoflux
// dimitar.lazarov@usa.net

#ifndef _HPHA_H_
#define _HPHA_H_

#include <memory.h>
#include <string.h>
#include <assert.h>
#include <new>

#if defined(WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT WINVER
#endif
#include <windows.h>
#else
#error "Platform undefined or not supported"
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define DEBUG_ALLOCATOR
//#define DEBUG_MULTI_RBTREE
#endif

#define MULTITHREADED

//////////////////////////////////////////////////////////////////////////
// on Win32 we use CRITICAL_SECTION
// if atomic operations are available, 
// the small allocator can simply use a spinlock
#ifdef MULTITHREADED
class mutex {
private:
	mutex(const mutex&);
	mutex& operator=(const mutex&);

	#if defined(WIN32)
	CRITICAL_SECTION mCS;
	void create() {
		InitializeCriticalSection(&mCS);
	}
	void create(size_t spinCount) {
		InitializeCriticalSectionAndSpinCount(&mCS, (DWORD)spinCount);
	}
	void destoy() {
		DeleteCriticalSection(&mCS);
	}
	#else
	#error "Platform undefined or not supported"
	#endif

public:
	mutex() {
		create();
	}
	explicit mutex(size_t spinCount) {
		create(spinCount);
	}
	~mutex() {
		destoy();
	}

	#if defined(WIN32)
	void lock() {
		EnterCriticalSection(&mCS);
	}
	void unlock() {
		LeaveCriticalSection(&mCS);
	}
	#else
	#error "Platform undefined or not supported"
	#endif

	class scoped {
	private:
		mutex& mLWM;
		scoped(const scoped &);
		scoped& operator=(const scoped &);
	public:
		explicit scoped(mutex& lwm): mLWM(lwm) {
			mLWM.lock();
		}
		~scoped() {
			mLWM.unlock();
		}
	};
};
#endif // MULTITHREADED

//////////////////////////////////////////////////////////////////////////
// on Win32 we use the virtual memory system to get large pages from the OS
// virtual memory is not necessary for the correctness of the algorithm
#if defined(WIN32)
const size_t VIRTUAL_PAGE_SIZE_LOG2 = 16;
const size_t VIRTUAL_PAGE_SIZE  = (size_t)1 << VIRTUAL_PAGE_SIZE_LOG2;
#else
#error "Platform undefined or not supported"
#endif

inline void* virtual_alloc(size_t size) {
	assert(size / VIRTUAL_PAGE_SIZE * VIRTUAL_PAGE_SIZE == size);
	return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
}
inline void virtual_free(void* addr) {
	BOOL ret = VirtualFree(addr, 0, MEM_RELEASE);
	assert(ret);
}

//////////////////////////////////////////////////////////////////////////
template<class T> inline T round_down(T x, size_t a) {return x & -(int)a;}
template<class T> inline T round_up(T x, size_t a) {return (x + (a-1)) & -(int)a;}
template<class T> inline T* align_down(T* p, size_t a) {return (T*)((size_t)p & -(int)a);}
template<class T> inline T* align_up(T* p, size_t a) {return (T*)(((size_t)p + (a-1)) & -(int)a);}

//////////////////////////////////////////////////////////////////////////
class intrusive_list_base {
public:
	class node_base {
		node_base* mPrev;
		node_base* mNext;
	public:
		node_base* next() const {return mNext;}
		node_base* prev() const {return mPrev;}
		void reset() {
			mPrev = this;
			mNext = this;
		}
		void unlink() {
			mNext->mPrev = mPrev;
			mPrev->mNext = mNext;
		}
		void link(node_base* node) {
			mPrev = node->mPrev;
			mNext = node;
			node->mPrev = this;
			mPrev->mNext = this;
		}
	};
	intrusive_list_base() {
		mHead.reset();
	}
	intrusive_list_base(const intrusive_list_base&) {
		mHead.reset();
	}
	bool empty() const {return mHead.next() == &mHead;}
	void swap(intrusive_list_base& other) {
		node_base* node = &other.mHead;
		if (!empty()) {
			node = mHead.next();
			mHead.unlink();
			mHead.reset();
		}
		node_base* other_node = &mHead;
		if (!other.empty()) {
			other_node = other.mHead.next();
			other.mHead.unlink();
			other.mHead.reset();
		}
		mHead.link(other_node);
		other.mHead.link(node);
	}
protected:
	node_base mHead;
};

//////////////////////////////////////////////////////////////////////////
template<class T> class intrusive_list : public intrusive_list_base {
	intrusive_list(const intrusive_list& rhs);
	intrusive_list& operator=(const intrusive_list& rhs);
public:
	class node : public node_base {
	public:
		T* next() const {return static_cast<T*>(node_base::next());}
		T* prev() const {return static_cast<T*>(node_base::prev());}
		const T& data() const {return *static_cast<const T*>(this);}
		T& data() {return *static_cast<T*>(this);}
	};

	class const_iterator;
	class iterator {
		typedef T& reference;
		typedef T* pointer;
		friend class const_iterator;
		T* mPtr;
	public:
		iterator() : mPtr(0) {}
		explicit iterator(T* ptr) : mPtr(ptr) {}
		reference operator*() const {return mPtr->data();}
		pointer operator->() const {return &mPtr->data();}
		operator pointer() const {return &mPtr->data();}
		iterator& operator++() {
			mPtr = mPtr->next();
			return *this;
		}
		iterator& operator--() {
			mPtr = mPtr->prev();
			return *this;
		}
		bool operator==(const iterator& rhs) const {return mPtr == rhs.mPtr;}
		bool operator!=(const iterator& rhs) const {return mPtr != rhs.mPtr;}
		T* ptr() const {return mPtr;}
	};

	class const_iterator {
		typedef const T& reference;
		typedef const T* pointer;
		const T* mPtr;
	public:
		const_iterator() : mPtr(0) {}
		explicit const_iterator(const T* ptr) : mPtr(ptr) {}
		const_iterator(const iterator& it) : mPtr(it.mPtr) {}
		reference operator*() const {return mPtr->data();}
		pointer operator->() const {return &mPtr->data();}
		operator pointer() const {return &mPtr->data();}
		const_iterator& operator++() {
			mPtr = mPtr->next();
			return *this;
		}
		const_iterator& operator--() {
			mPtr = mPtr->prev();
			return *this;
		}
		bool operator==(const const_iterator& rhs) const {return mPtr == rhs.mPtr;}
		bool operator!=(const const_iterator& rhs) const {return mPtr != rhs.mPtr;}
		const T* ptr() const {return mPtr;}
	};

	intrusive_list() : intrusive_list_base() {}
	~intrusive_list() {clear();}

	const_iterator begin() const {return const_iterator((const T*)mHead.next());}
	iterator begin() {return iterator((T*)mHead.next());}
	const_iterator end() const {return const_iterator((const T*)&mHead);}
	iterator end() {return iterator((T*)&mHead);}

	const T& front() const {
		assert(!empty());
		return *begin();
	}
	T& front() {
		assert(!empty());
		return *begin();
	}
	const T& back() const {
		assert(!empty());
		return *(--end());
	}
	T& back() {
		assert(!empty());
		return *(--end());
	}

	void push_front(T* v) {insert(this->begin(), v);}
	void pop_front() {erase(this->begin());}
	void push_back(T* v) {insert(this->end(), v);}
	void pop_back() {erase(--(this->end()));}

	iterator insert(iterator where, T* node) {
		T* newLink = node;
		newLink->link(where.ptr());
		return iterator(newLink);
	}
	iterator erase(iterator where) {
		T* node = where.ptr();
		++where;
		node->unlink();
		return where;
	}
	void erase(T* node) {
		node->unlink();
	}
	void clear() {
		while (!this->empty()) {
			this->pop_back();
		}
	}
};

//////////////////////////////////////////////////////////////////////////
// template class to store bits of information 
// in the least significant bits of a pointer
template<class T> struct ptr_bits_traits {
	typedef T& reference;
};
template<> struct ptr_bits_traits<void> {
	typedef void reference;
};

template<class T, size_t BITS> class ptr_bits {
	enum {BITMASK = (1 << BITS)-1};
	T* mPtr;
public:
	typedef typename ptr_bits_traits<T>::reference reference;
	ptr_bits() : mPtr(0) {}
	ptr_bits(T* ptr) : mPtr(ptr) {
		assert(((size_t)ptr & BITMASK) == 0);
	}
	ptr_bits(T* ptr, size_t bits) : mPtr((T*)((size_t)ptr | bits)) {
		assert(((size_t)ptr & BITMASK) == 0);
		assert((bits & ~BITMASK) == 0);
	}
	ptr_bits& operator=(const T* ptr) {
		assert(((size_t)ptr & BITMASK) == 0);
		size_t bits = (size_t)mPtr & BITMASK;
		mPtr = (T*)((size_t)ptr | bits);
		return *this;
	}
	ptr_bits(const ptr_bits& rhs) : mPtr(rhs) {
		assert(((size_t)mPtr & BITMASK) == 0);
	}
	ptr_bits& operator=(const ptr_bits& rhs) {
		const T* ptr = rhs;
		assert(((size_t)ptr & BITMASK) == 0);
		size_t bits = (size_t)mPtr & BITMASK;
		mPtr = (T*)((size_t)ptr | bits);
		return *this;
	}
	size_t get_bits() const {
		return (size_t)mPtr & BITMASK;
	}
	void set_bits(size_t bits = BITMASK) {
		assert((bits & ~BITMASK) == 0);
		mPtr = (T*)(((size_t)mPtr & ~BITMASK) | bits);
	}
	void clear_bits() {
		mPtr = (T*)((size_t)mPtr & ~BITMASK);
	}
	void swap_bits(ptr_bits& rhs) {
		size_t bits = get_bits();
		set_bits(rhs.get_bits());
		rhs.set_bits(bits);
	}
	template<size_t BIT> bool get_bit() const {
		size_t bits = (size_t)mPtr & BITMASK;
		return !!(bits & (1 << BIT));
	}
	template<size_t BIT> void set_bit() {
		mPtr = (T*)((size_t)mPtr | (1 << BIT));
	}
	template<size_t BIT> void clear_bit() {
		mPtr = (T*)((size_t)mPtr & ~(1 << BIT));
	}
	reference operator*() const {
		assert((size_t)mPtr & ~BITMASK);
		return *(T*)((size_t)mPtr & ~BITMASK);
	}
	T* operator->() const {
		assert((size_t)mPtr & ~BITMASK);
		return (T*)((size_t)mPtr & ~BITMASK);
	}
	operator T*() const {
		return (T*)((size_t)mPtr & ~BITMASK);
	}
};

//////////////////////////////////////////////////////////////////////////
class intrusive_multi_rbtree_base {
public:
	enum side {LEFT = 0, RIGHT = 1};
	enum colour {BLACK = 0, RED = 1};

	class node_base {
		enum {BIT_RB, BIT_PS, NUM_BITS};
		node_base* mChildren[2];
		node_base* mNeighbours[2];
		ptr_bits<node_base,NUM_BITS> mParent;
	public:
		node_base() {
			mChildren[LEFT] = this;
			mChildren[RIGHT] = this;
			mNeighbours[LEFT] = this;
			mNeighbours[RIGHT] = this;
			mParent = this;
		}
		node_base* parent() const {return mParent;}
		node_base* child(side s) const {return mChildren[s];}
		node_base* left() const {return mChildren[LEFT];}
		node_base* right() const {return mChildren[RIGHT];}
		node_base* neighbour(side s) const {return mNeighbours[s];}
		node_base* prev() const {return mNeighbours[LEFT];}
		node_base* next() const {return mNeighbours[RIGHT];}
		bool chained() const {return mNeighbours[LEFT] != this;}
		bool head() const {return !!mParent;}
		side parent_side() const {return (side)(mParent.get_bits() >> BIT_PS);}
		void set_parent_side(side s) {mParent.set_bits((mParent.get_bits() & ~(1 << BIT_PS)) | (s << BIT_PS));}
		bool red() const {return mParent.get_bit<BIT_RB>();}
		bool black() const {return !mParent.get_bit<BIT_RB>();}
		colour red_black() const {return (colour)(mParent.get_bits() & (1 << BIT_RB));}
		void make_red() {mParent.set_bit<BIT_RB>();}
		void make_black() {mParent.clear_bit<BIT_RB>();}
		void make_red_black(colour rb) {mParent.set_bits((mParent.get_bits() & ~(1 << BIT_RB)) | (rb << BIT_RB));}
		bool nil() const {return this == mChildren[RIGHT];}
		void rotate(side s) {
			assert(mParent->mChildren[parent_side()] == this);
			side o = (side)(1 - s);
			side ps = parent_side();
			node_base* top = mChildren[o];
			mChildren[o] = top->mChildren[s];
			mChildren[o]->mParent = this;
			mChildren[o]->set_parent_side(o);
			top->mParent = mParent;   
			top->set_parent_side(ps);
			mParent->mChildren[ps] = top;
			top->mChildren[s] = this;
			mParent = top;
			set_parent_side(s);
		}
		node_base* pred_or_succ(side s) const {
			node_base* cur = const_cast<node_base*>(this);
			cur = cur->mNeighbours[s];
			if (!cur->mParent)
				return cur;
			node_base* xessor = cur->mChildren[s];
			if (!xessor->nil()) {
				side o = (side)(1 - s);
				while (!xessor->mChildren[o]->nil())
					xessor = xessor->mChildren[o];
			} else {
				assert(!cur->nil());
				xessor = cur->mParent;
				while (cur->parent_side() == s) {
					assert(!xessor->nil());
					cur = xessor;
					xessor = xessor->mParent;
				}
			}
			return xessor;
		}
		node_base* pred() const {return pred_or_succ(LEFT);}
		node_base* succ() const {return pred_or_succ(RIGHT);}
		node_base* min_or_max(side s) const {
			node_base* cur = const_cast<node_base*>(this);
			node_base* minmax = cur;
			while (!cur->nil()) {
				minmax = cur;
				cur = cur->mChildren[s];
			}
			return minmax;
		}
		node_base* minimum() const {return min_or_max(LEFT);}
		node_base* maximum() const {return min_or_max(RIGHT);}
	protected:
		template<class T> friend class intrusive_multi_rbtree;
		void attach_to(node_base* parent, side s) {
			mNeighbours[LEFT] = this;
			mNeighbours[RIGHT] = this;
			mChildren[LEFT] = parent->child(s);
			mChildren[RIGHT] = parent->child(s);
			mParent = parent;
			set_parent_side(s);
			parent->mChildren[s] = this;
			make_red();
		}
		void substitute_with(node_base* child) {
			side ps = parent_side();
			child->mParent = mParent;
			child->set_parent_side(ps);
			mParent->mChildren[ps] = child;
		}
		void switch_with(node_base* node) {
			assert(this != node);
			assert(node->head());
			side nps = node->parent_side();
			mChildren[LEFT] = node->child(LEFT);
			mChildren[RIGHT] = node->child(RIGHT);
			mParent = node->mParent;
			set_parent_side(nps);
			node->child(LEFT)->mParent = this;
			node->child(LEFT)->set_parent_side(LEFT);
			node->child(RIGHT)->mParent = this;
			node->child(RIGHT)->set_parent_side(RIGHT);
			node->mParent->mChildren[nps] = this; 
			make_red_black(node->red_black());
		}
		void unlink() {
			mNeighbours[RIGHT]->mNeighbours[LEFT] = mNeighbours[LEFT];
			mNeighbours[LEFT]->mNeighbours[RIGHT] = mNeighbours[RIGHT];
		}
		void link(node_base* node) {
			mNeighbours[LEFT] = node->mNeighbours[LEFT];
			mNeighbours[RIGHT] = node;
			mNeighbours[RIGHT]->mNeighbours[LEFT] = this;
			mNeighbours[LEFT]->mNeighbours[RIGHT] = this;
			mChildren[LEFT] = NULL;
			mChildren[RIGHT] = NULL;
			mParent = NULL;
			set_parent_side(LEFT);
			make_red();
		}
	};

	intrusive_multi_rbtree_base() {}
	intrusive_multi_rbtree_base(const intrusive_multi_rbtree_base&) {}
	bool empty() const {return mHead.child(LEFT) == &mHead;}
	#ifdef DEBUG_MULTI_RBTREE
	void check() const;
	#endif

protected:
	node_base mHead;
	void insert_fixup(node_base* node);
	void erase_fixup(node_base* node);
private:
	#ifdef DEBUG_MULTI_RBTREE
	unsigned check_height(node_base* node) const;
	#endif
};

template<class T> class intrusive_multi_rbtree : public intrusive_multi_rbtree_base {
	intrusive_multi_rbtree(const intrusive_multi_rbtree& rhs);
	intrusive_multi_rbtree& operator=(const intrusive_multi_rbtree& rhs);
public:
	class node : public node_base {
	public:
		const T& data() const {return *(T*)this;}
		T& data() {return *(T*)this;}
		T* parent() const {return (T*)node_base::parent();}
		T* child(side s) const {return (T*)node_base::child(s);}
		T* left() const {return (T*)node_base::left();}
		T* right() const {return (T*)node_base::right();}
		T* neighbour(side s) const {return (T*)node_base::neighbour(s);}
		T* next() const {return (T*)node_base::next();}
		T* prev() const {return (T*)node_base::prev();}
		T* pred_or_succ(side s) const {return (T*)node_base::pred_or_succ(s);}
		T* pred() const {return (T*)node_base::pred();}
		T* succ() const {return (T*)node_base::succ();}
		T* min_or_max(side s) const {return (T*)node_base::min_or_max(s);}
		T* minimum() const {return (T*)node_base::minimum();}
		T* maximum() const {return (T*)node_base::maximum();}
	};

	class const_iterator;
	class iterator {
		typedef T& reference;
		typedef T* pointer;
		friend class const_iterator;
		T* mPtr;
	public:
		iterator() : mPtr(0) {}
		explicit iterator(T* ptr) : mPtr(ptr) {}
		reference operator*() const {return mPtr->data();}
		pointer operator->() const {return &mPtr->data();}
		operator pointer() const {return &mPtr->data();}
		iterator& operator++() {
			mPtr = mPtr->succ();
			return *this;
		}
		iterator& operator--() {
			mPtr = mPtr->pred();
			return *this;
		}
		bool operator==(const iterator& rhs) const {return mPtr == rhs.mPtr;}
		bool operator!=(const iterator& rhs) const {return mPtr != rhs.mPtr;}
		T* ptr() const {return mPtr;}
	};

	class const_iterator {
		typedef const T& reference;
		typedef const T* pointer;
		const T* mPtr;
	public:
		const_iterator() : mPtr(0) {}
		explicit const_iterator(const T* ptr) : mPtr(ptr) {}
		const_iterator(const iterator& it) : mPtr(it.mPtr) {}
		reference operator*() const {return mPtr->data();}
		pointer operator->() const {return &mPtr->data();}
		operator pointer() const {return &mPtr->data();}
		const_iterator& operator++() {
			mPtr = mPtr->succ();
			return *this;
		}
		const_iterator& operator--() {
			mPtr = mPtr->pred();
			return *this;
		}
		bool operator==(const const_iterator& rhs) const {return mPtr == rhs.mPtr;}
		bool operator!=(const const_iterator& rhs) const {return mPtr != rhs.mPtr;}
		const T* ptr() const {return mPtr;}
	};

	template<class K> iterator lower_bound(const K& key) {return iterator(do_lower_bound(key));}
	template<class K> const_iterator lower_bound(const K& key) const {return const_iterator(do_lower_bound(key));}
	template<class K> iterator upper_bound(const K& key) {return iterator(do_upper_bound(key));}
	template<class K> const_iterator upper_bound(const K& key) const {return const_iterator(do_upper_bound(key));}
	template<class K> iterator find(const K& key) { 
		T* found = do_lower_bound(key); 
		if (found == nil_multi_rbnode() || found->data() > key)
			return end();
		return iterator(found);
	}
	template<class K> const_iterator find(const K& key) const {
		T* found = do_lower_bound(key);
		if (found == nil_multi_rbnode() || found->data() > key)
			return end();
		return const_iterator(found);
	}

	const_iterator begin() const {return minimum();}
	iterator begin() {return minimum();}
	const_iterator end() const {return const_iterator(nil_multi_rbnode());}
	iterator end() {return iterator(nil_multi_rbnode());}
	const_iterator root() const {return const_iterator(root_multi_rbnode());}
	iterator root() {return iterator(root_multi_rbnode());}
	const_iterator minimum() const {return const_iterator(root_multi_rbnode()->minimum());}
	iterator minimum() {return iterator(root_multi_rbnode()->minimum());}
	const_iterator maximum() const {return const_iterator(root_multi_rbnode()->maximum());}
	iterator maximum() {return iterator(root_multi_rbnode()->maximum());}

	intrusive_multi_rbtree() : intrusive_multi_rbtree_base() {}
	~intrusive_multi_rbtree() {clear();}
	iterator insert(T* node) {
		this->do_insert(node);
		return iterator(node);
	}
	iterator erase(iterator where) {
		T* node = where.ptr();
		++where;
		this->do_erase(node);
		return where;
	}
	void erase(T* node) {
		this->do_erase(node);
	}
	void clear() {
		for (iterator it = this->begin(); it != this->end();)
			it = this->erase(it);
	}
protected:
	T* nil_multi_rbnode() const {return (T*)&mHead;}
	T* root_multi_rbnode() const {return (T*)mHead.child(LEFT);}
	template<class K> T* do_lower_bound(const K& key) const {
		T* endNode = nil_multi_rbnode();
		T* bestNode = nil_multi_rbnode();
		T* curNode = root_multi_rbnode();
		while (curNode != endNode) {
			if (curNode->data() < key)
				curNode = curNode->child(RIGHT);
			else {
				bestNode = curNode;
				curNode = curNode->child(LEFT);
			}
		}
		return bestNode;
	}
	template<class K> T* do_upper_bound(const K& key) const {
		T* endNode = nil_multi_rbnode();
		T* bestNode = nil_multi_rbnode();
		T* curNode = root_multi_rbnode();
		while (curNode != endNode) {
			if (curNode->data() > key) {
				bestNode = curNode;
				curNode = curNode->child(LEFT);
			} else
				curNode = curNode->child(RIGHT);
		}
		return bestNode;
	}
	void do_insert(T* node) {
		T* endNode = nil_multi_rbnode();
		T* lastNode = nil_multi_rbnode();
		T* curNode = root_multi_rbnode();
		side s = LEFT;
		while (curNode != endNode) {
			lastNode = curNode;
			s = RIGHT;
			if (node->data() < curNode->data()) {
				s = LEFT;
			} else if (!(node->data() > curNode->data())) {
				node->link(curNode);
				return;
			}
			curNode = curNode->child(s);
		}
		node->attach_to(lastNode, s);
		insert_fixup(node);
		#ifdef DEBUG_MULTI_RBTREE
		check();
		#endif
	}
	void do_erase(T* node) {
		if (node->chained()) {
			if (!node->head()) {
				assert(node->child(LEFT) == NULL);
				assert(node->child(RIGHT) == NULL);
				node->unlink();
				return;
			}
			T* repl = node->next();
			assert(repl != nil_multi_rbnode());
			assert(!repl->parent());
			assert(repl->child(LEFT) == NULL);
			assert(repl->child(RIGHT) == NULL);
			repl->switch_with(node);
			node->unlink();
			return;
		}
		T* endNode = nil_multi_rbnode();
		T* repl = node;
		side s = LEFT;
		if (node->child(RIGHT) != endNode) {
			if (node->child(LEFT) != endNode) {
				repl = node->child(RIGHT);
				while (repl->child(LEFT) != endNode)
					repl = repl->child(LEFT);
			}
			s = RIGHT;
		}
		assert(repl->child((side)(1-s)) == endNode);
		bool red = repl->red();
		T* replChild = repl->child(s);
		repl->substitute_with(replChild);
		if (repl != node)
			repl->switch_with(node);
		if (!red) 
			erase_fixup(replChild);
		#ifdef DEBUG_MULTI_RBTREE
		check();
		#endif
	}
};

#ifdef DEBUG_ALLOCATOR
//////////////////////////////////////////////////////////////////////////
template<class T> class virtual_book {
	struct page {
		enum {BIT_NIL, NUM_BITS};
		ptr_bits<page,NUM_BITS> mPrev;
		page* mNext;
		page() {
			mPrev = this;
			mNext = this;
		}
		bool nil() const {return mPrev.get_bit<BIT_NIL>();}
		void set_nil() {
			mPrev.set_bit<BIT_NIL>();
		}
		void reset() {
			mPrev = this;
			mNext = this;
		}
		void unlink() {
			mNext->mPrev = mPrev;
			mPrev->mNext = mNext;
		}
		void link(page* nextPage) {
			mPrev = nextPage->mPrev;
			mNext = nextPage;
			nextPage->mPrev = this;
			mPrev->mNext = this;
		}
		void* mem() const {
			return align_down((void*)this, VIRTUAL_PAGE_SIZE);
		}
		T* first() const {
			return (T*)align_down((void*)this, VIRTUAL_PAGE_SIZE);
		}
		T* last() const {
			return first() + (VIRTUAL_PAGE_SIZE-sizeof(page))/sizeof(T);
		}
	};

	static inline page* get_page(void* ptr) {
		return (page*)(align_down((char*)ptr + VIRTUAL_PAGE_SIZE, VIRTUAL_PAGE_SIZE) - sizeof(page));
	}

	bool grow() {
		void* mem = virtual_alloc(VIRTUAL_PAGE_SIZE);
		if (!mem)
			return false;
		page* newPage = get_page(mem);
		newPage->link(&mHead);
		mFirst = iterator(mHead.mNext->first());
		if (mLast == mLastReserved)
			mLast = iterator(newPage->first());
		mLastReserved = iterator(newPage->last());
		return true;
	}

public:
	class const_iterator;
	class iterator {
		typedef T& reference;
		typedef T* pointer;
		friend class const_iterator;
		T* mPtr;
	public:
		iterator() : mPtr(0) {}
		explicit iterator(T* ptr) : mPtr(ptr) {}
		reference operator*() const {return *mPtr;}
		pointer operator->() const {return mPtr;}
		operator pointer() const {return mPtr;}
		iterator& operator++() {
			page* curPage = get_page(mPtr);
			T* last = curPage->last();
			assert(mPtr < last);
			++mPtr;
			if (mPtr == last) {
				page* nextPage = curPage->mNext;
				if (!nextPage->nil())
					mPtr = nextPage->first();
			}
			return *this;
		}
		iterator& operator--() {
			page* curPage = get_page(mPtr);
			T* first = curPage->first();
			if (mPtr == first) {
				page* prevPage = curPage->mPrev;
				assert(!prevPage->nil());
				mPtr = prevPage->last();
			}
			--mPtr;
			return *this;
		}
		bool operator==(const iterator& rhs) const {return mPtr == rhs.mPtr;}
		bool operator!=(const iterator& rhs) const {return mPtr != rhs.mPtr;}
		T* ptr() const {return mPtr;}
	};
	class const_iterator {
		typedef const T& reference;
		typedef const T* pointer;
		const T* mPtr;
	public:
		const_iterator() : mPtr(0) {}
		explicit const_iterator(const T* ptr) : mPtr(ptr) {}
		const_iterator(const iterator& it) : mPtr(it.mPtr) {}
		reference operator*() const {return *mPtr;}
		pointer operator->() const {return mPtr;}
		operator pointer() const {return mPtr;}
		const_iterator& operator++() {
			page* curPage = get_page(mPtr);
			T* last = curPage->last();
			assert(mPtr < last);
			++mPtr;
			if (mPtr == last) {
				page* nextPage = curPage->mNext;
				if (!nextPage->nil())
					mPtr = nextPage->first();
			}
			return *this;
		}
		const_iterator& operator--() {
			page* curPage = get_page(mPtr);
			T* first = curPage->first();
			if (mPtr == first) {
				page* prevPage = curPage->mPrev;
				assert(!prevPage->nil());
				mPtr = prevPage->last();
			}
			--mPtr;
			return *this;
		}
		bool operator==(const const_iterator& rhs) const {return mPtr == rhs.mPtr;}
		bool operator!=(const const_iterator& rhs) const {return mPtr != rhs.mPtr;}
		const T* ptr() const {return mPtr;}
	};

	virtual_book() {
		mHead.set_nil();
	}
	~virtual_book() {
		clear(); 
		purge();
	}

	bool empty() const {mFirst == mLast;}
	bool full() const {mLast == mLastReserved;}

	bool push_back(const T& value) {
		if (mLast == mLastReserved) {
			if (!grow())
				return false;
		}
		new	(mLast.ptr()) T(value);
		++mLast;
		return true;
	}

	void pop_back() {
		--mLast;
		mLast->~T();
	}

	void clear() {
		while (mFirst != mLast)
			pop_back();
	}

	void purge() {
		if (mLast != mLastReserved) {
			page* lastPage = get_page(mLast.ptr());
			if (mFirst != mLast)
				lastPage = lastPage->mNext;
			while (lastPage != &mHead) {
				page* curPage = lastPage;
				lastPage = lastPage->mNext;
				void* mem = curPage->mem();
				curPage->unlink();
				virtual_free(mem);
			}
			if (mFirst == mLast) {
				mFirst = iterator();
				mLast = iterator();
				mLastReserved = iterator();
			} else {
				mLastReserved = iterator(mHead.mPrev->last());
			}
		}
	}

	const_iterator begin() const {return mFirst;}
	iterator begin() {return mFirst;}
	const_iterator end() const {return mLast;}
	iterator end() {return mLast;}
	const T& front() const {assert(mFirst != mLast); return *begin();}
	T& front() {assert(mFirst != mLast); return *begin();}
	const T& back() const {assert(mFirst != mLast); return *(--end());}
	T& back() {assert(mFirst != mLast); return *(--end());}
private:
	page mHead;
	iterator mFirst;
	iterator mLast;
	iterator mLastReserved;
};
#endif // DEBUG_ALLOCATOR


//////////////////////////////////////////////////////////////////////////
class allocator {
	// the guard size controls how many extra bytes are stored after
	// every allocation payload to detect memory stomps
	#ifdef DEBUG_ALLOCATOR
	static const size_t MEMORY_GUARD_SIZE  = 16UL;
	#else
	static const size_t MEMORY_GUARD_SIZE  = 0UL;
	#endif

	// minimum allocation size, must be a power of two
	// and it needs to be able to fit a pointer
	static const size_t MIN_ALLOCATION_LOG2 = 3UL;
	static const size_t MIN_ALLOCATION  = 1UL << MIN_ALLOCATION_LOG2;

	// the maximum small allocation, anything larger goes to the tree allocator
	// must be a power of two
	static const size_t MAX_SMALL_ALLOCATION_LOG2 = 8UL;
	static const size_t MAX_SMALL_ALLOCATION  = 1UL << MAX_SMALL_ALLOCATION_LOG2;

	// default alignment, must be a power of two
	static const size_t DEFAULT_ALIGNMENT  = sizeof(double);

	// page size controls the size of pages we get from the OS
	// virtual memory is not necessary, on Win32 it's just convenient
	static const size_t PAGE_SIZE_LOG2  = VIRTUAL_PAGE_SIZE_LOG2;
	static const size_t PAGE_SIZE  = 1UL << PAGE_SIZE_LOG2;

	static const size_t NUM_BUCKETS  = (MAX_SMALL_ALLOCATION / MIN_ALLOCATION);

	static inline bool is_small_allocation(size_t s) {
		return s + MEMORY_GUARD_SIZE <= MAX_SMALL_ALLOCATION;
	}
	static inline size_t clamp_small_allocation(size_t s) {
		return (s + MEMORY_GUARD_SIZE < MIN_ALLOCATION) ? MIN_ALLOCATION - MEMORY_GUARD_SIZE : s;
	}

	// bucket spacing functions control how the size-space is divided between buckets
	// currently we use linear spacing, could be changed to logarithmic etc
	static inline unsigned bucket_spacing_function(size_t size) { 
		return (unsigned)((size + (MIN_ALLOCATION-1)) >> MIN_ALLOCATION_LOG2) - 1;
	}
	static inline unsigned bucket_spacing_function_aligned(size_t size) { 
		return (unsigned)(size >> MIN_ALLOCATION_LOG2) - 1;
	}
	static inline size_t bucket_spacing_function_inverse(unsigned index) { 
		return (size_t)(index + 1) << MIN_ALLOCATION_LOG2;
	}

	// the page structure is where the small allocator stores all its book-keeping information
	// it is always located at the back of a OS page 
	struct free_link {
		free_link* mNext;
	};
	struct page : intrusive_list<page>::node {
		page(free_link* freeList, size_t elemSize, unsigned marker) 
			: mFreeList(freeList), mBucketIndex((unsigned short)bucket_spacing_function_aligned(elemSize)), mUseCount(0) {
			mMarker = marker ^ (unsigned)((size_t)this); 
		}
		free_link* mFreeList;
		unsigned short mBucketIndex;
		unsigned short mUseCount;
		unsigned mMarker;
		size_t elem_size() const {return bucket_spacing_function_inverse(mBucketIndex);}
		unsigned bucket_index() const {return mBucketIndex;}
		size_t count() const {return mUseCount;}
		bool empty() const {return mUseCount == 0;}
		void inc_ref() {mUseCount++;}
		void dec_ref() {assert(mUseCount > 0); mUseCount--;}
		bool check_marker(unsigned marker) const {return mMarker == (marker ^ (unsigned)((size_t)this));}
	};
	typedef intrusive_list<page> page_list;
	class bucket {
		page_list mPageList;
		#ifdef MULTITHREADED
		static const size_t SPIN_COUNT = 256;
		mutable mutex mLock;
		#endif
		unsigned mMarker;
		#ifdef MULTITHREADED
		unsigned char _padding[sizeof(void*)*16 - sizeof(page_list) - sizeof(mutex) - sizeof(unsigned)];
		#else
		unsigned char _padding[sizeof(void*)*4 - sizeof(page_list) - sizeof(unsigned)];
		#endif
		static const unsigned MARKER = 0x628bf2b6;
	public:
		bucket();
		#ifdef MULTITHREADED
		mutex& get_lock() const {return mLock;}
		#endif
		unsigned marker() const {return mMarker;}
		const page* page_list_begin() const {return mPageList.begin();}
		page* page_list_begin() {return mPageList.begin();}
		const page* page_list_end() const {return mPageList.end();}
		page* page_list_end() {return mPageList.end();}
		bool page_list_empty() const {return mPageList.empty();}
		void add_free_page(page* p) {mPageList.push_front(p);}
		page* get_free_page();
		void* alloc(page* p);
		void free(page* p, void* ptr);
	};
	void* bucket_system_alloc();
	void bucket_system_free(void* ptr);
	page* bucket_grow(size_t elemSize, unsigned marker);
	void* bucket_alloc(size_t size);
	void* bucket_alloc_direct(unsigned bi);
	void* bucket_realloc(void* ptr, size_t size);
	void bucket_free(void* ptr);
	void bucket_free_direct(void* ptr, unsigned bi);
	void bucket_purge();

	// locate the page information from a pointer
	static inline page* ptr_get_page(void* ptr) {
		return (page*)(align_down((char*)ptr, PAGE_SIZE) + (PAGE_SIZE - sizeof(page)));
	}

	bool ptr_in_bucket(void* ptr) const {
		bool result = false;
		page* p = ptr_get_page(ptr);
		unsigned bi = p->bucket_index();
		if (bi < NUM_BUCKETS) {
			result = p->check_marker(mBuckets[bi].marker());
			// there's a minimal chance the marker check is not sufficient
			// due to random data that happens to match the marker
			// the exhaustive search below will catch this case 
			// and that will indicate that more secure measures are needed
			#ifndef NDEBUG
			#ifdef MULTITHREADED
			mutex::scoped lock(mBuckets[bi].get_lock());
			#endif
			const page* pe = mBuckets[bi].page_list_end();
			const page* pb = mBuckets[bi].page_list_begin();
			for (; pb != pe && pb != p; pb = pb->next()) {}
			assert(result == (pb == p));
			#endif
		}
		return result;
	}

	// block header is where the large allocator stores its book-keeping information
	// it is always located in front of the payload block
	class block_header {
		enum block_flags {BL_USED = 1};
		block_header* mPrev;
		size_t mSizeAndFlags;
		#if defined( _MSC_VER )
		#pragma warning(push)
		#pragma warning (disable : 4200) // zero sized array
		#endif
		unsigned char _padding[DEFAULT_ALIGNMENT <= sizeof(block_header*) + sizeof(size_t) ? 0 : DEFAULT_ALIGNMENT - sizeof(block_header*) - sizeof(size_t)];
		#if defined( _MSC_VER )
		#pragma warning(pop)
		#endif
	public:
		typedef block_header* block_ptr;
		size_t size() const {return mSizeAndFlags & ~3;}
		block_ptr next() const {return (block_ptr)((char*)mem() + size());}
		block_ptr prev() const {return mPrev;}
		void* mem() const {return (void*)((char*)this + sizeof(block_header));}
		bool used() const {return (mSizeAndFlags & BL_USED) != 0;}
		void set_used() {mSizeAndFlags |= BL_USED;}
		void set_unused() {mSizeAndFlags &= ~BL_USED;}
		void unlink() {
			next()->prev(prev());
			prev()->next(next());
		}
		void link_after(block_ptr link) {
			prev(link);
			next(link->next());
			next()->prev(this);
			prev()->next(this);
		}
		void size(size_t size) {
			assert((size & 3) == 0);
			mSizeAndFlags = (mSizeAndFlags & 3) | size;
		}
		void next(block_ptr next) {
			assert(next >= mem());
			size((char*)next - (char*)mem());
		}
		void prev(block_ptr prev) {
			mPrev = prev;
		}
	};

	// small free nodes is an optimization for the large allocator
	// these blocks are never queried so they are simply kept in a linked list
	// this way inserts and erases are much faster
	struct small_free_node : public intrusive_list<small_free_node>::node {};
	typedef intrusive_list<small_free_node> small_free_node_list;

	// free node is what the large allocator uses to find free space
	// it's stored next to the block header when a block is not in use
	struct free_node : public intrusive_multi_rbtree<free_node>::node {
		block_header* get_block() const {return (block_header*)((char*)this - sizeof(block_header));}
		bool operator<(const free_node& rhs) const {return get_block()->size() < rhs.get_block()->size();}
		bool operator>(const free_node& rhs) const {return get_block()->size() > rhs.get_block()->size();}
		bool operator<(size_t size) const {return get_block()->size() < size;}
		bool operator>(size_t size) const {return get_block()->size() > size;}
	};
	typedef intrusive_multi_rbtree<free_node> free_node_tree;

	static inline block_header* ptr_get_block_header(void* ptr) {
		return (block_header*)((char*)ptr - sizeof(block_header));
	}

	// helper functions to manipulate block headers
	void split_block(block_header* bl, size_t size);
	block_header* shift_block(block_header* bl, size_t offs);
	block_header* coalesce_block(block_header* bl);

	void* tree_system_alloc(size_t size);
	void tree_system_free(void* ptr, size_t size);
	block_header* tree_extract(size_t size);
	block_header* tree_extract_aligned(size_t size, size_t alignment);
	block_header* tree_add_block(void* mem, size_t size);
	block_header* tree_grow(size_t size);
	void tree_attach(block_header* bl);
	void tree_detach(block_header* bl);
	void tree_purge_block(block_header* bl);

	void* tree_alloc(size_t size);
	void* tree_alloc_aligned(size_t size, size_t alignment);
	void* tree_realloc(void* ptr, size_t size);
	void* tree_realloc_aligned(void* ptr, size_t size, size_t alignment);
	size_t tree_resize(void* ptr, size_t size);
	void tree_free(void* ptr);
	void tree_purge();

	bucket mBuckets[NUM_BUCKETS];
	block_header* mMRFreeBlock; // most recent block, used by the tree allocator
	free_node_tree mFreeTree;
	small_free_node_list mSmallFreeList;
	#ifdef MULTITHREADED
	mutex mTreeMutex;
	#endif

	enum debug_source {DEBUG_SOURCE_BUCKETS = 0, DEBUG_SOURCE_TREE = 1};

	#ifdef DEBUG_ALLOCATOR
	// debug record stores all debugging information for every allocation
	class debug_record : public intrusive_multi_rbtree<debug_record>::node {
	public:
		static const unsigned MAX_CALLSTACK_DEPTH = 8;
		debug_record() {}
		debug_record(void* ptr, size_t size, debug_source source) : mPtr(ptr), mSize(size), mSource(source) {
			write_guard();
			record_stack(); 
		}
		void* ptr() const {return mPtr;}
		void set_ptr(void* ptr) {mPtr = ptr;}
		size_t size() const {return mSize;}
		debug_source source() const {return (debug_source)mSource;}
		void set_size(size_t size) {mSize = size;}
		const char* stack(unsigned i) const {return mCallstack[i];}
		void record_stack();
		void write_guard();
		bool check_guard() const;
		// comparison operators are needed for the red-black tree
		bool operator<(const debug_record& rhs) const {return mPtr < rhs.mPtr;}
		bool operator>(const debug_record& rhs) const {return rhs.mPtr < mPtr;}
		bool operator<(void* ptr) const {return mPtr < ptr;}
		bool operator>(void* ptr) const {return ptr < mPtr;}
	private:
		void* mPtr;
		size_t mSize;
		unsigned char mSource;
		unsigned char mGuardByte;
		const char* mCallstack[MAX_CALLSTACK_DEPTH];
	};

	// helper structure for returning debug record information
	struct debug_info {
		size_t size;
		debug_source source;
		debug_info(size_t _size, debug_source _source) : size(_size), source(_source) {}
	};

	// record map that keeps all records in an embedded red-black tree, 
	// the memory for which is managed by a virtual book
	// a multi-red-black tree is technically not needed since addresses are always unique
	// but for brevity we omit the inclusion of the intrusive_red_black_tree class
	class debug_record_map : public intrusive_multi_rbtree<debug_record> {
		virtual_book<debug_record> mRecords;
		typedef intrusive_multi_rbtree<debug_record> base;
		static void initial_fill(void* ptr, size_t size);
	public:
		typedef base::const_iterator const_iterator;
		typedef base::iterator iterator;
		bool add(void* ptr, size_t size, debug_source source);
		debug_info remove(void* ptr);
		debug_info remove(void* ptr, size_t size);
		debug_info replace(void* ptr, void* newPtr, size_t size, debug_source source);
		debug_info update(void* ptr, size_t size);
		void check(void* ptr) const;
		void purge();
	};
	debug_record_map mDebugMap;
	#ifdef MULTITHREADED
	mutex mDebugMutex;
	#endif

	size_t mTotalRequestedSizeBuckets;
	size_t mTotalRequestedSizeTree;

	void* debug_add(void* ptr, size_t size, debug_source source);
	void debug_remove(void* ptr);
	void debug_remove(void* ptr, size_t size);
	void debug_replace(void* ptr, void* newPtr, size_t size, debug_source source);
	void debug_update(void* ptr, size_t size);
	void debug_check(void* ptr);
	void debug_purge();

	#else // !DEBUG_ALLOCATOR

	void* debug_add(void* ptr, size_t, debug_source) {return ptr;}
	void debug_remove(void*) {}
	void debug_remove(void*, size_t) {}
	void debug_replace(void*, void*, size_t, debug_source) {}
	void debug_update(void*, size_t) {}
	void debug_check(void*) {}
	void debug_purge() {}

	#endif // DEBUG_ALLOCATOR

	size_t mTotalAllocatedSizeBuckets;
	size_t mTotalAllocatedSizeTree;
public:
	allocator();
	~allocator();
	// allocate memory using DEFAULT_ALIGNMENT
	// size == 0 returns NULL
	void* alloc(size_t size) {
		if (!is_small_allocation(size)) {
			void* ptr = tree_alloc(size + MEMORY_GUARD_SIZE);
			return debug_add(ptr, size, DEBUG_SOURCE_TREE);
		}
		if (size == 0)
			return NULL;
		size = clamp_small_allocation(size);
		void* ptr = bucket_alloc_direct(bucket_spacing_function(size + MEMORY_GUARD_SIZE));
		return debug_add(ptr, size, DEBUG_SOURCE_BUCKETS);
	}
	// allocate memory with a specific alignment
	// size == 0 returns NULL
	// alignment <= DEFAULT_ALIGNMENT acts as alloc
	void* alloc(size_t size, size_t alignment) {
		assert((alignment & (alignment-1)) == 0);
		if (alignment <= DEFAULT_ALIGNMENT)
			return alloc(size);
		if (!is_small_allocation(size) || alignment > MAX_SMALL_ALLOCATION) {
			void* ptr = tree_alloc_aligned(size + MEMORY_GUARD_SIZE, alignment);
			return debug_add(ptr, size, DEBUG_SOURCE_TREE);
		}
		if (size == 0)
			return NULL;
		size = clamp_small_allocation(size);
		void* ptr = bucket_alloc_direct(bucket_spacing_function(round_up(size + MEMORY_GUARD_SIZE, alignment)));
		return debug_add(ptr, size, DEBUG_SOURCE_BUCKETS);
	}
	// allocate count*size and clear the memory block to zero, always uses DEFAULT_ALIGNMENT
	void* calloc(size_t count, size_t size) {
		void* p = alloc(count * size);
		if (p)
			memset(p, 0, count * size);
		return p;
	}
	// realloc the memory block using DEFAULT_ALIGNMENT
	// ptr == NULL acts as alloc
	// size == 0 acts as free
	void* realloc(void* ptr, size_t size) {
		if (ptr == NULL)
			return alloc(size);
		if (size == 0) {
			free(ptr);
			return NULL;
		}
		debug_check(ptr);
		if (ptr_in_bucket(ptr)) {
			size = clamp_small_allocation(size);
			if (is_small_allocation(size)) {
				void* newPtr = bucket_realloc(ptr, size + MEMORY_GUARD_SIZE);
				debug_replace(ptr, newPtr, size, DEBUG_SOURCE_BUCKETS);
				return newPtr;
			}
			void* newPtr = tree_alloc(size + MEMORY_GUARD_SIZE);
			if (!newPtr)
				return NULL;
			memcpy(newPtr, ptr, ptr_get_page(ptr)->elem_size() - MEMORY_GUARD_SIZE);
			bucket_free(ptr);
			debug_replace(ptr, newPtr, size, DEBUG_SOURCE_TREE);
			return newPtr;
		}
		void* newPtr = tree_realloc(ptr, size + MEMORY_GUARD_SIZE);
		debug_replace(ptr, newPtr, size, DEBUG_SOURCE_TREE);
		return newPtr;
	}
	// realloc the memory block using a specific alignment
	// ptr == NULL acts as alloc
	// size == 0 acts as free
	// alignment <= DEFAULT_ALIGNMENT acts as realloc
	void* realloc(void* ptr, size_t size, size_t alignment) {
		assert((alignment & (alignment-1)) == 0);
		if (alignment <= DEFAULT_ALIGNMENT)
			return realloc(ptr, size);
		if (ptr == NULL)
			return alloc(size, alignment);
		if (size == 0) {
			free(ptr);
			return NULL;
		}
		if ((size_t)ptr & (alignment-1)) {
			void* newPtr = alloc(size, alignment);
			if (!newPtr)
				return NULL;
			size_t count = this->size(ptr);
			if (count > size)
				count = size;
			memcpy(newPtr, ptr, count);
			free(ptr);
			return newPtr;
		}
		debug_check(ptr);
		if (ptr_in_bucket(ptr)) {
			size = clamp_small_allocation(size);
			if (is_small_allocation(size) && alignment <= MAX_SMALL_ALLOCATION) {
				void* newPtr = bucket_realloc(ptr, size + MEMORY_GUARD_SIZE);
				debug_replace(ptr, newPtr, size, DEBUG_SOURCE_BUCKETS);
				return newPtr;
			}
			void* newPtr = tree_alloc_aligned(size + MEMORY_GUARD_SIZE, alignment);
			if (!newPtr)
				return NULL;
			memcpy(newPtr, ptr, ptr_get_page(ptr)->elem_size() - MEMORY_GUARD_SIZE);
			bucket_free(ptr);
			debug_replace(ptr, newPtr, size, DEBUG_SOURCE_TREE);
			return newPtr;
		}
		void* newPtr = tree_realloc_aligned(ptr, size + MEMORY_GUARD_SIZE, alignment);
		debug_replace(ptr, newPtr, size, DEBUG_SOURCE_TREE);
		return newPtr;
	}
	// resize the memory block to the extent possible
	// returns the size of the resulting memory block
	size_t resize(void* ptr, size_t size) {
		if (ptr == NULL)
			return 0;
		assert(size > 0);
		debug_check(ptr);
		if (ptr_in_bucket(ptr)) {
			size = ptr_get_page(ptr)->elem_size() - MEMORY_GUARD_SIZE;
			debug_update(ptr, size);
			return size;
		}
		size = tree_resize(ptr, size + MEMORY_GUARD_SIZE) - MEMORY_GUARD_SIZE;
		debug_update(ptr, size);
		return size;
	}
	// query the size of the memory block
	size_t size(void* ptr) const {
		if (ptr == NULL)
			return 0;
		if (ptr_in_bucket(ptr)) 
			return ptr_get_page(ptr)->elem_size() - MEMORY_GUARD_SIZE;
		return ptr_get_block_header(ptr)->size() - MEMORY_GUARD_SIZE;
	}
	// free the memory block
	void free(void* ptr) {
		if (ptr == NULL)
			return;
		debug_remove(ptr);
		if (ptr_in_bucket(ptr))
			return bucket_free(ptr);
		tree_free(ptr);
	}
	// free the memory block supplying the original size with DEFAULT_ALIGNMENT
	void free(void* ptr, size_t origSize) {
		if (ptr == NULL)
			return;
		debug_remove(ptr, origSize);
		if (is_small_allocation(origSize)) {
			// if this asserts probably the original alloc used alignment
			assert(ptr_in_bucket(ptr));
			return bucket_free_direct(ptr, bucket_spacing_function(origSize + MEMORY_GUARD_SIZE));
		}
		tree_free(ptr);
	}
	// free the memory block supplying the original size and alignment
	void free(void* ptr, size_t origSize, size_t oldAlignment) {
		if (ptr == NULL)
			return;
		debug_remove(ptr, origSize);
		if (is_small_allocation(origSize) && oldAlignment <= MAX_SMALL_ALLOCATION) {
			assert(ptr_in_bucket(ptr));
			return bucket_free_direct(ptr, bucket_spacing_function(round_up(origSize + MEMORY_GUARD_SIZE, oldAlignment)));
		}
		tree_free(ptr);
	}
	// return all unused memory pages to the OS
	// this function doesn't need to be called unless needed
	// it can be called periodically when large amounts of memory can be reclaimed
	// in all cases memory is never automatically returned to the OS
	void purge() {
		tree_purge();
		bucket_purge();
		debug_purge();
	}
	#ifdef DEBUG_ALLOCATOR
	// print allocator statistics
	void report();
	// check memory integrity
	void check();
	// return the total number of requested memory
	size_t requested() const {
		return mTotalRequestedSizeBuckets + mTotalRequestedSizeTree;
	}
	#endif
	// return the total number of allocated memory
	size_t allocated() const {
		return mTotalAllocatedSizeBuckets + mTotalAllocatedSizeTree;
	}
};

#endif // _HPHA_H_
