/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrvctrl.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of visit control functions
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//    Handle case where mapping all but one ctrl on a window.
//
// 1999.02.10    DKS
//    Initially grayed controls properly grayed/enabled in
//    subsequent iterations.
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Function to process the controls to disable/enable based on setting
// ASYNC processing ... this does not set the MapAct flag for the control
// which determines whether the control is enabled or disabled.
void
fnEnableDisableCtrls( ZMapAct *pzma,
                      WPARAM  wParam,
                      LPARAM  lParam )
{
   if ( pzma && (pzma->m_ulMapActFlags & zMAPACT_ENABLED) )
      pzma->m_pCtrl->EnableWindow( (zBOOL) wParam );
}

// Function to process the controls in order to initially disable them
// based on the value specified by the user in common detail.
void
fnDisableCtrls( ZMapAct *pzma,
                WPARAM  wParam,
                LPARAM  lParam )
{
   if ( pzma && ((pzma->m_ulMapActFlags & zMAPACT_ENABLED) == 0) )
   {
      // Save and restore flags since we don't want to override AUTOGRAY
      // settings at this point.
      zULONG ulMapActFlags = pzma->m_ulMapActFlags;
      pzma->SetZCtrlState( zCONTROL_STATUS_ENABLED, FALSE );
      pzma->m_ulMapActFlags = ulMapActFlags;
   }
}

// Function to process the controls in order to clear the bCtrlErrFlag.
void
fnClearCtrlErr( ZMapAct *pzma,
                WPARAM  wParam,
                LPARAM  lParam )
{
   if ( pzma && pzma->m_ulCtrlErr )
      pzma->SetZCtrlState( zCONTROL_STATUS_ERROR, FALSE );
}

// Function to process the controls in order for mapping to the OI.
void
fnMapCtrlsToOI( ZMapAct *pzma,
                WPARAM  wParam,
                LPARAM  lParam )
{
#if 0 //def DEBUG_ALL lots of messages
   if ( pzma )
   {
      TraceLineS( "Map to OI from control: ", *(pzma->m_pzsTag) );
      if ( zstrcmp( *(pzma->m_pZSubtask->m_pzsWndTag), "Invoice" ) == 0 &&
           pzma->m_pzsAName && pzma->m_pzsEName )
      {
         zVIEW vApp;

         GetViewByName( &vApp, "mInvO",
                        pzma->m_pZSubtask->m_vDialog, zLEVEL_ANY );
      // GetViewByName( &vApp, *(pzma->m_pzsVName),
      //                pzma->m_pZSubtask->m_vDialog, zLEVEL_ANY );
         if ( vApp )
         {
            DisplayEntityInstance( vApp, "ReceiptApplied" );
         // DisplayEntityInstance( vApp, *(pzma->m_pzsEName) );
         }
      }
   }
#endif
   // This code lets only one mapping error occur per iteration.
   if ( pzma &&
        pzma->m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr == 0 &&
        pzma->MapToOI( ) < 0 )
   {
      pzma->SetZCtrlState( zCONTROL_STATUS_ERROR, TRUE );
      pzma->m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr = (zPVOID) pzma;
   }
}

// Function to process the controls in order for mapping from the OI.
// lParam is a pointer to the unique name of the control which is NOT
// to be refreshed.  This may be a semi-colon separated list of controls
// if the first character is a tab character.
zBOOL
fnFindCtrlInList( ZMapAct *pzma, zCPCHAR cpcCtrlList )
{
   if ( cpcCtrlList && *cpcCtrlList )
   {
      if ( *cpcCtrlList == '\t' )
      {
         zPCHAR pch = (zPCHAR) cpcCtrlList + 1;
         zPCHAR pchSemi;
         while ( pch && *pch )
         {
            pchSemi = zstrchr( pch, ';' );
            if ( pchSemi )
               *pchSemi = 0;

            if ( zstrcmp( (*(pzma->m_pzsTag)).GetString(), pch ) == 0 )
            {
               if ( pchSemi )
                  *pchSemi = ';';

               return( TRUE );
            }

            if ( pchSemi )
            {
               *pchSemi = ';';
               pch = pchSemi + 1;
            }
            else
               pch = 0;
         }
      }
      else
      {
         if ( zstrcmp( (*(pzma->m_pzsTag)).GetString(), cpcCtrlList ) == 0 )
            return( TRUE );
      }
   }

   return( FALSE );
}

void
fnMapCtrlsFromOI( ZMapAct *pzma,
                  WPARAM  wParam,
                  LPARAM  lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "MapCtrlsFromOI: ", *(pzma->m_pzsTag) );
   if ( zstrcmp( *(pzma->m_pzsTag), "Text42" ) == 0 )
      TraceLineS( "Before MapCtrlsFromOI: ", *(pzma->m_pzsTag) );
