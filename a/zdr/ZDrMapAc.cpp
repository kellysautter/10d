/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrmapac.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of class ZMapAct.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.06.19    DKS     FH
//    Added new virtual method ResetFocus to permit group type controls
//    (such as radio button groups) to set focus to the appropriate
//    sub-control when focus is being reset due to a control being disabled.
//
// 2002.06.11    BL/DKS  R55711
//    Bugfix SetCtrlMapping for a Control without Mapping in PWD.
//
// 2002.04.29    DKS
//    Issue application error for undefined cursor during mapping.
//
// 2002.04.10    DKS
//    Altered Get/SetRowColText in support of unmapped ComboBox.
//
// 2002.03.05    DKS
//    GetMappingAutoGray required a different check to determine existence
//    of an entity.
//
// 2002.02.14    DKS
//    Added Domain translation via code.
//
// 2002.01.04    DKS   Z10
//    Added check for ReadOnly view when determining if an attribute can
//    be written to the OI in IsVisibleForMapToOI method.
//
// 2001.09.24    DKS   F51176
//    Final analysis of Repaint following
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.09.21    DKS   Enhance
//    Added SetCtrlMapping to remap a Control.
//
// 2001.05.04    DKS   Enhance
//    SetZCtrlProperty handles zCONTROL_STATUS_FONT as follows:
//    SetZCtrlProperty( zCONTROL_STATUS_FONT, 85, "Courier New" );
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 2001.01.03    DKS   Z10
//    Permit select of text in EditBox/MLE.
//
// 2000.04.06    TMV   Z10  ActiveX
//    In ~ZActiveX call into user dll to give user a chance to delete all
//    he has allocated. To do this SetWrapperInstance had to be changed.
//
// 1999.12.02    TMV   Z10   TB853
//    fixing crash when working with multiple sources.
//
// 1999.10.17    DKS   Z10    TB767/TB784/TB789
//    MapFromOI in MapChildren is moved so that default Radio Button mapping
//    works properly.
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added Mapping List.
//
// 1999.07.22    DKS   Z10    QS999
//    Permit ctrls to be disabled in a "ReadOnly" mode (as available
//    on a ctrl by ctrl basis).
//
// 1999.05.11    DKS   Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//    Fixed Drag/Drop code for new Header ListBox type.
//
// 1999.03.10    DKS
//    Altered the TranslateColor function to remove name conflict.
//
// 1999.02.19    DKS
//    EventAct search now terminated by count rather than termination flag.
//    An event with id zero is now permitted (for ActiveX events).
//
// 1998.10.29    DKS
//    Error on ctrl within tab now displayed and gets focus properly.
//
// 1998.10.20    DKS
//    Radio buttons on a tab control handle TAB and ARROW keys properly.
//
// 1998.10.14    DKS
//    Text on Tab Ctrl colored in PostBuild, switch no longer loses color
//    (TB 298).
//
// 1998.10.12    DKS
//    Modified Dialog conversion to set the size of the Relative Pos/Size
//    area properly.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
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
// #define DEBUG_RESIZE
// #define zNEW_RESIZE
static zLONG g_lRel = (1L << 24) + (0L << 16) + ('a' << 8) + 1;

/////////////////////////////////////////////////////////////////////////////
// ZMapAct Implementation
//

