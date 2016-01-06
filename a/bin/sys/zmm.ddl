database ZMM [2048] {
/*
   Zeidon Model
*/

   data file "ZMM.d01" contains
           ZOF_REC0                       ,
           AccessAuthority                , /* AccessAuthority */
           Action                         , /* Action */
           ADW_InfoType                   , /* ADW_InfoType */
           Application                    , /* Application */
           Color                          , /* Color */
           ColorDef                       , /* ColorDef */
           ColorOverride                  , /* ColorOverride */
           Context                        , /* Context */
           Control                        , /* Control */
           ControlDef                     , /* ControlDef */
           ControlStandard                , /* ControlStandard */
           CPL                            , /* CPL */
           CPL_ReuseableItem              , /* CPL_ReuseableItem */
           CPLR                           , /* CPLR */
           CPLR_Enqueue                   , /* CPLR_Enqueue */
           CtrlDefOption                  , /* CtrlDefOption */
           CtrlDefSubtype                 , /* CtrlDefSubtype */
           DefinedItem                    , /* DefinedItem */
           DeliveryEnvPkg                 , /* DeliveryEnvPkg */
           Dialog                         , /* Dialog */
           Domain                         , /* Domain */
           EntpER_Model                   , /* EntpER_Model */
           ER_Attribute                   , /* ER_Attribute */
           ER_EntIdentifier               , /* ER_EntIdentifier */
           ER_Entity                      , /* ER_Entity */
           ER_FactType                    , /* ER_FactType */
           ER_RelLink                     , /* ER_RelLink */
           ER_RelType                     , /* ER_RelType */
           Event                          , /* Event */
           EventDef                       , /* EventDef */
           ExecComposite                  , /* ExecComposite */
           Font                           , /* Font */
           FontDef                        , /* FontDef */
           FontOverride                   , /* FontOverride */
           Group                          , /* Group */
           GroupSet                       , /* GroupSet */
           HeaderFile                     , /* HeaderFile */
           Hotkey                         , /* Hotkey */
           Installation                   , /* Installation */
           LOD                            , /* LOD */
           LOD_Attribute                  , /* LOD_Attribute */
           LOD_Entity                     , /* LOD_Entity */
           LPLR                           , /* LPLR */
           MapRef                         , /* MapRef */
           MapRefDef                      , /* MapRefDef */
           Menu                           , /* Menu */
           NLS_Text                       , /* NLS_Text */
           Operation                      , /* Operation */
           Option                         , /* Option */
           OrigSironField                 , /* OrigSironField */
           Page                           , /* Page */
           Parameter                      , /* Parameter */
           PhysOverride                   , /* PhysOverride */
           POD                            , /* POD */
           POD_Attribute                  , /* POD_Attribute */
           POD_Entity                     , /* POD_Entity */
           POD_EntityMapFld               , /* POD_EntityMapFld */
           POD_EntityMapRec               , /* POD_EntityMapRec */
           POD_RelLinkMapFld              , /* POD_RelLinkMapFld */
           POD_RelLinkMapRec              , /* POD_RelLinkMapRec */
           PresEnvDef                     , /* PresEnvDef */
           PropertyDef                    , /* PropertyDef */
           RegularExpression              , /* RegularExpression */
           Report                         , /* Report */
           RepositoryClient               , /* RepositoryClient */
           SA_Entity                      , /* SA_Entity */
           SA_RelType                     , /* SA_RelType */
           SironComment                   , /* SironComment */
           SironKatalog                   , /* SironKatalog */
           SourceFile                     , /* SourceFile */
           SubjectArea                    , /* SubjectArea */
           TableEntry                     , /* TableEntry */
           TE_DB_Environ                  , /* TE_DB_Environ */
           TE_DBMS_Source                 , /* TE_DBMS_Source */
           TE_FieldDataRel                , /* TE_FieldDataRel */
           TE_PhysicalDataType            , /* TE_PhysicalDataType */
           TE_TablRecDataRel              , /* TE_TablRecDataRel */
           TE_TablRecKey                  , /* TE_TablRecKey */
           UI_Spec                        , /* UI_Spec */
           UIS_Entity                     , /* UIS_Entity */
           UIS_Include                    , /* UIS_Include */
           UIS_QualList                   , /* UIS_QualList */
           UIS_Tab                        , /* UIS_Tab */
           User                           , /* User */
           UserMetaStatus                 , /* UserMetaStatus */
           ViewObjRef                     , /* ViewObjRef */
           Window                         , /* Window */
           WindowStyle                    , /* WindowStyle */
           WndDesignSpec                  , /* WndDesignSpec */
           WndEvent                       , /* WndEvent */
           Z_MetaDef                      , /* Z_MetaDef */
           Mm_UIS_Tab_for_LOD_Attribute   , /* M-M correspondence table */
           Mm_TE_DB_Envrn_ptntlycsTEDBMSSr, /* M-M correspondence table */
           Mm_TE_TblRcDtRl_sdn_TE_DB_Envrn, /* M-M correspondence table */
           Mm_ColorOverride_has_Color     , /* M-M correspondence table */
           Mm_ViewObjRef_for_Report       , /* M-M correspondence table */
           Mm_Aplctn_sxctblby_RpstryClnt  , /* M-M correspondence table */
           Mm_UI_Spec_has_ViewObjRef      , /* M-M correspondence table */
           Mm_CntrlDf_hsmsg_Oprtn         , /* M-M correspondence table */
           Mm_TE_TblRcKy_s_TE_FldDtRl     , /* M-M correspondence table */
           Mm_CntrlDf_4cptschngfrm_CntrlDf, /* M-M correspondence table */
           Mm_CntrlDf_1cptsprnt_CntrlDf   , /* M-M correspondence table */
           Mm_CntrlDf_2cptschld_CntrlDf   , /* M-M correspondence table */
           Mm_SA_Enty_snvlvdn_SbjctAr     , /* M-M correspondence table */
           Mm_TE_TblRcDtRl_mplmnts_ER_Enty, /* M-M correspondence table */
           Mm_TE_PhysclDtTyp_cnmplmnt_Dmn , /* M-M correspondence table */
           Mm_SA_RlTyp_snvlvdn_SbjctAr    , /* M-M correspondence table */
           Mm_UIS_Enty_hsmnt_LOD_Atrbt    , /* M-M correspondence table */
           Mm_UIS_Enty_hslst_LOD_Atrbt    , /* M-M correspondence table */
           Mm_SourceFile_ownedby_LOD      , /* M-M correspondence table */
           Mm_SourceFile_ownedby_Dialog   , /* M-M correspondence table */
           Mm_TEPhysclDtTypsvlblfrTEDBMSSr, /* M-M correspondence table */
           Mm_User_belongto_Installation  , /* M-M correspondence table */
           Mm_RepositoryClient_has_User   , /* M-M correspondence table */
           Mm_CPLR_isreusedby_CPLR        , /* M-M correspondence table */
           Mm_Dialog_has_ViewObjRef       , /* M-M correspondence table */
           Mm_ExcCmpst_sgrpngf_Z_MtDf     , /* M-M correspondence table */
           Mm_Dlg_cntnsptn_CPL_RsblItm    , /* M-M correspondence table */
           Mm_Dlg_cntnswndw_CPL_RsblItm   , /* M-M correspondence table */
           Mm_Dlg_cntnsgrp_CPL_RsblItm    , /* M-M correspondence table */
           Mm_Menu_for_Window             , /* M-M correspondence table */
           Mm_Action_activates_Window     ; /* M-M correspondence table */

   key  file "ZMM.k01" contains
           AccessAuthority.ZKey,
           Action.ZKey,
           ADW_InfoType.ZKey,
           Application.ZKey,
           Color.ZKey,
           ColorDef.ZKey,
           ColorOverride.ZKey,
           Context.ZKey,
           Control.ZKey,
           ControlDef.ZKey,
           CPL.ZKey,
           CPL_ReuseableItem.ZKey,
           CPLR.ZKey,
           CPLR_Enqueue.z_TimeStamp,
           CtrlDefOption.ZKey,
           CtrlDefSubtype.ZKey,
           DefinedItem.ZKey,
           DeliveryEnvPkg.ZKey,
           Dialog.ZKey,
           Domain.ZKey,
           EntpER_Model.ZKey,
           ER_Attribute.ZKey,
           ER_EntIdentifier.ZKey,
           ER_Entity.ZKey,
           ER_FactType.ZKey,
           ER_RelLink.ZKey,
           ER_RelType.ZKey,
           Event.ZKey,
           EventDef.ZKey,
           ExecComposite.ZKey,
           Font.ZKey,
           FontDef.ZKey,
           FontOverride.ZKey,
           Group.ZKey,
           GroupSet.ZKey,
           HeaderFile.ZKey,
           Hotkey.ZKey,
           Installation.ZKey,
           LOD.ZKey,
           LOD_Attribute.ZKey,
           LOD_Entity.ZKey,
           LPLR.ZKey,
           MapRef.ZKey,
           MapRefDef.ZKey,
           Menu.ZKey,
           NLS_Text.ZKey,
           Operation.ZKey,
           Option.ZKey,
           Page.ZKey,
           Parameter.ZKey,
           POD.ZKey,
           POD_Attribute.ZKey,
           POD_Entity.ZKey,
           POD_EntityMapFld.ZKey,
           POD_EntityMapRec.ZKey,
           POD_RelLinkMapFld.ZKey,
           POD_RelLinkMapRec.ZKey,
           PresEnvDef.ZKey,
           PropertyDef.ZKey,
           RegularExpression.ZKey,
           Report.ZKey,
           RepositoryClient.ZKey,
           SA_Entity.ZKey,
           SA_RelType.ZKey,
           SourceFile.ZKey,
           SubjectArea.ZKey,
           TableEntry.ZKey,
           TE_DB_Environ.ZKey,
           TE_DBMS_Source.ZKey,
           TE_FieldDataRel.ZKey,
           TE_PhysicalDataType.ZKey,
           TE_TablRecDataRel.ZKey,
           TE_TablRecKey.ZKey,
           UI_Spec.ZKey,
           UIS_Entity.ZKey,
           UIS_Include.ZKey,
           UIS_QualList.ZKey,
           User.ZKey,
           UserMetaStatus.ZKey,
           ViewObjRef.ZKey,
           Window.ZKey,
           WindowStyle.ZKey,
           WndDesignSpec.ZKey,
           WndEvent.ZKey,
           Z_MetaDef.ZKey;

record ZOF_REC0 {
           db_addr ZOF_DBADDR;
           char    ZOF_Data[ 1000 ];
}  /* Generated Overflow Record */

record AccessAuthority /* For Entity AccessAuthority */ {
                unique key long ZKey;
                           long Authority;
}

record Action /* For Entity Action */ {
                           char Tag[ 33 ];
                           long SB_SubAction;
                           long SB_NextPrev;
                           long SB_DeleteNull;
                           long SB_ActionDisabling;
                           char NoMap[ 2 ];
                           long Type;
                           char ActionTypeAssociatedText[ 100 ];
                           char TypeData[ 33 ];
                unique key long ZKey;
                           char DialogName[ 33 ];
                           char WindowName[ 33 ];
                           long OperCtrl;
}

record ADW_InfoType /* For Entity ADW_InfoType */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           long ER_EP_Tokn;
                           char Type[ 2 ];
                           long InternalLth;
                           long ExternalLth;
                           long DecimalLth;
}

