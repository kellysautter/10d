// CHANGE LOG most recent first order
//
// 2002.12.12   HH
//    R57617, empty data type handled as void.
//
// 2002.08.29  DGC
//    Added an argument to ParseSource( ).
//
// 2002.08.15  RR
//    Replace Name of Zeidon Online Manuals by DEFINES.
//    Remove Prefix "fn " and "vml " of  C-Function name and VML-Operation
//    name in function VML_StartHelp and OpIns_StartOperationHelp
//
// 2002.06.26   DKS    Z10
//    Fix for overwrite error.
//
// 2002.06.10   BL     R55735
//    Target Specification: if TargetExecutable not selected, set selection
//    for current Meta
//
// 2002.02.26   TMV
//    fixed search bug     R053178
//    fixed error list bug F051194
//    fixed crash bug      R055546 (occurs with large source files... > 20000 lines)
//
// 2001.6.26   TMV
//    add radio button to display all Operation except object oprs.
//    in operation insert Dialog add radio button to display all Operations
//    except object oprs. in operation insert Dialog      : see bug report R50792
//    Fix Opreation Insert Dialog to search using matchcode * which is
//    more popular than /                                 : see bug report F50708, F50710
//
//
// 2001.06.24  BL   Repository
//    If save VML or C file, set UpdateInd flag in corresponding
//    Meta (R54521, R53835)
//
// 2001.01.02  HH
//    Correct string parameter generation in function header.
//
// 2000.12.06  BL    2000
//    Bugfix Copy and Paste: Do not disable a reused Action
//
// 2000.11.07  DKS   2000
//    Do not disable Cut and Copy commands until they can be
//    appropriately enabled.  The accelerator keys for Cut and
//    Copy (Ctrl+C) do not work after a command such as Parse VML.
//
// 2000.11.07  TMV   2000  RAD 53090
//    command completion now inserts the FOR UPDATE clause when
//    completing the ACTIVATE under FULL INSERT conditions
//
// 2000.11.07  TMV   2000  53744, 50605,
//    Modified SystemClose : see Bug report 53744 (File open failed)
//    **** never ever drop the view named TZEDWRKO on zLEVEL_TASK ****
//    avoid syntac coloring when open a non VML-file via File-Open
//    avoid command completion if current file is not type of vml
//
// 2000.10.16  TMV   2000
//    removed obsolete code from TZEDFRMD_Keystroke( zVIEW vSubtask )
//    because replacement of TABS is done within the TBEDIT.OCX
//
// 2000.10.30 RG  Z2000
//    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
//
// 2000.10.16  TMV   2000   050764, 053932, 050613, 050668
//    indent parameter when inserting a new operation           : see Bug report 050764
//    replace search string now starts the very top of the file : see Bug report 053932
//    generating proper domain operation C-prototype            : see Bug report 050613
//    disable "Keyword Insert" option when editing a C-File     : see Bug report 050668
//    extend "Operation Insert" list to have operations of current source : see Bug report 050403
//
// 2000.09.23  BL     Z10
//    Modified FindBeginOfOperation for RAD 53838 (Endless Loop by Move Code)
//    Modified Generate for RAD 53811 (other symbol in Message Box)
//    Modified TZEDFRMD_DisableActions, OpIns_BuildOperList for RAD 53833
//    (insert Operation from OperationList)
//    Modified SystemClose for RAD 53744 (File open failed)
//    Modified PasteOperation, replace String with fix length with CString
//    (without this fix, ZDR.exe Crash, if insert 3x Operation AddPagePosition)
//
// 2000.09.04  BL     Z10
//    Modified SaveAndParse for not Checked out Dialogs
//
// 2000.08.31  BL     Z10
//    Modified FindBeginOfOperation for delete first Operation from Source
//    File (Operation has no comments and is in first line)
//
// 2000.08.24  BL     Z10
//    remove workaround for Repository and Tools
//
// 2000.08.16  BL     Z10
//    Code with comment BLXx is a workaround for Repository and Tools, if
//    a Dialog not checked out
//
// 2000.08.12  BL     Z10
//    Added TZEDFRMD_CopyOperationCode, RenameOperation for save as with
//    copy Operationcode
//    Remove Operation EnableZeidonMenu
//
// 2000.08.08  RG     Z2000
//    Removed Compiler-Warnings
//
// 2000.08.08  BL     Z10
//    Modified SaveFile for reuse Code
//    Added TZEDFRMD_AskForSave, TZEDFRMD_AskForSaveWithParse,
//    fnTZEDFRMD_SaveFile: if File updated and user call Target Specification
//    ask for save ( Bug TB 51210 )
//    If Component not checked out disable all function for update.
//    Modified operations for completely maintained Editor
//
// 2000.07.28  BL     Z10
//    Modified PostBuild: Delete or Move Operation from File does not open
//    Window TZEDFRMD, we reuse this Code only
//    Added TZEDFRMD_MoveOperationCode, InitEditorForMoveAndDelete,
//    OpenFileForMoveAndDelete, DropViewsForMoveAndDelete, GetOperationPosition,
//    CopyOperationToNewFile and modified PostBuild for move Operation with
//    Operation Code
//    Modified FindEndOfOperation, GotoCurrentOperation, FindBeginOfOperation and
//    added GetPositionForNextInsert: if the Source File is a C-File and do you
//    want to search the last operation, search for position of #ifdef __cplusplus
//    (if exists) or end of file
//    Modified FindBeginOfOperation: if the Source File is a C-File and do you
//    want to search the first operation, search for position of ";" (semicolon for
//    Prototypes) or "#" (for defines, includes)
//
// 2000.07.25  TMV    Z10
//    FindBeginOfOperation returns ZERO if operation starts at BOF
//    intorduce FileIsEmpty operation to check whether a file contains
//    whitespaces only

// 2000.07.20  BL    Z10
//    Added CreateSourceFile, CreateFileInfo, GotoCurrentOperation,
//    SetTargetExecutableName, CheckFileAttribute, CreateFileName,
//    SetEditorStatus, DeleteOperationFromCurrentFile and modified InitSession,
//    DropOperationFromSource for delete many Operations from Source File
//
// 2000.06.02  DKS   Z10
//    Force focus to the Editor control whenever the Editor frame is active.
//
// 2000.05.24  TMV   Z10
//    Inserting operations in C-Files that have more than one parameter
//    produced garbage
//    changed OperTemplate to insert a single string instead of multiple calls
//    of BufInsertStr...
//
// 2000.04.13  TMV   Z10
//    changed algorithm for extracting Operationnames in
//    TZEDFRMD_GotoOperation because there is a chance to get an assertion
//    under some circumstances
//    check return of GetTextFromLineOfIndex whether it's >= than tha passed buffer
//    size instead of >
//
// 2000.03.20  TMV   Z10
//    Rewrite code for SearchForward and SearchBackward
//    fixing bug in InitSession when inserting new operations
//
// 2000.03.13  TMV   Z10   TB807 TB897 TB932 TB969 TB983 TB984
//    TB807
//    generate function body within #ifdef __cplusplus #endif to have
//    a proper c-linkage
//
//    TB897
//    Find/Replace is now handled by TZEDFRMD code
//
//    TB932
//    set editor cursor to the next token if it is placed on a whitespace
//    to avoid an endless loop in TZEDFRMD_GotoOperation
//
//    TB969
//    replace CRLF by space
//
//    TB983 TB984
//    set focus to the tbedit control when pasting text from child dialogs
//
// 1999.09.03  DKS   Z10  QS999
//    Changed OperTemplate signature to pass view to subtask so that the
//    mGetWorkView macro can work properly (requires view to subtask).
//
// 1999.09.03  TMV 10 TB751
//    Implement missing GoTo Dialog
//    Remove obsolete Keystroke handling for ENTER and TAB Key
//
// 1999.08.17  TMV ALL TB718
//    avoid Syntax coloring when editing a C-File
//
// 1999.06.09  TMV ALL TB679
//    When the compiler reports Skipping... (no relevant changes ...)
//    during a compile [ one has to compile same (error/ warning free )
//    source without changes ]
//    an invalid string is assigned as a LineNumber when the error list is
//    opened now this string is checked via isdigit( p ) prior to assignment
//    Related operation;
//       ErrList_GetFocus
//
//    While pasting an operation via Menu "Zeidon/Operation Insert"
//    a decimal parameter is pasted as "DOUBLE" it has to be "DECIMAL"
//
// 1999.06.09  TMV 9J TB674
//    wrap new ocx methods into conditional compile using EXTENDED_TBEDIT
//    to prevent the users of the old ocx from getting error messages
//    related methods are:
//       SearchWindow, ReplaceWindow, GotoWindow, CanCopy, CanPaste,
//       CanUndo, CanRedo, Undo, Redo
//
// 1999.05.11  DKS   Z2000   QS999
//    The Enter key does not seem to be handled properly by the editor.
//    Temporary kludge for Enter key.
//
// 1999.05.19  TMV
//    fixing endless loop that occurs when deleting an operation from
//    source.
//
// 1999.05.11  DKS/TMV  Z10   ActiveX
//    Used new implementation of TBEditor as ActiveX control with no special
//    handling.  Temporary fixes for Return and Tab keys.
//
// 1999.04.01  TMV
//    new tbedt.ocx caused the implementation of a menu for edit and options
//    it also has some new methods to handle clipboard actions properly
//
// 1999.03.01  TMV
//    SL dropped some obsolete methods within tbedt.ocx.  Determine whether
//    any of those methods have been used in this module ... if so, replace
//    them by the newer (more flexible) methods.
//       IsComment( ) -> IsCommentAtIndex(long lIndex)
//
// 1999.02.09   TMV
//    Fix a command completion bug for =,>=,...
//
//
// 1999.01.22   TMV
//    Fixing the following Bugs
//       TB 487, TB 385, TB 494, TB 290, TB 386,
//       TB 326, TB 170, TB 151, TB 481, TB 326
//    see bug tracking system for detail information
//
// 1999.01.14  TMV
//
//    Remove obsolete DEL Key Handler
//    Do a proper "Delete Operation from Source" if the file is a C-Source
//
// 1998.12.18  TMV
//
//    Create a short cut that handles DEL Key   (TB308)
//
// 1998.11.06  TMV
//    Rewrite the Code of fnGetNextTokenFromBuffer because of a BLOCK DAMAGE error
//    from MFC Runtime. This error sometimes occured when calling the funktion AEQ_GetViews
//    to retrieve the Viewnames
//
//    Create a new function which deletes an operation from Source.
//
//    If current File is a C-Source then don't do command completion.
//
//    Insert the Describtion of the operation as a comment into the source
//
// 1998.10.16  TMV
//    Setting the editor into readonly mode if file is writeprotected
//
// 1998.10.15  TMV
//    when creating a new operation the function InitSession under some circumstances
//    steps into an endless loop
//
// 1998.10.05  TMV
//    Do some indention when inserting Operations via dialog Insert-operation
//
// 1998.09.30  HH
//    fixed following bug: loop occured on "Get Views" when there was the
//    string "OPERATION" in comments.
//
// 1998.09.11  HH
//    removed 1 (one!) blank, "VIEW " -> "VIEW" .
//
// 1998.09.09  TMV
//    Use new Methods of the ocx to prevent editor from scrolling while searching for
//    view definitions (VIEW xy BASED ON LOD xy.....)
//
// 1998.8.24   TMV / DGC
//    Do a manual merge of DGC Changes
//    Fixed VOR_PasteName( ) and OBJ_PasteObjectName( ).  They were getting some info
//    from the profile object when they should be getting info from the
//    editor work object.
//
// 1998.8.18   TMV
//    Fixing the following Bugs
//       TB  99, TB  264, TB  265, TB 317,
//    see bug tracking system for detail information
//
// 1998.8.18   TMV
//    Fixing the following Bugs
//       TB  86, TB  68, TB  98, TB 104, TB 317,
//       TB  84, TB  85, TB  90, TB 108, TB 110,
//       TB 168, TB 212, TB 222, TB 223, TB 207,
//       TB 211, TB 220, TB 169, TB 205, TB 258,
//       TB 192, TB 196, TB 225, TB 226, TB 262
//    see bug tracking system for detail information
//
// 1998.7.27   DGC
//    Added "CREATE" as Command Completion keyword for the Create statement.
//
// 1998.7.23   DGC
//    Changed argument to Generate3GL( ) to correctly pass in VML file name.
//
//
// 1998.7.15   TMV
//    position information after keystroke events are not valid erverytime
//    so the event interface of the editor control was changed.
//    The event fires after each keystroke and passes position information
//    (line and column) as paramaters
//
// 1998.7.14   TMV
//    Disable Zeidon Popup Menus if not needed
//    ( e.g. using file open to edit another file)
//    Entity Insert generates an invalid list of views when
//    the keyword VIEW is not indented the right way (Indention = 1)
//
// 1998.7.13   TMV
//    Fixing bug in : 'File new'
//
// 1998.06.29 - 1998.07.08   RG
//    Several changes from Quinsoft without change log
//
// 1998.06.22    RG
//    Added some more signs in the searchstring
//
// 1998.06.19    RG
//    Changed searchstring for function FindTextPosition because of
//    the new editor
//
// 1998.06.15    TMV (RG)
//    Fixed a bug for: 'Save As'
//
// 1998.06.02    TMV
//    Just looking for '//>' in order to make the New editor run as needed
//
// 1998.05.29    RG!!
//    Several changes from DKS without change log
//

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>

#include "tz__oprs.h"
#include "tzlodopr.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"


#define mGetProfileView( pv )      GetViewByName( (pv), szlProfileXFER, vSubtask, \
                                                  zLEVEL_TASK )
#define mGetWorkView( pvView, v )  GetViewByName( (pvView), szlTZEDWRKO, v, \
                                                  zLEVEL_SUBTASK )

// Editor related Macros
// for cursor positioning purpose
#define MovRight(n)   oEditor->GetCursorPosition( &lMoveBaseLine,      \
                                                  &lMoveBaseColumn,    \
                                                  &lMoveBaseIndex);    \
                      oEditor->SetCursorPositionByLine(lMoveBaseLine,  \
                                                  lMoveBaseColumn + n)

#define MovUp( n )    oEditor->GetCursorPosition( &lMoveBaseLine,      \
                                                  &lMoveBaseColumn,    \
                                                  &lMoveBaseIndex);    \
                      oEditor->SetCursorPositionByLine(lMoveBaseLine - n,  \
                                                  lMoveBaseColumn)

#define MovEOF( )      oEditor->GetLineCount(&lMoveBaseLine);\
                      oEditor->SetCursorPositionByLine(lMoveBaseLine + 1,  \
                                                  0)

/*
#define MovEOL( )      {\
                       zCHAR Buffer[ 2 ];\
                       oEditor->GetCursorPosition( &lMoveBaseLine,      \
                                                   &lMoveBaseColumn,    \
                                                   &lMoveBaseIndex);    \
                       lMoveBaseColumn = oEditor->GetActualTextLine( Buffer, 1);\
                       oEditor->SetCursorPositionByLine(lMoveBaseLine,  \
                                                  lMoveBaseColumn);     \
                      }
*/

#define MAIN_DIL             1
#define FILECHANGED_DIL      2
#define LINECOL_DIL          3
#define INSERTMODE_DIL       4
#define EDIT_CONTROL_NAME    "_tbe"
#define MAX_TOKEN_LTH        100
#define BUFFER_SIZE          512

#define SRCH_BACKWARD        0x00000000
#define SRCH_FORWARD         0x00000001
#define SRCH_MATCHCASE       0x00000002
#define SRCH_REGEXPR         0x00000004

#define TZEDFRMD_EDIT_CONTEXT_MENU_TAG       "EditContext"
#define TZEDFRMD_EDIT_CONTEXT_MENU_UNDO      "EditContextUndo"
#define TZEDFRMD_EDIT_CONTEXT_MENU_REDO      "EditContextRedo"
#define TZEDFRMD_EDIT_CONTEXT_MENU_CUT       "EditContextCut"
#define TZEDFRMD_EDIT_CONTEXT_MENU_COPY      "EditContextCopy"
#define TZEDFRMD_EDIT_CONTEXT_MENU_PASTE     "EditContextPaste"
#define TZEDFRMD_EDIT_CONTEXT_MENU_DELETE    "EditContextDelete"
#define TZEDFRMD_EDIT_CONTEXT_MENU_SELECTALL "EditContextSelectAll"

#define TZEDFRMD_ACTION_EDIT_UNDO      "EditUndo"
#define TZEDFRMD_ACTION_EDIT_REDO      "EditRedo"
#define TZEDFRMD_ACTION_EDIT_CUT       "EditCut"
#define TZEDFRMD_ACTION_EDIT_COPY      "EditCopy"
#define TZEDFRMD_ACTION_EDIT_PASTE     "EditPaste"
#define TZEDFRMD_ACTION_EDIT_DELETE    "EditDelete"
#define TZEDFRMD_ACTION_EDIT_SELECTALL "EditSelectAll"

// Used when pasting V.E.A
#define PASTE_VIEW_VARIABLE  0x0001
#define PASTE_ENTITY         0x0002
#define PASTE_ATTRIBUTE      0x0004
#define PASTE_WITH_QUOTE     TRUE
#define PASTE_WITHOUT_QUOTE  FALSE

#define FILE_NO_PARSE        0
#define PARSE_FILE           1

// Editor string values.
char szlSysEditorActivate[]  = "__SysEditorActivate";
char szlActiveStatus[]       = "ActiveStatus";
char szlAEQ_PasteSubtask[]   = "AEQ_PasteSubtask";
char szlAttribute[]          = "Attribute";
char szlAttributeName[]      = "AttributeName";
char szlBuffer[]             = "Buffer";
char szlBufferSaveList[]     = "BufferSaveList";
char szlC_File[]             = "C";
char szlCompilerSpecification[] = "CompilerSpecification";
char szlDesc[]               = "Desc";
char szlED[]                 = "ED";
char szlEditor[]             = "Editor";
char szlEditorProfile[]      = "EditorProfile";
char szlEntity[]             = "Entity";
char szlEntityName[]         = "EntityName";
char szlErrorList[]          = "ErrorList";
char szlExtension[]          = "Extension";
char szlFileName[]           = "FileName";
char szlFileOpenSubtask[]    = "FileOpenSubtask";
char szlHeight[]             = "Height";
char szlInsertWithQuote[]    = "InsertWithQuote";
char szlInsertText[]         = "InsertText";
char szlInsertTextFull[]     = "InsertTextFull";
char szlLanguageType[]       = "LanguageType";
char szlLOD[]                = "LOD";
char szlLPLR[]               = "LPLR";
char szlLPLR_Name[]          = "LPLR_Name";
char szlMetaType[]           = "MetaType";
char szlName[]               = "Name";
char szlNone[]               = "None";
char szlObjectName[]         = "ObjectName";
char szlObjectSelAQ[]        = "ObjectSelAQ";
char szlOper[]               = "Oper";
char szlOperation[]          = "Operation";
char szlOperationsList[]     = "OperationsList";
char szlOpIns_InsertDescribtion[] = "OpIns_InsertDescribtion";
char szlOperListType[]       = "OperListType";
char szlParentCount[]        = "ParentCount";
char szlParentSubtask[]      = "ParentSubtask";
char szlParentSubtaskName[]  = "ParentSubtaskName";
char szlPasteObjectName[]    = "PasteObjectName";
char szlPasteObjectOption[]  = "PasteObjectOption";
char szlPasteVorOption[]     = "PasteVorOption";
char szlPgmSrcDir[]          = "PgmSrcDir";
char szlPosX[]               = "PosX";
char szlPosY[]               = "PosY";
char szlProfileXFER[]        = "ProfileXFER";
char szlSaveBuffers[]        = "SaveBuffers";
char szlSaveBuffersReturnCode[] = "SaveBuffersReturnCode";
char szlSourceFile[]         = "SourceFile";
char szlSourceFileName[]     = "SourceFileName";
char szlSourceMetaCount[]    = "SourceMetaCount";
char szlSourceMetaName[]     = "SourceMetaName";
char szlSourceViewID[]       = "SourceViewID";
char szlSubdirectory[]       = "Subdirectory";
char szlSubwindow[]          = "Subwindow";
char szlSubwindowActive[]    = "SubwindowActive";
char szlSubwindowSubtask[]   = "SubwindowSubtask";
char szlTargetSpecification[] = "TargetSpecification";
char szlTaskLPLR[]           = "TaskLPLR";
char szlText[]               = "Text";
char szlType[]               = "Type";
char szlTZEDCMPD[]           = "TZEDCMPD";
char szlTZEDFRMD[]           = "TZEDFRMD";
char szlTZCMLPLO_List[]      = "TZEDFRMD_TZCMLPLO_List";
char szlTZEDWRKO[]           = "TZEDWRKO";
char szlTZOPSIGK[]           = "TZOPSIGK";
char szlTZOPSRCO[]           = "TZOPSRCO";
char szlVML_File[]           = "V";
char szlVML_FileExtension[]  = "VML";
char szlVML_Insert[]         = "VML_Insert";
char szlVML_InsertFull[]     = "VML_InsertFull";
char szlVML_Subtask[]        = "VML_Subtask";
char szlVML_Text[]           = "VML_Text";
char szlVariableName[]       = "VariableName";
char szlView[]               = "View";
char szlMETAOPERATIONS[]     = "vMETAOPERATIONS";
char szlSubtask[]            = "vSubtask";
char szlReadOnly[]           = "ReadOnly";
char szlW_MetaDef[]          = "W_MetaDef";
char szlW_MetaType[]         = "W_MetaType";
char szlWidth[]              = "Width";
char szlZeidon[]             = "Zeidon";
char szlZeidonEditor[]       = "Zeidon Editor";
char szlZeidonEditorSubtask[] = "_ZeidonEditorSubtask";
char szlZeidonInitialized[]  = "ZeidonInitialized";
char szlZeidonRestoreState[] = "ZeidonRestoreState";
char szlZKey[]               = "ZKey";

long lMoveBaseLine, lMoveBaseColumn, lMoveBaseIndex;

zBOOL   g_bIsFileNew = FALSE; // to handle "File New" menu command
FINDREPLACE g_fr;
CString g_strFindWhat = "";
zBOOL   g_bMatchCase = FALSE;

/////////////////////////////////////////////////////////////////////////////
// Operation declarations

zOPER_EXPORT zSHORT OPERATION
SearchContinue( zVIEW vSubtask,
                zLONG lDirectionFlag );
