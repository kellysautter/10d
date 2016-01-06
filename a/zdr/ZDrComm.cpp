/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrcomm.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Common dialogs.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...

// 2002.12.12    DKS
//   Added Directory Selector option to OperatorPromptForFile operation.
//
// 2001.09.05    DKS
//   Set parent window of OperatorPrompt operations.
//
// 2001.06.29    TMV
//   change OperatorPromptFindReplace to use customized
//   dialog ZFindReplaceDialog instead CFindReplaceDialog
//
// 2000.11.28    HH
//   const zCHAR change OperatorPromptForFile
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added operation OperatorPromptFindReplace.
//

#include "zstdafx.h"
#include "resource.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class ZDirDialog
{
public:

   ZDirDialog( );
   virtual ~ZDirDialog( );

   zBOOL DoBrowse( CWnd *pwndParent = 0 );

   CString m_csWindowTitle;
   CString m_csPath;
   CString m_csInitDir;
   CString m_csSelDir;
   CString m_csTitle;
   int     m_iImageIndex;
   zBOOL   m_bStatus;

private:

   virtual BOOL SelChanged(LPCSTR lpcsSelection, CString& csStatusText)
   {
      return TRUE;
   };

   static int __stdcall ZDirDialog::BrowseCtrlCallback( HWND hWnd, UINT uMsg,
                                                        LPARAM lParam,
                                                        LPARAM lpData );
};

/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OperatorPromptForColor
//                                              12/29/95 Modified: 12/29/95
//
//  PURPOSE:   To select a color using the common color dialog
//
//  PARAMETERS: vSubtask    - the application subtask view
//              lpColorRef  - pointer to the return color
//              lpaCustColorRef - return 32 item array of custom colors
//              bUpdate     - TRUE ==> start with current color
//
//  RETURNS:   -1 - Error locating window
//              1 - Operator selected a color
//              2 - Operator canceled color selection dialog
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
OperatorPromptForColor( zVIEW    vSubtask,
                        zPLONG   lpColorRef,
                        zPLONG   lpaCustColorRef,
                        zSHORT   bUpdate )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zSHORT nRC;

      if ( bUpdate == FALSE )
         *lpColorRef = RGB( 0, 0, 0 );

      CColorDialog dlg( *lpColorRef, 0, pZSubtask->m_pZFWnd );

      if ( (nRC = dlg.DoModal( )) == IDOK )
         *lpColorRef = (zLONG) dlg.GetColor( );

      return( nRC );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OperatorPromptForFont
//                                              12/29/95 Modified: 12/29/95
//
//  PURPOSE:   To select a font using the common font dialog
//
//  PARAMETERS: vSubtask  - the application subtask view
//              lpLogFont - pointer to the return font structure
//              lplPointSize - pointer to the return font point size
//              lpColorRef - pointer to the return color
//              bUpdate    - TRUE ==> start with current font
//
//  RETURNS:   -1 - Error locating window
//              1 - Operator selected a font
//              2 - Operator canceled font selection dialog
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
OperatorPromptForFont( zVIEW    vSubtask,
                       zPVOID   lpLogFont,
                       zPLONG   lplPointSize,
                       zPLONG   lpColorRef,
                       zSHORT   bUpdate )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      CClientDC dc( 0 );
      LOGFONT lf;
      zSHORT  nRC;

      if ( bUpdate )
         lf = *((LOGFONT *) lpLogFont);

      lf.lfHeight = *lplPointSize;
      lf.lfHeight = ::MulDiv( lf.lfHeight,
                              dc.GetDeviceCaps( LOGPIXELSY ), 72 );

      CFontDialog dlg( &lf, CF_EFFECTS | CF_SCREENFONTS,
                       0, pZSubtask->m_pZFWnd );

      if ( lpColorRef )
         dlg.m_cf.rgbColors = *lpColorRef;
      else
         dlg.m_cf.rgbColors = RGB( 0, 0, 0 );

      if ( (nRC = dlg.DoModal( )) == IDOK )
      {
         lf.lfHeight = ::MulDiv( lf.lfHeight, 72,
                                 dc.GetDeviceCaps( LOGPIXELSY ) );
         if ( lpColorRef )
            *lpColorRef = dlg.GetColor( );

         *((LOGFONT *) lpLogFont) = lf;
         if ( lf.lfHeight < 0 )
            lf.lfHeight = -lf.lfHeight;

         *lplPointSize = lf.lfHeight;
      }

      return( nRC );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OperatorPromptForFile
