// DbgLibSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// helper function to cause a stack overflow
void Recursive(int recurse)
{
	alloca(2000);
	if (recurse)
		Recursive(recurse);
}

// exception callback function
void ExceptionCallback(void*)
{
#if defined(_WIN32)
	MessageBox(NULL, _T("This message box comes from our exception callback!"), _T("huch!"), MB_OK);
#else
	_tprintf(_T("This message comes from our exception callback!"));
#endif
}

// this class instance leaks memory in its constructor
class ILeak
{
	public:
		ILeak() { new char[20]; }
};
ILeak g_leaK;


int _tmain(int argc, TCHAR* argv[])
{
	// enable exception handlers
	DbgLib::CDebugFx::SetExceptionHandler(true);

	// install an exception callback
	DbgLib::CDebugFx::SetExceptionCallback(ExceptionCallback, NULL);

	_tprintf(_T("This program demonstrates the unhandled exception handling and memory leak detection of the debug library.\n"));
	_tprintf(_T("To see the beautiful memory leak detection start the application under the Visual Studio debugger or use DebugView (can be found via Google) to see the output!\n\n"));
	_tprintf(_T("Choose one of the following options:\n"));
	_tprintf(_T("  Press 1 to cause an unhandled exception caused by accessing a NULL pointer\n"));
	_tprintf(_T("  Press 2 to cause an unhandled exception caused by a stack overflow\n"));
	_tprintf(_T("  Press any other key to abort\n"));
	TCHAR option = _gettchar();

	// either crash using a stack overflow, or a NULL pointer access, if we do nothing of these we "just" leak memory
	switch(option)
	{
		case _T('1'):
			{
				int* i = NULL;
				*i = 1;
			}
			break;
		
		case _T('2'):
			{
				Recursive(1);
			}
			break;

		default:
			break;
	}

	// another leak
	malloc(10);

	return 0;
}

