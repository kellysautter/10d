/////////////////////////////////////////////////////////////////////////////
// Project MSDBGRID.DLL
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2000 Ton Beller AG.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         MSDBMAP.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of dbgrid32.ocx as an Zeidon ActiveX-Control.
//

/*
 CHANGE LOG - most recent first order

   200y.mm.dd    xxx
      Note ...

   2000.08.16    TMV
      Implement routines for multiple selection
      Fix Bookmark bug when reading a single row

   2000.06.15    TMV
      Implement routines for manual navigation by using row and col indexes

   2000.05.29    TMV
      call InitMapping whenever MapFromOI is called to allow
      dynamic mapping and fix bug 051369

   2000.05.05    TMV
      add exception handling when call several
      dispatch methods

   2000.04.13  TMV    Z10  DBGRID
      wrapp predefined DISPID's via ifdefs because they are more detailed in
      VC++ 6.0 than in VC++ 5.0 which makes this source not compilable with VC++ 5.0


*/

#include "msdbgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



void IMsDgridCtrl::Init()
{
   m_nCurrentOrderCol   = -1;
   m_nCurrentOrderMode  =  0;
   m_nLogX              =  0;
   m_nLogY              =  0;
   m_lPixelsPerInchX    =  0;
   m_lPixelsPerInchY    =  0;
   m_strScopeEntityName = "";
   m_strEntityName      = "";
   m_strViewName        = "";
//   m_bRowChangedByApplication = FALSE;
   m_bTraceEvents = FALSE;
   m_pWndListArray = 0;
   m_pWndListBox = 0;
   m_vGridApp = 0;

   // Grid cursor set by SetCurrentCell
   m_bExternalRowChange = FALSE;

   // do we have to trace the events?
   CString strHelp;
   zPCHAR lpszTrace = strHelp.GetBufferSetLength( MAX_PATH );
   SysReadZeidonIni( -1, "[ActiveX]", "TraceEvents", lpszTrace );
   strHelp.ReleaseBuffer();
   if ( strHelp.CompareNoCase( "y" ) == 0 )
   {
      m_bTraceEvents = TRUE;
      InitEventList();
   }
}

//
// Get name of a event for debug traces
//
CString IMsDgridCtrl::GetEventName( long l)
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

void IMsDgridCtrl::InitEventList()
{
   if ( m_bTraceEvents )
   {
      m_EventList.AddTail( new CActiveXEvent ("AfterColUpdate(short ColIndex)", 0x1 ));
      m_EventList.AddTail( new CActiveXEvent ("AfterDelete()", 0x2 ));
      m_EventList.AddTail( new CActiveXEvent ("AfterInsert()", 0x3 ));
      m_EventList.AddTail( new CActiveXEvent ("AfterUpdate()", 0x4 ));
      m_EventList.AddTail( new CActiveXEvent ("BeforeColUpdate(short ColIndex, VARIANT* OldValue, short* Cancel)", 0x5 ));
      m_EventList.AddTail( new CActiveXEvent ("BeforeDelete(short* Cancel)", 0x6 ));
      m_EventList.AddTail( new CActiveXEvent ("BeforeInsert(short* Cancel)", 0x7 ));
      m_EventList.AddTail( new CActiveXEvent ("BeforeUpdate(short* Cancel)", 0x8 ));
      m_EventList.AddTail( new CActiveXEvent ("ColResize(short ColIndex, short* Cancel)", 0x9 ));
      m_EventList.AddTail( new CActiveXEvent ("HeadClick(short ColIndex)", 0xa ));
      m_EventList.AddTail( new CActiveXEvent ("RowColChange(VARIANT* LastRow, short LastCol)", 0xb ));
      m_EventList.AddTail( new CActiveXEvent ("RowResize(short* Cancel)", 0xd ));
      m_EventList.AddTail( new CActiveXEvent ("Scroll(short* Cancel)", 0xe ));
      m_EventList.AddTail( new CActiveXEvent ("SelChange(short* Cancel)", 0xf ));
      m_EventList.AddTail( new CActiveXEvent ("SplitChange()", 0x18 ));
      m_EventList.AddTail( new CActiveXEvent ("Change()", 0x10 ));
      m_EventList.AddTail( new CActiveXEvent ("UnboundGetRelativeBookmark(VARIANT* StartLocation, long OffSet, VARIANT* NewLocation, long* ApproximatePosition)", 0x2a ));
      m_EventList.AddTail( new CActiveXEvent ("UnboundReadData(LPDISPATCH RowBuf, VARIANT* StartLocation, BOOL ReadPriorRows)", 0x12 ));
      m_EventList.AddTail( new CActiveXEvent ("UnboundWriteData(LPDISPATCH RowBuf, VARIANT* WriteLocation)", 0x13 ));
      m_EventList.AddTail( new CActiveXEvent ("UnboundAddData(LPDISPATCH RowBuf, VARIANT* NewRowBookmark)", 0x14 ));
      m_EventList.AddTail( new CActiveXEvent ("UnboundDeleteRow(VARIANT* Bookmark)", 0x15 ));
      m_EventList.AddTail( new CActiveXEvent ("Error(short DataError, short* Response)", 0x16 ));
      m_EventList.AddTail( new CActiveXEvent ("AfterColEdit(short ColIndex)", 0x19 ));
      m_EventList.AddTail( new CActiveXEvent ("BeforeColEdit(short ColIndex, short KeyAscii, short* Cancel)", 0x1a ));
      m_EventList.AddTail( new CActiveXEvent ("ColEdit(short ColIndex)", 0x1b ));
      m_EventList.AddTail( new CActiveXEvent ("ButtonClick(short ColIndex)", 0x21 ));
      m_EventList.AddTail( new CActiveXEvent ("OnAddNew()", 0x25 ));
      m_EventList.AddTail( new CActiveXEvent ("Click()", 0xfffffda8 ));
      m_EventList.AddTail( new CActiveXEvent ("DoubleClick", 0xfffffda7 ));
   }
}