record Application /* For Entity Application */ {
                unique key long ZKey;
}

record Color /* For Entity Color */ {
                unique key long ZKey;
                           char CustomColors[ 100 ] [ 1 ];
                           char Tag[ 33 ];
                           long RGB_Color;
}

record ColorDef /* For Entity ColorDef */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           long SeqNo;
}

record ColorOverride /* For Entity ColorOverride */ {
                unique key long ZKey;
                           long SeqNo_DlgCtrl;
                           long SeqNo_DlgWnd;
}

record Context /* For Entity Context */ {
                unique key long ZKey;
                           char z_Desc[ 100 ];
                           char Name[ 33 ];
                           long DecimalFormat;
                           char EditString[ 33 ];
                           char IsDefault[ 2 ];
                           char DIL_Text[ 100 ];
}

record Control /* For Entity Control */ {
                           char Tag[ 33 ];
                           char Text[ 100 ] [ 1 ];
                unique key long ZKey;
                           long Type;
                           long Subtype;
                           char DD_TargetKey[ 33 ];
                           char DD_SourceKey[ 33 ];
                           long OptionFlags;
                           char DD_Type[ 2 ];
                           long IdNbr;
                           long PSDLG_X;
                           long PSDLG_Y;
                           long SZDLG_X;
                           long SZDLG_Y;
                           char CtrlBOI[ 100 ] [ 1 ];
                           long Underline;
                           long SyncKey;
                           char DIL_Text[ 100 ];
                           char ReportGroupType[ 3 ];
                           char ForceHeaderPerEntity[ 2 ];
}

record ControlDef /* For Entity ControlDef */ {
                unique key long ZKey;
                            int LastSyncDate[ 3 ];
                           char z_Desc[ 100 ];
                           char Tag[ 33 ];
                           long Id;
                           char RT_DLL_Name[ 9 ];
                           char RT_OperationName[ 33 ];
                           char PainterDLL_Name[ 9 ];
                           char PainterC_OperationName[ 33 ];
                           char PainterConvertOperationName[ 33 ];
                           char PainterBitmap[ 7 ];
                           char MaintenanceDialog[ 33 ];
                           char MaintenanceWindow[ 33 ];
                           char AcceptsAllParents[ 2 ];
                           char AcceptsAllChildren[ 2 ];
                           char RequiresParent[ 2 ];
                           char ForceUpdateOnCreate[ 2 ];
                           char DlgClass[ 33 ];
                           char DlgMapToOI[ 33 ];
                           char DlgMapFromOI[ 33 ];
                           long DlgStyle;
                           long Invisible;
}

record ControlStandard /* For Entity ControlStandard */ {
                           long ZKey;
                           char Tag[ 33 ];
                           long IdNbr;
                           char Text[ 33 ];
}

record CPL /* For Entity CPL */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
}

record CPL_ReuseableItem /* For Entity CPL_ReuseableItem */ {
                unique key long ZKey;
}

record CPLR /* For Entity CPLR */ {
                           char Name[ 33 ];
                            int LastCheckInDate[ 3 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
                           char MetaSrcDir[ 100 ] [ 1 ];
                           char PgmSrcDir[ 100 ] [ 1 ];
                           char ExecDir[ 100 ] [ 1 ];
                           long DB_Mode;
}

record CPLR_Enqueue /* For Entity CPLR_Enqueue */ {
                 unique key int z_TimeStamp[ 3 ];
                           long EnqueueType;
}

record CtrlDefOption /* For Entity CtrlDefOption */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           long ID;
}

record CtrlDefSubtype /* For Entity CtrlDefSubtype */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           long ID;
}

record DefinedItem /* For Entity DefinedItem */ {
                           char ExternalValue[ 65 ];
                unique key long ZKey;
                           char InternalValue[ 100 ];
                           char DataType[ 2 ];
                           long SeqNo;
}

record DeliveryEnvPkg /* For Entity DeliveryEnvPkg */ {
                unique key long ZKey;
}

record Dialog /* For Entity Dialog */ {
                           char Tag[ 9 ];
                           char DLL_Name[ 9 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
}

record Domain /* For Entity Domain */ {
                unique key long ZKey;
                           char Name[ 33 ];
                           char z_Desc[ 100 ];
                           char DomainType[ 2 ];
                           char DataType[ 2 ];
                           long MaxStringLth;
                           char ContextRestriction[ 2 ];
                           long DecimalFormat;
}

record EntpER_Model /* For Entity EntpER_Model */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
}

record ER_Attribute /* For Entity ER_Attribute */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           long Lth;
                           char z_Key[ 2 ];
                           char NotNull[ 2 ];
                           char z_Desc[ 100 ];
                           char PromptPaintLit[ 61 ];
                           long PromptPaintLth;
                           char ListPaintLit[ 61 ];
                           long ListPaintLth;
                           char CaseSens[ 2 ];
                           char ADW_Position[ 17 ];
                           char TE_ColumnName[ 33 ];
}

record ER_EntIdentifier /* For Entity ER_EntIdentifier */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char Type[ 2 ];
                           char SystemMaintained[ 2 ];
                           char z_Desc[ 100 ];
}

record ER_Entity /* For Entity ER_Entity */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char Purpose[ 13 ];
                           char z_Desc[ 100 ];
                           char OwningArea[ 33 ];
                           long ER_DiagramPosX;
                           long ER_DiagramPosY;
                           long ER_PortFactor;
                           char TE_TableName[ 33 ];
}

record ER_FactType /* For Entity ER_FactType */ {
                unique key long ZKey;
}

record ER_RelLink /* For Entity ER_RelLink */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char CardMin[ 9 ];
                           char CardMax[ 9 ];
                           long SystemMaintainedSeq;
}

record ER_RelType /* For Entity ER_RelType */ {
                unique key long ZKey;
                           char Fact[ 100 ] [ 1 ];
                           long ER_DiagramBendPosX;
                           long ER_DiagramBendPosY;
                           long ER_DiagramE1PosX;
                           long ER_DiagramE1PosY;
                           long ER_DiagramE2PosX;
                           long ER_DiagramE2PosY;
}

record Event /* For Entity Event */ {
                           long Type;
                unique key long ZKey;
}

record EventDef /* For Entity EventDef */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           long Type;
}

record ExecComposite /* For Entity ExecComposite */ {
                unique key long ZKey;
                           char Name[ 9 ];
}

record Font /* For Entity Font */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           long Id;
                           char WindowsStructure[ 100 ] [ 1 ];
                           long RGB_Color;
                           long PointSize;
                           long Invisible;
}

record FontDef /* For Entity FontDef */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           long SeqNo;
}

record FontOverride /* For Entity FontOverride */ {
                unique key long ZKey;
                           long SeqNo_DlgCtrl;
                           long SeqNo_DlgWnd;
}

record Group /* For Entity Group */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           char Text[ 100 ];
                           char Type[ 3 ];
                           char SubType[ 100 ];
                           long PSDLG_X;
                           long PSDLG_Y;
                           long SZDLG_X;
                           long SZDLG_Y;
                           long SyncKey;
                           long PPE_Type;
                           char ForceHeaderPerEntity[ 2 ];
}

record GroupSet /* For Entity GroupSet */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           char Text[ 100 ];
                           char Type[ 100 ];
                           long SubType;
                           long PSDLG_X;
                           long PSDLG_Y;
                           long SZDLG_X;
                           long SZDLG_Y;
                           long SyncKey;
                           long PPE_Type;
}

record HeaderFile /* For Entity HeaderFile */ {
                           char Name[ 9 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
                           char Extension[ 4 ];
}

record Hotkey /* For Entity Hotkey */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           char KeyCombo[ 100 ] [ 1 ];
                           char Title[ 33 ];
                           long Mode;
                           long VKey;
}

