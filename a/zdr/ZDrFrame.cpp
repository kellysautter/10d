/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrframe.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of class ZDrFrame.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.03.18    DKS    XP
//    Enable/Disable System Menu items appropriately for XP.
//
// 2001.10.25    DKS    Z10
//    Eliminate GPF when trying to activate a window that is being destroyed.
//
// 2001.08.08    DKS    R55618
//    Fix for Replace window with same window.
//
// 2001.05.22    DKS    Z10
//    Eliminated hanging hourglass when showing popup menu.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2000.13.03    TMV    TB872
//    Make window resizeable even if it is created modal.
//
// 1999.12.06    DKS    Z10    TB861
//    Fixed initial paint problems when repositioning to previous coordinates.
//
// 1999.10.19    DKS    Z10    QS9999
//    Drag & Drop to a listbox within a group now shows the proper cursor.
//
// 1999.09.15    DKS    Z10    ActiveX
//    Added Mapping List.
//
// 1999.07.01    DKS    Z10    QS999
//    Tooltips are now visible to the painter.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit single document for all views.
//
// 1999.02.03    DKS
//    Removed pass thru of "unknown commands" to default CFrameWnd.
//
// 1999.01.23    DKS
//    Minimize parent(s) when minimizing a modal sub-window.  Set the system
//    menu according to the window styles.
//
// 1998.12.15    DKS
//    Synchronize forced close from OE ... permit zZEIDON_POSTED_CLOSE
//    with a validation that does not require the pointer to the ZSubtask.
//
// 1998.12.04    DKS
//    WriteWindowPlacement's new signature.
//
// 1998.10.12    DKS
//    Modified Dialog conversion to set the size of the Relative Pos/Size
//    area properly.
//
// 1998.10.05    DKS
//    Corrected algorithm for locating a Subtask across tasks.
//
// 1998.10.05    DKS
//    Fix to gray of menu options.
//
// 1998.09.03    DKS
//    Attempted fix to SetWindowState DESTROY case.
//
// 1998.08.13    DKS
//    Commented out Debug TraceLine.
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

//     .~.
//     /V\
//    // \\
//   /(   )\
//    ^^-^^

// #define DEBUG_ALL

/////////////////////////////////////////////////////////////////////////////
// ZDrFrame

// IMPLEMENT_DYNAMIC( ZDrFrame, CFrameWnd )
IMPLEMENT_DYNCREATE( ZDrFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( ZDrFrame, CFrameWnd )
   ON_WM_CREATE( )
   ON_WM_CLOSE( )
   ON_WM_DESTROY( )
   ON_WM_NCDESTROY( )
   ON_WM_ACTIVATE( )
   ON_WM_CTLCOLOR( )
   ON_WM_ERASEBKGND( )
   ON_WM_INITMENU( )
   ON_WM_INITMENUPOPUP( )
   ON_WM_KILLFOCUS( )
   ON_WM_MENUSELECT( )
   ON_WM_MOVE( )
   ON_WM_PAINT( )
   ON_WM_PALETTECHANGED( )
// ON_WM_QUERYDRAGICON( )
   ON_WM_ENABLE( )
   ON_WM_SETCURSOR( )
   ON_WM_SETFOCUS( )
   ON_WM_SIZE( )
   ON_WM_SYSCOLORCHANGE( )
   ON_WM_SYSCOMMAND( )
   ON_WM_GETMINMAXINFO( )

#if 0
   ON_COMMAND( ID_EDIT_BM_CLEARALL, OnMenuSelection )
   ON_COMMAND( ID_EDIT_BM_NEXT, OnMenuSelection )
   ON_COMMAND( ID_EDIT_BM_PREV, OnMenuSelection )
   ON_COMMAND( ID_EDIT_BM_TOGGLE, OnMenuSelection )
   ON_COMMAND( ID_EDIT_FINDINFILES, OnMenuSelection )
   ON_COMMAND( ID_EDIT_INDENT, OnMenuSelection )
   ON_COMMAND( ID_EDIT_FIND, OnMenuSelection )

   ON_COMMAND( ID_EDIT_COPY, OnEditCopy )
   ON_COMMAND( ID_EDIT_CUT, OnEditCut )
   ON_COMMAND( ID_EDIT_PASTE, OnEditPaste )
   ON_COMMAND( ID_EDIT_UNDO, OnEditUndo )
   ON_COMMAND( ID_EDIT_REDO, OnEditRedo )
#endif

   ON_MESSAGE( WM_SETMESSAGESTRING, OnSetMessageString )
   ON_MESSAGE( zZEIDON_PROCESS_EVENT, OnProcessEvent )
   ON_MESSAGE( zZEIDON_PROCESS_ACTION, OnProcessAction )
   ON_MESSAGE( zZEIDON_PROCESS_HOTKEY, OnProcessHotkey )
   ON_MESSAGE( zZEIDON_POSTED_ACTIVATE, OnPostedActivate )
   ON_MESSAGE( zZEIDON_POSTED_POPUP_MENU, OnPostedPopupMenu )
// ON_MESSAGE( zZEIDON_POSTED_CLOSE, OnPostedClose )
   ON_REGISTERED_MESSAGE( g_uMsgPostedClose, OnPostedClose )
   ON_MESSAGE( zZEIDON_POSTED_SPLITTER_CLOSE, OnPostedSplitterClose )
   ON_MESSAGE( zZEIDON_POSTED_MARK_DELETE, OnPostedMarkDelete )
   ON_MESSAGE( zZEIDON_RESUME_FROM_PAINTER, OnResumeFromPainter )
   ON_MESSAGE( zZEIDON_FIND_DIALOG_WINDOW, OnFindDialogWindow )
   ON_MESSAGE( zZEIDON_SET_WINDOW_STATE, OnSetWindowState )
   ON_MESSAGE( zZEIDON_SET_WAB, OnSetWindowActionBehavior )
   ON_MESSAGE( zZEIDON_BEGIN_DRAG, OnBeginDrag )
   ON_MESSAGE( zZEIDON_QUERY_DROP, OnQueryDrop )
   ON_MESSAGE( zZEIDON_DO_DROP, OnDoDrop )

   ON_UPDATE_COMMAND_UI( SC_RESTORE, OnUpdateRestore )
   ON_UPDATE_COMMAND_UI( SC_MINIMIZE, OnUpdateMinimize )
   ON_UPDATE_COMMAND_UI( SC_MAXIMIZE, OnUpdateMaximize )

#if 0
   // Global help commands
   ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
   ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
   ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
   ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)

   // turning on and off standard frame gadgetry
   ON_UPDATE_COMMAND_UI( ID_VIEW_STATUS_BAR, OnUpdateControlBarMenu )
   ON_COMMAND_EX( ID_VIEW_STATUS_BAR, OnBarCheck )
   ON_UPDATE_COMMAND_UI( ID_VIEW_TOOLBAR, OnUpdateControlBarMenu )
   ON_COMMAND_EX( ID_VIEW_TOOLBAR, OnBarCheck )

   // turning on and off standard mode indicators
   ON_UPDATE_COMMAND_UI( ID_INDICATOR_CAPS, OnUpdateKeyIndicator )
   ON_UPDATE_COMMAND_UI( ID_INDICATOR_NUM, OnUpdateKeyIndicator )
   ON_UPDATE_COMMAND_UI( ID_INDICATOR_SCRL, OnUpdateKeyIndicator )
   ON_UPDATE_COMMAND_UI( ID_INDICATOR_KANA, OnUpdateKeyIndicator )

   // standard help handling
   ON_UPDATE_COMMAND_UI( ID_CONTEXT_HELP, OnUpdateContextHelp )
#endif

   // toolbar "tooltip" notification
   ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText )
   ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText )
// ON_NOTIFY( TBN_DROPDOWN, 0xE822, OnToolbarDropDown )
   ON_NOTIFY_EX_RANGE( TBN_DROPDOWN, zZEIDON_FIRST_TOOLBAR, zZEIDON_LAST_TOOLBAR, OnToolbarDropDown )
END_MESSAGE_MAP( )

#if 0
static UINT indicators[] =
{
   ID_SEPARATOR,           // status line indicator
   ID_INDICATOR_CAPS,
   ID_INDICATOR_NUM,
   ID_INDICATOR_SCRL,
};
#endif

// Do I have to have a menu or a title bar with an SDI MFC application?
// You will need to override CFrameWnd::LoadMenu if you intend to use the
// Microsoft Foundation Class Library (MFC) document/view architecture
// (which is what AppWizard provides). You will also need to override
// CFrameWnd::PreCreateWindow so you can tweak your window style so that
// a caption is not on the main window.

/////////////////////////////////////////////////////////////////////////////
// ZDrFrame construction/destruction

zLONG ZDrFrame::g_lIdNbr = 1;
ZDrFrame::ZDrFrame( ZSubtask *pZSubtask )
{
   m_ulFrameIdNbr = g_lIdNbr++;
   if ( pZSubtask )
      m_pZSubtask = pZSubtask;
   else
      m_pZSubtask = ((ZDrApp *) AfxGetApp( ))->m_pZSubtaskCurr;

   m_pZSubtaskPopup = 0;

#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame::ctor (0x%08x): %d  %s ++++", this, m_ulFrameIdNbr,
              m_pZSubtask ? *(m_pZSubtask->m_pzsWndTag) : "" );
#endif

// m_bAutoMenuEnable = FALSE;
   m_pZView = 0;
   m_pZTwain = 0;
   m_pSplitterWnd = 0;
   m_nMinSizeX = 0;
   m_nMinSizeY = 0;
   m_nMaxSizeX = 0;
   m_nMaxSizeY = 0;
   m_pSysMenuEvents = 0;
   m_ulFrameFlags = 0;

#if 0
   Provided WNDCLASSes

   In previous versions of MFC (prior to MFC 4.0), there were a number of
   predefined Window classes provided.  These Window classes are no longer
   provided by default because of technical problems related to versioning
   (multiple versions of MFC loaded in one address space) as well as
   concerns relating to the fact that both MFC applications and OLE
   Controls may use the MFC DLLs.

   The following reference is provided to help migrate code that uses
   these previously provided WNDCLASSes.  Applications should use
   AfxRegisterWndClass (with the appropriate parameters) in place of these
   classes.

   The following shows the classes and their attributes:

     "AfxWnd" is used for all child windows created with CWnd::Create.
     class style : CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW
     no icon
     arrow cursor
     no background color
     "AfxFrameOrView" is used for frame windows and views (including
      stand-alone CFrameWnds and CMDIChildWnds).
     class style : CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
     icon AFX_IDI_STD_FRAME
     arrow cursor
     COLOR_WINDOW background color
     "AfxMDIFrame" is used for the MDI frame window (that is, the parent)
      created with CMDIFrameWnd::Create.
     class style : CS_DBLCLKS [ reduces flash during resizing ]
     icon AFX_IDI_STD_MDIFRAME
     arrow cursor
     no background color
     "AfxControlBar" is used for the standard control bar implementation.
     class style : 0 [reduces flash during resizing, no double clicks]
     no icon
     arrow cursor
     gray background color (COLOR_BTNFACE)

   If the application provides a resource with the specified resource ID
   (for example, AFX_IDI_STD_FRAME) ID, MFC will use that resource. Otherwise
   the default resource is used. For the icon, the standard application icon
   is used, and for the cursor, the standard arrow cursor is used.

   There are two icons that support MDI applications with single document
   types (one icon for the main application, the other icon for iconic
   document/MDIChild windows). For multiple document types with different
   icons, you must register additional WNDCLASSes or use the
   CFrameWnd::LoadFrame function.

   CFrameWnd::LoadFrame will automatically register a WNDCLASS using the
   standard "AfxFrameOrView" attributes but using the icon ID you specify
   as the first parameter to LoadFrame.

   The values for background color and cursor for the MDIFrameWnd are not
   used since the client area of the MDIFrameWnd is completely covered by
   the "MDICLIENT" window. Microsoft does not encourage subclassing the
   "MDICLIENT" window so use the standard colors and cursor types when
   possible.

   LPCTSTR AFXAPI
   AfxRegisterWndClass( UINT nClassStyle, HCURSOR hCursor = 0,
                        HBRUSH hbrBackground = 0, HICON hIcon = 0 );

   CS_GLOBALCLASS  Allows an application to create a window of the class
      regardless of the value of the hInstance parameter passed to the
      CreateWindow or CreateWindowEx function.  If you do not specify this
      style, the hInstance parameter passed to the CreateWindow (or
      CreateWindowEx) function must be the same as the hInstance parameter
      passed to the RegisterClass function.

      You can create a global class by creating the window class in a
      dynamic-link library (DLL) and listing the name of the DLL in the
      registry under the following keys:

      HKEY_LOCAL_MACHINE\Software
      \Microsoft\Windows NT\
      CurrentVersion\Windows\APPINIT_DLLS

      Whenever a process starts, the operating system loads the specified
      DLLs in the context of the newly started process before calling the
      main function in that process.  The DLL must register the class during
      its initialization procedure and must specify the CS_GLOBALCLASS style.

   BOOL CreateEx( DWORD dwExStyle, LPCTSTR lpszClassName,
                  LPCTSTR lpszWindowName, DWORD dwStyle,
                  int x, int y, int nWidth, int nHeight,
                  HWND hwndParent, HMENU nIDorHMenu, LPVOID lpParam = NULL );

   BOOL CreateEx( DWORD dwExStyle, LPCTSTR lpszClassName,
                  LPCTSTR lpszWindowName, DWORD dwStyle,
                  const RECT& rect, CWnd* pParentWnd,
                  UINT nID, LPVOID lpParam = NULL);

   Return Value

      Nonzero if successful; otherwise 0.

   Parameters

      dwExStyle   Specifies the extended style of the CWnd being created.
                  Apply any of the extended window styles to the window.
      lpszClassName Points to a null-terminated character string that
                  names the Windows class (a WNDCLASS structure). The class
                  name can be any name registered with the global
                  AfxRegisterWndClass function or any of the predefined
                  control-class names. It must not be NULL.
      lpszWindowName Points to a null-terminated character string that
                  contains the window name.
      dwStyle     Specifies the window style attributes. See Window Styles
                  and CWnd::Create for a description of the possible values.
      x           Specifies the initial x-position of the CWnd window.
      y           Specifies the initial top position of the CWnd window.
      nWidth      Specifies the width (in device units) of the CWnd window.
      nHeight     Specifies the height (in device units) of the CWnd window.
      hwndParent  Identifies the parent or owner window of the CWnd window
                  being created. Use NULL for top-level windows.
      nIDorHMenu  Identifies a menu or a child-window identifier. The
                  meaning depends on the style of the window.
      lpParam     Points to the data referenced by the lpCreateParams field
                  of the CREATESTRUCT structure.
      rect        The size and position of the window, in client coordinates
                  of pParentWnd.
      pParentWnd  The parent window.
      nID         The ID of the child window.

   Remarks

      Creates an overlapped, pop-up, or child window with the extended
      style specified in dwExStyle.

      WS_EX_ACCEPTFILES   Specifies that a window created with this style
                           accepts drag-and-drop files.
      WS_EX_CLIENTEDGE    Specifies that a window has a 3D look - that is,
                           a border with a sunken edge.
      WS_EX_CONTEXTHELP   Includes a question mark in the title bar of the
                           window.  When the user clicks the question mark,
                           the cursor changes to a question mark with a
                           pointer.  If the user then clicks a child window,
                           the child receives a WM_HELP message.
      WS_EX_CONTROLPARENT Allows the user to navigate among the child
                           windows of the window by using the TAB key.
      WS_EX_DLGMODALFRAME Designates a window with a double border that may
                           (optionally) be created with a title bar when you
                           specify the WS_CAPTION style flag in the dwStyle
                           parameter.
      WS_EX_LEFT          Gives window generic left-aligned properties. This
                           is the default.
      WS_EX_LEFTSCROLLBAR Places a vertical scroll bar to the left of the
                           client area.
      WS_EX_LTRREADING    Displays the window text using left-to-right
                           reading order properties. This is the default.
      WS_EX_MDICHILD      Creates an MDI child window.
      WS_EX_NOPARENTNOTIFY Specifies that a child window created with this
                           style will not send the WM_PARENTNOTIFY message
                           to its parent window when the child window is
                           created or destroyed.
      WS_EX_OVERLAPPEDWINDOW Combines the WS_EX_CLIENTEDGE and
                           WS_EX_WINDOWEDGE styles
      WS_EX_PALETTEWINDOW Combines the WS_EX_WINDOWEDGE and WS_EX_TOPMOST
                           styles.
      WS_EX_RIGHT         Gives a window generic right-aligned properties.
                           This depends on the window class.
      WS_EX_RIGHTSCROLLBAR Places a vertical scroll bar (if present) to the
                           right of the client area. This is the default.
      WS_EX_RTLREADING    Displays the window text using right-to-left
                           reading order properties.
      WS_EX_STATICEDGE    Creates a window with a three-dimensional border
                           style intended to be used for items that do not
                           accept user input.
      WS_EX_TOOLWINDOW    Creates a tool window, which is a window intended
                           to be used as a floating toolbar. A tool window
                           has a title bar that is shorter than a normal
                           title bar, and the window title is drawn using a
                           smaller font. A tool window does not appear in the
                           task bar or in the window that appears when the
                           user presses ALT+TAB.
      WS_EX_TOPMOST       Specifies that a window created with this style
                           should be placed above all nontopmost windows
                           and stay above them even when the window is
                           deactivated.  An application can use the
                           SetWindowPos member function to add or remove this
                           attribute.
      WS_EX_TRANSPARENT   Specifies that a window created with this style is
                           to be transparent. That is, any windows that are
                           beneath the window are not obscured by the window.
                           A window created with this style receives WM_PAINT
                           messages only after all sibling windows beneath it
                           have been updated.
      WS_EX_WINDOWEDGE    Specifies that a window has a border with a raised
                           edge.

      The CreateEx parameters specify the WNDCLASS, window title, window
      style, and (optionally) initial position and size of the window.
      CreateEx also specifies the window's parent (if any) and ID.

      When CreateEx executes, Windows sends the WM_GETMINMAXINFO,
      WM_NCCREATE, WM_NCCALCSIZE, and WM_CREATE messages to the window.

      To extend the default message handling, derive a class from CWnd,
      add a message map to the new class, and provide member functions for
      the above messages. Override OnCreate, for example, to perform
      needed initialization for a new class.

      Override further OnMessage message handlers to add further
      functionality to your derived class.

      If the WS_VISIBLE style is given, Windows sends the window all the
      messages required to activate and show the window. If the window
      style specifies a title bar, the window title pointed to by the
      lpszWindowName parameter is displayed in the title bar.

      The dwStyle parameter can be any combination of window styles.

   // Window Styles
      WS_OVERLAPPED       0x00000000L
      WS_POPUP            0x80000000L
      WS_CHILD            0x40000000L
      WS_MINIMIZE         0x20000000L
      WS_VISIBLE          0x10000000L
      WS_DISABLED         0x08000000L
      WS_CLIPSIBLINGS     0x04000000L
      WS_CLIPCHILDREN     0x02000000L
      WS_MAXIMIZE         0x01000000L
      WS_CAPTION          0x00C00000L     // WS_BORDER | WS_DLGFRAME
      WS_BORDER           0x00800000L
      WS_DLGFRAME         0x00400000L
      WS_VSCROLL          0x00200000L
      WS_HSCROLL          0x00100000L
      WS_SYSMENU          0x00080000L
      WS_THICKFRAME       0x00040000L
      WS_MINIMIZEBOX      0x00020000L
      WS_MAXIMIZEBOX      0x00010000L
      WS_GROUP            0x00020000L
      WS_TABSTOP          0x00010000L

      WS_TILED            WS_OVERLAPPED
      WS_ICONIC           WS_MINIMIZE
      WS_SIZEBOX          WS_THICKFRAME
      WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW

      WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
                           WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

      WS_POPUPWINDOW      (WS_POPUP | WS_BORDER | WS_SYSMENU)

      WS_CHILDWINDOW      (WS_CHILD)

   // Extended Window Styles

      WS_EX_DLGMODALFRAME     0x00000001L
      WS_EX_NOPARENTNOTIFY    0x00000004L
      WS_EX_TOPMOST           0x00000008L
      WS_EX_ACCEPTFILES       0x00000010L
      WS_EX_TRANSPARENT       0x00000020L

    if WINVER >= 0x0400
      WS_EX_MDICHILD          0x00000040L
      WS_EX_TOOLWINDOW        0x00000080L
      WS_EX_WINDOWEDGE        0x00000100L
      WS_EX_CLIENTEDGE        0x00000200L
      WS_EX_CONTEXTHELP       0x00000400L

      WS_EX_RIGHT             0x00001000L
      WS_EX_LEFT              0x00000000L
      WS_EX_RTLREADING        0x00002000L
      WS_EX_LTRREADING        0x00000000L
      WS_EX_LEFTSCROLLBAR     0x00004000L
      WS_EX_RIGHTSCROLLBAR    0x00000000L

      WS_EX_CONTROLPARENT     0x00010000L
      WS_EX_STATICEDGE        0x00020000L
      WS_EX_APPWINDOW         0x00040000L


      WS_EX_OVERLAPPEDWINDOW  (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
      WS_EX_PALETTEWINDOW     (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)

    endif // WINVER >= 0x0400

#endif

   // Creation is achieved using LoadFrame in ZSubtask::CreateZ
// VERIFY( CFrameWnd::Create( NULL, _T( "CFrame::Create - Zeidon Application" ),
//                            WS_OVERLAPPEDWINDOW, rectDefault, NULL, MAKEINTRESOURCE( IDR_MAINFRAME ) ) );
}

