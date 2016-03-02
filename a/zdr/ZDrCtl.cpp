/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrctl.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Controls (CWnd).
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2001.03.02    DKS   Z10
//    Minor fix to TraceBlob when displaying beyond end of blob.
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added Mapping List.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1999.01.04    DKS
//    TraceRect / TracePoint / TraceSize now protected from long strings.
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

// #define DEBUG_ALL

///////////////////////////////////////////////////////////////////////////////////////
//
//   bSubcontrol - TRUE  - add controls to existing pZSubtask->m_pZMIXCtrl
//                 FALSE - delete existing pZSubtask->m_pZMIXCtrl and start a new list
//
///////////////////////////////////////////////////////////////////////////////////////
void OPERATION
CreateZeidonCtrls( zVIEW    vSubobject,
                   ZSubtask *pZSubtask,
                   CWnd     *pWndParent,
                   ZMapAct  *pzmaComposite,
                   zSHORT   nOffsetX,
                   zSHORT   nOffsetY,
                   zBOOL    bSubcontrol )
{
   zKZWDLGXO_Ctrl_DEF CtrlDef;
   zFARPROC_CONTROL lpfnCtrlProc;
   LPLIBRARY hCCtlLibrary;
   zPCHAR    pchDLL_Name;
   zPCHAR    pchCOP;
   zSHORT    nRC;

#ifdef DEBUG_ALL
   TraceLineX( "CreateZeidonCtrls parent - ", (zLONG) pZSubtask );
#endif

   nRC = CheckExistenceOfEntity( vSubobject, szlCtrl );
   if ( bSubcontrol == FALSE )
   {
      mDeleteInit( pZSubtask->m_pZMIXCtrl );
      if ( nRC > zCURSOR_UNCHANGED )
      {
         zLONG  lTempCtrlCnt;       // just to get the long temporarily

         GetIntegerFromAttribute( &lTempCtrlCnt, pZSubtask->m_vDialog, szlWnd, "CtrlCnt" );
//       TraceLineI( "CreateZeidonCtrls count ", lTempCtrlCnt );
         pZSubtask->m_pZMIXCtrl = new ZMultiIdxSet( (fnCOMPARE) fnCompareCtrlNodes, (fnCOMPARE) fnSeekCtrlNode, lTempCtrlCnt );
         // Instead of having ZMultiIdxSet delete the nodes itself, we call a recursive routine (fnDeleteCtrls) to
         // guarantee the child controls are deleted before their parents.
      }
      else
         pZSubtask->m_pZMIXCtrl = 0;

      CreateViewFromViewForTask( &(pZSubtask->m_vPE), pZSubtask->m_pZTask->m_vPE, pZSubtask->m_vDialog );
   }

   CtrlDef.CheckByte = 0xFE;
   CtrlDef.lStructSize = sizeof( zKZWDLGXO_Ctrl_DEF );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Get the structure of attributes for this control.
      zPCHAR pch = 0;   // for derived Text attribute
      GetStructFromEntityAttrs( (zPCHAR) &CtrlDef, vSubobject, szlCtrl );
      if ( CtrlDef.CheckByte != 0xFE )
         MessageBox( 0, "CtrlDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

      GetAddrForAttribute( (zPCHAR *) &pch, vSubobject, szlCtrl, szlD_Text );
      if ( *pch )
         strcpy_s( CtrlDef.Text, zsizeof( CtrlDef.Text ), pch );

#ifdef DEBUG_ALL
      TraceLineS( "Tag ", CtrlDef.Tag );
      TraceLineI( "Type ", CtrlDef.Type );
      TraceLineI( "IdNbr ", CtrlDef.IdNbr );
      TraceLineI( "PSDLG_X ", CtrlDef.PSDLG_X );
      TraceLineI( "PSDLG_Y ", CtrlDef.PSDLG_Y );
      TraceLineI( "SZDLG_X ", CtrlDef.SZDLG_X );
      TraceLineI( "SZDLG_Y ", CtrlDef.SZDLG_Y );
#endif

      // The token is validated by the painter and/or conversion to the executable object, but we check here anyway.
      nRC = SetCursorFirstEntityByInteger( pZSubtask->m_pZTask->m_vPE, szlCtrl, szlKey, CtrlDef.Type, 0 );
      if ( nRC != zCURSOR_SET )
      {
         TraceLine( "(drvr) ID (%d) not found (%d) in PPE for Ctrl: %s.%s.%s", CtrlDef.Type, nRC,
                    pZSubtask->m_pzsDlgTag ? *(pZSubtask->m_pzsDlgTag) : "?",
                    pZSubtask->m_pzsWndTag ? *(pZSubtask->m_pzsWndTag) : "?", CtrlDef.Tag );
         DisplayEntityInstance( pZSubtask->m_pZTask->m_vPE, szlCtrl );
         DisplayObjectInstance( pZSubtask->m_pZTask->m_vPE, 0, 0 );

         // Skip this one and try the next.
         nRC = SetCursorNextEntity( vSubobject, szlCtrl, 0 );
         continue;
      }

      GetAddrForAttribute( (zPCHAR *) &pchDLL_Name, pZSubtask->m_pZTask->m_vPE, szlCtrl, szlDLL );
      GetAddrForAttribute( (zPCHAR *) &pchCOP, pZSubtask->m_pZTask->m_vPE, szlCtrl, szlCOP );

      // If the DLL name is added to the list, this is the first time it has been encountered ... so call its initialization routine.
      pZSubtask->m_pZTask->AddApplicationDLL( pZSubtask, pchDLL_Name );

      hCCtlLibrary = 0;  // set to zero for GetOperationDynamicCallAddress

      lpfnCtrlProc = (zFARPROC_CONTROL) GetOperationDynamicCallAddress( pZSubtask->m_vDialog, (LPLPLIBRARY) &hCCtlLibrary,
                                                                        pchDLL_Name, pchCOP, "(drvr) Ctrl" );

      try
      {
         if ( lpfnCtrlProc ) // && (k % 2) == 0 )
         {
         // TraceLineS( "Create Control DLL: ", pchDLL_Name );
         // TraceLineS( "Create Control Operation: ", pchCOP );
            (*lpfnCtrlProc)( pZSubtask, pWndParent, pzmaComposite, vSubobject, nOffsetX, nOffsetY, &CtrlDef );
         // TraceLineS( "Create Control ", "Succeeded" );
         }
      }
#if 0
      catch( CResourceException *e )
      {
         zCHAR szMsg[ 256 ];
         e->GetErrorMessage( szMsg, zsizeof( szMsg ) );
         strcat_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ),
      }
#endif
      catch( CException *e )
      {
         zCHAR szMsg[ 256 ];

         e->GetErrorMessage( szMsg, zsizeof( szMsg ) );
         MessageSend( pZSubtask->m_vDialog, 0, "Zeidon Control", szMsg, zMSGQ_MODAL_ERROR, FALSE );
         break;
      }

      nRC = SetCursorNextEntity( vSubobject, szlCtrl, 0 );

   }  // end of: while ( nRC == zCURSOR_UNCHANGED )  looping thru controls

   if ( bSubcontrol == FALSE )
   {
      DropView( pZSubtask->m_vPE );
      pZSubtask->m_vPE = 0;  // using this to determine Ctrls have been created

      // Ctrl creation is complete ... notify each ctrl.
      if ( pZSubtask->m_pZMIXCtrl )
      {
         pZSubtask->m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnPostCreateCtrls, 0, 0, 0 );
      }
   }
}