zLONG ZMapAct::g_lIdNbr = 1;
// ZMapAct - ctor for mapping/action object
ZMapAct::ZMapAct( ZSubtask *pZSubtask,
                  ZMapAct  *pzmaComposite,
                  CWnd     *pWndParent,
                  CWnd     *pCtrl,
                  zVIEW    vDialog,
                  zSHORT   nOffsetX,
                  zSHORT   nOffsetY,
                  zKZWDLGXO_Ctrl_DEF *pCtrlDef,
                  zCPCHAR  cpcType ) :
         m_csERemote( cpcType )
{
   zULONG  ulLth;

#ifdef DEBUG_ALL
   TraceLineI( "In ctor for ZMapAct this = ", (zLONG) this );
   TraceLineI( "In ctor for ZMapAct CWnd* = ", (zLONG) this );
   TraceLineI( "parent", (zLONG) pZSubtask );
#endif

   m_ulMapActIdNbr = g_lIdNbr++;
   m_lKeyId = pCtrlDef->Type;    // the IdNbr ("Key") of this control
   m_pZSubtask = pZSubtask;               // Zeidon primary window
   m_pCtrl = pCtrl;
   m_pzmaComposite = pzmaComposite;
   m_pzsTag = new CString( pCtrlDef->Tag );
   m_pzsText = new CString( pCtrlDef->Text );
   m_nIdNbr = (zSHORT) pCtrlDef->IdNbr;
   m_pWndParent = pWndParent;
   m_pWndLastFocus = 0;
   m_pzsVName = m_pzsEName = m_pzsAName = m_pzsContext = 0;

   m_hWndChild = 0;
   m_chQuickKey = 0;
   m_pDispatch = 0;
   m_pWrapperInstance = 0;
   m_hWndFullClient = 0;

#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "Active" ) == 0 )
   {
      TraceLineS( "Control tag ", *m_pzsTag );
      TraceLineI( "       type ", m_lKeyId );
   }
#endif

   m_ulMapActFlags = zMAPACT_INITIALIZE;
   m_ulMapActFlag2 = zMAPACT_INITIALIZE;
   m_ptMouseDown.x = 0;
   m_ptMouseDown.y = 0;
   m_ptMouseUp.x = 0;
   m_ptMouseUp.y = 0;
   m_pTranslationList = 0;

   if ( pCtrlDef->Subtype & zCONTROL_DISABLED )
   {
      m_ulMapActFlags &= ~zMAPACT_ENABLED;
   }
   else
   {
      m_ulMapActFlags |= zMAPACT_ENABLED;
   }

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
   {
      m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
   }
   else
   {
      m_ulMapActFlags |= zMAPACT_FOCUS_ENABLED;
   }

   if ( pCtrlDef->Subtype & zCONTROL_INVISIBLE )
   {
      Attr.Style = WS_CHILD;
      m_ulMapActFlags &= ~zMAPACT_VISIBLE;
   }
   else
   {
      Attr.Style = WS_VISIBLE | WS_CHILD;
      m_ulMapActFlags |= zMAPACT_VISIBLE;
   }

   if ( pCtrlDef->StyleX & zCONTROLX_DISABLE_READONLY )
      m_ulMapActFlags |= zMAPACT_DISABLE_READONLY;
   else
      m_ulMapActFlags &= ~zMAPACT_DISABLE_READONLY;

   if ( pCtrlDef->StyleX & zCONTROLX_ABSOLUTE_PIXEL_SIZE )
      m_ulMapActFlags |= zMAPACT_ABSOLUTE_PIXEL_SIZE;
   else
      m_ulMapActFlags &= ~zMAPACT_ABSOLUTE_PIXEL_SIZE;

   if ( pCtrlDef->StyleX & zCONTROLX_ABSOLUTE_PIXEL_POS )
      m_ulMapActFlags |= zMAPACT_ABSOLUTE_PIXEL_POS;
   else
      m_ulMapActFlags &= ~zMAPACT_ABSOLUTE_PIXEL_POS;

   if ( pCtrlDef->StyleX & zCONTROLX_PREFILL )
      m_ulMapActFlag2 |= zMAPACT_PREFILL;
   else
      m_ulMapActFlag2 &= ~zMAPACT_PREFILL;

   // Add this control to driver's list.
   // m_lIdx = pZSubtask->m_pZMIXCtrl->InsertNode( this );
   if ( (m_lIdx = pZSubtask->m_pZMIXCtrl->InsertNode( this )) == 0 )
   {
      // Should not happen ... protected by painter?
      TraceLineS( "(drvr) Control Error (DUPLICATE Ctrl tag) ===> ", pCtrlDef->Tag );
      OperatorSend( 0, szlDrvr, "DUPLICATE Ctrl tag!!!", TRUE );
      pZSubtask->m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnOrderedCtrls );

      zCHAR  szTag[ zTAG_LTH ];
      zCHAR  ch = '0';
      zSHORT k;

      strcpy_s( szTag, sizeof( szTag ), pCtrlDef->Tag );
      k = (zSHORT) zstrlen( szTag );
      if ( k >= 32 )
         k = 31;

      szTag[ k + 1 ] = 0;

      do
      {
         szTag[ k ] = ch++;
         delete( m_pzsTag );
         m_pzsTag = new CString( szTag );

      } while ( (m_lIdx = pZSubtask->m_pZMIXCtrl->InsertNode( this )) == 0 && ch != '0' ); // don't wrap forever.

      if ( m_lIdx == 0 )
      {
         TraceLineS( "(drvr) Could not generate unique Ctrl tag ===> ", pCtrlDef->Tag );
         m_lIdx = 0;
         return;
      }
      else
      {
         zCHAR szMessage[ 256 ];

         strcpy_s( szMessage, sizeof( szMessage ), "(drvr[" );
         _ltoa_s( (zLONG) pZSubtask, szMessage + 6, sizeof( szMessage ) - 6, 10 );
         if ( pZSubtask )
         {
            if ( pZSubtask->m_pzsDlgTag )
            {
               strcat_s( szMessage, sizeof( szMessage ), "." );
               strcat_s( szMessage, sizeof( szMessage ), *(pZSubtask->m_pzsDlgTag) );
            }

            if ( pZSubtask->m_pzsWndTag )
            {
               strcat_s( szMessage, sizeof( szMessage ), "." );
               strcat_s( szMessage, sizeof( szMessage ), *(pZSubtask->m_pzsWndTag) );
            }
         }

         strcat_s( szMessage, sizeof( szMessage ), "] Generated unique Ctrl tag ===> " );
         TraceLineS( szMessage, *m_pzsTag );
      }
   }
   else
   {
#ifdef DEBUG_ALL
      zCHAR szBuffer[ 64 ];
      strcpy_s( szBuffer, sizeof( szBuffer ), "ZMapAct ctor for " );
      strcat_s( szBuffer, sizeof( szBuffer ), *m_pzsTag );
      strcat_s( szBuffer, sizeof( szBuffer ), ": " );
      TraceLineI( szBuffer, m_lIdx );
#endif
   }

   // Drag and Drop information is carried in a blob with the format of:
   //
   //    4-byte flag - S (source), T (target), B (both) ... only 1 byte used
   //    sizeof( zLONG ) - length of first key (source or target)
   //    first key string
   //    null terminator
   //    second key string (if both source and target)
   //    second null terminator
   //
   GetAttributeLength( &ulLth, vDialog, szlCtrl, szlDD_BOI );
   if ( ulLth )
   {
      m_pchDragDrop = new char[ ulLth ];
      GetBlobFromAttribute( m_pchDragDrop, &ulLth, vDialog, szlCtrl, szlDD_BOI );
      if ( *m_pchDragDrop == 'B' || *m_pchDragDrop == 'S' )
         m_ulMapActFlags |= zMAPACT_DRAG_ENABLED;
   }
   else
   {
      m_pchDragDrop = 0;
   }

   // Set the relative offset of the DIL message for this control.
   m_nDIL = (zSHORT) pCtrlDef->DIL;
   if ( CheckExistenceOfEntity( vDialog, szlCtrlMap ) > zCURSOR_UNCHANGED )
   {
      zKZWDLGXO_CtrlMap_DEF CtrlMapDef;

      CtrlMapDef.CheckByte = 0xFE;
      CtrlMapDef.lStructSize = sizeof( zKZWDLGXO_CtrlMap_DEF );
      GetStructFromEntityAttrs( (zPCHAR) &CtrlMapDef, vDialog, szlCtrlMap );
      if ( CtrlMapDef.CheckByte != 0xFE )
         MessageBox( 0, "CtrlMapDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

#ifdef DEBUG_ALL
      TraceLineS( "ViewName ", CtrlMapDef.VN );
      TraceLineS( "Entity ", CtrlMapDef.EN );
      TraceLineS( "Attribute ", CtrlMapDef.AN );
      TraceLineS( "Context ", CtrlMapDef.Context );
      if ( CtrlMapDef.VN[ 0 ] )
      {
         if ( CtrlMapDef.EN[ 0 ] == 0 )
            TraceLineS( "View name with no entity!!!", "" );

         if ( CtrlMapDef.AN[ 0 ] == 0 )
            TraceLineS( "View Name with no attribute!!!", "" );
      }
#endif

      // If there is a view name, then fill in the rest of the attributes,
      // otherwise let them stay null.
      if ( CtrlMapDef.VN[ 0 ] )
      {
         m_pzsVName = new CString( CtrlMapDef.VN );
         m_pzsEName = new CString( CtrlMapDef.EN );
         m_pzsAName = new CString( CtrlMapDef.AN );
         m_pzsContext = new CString( CtrlMapDef.Context );
      }
   }

   // Get all the events for this control.
   GetAttributeLength( &ulLth, vDialog, szlCtrl, szlEventAct );
   if ( ulLth )
   {
      m_pchEventAction = (zPCHAR) pCtrlDef->EventAct;
      m_nEventActionCnt = (zSHORT) (ulLth / sizeof( zLONG ));
      if ( m_nEventActionCnt % 2 )
      {
         // Conversion added an extra long as a termination flag.
         m_nEventActionCnt--;
      }

      m_nEventActionCnt /= 2;  // two long values define each EventAction
   }
   else
   {
      m_pchEventAction = 0;
      m_nEventActionCnt = 0;
   }

   if ( m_ulMapActFlags & zMAPACT_ABSOLUTE_PIXEL_POS )
   {
      pCtrlDef->PSDLG_X += nOffsetX;
      Attr.X = pCtrlDef->PSDLG_X;
      pCtrlDef->PSDLG_Y += nOffsetY;
      Attr.Y = pCtrlDef->PSDLG_Y;
   }
   else
   {
      if ( m_pZSubtask->m_bUseMapDlgUnits )
      {
         Attr.X = pCtrlDef->PSDLG_X = mConvertMapDlgToPixelX( pCtrlDef->PSDLG_X ) + nOffsetX;
         Attr.Y = pCtrlDef->PSDLG_Y = mConvertMapDlgToPixelY( pCtrlDef->PSDLG_Y ) + nOffsetY;
      }
      else
      {
         Attr.X = pCtrlDef->PSDLG_X = mConvertDlgUnitToPixelX( pCtrlDef->PSDLG_X ) + nOffsetX;
         Attr.Y = pCtrlDef->PSDLG_Y = mConvertDlgUnitToPixelY( pCtrlDef->PSDLG_Y ) + nOffsetY;
      }
   }

   if ( m_ulMapActFlags & zMAPACT_ABSOLUTE_PIXEL_SIZE )
   {
      Attr.W = pCtrlDef->SZDLG_X;
      Attr.H = pCtrlDef->SZDLG_Y;
   }
   else
   {
      if ( m_pZSubtask->m_bUseMapDlgUnits )
      {
         Attr.W = pCtrlDef->SZDLG_X = mConvertMapDlgToPixelX( pCtrlDef->SZDLG_X );
         Attr.H = pCtrlDef->SZDLG_Y = mConvertMapDlgToPixelY( pCtrlDef->SZDLG_Y );
      }
      else
      {
         Attr.W = pCtrlDef->SZDLG_X = mConvertDlgUnitToPixelX( pCtrlDef->SZDLG_X );
         Attr.H = pCtrlDef->SZDLG_Y = mConvertDlgUnitToPixelY( pCtrlDef->SZDLG_Y );
      }
   }

#ifdef DEBUG_RESIZE
   zCHAR szTag[ zTAG_LTH ];
   strcpy_s( szTag, sizeof( szTag ), *m_pzsTag );
   if ( (szTag[ 0 ] == 'L' && szTag[ 1 ] == 'i' &&
         szTag[ 2 ] == 's' && szTag[ 3 ] == 't' &&
         szTag[ 4 ] == 'B' && szTag[ 5 ] == 'o' &&
         szTag[ 6 ] == 'x' && szTag[ 7 ] == 'S' &&
         szTag[ 8 ] == 'i' && szTag[ 9 ] == 'z' &&
         szTag[ 10 ] == 'e' && szTag[ 11 ] == 0) )
   {
      TraceLineS( "ZMapAct::ctor ", *m_pzsTag );
      TraceLineI( "Attr.X: ", Attr.X );
      TraceLineI( "Attr.Y: ", Attr.Y );
      TraceLineI( "Attr.W: ", Attr.W );
      TraceLineI( "Attr.H: ", Attr.H );
   }
#endif

   if ( Attr.X + Attr.W > pZSubtask->m_lExtentX )
      pZSubtask->m_lExtentX = Attr.X + Attr.W;

   if ( Attr.Y + Attr.H > pZSubtask->m_lExtentY )
      pZSubtask->m_lExtentY = Attr.Y + Attr.H;

   if ( pCtrlDef->Subtype & zCONTROL_FULL_CLIENT )
   {
      m_ulMapActFlags |= zMAPACT_FULL_CLIENT;
   }

   m_nRelSizeCntX = zLOUSHORT( pCtrlDef->RELCNT_X );
   m_nRelSizeTotalX = zHIUSHORT( pCtrlDef->RELCNT_X );
   m_nRelSizeCntY = zLOUSHORT( pCtrlDef->RELCNT_Y );
   m_nRelSizeTotalY = zHIUSHORT( pCtrlDef->RELCNT_Y );

   m_lRelPsX = m_lRelSzX = m_lRelPsY = m_lRelSzY = 0;
// if ( zstrcmp( *m_pzsTag, "AddMapEntity" ) == 0 )
//    TraceLineS( "ZMapAct::ctor tag: ", *m_pzsTag );

   if ( pCtrlDef->Subtype & zCONTROL_RELATIVE_POS_X )
   {
      pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_SCROLLBARS;
      m_lRelPsX = Attr.X - pZSubtask->m_lRelLeft;
      if ( m_nRelSizeCntX == 0 )
         m_nRelSizeCntX = 1;

      if ( m_nRelSizeTotalX == 0 )
         m_nRelSizeTotalX = m_nRelSizeCntX;

      if ( pCtrlDef->Subtype & zCONTROL_RELATIVE_SIZE_X )
         m_lRelSzX = Attr.W;
   }
   else
   if ( pCtrlDef->Subtype & zCONTROL_RELATIVE_SIZE_X )
   {
      pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_SCROLLBARS;
      m_lRelSzX = Attr.W;
   }

   if ( pCtrlDef->Subtype & zCONTROL_RELATIVE_POS_Y )
   {
      pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_SCROLLBARS;
      m_lRelPsY = Attr.Y - pZSubtask->m_lRelTop;
      if ( m_nRelSizeCntY == 0 )
         m_nRelSizeCntY = 1;

      if ( m_nRelSizeTotalY == 0 )
         m_nRelSizeTotalY = m_nRelSizeCntY;

      if ( pCtrlDef->Subtype & zCONTROL_RELATIVE_SIZE_Y )
         m_lRelSzY = Attr.H;
   }
   else
   if ( pCtrlDef->Subtype & zCONTROL_RELATIVE_SIZE_Y )
   {
      pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_SCROLLBARS;
      m_lRelSzY = Attr.H;
   }

//#ifdef zNEW_RESIZE
   if ( pZSubtask->m_lRelease > g_lRel )
   {
      if ( m_lRelSzX && m_lRelSzX + Attr.X > pZSubtask->m_lOrigRelSizeX )
         m_lRelSzX = m_pZSubtask->m_lOrigRelSizeX - Attr.X;

      if ( m_lRelSzY && m_lRelSzY + Attr.Y > pZSubtask->m_lOrigRelSizeY )
         m_lRelSzY = m_pZSubtask->m_lOrigRelSizeY - Attr.Y;
   }
//#endif

   if ( m_nRelSizeTotalX > 0 || m_nRelSizeTotalY > 0 )
      m_pZSubtask->m_ulSubtaskFlags |= zSUBTASK_SINGLE_RELSIZE;
   else
   if ( m_lRelPsX == 0 && m_lRelPsY == 0 && m_lRelSzX == 0 && m_lRelSzY == 0 )
      m_ulMapActFlags |= zMAPACT_IGNORE_RESIZE;
   else
      m_nRelSizeCntX = m_nRelSizeCntY = m_nRelSizeTotalX = m_nRelSizeTotalY = 1;

#ifdef DEBUG_RESIZE
   if ( m_lRelPsX || m_lRelPsY || m_lRelSzX || m_lRelSzY )
   {
      if ( (szTag[ 0 ] == 'L' && szTag[ 1 ] == 'i' &&
            szTag[ 2 ] == 's' && szTag[ 3 ] == 't' &&
            szTag[ 4 ] == 'B' && szTag[ 5 ] == 'o' &&
            szTag[ 6 ] == 'x' && szTag[ 7 ] == 'S' &&
            szTag[ 8 ] == 'i' && szTag[ 9 ] == 'z' &&
            szTag[ 10 ] == 'e' && szTag[ 11 ] == 0) )
      {
         TraceLineS( "Ctrl tag ===> ", *m_pzsTag );
         TraceLineI( "m_lRelPsX = ", m_lRelPsX );
         TraceLineI( "m_lRelPsY = ", m_lRelPsY );
         TraceLineI( "m_lRelSzX = ", m_lRelSzX );
         TraceLineI( "m_lRelSzY = ", m_lRelSzY );

         TraceLineI( "X coord =", Attr.X );
         TraceLineI( "Y coord =", Attr.Y );
         TraceLineI( "X size  =", Attr.W );
         TraceLineI( "Y size  =", Attr.H );
      }
   }
#endif

   m_ulCtrlErr = FALSE;       // control error flag
   m_clrText = fnTranslateColor( pCtrlDef->ColorFg );
   m_clrBk = fnTranslateColor( pCtrlDef->ColorBk );
   if ( m_clrBk )
   {
      m_pBrush = new CBrush( m_clrBk );
   }
   else
   if ( m_pZSubtask->m_clrBk )
   {
      m_clrBk = m_pZSubtask->m_clrBk;
      m_pBrush = new CBrush( m_clrBk );
      m_ulMapActFlags |= zMAPACT_USE_SUBTASK_BRUSH;
   }
   else
   {
      m_pBrush = 0;
   }

   m_pFont = 0;

#if 0
   if ( CheckExistenceOfEntity( vDialog, szlCtrlFont ) > zCURSOR_UNCHANGED )
   {
      GetAttributeLength( &ulLth, vDialog, szlCtrlFont, "LFont" );
      if ( ulLth <= sizeof( LOGFONT ) )
      {
         LOGFONT  lf;
         zLONG lColor;

         GetBlobFromAttribute( &lf, &ulLth, vDialog, szlCtrlFont, "LFont" );
         GetIntegerFromAttribute( &lColor, vDialog, szlCtrlFont, "RGB" );

         m_pFont = new CFont( );
         m_pFont->CreateFontIndirect( &lf );
         if ( m_clrText == 0 )
         {
            m_clrText = fnTranslateColor( lColor );
         }
      }
   }
#endif

   if ( m_pZSubtask->m_vPE )
   {
      zLONG lEvent;

      // If a CtrlEvent Entity exists we have to register it for driver default handling.
      zSHORT nRC = SetCursorFirstEntity( m_pZSubtask->m_vPE , "CtrlEvent", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lEvent, m_pZSubtask->m_vPE , "CtrlEvent", "Type" );
         RegisterEvent( lEvent );
         nRC = SetCursorNextEntity( m_pZSubtask->m_vPE , "CtrlEvent", 0 );
      }
   }
}

