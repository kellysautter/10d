/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrmisc.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of miscellaneous operations ...
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.03.01    DKS    Z10
//    OperatorPromptForInput corrected after Ctrl-P enhancement using
//    ConstructZeidonPrompt.
//
// 2001.10.02    DKS    HotkeyToPainter
//    Modified PumpMessages to prevent wait.
//
// 2000.04.05    DKS    Z2000   TB1105
//    Fixed urgent mnemonic bug submitted by Ruth.
//
// 1999.07.28    DKS    Z2000   QS999
//    Added list of Zeidon subtasks to ZDr.
//
// 1999.03.10    DKS
//    Altered the TranslateColor function to remove name conflict.
//
// 1999.01.25    DKS
//    Fix to let ComboBox handle Esc key properly (TB 362).
//
// 1999.01.12    DKS
//    Fix to permit the correct window to get an accelerator key (such as
//    VK_DELETE).  This fix is done for the editor, but should be a valid
//    protection in general.  The potential side-effects are quite far
//    reaching and a test suite to fully test would be difficult to set up.
//
// 1998.12.14    DKS
//    Added code for the new Windows key VK_APPS (TB 361).
//
// 1998.08.12    DKS
//    Accelerator processing has changed so that the Shift key setting
//    is always honored when checking for a given accelerator.
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


//./ ADD NAME=zkSelectAll
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  zkSelectAll
//
//  PURPOSE:    Used to handle the Ctrl-A key as the standard select all
//              action.
//
//  PARAMETERS: vSubtask -- current window
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG  OPERATION
zkSelectAll( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;

   GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 );
   if ( pZSubtask )
      return( pZSubtask->SendCtrlSelectAll( ) );

   return( -1 );
}

//./ ADD NAME=zkCancel
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  zkCancel
//
//  PURPOSE:    Used to handle the Escape key as the standard cancel
//              action.
//
//  PARAMETERS: vSubtask -- current window
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
zkCancel( zVIEW vSubtask )
{
   zSHORT nRC;

   nRC = InvokeAction( vSubtask, "ESC" );
   if ( nRC <= -2 )
   {
      MessageSend( vSubtask, "LO00101", "Zeidon Tools",
                   "No Cancel Action Specified\nPlease try Alt+F4.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, FALSE );
   }

   return( 0 );
}

//./ ADD NAME=zkExitCloseOK
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  zkExitCloseOK
//
//  PURPOSE:    Used to handle Alt-F4 key as the standard exit, close,
//              OK action.
//
//  PARAMETERS: vSubtask -- current window
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
zkExitCloseOK( zVIEW vSubtask )
{
   zSHORT nRC;

   nRC = InvokeAction( vSubtask, "Alt-F4" );
   if ( nRC <= -2 )
   {
      if ( MessagePrompt( vSubtask, "LO00102",
                          "Zeidon Tools",
                          "No close action specified.\n"
                          "Permit close?",
                          FALSE, zBUTTONS_YESNO,
                          zRESPONSE_NO, 0 ) == zRESPONSE_YES )
      {
         SetWindowActionBehavior( vSubtask,
                                  zWAB_ReturnToParentWithRefresh |
                                    zWAB_ProcessImmediateAction,
                                  0, 0 );
      }
   }

   return( 0 );
}

zSHORT
ListSubCtrls( ZMapAct *pzmaParent, zPCHAR pchCtrlList,
              zSHORT nTgtLth, zSHORT nMaxLth )
{
   ZSubtask *pZSubtask = pzmaParent->m_pZSubtask;
   zSHORT nCnt = 0;

   if ( pZSubtask->m_pZMIXCtrl )
   {
      ZMapAct *pzma;
      zSHORT  nLineLth = 3;
      zSHORT  nCurrLth = 0;
      zSHORT  nLth;
      zLONG k;

      for ( k = 0; k < pZSubtask->m_pZMIXCtrl->GetCount( ); k++ )
      {
         pzma = (ZMapAct *) *(pZSubtask->m_pZMIXCtrl->m_pvNode + k);
         if ( pzma->m_pzmaComposite == pzmaParent )
         {
            nCnt++;
            if ( nLineLth > 3 )
            {
               pchCtrlList[ nCurrLth++ ] = ',';
               pchCtrlList[ nCurrLth++ ] = ' ';
               nLineLth += 2;
               if ( nLineLth > nTgtLth )
               {
                  strcpy_s( pchCtrlList + nCurrLth, nMaxLth - nCurrLth, "\n  " );
                  nCurrLth += 4;
                  nLineLth = 3;
               }
            }

            strcpy_s( pchCtrlList + nCurrLth, nMaxLth - nCurrLth, *(pzma->m_pzsTag) );
            nLth = (zSHORT) zstrlen( *(pzma->m_pzsTag) );
            nCurrLth += nLth;
            nLineLth += nLth;
            if ( nCurrLth >= nMaxLth - 32 )
            {
               pchCtrlList[ nCurrLth++ ] = ' ';
               pchCtrlList[ nCurrLth++ ] = '.';
               pchCtrlList[ nCurrLth++ ] = '.';
               pchCtrlList[ nCurrLth++ ] = 0;

               break;  // not enough room for another tag
            }
         }
      }
   }

   return( nCnt );
}

//./ ADD NAME=zkCtrlInfo
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  zkCtrlInfo
//
//  PURPOSE:    Used to display the Ctrl Tag with focus.
//
//  PARAMETERS: vSubtask -- current window
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
zkCtrlInfo( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZMapAct *pzma;

      if ( pZSubtask->m_pzmaLastFlyBy )
         pzma = pZSubtask->m_pzmaLastFlyBy;
      else
         pzma = pZSubtask->m_pzmaWithFocus;

      if ( pzma )
      {
         zCHAR  szMsg[ 8092 ];
         zLONG lLth = 0;

         sprintf( szMsg, "Dlg: %s  Wnd: %s  Ctrl: %s",
                  (*(pZSubtask->m_pzsDlgTag)).GetString(),
                  (*(pZSubtask->m_pzsWndTag)).GetString(),
                  (*(pzma->m_pzsTag)).GetString() );
         lLth = zstrlen( szMsg ) + 1;
         if ( zsizeof( szMsg ) - lLth > 50 )
         {
            if ( pzma->m_pzsVName && *(pzma->m_pzsVName) )
            {
               strcat_s( szMsg, zsizeof( szMsg ), " View: " );
               lLth = zstrlen( szMsg ) + 1;
               strncat_s( szMsg, zsizeof( szMsg ), *(pzma->m_pzsVName), zsizeof( szMsg ) - lLth );
            }
         }

         if ( zsizeof( szMsg ) - zstrlen( szMsg ) > 50 )
         {
            if ( pzma->m_pzsEName && *(pzma->m_pzsEName) )
            {
               strcat_s( szMsg, zsizeof( szMsg ), " Entity: " );
               lLth = zstrlen( szMsg ) + 1;
               strncat_s( szMsg, zsizeof( szMsg ), *(pzma->m_pzsEName), zsizeof( szMsg ) - lLth );
            }
         }

         if ( zsizeof( szMsg ) - zstrlen( szMsg ) > 50 )
         {
            if ( pzma->m_pzsAName && *(pzma->m_pzsAName) )
            {
               strcat_s( szMsg, zsizeof( szMsg ), " Attribute: " );
               lLth = zstrlen( szMsg ) + 1;
               strncat_s( szMsg, zsizeof( szMsg ), *(pzma->m_pzsAName), zsizeof( szMsg ) - lLth );
            }
         }

         lLth = zstrlen( szMsg );
         if ( **(pzma->m_pzsText) )
         {
            strcat_s( szMsg, zsizeof( szMsg ), " Text: " );
            strncat_s( szMsg, zsizeof( szMsg ), *(pzma->m_pzsText), zsizeof( szMsg ) - lLth - 1000 );
         }

         lLth = zstrlen( szMsg );
         szMsg[ lLth ] = '\n';
         strcpy_s( szMsg + lLth + 1, zsizeof( szMsg ) - lLth - 1, "Sub Ctrls: " );
         if ( ListSubCtrls( pzma, szMsg + lLth + 12, (zSHORT) lLth, (zSHORT) (zsizeof( szMsg ) - lLth - 16) ) <= 0 )
         {
            szMsg[ lLth ] = 0;
         }

         if ( pZSubtask->m_pZSubtaskGlobal )
            pZSubtask = pZSubtask->m_pZSubtaskGlobal;

      // if ( pZSubtask->m_pDIL && pZSubtask->m_nAutoDIL )
      //    pZSubtask->SetStatusBarText( szMsg, 0 );
      // else
            OperatorSend( vSubtask, "Control with focus Information", szMsg, FALSE );
      }
   }

   return( 0 );
}

COLORREF OPERATION
fnTranslateColor( zLONG rgb )
{
   if ( rgb != -1 && (rgb & 0xFF000000) )
   {
      if ( rgb == zCOLOR_SCROLLBAR )
         rgb = COLOR_SCROLLBAR;   // for windows

      rgb = LOWORD( rgb );
   }

   if ( rgb == -1 || (rgb == 0xFF000000) )
      return( ::GetSysColor( COLOR_WINDOWTEXT ) );

   if ( rgb & 0xFF000000L )
      return( ::GetSysColor( rgb ) );

   return( rgb );
}

// Make sure the entity is in view and then check for existence
// of the entity.
zOPER_EXPORT zSHORT OPERATION
CheckEntityExistence( zVIEW v, zCPCHAR cpcEntityName )
{
   zSHORT nRC;

   nRC = CheckEntityInView( v, cpcEntityName );
   if ( nRC == 0 )
      nRC = CheckExistenceOfEntity( v, cpcEntityName );
   else
      nRC = zCURSOR_ENTITY_NOT_IN_VIEW;

   return( nRC );
}

// Find the "real" entity name (e.g.  Ctrl instead of CtrlCtrl).
zOPER_EXPORT zSHORT OPERATION
GetRealEntityName( zVIEW v, zPCHAR pchEntityName )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWENTITY lpParent;
   zCHAR szEntityName[ zTAG_LTH ];

   strcpy_s( szEntityName, zsizeof( szEntityName ), pchEntityName );
   lpViewEntity = (LPVIEWENTITY) MiGetViewEntityForView( v, pchEntityName );
   if ( lpViewEntity->bRecursiveSt )
   {
      for ( lpParent = (LPVIEWENTITY) zGETPTR( lpViewEntity->hParent );
            lpParent && lpParent->bRecursivePar == 0;
            lpParent = (LPVIEWENTITY) zGETPTR( lpParent->hParent ) )
      {
         // nothing to do
      }

      if ( lpParent )
      {
         strcpy_s( pchEntityName, zTAG_LTH, lpParent->szName );
      }
      else
         return( -1 );
   }

   return( 0 );
}

//./ ADD NAME=ListSubtasks
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  ListSubtasks
//
//  PURPOSE:    Used to list subtasks.
//
//  PARAMETERS: none
//
//  RETURNS:    0 - on cancel
//              1 - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG OPERATION
ListSubtasks( zVIEW vSubtask )
{
   ZSubtaskList dlg( vSubtask );
   if ( dlg.DoModal( ) == IDCANCEL )
      return( 0 );

   return( 1 );
}

