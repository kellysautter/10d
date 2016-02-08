/////////////////////////////////////////////////////////////////////////////
// Project ZDR
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrcmdln.cpp
// AUTHOR:       Thomas Müller-von der Bank
//
// OVERVIEW
// ========
// Class implementation for ZCommandlineInfo.
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
// 2001.06.15    DKS    WEB
//    Fix for Replace window with same window.
//
// 2001.06.08    DKS    Debug
//    Added debugging code to solve Action bug for TMV.
//
// 2001.05.23    DKS    Z10
//    Disable MessageBar from Conversational dialogs.
//
// 2001.04.19    DKS    Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
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

// #define DEBUG_ACTION 12
// #define DEBUG_ALL

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZModalDlg dialog

BEGIN_MESSAGE_MAP(ZModalDlg, CDialog)
   //{{AFX_MSG_MAP(ZModalDlg)
   ON_WM_SYSCOMMAND( )
   ON_MESSAGE( zZEIDON_PROCESS_EVENT, OnProcessEvent )
   ON_MESSAGE( zZEIDON_PROCESS_ACTION, OnProcessAction )
   ON_MESSAGE( zZEIDON_PROCESS_HOTKEY, OnProcessHotkey )
   ON_REGISTERED_MESSAGE( g_uMsgPostedClose, OnPostedClose )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

ZCommandLineInfo::ZCommandLineInfo()
{
}

ZCommandLineInfo::~ZCommandLineInfo()
{
}

CCommandLineInfo &ZCommandLineInfo::operator=(ZCommandLineInfo &ZCmdLI)
{
   return(*this);
}

CCommandLineInfo *ZCommandLineInfo::operator=(ZCommandLineInfo *ZCmdLI)
{
   return(this);
}

void
ZCommandLineInfo::ParseParam( const TCHAR *pszParam, BOOL bFlag, BOOL bLast )
{
   CString param_or_switch( pszParam );
   if ( bFlag )
      m_switches.Add( param_or_switch );
   else
      m_params.Add( param_or_switch );
}

void
ZCommandLineInfo::GetParams( CStringArray& params )
{
   int cnt = m_params.GetCount();
   for ( int k = 0; k < cnt; k++ )
      params.Add( m_params.GetAt(k) );
}

void
ZCommandLineInfo::GetSwitches( CStringArray& switches )
{
   int cnt = m_switches.GetCount();
   for ( int k = 0; k < cnt; k++ )
      switches.Add( m_switches.GetAt(k) );
}

ZModalDlg::ZModalDlg( zPCHAR   pchReturnCtrlTag,
                      ZSubtask *pZSubtaskParent,
                      zCPCHAR  cpcDlgTag,
                      zCPCHAR  cpcWndTag,
                      zVIEW    vDialog ) :
           CDialog( ZModalDlg::IDD, pZSubtaskParent->m_pZFWnd )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::ctor", "" );
#endif
   //{{AFX_DATA_INIT(ZModalDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT

   m_pchReturnCtrlTag = pchReturnCtrlTag;
   *m_pchReturnCtrlTag = 0;
   m_pZSubtask = new ZSubtask( pZSubtaskParent->m_pZTask, this );
   m_pZSubtask->m_pZParent = pZSubtaskParent;
   mDeleteInit( m_pZSubtask->m_pzsDlgTag );
   mDeleteInit( m_pZSubtask->m_pzsWndTag );
   m_pZSubtask->m_pzsDlgTag = new CString( cpcDlgTag );
   m_pZSubtask->m_pzsWndTag = new CString( cpcWndTag );
   m_pZSubtask->m_vDialog = vDialog;
}

