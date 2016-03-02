/////////////////////////////////////////////////////////////////////////////
// Project TRUEGRID.DLL
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2000 Ton Beller AG.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         trdgmap.cpp wrapp the ITrueDBGridCtrl control
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of tdbg6.ocx as an Zeidon ActiveX-Control.
//
/* Change log most recent first order:

   2002.08.09  BL
      Modified GetCellData, SetCellData and added GetCheckBoxValueForSetData,
      GetCheckBoxValueForGetData: if column a CheckBox, than return/set the
      CheckBox Value (not Text Value)

   2002.2.25   TMV   2000   ActiveX
      Fix Bug R055676 toggle checkbox when SPACE Key is hit

   2001.01.16  BL
      insert TAB and RETURN at Editcursor position for a Edit field only

   2002.01.07  HH
      PostEvent was not registered (if trace was not active).

   2001.12.3   TMV   2000   ActiveX
      initialize cells when BeforeInsert Event occurs. ( Column.SetText("") );
      set default values for RadioButtons

   2001.11.26   TMV   2000   ActiveX
      Invoke a rebind even if there is no view

   2001.11.06   TMV   2000   ActiveX
      changed InitMapping to avoid a second call of ReBind which can
      force a crash under some strange circumstances

   2001.11.1   TMV   2000   ActiveX
      add handlers for events BeforeUpdate, AfterUpdate

   2001.10.23   TMV   2000   ActiveX
      Change Event handling of BeforeRowColChange to have a
      chance to cancel navigation

   2001.10.9   TMV   2000   ActiveX
      changed SetZControlState to check the possibility of enabling against
      readonly setting of the view prior to lock or unlock the grid

   2001.10.8   TMV   2000   ActiveX
      Changed StorageModeMappingEx to check column locking no matter
      if an entity exists

   2001.8.16   TMV   2000   ActiveX
      - Add TraceState for debugging
      - renamed SetRadioGroupsDefault to SetDefaultValueItem
      - changed SetDefaultValueItem to keep track about ComboBox defaults too.

   2001.8.16   TMV   2000   ActiveX
      - synchronize cursors if a single row is selected
      - enable deleting a single row
      - initial setup for RadioButtons if a new row is inserted via OnBeforeInsert

   2001.7.3   TMV   2000   ActiveX
      fix UpdateAllColumnValues to work correctly in RowInserMode.
      fix crash if user chooses Return/Refresh in RowInsertMode

   2001.6.18  BL    2000    ActiveX
      Modified LockColumn and SetCurrentCell for check uncorrekt
      Column Number

   2001.6.16  BL    2000    ActiveX
      Modified ~ITrueDBGridCtrl for drop Grid View

   2001.6.1   TMV   2000
      fix InitMapping to use OCX method ReBind instead of Refresh.
      This is for displaying a Grid with no Rows in it

   2001.4.26   TMV   2000   R55120/F51749
      - fix Insert behavior
      - changed GetPositionFromBookmark to check the incomming variant against
      NULL prior to call ChangeType to avoid an Exception

   2001.4.11   TMV   2000   ActiveX
      Add additional TraceLine for debugging purpose
      now current Row and Column are traced with each event

   2001.4.10   TMV   2000   ActiveX
      synchronize the cursors (View and Grid) by calling SetViewCursorFromGridCursor
      prior to do an update to the underlying zeidon object. This will fix
      a bug with the truegrid and navigating to a cell thats a checkbox

   2001.4.6   TMV   2000   ActiveX
      fix InitMapping XArrayDB does not like calling DeleteRows with an
      amount of 0 rows to delete

   2001.3.20   TMV   2000   ActiveX
      Introduced new method ZGridColumn::GetVariantFromAttribute to clean code

   2001.3.15   TMV   2000   ActiveX
      modified SetCurrentCell and OnRowColChange to unlock locked columns
      if possible and set cell cursor properly

   2001.3.13   TMV   2000   ActiveX
      Change OnHeadClick   to keep track about autosort setting
      Change OnComboSelect to route event to the user
      Change InitMapping   to delete all rows if mapped view is not available
      Handle DISPID_Click Event, return zACTIVEX_EVENT_COMPLETED to avoid calling
         into user code

   2001.3.6   TMV   2000   ActiveX
      use new ZGridColumn Method CheckLocking( zVIEW vApp ) to make sure a key
      attribute can be changed if needed even in new mapping mode

   2001.2.8   TMV   2000   ActiveX
      Implement complex combobox mapping to allow mapping of a combobox - List
      Entity.

   2001.01.25  TMV   2000   ActiveX
      Modified InitMapping to store the scoping entity name ( instead of the
      scoping view name which is definitely wrong ) in m_strScopeEntityName
      for later use.
      Check the mapped attributes whether they are updateble each time the row-focus
      has changed ( OnRowColChange ) to make sure a key attribute can be changed
      if needed

   2001.01.03  BL    2000   ActiveX
      Modified InitMapping for save the Name for temporaly View to member variable
      (if drop Application View with function DropObjectInstance, then m_vGridApp
      is not null and the Application View does not exists -> Error by DropView)

   2001.01.02  BL    2000   ActiveX
      Modified ITrueDBGridCtrl::Init() for set m_bInsertMode = FALSE

   2000.9.27   TMV   2000   ActiveX
      Fixing bug. DLL crashes if number of mappings exceeds number of columns.
      Crash occurs in ITrueDBGrid::StorageMappingMode()

   2000.9.22   TMV   2000   ActiveX
      Implement method GetGridColumn to avoid direct access to m_aGridCols
      Handle picture annotations with ComboBoxes

   2000.9.21   TMV   2000   ActiveX
      handle OnAddNew Event by default

   2000.9.19   TMV   2000   ActiveX
      fix for handling selectstates of an entity during Mapping

   2000.9.18   TMV   2000   ActiveX
      do inline documentation

*/

#include "zstdafx.h"
#include "tdbg6.h"
#include "trhlpr.h"

//#define _DEBUG_METHOD
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#ifdef _DEBUG
void ITrueDBGridCtrl::TraceState( CString strExtra )
{
   CString strTag( GetControlTag() );
   CString strMethodName = strTag + " - " + strExtra;
   COleVariant vRow = GetBookmark();
   zLONG lRow = GetPositionFromBookmark( vRow );
   short nCol = GetCol();
   CString strHelp;
   strHelp.Format( "Method : %s Row : %d, Column : %d", strMethodName, lRow, nCol );
   TraceLineS("<TrueGrid UsrDll> ", strHelp );
}
//#endif


ITrueDBGridCtrl::~ITrueDBGridCtrl()
{
   StoreLayoutChanges();

   // free list of CActiveXEvents
   while ( m_EventList.IsEmpty() == FALSE )
   {
      CActiveXEvent *a = (CActiveXEvent *)m_EventList.RemoveHead( );
      delete a;
   }

   for ( int i = 0; m_pWndArray && i < m_aGridCols.GetSize(); i++ )
   {
      if( m_pWndArray[i] )
         delete m_pWndArray[i];
   }
   delete [] m_pWndArray;

   while( m_aGridCols.GetSize() > 0 )
   {
      ZGridColumn * pGC = m_aGridCols[0];
      m_aGridCols.RemoveAt(0);
      delete pGC;
   }

   while( m_aGridSplits.GetSize() > 0 )
   {
      ZGridSplit * pGS = m_aGridSplits[0];
      m_aGridSplits.RemoveAt(0);
      delete pGS;
   }

}


/***********************************************************************************
**
** helper functions for bookmark handling
**
***********************************************************************************/


/**************************************************************************/
// Function name   : ITrueDBGridCtrl::MakeBookmark
// Description     : converts a given position into a variant that can be used as a BOOKMARK
//                   that is used to address a specific row in the grid
// Return type     : void
// Argument        : long lAbsolutePosition the position that has to be converted
// Argument        : LPVARIANT vBookmark target variant
/**************************************************************************/
void ITrueDBGridCtrl::MakeBookmark( long lAbsolutePosition, LPVARIANT vBookmark )
{
   // Convert it into a type that should be used
   // when dealing with this grid
   vBookmark->vt = zDBGRID_VT_BOOKMARK;
   vBookmark->lVal = lAbsolutePosition;
}



/**************************************************************************/
// Function name   : ITrueDBGridCtrl::GetPositionFromBookmark
// Description     : converts a given bookmark into a long value that can be used
//                   to address a specific row Entity in a view by calling
//                   SetCursorRelativePosition(...)
// Return type     : long position extracted from given bookmark
// Argument        : COleVariant v representing a bookmark
/**************************************************************************/
long ITrueDBGridCtrl::GetPositionFromBookmark( COleVariant v )
{
   // Convert it into a type that should be used
   // when dealing with this grid
   long lPosition = -1;
   COleVariant position;
   if( ::IsVariantNULL( v ) )
      lPosition = -1;
   else
   {
      position.ChangeType( zDBGRID_VT_BOOKMARK, v );
      lPosition = position.lVal;
   }
   return lPosition;
}

/***********************************************************************************
**
** helper functions for initialization
**
***********************************************************************************/

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::Init
// Description     : Initialize the current object instance
// Return type     : void
/**************************************************************************/
void ITrueDBGridCtrl::Init()
{
   m_strScopeEntityName = "";
   m_strEntityName      = "";
   m_strViewName        = "";
   m_bTraceEvents       = FALSE;
   m_bLayoutChanges     = FALSE;
   m_bInsertMode        = FALSE;
   m_bAutosortEnabled   = TRUE;
   m_nCurrentCol        = -1;
   m_bLock              = FALSE;
   m_pWndArray          = NULL;
   m_pWndDateCtrl       = NULL;
   short i;

   SetRowInsertMode( FALSE );

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
            MessagePrompt( GetDialogView(), "TRUEGRID SCR00001", "Invalid Property", strMsg,
                           TRUE, zBUTTONS_OK, 0, zICON_ERROR );
         }
         SetBorderStyle( 1 );

         // load column properties ( Mapping ) defined by Zeidon Painter dialogs
         LoadZeidonControlProperties();

         // retieve initial column colors
         Columns cols = Columns( GetColumns() );
         zLONG lColCount = cols.GetCount();
         for ( i = 0; i < lColCount; i++ )
         {
            Column c = cols.GetColumn( i );
            ZGridColumn *pGC = new ZGridColumn( this );
            pGC->SetColumnIndex( i );
            m_aGridCols.Add( pGC );
         }
         // retrieve initial Split colors
         Splits splits( GetSplits() );
         zLONG lSplitCount = splits.GetCount();
         for ( i = 0; i < lSplitCount; i++ )
         {
            Split s = splits.GetSplit( i );
            ZGridSplit *pGS = new ZGridSplit( this );
            pGS->SetSplitIndex( i );
            m_aGridSplits.Add( pGS );
         }

         LoadLayoutChanges();
      }
      CATCH( COleException, e )
      {
         e->ReportError();
         // clean up
         m_pZActiveX->SetWrapperInstance( NULL );
         CString strMsg = "Unable to request Data mode. Mapping code will not be called!";
         MessagePrompt( GetDialogView(), "TRUEGRID SCR00002", "Dispatch Driver", strMsg,
                        TRUE, zBUTTONS_OK, 0, zICON_ERROR );
      }
      END_CATCH
   }
}



/**************************************************************************/
// Function name   : ITrueDBGridCtrl::GetDialogView
// Description     : returns the view to the dialog
// Return type     : zVIEW
/**************************************************************************/
zVIEW ITrueDBGridCtrl::GetDialogView()
{
   if( !m_pZActiveX )
   {
      TraceLineS( "Invalid Zeidon Control Pointer ", "" );
      return( 0 );
   }
   if( !m_pZActiveX->m_pZSubtask )
   {
      TraceLineS( "Invalid Zeidon Subtask for control ", GetControlTag() );
      return( 0 );
   }

   return m_pZActiveX->m_pZSubtask->m_vDialog;
}



/**************************************************************************/
// Function name   : ITrueDBGridCtrl::GetControlTag
// Description     : returns the controls tag
// Return type     : LPCSTR
/**************************************************************************/
LPCSTR ITrueDBGridCtrl::GetControlTag()
{
   CString strHelp = "<Unknown>";
   if( m_pZActiveX )
   {
      strHelp = *m_pZActiveX->m_pzsTag;
   }
   return strHelp;
}



