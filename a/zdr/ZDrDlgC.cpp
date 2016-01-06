/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdlgc.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of "C interface" routines into the
// C++ functionality for controls available through the Zeidon driver.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.06.20    DKS/BL/FH
//    Hack to remove differences in GetWindowText between Static and Edit
//    controls.
//
// 2001.11.30    DKS
//    Always call SetUpdateFlagForAttribute for XML generation.
//
// 2001.07.02    TMV
//    Change RefreshCtrl to keep track about "disable action conditions"
//
// 2001.05.15    BL    WEB
//     Added SetUpdateFlagForAttribute and modified SetRemoteZCtrlAttribute:
//     If the Control Text is null, we have to set the Update flag for XML
//     generation.
//
// 2001.05.09    DKS   WEB
//    Fix Set and Get of Ctrl Size and Position for WEB.
//
// 2001.05.08    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 2001.02.26    BL/DKS
//     Modified RefreshCtrl: do not set old Control Flags
//
// 2001.02.06    DKS   F51326
//    Force new "first focus control" when application calls SetFocusToCtrl.
//    This may cause new "focus problems" in existing applications.  We
//    need to be on the lookout for this.
//
// 2000.10.26    DKS   WEB
//    Fix to GetCtrlText and SetCtrlText.
//
// 2000.03.01    DKS   Z10   TB99999
//    Added ValidateMapCtrl to permit use of LoseFocus event.
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added Mapping List.
//
// 1999.05.17    DKS   Z10+9J  QS999
//    TMV reported bug regarding GetCtrlWidthFocus, which now returns 1 when
//    the control was the last known to the Zeidon Driver to have focus, but
//    which no longer has the focus (another application is active).
//
// 1999.05.11    DKS   Z10   ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1999.02.24    DKS
//    SetCtrlText changed to set the ZMapAct m_pzsText member.
//    GetCtrlText changed to get the ZMapAct m_pzsText member if the
//    ctrl is not currently a valid hWnd.
//
// 1998.08.13    DKS
//    Changed the return code from MapCtrl so the application could
//    determine the correct error condition.
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


//./ ADD NAME=HiliteListBoxItem
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: HiliteListBoxItem
//                                              04/01/93 Modified: 04/16/93
//
//  PURPOSE:    To hilite listbox item(s) at a given offset/cursor position/
//              selected entity(s)
//
//    This operation permits the user to control the hilighting of listbox
//    items without the overhead of rebuilding the list from the object
//    instance.  There are three methods of hilighting as follows:
//
//    lType  -  >= 0   ==> hilite at given offset
//              -1     ==> hilite at current cursor position
//              -2     ==> hilite selected entities
//
//    There are two cases of a listbox to consider - single and multiple
//    select.
//
//    Single select is quite straightforward.  If bHilite is FALSE, no
//    matter what, every item will be deselected.  If bHilite is TRUE,
//    processing will occur as follows:
//
//       Hilite at given offset - reset the hilite to the entity at the
//                                given offset.
//       Hilite at current ...  - reset the hilite to the entity at the
//                                current cursor position.
//       Hilite selected entity - reset the hilite to the selected entity.
//
//    Multiple select:
//
//       Hilite at given offset - add/subtract the hilite on the item
//                                corresponding to the entity at the
//                                given offset (based on bHilite).
//
//       Hilite at current ...  - add/subtract the hilite on the item
//                                corresponding to the entity at the
//                                current cursor position (based on bHilite).
//
//       Hilite selected entity - hilite the item(s) corresponding to
//                                the selected entity(s).  N.B.  bHilite
//                                is ignored.
//
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is the subtask
//                            passed into the dialog operation if the
//                            control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              bHilite     - TRUE ==> hilite on ... FALSE ==> hilite off
//              lType       - >= 0 ==> hilite at given offset
//                            -1   ==> hilite at current cursor position
//                            -2   ==> hilite selected entities
//
//  RETURNS:    0 - Control hilighted
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
HiliteListBoxItem( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zSHORT  bHilite,
                   zLONG   lType )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In HiliteControlItem ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->HiliteText( (zBOOL) bHilite, lType );
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=GetLastCtrlTag
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetLastCtrlTag
//                                              10/27/99 Modified: 10/27/99
//
//  PURPOSE:   To get the most recent tag of the control that triggered an
//             action.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the action.
//              pchReturnTag - The return buffer into which to copy the
//                            control tag.  This buffer should be at least
//                            zTAG_LTH bytes long.
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
GetLastCtrlTag( zVIEW  vSubtask,
                zPCHAR pchReturnTag )
{
   ZSubtask *pZSubtask;

   *pchReturnTag = 0;
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pzsLastCtrlTag )
         strcpy_s( pchReturnTag, zTAG_LTH, *(pZSubtask->m_pzsLastCtrlTag) );

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=SetCtrlError
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlError
//                                              04/19/93 Modified: 04/19/93
//
//  PURPOSE:    This operation sets the specified ctrl in error.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be mapped. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              cpcMsgText  - Message to be presented (null ==> domain error)
//
//  RETURNS:    0 - Control mapped
//              1 - Mapping error occurred
//             -1 - Error locating control
//
//  NOTE:   To map controls on other windows, the subtask under which the
//          other window was initially created may be given a name during
//          windows initialization code. The name may then be picked up
//          later for use as the subtask qualifier in a map request.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
SetCtrlError( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              zCPCHAR cpcMsgText )
{
#ifdef DEBUG_ALL
      TraceLineS( "In SetCtrlError ", cpcCtrlTag );
#endif

   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      if ( cpcMsgText && *cpcMsgText )
      {
         OperatorPrompt( vSubtask, "Control Error", cpcMsgText, 0, 0, 0, 0 );
      }
      else
      {
         pZSubtask->m_ulSubtaskFlags |= zSUBTASK_CONTROL_DOMAINERR;
         MessagePresent( vSubtask, zMSGQ_AfterMapToOI, 0 );
      }

      if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
      {
         pzma->m_pCtrl->SetFocus( );
      }

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=MapCtrl
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: MapCtrl
//                                              04/19/93 Modified: 04/19/93
//
//  PURPOSE:    This operation calls the appropriate mapping operation to
//              map (MapToOI) a control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be mapped. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Control mapped
//              1 - Mapping error occurred
//             -1 - Error locating control
//
//  NOTE:   To map controls on other windows, the subtask under which the
//          other window was initially created may be given a name during
//          windows initialization code. The name may then be picked up
//          later for use as the subtask qualifier in a map request.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
MapCtrl( zVIEW   vSubtask,
         zCPCHAR cpcCtrlTag )
{
#ifdef DEBUG_ALL
   TraceLineS( "In MapCtrl ", cpcCtrlTag );
#endif

   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      zULONG  ulMapActFlags = pzma->m_ulMapActFlags;
      pzma->m_ulMapActFlags &= ~zMAPACT_NOAUTOMAP_TO_OI;
      zSHORT nRC = pzma->MapToOI( );
      pzma->m_ulMapActFlags = ulMapActFlags;
      if ( nRC == 0 )
         return( 0 );
      else
         return( 1 );
   }

   return( -1 );
}

