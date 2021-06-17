Article: Advanced Debuggin Techniques
Author: Martin Fleisz

====================================================================================
Requirements:
  Windows
    - Microsoft Visual Studio 2005
    - Latest version of the Microsoft Debugging Tools SDK (available at http://www.microsoft.com/whdc/devtools/debugging/default.mspx).
    - Latest version of the Vista SDK (available at: http://www.microsoft.com/downloads/details.aspx?familyid=4377f86d-c913-4b5c-b87e-ef72e5b4e065&displaylang=en).
    - Ensure that the latest version of dbghelp.dll is either in the application directory or 
      in your system directory. If the library loads an older version it might assert() or 
      behaves undefined when calling Debug Helper API functions.
      
  Unix
    - Tested on Fedora Linux 6 with gcc version 4.0.x and glibc version 2.5
    
====================================================================================
Debug Library Notes

  Windows
    - Memory leak detection is only active in debug builds. It's not possible to install allocation hooks
      in Release builds because the CRT removes all calls to _CrtSetAllocHook from user code.
    - Under some circumstances, it can happen that your application dead-locks when using the memory leak
      detector and loading dlls that dynamically allocate memory in DllMain. This is caused by the Windows
      load lock that is held by the OS when loading an application. The same lock is acquired by 
      GetModuleFileName which is called inside the allocation hook. If an allocation is done from within
      DllMain the OS will hold this lock, while GetModuleFileName waits for it to be released - then your
      application will be dead-lock. If this is a problem you can disable the module info caching using
      CMemLeakDetector::SetModuleInfoCaching. However, if you have leaks in dlls that you dynamically
      unload during runtime the reporter won't be able to resolve the symbols for the callstack frames
      of this dll.
    - if you #define USE_WER when compiling the Debug Library the Exception Handler will use Vista's WER API 
      to report errors. This define can also be enabled when you compile for platforms that don't support WER.
      In this case the standard reporting (mini dump creation) is performed. In case you enable WER you should
      modify the CDebugHelp::CreateWERReport function so that the report contains the information you want
      and need (i.e. additional log files of your game, etc.).
  
  Unix
    - You might want to consider to synchronize the CDebugHelp::CreateMiniDump function as the coredumper 
      API is not thread safe (see documentation for WriteCoreDump in the coredumper library)
    - You might need to adjust the makefile provided with the code to build the library for your target platform
    - Memory leak detection is only available if the library is build with _DEBUG defined
    - For Unicode support build the library with _UNICODE defined

====================================================================================
Usage:
  You will need to include DbgLib.h into your project (i.e. into your precompiled header file).
  By default, memory leak detection is enabled automatically when your application is linked 
  with DbgLib. To change this behavior undefine AUTO_ENABLE_MEMLEAKDETECTOR in the stdafx.h
  header of the library.  
  The API exposed by DbgLib is not thread safe!
  
  To install the unhandled exception handler use the following line of code.
 
    DbgLib::CDebugFx::SetExceptionHandlers(true);
  
  Additionally you can install a custom exception handler callback that is executed after the exception
  handler has finished collectin information about the crash. Here is an example:
  
    void MyHandler(void* p_GameInstance)
    {
       static_cast<IGame*>(p_GameInstance)->SaveGame();
    }
    ...
    DbgLib::CDebugFx::SetExceptionCallback(MyHandler, &gameObject);
  
  If you turned of the auto-registration of the memory leak detector you can active tracking
  in your application with the following code:
  
    int main(..)
    {
	   using namespace DbgLib;
       
       CMemLeakDetector* detector = CDebugFx::GetMemLeakDetector();
       
       // optional: set a custom reporter
       detector->SetReporter(new CMyReporter);
       // enable tracking
       detector->Enable();
       
       // game starts
       ....
       
       // disable tracking
       detector->Disable();
       // game finished - all resources were freed(?) - report leaks
       detector->ReportLeaks();
    }
    
