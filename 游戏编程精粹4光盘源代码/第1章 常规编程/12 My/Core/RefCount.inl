//--------------------------------------------------------------------------------------------------------------------//
//                                                REFERENCE COUNTING CLASS                                            //
// written by Frederic My                                                                                             //
// fmy@fairyengine.com                                                                                                //
//--------------------------------------------------------------------------------------------------------------------//

#ifndef   _GPG4_REFCOUNT_INL_
#define   _GPG4_REFCOUNT_INL_

namespace GPG4
{

//--------------------------------------------------------------------------------------------------------------------//
//                                                COUNTING                                                            //
//--------------------------------------------------------------------------------------------------------------------//

INLINE void CRefCount::AddRef()
  {
  m_u32RefCount++;
  }

//

INLINE void CRefCount::Release()
  {
  m_u32RefCount--;
  if(!m_u32RefCount) delete this;                           // object allocated by 'new'
  }

//

INLINE u32 CRefCount::GetRefCount() const
  {
  return m_u32RefCount;
  }

//--------------------------------------------------------------------------------------------------------------------//
//                                                CONSTRUCTORS / DESTRUCTOR                                           //
//--------------------------------------------------------------------------------------------------------------------//

INLINE CRefCount::CRefCount() : m_u32RefCount(0)
  {}

INLINE CRefCount::CRefCount(const CRefCount& RefCount) : m_u32RefCount(0)
  {}

INLINE CRefCount::~CRefCount()
  {}

//--------------------------------------------------------------------------------------------------------------------//
//                                                OPERATORS                                                           //
//--------------------------------------------------------------------------------------------------------------------//

INLINE CRefCount& CRefCount::operator = (const CRefCount& RefCount)
  {
  return *this;
  }

//--------------------------------------------------------------------------------------------------------------------//

} // namespace

#endif // _GPG4_REFCOUNT_INL_