//./ ADD NAME=ValidateMapCtrl
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ValidateMapCtrl
//                                              04/19/93 Modified: 04/19/93
//
//  PURPOSE:    This operation calls the appropriate mapping operation to
//              validate the mapping (MapToOI) for a control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be mapped. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control (if blank,
//                            all controls are checked).
//
//  RETURNS:    0 - Control mapping is validated
//              1 - Mapping error occurred
//             -1 - Error locating control
//
//  NOTE:   To map controls on other windows, the subtask under which the
//          other window was initially created may be given a name during
//          windows initialization code. The name may then be picked up
//          later for use as the subtask qualifier in a map request.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
ValidateMapCtrl( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineS( "In ValidateMapCtrl ", cpcCtrlTag );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      zULONG ulMapActFlags;
      zSHORT nRC;

      if ( pzma )
      {
         ulMapActFlags = pzma->m_ulMapActFlags;
         pzma->m_ulMapActFlags &= ~zMAPACT_NOAUTOMAP_TO_OI;
         nRC = pzma->MapToOI( zVALIDATE );
         pzma->m_ulMapActFlags = ulMapActFlags;
         if ( nRC == 0 )
            return( 0 );
         else
            return( 1 );
      }
      else
      if ( pZSubtask->m_pZMIXCtrl )
      {
         zLONG   lIdx = 0;
         zLONG   lCnt = pZSubtask->m_pZMIXCtrl->GetCount( );

         while ( lIdx++ < lCnt )
         {
            pzma = (ZMapAct *) pZSubtask->m_pZMIXCtrl->GetAt( lIdx );
            ulMapActFlags = pzma->m_ulMapActFlags;
            pzma->m_ulMapActFlags &= ~zMAPACT_NOAUTOMAP_TO_OI;
            nRC = pzma->MapToOI( zVALIDATE );
            pzma->m_ulMapActFlags = ulMapActFlags;
            if ( nRC )
               return( 1 );
         }

         return( 0 );
      }
   }

   return( -1 );
}

void
fnRefreshCtrl( ZMapAct *pzma, zBOOL bProcessChildren );

void
fnMapChildFromOI( ZMapAct *pzma,
                  WPARAM  wParam,
                  LPARAM  lParam )
{
   ZMapAct *pzmaParent = (ZMapAct *) lParam;
   if ( pzma->m_pzmaComposite == pzmaParent )
      fnRefreshCtrl( pzma, (zBOOL) wParam );
}

// Recursive routine to refresh child ctrls.
void
fnRefreshCtrl( ZMapAct *pzma, zBOOL bProcessChildren )
{
   if ( bProcessChildren )
   {
      pzma->m_pZSubtask->
         m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnMapChildFromOI,
                                    0, bProcessChildren, (LPARAM) pzma );
   }

   zBOOL bResetNoAutoMap;

   if ( pzma->m_ulMapActFlags & zMAPACT_NOAUTOMAP_FROM_OI )
   {
      bResetNoAutoMap = TRUE;
      pzma->m_ulMapActFlags &= ~zMAPACT_NOAUTOMAP_FROM_OI;
   }
   else
      bResetNoAutoMap = FALSE;

   pzma->MapFromOI( );
   if ( bResetNoAutoMap )
      pzma->m_ulMapActFlags |= zMAPACT_NOAUTOMAP_FROM_OI;

}

//./ ADD NAME=RefreshCtrl
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RefreshCtrl
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:    This operation calls the appropriate mapping operation to
//              refresh (MapFromOI) a control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be refreshed. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control to be refreshed.
//                            This may be a semi-colon separated list of
//                            controls if the first character is a TAB
//                            character.  If the second character is also a
//                            TAB, the list is a list of parent controls for
//                            which all children will be refreshed as well.
//
//  RETURNS:    0 - Control refreshed
//             -1 - Error locating control
//
//  NOTE:   To refresh controls on other windows, the subtask under
//          which the other window was initially created may be given a
//          name by that windows initialization code. The name may then
//          be picked up later for use as the subtask qualifier in
//          a refresh.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
RefreshCtrl( zVIEW   vSubtask,
             zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In RefreshCtrl ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 && pZSubtask->m_pZMIXCtrl )
   {
      ZMapAct *pzma;
      zPCHAR  pch;
      zBOOL   bTagList = FALSE;
      zBOOL   bProcessChildren = FALSE;

      if ( cpcCtrlTag && *cpcCtrlTag )  // purist's goto
      {
         zLONG  lLth = zstrlen( cpcCtrlTag ) + 1;
         zPCHAR pchTags = new char[ lLth ];
         strcpy_s( pchTags, lLth, cpcCtrlTag );

         pch = pchTags;
         if ( *pch == '\t' )
         {
            pch++;
            bTagList = TRUE;
            if ( *pch == '\t' )
            {
               pch++;
               bProcessChildren = TRUE;
            }
         }

         zPCHAR  pchTag = 0;
         while ( pch && *pch )
         {
            if ( bTagList )
               pchTag = zstrchr( pch, ';' );

            if ( pchTag )
               *pchTag = 0;

            CString csSeek( pch );  // localize Tag for the seek
            pzma = (ZMapAct *) pZSubtask->m_pZMIXCtrl->SeekNode( &csSeek );

            if ( pchTag )
            {
               *pchTag = ';';
               pch = pchTag + 1;
            }
            else
               pch = 0;

            if ( pzma )
               fnRefreshCtrl( pzma, bProcessChildren );
         }

         delete [] pchTags;
      }

      // If action autogray is requested for this window, gray any actions
      // for which automatic graying is requested.
      if ( pZSubtask->m_ulWndOper & zWND_EVENT_ACTION_AUTOGRAY )
         pZSubtask->AutoGrayActions( );

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=RefreshVisibleItems
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RefreshVisibleItems
//                                              12/01/99 Modified: 12/01/99
//
//  PURPOSE:    This operation calls the appropriate mapping operation to
//              redisplay new data in a control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be refreshed. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              lFlag       - 0 - repaint entire display area of control
//                            1 - repaint display area of item at current
//                                cursor position in the control
//                            2 - refresh control maintaining top
//
//  RETURNS:    0 - Control refreshed
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
RefreshVisibleItems( zVIEW   vSubtask,
                     zCPCHAR cpcCtrlTag,
                     zLONG   lFlag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In RefreshVisibleItems ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->SetZCtrlState( zCONTROL_STATUS_REPAINT, lFlag );
      return( 0 );
   }

   return( -1 );
}