#endif

   // Map all except the one(s) specified (if any).
   if ( pzma &&
        (lParam == 0 || fnFindCtrlInList( pzma, (zCPCHAR) lParam ) == FALSE) )
   {
#ifdef DEBUG_ALL
      if ( zstrcmp( *(pzma->m_pZSubtask->m_pzsWndTag), "Invoice" ) == 0 &&
           pzma->m_pzsAName && pzma->m_pzsEName )
      {
         zVIEW vApp;

         GetViewByName( &vApp, "mInvO",
                        pzma->m_pZSubtask->m_vDialog, zLEVEL_ANY );
      // GetViewByName( &vApp, *(pzma->m_pzsVName),
      //                pzma->m_pZSubtask->m_vDialog, zLEVEL_ANY );
         if ( vApp )
         {
            DisplayEntityInstance( vApp, "ReceiptApplied" );
         // DisplayEntityInstance( vApp, *(pzma->m_pzsEName) );
         }
      }
#endif

      pzma->MapFromOI( wParam );

#ifdef DEBUG_ALL
   // if ( zstrcmp( *(pzma->m_pzsTag), "Text42" ) == 0 ||
   //      zstrcmp( *(pzma->m_pzsTag), "Text44" ) == 0 )
      if ( zstrcmp( *(pzma->m_pZSubtask->m_pzsWndTag), "Invoice" ) == 0 &&
           pzma->m_pzsAName && pzma->m_pzsEName )
      {
         zVIEW vApp;

         TraceLineS( "After MapCtrlsFromOI: ", *(pzma->m_pzsTag) );
         GetViewByName( &vApp, "mInvO",
                        pzma->m_pZSubtask->m_vDialog, zLEVEL_ANY );
         if ( vApp )
            DisplayEntityInstance( vApp, "ReceiptApplied" );
      }
#endif
   }
}

void
fnPostCreateCtrls( ZMapAct *pzma,
                   WPARAM  wParam,
                   LPARAM  lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "PostCreateCtrls: ", *(pzma->m_pzsTag) );
#endif

   // Notify all except the one(s) specified (if any).
   if ( pzma &&
        (lParam == 0 || fnFindCtrlInList( pzma, (zCPCHAR) lParam ) == FALSE) )
   {
      pzma->PostCreateCtrls( wParam );
   }
}

void
fnPostMapCtrlsFromOI( ZMapAct *pzma,
                      WPARAM  wParam,
                      LPARAM  lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "PostMapCtrlsFromOI: ", *(pzma->m_pzsTag) );
#endif

   // Notify all except the one(s) specified (if any).
   if ( pzma &&
        (lParam == 0 || fnFindCtrlInList( pzma, (zCPCHAR) lParam ) == FALSE) )
   {
      pzma->PostMapFromOI( wParam );
   }
}

// Function to process the controls in order for resize.
void
fnSendCtrlsParentResize( ZMapAct *pzma,
                         WPARAM  wParam,
                         LPARAM  lParam )
{
   if ( pzma )
      pzma->ParentResize( (ZMapAct *) wParam, (CSize *) lParam );
}

// Function to process the controls in order for move.
void
fnSendCtrlsParentMove( ZMapAct *pzma,
                       WPARAM  wParam,
                       LPARAM  lParam )
{
   if ( pzma )
      pzma->ParentMove( );
}

// Function to process the controls in order for notification of
// palette changed.
void
fnSendCtrlsPaletteChanged( ZMapAct *pzma,
                           WPARAM  wParam,
                           LPARAM  lParam )
{
   if ( pzma )
      pzma->PaletteChanged( );
}

// Function to process the controls in order for notification of
// parent activated.
void
fnSendCtrlsParentActivated( ZMapAct *pzma,
                            WPARAM  wParam,
                            LPARAM  lParam )
{
   if ( pzma )
      pzma->ParentActivated( );
}

