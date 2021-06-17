/*******************************************************************\
| MemLeakDetector.h
|--------------------------------------------------------------------
| CREATED:		2007/7/11
| AUTHOR:		Martin Fleisz
\********************************************************************/
#ifndef DBGLIB_MEMLEAKDETECTOR_H
#define DBGLIB_MEMLEAKDETECTOR_H

#if defined(PLATFORM_WINDOWS)
#include "DbgAllocator.h"
#endif

namespace DbgLib
{

class IMemLeakReporter;

//*****************************************************************************
// class overview:
//   Memory leak detector class.
//-----------------------------------------------------------------------------
class CMemLeakDetector
{
	public:
		// prohibit instantiation of CMemLeakDetector from anyone else except CDebugFX
		CMemLeakDetector();
		~CMemLeakDetector();

	public:
		// sets a new memory leak reporter (the instance will be deleted by the leak detector)
		void SetReporter(IMemLeakReporter* p_Reporter);

		// enables the leak detector
		void Enable();

		// disables the leak detector (can be used to temporarily disable the detector to allocate
		// a new leak reporter and reenable leak detection afterwards).
		void Disable();

		// clears all allocation information collected so far and thus "resets" the leak detector
		void ClearAllocInfo();

		// reports memory leaks
		void ReportLeaks();

		// enables/disables module info caching
		void SetModuleInfoCaching(bool p_Enable);

		// module information table used on windows for symbol resolving in the reporter
		struct ModuleInfo
		{
			ModuleHandle m_hModule;
			tchar* m_ModuleName;
		};

	private:
		// track an allocation with the given id and the given block size
		void TrackAllocInfo(uintx p_AllocID, uintx p_Size);

		// remove the allocation info for the given allocation id from our tracker
		void RemoveAllocInfo(uintx p_AllocID);

		// allocation info structure
		struct AllocInfo
		{
			// constant specifying the maximum callstack depth
			static const uintx MaxCallstackDepth = 512;

#if defined(PLATFORM_UNIX)
			uintx m_Size;
#endif
			uintx m_Callstack[MaxCallstackDepth];
			intx m_ModuleReference[MaxCallstackDepth];
			uintx m_CallstackSize;
		};

		// current reporter
		IMemLeakReporter* m_Reporter;

		// flag indicating if module information should be cached (for delayed symbol resolving)
		bool m_CacheModuleInfo;

		// used to check status of allocation hook function
		bool m_HooksEnabled;

#if defined(PLATFORM_WINDOWS)
	private:
		// crt allocation hook
		static int CrtAllocHook(int allocType, void *userData, size_t size, int blockType, long requestNumber, const unsigned char *filename, int lineNumber);

		// pointer to the previously installed allocation hook function
		_CRT_ALLOC_HOOK m_OldHook;

		// map storing our allocation information
		typedef std::map<uintx, AllocInfo, std::less<uintx>, Internal::DbgAllocator<std::pair<const uintx, AllocInfo> > > AllocInfoMap;
		AllocInfoMap m_AllocInfoMap;

		// map containing module names referenced by addresses in the callstacks stored in the allocinfo map
		typedef std::vector<ModuleInfo, Internal::DbgAllocator<ModuleInfo> > ModuleInfoVector;
		ModuleInfoVector m_ModuleInfoVector;
	
#else	// PLATFORM_WINDOWS
	public:
		// first-time initialization function to enable our own allocation hooks
		static void InitHooks();

	private:
		// hook function for malloc()
		static void* MallocHook(size_t p_Size, const void* p_Caller);

		// hook function for realloc()
		static void* ReallocHook(void* p_MemPtr, size_t p_Size, const void* p_Caller);

		// hook function for free()
		static void FreeHook(void* p_MemPtr, const void* p_Caller);

		// helper function to install our own allocation hooks
		static void InstallHooks();

		// helper function to save the current hooks
		static void SaveHooks();

		// helper function to restore previously saved hooks
		static void RestoreHooks();

		// map storing our allocation information
		typedef std::map<uintx, AllocInfo> AllocInfoMap;
		AllocInfoMap m_AllocInfoMap;

		typedef void* (*MALLOCHOOKFUNC)(size_t, const void*);			// typedef for malloc hook function
		typedef void* (*REALLOCHOOKFUNC)(void*, size_t, const void*);	// typedef for realloc hook function
		typedef void (*FREEHOOKFUNC)(void*, const void*);				// typedef for free hook function

		static MALLOCHOOKFUNC m_OldMallocFunc;		// backup of the malloc function before hooking
		static REALLOCHOOKFUNC m_OldReallocFunc;		// backup of the realloc function before hooking
		static FREEHOOKFUNC m_OldFreeFunc;			// backup of the free function before hooking

#endif	// PLATFORM_WINDOWS
};


//*****************************************************************************
// class overview:
//   Interface declaration for the memory leak reporter. The leak detector sends
//   information about the leaks found to a leak reporter that is derived from
//   this interface and set via CMemLeakDetector::SetReporter().
//-----------------------------------------------------------------------------
class IMemLeakReporter
{
	public:
		virtual ~IMemLeakReporter() { }

	public:
		// is called before all leaks are enumerated
		virtual void WriteHeader(uintx p_LeaksFound) = 0;

		// writes informatin about a memory leak, gets called for each memory leak found
		virtual void WriteLeak(uintx p_Request, void* p_Data, uintx p_DataSize, uintx p_StackSize, uintx* p_Callstack, intx* p_ModRefs, CMemLeakDetector::ModuleInfo* p_ModuleInfo) = 0;

		// called after all memory leaks were reported
		virtual void WriteFooter() = 0;
};

//*****************************************************************************
// class overview:
//   Default implementation of IMemLeakReporter that writes the leak information
//   to the debug output (on Windows platforms) or to the standard error output
//   stream.
//-----------------------------------------------------------------------------
class CSymbolResolver;

class CDefaultLeakReporter : public IMemLeakReporter
{
	public:
		// p_MaxLeaksToReport defines how many leaks should be reported at a maximum, 0 reports all leaks
		CDefaultLeakReporter(bool p_ShowDataBytes, bool p_ShowCallstack, uintx p_MaxLeaksToReport = 0);
		virtual ~CDefaultLeakReporter();

	public:
		// is called before all leaks are enumerated
		virtual void WriteHeader(uintx p_LeaksFound);

		// writes informatin about a memory leak, gets called for each memory leak found
		virtual void WriteLeak(uintx p_Request, void* p_Data, uintx p_DataSize, uintx p_StackSize, uintx* p_Callstack, intx* p_ModRefs, CMemLeakDetector::ModuleInfo* p_ModuleInfo);

		// called after all memory leaks were reported
		virtual void WriteFooter();

	private:
		// writes a message to the debug output
		void WriteDebugString(const tchar* p_Msg);

		// specifies the maximum number of data bytes shown in the leak information
		static const uintx m_MaxDataBytes = 16;

		// vector storing callstack frames that should not be displayed in the leak report
		std::vector<tstring> m_IgnoreCallstackFrames;

		// symbol resolver instance
		CSymbolResolver* m_SymbolResolver;

		// maximum amount of leaks to report
		uintx m_MaxLeaksToReport;

		// if set to true shows the first few bytes of the allocated data
		bool m_ShowDataBytes;
		
		// if set to true shows the callstack of the allocation
		bool m_ShowCallstack;
};


}

#endif // DBGLIB_MEMLEAKDETECTOR_H