zOPER_EXPORT zBOOL OPERATION
PositionOnRemoteCtrlParent( ZMapAct *pzma,
                            zVIEW   vXRA,
                            zBOOL   bCreateEntity )
{
#ifdef zREMOTE_SERVER
   zBOOL bRC = TRUE;

   if ( pzma->m_pzmaComposite )
   {
      bRC = PositionOnRemoteCtrlParent( pzma->m_pzmaComposite, vXRA, bCreateEntity );
      if ( bRC )
         SetViewToSubobject( vXRA, "CtrlCtrl" );
   }

   if ( bRC )
   {
      if ( CheckExistenceOfEntity( vXRA, "Ctrl" ) >= zCURSOR_SET &&
           SetCursorFirstEntityByString( vXRA, "Ctrl", "Tag", *(pzma->m_pzsTag), 0 ) == 0 )
      {
         if ( CheckExistenceOfEntity( vXRA, pzma->m_csERemote ) < zCURSOR_SET )
         {
            CreateEntity( vXRA, pzma->m_csERemote, zPOS_AFTER );
         }

         return( TRUE );
      }
      else
      if ( bCreateEntity && CreateEntity( vXRA, "Ctrl", zPOS_AFTER ) == 0 )
      {
         SetAttributeFromString( vXRA, "Ctrl", "Tag", *(pzma->m_pzsTag) );
         CreateEntity( vXRA, pzma->m_csERemote, zPOS_AFTER );

         return( TRUE );
      }
   }
#endif
   return( FALSE );
}

zOPER_EXPORT zBOOL OPERATION
PositionOnRemoteCtrl( ZMapAct *pzma,
                      zVIEW   vXRA,
                      zBOOL   bCreateEntity )
{
#ifdef zREMOTE_SERVER
   while ( ResetViewFromSubobject( vXRA ) == 0 )
   {
   }

   if ( PositionOnRemoteCtrlParent( pzma, vXRA, bCreateEntity ) )
      return( TRUE );
#endif

   return( FALSE );
}

zOPER_EXPORT zSHORT OPERATION
SetUpdateFlagForAttribute( zVIEW   vView,
                           zCPCHAR cpcEntity,
                           zCPCHAR cpcAttribute,
                           zBOOL   bValue )
{
   LPVIEWENTITY  lpViewEntity;
   LPVIEWATTRIB  lpViewAttrib;
   LPATTRIBFLAGS lpAttribFlags;

   if ( vView == 0 )
      return( -1 );

   lpViewEntity = MiGetViewEntityForView( vView, cpcEntity );

   if ( lpViewEntity )
   {
      for ( lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewEntity->hFirstOD_Attrib );
            lpViewAttrib;
            lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         if ( zstrcmp( lpViewAttrib->szName, cpcAttribute ) == 0 )
         {
            GetAttributeFlags( &lpAttribFlags, vView, lpViewEntity, lpViewAttrib );
            lpAttribFlags->u.bFlags.bUpdated = bValue;
            return( 0 );
         }
      }
   }

   return( -1 );
}

//./ ADD NAME=SetRemoteZCtrlAttribute
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetRemoteZCtrlAttribute
//                                              01/07/00 Modified: 01/07/00
//
//  PURPOSE:   To set an attribute on an entity in the XRA.
//
//  PARAMETERS: pzma        - The target ctrl pointer
//              cpcEntity   - Name of the Entity containing the attribute
//                            (must either be Ctrl or a direct child of Ctrl).
//              cpcAttribute - Name of the Attribute to set.
//              cpcValue    - pointer to the string from which to set
//                            the control's attribute.
//
//  RETURNS:    0 - Control attribute set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
SetRemoteZCtrlAttribute( ZMapAct *pzma,
                         zCPCHAR cpcEntity,
                         zCPCHAR cpcAttribute,
                         zCPCHAR cpcValue )
{
#ifdef zREMOTE_SERVER

#ifdef DEBUG_ALL
 TraceLineS( "In SetRemoteZCtrlAttribute ", *(pzma->m_pzsTag) );
   zPCHAR pch;
   if ( (pch = zstrstr( cpcValue, "images" )) != 0 &&
        (pch = zstrstr( pch + 1, "images" )) != 0 )
   {
      TraceLineS( "SetRemoteZCtrlAttribute double images: ", cpcValue );
   }
#endif

   ZSocketTask *pZSocketTask = pzma->m_pZSubtask->m_pZSocketTask;
   ZXRA_Item *pXRA;

   if ( pZSocketTask &&
        (pXRA = pZSocketTask->m_pCacheWndList->Find( *(pzma->m_pZSubtask->m_pzsRemoteWndTag),
                                                     pzma->m_pZSubtask, "SetRemoteZCtrlAttribute" )) != 0 &&
        pXRA->m_vXRA )
   {
      zVIEW  vXRA;
      zSHORT nPass;

#ifdef DEBUG_ALL
      zCHAR  szMsg[ 256 ];
      sprintf_s( szMsg, sizeof( szMsg ), "SetRemoteZCtrlAttribute Tag: %s Entity: %s Attribute: %s  Value: %s",
                 *(pzma->m_pzsTag), cpcEntity, cpcAttribute, cpcValue );
      TraceLineS( szMsg, "" );
#endif
      if ( pXRA->m_vXRA_Delta == 0 )
         pXRA->ActivateEmptyDelta( pZSocketTask->m_vRegisteredSubtask, pZSocketTask->m_szAppTag );

      if ( pXRA->m_vXRA_Delta == pXRA->m_vXRA )
         nPass = 1;
      else
         nPass = 0;

      while ( nPass <= 1 )
      {
         if ( nPass == 0 )
            CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA_Delta, m_pZSubtask->m_vDialog );
         else
            CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA, m_pZSubtask->m_vDialog );

         while ( ResetViewFromSubobject( vXRA ) == 0 )
         {
         }

         if ( CheckExistenceOfEntity( vXRA, "Wnd" ) < zCURSOR_SET ||
              SetCursorFirstEntityByString( vXRA, "Wnd", "Tag", *(pzma->m_pZSubtask->m_pzsRemoteWndTag), 0 ) < zCURSOR_SET )
         {
            CreateEntity( vXRA, "Wnd", zPOS_AFTER );
            SetAttributeFromString( vXRA, "Wnd", "Tag", *(pzma->m_pZSubtask->m_pzsRemoteWndTag) );
         }

         if ( PositionOnRemoteCtrl( pzma, vXRA, TRUE ) )
         {
            // Always call SetUpdateFlagForAttribute for XML generation.
         // if ( cpcValue[ 0 ] == 0 && pzma->m_pzsText && **(pzma->m_pzsText) )
         // {
         //    // Force the bUpdated flag to be set.
         //    SetAttributeFromString( vXRA, cpcEntity, cpcAttribute, *(pzma->m_pzsText) );
         // }

            SetAttributeFromString( vXRA, cpcEntity, cpcAttribute, cpcValue );

            // If the Control Text is null, we have to set the Update flag for XML generation.
         // if ( cpcValue[ 0 ] == 0 )
               SetUpdateFlagForAttribute( vXRA, cpcEntity, cpcAttribute, TRUE );
         }

         DropView( vXRA );
         nPass++;
      }
   }

   return( 0 );
#else
   return( -1 );
#endif
}