ZDrFrame::~ZDrFrame( )
{
#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame::dtor [0x%08x]: %d   Subtask: [0x%08x] IdNbr: %d  %s.%s ++++",
              this, m_ulFrameIdNbr, m_pZSubtask,
              m_pZSubtask ? m_pZSubtask->m_ulSubtaskIdNbr : 0,
              m_pZSubtask ? *(m_pZSubtask->m_pzsDlgTag) : "",
              m_pZSubtask ? *(m_pZSubtask->m_pzsWndTag) : "" );
#endif

   mDeleteInit( m_pSplitterWnd );
   mDeleteInit( m_pZSubtask );
   mDeleteInit( m_pSysMenuEvents );
// mDeleteInit( m_pZView );
}

/////////////////////////////////////////////////////////////////////////////
// Rules the taskbar uses to decide whether a button should be shown for
// a window:
// - if the WS_EX_APPWINDOW style is set, the taskbar shows a button
// - if the WS_EX_TOOLWINDOW style is set, the taskbar does NOT show a button
// Note: You should never create a window with both of these styles set.
// - if the window has neither of the above styles set, the taskbar decides
//   to create a button if the window is unowned, and does not create a
//   button if the window is owned.
// Note: If the WS_VISIBLE style is off, the taskbar does not create a button,
//   since the taskbar never shows a button for a hidden window.
//
BOOL
ZDrFrame::PreCreateWindow( CREATESTRUCT& cs )
{
   // TODO: Modify the Window class or styles here by modifying
   // the CREATESTRUCT cs
   if ( m_pZSubtask->m_pZParent && m_pZSubtask->m_pZParent->m_pZFWnd )
      cs.hwndParent = m_pZSubtask->m_pZParent->m_pZFWnd->m_hWnd;

   cs.x = m_pZSubtask->Attr.X;
   cs.y = m_pZSubtask->Attr.Y;
   cs.cx = m_pZSubtask->Attr.W;
   cs.cy = m_pZSubtask->Attr.H;

   // CFrameWnd::PreCreateWindow(cs) sets some extended styles that need to
   // be removed in this app. One of them is WS_EX_OVERLAPPEDWINDOW. If this
   // style isn't removed, there will be a window border problem.
// cs.dwExStyle &= ~WS_EX_OVERLAPPEDWINDOW;

   return( CFrameWnd::PreCreateWindow( cs ) );
}

// FindMenuItem( ) will find a menu item string from the specified
// popup menu and returns its position (0-based) in the specified
// popup menu.  It returns -1 if no such menu item string is found.
int
FindMenuItem( CMenu *pMenu, zCPCHAR cpcMenuText )
{
   CString cs;
   ASSERT( ::IsMenu( pMenu->GetSafeHmenu( ) ) );

   int nCount = pMenu->GetMenuItemCount( );
   for ( int k = 0; k < nCount; k++ )
   {
      if ( pMenu->GetMenuString( k, cs, MF_BYPOSITION ) && (zstrstr( cs, cpcMenuText ) != 0) )
      {
         return( k );
      }
   }

   return( -1 );
}

int
ZDrFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

#ifdef DEBUG_ALL
   zSHORT k = 0;
   TraceLine( "ZDrFrame::OnCreate  %s.%s  hWnd: 0x%08x   <<<<<==============",
              *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), m_hWnd );
   ListWindowsAndControls( m_pZSubtask, "ZDrFrame::OnCreate", 0 );
// if ( zstrcmp( *(m_pZSubtask->m_pzsWndTag), "ProspectDetail" ) == 0 )
//    k /= k;
#endif

   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSubtask->m_vDialog, "ZDr", 0, 0 );

   zPVOID pvDialog = SysGetHandleFromPointer( m_pZSubtask->m_vDialog );
   zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   while ( lItemCnt > 0 )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask && pvDialog == g_pSubtaskList[ lItemCnt ].pvDialog && m_pZSubtask == g_pSubtaskList[ lItemCnt ].pZSubtask )
      {
         g_pSubtaskList[ lItemCnt ].hWnd = m_hWnd;
         break;
      }

      lItemCnt--;
   }

   SysMutexUnlock( m_pZSubtask->m_vDialog, "ZDr", 0 );

// TraceLineX( "ZDrFrame::OnCreate Style: ", m_pZSubtask->Attr.Style );
   ModifyStyle( ~(m_pZSubtask->Attr.Style | 0x00001111), m_pZSubtask->Attr.Style & 0x11110000 );
   if ( m_pZSubtask->m_pZParent )
   {
//    TraceLineS( "(frame) OnCreate modify style from OVERLAPPED to POPUP: ", *(m_pZSubtask->m_pzsWndTag) );
   // ModifyStyle( 0, /* WS_POPUPWINDOW | */ WS_MINIMIZEBOX | WS_MAXIMIZEBOX );

      m_pZSubtask->Attr.StyleEx &= ~WS_EX_APPWINDOW;
      m_pZSubtask->Attr.StyleEx |= WS_EX_CONTROLPARENT;
      ModifyStyleEx( WS_EX_APPWINDOW, m_pZSubtask->Attr.StyleEx );
   }
   else
   {
      m_pZSubtask->Attr.StyleEx |= WS_EX_APPWINDOW | WS_EX_CONTROLPARENT | WS_EX_OVERLAPPEDWINDOW;
      ModifyStyleEx( 0, m_pZSubtask->Attr.StyleEx );
   }

   if ( m_pZSubtask->m_hInstDLL )
   {
      HINSTANCE hInst = AfxGetResourceHandle( );
      AfxSetResourceHandle( m_pZSubtask->m_hInstDLL );
      HICON hIcon = m_pZSubtask->m_pZTask->m_pZDrApp->LoadIcon( *(m_pZSubtask->m_pzsDLL_Name) );
//    HICON hIcon = ::LoadIcon( m_pZSubtask->m_hInstDLL, pchDLL_Name );
      if ( hIcon )
      {
         SetIcon( hIcon, FALSE );      // Set small icon
         SetIcon( hIcon, TRUE );       // Set big icon
      }

      AfxSetResourceHandle( hInst );
   }

#if 0
   if ( m_wndToolBar.Create( this ) == 0 || m_wndToolBar.LoadToolBar( IDR_MAINFRAME ) == 0 )
   {
      TraceLineS( "Failed to create toolbar", "" );
      return( -1 );      // fail to create
   }
#endif

#if 0
   if ( m_wndStatusBar.Create( this ) == 0 || m_wndStatusBar.SetIndicators( indicators, mElementCnt( indicators ) ) == 0 )
   {
      TraceLineS( "ZDrFrame::OnCreate Failed to create status bar", "" );
      return( -1 );      // fail to create
   }

   // Remove this if you don't want tool tips or a resizeable toolbar.
   m_wndToolBar.SetBarStyle( m_wndToolBar.GetBarStyle( ) | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

   // Delete these three lines if you don't want the toolbar to be dockable.
   m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
   EnableDocking( CBRS_ALIGN_ANY );
   DockControlBar( &m_wndToolBar );
#endif

   SetSystemMenuItem( "Min/Restore", "&Close", 0xEFF0, 0, 0 );

// SendMessage( WM_SETFONT, (WPARAM) m_pZSubtask->m_hFont );
   return( 0 );
}

// All predefined Control-menu items have ID numbers greater than 0xF000. If an application adds items to the
// Control menu, it should use ID numbers less than 0xF000.
zBOOL
ZDrFrame::SetSystemMenuItem( zCPCHAR cpcItemText, zCPCHAR cpcAfterItem,
                             zUSHORT uItemId,
                             zCPCHAR cpcDLL_Name, zCPCHAR cpcOperationName )
{
   if ( mIs_hWnd( m_hWnd ) )
   {
      CMenu *pSystemMenu = GetSystemMenu( FALSE );
      if ( pSystemMenu )
      {
         int nPos = FindMenuItem( pSystemMenu, cpcAfterItem );
         if ( nPos <= 0 )
            pSystemMenu->AppendMenu( MF_STRING, uItemId, cpcItemText );
         else
            pSystemMenu->InsertMenu( nPos - 1, MF_BYPOSITION, uItemId, cpcItemText );

         if ( cpcDLL_Name && cpcOperationName && *cpcDLL_Name && *cpcOperationName )
         {
            if ( m_pSysMenuEvents == 0 )
               m_pSysMenuEvents = new ZEventItem( 0 ); // we don't care about the Id

            CString cs = cpcDLL_Name;
            cs += "\t";
            cs += cpcOperationName;

            m_pSysMenuEvents->AddParm( uItemId, cs );
         }

         return( TRUE );
      }
   }

   return( FALSE );
}

zOPER_EXPORT zSHORT OPERATION
SetSystemMenuItem( zVIEW vSubtask, zCPCHAR cpcItemText,
                   zCPCHAR cpcAfterItem, zUSHORT uItemId,
                   zCPCHAR cpcDLL_Name, zCPCHAR cpcOperationName )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pZFWnd )
      {
         zBOOL bRC =
         ((ZDrFrame *) (pZSubtask->m_pZFWnd))->SetSystemMenuItem( cpcItemText, cpcAfterItem, uItemId, cpcDLL_Name, cpcOperationName );
         if ( bRC )
            return( 0 );
      }
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: AddHotkeysToGlobalHotkeys
//
// Adds the Hotkeys defined to the Window to the Global Hotkeys object.  This
// should generally be called from prebuild for the Login or Statup window of
// the Application.  They are added before the existing Global Hotkeys so
// that the Application can override any conflicting Hotkeys.
//
// All hot keys on the window that calls AddHotkeysToGlobalHotkeys
// must have operation code.  The code must:
//  A. use Conversational Dialogs, or
//  B. retrieve the view to a window that is currently loaded (usually
//     the "main" window of the application and the one that called
//     AddHotkeysToGlobalHotkeys), then invoke an action in that window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
AddHotkeysToGlobalHotkeys( zVIEW vSubtask )
{
   zCHAR   Accels[ 8192 ];
   zVIEW   vHotkey;
   zULONG  ulHotkeyLth;
   zULONG  ulGlobalHotkeyLth;
   zSHORT  nPosition;
   zSHORT  nRC;

   GetViewByName( &vHotkey, "__HKEY", vSubtask, zLEVEL_TASK );
   if ( vHotkey == 0 )
   {
      IssueError( vSubtask, 0, 0,
                  "Internal Application Error:  Hotkey object has not yet been created." );
      return( -1 );
   }

   // Concatenate the binary hotkey accelerator table information.
   GetAttributeLength( &ulHotkeyLth, vSubtask, "Wnd", "Hotkey" );
   GetAttributeLength( &ulGlobalHotkeyLth, vHotkey, "App", "Hotkey" );
   GetBlobFromAttribute( Accels, &ulHotkeyLth, vSubtask, "Wnd", "Hotkey" );
   GetBlobFromAttribute( Accels + ulHotkeyLth, &ulGlobalHotkeyLth, vHotkey, "App", "Hotkey" );

   // Reset the ID since it is used later to position on the Hotkey entity.  Also force the zACCEL_ACTION window keys
   // to zACCEL_HOTKEY keys that invoke operation code.
   zSHORT k, kk;
   k = (zSHORT) (ulHotkeyLth / sizeof( zACCEL_TABLE ) + ulGlobalHotkeyLth / sizeof( zACCEL_TABLE ));
   kk = k - 2;
   LPACCEL_TABLE lpAT = (LPACCEL_TABLE) Accels;
// while ( k >= 0 )
// {
//    TraceLine( "AddHotkeysToGlobalHotkeys: lpAT[%d].nID ==> %d for VK = 0x%x", k, lpAT[ k ].nID, lpAT[ k ].ac.uVK );
//    k--;
// }

   while ( kk >= 0 )
   {
      lpAT[ kk ].nID = kk;
      if ( lpAT[ kk ].chType == zACCEL_ACTION )
         lpAT[ kk ].chType = zACCEL_HOTKEY;

      TraceLine( "AddHotkeysToGlobalHotkeys: reset lpAT[%d].nID ==> %d for VK = 0x%x", kk, lpAT[ kk ].nID, lpAT[ kk ].ac.uVK );
      kk--;
   }

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zmemcpy( pZSubtask->m_pAccels, Accels, ulHotkeyLth + ulGlobalHotkeyLth );
      SetAttributeFromBlob( vHotkey, "App", "Hotkey", Accels, ulHotkeyLth + ulGlobalHotkeyLth );

      // Now add the Hotkey entities that define the operation to execute.
      SetCursorFirstEntity( vHotkey, "Hotkey", 0 );
      nPosition = zPOS_BEFORE;
      for ( nRC = SetCursorFirstEntity( vSubtask, "Hotkey", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vSubtask, "Hotkey", 0 ) )
      {
         if ( CheckExistenceOfEntity( vSubtask, "HotAct" ) >= zCURSOR_SET )
         {
            CreateEntity( vHotkey, "Hotkey", nPosition );
            SetAttributeFromAttribute( vHotkey, "Hotkey", "Tag", vSubtask, "Hotkey", "Tag" );
            SetAttributeFromAttribute( vHotkey, "Hotkey", "Title", vSubtask, "Hotkey", "Title" );
            SetAttributeFromAttribute( vHotkey, "Hotkey", "COP", vSubtask, "HotAct", "DO" );
            SetAttributeFromAttribute( vHotkey, "Hotkey", "DLL", vSubtask, "Dlg", "DLL" );
            nPosition = zPOS_AFTER;
         }
      }

      return( 0 );
   }
   else
      return( -1 );

} // AddHotkeysToGlobalHotkeys