record Installation /* For Entity Installation */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char Sysadm[ 33 ];
                           char Password[ 9 ];
                           long NextWKS_Id;
                           long NextInstallZKey;
                           char z_Desc[ 100 ];
}

record LOD /* For Entity LOD */ {
                unique key long ZKey;
                           char DLL_Name[ 9 ];
                           char Name[ 9 ];
                           char z_Desc[ 100 ];
                           char ActivateConstraint[ 2 ];
                           char ActivateServerConstraint[ 2 ];
                           char ActivateEmptyConstraint[ 2 ];
                           char CommitConstraint[ 2 ];
                           char CommitServerConstraint[ 2 ];
                           char DropOIConstraint[ 2 ];
                           long CacheNumberPerEntity;
}

record LOD_Attribute /* For Entity LOD_Attribute */ {
                unique key long ZKey;
                           char Work[ 2 ];
                           char Update[ 2 ];
                           char Init[ 100 ] [ 1 ];
                           char Hidden[ 2 ];
                           long SeqPriority;
                           char SeqOrder[ 2 ];
                           char DIL_Text[ 100 ];
                           long SeqNo_LOD_Entity;
                           long SeqNo_UIS_Entity_M;
                           long SeqNo_UIS_Entity_L;
                           char DB_DerivedValue[ 2 ];
}

record LOD_Entity /* For Entity LOD_Entity */ {
                           char Name[ 33 ];
                           char IndentName[ 100 ] [ 1 ];
                           long IndentLvl;
                unique key long ZKey;
                           char Create[ 2 ];
                           char Delete[ 2 ];
                           char Update[ 2 ];
                           char Include[ 2 ];
                           char InclSrc[ 2 ];
                           char Exclude[ 2 ];
                           char Hidden[ 2 ];
                           char Recursive[ 2 ];
                           char AutoCreate[ 2 ];
                           char Derived[ 2 ];
                           char SeqType[ 2 ];
                           char Work[ 2 ];
                           char ParentDeleteBehave[ 2 ];
                           char DupEntityInstance[ 2 ];
                           char DupRelationshipInstance[ 2 ];
                           char RelLinkIndicator[ 2 ];
                           char z_Desc[ 100 ];
                           char CreateConstraint[ 2 ];
                           char DeleteConstraint[ 2 ];
                           char IncludeConstraint[ 2 ];
                           char ExcludeConstraint[ 2 ];
                           char AcceptConstraint[ 2 ];
                           char CancelConstraint[ 2 ];
                           long SeqNo;
}

record LPLR /* For Entity LPLR */ {
                           char Name[ 9 ];
                           char z_Desc[ 100 ];
                unique key long ZKey;
                           char PgmSrcDir[ 100 ] [ 1 ];
                           char ExecDir[ 100 ] [ 1 ];
                           char MetaSrcDir[ 100 ] [ 1 ];
                           long TaskUseCount;
                            int LastRefreshDate[ 3 ];
}

record MapRef /* For Entity MapRef */ {
                           char Tag[ 33 ];
                unique key long ZKey;
                           long SeqNo_DlgCtrl;
                           long SeqNo_DlgAct;
}

record MapRefDef /* For Entity MapRefDef */ {
                unique key long ZKey;
                           char z_Desc[ 100 ];
                           char Tag[ 33 ];
                           char Type[ 2 ];
                           char Iterator[ 2 ];
}

record Menu /* For Entity Menu */ {
                           char Tag[ 33 ];
                unique key long ZKey;
}

record NLS_Text /* For Entity NLS_Text */ {
                           char Text[ 100 ] [ 1 ];
                           long LanguageIndex;
                unique key long ZKey;
}

record Operation /* For Entity Operation */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
                           char Type[ 2 ];
                           char ReturnDesc[ 65 ];
                           char ReturnDataType[ 2 ];
                           long OrdinalValue;
                           long Class;
}

record Option /* For Entity Option */ {
                           char Tag[ 33 ];
                           char Text[ 33 ];
                           char AccelTitle[ 33 ];
                           char CheckMarkValueOn[ 65 ];
                           char CheckMarkValueOff[ 65 ];
                           char Separator[ 2 ];
                unique key long ZKey;
                           long Underline;
                           char DIL_Text[ 100 ];
                           long SeqNo_Menu;
                           long SeqNo_Option;
}

record OrigSironField /* For Entity OrigSironField */ {
                           long ZKey;
                           char KatalogFlag[ 2 ];
                           char Attribut[ 3 ];
                           long FieldNr;
                           long Level;
                           long Sthk;
                           long OccursNr;
                           char KeyCode[ 3 ];
                           long Dimension;
                           long Length;
                           long RelAdr;
                           long Additor;
                           char FieldName[ 34 ];
}

record Page /* For Entity Page */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           char Type[ 2 ];
                           long SizeX;
                           long SizeY;
}

record Parameter /* For Entity Parameter */ {
                           char ShortDesc[ 65 ];
                           char DataType[ 2 ];
                           char PFlag[ 2 ];
                           char UFlag[ 2 ];
                unique key long ZKey;
                           long SeqNo;
}

record PhysOverride /* For Entity PhysOverride */ {
                           long ZKey;
}

record POD /* For Entity POD */ {
                           char NAME[ 33 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
                           char DFT_DBNAME[ 33 ];
                           char DFT_SERVER[ 33 ];
                           char OPER_LIBNM[ 9 ];
                           char CR_DATE[ 19 ];
                           char UP_DATE[ 19 ];
                           char USERID[ 33 ];
                           long MULTIPLE_ROOT_LIMIT;
                           long TE_SourceZKey;
}

record POD_Attribute /* For Entity POD_Attribute */ {
                           char NAME[ 33 ];
                unique key long ZKey;
                           long SEQNR;
                           long ERATT;
                           char TYPE[ 2 ];
                           long LTH;
                           long DECIMAL;
                           char INIT[ 100 ] [ 1 ];
                           char PERSIST[ 2 ];
                           char z_KEY[ 2 ];
                           char GENKEY[ 2 ];
                           char REQUIRED[ 2 ];
                           char CASESENS[ 2 ];
                           long SEQUENCING;
                           char SEQ_AD[ 2 ];
                           char AUTO_SEQ[ 2 ];
                           char HIDDEN[ 2 ];
                           char CR_DATE[ 2 ];
                           char UP_DATE[ 2 ];
                           char USERID[ 2 ];
}

record POD_Entity /* For Entity POD_Entity */ {
                           char NAME[ 33 ];
                unique key long ZKey;
                           long SEQNR;
                           char SERVER[ 33 ];
                           char DBNAME[ 33 ];
                           char FULLPERSIST[ 2 ];
                           long ERENT_TOK;
                           long ERREL_TOK;
                           long LEVEL;
                           long CARDMIN;
                           long CARDMAX;
                           char CREATE[ 2 ];
                           char DELETE[ 2 ];
                           char INCLUDE[ 2 ];
                           char EXCLUDE[ 2 ];
                           char HIDDEN[ 2 ];
                           char RECURSIVE[ 2 ];
                           char AUTOCREATE[ 2 ];
}

record POD_EntityMapFld /* For Entity POD_EntityMapFld */ {
                unique key long ZKey;
                           long SEQNR;
                           char FLDNAME[ 33 ];
                           long FLDID;
                           char FLDTYPE[ 2 ];
                           long FLDLTH;
                           long FLDOFFSET;
}

record POD_EntityMapRec /* For Entity POD_EntityMapRec */ {
                unique key long ZKey;
                           long SEQNR;
                           char FLDNAME[ 33 ];
                           long FLDID;
                           char FLDTYPE[ 2 ];
                           long FLDLTH;
                           long FLDOFFSET;
}

record POD_RelLinkMapFld /* For Entity POD_RelLinkMapFld */ {
                unique key long ZKey;
                           long SEQNR;
                           char SRCAT_RCNM[ 33 ];
                           char SRCAT_FLNM[ 33 ];
                           char FLDNAME[ 33 ];
}

record POD_RelLinkMapRec /* For Entity POD_RelLinkMapRec */ {
                unique key long ZKey;
                           char OWNER_MEMB[ 2 ];
                           char RECNAME[ 33 ];
                           long RECID;
                           char SETNAME1[ 33 ];
                           long SETID1;
                           char SETNAME2[ 33 ];
                           long SETID2;
}

record PresEnvDef /* For Entity PresEnvDef */ {
                unique key long ZKey;
                           long CtrlFontDel;
                           char z_Desc[ 100 ];
                           char Name[ 34 ];
                           long LastCtrl;
                           long LastFont;
}

record PropertyDef /* For Entity PropertyDef */ {
                unique key long ZKey;
                           long Tag;
                           long Type;
                           long Mode;
}

record RegularExpression /* For Entity RegularExpression */ {
                unique key long ZKey;
}

record Report /* For Entity Report */ {
                unique key long ZKey;
                           char Tag[ 9 ];
                           char z_Desc[ 100 ];
                           char SironName[ 9 ];
}

record RepositoryClient /* For Entity RepositoryClient */ {
                unique key long ZKey;
                           long WKS_Id;
                           long MaxZKey;
                           long LastSyncZKey;
                           char z_Desc[ 100 ];
                           long DefaultLPLR_ZKey;
}

record SA_Entity /* For Entity SA_Entity */ {
                unique key long ZKey;
                           long ER_PortFactor;
                           long ER_DiagramPosX;
                           long ER_DiagramPosY;
}

record SA_RelType /* For Entity SA_RelType */ {
                unique key long ZKey;
                           long ER_DiagramBendPosX;
                           long ER_DiagramBendPosY;
                           long ER_DiagramE1PosX;
                           long ER_DiagramE1PosY;
                           long ER_DiagramE2PosX;
                           long ER_DiagramE2PosY;
}

record SironComment /* For Entity SironComment */ {
                           long ZKey;
                           char Text[ 100 ];
                           long Number;
                           long Length;
}

record SironKatalog /* For Entity SironKatalog */ {
                           long ZKey;
                           char Name[ 9 ];
                           char OrgForm[ 17 ];
                           char KeyAttribute[ 2 ];
                           long RecLen;
                           char RecForm[ 3 ];
                           char DDName[ 9 ];
                           char Password[ 9 ];
                           char UproName[ 9 ];
                           long BlkLen;
                           long KeyLen;
                           long KeyPos;
}

record SourceFile /* For Entity SourceFile */ {
                unique key long ZKey;
                           char Name[ 9 ];
                           char z_Desc[ 100 ];
                           char Extension[ 4 ];
                           char CM_Managed[ 2 ];
                           char OwnerType[ 2 ];
                           char LanguageType[ 2 ];
                           char Subdirectory[ 33 ];
                           char DomainAndGlobalOpGroupName[ 9 ];
}

record SubjectArea /* For Entity SubjectArea */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
                           char UpdateAuth[ 2 ];
}