// Function to find the first control which is to gain focus.
void
fnSetFirstLastFocus( ZMapAct *pzma,
                     WPARAM  wParam,
                     LPARAM  lParam )
{
   if ( (pzma->m_ulMapActFlags & zMAPACT_VISIBLE) && (pzma->m_ulMapActFlags & zMAPACT_ENABLED) &&
        (pzma->m_ulMapActFlags & zMAPACT_FOCUS_ENABLED) )
   {
      pzma->m_pZSubtask->m_pzmaLastFocus = pzma;
      if ( pzma->m_pZSubtask->m_pzmaFirstFocus == 0 )
      {
         pzma->m_pZSubtask->m_pzmaFirstFocus = pzma;
         pzma->m_pZSubtask->m_pzmaWithFocus = pzma;
      }
   }
}

// Comparison function required to order objects added to the list.  In this case it just compares the names
// embedded in the ZMapAct type.
zSHORT
fnCompareCtrlNodes( const ZMapAct *pzma1,
                    const ZMapAct *pzma2 )
{
   return( pzma1->m_pzsTag->Compare( *(pzma2->m_pzsTag) ) );
}

// Comparison function required to seek for an object in the list.  In this case it just compares the names
// embedded in the ZMapAct type.
zSHORT
fnSeekCtrlNode( const CString *pzsTag,
                const ZMapAct *pzma )
{
   return( pzsTag->Compare( *(pzma->m_pzsTag) ) );
}