BOOL
ZDrFrame::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMsg )
{
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && m_pZSubtask && m_pZSubtask->m_pZView )
   {
      return( CFrameWnd::OnSetCursor( pWnd, uHitTest, uMsg ) );
   }
   else
      return( TRUE );  // halt further processing
}

void
ZDrFrame::OnEnable( BOOL bEnable )
{
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && m_pZSubtask && m_pZSubtask->m_pZView )
   {
      CFrameWnd::OnEnable( bEnable );
   }
}

void
ZDrFrame::OnClose( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnClose", "" );
#endif
// AfxMessageBox( "ZDrFrame::OnClose" );
// mDeleteInit( m_pZSubtask );
// mDeleteInit( m_pZView );
   CFrameWnd::OnClose( );
}

void
ZDrFrame::OnNcDestroy( )
{
#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame::OnNcDestroy: [0x%08x]  %s.%s  hWnd: 0x%08x", this,
              m_pZSubtask && m_pZSubtask->m_pzsDlgTag ? *(m_pZSubtask->m_pzsDlgTag) : "Dlg?",
              m_pZSubtask && m_pZSubtask->m_pzsWndTag ? *(m_pZSubtask->m_pzsWndTag) : "Wnd?", m_hWnd );
#endif

   CFrameWnd::OnNcDestroy( );
}

void
ZDrFrame::OnDestroy( )
{
#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame::OnDestroy  [0x%08x]  IdNbr: %d  %s.%s  IdNbr: %d  hWnd: 0x%08x  =======>>>>>>>>>>>",
              this, m_ulFrameIdNbr, *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), m_pZSubtask->m_ulSubtaskIdNbr, m_hWnd );
   ListWindowsAndControls( m_pZSubtask, "ZDrFrame::OnDestroy", 0 );
#endif

   ZSubtask *pZSubtask = m_pZSubtask;
   m_pZSubtask = 0;
   mDeleteInit( pZSubtask );   // set to zero to prohibit further reference

#if 0
   // MFC code: reset menu to default before final shutdown
   if ( m_hMenuDefault && ::GetMenu( m_hWnd ) != m_hMenuDefault )
      ::SetMenu( m_hWnd, m_hMenuDefault );

   if ( ::GetMenu( m_hWnd ) == m_hMenuDefault )
      CFrameWnd::OnDestroy( );
   else
      DestroyDockBars( );
#else
   CFrameWnd::OnDestroy( );
#endif
}

BOOL
ZDrFrame::DestroyWindow( )
{
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   if ( ulSubtaskState & zSUBTASK_STATE_STOP_RECURSE )
      return( FALSE );

#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame::DestroyWindow: [0x%08x] IdNbr: %d  Subtask: %s.%s  IdNbr: %d",
              this, m_ulFrameIdNbr,
              m_pZSubtask && m_pZSubtask->m_pzsDlgTag ? *(m_pZSubtask->m_pzsDlgTag) : "",
              m_pZSubtask && m_pZSubtask->m_pzsWndTag ? *(m_pZSubtask->m_pzsWndTag) : "",
              m_pZSubtask ? m_pZSubtask->m_ulSubtaskIdNbr : 0 );
#endif

   ZSubtask::SetSubtaskState( m_pZSubtask, 0, zSUBTASK_STATE_STOP_RECURSE );

#ifdef __WIN32__
   WINDOWPLACEMENT wp;
   wp.length = sizeof( wp );

   if ( (m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) && m_pZSubtask != m_pZSubtask->GetSplitterMainSubtask( ) )
   {
#if 0
      if ( m_pZView->m_pOrigParent == this )
      {
         if ( m_pZSubtask->m_pZParent && m_pZSubtask->m_pZParent->m_pSplitterView )
         {
            m_pZSubtask->m_pZParent->m_pSplitterView->SendMessage( zZEIDON_POSTED_SPLITTER_CLOSE, 0, (LPARAM) m_pZSubtask );
         }
      }
#endif
   }
   else
   if ( GetWindowPlacement( &wp ) )
   {
      if ( IsZoomed( ) )
         wp.flags = WPF_RESTORETOMAXIMIZED;
      else
         wp.flags = 0;

      APPBARDATA abd;
      abd.cbSize = sizeof( abd );
      abd.hWnd = m_hWnd;

      // Ignored members: uCallbackMessage, uEdge, rc, lParam
      if ( SHAppBarMessage( ABM_GETTASKBARPOS, &abd ) == FALSE )
      {
         abd.hWnd = ::GetDesktopWindow( );
         SHAppBarMessage( ABM_GETTASKBARPOS, &abd );
      }

      UINT uEdge = GetTaskbarEdge( abd.rc );

   // #define ABE_LEFT        0
   // #define ABE_TOP         1
   // #define ABE_RIGHT       2
   // #define ABE_BOTTOM      3

   // TraceRect( "ZDrFrame::OnDestroy NormalPos:", wp.rcNormalPosition );
   // TraceRect( "ZDRFrame::OnDestroy abd.rc:", abd.rc );
   // TraceLineI( "ZdrFrame::OnDestroy uEdge", uEdge );

      // abd.rc contains the rectangular location of the taskbar in
      // screen coordinates.
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
      WriteWindowPlacement( m_pZSubtask->m_vDialog, "ZDR", *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), &wp );
   }
#else
   if ( (m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) == 0 &&
        m_pZSubtask->m_pzsProfileViewName )
   {
      zVIEW vProfile;

      if ( GetViewByName( &vProfile, *(m_pZSubtask->m_pzsProfileViewName), m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         SaveWindowPlacement( m_pZSubtask->m_vDialog, vProfile, 0L );
      }
   }
#endif

   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_ON_CLEANUP) == zWND_EVENT_ON_CLEANUP &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog, szlWndEvent, szlType, zWND_EVENT_ON_CLEANUP, 0 ) > zCURSOR_UNCHANGED )
   {
      ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0, zACTION_DIALOG_CODE_ONLY, TRUE, "OnCleanup" );   // what happens on error???
   }

   // Moved to support TB ActiveX Editor - 1998/4/18
// SetMenu( 0 );   moved to ZSubtask dtor for timing purposes
// mDeleteInit( m_pZSubtask->m_pMenu );
   // Moved to support TB ActiveX Editor - 1998/4/18

   m_pZSubtask->SaveBarState( );

   m_pZSubtask->m_pZTask->ReleaseTwain( this );
   return( CFrameWnd::DestroyWindow( ) );
}

/////////////////////////////////////////////////////////////////////////////
// ZDrFrame diagnostics

#ifdef _DEBUG
void
ZDrFrame::AssertValid( ) const
{
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   if ( (ulSubtaskState & zSUBTASK_STATE_DELETED) == zSUBTASK_STATE_DELETED )
   {
      ListWindowsAndControls( m_pZSubtask, "ZDrFrame::OnDestroy!!!!!!", 0 );
      ASSERT( FALSE );
   }

   CFrameWnd::AssertValid( );
}

void
ZDrFrame::Dump( CDumpContext& dc ) const
{
   CFrameWnd::Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ZDrFrame message handlers
//

/////////////////////////////////////////////////////////////////////////////
// Command notifications for CCmdTarget notifications
//
// #define CN_COMMAND              0               // void ()
// #define CN_UPDATE_COMMAND_UI    ((UINT)(-1))    // void (CCmdUI*)
// #define CN_EVENT                ((UINT)(-2))    // OLE event
// #define CN_OLECOMMAND           ((UINT)(-3))    // OLE document command
// #define CN_OLE_UNREGISTER       ((UINT)(-4))    // OLE unregister
//    > 0 are control notifications
//    < 0 are for MFC's use
//
BOOL
ZDrFrame::OnCmdMsg( UINT uID, int nCode, void *pExtra,
                    AFX_CMDHANDLERINFO *pHandlerInfo )
{
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );

   if ( CFrameWnd::OnCmdMsg( uID, nCode, pExtra, pHandlerInfo ) != 0 || (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) != 0 ||
                             m_pZSubtask == 0 || m_pZSubtask->m_pZMIXCtrl == 0 )
   {
      return( TRUE );
   }

   // Any questions ... do it here!
#if 1
   if ( uID >= AFX_IDW_CONTROLBAR_FIRST + 32 && uID <= AFX_IDW_CONTROLBAR_LAST )
   {
      if ( nCode == CN_UPDATE_COMMAND_UI || nCode == CN_COMMAND )
      {
      // TraceLineI( "ZDrFrame::OnCmdMsg CN_UPDATE_COMMAND_UI nID: ", nID - (AFX_IDW_CONTROLBAR_FIRST + 31) );
      // TraceLineI( "ZDrFrame::OnCmdMsg nMsg ", HIWORD( nCode ) );
      // TraceLineI( "          OnCmdMsg nCode ", LOWORD( nCode ) );
         ZSubtask *pZSubtask;
         if ( m_pZSubtask->m_pZSubtaskGlobal )
            pZSubtask = m_pZSubtask->m_pZSubtaskGlobal;
         else
            pZSubtask = m_pZSubtask;

         CCmdUI *pCmdUI = (CCmdUI *) pExtra;
         if ( pCmdUI && pCmdUI->m_pOther )
         {
            zLONG   lPos = 1;
            ZMapAct *pzma;
            while ( (pzma = (ZMapAct *) pZSubtask->m_pZMIXCtrl->GetAt( lPos )) != 0 )
            {
               if ( pzma->m_pCtrl == pCmdUI->m_pOther )
               {
                  if ( nCode == CN_UPDATE_COMMAND_UI )
                     return( pzma->IsEventHandled( uID ) );
                  else
                     return( pzma->HandleEvent( uID ) );
               }

               lPos++;
            }
         }
      }
   // else
   //    TraceLineI( "ZDrFrame::OnCmdMsg Code: ", nCode );
   }
#endif

   return( FALSE );
}

BOOL
ZDrFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext *pContext )
{
#if 0
   // TODO: Add your specialized code here and/or call the base class
   if (!m_wndSplitter.CreateStatic(this, 1, 2))
   {
      TRACE0("Failed to create splitter window\n");
      return FALSE;
   }

   // Get the client rect first for calc left pane size
   CRect rect;
   GetClientRect(&rect);

   // create the left tree view first.
   if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CPaneView), CSize(rect.Width()/3, 0), pContext))
   {
      TRACE0("Failed to create left pane view\n");
      return FALSE;
   }

   // The right pane is a frame which and contain several different views.
   // The is can be set to active or non-active
   if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CPaneFrame), CSize(0, 0), pContext))
   {
      TRACE0("Failed to create right pane frame\n");
      return FALSE;
   }

   CPaneView* pLeftPaneView = (CPaneView *) m_wndSplitter.GetPane(0,0);
   pLeftPaneView->m_pRightPaneFrame = (CPaneFrame *) m_wndSplitter.GetPane(0,1);

   // Set the left pane as the active view
   SetActiveView((CView*) m_wndSplitter.GetPane(0, 0));

   return TRUE;
#else
#if 0

   return( CFrameWnd::OnCreateClient( lpcs, pContext ) );

#else

   ASSERT( mIs_hWnd( m_hWnd ) );

#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnCreateClient", "" );
#endif

   m_pZView = new ZDrView( m_pZSubtask );
   if ( m_pZView == 0 )
   {
      TraceLineS( "ZDrFrame::CreateView failed", "" );
      return( FALSE );
   }

#ifdef _SCB_REPLACE_MINIFRAME
   m_pFloatingFrameClass = RUNTIME_CLASS( ZSCBMiniDockFrameWnd );
#endif //_SCB_REPLACE_MINIFRAME

   // Views are always created with a border!
   if ( m_pZView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect( 0, 0, 0, 0 ), this, AFX_IDW_PANE_FIRST, pContext ) == 0 )
   {
      TraceLineS( "ZDrFrame::CreateView failed", " creation" );
      return( FALSE );   // can't continue without a view
   }

#if 0   // Assume NT is dead ... dks 2012.10.09
   // Need to get Windows version for this.
   if ( afxData.bWin4 && (m_pZView->GetExStyle( ) & WS_EX_CLIENTEDGE) )
   {
      // Remove the 3d style from the frame, since the view is providing it.  Make sure to recalc the non-client area.
      ModifyStyleEx( WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED );
   }
#endif

   return( TRUE );

#endif
#endif
}