zVIEW
ZMapAct::SetMapViewFromView( zVIEW v,
                             zCPCHAR cpcEName,
                             BOOL bSrc )
{
   zVIEW  vSrc;
   zVIEW  vTgt;

   if ( m_pzsVName )
   {
      GetViewByName( &vTgt, *m_pzsVName, v, zLEVEL_TASK );
      if ( vTgt )
      {
         if ( bSrc )
         {
            vSrc = v;
            v = vTgt;
         }
         else
         {
            vSrc = vTgt;
            vTgt = v;
            v = vSrc;
         }

         zULONG ulKey = GetEntityKey( vSrc, cpcEName );
         if ( SetEntityCursor( vTgt, cpcEName, 0, zQUAL_ENTITYKEY | zPOS_FIRST, (zCPVOID) &ulKey, 0, 0, 0, 0, 0 ) != 0 )
         {
            zCHAR szMsg[ 256 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Entity Key Error: Control: %s  Entity: %s", (*m_pzsTag).GetString(), cpcEName );
            TraceLineS( "ZMapAct::SetMapViewFromView ", szMsg );
         }

      // SetViewFromView( vTgt, vSrc );
      }
   }

   return( v );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:   GetMappingInfo
//
// PARAMETERS:  pvApp
//              cpcVName
//              cpcEName
//              cpcAName
//
// RETURNS:     uMapState - return codes in bitwise value
//
//                          zMAPACT_ENTITY_EXISTS
//                          zMAPACT_ENTITY_UPDATEABLE
//
// Note that the pvApp will be zero if the view name is not found.
//
/////////////////////////////////////////////////////////////////////////////
zUSHORT
ZMapAct::GetMappingAutoGray( zVIEW   *pvApp,
                             zCPCHAR cpcVName,
                             zCPCHAR cpcEName,
                             zCPCHAR cpcAName )
{
   zUSHORT uMapState = 0;

   if ( GetViewByName( pvApp, cpcVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      if ( MiGetUpdateForViewEntityAttr( *pvApp, cpcEName, cpcAName, 0 ) > 0 )
      {
         uMapState |= zMAPACT_ENTITY_UPDATEABLE;
      }

      zSHORT nRC = CheckExistenceOfEntity( *pvApp, cpcEName );
      if ( nRC >= zCURSOR_UNDEFINED )
      {
         if ( nRC == zCURSOR_UNDEFINED )
         {
            zCHAR szMsg[ 512 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Resetting (undefined) application cursor for View.Entity: %s.%s\nApplication must position cursor appropriately.",
                      cpcVName, cpcEName );
            MessageSend( m_pZSubtask->m_vDialog, 0,
                         "Zeidon Application", szMsg,
                         zMSGQ_MODAL_ERROR, FALSE );
            SetCursorPrevEntity( *pvApp, cpcEName, 0 );
         }

         uMapState |= zMAPACT_ENTITY_EXISTS;
      }
   }
   else
   {
      *pvApp = 0;
      if ( cpcVName && *cpcVName )
      {
         if ( zstrcmp( m_pZSubtask->m_csVName.GetString(), cpcVName ) )
         {
            TraceLineS( "View name not found for AutoGray: ", cpcVName );
            m_pZSubtask->m_csVName = cpcVName;
         }
      }
   }

   // Auto Graying, if the view does not permit update for the mapping entity or the entity instance does not exist, disable control
   if ( (uMapState & zMAPACT_ENTITY_EXISTS) && (uMapState & zMAPACT_ENTITY_UPDATEABLE) )
   {
      SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, TRUE );
   }
   else
   {
      SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, FALSE );
   }

   return( uMapState );
}

zBOOL
ZMapAct::IsVisibleForMapFromOI( )
{
   // zMAPACT_VISIBLE             0x00000010
   // zMAPACT_CREATED             0x00000000 // Flag2
   // zMAPACT_NOAUTOMAP_FROM_OI   0x00020000
   // Determine if the control is Visible for mapping.
   if ( (m_ulMapActFlags & zMAPACT_VISIBLE) == 0 || (m_ulMapActFlag2 & zMAPACT_CREATED) == 0 || (m_ulMapActFlags & zMAPACT_NOAUTOMAP_FROM_OI) )
   {
      return( FALSE );
   }

   ZMapAct *pzmaParent = m_pzmaComposite;
   while ( pzmaParent )
   {
      if ( (pzmaParent->m_ulMapActFlags & zMAPACT_VISIBLE) == 0 )
         return( FALSE );

      pzmaParent = pzmaParent->m_pzmaComposite;
   }

   return( TRUE );
}

// Default processing for a ctrl's mapping from the data OI ... if the View and Entity and Attribute have been specified, SetText using
// the value returned by GetStringFromAttribute returning 0, otherwise return -1.
zSHORT
ZMapAct::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMapAct::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zCHAR  szMap[ 256 ];
      zVIEW  vApp;
      zPCHAR pch;

      m_ulMapActFlag2 |= zMAPACT_MAPPING_FROM_OI;
      zUSHORT uMapState = GetMappingAutoGray( &vApp, *m_pzsVName, *m_pzsEName, *m_pzsAName );
   // TraceLineS( "ZMapAct::MapFromOI ", *m_pzsTag );
   // TraceLineX( "ZMapAct::MapFromOI vApp: ", (zLONG) vApp );
      if ( vApp && (uMapState & zMAPACT_ENTITY_EXISTS) )
      {
         zULONG ulLth;

         GetAttributeDisplayLength( &ulLth, vApp, *m_pzsEName, *m_pzsAName, *m_pzsContext );
         if ( ulLth > sizeof( szMap ) - 1 )
         {
            GetAddrForAttribute( (zPCHAR *) &pch, vApp, *m_pzsEName, *m_pzsAName );
         }
         else
         {
            zSHORT nRC = GetVariableFromAttribute( szMap, 0, zTYPE_STRING, sizeof( szMap ) - 1, vApp,
                                                   *m_pzsEName, *m_pzsAName, *m_pzsContext,
                                                   m_pzsContext->IsEmpty( ) ? zUSE_DEFAULT_CONTEXT : 0 );
            if ( nRC == zVAR_NULL )
               pch = 0;
            else
               pch = szMap;
         }
      }
      else
      {
         pch = 0;
      }

      if ( pch == 0 || pch[ 0 ] == 0 )
      {
         szMap[ 0 ] = 0;
         pch = szMap;
      }

      TranslateValue( pch, sizeof( szMap ), TRUE, 1 );
      if ( pch[ 0 ] == 0 && (m_ulMapActFlag2 & zMAPACT_PREFILL) )
         strcpy_s( pch, sizeof( szMap ), *m_pzsText );

      SetZCtrlText( this, pch );
      m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
      return( 0 );
   }

   return( -1 );
}