zSHORT
IMsDgridCtrl::InitMapping( )
{
   // setting maximum length of values to diplay for each attribute
   zLONG lCol;
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

   if ( m_vGridApp )
   {
      DropView( m_vGridApp );
      m_vGridApp = NULL;
   }

   // If we cannot get the view ... quit.
   zVIEW vApp;
   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      nRC = -1;
   }
   else
   {
      CreateViewFromView( &m_vGridApp, vApp );
      // try to setup a unique viewname
      CString strFormat = m_strViewName + "%p";
      CString strViewName;
      strViewName.Format( strFormat, m_vGridApp );

      // name the view to make sure it's not dropped by some garbage collection
      SetNameForView( m_vGridApp, ( LPCSTR ) strViewName, 0, zLEVEL_TASK );

      for ( lCol = 0; lCol < m_pZActiveX->m_MapList.GetSize(); lCol++ )
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

void
IMsDgridCtrl::WantHandleEvents(  )
{
   // this DLL handles the events below
   // getting bookmarks for positioning purpose
   m_pZActiveX->RegisterEvent( DISPID_UnboundGetBookmark);
   // reading data
   m_pZActiveX->RegisterEvent( DISPID_UnboundReadData );
   // deleting a single row
   m_pZActiveX->RegisterEvent( DISPID_UnboundDeleteData );
   // used to syncronize entity cursor against cntrol
   m_pZActiveX->RegisterEvent( DISPID_RowColChange );
   // Used to display a combobox for a specific cell
   m_pZActiveX->RegisterEvent( DISPID_ButtonClick       );
   // used to implement automatic sort
   m_pZActiveX->RegisterEvent( DISPID_HeadClick         );
   // used to validate data against domain
   m_pZActiveX->RegisterEvent( DISPID_BeforeColUpdate   );
   // used to copy data into the entity - attribute
   m_pZActiveX->RegisterEvent( DISPID_AfterColUpdate    );

   // change selection
   m_pZActiveX->RegisterEvent( DISPID_SelChange         );
   // writing  a single row
   //( ( ZActiveX * ) m_pZActiveX )->RegisterEvent( DISPID_UnboundWriteData );

   // just keep track about their occurence for proper cursor setting
   // in OnRowColChange
   m_pZActiveX->RegisterEvent( DISPID_Click             );
   m_pZActiveX->RegisterEvent( DISPID_KeyDown           );
   m_pZActiveX->RegisterEvent( DISPID_MouseDown         );

   // for debug purpose add almost any event
   if ( m_bTraceEvents )
   {
      m_pZActiveX->RegisterEvent( DISPID_UnboundAddData );
      m_pZActiveX->RegisterEvent( DISPID_SplitChange       );
      m_pZActiveX->RegisterEvent( DISPID_DblClick          );
      m_pZActiveX->RegisterEvent( DISPID_KeyUp             );
      m_pZActiveX->RegisterEvent( DISPID_KeyPress          );
      m_pZActiveX->RegisterEvent( DISPID_AfterColUpdate    );
      m_pZActiveX->RegisterEvent( DISPID_AfterDelete       );
      m_pZActiveX->RegisterEvent( DISPID_AfterInsert       );
      m_pZActiveX->RegisterEvent( DISPID_AfterUpdate       );
      m_pZActiveX->RegisterEvent( DISPID_BeforeDelete      );
      m_pZActiveX->RegisterEvent( DISPID_BeforeInsert      );
      m_pZActiveX->RegisterEvent( DISPID_BeforeUpdate      );
      m_pZActiveX->RegisterEvent( DISPID_ColResize         );
      m_pZActiveX->RegisterEvent( DISPID_RowResize         );
      m_pZActiveX->RegisterEvent( DISPID_Scroll            );
      m_pZActiveX->RegisterEvent( DISPID_Change            );
      m_pZActiveX->RegisterEvent( DISPID_UnboundReadData   );
      m_pZActiveX->RegisterEvent( DISPID_UnboundAddData    );
      m_pZActiveX->RegisterEvent( DISPID_UnboundDeleteData );
      m_pZActiveX->RegisterEvent( DISPID_UnboundWriteData  );
      m_pZActiveX->RegisterEvent( DISPID_Error             );
      m_pZActiveX->RegisterEvent( DISPID_AfterColEdit      );
      m_pZActiveX->RegisterEvent( DISPID_BeforeColEdit     );
      m_pZActiveX->RegisterEvent( DISPID_ColEdit           );
      m_pZActiveX->RegisterEvent( DISPID_OnAddNew          );
      m_pZActiveX->RegisterEvent( DISPID_MouseUp           );
   }
}





zSHORT
IMsDgridCtrl::HandleEvent( DISPID dispEvent )
{
   if (m_bTraceEvents )
   {
      CString strEventName = GetEventName( dispEvent );
      TraceLineS("<UsrDll> Event : ", strEventName );
   }

   zSHORT nRC = zACTIVEX_EVENT_COMPLETED;

   zLONG lRowColChange = 0;

   switch ( dispEvent )
   {
   case DISPID_UnboundGetBookmark:
      // add additional code to handle current event
      OnUnboundGetRelativeBookmark( );
      nRC = zACTIVEX_EVENT_COMPLETED; // all done don't call user provided code
      break;

   case DISPID_UnboundReadData:
      // add additional code to handle current event
      OnUnboundReadData( );
      nRC = zACTIVEX_EVENT_COMPLETED; // all done don't call user provided code
      break;

   case DISPID_UnboundWriteData:
      //OnUnboundWriteData( m_pZActiveX );
      nRC = zACTIVEX_EVENT_COMPLETED; // all done don't call user provided code
      break;
   case DISPID_UnboundAddData:
      nRC = zACTIVEX_EVENT_COMPLETED; // all done don't call user provided code
      break;
   case DISPID_UnboundDeleteData:
      // add additional code to handle current event
      OnUnboundWriteData( );
      nRC = zACTIVEX_EVENT_COMPLETED; // all done don't call user provided code
      break;

   case DISPID_RowColChange:
      lRowColChange = OnRowColChange( );

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
   case DISPID_ButtonClick:
      OnButtonClick(  );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_SelChange:
      OnSelChange( );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_HeadClick:
      OnHeadClick( );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_AfterColUpdate:
      OnAfterColUpdate(  );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_BeforeColUpdate:
      OnBeforeColUpdate(  );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_SplitChange:
//      OnSplitChange(  );
      nRC = zACTIVEX_EVENT_CONTINUE;
      break;
   case DISPID_KeyDown:
      OnKeyDown( );
      nRC = zACTIVEX_EVENT_CONTINUE;
      break;
   case DISPID_MouseDown:
      OnMouseDown( );
      nRC = zACTIVEX_EVENT_CONTINUE;
      break;
   default:
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
   break;
   }

   m_DispIdPrevEvent = dispEvent;
   return nRC;
}

void
IMsDgridCtrl::OnUnboundReadData( )
{
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;

   COleVariant vValue;
   CString strValue;
   zLONG lRow = 0;
   zLONG lCol = 0;
   zLONG lPosition = 0;
   zSHORT nRC = zCURSOR_NULL;
   unsigned long ulCursorFlags = 0;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   if ( !m_vGridApp )
      return ;

   if ( pEvent->m_dispid != DISPID_UnboundReadData )
      return ;

   TRY
   {  // Extract Parameter
      LPDISPATCH pRowBufDispatch = NULL;

      pRowBufDispatch      = pEvent->m_pDispParams->rgvarg[2].pdispVal;
      VARIANT * pStartLoc  = pEvent->m_pDispParams->rgvarg[1].pvarVal;
      // we got a start location and make a bookmark out of it no matter whether its NULL
      COleVariant vBookmark ( pStartLoc );

      if (!pRowBufDispatch )// no chance to create rowbuffer object
         return;
      // instanciate wrapper class RowBuffer using dispatchpointer to get row and column count
      RowBuffer rowBuf( pRowBufDispatch );
      zLONG lRowCount = rowBuf.GetRowCount ();
      zLONG lColCount = rowBuf.GetColumnCount ();

      // more columns than mappings ?
      if ( m_pZActiveX->m_MapList.GetSize() == 0 ||
           lColCount > m_pZActiveX->m_MapList.GetSize() )
      {
         TraceLineS("*** Column / Mapping count mismatch Mapping aboarded", *(m_pZActiveX->m_pzsTag) );
         rowBuf.DetachDispatch ();
         return;
      }
//#define _DEBUG_READ_UNBOUND
#ifdef _DEBUG_READ_UNBOUND
      TraceLineS("   Parameter Trace", "");
#endif
      // check read direction
      BOOL bReadPrev = FALSE;
      if ( pEvent->m_pDispParams->rgvarg[0].boolVal != VARIANT_FALSE )
      {  // set flag for read previous
         ulCursorFlags = m_pZActiveX->m_ulFlag | zPOS_PREV;
         bReadPrev = TRUE;
#ifdef _DEBUG_READ_UNBOUND
         TraceLineS("      bReadPriorRows = ", "TRUE");
#endif
      }
      else
      {  // set flag for read next
         ulCursorFlags = m_pZActiveX->m_ulFlag | zPOS_NEXT;
         bReadPrev = FALSE;
#ifdef _DEBUG_READ_UNBOUND
         TraceLineS("      bReadPriorRows = ", "FALSE");
#endif
      }
      // position cursor to the first, last or current row
      // a bookmark of NULL tell us to set cursor to BOF
      if ( IsVariantNULL ( vBookmark ) )
      {
#ifdef _DEBUG_READ_UNBOUND
         TraceLineS("      startlocation vBookmark = ", "NULL");
#endif
         if ( bReadPrev )
            nRC = SetCursorLastEntity( m_vGridApp, m_strEntityName, m_strScopeEntityName );
         else
            nRC = SetCursorFirstEntity( m_vGridApp, m_strEntityName, m_strScopeEntityName );
      }
      else
      {
         lPosition = GetPositionFromBookmark( vBookmark );
#ifdef _DEBUG_READ_UNBOUND
         TraceLineI("      startlocation vBookmark = ", lPosition );
#endif
         nRC = SetEntityCursor( m_vGridApp, m_strEntityName, 0,
                    zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                    lPosition, m_strScopeEntityName, 0 );
         if ( nRC == zCURSOR_SET )
         {
            nRC = SetEntityCursor( m_vGridApp, m_strEntityName, 0,
                          ulCursorFlags, 0, 0, 0,
                          0, m_strScopeEntityName, 0 );
         }
      }

      // assuming we have a valid cursor
      for ( lRow = 0; lRow < lRowCount && nRC == zCURSOR_SET; lRow++ )
      {
         lPosition = GetRelativeEntityNumber( m_vGridApp, m_strEntityName, 0, 0 );

         zSHORT sVisibleCol = 0;
         for ( lCol = 0; lCol < m_pZActiveX->m_MapList.GetSize(); lCol++ )
         {
            pMapping = m_pZActiveX->m_MapList[ lCol ];
            if ( pMapping && pMapping != pMapList && pMapping != pMapScope )
            {
               LPSTR szValue = strValue.GetBufferSetLength( 255 );
               GetVariableFromAttribute( szValue, 0, zTYPE_STRING,
                                               254,
                                               m_vGridApp,
                                               pMapping->m_csEName,
                                               pMapping->m_csAName,
                                               pMapping->m_csContext,
                                               *pMapping->m_csContext ?
                                                zACCEPT_NULL_ENTITY :
                                                zACCEPT_NULL_ENTITY |
                                                      zUSE_DEFAULT_CONTEXT );

               strValue.ReleaseBuffer();
               vValue = strValue;
               rowBuf.SetValue (lRow, sVisibleCol, vValue);
               sVisibleCol++;
            }
         }// end: for each column
         // setting bookmark
         COleVariant vB = rowBuf.GetBookmark( lRow );
         MakeBookmark( lPosition, vB );
         rowBuf.SetBookmark (lRow, vB );
#ifdef _DEBUG_READ_UNBOUND
         TraceLineI("         Current vBookmark = ", lPosition );
#endif

         // setup multiple selection for control
         if ( GetSelectStateOfEntity( m_vGridApp, m_strEntityName ) == 1 )
         {
            COleVariant v;
            v = GetSelBookmarks( );
            LPDISPATCH lpSelDispatch = v.pdispVal;
            SelBookmarks Selection( lpSelDispatch );
            Selection.Add( vB );
            #ifdef _DEBUG
            TraceLineI( "Selection added for Bookmark ", vB.lVal );
            #endif
         }// setup selection for control
         // next row
         nRC = SetEntityCursor( m_vGridApp, m_strEntityName, 0,
                          ulCursorFlags, 0, 0, 0,
                          0, m_strScopeEntityName, 0 );

      }// end for each rowbuffer
      rowBuf.SetRowCount( lRow );
      rowBuf.DetachDispatch ();
   }
   CATCH ( COleException, e )
   {
      e->ReportError();
   }
   END_CATCH
}



void
IMsDgridCtrl::OnUnboundWriteData ( )
{
#ifdef _DEBUG
   CString strMethodName = "IMsDgridCtrl::UnboundWriteData";
   TraceLineS("<UsrDll> Method : ", strMethodName );
#endif

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;
   zSHORT nRC = 0;
   BOOL bProceed = FALSE;
   VARIANT *pWriteLoc = NULL;
   long lPosition = 0;

   // If we cannot get the view ... quit.
   zVIEW vApp;
   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      return;
   }

   // If we have a View name and a Scoping (list) Entity name ... continue
   if ( m_strViewName.IsEmpty( ) || m_strEntityName.IsEmpty( ) )
      return;


   TRY
   {
      LPDISPATCH pRowBufDispatch = pEvent->m_pDispParams->rgvarg[1].pdispVal;
      pWriteLoc = pEvent->m_pDispParams->rgvarg[0].pvarVal;

      if (!pRowBufDispatch )
        return;

      RowBuffer rowBuf(pRowBufDispatch);

      if ( pEvent->m_dispid == DISPID_UnboundDeleteData )
      {
         COleVariant v( pWriteLoc );
         long lPosition = 0;
         lPosition = GetPositionFromBookmark( v );
         nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                    zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                    lPosition, m_strScopeEntityName, 0 );
         if ( nRC > zCURSOR_UNCHANGED )
         {
            if ( DeleteEntity (vApp, m_strEntityName, zREPOS_NONE ) == zCALL_ERROR )
            {
               // set bookmark to NULL to indicate error to Grid
               pWriteLoc->vt = VT_NULL;
            }
         }
         else
         {
            // set bookmark to NULL to indicate error to Grid
            pWriteLoc->vt = VT_NULL;
         }
         return;
      }
      else
      {
         // get the rowbuffer dispatch pointer and instanciate wrapper
         // reverse order in Dispatch-Params, so RowBuffer=Index 1

         bProceed = FALSE;

         zLONG lRowCount = rowBuf.GetRowCount ();
         zLONG lColCount = rowBuf.GetColumnCount ();

         if ( m_pZActiveX->m_MapList.GetSize() == 0 ||
              lColCount > m_pZActiveX->m_MapList.GetSize() )
         {
            // more columns than mappings ?
            TraceLineS("*** Columns / Mappings mismatch ", "Mapping aboarded");

            rowBuf.DetachDispatch();
            return;
         }
         if ( pEvent->m_dispid == DISPID_UnboundWriteData )
         {
            /*
            pWriteLoc = pEvent->m_pDispParams->rgvarg[0].pvarVal;
            COleVariant v( pWriteLoc );
            lPosition = GetPositionFromBookmark( v );
            nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                       zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                       lPosition, m_strScopeEntityName, 0 );
            if ( nRC > zCURSOR_UNCHANGED )
               bProceed = TRUE;
            */
         }
         else
         if ( pEvent->m_dispid == DISPID_UnboundAddData )
         {
            //bProceed = (CreateEntity (vApp, m_strEntityName, zPOS_LAST) == 0 );
            bProceed = FALSE;
            if ( bProceed )
            {
               lPosition = GetRelativeEntityNumber( vApp, m_strEntityName, 0, m_pZActiveX->m_ulFlag );
               MakeBookmark ( lPosition, pWriteLoc );
            }
         }

      // the following is identical for AddData and WriteData, Cursor is positioned to correct position
         if  ( bProceed && pEvent->m_dispid != DISPID_UnboundWriteData )
         {
            zSHORT sVisibleColumn = 0;

            for ( long lCol = 0; lCol < m_pZActiveX->m_MapList.GetSize(); lCol++ )
            {
               pMapping = m_pZActiveX->m_MapList[ lCol ];
               if ( pMapping && pMapping != pMapList && pMapping != pMapScope )
               {
                  COleVariant v;
                  CString str;

                  v.Clear();
                  str.Empty();
                  v = rowBuf.GetValue (0, sVisibleColumn );

                  /* transfer data to entity-attribute if != NULL
                  (WriteData delivers only changed column-values with values != NULL.
                  Unchanged attributes are transferred with NULL-values.
                  For AddData, same code can be used as only non-null-values have to be
                  filled in the entity.
                  */

                  if ( v.vt != VT_NULL)
                  {
                     v.ChangeType (  VT_BSTR );
                     str = v.bstrVal;
                     nRC = SetAttributeFromVariable( vApp,
                                                  pMapping->m_csEName,
                                                  pMapping->m_csAName,
                                                  (LPCSTR) str,
                                                  zTYPE_STRING,
                                                  (zSHORT)str.GetLength(),
                                                  pMapping->m_csContext,
                                                  *pMapping->m_csContext ?
                                                      zACCEPT_NULL_ENTITY :
                                                      zACCEPT_NULL_ENTITY |
                                                            zUSE_DEFAULT_CONTEXT );

                  }
                  sVisibleColumn++;
               }
            }  // for each column
         }   // if proceed
         else
         {
            if ( pEvent->m_dispid != DISPID_UnboundWriteData )
            {
              // indicate error to Grid
              rowBuf.SetRowCount (0);
            }
         }
      }

      rowBuf.DetachDispatch ();
   }
   CATCH ( COleException, e )
   {
      CString strErrorMessage;
      LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
      e->GetErrorMessage( lpErrorMessage, 1024 );
      strErrorMessage.ReleaseBuffer();
      TraceLineS( "MsDBGrid - Exception Handling OnUnboundWriteData ", strErrorMessage  );
      e->ReportError();
   }
   END_CATCH
}