//./ ADD NAME=GetRemoteZCtrlAttribute
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetRemoteZCtrlAttribute
//                                              01/26/00 Modified: 01/26/00
//
//  PURPOSE:   To get an attribute from an entity in the XRA.
//
//  PARAMETERS: pzma        - The target ctrl pointer
//              cpcEntity   - Name of the Entity containing the attribute
//                            (must either be Ctrl or a direct child of Ctrl).
//              cpcAttribute - Name of the Attribute .
//              pchValue    - pointer to the (return) string which receives
//                            the control's attribute.
//
//  RETURNS:    0 - Control attribute set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
GetRemoteZCtrlAttribute( ZMapAct *pzma,
                         zCPCHAR cpcEntity,
                         zCPCHAR cpcAttribute,
                         zPCHAR  pchValue,
                         zUSHORT uMaxLth )
{
#ifdef zREMOTE_SERVER

#ifdef DEBUG_ALL
   TraceLineS( "In GetRemoteZCtrlAttribute ", *(pzma->m_pzsTag) );
#endif

   ZXRA_Item *pXRA;

// *pchValue = 0;  permit the caller to set the default return value!

   if ( pzma->m_pZSubtask->m_pZSocketTask &&
        (pXRA = pzma->m_pZSubtask->m_pZSocketTask->m_pCacheWndList->Find( *(pzma->m_pZSubtask->m_pzsRemoteWndTag),
                                                                          pzma->m_pZSubtask, "GetRemoteZCtrlAttribute" )) != 0 &&
        (pXRA->m_vXRA || pXRA->m_vXRA_Delta) )
   {
      zVIEW  vXRA;
      zSHORT nPass;

#ifdef DEBUG_ALL
      TraceLineS( "GetRemoteZCtrlAttribute tag: ", *(pzma->m_pzsTag) );
#endif
      if ( pXRA->m_vXRA_Delta == 0 )
         nPass = 1;
      else
         nPass = 0;

      while ( TRUE )
      {
         if ( nPass == 0 )
            CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA_Delta, m_pZSubtask->m_vDialog );
         else
            CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA, m_pZSubtask->m_vDialog );

         while ( ResetViewFromSubobject( vXRA ) == 0 )
         {
         }

         if ( CheckExistenceOfEntity( vXRA, "Wnd" ) < zCURSOR_SET ||
              SetCursorFirstEntityByString( vXRA, "Wnd", "Tag", *(pzma->m_pZSubtask->m_pzsRemoteWndTag), 0 ) < zCURSOR_SET )
         {
            DropView( vXRA );
            if ( nPass == 0 )
            {
               nPass = 1;
               continue;
            }
            else
            {
               return( -1 );
            }
         }

         if ( PositionOnRemoteCtrl( pzma, vXRA, FALSE ) )
         {
            GetVariableFromAttribute( pchValue, 0, zTYPE_STRING, uMaxLth - 1, vXRA, cpcEntity, cpcAttribute, 0, zUSE_DEFAULT_CONTEXT );
         }
         else
         if ( nPass == 0 )
         {
            nPass = 1;
            DropView( vXRA );
            continue;
         }

         DropView( vXRA );
         break;
      }
   }

   return( 0 );
#else
   return( -1 );
#endif
}

//./ ADD NAME=SetRemoteCtrlAttribute
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetRemoteCtrlAttribute
//                                              01/07/00 Modified: 01/07/00
//
//  PURPOSE:   To set an attribute on an entity in the XRA.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              cpcEntity   - Name of the Entity containing the attribute
//                            (must either be Ctrl or a direct child of Ctrl).
//              cpcAttribute - Name of the Attribute to set.
//              cpcValue    - pointer to the string from which to set
//                            the control's attribute.
//
//  RETURNS:    0 - Control attribute set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
SetRemoteCtrlAttribute( zVIEW   vSubtask,
                        zCPCHAR cpcCtrlTag,
                        zCPCHAR cpcEntity,
                        zCPCHAR cpcAttribute,
                        zCPCHAR cpcValue )
{
#ifdef zREMOTE_SERVER
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetRemoteCtrlAttribute ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      return( SetRemoteZCtrlAttribute( pzma, cpcEntity, cpcAttribute, cpcValue ) );
   }
   else
      return( -1 );
#else
   return( -1 );
#endif
}

//./ ADD NAME=GetZCtrlText
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetZCtrlText
//                                              04/01/93 Modified: 12/09/98
//
//  PURPOSE:   To get the text of a control
//
//  PARAMETERS: pzma        - The target ctrl pointer
//              cpcText     - Points to the buffer that will receive the text
//              uMaxTextLth - Specifies the maximum number of characters to
//                            copy to the buffer, including the NULL character.
//                            If the text exceeds this limit, it is truncated.
//
//
//  RETURNS:    0 - got Control text
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GetZCtrlText( ZMapAct *pzma,
              zPCHAR  pchText,
              zUSHORT uMaxTextLth )
{
#ifdef DEBUG_ALL
   TraceLineS( "In GetZCtrlText ", *(pzma->m_pzsTag) );
#endif

   pchText[ 0 ] = 0;
#ifdef zREMOTE_SERVER
   if ( GetRemoteZCtrlAttribute( pzma, "Ctrl", "Text", pchText, uMaxTextLth ) >= 0 )
   {
      if ( pzma->m_pzsText == 0 ||
           zstrcmp( pchText, *(pzma->m_pzsText) ) != 0 )
      {
         mDeleteInit( pzma->m_pzsText );
         pzma->m_pzsText = new CString( pchText );
      }
   }
#else
   if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
   {
      zPCHAR pch = new char[ uMaxTextLth + 2 ];    // GetWindowText difference
      pzma->GetRowColText( pch, uMaxTextLth + 1 ); // between Static and Edit
      strncat_s( pchText, uMaxTextLth, pch, uMaxTextLth - 1 );      // controls force this
      delete [] pch;                               // gimmick.
      if ( pzma->m_pzsText == 0 || zstrcmp( pchText, (*(pzma->m_pzsText)).GetString() ) != 0 )
      {
         mDeleteInit( pzma->m_pzsText );
         pzma->m_pzsText = new CString( pchText );
      }
   }
   else
   if ( pzma->m_pzsText )
   {
      strncat_s( pchText, uMaxTextLth, *(pzma->m_pzsText), uMaxTextLth - 1 );
      return( 1 );
   }
#endif

   return( 0 );
}

//./ ADD NAME=GetCtrlText
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCtrlText
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To retrieve the text from a control
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              pchText     - pointer to a return string in which to
//                            put the text
//              uLth        - Maximum length to return (size of the return
//                            string including the null terminator ... if
//                            1 character is desired, this length should be
//                            2 ... one for the character and one for the
//                            null terminator).
//
//  RETURNS:    0 - Control text retrieved
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetCtrlText( zVIEW   vSubtask,
             zCPCHAR cpcCtrlTag,
             zPCHAR  pchText,
             zUSHORT uLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GetCtrlText ", (zLONG) vSubtask );
#endif

   pchText[ 0 ] = 0;
   if ( cpcCtrlTag && uLth > 0 && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      return( GetZCtrlText( pzma, pchText, uLth - 1 ) );
   }
   else
   {
      return( -1 );
   }
}

