/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdlgm.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of "C interface" routines into the
// C++ functionality for menus available through the Zeidon driver.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.03.20    DKS   Z10
//    Memory leak cleaned up when removing menu item.
//
// 2001.07.26    DKS   Z10
//    Remove of menu items in a menu (popup or primary) corrected.
//
// 2001.05.22    DKS   Z10
//    Permit add/remove of menu items in a popup (tracking) menu.
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
// 2001.02.07    DKS   WEB
//    Fixed SetOptionState for WEB.
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added Mapping List.
//
// 1998.08.13    DKS
//    Fix to SetOption states in recursive situations.
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


//./ ADD NAME=SetDrawMenuBar
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetDrawMenuBar
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the tag of the last popup menu invoked
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the menu.
//              bSetRedraw  - TRUE = DrawMenuBar will be called
//                            FALSE = Turn off DrawMenuBar
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
SetDrawMenuBar( zVIEW  vSubtask,
                zSHORT bSetRedraw )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( bSetRedraw )
         pZSubtask->m_nDrawMenuBar++;
      else
         pZSubtask->m_nDrawMenuBar--;

      if ( pZSubtask->m_nDrawMenuBar >= 0 )
      {
         if ( pZSubtask->m_pZFWnd )
            pZSubtask->m_pZFWnd->DrawMenuBar( );
      }

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=GetLastPopupMenu
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetLastPopupMenu
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the tag of the last popup menu invoked
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the popup menu.
//              pchReturnPopupTag - The return buffer into which to copy
//                            the popup menu's tag.  This buffer should
//                            be at least zTAG_LTH bytes long.
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
GetLastPopupMenu( zVIEW  vSubtask,
                  zPCHAR pchReturnPopupTag )
{
   ZSubtask *pZSubtask;

   *pchReturnPopupTag = 0;
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pzsPopupTag )
         strcpy_s( pchReturnPopupTag, zTAG_LTH, *(pZSubtask->m_pzsPopupTag) );

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=GetLastCommandTag
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetLastCommandTag
//                                              11/20/97 Modified: 11/20/97
//
//  PURPOSE:   To get the most recent tag of the control or option that
//             triggered an action.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the action.
//              pchReturnTag - The return buffer into which to copy the
//                            control or option tag.  This buffer should
//                            be at least zTAG_LTH bytes long.
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
GetLastCommandTag( zVIEW  vSubtask,
                   zPCHAR pchReturnTag )
{
   ZSubtask *pZSubtask;

   *pchReturnTag = 0;
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pzsLastTag )
         strcpy_s( pchReturnTag, zTAG_LTH, *(pZSubtask->m_pzsLastTag) );

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=ReplacePrimaryMenu
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ReplacePrimaryMenu
//                                              08/31/00 Modified: 08/31/00
//
//  PURPOSE:   To replace the primary menu for a window
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the menu.
//              cpcMenuTag  - The tag of the new menu.
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//               1  - Menu not replaced (menu destroyed for window)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
ReplacePrimaryMenu( zVIEW   vSubtask,
                    zCPCHAR cpcMenuTag )
{
#ifdef zREMOTE_SERVER

   SetRemoteWndAttribute( vSubtask, "PrimaryMenu", cpcMenuTag );
   return( 0 );

#else

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      pZSubtask->CreateZeidonMenu( cpcMenuTag );
      if ( pZSubtask->m_pZBinMenu )
         return( 0 );
      else
         return( 1 );
   }
   else
      return( -1 );

#endif
}

zSHORT
fnRemoveOptionFromMenu( zVIEW   vSubtask,
                        zCPCHAR cpcOptionTag,
                        zBOOL   bPopup )
{
   if ( cpcOptionTag == 0 || *cpcOptionTag == 0 )
      return( -2 );

#ifdef zREMOTE_SERVER

//?SetRemoteWndAttribute( vSubtask, "RemoveOption", cpcMenuTag );
   return( 0 );

#else

   ZSubtask *pZSubtask;
   ZOption  *pZOption;

   if ( (bPopup &&
         GetWindowAndPopupOption( &pZSubtask, &pZOption,
                                  vSubtask, cpcOptionTag ) == 0) ||
        (bPopup == FALSE &&
         GetWindowAndOption( &pZSubtask, &pZOption,
                             vSubtask, cpcOptionTag ) == 0) )
   {
      // Reset the relative Option positions of sibling Options.
      ZOption *pCurrOption;
      zULONG  ulOptCnt;
      zULONG  k;

      if ( bPopup )
         ulOptCnt = pZSubtask->m_pZBinPopup->GetCount( );
      else
         ulOptCnt = pZSubtask->m_pZBinMenu->GetCount( );

      for ( k = 0; k < ulOptCnt; k++ )
      {
         if ( bPopup )
            pCurrOption = (ZOption *) *(pZSubtask->m_pZBinPopup->m_pvNode + k);
         else
            pCurrOption = (ZOption *) *(pZSubtask->m_pZBinMenu->m_pvNode + k);

         if ( pZOption->m_pParent == pCurrOption->m_pParent &&
              pCurrOption->m_uOptionPos > pZOption->m_uOptionPos )
         {
            pCurrOption->m_uOptionPos--;
         }
      }

      zBOOL bRC = ::RemoveMenu( pZOption->m_hMenu,
                                pZOption->m_uOptionPos, MF_BYPOSITION  );

      zLONG lIdx;

      if ( bPopup )
         lIdx = pZSubtask->m_pZBinPopup->RemoveNode( pZOption );
      else
         lIdx = pZSubtask->m_pZBinMenu->RemoveNode( pZOption );

      mDeleteInit( pZOption );
      if ( bRC )
      {
         if ( bPopup == FALSE && pZSubtask->m_pZFWnd )
         {
            if ( pZSubtask->m_nDrawMenuBar >= 0 )
               pZSubtask->m_pZFWnd->DrawMenuBar( );

            return( (zSHORT) lIdx );
         }
         else
         {
            return( 0 );
         }
      }
   }

   return( -1 );

#endif
}

