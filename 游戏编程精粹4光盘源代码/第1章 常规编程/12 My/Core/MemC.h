//--------------------------------------------------------------------------------------------------------------------//
//                                                MEMORY FUNCTIONS                                                    //
// written by Frederic My                                                                                             //
// fmy@fairyengine.com                                                                                                //
//--------------------------------------------------------------------------------------------------------------------//

#ifndef   _GPG4_MEMC_H_
#define   _GPG4_MEMC_H_

//----------------------------------------------- INCLUDES -----------------------------------------------------------//

#include  "BasicTypes.h"
#include  "Defines.h"

//----------------------------------------------- CLASSES ------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//                                                CMemC                                                               //
//--------------------------------------------------------------------------------------------------------------------//

namespace GPG4
{
  class CMemC
  {
  // public methods

  public:

    static void*              MemSet              (void* pvDest,ui uiChr,u32 u32Count);
    static void*              MemCpy              (void* pvDest,const void* pvSrc,u32 u32Count);

    static void*              Malloc              (u32 u32Size);
    static bool               Free                (void* pvAddr);
  };

  typedef CMemC Memory;
}

//----------------------------------------------- INLINES ------------------------------------------------------------//

#ifndef   _DEBUG
#include  "MemC.inl"
#endif

//--------------------------------------------------------------------------------------------------------------------//

#endif // _GPG4_MEMC_H_