record TableEntry /* For Entity TableEntry */ {
                unique key long ZKey;
                           char ExternalValue[ 100 ] [ 1 ];
                           char InternalValue[ 100 ] [ 1 ];
}

record TE_DB_Environ /* For Entity TE_DB_Environ */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char Paradigm[ 2 ];
                           char z_Desc[ 100 ];
                           char Type[ 2 ];
}

record TE_DBMS_Source /* For Entity TE_DBMS_Source */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           long MaxFixedAttrLth;
                           long OverflowAttrLth;
                           char z_Desc[ 100 ];
                           char Paradigm[ 2 ];
                           char DBMS[ 31 ];
                           char GenerateExecutable[ 9 ];
                           char Executable[ 9 ];
                           char GenKeyExecutable[ 33 ];
                           char ConectionType[ 2 ];
                           char SironDB_Type[ 2 ];
                           char SironDB_Subtype[ 2 ];
                           char DefaultOwner[ 33 ];
                           char DisplayOwner[ 2 ];
                           char DisplayDBMS_ID[ 2 ];
                           char Network[ 9 ];
}

record TE_FieldDataRel /* For Entity TE_FieldDataRel */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char z_Desc[ 100 ];
                           char DataOrRelfieldOrSet[ 2 ];
                           char z_Key[ 2 ];
                           long DBMS_ID;
                           char DataType[ 9 ];
                           long Length;
                           long Offset;
                           long SQL_SCALE;
                           char SQL_NULLS[ 2 ];
                           char SQL_LABEL[ 31 ];
                           char SQL_REMARKS[ 100 ] [ 1 ];
}

record TE_PhysicalDataType /* For Entity TE_PhysicalDataType */ {
                unique key long ZKey;
}

record TE_TablRecDataRel /* For Entity TE_TablRecDataRel */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char DataOrRel[ 2 ];
                           long DBMS_ID;
                           char SQL_TableOwner[ 33 ];
                           char SQL_TableQual[ 33 ];
                           char z_Desc[ 100 ];
}

record TE_TablRecKey /* For Entity TE_TablRecKey */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char Category[ 2 ];
                           char z_Desc[ 100 ];
}

record UI_Spec /* For Entity UI_Spec */ {
                unique key long ZKey;
                           char Name[ 9 ];
                           char ActionType[ 2 ];
                           char ListSelect[ 2 ];
                           char ListAllText[ 51 ];
                           char ListQualifyText[ 51 ];
                           char WDOD_Style[ 2 ];
                           char ListType[ 2 ];
                           char TransferObjectName[ 33 ];
                           char TransferQualifier[ 100 ] [ 1 ];
                           char ExitText[ 51 ];
                           char LoadText[ 51 ];
                           char ActionBarText[ 51 ];
                           char SelectQualifier[ 100 ] [ 1 ];
                           char ExitMenuText[ 51 ];
                           char SelectText[ 51 ];
                           char SelectMenuText[ 51 ];
                           char ListMenuText[ 51 ];
                           char StyleObjectType[ 2 ];
                           char z_Desc[ 100 ];
}

record UIS_Entity /* For Entity UIS_Entity */ {
                unique key long ZKey;
                           char EliminateWnd[ 2 ];
                           long SeqNo;
                           char ListTypeFlag[ 2 ];
                           char ReportOrientationFlag[ 100 ];
                           char ReportGroupBorder[ 2 ];
                           long VerticalControlSize;
                           long HorizontalControlSize;
                           long VerticalSpacingIncrement;
                           char AutodesignCaseType[ 3 ];
                           char FirstDataFieldName[ 11 ];
                           char RootOnlyList[ 2 ];
                           char HeaderText[ 100 ];
                           char FooterText[ 100 ];
                           char ForceHeaderPerEntity[ 2 ];
                           long FontSize;
                           char BoldFlag[ 2 ];
                           char ItalicFlag[ 2 ];
                           long HeaderFontSize;
                           char HeaderBoldFlag[ 2 ];
                           char HeaderItalicFlag[ 2 ];
                           long FooterFontSize;
                           char FooterBoldFlag[ 2 ];
                           char FooterItalicFlag[ 2 ];
}

record UIS_Include /* For Entity UIS_Include */ {
                unique key long ZKey;
                           char SubObjectType[ 2 ];
                           char SubObjectName[ 33 ];
                           char SubObjectListQual[ 100 ] [ 1 ];
                           char SubObjectTransQual[ 100 ] [ 1 ];
                           char SubObjectOperators[ 3 ];
}

record UIS_QualList /* For Entity UIS_QualList */ {
                unique key long ZKey;
                           long QualAttribZKey;
                           char QualifierAttrib[ 33 ];
                           char Join[ 4 ];
                           char Operator[ 3 ];
                           long SeqNo;
}

record UIS_Tab /* For Entity UIS_Tab */ {
                           long ZKey;
                           char Title[ 51 ];
}

record User /* For Entity User */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           char Password[ 9 ];
}

record UserMetaStatus /* For Entity UserMetaStatus */ {
                unique key long ZKey;
                           long Status;
                            int CheckOutDate[ 3 ];
                           char Comments[ 100 ] [ 1 ];
}

record ViewObjRef /* For Entity ViewObjRef */ {
                unique key long ZKey;
                           char Name[ 33 ];
                           char z_Desc[ 100 ];
                           long Level;
}

record Window /* For Entity Window */ {
                           char Tag[ 33 ];
                           long Subtype;
                           char Caption[ 65 ];
                           long PSDLG_X;
                           long PSDLG_Y;
                           long SZDLG_X;
                           long SZDLG_Y;
                           char DfltButton[ 33 ];
                unique key long ZKey;
                           long ResourceID;
                           char DIL_Active[ 2 ];
                           char ProfileView[ 33 ];
                           long GeneralFlag;
}

record WindowStyle /* For Entity WindowStyle */ {
                unique key long ZKey;
                           char Tag[ 33 ];
                           char z_Desc[ 100 ];
                           long Style;
                           long StyleEx;
}

record WndDesignSpec /* For Entity WndDesignSpec */ {
                unique key long ZKey;
                           char EntityFromSubObj[ 2 ];
                           char AutoVersion[ 2 ];
                           char ActionType[ 2 ];
                           char UsageStyle[ 2 ];
                           char Cascade[ 2 ];
                           char UpdateCancelAction[ 2 ];
                           char UpdateDeleteAction[ 2 ];
                           char UpdateSaveAction[ 2 ];
                           char UpdateNextAction[ 2 ];
                           char UpdatePrevAction[ 2 ];
                           char UpdateCreateAction[ 2 ];
                           char UpdateSaveText[ 51 ];
                           char UpdateCancelText[ 51 ];
                           char UpdateDeleteText[ 51 ];
                           char UpdateNextText[ 51 ];
                           char UpdatePrevText[ 51 ];
                           char UpdateSaveAccel[ 2 ];
                           char UpdateCancelAccel[ 2 ];
                           char UpdateDeleteAccel[ 2 ];
                           char UpdateNextAccel[ 2 ];
                           char UpdatePrevAccel[ 2 ];
                           char ListOptionPosition[ 2 ];
                           char ListUpdateAction[ 51 ];
                           char ListActionMenuText[ 51 ];
                           char ListActionAccel[ 2 ];
                           char ListDeleteAction[ 2 ];
                           char ListCreateAction[ 2 ];
                           char CreateFromNewInd[ 2 ];
                           char ListDeleteText[ 51 ];
                           char ListCreateText[ 51 ];
                           char ListUpdateText[ 51 ];
                           char ListNewIndText[ 51 ];
                           char ListDeleteAccel[ 2 ];
                           char ListCreateAccel[ 2 ];
                           char ListUpdateAccel[ 2 ];
                           char ListTriggerMenuTxt[ 51 ];
                           char ListTriggerText[ 51 ];
                           char ListGroupboxText[ 51 ];
                           char UpdtActionMenuTxt[ 51 ];
                           char WndStyle[ 33 ];
                           char WndName[ 9 ];
                           char WndCaption[ 51 ];
                           char WndCaption2[ 51 ];
                           char WndNoMap[ 2 ];
                           char WndRefresh[ 2 ];
                           long WndSize_Y;
                           long WndSize_X;
                           long WndStart_Y;
                           long WndStart_X;
                           char WndDIL_Message[ 2 ];
                           char FullWnd[ 2 ];
}

