/*
CHANGE LOG

2001.10.12  HH
   removed redundant prototypes.

1999.04.19  RS
   Commentlines required for Rose.

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoengaa.h> //required for Rose

#ifdef __cplusplus
extern "C"
{
#endif

#define zC 1000
#define zMAXLINELTH 1024

#define zXC_NOT_LOADED              0x00010000
#define zXC_OPERATION_NOT_SELECTED  0x00010000
#define zXC_MODULE_NOT_SELECTED     0x00010000
#define zXC_OPERATION_EXECUTED      0x00000000
#define zXC_STEP_EXECUTED           0x00010000
#define zXC_OPERATION_SELECTED      0x00010000
#define zXC_SETUP_DEBUGGER          0x00020000
#define zXC_OPERATION_STAGED        0x00030000


// valid ParseAction Parms
#define zPARSE_ACTION_FULL           2
#define zPARSE_ACTION_INCREMENTAL    1
#define zPARSE_ACTION_OPERATION_ONLY 1

// valid ParseLimit Parms
#define zPARSE_LIMIT_SIGNATURES      1
#define zPARSE_LIMIT_OPERATION       2
#define zPARSE_LIMIT_SOURCE          3

// valid ParseSource Parms
#define zPARSE_SOURCE_FILE           1
#define zPARSE_SOURCE_BUFFER         2
#define zPARSE_SOURCE_XPG            3


zOPER_EXPORT zSHORT OPERATION
Exec_SfCreateSystemSubtask( zPVIEW vReturnSubtask,
                            zPCHAR szAppName );
zOPER_EXPORT zSHORT OPERATION
Exec_SfCreateSubtask( zPVIEW vReturnSubtask,
                      zPCHAR szAppName );

zOPER_EXPORT zSHORT OPERATION
StageBrowser( zVIEW vSubtask, zVIEW vVarView );

zOPER_EXPORT zSHORT OPERATION
ParseSourceFromEditor( zVIEW  vSubtask,
                       zVIEW  vSourceMeta,
                       zLONG  nInParseAction,
                       zLONG  nInParseLimit,
                       zLONG  nInParseSource,
                       zPCHAR szInSourceFileName,
                       zPCHAR szInOperationName,
                       zCPCHAR cpcGenLang );

#ifdef __cplusplus
}
#endif
