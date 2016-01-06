/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:  A simple implementation of a directory services DLL.
//               This simply reads the appl server address from the INI file
//               and stores it in the DBH system OI.
//
// OPERATIONS:
//
/////////////////////////////////////////////////////////////////////////////
//    Copyright (c) 1993-2010 QuinSoft, Corporation. All rights reserved.
//    Confidential and Proprietary material subject to license - do not
//    reproduce or disclose.  This material is an unpublished work and
//    is considered a trade secret belonging to the copyright holder.
/////////////////////////////////////////////////////////////////////////////
//
//  AUTHOR:  DG Christensen
//  DATE:    6/29/2001
//  API:     Zeidon Core
//
/////////////////////////////////////////////////////////////////////////////

/*
CHANGE LOG
*/

#define  KZOESYS_INCL
#include <kzoengaa.h>     // Object Services public prototypes
#include <stdio.h>

zSHORT OPERATION
zServerDirInit( LPTASK  lpTask,
                zPCHAR  pchNetworkName,
                zPPVOID pvData,
                zLONG   lControl )
{
   // No init to do.
   *pvData = 0;

   return( 0 );
}

zSHORT OPERATION
zServerDirClose( LPTASK  lpTask,
                 zPPVOID pvData,
                 zLONG   lControl )
{
   // No close to do.
   return( 0 );
}

// Returns the server address specified in the system DBH object.
zSHORT OPERATION
zServerDirGetAddr( LPTASK  lpTask,
                   zPCHAR  pchReturnAddress,
                   zPPVOID pvData,
                   zLONG   lRequest,
                   zPVOID  pvInfoStruct )
{
   LPSERVDIRBASE lpBaseInfo = (LPSERVDIRBASE) pvInfoStruct;
   zVIEW         vTask;
   zVIEW         vOI;

   // We'll only allow one attempt to the server, so if the RETRY flag is
   // set, we'll return an error.
   if ( lRequest & zAPPLREQ_RETRY )
      return( zCALL_ERROR );

   // If the user supplied an address, we can just use that address.
   if ( lpBaseInfo->pszUserSuppliedAddress && lpBaseInfo->pszUserSuppliedAddress[ 0 ] )
   {
      strcpy_s( pchReturnAddress, 33, lpBaseInfo->pszUserSuppliedAddress );
      return( 0 );
   }

   // Set lock so nobody can modify the OI while we're using it.
   vTask = SysGetPointerFromHandle( lpTask->hFirstView );
   GetViewByName( &vOI, "Zeidon DBHandler Work Object", vTask, zLEVEL_SYSTEM );
   SysMutexLock( vTask, "Zeidon DBH", 0, 0 );

   // Have we already created the Group for ApplServ?
   if ( SetCursorFirstEntityByString( vOI, "Group", "Name", "ApplServ", 0 ) < zCURSOR_SET )
   {
      // No...create it.
      CreateEntity( vOI, "Group", zPOS_LAST );
      SetAttributeFromString( vOI, "Group", "Name", "ApplServ" );
      SetAttributeFromString( vOI, "Group", "Desc",
                              "Set from zSimpDir.DLL to determine the address "
                              "of the application server." );
   }

   // Have we already created the key name for the server address?
   if ( SetCursorFirstEntityByString( vOI, "Key", "Name", "ServerAddress", 0 ) < zCURSOR_SET )
   {
      zCHAR szGroup[ 4096 ];

      // No...create it and retrieve the original value from the INI file.
      CreateEntity( vOI, "Key", zPOS_LAST );
      SetAttributeFromString( vOI, "Key", "Name", "ServerAddress" );
      SetAttributeFromString( vOI, "Key", "Desc", "Current address of the appl server" );

      // Read the address from the zeidon ini.
      sprintf_s( szGroup, sizeof( szGroup ), "[%s]", lpBaseInfo->pszNetworkName );
      SysReadZeidonIni( -1, szGroup, "ApplServer", pchReturnAddress, 33 );  // note that this 33 length has not been verified ... dks 2015.11.17
      SetAttributeFromString( vOI, "Key", "Value", pchReturnAddress );
   }
   else
      GetStringFromAttribute( pchReturnAddress, 33, vOI, "Key", "Value" );

   SysMutexUnlock( vTask, "Zeidon DBH", 0 );

   return( 0 );
}