//./ ADD NAME=SetZCtrlText
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetZCtrlText
//                                              04/01/93 Modified: 12/09/98
//
//  PURPOSE:   To set the text of a control
//
//  PARAMETERS: pzma        - The target ctrl pointer
//              cpcText     - pointer to the string from which to set
//                            the control's text.
//
//  RETURNS:    0 - Control text set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
SetZCtrlText( ZMapAct *pzma,
              zCPCHAR cpcText )
{
#ifdef DEBUG_ALL
   TraceLineS( "In SetZCtrlText ", cpcText );
#endif

#ifdef zREMOTE_SERVER
   SetRemoteZCtrlAttribute( pzma, "Ctrl", "Text", cpcText );
#else
   if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
   {
      zULONG ulLth = pzma->m_pCtrl->GetWindowTextLength( );
      zPCHAR pch = new char[ ulLth + 2 ];
      pzma->m_pCtrl->GetWindowText( pch, ulLth + 1 );
      if ( *pch == 0 || zstrcmp( pch, cpcText ) )
         pzma->SetRowColText( cpcText );

      delete [] pch;
   }
#endif

   if ( pzma->m_pzsText == 0 || zstrcmp( cpcText, (*(pzma->m_pzsText)).GetString() ) != 0 )
   {
      mDeleteInit( pzma->m_pzsText );
      pzma->m_pzsText = new CString( cpcText );
   }

   return( 0 );
}

//./ ADD NAME=SetCtrlText
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlText
//                                              04/01/93 Modified: 12/09/98
//
//  PURPOSE:   To set the text of a control
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              cpcText     - pointer to the string from which to set
//                            the control's text.
//
//  RETURNS:    0 - Control text set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
SetCtrlText( zVIEW   vSubtask,
             zCPCHAR cpcCtrlTag,
             zCPCHAR cpcText )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlText ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      return( SetZCtrlText( pzma, cpcText ) );
   }
   else
      return( -1 );
}

//./ ADD NAME=SetCtrlTextLth
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlTextLth
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set the maximum length of a control's text
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              uTextLth    - maximum length of the text for a control.
//
//  RETURNS:    0 - Control text length set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
SetCtrlTextLth( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zUSHORT uTextLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlLth ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 && pzma->m_pCtrl )
   {
      if ( pzma->m_pCtrl->m_hWnd )
         SendMessage( pzma->m_pCtrl->m_hWnd, EM_LIMITTEXT, uTextLth, 0L );

      return( 0 );
   }
   else
      return( -1 );
}

//./ ADD NAME=SetCtrlRowColText
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlRowColText
//                                              03/10/99 Modified: 03/10/99
//
//  PURPOSE:   To set the text of a control for a given row/column.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              lRow        - The row for which to set the text.  If lRow
//                            is set to 0, the header text is modified.
//              lColumn     - The column for which to set the text.
//              cpcText     - pointer to the string from which to set
//                            the control's text.
//
//  RETURNS:    0 - Control text set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
SetCtrlRowColText( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zLONG   lRow,
                   zLONG   lColumn,
                   zCPCHAR cpcText )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlRowColText ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      if ( pzma->m_pCtrl )
         pzma->SetRowColText( cpcText, lRow, lColumn );

      return( 0 );
   }
   else
      return( -1 );
}

//./ ADD NAME=GetCtrlRowColText
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCtrlRowColText
//                                              03/10/99 Modified: 03/10/99
//
//  PURPOSE:   To get the text of a control for a given row/column.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              lRow        - The row for which to get the text.  If lRow
//                            is set to 0, the header text is retrieved.
//              lColumn     - The column for which to get the text.
//              pchText     - pointer to the string from which to set
//                            the control's text.
//              uLth        - Maximum length to return (size of the return
//                            string including the null terminator ... if
//                            1 character is desired, this length should be
//                            2 ... one for the character and one for the
//                            null terminator).
//
//  RETURNS:    0 - Control text retrieved
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
GetCtrlRowColText( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zLONG   lRow,
                   zLONG   lColumn,
                   zPCHAR  pchText,
                   zUSHORT uLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GetCtrlRowColText ", (zLONG) vSubtask );
#endif

   *pchText = 0;  // initialize the return value
   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      if ( pzma->m_pCtrl )
         pzma->GetRowColText( pchText, uLth, lRow, lColumn );

      return( 0 );
   }
   else
      return( -1 );
}

//./ ADD NAME=TranslateCtrlValue
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TranslateCtrlValue
//                                              03/10/99 Modified: 03/10/99
//
//  PURPOSE:   To translate the text value of a ctrl for a given row/column.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcCtrlTag  - The unique name of the control.
//              nColumn     - The column for which to translate the text.
//                            This may be zero if column is not required.
//              bToDisplay  - TRUE ==> from internal to display value.
//                            FALSE ==> from display to internal value.
//              pchValue    - pointer to the string from which to set
//                            the control's text.
//              uLth        - Maximum length to return (size of the return
//                            string including the null terminator ... if
//                            1 character is desired, this length should be
//                            2 ... one for the character and one for the
//                            null terminator).
//
//  RETURNS:    0 - Control text translated
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
TranslateCtrlValue( zVIEW   vSubtask,
                    zCPCHAR cpcCtrlTag,
                    zSHORT  nColumn,
                    zSHORT  bToDisplay,
                    zPCHAR  pchValue,
                    zUSHORT uLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In TranslateCtrlValue ", (zLONG) vSubtask );
#endif

   // If unable to translate the value, return the original value.
   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      zCHAR szValue[ 4096 ]; // hardwire max size (256 should be adequate)
      strcpy_s( szValue, sizeof( szValue ), pchValue );
      *pchValue = 0;
      pzma->TranslateValue( szValue, sizeof( szValue ), (zBOOL) bToDisplay, nColumn );
      strncat_s( pchValue, uLth, szValue, uLth - 1 );
      return( 0 );
   }

   return( -1 );
}

zOPER_EXPORT zLONG OPERATION
GetCtrlID( zVIEW   vSubtask,
           zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GetCtrlID ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 && pzma->m_pCtrl )
   {
#ifdef zREMOTE_SERVER
      return( pzma->m_nIdNbr );
#else
      if ( mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
         return( ::GetWindowLong( pzma->m_pCtrl->m_hWnd, GWL_ID ) );
#endif
   }

   return( -1 );
}

