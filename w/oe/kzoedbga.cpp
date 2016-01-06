// kzoedbga.cpp : Defines the entry point for the DLL application
//

/*
CHANGE LOG
2001.03.12  US
   call of AfxDumpStack only if the define DEBUG is set.
   The call caused a delay when starting and shutting down
   all zeidon applications especially in release versions, because
   the call causes the system to search the disk for debug files
*/

#define _USING_V110_SDK71_ 1
#define _UNICODE
#define _WIN32_WINNT _WIN32_WINNT_MAXVER

//#include <windows.h>
#include <afx.h>
#include "kzoedbga.h"

extern "C" {

// This dumps the current stack to debug trace.
KZOEDBGA_API void zDumpStack(void)
{
#if defined DEBUG
   AfxDumpStack( AFX_STACK_DUMP_TARGET_TRACE );
#else
   return;
#endif
}

KZOEDBGA_API void zDumpString( char * psz )
{
//   TRACE0( psz );
}

} // extern "C"...