ZModalDlg::~ZModalDlg( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::dtor", "" );
#endif
// DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// ZModalDlg message handlers

BOOL
ZModalDlg::OnInitDialog( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnInitDialog", "" );
#endif
   ZTask   *pZTask = m_pZSubtask->m_pZTask;
   zVIEW   vMsgQ;
   zPCHAR  pch;
   zULONG  ulSubtaskState;
   zLONG   lRC;
   zLONG   k;

   m_pZSubtask->m_pZTask->m_pZSubtaskConversational = m_pZSubtask;
   CDialog::OnInitDialog( );

   m_pZSubtask->m_pZFWnd = new ZDrFrame( m_pZSubtask );
   if ( mIs_hWnd( m_hWnd ) == 0 )
   {
      TraceLineS( "ZModalDlg::OnInitDialog Invalid Window", "" );
      ASSERT( mIs_hWnd( m_hWnd ) );
      return( TRUE );
   }

   m_pZSubtask->m_pZFWnd->m_hWnd = m_hWnd;
#if 0
   m_pZSubtask->m_pZView = new ZDrView( m_pZSubtask );
   m_pZSubtask->m_pZView->Create( "", "", 0x50800000, CRect( 0, 0, 0, 0 ),
                                  m_pZSubtask->m_pZFWnd, 0x0000e900, 0 );
#endif
   ZSubtask::InitializeSubtaskState( m_pZSubtask, m_hWnd );
   m_pZSubtask->CreateDefaultFont( );
   m_pZSubtask->m_ulSubtaskFlags = zSUBTASK_CREATED_MODAL | zSUBTASK_SUBWND_CONVERSATIONAL;

   m_pZSubtask->EnableParentWindows( FALSE );

   if ( m_pZSubtask->m_vMsgQ == 0 )
   {
      GetViewByName( &vMsgQ, szl__MSGQ, m_pZSubtask->m_vDialog, zLEVEL_TASK );
      CreateViewFromViewForTask( &(m_pZSubtask->m_vMsgQ), vMsgQ, m_pZSubtask->m_vDialog );
      SfLockView( m_pZSubtask->m_vMsgQ ); // we lock this view because it is
                                          // unnamed ... to prevent the OE from
                                          // automatically cleaning it up during
                                          // the deletion of an OE subtask.
   }

// TraceLineI( "Create MsgQ view for Task: ", m_pZSubtask->m_lTID );
   if ( SetEntityCursor( m_pZSubtask->m_vMsgQ, szlTask, szlId,
                         zPOS_FIRST | zQUAL_INTEGER,
                         &(m_pZSubtask->m_lTID), 0, 0, 0, 0, 0 ) < 0 )
   {
      zUSHORT uLth = sizeof( zLONG ) + 2 * sizeof( zSHORT ) + 10 * sizeof( zLONG );

      CreateEntity( m_pZSubtask->m_vMsgQ, szlTask, zPOS_LAST );
      SetAttributeFromVariable( m_pZSubtask->m_vMsgQ, szlTask, szlId, &(m_pZSubtask->m_lTID), zTYPE_INTEGER, 0, 0, 0 );
      SetAttributeFromVariable( m_pZSubtask->m_vMsgQ, szlTask, szlClient, szlZeidon, zTYPE_STRING, 0, 0, 0 );

      pch = new char[ uLth ];
      *((zPSHORT) (pch + sizeof( zLONG ))) = 0;
      *((zPSHORT) (pch + sizeof( zLONG ) + sizeof( zSHORT ))) = 10;
      *((zPLONG) pch ) = 0;
      SetAttributeFromBlob( m_pZSubtask->m_vMsgQ, szlTask,
                            szlStatus, pch, uLth );
   // TraceLineI( "ModalDlg Create MsgQ Status Attribute for Task: ",
   //             m_pZSubtask->m_lTID );
      delete [] pch;
   }

// DisplayObjectInstance( m_pZSubtask->m_vMsgQ, 0, 0 );

   zVIEW   vHotkey = m_pZSubtask->m_pZTask->m_vHK;
   zULONG  ulHotkeyLth;
   zULONG  ulGlobalHotkeyLth;

   GetAttributeLength( &ulGlobalHotkeyLth, vHotkey, szlApp, szlHotkey );
   GetAttributeLength( &ulHotkeyLth, m_pZSubtask->m_vDialog, szlWnd, szlHotkey );

   // Check out RegisterHotKey and UnregisterHotKey???
   if ( ulHotkeyLth % sizeof( zACCEL_TABLE ) )
   {
      TraceLineI( "Hotkey length = ", ulHotkeyLth );
      TraceLineI( "zACCEL_ACTION = ", sizeof( zACCEL_TABLE ) );
      MessageSend( m_pZSubtask->m_vDialog, 0, *(m_pZSubtask->m_pzsDlgTag),
                   "Hotkey length error - please resave .PWD", zMSGQ_MODAL_ERROR, FALSE );
   }

   if ( ulGlobalHotkeyLth % sizeof( zACCEL_TABLE ) )
   {
      TraceLineI( "Hotkey global = ", ulGlobalHotkeyLth );
      TraceLineI( "zACCEL_ACTION = ", sizeof( zACCEL_TABLE ) );
      MessageSend( m_pZSubtask->m_vDialog, 0, *(m_pZSubtask->m_pzsDlgTag),
                   "Global Hotkey length error - please resave .HKY",
                   zMSGQ_MODAL_ERROR, FALSE );
   }

// if ( ulHotkeyLth || ulGlobalHotkeyLth )
   {
      k = (zSHORT) (ulHotkeyLth / sizeof( zACCEL_TABLE ) + ulGlobalHotkeyLth / sizeof( zACCEL_TABLE ));
      m_pZSubtask->m_pAccels = new char[ (k + 1) * sizeof( zACCEL_TABLE ) ];
      GetBlobFromAttribute( m_pZSubtask->m_pAccels, &ulHotkeyLth, m_pZSubtask->m_vDialog, szlWnd, szlHotkey );
      GetBlobFromAttribute( m_pZSubtask->m_pAccels + ulHotkeyLth, &ulGlobalHotkeyLth, vHotkey, szlApp, szlHotkey );
      LPACCEL_TABLE lpAT = (LPACCEL_TABLE) m_pZSubtask->m_pAccels;
      lpAT[ k ].nID = -1;
//    while ( k > 0 )
//    {
//       TraceLineI( "lpAT ====> ", lpAT[ k ].nID );
//       k--;
//    }
   }

   ZSubtask::SetSubtaskState( m_pZSubtask, 0, zSUBTASK_STATE_IN_CONSTRUCTION );

   // Set the address to this C++ object in the Zeidon subtask object ...
   // this is our link back to the C++ object from the Zeidon subtask!
   CreateEntity( m_pZSubtask->m_vDialog, szlSubtask, zPOS_AFTER );
   SetAttributeFromInteger( m_pZSubtask->m_vDialog, szlSubtask, szlCPP, (zLONG) m_pZSubtask );

   // Get the structure of attributes for this window.
   if ( m_pZSubtask->m_pWndDef == 0 )
   {
      m_pZSubtask->m_pWndDef = new tagzKZWDLGXO_Wnd_DEF;

      m_pZSubtask->m_pWndDef->CheckByte = 0xFE;
      m_pZSubtask->m_pWndDef->lStructSize = sizeof( zKZWDLGXO_Wnd_DEF );
      GetStructFromEntityAttrs( (zPCHAR) m_pZSubtask->m_pWndDef, m_pZSubtask->m_vDialog, szlWnd );
      if ( m_pZSubtask->m_pWndDef->CheckByte != 0xFE )
         MessageBox( "WndDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );
   }

   GetAddrForAttribute( (zPCHAR *) &pch, m_pZSubtask->m_vDialog, szlWnd, szlD_Caption );
   if ( *pch )
      strcpy_s( m_pZSubtask->m_pWndDef->Caption, sizeof( m_pZSubtask->m_pWndDef->Caption ), pch );

   m_pZSubtask->m_ulSubtaskFlags |= zSUBTASK_NO_MAIN_MENU;
   m_pZSubtask->m_ulSubtaskFlags |= zSUBTASK_DIALOG_BOX; // for with new 3D???
   m_pZSubtask->m_ulSubtaskFlags |= zSUBTASK_SUBWND_SDI_FRAME;

   if ( m_pZSubtask->m_pZParent )
   {
      m_pZSubtask->m_vInterpSubtask = m_pZSubtask->m_pZParent->m_vInterpSubtask;
   }
   else
   {
      m_pZSubtask->m_vInterpSubtask = pZTask->m_vInterpSubtask;
   }

   m_pZSubtask->m_pchEnabledAction = new char[ (zSHORT) (m_pZSubtask->m_pWndDef->ActCnt + sizeof( zSHORT )) ];

   // Initialize all actions to be enabled as the default
   // how can we so non-chalantly unset ActCnt? ... because it appears
   // that ActCnt is never referenced again.
   zmemset( m_pZSubtask->m_pchEnabledAction, zAUTO_ENABLE,
            (zSHORT) (m_pZSubtask->m_pWndDef->ActCnt + sizeof( zSHORT )) );
   zSHORT *pn = (zSHORT *) (m_pZSubtask->m_pchEnabledAction);
   *pn = (zSHORT) m_pZSubtask->m_pWndDef->ActCnt;

#ifdef DEBUG_ACTION
   TraceLineI( "zdrcmdln init lth: ", m_pZSubtask->m_pWndDef->ActCnt + sizeof( zSHORT ) );
   TraceBlob( "zdrcmdln init:",
              m_pZSubtask->m_pchEnabledAction + sizeof( zSHORT ),
              *((zSHORT *) m_pZSubtask->m_pchEnabledAction) );
#endif

   if ( m_pZSubtask->m_vInterpSubtask == 0 )   // not done if interpreting
   {
      // Get the DLL for this dialog and load the library.
      zPCHAR pch;

      GetAddrForAttribute( &pch, m_pZSubtask->m_vDialog, szlDlg, szlDLL );
      if ( *pch == 0 )
         m_pZSubtask->m_hLibrary = 0;
      else
      {
      // m_hInstDll = LoadLibraryEx( szFileName, 0, LOAD_LIBRARY_AS_DATAFILE );

         m_pZSubtask->m_pzsDLL_Name = new CString( pch );
         m_pZSubtask->m_hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog, pch, 0 );
         m_pZSubtask->m_hInstDLL = (HINSTANCE) SysGetInstance( m_pZSubtask->m_hLibrary );
         m_pZSubtask->m_hLibRes = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog, pch, zLOADLIB_RESOURCES );
         m_pZSubtask->m_hInstRes = (HINSTANCE) SysGetInstance( m_pZSubtask->m_hLibRes );
      }
   }
   else
      m_pZSubtask->m_hLibrary = 0;

   zSHORT nClientWidth;
   zSHORT nClientHeight;

   if ( m_pZSubtask->m_bUseMapDlgUnits )
   {
      m_pZSubtask->m_pWndDef->PSDLG_X = mConvertMapDlgToPixelX( m_pZSubtask->m_pWndDef->PSDLG_X );
      m_pZSubtask->m_pWndDef->PSDLG_Y = mConvertMapDlgToPixelY( m_pZSubtask->m_pWndDef->PSDLG_Y );
      nClientWidth = mConvertMapDlgToPixelX( zHIUSHORT( m_pZSubtask->m_pWndDef->SZDLG_X ) );
      nClientHeight = mConvertMapDlgToPixelX( zHIUSHORT( m_pZSubtask->m_pWndDef->SZDLG_Y ) );
      m_pZSubtask->m_pWndDef->SZDLG_X = mConvertMapDlgToPixelX( zLOUSHORT( m_pZSubtask->m_pWndDef->SZDLG_X ) );
      m_pZSubtask->m_pWndDef->SZDLG_Y = mConvertMapDlgToPixelY( zLOUSHORT( m_pZSubtask->m_pWndDef->SZDLG_Y ) );

      m_pZSubtask->Attr.X = m_pZSubtask->m_pWndDef->PSDLG_X;
      m_pZSubtask->Attr.Y = m_pZSubtask->m_pWndDef->PSDLG_Y;
      m_pZSubtask->Attr.W = m_pZSubtask->m_pWndDef->SZDLG_X;
      m_pZSubtask->Attr.H = m_pZSubtask->m_pWndDef->SZDLG_Y;

      m_pZSubtask->m_lRelLeft = mConvertMapDlgToPixelX( zLOUSHORT( m_pZSubtask->m_pWndDef->REL_X ) );
      m_pZSubtask->m_lOrigRelSizeX = mConvertMapDlgToPixelX( zHIUSHORT( m_pZSubtask->m_pWndDef->REL_X ) );
      m_pZSubtask->m_lRelRight = nClientWidth - m_pZSubtask->m_lOrigRelSizeX;
      m_pZSubtask->m_lOrigRelSizeX -= m_pZSubtask->m_lRelLeft;

      m_pZSubtask->m_lRelTop = mConvertMapDlgToPixelY( zLOUSHORT( m_pZSubtask->m_pWndDef->REL_Y ) );
      m_pZSubtask->m_lOrigRelSizeY = mConvertMapDlgToPixelY( zHIUSHORT( m_pZSubtask->m_pWndDef->REL_Y ) );
      m_pZSubtask->m_lRelBottom = nClientHeight - m_pZSubtask->m_lOrigRelSizeY;
      m_pZSubtask->m_lOrigRelSizeY -= m_pZSubtask->m_lRelTop;
   }
   else
   {
      m_pZSubtask->m_pWndDef->PSDLG_X = mConvertDlgUnitToPixelX( m_pZSubtask->m_pWndDef->PSDLG_X );
      m_pZSubtask->m_pWndDef->PSDLG_Y = mConvertDlgUnitToPixelY( m_pZSubtask->m_pWndDef->PSDLG_Y );
      nClientWidth = mConvertDlgUnitToPixelX( zHIUSHORT( m_pZSubtask->m_pWndDef->SZDLG_X ) );
      nClientHeight = mConvertDlgUnitToPixelX( zHIUSHORT( m_pZSubtask->m_pWndDef->SZDLG_Y ) );
      m_pZSubtask->m_pWndDef->SZDLG_X = mConvertDlgUnitToPixelX( zLOUSHORT( m_pZSubtask->m_pWndDef->SZDLG_X ) );
      m_pZSubtask->m_pWndDef->SZDLG_Y = mConvertDlgUnitToPixelY( zLOUSHORT( m_pZSubtask->m_pWndDef->SZDLG_Y ) );

      m_pZSubtask->Attr.X = m_pZSubtask->m_pWndDef->PSDLG_X;
      m_pZSubtask->Attr.Y = m_pZSubtask->m_pWndDef->PSDLG_Y;
      m_pZSubtask->Attr.W = m_pZSubtask->m_pWndDef->SZDLG_X;
      m_pZSubtask->Attr.H = m_pZSubtask->m_pWndDef->SZDLG_Y;

      m_pZSubtask->m_lRelLeft = mConvertDlgUnitToPixelX( zLOUSHORT( m_pZSubtask->m_pWndDef->REL_X ) );
      m_pZSubtask->m_lOrigRelSizeX = mConvertDlgUnitToPixelX( zHIUSHORT( m_pZSubtask->m_pWndDef->REL_X ) );
      m_pZSubtask->m_lRelRight = nClientWidth - m_pZSubtask->m_lOrigRelSizeX;
      m_pZSubtask->m_lOrigRelSizeX -= m_pZSubtask->m_lRelLeft;

      m_pZSubtask->m_lRelTop = mConvertDlgUnitToPixelY( zLOUSHORT( m_pZSubtask->m_pWndDef->REL_Y ) );
      m_pZSubtask->m_lOrigRelSizeY = mConvertDlgUnitToPixelY( zHIUSHORT( m_pZSubtask->m_pWndDef->REL_Y ) );
      m_pZSubtask->m_lRelBottom = nClientHeight - m_pZSubtask->m_lOrigRelSizeY;
      m_pZSubtask->m_lOrigRelSizeY -= m_pZSubtask->m_lRelTop;
   }

   // Ensure that the original relative sizes are non-zero for later divide.
   if ( m_pZSubtask->m_lOrigRelSizeX == 0 )
      m_pZSubtask->m_lOrigRelSizeX = 1;

   if ( m_pZSubtask->m_lOrigRelSizeY == 0 )
      m_pZSubtask->m_lOrigRelSizeY = 1;

   m_pZSubtask->m_clrText = m_pZSubtask->m_clrBk = 0;
   m_pZSubtask->m_clrErrText = zRGB( 255, 0, 0 ); // default to red

#if 0
   COLORREF cref;
   zLONG  rgb;

   k = 0;
   lRC = SetCursorFirstEntity( m_pZSubtask->m_vDialog, szlWndColor, 0 );
   while ( lRC > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &rgb, m_pZSubtask->m_vDialog, szlWndColor, szlRGB );
      cref = fnTranslateColor( rgb );
      switch ( k )
      {
         case 0:
            m_pZSubtask->m_clrText = cref;       // window text color (0 ==> windows dflt)
            break;

         case 1:
            m_pZSubtask->m_clrBk = cref;
            break;

         case 2:
            m_pZSubtask->m_clrErrText = cref;
            break;

//???    case 3:
//          m_pZSubtask->m_clrErrText = cref;
//          break;
      }

      k++;
      lRC = SetCursorNextEntity( m_pZSubtask->m_vDialog, szlWndColor, 0 );
   }