zBOOL
ZDrFrame::SetApplicationState( UINT uCmdType )
{
   static zBOOL bRecurse = FALSE;
   ZTask    *pZTask;
   ZSubtask *pZSubtask;
   HWND     hWnd;
   zLONG    lState;
   zBOOL    bHandled = FALSE;

   if ( bRecurse )
      return( bHandled );

   bRecurse = TRUE;

   pZTask = m_pZSubtask->m_pZTask;
   switch ( uCmdType )
   {
      case SC_MINIMIZE:
      {
         pZSubtask = m_pZSubtask;
         if ( (pZSubtask->m_ulSubtaskFlags & zSUBTASK_CREATED_MODAL) == 0 )
         {
            bHandled = FALSE;  // for Jeff so that modeless windows do not
            break;             // cause their "parent" window to minimize.
         }

         while ( pZSubtask && pZSubtask->m_pZParent )
            pZSubtask = pZSubtask->m_pZParent;

         zPVOID pvDialog = SysGetHandleFromPointer( pZSubtask->m_vDialog );

         mDeleteInit( pZTask->m_pWndList );
         pZTask->m_pWndList = new ZPairedList( );

         SysMutexLock( pZSubtask->m_vDialog, "ZDr", 0, 0 );

         zLONG lPos = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
         while ( lPos > 0 )
         {
            if ( g_pSubtaskList[ lPos ].pZSubtask && g_pSubtaskList[ lPos ].lTID == pZSubtask->m_lTID &&
                 (g_pSubtaskList[ lPos ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && mIs_hWnd( g_pSubtaskList[ lPos ].hWnd ) )
            {
            // vSubtaskReturn = (zVIEW) SysGetPointerFromHandle( g_pSubtaskList[ lPos ].pvDialog );
            // if ( vSubtaskReturn && MiValidView( vSubtaskReturn ) &&

               lState = 0;

               if ( ::IsIconic( g_pSubtaskList[ lPos ].hWnd ) )  // minimized
                  lState |= 0x00000001;

               if ( ::IsZoomed( g_pSubtaskList[ lPos ].hWnd ) )  // maximized
                  lState |= 0x00000002;

               if ( mIs_hWndVisible( g_pSubtaskList[ lPos ].hWnd ) ) // visible
                  lState |= 0x00000004;

               if ( mIs_hWndEnabled( g_pSubtaskList[ lPos ].hWnd ) ) // enabled
                  lState |= 0x00000008;

               if ( g_pSubtaskList[ lPos ].pZSubtask == m_pZSubtask ) // active
                  lState |= 0x80000000;

               pZTask->m_pWndList->Add( (zLONG) g_pSubtaskList[ lPos ].hWnd, (zLONG) g_pSubtaskList[ lPos ].pZSubtask, lState, 0 );
               if ( g_pSubtaskList[ lPos ].pZSubtask == pZSubtask )
               {
                  CString csCaption;
                  CWnd    *pWnd = CWnd::FromHandle( g_pSubtaskList[ lPos ].hWnd );
                  pWnd->GetWindowText( pZTask->m_csHoldCaption );
                  pWnd->ShowWindow( SW_SHOWMINNOACTIVE ); // SW_SHOWMINIMIZED
                  GetWindowText( csCaption );
                  pWnd->SetWindowText( csCaption );
               }
               else
               {
                  ::ShowWindow( g_pSubtaskList[ lPos ].hWnd, SW_HIDE );
               }
            }

            lPos--;
         }

         SysMutexUnlock( pZSubtask->m_vDialog, "ZDr", 0 );
         bHandled = TRUE;
         break;
      }

      case SC_MAXIMIZE:
      case SC_RESTORE:
      {
         if ( pZTask->m_pWndList )
         {
            HWND hWndFocus = 0;
            ZPairedItem *pItem = pZTask->m_pWndList->m_pHeadItem;
            while ( pItem )
            {
               hWnd = (HWND) pItem->m_lValue1;
               pZSubtask = (ZSubtask *) pItem->m_lValue2;
               lState = pItem->m_lValue3;
               pZTask->m_pWndList->Remove( pItem );
            // pItem->m_pNextItem = 0;  done by Remove
               delete( pItem );

               if ( mIs_hWnd( hWnd ) )
               {
                  if ( lState & 0x00000004 )  // visible
                  {
                     if ( lState & 0x00000008 )  // enabled
                     {
                        if ( lState & 0x80000000 )  // active
                        {
                           if ( lState & 0x00000001 )  // minimized
                              ::ShowWindow( hWnd, SW_SHOWMINIMIZED );
                           else
                           if ( lState & 0x00000002 )  // maximized
                              ::ShowWindow( hWnd, SW_SHOWMAXIMIZED );
                           else
                              ::ShowWindow( hWnd, SW_SHOWNORMAL );

                           hWndFocus = hWnd;
                        }
                        else
                        {
                           if ( lState & 0x00000001 )  // minimized
                              ::ShowWindow( hWnd, SW_SHOWMINNOACTIVE );
                           else
                           if ( lState & 0x00000002 )  // maximized
                              ::ShowWindow( hWnd, SW_SHOWMAXIMIZED );
                           else
                              ::ShowWindow( hWnd, SW_SHOWNOACTIVATE );
                        }
                     }
                     else
                     {
                        // disabled ...
                        if ( lState & 0x80000000 )  // active
                        {
                           if ( lState & 0x00000001 )  // minimized
                              ::ShowWindow( hWnd, SW_SHOWMINIMIZED );
                           else
                           if ( lState & 0x00000002 )  // maximized
                              ::ShowWindow( hWnd, SW_SHOWMAXIMIZED );
                           else
                              ::ShowWindow( hWnd, SW_SHOWNORMAL );
                        }
                        else
                        {
                           if ( lState & 0x00000001 )  // minimized
                              ::ShowWindow( hWnd, SW_SHOWMINNOACTIVE );
                           else
                           if ( lState & 0x00000002 )  // maximized
                              ::ShowWindow( hWnd, SW_SHOWMAXIMIZED );
                           else
                              ::ShowWindow( hWnd, SW_SHOWNOACTIVATE );
                        }
                     }
                  }
               }

               pItem = pZTask->m_pWndList->m_pHeadItem;
            }

            bHandled = TRUE;
            if ( hWndFocus )
               ::SetFocus( hWndFocus );

            SetWindowText( pZTask->m_csHoldCaption );
         }

         mDeleteInit( pZTask->m_pWndList );
      }
   }

   bRecurse = FALSE;
   return( bHandled );
}

//
// afx_msg void OnSysCommand( UINT uCmdType, LPARAM lParam );
//
// Parameters
//
// uCmdType Specifies the type of system command requested. This parameter
// can be any one of the following values:
//
// SC_CLOSE Close the CWnd object.
//
// SC_HOTKEY Activate the CWnd object associated with the
// application-specified hot key. The low-order word of lParam
// identifies the HWND of the window to activate.
//
// SC_HSCROLL Scroll horizontally.
//
// SC_KEYMENU Retrieve a menu through a keystroke.
//
// SC_MAXIMIZE (or SC_ZOOM) Maximize the CWnd object.
//
// SC_MINIMIZE (or SC_ICON) Minimize the CWnd object.
//
// SC_MOUSEMENU Retrieve a menu through a mouse click.
//
// SC_MOVE Move the CWnd object.
//
// SC_NEXTWINDOW Move to the next window.
//
// SC_PREVWINDOW Move to the previous window.
//
// SC_RESTORE Restore window to normal position and size.
//
// SC_SCREENSAVE Executes the screen-saver application specified in
// the [boot] section of the SYSTEM.INI file.
//
// SC_SIZE Size the CWnd object.
//
// SC_TASKLIST Execute or activate the Windows Task Manager
// application.
//
// SC_VSCROLL Scroll vertically.
//
// lParam If a Control-menu command is chosen with the mouse, lParam
// contains the cursor coordinates. The low-order word contains the x
// coordinate, and the high-order word contains the y coordinate.
// Otherwise this parameter is not used.
//
// SC_HOTKEY Activate the window associated with the
// application-specified hot key. The low-order word of lParam
// identifies the window to activate.
//
// SC_SCREENSAVE Execute the screen-save application specified in the
// Desktop section of Control Panel.
//
// Remarks
//
// The framework calls this member function when the user selects a
// command from the Control menu, or when the user selects the Maximize
// or the Minimize button.
//
// By default, OnSysCommand carries out the Control-menu request for
// the predefined actions specified in the preceding table.
//
// In WM_SYSCOMMAND messages, the four low-order bits of the nID
// parameter are used internally by Windows. When an application tests
// the value of nID, it must combine the value 0xFFF0 with the nID
// value by using the bitwise-AND operator to obtain the correct
// result.
//
// The menu items in a Control menu can be modified with the
// GetSystemMenu, AppendMenu, InsertMenu, and ModifyMenu member
// functions. Applications that modify the Control menu must process
// WM_SYSCOMMAND messages, and any WM_SYSCOMMAND messages not handled
// by the application must be passed on to OnSysCommand. Any command
// values added by an application must be processed by the application
// and cannot be passed to OnSysCommand.  An application can carry out
// any system command at any time by passing a WM_SYSCOMMAND message to
// OnSysCommand.
//
// Accelerator (shortcut) keystrokes that are defined to select items
// from the Control menu are translated into OnSysCommand calls; all
// other accelerator keystrokes are translated into WM_COMMAND
// messages.
//
// Note This member function is called by the framework to allow your
// application to handle a Windows message. The parameters passed to
// your function reflect the parameters received by the framework when
// the message was received. If you call the base-class implementation
// of this function, that implementation will use the parameters
// originally passed with the message and not the parameters you supply
// to the function.
//
// ZDrFrame - system menu command
void
ZDrFrame::OnSysCommand( UINT uCmdType, LPARAM lParam )
{
// TraceLineX( "ZDrFrame::OnSysCommand = ", uCmdType );
   ZSubtask *pZSubtask;
   zLONG lEvent;
   zBOOL bHandled = FALSE;

   if ( m_pZSubtask->m_pZSubtaskGlobal )
      pZSubtask = m_pZSubtask->m_pZSubtaskGlobal;
   else
      pZSubtask = m_pZSubtask;

   // In WM_SYSCOMMAND messages, the four low-order bits of the wParam
   // parameter are used internally by the system. To obtain the correct
   // result when testing the value of wParam, an application must combine
   // the value 0xFFF0 with the wParam value by using the bitwise AND
   // operator.

   // SC_CLOSE is 0xF060, but when a popup menu is up, the uCmdType we get
   // is 0xF063 for some reason.  Since all SC_ messages end in 0, we
   // will try the following to clear the last 4 bits.
   switch ( uCmdType & 0xFFF0 )
   {
      case SC_CLOSE:
         lEvent = zWND_EVENT_CLOSE;
         if ( pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT )
         {
            CFrameWnd *pFrame = 0;
            if ( pZSubtask && pZSubtask->m_pZParent && pZSubtask->m_pZParent->m_pZFWnd && mIs_hWnd( pZSubtask->m_pZParent->m_pZFWnd->m_hWnd ) )
            {
            // TraceLineX( "ZDrFrame::OnSysCommand ShowWindow: ", (zLONG) pZSubtask->m_pZParent->m_pZFWnd->m_hWnd );
               pFrame = pZSubtask->m_pZParent->m_pZFWnd;
            }

        //  TraceLineS( "ZDrFrame::OnSysCommand Preview SC_CLOSE", "" );
            SendMessage( WM_COMMAND, AFX_ID_PREVIEW_CLOSE );
            if ( pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_CLOSE )
            {
               Default( );
            }

            if ( pFrame )
               pFrame->PostMessage( zZEIDON_POSTED_ACTIVATE );

            return;
         }

         break;

      case SC_MINIMIZE:
      {
         bHandled = SetApplicationState( SC_MINIMIZE );

//       TraceLineS( "ZDrFrame::OnSysCommand SC_MINIMIZE ", *(pZSubtask->m_pzsWndTag) );
#if 0 //def QUINSOFT
         if ( IsZoomed( ) )
         {
            if ( pZSubtask->m_pZTask->m_nOS_Version >= 60 && pZSubtask->m_pZTask->m_nOS_Version < 70 )
            {
               PostMessage( WM_SYSCOMMAND, SC_RESTORE, 0 );
               return;
            }
         }
#endif
         lEvent = zWND_EVENT_MINIMIZE;
         break;
      }

      case SC_MAXIMIZE:

         bHandled = SetApplicationState( SC_MAXIMIZE );

         // 2000.13.03  TMV   TB872
         // Make window resizeable even if it is created modal.
      // if ( pZSubtask->m_ulSubtaskFlags & zSUBTASK_CREATED_MODAL )
      // {
      //    lEvent = zWND_EVENT_RESTORE;
      //    uCmdType &= ~SC_MAXIMIZE;
      //    uCmdType |= SC_RESTORE;
      // }
      // else
#if 0 //def QUINSOFT
         if ( IsIconic( ) )
         {
            if ( pZSubtask->m_pZTask->m_nOS_Version >= 60 && pZSubtask->m_pZTask->m_nOS_Version < 70 )
            {
               PostMessage( WM_SYSCOMMAND, SC_RESTORE, 0 );
               return;
            }
         }
#endif

         lEvent = zWND_EVENT_MAXIMIZE;
         break;

      case SC_RESTORE:
         bHandled = SetApplicationState( SC_RESTORE );

//       TraceLineS( "ZDrFrame::OnSysCommand SC_RESTORE ", *(pZSubtask->m_pzsWndTag) );
         lEvent = zWND_EVENT_RESTORE;
         break;

      case 0xEFF0:
      // m_pSystemMenu = GetSystemMenu( FALSE );
      // if ( m_pSystemMenu )
         {
            if ( ::IsIconic( m_hWnd ) )  // minimized
            {
               bHandled = SetApplicationState( SC_RESTORE );
            // m_pSystemMenu->ModifyMenu( 0xEFF0, MF_BYCOMMAND | MF_STRING, 0xEFF0, "Minimize App" );
            }
            else
            {
               bHandled = SetApplicationState( SC_MINIMIZE );
            // m_pSystemMenu->ModifyMenu( 0xEFF0, MF_BYCOMMAND | MF_STRING, 0xEFF0, "Restore App" );
            }
         }

         lEvent = 0;
         break;

      default:
         if ( m_pSysMenuEvents && uCmdType < 0xF000 )
         {
            ZParmItem *pItem = m_pSysMenuEvents->FindParm( uCmdType );
            if ( pItem )
            {
               int nPos = pItem->GetValue( ).Find( "\t" );
               if ( nPos > 0 )
               {
                  zFARPROC_DRIVER lpfnDynRoutine;
                  LPLIBRARY lpLibrary;
                  CString   csDLL_Name;
                  CString   csOperation;

                  csDLL_Name = pItem->GetValue( ).Left( nPos );
                  csOperation = pItem->GetValue( ).Right( nPos );
                  lpfnDynRoutine = (zFARPROC_DRIVER) GetOperationDynamicCallAddress( pZSubtask->m_vDialog, (LPLPLIBRARY) &lpLibrary,
                                                                                     csDLL_Name, csOperation, "(SysCommand)" );
                  if ( lpfnDynRoutine )
                  {
                     zSHORT nRC = (*lpfnDynRoutine)( pZSubtask->m_vDialog );
                     if ( nRC == 0 )
                        bHandled = TRUE;
                  }
               }
            }
         }

         lEvent = 0;
         break;
   }

   if ( lEvent )
   {
      if ( pZSubtask->HandleEvent( lEvent ) == zACTIVEX_EVENT_COMPLETED )
         return;     // don't let default processing occur
   }

   if ( bHandled == FALSE )
      CFrameWnd::OnSysCommand( uCmdType, lParam );

// Default( ); // CWnd::OnSysCommand just calls Default
}

void
ZDrFrame::OnGetMinMaxInfo( MINMAXINFO *lpMMI )
{
   if ( m_nMinSizeX )
      lpMMI->ptMinTrackSize.x = m_nMinSizeX;

   if ( m_nMinSizeY )
      lpMMI->ptMinTrackSize.y = m_nMinSizeY;

   if ( m_nMaxSizeX )
      lpMMI->ptMaxTrackSize.x = m_nMaxSizeX;

   if ( m_nMaxSizeY )
      lpMMI->ptMaxTrackSize.y = m_nMaxSizeY;
}

LRESULT
ZDrFrame::OnProcessEvent( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnProcessEvent", "" );
#endif
// if ( m_pZSubtask->m_pEventParms == 0 )
//    m_pZSubtask->m_pEventParms = (zPVOID) wParam;

   if ( m_pZSubtask == 0 )
      return( 0 );

   return( ProcessActionLoop( m_pZSubtask, (zCPCHAR) lParam, wParam, 0, 0, FALSE, "CtrlEvent" ) );
}

LRESULT
ZDrFrame::OnProcessAction( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnProcessAction", "" );
#endif
   if ( SetEntityCursor( m_pZSubtask->m_vDialog, szlAct, 0, zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0, (zSHORT) lParam, 0, 0 ) > zCURSOR_UNCHANGED )
   {
      zPCHAR pch;

   // if ( m_pZSubtask->m_pEventParms == 0 )
   //    m_pZSubtask->m_pEventParms = (zPVOID) wParam;

      GetAddrForAttribute( &pch, m_pZSubtask->m_vDialog, szlAct, szlTag );
      ProcessActionLoop( m_pZSubtask, pch, wParam, 0, 0, FALSE, szlHotkey );
   }

   return( 0 );
}

LRESULT
ZDrFrame::OnProcessHotkey( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnProcessHotkey", "" );
#endif
   zFARPROC_DRIVER lpfnDynRoutine;
   LPLIBRARY hHotkeyLibrary = 0;
   zCHAR  szDLL_Name[ 9 ];
   zCHAR  szOperName[ zTAG_LTH ];
   zVIEW  vHotkey;

   // Get access to hotkey object instance ... it must exist.
   CreateViewFromViewForTask( &vHotkey, m_pZSubtask->m_pZTask->m_vHK, m_pZSubtask->m_vDialog );
   SetEntityCursor( vHotkey, szlHotkey, 0, zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0, (zSHORT) lParam, 0, 0 );
   GetStringFromAttribute( szDLL_Name, sizeof( szDLL_Name ), vHotkey, szlHotkey, szlDLL );
   GetStringFromAttribute( szOperName, sizeof( szOperName ), vHotkey, szlHotkey, szlCOP );
   DropView( vHotkey );  // created to stop RESETTING task message

// TraceLine( "ZDrFrame::OnProcessHotkey Zeidon Hotkey: %d   DLL: %s   Operation: %s", lParam, szDLL_Name, szOperName );
   lpfnDynRoutine = (zFARPROC_DRIVER) GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog, (LPLPLIBRARY) &hHotkeyLibrary,
                                               szDLL_Name, szOperName, "(drvr hotkey)" );
   if ( lpfnDynRoutine )
   {
      CWaitCursor wait;
      (*lpfnDynRoutine)( m_pZSubtask->m_vDialog );
   }
   else
   {
   // TraceLine( "ZDrFrame::OnProcessHotkey could not load operation %s from %s", szOperName, szDLL_Name );
   }

   return( 0 );
}

LRESULT
ZDrFrame::OnPostedActivate( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnPostedActivate", "" );
#endif

   ActivateFrame( SW_SHOWNORMAL );
   SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_ASYNCWINDOWPOS );
   SetWindowPos( &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_ASYNCWINDOWPOS );
   FlashWindow( FALSE );
   SetFocus( );
   m_pZSubtask->m_pZTask->m_pZSubtaskFocus = m_pZSubtask;
// TraceLineS( "ZDrFrame::OnPostedActivate: ", *(m_pZSubtask->m_pzsWndTag) );
   return( 1 );
}

LRESULT
ZDrFrame::OnPostedPopupMenu( WPARAM wParam, LPARAM lParam )
{
   if ( m_pZSubtask && m_pZSubtask->m_pzsPopupTag )
   {
      zCHAR szPopupTag[ zTAG_LTH ];
      CPoint pt = CPoint( lParam );

      strcpy_s( szPopupTag, sizeof( szPopupTag ), *(m_pZSubtask->m_pzsPopupTag) );
      m_pZSubtask->CreateZeidonPopupMenu( szPopupTag, pt );
   }

   return( 1 );
}

LRESULT
ZDrFrame::OnPostedSplitterClose( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineX( "ZDrFrame::OnPostedSplitterClose: ", (zLONG) this );
   if ( m_pZSubtask && m_pZSubtask->m_pzsWndTag )
      TraceLineS( "ZDrFrame::OnPostedSplitterClose: ", *(m_pZSubtask->m_pzsWndTag) );
#endif

   if ( m_pZSubtask->m_pSplitterView )
   {
      ZSplitterWnd *pSplitterWnd = m_pZSubtask->m_pSplitterView->m_pSplitterWnd;
      CFrameWnd *pFrame = (CFrameWnd *) pSplitterWnd->GetParentFrame( );
      zLONG lBaseId;

      ZDrView *pBaseView = pSplitterWnd->GetCurrentView( 0, 0, &lBaseId );
      ZDrView *pView = pSplitterWnd->RemoveView( lBaseId );
      if ( pSplitterWnd->m_pZView == pBaseView )
      {
         CWnd *pParentWnd = pSplitterWnd->m_pParentWnd;
         CWnd *p = pSplitterWnd->m_pZView->SetParent( pParentWnd );

         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, sizeof( szMsg ), "ZDrFrame::OnPostedSplitterClose SetParent of (SplitterData ZView) 0x%08x to (pParentWnd) 0x%08x from 0x%08x",
                    (zULONG) pSplitterWnd->m_pZView, (zULONG) pParentWnd, (zULONG) p );
         TraceLineS( szMsg, "" );

         if ( pSplitterWnd->m_pZView )
         {
            pSplitterWnd->m_pZView->m_pParentSplitterWnd = 0;
            pSplitterWnd->m_pZView = 0;
         }

         pSplitterWnd->DestroyWindow( );
         mDeleteInit( m_pZSubtask->m_pSplitterView->m_pSplitterWnd );
         m_pZSubtask->m_pSplitterView->DestroyWindow( );
         m_pZSubtask->m_pSplitterView = 0;
         pFrame->RecalcLayout( );
      }
   }

   return( 0 );
}

LRESULT
ZDrFrame::OnPostedMarkDelete( WPARAM wParam, LPARAM lParam )
{
   if ( wParam )
      m_ulFrameFlags |= zSUBTASK_STATE_MARK_FOR_DELETE;
   else
      m_ulFrameFlags &= ~zSUBTASK_STATE_MARK_FOR_DELETE;

   return( 1 );
}