///////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: GetSafeCtrlNode
//
// PURPOSE: This routine finds a control ensuring that the requested  control ID is valid.
//          First ... first use the incoming id as an index into the ZBinCtrl collection
//          array ... if that doesn't yield the correct control, sequentially search
//          through the collection array until found.  If not found, return 0.
//
///////////////////////////////////////////////////////////////////////////////////////
ZMapAct *
GetSafeCtrlNode( ZMultiIdxSet *pZMIXCtrl,
                 zUSHORT      uID )
{
   if ( pZMIXCtrl )
   {
      ZMapAct *pzma = (ZMapAct *) pZMIXCtrl->GetAt( uID );
      if ( pzma && pzma->m_nIdNbr == uID )
         return( pzma );
      else
      {
         zLONG k = 1;
         while ( (pzma = (ZMapAct *) pZMIXCtrl->GetAt( k++ )) != 0 )
         {
            if ( pzma->m_nIdNbr == uID )
               return( pzma );
         }
      }
   }

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT void OPERATION
MakeShortString( CDC *pDC, zPCHAR pchReturn, zLONG lMaxReturnLth,
                 zCPCHAR cpcString, zLONG lColLth, zLONG lOffset )
{
   static const _TCHAR szThreeDots[ ] = _T( "..." );

   if ( lMaxReturnLth == 0 )
   {
      pchReturn[ 0 ] = 0;
      return;
   }

   strncpy_s( pchReturn, lMaxReturnLth, cpcString, lMaxReturnLth - 1 );
   zLONG  lStringLth = zstrlen( cpcString );
   CSize  size;
   zLONG  k;

   zCHAR szFontFace[ 32 ];

   GetTextFace( pDC->m_hDC, zsizeof( szFontFace ), szFontFace );
   GetTextFace( pDC->m_hAttribDC, zsizeof( szFontFace ), szFontFace );
   GetTextExtentPoint32( pDC->m_hDC, cpcString, lStringLth, &size );
   if ( pDC->m_bPrinting )
      GetTextExtentPoint32( pDC->m_hAttribDC, cpcString, lStringLth, &size );

   if ( lStringLth == 0 || (size.cx + lOffset) <= lColLth )
      return;

   GetTextExtentPoint32( pDC->m_hDC, szThreeDots, zsizeof( szThreeDots ), &size );
   zLONG lAddLth = size.cx;

   if ( lStringLth >= lMaxReturnLth - 3 )
      lStringLth = lMaxReturnLth - 3;

   if ( lStringLth > 0 )
   {
      for ( k = lStringLth - 1; k > 0; k-- )
      {
         pchReturn[ k ] = 0;
         GetTextExtentPoint32( pDC->m_hDC, pchReturn, k, &size );
         if ( pDC->m_bPrinting )
            GetTextExtentPoint32( pDC->m_hAttribDC, pchReturn, k, &size );

         if ( (size.cx + lOffset + lAddLth) <= lColLth )
            break;
      }
   }
   else
   {
      pchReturn[ 0 ] = 0;
   }

   strcat_s( pchReturn, lMaxReturnLth, szThreeDots );
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT void OPERATION
TraceRect( zCPCHAR cpcMessage,
           RECT&   rect )
{
   zCHAR  szBuffer[ 256 ];
   zSHORT nLth;

   nLth = (zSHORT) zstrlen( cpcMessage );
   if ( nLth < zsizeof( szBuffer ) - 32 )
      strcpy_s( szBuffer, zsizeof( szBuffer ), cpcMessage );
   else
      nLth = 0;

   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " X:" );
   _ltoa_s( rect.left, szBuffer + nLth + 3, zsizeof( szBuffer ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szBuffer );
   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " Y:" );
   _ltoa_s( rect.top, szBuffer + nLth + 3, zsizeof( szBuffer ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szBuffer );
   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " W:" );
   _ltoa_s( rect.right - rect.left, szBuffer + nLth + 3, zsizeof( szBuffer ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szBuffer );
   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " H:" );
// _ltoa_s( rect.bottom - rect.top, szBuffer + nLth + 3 );
   TraceLineI( szBuffer, rect.bottom - rect.top );
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT void OPERATION
TracePoint( zCPCHAR cpcMessage,
            POINT&  pt )
{
   zCHAR  szBuffer[ 256 ];
   zSHORT nLth;

   nLth = (zSHORT) zstrlen( cpcMessage );
   if ( nLth < zsizeof( szBuffer ) - 32 )
      strcpy_s( szBuffer, zsizeof( szBuffer ), cpcMessage );
   else
      nLth = 0;

   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " X=" );
   _ltoa_s( pt.x, szBuffer + nLth + 3, zsizeof( szBuffer ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szBuffer );
   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " Y=" );
// _ltoa_s( pt.y, szBuffer + nLth + 3 );
   TraceLineI( szBuffer, pt.y );
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT void OPERATION
TraceSize( zCPCHAR cpcMessage,
           SIZE    &size )
{
   zCHAR  szBuffer[ 256 ];
   zSHORT nLth;

   nLth = (zSHORT) zstrlen( cpcMessage );
   if ( nLth < zsizeof( szBuffer ) - 32 )
      strcpy_s( szBuffer, zsizeof( szBuffer ), cpcMessage );
   else
      nLth = 0;

   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " cX=" );
   _ltoa_s( size.cx, szBuffer + nLth + 4, zsizeof( szBuffer ) - nLth - 4, 10 );
   nLth = (zSHORT) zstrlen( szBuffer );
   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " cY=" );
// _ltoa_s( size.cy, szBuffer + nLth + 4 );
   TraceLineI( szBuffer, size.cy );
}