//                                              12/29/95 Modified: 12/29/95
//
//  PURPOSE:   To select a file using the common file dialog
//
//  PARAMETERS: vSubtask  - the application subtask view
//              pchFileName - return file name
//              nFileNameLth - maximum return file name length
//              cpcFilter - filter to select files (defaults to "*.*")
//                          (current selected directory for directory select)
//              cpcDefaultExtension - default extension (Title for directory)
//              lFlags - TRUE, file must exist (for read)
//                       FALSE, file may not exist (for SaveAs)
//
// CFileDialog( BOOL bOpenFileDialog,  LPCTSTR lpszDefExt = NULL,
//              LPCTSTR lpszFileName = NULL,
//              DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//              LPCTSTR lpszFilter = NULL, CWnd *pParentWnd = NULL );
//
// Parameters
// ----------
// bOpenFileDialog - Set to TRUE to construct a File Open dialog box or FALSE
// to construct a File Save As dialog box.
//
// lpszDefExt - The default filename extension. If the user does not include an
// extension in the Filename edit box, the extension specified by lpszDefExt
// is automatically appended to the filename. If this parameter is NULL, no
// file extension is appended.
//
// lpszFileName - The initial filename that appears in the filename edit box.
// If NULL, no filename initially appears.
//
// dwFlags - A combination of one or more flags that allow you to customize
// the dialog box. For a description of these flags, see the OPENFILENAME
// structure in the Win32 SDK documentation. If you modify the m_ofn.Flags
// structure member, use a bitwise-OR operator in your changes to keep the
// default behavior intact.
//
// lpszFilter - A series of string pairs that specify filters you can apply to
// the file. If you specify file filters, only selected files will appear in
// the Files list box. See the Remarks section for more information on how to
// work with file filters.
//
// pParentWnd - A pointer to the file dialog-box object's parent or owner window.
//
// Remarks
// -------
// Call this function to construct a standard Windows file dialog box-object.
// Either a File Open or File Save As dialog box is constructed, depending on
// the value of bOpenFileDialog.
//
// To allow the user to select multiple files, set the OFN_ALLOWMULTISELECT
// flag before calling DoModal. You need to supply your own filename buffer to
// accommodate the returned list of multiple filenames. Do this by replacing
// m_ofn lpstrFile with a pointer to a buffer you have allocated, after
// constructing the CFileDialog, but before calling DoModal. Additionally, you
// must set m_ofn.nMaxFile with the number of characters in the buffer pointed
// to by m_ofn.lpstrFile.
//
// The lpszFilter parameter is used to determine the type of filename a file
// must have to be displayed in the file list box.  The first string in the
// string pair describes the filter; the second string indicates the file
// extension to use. Multiple extensions may be specified using ';' as the
// delimiter.  The string ends with two '|' characters, followed by a NULL
// character.  You can also use a CString object for this parameter.
//
// For example, Microsoft Excel permits users to open files with extensions
// .XLC (chart) or .XLS (worksheet), among others. The filter for Excel could
// be written as:
//
// static zCHAR szFilter[] =
//    "Chart Files (*.xlc)|*.xlc|Worksheet Files
//    (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||";
//
//
//   lFlags values:
//
//    0 - zOFN_HIDEREADONLY | zOFN_NOREADONLYRETURN | zOFN_NOCHANGEDIR |
//           zOFN_PATHMUSTEXIST;
//    1 - zOFN_HIDEREADONLY | zOFN_NOREADONLYRETURN | zOFN_NOCHANGEDIR |
//           zOFN_FILEMUSTEXIST;
//   otw - the lFlags flag is set by the application as a sum of any
//         of the following values:
//
//   zOFN_READONLY  - 2
//   Causes the Read Only check box to be initially checked when the dialog
//   box is created. When the user chooses the OK button to close the dialog
//   box, the state of the Read Only check box is specified by this member.
//   This flag can be set on input and output.
//
//   zOFN_OVERWRITEPROMPT - 4
//   Causes the Save As dialog box to generate a message box if the selected
//   file already exists. The user must confirm whether to overwrite the
//   file.
//
//   zOFN_HIDEREADONLY - 8
//   Hides the Read Only check box.
//
//   zOFN_NOCHANGEDIR - 16
//   Forces the dialog box to reset the current directory to what it was when
//   the dialog box was created.
//
//   zOFN_SHOWHELP - 32
//   Causes the dialog box to show the Help push button. The hwndOwner must
//   not be NULL if this option is specified.
//
//   zOFN_NOVALIDATE - 64
//   Specifies that the common dialog boxes will allow invalid characters in
//   the returned filename. Typically, the calling application uses a hook
//   function that checks the filename using the FILEOKSTRING registered
//   message.  If the text in the edit control is empty or contains nothing
//   but spaces, the lists of files and directories are updated.  If the text
//   in the edit control contains anything else, the nFileOffset and
//   nFileExtension members are set to values generated by parsing the text.
//   No default extension is added to the text, nor is text copied to the
//   lpstrFileTitle buffer. If the value specified by the nFileOffset member
//   is negative, the filename is invalid. If the value specified by
//   nFileOffset is not negative, the filename is valid, and nFileOffset and
//   nFileExtension can be used as if the OFN_NOVALIDATE flag had not been
//   set.
//
//   zOFN_ALLOWMULTISELECT - 128
//   Specifies that the File Name list box is to allow multiple selections.
//   When this flag is set, the lpstrFile member points to a buffer
//   containing the path to the current directory and all filenames in the
//   selection. The first filename is separated from the path by a space.
//   Each subsequent filename is separated by one space from the preceding
//   filename. Some of the selected filenames may be preceded by relative
//   paths; for example, the buffer could contain something like this:
//
//   zOFN_EXTENSIONDIFFERENT - 256
//   Indicates that the extension of the returned filename is different from
//   the extension specified by the lpstrDefExt member. This flag is not set
//   if lpstrDefExt is NULL, if the extensions match, or if the file has no
//   extension. This flag can be set on output.
//
//   zOFN_PATHMUSTEXIST - 512
//   Specifies that the user can type only valid paths. If this flag is set
//   and the user types an invalid path in the File Name edit control, the
//   dialog box procedure displays a warning in a message box.
//
//   zOFN_FILEMUSTEXIST - 1024
//   Specifies that the user can type only the names of existing files in the
//   File Name edit control. If this flag is set and the user types an
//   invalid filename in the File Name edit control, the dialog box procedure
//   displays a warning in a message box. (This flag also causes the
//   OFN_PATHMUSTEXIST flag to be set.)
//
//   zOFN_CREATEPROMPT - 2048
//   Causes the dialog box procedure to generate a message box to notify the
//   user when a specified file does not currently exist and to make it
//   possible for the user to specify that the file should be created. (This
//   flag automatically sets the OFN_PATHMUSTEXIST and OFN_FILEMUSTEXIST
//   flags.)
//
//   zOFN_SHAREAWARE - 4096
//   Specifies that if a call to the OpenFile function has failed because of
//   a network sharing violation, the error is ignored and the dialog box
//   returns the given filename.
//
//   zOFN_NOREADONLYRETURN - 8192
//   Specifies that the file returned will not have the Read Only attribute
//   set and will not be in a write-protected directory.
//
//   zOFN_SAVEAS - 16384
//   Specifies that a File SaveAs dialog will be created (rather than a
//   File Open dialog.
//
//   zOFN_GETDIRECTORY - 32768
//   Specifies that a Locate Directory dialog will be created.
//
//  RETURNS:   -1 - Error locating window
//              1 - (IDOK) Operator selected a file
//              2 - Operator canceled file selection dialog
//
// Example: zCHAR szFilter[] = "Chart Files (*.xlc)|*.xlc|"
//                              "Worksheet Files (*.xls)|*.xls|"
//                              "Data Files (*.xlc;*.xls)|*.xlc; *.xls|"
//                              "All Files (*.*)|*.*||";
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
OperatorPromptForFile( zVIEW    vSubtask,
                       zPCHAR   pchFileName,
                       zSHORT   nFileNameLth,
                       zCPCHAR  cpcFilter,
                       zCPCHAR  cpcDefaultExtension,
                       zULONG   ulZFlags )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zPCHAR pchDfltExt;
      zULONG ulFlags;
      zBOOL  bFileOpen = TRUE;
      zLONG  lLth;

      if ( cpcDefaultExtension )
      {
         lLth = zstrlen(cpcDefaultExtension) + 1;
         pchDfltExt = new char[ lLth ];
         strcpy_s( pchDfltExt, lLth, cpcDefaultExtension );
      }
      else
         pchDfltExt = 0;

      if ( ulZFlags == 0 )
         ulFlags = OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_NOCHANGEDIR |
                   OFN_PATHMUSTEXIST;
                   // OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT; user will check
      else
      if ( ulZFlags == 1 )
         ulFlags = OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_NOCHANGEDIR |
                   OFN_FILEMUSTEXIST;
      else
      {
#if (WINVER >= 0x0400 )
         ulFlags = OFN_EXPLORER;
#else
         ulFlags = 0;
#endif // WINVER >= 0x0400

         if ( ulZFlags & zOFN_READONLY )
            ulFlags |= OFN_READONLY;

         if ( ulZFlags & zOFN_OVERWRITEPROMPT )
            ulFlags |= OFN_OVERWRITEPROMPT;

         if ( ulZFlags & zOFN_HIDEREADONLY )
            ulFlags |= OFN_HIDEREADONLY;

         if ( ulZFlags & zOFN_NOCHANGEDIR )
            ulFlags |= OFN_NOCHANGEDIR;

         if ( ulZFlags & zOFN_SHOWHELP )
            ulFlags |= OFN_SHOWHELP;

         if ( ulZFlags & zOFN_NOVALIDATE )
            ulFlags |= OFN_NOVALIDATE;

         if ( ulZFlags & zOFN_ALLOWMULTISELECT )
            ulFlags |= OFN_ALLOWMULTISELECT;

         if ( ulZFlags & zOFN_EXTENSIONDIFFERENT )
            ulFlags |= OFN_EXTENSIONDIFFERENT;

         if ( ulZFlags & zOFN_PATHMUSTEXIST )
            ulFlags |= OFN_PATHMUSTEXIST;

         if ( ulZFlags & zOFN_FILEMUSTEXIST )
            ulFlags |= OFN_FILEMUSTEXIST;

         if ( ulZFlags & zOFN_CREATEPROMPT )
            ulFlags |= OFN_CREATEPROMPT;

         if ( ulZFlags & zOFN_SHAREAWARE )
            ulFlags |= OFN_SHAREAWARE;

         if ( ulZFlags & zOFN_NOREADONLYRETURN )
            ulFlags |= OFN_NOREADONLYRETURN;

         if ( ulZFlags & zOFN_SAVEAS )
            bFileOpen = FALSE;
      }

      zSHORT nRC = 0;
      if ( ulZFlags & zOFN_GETDIRECTORY )
      {
         ZDirDialog dlg;

         if ( cpcDefaultExtension && *cpcDefaultExtension )
            dlg.m_csTitle = cpcDefaultExtension;
         else
            dlg.m_csTitle = _T( "Choose directory" );

         // Default previous file1; New by Klaus 981014
         dlg.m_csSelDir = cpcFilter;

         if ( dlg.DoBrowse( pZSubtask->m_pZFWnd ) )
         {
            nRC = IDOK;
            strcpy_s( pchFileName, nFileNameLth, dlg.m_csPath );
         }
         else
         {
            nRC = 2;
            pchFileName[ 0 ] = 0;
         }
      }
      else
      {
         TraceLine( "OperatorPromptForFile FileOpen: %d  DfltExt: %s  "
                    "FileName: %s  Flags: 0x%08x  Filter: %s",
                    bFileOpen, pchDfltExt, pchFileName, ulFlags,
                    cpcFilter ? cpcFilter : "All Files (*.*)|*.*||" );
         CFileDialog dlg( bFileOpen, pchDfltExt, pchFileName, ulFlags,
                          cpcFilter ? cpcFilter : "All Files (*.*)|*.*||",
                          pZSubtask->m_pZFWnd );

         nRC = dlg.DoModal( );
         mDeleteInitA( pchDfltExt );
         if ( nRC == IDOK )
         {
            strcpy_s( pchFileName, nFileNameLth, dlg.GetPathName( ) );
         }
         else
         {
            DWORD dw = ::CommDlgExtendedError( );
#if 0
#define CDERR_DIALOGFAILURE    0xFFFF

#define CDERR_GENERALCODES     0x0000
#define CDERR_STRUCTSIZE       0x0001
#define CDERR_INITIALIZATION   0x0002
#define CDERR_NOTEMPLATE       0x0003
#define CDERR_NOHINSTANCE      0x0004
#define CDERR_LOADSTRFAILURE   0x0005
#define CDERR_FINDRESFAILURE   0x0006
#define CDERR_LOADRESFAILURE   0x0007
#define CDERR_LOCKRESFAILURE   0x0008
#define CDERR_MEMALLOCFAILURE  0x0009
#define CDERR_MEMLOCKFAILURE   0x000A
#define CDERR_NOHOOK           0x000B
#define CDERR_REGISTERMSGFAIL  0x000C

#define PDERR_PRINTERCODES     0x1000
#define PDERR_SETUPFAILURE     0x1001
#define PDERR_PARSEFAILURE     0x1002
#define PDERR_RETDEFFAILURE    0x1003
#define PDERR_LOADDRVFAILURE   0x1004
#define PDERR_GETDEVMODEFAIL   0x1005
#define PDERR_INITFAILURE      0x1006
#define PDERR_NODEVICES        0x1007
#define PDERR_NODEFAULTPRN     0x1008
#define PDERR_DNDMMISMATCH     0x1009
#define PDERR_CREATEICFAILURE  0x100A
#define PDERR_PRINTERNOTFOUND  0x100B
#define PDERR_DEFAULTDIFFERENT 0x100C

#define CFERR_CHOOSEFONTCODES  0x2000
#define CFERR_NOFONTS          0x2001
#define CFERR_MAXLESSTHANMIN   0x2002

#define FNERR_FILENAMECODES    0x3000
#define FNERR_SUBCLASSFAILURE  0x3001
#define FNERR_INVALIDFILENAME  0x3002
#define FNERR_BUFFERTOOSMALL   0x3003

#define FRERR_FINDREPLACECODES 0x4000
#define FRERR_BUFFERLENGTHZERO 0x4001

#define CCERR_CHOOSECOLORCODES 0x5000
#endif
            nRC = 2;
            pchFileName[ 0 ] = 0;
         }
      }

      return( nRC );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// DIALOG OPERATION: OperatorPromptFindReplace