LRESULT
ZDrFrame::OnPostedClose( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame::OnPostedClose: [0x%08x]  IdNbr: %d  lParam: 0x%08x  Subtask: [0x%08x] IdNbr: %d  %s.%s",
              this, m_ulFrameIdNbr, lParam, m_pZSubtask,
              m_pZSubtask ? m_pZSubtask->m_ulSubtaskIdNbr : 0,
              (m_pZSubtask && m_pZSubtask->m_pzsDlgTag) ? *(m_pZSubtask->m_pzsDlgTag) : "?",
              (m_pZSubtask && m_pZSubtask->m_pzsWndTag) ? *(m_pZSubtask->m_pzsWndTag) : "?" );
#endif

   m_ulFrameFlags |= zSUBTASK_STATE_MARK_FOR_DELETE;
   ZSubtask *pZSubtask = (ZSubtask *) lParam;
   if ( pZSubtask == m_pZSubtask || (wParam == -1 && lParam == (zLONG) m_hWnd) )
   {
      ZSubtask::SetSubtaskState( m_pZSubtask, 0, zSUBTASK_STATE_MARK_FOR_DELETE );
      m_pZSubtask->m_pZTask->DeleteSubtask( m_pZSubtask );
   // TraceLineS( "ZDrFrame::OnPostedClose Terminate", "" );
      return( 0 );
   }

   TraceLineX( "PostedClose failed for Subtask: ", lParam );
   return( 1 );
}

LRESULT
ZDrFrame::OnResumeFromPainter( WPARAM wParam, LPARAM lParam )
{
   fnResumeFromPainter( (ZSubtask *) lParam );
   return( 0 );
}

LRESULT
ZDrFrame::OnFindDialogWindow( WPARAM wParam, LPARAM lParam )
{
   ZSubtask *pZSubtask = 0;
   zVIEW    vSubtask = 0;
   ZFindDialog *pfd = (ZFindDialog *) SfLockTaskMemory( (zPVOID) lParam );

// TraceLineX( "OnFindDialogWindow pfd: ", (zLONG) pfd );
// TraceLineX( "OnFindDialogWindow pfd->pvDialog: ", (zLONG) pfd->pvDialog );

   if ( pfd->pvDialog )
   {
      GetWindowAndCtrl( &(pfd->pZSubtask), 0, (zVIEW) SysGetPointerFromHandle( pfd->pvDialog ), 0 );
   }
   else
   {
      pfd->pZSubtask = fnGetSubtask( m_pZSubtask, &vSubtask, pfd->DlgN, pfd->WndN, FALSE );
   }

   if ( pfd->pZSubtask )
   {
      pZSubtask = pfd->pZSubtask;
      pfd->hWnd = pZSubtask->m_pZFWnd->m_hWnd;
      pfd->pvDialog = SysGetHandleFromPointer( vSubtask );
      strcpy_s( pfd->DlgN, sizeof( pfd->DlgN ), *(pZSubtask->m_pzsDlgTag) );
      strcpy_s( pfd->WndN, sizeof( pfd->WndN ), *(pZSubtask->m_pzsWndTag) );
   }

   SfUnlockTaskMemory( pfd );
   return( (LPARAM) pZSubtask );
}

// This function is Copyright (c) 2000, Cristi Posea.
// See www.datamekanix.com for more control bars tips&tricks.
BOOL
ZDrFrame::VerifyBarState( zCPCHAR cpcProfileName )
{
   CDockState state;
   state.LoadState( cpcProfileName );

   for ( zSHORT k = 0; k < state.m_arrBarInfo.GetSize( ); k++ )
   {
      CControlBarInfo *pInfo = (CControlBarInfo *) state.m_arrBarInfo[ k ];
      ASSERT( pInfo );
      if ( pInfo->m_bDocking && pInfo->m_pBar )
         ASSERT( pInfo->m_pBar->m_pDockContext );

      zSHORT  nDockedCount = pInfo->m_arrBarID.GetSize( );
      if ( nDockedCount > 0 )
      {
          // dockbar
          for ( zSHORT j = 0; j < nDockedCount; j++ )
          {
             UINT nID = (UINT) pInfo->m_arrBarID[ j ];
             if ( nID == 0 )
                continue; // row separator

             if ( nID > 0xFFFF )
                 nID &= 0xFFFF; // placeholder - get the ID

             if ( GetControlBar( nID ) == NULL )
                 return( FALSE );
          }
      }

      if ( pInfo->m_bFloating == FALSE ) // floating dockbars can be created later
      {
         if ( GetControlBar( pInfo->m_nBarID ) == NULL )
            return( FALSE ); // invalid bar ID
      }
   }

   return( TRUE );
}

LRESULT
ZDrFrame::OnSetWindowState( WPARAM wParam, LPARAM lParam )
{
#if 1 //def DEBUG_ALL
   if ( wParam == zWINDOW_STATUS_LAST )
   {
      ListWindowsAndControls( m_pZSubtask, "", lParam );
      return( 0 );
   }
// else
//    TraceLineS( "ZDrFrame::OnSetWindowState", "" );
#endif

   return( SetWindowState( m_pZSubtask->m_vDialog, wParam, lParam ) );
}

LRESULT
ZDrFrame::OnSetWindowActionBehavior( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnSetWindowActionBehavior", "" );
#endif
   ZFindDialog *pfd = (ZFindDialog *) SfLockTaskMemory( (zPVOID) lParam );
   LRESULT lr = SetWindowActionBehavior( m_pZSubtask->m_vDialog,
                                         pfd->lTID,  // carries behavior
                                         pfd->DlgN, pfd->WndN );
   pfd = (ZFindDialog *) SfUnlockTaskMemory( pfd );
   return( lr );
}

ZOption *
fnFindOption( ZMultiIdxSet *pMenu, UINT uID )
{
   ZOption *pOption;
   zULONG  ulNodeCnt = pMenu->GetCount( );
// zULONG  ulNodeCnt = pMenu->GetMaxCount( );
   zULONG  k;

   for ( k = 1; k <= ulNodeCnt; k++ )
   {
      pOption = (ZOption *) pMenu->GetAt( k );
      if ( pOption && pOption->m_uOptionID == uID )
         return( pOption );
   }

   return( 0 );
}

// ZDrFrame - menu/control command
//
// Here we handle all menu and accelerator notifications at once rather
// than have separate response table entries for each ...
//
// WindowProc calls OnCommand to handle WM_COMMAND messages.  uID is the
// identifier of the menu item or control.  hWndCtl holds a value that
// represents the control sending the message.  If the message is not from
// a control, it is 0.  uNotifyCode holds a value that represents the
// control's notification message.  If the message is from an accelerator,
// notifyCode is 1; if the message is from a menu, notifyCode is 0.
//
// The application should return non-zero if it processes this message,
// otherwise return zero.  This is different from the Win SDK documentation
// which implies that zero should be returned if the application processes
// the message ... which one is correct???  I am going with SDK doc for now.
//
// ??????????????????
// Parameters
//
// wNotifyCode
//
// Value of the high-order word of wParam.  Specifies the notification code
// if the message is from a control.  If the message is from an accelerator,
// this parameter is 1.  If the message is from a menu, this parameter is 0.
//
// wID
//
// Value of the low-order word of wParam. Specifies the identifier of the
// menu item, control, or accelerator.
//
// hwndCtl
//
// Value of lParam.  Identifies the control sending the message if the
// message is from a control.  Otherwise, this parameter is NULL.
//
// Return Value
//
// An application returns nonzero if it processes this message!!!
// otherwise 0.
//
// Parameters
//
// wParam The low-order word of wParam identifies the command ID of
// the menu item or control.  The high-order word of wParam specifies
// the notification message if the message is from a control.  If the
// message is from an accelerator, the high-order word is 1.  If the
// message is from a menu, the high-order word is 0.
//
// lParam Identifies the control that sends the message if the message is
// from a control.  Otherwise, lParam is 0.
//
// Remarks
//
// The framework calls this member function when the user selects an item
// from a menu, when a child control sends a notification message, or when
// an accelerator keystroke is translated.
//
// OnCommand processes the message map for control notification and
// ON_COMMAND entries, and calls the appropriate member function.
//
// Override this member function in your derived class to handle the
// WM_COMMAND message.  An override will not process the message map unless
// the base class OnCommand is called.
//
// Note This member function is called by the framework to allow your
// application to handle a Windows message.  The parameters passed to your
// function reflect the parameters received by the framework when the
// message was received. If you call the base-class implementation of this
// function, that implementation will use the parameters originally passed
// with the message and not the parameters you supply to the function.
//
BOOL
ZDrFrame::OnCommand( WPARAM wParam, LPARAM lParam )
{
// TraceLineX( "ZDrFrame::OnCommand wParam: ", wParam );
// TraceLineX( "ZDrFrame::OnCommand lParam: ", lParam );

   UINT k;
   UINT uID = LOWORD( wParam );
   UINT uNotifyCode = HIWORD( wParam );
   HWND hWndCtrl = (HWND) lParam;

// TraceLineI( "ZDrFrame::OnCommand NotifyCode = ", uNotifyCode );
// TraceLineI( "ZDrFrame::OnCommand hWndCtrl = ", (zLONG) hWndCtrl );
// TraceLineI( "ZDrFrame::OnCommand uID = ", uID );

   if ( uID >= zZEIDON_FIRST_TOOLBAR_CMD && uID <= zZEIDON_LAST_TOOLBAR_CMD )
   {
   // TraceLineX( "ZDrFrame::OnCommand TOOLBAR uID = ", uID );
      return( CFrameWnd::OnCommand( wParam, lParam ) );
   }

   if ( uID >= zZEIDON_FIRST_TOOLBAR_CMD + 0x0100 &&
        uID <= zZEIDON_LAST_TOOLBAR_CMD + 0x0100 )
   {
   // TraceLineX( "ZDrFrame::OnCommand TOOLBAR ctrl uID = ", uID );
      return( CFrameWnd::OnCommand( wParam, lParam ) );
   }

   // Conventional WM_COMMAND processing.
   if ( uID == 0 )
   {
      TraceLineS( szlDrvr, "OnCommand not processed for =========== ID 0" );
      return( TRUE );       // 0 control IDs are not allowed!
   }

   // Note that uID's < zPOPUP_MENU_FIRST are standard menu id's default routing for command messages (through closure table).
   if ( hWndCtrl == 0 )  // ==> this is a menu command
   {
      zVIEW vApp = 0;
      zCHAR szChkValue[ 65 ];

      ZSubtask *pZSubtask = m_pZSubtask->m_pZSubtaskGlobal ? m_pZSubtask->m_pZSubtaskGlobal : m_pZSubtask;
      if ( m_pZSubtaskPopup && (uID >= zPOPUP_MENU_FIRST) )
         pZSubtask = m_pZSubtaskPopup;

      m_pZSubtaskPopup = 0;

      if ( pZSubtask->m_pZBinMenu && uID < zPOPUP_MENU_FIRST )
      {
         ZOption *pOption = (ZOption *) pZSubtask->m_pZBinMenu->GetAt( uID );

         if ( pOption == 0 || uID != pOption->m_uOptionID )
            pOption = fnFindOption( pZSubtask->m_pZBinMenu, uID );

//       uNotifyCode = 0;        // accelerators are not special
         if ( pOption )
         {
#if 0
            // These Traces cause a GPF when exiting using a menu option.
            TraceLineS( "Option command Tag - ", *(pOption->m_pzsTag) );
            TraceLineS( "Option command ViewName - ", *(pOption->m_pzsVName) );
            TraceLineS( "Option command Entity - ", *(pOption->m_pzsEName) );
            TraceLineS( "Option command Attribute - ", *(pOption->m_pzsAName) );
            TraceLineS( "Option command ACTION - ", *(pOption->m_pzsActionTag) );
#endif

            if ( pOption->m_pzsVName )
            {
               if ( GetViewByName( &vApp, *(pOption->m_pzsVName), pOption->m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
               {
                  vApp = 0;   // this is done by OE
               }
            }

            // If it's off ... it's going to be turned on.  If there are other options that have the same mapping, they must all
            // be turned off (to mimic radio button behavior)
            if ( vApp && pOption->m_bChecked == FALSE )
            {
               ZOption *pOptTemp;
               zULONG  ulNodeCnt = m_pZSubtask->m_pZBinMenu->GetCount( );

               for ( k = 1; k <= ulNodeCnt; k++ )
               {
                  pOptTemp = (ZOption *) m_pZSubtask->m_pZBinMenu->GetAt( k );

                  // Note that the m_bChecked test knocks out the
                  // current option (known to be unchecked).
                  if ( pOptTemp->m_bChecked && pOptTemp->m_pzsVName &&
                       zstrcmp( (*(pOptTemp->m_pzsAName)).GetString(), (*(pOption->m_pzsAName)).GetString() ) == 0 &&
                       zstrcmp( (*(pOptTemp->m_pzsEName)).GetString(), (*(pOption->m_pzsEName)).GetString() ) == 0 &&
                       zstrcmp( (*(pOptTemp->m_pzsVName)).GetString(), (*(pOption->m_pzsVName)).GetString() ) == 0 )
                  {
                     pOptTemp->m_bChecked = FALSE;
//                   pOption->m_pZSubtask->m_pMenu->CheckMenuItem( pOptTemp->m_uOptionID,
//  done in MenuInit method!!!                                     MF_BYCOMMAND | MF_UNCHECKED );
                  }
               }
            }

            if ( vApp &&
                 CheckExistenceOfEntity( vApp, *(pOption->m_pzsEName) ) > zCURSOR_UNCHANGED )
            {
               if ( pOption->m_bChecked )
               {
                  strcpy_s( szChkValue, sizeof( szChkValue ), *(pOption->m_pzsValueOff) );
                  pOption->m_bChecked = FALSE;
               }
               else
               {
                  strcpy_s( szChkValue, sizeof( szChkValue ), *(pOption->m_pzsValueOn) );
                  pOption->m_bChecked = TRUE;
               }

//             TraceLineS( "Cmd OptTag - ", *(pOption->m_pzsTag) );
//             TraceLineS( " Set Value - ", szChkValue );

               SetAttributeFromVariable( vApp, *(pOption->m_pzsEName), *(pOption->m_pzsAName), szChkValue, zTYPE_STRING, 0,
                                         *(pOption->m_pzsContext), **(pOption->m_pzsContext) ? 0 : zUSE_DEFAULT_CONTEXT );
//             GetStringFromAttribute( szChkValue, sizeof( szChkValue ), vApp, *(pOption->m_pzsEName), *(pOption->m_pzsAName) );
//             TraceLineS( " Get Value Before - ", szChkValue );

//             pOption->m_pZSubtask->m_pMenu->CheckMenuItem( pOption->m_uOptionID,
//  done in MenuInit method!!!                               MF_BYCOMMAND | (pOption->m_bChecked ? MF_CHECKED : MF_UNCHECKED) );
            }

            if ( pOption->m_pzsActionTag )
            {
               pOption->m_pZSubtask->m_pzsLastTag = pOption->m_pzsTag;
               ProcessActionLoop( pOption->m_pZSubtask, *(pOption->m_pzsActionTag), 0, 0, 0, FALSE, "Option" );
            }

//          if ( vApp )
//          {
//             GetStringFromAttribute( szChkValue, sizeof( szChkValue ), vApp, *(pOption->m_pzsEName), *(pOption->m_pzsAName) );
//             TraceLineS( " Get Value After - ", szChkValue );
//          }

            return( TRUE );   // We have processed the menu ... if we do not return TRUE (==> we have processed the
                              // message), the CWnd class will attempt to forward the message to the default proc.
                              // If our processing has destroyed this frame window, the CWnd instance will have been
                              // freed from memory and we will crash!
         }
      }
      else
      if ( pZSubtask->m_pZBinPopup && uID >= zPOPUP_MENU_FIRST )
      {
         k = uID;
         k %= zPOPUP_MENU_FIRST;
         ZOption *pOption = (ZOption *) pZSubtask->m_pZBinPopup->GetAt( k );

         if ( pOption == 0 || uID != pOption->m_uOptionID )
            pOption = fnFindOption( pZSubtask->m_pZBinPopup, uID );

         if ( pOption && pOption->m_pzsVName && GetViewByName( &vApp, *(pOption->m_pzsVName), pOption->m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
         {
            if ( pOption->m_bChecked )
            {
               strcpy_s( szChkValue, sizeof( szChkValue ), *(pOption->m_pzsValueOff) );
               pOption->m_bChecked = FALSE;
            }
            else
            {
               strcpy_s( szChkValue, sizeof( szChkValue ), *(pOption->m_pzsValueOn) );
               pOption->m_bChecked = TRUE;
            }

//          TraceLineS( "CmdPop OptTag - ", *(pOption->m_pzsTag) );
//          TraceLineS( " Set Value - ", szChkValue );

            SetAttributeFromVariable( vApp, *(pOption->m_pzsEName), *(pOption->m_pzsAName), szChkValue, zTYPE_STRING, 0,
                                      *(pOption->m_pzsContext), **(pOption->m_pzsContext) ? 0 : zUSE_DEFAULT_CONTEXT );
         }

         if ( pOption && pOption->m_pzsActionTag )
         {
            pOption->m_pZSubtask->m_pzsLastTag = pOption->m_pzsTag;
            ProcessActionLoop( pOption->m_pZSubtask, *(pOption->m_pzsActionTag), 0, 0, 0, FALSE, "PopUp" );
         }

         return( TRUE );   // We have processed the popup menu ... if we not return TRUE (==> we have processed the
                           // message), the CWnd class will attempt to forward the message to the default proc.
                           // If our processing has destroyed this frame window, the CWnd instance will have been
                           // freed from memory and we will crash!
      }
   }

#if 0  // we don't want to have to notify the control if not necessary ...
       // let's see how this works in the new world of CHILD_NOTIFY???
   else
   if ( m_pZSubtask && m_pZSubtask->m_pZMIXCtrl )
   {
      // find the control ... first use the incoming id as an index into
      // the ZMIXCtrl collection array ... if that doesn't yield the correct
      // control, sequentially search through the collection array

#ifdef DEBUG_ALL
      TraceLineI( "OnCommand ID ", uID );
      TraceLineI( "Notification code ", uNotifyCode );
#endif

      ZMapAct *pzma = GetSafeCtrlNode( m_pZSubtask->m_pZMIXCtrl, uID );
      if ( pzma )
      {
         CFrameWnd::OnCommand( uID, hWndCtrl, uNotifyCode );
         pzma->ProcessCommand( uNotifyCode );

         return( TRUE );
      }
   }
#endif

   if ( uID >= zZEIDON_DRIVER_MSG && uID <= zZEIDON_LAST_APP_RESERVED )
   {
      // The driver ignores all messages in this range (to permit handling of special messages by the application)
      return( FALSE );   // we didn't do anything
   }
   else
   {
      UINT uMsg = 0;

      if ( uID == ID_EDIT_COPY )    // ID_EDIT_COPY      0xE122
      {
      // TraceLineS( "(drvr) OnCommand not processed for ", "EDIT_COPY" );
         uMsg = WM_COPY;
      }
      else
      if ( uID == ID_EDIT_CUT )     // ID_EDIT_CUT       0xE123
      {
      // TraceLineS( "(drvr) OnCommand not processed for ", "EDIT_CUT" );
         uMsg = WM_CUT;
      }
      else
      if ( uID == ID_EDIT_PASTE )   // ID_EDIT_PASTE     0xE125
      {
      // TraceLineS( "(drvr) OnCommand not processed for ", "EDIT_PASTE" );
         uMsg = WM_PASTE;
      }
      else
      if ( uID == ID_EDIT_UNDO )    // ID_EDIT_UNDO      0xE12B
      {
      // TraceLineS( "(drvr) OnCommand not processed for ", "EDIT_UNDO" );
         uMsg = WM_UNDO;
      }
      else
      if ( uID == ID_EDIT_CLEAR )   // ID_EDIT_CLEAR     0xE120
      {
      // TraceLineS( "(drvr) OnCommand not processed for ", "EDIT_CLEAR" );
         uMsg = WM_CLEAR;
      }
      else
      if ( uID == ID_EDIT_FIND )    // ID_EDIT_FIND      0xE124
      {
      // TraceLineS( "(drvr) OnCommand not processed for ", "EDIT_FIND" );
         uMsg = 0;
      }
      else
      {
         // AFX_ID_PREVIEW_CLOSE       0xE300  58112
         // AFX_ID_PREVIEW_NUMPAGE     0xE301        // One/Two Page button
         // AFX_ID_PREVIEW_NEXT        0xE302
         // AFX_ID_PREVIEW_PREV        0xE303
         // AFX_ID_PREVIEW_PRINT       0xE304  58116
         // AFX_ID_PREVIEW_ZOOMIN      0xE305
         // AFX_ID_PREVIEW_ZOOMOUT     0xE306  58118
         if ( uID >= 58112 && uID <= 58118 )
         {
//          TraceLineI( "(drvr) OnCommand: ", uID );
            if ( uID == 58116 )  // AFX_ID_PREVIEW_PRINT
            {
               m_pZSubtask->m_pZView->SendMessage( WM_COMMAND, ID_FILE_PRINT_DIRECT );
               if ( m_pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_CLOSE )
               {
                  PostMessage( WM_SYSCOMMAND, SC_CLOSE );
                  return( TRUE );
               }
            }
            else
            if ( uID == 58112 )  // AFX_ID_PREVIEW_CLOSE
            {
//             TraceLineX( "ZDrFrame::OnCommand PREVIEW_CLOSE hWnd", (zLONG) hWndCtrl );
               if ( hWndCtrl )
               {
               // if ( m_pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_CLOSE )
                     PostMessage( WM_SYSCOMMAND, SC_CLOSE );

                  return( TRUE );
               }
#if 1
               else
               {
                  if ( m_pZSubtask && m_pZSubtask->m_pZParent && m_pZSubtask->m_pZParent->m_pZFWnd && mIs_hWnd( m_pZSubtask->m_pZParent->m_pZFWnd->m_hWnd ) )
                  {
//                   TraceLineX( "ZDrFrame::OnCommand ShowWindow: ", (zLONG) m_pZSubtask->m_pZParent->m_pZFWnd->m_hWnd );
                     m_pZSubtask->m_pZParent->m_pZFWnd->PostMessage( zZEIDON_POSTED_ACTIVATE );
                  }
               }
#endif
            }

            return( CFrameWnd::OnCommand( wParam, lParam ) );
         }

         // ID_FILE_PRINT              0xE107
         if ( uID == ID_FILE_PRINT )
            return( CFrameWnd::OnCommand( wParam, lParam ) );

         TraceLineX( "(drvr) OnCommand not processed for ", uID );
         return( FALSE );
      }

      if ( uMsg )
      {
         CWnd::GetFocus( )->SendMessage( uMsg );
         return( TRUE );
      }

      return( FALSE );   // we didn't do anything
   }
}

LRESULT
ZDrFrame::OnBeginDrag( WPARAM wParam, LPARAM lParam )
{
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );

   ZMapAct *pzmaSrc = (ZMapAct *) lParam;
   g_pzmaDragDrop = 0;     // reset
   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && pzmaSrc && pzmaSrc->m_pchDragDrop )
   {
      m_pZSubtask->m_pZTask->m_hcurSave = (HCURSOR) ::SetCursor( m_pZSubtask->m_pZTask->m_hcurDropNot );
      strcpy_s( g_DragDrop.szSrcDragKey, pzmaSrc->m_pchDragDrop + (2 * sizeof( zLONG )) );
      g_DragDrop.pzmaSrc = pzmaSrc;
      g_DragDrop.vSrcSubtask = pzmaSrc->m_pZSubtask->m_vDialog;
      strcpy_s( g_DragDrop.szSrcDialogName, sizeof( g_DragDrop.szSrcDialogName ), *(pzmaSrc->m_pZSubtask->m_pzsDlgTag) );
      strcpy_s( g_DragDrop.szSrcWindowName, sizeof( g_DragDrop.szSrcWindowName ), *(pzmaSrc->m_pZSubtask->m_pzsWndTag) );
      strcpy_s( g_DragDrop.szSrcCtrlName, sizeof( g_DragDrop.szSrcCtrlName ), *(pzmaSrc->m_pzsTag) );
      g_DragDrop.vData = 0;

      if ( pzmaSrc->m_pzsVName )
         strcpy_s( g_DragDrop.szViewName, sizeof( g_DragDrop.szViewName ), *(pzmaSrc->m_pzsVName) );

      if ( pzmaSrc->m_pzsEName )
         strcpy_s( g_DragDrop.szEntityName, sizeof( g_DragDrop.szEntityName ), *(pzmaSrc->m_pzsEName) );

      if ( pzmaSrc->m_pzsAName )
         strcpy_s( g_DragDrop.szAttributeName, sizeof( g_DragDrop.szAttributeName ), *(pzmaSrc->m_pzsAName) );

      if ( wParam & MK_CONTROL )   // we don't yet know about multiple
         g_DragDrop.fCopyMultiple = 0x01;
      else
         g_DragDrop.fCopyMultiple = 0x00;

      g_DragDrop.pzmaTgt = 0;
      g_DragDrop.vTgtSubtask = 0;
      g_DragDrop.szTgtDialogName[ 0 ] = 0;
      g_DragDrop.szTgtWindowName[ 0 ] = 0;
      g_DragDrop.szTgtCtrlName[ 0 ] = 0;
      g_DragDrop.ptDrop = pzmaSrc->m_ptMouseDown;
      pzmaSrc->BeginDrag( &g_DragDrop );
   }

   return( 0 );
}

LRESULT
ZDrFrame::OnQueryDrop( WPARAM wParam, LPARAM lParam )
{
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   ZMapAct *pzmaSrc = (ZMapAct *) lParam;
   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 || pzmaSrc == 0 || pzmaSrc->m_pchDragDrop == 0 )
   {
      return( 0 );
   }

   CPoint ptScreen = pzmaSrc->m_ptMouseUp;

// TraceLineI( "OnQueryDrop Original x-coord: ", ptScreen.x );
// TraceLineI( "                     y-coord: ", ptScreen.y );

   // Convert the mouse coords to screen coords
   pzmaSrc->m_pCtrl->ClientToScreen( &ptScreen );
// TraceLineI( "Screen from control x-coord: ", ptScreen.x );
// TraceLineI( "Screen from control y-coord: ", ptScreen.y );

   CWnd *pWndChild = 0;
   CWnd *pWnd = CWnd::WindowFromPoint( ptScreen );
   if ( pWnd )
   {
      CPoint pt = ptScreen;
      pWnd->ScreenToClient( &pt );
//    TraceLineI( "Client x-coord: ", pt.x );
//    TraceLineI( "Client y-coord: ", pt.y );

      pWndChild = pWnd->ChildWindowFromPoint( pt );
      if ( pWndChild && pWnd->m_hWnd == pWndChild->m_hWnd )
      {
         // Both API's returned the same window ... use this window's parent.
         pWnd = pWndChild->GetParentFrame( );
      }
   }

// TraceLineI( "Window = ", (zLONG) hWnd );
// TraceLineI( "Child  = ", (zLONG) hWndChild );

   // If there is no window here, can't drop.
   if ( pWnd == 0 || pWndChild == 0 )
   {
      ::SetCursor( m_pZSubtask->m_pZTask->m_hcurDropNot );
      g_pzmaDragDrop = 0;
      return( 0 );
   }

   // Try to locate the parent window in the subtask list.
   ZSubtask *pZSubtask = 0;
   zLONG    lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   while ( lItemCnt )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask && g_pSubtaskList[ lItemCnt ].lTID == m_pZSubtask->m_lTID &&
           ((g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZView &&  g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZView->m_hWnd == pWnd->m_hWnd) ||
            g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZFWnd->m_hWnd == pWnd->m_hWnd) )
      {
         pZSubtask = g_pSubtaskList[ lItemCnt ].pZSubtask;
         break;
      }

      lItemCnt--;
   }

   if ( pZSubtask == 0 )  // couldn't locate parent window in subtask list
   {
      ::SetCursor( m_pZSubtask->m_pZTask->m_hcurDropNot );
      g_pzmaDragDrop = 0;
      return( 0 );
   }