//./ ADD NAME=GetNextSubtaskWithinParent
/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  GetNextSubtaskWithinParent
//
//  PURPOSE:    Used to get a list of subtasks.  If the parent subtask
//              is not specified, all subtasks will be returned.
//
//  PARAMETERS: vSubtaskParent - view to parent window.
//              vSubtaskLast   - view to last window returned by
//                               this operation.
//              lFlag - 1 ==> use parent subtask
//
//  RETURNS:    0  - at end of list
//             otw - view to subtask
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zVIEW OPERATION
GetNextSubtaskWithinParent( zVIEW vSubtaskParent,
                            zVIEW vSubtaskLast,
                            zLONG lFlag )
{
   ASSERT( lFlag == 0 );
   ZSubtask *pZSubtaskParent = 0;
// ZSubtask *pZSubtaskLast = 0;
   ZSubtask *pZSubtask;

// if ( (vSubtaskParent == 0 ||
//       GetWindowAndCtrl( &pZSubtaskParent, 0, vSubtaskParent, 0 ) == 0) &&
//      (vSubtaskLast == 0 ||
//       GetWindowAndCtrl( &pZSubtaskLast, 0, vSubtaskLast, 0 ) == 0) )
   {
      zVIEW vDialog;
      zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      zLONG lPosSubtaskParent = 0;
      zLONG lPosSubtaskLast = 0;
      zLONG lPos;

      // First locate parent and last positions.
      for ( lPos = 1; lPos <= lItemCnt; lPos++ )
      {
         if ( g_pSubtaskList[ lPos ].pZSubtask )
         {
            vDialog = (zVIEW)
                  SysGetPointerFromHandle( g_pSubtaskList[ lPos ].pvDialog );

            if ( lPosSubtaskParent == 0 &&
                 vSubtaskParent && vDialog == vSubtaskParent )
            {
               lPosSubtaskParent = lPos;
               pZSubtaskParent = g_pSubtaskList[ lPos ].pZSubtask;
            }

            if ( lPosSubtaskLast == 0 &&
                 vSubtaskLast && vDialog == vSubtaskLast )
            {
               lPosSubtaskLast = lPos;
            // pZSubtaskLast = g_pSubtaskList[ lPos ].pZSubtask;
            }
         }
      }

      // Find the next subtask that fits the requirements.
      if ( pZSubtaskParent &&
           (vSubtaskParent == 0 || lPosSubtaskParent > 0) &&
           (vSubtaskLast == 0 || lPosSubtaskLast > 0) )
      {
         for ( lPos = lPosSubtaskLast + 1; lPos <= lItemCnt; lPos++ )
         {
            if ( g_pSubtaskList[ lPos ].pZSubtask &&
                 g_pSubtaskList[ lPos ].lTID == pZSubtaskParent->m_lTID &&
                 (g_pSubtaskList[ lPos ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
            {
               if ( (lFlag & 0x00000001) == 0 )
               {
                  return( (zVIEW) SysGetPointerFromHandle( g_pSubtaskList[ lPos ].pvDialog ) );
               }
               else
               {
                  pZSubtask = g_pSubtaskList[ lPos ].pZSubtask;
                  while ( pZSubtask )
                  {
                     if ( pZSubtask->m_pZParent == pZSubtaskParent )
                     {
                        return( (zVIEW) SysGetPointerFromHandle( g_pSubtaskList[ lPos ].pvDialog ) );
                     }

                     pZSubtask = pZSubtask->m_pZParent;
                  }
               }
            }
         }
      }
   }

   return( 0 );
}

// #define DEBUG_TASK
#if 1 // defined DEBUG_SUBTASK || defined DEBUG_LISTWND || defined DEBUG_TASK
void
ListWindowsAndControls( ZSubtask *pZSubtask,
                        zPCHAR   lpLabel,
                        zLONG    lItem )
{
   zCHAR  szListLine[ 32 ];
   zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   zLONG lTID;
   HWND  hWnd;
   zCHAR  szFlag[ 2 ];

   if ( lItem )
   {
      lItemCnt = lItem;
      szFlag[ 0 ] = '*';
      szFlag[ 1 ] = 0;
   }
   else
      szFlag[ 0 ] = 0;

   if ( lpLabel[ 0 ] )
   {
      TraceLine( "ListWindowsAndControls  %s   ItemCount: %d   Subtask: 0x%08x  IdNbr: %d",
                 lpLabel, lItemCnt, pZSubtask, pZSubtask ? pZSubtask->m_ulSubtaskIdNbr : 0 );
   }

// TraceLineI( "ItemCount = ", lItemCnt ); // ZSubtask::GetSubtaskCount( lTID ) );
   if ( pZSubtask )
   {
      lTID = pZSubtask->m_lTID;
      if ( lItem == 0 )
         TraceLine( "ListWindows Task Id = 0x%08x", lTID );
   }
   else
   {
      lTID = SysGetTaskFromView( GetDefaultViewForActiveTask( ) );
      if ( lItem == 0 )
         TraceLine( "ListWindows (DEFAULT VIEW) Task Id = 0x%08x", lTID );
   }

   while ( lItemCnt )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask )
      {
         if ( mIs_hWnd( g_pSubtaskList[ lItemCnt ].hWnd ) )
         {
            if ( mIs_hWndVisible( g_pSubtaskList[ lItemCnt ].hWnd ) )
            {
               if ( mIs_hWndEnabled( g_pSubtaskList[ lItemCnt ].hWnd ) )
                  strcpy_s( szListLine, zsizeof( szListLine ), "Enabled" );
               else
                  strcpy_s( szListLine, zsizeof( szListLine ), "Disabled" );
            }
            else
               strcpy_s( szListLine, zsizeof( szListLine ), "Invisible" );
         }
         else
            strcpy_s( szListLine, zsizeof( szListLine ), "Invalid" );

         if ( g_pSubtaskList[ lItemCnt ].lTID == lTID )
         {
            if ( g_pSubtaskList[ lItemCnt ].hWnd )
               hWnd = g_pSubtaskList[ lItemCnt ].hWnd;
            else
            if ( g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZFWnd )
               hWnd = g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZFWnd->m_hWnd;
            else
               hWnd = 0;

            if ( g_pSubtaskList[ lItemCnt ].pZSubtask->m_pzsWndTag )
            {
               TraceLine( "%d.%d Dlg.Wnd %s.%s   %sTask: 0x%08x   Subtask: 0x%08x  Wnd: 0x%08x  TrueWnd: 0x%08x [%s]",
                          lItemCnt, g_pSubtaskList[ lItemCnt ].pZSubtask->m_ulSubtaskIdNbr,
                          *(g_pSubtaskList[ lItemCnt ].pZSubtask->m_pzsDlgTag),
                          *(g_pSubtaskList[ lItemCnt ].pZSubtask->m_pzsWndTag), szFlag,
                          g_pSubtaskList[ lItemCnt ].pZSubtask->m_lTID,
                          g_pSubtaskList[ lItemCnt ].pZSubtask,
                          g_pSubtaskList[ lItemCnt ].hWnd, hWnd, szListLine );
            }
            else
            {
               TraceLine( "%d.%d DlgWnd: %s.%s   %sTask: 0x%08x   Subtask: 0x%08x   Wnd: 0x%08x   TrueWnd: 0x%08x [%s]",
                          lItemCnt, g_pSubtaskList[ lItemCnt ].pZSubtask->m_ulSubtaskIdNbr,
                          g_pSubtaskList[ lItemCnt ].DlgN,
                          g_pSubtaskList[ lItemCnt ].WndN, szFlag,
                          g_pSubtaskList[ lItemCnt ].pZSubtask->m_lTID,
                          g_pSubtaskList[ lItemCnt ].pZSubtask,
                          g_pSubtaskList[ lItemCnt ].hWnd, hWnd, szListLine );
            }
         }
         else
         if ( mIs_hWnd( g_pSubtaskList[ lItemCnt ].hWnd ) )
         {
            ::SendMessage( g_pSubtaskList[ lItemCnt ].hWnd,
                           zZEIDON_SET_WINDOW_STATE,
                           zWINDOW_STATUS_LAST, lItemCnt );
         }
         else
         {
            TraceLine( "%d.? Dlg.Wnd: %s.%s   %sTask: 0x%08x   Subtask: 0x%08x  Wnd: 0x%08x  TrueWnd: ? [%s]",
                       lItemCnt, g_pSubtaskList[ lItemCnt ].DlgN,
                       g_pSubtaskList[ lItemCnt ].WndN, szFlag,
                       g_pSubtaskList[ lItemCnt ].lTID,
                       g_pSubtaskList[ lItemCnt ].pZSubtask,
                       g_pSubtaskList[ lItemCnt ].hWnd, szListLine );
         }

#if 0 // def DEBUG_LISTWND
         ZSubtask *pZSubtaskCurr = g_pSubtaskList[ lItemCnt ].pZSubtask;
         if ( pZSubtaskCurr->m_pZMIXCtrl )
         {
            TraceLineI( "pZMIXCtrl = ",
                        (zLONG) g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZMIXCtrl );
            g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZMIXCtrl->
                        VisitInorder( (fnVISIT_INORDER) fnOrderedCtrls );
         }
#endif
      }

      if ( lItemCnt == lItem )
         break;

      lItemCnt--;
   }
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: ConstructZeidonPrompt
//
// PURPOSE: This routine creates a dialog box to retrieve a single data item
//       along with two possible check box values.
//
//       This dialog box is a simple dialog box that prompts the user
//       for a single line of text input, as well as two possible check
//
//       Example:
//
//       zCHAR szPatientName[ zTAG_LTH ] = "";
//
//       ZInputDialog( this, "Patient name",
//                     "Enter the patient's name:",
//                     szPatientName,
//                     zsizeof( szPatientName ) );
//
//       In this example, patientName is a text buffer that gets filled with
//       the user's input when the user chooses OK. It should be initialized
//       (perhaps to an empty string) for the default text.
//
//       plFlags is the most complex parameter with the following values:
//         0x00000000 - checkboxes invisible
//         0x00010001 - checkbox 1 visible and initially checked
//         0x00010000 - checkbox 1 visible and initially unchecked
//         0x00020002 - checkbox 2 visible and initially checked
//         0x00020000 - checkbox 2 visible and initially unchecked
//         0x00040004 - checkbox 3 visible and initially checked
//         0x00040000 - checkbox 3 visible and initially unchecked
//         0x00080008 - checkbox 4 visible and initially checked
//         0x00080000 - checkbox 4 visible and initially unchecked
//         0x00100010 - checkbox 5 visible and initially checked
//         0x00100000 - checkbox 5 visible and initially unchecked
//         0x00200020 - checkbox 6 visible and initially checked
//         0x00200000 - checkbox 6 visible and initially unchecked
//         0x01000000 - IDNO button is invisible (leaving IDOK and IDCANCEL)
//       On return, if the 1-bit is on, checkbox1 is checked; if the 2-bit
//       is on, checkbox2 is checked; if the 4-bit is on, checkbox3 is
//       checked; and if the 8-bit is on, checkbox4 is checked.  Note that
//       these flags may be "added".
//
//       cpcCtrlText specifies the title and button texts as:
//         Title:;OK;No;Check1;Check2;Check3;Check4;Check5;Check6
//       It is a positionally specified semi-colon separated list of text
//       values (which if any are empty, default to a somewhat meaningless
//       original value).
//
// RETURNS:  -1 - unable to build window
//            1 - IDOK
//            2 - IDCANCEL
//            3 - IDABORT
//            4 - IDRETRY
//            5 - IDIGNORE
//            6 - IDYES
//            7 - IDNO
//            8 - IDCLOSE
//            9 - IDHELP
//
/////////////////////////////////////////////////////////////////////////////
class ZInputDialog : public CDialog
{
public:
   ZInputDialog( ZSubtask *pZSubtask, zCPCHAR cpcTitle, zCPCHAR cpcPrompt,
                 zPCHAR pchReturnBuffer, zUSHORT uBufferLth,
                 zPLONG plFlags,
                 zCPCHAR cpcCtrlText,  // Title:;Yes;No;Check1;...Check6
                 zCHAR chPassword, UINT uResId );
   virtual ~ZInputDialog( );

// Attributes
   ZSubtask *m_pZSubtask;
   CString  m_csTitle;
   CString  m_csPrompt;
   CString  m_csCtrlText;
   zPCHAR   m_pchReturnBuffer;
   zUSHORT  m_uBufferLth;
   CEdit    m_editReturn;
   CStatic  m_textPrompt;
   CStatic  m_textTitle;
   CButton  m_OK;
   CButton  m_No;
   CButton  m_Cancel;
   CButton  m_check1;
   CButton  m_check2;
   CButton  m_check3;
   CButton  m_check4;
   CButton  m_check5;
   CButton  m_check6;
   zCHAR    m_chPassword;
   zPLONG   m_plFlags;

// Operations
public:

// Overrides
public:
   virtual void OnOK( );
   virtual void OnCancel( );
   virtual BOOL OnInitDialog( );
   virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
   void MoveChildWindow( CWnd& rWnd, zLONG lOffsetX, zLONG lOffsetY );
};

ZInputDialog::ZInputDialog( ZSubtask *pZSubtask,
                            zCPCHAR  cpcTitle,
                            zCPCHAR  cpcPrompt,
                            zPCHAR   pchReturnBuffer,
                            zUSHORT  uBufferLth,
                            zPLONG   plFlags,
                            zCPCHAR  cpcCtrlText,
                            zCHAR    chPassword,
                            UINT     uResId ) :
              CDialog( uResId, pZSubtask->GetSplitterMainSubtask( ) ?  // need to get the correct parent
                               pZSubtask->GetSplitterMainSubtask( )->m_pZFWnd : pZSubtask->m_pZFWnd )
{
   ASSERT( uBufferLth > 0 );

   m_pZSubtask = pZSubtask->GetSplitterMainSubtask( );
   if ( m_pZSubtask == 0 )
      m_pZSubtask = pZSubtask;

   m_csTitle = cpcTitle;
   m_csPrompt = cpcPrompt;
   m_csCtrlText = cpcCtrlText;
   m_pchReturnBuffer = pchReturnBuffer;
   if ( uBufferLth > 0 )
      m_uBufferLth = uBufferLth;
   else
      m_uBufferLth = 0;

   m_chPassword = chPassword;
   m_plFlags = plFlags;
}

ZInputDialog::~ZInputDialog( )
{
}

zLONG
fnGetRequiredClientHeight( CWnd *pWnd, zCPCHAR cpcText )
{
   CClientDC dc( pWnd );
   CSize   size;
   CRect   rect;
   int     nNbrChars;
   zSHORT  nCharCnt = 0;
   zSHORT  nLineCnt = 0;
   zSHORT  nLth = (zSHORT) zstrlen( cpcText );
   pWnd->GetClientRect( rect );
   zLONG   lWidth = rect.Width( );

   CFont *pFontOld = 0;
   CFont *pFont = pWnd->GetFont( );
   if ( pFont )
      pFontOld = dc.SelectObject( pFont );

   while ( nCharCnt < nLth )
   {
      nLineCnt++;
      GetTextExtentExPoint( dc, cpcText + nCharCnt, nLth - nCharCnt,
                            lWidth, &nNbrChars, 0, &size );
      nCharCnt += nNbrChars;
   }

   if ( pFontOld )
      dc.SelectObject( pFontOld );

   return( (size.cy * nLineCnt) + 4 );
}

void
ZInputDialog::MoveChildWindow( CWnd& rWnd, zLONG lOffsetX, zLONG lOffsetY )
{
   CRect rectParent;
   CRect rect;

   GetWindowRect( rectParent );
   rWnd.GetWindowRect( rect );
   rect.top -= rectParent.top;
   rect.left -= rectParent.left;
   rect.bottom -= rectParent.top;
   rect.right -= rectParent.left;

   rect.top += lOffsetY;
   rect.bottom += lOffsetY;
   rect.left += lOffsetX;
   rect.right += lOffsetX;

   // All coordinates for child windows are client coordinates (relative
   // to the upper-left corner of the parent window’s client area).
   rWnd.SetWindowPos( 0, rect.left, rect.top, 0, 0,
                      SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_SHOWWINDOW );
}

BOOL
ZInputDialog::OnInitDialog( )
{
   CRect rect;
   CRect rectChild;
   zLONG lFlags;
   zLONG lEditSize = 0;

   VERIFY( m_editReturn.SubclassDlgItem( IDC_INPUT, this ) );
   VERIFY( m_textPrompt.SubclassDlgItem( IDC_PROMPT, this ) );
   VERIFY( m_check1.SubclassDlgItem( IDC_CHECK1, this ) );
   VERIFY( m_check2.SubclassDlgItem( IDC_CHECK2, this ) );
   VERIFY( m_check3.SubclassDlgItem( IDC_CHECK3, this ) );
   VERIFY( m_check4.SubclassDlgItem( IDC_CHECK4, this ) );
   VERIFY( m_check5.SubclassDlgItem( IDC_CHECK5, this ) );
   VERIFY( m_check6.SubclassDlgItem( IDC_CHECK6, this ) );
   VERIFY( m_textTitle.SubclassDlgItem( IDC_TITLE, this ) );
   VERIFY( m_OK.SubclassDlgItem( IDOK, this ) );
   VERIFY( m_No.SubclassDlgItem( IDNO, this ) );
   VERIFY( m_Cancel.SubclassDlgItem( IDCANCEL, this ) );

   zPCHAR pch = m_csCtrlText.GetBuffer( m_csCtrlText.GetLength( ) );
   zPCHAR pchSemiColon;
   zSHORT nText = 0;

   while ( pch && *pch )
   {
      pchSemiColon = zstrchr( pch, ';' );
      if ( pchSemiColon )
         *pchSemiColon = 0;

      if ( pch && *pch )
      {
         switch ( nText )
         {
            case 0:  // Title
            {
               if ( *pch == ' ' && *(pch + 1) == 0 )
               {
                  m_textTitle.GetWindowRect( rectChild );
                  m_editReturn.GetWindowRect( rect );
                  lEditSize = rect.left - rectChild.left,
                  m_textTitle.ShowWindow( SW_HIDE );
               }
               else
               {
                  m_textTitle.SetWindowText( pch );
               }

               break;
            }

            case 1:  // OK
            {
               m_OK.SetWindowText( pch );
               break;
            }

            case 2:  // No
            {
               m_No.SetWindowText( pch );
               break;
            }

            case 3:  // Check1
            {
               m_check1.SetWindowText( pch );
               break;
            }

            case 4:  // Check2
            {
               m_check2.SetWindowText( pch );
               break;
            }

            case 5:  // Check3
            {
               m_check3.SetWindowText( pch );
               break;
            }

            case 6:  // Check4
            {
               m_check4.SetWindowText( pch );
               break;
            }

            case 7:  // Check5
            {
               m_check5.SetWindowText( pch );
               break;
            }

            case 8:  // Check6
            {
               m_check6.SetWindowText( pch );
               break;
            }

            default:
               break;
         }
      }

      nText++;
      if ( pchSemiColon )
      {
         *pchSemiColon = ';';
         pch = pchSemiColon + 1;
      }
      else
         pch = 0;
   }

   m_csCtrlText.ReleaseBuffer( -1 );

   // Does not include room for null terminator.
   m_editReturn.SetLimitText( m_uBufferLth - 1 );
   SetWindowText( m_csTitle );

#if 0
   zCPCHAR cpc = "This is the dawning of a new day in which there is"
                 " a great deal of hope for all of mankind to be sur"
                 "e that we do not ever get behind the wheel of fort"
                 "une again in the next millenium for this is 200 ch";
   zLONG lClientHeight = fnGetRequiredClientHeight( &m_textPrompt,
                                                    cpc );
#else
   zLONG lClientHeight = fnGetRequiredClientHeight( &m_textPrompt,
                                                    m_csPrompt );
#if 0
   if ( (pch = zstrchr( m_csPrompt, '\n' )) != 0 )
   {
      // There are linefeeds in the prompt text.  Count them and
      // adjust the ClientHeight accordingly.
      zSHORT nCnt = 1;
      while ( pch )
      {
         nCnt++;
         pch = zstrchr( pch + 1, '\n' );
      }

      lClientHeight *= nCnt;
   }
#endif

#endif

   // Get number of pixels separating vertical controls.
   m_Cancel.GetWindowRect( rect );
   m_editReturn.GetWindowRect( rectChild );
   zLONG lSep = rect.top - rectChild.bottom;

   // Get top of control vertically adjacent to "prompt" ctrl.
   m_check1.GetWindowRect( rectChild );
   m_textPrompt.GetWindowRect( rect );
   zLONG lOrigSep = rectChild.top - rect.bottom;  // separation between prompt and check1
   zLONG lCheckRowHeight = rectChild.bottom - rect.bottom;  // check box row height
   zLONG lOffset = lClientHeight - (rect.bottom - rect.top);  // offset = new height - old height
   int   nCapHeight = GetSystemMetrics( SM_CYCAPTION );
   lOffset -= nCapHeight;

   rect.bottom = rect.top + lClientHeight;  // new height of "prompt" ctrl
   m_textPrompt.SetWindowPos( 0, 0, 0, rect.Width( ), rect.Height( ),
                              SWP_NOMOVE | SWP_NOOWNERZORDER );
#if 0
   m_textPrompt.SetWindowText( cpc );
#else
   m_textPrompt.SetWindowText( m_csPrompt );
#endif

   if ( m_plFlags )
      lFlags = *m_plFlags;
   else
      lFlags = 0;

   MoveChildWindow( m_check1, 0, lOffset );
   MoveChildWindow( m_check2, 0, lOffset );
   if ( (lFlags & 0x00010000) == 0 )
      lOffset -= lCheckRowHeight;

   MoveChildWindow( m_check3, 0, lOffset );
   MoveChildWindow( m_check4, 0, lOffset );

   if ( (lFlags & 0x00040000) == 0 )
      lOffset -= lCheckRowHeight;

   MoveChildWindow( m_check5, 0, lOffset );
   MoveChildWindow( m_check6, 0, lOffset );

   if ( (lFlags & 0x00100000) == 0 )
      lOffset -= lCheckRowHeight;

   MoveChildWindow( m_editReturn, 0, lOffset );
   if ( lEditSize == 0 )
      MoveChildWindow( m_textTitle, 0, lOffset );

   MoveChildWindow( m_OK, 0, lOffset );
   MoveChildWindow( m_No, 0, lOffset );
   MoveChildWindow( m_Cancel, 0, lOffset );

   if ( m_chPassword )
      m_editReturn.SetPasswordChar( m_chPassword );

   m_editReturn.SetWindowText( m_pchReturnBuffer );
   m_check1.ShowWindow( SW_HIDE );
   m_check2.ShowWindow( SW_HIDE );
   m_check3.ShowWindow( SW_HIDE );
   m_check4.ShowWindow( SW_HIDE );
   m_check5.ShowWindow( SW_HIDE );
   m_check6.ShowWindow( SW_HIDE );
   if ( lFlags & 0x00010000 )
   {
      m_check1.ShowWindow( SW_SHOW );
      if ( lFlags & 0x00000001 )
         m_check1.SetCheck( TRUE );

      if ( lFlags & 0x00020000 )
      {
         m_check2.ShowWindow( SW_SHOW );
         if ( lFlags & 0x00000002 )
            m_check2.SetCheck( TRUE );
      }

      if ( lFlags & 0x00040000 )
      {
         m_check3.ShowWindow( SW_SHOW );
         if ( lFlags & 0x00000004 )
            m_check3.SetCheck( TRUE );
      }

      if ( lFlags & 0x00080000 )
      {
         m_check4.ShowWindow( SW_SHOW );
         if ( lFlags & 0x00000008 )
            m_check4.SetCheck( TRUE );
      }

      if ( lFlags & 0x00100000 )
      {
         m_check5.ShowWindow( SW_SHOW );
         if ( lFlags & 0x00000010 )
            m_check5.SetCheck( TRUE );
      }

      if ( lFlags & 0x00200000 )
      {
         m_check6.ShowWindow( SW_SHOW );
         if ( lFlags & 0x00000020 )
            m_check6.SetCheck( TRUE );
      }
   }

#if 0
   if ( (lFlags & 0x00040000) == 0 )
   {
      CRect rectCheck;
      CRect rect;

      if ( (lFlags & 0x00010000) == 0 )
      {
         m_textPrompt.GetWindowRect( rectCheck );
         m_check1.ShowWindow( SW_HIDE );
         m_check2.ShowWindow( SW_HIDE );
      }
      else
      {
         m_check1.GetWindowRect( rectCheck );
      }

      m_check3.ShowWindow( SW_HIDE );
      m_check4.ShowWindow( SW_HIDE );
      m_check5.ShowWindow( SW_HIDE );
      m_check6.ShowWindow( SW_HIDE );
      rectCheck.top = rectCheck.bottom + lSep;

      m_editReturn.GetWindowRect( rect );
      lOffset = rect.top - rectCheck.top;
      if ( lEditSize == 0 )
      {
         MoveChildWindow( m_editReturn, 0, lOffset );
         MoveChildWindow( m_textTitle, 0, lOffset );
      }
      else
      {
         m_editReturn.SetWindowPos( 0, 0, 0,
                                    rect.Width( ) + lEditSize,
                                    rect.Height( ),
                                    SWP_NOMOVE |
                                    SWP_NOZORDER | SWP_NOOWNERZORDER |
                                    SWP_NOREDRAW | SWP_SHOWWINDOW );
         MoveChildWindow( m_editReturn, lEditSize, lOffset );
      }

      m_OK.GetWindowRect( rect );
      lOffset = rect.top - rectCheck.top;
      MoveChildWindow( m_OK, 0, lOffset );
      MoveChildWindow( m_No, 0, lOffset );
      MoveChildWindow( m_Cancel, 0, lOffset );
   }
#endif

   if ( lFlags & 0x01000000 )
   {
      CRect rectNo;

      m_No.GetWindowRect( rectNo );
      m_No.ShowWindow( SW_HIDE );

      lOffset = rectNo.Width( ) / 2;
      MoveChildWindow( m_OK, +lOffset, 0 );
      MoveChildWindow( m_Cancel, -lOffset, 0 );
   }

   GetWindowRect( rect );
   m_Cancel.GetWindowRect( rectChild );
   rect.bottom = rectChild.bottom + lSep;
   SetWindowPos( 0, 0, 0, rect.Width( ), rect.Height( ),
                 SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_SHOWWINDOW );

   m_editReturn.SetFocus( );
   return( FALSE ); // return TRUE unless you set the focus to a control
}

BOOL
ZInputDialog::OnCommand( WPARAM wParam, LPARAM lParam )
{
   UINT uID = LOWORD( wParam );
   UINT uNotifyCode = HIWORD( wParam );
   HWND hWndCtrl = (HWND) lParam;

   if ( hWndCtrl && uID >= IDOK && uID <= IDHELP && uID != IDCANCEL )
   {
      // Includes room for null terminator.
      m_editReturn.GetWindowText( m_pchReturnBuffer, m_uBufferLth );
      if ( m_plFlags )
      {
         *m_plFlags &= 0xFFFFFF00;
         if ( m_check1.GetCheck( ) )
            *m_plFlags |= 0x00000001;

         if ( m_check2.GetCheck( ) )
            *m_plFlags |= 0x00000002;

         if ( m_check3.GetCheck( ) )
            *m_plFlags |= 0x00000004;

         if ( m_check4.GetCheck( ) )
            *m_plFlags |= 0x00000008;

         if ( m_check5.GetCheck( ) )
            *m_plFlags |= 0x00000010;

         if ( m_check6.GetCheck( ) )
            *m_plFlags |= 0x00000020;
      }

      EndDialog( uID );
      return( TRUE );   // we processed the command
   }

   return( CDialog::OnCommand( wParam, lParam ) );
}

void
ZInputDialog::OnOK( )
{
   MessageBox( "OK Command", "Zeidon Input Dialog Error" );
// m_editReturn.GetWindowText( m_pchReturnBuffer, m_uBufferLth );
   CDialog::OnOK( );
}

void
ZInputDialog::OnCancel( )
{
   CDialog::OnCancel( );
}

zOPER_EXPORT zSHORT OPERATION
ConstructZeidonPrompt( zVIEW    vSubtask,
                       zCPCHAR  cpcTitle,
                       zCPCHAR  cpcPrompt,
                       zPCHAR   pchReturnBuffer,
                       zSHORT   nBufferLth,
                       zPLONG   plFlags,
                       zCPCHAR  cpcCtrlText,  // Title:;Yes;No;Check1-Check4
                       zSHORT   chPassword,   // 0 if not a password
                       zLONG    lResId )      // 0 for default
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( chPassword == 1 )
         chPassword = 0;

      if ( lResId == 0 )
         lResId = IDD_INPUTDIALOG;

      ZInputDialog dlg( pZSubtask, cpcTitle, cpcPrompt,
                        pchReturnBuffer, nBufferLth,
                        plFlags, cpcCtrlText,
                        (char) chPassword, (UINT) lResId );
      return( dlg.DoModal( ) );
   }
   else
   {
      TraceLineS( "Construct Prompt failed: ", cpcPrompt );
      return( IDCANCEL );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: EnableZWindowsForApplication
//
// PURPOSE: This routine enables/disables all windows for the current
//          application.  This requires us to get the application for
//          each subtask.
//
// RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
EnableZWindowsForApplication( ZSubtask *pZSubtask,
                              zBOOL    bEnable )
{
   zLONG  lItemCnt;

#ifdef DEBUG_ALL

   zULONG ulInterp = 0;
// LPAPP  pApp;

// SfGetApplicationForSubtask( &pApp, pZSubtask->m_vDialog );

// lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
// while ( lItemCnt )
// {
//    if ( g_pSubtaskList[ lItemCnt ].lTID == pZSubtask->m_lTID &&
//         g_pSubtaskList[ lItemCnt ].pZSubtask == pZSubtask )
//    {
//       ulInterp = (g_pSubtaskList[ lItemCnt ].ulSubtaskState &
//                                             zSUBTASK_STATE_INTERP_DEBUG);
//       break;
//    }
//
//    lItemCnt--;
// }

   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, zsizeof( szMsg ), "EnableZWindowsForApplication bEnable: %s ulInterp: ",
             bEnable ? "TRUE" : "FALSE" );
   TraceLineX( szMsg, ulInterp );
#endif

   // Run through the list of windows for this task to find any that are
   // within this application ... enable/disable those that are.
   lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   while ( lItemCnt )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask )
      {
         if ( (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 &&
              g_pSubtaskList[ lItemCnt ].lTID == pZSubtask->m_lTID )
              // &&
              // (ulInterp & g_pSubtaskList[ lItemCnt ].ulSubtaskState) == ulInterp )
         {
            if ( bEnable )
            {
               if ( g_pSubtaskList[ lItemCnt ].pZSubtask->m_nInvisible < 0 )
                  g_pSubtaskList[ lItemCnt ].pZSubtask->m_nInvisible++;

               if ( g_pSubtaskList[ lItemCnt ].pZSubtask->m_nInvisible >= 0 )
                  g_pSubtaskList[ lItemCnt ].pZSubtask->EnableWindow( TRUE );
            }
            else
            {
               if ( g_pSubtaskList[ lItemCnt ].pZSubtask->m_nInvisible <= 0 )
                  g_pSubtaskList[ lItemCnt ].pZSubtask->m_nInvisible--;
               else
                  g_pSubtaskList[ lItemCnt ].pZSubtask->m_nInvisible = -1;

               g_pSubtaskList[ lItemCnt ].pZSubtask->EnableWindow( FALSE );
            }

#ifdef DEBUG_ALL
            zCHAR szMsg[ 256 ];

            sprintf_s( szMsg, zsizeof( szMsg ), "EnableZWindowsForApplication Dlg: %s Wnd: %s nInvisible: ",
                      *(g_pSubtaskList[ lItemCnt ].pZSubtask->m_pzsDlgTag),
                      *(g_pSubtaskList[ lItemCnt ].pZSubtask->m_pzsWndTag) );
            TraceLineI( szMsg, g_pSubtaskList[ lItemCnt ].pZSubtask->m_nInvisible );
#endif
         }

#if 0  // what am I trying to do here???  // InterpDebug ... DKS 2000.12.14
         else
         {
            LPAPP pAppCurr;

            SfGetApplicationForSubtask( &pAppCurr, (zVIEW)
              SysGetPointerFromHandle( g_pSubtaskList[ lItemCnt ].pvDialog ) );
            if ( pAppCurr == pApp )
               g_pSubtaskList[ lItemCnt ].pZSubtask->EnableWindow( bEnable );
         }
#endif
      }

      lItemCnt--;
   }

   return( 0 );
}

