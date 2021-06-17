#include "stdafx.h"
#include "DebugHelp.h"
#include "MemLeakDetector.h"
#include "Utils.h"

#if defined(PLATFORM_WINDOWS)
// we have to define _CRTBLD in order to include dbgint.h and mtdll.h
#define _CRTBLD

#ifdef _MT
// required for synchronization with _mlock/_munlock
#include <..\crt\src\mtdll.h>
#endif // #ifdef _MT

// required for internal crt heap structures
#include <..\crt\src\dbgint.h>

#undef _CRTBLD

// macros for lock the heap (only when compiling with _MT)
#ifdef _MT
#define LOCK_HEAP _mlock(_HEAP_LOCK); 
#define UNLOCK_HEAP { _munlock(_HEAP_LOCK); }
#else
#define LOCK_HEAP
#define UNLOCK_HEAP
#endif

#else // PLATFORM_WINDOWS

#include <malloc.h>

#define LOCK_HEAP
#define UNLOCK_HEAP

// static class members
namespace DbgLib
{
CMemLeakDetector::MALLOCHOOKFUNC CMemLeakDetector::m_OldMallocFunc;		// backup of the malloc function before hooking
CMemLeakDetector::REALLOCHOOKFUNC CMemLeakDetector::m_OldReallocFunc;	// backup of the realloc function before hooking
CMemLeakDetector::FREEHOOKFUNC CMemLeakDetector::m_OldFreeFunc;			// backup of the free function before hooking
}

// install our malloc hook initialization function
#if defined(_DEBUG) && defined(AUTO_ENABLE_MEMLEAKDETECTOR)
void (*__malloc_initialize_hook) (void) = DbgLib::CMemLeakDetector::InitHooks;
#endif

#endif // PLATFORM_WINDOWS


namespace DbgLib
{


// Put the leak detector in the library initialization segment so we can trace memory allocs as soon as possible
// Otherwise we could miss leaks caused by global/static objects.
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#endif
CMemLeakDetector g_MemLeakDetector;
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4073)
#endif


//////////////////////////////////////////////////////////////////////////
CMemLeakDetector::CMemLeakDetector()
{
	m_HooksEnabled = false;

#if defined(PLATFORM_WINDOWS)
	m_Reporter = new CDefaultLeakReporter(true, true);
	m_CacheModuleInfo = true;
#if defined(AUTO_ENABLE_MEMLEAKDETECTOR)
	Enable();
#endif
#endif
}