//                                              12/29/95 Modified: 12/29/95
//
// PURPOSE:   To find/replace using the common find/replace dialog
//
// FINDREPLACE structure definition:
// typedef struct
// {
//    DWORD        lStructSize;
//    HWND         hwndOwner;
//    HINSTANCE    hInstance;
//    DWORD        Flags;
//    LPTSTR       lpstrFindWhat;
//    LPTSTR       lpstrReplaceWith;
//    WORD         wFindWhatLen;
//    WORD         wReplaceWithLen;
//    LPARAM       lCustData;
//    LPFRHOOKPROC lpfnHook;
//    LPCTSTR      lpTemplateName;
// } FINDREPLACE;
//
// lStructSize
//    Specifies the length, in bytes, of the structure.
//
// hwndOwner
//    Identifies the window that owns the dialog box. The window
//    procedure of the specified window receives FINDMSGSTRING
//    messages from the dialog box. This member can be any valid
//    window handle, but it must not be NULL.
//
// hInstance
//    If the FR_ENABLETEMPLATEHANDLE flag is set in the Flags
//    member, hInstance is the handle of a memory object
//    containing a dialog box template. If the FR_ENABLETEMPLATE
//    flag is set, hInstance identifies a module that contains a
//    dialog box template named by the lpTemplateName member. If
//    neither flag is set, this member is ignored.
//
// Flags
//    A set of bit flags that you can use to initialize the dialog
//    box. The dialog box sets these flags when it sends the
//    FINDMSGSTRING registered message to indicate the user's
//    input. This member can be a combination of the following
//    flags:
//
// Flag    Meaning
//    FR_DIALOGTERM
//       If set in a FINDMSGSTRING message, indicates that the dialog
//       box is closing. When you receive a message with this flag
//       set, the dialog box window handle returned by the FindText
//       or ReplaceText function is no longer valid.
//
//    FR_DOWN
//       If set, the Down button of the direction radio buttons in a
//       Find dialog box is selected indicating that you should
//       search from the current location to the end of the document.
//       If not set, the Up button is selected so you should search
//       to the beginning of the document. You can set this flag to
//       initialize the dialog box. If set in a FINDMSGSTRING
//       message, indicates the user's selection.
//
//    FR_ENABLEHOOK
//       Enables the hook function specified in the lpfnHook member.
//       This flag is used only to initialize the dialog box.
//
//    FR_ENABLETEMPLATE
//       Indicates that the hInstance and lpTemplateName members
//       specify a dialog box template to use in place of the default
//       template. This flag is used only to initialize the dialog
//       box.
//
//    FR_ENABLETEMPLATEHANDLE
//       Indicates that the hInstance member identifies a data block
//       that contains a preloaded dialog box template. The system
//       ignores the lpTemplateName member if this flag is
//       specified.
//
//    FR_FINDNEXT
//       If set in a FINDMSGSTRING message, indicates that the user
//       clicked the Find Next button in a Find or Replace dialog
//       box. The lpstrFindWhat member specifies the string to search
//       for.
//
//    FR_HIDEUPDOWN
//       If set when initializing a Find dialog box, hides the search
//       direction radio buttons.
//
//    FR_HIDEMATCHCASE
//       If set when initializing a Find or Replace dialog box, hides
//       the Match Case check box.
//
//    FR_HIDEWHOLEWORD
//       If set when initializing a Find or Replace dialog box, hides
//       the Match Whole Word Only check box.
//
//    FR_MATCHCASE
//       If set, the Match Case check box is checked indicating that
//       the search should be case-sensitive. If not set, the check
//       box is unchecked so the search should be case-insensitive.
//       You can set this flag to initialize the dialog box. If set
//       in a FINDMSGSTRING message, indicates the user's selection.
//
//    FR_NOMATCHCASE
//       If set when initializing a Find or Replace dialog box,
//       disables the Match Case check box.
//
//    FR_NOUPDOWN
//       If set when initializing a Find dialog box, disables the
//       search direction radio buttons.
//
//    FR_NOWHOLEWORD
//       If set when initializing a Find or Replace dialog box,
//       disables the Whole Word check box.
//
//    FR_REPLACE
//       If set in a FINDMSGSTRING message, indicates that the user
//       clicked the Replace button in a Replace dialog box. The
//       lpstrFindWhat member specifies the string to be replaced and
//       the lpstrReplaceWith member specifies the replacement
//       string.
//
//    FR_REPLACEALL
//       If set in a FINDMSGSTRING message, indicates that the user
//       clicked the Replace All button in a Replace dialog box. The
//       lpstrFindWhat member specifies the string to be replaced and
//       the lpstrReplaceWith member specifies the replacement
//       string.
//
//    FR_SHOWHELP
//       Causes the dialog box to display the Help button. The
//       hwndOwner member must specify the window to receive the
//       HELPMSGSTRING registered messages that the dialog box sends
//       when the user clicks the Help button.
//
//    FR_WHOLEWORD
//       If set, the Match Whole Word Only check box is checked
//       indicating that you should search only for whole words that
//       match the search string. If not set, the check box is
//       unchecked so you should also search for word fragments that
//       match the search string. You can set this flag to initialize
//       the dialog box. If set in a FINDMSGSTRING message, indicates
//       the user's selection.
//
// lpstrFindWhat
//    Pointer to a buffer that a FINDMSGSTRING message uses to
//    pass the null terminated search string that the user typed
//    in the Find What edit control. You must dynamically allocate
//    the buffer or use a global or static array so it does not go
//    out of scope before the dialog box closes. The buffer should
//    be at least 80 characters long. If the buffer contains a
//    string when you initialize the dialog box, the string is
//    displayed in the Find What edit control.  If a FINDMSGSTRING
//    message specifies the FR_FINDNEXT flag, lpstrFindWhat
//    contains the string to search for. The FR_DOWN,
//    FR_WHOLEWORD, and FR_MATCHCASE flags indicate the direction
//    and type of search. If a FINDMSGSTRING message specifies the
//    FR_REPLACE or FR_REPLACE flags, lpstrFindWhat contains the
//    string to be replaced.
//
// lpstrReplaceWith
//    Pointer to a buffer that a FINDMSGSTRING message uses to
//    pass the null terminated replacement string that the user
//    typed in the Replace With edit control. You must dynamically
//    allocate the buffer or use a global or static array so it
//    does not go out of scope before the dialog box closes. If
//    the buffer contains a string when you initialize the dialog
//    box, the string is displayed in the Replace With edit
//    control.  If a FINDMSGSTRING message specifies the
//    FR_REPLACE or FR_REPLACEALL flags, lpstrReplaceWith contains
//    the replacement string . The FindText function ignores this
//    member.
//
// wFindWhatLen
//    Specifies the length, in bytes, of the buffer pointed to by
//    the lpstrFindWhat member.
//
// wReplaceWithLen
//    Specifies the length, in bytes, of the buffer pointed to by
//    the lpstrReplaceWith member.
//
// lCustData
//    Specifies application-defined data that the system passes to
//    the hook procedure identified by the lpfnHook member. When
//    the system sends the WM_INITDIALOG message to the hook
//    procedure, the message's lParam parameter is a pointer to
//    the FINDREPLACE structure specified when the dialog was
//    created. The hook procedure can use this pointer to get the
//    lCustData value.
//
// lpfnHook
//    Pointer to an FRHookProc hook procedure that can process
//    messages intended for the dialog box. This member is ignored
//    unless the FR_ENABLEHOOK flag is set in the Flags member.
//    If the hook procedure returns FALSE in response to the
//    WM_INITDIALOG message, the hook procedure must display the
//    dialog box or else the dialog box will not be shown. To do
//    this, first perform any other paint operations, and then
//    call the ShowWindow and UpdateWindow functions.
//
// lpTemplateName
//    Pointer to a null-terminated string that names the dialog
//    box template resource in the module identified by the
//    hInstance member. This template is substituted for the
//    standard dialog box template. For numbered dialog box
//    resources, this can be a value returned by the
//    MAKEINTRESOURCE macro. This member is ignored unless the
//    FR_ENABLETEMPLATE flag is set in the Flags member.
//
//
// CFindReplaceDialog::Create
//    BOOL Create( BOOL bFindDialogOnly, LPCTSTR lpszFindWhat,
//                 LPCTSTR lpszReplaceWith = NULL,
//                 DWORD dwFlags = FR_DOWN, CWnd *pParentWnd = NULL );
//
// Return Value
//    Nonzero if the dialog box object was successfully created;
//    otherwise 0.
//
// Parameters
//
// bFindDialogOnly
//    Set this parameter to TRUE to display the standard Windows
//    Find dialog box. Set it to FALSE to display the Windows
//    Find/Replace dialog box.
//
// lpszFindWhat
//    Specifies the string for which to search.
//
// lpszReplaceWith
//    Specifies the default string with which to replace found strings.
//
// dwFlags
//    One or more flags you can use to customize the settings of
//    the dialog box, combined using the bitwise OR operator. The
//    default value is FR_DOWN, which specifies that the search is
//    to proceed in a downward direction. See the FINDREPLACE
//    structure in the Win32 SDK documentation for more
//    information on these flags.
//
// pParentWnd
//    A pointer to the dialog box’s parent or owner window. This
//    is the window that will receive the special message
//    indicating that a find/replace action is requested. If NULL,
//    the application’s main window is used.
//
// Remarks
//    Creates and displays either a Find or Find/Replace dialog
//    box object, depending on the value of bFindDialogOnly.  In
//    order for the parent window to be notified of find/replace
//    requests, you must use the Windows RegisterWindowMessage
//    function whose return value is a message number unique to
//    the application's instance. Your frame window should have a
//    message map entry that declares the callback function
//    (OnFindReplace in the example that follows) that handles
//    this registered message. The following code fragment is an
//    example of how to do this for a frame window class named
//
// CMyFrameWnd:
//
//     class CMyFrameWnd : public CFrameWnd
//     {
//     protected:
//         afx_msg LONG OnFindReplace(WPARAM wParam, LPARAM lParam);
//
//         DECLARE_MESSAGE_MAP( )
//     };
//     static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);
//
//     BEGIN_MESSAGE_MAP( CMyFrameWnd, CFrameWnd )
//        //Normal message map entries here.
//        ON_REGISTERED_MESSAGE( WM_FINDREPLACE, OnFindReplace )
//     END_MESSAGE_MAP
//
// Within your OnFindReplace function, you interpret the
// intentions of the user and create the code for the
// find/replace operations.
//
//  PARAMETERS: vSubtask  - the application subtask view
//              bReplace  - TRUE ==> Find and Replace ... otw Find only
//
//  RETURNS:   -1 - Error locating window
//             -2 - Error creating find/replace dialog
//              1 - Operator performed a find/replace
//              2 - Operator canceled find/replace dialog
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
OperatorPromptFindReplace( zVIEW    vSubtask,
                           zPCHAR   pchFindWhat,
                           zPCHAR   pchReplaceWith,
                           zSHORT   bReplace )
{
#ifndef zREMOTE_SERVER

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zSHORT nRC = -2;
      zLONG  lFRFlags = FR_DOWN;

      ZFindReplaceDialog *pFindReplaceDlg = new ZFindReplaceDialog;
      zPCHAR pchResource = MAKEINTRESOURCE( bReplace ? IDD_ZREPLACEDLG :
                                                       IDD_ZFINDDLG );
      if ( pchResource )
      {
         pFindReplaceDlg->m_fr.lpTemplateName = pchResource;
         lFRFlags = lFRFlags|FR_ENABLETEMPLATE;
      }

      nRC = pFindReplaceDlg->Create( !bReplace, pchFindWhat,
                                     pchReplaceWith, lFRFlags,
                                     pZSubtask->m_pZView );
      return( nRC );
   }