/**************************************************************************/
// Function name   : ITrueDBGridCtrl::InitMapping
// Description     : Initializes the mapping
// Return type     : zSHORT
/**************************************************************************/
zSHORT ITrueDBGridCtrl::InitMapping( )
{
   // setting maximum length of values to diplay for each attribute
   zLONG lCol;
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;
   zSHORT   nRC = 0;

   if ( pMapList && !pMapList->m_csVName.IsEmpty() )
   {
      m_strViewName = pMapList->m_csVName;   // a list entity was specified
      m_strEntityName = pMapList->m_csEName;
   }
   else
   {
      // new style
      POSITION pos = m_ZeidonColumns.GetFirstColumnPosition();
      if( pos )
      {
         ZGridColCtrl *c = m_ZeidonColumns.GetNextColumn( pos );
         m_strViewName = c->GetMapViewName();
         m_strEntityName = c->GetMapEntityName();
      }
      else
      {
         // for backward compatibility
         pMapping = m_pZActiveX->m_MapList[ 0 ];              // default to first entity as list
         m_strViewName = pMapping->m_csVName;
         m_strEntityName = pMapping->m_csEName;
      }
   }

   if ( pMapScope )
   {
      //m_strScopeEntityName = pMapScope->m_csVName;  // a scope entity was specified
      m_strScopeEntityName = pMapScope->m_csEName;  // a scope entity was specified
   }

   // reinitialize the grid having now no rows
   long lRowCount = m_xArrayDB.GetCount( zGRIDROWS );
   if( lRowCount > 0 )
      m_xArrayDB.DeleteRows( 0, lRowCount );


   // If we cannot get the view ... instanciate underlying XArray and thats it.
   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
   {
      LockGrid( -1, TRUE );
      SetRowInsertMode( FALSE );
      nRC = -1;
   }
   else
   {
      //Determine how the mappings are created if there is no entry
      //in the m_ZeidonColumns list the mapping was created using the generic
      //ActiveX configuration dialog. Otherwise mappings are created using TZAXCFGD
      if( m_ZeidonColumns.GetCount() )
      {
         InitMappingAttributeLength( vApp );
         PopulateListOfValues( vApp );
      }
      else
      {
         // support old style
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
      }
      LockGrid( -1, FALSE );
      nRC = 1;
   }

   return nRC;
}


/**************************************************************************/
// Function name   : ITrueDBGridCtrl::InitMappingAttributeLength
// Description     : Step throug all column mappings and retrieve the
//                   display length of that attribute. This is used to
//                   limit the input length for the column ( avoid domain errors)
// Return type     : void
// Argument        : zVIEW vApp  View that contains infomation about that mapping
/**************************************************************************/
void ITrueDBGridCtrl::InitMappingAttributeLength(zVIEW vApp)
{
   POSITION pos = m_ZeidonColumns.GetFirstColumnPosition();
   while( pos )
   {
      ZColumnMapping *pM;
      ZGridColCtrl * o = m_ZeidonColumns.GetNextColumn( pos );
      pM = o->GetColumnMapping();
      pM->InitMappingAttributeLength( vApp );
      if( o->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
      {
         // after each refresh the mapped List view has to be resreshed
         // so set a flag to retrieve new values the next time
         // the combobox button is clicked
         ((ZColComboBox *)o)->SetListLoaded( FALSE );
      }
   }
}

/******************************************************************************/
// Function name   : ITrueDBGridCtrl::InitEventList
// Description     : Initializes the event list that can be used for debugging
//                   purpose to get an idea about the eventsequence
// Return type     : void
/******************************************************************************/
void ITrueDBGridCtrl::InitEventList()
{
   if ( m_bTraceEvents )
   {
      m_EventList.AddTail( new CActiveXEvent ( "Click",   DISPID_Click               ));
      m_EventList.AddTail( new CActiveXEvent ( "DblClick",   DISPID_DblClick            ));
      m_EventList.AddTail( new CActiveXEvent ( "AfterColUpdate",   DISPID_AfterColUpdate      ));
      m_EventList.AddTail( new CActiveXEvent ( "AfterDelete",   DISPID_AfterDelete         ));
      m_EventList.AddTail( new CActiveXEvent ( "AfterInsert",   DISPID_AfterInsert         ));
      m_EventList.AddTail( new CActiveXEvent ( "AfterUpdate",   DISPID_AfterUpdate         ));
      m_EventList.AddTail( new CActiveXEvent ( "BeforeColUpdate",   DISPID_BeforeColUpdate   ));
      m_EventList.AddTail( new CActiveXEvent ( "BeforeDelete",   DISPID_BeforeDelete      ));
      m_EventList.AddTail( new CActiveXEvent ( "BeforeInsert",   DISPID_BeforeInsert      ));
      m_EventList.AddTail( new CActiveXEvent ( "BeforeUpdate",   DISPID_BeforeUpdate      ));
      m_EventList.AddTail( new CActiveXEvent ( "ColResize",   DISPID_ColResize         ));
      m_EventList.AddTail( new CActiveXEvent ( "HeadClick",   DISPID_HeadClick         ));
      m_EventList.AddTail( new CActiveXEvent ( "FootClick",   DISPID_FootClick         ));
      m_EventList.AddTail( new CActiveXEvent ( "OwnerDrawCell",   DISPID_OwnerDrawCell      ));
      m_EventList.AddTail( new CActiveXEvent ( "BeforeRowColChange",   DISPID_BeforeRowColChange));
      m_EventList.AddTail( new CActiveXEvent ( "RowColChange",   DISPID_RowColChange      ));
      m_EventList.AddTail( new CActiveXEvent ( "RowResize",   DISPID_RowResize         ));
      m_EventList.AddTail( new CActiveXEvent ( "Scroll",   DISPID_Scroll            ));
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
// Function name   : ITrueDBGridCtrl::WantHandleEvents
// Description     : Notifies the Zeidon Control which events are handles
//                   by this DLL
// Return type     : void
/**************************************************************************/
void ITrueDBGridCtrl::WantHandleEvents(  )
{
   // this DLL handles the events below
   // special key handling for ctrl+tab
   m_pZActiveX->RegisterEvent( DISPID_KeyDown );
   m_pZActiveX->RegisterEvent( DISPID_MouseUp );

   // used to syncronize entity cursor against cntrol
   m_pZActiveX->RegisterEvent( DISPID_RowColChange );
   m_pZActiveX->RegisterEvent( DISPID_BeforeRowColChange );

   m_pZActiveX->RegisterEvent( DISPID_HeadClick );

   // used to validate data against domain
   m_pZActiveX->RegisterEvent( DISPID_BeforeColUpdate );
   // used to copy data into the entity - attribute
   m_pZActiveX->RegisterEvent( DISPID_AfterColUpdate );

   // change selection
   m_pZActiveX->RegisterEvent( DISPID_SelChange );
   // kepp track about layout changes
   m_pZActiveX->RegisterEvent( DISPID_RowResize );
   m_pZActiveX->RegisterEvent( DISPID_ColResize );

   m_pZActiveX->RegisterEvent( DISPID_OnAddNew );
   m_pZActiveX->RegisterEvent( DISPID_AfterInsert );
   m_pZActiveX->RegisterEvent( DISPID_BeforeInsert );

   // combobox events
   m_pZActiveX->RegisterEvent( DISPID_ComboSelect );
   m_pZActiveX->RegisterEvent( DISPID_ButtonClick );

   m_pZActiveX->RegisterEvent( DISPID_BeforeDelete );
   m_pZActiveX->RegisterEvent( DISPID_Error );
   //m_pZActiveX->RegisterEvent( DISPID_Click );

   m_pZActiveX->RegisterEvent( DISPID_AfterUpdate );
   m_pZActiveX->RegisterEvent( DISPID_BeforeUpdate );

   m_pZActiveX->RegisterEvent( DISPID_PostEvent );
   m_pZActiveX->RegisterEvent( DISPID_Scroll );

   // for debug purpose add almost any event
   if ( m_bTraceEvents )
   {
      m_pZActiveX->RegisterEvent( DISPID_AfterDelete );
      m_pZActiveX->RegisterEvent( DISPID_AfterColEdit );
      m_pZActiveX->RegisterEvent( DISPID_BeforeColEdit );
      m_pZActiveX->RegisterEvent( DISPID_Change );
      m_pZActiveX->RegisterEvent( DISPID_ColEdit );
      m_pZActiveX->RegisterEvent( DISPID_ColMove );
      m_pZActiveX->RegisterEvent( DISPID_DataSourceChanged );
      m_pZActiveX->RegisterEvent( DISPID_DblClick );
      m_pZActiveX->RegisterEvent( DISPID_DragCell );
      m_pZActiveX->RegisterEvent( DISPID_FetchCellStyle );
      m_pZActiveX->RegisterEvent( DISPID_FetchCellTips );
      m_pZActiveX->RegisterEvent( DISPID_FirstRowChange );
      m_pZActiveX->RegisterEvent( DISPID_FetchRowStyle );
      m_pZActiveX->RegisterEvent( DISPID_FormatText );
      m_pZActiveX->RegisterEvent( DISPID_FootClick );
      m_pZActiveX->RegisterEvent( DISPID_LayoutReady );
      m_pZActiveX->RegisterEvent( DISPID_LeftColChange );
      m_pZActiveX->RegisterEvent( DISPID_OLECompleteDrag );
      m_pZActiveX->RegisterEvent( DISPID_OLEDragDrop );
      m_pZActiveX->RegisterEvent( DISPID_OLEDragOver );
      m_pZActiveX->RegisterEvent( DISPID_OLEGiveFeedback );
      m_pZActiveX->RegisterEvent( DISPID_OLESetData );
      m_pZActiveX->RegisterEvent( DISPID_OLEStartDrag );
      m_pZActiveX->RegisterEvent( DISPID_OnInit );
      m_pZActiveX->RegisterEvent( DISPID_OwnerDrawCell );
      m_pZActiveX->RegisterEvent( DISPID_SplitChange );
      m_pZActiveX->RegisterEvent( DISPID_ValueItemError );
   }

}


/***********************************************************************************
**
** helper function for Event handling
**
***********************************************************************************/

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::GetEventName
// Description     : Translates an event into a string
// Return type     : CString
// Argument        : long l - the events ID
/**************************************************************************/
CString ITrueDBGridCtrl::GetEventName( long l)
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
// Function name   : ITrueDBGridCtrl::HandleEvent
// Description     : general event handler
// Return type     : zSHORT
// Argument        : DISPID dispEvent
/**************************************************************************/
zSHORT ITrueDBGridCtrl::HandleEvent( DISPID dispEvent )
{
   if (m_bTraceEvents )
   {
      CString strEventName = GetEventName( dispEvent );
      int iSpaces = 48 - strEventName.GetLength();
      CString strSpaces(' ', __max( iSpaces, 1 ) );// maximum number but at least one
      COleVariant vRow = GetBookmark();
      zLONG lRow = GetPositionFromBookmark( vRow );
      short nCol = GetCol();
      CString strHelp;
      strHelp.Format( "Event on %s : %s%s Row : %d, Column : %d", GetControlTag(),
                                                                  strEventName,
                                                                  strSpaces,
                                                                  lRow,
                                                                  nCol );
      TraceLineS("<TrueGrid UsrDll> ", strHelp );
   }

   zSHORT nRC = zACTIVEX_EVENT_COMPLETED;

   zLONG lRowColChange = 0;

   switch ( dispEvent )
   {
   case DISPID_Click:
      //nRC = zACTIVEX_EVENT_COMPLETED;// avoid calling into user code
      break;
   case DISPID_BeforeRowColChange:
      nRC = (zSHORT)OnBeforeRowColChange(); // call user provided code
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
   case DISPID_SelChange:
      OnSelChange( );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_ComboSelect:
      nRC = (zSHORT)OnComboSelect();
      break;
   case DISPID_HeadClick:
      OnHeadClick( );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_AfterColUpdate:
      OnAfterColUpdate( );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_BeforeColUpdate:
      OnBeforeColUpdate( );
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_RowResize:
   case DISPID_ColResize:
      m_bLayoutChanges = TRUE;
      break;
   case DISPID_OnAddNew:
      nRC = (short)OnAddNew();
      break;
   case DISPID_AfterInsert:
      nRC = (short)OnAfterInsert();
      break;
   case DISPID_BeforeInsert:
      OnBeforeInsert();
      nRC = zACTIVEX_EVENT_COMPLETED;
      break;
   case DISPID_BeforeDelete:
      OnBeforeDelete();
      nRC = zACTIVEX_EVENT_COMPLETED;
      break;
   case DISPID_Error:
      OnError();
      nRC = zACTIVEX_EVENT_COMPLETED;
      break;
   case DISPID_ButtonClick:
      nRC = (zSHORT)OnButtonClick();
      nRC = zACTIVEX_EVENT_COMPLETED;
      break;
   case DISPID_ColEdit:
      nRC = (zSHORT)OnColEdit();
      break;
   case DISPID_BeforeUpdate:
      nRC = (zSHORT) OnBeforeUpdate();
      nRC = zACTIVEX_EVENT_COMPLETED;
      break;
   case DISPID_AfterUpdate:
      nRC = (zSHORT) OnAfterUpdate();
      nRC = zACTIVEX_EVENT_COMPLETED;
      break;
   case DISPID_KeyDown:
      nRC = (zSHORT) OnKeyDown();
      nRC = zACTIVEX_EVENT_CONTINUE;
      break;
   case DISPID_MouseUp:
      nRC = ( zSHORT ) OnMouseUp();
      break;
   case DISPID_PostEvent:
      nRC = ( zSHORT ) OnPostEvent();
      break;
   case DISPID_Scroll:
      nRC = ( zSHORT ) OnScroll();
      break;
   default:
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
   break;
   }

   return nRC;
}


/**************************************************************************/
// Function name   : ITrueDBGridCtrl::MapFromOI
// Description     : Zeidon specific routine MapFromOI
//                   It is not realy necessary to do it this way
//                   but it is useful for documentation
// Return type     : zSHORT
/**************************************************************************/
zSHORT ITrueDBGridCtrl::MapFromOI( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN MapFromOI" );
#endif
   zSHORT nRC = 0;

   LPDISPATCH pDispatch = GetArray();

   if ( pDispatch == NULL )
   {
      m_xArrayDB.CreateDispatch( m_xArrayDB.GetCLSID() );
      zLONG lArrayCols = m_xArrayDB.GetCount( zGRIDCOLS );
      if( lArrayCols < 1 )
      {
         Columns cols ( GetColumns() );
         zLONG lColCount = cols.GetCount();
         m_xArrayDB.ReDim( 0, 1, 0, lColCount - 1 );
      }
      SetArray( m_xArrayDB );
   }

   long lRowCount = m_xArrayDB.GetCount( zGRIDROWS );
   if( lRowCount > 0 ) //xarraydb crashes if lRowcount is 0
      m_xArrayDB.DeleteRows( 0, lRowCount );


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
         LockGrid( -1, FALSE );
      }

      if ( InitMapping( ) < 0 )
      {
         m_pZActiveX->m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
      }

      if( IsRowInsertMode() )
      {
         CString strTag( GetControlTag() );
         strTag = "W A R N I N G : Refresh called during InsertMode. InsertMode Canceled! " + strTag;
         TraceLineS("<TrueGrid UsrDll> ", strTag );
         SetRowInsertMode( FALSE );
      }

      TRY
      {
         //whatever comes with MapFromOI the Insert mode is finished
         //SetRowInsertMode( FALSE );
         // Refresh will force the control to fire DISPID_UnboundReadData events
         if( GetDataMode() == dbgUnboundSt )
            if ( m_ZeidonColumns.GetCount() )
               StorageModeMappingEx();
            else //for backward compatibility
               StorageModeMapping();
         //just in case that the driver doesn't tell that this guy is readonly
         zBOOL bUpdateable = FALSE;
         zVIEW vApp = GetViewNamed( m_strViewName );
         if( vApp )
            bUpdateable = (zBOOL)MiGetUpdateForView( vApp );
         LockGrid( -1, !bUpdateable );
      }
      CATCH ( COleException, e )
      {
         e->ReportError();
         nRC = -3;
      }
      END_CATCH
   }

#ifdef _DEBUG_METHOD
   TraceState( "END MapFromOI" );
#endif
   return nRC;
}

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::MapToOI
// Description     : Zeidon specific routine MapToOI
//                   It is not realy necessary to do it this way
//                   but it is useful for documentation
// Return type     : zSHORT
/**************************************************************************/
zSHORT ITrueDBGridCtrl::MapToOI( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN MapToOI" );
#endif

   if ( m_pZActiveX->m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
   {
      m_pZActiveX->m_ulMapActFlags |= zMAPACT_ENABLED;
      m_pZActiveX->m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
   }

   m_pZActiveX->m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;

   if( GetDataChanged() )
   {
      UpdateAllColumnValues();
   }

#ifdef _DEBUG_METHOD
   TraceState( "END MapToOI" );
#endif

   return ( 0 );
}



/**********************************************************************************************/
// Function name   : ITrueDBGridCtrl::StorageModeMapping
// Description      : Transfers data to the grid using storage mode
//                  to use Storage mode another ActiveX ( IXArrayDB )has to be instanciated
//                  filled with data and passed to the grid. IXArrayDB is kind of an Safearray
//                  datamembers of IXArrayDB can be accessed by row & column indexes
// Return type      : zSHORT
/**********************************************************************************************/
zSHORT ITrueDBGridCtrl::StorageModeMapping()
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN StorageModeMapping" );
#endif
   ZMapping *pMapList = m_pZActiveX->SeekMapItem( "List" );    // check for specific list entity
   ZMapping *pMapScope = m_pZActiveX->SeekMapItem( "Scope" );  // check for specific scope entity
   ZMapping *pMapping = 0;

   zVIEW vApp = 0, vTemp = 0;

   zLONG lMapCount = m_pZActiveX->m_MapList.GetSize();
   zLONG lCurrentCol = GetCol();

   if( pMapScope )
      lMapCount--;

   COleVariant vValue;
   CString strValue;

   zSHORT nRC = 0;

   zLONG lColCount = 0;
   Columns cols ( GetColumns() );
   lColCount = cols.GetCount();

   vTemp = GetViewNamed( m_strViewName );
   if ( !vTemp )
      return( -1 );
   else
   {
      CreateViewFromView( &vApp, vTemp );
      if( vApp == 0 )
      {
         CString strHelp = "Unable to load view for TrueGrid - ";
         strHelp += GetControlTag();
         strHelp += " ";
         TraceLineS( strHelp, m_strViewName );
         return( -1 );
      }
   }

   if( lColCount == 0 || lMapCount == 0 )
      return( -1 );

   // cache column properties
   nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                          m_pZActiveX->m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                          m_strScopeEntityName, 0 );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      zSHORT lRealColumn = 0;
      for ( zSHORT lCol = 0; lCol < m_pZActiveX->m_MapList.GetSize(); lCol++ )
      {
         pMapping = m_pZActiveX->m_MapList[ lCol ];
         if ( pMapping && pMapping != pMapList && pMapping != pMapScope )
         {
            ZGridColumn *pGC = GetGridColumn( lRealColumn );
            if  ( pGC )
            {
               pGC->SetMapping( pMapping );
               Column c = cols.GetColumn( lRealColumn );
               c.SetDataWidth( pMapping->m_ulDisplayLth );
               pGC->CheckLocking( vApp );
               // setup list of Values for current mapping
               // to display proper values for ComboBoxes / RadioButtons etc.
               PopulateList( lRealColumn );
            }
            else
            {
               TraceLineS("Number of mappings exceeds number of Grid columns: ITrueDBGridCtrl::StorageModeMapping ", GetControlTag() );
            }
            lRealColumn++;
         }
      }
   }

   zLONG iRelativeNbr = 0;
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if( iRelativeNbr <= m_xArrayDB.GetCount( zGRIDROWS ) )
         m_xArrayDB.AppendRows( 1 );

      zLONG lRealColumn = 0;
      for ( zSHORT lCol = 0; lCol < m_aGridCols.GetSize(); lCol++ )
      {
         ZGridColumn *pGC = GetGridColumn( lCol );
         pMapping = pGC->GetMapping();
         if ( pMapping && pMapping != pMapList && pMapping != pMapScope )
         {
            COleVariant v;
            CString str;
            str = pGC->GetAttributeValue( vApp );
            // check whether Column is a CheckBox
            if ( pGC->HasValidCheckBoxTranslation() )
            {
               Columns cols(GetColumns());
               Column col = cols.GetColumn( (short)lRealColumn );
               ValueItems ListOfValues = col.GetListOfValues();
               if( col.GetNumberFormat() == "Yes/No" ||
                   col.GetNumberFormat() == "True/False" ||
                   col.GetNumberFormat() == "On/Off" ||
                   ListOfValues.GetPresentation() == dbgCheckBox )
               {
                  if( str == pGC->GetCheckBoxValue( FALSE ) )
                     str == "0";
                  else
                     str == "1";
               }
            }

            v = str;
            m_xArrayDB.Set( iRelativeNbr, lRealColumn, v );
            lRealColumn++;
         }
      }
      nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                             m_pZActiveX->m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                             m_strScopeEntityName, 0 );
      iRelativeNbr++;
   } // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru entities

   SetCol( (short) lCurrentCol );

   Refresh( );


   // Setup selection setting
   // we are positioned on the first entity
   iRelativeNbr = 0;
   nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                          m_pZActiveX->m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                          m_strScopeEntityName, 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      COleVariant vB;
      MakeBookmark( iRelativeNbr, vB );
      // setup multiple selection for control
      if ( GetSelectStateOfEntity( vApp, m_strEntityName ) == 1 )
      {
         SelBookmarks Selection( GetSelBookmarks( ) );
         Selection.Add( vB );
      }// setup selection for control

      nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                             m_pZActiveX->m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                             m_strScopeEntityName, 0 );
      iRelativeNbr++;
   } // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru entities

