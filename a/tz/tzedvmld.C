//
/////////////////////////////////////////////////////////////////////////////
//
// .Name:         tzedvmld.c
//
// .Version:      1.0
//
// .Last change:  13-03-2002
//
// .Description:  Zeidon operations
//
/////////////////////////////////////////////////////////////////////////////
// .Change notes
//
// 2002.08.29  DGC
//    Added an argument to ParseSource().
//
// 2002.08.15  RR
//    Replace Name of Zeidon Online Manuals by DEFINES
//    Remove Prefix "fn " and "vml " of  C-Function name and VML-Operation
//    name in function VML_StartHelp and OpIns_StartOperationHelp
//
//  1.0  = New
//        (13-03-2002) Zeidon
//
/////////////////////////////////////////////////////////////////////////////
//

#include <zstdafx.h>
#define zGLOBAL_DATA

#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"
#include "ax\tzvmledt\tzvmledt.h"
#include "ax\tzvmledt\tzvmlcst.h"  // control specific constants
#include "ax\tzvmledt\zstring.h"   // string class for tokenizing

#define KZSYSSVC_INCL
#include <KZOENGAA.H>
#include <ZDRVROPR.H>

#include "tz__oprs.h"
#include "tzlodopr.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "ZEIDONOP.H"

#define mGetProfileView( pv, v )   GetViewByName( (pv), szlProfileXFER, v, zLEVEL_TASK )
#define mGetWorkView( pvView, v )  GetViewByName( (pvView), szlTZEDWRKO, v, zLEVEL_SUBTASK )
#define MAIN_DIL             1
#define FILECHANGED_DIL      2
#define LINECOL_DIL          3
#define INSERTMODE_DIL       4
#define EDIT_CONTROL_NAME    "_ZeidonEditor"
#define MAX_TOKEN_LTH        100
#define BUFFER_SIZE          512

#define SRCH_BACKWARD        0x00000000
#define SRCH_FORWARD         0x00000001
#define SRCH_MATCHCASE       0x00000002
#define SRCH_REGEXPR         0x00000004

#define DISP_SELECT_SET    1
#define SEL_SELECT_SET     2


#define TZEDVMLD_EDIT_CONTEXT_MENU_TAG       "EditContext"
#define TZEDVMLD_EDIT_CONTEXT_MENU_UNDO      "EditContextUndo"
#define TZEDVMLD_EDIT_CONTEXT_MENU_REDO      "EditContextRedo"
#define TZEDVMLD_EDIT_CONTEXT_MENU_CUT       "EditContextCut"
#define TZEDVMLD_EDIT_CONTEXT_MENU_COPY      "EditContextCopy"
#define TZEDVMLD_EDIT_CONTEXT_MENU_PASTE     "EditContextPaste"
#define TZEDVMLD_EDIT_CONTEXT_MENU_DELETE    "EditContextDelete"
#define TZEDVMLD_EDIT_CONTEXT_MENU_SELECTALL "EditContextSelectAll"

#define TZEDVMLD_ACTION_EDIT_UNDO      "EditUndo"
#define TZEDVMLD_ACTION_EDIT_REDO      "EditRedo"
#define TZEDVMLD_ACTION_EDIT_CUT       "EditCut"
#define TZEDVMLD_ACTION_EDIT_COPY      "EditCopy"
#define TZEDVMLD_ACTION_EDIT_PASTE     "EditPaste"
#define TZEDVMLD_ACTION_EDIT_DELETE    "EditDelete"
#define TZEDVMLD_ACTION_EDIT_SELECTALL "EditSelectAll"

// Used when pasting V.E.A
#define PASTE_VIEW_VARIABLE  0x0001
#define PASTE_ENTITY         0x0002
#define PASTE_ATTRIBUTE      0x0004
#define PASTE_WITH_QUOTE     TRUE
#define PASTE_WITHOUT_QUOTE  FALSE

#define FILE_NO_PARSE        0
#define PARSE_FILE           1


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
   "VIEW var_name BASED ON LOD lod_name", "D",
   "VIEW & BASED ON LOD ",
   "VIEW &var_name BASED ON LOD lod_name",
   "BOL", "Y",

   "VIEW var_name REGISTERED AS", "D",
   "VIEW & REGISTERED AS ",
   "VIEW &var_name REGISTERED AS regv_name",
   "VIEW", "Y",

   "INTEGER var_name", "D",
   "INTEGER &",
   "INTEGER &var_name",
   "INTEGER", "",

   "SHORT var_name", "D",
   "SHORT &",
   "SHORT &var_name",
   "SHORT", "",

   "STRING ( lth ) var_name", "D",
   "STRING ( & ) ",
   "STRING ( &lth ) var_name",
   "STRING", "Y",

   "DECIMAL var_name", "D",
   "DECIMAL &",
   "DECIMAL &var_name",
   "DECIMAL", "",

   "CHARACTER var_name", "D",
   "CHARACTER &",
   "CHARACTER &var_name",
   "CHARACTER", "",

   "VIEW var_name", "D",
   "VIEW &",
   "VIEW &var_name",
   "VW", "",

   "ENTITYNAME var_name", "D",
   "ENTITYNAME &",
   "ENTITYNAME &var_name",
   "ENT", "",

   "ATTRIBUTENAME var_name", "D",
   "ATTRIBUTENAME &",
   "ATTRIBUTENAME &var_name",
   "ATTR", "",

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
   "COMMIT", "",

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
   "    [ WITHIN scoping-view.entity | OBJECT ]\r\n"
   "    [ WHERE condition ]\r\n"
   "END",
   "FOR", "Y",

   "GET VIEW view NAMED view_name", "V",
   "GET VIEW & NAMED \"\"",
   "GET VIEW &view NAMED \"view_name\"\r\n"
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

   "NAME VIEW view view_name", "V",
   "NAME VIEW & \"\"",
   "NAME VIEW &view \"view_name\"\r\n"
   "   [ AT LEVEL { TASK | SUBTASK | APPLICATION | SYSTEM | ANY } ] [ USING Subtask ]",
   "NAME", "Y",

   "SET CURSOR view.entity", "E",
   "SET CURSOR &",
   "SET CURSOR &[ FIRST | LAST | NEXT | PREVIOUS ] view.entity\r\n"
   "    [ WITHIN scoping-view.entity | OBJECT ]\r\n"
   "    [ WHERE condition ]",
   "SET", "Y",

   "SET CURSOR FIRST view.entity WHERE", "E",
   "SET CURSOR FIRST &\r\n    WHERE ",
   "SET CURSOR FIRST &view.entity\r\n"
   "    [ WITHIN scoping-view.entity | OBJECT ]\r\n"
   "    [ WHERE condition ]",
   "SETF", "Y",

   "SET CURSOR NEXT view.entity WHERE", "E",
   "SET CURSOR NEXT &\r\n    WHERE ",
   "SET CURSOR NEXT &view.entity\r\n"
   "    [ WITHIN scoping-view.entity | OBJECT ]\r\n"
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

#define USE_EDITOR  LPVMLWRAPPER  oEditor = \
                      (LPVMLWRAPPER ) GetActiveX_WrapperInstance( vSubtask, EDIT_CONTROL_NAME );\
                         if ( oEditor == 0 )\
                         {\
                            TraceLineS( "TZEDVMLD ", "No editor control found returning 0");\
                            return( 0 );\
                         }

#define MovEOF()      oEditor->SetCursorPositionByLine( oEditor->GetLineCount() + 1, 0)
#define MovUp( n )    oEditor->GetCursorPosition( &lMoveBaseLine, &lMoveBaseColumn, &lMoveBaseIndex); \
                                                  oEditor->SetCursorPositionByLine(lMoveBaseLine - n, lMoveBaseColumn)


long lMoveBaseLine, lMoveBaseColumn, lMoveBaseIndex;

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
char szlTZEDVMLD[]           = "TZEDVMLD";
char szlTZCMLPLO_List[]      = "TZEDVMLD_TZCMLPLO_List";
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

//global variables
zBOOL   g_bIsFileNew = FALSE; // to handle "File New" menu command
FINDREPLACE g_fr;
CString g_strFindWhat = "";
zBOOL   g_bMatchCase = FALSE;

/////////////////////////////////////////////////////////////////////////////
// Operation declarations
zSHORT
InitSession( zVIEW  vSubtask );

zOPER_EXPORT zSHORT /* DIALOG */  OPERATION
TZEDVMLD_OnEditFind( zVIEW vSubtask );

zLONG FindBeginOfOperation( zVIEW  vSubtask, LPCSTR szOperationName, LPVMLWRAPPER oEditor );

zLONG FindEndOfOperation( zVIEW  vSubtask, LPCSTR szOperationName, LPVMLWRAPPER oEditor );

zBOOL FileIsEmpty ( LPVMLWRAPPER oEditor );

void BufInsertStr( LPVMLWRAPPER oEditor, zCPCHAR psz );

zOPER_EXPORT zSHORT OPERATION
InsertComment( CString &csComment, LPSTR szOperName, LPSTR szOperComment );

int
fnShowSubwindows( zVIEW  vSubtask );