#endif

   zVIEW vProfile;

   if ( m_pZSubtask->m_pWndDef->ProfileView[ 0 ] &&
        GetViewByName( &vProfile, m_pZSubtask->m_pWndDef->ProfileView,
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   {
      m_pZSubtask->m_pzsProfileViewName =
                    new CString( m_pZSubtask->m_pWndDef->ProfileView );
      RestoreWindowPlacement( m_pZSubtask->m_vDialog, vProfile );
   }

   m_pZSubtask->m_ulWndOper = *((zPLONG) m_pZSubtask->m_pWndDef->OperCtrl);
   ZSubtask::SetSubtaskState( m_pZSubtask, 0, zSUBTASK_STATE_PREBUILD_PRECODE );

   // Based on flags in the Dialog, do the right thing here (MDI/SDI, 3D)???
#pragma mMSG( Override OnUpdateFrameTitle for setting caption )
   if ( m_pZSubtask->m_pWndDef->Caption[ 0 ] )
      m_pZSubtask->SetCaptionTitle( m_pZSubtask->m_pWndDef->Caption, 0, FALSE );

   if ( SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog, szlWndEvent,
                                       szlType, zWND_EVENT_PRE_BUILD, 0 ) > zCURSOR_UNCHANGED )
   {
      lRC = ProcessActionLoop( m_pZSubtask, szlWndAct, GetMessagePos( ), 0, 0, TRUE, "PreBuild" );
      if ( (pZTask->m_uAppState & zAPP_STATE_TERMINATED) ||
           ((ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask )) & zSUBTASK_STATE_MARK_FOR_DELETE) )
      {
         return( FALSE );
      }

      if ( zHIUSHORT( lRC ) == 2 )
      {
         mDeleteInit( m_pZSubtask->m_pzsResumeAction );
         return( FALSE );     // leave state at zSUBTASK_PREBUILD_PRECODE
      }
   }

   ZSubtask::SetSubtaskState( m_pZSubtask, 0, zSUBTASK_STATE_PREBUILD_POSTCODE );