HICON
GetIconForWindow( ZSubtask *pZSubtask )
{
   HICON hIcon = 0;
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );

   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && pZSubtask->m_hLibrary )
   {
      hIcon = ::LoadIcon( (HINSTANCE) pZSubtask->m_hLibrary->hLibrary, *(pZSubtask->m_pzsDlgTag) );
   }

   if ( hIcon == 0 )
   {
      // if no icon exists in the application DLL, load the default
      // application icon from ZDR.
//???      hIcon = ::LoadIcon( (HINSTANCE) pZSubtask->m_pZTask->m_hInstance,
//???                          MAKEINTRESOURCE( IDI_SDIAPPLICATION ) );
//    TraceLineI( "Driver icon set for ZSubtask ", (zLONG) pZSubtask );
   }
// else
//    TraceLineS( "App icon set for ZSubtask ", *(Subtask->m_pzsDlgTag) );

   return( hIcon );
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: InvalidateChildRect
//
// PURPOSE: This routine invalidates a child window
//          bErase is sent on to the InvalidateRect API
//
/////////////////////////////////////////////////////////////////////////////
void
InvalidateChildRect( ZMapAct *pzma,
                     zBOOL   bErase )
{
   CRect  rect;

   rect.left = pzma->Attr.X;
   rect.top = pzma->Attr.Y;
   rect.right = rect.left + pzma->Attr.W;
   rect.bottom = rect.top + pzma->Attr.H;
   if ( pzma->m_pZSubtask &&
        pzma->m_pZSubtask->m_pZFWnd &&
        mIs_hWnd( pzma->m_pZSubtask->m_pZFWnd->m_hWnd ) )
   {
      pzma->m_pZSubtask->m_pZFWnd->InvalidateRect( rect, bErase );
   }
}

