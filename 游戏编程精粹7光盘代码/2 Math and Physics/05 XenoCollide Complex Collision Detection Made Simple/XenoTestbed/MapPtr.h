
#pragma once

//////////////////////////////////////////////////////////////////////////////
// MapPtr<> is a quick and dirty smart pointer template.  MapPtr<> pointers
// utilize a reference count stored in a hash_map, indexed by pointer value.
// MapPtr<> allows any C++ object to be reference counted and deleted
// when the number of references drop to zero.

#include <map>
#include <hash_map>

class MapPtrBase
{
protected:
	static stdext::hash_map<void*, int> s_refCount;
};

template <class T>
class MapPtr : MapPtrBase
{
public:

	MapPtr()
	{
		m_pointer = NULL;
	}

	MapPtr(T* pointer)
	{
		m_pointer = pointer;
		if (pointer)
		{
			s_refCount[pointer]++;
		}
	}

	MapPtr(const MapPtr& other)
	{
		m_pointer = NULL;
		*this = other;
	}

	~MapPtr()
	{
		if ( m_pointer && --s_refCount[m_pointer] == 0 )
		{
			s_refCount.erase(m_pointer);
			delete m_pointer;
			m_pointer = NULL;
		}
	}

	void Release()
	{
		if (m_pointer)
		{
			if (--s_refCount[m_pointer] == 0)
			{
				s_refCount.erase(m_pointer);
				delete m_pointer;
				m_pointer = NULL;
			}
		}
	}

	void AddRef()
	{
		if (m_pointer)
		{
			s_refCount[m_pointer]++;
		}
	}

	MapPtr& operator= (const MapPtr& other)
	{
		Release();
		m_pointer = other.m_pointer;
		AddRef();
		return *this;
	}

	T* Pointer()
	{
		return m_pointer;
	}

	operator T*()
	{
		return m_pointer;
	}

	T* operator -> ()
	{
		return m_pointer;
	}

private:

	T* m_pointer;

};