CMemLeakDetector::~CMemLeakDetector()
{
#if defined(AUTO_ENABLE_MEMLEAKDETECTOR)
	// report leaks
	Disable();
	ReportLeaks();
#endif

#if defined(PLATFORM_WINDOWS)
	for(std::size_t i = 0; i < m_ModuleInfoVector.size(); ++i)
		_free_dbg(m_ModuleInfoVector[i].m_ModuleName, _CRT_BLOCK);
#endif

	delete m_Reporter;
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::TrackAllocInfo(uintx p_AllocID, uintx p_Size)
{
	AllocInfo& ainfo = m_AllocInfoMap[p_AllocID];

#if defined(PLATFORM_UNIX)
	ainfo.m_Size = p_Size;
#endif
	ainfo.m_CallstackSize = CDebugHelp::DoStackWalk(ainfo.m_Callstack, AllocInfo::MaxCallstackDepth);

#if defined(PLATFORM_WINDOWS)
	// determine the names of all modules referenced in our callstack to load the symbol information
	// later on when reporting a leak
	for(uintx i = 0; i < ainfo.m_CallstackSize; ++i)
	{
		ainfo.m_ModuleReference[i] = -1;
		if(m_CacheModuleInfo)
		{
			// determine the module handle for the current callstack address
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(reinterpret_cast<LPCVOID>(static_cast<size_t>(ainfo.m_Callstack[i])), &mbi, sizeof(mbi));
			HMODULE hModule = static_cast<HMODULE>(mbi.AllocationBase);

			// check if we already have that module in our vector
			for(uintx curModInfo = 0; curModInfo < m_ModuleInfoVector.size(); ++curModInfo)
			{
				if(m_ModuleInfoVector[curModInfo].m_hModule == hModule)
					ainfo.m_ModuleReference[i] = curModInfo;
			}

			// if we haven't got that module - determine it's name
			if(ainfo.m_ModuleReference[i] == -1)
			{
				TCHAR moduleName[MAX_PATH];
				DWORD moduleNameSize = MAX_PATH;
				if((moduleNameSize = GetModuleFileName(hModule, moduleName, moduleNameSize)) != 0)
				{
					ModuleInfo modInfo;
					modInfo.m_hModule = hModule;
					modInfo.m_ModuleName = static_cast<tchar*>(_malloc_dbg(++moduleNameSize * sizeof(tchar), _CRT_BLOCK, __FILE__, __LINE__));
					_tcscpy_s(modInfo.m_ModuleName, moduleNameSize, moduleName);

					m_ModuleInfoVector.push_back(modInfo);
					ainfo.m_ModuleReference[i] = static_cast<intx>(m_ModuleInfoVector.size() - 1);
				}
			}
		}
	}
#else
	// set all module refs to -1
	for(uintx i = 0; i < ainfo.m_CallstackSize; ++i)
		ainfo.m_ModuleReference[i] = -1;
#endif
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::RemoveAllocInfo(uintx p_AllocID)
{
	m_AllocInfoMap.erase(p_AllocID);
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::Enable()
{
	if(m_HooksEnabled)
		return;

#if defined(PLATFORM_WINDOWS)
	m_OldHook = _CrtSetAllocHook(CrtAllocHook);
#else
#if defined(_DEBUG)
	SaveHooks();
	InstallHooks();
#endif
#endif
	m_HooksEnabled = true;
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::Disable()
{
	if(!m_HooksEnabled)
		return;

#if defined(PLATFORM_WINDOWS)
	_CrtSetAllocHook(m_OldHook);
	m_OldHook = NULL;
#else
#if defined(_DEBUG)
	RestoreHooks();
#endif
#endif
	m_HooksEnabled = false;
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::SetReporter(IMemLeakReporter* p_Reporter)
{
	delete m_Reporter;
	m_Reporter = p_Reporter;
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::ClearAllocInfo()
{
	m_AllocInfoMap.clear();
#if defined(PLATFORM_WINDOWS)
	for(std::size_t i = 0; i < m_ModuleInfoVector.size(); ++i)
		_free_dbg(m_ModuleInfoVector[i].m_ModuleName, _CRT_BLOCK);
	m_ModuleInfoVector.clear();
#endif
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::SetModuleInfoCaching(bool p_Enable)
{
#if defined(PLATFORM_WINDOWS)
	m_CacheModuleInfo = p_Enable;
#else
	UNREFERENCED_PARAMETER(p_Enable);
#endif
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::ReportLeaks()
{
	// hooks must be disabled for reporting
	assert(!m_HooksEnabled);

	if(m_AllocInfoMap.size() > 0)
	{
		// start reporting
		m_Reporter->WriteHeader(static_cast<uintx>(m_AllocInfoMap.size()));

		// we'r accessing heap-internal structures so synchronize via heap lock
		LOCK_HEAP

			// We employ a simple trick here to get a pointer to the first allocated
			// block: just allocate a new block and get the new block's memory header.
			// This works because the most recently allocated block is always placed at
			// the head of the allocated list. We can then walk the list from head to
			// tail. For each block still in out allocation list search for the entry
			// in the crt list.
#if defined(PLATFORM_WINDOWS)
		char *pHeap = new char;
		_CrtMemBlockHeader *pHeader = pHdr(pHeap)->pBlockHeaderNext;
		delete(pHeap);
#endif

		// search through all alloc entries
		for(AllocInfoMap::iterator iter = m_AllocInfoMap.begin(); iter != m_AllocInfoMap.end(); ++iter)
		{
			AllocInfo& info = (*iter).second;
			uintx request = (*iter).first;

#if defined(PLATFORM_WINDOWS)
			// search for entry in the crt list
			_CrtMemBlockHeader *pNext = pHeader;
			while(pNext && pNext->lRequest != request) pNext = pNext->pBlockHeaderNext;

			// write memory leak infos
			if(pNext && pNext->lRequest == request)
				m_Reporter->WriteLeak(request, pbData(pNext), static_cast<uintx>(pNext->nDataSize), info.m_CallstackSize, info.m_Callstack, info.m_ModuleReference, &m_ModuleInfoVector[0]);
			else
				m_Reporter->WriteLeak(request, NULL, 0, info.m_CallstackSize, info.m_Callstack, info.m_ModuleReference, &m_ModuleInfoVector[0]);
#else
			// restore old malloc functions
			//RestoreHooks();

			m_Reporter->WriteLeak(request, reinterpret_cast<void*>(request), info.m_Size, info.m_CallstackSize, info.m_Callstack, info.m_ModuleReference, NULL);

			// set hooking functions
			//SaveHooks();
			//InstallHooks();
#endif
		}

		UNLOCK_HEAP

		// finished reporting
		m_Reporter->WriteFooter();
	}
}

#if defined(PLATFORM_WINDOWS)
//////////////////////////////////////////////////////////////////////////
int CMemLeakDetector::CrtAllocHook(int allocType, void *userData, size_t size, int blockType, long requestNumber, const unsigned char *filename, int lineNumber)
{
	UNREFERENCED_PARAMETER(size);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(lineNumber);

	bool trackAllocation = true;

	// do not memory tracking on crt blocks or if detection is disabled for this thread
	if(blockType == _CRT_BLOCK)
		trackAllocation = false;

	// get the current debug flag bits, only track debug allocations
	// INFO: The _CrtSetDbgFlag does NOT store its flags thread local!
	//       Therefore if one thread deactivates memory tracking, an allocation in another thread could not be tracked.
	//       This MFC default usage behavior and must be implemented, otherwise many MFC memory leak will be reported.
	//       The function does not affect applications without MFC, because this flag is not used by applications without MFC.
	int debugFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	if(!(debugFlags & _CRTDBG_ALLOC_MEM_DF))
		trackAllocation = false;

	// prevent the current thread from re-entering on allocs/reallocs/frees
	// that we or the CRT do internally to record the data we collect.
	if(trackAllocation)
	{
		// handle the allocation request
		switch(allocType)
		{
			case _HOOK_ALLOC:
				g_MemLeakDetector.TrackAllocInfo(requestNumber, 0);
				break;

			case _HOOK_FREE:
				g_MemLeakDetector.RemoveAllocInfo(pHdr(userData)->lRequest);
				break;

			case _HOOK_REALLOC:
				g_MemLeakDetector.RemoveAllocInfo(pHdr(userData)->lRequest);
				g_MemLeakDetector.TrackAllocInfo(requestNumber, 0);
				break;
		}
	}

	if(g_MemLeakDetector.m_OldHook)
		return g_MemLeakDetector.m_OldHook(blockType, userData, size, blockType, requestNumber, filename, lineNumber);
	
	return TRUE;
}

#else	// PLATFORM_WINDOWS

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::InitHooks()
{
	g_MemLeakDetector.m_Reporter = new CDefaultLeakReporter(true, true);
	g_MemLeakDetector.Enable();
}

//////////////////////////////////////////////////////////////////////////
void* CMemLeakDetector::MallocHook(size_t p_Size, const void* p_Caller)
{
	void *result;
	
	// Restore all old hooks
	RestoreHooks();
	
	// Call recursively
	result = malloc(p_Size);
	
	// Save underlying hooks
	SaveHooks();

	// might call malloc, so protect it too.
	if(result)
		g_MemLeakDetector.TrackAllocInfo((uintx)result, (uintx)p_Size);

	// Install our own hooks
	InstallHooks();

	return result;
}

//////////////////////////////////////////////////////////////////////////
void* CMemLeakDetector::ReallocHook(void* p_MemPtr, size_t p_Size, const void* p_Caller)
{
	void *result;

	// Restore all old hooks
	RestoreHooks();

	// Call recursively
	result = realloc(p_MemPtr, p_Size);

	// Save underlying hooks
	SaveHooks();

	// might call malloc, so protect it too.
	if(p_MemPtr)
		g_MemLeakDetector.RemoveAllocInfo((uintx)p_MemPtr);
	if(result)
		g_MemLeakDetector.TrackAllocInfo((uintx)result, (uintx)p_Size);

	// Install our own hooks
	InstallHooks();
	return result;
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::FreeHook(void* p_MemPtr, const void* p_Caller)
{
	// Restore all old hooks
	RestoreHooks();

	// Call recursively
	free(p_MemPtr);

	// Save underlying hooks
	SaveHooks();

	// might call free, so protect it too.
	if(p_MemPtr)
		g_MemLeakDetector.RemoveAllocInfo((uintx)p_MemPtr);
	
	// install our own hooks
	InstallHooks();
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::InstallHooks()
{
	__malloc_hook = CMemLeakDetector::MallocHook;
	__realloc_hook = CMemLeakDetector::ReallocHook;
	__free_hook = CMemLeakDetector::FreeHook;
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::SaveHooks()
{
	CMemLeakDetector::m_OldMallocFunc = __malloc_hook;
	CMemLeakDetector::m_OldReallocFunc = __realloc_hook;
	CMemLeakDetector::m_OldFreeFunc = __free_hook;
}

//////////////////////////////////////////////////////////////////////////
void CMemLeakDetector::RestoreHooks()
{
	__malloc_hook = CMemLeakDetector::m_OldMallocFunc;
	__realloc_hook = CMemLeakDetector::m_OldReallocFunc;
	__free_hook = CMemLeakDetector::m_OldFreeFunc;
}


#endif	// PLATFORM_WINDOWS


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDefaultLeakReporter::CDefaultLeakReporter(bool p_ShowDataBytes, bool p_ShowCallstack, uintx p_MaxLeaksToReport)
	: m_SymbolResolver(NULL), m_MaxLeaksToReport(p_MaxLeaksToReport), m_ShowDataBytes(p_ShowDataBytes), m_ShowCallstack(p_ShowCallstack)
{
	// fill callstack ignore table
#if defined(PLATFORM_WINDOWS)
	m_IgnoreCallstackFrames.push_back(_T("malloc_dbg"));
	m_IgnoreCallstackFrames.push_back(_T("malloc"));
	m_IgnoreCallstackFrames.push_back(_T("realloc_dbg"));
	m_IgnoreCallstackFrames.push_back(_T("realloc"));
	m_IgnoreCallstackFrames.push_back(_T("_heap_alloc_dbg"));
	m_IgnoreCallstackFrames.push_back(_T("_nh_malloc_dbg"));
	m_IgnoreCallstackFrames.push_back(_T("_malloc_dbg"));
	m_IgnoreCallstackFrames.push_back(_T("operator new"));
	m_IgnoreCallstackFrames.push_back(_T("operator new[]"));
	m_IgnoreCallstackFrames.push_back(_T("DbgLib::CDebugHelp::DoStackWalk"));
	m_IgnoreCallstackFrames.push_back(_T("DbgLib::CMemLeakDetector::TrackAllocInfo"));
	m_IgnoreCallstackFrames.push_back(_T("DbgLib::CMemLeakDetector::CrtAllocHook"));
#else
	m_IgnoreCallstackFrames.push_back(_T("*DbgLib*CDebugHelp*DoStackWalk*"));
	m_IgnoreCallstackFrames.push_back(_T("*DbgLib*CMemLeakDetector*TrackAllocInfo*"));
	m_IgnoreCallstackFrames.push_back(_T("*DbgLib*CMemLeakDetector*MallocHook*"));
	m_IgnoreCallstackFrames.push_back(_T("*DbgLib*CMemLeakDetector*ReallocHook*"));
	m_IgnoreCallstackFrames.push_back(_T("*(calloc+0x*"));
	m_IgnoreCallstackFrames.push_back(_T("*(malloc+0x*"));
#endif
}

CDefaultLeakReporter::~CDefaultLeakReporter()
{
	delete m_SymbolResolver;
}

//////////////////////////////////////////////////////////////////////////
void CDefaultLeakReporter::WriteHeader(uintx p_LeaksFound)
{
	assert(m_SymbolResolver == NULL);

	m_SymbolResolver = new CSymbolResolver(CSymbolResolver::GetDefaultSymbolSearchPath().c_str());

	// write leaks to stream
	tostringstream outStream;
	outStream << _T("Detected ") << p_LeaksFound << (" memory leaks!") << std::endl;

	// check how much to dump
	if(m_MaxLeaksToReport > 0 && m_MaxLeaksToReport < p_LeaksFound)
		outStream << _T("Dumping the first ") << m_MaxLeaksToReport << _T(" leaks.");

	// write to debug output
	WriteDebugString(_T("----------------------------------------------------------------------------------------------------\n"));
	WriteDebugString(outStream.str().c_str());
}

//////////////////////////////////////////////////////////////////////////
void CDefaultLeakReporter::WriteFooter()
{
	WriteDebugString(_T("----------------------------------------------------------------------------------------------------\n"));
	delete m_SymbolResolver;
	m_SymbolResolver = NULL;
}

//////////////////////////////////////////////////////////////////////////
void CDefaultLeakReporter::WriteLeak(uintx p_Request, void* p_Data, uintx p_DataSize, uintx p_StackSize, uintx* p_Callstack, intx* p_ModRefs, CMemLeakDetector::ModuleInfo* p_ModuleInfo)
{
	tostringstream leakInfoStream;
	leakInfoStream << _T("- Memleak: {") << std::dec << p_Request << _T("} at 0x") << Internal::Hex(digits_intx) << reinterpret_cast<size_t>(p_Data) << _T(", ") << std::dec << p_DataSize << _T(" bytes");
	if(m_ShowDataBytes)
	{
		// print data buffer in ASCII
		leakInfoStream << _T(" <");
		for(uintx i = 0; i < m_MaxDataBytes && i < p_DataSize; ++i)
		{
			tchar c = static_cast<tchar>(static_cast<byte*>(p_Data)[i]);
			if(c < 32) c = _T('.');
			leakInfoStream << c;
		}
		leakInfoStream << _T(">");

		// print data buffer in HEX
		leakInfoStream << _T(" { ") << Internal::Hex(2);
		for(uintx i = 0; i < m_MaxDataBytes && i < p_DataSize; ++i)
			leakInfoStream << static_cast<uintx>(static_cast<byte*>(p_Data)[i]) << _T(" ");
		leakInfoStream << _T("}");
	}
	WriteDebugString(leakInfoStream.str().c_str());

	// show callstack
	for(uintx i = 0; i < p_StackSize; ++i)
	{
		// get module name
		const tchar* moduleName = NULL;
		ModuleHandle hModule = NULL;
		if(p_ModRefs[i] != -1)
		{
			moduleName = p_ModuleInfo[p_ModRefs[i]].m_ModuleName;
			hModule = p_ModuleInfo[p_ModRefs[i]].m_hModule;
		}

		// resolve debug symbols
		tstring symFunc;
		tstring symFile;
		uintx symLine;
		m_SymbolResolver->ResolveSymbol(p_Callstack[i], hModule, moduleName, symFunc, symFile, symLine);

		// set unknown symbols
		if(symFunc.empty())
			symFunc = _T("Unknown Function");
		if(symFile.empty())
			symFile = _T("Unknown File");

		// ignore some functions
		bool ignore = false;
		for(uintx curEntry = 0; curEntry < m_IgnoreCallstackFrames.size(); ++curEntry)
		{
			if(Internal::WildMatch(m_IgnoreCallstackFrames[curEntry].c_str(), symFunc.c_str()))
			{
				ignore = true;
				break;
			}
		}
		
		if(!ignore)
		{
			// print symbols
			tostringstream stackInfoStream;
			stackInfoStream << _T("    ") << symFile << _T("(") << std::dec << symLine << _T(") : ") << symFunc << _T(" 0x") << Internal::Hex(digits_intx) << p_Callstack[i] << _T(" ");

			// write module name
			if(moduleName)
			{
				stackInfoStream.fill(_T(' '));
				if(stackInfoStream.tellp() < 140)
					stackInfoStream << std::setw(140 - stackInfoStream.tellp()) << _T(" ") << std::setw(1);
				stackInfoStream << _T("(") << moduleName << _T(")");
			}

			// write to debug output
			WriteDebugString(stackInfoStream.str().c_str());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CDefaultLeakReporter::WriteDebugString(const tchar* p_Msg)
{
#if defined(PLATFORM_WINDOWS)
	OutputDebugString(p_Msg);
	OutputDebugString(_T("\n"));
#else
#if defined(_UNICODE)
	fwprintf(stderr, p_Msg);
	fwprintf(stderr, _T("\n"));
#else
	fprintf(stderr, p_Msg);
	fprintf(stderr, _T("\n"));
#endif
#endif
}


}