//./ ADD NAME=SetCtrlPosition
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlPosition
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To move a control (or window)
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.  If this is
//                            zero, the window associated with the specified
//                            subtask is moved.
//              nX          - width (in pixels or dialog units)
//              nY          - height (in pixels or dialog units)
//              bDlgUnits   - TRUE  ==> size in dialog units
//                          - FALSE ==> size in pixels
//
//  RETURNS:    -1  - Error locating window or control
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
SetCtrlPosition( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zLONG   lX,
                 zLONG   lY,
                 zSHORT  bDlgUnits )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlPosition ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      CRect rect;

      if ( bDlgUnits )
      {
         if ( pZSubtask->m_bUseMapDlgUnits )
         {
            lX = mConvertMapDlgToPixelX( lX );
            lY = mConvertMapDlgToPixelY( lY );
         }
         else
         {
            lX = mConvertDlgUnitToPixelX( lX );
            lY = mConvertDlgUnitToPixelY( lY );
         }
      }

      if ( cpcCtrlTag )
      {
         pzma->Attr.X = lX;
         pzma->Attr.Y = lY;

#ifdef zREMOTE_SERVER
         zCHAR szPos[ 32 ];

         _ltoa_s( lX, szPos );
         SetRemoteZCtrlAttribute( pzma, "Ctrl", "PSDLG_X", szPos );
         _ltoa_s( lY, szPos );
         SetRemoteZCtrlAttribute( pzma, "Ctrl", "PSDLG_Y", szPos );
#else
         if ( mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
         {
            pzma->m_pCtrl->SetWindowPos( 0, lX, lY, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
         }
#endif
      }
      else
      {
#ifdef zREMOTE_SERVER
         zCHAR szPos[ 32 ];

         _ltoa_s( lX, szPos );
         SetRemoteWndAttribute( pZSubtask->m_vDialog, "PSDLG_X", szPos, FALSE );
         _ltoa_s( lY, szPos );
         SetRemoteWndAttribute( pZSubtask->m_vDialog, "PSDLG_Y", szPos, FALSE );
#else
         if ( mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
         {
            pZSubtask->m_pZFWnd->SetWindowPos( 0, lX, lY, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
         }
#endif
      }

      return( 0 );
   }
   else
   {
      return( -1 );
   }
}

//./ ADD NAME=SetCtrlSize
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlSize
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To size a control (or window)
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.  If this is
//                            zero, the window associated with the specified
//                            subtask is sized.
//              nCX         - width (in pixels or dialog units)
//              nCY         - height (in pixels or dialog units)
//              bDlgUnits   - TRUE  ==> size in dialog units
//                          - FALSE ==> size in pixels
//
//  RETURNS:    -1  - Error locating window or control
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
SetCtrlSize( zVIEW   vSubtask,
             zCPCHAR cpcCtrlTag,
             zLONG   lCX,
             zLONG   lCY,
             zSHORT  bDlgUnits )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlSize ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      CRect rect;

      if ( bDlgUnits )
      {
         if ( pZSubtask->m_bUseMapDlgUnits )
         {
            lCX = mConvertMapDlgToPixelX( lCX );
            lCY = mConvertMapDlgToPixelY( lCY );
         }
         else
         {
            lCX = mConvertDlgUnitToPixelX( lCX );
            lCY = mConvertDlgUnitToPixelY( lCY );
         }
      }

      if ( cpcCtrlTag )
      {
#ifdef zREMOTE_SERVER
         zCHAR szSize[ 32 ];

         _ltoa_s( lCX, szSize );
         SetRemoteZCtrlAttribute( pzma, "Ctrl", "SZDLG_X", szSize );
         _ltoa_s( lCY, szSize );
         SetRemoteZCtrlAttribute( pzma, "Ctrl", "SZDLG_Y", szSize );
#else
         if ( mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
         {
            pzma->m_pCtrl->SetWindowPos( 0, 0, 0, lCX, lCY, SWP_NOMOVE | SWP_NOZORDER );
         }
#endif
      }
      else
      {
#ifdef zREMOTE_SERVER
         zCHAR szSize[ 32 ];

         _ltoa_s( lCX, szSize );
         SetRemoteWndAttribute( pZSubtask->m_vDialog, "SZDLG_X", szSize, FALSE );
         _ltoa_s( lCY, szSize );
         SetRemoteWndAttribute( pZSubtask->m_vDialog, "SZDLG_Y", szSize, FALSE );
#else
         if ( mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
         {
            pZSubtask->m_pZFWnd->SetWindowPos( 0, 0, 0, lCX, lCY, SWP_NOMOVE | SWP_NOZORDER );
         }
#endif
      }

      return( 0 );
   }
   else
   {
      return( -1 );
   }
}

//./ ADD NAME=GetCtrlPosition
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCtrlPosition
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the position of a control (or window)
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.  If this is
//                            zero, the window associated with the specified
//                            subtask is moved.
//              nX          - return width (in pixels or dialog units)
//              nY          - return height (in pixels or dialog units)
//              bDlgUnits   - TRUE  ==> size in dialog units
//                          - FALSE ==> size in pixels
//
//  RETURNS:    -1  - Error locating window or control ... nX and nY are
//                    set to 0;
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
GetCtrlPosition( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zPLONG  lX,
                 zPLONG  lY,
                 zSHORT  bDlgUnits )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlPosition ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      CRect rect( 0, 0, 0, 0 );

      if ( cpcCtrlTag && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
      {
         CPoint pt;

         pzma->m_pCtrl->GetWindowRect( rect );
         pt.x = rect.left;
         pt.y = rect.top;
         pZSubtask->m_pZFWnd->ScreenToClient( &pt );
         rect.left = pt.x;
         rect.top = pt.y;
      }
      else
      if ( mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
         pZSubtask->m_pZFWnd->GetWindowRect( rect );

      if ( bDlgUnits )
      {
         if ( pZSubtask->m_bUseMapDlgUnits )
         {
            *lX = mConvertPixelToMapDlgX( rect.left );
            *lY = mConvertPixelToMapDlgY( rect.top );
         }
         else
         {
            *lX = mConvertPixelToDlgUnitX( rect.left );
            *lY = mConvertPixelToDlgUnitY( rect.top );
         }
      }
      else
      {
         *lX = rect.left;
         *lY = rect.top;
      }

      return( 0 );
   }
   else
   {
      *lX = *lY = 0;
      return( -1 );
   }
}

//./ ADD NAME=GetCtrlSize
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCtrlSize
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To size a control (or window)
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.  If this is
//                            zero, the window associated with the specified
//                            subtask is sized.
//              nCX         - return width (in pixels or dialog units)
//              nCY         - return height (in pixels or dialog units)
//              bDlgUnits   - TRUE  ==> size in dialog units
//                          - FALSE ==> size in pixels
//
//  RETURNS:    -1  - Error locating window or control
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetCtrlSize( zVIEW   vSubtask,
             zCPCHAR cpcCtrlTag,
             zPLONG  lCX,
             zPLONG  lCY,
             zSHORT  bDlgUnits )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlSize ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      CRect rect( 0, 0, 0, 0 );

      if ( cpcCtrlTag && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
         pzma->m_pCtrl->GetWindowRect( rect );
      else
      if ( mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
         pZSubtask->m_pZFWnd->GetWindowRect( rect );

      rect.right -= rect.left;
      rect.bottom -= rect.top;
      if ( bDlgUnits )
      {
         if ( pZSubtask->m_bUseMapDlgUnits )
         {
            *lCX = mConvertPixelToMapDlgX( rect.right );
            *lCY = mConvertPixelToMapDlgY( rect.bottom );
         }
         else
         {
            *lCX = mConvertPixelToDlgUnitX( rect.right );
            *lCY = mConvertPixelToDlgUnitY( rect.bottom );
         }
      }
      else
      {
         *lCX = rect.right;
         *lCY = rect.bottom;
      }

      return( 0 );
   }
   else
   {
      *lCX = *lCY = 0;
      return( -1 );
   }
}

//./ ADD NAME=GetCtrlState
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCtrlState
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get enable/visible/check... for a control
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.
//              lStatusType - Status type to get:
//                               zCONTROL_STATUS_ENABLED                1
//                               zCONTROL_STATUS_CHECKED                2
//                               zCONTROL_STATUS_VISIBLE                3
//                               zCONTROL_STATUS_BACKGROUND_COLOR       4
//                               zCONTROL_STATUS_TEXT_COLOR             5
//                               zCONTROL_STATUS_FONT                   6
//                               zCONTROL_STATUS_ENTER                  7
//                               zCONTROL_STATUS_ERROR                  8
//                               zCONTROL_STATUS_AUTOGRAY               9
//                               zCONTROL_STATUS_AUTOMAP_FROM_OI       13
//                               zCONTROL_STATUS_AUTOMAP_TO_OI         14
//                               zCONTROL_STATUS_MAP_ERROR             32
//
//  RETURNS:    -1  - Error locating window
//              otw - state of the control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zLONG OPERATION
GetCtrlState( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              zLONG   lStatusType )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GetCtrlState ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
       return( pzma->GetZCtrlState( lStatusType ) );
   }

   return( -1 );
}