//  lFlag   - Flag controlling how the option will be added:
//            0 - as Next option (default - after sibling)
//            1 - as Prev option (before sibling)
//            2 - as First option within a popup
//            4 - as Last option within a popup
//           16 - add option after first adding a separator
//           32 - option is checked
//           64 - option is disabled
//          128 - delete child options first (requires ParentTag)
//          512 - option is a new "popup"
//  RETURNS: -1 - Error locating window
//           -2 - Option not added (couldn't locate specified parent)
//           -3 - Option not added (couldn't locate specified sibling)
//           -4 - Specified parent is not the correct parent of option
//           -5 - Empty Sibling tag requires First or Last flag
//           -6 - Invalid (mutually exclusive) Flag combination
//           -7 - Prev or Next requires a Sibling tag
//           -8 - First or Last requires an empty Sibling tag
//           -9 - Menu item not added
//         >= 0 - Id of menu option added
zSHORT
fnAddOptionToMenu( zVIEW   vSubtask,
                   zCPCHAR cpcParentTag,
                   zCPCHAR cpcSiblingTag,
                   zCPCHAR cpcOptionTag,
                   zCPCHAR cpcOptionText,
                   zCPCHAR cpcActionTag,
                   zLONG   lFlag,
                   zBOOL   bPopup )
{
   if ( cpcOptionTag == 0 || *cpcOptionTag == 0 )
      return( -2 );

#ifdef zREMOTE_SERVER

//?SetRemoteWndAttribute( vSubtask, "AddOption", cpcMenuTag );
   return( 0 );

#else

   zLONG lTemp = (lFlag & 0x00000007);

   if ( lTemp == 3 || lTemp == 5 || lTemp == 6 || lTemp == 7 )
      return( -6 );

   if ( lTemp < 2 && (cpcSiblingTag == 0 || *cpcSiblingTag == 0) )
      return( -7 );

   if ( lTemp >= 2 && cpcSiblingTag && *cpcSiblingTag )
      return( -8 );

   ZSubtask *pZSubtask;
   ZOption  *pZOption;
   HMENU    hMenu = 0;
   zULONG   ulOptCnt;
   zULONG   k;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        ((bPopup && pZSubtask->m_pZBinPopup) ||
         (bPopup == FALSE && pZSubtask->m_pZBinMenu)) )
   {
      ZOption *pParent = 0;
      ZOption *pSibling = 0;

      if ( cpcParentTag && *cpcParentTag )
      {
         CString csSeek( cpcParentTag );

         ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
         if ( pZSubtaskMain )
            pZSubtask = pZSubtaskMain;

         if ( bPopup )
         {
            pParent = (ZOption *) pZSubtask->m_pZBinPopup->SeekNode( &csSeek );
         }
         else
         {
            pParent = (ZOption *) pZSubtask->m_pZBinMenu->SeekNode( &csSeek );
         }

         if ( pParent == 0 )
         {
            TraceLine( "fnAddOptionToMenu %s.%s.%s   Cannot find Option Tag: %s",
                       *(pZSubtask->m_pzsDlgTag), *(pZSubtask->m_pzsWndTag),
                       bPopup ? *(pZSubtask->m_pzsPopupTag) : *(pZSubtask->m_pzsMenuTag),
                       csSeek );
            return( -2 );
         }

         if ( lFlag & 0x00000080 ) // 128 - delete child options first (requires ParentTag)
         {
            zBOOL bDone = FALSE;

            while ( bDone == FALSE )
            {
               bDone = TRUE;
               if ( bPopup )
                  ulOptCnt = pZSubtask->m_pZBinPopup->GetCount( );
               else
                  ulOptCnt = pZSubtask->m_pZBinMenu->GetCount( );

               for ( k = 0; k < ulOptCnt; k++ )
               {
                  if ( bPopup )
                     pZOption = (ZOption *) *(pZSubtask->m_pZBinPopup->m_pvNode + k);
                  else
                     pZOption = (ZOption *) *(pZSubtask->m_pZBinMenu->m_pvNode + k);

                  if ( pParent == pZOption->m_pParent )
                  {
                     fnRemoveOptionFromMenu( vSubtask, *(pZOption->m_pzsTag), bPopup );
                     bDone = FALSE;
                     break;  // out of inner loop
                  }
               }
            }
         }

         hMenu = pParent->m_hMenu;
      }
      else
      {
         if ( bPopup )
            hMenu = pZSubtask->m_pTrackMenu->m_hMenu;
         else
            hMenu = pZSubtask->m_pMenu->m_hMenu;
      }

      if ( cpcSiblingTag && *cpcSiblingTag )
      {
         CString csSeek( cpcSiblingTag );
         if ( bPopup )
         {
            pSibling = (ZOption *)
                                 pZSubtask->m_pZBinPopup->SeekNode( &csSeek );
         }
         else
         {
            pSibling = (ZOption *)
                                 pZSubtask->m_pZBinMenu->SeekNode( &csSeek );
         }

         if ( pSibling == 0 )
            return( -3 );

         if ( pParent )
         {
            if ( pParent != pSibling->m_pParent )
               return( -4 );
         }

         if ( pSibling->m_pParent )
            hMenu = pSibling->m_hMenu;
      }
      else
      if ( (lFlag & 6) == 0 )  // must specify first or last
      {
         return( -5 );
      }

      // Find the next available Option ID and reset the relative Option
      // positions of sibling Options.
      zUSHORT uNextID = 0;
      zUSHORT uOptionPos;

      if ( pSibling )
      {
         if ( lFlag & 0x00000001 )  // Prev
            uOptionPos = pSibling->m_uOptionPos;
         else
            uOptionPos = pSibling->m_uOptionPos + 1;
      }
      else
         uOptionPos = 0;

      if ( bPopup )
         ulOptCnt = pZSubtask->m_pZBinPopup->GetCount( );
      else
         ulOptCnt = pZSubtask->m_pZBinMenu->GetCount( );

      for ( k = 0; k < ulOptCnt; k++ )
      {
         if ( bPopup )
            pZOption = (ZOption *) *(pZSubtask->m_pZBinPopup->m_pvNode + k);
         else
            pZOption = (ZOption *) *(pZSubtask->m_pZBinMenu->m_pvNode + k);

         if ( pZOption->m_uOptionID >= uNextID )
            uNextID = pZOption->m_uOptionID + 1;

         if ( pSibling )
         {
            if ( pSibling->m_pParent == pZOption->m_pParent &&
                 pZOption->m_uOptionPos >= uOptionPos )
            {
               if ( lFlag & 0x00000004 )
                  pZOption->m_uOptionPos += 2;
               else
                  pZOption->m_uOptionPos++;
            }
         }
         else
         if ( pParent && pZOption->m_pParent == pParent )
         {
            if ( lFlag & 0x00000002 )  // First
               pZOption->m_uOptionPos++;
            else
            if ( lFlag & 0x00000004 )  // Last
               uOptionPos = pZOption->m_uOptionPos + 1;
         }
      }

   // TraceLine( "fnAddOptionToMenu %s.%s.%s   Option: %s",
   //            *(pZSubtask->m_pzsDlgTag), *(pZSubtask->m_pzsWndTag),
   //            bPopup ? *(pZSubtask->m_pzsPopupTag) : *(pZSubtask->m_pzsMenuTag),
   //            cpcOptionText );
      // Put this option into the driver's menu list for quick reference.
      pZOption = new ZOption( uNextID, cpcOptionTag,
                              cpcOptionText, cpcActionTag,
                              (lFlag & 0x00000032) ? TRUE : FALSE,
                              pParent, pZSubtask );

      if ( lFlag & 0x00000010 )
      {
         pZOption->m_uSeparator = 1;
         ::InsertMenu( hMenu, uOptionPos++,
                       MF_BYPOSITION | MF_SEPARATOR, -1, 0 );
      }
      else
         pZOption->m_uSeparator = 0;

      if ( pParent )
         pZOption->m_uParentID = pParent->m_uOptionID;
      else
         pZOption->m_uParentID = 0;

      pZOption->m_uOptionPos = uOptionPos;

      if ( (bPopup &&
            pZSubtask->m_pZBinPopup->InsertNode( pZOption ) == 0) ||
           (bPopup == FALSE &&
            pZSubtask->m_pZBinMenu->InsertNode( pZOption ) == 0) )
      {
         TraceLineS( "(drvr) Menu Error (DUPLICATE Menu Item tag) ==> ",
                     cpcOptionTag );

         return( -1 );
      }

      zBOOL   bRC;
      zUSHORT uFlags = MF_BYPOSITION | MF_STRING;
      if ( lFlag & 0x00000200 )  // 512 ==> option is a new "popup"
      {
         CMenu *pSubMenu = new CMenu( );
         pSubMenu->CreateMenu( );
         pZOption->m_hMenu = pSubMenu->Detach( );
         delete( pSubMenu );

         if ( lFlag & 0x00000040 )
            uFlags |= MF_DISABLED;

         uFlags |= MF_POPUP;
         pZOption->m_bParent = TRUE;
         bRC = ::InsertMenu( hMenu, uOptionPos, uFlags,
                            (UINT) pZOption->m_hMenu, cpcOptionText );
      }
      else
      {
         pZOption->m_bParent = FALSE;
         if ( lFlag & 0x00000020 )
            uFlags |= MF_CHECKED;

         if ( lFlag & 0x00000040 )
            uFlags |= MF_DISABLED;

         pZOption->m_hMenu = hMenu;
         bRC = ::InsertMenu( hMenu, uOptionPos, uFlags,
                             uNextID, cpcOptionText );
      }

      if ( bRC )
      {
         if ( bPopup == FALSE && pZSubtask->m_pZFWnd &&
              pZSubtask->m_nDrawMenuBar >= 0 )
         {
            pZSubtask->m_pZFWnd->DrawMenuBar( );
         }

         return( 0 );
      }
      else
      {
         TraceLineS( "(drvr) Menu Error - could not add tag ==> ",
                     cpcOptionTag );
         return( -9 );
      }
   }
   else
      return( -1 );