//PREBUILD_POSTCODE_label:

   m_pZSubtask->Attr.StyleEx = m_pZSubtask->m_pWndDef->StyleEx;
   m_pZSubtask->Attr.Style = m_pZSubtask->m_pWndDef->Style;

   m_pZSubtask->SetCaptionTitle( 0, 0 );

   CreateZeidonCtrls( m_pZSubtask->m_vDialog, m_pZSubtask, this, 0, 0, 0, FALSE );

   if ( m_pZSubtask->m_pZMIXCtrl )
   {
      m_pZSubtask->m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnSetFirstLastFocus, 0 );
      if ( m_pZSubtask->m_pzmaWithFocus &&
           m_pZSubtask->m_pzmaWithFocus->m_pCtrl &&
           mIs_hWnd( m_pZSubtask->m_pzmaWithFocus->m_pCtrl->m_hWnd ) )
      {
         m_pZSubtask->m_pzmaWithFocus->m_pCtrl->SetFocus( );
      }
   }

   ZSubtask::SetSubtaskState( m_pZSubtask, 0, zSUBTASK_STATE_POSTBUILD_PRECODE );
//POSTBUILD_PRECODE_label:

   if ( m_pZSubtask->m_pZFWnd && mIs_hWnd( m_pZSubtask->m_pZFWnd->m_hWnd ) )
   {
   // WINDOWPLACEMENT *pwp;
   // pwp = (WINDOWPLACEMENT *) m_pchWindowPlacement;
   // if ( m_pchWindowPlacement &&
   //      pwp->length == sizeof( WINDOWPLACEMENT ) )
   // {
   //    m_pZFWnd->SetWindowPlacement( pwp );
   //    m_pZFWnd->SetActiveWindow( );
   // }

      WINDOWPLACEMENT wp;
      APPBARDATA abd;
      abd.cbSize = sizeof( abd );

      // Ignored members: hwnd, uCallbackMessage, uEdge, rc, lParam.
      // abd.rc contains the rectangular location of the taskbar in
      // screen coordinates.
      SHAppBarMessage( ABM_GETTASKBARPOS, &abd );
      UINT uEdge = GetTaskbarEdge( abd.rc );

      if ( ReadWindowPlacement( m_pZSubtask->m_vDialog, "ZDR", *(m_pZSubtask->m_pzsDlgTag),
                                *(m_pZSubtask->m_pzsWndTag), &wp ) && CheckWindowPos( &wp ) )
      {
         if ( m_pZSubtask->m_pWndDef->Style == (WS_POPUP | WS_VISIBLE | WS_DLGFRAME) )
         {
            wp.rcNormalPosition.left = m_pZSubtask->Attr.X;
            wp.rcNormalPosition.top = m_pZSubtask->Attr.Y;
         }

         if ( uEdge == ABE_TOP )
            wp.rcNormalPosition.top += (abd.rc.bottom - abd.rc.top);
         else
         if ( uEdge == ABE_LEFT )
            wp.rcNormalPosition.left += (abd.rc.right - abd.rc.left);

         if ( (m_pZSubtask->m_pWndDef->Style & WS_THICKFRAME) == 0 )
         {
            wp.rcNormalPosition.right = wp.rcNormalPosition.left + m_pZSubtask->Attr.W;
            wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + m_pZSubtask->Attr.H;
         }
         else
         if ( uEdge == ABE_TOP )
            wp.rcNormalPosition.bottom += (abd.rc.bottom - abd.rc.top);
         else
         if ( uEdge == ABE_LEFT )
            wp.rcNormalPosition.right += (abd.rc.right - abd.rc.left);

         m_pZSubtask->m_pZFWnd->SetWindowPlacement( &wp );
//       TraceLineS( "Setting ACTIVE window ==============> ", *m_pzsWndTag );
         m_pZSubtask->m_pZFWnd->SetActiveWindow( );
      }
      else
      {
//       if ( wp )
//          TraceLineI( "Invalid window placement ", wp->length );

         if ( m_pZSubtask->m_nInvisible < 0 )
            m_pZSubtask->m_pZFWnd->ShowWindow( SW_HIDE );
         else
         {
            int iCmdShow;

            if ( pZTask->m_pZSubtaskFocus == m_pZSubtask )
            {
               iCmdShow = SW_SHOW;
               pZTask->m_pZSubtaskFocus = 0;
            }
            else
               iCmdShow = SW_SHOWNA;

            if ( uEdge == ABE_TOP )
               m_pZSubtask->m_pZFWnd->MoveWindow( m_pZSubtask->Attr.X,
                                                  m_pZSubtask->Attr.Y + abd.rc.bottom,
                                                  m_pZSubtask->Attr.W,
                                                  m_pZSubtask->Attr.H, TRUE );
            else
            if ( uEdge == ABE_LEFT )
               m_pZSubtask->m_pZFWnd->MoveWindow( m_pZSubtask->Attr.X + abd.rc.right,
                                                  m_pZSubtask->Attr.Y,
                                                  m_pZSubtask->Attr.W,
                                                  m_pZSubtask->Attr.H, TRUE );
            else
               m_pZSubtask->m_pZFWnd->MoveWindow( m_pZSubtask->Attr.X,
                                                  m_pZSubtask->Attr.Y,
                                                  m_pZSubtask->Attr.W,
                                                  m_pZSubtask->Attr.H, TRUE );

            m_pZSubtask->m_pZFWnd->ShowWindow( iCmdShow );
         }
      }

      if ( m_pZSubtask->Attr.W != (zSHORT) m_pZSubtask->m_pWndDef->SZDLG_X ||
           m_pZSubtask->Attr.H != (zSHORT) m_pZSubtask->m_pWndDef->SZDLG_Y )
      {
         // We are not building at the original design size, so
         // inform all controls that a WM_SIZE has occurred.
         if ( m_pZSubtask->m_pZMIXCtrl )
         {
            CRect rect;
            m_pZSubtask->m_pZFWnd->GetClientRect( rect );
            CSize size( rect.right - rect.left, rect.bottom - rect.top );
            m_pZSubtask->SendParentResize( 0, &size );
         }
      }
   }

   // Run through the list of Child controls and disable as specified
   // in the common attributes in the PWD.
   if ( m_pZSubtask->m_pZMIXCtrl )
   {
      m_pZSubtask->m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnDisableCtrls, 0 );
   }

   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_POST_BUILD) &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog, szlWndEvent,
                                       szlType, zWND_EVENT_POST_BUILD, 0 ) > zCURSOR_UNCHANGED )
   {
      lRC = ProcessActionLoop( m_pZSubtask, szlWndAct, GetMessagePos( ), 0, 0, TRUE, "PostBuild" );
      if ( (pZTask->m_uAppState & zAPP_STATE_TERMINATED) ||
           ((ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask )) & zSUBTASK_STATE_MARK_FOR_DELETE) )
      {
         return( FALSE );
      }

      if ( zHIUSHORT( lRC ) == 2 )
      {
         mDeleteInit( m_pZSubtask->m_pzsResumeAction );
         return( FALSE );     // leave state at zSUBTASK_POSTBUILD_PRECODE
      }
   }

   ZSubtask::SetSubtaskState( m_pZSubtask, 0, zSUBTASK_STATE_POSTBUILD_POSTCODE );