record WndEvent /* For Entity WndEvent */ {
                           long Type;
                           char Accelerator[ 33 ];
                unique key long ZKey;
}

record Z_MetaDef /* For Entity Z_MetaDef */ {
                           char Name[ 33 ];
                unique key long ZKey;
                           long Type;
                            int LastUpdateDate[ 3 ];
                            int LastCheckInDate[ 3 ];
                           char z_Desc[ 100 ];
}

record Mm_UIS_Tab_for_LOD_Attribute /* For Entity M-M correspondence table */ {
}

record Mm_TE_DB_Envrn_ptntlycsTEDBMSSr /* For Entity M-M correspondence table */ {
}

record Mm_TE_TblRcDtRl_sdn_TE_DB_Envrn /* For Entity M-M correspondence table */ {
}

record Mm_ColorOverride_has_Color /* For Entity M-M correspondence table */ {
}

record Mm_ViewObjRef_for_Report /* For Entity M-M correspondence table */ {
}

record Mm_Aplctn_sxctblby_RpstryClnt /* For Entity M-M correspondence table */ {
}

record Mm_UI_Spec_has_ViewObjRef /* For Entity M-M correspondence table */ {
}

record Mm_CntrlDf_hsmsg_Oprtn /* For Entity M-M correspondence table */ {
}

record Mm_TE_TblRcKy_s_TE_FldDtRl /* For Entity M-M correspondence table */ {
}

record Mm_CntrlDf_4cptschngfrm_CntrlDf /* For Entity M-M correspondence table */ {
}

record Mm_CntrlDf_1cptsprnt_CntrlDf /* For Entity M-M correspondence table */ {
}

record Mm_CntrlDf_2cptschld_CntrlDf /* For Entity M-M correspondence table */ {
}

record Mm_SA_Enty_snvlvdn_SbjctAr /* For Entity M-M correspondence table */ {
}

record Mm_TE_TblRcDtRl_mplmnts_ER_Enty /* For Entity M-M correspondence table */ {
}

record Mm_TE_PhysclDtTyp_cnmplmnt_Dmn /* For Entity M-M correspondence table */ {
}

record Mm_SA_RlTyp_snvlvdn_SbjctAr /* For Entity M-M correspondence table */ {
}

record Mm_UIS_Enty_hsmnt_LOD_Atrbt /* For Entity M-M correspondence table */ {
}

record Mm_UIS_Enty_hslst_LOD_Atrbt /* For Entity M-M correspondence table */ {
}

record Mm_SourceFile_ownedby_LOD /* For Entity M-M correspondence table */ {
}

record Mm_SourceFile_ownedby_Dialog /* For Entity M-M correspondence table */ {
}

record Mm_TEPhysclDtTypsvlblfrTEDBMSSr /* For Entity M-M correspondence table */ {
}

record Mm_User_belongto_Installation /* For Entity M-M correspondence table */ {
}

record Mm_RepositoryClient_has_User /* For Entity M-M correspondence table */ {
}

record Mm_CPLR_isreusedby_CPLR /* For Entity M-M correspondence table */ {
}

record Mm_Dialog_has_ViewObjRef /* For Entity M-M correspondence table */ {
}

record Mm_ExcCmpst_sgrpngf_Z_MtDf /* For Entity M-M correspondence table */ {
}

record Mm_Dlg_cntnsptn_CPL_RsblItm /* For Entity M-M correspondence table */ {
}

record Mm_Dlg_cntnswndw_CPL_RsblItm /* For Entity M-M correspondence table */ {
}

record Mm_Dlg_cntnsgrp_CPL_RsblItm /* For Entity M-M correspondence table */ {
}

record Mm_Menu_for_Window /* For Entity M-M correspondence table */ {
}

record Mm_Action_activates_Window /* For Entity M-M correspondence table */ {
}

set CPLR_has_AccessAuthority {
           order  last;
           owner  CPLR;
           member AccessAuthority;
}

set User_has_AccessAuthority {
           order  last;
           owner  User;
           member AccessAuthority;
}

set Window_has_Action {
           order  last;
           owner  Window;
           member Action;
}

set Operation_istriggeredby_Action {
           order  last;
           owner  Operation;
           member Action;
}

set PresEnvDef_has_Color {
           order  last;
           owner  PresEnvDef;
           member Color;
}

set ControlDef_uses_ColorDef {
           order  last;
           owner  ControlDef;
           member ColorDef;
}

set WindowStyle_uses_ColorDef {
           order  last;
           owner  WindowStyle;
           member ColorDef;
}

set Color_isusedby_ColorDef {
           order  last;
           owner  Color;
           member ColorDef;
}

set Window_has_ColorOverride {
           order  last;
           owner  Window;
           member ColorOverride;
}

set ColorDef_isunder_ColorOverride {
           order  last;
           owner  ColorDef;
           member ColorOverride;
}

set Control_has_ColorOverride {
           order  last;
           owner  Control;
           member ColorOverride;
}

set Domain_has_Context {
           order  last;
           owner  Domain;
           member Context;
}

set Control_hassub_Control {
           order  last;
           owner  Control;
           member Control;
}

set Window_has_Control {
           order  last;
           owner  Window;
           member Control;
}

set Menu_for_Control {
           order  last;
           owner  Menu;
           member Control;
}

set Control_islinkedto_Control {
           order  last;
           owner  Control;
           member Control;
}

set ControlDef_isusedby_Control {
           order  last;
           owner  ControlDef;
           member Control;
}

set PhysOverride_isfor_Control {
           order  last;
           owner  PhysOverride;
           member Control;
}

set Group_has_Control {
           order  last;
           owner  Group;
           member Control;
}

set CntrlDf_3cnvrtsfrm_CntrlDf {
           order  last;
           owner  ControlDef;
           member ControlDef;
}

set PresEnvDef_has_ControlDef {
           order  last;
           owner  PresEnvDef;
           member ControlDef;
}

set PresEnvDef_has_ControlStandard {
           order  last;
           owner  PresEnvDef;
           member ControlStandard;
}

set Installation_contains_CPL {
           order  last;
           owner  Installation;
           member CPL;
}

set User_mayadminister_CPL {
           order  last;
           owner  User;
           member CPL;
}

set Application_hasroot_CPLR {
           order  last;
           owner  Application;
           member CPLR;
}

set CPL_has_CPLR {
           order  last;
           owner  CPL;
           member CPLR;
}

set RpstryClnt_hsnqfr_CPLR_Enq {
           order  last;
           owner  RepositoryClient;
           member CPLR_Enqueue;
}

set CPLR_mayhave_CPLR_Enqueue {
           order  last;
           owner  CPLR;
           member CPLR_Enqueue;
}

set CtrlDfSbtyp_hs_CtrlDfOptn {
           order  last;
           owner  CtrlDefSubtype;
           member CtrlDefOption;
}

set ControlDef_has_CtrlDefOption {
           order  last;
           owner  ControlDef;
           member CtrlDefOption;
}

set ControlDef_has_CtrlDefSubtype {
           order  last;
           owner  ControlDef;
           member CtrlDefSubtype;
}

set HeaderFile_has_DefinedItem {
           order  last;
           owner  HeaderFile;
           member DefinedItem;
}

set Application_has_DeliveryEnvPkg {
           order  last;
           owner  Application;
           member DeliveryEnvPkg;
}

set Window_for_Dialog {
           order  last;
           owner  Window;
           member Dialog;
}

set Z_MetaDef_represents_Dialog {
           order  last;
           owner  Z_MetaDef;
           member Dialog;
}

set SourceFile_ownedby_Domain {
           order  last;
           owner  SourceFile;
           member Domain;
}

set Z_MetaDef_represents_Domain {
           order  last;
           owner  Z_MetaDef;
           member Domain;
}

set ADW_InfoType_has_Domain {
           order  last;
           owner  ADW_InfoType;
           member Domain;
}

set Operation_isfor_Domain {
           order  last;
           owner  Operation;
           member Domain;
}

set Z_MtDf_rprsnts_EntpER_Mdl {
           order  last;
           owner  Z_MetaDef;
           member EntpER_Model;
}

set Domain_constrains_ER_Attribute {
           order  last;
           owner  Domain;
           member ER_Attribute;
}

set ER_Entity_hasfact_ER_Attribute {
           order  last;
           owner  ER_Entity;
           member ER_Attribute;
}

set ER_Entity_has_ER_EntIdentifier {
           order  last;
           owner  ER_Entity;
           member ER_EntIdentifier;
}

set SubjectArea_owns_ER_Entity {
           order  last;
           owner  SubjectArea;
           member ER_Entity;
}

set EntpER_Model_has_ER_Entity {
           order  last;
           owner  EntpER_Model;
           member ER_Entity;
}

set ER_RelLink_isa_ER_FactType {
           order  last;
           owner  ER_RelLink;
           member ER_FactType;
}

set ER_Attribute_isa_ER_FactType {
           order  last;
           owner  ER_Attribute;
           member ER_FactType;
}

set ER_EntIdntfr_hs_ER_FctTyp {
           order  last;
           owner  ER_EntIdentifier;
           member ER_FactType;
}

set ER_RelType_has_ER_RelLink {
           order  last;
           owner  ER_RelType;
           member ER_RelLink;
}

set ER_Entity_hasfact_ER_RelLink {
           order  last;
           owner  ER_Entity;
           member ER_RelLink;
}

set EntpER_Model_has_ER_RelType {
           order  last;
           owner  EntpER_Model;
           member ER_RelType;
}

set SubjectArea_owns_ER_RelType {
           order  last;
           owner  SubjectArea;
           member ER_RelType;
}

set Action_istriggeredby_Event {
           order  last;
           owner  Action;
           member Event;
}

