/*******************************************************************\
| DbgAllocator.h
|--------------------------------------------------------------------
| CREATED:		2007/7/22
| AUTHOR:		Martin Fleisz
\********************************************************************/
#ifndef DBGLIB_DBGALLOCATOR_H
#define DBGLIB_DBGALLOCATOR_H

namespace DbgLib
{

namespace Internal
{

//*****************************************************************************
// class overview:
//   Custom stl allocator used on windows for the std::map of our memory leak
//   detector. This allocator allocates memory flagged as CRT allocations and
//   will therefore be ignored and not tracked by the leak detector.
//-----------------------------------------------------------------------------
template<typename T>
class DbgAllocator
{
	public : 
		// define required types
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

	public : 
		// convert an allocator<T> to allocator<U>
		template<typename U>
		struct rebind 
		{	
			typedef DbgAllocator<U> other;
		};

	public : 
		inline DbgAllocator() {}
		inline ~DbgAllocator() {}
		inline DbgAllocator(DbgAllocator const&) {}
		template<typename U>
		inline DbgAllocator(DbgAllocator<U> const&) {}

		// address
		inline pointer address(reference r) { return &r; }
		inline const_pointer address(const_reference r) { return &r; }

		// memory allocation
		inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0)
		{
			return reinterpret_cast<pointer>(_malloc_dbg(cnt * sizeof(T), _CRT_BLOCK, __FILE__, __LINE__)); 
		}
		
		inline void deallocate(pointer p, size_type) 
		{ 
			_free_dbg(p, _CRT_BLOCK); 
		}

		// size
		inline size_type max_size() const 
		{ 
			#pragma push_macro("max")
			#undef max
			return std::numeric_limits<size_type>::max() / sizeof(T);
			#pragma pop_macro("max")
		}

		// construction/destruction
		inline void construct(pointer p, const T& t) { new(p) T(t); }
		inline void destroy(pointer p) { p->~T(); }

		inline bool operator==(DbgAllocator const&) { return true; }
		inline bool operator!=(DbgAllocator const& a) { return !operator==(a); }
};

} // namespace Internal

} // namespace DbgLib

#endif // DBGLIB_DBGALLOCATOR_H