#if 0
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

fnIsAccelerator  msg.wParam ==========> 0x57010006
fnIsAccelerator  msg.lParam ==========> 0x2a8500
fnIsAccelerator  pCharMsg->fs ========> 0x6
fnIsAccelerator  pCharMsg->cRepeat ===> 0x1
fnIsAccelerator  pCharMsg->scancode ==> 0x57
fnIsAccelerator  pCharMsg->chr =======> 0x8500
fnIsAccelerator  pCharMsg->vkey ======> 0x2a
(drvr[5800264.ACCOUNT.account]) Action=>F11  Event=>Hotkey
fnIsAccelerator  msg.wParam ==========> 0x37010005
fnIsAccelerator  msg.lParam ==========> 0x2a
fnIsAccelerator  pCharMsg->fs ========> 0x5
fnIsAccelerator  pCharMsg->cRepeat ===> 0x1
fnIsAccelerator  pCharMsg->scancode ==> 0x37
fnIsAccelerator  pCharMsg->chr =======> 0x2a
fnIsAccelerator  pCharMsg->vkey ======> 0x0
(drvr[5800264.ACCOUNT.account]) Action=>Multiply Event=>Hotkey
fnIsAccelerator  msg.wParam ==========> 0x58011006
fnIsAccelerator  msg.lParam ==========> 0x2b8600
fnIsAccelerator  pCharMsg->fs ========> 0x1006
fnIsAccelerator  pCharMsg->cRepeat ===> 0x1
fnIsAccelerator  pCharMsg->scancode ==> 0x58
fnIsAccelerator  pCharMsg->chr =======> 0x8600
fnIsAccelerator  pCharMsg->vkey ======> 0x2b
(drvr[5800264.ACCOUNT.account]) Action=>F12  Event=>Hotkey
fnIsAccelerator  msg.wParam ==========> 0x4e010005
fnIsAccelerator  msg.lParam ==========> 0x2b
fnIsAccelerator  pCharMsg->fs ========> 0x5
fnIsAccelerator  pCharMsg->cRepeat ===> 0x1
fnIsAccelerator  pCharMsg->scancode ==> 0x4e
fnIsAccelerator  pCharMsg->chr =======> 0x2b
fnIsAccelerator  pCharMsg->vkey ======> 0x0
(drvr[5800264.ACCOUNT.account]) Action=>Plus Event=>Hotkey
fnIsAccelerator  msg.wParam ==========> 0x5c011005
fnIsAccelerator  msg.lParam ==========> 0x2f
fnIsAccelerator  pCharMsg->fs ========> 0x1005
fnIsAccelerator  pCharMsg->cRepeat ===> 0x1
fnIsAccelerator  pCharMsg->scancode ==> 0x5c
fnIsAccelerator  pCharMsg->chr =======> 0x2f
fnIsAccelerator  pCharMsg->vkey ======> 0x0
(drvr[5800264.ACCOUNT.account]) Action=>Divide Event=>Hotkey
fnIsAccelerator  msg.wParam ==========> 0x4a011005
fnIsAccelerator  msg.lParam ==========> 0x2d
fnIsAccelerator  pCharMsg->fs ========> 0x1005
fnIsAccelerator  pCharMsg->cRepeat ===> 0x1
fnIsAccelerator  pCharMsg->scancode ==> 0x4a
fnIsAccelerator  pCharMsg->chr =======> 0x2d
fnIsAccelerator  pCharMsg->vkey ======> 0x0
(drvr[5800264.ACCOUNT.account]) Action=>Minus  Event=>Hotkey
fnIsAccelerator  msg.wParam ==========> 0x5a011007
fnIsAccelerator  msg.lParam ==========> 0x1e000d
fnIsAccelerator  pCharMsg->fs ========> 0x1007
fnIsAccelerator  pCharMsg->cRepeat ===> 0x1
fnIsAccelerator  pCharMsg->scancode ==> 0x5a
fnIsAccelerator  pCharMsg->chr =======> 0xd
fnIsAccelerator  pCharMsg->vkey ======> 0x1e
                                        <BigEnter> Event
