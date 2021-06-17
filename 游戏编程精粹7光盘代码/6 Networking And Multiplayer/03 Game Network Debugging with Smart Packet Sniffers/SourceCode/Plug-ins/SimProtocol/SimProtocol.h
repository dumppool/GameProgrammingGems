// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SIMPROTOCOL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SIMPROTOCOL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SIMPROTOCOL_EXPORTS
#define SIMPROTOCOL_API __declspec(dllexport)
#else
#define SIMPROTOCOL_API __declspec(dllimport)
#endif

extern "C"
{

SIMPROTOCOL_API int			fnGetVersion(void);
SIMPROTOCOL_API const char*	fnGetProtocolName(void);
SIMPROTOCOL_API int			fnGetProtocolPort(void);
SIMPROTOCOL_API int			fnGetIsExclusiveAccess(void);
SIMPROTOCOL_API const char*	fnHandlePacket(void* pData, int nSize);

};