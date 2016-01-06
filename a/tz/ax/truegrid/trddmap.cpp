/////////////////////////////////////////////////////////////////////////////
// Project TRUEGRID.DLL
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2000 Ton Beller AG.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         trddmap.cpp wrapp the ITrueDBDropDownCtrl control
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of dbgrid32.ocx as an Zeidon ActiveX-Control.
//

/* Change log most recent first order:

   2001.01.03  BL    2000   ActiveX
      Modified InitMapping for save the Name for temporaly View to member variable
      (if drop Application View with function DropObjectInstance, then m_vGridApp
      is not null and the Application View does not exists -> Error by DropView)

   2000.9.18   TMV   2000   ActiveX
      do inline documentation

*/


#include "zstdafx.h"
#include "tdbg6.h"
#include "XARRAYDB.H"// include Header of array
#include "trhlpr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef TB_DROPDOWN_IMPLEMENT

ITrueDBDropDownCtrl::~ITrueDBDropDownCtrl()
{
   StoreLayoutChanges();
   // free list of CActiveXEvents
   POSITION pos = m_EventList.GetHeadPosition( );
   while ( m_EventList.IsEmpty() == FALSE )
   {
      CActiveXEvent *a = (CActiveXEvent *)m_EventList.RemoveHead( );
      delete a;
   }
}


/***********************************************************************************
**
** helper functions for bookmark handling
**
***********************************************************************************/


/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::MakeBookmark
// Description     : converts a given position into a variant that can be used as a BOOKMARK
//                   that is used to address a specific row in the grid
// Return type     : void
// Argument        : long lAbsolutePosition the position that has to be converted
// Argument        : LPVARIANT vBookmark target variant
/**************************************************************************/
void ITrueDBDropDownCtrl::MakeBookmark( long lAbsolutePosition, LPVARIANT vBookmark )
{
   // Convert it into a type that should be used
   // when dealing with this grid
   vBookmark->vt = zDBGRID_VT_BOOKMARK;
   vBookmark->lVal = lAbsolutePosition;
}



/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::GetPositionFromBookmark
// Description     : converts a given bookmark into a long value that can be used
//                   to address a specific row Entity in a view by calling
//                   SetCursorRelativePosition(...)
// Return type     : long position extracted from given bookmark
// Argument        : COleVariant v representing a bookmark
/**************************************************************************/
long ITrueDBDropDownCtrl::GetPositionFromBookmark( COleVariant v )
{
   // Convert it into a type that should be used
   // when dealing with this grid
   COleVariant position;
   position.ChangeType( zDBGRID_VT_BOOKMARK, v );
   long lPosition = position.lVal;
   return lPosition;
}

/***********************************************************************************
**
** helper functions for initialization
**
***********************************************************************************/


/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::Init
// Description     : Initialize current object instance
// Return type     : void
/**************************************************************************/
void ITrueDBDropDownCtrl::Init()
{
   m_strScopeEntityName = "";
   m_strEntityName      = "";
   m_strViewName        = "";
   m_bTraceEvents       = FALSE;
   m_bLayoutChanges     = FALSE;
   m_vGridApp = 0;

   // Grid cursor set by SetCurrentCell
   m_bExternalRowChange = FALSE;

   // do we have to trace the events?
   CString strHelp;
   zPCHAR lpszTrace = strHelp.GetBufferSetLength( MAX_PATH );
   SysReadZeidonIni( -1, "[ActiveX]", "TraceEvents", lpszTrace, MAX_PATH );
   strHelp.ReleaseBuffer();
   if ( strHelp.CompareNoCase( "y" ) == 0 )
   {
      m_bTraceEvents = TRUE;
   }

   if( m_pZActiveX )
   {
      m_pZActiveX ->SetWrapperInstance( this );

      TRY
      {
         // Zeidon requires specific Datamode
         long DataMode = GetDataMode( );
         if ( DataMode != dbgUnboundSt ) // 1 = Unbound 4 = Storage
         {
            CString strMsg = "Data mode must be set to \"Storage\". Mapping code will not be called!";
            MessagePrompt( GetDialogView(),
                           "TRUEGRID SCR00001",
                           "Invalid Property",
                           strMsg,
                           TRUE,
                           zBUTTONS_OK,
                           0,
                           zICON_ERROR );
         }
         SetBorderStyle( 1 );

         LoadLayoutChanges();
      }
      CATCH( COleException, e )
      {
         e->ReportError();
         // clean up
         m_pZActiveX->SetWrapperInstance( NULL );
         CString strMsg = "Unable to request Data mode. Mapping code will not be called!";
         MessagePrompt( GetDialogView(),
                        "TRUEGRID SCR00002",
                        "Dispatch Driver",
                        strMsg,
                        TRUE,
                        zBUTTONS_OK,
                        0,
                        zICON_ERROR );
      }
      END_CATCH
   }
}



/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::GetDialogView
// Description     : returns the view to the dialog
// Return type     : zVIEW
/**************************************************************************/
zVIEW ITrueDBDropDownCtrl::GetDialogView()
{
   if( !m_pZActiveX )
   {
      TraceLineS( "Invalid Zeidon Control Pointer ", "" );
      return( 0 );
   }
   if( !m_pZActiveX->m_pZSubtask )
   {
      TraceLineS( "Invalid Zeidon Subtask ", "" );
      return( 0 );
   }

   return m_pZActiveX->m_pZSubtask->m_vDialog;
}