set Control_hasactive_Event {
           order  last;
           owner  Control;
           member Event;
}

set ControlDef_has_EventDef {
           order  last;
           owner  ControlDef;
           member EventDef;
}

set WindowStyle_has_EventDef {
           order  last;
           owner  WindowStyle;
           member EventDef;
}

set DlvryEnvPkg_dfnscmpstnf_ExcCmps {
           order  last;
           owner  DeliveryEnvPkg;
           member ExecComposite;
}

set PresEnvDef_has_Font {
           order  last;
           owner  PresEnvDef;
           member Font;
}

set WindowStyle_uses_FontDef {
           order  last;
           owner  WindowStyle;
           member FontDef;
}

set Font_isusedby_FontDef {
           order  last;
           owner  Font;
           member FontDef;
}

set ControlDef_uses_FontDef {
           order  last;
           owner  ControlDef;
           member FontDef;
}

set Window_has_FontOverride {
           order  last;
           owner  Window;
           member FontOverride;
}

set Font_isunder_FontOverride {
           order  last;
           owner  Font;
           member FontOverride;
}

set FontDef_isunder_FontOverride {
           order  last;
           owner  FontDef;
           member FontOverride;
}

set Control_has_FontOverride {
           order  last;
           owner  Control;
           member FontOverride;
}

set GroupSet_has_Group {
           order  last;
           owner  GroupSet;
           member Group;
}

set LOD_Entity_triggers_GroupSet {
           order  last;
           owner  LOD_Entity;
           member GroupSet;
}

set Page_has_GroupSet {
           order  last;
           owner  Page;
           member GroupSet;
}

set Z_MetaDef_represents_HeaderFile {
           order  last;
           owner  Z_MetaDef;
           member HeaderFile;
}

set Action_istriggeredby_Hotkey {
           order  last;
           owner  Action;
           member Hotkey;
}

set Window_has_Hotkey {
           order  last;
           owner  Window;
           member Hotkey;
}

set Z_MetaDef_represents_LOD {
           order  last;
           owner  Z_MetaDef;
           member LOD;
}

set LOD_Entity_rootof_LOD {
           order  last;
           owner  LOD_Entity;
           member LOD;
}

set Oprtn_mplmntcnstrntn_LOD {
           order  last;
           owner  Operation;
           member LOD;
}

set LOD_Enty_hsfct_LOD_Atrbt {
           order  last;
           owner  LOD_Entity;
           member LOD_Attribute;
}

set ER_Atrbt_scsdby_LOD_Atrbt {
           order  last;
           owner  ER_Attribute;
           member LOD_Attribute;
}

set Oprtn_mplmntsdrvtnf_LOD_Atrbt {
           order  last;
           owner  Operation;
           member LOD_Attribute;
}

set UIS_Enty_hsncldlst_LOD_Atrbt {
           order  last;
           owner  UIS_Entity;
           member LOD_Attribute;
}

set ER_Atrbt_hsDB_DrvdVl_LOD_Atrbt {
           order  last;
           owner  ER_Attribute;
           member LOD_Attribute;
}

set LOD_Enty_sprntf_LOD_Enty {
           order  last;
           owner  LOD_Entity;
           member LOD_Entity;
}

set ER_RlLnk_dfnstrctrfr_LOD_Enty {
           order  last;
           owner  ER_RelLink;
           member LOD_Entity;
}

set ER_Enty_scsdby_LOD_Enty {
           order  last;
           owner  ER_Entity;
           member LOD_Entity;
}

set Oprtn_mplmntscnstrntn_LOD_Enty {
           order  last;
           owner  Operation;
           member LOD_Entity;
}

set RepositoryClient_contains_LPLR {
           order  last;
           owner  RepositoryClient;
           member LPLR;
}

set CPLR_hascorresponding_LPLR {
           order  last;
           owner  CPLR;
           member LPLR;
}

set ViewObjRef_referencedby_MapRef {
           order  last;
           owner  ViewObjRef;
           member MapRef;
}

set Control_references_MapRef {
           order  last;
           owner  Control;
           member MapRef;
}

set Actn_hsbjctbhvrfr_MpRf {
           order  last;
           owner  Action;
           member MapRef;
}

set Context_isoverridefor_MapRef {
           order  last;
           owner  Context;
           member MapRef;
}

set LOD_Entity_for_MapRef {
           order  last;
           owner  LOD_Entity;
           member MapRef;
}

set LOD_Attribute_for_MapRef {
           order  last;
           owner  LOD_Attribute;
           member MapRef;
}

set Window_hascaptiontitle_MapRef {
           order  last;
           owner  Window;
           member MapRef;
}

set CtrlDefSubtype_has_MapRefDef {
           order  last;
           owner  CtrlDefSubtype;
           member MapRefDef;
}

set Window_hasdefaultactionbar_Menu {
           order  last;
           owner  Window;
           member Menu;
}

set Window_has_NLS_Text {
           order  last;
           owner  Window;
           member NLS_Text;
}

set TableEntry_has_NLS_Text {
           order  last;
           owner  TableEntry;
           member NLS_Text;
}

set Option_hasText_NLS_Text {
           order  last;
           owner  Option;
           member NLS_Text;
}

set LOD_Attribute_has_NLS_Text {
           order  last;
           owner  LOD_Attribute;
           member NLS_Text;
}

set Control_hasText_NLS_Text {
           order  last;
           owner  Control;
           member NLS_Text;
}

set Context_has_NLS_Text {
           order  last;
           owner  Context;
           member NLS_Text;
}

set Control_hasDIL_Text_NLS_Text {
           order  last;
           owner  Control;
           member NLS_Text;
}

set Option_hasDIL_Text_NLS_Text {
           order  last;
           owner  Option;
           member NLS_Text;
}

set SourceFile_contains_Operation {
           order  last;
           owner  SourceFile;
           member Operation;
}

set HdrFl_hsprtypfr_Oprtn {
           order  last;
           owner  HeaderFile;
           member Operation;
}

set Z_MtDf_rprsntsglbl_Oprtn {
           order  last;
           owner  Z_MetaDef;
           member Operation;
}

set SrcFl_swndbyglbl_Oprtn {
           order  last;
           owner  SourceFile;
           member Operation;
}

set Action_istriggeredby_Option {
           order  last;
           owner  Action;
           member Option;
}

set MapRef_for_Option {
           order  last;
           owner  MapRef;
           member Option;
}

set Option_has_Option {
           order  last;
           owner  Option;
           member Option;
}

set Menu_has_Option {
           order  last;
           owner  Menu;
           member Option;
}

set SironKatalog_has_OrigSironField {
           order  last;
           owner  SironKatalog;
           member OrigSironField;
}

set OrgSrnFld_hsb_OrgSrnFld {
           order  last;
           owner  OrigSironField;
           member OrigSironField;
}

set Report_has_Page {
           order  last;
           owner  Report;
           member Page;
}

set Operation_has_Parameter {
           order  last;
           owner  Operation;
           member Parameter;
}

set Font_usedby_PhysOverride {
           order  last;
           owner  Font;
           member PhysOverride;
}

set Z_MetaDef_represents_POD {
           order  last;
           owner  Z_MetaDef;
           member POD;
}

set POD_Entity_rootof_POD {
           order  last;
           owner  POD_Entity;
           member POD;
}

set LOD_isbasefor_POD {
           order  last;
           owner  LOD;
           member POD;
}

set TE_DB_Environ_usedby_POD {
           order  last;
           owner  TE_DB_Environ;
           member POD;
}

set POD_Enty_hsfct_POD_Atrbt {
           order  last;
           owner  POD_Entity;
           member POD_Attribute;
}

set LOD_Atrbt_smplmntdby_POD_Atrbt {
           order  last;
           owner  LOD_Attribute;
           member POD_Attribute;
}

set POD_Enty_sprntf_POD_Enty {
           order  last;
           owner  POD_Entity;
           member POD_Entity;
}

set LOD_Enty_smplmntdby_POD_Enty {
           order  last;
           owner  LOD_Entity;
           member POD_Entity;
}

set POD_EntyMpRc_hs_POD_EntyMpFld {
           order  last;
           owner  POD_EntityMapRec;
           member POD_EntityMapFld;
}

set POD_Atrbt_srcfr_POD_EntyMpFld {
           order  last;
           owner  POD_Attribute;
           member POD_EntityMapFld;
}

set TE_TblRcDtRl_scsdthr_PODEntyMpR {
           order  last;
           owner  TE_TablRecDataRel;
           member POD_EntityMapRec;
}

set POD_Entity_has_POD_EntityMapRec {
           order  last;
           owner  POD_Entity;
           member POD_EntityMapRec;
}

set POD_RlLnkMpRc_hs_POD_RlLnkMpFld {
           order  last;
           owner  POD_RelLinkMapRec;
           member POD_RelLinkMapFld;
}

set POD_AtrbtsmplmntdbyPODRlLnkMpFl {
           order  last;
           owner  POD_Attribute;
           member POD_RelLinkMapFld;
}

set TE_TblRcDtRl_scsdthrPODRlLnkMpR {
           order  last;
           owner  TE_TablRecDataRel;
           member POD_RelLinkMapRec;
}

set POD_Enty_hschldnly_POD_RlLnkMpR {
           order  last;
           owner  POD_Entity;
           member POD_RelLinkMapRec;
}

set POD_EntyMpRc_hs_POD_RlLnkMpRc {
           order  last;
           owner  POD_EntityMapRec;
           member POD_RelLinkMapRec;
}

set Z_MetaDef_represents_PresEnvDef {
           order  last;
           owner  Z_MetaDef;
           member PresEnvDef;
}