// Function to process the controls in order.
void
fnOrderedCtrls( const ZMapAct *pzma,
                WPARAM wParam,
                LPARAM lParam )
{
   zCHAR szBuffer[ 64 ];
   strcpy_s( szBuffer, zsizeof( szBuffer ), *(pzma->m_pzsTag) );
   strcat_s( szBuffer, zsizeof( szBuffer ), ": " );
   TraceLineI( szBuffer, pzma->m_nIdNbr );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CreateCommaSeparatedList
//
//    N.B.  The user is responsible for calling DeleteCommaSeparatedList
//          with the handle returned from this call.  If not, a memory
//          leak will occur!!!
//
//    lFlags - 0x00000001 ==> use only selected entities
//             0x00000002 ==> use only parent-selected entities
//             0x00000004 ==> use semi-colon to separate (rather than comma)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
CreateCommaSeparatedList( zVIEW   v,
                          zCPCHAR cpcEntityName,
                          zCPCHAR cpcAttributeName,
                          zCPCHAR cpcContext,
                          zCPCHAR cpcScope,
                          zLONG   lFlags )
{
   zCHAR  szParentEntity[ zTAG_LTH ];
   zPCHAR lpMemory;
   zLONG  lEntityCnt;
   zULONG ulAttributeLth;
   zLONG  lTotalSize;
   zLONG  lLth = 0;
   zCHAR  cSeparator;

   zSHORT nRC;

   if ( (lFlags & 0x00000002) )
   {
      MiGetParentEntityNameForView( szParentEntity, v, cpcEntityName );
   // if ( szParentEntity[ 0 ] )
   //    SetCursorFirstEntity( v, szParentEntity, cpcScope );
   }

   if ( (lFlags & 0x00000004) == 0 )
      cSeparator = ',';
   else
      cSeparator = ';';

   lEntityCnt = CountEntitiesForView( v, cpcEntityName );
   GetAttributeDisplayLength( &ulAttributeLth, v, cpcEntityName,
                              cpcAttributeName, cpcContext );
   lTotalSize = lEntityCnt * (zLONG) ulAttributeLth;  // a starting point
   DrAllocTaskMemory( (zCOREMEM) &lpMemory, lTotalSize + 1 );

   // For each entity, append the specified data to the list.
// nRC = SetCursorFirstEntity( v, cpcEntityName, cpcScope );
   nRC = SetEntityCursor( v, cpcEntityName, 0, zPOS_FIRST,
                          0, 0, 0, 0, cpcScope, 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( (lFlags & 0x00000001) == 0 ||
           ((lFlags & 0x00000001) &&
            GetSelectStateOfEntity( v, cpcEntityName ) != 0) ||
           ((lFlags & 0x00000002) &&
            GetSelectStateOfEntity( v, szParentEntity ) != 0) )
      {
         GetVariableFromAttribute( lpMemory + lLth, 0, zTYPE_STRING,
                                   lTotalSize - lLth - 1, v,
                                   cpcEntityName, cpcAttributeName,
                                   cpcContext,
                                   cpcContext && *cpcContext ?
                                      0: zUSE_DEFAULT_CONTEXT );
         lLth = zstrlen( lpMemory );
         while ( lLth > 0 && lpMemory[ lLth - 1 ] == ' ' )
         {
            lLth--;
            lpMemory[ lLth ] = 0;
         }
      }

   // nRC = SetCursorNextEntity( v, cpcEntityName, cpcScope );
      nRC = SetEntityCursor( v, cpcEntityName, 0, zPOS_NEXT,
                             0, 0, 0, 0, cpcScope, 0 );
      if ( nRC > zCURSOR_UNCHANGED )
      {
      // lLth = zstrlen( lpMemory );
         if ( lTotalSize - lLth < (zLONG) ulAttributeLth )
         {
            zPCHAR lpTemp;

            lEntityCnt *= 2;
            lTotalSize = lEntityCnt * (zLONG) ulAttributeLth;

            DrAllocTaskMemory( (zCOREMEM) &lpTemp, lTotalSize + 1 );
            zmemcpy( lpTemp, lpMemory, lLth );
            DrFreeTaskMemory( lpMemory );
            lpMemory = lpTemp;
         }

         if ( lLth > 0 && lpMemory[ lLth - 1 ] != cSeparator )
         {
            lpMemory[ lLth++ ] = cSeparator;
            lpMemory[ lLth ] = 0;
         }
      }
   }

   return( (zLONG) lpMemory );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DeleteCommaSeparatedList
//
//    N.B.  The user is responsible for calling DeleteCommaSeparatedList
//          with the handle returned from CreateCommaSeparatedList.  If not,
//          a memory leak will occur!!!
//          After calling DeleteCommaSeparatedList, the handle can no longer
//          be used!!!
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG /*GLOBAL */  OPERATION
DeleteCommaSeparatedList( zLONG lpMemory )
{
   DrFreeTaskMemory( (zPCHAR) lpMemory );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: CreateMemoryHandle
//
// N.B.  The user is responsible for calling DeleteMemoryHandle
//       with the handle returned from this call.  If not, a memory
//       leak will occur!!!
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
CreateMemoryHandle( zLONG lInitialSize )
{
   ZDrMemory *pMemory = new ZDrMemory( lInitialSize );
   return( (zLONG) pMemory );
}

zOPER_EXPORT void OPERATION
DeleteMemoryHandle( zLONG hMemory )
{
   ZDrMemory *pMemory = (ZDrMemory *) hMemory;
   mDeleteInit( pMemory );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: AddAttributeToMemory
//
// Returns: >= 0 - Resultant length of string in memory
//                 Attribute successfully retrieved
//           < 0 - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG  OPERATION
AddAttributeToMemory( zLONG    hMemory,
                      zVIEW    vApp,
                      zCPCHAR  cpcEntityName,
                      zCPCHAR  cpcAttributeName,
                      zLONG    lFlag )
{
   ZDrMemory *pMemory = (ZDrMemory *) hMemory;
   zPCHAR pch;
   zLONG  nRC;

   nRC = GetAddrForAttribute( &pch, vApp, cpcEntityName, cpcAttributeName );
   if ( nRC == 0 )
      return( pMemory->AddStringToMemory( pch, lFlag ) );
   else
      return( zCALL_ERROR );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: AddStringToMemory
//
// Parameters:  cpcString - address of string to add
//              lFlag - 0 ==> just add string as is
//                      1 ==> add string surrounded by single quotes
//                      2 ==> add string surrounded by double quotes
//                      4 ==> add string and terminate with CRLF
//
// Returns: Resultant length of string in memory
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG  OPERATION
AddStringToMemory( zLONG    hMemory,
                   zCPCHAR  cpcString,
                   zLONG    lFlag )
{
   ZDrMemory *pMemory = (ZDrMemory *) hMemory;
   return( pMemory->AddStringToMemory( cpcString, lFlag ) );
}

zOPER_EXPORT void  OPERATION
ClearMemory( zLONG    hMemory )
{
   ZDrMemory *pMemory = (ZDrMemory *) hMemory;
   pMemory->ClearMemory( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: SetMemoryToAttribute
//
// Returns: 0  - Attribute successfully set
//         otw - Error setting attribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SetMemoryToAttribute( zLONG    hMemory,
                      zVIEW    vApp,
                      zCPCHAR  cpcEntityName,
                      zCPCHAR  cpcAttributeName )
{
   ZDrMemory *pMemory = (ZDrMemory *) hMemory;
   return( SetAttributeFromString( vApp, cpcEntityName, cpcAttributeName,
                                   pMemory->GetStringFromMemory( ) ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZDrMemory::ZDrMemory( zLONG lInitialSize )
{
   if ( lInitialSize < 8192 )
      lInitialSize = 8192;

   DrAllocTaskMemory( (zCOREMEM) &m_pchMemory, lInitialSize + 1 );
   m_lMemorySize = lInitialSize;
   m_lUsed = 0;
}

ZDrMemory::~ZDrMemory( )
{
   DrFreeTaskMemory( (zCOREMEM) &m_pchMemory );
}

// Parameters:  cpcString - address of string to add
//              lFlag - 0 ==> just add string as is
//                      1 ==> add string surrounded by single quotes
//                      2 ==> add string surrounded by double quotes
//                      4 ==> add string terminated by CRLF
// Returns: Resultant length of string in memory
zLONG
ZDrMemory::AddStringToMemory( zCPCHAR cpcString, zLONG lFlag )
{
   zLONG lLth = zstrlen( cpcString );
   zLONG lAddedLth = 0;
   zCHAR chQuote = 0;

   if ( lFlag & 3 )
   {
      lAddedLth += 2;
      if ( lFlag & 1 )
         chQuote = '\'';
      else
         chQuote = '"';
   }

   if ( lLth & 4 )
      lAddedLth += 2;

   if ( lLth + m_lUsed + lAddedLth >= m_lMemorySize )
   {
      zPCHAR pch;
      zLONG  lTemp;

      if ( lLth > m_lMemorySize )
         lTemp = lLth * 2 + lAddedLth;
      else
         lTemp = m_lMemorySize * 2 + lAddedLth;

      DrAllocTaskMemory( (zCOREMEM) &pch, lTemp + 1 );
      strcpy_s( pch, lTemp + 1, m_pchMemory );
      DrFreeTaskMemory( m_pchMemory );
      m_pchMemory = pch;
   }

   if ( chQuote )
      m_pchMemory[ m_lUsed++ ] = chQuote;

   strcpy_s( m_pchMemory + m_lUsed, m_lMemorySize - m_lUsed, cpcString );
   m_lUsed += lLth;
   if ( chQuote )
   {
      m_pchMemory[ m_lUsed++ ] = chQuote;
      m_pchMemory[ m_lUsed ] = 0;
   }

   if ( lLth & 4 )
   {
      m_pchMemory[ m_lUsed++ ] = '\r';
      m_pchMemory[ m_lUsed++ ] = '\n';
      m_pchMemory[ m_lUsed ] = 0;
   }

   return( m_lUsed );
}