#ifdef _DEBUG_METHOD
   TraceState( "END StorageModeMapping" );
#endif
   return nRC;
}


/**********************************************************************************************/
// Function name   : ITrueDBGridCtrl::StorageModeMappingEx
// Description     : Transfers data to the grid using storage mode
//                   to use Storage mode another ActiveX ( IXArrayDB )has to be instanciated
//                   filled with data and passed to the grid. IXArrayDB is kind of an Safearray
//                   datamembers of IXArrayDB can be accessed by row & column indexes:
//                   The difference between this method and StorageModeMapping is that this method
//                   uses the new serialized informations for columns as CtrlCtrl underneeth a grid
// Return type     : zSHORT
/**********************************************************************************************/
zSHORT ITrueDBGridCtrl::StorageModeMappingEx()
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN StorageModeMappingEx" );
#endif

   zLONG lCurrentCol = GetCol();
   zLONG lCurrentRow = 0;

   COleVariant vValue;
   CString strValue;

   zSHORT nRC = 0;

   zLONG lColCount = 0;
   Columns cols ( GetColumns() );
   lColCount = cols.GetCount();

   TRY
   {
      long lRowCount = m_xArrayDB.GetCount( zGRIDROWS );
      if( lRowCount > 0 ) //xarraydb crashes if lRowcount is 0
         m_xArrayDB.DeleteRows( 0, lRowCount );
      zVIEW vApp = 0, vTemp = 0;
      vTemp = GetViewNamed( m_strViewName );
      if ( !vTemp )
         nRC = -1;
      else
      {
         CreateViewFromView( &vApp, vTemp );
         if( vApp == 0 )
         {
            CString strHelp = "Unable to load view for TrueGrid - ";
            strHelp += GetControlTag();
            strHelp += " ";
            TraceLineS( strHelp, m_strViewName );
            nRC = -1;
         }
      }

      POSITION pos = m_ZeidonColumns.GetHeadPosition();

      if( lColCount == 0 || pos == NULL )
         nRC = -1;

      if ( nRC >= 0 )
      {
         // cache column properties
         for ( zSHORT lCol = 0; pos ; lCol++ )
         {
            ZGridColCtrl * pCol = m_ZeidonColumns.GetNextColumn( pos );

            if ( pCol )
            {
               ZGridColumn *pGC = GetGridColumn( lCol );
               if  ( pGC )
               {
                  pGC->AttachZeidonControlColumn( pCol );
                  ZColumnMapping * pMapping = pCol->GetColumnMapping();
                  Column c = cols.GetColumn( lCol );
                  c.SetDataWidth( pMapping->GetAttributeDisplayLength() );
                  pGC->CheckLocking( vApp );
                  // setup list of Values for current mapping
                  // to display proper values for ComboBoxes / RadioButtons etc.
                  //PopulateList( lCol );
                  pGC->PopulateListOfValues();
               }
               else
               {
                  TraceLineS("Number of mappings exceeds number of Grid columns: ITrueDBGridCtrl::StorageModeMapping ", GetControlTag() );
               }
            }
         }

         zLONG lNumOfCols = m_aGridCols.GetSize();
         zLONG lNumOfMappings = m_ZeidonColumns.GetCount();
         nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                                m_pZActiveX->m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                                m_strScopeEntityName, 0 );

         while ( nRC > zCURSOR_UNCHANGED )
         {
            if( lCurrentRow >= m_xArrayDB.GetCount( zGRIDROWS ) && nRC > zCURSOR_UNCHANGED )
                m_xArrayDB.AppendRows( 1 );

            for ( zSHORT lCol = 0;lCol < lNumOfCols && lCol < lNumOfMappings; lCol++ )
            {
               COleVariant v;
               ZGridColumn *pGC = GetGridColumn( lCol );

               if( pGC )
                  pGC->GetVariantFromAttribute( v, vApp );

               m_xArrayDB.Set( lCurrentRow, lCol, v );
            }
            nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                                   m_pZActiveX->m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                                   m_strScopeEntityName, 0 );
            lCurrentRow++;
         } // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru entities
      }

      SetCol( (short) lCurrentCol );
      SetApproxCount( m_xArrayDB.GetCount( zGRIDROWS ) );

      // the underlying datastructure has changed
      // because rows where deleted and added to the m_xArrayDB
      // do a rebind to tell the grid about those changes
      ReBind();

      // Setup selection setting
      // we are positioned on the first entity
      if ( vApp != 0 && m_xArrayDB.GetCount( zGRIDROWS ) > 0)
      {
         lCurrentRow = 0;
         nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                                m_pZActiveX->m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                                m_strScopeEntityName, 0 );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            // setup multiple selection for control
            if ( GetSelectStateOfEntity( vApp, m_strEntityName ) == 1 )
            {
               COleVariant vB;
               MakeBookmark( lCurrentRow, vB );
               SelBookmarks Selection( GetSelBookmarks( ) );
               Selection.Add( vB );
            }// setup selection for control

            nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                                   m_pZActiveX->m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                                   m_strScopeEntityName, 0 );
            lCurrentRow++;
         } // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru entities
      }
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::StorageModeMappingEx" );
   }
   END_CATCH_ALL


