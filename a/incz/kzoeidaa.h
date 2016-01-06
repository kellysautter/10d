//KZOEIDAA.H - ID's for internal object Engine tables

/*
CHANGE LOG

2003.04.10  DGC
   Added WriteSubobjectToXML.

2002.11.20  DGC
   Added GenerateQualFromEntityList.

2002.10.04  DGC
   Added iStoreString/ValueInDerivedAttribute().

2002.01.15  HH
   Added SetDatabaseUser.

2001.12.06  DGC
   Added ActivateOI_FromXML_File.

2001.10.11  DGC  10b
   Added code for GetViewFlags().

2001.08.02  HH
   added SubobjectVersionUpdated.

2001.07.30  DGC  10b
   Added iSetBlobFromFile and iWriteBlobToFile.

2001.04.18  DGC  10b
   Added code for DropViewObject() logic.

2001.04.06  DGC 10b
   Added iGetEntityKey

2000.07.27  DGC 10b
   Added IncludeSubobjectFromSubobjectEx.

2000.07.10  DGC  10b
   Added SfCheckOI_Integrity

2000.05.02  DGC  10b
   Added iMiGetActivateFlags, iMiHasPersistentChanges.

2000.03.14  DGC  10b
   Added iAddToViewCluster.

1999.10.08  DGC  10a
   Added SetViewFlags.

1999.07.21  DGC  10a
   Made changes for cluster interface changes.

1999.06.01  DGC  10a
   Added GetAttributeFlags().

1999.05.25  DGC  10a
   Added stuff for GetEntityNameForHierarchicalCsr().

1999.05.07  DGC  10a
   Changed NetCallTransformation to NetCallOperation.

*/

// Table ID's - Each table is given a unique identifier in order
// to validate that pointers are pointing to the right thing

#define iAnchor               10001    // Anchor block
#define iDataHeader           10002    // Table Header
#define iApp                  10003    // Application Definition
#define iDomain               10004    // Domain Definition
#define iDBHandler            10005    // Database Handler
#define iTask                 10006    // Application Task
#define iTaskShare            10007    // Application Task Share
#define iTaskDomain           10008    // Domain Definition
#define iTaskDBHandler        10009    // DBHandler definition for task
#define iSubtask              10010    // Application Subtask
#define iViewOD               10011    // View Object Definition
#define iViewEntity           10012    // View Entity
#define iViewAttrib           10013    // View Attribute
#define iDataRecord           10014    // Data Record
#define iDataField            10015    // Field Mapping
#define iRelRecord            10016    // Relationship Record
#define iRelField             10017    // Field Mapping
#define iViewOI               10018    // View Object Instance
#define iViewCsr              10019    // View Cursor Set
#define iViewSubobject        10020    // View Subobject for recurs Subs
#define iView                 10021    // View
#define iViewName             10022    // View Name
#define iViewEntityCsr        10023    // View Entity Csr
#define iEntityInstance       10024    // Entity Instance
#define iLibrary              10025    // Library
#define iUsedFreespace        10026    // Used Freespace Chunk
#define iFreespace            10027    // Freespace
#define iOpenFile             10028    // Open File
#define iError                10029    // Error Object
#define iContext              10030    // Context Object
#define iTableEntry           10031    // TableEntry Object
#define iRegularExpression    10032    // RegularExpression Object
#define iSelectedInstance     10033    // Selected Instance
#define iTaskOperation        10034    // Task operations
#define iGKHandler            10035    // Genkey handler
#define iTaskGKHandler        10036    // GKHandler definition for task
#define iTaskApp              10037    // TaskApp definition for task
#define iModelEnt             10038    // Model Entity Struct
#define iModelAtt             10039    // Model Attribute Struct
#define iModelRel             10040    // Model Rel Struct
#define iLName                10041    // Logical Name struct
#define iInternational        10042    // International information
#define iNLS_TableEntry       10043    // NLS_TableEntry Object
#define iTaskMemory           10044    // Mem alloc'd by SfAllocTaskMemory().
#define iMutex                10045    //
#define iCountBuf             10046    // Used in OperationCount().
#define iMQInterface          10047    //
#define iOperIdx              10048    // Operation Index
#define iHandler              10049    // Handler pointer Index
#define iProcess              10050    // Process struct
#define iUsedFreespace1       10051    // Used Freespace Chunk marked in last cleanup

#define iDebugChange          10052    // MUST BE LAST!

// Following don't cause nTableID to be set in the allocated block.
#define iAttributeRecord      10100    //
#define iBlob                 10101    //
#define iAttributeString      10102    //
#define iString               10103    //
#define iCluster              10104    //
#define iBuffer               10105    //
#define iCacheBuffer          10106    //
#define iRelinkBuffer         10107    //

// Operation ID's - Each documented operation is given a unique
// identifier so debugging may take place