//POSTBUILD_POSTCODE_label:

   m_pZSubtask->MapFromOI( zMAP_FIRST_TIME );  // do mapping for all controls

   ZSubtask::SetSubtaskState( m_pZSubtask,
                              zSUBTASK_STATE_REPLACED_BY_SELF | 0x000000FF,
                              zSUBTASK_STATE_WELL_FORMED );
   if ( m_pZSubtask->m_pZFWnd && mIs_hWnd( m_pZSubtask->m_pZFWnd->m_hWnd ) )
   {
   // if ( m_pZSubtask->Attr.Style & WS_THICKFRAME )             doesn't work
   //    m_pZSubtask->m_pZFWnd->ModifyStyle( 0, WS_THICKFRAME ); since WM_SIZE
   //                                                            isn't handled

   // TraceLineS( "(stask) UpdateWindow for: ", *m_pzsWndTag );
      m_pZSubtask->m_pZFWnd->UpdateWindow( );

      // We have disabled MessageBars in Conversational dialogs.
   // if ( m_pZSubtask->m_pDIL )
   // {
   //    m_pZSubtask->m_pZFWnd->ShowControlBar( m_pZSubtask->m_pDIL, TRUE, FALSE );
   // }
   }

// GotoDlgCtrl( CWnd *pWndCtrl );

   return( FALSE );  // return TRUE unless you set the focus to a control
                     // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL
ZModalDlg::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN )
   {
      if ( pMsg->wParam == VK_ESCAPE )
      {
      // TraceLineS( "ZModalDlg::PreTranslateMessage: ", "VK_ESCAPE" );
         ::PostMessage( m_hWnd, g_uMsgPostedClose, 0, (LPARAM) m_pZSubtask );
         return( TRUE );
      }
   }

   return( CDialog::PreTranslateMessage( pMsg ) );
}

void
ZModalDlg::OnOK( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnOK", "" );
#endif
   if ( m_pZSubtask->m_pDfltBtn &&
        (m_pZSubtask->m_pDfltBtn->m_ulMapActFlags & zMAPACT_ENABLED) )
   {
      zSHORT nRC;
   // TraceLineS( "ZModalDlg::OnOK Event: ", *(m_pZSubtask->m_pDfltBtn->m_pzsTag) );
      strcpy_s( m_pchReturnCtrlTag, zTAG_LTH, *(m_pZSubtask->m_pDfltBtn->m_pzsTag) );
      nRC = (zSHORT) ProcessImmediateEvent( m_pZSubtask->m_pDfltBtn, 1 );
   // TraceLineI( "ZModalDlg::OnOK RC: ", nRC );
      if ( nRC < 0 )
      {
         m_pchReturnCtrlTag[ 0 ] = 0;
         return;
      }
   }

   ::PostMessage( m_hWnd, g_uMsgPostedClose, 1, (LPARAM) m_pZSubtask );
// mDeleteInit( m_pZSubtask );
// CDialog::OnOK( );
}