#endif
   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//#include "shlobj.h"

// Callback function called by SHBrowseForFolder's browse control
// after initialization and when selection changes.
int __stdcall
ZDirDialog::BrowseCtrlCallback( HWND hWnd, UINT uMsg,
                                LPARAM lParam, LPARAM lpData )
{
   ZDirDialog *pDirDialogObj = (ZDirDialog *) lpData;
   if ( uMsg == BFFM_INITIALIZED )
   {
      if ( pDirDialogObj->m_csSelDir.IsEmpty( ) == FALSE )
         ::SendMessage( hWnd, BFFM_SETSELECTION, TRUE,
                        (LPARAM)(LPCTSTR) (pDirDialogObj->m_csSelDir) );

      if ( pDirDialogObj->m_csWindowTitle.IsEmpty( ) == FALSE )
         ::SetWindowText( hWnd, (LPCTSTR) pDirDialogObj->m_csWindowTitle );
   }
   else
   if ( uMsg == BFFM_SELCHANGED )
   {
      LPITEMIDLIST pidl = (LPITEMIDLIST) lParam;
      zCHAR szSelection[ MAX_PATH ];

      if ( ::SHGetPathFromIDList( pidl, szSelection ) == FALSE )
         szSelection[ 0 ] = 0;

      CString csStatusText;
      BOOL bOk = pDirDialogObj->SelChanged( szSelection, csStatusText );

      if ( pDirDialogObj->m_bStatus )
         ::SendMessage( hWnd, BFFM_SETSTATUSTEXT, 0,
                        (LPARAM)(LPCSTR) csStatusText );

      ::SendMessage( hWnd, BFFM_ENABLEOK, 0, bOk );
   }

   return( 0 );
}

