/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrmenu.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of ZOption.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.05.29    DKS    Cosmetic.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added Mapping List.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Hold on to default menu for ActiveX menu insertion.
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

// these ZSubtask methods have been moved to this source file to reduce the
// size of zdrstask.cpp
//
/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: ZSubtask::CreateZeidonMenu
//
// PURPOSE: This routine processes OPTION entities and creates
//          the menu (action bar).
//
// =====> example of creating a menu and sub-menus
//
// // create an empty menu
// CMenu *menu = new CMenu( );
//
// // create a new popup window
// CMenu *subMenu = new CMenu( );
//
// // Add file to the new popup menu
// subMenu->AppendMenu( MF_STRING, CM_FILENEW, "&New" );
// subMenu->AppendMenu( MF_STRING, CM_FILEOPEN, "&Open" );
// subMenu->AppendMenu( MF_STRING, CM_FILESAVE, "&Save" );
//
// // Add the new menu to the window's menu bar.
// menu->AppendMenu( MF_POPUP, (UINT)(HMENU) *subMenu, "&File" );
//
// // GetApplication( )->GetMainWindow( )->SetMenu( (HMENU) menu );
// SetMenu( (HMENU) *menu );
//
// // Draw the menu bar
// DrawMenuBar( );
//
// =====> end of: example of creating a menu and sub-menus
//
/////////////////////////////////////////////////////////////////////////////
void
ZSubtask::CreateZeidonMenu( zCPCHAR cpcMenuName )
{
#ifdef DEBUG_ALL
TraceLineS( "In CreateZeidonMenu - ", cpcMenuName );
#endif

   mDeleteInit( m_pZBinMenu );  // note: all ZOptions are deleted as well
   if ( m_pZFWnd )
      m_pZFWnd->SetMenu( 0 );

   mDeleteInit( m_pMenu );
   mDeleteInit( m_pzsMenuTag );
   m_pzsMenuTag = new CString( cpcMenuName );

// AutoGrayActions( );     this is too early

   if ( (m_ulSubtaskFlags & zSUBTASK_SUBWND_MDI_FRAME) )
   {
//??? m_pMenu = new CMenu( (HINSTANCE) m_pZDrApp->m_hInstance, TResId( MDI_MENU ) );
   }

   // Check if any menu entities exist.
   if ( cpcMenuName[ 0 ] &&
        SetCursorFirstEntityByString( m_vDialog, szlMenu, szlTag, cpcMenuName, 0 ) > zCURSOR_UNCHANGED &&
        CheckExistenceOfEntity( m_vDialog, szlOpt ) > zCURSOR_UNCHANGED )
   {
      zLONG  lOptCnt;

      GetIntegerFromAttribute( &lOptCnt, m_vDialog, szlMenu, szlOptCnt );
      m_pZBinMenu = new ZMultiIdxSet( (fnCOMPARE) fnCompareOptionNodes,
                                      (fnCOMPARE) fnSeekOptionNode,
                                      lOptCnt,
                                      (fnVISIT_INORDER) fnDeleteOptionObject );

      zVIEW vSubobject;

      CreateViewFromViewForTask( &vSubobject, m_vDialog, m_vDialog );
      if ( m_pMenu == 0 )
      {
         m_pMenu = new CMenu( );
         m_pMenu->CreateMenu( );
      }

      fnRecurseSubOptions( vSubobject, m_pZBinMenu, m_pMenu, 0, 0, 0, 0 );
      DropView( vSubobject );

#ifdef DEBUG_ALL
      TraceLineS( "MENU COMPLETE ... ", "list in order" );
      m_pZBinMenu->VisitInorder( (fnVISIT_INORDER) fnOrderedOptions );
#endif
   }

   if ( m_pZFWnd )
   {
      if ( m_pMenu )
         m_pZFWnd->m_hMenuDefault = m_pMenu->m_hMenu;
      else
         m_pZFWnd->m_hMenuDefault = 0;

      m_pZFWnd->SetMenu( m_pMenu );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: CreateZeidonPopupMenu
//
// PURPOSE: This routine processes OPTION entities and creates the popup menu
//
/////////////////////////////////////////////////////////////////////////////
void
ZSubtask::CreateZeidonPopupMenu( zCPCHAR cpcMenuName,
                                 POINT   pt )
{
#ifdef DEBUG_ALL
TraceLineS( "In CreateZeidonPopupMenu - ", cpcMenuName );
#endif

   if ( m_pzsPopupTag )
   {
//
//    Caching a menu causes problems (e.g.  when a menu is created
//    based on enabled actions ... and the cursor position changes
//    which in turn alters the set of actions that are enabled ...
//    but the cached menu prevents the menu from being recreated -
//    when it really needs to be)!!!
//
//    if ( zstrcmp( cpcMenuName, *m_pzsPopupTag ) == 0 )
//    {
//       m_pTrackMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, *m_pZFWnd );
//       return;
//    }
//    else
      {
         mDeleteInit( m_pZBinPopup );  // note: deletes all ZOptions as well
         mDeleteInit( m_pTrackMenu );
         mDeleteInit( m_pzsPopupTag );
      }
   }

   AutoGrayActions( );

   // Check if any menu entities exist.
   if ( cpcMenuName[ 0 ] &&
        SetCursorFirstEntityByString( m_vDialog, szlMenu, szlTag, cpcMenuName, 0 ) > zCURSOR_UNCHANGED &&
        CheckExistenceOfEntity( m_vDialog, szlOpt ) > zCURSOR_UNCHANGED )
   {
      zVIEW  vSubobject;
      zLONG  lOptCnt;

      GetIntegerFromAttribute( &lOptCnt, m_vDialog, szlMenu, szlOptCnt );
      m_pZBinPopup = new ZMultiIdxSet( (fnCOMPARE) fnCompareOptionNodes,
                                       (fnCOMPARE) fnSeekOptionNode,
                                       lOptCnt,
                                       (fnVISIT_INORDER) fnDeleteOptionObject );

      CreateViewFromViewForTask( &vSubobject, m_vDialog, m_vDialog );
      m_pTrackMenu = new CMenu( );
      m_pTrackMenu->CreatePopupMenu( );
      fnRecurseSubOptions( vSubobject, m_pZBinPopup, m_pTrackMenu,
                           0, 0, 0, zPOPUP_MENU_FIRST );
      DropView( vSubobject );

#ifdef DEBUG_ALL
      TraceLineS( "MENU COMPLETE ... ", "list in order" );
      m_pZBinPopup->VisitInorder( (fnVISIT_INORDER) fnOrderedOptions );
#endif

      m_pzsPopupTag = new CString( cpcMenuName );
      HCURSOR hCursorSave = ::SetCursor( ::LoadCursor( 0, IDC_ARROW ) );
   // TraceLineS( "CreateZeidonPopupMenu TrackPopupMenu - ", cpcMenuName );

      ZSubtask *pZSubtaskMain = GetSplitterMainSubtask( );
      if ( pZSubtaskMain == 0 )
            pZSubtaskMain = this;

      ((ZDrFrame *) (pZSubtaskMain->m_pZFWnd))->m_pZSubtaskPopup = this;
      m_pTrackMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, pZSubtaskMain->m_pZFWnd, 0 );
   // if ( m_pZTask->m_nTraceAction > 2 )
   //    TraceLineS( "CreateZeidonPopupMenu After Track - ", cpcMenuName );

      ::SetCursor( hCursorSave );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: fnRecurseSubOptions
//
// PURPOSE: This routine processes Option and OptOpt entities to create
//          sub-options on the action bar.  At this point, we are
//          (recursively) processing the Option entity to add a
//          corresponding item to the action bar.
//
/////////////////////////////////////////////////////////////////////////////
zUSHORT
ZSubtask::fnRecurseSubOptions( zVIEW   vSubOption,
                               ZMultiIdxSet *pZBinOpt,
                               CMenu   *pMenu,
                               ZOption *pParent,
                               zUSHORT uParentOptID,
                               zUSHORT uOptID,
                               zUSHORT uOptBaseID )
{
   zKZWDLGXO_Opt_DEF OptDef;
   zKZWDLGXO_OptMap_DEF OptMapDef;
   ZOption *pZOption;
   CMenu   *pSubMenu;
   zUSHORT uFlags;
   zCHAR   szOptionText[ 256 ];
   zUSHORT uOptionPos = -1;
   zUSHORT uSeparatorCnt;
   zSHORT  nRC = zCURSOR_SET;    // existence has already been established

   OptDef.CheckByte = 0xFE;
   OptDef.lStructSize = sizeof( zKZWDLGXO_Opt_DEF );
   OptMapDef.CheckByte = 0xFE;
   OptMapDef.lStructSize = sizeof( zKZWDLGXO_OptMap_DEF );

   // Note that existence of the Option entity has already been established.
   while ( nRC > zCURSOR_UNCHANGED )        // Loop for each Option entity
   {
      uSeparatorCnt = 0;
      uFlags = MF_BYCOMMAND | MF_STRING;

      // get the structure of attributes for this option
      zPCHAR pch;   // for derived Text attribute

      GetStructFromEntityAttrs( (zPCHAR) &OptDef, vSubOption, szlOpt );
      if ( OptDef.CheckByte != 0xFE )
         MessageBox( 0, "OptDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

      GetAddrForAttribute( (zPCHAR *) &pch, vSubOption, szlOpt, szlD_Text );
      if ( *pch )
         strcpy_s( OptDef.Text, sizeof( OptDef.Text ), pch );

      if ( CheckExistenceOfEntity( vSubOption, szlOptAct ) > zCURSOR_UNCHANGED )
      {
         GetAddrForAttribute( (zPCHAR *) &pch, vSubOption, szlOptAct, szlTag );
         if ( pch[ 0 ] && SetCursorFirstEntityByString( m_vDialog, szlAct, szlTag, pch, 0 ) > zCURSOR_UNCHANGED )
         {
            zULONG  ulIdx;

            // Get the sync index for this action.
            GetIntegerFromAttribute( (zPLONG) &ulIdx, m_vDialog, szlAct, "SyncIdx" );
            if ( m_pchEnabledAction[ (zSHORT) (ulIdx + sizeof( zSHORT )) ] <= 0 )
            {
               if ( uOptBaseID >= zPOPUP_MENU_FIRST )
               {
                  nRC = SetCursorNextEntity( vSubOption, szlOpt, 0 );
                  continue;
               }
               else
               {
                  uFlags |= MF_GRAYED;
               }
            }
         }
      }

      if ( CheckExistenceOfEntity( vSubOption, szlOptMap ) > zCURSOR_UNCHANGED )
      {
         GetStructFromEntityAttrs( (zPCHAR) &OptMapDef, vSubOption, szlOptMap );
         if ( OptMapDef.CheckByte != 0xFE )
            MessageBox( 0, "OptMapDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );
      }
      else
      {
         OptMapDef.Tag[ 0 ] = 0;
         OptMapDef.VN[ 0 ] = 0;
         OptMapDef.EN[ 0 ] = 0;
         OptMapDef.AN[ 0 ] = 0;
         OptMapDef.Context[ 0 ] = 0;
      }

#ifdef DEBUG_ALL
      TraceLineS( "Tag ", OptDef.Tag );
      TraceLineS( "Text ", OptDef.Text );
      TraceLineS( "CheckMarkValueOn ", OptDef.ChkOn );
      TraceLineS( "CheckMarkValueOff ", OptDef.ChkOff );
      TraceLineS( "ViewName ", OptMapDef.VN );
      TraceLineS( "Entity ", OptMapDef.EN );
      TraceLineS( "Attribute ", OptMapDef.AN );
      TraceLineS( "Context ", OptMapDef.Context );
      TraceLineS( "Separator ", OptDef.Separator );
#endif

      if ( OptDef.Separator[ 0 ] == 'Y' )
      {
         uSeparatorCnt++;
         pMenu->AppendMenu( MF_SEPARATOR );
         uOptionPos++;

         // If we are at the top level of the menu, put a double separator
         // in for potential ActiveX menu inclusion.  Note that we do not
         // want this to happen for popup menus!
         if ( uParentOptID == 0 && uOptBaseID < zPOPUP_MENU_FIRST )
         {
            uSeparatorCnt++;
            pMenu->AppendMenu( MF_SEPARATOR );
            uOptionPos++;
         }
      }

      // Put this option into the driver's menu list for quick reference.
      uOptID++;
      pZOption = new ZOption( vSubOption, &OptDef, &OptMapDef,
                              uOptBaseID + uOptID, pParent, this );
      pZOption->m_uParentID = uParentOptID;
      pZOption->m_uSeparator = uSeparatorCnt;

      uOptionPos++;
      pZOption->m_uOptionPos = uOptionPos;

      if ( pZBinOpt->InsertNode( pZOption ) == 0 )
      {
         TraceLineS( "(drvr) Menu Error (DUPLICATE Menu Item tag) ==> ",  OptDef.Tag );

#ifdef DEBUG     // should not happen ... protected by painter???
         OperatorSend( 0, szlDrvr, "DUPLICATE Menu Item tag!!!", TRUE );
         pZBinOpt->VisitInorder( (fnVISIT_INORDER) fnOrderedOptions );
#endif
         return( -1 );
      }

      strcpy_s( szOptionText, sizeof( szOptionText ), OptDef.Text );
      if ( CheckExistenceOfEntity( vSubOption, szlOptOpt ) > zCURSOR_UNCHANGED )
      {
         pZOption->m_bParent = TRUE;
         uFlags |= MF_POPUP;
         pSubMenu = new CMenu( );
         pSubMenu->CreateMenu( );
         SetViewToSubobject( vSubOption, szlOptOpt );
         uOptID = fnRecurseSubOptions( vSubOption, pZBinOpt, pSubMenu,  pZOption, uOptID, uOptID, uOptBaseID );
         ResetViewFromSubobject( vSubOption );
         if ( uOptID <= 0 )
         {
            if ( pSubMenu )
               delete( pSubMenu );

            return( -1 );     // some kind of error has occurred
         }
      }
      else
      {
         pZOption->m_bParent = FALSE;

         // accelerators may only occur if no sub-options exist
         if ( CheckExistenceOfEntity( vSubOption, szlOptAct ) == 0 )
         {
            zVIEW   vAct;
            zPCHAR pch;

            CreateViewFromViewForTask( &vAct, m_vDialog, m_vDialog );
            SetEntityCursor( vAct, szlAct, 0, zPOS_FIRST | zQUAL_ENTITYCSR, vSubOption, szlOptAct, 0, 0, 0, 0 );
            if ( SetEntityCursor( vAct, szlActHot, 0, zPOS_FIRST, 0, 0, 0, 0, 0, 0 ) == 0 )
            {
//             SetEntityCursor( vAct, szlHotkey, 0, zPOS_FIRST | zQUAL_ENTITYCSR, vAct, szlActHot, 0, 0, 0, 0 );
               GetAddrForAttribute( (zCOREMEM) &pch, vAct, szlActHot, szlTitle );
               if ( *pch )
               {
                  strcat_s( szOptionText, sizeof( szOptionText ), "\t" );
                  strcat_s( szOptionText, sizeof( szOptionText ), pch );
               }
            }

            DropView( vAct );
         }

         if ( pZOption->m_bChecked )
            uFlags |= MF_CHECKED;
         else
            uFlags |= MF_UNCHECKED;

         pSubMenu = 0;
      }

      mDeleteInit( pZOption->m_pzsText );
      pZOption->m_pzsText = new CString( szOptionText );
      if ( pSubMenu )
      {
         pZOption->m_hMenu = pSubMenu->Detach( );
         pMenu->AppendMenu( uFlags, (UINT) pZOption->m_hMenu, szOptionText );
         delete( pSubMenu );
      }
      else
      {
         pZOption->m_hMenu = pMenu->m_hMenu;
         pMenu->AppendMenu( uFlags, uOptID + uOptBaseID, szOptionText );
      }

      nRC = SetCursorNextEntity( vSubOption, szlOpt, 0 );

   }  // end of:  while ( nRC > zCURSOR_UNCHANGED ) .. Loop for each Option

   return( uOptID );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZOption - ctor
ZOption::ZOption( zVIEW    vSubOption,
                  zKZWDLGXO_Opt_DEF *pOptDef,
                  zKZWDLGXO_OptMap_DEF *pOptMapDef,
                  zUSHORT  uID,
                  ZOption  *pParent,
                  ZSubtask *pZSubtask )
{
#ifdef DEBUG_ALL
   TraceLine( "ZOption::ctor Option Tag: %s   ID: %d", pOptDef->Tag, uID );
#endif

   // Options must have tags ... painter/convert responsibility???
   m_pzsTag = new CString( pOptDef->Tag );
   m_pzsText = 0;
   m_hMenu = 0;
   m_pParent = pParent;

   // painter ensures EName and AName if VName and/or check mark values
   // are specified???
   if ( pOptMapDef->VN[ 0 ] )
   {
      m_pzsVName = new CString( pOptMapDef->VN );
      m_pzsEName = new CString( pOptMapDef->EN );
      m_pzsAName = new CString( pOptMapDef->AN );
      m_pzsContext = new CString( pOptMapDef->Context );
      m_pzsValueOn = new CString( pOptDef->ChkOn );
      m_pzsValueOff = new CString( pOptDef->ChkOff );
   }
   else
   {
      m_pzsVName = 0;
      m_pzsEName = 0;
      m_pzsAName = 0;
      m_pzsContext = 0;
      m_pzsValueOn = 0;
      m_pzsValueOff = 0;
   }

#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "Option tag - ", pOptDef->Tag );
   TraceLineS( "        VN - ", pOptMapDef->VN );
   TraceLineS( "        EN - ", pOptMapDef->EN );
   TraceLineS( "        AN - ", pOptMapDef->AN );
   TraceLineS( "   Context - ", pOptMapDef->Context );
   TraceLineS( "     ChkOn - ", pOptDef->ChkOn );
   TraceLineS( "    ChkOff - ", pOptDef->ChkOff );
#endif

   m_pzsActionTag = 0;
   if ( CheckExistenceOfEntity( vSubOption, szlOptAct ) > zCURSOR_UNCHANGED )
   {
      zPCHAR pch;

      GetAddrForAttribute( (zPCHAR *) &pch, vSubOption, szlOptAct, szlTag );
      if ( pch[ 0 ] )
         m_pzsActionTag = new CString( pch );
   }

   m_uOptionID = uID;
   m_nDIL = (zSHORT) pOptDef->DIL;
   m_pZSubtask = pZSubtask;    // needed???
   m_uSeparator = 0;
   m_uOptionPos = 0;

   m_bEnabled = TRUE;
   m_bChecked = FALSE;
   m_bVisible = TRUE;

// SetCheckForOption( );
}

// ZOption - alternate ctor
ZOption::ZOption( zUSHORT  uID,
                  zCPCHAR  cpcTag,
                  zCPCHAR  cpcText,
                  zCPCHAR  cpcActionTag,
                  zBOOL    bChecked,
                  ZOption  *pParent,
                  ZSubtask *pZSubtask )
{
#ifdef DEBUG_ALL
   TraceLine( "ZOption::ctor Option Tag: %s   ID: %d", cpcTag, uID );
#endif

   // Options must have tags ... painter/convert responsibility???
   m_pzsTag = new CString( cpcTag );
   m_pzsText = new CString( cpcText );
   m_hMenu = 0;
   m_pParent = pParent;

   m_pzsVName = 0;
   m_pzsEName = 0;
   m_pzsAName = 0;
   m_pzsContext = 0;
   m_pzsValueOn = 0;
   m_pzsValueOff = 0;

   if ( cpcActionTag && *cpcActionTag )
      m_pzsActionTag = new CString( cpcActionTag );
   else
      m_pzsActionTag = 0;

   m_uOptionID = uID;
   m_nDIL = 0;
   m_pZSubtask = pZSubtask;    // needed???
   m_uSeparator = 0;
   m_uOptionPos = 0;

   m_bEnabled = TRUE;
   m_bChecked = bChecked;
   m_bVisible = TRUE;

// SetCheckForOption( );
}

// ZOption -   dtor
ZOption::~ZOption( )
{
#ifdef DEBUG_ALL
// if TraceLineS is called in a destructor, things ain't necessarily up
// to snuff (core trace may be gone) ... so use this with caution
#endif
// TraceLineS( "ZOption dtor for option ", *m_pzsTag );

   mDeleteInit( m_pzsTag );
   mDeleteInit( m_pzsText );
   mDeleteInit( m_pzsVName );
   mDeleteInit( m_pzsEName );
   mDeleteInit( m_pzsAName );
   mDeleteInit( m_pzsContext );
   mDeleteInit( m_pzsValueOn );
   mDeleteInit( m_pzsValueOff );
   mDeleteInit( m_pzsActionTag );
}

// only reset the check value if there is mapping ... otherwise the check
// value is under the control of the application.
zBOOL
ZOption::SetCheckForOption( )
{
   zCHAR  szText[ 65 ];
   zVIEW vApp;

   if ( m_pzsVName && m_pzsValueOn &&
        GetViewByName( &vApp, *m_pzsVName,
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      m_bChecked = FALSE;
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) > zCURSOR_UNCHANGED )
      {
         GetVariableFromAttribute( szText, 0, zTYPE_STRING, 64, vApp, *m_pzsEName, *m_pzsAName, *m_pzsContext, **m_pzsContext ? 0 : zUSE_DEFAULT_CONTEXT );
#ifdef DEBUG_ALL
         TraceLineS( "Option tag - ", *m_pzsTag );
         TraceLineS( " Option VN - ", *m_pzsVName );
         TraceLineS( " Option EN - ", *m_pzsEName );
         TraceLineS( " Option AN - ", *m_pzsAName );
         TraceLineS( "Option value - ", szText );
#endif
         if ( zstrcmp( (*m_pzsValueOn).GetString(), szText ) == 0 )
            m_bChecked = TRUE;
      }
   }

   return( m_bChecked );
}

/////////////////////////////////////////////////////////////////////////////
//
// CMenu * GetSubMenu( int nPos ) const;
//
// Returns:
//    A pointer to a CMenu object whose m_hMenu member contains a handle
//    to the pop-up menu if a pop-up menu exists at the given position;
//    otherwise NULL.  If a CMenu object does not exist, then a temporary
//    one is created.  The CMenu pointer returned should not be stored.
//
// Parameters:
//    nPos - Specifies the position of the pop-up menu contained in the
//       menu.  Position values start at 0 for the first menu item.  The
//       pop-up menu's identifier cannot be used in this function.
//
// Remarks:
//    Retrieves the CMenu object of a pop-up menu.
//
/////////////////////////////////////////////////////////////////////////////
CMenu *
ZOption::GetMenu( ZSubtask *pZSubtask )
{
   CMenu *pMenu;

   if ( m_pParent )
      pMenu = m_pParent->GetMenu( pZSubtask );
   else
      pMenu = pZSubtask->m_pMenu;

   return( pMenu->GetSubMenu( m_uOptionPos ) );
}

zBOOL
ZOption::RemoveMenu( ZSubtask *pZSubtask )
{
   // If this menu item has children, remove all child menu items.
   ZOption *pZOption;
   zUSHORT k;

   for ( k = 1; k <= pZSubtask->m_pZBinMenu->GetCount( ); k++ )
   {
      pZOption = (ZOption *) pZSubtask->m_pZBinMenu->GetAt( k );
      if ( pZOption->m_pParent == this )
         pZOption->RemoveMenu( pZSubtask );
   }

   if ( pZSubtask->m_pMenu->RemoveMenu( m_uOptionID, MF_BYCOMMAND ) == 0 )
      return( FALSE );

   m_bVisible = FALSE;
   return( TRUE );
}

zBOOL
ZOption::InsertMenu( CMenu    *pMenu,
                     ZSubtask *pZSubtask )
{
   ZOption  *pZOption;
   zSHORT   nFound = -1;
   zSHORT   nRC;
   zUSHORT  k;

   // Locate the item before which this menu item is to be inserted.
   for ( k = 1; k <= pZSubtask->m_pZBinMenu->GetCount( ); k++ )
   {
      pZOption = (ZOption *) pZSubtask->m_pZBinMenu->GetAt( k );
      if ( pZOption == this )
      {
         while ( k < pZSubtask->m_pZBinMenu->GetCount( ) )
         {
            k++;
            pZOption = (ZOption *) pZSubtask->m_pZBinMenu->GetAt( k );
            if ( pZOption->m_bVisible )
            {
               nFound = k;
               break;   // out of inner loop
            }
         }

         break;   // out of outer loop
      }
   }

   if ( nFound >= 0 )
      pZOption = (ZOption *) pZSubtask->m_pZBinMenu->GetAt( nFound );
   else
      pZOption = 0;

   zUSHORT uFlags = MF_BYCOMMAND | MF_STRING;
   if ( m_bChecked )
      uFlags |= MF_CHECKED;
   else
      uFlags |= MF_UNCHECKED;

   if ( m_bEnabled == FALSE )
      uFlags |= MF_GRAYED;

   if ( m_bParent )
   {
      uFlags |= MF_POPUP;
      CMenu *pSubMenu = new CMenu( );
      pSubMenu->CreateMenu( );

      // If this menu item has children, insert all child menu items.
      ZOption *pZOptionCurr;
      for ( k = 1; k <= pZSubtask->m_pZBinMenu->GetCount( ); k++ )
      {
         pZOptionCurr = (ZOption *) pZSubtask->m_pZBinMenu->GetAt( k );
         if ( pZOptionCurr->m_pParent == this )
            pZOptionCurr->InsertMenu( pSubMenu, pZSubtask );
      }

      if ( pZOption )
         nRC = pMenu->InsertMenu( pZOption->m_uOptionID, uFlags, (UINT) pSubMenu->Detach( ), *m_pzsText );
      else
         nRC = pMenu->AppendMenu( uFlags, (UINT) pSubMenu->Detach( ), *m_pzsText );

      delete( pSubMenu );
   }
   else
   {
      if ( pZOption )
         nRC = pMenu->InsertMenu( pZOption->m_uOptionID, uFlags,  m_uOptionID, *m_pzsText );
      else
         nRC = pMenu->AppendMenu( uFlags, m_uOptionID, *m_pzsText );
   }

   m_bVisible = TRUE;

   if ( nRC == 0 )
      return( TRUE );
   else
      return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
