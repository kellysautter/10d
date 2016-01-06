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
char szlED[]                 = "ED";
char szlEditor[]             = "Editor";
char szlEditorProfile[]      = "EditorProfile";
char szlEntity[]             = "Entity";
char szlEntityName[]         = "EntityName";
char szlExtension[]          = "Extension";
char szlFileName[]           = "FileName";
char szlFileOpenSubtask[]    = "FileOpenSubtask";
char szlhBuffer[]            = "hBuffer";
char szlHeight[]             = "Height";
char szlhWndCW[]             = "hWndCW";
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
char szlOperListType[]       = "OperListType";
char szlParentCount[]        = "ParentCount";
char szlParentSubtask[]      = "ParentSubtask";
char szlParentSubtaskName[]  = "ParentSubtaskName";
char szlParentToolSubtaskID[] = "ParentToolSubtaskID";
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
char szlTZCMLPLO_List[]      = "TZEDCWZD_TZCMLPLO_List";
char szlTZEDCWZD[]           = "TZEDCWZD";
char szlTZEDWRKO[]           = "TZEDWRKO";
char szlTZOPSIGK[]           = "TZOPSIGK";
char szlTZOPSRCO[]           = "TZOPSRCO";
char szlVML_File[]           = "V";
char szlVML_Insert[]         = "VML_Insert";
char szlVML_InsertFull[]     = "VML_InsertFull";
char szlVML_Subtask[]        = "VML_Subtask";
char szlVML_Text[]           = "VML_Text";
char szlVariableName[]       = "VariableName";
char szlView[]               = "View";
char szlMETAOPERATIONS[]     = "vMETAOPERATIONS";
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

// CW system user flags                If set...
#define zUF_SetDefault        0x00000000  // normal settings.
#define zUF_NoExit            0x00000001  // don't allow user to exit.
#define zUF_CurrentlyParsing  0x00000002  // currently parsing current buffer.
#define zUF_OpeningZeidonFile 0x00000004  // opening Zeidon file.
#define zUF_PerformingExit    0x00000008  // performing exit code.
#define zUF_InWindowActivate  0x00000010  // performing WindowActivation code.
#define zUF_WindowClosed      0x00000020  // editor was closed.
#define zUF_Minimized         0x00000040  // editor is minimized.
#define zUF_InBufferSaved     0x00000080  // currently executing BuffSav event.

#define UserFlagsOn( a )      ( (a) & SysQUserFlags () )