zOPER_EXPORT zSHORT OPERATION
SystemClose( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
InsertComment( ZDrTBEdt *oEditor, LPSTR szOperName, LPSTR szOperComment );
zOPER_EXPORT zSHORT OPERATION
OpIns_BuildOperList( zVIEW vSubtask );

zSHORT LOCALOPER
fnInsertVML_Text( zVIEW    vSubtask,
                  zVIEW    vEdWrk,
                  zVIEW    vProfileXFER,
                  ZDrTBEdt *oEditor );
zOPER_EXPORT zSHORT OPERATION
SaveAndParse( zVIEW vSubtask, zCPCHAR cpcGenLang );
zOPER_EXPORT zSHORT OPERATION
SaveAndParseC( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
SaveAndParseJava( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_MoveOperationCode( zVIEW  vSubtask,
                            zSHORT nCopyOrMoveFlag );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_DropOperationFromSource( zVIEW  vSubtask );

zOPER_EXPORT zSHORT OPERATION
AEQ_SelectView( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDFRMD_DisableActions( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDFRMD_EnableEditMenuActions( zVIEW vSubtask );

zLONG
FindBeginOfOperation( zVIEW    vSubtask,
                      LPCSTR   szOperationName,
                      ZDrTBEdt *oEditor );
zLONG
FindEndOfOperation( zVIEW    vSubtask,
                    LPCSTR   szOperationName,
                    ZDrTBEdt *oEditor );
zBOOL
FileIsEmpty( ZDrTBEdt *oEditor );

void
BufInsertStr( ZDrTBEdt *oEditor,
              zCPCHAR  cpc );
static zSHORT
CreateFileInfo( ZDrTBEdt *oEditor,
                zPCHAR   pszName );
static zSHORT
CreateSourceFile( zVIEW    vSubtask,
                  zVIEW    vSource,
                  ZDrTBEdt *oEditor,
                  zBOOL    bCFile,
                  zPCHAR   szSourceFileEntityName );
static zSHORT
GotoCurrentOperation( zVIEW    vSubtask,
                      zVIEW    vSource,
                      ZDrTBEdt *oEditor,
                      zPCHAR   szOperSrch,
                      zPCHAR   szMetaName,
                      zBOOL    bCFile );
static zSHORT
SetTargetExecutableName( zVIEW  vSource,
                         zVIEW  vEdWrk,
                         zPCHAR pszInvokingTool,
                         zPCHAR szMetaName,
                         zPLONG plMetaType );
static zSHORT
CheckFileAttribute( zPCHAR  pchFileName,
                    zPBOOL  bReadOnly );
static zSHORT
CreateFileName( zVIEW   vTaskLPLR,
                zVIEW   vSource,
                zPCHAR  pchFileName,
                zLONG   lMaxLth,
                zPCHAR  pszInvokingTool,
                zPCHAR  szSourceFileEntityName,
                zPBOOL  pbFileExists );
static zSHORT
SetEditorStatus( ZDrTBEdt *oEditor,
                 zVIEW    vSource,
                 zVIEW    vEdWrk );
static zSHORT
DeleteOperationFromCurrentFile( zVIEW    vSubtask,
                                zVIEW    vSource,
                                zVIEW    vDeleteOp,
                                zVIEW    vEdWrk,
                                zVIEW    vProfileXFER,
                                ZDrTBEdt *oEditor,
                                zPCHAR   pchFileName );
static zSHORT
InitEditorForMoveAndDelete( ZDrTBEdt *oEditor,
                            zVIEW    vSubtask,
                            zPVIEW   pvDeleteOp,
                            zPVIEW   pvSource,
                            zPVIEW   pvEdWrk,
                            zPVIEW   pvProfileXFER,
                            zPCHAR   pchFileName,
                            zLONG    lMaxLth );
static zSHORT
OpenFileForMoveAndDelete( ZDrTBEdt *oEditor,
                          zVIEW    vDeleteOp,
                          zVIEW    vEdWrk,
                          zVIEW    vSource,
                          zVIEW    vProfileXFER,
                          zPCHAR   pchFileName,
                          zPCHAR   szSourceFileName,
                          zBOOL    bCreateFile );
static zSHORT
DropViewsForMoveAndDelete( zVIEW vSubtask,
                           zVIEW vDeleteOp,
                           zVIEW vEdWrk );
static zBOOL
GetOperationPosition( zVIEW    vSubtask,
                      ZDrTBEdt *oEditor,
                      zVIEW    vDeleteOp,
                      zPLONG   plOpStartIndex,
                      zPLONG   plOpEndIndex );
static zLONG
CopyOperationToNewFile( ZDrTBEdt *oEditor,
                        zVIEW    vEdWrk,
                        zPCHAR   pszBuffer );
static zSHORT
GetPositionForNextInsert( ZDrTBEdt *oEditor,
                          zPLONG   plPosIndex );
static zSHORT /*LOCAL */
fnTZEDFRMD_SaveFile( zVIEW    vSubtask,
                     ZDrTBEdt *oEditor,
                     zPCHAR   pchFileName,
                     zLONG    lMaxLth,
                     zSHORT   nAskForParse );
static zSHORT
CreateErrorMessage( zVIEW  vSubtask,
                    zPCHAR szMsgText );
static zSHORT
RenameOperation( ZDrTBEdt *oEditor,
                 zVIEW    vSubtask,
                 zLONG    lPosition );
static zSHORT
SetUpdateFlagInMeta( zVIEW vSubtask );

/////////////////////////////////////////////////////////////////////////////
// Setup a comment in the current editor instance
//
// Parameter :
//    ZDrTBEdt *oEditor      I  Pointer to the Editor OCX-Instance
//    LPSTR szOperName       I  Pointer to an Operation Name
//    LPSTR szOperComment    I  Pointer to a comment
//
// Return :
//    zShort number of lines inserted as a comment
//
zOPER_EXPORT zSHORT OPERATION
InsertComment( ZDrTBEdt *oEditor, LPSTR szOperName, LPSTR szOperComment )
{
// zCHAR       szCommentType[ 256 ];
   CStringList sl;
   POSITION    pos = NULL;
   CString     strSource, strLine;
   CString     strHelp,
               strCommentStart = "/*",
               strLineStart = "**    ",
               strCommentStop = "*/",
               strNewLine = "\r\n";
   zPCHAR      pszBuffer = 0;
   zLONG       lCommentLength = 0;
   zLONG       lLineLength = 100;
   zLONG       k = 0, j = 0;

   // A line containing the beginning of Operation Documentation like "///////////..."
   CString strBegin( '*', lLineLength - strCommentStart.GetLength( ) - strCommentStop.GetLength( ) );
#if 0
   if ( GetAppOrWorkstationValue( vSource, "CommentType",
                                  szCommentType, sizeof( szCommentType ) ) &&
        szCommentType[ 0 ] == '/' )
   {
      strCommentStart = "//";
      strLineStart = "// ";
      strCommentStop = "//";

      // Fix line containing the beginning of Operation Documentation like "///// ..."
      strBegin.Replace( '*', '/' );
   }
#endif
   // A line which contains only blanks.
   CString strBlanks( ' ', lLineLength - strCommentStart.GetLength( ) - strCommentStop.GetLength( ) );
   strSource = szOperComment;

   // Setting up a list of strings which are part of the Comment.
   strLine = strNewLine + strNewLine + strCommentStart + strBegin + strNewLine;
   sl.AddTail( strLine );
   sl.AddTail( strLineStart + strNewLine ); // insert an empty comment line

   // Add operation name.
   strLine = strLineStart + "OPERATION: " + szOperName + strNewLine;
   sl.AddTail( strLine );

// strLine = strLineStart + strNewLine;  // insert an empty comment line
// sl.AddTail( strLine );

   lCommentLength = strSource.GetLength( );
   pszBuffer = strSource.GetBufferSetLength( lCommentLength );

   // replace \r and \n by spaces
   while ( *pszBuffer )
   {
      if ( *pszBuffer == '\r' || *pszBuffer == '\n' )
         *pszBuffer = ' ';

      pszBuffer++;
   }

   strSource.ReleaseBuffer( );

   strSource.TrimLeft( );     // ensure there is no blank at the beginning
   lCommentLength = strSource.GetLength( );

   j = k = 0;
   while ( lCommentLength > 0 )
   {
      strHelp = strSource.Mid( k, lLineLength ); // take the first 100 bytes

      strHelp.TrimLeft( ); // make sure there is no blank at the beginning

      // Find a previous blank to have no linefeed within a word.
      if ( strHelp.GetLength( ) >= lLineLength )
         j = strHelp.ReverseFind( ' ' );
      else
         j= -1;

      if ( j > -1 )
      {
         strHelp = strHelp.Left( j );
      }

      if ( !strHelp.IsEmpty( ) )
      {
         k += strHelp.GetLength( );    // The next 100 bytes we want to proceed with
                                       // start at k + length of current line
         lCommentLength = lCommentLength - strHelp.GetLength( );
         strHelp.TrimRight( );
         strLine = strLineStart;
         strLine = strLine + strHelp + strNewLine;
         sl.AddTail( strLine );
      }
      else
      {
         k++;
         lCommentLength--;
      }
   }

   strLine = strLineStart + strNewLine;  // insert an empty comment line
   sl.AddTail(strLine );

   strLine = strBegin + strCommentStop + strNewLine;
   sl.AddTail( strLine );

   // inserting the strings stored in the stringlist
   pos = sl.GetHeadPosition( );  // get the beginning of the list

   while ( pos )
   {
      strHelp = sl.GetAt( pos );
      oEditor->InsertItem( strHelp ); // send string to Editor control
      sl.GetNext( pos );  // get next list entry
   }

   return( sl.GetCount( ) );
}

/////////////////////////////////////////////////////////////////////////////
// check whether a file contains whitespaces only
// Parameter :
//    ZDrTBEdt *oEditor       I  Pointer to the Editor OCX-Instance
//
// Return : TRUE if whitespace only otherwise FALSE
//
zBOOL
FileIsEmpty( ZDrTBEdt *oEditor )
{
   zBOOL bIsSpace = TRUE;
   zCHAR szHelp[ BUFFER_SIZE + 1 ];
   zCHAR      szFileName[ MAX_PATH + 1 ];

   memset( szFileName, ' ', MAX_PATH );
   szFileName[ MAX_PATH + 1 ] = 0;

   if ( oEditor )
   {
      zLONG lByteCount = 1;
      zLONG lStart = 0;
      zLONG lEnd = lStart + BUFFER_SIZE;

      while ( bIsSpace && lByteCount > 0 )
      {
         lByteCount = oEditor->GetTextFromRange( szHelp, lStart, lEnd );
         for ( zLONG k = 0; k < lByteCount && bIsSpace; k++ )
         {
            if ( !isspace( szHelp[ k ]) )
               bIsSpace = FALSE;
         }

         lStart = lEnd;
         lEnd = lStart + BUFFER_SIZE;
      }
   }
   else
      bIsSpace = FALSE;

   return( bIsSpace );
}

/////////////////////////////////////////////////////////////////////////////
// Search the source for the first occurence of a requestet Operation
// which is not part of a comment
//
// Parameter :
//    LPCSTR szOperationName  I  Name of the requestet operation
//    ZDrTBEdt *oEditor       I  Pointer to the Editor OCX-Instance
//
// Return :
//    zLONG a zerobased index which represents the position of
//          the operation within the source
//
zLONG
FindBeginOfOperation( zVIEW vSubtask, LPCSTR szOperationName, ZDrTBEdt *oEditor )
{
   zLONG   lIndex = 0;
   zLONG   lENDIndex, lLine,lLastLine,lColumn;
   zCHAR   szOperSrch[ 256 ];
   long    lTBEDTDefaultSearchBehavior = 5;
   zCHAR   szExtension[ 3 ];
   CString csENDString;
   zLONG   lKeyWordLength = 0;
   zVIEW   vEdWrk = 0;
   zLONG   lDefaultLineLength = 255;

   mGetWorkView( &vEdWrk, vSubtask );

   GetStringFromAttribute( szExtension, sizeof( szExtension ), vEdWrk, szlBuffer, szlLanguageType );

   // If current file is a C-Source the end of an operation is "}" otherwise it's END
   if (zstrcmp(szExtension, szlC_File) == 0)
   {
      csENDString = "} *";
      lKeyWordLength = 1; // }
   }
   else
   {
      csENDString = "\\bEND[^!-~°§²³´ßäöüÄÖÜ]+";
      lKeyWordLength = 3; // END
   }

   // Set up search string.
   zsprintf( szOperSrch, "\\bOPERATION[^!-~°§²³´ßäöüÄÖÜ]+%s[^!-~°§²³´ßäöüÄÖÜ]*(", szOperationName );

   oEditor->FindTextPosition( szOperSrch, &lIndex, lTBEDTDefaultSearchBehavior );

   // Skipping lines where the search text is part of a comment.
   while ( oEditor->IsCommentAtIndex(lIndex ) && lIndex > -1 )
   {
      lIndex++;
      oEditor->FindTextPosition( szOperSrch, &lIndex, lTBEDTDefaultSearchBehavior );
   }

   // Now we are at the position which looks like this "OPERATION MyOperation ("
   // to find the real begin of the operation we step back until we find
   // the keyword "END" of the previous operation in order to drop all comments
   // between the begin of an operation to the end of the previous Operation.
   if ( lIndex > -1 )
   {
      lENDIndex = lIndex;
      oEditor->FindTextPosition( csENDString, &lENDIndex, SRCH_REGEXPR );

      while ( oEditor->IsCommentAtIndex(lENDIndex ) && lENDIndex > -1 )
      {
         lENDIndex--;
         oEditor->FindTextPosition( csENDString, &lENDIndex, SRCH_REGEXPR );
      }

      if ( lENDIndex > -1 )
      {
         lIndex = lENDIndex + lKeyWordLength;// add 3 for keywordlength

         // Setting index to the first column of the next line
         oEditor->GetLineCount( &lLastLine );
         oEditor->GetPositionByIndex( lIndex, &lLine, &lColumn );
         while ( lLine < lLastLine && lColumn > 0 )
         {
            lIndex++;
            oEditor->GetPositionByIndex( lIndex, &lLine, &lColumn );
         }
      }
      else
      {
         // a Value of -1 tells us there is no previous END Keyword
         // therefore we have to search for an empty line
         // the function GetTextFromLineOfIndex retrieves the text and the text-length
         // we are still at the position which looks like this "OPERATION MyOperation ("
         zLONG lReturnedBuffSize = 0;
         BOOL bIsSpace = FALSE;
         CString strBuffer;
         LPSTR szBuffer;
         szBuffer = strBuffer.GetBufferSetLength( lDefaultLineLength );

         lENDIndex = lIndex;

         oEditor->GetPositionByIndex(lENDIndex, &lLine, &lColumn );
         lENDIndex -= lColumn; // set index to begin of line
         lENDIndex --;         // set index to end of previous line

         // Get the text of the current line.
         lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer, lDefaultLineLength, lENDIndex );
         while ( lReturnedBuffSize > 0 && bIsSpace == FALSE )
         {
            LPSTR p = szBuffer;

            // Check whether the buffer contains only whitespaces.
            while (isspace(*p) && ((zLONG)(p - szBuffer) < lDefaultLineLength ))
            {
               p++;
            }

            if ( (lReturnedBuffSize - ((zLONG)(p - szBuffer))) == 0 )
            {
               // Ignore empty lines in a comment.
               if ( !oEditor->IsCommentAtIndex( lENDIndex ) )
               {
                  bIsSpace = TRUE;
               }
               else
               {
                  oEditor->GetPositionByIndex( lENDIndex, &lLine, &lColumn );
                  lENDIndex -= lColumn; // set index to begin of line
                  lENDIndex --;         // set index to end of previous line

                  // Get the text of the now current line.
                  lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer, lDefaultLineLength, lENDIndex );
               }
            }
            else
            {
               oEditor->GetPositionByIndex( lENDIndex, &lLine, &lColumn );
               lENDIndex -= lColumn; // set index to begin of line
               lENDIndex --;         // set index to end of previous line

               // Get the text of the now current line.
               lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer, lDefaultLineLength, lENDIndex);
            }
         }

         // There is a chance that there is no space leading a comment.
         // If so we are at the beginning of the file.
         if ( lENDIndex < 0 )
         {
            long l = lIndex - lColumn;// set index to line preceeding "OPERATION......"
            while ( oEditor->IsCommentAtIndex( l ) && l > 0 )
            {
               l--;
            }
            if ( l == 0 )// we are at BOF
            {
                bIsSpace = TRUE;
                lENDIndex = 0;
            }
         }

         if ( lReturnedBuffSize == 0 || bIsSpace == TRUE )
         {
            lIndex = lENDIndex;
         }
         else
         {
            lIndex = -1;
         }

         // If the Source File is a C-File and do you want to search the first
         // operation, search for position of ";" (semicolon for Prototypes) or
         // "#" (for defines, includes).
         if ( zstrcmp(szExtension, szlC_File) == 0 )
         {
            csENDString = "[#;]";

            oEditor->FindTextPosition( szOperSrch, &lENDIndex,
                                       lTBEDTDefaultSearchBehavior );
            oEditor->FindTextPosition( csENDString, &lENDIndex, SRCH_REGEXPR );
            if ( lENDIndex > -1 )
            {
               // Setting index to the first column of the next line.
               oEditor->GetPositionByIndex( lENDIndex, &lLine, &lColumn );
               oEditor->SetCursorPositionByLine( lLine + 1, 0 );
               oEditor->GetCursorPosition( &lLine, &lColumn, &lENDIndex );
            }
            if ( lENDIndex > 0 && lENDIndex > lIndex )
               lIndex = lENDIndex;
         }

      } // End of search for an empty line
   } // End of search for the end of the previous operation

   return( lIndex );
}

/////////////////////////////////////////////////////////////////////////////
// Search the source for the end of a requestet Operation
// which is not part of a comment
//
// Parameter :
//    LPCSTR szOperationName I  Name of the requestet operation the operation
//                              within the source where we start the search
//    ZDrTBEdt *oEditor      I  Pointer to the Editor OCX-Instance
//
// Return :
//    zLONG a zero based index which represents the position of
//          the end of an operation within the source
//
zLONG
FindEndOfOperation( zVIEW    vSubtask,
                    LPCSTR   szOperationName,
                    ZDrTBEdt *oEditor )
{
   zLONG    lIndex = 0, lKeyWordLength = 0;
   static   zCHAR szOperationString[] = "\\bOPERATION[^!-~°§²³´ßäöüÄÖÜ]+";
   zVIEW    vEdWrk = 0;
   zCHAR    szOperSrch[ 256 ];
   long     lTBEDTDefaultSearchBehavior = 5;
   zCHAR    szExtension[ 3 ];
   CString  csENDString;

   mGetWorkView( &vEdWrk, vSubtask );
   GetStringFromAttribute( szExtension, sizeof( szExtension ), vEdWrk, szlBuffer, szlLanguageType );

   if ( zstrcmp( szExtension, szlC_File ) == 0 )
   {
      csENDString = "} *";
      lKeyWordLength = 1; // }
   }
   else
   {
      csENDString = "\\bEND[^!-~°§²³´ßäöüÄÖÜ]+";
      lKeyWordLength = 3; // END
   }

   // setup searchstring "OPERATION OperationName ("
   zsprintf( szOperSrch, "\\bOPERATION[^!-~°§²³´ßäöüÄÖÜ]+%s[^!-~°§²³´ßäöüÄÖÜ]*(", szOperationName );
   oEditor->FindTextPosition( szOperSrch, &lIndex, lTBEDTDefaultSearchBehavior );

   // Skipping lines where the search text is part of a comment
   while ( oEditor->IsCommentAtIndex( lIndex ) && lIndex > -1 )
   {
      lIndex++;
      oEditor->FindTextPosition( szOperSrch, &lIndex,
                                 lTBEDTDefaultSearchBehavior );
      TraceLineI( "(xxx) Return value from search = ", lIndex );
   }

   if ( lIndex < 0 )
      return( lIndex );

   lIndex++; // prevent from finding the same token ("OPERATION") which we already have

   // Find the end of the current operation by searching for the beginning
   // of the next operation.  Since we want to ignore any operations that
   // might be commented out, keep searching until we are not in a comment.

   oEditor->FindTextPosition( szOperationString, &lIndex,
                              SRCH_FORWARD | SRCH_REGEXPR );
   while (oEditor->IsCommentAtIndex(lIndex ) && lIndex > -1)
   {
      lIndex++;
      oEditor->FindTextPosition( szOperationString, &lIndex,
                                 SRCH_FORWARD | SRCH_REGEXPR );
   }

   // Now we should have the begin of the next operation.
   // With this information we can search backwards until we find
   // the keyword END to skip important comments
   //
   if ( lIndex > -1 )
   {
      oEditor->FindTextPosition( csENDString, &lIndex, SRCH_REGEXPR );
      while ( oEditor->IsCommentAtIndex( lIndex ) && lIndex > -1 )
      {
         lIndex--;
         oEditor->FindTextPosition( csENDString, &lIndex, SRCH_REGEXPR );
      }

      // Right now we have the startposition of the keyword "END" which
      // ends the operation add 3 for keywordlength
      if ( lIndex > -1 )
      {
         lIndex += lKeyWordLength;

         // Setting index to the first column of the next line
         // assuming that comments in the same line as the closing END
         // belong to the operation
         zLONG lLastLine = 0, lLine = 0, lColumn = 0;
         oEditor->GetLineCount( &lLastLine );
         oEditor->GetPositionByIndex( lIndex, &lLine, &lColumn );
         while ( lLine < lLastLine && lColumn > 0 )
         {
            lIndex++;
            oEditor->GetPositionByIndex( lIndex, &lLine, &lColumn );
         }
      }
   }
   else
   {
      // an index of -1 means there is no next Operation so delete anything
      // to the end of file
      if ( zstrcmp( szExtension, szlC_File ) == 0 )
      {
         // in c-files search for #ifdef __cplusplus (from end to begin)
         // to get the position where to insert new operation
         static zCHAR szSearchString[] = "\\bifdef __cplusplus[^!-~°§²³´ßäöüÄÖÜ]+";

         zLONG lLine = 0;
         // set cursor to the last line
         oEditor->GetLineCount( &lLine );
         oEditor->SetCursorPositionByLine( lLine - 1, 0 );
         oEditor->FindTextPosition( szSearchString, &lIndex,
                                    SRCH_BACKWARD | SRCH_REGEXPR );
         lIndex--;
         zLONG lOpStartIndex = -1;
         lOpStartIndex = FindBeginOfOperation( vSubtask, szOperationName, oEditor );

         // if end position < start position, set end of file
         if ( lOpStartIndex >= 0 && lIndex < 0 || lOpStartIndex >= lIndex )
            lIndex =  1048576; // this is a Megabyte I never saw a single sorcefile

      }
      else
         lIndex =  1048576; // this is a Megabyte I never saw a single sorcefile
                            // with that size
   }

   return( lIndex );
}

static zSHORT
DeleteOperationFromCurrentFile( zVIEW    vSubtask,
                                zVIEW    vSource,
                                zVIEW    vDeleteOp,
                                zVIEW    vEdWrk,
                                zVIEW    vProfileXFER,
                                ZDrTBEdt *oEditor,
                                zPCHAR   pchFileName )
{
   zCHAR      szSourceFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR      szOperName[ 33 ];
   zBOOL      bReturn = FALSE;
   zLONG      lOpStartIndex;
   zLONG      lOpEndIndex;
   zSHORT     nRC;

   if ( OpenFileForMoveAndDelete( oEditor, vDeleteOp, vEdWrk, vSource, vProfileXFER,
                                  pchFileName, szSourceFileName, FALSE ) >= 0 )
   {
      bReturn = FALSE;

      for ( nRC = SetCursorFirstEntity( vDeleteOp, "Operation", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDeleteOp, "Operation", "" ) )
      {
         bReturn = FALSE;
         lOpStartIndex  = -1;
         lOpEndIndex    = -1;
         GetStringFromAttribute( szOperName, sizeof( szOperName ), vDeleteOp, "Operation", "Name" );

         // 1. find begin and end for the requestet operation
         lOpStartIndex = FindBeginOfOperation( vSubtask, szOperName, oEditor );

         if ( lOpStartIndex >= 0 )
            lOpEndIndex = FindEndOfOperation( vSubtask, szOperName, oEditor );

         // 2. delete code within the range of lOpStartIndex and lOpEndIndex
         if ( lOpStartIndex >= 0 && lOpEndIndex >= 0 && lOpStartIndex < lOpEndIndex )
         {
            bReturn = oEditor->DeleteTextRange( lOpStartIndex, lOpEndIndex );
            if ( bReturn == FALSE )
               bReturn = TRUE;
         }
      } // endfor ( nRC = SetCursorFirstEntity( vDeleteOp, "Operation", "" )

      if ( bReturn == TRUE)
         oEditor->SaveObject( );

      // if source file empty, delete it
      if ( FileIsEmpty( oEditor ) )
         SysOpenFile( vSubtask, szSourceFileName, COREFILE_DELETE );

      DeleteEntity( vEdWrk, "Buffer", zREPOS_NEXT );
   } // endif ( OpenFileForMoveAndDelete( oEditor, vDeleteOp, vEdWrk, vSource,...

   return( (zSHORT) bReturn );
}


zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_DropOperationFromSource( zVIEW vSubtask )
{
   zSHORT     nReturn = 0;
   zSHORT     nRC;
   zCHAR      szFileName[ zMAX_FILENAME_LTH + 1 ];
   zVIEW      vEdWrk  = 0;
   zVIEW      vSource = 0;
   zVIEW      vProfileXFER = 0;
   zVIEW      vDeleteOp = 0;
   ZDrTBEdt *oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( InitEditorForMoveAndDelete( oEditor, vSubtask, &vDeleteOp, &vSource,
                                    &vEdWrk, &vProfileXFER, szFileName, sizeof( szFileName ) ) < 0 )
   {
      return( -1 );
   }


   for ( nRC = SetCursorFirstEntity( vDeleteOp, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDeleteOp, "SourceFile", "" ) )
   {
      nReturn = DeleteOperationFromCurrentFile( vSubtask, vSource, vDeleteOp,
                                                vEdWrk, vProfileXFER, oEditor,
                                                szFileName );
   }

   DropViewsForMoveAndDelete( vSubtask, vDeleteOp, vEdWrk );

   return( nReturn );
}

// The new editor doesn't support a case-sensitive search
// the same way as the old one does. To do a case sensitive search,
// we have to format the search string the following way
// Source string : View
// Target string : [Vv][Ii][Ee][Ww]
// This function formats every character in the source, regardless of
// escape sequences. So be aware of passing incorrect search strings.
zLONG
BuildCaseSensitiveSearchString( LPCSTR lpSource, CString &strTarget )
{
   CString strLower = lpSource;
   CString strUpper = lpSource;
   CString strHelp;
   CString strFormatHelp = "[%c%c]";
   zLONG lLength;

   strTarget.Empty( );      // clean Target
   strUpper.MakeUpper( );   // Conversion so I don't have to do
   strLower.MakeLower( );   // thus c stuff isupper(*lpx)?*lpx:*lpx += 32;

   lLength = strLower.GetLength( );
   for ( zLONG k = 0; k < lLength; k++ )
   {
      if ( strUpper.GetAt( k ) != ' ' )// skip formating if blank occurs
      {
         strHelp.Format( strFormatHelp, strUpper.GetAt( k ), strLower.GetAt( k ) );
      }
      else
      {
         strHelp = " ";
      }

      strTarget += strHelp;
   }

   return( strTarget.GetLength( ) );
}

// Get the Numer of Tabs in the current line up to the given position.
int
GetTabsInLine( ZDrTBEdt *oEditor, zLONG lUpToPosition )
{
   CString  strCurrentLine;
   LPSTR    lpCurrentLine;
   zLONG    lBuffSize = 64000;   // this crashes at 16000!!!
   zLONG    lReturnedBuffSize = 0;
   zLONG    lPosition = 0;
   int      k = 0;

   lpCurrentLine = strCurrentLine.GetBufferSetLength( lBuffSize );

   // Get the text of the current line.
   lReturnedBuffSize = oEditor->GetActualTextLine( lpCurrentLine,
                                                   lBuffSize - 1 );

   // Buffer to small -> increase Buffer and do it once more.
   if ( lBuffSize < lReturnedBuffSize )
   {
      lBuffSize = lReturnedBuffSize + 1;
      lpCurrentLine = strCurrentLine.GetBufferSetLength( lBuffSize );
      lReturnedBuffSize = oEditor->GetActualTextLine( lpCurrentLine,
                                                      lBuffSize - 1 );
   }

   // Count the Number of TABS.
   for ( k = 0, lPosition;
         *lpCurrentLine && lPosition <= lUpToPosition;
         lpCurrentLine++, lPosition++ )
   {
      if ( *lpCurrentLine == '\t' )
      {
         k++;
      }
   }

   return( k );
}

void
BufInsertStr( ZDrTBEdt *oEditor, zCPCHAR psz )
{
   oEditor->InsertItem( psz );
   return;
}

int
OperTemplate( ZDrTBEdt *oEditor, zVIEW vSource, zVIEW vSubtask )
{
   zVIEW    vLOD   = 0;
   zVIEW    vEdWrk = 0;
   LPSTR    szLOD  = 0;
   LPSTR    szOperName;
   LPSTR    szType;
   LPSTR    szReturnDataType;
   LPSTR    szDataType;
   zBOOL    bPointer;
   zBOOL    bUnsigned;
   zBOOL    bFileC;
   zSHORT   nParam;
   zCHAR    szParam[ 65 ];
   zCHAR    szParamCase[ 65 ];
   zCHAR    szLine[ 65 ];
   LPSTR    szOperComment;
   LPSTR    szTab = "   ";
   CString  strFormat, strInsertBuffer;
   zSHORT   nRC;
   zSHORT   nLth;

   mGetWorkView( &vEdWrk, vSubtask );

   GetAddrForAttribute( &szOperName, vSource, szlOperation, szlName );
   GetAddrForAttribute( &szOperComment, vSource, szlOperation, "Desc" );
   GetAddrForAttribute( &szType, vSource, szlOperation, "Type" );
   GetAddrForAttribute( &szReturnDataType,
                        vSource, szlOperation, "ReturnDataType" );
   bFileC = CompareAttributeToString( vEdWrk, szlBuffer,
                                      szlLanguageType, szlC_File ) == 0;

   // First of all set up comment for the Operation.
   InsertComment( oEditor, szOperName, szOperComment );

   if ( szType )
   {
      if ( bFileC )
      {
         strFormat = "zOPER_EXPORT %s /*";
         switch ( *szReturnDataType )
         {
            case 'V':
               strInsertBuffer.Format( strFormat, "zVIEW" );
               break;

            case 'N':
               strInsertBuffer.Format( strFormat, "zSHORT" );
               break;

            case 'M':
               strInsertBuffer.Format( strFormat, "zDECIMAL" );
               break;

            case 'Y':
               strInsertBuffer.Format( strFormat, "zCHAR" );
               break;

            case 'L':
               strInsertBuffer.Format( strFormat, "zLONG" );
               break;

            case 'O':
            case 0: // empty data type is handled as void
               strInsertBuffer.Format( strFormat, "zVOID" );
               break;

            default:
               strInsertBuffer.Format( strFormat, "zSHORT" );
         }
         //BufInsertStr( oEditor, strInsertBuffer );
      }

      switch ( *szType )
      {
         case 'D':
            bFileC ? strInsertBuffer += "DIALOG */  OPERATION\r\n"
                   : strInsertBuffer += "DIALOG OPERATION\r\n";
            break;

         case 'E':
            bFileC ? strInsertBuffer += "ENTITY CONSTRAINT */  OPERATION\r\n"
                   : strInsertBuffer += "ENTITY CONSTRAINT OPERATION\r\n";
            break;

         case 'C':
         case 'O':
            bFileC ? strInsertBuffer += "OBJECT CONSTRAINT */  OPERATION\r\n"
                   : strInsertBuffer += "OBJECT CONSTRAINT OPERATION\r\n";
            break;

         case 'T':
            bFileC ? strInsertBuffer += "TRANSFORMATION */  OPERATION\r\n"
                   : strInsertBuffer += "TRANSFORMATION OPERATION\r\n";
            break;

         case 'A':
            bFileC ? strInsertBuffer += "DERIVED ATTRIBUTE */  OPERATION\r\n"
                   : strInsertBuffer += "DERIVED ATTRIBUTE OPERATION\r\n";
            break;

         case 'M':
            bFileC ? strInsertBuffer += "DOMAIN */  OPERATION\r\n"
                   : strInsertBuffer += "DOMAIN OPERATION\r\n";
            break;

         case 'G':
            bFileC ? strInsertBuffer += "GLOBAL */  OPERATION\r\n"
                   : strInsertBuffer += "GLOBAL OPERATION\r\n";
            break;

         case 'L':
         default:
            bFileC ? strInsertBuffer += "LOCAL */  OPERATION\r\n"
                   : strInsertBuffer += "LOCAL OPERATION\r\n";
         }
   }
   else
      bFileC ? strInsertBuffer += "LOCAL */  OPERATION\r\n"
             : strInsertBuffer += "LOCAL OPERATION\r\n";

   nLth = lstrlen( szOperName ) + 2;
   strInsertBuffer +=  szOperName;
   strInsertBuffer += "( ";

   CString strParamIndent( ' ', nLth );

   // Insert parameter list.
   nParam = 0;
   for ( nRC = SetCursorFirstEntity( vSource, "Parameter", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSource, "Parameter", "" ) )
   {
      if ( nParam > 0 )
      {
         strInsertBuffer += ",\r\n";
         strInsertBuffer += strParamIndent;
         //MovRight( nLth );
      }

      nParam++;
      bPointer  = CompareAttributeToString( vSource, "Parameter",
                                            "PFlag", "Y" ) == 0;
      bUnsigned = CompareAttributeToString( vSource, "Parameter",
                                            "UFlag", "Y" ) == 0;
      GetAddrForAttribute( &szDataType, vSource, "Parameter", "DataType" );

      // Create prefix for parameter name.
      strcpy_s( szParam, sizeof( szParam ), "");
      if (bFileC)
      {
         if ( bPointer && *szDataType != 'S' )
           strcat_s( szParam, sizeof( szParam ), "p");

         if ( bUnsigned && *szDataType == 'N' || *szDataType == 'L' )
           strcat_s( szParam, sizeof( szParam ), "u" );

         switch ( *szDataType )
         {
            case 'V':
               strcat_s( szParam, sizeof( szParam ), "v");
               break;

            case 'N':
               strcat_s( szParam, sizeof( szParam ), "n");
               break;

            case 'M':
               strcat_s( szParam, sizeof( szParam ), "d");
               break;

            case 'Y':
               strcat_s( szParam, sizeof( szParam ), "c");
               break;

            case 'L':
               strcat_s( szParam, sizeof( szParam ), "l");
               break;

            case 'I':
               strcat_s( szParam, sizeof( szParam ), "lpI");
               break;

            case 'O':
               // no prefix for void pointers
               break;

            default:
               strcat_s( szParam, sizeof( szParam ), "sz");
         }
      }

      // create parameter name
      nLth = (zSHORT) zstrlen( szParam );
      GetStringFromAttribute( szParam + nLth, sizeof( szParam ) - nLth, vSource, "Parameter", "ShortDesc" );
      UfCompressName( szParam + nLth,
         szParam + nLth, 32 - nLth, "", "BL", "", "B_", 1 );

      strcpy_s( szLine, sizeof( szLine ), "");
      if (bFileC)
      {
         // create prefix for "C"
         strcat_s( szLine, sizeof( szLine ), "z");

         if ( bPointer && *szDataType != 'S' ) // not for String
           strcat_s( szLine, sizeof( szLine ), "P");

         if ( bUnsigned )
         {
            switch ( *szDataType )
            {
//               case 'Y':
               case 'N':
               case 'L':
                  strcat_s( szLine, sizeof( szLine ), "U");
                  break;
            }
         }
      }

      switch ( *szDataType )
      {
         case 'V':
            strcat_s( szLine, sizeof( szLine ), "VIEW ");
            strcat_s( szLine, sizeof( szLine ), szParam );

            if ( nParam == 1 &&
                 ( *szType == 'A' || *szType == 'E' || *szType == 'T' ||
                   *szType == 'C' || *szType == 'O' ) )
            {
               zLONG ZKey;

               if ( !szLOD )
               {
                  GetIntegerFromAttribute( &ZKey, vSource, "LOD", szlZKey );
                  nRC = ActivateMetaOI_ByZKey( vSubtask, &vLOD, 0, zREFER_LOD_META,
                                               zSINGLE | zACTIVATE_ROOTONLY,
                                               ZKey, 0 );
                  if ( nRC < 0 )
                  {
                     MessagePrompt( vEdWrk, "ED0001",szlZeidonEditor,
                                    "Internal Error -- Couldn't find LOD to go \
                                    with Source Meta",
                                    1, 0, 0, 0 );
                     szLOD = "error";
                  }
                  else
                     GetAddrForAttribute( &szLOD, vLOD, "LOD", szlName );
               }

               if ( bFileC )
                  strcat_s( szLine, sizeof( szLine ), " /* " );

               strcat_s( szLine, sizeof( szLine ), " BASED ON LOD " );
               strcat_s( szLine, sizeof( szLine ), szLOD );
               if ( bFileC )
                  strcat_s( szLine, sizeof( szLine ), " */" );
            }

            break;

         case 'N':
            strcat_s( szLine, sizeof( szLine ), "SHORT ");
            strcat_s( szLine, sizeof( szLine ), szParam );
            break;

         case 'M':
            strcat_s( szLine, sizeof( szLine ), "DECIMAL ");
            strcat_s( szLine, sizeof( szLine ), szParam );
            break;

         case 'Y':
            strcat_s( szLine, sizeof( szLine ), "CHAR ");
            strcat_s( szLine, sizeof( szLine ), szParam );
            break;

         case 'L':
            bFileC ? strcat_s( szLine, sizeof( szLine ), "LONG " )
                   : strcat_s( szLine, sizeof( szLine ), "INTEGER " );
            strcat_s( szLine, sizeof( szLine ), szParam );
            break;

         case 'O':
            bFileC ? strcat_s( szLine, sizeof( szLine ), "VOID " )
                   : strcat_s( szLine, sizeof( szLine ), "VOID " );
            strcat_s( szLine, sizeof( szLine ), szParam );
            break;

         case 'I':
            bFileC ? strcpy_s( szLine, sizeof( szLine ), "LPDOMAINDATA " )
                   : strcpy_s( szLine, sizeof( szLine ), "STRING ( 32 ) " );
            strcat_s( szLine, sizeof( szLine ), szParam );
            break;

         case 'A':
         case 'E':
         default:
            if ( bFileC )
            {
               if ( !bPointer && GetVMLConstChar( ) )
                  strcat_s( szLine, sizeof( szLine ), "CPCHAR " );
               else
                  strcat_s( szLine, sizeof( szLine ), "PCHAR " );
            }
            else
               strcat_s( szLine, sizeof( szLine ), "STRING ( 32 ) " );

            strcat_s( szLine, sizeof( szLine ), szParam );
            break;
      }

      strInsertBuffer += szLine;

      // Store parameters for CASE in VML function prototype.
      switch ( nParam )
      {
         case 1:
            if ( *szType == 'M' )
               strcpy_s( szParamCase, sizeof( szParamCase ), szParam );

            break;

         case 2:
            if ( *szType == 'C' || *szType == 'O' )
               strcpy_s( szParamCase, sizeof( szParamCase ), szParam );

            break;

         case 3:
            if ( *szType == 'E' )
               strcpy_s( szParamCase, sizeof( szParamCase ), szParam );

            break;

         case 4:
            if ( *szType == 'A' )
               strcpy_s( szParamCase, sizeof( szParamCase ), szParam );

            break;

      }

   }  // End FOR EACH Parameter

   if ( bFileC )
   {
      // Use dummy parameter, to avoid compiler warnings.
      if ( nParam == 0 )
         strInsertBuffer += "void";

      strInsertBuffer += " )\r\n";
   }
   else
   {
      strInsertBuffer += " )\r\n\r\n";
   }

   if ( bFileC == FALSE )
   {
      switch ( *szType )
      {
         case 'E':
            //MovRight( 3 );
            strInsertBuffer += szTab;
            strInsertBuffer +=  "CASE ";
            strInsertBuffer +=  szParamCase;
            strInsertBuffer +=  "\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zECE_ACCEPT:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zECE_ACCEPT */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zECE_CANCEL:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zECE_CANCEL */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zECE_CREATE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zECE_CREATE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zECE_DELETE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zECE_DELETE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zECE_EXCLUDE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zECE_EXCLUDE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zECE_INCLUDE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zECE_INCLUDE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "END  /* case */\r\n";
            break;

         case 'C':
         case 'O':
            strInsertBuffer += szTab;
            strInsertBuffer +=  "CASE ";
            strInsertBuffer +=  szParamCase;
            strInsertBuffer +=  "\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zOCE_ACTIVATE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zOCE_ACTIVATE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zOCE_ACTIVATE_EMPTY:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zOCE_ACTIVATE_EMPTY */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zOCE_COMMIT:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zOCE_COMMIT */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zOCE_DROPOI:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zOCE_DROPOI */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "END  /* case */\r\n";
            break;

         case 'A':
            strInsertBuffer += szTab;
            strInsertBuffer +=  "CASE ";
            strInsertBuffer +=  szParamCase;
            strInsertBuffer +=  "\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDERIVED_GET:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDERIVED_GET */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDERIVED_SET:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDERIVED_SET */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "END  /* case */\r\n";
            break;

         case 'M':
            strInsertBuffer += szTab;
            strInsertBuffer +=  "CASE ";
            strInsertBuffer +=  szParamCase;
            strInsertBuffer +=  "\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_SET_ATTRIBUTE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_SET_ATTRIBUTE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_GET_VARIABLE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_GET_VARIABLE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_COMPARE_ATTRIBUTE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_COMPARE_ATTRIBUTE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_GET_FIRST_TBL_ENT_FOR_ATTR:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_GET_FIRST_TBL_ENT_FOR_ATTR */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_GET_NEXT_TBL_ENT_FOR_ATTR:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_GET_NEXT_TBL_ENT_FOR_ATTR */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_SET_ATTRIBUTE_VALUE_NEXT:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_SET_ATTRIBUTE_VALUE_NEXT */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_SET_ATTRIBUTE_VALUE_PREV:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_SET_ATTRIBUTE_VALUE_PREV */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_ADD_TO_ATTRIBUTE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_ADD_TO_ATTRIBUTE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_GET_COMPARE_VALUE:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_GET_COMPARE_VALUE */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "OF   zDME_VALIDATE_LPDATA:\r\n\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "     /* end zDME_VALIDATE_LPDATA */\r\n";
            strInsertBuffer += szTab;
            strInsertBuffer +=  "END  /* case */\r\n";
            break;

         case 'T':
         case 'G':
         case 'D':
         case 'L':
         default:
            strInsertBuffer +=  "\r\n";
            break;
      }
   }

   if ( bFileC )
   {
      strcpy_s( szLine, sizeof( szLine ), "{\r\n\r\n\r\n" );
      if ( *szReturnDataType == 'O' ||
         *szReturnDataType == 0 ) // empty data type handled as void
         strcat_s( szLine, sizeof( szLine ), "\treturn;\r\n" );
      else
         strcat_s( szLine, sizeof( szLine ), "\treturn( 0 );\r\n" );

      strcat_s( szLine, sizeof( szLine ), "} // ");
      strcat_s( szLine, sizeof( szLine ), szOperName );
      strcat_s( szLine, sizeof( szLine ), "\r\n" );
   }
   else
   {
      strcpy_s( szLine, sizeof( szLine ), "\r\nEND\r\n" );
   }

   strInsertBuffer += szLine;

   // insert a single string
   BufInsertStr( oEditor, strInsertBuffer );
   if ( bFileC )
      BufInsertStr( oEditor, "\r\n\r\n" );

   if ( vLOD )
      DropView( vLOD );

   return( 0 );
} // OperTemplate

// Operation: fnCursorInCommentOrQuote
// Returns:  TRUE if index is in comment or quotes.
zSHORT
fnCursorInCommentOrQuote( ZDrTBEdt *oEditor, long lIndex )
{
   CString strBuffer;
   LPSTR  szBuffer;
   zLONG  lReturnedBuffSize = MAX_TOKEN_LTH;
   zLONG  lCol, lLine;
   zBOOL  bReturn = FALSE;

   bReturn = oEditor->IsCommentAtIndex( lIndex );

   // If cursor is not in a comment, check to see if it is in quotes.
   if ( bReturn == FALSE )
   {
      // Read line.
      szBuffer = strBuffer.GetBufferSetLength( lReturnedBuffSize );
      lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer,
                                                           lReturnedBuffSize,
                                                           lIndex );
      // in Case of an editor error
      if ( lReturnedBuffSize >= MAX_TOKEN_LTH )
      {
         lReturnedBuffSize++;
         szBuffer = strBuffer.GetBufferSetLength( lReturnedBuffSize );
         lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer,
                                                              lReturnedBuffSize,
                                                              lIndex );
      }

      strBuffer.ReleaseBuffer( );

      // Check to see if the original cursor position is inside quotes.  To
      // do this, we first go to the beginning of the line and then start
      // counting quotes until we get to the original cursor position.
      // Once we've done this, if the number of quotes is odd, then the
      // cursor is in quotes, otherwise it isn't.  This assumes that quoted
      // strings must be contained on one line e.g. they cannot be continued
      // on to multiple lines.

      // Reset the charactercount (lCol) relative to the begin of line.
      oEditor->GetPositionByIndex( lIndex, &lLine, &lCol );

      // Set bReturn to false.  bReturn is an indicator that
      // specifies whether the last quote found is an open quote.
      bReturn = FALSE;
      for ( long l = 0;l < lReturnedBuffSize; l++ )
      {
         // If quote is before the original cursor position, then toggle
         // bReturn, otherwise break out of loop.
         if ( strBuffer.GetAt( l ) == '\"' )
            if ( l <= lCol )
               bReturn = !bReturn;
            else
               break;
      }
   }

   return( bReturn );

} // fnCursorInCommentOrQuote