/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::InitMapping
// Description     : Initializes the mapping
// Return type     : zSHORT
/**************************************************************************/
zSHORT ITrueDBDropDownCtrl::InitMapping( )
{
   // setting maximum length of values to diplay for each attribute
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;
   zSHORT   nRC = 0;

   if ( pMapList )
   {
      m_strViewName = pMapList->m_csVName;   // a list entity was specified
      m_strEntityName = pMapList->m_csEName;
   }
   else
   {
      pMapping = m_pZActiveX->m_MapList[ 0 ];              // default to first entity as list
      m_strViewName = pMapping->m_csVName;
      m_strEntityName = pMapping->m_csEName;
   }

   if ( pMapScope )
      m_strScopeEntityName = pMapScope->m_csVName;  // a scope entity was specified

   if ( GetViewByName( &m_vGridApp, m_strTempViewName,
                       GetDialogView(), zLEVEL_ANY ) > 0 )
   {
      DropView( m_vGridApp );
      m_vGridApp = NULL;
   }

   // If we cannot get the view ... quit.
   zVIEW vApp;
   if ( GetViewByName( &vApp, m_strViewName,
                       GetDialogView(), zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for TrueGrid - ", m_strViewName );
      nRC = -1;
   }
   else
   {
      CreateViewFromView( &m_vGridApp, vApp );
      // try to setup a unique viewname
      CString strFormat = m_strViewName + "%p";
      m_strTempViewName.Format( strFormat, m_vGridApp );

      // name the view to make sure it's not dropped by some garbage collection
      SetNameForView( m_vGridApp, ( LPCSTR ) m_strTempViewName, 0, zLEVEL_TASK );

      for ( short lCol = 0; lCol < m_pZActiveX->m_MapList.GetSize(); lCol++ )
      {
         pMapping = m_pZActiveX->m_MapList[ lCol ];
         if ( pMapping )
         {
            zCPCHAR cpcContext = pMapping->m_csContext;
            GetAttributeDisplayLength( &(pMapping->m_ulDisplayLth), vApp,
                                       pMapping->m_csEName,
                                       pMapping->m_csAName,
                                       *cpcContext ? cpcContext : 0 );
         }
         else
            pMapping->m_ulDisplayLth = 0;
      }

      nRC = 1;
   }

   return nRC;
}


/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::InitEventList
// Description     : Initializes the event list that can be used for debugging
//                   purpose to get an idea about the eventsequence
// Return type     : void
/**************************************************************************/
void ITrueDBDropDownCtrl::InitEventList()
{
   if ( m_bTraceEvents )
   {
      m_EventList.AddTail( new CActiveXEvent ( "ColResize",   DISPID_ColResize         ));
      m_EventList.AddTail( new CActiveXEvent ( "RowChange",   DISPID_RowChange      ));
      m_EventList.AddTail( new CActiveXEvent ( "RowResize",   DISPID_RowResize         ));
      m_EventList.AddTail( new CActiveXEvent ( "SelChange",   DISPID_SelChange         ));
      m_EventList.AddTail( new CActiveXEvent ( "SplitChange",   DISPID_SplitChange         ));
      //m_EventList.AddTail( new CActiveXEvent ( "MouseMove",   DISPID_MouseMove         ));
      m_EventList.AddTail( new CActiveXEvent ( "MouseDown",   DISPID_MouseDown         ));
      m_EventList.AddTail( new CActiveXEvent ( "MouseUp",   DISPID_MouseUp            ));
      m_EventList.AddTail( new CActiveXEvent ( "KeyUp",   DISPID_KeyUp               ));
      m_EventList.AddTail( new CActiveXEvent ( "KeyDown",   DISPID_KeyDown            ));
      m_EventList.AddTail( new CActiveXEvent ( "KeyPress",   DISPID_KeyPress            ));
      m_EventList.AddTail( new CActiveXEvent ( "Change",   DISPID_Change            ));
      m_EventList.AddTail( new CActiveXEvent ( "UnboundGetRelativeBookmark",   DISPID_UnboundGetRelativeBookmark   ));
      m_EventList.AddTail( new CActiveXEvent ( "UnboundReadData",   DISPID_UnboundReadData   ));
      m_EventList.AddTail( new CActiveXEvent ( "UnboundReadDataEx",   DISPID_UnboundReadDataEx   ));
      m_EventList.AddTail( new CActiveXEvent ( "ClassicRead",   DISPID_ClassicRead         ));
      m_EventList.AddTail( new CActiveXEvent ( "ClassicWrite",   DISPID_ClassicWrite      ));
      m_EventList.AddTail( new CActiveXEvent ( "ClassicDelete",   DISPID_ClassicDelete      ));
      m_EventList.AddTail( new CActiveXEvent ( "ClassicAdd",   DISPID_ClassicAdd         ));
      m_EventList.AddTail( new CActiveXEvent ( "UnboundWriteData",   DISPID_UnboundWriteData   ));
      m_EventList.AddTail( new CActiveXEvent ( "UnboundAddData",   DISPID_UnboundAddData      ));
      m_EventList.AddTail( new CActiveXEvent ( "UnboundDeleteRow",   DISPID_UnboundDeleteRow   ));
      m_EventList.AddTail( new CActiveXEvent ( "Error",   DISPID_Error               ));
      m_EventList.AddTail( new CActiveXEvent ( "UnboundColumnFetch",   DISPID_UnboundColumnFetch));
      m_EventList.AddTail( new CActiveXEvent ( "AfterColEdit",   DISPID_AfterColEdit      ));
      m_EventList.AddTail( new CActiveXEvent ( "BeforeColEdit",   DISPID_BeforeColEdit      ));
      m_EventList.AddTail( new CActiveXEvent ( "ColEdit",   DISPID_ColEdit            ));
      m_EventList.AddTail( new CActiveXEvent ( "FirstRowChange",   DISPID_FirstRowChange      ));
      m_EventList.AddTail( new CActiveXEvent ( "LeftColChange",   DISPID_LeftColChange      ));
      m_EventList.AddTail( new CActiveXEvent ( "FormatText",   DISPID_FormatText         ));
      m_EventList.AddTail( new CActiveXEvent ( "DragCell",   DISPID_DragCell            ));
      m_EventList.AddTail( new CActiveXEvent ( "ButtonClick",   DISPID_ButtonClick         ));
      m_EventList.AddTail( new CActiveXEvent ( "ComboSelect",   DISPID_ComboSelect         ));
      m_EventList.AddTail( new CActiveXEvent ( "ValueItemError",   DISPID_ValueItemError      ));
      m_EventList.AddTail( new CActiveXEvent ( "FetchCellStyle",   DISPID_FetchCellStyle      ));
      m_EventList.AddTail( new CActiveXEvent ( "FetchRowStyle",   DISPID_FetchRowStyle      ));
      m_EventList.AddTail( new CActiveXEvent ( "OnAddNew",   DISPID_OnAddNew            ));
      m_EventList.AddTail( new CActiveXEvent ( "Paint",   DISPID_Paint               ));
      m_EventList.AddTail( new CActiveXEvent ( "ColMove",   DISPID_ColMove            ));
      m_EventList.AddTail( new CActiveXEvent ( "PostEvent",   DISPID_PostEvent         ));
      m_EventList.AddTail( new CActiveXEvent ( "FetchCellTips",   DISPID_FetchCellTips      ));
      m_EventList.AddTail( new CActiveXEvent ( "OLECompleteDrag",   DISPID_OLECompleteDrag   ));
      m_EventList.AddTail( new CActiveXEvent ( "OLEDragDrop",   DISPID_OLEDragDrop         ));
      m_EventList.AddTail( new CActiveXEvent ( "OLEDragOver",   DISPID_OLEDragOver         ));
      m_EventList.AddTail( new CActiveXEvent ( "OLEGiveFeedback",   DISPID_OLEGiveFeedback   ));
      m_EventList.AddTail( new CActiveXEvent ( "OLESetData",   DISPID_OLESetData         ));
      m_EventList.AddTail( new CActiveXEvent ( "OLEStartDrag",   DISPID_OLEStartDrag      ));
      m_EventList.AddTail( new CActiveXEvent ( "OnInit",   DISPID_OnInit            ));
      m_EventList.AddTail( new CActiveXEvent ( "DataSourceChanged",   DISPID_DataSourceChanged   ));
      m_EventList.AddTail( new CActiveXEvent ( "LayoutReady",   DISPID_LayoutReady         ));
   }
}


/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::WantHandleEvents
// Description     : Notifies the Zeidon Control which events are handles
//                   by this DLL
// Return type     : void
/**************************************************************************/
void ITrueDBDropDownCtrl::WantHandleEvents(  )
{
   // this DLL handles the events below
   m_pZActiveX->RegisterEvent( DISPID_RowChange );
   // change selection
   m_pZActiveX->RegisterEvent( DISPID_SelChange         );
   m_pZActiveX->RegisterEvent( DISPID_RowChange         );
   // writing  a single row
   m_pZActiveX->RegisterEvent( DISPID_RowResize         );
   m_pZActiveX->RegisterEvent( DISPID_ColResize         );

   // for debug purpose add almost any event
   if ( m_bTraceEvents )
   {
      m_pZActiveX->RegisterEvent( DISPID_Click            );
      m_pZActiveX->RegisterEvent( DISPID_DblClick            );
      m_pZActiveX->RegisterEvent( DISPID_HeadClick         );
      m_pZActiveX->RegisterEvent( DISPID_FootClick         );
      m_pZActiveX->RegisterEvent( DISPID_OwnerDrawCell      );
      m_pZActiveX->RegisterEvent( DISPID_Scroll            );
      m_pZActiveX->RegisterEvent( DISPID_SelChange         );
      m_pZActiveX->RegisterEvent( DISPID_Change            );
      m_pZActiveX->RegisterEvent( DISPID_Error            );
      m_pZActiveX->RegisterEvent( DISPID_UnboundColumnFetch   );
      m_pZActiveX->RegisterEvent( DISPID_AfterColEdit         );
      m_pZActiveX->RegisterEvent( DISPID_BeforeColEdit      );
      m_pZActiveX->RegisterEvent( DISPID_ColEdit            );
      m_pZActiveX->RegisterEvent( DISPID_FirstRowChange      );
      m_pZActiveX->RegisterEvent( DISPID_LeftColChange      );
      m_pZActiveX->RegisterEvent( DISPID_FormatText         );
      m_pZActiveX->RegisterEvent( DISPID_DragCell            );
      m_pZActiveX->RegisterEvent( DISPID_ButtonClick         );
      m_pZActiveX->RegisterEvent( DISPID_ComboSelect         );
      m_pZActiveX->RegisterEvent( DISPID_ValueItemError      );
      m_pZActiveX->RegisterEvent( DISPID_FetchCellStyle      );
      m_pZActiveX->RegisterEvent( DISPID_FetchRowStyle      );
      m_pZActiveX->RegisterEvent( DISPID_OnAddNew            );
      m_pZActiveX->RegisterEvent( DISPID_Paint            );
      m_pZActiveX->RegisterEvent( DISPID_ColMove            );
      m_pZActiveX->RegisterEvent( DISPID_PostEvent         );
      m_pZActiveX->RegisterEvent( DISPID_FetchCellTips      );
      m_pZActiveX->RegisterEvent( DISPID_OLECompleteDrag      );
      m_pZActiveX->RegisterEvent( DISPID_OLEDragDrop         );
      m_pZActiveX->RegisterEvent( DISPID_OLEDragOver         );
      m_pZActiveX->RegisterEvent( DISPID_OLEGiveFeedback      );
      m_pZActiveX->RegisterEvent( DISPID_OLESetData         );
      m_pZActiveX->RegisterEvent( DISPID_OLEStartDrag         );
      m_pZActiveX->RegisterEvent( DISPID_OnInit            );
      m_pZActiveX->RegisterEvent( DISPID_DataSourceChanged   );
      m_pZActiveX->RegisterEvent( DISPID_LayoutReady         );
   }

}


/***********************************************************************************
**
** helper function for Event handling
**
***********************************************************************************/
//
// Get name of a event for debug traces
//

/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::GetEventName
// Description     : Translates an event into a string
// Return type     : CString
// Argument        : long l
/**************************************************************************/
CString ITrueDBDropDownCtrl::GetEventName( long l)
{
   CString strOut;
   switch ( l )
   {
      case  DISPID_AUTOSIZE :
         strOut = _T("DISPID_AUTOSIZE");
         break;
      case  DISPID_BACKCOLOR :
         strOut = _T("DISPID_BACKCOLOR");
         break;
      case  DISPID_BACKSTYLE :
         strOut = _T("DISPID_BACKSTYLE");
         break;
      case  DISPID_BORDERCOLOR :
         strOut = _T("DISPID_BORDERCOLOR");
         break;
      case  DISPID_BORDERSTYLE :
         strOut = _T("DISPID_BORDERSTYLE");
         break;
      case  DISPID_BORDERWIDTH :
         strOut = _T("DISPID_BORDERWIDTH");
         break;
      case  DISPID_DRAWMODE :
         strOut = _T("DISPID_DRAWMODE");
         break;
      case  DISPID_DRAWSTYLE :
         strOut = _T("DISPID_DRAWSTYLE");
         break;
      case  DISPID_DRAWWIDTH :
         strOut = _T("DISPID_DRAWWIDTH");
         break;
      case  DISPID_FILLCOLOR :
          strOut = _T("DISPID_FILLCOLOR");
         break;
      case  DISPID_FILLSTYLE :
          strOut = _T("DISPID_FILLSTYLE");
         break;
      case  DISPID_FONT :
         strOut = _T("DISPID_FONT");
         break;
      case  DISPID_FORECOLOR :
         strOut = _T("DISPID_FORECOLOR");
         break;
      case  DISPID_ENABLED :
         strOut = _T("DISPID_ENABLED");
         break;
      case  DISPID_HWND :
         strOut = _T("DISPID_HWND");
         break;
      case  DISPID_TABSTOP :
         strOut = _T("DISPID_TABSTOP");
         break;
      case  DISPID_TEXT :
         strOut = _T("DISPID_TEXT");
         break;
      case  DISPID_CAPTION :
         strOut = _T("DISPID_CAPTION");
         break;
      case  DISPID_BORDERVISIBLE :
         strOut = _T("DISPID_BORDERVISIBLE");
         break;
      case  DISPID_APPEARANCE :
         strOut = _T("DISPID_APPEARANCE");
         break;
      case  DISPID_MOUSEPOINTER :
         strOut = _T("DISPID_MOUSEPOINTER");
         break;
      case  DISPID_MOUSEICON :
         strOut = _T("DISPID_MOUSEICON");
         break;
      case  DISPID_PICTURE :
         strOut = _T("DISPID_PICTURE");
         break;
      case  DISPID_VALID :
         strOut = _T("DISPID_VALID");
         break;
      case  DISPID_READYSTATE :
         strOut = _T("DISPID_READYSTATE");
         break;
#ifdef DISPID_LISTINDEX
      case  DISPID_LISTINDEX :
         strOut = _T("DISPID_LISTINDEX");
         break;
      case  DISPID_SELECTED :
         strOut = _T("DISPID_SELECTED");
         break;
      case  DISPID_LIST :
         strOut = _T("DISPID_LIST");
         break;
      case  DISPID_COLUMN :
         strOut = _T("DISPID_COLUMN");
         break;
      case  DISPID_LISTCOUNT :
         strOut = _T("DISPID_LISTCOUNT");
         break;
      case  DISPID_MULTISELECT :
         strOut = _T("DISPID_MULTISELECT");
         break;
      case  DISPID_MAXLENGTH :
         strOut = _T("DISPID_MAXLENGTH");
         break;
      case  DISPID_PASSWORDCHAR :
         strOut = _T("DISPID_PASSWORDCHAR");
         break;
      case  DISPID_SCROLLBARS :
         strOut = _T("DISPID_SCROLLBARS");
         break;
      case  DISPID_WORDWRAP :
         strOut = _T("DISPID_WORDWRAP");
         break;
      case  DISPID_MULTILINE :
         strOut = _T("DISPID_MULTILINE");
         break;
      case  DISPID_NUMBEROFROWS :
         strOut = _T("DISPID_NUMBEROFROWS");
         break;
      case  DISPID_NUMBEROFCOLUMNS :
         strOut = _T("DISPID_NUMBEROFCOLUMNS");
         break;
      case  DISPID_DISPLAYSTYLE :
         strOut = _T("DISPID_DISPLAYSTYLE");
         break;
      case  DISPID_GROUPNAME :
         strOut = _T("DISPID_GROUPNAME");
         break;
      case  DISPID_IMEMODE :
         strOut = _T("DISPID_IMEMODE");
         break;
      case  DISPID_ACCELERATOR :
         strOut = _T("DISPID_ACCELERATOR");
         break;
      case  DISPID_ENTERKEYBEHAVIOR :
         strOut = _T("DISPID_ENTERKEYBEHAVIOR");
         break;
      case  DISPID_TABKEYBEHAVIOR :
         strOut = _T("DISPID_TABKEYBEHAVIOR");
         break;
      case  DISPID_SELTEXT :
         strOut = _T("DISPID_SELTEXT");
         break;
      case  DISPID_SELSTART :
         strOut = _T("DISPID_SELSTART");
         break;
      case  DISPID_SELLENGTH :
         strOut = _T("DISPID_SELLENGTH");
         break;
      case  DISPID_REFRESH :
         strOut = _T("DISPID_REFRESH");
         break;
      case  DISPID_DOCLICK :
         strOut = _T("DISPID_DOCLICK");
         break;
      case  DISPID_ABOUTBOX :
         strOut = _T("DISPID_ABOUTBOX");
         break;
      case  DISPID_ADDITEM :
         strOut = _T("DISPID_ADDITEM");
         break;
      case  DISPID_CLEAR :
         strOut = _T("DISPID_CLEAR");
         break;
      case  DISPID_REMOVEITEM :
         strOut = _T("DISPID_REMOVEITEM");
         break;
      case  DISPID_CLICK :
         strOut = _T("DISPID_CLICK");
         break;
      case  DISPID_DBLCLICK :
         strOut = _T("DISPID_DBLCLICK");
         break;
      case  DISPID_KEYDOWN :
         strOut = _T("DISPID_KEYDOWN");
         break;
      case  DISPID_KEYPRESS :
         strOut = _T("DISPID_KEYPRESS");
         break;
      case  DISPID_KEYUP :
         strOut = _T("DISPID_KEYUP");
         break;
      case  DISPID_MOUSEDOWN :
         strOut = _T("DISPID_MOUSEDOWN");
         break;
      case  DISPID_MOUSEMOVE :
         strOut = _T("DISPID_MOUSEMOVE");
         break;
      case  DISPID_MOUSEUP :
         strOut = _T("DISPID_MOUSEUP");
         break;
      case  DISPID_ERROREVENT :
         strOut = _T("DISPID_ERROREVENT");
         break;
      case  DISPID_READYSTATECHANGE :
         strOut = _T("DISPID_READYSTATECHANGE");
         break;
      case  DISPID_CLICK_VALUE :
         strOut = _T("DISPID_CLICK_VALUE");
         break;
      case  DISPID_RIGHTTOLEFT :
         strOut = _T("DISPID_RIGHTTOLEFT");
         break;
      case  DISPID_TOPTOBOTTOM :
         strOut = _T("DISPID_TOPTOBOTTOM");
         break;
      case  DISPID_THIS :
         strOut = _T("DISPID_THIS");
         break;
      case  DISPID_AMBIENT_BACKCOLOR :
         strOut = _T("DISPID_AMBIENT_BACKCOLOR");
         break;
      case  DISPID_AMBIENT_DISPLAYNAME :
         strOut = _T("DISPID_AMBIENT_DISPLAYNAME");
         break;
      case  DISPID_AMBIENT_FONT :
         strOut = _T("DISPID_AMBIENT_FONT");
         break;
      case  DISPID_AMBIENT_FORECOLOR :
         strOut = _T("DISPID_AMBIENT_FORECOLOR");
         break;
      case  DISPID_AMBIENT_LOCALEID :
         strOut = _T("DISPID_AMBIENT_LOCALEID");
         break;
      case  DISPID_AMBIENT_MESSAGEREFLECT :
         strOut = _T("DISPID_AMBIENT_MESSAGEREFLECT");
         break;
      case  DISPID_AMBIENT_SCALEUNITS :
         strOut = _T("DISPID_AMBIENT_SCALEUNITS");
         break;
      case  DISPID_AMBIENT_TEXTALIGN :
         strOut = _T("DISPID_AMBIENT_TEXTALIGN");
         break;
      case  DISPID_AMBIENT_USERMODE :
         strOut = _T("DISPID_AMBIENT_USERMODE");
         break;
      case  DISPID_AMBIENT_UIDEAD :
         strOut = _T("DISPID_AMBIENT_UIDEAD");
         break;
      case  DISPID_AMBIENT_SHOWGRABHANDLES :
         strOut = _T("DISPID_AMBIENT_SHOWGRABHANDLES");
         break;
      case  DISPID_AMBIENT_SHOWHATCHING :
         strOut = _T("DISPID_AMBIENT_SHOWHATCHING");
         break;
      case  DISPID_AMBIENT_DISPLAYASDEFAULT :
         strOut = _T("DISPID_AMBIENT_DISPLAYASDEFAULT");
         break;
      case  DISPID_AMBIENT_SUPPORTSMNEMONICS :
         strOut = _T("DISPID_AMBIENT_SUPPORTSMNEMONICS");
         break;
      case  DISPID_AMBIENT_AUTOCLIP :
         strOut = _T("DISPID_AMBIENT_AUTOCLIP");
         break;
      case  DISPID_AMBIENT_APPEARANCE :
         strOut = _T("DISPID_AMBIENT_APPEARANCE");
         break;
      case  DISPID_AMBIENT_CODEPAGE :
         strOut = _T("DISPID_AMBIENT_CODEPAGE");
         break;
      case  DISPID_AMBIENT_PALETTE :
         strOut = _T("DISPID_AMBIENT_PALETTE");
         break;
      case  DISPID_AMBIENT_CHARSET :
         strOut = _T("DISPID_AMBIENT_CHARSET");
         break;
      case  DISPID_AMBIENT_TRANSFERPRIORITY :
         strOut = _T("DISPID_AMBIENT_TRANSFERPRIORITY");
         break;
      case  DISPID_AMBIENT_RIGHTTOLEFT :
         strOut = _T("DISPID_AMBIENT_RIGHTTOLEFT");
         break;
      case  DISPID_AMBIENT_TOPTOBOTTOM :
         strOut = _T("DISPID_AMBIENT_TOPTOBOTTOM");
         break;
#endif
      case  DISPID_Name :
         strOut = _T("DISPID_Name");
         break;
      case  DISPID_Delete :
         strOut = _T("DISPID_Delete");
         break;
      case  DISPID_Object :
         strOut = _T("DISPID_Object");
         break;
      case  DISPID_Parent :
         strOut = _T("DISPID_Parent");
         break;
      default:
         POSITION pos = m_EventList.GetHeadPosition();
         while ( pos && strOut.IsEmpty() )
         {
            CActiveXEvent *a = (CActiveXEvent *)m_EventList.GetNext( pos );
            if ( a->m_lID == l)
               strOut = a->m_strName;
         }
         break;
   }

   if ( strOut.IsEmpty() )
      strOut.Format( "Unknown Eventname for id %d", l );

   return strOut;
}



/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::HandleEvent
// Description     : general event handler
// Return type     : zSHORT
// Argument        : DISPID dispEvent
/**************************************************************************/
zSHORT ITrueDBDropDownCtrl::HandleEvent( DISPID dispEvent )
{
   if (m_bTraceEvents )
   {
      CString strEventName = GetEventName( dispEvent );
      TraceLineS("<TrueGrid UsrDll> Event : ", strEventName );
   }

   zSHORT nRC = zACTIVEX_EVENT_COMPLETED;

   zLONG lRowColChange = 0;

   switch ( dispEvent )
   {
   case DISPID_RowChange:
      lRowColChange = OnRowChange( );

      if ( zDBGRID_AVOID_EVENT_ROUTING == lRowColChange )
      {
         nRC = zACTIVEX_EVENT_COMPLETED; // all done don't call user provided code
      }
      else
      {
         nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      }

      if ( m_pZActiveX->m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI )
      {
         nRC = zACTIVEX_EVENT_COMPLETED; // all done don't call user provided code
         m_pZActiveX->m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
      }
      break;
   case DISPID_SelChange:
      OnSelChange( );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_RowResize:
   case DISPID_ColResize:
      m_bLayoutChanges = TRUE;
      break;
   default:
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
   break;
   }

   return nRC;
}




/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::MapFromOI
// Description     : Zeidon specific routine MapFromOI
//                   It is not realy necessary to do it this way
//                   but it is useful for documentation
// Return type     : zSHORT
/**************************************************************************/
zSHORT ITrueDBDropDownCtrl::MapFromOI( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ITrueDBDropDownCtrl::MapFromOI ", *m_pZActiveX->m_pzsTag );
#endif
   zSHORT nRC = 0;

   // if the control is not Visible, do not do mapping
   if ( (m_pZActiveX->m_ulMapActFlags & zMAPACT_VISIBLE) == 0 ||
        (m_pZActiveX->m_ulMapActFlag2 & zMAPACT_CREATED) == 0 ||
        (m_pZActiveX->m_ulMapActFlags & zMAPACT_NOAUTOMAP_FROM_OI) )
   {
      nRC = -1;
   }

   if ( nRC >= 0 )
   {
      m_pZActiveX->m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI | zMAPACT_MAPPING_FROM_OI;

      // Auto-enable if this control was auto-disabled.
      if ( m_pZActiveX->m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
      {
         m_pZActiveX->m_ulMapActFlags |= zMAPACT_ENABLED;
         m_pZActiveX->m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
      }

      if ( InitMapping( ) < 0 )
      {
         m_pZActiveX->m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
      }

      TRY
      {
         // Refresh will force the control to fire DISPID_UnboundReadData events
         if ( m_vGridApp )
         {
            if( GetDataMode() == dbgUnboundSt )
               StorageModeMapping();
         }
      }
      CATCH ( COleException, e )
      {
         e->ReportError();
         nRC = -3;
      }
      END_CATCH
   }

   return nRC;
}

/**********************************************************************************************/
// Function name   : ITrueDBDropDownCtrl::StorageModeMapping
// Description      : Transfers data to the grid using storage mode
//                  to use Storage mode another ActiveX ( IXArrayDB )has to be instanciated
//                  filled with data and passed to the grid. IXArrayDB is kind of an Safearray
//                  datamembers of IXArrayDB can be accessed by row & column indexes
// Return type      : zSHORT
/**********************************************************************************************/
zSHORT ITrueDBDropDownCtrl::StorageModeMapping()
{
#ifdef DEBUG_ALL
   TraceLineS( "ITrueDBDropDownCtrl::StorageModeMapping ", *m_pZActiveX->m_pzsTag );
#endif
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;

   zLONG lMapCount = m_pZActiveX->m_MapList.GetSize();

   if( pMapScope )
      lMapCount--;

   COleVariant vValue;
   CString strValue;

   zSHORT nRC = 0;


   zLONG lColCount = 0;
   Columns cols ( GetColumns() );
   lColCount = cols.GetCount();

   LPDISPATCH pDispatch = GetArray();
   IXArrayDB xARRAY ( pDispatch );

   if ( pDispatch == NULL )
   {
      xARRAY.CreateDispatch( xARRAY.GetCLSID() );
   }

   long lRowCount = CountEntitiesForView( m_vGridApp, m_strEntityName );

   if( lColCount == 0 || lMapCount == 0 )
      return( -1 );

   xARRAY.ReDim( 0, lRowCount - 1, 0, lColCount - 1 );

   nRC = SetEntityCursor( m_vGridApp, m_strEntityName, 0,
                          m_pZActiveX->m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                          m_strScopeEntityName, 0 );

   zLONG iRelativeNbr = 0;
   while ( nRC > zCURSOR_UNCHANGED )
   {
      zLONG lRealColumn = 0;
      for ( zSHORT lCol = 0; lCol < m_pZActiveX->m_MapList.GetSize(); lCol++ )
      {
         pMapping = m_pZActiveX->m_MapList[ lCol ];
         if ( pMapping && pMapping != pMapList && pMapping != pMapScope )
         {
            COleVariant v;
            CString str;
            LPSTR szValue = str.GetBufferSetLength( pMapping->m_ulDisplayLth + 1 );
            nRC = GetVariableFromAttribute( szValue, 0, zTYPE_STRING,
                                            pMapping->m_ulDisplayLth,
                                            m_vGridApp,
                                            pMapping->m_csEName,
                                            pMapping->m_csAName,
                                            pMapping->m_csContext,
                                            *pMapping->m_csContext ?
                                             zACCEPT_NULL_ENTITY :
                                             zACCEPT_NULL_ENTITY |
                                                   zUSE_DEFAULT_CONTEXT );

            if ( nRC == zVAR_NULL )
               szValue[ 0 ] = 0;

            str.ReleaseBuffer();
            v = str;
            xARRAY.Set( iRelativeNbr, lRealColumn, v );
            lRealColumn++;
         }
      }
      nRC = SetEntityCursor( m_vGridApp, m_strEntityName, 0,
                             m_pZActiveX->m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                             m_strScopeEntityName, 0 );
      iRelativeNbr++;
   } // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru entities

   SetArray( xARRAY );
   ReBind( );

   return nRC;
}




/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::MapToOI
// Description     : Zeidon specific routine MapToOI
//                   It is not realy necessary to do it this way
//                   but it is useful for documentation
// Return type     : zSHORT
/**************************************************************************/
zSHORT ITrueDBDropDownCtrl::MapToOI( )
{
   if ( m_pZActiveX->m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
   {
      m_pZActiveX->m_ulMapActFlags |= zMAPACT_ENABLED;
      m_pZActiveX->m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
   }

   m_pZActiveX->m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;

   return ( 0 );
}




/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::LoadLayoutChanges
// Description     : Loads layout changes made during runtime.
//                   Thus are restricted to Column width and row hight
// Return type     : void
/**************************************************************************/
void ITrueDBDropDownCtrl::LoadLayoutChanges()
{
#ifdef DEBUG_ALL
   TraceLineS( "ITrueDBDropDownCtrl::StoreLayoutChanges() ", *(m_pZActiveX->m_pzsTag) );
#endif
   int nRC = 0;
   CString strCtrlBuffer = "Ctrl:";
   strCtrlBuffer += *(m_pZActiveX->m_pzsTag);

   CString strPositionBuffer;


   Columns cols( GetColumns() );
   int nCols = cols.GetCount();

   zSHORT nLth = 80 * nCols;

   strPositionBuffer.Empty();

   char *pchBuffer = strPositionBuffer.GetBufferSetLength( nLth + 1 );
   char *pch;
   char *pchComma;

   GetWindowPreferenceString( GetDialogView(),
                              strCtrlBuffer, pchBuffer, nLth );

   zSHORT nCol = 0;
   if ( *pchBuffer )
   {
      pch = pchBuffer;
      while ( pch )
      {
         pch = strchr( pch, ',' );
         if ( pch )
            pch++;
         nCol++;
      }
   }

   if ( nCol == nCols )
   {
      pch = pchBuffer;
      for ( nCol = 0; nCol < nCols; nCol++ )
      {
         pchComma = strchr( pch, ',' );
         if ( pchComma )
            *pchComma = 0;
         Column c = cols.GetColumn( nCol );
         long lColWidth = zatol( pch );
         c.SetWidth( (float)lColWidth );

         if ( pchComma )
            pch = pchComma + 1;
         else
            break;
      }
   }

   strCtrlBuffer = "RowHeight:";
   strCtrlBuffer += *( m_pZActiveX->m_pzsTag );
   nLth = 32;
   LPSTR lpszBuffer = strPositionBuffer.GetBufferSetLength( nLth );
   GetWindowPreferenceString( GetDialogView(),
                              strCtrlBuffer, lpszBuffer, nLth );

   long lRowHeigth = zatol( lpszBuffer );
   SetRowHeight( ( float ) lRowHeigth );
}

/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::StoreLayoutChanges
// Description     : stores layout changes made during runtime.
//                   Thus are restricted to Column width and row hight
// Return type     : void
/**************************************************************************/
void ITrueDBDropDownCtrl::StoreLayoutChanges()
{
#ifdef DEBUG_ALL
   TraceLineS( "ITrueDBDropDownCtrl::StoreLayoutChanges() ", *(m_pZActiveX->m_pzsTag) );
#endif
   CString strCtrlBuffer = "Ctrl:";
   CString strPositionBuffer;

   zSHORT nLth = 0;

   if ( !m_bLayoutChanges )
      return;

   Columns cols( GetColumns() );
   int nCols = cols.GetCount();

   strCtrlBuffer += *(m_pZActiveX->m_pzsTag);

   strPositionBuffer.Empty();

   for ( zSHORT k = 0; k < nCols; k++ )
   {
      Column c = cols.GetColumn( k );
      char szBuffer[ 32 ];
      szBuffer[ 0 ] = 0;
      _ltoa_s( (zLONG) c.GetWidth(), szBuffer, sizeof( szBuffer ), 10 );
      strPositionBuffer += szBuffer;
      if ( k < nCols - 1 )
      {
         strPositionBuffer += ",";
      }
   }

   SetWindowPreferenceString( GetDialogView(), strCtrlBuffer, strPositionBuffer );

   strCtrlBuffer = "RowHeight:";
   strCtrlBuffer += *( m_pZActiveX->m_pzsTag );

   long lRowHeigth = ( long )GetRowHeight();
   LPSTR lpszBuffer = strPositionBuffer.GetBufferSetLength( 32 );
   _ltoa_s( (zLONG) lRowHeigth, lpszBuffer, 32, 10 );
   strPositionBuffer.ReleaseBuffer();

   SetWindowPreferenceString( GetDialogView(), strCtrlBuffer, strPositionBuffer );

}


/***********************************************************************************
**
** helper function for special exported functions
**
***********************************************************************************/



/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::OnSelChange
// Description     : handles the SelChange event
// Return type     : zLONG
/**************************************************************************/
zLONG ITrueDBDropDownCtrl::OnSelChange( )
{
   zLONG nRC = 0;
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;

   zVIEW vApp;
   if ( GetViewByName( &vApp, m_strViewName,
                       GetDialogView(), zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for TrueGrid - ", m_strViewName );
      nRC = -1;
   }

   return nRC;
}



/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::OnRowChange
// Description     : Handles the row change event
// Return type     : zLONG
/**************************************************************************/
zLONG ITrueDBDropDownCtrl::OnRowChange( )
{
#ifdef DEBUG_ALL
   CString strMethodName = "ITrueDBDropDownCtrl::OnRowColChange";
   TraceLineS("<TrueGrid UsrDll> Method : ", strMethodName );
#endif

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we call in the right context
   if ( pEvent->m_dispid != DISPID_RowColChange )
   {
      TraceLineI( "TrueGrid - OnRowColChange called on wrong event ", pEvent->m_dispid );
      return( -1 );
   }

   // extract parameter
   COleVariant vLastRow( pEvent->m_pDispParams->rgvarg[ 1 ].pvarVal );
   VARIANT     *pvLastRow = (pEvent->m_pDispParams->rgvarg[ 1 ].pvarVal);
   zSHORT      nLastCol = (pEvent->m_pDispParams->rgvarg[ 0 ].iVal);
   zLONG lPosition = zDBGRID_AVOID_EVENT_ROUTING;

   // we are just doing mapping
   // so we have to reset the zMAPACT_MAPPING_FROM_OI because
   // if MapFromOI is called as sequence of events are fired and at the end there
   // is the event DISPID_RowColChange
   if ( (m_pZActiveX->m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI) ||
        m_bExternalRowChange )
   {
      SetGridCursorFromViewCursor( );
      lPosition = zDBGRID_AVOID_EVENT_ROUTING;
      if ( m_bExternalRowChange )
         m_bExternalRowChange = FALSE;
   }
   else
   {
      // if so we have a last row and a last column as parameters
      // but we only need the row to check whether syncronization has to be done
      TRY
      {
         // we have to set the view cursor if vLastRow is not the current one
         COleVariant vCurrentRow ( GetBookmark() );
         zLONG lCurrentRow = 0;
         zLONG lLastRow    = 0;
         if ( !::IsVariantNULL( vCurrentRow ) && !::IsVariantNULL( vLastRow ) )
         {
            lCurrentRow = GetPositionFromBookmark( vCurrentRow );
            lLastRow = GetPositionFromBookmark( vLastRow );
         }

         if ( lLastRow != lCurrentRow )
         {//
            // hey we got the last row now we have to get the current one
            lPosition = SetViewCursorFromGridCursor( vCurrentRow );
         }
      }
      CATCH_ALL (  e )
      {
         CString strErrorMessage;
         LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
         e->GetErrorMessage( lpErrorMessage, 1024 );
         strErrorMessage.ReleaseBuffer();
         TraceLineS( "TrueGrid- Exception Handling OnRowColChange ", strErrorMessage  );
      }
      END_CATCH_ALL
   }
   return lPosition;
}

//
/**************************************************************************/
// Function name   : ITrueDBDropDownCtrl::SetGridCursorFromViewCursor
// Description     : Sets the grids input focus row to the row that is
//                   referenced by cursorsetting in the view
// Return type     : void
/**************************************************************************/
void ITrueDBDropDownCtrl::SetGridCursorFromViewCursor( )
{

   zLONG lPosition = 0;

   TRY
   {
      // If we have a View name and a Scoping (list) Entity name ... continue
      if ( m_strViewName.IsEmpty( ) || m_strEntityName.IsEmpty( ) )
      {
         TraceLineS( "TrueGrid- ","Empty View or Entityname passed"  );
         return;
      }
      // get the view and position cursor.
      zVIEW vApp;
      if ( GetViewByName( &vApp, m_strViewName,
                          GetDialogView(), zLEVEL_ANY ) <= 0 )
      {
         TraceLineS( "Unable to load view for MsDBGrid - ", m_strViewName );
         lPosition = 0;
      }
      else
         lPosition = GetRelativeEntityNumber( vApp, m_strEntityName, 0, m_pZActiveX->m_ulFlag );

      if ( lPosition >= 0 )
      {
         zSHORT nColumn = GetCol();
         COleVariant vCurrentRow ( GetBookmark() );
         vCurrentRow = lPosition;
         SetBookmark( vCurrentRow );
      }
   }
   CATCH_ALL ( e )
   {
      e->ReportError();
   }
   END_CATCH_ALL
}

/*********************************************************************************/
// Function name   : ITrueDBDropDownCtrl::SetViewCursorFromGridCursor
// Description     : Sets the views cursor to the input focus row referenced
//                   by vCurrentRow
// Return type     : zLONG - a negative value if the view or the entity is unknown
//                           or cursorsetting fails. Otherwise the current row
// Argument        : COleVariant vCurrentRow - the row that should get the cursor
/*********************************************************************************/
zLONG ITrueDBDropDownCtrl::SetViewCursorFromGridCursor( COleVariant vCurrentRow )
{
   // If we have a View name and a Scoping (list) Entity name ... continue
   if ( m_strViewName.IsEmpty( ) || m_strEntityName.IsEmpty( ) )
   {
      TraceLineS( "TrueGrid- ","Empty View or Entityname passed"  );
      return( -3 );
   }
   // get the view and position cursor.
   zVIEW vApp;
   if ( GetViewByName( &vApp, m_strViewName,
                       GetDialogView(), zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MsDBGrid - ", m_strViewName );
      return( -4 );
   }


   zLONG lPosition = GetPositionFromBookmark ( vCurrentRow );
   zSHORT nRC = SetEntityCursor( vApp, m_strEntityName, 0,
              zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
              lPosition, m_strScopeEntityName, 0 );

   if ( nRC <= zCURSOR_UNCHANGED )
   {
      TraceLineI( "TrueGrid- set cursor failed. nRC = ", nRC  );
      return -5;
   }
   return lPosition;
}


#endif // TB_DROPDOWN_IMPLEMENT