zSHORT
ZMapAct::PostCreateCtrls( WPARAM wFlag )
{
   return( 0 );
}

zSHORT
ZMapAct::PostMapFromOI( WPARAM wFlag )
{
   return( 0 );
}

zVIEW
ZMapAct::IsVisibleForMapToOI( zCPCHAR cpcViewName )
{
   // zMAPACT_ENABLED             0x00000004
   // zMAPACT_VISIBLE             0x00000010
   // zMAPACT_MAPPED_FROM_OI      0x00000000  // Flag2
   // zMAPACT_CREATED             0x00000000  // Flag2
   // zMAPACT_NOAUTOMAP_TO_OI     0x00040000
   // Determine if the control is Visible for mapping.
   if ( m_pCtrl == 0 || mIs_hWnd( m_pCtrl->m_hWnd ) == 0 ||
        (m_ulMapActFlags & zMAPACT_ENABLED) == 0 ||
        (m_ulMapActFlags & zMAPACT_VISIBLE) == 0 ||
        (m_ulMapActFlag2 & zMAPACT_CREATED) == 0 ||
        (m_ulMapActFlag2 & zMAPACT_MAPPED_FROM_OI) == 0 ||
        (m_ulMapActFlags & zMAPACT_NOAUTOMAP_TO_OI) )
   {
      return( 0 ); // not quite ready for the big time
   }

   zVIEW vApp = (zVIEW) -1; // don't return 0 if view name not specified
   if ( cpcViewName && *cpcViewName )
   {
      if ( GetViewByName( &vApp, cpcViewName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 ||
           MiGetUpdateForView( vApp ) <= 0 )
      {
         return( 0 ); // either the view was not found or it is ReadOnly
      }
   }

   ZMapAct *pzmaParent = m_pzmaComposite;
   while ( pzmaParent )
   {
      if ( (pzmaParent->m_ulMapActFlags & zMAPACT_ENABLED) == 0 || (pzmaParent->m_ulMapActFlags & zMAPACT_VISIBLE) == 0 )
      {
         return( 0 );
      }

      pzmaParent = pzmaParent->m_pzmaComposite;
   }

   return( vApp );
}

// If zVALIDATE is specified as the flag, MapToOI will validate the data
// but will not map to the object instance.  A return code of 0 for MapToOI
// indicates successful mapping; otherwise, mapping is NOT successful.
zSHORT
ZMapAct::MapToOI( zLONG lFlag )
{
   return( 0 );
}

// The composite (parent) should return FALSE if it does not want the
// specified child ctrl to receive focus.
zBOOL
ZMapAct::IsFocusOKForChild( ZMapAct *pzmaReceivingFocus )
{
   return( TRUE );
}

// The ctrl cannot be printed!
zBOOL
ZMapAct::PrintZCtrl( zBOOL bTest, zCPCHAR cpcTitle, zLONG lFlags,
                     zSHORT nFontSize, zCPCHAR cpcFontFace )
{
   return( FALSE );
}

zSHORT
ZMapAct::PrintZPage( zLONG  lPageNbr,
                     zLONG  lPageWidth,
                     zLONG  lPageHeight )
{
   return( 0 ); // nothing to print
}

ZMapAct *
ZMapAct::GetRealFocus( )
{
   return( this );
}

// The composite should return FALSE if it does not want to receive focus
// and set focus to whichever control should get it.
zBOOL
ZMapAct::InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus )
{
   return( TRUE );
}

zBOOL
ZMapAct::InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus )
{
   return( TRUE );
}

void
ZMapAct::HiliteText( zBOOL bHilite,
                     zLONG lType,
                     zLONG lRange,
                     zBOOL bHiliteItem )
{
}

/////////////////////////////////////////////////////////////////////////////
//
// ParentResize is used to automatically resize/reposition relative
// size/position controls.  The algorithm consists of determining the
// available proportional area of the parent and resizing/repositioning
// relative size/position controls appropriately.  See the discussion
// given in the ZSubtask::ctor notes.
//
/////////////////////////////////////////////////////////////////////////////
//
void
ZMapAct::ParentResize( ZMapAct *pParent,
                       CSize   *pSize )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "SAK" ) == 0 )
      TraceLineS( "ZMapAct::ParentResize tag: ", *m_pzsTag );
#endif

//#ifdef zNEW_RESIZE
   if ( m_pZSubtask->m_lRelease > g_lRel )
   {
      if ( pParent && pParent != m_pzmaComposite )
         return;  // can't just break
   }
//#endif

   ZAttr aw = Attr;

   do // purist's goto
   {

   if ( (m_ulMapActFlags & zMAPACT_IGNORE_RESIZE) ||
     // (m_lRelPsX == 0 && m_lRelPsY == 0 && m_lRelSzX == 0 && m_lRelSzY == 0) ||
        (mIs_hWnd( m_pCtrl->m_hWnd ) == 0) ||
        (pSize->cx < m_pZSubtask->m_lRelLeft + m_pZSubtask->m_lRelRight && pSize->cy < m_pZSubtask->m_lRelTop + m_pZSubtask->m_lRelBottom) )
   {
      break;
   }

//#ifndef zNEW_RESIZE

   ZMapAct *pCtrl = this;
   while ( pCtrl )
   {
      if ( pCtrl->m_ulMapActFlags & zMAPACT_CLIENT_TYPE )
         break;

      pCtrl = pCtrl->m_pzmaComposite;
   }

   if ( m_pZSubtask->m_lRelease > g_lRel )
   {
      if ( pCtrl && (pCtrl->m_ulMapActFlags & zMAPACT_CLIENT_TYPE) &&
           pParent != pCtrl )
      {
         break;
      }
   }
   else
// if ( m_pZSubtask->m_lRelease <= g_lRel )
   {
      if ( pParent != pCtrl )
      {
         return;  // can't just break
      }
   }

//#endif

   ZAttr as = Attr;
   zLONG lRemainX;
   zLONG lRemainY;

#ifdef DEBUG_RESIZE
   zCHAR szTag[ zTAG_LTH ];
   strcpy_s( szTag, sizeof( szTag ), *m_pzsTag );

// if ( zstrcmp( "OK", *m_pzsTag ) == 0 || zstrcmp( "Cancel", *m_pzsTag ) == 0 )
//    TraceLineS( "ZMapAct::ParentResize ", *m_pzsTag );
// if ( zstrcmp( "OK", *m_pzsTag ) == 0 || zstrcmp( "Cancel", *m_pzsTag ) == 0 )
   if ( (szTag[ 0 ] == 'L' && szTag[ 1 ] == 'i' &&
         szTag[ 2 ] == 's' && szTag[ 3 ] == 't' &&
         szTag[ 4 ] == 'B' && szTag[ 5 ] == 'o' &&
         szTag[ 6 ] == 'x' && szTag[ 7 ] == 'S' &&
         szTag[ 8 ] == 'i' && szTag[ 9 ] == 'z' &&
         szTag[ 10 ] == 'e' && szTag[ 11 ] == 0) )
   {
      TraceLineS( "ZMapAct::ParentResize ", *m_pzsTag );
      TraceLineI( "Attr.Y: ", aw.Y );
      TraceLineI( "Attr.H: ", aw.H );
   }
#endif

   if ( m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SINGLE_RELSIZE )
   {
      if ( m_nRelSizeTotalY )
      {
         lRemainY = pSize->cy - m_pZSubtask->m_lOrigRelSizeY;
         if ( m_lRelPsY )
         {
            aw.Y = Attr.Y + ((lRemainY / m_nRelSizeTotalY) * m_nRelSizeCntY);
            if ( aw.Y < 0 )
               aw.Y = 0;
         }

         if ( m_lRelSzY )
         {
            aw.H = m_lRelSzY + (lRemainY / m_nRelSizeTotalY);
            if ( aw.H < 0 )
               aw.H = 10;  // arbitrary small size

            if ( aw.H > pSize->cy )
               aw.H = pSize->cy - Attr.Y;  // no sense going beyond screen size
         }
      }
   }
   else
   {
      // Determine available proportional vertical space on the client area.
      lRemainY = pSize->cy - m_pZSubtask->m_lRelTop - m_pZSubtask->m_lRelBottom;
      if ( lRemainY > 0 )
      {
         if ( m_lRelSzY && m_lRelPsY )
         {
            aw.Y = ((lRemainY * m_lRelPsY) / m_pZSubtask->m_lOrigRelSizeY) + m_pZSubtask->m_lRelTop;
            aw.H = ((lRemainY * m_lRelSzY) / m_pZSubtask->m_lOrigRelSizeY);
         }
         else
         if ( m_lRelPsY )
         {
            aw.Y = ((lRemainY * m_lRelPsY) / m_pZSubtask->m_lOrigRelSizeY) + m_pZSubtask->m_lRelTop;
            aw.H = ((lRemainY * Attr.H) / m_pZSubtask->m_lOrigRelSizeY);
            aw.Y = aw.Y + aw.H - Attr.H;
            aw.H = Attr.H;
         }
         else
         if ( m_lRelSzY )
         {
            aw.H = ((Attr.Y + m_lRelSzY - m_pZSubtask->m_lRelTop) * lRemainY / m_pZSubtask->m_lOrigRelSizeY) - (Attr.Y - m_pZSubtask->m_lRelTop);
         }
      }
      else
      if ( m_lRelSzY )
         aw.H = 0;
   }

   if ( m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SINGLE_RELSIZE )
   {
      if ( m_nRelSizeTotalX )
      {
         lRemainX = pSize->cx - m_pZSubtask->m_lOrigRelSizeX;
         if ( m_lRelPsX )
         {
            aw.X = Attr.X + ((lRemainX / m_nRelSizeTotalX) * m_nRelSizeCntX);
            if ( aw.X < 0 )
               aw.X = 0;
         }

         if ( m_lRelSzX )
         {
            aw.W = m_lRelSzX + (lRemainX / m_nRelSizeTotalX);
            if ( aw.W < 0 )
               aw.W = 10;  // arbitrary small size

            if ( aw.W > pSize->cx )
               aw.W = pSize->cx - Attr.X;  // no sense going beyond screen size
         }
      }
   }
   else
   {
      // Determine available proportional vertical space on the client area.
      lRemainX = pSize->cx - m_pZSubtask->m_lRelLeft - m_pZSubtask->m_lRelRight;
      if ( lRemainX > 0 )
      {
         if ( m_lRelSzX && m_lRelPsX )
         {
            aw.X = ((lRemainX * m_lRelPsX) / m_pZSubtask->m_lOrigRelSizeX) + m_pZSubtask->m_lRelLeft;
            aw.W = ((lRemainX * m_lRelSzX) / m_pZSubtask->m_lOrigRelSizeX);
         }
         else
         if ( m_lRelPsX )
         {
            aw.X = ((lRemainX * m_lRelPsX) / m_pZSubtask->m_lOrigRelSizeX) + m_pZSubtask->m_lRelLeft;
            aw.W = ((lRemainX * Attr.W) / m_pZSubtask->m_lOrigRelSizeX);
            aw.X = aw.X + aw.W - Attr.W;
            aw.W = Attr.W;
         }
         else
         if ( m_lRelSzX )
         {
            aw.W = ((Attr.X + m_lRelSzX - m_pZSubtask->m_lRelLeft) * lRemainX / m_pZSubtask->m_lOrigRelSizeX) - (Attr.X - m_pZSubtask->m_lRelLeft);
         }
      }
      else
      if ( m_lRelSzX )
         aw.W = 0;
   }

#ifdef DEBUG_RESIZE
   if ( m_lRelPsX || m_lRelPsY || m_lRelSzX || m_lRelSzY )
   {
   if ( (szTag[ 0 ] == 'L' && szTag[ 1 ] == 'i' &&
         szTag[ 2 ] == 's' && szTag[ 3 ] == 't' &&
         szTag[ 4 ] == 'B' && szTag[ 5 ] == 'o' &&
         szTag[ 6 ] == 'x' && szTag[ 7 ] == 'S' &&
         szTag[ 8 ] == 'i' && szTag[ 9 ] == 'z' &&
         szTag[ 10 ] == 'e' && szTag[ 11 ] == 0) )
      {
         TraceLineS( "Ctrl tag resize ===> ", *m_pzsTag );
         TraceLineI( "Size.X ", pSize->cx );
         TraceLineI( "Size.Y ", pSize->cy );

         TraceLineI( "m_lRelPsX = ", m_lRelPsX );
         TraceLineI( "m_lRelSzX = ", m_lRelSzX );
         TraceLineI( "lRemainX = ", lRemainX );
         TraceLineI( "m_lRelPsY = ", m_lRelPsY );
         TraceLineI( "m_lRelSzY = ", m_lRelSzY );
         TraceLineI( "lRemainY = ", lRemainY );
         TraceLineI( "Attr.W = ", aw.W );
         TraceLineI( "Attr.H = ", aw.H );
         TraceLineI( "Attr.X = ", aw.X );
         TraceLineI( "Attr.Y = ", aw.Y );
      }
   }
#endif

   if ( m_ulMapActFlags & zMAPACT_FULL_CLIENT )
   {
      if ( m_lRelSzY )
         aw.H = pSize->cy - Attr.Y;

      if ( m_lRelSzX )
         aw.W = pSize->cx - Attr.X;
   }

   if ( as.X != aw.X || as.Y != aw.Y || as.W != aw.W || as.H != aw.H )
      m_pCtrl->MoveWindow( aw.X, aw.Y, aw.W, aw.H, TRUE );

   } while ( FALSE );  // end of:  purist's goto