// We're about to open a sub-window.  We need to name the subtask view for
// the the current editor so that the subwindow can retrieve it.
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_OpenSubwindow( zVIEW vSubtask )
{
   zVIEW vEdWrk;

   mGetWorkView( &vEdWrk, vSubtask );
   SetNameForView( vEdWrk, "__EditorSubtask", vSubtask, zLEVEL_TASK );
   return( 0 );
}

int
fnShowSubwindows( zVIEW  vSubtask )
{
   zSHORT nRC;
   zVIEW  vProfileXFER;

   // Call dialog op to indicate we're about to open some subwindows.
   TZEDFRMD_OpenSubwindow( vSubtask );

   mGetProfileView( &vProfileXFER );
   if ( vProfileXFER )
   {
      for ( nRC = SetCursorFirstEntity( vProfileXFER, szlSubwindow, 0 );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vProfileXFER, szlSubwindow, 0 ) )
      {
         zPCHAR pszWindowName;

         if ( CompareAttributeToString( vProfileXFER, szlSubwindow,
                                        szlSubwindowActive, "Y" ) != 0 )
            continue;

         GetAddrForAttribute( &pszWindowName, vProfileXFER, szlSubwindow, szlName );
         SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow |
                                  zWAB_ProcessImmediateAction, szlTZEDFRMD,
                                  pszWindowName );
      }
   }

   // set Focus to Editor Window
   SetWindowState( vSubtask, zWINDOW_STATUS_SETFOCUS, TRUE );

   return( 0 );

} // fnShowSubwindows


static zSHORT
CreateFileInfo( ZDrTBEdt *oEditor,
                zPCHAR   pszName )

{
   zCHAR    szDateRaw[ 20 ];
   zCHAR    szDateFormatted[ 20 ];

   SysGetDateTime( szDateRaw, sizeof( szDateRaw ) );
   UfFormatDateTime( szDateFormatted, sizeof( szDateFormatted ), szDateRaw, "DD-MM-YYYY" );

   BufInsertStr( oEditor,
     "/*\r\n"
     "//----------------------------------------------------------------------\r\n"
     "//\r\n"
     "// .Name:         " );
   BufInsertStr( oEditor, pszName );
   BufInsertStr( oEditor, ".c\r\n"
     "//\r\n"
     "// .Version:      1.0\r\n"
     "//\r\n"
     "// .Last change:  " );
   BufInsertStr( oEditor, szDateFormatted );
   BufInsertStr( oEditor, "\r\n"
     "//\r\n"
     "// .Description:  Zeidon operations\r\n"
     "//\r\n"
     "//----------------------------------------------------------------------\r\n"
     "// .Change notes\r\n"
     "//\r\n"
     "//  1.0  = New\r\n"
     "//        (" );
   BufInsertStr( oEditor, szDateFormatted );
   BufInsertStr( oEditor, ") Zeidon\r\n"
     "//\r\n"
     "//----------------------------------------------------------------------\r\n"
     "*/\r\n\r\n" );

   BufInsertStr( oEditor,
     "#define KZSYSSVC_INCL\r\n"
     "#include <KZOENGAA.H>\r\n"
     "#include <ZDRVROPR.H>\r\n"
     "#ifdef __cplusplus\r\n"
     "extern \"C\"\r\n"
     "{\r\n"
     "#endif\r\n"
     "#include \"ZEIDONOP.H\"\r\n\r\n"
     "#ifdef __cplusplus\r\n"
     "}\r\n"
     "#endif\r\n"
     "\r\n\r\n" );

   return( 0 );
} // CreateFileInfo

static zSHORT
CreateSourceFile( zVIEW    vSubtask,
                  zVIEW    vSource,
                  ZDrTBEdt *oEditor,
                  zBOOL    bCFile,
                  zPCHAR   szSourceFileEntityName )
{
   zVIEW    vTmp;
   zPCHAR   pszName;
   zLONG    lLine, lCol, lIndex;
   zSHORT   nRC;

   oEditor->SetCursorPositionByLine( 0, 0 );

   if ( bCFile )
   {
      // Create a "C" source header.
      GetAddrForAttribute( &pszName, vSource, szSourceFileEntityName, szlName );

      CreateFileInfo( oEditor, pszName );

      // In c-files search for #ifdef __cplusplus (from end to begin)
      // to get the position where to insert new operation.
      static zCHAR szSearchString[] = "\\bifdef __cplusplus[^!-~°§²³´ßäöüÄÖÜ]+";

      // Set cursor to the last line.
      MovEOF( );
      oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );

      oEditor->FindTextPosition( szSearchString, &lIndex, SRCH_BACKWARD | SRCH_REGEXPR );

      // Something found so insert two blank lines and reposition the cursor
      // to one of the blank lines.
      if ( lIndex > -1 )
      {
         // Set cursor to begin of line
         oEditor->GetPositionByIndex( lIndex, &lLine, &lCol );
         oEditor->SetCursorPositionByLine( lLine, 0 );
         BufInsertStr( oEditor, "\r\n\r\n" );
         oEditor->GetCursorPosition( &lLine, &lCol, &lIndex);
         oEditor->SetCursorPositionByIndex( lIndex - 1 );
      }

   }

   CreateViewFromViewForTask( &vTmp, vSource, 0 );
   for ( nRC = SetCursorFirstEntity( vTmp, szlOperation, "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTmp, szlOperation, "" ) )
   {
      OperTemplate( oEditor, vTmp, vSubtask );
      // reposition cursor for next insert
      if ( bCFile )
      {
         oEditor->GetLineCount( &lIndex );
         oEditor->SetCursorPositionByLine( lIndex, 0 );
         oEditor->InsertItem( "\r\n\r\n" );
         oEditor->GetLineCount( &lIndex );
         oEditor->SetCursorPositionByLine( lIndex, 0 );
      }

   } // End for each Operation

   DropView( vTmp );
   return( 0 );

} // CreateSourceFile

static zSHORT
CreateErrorMessage( zVIEW  vSubtask,
                    zPCHAR szMsgText )
{
   zVIEW      vProfileXFER;
   zPCHAR     pszInvokingTool;
   zCHAR      szComponentType[ 33 ];
   zCHAR      szMsg[ 100 ];

   mGetProfileView( &vProfileXFER );

   GetAddrForAttribute( &pszInvokingTool, vProfileXFER, szlED, "InvokingTool" );

   if ( zstrcmp( pszInvokingTool, "GO" ) == 0 )
      strcpy_s( szComponentType, sizeof( szComponentType ), "Global Operation Group" );
   else
   if ( zstrcmp( pszInvokingTool, "DM" ) == 0 )
      strcpy_s( szComponentType, sizeof( szComponentType ), "Domain Group" );
   else
   if ( zstrcmp( pszInvokingTool, "ZO" ) == 0 )
      strcpy_s( szComponentType, sizeof( szComponentType ), "LOD" );
   else
      strcpy_s( szComponentType, sizeof( szComponentType ), "Dialog" );

   strcpy_s( szMsg, sizeof( szMsg ), "Cannot create " );
   strcat_s( szMsg, sizeof( szMsg ), szMsgText );
   strcat_s( szMsg, sizeof( szMsg ), ", because " );
   strcat_s( szMsg, sizeof( szMsg ), szComponentType );
   strcat_s( szMsg, sizeof( szMsg ), " is not checked out." );

   MessagePrompt( vSubtask, "ED0003", szlZeidonEditor,
                  szMsg,
                  FALSE, zBUTTONS_OK, 0, zICON_EXCLAMATION );
   return( 0 );
}

static zSHORT
GotoCurrentOperation( zVIEW    vSubtask,
                      zVIEW    vSource,
                      ZDrTBEdt *oEditor,
                      zPCHAR   szOperSrch,
                      zPCHAR   szMetaName,
                      zBOOL    bCFile )
{
   zLONG  lIndex = 0;
   zLONG  lLine, lCol;
   long   lTBEDTDefaultSearchBehavior = 5;

   oEditor->SetCursorPositionByIndex( lIndex );
   oEditor->FindTextPosition( szOperSrch, &lIndex, lTBEDTDefaultSearchBehavior );
   oEditor->SetCursorPositionByIndex( lIndex );
   // TraceLineI( "(xxx) Return value from search = ", lIndex );

   // Skipping lines where the search text is part of a comment
   while (oEditor->IsCommentAtIndex(lIndex) && lIndex > -1) // 1998.10.15  TMV check lIndex
   {
      lIndex++;
      oEditor->SetCursorPositionByIndex( lIndex );
      oEditor->FindTextPosition( szOperSrch, &lIndex, lTBEDTDefaultSearchBehavior );
      oEditor->SetCursorPositionByIndex( lIndex );
   }

   // New operation code?
   if ( lIndex == -1 )
   {
      // set cursor to the last line
      MovEOF( );
      if ( bCFile )
      {
         // in c-files search for #ifdef __cplusplus (from end to begin)
         // to get the position where to insert new operation
         GetPositionForNextInsert( oEditor, &lIndex );
         oEditor->SetCursorPositionByIndex( lIndex );
      }

      // if file read only, do not create operation
      if ( oEditor->IsReadOnly( ) )
         CreateErrorMessage( vSubtask, "Operation" );
      else
      {
         OperTemplate( oEditor, vSource, vSubtask );
         MovUp( 3 );
      }
   }
   else
   {
      // Find end of parm list.
      oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );
      // Move cursor to beginning of next line.
      oEditor->SetCursorPositionByLine( lLine + 1, 3 );
   }

   oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );
   zsprintf( szMetaName, "(%ld,%ld)", lLine + 1, lCol + 1 );

   MB_SetMessage( vSubtask, LINECOL_DIL, szMetaName );
   return( 0 );

} //GotoCurrentOperation

static zSHORT
SetTargetExecutableName( zVIEW  vSource,
                         zVIEW  vEdWrk,
                         zPCHAR pszInvokingTool,
                         zPCHAR szMetaName,
                         zPLONG plMetaType )
{
   // Global Operations
   if ( zstrcmp( pszInvokingTool, "GO" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "GlobalOperationGroup",
                                 "DomainAndGlobalOpGroupName" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_GOPGRP_META );
      *plMetaType = zSOURCE_GOPGRP_META;
      GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vSource, "GlobalOperationGroup", "Name" );
   }
   else
   // Domains
   if ( zstrcmp( pszInvokingTool, "DM" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "DomainGroup", "DomainAndGlobalOpGroupName" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_DOMAINGRP_META );
      *plMetaType = zSOURCE_DOMAINGRP_META;
      GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vSource, "DomainGroup", "Name" );
   }
   else
   // LOD's
   if ( zstrcmp( pszInvokingTool, "ZO" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "LOD", "DLL_Name" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_LOD_META );
      *plMetaType = zSOURCE_LOD_META;
      GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vSource, "LOD", "Name" );
   }
   else
   if ( zstrcmp( pszInvokingTool, "WD" ) == 0 ||
        zstrcmp( pszInvokingTool, "PN" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, szlSourceFileName,
                                 vSource, szlSourceFile, szlName );

      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "Dialog", "DLL_Name" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_DIALOG_META );

      // If there is no DLL name for the window, then assume the DLL name is
      // the window name.
      if ( CompareAttributeToString( vEdWrk, szlBuffer, "TargetExecutableName",
                                     "" ) == 0 )
      {
         SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                    vSource, "Dialog", "Tag" );
      }

      *plMetaType = zSOURCE_DIALOG_META;
      GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vSource, "Dialog", "Tag" );
   }

   return( 0 );

} // SetTargetExecutableName

static zSHORT
CheckFileAttribute( zPCHAR  pchFileName,
                    zPBOOL  bReadOnly )
{

   DWORD dwAttribute = ::GetFileAttributes(pchFileName);
   if (dwAttribute == 0xFFFFFFFF) //call of function GetFileAttributes failed
   {
      LPSTR lpMsgBuf = NULL;
      CString strMsgTitle = "Zeidon";
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError( ),
                     MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                                 lpMsgBuf, 0, NULL );
      MessageBox( NULL, (LPCSTR) lpMsgBuf, strMsgTitle,
                  MB_OK | MB_ICONINFORMATION );

      // Free the buffer.
      LocalFree( lpMsgBuf );
      *bReadOnly = FALSE;
   }
   else
   {
      if ( dwAttribute & FILE_ATTRIBUTE_READONLY )
         *bReadOnly = TRUE;
   }

   return( 0 );

} // CheckFileAttribute

static zSHORT
CreateFileName( zVIEW   vTaskLPLR,
                zVIEW   vSource,
                zPCHAR  pchFileName,
                zLONG   lMaxLth,
                zPCHAR  pszInvokingTool,
                zPCHAR  szSourceFileEntityName,
                zPBOOL  pbFileExists )
{
   zCHAR szFileName[ zMAX_FILENAME_LTH + 1 ];

   //
   // Create the name of the file.
   //
   if ( zstrcmp( pszInvokingTool, "GO" ) == 0 )
      strcpy_s( szSourceFileEntityName, sizeof( szSourceFileEntityName ), "GlobalOperationGroup" );
   else
   if ( zstrcmp( pszInvokingTool, "DM" ) == 0 )
      strcpy_s( szSourceFileEntityName, sizeof( szSourceFileEntityName ), "DomainGroup" );
   else
      strcpy_s( szSourceFileEntityName, sizeof( szSourceFileEntityName ), szlSourceFile );

   // Get the source dir from the lplr view.
   GetStringFromAttribute( pchFileName, lMaxLth, vTaskLPLR, szlLPLR, szlPgmSrcDir );
   SysAppendcDirSep( pchFileName );

   // Get the subdir from the source file entity.  If GetString... returns < 0
   // then the subdir attribute is null, so nothing was retrieved.  If subdir
   // is set, then we need to add a '\'.
   if ( GetStringFromAttribute( pchFileName + zstrlen( pchFileName ), lMaxLth - zstrlen( pchFileName ),
                                vSource, szSourceFileEntityName, szlSubdirectory ) >= 0 )
   {
      SysAppendcDirSep( pchFileName );
   }

   // Get the file name and extension.
   GetStringFromAttribute( pchFileName + zstrlen( pchFileName ), lMaxLth - zstrlen( pchFileName ),
                           vSource, szSourceFileEntityName, szlName );
   strcat_s( pchFileName, zMAX_FILENAME_LTH + 1, "." );
   GetStringFromAttribute( pchFileName + zstrlen( pchFileName ), lMaxLth - zstrlen( pchFileName ),
                           vSource, szSourceFileEntityName, szlExtension );
   strcpy_s( szFileName, sizeof( szFileName ), pchFileName );
   SysConvertEnvironmentString( pchFileName, lMaxLth, szFileName );

   if ( SysOpenFile( vTaskLPLR, pchFileName, COREFILE_EXIST ) == -1 )
   {
      zLONG hFile;

      // Create an empty file.
      *pbFileExists = FALSE;
      if ( !MiGetUpdateForView( vSource ) )
         return( -1 );
      else
      {
         hFile = SysOpenFile( vTaskLPLR, pchFileName, COREFILE_CREATE );
         SysCloseFile( vTaskLPLR, hFile, 0 );
      }
   }
   else
      *pbFileExists = TRUE;

   return( 0 );

} // CreateFileName

static zSHORT
SetEditorStatus( ZDrTBEdt *oEditor,
                 zVIEW    vSource,
                 zVIEW    vEdWrk )
{
   if ( MiGetUpdateForView( vSource ) )
   {
      // Source view is updateable
      SetAttributeFromString( vEdWrk, szlBuffer, szlActiveStatus, "Y" );
      // set ReadOnly off;
      oEditor->ReadOnlyModus( FALSE );
   }
   else
   {
      // Source view is NOT updateable
      SetAttributeFromString( vEdWrk, szlBuffer, szlActiveStatus, "N" );
      // ReadOnly on;
      oEditor->ReadOnlyModus( TRUE );
   }

   return( 0 );
}

