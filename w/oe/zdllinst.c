
#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include <windows.h>

HINSTANCE g_hInstanceDLL = 0;

#ifdef __WIN32__
/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:    DllMain
//
// INPUTS:      hDLL       - handle of DLL
//              dwReason   - indicates why DLL called
//              lpReserved - reserved
//
// RETURNS:     TRUE (always, in this example.)
//
//              The function should return TRUE if DLL initialization
//              succeeded, or FALSE it it failed.
//
// GLOBAL VARS: ghMod - handle of DLL (initialized when PROCESS_ATTACHes)
//
// COMMENTS:    The function will display a dialog box informing user of
//              each notification message & the name of the attaching/
//              detaching process/thread. For more information see
//              "DllMain" in the Win32 API reference.
//
/////////////////////////////////////////////////////////////////////////////
BOOL WINAPI
DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved )
{
  switch ( dwReason )
  {
      case DLL_PROCESS_ATTACH:
         g_hInstanceDLL = (HINSTANCE) hDLL;
         break;

      case DLL_THREAD_ATTACH:
         break;

      case DLL_THREAD_DETACH:
         break;

      case DLL_PROCESS_DETACH:
         break;
   }

   return( TRUE );
}

zOPER_EXPORT HINSTANCE OPERATION
GetDLL_Instance( zVIEW vSubtask )
{
   return( g_hInstanceDLL );
}

#else

/***********************************************************************

   LibMain and WEP - required for Windows

***********************************************************************/
// does not need to be exported since it is called only from libentry.obj
int FAR PASCAL
LibMain( HANDLE  hLibInst,
         WORD    wDataSeg,
         WORD    cbHeapSize,
         LPSTR   lpszCmdLine )
{
   if ( cbHeapSize <= 0 )   /* dll did not get loaded properly */
      return( 0 );

   /* any initialization code should go here */
   g_hInstanceDLL = (HINSTANCE) hLibInst;
   UnlockData( 0 );
   return( 1 );
}

int FAR PASCAL
WEP( int  bSystemExit )
{
   return( 1 );
}

#endif