#ifdef _DEBUG_METHOD
   TraceState( "END StorageModeMappingEx" );
#endif
   return nRC;
}



/**************************************************************************/
// Function name   : ITrueDBGridCtrl::UpdateAllColumnValues
// Description     : stores all changed values in the mapped column
// Return type     : void
/**************************************************************************/
void ITrueDBGridCtrl::UpdateAllColumnValues()
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN UpdateAllColumnValues" );
#endif

   short nRC = 0;

   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      return;

   // Set Cursor to the row currently accessed by the grid
   SetViewCursorFromGridCursor( GetBookmark() );

   TRY
   {
      Columns cols( GetColumns() );
      for ( short i = 0; i < cols.GetCount() && i < m_aGridCols.GetSize(); i++ )
      {
         Column col = cols.GetColumn( i );
         if( col.GetDataChanged() )
         {
            ZGridColumn *pGC = GetGridColumn( i );
            nRC = pGC->UpdateAttribute( vApp );
         }
      }
   }
   CATCH_ALL(e)
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::UpdateAllColumnValues" );
   }
   END_CATCH_ALL

#ifdef _DEBUG_METHOD
   TraceState( "END UpdateAllColumnValues" );
#endif
}


/**************************************************************************/
// Function name   : ITrueDBGridCtrl::LoadLayoutChanges
// Description     : Loads layout changes made during runtime.
//                   Thus are restricted to Column width and row hight
// Return type     : void
/**************************************************************************/
void ITrueDBGridCtrl::LoadLayoutChanges()
{
   if( !m_pZActiveX )
      return;
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN LoadLayoutChanges" );
#endif

   int nRC = 0;
   CString strCtrlBuffer = "Ctrl:";
   strCtrlBuffer += *( m_pZActiveX->m_pzsTag );

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

#ifdef _DEBUG_METHOD
   TraceState( "END LoadLayoutChanges" );
#endif
}

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::StoreLayoutChanges
// Description     : stores layout changes made during runtime.
//                   Thus are restricted to Column width and row hight
// Return type     : void
/**************************************************************************/
void ITrueDBGridCtrl::StoreLayoutChanges()
{
   if( !m_pZActiveX )
      return;
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN StoreLayoutChanges" );
#endif

   CString strCtrlBuffer = "Ctrl:";
   CString strPositionBuffer;

   zSHORT nLth = 0;

   if ( !m_bLayoutChanges )
      return;

   Columns cols( GetColumns() );
   int nCols = cols.GetCount();

   strCtrlBuffer += *( m_pZActiveX->m_pzsTag );

   strPositionBuffer.Empty();

   for ( zSHORT k = 0; k < nCols; k++ )
   {
      Column c = cols.GetColumn( k );
      char szBuffer[ 32 ];
      szBuffer[ 0 ] = 0;
      _ltoa_s( (zLONG) c.GetWidth(), szBuffer, zsizeof( szBuffer ), 10 );
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

#ifdef _DEBUG_METHOD
   TraceState( "END StoreLayoutChanges" );
#endif
}


/**************************************************************************/
// Function name   : ITrueDBGridCtrl::TraceExceptionError
// Description     : Generates an Exception error message string and traces
//                   it into the OE-Service Window
// Return type     : void
// Argument        : CException *e    - Exception that occured
// Argument        : CString strExtra - Extra information
/**************************************************************************/
void ITrueDBGridCtrl::TraceExceptionError(CException *e, CString strExtra )
{
   CString strTitle = "<TrueGrid Exception> " + strExtra + " ";

   CString strErrorMessage;
   LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
   e->GetErrorMessage( lpErrorMessage, 1024 );
   strErrorMessage.ReleaseBuffer();
   strErrorMessage = "   " + strErrorMessage;

   TraceLineS( strTitle, GetControlTag() );
   TraceLineS( strErrorMessage, "");
}


/***********************************************************************************
**
** helper function for special exported functions
**
***********************************************************************************/

/*****************************************************************************************************/
// Function name   : OPERATION ITrueDBGridCtrl_SetCheckBoxTranslation
// Description     : Defines Translation strings for a checked and unchecked
//                   Checkbox in a grid
// Return type     : zOPER_EXPORT zSHORT - return -2 if column index is out of range
//                                                -3 if Checked/uncecked values are eqaul
// Argument        : zSHORT nColumn      - ColumnIndex of the column that
//                                         that displays a checkbox
// Argument        : zCPCHAR zcpcTRUE    - String that is assigned to the entityattribute
//                                         if the checkbox is checked
// Argument        : zCPCHAR zcpcFALSE   - String that is assigned to the entityattribute
//                                         if the checkbox is unchecked
/*****************************************************************************************************/
zSHORT ITrueDBGridCtrl::SetCheckBoxTranslation(zSHORT nColumn, zCPCHAR zcpcTRUE, zCPCHAR zcpcFALSE)
{
   zSHORT nRC = 0;

   if( m_aGridCols.GetSize() < nColumn || nColumn < 0 )
   {
      nRC = -2;
      CString strFormat = "Column index %d out of range! Control = %s Operation = %s";
      CString strHelp;
      strHelp.Format( strFormat, nColumn, GetControlTag(), "ITrueDBGridCtrl::SetCheckBoxTranslation");
   }
   else
   {
      ZGridColumn * pGC = GetGridColumn( nColumn );
      if ( !pGC->SetCheckBoxTranslation( zcpcTRUE, zcpcFALSE ) )
      {
         CString strFormat = "TRUE and FALSE values are equal! Column = %d Control = %s Operation = %s";
         CString strHelp;
         strHelp.Format( strFormat, nColumn, GetControlTag(), "ITrueDBGridCtrl::SetCheckBoxTranslation");
         nRC = -3;
      }
   }
   return nRC;
}


/**********************************************************************************/
// Function name   : ITrueDBGridCtrl::LockColumn
// Description      : based on the bLock parameter a column referenced by nIndex
//                  is locked or unlocked
// Return type      : zSHORT
//                  0 if successfully locked/unlocked
//                  < 0 if failed
//                  -3 nIndex is out of range
//                  -2 if mapping attribute is readonly
//
// Argument       : zSHORT nIndex index of the column that should be locked/unlocked
// Argument       : zBOOL bLock TRUE if column should be locked otherwise FALSE
/***********************************************************************************/
zSHORT ITrueDBGridCtrl::LockColumn( zSHORT nIndex, zBOOL bLock )
{
   zSHORT nRC = 0;

   Columns cols ( GetColumns() );
   TRY
   {
      if ( cols.GetCount() <= nIndex || nIndex < 0 )
      {
         nRC = -3;
         return nRC;
      }

      ZGridColumn *pGC = GetGridColumn( nIndex );
      pGC->SetInitiallyLocked( bLock );
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::LockColumn" );
      nRC = -4;
   }
   END_CATCH_ALL

   return nRC;
}


CString ITrueDBGridCtrl::GetCheckBoxValueForGetData( CString strText,
                                                     short bText,
                                                     Column  col,
                                                     zLONG lCol )
{
   CString strHelp = strText;

   ValueItems ListOfValues = col.GetListOfValues();

   if ( ListOfValues.GetPresentation() == dbgCheckBox ) // CheckBox
   {
      ZGridColumn *pGC = GetGridColumn( (zSHORT) lCol );
      if ( pGC )
      {
         if( bText == 0 || strText == "0" )
            strHelp = pGC->GetCheckBoxValue( FALSE );
         else
            strHelp = pGC->GetCheckBoxValue( TRUE );
      }
   }

   return strHelp;
}


/****************************************************************************************/
// Function name   : ITrueDBGridCtrl::GetCellData
// Description     : sets the grids inputfocus to a cell referenced by Row and column index
//                   and returns the value of that cell
// Return type     : zSHORT               - 1 if successful otherwise 0
// Argument        : zPCHAR pchReturnData - pointer to the memory that gets the content
//                                          of the referenced cell
// Argument        : zLONG lRowNbr        - row index of the referenced cell
// Argument        : zLONG lColNbr        - column index of the referenced cell
/****************************************************************************************/
zSHORT ITrueDBGridCtrl::GetCellData( zPCHAR pchReturnData, zLONG lRowNbr, zLONG lColNbr )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN GetCellData" );
#endif

   zSHORT nRC = 0;
   zLONG lRow = lRowNbr;
   zLONG lCol = lColNbr;
   CString strText;

   if( lRowNbr == -1 )
      lRow = GetPositionFromBookmark( GetBookmark() );

   // Use current column?
   if( lColNbr == -1 )
      lCol = GetCol();

   TRY
   {
      Columns cols( GetColumns () );
      Column col = cols.GetColumn( (short)lCol );

      // in case of AddNewMode there is no entry in the underlying
      // xArray, so set the text only in the grid and update it
      // against the view using updateattribute
      if ( lRowNbr < 0 && GetAddNewMode() != dbgNoAddNew )
      {
         strText = col.GetText( );

         // if column a CheckBox, than return the CheckBoxValue (not Textvalue)
         strText = GetCheckBoxValueForGetData( strText, -1, col, lCol );

         nRC = strText.GetLength( );
         zstrcpy( pchReturnData, strText );
         return nRC;
      }

      if( lRow < m_xArrayDB.GetLowerBound( 1 ) || // check row Index out of range
          lRow > m_xArrayDB.GetUpperBound( 1 ) ||
          lCol < m_xArrayDB.GetLowerBound( 2 ) || // check col Index out of range
          lCol > m_xArrayDB.GetUpperBound( 2 ) )
          return( 0 );

      COleVariant v;
      v = m_xArrayDB.GetValue( lRow, lCol );
      strText = v.bstrVal;

      // if column a CheckBox, than return the CheckBoxValue (not Textvalue)
      strText = GetCheckBoxValueForGetData( strText, v.boolVal, col, lCol );

      nRC = strText.GetLength( );
      zstrcpy( pchReturnData, strText );
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::GetCellData" );
      nRC = 0;
   }
   END_CATCH_ALL

#ifdef _DEBUG_METHOD
   TraceState( "END GetCellData" );
#endif
   return nRC;

}



