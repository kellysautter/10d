//KZOEP0AA.H - Ring 0 prototypes

/*
CHANGE LOG
2002.01.17  HH
   added fnGetDBHandlerOper.

2001.04.20  DGC
   Added flag to fnValidViewAttrib() to allow hidden attributes.

2001.04.06  DGC
   Added fnSetEntityKey();

2000.12.20  HH
   const char change.

2000.12.01  DGC  10b
   Fix for important bug reported by Frank H.  When an entity was being
   excluded it was being flagged as updated.

2000.11.22  DGC  10b
   Added an argument to fnValidSubobjectStructureMatch().

2000.11.07  DGC  10b
   Moved some prototypes into here.

2000.04.08  DGC  10b
   Changed signature for fnSetUpdateIndicator().

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoetdaa.h> //required for Rose
//#include <kzoep1aa.h> //required for Rose
//#include <kzoemeaa.h> //required for Rose

// Include OS-dependent stuff.
#ifndef zNO_KZOEINCO
#include "kzoeinco.h"
#endif

// Used by fnValidViewAttrib()
#define zALLOW_HIDDEN   0x00000001

// Start of prototypes internal to Object Services ONLY

// Prototypes from source module kzoesyaa.c

void LOCALOPER
fnConvertEnvironmentString( zPCHAR pszTarget, zLONG lMaxLth, zCPCHAR cpcSource );

zPVOID LOCALOPER
fnSysGetPointerFromHandle( zPVOID pHandle );

zPVOID OPERATION
fnSysGetHandleFromPointer( zPVOID pPointer );

LPANCHOR OPERATION
InitializeAnchorBlock( zLONG  OE_AnchorBlock,
                       zLONG  OE_MainWindow,
                       zLONG  OE_ListWindow,
                       zLONG  OE_Message );

zSHORT OPERATION
FreeAnchorBlock( LPTASK lpTask );

LPANCHOR  OPERATION
SysGetAnchorBlock( void );

zPVOID  OPERATION
SysGetAnchorBlockData( zLONG lFlag );

zSHORT  OPERATION
SysGetBaseMessage( zPCHAR szReturnString, long lID, zSHORT nMaxLth );

zSHORT  OPERATION
SysGetOperationMessage( zPCHAR szReturnString, long lID, zSHORT nMaxLth );

zSHORT
SysStartObjectServices( zLONG lAppOE_Msg, zLONG hWndAppOE_Msg );

void
SfListTasks( void );

LPINTERNATIONAL OPERATION
SysCreateInternational( zVOID );

zVOID OPERATION
SysFreeInternational( LPINTERNATIONAL lpInternational );

void OPERATION
TraceDataHeader( LPDATAHEADER lpDataHeader, zBOOL bAll );

zLONG LOCALOPER
fnAllocSharedMemory( zCOREMEM ppvMemory,
                     LPTASK   lpTask,
                     zLONG    lBytes,
                     zCPCHAR  cpcShareName );

zLONG LOCALOPER
fnFreeSharedMemory( LPTASK lpTask, zLONG hMemory );

zPVOID LOCALOPER
fnSysCreateHandle( LPDATAHEADER lpDataHeader,
                   zPVOID       pPointer );

void LOCALOPER
fnFreeLocalAnchorBlock( zBOOL bWebTask );

zSHORT LOCALOPER
fnSysMutexUnlock( LPTASK lpTask, LPMUTEX lpMutex );

zSHORT LOCALOPER
fnSysMutexLock( LPTASK lpTask, LPMUTEX lpMutex, zULONG lTimeout, zLONG lControl );

zSHORT LOCALOPER
fnSysMutexDestroy( LPTASK lpTask, LPMUTEX lpMutex );

zSHORT LOCALOPER
fnMutexCreateOS( LPTASK lpTask, LPMUTEX lpMutex, zLONG lControl );

//
// Prototypes from source module kzoeapaa.c
//

zSHORT
fnParseAppString( zPCHAR   szReturnApp,
                  zPCHAR   szReturnWDOD,
                  zPCHAR   szReturnUserID,
                  zPCHAR   szReturnPassword,
                  zPCHAR   szReturnCommand,
                  zCPCHAR  szAppString );

LPAPP
fnCreateApp( LPTASK  lpTask,
             zCPCHAR szAppName );

zSHORT
fnDeleteApp( LPAPP lpApp );

void
fnDropTableEntry( zLPCONTEXT   lpContext,
                  LPTABLEENTRY lpTableEntry,
                  zSHORT       nMaintainLinks );

zSHORT
fnDeleteContext( LPDOMAIN lpDomain );

//
// Prototypes from source module kzoeataa.c
//
LPATTRIBFLAGS LOCALOPER
fnGetAttribFlagsPtr( LPENTITYINSTANCE lpEntityInstance,
                     LPVIEWATTRIB     lpViewAttrib );

zSHORT
fnSetAttributeFromString( zVIEW            zView,
                          LPVIEWENTITYCSR  lpViewEntityCsr,
                          LPVIEWATTRIB     lpViewAttrib,
                          zCPCHAR          szStringValue,
                          zSHORT           bProcessDomain );

zSHORT
fnSetAttributeFromInteger( zVIEW            lpView,
                           LPVIEWENTITYCSR  lpViewEntityCsr,
                           LPVIEWATTRIB     lpViewAttrib,
                           zLONG            lIntegerValue );

zSHORT
fnAddToAttributeFromInteger( zVIEW            lpView,
                             LPVIEWENTITYCSR  lpViewEntityCsr,
                             LPVIEWATTRIB     lpViewAttrib,
                             zLONG            lIntegerValue );

zSHORT
fnSetAttributeFromDecimal( zVIEW            lpView,
                           LPVIEWENTITYCSR  lpViewEntityCsr,
                           LPVIEWATTRIB     lpViewAttrib,
                           zDECIMAL         dDecimalValue );

zSHORT
fnAddToAttributeFromDecimal( zVIEW            lpView,
                             LPVIEWENTITYCSR  lpViewEntityCsr,
                             LPVIEWATTRIB     lpViewAttrib,
                             zDECIMAL         dDecimalValue );

zSHORT
fnAddToAttributeFromVariable( zVIEW            lpView,
                              LPVIEWENTITYCSR  lpViewEntityCsr,
                              LPVIEWATTRIB     lpViewAttrib,
                              zPVOID           lpVariable,
                              zCHAR            cVariableType,
                              zULONG           ulVariableLth,
                              zCPCHAR          szContextName,
                              LPTASK           lpCurrentTask );

zSHORT
fnValidateAttributeParameters( LPTASK          *plpReturnTask,
                               LPVIEWENTITY    *plpReturnViewEntity,
                               LPVIEWENTITYCSR *plpReturnViewEntityCsr,
                               LPVIEWATTRIB    *plpReturnViewAttrib,
                               zSHORT          iOperationID,
                               zSHORT          bUpdate, // Set Operations
                               zVIEW           zView,
                               zCPCHAR         cpcEntityName,
                               zCPCHAR         cpcAttributeName,
                               zSHORT          nFlag );

zSHORT
fnGetAttrAddrFromEntityInstance( zCOREMEM         lpReturnAddress,
                                 zPULONG          uReturnLength,
                                 LPENTITYINSTANCE lpEntityInstance,
                                 LPVIEWATTRIB     lpViewAttrib );

zSHORT
fnStoreValueInEntityInstance( LPENTITYINSTANCE lpEntityInstance,
                              LPVIEWENTITY     lpViewEntity,
                              LPVIEWATTRIB     lpViewAttribute,
                              zPVOID           lpValue,
                              zULONG           uLength );

zSHORT
fnGetStringFromAttribute( zPCHAR          pszReturnString,
                          zULONG          nMaxLth,
                          zVIEW           zView,
                          LPVIEWENTITYCSR lpViewEntityCsr,
                          LPVIEWATTRIB    lpViewAttrib,
                          zCPCHAR         cpcContextName );

zSHORT
fnGetIntegerFromAttribute( zPLONG          lplReturnInteger,
                           zVIEW           zView,
                           LPVIEWENTITYCSR lpViewEntityCsr,
                           LPVIEWATTRIB    lpViewAttrib );

zSHORT
fnGetDecimalFromAttribute( zPDECIMAL       lpdReturnDecimal,
                           zVIEW           zView,
                           LPVIEWENTITYCSR lpViewEntityCsr,
                           LPVIEWATTRIB    lpViewAttrib );

zSHORT
fnGetBlobFromAttribute( zPVOID          pReturnBlob,
                        zPULONG         puMaxLth,
                        zVIEW           zView,
                        LPVIEWENTITYCSR lpViewEntityCsr,
                        LPVIEWATTRIB    lpViewAttrib );

zSHORT
fnSetAttributeFromBlob( zVIEW            zView,
                        LPVIEWENTITYCSR  lpViewEntityCsr,
                        LPVIEWATTRIB     lpViewAttrib,
                        zPVOID           pBlobValue,
                        zULONG           uBlobLth );

zSHORT
fnGetAddrForAttribute( zCOREMEM         lpReturnAddr,
                       zVIEW            zView,
                       LPVIEWENTITYCSR  lpViewEntityCsr,
                       LPENTITYINSTANCE lpEntityInstance,
                       LPVIEWATTRIB     lpViewAttrib );

zSHORT
fnGetAttributeDisplayLength( zPULONG         puReturnLth,
                             zVIEW           lpView,
                             LPVIEWENTITYCSR lpViewEntityCsr,
                             LPVIEWATTRIB    lpViewAttrib,
                             zLPCONTEXT      lpContext,
                             LPTASK          lpCurrentTask );

zSHORT
fnGetAttributeLength( zPULONG          puReturnLth,
                      zVIEW            zView,
                      LPENTITYINSTANCE lpEntityInstance,
                      LPVIEWATTRIB     lpViewAttrib );

zSHORT
fnGetActualAttributeLength( zPULONG         puReturnLth,
                            zVIEW           zView,
                            LPVIEWENTITYCSR lpViewEntityCsr,
                            LPVIEWATTRIB    lpViewAttrib );

zPVOID
fnCreateAttributeRecord( LPTASK           lpTask,
                         LPVIEWENTITY     lpViewEntity,
                         LPENTITYINSTANCE lpEntityInstance );

void
fnConvertDecimalToString( zPCHAR       pszString,
                          LPVIEWATTRIB lpViewAttrib,
                          zDECIMAL     dDouble,
                          zCPCHAR      szContextName );

zSHORT
fnStoreStringInRecord( LPTASK  lpTask,
                       zPCHAR  lpRecord,
                       zULONG  ulOffset,
                       zCPCHAR cpcString,
                       zULONG  ulLth,
                       zSHORT  bNullTerminated );

zSHORT
fnStoreBlobInRecord( LPTASK lpTask,
                     zPCHAR lpRecord,
                     zULONG ulOffset,
                     zPVOID pBlob,
                     zULONG ulBlobLth );

zPCHAR
fnCopyAttributeRecord( LPTASK       lpTask,
                       LPVIEWENTITY lpSrcViewEntity,
                       zCPCHAR      lpSrcRecord,
                       zBOOL        bPersist );

zSHORT
fnDeleteAttributeRecord( LPVIEWENTITY     lpViewEntity,
                         LPENTITYINSTANCE lpEntityInstance,
                         zBOOL            bPersistent );

zSHORT
fnSetAttributeFromVariable( zVIEW            lpView,
                            LPVIEWENTITYCSR  lpViewEntityCsr,
                            LPVIEWATTRIB     lpViewAttrib,
                            zCPVOID          lpVariable,
                            zCHAR            cVariableType,
                            zULONG           ulVariableLth,
                            zCPCHAR          szContextName,
                            LPTASK           lpCurrentTask,
                            zSHORT           nFlag );

zSHORT
fnGetVariableFromAttribute( zPVOID           lpVariable,
                            zPULONG          pusReturnLth,
                            zCHAR            cVariableType,
                            zULONG           ulVariableLth,
                            zVIEW            lpView,
                            LPVIEWENTITYCSR  lpViewEntityCsr,
                            LPVIEWATTRIB     lpViewAttrib,
                            zCPCHAR          szContextName,
                            LPTASK           lpCurrentTask,
                            zSHORT           nFlag );

zSHORT
fnSetAttributeValue( zVIEW    lpView,
                     zCPCHAR  szEntityName,
                     zCPCHAR  szAttributeName,
                     zCPCHAR  szContextName,
                     zSHORT   iOperationID );

zSHORT
fnCompareDateTimeToDateTime( LPDTINTERNAL lpDateTime1,
                             LPDTINTERNAL lpDateTime2 );

zSHORT
fnCompareAttributeToVariable( zVIEW             lpView,
                              LPVIEWENTITYCSR   lpViewEntityCsr,
                              LPVIEWATTRIB      lpViewAttrib,
                              zPVOID            lpVariable,
                              zCHAR             cVariableType,
                              zULONG            ulVariableLth,
                              zCPCHAR           szContextName,
                              LPTASK            lpCurrentTask,
                              zSHORT            nFlag );

zSHORT
fnAttributeValueNull( zVIEW            zView,
                      LPENTITYINSTANCE lpEntityInstance,
                      LPVIEWATTRIB     lpViewAttrib );

//zPVOID
//fnGetDomainEntry( zVIEW zView, LPDOMAIN lpDomain );

LPTASKDOMAIN
fnGetTaskDomain( zVIEW zView, LPTASK lpCurrentTask, LPDOMAIN lpDomain );

zPOBJOPER
fnGetAttribOper( zVIEW lpView, LPVIEWATTRIB lpViewAttrib,
                 LPTASK lpCurrentTask );

zPCHAR
fnRecordForEntityAttr( LPENTITYINSTANCE  lpEntityInstance,
                       LPVIEWATTRIB      lpViewAttribute );

zSHORT
fnInvokeDerivedOperation( zVIEW        lpView,
                          LPVIEWENTITY lpViewEntity,
                          LPVIEWATTRIB lpViewAttrib,
                          LPTASK       lpCurrentTask,
                          zSHORT       nControl );

void
fnSetUpdateIndicator( LPVIEWOI         lpViewOI,
                      LPENTITYINSTANCE lpEntityInstance,
                      LPVIEWATTRIB     lpViewAttrib,
                      zBOOL            bUpdated,
// >>> RAD54278-1 BEGIN
                      zBOOL            bSetLinked,
                      zBOOL            bUpdateEntity );
// >>> RAD54278-1 END

zPCHAR
fnGetTextForTableEntry( LPTABLEENTRY lpTableEntry );

//
// Prototypes from source module kzoecmaa.c
//

zSHORT
fnScanEI_Scoping( zVIEW            zView,
                  LPVIEWENTITY     lpViewEntity,
                  LPVIEWENTITYCSR  lpViewEntityCsr,
                  LPVIEWATTRIB     lpViewEntityAttrib,
                  zPVOID           pQualValue,
                  LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                  LPVIEWATTRIB     lpSrcViewEntityAttrib,
                  zLONG            lRelPosition,
                  LPVIEWENTITYCSR  lpScopingViewEntityCsr,
                  zCPCHAR          szContextName,
                  LPTASK           lpCurrentTask,
                  LPENTITYINSTANCE lpSrcEntityInstance,
                  zVIEW            vAttrView,
                  zLONG            lAbsPosition,
                  zLONG            lQualRequest,
                  zLONG            lQualOperator,
                  zSHORT           nDesiredSelectState,
                  zBOOL            bSelectRelevant,
                  zBOOL            bRelPosition,
                  zBOOL            bSameParentKeepChildren,
                  zBOOL            bRecurse,
                  zBOOL            bCursorTest );

zSHORT
fnScanEI_NoScoping( zVIEW            zView,
                    LPVIEWENTITY     lpViewEntity,
                    LPVIEWENTITYCSR  lpViewEntityCsr,
                    LPVIEWATTRIB     lpViewEntityAttrib,
                    zPVOID           pQualValue,
                    LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                    LPVIEWATTRIB     lpSrcViewEntityAttrib,
                    zLONG            lRelPosition,
                    LPVIEWENTITYCSR  lpScopingViewEntityCsr,
                    zCPCHAR          szContextName,
                    LPTASK           lpCurrentTask,
                    LPENTITYINSTANCE lpSrcEntityInstance,
                    zVIEW            vAttrView,
                    zLONG            lAbsPosition,
                    zLONG            lQualRequest,
                    zLONG            lQualOperator,
                    zSHORT           nDesiredSelectState,
                    zBOOL            bSelectRelevant,
                    zBOOL            bRelPosition,
                    zBOOL            bSameParentKeepChildren,
                    zBOOL            bCursorTest );

zSHORT
fnSetEntityCursor( zVIEW            zView,
                   LPVIEWENTITY     lpViewEntity,
                   LPVIEWENTITYCSR  lpViewEntityCsr,
                   LPVIEWATTRIB     lpViewEntityAttrib,
                   zLONG            lControl,
                   zPVOID           pQualValue,
                   LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                   LPVIEWATTRIB     lpSrcViewEntityAttrib,
                   zLONG            lRelPosition,
                   LPVIEWENTITYCSR  lpScopingViewEntityCsr,
                   zCPCHAR          szContextName,
                   LPTASK           lpCurrentTask );

zSHORT
fnSetEntityCursorRecursive( zVIEW            zView,
                            LPVIEWENTITY     lpViewEntity,
                            LPVIEWENTITYCSR  lpViewEntityCsr,
                            LPVIEWATTRIB     lpViewEntityAttrib,
                            zSHORT           nControl,
                            zPVOID           pQualValue,
                            LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                            LPVIEWATTRIB     lpSrcViewEntityAttrib,
                            zLONG            lRelPosition,
                            LPVIEWENTITYCSR  lpScopingViewEntityCsr,
                            zCPCHAR          szContextName,
                            LPTASK           lpCurrentTask );

zSHORT
fnQualifiedInstance( zVIEW             vDandDView,
                     zLONG             lQualRequest,
                     zLONG             lQualOperator,
                     zPVOID            pQualValue,
                     LPENTITYINSTANCE  lpSearchEntityInstance,
                     LPVIEWENTITYCSR   lpViewEntityCsr,
                     LPVIEWATTRIB      lpViewAttrib,
                     LPENTITYINSTANCE  lpSrcEntityInstance,
                     LPVIEWENTITYCSR   lpSrcViewEntityCsr,
                     LPVIEWATTRIB      lpSrcViewAttrib,
                     zCPCHAR           szContextName,
                     LPTASK            lpCurrentTask );

zSHORT
fnSelectedInstanceFarbler( LPVIEWCSR        lpViewCsr,
                           LPVIEWENTITY     lpViewEntity,
                           LPENTITYINSTANCE lpEntityInstance,
                           zSHORT           nFunction );

zSHORT
fnValidateCursorParameters( LPTASK          *lpReturnTask,
                            LPVIEWENTITY    *lpReturnViewEntity,
                            LPVIEWENTITY    *lpReturnScopingViewEntity,
                            LPVIEWENTITYCSR *lpReturnViewEntityCsr,
                            LPVIEWENTITYCSR *lpReturnScopingViewEntityCsr,
                            zSHORT  iOperationID,
                            zVIEW   zView,
                            zCPCHAR  szEntityName,
                            zCPCHAR  szScopingEntity );

void
fnNullCursorForViewChildren( LPVIEWENTITYCSR lpViewEntityCsr );

void
fnResetCursorForViewChildren( LPVIEWENTITYCSR lpViewEntityCsr );

LPENTITYINSTANCE
fnEstablishCursorForView( LPVIEWENTITYCSR lpViewEntityCsr );

//
// Prototypes from source module kzoedmaa.c
//

zVIEW OPERATION
fnGetMsgQ( zVIEW vSubtask, zPVOID lpTask, zSHORT lFlag );

zSHORT OPERATION
fnGetTableEntryForDomain( zVIEW lpView, LPVIEWENTITY lpViewEntity,
                          LPVIEWATTRIB lpViewAttribute, LPDOMAIN lpDomain,
                          zCPCHAR cpcContextName,
                          zPVOID pvData, zULONG uMaxStringLength,
                          zPVOID pvTablePosition,
                          zLONG lFlag );  // 1-required   2-next

zSHORT OPERATION
Table_Handler( zLONG        lEntryType,
               zCHAR        cType,
               zPVOID       lpData,
               zCPCHAR      szContextName,
               zVIEW        zView,
               LPVIEWENTITY lpViewEntity,
               LPVIEWATTRIB lpViewAttribute,
               zULONG       nMaxStringLength,
               zPVOID       pTablePosition );

zCHAR
fnDomainProcessingRequired( LPTASKDOMAIN  *lpReturnTaskDomain,
                            zVIEW         zView,
                            LPTASK        lpCurrentTask,
                            LPDOMAIN      lpDomain );

zLPCONTEXT
fnGetContext( LPDOMAIN lpDomain, zCPCHAR szContextName );

zSHORT LOCALOPER
fnCallDomainOper( LPTASK       lpTask,
                  LPTASKDOMAIN lpTaskDomain,
                  zLONG        lEntryType,
                  zCHAR        cType,
                  zPVOID       lpData,
                  zCPCHAR      cpcContextName,
                  zVIEW        zView,
                  LPVIEWENTITY lpViewEntity,
                  LPVIEWATTRIB lpViewAttribute,
                  zULONG       uMaxStringLength );

zSHORT LOCALOPER
fnCallDomainOper2( LPTASK       lpTask,
                   LPTASKDOMAIN lpTaskDomain,
                   zLONG        lEntryType,
                   zVIEW        zView,
                   LPVIEWENTITY lpViewEntity,
                   LPVIEWATTRIB lpViewAttribute,
                   zVIEW        zSrcView,
                   LPVIEWENTITY lpSrcViewEntity,
                   LPVIEWATTRIB lpSrcViewAttribute );

//
//
//

zPVOID
fnAllocDataspace( LPDATAHEADER tHeader,
                  zULONG       ulReqSize,
                  zSHORT       nInitialize,
                  char         cInitByte,
                  zSHORT       nID );

zULONG
fnGetDataspaceLength( zPVOID Address );

zPCHAR
fnStoreStringInDataspace( LPDATAHEADER tHeader,
                          zCPCHAR      szName );

zULONG
fnInitializeDataspace( LPTASK        lpCurrentTask,
                       zCOREMEM      Address,
                       zSHORT        nTableID,
                       zCPCHAR       szTitle,
                       zULONG        ulNumberOfEntries,
                       zCPCHAR       szShareName );
zPVOID
fnFreeDataspace( zPVOID Address );
zPVOID
fnFreeDataspace( zPVOID Address );

//
// Prototypes from source module kzoeeiaa.c
//
zSHORT LOCALOPER
fnEntityInstanceIsDead( LPENTITYINSTANCE lpEntityInstance );

void LOCALOPER
fnSetEntityKey( LPTASK           lpTask,
                LPTASK           lpOwningTask,
                LPENTITYINSTANCE lpEntityInstance );

zSHORT OPERATION
fnCreateEntity( zVIEW            zView,
                LPVIEWENTITY     lpViewEntity,
                LPVIEWENTITYCSR  lpViewEntityCsr,
                zSHORT           nPosition,
                zSHORT           bNewRoot );

zSHORT
fnDeleteEntity( zVIEW            lpView,
                LPVIEWENTITY     lpViewEntity,
                LPVIEWENTITYCSR  lpViewEntityCsr,
                zSHORT           nReposition,
                LPTASK           lpCurrentTask );

zSHORT
fnExcludeEntity( zVIEW lpView,
                 LPVIEWENTITY lpViewEntity,
                 LPVIEWENTITYCSR lpViewEntityCsr,
                 zSHORT nReposition,
                 zSHORT bMarkExcluded );

LPVIEWENTITYCSR
fnEstablishViewForInstance( zVIEW             lpView,
                            LPVIEWENTITYCSR   lpViewEntityCsr,
                            LPENTITYINSTANCE  lpEntityInstance );

zSHORT
fnValidateInsertPosition( zPSHORT         bNewRoot,
                          zSHORT          iOperationID,
                          zVIEW           zView,
                          LPVIEWENTITYCSR lpViewEntityCsr,
                          zSHORT          nPosition );

zSHORT
fnValidSubobjectStructureMatch( zVIEW        zTgtView,
                                LPVIEWENTITY lpTgtViewEntity,
                                zVIEW        lpSrcView,
                                LPVIEWENTITY lpSrcViewEntity,
                                zBOOL        bInclude );

zSHORT
fnValidateInstanceParameters( LPTASK           *lpReturnTask,
                              LPVIEWENTITY     *lpReturnViewEntity,
                              LPVIEWENTITYCSR  *lpReturnViewEntityCsr,
                              zSHORT           iOperationID,
                              zVIEW            zView,
                              zCPCHAR          szEntityName,
                              zSHORT           nFlags );

zSHORT
fnIncludeSubobjectFromSubobject( zVIEW         zTgtView,
                                 LPVIEWENTITY  lpTgtViewEntity,
                                 zVIEW         zSrcView,
                                 LPVIEWENTITY  lpSrcViewEntity,
                                 zSHORT        nPosition,
                                 zSHORT        bNewRoot,
                                 zSHORT        bMarkIncluded );

zBOOL
fnExcludeEntityInstance( LPENTITYINSTANCE lpEntityInstance );

zSHORT
fnSpawnCreateEntity( zVIEW lpView,
                     LPENTITYINSTANCE lpEntityInstance );

zSHORT
fnSpawnIncludeSubobject( zVIEW            lpTgtView,
                         LPVIEWOI         lpTgtViewOI,
                         LPVIEWENTITY     lpTgtViewEntity,
                         LPVIEWENTITYCSR  lpTgtViewEntityCsr,
                         zSHORT           bMarkIncluded );

zVOID LOCALOPER
fnReclaimHiddenInstances( LPVIEWOI  lpViewOI );

zBOOL
fnInstanceLinkedToInstance( LPENTITYINSTANCE lpSrcInstance,
                            LPENTITYINSTANCE lpTgtInstance );

zBOOL
fnInstanceVersioned( LPENTITYINSTANCE lpEntityInstance );

zBOOL
fnInstanceVersionRootMatch( LPENTITYINSTANCE lpTgtEntityInstance,
                            LPENTITYINSTANCE lpSrcEntityInstance );

zSHORT
fnHideLinkedChildren( LPENTITYINSTANCE lpStartEntityInstance,
                      zSHORT           bMarkExcluded );

zSHORT
fnHideExcludedDeletedInstances( LPENTITYINSTANCE lpStartEntityInstance,
                                zSHORT           bMarkExcluded );

zSHORT
fnInstanceKeysMatch( LPENTITYINSTANCE lpTgtEntityInstance,
                     LPENTITYINSTANCE lpSrcEntityInstance );

zSHORT
fnRelinkInstanceToInstance( LPENTITYINSTANCE lpTgtEntityInstance,
                            LPENTITYINSTANCE lpSrcEntityInstance );

zSHORT
fnRelinkVsnSubobjectToSubobject( zVIEW            lpTgtView,
                                 LPENTITYINSTANCE lpTgtEntityInstance,
                                 zVIEW            lpSrcView,
                                 LPENTITYINSTANCE lpSrcEntityInstance,
                                 zSHORT           nInvertedParents,
                                 LPVIEWENTITY     lpInvertedSrcEntity );

zSHORT
fnOEAcceptConstraints( zVIEW             lpView,
                       LPVIEWENTITYCSR   lpViewEntityCsr,
                       zBOOL             bCheckChildEntities );

zPECEOPER
fnGetEntityCnstrntOper( zVIEW    lpView, LPVIEWENTITY lpViewEntity,
                        LPTASK   lpCurrentTask );

zSHORT
fnInvokeECEOperation( zVIEW        lpView,
                      LPVIEWENTITY lpViewEntity,
                      LPTASK       lpCurrentTask,
                      zSHORT       nEvent,
                      zSHORT       nState );

zSHORT
fnCheckChildEntities( zVIEW            lpView,
                      LPENTITYINSTANCE *lpEntityInstance,
                      zBOOL            bChkAllEntitiesAtChildLvl );

zSHORT
fnRemoveAllChildTemporalVsns( zVIEW            lpView,
                              LPENTITYINSTANCE lpEntityInstance,
                              zBOOL            bAccept );

zSHORT
fnRemoveOtherOI_LinkedInstances( LPENTITYINSTANCE lpTgtEntityInstance,
                                 LPENTITYINSTANCE lpSrcEntityInstance );

zBOOL
fnEntityInRecursivePath( LPVIEWENTITY lpViewEntity );

LPENTITYINSTANCE
fnFirstLevel2_ForViewEntity( LPENTITYINSTANCE lpStartEntityInstance,
                             LPVIEWENTITY lpViewEntity );

zBOOL
fnStillPrevVersion( LPENTITYINSTANCE lpEntityInstance );

zBOOL LOCALOPER
fnEntityContainsEntity( LPVIEWENTITY lpVE1, LPVIEWENTITY lpVE2 );

//
// Prototypes from source module kzoeeraa.c
//
zSHORT
fnIssueCoreError( LPTASK       lpTask,
                  zVIEW        lpView,
                  zSHORT       nSeverity,
                  zSHORT       nID,
                  zLONG        lLong,
                  zCPCHAR      szMessage1,
                  zCPCHAR      szMessage2 );

zSHORT
fnIssueCoreError_MsgType( zVIEW        lpView,
                          zSHORT       nSeverity,
                          zSHORT       nID,
                          zLONG        lMsgType,
                          zLONG        lLong,
                          zCPCHAR      szMessage1,
                          zCPCHAR      szMessage2 );

//
// Prototypes from source module kzoeoaaa.c
//
zSHORT      OPERATION
fnDeleteViewObject( LPVIEWOD lpViewOD );

//
// Prototypes from source module kzoeodaa.c
//

zSHORT
fnActivateEmptyObjectInstance( LPTASK lpTask, zVIEW zView, zLONG lControl );

zSHORT
fnActivateObjectInstance( LPTASK lpCurrentTask,
                          zVIEW zView, zVIEW zQualView, zLONG lControl );

zSHORT
fnActivateOI_FromFile( zVIEW   zView,
                       zCPCHAR szFilename,
                       zLONG   lControl );

zSHORT
fnActivateOI_FromPortableFile( zVIEW   lpView,
                               zLONG   hFile,
                               zBOOL   bIncremental,
                               zBOOL   bRootOnly,
                               zLONG   lControl,
                               zCPCHAR szOpenFileName );

zSHORT
fnActivateOI_FromBinaryFile( zVIEW   lpView,
                             zLONG   hFile,
                             zBOOL   bIncremental,
                             zBOOL   bRootOnly,
                             zLONG   lControl,
                             zCPCHAR szOpenFileName );

zSHORT
fnCommitOI_ToPortableFile( zVIEW  lpView,
                           zLONG  hFile,
                           zBOOL  bIncremental );
zSHORT
fnCommitOI_ToBinaryFile( zVIEW  lpView,
                         zLONG  hFile,
                         zBOOL  bIncremental );

//
// Prototypes from source module kzoemqaa.c
//

LPTASKAPP
fnFindTaskApp( zVIEW zView, LPTASK lpTask, LPAPP lpApp );

LPMQINTERFACE
fnFindMQInterface( zVIEW zView, LPTASK lpTask );

//
// Prototypes from source module kzoeoiaa.c
//

zSHORT
fnDropObjectInstance( zVIEW zView );

void
fnDropViewOI( LPVIEWOI lpViewOI );

void
fnDateTimeFormat( zPCHAR pchDateTime, zPCHAR pchDate, zLONG lMaxDateLth, zPCHAR pchTime, zLONG lMaxTimeLth );

zPOCEOPER
fnGetObjCnstrntOper( zVIEW    lpView,
                     LPTASK   lpCurrentTask );

zSHORT
fnInvokeOCEOperation( zVIEW        lpView,
                      LPTASK       lpCurrentTask,
                      zSHORT       nEvent,
                      zSHORT       nState );

void
fnCleanupInstance( zVIEW lpView, zLONG lHiddenCnt );

LPTASKDBHANDLER LOCALOPER
fnGetDBHandlerOper( LPTASK      lpTask,
                    zVIEW       lpView,
                    zlpPDBHOPER pOperReturn );
LPDBHANDLER LOCALOPER
fnGetDBHandler( LPTASK  hTask,
                zCPCHAR cpcDBHandlerName );
LPGKHANDLER LOCALOPER
fnGetGKHandler( LPTASK  hTask,
                zCPCHAR cpcGKHandlerName );
zSHORT
fnGetGKHandlerOper( zVIEW       lpView,
                    LPTASK      lpTask,
                    LPGKHANDLER lpGKHandler,
                    zlpPGKHOPER pOperReturn );
//
// Prototypes from source module kzoetmaa.c
//

zSHORT OPERATION
DeleteTask( LPTASK lpTask );

LPPROCESS
fnFindProcess( LPTASK hTask );

zVIEW  OPERATION
FindWebSubtask( zCPCHAR cpcSessionId );

void OPERATION
fnDropTaskOperations( LPTASK lpTask );

zSHORT OPERATION
fnDropTaskHandlers( LPTASK lpTask );

void OPERATION
fnDropTaskDomains( LPTASK lpTask );

void OPERATION
fnFreeTaskModules( LPTASK lpTask );

void OPERATION
fnDropTaskMutexes( LPTASK lpTask );

void OPERATION
fnCloseTaskOpenFiles( LPTASK lpTask );

void OPERATION
fnCloseTaskNetworkModules( LPTASK lpTask );

void
fnStartNetwork( void );

LPPROCESS
fnCreateProcess( zLONG   hMainWnd,
                 zLONG   lMsg );
LPTASK
fnCreateTask( zLONG   lSystemAnchorBlock,
              zLONG   hMainWnd,
              zLONG   lMsg,
              zCPCHAR cpcTaskTitle );

zVIEW
fnCreateSubtask( LPTASK lpTask, LPAPP lpApp );

zSHORT LOCALOPER
fnGetApplicationForSubtask( LPAPP *lpApp, zVIEW lpView );

zSHORT
fnDropSubtask( LPTASK lpCurrentTask, LPSUBTASK lpSubtask, zSHORT nCleanup );

zSHORT
fnFreeTaskDataspace( LPTASK lpTask );

zSHORT LOCALOPER
fnStartBrowseOfProcessList( LPTASK lpTask, zBOOL bForWrite );

void LOCALOPER
fnEndBrowseOfProcessList( zBOOL bForWrite );

zSHORT LOCALOPER
fnStartBrowseOfTaskList( LPTASK lpTask, zBOOL bForWrite );

void LOCALOPER
fnEndBrowseOfTaskList( zBOOL bForWrite );

//
// Prototypes from source module kzoetraa.c
//

zSHORT
fnDisplayEntityInstance( zVIEW            zView,
                         LPVIEWENTITYCSR  lpViewEntityCsr,
                         zSHORT           nSpaces,
                         LPVIEWENTITYCSR  lpViewEntityCsrPosition );
zSHORT
fnCompareOI_ToOI( zVIEW lpTgtView, zVIEW lpSrcView, zLONG lControl );
zSHORT
fnCompareEntityToEntity( LPVIEWENTITY     lpTgtViewEntity,
                         LPENTITYINSTANCE lpTgtEntityInstance,
                         LPVIEWENTITY     lpSrcViewEntity,
                         LPENTITYINSTANCE lpSrcEntityInstance,
                         zBOOL            bComparePersistOnly,
                         zBOOL            bCompareKeysOnly );


//
// Prototypes from source module kzoeufaa.c
//

zSHORT
fnDetermineDateTimeStrlen( zCPCHAR cpcEditString );


//
// Prototypes from source module kzoevlaa.c
//

#define zVALID_VIEW           0x00000001
#define zVALID_SUBTASK_VIEW   0x00000002 + zVALID_VIEW
#define zVALID_VIEW_OBJECT    0x00000004 + zVALID_VIEW
#define zVALID_VIEW_CSR       0x00000008 + zVALID_VIEW_OBJECT

LPTASK
fnOperationCall( zSHORT nOperationID, zVIEW vTaskView, zLONG lFlags );

void
fnOperationReturn( zSHORT nOperationID, LPTASK lpTask );

zSHORT
fnValidView( LPTASK lpTask, zVIEW zView );

zSHORT
fnValidViewObject( LPTASK lpTask, zVIEW zView );

zSHORT
fnValidViewCsr( LPTASK lpTask, zVIEW zView );

zSHORT
fnValidSubtaskView( LPTASK lpTask, zVIEW zView );

LPVIEWENTITY OPERATION
fnValidViewEntity( LPVIEWENTITYCSR *lpReturnViewEntityCsr,
                   zVIEW zView, zCPCHAR cpcEntityName, zLONG lFlags );

LPVIEWATTRIB
fnValidViewAttrib( zVIEW        zView,
                   LPVIEWENTITY lpViewEntity,
                   zCPCHAR      cpcAttribName,
                   zLONG        lControl );

zSHORT
fnValidNumericString( zPLONG lplReturnValue, zCPCHAR cpcString );

zSHORT
fnValidDecimalString( zPDECIMAL pdReturnValue,
                      zCPCHAR   cpcString );

zVOID LOCALOPER
fnCompareDebugChange( zBOOL bOperationCall, LPTASK lpTask );

zVOID LOCALOPER
fnAddEntityToDebugChange( LPENTITYINSTANCE lpEntityInstance );

zVOID LOCALOPER
fnRemoveEntityFromDebugChange( LPENTITYINSTANCE lpEntityInstance );

//
// Prototypes from source module kzoevmaa.c
//

zSHORT OPERATION
fnDeclareView( zPVIEW zpReturnView, LPTASK lpCurrentTask, zVIEW zView,
               zCPCHAR cpcViewOD_Name, zBOOL bSystemObject );

zSHORT
fnSetViewFromView( zVIEW  lpTgtView, zVIEW  lpSrcView );

zSHORT OPERATION
fnDropView( zVIEW zView );

zSHORT
fnDropViewCsr( LPVIEWCSR lpViewCsr );

zSHORT
fnSetViewToSubobject( zVIEW  lpView,
                      LPVIEWENTITY lpViewEntity,
                      LPVIEWENTITYCSR lpViewEntityCsr );

zSHORT
fnResetView( zVIEW  lpView, zSHORT nControl );

zSHORT
fnResetViewFromSubobject( zVIEW zView );

void
fnDropAllViewSubobjects( zVIEW zView );

zSHORT
fnOrderEntitiesByDefaultAttribs( zVIEW            zView,
                                 LPENTITYINSTANCE lpEntityInstance );

zSHORT
fnOrderOI_ByDefaultAttribs( zVIEW zView );