#endif
}

//./ ADD NAME=AddOptionToMenu
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: AddOptionToMenu
//                                              08/31/00 Modified: 08/31/00
//
//  PURPOSE:   To add an option to the current primary menu for a window.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the menu.
//              cpcParentTag - Tag of the parent (popup) option (may be null
//                             if the option to be added is at the top level).
//              cpcSiblingTag - Tag of the adjacent option at the same level.
//              cpcOptionTag - Tag of the option to be added.
//              cpcOptionText - Text of the option to be added.
//              cpcActionTag - Tag of action associated with the option.
//              lFlag        - Flag controlling how the option will be added:
//                             0 - as Next option (default - after sibling)
//                             1 - as Prev option (before sibling)
//                             2 - as First option within a popup
//                             4 - as Last option within a popup
//                            16 - add option after first adding a separator
//                            32 - option is checked
//                            64 - option is disabled
//                           512 - option is a new "popup"
//
//  RETURNS:    -1 - Error locating window
//              -2 - Option not added (couldn't locate specified parent)
//              -3 - Option not added (couldn't locate specified sibling)
//              -4 - Specified parent is not the correct parent of option
//              -5 - Empty Sibling tag requires First or Last flag
//              -6 - Invalid (mutually exclusive) Flag combination
//              -7 - Prev or Next requires a Sibling tag
//              -8 - First or Last requires an empty Sibling tag
//              -9 - Menu item not added
//            >= 0 - Id of menu option added
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
AddOptionToMenu( zVIEW   vSubtask,
                 zCPCHAR cpcParentTag,
                 zCPCHAR cpcSiblingTag,
                 zCPCHAR cpcOptionTag,
                 zCPCHAR cpcOptionText,
                 zCPCHAR cpcActionTag,
                 zLONG   lFlag )
{
   return( fnAddOptionToMenu( vSubtask, cpcParentTag, cpcSiblingTag,
                              cpcOptionTag, cpcOptionText, cpcActionTag,
                              lFlag, FALSE ) );
}

//./ ADD NAME=AddOptionToPopupMenu
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: AddOptionToPopupMenu
//                                              05/22/01 Modified: 05/22/01
//
//  PURPOSE:   To add an option to the current popup menu for a window.
//
//  PARAMETERS: vSubtask      - The subtask view for the window containing
//                              the menu.
//              cpcParentTag  - Tag of the parent (popup) option (may be null
//                              if the option to be added is at the top level).
//              cpcSiblingTag - Tag of the adjacent option at the same level.
//              cpcOptionTag  - Tag of the option to be added.
//              cpcOptionText - Text of the option to be added.
//              cpcActionTag  - Tag of action associated with the option.
//              lFlag         - Flag controlling how the option will be added:
//                              0 - as Next option (default - after sibling)
//                              1 - as Prev option (before sibling)
//                              2 - as First option within a popup
//                              4 - as Last option within a popup
//                             16 - add option after first adding a separator
//                             32 - option is checked
//                             64 - option is disabled
//                            512 - option is a new "popup"
//
//  RETURNS:    -1 - Error locating window
//              -2 - Option not added (couldn't locate specified parent)
//              -3 - Option not added (couldn't locate specified sibling)
//              -4 - Specified parent is not the correct parent of option
//              -5 - Empty Sibling tag requires First or Last flag
//              -6 - Invalid (mutually exclusive) Flag combination
//              -7 - Prev or Next requires a Sibling tag
//              -8 - First or Last requires an empty Sibling tag
//              -9 - Menu item not added
//            >= 0 - Id of menu option added
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
AddOptionToPopupMenu( zVIEW   vSubtask,
                      zCPCHAR cpcParentTag,
                      zCPCHAR cpcSiblingTag,
                      zCPCHAR cpcOptionTag,
                      zCPCHAR cpcOptionText,
                      zCPCHAR cpcActionTag,
                      zLONG   lFlag )
{
   return( fnAddOptionToMenu( vSubtask, cpcParentTag, cpcSiblingTag,
                              cpcOptionTag, cpcOptionText, cpcActionTag,
                              lFlag, TRUE ) );
}

//./ ADD NAME=RemoveOptionFromMenu
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RemoveOptionFromMenu
//                                              08/31/00 Modified: 08/31/00
//
//  PURPOSE:   To remove an option from the current primary menu for a window.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the menu.
//              cpcOptionTag - Tag of the option to be removed.
//
//  RETURNS:    -1 - Error locating window
//              -2 - Error locating menu option
//            >= 0 - Index of option removed
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
RemoveOptionFromMenu( zVIEW   vSubtask,
                      zCPCHAR cpcOptionTag )
{
   return( fnRemoveOptionFromMenu( vSubtask, cpcOptionTag, FALSE ) );
}

//./ ADD NAME=RemoveOptionFromPopupMenu
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RemoveOptionFromPopupMenu
//                                              05/22/01 Modified: 05/22/01
//
//  PURPOSE:   To remove an option from the current popup menu for a window.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the menu.
//              cpcOptionTag - Tag of the option to be removed.
//
//  RETURNS:    -1 - Error locating window
//              -2 - Error locating menu option
//            >= 0 - Index of option removed
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
RemoveOptionFromPopupMenu( zVIEW   vSubtask,
                           zCPCHAR cpcOptionTag )
{
   return( fnRemoveOptionFromMenu( vSubtask, cpcOptionTag, TRUE ) );
}

//./ ADD NAME=CreateTrackingPopupMenu
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CreateTrackingPopupMenu
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To create a popup menu at a given location
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the popup menu.
//              cpcPopupTag - The tag of the popup menu.
//              nXCoord     - screen (or window) x-coordinate at which to
//                            place the popup menu.
//              nYCoord     - screen (or window) y-coordinate at which to
//                            place the popup menu.
//              bClientCoords - TRUE  ==> client window coords
//                            - FALSE ==> physical screen coords
//              bDlgUnits   - TRUE  ==> coordinates are in dialog units
//                          - FALSE ==> coordinates are in pixels
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//               1  - Popup not created
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
CreateTrackingPopupMenu( zVIEW   vSubtask,
                         zCPCHAR cpcPopupTag,
                         zLONG   lXCoord,
                         zLONG   lYCoord,
                         zSHORT  bClientCoords,
                         zSHORT  bDlgUnits )
{
#ifdef zREMOTE_SERVER

   SetRemoteWndAttribute( vSubtask, "ActivePopup", cpcPopupTag );
   return( 0 );

#else

   ZSubtask *pZSubtask;
   CPoint pt;
   CPoint pt1;
   DWORD  dwPoint;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      pt.x = lXCoord;
      pt.y = lYCoord;
      if ( bClientCoords || (pt.x >= 0 && pt.y >= 0) )
      {
         if ( bDlgUnits )
         {
            if ( pZSubtask->m_bUseMapDlgUnits )
            {
               pt.x = mConvertMapDlgToPixelX( pt.x );
               pt.y = mConvertMapDlgToPixelY( pt.y );
            }
            else
            {
               pt.x = mConvertDlgUnitToPixelX( pt.x );
               pt.y = mConvertDlgUnitToPixelY( pt.y );
            }
         }

         if ( bClientCoords )
         {
            pZSubtask->m_pZFWnd->ClientToScreen( &pt );
         }

         pt1 = pt;
         dwPoint = MAKELONG( pt.x, pt.y );
      }
      else
      {
         dwPoint = GetMessagePos( );
         if ( pZSubtask->m_pEventParms )
            dwPoint = (DWORD) pZSubtask->m_pEventParms;

#ifdef __WIN32__
         pt1 = CPoint( dwPoint );
#else
         pt1 = dwPoint;
#endif
      }

      if ( SetCursorFirstEntityByString( pZSubtask->m_vDialog,
                                         szlMenu, szlTag,
                                         cpcPopupTag, 0 ) < zCURSOR_SET )
      {
         ZSubtask *pZSubtaskTemp = pZSubtask->GetSplitterMainSubtask( );
         if ( pZSubtaskTemp )
            pZSubtask = pZSubtaskTemp;
      }

      mDeleteInit( pZSubtask->m_pzsPopupTag );
      pZSubtask->m_pzsPopupTag = new CString( cpcPopupTag );
      pZSubtask->m_pZFWnd->
                        PostMessage( zZEIDON_POSTED_POPUP_MENU, 0, dwPoint );
   // pZSubtask->CreateZeidonPopupMenu( cpcPopupTag, pt1 );
   // if ( pZSubtask->m_pTrackMenu )
         return( 0 );
   // else
   //    return( 1 );
   }
   else
      return( -1 );