CString ITrueDBGridCtrl::GetCheckBoxValueForSetData( CString strText,
                                                     Column  col,
                                                     zLONG lCol )
{
   CString strHelp = strText;

   ValueItems ListOfValues = col.GetListOfValues();

   if ( ListOfValues.GetPresentation() == dbgCheckBox ) // CheckBox
   {
      ZGridColumn *pGC = GetGridColumn( (zSHORT) lCol );
      if ( pGC )
      {
         // this will be ether 0 or -1 representing VARIANT_TRUE / VARIANT_FALSE
         if ( strText == pGC->GetCheckBoxValue( TRUE ) )
            strHelp = "-1";
         else
            strHelp = "0";
      }
   }

   return strHelp;
}

/****************************************************************************************/
// Function name   : ITrueDBGridCtrl::SetCellData
// Description     : sets the grids inputfocus to a cell referenced by Row and column index
//                   and sets the value of that cell
// Return type     : zSHORT                - 1 if successful otherwise 0
// Argument        : zCPCHAR pchReturnData - pointer to the string to be set
//                                           to the referenced cell
// Argument        : zLONG lRowNbr         - row index of the referenced cell
// Argument        : zLONG lColNbr         - column index of the referenced cell
/****************************************************************************************/
zSHORT ITrueDBGridCtrl::SetCellData( zCPCHAR pchReturnData,
                                     zLONG lRowNbr, zLONG lColNbr )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN SetCellData" );
#endif

   zSHORT nRC = 0;
   zVIEW vApp = 0, vTemp = 0;
   CString strText = pchReturnData;
   CString strHelp;
   zLONG lRow = 0, lCol = 0;

   // Use current row?
   if( lRowNbr == -1 )
   {
      lRow = GetPositionFromBookmark( GetBookmark() );
   }
   else
      lRow = lRowNbr;

   // Use current column?
   if( lColNbr == -1 )
      lCol = GetCol();
   else
      lCol = lColNbr;

   vTemp = GetViewNamed( m_strViewName );
   if ( !vTemp )
      return( 0 );
   else
   {
      CreateViewFromView( &vApp, vTemp );
      if( vApp == 0 )
      {
         strHelp = "Unable to load view for TrueGrid - ";
         strHelp += GetControlTag();
         strHelp += " ";
         TraceLineS( strHelp, m_strViewName );
         return( 0 );
      }
   }

   nRC = 0;
   TRY
   {
      // get the column and set the columntext this will switch the
      // DataChange flag of the column
      Columns cols( GetColumns() );

      // in case of AddNewMode there is no entry in the underlying
      // xArray, so set the text only in the grid and update it
      // against the view using updateattribute
      if ( lRowNbr < 0 && GetAddNewMode() != dbgNoAddNew )
      {
         Column col = cols.GetColumn( (short)lColNbr );
         // if column a CheckBox, than set correctly CheckBoxValue
         strHelp = GetCheckBoxValueForSetData( strText, col, lCol );

         col.SetText( strHelp );
         ZGridColumn *pGC = GetGridColumn( (short)lCol );
         if( !pGC )
            return( 0 );
         nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                                zPOS_LAST, 0, 0, 0,
                                0, m_strScopeEntityName, 0 );
         nRC = pGC->UpdateAttribute( vApp, strText );
         if ( nRC == 0)
            nRC = 1;
         DropView( vApp );
         return nRC;
      }

      if( lRow < m_xArrayDB.GetLowerBound( 1 ) || // check row Index out of range
          lRow > m_xArrayDB.GetUpperBound( 1 ) ||
          lCol < m_xArrayDB.GetLowerBound( 2 ) || // check col Index out of range
          lCol > m_xArrayDB.GetUpperBound( 2 ) )
          return( 0 );

      // if column a CheckBox, than set correctly CheckBoxValue
      Column col = cols.GetColumn( (short)lCol );
      strHelp = GetCheckBoxValueForSetData( strText, col, lCol );

      COleVariant v( strHelp );
      // Do the update against savearray and refresh the grid
      m_xArrayDB.Set( lRow, lCol, v );
      Refresh();

      // perform update of column value using the representative of
      // the grid column
      nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                    zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                    lRow, m_strScopeEntityName, 0 );

      if( nRC > zCURSOR_UNCHANGED )
      {
         ZGridColumn *pGC = GetGridColumn( (short)lCol );
         if( !pGC )
            return( 0 );
         nRC = pGC->UpdateAttribute( vApp, strText );
         if ( nRC == 0)
            nRC = 1;
      }
      else
         nRC = 0;
      DropView( vApp );
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::SetCellData" );
      nRC = 0;
   }
   END_CATCH_ALL

#ifdef _DEBUG_METHOD
   TraceState( "END SetCellData" );
#endif

   return nRC;
}



/**************************************************************************/
// Function name   : ITrueDBGridCtrl::GetActiveCell
// Description     : returns row and column index of the cell that has input focus
// Return type     : zSHORT          - 1 if successfull, otherwise 0
// Argument        : zPLONG plRowNbr - pointer to a long that gets the row index
// Argument        : zPLONG plColNbr - pointer to a long that gets the column index
/**************************************************************************/
zSHORT ITrueDBGridCtrl::GetActiveCell( zPLONG plRowNbr, zPLONG plColNbr )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN GetActiveCell" );
#endif

   zSHORT nRC = 0;

   *plRowNbr = -1;
   *plColNbr = -1;

   TRY
   {
      *plColNbr = GetCol();

      if( GetAddNewMode() != dbgNoAddNew )
      {
         *plRowNbr = -1;
      }
      else
      {
         COleVariant v = GetBookmark();
         zLONG lPosition = GetPositionFromBookmark( v );
         *plRowNbr = lPosition;
      }

      if ( *plColNbr >= 0 && *plRowNbr >= -1 )
         nRC = 1;
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::GetActiveCell" );
      nRC = 0;
   }
   END_CATCH_ALL
#ifdef _DEBUG_METHOD
   TraceState( "END GetActiveCell" );
#endif
   return nRC;
   //SQL_C_NUMERIC
}


/*************************************************************************************/
// Function name   : ITrueDBGridCtrl::SetCurrentCell
// Description     : sets input focus to the cell referenced by row and column index
// Return type     : zSHORT              - 1 if successfull, otherwise 0
// Argument        : zLONG lRowNbr       - row index of the referenced cell
// Argument        : zLONG lColNbr       - column index of the referenced cell
// Argument        : zBOOL bFromExternal - default parameter that identifies
//                                         whether the call to this function is invoked
//                                         from outside this DLL
/*************************************************************************************/
zSHORT ITrueDBGridCtrl::SetCurrentCell( zLONG lRowNbr, zLONG lColNbr, zBOOL bFromExternal )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN SetCurrentCell" );
#endif

   zSHORT nRC = 0;
   zLONG lRow = lRowNbr;
   zLONG lCol = lColNbr;

   // get current position
   COleVariant v = GetBookmark();
   zLONG lPosition = GetPositionFromBookmark( v );

   if( lRowNbr < 0 )
      lRow = lPosition;

   if ( lColNbr < 0 )
      lCol = GetCol();

   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      return nRC;

   TRY
   {
      m_bExternalRowChange = bFromExternal;
      // navigation has to occur so better change all values
      // check to see whether an update has to occur
      if ( GetDataChanged() )
      {
         UpdateAllColumnValues( );
      }

      // if column Navigation has to occur check for valid column index
      Columns cols ( GetColumns() );
      if ( cols.GetCount() <= lCol )
      {
         return nRC;
      }

      zLONG lOffset = lRow - lPosition;
      if ( lOffset != 0 )
      {
         v = GetBookmark( lOffset );
         // if function is called from VML code there is a chance that
         // view and grid cursors get out of sync if it is followed
         // by a "Return Refresh" termination so the view cursor has to be set now
         if( m_bExternalRowChange )
         {
            lPosition = GetPositionFromBookmark( v );
            nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                          zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                          lPosition, m_strScopeEntityName, 0 );
         }
         SetBookmark( v );
      }
      // if column navigation has to occur check the locking
      // this has to be done after the row navigation part, because Zeidon attribute permission
      // depends on the current cursor position ( e.g key columns in new entities )
      ZGridColumn *pGC = GetGridColumn( (short)lCol );
      if( pGC )
         pGC->CheckLocking( vApp ); // will unlock a cell if possible

      SetCol( (short) lCol );
      m_nCurrentCol = (short)lCol;
      nRC = 1;
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::SetCurrentCell" );
      nRC = 0;
   }
   END_CATCH_ALL

#ifdef _DEBUG_METHOD
   TraceState( "END SetCurrentCell" );
#endif

   return nRC;
}


/***********************************************************************************
**
** Event handlers
**
***********************************************************************************/



/**************************************************************************/
// Function name   : ITrueDBGridCtrl::OnSelChange
// Description     : handles the SelChange Event
//                   by resetting the selectstate of all entities and looping
//                   throug all entries in the SelBookmarks object.
// Return type     : zLONG
/**************************************************************************/
zLONG
ITrueDBGridCtrl::OnSelChange( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnSelChange" );
#endif

   zLONG nRC = 0;
   ZMapping *pMapping = 0;

   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      return nRC;

   // Reset select flags for all entities
   SetAllSelectStatesForEntity( vApp,
                                m_strEntityName,
                                FALSE,
                                m_strScopeEntityName );

   // GetSelBookmarks return a dispatch pointer that can be used to instatiate
   // a SelBookmarks object
   SelBookmarks Selection( GetSelBookmarks() );

   long nCount = Selection.GetCount( );

   // Loop through all entries, retrieve the bookmark which is the
   // absolute entity position
   // set the cursor and set select state
   for ( long nIndex = 0; nIndex < nCount; nIndex++ )
   {
      COleVariant v = nIndex;
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

   if ( nCount == 1 )
   {
      if( GetAddNewMode() == dbgNoAddNew )
      {
         // this is to set the Row cursor to the single selected Row
         SetGridCursorFromViewCursor( );
      }
      // set edit active mode to enable row deleting
      SetEditActive( TRUE );
   }

#ifdef _DEBUG_METHOD
   TraceState( "END OnSelChange" );
#endif
   return nRC;
}

zLONG ITrueDBGridCtrl::OnScroll( )
{
   if( m_pWndDateCtrl )
   {
      if( m_pWndDateCtrl->IsWindowVisible() )
         m_pWndDateCtrl->ShowWindow( SW_HIDE );
   }
   return zACTIVEX_EVENT_CONTINUE;
}




// handles HeadClick event.
// it orders the entities


/*******************************************************************************/
// Function name   : ITrueDBGridCtrl::OnHeadClick
// Description     : Handles the HeadClick - Event by ordering the view Entities
// Return type     : zLONG - 0 if successfull otherwis < 0
/*******************************************************************************/
zLONG ITrueDBGridCtrl::OnHeadClick( )
{
   zLONG nRC = 0;
   ZMapping *pMapping = 0;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   zSHORT nEventCol = pEvent->m_pDispParams->rgvarg[0].iVal;

   if ( !m_bAutosortEnabled )
      return nRC;

   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      return nRC;

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

      ZGridColumn *pGC = GetGridColumn( nEventCol );
      if ( pGC )
      {  // for backward compatibility
         pMapping = pGC->GetMapping();
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
            InitMapping( );
            StorageModeMapping();
         }
         else
         {  // new style
            ZColumnMapping *pCM = pGC->GetMappingEx();
            if ( pCM )
            {
               CString strOrderKey = pCM->GetMapAttributeName();
               if ( m_nCurrentOrderMode == 0 )
               {
                  strOrderKey += " A";
               }
               else
               {
                  strOrderKey += " D";
               }

               OrderEntityForView( vApp, pCM->GetMapEntityName(), strOrderKey );
               m_pZActiveX->m_ulMapActFlag2 |= zMAPACT_MAPPING_FROM_OI;
               InitMapping( );
               StorageModeMappingEx();
            }
         }
      }
   }

   return nRC;
}



