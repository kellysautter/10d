#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define  KZSYSSVC_INCL
#include "kzoengaa.h"

#ifndef TRUE
   #define TRUE  1
   #define FALSE 0
#endif

HINSTANCE g_hInst = 0;
zVIEW     vSubtask = 0;

void PrintHelp( void )
{
   printf( "SyncXOD: Changes an XOD to match ZEIDON.XDM in the current dir.\n\n" );
   printf( "      Syntax: SYNCXOD xod-name\n\n" );
}

void main ( int argc, char *argv[], char *envp[] )
{
   zSHORT  nRC;
   zVIEW   vXOD, vXDM;
   zCHAR   szEntityName[ 50 ];
   zUSHORT usReturnLevel;
   zCHAR   szXOD[ 20 ];
   zCHAR   szFile[ 50 ];

   if ( argc != 2 || *argv[ 1 ] == '?' || argv[ 1 ][ 1 ] == '?' )
   {
      PrintHelp();
      exit( 1 );
   }

   RegisterZeidonApplication( &vSubtask,
                              (zLONG) g_hInst,
                              (zLONG) 0,
                              (zLONG) 0,
                              "\\\\Zeidon_Tools\\", 0, 0 );

   printf( "Converting XOD: %s\n", argv[ 1 ] );
   sprintf( szXOD, ".\\%s.XOD", argv[ 1 ] );

   if ( ActivateOI_FromFile( &vXOD, "TZZOXODO", vSubtask, szXOD, zSINGLE ) != 0 )
   {
      printf( "Couldn't load %s\n", szXOD );
      exit( 1 );
   }

   sprintf( szFile, ".\\%s.BAK", argv[ 1 ] );
   CommitOI_ToFile( vXOD, szFile, 0 );

   if ( ActivateOI_FromFile( &vXDM, "TZDMXGPO", vSubtask, ".\\ZEIDON.XDM",
                             zSINGLE ) != 0 )
   {
      printf( "Couldn't load ZEIDON.XDM\n" );
      exit( 1 );
   }

   // Initialize szEntityName to "ATTRIB" so first hier csr is processed.
   strcpy_s( szEntityName, zsizeof( szEntityName ), "ATTRIB" );

   for ( nRC = DefineHierarchicalCursor( vXOD, "ATTRIB" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityHierarchical( &usReturnLevel,
                                                szEntityName, vXOD ) )
   {
      zPCHAR psz;

      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
         SetViewToSubobject( vXOD, "CHILDENTITY" );

      if ( zstrcmp( szEntityName, "ATTRIB" ) != 0 )
         continue;

      GetAddrForAttribute( &psz, vXOD, "ATTRIB", "NAME" );
      printf( "Attribute Name = %s\n", psz );

      GetAddrForAttribute( &psz, vXOD, "ATTRIB", "DOMAIN" );
      if ( SetCursorFirstEntityByString( vXDM, "Domain", "Name",
                                         psz, 0 ) >= zCURSOR_SET )
      {
         SetAttributeFromAttribute( vXOD, "ATTRIB", "APDM_TOK",
                                    vXDM, "Domain", "ZKey" );
         SetAttributeFromAttribute( vXOD, "ATTRIB", "TYPE",
                                    vXDM, "Domain", "DataType" );
      }
      else
         SysMessageBox( "Core Tool", "Couldn't match Domain name", 0 );
   }

   DropHierarchicalCursor( vXOD );

   sprintf( szFile, ".\\%s.XOD", argv[ 1 ] );
   CommitOI_ToFile( vXOD, szFile, 0 );

   exit( 0 );
}


BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
  switch ( dwReason )
  {
      case DLL_PROCESS_ATTACH:
         g_hInst = hDLL;
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