//#ifdef zNEW_RESIZE
   if ( m_pZSubtask->m_lRelease > g_lRel )
   {
      CSize size( aw.W, aw.H );
      ParentResize( this, &size );
   }
//#endif
}

void
ZMapAct::ParentMove( )
{
}

 VOID
ZMapAct::PaletteChanged( )
{
   return;
}

void
ZMapAct::ParentActivated( )
{
   return;
}

// This is a method which can be used to get any message to the "other
// side" (the Control side of the processing).  The pchMessage parameter
// is a string that is to be queried by the receiving control to determine
// if the message applies.
zLONG
ZMapAct::MessageFromMapAct( zPCHAR pchMessage,
                            WPARAM wParam,
                            LPARAM lParam )
{
   return( 0 );
}

void
ZMapAct::ResetFocus( )
{
   if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
      m_pCtrl->SetFocus( );
}

/////////////////////////////////////////////////////////////////////////////
//
// Controls which want to handle accelerator keys (for example when they
// are in a modal state) should override this method.  The control is
// given the first shot at the accelerator key.  If the control handles
// the accelerator key, return TRUE.  Otherwise, return FALSE.  Any control
// that wants to handle this message must set its Subtask's m_pzmaAccel
// to itself (it may want to check to determine if another control has
// established itself) until it is out of the modal state, at which time
// the Subtask's m_pzmaAccel must be reset to 0.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZMapAct::HandleAccel( zACCEL_DEF& ad )
{
   return( FALSE );
}

// When a composite ctrl (such as a tab ctrl) wants to process keystrokes
// properly, sometimes it has to ask if a particular keystroke should be
// handled by the ctrl that currently has focus (such as a radio button).
zBOOL
ZMapAct::HandleQuickKey( zACCEL_DEF ad )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMapAct::HandleQuickKey ", *m_pzsTag );
#endif

   return( FALSE );
}

#define IDM_GETACTIONTRACE 204