void
ZModalDlg::OnCancel( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnCancel", "" );
#endif
   ::PostMessage( m_hWnd, g_uMsgPostedClose, 0, (LPARAM) m_pZSubtask );
// mDeleteInit( m_pZSubtask );
// CDialog:: OnCancel( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#if 0
void
ZModalDlg::OnDblclkListSubtasks( NMHDR *pNMHDR, LRESULT *pResult )
{

   // TODO: Add your control notification handler code here
   *pResult = 0;
   OnOK( );
}
#endif

void
ZModalDlg::OnSysCommand( UINT uCmdType, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnSysCommand", "" );
   TraceLineX( "ZModalDlg::OnSysCommand CmdType: ", uCmdType );
   TraceLineX( "ZModalDlg::OnSysCommand lParam: ", lParam );
#endif

   zLONG lEvent;

   // SC_CLOSE is 0xF060, but when a popup menu is up, the uCmdType we get
   // is 0xF063 for some reason.  Since all SC_ messages end in 0, we
   // will try the following to clear the last 4 bits.
   switch ( uCmdType & 0xFFF0 )
   {
      case SC_CLOSE:
         lEvent = zWND_EVENT_CLOSE;
         break;

       default:
         lEvent = 0;
   }

   if ( lEvent )
   {
      if ( m_pZSubtask->HandleEvent( lEvent ) == zACTIVEX_EVENT_COMPLETED )
         return;     // don't let default processing occur
   }

   Default( ); // CWnd::OnSysCommand just calls Default
}

BOOL
ZModalDlg::OnCommand( WPARAM wParam, LPARAM lParam )
{
   // TODO: Add your specialized code here and/or call the base class
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnCommand", "" );
   TraceLineX( "ZModalDlg::OnCommand wParam: ", (zLONG) wParam );
   TraceLineX( "ZModalDlg::OnCommand lParam: ", (zLONG) lParam );
#endif
// zLONG lHiWord = HIWORD( wParam );

   return( CDialog::OnCommand( wParam, lParam ) );
}

LRESULT
ZModalDlg::OnProcessEvent( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnProcessEvent", "" );
#endif
   return( ProcessActionLoop( m_pZSubtask, (zCPCHAR) lParam, wParam,
                              0, 0, FALSE, "CtrlEvent" ) );
}

LRESULT
ZModalDlg::OnProcessAction( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnProcessAction", "" );
#endif
   if ( SetEntityCursor( m_pZSubtask->m_vDialog, szlAct, 0,
                         zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                         (zSHORT) lParam, 0, 0 ) > zCURSOR_UNCHANGED )
   {
      zPCHAR pch;

      GetAddrForAttribute( (zPCHAR *) &pch, m_pZSubtask->m_vDialog, szlAct, szlTag );
      ProcessActionLoop( m_pZSubtask, pch, wParam, 0, 0, FALSE, szlHotkey );
   }

   return( 0 );
}

LRESULT
ZModalDlg::OnProcessHotkey( WPARAM wParam, LPARAM lParam )
{
   zFARPROC_DRIVER lpfnDynRoutine;
   LPLIBRARY hHotkeyLibrary = 0;
   zCHAR  szDLL_Name[ 9 ];
   zCHAR  szOperName[ zTAG_LTH ];
   zVIEW  vHotkey;

#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnProcessHotkey", "" );
   TraceLineI( "Zeidon Hotkey = ", lParam );
#endif

   // Get access to hotkey object instance ... it must exist.
   GetViewByName( &vHotkey, szl__HKEY, m_pZSubtask->m_vDialog, zLEVEL_TASK );
   SetEntityCursor( vHotkey, szlHotkey, 0, zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0, (zSHORT) lParam, 0, 0 );
   GetStringFromAttribute( szDLL_Name, sizeof( szDLL_Name ), vHotkey, szlHotkey, szlDLL );
   GetStringFromAttribute( szOperName, sizeof( szOperName ), vHotkey, szlHotkey, szlCOP );

   lpfnDynRoutine = (zFARPROC_DRIVER) GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog, (LPLPLIBRARY) &hHotkeyLibrary,
                                                                      szDLL_Name, szOperName, "(drvr hotkey)" );
   if ( lpfnDynRoutine )
   {
      CWaitCursor wait;
      (*lpfnDynRoutine)( m_pZSubtask->m_vDialog );
   }

   return( 0 );
}