ZDirDialog::ZDirDialog( )
{
   m_bStatus = FALSE;
}

ZDirDialog::~ZDirDialog( )
{
}

zBOOL
ZDirDialog::DoBrowse( CWnd *pwndParent )
{
   if ( m_csSelDir.IsEmpty( ) == FALSE )
   {
      m_csSelDir.TrimRight( );
      if ( m_csSelDir.Right( 1 ) == "\\" || m_csSelDir.Right( 1 ) == "//" )
         m_csSelDir = m_csSelDir.Left( m_csSelDir.GetLength( ) - 1 );
   }

   LPMALLOC pMalloc;
   if ( SHGetMalloc( &pMalloc ) != NOERROR )
       return( FALSE );

   BROWSEINFO bInfo;
   LPITEMIDLIST pidl;
   ZeroMemory( (PVOID) &bInfo, sizeof( BROWSEINFO ) );

   if ( m_csInitDir.IsEmpty( ) == FALSE )
   {
      OLECHAR       olePath[ MAX_PATH ];
      ULONG         chEaten;
      ULONG         dwAttributes;
      HRESULT       hr;
      LPSHELLFOLDER pDesktopFolder;

      // Get a pointer to the Desktop's IShellFolder interface.
      if ( SUCCEEDED( SHGetDesktopFolder( &pDesktopFolder ) ) )
      {
         // IShellFolder::ParseDisplayName requires the file name be in Unicode.
         MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED,
                              m_csInitDir.GetBuffer( MAX_PATH ), -1,
                              olePath, MAX_PATH );

         m_csInitDir.ReleaseBuffer( -1 );

         // Convert the path to an ITEMIDLIST.
         hr = pDesktopFolder->ParseDisplayName( NULL, NULL, olePath,
                                                &chEaten, &pidl,
                                                &dwAttributes );
         if ( FAILED( hr ) )
         {
            pMalloc->Free( pidl );
            pMalloc->Release( );
            return( FALSE );
         }

         bInfo.pidlRoot = pidl;
      }
   }

   bInfo.hwndOwner = pwndParent == 0 ? 0 : pwndParent->GetSafeHwnd( );
   bInfo.pszDisplayName = m_csPath.GetBuffer( MAX_PATH );
   bInfo.lpszTitle = (m_csTitle.IsEmpty( )) ? "Open" : m_csTitle;
   bInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS |
                   (m_bStatus ? BIF_STATUSTEXT : 0);

   bInfo.lpfn = BrowseCtrlCallback; // address of callback function
   bInfo.lParam = (LPARAM) this;    // pass address of object to callback function

   if ( (pidl = ::SHBrowseForFolder(&bInfo)) == NULL )
   {
      return( FALSE );
   }

   m_csPath.ReleaseBuffer( );
   m_iImageIndex = bInfo.iImage;

   if ( ::SHGetPathFromIDList( pidl,
                               m_csPath.GetBuffer( MAX_PATH ) ) == FALSE )
   {
      pMalloc->Free( pidl );
      pMalloc->Release( );
      return( FALSE );
   }

   m_csPath.ReleaseBuffer( );

   pMalloc->Free( pidl );
   pMalloc->Release( );

   return( TRUE );
}