// TraceLineI( "Subtask found = ", (zLONG) pZSubtask->m_pZFWnd->m_hWnd );

   // Attempt to locate the child window using the list of Zeidon MapActs
   ZMapAct *pzmaTgt = 0;
   zULONG  k = pZSubtask->m_pZMIXCtrl->GetCount( );
   while ( k )
   {
      pzmaTgt = (ZMapAct *) pZSubtask->m_pZMIXCtrl->GetAt( k );
      if ( pzmaTgt && pzmaTgt->m_pCtrl && pWndChild && pzmaTgt->m_pCtrl->m_hWnd == pWndChild->m_hWnd )
      {
         break;
      }

      k--;
   }

   g_DragDrop.pzmaTgt = pzmaTgt;
   if ( k && pzmaTgt )
   {
//    TraceLineI( "MapAct found = ", (zLONG) pzmaTgt->m_pCtrl->m_hWnd );

      // get coords in terms of client coords of target window
      pzmaTgt->m_pCtrl->ScreenToClient( &ptScreen );
      g_DragDrop.ptDrop = ptScreen;

      // try leaving this out ... it is here only for performance reasons
      // and it looks as though we need to continually query for other
      // reasons (e.g.  to permit a target to scroll if necessary).
   // if ( g_pzmaDragDrop == pzmaTgt )
   // {
   //    return( 0 ); // if it's not a new window, nothing to do
   // }

      // need to check for the existence of a QueryDrop event on this
      // control.  If the QueryDrop event does not exist, the query
      // drop is denied.  Otherwise, the application code specified
      // for the event determines accept/deny.

//    TraceLineS( "ZDrFrame::OnQueryDrop to Tgt: ", *(pzmaTgt->m_pzsTag) );
      if ( pzmaTgt->QueryDrop( &g_DragDrop ) )
      {
         g_pzmaDragDrop = pzmaTgt;

         if ( g_DragDrop.fCopyMultiple & 0x10 )       // MULTIPLE?
         {
            if ( g_DragDrop.fCopyMultiple & 0x01 )    // COPY?
               ::SetCursor( m_pZSubtask->m_pZTask->m_hcurDropOKCM );
            else
               ::SetCursor( m_pZSubtask->m_pZTask->m_hcurDropOKMM );
         }
         else                                         // SINGLE
         {
            if ( g_DragDrop.fCopyMultiple & 0x01 )    // COPY?
               ::SetCursor( m_pZSubtask->m_pZTask->m_hcurDropOKC1 );
            else
               ::SetCursor( m_pZSubtask->m_pZTask->m_hcurDropOKM1 );
         }

         return( 0 );
      }
   }

   g_pzmaDragDrop = 0;
   ::SetCursor( m_pZSubtask->m_pZTask->m_hcurDropNot );

   return( 0 );
}

LRESULT
ZDrFrame::OnDoDrop( WPARAM wParam, LPARAM lParam )
{
// TraceLineS( "Zeidon DoDrop", "" );
   ::SetCursor( m_pZSubtask->m_pZTask->m_hcurSave );

   g_pzmaDragDrop = 0;    // this forces OnQueryDrop to do full check
   OnQueryDrop( wParam, lParam );    // sets g_pzmaDragDrop if it will accept the drop
   if ( g_pzmaDragDrop )  // this guy will accept the drop
   {
      g_DragDrop.vTgtSubtask = g_pzmaDragDrop->m_pZSubtask->m_vDialog;
      strcpy_s( g_DragDrop.szTgtDialogName, sizeof( g_DragDrop.szTgtDialogName ), *(g_pzmaDragDrop->m_pZSubtask->m_pzsDlgTag) );
      strcpy_s( g_DragDrop.szTgtWindowName, sizeof( g_DragDrop.szTgtWindowName ), *(g_pzmaDragDrop->m_pZSubtask->m_pzsWndTag) );
      strcpy_s( g_DragDrop.szTgtCtrlName, sizeof( g_DragDrop.szTgtCtrlName ), *(g_pzmaDragDrop->m_pzsTag) );

      // Need to check for the existence of a DoDrop event on this control.  If the DoDrop event does not exist,
      // the do drop is denied.  Otherwise, the application code specified for the event is called to process the drop.
      g_pzmaDragDrop->DoDrop( &g_DragDrop );
      ZMapAct *pzmaSrc = (ZMapAct *) lParam;
      pzmaSrc->Drop( &g_DragDrop );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// afx_msg void OnActivate( UINT nState, CWnd *pWndOther, BOOL bMinimized );
//
// Parameters
//
// nState Specifies whether the CWnd is being activated or
// deactivated.  It can be one of the following values:
//
// WA_INACTIVE The window is being deactivated.
//
// WA_ACTIVE The window is being activated through some method other
// than a mouse click (for example, by use of the keyboard interface to
// select the window).
//
// WA_CLICKACTIVE The window is being activated by a mouse click.
//
// pWndOther Pointer to the CWnd being activated or deactivated. The
// pointer can be NULL, and it may be temporary.
//
// bMinimized Specifies the minimized state of the CWnd being
// activated or deactivated.  A value of TRUE indicates the window is
// minimized.
//
// If TRUE, the CWnd is being activated; otherwise deactivated.
//
// Remarks
//
// The framework calls this member function when a CWnd object is being
// activated or deactivated. First, the main window being deactivated
// has OnActivate called, and then the main window being activated has
// OnActivate called.
//
// If the CWnd object is activated with a mouse click, it will also
// receive an OnMouseActivate member function call.
//
// Note This member function is called by the framework to allow your
// application to handle a Windows message.  The parameters passed to
// your function reflect the parameters received by the framework when
// the message was received. If you call the base-class implementation
// of this function, that implementation will use the parameters
// originally passed with the message and not the parameters you supply
// to the function.

void
ZDrFrame::OnActivate( UINT fActive, CWnd *pWndOther, BOOL bIsIconified )
{
// WM_ACTIVATE fActive state values */
// #define WA_INACTIVE     0 - window is being deactivated
// #define WA_ACTIVE       1 - window is being activated (not by mouse-click)
// #define WA_CLICKACTIVE  2 - window is being activated by mouse-click

#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame OnActivate fActive: %d  %s.%s  hWnd: 0x%08x   IdNbr: %d",
              fActive, *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag),
              m_hWnd, m_pZSubtask->m_ulSubtaskIdNbr );
#endif

   zULONG ulSubtaskState = ZSubtask::SetSubtaskState( m_pZSubtask, zSUBTASK_STATE_ON_RETURN, 0 );

   if ( fActive && pWndOther == this && (ulSubtaskState & (zSUBTASK_STATE_IN_CONSTRUCTION | zSUBTASK_STATE_ON_RETURN)) == 0 )
   {
      return;
   }

   if ( (m_ulFrameFlags & zSUBTASK_STATE_MARK_FOR_DELETE) || (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) ||
        (ulSubtaskState & (zSUBTASK_STATE_POSTBUILD_PRECODE | zSUBTASK_STATE_POSTBUILD_POSTCODE | zSUBTASK_STATE_WELL_FORMED)) == 0 )
   {
      return;
   }

   CFrameWnd::OnActivate( fActive, pWndOther, bIsIconified );
   if ( fActive != WA_INACTIVE && bIsIconified == false && (m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_PROCESSING_ACTIVATE) == 0 && m_pZSubtask->m_nInvisible >= 0 )
   {
      m_pZSubtask->m_ulSubtaskFlags |= zSUBTASK_PROCESSING_ACTIVATE; // prohibit recursion
      if ( m_pZSubtask->m_pzmaWithFocus && m_pZSubtask->m_pzmaWithFocus->m_pCtrl && mIs_hWnd( m_pZSubtask->m_pzmaWithFocus->m_pCtrl->m_hWnd ) )
      {
         m_pZSubtask->m_pzmaWithFocus->m_pCtrl->SetFocus( );   // necessary???
      }

      if ( m_pZSubtask->m_pZMIXCtrl && (m_pZSubtask->m_ulWndOper & zWND_EVENT_NOTIFYCHILD_ACTIVATE) == zWND_EVENT_NOTIFYCHILD_ACTIVATE )
      {
         m_pZSubtask->m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnSendCtrlsParentActivated, 0 );
      }

      if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_ON_ACTIVATE) == zWND_EVENT_ON_ACTIVATE &&
           SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog, szlWndEvent, szlType, zWND_EVENT_ON_ACTIVATE, 0 ) > zCURSOR_UNCHANGED )
      {
         ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0, zACTION_DIALOG_CODE_ONLY, TRUE, "OnActivate" );   // what happens on error???
      }

      m_pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_PROCESSING_ACTIVATE;
   }
}