/*******************************************************************************/
// Function name   : ITrueDBGridCtrl::PopulateList
// Description     : Populates a list of values that are allowed for the column
//                   referenced by column index. The values are only populated if
//                   the presentation of the column is RadioButton or combobox
//                   and the column has a valid mapping
// Return type     : void
// Argument        : zSHORT nColumn - column index of the column whose list of
//                                    values has to be populated
/*******************************************************************************/
void ITrueDBGridCtrl::PopulateList( zSHORT nColumn )
{
   zVIEW vApp = 0;
   zSHORT  nRC = 0;

   vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      return;

   CString strHelp;

   TRY
   {

      Columns cols( GetColumns () );
      if( cols.GetCount() <= nColumn )
         return;

      Column col = cols.GetColumn( nColumn );

      ValueItems ListOfValues = col.GetListOfValues();
      if (    ListOfValues.GetPresentation() != dbgRadioButton // Radio Button
           && ListOfValues.GetPresentation() != dbgComboBox // Combo Box
           && ListOfValues.GetPresentation() != dbgSortedComboBox )// Sorted Combo Box
           return;

      ZGridColumn * pGC = GetGridColumn( nColumn );
      if ( !pGC )
         return;
      ZMapping *pMapping = pGC->GetMapping();

      if ( !pMapping )
         return;

      zLONG lCursor = 0;
      PCHAR pchBuffer = strHelp.GetBufferSetLength( pMapping->m_ulDisplayLth );

      zSHORT sDomainType = tgGetDomainType( vApp,
                                            pMapping->m_csEName,
                                            pMapping->m_csAName );
      if( sDomainType == 'T' )
         ListOfValues.Clear();// we do not like any user defined stuff here
      else
         return;
      nRC = GetFirstTableEntryForAttribute( pchBuffer, pMapping->m_ulDisplayLth, vApp, pMapping->m_csEName, pMapping->m_csAName, pMapping->m_csContext, &lCursor );
      while ( nRC == 0 )
      {
         strHelp.ReleaseBuffer();
         COleVariant vValue = strHelp;

         ValueItemDisp ValueItem;
         ValueItem.CreateDispatch( ValueItem.GetCLSID() );
         ValueItem.SetDisplayValue( vValue ); // set external attribute value
         ValueItem.SetValue( vValue );        // set internal attribute value
                                              // translatiion will be made by zeidon oe so both must be the same
         ListOfValues.Add( ValueItem );

         pchBuffer = strHelp.GetBufferSetLength( pMapping->m_ulDisplayLth );
         nRC = GetNextTableEntryForAttribute( pchBuffer, pMapping->m_ulDisplayLth, vApp, pMapping->m_csEName, pMapping->m_csAName, pMapping->m_csContext, &lCursor );
      }
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::PopulateList" );
   }
   END_CATCH_ALL
}

void ITrueDBGridCtrl::PopulateListOfValues( zVIEW vApp )
{
   if ( vApp == 0 )
      return;

   POSITION pos = m_ZeidonColumns.GetFirstColumnPosition();

   for ( zSHORT lCol = 0; pos ; lCol++ )
   {
      ZGridColCtrl * pCol = m_ZeidonColumns.GetNextColumn( pos );

      if ( pCol )
      {
         ZGridColumn *pGC = GetGridColumn( lCol );
         if ( pGC )
         {
            if ( !pGC->GetMappingEx() )
            {
               pGC->AttachZeidonControlColumn( pCol );
            }
            // setup list of Values for current mapping
            // to display proper values for ComboBoxes / RadioButtons etc.
            pGC->PopulateListOfValues();
         }
         else
         {
            TraceLineS( "Number of mappings exceeds number of Grid columns: ITrueDBGridCtrl::OnBeforeInsert ",
                        GetControlTag() );
         }
      }
   }
}

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::OnRowColChange
// Description     : Handles the RowColChange - Event by synchronizing
//                   the grids input row against the current view cursor setting
//                   and vice versa depending on the calling context
// Return type     : zLONG
/**************************************************************************/
zLONG ITrueDBGridCtrl::OnRowColChange( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnRowColChange" );
#endif
   zLONG lPosition = zDBGRID_AVOID_EVENT_ROUTING;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_RowColChange )
   {
      TraceLineI( "TrueGrid - OnRowColChange called on wrong event ", pEvent->m_dispid );
      return( -1 );
   }

   // extract parameter
   COleVariant vLastRow( pEvent->m_pDispParams->rgvarg[ 1 ].pvarVal );
   VARIANT     *pvLastRow = (pEvent->m_pDispParams->rgvarg[ 1 ].pvarVal);
   zSHORT      nLastCol = (pEvent->m_pDispParams->rgvarg[ 0 ].iVal);

   if ( m_strViewName.IsEmpty( ) )
   {
      TraceLineS( "TrueGrid - ", "Empty View passed" );
      return( -2 );
   }

   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      return( -3 );

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

      // check whether column is updatetable and make it available
      if( m_nCurrentCol >= 0 )
      {
         ZGridColumn *pGC = GetGridColumn( m_nCurrentCol );
         if( pGC )
         {
            if( pGC->CheckLocking( vApp ) )
               SetCol( m_nCurrentCol );
         }
         m_nCurrentCol = -1;
      }
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
         lCurrentRow = GetPositionFromBookmark( vCurrentRow );
         lLastRow = GetPositionFromBookmark( vLastRow );
         if ( lLastRow != lCurrentRow )
         {//
            // hey we got the last row now we have to get the current one
            lPosition = SetViewCursorFromGridCursor( vCurrentRow );
         }

         if( !IsGridLocked() )
         {
            // check whether column is updatetable and make it available
            // we need this because a new entities Key attribute is updateable
            // until it is made persistent
            zSHORT sCurrentCol = 0;
            Columns cols( GetColumns() );
            ZGridColumn *pGC = GetGridColumn( sCurrentCol );
            while ( pGC )
            {
               pGC->CheckLocking( vApp );
               sCurrentCol++;
               pGC = GetGridColumn( sCurrentCol );
            }

            pGC = GetGridColumn( GetCol() );
            if( pGC && pGC->IsDateCtrl() )
            {
               PostMsg( zDBGRID_PostEventCreateDateCtrl );
            }
         }
      }
      CATCH_ALL (  e )
      {
         TraceExceptionError( e, "ITrueDBGridCtrl::OnRowColChange" );
      }
      END_CATCH_ALL
   }

#ifdef _DEBUG_METHOD
   TraceState( "END OnRowColChange" );
#endif
   return lPosition;
}

zLONG ITrueDBGridCtrl::OnBeforeRowColChange( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnBeforeRowColChange" );
#endif
   zLONG nRC = zACTIVEX_EVENT_COMPLETED;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   // extract parameter
   zSHORT *pnCancel = pEvent->m_pDispParams->rgvarg[0].piVal;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_BeforeRowColChange )
   {
      TraceLineI( "TrueGrid - OnBeforeRowColChange called on wrong event ", pEvent->m_dispid );
      return zACTIVEX_EVENT_COMPLETED;
   }

   if( GetDataChanged() )
      UpdateAllColumnValues( );

   nRC = ProcessImmediateEvent( (ZMapAct *) m_pZActiveX,
                                   pEvent->m_dispid,
                                   pEvent );

   if ( nRC != 0 && nRC != zNO_APPLICATION_EVENT)
   {
      *pnCancel = 1;
   }

#ifdef _DEBUG_METHOD
   TraceState( "END OnBeforeRowColChange" );
#endif
   return zACTIVEX_EVENT_COMPLETED;
}
//

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::SetGridCursorFromViewCursor
// Description     : Sets the grids input focus row to the row that is
//                   referenced by cursorsetting in the view
// Return type     : void
/**************************************************************************/
void ITrueDBGridCtrl::SetGridCursorFromViewCursor( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN SetGridCursorFromViewCursor" );
#endif

   zLONG lPosition = 0;

   TRY
   {
      // If we have a View name and a Scoping (list) Entity name ... continue
      if ( m_strViewName.IsEmpty( ) || m_strEntityName.IsEmpty( ) )
      {
         TraceLineS( "TrueGrid- ","Empty View or Entityname passed" );
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
         COleVariant vCurrentRow ( GetBookmark() );
         vCurrentRow = lPosition;
         SetBookmark( vCurrentRow );
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::SetGridCursorFromViewCursor" );
   }
   END_CATCH_ALL

#ifdef _DEBUG_METHOD
   TraceState( "END SetGridCursorFromViewCursor" );
#endif

}

/*********************************************************************************/
// Function name   : ITrueDBGridCtrl::SetViewCursorFromGridCursor
// Description     : Sets the views cursor to the input focus row referenced
//                   by vCurrentRow
// Return type     : zLONG - a negative value if the view or the entity is unknown
//                           or cursorsetting fails. Otherwise the current row
// Argument        : COleVariant vCurrentRow - the row that should get the cursor
/*********************************************************************************/
zLONG ITrueDBGridCtrl::SetViewCursorFromGridCursor( COleVariant vCurrentRow )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN SetViewCursorFromGridCursor" );
#endif
   // If we have a View name and a Scoping (list) Entity name ... continue
   if ( m_strViewName.IsEmpty( ) || m_strEntityName.IsEmpty( ) )
   {
      TraceLineS( "TrueGrid- ","Empty View or Entityname passed" );
      return( -3 );
   }
   // get the view and position cursor.
   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      return( -4 );

   zLONG lPosition = -1;
   zSHORT nRC = 0;

   if( GetAddNewMode() != dbgNoAddNew )
   {
      nRC = SetEntityCursor( vApp, m_strEntityName, 0,
              zPOS_LAST , 0, 0, 0,
              0, m_strScopeEntityName, 0 );
   }
   else
   {
      lPosition = GetPositionFromBookmark ( vCurrentRow );
      nRC = SetEntityCursor( vApp, m_strEntityName, 0,
                 zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                 lPosition, m_strScopeEntityName, 0 );
   }

   if ( nRC <= zCURSOR_UNCHANGED )
   {
      TraceLineI( "TrueGrid- set cursor failed. nRC = ", nRC );
      return -5;
   }
#ifdef _DEBUG_METHOD
   TraceState( "END SetViewCursorFromGridCursor" );
#endif
   return lPosition;
}



/**********************************************************************************/
// Function name   : ITrueDBGridCtrl::OnAfterColUpdate
// Description     : Handles the AfterColUpdate and stores a cells value in
//                   the corresponding attribute
// Return type     : zLONG - returns a value < 0 or any return of UpdateAttribute
/*********************************************************************************/
zLONG ITrueDBGridCtrl::OnAfterColUpdate( )
{
   zLONG nRC = 0;
   ZMapping *pMapping = 0;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   if ( pEvent == NULL || pEvent->m_pDispParams == NULL )
      return zACTIVEX_EVENT_COMPLETED;

   zSHORT nEventCol = pEvent->m_pDispParams->rgvarg[0].iVal;

   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      nRC = -1;

   if ( nRC >= 0 )
   {
      ZGridColumn *pGC = GetGridColumn( nEventCol );
      if ( pGC )
      {
         nRC = pGC->UpdateAttribute( vApp );
         Update();
      }
      else
         nRC = -2;
   }

   return nRC;

}


/********************************************************************************/
// Function name   : ITrueDBGridCtrl::OnBeforeColUpdate
// Description     : Handles the BeforeColUpdate and validates a cells value in
//                   the corresponding attribute
// Return type     : zLONG
/********************************************************************************/
zLONG ITrueDBGridCtrl::OnBeforeColUpdate( )
{
   zLONG nRC = 0;
   ZMapping *pMapping = 0;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   zSHORT *pnCancel = pEvent->m_pDispParams->rgvarg[0].piVal;
   VARIANT *pvOldValue = pEvent->m_pDispParams->rgvarg[1].pvarVal;
   zSHORT nColIndex = pEvent->m_pDispParams->rgvarg[2].iVal;

   zVIEW vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
      nRC = -1;

   SetViewCursorFromGridCursor( GetBookmark() );

   zBOOL bUpdateable = (zBOOL)MiGetUpdateForView( vApp );

   if ( bUpdateable )
      nRC = ProcessImmediateEvent( (ZMapAct *) m_pZActiveX,
                                   pEvent->m_dispid,
                                   pEvent );
   else
      nRC = 1;

   if ( nRC != 0 && nRC != zNO_APPLICATION_EVENT )
   {
      *pnCancel = 1;
   }
   else
   {
      ZGridColumn *pGC = GetGridColumn( nColIndex );
      if ( pGC )
         nRC = pGC->ValidateAttribute( vApp );
      else
         nRC = zCALL_ERROR;

      if ( zCALL_ERROR == nRC )
      {
         *pnCancel = 1;
      }
   }

   return zACTIVEX_EVENT_COMPLETED;
}