LRESULT
ZModalDlg::OnPostedClose( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZModalDlg::OnPostedClose", "" );
#endif
   if ( mIs_hWnd( m_hWnd ) == 0 )
   {
      TraceLineS( "ZModalDlg::OnPostedClose Invalid Window", "" );
      ASSERT( mIs_hWnd( m_hWnd ) );
      return( 0 );
   }

   // Remove this frame window from the list of frame windows.
   AFX_MODULE_THREAD_STATE *pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
   CFrameWnd *pFrameWnd = pState->m_frameList;
   while ( pFrameWnd )
   {
      if ( pFrameWnd->m_hWnd == m_hWnd )
      {
         pState->m_frameList.Remove( pFrameWnd );
         break;
      }

      pFrameWnd = pFrameWnd->m_pNextFrameWnd;
   }

   ZSubtask *pZSubtask = (ZSubtask *) lParam;

   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   if ( ulSubtaskState == zSUBTASK_STATE_ERROR )
      m_pZSubtask = 0;

   ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
   if ( ulSubtaskState == zSUBTASK_STATE_ERROR )
     pZSubtask = 0;

   if ( m_pZSubtask && (pZSubtask == m_pZSubtask || (wParam == -1 && lParam == (zLONG) m_hWnd)) )
   {
      ZTask *pZTask = m_pZSubtask->m_pZTask;

      WINDOWPLACEMENT wp;
      wp.length = sizeof( wp );
      if ( GetWindowPlacement( &wp ) )
      {
         wp.flags = 0;
         if ( IsZoomed( ) )
            wp.flags |= WPF_RESTORETOMAXIMIZED;

         APPBARDATA abd;
         abd.cbSize = sizeof( abd );
         abd.hWnd = m_hWnd;

         // Ignored members: hwnd, uCallbackMessage, uEdge, rc, lParam
         SHAppBarMessage( ABM_GETTASKBARPOS, &abd );
         UINT uEdge = GetTaskbarEdge( abd.rc );

         // abd.rc contains the rectangular location of the taskbar in screen coordinates.
         if ( uEdge == ABE_TOP )
         {
            wp.rcNormalPosition.top -= (abd.rc.bottom - abd.rc.top);
            wp.rcNormalPosition.bottom -= (abd.rc.bottom - abd.rc.top);
         }
         else
         if ( uEdge == ABE_LEFT )
         {
            wp.rcNormalPosition.left -= (abd.rc.right - abd.rc.left);
            wp.rcNormalPosition.right -= (abd.rc.right - abd.rc.left);
         }

         // Write the window placement to the .INI file.
         WriteWindowPlacement( m_pZSubtask->m_vDialog, "ZDR", *(m_pZSubtask->m_pzsDlgTag),
                               *(m_pZSubtask->m_pzsWndTag), &wp );
      }

      m_pZSubtask->EnableParentWindows( TRUE );

      m_pZSubtask->m_pZFWnd->m_hWnd = 0;
      ZDrFrame *pZDrFrame = (ZDrFrame *) m_pZSubtask->m_pZFWnd;
      pZDrFrame->m_pZSubtask = 0;

      // dks conversational fix
      if ( m_pZSubtask->m_szActionName[ 0 ] )
         strcpy_s( m_pchReturnCtrlTag, zTAG_LTH, m_pZSubtask->m_szActionName );
      else
      if ( m_pZSubtask->m_pzsLastTag )
         strcpy_s( m_pchReturnCtrlTag, zTAG_LTH, *(m_pZSubtask->m_pzsLastTag) );
      else
         m_pchReturnCtrlTag[ 0 ] = 0;

      mDeleteInit( m_pZSubtask->m_pZFWnd );
      pZTask->DeleteSubtask( m_pZSubtask );
   // EndDialog( IDOK ); // This value is returned by DoModal!
   // return( 1 );
   }
   else
   {
      TraceLineX( "PostedClose failed for Subtask: ", lParam );
      if ( m_pZSubtask )
         m_pZSubtask->m_pZTask->DeleteSubtask( m_pZSubtask );
   }

   if ( wParam == 0 )
   {
      CDialog::OnCancel( );
   // EndDialog( IDCANCEL ); // This value is returned by DoModal!
   }
   else
   {
      CDialog::OnOK( );
   // EndDialog( IDOK );
   }

   return( 1 );
}