zLONG
ZMapAct::GetZCtrlState( zLONG lStatusType )
{
   if ( lStatusType == zCONTROL_STATUS_ENABLED )
      return( (m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE );
   else
   if ( lStatusType == zCONTROL_STATUS_VISIBLE )
      return( (m_ulMapActFlags & zMAPACT_VISIBLE ) ? TRUE : FALSE );
   else
   if ( lStatusType == zCONTROL_STATUS_AUTOMAP_FROM_OI )
      return( (m_ulMapActFlags & zMAPACT_NOAUTOMAP_FROM_OI) ? FALSE : TRUE );
   else
   if ( lStatusType == zCONTROL_STATUS_AUTOMAP_TO_OI )
      return( (m_ulMapActFlags & zMAPACT_NOAUTOMAP_TO_OI) ? FALSE : TRUE );
   else
   if ( lStatusType == zCONTROL_STATUS_DISABLE_READONLY )
      return( (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) ? TRUE : FALSE );
   else
   if ( lStatusType == zCONTROL_STATUS_ABSOLUTE_PIXEL_SIZE )
      return( (m_ulMapActFlags & zMAPACT_ABSOLUTE_PIXEL_SIZE) ? TRUE : FALSE );
   else
   if ( lStatusType == zCONTROL_STATUS_ABSOLUTE_PIXEL_POS )
      return( (m_ulMapActFlags & zMAPACT_ABSOLUTE_PIXEL_POS) ? TRUE : FALSE );
   else
   if ( lStatusType == zCONTROL_STATUS_ENABLE_TABSTOP )
      return( (m_ulMapActFlags & zMAPACT_FOCUS_ENABLED) ? TRUE : FALSE );
   else
   if ( lStatusType == zCONTROL_STATUS_BACKGROUND_COLOR )
      return( m_clrBk );
   else
   if ( lStatusType == zCONTROL_STATUS_TEXT_COLOR )
      return( m_clrText );
   else
   if ( lStatusType == zCONTROL_STATUS_CREATED )
      return( (m_ulMapActFlag2 & zMAPACT_CREATED_ONCE) ? TRUE : FALSE );
   else
   if ( lStatusType == zCONTROL_STATUS_MAPPED )
      return( (m_ulMapActFlag2 & zMAPACT_MAPPED_ONCE) ? TRUE : FALSE );
   else
      return( 0 );
}

zLONG
ZMapAct::SetZCtrlState( zLONG  lStatusType,
                        zULONG ulValue )
{
   zLONG lRC = 0;

#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "Mr1" ) == 0 || zstrcmp( *m_pzsTag, "Mr2" ) == 0 )
   {
      TraceLineS( "ZMapAct::SetZCtrlState ", *m_pzsTag );
   // TraceLineX( "ZMapAct::SetZCtrlState lStatusType: ", lStatusType );
   // TraceLineX( "ZMapAct::SetZCtrlState MapActFlags: ", m_ulMapActFlags );
   }
#endif

   switch ( lStatusType )
   {
      case zCONTROL_STATUS_HWND:
      {
         m_pCtrl->m_hWnd = (HWND) ulValue;
         break;
      }

      case zCONTROL_STATUS_ENABLED:
      {
         // Shut off auto enabled flag ... user is overriding enabled status.
         m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
         if ( ulValue )
         {
            m_ulMapActFlags |= zMAPACT_ENABLED;
         }
         else
         {
            m_ulMapActFlags &= ~zMAPACT_ENABLED;
            if ( mIs_hWnd( m_pCtrl->m_hWnd ) &&
                 m_pZSubtask->m_pzmaWithFocus == this )
            {
               // Disabled control cannot have focus.
               m_pZSubtask->ResetFocus( this );
            }
         }

#ifdef zREMOTE_SERVER
         SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled",
                                  ulValue ? "Y" : "N" );
#else
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
            lRC = m_pCtrl->EnableWindow( (zBOOL) ulValue );
#endif

         break;
      }

      case zCONTROL_STATUS_AUTOGRAY:
      {
         if ( ulValue )
         {
            // We can't auto-enable a manually disabled control.
            if ( m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
            {
               m_ulMapActFlags |= zMAPACT_ENABLED;
               m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
            // TraceLineS( "AUTOGRAYING: ", *m_pzsTag );
            }
            else
            {
            // TraceLineS( "CANNOT AUTOGRAY: ", *m_pzsTag );
            }
         }
         else
         {
            // We can't auto-disable a manually disabled control.
            if ( m_ulMapActFlags & zMAPACT_ENABLED )
            {
               m_ulMapActFlags &= ~zMAPACT_ENABLED;
               m_ulMapActFlags |= zMAPACT_AUTO_DISABLED;

               // We can't auto-disable a control with more than one action
               // unless all the actions are disabled.  I am going to make
               // the code match the previous statement ... dks 2005.04.26
               if ( m_pchEventAction )
               {
                  zLONG  l;
                  zSHORT k = 0;
                  zSHORT nCnt = 0;

                  for ( k = 0; k < m_nEventActionCnt; k++ )
                  {
                  // l = *((zPLONG) (m_pchEventAction + (k * 2 * sizeof( zLONG ))));
                     l = *((zPLONG) (m_pchEventAction + ((k * 2 * sizeof( zLONG )) + sizeof( zLONG ))));
                     if ( m_pZSubtask->m_pchEnabledAction[ (zSHORT) (l + sizeof( zSHORT )) ] > 0 )
                     {
                        nCnt++;
                        if ( nCnt > 1 )
                        {
                           m_ulMapActFlags |= zMAPACT_ENABLED;
                           m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
                           break;
                        }
                     }
                  }
               }
            }
         }

#ifdef zREMOTE_SERVER
         zBOOL b = (m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE;
         SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled", b ? "Y" : "N" );
#else
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            zBOOL b = (m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE;
            if ( b == FALSE && this == m_pZSubtask->m_pzmaWithFocus )
               m_pZSubtask->ResetFocus( this );

            m_pCtrl->EnableWindow( b );
         }
#endif

         break;
      }

      case zCONTROL_STATUS_VISIBLE:
      {
#ifdef zREMOTE_SERVER
         SetRemoteZCtrlAttribute( this, "Ctrl", "Visible", ulValue ? "Y" : "N" );
#endif
         if ( ulValue )
         {
            m_ulMapActFlags |= zMAPACT_VISIBLE;
            if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
               lRC = m_pCtrl->ShowWindow( SW_SHOW );
         }
         else
         {
            m_ulMapActFlags &= ~zMAPACT_VISIBLE;
            if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
               lRC = m_pCtrl->ShowWindow( SW_HIDE );
         }

         break;
      }

      case zCONTROL_STATUS_MAP_ERROR:
      case zCONTROL_STATUS_ERROR:
      {
         // Alter color control background/text to be in error.
         m_ulCtrlErr = ulValue;

#ifdef zREMOTE_SERVER
         if ( ulValue )
            SetRemoteCtrlAttribute( m_pZSubtask->m_vDialog, *m_pzsTag, "Ctrl", "Error", "Y" );
#else
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
            m_pCtrl->Invalidate( TRUE );
#endif

         if ( ulValue && lStatusType == zCONTROL_STATUS_MAP_ERROR && m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr == 0 )
         {
            m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr = this;
         }

         break;
      }

      case zCONTROL_STATUS_BACKGROUND_COLOR:
      {
         mDeleteInit( m_pBrush );

         m_clrBk = ulValue;
         m_pBrush = new CBrush( m_clrBk );
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
            InvalidateChildRect( this, TRUE );

         break;
      }

      case zCONTROL_STATUS_TEXT_COLOR:
      {
         m_clrText = ulValue;
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
            InvalidateChildRect( this, FALSE );

         break;
      }

      case zCONTROL_STATUS_TOPMOST:
      {
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
            m_pCtrl->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

         break;
      }

      case zCONTROL_STATUS_BOTTOMMOST:
      {
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
            m_pCtrl->SetWindowPos( &CWnd::wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

         break;
      }

      case zCONTROL_STATUS_AUTOMAP_FROM_OI:
      {
         if ( ulValue )
            m_ulMapActFlags &= ~zMAPACT_NOAUTOMAP_FROM_OI;
         else
            m_ulMapActFlags |= zMAPACT_NOAUTOMAP_FROM_OI;

         break;
      }

      case zCONTROL_STATUS_AUTOMAP_TO_OI:
      {
         if ( ulValue )
            m_ulMapActFlags &= ~zMAPACT_NOAUTOMAP_TO_OI;
         else
            m_ulMapActFlags |= zMAPACT_NOAUTOMAP_TO_OI;

         break;
      }

      case zCONTROL_STATUS_DISABLE_READONLY:
      {
         if ( ulValue )
         {
            m_ulMapActFlags |= zMAPACT_DISABLE_READONLY;
            m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
         }
         else
         {
            m_ulMapActFlags &= ~zMAPACT_DISABLE_READONLY;
            m_ulMapActFlags |= zMAPACT_FOCUS_ENABLED;
         }

#ifdef zREMOTE_SERVER
         SetRemoteZCtrlAttribute( this, "Ctrl", "ReadOnly", ulValue ? "Y" : "N" );
#endif
         break;
      }

      case zCONTROL_STATUS_ABSOLUTE_PIXEL_SIZE:
      {
         if ( ulValue )
            m_ulMapActFlags |= zMAPACT_ABSOLUTE_PIXEL_SIZE;
         else
            m_ulMapActFlags &= ~zMAPACT_ABSOLUTE_PIXEL_SIZE;

         break;
      }

      case zCONTROL_STATUS_ABSOLUTE_PIXEL_POS:
      {
         if ( ulValue )
            m_ulMapActFlags |= zMAPACT_ABSOLUTE_PIXEL_POS;
         else
            m_ulMapActFlags &= ~zMAPACT_ABSOLUTE_PIXEL_POS;

         break;
      }

      case zCONTROL_STATUS_REPAINT:
      {
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            m_pCtrl->Invalidate( );
            if ( ulValue & 0x00000001 )
            {
               HWND     hWnd = m_pCtrl->m_hWnd;
               ZSubtask *pZSubtask = m_pZSubtask;
               ZDrView  *pZView = pZSubtask->m_pZView;

               do
               {
                  PumpMessages( pZSubtask->m_vDialog );

                  if ( mIs_hWnd( hWnd ) == FALSE )
                     break;

                  if ( pZView )
                     pZView = pZView->m_pNextView;

                  if ( pZView )
                     pZSubtask = pZView->m_pZSubtask;
                  else
                     pZSubtask = 0;

               }  while ( pZSubtask && pZSubtask != m_pZSubtask );
            }

            if ( ulValue & 0x00000002 )
               SysSendOE_Message( IDM_GETACTIONTRACE, 0 ); // get task switch
         }
         else
            lRC = 1;

         break;
      }

      case zCONTROL_STATUS_MARK_READONLY_ENABLED:
      {
         SetZCtrlState( zCONTROL_STATUS_DISABLE_READONLY, TRUE );
         SetZCtrlState( zCONTROL_STATUS_ENABLED, ulValue );

         break;
      }

      case zCONTROL_STATUS_ENABLE_TABSTOP:
      {
         if ( ulValue )
            m_ulMapActFlags |= zMAPACT_FOCUS_ENABLED;
         else
            m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
      }

      case zCONTROL_STATUS_RESET:
      case zCONTROL_STATUS_SELECT_INDEX:
      default:
         break;
   }

#ifdef DEBUG_ALL
   TraceLineX( "ZMapAct::SetZCtrlState Term MapActFlags: ", m_ulMapActFlags );
#endif

   return( lRC );
}

zLONG
ZMapAct::SetZCtrlProperty( zLONG   lPropertyType,
                           zULONG  ulNumericProperty,
                           zCPCHAR cpcStringProperty )
{
   zLONG lRC = -1;

#ifdef DEBUG_ALL
   TraceLineS( "ZMapAct::SetZCtrlProperty ", *m_pzsTag );
   TraceLineX( "ZMapAct::SetZCtrlProperty PropertyType: ", lPropertyType );
   TraceLineX( "ZMapAct::SetZCtrlProperty MapActFlags: ", m_ulMapActFlags );
#endif

   switch ( lPropertyType )
   {
      case zCONTROL_PROPERTY_INTEGER_DATA:
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            zCHAR szNumeric[ 64 ];

            _ltoa_s( ulNumericProperty, szNumeric, sizeof( szNumeric ), 10 );
            SetWindowText( m_pCtrl->m_hWnd, szNumeric );
            lRC = 0;
         }

         break;

      case zCONTROL_PROPERTY_STRING_DATA:
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            SetWindowText( m_pCtrl->m_hWnd, cpcStringProperty );
            lRC = 0;
         }

         break;

      case zCONTROL_PROPERTY_TAG:
         mDeleteInit( m_pzsTag );
         m_pzsTag = new CString( cpcStringProperty );
         lRC = 0;
         break;

      case zCONTROL_PROPERTY_FONT:
      {
         if ( ulNumericProperty > 0 && cpcStringProperty &&
              *cpcStringProperty )
         {
            CFont *pFont = new CFont;
            if ( pFont->CreatePointFont( ulNumericProperty, cpcStringProperty ) == 0 )
            {
               mDeleteInit( pFont );
            }
            else
            {
               mDeleteInit( m_pFont );
               m_pFont = pFont;
               if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
               {
                  m_pCtrl->SendMessage( WM_SETFONT, (WPARAM) (HFONT) *m_pFont );
                  m_pCtrl->Invalidate( );
               }

               lRC = 0;
            }
         }

         break;
      }

      default:
         break;
   }

#ifdef DEBUG_ALL
   TraceLineX( "ZMapAct::SetZCtrlProperty Term MapActFlags: ",
               m_ulMapActFlags );
#endif

   return( lRC );
}