fnIsAccelerator  msg.wParam ==========> 0x1c010007
fnIsAccelerator  msg.lParam ==========> 0x8000d
fnIsAccelerator  pCharMsg->fs ========> 0x7
fnIsAccelerator  pCharMsg->cRepeat ===> 0x1
fnIsAccelerator  pCharMsg->scancode ==> 0x1c
fnIsAccelerator  pCharMsg->chr =======> 0xd
fnIsAccelerator  pCharMsg->vkey ======> 0x8
                                        <Enter> Event
(drvr[5800264.ACCOUNT.account]) Action=>Cancel  Event=>CtrlEvent  (no code)  WAB=>71
(drvr[5800264.ACCOUNT..ccount]) Action=>Cleanup  Event=>OnCleanup  Operation=>Cleanup (code only)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

WM_CHAR   wParam     lParam   Posted/Sent  Target Window       // from Spy
         ========   ========  ===========  =============
         1E010005   00000061       P         8000014D
         1E010144   00001E61       P         8000014D
         1E010144   00001E61       S         8000012F
         0F011007   00060009       P         8000014D
         0F011007   00060009       S         8000012F
         0F011146   00060F09       P         8000012F
         0F011146   00060F09       S         8000012A

wParam   1E-ScanCode  01-RepeatCnt  0005-fsFlags: 0005=KC_CHAR | KC_SCANCODE
lParam   0000-CharCode  00-?  61-VirtualKey

DlgClient PreTranslateMessage ==========> pMsg = 0x7a
DlgClient PreTranslate pMsg->message 0x7a
DlgClient PreTranslate pMsg->wParam 0x1e010005
DlgClient PreTranslate pMsg->lParam 0x61
DlgClient PreTranslate fs 0x7a
DlgClient PreTranslate vkey 0x1e01
DlgClient PreTranslate chr 0x5
DlgClient PreTranslate scan 0x0
DlgClient PreTranslateMessage ==========> pMsg = 0x7a
DlgClient PreTranslate pMsg->message 0x7a
DlgClient PreTranslate pMsg->wParam 0x1e010144
DlgClient PreTranslate pMsg->lParam 0x1e61
DlgClient PreTranslate fs 0x7a
DlgClient PreTranslate vkey 0x1e01
DlgClient PreTranslate chr 0x144
DlgClient PreTranslate scan 0x0


DlgClient PreTranslateMessage ==========> pMsg = 0x7a
DlgClient PreTranslate pMsg->message 0x7a
DlgClient PreTranslate pMsg->wParam 0xf011007
DlgClient PreTranslate pMsg->lParam 0x60009
DlgClient PreTranslate fs 0x7a
DlgClient PreTranslate vkey 0xf01
DlgClient PreTranslate chr 0x1007
DlgClient PreTranslate scan 0x0
DlgClient PreTranslateMessage ==========> pMsg = 0x7a
DlgClient PreTranslate pMsg->message 0x7a
DlgClient PreTranslate pMsg->wParam 0xf011146
DlgClient PreTranslate pMsg->lParam 0x60f09
DlgClient PreTranslate fs 0x7a
DlgClient PreTranslate vkey 0xf01
DlgClient PreTranslate chr 0x1146
DlgClient PreTranslate scan 0x0


WM_CHAR  wParam:  USHORT   fsFlags
                  UCHAR    ucRepeat
                  UCHAR    ucScanCode

         lParam:  USHORT   usCharacterCode
                  USHORT   usVK  - LOBYTE contains the vk value
                                 - HIBYTE always set to 0 by the standard
                                   translate table

// WM_CHAR fs field bits
// fsFlags (USHORT) ... Keyboard control codes:
#define KC_NONE         0x0000   // Reserved
#define KC_CHAR         0x0001   // Indicates that usCharacterCode value
                                 // is valid.

#define KC_VIRTUALKEY   0x0002   // Indicates that usVK is valid.
// Normally usVK should be given precedence when processing the message.

#define KC_SCANCODE     0x0004    // Indicates that ucScanCode is valid.
// Generally, this is set in all WM_CHAR messages generated from actual
// operator input.  However, if the message has been generated by an
// application that has issued the WinSetHook function to filter keystrokes,
// or posted to the application queue, this may not be set.

#define KC_SHIFT        0x0008   // The SHIFT state is active when key press
                                 // or release occurred.

#define KC_CTRL         0x0010   // The CTRL state was active when key press
                                 // or release occurred.

#define KC_ALT          0x0020   // The ALT state is active when key press or
                                 // release occurred.

#define KC_KEYUP        0x0040   // The event is a key-up transition;
                                 // otherwise it is a down transition.

#define KC_PREVDOWN     0x0080   // The key has been previously down;
                                 // otherwise it has been previously up.
#define KC_LONEKEY      0x0100   // Indicates if the key is pressed and
                                 // released without any other keys being
                                 // pressed or released between the time the
                                 // key goes down and up.

#define KC_DEADKEY      0x0200   // The character code is a dead key. The
                                 // application is responsible for displaying
                                 // the glyph for the dead key without
                                 // advancing the cursor.

#define KC_COMPOSITE    0x0400   // The character code is formed by combining
                                 // the current key with the previous dead key.

#define KC_INVALIDCOMP  0x0800
// The character code is not a valid combination with the preceding dead key.
// The  application is responsible for advancing the cursor past the dead-key
// glyph and then, if the current character is not a space, sounding the alarm
// and displaying the new character code.

#define KC_TOGGLE       0x1000
#define KC_INVALIDCHAR  0x2000
#define KC_DBCSRSRVD1   0x4000
#define KC_DBCSRSRVD2   0x8000

// ucRepeat (UCHAR) ... Repeat count.

// ucScanCode (UCHAR) ... Hardware scan code.
// A keyboard-generated value that identifies the keyboard event. This is the
// raw scan code, not the translated scan code.

// usCharacterCode (USHORT) ... Character code.
// The character value translation of the keyboard event resulting from the
// current code page that would apply if the CTRL or ALT keys were not
// depressed.

// usVK (USHORT) ... Virtual key codes.
// A virtual key value translation of the keyboard event resulting from the
// virtual key code table. The low-order byte contains the vk value, and the
// high-order byte is always set to zero by the standard translate table.
//
//  0 - This value applies if fsFlags does not contain KC_VIRTUALKEY.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#endif

/////////////////////////////////////////////////////////////////////////////
//
// Determine if the hWndChild window is a direct descendent of hWndParent.
// This will be true if:
//  - hWndParent == hWndChild ... or
//  - hWndChild has a WS_CHILD style and every parent of hWndChild
//    (except for hWndParent) also has the WS_CHILD style.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
fnIsChild( HWND hWndParent, HWND hWndChild )
{
   zLONG lStyle;

   if ( hWndChild == hWndParent )
      return( TRUE );

   if ( ::IsChild( hWndParent, hWndChild ) == FALSE )
      return( FALSE );

   while ( hWndChild && hWndChild != hWndParent )
   {
      lStyle = ::GetWindowLong( hWndChild, GWL_STYLE );
      if ( (lStyle & WS_CHILD) == 0 )
         return( FALSE );

      hWndChild = ::GetParent( hWndChild );
   }

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: fnIsAccelerator
//
// PURPOSE: This routine determinses if a keyboard message is an accelerator
//
// returns TRUE  - if it is a Zeidon accelerator
//         FALSE - otherwise
//
/////////////////////////////////////////////////////////////////////////////
zBOOL OPERATION
fnIsAccelerator( ZSubtask *pZSubtask,
                 MSG      *pMsg )
{
#if 0
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN ||
        pMsg->message == WM_EXITSIZEMOVE )
   {
      if ( pMsg->wParam == VK_ESCAPE )
      {
         TraceLineS( "fnIsAccelerator: ", "VK_ESCAPE" );
      }
   }
#endif

#if 0
   if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB )
   {
      if ( fnValidTab( pZSubtask, (GetKeyState( VK_SHIFT ) < 0) ) )
         return( FALSE );
      else
         return( TRUE );
   }