/**************************************************************************/
// Function name   : ITrueDBGridCtrl::OnAddNew
// Description     : Event handler for OnAddNew Event. Sets the ITrueDBGrid
//                   control in Insert Mode ( see calls of IsRowInsertMode to
//                   find related code )
// Return type     : zLONG
/**************************************************************************/
zLONG ITrueDBGridCtrl::OnAddNew( )
{
//   SetRowInsertMode( FALSE );
   return zACTIVEX_EVENT_CONTINUE;
}




/**************************************************************************/
// Function name   : ITrueDBGridCtrl::OnAfterInsert
// Description     : Event handler for AfterInsert. Ends the Insert mode.
// Return type     : zLONG
/**************************************************************************/
zLONG ITrueDBGridCtrl::OnAfterInsert()
{
   zLONG nRC = zACTIVEX_EVENT_CONTINUE;
   if( GetDataChanged() )
      UpdateAllColumnValues();
   SetRowInsertMode( FALSE );
   return nRC;
}

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::OnBeforeInsert
// Description     : Event handler for BeforeInsert. Starts the Edit mode
//                   if user action was invoked without error
// Return type     : zLONG
/**************************************************************************/
zLONG ITrueDBGridCtrl::OnBeforeInsert()
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnBeforeInsert" );
#endif

   zLONG lRC = zACTIVEX_EVENT_COMPLETED;
   zVIEW vApp = 0;
   zBOOL bUpdateable = FALSE;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_BeforeInsert )
   {
      TraceLineI( "TrueGrid - OnBeforeInsert called on wrong event ", pEvent->m_dispid );
      return lRC;
   }

   // extract parameter
   zSHORT *pnCancel = pEvent->m_pDispParams->rgvarg[0].piVal;

   if( IsRowInsertMode() )
   {
      UpdateAllColumnValues();
   }

   vApp = GetViewNamed( m_strViewName );
   if ( vApp == 0 )
   {
      *pnCancel = 1;
      return zACTIVEX_EVENT_COMPLETED;
   }

   bUpdateable = (zBOOL)MiGetUpdateForView( vApp );

   if ( bUpdateable )
      lRC = ProcessImmediateEvent( (ZMapAct *) m_pZActiveX,
                                   pEvent->m_dispid,
                                   pEvent );
   else
      lRC = 1;

   if ( lRC != 0 && lRC != zNO_APPLICATION_EVENT )
   {
      *pnCancel = 1;
      SetRowInsertMode( FALSE );
   }
   else
   {
      // Set Cursor to the row currently accessed by the grid
      lRC = SetEntityCursor( vApp, m_strEntityName, 0,
                             m_pZActiveX->m_ulFlag | zPOS_LAST, 0, 0, 0, 0,
                             m_strScopeEntityName, 0 );
/*
      SetRowInsertMode( TRUE );
*/
      /***************** absolutely neccessary ****************************/
      // initialize the columns with an Empty value
      Columns cols( GetColumns() );
      for ( zSHORT lCol = 0; lCol < cols.GetCount(); lCol ++ )
      {
         Column col = cols.GetColumn( lCol );
         CString strHelp = col.GetText();
         if( strHelp.IsEmpty() )
            col.SetText( "" );
      }
      /***************** absolutely neccessary ****************************/
      // set default Radio Button values
      SetDefaultValueItem( vApp );
      if( GetVisibleRows() == 1 )
         Scroll( 0, -1 );

   }

#ifdef _DEBUG_METHOD
   TraceState( "END OnBeforeInsert" );
#endif

   return zACTIVEX_EVENT_COMPLETED;
}


zLONG ITrueDBGridCtrl::OnBeforeDelete()
{
   zLONG lRC = 0;
   zBOOL bUpdateable = FALSE;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_BeforeDelete )
   {
      TraceLineI( "TrueGrid - OnBeforeDelete called on wrong event ", pEvent->m_dispid );
      return zACTIVEX_EVENT_COMPLETED;
   }

   // extract parameter
   zSHORT *pnCancel = pEvent->m_pDispParams->rgvarg->piVal;

   if( GetDataChanged() )
      UpdateAllColumnValues();

   zVIEW vApp = 0;

   GetViewByName( &vApp, m_strViewName,
                         GetDialogView(), zLEVEL_ANY );

   if ( vApp )
      bUpdateable = (zBOOL)MiGetUpdateForView( vApp );

   if ( bUpdateable )
      lRC = ProcessImmediateEvent( (ZMapAct *) m_pZActiveX,
                                   pEvent->m_dispid,
                                   pEvent );
   else
      lRC = 1;

   if ( lRC != 0 && lRC != zNO_APPLICATION_EVENT )
      *pnCancel = 1;

   return zACTIVEX_EVENT_COMPLETED;
}

zLONG ITrueDBGridCtrl::OnAfterUpdate()
{
   zLONG nRC = zACTIVEX_EVENT_COMPLETED;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_AfterUpdate )
   {
      TraceLineI( "TrueGrid - OnAfterUpdate called on wrong event ", pEvent->m_dispid );
   }

   ProcessImmediateEvent( (ZMapAct *) m_pZActiveX,
                                pEvent->m_dispid,
                                pEvent );

   return nRC;
}



zLONG ITrueDBGridCtrl::OnBeforeUpdate()
{
   zLONG lRC = 0;
   zBOOL bUpdateable = FALSE;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_BeforeUpdate )
   {
      TraceLineI( "TrueGrid - OnBeforeUpdate called on wrong event ", pEvent->m_dispid );
      return zACTIVEX_EVENT_COMPLETED;
   }

   // extract parameter
   zSHORT *pnCancel = pEvent->m_pDispParams->rgvarg->piVal;

   if( GetDataChanged() )
      UpdateAllColumnValues();

   zVIEW vApp = 0;

   GetViewByName( &vApp, m_strViewName,
                         GetDialogView(), zLEVEL_ANY );

   if ( vApp )
      bUpdateable = (zBOOL)MiGetUpdateForView( vApp );

   if ( bUpdateable )
      lRC = ProcessImmediateEvent( (ZMapAct *) m_pZActiveX,
                                   pEvent->m_dispid,
                                   pEvent );
   else
      lRC = 1;

   if ( lRC != 0 && lRC != zNO_APPLICATION_EVENT )
      *pnCancel = 1;

   return zACTIVEX_EVENT_COMPLETED;
}

zLONG ITrueDBGridCtrl::OnError()
{

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_Error )
   {
      TraceLineI( "TrueGrid - OnError called on wrong event ", pEvent->m_dispid );
      return zACTIVEX_EVENT_COMPLETED;
   }

   // extract parameter
   zSHORT nErrorCode = pEvent->m_pDispParams->rgvarg[1].iVal;
   zSHORT *pnCancel  = pEvent->m_pDispParams->rgvarg[0].piVal;

   if (m_bTraceEvents )
   {
      CString strErrorText = GetErrorText();
      TraceLineS("Error Event handled by default! ", strErrorText );
   }

   *pnCancel = 0;

   return zACTIVEX_EVENT_COMPLETED;
}

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::GetGridColumn
// Description     : returns a ZGridColumn referenced by lIndex. This is to avoid
//                   direct access to member variable m_aGridCols without
//                   checking the index
// Return type     : ZGridColumn * - NULL if lIndex is out of range
//                                 - otherwise a ZGridColumn
// Argument        : short lIndex  - index of a column to be retrieved
/**************************************************************************/
ZGridColumn * ITrueDBGridCtrl::GetGridColumn( short lIndex )
{
   ZGridColumn * pGC = NULL;

   if( lIndex < m_aGridCols.GetSize() && lIndex >= 0 )
      pGC = m_aGridCols[ lIndex ];

   return pGC;
}

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::GetGridColumn
// Description     : returns a ZGridSplit referenced by lIndex. This is to avoid
//                   direct access to member variable m_aGridCols without
//                   checking the index
// Return type     : ZGridSplit * - NULL if lIndex is out of range
//                                 - otherwise a ZGridSplit
// Argument        : short lIndex  - index of a column to be retrieved
/**************************************************************************/
ZGridSplit * ITrueDBGridCtrl::GetGridSplit( short lIndex )
{
   ZGridSplit * pGS = NULL;

   if( lIndex < m_aGridSplits.GetSize() && lIndex >= 0 )
      pGS = m_aGridSplits[ lIndex ];

   return pGS;
}


/**************************************************************************/
// Function name   : ITrueDBGridCtrl::OnComboSelect
// Description     : Event handle for ComboSelect
// Return type     : zLONG
/**************************************************************************/
zLONG ITrueDBGridCtrl::OnComboSelect( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnComboSelect" );
#endif
   CString strText;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   zSHORT nEventCol = pEvent->m_pDispParams->rgvarg[0].iVal;

   zLONG nRC = zACTIVEX_EVENT_CONTINUE;

   // new style
   if ( m_ZeidonColumns.GetCount() )
   {
      ZGridColumn *pGC = this->GetGridColumn( nEventCol );
      nRC = pGC->SetupComboBoxSelection();
      if( 0 == nRC )
         nRC = zACTIVEX_EVENT_CONTINUE;
   }
   else
   {
      // simple combobox domain mapping
      Columns cols ( GetColumns() );
      Column  col = cols.GetColumn( nEventCol );

      ValueItems ListOfValues = col.GetListOfValues();
      if( ListOfValues.GetAnnotatePicture() )
      {
         COleVariant v = col.GetValue();
         strText = v.bstrVal;
         col.SetText( strText );
      }
      nRC = zACTIVEX_EVENT_CONTINUE;
   }

#ifdef _DEBUG_METHOD
   TraceState( "END OnComboSelect" );
#endif
   return nRC;
}

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::OnButtonClick
// Description     : Event handler for OnButtonClick
// Return type     : zLONG
/**************************************************************************/
zLONG ITrueDBGridCtrl::OnButtonClick( )
{
   ZGridColumn *pGC = NULL;
   zLONG nRC = zACTIVEX_EVENT_COMPLETED;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;
   zSHORT nEventCol = pEvent->m_pDispParams->rgvarg[0].iVal;


   // savety first. are we called in the right context
   switch ( pEvent->m_dispid )
   {
      case DISPID_ButtonClick :
         break;
      case DISPID_PostEvent:
         ProcessImmediateEvent( (ZMapAct *)m_pZActiveX,
                                      DISPID_ButtonClick,
                                      pEvent );
         break;
      default:
         {
            TraceLineI( "TrueGrid - OnButtonClick called on wrong event ", pEvent->m_dispid );
            break;
         }
   }

   return nRC;
}


zLONG
ITrueDBGridCtrl::OnColEdit( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnColEdit" );
#endif
   zLONG nRC = zACTIVEX_EVENT_COMPLETED;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_ColEdit )
   {
      TraceLineI( "TrueGrid - OnColEdit called on wrong event ", pEvent->m_dispid );
      return nRC;
   }
   return nRC;
#ifdef _DEBUG_METHOD
   TraceState( "END OnColEdit" );
#endif

}

