
#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define KZOESYS_INCL
#define zNO_KZOEINCO    // to get the correct operation thru zGET_PTR
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#include "KZOENGAA.H"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>

#include "tz__oprs.h"
#include "tzlodopr.h"

#include "ZeidonOp.H"

or...
//************************ This is what is in oclncsv.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define  KZSYSSVC_INCL
#include "kzoengaa.h"
#include "tzvmlip.h"

#ifndef TRUE
   #define TRUE  1
   #define FALSE 0
#endif

////
//#include "ZDRVROPR.H"
//#include "TZVMLIP.H"

#include "ZEIDONOP.H"
#include <wininet.h>
//************************


HINSTANCE g_hInst = 0;
zVIEW     vSubtask = 0;

void
main( int argc, char *argv[], char *envp[] )
{
   // Pass in LPLR
   RegisterZeidonApplication( &vSubtask, (zLONG) g_hInst,
                              0, 0, "\\\\Zencas\\" );
   BuildXODsOnLPLR( vSubtask );

   UnregisterZeidonApplication( vSubtask );
   exit( 0 );
}



zOPER_EXPORT zSHORT OPERATION
BuildXODsOnLPLR( zVIEW vSubtask )
{
   // This routine does not actually save LODs, but only builds the XODs
   // on the LPLR.

   zVIEW  vDTE;
   zVIEW  vLPLR;
   zVIEW  vLOD_List;
   zVIEW  vLOD;
   zVIEW  vXOD;
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szTemp[ zMAX_FILESPEC_LTH + 1 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Save Subtask for use by XOD build routine.
   SetNameForView( vSubtask, "TE_Window", vSubtask, zLEVEL_TASK );

   // For each selected LOD, build the XOD using the current DBMS and
   // commit as file to LPLR, unless the LOD does not have a POD.
   // Note that we are not looking to see if the DBMS identified
   // in the POD.TE_SourceZKey matches the current DBMS. We will save
   // selected LODs with the current DBMS no matter how they were saved
   // last time.

   for ( nRC = SetCursorFirstSelectedEntity( vLOD_List, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vLOD_List, "W_MetaDef", "" ) )
   {
      GetStringFromAttribute( szLOD_Name, zsizeof( szLOD_Name ), vLOD_List, "W_MetaDef", "Name" );

      nRC = ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zCURRENT_OI );
      if ( nRC < 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Could not Activate LOD: " );
         strcat_s( szMsg, zsizeof( szMsg ), szLOD_Name );
         strcat_s( szMsg, zsizeof( szMsg ), ".\nAborting Build" );
         MessageSend( vSubtask, "TE00426", "Physical Data Model",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
      else
      {
         // Send message that we are building the LOD.
         strcpy_s( szMsg, zsizeof( szMsg ), "Building executable for LOD: " );
         strcat_s( szMsg, zsizeof( szMsg ), szLOD_Name );
         strcat_s( szMsg, zsizeof( szMsg ), "." );
         MB_SetMessage( vSubtask, 1, szMsg );

         // Make sure the TE_SourceZKey attribute is set because it determines
         // what DBMS_Source is used in building the XOD.
         if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET )
            SetAttributeFromAttribute( vLOD, "POD", "TE_SourceZKey",
                                       vDTE, "TE_DBMS_Source", "ZKey" );

         // Build the XOD in memory.
         oTZZOXODO_SaveXOD( vSubtask, vLOD );
         SetNameForView( vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
         GetViewByName( &vXOD, "TZZOXODO", vSubtask, zLEVEL_TASK );
         DropMetaOI( vSubtask, vLOD );

         // Commit the XOD to LPLR file.
         GetStringFromAttribute( szTemp, zsizeof( szTemp ), vLPLR, "LPLR", "ExecDir" );
         SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), szTemp );
         ofnTZCMWKSO_AppendSlash( szFileName );
         strcat_s( szFileName, zsizeof( szFileName ), szLOD_Name );
         strcat_s( szFileName, zsizeof( szFileName ), ".XOD" );
         TraceLineS( "*** Committing workstation file: ", szFileName );
         CommitOI_ToFile( vXOD, szFileName, zSINGLE );
      }
   }

   MB_SetMessage( vSubtask, 1, "Build of XODs complete." );
   return( 0 );
}