#endif

   zACCEL_DEF ad;

   ad.uVK = pMsg->wParam;

   if ( ad.uVK == VK_SHIFT || ad.uVK == VK_MENU || ad.uVK == VK_CONTROL )
      return( FALSE );

   ad.fAlt = (unsigned) (GetKeyState( VK_MENU ) < 0);
   ad.fCtrl = (unsigned) (GetKeyState( VK_CONTROL ) < 0);
   ad.fShift = (unsigned) (GetKeyState( VK_SHIFT ) < 0);

#if 0
   if ( ad.fAlt && ad.fCtrl )
      TraceLineX( "PreTranslateMessage Alt + Ctrl + VK ", ad.uVK );
   else
   if ( ad.fCtrl )
      TraceLineX( "PreTranslateMessage Ctrl + VK ", ad.uVK );
   else
   if ( ad.fAlt )
      TraceLineX( "PreTranslateMessage Alt + VK ", ad.uVK );
   else
      TraceLineX( "PreTranslateMessage VK ", ad.uVK );
#endif

   // At least one of the two keys (Alt/Ctrl) must be pressed, or it is an
   // extended key (such as PageDown) ... for this to be an accelerator.

   HWND hWndFocus = ::GetFocus( );
   if ( hWndFocus )
   {
      ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
      if ( pZSubtaskMain == 0 )
         pZSubtaskMain = pZSubtask;

      if ( (pZSubtaskMain->m_pZFWnd == 0 ||
            pZSubtaskMain->m_pZFWnd->m_hWnd == 0 ||
            fnIsChild( pZSubtaskMain->m_pZFWnd->m_hWnd, hWndFocus ) == FALSE) )
      {
         return( FALSE );
      }
   }

   if ( pZSubtask->m_pAccels )
   {
      if ( pMsg->message == WM_SYSKEYDOWN ||  // system keydown  ... or
           (pMsg->message == WM_KEYDOWN &&    // keydown with alt or ctrl ... or
            ((ad.fAlt || ad.fCtrl) &&
             (ad.uVK >= 'A' && ad.uVK <= 'Z' ||
              ad.uVK >= '0' && ad.uVK <= '9')) ||
             !(ad.uVK >= 'A' && ad.uVK <= 'Z' ||   // extended keydown
               ad.uVK >= '0' && ad.uVK <= '9')) )
      {
      // if ( ad.uVK == 'E' )
      //    TraceLineS( "", "" );
//       if ( ad.uVK == VK_ESCAPE )
//          TraceLineX( "PreTranslateMessage m_pzmaAccel ",
//                      (zLONG) pZSubtask->m_pzmaAccel );

         if ( pZSubtask->m_pzmaAccel &&
              pZSubtask->m_pzmaAccel->m_pCtrl &&
              mIs_hWnd( pZSubtask->m_pzmaAccel->m_pCtrl->m_hWnd ) &&
              (hWndFocus == pZSubtask->m_pzmaAccel->m_pCtrl->m_hWnd ||
               (hWndFocus == 0 || GetParent( hWndFocus ) ==
                             pZSubtask->m_pzmaAccel->m_pCtrl->m_hWnd)) &&
              pZSubtask->m_pzmaAccel->HandleAccel( ad ) )
         {
            return( TRUE );
         }

      // TraceLineX( "Looking for VK ", ad.uVK );
         LPACCEL_TABLE lpAT = (LPACCEL_TABLE) pZSubtask->m_pAccels;
         while ( lpAT->nID != -1 )
         {
         // TraceLineX( "Accels table value ", lpAT->ac.uVK );
            // If the Virtual Key matches and the state of the Alt and
            // Ctrl keys match ... this accelerator is active for this
            // window.  Note that the state of the shift key is now always
            // relevent, even if the accelerator does not specify the
            // shift key.
            if ( lpAT->ac.uVK == ad.uVK &&
                 lpAT->ac.fAlt == ad.fAlt &&
                 lpAT->ac.fCtrl == ad.fCtrl &&
                 lpAT->ac.fShift == ad.fShift )
            {
            // TraceLineX( "IsAccelerator Located VK in table = ", lpAT->ac.uVK );
               if ( lpAT->chType == zACCEL_ACTION )
                  pZSubtask->m_pZFWnd->PostMessage( zZEIDON_PROCESS_ACTION,
                                                    0, lpAT->nID );
               else
               if ( lpAT->chType == zACCEL_HOTKEY )
                  pZSubtask->m_pZFWnd->PostMessage( zZEIDON_PROCESS_HOTKEY,
                                                    0, lpAT->nID );

           //  else ... handled for us by IsDialogMessage
           //  if ( lpAT->chType == zACCEL_CONTROL )
           //     SetFocus( GetDlgItem( ActiveWindow->HWindow, lpAT->nID ) );

               return( TRUE );
            }

            lpAT++;

         } // end of:  while ( lpAT->nID != -1 )

      } // end of:  if ( ad.fAlt || ad.fCtrl )
   }

   if ( ad.uVK == VK_RETURN || ad.uVK == VK_TAB || ad.uVK == VK_APPS )
   {
   // if ( pZSubtask->m_pzmaWithFocus )
   //    TraceLineS( "IsAccel VK_RETURN for pLastFocus ",
   //                *(pZSubtask->m_pzmaWithFocus->m_pzsTag) );

   // if ( ad.uVK == VK_TAB )
   //    TraceLineX( "fnIsAccelerator hWndFocus: ", (zLONG) hWndFocus );

      if ( pZSubtask->m_pzmaWithFocus &&
           pZSubtask->m_pzmaWithFocus->m_pCtrl &&
           mIs_hWnd( pZSubtask->m_pzmaWithFocus->m_pCtrl->m_hWnd ) )
      {
         if ( (hWndFocus == pZSubtask->m_pzmaWithFocus->m_pCtrl->m_hWnd ||
               hWndFocus == pZSubtask->m_pzmaWithFocus->m_hWndChild) )
         {
#if 0
            if ( (pZSubtask->m_ulWndOperX & zWND_EVENTX_HANDLE_KEY) &&
                 SetCursorFirstEntityByInteger( pZSubtask->m_vDialog,
                                                szlWndEvent, szlType,
                                                zWND_EVENTX_HANDLE_KEY, 0 )
                                                       > zCURSOR_UNCHANGED )
            {
               lRC = ProcessActionLoop( this, szlWndAct, 0, vAppSubtask,
                                        0, TRUE, "PostBuild" );
            ProcessImmediateEvent( this, zEDITBOX_Change );
#endif
            if ( pZSubtask->m_pzmaWithFocus->HandleQuickKey( ad ) )
            {
               return( TRUE );
            }
         }

         if ( ad.uVK == VK_APPS )
         {
            CPoint ptMouse;
            CPoint pt( 2, 2 );
            ::GetCursorPos( &ptMouse );
            pZSubtask->m_pzmaWithFocus->m_pCtrl->ClientToScreen( &pt );
            ::SetCursorPos( pt.x, pt.y );
            pZSubtask->m_pzmaWithFocus->m_pCtrl->SendMessage( WM_RBUTTONDOWN );
            pZSubtask->m_pzmaWithFocus->m_pCtrl->SendMessage( WM_RBUTTONUP );
            ::SetCursorPos( ptMouse.x, ptMouse.y );
            return( TRUE );
         }
      }

      if ( ad.uVK == VK_RETURN &&
           pZSubtask->m_pCurrDfltBtn &&
           (ad.fAlt == 0 && ad.fCtrl == 0 && ad.fShift == 0) &&
           pZSubtask->m_pCurrDfltBtn->m_pCtrl &&
           mIs_hWnd( pZSubtask->m_pCurrDfltBtn->m_pCtrl->m_hWnd ) )
      {
      // TraceLineS( "fnIsAccelerator VK_RETURN: ",
      //             *(pZSubtask->m_pCurrDfltBtn->m_pzsTag) );
         pZSubtask->m_pCurrDfltBtn->m_pCtrl->SendMessage( WM_LBUTTONDOWN );
         pZSubtask->m_pCurrDfltBtn->m_pCtrl->SendMessage( WM_LBUTTONUP );
         return( TRUE );
      }

      if ( ad.uVK == VK_TAB && hWndFocus == 0 )
         pZSubtask->FindFirstLastFocus( );
   }

   if ( pMsg->message == WM_SYSKEYDOWN && pZSubtask->m_pZMIXCtrl )
   {
      ZMapAct *pzma = pZSubtask->FindCtrlWithQuickKey( 0, (char) ad.uVK );
      if ( pzma && pzma->HandleQuickKey( ad ) )
         return( TRUE );
   }

   return( FALSE );
}

// returns TRUE  - if tab is ok
//         FALSE - if focus should stay on current control (ignore tab)
zBOOL
fnValidTab( ZSubtask *pZSubtask,
            zBOOL bBackward )
{
// TraceLineI( "Process TAB ", bBackward );
   if ( pZSubtask->m_pZMIXCtrl && pZSubtask->m_pzmaWithFocus )
   {
      zSHORT nRC;

      fnClearCtrlErr( pZSubtask->m_pzmaWithFocus, 0, 0 );
      fnPushMsgQ_State( pZSubtask->m_vMsgQ, zMSGQ_Tabbing );
      nRC = pZSubtask->m_pzmaWithFocus->MapToOI( zVALIDATE );
      fnPopMsgQ_State( pZSubtask->m_vMsgQ );

      if ( nRC < 0 )
      {
         pZSubtask->m_ulSubtaskFlags |= zSUBTASK_CONTROL_DOMAINERR;
         pZSubtask->m_pzmaWithFocus->SetZCtrlState( zCONTROL_STATUS_ERROR,
                                                    TRUE );
         MessagePresent( pZSubtask->m_vDialog, zMSGQ_AfterTabbing, 0 );
      }

//??? return( FALSE );
      return( TRUE );
   }

   return( TRUE );
}

// Let Windows have a shot at doing any outstanding processing.
zBOOL OPERATION
PumpMessages( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZTask *pZTask = pZSubtask->m_pZTask;
      MSG   msg;
      zBOOL bMsg = FALSE;

      while ( ::PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE ) )
      {
         // We can't knock out the WM_PAINT since Phil needs to see
         // the Cancel button, etc. painted.
         if ( msg.message == WM_KICKIDLE ) // || msg.message == WM_PAINT )
            break;

         bMsg = TRUE;
         pZTask->m_pZDrApp->PumpMessage( );
      }

      return( bMsg );
   }

   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0

STRINGTABLE DISCARDABLE
BEGIN
    IDS_TEENS_10            "Ten"
    IDS_TEENS_11            "Eleven"
    IDS_TEENS_12            "Twelve"
    IDS_TEENS_13            "Thirteen"
    IDS_TEENS_14            "Fourteen"
    IDS_TEENS_15            "Fifteen"
END

STRINGTABLE DISCARDABLE
BEGIN
    IDS_TEENS_16            "Sixteen"
    IDS_TEENS_17            "Seventeen"
    IDS_TEENS_18            "Eighteen"
    IDS_TEENS_19            "Nineteen"
END

STRINGTABLE DISCARDABLE
BEGIN
    IDS_TENS_0              "?"
    IDS_TENS_10             "Ten"
    IDS_TENS_20             "Twenty"
    IDS_TENS_30             "Thirty"
    IDS_TENS_40             "Forty"
    IDS_TENS_50             "Fifty"
    IDS_TENS_60             "Sixty"
    IDS_TENS_70             "Seventy"
    IDS_TENS_80             "Eighty"
    IDS_TENS_90             "Ninety"
END