//./ ADD NAME=SetRGB
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetRGB
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set an RGB color (can be used in SetCtrlState)
//
//  PARAMETERS: nRed   - red component of RGB value
//              nGreen - green component of RGB value
//              nBlue  - blue component of RGB value
//
//  RETURNS:    combined RGB value
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zLONG OPERATION
SetRGB( zSHORT nRed,
        zSHORT nGreen,
        zSHORT nBlue )
{
   return( zRGB( nRed, nGreen, nBlue ) );
}

//./ ADD NAME=SetCtrlState
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlState
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set enable/visible/check... for a control
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.
//              lStatusType - Status type to be set:
//                               zCONTROL_STATUS_ENABLED                1
//                               zCONTROL_STATUS_CHECKED                2
//                               zCONTROL_STATUS_VISIBLE                3
//                               zCONTROL_STATUS_BACKGROUND_COLOR       4
//                               zCONTROL_STATUS_TEXT_COLOR             5
//                               zCONTROL_STATUS_FONT                   6
//                               zCONTROL_STATUS_ENTER                  7
//                               zCONTROL_STATUS_ERROR                  8
//                               zCONTROL_STATUS_AUTOGRAY               9
//                               zCONTROL_STATUS_TOPMOST               10
//                               zCONTROL_STATUS_BOTTOMMOST            11
//                               zCONTROL_STATUS_HWND                  12
//                               zCONTROL_STATUS_AUTOMAP_FROM_OI       13
//                               zCONTROL_STATUS_AUTOMAP_TO_OI         14
//                               zCONTROL_STATUS_DISABLE_READONLY      15
//                               zCONTROL_STATUS_REPAINT               16
//                               zCONTROL_STATUS_MAP_ERROR             32
//                               zCONTROL_STATUS_SELECT_ALL            zTAG_LTH
//                               zCONTROL_STATUS_RESET                 34
//              ulValue     - TRUE  - enable, make visible, check,
//                            auto map on ...
//                          - FALSE - disable, make invisible, uncheck, ...
//                          - RGB Color
//
//  RETURNS:    -1  - Error locating window
//              otw - original state of the control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
SetCtrlState( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              zLONG   lStatusType,
              zULONG  ulValue )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlState ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      if ( lStatusType == zCONTROL_STATUS_SELECT_ALL )
         return( pzma->SelectAll( ) );
      else
         return( pzma->SetZCtrlState( lStatusType, ulValue ) );
   }

   return( -1 );
}

//./ ADD NAME=SetCtrlProperty
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlProperty
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set properties for a control
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.
//              lPropertyType - Property type to be set:
//                               zCONTROL_PROPERTY_INTEGER_DATA         1
//                               zCONTROL_PROPERTY_STRING_DATA          2
//                               zCONTROL_PROPERTY_FONT                 6
//                               zCONTROL_PROPERTY_SUBCTRL_JUSTIFY      7
//              ulNumericProperty - numeric property to set
//              cpcStringProperty - string property to set
//
//  RETURNS:    -1  - Error locating window
//              otw - original state of the control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
SetCtrlProperty( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zLONG   lPropertyType,
                 zULONG  ulNumericProperty,
                 zCPCHAR cpcStringProperty )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlProperty ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      return( pzma->SetZCtrlProperty( lPropertyType, ulNumericProperty, cpcStringProperty ) );
   }

   return( -1 );
}

//./ ADD NAME=GetCtrlProperty
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCtrlProperty
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set properties for a control
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.
//              lPropertyType - Property type to be set:
//                               zCONTROL_PROPERTY_INTEGER_DATA         1
//                               zCONTROL_PROPERTY_STRING_DATA          2
//                               zCONTROL_PROPERTY_FONT                 6
//              pulNumericProperty - numeric property to get
//              pchStringProperty - string property to get
//
//  RETURNS:    -1  - Error locating window
//              otw - original state of the control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
GetCtrlProperty( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zLONG   lPropertyType,
                 zPULONG pulNumericProperty,
                 zPCHAR  pchStringProperty,
                 zLONG   lMaxLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GetCtrlProperty ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      return( pzma->GetZCtrlProperty( lPropertyType, pulNumericProperty, pchStringProperty, lMaxLth ) );
   }

   return( -1 );
}

//./ ADD NAME=SetFocusToCtrl
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetFocusToCtrl
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set focus to a control
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    -1  - Error locating window or control
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
SetFocusToCtrl( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   if ( cpcCtrlTag )
      TraceLineS( "SetFocusToCtrl ", cpcCtrlTag );
   else
      TraceLineI( "SetFocusToCtrl ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && *cpcCtrlTag == 0 )
      cpcCtrlTag = 0;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      if ( pzma == 0 )
         pzma = pZSubtask->m_pzmaWithFocus;

      if ( pzma )
      {
         pZSubtask->m_pzmaFirstFocus = pzma;  // force a new first control
         pZSubtask->m_pzmaWithFocus = pzma;
         if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
            ::SetFocus( pzma->m_pCtrl->m_hWnd );
      }

      return( 0 );
   }
   else
   {
      return( -1 );
   }
}