// 1-50     registration and system stuff 0 is RESERVED!
#define iSfCreateSubtask                  10
#define iSfDropSubtask                    11
#define iDisableTask                      12
#define iEnableTask                       13
#define iGetApplDirectoryFromView         20
#define iSfGetApplicationForSubtask       21
#define iSfSetApplicationForSubtask       22
#define iSfGetDefaultContextForDomain     23
#define iSfGetTaskInfo                    24
#define iSfAllocTaskMemory                25
#define iSfFreeTaskMemory                 26
#define iSfReAllocTaskMemory              27
#define iSfCheckOI_Integrity              28
#define iSetDatabaseUser                  29

// 50-100   Instance activation
#define iActivateEmptyObjectInstance      50
#define iActivateObjectInstance           52
#define iActivateOI_FromFile              54
#define iActivateOI_FromOI                55
#define iCommitObjectInstance             60
#define iCommitOI_ToFile                  61
#define iCommitMultipleOIs                62
#define iDropViewCluster                  63
#define iAddToViewCluster                 64
#define iGenerateQualFromEntityList       65
#define iDropObjectInstance               70
// 100-150  Entity creation/deletion etc
#define iCheckExistenceOfEntity          100
#define iCreateEntity                    101
#define iCreateTemporalSubobjectVersion  102
#define iAcceptSubobject                 103
#define iCancelSubobject                 104
#define iIncludeSubobjectFromSubobject   105
#define iDeleteEntity                    106
#define iExcludeEntity                   107
#define iGetEntityKey                    108
#define iMoveSubobject                   110
#define iGetSelectStateOfEntity          112
#define iSetAllSelectStatesForEntity     113
#define iSetSelectStateOfEntity          114
#define iCreateTemporalEntity            115
#define iRelinkInstanceToInstance        116
#define iLoadEntity                      117
#define iObjectInstanceUpdated           118
#define iSetIncrementalUpdateFlags       119
#define iGetIncrementalUpdateFlags       120
#define iRelinkAllSubobjectsForOI        121
#define iOrderEntityForView              122
#define iCountEntitiesForView            123
#define iOrderEntitiesByDefaultAttribs   124
#define iOrderOI_ByDefaultAttribs        125
#define iDropEntity                      126
#define iAcceptAllTemporalSubobjects     127
#define iCancelAllTemporalSubobjects     128
#define iSetSelectSetForView             129
#define iObjectInstanceUpdatedFromFile   130
#define iCountEntitiesForViewUnderParent 131
#define iCheckEntityInView               132
#define iIncludeSubobjectFromSubobjectEx 133
#define iSubobjectVersionUpdated         134

// 150-200  Cursor navigation
#define iSetCursorNextEntity             150
#define iSetCursorPrevEntity             151
#define iSetCursorFirstEntity            152
#define iSetCursorLastEntity             153
#define iSetCursorFirstEntityByString    170
#define iSetCursorNextEntityByString     171
#define iSetCursorFirstEntityByInteger   172
#define iSetCursorNextEntityByInteger    173
#define iSetCursorFirstEntityByDecimal   174
#define iSetCursorNextEntityByDecimal    175
#define iSetCursorFirstEntityByAttr      176
#define iSetCursorNextEntityByAttr       177
#define iSetCursorFirstEntityByEntityCs  178
#define iSetCursorNextEntityByEntityCsr  179
#define iSetCursorFirstSelectedEntity    180
#define iSetCursorNextSelectedEntity     181
#define iSetCursorRelativeEntity         182
#define iGetRelativeEntityNumber         183
#define iSetEntityCursor                 184
#define iGetAbsolutePositionForEntity    186
#define iSetCursorAbsolutePosition       187
//
// Note: All non hierarchical operations should have
//       id's that are less than iDefineHierarchicalCursor,
//       otherwise an error will occur in fnValidateCursorParameters
//
#define iDefineHierarchicalCursor        190
#define iDropHierarchicalCursor          191
#define iSetCursorNextEntityHierarchica  192
#define iGetEntityNameForHierarchicalCsr 193
#define iGetEntityKeyForHierarchicalCsr  194

// 200-249  View management
#define iCreateViewFromView              200
#define iSetViewFromView                 201
#define iSetViewToSubobject              202
#define iResetViewFromSubobject          203
#define iSetNameForView                  204
#define iDropNameForView                 205
#define iGetViewByName                   206
#define iSetSubtaskView                  207
#define iGetNameForView                  208
#define iSfGetFirstSubtaskView           209
#define iSfGetNextSubtaskView            210
#define iResetView                       211
#define iDropView                        212
#define iSfTransferView                  213
#define iSfLockView                      214
#define iSfActivateSysOI_FromFile        215
#define iSfActivateSysEmptyOI            216
#define iSetViewReadOnly                 217
#define iSfGetFirstNamedView             218
#define iSfGetNextNamedView              219
#define iSfCreateSysViewFromView         220
#define iResetViewPositions              221
#define iSetViewFlags                    222
#define iDropViewObject                  223
#define iGetViewFlags                    224
#define iSfSetApplicationTask            225
#define iSfGetUserIdForTask              226
#define iSfSetUserIdForTask              227