STRINGTABLE DISCARDABLE
BEGIN
    IDS_ONES_0              "Zero"
    IDS_ONES_1              "One"
    IDS_ONES_2              "Two"
    IDS_ONES_3              "Three"
    IDS_ONES_4              "Four"
    IDS_ONES_5              "Five"
    IDS_ONES_6              "Six"
    IDS_ONES_7              "Seven"
END

STRINGTABLE DISCARDABLE
BEGIN
    IDS_ONES_8              "Eight"
    IDS_ONES_9              "Nine"
    IDS_UNKNOWN             "???"
    IDS_MILLION             "Million"
    IDS_THOUSAND            "Thousand"
    IDS_AND                 "and"
    IDS_HUNDRETHS_DOLLARS   "/100ths Dollars"
    IDS_HUNDRED             "Hundred"
END

#define INVALID_DOLLARS_CENTS 0xffffffff

zBOOL OPERATION GetDollarsCents( CWnd *pWnd, ZDecimal& dwCents );
zBOOL OPERATION GetDollarsCents( HWND hWnd, ZDecimal& dwCents );
void  OPERATION SetDollarsCents( CWnd *pWnd, ZDecimal dwCents );
CString OPERATION GetDollarsCentsFormatted( ZDecimal dwCents );
CString OPERATION GetDollarsCentsText( ZDecimal dwCents );
CString HundredsTensOnes( ZDecimal dwHundredsTensOnes );
CString TensOnes( ZDecimal dwTensOnes );

zBOOL OPERATION
GetDollarsCents( CWnd *pWnd, ZDecimal& dwCents )
{
   ASSERT( pWnd );
   return( GetDollarsCents( pWnd->m_hWnd, dwCents ) );
}

zBOOL OPERATION
GetDollarsCents( HWND hWnd, ZDecimal& dwCents )
{
   zCHAR  szWindowText[ 128 ];
   ZDecimal dwDollars;
   int nCents;
   zPCHAR pch;
   zPCHAR pchDollars;
   zPCHAR pchCents;

   ::GetWindowText( hWnd, szWindowText, zsizeof( szWindowText ) - 1 );

   // strip leading blanks
   for ( pchDollars = szWindowText;  *pchDollars == ' ';  pchDollars++ )
   {
      if ( *pchDollars == 0 )
      {
         dwCents = 0;
         return( TRUE );
      }
   }

   // parse dollar amount, before optional decimal point
   for ( pch = pchDollars; *pch && (*pch != '.') && (*pch != ' '); pch++ )
   {
      if ( (*pch < '0') || (*pch > '9') )
         return( FALSE );
   }

   zBOOL bDollarsOnly = (*pch == 0);
   *pch = 0;

   if ( zstrlen( pchDollars ) > 8 )
      return( FALSE );

   if ( zstrlen( pchDollars ) == 0 )
   {
      dwDollars = 0L;
   }
   else
   {
      dwDollars = _ttol( pchDollars );
      if ( dwDollars > ((ZDecimal) 0xffffffff) / 100 )
         return( FALSE );
   }

   if ( bDollarsOnly )
   {
      nCents = 0;
   }
   else  // decimal point was found
   {
      // parse cents
      for ( pchCents = ++pch; *pch && (*pch != ' '); pch++ )
      {
         if ( (*pch < '0') || (*pch > '9') )
            return( FALSE );
      }

      if ( *pch == ' ' )
      {
         for ( pch++ ; *pch; pch++ )
         {
            if ( *pch != ' ' )
               return( FALSE );
         }
      }

      zSHORT nCentsStrLen = (zSHORT) zstrlen( pchCents );
      switch ( nCentsStrLen )
      {
         case 0:
            nCents = 0;
            break;
         case 1:
            nCents = _ttoi( pchCents ) * 10;
            break;
         case 2:
            nCents = _ttoi( pchCents );
            break;
         default:
            return( FALSE );
      }
   }

   dwCents = dwDollars * 100 + nCents;
   return( TRUE );
}

void OPERATION
SetDollarsCents( CWnd *pWnd, ZDecimal dwCents )
{
   ASSERT( pWnd );
   SetDollarsCents( pWnd->m_hWnd, dwCents );
}

void OPERATION
SetDollarsCents( HWND hWnd, ZDecimal dwCents )
{
   // Convert the ZDecimal dollars/cents value to a string and
   // display it in the dollars/cents control.

   // If the dollar cent field has been previously determined to be
   // invalid, then don't update it.  Leave the invalid data in the
   // field so the user can correct it, rather than replace it with the
   // literal translation of the INVALID_DOLLARS_CENTS #define value.

   if ( dwCents == INVALID_DOLLARS_CENTS )
      return;

   CString cs = GetDollarsCentsFormatted( dwCents );
   ::SetWindowText( hWnd, cs.GetBufferSetLength( 20 ) );
}

CString OPERATION
GetDollarsCentsFormatted( ZDecimal dwCents )
{
   if ( dwCents == INVALID_DOLLARS_CENTS )
   {
      CString cs;
      cs.LoadString( IDS_UNKNOWN );
      return( cs );
   }

   ZDecimal dwDollars = dwCents / 100;
   WORD wCents = (WORD)(dwCents - 100  *dwDollars);

   CString cs;
   cs.Format( _T( "%lu.%02u" ), dwDollars, wCents );
   return( cs );
}

CString OPERATION
GetDollarsCentsText( ZDecimal dwCents )
{
   CString cs;
   CString csTemp;

   if ( dwCents == INVALID_DOLLARS_CENTS )
   {
      cs.LoadString( IDS_UNKNOWN );
      return( cs );
   }

   ZDecimal dwDollars = dwCents / 100;
   WORD wCents = (WORD)(dwCents - (dwDollars * 100));
   if ( dwDollars == 0L )
   {
      cs.LoadString( IDS_ONES_0 );   // "Zero"
      cs += ' ';
   }
   else
   {
      if ( dwDollars >= 1000000 )
      {
         ZDecimal dwMillions = dwDollars / 1000000;
         CString csMillions = HundredsTensOnes( dwMillions );
         csTemp.LoadString( IDS_MILLION );    // "Million"
         cs = csMillions;
         cs += ' ';
         cs += csTemp;
         cs += ' ';
         dwDollars -= (dwMillions * 1000000);
      }

      if ( dwDollars >= 1000 )
      {
         ZDecimal dwThousands = dwDollars / 1000;
         CString csThousands = HundredsTensOnes( dwThousands );
         csTemp.LoadString( IDS_THOUSAND );   // "Thousand"
         cs += csThousands;
         cs += ' ';
         cs += csTemp;
         cs += ' ';
         dwDollars -= (dwThousands * 1000);
      }

      if ( dwDollars > 0 )
      {
         CString csHundredsTensOnes = HundredsTensOnes( dwDollars );
         cs += csHundredsTensOnes;
         cs += ' ';
      }
   }

   zCHAR szCents[ 10 ];
   CString csCents( _itot( wCents, szCents, 10 ) );
   csTemp.LoadString( IDS_AND );    // "and"
   cs += csTemp;
   cs += ' ';
   cs += csCents;
   csTemp.LoadString( IDS_HUNDRETHS_DOLLARS );  // "/100ths Dollars"
   cs += csTemp;
   return( cs );
}

CString
HundredsTensOnes( ZDecimal dwHundredsTensOnes )
{
   CString cs;
   CString csTemp;

   if ( dwHundredsTensOnes >= 100 )
   {
      ZDecimal dwHundreds = dwHundredsTensOnes / 100;
      CString csHundreds;
      csHundreds.LoadString( IDS_ONES_0 + dwHundreds );
      csTemp.LoadString( IDS_HUNDRED );
      cs = csHundreds;
      cs += ' ';
      cs += csTemp;
      cs += ' ';
      dwHundredsTensOnes -= (dwHundreds * 100);
   }

   if ( dwHundredsTensOnes > 0 )
   {
      CString csTensOnes = TensOnes( dwHundredsTensOnes );
      cs += csTensOnes;
   }

   return( cs );
}