// Sets the line,col DIL message.  Since the line,col values are 0-based we
// add 1 before displaying them.
// Also checks to see if the file's been changed.
zSHORT
InitSession( zVIEW  vSubtask )
{
   zPCHAR   lpszOperName = 0;
   zPCHAR   pszInvokingTool = 0;
   zCHAR    szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR    szSourceFileEntityName[ 33 ];
   zCHAR    szOperSrch[ 256 ];
   zLONG    lMetaType = 0;
   zCHAR    szMetaName[ 33 ];
   zBOOL    pbFileExists = FALSE;
   zBOOL    bCFile = FALSE;
   zVIEW    vEdWrk=0;
   zVIEW    vSource = 0;
   zVIEW    vTaskLPLR = 0;
   zVIEW    vProfileXFER = 0;
   zLONG    lIndex = -1;
   zBOOL    bReadOnly = FALSE;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   // TMV 1998.05.26 registry problem
   // Don't know if this is the best way to do this, but it's better than an access violation.
   if ( oEditor == 0 )
      return( -1 );

   // Flag which says that the Object is created by Menu-Command 'File New'.
   g_bIsFileNew = FALSE;

   oEditor->WantKeystrokes( TRUE );
   mGetWorkView( &vEdWrk, vSubtask );
   mGetProfileView( &vProfileXFER );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetAddrForAttribute( &pszInvokingTool, vProfileXFER, szlED, "InvokingTool" );

   // Retrieve the source file name and open a buffer.
   GetViewByName( &vSource, "METAOPERATIONS", vSubtask, zLEVEL_TASK );
   if ( vSource )
   {
      // Clean up a little by dropping the name.
      DropNameForView( vSource, "METAOPERATIONS", vSubtask, zLEVEL_TASK );
      if ( CreateFileName( vTaskLPLR, vSource, szFileName, sizeof( szFileName ), pszInvokingTool,
                           szSourceFileEntityName, &pbFileExists ) < 0 )
      {
         CreateErrorMessage( vSubtask, "Source File" );
         return( -1 );
      }
   }
   else
   {
      // If there is no vSource, the editor is invoked by another editor instance.
      // There is no meta view so we must be opening the current file using
      // the Open/File menu command.  Get the file name.
      GetStringFromAttribute( szFileName, sizeof( szFileName ), vEdWrk, szlEditor, "OpenFileName" );
      if ( CompareAttributeToString( vEdWrk, szlEditor,
                                     "OpenReadOnly", "Y" ) == 0 )
      {
         bReadOnly = TRUE;
      }

      SetAttributeFromString( vEdWrk, szlEditor, "OpenReadOnly", "" );
   }

   // Check whether the file is marked as readonly.
   CheckFileAttribute( szFileName, &bReadOnly );

   CreateEntity( vEdWrk, szlBuffer, zPOS_LAST );
   SetAttributeFromString( vEdWrk, szlBuffer, szlFileName, szFileName );
   SetMatchingAttributesByName( vEdWrk, szlBuffer, vProfileXFER, "ED", zSET_ALL );

   oEditor->OpenObject( szFileName );
   if ( bReadOnly )
   {
      // Set file read only.
      oEditor->ReadOnlyModus( bReadOnly );
      SetAttributeFromString( vEdWrk, szlBuffer, szlReadOnly, "Y" );
      MB_SetMessage( vSubtask, MAIN_DIL, "File is write protected");
   }

   // If vSource is 0 then we must have been started via a File/Open
   // command and therefore we don't have a meta.  We've done all we
   // need to do ... so exit.
   if ( vSource == 0 )
   {
      // Extension checking is the only way to find out whether syntax
      // coloring can occur.
      CString strExtension = szFileName;
      int iPoint = strExtension.ReverseFind( '.' );
      CString strHelp = strExtension.Mid( iPoint + 1 );
      if ( strHelp.CompareNoCase( "VML" ) == 0 )
      {
         bCFile = FALSE;
         oEditor->ZeidonSyntaxOn( );
      }
      else
      {
         bCFile = TRUE;
         oEditor->SyntaxOff( );  // no syntax coloring in C-Sources
      }

      return( 0 );
   }

   SetAttributeFromInteger( vEdWrk, szlBuffer, szlSourceViewID, (zLONG) vSource );

   // Set the target executable name from the source meta.
   // Also set the MetaType and MetaName to be used further on.
   SetTargetExecutableName( vSource, vEdWrk, pszInvokingTool,
                            szMetaName, &lMetaType );

   // If the source meta view is not updateable, then make sure that the edit
   // buffer is not updateable.  There is a chance that the souce meta
   // properties conflict with the readonly attribute setting of the file
   // itself (checked above).
   if ( bReadOnly == FALSE )
      SetEditorStatus( oEditor, vSource, vEdWrk );

   // Set the LanguageType for the current buffer.
   SetAttributeFromAttribute( vEdWrk, szlBuffer, szlLanguageType,
                              vSource, szSourceFileEntityName, szlLanguageType );

   // If source file is of type "C" ...
   if  ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlC_File ) != 0 )
   {
      bCFile = FALSE;
      oEditor->ZeidonSyntaxOn( );
   }
   else
   {
      bCFile = TRUE;
      oEditor->SyntaxOff( );  // no syntax coloring in C-Sources
   }

   // If the file did not previously exist or if its empty then init
   // all operations in Meta.
   oEditor->GetLineCount( &lIndex );
   if ( pbFileExists == FALSE || lIndex < 1 )
      CreateSourceFile( vSubtask, vSource, oEditor, bCFile, szSourceFileEntityName );

   // Determine if the operation is in the file or we should create a template
   GetAddrForAttribute( &lpszOperName, vSource, szlOperation, szlName );
   zsprintf( szOperSrch, "\\bOPERATION[^!-~°§²³´ßäöüÄÖÜ]+%s[^!-~°§²³´ßäöüÄÖÜ]*(", lpszOperName );

   GotoCurrentOperation( vSubtask, vSource, oEditor, szOperSrch, szMetaName, bCFile );
   return( 0 );

} // InitSession

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_ZeidonMenuSelected( zVIEW vSubtask )
{
   zVIEW      vEdWrk;

   mGetWorkView( &vEdWrk, vSubtask );

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  szlLanguageType, szlVML_File ) == 0 )
   {
      // We're editing a VML file so enable certain items.
      SetOptionState( vSubtask, "ParseVML", zCONTROL_STATUS_ENABLED, TRUE );
      SetOptionState( vSubtask, "GenerateC",  zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      // We're editing something that is not a VML file so disable items.
      SetOptionState( vSubtask, "ParseVML", zCONTROL_STATUS_ENABLED, FALSE );
      SetOptionState( vSubtask, "GenerateC", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_InitMenu( zVIEW vSubtask )
{
   zVIEW    vEdWrk;
   zBOOL    bEnable   = FALSE;
   zBOOL    bReadOnly = TRUE;
   ZDrTBEdt *oEditor =
              (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   mGetWorkView( &vEdWrk, vSubtask );

   if ( oEditor->IsReadOnly( ) )
   {
      bReadOnly = FALSE;
   }

   BOOL bCanCopy  = oEditor->CanCopy( );
   BOOL bCanPaste = oEditor->CanPaste( );
   BOOL bCanUndo  = oEditor->CanUndo( );
   BOOL bCanRedo  = oEditor->CanRedo( );

   SetOptionState( vSubtask, "SaveFile", zCONTROL_STATUS_ENABLED, bReadOnly );
   SetOptionState( vSubtask, "EditReplace", zCONTROL_STATUS_ENABLED, bReadOnly );

   SetOptionState( vSubtask, "EditCopy", zCONTROL_STATUS_ENABLED, bCanCopy );
   SetOptionState( vSubtask, "EditUndo", zCONTROL_STATUS_ENABLED, bReadOnly ? bCanUndo : FALSE );
   SetOptionState( vSubtask, "EditRedo", zCONTROL_STATUS_ENABLED, bReadOnly ? bCanRedo : FALSE );
   SetOptionState( vSubtask, "EditCut",  zCONTROL_STATUS_ENABLED, bReadOnly ? bCanCopy : FALSE );
   SetOptionState( vSubtask, "EditPaste", zCONTROL_STATUS_ENABLED, bReadOnly ? bCanPaste : FALSE );
   SetOptionState( vSubtask, "EditDelete", zCONTROL_STATUS_ENABLED, bReadOnly ? bCanCopy : FALSE );
   SetOptionState( vSubtask, "EditSelectAll", zCONTROL_STATUS_ENABLED, !FileIsEmpty( oEditor ) );

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) == 0 )
   {
      bEnable = TRUE;
   }

   SetOptionState( vSubtask, "ParseVML", zCONTROL_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "GenerateC", zCONTROL_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "EntityInsert", zCONTROL_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "KeyWordInsert", zCONTROL_STATUS_ENABLED, bEnable );

   return( 0 );
}

// When the editor window gets focus we need to do a few things:
// o  Check to see if the editor work view is available yet.  If it's not then
//    skip the rest of the function.
// o  Lastly, set focus to the editor control.
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_ReceiveFocus( zVIEW vSubtask )
{
#if 1
   zVIEW    vEdWrk;
   ZDrTBEdt *oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
      return( 0 );

   if ( oEditor )
      SetFocusToCtrl( vSubtask, EDIT_CONTROL_NAME );
#endif
   return( 0 );
}

// The user just typed a space.  Check to see if we should perform command
// completion.
// Returns: TRUE  - Command completion performed.
//          FALSE - Nothing done.
//
zSHORT LOCALOPER
fnCommandCompletion( zVIEW vSubtask )
{
   zVIEW    vEdWrk;
   zVIEW    vProfileXFER;
   zLONG    lLine, lCol, lIndex;
   zLONG    lLth;
   zCHAR    szBuffer[ 300 ];
   zPCHAR   pszToken;
   zCHAR    szFileName[ MAX_PATH + 1 ];
   CString  strHelp;
   ZDrTBEdt *oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   mGetWorkView( &vEdWrk, vSubtask );

   // If current File is a C-Source then don't do command completion.
   zmemset( szFileName, 0, MAX_PATH + 1);
   zmemset( szFileName, ' ', MAX_PATH );
   oEditor->GetObjectName( szFileName );

   CString strExtension = szFileName;
   int iPoint = strExtension.ReverseFind( '.' );
   strHelp = strExtension.Mid( iPoint + 1 );
   if ( strHelp.CompareNoCase( "VML" ) != 0 )
   {
      return( FALSE );
   }

   oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );

   // No cc if we are in the first col.
   if ( lCol == 0 )
   {
      return( FALSE );
   }

   // If we're in a comment or quote then don't do command completion.
   if ( fnCursorInCommentOrQuote( oEditor, lIndex ) )
      return( FALSE );

   zmemset( szBuffer, 0, sizeof( szBuffer ) );
   lLth = oEditor->GetActualTextLine( szBuffer, sizeof( szBuffer ) - 1 );

   // If the previous character to the space that was just entered is also a
   // space then skip command completion.
   if ( lLth < 2 || isspace( szBuffer[ lCol - 1 ]) )
      return( FALSE );

   // Check to see if anything comes after the space.  If there is and it's
   // not a white space then skip CC.
   if ( lLth > lCol + 1 )
   {
      for ( pszToken = &szBuffer[ lCol + 1 ]; *pszToken; pszToken++ )
      {
         if ( isspace( *pszToken ) == FALSE )
            return( FALSE );
      }
   }

   // Change the ' ' to a null-term.
   szBuffer[ lCol ] = 0;

   // Find the last token before what was the space.
   for ( pszToken = &szBuffer[ lCol - 1 ];
         !isspace( *pszToken ) && pszToken >= szBuffer;
         pszToken--)
   {
      // Nothing needs to be done here.
   }

   // Set pszToken to point to the first zCHAR of the token.
   pszToken++;

   // Create a temp view so we don't mess up cursors.
   mGetProfileView( &vProfileXFER );
   CreateViewFromViewForTask( &vProfileXFER, vProfileXFER, 0 );

   // Now try to find the token in the list of commands.
   if ( SetCursorFirstEntityByString( vProfileXFER, "VML_Text", "CC_Code",
                                      pszToken, 0 ) != zCURSOR_SET )
   {
      DropView( vProfileXFER );     // Drop temp copy of vProfileXFER.
      return( FALSE );              // Code not found so exit.
   }

   // Check to see if we only perform CC for this token if it is the first token on the line.
   if ( CompareAttributeToString( vProfileXFER, "VML_Text", "CC_FirstChar", "Y" ) == 0 )
   {
      zPCHAR psz;

      // Scan the current line before the token to see if there is a non-space.
      // If there is then we don't perform CC so exit.
      for ( psz = pszToken - 1; psz >= szBuffer; psz-- )
      {
         if ( isspace( *psz ) == FALSE )
         {
            DropView( vProfileXFER );  // drop temp copy of vProfileXFER.
            return( FALSE );
         }
      }
   }

   // OK...we are about to do command completion so we need to remove the
   // token from the current line.
// oEditor->DeleteTextRange( lIndex - zstrlen( pszToken ), lIndex );
   strHelp = pszToken;
   oEditor->SelectItem( lLine + 1, strHelp );
// oEditor->SetCursorPositionByLine( lLine, lCol - zstrlen( pszToken ) );

   // Call operation to insert currently selected keyword in profile OI.
   fnInsertVML_Text( vSubtask, vEdWrk, vProfileXFER, oEditor );

   DropView( vProfileXFER );     // drop temp copy of vProfileXFER.
   return( TRUE );

} // fnCommandCompletion

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_Keystroke( zVIEW vSubtask )
{
   zCTRL_EVENT *pCE;
   zSHORT      nRC;
   zCHAR       cKey;
   zVIEW       vEdWrk;
   zVIEW       vProfileXFER;

   //TraceLineS( "(tzedfrmd)", "Keystroke" );

   mGetWorkView( &vEdWrk, vSubtask );
   mGetProfileView( &vProfileXFER );

   pCE = (zCTRL_EVENT *) GetActionParameters( vSubtask );
   cKey = (zCHAR) pCE->m_pEvent->m_pDispParams->rgvarg[ 1 ].iVal;

   // Check to see if the key is a space.  If it is then we might need to
   // perform command completion.
   if ( cKey == ' ' )
      nRC = fnCommandCompletion( vSubtask );
      pCE->m_ulRC = 0x00000001;

   pCE->m_ulRC = 0x00000001;
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_LineChange( zVIEW vSubtask )
{
   zCTRL_EVENT *pCE;
   zLONG       lLine, lCol;
   zCHAR       szData[ 30 ];
   ZDrTBEdt *oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

// TraceLineS( "(tzedfrmd)", "LineChange" );
   szData[ 0 ] = 0;
   pCE = (zCTRL_EVENT *) GetActionParameters( vSubtask );
   if ( pCE->m_pEvent->m_pDispParams->cArgs >= 2 )
   {
      // 1998.7.15   TMV
      // Get parameters in reverse order.
      // Refer to typelib for more information about the event interface
      // of the current control.
      lLine = pCE->m_pEvent->m_pDispParams->rgvarg[ 1 ].iVal;
      lCol  = pCE->m_pEvent->m_pDispParams->rgvarg[ 0 ].iVal;
      zsprintf( szData, "(%ld,%ld)", lLine, lCol);
   }

   MB_SetMessage( vSubtask, LINECOL_DIL, szData );

   if ( oEditor )
   {
      oEditor->IsObjectChanged( (BOOL *) &lLine );
      if ( lLine )
         strcpy_s( szData, sizeof( szData ), "*" );
      else
         strcpy_s( szData, sizeof( szData ), "" );
   }

   MB_SetMessage( vSubtask, FILECHANGED_DIL, szData );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PostBuild( zVIEW vSubtask )
{
   zVIEW   vTaskLPLR;
   zVIEW   vProfileXFER;
   zVIEW   vEdWrk;
   zCHAR   sz[300];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   mGetProfileView( &vProfileXFER );
   if ( vProfileXFER )
   {
      // Set a couple of attributes to NULL.  We don't need them anymore and
      // someday we'll want to get rid of them.
      SetAttributeFromString( vProfileXFER, "ED", "SearchFlags", "" );
      SetAttributeFromString( vProfileXFER, "ED", "FontString", "" );
   }

   // First thing - create an editor work object instance.
   GetViewByName( &vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_SUBTASK );

   if ( vEdWrk == 0 )
   {
      GetViewByName( &vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_TASK );
      if ( vEdWrk == 0 )
      {
         ActivateEmptyObjectInstance( &vEdWrk, szlTZEDWRKO, vTaskLPLR, zSINGLE );

         // Create empty root.
         CreateEntity( vEdWrk, "Editor", zPOS_FIRST );
         SetNameForView( vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_TASK );
      }

      CreateViewFromViewForTask( &vEdWrk, vEdWrk, 0 );
      SetNameForView( vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_SUBTASK );
   }

   // If InitSession returns a value of 1,the editor has been used for
   // dropping an operation from source code.  If so all the work is done.
   if ( InitSession( vSubtask ) == 0 )
   {
      SetAttributeFromInteger( vEdWrk, szlBuffer, "vSubtask", (zLONG) vSubtask );
      GetStringFromAttribute( sz, sizeof( sz ), vEdWrk, szlBuffer, szlFileName );
      // Set checked out state in title
      if ( CompareAttributeToString( vEdWrk, szlBuffer, szlActiveStatus, "N" ) == 0 )
         strcat_s( sz, sizeof( sz ), " < not checked out >" );
      else
      if ( CompareAttributeToString( vEdWrk, szlBuffer, szlReadOnly, "Y" ) == 0 )
         strcat_s( sz, sizeof( sz ), " < Read-Only mode >" );

      SetWindowCaptionTitle( vSubtask, "Zeidon Editor", sz );
      fnShowSubwindows( vSubtask );
   }
   else
   {
      // Operation dropped ... go back to the parent.
      DropView( vEdWrk );
      GetViewByName( &vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_TASK );
      if ( vEdWrk )
         DropView( vEdWrk );

      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, "", "" );
   }

   return( 0 );
}

static zSHORT /*LOCAL */
fnTZEDFRMD_SaveFile( zVIEW    vSubtask,
                     ZDrTBEdt *oEditor,
                     zPCHAR   pchFileName,
                     zLONG    lMaxLth,
                     zSHORT   nAskForParse )
{
   zULONG  ulZFlags   = 0;
   BOOL    bFileSaved = FALSE;

   if ( g_bIsFileNew )
   {
      ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT | zOFN_SAVEAS;

      if ( OperatorPromptForFile( vSubtask, pchFileName, zMAX_FILENAME_LTH + 1,
                                  "VML files|*.VML|Source Files|*.VML;*.C|"
                                  "All files|*.*||", 0, ulZFlags ) != 1 )
      {
         return( -1 );
      }

      bFileSaved = oEditor->SaveObjectAs( pchFileName );
      if ( bFileSaved == TRUE )
      {
         g_bIsFileNew = FALSE;
         SetWindowCaptionTitle( vSubtask, "Zeidon Editor", pchFileName);
      }
   }
   else
   {
      zVIEW vEdWrk;

      mGetWorkView( &vEdWrk, vSubtask );
      if ( vEdWrk == 0 )
      {
         MessagePrompt( vSubtask, "ED0001", szlZeidonEditor,
                        "TZEDWRKO not Available at SaveFile.", 1,
                        0, 0, zICON_EXCLAMATION );
         return( -1 );
      }

      bFileSaved = oEditor->SaveObject( );

      // Parse only valid for VML-Files.
      if ( nAskForParse == PARSE_FILE &&
           CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType,
                                     szlVML_File ) == 0 )
      {
         CString strMsg;

         GetStringFromAttribute( pchFileName, lMaxLth, vEdWrk, szlBuffer, szlFileName );
         strMsg.Format("Do you want to parse '%s' for C (Yes) or Java (No)?",pchFileName);
         zLONG lRC = OperatorPrompt( vSubtask, szlZeidonEditor, strMsg, TRUE,
                                     zBUTTONS_YESNOCANCEL, zRESPONSE_YES,
                                     zICON_EXCLAMATION );

         if ( lRC == zRESPONSE_YES )
         {
            if ( SaveAndParse( vSubtask, "C" ) != 0 )
               return( -1 );
         }
         else
         if ( lRC == zRESPONSE_NO )
         {
            if ( SaveAndParse( vSubtask, "Java" ) != 0 )
               return( -1 );
         }
      }
   } // endif ( g_bIsFileNew )

   if ( bFileSaved )
      MB_SetMessage( vSubtask, MAIN_DIL, "File saved." );
   else
      MB_SetMessage( vSubtask, MAIN_DIL, "File not changed." );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_AskForSaveWithParse
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
TZEDFRMD_AskForSaveWithParse( zVIEW vSubtask,
                              zSHORT nAskForParse )
{
   zVIEW    vEdWrk;
   BOOL     bObjectChanged = FALSE;
   zCHAR    szFileName[ zMAX_FILENAME_LTH + 1 ];
   zULONG   ulZFlags = 0;
   ZDrTBEdt *oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   mGetWorkView( &vEdWrk, vSubtask );

   // Check if file changed.
   oEditor->IsObjectChanged( &bObjectChanged );

   // In 9i there was a dialog which offers something more than this
   // Save & Parse, Save, Exit etc...
   if ( bObjectChanged == FALSE || oEditor->IsReadOnly( ) )
      return( 0 );

   zSHORT nRC = 0;
   zCHAR  szMsg[ zMAX_FILENAME_LTH + 200 ];

   if ( g_bIsFileNew == FALSE)
   {
      // Get current file name.
      GetStringFromAttribute( szFileName, sizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );
   }
   else
   {
      // This is a good name for a new source.
      strcpy_s( szFileName, sizeof( szFileName ), "Noname.vml" );
   }

   sprintf_s( szMsg, sizeof( szMsg ), "File '%s' has changed. Do you want to save it?",
             szFileName );
   nRC = OperatorPrompt( vSubtask, szlZeidonEditor, szMsg, TRUE,
                         zBUTTONS_YESNOCANCEL, zRESPONSE_YES,
                         zICON_EXCLAMATION );

   if ( nRC == zRESPONSE_CANCEL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   else
   if ( nRC == zRESPONSE_YES )
   {
      if ( fnTZEDFRMD_SaveFile( vSubtask, oEditor, szFileName, sizeof( szFileName ), nAskForParse ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      SetUpdateFlagInMeta( vSubtask );
   }

   return( 0 );
} // TZEDFRMD_AskForSaveWithParse

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: TZEDFRMD_AskForSave
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDFRMD_AskForSave( zVIEW vSubtask )
{
   zVIEW        vEdWrk;
   zVIEW        vSource;
   zVIEW        vMeta;
   zVIEW        vTaskLPLR;
   zLONG        lMetaZKey;
   zSHORT       nRC = zCALL_ERROR;
   LPVIEWOD     lpViewOD;
   LPVIEWENTITY lpViewEntity;


   if ( TZEDFRMD_AskForSaveWithParse( vSubtask, FILE_NO_PARSE ) < 0 )
      return( nRC );

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      return( nRC );

   // if no selection found, select current meta
   if ( CheckExistenceOfEntity( vTaskLPLR, "TargetExecutable" ) != zCURSOR_SET ||
        SetCursorFirstSelectedEntity( vTaskLPLR,
                                      "TargetExecutable", "" ) >= zCURSOR_SET )
   {
      return( 0 );
   }

   // get current Meta
   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
      return( nRC );

   // Get the Source Meta for the current buffer.
   GetIntegerFromAttribute( (zPLONG) &vSource, vEdWrk, szlBuffer, szlSourceViewID );

   if ( vSource == 0 ||
        CompareAttributeToString( vEdWrk, szlBuffer, szlMetaType, "" ) == 0 )
   {
      return( nRC );
   }

   CreateViewFromViewForTask( &vMeta, vSource, 0 );
   ResetView( vMeta );

   // Get Root Entity Name
   lpViewOD = (LPVIEWOD) MiGetViewEntityForView( vMeta, 0 );
   if ( !lpViewOD )
      return( nRC );

   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );

   GetIntegerFromAttribute( &lMetaZKey, vMeta, lpViewEntity->szName, "ZKey" );

   if ( SetCursorFirstEntityByInteger( vTaskLPLR, "Meta", "ZKey",
                                       lMetaZKey, "LPLR" ) >= zCURSOR_SET )
   {
      SetSelectStateOfEntity( vTaskLPLR, "TargetExecutable", TRUE );
   }

   DropView( vMeta );

   return( 0 );
} // TZEDFRMD_AskForSave

/////////////////////////////////////////////////////////////////////////////

zSHORT g_nTrace;

zOPER_EXPORT zSHORT OPERATION
PreBuild( zVIEW vSubtask )
{
   SetWindowState( vSubtask, zWINDOW_STATUS_VISIBLE, FALSE );
   g_nTrace = SetActionTrace( vSubtask, 0 );
   return( 0 );
} // PreBuild

zOPER_EXPORT zSHORT OPERATION
SystemClose( zVIEW vSubtask )
{
   zVIEW    vEdWrk = 0;
   zVIEW    vLastEdWrk = 0;
   zVIEW    vProfileXFER = 0;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   // Are there any Subwindows of the control which have to be closed.
   if ( oEditor->CloseSubWindow( ) )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   if ( TZEDFRMD_AskForSaveWithParse( vSubtask, PARSE_FILE ) < 0 )
      return( -1 );

   // Save the current options values in the editor to the profile.
   mGetProfileView( &vProfileXFER );
   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk )
   {
      if ( vProfileXFER )
         SetMatchingAttributesByName( vProfileXFER, "ED", vEdWrk, szlBuffer, zSET_ALL );

      // Since we're closing the editor delete the buffer entry.
      DeleteEntity( vEdWrk, "Buffer", zREPOS_NEXT );

      // If an other Editor open, do not drop vEdWrk
      GetViewByName( &vLastEdWrk, "__EditorSubtask", vSubtask, zLEVEL_TASK );
      if ( vLastEdWrk == 0 )
         DropView( vEdWrk );
   }

   if ( g_nTrace )
      SetActionTrace( vSubtask, g_nTrace );

   return( 0 );

} // SystemClose

// Save the current position of the subwindow in the Profile object.
zSHORT
fnSaveSubwindowPosition( zVIEW  vSubtask,
                         zPCHAR pszName )
{
   zSHORT nRC;
   zVIEW  vProfileXFER;
   zLONG  lPosX, nPosY;
   zVIEW  vEdWrk;
   zPCHAR pszWindowActive;

   // If we can't find the editor work view for the parent editor subtask
   // then we are shutting down the editor (as opposed to just closing the
   // subwindow).

   // Get the work view for the current subtask.
   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
   {
      // We're closing down the editor so set flag so that we the subwindow
      // comes back up when we open the next editor.
      pszWindowActive = "Y";
   }
   else
      pszWindowActive = "N";

   mGetProfileView( &vProfileXFER );
   if ( vProfileXFER )
   {
      nRC = SetCursorFirstEntityByString( vProfileXFER, szlSubwindow, szlName, pszName, "" );
      if ( nRC != zCURSOR_SET )
      {
         CreateEntity( vProfileXFER, szlSubwindow, zPOS_LAST );
         SetAttributeFromString( vProfileXFER, szlSubwindow, szlName, pszName );
      }
   }

   GetCtrlPosition( vSubtask, 0, &lPosX, &nPosY, TRUE );
   SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlPosX, lPosX );
   SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlPosY, nPosY );

   GetCtrlSize( vSubtask, 0, &lPosX, &nPosY, TRUE );
   SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlWidth, lPosX );
   SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlHeight, nPosY );

   SetAttributeFromString( vProfileXFER, szlSubwindow, szlSubwindowActive, pszWindowActive );
   return( 0 );
}