// ??? do we need m_pzmaWithFocus anymore???
void
ZDrFrame::OnKillFocus( CWnd *pWndGotFocus )
{
   zULONG ulSubtaskState;

#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame OnKillFocus  %s.%s  hWnd: 0x%08x   GotFocus: 0x%08x",
              *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag),
              m_hWnd, pWndGotFocus ? pWndGotFocus->m_hWnd : 0 );
#endif

   if ( ((ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask )) & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
   {
      Default( );
   }
}

// we need m_pzmaWithFocus for Activate
void
ZDrFrame::OnSetFocus( CWnd *pWndLostFocus )
{
   zULONG ulSubtaskState = 0;

#ifdef DEBUG_ALL
   TraceLine( "ZDrFrame OnSetFocus  %s.%s  hWnd: 0x%08x   LostFocus: 0x%08x",
              *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag), m_hWnd, pWndLostFocus ? pWndLostFocus->m_hWnd : 0 );
#endif

#if 0
   CWnd *pParent = pWndLostFocus;
   while ( pParent )
   {
      if ( pParent == this )
         break;

      pParent = pWndLostFocus->GetParent( );
   }

   if ( pParent != this )
      Default( );
#endif

   if ( (m_ulFrameFlags & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
   {
      if ( m_pZSubtask && m_pZSubtask->m_pZMIXCtrl && ((ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask )) & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
      {
         if ( m_pZSubtask->m_pzmaWithFocus == 0 )
         {
            m_pZSubtask->m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnSetFirstLastFocus, 0 );
//          if ( m_pZSubtask->m_pzmaWithFocus == 0 )
//             CFrameWnd::OnSetFocus( hWndLostFocus );
         }
         else
         {
//          TraceLineS( "ZDrFrame m_pzmaWithFocus ", *(m_pZSubtask->m_pzmaWithFocus->m_pzsTag) );
            if ( m_pZSubtask->m_pzmaWithFocus->m_pCtrl && mIs_hWnd( m_pZSubtask->m_pzmaWithFocus->m_pCtrl->m_hWnd ) )
            {
               m_pZSubtask->m_pzmaWithFocus->m_pCtrl->SetFocus( );
            }
         }
      }
   }
}

HANDLE
ZDrFrame::OnQueryDragIcon( )
{
   return( (HANDLE) GetIconForWindow( m_pZSubtask ) );
}

// ZDrFrame - paint window ... for Iconic painting only
void
ZDrFrame::OnPaint( )
{
#ifndef __WIN32__
   if ( IsIconic( ) )
   {
      PAINTSTRUCT ps;
      HICON hIcon = GetIconForWindow( m_pZSubtask );

      ::BeginPaint( m_hWnd, &ps );
      SetClassWord( GCW_HICON, (zSETCLASSWORD) hIcon );
      SendMessage( WM_ICONERASEBKGND, (WORD) ps.hdc, 0 );
      SetClassWord( GCW_HICON, 0 );
      DrawIcon( ps.hdc, 0, 0, hIcon );
      ::EndPaint( m_hWnd, &ps );
   }
   else
#endif
      CFrameWnd::OnPaint( );
}

void
ZDrFrame::OnSize( UINT uSizeType, int cx, int cy )
{
#if 0 // def DEBUG_ALL
   if ( m_pZSubtask && m_pZSubtask->m_pzsWndTag )
      TraceLineS( "ZDrFrame::OnSize tag ", *(m_pZSubtask->m_pzsWndTag) );

   TraceLine( "ZDrFrame::OnSize  cx: %d  cy: %d", cx, cy );
#endif

   CFrameWnd::OnSize( uSizeType, cx, cy );

   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) || (ulSubtaskState & 0x000000FF) < zSUBTASK_STATE_POSTBUILD_PRECODE )
   {
      return;
   }

   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_ON_SIZE) == zWND_EVENT_ON_SIZE &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog, szlWndEvent, szlType, zWND_EVENT_ON_SIZE, 0 ) > zCURSOR_UNCHANGED )
   {
      if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0, zACTION_DIALOG_CODE_ONLY, TRUE, "OnSize" ) > 0 )
         return;
   }

#if 0    // moved to ZDrView class so that GetClientRect is precise.

   // Inform all controls that a WM_SIZE has occurred.
   if ( m_pZSubtask->m_pZMIXCtrl )
   {
      CRect rect;
      GetClientRect( rect );
      TraceRect( "ZDrFrame::OnSize client:", rect );

      CSize size( cx, cy );
      TraceSize( "ZDrFrame::OnSize:", size );

      m_pZSubtask->SendParentResize( 0, &size );
   }
#endif
}

void
ZDrFrame::OnMove( int x, int y )
{
#if 0 // def DEBUG_ALL
   if ( m_pZSubtask && m_pZSubtask->m_pzsWndTag )
      TraceLineS( "ZDrFrame::OnMove tag ", *(m_pZSubtask->m_pzsWndTag) );

   TraceLineI( "ZDrFrame::OnMove   x ", x );
   TraceLineI( "===============>   y ", y );
#endif
   CFrameWnd::OnMove( x, y );

   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   if ( ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE )
      return;

   if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_ON_MOVE) == zWND_EVENT_ON_MOVE &&
        SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog, szlWndEvent, szlType, zWND_EVENT_ON_MOVE, 0 ) > zCURSOR_UNCHANGED )
   {
      if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0, zACTION_DIALOG_CODE_ONLY, TRUE, "OnMove" ) > 0 )
         return;
   }

   // Inform all controls that a WM_MOVE has occurred.
   if ( m_pZSubtask->m_pZMIXCtrl )
   {
      CPoint pt( x, y );
      m_pZSubtask->m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnSendCtrlsParentMove, 0, 0, (LPARAM) &pt );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// The WM_ERASEBKGND message is sent when the window background needs to be
// erased (for example, when a window is resized).  It is sent to prepare an
// invalidated region for painting.
//
// The DefWindowProc function erases the background by using the class
// background brush specified by the hbrbackground member of the WNDCLASS
// structure.
//
// If the hbrbackground member is NULL, the application should process the
// WM_ERASEBKGND message and erase the background color.  When processing
// the WM_ERASEBKGND message, the application must align the origin of the
// intended brush with the window coordinates by first calling the
// UnrealizeObject function for the brush and then selecting the brush.  An
// application should return nonzero in response to WM_ERASEBKGND if it
// processes the message and erases the background; this indicates that no
// further erasing is required.  If the app returns zero the window will
// remain marked as needing to be erased (typically, this means that the
// fErase member of the PAINTSTRUCT structure will be TRUE).  Windows
// computes the background by using the MM_TEXT mapping mode.  If the device
// context is using any other mapping mode, the area erased may not be
// within the visible part of the client area.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
BOOL
ZDrFrame::OnEraseBkgnd( CDC *pDC )
{
   return( TRUE );   // the view will erase/paint itself
// return( CFrameWnd::OnEraseBkgnd( pDC ) );
}

void
ZDrFrame::OnSysColorChange( )
{
   CFrameWnd::OnSysColorChange( );
}

HBRUSH
ZDrFrame::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnCtlColor", "" );
#endif
   return( CFrameWnd::OnCtlColor( pDC, pWnd, nCtlColor ) );
}

void
ZDrFrame::OnMenuSelect( UINT uMenuItemId, UINT uFlags, HMENU hMenu )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnMenuSelect", "" );
#endif
   ZSubtask *pZSubtask = m_pZSubtask->m_pZSubtaskGlobal ? m_pZSubtask->m_pZSubtaskGlobal : m_pZSubtask;
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && pZSubtask->m_pDIL && pZSubtask->m_nAutoDIL )
   {
      ZOption *pOption = 0;
      zPCHAR  pch;

      // This processing assumes that the first DIL message is "".
      SetCursorFirstEntity( pZSubtask->m_vDialog, szlDIL, 0 );

      if ( uFlags == (zUSHORT) -1 || uFlags & MF_POPUP || uFlags & MF_SEPARATOR || uFlags & MF_SYSMENU )
      {
         // do nothing
      // TraceLineS( "ZDrFrame::OnMenuSelect ", "Do Nothing" );
         GetAddrForAttribute( (zPCHAR *) &pch, pZSubtask->m_vDialog, szlDIL, szlD_Msg );
      }
      else
      {
      // TraceLineS( "ZDrFrame::OnMenuSelect ", "Do Something" );
         if ( m_pZSubtask->m_pZBinPopup && uMenuItemId > zPOPUP_MENU_FIRST )
            pOption = (ZOption *) m_pZSubtask->m_pZBinPopup->GetAt( uMenuItemId - zPOPUP_MENU_FIRST );
         else
         if ( m_pZSubtask->m_pZBinMenu )
            pOption = (ZOption *) m_pZSubtask->m_pZBinMenu->GetAt( uMenuItemId );

         if ( pOption )
            SetCursorRelativeEntity( m_pZSubtask->m_vDialog, szlDIL, pOption->m_nDIL, 0 );

         GetAddrForAttribute( (zPCHAR *) &pch, m_pZSubtask->m_vDialog, szlDIL, szlD_Msg );
      }

      pZSubtask->SetStatusBarText( pch );    // message to default DIL item
   }
   else
   {
   // TraceLineS( "ZDrFrame::OnMenuSelect ", "Do ????" );
   }

// CFrameWnd::OnMenuSelect( uMenuItemId, uFlags, hMenu );
}

// ZDrFrame - menu pre-dropdown
void
ZDrFrame::OnInitMenu( CMenu *pMenu )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZDrFrame::OnInitMenu count: ", pMenu->GetMenuItemCount( ) );
#endif
   ZSubtask *pZSubtask = m_pZSubtask;
   ZOption *pZOption;
   zBOOL   bChecked;
   zLONG   lOper = 0;
   zUSHORT k;
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );

   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 &&
        pZSubtask->m_pZBinPopup && pMenu == pZSubtask->m_pTrackMenu )
   {
      if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_POPUP_PRE_DROPDOWN) == zWND_EVENT_POPUP_PRE_DROPDOWN )
      {
         lOper = zWND_EVENT_POPUP_PRE_DROPDOWN;
      }

      for ( k = 1; k <= pZSubtask->m_pZBinPopup->GetCount( ); k++ )
      {
         pZOption = (ZOption *) m_pZSubtask->m_pZBinPopup->GetAt( k );
         bChecked = pZOption->SetCheckForOption( );
         pZSubtask->m_pTrackMenu->CheckMenuItem( pZOption->m_uOptionID, MF_BYCOMMAND | (bChecked ? MF_CHECKED : MF_UNCHECKED) );
      }
   }
   else
   {
      if ( m_pZSubtask->m_pZSubtaskGlobal )
         pZSubtask = m_pZSubtask->m_pZSubtaskGlobal;

      ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
      if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && pZSubtask->m_pZBinMenu && pMenu == pZSubtask->m_pMenu )
      {
         pZSubtask->AutoGrayActions( );
         if ( (pZSubtask->m_ulWndOper & zWND_EVENT_MENU_PRE_DROPDOWN) == zWND_EVENT_MENU_PRE_DROPDOWN )
         {
            lOper = zWND_EVENT_MENU_PRE_DROPDOWN;
         }

         for ( k = 1; k <= pZSubtask->m_pZBinMenu->GetCount( ); k++ )
         {
            pZOption = (ZOption *) pZSubtask->m_pZBinMenu->GetAt( k );
            bChecked = pZOption->SetCheckForOption( );
            pZSubtask->m_pMenu->CheckMenuItem( pZOption->m_uOptionID, MF_BYCOMMAND | (bChecked ? MF_CHECKED : MF_UNCHECKED) );
         // TraceLineS( "ZDrFrame::OnInitMenu EnableMenuItem", "" );
            pZSubtask->m_pMenu->EnableMenuItem( pZOption->m_uOptionID, MF_BYCOMMAND | pZOption->m_bEnabled ? MF_ENABLED : MF_GRAYED );
         }
      }
   }

   if ( lOper && SetCursorFirstEntityByInteger( pZSubtask->m_vDialog, szlWndEvent, szlType, lOper, 0 ) > zCURSOR_UNCHANGED )
   {
      ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0, 0, TRUE, "PreDropDown" );
   }

// This default handler disables menu items ... we want control of this ...
// so we do not call the default handler
// CFrameWnd::OnInitMenu( pMenu );
}

void
ZDrFrame::OnInitMenuPopup( CMenu *pPopupMenu, UINT uIndex, BOOL bSysMenu )
{
   if ( bSysMenu || pPopupMenu == 0 )
   {
   // CFrameWnd::OnInitMenuPopup( pPopupMenu, uIndex, bSysMenu );
   // Default( );
      if ( bSysMenu )
      {
         zLONG lStyle = GetStyle( );
         if ( (lStyle & WS_THICKFRAME) == WS_THICKFRAME )
            pPopupMenu->EnableMenuItem( SC_SIZE, MF_BYCOMMAND | MF_ENABLED );
         else
            pPopupMenu->EnableMenuItem( SC_SIZE, MF_BYCOMMAND | MF_GRAYED );
#if 0
         lStyle = GetExStyle( );
         if ( (lStyle & WS_EX_DLGMODALFRAME) == WS_EX_DLGMODALFRAME )
            pPopupMenu->EnableMenuItem( SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED );
         else
            pPopupMenu->EnableMenuItem( SC_MAXIMIZE, MF_BYCOMMAND | MF_ENABLED );

         pPopupMenu->EnableMenuItem( SC_MINIMIZE, MF_BYCOMMAND | MF_ENABLED );
         pPopupMenu->EnableMenuItem( SC_RESTORE, MF_BYCOMMAND | MF_ENABLED );
#else
         // Enable/Disable System Menu items appropriately.
         int nCnt = pPopupMenu->GetMenuItemCount( );
         int k;
         UINT uID;
         int nState;
         if ( IsIconic( ) )
            nState = -1;
         else
         if ( IsZoomed( ) )
            nState = 1;
         else
            nState = 0;

//       TraceLineI( "OnInitMenuPopup count: ", nCnt );
//       TraceLineI( "OnInitMenuPopup SC_SIZE: ", SC_SIZE );
//       TraceLineI( "OnInitMenuPopup SC_MOVE: ", SC_MOVE );
//       TraceLineI( "OnInitMenuPopup SC_RESTORE: ", SC_RESTORE );
//       TraceLineI( "OnInitMenuPopup SC_MINIMIZE: ", SC_MINIMIZE );
//       TraceLineI( "OnInitMenuPopup SC_MAXIMIZE: ", SC_MAXIMIZE );
//       TraceLineI( "OnInitMenuPopup SC_CLOSE: ", SC_CLOSE );
         for ( k = 0; k < nCnt; k++ )
         {
            uID = pPopupMenu->GetMenuItemID( k );
         // TraceLineI( "ZDrFrame::OnInitMenuPopup uID: ", uID );
            if ( uID == SC_RESTORE )
            {
               if ( nState == 0 )
                  pPopupMenu->EnableMenuItem( k, MF_BYPOSITION | MF_DISABLED | MF_GRAYED );
               else
                  pPopupMenu->EnableMenuItem( k, MF_BYPOSITION | MF_ENABLED );
            }
            else
            if ( uID == SC_MINIMIZE )
            {
               if ( nState < 0 )
                  pPopupMenu->EnableMenuItem( k, MF_BYPOSITION | MF_DISABLED | MF_GRAYED );
               else
                  pPopupMenu->EnableMenuItem( k, MF_BYPOSITION | MF_ENABLED );
            }
            else
            if ( uID == SC_MAXIMIZE )
            {
               if ( nState > 0 )
                  pPopupMenu->EnableMenuItem( k, MF_BYPOSITION | MF_DISABLED | MF_GRAYED );
               else
                  pPopupMenu->EnableMenuItem( k, MF_BYPOSITION | MF_ENABLED );
            }
         }

#if 0
         if ( IsIconic( ) )
         {
         // TraceLineS( "ZDrFrame::OnInitMenuPopup: ", "ICONIC" );
            pPopupMenu->EnableMenuItem( SC_MAXIMIZE, MF_BYCOMMAND | MF_ENABLED );
            pPopupMenu->EnableMenuItem( SC_MINIMIZE, MF_BYCOMMAND | MF_GRAYED );
            pPopupMenu->EnableMenuItem( SC_RESTORE, MF_BYCOMMAND | MF_ENABLED );
         }
         else
         if ( IsZoomed( ) )
         {
         // TraceLineS( "ZDrFrame::OnInitMenuPopup: ", "ZOOMED" );
            pPopupMenu->EnableMenuItem( SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED );
            pPopupMenu->EnableMenuItem( SC_MINIMIZE, MF_BYCOMMAND | MF_ENABLED );
            pPopupMenu->EnableMenuItem( SC_RESTORE, MF_BYCOMMAND | MF_ENABLED );
         }
         else
         {
         // TraceLineS( "ZDrFrame::OnInitMenuPopup: ", "NORMAL" );
            pPopupMenu->EnableMenuItem( SC_MAXIMIZE, MF_BYCOMMAND | MF_ENABLED );
            pPopupMenu->EnableMenuItem( SC_MINIMIZE, MF_BYCOMMAND | MF_ENABLED );
            pPopupMenu->EnableMenuItem( SC_RESTORE, MF_BYCOMMAND | MF_GRAYED );
         }
#endif

         DrawMenuBar( );
#endif
      }

      return;
   }

// This default handler disables menu items ... we want control of this ...
// so we do not call the default handler
// CFrameWnd::OnInitMenuPopup( pPopupMenu, uIndex, bSysMenu );
}