zLONG
ITrueDBGridCtrl::OnKeyDown( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnKeyDown" );
#endif
   zLONG nRC = zACTIVEX_EVENT_CONTINUE;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_KeyDown )
   {
      TraceLineI( "TrueGrid - OnKeyDown called on wrong event ", pEvent->m_dispid );
      return nRC;
   }

   // insert TAB and RETURN at Editcursor position for a Edit field only
   zSHORT nColumn = GetCol();
   if ( nColumn >= 0 )
   {
      Columns cols( GetColumns () );
      if( nColumn < cols.GetCount() )
      {
         Column col = cols.GetColumn( nColumn );
         ValueItems ListOfValues = col.GetListOfValues();

         if ( ListOfValues.GetPresentation() == dbgNormal ) // Editfield
         {
            zPSHORT pnKeyCode = pEvent->m_pDispParams->rgvarg[1].piVal;
            zSHORT  nShift    = pEvent->m_pDispParams->rgvarg[0].iVal;

            // CTRL + TAB was pressed, insert TAB at Editcursor position
            if( nShift == 2 && *pnKeyCode == VK_TAB )
            {
               SetSelText( "\t" );
               *pnKeyCode = 0;
            }

            // CTRL + TAB was pressed, insert TAB at Editcursor position
            if( nShift == 2 && *pnKeyCode == VK_RETURN )
            {
               SetSelText( "" );  // set Grid to Edit Mode
               *pnKeyCode = 0;
            }
         }
         else
         if ( ListOfValues.GetPresentation() == dbgCheckBox ) // Editfield
         {
            zPSHORT pnKeyCode = pEvent->m_pDispParams->rgvarg[1].piVal;
            zSHORT  nShift    = pEvent->m_pDispParams->rgvarg[0].iVal;
            // SPACE was pressed, Toggle checkbox value
            if( *pnKeyCode == VK_SPACE )
            {
               COleVariant v = col.GetValue();
               CString strHelp = v.bstrVal;  // this will be ether 0 or -1 representing VARIANT_TRUE / VARIANT_FALSE
               if( strHelp == "0" )
                  strHelp = "-1";
               else
                  strHelp = "0";

               v = strHelp;
               col.SetValue( v );

               *pnKeyCode = 0;
            }
         }
      }
   }

#ifdef _DEBUG_METHOD
   TraceState( "END OnKeyDown" );
#endif

   return nRC;
}


//                                     1 - left
//                                     2 - right
//                                     4 - middle
//
//               zPSHORT psShift  - pointer to a short getting informaation of
//                                  which key was press during MouseDown event.
//                                  Possible values are
//                                     1 - shift
//                                     2 - ctrl
//                                     4 - alt
//                                     5 - alt + shift
//                                     6 - alt + ctrl
//                                     7 - alt + ctrl + shift

zLONG
ITrueDBGridCtrl::OnMouseUp( )
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnMouseUp" );
#endif
   zLONG nRC = zACTIVEX_EVENT_CONTINUE;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_MouseUp )
   {
      TraceLineI( "TrueGrid - OnMouseUp called on wrong event ", pEvent->m_dispid );
      return nRC;
   }

   zSHORT  nButton  = pEvent->m_pDispParams->rgvarg[3].iVal;
   zSHORT  nShift   = pEvent->m_pDispParams->rgvarg[2].iVal;
   zLONG   X        = pEvent->m_pDispParams->rgvarg[1].lVal;
   zLONG   Y        = pEvent->m_pDispParams->rgvarg[0].lVal;

   if( nShift == 0 && nButton == 1 ) // Rigth Mouse butten was pressed, check for col for Button
   {
      zLONG  lRow;
      zSHORT lCol;
      if( CellContaining( (float)X, (float)Y, &lRow, &lCol ) )
      {
         if( lRow >= 0 && lCol >= 0 )
         {
            Columns cols( GetColumns( ) );
            Column col = cols.GetColumn( lCol );
            if( col.GetButtonText() ) // if its a button invoke button pressed
               PostMsg( zDBGRID_PostEventButtonClick );
            else
            {
               ZGridColumn *pGC = GetGridColumn( (zSHORT)lCol );
               if( pGC && pGC->IsDateCtrl() )
               {
                  PostMsg( zDBGRID_PostEventCreateDateCtrl );
               }
            }
         }
      }
   }

#ifdef _DEBUG_METHOD
   TraceState( "END OnMouseUp" );
#endif

   return nRC;
}

zLONG
ITrueDBGridCtrl::OnPostEvent()
{
#ifdef _DEBUG_METHOD
   TraceState( "BEGIN OnPostEvent" );
#endif
   zLONG nRC = zACTIVEX_EVENT_CONTINUE;
   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   // savety first. are we called in the right context
   if ( pEvent->m_dispid != DISPID_PostEvent )
   {
      TraceLineI( "TrueGrid - OnPostEvent called on wrong event ", pEvent->m_dispid );
      return zACTIVEX_EVENT_COMPLETED;
   }

   zLONG lmsg = pEvent->m_pDispParams->rgvarg[0].iVal;
   switch( lmsg )
   {
      case zDBGRID_PostEventButtonClick:
      {
         OnButtonClick();
         nRC = zACTIVEX_EVENT_COMPLETED;
         break;
      }
      case zDBGRID_PostEventCreateDateCtrl:
      {
         CreateDateTimeCtrl();
         nRC = zACTIVEX_EVENT_COMPLETED;
         break;
      }
      default:
         break;
   }

#ifdef _DEBUG_METHOD
   TraceState( "END OnPostEvent" );
#endif

   return nRC;
}

/**************************************************************************/
// Function name   : ITrueDBGridCtrl::LoadZeidonControlProperties
// Description     : Retrives the CtrlBOI, creates an CArchive using the CtrlBOI
//                   and starts serialization to setup the list of Subcontrols
//                   Each  Subcontrol will represent a single column in the grid
// Return type     : zSHORT
/**************************************************************************/
zSHORT
ITrueDBGridCtrl::LoadZeidonControlProperties()
{
   zSHORT nRC = 0;
   zPVOID pCtrlBOI = NULL;

   if ( !m_pZActiveX )
   {
      return ( nRC );
   }

   pCtrlBOI = m_pZActiveX->GetCtrlBOI();
   if ( ! pCtrlBOI )
   {
      return ( nRC );
   }
   else
   {
      TRY
      {
         CMemFile f( (BYTE*) pCtrlBOI, m_pZActiveX->GetCtrlBOISize() );
         CArchive ar( &f, CArchive::load );
         m_ZeidonColumns.Serialize( ar );
         nRC = 1;
      }
      CATCH_ALL( e )
      {
         // if serialization fails, there is a chance that the list
         // contains NULL pointers. Removeing all items will prevent
         // the code from crashing.
         POSITION pos = m_ZeidonColumns.GetFirstColumnPosition();
         while( pos )
         {
            ZGridColCtrl * pGCC = m_ZeidonColumns.GetNextColumn( pos );
            if( pGCC )
               delete pGCC;
         }
         m_ZeidonColumns.RemoveAll();

         TraceExceptionError( e, "ITrueDBGridCtrl::LoadZeidonControlProperties()" );
         nRC = -1;
      }
      END_CATCH_ALL

   }
   return nRC;
}


zSHORT ITrueDBGridCtrl::LockGrid( zLONG nSplit, BOOL bLock )
{
   zSHORT nRC = 0;

   TRY
   {
      LPDISPATCH pDispatch = GetSplits();
      if( !pDispatch )
         return( zCALL_ERROR );

      Splits splits( pDispatch );

      zLONG nSplitCount = splits.GetCount();

      if(  nSplitCount < nSplit )// check against upper bounds
      {
         TraceLineS( "Index out of Range ", "ITrueDBGridCtrl::LockGrid" );
         return( zCALL_ERROR );
      }

      if( nSplit < 0 ) // lock all splits
      {
         for( long lIndex = 0; lIndex < nSplitCount; lIndex++ )
         {
            Split split( splits.GetItem( lIndex ) ) ;
            split.SetLocked( bLock );
         }
      }
      else
      {
         long lIndex = 0;
         Split split( splits.GetItem( lIndex ) ) ;
         split.SetLocked( bLock );
      }
      m_bLock = bLock;
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::LockGrid" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;

}

void ITrueDBGridCtrl::SetDefaultValueItem( zVIEW vApp )
{
   TRY
   {
      Columns cols ( GetColumns() );
      POSITION pos = m_ZeidonColumns.GetHeadPosition();
      for ( zSHORT lCol = 0; pos ; lCol++ )
      {
         ZGridColCtrl * pCol = m_ZeidonColumns.GetNextColumn( pos );

         if ( pCol )
         {
            if( pCol->IsKindOf( RUNTIME_CLASS( ZColRadioGrp ) ) )
                //|| pCol->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
            {
               Column col( cols.GetColumn( lCol ) );

               ValueItems ListOfValues = col.GetListOfValues();

               zLONG nItemCount = ListOfValues.GetCount();
               COleVariant vAttribute;
               ZGridColumn *pGC = GetGridColumn( lCol );
               pGC->GetVariantFromAttribute( vAttribute, vApp );// get the current view.entity.attribute
               zSHORT i = 0;
               zBOOL bFound = FALSE;
               for( i = 0; i < nItemCount && !bFound; i++ )
               {
                  ValueItemDisp vi = ListOfValues.GetValueItem( i );
                  COleVariant v = vi.GetValue();
                  if( v == vAttribute )
                  {
                     bFound = TRUE;
                     CString strText = v.bstrVal;
                     col.SetText( strText );
                     col.Refresh();
                     pGC->SetAttributeFromVariant( vAttribute, vApp );
                  }
               }
               if ( nItemCount > 0 && i < nItemCount)
                  ListOfValues.SetDefaultItem( i - 1 );
               else
                  ListOfValues.SetDefaultItem( 0 );
               // if nothing was found in the the view
               // update the underlying view with defaults
               if( !bFound )
               {
                  zSHORT sDefaultItem = ListOfValues.GetDefaultItem();
                  ValueItemDisp vi = ListOfValues.GetValueItem( sDefaultItem );
                  COleVariant v = vi.GetValue();
                  CString strText = v.bstrVal;
                  col.SetText( strText );
                  col.Refresh();
                  pGC->SetAttributeFromVariant( vAttribute, vApp );
               }

            }
         }
      }
   }
   CATCH_ALL( e );
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::SetInitialValues" );
   }
   END_CATCH_ALL

}

zBOOL ITrueDBGridCtrl::IsRowInsertMode()
{

   zBOOL bReturn = FALSE;
   switch ( GetAddNewMode() )
   {
   case dbgNoAddNew:
      bReturn = FALSE;
      break;
   case dbgAddNewCurrent:
   case dbgAddNewPending:
      bReturn = TRUE;
      break;
   default:
      bReturn = FALSE;
      break;
   }
//   return bReturn;
//   ASSERT( bReturn == m_bInsertMode );
   return m_bInsertMode;
}


zLONG
ITrueDBGridCtrl::SetZCtrlState( zLONG  lStatusType, zULONG ulValue )
{
   zLONG nRC = 0;
   zBOOL bEnabled = FALSE;

   if( m_pZActiveX )
   {
      // call base class implementation to do all the zeidon internal stuff
      nRC = m_pZActiveX->ZMapAct::SetZCtrlState( lStatusType, ulValue );
   }

   if( lStatusType == zCONTROL_STATUS_ENABLED )
   {
      bEnabled = (ulValue == 0) ? FALSE : TRUE;
   }
   else
   if( lStatusType == zCONTROL_STATUS_AUTOGRAY )
   {
      bEnabled = (m_pZActiveX->m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE;
      if ( bEnabled )
         bEnabled = (m_pZActiveX->m_ulMapActFlags & zMAPACT_AUTO_DISABLED) ? FALSE : TRUE;
   }

   if( lStatusType == zCONTROL_STATUS_AUTOGRAY
    || lStatusType == zCONTROL_STATUS_ENABLED )
   {
      m_pZActiveX->EnableWindow( TRUE );
      if( bEnabled )
      {
         // no matter what the view got the last objection
         zBOOL bUpdateable = FALSE;
         zVIEW vApp = GetViewNamed( m_strViewName );
         if( vApp )
            bUpdateable = (zBOOL)MiGetUpdateForView( vApp );
         LockGrid( -1, !bUpdateable );
      }
      else
         LockGrid( -1, !bEnabled);
   }


   return nRC;
}


zVIEW
ITrueDBGridCtrl::GetViewNamed( CString strViewName )
{
   zVIEW vApp = NULL;

   CString strView = strViewName;
   if( strView.IsEmpty() )
      strView = m_strViewName;

   if ( GetViewByName( &vApp, strView,
                       GetDialogView(), zLEVEL_ANY ) <= 0 )
   {
      CString strHelp = "Unable to load view for TrueGrid - ";
      strHelp += GetControlTag();
      strHelp += " ";
      TraceLineS( strHelp, strView );
   }
   return vApp;
}