set ControlDef_has_PropertyDef {
           order  last;
           owner  ControlDef;
           member PropertyDef;
}

set Cntxt_hsdfng_RglrExprsn {
           order  last;
           owner  Context;
           member RegularExpression;
}

set Z_MetaDef_isa_Report {
           order  last;
           owner  Z_MetaDef;
           member Report;
}

set Instltn_hstlt_RpstryClnt {
           order  last;
           owner  Installation;
           member RepositoryClient;
}

set ER_Enty_spstndby_SA_Enty {
           order  last;
           owner  ER_Entity;
           member SA_Entity;
}

set ER_RlTyp_spstndby_SA_RlTyp {
           order  last;
           owner  ER_RelType;
           member SA_RelType;
}

set OrigSironField_has_SironComment {
           order  last;
           owner  OrigSironField;
           member SironComment;
}

set SironKatalog_has_SironComment {
           order  last;
           owner  SironKatalog;
           member SironComment;
}

set TE_TblRcDtRl_hs_SrnKtlg {
           order  last;
           owner  TE_TablRecDataRel;
           member SironKatalog;
}

set PresEnvDef_owns_SourceFile {
           order  last;
           owner  PresEnvDef;
           member SourceFile;
}

set ExcCmpst_smdpf_SrcFl {
           order  last;
           owner  ExecComposite;
           member SourceFile;
}

set Z_MtDf_rprsnts_SbjctAr {
           order  last;
           owner  Z_MetaDef;
           member SubjectArea;
}

set EntpER_Model_has_SubjectArea {
           order  last;
           owner  EntpER_Model;
           member SubjectArea;
}

set Context_hasdefining_TableEntry {
           order  last;
           owner  Context;
           member TableEntry;
}

set Z_MtDf_rprsnts_TE_DB_Envrn {
           order  last;
           owner  Z_MetaDef;
           member TE_DB_Environ;
}

set SbjctAr_smplntdthr_TE_DB_Envrn {
           order  last;
           owner  SubjectArea;
           member TE_DB_Environ;
}

set TE_DB_Envrn_cntnsdfntnfTEDBMSSr {
           order  last;
           owner  TE_DB_Environ;
           member TE_DBMS_Source;
}

set TE_PhysclDtTyp_sfr_TE_FldDtRl {
           order  last;
           owner  TE_PhysicalDataType;
           member TE_FieldDataRel;
}

set PODRlLnkMpFldsmplmntdbyTEFldDtR {
           order  last;
           owner  POD_RelLinkMapFld;
           member TE_FieldDataRel;
}

set TE_TblRcDtRl_hs_TE_FldDtRl {
           order  last;
           owner  TE_TablRecDataRel;
           member TE_FieldDataRel;
}

set POD_EntyMpFldsmplmntdbyTEFldDtR {
           order  last;
           owner  POD_EntityMapFld;
           member TE_FieldDataRel;
}

set ER_RlLnk_smplmntdby_TE_FldDtRl {
           order  last;
           owner  ER_RelLink;
           member TE_FieldDataRel;
}

set ER_Atrbt_mplmntdby_TE_FldDtRl {
           order  last;
           owner  ER_Attribute;
           member TE_FieldDataRel;
}

set TE_FldDtRl_hsqvlntFK_TE_FldDtRl {
           order  last;
           owner  TE_FieldDataRel;
           member TE_FieldDataRel;
}

set OrgSrnFld_fr_TE_FldDtRl {
           order  last;
           owner  OrigSironField;
           member TE_FieldDataRel;
}

set TE_DBMS_Src_hs_TE_TblRcDtRl {
           order  last;
           owner  TE_DBMS_Source;
           member TE_TablRecDataRel;
}

set ER_RlTyp_smplmntdby_TE_TblRcDtR {
           order  last;
           owner  ER_RelType;
           member TE_TablRecDataRel;
}

set OrgSrnFld_fr_TE_TblRcDtRl {
           order  last;
           owner  OrigSironField;
           member TE_TablRecDataRel;
}

set ER_EntIdntfr_hsphyscl_TE_TblRcK {
           order  last;
           owner  ER_EntIdentifier;
           member TE_TablRecKey;
}

set TE_TblRcDtRl_hs_TE_TblRcKy {
           order  last;
           owner  TE_TablRecDataRel;
           member TE_TablRecKey;
}

set UIS_Entity_isroot_UI_Spec {
           order  last;
           owner  UIS_Entity;
           member UI_Spec;
}

set WndDsgnSpc_sdfltfr_UI_Spc {
           order  last;
           owner  WndDesignSpec;
           member UI_Spec;
}

set Z_MetaDef_represents_UI_Spec {
           order  last;
           owner  Z_MetaDef;
           member UI_Spec;
}

set UIS_Entity_haschild_UIS_Entity {
           order  last;
           owner  UIS_Entity;
           member UIS_Entity;
}

set LOD_Entity_for_UIS_Entity {
           order  last;
           owner  LOD_Entity;
           member UIS_Entity;
}

set UIS_Enty_hsdfnd_UIS_Incld {
           order  last;
           owner  UIS_Entity;
           member UIS_Include;
}

set VwObjRf_slstfr_UIS_Incld {
           order  last;
           owner  ViewObjRef;
           member UIS_Include;
}

set UI_Spec_has_UIS_QualList {
           order  last;
           owner  UI_Spec;
           member UIS_QualList;
}

set UIS_Entity_has_UIS_Tab {
           order  last;
           owner  UIS_Entity;
           member UIS_Tab;
}

set Z_MetaDef_has_UserMetaStatus {
           order  last;
           owner  Z_MetaDef;
           member UserMetaStatus;
}

set User_has_UserMetaStatus {
           order  last;
           owner  User;
           member UserMetaStatus;
}

set RpstryClnt_cntns_UsrMtSts {
           order  last;
           owner  RepositoryClient;
           member UserMetaStatus;
}

set LPLR_hschckdtmtsfrm_UsrMtSts {
           order  last;
           owner  LPLR;
           member UserMetaStatus;
}

set LOD_for_ViewObjRef {
           order  last;
           owner  LOD;
           member ViewObjRef;
}

set Z_MetaDef_represents_ViewObjRef {
           order  last;
           owner  Z_MetaDef;
           member ViewObjRef;
}

set UI_Spc_hslstbxfr_VwObjRf {
           order  last;
           owner  UI_Spec;
           member ViewObjRef;
}

set UIS_Incld_hsncld_VwObjRf {
           order  last;
           owner  UIS_Include;
           member ViewObjRef;
}

set Dialog_has_Window {
           order  last;
           owner  Dialog;
           member Window;
}

set WindowStyle_defines_Window {
           order  last;
           owner  WindowStyle;
           member Window;
}

set PhysOverride_isfor_Window {
           order  last;
           owner  PhysOverride;
           member Window;
}

set PresEnvDef_has_WindowStyle {
           order  last;
           owner  PresEnvDef;
           member WindowStyle;
}

set UIS_Enty_hsdfnd_WndDsgnSpc {
           order  last;
           owner  UIS_Entity;
           member WndDesignSpec;
}

set WindowStyle_isin_WndDesignSpec {
           order  last;
           owner  WindowStyle;
           member WndDesignSpec;
}

set Action_by_WndEvent {
           order  last;
           owner  Action;
           member WndEvent;
}

set Window_has_WndEvent {
           order  last;
           owner  Window;
           member WndEvent;
}

set CPLR_contains_Z_MetaDef {
           order  last;
           owner  CPLR;
           member Z_MetaDef;
}

set SrcFl_srprsntdby_Z_MtDf {
           order  last;
           owner  SourceFile;
           member Z_MetaDef;
}

set LOD_Atrbt_prsn_MmUISTbfrLODAtrb {
           order  last;
           owner  LOD_Attribute;
           member Mm_UIS_Tab_for_LOD_Attribute;
}

set UIS_Tb_fr_Mm_UIS_Tb_fr_LOD_Atrb {
           order  last;
           owner  UIS_Tab;
           member Mm_UIS_Tab_for_LOD_Attribute;
}

set TEDBMSSrcscsblnMmTEDBEnvrnptntl {
           order  last;
           owner  TE_DBMS_Source;
           member Mm_TE_DB_Envrn_ptntlycsTEDBMSSr;
}

set TEDBEnvrnptntlycsMmTEDBEnvrnptn {
           order  last;
           owner  TE_DB_Environ;
           member Mm_TE_DB_Envrn_ptntlycsTEDBMSSr;
}

set TEDBEnvrnhscstMmTETblRcDtRlsdnT {
           order  last;
           owner  TE_DB_Environ;
           member Mm_TE_TblRcDtRl_sdn_TE_DB_Envrn;
}

set TETblRcDtRlsdnMmTETblRcDtRlsdnT {
           order  last;
           owner  TE_TablRecDataRel;
           member Mm_TE_TblRcDtRl_sdn_TE_DB_Envrn;
}

set Clr_sndr_Mm_ClrOvrd_hs_Clr {
           order  last;
           owner  Color;
           member Mm_ColorOverride_has_Color;
}

set ClrOvrd_hs_Mm_ClrOvrd_hs_Clr {
           order  last;
           owner  ColorOverride;
           member Mm_ColorOverride_has_Color;
}

set Rprt_hs_Mm_VwObjRf_fr_Rprt {
           order  last;
           owner  Report;
           member Mm_ViewObjRef_for_Report;
}

set VwObjRf_fr_Mm_VwObjRf_fr_Rprt {
           order  last;
           owner  ViewObjRef;
           member Mm_ViewObjRef_for_Report;
}