// Set the position of a subwindow determined by vSubtask.
// Return TRUE  if window was repositioned,
//        FALSE if window was not.
zSHORT
fnSetSubwindowPosition( zVIEW  vSubtask,
                        zPCHAR szName )
{
   zSHORT nRC;
   zVIEW  vProfileXFER;

   mGetProfileView( &vProfileXFER );
   nRC = SetCursorFirstEntityByString( vProfileXFER, szlSubwindow, szlName, szName, 0 );
   if ( nRC != zCURSOR_SET )
      return( FALSE );

// Currently handled by 32-bit driver.
#ifndef __WIN32__
   {
      zLONG  lPosX, lPosY;

      GetIntegerFromAttribute( &lPosX, vProfileXFER, szlSubwindow, szlPosX );
      GetIntegerFromAttribute( &lPosY, vProfileXFER, szlSubwindow, szlPosY );
      SetCtrlPosition( vSubtask, 0, lPosX, lPosY, TRUE );

      GetIntegerFromAttribute( &lPosX, vProfileXFER, szlSubwindow, szlWidth );
      GetIntegerFromAttribute( &lPosY, vProfileXFER, szlSubwindow, szlHeight );
      SetCtrlSize( vSubtask, 0, (zSHORT) lPosX, (zSHORT) lPosY, TRUE );

   }
#endif

   return( TRUE );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_Postbuild( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zBOOL  bReadOnly = TRUE;
   zVIEW  vParent;

   GetParentWindow( &vParent, vSubtask );
   if ( vParent )
   {
      mGetWorkView( &vEdWrk, vParent );

      if ( CompareAttributeToString( vEdWrk, szlBuffer, szlReadOnly, "Y" ) == 0 ||
           CompareAttributeToString( vEdWrk, szlBuffer, szlActiveStatus, "N" ) == 0 )
      {
         bReadOnly = FALSE;
      }
   }

   SetOptionState( vSubtask, "PasteObjectName", zCONTROL_STATUS_ENABLED, bReadOnly );
   SetOptionState( vSubtask, "PasteVOR_Name", zCONTROL_STATUS_ENABLED, bReadOnly );
   EnableAction( vSubtask, "PasteView", bReadOnly );
   EnableAction( vSubtask, "PasteEQ", bReadOnly );
   EnableAction( vSubtask, "PasteEQ_Quoted", bReadOnly );
   EnableAction( vSubtask, "PasteAQ", bReadOnly );
   EnableAction( vSubtask, "PasteAQ_Quoted", bReadOnly );

   // Get the subtask view for the parent editor window and give it another
   // name at the current subtask level so we can easily retrieve it.
   GetViewByName( &vEdWrk, "__EditorSubtask", vSubtask, zLEVEL_TASK );
   SetNameForView( vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_SUBTASK );

   fnSetSubwindowPosition( vSubtask, szlObjectSelAQ );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_Close( zVIEW vSubtask )
{
   fnSaveSubwindowPosition( vSubtask, szlObjectSelAQ );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_RefreshAEQ( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "ViewList" );
   RefreshCtrl( vSubtask, "EntityList" );
   RefreshCtrl( vSubtask, "AttributeList" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_RefreshAEQ_AttrList( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "AttributeList" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_ClearViews( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zSHORT nRC;

   mGetWorkView( &vEdWrk, vSubtask );
   nRC = SetCursorFirstEntity( vEdWrk, "View", "" );
   while ( nRC == zCURSOR_SET )
      nRC = DeleteEntity( vEdWrk, "View", zREPOS_NEXT );

   AEQ_RefreshAEQ( vSubtask );
   return( 0 );
}

// OPERATION:  fnGetNextTokenFromBuffer
// PURPOSE:    Reads the current line in the current buffer starting at the
//             current cursor position and returns the first token found.
//
zSHORT LOCALOPER
fnGetNextTokenFromBuffer( ZDrTBEdt *oEditor, LPSTR szToken, long lIndex)
{
   CString strLine;
   LPSTR   szBuffer;
   zLONG   lReturnedBuffSize = MAX_TOKEN_LTH;
   zLONG   lCol, lLine;

   CString strDelimiter = "\t,)( ";

   szToken[ 0 ] = 0;

   // Read line from Editor.
   szBuffer = strLine.GetBufferSetLength( lReturnedBuffSize );
   lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer, lReturnedBuffSize, lIndex );

   // In Case of an editor error (Buffer too small) ...
   if ( lReturnedBuffSize >= MAX_TOKEN_LTH )
   {
      lReturnedBuffSize++;
      strLine.ReleaseBuffer( );
      szBuffer = strLine.GetBufferSetLength( lReturnedBuffSize );
      lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer, lReturnedBuffSize, lIndex );
   }

   strLine.ReleaseBuffer( );

   // Since we retrieved the whole line we need to start where the cursor is.
   oEditor->GetPositionByIndex( lIndex, &lLine, &lCol );
   strLine = strLine.Mid( lCol, strLine.GetLength( ) - lCol );

   // Remove obsolete whitespaces.
   strLine.TrimLeft( );
   strLine.TrimRight( );

   int iTokenLength = strLine.FindOneOf( strDelimiter );
   if ( iTokenLength >= 0 )
   {
      strLine = strLine.Mid( 0, iTokenLength );
   }

   strcpy_s( szToken, sizeof( szToken ), strLine );
   return( TRUE );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_GetViews( zVIEW vSubtask )
{
   static zCHAR szOperationString[] = "\\bOPERATION[^!-~°§²³´ßäöüÄÖÜ]+";
   zSHORT   nRC;
   zSHORT   nViewType;
   zVIEW    vEdWrk;
   zVIEW    vVor;
   zVIEW    vTemp;
   zLONG    lIndex, lCol;
   zLONG    lOperationEndIndex = -1, lOperationStartIndex = -1;
   zLONG    lIndexAfterObjectName = 0;
   zCHAR    szObjectName[ MAX_TOKEN_LTH ];
   zCHAR    szVarName[ MAX_TOKEN_LTH ];
   zCHAR    sz[ 100 ];
   zPCHAR   szSearchString;
   ZDrTBEdt *oEditor = 0;

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
   {
      strcpy_s( sz, sizeof( sz ), "TZEDWRKO not Available at GetViews." );
      OperatorPrompt( 0, "ED - Error", sz, 0, zBUTTONS_OK, 0, 0 );
      return( zCALL_ERROR );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  szlLanguageType, szlVML_File ) != 0 )
   {
      strcpy_s( sz, sizeof( sz ), "'Get Views' only works on VML Files." );
      OperatorPrompt( 0, "ED - Error", sz, 0, zBUTTONS_OK, 0, 0 );
      return( zCALL_ERROR );
   }

   GetIntegerFromAttribute( (zPLONG) &vTemp, vEdWrk, szlBuffer, "vSubtask" );
   oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vTemp,
                                                      EDIT_CONTROL_NAME );

   // Find the end of the current operation by searching for the beginning
   // of the next operation.  Since we want to ignore any operations that
   // might be commented out, keep searching until we are not in a comment.

   oEditor->GetCursorPosition( &lOperationEndIndex,&lCol,&lIndex );
   lOperationEndIndex = lIndex;
   for (;;)
   {
      oEditor->FindTextPosition( szOperationString, &lOperationEndIndex,
                                 SRCH_FORWARD | SRCH_REGEXPR );
      if ( lOperationEndIndex == -1 )
      {
         // Operation not found, so set lLastLine to some ridiculously high
         //number (this number indicates the last line of the current op)
         // and stop searching
         lOperationEndIndex = 99999999;
         break;
      }

      if ( fnCursorInCommentOrQuote( oEditor, lOperationEndIndex ) )
         lOperationEndIndex ++;
      else
         break;
   }

   // From the end of the current operation, search back to the beginning
   // of current operation.
   lOperationStartIndex = lOperationEndIndex;
   do
   {
      oEditor->FindTextPosition( szOperationString, &lOperationStartIndex,
                                 SRCH_REGEXPR );
      if ( lOperationStartIndex == -1 )
      {
         strcpy_s( sz, sizeof( sz ), "Couldn't find beginning of operation" );
         OperatorPrompt( 0, szlZeidonEditor, sz, 0, zBUTTONS_OK, 0, 0 );
         return( zCALL_ERROR );
      }

   } while ( fnCursorInCommentOrQuote( oEditor, lOperationStartIndex ) );

   // Search for 'BASED ON LOD' and 'REGISTERED AS' and retrieve the object
   // name and view variable name.
   for ( nViewType = 0; nViewType < 2; nViewType++ )
   {
      // NOTE: If we go back to using regular expressions then we need to
      // change the logic for finding the variable name.  Look where we set
      // the cursor position for more.
      CString strFind;

      if ( nViewType == 0 )
      {  // searching for Based on lod
         szSearchString = "BASED ON LOD ";
         CString strTemp;
         BuildCaseSensitiveSearchString( "BASED", strTemp );
         strFind = "\\b";
         strFind = strFind + strTemp + "[^!-~°§²³´ßäöüÄÖÜ]+";
         BuildCaseSensitiveSearchString("ON", strTemp);
         strFind += "\\b";
         strFind = strFind + strTemp + "[^!-~°§²³´ßäöüÄÖÜ]+";
         BuildCaseSensitiveSearchString( "LOD", strTemp );
         strFind += "\\b";
         strFind = strFind + strTemp + "[^!-~°§²³´ßäöüÄÖÜ]+";
      }
      else
      if ( nViewType == 1 )
      {
         szSearchString = "REGISTERED AS ";    // see NOTE above.
         CString strTemp;
         BuildCaseSensitiveSearchString( "REGISTERED", strTemp );
         strFind = "\\b";
         strFind = strFind + strTemp + "[^!-~°§²³´ßäöüÄÖÜ]+";
         BuildCaseSensitiveSearchString( "AS", strTemp );
         strFind += "\\b";
         strFind = strFind + strTemp + "[^!-~°§²³´ßäöüÄÖÜ]+";
      }

      lIndex = lOperationStartIndex;

      // Search for view declaration and retrieve object names.
      for ( oEditor->FindTextPosition( strFind, &lIndex,
                                       SRCH_FORWARD | SRCH_REGEXPR );
            lIndex > -1;
            oEditor->FindTextPosition( strFind, &lIndex,
                                       SRCH_FORWARD | SRCH_REGEXPR ) )
      {
         // Stop searching once we have passed the last line of the current
         // operation.
         if ( lIndex > lOperationEndIndex)
            break;

         // If current string is commented or quoted, continue.
         if ( fnCursorInCommentOrQuote( oEditor, lIndex ) )
         {
            // Increment Index so we don't step into an endless loop
            lIndex++;
            continue;
         }

         lIndex = lIndex + zstrlen( szSearchString );
         fnGetNextTokenFromBuffer( oEditor, szObjectName, lIndex );

         // Get Index of first zCHAR after Object name.
         lIndexAfterObjectName = lIndex + zstrlen(szObjectName);

         // Find variable name.
         do
         {
            CString strFind, strTemp;
            // prepare for Casesensitive search
            BuildCaseSensitiveSearchString( "VIEW", strTemp );
            strFind = "\\b";
            strFind = strFind + strTemp + "[^!-~°§²³´ßäöüÄÖÜ]+";

            oEditor->FindTextPosition( strFind, &lIndex, SRCH_REGEXPR );

         } while ( fnCursorInCommentOrQuote( oEditor, lIndex ) );

         // The search doesn't set the cursor position so set it.
         lIndex = lIndex + zstrlen( "VIEW " );
         fnGetNextTokenFromBuffer( oEditor, szVarName,lIndex );

         // Move back to position after Object name.
         lIndex = lIndexAfterObjectName;
         // If we are searching for registered views, then szObjectName is
         // really the VOR name.  Get the object name.
         if ( nViewType == 1 )
         {
            // Add view variable and Object name to editor work object.
            // First, get the VOR by name.
            nRC = ActivateMetaOI_ByName( vSubtask, &vVor, 0, zREFER_VOR_META,
                                         zSINGLE | zLEVEL_APPLICATION,
                                         szObjectName, 0 );
            if ( nRC < 0 )
            {
               strcpy_s( sz, sizeof( sz ), "Can't find Registered View with name " );
               strcat_s( sz, sizeof( sz ), szObjectName );
               OperatorPrompt( 0, szlZeidonEditor, sz, 0, zBUTTONS_OK, 0, 0 );
               continue;
            }

            // Get Object name from VOR.
            GetStringFromAttribute( szObjectName, sizeof( szObjectName ), vVor, "LOD", szlName );
            DropMetaOI( vSubtask, vVor );
         }

         // Add view variable and Object name to editor work object.

         // If entity with szVarName does NOT exist, create it.
         // If the variable name is the same but the object name is different,
         // delete the old View and replace it with a new View.  If variable
         // name and object name are the same, then don't do anything.
         nRC = SetCursorFirstEntityByString( vEdWrk, szlView, szlVariableName,
                                             szVarName, "" );
         if ( nRC != zCURSOR_SET )
         {
            CreateEntity( vEdWrk, szlView, zPOS_LAST );
            SetAttributeFromString( vEdWrk,
                                    szlView, szlObjectName, szObjectName );
            SetAttributeFromString( vEdWrk,
                                    szlView, szlVariableName, szVarName );
         }
         else
         if ( CompareAttributeToString( vEdWrk, szlView, szlObjectName,
                                        szObjectName ) != 0 )
         {
            // We must do a delete to get rid of all Entity/Attribute entities.
            DeleteEntity( vEdWrk, szlView, zREPOS_FIRST );

            CreateEntity( vEdWrk, szlView, zPOS_LAST );
            SetAttributeFromString( vEdWrk,
                                    szlView, szlObjectName, szObjectName );
            SetAttributeFromString( vEdWrk,
                                    szlView, szlVariableName, szVarName );
         }
      } // while ( SrchFind... )
   } //  for ( nViewType = 0; nViewType < 2; nViewType++ )...

   if ( CheckExistenceOfEntity( vEdWrk, szlView ) >= zCURSOR_SET )
      AEQ_SelectView( vSubtask );

   return( 0 );

} // AEQ_GetViews

zSHORT LOCALOPER
fnPasteQualifier( zVIEW  vSubtask,
                  zSHORT nPasteCtl,
                  zBOOL  bInsertWithQuote )
{
   zVIEW  vEdWrk;
   zVIEW  vEditorSubtask;
   zPCHAR pszEntityName;
   zPCHAR pszAttribName;
   zCHAR  sz[ 400 ];

   mGetWorkView( &vEdWrk, vSubtask );
   GetParentWindow( &vEditorSubtask, vSubtask );

   if ( vEdWrk == 0 )
   {
      strcpy_s( sz, sizeof( sz ), "TZEDWRKO not Available at PasteQualifier." );
      OperatorPrompt( 0, "ED - Error", sz, 0, zBUTTONS_OK, 0, 0 );
      return( zCALL_ERROR );
   }

   if ( CheckExistenceOfEntity( vEdWrk, szlView ) != zCURSOR_SET )
      return( zCALL_ERROR );

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  szlLanguageType, szlVML_File ) != 0 )
   {
      strcpy_s( sz, sizeof( sz ), "Paste only works for VML Files." );
      OperatorPrompt( 0, szlZeidonEditor, sz, 0, zBUTTONS_OK, 0, 0 );
      return( zCALL_ERROR );
   }

   *sz = 0;

   if ( nPasteCtl & PASTE_VIEW_VARIABLE &&
        CheckExistenceOfEntity( vEdWrk, szlView ) == zCURSOR_SET )
   {
      GetStringFromAttribute( sz + zstrlen( sz ), sizeof( sz ) - zstrlen( sz ),
                              vEdWrk, szlView, "VariableName" );
   }

   if ( nPasteCtl & PASTE_ENTITY &&
        CheckExistenceOfEntity( vEdWrk, szlEntity ) == zCURSOR_SET )
   {
      GetAddrForAttribute( &pszEntityName, vEdWrk, szlEntity, szlEntityName );
      if ( pszEntityName )
      {
         if ( *sz )
         {
            if ( bInsertWithQuote )
               strcat_s( sz, sizeof( sz ), ", " );
            else
               strcat_s( sz, sizeof( sz ), "." );
         }

         if ( bInsertWithQuote )
            strcat_s( sz, sizeof( sz ), "\"" );

         // Get rid of any leading spaces.
         while ( *pszEntityName == ' ' )
            pszEntityName++;
         strcat_s( sz, sizeof( sz ), pszEntityName );

         if ( bInsertWithQuote )
            strcat_s( sz, sizeof( sz ), "\"" );
      }
   }

   if ( nPasteCtl & PASTE_ATTRIBUTE &&
        CheckExistenceOfEntity( vEdWrk, szlAttribute ) == zCURSOR_SET )
   {
      GetAddrForAttribute( &pszAttribName,
                           vEdWrk, szlAttribute, szlAttributeName );
      if ( pszAttribName )
      {
         if ( *sz )
         {
            if ( bInsertWithQuote )
               strcat_s( sz, sizeof( sz ), ", " );
            else
               strcat_s( sz, sizeof( sz ), "." );
         }

         if ( bInsertWithQuote )
            strcat_s( sz, sizeof( sz ), "\"" );

         strcat_s( sz, sizeof( sz ), pszAttribName );

         if ( bInsertWithQuote )
            strcat_s( sz, sizeof( sz ), "\"" );
      }
   }

   if ( *sz )
   {
      ZDrTBEdt *oEditor;
      zVIEW      vTemp;
      CString strStringToInsert;

      // Get the subtask for the editor window.
      GetParentWindow( &vTemp, vSubtask );
      oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vTemp,
                                                         EDIT_CONTROL_NAME );

      strcat_s( sz, sizeof( sz ), " " );
      strStringToInsert = sz;
      oEditor->InsertItem( strStringToInsert );
      SetFocusToCtrl( vTemp, EDIT_CONTROL_NAME );
   }

   return( 0 );

} // fnPasteQualifier

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteView( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE,
                     PASTE_WITHOUT_QUOTE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteViewEntity( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY,
                     PASTE_WITHOUT_QUOTE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteViewEntity_Quoted( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY,
                     PASTE_WITH_QUOTE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteViewEntityAttr( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY | PASTE_ATTRIBUTE,
                     PASTE_WITHOUT_QUOTE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteViewEntityAttr_Quoted( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY | PASTE_ATTRIBUTE,
                     PASTE_WITH_QUOTE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_SelectView( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vList;
   zVIEW  vLOD;
   zVIEW  vEdWrk;
   zPCHAR szLOD_Name;
   zPCHAR szName;

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
   {
      zCHAR sz[ 200 ];

      strcpy_s( sz, sizeof( sz ), "TZEDWRKO not Available at ObjSelAQ_ObjSelView." );
      OperatorPrompt( 0, "ED - Error", sz, 0, zBUTTONS_OK, 0, zICON_STOP );
      return( zCALL_ERROR );
   }

   // If Entity entity exists, then the LOD has already been loaded for
   // for this view.  Exit because nothing else needs to be done.
   if ( CheckExistenceOfEntity( vEdWrk, "Entity" ) == zCURSOR_SET )
   {
      AEQ_RefreshAEQ( vSubtask );
      return( 0 );
   }

   // Activate the LOD specified by vEdWrk.View.ObjectName
   GetAddrForAttribute( &szLOD_Name, vEdWrk, "View", "ObjectName" );
   nRC = RetrieveViewForMetaList( vSubtask, &vList, zREFER_LOD_META );
   nRC = SetCursorFirstEntity( vList, "W_MetaDef", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( &szName, vList, "W_MetaDef", "Name" );
      if ( zstrcmpi( szName, szLOD_Name ) == 0 )
         break;

      nRC = SetCursorNextEntity( vList, "W_MetaDef", "" );
   }

   if ( nRC != zCURSOR_SET )
   {
      zCHAR sz[ 200 ];

      DropView( vList );
      strcpy_s( sz, sizeof( sz ), "LOD " );
      strcat_s( sz, sizeof( sz ), szLOD_Name );
      strcat_s( sz, sizeof( sz ), " not found. Possibly wrong LPLR?" );
      OperatorPrompt( vSubtask, "ZeidonEditor", sz, 0, 0, 1, zICON_STOP );
      return( zCALL_ERROR );
   }

   nRC = ActivateMetaOI( vSubtask, &vLOD, vList, zREFER_LOD_META,
                         zSINGLE | zLEVEL_APPLICATION );
   DropView( vList );

   // Copy Entity and Attribute names from LOD to vEdWrk.
   nRC = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateEntity( vEdWrk, "Entity", zPOS_LAST );
      GetAddrForAttribute( &szName, vLOD, "LOD_Entity", "IndentName" );
      TraceLineS( "LOD_Entity IndentName: ", szName );
      SetAttributeFromAttribute( vEdWrk, "Entity", "EntityName",
                                 vLOD, "LOD_Entity", "IndentName" );

      nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
      while ( nRC == zCURSOR_SET )
      {
         CreateEntity( vEdWrk, "Attribute", zPOS_LAST );
         SetAttributeFromAttribute( vEdWrk, "Attribute", "AttributeName",
                                    vLOD, "ER_Attribute", "Name" );

         nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", "" );
      }

      nRC = SetCursorNextEntity( vLOD, "LOD_Entity", "" );
   }

   DropMetaOI( vSubtask, vLOD );
   nRC = SetCursorFirstEntity( vEdWrk, "Entity", "" );
   SetSelectStateOfEntity( vEdWrk, "Entity", 1 );

   if ( CheckExistenceOfEntity( vEdWrk, "Attribute" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vEdWrk, "Attribute", 1 );

   AEQ_RefreshAEQ( vSubtask );
   return( 0 );

} // AEQ_SelectView

zOPER_EXPORT zSHORT OPERATION
AEQ_ViewPopup( zVIEW vSubtask )
{
// POINT  pt;
   CPoint pt( GetMessagePos( ) );
// pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "ViewPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_EntityPopup( zVIEW vSubtask )
{
// POINT  pt;
   CPoint pt( GetMessagePos( ) );
// pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "EntityPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_AttributePopup( zVIEW vSubtask )
{
// POINT  pt;
   CPoint pt( GetMessagePos( ) );
// pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "AttributePopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OpIns_Postbuild( zVIEW vSubtask )
{
   zVIEW vEdWrk;

   TZEDFRMD_DisableActions( vSubtask );

   // Get the subtask view for the parent editor window and give it another
   // name at the current subtask level so we can easily retrieve it.
   GetViewByName( &vEdWrk, "__EditorSubtask", vSubtask, zLEVEL_TASK );
   SetNameForView( vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_SUBTASK );

   fnSetSubwindowPosition( vSubtask, szlOperationsList );
   OpIns_BuildOperList( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION OpIns_Close( zVIEW vSubtask )
{
   fnSaveSubwindowPosition( vSubtask, szlOperationsList );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OpIns_RefreshSelectButton( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zCHAR  szCurrentType[ 5 ];

   mGetWorkView( &vEdWrk, vSubtask );
   GetStringFromAttribute( szCurrentType, sizeof( szCurrentType ), vEdWrk, "OperListType", "Type" );
   if ( zstrcmp( szCurrentType, "Z" ) == 0 )
      MB_SetMessage( vSubtask, 0, "Press F1 for Help on the operation." );

   RefreshCtrl( vSubtask, "SelectButton" );
   return( 0 );
}

zSHORT LOCALOPER
PasteOperation( zVIEW vSubtask, zVIEW vOp )
{
   zSHORT     nRC;
   zVIEW      vEdWrk;
   zPCHAR     szStr;
   zBOOL      bInsertComment = FALSE;
   zBOOL      bVML_File;
   zVIEW      vEditorSubtask;
   zLONG      lOperationNameLength = 0,
              lLine, lColumn, lIndex;
   zLONG      lTabSize = 4; // Today a fixed length, in future this has to be queried
                            // from the edit-control
   CString    strComment;
   CString    strIndent;
   CString    strTemp;
   ZDrTBEdt *oEditor = 0;

   if ( lTabSize > 1 )  // Don't ask why ... it's hard to explain
      lTabSize--;

   // Get the c++ object handle for the editor control.
   mGetWorkView( &vEdWrk, vSubtask );

   GetIntegerFromAttribute( (zPLONG) &vEditorSubtask,
                            vEdWrk, szlBuffer, "vSubtask" );
   oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vEditorSubtask,
                                                      EDIT_CONTROL_NAME );
   if ( oEditor == 0 )
   {
      SysMessageBox( vSubtask, "Zeidon Error",
                     "Couldn't find Editor control.", 1 );
      return( 0 );
   }

   bVML_File = (CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType,
                                          szlVML_File ) == 0);
   bInsertComment = (CompareAttributeToString( vEdWrk, szlBuffer,
                                               szlOpIns_InsertDescribtion,
                                               "Y" ) == 0);
   GetAddrForAttribute( &szStr, vOp, szlOperation, szlName );
   if ( bInsertComment )
   {
      zPCHAR  szComment;
      zLONG   lCommentLength = 0;
      GetAddrForAttribute( &szComment, vOp, szlOperation, szlDesc );

      lCommentLength = zstrlen( szComment );
      if ( lCommentLength > 0 )
      {
         strComment  = "/*\n";
         strComment += szComment;
         strComment += "*/\n";
      }
      else
         strComment.Empty( );
   }

   strTemp = szStr;
   strTemp += "( ";

   // Get length of operation name and add position within current line.
   lOperationNameLength = zstrlen( strTemp );
   oEditor->GetCursorPosition( &lLine, &lColumn, &lIndex );
   lOperationNameLength += lColumn;
   lColumn = GetTabsInLine( oEditor, lColumn ); // Get number of tabs until current Cursorposition
   lOperationNameLength -=lColumn;  // Control interprets tab as a single character
   lColumn = lColumn * lTabSize;    // remove single zCHAR and add multiple
   lOperationNameLength += lColumn; // (virtual) characters depending on tabsize

   // Set up a string which contains a number of tabs and blanks depending
   // on operation name and position, to format the operation insert frame.

   // insert several tabs
   // while ( lOperationNameLength > lTabSize )
   // {
   //   strIndent += "\t";
   //   lOperationNameLength -= lTabSize;
   // }

   // insert several tabs
   while ( lOperationNameLength > 0 )
   {
      strIndent += " ";
      lOperationNameLength--;
   }

   nRC = SetCursorFirstEntity( vOp, "Parameter", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( &szStr, vOp, "Parameter", "PFlag" );
      if ( bVML_File == FALSE && *szStr == 'Y' )
         strTemp += "PTR ";

      GetAddrForAttribute( &szStr, vOp, "Parameter", "UFlag" );
      if ( bVML_File == FALSE  && *szStr == 'Y' )
         strTemp += "UNSIGNED ";

      GetAddrForAttribute( &szStr, vOp, "Parameter", "DataType" );
      if ( *szStr )
      {
         if ( bVML_File )
         {
            switch ( *szStr )
            {
               case 'A':
                  strTemp += "\"ATTRIBUTE_NAME\"";
                  break;

               case 'E':
                  strTemp +="\"ENTITY_NAME\"";
                  break;

               case 'L':
                  strTemp += "INTEGER";
                  break;

               case 'M':
                  strTemp += "DOUBLE";
                  break;

               case 'N':
                  strTemp += "INTEGER";
                  break;

               case 'O':
                  strTemp += "VOID";
                  break;

               case 'S':
                  strTemp += "STRING";
                  break;

               case 'V':
                  strTemp += "VIEW";
                  break;

               case 'Y':
                  strTemp += "STRING";
                  break;

               default :
                  strTemp += szStr;
                  break;

            } // switch
         }
         else
         {
            switch ( *szStr )
            {
               case 'A':
                  strTemp += "\"ATTRIBUTE_NAME\"";
                  break;

               case 'E':
                  strTemp += "\"ENTITY_NAME\"";
                  break;

               case 'L':
                  strTemp +="zLONG";
                  break;

               case 'M':
                  strTemp += "DOUBLE";
                  break;

               case 'N':
                  strTemp += "zSHORT";
                  break;

               case 'O':
                  strTemp += "void";
                  break;

               case 'S':
                  strTemp += "zCHAR";
                  break;

               case 'V':
                  strTemp += "zVIEW";
                  break;

               case 'Y':
                  strTemp += "zCHAR";
                  break;

               default :
                  strTemp += szStr;
                  break;

            } // switch
         } // if ( bVML_File )...else...
      }

      GetAddrForAttribute( &szStr, vOp, "Parameter", "ShortDesc" );
      if ( *szStr )
      {
         strTemp += " /* ";
         strTemp += szStr;
         strTemp += " */";
      }

      nRC = SetCursorNextEntity( vOp, "Parameter", "" );
      if ( nRC == zCURSOR_SET )
      {
         strTemp += ",\n";
         strTemp += strIndent;
      }
      else
         strTemp += " ";
   }

   strTemp += ")";
   if ( bVML_File == FALSE )
      strTemp += ";";

   if ( strComment.GetLength( ) > 0 )
   {
      strComment += strTemp;
      strTemp = strComment;
   }

   oEditor->InsertItem( strTemp );
   return( 0 );

} // PasteOperation

zOPER_EXPORT zSHORT OPERATION
OpIns_InsertOperation( zVIEW vSubtask )
{
   zCHAR  szType[ 5 ];
   zVIEW  vEdWrk;
   zVIEW  vTemp;

   mGetWorkView( &vEdWrk, vSubtask );
   GetStringFromAttribute( szType, sizeof( szType ), vEdWrk, szlOperListType, szlType );
   switch ( *szType )
   {
      case 'Z':
      {
         zVIEW vTZOPSIGK;

         // User wants to insert a Zeidon operation.  Retrieve the view that
         // contains the list of Zeidon operations and past the operation.
         GetViewByName( &vTZOPSIGK, szlTZOPSIGK, vSubtask, zLEVEL_TASK );
         SetCursorFirstEntityByAttr( vTZOPSIGK, szlOperation, szlName,
                                     vEdWrk, szlOper, szlName, 0 );
         PasteOperation( vSubtask, vTZOPSIGK );
         break;
      }

      case 'G':
      {
         zVIEW vOp;
         zCHAR  szOpName[ 255 ];

         // User wants to insert a Global operation.  Retrieve the Global
         // Operations Group view that contains the operation.

         GetStringFromAttribute( szOpName, sizeof( szOpName ), vEdWrk, szlOper, szlName );
         ActivateMetaOI_ByName( vSubtask, &vOp, 0, zREFER_GO_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                szOpName, zCURRENT_OI );
         PasteOperation( vSubtask, vOp );
         break;
      }

      case 'L':
      {
         zVIEW vMeta;
         zLONG lMetaType;

         // User wants to insert a "Local"operation.  Retrieve the meta
         // and paste.

         GetIntegerFromAttribute( (zPLONG) &vMeta, vEdWrk, szlBuffer,
                                  szlSourceViewID );
         GetIntegerFromAttribute( &lMetaType, vEdWrk, szlBuffer, szlMetaType );

         // Create a temp view so that we can muck around with cursors.
         CreateViewFromViewForTask( &vMeta, vMeta, 0 );

         if ( lMetaType == zREFER_DIALOG_META ||
              lMetaType == zREFER_LOD_META )
         {
            // Dialog and LOD meta's can have multiple source files for one
            // meta.  Make sure we are dealing with the opertions for the
            // correct source file.
            SetCursorFirstEntityByAttr( vMeta, szlSourceFile, szlName,
                                        vEdWrk, szlBuffer,
                                        szlSourceFileName, 0 );
         }

         SetCursorFirstEntityByAttr( vMeta, szlOperation, szlName,
                                     vEdWrk, szlOper, szlName, 0 );

         PasteOperation( vSubtask, vMeta );

         DropView( vMeta );
         break;

      } // case 'L'...

      case 'O':
      {
         zVIEW  vObj;
         zCHAR  szObjectName[ 50 ];
         zCHAR  szOperName[ 255 ];
         zPCHAR psz;

         // User wants to insert an Object Operation.
         GetStringFromAttribute( szObjectName, sizeof( szObjectName ), vEdWrk, szlOper, szlName );

         // Look for the period between the Object name and the Oper name.
         for ( psz = szObjectName; *psz && *psz != '.'; psz++ )
            ; // Nothing needs to be done here.

         // Change the period to a null-term and bump up the pointer to
         // point to the oper name.
         *psz++ = 0;

         // Copy the oper name.
         strcpy_s( szOperName, sizeof( szOperName ), psz );

         // Activate the object meta.
         ActivateMetaOI_ByName( vSubtask, &vObj, 0, zREFER_LOD_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                szObjectName, zFORCE_RELOAD );

         // Set the cursor to the operation.
         SetCursorFirstEntityByString( vObj, szlOperation, szlName,
                                       szOperName, szlLOD );

         PasteOperation( vSubtask, vObj );

         DropMetaOI( vSubtask, vObj );
         break;

      } // case 'O'...

      case 'X':
      {
         CString  strIndent;
         zVIEW    vEditorSubtask;
         ZDrTBEdt *oEditor = 0;
         zCHAR    szBuffer[ 1024 ];
         zLONG    lOperationNameLth = 0,
                  lLine, lColumn, lIndex;
         zLONG    lTabSize = 4; // Today a fixed length, in future this has
                                // to be queried from the edit-control
         zBOOL    bVML_File;

         GetStringFromAttribute( szBuffer, sizeof( szBuffer ), vEdWrk, szlOper, szlName );
         BuildTZEDWRKO( vEdWrk, szBuffer, sizeof( szBuffer ) );

         if ( lTabSize > 1 )  // Don't ask why ... it's hard to explain
            lTabSize--;

         // Get the C++ object handle for the editor control.
         GetIntegerFromAttribute( (zPLONG) &vEditorSubtask,
                                  vEdWrk, szlBuffer, "vSubtask" );
         oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vEditorSubtask,
                                                            EDIT_CONTROL_NAME );
         if ( oEditor == 0 )
         {
            SysMessageBox( vSubtask, "Zeidon Error",
                           "Couldn't find Editor control.", 1 );
            return( 0 );
         }

         bVML_File = (CompareAttributeToString( vEdWrk, szlBuffer,
                                                szlLanguageType,
                                                szlVML_File ) == 0);

         // Get length of operation name and add position within current line
         lOperationNameLth = zstrlen( szBuffer );
         oEditor->GetCursorPosition( &lLine, &lColumn, &lIndex );
         lOperationNameLth += lColumn;
         lColumn = GetTabsInLine( oEditor, lColumn ); // Get number of tabs until current Cursorposition
         lOperationNameLth -=lColumn;  // Control interprets tab as a single character
         lColumn = lColumn * lTabSize;    // remove single zCHAR and add multiple
         lOperationNameLth += lColumn; // (virtual) characters depending on tabsize

         // Set up a string which contains a number of tabs and blanks depending
         // on operation name and position, to format the operation insert frame.

         // insert several tabs
         while ( lOperationNameLth > 1 )
         {
            strIndent += " ";
            lOperationNameLth--;
         }

         if ( bVML_File == FALSE )
            strcat_s( szBuffer, sizeof( szBuffer ), ";" );

         strIndent += szBuffer;
         oEditor->InsertItem( strIndent );
         break;
      }

      default:
         SysMessageBox( vSubtask, "Zeidon Internal",
                        "This operation type not yet supported", 0 );
         break;

   } // switch ( *szType )...

   // Set Focus to Editor
   GetParentWindow( &vTemp, vSubtask );
   SetFocusToCtrl( vTemp, EDIT_CONTROL_NAME );
   return( 0 );
}

// This operation builds the operation list for the OperationList window.
// Depending on what the value is for vEdWrk.Buffer.OpIns_CurrentType
// we build different operations lists.
zOPER_EXPORT zSHORT OPERATION
OpIns_BuildOperList( zVIEW vSubtask )
{
#define DISP_SELECT_SET    1
#define SEL_SELECT_SET     2

   zCHAR  szCurrentType[ 5 ];
   zVIEW  vEdWrk;
   zSHORT nRC;
   zCHAR  szKeyword[ 255 ];
   zLONG  lSelectCount = 0;

   mGetWorkView( &vEdWrk, vSubtask );

   LB_SetDisplaySelectSet( vSubtask, "OperationList", DISP_SELECT_SET );
   LB_SetSelectSelectSet( vSubtask, "OperationList", SEL_SELECT_SET );
   SetAllSelStatesForEntityForSet( vEdWrk, "Oper", FALSE, DISP_SELECT_SET, 0 );
   SetAllSelStatesForEntityForSet( vEdWrk, "Oper", FALSE, SEL_SELECT_SET, 0 );

   GetStringFromAttribute( szCurrentType, sizeof( szCurrentType ), vEdWrk, szlBuffer,
                           "OpIns_CurrentType" );

   // Default of CurrentType is 'Z'...
   if ( *szCurrentType == 0 )
   {
      strcpy_s( szCurrentType, sizeof( szCurrentType ), "Z" );
      SetAttributeFromString( vEdWrk, szlBuffer, "OpIns_CurrentType",
                              szCurrentType );
   }

   // Check to see if the operations for the current type are already
   // loaded. If they are, then we obviously don't need to load them again.
   if ( SetCursorFirstEntityByString( vEdWrk, "OperListType", "Type",
                                      szCurrentType, 0 ) != zCURSOR_SET )
   {
      // Operations haven't been loaded yet, so load 'em up.

      CreateEntity( vEdWrk, "OperListType", zPOS_LAST );
      SetAttributeFromString( vEdWrk, "OperListType", "Type", szCurrentType );

      switch ( *szCurrentType )
      {
         case 'A':
         case 'Z':
         {
            zVIEW vTZOPSIGK;

            // Get list of Zeidon operations.

            // Check to see if we've already activated the Zeidon opers list.
            // If not, activate it and save the view.
            GetViewByName( &vTZOPSIGK, "TZOPSIGK", vSubtask, zLEVEL_TASK );
            if ( !vTZOPSIGK )
            {
               oTZOPGRPO_GetViewForXGO( &vTZOPSIGK, zSYS_CURRENT_OI );
               SetNameForView( vTZOPSIGK, "TZOPSIGK", vSubtask, zLEVEL_TASK );
            }

            // Now copy each of the operations to the editor work object.
            for ( nRC = SetCursorFirstEntity( vTZOPSIGK, "Operation", 0 );
                  nRC == zCURSOR_SET;
                  nRC = SetCursorNextEntity( vTZOPSIGK, "Operation", 0 ) )
            {
               CreateEntity( vEdWrk, "Oper", zPOS_LAST );
               SetMatchingAttributesByName( vEdWrk, "Oper",
                                            vTZOPSIGK, "Operation", zSET_ALL );
            }

            if ( *szCurrentType != 'A' )
               break;
         } // Case 'Z'...

         case 'G':
         {
            zVIEW vOp;

            // Get list of Global operations.

            RetrieveViewForMetaList( vSubtask, &vOp, zREFER_GO_META );

            // Now copy each of the operations to the editor work object.
            for ( nRC = SetCursorFirstEntity( vOp, "W_MetaDef", 0 );
                  nRC == zCURSOR_SET;
                  nRC = SetCursorNextEntity( vOp, "W_MetaDef", 0 ) )
            {
               CreateEntity( vEdWrk, "Oper", zPOS_LAST );
               SetMatchingAttributesByName( vEdWrk, "Oper",
                                            vOp, "W_MetaDef", zSET_ALL );
            }

            DropView( vOp );

            if ( *szCurrentType != 'A' )
               break;
         } // Case 'G'...

         case 'L':
         {
            zVIEW vMeta;
            zLONG lMetaType;

            // Get list of "Local" operations.  "Local" operations in this
            // case mean all operations that are for the current meta.
            // For example, if the current meta is a Dialog, then the "Local"
            // operations include local and Dialog operations.

            // Get the source view and the meta type.
            GetIntegerFromAttribute( (zPLONG) &vMeta, vEdWrk, "Buffer",
                                     "SourceViewID" );
            GetIntegerFromAttribute( &lMetaType, vEdWrk, "Buffer",
                                     "MetaType" );

            // Create a new, temp view of the meta so that we can muck around
            // with the cursors.
            CreateViewFromViewForTask( &vMeta, vMeta, 0 );

            //
            // Now copy the "Local" meta operations to vEdWrk.
            //

            if ( lMetaType == zREFER_DIALOG_META ||
                 lMetaType == zREFER_LOD_META )
            {
               // Make sure we are reading the "Local" operations for
               // the current source file.
               SetCursorFirstEntityByAttr( vMeta, "SourceFile", "Name",
                                           vEdWrk, "Buffer",
                                           "SourceFileName", 0 );
            }

            // Copy all the operations to vEdWrk
            for ( nRC = SetCursorFirstEntity( vMeta, "Operation", 0 );
                  nRC == zCURSOR_SET;
                  nRC = SetCursorNextEntity( vMeta, "Operation", 0 ) )
            {
               CreateEntity( vEdWrk, "Oper", zPOS_LAST );
               SetMatchingAttributesByName( vEdWrk, "Oper",
                                            vMeta, "Operation",
                                            zSET_ALL );
            }

            DropView( vMeta );
            break;

         } // Case 'L'...

         case 'O':
         {
            // Since we load the object operations by object, we don't have
            // to do anything here.
            break;

         } // Case 'O'...

         case 'X':  // ActiveX operations
         {
            BuildTZEDWRKO( vEdWrk, 0, 0 );
            break;
         }

         default:
            SysMessageBox( vSubtask, "Zeidon Internal",
                           "This operation type not yet supported", 0 );
            break;

      } // switch ( *szCurrentType )...

      // Now that we've loaded the operations, sort them.
      OrderEntityForView( vEdWrk, "Oper", "Name A" );

   } // if ( SetCursor...)...

   // Disable the "Objects" button unless we are showing object operations.
   if ( *szCurrentType == 'O' )
      SetCtrlState( vSubtask, "ListObjects", zCONTROL_STATUS_ENABLED, TRUE );
   else
      SetCtrlState( vSubtask, "ListObjects", zCONTROL_STATUS_ENABLED, FALSE );

   // Now set the select states depending on the keyword values.
   GetStringFromAttribute( szKeyword, sizeof( szKeyword ), vEdWrk, szlBuffer, "OpIns_Keyword" );
   if ( *szKeyword &&
        CompareAttributeToString( vEdWrk,
                                  szlBuffer, "OpIns_UseKeyword", "Y" ) == 0 )
   {
      zCHAR  szIgnoreCase[ 5 ];
      zPCHAR pszKeyword;
      zBOOL  bStartColOne;
      zSHORT nKeywordLth;

      // De-select all operations.
      SetAllSelStatesForEntityForSet( vEdWrk, "Oper", FALSE, DISP_SELECT_SET, 0 );
      SetAllSelStatesForEntityForSet( vEdWrk, "Oper", FALSE, SEL_SELECT_SET, 0 );

      GetStringFromAttribute( szIgnoreCase, sizeof( szIgnoreCase ), vEdWrk, szlBuffer,
                              "OpIns_KeywordIgnoreCase" );
      if ( *szIgnoreCase == 'Y' )
      {
         for ( pszKeyword = szKeyword; *pszKeyword; pszKeyword++ )
            *pszKeyword = ztoupper( *pszKeyword );
      }

      // If the keyword starts with a "*", then the keyword search starts
      // with the first zCHAR (e.g. column) ONLY.
      pszKeyword = szKeyword;
      if ( *pszKeyword == '*' || *pszKeyword == '/' )
      {
         pszKeyword++;
         bStartColOne = FALSE;
      }
      else
         bStartColOne = TRUE;

      nKeywordLth = (zSHORT) zstrlen( pszKeyword );

      for ( nRC = SetCursorFirstEntity( vEdWrk, "Oper", 0 );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vEdWrk, "Oper", 0 ) )
      {
         zCHAR  szOperName[ 255 ];

         GetStringFromAttribute( szOperName, sizeof( szOperName ), vEdWrk, "Oper", "Name" );
         if ( *szIgnoreCase == 'Y' )
         {
            zPCHAR pszOperName;

            pszOperName = zstrupr( szOperName );
         }

         if ( bStartColOne )
         {
            if ( zstrncmp( szOperName, pszKeyword, nKeywordLth ) == 0 )
            {
               SetSelectStateOfEntityForSet( vEdWrk, "Oper", TRUE, DISP_SELECT_SET );
               lSelectCount++;
            }
         }
         else
         {
            if ( strstr( szOperName, pszKeyword ) )
            {
               SetSelectStateOfEntityForSet( vEdWrk, "Oper", TRUE, DISP_SELECT_SET );
               lSelectCount++;
            }
         }
      } // for...
   }
   else
   {
      // Select all operations.
      SetAllSelStatesForEntityForSet( vEdWrk, "Oper", TRUE, DISP_SELECT_SET, 0 );
      lSelectCount = CountEntitiesForView( vEdWrk, "Oper" );
   }

   SetSelectSetForView(   vEdWrk, DISP_SELECT_SET );
   SetCursorFirstSelectedEntity( vEdWrk, "Oper", 0 );

   // Re-use szKeyword to set the message...
   sprintf( szKeyword, "%d Operations listed", lSelectCount );
   MB_SetMessage( vSubtask, 0, szKeyword );

   RefreshCtrl( vSubtask, "OperationList" );
   TZEDFRMD_DisableActions( vSubtask );
   return( 0 );

} // OpIns_BuildOperList

zOPER_EXPORT zSHORT OPERATION
OpIns_RebuildOperList( zVIEW vSubtask )
{
   zCHAR  szCurrentType[ 5 ];
   zVIEW  vEdWrk;

   mGetWorkView( &vEdWrk, vSubtask );
   GetStringFromAttribute( szCurrentType, sizeof( szCurrentType ), vEdWrk, szlBuffer,
                           "OpIns_CurrentType" );

   // Default of CurrentType is 'Z'...
   if ( *szCurrentType == 0 )
   {
      strcpy_s( szCurrentType, sizeof( szCurrentType ), "Z" );
      SetAttributeFromString( vEdWrk, szlBuffer, "OpIns_CurrentType",
                              szCurrentType );
   }

   // Check to see if the operations for the current type are already
   // loaded. If they are, then delete the list.
   if ( SetCursorFirstEntityByString( vEdWrk, "OperListType", "Type",
                                      szCurrentType, 0 ) == zCURSOR_SET )
   {
      DeleteEntity( vEdWrk, "OperListType", zREPOS_FIRST );
   } // if ( SetCursor...)...

   // Now call BuildOperList to rebuild the oper list.
   OpIns_BuildOperList( vSubtask );
   return( 0 );

} // OpIns_RebuildOperList

// Loads a list of Objects (LODs) for the current lplr.
zOPER_EXPORT zSHORT OPERATION
ObjList_Prebuild( zVIEW vSubtask )
{
   zVIEW vObjList;

   GetViewByName( &vObjList, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( vObjList )
      DropView( vObjList );

   RetrieveViewForMetaList( vSubtask, &vObjList, zREFER_LOD_META );
   SetNameForView( vObjList, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   return( 0 );
}

// The user selected a LOD in the Object list.  Load the LOD meta and copy
// all object operations to the editor work object.
zOPER_EXPORT zSHORT OPERATION
ObjList_AddObjOpersToWorkList( zVIEW vSubtask )
{
   zVIEW  vObjList;
   zVIEW  vLOD;
   zVIEW  vEdWrk;
   zVIEW  vParentSubtask;
   zSHORT nRC;
   zSHORT nLth;
   zCHAR  szOpName[ 255 ];

   // We need to get the parent subtask view so that we can retrieve the work view.
   GetParentWindow( &vParentSubtask, vSubtask );
   mGetWorkView( &vEdWrk, vParentSubtask );

   GetViewByName( &vObjList, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   ActivateMetaOI( vSubtask, &vLOD, vObjList, zREFER_LOD_META, 0 );
   GetStringFromAttribute( szOpName, sizeof( szOpName ), vObjList, "W_MetaDef", "Name" );
   strcat_s( szOpName, sizeof( szOpName ), "." );
   nLth = (zSHORT) zstrlen( szOpName );

   for ( nRC = SetCursorFirstEntity( vLOD, "OperationList", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "OperationList", 0 ) )
   {
      GetStringFromAttribute( szOpName + nLth, sizeof( szOpName ) - nLth, vLOD, "OperationList", "Name" );

      CreateEntity( vEdWrk, "Oper", zPOS_LAST );
      SetAttributeFromString( vEdWrk, "Oper", "Name", szOpName );
   }

   DropMetaOI( vSubtask, vLOD );
   DropView( vObjList );
   return( 0 );

} // ObjList_AddObjOpersToWorkList

zOPER_EXPORT zSHORT OPERATION
ObjList_RefreshSelectButton( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "LoadObjOperations" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SortMetaListByName( zVIEW vSubtask )
{
   zVIEW  vMetaList;

   GetViewByName( &vMetaList, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vMetaList, "W_MetaDef", "Name" );
   RefreshCtrl( vSubtask, "ListObjects" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SortMetaListByDate ( zVIEW vSubtask )
{
   zVIEW  vMetaList;

   GetViewByName( &vMetaList, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vMetaList, "W_MetaDef", "LastUpdateDate D" );
   RefreshCtrl( vSubtask, "ListObjects" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VOR_Prebuild( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zVIEW  vParentSubtask;
   zVIEW  vList;

   // We need to get the parent subtask view so that we can retrieve the work view.
   GetParentWindow( &vParentSubtask, vSubtask );
   mGetWorkView( &vEdWrk, vParentSubtask );

   GetViewByName( &vList, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( vList )
      DropView( vList );

   RetrieveViewForMetaList( vSubtask, &vList, zREFER_VOR_META );
   SetNameForView( vList, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VOR_PasteName( zVIEW vSubtask )
{
   zVIEW      vList;
   zVIEW      vEdWrk;
   zVIEW      vTemp;
   zVIEW      vEditorSubtask;
   zPCHAR     szOption;
   zPCHAR     pszName;
   zCHAR      sz[ 200 ];
   ZDrTBEdt *oEditor;

   GetViewByName( &vList, szlTZCMLPLO_List, vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vList, szlW_MetaDef ) != zCURSOR_SET )
      return( 0 );

   // Get the subtask for the editor window.
   GetParentWindow( &vTemp, vSubtask );
   GetParentWindow( &vEditorSubtask, vTemp );
   mGetWorkView( &vEdWrk, vEditorSubtask );
   oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vEditorSubtask,
                                                      EDIT_CONTROL_NAME );

   GetAddrForAttribute( &szOption, vEdWrk, szlBuffer, szlPasteVorOption );
   GetAddrForAttribute( &pszName, vList, szlW_MetaDef, szlName );

   switch ( *szOption )
   {
      case 'N':
         strcpy_s( sz, sizeof( sz ), pszName );
         break;

      case 'R':
         sprintf_s( sz, sizeof( sz ), "VIEW %s REGISTERED AS %s", pszName, pszName );
         break;

      case 'G':
         sprintf_s( sz, sizeof( sz ), "GET VIEW %s NAMED \"%s\"", pszName, pszName );
         break;

      case 'S':
         sprintf_s( sz, sizeof( sz ), "NAME VIEW %s \"%s\"", pszName, pszName );
         break;

      case 'B':
      {
         zVIEW  vVOR;
         zPCHAR pszLOD_Name;

         ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zREFER_VOR_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                pszName, zCURRENT_OI );

         GetAddrForAttribute( &pszLOD_Name, vVOR, "LOD", "Name" );
         sprintf_s( sz, sizeof( sz ), "VIEW %s BASED ON LOD %s", pszName, pszLOD_Name );

         DropMetaOI( vSubtask, vVOR );
         break;
      }
   }

// CString zs = sz;
// BSTR bstr = zs.AllocSysString( );
   oEditor->InsertItem( sz );
   DropView( vList );


   //> fnSetFocusEditor( );
   //x oEditor->SetFocus( );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// Following structure contains a list of all standard VML insert strings.
// It is used only by VML_BuildTextList
/////////////////////////////////////////////////////////////////////////////
typedef struct _VML_TextStringRecord
{
   zPCHAR  pszText;
   zPCHAR  pszType;
   zPCHAR  pszInsertText;
   zPCHAR  pszInsertTextFull;
   zPCHAR  pszCC_Code;
   zPCHAR  pszCC_FirstChar;
} VML_TextStringRecord;

VML_TextStringRecord g_VML_TextStrings[] =
{
   "VIEW var-name BASED ON LOD lod-name", "D",
   "VIEW & BASED ON LOD ",
   "VIEW &var-name BASED ON LOD lod-name",
   "BOL", "Y",

   "VIEW var-name REGISTERED AS", "D",
   "VIEW & REGISTERED AS ",
   "VIEW &var-name REGISTERED AS regv-name",
   "VIEW", "Y",

   "INTEGER var-name", "D",
   "INTEGER &",
   "INTEGER &var-name",
   "", "",

   "SHORT var-name", "D",
   "SHORT &",
   "SHORT &var-name",
   "", "",

   "STRING ( lth ) var-name", "D",
   "STRING ( & ) ",
   "STRING ( &lth ) var-name",
   "STRING", "Y",

   "DECIMAL var-name", "D",
   "DECIMAL &",
   "DECIMAL &var-name",
   "", "",

   "CHARACTER var-name", "D",
   "CHARACTER &",
   "CHARACTER &var-name",
   "", "",

   "VIEW var-name", "D",
   "VIEW &",
   "VIEW &var-name",
   "", "",

   "ENTITYNAME var-name", "D",
   "ENTITYNAME &",
   "ENTITYNAME &var-name",
   "", "",

   "ATTRIBUTENAME var-name", "D",
   "ATTRIBUTENAME &",
   "ATTRIBUTENAME &var-name",
   "", "",

   "ACTIVATE view WHERE", "V",
   "ACTIVATE & WHERE ",
   "ACTIVATE &view [ FOR UPDATE ][ SINGLE | MULTIPLE | ROOTONLY | ROOTONLYMULTIPLE | EMPTY ]\r\n"
   "   [ WHERE Condition [ RESTRICTING Entity TO Condition ] ]",
   "ACTIVATE", "Y",

   "CASE int-var...", "C",
   "CASE &\r\n   OF :\r\nEND",
   "CASE &int-var\r\n   OF int-constant:\r\n   OTHERWISE:\r\nEND",
   "CASE", "Y",


   "COMMIT view", "V",
   "COMMIT &",
   "COMMIT &view",
   "", "",

   "CREATE ENTITY view.entity", "E",
   "CREATE ENTITY &",
   "CREATE ENTITY &view.entity [ AFTER | BEFORE | FIRST | LAST ]",
   "CREATE", "",

   "DELETE ENTITY view.entity", "E",
   "DELETE ENTITY &",
   "DELETE ENTITY &view.entity [ NEXT | PREVIOUS | FIRST | LAST | NONE ]",
   "DELETE", "Y",

   "EXCLUDE view.entity", "E",
   "EXCLUDE &",
   "EXCLUDE &view.entity [ AFTER | BEFORE | NONE ]",
   "EXCLUDE", "Y",

   "FOR view.entity ... END", "E",
   "FOR &\r\nEND",
   "FOR &[ EACH ] [ integer ] view.entity\r\n"
   "    [ WITHIN scoping-view.entity ]\r\n"
   "    [ WHERE condition ]\r\n"
   "END",
   "FOR", "Y",

   "GET VIEW view NAMED view-name", "V",
   "GET VIEW & NAMED \"\"",
   "GET VIEW &view NAMED \"view-name\"\r\n"
   "    [ AT LEVEL { TASK | SUBTASK | APPLICATION | SYSTEM | ANY } ] [ USING Subtask ]",
   "GET", "Y",

   "IF condition... END", "C",
   "IF &\r\nEND",
   "IF &condition\r\n[ ELSE ]\r\nEND",
   "IF", "Y",

   "IF condition... ELSE...END", "C",
   "IF &\r\nELSE\r\nEND",
   "IF &condition\r\nELSE\r\nEND",
   "IFE", "Y",

   "INCLUDE target-view.entity FROM source-view.entity", "E",
   "INCLUDE & FROM ",
   "INCLUDE &target-view.entity FROM source-view.entity [ AFTER | BEFORE ]",
   "INCLUDE", "Y",

   "LOOP ... END", "C",
   "LOOP &\r\nEND",
   "LOOP &[ WHILE condition ]\r\n{ END | UNTIL condition }",
   "LOOP", "Y",

   "LOOP WHILE condition... END", "C",
   "LOOP WHILE &\r\nEND",
   "LOOP WHILE &condition ]\r\nEND",
   "LOOPW", "Y",

   "LOOP ... UNTIL condition", "C",
   "LOOP\r\nUNTIL &",
   "LOOP\r\nUNTIL &condition",
   "LOOPU", "Y",

   "NAME VIEW view view-name", "V",
   "NAME VIEW & \"\"",
   "NAME VIEW &view \"view-name\"\r\n"
   "   [ AT LEVEL { TASK | SUBTASK | APPLICATION | SYSTEM | ANY } ] [ USING Subtask ]",
   "NAME", "Y",

   "SET CURSOR view.entity", "E",
   "SET CURSOR &",
   "SET CURSOR &[ FIRST | LAST | NEXT | PREVIOUS ] view.entity\r\n"
   "    [ WITHIN scoping-view.entity ]\r\n"
   "    [ WHERE condition ]",
   "SET", "Y",

   "SET CURSOR FIRST view.entity WHERE", "E",
   "SET CURSOR FIRST &\r\n    WHERE ",
   "SET CURSOR FIRST &view.entity\r\n"
   "    [ WITHIN scoping-view.entity ]\r\n"
   "    [ WHERE condition ]",
   "SETF", "Y",

   "SET CURSOR NEXT view.entity WHERE", "E",
   "SET CURSOR NEXT &\r\n    WHERE ",
   "SET CURSOR NEXT &view.entity\r\n"
   "    [ WITHIN scoping-view.entity ]\r\n"
   "    [ WHERE condition ]",
   "SETN", "Y",

   "=",  "O", "= ",  0, 0, 0,
   "!=", "O", "!= ", 0, 0, 0,
   "<=", "O", "<= ", 0, 0, 0,
   ">=", "O", ">= ", 0, 0, 0,
   "<",  "O", "< ",  0, 0, 0,
   ">",  "O", "> ",  0, 0, 0,
   "+",  "O", "+ ",  0, 0, 0,
   "-",  "O", "- ",  0, 0, 0,
   "*",  "O", "* ",  0, 0, 0,
   "/",  "O", "/ ",  0, 0, 0,
   "(",  "O", "( ",  0, 0, 0,
   ")",  "O", ") ",  0, 0, 0,

   "OR",  "O", "OR ",  0, 0, 0,
   "AND", "O", "AND ",  0, 0, 0,

   "space",     "?", " ",     0, 0, 0,
   "backspace", "?", "%ml1",  0, 0, 0,
   "newline",   "?", "\n&",   0, 0, 0,

   // Following indicates the end of the list:
   0, 0, 0, 0, 0, 0
};

/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
VML_DisplayList( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zVIEW  vEdWrk;
   zCHAR  szShowStr[ 10 ];
   zSHORT nRC;
   zLONG  lEntityCount, lSelectedCount;
   zCHAR  szMsg[ 100 ];

   mGetWorkView( &vEdWrk, vSubtask );
   mGetProfileView( &vProfileXFER );
   szShowStr[ 0 ] = 0;

   // Build a string that contains each of the VML types that the user
   // wants shown.

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  "VML_ShowDeclarations", "Y" ) == 0 )
   {
      strcat_s( szShowStr, sizeof( szShowStr ), "D" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, "VML_ShowControlStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, sizeof( szShowStr ), "C" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, "VML_ShowViewStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, sizeof( szShowStr ), "V" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  "VML_ShowEntityStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, sizeof( szShowStr ), "E" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  "VML_ShowOperators", "Y" ) == 0 )
   {
      strcat_s( szShowStr, sizeof( szShowStr ), "O" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer,
                                  "VML_ShowOther", "Y" ) == 0 )
   {
      strcat_s( szShowStr, sizeof( szShowStr ), "?" );
   }

   lEntityCount = 0;
   lSelectedCount = 0;

   // szShowStr lists all the types that the user wants shown. eg "CV?" means
   // that the user wants all control, view, and 'other' vml statements
   // shown.
   for ( nRC = SetCursorFirstEntity( vProfileXFER, "VML_Text", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, "VML_Text", 0 ) )
   {
      zCHAR szType[ 5 ];

      lEntityCount++;

      GetStringFromAttribute( szType, sizeof( szType ), vProfileXFER, "VML_Text", "Type" );
      if ( zstrchr( szShowStr, *szType ) )
      {
         SetSelectStateOfEntity( vProfileXFER, "VML_Text", TRUE );
         lSelectedCount++;
      }
      else
         SetSelectStateOfEntity( vProfileXFER, "VML_Text", FALSE );
   }

   SetCursorFirstSelectedEntity( vProfileXFER, "VML_Text", 0 );
   LB_SetDisplaySelectSet( vSubtask, "TextList", DISP_SELECT_SET );
   LB_SetSelectSelectSet( vSubtask, "TextList", SEL_SELECT_SET );

   sprintf_s( szMsg, sizeof( szMsg ), "Listing %ld of %ld", lSelectedCount, lEntityCount );
   MB_SetMessage( vSubtask, 0, szMsg );
   return( 0 );

} // VML_DisplayList

zOPER_EXPORT zSHORT OPERATION
VML_BuildTextList( zVIEW vSubtask )
{
   zSHORT k;
   zVIEW  vProfileXFER;

   mGetProfileView( &vProfileXFER );

   // Add each of the 'standard' VML statements to the text list.
   for ( k = 0; g_VML_TextStrings[ k ].pszText != 0; k++ )
   {
      if ( SetCursorFirstEntityByString( vProfileXFER, "VML_Text", "Text",
                                         g_VML_TextStrings[ k ].pszText, 0 )
                                                              != zCURSOR_SET )
      {
         // Text doesn't exist, so create it.
         CreateEntity( vProfileXFER, "VML_Text", zPOS_LAST );

         SetAttributeFromString( vProfileXFER, "VML_Text", "Text",
                                 g_VML_TextStrings[ k ].pszText );
         SetAttributeFromString( vProfileXFER, "VML_Text", "Type",
                                 g_VML_TextStrings[ k ].pszType );
         SetAttributeFromString( vProfileXFER, "VML_Text", "UserDefined", "N" );

         if ( g_VML_TextStrings[ k ].pszInsertText )
            SetAttributeFromString( vProfileXFER, "VML_Text", "InsertText",
                                    g_VML_TextStrings[ k ].pszInsertText );

         if ( g_VML_TextStrings[ k ].pszInsertTextFull )
            SetAttributeFromString( vProfileXFER, "VML_Text", "InsertTextFull",
                                    g_VML_TextStrings[ k ].pszInsertTextFull );

         if ( g_VML_TextStrings[ k ].pszCC_Code )
            SetAttributeFromString( vProfileXFER, "VML_Text", "CC_Code",
                                    g_VML_TextStrings[ k ].pszCC_Code );

         if ( g_VML_TextStrings[ k ].pszCC_FirstChar )
            SetAttributeFromString( vProfileXFER, "VML_Text", "CC_FirstChar",
                                    g_VML_TextStrings[ k ].pszCC_FirstChar );
      }
   }

   OrderEntityForView( vProfileXFER, "VML_Text", "Text A" );
   VML_DisplayList( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_Postbuild( zVIEW vSubtask )
{
   zVIEW vEdWrk;

   // Get the subtask view for the parent editor window and give it another
   // name at the current subtask level so we can easily retrieve it.
   GetViewByName( &vEdWrk, "__EditorSubtask", vSubtask, zLEVEL_TASK );
   SetNameForView( vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_SUBTASK );

   VML_DisplayList( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_Close( zVIEW vSubtask )
{
   fnSaveSubwindowPosition( vSubtask, szlVML_Insert );
   return( 0 );
}

// Insert the currently selected text.
zSHORT LOCALOPER
fnInsertVML_Text( zVIEW      vSubtask,
                  zVIEW      vEdWrk,
                  zVIEW      vProfileXFER,
                  ZDrTBEdt   *oEditor )
{
   zCHAR      szInsertString[ 1000 ];
   zPCHAR     pszStrBegin;
   zPCHAR     pszStrEnd;
   zLONG      lIndex;
   zLONG      lOriginalCol;
   zLONG      lOriginalLine;
   zLONG      lTabCount, lTabSize = 4;
   zLONG      lPositionCursor;
   CString    strCompleteCommand;

   szInsertString[ 0 ] = 0;

   if ( lTabSize > 1 ) // don't ask why ... it's hard to explain
      lTabSize--;

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlVML_InsertFull, "Y" ) == 0 )
   {
      GetStringFromAttribute( szInsertString, sizeof( szInsertString ), vProfileXFER, szlVML_Text, szlInsertTextFull );
   }

   if ( szInsertString[ 0 ] == 0 )
   {
      GetStringFromAttribute( szInsertString, sizeof( szInsertString ), vProfileXFER, szlVML_Text, szlInsertText );
   }

   if ( szInsertString[ 0 ] == 0 )
   {
      GetStringFromAttribute( szInsertString, sizeof( szInsertString ), vProfileXFER, szlVML_Text, szlText );
   }

   if ( szInsertString[ 0 ] == 0 )
      return( 0 );

   lPositionCursor = (zstrchr( szInsertString, '&' ) != NULL);
   oEditor->GetCursorPosition( &lOriginalLine, &lOriginalCol, &lIndex );

   // Tabs have to be replaced by a number of blanks.
   lTabCount = GetTabsInLine( oEditor, lOriginalCol );

   lOriginalCol -=lTabCount;
   lOriginalCol += (lTabCount * lTabSize);

   // Insert each line seperately.  Setup insert string line by line.
   strCompleteCommand.Empty( );
   for ( pszStrBegin = szInsertString; *pszStrBegin; pszStrBegin = pszStrEnd )
   {
      zBOOL bMoreLines;

      // Find the end of the current line by looking for the '\n' zCHAR or
      // the null terminator.
      for ( pszStrEnd = pszStrBegin;
            *pszStrEnd != '\n' && *pszStrEnd != '\r' && *pszStrEnd != 0;
            pszStrEnd++ )
      {
         ;  // nothing needs to be done here
      }

      // If we found a newline char, then change it to a null-terminator and
      // bump up pszStrEnd to point to the next char.
      if ( *pszStrEnd )
      {
         // If we found a carriage return, then a line feed is sure to follow
         // so we need to consider both chars.
         if ( *pszStrEnd == '\r' )
            *pszStrEnd++ = 0;

         *pszStrEnd++ = 0;
         bMoreLines = TRUE;
      }
      else
         bMoreLines = FALSE;

      // Setup the string that has to be inserted
      strCompleteCommand += pszStrBegin;
      //oEditor->InsertItem( pszStrBegin );

      if ( bMoreLines )
      {
         zCHAR  szTemp[ 100 ] = "\n";
         zSHORT k = (zSHORT) zstrlen( szTemp );

         // We need to add the new-line and then a spaces to match original column position.

         zmemset( &szTemp[ k ], ' ', lOriginalCol );
         szTemp[ k + lOriginalCol ] = 0;

         strCompleteCommand += szTemp;
         //oEditor->InsertItem( szTemp );
      }
   } // for...

   lPositionCursor = strCompleteCommand.Find( _T( "&" ) );
   if ( lPositionCursor >= 0 )
   {  // Get rid of that "&" so we don't have to call DeleteTextRange
      strCompleteCommand.SetAt( lPositionCursor, ' ' );
   }

   oEditor->InsertItem( strCompleteCommand );

   if ( lPositionCursor >= 0 )
   {
      oEditor->SetCursorPositionByIndex( lIndex + lPositionCursor );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_InsertText( zVIEW vSubtask )
{
   zVIEW      vEdWrk;
   zVIEW      vEditorSubtask;
   zVIEW      vProfileXFER;
   zVIEW      vTemp;
   ZDrTBEdt   *oEditor;

   mGetProfileView( &vProfileXFER );
   mGetWorkView( &vEdWrk, vSubtask );
   GetParentWindow( &vEditorSubtask, vSubtask );
   oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vEditorSubtask,
                                                      EDIT_CONTROL_NAME );

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlReadOnly, "Y" ) != 0 &&
        CompareAttributeToString( vEdWrk, szlBuffer, szlActiveStatus, "N" ) != 0 )
   {
      fnInsertVML_Text( vSubtask, vEdWrk, vProfileXFER, oEditor );
   }

   // Set Focus to Editor
   GetParentWindow( &vTemp, vSubtask );
   SetFocusToCtrl( vTemp, EDIT_CONTROL_NAME );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_ShowPopupMenu( zVIEW vSubtask )
{
// POINT  pt;
   CPoint pt( GetMessagePos( ) );
// pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "Popup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_CreateText_Exit( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   mGetProfileView( &vProfileXFER );
   OrderEntityForView( vProfileXFER, "VML_Text", "Text A" );
   return( 0 );

} // VML_CreateText_Exit

zOPER_EXPORT zSHORT OPERATION
VML_CreateText_DeleteAll( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zSHORT nRC;

   if ( MessagePrompt( vSubtask, "ED0101", "Zeidon Editor",
                       "Delete All VML Keywords?", 0, zBUTTONS_YESNO,
                       zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   mGetProfileView( &vProfileXFER );
   for ( nRC = SetCursorFirstEntity( vProfileXFER, "VML_Text", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, "VML_Text", 0 ) )
   {
      DeleteEntity( vProfileXFER, "VML_Text", zREPOS_NONE );
   }

   return( 0 );

} // VML_CreateText_DeleteAll

zOPER_EXPORT zSHORT OPERATION
VML_SelectItem( zVIEW vSubtask )
{
   MB_SetMessage( vSubtask, 0, "Press F1 for Help on this item." );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_StartHelp( zVIEW vSubtask )
{

   zCHAR  szBuffer[60];
   zPCHAR pszText;

   zSHORT nRC;
   zSHORT k;
   zSHORT j;

   zVIEW  vProfileXFER;

   // GET ADDRESS OF THE STRING FOR THE TEXT IN THE LIST BOX
   mGetProfileView( &vProfileXFER );
   GetAddrForAttribute( &pszText,       /* Return Address */
                        vProfileXFER,  /* View Id */
                        "VML_Text",    /* Entity name */
                        "Text" /* Attribute Name */ );

   // EXTRACT FIRST ALL CAP WORD FROM pszText
   k = 0;
   j = 0;

   // look for first cap
   while ( !isupper( pszText[ k ] ) &&  /* not upper case */
           pszText[ k ] != 0 )          /* not null terminator */
   {
      k++;
   }

   // if no cap is found, no help is available. get out now.
   if ( pszText[ k ] == 0)
   {
      MB_SetMessage( vSubtask, 0, "Help for this operation is not currently available." );
      return( 0 );
   }

   // copy first all cap word into the buffer
   while ( isupper( pszText[ k ] )  &&      /* upper case */
           pszText[ k ] != 0 &&             /* not null terminator */
           j < 59 )                         /* room left in buffer */
   {
      szBuffer[ j++ ] = pszText[ k++ ];
   }

   szBuffer[ j ] = 0;

   // CALL HELP
   nRC = StartZeidonHelp( vSubtask, szBuffer );

   // IF HELP WAS NOT FOUND, NOTIFY THE USER.
   if ( nRC == zCALL_ERROR )
   {
      MB_SetMessage( vSubtask, 0, "Help for this item is not available." );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ErrList_GetFocus( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zLONG  hFile;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zPCHAR psz;

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
      return( 0 );         // We haven't built the window yet so exit.

   if ( CheckExistenceOfEntity( vEdWrk, "ErrorList" ) == zCURSOR_SET )
      // Error list is OK so exit.
      return( 0 );

   CreateEntity( vEdWrk, "ErrorList", zPOS_LAST );

   // Create the error file name.
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );
   psz = &szFileName[ zstrlen( szFileName ) - 3 ];
   zstrcpy( psz, "ERR" );       // Change ".VML" to ".ERR".

   hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( hFile == -1 )
      return( 0 );         // No error file means no error messages.

   // Parse each line into seperate messages.
   while ( SysReadLine( vSubtask, &psz, hFile ) == 1 )
   {
      zPCHAR psz2;

      // Sample error message:
      // c:\9j\a\tz\TZDBHTSD.VML(425) : error 539: An operation blah blah...

      // Look for open paren.  If none found skip the line.
      psz2 = strchr( psz, '(' );
      if ( psz2 == 0 )
         continue;

      // Find close paren.
      psz2++;  // Skip open paren.
      psz = strchr( psz2, ')' );
      if ( psz == 0 )
         continue;

      *psz++ = 0; // Set close paren to 0 and bump up pointer.

      // Is line number numeric???
      // When no compilation is needed compiler generates
      // a message "Skipping ( no relevant changes detected )"
      zBOOL bIsNumeric = TRUE;
      zPCHAR psz3 = psz2;

      while ( *psz3 && bIsNumeric == TRUE )
      {
         if ( isdigit( *psz3 ) == 0 )
         {
            bIsNumeric = FALSE;
         }
         psz3++;
      }

      if ( !bIsNumeric )
         continue;

      // We have an error message so create entity.
      CreateEntity( vEdWrk, "Error", zPOS_LAST );

      SetAttributeFromString( vEdWrk, "Error", "LineNumber", psz2 );

      // Skip first ':'.
      psz = strchr( psz, ':' );
      if ( psz == 0 )
         continue;

      // Skip second ':'.
      psz = strchr( ++psz, ':' );
      if ( psz == 0 )
         continue;

      psz += 2;  // Skip ": ".
      SetAttributeFromString( vEdWrk, "Error", "ErrorText", psz );
   }

   SysCloseFile( vSubtask, hFile, 0 );
   RefreshCtrl( vSubtask, "ErrorList" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ErrList_Postbuild( zVIEW vSubtask )
{
   zVIEW vEdWrk;

   // Get the subtask view for the parent editor window and give it another
   // name at the current subtask level so we can easily retrieve it.
   GetViewByName( &vEdWrk, "__EditorSubtask", vSubtask, zLEVEL_TASK );
   SetNameForView( vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_SUBTASK );

   // Call the GetFocus code to build the error list.
   ErrList_GetFocus( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ErrList_Close( zVIEW vSubtask )
{
   fnSaveSubwindowPosition( vSubtask, szlErrorList );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ErrList_ShowError( zVIEW vSubtask )
{
   zVIEW      vEdWrk;
   zVIEW      vEditorSubtask;
   ZDrTBEdt *oEditor;
   zLONG      lErrorLine, lLine, lCol, lIndex;

   lLine = lCol = lIndex = 0;

   mGetWorkView( &vEdWrk, vSubtask );
   GetParentWindow( &vEditorSubtask, vSubtask );

   GetIntegerFromAttribute( &lErrorLine, vEdWrk, "Error", "LineNumber" );
   lErrorLine--;

   oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vEditorSubtask,
                                                      EDIT_CONTROL_NAME );
   // Set cursor only if it is not already there, otherwise the Editctrl will
   // scroll up for one line and this will drive "evil" crazy.
   oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );
   if ( lErrorLine != lLine )
   {
      oEditor->SetCursorPositionByLine( lErrorLine, 0 );
   }

   SetFocusToCtrl( vEditorSubtask, EDIT_CONTROL_NAME );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zTZEDFRMD_OpenFile( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
// zVIEW  vTempEdWrk;
   zVIEW  vParentSubtask;
   zSHORT nRC;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zULONG ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT;

   mGetWorkView( &vEdWrk, vSubtask );
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );

   if ( OperatorPromptForFile( vSubtask, szFileName, zMAX_FILENAME_LTH + 1,
                               "VML files|*.VML|Source Files|*.VML;*.C|"
                               "All files|*.*||", 0, ulZFlags) != 1 )
   {
      return( 0 );
   }

// CreateViewFromViewForTask( &vTempEdWrk, vEdWrk ); why??? dks 2004.12.06
   nRC = SetCursorFirstEntityByString( vEdWrk, szlBuffer, szlFileName,
                                       szFileName, 0 );
   if ( nRC == zCURSOR_SET )
   {
      zCHAR sz[ zMAX_FILENAME_LTH + 100 ];

      zsprintf( sz, "File '%s' is already being edited.  Open new window "
                "in Read-Only mode?", szFileName );
      nRC = OperatorPrompt( vSubtask, szlZeidonEditor, sz, TRUE,
                            zBUTTONS_YESNOCANCEL, zRESPONSE_NO, zICON_EXCLAMATION );

      if ( nRC == zRESPONSE_CANCEL )
      {
      // DropView( vTempEdWrk );
         return( 0 );
      }

      if ( nRC == zRESPONSE_NO )
      {
         GetIntegerFromAttribute( (zPLONG) &vSubtask, vEdWrk, szlBuffer, "vSubtask" );
      // DropView( vTempEdWrk );
         SetFocusToCtrl( vSubtask, EDIT_CONTROL_NAME );
         return( 0 );
      }

      // Indicate that file file should be read-only.
      SetAttributeFromString( vEdWrk, szlEditor, "OpenReadOnly", "Y" );
   }

// DropView( vTempEdWrk );

   // Set attribute value so that next editing session knows what file to
   // open.
   SetAttributeFromString( vEdWrk, szlEditor, "OpenFileName", szFileName );

   // Start up another editor.  We use the parent subtask of the current
   // editor so that the new editor is a sibling of the current editor.
   GetParentWindow( &vParentSubtask, vSubtask );
   SetWindowActionBehavior( vParentSubtask, zWAB_StartModelessSubwindow | zWAB_ProcessImmediateAction,
                            "TZEDFRMD", "TZEDFRMD" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zTZEDFRMD_FileClose( zVIEW vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_SetInsert( zVIEW vSubtask )
{
   //SysMessageBox( "dgc", "SetInsert", 0 );
   //TraceLineS( "(tzedfrmd)", "SetInsert" );
   MB_SetMessage( vSubtask, INSERTMODE_DIL, "Ins" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_SetOverstrike( zVIEW vSubtask )
{
   //SysMessageBox( "dgc", "SetOver", 0 );
   //TraceLineS( "(tzedfrmd)", "SetOverstrike" );
   MB_SetMessage( vSubtask, INSERTMODE_DIL, "Ovr" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SaveFile( zVIEW vSubtask )
{
   zLONG      lChanged;
   zCHAR      szFileName[ zMAX_FILENAME_LTH + 1];
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   oEditor->IsObjectChanged( (BOOL *) &lChanged );
   if ( lChanged && !oEditor->IsReadOnly( ) )
   {
      //if ( fnTZEDFRMD_SaveFile( vSubtask, oEditor, szFileName, PARSE_FILE ) < 0 )
      // KJS 06/19/12 - I would like SaveFile to only save the file not prompt for parsing.  This is
      // because then everytime the user "saves" it prompts to parse either c or vml and always parses
      // one of them.  I don't want to always parse.
      if ( fnTZEDFRMD_SaveFile( vSubtask, oEditor, szFileName, sizeof( szFileName ), 0 ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      SetUpdateFlagInMeta( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FileSaveAs( zVIEW vSubtask )
{
   zVIEW      vEdWrk;
   zCHAR      szMsg[ zMAX_FILENAME_LTH + 40 ];
   zCHAR      szFileName[ zMAX_FILENAME_LTH + 1];
   zCHAR      szExtension[ 3 ];
   zULONG     ulZFlags = 0;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   mGetWorkView( &vEdWrk, vSubtask );

   if ( g_bIsFileNew == TRUE )
   {
      GetStringFromAttribute( szFileName, sizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );
   }
   else
   {
      GetStringFromAttribute( szExtension, sizeof( szExtension ), vEdWrk, szlBuffer, szlLanguageType);
      strcpy_s( szFileName, sizeof( szFileName ), "NONAME." );
      if ( zstrcmp( szExtension, szlC_File ) != 0 )
         strcpy_s( szExtension, sizeof( szExtension ), szlVML_FileExtension );
      strcat_s( szFileName, sizeof( szFileName ), szExtension );
   }

   ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT | zOFN_SAVEAS;

   if ( OperatorPromptForFile( vSubtask, szFileName, zMAX_FILENAME_LTH + 1,
                               "VML files|*.VML|Source Files|*.VML;*.C|"
                               "All files|*.*||", 0, ulZFlags) != 1 )
      return( 0 );

   if ( oEditor->SaveObjectAs( szFileName ))
   {
      g_bIsFileNew = FALSE;
      sprintf_s( szMsg, sizeof( szMsg ), "File saved as '%s'.", szFileName);
      MB_SetMessage( vSubtask, MAIN_DIL, szMsg );
      SetAttributeFromString( vEdWrk, szlBuffer, szlFileName, szFileName);
      SetWindowCaptionTitle( vSubtask, "Zeidon Editor", szFileName);
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SaveAndParse( zVIEW vSubtask, zCPCHAR cpcGenLang )
{
   zSHORT     nRC = 0;
   zLONG      lParseAction = zPARSE_ACTION_FULL;
   zLONG      lParseLimit  = zPARSE_LIMIT_SOURCE;
   zLONG      lParseSource = zPARSE_SOURCE_FILE;
   zCHAR      szFileName[ zMAX_FILENAME_LTH + 1 ];
   zPCHAR     psz;
   zCHAR      sz[ 300 ];
   zVIEW      vEdWrk;
   zVIEW      vSource;
   zVIEW      vWindow;
   zBOOL      bReadOnly = FALSE;
   zLONG      lMetaType;
   zLONG      lChanged;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
   {
      strcpy_s( sz, sizeof( sz ), "TZEDWRKO not Available at _zfParseFile." );
      MessagePrompt( vSubtask, "ED0001", szlZeidonEditor, sz, 1, 0, 0, zICON_EXCLAMATION );
      return( 0 );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) != 0 )
   {
      strcpy_s( sz, sizeof( sz ), "Parse only valid with VML Files." );
      MessagePrompt( vSubtask, "ED0002", szlZeidonEditor,
                     sz, 1, 0, 0, zICON_EXCLAMATION );
      nRC = 1;
      goto EndOfFunction;
   }

   // Get the Source Meta for the current buffer.
   GetIntegerFromAttribute( (zPLONG) &vSource, vEdWrk, szlBuffer, szlSourceViewID );
   if ( vSource == 0 )
   {
      MessagePrompt( vSubtask, "ED0106", szlZeidonEditor,
                     "Cannot parse VML that has been opened via File/Open",
                     TRUE, zBUTTONS_OK, 0, zICON_ERROR );
      nRC = 1;
      goto EndOfFunction;
   }

   // Just look if there is anything changed
   oEditor->IsObjectChanged( (BOOL *) &lChanged );
   if ( lChanged && !oEditor->IsReadOnly( ) )
   {
      oEditor->SaveObject( );
      SetUpdateFlagInMeta( vSubtask );
   }

   GetStringFromAttribute( szFileName, sizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );

   // Drop the '.VML' from the file name.
   psz = szFileName + zstrlen( szFileName );
   psz -= 4;
   *psz = 0;

   // Now set up psz to point to the beginning of the filename.
   while ( psz > szFileName && *psz != '\\' )
      psz--;

   psz++;

   GetIntegerFromAttribute( &lMetaType, vEdWrk, szlBuffer, szlMetaType );
   sprintf( sz, "Parsing File '%s.VML'...", szFileName );
   MB_SetMessage( vSubtask, MAIN_DIL, sz );

   if ( !MiGetUpdateForView( vSource ) )
   {
      bReadOnly = TRUE;
      SetViewUpdate( vSource );
   }

   nRC = (zSHORT) ParseSource( vSubtask, vSource,
                               lParseAction, lParseLimit, lParseSource,
                               psz, "", lMetaType, 0, cpcGenLang );

   if ( bReadOnly )
      SetViewReadOnly( vSource );

   // If we get here then we want to make sure that the error list needs to
   // be reloaded.
   if ( CheckExistenceOfEntity( vEdWrk, "ErrorList" ) == zCURSOR_SET )
      DeleteEntity( vEdWrk, "ErrorList", zREPOS_FIRST );

   if ( nRC < 0 )
   {
      SetCursorFirstEntity( vEdWrk, "Error", 0 );

      // Fire up the error list window.
      TZEDFRMD_OpenSubwindow( vSubtask );
      SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow |
                               zWAB_ProcessImmediateAction, szlTZEDFRMD,
                               "ErrorList" );

      sprintf_s( sz, sizeof( sz ), "Error parsing File '%s.VML'", szFileName );
      MB_SetMessage( vSubtask, MAIN_DIL, sz );

      // There is a parse error.  If performing exit, then show one message,
      // otherwise show message that allows user to go directly to the error file.
      if ( FALSE ) //> performing exit.
      {
         strcpy_s( sz, sizeof( sz ), "Parse completed with Errors.\n\n"
                   "See Error File for list of errors." );
         nRC = MessagePrompt( vSubtask, "ED0002", szlZeidonEditor,
                              sz, 1, 0, 0, zICON_ERROR );
      }

      nRC = 1;
   }
   else
   {
      // Close Error List
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ) >= 0 )
         SetWindowActionBehavior( vWindow, zWAB_ReturnToParent |
                                  zWAB_ProcessImmediateAction |
                                  zWAB_ProcessImmediateReturn, 0, 0 );

      sprintf_s( sz, sizeof( sz ), "Parse completed successfully for: '%s.VML'", szFileName );
      MB_SetMessage( vSubtask, MAIN_DIL, sz );
      MessagePrompt( vSubtask, "ED0003", szlZeidonEditor,
                     sz, 0, 0, 0, zICON_INFORMATION );
      nRC = 0;
   }

EndOfFunction:
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
SaveAndParseC( zVIEW vSubtask )
{
   return( SaveAndParse( vSubtask, "C" ) );
}

zOPER_EXPORT zSHORT OPERATION
SaveAndParseJava( zVIEW vSubtask )
{
   return( SaveAndParse( vSubtask, "Java" ) );
}

zOPER_EXPORT zSHORT OPERATION
Generate( zVIEW vSubtask, zCPCHAR cpcGenLang )
{
   ZDrTBEdt *oEditor;
   zVIEW    vEdWrk;
   zVIEW    vSource;
   zCHAR    szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR    sz[ 100 ];
   zPCHAR   psz;
   zLONG    lChanged;
   zLONG    lType = 0;
   zSHORT   nRC;

   oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
   {
      strcpy_s( sz, sizeof( sz ), "TZEDWRKO not Available at _zfGenerateCFile." );
      SysMessageBox( vSubtask, szlZeidonEditor, sz, 1 );
      return( 0 );
   }

   // Get the Source Meta for the current buffer.
   GetIntegerFromAttribute( (zPLONG) &vSource, vEdWrk, szlBuffer,
                            szlSourceViewID );
   if ( vSource == 0 )
   {
      MessagePrompt( vSubtask, "ED0107", szlZeidonEditor,
                     "Cannot generate VML that has been opened via File/Open",
                     TRUE, zBUTTONS_OK, 0, zICON_ERROR );
      nRC = 1;
      goto EndOfFunction;
   }

   //>SetUserFlagsOn( zUF_CurrentlyParsing );

   oEditor->IsObjectChanged( (BOOL *) &lChanged );
   if ( lChanged )
   {
      MB_SetMessage( vSubtask, MAIN_DIL, "Saving file..." );
      oEditor->SaveObject( );
      SetUpdateFlagInMeta( vSubtask );
   }

   GetIntegerFromAttribute( &lType, vEdWrk, szlBuffer, szlMetaType );
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );

   // Drop the '.VML' from the file name.
   psz = szFileName + zstrlen( szFileName );
   psz -= 4;
   *psz = 0;

   // Now set up psz to point to the beginning of the filename.
   while ( psz > szFileName && *psz != '\\' )
      psz--;

   psz++;

   nRC = Generate3GL( vSubtask, vSubtask, vSource, psz, cpcGenLang, lType );

   if ( nRC < 0 )
   {
      strcpy_s( sz, sizeof( sz ), "Errors in Generate." );
      strcat_s( sz, sizeof( sz ), "\n\nSee Messages in Error Box" );
      SysMessageBox( vSubtask, szlZeidonEditor, sz, 1 );
      nRC = 1;
   }
   else
   {
      strcpy_s( sz, sizeof( sz ), "Generate completed successfully." );
      MessagePrompt( vSubtask, "ED0003", szlZeidonEditor, sz, 0, 0, 0, zICON_INFORMATION );
      nRC = 0;
   }

EndOfFunction:

   //> SetUserFlagsOff( zUF_CurrentlyParsing );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
GenerateC( zVIEW vSubtask )
{
   return( Generate( vSubtask, "C" ) );
}

zOPER_EXPORT zSHORT OPERATION
GenerateJava( zVIEW vSubtask )
{
   return( Generate( vSubtask, "Java" ) );
}

/*************************************************************************************************
**
**    OPERATION: GenerateCompileJava
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateCompileJava( zVIEW vSubtask )
{
   zVIEW    vLPLR;
   zSHORT   nRC;
   zCHAR    szSystemApp[ 65 ] = { 0 };
   zCHAR    szJavaCompileBat[ 256 ] = { 0 };
   zCHAR    szLPLR_Name[ 33 ] = { 0 };

   nRC = Generate( vSubtask, "Java" );

   if (nRC != 0)
      return ( nRC );

   // KJS 08/21/14
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetVariableFromAttribute( szLPLR_Name, 0, 'S', 33, vLPLR, "LPLR", "Name", "", 0 );
   ZeidonStringCopy( szSystemApp, 1, 0, "[App.", 1, 0, 65 );
   ZeidonStringConcat( szSystemApp, 1, 0, szLPLR_Name, 1, 0, 65 );
   ZeidonStringConcat( szSystemApp, 1, 0, "]", 1, 0, 65 );

   // KJS 08/21/14 - Get JavaCompileBat from the zeidon.ini and run it (currently we are using maven for compile).
   SysReadZeidonIni( -1, szSystemApp, "JavaCompileBat", szJavaCompileBat, sizeof( szJavaCompileBat ) );
   system( szJavaCompileBat );

   return nRC;

} // GenerateCompileJava

/*************************************************************************************************
**
**    OPERATION: CompileJava
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CompileJava( zVIEW vSubtask )
{
   zVIEW    vLPLR;
   zCHAR    szSystemApp[ 65 ] = { 0 };
   zCHAR    szJavaCompileBat[ 256 ] = { 0 };
   zCHAR    szLPLR_Name[ 33 ] = { 0 };

   // KJS 08/21/14
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetVariableFromAttribute( szLPLR_Name, 0, 'S', 33, vLPLR, "LPLR", "Name", "", 0 );
   ZeidonStringCopy( szSystemApp, 1, 0, "[App.", 1, 0, 65 );
   ZeidonStringConcat( szSystemApp, 1, 0, szLPLR_Name, 1, 0, 65 );
   ZeidonStringConcat( szSystemApp, 1, 0, "]", 1, 0, 65 );

   // KJS 08/21/14 - Get JavaCompileBat from the zeidon.ini and run it (currently we are using maven for compile).
   SysReadZeidonIni( -1, szSystemApp, "JavaCompileBat", szJavaCompileBat, sizeof( szJavaCompileBat ) );
   system( szJavaCompileBat );

   return 0;

} // CompileJava


zOPER_EXPORT zSHORT OPERATION
zPrint( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   oEditor->PrintObject( );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActivateBasicsHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActivateUsersGuide( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActivateProgrammersRef( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, PROG_REF );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActivateOperationsRef( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, OPER_REF );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OBJ_Prebuild( zVIEW vSubtask )
{
   zSHORT   nRC;
   zVIEW    vOp;
   zVIEW    vSource;

   nRC = GetViewByName( &vOp, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
      DropView( vOp );

   RetrieveViewForMetaList( vSubtask, &vOp, zREFER_LOD_META );
   OrderEntityForView( vOp, "W_MetaDef", "Name A" );
   SetNameForView( vOp, "TZEDFRMD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );

   // Make sure that there is no TZOPSRCO.
   nRC = GetViewByName( &vSource, "TZOPSRCO", vSubtask, zLEVEL_TASK );
   if ( vSource )
      DropMetaOI( vSubtask, vSource );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OBJ_PasteObjectName( zVIEW vSubtask )
{
   zVIEW    vList;
   zVIEW    vTemp;
   zVIEW    vEdWrk;
   zVIEW    vEditorSubtask;
   zPCHAR   szOption;
   zPCHAR   szName;
   CString  zs;
   ZDrTBEdt *oEditor = 0;

   GetViewByName( &vList, szlTZCMLPLO_List, vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vList, szlW_MetaDef ) != zCURSOR_SET )
      return( 0 );

   // Get the subtask for the editor window.
   GetParentWindow( &vTemp, vSubtask );
   GetParentWindow( &vEditorSubtask, vTemp );
   mGetWorkView( &vEdWrk, vEditorSubtask );
   oEditor = (ZDrTBEdt *) GetActiveX_WrapperInstance( vEditorSubtask,
                                                      EDIT_CONTROL_NAME );

   GetAddrForAttribute( &szOption, vEdWrk, szlBuffer, szlPasteObjectOption );
   GetAddrForAttribute( &szName, vList, szlW_MetaDef, szlName );

   switch ( *szOption )
   {
      case 'N':
         zs = szName;
         oEditor->InsertItem( zs );
         break;

      case 'B':
         zs = "VIEW ";
         zs += szName;
         zs += " BASED ON LOD ";
         zs += szName;
         oEditor->InsertItem( zs );
         break;

      case 'G':
         zs = "GET VIEW ";
         zs += szName;
         zs += " NAMED \"";
         zs += szName;
         zs += "\"";
         oEditor->InsertItem( zs );
         break;

      case 'S':
         zs = "NAME VIEW ";
         zs += szName;
         zs += " \"";
         zs += szName;
         zs += "\"";
         oEditor->InsertItem( zs );
         break;
   }

   DropView( vList );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FileNew( zVIEW vSubtask )
{
     // TMV 1998.06.15
   BOOL     bObjectChanged = FALSE;
   zPCHAR   szMsg = {"Do you want to save the changes?"};
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   // Check if file changed.
   oEditor->IsObjectChanged( &bObjectChanged );
   if ( bObjectChanged == TRUE )
   {
      if ( OperatorPrompt( vSubtask, szlZeidonEditor, szMsg, TRUE,
                           zBUTTONS_YESNO, zRESPONSE_YES,
                           zICON_EXCLAMATION ) == zRESPONSE_YES)
      {
         if ( g_bIsFileNew != TRUE )
         {
            oEditor->SaveObject( ); // Promp user to parse current Source
            SetUpdateFlagInMeta( vSubtask );
         }
         else
         {
            FileSaveAs( vSubtask );
            g_bIsFileNew = FALSE;
         }
      }
   }

   g_bIsFileNew = oEditor->CloseObject( );
   if ( g_bIsFileNew )
   {
      zVIEW  vEdWrk;
      zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
      zCHAR  szExtension[ zMAX_FILENAME_LTH + 1 ];

      mGetWorkView( &vEdWrk, vSubtask );
      GetStringFromAttribute( szExtension, sizeof( szExtension ), vEdWrk, szlBuffer, szlLanguageType);
      strcpy_s( szFileName, sizeof( szFileName ), "NONAME." );
      if ( zstrcmp( szExtension, szlC_File ) != 0 )
         strcpy_s( szExtension, sizeof( szExtension ), szlVML_FileExtension );
      else
         strcpy_s( szExtension, sizeof( szExtension ), szlVML_FileExtension );
      strcat_s( szFileName, sizeof( szFileName ), szExtension );
      SetAttributeFromString( vEdWrk, szlBuffer, szlFileName, szFileName);
      SetWindowCaptionTitle( vSubtask, "Zeidon Editor", szFileName);
   }

   return( 0 );
}

//
// Handles a pressed ESC-Key
// There are cases which must be handled
// 1. There is a Modeless Dialog which is owned by the ocx. If so close it
// 2. User wants to deselect current selection
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_Cancel( zVIEW vSubtask )
{
   zSHORT   bSubwindowOpen = FALSE;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   bSubwindowOpen = oEditor->CloseSubWindow( );

   if ( bSubwindowOpen == FALSE )
   {
      oEditor->Deselect( );
   }

   return( 0 );
}

//
// Handles a pressed F1-Key in the operation-insert-dialog
//
zOPER_EXPORT zSHORT OPERATION
OpIns_StartOperationHelp( zVIEW vSubtask )
{

   zVIEW  vEdWrk;
   zCHAR  szOperName[ 40 ];
   zCHAR  szOperTag[ 40 ];
   zCHAR  szOperType[ 5 ];
   zSHORT nRC;

   // FIND OUT WHAT TYPE OF OPERATION LIST IS CURRENTLY DISPLAYED.
   mGetWorkView( &vEdWrk,vSubtask );
   GetStringFromAttribute( szOperType, sizeof( szOperType ), vEdWrk, "OperListType", "Type" );

   // IF IT'S NOT A ZEIDON OPER LIST, GET OUT. NO HELP IS AVAILABLE.
   if ( szOperType[ 0 ] != 'Z' )
   {
      return( 0 );
   }

   // GET THE OPERATION TAG FOR THE CURRENT CURSOR POSITION.
   GetStringFromAttribute( szOperName, sizeof( szOperName ), vEdWrk, "Oper", "Name" );
   strcpy_s( szOperTag, sizeof( szOperTag ), "fn " );
   strcat_s( szOperTag, sizeof( szOperTag ), szOperName );

   // CALL HELP
   nRC = StartZeidonHelp( vSubtask, szOperTag );

   // IF HELP WAS NOT FOUND, NOTIFY THE USER.
   if ( nRC == zCALL_ERROR )
   {
      MB_SetMessage( vSubtask, 0, "Help for this operation is not currently available." );
   }

   return( 0 );
}

// Handles a pressed F3-Key in the Editor.
zOPER_EXPORT zSHORT OPERATION
SearchForward( zVIEW vSubtask )
{
   return( SearchContinue( vSubtask, SRCH_FORWARD ) );
}

// Handles a pressed Shift-F3-Key in the Editor.
zOPER_EXPORT zSHORT OPERATION
SearchBackward( zVIEW vSubtask )
{
   return( SearchContinue( vSubtask, SRCH_BACKWARD ) );
}

// Continue a search that has been started with Find/Replace Dialog.
zOPER_EXPORT zSHORT OPERATION
SearchContinue( zVIEW vSubtask, zLONG lDirectionFlag )
{
   CString  strMsg, strMsgFormat = "Could not find requested string\"%s\"!";
   CString  strStringToFind = "";
   zLONG    lPosition = 0, lLine = 0, lCol = 0;
   zLONG    lSearchBehavior = lDirectionFlag;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   oEditor->GetCursorPosition( &lLine, &lCol, &lPosition );

   zLONG lCharCount = g_strFindWhat.GetLength( );
   strStringToFind = g_strFindWhat;

   if ( g_fr.Flags & FR_WHOLEWORD )
   {
      lSearchBehavior |= SRCH_REGEXPR;
      strStringToFind = "\\b";
      CString strUpper = g_strFindWhat;
      CString strLower = g_strFindWhat;
      strUpper.MakeUpper( );
      strLower.MakeLower( );
      for( int k = 0; k < strUpper.GetLength( ); k++ )
      {
         strStringToFind += "[";
         strStringToFind += strUpper[ k ];
         strStringToFind += strLower[ k ];
         strStringToFind += "]";
      }

      strStringToFind += "[^a-zA-Z_0-9]";
      if ( oEditor->CanCopy( ) )
      {
         CString strHelp;
         LPSTR pszBuffer = strHelp.GetBufferSetLength( lCharCount );
         oEditor->GetTextFromRange( pszBuffer, lPosition, lPosition + lCharCount );
         strHelp.ReleaseBuffer( );
         if ( strHelp.CompareNoCase( g_strFindWhat ) == 0 && g_fr.Flags & FR_DOWN )
         {
            lPosition += lCharCount;
            oEditor->SetCursorPositionByIndex( lPosition );
         }
      }
   }

   if ( g_fr.Flags & FR_MATCHCASE )
   {
      lSearchBehavior |= SRCH_MATCHCASE;
      g_bMatchCase = TRUE;
//    strStringToFind = "\\b";
      strStringToFind = g_strFindWhat;
//    strStringToFind += "[^a-zA-Z_0-9]";
   }

   oEditor->FindTextPosition( strStringToFind, &lPosition, lSearchBehavior );
   if ( lPosition > -1 )
   {
      oEditor->SelectRange( lPosition, g_strFindWhat.GetLength( ) );
   }
   else
   {
      strMsg.Format( strMsgFormat, g_strFindWhat );
      MB_SetMessage( vSubtask, MAIN_DIL, strMsg );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: TZEDFRMD_GotoOperation
//
// Searches current source file for the operation that is referenced by
// cursor position (Cursor in this case means the indicator within the editor).
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_GotoOperation( zVIEW vSubtask )
{

   zLONG    lLine = 0, lCol = 0, lIndex = 0;
   zLONG    lReturnedBuffSize = MAX_TOKEN_LTH;
   CString  strBuffer;
   CString  strOperSrch = "\\bOPERATION[^!-~°§²³´ßäöüÄÖÜ]+%s[^!-~°§²³´ßäöüÄÖÜ]*(";
   LPSTR    szBuffer = NULL;
   CString  strDelimiter = "\t,)( ";
   long     lTBEDTDefaultSearchBehavior = 5;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor == 0 )
      return( 0 );

   oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );

   // return if we are within a comment
   if ( oEditor->IsCommentAtIndex( lIndex ) )
   {
      return( 0 );
   }

   szBuffer = strBuffer.GetBufferSetLength( lReturnedBuffSize );
   lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer,
                                                        lReturnedBuffSize,
                                                        lIndex );

   // In Case of an editor error (e.g. Buffer to small), retry with
   // new buffersize.
   if ( lReturnedBuffSize >= MAX_TOKEN_LTH )
   {
      lReturnedBuffSize++;
      szBuffer = strBuffer.GetBufferSetLength( lReturnedBuffSize );
      lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer,
                                                           lReturnedBuffSize,
                                                           lIndex );
   }

   strBuffer.ReleaseBuffer( );
   zLONG lBufferLth = strBuffer.GetLength( );
   if ( lBufferLth == 0 )
   {
      return( 0 );
   }

   // If positioned on a whitespace go to the next token.
   TCHAR c = 0;

   if ( lCol < lBufferLth )
   {
      for ( c = strBuffer.GetAt( lCol );
            isspace( c ) && lCol < lBufferLth;
            lCol++ )
      {
         c = strBuffer.GetAt( lCol );
      }
   }

   // Now extract the operation name from current column going back and
   // forth to detect the token that seems to be an operation name.
   zLONG lTokenStart    =  0;
   zLONG lTokenEnd      =  0;
   zLONG lCurrentColumn = lCol;

   // Get beginning of operation name.
   while ( lTokenStart == 0 &&
           lCurrentColumn >= 0 &&        // iterate only between lower
           lCurrentColumn < lBufferLth ) // and upper bounds
   {
      c = strBuffer.GetAt(lCurrentColumn);
      if ( !__iscsym( c )) // Alphanumeric + digits + underscore
      {
         lTokenStart = lCurrentColumn + 1;
      }
      else
      {
         lCurrentColumn--;
      }
   }

   // Get the end of the operation name.
   lCurrentColumn = lCol;
   while ( lTokenEnd == 0 &&
           lCurrentColumn >= 0  &&                   // iterate only between lower
           lCurrentColumn < strBuffer.GetLength( ) ) // and upper bounds
   {
      c = strBuffer.GetAt( lCurrentColumn );
      if ( !__iscsym( c ) )  // alphanumeric + digits + underscore
      {
         lTokenEnd = lCurrentColumn;
      }
      else
      {
         lCurrentColumn++;
      }
   }

   if ( lTokenEnd == 0 )
      lTokenEnd = strBuffer.GetLength( );

   // Extract OperationName.
   strBuffer = strBuffer.Mid( lTokenStart, ( lTokenEnd - lTokenStart ) );

   if ( strBuffer.GetLength( ) == 0 )
      return( 0 );

   CString strSearchString;
   strSearchString.Format( strOperSrch, strBuffer );

   lIndex = 0; // search from begin of file
   oEditor->FindTextPosition( strSearchString, &lIndex,
                              lTBEDTDefaultSearchBehavior );

   // Skipping lines where the search text is part of a comment
   while ( oEditor->IsCommentAtIndex( lIndex ) && lIndex > -1 )
   {
      lIndex++;
      oEditor->FindTextPosition( strSearchString, &lIndex,
                                 lTBEDTDefaultSearchBehavior );
   }

   if ( lIndex >= 0 ) // we found something so place the cursor onto it
   {
      oEditor->SetCursorPositionByIndex( lIndex + 12 );
      return( 1 );
   }
   else
   {
      strSearchString.Format( "Operation \'%s\' could not be located", strBuffer );
      MB_SetMessage( vSubtask, MAIN_DIL, strSearchString );
   }

   return( 0 );

} // TZEDFRMD_GotoOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditCut
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditCut( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
   {
      if ( oEditor->CanCopy( ) )
         oEditor->CutText( );
   }

   return( 0 );

}// TZEDFRMD_EditCut

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditPaste
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditPaste( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
   {
      if ( oEditor->CanPaste( ) )
         oEditor->PasteText( );
   }

   return( 0 );

}// TZEDFRMD_EditPaste

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditCopy
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditCopy( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
   {
      if ( oEditor->CanCopy( ) )
         oEditor->CopyText( );
   }

   return( 0 );

} // TZEDFRMD_EditCopy

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditSelectAll
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditSelectAll( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
      oEditor->SelectAll( );

   return( 0 );

} // TZEDFRMD_EditSelectAll

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditDelete
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditDelete( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
   {
      if ( oEditor->CanCopy( ) )
         oEditor->DeleteText( );
   }

   return( 0 );

} // TZEDFRMD_EditDelete

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditUndo
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditUndo( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
   {
      if ( oEditor->CanUndo( ) )
         oEditor->Undo( );
   }

   return( 0 );

} // TZEDFRMD_EditUndo

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditRedo
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditRedo( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
   {
      if ( oEditor->CanRedo( ) )
         oEditor->Redo( );
   }

   return( 0 );

} // TZEDFRMD_EditRedo

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditFind
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditFind( zVIEW vSubtask )
{
   zPCHAR   pch = 0;
   CString  strBuffer;
   zLONG    lBufferLth = 512;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
   {
      if ( oEditor->CanCopy( ) ) // something is selected
      {
         pch = strBuffer.GetBufferSetLength( lBufferLth );
         zLONG lReturn = oEditor->GetSelectedText( pch, lBufferLth );
         while ( lReturn > lBufferLth )
         {
            lBufferLth = lReturn + 1;
            lReturn = oEditor->GetSelectedText( pch, lBufferLth );
         }
      }
   }
   else
   {
      return( -1 );
   }

   if ( pch == 0 )
      pch = g_strFindWhat.GetBufferSetLength( g_strFindWhat.GetLength( ) );

   return( OperatorPromptFindReplace( vSubtask, pch,"", FALSE ) );

} // TZEDFRMD_EditFind

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EditReplace
//
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_EditReplace( zVIEW vSubtask )
{
   CString  strMsg,
            strMsgFormat = "%d occurrences of \"%s\" have been replaced by \"%s\"!";
   zLONG    lSearchBehavior = 0, lPosition = 0, lReplacements = 0;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   FINDREPLACE *pfr = (FINDREPLACE *) GetActionParameters( vSubtask );

   if ( zstrlen(pfr->lpstrReplaceWith ) > 0 )
   {
      if ( oEditor == 0 )
         return( 0 );

      if ( pfr->Flags & FR_REPLACEALL )
      {
         // remember current position
         long lLine = 0, lCol = 0, lIndex = 0;
         oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );
         oEditor->SetCursorPositionByIndex( 0 );

         // replace current selection
         if ( oEditor->CanCopy( ) ) // is something selected?
         {
            lReplacements++;
            oEditor->InsertItem( pfr->lpstrReplaceWith );
         }

         if ( pfr->Flags & FR_MATCHCASE )
         {
            lSearchBehavior |= SRCH_MATCHCASE;
         }

         if ( pfr->Flags & FR_DOWN )
            lSearchBehavior |= SRCH_FORWARD;

         oEditor->FindTextPosition( pfr->lpstrFindWhat, &lPosition, lSearchBehavior );
         while ( lPosition > -1 )
         {
            lReplacements++;
            oEditor->SelectRange( lPosition, zstrlen( pfr->lpstrFindWhat ) );
            if ( oEditor->CanCopy( ) ) // is something selected?
               oEditor->InsertItem( pfr->lpstrReplaceWith );
            oEditor->FindTextPosition( pfr->lpstrFindWhat, &lPosition, lSearchBehavior );
         }
         if ( lReplacements > 0 )
         {
            oEditor->SetCursorPositionByIndex( lIndex );

            strMsg.Format( strMsgFormat,
                           lReplacements,
                           pfr->lpstrFindWhat,
                           pfr->lpstrReplaceWith );
            SysMessageBox( vSubtask, "Replace", strMsg, FALSE );
         }
      }
      else
      if ( oEditor->CanCopy( ) ) // is something selected?
         oEditor->InsertItem( pfr->lpstrReplaceWith );
   }

   return( 0 );

} // TZEDFRMD_EditReplace

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: OptionSettings
//
zOPER_EXPORT zSHORT OPERATION
OptionSettings( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
      oEditor->PropertyDialog( );

   return( 0 );

} // OptionSettings

zOPER_EXPORT zSHORT OPERATION
GotoWindow( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor )
      oEditor->GotoWindow( );

   return( 0 );

} // GotoWindow

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: TZEDFRMD_OpenFindReplace
//
//  Popup common Find/Replace Dialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /* DIALOG */  OPERATION
TZEDFRMD_OpenFindReplace( zVIEW vSubtask )
{
   return( OperatorPromptFindReplace( vSubtask, "","", TRUE ) );

} // TZEDFRMD_OpenFindReplace

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: TZEDFRMD_OnEditFind
//
//  Handles the edit find event
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /* DIALOG */  OPERATION
TZEDFRMD_OnEditFind( zVIEW vSubtask )
{
   CString  strMsg, strMsgFormat = "Could not find requested string\"%s\"!";
   zLONG    lSearchBehavior = 0, lPosition = 0, lLine = 0;
   zLONG    lCol = 0, lCharCount = 0;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );
   FINDREPLACE *pfr = (FINDREPLACE *) GetActionParameters( vSubtask );

   memcpy( &g_fr, pfr, sizeof( FINDREPLACE ) );
   oEditor->GetCursorPosition( &lLine, &lCol, &lPosition );

   if ( zstrlen( pfr->lpstrFindWhat ) == 0 )
      return( 0 );

   // store search string for F3 / Shift - F3 use
   g_strFindWhat = pfr->lpstrFindWhat;
   lCharCount = g_strFindWhat.GetLength( );
   CString strStringToFind = g_strFindWhat;

   // Check if currently selected text already matches search text. If so,
   // set cursor to the end of the selected text but only in FR_DOWN mode.
   if ( pfr->Flags & FR_WHOLEWORD )
   {
      lSearchBehavior = SRCH_REGEXPR;
      strStringToFind = "\\b";
      CString strUpper = g_strFindWhat;
      CString strLower = g_strFindWhat;
      strUpper.MakeUpper( );
      strLower.MakeLower( );
      for( int k = 0; k < strUpper.GetLength( ); k++ )
      {
         strStringToFind += "[";
         strStringToFind += strUpper[ k ];
         strStringToFind += strLower[ k ];
         strStringToFind += "]";
      }

      strStringToFind += "[^a-zA-Z_0-9]";
      if ( oEditor->CanCopy( ) )
      {
         CString strHelp;
         LPSTR pszBuffer = strHelp.GetBufferSetLength( lCharCount );
         oEditor->GetTextFromRange( pszBuffer, lPosition, lPosition + lCharCount );
         strHelp.ReleaseBuffer( );
         if ( strHelp.CompareNoCase( g_strFindWhat ) == 0 && pfr->Flags & FR_DOWN )
         {
            lPosition += lCharCount;
            oEditor->SetCursorPositionByIndex( lPosition );
         }
      }
   }

   if ( pfr->Flags & FR_MATCHCASE )
   {
      lSearchBehavior |= SRCH_MATCHCASE;
      g_bMatchCase = TRUE;
   // strStringToFind = "\\b";
      strStringToFind = g_strFindWhat;
   // strStringToFind += "[^a-zA-Z_0-9]";
   }

   if ( pfr->Flags & FR_DOWN )
      lSearchBehavior |= SRCH_FORWARD;

   oEditor->FindTextPosition( strStringToFind, &lPosition, lSearchBehavior );
   if ( lPosition > -1 )
   {
      oEditor->SelectRange( lPosition, lCharCount);
   }
   else
   {
      strMsg.Format( strMsgFormat, pfr->lpstrFindWhat );
      MB_SetMessage( vSubtask, MAIN_DIL, strMsg );
   }

   return( 0 );

} // TZEDFRMD_OnEditFind

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_OnEditFindNext
//
//    handles the EditFindNext event
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDFRMD_OnEditFindNext( zVIEW vSubtask )
{
   TZEDFRMD_OnEditFind( vSubtask );
   return( 0 );

} // TZEDFRMD_OnEditFindNext

static zBOOL
GetOperationPosition( zVIEW    vSubtask,
                      ZDrTBEdt *oEditor,
                      zVIEW    vDeleteOp,
                      zPLONG   plOpStartIndex,
                      zPLONG   plOpEndIndex )
{
   zCHAR      szOperName[ 33 ];
   zBOOL      bReturn = FALSE;

   *plOpStartIndex  = -1;
   *plOpEndIndex    = -1;

   GetStringFromAttribute( szOperName, sizeof( szOperName ), vDeleteOp, "Operation", "Name" );

   // 1. find begin and end for the requestet operation
   *plOpStartIndex = FindBeginOfOperation( vSubtask, szOperName, oEditor );

   if ( *plOpStartIndex >= 0 )
      *plOpEndIndex = FindEndOfOperation( vSubtask, szOperName, oEditor );

   // 2. delete code within the range of plOpStartIndex and plOpEndIndex
   if ( *plOpStartIndex >= 0 && *plOpEndIndex >= 0 && *plOpStartIndex < *plOpEndIndex )
      bReturn = TRUE;

   return( bReturn );
}

static zSHORT
GetPositionForNextInsert( ZDrTBEdt *oEditor,
                          zPLONG   plPosIndex )
{
   zLONG lLine  = -1;
   zLONG lCol   = -1;
   zLONG lIndex = -1;

   // set cursor to the last line
   oEditor->GetLineCount( &lLine );
   oEditor->SetCursorPositionByLine( lLine - 1, 0 );

   // in c-files search for #ifdef __cplusplus (from end to begin)
   static zCHAR szSearchString[] = "\\bifdef[^!-~°§²³´ßäöüÄÖÜ]+__cplusplus[^!-~°§²³´ßäöüÄÖÜ]+";

   oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );
   oEditor->FindTextPosition( szSearchString, &lIndex, SRCH_BACKWARD | SRCH_REGEXPR );
   lIndex--;

   if ( lIndex > -1 )
   {
      zLONG  lIndex_1;
      zLONG  lIndex_2;

      //set cursor to #ifdef __cplusplus
      oEditor->SetCursorPositionByIndex( lIndex );

      // search for } ( from #ifdef __cplusplus to end )
      static zCHAR szSearchString_1[] = "} *";
      oEditor->GetCursorPosition( &lLine, &lCol, &lIndex_1 );
      oEditor->FindTextPosition( szSearchString_1, &lIndex_1, SRCH_FORWARD |
                                 SRCH_REGEXPR );

      if ( lIndex_1 >= 0 && lIndex_1 > lIndex )
      {
         //set cursor to #ifdef __cplusplus
         oEditor->SetCursorPositionByIndex( lIndex );

         // search for #endif (from #ifdef __cplusplus to end)
         static zCHAR szSearchString_2[] = "\\bendif[^!-~°§²³´ßäöüÄÖÜ]*";

         oEditor->GetCursorPosition( &lLine, &lCol, &lIndex_2 );
         oEditor->FindTextPosition( szSearchString_2, &lIndex_2, SRCH_FORWARD |
                                    SRCH_REGEXPR );
         if ( lIndex_2 < 0 || lIndex_2 <= lIndex_1 )
            lIndex =  1048576; // end of file
      }
      else
      {
         lIndex =  1048576; // end of file
      }
   }
   else
   {
      lIndex =  1048576; // end of file
   }

   *plPosIndex = lIndex;
   return( 0 );
}

static zLONG
CopyOperationToNewFile( ZDrTBEdt *oEditor,
                        zVIEW    vEdWrk,
                        zPCHAR   pszBuffer )
{
   zLONG    lReturnedBuffSize = -1;
   zLONG    lLine, lCol, lIndex = -1;
   CString  strBuffer;
   zPCHAR   szBuffer = NULL;

   // if source file is of type "C"
   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlC_File ) == 0 )
   {
      // in c-files search for #ifdef __cplusplus (from end to begin)
      GetPositionForNextInsert( oEditor, &lIndex );
      oEditor->SetCursorPositionByIndex( lIndex );
   }
   else
   {
      // set cursor to the last line
      oEditor->GetLineCount( &lLine );
      oEditor->SetCursorPositionByLine( lLine - 1, 0 );
      oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );
      szBuffer = strBuffer.GetBufferSetLength( 64004 );   // 255
      lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer,
                                                           64000, lIndex );
      oEditor->SetCursorPositionByLine( lLine, lReturnedBuffSize );
   }

   oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );

   // insert Operation
   BufInsertStr( oEditor, "\r\n\r\n" );
   BufInsertStr( oEditor, pszBuffer );

   oEditor->SaveObject( );
   return( lIndex );
}

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_MoveOperationCode( zVIEW  vSubtask,
                            zSHORT nCopyOrMoveFlag )
{
   zPCHAR   pszBuffer = NULL;
   CString  strBuffer;
   zCHAR    szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR    szSourceFileName[ zMAX_FILENAME_LTH + 1 ];
   zVIEW    vEdWrk  = 0;
   zVIEW    vSource = 0;
   zVIEW    vProfileXFER  = 0;
   zVIEW    vDeleteOp     = 0;
   zLONG    lOpStartIndex = -1;
   zLONG    lOpEndIndex   = -1;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( InitEditorForMoveAndDelete( oEditor, vSubtask, &vDeleteOp, &vSource,
                                    &vEdWrk, &vProfileXFER, szFileName, sizeof( szFileName ) ) < 0 )
   {
      return( -1 );
   }

   // copy Operation Code from 1. Source File
   if ( SetCursorFirstEntity( vDeleteOp, "SourceFile", "" ) < zCURSOR_SET ||
        OpenFileForMoveAndDelete( oEditor, vDeleteOp, vEdWrk, vSource, vProfileXFER,
                                  szFileName, szSourceFileName, FALSE ) < 0 )
   {
      DropViewsForMoveAndDelete( vSubtask, vDeleteOp, vEdWrk );
      return( -1 );
   }

   if ( SetCursorFirstEntity( vDeleteOp, "Operation", "" ) >= zCURSOR_SET )
   {
      if ( GetOperationPosition( vSubtask, oEditor, vDeleteOp, &lOpStartIndex, &lOpEndIndex ) == TRUE )
      {
         // get Operation Code
         pszBuffer = strBuffer.GetBufferSetLength( lOpEndIndex - lOpStartIndex + 1 );
         oEditor->GetTextFromRange( pszBuffer, lOpStartIndex, lOpEndIndex );

         if ( *pszBuffer )
         {
            // paste Operation Code to 2. Source File
            if ( SetCursorNextEntity( vDeleteOp, "SourceFile", "" ) < zCURSOR_SET ||
                 OpenFileForMoveAndDelete( oEditor, vDeleteOp, vEdWrk, vSource,
                                           vProfileXFER, szFileName,
                                           szSourceFileName, TRUE ) < 0 )
            {
               DropViewsForMoveAndDelete( vSubtask, vDeleteOp, vEdWrk );
               return( -1 );
            }

            // set cursor to last line and insert operation
            CopyOperationToNewFile( oEditor, vEdWrk, pszBuffer );
            DeleteEntity( vEdWrk, "Buffer", zREPOS_NEXT );

            if ( nCopyOrMoveFlag == zMOVE_OPERATION )
            {
               // Drop 2. Source File from Delete LOD (do not delete in this file)
               DeleteEntity( vDeleteOp, "SourceFile", zREPOS_PREV );

               // Drop Operation Code from 1. Source File
               DeleteOperationFromCurrentFile( vSubtask, vSource, vDeleteOp, vEdWrk,
                                               vProfileXFER, oEditor, szFileName );
            }
         } // endif ( *pszBuffer )
      } // endif ( GetOperationPosition( vSubtask, oEditor, vDeleteOp, ...
   } // endif ( SetCursorFirstEntity( vDeleteOp, "Operation", "" ) >= zCURSOR_SET )

   DeleteEntity( vEdWrk, "Buffer", zREPOS_NEXT );
   DropViewsForMoveAndDelete( vSubtask, vDeleteOp, vEdWrk );
   return( 0 );
}

static zSHORT
RenameOperation( ZDrTBEdt *oEditor,
                 zVIEW    vSubtask,
                 zLONG    lPosition )
{
   zVIEW   vDeleteOp;
   zCHAR   szSearchText[ 256 ];
   zCHAR   szReplaceText[ 33 ];
   zCHAR   szOperation[ 33 ];
   zLONG   lIndex = lPosition;

   GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntity( vDeleteOp, "SourceFile", "" );
   SetCursorFirstEntity( vDeleteOp, "Operation", "" );
   GetStringFromAttribute( szOperation, sizeof( szOperation ), vDeleteOp, "Operation", "Name" );

   SetCursorNextEntity( vDeleteOp, "SourceFile", "" );
   if ( SetCursorFirstEntity( vDeleteOp, "Operation", "" ) < zCURSOR_SET )
      return( 0 );

   GetStringFromAttribute( szReplaceText, sizeof( szReplaceText ), vDeleteOp, "Operation", "Name" );

   // replace Operation name in comments
   zsprintf( szSearchText, "\\b%s\\W", szOperation );
   oEditor->SetCursorPositionByIndex( lPosition );
   oEditor->FindTextPosition( szSearchText, &lPosition, SRCH_FORWARD | SRCH_REGEXPR );
   while ( lPosition > -1 )
   {
      if ( fnCursorInCommentOrQuote( oEditor, lPosition ) == 1 )
      {
         oEditor->SelectRange( lPosition, zstrlen( szOperation ) );
         if ( oEditor->CanCopy( ) ) // is something selected?
            oEditor->InsertItem( szReplaceText );
      }
      lPosition++;
      oEditor->FindTextPosition( szSearchText, &lPosition, SRCH_FORWARD | SRCH_REGEXPR );
   }

   // replace Operation name
   zsprintf( szSearchText, "\\b%s[^!-~°§²³´ßäöüÄÖÜ]*(", szOperation );
   oEditor->SetCursorPositionByIndex( lIndex );
   oEditor->FindTextPosition( szSearchText, &lIndex, SRCH_FORWARD | SRCH_REGEXPR );
   while ( lIndex > -1 )
   {
      oEditor->SelectRange( lIndex, zstrlen( szOperation ) );
      if ( oEditor->CanCopy( ) ) // is something selected?
         oEditor->InsertItem( szReplaceText );

      oEditor->FindTextPosition( szSearchText, &lIndex,
                                 SRCH_FORWARD | SRCH_REGEXPR );
   }

   oEditor->SaveObject( );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_CopyOperationCode
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_CopyOperationCode( zVIEW vSubtask,
                            zSHORT nRenameOperation )
{
   zPCHAR   pszBuffer = NULL;
   CString  strBuffer;
   zCHAR    szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR    szSourceFileName[ zMAX_FILENAME_LTH + 1 ];
   zVIEW    vEdWrk  = 0;
   zVIEW    vSource = 0;
   zVIEW    vProfileXFER  = 0;
   zVIEW    vDeleteOp     = 0;
   zLONG    lOpStartIndex = -1;
   zLONG    lOpEndIndex   = -1;
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( InitEditorForMoveAndDelete( oEditor, vSubtask, &vDeleteOp, &vSource,
                                    &vEdWrk, &vProfileXFER, szFileName, sizeof( szFileName ) ) < 0 )
   {
      return( -1 );
   }

   // copy Operation Code from 1. Source File
   if ( SetCursorFirstEntity( vDeleteOp, "SourceFile", "" ) < zCURSOR_SET ||
        OpenFileForMoveAndDelete( oEditor, vDeleteOp, vEdWrk, vSource, vProfileXFER,
                                  szFileName, szSourceFileName, FALSE ) < 0 )
   {
      DropViewsForMoveAndDelete( vSubtask, vDeleteOp, vEdWrk );
      return( -1 );
   }

   if ( SetCursorFirstEntity( vDeleteOp, "Operation", "" ) >= zCURSOR_SET )
   {
      if ( GetOperationPosition( vSubtask, oEditor, vDeleteOp, &lOpStartIndex,
                                 &lOpEndIndex ) == TRUE )
      {
         // get Operation Code
         pszBuffer = strBuffer.GetBufferSetLength( lOpEndIndex - lOpStartIndex + 1 );
         oEditor->GetTextFromRange( pszBuffer, lOpStartIndex, lOpEndIndex );

         if ( *pszBuffer )
         {
            // paste Operation Code to 2. Source File
            if ( SetCursorNextEntity( vDeleteOp, "SourceFile", "" ) < zCURSOR_SET ||
                 OpenFileForMoveAndDelete( oEditor, vDeleteOp, vEdWrk, vSource,
                                           vProfileXFER, szFileName,
                                           szSourceFileName, TRUE ) < 0 )
            {
               DropViewsForMoveAndDelete( vSubtask, vDeleteOp, vEdWrk );
               return( -1 );
            }

            // set cursor to last line and insert operation
            lOpStartIndex = CopyOperationToNewFile( oEditor, vEdWrk, pszBuffer );

            // Rename Operation
            if ( nRenameOperation == zRENAME_OPERATION )
               RenameOperation( oEditor, vSubtask, lOpStartIndex );

            DeleteEntity( vEdWrk, "Buffer", zREPOS_NEXT );

         } // endif ( *pszBuffer )
      } // endif ( GetOperationPosition( vSubtask, oEditor, vDeleteOp, ...
   } // endif ( SetCursorFirstEntity( vDeleteOp, "Operation", "" ) >= zCURSOR_SET )

   DeleteEntity( vEdWrk, "Buffer", zREPOS_NEXT );
   DropViewsForMoveAndDelete( vSubtask, vDeleteOp, vEdWrk );
   return( 0 );

} // TZEDFRMD_CopyOperationCode

static zSHORT
InitEditorForMoveAndDelete( ZDrTBEdt *oEditor,
                            zVIEW    vSubtask,
                            zPVIEW   pvDeleteOp,
                            zPVIEW   pvSource,
                            zPVIEW   pvEdWrk,
                            zPVIEW   pvProfileXFER,
                            zPCHAR   pchFileName,
                            zLONG    lMaxLth )
{
   zVIEW      vTaskLPLR;
   zCHAR      szInvokingTool[ 33 ];
   zCHAR      szSourceFileEntityName[ 33 ];

   // registry problem. Don't know if this is the best way to do that but it's better than an access-violation
   if ( oEditor == 0 )
      return( -1 );

   if ( GetViewByName( pvDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) < 0 ||
        GetViewByName( pvSource, "METAOPERATIONS", vSubtask, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   mGetWorkView( pvEdWrk, vSubtask );
   mGetProfileView( pvProfileXFER );

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szInvokingTool, sizeof( szInvokingTool ), *pvProfileXFER, szlED, "InvokingTool" );

   // Clean up a little by dropping the name.
   DropNameForView( *pvSource, "METAOPERATIONS", vSubtask, zLEVEL_TASK );

   // Create the name of the file.
   if ( zstrcmp( szInvokingTool, "GO" ) == 0 )
      strcpy_s( szSourceFileEntityName, sizeof( szSourceFileEntityName ), "GlobalOperationGroup" );
   else
      if ( zstrcmp( szInvokingTool, "DM" ) == 0 )
         strcpy_s( szSourceFileEntityName, sizeof( szSourceFileEntityName ), "DomainGroup" );
      else
         strcpy_s( szSourceFileEntityName, sizeof( szSourceFileEntityName ), szlSourceFile );

   // Get the source dir from the lplr view.
   GetStringFromAttribute( pchFileName, lMaxLth, vTaskLPLR, szlLPLR, szlPgmSrcDir );
   SysAppendcDirSep( pchFileName );

   return( 0 );
}

static zSHORT
OpenFileForMoveAndDelete( ZDrTBEdt *oEditor,
                          zVIEW    vDeleteOp,
                          zVIEW    vEdWrk,
                          zVIEW    vSource,
                          zVIEW    vProfileXFER,
                          zPCHAR   pchFileName,
                          zPCHAR   szSourceFileName,
                          zBOOL    bCreateFile )
{
   zCHAR  szName[33];
   zLONG  hFile;
   zBOOL  bCreateFileInfo = FALSE;

   strcpy_s( szSourceFileName, sizeof( szSourceFileName ), pchFileName );

   // Get the subdir from the source file entity.  If GetString... returns < 0
   // then the subdir attribute is null, so nothing was retrieved.  If subdir
   // is set, then we need to add a '\'.
   if ( GetStringFromAttribute( szSourceFileName + zstrlen( szSourceFileName ), sizeof( szSourceFileName ) - zstrlen( szSourceFileName ),
                                vDeleteOp, "SourceFile", "Subdirectory" ) >= 0 )
   {
      SysAppendcDirSep( szSourceFileName );
   }

   // Get the file name and extension.
   GetStringFromAttribute( szSourceFileName + zstrlen( szSourceFileName ), sizeof( szSourceFileName ) - zstrlen( szSourceFileName ),
                           vDeleteOp, "SourceFile", "Name" );
   strcat_s( szSourceFileName, sizeof( szSourceFileName ), "." );
   GetStringFromAttribute( szSourceFileName + zstrlen( szSourceFileName ), sizeof( szSourceFileName ) - zstrlen( szSourceFileName ),
                           vDeleteOp, "SourceFile", "Extension" );

   if ( SysOpenFile( vDeleteOp, szSourceFileName, COREFILE_EXIST ) == -1 )
   {
      if ( !bCreateFile )
         return( -1 );
      else
      {
         // Create an empty file.
         hFile = SysOpenFile( vDeleteOp, szSourceFileName, COREFILE_CREATE );
         SysCloseFile( vDeleteOp, hFile, 0 );
         bCreateFileInfo = TRUE;
      }
   } //endif ( SysOpenFile( vDeleteOp, szSourceFileName, COREFILE_EXIST ) == -1 )

   oEditor->OpenObject( szSourceFileName );

   CreateEntity( vEdWrk, szlBuffer, zPOS_LAST );
   SetAttributeFromString( vEdWrk, szlBuffer, szlFileName, szSourceFileName );
   SetMatchingAttributesByName( vEdWrk, szlBuffer, vProfileXFER, "ED", zSET_ALL );
   SetAttributeFromInteger( vEdWrk, szlBuffer, szlSourceViewID, (zLONG) vSource );

   // Set the LanguageType for the current buffer.
   SetAttributeFromAttribute( vEdWrk, szlBuffer, szlLanguageType,
                              vDeleteOp, "SourceFile", "LanguageType" );

   // if a new C File ?
   if ( bCreateFileInfo &&
        CompareAttributeToString( vDeleteOp, "SourceFile", "LanguageType",
                                  szlC_File ) == 0 )
   {
      GetStringFromAttribute( szName, sizeof( szName ), vDeleteOp, "SourceFile", "Name" );
      CreateFileInfo( oEditor, szName );
   }

   return( 0 );
}

static zSHORT
DropViewsForMoveAndDelete( zVIEW vSubtask,
                           zVIEW vDeleteOp,
                           zVIEW vEdWrk )
{
   DropView( vDeleteOp );
   DropView( vEdWrk );

   GetViewByName( &vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_TASK );

   if ( vEdWrk )
      DropView( vEdWrk );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_DisableActions
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDFRMD_DisableActions( zVIEW vSubtask )
{
   zBOOL  bReadOnly = TRUE;
   zVIEW  vEdWrk;
   zVIEW  vWindow;

   GetParentWindow( &vWindow, vSubtask );
   if ( vWindow )
   {
      mGetWorkView( &vEdWrk, vWindow );

      if ( SetCursorFirstSelectedEntity( vEdWrk, "Oper", 0 ) < zCURSOR_SET ||
           CompareAttributeToString( vEdWrk, szlBuffer,
                                     szlReadOnly, "Y" ) == 0 ||
           CompareAttributeToString( vEdWrk, szlBuffer,
                                     szlActiveStatus, "N" ) == 0 )
      {
         bReadOnly = FALSE;
      }
   }

   EnableAction( vSubtask, "InsertOperation", bReadOnly );

   return( 0 );

} // TZEDFRMD_DisableActions

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_CreateContextMenue
//
//    Creates a Context menu for Editor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDFRMD_CreateContextMenue( zVIEW vSubtask )
{
// POINT  pt;
   CPoint pt( GetMessagePos( ) );
// pt.x = pt.y = -1;
   TZEDFRMD_EnableEditMenuActions( vSubtask );

   CreateTrackingPopupMenu( vSubtask, TZEDFRMD_EDIT_CONTEXT_MENU_TAG,
                            pt.x, pt.y, FALSE, FALSE );

   return( 0 );

} // TZEDFRMD_CreateContextMenue

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDFRMD_EnableEditMenuActions
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDFRMD_EnableEditMenuActions( zVIEW vSubtask )
{
   ZDrTBEdt *oEditor =
      (ZDrTBEdt *) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );

   if ( oEditor == 0 )
      return( 0 );

   BOOL bEnable = TRUE;
   BOOL bCanCopy  = oEditor->CanCopy( );
   BOOL bCanPaste = oEditor->CanPaste( );
   BOOL bCanUndo  = oEditor->CanUndo( );
   BOOL bCanRedo  = oEditor->CanRedo( );

   if ( oEditor->IsReadOnly( ) )
      bEnable = FALSE;

   EnableAction( vSubtask, TZEDFRMD_EDIT_CONTEXT_MENU_UNDO,
                 bEnable ? bCanUndo : FALSE );
   EnableAction( vSubtask, TZEDFRMD_EDIT_CONTEXT_MENU_REDO,
                 bEnable ? bCanRedo : FALSE );
   EnableAction( vSubtask, TZEDFRMD_EDIT_CONTEXT_MENU_CUT,
                 bEnable ? bCanCopy : FALSE );
   EnableAction( vSubtask, TZEDFRMD_EDIT_CONTEXT_MENU_COPY,
                 bCanCopy );
   EnableAction( vSubtask, TZEDFRMD_EDIT_CONTEXT_MENU_PASTE,
                 bEnable ? bCanPaste : FALSE );
   EnableAction( vSubtask, TZEDFRMD_EDIT_CONTEXT_MENU_DELETE,
                 bEnable ? bCanCopy : FALSE );
   EnableAction( vSubtask, TZEDFRMD_EDIT_CONTEXT_MENU_SELECTALL,
                 !FileIsEmpty( oEditor ) );

   return( 0 );

} // TZEDFRMD_EnableEditMenuActions

static zSHORT
SetUpdateFlagInMeta( zVIEW vSubtask )
{
   zVIEW        vEdWrk;
   zVIEW        vSource;
   zVIEW        vMeta;
   zVIEW        vTaskLPLR;
   zVIEW        vZeidonCM;
   zVIEW        vActiveMetas;
   zLONG        lMetaType;
   zLONG        lMetaZKey;
   zSHORT       nRC = zCALL_ERROR;
   zCHAR        szTimestamp[ 33 ];
   zCHAR        szUpdateInd[ 5 ];
   LPVIEWOD     lpViewOD;
   LPVIEWENTITY lpViewEntity;

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
   {
      return( nRC );
   }

   // Get the Source Meta for the current buffer.
   GetIntegerFromAttribute( (zPLONG) &vSource, vEdWrk, szlBuffer,
                            szlSourceViewID );

   if ( vSource == 0 || MiGetUpdateForView( vSource ) == 0 )
   {
      return( nRC );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlMetaType,
                                   "" ) == 0 )
   {
      return( 0 );
   }

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 ||
        GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask, zLEVEL_APPLICATION ) < 0 ||
        GetViewByName( &vActiveMetas, "OpenCM_Metas", vZeidonCM, zLEVEL_SUBTASK ) < 0 )
   {
      return( nRC );
   }

   GetIntegerFromAttribute( &lMetaType, vEdWrk, szlBuffer, szlMetaType );
   if ( lMetaType >= 2000 )
      lMetaType -= 2000;

   if ( SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaType", "Type",
                                       lMetaType, "" ) < zCURSOR_SET )
   {
      return( nRC );
   }

   CreateViewFromViewForTask( &vMeta, vSource, 0 );
   ResetView( vMeta );

   // Get Root Entity Name
   lpViewOD = (LPVIEWOD) MiGetViewEntityForView( vMeta, 0 );
   if ( lpViewOD == 0 )
      return( nRC );

   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );

   GetIntegerFromAttribute( &lMetaZKey, vMeta, lpViewEntity->szName, "ZKey" );

   if ( SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaDef", "CPLR_ZKey",
                                       lMetaZKey, "" ) >= zCURSOR_SET &&
        CompareAttributeToInteger( vActiveMetas, "W_MetaDef", "Status",
                                   1 ) == 0 )
   {
      GetStringFromAttribute( szUpdateInd, sizeof( szUpdateInd ), vActiveMetas, "W_MetaDef", "UpdateInd" );

      if ( CompareAttributeToInteger( vActiveMetas, "W_MetaDef",
                                      "UpdateInd", 0 ) == 0 ||
           zstrcmp( szUpdateInd, "" ) == 0 )
      {
         // Set Update Indicator to update = Yes ( Implicit, SourceFile )
         SetAttributeFromInteger( vActiveMetas, "W_MetaDef", "UpdateInd", 4 );
         nRC = 1;
      }

      if ( CompareAttributeToInteger( vActiveMetas, "W_MetaDef",
                                      "UpdateInd", 4 ) == 0 )
      {
         SysGetDateTime( szTimestamp, sizeof( szTimestamp ) );
         SetAttributeFromString( vActiveMetas, "W_MetaDef",
                                 "LastUpdateDate", szTimestamp );
         SetAttributeFromString( vActiveMetas, "W_MetaDef",
                                 "LastSyncDate", szTimestamp );
      }
   }

   DropView( vMeta );
   return( nRC );
}


#ifdef __cplusplus
}
#endif