zOPER_EXPORT zSHORT OPERATION
VML_DisplayList( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDVMLD_DisableActions( zVIEW vSubtask );


zSHORT LOCALOPER
fnInsertVML_Text( zVIEW    vSubtask,
                  zVIEW    vEdWrk,
                  zVIEW    vProfileXFER,
                  LPVMLWRAPPER oEditor );

zOPER_EXPORT zSHORT OPERATION
OpIns_BuildOperList( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
TZEDVMLD_AskForSaveWithParse( zVIEW vSubtask,
                              zSHORT nAskForParse );
int
OperTemplate( CString &csTarget, zVIEW vSource, zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
AEQ_SelectView( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
SaveAndParse( zVIEW vSubtask, zCPCHAR cpcGenLang );


zBOOL
GetPositionForNextInsert( LPVMLWRAPPER oEditor );

zSHORT
GotoCurrentOperation( zVIEW    vSubtask,
                      zVIEW    vSource,
                      LPVMLWRAPPER oEditor,
                      zPCHAR   szOperSrch,
                      zPCHAR   szMetaName,
                      zBOOL    bCFile );


zOPER_EXPORT zSHORT OPERATION
GotoWindow( zVIEW vSubtask )
{
   COleVariant v = -1L;
   USE_EDITOR

   oEditor->ICodeSense::ExecuteCmd( cmCmdGotoLine, v );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_InitCommandCompletion( zVIEW vSubtask )
{
   zSHORT nRC = 0;
   zVIEW vProfileXFER;
   zPCHAR pszType;
   zPCHAR pszText;
   zPCHAR pszInsertText;
   zPCHAR pszInsertTextFull;
   zPCHAR pszCC_Code;
   zPCHAR pszCC_FirstChar;
   zVIEW  vEdWrk;

   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );

   oEditor->ResetCommands();

   mGetProfileView( &vProfileXFER, vSubtask );
   for ( nRC = SetCursorFirstEntity( vProfileXFER, "VML_Text", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, "VML_Text", 0 ) )
   {
      GetAddrForAttribute( &pszType,           vProfileXFER, "VML_Text", "Type" );
      GetAddrForAttribute( &pszText,           vProfileXFER, "VML_Text", "Text" );
      GetAddrForAttribute( &pszInsertText,     vProfileXFER, "VML_Text", "InsertText" );
      GetAddrForAttribute( &pszInsertTextFull, vProfileXFER, "VML_Text", "InsertTextFull" );
      GetAddrForAttribute( &pszCC_Code,        vProfileXFER, "VML_Text", "CC_Code" );
      GetAddrForAttribute( &pszCC_FirstChar,   vProfileXFER, "VML_Text", "CC_FirstChar" );

      if ( *pszCC_Code )
         oEditor->AddCommandToComplete( pszText, pszType, pszInsertText, pszInsertTextFull, pszCC_Code,
                                        *pszCC_FirstChar == 'Y' ? TRUE : FALSE );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlVML_InsertFull, "Y" ) == 0 )
   {
      oEditor->SetCommandCompleteStyle( "F" ); //insert full completion text
   }
   else
   {
      oEditor->SetCommandCompleteStyle( "S" ); // insert short completion text
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: TZEDVMLD_EnableEditMenuActions
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDVMLD_EnableEditMenuActions( zVIEW vSubtask )
{
   USE_EDITOR

   if ( !oEditor )
      return( 0 );

   BOOL bEnable = TRUE;
   BOOL bCanCopy  = oEditor->CanCopy();
   BOOL bCanPaste = oEditor->CanPaste();
   BOOL bCanUndo  = oEditor->CanUndo();
   BOOL bCanRedo  = oEditor->CanRedo();

   if ( oEditor->GetReadOnly() )
      bEnable = FALSE;

   EnableAction( vSubtask, TZEDVMLD_EDIT_CONTEXT_MENU_UNDO, bEnable ? bCanUndo : FALSE );
   EnableAction( vSubtask, TZEDVMLD_EDIT_CONTEXT_MENU_REDO, bEnable ? bCanRedo : FALSE );
   EnableAction( vSubtask, TZEDVMLD_EDIT_CONTEXT_MENU_CUT, bEnable ? bCanCopy : FALSE );
   EnableAction( vSubtask, TZEDVMLD_EDIT_CONTEXT_MENU_COPY, bCanCopy );
   EnableAction( vSubtask, TZEDVMLD_EDIT_CONTEXT_MENU_PASTE, bEnable ? bCanPaste : FALSE );
   EnableAction( vSubtask, TZEDVMLD_EDIT_CONTEXT_MENU_DELETE, bEnable ? bCanCopy : FALSE );
   EnableAction( vSubtask, TZEDVMLD_EDIT_CONTEXT_MENU_SELECTALL, !FileIsEmpty( oEditor ) );
   return( 0 );

} // TZEDVMLD_EnableEditMenuActions

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
   GetIntegerFromAttribute( (zPLONG) &vSource, vEdWrk, szlBuffer, szlSourceViewID );

   if ( vSource == 0 || MiGetUpdateForView( vSource ) == 0 )
   {
      return( nRC );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlMetaType, "" ) == 0 )
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

   if ( SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaType", "Type", lMetaType, "" ) < zCURSOR_SET )
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

   if ( SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaDef", "CPLR_ZKey", lMetaZKey, "" ) >= zCURSOR_SET &&
        CompareAttributeToInteger( vActiveMetas, "W_MetaDef", "Status", 1 ) == 0 )
   {
      GetStringFromAttribute( szUpdateInd, zsizeof( szUpdateInd ), vActiveMetas, "W_MetaDef", "UpdateInd" );

      if ( CompareAttributeToInteger( vActiveMetas, "W_MetaDef", "UpdateInd", 0 ) == 0 || zstrcmp( szUpdateInd, "" ) == 0 )
      {
         // Set Update Indicator to update = Yes ( Implicit, SourceFile )
         SetAttributeFromInteger( vActiveMetas, "W_MetaDef", "UpdateInd", 4 );
         nRC = 1;
      }

      if ( CompareAttributeToInteger( vActiveMetas, "W_MetaDef", "UpdateInd", 4 ) == 0 )
      {
         SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
         SetAttributeFromString( vActiveMetas, "W_MetaDef", "LastUpdateDate", szTimestamp );
         SetAttributeFromString( vActiveMetas, "W_MetaDef", "LastSyncDate", szTimestamp );
      }
   }

   DropView( vMeta );
   return( nRC );
}

// Save the current position of the subwindow in the Profile object.
zSHORT
fnSaveSubwindowPosition( zVIEW  vSubtask,
                         zPCHAR pszName )
{
   zSHORT nRC;
   zVIEW  vProfileXFER;
   zLONG  lPosX, lPosY;
   zVIEW  vEdWrk;
   zPCHAR pszWindowActive;

   // If we can't find the editor work view for the parent editor subtask,
   // then we are shutting down the editor (as opposed to just closing the subwindow).

   // Get the work view for the current subtask.
   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
      // We're closing down the editor so set flag so that we the subwindow
      // comes back up when we open the next editor.
      pszWindowActive = "Y";
   else
      pszWindowActive = "N";

   mGetProfileView( &vProfileXFER, vSubtask );
   nRC = SetCursorFirstEntityByString( vProfileXFER, szlSubwindow, szlName, pszName, "" );
   if ( nRC != zCURSOR_SET )
   {
      CreateEntity( vProfileXFER, szlSubwindow, zPOS_LAST );
      SetAttributeFromString( vProfileXFER, szlSubwindow, szlName, pszName );
   }


   GetCtrlPosition( vSubtask, 0, &lPosX, &lPosY, TRUE );
   SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlPosX, lPosX );
   SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlPosY, lPosY );

   GetCtrlSize( vSubtask, 0, &lPosX, &lPosY, TRUE );
   SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlWidth, lPosX );
   SetAttributeFromInteger( vProfileXFER, szlSubwindow, szlHeight, lPosY );

   SetAttributeFromString( vProfileXFER, szlSubwindow, szlSubwindowActive, pszWindowActive );
   return( 0 );
}

// Set the position of a subwindow determined by vSubtask.
// Return TRUE  if window was repositioned,
//        FALSE if window was not.
//
zSHORT
fnSetSubwindowPosition( zVIEW  vSubtask,
                        zPCHAR szName )
{
   zSHORT nRC;
   zVIEW  vProfileXFER;

   mGetProfileView( &vProfileXFER, vSubtask );
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
ActivateBasicsHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

zSHORT LOCALOPER
fnPasteQualifier( zVIEW  vSubtask,
                  zSHORT nPasteCtl,
                  zBOOL  bInsertWithQuote );


zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_ZeidonMenuSelected( zVIEW vSubtask )
{
   zVIEW      vEdWrk;

   mGetWorkView( &vEdWrk, vSubtask );

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) == 0 )
   {
      // We're editing a VML file so enable certain items.
      SetOptionState( vSubtask, "ParseVML", zCONTROL_STATUS_ENABLED, TRUE );
      SetOptionState( vSubtask, "GenerateC",  zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      // We're editing something that is not a VML file so disable items.
      SetOptionState( vSubtask, "ParseVML", zCONTROL_STATUS_ENABLED, FALSE );
      SetOptionState( vSubtask, "GenerateC",  zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: OptionSettings
//
zOPER_EXPORT zSHORT OPERATION
OptionSettings( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor )
   {
      oEditor->PropertyDialog();
   }

   return( 0 );

} // OptionSettings

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_CreateContextMenue
//
//    Creates a Context menu for Editor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDVMLD_CreateContextMenue( zVIEW vSubtask )
{
// POINT  pt;
   CPoint pt( GetMessagePos( ) );
// pt.x = pt.y = -1;

//   TZEDVMLD_EnableEditMenuActions( vSubtask );

   CreateTrackingPopupMenu( vSubtask, TZEDVMLD_EDIT_CONTEXT_MENU_TAG,
                            pt.x, pt.y, FALSE, FALSE );
   return( 0 );

} // TZEDVMLD_CreateContextMenue

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_AskForSave
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDVMLD_AskForSave( zVIEW vSubtask )
{
//   TZEDVMLD_AskForSaveWithParse( vSubtask, FILE_NO_PARSE );

   return( 0 );
} // TZEDVMLD_AskForSave

zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_SetOverstrike( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor->GetOvertype() )
      MB_SetMessage( vSubtask, INSERTMODE_DIL, "Ovr" );
   else
      MB_SetMessage( vSubtask, INSERTMODE_DIL, "Ins" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FileSaveAs( zVIEW vSubtask )
{
   zVIEW      vEdWrk;
   zCHAR      szMsg[ zMAX_FILENAME_LTH + 40 ];
   zCHAR      szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR      szExtension[ 3 ];
   zULONG     ulZFlags = 0;

   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );

   if ( g_bIsFileNew == TRUE )
   {
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );
   }
   else
   {
      GetStringFromAttribute( szExtension, zsizeof( szExtension ), vEdWrk, szlBuffer, szlLanguageType);
      strcpy_s( szFileName, zsizeof( szFileName ), "NONAME." );
      if ( zstrcmp( szExtension, szlC_File) != 0 )
         strcpy_s( szExtension, zsizeof( szExtension ), szlVML_FileExtension );

      strcat_s( szFileName, zsizeof( szFileName ), szExtension );
   }

   ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT | zOFN_SAVEAS;

   if ( OperatorPromptForFile( vSubtask, szFileName, zMAX_FILENAME_LTH + 1,
                               "VML files|*.VML|Source Files|*.VML;*.C|"
                               "All files|*.*||", 0, ulZFlags) != 1 )
   {
      return( 0 );
   }

   oEditor->SaveFile( szFileName, FALSE );
   {
      g_bIsFileNew = FALSE;
      sprintf_s( szMsg, zsizeof( szMsg ), "File saved as '%s'.", szFileName);
      MB_SetMessage( vSubtask, MAIN_DIL, szMsg );
      SetAttributeFromString( vEdWrk, szlBuffer, szlFileName, szFileName);
      SetWindowCaptionTitle( vSubtask, "Zeidon Editor", szFileName);
   }

   return( 0 );
}

// When the editor window gets focus we need to do a few things:
// o  Check to see if the editor work view is available yet.  If it's not then
//    skip the rest of the function.
// o  Lastly, set focus to the editor control.
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_ReceiveFocus( zVIEW vSubtask )
{
   zVIEW      vEdWrk;
   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
      return( 0 );

   if ( oEditor )
      SetFocusToCtrl( vSubtask, EDIT_CONTROL_NAME );
   return( 0 );
}

// The user just typed a space.  Check to see if we should perform command
// completion.
// Returns: TRUE  - Command completion performed.
//          FALSE - Nothing done.
zSHORT LOCALOPER
fnCommandCompletion( zVIEW vSubtask )
{
   zVIEW      vEdWrk;
   zVIEW      vProfileXFER;
   zLONG      lLine, lCol, lIndex;
   zLONG      lLth;
   zCHAR      szBuffer[ 300 ];
   zPCHAR     pszToken;
   zCHAR      szFileName[MAX_PATH + 1];
   CString    strHelp;

   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );

   // If current File is a C-Source then don't do command completion.

   memset( szFileName, 0, MAX_PATH + 1);
   memset( szFileName, ' ', MAX_PATH );
   oEditor->GetObjectName( szFileName );

   CString strExtension = szFileName;
   int iPoint = strExtension.ReverseFind( '.' );
   strHelp = strExtension.Mid( iPoint + 1 );
   if ( strHelp.CompareNoCase( "VML" ) != 0 )
   {
      return( FALSE );
   }

   oEditor->GetCursorPosition( &lLine, &lCol, &lIndex );

   // no cc if we are in the first col
   if ( lCol == 0 )
   {
      return( FALSE );
   }

   // If we're in a comment or quote then don't do command completion.
// if ( fnCursorInCommentOrQuote( oEditor, lIndex ) )
      return( FALSE );

   zmemset( szBuffer, 0, zsizeof( szBuffer ) );
   //lLth = oEditor->GetActualTextLine( szBuffer, zsizeof( szBuffer ) - 1 );

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
         if ( !isspace( *pszToken ) )
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
   mGetProfileView( &vProfileXFER, vSubtask );
   CreateViewFromViewForTask( &vProfileXFER, vProfileXFER, 0 );

   // Now try to find the token in the list of commands.
   if ( SetCursorFirstEntityByString( vProfileXFER, "VML_Text", "CC_Code", pszToken, 0 ) != zCURSOR_SET )
   {
      DropView( vProfileXFER );     // Drop temp copy of vProfileXFER.
      return( FALSE );              // Code not found so exit.
   }

   // Check to see if we only perform CC for this token if it is the
   // first token on the line.
   if ( CompareAttributeToString( vProfileXFER, "VML_Text", "CC_FirstChar", "Y" ) == 0 )
   {
      zPCHAR psz;

      // Scan the current line before the token to see if there is a non-space.
      // If there is then we don't perform CC so exit.
      for ( psz = pszToken - 1; psz >= szBuffer; psz-- )
      {
         if ( !isspace( *psz ) )
         {
            DropView( vProfileXFER );     // Drop temp copy of vProfileXFER.
            return( FALSE );
         }
      }
   }

   // OK...we are about to do command completion so we need to remove the
   // token from the current line.
// oEditor->DeleteTextRange( lIndex - zstrlen( pszToken ), lIndex );
   strHelp = pszToken;
   // oEditor->SelectItem( lLine + 1, strHelp );
// oEditor->SetCursorPositionByLine( lLine, lCol - zstrlen( pszToken ) );

   // Call operation to insert currently selected keyword in profile OI.
//   fnInsertVML_Text( vSubtask, vEdWrk, vProfileXFER, oEditor );

   DropView( vProfileXFER );     // Drop temp copy of vProfileXFER.
   return( TRUE );

} // fnCommandCompletion

zOPER_EXPORT zSHORT OPERATION
zPrint( zVIEW vSubtask )
{
   USE_EDITOR

   oEditor->PrintContents( NULL, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PreBuild( zVIEW vSubtask )
{
   SetWindowState( vSubtask, zWINDOW_STATUS_VISIBLE, FALSE );
   return( 0 );
} // PreBuild

zOPER_EXPORT zSHORT OPERATION
PostBuild( zVIEW vSubtask )
{
   zVIEW   vTaskLPLR;
   zVIEW   vProfileXFER;
   zVIEW   vEdWrk;
// zCHAR   sz[ 300 ];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   mGetProfileView( &vProfileXFER, vSubtask );

   if ( vProfileXFER == 0 )
   {
      oTZ__PRFO_GetViewToProfile( &vProfileXFER, "WD", vSubtask, zCURRENT_OI );
      if ( vProfileXFER )
         SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   }

   // Set a couple of attributes to NULL.  We don't need them anymore and
   // someday we'll want to get rid of them.
   SetAttributeFromString( vProfileXFER, "ED", "SearchFlags", "" );
   SetAttributeFromString( vProfileXFER, "ED", "FontString", "" );

   // First thing - create a editor work object instance.
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

   // if InitSession returns a value of 1
   // the editor has been used for dropping an operation from sourcecode
   // if so all the work is done
   /*
   if ( InitSession( vSubtask ) == 0 )
   {
      SetAttributeFromInteger( vEdWrk, szlBuffer, "vSubtask", (zLONG) vSubtask );
      GetStringFromAttribute( sz, zsizeof( sz ), vEdWrk, szlBuffer, szlFileName );
      // Set checked out state in title
      if ( CompareAttributeToString( vEdWrk, szlBuffer, szlActiveStatus, "N" ) == 0 )
         strcat_s( sz, zsizeof( sz ), " < not checked out >" );
      else
      if ( CompareAttributeToString( vEdWrk, szlBuffer, szlReadOnly, "Y" ) == 0 )
      {
         strcat_s( sz, zsizeof( sz ), " < Read-Only mode >" );
      }

      SetWindowCaptionTitle( vSubtask, "Zeidon Editor", sz );
      //populate list of commands for command completion
      TZEDVMLD_InitCommandCompletion( vSubtask );
      fnShowSubwindows( vSubtask );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, "", "" );
   }
   else
   {  // operation dropped go back to the parent
      DropView( vEdWrk );
      GetViewByName( &vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_TASK );
      if ( vEdWrk )
         DropView( vEdWrk );

      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, "", "" );
   }
   */
   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, "", "" );
   return( 0 );
}

static zSHORT /*LOCAL */
fnTZEDVMLD_SaveFile( zVIEW    vSubtask,
                     LPVMLWRAPPER oEditor,
                     zPCHAR   pchFileName,
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

      oEditor->SaveFile( pchFileName, FALSE );
      g_bIsFileNew = FALSE;
      SetWindowCaptionTitle( vSubtask, "Zeidon Editor", pchFileName);
   }
   else
   {
      zVIEW vEdWrk;

      mGetWorkView( &vEdWrk, vSubtask );
      if ( vEdWrk == 0 )
      {
         MessagePrompt( vEdWrk, "ED0001", szlZeidonEditor,
                        "TZEDWRKO not Available at SaveFile.", 1,
                        0, 0, zICON_EXCLAMATION );
         return( -1 );
      }


      bFileSaved = oEditor->FileSave( );

      // parse only valid for VML-Files
      if ( nAskForParse == PARSE_FILE &&
           CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) == 0 )
      {
         CString strMsg;

         GetStringFromAttribute( pchFileName, zMAX_FILENAME_LTH + 1, vEdWrk, szlBuffer, szlFileName );
         strMsg.Format("Do you want to parse '%s'?",pchFileName);
         zLONG lRC = OperatorPrompt( vSubtask, szlZeidonEditor, strMsg, TRUE,
                                     zBUTTONS_YESNO, zRESPONSE_YES, zICON_EXCLAMATION );

         if ( lRC == zRESPONSE_YES )
         {
            if ( SaveAndParse( vSubtask, "C" ) != 0 )
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

static zBOOL
GetOperationPosition( zVIEW    vSubtask,
                      LPVMLWRAPPER oEditor,
                      zVIEW    vDeleteOp,
                      zPLONG   lOpStartIndex,
                      zPLONG   lOpEndIndex )
{
   zCHAR      szOperName[ 33 ];
   zBOOL      bReturn = FALSE;

   *lOpStartIndex  = -1;
   *lOpEndIndex    = -1;

   GetStringFromAttribute( szOperName, zsizeof( szOperName ), vDeleteOp, "Operation", "Name" );

   // 1. find begin and end for the requestet operation
   *lOpStartIndex = FindBeginOfOperation( vSubtask, szOperName, oEditor );

   if ( *lOpStartIndex >= 0 )
      *lOpEndIndex = FindEndOfOperation( vSubtask, szOperName, oEditor );

   // 2. delete code within the range of lOpStartIndex and lOpEndIndex
   if ( *lOpStartIndex >= 0 && *lOpEndIndex >= 0 && *lOpStartIndex < *lOpEndIndex )
      bReturn = TRUE;

   return( bReturn );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_OnEditFind
//
//    Handles the edit find event
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /* DIALOG */  OPERATION
TZEDVMLD_OnEditFind( zVIEW vSubtask )
{
   CString strMsg, strMsgFormat = "Could not find requested string\"%s\"!";
   zLONG lSearchBehavior = 0, lPosition = 0, lLine = 0, lCol = 0, lCharCount = 0;
   FINDREPLACE *pfr = (FINDREPLACE *) GetActionParameters( vSubtask );

   memcpy( &g_fr, pfr, sizeof( FINDREPLACE ) );

   USE_EDITOR

   oEditor->GetCursorPosition( &lLine, &lCol, &lPosition );

   if ( zstrlen ( pfr->lpstrFindWhat) == 0 )
      return( 0 );

   // store search string for F3 / Shift - F3 use
   g_strFindWhat = pfr->lpstrFindWhat;
   lCharCount = g_strFindWhat.GetLength();
   CString strStringToFind = g_strFindWhat;

   // check if current selected text allready match search text
   // if so, set cursor to the end of the selected text but only in FR_DOWN mode

   if ( pfr->Flags & FR_WHOLEWORD )
   {
      lSearchBehavior = SRCH_REGEXPR;
      strStringToFind = "\\b";
      CString strUpper = g_strFindWhat;
      CString strLower = g_strFindWhat;
      strUpper.MakeUpper();
      strLower.MakeLower();
      for( int k = 0; k < strUpper.GetLength(); k++ )
      {
         strStringToFind += "[";
         strStringToFind += strUpper[ k ];
         strStringToFind += strLower[ k ];
         strStringToFind += "]";
      }
      strStringToFind += "[^a-zA-Z_0-9]";
      if ( oEditor->CanCopy() )
      {
         CString strHelp;
         LPSTR pszBuffer = strHelp.GetBufferSetLength( lCharCount );
//         oEditor->GetTextFromRange( pszBuffer, lPosition, lPosition + lCharCount );
         strHelp.ReleaseBuffer();
         if ( strHelp.CompareNoCase( g_strFindWhat ) == 0 && pfr->Flags & FR_DOWN )
         {
            lPosition += lCharCount;
  //          oEditor->SetCursorPositionByIndex( lPosition );
         }
      }
   }

   if ( pfr->Flags & FR_MATCHCASE )
   {
      lSearchBehavior |= SRCH_MATCHCASE;
      g_bMatchCase = TRUE;
//      strStringToFind = "\\b";
      strStringToFind = g_strFindWhat;
      //strStringToFind += "[^a-zA-Z_0-9]";
   }

   if ( pfr->Flags & FR_DOWN )
      lSearchBehavior |= SRCH_FORWARD;

   //oEditor->FindTextPosition( strStringToFind, &lPosition, lSearchBehavior );
   if ( lPosition > -1 )
   {
         //oEditor->SelectRange( lPosition, lCharCount);
   }
   else
   {
      strMsg.Format( strMsgFormat, pfr->lpstrFindWhat );
      MB_SetMessage( vSubtask, MAIN_DIL, strMsg );
   }

   return( 0 );

} // TZEDVMLD_OnEditFind

zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_LineChange( zVIEW vSubtask )
{
   zLONG       lLine, lCol;
   zCHAR       szData[ 30 ];

   USE_EDITOR

   if ( !oEditor )
      return( 0 );

   IRange range( oEditor->GetSel( TRUE ) );

   lLine = range.GetEndLineNo();
   lCol  = range.GetEndColNo();
   zsprintf( szData, "(%ld,%ld)", lLine + 1, lCol + 1);

   MB_SetMessage( vSubtask, LINECOL_DIL, szData );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_ModifiedChange( zVIEW vSubtask )
{
   zCHAR       szData[ 30 ];

   USE_EDITOR

   if ( !oEditor )
      return( 0 );

   zBOOL bChange = (zBOOL)oEditor->GetModified( );
   if ( bChange )
      strcpy_s( szData, zsizeof( szData ), "*" );
   else
      strcpy_s( szData, zsizeof( szData ), "" );

   MB_SetMessage( vSubtask, FILECHANGED_DIL, szData );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_Keystroke( zVIEW vSubtask )
{
   zCTRL_EVENT *pCE;
   zSHORT      nRC;
   zCHAR       cKey;
   zVIEW       vEdWrk;
   zVIEW       vProfileXFER;

   //TraceLineS( "(TZEDVMLD)", "Keystroke" );

   mGetWorkView( &vEdWrk, vSubtask );
   mGetProfileView( &vProfileXFER, vSubtask );

   pCE = (zCTRL_EVENT *) GetActionParameters( vSubtask );
   cKey = (zCHAR) pCE->m_pEvent->m_pDispParams->rgvarg[ 1 ].iVal;

   // Check to see if the key is a space.  If it is then we might need to perform command completion.
   if ( cKey == ' ' )
      nRC = fnCommandCompletion( vSubtask );
      pCE->m_ulRC = 0x00000001;

   pCE->m_ulRC = 0x00000001;
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_InitMenu( zVIEW vSubtask )
{
   zVIEW      vEdWrk;
   zBOOL      bEnable   = FALSE;
   zBOOL      bReadOnly = TRUE;

   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );

   if ( oEditor->GetReadOnly() )
   {
      bReadOnly = FALSE;
   }

   BOOL bCanCopy  = oEditor->CanCopy();
   BOOL bCanPaste = oEditor->CanPaste();
   BOOL bCanUndo  = oEditor->CanUndo();
   BOOL bCanRedo  = oEditor->CanRedo();

   SetOptionState( vSubtask, "SaveFile",    zCONTROL_STATUS_ENABLED, bReadOnly );
   SetOptionState( vSubtask, "EditReplace", zCONTROL_STATUS_ENABLED, bReadOnly );

   SetOptionState( vSubtask, "EditCopy", zCONTROL_STATUS_ENABLED, bCanCopy );
   SetOptionState( vSubtask, "EditUndo", zCONTROL_STATUS_ENABLED,
                   bReadOnly ? bCanUndo : FALSE );
   SetOptionState( vSubtask, "EditRedo", zCONTROL_STATUS_ENABLED,
                   bReadOnly ? bCanRedo : FALSE );
   SetOptionState( vSubtask, "EditCut",  zCONTROL_STATUS_ENABLED,
                   bReadOnly ? bCanCopy : FALSE );
   SetOptionState( vSubtask, "EditPaste", zCONTROL_STATUS_ENABLED,
                   bReadOnly ? bCanPaste : FALSE );
   SetOptionState( vSubtask, "EditDelete",  zCONTROL_STATUS_ENABLED,
                   bReadOnly ? bCanCopy : FALSE );
   SetOptionState( vSubtask, "EditSelectAll", zCONTROL_STATUS_ENABLED,
                   !FileIsEmpty( oEditor ) );

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType,
                                  szlVML_File ) == 0 )
   {
      bEnable = TRUE;
   }

   SetOptionState( vSubtask, "ParseVML",     zCONTROL_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "GenerateC",    zCONTROL_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "EntityInsert", zCONTROL_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "KeyWordInsert", zCONTROL_STATUS_ENABLED, bEnable );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_GotoOperation
//
// Searches current source file for the operation that is referenced by cursor
// position (cursor in this case means the indicator within the editor).
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_GotoOperation( zVIEW vSubtask )
{
   TraceLineS( "TZEDVMLD ", "TZEDVMLD_GotoOperation not implemented yet" );
   return( 0 );

} // TZEDVMLD_GotoOperation

zOPER_EXPORT zSHORT OPERATION
Generate( zVIEW vSubtask, zCPCHAR cpcGenLang )
{
   zVIEW      vEdWrk;
   zVIEW      vSource;
   zCHAR      szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR      sz[ 100 ];
   zPCHAR     psz;
   zBOOL      bChanged;
   zLONG      lType = 0;
   zSHORT     nRC;

   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at _zfGenerateCFile." );
      SysMessageBox( vSubtask, szlZeidonEditor, sz, 1 );
      return( 0 );
   }

   // Get the Source Meta for the current buffer.
   GetIntegerFromAttribute( (zPLONG) &vSource, vEdWrk, szlBuffer,
                            szlSourceViewID );
   if ( vSource == 0 )
   {
      MessagePrompt( vEdWrk, "ED0107", szlZeidonEditor,
                     "Cannot generate VML that has been opened via File/Open",
                     TRUE, zBUTTONS_OK, 0, zICON_ERROR );
      nRC = 1;
      return( nRC );
   }

   //>SetUserFlagsOn( zUF_CurrentlyParsing );

   bChanged = oEditor->GetModified();
   if ( bChanged )
   {
      MB_SetMessage( vSubtask, MAIN_DIL, "Saving file..." );
      oEditor->FileSave();
      SetUpdateFlagInMeta( vSubtask );
   }

   GetIntegerFromAttribute( &lType, vEdWrk, szlBuffer, szlMetaType );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );

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
      strcpy_s( sz, zsizeof( sz ), "Errors in Generate." );
      strcat_s( sz, zsizeof( sz ), "\n\nSee Messages in Error Box" );
      SysMessageBox( vSubtask, szlZeidonEditor, sz, 1 );
      nRC = 1;
   }
   else
   {
      strcpy_s( sz, zsizeof( sz ), "Generate completed successfully." );
      MessagePrompt( vEdWrk, "ED0003", szlZeidonEditor,
                     sz, 0, 0, 0, zICON_INFORMATION );
      nRC = 0;
   }

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

zOPER_EXPORT zSHORT OPERATION
SaveFile( zVIEW vSubtask )
{
   zBOOL  bChanged;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];

   USE_EDITOR

   bChanged = (zBOOL)oEditor->GetModified( );
   if ( bChanged && !oEditor->GetReadOnly( ) )
   {
      if ( fnTZEDVMLD_SaveFile( vSubtask, oEditor, szFileName, PARSE_FILE ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      SetUpdateFlagInMeta( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zTZEDVMLD_OpenFile( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zVIEW  vTempEdWrk;
   zVIEW  vParentSubtask;
   zSHORT nRC;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zULONG ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT;

   mGetWorkView( &vEdWrk, vSubtask );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );

   if ( OperatorPromptForFile( vSubtask, szFileName, zMAX_FILENAME_LTH + 1,
                               "VML files|*.VML|Source Files|*.VML;*.C|"
                               "All files|*.*||", 0, ulZFlags) != 1 )
   {
      return( 0 );
   }

   CreateViewFromViewForTask( &vTempEdWrk, vEdWrk, 0 );
   nRC = SetCursorFirstEntityByString( vEdWrk, szlBuffer, szlFileName, szFileName, 0 );
   if ( nRC == zCURSOR_SET )
   {
      zCHAR  sz[ zMAX_FILENAME_LTH + 100 ];

      zsprintf( sz, "File '%s' is already being edited.  Open new window "
                "in Read-Only mode?", szFileName );
      nRC = OperatorPrompt( vSubtask, szlZeidonEditor, sz, TRUE,
                            zBUTTONS_YESNOCANCEL, zRESPONSE_NO, zICON_EXCLAMATION );

      if ( nRC == zRESPONSE_CANCEL )
      {
         DropView( vTempEdWrk );
         return( 0 );
      }

      if ( nRC == zRESPONSE_NO )
      {
         GetIntegerFromAttribute( (zPLONG) &vSubtask, vEdWrk, szlBuffer, "vSubtask" );
         DropView( vTempEdWrk );
         SetFocusToCtrl( vSubtask, EDIT_CONTROL_NAME );
         return( 0 );
      }

      // Indicate that file file should be read-only.
      SetAttributeFromString( vEdWrk, szlEditor, "OpenReadOnly", "Y" );
   }

   DropView( vTempEdWrk );

   // Set attribute value so that next editing session knows what file to
   // open.
   SetAttributeFromString( vEdWrk, szlEditor, "OpenFileName", szFileName );

   // Start up another editor.  We use the parent subtask of the current
   // editor so that the new editor is a sibling of the current editor.
   GetParentWindow( &vParentSubtask, vSubtask );
   SetWindowActionBehavior( vParentSubtask, zWAB_StartModelessSubwindow | zWAB_ProcessImmediateAction,
                            "TZEDVMLD", "TZEDVMLD" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FileNew( zVIEW vSubtask )
{
   BOOL       bObjectChanged = FALSE;
   zPCHAR     szMsg = {"Do you want to save the changes?"};

   USE_EDITOR

   // Check if file changed.
   bObjectChanged = oEditor->GetModified( );
   if ( bObjectChanged == TRUE )
   {
      if ( OperatorPrompt( vSubtask, szlZeidonEditor, szMsg, TRUE,
                           zBUTTONS_YESNO, zRESPONSE_YES,
                           zICON_EXCLAMATION ) == zRESPONSE_YES )
      {
         if ( g_bIsFileNew != TRUE )
         {
            oEditor->FileSave(); // Promp user to parse current Source
            SetUpdateFlagInMeta( vSubtask );
         }
         else
         {
            FileSaveAs( vSubtask );
            g_bIsFileNew = FALSE;
         }
      }
   }

// g_bIsFileNew = oEditor->CloseObject();
   if ( g_bIsFileNew )
   {
      zVIEW  vEdWrk;
      zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
      zCHAR  szExtension[ zMAX_FILENAME_LTH + 1 ];

      mGetWorkView( &vEdWrk, vSubtask );
      GetStringFromAttribute( szExtension, zsizeof( szExtension ), vEdWrk, szlBuffer, szlLanguageType );
      strcpy_s( szFileName, zsizeof( szFileName ), "NONAME." );
      if (zstrcmp( szExtension, szlC_File ) != 0)
         strcpy_s( szExtension, zsizeof( szExtension ), szlVML_FileExtension );
      else
         strcpy_s( szExtension, zsizeof( szExtension ), szlVML_FileExtension );

      strcat_s( szFileName, zsizeof( szFileName ), szExtension );
      SetAttributeFromString( vEdWrk, szlBuffer, szlFileName, szFileName );
      SetWindowCaptionTitle( vSubtask, "Zeidon Editor", szFileName );
   }

   return( 0 );
}

// We're about to open a sub-window.  We need to name the subtask view for
// the the current editor so that the subwindow can retrieve it.
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_OpenSubwindow( zVIEW vSubtask )
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
   TZEDVMLD_OpenSubwindow( vSubtask );

   mGetProfileView( &vProfileXFER, vSubtask );

   for ( nRC = SetCursorFirstEntity( vProfileXFER, szlSubwindow, 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, szlSubwindow, 0 ) )
   {
      zPCHAR pszWindowName;

      if ( CompareAttributeToString( vProfileXFER, szlSubwindow, szlSubwindowActive, "Y" ) != 0 )
         continue;

      GetAddrForAttribute( &pszWindowName, vProfileXFER, szlSubwindow, szlName );
      SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow |
                               zWAB_ProcessImmediateAction, szlTZEDVMLD, pszWindowName );
   }

   // set Focus to Editor Window
   SetWindowState( vSubtask, zWINDOW_STATUS_SETFOCUS, TRUE );
   return( 0 );

} // fnShowSubwindows

static zSHORT
CreateFileInfoHeader( LPVMLWRAPPER oEditor,
                      zPCHAR   pszName )

{
   zCHAR    szDateRaw[ 20 ];
   zCHAR    szDateFormatted[ 20 ];
   CString csBuffer;

   SysGetDateTime( szDateRaw, zsizeof( szDateRaw ) );
   UfFormatDateTime( szDateFormatted, zsizeof( szDateFormatted ), szDateRaw, "DD-MM-YYYY" );

   csBuffer =  "/*\r\n";
   csBuffer += "/////////////////////////////////////////////////////////////////////////////\r\n";
   csBuffer += "//\r\n";
   csBuffer += "// .Name:         ";
   csBuffer += pszName;
   csBuffer += ".c\r\n";
   csBuffer += "//\r\n";
   csBuffer += "// .Version:      1.0\r\n";
   csBuffer += "//\r\n";
   csBuffer += "// .Last change:  ";
   csBuffer += szDateFormatted;
   csBuffer += "\r\n";
   csBuffer += "//\r\n";
   csBuffer += "// .Description:  Zeidon operations\r\n";
   csBuffer += "//\r\n";
   csBuffer += "/////////////////////////////////////////////////////////////////////////////\r\n";
   csBuffer += "// .Change notes\r\n";
   csBuffer += "//\r\n";
   csBuffer += "//  1.0  = New\r\n";
   csBuffer += "//        (";
   csBuffer += szDateFormatted;
   csBuffer += ") Zeidon\r\n";
   csBuffer += "//\r\n";
   csBuffer += "/////////////////////////////////////////////////////////////////////////////\r\n*/\r\n\r\n";

   csBuffer += "#define KZSYSSVC_INCL\r\n";
   csBuffer += "#include <KZOENGAA.H>\r\n";
   csBuffer += "#include <ZDRVROPR.H>\r\n";
   csBuffer += "#ifdef __cplusplus\r\n";
   csBuffer += "extern \"C\"\r\n";
   csBuffer += "{\r\n";
   csBuffer += "#endif\r\n";
   csBuffer += "#include \"ZEIDONOP.H\"\r\n\r\n";
   csBuffer += "\r\n\r\n";

   oEditor->AddText( csBuffer );
   return( 0 );

} // CreateFileInfo

static zSHORT
CreateSourceFile( zVIEW    vSubtask,
                  zVIEW    vSource,
                  LPVMLWRAPPER oEditor,
                  zBOOL    bCFile,
                  zPCHAR   szSourceFileEntityName )
{
   zVIEW    vTmp;
   zPCHAR   pszName;
   zSHORT   nRC = 0;

   IRange range( oEditor->GetSel( TRUE ) );

   range.SetStartColNo( 0 );
   range.SetEndColNo( 0 );
   range.SetStartLineNo( 0 );
   range.SetEndLineNo( 0 );

//   oEditor->SetSel( range, FALSE );
   if ( bCFile )
   {
      // create a "C" source header
      GetAddrForAttribute( &pszName, vSource, szSourceFileEntityName, szlName );

      CreateFileInfoHeader( oEditor, pszName );

      // set cursor to the last line
      oEditor->ExecuteCmd( cmCmdDocumentEnd );

      range = oEditor->GetSel( TRUE );
      // insert 2 blank lines
      oEditor->InsertLine( range.GetStartLineNo( ), "" );
      oEditor->InsertLine( range.GetStartLineNo( ), "" );
   }

   CreateViewFromViewForTask( &vTmp, vSource, 0 );
   for ( nRC = SetCursorFirstEntity( vTmp, szlOperation, "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTmp, szlOperation, "" ) )
   {
      CString strTemplate;
      OperTemplate( strTemplate, vTmp, vSubtask );
      oEditor->AddText( strTemplate );
   } // End for each Operation

   if ( bCFile )
     oEditor->AddText( "#ifdef __cplusplus\r\n}\r\n#endif\r\n" );

   DropView( vTmp );
   return( 0 );

} // CreateSourceFile

int
OperTemplate( CString &csTarget, zVIEW vSource, zVIEW vSubtask )
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
   zCHAR    szParam[ 65 ];
   zCHAR    szParamCase[ 65 ];
   zCHAR    szLine[ 65 ];
   LPSTR    szOperComment;
   LPSTR    szTab = "   ";
   CString  csFormat, csInsertBuffer, csComment;
   zSHORT   nParam;
   zSHORT   nLth;
   zSHORT   nRC;

   mGetWorkView( &vEdWrk, vSubtask );

   GetAddrForAttribute( &szOperName, vSource, szlOperation, szlName );
   GetAddrForAttribute( &szOperComment, vSource, szlOperation, "Desc" );
   GetAddrForAttribute( &szType, vSource, szlOperation, "Type" );
   GetAddrForAttribute( &szReturnDataType, vSource, szlOperation, "ReturnDataType" );
   bFileC = CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlC_File ) == 0;

   // First of all Setting up comment for the Operation
   InsertComment( csComment, szOperName, szOperComment );

   if ( szType )
   {
      if ( bFileC )
      {
         csFormat = "zOPER_EXPORT %s /*";
         switch ( *szReturnDataType )
         {
            case 'V':
               csInsertBuffer.Format( csFormat, "zVIEW" );
               break;
            case 'N':
               csInsertBuffer.Format( csFormat, "zSHORT" );
               break;
            case 'M':
               csInsertBuffer.Format( csFormat, "zDECIMAL" );
               break;
            case 'Y':
               csInsertBuffer.Format( csFormat, "zCHAR" );
               break;
            case 'L':
               csInsertBuffer.Format( csFormat, "zLONG" );
               break;
            case 'O':
               csInsertBuffer.Format( csFormat, "zVOID" );
               break;
            default:
               csInsertBuffer.Format( csFormat, "zSHORT" );
         }
         //BufInsertStr( oEditor, csInsertBuffer );
      }

      switch ( *szType )
      {
           case 'D':
         bFileC ? csInsertBuffer += "DIALOG */  OPERATION\r\n"
                : csInsertBuffer += "DIALOG OPERATION\r\n";
                   break;
           case 'E':

         bFileC ? csInsertBuffer += "ENTITY CONSTRAINT */  OPERATION\r\n"
                : csInsertBuffer += "ENTITY CONSTRAINT OPERATION\r\n";
                   break;
           case 'C':
           case 'O':

         bFileC ? csInsertBuffer += "OBJECT CONSTRAINT */  OPERATION\r\n"
                : csInsertBuffer += "OBJECT CONSTRAINT OPERATION\r\n";
                   break;
           case 'T':
         bFileC ? csInsertBuffer += "TRANSFORMATION */  OPERATION\r\n"
                : csInsertBuffer += "TRANSFORMATION OPERATION\r\n";
                   break;
           case 'A':
         bFileC ? csInsertBuffer += "DERIVED ATTRIBUTE */  OPERATION\r\n"
                : csInsertBuffer += "DERIVED ATTRIBUTE OPERATION\r\n";
                   break;
           case 'M':
         bFileC ? csInsertBuffer += "DOMAIN */  OPERATION\r\n"
                : csInsertBuffer += "DOMAIN OPERATION\r\n";
                   break;
           case 'G':
         bFileC ? csInsertBuffer += "GLOBAL */  OPERATION\r\n"
                : csInsertBuffer += "GLOBAL OPERATION\r\n";
                   break;
           case 'L':
           default:
         bFileC ? csInsertBuffer += "LOCAL */  OPERATION\r\n"
                : csInsertBuffer += "LOCAL OPERATION\r\n";
         }
   }
   else
      bFileC ? csInsertBuffer += "LOCAL */  OPERATION\r\n"
             : csInsertBuffer += "LOCAL OPERATION\r\n";

   nLth = lstrlen( szOperName ) + 2;
   csInsertBuffer +=  szOperName;
   csInsertBuffer += "( ";

   CString strParamIndent( ' ', nLth );

   // Insert parameter list.
   nParam = 0;
   for ( nRC = SetCursorFirstEntity( vSource, "Parameter", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSource, "Parameter", "" ) )
   {
      if ( nParam > 0 )
      {
         csInsertBuffer += ",\r\n";
         csInsertBuffer += strParamIndent;
         //MovRight( nLth );
      }

      nParam++;
      bPointer  = CompareAttributeToString( vSource, "Parameter", "PFlag", "Y" ) == 0;
      bUnsigned = CompareAttributeToString( vSource, "Parameter", "UFlag", "Y" ) == 0;
      GetAddrForAttribute( &szDataType, vSource, "Parameter", "DataType" );

      // create prefix for parameter name
      strcpy_s( szParam, zsizeof( szParam ), "");
      if ( bFileC )
      {
         if ( bPointer && *szDataType != 'S' )
           strcat_s( szParam, zsizeof( szParam ), "p");

         if ( bUnsigned && *szDataType == 'N' || *szDataType == 'L' )
            strcat_s( szParam, zsizeof( szParam ), "u");

         switch ( *szDataType )
         {
            case 'V':
               strcat_s( szParam, zsizeof( szParam ), "v" );
               break;

            case 'N':
               strcat_s( szParam, zsizeof( szParam ), "n" );
               break;

            case 'M':
               strcat_s( szParam, zsizeof( szParam ), "d" );
               break;

            case 'Y':
               strcat_s( szParam, zsizeof( szParam ), "c" );
               break;

            case 'L':
               strcat_s( szParam, zsizeof( szParam ), "l" );
               break;

            case 'I':
               strcat_s( szParam, zsizeof( szParam ), "lpI" );
               break;

            case 'O':
               // no prefix for void pointers
               break;

            default:
               strcat_s( szParam, zsizeof( szParam ), "sz" );
         }
      }

      // create parameter name
      nLth = (zSHORT) zstrlen( szParam );
      GetStringFromAttribute( szParam + nLth, zsizeof( szParam ) - nLth, vSource, "Parameter", "ShortDesc" );
      UfCompressName( szParam + nLth, szParam + nLth, 32 - nLth, "", "BL", "", "B_", 1 );

      strcpy_s( szLine, zsizeof( szLine ), "" );
      if ( bFileC )
      {
         // create prefix for "C"
         strcat_s( szLine, zsizeof( szLine ), "z" );

         if ( bPointer && *szDataType != 'S' ) // not for String
           strcat_s( szLine, zsizeof( szLine ), "P");

         if ( bUnsigned )
         {
            switch ( *szDataType )
            {
//               case 'Y':
               case 'N':
               case 'L':
                  strcat_s( szLine, zsizeof( szLine ), "U");
                  break;
            }
         }
      }

      switch ( *szDataType )
      {
         case 'V':
            strcat_s( szLine, zsizeof( szLine ), "VIEW ");
            strcat_s( szLine, zsizeof( szLine ), szParam );

            if ( nParam == 1 &&
                 ( *szType == 'A' || *szType == 'E' || *szType == 'T' || *szType == 'C' || *szType == 'O' ) )
            {
               zLONG ZKey;

               if ( !szLOD )
               {
                  GetIntegerFromAttribute( &ZKey, vSource, "LOD", szlZKey );
                  nRC = ActivateMetaOI_ByZKey( vSubtask, &vLOD, 0, zREFER_LOD_META,
                                               zSINGLE | zACTIVATE_ROOTONLY, ZKey, 0 );
                  if ( nRC < 0 )
                  {
                     MessagePrompt( vEdWrk, "ED0001",szlZeidonEditor,
                                    "Internal Error -- Couldn't find LOD to go with Source Meta",
                                    1, 0, 0, 0 );
                     szLOD = "error";
                  }
                  else
                     GetAddrForAttribute( &szLOD, vLOD, "LOD", szlName );
               }

               if ( bFileC )
                  strcat_s( szLine, zsizeof( szLine ), " /* " );

               strcat_s( szLine, zsizeof( szLine ), " BASED ON LOD " );
               strcat_s( szLine, zsizeof( szLine ), szLOD );
               if ( bFileC )
                  strcat_s( szLine, zsizeof( szLine ), " */" );
            }

            break;

         case 'N':
            strcat_s( szLine, zsizeof( szLine ), "SHORT ");
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;
         case 'M':
            strcat_s( szLine, zsizeof( szLine ), "DECIMAL ");
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;

         case 'Y':
            strcat_s( szLine, zsizeof( szLine ), "CHAR ");
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;

         case 'L':
            bFileC ? strcat_s( szLine, zsizeof( szLine ), "LONG " )
                   : strcat_s( szLine, zsizeof( szLine ), "INTEGER " );
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;

         case 'O':
            bFileC ? strcat_s( szLine, zsizeof( szLine ), "VOID " )
                   : strcat_s( szLine, zsizeof( szLine ), "VOID " );
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;

         case 'I':
            bFileC ? strcpy_s( szLine, zsizeof( szLine ), "LPDOMAINDATA " )
                   : strcpy_s( szLine, zsizeof( szLine ), "STRING ( 32 ) " );
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;

         case 'A':
         case 'E':
         default:
            if ( bFileC )
            {
               if ( !bPointer && GetVMLConstChar( ) )
                  strcat_s( szLine, zsizeof( szLine ), "CPCHAR " );
               else
                  strcat_s( szLine, zsizeof( szLine ), "PCHAR " );
            }
            else
               strcat_s( szLine, zsizeof( szLine ), "STRING ( 32 ) " );
            strcat_s( szLine, zsizeof( szLine ), szParam );
            break;
      }

      csInsertBuffer += szLine;

      // store parameters for CASE in VML function prototype
      switch ( nParam )
      {
         case 1:
            if ( *szType == 'M' )
            {
               strcpy_s( szParamCase, zsizeof( szParamCase ), szParam );
            }

            break;

         case 2:
            if ( *szType == 'C' || *szType == 'O' )
            {
               strcpy_s( szParamCase, zsizeof( szParamCase ), szParam );
            }

            break;

         case 3:
            if ( *szType == 'E' )
            {
               strcpy_s( szParamCase, zsizeof( szParamCase ), szParam );
            }

            break;

         case 4:
            if ( *szType == 'A' )
            {
               strcpy_s( szParamCase, zsizeof( szParamCase ), szParam );
            }

            break;
      }

   }  // End FOR EACH Parameter

   if ( bFileC )
   {
      // use dummy parameter, to avoid compiler warnings
      if (nParam == 0)
         csInsertBuffer += "void";

      csInsertBuffer += " )\r\n";
   }
   else
   {
      csInsertBuffer += " )\r\n\r\n";
   }

   if ( bFileC == FALSE )
   {
      switch ( *szType )
      {
         case 'E':
            //MovRight( 3 );
            csInsertBuffer += szTab;
            csInsertBuffer +=  "CASE ";
            csInsertBuffer +=  szParamCase;
            csInsertBuffer +=  "\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zECE_ACCEPT:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zECE_ACCEPT */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zECE_CANCEL:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zECE_CANCEL */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zECE_CREATE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zECE_CREATE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zECE_DELETE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zECE_DELETE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zECE_EXCLUDE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zECE_EXCLUDE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zECE_INCLUDE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zECE_INCLUDE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "END  /* case */\r\n";
            break;
         case 'C':
         case 'O':
            csInsertBuffer += szTab;
            csInsertBuffer +=  "CASE ";
            csInsertBuffer +=  szParamCase;
            csInsertBuffer +=  "\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zOCE_ACTIVATE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zOCE_ACTIVATE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zOCE_ACTIVATE_EMPTY:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zOCE_ACTIVATE_EMPTY */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zOCE_COMMIT:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zOCE_COMMIT */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zOCE_DROPOI:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zOCE_DROPOI */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "END  /* case */\r\n";
            break;

         case 'A':
            csInsertBuffer += szTab;
            csInsertBuffer +=  "CASE ";
            csInsertBuffer +=  szParamCase;
            csInsertBuffer +=  "\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDERIVED_GET:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDERIVED_GET */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDERIVED_SET:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDERIVED_SET */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "END  /* case */\r\n";
            break;

         case 'M':
            csInsertBuffer += szTab;
            csInsertBuffer +=  "CASE ";
            csInsertBuffer +=  szParamCase;
            csInsertBuffer +=  "\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_SET_ATTRIBUTE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_SET_ATTRIBUTE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_GET_VARIABLE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_GET_VARIABLE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_COMPARE_ATTRIBUTE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_COMPARE_ATTRIBUTE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_GET_FIRST_TBL_ENT_FOR_ATTR:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_GET_FIRST_TBL_ENT_FOR_ATTR */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_GET_NEXT_TBL_ENT_FOR_ATTR:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_GET_NEXT_TBL_ENT_FOR_ATTR */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_SET_ATTRIBUTE_VALUE_NEXT:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_SET_ATTRIBUTE_VALUE_NEXT */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_SET_ATTRIBUTE_VALUE_PREV:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_SET_ATTRIBUTE_VALUE_PREV */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_ADD_TO_ATTRIBUTE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_ADD_TO_ATTRIBUTE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_GET_COMPARE_VALUE:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_GET_COMPARE_VALUE */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "OF   zDME_VALIDATE_LPDATA:\r\n\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "     /* end zDME_VALIDATE_LPDATA */\r\n";
            csInsertBuffer += szTab;
            csInsertBuffer +=  "END  /* case */\r\n";
            break;

         case 'T':
         case 'G':
         case 'D':
         case 'L':
         default:
            csInsertBuffer +=  "\r\n";
            break;
      }
   }

   if ( bFileC )
   {
      strcpy_s( szLine, zsizeof( szLine ), "{\r\n\r\n\r\n" );
      if ( *szReturnDataType == 'O')
         strcat_s( szLine, zsizeof( szLine ), "\treturn;\r\n" );
      else
         strcat_s( szLine, zsizeof( szLine ), "\treturn( 0 );\r\n" );

      strcat_s( szLine, zsizeof( szLine ), "} // ");
      strcat_s( szLine, zsizeof( szLine ), szOperName );
      strcat_s( szLine, zsizeof( szLine ), "\r\n" );
   }
   else
   {
      strcpy_s( szLine, zsizeof( szLine ), "\r\nEND\r\n" );
   }

   csInsertBuffer += szLine;

   // insert a single string
   csTarget = csInsertBuffer;
   if (bFileC)
      csTarget += "\r\n\r\n";

   if ( vLOD )
      DropView( vLOD );

   return( 0 );
} // OperTemplate

static zSHORT
CreateErrorMessage( zVIEW  vSubtask,
                    zPCHAR szMsgText )
{
   zVIEW      vProfileXFER;
   zPCHAR     pszInvokingTool;
   zCHAR      szComponentType[ 33 ];
   zCHAR      szMsg[ 100 ];

   mGetProfileView( &vProfileXFER, vSubtask );

   GetAddrForAttribute( &pszInvokingTool, vProfileXFER, szlED, "InvokingTool" );

   if ( zstrcmp( pszInvokingTool, "GO" ) == 0 )
      strcpy_s( szComponentType, zsizeof( szComponentType ), "Global Operation Group" );
   else
   if ( zstrcmp( pszInvokingTool, "DM" ) == 0 )
      strcpy_s( szComponentType, zsizeof( szComponentType ), "Domain Group" );
   else
   if ( zstrcmp( pszInvokingTool, "ZO" ) == 0 )
      strcpy_s( szComponentType, zsizeof( szComponentType ), "LOD" );
   else
      strcpy_s( szComponentType, zsizeof( szComponentType ), "Dialog" );

   strcpy_s( szMsg, zsizeof( szMsg ), "Cannot create " );
   strcat_s( szMsg, zsizeof( szMsg ), szMsgText );
   strcat_s( szMsg, zsizeof( szMsg ), ", because " );
   strcat_s( szMsg, zsizeof( szMsg ), szComponentType );
   strcat_s( szMsg, zsizeof( szMsg ), " is not checked out." );

   MessagePrompt( vSubtask, "ED0003", szlZeidonEditor,
                  szMsg,
                  FALSE, zBUTTONS_OK, 0, zICON_EXCLAMATION );
   return( 0 );
}

/*
The regular expression implementation in CodeMax's search and replace
functionality handles the following syntax:

�   Wildcards: ? (for any character),
              + (for one or more ot something),
              * (for zero or more of something).
�   Sets of characters: Characters enclosed in square brackets will be treated as an option set.
      Character ranges may be specified with a - (e.g. [a-c]).
�   Logical OR: Subexpressions may be ORed together with the | pipe symbol.
�   Parenthesized subexpressions: A regular expression may be enclosed within parentheses and will be treated as a unit.
�   Escape characters:
      Sequences such as \t, etc. will be substituted for an equivalent single character.
      \\ represents the backslash.
*/
zSHORT
GotoCurrentCFunction( zVIEW    vSubtask,
                      zVIEW    vSource,
                      LPVMLWRAPPER oEditor,
                      zPCHAR   szOperSrch,
                      zPCHAR   szMetaName,
                      zBOOL    bCFile )
{
   zSHORT  bReturn = zCALL_ERROR;
   CString strSearchString;
   zBOOL   bStop = FALSE, bFound = FALSE;

   if ( bCFile == FALSE )
      return( 0 );

   // Find the end of the current operation by searching for the beginning
   // of the next operation.  Since we want to ignore any operations that
   // might be commented out, keep searching until we are not in a comment.
   IRange currentRange = oEditor->GetSel( FALSE );//save current position
   IRange oprStartRange = oEditor->GetSel( FALSE );
   IRange oprFoundRange;
   oprStartRange.SetStartLineNo( 0 );
   oprStartRange.SetEndLineNo( 0 );
   oprStartRange.SetEndColNo( 0 );
   oprStartRange.SetStartColNo( 0 );

   //we start a the top of the file
   oEditor->SetSel( oprStartRange, FALSE );

   // tell the wrapper that we are going to do a find from code
   // so we can ask if a wrapped search occured
   oEditor->SetRedraw( FALSE );
   oEditor->SetFindFromCode( TRUE );
   oEditor->SetFindFromCodeWrapped( FALSE );

   do
   {
      oEditor->SetSearchCaseSensitive( TRUE );
      oEditor->SetSearchWholeWord( TRUE );
      strSearchString = szOperSrch;
      oEditor->SetFindText( strSearchString );

      oEditor->ExecuteCmd( cmCmdFindNext ); // this invokes the find forward
      if ( oEditor->GetLastCmd() == cmCmdFindNext && oEditor->GetLastCmdFailure() == cmErrNotFound )
      {
         bStop = TRUE;
      }

      // current token must be a string
      if ( oEditor->GetCurrentToken() == cmTokenTypeText ||
           oEditor->GetCurrentToken() == cmTokenTypeUnknown ||
           oEditor->GetCurrentToken() == cmTokenTypeString )
      {
         // Found something that looks like the desired functionname
         // now see if it is the dedeclaration or the implementation
         //get current selection + next 20 lines
         oprStartRange = oEditor->GetSel(FALSE);
         oprFoundRange = oEditor->GetSel(FALSE);
         oprStartRange.SetEndLineNo( oprStartRange.GetEndLineNo() + 20 );
         oprStartRange.SetEndColNo( oEditor->GetLineLength(oprStartRange.GetEndLineNo()));
         oprStartRange.SetStartColNo( 0 );

         oEditor->SetSel( oprStartRange, FALSE );
         CString csBuffer = oEditor->GetSelText();

         int iFoundAt = csBuffer.Find( szOperSrch );
         if ( iFoundAt == 0 )
         {
            int k = zstrlen( szOperSrch );
            while ( k < csBuffer.GetLength( ) && bFound == FALSE )
            {
               k++;
               zCHAR c = csBuffer.GetAt( k );
               switch( c )
               {
                  case '{':
                     bFound = TRUE;
                     break;

                  case ';' :
                     k += csBuffer.GetLength( );
                     break;

                  default:
                     break;
               }
            }
         }

      }
      else
         bFound = FALSE;

   } while( bFound == FALSE && bStop == FALSE );

   oEditor->SetFindFromCode( FALSE );
   oEditor->SetFindFromCodeWrapped( FALSE );

   oEditor->SetRedraw( TRUE );
   if ( bFound )
      oEditor->SetSel( oprFoundRange, TRUE );
   else
      oEditor->SetSel( currentRange, FALSE );

   return( bReturn );
}

zSHORT
GotoCurrentOperation( zVIEW    vSubtask,
                      zVIEW    vSource,
                      LPVMLWRAPPER oEditor,
                      zPCHAR   szOperSrch,
                      zPCHAR   szMetaName,
                      zBOOL    bCFile )
{
   zSHORT bReturn = zCALL_ERROR;
   static LPSTR szOperationString = {"OPERATION"};
   CString   strSearchString;
   zBOOL bStop = FALSE, bFound = FALSE;

   if ( bCFile )
      return( GotoCurrentCFunction( vSubtask, vSource, oEditor,
                                    szOperSrch, szMetaName, bCFile ) );

   // Find the end of the current operation by searching for the beginning
   // of the next operation.  Since we want to ignore any operations that
   // might be commented out, keep searching until we are not in a comment.
   IRange currentRange = oEditor->GetSel( TRUE );//save current position
   IRange oprStartRange = oEditor->GetSel( TRUE );
   oprStartRange.SetStartLineNo( 0 );
   oprStartRange.SetEndLineNo( 0 );
   oprStartRange.SetEndColNo( 0 );
   oprStartRange.SetStartColNo( 0 );

   //we start a the top of the file
   oEditor->SetSel( oprStartRange, FALSE );

   IRange oprEndRange;

   // tell the wrapper that we are going to do a find from code
   // so we can ask if a wrapped search occured
   oEditor->SetRedraw( FALSE );
   oEditor->SetFindFromCode( TRUE );
   oEditor->SetFindFromCodeWrapped( FALSE );

   oEditor->SetSearchRegExp( FALSE );
   oEditor->SetSearchCaseSensitive( TRUE );
   oEditor->SetFindText( szOperationString );
   do
   {
      oEditor->ExecuteCmd( cmCmdFindNext ); // this invokes the find forward
      if ( oEditor->GetLastCmd() == cmCmdFindNext &&
           oEditor->GetLastCmdFailure() == cmErrNotFound )
      {
         bStop = TRUE;
      }

      // current token must be a keyword, not string or comment
      if ( oEditor->GetCurrentToken() == cmTokenTypeKeyword )
      {
         // Found Keyword OPERATION
         bFound = TRUE;

         // Now see if next word is the desired operation name
         // set range to next word
         oEditor->ExecuteCmd( cmCmdWordRight );
         CString strOprName = oEditor->GetCurrentWord();
         while( strOprName.IsEmpty() )
         {
            oEditor->ExecuteCmd( cmCmdWordRight );
            oprEndRange = oEditor->GetSel( TRUE );
            strOprName = oEditor->GetCurrentWord();
         }


         /*
         CString strHelp;
         strHelp.Format( "Operation %s found at line %d, Column %d",
                         strOprName, oprEndRange.GetStartLineNo(), oprEndRange.GetStartColNo() );
         TraceLineS( "TZEDVMLD ", strHelp );
         */
         if ( strOprName.Compare( szOperSrch ) == 0 )
         {
            bFound = TRUE;
            oprEndRange = oEditor->GetSel( TRUE );
         }
         else
         {
            bFound = FALSE;
         }
      }
   } while( bFound == FALSE && bStop == FALSE );

   oEditor->SetFindFromCode( FALSE );
   oEditor->SetFindFromCodeWrapped( FALSE );

   if ( bFound )
   {
      oEditor->SetSel( oprEndRange, TRUE );
      oEditor->SetRedraw( TRUE );
   }
   else
   {
      oEditor->SetSel( currentRange, FALSE );
      oEditor->SetRedraw( TRUE );

      //create new code here
            // set cursor to the last line
      MovEOF();
      if ( bCFile )
      {
         // in c-files search for #ifdef __cplusplus (from end to begin)
         // to get the position where to insert new operation
         GetPositionForNextInsert( oEditor );
      }

      // if file read only, do not create operation
      if ( oEditor->GetReadOnly() )
         CreateErrorMessage( vSubtask, "Operation" );
      else
      {
         CString strOper;
         // create prototypes
         OperTemplate( strOper, vSource, vSubtask );
         strOper = "\n\n" + strOper;
         if ( bCFile )
         {
            oEditor->ReplaceSel( strOper );
            zLONG lLines = oEditor->GetLineCount();
            COleVariant v( (LONG) (lLines - 3), VT_I4 );
            oEditor->ICodeSense::ExecuteCmd( cmCmdGotoLine, v );
         }
         else
         {
            oEditor->AddText( strOper );
            zLONG lLines = oEditor->GetLineCount();
            COleVariant v( (LONG) (lLines - 3), VT_I4 );
            oEditor->ICodeSense::ExecuteCmd( cmCmdGotoLine, v );
         }
      }
   }

   return( bReturn );

}// GotoCurrentOperation

static zSHORT
SetTargetExecutableName( zVIEW  vSource,
                         zVIEW  vEdWrk,
                         zPCHAR pchInvokingTool,
                         zPCHAR pchMetaName,
                         zPLONG lMetaType )
{
   // Global Operations
   if ( zstrcmp( pchInvokingTool, "GO" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "GlobalOperationGroup", "DomainAndGlobalOpGroupName" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_GOPGRP_META );
      *lMetaType = zSOURCE_GOPGRP_META;
      GetStringFromAttribute( pchMetaName, 33, vSource, "GlobalOperationGroup", "Name" );
   }
   else
   // Domains
   if ( zstrcmp( pchInvokingTool, "DM" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName",
                                 vSource, "DomainGroup", "DomainAndGlobalOpGroupName" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_DOMAINGRP_META );
      *lMetaType = zSOURCE_DOMAINGRP_META;
      GetStringFromAttribute( pchMetaName, 33, vSource, "DomainGroup", "Name" );
   }
   else
   // LOD's
   if ( zstrcmp( pchInvokingTool, "ZO" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName", vSource, "LOD", "DLL_Name" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_LOD_META );
      *lMetaType = zSOURCE_LOD_META;
      GetStringFromAttribute( pchMetaName, 33, vSource, "LOD", "Name" );
   }
   else
   if ( zstrcmp( pchInvokingTool, "WD" ) == 0 || zstrcmp( pchInvokingTool, "PN" ) == 0 )
   {
      SetAttributeFromAttribute( vEdWrk, szlBuffer, szlSourceFileName, vSource, szlSourceFile, szlName );

      SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName", vSource, "Dialog", "DLL_Name" );
      SetAttributeFromInteger( vEdWrk, szlBuffer, szlMetaType, zREFER_DIALOG_META );

      // If there is no DLL name for the window, then assume the DLL name is
      // the window name.
      if ( CompareAttributeToString( vEdWrk, szlBuffer, "TargetExecutableName", "" ) == 0 )
      {
         SetAttributeFromAttribute( vEdWrk, szlBuffer, "TargetExecutableName", vSource, "Dialog", "Tag" );
      }

      *lMetaType = zSOURCE_DIALOG_META;
      GetStringFromAttribute( pchMetaName, 33, vSource, "Dialog", "Tag" );
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
      MessageBox( NULL, lpMsgBuf, strMsgTitle, MB_OK | MB_ICONINFORMATION );

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
                zPCHAR  pchInvokingTool,
                zPCHAR  pchSourceFileEntityName,
                zLONG   lEntityNameLth,
                zPBOOL  pbFileExists )
{
   //
   // Create the name of the file.
   //
   if ( zstrcmp( pchInvokingTool, "GO" ) == 0 )
      strcpy_s( pchSourceFileEntityName, lEntityNameLth, "GlobalOperationGroup" );
   else
   if ( zstrcmp( pchInvokingTool, "DM" ) == 0 )
      strcpy_s( pchSourceFileEntityName, lEntityNameLth, "DomainGroup" );
   else
      strcpy_s( pchSourceFileEntityName, lEntityNameLth, szlSourceFile );

   // Get the source dir from the lplr view.
   GetStringFromAttribute( pchFileName, lMaxLth, vTaskLPLR, szlLPLR, szlPgmSrcDir );
   SysAppendcDirSep( pchFileName );

   // Get the subdir from the source file entity.  If GetString... returns < 0
   // then the subdir attribute is null, so nothing was retrieved.  If subdir
   // is set, then we need to add a '\'.
   if ( GetStringFromAttribute( pchFileName + zstrlen( pchFileName ), lMaxLth - zstrlen( pchFileName ),
                                vSource, pchSourceFileEntityName, szlSubdirectory ) >= 0 )
   {
      SysAppendcDirSep( pchFileName );
   }

   // Get the file name and extension.
   GetStringFromAttribute( pchFileName + zstrlen( pchFileName ), lMaxLth - zstrlen( pchFileName ),
                           vSource, pchSourceFileEntityName, szlName );
   strcat_s( pchFileName, lMaxLth, "." );
   GetStringFromAttribute( pchFileName + zstrlen( pchFileName ), lMaxLth - zstrlen( pchFileName ),
                           vSource, pchSourceFileEntityName, szlExtension );

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
SetEditorStatus( LPVMLWRAPPER oEditor,
                 zVIEW    vSource,
                 zVIEW    vEdWrk )
{
   if ( MiGetUpdateForView( vSource ) )
   {
      // Source view is updateable
      SetAttributeFromString( vEdWrk, szlBuffer, szlActiveStatus, "Y" );
      // set ReadOnly off;
      oEditor->SetReadOnly( FALSE );
   }
   else
   {
      // Source view is NOT updateable
      SetAttributeFromString( vEdWrk, szlBuffer, szlActiveStatus, "N" );
      // ReadOnly on;
      oEditor->SetReadOnly( TRUE );
   }

   return( 0 );
}

// Sets the line,col DIL message.  Since the line,col values are 0-based we
// add 1 before displaying them.
// Also checks to see if the file's been changed.
zSHORT
InitSession( zVIEW  vSubtask )
{
   zPCHAR     lpszOperName = 0;
   zPCHAR     pchInvokingTool = 0;
   zCHAR      szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR      szSourceFileEntityName[ 33 ];
   zLONG      lMetaType = 0;
   zCHAR      szMetaName[ 33 ];
   zBOOL      bFileExists = FALSE;
   zBOOL      bCFile = FALSE;
   zVIEW      vEdWrk=0;
   zVIEW      vSource = 0;
   zVIEW      vTaskLPLR = 0;
   zVIEW      vProfileXFER = 0;
   zLONG      lIndex = -1;
   zBOOL      bReadOnly = FALSE;

   USE_EDITOR

   // Flag which says that the Object is created by Menu-Command 'File New'
   g_bIsFileNew = FALSE;

// oEditor->WantKeystrokes( TRUE );
   mGetWorkView( &vEdWrk, vSubtask );
   mGetProfileView( &vProfileXFER, vSubtask );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetAddrForAttribute( &pchInvokingTool, vProfileXFER, szlED, "InvokingTool" );

   // Retrieve the source file name and open a buffer.
   GetViewByName( &vSource, "METAOPERATIONS", vSubtask, zLEVEL_TASK );
   if ( vSource )
   {
      // Clean up a little by dropping the name.
      DropNameForView( vSource, "METAOPERATIONS", vSubtask, zLEVEL_TASK );
      if ( CreateFileName( vTaskLPLR, vSource, szFileName, zsizeof( szFileName ), pchInvokingTool,
                           szSourceFileEntityName, zsizeof( szSourceFileEntityName ), &bFileExists ) < 0 )
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
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vEdWrk, szlEditor, "OpenFileName" );
      if ( zstrlen( szFileName ) == 0 )
      {
         if ( OperatorPromptForFile( vSubtask, szFileName, zMAX_FILENAME_LTH + 1,
                                     "VML files|*.VML|Source Files|*.VML;*.C|"
                                     "All files|*.*||", 0, 0) != 1 )
         {
            return( 0 );
         }
         else
            SetAttributeFromString( vEdWrk, szlEditor, "OpenFileName", szFileName );
      }

      if ( CompareAttributeToString( vEdWrk, szlEditor, "OpenReadOnly", "Y" ) == 0 )
      {
         bReadOnly = TRUE;
      }

      SetAttributeFromString( vEdWrk, szlEditor, "OpenReadOnly", "" );
   }

   // Check whether the file is marked as readonly
   CheckFileAttribute( szFileName, &bReadOnly );

   CreateEntity( vEdWrk, szlBuffer, zPOS_LAST );
   SetAttributeFromString( vEdWrk, szlBuffer, szlFileName, szFileName );
   SetMatchingAttributesByName( vEdWrk, szlBuffer, vProfileXFER, "ED", zSET_ALL );

   oEditor->FileOpen( szFileName );
   if ( bReadOnly )
   {
      // Set file read only.
      oEditor->SetReadOnly( bReadOnly );
      SetAttributeFromString( vEdWrk, szlBuffer, szlReadOnly, "Y" );
      MB_SetMessage( vSubtask, MAIN_DIL, "File is write protected");
   }

   // If vSource is 0 then we must have been started via a File/Open command
   // and therefore we don't have a meta.  We've done all we need to do so
   // exit.
   if ( vSource == 0 )
   {
      // extension checking is the only way to find out whether syntax coloring can occur
      CString strExtension = szFileName;
      int iPoint = strExtension.ReverseFind( '.' );
      CString strHelp = strExtension.Mid( iPoint + 1 );
      if ( strHelp.CompareNoCase( "VML" ) == 0 )
      {
         bCFile = FALSE;
      }
      else
      {
         bCFile = TRUE;
      }

      return( 0 );
   }

   SetAttributeFromInteger( vEdWrk, szlBuffer, szlSourceViewID, (zLONG) vSource );

   // Set the target executable name from the source meta.
   // Also set the MetaType and MetaName to be used further on.
   SetTargetExecutableName( vSource, vEdWrk, pchInvokingTool, szMetaName, &lMetaType );

   // If the source meta view is not updateable, then make sure that buffer
   // is not updateable. There is a chance that the souce meta properties conflicts
   // with readonly attribute setting of the file itself that is checked above
   if ( bReadOnly == FALSE )
      SetEditorStatus( oEditor, vSource, vEdWrk );

   // Set the LanguageType for the current buffer.
   SetAttributeFromAttribute( vEdWrk, szlBuffer, szlLanguageType, vSource, szSourceFileEntityName, szlLanguageType );

   // if source file is of type "C"
   if  ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlC_File ) != 0 )
   {
      bCFile = FALSE;
   }
   else
   {
      bCFile = TRUE;
   }

   // If the file did not previously exist or if its empty then init all operations in Meta.
   lIndex = oEditor->GetLineCount( );
   if ( bFileExists == FALSE || lIndex < 1 )
      CreateSourceFile( vSubtask, vSource, oEditor, bCFile, szSourceFileEntityName );

   // Determine if the operation is in the file or we should create a template
   GetAddrForAttribute( &lpszOperName, vSource, szlOperation, szlName );

   GotoCurrentOperation( vSubtask, vSource, oEditor, lpszOperName, szMetaName, bCFile );
   return( 0 );

} // InitSession

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_EditSelectAll
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_EditSelectAll( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor )
   {
      oEditor->SelectAll();
   }

   return( 0 );

} // TZEDVMLD_EditSelectAll

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_OpenFindReplace
//
//    Popup common Find/Replace Dialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /* DIALOG */  OPERATION
TZEDVMLD_OpenFindReplace( zVIEW vSubtask )
{
   USE_EDITOR

   oEditor->ExecuteCmd( cmCmdFindReplace );

   return( 0 );

} // TZEDVMLD_OpenFindReplace

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_EditFind
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_EditFind( zVIEW vSubtask )
{
   USE_EDITOR

   oEditor->ExecuteCmd( cmCmdFind );
   if ( oEditor->GetLastCmdFailure() == cmErrNotFound )
   {
      CString strMsg = "Unable to find ";
      strMsg += oEditor->GetFindText();
      MB_SetMessage( vSubtask, MAIN_DIL, (zPCHAR)(zCPCHAR)strMsg );
   }

   return( 0 );

} // TZEDVMLD_EditFind

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_EditUndo
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_EditUndo( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor->CanUndo() )
      oEditor->Undo();

   return( 0 );

} // TZEDVMLD_EditUndo

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_EditRedo
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_EditRedo( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor->CanRedo() )
      oEditor->Redo();

   return( 0 );

} // TZEDVMLD_EditRedo

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_EditPaste
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_EditPaste( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor->CanPaste() )
      oEditor->Paste();

   return( 0 );

}// TZEDVMLD_EditPaste

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_EditDelete
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_EditDelete( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor->CanCopy() )
      oEditor->DeleteSel();

   return( 0 );

} // TZEDVMLD_EditDelete

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_EditCut
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_EditCut( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor->CanCut() )
      oEditor->Cut();

   return( 0 );

}// TZEDVMLD_EditCut

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_EditCopy
//
zOPER_EXPORT zSHORT OPERATION
TZEDVMLD_EditCopy( zVIEW vSubtask )
{
   USE_EDITOR

   if ( oEditor->CanCopy() )
      oEditor->Copy();

   return( 0 );

} // TZEDVMLD_EditCopy

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

zBOOL
FileIsEmpty ( LPVMLWRAPPER oEditor )
{
   zBOOL bIsSpace = TRUE;
   CString strLine;


   if ( oEditor )
   {
      zLONG lLineCount = oEditor->GetLineCount();

      for( zLONG k = 0; k < lLineCount && bIsSpace; k++ )
      {
         strLine = oEditor->GetLine( k );
         strLine.TrimLeft(); // if it is a blank line, string should be empty now
         bIsSpace = strLine.IsEmpty();
      }
   }
   else
      bIsSpace = FALSE;

   return( bIsSpace );
}

void
BufInsertStr( LPVMLWRAPPER oEditor, zCPCHAR psz )
{
   oEditor->ReplaceSel( psz );
}

zOPER_EXPORT zSHORT OPERATION
ErrList_Close( zVIEW vSubtask )
{
   fnSaveSubwindowPosition( vSubtask, szlErrorList );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ErrList_ShowError( zVIEW vErrorListSubtask )
{
   zVIEW vSubtask = 0;
   zVIEW vEdWrk;
   zLONG lErrorLine = 0;

   mGetWorkView( &vEdWrk, vErrorListSubtask );
   GetParentWindow( &vSubtask, vErrorListSubtask );

   USE_EDITOR

   GetIntegerFromAttribute( &lErrorLine, vEdWrk, "Error", "LineNumber" );
   lErrorLine--;

   oEditor->SetHighlightedLine(-1);
   // other than the parser Codemax linenumbering is zero based
   oEditor->SetHighlightedLine( lErrorLine );
   oEditor->SelectLine( lErrorLine, TRUE );

   SetFocusToCtrl( vSubtask, EDIT_CONTROL_NAME );

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
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );
   psz = &szFileName[ zstrlen( szFileName ) - 3 ];
   strcpy_s( psz, zsizeof( szFileName ) - (psz - szFileName ), "ERR" );      // Change ".VML" to ".ERR".

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
      // When no compilation is needed compiler generates a message "Skipping (no relevant changes detected)"
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
SortMetaListByName( zVIEW vSubtask )
{
   zVIEW  vMetaList;

   GetViewByName( &vMetaList, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vMetaList, "W_MetaDef", "Name" );
   RefreshCtrl( vSubtask, "ListObjects" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SortMetaListByDate ( zVIEW vSubtask )
{
   zVIEW  vMetaList;

   GetViewByName( &vMetaList, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vMetaList, "W_MetaDef", "LastUpdateDate D" );
   RefreshCtrl( vSubtask, "ListObjects" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OBJ_Prebuild( zVIEW vSubtask )
{
   zSHORT   nRC;
   zVIEW    vOp;
   zVIEW    vSource;

   nRC = GetViewByName( &vOp, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
      DropView( vOp );

   RetrieveViewForMetaList( vSubtask, &vOp, zREFER_LOD_META );
   OrderEntityForView( vOp, "W_MetaDef", "Name A" );
   SetNameForView( vOp, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );

   // Make sure that there is no TZOPSRCO.
   nRC = GetViewByName( &vSource, "TZOPSRCO", vSubtask, zLEVEL_TASK );
   if ( vSource )
      DropMetaOI( vSubtask, vSource );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OBJ_PasteObjectName( zVIEW vCurrentSubtask )
{
   zVIEW      vList;
   zVIEW      vTemp;
   zVIEW      vEdWrk;
   zVIEW      vSubtask;
   zPCHAR     szOption;
   zPCHAR     szName;
   CString    zs;

   GetViewByName( &vList, szlTZCMLPLO_List, vCurrentSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vList, szlW_MetaDef ) != zCURSOR_SET )
      return( 0 );

   // Get the subtask for the editor window.
   GetParentWindow( &vTemp, vCurrentSubtask );
   GetParentWindow( &vSubtask, vTemp );
   mGetWorkView( &vEdWrk, vSubtask );

   USE_EDITOR

   GetAddrForAttribute( &szOption, vEdWrk, szlBuffer, szlPasteObjectOption );
   GetAddrForAttribute( &szName, vList, szlW_MetaDef, szlName );

   switch ( *szOption )
   {
      case 'N':
         zs = szName;
         oEditor->ReplaceSel( zs );
         break;

      case 'B':
         zs = "VIEW ";
         zs += szName;
         zs += " BASED ON LOD ";
         zs += szName;
         oEditor->ReplaceSel( zs );
         break;

      case 'G':
         zs = "GET VIEW ";
         zs += szName;
         zs += " NAMED \"";
         zs += szName;
         zs += "\"";
         oEditor->ReplaceSel( zs );
         break;

      case 'S':
         zs = "NAME VIEW ";
         zs += szName;
         zs += " \"";
         zs += szName;
         zs += "\"";
         oEditor->ReplaceSel( zs );
         break;
   }

   DropView( vList );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VOR_PasteName( zVIEW vCurrentSubtask )
{
   zVIEW      vList;
   zVIEW      vEdWrk;
   zVIEW      vTemp;
   zVIEW      vSubtask;
   zPCHAR     szOption;
   zPCHAR     pszName;
   zCHAR      sz[ 200 ];

   // Get the subtask for the editor window.
   GetParentWindow( &vTemp, vCurrentSubtask );
   GetParentWindow( &vSubtask, vTemp );
   mGetWorkView( &vEdWrk, vSubtask );

   USE_EDITOR

   GetViewByName( &vList, szlTZCMLPLO_List, vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vList, szlW_MetaDef ) != zCURSOR_SET )
      return( 0 );

   GetAddrForAttribute( &szOption, vEdWrk, szlBuffer, szlPasteVorOption );
   GetAddrForAttribute( &pszName, vList, szlW_MetaDef, szlName );

   switch ( *szOption )
   {
      case 'N':
         strcpy_s( sz, zsizeof( sz ), pszName );
         break;

      case 'R':
         sprintf_s( sz, zsizeof( sz ), "VIEW %s REGISTERED AS %s", pszName, pszName );
         break;

      case 'G':
         sprintf_s( sz, zsizeof( sz ), "GET VIEW %s NAMED \"%s\"", pszName, pszName );
         break;

      case 'S':
         sprintf_s( sz, zsizeof( sz ), "NAME VIEW %s \"%s\"", pszName, pszName );
         break;

      case 'B':
      {
         zVIEW  vVOR;
         zPCHAR pszLOD_Name;

         ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zREFER_VOR_META,
                                zSINGLE | zLEVEL_APPLICATION, pszName, zCURRENT_OI );

         GetAddrForAttribute( &pszLOD_Name, vVOR, "LOD", "Name" );
         sprintf_s( sz, zsizeof( sz ), "VIEW %s BASED ON LOD %s", pszName, pszLOD_Name );

         DropMetaOI( vSubtask, vVOR );
         break;
      }
   }

   oEditor->ReplaceSel( sz );
   DropView( vList );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VOR_Prebuild( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zVIEW  vParentSubtask;
   zVIEW  vList;

   // We need to get the parent subtask view so that we can retrieve the
   // work view.
   GetParentWindow( &vParentSubtask, vSubtask );
   mGetWorkView( &vEdWrk, vParentSubtask );

   GetViewByName( &vList, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( vList )
      DropView( vList );

   RetrieveViewForMetaList( vSubtask, &vList, zREFER_VOR_META );
   SetNameForView( vList, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_Close( zVIEW vSubtask )
{
   fnSaveSubwindowPosition( vSubtask, szlObjectSelAQ );
   return( 0 );
}

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
AEQ_RefreshAEQ_AttrList( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "AttributeList" );
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
AEQ_Postbuild( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zBOOL  bReadOnly = TRUE;
   zVIEW  vWindow;

   GetParentWindow( &vWindow, vSubtask );
   if ( vWindow )
   {
      mGetWorkView( &vEdWrk, vWindow );

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
AEQ_PasteView( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE,
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
AEQ_PasteViewEntity( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY,
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
AEQ_PasteViewEntityAttr( zVIEW vSubtask )
{
   fnPasteQualifier( vSubtask,
                     PASTE_VIEW_VARIABLE | PASTE_ENTITY | PASTE_ATTRIBUTE,
                     PASTE_WITHOUT_QUOTE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_GetViews( zVIEW vCurrentSubtask )
{
   zVIEW      vSubtask;
   zVIEW      vEdWrk;
   static LPSTR szOperationString = {"*OPERATION*"};
   SHORT     nViewType;
   CString   strObjectName;
   CString   strVarName;
   CString   strSearchString;

   mGetWorkView( &vEdWrk, vCurrentSubtask );
   if ( vEdWrk == 0 )
   {
      OperatorPrompt( vCurrentSubtask, "ED - Error", "TZEDWRKO not Available at GetViews.", 0, zBUTTONS_OK, 0, 0 );
      return( zCALL_ERROR );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) != 0 )
   {
      TraceLineS( "TZEDVMLD ", "'Get Views' only works on VML Files." );
      return( zCALL_ERROR );
   }

   GetIntegerFromAttribute( (zPLONG) &vSubtask, vEdWrk, szlBuffer, "vSubtask" );

   USE_EDITOR

   // Find the end of the current operation by searching for the beginning
   // of the next operation.  Since we want to ignore any operations that
   // might be commented out, keep searching until we are not in a comment.
   IRange currentRange = oEditor->GetSel( FALSE );
   IRange oprStartRange;
   IRange oprEndRange = oEditor->GetSel( FALSE );
   IRange vDecl;

   oEditor->SetRedraw( FALSE );
   oEditor->SetSearchRegExp( TRUE );
   oEditor->SetSearchCaseSensitive( TRUE );
   oEditor->SetFindFromCode( TRUE );
   oEditor->SetFindText( szOperationString );

   oEditor->SetFindFromCodeWrapped( FALSE );
   do
   {

      oEditor->ExecuteCmd( cmCmdFindNext ); // this invokes the find forward
      // if nothings found search up to eof
      if ( oEditor->GetFindFromCodeWrapped() )
      {
         zLONG lLine = oEditor->GetLineCount();
         zLONG lCol  = oEditor->GetLineLength( lLine );
         oprEndRange.SetEndColNo( lCol );
         oprEndRange.SetEndLineNo( lLine );
         oprEndRange.SetStartColNo( lCol );
         oprEndRange.SetStartLineNo( lLine );
         break;
      }
      else
      {
         oprEndRange = oEditor->GetSel( FALSE );
      }

   } while( oEditor->GetCurrentToken() != cmTokenTypeKeyword );

   TRACE2( "End of operation is at line %d, Column %d\n", oprEndRange.GetStartLineNo(), oprEndRange.GetStartColNo() );

   do
   {
      oEditor->ExecuteCmd( cmCmdFindPrev ); // this invokes the find backward
      /* BLZ
      if ( oEditor->GetLastCmd() == cmCmdFindPrev && oEditor->GetLastCmdFailure() == cmErrNotFound )
      {
         oEditor->SetRedraw( TRUE );
         return( zCALL_ERROR );
      }
      */
      oprStartRange = oEditor->GetSel( FALSE );

   } while( oEditor->GetCurrentToken() != cmTokenTypeKeyword );

   TRACE2( "Start of Operation is at line %d, Column %d\n", oprStartRange.GetStartLineNo(), oprStartRange.GetStartColNo() );

   for ( nViewType = 0; nViewType < 2; nViewType++ )
   {
      // NOTE: If we go back to using regular expressions then we need to change the logic
      // for finding the variable name.  Look where we set the cursor position for more.

      if ( nViewType == 0 )
      {  // searching for Based on lod
         strSearchString = "BASED ON LOD";
      }
      else
      if ( nViewType == 1 )
      {
         strSearchString = "REGISTERED AS";    // Set NOTE above.
      }

      oEditor->SetSel(oprStartRange, FALSE );
      oEditor->SetFindText( strSearchString );
      oEditor->SetFindFromCodeWrapped( FALSE );
      BOOL bFinished = FALSE;
      do
      {
         oEditor->ExecuteCmd( cmCmdFindNext ); // this invokes the find forward
         if ( oEditor->GetFindFromCodeWrapped() )
            bFinished = TRUE;

         vDecl = oEditor->GetSel( FALSE );
         TRACE2( "vDecl is at line %d, Column %d\n", vDecl.GetStartLineNo(), vDecl.GetStartColNo() );
         TRACE2( "started at line %d, Column %d\n", oprStartRange.GetStartLineNo(), oprStartRange.GetStartColNo() );
         TRACE2( "should end at line %d, Column %d\n", oprEndRange.GetStartLineNo(), oprEndRange.GetStartColNo() );
         if ( vDecl.GetStartLineNo() > oprEndRange.GetStartLineNo() ||
              vDecl.GetStartLineNo() <= oprStartRange.GetStartLineNo() )
         {
            bFinished = TRUE;
         }

         if ( !bFinished && oEditor->GetCurrentToken() == cmTokenTypeKeyword )
         {
            SHORT nRC = 0;
            zVIEW vVOR = 0;
            ZString str = oEditor->GetLine( vDecl.GetStartLineNo() );
            TRACE1( "%s\n", str );
            // tokenize str to get the variable name
            CStringArray sar;
            str.Tokenize( " \t", sar );
            // second entry is variable name e.g. VIEW CUSTOMER_LST BASED ON LOD CUSTOMER
            //                                         ============              ========
            //                                         strVarName                strObjectName
            CString strVarName = sar[ 1 ];
            CString strObjectName = sar[sar.GetSize() - 1];
            for( int k = 0; k < sar.GetSize(); k++ )
            {
               if ( sar[ k ].CompareNoCase("VIEW") == 0 )
                  strVarName = sar[k+1];
               if ( nViewType == 0 && sar[ k ].CompareNoCase("LOD") == 0 )
                   strObjectName = sar[k+1];
               if ( nViewType == 1 && sar[ k ].CompareNoCase("AS") == 0 )
                   strObjectName = sar[k+1];
            }

            // If we are searching for registered views, then szObjectName is
            // really the VOR name.  Get the object name.

            // Add view variable and Object name to editor work object.
            if ( nViewType == 1 )
            {
               // Add view variable and Object name to editor work object.
               // First, get the VOR by name.
               zVIEW vVor = 0;
               nRC = ActivateMetaOI_ByName( vSubtask, &vVor, 0, zREFER_VOR_META,
                                            zSINGLE | zLEVEL_APPLICATION,
                                            (LPSTR)(LPCSTR)strObjectName, 0 );
               if ( nRC < 0 )
               {
                  CString strHelpMessage = "Can't find Registered View named ";
                  strHelpMessage += strObjectName;
                  AfxMessageBox( strHelpMessage );
                  continue;
               }

               // Get Object name from VOR.
               GetStringFromAttribute( (LPSTR)(LPCSTR)strObjectName.GetBufferSetLength( 256 ), 256, vVor, "LOD", "Name" );
               DropMetaOI( vSubtask, vVor );
            }

            // Add view variable and Object name to editor work object.

            // If entity with szVarName does NOT exist, create it.
            // If the variable name is the same but the object name is different,
            // delete the old View and replace it with a new View.  If variable
            // name and object name are the same, then don't do anything.
            nRC = SetCursorFirstEntityByString( vEdWrk, szlView, szlVariableName, strVarName, "" );
            if ( nRC != zCURSOR_SET )
            {
               CreateEntity( vEdWrk, szlView, zPOS_LAST );
               SetAttributeFromString( vEdWrk, szlView, szlObjectName, strObjectName );
               SetAttributeFromString( vEdWrk, szlView, szlVariableName, strVarName );
            }
            else
            if ( CompareAttributeToString( vEdWrk, szlView, szlObjectName, strObjectName ) != 0 )
            {
               // We must do a delete to get rid of all Entity/Attribute entities.
               DeleteEntity( vEdWrk, szlView, zREPOS_FIRST );

               CreateEntity( vEdWrk, szlView, zPOS_LAST );
               SetAttributeFromString( vEdWrk, szlView, szlObjectName, strObjectName );
               SetAttributeFromString( vEdWrk, szlView, szlVariableName, strVarName );
            }
         }
      } while ( bFinished == FALSE );
   }

   // Replace cursor that might be changed due to search of view statements.
   oEditor->SetRedraw( TRUE );
   oEditor->SetSel( currentRange, TRUE );

   if ( CheckExistenceOfEntity( vEdWrk, szlView ) >= zCURSOR_SET )
      AEQ_SelectView( vCurrentSubtask );

   oEditor->SetFindFromCode( FALSE );
   return( 0 );

} // AEQ_GetViews

zSHORT LOCALOPER
fnPasteQualifier( zVIEW  vCurrentSubtask,
                  zSHORT nPasteCtl,
                  zBOOL  bInsertWithQuote )
{
   zVIEW  vEdWrk;
   zVIEW  vSubtask;
   zPCHAR pszEntityName;
   zPCHAR pszAttribName;
   zCHAR  sz[ 400 ];

   mGetWorkView( &vEdWrk, vCurrentSubtask );
   GetParentWindow( &vSubtask, vCurrentSubtask );

   if ( vEdWrk == 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at PasteQualifier." );
      OperatorPrompt( vCurrentSubtask, "ED - Error", sz, 0, zBUTTONS_OK, 0, 0 );
      return( zCALL_ERROR );
   }

   if ( CheckExistenceOfEntity( vEdWrk, szlView ) != zCURSOR_SET )
      return( zCALL_ERROR );

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) != 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "Paste only works for VML Files." );
      OperatorPrompt( vCurrentSubtask, szlZeidonEditor, sz, 0, zBUTTONS_OK, 0, 0 );
      return( zCALL_ERROR );
   }

   *sz = 0;

   if ( nPasteCtl & PASTE_VIEW_VARIABLE && CheckExistenceOfEntity( vEdWrk, szlView ) == zCURSOR_SET )
   {
      GetStringFromAttribute( sz + zstrlen( sz ), zsizeof( sz ) - zstrlen( sz ),
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
               strcat_s( sz, zsizeof( sz ), ", " );
            else
               strcat_s( sz, zsizeof( sz ), "." );
         }

         if ( bInsertWithQuote )
            strcat_s( sz, zsizeof( sz ), "\"" );

         // Get rid of any leading spaces.
         while ( *pszEntityName == ' ' )
            pszEntityName++;
         strcat_s( sz, zsizeof( sz ), pszEntityName );

         if ( bInsertWithQuote )
            strcat_s( sz, zsizeof( sz ), "\"" );
      }
   }

   if ( nPasteCtl & PASTE_ATTRIBUTE && CheckExistenceOfEntity( vEdWrk, szlAttribute ) == zCURSOR_SET )
   {
      GetAddrForAttribute( &pszAttribName, vEdWrk, szlAttribute, szlAttributeName );
      if ( pszAttribName )
      {
         if ( *sz )
         {
            if ( bInsertWithQuote )
               strcat_s( sz, zsizeof( sz ), ", " );
            else
               strcat_s( sz, zsizeof( sz ), "." );
         }

         if ( bInsertWithQuote )
            strcat_s( sz, zsizeof( sz ), "\"" );

         strcat_s( sz, zsizeof( sz ), pszAttribName );

         if ( bInsertWithQuote )
            strcat_s( sz, zsizeof( sz ), "\"" );
      }
   }

   if ( *sz )
   {
      CString strStringToInsert;

      USE_EDITOR

      strcat_s( sz, zsizeof( sz ), " " );
      strStringToInsert = sz;
      oEditor->ReplaceSel( strStringToInsert );

      // get the current position
      IRange range = oEditor->GetSel( FALSE );
      // set position to the end of the inserted text
      range.SetEndColNo( range.GetEndColNo() + strStringToInsert.GetLength() );
      range.SetStartColNo( range.GetEndColNo() );

      oEditor->SetSel( range, TRUE );

      SetFocusToCtrl( vSubtask, EDIT_CONTROL_NAME );
   }

   return( 0 );

} // fnPasteQualifier

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
// PURPOSE:    Reads the current line in the current buffer starting at
//             the current cursor position and returns the first token found.
zSHORT LOCALOPER
fnGetNextTokenFromBuffer( LPVMLWRAPPER oEditor, LPSTR pchToken, long lIndex)
{
   /*
   CString strLine;
   LPSTR  szBuffer;
   zLONG  lReturnedBuffSize = MAX_TOKEN_LTH;
   zLONG  lCol, lLine;

   CString strDelimiter = "\t,)( ";

   pchToken[ 0 ] = 0;

   // Read line from Editor.
   szBuffer = strLine.GetBufferSetLength( lReturnedBuffSize );
   lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer, lReturnedBuffSize, lIndex );
   // in Case of an editor error (Buffer to small)
   if ( lReturnedBuffSize >= MAX_TOKEN_LTH )
   {
      lReturnedBuffSize++;
      strLine.ReleaseBuffer();
      szBuffer = strLine.GetBufferSetLength( lReturnedBuffSize );
      lReturnedBuffSize = oEditor->GetTextFromLineOfIndex( szBuffer, lReturnedBuffSize, lIndex );
   }

   strLine.ReleaseBuffer();

   // Since we retrieved the whole line we need to start where the cursor is.
   oEditor->GetPositionByIndex( lIndex, &lLine, &lCol );
   strLine = strLine.Mid( lCol, strLine.GetLength() - lCol );

   // Remove obsolete whitespaces
   strLine.TrimLeft();
   strLine.TrimRight();


   int iTokenLength = strLine.FindOneOf( strDelimiter );
   if ( iTokenLength >= 0 )
   {
      strLine = strLine.Mid( 0, iTokenLength );
   }

   strcpy_s( pchToken, zsizeof( pchToken ), strLine );
*/
   return( TRUE );
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

   // We need to get the parent subtask view so that we can retrieve the
   // work view.
   GetParentWindow( &vParentSubtask, vSubtask );
   mGetWorkView( &vEdWrk, vParentSubtask );

   GetViewByName( &vObjList, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   ActivateMetaOI( vSubtask, &vLOD, vObjList, zREFER_LOD_META, 0 );
   GetStringFromAttribute( szOpName, zsizeof( szOpName ), vObjList, "W_MetaDef", "Name" );
   strcat_s( szOpName, zsizeof( szOpName ), "." );
   nLth = (zSHORT) zstrlen( szOpName );

   for ( nRC = SetCursorFirstEntity( vLOD, "OperationList", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "OperationList", 0 ) )
   {
      GetStringFromAttribute( szOpName + nLth, zsizeof( szOpName ) - nLth, vLOD, "OperationList", "Name" );

      CreateEntity( vEdWrk, "Oper", zPOS_LAST );
      SetAttributeFromString( vEdWrk, "Oper", "Name", szOpName );
   }

   DropMetaOI( vSubtask, vLOD );
   DropView( vObjList );

   return( 0 );

} // ObjList_AddObjOpersToWorkList

// Loads a list of Objects (LODs) for the current lplr.
zOPER_EXPORT zSHORT OPERATION
ObjList_Prebuild( zVIEW vSubtask )
{
   zVIEW vObjList;

   GetViewByName( &vObjList, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( vObjList )
      DropView( vObjList );

   RetrieveViewForMetaList( vSubtask, &vObjList, zREFER_LOD_META );
   SetNameForView( vObjList, "TZEDVMLD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ObjList_RefreshSelectButton( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "LoadObjOperations" );
   return( 0 );
}

//
// Handles a pressed F1-Key in the operation-insert-dialog
//
zOPER_EXPORT zSHORT OPERATION
OpIns_StartOperationHelp( zVIEW vSubtask )
{

   zVIEW  vEdWrk;
   zCHAR  szOperName[40];
   zCHAR  szOperType[ 5 ];
   zSHORT nRC;

   // FIND OUT WHAT TYPE OF OPERATION LIST IS CURRENTLY DISPLAYED.
   mGetWorkView( &vEdWrk,vSubtask );
   GetStringFromAttribute( szOperType, zsizeof( szOperType ), vEdWrk, "OperListType", "Type" );

   // IF IT'S NOT A ZEIDON OPER LIST, GET OUT. NO HELP IS AVAILABLE.
   if ( szOperType[ 0 ] != 'Z' )
   {
      return( 0 );
   }

   // GET THE OPERATION TAG FOR THE CURRENT CURSOR POSITION.
   GetStringFromAttribute( szOperName, zsizeof( szOperName ), vEdWrk, "Oper", "Name" );

   // CALL HELP
   nRC = StartZeidonHelp( vSubtask, szOperName );

   // IF HELP WAS NOT FOUND, NOTIFY THE USER.
   if ( nRC == zCALL_ERROR )
   {
      MB_SetMessage( vSubtask, 0, "Help for this operation is not currently available." );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OpIns_RefreshSelectButton( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zCHAR  szCurrentType[ 5 ];

   mGetWorkView( &vEdWrk, vSubtask );
   GetStringFromAttribute( szCurrentType, zsizeof( szCurrentType ), vEdWrk, "OperListType", "Type" );
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

   CString    csComment;
   CString    strTemp;
   int        iOperationNameLength;

   mGetWorkView( &vEdWrk, vSubtask );

   USE_EDITOR

   // save current carret position
   IRange currentRange = oEditor->GetSel( FALSE );

   GetIntegerFromAttribute( (zPLONG) &vEditorSubtask, vEdWrk, szlBuffer, "vSubtask" );

   bVML_File = (CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) == 0);
   bInsertComment = (CompareAttributeToString( vEdWrk, szlBuffer, szlOpIns_InsertDescribtion, "Y" ) == 0);
   GetAddrForAttribute( &szStr, vOp, szlOperation, szlName );
   if ( bInsertComment )
   {
      zPCHAR  szComment;
      zLONG   lCommentLength = 0;
      GetAddrForAttribute( &szComment, vOp, szlOperation, szlDesc );

      lCommentLength = zstrlen( szComment );
      if ( lCommentLength > 0 )
      {
         csComment  = "/*\n";
         csComment += szComment;
         csComment += "*/\n";
         int nNewLineCount = 0;
         for( int k = 0; k < csComment.GetLength(); k++ )
         {
            if ( csComment.GetAt( k ) == '\n' )
               nNewLineCount++;
         }
         currentRange.SetStartLineNo( currentRange.GetStartLineNo() + nNewLineCount );
         currentRange.SetEndLineNo( currentRange.GetStartLineNo() );
      }
      else
         csComment.Empty();
   }

   // this is the operation name
   strTemp = szStr;
   strTemp += "( ";

   iOperationNameLength = strTemp.GetLength();
   int iIndention = currentRange.GetStartColNo() + iOperationNameLength;

   // create indentionstring for parameters 2, 3, 4 ...
    CString    strIndent( ' ', iIndention );

   // to select the first parameter we need to add the stringlength
   // of the operationname to the selection start
   currentRange.SetStartColNo( currentRange.GetStartColNo() + iOperationNameLength );

   int iParameter = 0;
   nRC = SetCursorFirstEntity( vOp, "Parameter", "" );
   while ( nRC == zCURSOR_SET )
   {
      iParameter++;
      CString strParameter;
      GetAddrForAttribute( &szStr, vOp, "Parameter", "PFlag" );
      if ( !bVML_File && *szStr == 'Y' )
         strParameter += "PTR ";

      GetAddrForAttribute( &szStr, vOp, "Parameter", "UFlag" );
      if ( !bVML_File && *szStr == 'Y' )
         strParameter += "UNSIGNED ";

      GetAddrForAttribute( &szStr, vOp, "Parameter", "DataType" );
      if ( *szStr )
      {
         if ( bVML_File )
         {
            switch ( *szStr )
            {
               case 'A':
                  strParameter += "\"ATTRIBUTE_NAME\"";
                  break;

               case 'E':
                  strParameter +="\"ENTITY_NAME\"";
                  break;

               case 'L':
                  strParameter += "INTEGER";
                  break;

               case 'M':
                  strParameter += "DOUBLE";
                  break;

               case 'N':
                  strParameter += "INTEGER";
                  break;

               case 'O':
                  strParameter += "VOID";
                  break;

               case 'S':
                  strParameter += "STRING";
                  break;

               case 'V':
                  strParameter += "VIEW";
                  break;

               case 'Y':
                  strTemp += "STRING";
                  break;

               default :
                  strParameter += szStr;
                  break;

            } // switch
         }
         else
         {
            switch ( *szStr )
            {
               case 'A':
                  strParameter += "\"ATTRIBUTE_NAME\"";
                  break;

               case 'E':
                  strParameter += "\"ENTITY_NAME\"";
                  break;

               case 'L':
                  strParameter +="zLONG";
                  break;

               case 'M':
                  strParameter += "DOUBLE";
                  break;

               case 'N':
                  strParameter += "zSHORT";
                  break;

               case 'O':
                  strParameter += "void";
                  break;

               case 'S':
                  strParameter += "zCHAR";
                  break;

               case 'V':
                  strParameter += "zVIEW";
                  break;

               case 'Y':
                  strParameter += "zCHAR";
                  break;

               default :
                  strParameter += szStr;
                  break;

            } // switch
         } // if ( bVML_File )...else...
      }

      GetAddrForAttribute( &szStr, vOp, "Parameter", "ShortDesc" );
      if ( *szStr )
      {
         strParameter += " /* ";
         strParameter += szStr;
         strParameter += " */";
      }

      // if it is the first parameter we want to select it after
      // pasting the text
      if ( iParameter == 1 )
      {
         currentRange.SetEndColNo( currentRange.GetStartColNo() + strParameter.GetLength());
      }

      strTemp += strParameter;

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
   if ( !bVML_File )
      strTemp += ";";

   if ( csComment.GetLength() > 0 )
   {
      csComment += strTemp;
      strTemp = csComment;
   }

   oEditor->ReplaceSel( strTemp );
   oEditor->SetSel( currentRange, TRUE );
   return( 0 );

} // PasteOperation

zOPER_EXPORT zSHORT OPERATION
OpIns_RebuildOperList( zVIEW vSubtask )
{
   zCHAR  szCurrentType[ 5 ];
   zVIEW  vEdWrk;

   mGetWorkView( &vEdWrk, vSubtask );
   GetStringFromAttribute( szCurrentType, zsizeof( szCurrentType ), vEdWrk, szlBuffer, "OpIns_CurrentType" );

   // Default of CurrentType is 'Z'...
   if ( *szCurrentType == 0 )
   {
      strcpy_s( szCurrentType, zsizeof( szCurrentType ), "Z" );
      SetAttributeFromString( vEdWrk, szlBuffer, "OpIns_CurrentType", szCurrentType );
   }

   // Check to see if the operations for the current type are already
   // loaded. If they are, then delete the list.
   if ( SetCursorFirstEntityByString( vEdWrk, "OperListType", "Type", szCurrentType, 0 ) == zCURSOR_SET )
   {
      DeleteEntity( vEdWrk, "OperListType", zREPOS_FIRST );
   } // if ( SetCursor...)...

   // Now call BuildOperList to rebuild the oper list.
   OpIns_BuildOperList( vSubtask );
   return( 0 );

} // OpIns_RebuildOperList

zOPER_EXPORT zSHORT OPERATION
OpIns_Postbuild( zVIEW vSubtask )
{
   zVIEW vEdWrk;

   TZEDVMLD_DisableActions( vSubtask );

   // Get the subtask view for the parent editor window and give it another
   // name at the current subtask level so we can easily retrieve it.
   GetViewByName( &vEdWrk, "__EditorSubtask", vSubtask, zLEVEL_TASK );
   SetNameForView( vEdWrk, szlTZEDWRKO, vSubtask, zLEVEL_SUBTASK );

   fnSetSubwindowPosition( vSubtask, szlOperationsList );

   OpIns_BuildOperList( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OpIns_InsertOperation( zVIEW vCurrentSubtask )
{
   zCHAR  szType[ 5 ];
   zVIEW  vEdWrk;
   zVIEW  vTemp;
   zVIEW vSubtask;

   // Get the subtask for the editor window.
   //GetParentWindow( &vTemp, vCurrentSubtask );
   GetParentWindow( &vSubtask, vCurrentSubtask );

   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );

   //GetStringFromAttribute( szType, zsizeof( szType ), vEdWrk, szlOperListType, szlType );
   GetStringFromAttribute( szType, zsizeof( szType ), vEdWrk, szlOper, szlType );

   switch ( *szType )
   {
      case 'Z':
      {
         zVIEW vTZOPSIGK;

         // User wants to insert a Zeidon operation.  Retrieve the view that
         // contains the list of Zeidon operations and past the operation.
         GetViewByName( &vTZOPSIGK, szlTZOPSIGK, vSubtask, zLEVEL_TASK );
         SetCursorFirstEntityByAttr( vTZOPSIGK, szlOperation, szlName, vEdWrk, szlOper, szlName, 0 );
         PasteOperation( vSubtask, vTZOPSIGK );

         break;
      }

      case 'G':
      {
         zVIEW vOp;
         zCHAR  szOpName[ 255 ];

         // User wants to insert a Global operation.  Retrieve the Global
         // Operations Group view that contains the operation.

         GetStringFromAttribute( szOpName, zsizeof( szOpName ), vEdWrk, szlOper, szlName );
         ActivateMetaOI_ByName( vSubtask, &vOp, 0, zREFER_GO_META,
                                zSINGLE | zLEVEL_APPLICATION, szOpName, zCURRENT_OI );
         PasteOperation( vSubtask, vOp );

         break;
      }

      case 'L':
      {
         zVIEW vMeta;
         zLONG lMetaType;

         // User wants to insert a "Local"operation.  Retrieve the meta and paste.

         GetIntegerFromAttribute( (zPLONG) &vMeta, vEdWrk, szlBuffer, szlSourceViewID );
         GetIntegerFromAttribute( &lMetaType, vEdWrk, szlBuffer, szlMetaType );

         // Create a temp view so that we can muck around with cursors.
         CreateViewFromViewForTask( &vMeta, vMeta, 0 );

         if ( lMetaType == zREFER_DIALOG_META || lMetaType == zREFER_LOD_META )
         {
            // Dialog and LOD meta's can have multiple source files for one meta.
            // Make sure we are dealing with the opertions for the correct source file.
            SetCursorFirstEntityByAttr( vMeta, szlSourceFile, szlName,
                                        vEdWrk, szlBuffer, szlSourceFileName, 0 );
         }

         SetCursorFirstEntityByAttr( vMeta, szlOperation, szlName, vEdWrk, szlOper, szlName, 0 );

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
         GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vEdWrk, szlOper, szlName );

         // Look for the period between the Object name and the Oper name.
         for ( psz = szObjectName; *psz && *psz != '.'; psz++ )
            ; // Nothing needs to be done here.

         // Change the period to a null-term and bump up the pointer to
         // point to the oper name.
         *psz++ = 0;

         // Copy the operation name.
         strcpy_s( szOperName, zsizeof( szOperName ), psz );

         // Activate the object meta.
         ActivateMetaOI_ByName( vSubtask, &vObj, 0, zREFER_LOD_META,
                                zSINGLE | zLEVEL_APPLICATION, szObjectName, zFORCE_RELOAD );

         // Set the cursor to the operation.
         SetCursorFirstEntityByString( vObj, szlOperation, szlName, szOperName, szlLOD );

         PasteOperation( vSubtask, vObj );

         DropMetaOI( vSubtask, vObj );
         break;

      } // case 'O'...

      case 'X':
      {
         CString  strIndent;
         zVIEW    vEditorSubtask;
         zCHAR    szBuffer[ 1024 ];
         zLONG    lOperationNameLth = 0,
                  lLine, lColumn, lIndex;
         zLONG    lTabSize = 4; // Today a fixed length, in future this has
                                // to be queried from the edit-control
         zBOOL    bVML_File;

         GetStringFromAttribute( szBuffer, zsizeof( szBuffer ), vEdWrk, szlOper, szlName );
         BuildTZEDWRKO( vEdWrk, szBuffer, zsizeof( szBuffer ) );



         if ( lTabSize > 1 )  // Don't ask why ... it's hard to explain
            lTabSize--;

         // Get the C++ object handle for the editor control.
         GetIntegerFromAttribute( (zPLONG) &vEditorSubtask, vEdWrk, szlBuffer, "vSubtask" );


         bVML_File = (CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType, szlVML_File ) == 0);

         // Get length of operation name and add position within current line
         lOperationNameLth = zstrlen( szBuffer );

         oEditor->GetCursorPosition( &lLine, &lColumn, &lIndex );
         lOperationNameLth += lColumn;
//TMV    lColumn = GetTabsInLine( oEditor, lColumn ); // Get number of tabs until current Cursorposition
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
            strcat_s( szBuffer, zsizeof( szBuffer ), ";" );

         strIndent += szBuffer;

         oEditor->ReplaceSel( strIndent );

         break;
      }

      default:
         SysMessageBox( vSubtask, "Zeidon Internal", "This operation type not yet supported", 0 );
         break;

   } // switch ( *szType )...

   // Set Focus to Editor
   GetParentWindow( &vTemp, vSubtask );
   SetFocusToCtrl( vTemp, EDIT_CONTROL_NAME );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_CreateText_Exit( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   mGetProfileView( &vProfileXFER, vSubtask );
   OrderEntityForView( vProfileXFER, "VML_Text", "Text A" );

   return( 0 );

} // VML_CreateText_Exit

zOPER_EXPORT zSHORT OPERATION
VML_BuildTextList( zVIEW vCurrentSubtask )
{
   zSHORT k;
   zVIEW  vProfileXFER;
   zVIEW  vSubtask;

   mGetProfileView( &vProfileXFER, vCurrentSubtask );

   GetParentWindow( &vSubtask, vCurrentSubtask );

   USE_EDITOR

   oEditor->ResetCommands();


   // Add each of the 'standard' VML statements to the text list.
   for ( k = 0; g_VML_TextStrings[ k ].pszText != 0; k++ )
   {
      zBOOL bFirstChar = FALSE;
      if ( g_VML_TextStrings[ k ].pszCC_FirstChar &&
           *(g_VML_TextStrings[ k ].pszCC_FirstChar ) == 'Y' )
      {
         bFirstChar = TRUE;
      }

      if ( g_VML_TextStrings[ k ].pszCC_Code &&
           *(g_VML_TextStrings[ k ].pszCC_Code) )
      {
         oEditor->AddCommandToComplete( g_VML_TextStrings[ k ].pszText,
                                        g_VML_TextStrings[ k ].pszType,
                                        g_VML_TextStrings[ k ].pszInsertText,
                                        g_VML_TextStrings[ k ].pszInsertTextFull,
                                        g_VML_TextStrings[ k ].pszCC_Code,
                                        bFirstChar );
      }

      if ( SetCursorFirstEntityByString( vProfileXFER, "VML_Text", "Text",
                                         g_VML_TextStrings[ k ].pszText, 0 ) != zCURSOR_SET )
      {
         // Text doesn't exist, so create it.
         CreateEntity( vProfileXFER, "VML_Text", zPOS_LAST );

         SetAttributeFromString( vProfileXFER, "VML_Text", "Text", g_VML_TextStrings[ k ].pszText );
         SetAttributeFromString( vProfileXFER, "VML_Text", "Type", g_VML_TextStrings[ k ].pszType );
         SetAttributeFromString( vProfileXFER, "VML_Text", "UserDefined", "N" );

         if ( g_VML_TextStrings[ k ].pszInsertText )
            SetAttributeFromString( vProfileXFER, "VML_Text", "InsertText", g_VML_TextStrings[ k ].pszInsertText );

         if ( g_VML_TextStrings[ k ].pszInsertTextFull )
            SetAttributeFromString( vProfileXFER, "VML_Text", "InsertTextFull", g_VML_TextStrings[ k ].pszInsertTextFull );

         if ( g_VML_TextStrings[ k ].pszCC_Code )
            SetAttributeFromString( vProfileXFER, "VML_Text", "CC_Code", g_VML_TextStrings[ k ].pszCC_Code );

         if ( g_VML_TextStrings[ k ].pszCC_FirstChar )
            SetAttributeFromString( vProfileXFER, "VML_Text", "CC_FirstChar", g_VML_TextStrings[ k ].pszCC_FirstChar );
      }
   }

   OrderEntityForView( vProfileXFER, "VML_Text", "Text A" );
   VML_DisplayList( vSubtask );
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
   mGetProfileView( &vProfileXFER, vSubtask );
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
   szBuffer[j] = 0;

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
VML_ShowPopupMenu( zVIEW vSubtask )
{
// POINT  pt;
   CPoint pt( GetMessagePos( ) );
// pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "Popup", pt.x, pt.y, FALSE, FALSE );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_SelectItem( zVIEW vSubtask )
{
   MB_SetMessage( vSubtask, 0, "Press F1 for Help on this item." );

   return( 0 );
}

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
   mGetProfileView( &vProfileXFER, vSubtask );
   *szShowStr = 0;

   // Build a string that contains each of the VML types that the user
   // wants shown.
   if ( CompareAttributeToString( vEdWrk, szlBuffer, "VML_ShowDeclarations", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "D" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, "VML_ShowControlStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "C" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, "VML_ShowViewStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "V" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, "VML_ShowEntityStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "E" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, "VML_ShowOperators", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "O" );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, "VML_ShowOther", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "?" );
   }

   lEntityCount = 0;
   lSelectedCount = 0;

   // szShowStr lists all the types that the user wants shown. eg "CV?" means
   // that the user wants all control, view, and 'other' vml statements shown.
   for ( nRC = SetCursorFirstEntity( vProfileXFER, "VML_Text", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, "VML_Text", 0 ) )
   {
      zCHAR szType[ 5 ];

      lEntityCount++;

      GetStringFromAttribute( szType, zsizeof( szType ), vProfileXFER, "VML_Text", "Type" );
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

   sprintf_s( szMsg, zsizeof( szMsg ), "Listing %ld of %ld", lSelectedCount, lEntityCount );
   MB_SetMessage( vSubtask, 0, szMsg );

   return( 0 );

} // VML_DisplayList

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
VML_CreateText_DeleteAll( zVIEW vCurrentSubtask )
{
   zVIEW      vSubtask;
   zVIEW  vProfileXFER;
   zSHORT nRC;

   mGetProfileView( &vProfileXFER, vCurrentSubtask );

   GetParentWindow( &vSubtask, vCurrentSubtask );

   USE_EDITOR

   if ( MessagePrompt( vCurrentSubtask, "ED0101", "Zeidon Editor",
                       "Delete All VML Keywords?", 0, zBUTTONS_YESNO,
                       zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   oEditor->ResetCommands();

   mGetProfileView( &vProfileXFER, vSubtask );
   for ( nRC = SetCursorFirstEntity( vProfileXFER, "VML_Text", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, "VML_Text", 0 ) )
   {
      DeleteEntity( vProfileXFER, "VML_Text", zREPOS_NONE );
   }

   return( 0 );

} // VML_CreateText_DeleteAll

zOPER_EXPORT zSHORT OPERATION
VML_InsertText( zVIEW vCurrentSubtask )
{
   zVIEW      vEdWrk;
   zVIEW      vSubtask;
   zVIEW      vProfileXFER;
   zVIEW      vTemp;

   mGetProfileView( &vProfileXFER, vCurrentSubtask );
   mGetWorkView( &vEdWrk, vCurrentSubtask );

   GetParentWindow( &vSubtask, vCurrentSubtask );

   USE_EDITOR

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlReadOnly, "Y" ) != 0 &&
        CompareAttributeToString( vEdWrk, szlBuffer, szlActiveStatus, "N" ) != 0 )
   {
      fnInsertVML_Text( vSubtask, vEdWrk, vProfileXFER, oEditor );
   }

   // Set Focus to Editor.
   GetParentWindow( &vTemp, vSubtask );
   SetFocusToCtrl( vTemp, EDIT_CONTROL_NAME );

   return( 0 );
}

// Insert the currently selected text.
zSHORT LOCALOPER
fnInsertVML_Text( zVIEW      vSubtask,
                  zVIEW      vEdWrk,
                  zVIEW      vProfileXFER,
                  LPVMLWRAPPER oEditor )
{
   zCHAR      szInsertString[ 1000 ];
   zPCHAR     pszStrBegin;
   zPCHAR     pszStrEnd;
   zLONG      lOriginalCol;
   zLONG      lPositionCursor;
   CString    strCompleteCommand;

   *szInsertString = 0;

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlVML_InsertFull, "Y" ) == 0 )
   {
      GetStringFromAttribute( szInsertString, zsizeof( szInsertString ), vProfileXFER,
                              szlVML_Text, szlInsertTextFull );
   }

   if ( *szInsertString == 0 )
   {
      GetStringFromAttribute( szInsertString, zsizeof( szInsertString ), vProfileXFER,
                              szlVML_Text, szlInsertText );
   }

   if ( *szInsertString == 0 )
   {
      GetStringFromAttribute( szInsertString, zsizeof( szInsertString ), vProfileXFER,
                              szlVML_Text, szlText );
   }

   if ( *szInsertString == 0 )
      return( 0 );

   lPositionCursor = (zstrchr( szInsertString, '&' ) != NULL);
   IRange range( oEditor->GetSel(TRUE) );

   lOriginalCol = range.GetEndColNo();

   IPosition pos;
   pos.CreateDispatch( "CodeSense.Position" );
   pos.SetLineNo( range.GetEndLineNo() );
   pos.SetColNo( range.GetEndColNo() );

   // Insert each line seperately.
   // Setup insert string line by line
   strCompleteCommand.Empty();
   for ( pszStrBegin = szInsertString; *pszStrBegin; pszStrBegin = pszStrEnd )
   {
      zBOOL bMoreLines;

      // Find the end of the current line by looking for the '\n' zCHAR or
      // the null terminator.
      for ( pszStrEnd = pszStrBegin;
            *pszStrEnd != '\n' && *pszStrEnd != '\r' && *pszStrEnd != 0;
            pszStrEnd++ )
      {
         // Nothing needs to be done here.
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

      if ( bMoreLines )
      {
         zCHAR  szTemp[ 100 ] = "\n";
         zSHORT k = (zSHORT) zstrlen( szTemp );

         // We need to add the new-line and then a spaces to match original
         // column position.

         zmemset( &szTemp[ k ], ' ', lOriginalCol );
         szTemp[ k + lOriginalCol ] = 0;

         strCompleteCommand += szTemp;
      }
   } // for...

   lPositionCursor = strCompleteCommand.Find( _T( "&" ) );
   if ( lPositionCursor >= 0 )
   {  // Get rid of that "&" so we don't have to call DeleteTextRange
      strCompleteCommand.SetAt( lPositionCursor, ' ' );
   }

   oEditor->InsertText( strCompleteCommand, pos );

   if ( lPositionCursor >= 0 )
   {
      //position to the place where the & was found
      range.SetStartColNo( range.GetEndColNo() + lPositionCursor );
      range.SetEndColNo( range.GetEndColNo() + lPositionCursor );
      oEditor->SetSel( range, TRUE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_DisableActions
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZEDVMLD_DisableActions( zVIEW vSubtask )
{
   zBOOL  bReadOnly = TRUE;
   zVIEW  vEdWrk;
   zVIEW  vWindow;

   GetParentWindow( &vWindow, vSubtask );
   if ( vWindow )
   {
      mGetWorkView( &vEdWrk, vWindow );

      if ( SetCursorFirstSelectedEntity( vEdWrk, "Oper", 0 ) < zCURSOR_SET ||
           CompareAttributeToString( vEdWrk, szlBuffer, szlReadOnly, "Y" ) == 0 ||
           CompareAttributeToString( vEdWrk, szlBuffer, szlActiveStatus, "N" ) == 0 )
      {
         bReadOnly = FALSE;
      }
   }

   EnableAction( vSubtask, "InsertOperation", bReadOnly );

   return( 0 );
} // TZEDVMLD_DisableActions


//
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

   GetStringFromAttribute( szCurrentType, zsizeof( szCurrentType ), vEdWrk, szlBuffer,
                           "OpIns_CurrentType" );

   // Default of CurrentType is 'Z'...
   if ( *szCurrentType == 0 )
   {
      strcpy_s( szCurrentType, zsizeof( szCurrentType ), "Z" );
      SetAttributeFromString( vEdWrk, szlBuffer, "OpIns_CurrentType", szCurrentType );
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
               oTZOPGRPO_GetViewForXGO( vSubtask, &vTZOPSIGK, zSYS_CURRENT_OI );
               SetNameForView( vTZOPSIGK, "TZOPSIGK", vSubtask, zLEVEL_TASK );
            }

            // Now copy each of the operations to the editor work object.
            for ( nRC = SetCursorFirstEntity( vTZOPSIGK, "Operation", 0 );
                  nRC == zCURSOR_SET;
                  nRC = SetCursorNextEntity( vTZOPSIGK, "Operation", 0 ) )
            {
               CreateEntity( vEdWrk, "Oper", zPOS_LAST );
               SetMatchingAttributesByName( vEdWrk, "Oper", vTZOPSIGK, "Operation", zSET_ALL );
               SetAttributeFromString( vEdWrk, "Oper", "Type", "Z" );
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
               SetMatchingAttributesByName( vEdWrk, "Oper", vOp, "W_MetaDef", zSET_ALL );
               SetAttributeFromString( vEdWrk, "Oper", "Type", "G" );
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
            GetIntegerFromAttribute( (zPLONG) &vMeta, vEdWrk, "Buffer", "SourceViewID" );
            GetIntegerFromAttribute( &lMetaType, vEdWrk, "Buffer", "MetaType" );

            // Create a new, temp view of the meta so that we can muck around
            // with the cursors.
            CreateViewFromViewForTask( &vMeta, vMeta, 0 );

            //
            // Now copy the "Local" meta operations to vEdWrk.
            //

            if ( lMetaType == zREFER_DIALOG_META || lMetaType == zREFER_LOD_META )
            {
               // Make sure we are reading the "Local" operations for
               // the current source file.
               SetCursorFirstEntityByAttr( vMeta, "SourceFile", "Name",
                                           vEdWrk, "Buffer", "SourceFileName", 0 );
            }

            // Copy all the operations to vEdWrk
            for ( nRC = SetCursorFirstEntity( vMeta, "Operation", 0 );
                  nRC == zCURSOR_SET;
                  nRC = SetCursorNextEntity( vMeta, "Operation", 0 ) )
            {
               CreateEntity( vEdWrk, "Oper", zPOS_LAST );
               SetMatchingAttributesByName( vEdWrk, "Oper", vMeta, "Operation", zSET_ALL );
               SetAttributeFromString( vEdWrk, "Oper", "Type", "L" );
            }

            DropView( vMeta );
            break;

         } // Case 'L'...

         case 'O':
         {
            // Since we load the object operations by object, we don't have to do anything here.
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
   GetStringFromAttribute( szKeyword, zsizeof( szKeyword ), vEdWrk, szlBuffer, "OpIns_Keyword" );
   if ( *szKeyword && CompareAttributeToString( vEdWrk, szlBuffer, "OpIns_UseKeyword", "Y" ) == 0 )
   {
      zCHAR  szIgnoreCase[ 5 ];
      zPCHAR pszKeyword;
      zBOOL  bStartColOne;
      zSHORT nKeywordLth;

      // De-select all operations.
      SetAllSelStatesForEntityForSet( vEdWrk, "Oper", FALSE, DISP_SELECT_SET, 0 );
      SetAllSelStatesForEntityForSet( vEdWrk, "Oper", FALSE, SEL_SELECT_SET, 0 );

      GetStringFromAttribute( szIgnoreCase, zsizeof( szIgnoreCase ), vEdWrk, szlBuffer, "OpIns_KeywordIgnoreCase" );
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

         GetStringFromAttribute( szOperName, zsizeof( szOperName ), vEdWrk, "Oper", "Name" );
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
   TZEDVMLD_DisableActions( vSubtask );

   return( 0 );

} // OpIns_BuildOperList

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

      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at ObjSelAQ_ObjSelView." );
      OperatorPrompt( vSubtask, "ED - Error", sz, 0, zBUTTONS_OK, 0, zICON_STOP );
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
      strcpy_s( sz, zsizeof( sz ), "LOD " );
      strcat_s( sz, zsizeof( sz ), szLOD_Name );
      strcat_s( sz, zsizeof( sz ), " not found. Possibly wrong LPLR?" );
      OperatorPrompt( vSubtask, "ZeidonEditor", sz, 0, 0, 1, zICON_STOP );
      return( zCALL_ERROR );
   }

   nRC = ActivateMetaOI( vSubtask, &vLOD, vList, zREFER_LOD_META, zSINGLE | zLEVEL_APPLICATION );
   DropView( vList );

   // Copy Entity and Attribute names from LOD to vEdWrk.
   nRC = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateEntity( vEdWrk, "Entity", zPOS_LAST );
      GetAddrForAttribute( &szName, vLOD, "LOD_Entity", "IndentName" );
      TraceLineS( "LOD_Entity IndentName: ", szName );
      SetAttributeFromAttribute( vEdWrk, "Entity", "EntityName", vLOD, "LOD_Entity", "IndentName" );

      nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
      while ( nRC == zCURSOR_SET )
      {
         CreateEntity( vEdWrk, "Attribute", zPOS_LAST );
         SetAttributeFromAttribute( vEdWrk, "Attribute", "AttributeName", vLOD, "ER_Attribute", "Name" );

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

///////////////////////////////////////////////////////////////////
// Setup a comment in the current editor instance
//
//
// Parameter :
//    CString csComment      I  targetstring for comment
//    LPSTR szOperName        I  Pointer to an Operation Name
//    LPSTR szOperComment     I  Pointer to a comment
//
// Return :
//    zShort number of lines inserted as a comment
//
zOPER_EXPORT zSHORT OPERATION
InsertComment( CString& csComment, LPSTR szOperName, LPSTR szOperComment )
{
// zCHAR       szCommentType[ 256 ];
   CStringList sl;
   POSITION    pos = NULL;
   CString     strSource, strLine;
   CString     strHelp,
               strCommentStart = "/*",
               strLineStart = "//    ",
               strCommentStop = "*/",
               strNewLine = "\r\n";
   zPCHAR      pszBuffer = 0;
   zLONG       lCommentLength = 0;
   zLONG       lLineLength = 100;
   zLONG       k = 0, j = 0;

   // A line containing the beginning of Operation Documentation like "//////////*..."
   CString strBegin( '*', lLineLength - strCommentStart.GetLength( ) - strCommentStop.GetLength( ) );
#if 0
   if ( GetAppOrWorkstationValue( vSource, "CommentType",
                                  szCommentType, zsizeof( szCommentType ) ) &&
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

   strLine = strLineStart + strNewLine;  // insert an empty comment line
   sl.AddTail(strLine );

   lCommentLength = strSource.GetLength( );
   pszBuffer = strSource.GetBufferSetLength( lCommentLength );

   // replace \r and \n with spaces
   while ( *pszBuffer )
   {
      if ( *pszBuffer == '\r' || *pszBuffer == '\n' )
         *pszBuffer = ' ';

      pszBuffer++;
   }

   strSource.ReleaseBuffer( );

   strSource.TrimLeft( );  // ensure there is no blank at the beginning
   lCommentLength = strSource.GetLength( );

   j = k = 0;
   while ( lCommentLength > 0 )
   {
      strHelp = strSource.Mid( k, lLineLength ); // take the first 100 bytes

      strHelp.TrimLeft( );  // ensure there is no blank at the begining

      // find a previous blank to have no linefeed within a word
      if ( strHelp.GetLength( ) >= lLineLength )
         j = strHelp.ReverseFind( ' ' );
      else
         j= -1;

      if ( j > -1 )
      {
         strHelp = strHelp.Left( j );
      }

      if ( !strHelp.IsEmpty() )
      {
         k += strHelp.GetLength( );   // The next 100 bytes we want to proceed with
                                      // start at k +  length of current line
         lCommentLength = lCommentLength - strHelp.GetLength( );
         strHelp.TrimRight();
         strLine = strLineStart;
         strLine = strLine + strHelp + strNewLine;
         sl.AddTail(strLine);
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
   sl.AddTail(strLine);

   // inserting the strings stored in the stringlist
   pos = sl.GetHeadPosition(); // Get the begin of the list

   while ( pos )
   {
      strHelp = sl.GetAt( pos );
      csComment += strHelp;
      sl.GetNext(pos);  // get next list entry
   }

   return( sl.GetCount( ) );
}

zOPER_EXPORT zSHORT OPERATION OpIns_Close( zVIEW vSubtask )
{
   fnSaveSubwindowPosition( vSubtask, szlOperationsList );
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

   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );
   if ( vEdWrk == 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at _zfParseFile." );
      MessagePrompt( vEdWrk, "ED0001", szlZeidonEditor, sz, 1, 0, 0, zICON_EXCLAMATION );
      return( 0 );
   }

   if ( CompareAttributeToString( vEdWrk, szlBuffer, szlLanguageType,
                                  szlVML_File ) != 0 )
   {
      strcpy_s( sz, zsizeof( sz ), "Parse only valid with VML Files." );
      MessagePrompt( vEdWrk, "ED0002", szlZeidonEditor, sz, 1, 0, 0, zICON_EXCLAMATION );
      nRC = 1;
      return( nRC );
   }

   // Get the Source Meta for the current buffer.
   GetIntegerFromAttribute( (zPLONG) &vSource, vEdWrk, szlBuffer, szlSourceViewID );
   if ( vSource == 0 )
   {
      MessagePrompt( vEdWrk, "ED0106", szlZeidonEditor,
                     "Cannot parse VML that has been opened via File/Open",
                     TRUE, zBUTTONS_OK, 0, zICON_ERROR );
      nRC = 1;
      return( nRC );
   }

   // Just look if there is anything changed
   lChanged = oEditor->GetModified( );
   if ( lChanged && !oEditor->GetReadOnly() )
   {
      oEditor->FileSave();
      SetUpdateFlagInMeta( vSubtask );
   }

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );

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

   nRC = (zSHORT) ParseSource( vSubtask, vSource, lParseAction, lParseLimit, lParseSource,
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
      TZEDVMLD_OpenSubwindow( vSubtask );
      SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow | zWAB_ProcessImmediateAction,
                               szlTZEDVMLD, "ErrorList" );

      sprintf( sz, "Error parsing File '%s.VML'", szFileName );
      MB_SetMessage( vSubtask, MAIN_DIL, sz );

      // There is a parse error.  If performing exit, then show one message,
      // otherwise show message that allows user to go directly to the
      // error file.
      if ( FALSE ) //> performing exit.
      {
         strcpy_s( sz, zsizeof( sz ), "Parse completed with Errors.\n\nSee Error File for list of errors." );
         nRC = MessagePrompt( vEdWrk, "ED0002", szlZeidonEditor, sz, 1, 0, 0, zICON_ERROR );
      }

      nRC = 1;
   }
   else
   {
      // Close Error List
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ) >= 0 )
         SetWindowActionBehavior( vWindow, zWAB_ReturnToParent | zWAB_ProcessImmediateAction | zWAB_ProcessImmediateReturn, 0, 0 );

      strcpy_s( sz, zsizeof( sz ), "Parse completed successfully." );
      MessagePrompt( vEdWrk, "ED0003", szlZeidonEditor, sz, 0, 0, 0, zICON_INFORMATION );
      oEditor->SetHighlightedLine( -1 ); // unhighlight lines
      nRC = 0;
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
SystemClose( zVIEW vSubtask )
{
   zVIEW      vEdWrk = 0;
   zVIEW      vLastEdWrk = 0;
   zVIEW      vProfileXFER = 0;

   if ( TZEDVMLD_AskForSaveWithParse( vSubtask, PARSE_FILE ) < 0 )
      return( -1 );


   // Save the current options values in the editor to the profile.
   mGetProfileView( &vProfileXFER, vSubtask );
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

   return( 0 );

} // SystemClose

zOPER_EXPORT zSHORT OPERATION
VML_Close( zVIEW vCurrentSubtask )
{
   zVIEW vSubtask;
   zVIEW vEdWrk;

   mGetWorkView( &vEdWrk, vCurrentSubtask );

   GetParentWindow( &vSubtask, vCurrentSubtask );
   USE_EDITOR

   if ( CheckExistenceOfEntity( vEdWrk, szlBuffer ) == zCURSOR_SET )
   {
      if ( CompareAttributeToString( vEdWrk, szlBuffer, szlVML_InsertFull, "Y" ) == 0 )
      {
         oEditor->SetCommandCompleteStyle( "F" );
      }
      else
      {
         oEditor->SetCommandCompleteStyle( "S" );
      }
   }

   fnSaveSubwindowPosition( vSubtask, szlVML_Insert );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZEDVMLD_AskForSaveWithParse
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
TZEDVMLD_AskForSaveWithParse( zVIEW vSubtask,
                              zSHORT nAskForParse )
{
   zVIEW      vEdWrk;
   BOOL       bObjectChanged = FALSE;
   zCHAR      szFileName[ zMAX_FILENAME_LTH + 1 ];
   zULONG     ulZFlags = 0;

   USE_EDITOR

   mGetWorkView( &vEdWrk, vSubtask );

   // Check if file changed.
   bObjectChanged = oEditor->GetModified( );

   // In 9i there was a dialog which offers something more than this
   // Save & Parse, Save, Exit etc...
   if ( bObjectChanged == FALSE || oEditor->GetReadOnly() )
      return( 0 );

   zSHORT nRC = 0;
   zCHAR  szMsg[ zMAX_FILENAME_LTH + 200 ];

   if ( g_bIsFileNew == FALSE)
   {
      // getting current file name
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vEdWrk, szlBuffer, szlFileName );
   }
   else
   {
      // This is a good name for a new source
      strcpy_s( szFileName, zsizeof( szFileName ), "Noname.vml" );
   }

   sprintf_s( szMsg, zsizeof( szMsg ), "File '%s' has changed. Do you want to save it?", szFileName );
   nRC = OperatorPrompt( vSubtask, szlZeidonEditor, szMsg, TRUE,
                         zBUTTONS_YESNOCANCEL, zRESPONSE_YES, zICON_EXCLAMATION );

   if ( nRC == zRESPONSE_CANCEL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   else
   if ( nRC == zRESPONSE_YES )
   {
      if ( fnTZEDVMLD_SaveFile( vSubtask, oEditor, szFileName, nAskForParse ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      SetUpdateFlagInMeta( vSubtask );
   }

   return( 0 );

} // TZEDVMLD_AskForSaveWithParse

zOPER_EXPORT zSHORT OPERATION
zTZEDVMLD_FileClose( zVIEW vSubtask )
{
   return( 0 );
}

zBOOL
GetPositionForNextInsert( LPVMLWRAPPER oEditor )
{
   zSHORT bReturn = zCALL_ERROR;
   CString   strSearchString;
   zBOOL bStop = FALSE, bFound = FALSE;

   if ( oEditor->GetLanguage().Compare( szlVML_FileExtension ) == 0 )
      return( FALSE );

   // Find the end of the current operation by searching for the beginning
   // of the next operation.  Since we want to ignore any operations that
   // might be commented out, keep searching until we are not in a comment.
   IRange currentRange = oEditor->GetSel( FALSE );//save current position
   IRange oprStartRange = oEditor->GetSel( FALSE );
   IRange oprFoundRange;
   oprStartRange.SetStartLineNo( oEditor->GetLineCount() - 1 );
   oprStartRange.SetEndLineNo( oEditor->GetLineCount() - 1 );
   oprStartRange.SetEndColNo( oEditor->GetLineLength( oEditor->GetLineCount() - 1 ) );
   oprStartRange.SetStartColNo( oEditor->GetLineLength( oEditor->GetLineCount() - 1 ) );

   //we start a the End of the file
   oEditor->SetSel( oprStartRange, FALSE );

   // tell the wrapper that we are going to do a find from code
   // so we can ask if a wrapped search occured
   oEditor->SetRedraw( FALSE );
   oEditor->SetFindFromCode( TRUE );
   oEditor->SetFindFromCodeWrapped( FALSE );
   oEditor->SetSearchRegExp( TRUE );
   oEditor->SetSearchCaseSensitive( TRUE );
   oEditor->SetSearchWholeWord( TRUE );
   strSearchString = "#ifdef*__cplusplus";

   do
   {
      oEditor->SetFindText( strSearchString );

      oEditor->ExecuteCmd( cmCmdFindPrev ); // this invokes the find backward
      if ( oEditor->GetLastCmd() == cmCmdFindPrev &&
          oEditor->GetLastCmdFailure() == cmErrNotFound )
      {
         bStop = TRUE;
      }

      // current token must be a string
      if ( oEditor->GetCurrentToken() == cmTokenTypeText ||
           oEditor->GetCurrentToken() == cmTokenTypeUnknown ||
           oEditor->GetCurrentToken() == cmTokenTypeString )
      {
         oprFoundRange = oEditor->GetSel( FALSE );
         zLONG lLine = oprFoundRange.GetStartLineNo();
         oprFoundRange.SetEndLineNo( lLine );
         oprFoundRange.SetEndColNo( 0 );
         oprFoundRange.SetStartColNo( 0 );
         bFound = TRUE;
      }
      else
         bFound = FALSE;

   } while( bFound == FALSE && bStop == FALSE );

   oEditor->SetFindFromCode( TRUE );
   oEditor->SetFindFromCodeWrapped( FALSE );

   oEditor->SetRedraw( TRUE );
   if ( bFound )
      oEditor->SetSel( oprFoundRange, TRUE );
   else
      oEditor->SetSel( oprStartRange, TRUE );

   oEditor->SetRedraw( TRUE );

   return( bFound );
}

#ifdef __cplusplus
}
#endif