CString
TensOnes( ZDecimal dwTensOnes )
{
   CString cs;
   CString csTemp;

   if ( dwTensOnes > 19 )
   {
      ZDecimal dwTens = dwTensOnes / 10;
      csTemp.LoadString( IDS_TENS_0 + dwTens );
      cs += csTemp;
      dwTensOnes -= (dwTens * 10);
      if ( dwTensOnes > 0 )
      {
         CString csOnes;
         csOnes.LoadString( IDS_ONES_0 + dwTensOnes );
         cs += '-';
         cs += csOnes;
      }
   }
   else
   if ( dwTensOnes >= 10 )
   {
      CString csTeens;
      csTeens.LoadString( IDS_TEENS_10 + dwTensOnes - 10 );
      cs += csTeens;
   }
   else
   {
      CString csOnes;
      csOnes.LoadString( IDS_ONES_0 + dwTensOnes );
      cs += csOnes;
   }

   return( cs );
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZSubtaskList dialog

ZSubtaskList::ZSubtaskList( zVIEW vSubtask, CWnd *pParent /*=NULL*/ ) :
              CDialog( ZSubtaskList::IDD, pParent )
{
   m_vSubtask = vSubtask;

   //{{AFX_DATA_INIT(ZSubtaskList)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(ZSubtaskList, CDialog)
   //{{AFX_MSG_MAP(ZSubtaskList)
   ON_NOTIFY(NM_DBLCLK, IDC_LIST_SUBTASKS, OnDblclkListSubtasks)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZSubtaskList message handlers

BOOL
ZSubtaskList::OnInitDialog( )
{
   CDialog::OnInitDialog( );

   // TODO: Add extra initialization here

   CListCtrl *pListCtrl = (CListCtrl *) GetDlgItem( IDC_LIST_SUBTASKS );
   ASSERT( pListCtrl );

   pListCtrl->DeleteAllItems( );

   // Loop processing each column.
   LV_COLUMN lvc;
   zSHORT    nCol;

   for ( nCol = 0; nCol < 6; nCol++ )
   {
      lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
      lvc.fmt = LVCFMT_LEFT;
      switch ( nCol )
      {
         case 0:
            lvc.pszText = "Dialog";
            lvc.cx = 100;
            break;

         case 1:
            lvc.pszText = "Window";
            lvc.cx = 100;
            break;

         case 2:
            lvc.pszText = "Subtask";
            lvc.cx = 80;
            break;

         case 3:
            lvc.pszText = "Subtask Id";
            lvc.cx = 80;
            break;

         case 4:
            lvc.pszText = "Task Id";
            lvc.cx = 80;
            break;

         case 5:
            lvc.pszText = "Enabled";
            lvc.cx = 80;
            break;
      }

      lvc.iSubItem = nCol;
      if ( pListCtrl->InsertColumn( nCol, &lvc ) != nCol )
      {
         TraceLineI( "ZSubtaskList::OnInitDialog InsertColumn failed: ",
                     nCol );
      }
   }

   // Protect this with a semaphore!!!
   SysMutexLock( g_vRegister, "ZDr", 0, 0 );
// TraceLineS( "ZSubtaskList::OnInitDialog Lock Mutex ZDr", "" );

   zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   zLONG lPos;
   zCHAR  szListLine[ 256 ];
   LV_ITEM lvItem;

// pListCtrl->SetItemCount( lItemCnt );
   for ( lPos = 1; lPos <= lItemCnt; lPos++ )
   {
      if ( g_pSubtaskList[ lPos ].pZSubtask &&
           (g_pSubtaskList[ lPos ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
      {
         lvItem.mask = 0;  //LVIF_TEXT;
         lvItem.iItem = lPos - 1;
         lvItem.iSubItem = 0;
      // lvItem.pszText = g_pSubtaskList[ lPos ].DlgN;
      // lvItem.cchTextMax = zstrlen( g_pSubtaskList[ lPos ].DlgN ) + 1;
         pListCtrl->InsertItem( &lvItem );

         lvItem.mask = LVIF_TEXT;
         lvItem.iItem = lPos - 1;
         lvItem.iSubItem = 0;
         lvItem.pszText = g_pSubtaskList[ lPos ].DlgN;
         lvItem.cchTextMax = zstrlen( g_pSubtaskList[ lPos ].DlgN ) + 1;
         pListCtrl->SetItem( &lvItem );

         lvItem.mask = LVIF_TEXT;
         lvItem.iItem = lPos - 1;
         lvItem.iSubItem = 1;
         lvItem.pszText = g_pSubtaskList[ lPos ].WndN;
         lvItem.cchTextMax = zstrlen( g_pSubtaskList[ lPos ].WndN ) + 1;
         pListCtrl->SetItem( &lvItem );

         sprintf_s( szListLine, zsizeof( szListLine ), "0x%08x", (zULONG) g_pSubtaskList[ lPos ].pvDialog );
      // _ltoa_s( (zLONG) g_pSubtaskList[ lPos ].pvDialog, szListLine );
         lvItem.mask = LVIF_TEXT;
         lvItem.iItem = lPos - 1;
         lvItem.iSubItem = 2;
         lvItem.pszText = szListLine;
         lvItem.cchTextMax = zstrlen( szListLine ) + 1;
         pListCtrl->SetItem( &lvItem );

         if ( g_pSubtaskList[ lPos ].pZSubtask )
            _ltoa_s( (zLONG) g_pSubtaskList[ lPos ].pZSubtask->m_ulSubtaskIdNbr, szListLine, zsizeof( szListLine ) );
         else
            strcpy_s( szListLine, zsizeof( szListLine ), "0" );

         lvItem.mask = LVIF_TEXT;
         lvItem.iItem = lPos - 1;
         lvItem.iSubItem = 3;
         lvItem.pszText = szListLine;
         lvItem.cchTextMax = zstrlen( szListLine ) + 1;
         pListCtrl->SetItem( &lvItem );

         sprintf_s( szListLine, zsizeof( szListLine ), "0x%08x", g_pSubtaskList[ lPos ].lTID );
      // _ltoa_s( (zLONG) g_pSubtaskList[ lPos ].lTID, szListLine );
         lvItem.mask = LVIF_TEXT;
         lvItem.iItem = lPos - 1;
         lvItem.iSubItem = 4;
         lvItem.pszText = szListLine;
         lvItem.cchTextMax = zstrlen( szListLine ) + 1;
         pListCtrl->SetItem( &lvItem );

         if ( mIs_hWnd( g_pSubtaskList[ lPos ].hWnd ) )
         {
            if ( mIs_hWndVisible( g_pSubtaskList[ lPos ].hWnd ) )
            {
               if ( mIs_hWndEnabled( g_pSubtaskList[ lPos ].hWnd ) )
                  strcpy_s( szListLine, zsizeof( szListLine ), "Enabled" );
               else
                  strcpy_s( szListLine, zsizeof( szListLine ), "Disabled" );
            }
            else
               strcpy_s( szListLine, zsizeof( szListLine ), "Invisible" );
         }
         else
            strcpy_s( szListLine, zsizeof( szListLine ), "Invalid" );

      // _ltoa_s( (zLONG) g_pSubtaskList[ lPos ].lTID, szListLine );
         lvItem.mask = LVIF_TEXT;
         lvItem.iItem = lPos - 1;
         lvItem.iSubItem = 5;
         lvItem.pszText = szListLine;
         lvItem.cchTextMax = zstrlen( szListLine ) + 1;
         pListCtrl->SetItem( &lvItem );
      }
   }

   // Call this Auto Size Columns after your list control is filled.
   pListCtrl->SetRedraw( FALSE );
   for ( nCol = 0; nCol < 6; nCol++ )
   {
      pListCtrl->SetColumnWidth( nCol, LVSCW_AUTOSIZE );
      zLONG lColWidth1 = pListCtrl->GetColumnWidth( nCol );
      pListCtrl->SetColumnWidth( nCol, LVSCW_AUTOSIZE_USEHEADER );
      zLONG lColWidth2 = pListCtrl->GetColumnWidth( nCol );
      zLONG lColWidth = max( lColWidth1, lColWidth2 );
      pListCtrl->SetColumnWidth( nCol, lColWidth );
   }

   // RecalcHeaderTips( ); *** uncomment this if you use my header tips method
   pListCtrl->SetRedraw( TRUE );
   pListCtrl->Invalidate( ); // *** uncomment this if you don't use my SetRedraw function

// TraceLineS( "ZSubtaskList::OnInitDialog Unlock Mutex ZDr", "" );
   SysMutexUnlock( g_vRegister, "ZDr", 0 );

   pListCtrl->SetFocus( );
   return( FALSE );  // return TRUE unless you set the focus to a control
                     // EXCEPTION: OCX Property Pages should return FALSE
}

void
ZSubtaskList::OnOK( )
{
#if defined( _MSC_VER ) && _MSC_VER > 1100
   CListCtrl *pListCtrl = (CListCtrl *) GetDlgItem( IDC_LIST_SUBTASKS );
   ASSERT( pListCtrl );

   // Protect this with a semaphore!!!
   SysMutexLock( g_vRegister, "ZDr", 0, 0 );
// TraceLineS( "ZSubtaskList::OnOK Lock Mutex ZDr", "" );

   POSITION pos = pListCtrl->GetFirstSelectedItemPosition( );
   if ( pos )
   {
      zLONG lCurrSel = pListCtrl->GetNextSelectedItem( pos ) + 1;
      zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;

      if ( lCurrSel <= lItemCnt )
      {
         zLONG lPos;
         zLONG lSel = 0;

         for ( lPos = 1; lPos < lItemCnt; lPos++ )
         {
            if ( g_pSubtaskList[ lPos ].pZSubtask )
            {
               lSel++;
               if ( lSel == lCurrSel )
               {
                  lCurrSel = lPos;
                  break;
               }
            }
         }

         zVIEW vSubtask = (zVIEW)
              SysGetPointerFromHandle( g_pSubtaskList[ lCurrSel ].pvDialog );
         if ( vSubtask && MiValidView( vSubtask ) &&
              mIs_hWnd( g_pSubtaskList[ lCurrSel ].hWnd ) &&
              mIs_hWndEnabled( g_pSubtaskList[ lCurrSel ].hWnd ) )
         {
            if ( ::IsIconic( g_pSubtaskList[ lCurrSel ].hWnd ) )
               ::ShowWindow( g_pSubtaskList[ lCurrSel ].hWnd, SW_SHOWNORMAL );
            else
            {
               ::ShowWindow( g_pSubtaskList[ lCurrSel ].hWnd, SW_SHOWMINIMIZED );
               ::ShowWindow( g_pSubtaskList[ lCurrSel ].hWnd, SW_RESTORE );
            }

            ::SetWindowPos( g_pSubtaskList[ lCurrSel ].hWnd,
                            HWND_TOP, 0, 0, 0, 0,
                            SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );
            ::FlashWindow( g_pSubtaskList[ lCurrSel ].hWnd, FALSE );
         // ::SetFocus( g_pSubtaskList[ lCurrSel ].hWnd );
         }
      }
   }

// TraceLineS( "ZSubtaskList::OnOK Unlock Mutex ZDr", "" );
   SysMutexUnlock( g_vRegister, "ZDr", 0 );
#endif

   CDialog::OnOK( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZSubtaskList::OnDblclkListSubtasks( NMHDR *pNMHDR, LRESULT *pResult )
{
   // TODO: Add your control notification handler code here
   *pResult = 0;
   OnOK( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
SetClipboardBlob( zPVOID pvData, zLONG lDataLth, zCPCHAR cpcFormat )
{
   UINT uFormat;

   if ( cpcFormat && *cpcFormat )
   {
      ASSERT( lDataLth > 0 );
      uFormat = RegisterClipboardFormat( cpcFormat );
      if ( uFormat == 0 )
         return( -2 );
   }
   else
   {
      uFormat = CF_TEXT;
      lDataLth = zstrlen( (zPCHAR) pvData ) + 1;
   }

   // Test to see if we can open the clipboard first before
   // wasting any cycles with the memory allocation.
   if ( OpenClipboard( 0 ) )
   {
      // Empty the Clipboard. This also has the effect of allowing
      // Windows to free the memory associated with any data that
      // is in the Clipboard.
      EmptyClipboard( );

      // OK, we have the Clipboard locked and it's empty.
      // Now let's allocate the global memory for our data.

      // Use the GlobalAlloc function to allocate a block of data equal
      // to the text in the "to clipboard" edit control plus one character
      // for the terminating null character required when sending ANSI
      // text to the Clipboard.
      HGLOBAL hClipboardData;
      hClipboardData = GlobalAlloc( GMEM_DDESHARE, lDataLth );

      // Calling GlobalLock returns a pointer to the data
      // associated with the handle returned from GlobalAlloc.
      zPCHAR pchData;
      pchData = (zPCHAR) GlobalLock( hClipboardData );

      // At this point, all I need to do is use the standard
      // C/C++ memcpy function to copy the data from the local
      // variable to the global memory.
      zmemcpy( pchData, pvData, lDataLth );

      // Once done, unlock the memory - remember!!! DON'T call
      // GlobalFree because Windows will free the memory automatically
      // when EmptyClipboard is next called.
      GlobalUnlock( hClipboardData );

      // Now, set the Clipboard data by specifying the format that is
      // being used and passing the handle to the global memory.
      SetClipboardData( uFormat, hClipboardData );

      // Finally, when finished, simply close the Clipboard ...
      // which has the effect of unlocking it so that other
      // applications can examine or modify its contents.
      CloseClipboard( );
      return( 0 );
   }

   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
GetClipboardBlob( zPVOID pvData, zLONG lDataLth, zCPCHAR cpcFormat )
{
   UINT   uFormat;
   zSHORT nRC;

   if ( cpcFormat && *cpcFormat )
   {
      ASSERT( lDataLth > 0 );
      uFormat = RegisterClipboardFormat( cpcFormat );
      if ( uFormat == 0 )
         return( -2 );
   }
   else
   {
      uFormat = CF_TEXT;
      lDataLth++;
   }

   // Test to see if we can open the clipboard first.
   if ( OpenClipboard( 0 ) )
   {
      // In order to check the format of the data on the Clipboard,
      // use the IsClipboardAvailable function.
      if ( ::IsClipboardFormatAvailable( uFormat ) ||
           (uFormat == CF_TEXT && ::IsClipboardFormatAvailable( CF_OEMTEXT )) )
      {
         // Retrieve the Clipboard data specifying the required format.
         HANDLE hClipboardData = GetClipboardData( uFormat );

         // Call GlobalLock so that to retrieve a pointer to the data
         // associated with the handle returned from GetClipboardData.
         zPVOID pvClipboardData = GlobalLock( hClipboardData );

         // Set a local memory from the global (clipboard) data.
         zmemcpy( pvData, pvClipboardData, lDataLth );

         // Unlock the global memory.
         GlobalUnlock( hClipboardData );
         nRC = 0;
      }
      else
      {
         TraceLine( "GetClipboardBlob: There is no (%s) data on the Clipboard",
                    cpcFormat && *cpcFormat ? cpcFormat : "ANSI Text" );
         nRC = -1;
      }

      // Finally, simply close the Clipboard which has the effect of unlocking
      // it so that other applications can examine or modify its contents.
      CloseClipboard( );
   }
   else
      nRC = -1;

   return( nRC );
}