zLONG
IMsDgridCtrl::OnRowColChange( )
{
#ifdef DEBUG_ALL
   CString strMethodName = "IMsDgridCtrl::OnRowColChange";
   TraceLineS("<UsrDll> Method : ", strMethodName );
#endif

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we call in the right context
   if ( pEvent->m_dispid != DISPID_RowColChange )
   {
      TraceLineI( "MsDBGrid - OnRowColChange called on wrong event ", pEvent->m_dispid );
      return( -1 )
   }

   // extract parameter
   COleVariant vLastRow ( pEvent->m_pDispParams->rgvarg[1].pvarVal );
   VARIANT     *pvLastRow = ( pEvent->m_pDispParams->rgvarg[1].pvarVal );
   zSHORT      nLastCol =   ( pEvent->m_pDispParams->rgvarg[0].iVal );
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
         zLONG lCurrentRow = GetPositionFromBookmark( vCurrentRow );
         zLONG lLastRow = GetPositionFromBookmark( vLastRow );
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
         TraceLineS( "MsDBGrid - Exception Handling OnRowColChange ", strErrorMessage  );
      }
      END_CATCH_ALL
   }
   return lPosition;
}

//
void
IMsDgridCtrl::SetGridCursorFromViewCursor( )
{

   zLONG lPosition = 0;

   TRY
   {
      // If we have a View name and a Scoping (list) Entity name ... continue
      if ( m_strViewName.IsEmpty( ) || m_strEntityName.IsEmpty( ) )
      {
         TraceLineS( "MsDBGrid - ","Empty View or Entityname passed"  );
         return;
      }
      // get the view and position cursor.
      zVIEW vApp;
      if ( GetViewByName( &vApp, m_strViewName,
                          m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         TraceLineS( "Unable to load view for MsDBGrid - ", m_strViewName );
         lPosition = 0;
      }
      else
         lPosition = GetRelativeEntityNumber( vApp, m_strEntityName, 0, m_pZActiveX->m_ulFlag );

      if ( lPosition >= 0 )
      {
         zSHORT nColumn = GetCol();
//         SetCurrentCell (lPosition, nColumn );
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


void
IMsDgridCtrl::GridSetSelection( )
{
   zLONG lPosition = 0;

   TRY
   {

      // If we have a View name and a Scoping (list) Entity name ... continue
      if ( m_strViewName.IsEmpty( ) || m_strEntityName.IsEmpty( ) )
      {
         TraceLineS( "MsDBGrid - ","Empty View or Entityname passed"  );
         return;
      }
      // get the view and position cursor.
      zVIEW vApp;
      if ( GetViewByName( &vApp, m_strViewName,
                          m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         TraceLineS( "Unable to load view for MsDBGrid - ", m_strViewName );
         lPosition = 0;
         return;
      }
      else
         lPosition = GetRelativeEntityNumber( vApp, m_strEntityName, 0, m_pZActiveX->m_ulFlag );

      COleVariant vCurrentRow = GetBookmark();
      MakeBookmark (lPosition, vCurrentRow );

      SelBookmarks *sb = NULL;

      COleVariant vSelBM ( GetSelBookmarks() );

      if ( !IsVariantNULL( vSelBM ))
      {
         sb = new SelBookmarks( vSelBM.pdispVal );
         if ( sb )
         {
            COleVariant vIndex = 0L;
            while ( sb->GetCount() > 0 )
            {
               sb->Remove( vIndex );
            }
            sb->Add( vCurrentRow );
            delete sb;
         }
      }
   }
   CATCH ( COleException, e )
   {
      e->ReportError();
   }
   END_CATCH
}

zLONG
IMsDgridCtrl::SetViewCursorFromGridCursor( COleVariant vCurrentRow )
{
   // If we have a View name and a Scoping (list) Entity name ... continue
   if ( m_strViewName.IsEmpty( ) || m_strEntityName.IsEmpty( ) )
   {
      TraceLineS( "MsDBGrid - ","Empty View or Entityname passed"  );
      return( -3 );
   }
   // get the view and position cursor.
   zVIEW vApp;
   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
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
      TraceLineI( "MsDBGrid - set cursor failed. nRC = ", nRC  );
      return -5;
   }
   return lPosition;
}

zSHORT
IMsDgridCtrl::MapFromOI( )
{
#ifdef DEBUG_ALL
   TraceLineS( "IMsDgridCtrl::MapFromOI ", *m_pZActiveX->m_pzsTag );
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
            long lEntityCount = CountEntitiesForView( m_vGridApp, m_strEntityName );
            this->SetApproxCount( lEntityCount );
            Refresh(  );
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

// ---------------------------------------------
//
// MapToOI
//
// ---------------------------------------------
zSHORT
IMsDgridCtrl::MapToOI( )
{
   if ( m_pZActiveX->m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
   {
      m_pZActiveX->m_ulMapActFlags |= zMAPACT_ENABLED;
      m_pZActiveX->m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
   }

   m_pZActiveX->m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;

   UpdateAllColumnValues();

   return ( 0 );
}

void
IMsDgridCtrl::OnUnboundGetRelativeBookmark( )
{
#ifdef DEBUG_ALL
   CString strMethodName = "IMsDgridCtrl::UnboundGetRelativeBookmark";
   TraceLineS("<UsrDll> Method : ", strMethodName );
#endif

   AFX_EVENT   *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   VARIANT     *pStartLoc = NULL;
   zLONG        lOffset = 0;
   VARIANT     *pNewLoc = NULL;
   zLONG       *lApproximatePosition = NULL;
   zLONG lPosition = 0;
   zSHORT   nRC = zCURSOR_SET;
   unsigned long ulCursorFlags = 0;

   if ( pEvent->m_dispid != DISPID_UnboundGetBookmark )
      return;

   if( !m_vGridApp )
      return;

   // Extract Parameter
   TRY
   {
      pStartLoc            = pEvent->m_pDispParams->rgvarg[3].pvarVal;
      lOffset              = pEvent->m_pDispParams->rgvarg[2].lVal;
      pNewLoc              = pEvent->m_pDispParams->rgvarg[1].pvarVal;
      lApproximatePosition = pEvent->m_pDispParams->rgvarg[0].plVal;

      //*lApproximatePosition = 0;

      //set pNewLoc to NULL just for an error case
      pNewLoc->vt = VT_EMPTY;

      // we got a start location and make a bookmark out of it
      // no matter whether its NULL or carries an other value
      COleVariant vBookmark ( pStartLoc );
      if ( vBookmark.vt != zDBGRID_VT_BOOKMARK
           && !IsVariantNULL ( vBookmark ) )
      {
         // something strange happend
         // obviously a GRID-BUG because we always pass a bookmark
         // But I think I can deal with it by returning the current cursor position of
         // the view that is mapped to the control

         // If we cannot get the view ... quit.
         zVIEW vApp = 0;

         if ( GetViewByName( &vApp, m_strViewName,
                             m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
         {
            TraceLineS( "Unable to load view for MsDBGrid - ", m_strViewName );
            return;
         }
         lPosition = GetRelativeEntityNumber( vApp, m_strEntityName, 0, m_pZActiveX->m_ulFlag );
         MakeBookmark( lPosition, pNewLoc );
         *lApproximatePosition = lPosition;
      }
      else
      {
         // position cursor to the first or last row
         // a bookmark of NULL tell us to set cursor to EOF or BOF
         // depending on read direction
         if ( IsVariantNULL ( vBookmark ) )
         {
            if ( lOffset < 0 )
            {
               // Set cursor to EOF
               lOffset = lOffset * (-1); //to get a positiove value for SetCursorRelativePosition....
               ulCursorFlags = m_pZActiveX->m_ulFlag | zPOS_LAST | zPOS_RELATIVE;
            }
            else
            {
               // Set cursor to BOF
               ulCursorFlags = m_pZActiveX->m_ulFlag | zPOS_FIRST | zPOS_RELATIVE;
            }
         }
         else
         {
            lPosition = GetPositionFromBookmark( vBookmark );
            lOffset = lPosition + lOffset;
            ulCursorFlags = m_pZActiveX->m_ulFlag | zPOS_FIRST | zPOS_RELATIVE;
            if ( lOffset < 0 )
            {
               return;
            }
         }

         // the cursor has been set properly
         if ( nRC > zCURSOR_UNCHANGED )
         {
            nRC = SetEntityCursor( m_vGridApp, m_strEntityName, 0,
                                    ulCursorFlags , 0, 0, 0,
                                    lOffset, m_strScopeEntityName, 0 );
            if ( nRC > zCURSOR_UNCHANGED )
            {
               lPosition = GetRelativeEntityNumber( m_vGridApp, m_strEntityName, 0, m_pZActiveX->m_ulFlag );
               MakeBookmark( lPosition, pNewLoc );
               *lApproximatePosition = lPosition;
            }
         }
      }
   }
   CATCH ( COleException, e )
   {
      e->ReportError();
   }
   END_CATCH

}

zLONG
IMsDgridCtrl::OnAfterColUpdate( )
{
   zLONG nRC = 0;
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   zSHORT nEventCol = pEvent->m_pDispParams->rgvarg[0].iVal;
   zVIEW vApp = 0;

   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      nRC = -1;
   }

   if ( nRC >= 0 )
   {
      if ( m_pZActiveX->m_MapList.GetSize() <= nEventCol )
      {
         nRC = -2;
      }
   }

   if ( nRC >= 0 )
   {
      pMapping = m_pZActiveX->m_MapList[ nEventCol ];
      if ( pMapping )
      {
         CString strText = GetText();
         nRC = SetAttributeFromVariable( vApp,
                                         pMapping->m_csEName,
                                         pMapping->m_csAName,
                                         (LPCSTR) strText,
                                         zTYPE_STRING,
                                         (zSHORT)strText.GetLength(),
                                         pMapping->m_csContext,
                                         *pMapping->m_csContext ?
                                             zACCEPT_NULL_ENTITY :
                                             zACCEPT_NULL_ENTITY |
                                                   zUSE_DEFAULT_CONTEXT );
      }
   }

   return nRC;

}

zLONG
IMsDgridCtrl::OnBeforeColUpdate( )
{
   zLONG nRC = 0;
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   zSHORT *pnCancel = pEvent->m_pDispParams->rgvarg[0].piVal;
   VARIANT *pvOldValue = pEvent->m_pDispParams->rgvarg[1].pvarVal;
   zSHORT nColIndex = pEvent->m_pDispParams->rgvarg[2].iVal;
   zVIEW vApp = 0;

   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      nRC = -1;
   }

   if ( nRC >= 0 )
   {
      if ( m_pZActiveX->m_MapList.GetSize() <= nColIndex )
      {
         nRC = -2;
      }
   }

   if ( nRC >= 0 )
   {
      pMapping = m_pZActiveX->m_MapList[ nColIndex ];
      if ( pMapping )
      {
         CString strText = GetText();
         nRC = SetAttributeFromVariable( vApp,
                                         pMapping->m_csEName,
                                         pMapping->m_csAName,
                                         (LPCSTR) strText,
                                         zTYPE_STRING,
                                         (zSHORT)strText.GetLength(),
                                         pMapping->m_csContext,
                                         *pMapping->m_csContext ?
                                             zACCEPT_NULL_ENTITY | zVALIDATE :
                                             zACCEPT_NULL_ENTITY | zVALIDATE |
                                                   zUSE_DEFAULT_CONTEXT );
         if ( zCALL_ERROR == nRC )
         {
            *pnCancel = 1;
         }
      }
   }
   return nRC;
}


zLONG
IMsDgridCtrl::OnKeyDown()
{
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)( m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   zSHORT *KeyCode = pEvent->m_pDispParams->rgvarg[1].piVal;
   zSHORT nShift = pEvent->m_pDispParams->rgvarg[0].iVal;
   BOOL bCtrl = FALSE;
   zLONG nRC = 0L;

   if( ( (*KeyCode) == VK_DOWN ) )// nShift == 2 means the CTRL-Key has been pressed
   {
      // check whether it is a Combobox Column
      // if so create the combobox/listbox and
      // assing 0 to KeyCode to avoid further default pocessing
      TRY
      {
         Columns cols( GetColumns() );
         Column col = cols.GetColumn( GetCol() );
         if ( col.GetButton() )
         {
            *KeyCode = 0;
            CreateComboBox( GetCol() );
         }

      }
      CATCH_ALL ( e )
      {
         nRC = -1;
      }
      END_CATCH_ALL
   }
   return nRC;
}

zLONG
IMsDgridCtrl::OnMouseDown()
{
   zLONG nRC = 0L;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)( m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   zSHORT nButton = pEvent->m_pDispParams->rgvarg[3].iVal;
   zSHORT nShift  = pEvent->m_pDispParams->rgvarg[2].iVal;
   zLONG  lX      = pEvent->m_pDispParams->rgvarg[1].lVal;
   zLONG  lY      = pEvent->m_pDispParams->rgvarg[0].lVal;

   if( nButton == MK_LBUTTON && nShift == 1 )// Left button and SHIFT Key pressed?
   {
      // determine start position by getting the bookmark of the selected row
      double dTwip = double( lY );

      //dTwip = PixelToTwips( dTwip , FALSE );
      //dTwip = TwipsToPixel( dTwip , FALSE );
      dTwip = dTwip * -1.00;
      //dTwip = dTwip / 1000.0;

      zSHORT nRow = RowContaining( (float)dTwip );

      if( nRow < 0 ||  nRow >= GetVisibleRows( ) )
      {
         return nRC;
      }

      COleVariant vStart = RowBookmark( nRow );
      if ( IsVariantNULL( vStart ) )
      {
         return nRC;
      }

      // determine end position by getting the current bookmark
      COleVariant vEnd = GetBookmark();
      if ( IsVariantNULL( vEnd ) )
      {
         return nRC;
      }

      zVIEW vApp;
      if ( GetViewByName( &vApp, m_strViewName,
                          m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
         nRC = -1;
      }

      zVIEW vAppSelect = 0;
      CreateViewFromView( &vAppSelect, vApp );

      if ( !vAppSelect )
      {
         return nRC;
      }

      // GetSelBookmarks return a dispatch pointer that can be used to instatiate
      // a SelBookmarks object
      COleVariant v = GetSelBookmarks();
      SelBookmarks Selection( v.pdispVal );

      while ( Selection.GetCount( ) > 0 )
      {
         COleVariant v = 0L;
         Selection.Remove( v );
      }

      // Reset select flags for all entities
      SetAllSelectStatesForEntity( vAppSelect ,
                                   m_strEntityName,
                                   FALSE,
                                   m_strScopeEntityName );

      zLONG lStartPosition = GetPositionFromBookmark( vStart );
      zLONG lEndPosition   = GetPositionFromBookmark( vEnd   );

      // Set read direction
      zLONG ulCursorFlag = zPOS_NEXT;
      if ( lStartPosition > lEndPosition )
      {
         ulCursorFlag = zPOS_PREV;
      }

      zLONG lPosition = lStartPosition;

      nRC = SetEntityCursor( vApp , m_strEntityName, 0,
           zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
           lStartPosition, m_strScopeEntityName, 0 );

      while ( nRC == zCURSOR_SET )
      {
         SetSelectStateOfEntity( vApp , m_strEntityName, TRUE );

         lPosition = GetRelativeEntityNumber( vApp , m_strEntityName, 0, 0 );
         COleVariant vBookmark;
         MakeBookmark( lPosition, &vBookmark );
         Selection.Add( vBookmark );

         if ( lPosition == lEndPosition )
            nRC = zCURSOR_NULL;
         else
            nRC = SetEntityCursor( vApp , m_strEntityName, 0,
                          ulCursorFlag, 0, 0, 0,
                          0, m_strScopeEntityName, 0 );
      }

      DropView ( vAppSelect );
   }
   return nRC;
}

/***********************************************************************************
**
** helper functions for bookmark handling
**
***********************************************************************************/
void
IMsDgridCtrl::MakeBookmark( long lAbsolutePosition, LPVARIANT vBookmark )
{
   // Convert it into a type that should be used
   // when dealing with this grid
   vBookmark->vt = zDBGRID_VT_BOOKMARK;
   vBookmark->lVal = lAbsolutePosition;
}


long
IMsDgridCtrl::GetPositionFromBookmark( COleVariant v )
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
** helper function for converting coordinates from twips into pixels
**
***********************************************************************************/
double
IMsDgridCtrl::TwipsToPixel( double dTwipCoord, zBOOL bX )
{
   double dPixelPerInch = 0;
   double dPixel = 0.0;
   const double dTWIP_QUOTIENT = 1440.00;

   CDC* pDC = m_pZActiveX->GetDC( );

   if ( bX )
      dPixelPerInch = pDC->GetDeviceCaps( LOGPIXELSX );
   else
      dPixelPerInch = pDC->GetDeviceCaps( LOGPIXELSY );
   m_pZActiveX->ReleaseDC( pDC );

   dPixel  = dTwipCoord * dPixelPerInch / dTWIP_QUOTIENT;
   return dPixel;
}

double
IMsDgridCtrl::PixelToTwips( double dPixelCoord, zBOOL bX )
{
   double dPixelPerInch = 0;
   double dTwip = 0.0;
   const double dTWIP_QUOTIENT = 1440.00;

   CDC* pDC = m_pZActiveX->GetDC( );

   if ( bX )
      dPixelPerInch = pDC->GetDeviceCaps( LOGPIXELSX );
   else
      dPixelPerInch = pDC->GetDeviceCaps( LOGPIXELSY );
   m_pZActiveX->ReleaseDC( pDC );

   dTwip  = dPixelCoord * dTWIP_QUOTIENT / dPixelPerInch;
   return dTwip;
}

zSHORT IMsDgridCtrl::GetCellData( zPCHAR pchReturnData,
                                  zLONG lRowNbr, zLONG lColNbr )
{
   zSHORT nRC = 0;

   nRC = SetCurrentCell( lRowNbr, lColNbr );
   if ( nRC == 1 )
   {
      CString strText = GetText( );
      nRC = strText.GetLength( );
      zstrcpy( pchReturnData, strText );
   }

   return nRC;
}

zSHORT IMsDgridCtrl::SetCellData( zCPCHAR pchReturnData,
                                  zLONG lRowNbr, zLONG lColNbr )
{
   zSHORT nRC = 0;
   zVIEW vApp;
   CString strText = pchReturnData;

   nRC = SetCurrentCell( lRowNbr, lColNbr);
   if ( nRC != 1 )
      return nRC;

   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      return nRC;
   }

   nRC = 0;
   Columns *cols = NULL;
   TRY
   {
      LPDISPATCH lpDispatchColumns = NULL;
      lpDispatchColumns = GetColumns();
      if ( lpDispatchColumns )
      {
         cols = new Columns ( lpDispatchColumns );
         if ( cols )
         {
            Column col = cols->GetColumn( (short)lColNbr );
            ZMapping* pMapping = m_pZActiveX->m_MapList[ lColNbr ];
            if ( pMapping )
            {
               nRC = SetAttributeFromVariable( vApp,
                                               pMapping->m_csEName,
                                               pMapping->m_csAName,
                                               (LPCSTR) strText,
                                               zTYPE_STRING,
                                               (zSHORT)strText.GetLength(),
                                               pMapping->m_csContext,
                                               *pMapping->m_csContext ?
                                                   zACCEPT_NULL_ENTITY :
                                                   zACCEPT_NULL_ENTITY |
                                                         zUSE_DEFAULT_CONTEXT );
               if ( 0 == nRC )
               {
                  SetText( strText );
                  nRC = 0;
               }
            }
         }
      }
   }
   CATCH_ALL( e )
   {
      CString strErrorMessage;
      LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
      e->GetErrorMessage( lpErrorMessage, 1024 );
      strErrorMessage.ReleaseBuffer();
      TraceLineS( "MsDBGrid - Exception Handling SetCellData",
                  strErrorMessage  );
      nRC = 0;
   }
   END_CATCH_ALL

   return nRC;
}

zSHORT IMsDgridCtrl::GetActiveCell( zPLONG plRowNbr, zPLONG plColNbr )
{
   zSHORT nRC = 0;

   *plRowNbr = -1;
   *plColNbr = -1;

   TRY
   {
      *plColNbr = GetCol();

      COleVariant v = GetBookmark();
      zLONG lPosition = GetPositionFromBookmark( v );
      *plRowNbr = lPosition;
   }
   CATCH_ALL( e )
   {
      CString strErrorMessage;
      LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
      e->GetErrorMessage( lpErrorMessage, 1024 );
      strErrorMessage.ReleaseBuffer();
      TraceLineS( "MsDBGrid - Exception Handling GetActiveCell",
                  strErrorMessage  );

      nRC = 0;
   }
   END_CATCH_ALL
   return nRC;
}

zSHORT
IMsDgridCtrl::SetCurrentCell( zLONG lRowNbr, zLONG lColNbr, zBOOL bFromExternal )
{
   zSHORT nRC = 0;
   Columns *cols = NULL;

   if ( lRowNbr < 0 || lColNbr < 0 )
      return nRC;

   TRY
   {
      LPDISPATCH lpDispatchColumns = NULL;
      lpDispatchColumns = GetColumns();
      if ( lpDispatchColumns )
      {
         cols = new Columns ( lpDispatchColumns );
         if ( cols->GetCount() < lColNbr )
         {
            return nRC;
         }
      }
      else
      {
         return nRC;
      }

      // set the row to the requested position
      COleVariant v = GetBookmark();
      zLONG lPosition = GetPositionFromBookmark( v );
      // check to see whether an update has to occur
      if ( GetDataChanged() )
      {
         UpdateAllColumnValues( );
      }

      zLONG lOffset = lRowNbr - lPosition;
      if ( lOffset != 0 )
      {
         v = GetBookmark( lOffset );
         m_bExternalRowChange = bFromExternal;
         // if function is called from VML code there is a chance that
         // view and grid cursors get out of sync if it is followed
         // by a "Return Refresh" termination so the view cursor has to be set now
         if( m_bExternalRowChange )
         {
            zVIEW vApp = 0;
            lPosition = GetPositionFromBookmark( v );
            if ( GetViewByName( &vApp, m_strViewName,
                                m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
            {
               TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
            }
            else
            {
               nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                          zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                          lPosition, m_strScopeEntityName, 0 );
            }
         }
         SetBookmark( v );
      }
      SetCol( (short) lColNbr );
      nRC = 1;
   }
   CATCH_ALL( e )
   {
      CString strErrorMessage;
      LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
      e->GetErrorMessage( lpErrorMessage, 1024 );
      strErrorMessage.ReleaseBuffer();
      TraceLineS( "MsDBGrid - Exception Handling SetCurrentCell ", strErrorMessage  );
      nRC = 0;
   }
   END_CATCH_ALL
   return nRC;
}


zLONG
IMsDgridCtrl::OnButtonClick( )
{
   zLONG nRC = 0;
   AFX_EVENT *pEvent = (AFX_EVENT *)
              ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   short nColumn = pEvent->m_pDispParams->rgvarg[ 0 ].iVal;

   if ( CreateComboBox( nColumn ) )
      nRC = 1;

   return nRC;
}

BOOL IMsDgridCtrl::CreateComboBox(int nColumn)
{
   BOOL bReturn = TRUE;
   TRY
   {
      LPDISPATCH lpDispatchColumns = NULL;
      lpDispatchColumns = GetColumns();
      if ( lpDispatchColumns )
      {
         Columns cols( lpDispatchColumns );
         Column col = cols.GetColumn( nColumn );

         if ( m_pWndListArray == 0 )
         {
            m_pWndListBox = 0;
            m_lColumns = cols.GetCount( );
            m_pWndListArray = (IMsDbGridList **)
                          new char[ m_lColumns * zsizeof( IMsDbGridList *) ];

            for ( long k = 0; k < m_lColumns; k++ )
               m_pWndListArray[ k ] = 0;

            m_hFontTab = (HFONT) ::GetStockObject( DEFAULT_GUI_FONT );
            if ( m_hFontTab == 0 )
               m_hFontTab = (HFONT) ::GetStockObject( ANSI_VAR_FONT );
         }

         if ( m_pWndListBox &&
              ::IsWindowVisible( m_pWndListBox->m_hWnd ) &&
              m_pWndListBox == m_pWndListArray[ nColumn ] )
         {
            m_pWndListBox->ShowWindow( SW_HIDE );
            m_pWndListBox = 0;
            return( FALSE );
         }

         m_pWndListBox = m_pWndListArray[ nColumn ];

         if ( m_pWndListBox == 0 )
         {
            m_pWndListBox = new IMsDbGridList;
            m_pWndListArray[ nColumn ] = m_pWndListBox;
            m_pWndListBox->Create( WS_CHILD | WS_VISIBLE |
                                    WS_BORDER | WS_CLIPSIBLINGS |
                                    WS_VSCROLL | LBS_HASSTRINGS,
                                   CRect( 0, 0, 0, 0 ),
                                   m_pZActiveX->m_pZSubtask->m_pZView, -1 );
            // TMV
            m_pWndListBox->m_pParentGrid = this;
            m_pWndListBox->m_pParentMapAct = m_pZActiveX;
            m_pWndListBox->SendMessage( WM_SETFONT, (WPARAM) m_hFontTab );
            PopulateListBox( nColumn );
         }

         CRect rect;
         HWND hWnd = (HWND)GethWndEditor();
         ::GetWindowRect( hWnd, rect );

         rect.top = rect.top + rect.Height();

         m_pZActiveX->m_pZSubtask->m_pZView->ScreenToClient( rect );

         m_pWndListBox->SetWindowPos( &CWnd::wndTop,
                                      (INT) rect.left, (INT) rect.top,
                                      (INT) rect.Width(),
                                      (INT) rect.Height() + 100,
                                      SWP_SHOWWINDOW );
         m_pWndListBox->SetFocus( );
         CString strText = GetText( );
         m_pWndListBox->SelectString( -1, strText );
      }
   }

   CATCH_ALL( e )
   {
      CString strErrorMessage;
      LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
      e->GetErrorMessage( lpErrorMessage, 1024 );
      strErrorMessage.ReleaseBuffer();
      TraceLineS( "MsDBGrid - Exception Handling OnButtonClick ", strErrorMessage  );
      bReturn = FALSE;
   }
   END_CATCH_ALL

   return bReturn;
}


zLONG
IMsDgridCtrl::OnSelChange( )
{
   zLONG nRC = 0;
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;

   zVIEW vApp;
   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      nRC = -1;
   }

   zVIEW vAppSelect = 0;
   CreateViewFromView( &vAppSelect, vApp );

   if ( !vAppSelect )
   {
      return nRC;
   }

   // Reset select flags for all entities
   SetAllSelectStatesForEntity( vApp,
                                m_strEntityName,
                                FALSE,
                                m_strScopeEntityName );
/*   nRC = SetCursorFirstEntity( vAppSelect,
                                m_strEntityName,
                                m_strScopeEntityName );
   while( nRC == zCURSOR_SET )
   {
      SetSelectStateOfEntity( vAppSelect , m_strEntityName, FALSE );
      nRC = SetCursorNextEntity( vAppSelect,
                                m_strEntityName,
                                m_strScopeEntityName );
   }
*/
   // GetSelBookmarks return a dispatch pointer that can be used to instatiate
   // a SelBookmarks object
   COleVariant v = GetSelBookmarks();
   SelBookmarks Selection( v.pdispVal );

   long nCount = Selection.GetCount( );

   // Loop through all entries, retrieve the bookmark which is the
   // absolute entity position
   // set the cursor and set select state

   for ( long nIndex = 0; nIndex < nCount; nIndex++ )
   {
      v = nIndex;
      v = Selection.GetItem( v );
      long lPosition = 0;
      lPosition = GetPositionFromBookmark( v );
      nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                 zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                 lPosition, m_strScopeEntityName, 0 );
      if ( nRC > zCURSOR_UNCHANGED )
      {
         SetSelectStateOfEntity (vApp, m_strEntityName, TRUE );
      }
   }

   DropView( vAppSelect );
   return nRC;
}

// handles HeadClick event.
// it orders the entities
zLONG
IMsDgridCtrl::OnHeadClick( )
{
   zLONG nRC = 0;
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   zSHORT nEventCol = pEvent->m_pDispParams->rgvarg[0].iVal;
   zVIEW vApp = 0;

   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      nRC = -1;
   }

   if ( nRC >= 0 )
   {
      if ( m_pZActiveX->m_MapList.GetSize() <= nEventCol )
      {
         nRC = -2;
      }
   }

   if ( nRC >= 0 )
   {
      if ( m_nCurrentOrderCol != nEventCol )
      {
         // set order mode to ascending
         m_nCurrentOrderMode = 0;
         m_nCurrentOrderCol = nEventCol;
      }
      else
      {
         // switch order mode
         if ( m_nCurrentOrderMode == 0 )
         {
            m_nCurrentOrderMode = 1;
         }
         else
         {
            m_nCurrentOrderMode = 0;
         }
      }

      pMapping = m_pZActiveX->m_MapList[ nEventCol ];
      if ( pMapping )
      {
         CString strOrderKey = pMapping->m_csAName;
         if ( m_nCurrentOrderMode == 0 )
         {
            strOrderKey += " A";
         }
         else
         {
            strOrderKey += " D";
         }

         OrderEntityForView( vApp, pMapping->m_csEName, strOrderKey );
         m_pZActiveX->m_ulMapActFlag2 |= zMAPACT_MAPPING_FROM_OI;
         InitMapping(  );
         Refresh();
      }
   }

   return nRC;
}

void
IMsDgridCtrl::PopulateListBox( zSHORT nColumn )
{
   zVIEW vApp = 0;

   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      return;
   }

   if ( ::IsWindow( m_pWndListBox->m_hWnd ) )
   {
      CString strHelp;


      if ( m_pZActiveX->m_MapList.GetSize() <= nColumn )
      {
         return;
      }

      ZMapping *pMapping = m_pZActiveX->m_MapList[ nColumn ];
      zPVOID pvCursor = NULL;
      PCHAR pchBuffer = strHelp.GetBufferSetLength( pMapping->m_ulDisplayLth );

      zSHORT  nRC = GetFirstTableEntryForAttribute( pchBuffer, pMapping->m_ulDisplayLth, vApp, pMapping->m_csEName, pMapping->m_csAName, pMapping->m_csContext, &pvCursor );
      while ( nRC == 0 )
      {
         strHelp.ReleaseBuffer();
         m_pWndListBox->AddString( strHelp );
         pchBuffer = strHelp.GetBufferSetLength( pMapping->m_ulDisplayLth );
         nRC = GetNextTableEntryForAttribute( pchBuffer, pMapping->m_ulDisplayLth, vApp, pMapping->m_csEName, pMapping->m_csAName, pMapping->m_csContext, &pvCursor );
      }

      m_pWndListBox->m_nCurrentColumn = nColumn;
   }
}

IMsDgridCtrl::~IMsDgridCtrl()
{

   if ( m_pWndListArray )
   {
      for ( zLONG k = 0; k < m_lColumns; k++ )
      {
         m_pWndListBox = m_pWndListArray[ k ];
         delete( m_pWndListBox );
      }

      delete [] m_pWndListArray;
   }

   POSITION pos = m_EventList.GetHeadPosition( );
   while ( m_EventList.IsEmpty() == FALSE )
   {
      CActiveXEvent *a = (CActiveXEvent *)m_EventList.RemoveHead( );
      delete a;
   }
}


void
IMsDgridCtrl::UpdateAllColumnValues()
{
   zVIEW vApp;

   // Set Cursor to the row currently accessed by the grid
   if ( GetViewByName( &vApp, m_strViewName,
                       m_pZActiveX->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_strViewName );
      return;
   }

   COleVariant v = GetBookmark();

   long lPosition = 0;
   lPosition = GetPositionFromBookmark( v );
   short nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                 zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                 lPosition, m_strScopeEntityName, 0 );

   if ( nRC <= zCURSOR_UNCHANGED )
      return;

   Columns *cols = NULL;
   TRY
   {
      LPDISPATCH lpDispatchColumns = NULL;
      lpDispatchColumns = GetColumns();
      if ( lpDispatchColumns )
      {
         cols = new Columns ( lpDispatchColumns );
         if ( cols )
         {  // loop all columns
            for ( short i = 0; i < cols->GetCount(); i++ )
            {
               Column col = cols->GetColumn( i );
               ZMapping* pMapping = m_pZActiveX->m_MapList[ i ];
               if ( pMapping && col.GetDataChanged() )
               {
                  CString strText = col.GetText();
                  nRC = SetAttributeFromVariable( vApp,
                                                  pMapping->m_csEName,
                                                  pMapping->m_csAName,
                                                  (LPCSTR) strText,
                                                  zTYPE_STRING,
                                                  (zSHORT)strText.GetLength(),
                                                  pMapping->m_csContext,
                                                  *pMapping->m_csContext ?
                                                      zACCEPT_NULL_ENTITY :
                                                      zACCEPT_NULL_ENTITY |
                                                            zUSE_DEFAULT_CONTEXT );
#ifdef _DEBUG /////////////////////////////////////////////////////////////////////
                  if ( zCALL_ERROR == nRC )
                  {
                     CString strHelp = m_strViewName;
                     strHelp += ".";
                     strHelp += pMapping->m_csEName;
                     strHelp += ".";
                     strHelp += pMapping->m_csAName;
                     strHelp += " = ";
                     strHelp += strText;
                     TraceLineS( "MSDBGRID Assignment error ", strHelp );
                  }
#endif ////////////////////////////////////////////////////////////////////////////
               }
            }
         }
      }
      if( cols )
         delete cols;

   }
   CATCH_ALL(e)
   {
      if( cols )
         delete cols;
   }
   END_CATCH_ALL
}



/////////////////////////////////////////////////////////////////////////////
// IMsDbGridList

BEGIN_MESSAGE_MAP(IMsDbGridList, CListBox )
   //{{AFX_MSG_MAP(IMsDbGridList)
      // NOTE - the ClassWizard will add and remove mapping macros here.
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_KILLFOCUS()
   ON_WM_KEYDOWN()
   ON_WM_SETFOCUS()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMsDbGridList::IMsDbGridList()
{
   m_pWndLastFocus = 0;
   m_pParentMapAct = 0;
}

IMsDbGridList::~IMsDbGridList()
{
}

/////////////////////////////////////////////////////////////////////////////
// IMsDbGridList message handlers

void
IMsDbGridList::OnLButtonUp( UINT uModKeys, CPoint pt )
{
   // TODO: Add your control notification handler code here
   TraceLineS( "IMsDbGridList::OnLButtonUp", "" );
   if ( m_pParentGrid )
   {
      // set current cell text
      CString strText;
      zLONG   lIndex = GetCurSel();
      if ( lIndex >= 0 )
      {
         GetText( GetCurSel(), strText );
         m_pParentGrid->SetText( strText );
         UpdateEntityAttributeImmediate( strText );
      }
   }
      if ( m_pWndLastFocus )
   {
      if ( ::IsWindow( m_pWndLastFocus->m_hWnd ) )
         m_pWndLastFocus->SetFocus( );

      m_pWndLastFocus = 0;
   }

   ShowWindow( SW_HIDE );
}

void
IMsDbGridList::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
   TraceLineS( "IMsDbGridList::OnLButtonDblClk", "" );

   if ( m_pParentGrid )
   {
      // set current cell text
      CString strText;
      zLONG   lIndex = GetCurSel();
      if ( lIndex >= 0 )
      {
         GetText( lIndex, strText );
         m_pParentGrid->SetText( strText );
         UpdateEntityAttributeImmediate( strText );
      }
   }

   if ( m_pWndLastFocus )
   {
      if ( ::IsWindow( m_pWndLastFocus->m_hWnd ) )
         m_pWndLastFocus->SetFocus( );

      m_pWndLastFocus = 0;
   }

   ShowWindow( SW_HIDE );
}

void
IMsDbGridList::OnSetFocus(CWnd* pOldWnd)
{
   TraceLineS( "IMsDbGridList::OnSetFocus", "" );
   CListBox::OnSetFocus(pOldWnd);
   // TODO: Add your message handler code here
   m_pWndLastFocus = pOldWnd;

}


void
IMsDbGridList::OnKillFocus(CWnd* pNewWnd)
{
   TraceLineS( "IMsDbGridList::OnKillFocus", "" );
   CListBox::OnKillFocus(pNewWnd);

   // TODO: Add your message handler code here
   ShowWindow( SW_HIDE );

}



void
IMsDbGridList::UpdateEntityAttributeImmediate( CString strText )
{
   zVIEW vApp = 0;

   if ( GetViewByName( &vApp, m_pParentGrid->m_strViewName,
                       m_pParentMapAct->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      TraceLineS( "Unable to load view for MSDBGRID - ", m_pParentGrid->m_strViewName );
      return;
   }

   ZMapping *pMapping = m_pParentMapAct ->m_MapList[ m_nCurrentColumn ];
   SetAttributeFromVariable( vApp,
                             pMapping->m_csEName,
                             pMapping->m_csAName,
                             (LPCSTR) strText,
                             zTYPE_STRING,
                             (zSHORT)strText.GetLength(),
                             pMapping->m_csContext,
                             *pMapping->m_csContext ?
                                 zACCEPT_NULL_ENTITY :
                                 zACCEPT_NULL_ENTITY |
                                       zUSE_DEFAULT_CONTEXT );
}


void IMsDbGridList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   TraceLineS( "IMsDbGridList::OnKeyDown", "" );
   // TODO: Add your message handler code here and/or call default
   CListBox::OnKeyDown(nChar, nRepCnt, nFlags);

   if ( m_pParentGrid )
   {
      // set current cell text
      CString strText;
      zLONG   lIndex = GetCurSel();
      if ( lIndex >= 0 )
      {
         GetText( GetCurSel(), strText );
         m_pParentGrid->SetText( strText );
         UpdateEntityAttributeImmediate( strText );
      }
   }

}


