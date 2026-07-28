/*
   parsgen.h

   Update 27.08.1998 HH
   I guess I don't need the zwTZCMSLPD_RebuildMetaLists( zVIEW vSubtask ) operation??
*/

#define CMD_RUN  1


zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_RebuildTablesRels( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_InitTool( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwfnTZTEUPDD_InitDTE( zVIEW vSubtask,
                      zVIEW vCM_List );
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SaveModelDTE( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SwitchLPLR( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZCMSLPD_RebuildMetaLists( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZCMSLPD_SwitchLPLR( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_GenerateJSPJava(zVIEW vDialog, zVIEW vSubtask);