#endif
}

// Local function to get id corresponding to standard menu item.
zUSHORT
GetStandardOptionIdFromName( zCPCHAR cpcOptionName )
{
   zUSHORT uId = 0;
   TraceLineS( "GetStandardOptionIdFromName unsupported", "" );

#if 0    //???
   if ( *cpcOptionName == 'F' )
   {
      if ( zstrcmp( cpcOptionName, "FileNew" ) == 0 )
      {
         uId = CM_FILENEW;
      }
      else
      if ( zstrcmp( cpcOptionName, "FileOpen" ) == 0 )
      {
         uId = CM_FILEOPEN;
      }
      else
      if ( zstrcmp( cpcOptionName, "FileClose" ) == 0 )
      {
         uId = CM_FILECLOSE;
      }
      else
      if ( zstrcmp( cpcOptionName, "FileSave" ) == 0 )
      {
         uId = CM_FILESAVE;
      }
      else
      if ( zstrcmp( cpcOptionName, "FileSaveAs" ) == 0 )
      {
         uId = CM_FILESAVEAS;
      }
      else
      if ( zstrcmp( cpcOptionName, "FileExit" ) == 0 )
      {
         uId = CM_EXIT;
      }
   }
   else
   if ( *cpcOptionName == 'E' )
   {
      if ( zstrcmp( cpcOptionName, "EditCut" ) == 0 )
      {
         uId = CM_EDITCUT;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditCopy" ) == 0 )
      {
         uId = CM_EDITCOPY;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditPaste" ) == 0 )
      {
         uId = CM_EDITPASTE;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditDelete" ) == 0 )
      {
         uId = CM_EDITDELETE;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditClear" ) == 0 )
      {
         uId = CM_EDITCLEAR;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditUndo" ) == 0 )
      {
         uId = CM_EDITUNDO;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditRedo" ) == 0 )
      {
         uId = CM_EDITREDO;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditFind" ) == 0 )
      {
         uId = CM_EDITFIND;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditReplace" ) == 0 )
      {
         uId = CM_EDITREPLACE;
      }
      else
      if ( zstrcmp( cpcOptionName, "EditFindNext" ) == 0 )
      {
         uId = CM_EDITFINDNEXT;
      }
   }
   else
   if ( *cpcOptionName == 'C' &&
        zstrcmp( cpcOptionName, "CloseAll" ) == 0 )
   {
      uId = CM_CLOSECHILDREN;
   }
   else
   if ( *cpcOptionName == 'H' &&
        zstrcmp( cpcOptionName, "HelpAbout" ) == 0 )
   {
      uId = CM_HELPABOUT;
   }
   else
   {
      uId = 0;
   }
#endif

   return( uId );
}

zLONG
fnGetOptionState( zVIEW   vSubtask,
                  zCPCHAR cpcOptionTag,
                  zLONG   lStatusType,
                  zBOOL   bPopup )
{
   ZSubtask *pZSubtask;
   ZOption  *pZOption;

#ifdef DEBUG_ALL
   TraceLineI( "In GetOptionState ", (zLONG) vSubtask );
#endif

   if ( (bPopup &&
         GetWindowAndPopupOption( &pZSubtask, &pZOption,
                                  vSubtask, cpcOptionTag ) == 0) ||
        (bPopup == FALSE &&
         GetWindowAndOption( &pZSubtask, &pZOption,
                             vSubtask, cpcOptionTag ) == 0) )
   {
      if ( pZOption->m_uOptionID == 0 )
      {
         TraceLineS( "GetOptionState Option not found ", cpcOptionTag );
         return( 0 );
      }

      if ( lStatusType == zOPTION_STATUS_ENABLED )
      {
         if ( pZOption->m_bVisible )
            return( pZOption->m_bEnabled );
         else
            return( FALSE );
      }
      else
      if ( lStatusType == zOPTION_STATUS_CHECKED )
         return( pZOption->m_bChecked );
      else
      if ( lStatusType == zOPTION_STATUS_VISIBLE )
         return( pZOption->m_bVisible );
      else
      if ( lStatusType == zOPTION_STATUS_TEXT )
      {
//       CString cs;
//
//       for ( zSHORT k = 0; k < 10; k++ )
//       {
//          pZSubtask->m_pMenu->GetMenuString( k, cs, MF_BYPOSITION );
//          TraceLineI( cs, k );
//       }
//
         return( (zLONG)(zCPCHAR) *(pZOption->m_pzsText) );
      }
   }
   else
   if ( pZSubtask && bPopup == FALSE && pZSubtask->m_pMenu )
   {
      zUSHORT uId = GetStandardOptionIdFromName( cpcOptionTag );
      if ( uId )
      {
         zUSHORT uState;

         uState = pZSubtask->m_pMenu->GetMenuState( uId, MF_BYCOMMAND );
         if ( lStatusType == zOPTION_STATUS_ENABLED )
            return( (uState & MF_GRAYED) == 0 );
         else
         if ( lStatusType == zOPTION_STATUS_CHECKED )
            return( uState & MF_CHECKED );
      }
   }

   return( -1 );
}

//./ ADD NAME=GetOptionState
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetOptionState
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get enable/check ... for a menu option
//
//  PARAMETERS: vSubtask     - The subtask view for the window containing
//                             the option.
//              cpcOptionTag - The unique name of the option
//              lStatusType  - Status type to get:
//                                  zOPTION_STATUS_ENABLED            1
//                                  zOPTION_STATUS_CHECKED            2
//                                  zOPTION_STATUS_VISIBLE            3
//                                  zOPTION_STATUS_TEXT               4
//
//  RETURNS:    -1  - Error locating window or option or
//                    invalid status type
//              otw - original state of the option ... in the case of
//                    text, pointer to a string containing the menu
//                    item text.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zLONG OPERATION
GetOptionState( zVIEW   vSubtask,
                zCPCHAR cpcOptionTag,
                zLONG   lStatusType )
{
   return( fnGetOptionState( vSubtask, cpcOptionTag, lStatusType, FALSE ) );
}

//./ ADD NAME=GetPopupOptionState
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetPopupOptionState
//                                              05/22/01 Modified: 05/22/01
//
//  PURPOSE:   To get enable/check ... for a popup menu option
//
//  PARAMETERS: vSubtask     - The subtask view for the window containing
//                             the option.
//              cpcOptionTag - The unique name of the option
//              lStatusType  - Status type to get:
//                                  zOPTION_STATUS_ENABLED            1
//                                  zOPTION_STATUS_CHECKED            2
//                                  zOPTION_STATUS_VISIBLE            3
//                                  zOPTION_STATUS_TEXT               4
//
//  RETURNS:    -1  - Error locating window or option or
//                    invalid status type
//              otw - original state of the option ... in the case of
//                    text, pointer to a string containing the menu
//                    item text.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zLONG OPERATION
GetPopupOptionState( zVIEW   vSubtask,
                     zCPCHAR cpcOptionTag,
                     zLONG   lStatusType )
{
   return( fnGetOptionState( vSubtask, cpcOptionTag, lStatusType, TRUE ) );
}

zLONG
fnSetOptionState( zVIEW   vSubtask,
                  zCPCHAR cpcOptionTag,
                  zLONG   lStatusType,
                  zULONG  ulValue,
                  zBOOL   bPopup )
{
   ZSubtask *pZSubtask;
   ZOption  *pZOption;

   if ( (bPopup &&
         GetWindowAndPopupOption( &pZSubtask, &pZOption,
                                  vSubtask, cpcOptionTag ) == 0) ||
        (bPopup == FALSE &&
         GetWindowAndOption( &pZSubtask, &pZOption,
                             vSubtask, cpcOptionTag ) == 0) )
   {
      return( fnSetSuboptionStates( pZSubtask, pZOption,
                                    lStatusType, ulValue, bPopup ) );
   }

   return( -1 );
}

//./ ADD NAME=SetOptionState
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetOptionState
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set enable/delete/check ... for a menu option.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the option.
//              cpcOptionTag - The tag given to the option when initially
//                            painted.
//              lStatusType - Status type to be set:
//                                  zOPTION_STATUS_ENABLED            1
//                                  zOPTION_STATUS_CHECKED            2
//                                  zOPTION_STATUS_VISIBLE            3
//                                  zOPTION_STATUS_TEXT               4
//              lValue      - TRUE  - enable, make visible, check, ...
//                          - FALSE - disable, make invisible, uncheck, ...
//                            except in the case of TEXT in which the
//                            value is really a pointer to a string.
//
//  RETURNS:    -1  - Error locating window
//              -2  - Unrecognized state
//              otw - original state of the option
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
SetOptionState( zVIEW   vSubtask,
                zCPCHAR cpcOptionTag,
                zLONG   lStatusType,
                zULONG  ulValue )
{
#ifdef DEBUG_ALL
   TraceLineI( "In SetOptionState ", (zLONG) vSubtask );
#endif

   return( fnSetOptionState( vSubtask, cpcOptionTag,
                             lStatusType, ulValue, FALSE ) );
}

//./ ADD NAME=SetPopupOptionState
// Source Module=zdrdlgm.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetPopupOptionState
//                                              05/22/01 Modified: 05/22/01
//
//  PURPOSE:   To set enable/delete/check ... for a popup menu option.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the option.
//              cpcOptionTag - The tag given to the option when initially
//                            painted.
//              lStatusType - Status type to be set:
//                                  zOPTION_STATUS_ENABLED            1
//                                  zOPTION_STATUS_CHECKED            2
//                                  zOPTION_STATUS_VISIBLE            3
//                                  zOPTION_STATUS_TEXT               4
//              lValue      - TRUE  - enable, make visible, check, ...
//                          - FALSE - disable, make invisible, uncheck, ...
//                            except in the case of TEXT in which the
//                            value is really a pointer to a string.
//
//  RETURNS:    -1  - Error locating window
//              -2  - Unrecognized state
//              otw - original state of the option
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
SetPopupOptionState( zVIEW   vSubtask,
                     zCPCHAR cpcOptionTag,
                     zLONG   lStatusType,
                     zULONG  ulValue )
{
#ifdef DEBUG_ALL
   TraceLineI( "In SetPopupOptionState ", (zLONG) vSubtask );
#endif

   return( fnSetOptionState( vSubtask, cpcOptionTag,
                             lStatusType, ulValue, TRUE ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// The ModifyMenu function changes an existing menu item.  This function is
// used to specify the content, appearance, and behavior of the menu item.
// The ModifyMenu function has been superseded by the SetMenuItemInfo
// function.  You can still use ModifyMenu, however, if you do not need any
// of the extended features of SetMenuItemInfo.
//
// BOOL
// ModifyMenu( HMENU hMenu,      // handle of menu
//             UINT  uPosition,  // menu item to modify
//             UINT  uFlags,     // menu item flags
//             UINT  uIDNewItem, // menu item identifier
//                               // or handle of drop-down
//                               // menu or submenu
//             LPCTSTR lpNewItem // menu item content );
//
// Parameters:
//    hMenu - Identifies the menu to be changed.
//    uPosition - Specifies the menu item to be changed, as determined
//                by the uFlags parameter.
//    uFlags - Specifies flags that control the interpretation of the
//       uPosition parameter and the content, appearance, and behavior
//       of the menu item.  This parameter must be a combination of
//       one of the following required values and at least one of the
//       values listed in the following Remarks section.
//        ú MF_BYCOMMAND - Indicates that the uPosition parameter
//             gives the identifier of the menu item.  The
//             MF_BYCOMMAND flag is the default if neither the
//             MF_BYCOMMAND nor MF_BYPOSITION flag is specified.
//
//        ú MF_BYPOSITION - Indicates that the uPosition parameter gives
//             the zero-based relative position of the menu item.
//
//    uIDNewItem - Specifies either the identifier of the modified menu
//       item or, if the uFlags parameter has the MF_POPUP flag set,
//       the handle of the drop-down menu or submenu.
//
//    lpNewItem - Points to the content of the changed menu item. The
//       interpretation of this parameter depends on whether the uFlags
//       parameter includes the MF_BITMAP, MF_OWNERDRAW, or MF_STRING flag.
//        ú MF_BITMAP - Contains a bitmap handle.
//        ú MF_OWNERDRAW - Contains a 32-bit value supplied by an
//             application that is used to maintain additional data related
//             to the menu item. The value is in the itemData member of
//             the structure pointed to by the lparam parameter of the
//             WM_MEASUREITEM or WM_DRAWITEM messages sent when the menu
//             item is created or its appearance is updated.
//        ú MF_STRING - Contains a pointer to a null-terminated string
//             (the default).
//
// Return Values: If the function succeeds, the return value is nonzero.
//    If the function fails, the return value is zero.  To get extended
//    error information, call GetLastError.
//
// Remarks: If ModifyMenu replaces a menu item that opens a drop-down
//    menu or submenu, the function destroys the old drop-down menu or
//    submenu and frees the memory used by it.
//
//    The application must call the DrawMenuBar function whenever a menu
//    changes, whether or not the menu is in a displayed window.  To change
//    the attributes of existing menu items, it is much faster to use the
//    CheckMenuItem and EnableMenuItem functions.
//
//    The following list describes the flags that may be set in the uFlags
//    parameter:
//
//     ú MF_BITMAP - Uses a bitmap as the menu item.  The lpNewItem parameter
//          contains the handle of the bitmap.
//     ú MF_BYCOMMAND - Indicates that the uPosition parameter specifies the
//          identifier of the menu item (the default).
//     ú MF_BYPOSITION - Indicates that the uPosition parameter specifies the
//          zero-based relative position of the new menu item.
//     ú MF_CHECKED - Places a check mark next to the item.  If your
//          application provides check mark bitmaps (see the
//          SetMenuItemBitmaps function), this flag displays a checked
//          bitmap next to the menu item.
//     ú MF_DISABLED - Disables the menu item so that it cannot be selected,
//          but this flag does not gray it.
//     ú MF_ENABLED - Enables the menu item so that it can be selected and
//          restores it from its grayed state.
//     ú MF_GRAYED - Disables the menu item and grays it so that it cannot be
//          selected.
//     ú MF_MENUBARBREAK - Functions the same as the MF_MENUBREAK flag
//          for a menu bar. For a drop-down menu, submenu, or shortcut menu,
//          the new column is separated from the old column by a vertical
//          line.
//     ú MF_MENUBREAK - Places the item on a new line (for menu bars) or in
//          a new column (for a drop-down menu, submenu, or shortcut menu)
//          without separating columns.
//     ú MF_OWNERDRAW - Specifies that the item is an owner-drawn item.
//          Before the menu is displayed for the first time, the window
//          that owns the menu receives a WM_MEASUREITEM message to retrieve
//          the width and height of the menu item.  The WM_DRAWITEM message
//          is then sent to the window procedure of the owner window whenever
//          the appearance of the menu item must be updated.
//     ú MF_POPUP - Specifies that the menu item opens a drop-down menu or
//          submenu.  The uIDNewItem parameter specifies the handle of the
//          drop-down menu or submenu.  This flag is used to add a menu name
//          to a menu bar or a menu item that opens a submenu to a drop-down
//          menu, submenu, or shortcut menu.
//     ú MF_SEPARATOR - Draws a horizontal dividing line.  This flag is used
//          only in a drop-down menu, submenu, or shortcut menu.  The line
//          cannot be grayed, disabled, or highlighted.  The lpNewItem and
//          uIDNewItem parameters are ignored.
//     ú MF_STRING - Specifies that the menu item is a text string; the
//          lpNewItem parameter points to the string.
//     ú MF_UNCHECKED - Does not place a check mark next to the item (the
//          default).  If your application supplies check mark bitmaps (see
//          the SetMenuItemBitmaps function), this flag displays an unchecked
//          bitmap next to the menu item.
//
// The following groups of flags cannot be used together:
//
// ú MF_BYCOMMAND and MF_BYPOSITION
// ú MF_DISABLED, MF_ENABLED, and MF_GRAYED
// ú MF_BITMAP, MF_STRING, MF_OWNERDRAW, and MF_SEPARATOR
// ú MF_MENUBARBREAK and MF_MENUBREAK
// ú MF_CHECKED and MF_UNCHECKED
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// CMenu::ModifyMenu
//
// BOOL
// ModifyMenu( UINT uPosition,
//             UINT uFlags,
//             UINT uIDNewItem = 0,
//             LPCTSTR lpszNewItem = NULL );
//
// Return Value:  Nonzero if the function is successful; otherwise 0.
//
// Parameters:
//    uPosition - Specifies the menu item to be changed.  The uFlags
//       parameter can be used to interpret uPosition in the following ways:
// uFlags
//    MF_BYCOMMAND - Specifies that the parameter gives the command
//       ID of the existing menu item.  This is the default if neither
//       MF_BYCOMMAND nor MF_BYPOSITION is set.
//
//    MF_BYPOSITION - Specifies that the parameter gives the position of
//       the existing menu item.  The first item is at position 0.
//
// uFlags - Specifies how uPosition is interpreted and gives information
//    about the changes to be made to the menu item. For a list of flags
//    that may be set, see the AppendMenu member function.
//
// uIDNewItem - Specifies either the command ID of the modified menu item
//    or, if uFlags is set to MF_POPUP, the menu handle (HMENU) of a pop-up
//    menu.  The uIDNewItem parameter is ignored (not needed) if uFlags is
//    set to MF_SEPARATOR.
//
// lpszNewItem - Specifies the content of the new menu item.  The uFlags
//    parameter can be used to interpret lpszNewItem in the following ways:
//
//    uFlags - Interpretation of lpszNewItem
//       MF_OWNERDRAW - Contains an application-supplied 32-bit value that
//          the application can use to maintain additional data associated
//          with the menu item.  This 32-bit value is available to the
//          application when it processes MF_MEASUREITEM and MF_DRAWITEM.
//
//       MF_STRING - Contains a long pointer to a null-terminated string or
//          to a CString.
//
//       MF_SEPARATOR - The lpszNewItem parameter is ignored (not needed).
//
// pBmp - Points to a CBitmap object that will be used as the menu item.
//
// Remarks:  Changes an existing menu item at the position specified by
//    uPosition.  The application specifies the new state of the menu item
//    by setting values in uFlags.  If this function replaces a pop-up menu
//    associated with the menu item, it destroys the old pop-up menu and
//    frees the memory used by the pop-up menu.
//
// When uIDNewItem specifies a pop-up menu, it becomes part of the menu in
// which it is inserted.  If that menu is destroyed, the inserted menu will
// also be destroyed.  An inserted menu should be detached from a CMenu
// object to avoid conflict.
//
// Whenever a menu that resides in a window is changed (whether or not the
// window is displayed), the application should call CWnd::DrawMenuBar.  To
// change the attributes of existing menu items, it is much faster to use
// the CheckMenuItem and EnableMenuItem member functions.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// CMenu::InsertMenu
//
// BOOL
// InsertMenu( UINT nPosition,
//             UINT uFlags,
//             UINT uIDNewItem = 0,
//             LPCTSTR lpszNewItem = NULL );
// Return Value : Nonzero if the function is successful; otherwise 0.
// Parameters:
//    nPosition - Specifies the menu item before which the new menu item is
//       to be inserted.  The uFlags parameter can be used to interpret
//       nPosition in the following ways:
//    uFlags - Interpretation of nPosition:
//       MF_BYCOMMAND - Specifies that the parameter gives the command ID of
//          the existing menu item.  This is the default if neither
//          MF_BYCOMMAND nor MF_BYPOSITION is set.
//       MF_BYPOSITION - Specifies that the parameter gives the position of
//          the existing menu item.  The first item is at position 0.  If
//          nPosition is -1, the new menu item is appended to the end of the
//          menu.
//    uFlags - Specifies how nPosition is interpreted and specifies
//       information about the state of the new menu item when it is
//       added to the menu.  For a list of the flags that may be set,
//       see the AppendMenu member function.  To specify more than one
//       value, use the bitwise OR operator to combine them with the
//       MF_BYCOMMAND or MF_BYPOSITION flag.
//
//    uIDNewItem - Specifies either the command ID of the new menu item or,
//       if uFlags is set to MF_POPUP, the menu handle (HMENU) of the
//       pop-up menu.  The uIDNewItem parameter is ignored (not needed)
//       if uFlags is set to MF_SEPARATOR.
//
//    lpszNewItem - Specifies the content of the new menu item. uFlags can
//       be used to interpret lpszNewItem in the following ways:
//
//    uFlags - Interpretation of lpszNewItem
//       MF_OWNERDRAW - Contains an application-supplied 32-bit value that
//          the application can use to maintain additional data associated
//          with the menu item.  This 32-bit value is available to the
//          application in the itemData member of the structure supplied by
//          the WM_MEASUREITEM and WM_DRAWITEM messages.  These messages are
//          sent when the menu item is initially displayed or is changed.
//
//       MF_STRING - Contains a long pointer to a null-terminated string.
//          This is the default interpretation.
//
//       MF_SEPARATOR - The lpszNewItem parameter is ignored (not needed).
//
//       pBmp - Points to a CBitmap object that will be used as the menu item.
//
// Remarks:  Inserts a new menu item at the position specified by nPosition
//    and moves other items down the menu.  The application can specify the
//    state of the menu item by setting values in uFlags.
//
//    Whenever a menu that resides in a window is changed (whether or not
//    the window is displayed), the application should call CWnd::DrawMenuBar.
//
//    When uIDNewItem specifies a pop-up menu, it becomes part of the menu
//    in which it is inserted.  If that menu is destroyed, the inserted menu
//    will also be destroyed.  An inserted menu should be detached from a
//    CMenu object to avoid conflict.
//
//    If the active multiple document interface (MDI) child window is
//    maximized and an application inserts a pop-up menu into the MDI
//    application's menu by calling this function and specifying the
//    MF_BYPOSITION flag, the menu is inserted one position farther left
//    than expected.  This happens because the Control menu of the active
//    MDI child window is inserted into the first position of the MDI
//    frame window's menu bar.  To position the menu properly, the
//    application must add 1 to the position value that would otherwise
//    be used.  An application can use the WM_MDIGETACTIVE message to
//    determine whether the currently active child window is maximized.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnSetSuboptionStates( ZSubtask *pZSubtask,
                      ZOption  *pZOption,
                      zLONG    lStatusType,
                      zULONG   ulValue,
                      zBOOL    bPopup )
{
   CMenu  *pMenu;
   zSHORT nRC;

#ifdef DEBUG_ALL
   TraceLineS( "SetOptionState Menu Item tag: ", *(pZOption->m_pzsTag) );
#endif

   if ( pZOption->m_uOptionID == 0 )
      return( 0 );

   if ( pZOption->m_bParent &&
        (lStatusType == zOPTION_STATUS_ENABLED ||
         lStatusType == zOPTION_STATUS_VISIBLE) )
   {
      // If this menu item is parent, do this for all options that have the
      // same parent.
      ZOption *pZOptCurr;
      zULONG  ulOptCnt;
      zULONG  k;

      if ( bPopup )
         ulOptCnt = pZSubtask->m_pZBinPopup->GetCount( );
      else
         ulOptCnt = pZSubtask->m_pZBinMenu->GetCount( );

      for ( k = 1; k <= ulOptCnt; k++ )
      {
         if ( bPopup )
            pZOptCurr = (ZOption *) pZSubtask->m_pZBinPopup->GetAt( k );
         else
            pZOptCurr = (ZOption *) pZSubtask->m_pZBinMenu->GetAt( k );

         if ( pZOption->m_uOptionID == pZOptCurr->m_uParentID )
         {
            nRC = fnSetSuboptionStates( pZSubtask, pZOptCurr,
                                        lStatusType, ulValue, bPopup );
            if ( nRC == -1 )
               return( -1 );
         }
      }
   }

   if ( bPopup )
      pMenu = pZSubtask->m_pTrackMenu;
   else
      pMenu = pZSubtask->m_pMenu;

      if ( lStatusType == zOPTION_STATUS_ENABLED )
   {
      pZOption->m_bEnabled = (zBOOL) ulValue;
      if ( pZOption->m_bVisible )
      {
//       TraceLineS( "SetSubOptionState EnableMenuItem", "" );
#ifdef zREMOTE_SERVER
         nRC = SetRemoteZOptAttribute( pZOption, "Opt", "Enabled",
                                       ulValue ? "Y" : "N" );
#else
         nRC = pMenu->EnableMenuItem( pZOption->m_uOptionID,
                         MF_BYCOMMAND | (ulValue ? MF_ENABLED : MF_GRAYED) );
         if ( nRC == -1 )
         {
            TraceLineI( "(drvr) Enable Menu Item failed ",
                        pZOption->m_uOptionID );
            TraceLineS( "Menu Item tag ", *(pZOption->m_pzsTag) );
         }
         else
         {
//          TraceLineI( "(drvr) Enable Menu Item succeeded ",
//                      pZOption->m_uOptionID );
         if ( pZSubtask->m_nDrawMenuBar >= 0 )
            pZSubtask->m_pZFWnd->DrawMenuBar( );
         }
#endif
      }
      else
         TraceLineI( "(drvr) Enable Menu Item failed (hidden) ",
                     pZOption->m_uOptionID );

      return( nRC );
   }

   if ( lStatusType == zOPTION_STATUS_CHECKED )
   {
      pZOption->m_bChecked = (zBOOL) ulValue;
      if ( pZOption->m_bVisible )
      {
#ifdef zREMOTE_SERVER
         nRC = SetRemoteZOptAttribute( pZOption, "Opt", "Checked",
                                       ulValue ? "Y" : "N" );
#else
         nRC = pMenu->CheckMenuItem( pZOption->m_uOptionID,
                      MF_BYCOMMAND | (ulValue ? MF_CHECKED : MF_UNCHECKED) );
         if ( nRC == -1 )
         {
            TraceLineI( "(drvr) Check Menu Item failed ",
                        pZOption->m_uOptionID );
            TraceLineS( "Menu Item tag ", *(pZOption->m_pzsTag) );
         }
         else
         {
//          TraceLineI( "(drvr) Check Menu Item succeeded ",
//                      pZOption->m_uOptionID );
            if ( pZSubtask->m_nDrawMenuBar >= 0 )
               pZSubtask->m_pZFWnd->DrawMenuBar( );
         }
#endif
      }

      return( nRC );
   }

   if ( lStatusType == zOPTION_STATUS_VISIBLE )
   {
      // Hiding (by removing) menu item.
      if ( ulValue == 0 && pZOption->m_bVisible == TRUE )
      {
         pZOption->m_bVisible = FALSE;

#ifdef zREMOTE_SERVER
         nRC = SetRemoteZOptAttribute( pZOption, "Opt", "Visible", "N" );
#else
         if ( pZOption->RemoveMenu( pZSubtask ) == FALSE )
         {
            TraceLineI( "(drvr) Hide Menu Item failed ",
                        pZOption->m_uOptionID );
            TraceLineS( "Menu Item tag ", *(pZOption->m_pzsTag) );
            return( -1 );
         }
         else
         {
            TraceLineI( "(drvr) Hide Menu Item succeeded ",
                        pZOption->m_uOptionID );
            if ( pZSubtask->m_nDrawMenuBar >= 0 )
               pZSubtask->m_pZFWnd->DrawMenuBar( );
         }
#endif
         return( 0 );
      }
      else  // Showing the menu item
      if ( ulValue && pZOption->m_bVisible == FALSE )
      {
         pZOption->m_bVisible = TRUE;

#ifdef zREMOTE_SERVER
         nRC = SetRemoteZOptAttribute( pZOption, "Opt", "Visible", "Y" );
#else
         if ( pZOption->m_pParent )
            pMenu = pZOption->m_pParent->GetMenu( pZSubtask );

         if ( pZOption->InsertMenu( pMenu, pZSubtask ) == FALSE )
         {
            TraceLineI( "(drvr) Insert Menu Item failed ",
                        pZOption->m_uOptionID );
            TraceLineS( "Menu Item tag ", *(pZOption->m_pzsTag) );
            return( -1 );
         }
         else
            TraceLineI( "(drvr) Insert Menu Item succeeded ",
                        pZOption->m_uOptionID );
#endif
         return( 0 );
      }
   }

   if ( lStatusType == zOPTION_STATUS_TEXT )
   {
      mDeleteInit( pZOption->m_pzsText );
      pZOption->m_pzsText = new CString( (zCPCHAR) ulValue );

#ifdef zREMOTE_SERVER
      nRC = SetRemoteZOptAttribute( pZOption, "Opt", "Text", (zCPCHAR) ulValue );
#else
      nRC = pMenu->ModifyMenu( pZOption->m_uOptionID,
                               MF_BYCOMMAND | MF_STRING,
                               pZOption->m_uOptionID,
                               *(pZOption->m_pzsText) );
      if ( nRC == 0 )
      {
         TraceLineI( "(drvr) Modify Menu Item failed ", pZOption->m_uOptionID );
         TraceLineS( "Menu Item tag ", *(pZOption->m_pzsTag) );
         return( -1 );
      }
//    else
//       TraceLineS( "(drvr) Modify Menu Item succeeded ", (zPCHAR) ulValue );
#endif

      return( 0 );
   }

   return( -2 );  // unrecognized state
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zBOOL OPERATION
PositionOnRemoteOptParent( ZOption *pOpt,
                           zVIEW   vXRA,
                           zBOOL   bCreateEntity )
{
#ifdef zREMOTE_SERVER
   zBOOL bRC = TRUE;

   if ( pOpt->m_pParent )
   {
      bRC = PositionOnRemoteOptParent( pOpt->m_pParent,
                                       vXRA, bCreateEntity );
      if ( bRC )
         SetViewToSubobject( vXRA, "OptOpt" );
   }

   if ( bRC )
   {
      if ( CheckExistenceOfEntity( vXRA, "Opt" ) >= zCURSOR_SET &&
           SetCursorFirstEntityByString( vXRA, "Opt", "Tag",
                                         *(pOpt->m_pzsTag), 0 ) == 0 )
      {
         if ( pOpt->m_bParent &&
              CheckExistenceOfEntity( vXRA, "Menu" ) < zCURSOR_SET )
         {
            CreateEntity( vXRA, "Menu", zPOS_AFTER );
         }

         return( TRUE );
      }
      else
      if ( bCreateEntity && CreateEntity( vXRA, "Opt", zPOS_AFTER ) == 0 )
      {
         SetAttributeFromString( vXRA, "Opt", "Tag", *(pOpt->m_pzsTag) );
         if ( pOpt->m_bParent )
            CreateEntity( vXRA, "Menu", zPOS_AFTER );

         return( TRUE );
      }
   }
#endif
   return( FALSE );
}

zOPER_EXPORT zBOOL OPERATION
PositionOnRemoteOpt( ZOption *pOpt,
                     zVIEW   vXRA,
                     zBOOL   bCreateEntity )
{
#ifdef zREMOTE_SERVER
   while ( ResetViewFromSubobject( vXRA ) == 0 )
   {
   }

   if ( PositionOnRemoteOptParent( pOpt, vXRA, bCreateEntity ) )
      return( TRUE );
#endif

   return( FALSE );
}

//./ ADD NAME=SetRemoteZOptAttribute
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetRemoteZOptAttribute
//                                              01/07/00 Modified: 01/07/00
//
//  PURPOSE:   To set an attribute on an entity in the XRA.
//
//  PARAMETERS: pOpt        - The target ctrl pointer
//              cpcEntity   - Name of the Entity containing the attribute
//                            (must either be Opt or a direct child of Opt).
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
SetRemoteZOptAttribute( ZOption *pOpt,
                        zCPCHAR cpcEntity,
                        zCPCHAR cpcAttribute,
                        zCPCHAR cpcValue )
{
#ifdef zREMOTE_SERVER

#ifdef DEBUG_ALL
 TraceLineS( "In SetRemoteZOptAttribute ", *(pOpt->m_pzsTag) );
   zPCHAR pch;
   if ( (pch = zstrstr( cpcValue, "images" )) != 0 &&
        (pch = zstrstr( pch + 1, "images" )) != 0 )
   {
      TraceLineS( "SetRemoteZOptAttribute double images: ", cpcValue );
   }
#endif

   ZSocketTask *pZSocketTask = pOpt->m_pZSubtask->m_pZSocketTask;
   ZXRA_Item *pXRA;

   if ( pZSocketTask &&
        (pXRA = pZSocketTask->
             m_pCacheWndList->Find( *(pOpt->m_pZSubtask->m_pzsRemoteWndTag),
                                    pOpt->m_pZSubtask,
                                    "SetRemoteZOptAttribute" )) != 0 &&
        pXRA->m_vXRA )
   {
      zVIEW  vXRA;
      zSHORT nPass;

#ifdef DEBUG_ALL
      zCHAR  szMsg[ 256 ];
      sprintf_s( szMsg, zsizeof( szMsg ), "SetRemoteZOptAttribute Tag: %s Entity: %s "
                       "Attribute: %s  Value: %s",
                *(pOpt->m_pzsTag), cpcEntity, cpcAttribute, cpcValue );
      TraceLineS( szMsg, "" );
#endif
      if ( pXRA->m_vXRA_Delta == 0 )
         pXRA->ActivateEmptyDelta( pZSocketTask->m_vRegisteredSubtask,
                                   pZSocketTask->m_szAppTag );

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
              SetCursorFirstEntityByString( vXRA, "Wnd", "Tag",
                                      *(pOpt->m_pZSubtask->m_pzsRemoteWndTag),
                                      0 ) < zCURSOR_SET )
         {
            CreateEntity( vXRA, "Wnd", zPOS_AFTER );
            SetAttributeFromString( vXRA, "Wnd", "Tag",
                                    *(pOpt->m_pZSubtask->m_pzsRemoteWndTag) );
         }

         if ( CheckExistenceOfEntity( vXRA, "Menubar" ) < zCURSOR_SET ||
              SetCursorFirstEntityByString( vXRA, "Menubar", "Tag",
                                      *(pOpt->m_pZSubtask->m_pzsMenuTag),
                                      0 ) < zCURSOR_SET )
         {
            CreateEntity( vXRA, "Menubar", zPOS_AFTER );
            SetAttributeFromString( vXRA, "Menubar", "Tag",
                                    *(pOpt->m_pZSubtask->m_pzsMenuTag) );
         }

         if ( PositionOnRemoteOpt( pOpt, vXRA, TRUE ) )
         {
            if ( cpcValue[ 0 ] == 0 && pOpt->m_pzsText && **(pOpt->m_pzsText) )
            {
            // LPVIEWENTITY lpViewEntity;
            // LPVIEWATTRIB lpViewAttrib;

            // lpViewEntity = MiGetViewEntityForView( vApp, *pzsEntity );
            // if ( lpViewEntity && lpViewAttrib )
            // {
            //    GetAttributeFlags( &lpAttribFlags, vOI,
            //                       lpViewEntity, lpViewAttrib );
            //    lpAttribFlags->u.bFlags.bUpdated = TRUE;
            // }

               // Force the bUpdated flag to be set.
               SetAttributeFromString( vXRA, cpcEntity,
                                       cpcAttribute, *(pOpt->m_pzsText) );
            }

            SetAttributeFromString( vXRA, cpcEntity, cpcAttribute, cpcValue );
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

//./ ADD NAME=GetRemoteZOptAttribute
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetRemoteZOptAttribute
//                                              01/26/00 Modified: 01/26/00
//
//  PURPOSE:   To get an attribute from an entity in the XRA.
//
//  PARAMETERS: pOpt        - The target ctrl pointer
//              cpcEntity   - Name of the Entity containing the attribute
//                            (must either be Opt or a direct child of Opt).
//              cpcAttribute - Name of the Attribute .
//              pchValue    - pointer to the (return) string which receives
//                            the control's attribute.
//
//  RETURNS:    0 - Control attribute set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetRemoteZOptAttribute( ZOption *pOpt,
                        zCPCHAR cpcEntity,
                        zCPCHAR cpcAttribute,
                        zPCHAR  pchValue,
                        zUSHORT uMaxLth )
{
#ifdef zREMOTE_SERVER

#ifdef DEBUG_ALL
   TraceLineS( "In GetRemoteZOptAttribute ", *(pOpt->m_pzsTag) );
#endif

   ZXRA_Item *pXRA;

// *pchValue = 0;  permit the caller to set the default return value!

   if ( pOpt->m_pZSubtask->m_pZSocketTask &&
        (pXRA = pOpt->m_pZSubtask->m_pZSocketTask->
          m_pCacheWndList->Find( *(pOpt->m_pZSubtask->m_pzsRemoteWndTag),
                                 pOpt->m_pZSubtask,
                                 "GetRemoteZOptAttribute" )) != 0 &&
        (pXRA->m_vXRA || pXRA->m_vXRA_Delta) )
   {
      zVIEW  vXRA;
      zSHORT nPass;

#ifdef DEBUG_ALL
      TraceLineS( "GetRemoteZOptAttribute tag: ", *(pOpt->m_pzsTag) );
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
              SetCursorFirstEntityByString( vXRA, "Wnd", "Tag",
                                      *(pOpt->m_pZSubtask->m_pzsRemoteWndTag),
                                      0 ) < zCURSOR_SET )
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

         if ( PositionOnRemoteOpt( pOpt, vXRA, FALSE ) )
         {
            GetVariableFromAttribute( pchValue, 0, zTYPE_STRING,
                                      uMaxLth - 1, vXRA,
                                      cpcEntity, cpcAttribute,
                                      0, zUSE_DEFAULT_CONTEXT );
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

//./ ADD NAME=SetRemoteOptAttribute
// Source Module=zdrdlgc.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetRemoteOptAttribute
//                                              01/07/00 Modified: 01/07/00
//
//  PURPOSE:   To set an attribute on an entity in the XRA.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcOptionTag  - The unique name of the option.
//              cpcEntity   - Name of the Entity containing the attribute
//                            (must either be Opt or a direct child of Opt).
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
SetRemoteOptAttribute( zVIEW   vSubtask,
                       zCPCHAR cpcOptionTag,
                       zCPCHAR cpcEntity,
                       zCPCHAR cpcAttribute,
                       zCPCHAR cpcValue )
{
#ifdef zREMOTE_SERVER
   ZSubtask *pZSubtask;
   ZOption  *pOpt;

#ifdef DEBUG_ALL
   TraceLineI( "In SetRemoteOptAttribute ", (zLONG) vSubtask );
#endif

   if ( cpcOptionTag &&
        GetWindowAndOption( &pZSubtask, &pOpt, vSubtask, cpcOptionTag ) == 0 )
   {
      return( SetRemoteZOptAttribute( pOpt, cpcEntity,
                                      cpcAttribute, cpcValue ) );
   }
   else
      return( -1 );
#else
   return( -1 );
#endif
}