set RpstryClntxctsMmAplctnsxctblbyR {
           order  last;
           owner  RepositoryClient;
           member Mm_Aplctn_sxctblby_RpstryClnt;
}

set AplctnsxctblbyMmAplctnsxctblbyR {
           order  last;
           owner  Application;
           member Mm_Aplctn_sxctblby_RpstryClnt;
}

set VwObjRfsrfrncdbyMmUISpchsVwObjR {
           order  last;
           owner  ViewObjRef;
           member Mm_UI_Spec_has_ViewObjRef;
}

set UI_Spc_hs_Mm_UI_Spc_hs_VwObjRf {
           order  last;
           owner  UI_Spec;
           member Mm_UI_Spec_has_ViewObjRef;
}

set Oprtn_fr_Mm_CntrlDf_hsmsg_Oprtn {
           order  last;
           owner  Operation;
           member Mm_CntrlDf_hsmsg_Oprtn;
}

set CntrlDf_hsmsgMmCntrlDfhsmsgOprt {
           order  last;
           owner  ControlDef;
           member Mm_CntrlDf_hsmsg_Oprtn;
}

set TEFldDtRlprtcptsnMmTETblRcKysTE {
           order  last;
           owner  TE_FieldDataRel;
           member Mm_TE_TblRcKy_s_TE_FldDtRl;
}

set TE_TblRcKysMmTETblRcKysTEFldDtR {
           order  last;
           owner  TE_TablRecKey;
           member Mm_TE_TblRcKy_s_TE_FldDtRl;
}

set CntrlDf4mybchngdtMmCntrlDf4cpts {
           order  last;
           owner  ControlDef;
           member Mm_CntrlDf_4cptschngfrm_CntrlDf;
}

set CntrlDf4cptschngfrmMmCntrlDf4cp {
           order  last;
           owner  ControlDef;
           member Mm_CntrlDf_4cptschngfrm_CntrlDf;
}

set CntrlDf1cptdschldMmCntrlDf1cpts {
           order  last;
           owner  ControlDef;
           member Mm_CntrlDf_1cptsprnt_CntrlDf;
}

set CntrlDf1cptsprntMmCntrlDf1cptsp {
           order  last;
           owner  ControlDef;
           member Mm_CntrlDf_1cptsprnt_CntrlDf;
}

set CntrlDf2cptdsprntMmCntrlDf2cpts {
           order  last;
           owner  ControlDef;
           member Mm_CntrlDf_2cptschld_CntrlDf;
}

set CntrlDf2cptschldMmCntrlDf2cptsc {
           order  last;
           owner  ControlDef;
           member Mm_CntrlDf_2cptschld_CntrlDf;
}

set SbjctArnvlvsMmSAEntysnvlvdnSbjc {
           order  last;
           owner  SubjectArea;
           member Mm_SA_Enty_snvlvdn_SbjctAr;
}

set SAEntysnvlvdnMmSAEntysnvlvdnSbj {
           order  last;
           owner  SA_Entity;
           member Mm_SA_Enty_snvlvdn_SbjctAr;
}

set EREntymplmntdbyMmTETblRcDtRlmpl {
           order  last;
           owner  ER_Entity;
           member Mm_TE_TblRcDtRl_mplmnts_ER_Enty;
}

set TETblRcDtRlmplmntsMmTETblRcDtRl {
           order  last;
           owner  TE_TablRecDataRel;
           member Mm_TE_TblRcDtRl_mplmnts_ER_Enty;
}

set DmnscmptblwthMmTEPhysclDtTypcnm {
           order  last;
           owner  Domain;
           member Mm_TE_PhysclDtTyp_cnmplmnt_Dmn;
}

set TEPhysclDtTypcnmplmntMmTEPhyscl {
           order  last;
           owner  TE_PhysicalDataType;
           member Mm_TE_PhysclDtTyp_cnmplmnt_Dmn;
}

set SbjctArnvlvsMmSARlTypsnvlvdnSbj {
           order  last;
           owner  SubjectArea;
           member Mm_SA_RlTyp_snvlvdn_SbjctAr;
}

set SARlTypsnvlvdnMmSARlTypsnvlvdnS {
           order  last;
           owner  SA_RelType;
           member Mm_SA_RlTyp_snvlvdn_SbjctAr;
}

set LODAtrbtsmntfrMmUISEntyhsmntLOD {
           order  last;
           owner  LOD_Attribute;
           member Mm_UIS_Enty_hsmnt_LOD_Atrbt;
}

set UISEntyhsmntMmUISEntyhsmntLODAt {
           order  last;
           owner  UIS_Entity;
           member Mm_UIS_Enty_hsmnt_LOD_Atrbt;
}

set LODAtrbtslstMmUISEntyhslstLODAt {
           order  last;
           owner  LOD_Attribute;
           member Mm_UIS_Enty_hslst_LOD_Atrbt;
}

set UISEntyhslstMmUISEntyhslstLODAt {
           order  last;
           owner  UIS_Entity;
           member Mm_UIS_Enty_hslst_LOD_Atrbt;
}

set LOD_wns_Mm_SrcFl_wndby_LOD {
           order  last;
           owner  LOD;
           member Mm_SourceFile_ownedby_LOD;
}

set SrcFl_wndby_Mm_SrcFl_wndby_LOD {
           order  last;
           owner  SourceFile;
           member Mm_SourceFile_ownedby_LOD;
}

set Dlg_wns_Mm_SrcFl_wndby_Dlg {
           order  last;
           owner  Dialog;
           member Mm_SourceFile_ownedby_Dialog;
}

set SrcFl_wndby_Mm_SrcFl_wndby_Dlg {
           order  last;
           owner  SourceFile;
           member Mm_SourceFile_ownedby_Dialog;
}

set TEDBMSSrcdfnsvlblMmTEPhysclDtTy {
           order  last;
           owner  TE_DBMS_Source;
           member Mm_TEPhysclDtTypsvlblfrTEDBMSSr;
}

set TEPhysclDtTypsvlblfrMmTEPhysclD {
           order  last;
           owner  TE_PhysicalDataType;
           member Mm_TEPhysclDtTypsvlblfrTEDBMSSr;
}

set Instltn_hs_Mm_Usr_blngt_Instltn {
           order  last;
           owner  Installation;
           member Mm_User_belongto_Installation;
}

set Usr_blngt_Mm_Usr_blngt_Instltn {
           order  last;
           owner  User;
           member Mm_User_belongto_Installation;
}

set Usr_wrksn_Mm_RpstryClnt_hs_Usr {
           order  last;
           owner  User;
           member Mm_RepositoryClient_has_User;
}

set RpstryClnt_hs_Mm_RpstryClnthsUs {
           order  last;
           owner  RepositoryClient;
           member Mm_RepositoryClient_has_User;
}

set CPLR_rs_Mm_CPLR_srsdby_CPLR {
           order  last;
           owner  CPLR;
           member Mm_CPLR_isreusedby_CPLR;
}

set CPLR_srsdby_Mm_CPLR_srsdby_CPLR {
           order  last;
           owner  CPLR;
           member Mm_CPLR_isreusedby_CPLR;
}

set VwObjRf_rfrncdn_Mm_Dlg_hsVwObjR {
           order  last;
           owner  ViewObjRef;
           member Mm_Dialog_has_ViewObjRef;
}

set Dlg_hs_Mm_Dlg_hs_VwObjRf {
           order  last;
           owner  Dialog;
           member Mm_Dialog_has_ViewObjRef;
}

set ZMtDfrgrpdtprdcMmExcCmpstsgrpng {
           order  last;
           owner  Z_MetaDef;
           member Mm_ExcCmpst_sgrpngf_Z_MtDf;
}

set ExcCmpstsgrpngfMmExcCmpstsgrpng {
           order  last;
           owner  ExecComposite;
           member Mm_ExcCmpst_sgrpngf_Z_MtDf;
}

set CPLRsblItmptnfndnMmDlgcntnsptnC {
           order  last;
           owner  CPL_ReuseableItem;
           member Mm_Dlg_cntnsptn_CPL_RsblItm;
}

set DlgcntnsptnMmDlgcntnsptnCPLRsbl {
           order  last;
           owner  Dialog;
           member Mm_Dlg_cntnsptn_CPL_RsblItm;
}

set CPLRsblItmwndwfndnMmDlgcntnswnd {
           order  last;
           owner  CPL_ReuseableItem;
           member Mm_Dlg_cntnswndw_CPL_RsblItm;
}

set DlgcntnswndwMmDlgcntnswndwCPLRs {
           order  last;
           owner  Dialog;
           member Mm_Dlg_cntnswndw_CPL_RsblItm;
}

set CPLRsblItmgrpfndnMmDlgcntnsgrpC {
           order  last;
           owner  CPL_ReuseableItem;
           member Mm_Dlg_cntnsgrp_CPL_RsblItm;
}

set DlgcntnsgrpMmDlgcntnsgrpCPLRsbl {
           order  last;
           owner  Dialog;
           member Mm_Dlg_cntnsgrp_CPL_RsblItm;
}

set Window_has_Mm_Menu_for_Window {
           order  last;
           owner  Window;
           member Mm_Menu_for_Window;
}

set Menu_for_Mm_Menu_for_Window {
           order  last;
           owner  Menu;
           member Mm_Menu_for_Window;
}

set Wndw_sctvtdby_Mm_Actn_ctvts_Wnd {
           order  last;
           owner  Window;
           member Mm_Action_activates_Window;
}

set Actn_ctvts_Mm_Actn_ctvts_Wndw {
           order  last;
           owner  Action;
           member Mm_Action_activates_Window;
}

}  /* database ZMM */