// 250-300  Attribute retrieval/setting
#define iGetStringFromAttribute          250
#define iGetIntegerFromAttribute         251
#define iGetDecimalFromAttribute         252
#define iGetBlobFromAttribute            253
#define iGetAttributeFlags               254
#define iAttributeUpdated                255
#define iGetStructFromEntityAttrs        258
#define iGetAddrForAttribute             259
#define iSetAttributeFromString          260
#define iSetAttributeFromInteger         261
#define iSetAttributeFromDecimal         262
#define iSetAttributeFromBlob            263
#define iSetAttributeFromAttribute       264
#define iSetAttributeFromCurrentDate     265
#define iAddToAttributeFromInteger       266
#define iAddToAttributeFromDecimal       267
#define iAddToAttributeFromAttribute     268
#define iSetAttributeFromView            269
#define iCompareAttributeToString        270
#define iCompareAttributeToInteger       271
#define iCompareAttributeToDecimal       272
#define iCompareAttributeToAttribute     273
#define iGetAttributeLength              274
#define iSetMatchingAttributesByName     275
#define iSetBlobAttributeFromAttribute   276
#define iSetAttributeFromVariable        277
#define iCompareAttributeToVariable      278
#define iGetVariableFromAttribute        279
#define iGetFirstTableEntryForAttribute  280
#define iGetNextTableEntryForAttribute   281
#define iSetAttributeValueNext           282
#define iSetAttributeValuePrev           283
#define iStoreValueInRecord              284
#define iAddToAttributeFromVariable      285
#define iGetValueFromRecord              286
#define iStoreStringInRecord             287
#define iGetStringFromRecord             288
#define iGetActualAttributeLength        289
#define iGetAddrFromRecord               290
#define iSetBlobFromEntityAttributes     291
#define iSetEntityAttributesFromBlob     292
#define iGetAttributeDisplayLength       293
#define iSetBlobFromOI                   294
#define iSetOI_FromBlob                  295
#define iSetBlobFromFile                 296
#define iWriteBlobToFile                 297
#define iStoreValueInDerivedAttribute    298
#define iStoreStringInDerivedAttribute   299

// 350-359  Domain operations
#define iTableEntryExtToInt              350
#define iTableEntryIntToExt              351
#define iTableEntryForInternalValue      352

// 360-369 Message Object operations
#define iMessageSend                     360
#define iMessagePrompt                   361
#define iMessagePromptForInput           362
#define iMessagePresent                  363

#define iUfAddToDateTime                 370
// 371-399 open

// 400-449  Tracing operations
#define iDisplayEntityInstance           400
#define iDisplayObjectInstance           401
#define iDisplayCursorInfo               402
#define iGetTaskDBHandlerTraceLevel      403
#define iSetTaskDBHandlerTraceLevel      404
#define iDisplayEntityInstancePath       405
#define iCompareEntityToEntity           406
#define iCompareOI_ToOI                  407

// 450-499  Meta Inquiry operations
#define iMiGetParentEntityNameForView    450
#define iMiGetInstanceID_ForView         451
#define iMiGetUpdateForView              452
#define iMiGetUpdateForViewEntityAttr    453
#define iMiGetTemporalStateOfEntity      454
#define iMiGetDateTimeForOI              455
#define iMiGetERTokenForEntity           456
#define iMiGetKeyFromInstance            457
#define iMiGetViewEntityForView          458
#define iMiGetCardinalityForEntity       459
#define iMiGetObjectNameForView          460
#define iMiGetOI_ReleaseForView          461
#define iMiSetOI_ReleaseForView          462
#define iMiCompareOI_ReleaseToRelease    463
#define iMiGetActivateFlags              464
#define iMiHasPersistentChanges          465
#define iMiSetInstanceUpdateFlag         466
#define iMiGetTypeForViewEntityAttr      467
#define iMiGetSetViewTaskAppFlags        468
#define iMiEntityVersioned               469

// 500-549  Network operations
#define iNetActivateOI                   500
#define iNetCommitOI                     501
#define iNetClose                        502
#define iNetGetTraceLevel                503
#define iNetListen                       504
#define iNetProcessMessage               505
#define iNetSetTraceLevel                506
#define iNetStartup                      507
#define iNetStatus                       508
#define iNetStopListen                   509
#define iNetCommitOI_ToFile              510
#define iNetActivateOI_FromFile          511
#define iNetSendBootstrapFile            512
#define iNetCallOperation                513
#define iNetGetLocalHostAddress          514
#define iNetSendFile                     515

// 550-549  XML operations
#define iWriteOI_ToXML                   550
#define iActivateOI_FromXML_File         551
#define iCommitOI_ToXML_File             552
#define iWriteSubobjectToXML             553
#define iCommitSubobjectToXML_File       554