//./ ADD NAME=FindCtrlForFocus
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: FindCtrlForFocus
//                                          1993.04.01 Modified: 2003.02.19
//
//  PURPOSE:   To set focus to a control (next or previous).
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              pchReturnCtrlTag - The name of the control that gets focus
//                                (should allocate at least zTAG_LTH bytes).
//              bBackward   - Locate the previous control.
//              bSetFocus   - Actually set focus to the control if it
//                            is a valid hWnd, otherwise the user will
//                            set focus using FindCtrlForFocus.
//
//  RETURNS:    -1  - Error locating window ... or no control has focus
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
FindCtrlForFocus( zVIEW  vSubtask,
                  zPCHAR pchReturnCtrlTag,
                  zSHORT bBackward,
                  zSHORT bSetFocus )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In FindCtrlForFocus ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pZMIXCtrl && pZSubtask->m_pzmaWithFocus )
      {
         ZMapAct *pzma;
         zLONG   lIdx = pZSubtask->m_pzmaWithFocus->m_lIdx;
         zLONG   lCnt = pZSubtask->m_pZMIXCtrl->GetCount( );

         if ( bBackward )
         {
            if ( lIdx <= 1 )
               lIdx = lCnt;
            else
               lIdx--;
         }
         else
         {
            if ( lIdx >= lCnt )
               lIdx = 1;
            else
               lIdx++;
         }

         pzma = (ZMapAct *) pZSubtask->m_pZMIXCtrl->GetAt( lIdx );
         while ( pzma && pzma != pZSubtask->m_pzmaWithFocus )
         {
            if ( (pzma->m_ulMapActFlags & zMAPACT_VISIBLE) && (pzma->m_ulMapActFlags & zMAPACT_ENABLED) && (pzma->m_ulMapActFlags & zMAPACT_FOCUS_ENABLED) )
            {
               if ( bSetFocus && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
                  pzma->m_pCtrl->SetFocus( );

               if ( pzma->m_pzsTag )
               {
                  strcpy_s( pchReturnCtrlTag, zTAG_LTH, *(pzma->m_pzsTag) );
               }

               return( 0 );
            }

            lIdx = pzma->m_lIdx;
            if ( bBackward )
            {
               if ( lIdx <= 1 )
                  lIdx = lCnt;
               else
                  lIdx--;
            }
            else
            {
               if ( lIdx >= lCnt )
                  lIdx = 1;
               else
                  lIdx++;
            }

            pzma = (ZMapAct *) pZSubtask->m_pZMIXCtrl->GetAt( lIdx );
         }
      }
   }

   return( -1 );
}

//./ ADD NAME=SetFocusToNextCtrl
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetFocusToNextCtrl
//                                          1993.04.01 Modified: 2003.02.19
//
//  PURPOSE:   To set focus to a control (next or previous).
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              pchReturnCtrlTag - The name of the control that gets focus
//                                (should allocate at least zTAG_LTH bytes).
//
//  RETURNS:    -1  - Error locating window ... or no control has focus
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
SetFocusToNextCtrl( zVIEW  vSubtask,
                    zPCHAR pchReturnCtrlTag )
{
   return( FindCtrlForFocus( vSubtask, pchReturnCtrlTag, FALSE, TRUE ) );
}

//./ ADD NAME=GetCtrlWithFocus
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCtrlWithFocus
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the tag of the control that currently has focus
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              pchReturnCtrlTag - The name of the control that has focus
//                                (should allocate at least zTAG_LTH bytes).
//
//  RETURNS:    -1  - Error locating window ... or no control has focus
//               0  - OK
//               1  - This control was the last one known to have focus in
//                    this window, but no longer has the focus.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
GetCtrlWithFocus( zVIEW  vSubtask,
                  zPCHAR pchReturnCtrlTag )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In GetCtrlWithFocus ", (zLONG) vSubtask );
#endif

   *pchReturnCtrlTag = 0;
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pzmaWithFocus && pZSubtask->m_pzmaWithFocus->m_pzsTag )
      {
#ifdef zREMOTE_SERVER
         strcpy_s( pchReturnCtrlTag, zTAG_LTH, *(pZSubtask->m_pzmaWithFocus->m_pzsTag) );
         return( 0 );
#else
         HWND hWndFocus = ::GetFocus( );

         strcpy_s( pchReturnCtrlTag, zTAG_LTH, *(pZSubtask->m_pzmaWithFocus->m_pzsTag) );
         if ( pZSubtask->m_pzmaWithFocus->m_pCtrl && mIs_hWnd( pZSubtask->m_pzmaWithFocus->m_pCtrl->m_hWnd ) &&
              (hWndFocus == pZSubtask->m_pzmaWithFocus->m_pCtrl->m_hWnd || hWndFocus == pZSubtask->m_pzmaWithFocus->m_hWndChild) )
         {
            return( 0 );
         }
         else
         {
            return( 1 );
         }
#endif
      }
   }

   return( -1 );
}

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
GetCtrlFont( zVIEW    vSubtask,
             zCPCHAR  cpcCtrlTag,
             PCHARSET pFont )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In GetCtrlFont ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->GetFont( pFont );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlFontTag( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zCPCHAR cpcFontTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlFontTag ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetFontTag( cpcFontTag, 256 );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlFontBold( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zSHORT  bBold )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlFontBold ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetFontBold( bBold );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlFontItalic( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zSHORT  bItalic )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlFontItalic ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetFontItalic( bItalic );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlFontOverstrike( zVIEW   vSubtask,
                       zCPCHAR cpcCtrlTag,
                       zSHORT  bOverstrike )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlFontOverstrike ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetFontOverstrike( bOverstrike );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlFontUnderline( zVIEW   vSubtask,
                      zCPCHAR cpcCtrlTag,
                      zSHORT  bUnderline )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlFontUnderline ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetFontUnderline( bUnderline );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlFontSize( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zSHORT  nFontSize )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlFontSize ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetFontSize( nFontSize );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlBorder( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zCPCHAR cpcText )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlBorder ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetBorder( cpcText, 256 );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

#if 0
// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlParent( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               PGUIOBJ pParent )
{
   ZSubtask    *pZSubtask;
   ZMapAct *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlParent ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetParent( pParent );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}

// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlOwner( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              PGUIOBJ pOwner )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlOwner ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->m_pCtrl->SetParent( pOwner );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}
#endif

// not sure we want to support this yet???
zOPER_EXPORT zSHORT OPERATION
SetCtrlMousePointer( zVIEW   vSubtask,
                     zCPCHAR cpcCtrlTag,
                     zUSHORT uPointerID )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

#ifdef DEBUG_ALL
   TraceLineI( "In SetCtrlMousePointer ", (zLONG) vSubtask );
#endif

   if ( cpcCtrlTag && GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
//??? pzma->m_pCtrl->SetPointer( uPointerID );
      return( 0 );
   }
   else
   {
      return( -1 );
   }
}

//./ ADD NAME=SetCtrlTranslationList
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlTranslationList
//                                              02/14/02 Modified: 02/14/02
//
//  PURPOSE:    This operation sets a list of translation values for
//              a control.  This permits a runtime domain for simple
//              lists.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cpcPairedList - Paired list of translation values (semi-colon
//                            separated internal;display string values).
//              nCol        - Column for which to translate values (should
//                            be 1 for controls with no columns).
//
//  RETURNS:    0 - Paired list successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
SetCtrlTranslationList( zVIEW   vSubtask,
                        zCPCHAR cpcCtrlTag,
                        zCPCHAR cpcPairedList,
                        zSHORT  nCol )
{
   ASSERT( nCol > 0 );

   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      pzma->SetTranslationList( cpcPairedList, nCol - 1 );
      return( 0 );
   }

   return( -1 );
}