zLONG
ZMapAct::GetZCtrlProperty( zLONG   lPropertyType,
                           zPULONG pulNumericProperty,
                           zPCHAR  pchStringProperty,
                           zLONG   lMaxLth )
{
   zLONG lRC = -1;

#ifdef DEBUG_ALL
   TraceLineS( "ZMapAct::GetZCtrlProperty ", *m_pzsTag );
   TraceLineX( "ZMapAct::GetZCtrlProperty PropertyType: ", lPropertyType );
   TraceLineX( "ZMapAct::GetZCtrlProperty MapActFlags: ", m_ulMapActFlags );
#endif

   switch ( lPropertyType )
   {
      case zCONTROL_PROPERTY_INTEGER_DATA:
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            zLONG lLth = GetWindowTextLength( m_pCtrl->m_hWnd );
            zPCHAR pch = new char[ lLth + 1 ];
            GetWindowText( m_pCtrl->m_hWnd, pch, lLth );
            *pulNumericProperty = zatol( pch );
            delete [] pch;
            lRC = 0;
         }
         else
            *pulNumericProperty = 0;

         break;

      case zCONTROL_PROPERTY_STRING_DATA:
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            GetWindowText( m_pCtrl->m_hWnd, pchStringProperty, *pulNumericProperty );
            lRC = 0;
         }
         else
            pchStringProperty[ 0 ] = 0;

         break;

      case zCONTROL_PROPERTY_FONT:
      case zCONTROL_PROPERTY_COLUMN_COUNT:
      {
         lRC = 0;
         break;
      }

      case zCONTROL_PROPERTY_TAG:
         strcpy_s( pchStringProperty, lMaxLth, *m_pzsTag );
         lRC = 0;
         break;

      default:
         break;
   }

#ifdef DEBUG_ALL
   TraceLineX( "ZMapAct::GetZCtrlProperty Term MapActFlags: ", m_ulMapActFlags );
#endif

   return( lRC );
}

zBOOL
ZMapAct::DrawZCtrl( CDC *pDC, CRect& rectDraw )
{
   return( FALSE );
}

// default works for Domain type ComboBox ... PAS 2004.02.03
zSHORT
ZMapAct::SetCtrlMapping( zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                         zCPCHAR cpcAttrib, zCPCHAR cpcContext, zLONG lColumn )
{
   mDeleteInit( m_pzsVName );
   mDeleteInit( m_pzsEName );
   mDeleteInit( m_pzsAName );
   mDeleteInit( m_pzsContext );

   if ( cpcViewName )
      m_pzsVName = new CString( cpcViewName );

   if ( cpcEntity )
      m_pzsEName = new CString( cpcEntity );

   if ( cpcAttrib )
      m_pzsAName = new CString( cpcAttrib );

   if ( cpcContext )
      m_pzsContext = new CString( cpcContext );

   return( 0 );
}

// default works for Domain type ComboBox ... PAS 2004.02.03
zSHORT
ZMapAct::GetCtrlMapping( zPCHAR pchViewName, zPCHAR pchEntity,
                         zPCHAR pchAttrib, zPCHAR pchContext, zLONG lColumn )
{
   if ( m_pzsVName )
      strcpy_s( pchViewName, zTAG_LTH, *m_pzsVName );
   else
      pchViewName[ 0 ] = 0;

   if ( m_pzsEName )
      strcpy_s( pchEntity, zTAG_LTH, *m_pzsEName );
   else
      pchEntity[ 0 ] = 0;

   if ( m_pzsAName )
      strcpy_s( pchAttrib, zTAG_LTH, *m_pzsAName );
   else
      pchAttrib[ 0 ] = 0;

   if ( m_pzsContext )
      strcpy_s( pchContext, zTAG_LTH, *m_pzsContext );
   else
      pchContext[ 0 ] = 0;

   return( 0 );
}

zSHORT
ZMapAct::SetRowColText( zCPCHAR cpcText, zLONG lRow, zLONG lColumn )
{
   if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
   {
      m_pCtrl->SetWindowText( cpcText );
      m_pCtrl->Invalidate( );  // to force repaint
   }

   return( 0 );
}

zSHORT
ZMapAct::GetRowColText( zPCHAR pchText, zUSHORT uLth,
                        zLONG lRow, zLONG lColumn )
{
   if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
      m_pCtrl->GetWindowText( pchText, uLth );
   else
      *pchText = 0;

   return( 0 );
}

zBOOL
ZMapAct::BeginDrag( ZDragDrop *pDragDrop )
{
   return( TRUE );
}

zBOOL
ZMapAct::DoDrop( ZDragDrop *pDragDrop )
{
   zLONG  lEvent = GetDragDropEventId( eDoDrop );
   return( ProcessEvent( this, lEvent ) );
}

zBOOL
ZMapAct::Drop( ZDragDrop *pDragDrop )
{
   zLONG  lEvent = GetDragDropEventId( eDrop );
   return( ProcessEvent( this, lEvent ) );
}

zBOOL
ZMapAct::QueryDrop( ZDragDrop *pDragDrop )
{
// TraceLineS( "ZMapAct::QueryDrop: ", *m_pzsTag );
   zLONG  lRC;
   zLONG  lEvent = GetDragDropEventId( eQueryDrop );
   lRC = ProcessImmediateEvent( this, lEvent, pDragDrop );
   if ( lRC != zNO_APPLICATION_EVENT )
   {
      return( (zBOOL) LOWORD( lRC ) ); // an event was found ... code set RC
   }

   if ( m_pchDragDrop )
   {
      if ( *m_pchDragDrop == 'B' || *m_pchDragDrop == 'T' )
      {
         zPCHAR pch = m_pchDragDrop + (2 * sizeof( zLONG ));
         if ( *m_pchDragDrop == 'B' )
         {
            pch += *((zLONG *) (m_pchDragDrop + sizeof( zLONG )));
         }

         if ( zstrcmp( pch, pDragDrop->szSrcDragKey ) == 0 )
            return( TRUE );
      }
   }

   return( FALSE );
}