/////////////////////////////////////////////////////////////////////////////
//
// This OnInitMenuPopup message is not the result of mis-translating or
// mis-dispatching.  Owl, in FrameWindow::IdleAction simulates
// WM_INITMENUPOPUP (see the source for FRAMEWlN.CPP, IdleAction where
// there is ....HandleMessage( WM_INITMENUPOPUP,...).  This is done to
// support child windows that expect to be able to modify the menu
// (such as a child edit control that reflects appropriate EDIT and PASTE
// selections) so the means of resolving this is direct ... just override
// the Decorated frame class's IdleAction.
//
/////////////////////////////////////////////////////////////////////////////
BOOL
ZDrFrame::IdleAction( long lIdleCount )
{
// don't invoke the base class...
// int result = CFrameWnd::IdleAction( lIdleCount );
// return( result );

   return( FALSE );
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
// hwndPalChg =  handle of window that changed palette
//
// The WM_PALETTECHANGED message is sent to all top-level and overlapped
// windows after the window with the input focus has realized its logical
// palette, thereby changing the system palette. This message allows a
// window without the input focus that uses a color palette to realize its
// logical palette and update its client area.
//
// An application should return zero if it processes this message.
//
// This message is sent to all top-level and overlapped windows, including
// the one that changed the system palette and caused this message to be
// sent. If any child windows use a color palette, this message must be
// passed on to them.
//
// To avoid an infinite loop, a window that receives this message should
// not realize its palette unless it determines that hWndPalChg does not
// contain its own window handle.
//
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void
ZDrFrame::OnPaletteChanged( CWnd *pWndPalChg )
{
   CFrameWnd::OnPaletteChanged( pWndPalChg );

   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtask );
   if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
   {
      if ( m_pZSubtask->m_pZMIXCtrl && (m_pZSubtask->m_ulWndOper & zWND_EVENT_NOTIFYCHILD_PALETTE) == zWND_EVENT_NOTIFYCHILD_PALETTE )
      {
         m_pZSubtask->m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnSendCtrlsPaletteChanged, 0, 0, (LPARAM) pWndPalChg );
      }

      if ( (m_pZSubtask->m_ulWndOper & zWND_EVENT_ON_PALETTE_CHANGE) == zWND_EVENT_ON_PALETTE_CHANGE &&
           SetCursorFirstEntityByInteger( m_pZSubtask->m_vDialog, szlWndEvent, szlType, zWND_EVENT_ON_PALETTE_CHANGE, 0 ) > zCURSOR_UNCHANGED )
      {
         if ( ProcessActionLoop( m_pZSubtask, szlWndAct, 0, 0, zACTION_DIALOG_CODE_ONLY, TRUE, "OnPalette" ) > 0 )
         {
            return;
         }
      }
   }
}

LRESULT
ZDrFrame::OnSetMessageString( WPARAM wParam, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineX( "ZDrFrame::OnSetMessageString wParam: ", wParam );
// TraceLineX( "          OnSetMessageString lParam: ", lParam );
#endif
// if ( m_pZSubtask->m_pDIL )
//    m_pZSubtask->m_pDIL->SetWindowText( "OnSetMessageString" );
//
   m_pZSubtask->m_lMsgCmdId = wParam;

// return( CFrameWnd::OnSetMessageString( wParam, lParam ) );
   return( 0 );
}

void
ZDrFrame::OnUpdateControlBarMenu( CCmdUI *pCmdUI )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnUpdateControlBarMenu", "" );
#endif
   ASSERT( ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR );
   ASSERT( ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR );

   CControlBar *pBar = GetControlBar( pCmdUI->m_nID );
   if ( pBar )
   {
      pCmdUI->SetCheck( (pBar->GetStyle( ) & WS_VISIBLE) != 0 );
      return;
   }

   pCmdUI->ContinueRouting( );
}

BOOL
ZDrFrame::OnBarCheck( UINT nID )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnBarCheck", "" );
#endif
   ASSERT( ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR );
   ASSERT( ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR );

   CControlBar *pBar = GetControlBar( nID );
   if ( pBar )
   {
      ShowControlBar( pBar, (pBar->GetStyle( ) & WS_VISIBLE) == 0, FALSE );
      return( TRUE );
   }

   return( FALSE );
}

BOOL
ZDrFrame::OnToolbarDropDown( UINT uIdNbr, NMHDR *pNMHDR, LRESULT *plResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnToolbarDropDown", "" );
#endif
   LPNMTOOLBAR pNMTB = (LPNMTOOLBAR) pNMHDR;
   UINT uID = pNMTB->iItem;

   if ( uID )
   {
      HWND hWnd = m_pZSubtask->LocateBarTip( uID );
      if ( hWnd )
      {
         ::SendMessage( hWnd, TB_PRESSBUTTON, uID, (LPARAM) MAKELONG( TRUE, 0 ) );
      // TraceLineX( "ZDrFrame::OnToolTipText", (zLONG)(HWND) pTTTW->lParam );
         ::SendMessage( hWnd, zZEIDON_TOOLBAR_DROPDOWN, uID, 0 );
         Sleep( 150 );
         ::SendMessage( hWnd, TB_PRESSBUTTON, uID, (LPARAM) MAKELONG( FALSE, 0 ) );
      }
   }

#if 0

   CWindowRect rectCaller( m_btnListCtrl );
   CPopup *pPopup = (CPopup *) RUNTIME_CLASS (CPopup)->CreateObject( );

   VERIFY( pPopup->Display( &m_lstPopup, &m_btnListCtrl, rectCaller, &m_lstPopup.m_xPopupCtrlEvent ) );

////////////////////////////////////////////////////////////////////////////
   CRect rectCaller;

   // Get rect of toolbar item
   m_wndToolBar.GetItemRect( m_wndToolBar.CommandToIndex( ID_EDIT_MYUNDO ), rectCaller );
   m_wndToolBar.ClientToScreen( rectCaller );
   rcCaller.bottom += 2;

   CPopup *pPopup = (CPopup *) RUNTIME_CLASS (CPopup)->CreateObject( );

   //¨Popup the TreeCtrl
   VERIFY( pPopup->Display( &m_treePopup, this, rectCaller, &m_treePopup.m_xPopupCtrlEvent ) );

////////////////////////////////////////////////////////////////////////////

   // Load and display popup menu.
   CMenu menu;
   menu.LoadMenu( nID );
   CMenu *pPopup = menu.GetSubMenu( 0 );
   ASSERT( pPopup );

   CRect rect;
   pWnd->SendMessage( TB_GETRECT, pnmtb->iItem, (LPARAM) &rect );
   pWnd->ClientToScreen( &rect );

   pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rect.left, rect.bottom, this, &rect );

////////////////////////////////////////////////////////////////////////////

   m_wndToolBar.SendMessage( TB_PRESSBUTTON, lpnmtb->iItem, (LPARAM) MAKELONG( TRUE, 0 ) );

   switch ( lpnmtb->iItem )
   {
      case ID_EDIT_MYUNDO:
         OnEditUndo();
         break;

      case ID_EDIT_MYREDO:
         OnEditRedo();
         break;
   }

   Sleep( 150 );

   m_wndToolBar.SendMessage( TB_PRESSBUTTON, lpnmtb->iItem, (LPARAM) MAKELONG( FALSE, 0 ) );
#endif
   *plResult = TBDDRET_DEFAULT;
   return( TRUE );    // message was handled
}

BOOL
ZDrFrame::OnToolTipText( UINT uIdNbr, NMHDR *pNMHDR, LRESULT *plResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnToolTipText", "" );
#endif
   ASSERT( pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW );

   // Allow top level routing frame to handle the message.
// if ( GetRoutingFrame( ) ) DKS 1999/07/01 ... don't know why this was here,
//    return( FALSE );       but it stops the painter from getting tooltips.

   TCHAR szTipText[ zZEIDON_TOOLTIP_TEXTLTH ];
   UINT uID = pNMHDR->idFrom;
// TraceLineX( "ZDrFrame::OnToolTipText idFrom: ", nID );

   // Need to handle both ANSI and UNICODE versions of the message.
   TOOLTIPTEXTA *pTTTA = (TOOLTIPTEXTA *) pNMHDR;
   TOOLTIPTEXTW *pTTTW = (TOOLTIPTEXTW *) pNMHDR;
   if ( pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) || pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND) )
   {
      // idFrom is actually the HWND of the tool.
      uID = ::GetDlgCtrlID( (HWND) uID );
   }

   szTipText[ 0 ] = 0;
// TraceLineX( "ZDrFrame::OnToolTipText Sending nID: ", nID );

   HWND hWnd;

   // uID will be zero on a separator.
#ifndef _UNICODE
   if ( uID && (pNMHDR->code == TTN_NEEDTEXTA) && mIs_hWnd( (HWND) pTTTA->lParam ) )
      hWnd = (HWND) pTTTW->lParam;
#else
   if ( uID && (pNMHDR->code == TTN_NEEDTEXTW) && mIs_hWnd( (HWND) pTTTW->lParam ) )
      hWnd = (HWND) pTTTW->lParam;
#endif
   else
      hWnd = m_pZSubtask->LocateBarTip( uID );

   if ( hWnd )
   {
//    TraceLineX( "ZDrFrame::OnToolTipText", (zLONG) (HWND) pTTTW->lParam );
      ::SendMessage( (HWND) hWnd, zZEIDON_GET_TOOLTIP_TEXT, uID, (LPARAM) szTipText );
   }

#ifndef _UNICODE
   if ( pNMHDR->code == TTN_NEEDTEXTA )
      lstrcpyn( pTTTA->szText, szTipText, mElementCnt( pTTTA->szText ) );
   else
      _mbstowcsz( pTTTW->szText, szTipText, mElementCnt( pTTTW->szText ) );
#else
   if ( pNMHDR->code == TTN_NEEDTEXTA )
      _wcstombsz( pTTTA->szText, szTipText, mElementCnt( pTTTA->szText ) );
   else
      lstrcpyn( pTTTW->szText, szTipText, mElementCnt( pTTTW->szText ) );
#endif

   *plResult = 0;

   // Bring the tooltip window above other popup windows.
   ::SetWindowPos( pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER );

   return( TRUE );    // message was handled
}

void
ZDrFrame::OnUpdateKeyIndicator( CCmdUI *pCmdUI )
{
   TraceLineS( "ZDrFrame::OnUpdateKeyIndicator", "" );
   UINT nVK;
   UINT flag = 0x0001;

   switch ( pCmdUI->m_nID )
   {
      case ID_INDICATOR_CAPS:
         nVK = VK_CAPITAL;
         break;

      case ID_INDICATOR_NUM:
         nVK = VK_NUMLOCK;
         break;

      case ID_INDICATOR_SCRL:
         nVK = VK_SCROLL;
         break;

      case ID_INDICATOR_KANA:
         nVK = VK_KANA;

         // WINBUG: Special case for Windows 3.x.  The wrong bit was toggled in those systems so this must be special cased.
         // This is fixed on systems whose version is 4.x or greater.

         // Need to get Windows version for this
         // if ( !afxData.bWin4 )
            flag = 0x8000;

         break;

      default:
         TraceLineI( "Warning: OnUpdateKeyIndicator - unknown indicator: ", pCmdUI->m_nID );
         pCmdUI->ContinueRouting( );
         return; // not for us
   }

   pCmdUI->Enable( ::GetKeyState( nVK ) & flag );

   // enable static text based on toggled key state
   ASSERT( pCmdUI->m_bEnableChanged );
}

void
ZDrFrame::OnUpdateContextHelp( CCmdUI *pCmdUI )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrFrame::OnUpdateContextHelp", "" );
#endif
   if ( AfxGetMainWnd( ) == this )
      pCmdUI->SetCheck( !!m_bHelpMode );
   else
      pCmdUI->ContinueRouting( );
}

#if 0    // by handling OnUpdateCmdUI properly in the ControlBar, the command handler is not needed ...
         // except for standard commands like CUT, PASTE, etc?
void
ZDrFrame::OnMenuSelection( )
{
   // This sample doesn't do anything with button presses but a command handler needs to be present for MFC to enable the buttons
// TraceLineS( "ZDrFrame::OnMenuSelection", "" );
}

void
ZDrFrame::OnEditCut( )
{
   TraceLineS( "ZDrFrame::OnEditCut", "" );
}

void
ZDrFrame::OnEditCopy( )
{
   TraceLineS( "ZDrFrame::OnEditCopy", "" );
}

void
ZDrFrame::OnEditPaste( )
{
   TraceLineS( "ZDrFrame::OnEditPaste", "" );
}

void
ZDrFrame::OnEditUndo( )
{
   TraceLineS( "ZDrFrame::OnEditUndo", "" );
}

void
ZDrFrame::OnEditRedo( )
{
   TraceLineS( "ZDrFrame::OnEditRedo", "" );
}

#endif

void
ZDrFrame::OnUpdateRestore( CCmdUI *pCmdUI )
{
   if ( IsIconic( ) || IsZoomed( ) )
      pCmdUI->Enable( TRUE );
   else
      pCmdUI->Enable( FALSE );
}

void
ZDrFrame::OnUpdateMinimize( CCmdUI *pCmdUI )
{
   if ( IsIconic( ) )
      pCmdUI->Enable( FALSE );
   else
      pCmdUI->Enable( TRUE );
}

void
ZDrFrame::OnUpdateMaximize( CCmdUI *pCmdUI )
{
   if ( IsZoomed( ) )
      pCmdUI->Enable( FALSE );
   else
      pCmdUI->Enable( TRUE );
}

BOOL
ZDrFrame::PreTranslateMessage( MSG *pMsg )
{
#if 0
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN || pMsg->message == WM_EXITSIZEMOVE )
   {
      if ( pMsg->wParam == VK_ESCAPE )
      {
         TraceLineS( "ZDrFrame::PreTranslateMessage: ", "VK_ESCAPE" );
      }
   }
#endif

#if 0   // moved to ZDrView::PreTranslateMessage  // 01/29/03 dks
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN )
   {
   // TraceLine( "ZDrFrame::PreTranslateMessage %d, %d", pMsg->wParam, pMsg->lParam );
      if ( fnIsAccelerator( m_pZSubtask, pMsg ) )
      {
      // TraceLineI( "ZDrFrame::PreTranslateMessage TRUE ", pMsg->wParam );
         return( TRUE );
      }

   // if ( pMsg->wParam == 85 )
   //    TraceLineI( "ZDrFrame::PreTranslateMessage FALSE ", pMsg->wParam );
   }
// else
// if ( pMsg->message == WM_LBUTTONUP )
//    TraceLineI( "ZDrFrame::PreTranslateMessage ", pMsg->wParam );
#endif

   if ( m_pZTwain )
      m_pZTwain->ProcessMessage( pMsg );

   return( CFrameWnd::PreTranslateMessage( pMsg ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0    // we do not use MDI at this time
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDrFrameMDI

IMPLEMENT_DYNCREATE(ZDrFrameMDI, CMDIChildWnd)

BEGIN_MESSAGE_MAP(ZDrFrameMDI, CMDIChildWnd)
   //{{AFX_MSG_MAP(ZDrFrameMDI)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZDrFrameMDI construction/destruction

ZDrFrameMDI::ZDrFrameMDI()
{
   // TODO: add member initialization code here
}

ZDrFrameMDI::~ZDrFrameMDI()
{
}

BOOL
ZDrFrameMDI::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs.

   return( CMDIChildWnd::PreCreateWindow(cs) );
}

/////////////////////////////////////////////////////////////////////////////
// ZDrFrameMDI diagnostics

#ifdef _DEBUG
void
ZDrFrameMDI::AssertValid() const
{
   CMDIChildWnd::AssertValid();
}

void
ZDrFrameMDI::Dump(CDumpContext& dc) const
{
   CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ZDrFrameMDI message handlers

BOOL
ZDrFrameMDI::OnCreateClient( LPCREATESTRUCT /* lpcs */,
                             CCreateContext *pContext )
{
   return( m_wndSplitter.Create( this,
                                 2, 2, // TODO: adjust the number of rows, columns
                                 CSize( 10, 10 ), // TODO: adjust the minimum pane size
                                 pContext ) );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
