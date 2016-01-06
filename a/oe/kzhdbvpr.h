/*
CHANGE LOG

1999.04.19  RS
   Commentlines required for Rose.

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoep1aa.h> //required for Rose
//#include <vista.h> //required for Rose
//#include <kzoemeaa.h> //required for Rose

   /*** begin prototypes ***/

zSHORT InitDatabase ( zVIEW lpView );
zSHORT CloseDatabase ( zVIEW lpView, int Indicators );
zSHORT CloseDatabaseFiles( zVIEW lpView, int Indicators );
zSHORT BeginTransaction ( int      Indicators,
                          LPVIEWOD lpViewOD,
                          zVIEW    lpView,
                          zVIEW    lpQualView,
                          zPVOID   lpWorkPtr );
zSHORT CommitTransaction ( LPCONNECTION lpConnection );
zSHORT RollbackTransaction ( LPCONNECTION lpConnection );
zSHORT Load ( LPVIEWOD     lpViewOD,
              LPVIEWENTITY lpViewEntity,
              zVIEW        lpView,
              zVIEW        lpQualView,
              LPCONNECTION lpConnection,
              int          Indicators );
zSHORT Insert ( LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zVIEW        lpView,
                LPCONNECTION lpConnection );
zSHORT InsertRel ( LPVIEWOD     lpViewOD,
                   LPVIEWENTITY lpViewEntity,
                   zVIEW        lpView,
                   LPCONNECTION lpConnection );
zSHORT Delete ( LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zVIEW        lpView,
                LPCONNECTION lpConnection );
zSHORT DeleteRel ( LPVIEWOD     lpViewOD,
                   LPVIEWENTITY lpViewEntity,
                   zVIEW        lpView,
                   LPCONNECTION lpConnection );
zSHORT Update ( LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zVIEW        lpView,
                zSHORT       bAutoSeqOnly,
                LPCONNECTION lpConnection );
zSHORT AddEntityToParentPath ( zVIEW        lpView,
                               zSHORT       nPathType,
                               LPVIEWENTITY lpViewEntity,
                               LPCONNECTION lpConnection );
zSHORT AddKeysToEntity ( zVIEW        lpView,
                         LPVIEWENTITY lpViewEntity,
                         LPENTITY     lpEntity,
                         LPCONNECTION lpConnection );
zSHORT AddSetToPathTable ( zSHORT       nPathType,
                           LPSET        *plpSet,
                           LPCONNECTION lpConnection );
void BuildWhereClause ( zVIEW  lpView,
                        zPCHAR szWhereClause );
zSHORT ConvertAttributeValue ( char     cInType,
                               zPVOID   lpInValue,
                               char     cOutType,
                               zPVOID   lpOutValue,
                               zPUSHORT puLth );
zSHORT CopyEntityToRecordBuffer ( zVIEW        lpView,
                                  LPVIEWENTITY lpViewEntity,
                                  zSHORT       bAutoSeqOnly,
                                  LPCONNECTION lpConnection );
zSHORT CopyQualToPath ( zVIEW        lpQualView,
                        zVIEW        lpView,
                        LPCONNECTION lpConnection );
zSHORT CreateEntityInsFromCurrentRec ( zVIEW        lpView,
                                       LPVIEWENTITY lpViewEntity,
                                       LPCONNECTION lpConnection );
zSHORT CreateOverflowRecord ( zPCHAR       pDataValue,
                              zUSHORT      uLth,
                              zPCHAR       pBufferPtr,
                              LPDATAFIELD  lpDataField,
                              LPCONNECTION lpConnection );
void DBA_ToStr ( DB_ADDR *dba, zPCHAR str );
void DBH_Error ( zVIEW lpView, zPCHAR szErrorMsg,
                 zPCHAR szInfo1, zPCHAR szInfo2 );
zSHORT DeleteOverflowRecords ( LPCONNECTION lpConnection, zBOOL bDelete );
zSHORT DeleteOverflowRecsForCurrentRec ( LPVIEWENTITY lpViewEntity,
                                         LPCONNECTION lpConnection );
void FreeKeys ( LPENTITY lpEntity );
void FreeSets ( LPPATH lpPath );
zSHORT GetParentPathFromOI ( zVIEW        lpView,
                             LPVIEWENTITY lpViewEntity,
                             zSHORT       Message,
                             LPCONNECTION lpConnection );
int LocateFirstRecord ( LPENTITY     lpEntity,
                        zUSHORT      uControl,
                        LPCONNECTION lpConnection );
int LocateNextRecord ( LPENTITY lpEntity, LPCONNECTION lpConnection );
int NavigateOI_ParentPath ( zUSHORT Message, LPCONNECTION lpConnection );
void RaimaDM_RC ( zPCHAR       szRoutine,
                  zSHORT       nRC,
                  zPCHAR       szInfo1,
                  zPCHAR       szInfo2,
                  zSHORT       nTrcLvl,
                  LPCONNECTION lpConnection );
zSHORT ReadOverflowRecord ( zPVOID       lpSrc,
                            zPCHAR       lpDest,
                            LPDATAFIELD  lpDataField,
                            LPCONNECTION lpConnection );
void TracePathTable ( zSHORT nPathType, LPCONNECTION lpConnection );

DB_VOID OPERATION LocalDBError( int nErrorNum, char * szErrorMsg );
 /*** end of prototypes ***/