zLONG
ZMapAct::GetDragDropEventId( eDRAGDROP_EVENT eEventType )
{
   switch ( eEventType )
   {
      case eBeginDrag:
         return( zCTRL_EVENT_BEGINDRAG );
      case eQueryDrop:
         return( zCTRL_EVENT_QUERYDROP );
      case eDoDrop:
         return( zCTRL_EVENT_DODROP );
      case eDrop:
         return( zCTRL_EVENT_DROP );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// bToOI parameter:
//    TRUE  - MapToOI
//    FALSE - MapFromOI
//
// This routine maps data TO or FROM a control.  If mapping TO a control,
// the routine first ensures the control is created.  If mapping FROM a
// control, the routine destroys the control after mapping is complete.
// This routine is thereby recursive in nature and so care is taken to
// create children after the parent is created and destroying children
// prior to destroying the parent.
//
// Note that the create/destroy is done conditionally based on the
// CreateDestroy parameter:
//    TRUE  - Create or Destroy as well as map
//    FALSE - Simply map as directed
//
// MapChildren returns the return code from MapToOI if bToOI is specified.
// A zero return code indicates success; otherwise, mapping is unsuccessful.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZMapAct::MapChildren( zBOOL  bToOI,
                      zBOOL  bCreateDestroy )
{
   ZMapAct *pzma;
   zUSHORT nIdx;

   if ( bToOI )
   {
   // TraceLineS( "ZMapAct::MapChildren: ", "ToOI" );
      // If we are not destroying the control, simply validate the mapping.
      if ( MapToOI( bCreateDestroy ? 0 : zVALIDATE ) )
      {
         if ( m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr == 0 )
         {
            SetZCtrlState( zCONTROL_STATUS_ERROR, TRUE );
            m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr = this;
         }

         return( zCALL_ERROR );
      }
      else
         SetZCtrlState( zCONTROL_STATUS_ERROR, FALSE );

      // First validate mapping for all children.
      for ( nIdx = 1; nIdx <= m_pZSubtask->m_pZMIXCtrl->GetCount( ); nIdx++ )
      {
         pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( nIdx );
         if ( pzma->m_pzmaComposite == this )
         {
            if ( pzma->MapChildren( TRUE, FALSE ) )   // validate mapping
               return( zCALL_ERROR );
         }
      }

      // If we get here, mapping is AOK ... so get on with real mapping and
      // destroy controls at this level.
      for ( nIdx = 1; nIdx <= m_pZSubtask->m_pZMIXCtrl->GetCount( ); nIdx++ )
      {
         pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( nIdx );
         if ( pzma->m_pzmaComposite == this )
         {
            pzma->MapChildren( TRUE, TRUE );  // map and destroy this level
         }
      }

      if ( m_pCtrl && bCreateDestroy ) // && (m_ulMapActFlag2 & zMAPACT_CREATED) )
      {
         DestroyZ( );
      }
   }
   else
   {
   // TraceLineS( "ZMapAct::MapChildren: ", "FromOI" );
      if ( m_pCtrl && bCreateDestroy ) // && (m_ulMapActFlag2 & zMAPACT_CREATED) == 0 )
      {
         CreateZ( );
      }

      for ( nIdx = 1; nIdx <= m_pZSubtask->m_pZMIXCtrl->GetCount( ); nIdx++ )
      {
         pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( nIdx );
         if ( pzma->m_pzmaComposite == this )
         {
            pzma->MapChildren( FALSE, TRUE );   // create this level
         }
      }

      // The following MapFromOI is moved from prior to the above "for loop"
      // to here so that default Radio Button mapping works properly.
      MapFromOI( );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This routine sets the CtrlState for the descendents of a control.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZMapAct::SetChildCtrlState( zLONG  lStatusType,
                            zULONG ulValue,
                            zBOOL  bAtLevel )
{
   ZMapAct *pzma;
   zUSHORT nIdx;

// if ( zstrcmp( *m_pzsTag, "RadioGrp1" ) == 0 )
//    TraceLineS( "ZMapAct::SetChildCtrlState: ", *m_pzsTag );

   // Set the CtrlState for all child controls of this control
   for ( nIdx = 1; nIdx <= m_pZSubtask->m_pZMIXCtrl->GetCount( ); nIdx++ )
   {
      pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( nIdx );
      if ( pzma->m_pzmaComposite == this )
      {
         // set CtrlState for the child level
         pzma->SetChildCtrlState( lStatusType, ulValue, TRUE );
      }
   }

   if ( bAtLevel )
   {
      if ( (m_ulMapActFlag2 & zMAPACT_INPLACE_CTRL) == 0 )
         return( (zSHORT) SetZCtrlState( lStatusType, ulValue ) );
   }

   return( 0 );
}

zBOOL
ZMapAct::IsAncestor( ZMapAct *pzma ) const
{
   while ( pzma )
   {
      if ( pzma == this )
         break;

      pzma = pzma->m_pzmaComposite;
   }

   return( pzma ? TRUE : FALSE );
}

/////////////////////////////////////////////////////////////////////////////
//
// get the current XRA and position to the current control
// so that remote data can be attached
//
// passing true creates the entity representing the control if there
// is none
//
/////////////////////////////////////////////////////////////////////////////
#ifdef zREMOTE_SERVER
zVIEW
ZMapAct::SetupRemoteObject( zBOOL bCreate )
{
#ifdef zREMOTE_SERVER
   // Get the view that represents the current kzxra.
   ZSocketTask *pZSocketTask = m_pZSubtask->m_pZSocketTask;
   ZXRA_Item *pXRA;
   zVIEW  vXRA = 0;

   if ( pZSocketTask &&
        (pXRA = pZSocketTask->m_pCacheWndList->Find( *(m_pZSubtask->m_pzsRemoteWndTag), m_pZSubtask, "ZMapAct::SetupRemoteObject" )) != 0 &&
        (pXRA->m_vXRA || pXRA->m_vXRA_Delta) )
   {
      if ( pXRA->m_vXRA_Delta == 0 )
         pXRA->ActivateEmptyDelta( pZSocketTask->m_vRegisteredSubtask, pZSocketTask->m_szAppTag );

      CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA_Delta, m_pZSubtask->m_vDialog );
      while ( ResetViewFromSubobject( vXRA ) == 0 )
      {
      }

      if ( CheckExistenceOfEntity( vXRA, "Wnd" ) < zCURSOR_SET ||
           SetCursorFirstEntityByString( vXRA, "Wnd", "Tag", *(m_pZSubtask->m_pzsRemoteWndTag), 0 ) < zCURSOR_SET )
      {
         CreateEntity( vXRA, "Wnd", zPOS_AFTER );
         SetAttributeFromString( vXRA, "Wnd", "Tag", *(m_pZSubtask->m_pzsRemoteWndTag) );
      }

      // Determine the current ctrl's entity in KZXRA ... if the entity is not there, create it (if bCreate is TRUE).
      if ( PositionOnRemoteCtrl( this, vXRA, bCreate ) == FALSE )
      {
         DropView( vXRA );
         vXRA = 0;
      }
   }

   return( vXRA );
#else
   return( 0 );
#endif // zREMOTE_SERVER
}
#endif // zREMOTE_SERVER


zBOOL
ZMapAct::IsEventHandled( zLONG lEvent )
{
   zLONG lSize = m_aHandledEvents.GetSize( );
   zLONG k;

   // Determine if it is in the list.
   for ( k = 0; k < lSize; k++ )
   {
      zLONG lIdNbr = (zLONG) m_aHandledEvents.ElementAt( k );
      if ( lIdNbr == lEvent )
         return( TRUE );
   }

   return( FALSE );
}

zLONG
ZMapAct::SelectAll( )
{
// TraceLineS( "ZMapAct::SelectAll default: ", *m_pzsTag );
   return( 0 );
}

zLONG
ZMapAct::TransferTextFromInplace( zCPCHAR cpcText )
{
// TraceLineS( "ZMapAct::TransferTextFromInplace default: ", *m_pzsTag );
   SetRowColText( cpcText );
   return( 0 );
}

//
// Must be called from the wrapper dll to notify the driver
// that an event is handled by wrapper code.
//
// returns the index of the added event
//
zLONG
ZMapAct::RegisterEvent( zLONG nEvent )
{
   // Add it and don't worry if it's already there because it doesn't matter.
   zLONG lReturn = m_aHandledEvents.Add( (DWORD) nEvent );
   return( lReturn );
}

zLONG
ZMapAct::HandleEvent( zLONG lEvent )
{
   // No more code is to be executed after handling an event.
   return( zACTIVEX_EVENT_COMPLETED );
}

zBOOL
ZMapAct::SetFontOverride( )
{
   if ( m_pFont )
   {
      m_pCtrl->SendMessage( WM_SETFONT, (WPARAM) (HFONT) *m_pFont );
      return( TRUE );
   }
   else
   if ( m_pZSubtask->m_pFontCtrl )
   {
   // m_pCtrl->SetFont( m_pZSubtask->m_pFontCtrl );
      m_pCtrl->SendMessage( WM_SETFONT, (WPARAM)(HFONT) *(m_pZSubtask->m_pFontCtrl) );
      return( TRUE );
   }
   else
   if ( m_pZSubtask->m_pFontWS )
   {
      m_pCtrl->SendMessage( WM_SETFONT, (WPARAM)(HFONT) *(m_pZSubtask->m_pFontWS) );
      return( TRUE );
   }

   return( FALSE );
}

void
ZMapAct::SetTranslationList( zCPCHAR cpcPairedList, zSHORT nCol )
{
   ZCorrelatedItem *pItem = 0;
   if ( m_pTranslationList == 0 )
      m_pTranslationList = new ZCorrelatedList;
   else
   {
      pItem = m_pTranslationList->Remove( nCol );
      if ( pItem )
         delete( pItem );
   }

   m_pTranslationList->Add( nCol, cpcPairedList );
}

zBOOL
ZMapAct::TranslateValue( zPCHAR pchValue, zLONG lMaxLth, zBOOL bToDisplay, zSHORT nCol )
{
   ZCorrelatedItem *pItem;

   if ( m_pTranslationList == 0 ||
        (pItem = m_pTranslationList->Find( nCol )) == 0 )
   {
      return( FALSE );
   }

   zPCHAR pchInternal;
   zPCHAR pchDisplay;
   zPCHAR pchSemiInternal;
   zPCHAR pchSemiDisplay;
   zBOOL  bFound = FALSE;
   zPCHAR pch = pItem->m_csValue.GetBuffer( pItem->m_csValue.GetLength( ) );

   while ( pch && *pch )
   {
      pchSemiInternal = zstrchr( pch, ';' );
      if ( pchSemiInternal )
      {
         pchInternal = pch;
         *pchSemiInternal = 0;
         pch = pchSemiInternal + 1;
         pchDisplay = pch;
         pchSemiDisplay = zstrchr( pch, ';' );
         if ( pchSemiDisplay )
         {
            *pchSemiDisplay = 0;
            pch = pchSemiDisplay + 1;
         }
         else
         {
            pch = 0;
         }

         if ( bToDisplay )
         {
            if ( zstrcmp( pchInternal, pchValue ) == 0 )
            {
               strcpy_s( pchValue, lMaxLth, pchDisplay );
               pch = 0;
               bFound = TRUE;
            }
         }
         else
         {
            if ( zstrcmp( pchDisplay, pchValue ) == 0 )
            {
               strcpy_s( pchValue, lMaxLth, pchInternal );
               pch = 0;
               bFound = TRUE;
            }
         }

         *pchSemiInternal = ';';
         if ( pchSemiDisplay )
            *pchSemiDisplay = ';';
      }
      else
         pch = 0;
   }

   pItem->m_csValue.ReleaseBuffer( -1 );
   return( bFound );
}

/////////////////////////////////////////////////////////////////////////////
// Assigns an instance of the wrapper class to member that will be passed
// back to Application global operations.
//
void
ZMapAct::SetWrapperInstance( COleDispatchDriver *pWrapperInstance )
{
// mDeleteInit( m_pWrapperInstance );
   m_pWrapperInstance = pWrapperInstance;

// if ( m_pWrapperInstance->m_bAutoRelease == TRUE )
//    m_pWrapperInstance->m_bAutoRelease = FALSE; // free dispatch poiter separatly
}

COleDispatchDriver *
ZMapAct::GetWrapperInstance( )
{
   return( m_pWrapperInstance );
}

IDispatch *
ZMapAct::GetDispatchInterface( )
{
   return( m_pDispatch );
}

void
ZMapAct::DestroyZ( )
{
   if ( m_pCtrl && mIs_hWnd( m_pCtrl->m_hWnd ) )
      m_pCtrl->DestroyWindow( );

   m_ulMapActFlag2 &= ~(zMAPACT_CREATED | zMAPACT_MAPPED_FROM_OI);
}

// ZMapAct -  dtor for mapping/action object
ZMapAct::~ZMapAct( )
{
// TraceLineS( "ZMapAct dtor for ctrl ", *m_pzsTag );

   mDeleteInit( m_pzsTag );
   mDeleteInit( m_pzsVName );
   mDeleteInit( m_pzsEName );
   mDeleteInit( m_pzsAName );
   mDeleteInit( m_pzsContext );
   mDeleteInit( m_pzsText );
   mDeleteInitA( m_pchDragDrop );
// mDeleteInitA( m_pchEventAction );  // not allocated ... just a pointer
   mDeleteInit( m_pTranslationList );
   mDeleteInit( m_pBrush );
   mDeleteInit( m_pFont );

#if 0
   // We generate code to do this in the wrapper dll
   // should we really take care of this m_pWrapperInstance.
   mDeleteInit( m_pWrapperInstance );
   mReleaseInit( m_pDispatch );
#endif
}

ZDrView *
ZMapAct::GetFormView( )
{
   ZDrView * pZDrView = 0;

   if ( m_pZSubtask )
      pZDrView = m_pZSubtask->m_pZView;

   return( pZDrView );
}
