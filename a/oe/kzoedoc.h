//./ ADD NAME=StandardDomainInterface
// Source Module=kzoedoc.h
//***********************************************************************
//
// ENTRY:  StandardDomainInterface
//
//         This is a description of the Domain Interface supplied with
//         the Zeidon product.
//
//         A Domain is used for four purposes:
//         1. Validation - It validates any dat enterd through a window
//         or through a SetAttribute operation.
//         2. Presentation - It formats data when displaying it
//         on windows and reports and it accepts formatted information
//         from data entry on a window.
//         3. Data Conversion - It provides for the controlled
//         conversion of data from one valid Domain to another valid
//         Domain.
//         4. Data Comparison - It provides for the controlled
//         comparison of data from one valid Domain to another valid
//         Domain.
//
// OPERATIONS:   Each Domain entry should handle each of the possible
//               entry types.
// PARAMETERS:
//
// 1. lEntryType -
//       Indicates how to handle the other parameters.  Valid values are:
//       -  zDME_SET_ATTRIBUTE
//             Set Attribute, this entry is always used for setting an
//             attribute.  If the data type is string, it is probably the
//             normal data entry interface for validating string input.
//       -  zDME_GET_VARIABLE -
//             Get Variable, this entry is always used for setting a
//             variable from an attribute.  If the data type is string it
//             is probably the normal data presentation interface for
//             formatting data to a string.
//       -  zDME_COMPARE_ATTRIBUTE -
//             Compare Attribute, this entry will be called whenever an
//             attribute is compared to another attribute or variable.  The
//             Domain for the must include the code to handle the
//             comparison to source with any context defined for it.
//       -  zDME_GET_FIRST_TBL_ENT_FOR_ATTR
//       -  zDME_GET_NEXT_TBL_ENT_FOR_ATTR
//             These two entries are necessary to create the list of
//             entries for a Combo Box.  It could also be used by any
//             interface that wanted to list all the Domain values for a
//             given Context.
//       -  zDME_SET_ATTRIBUTE_VALUE_NEXT
//       -  zDME_SET_ATTRIBUTE_VALUE_PREV
//             These entries are necessary to handle a Spin Button.  It
//             would increment/decrement a numeric value or retrieve the
//             next or previous table entry for a given Context.
//       -  zDME_ADD_TO_ATTRIBUTE
//
//       -  zDME_GET_COMPARE_VALUE
//             Used by SetEntityCursor for performance.  SetEntityCursor
//             will ask for the internal attribute value for its input
//             qualifying value, and use the returned value in determining
//             if the entity instance matches the qualification request.
//             See Return Codes below.
//
//       -  zDME_VALIDATE_LPDATA
//             Go through validation procedures as if this was a
//             zDME_SET_ATTRIBUTE call, but do NOT set any attribute
//             values, just set the return code.
//
// 2. cType -
//       The Source/Target Data Type, identifies the type of data (of valid
//       Zeidon data types) that is the source in a Set call and the target
//       in a Get call.
// 3. lpData -
//       The Source/Target Data Pointer, points to the data that is the
//       source in a Set call and the target in a Get call.
// 4. szContextName    -
//       The Context Name to be applied to this Domain function.
// 5. zView -
//       Identifies the View that is the target in a Set call and the
//       source in a Get call.
// 6. lpViewEntity -
//       Points to the entity that is the target in a Set call and the
//       source in a Get call.
// 7. lpViewAttribute -
//       Points to the attribute that is the target in a Set call and the
//       source in a Get call.
// 8. sMaxStringLength -
//       For a Get call with data type 'string', it identifies the maximum
//       length of the string variable (should be used only with a
//       GetVariableFromAttribute operation).
//
// RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has been set
//
//  For zDME_GET_VARIABLE:
//              0 - Attribute value returned
//             -1 - Attribute value is 'null', but an appropriate,
//                  i.e. 0 for decimal or integer, value was returned.
//
//  For zDME_COMPARE_ATTRIBUTE:
//             -1 - Attribute value < lpData value
//              0 - Attribute value = lpData value
//              1 - Attribute value > lpData value
//
//  For zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
//              0 - Value returned
//             -1 - No value returned, Context has no table entries
//
//  For zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
//              0 - Value returned
//             -1 - No value returned, end of table has been reached
//
//  For zDME_SET_ATTRIBUTE_VALUE_NEXT:
//              0 - Attribute value has been set
//              1 - Attribute value is null
//
//
//  For zDME_SET_ATTRIBUTE_VALUE_PREV:
//             Same as zDME_SET_ATTRIBUTE_VALUE_NEXT.
//
//  For zDME_ADD_TO_ATTRIBUTE:
//              0 - Attribute value has been adjusted
//
//  For zDME_GET_COMPARE_VALUE:
//             -2 - lpData ignored, this tells SetEntityCursor that
//                  it should invoked domain processing with the
//                  zDME_COMPARE_ATTRIBUTE entry type, each time
//                  that is determining if the EntityInstance
//                  matches the requested qualifications.
//             -1 - lpData invalid
//              0 - lpData unchanged, okay to use as is.
//              1 - lpData returned, string value
//              2 - lpData returned, integer value
//              3 - lpData returned, decimal value
//
//  For zDME_VALIDATE_LPDATA:
//              0 - Data is okay.
//
//  For all of the above:
//    zCALL_ERROR - error on call
//
//***********************************************************************
//./ END + 9
int OPERATION
DomainSample( zLONG        lEntryType,
              zCHAR        cType,
              zPVOID       lpData,
              zCPCHAR      cpcContextName,
              zVIEW        zView,
              LPVIEWENTITY lpViewEntity,
              LPVIEWATTRIB lpViewAttribute,
              zUSHORT      uMaxStringLength )

//./ ADD NAME=DerivedAttributeInterface
// Source Module=kzoedoc.h
//***********************************************************************
//
// ENTRY:  DerivedAttributeInterface
//
//         This is a description of the Derived attribute  Interface
//         supplied with the Zeidon product.
//
//         A Derived attribute processing is invoked for an attribute
//         that is marked as derived when:
//
//         1. SetAttribute... operations are called.
//            EntryType = zDERIVED_SET.
//
//         2. Get...FromAttribute operations are called.
//            EntryType = zDERIVED_GET.
//
//         3. SetEntityCursor operation with any zQUAL_... specified
//            for lControl.
//            EntryType = zDERIVED_GET.
//
//         4. Order... operations that have derived attributes for
//            sort keys.
//            EntryType = zDERIVED_GET.
//
// OPERATIONS:   Each Derived Attribute entry should handle each of the
//               possible entry types.
//
// PARAMETERS:
//
// 1. zView -
//       Identifies the View that is the target in a Set call and the
//       source in a Get call.
//
// 2. lpViewEntity -
//       Points to the entity structure that contains the derived
//       attribute.
//
// 3. lpViewAttribute -
//       Points to the attribute structure for the derived attribute.
//
// 4. sEntryType -
//       Indicates the type of operation being performed on the attribute.
//       - zDERIVED_SET
//       - zDERIVED_GET
//
// RETURNS:
//             0 - Operation successful
//   zCALL_ERROR - Operation failed
//
//
//***********************************************************************
//./ END + 5
int OPERATION
DerivedAttributeSample( zVIEW        zView,
                        LPVIEWENTITY lpViewEntity,
                        LPVIEWATTRIB lpViewAttribute,
                        zSHORT       nEntryType )

//./ ADD NAME=@OE_Defines
// Source Module=kzoedoc.h
// ENTRY:  @OE_Defines
//
zACCEPT_NULL_ENTITY                 0x0001
zACTIVATE_ROOTONLY                  0x10000000L
zACTIVATE_ROOTONLY_MULTIPLE         (zACTIVATE_ROOTONLY | zMULTIPLE)
zAPPL_DIR_LIB                       1
zAPPL_DIR_LOCAL                     3
zAPPL_DIR_OBJECT                    2
zAPPL_DIR_SHARED                    4
zAPPL_NAME                          0
zASCII                              0x00000000L
zBINARY                             0x00010000L
zCALL_ERROR                         -16
zCALL_MIN_INFORC                    -10
zCONSTRAINT_VIOLATION               -12
zCURSOR_NULL                        -3
zCURSOR_SET                         0
zCURSOR_SET_NEWPARENT               1
zCURSOR_SET_RECURSIVECHILD          2
zCURSOR_UNCHANGED                   -1
zCURSOR_UNDEFINED                   -2
zDB_UNAVAIL                         -2
zDERIVED_GET                        2
zDERIVED_SET                        1
zDME_ADD_TO_ATTRIBUTE               8
zDME_COMPARE_ATTRIBUTE              3
zDME_GET_COMPARE_VALUE              9
zDME_GET_FIRST_TBL_ENT_FOR_ATTR     4
zDME_GET_NEXT_TBL_ENT_FOR_ATTR      5
zDME_GET_VARIABLE                   2
zDME_SET_ATTRIBUTE                  1
zDME_SET_ATTRIBUTE_VALUE_NEXT       6
zDME_SET_ATTRIBUTE_VALUE_PREV       7
zDME_VALIDATE_LPDATA                10
zDM_TYPE_ALGORITHM                  'A'
zDM_TYPE_EXPRESSION                 'E'
zDM_TYPE_FORMAT                     'F'
zDM_TYPE_TABLE                      'T'
zDT_DAY                             2
zDT_HOUR                            3
zDT_MINUTE                          4
zDT_MONTH                           1
zDT_SECOND                          5
zDT_YEAR                            0
zDUPLICATE_ROOT                     -4
zECE_ACCEPT                         1
zECE_CANCEL                         2
zECE_CREATE                         3
zECE_DELETE                         4
zECE_EXCLUDE                        5
zECE_INCLUDE                        6
zENTITY_NULL                        -2
zIGNORE_ATTRIB_ERRORS               0x00002000L
zIGNORE_ENTITY_ERRORS               0x00001000L
zIGNORE_ERRORS           ( zIGNORE_ENTITY_ERRORS | zIGNORE_ATTRIB_ERRORS )
zINCREMENTAL                        0x00010000L
zLBITS                              0xFFFFFFFFL
zLEVEL_ANY                          0x000F
zLEVEL_APPLICATION                  0x0004
zLEVEL_SAME                         0x0400
zLEVEL_SUBTASK                      0x0001
zLEVEL_SYSTEM                       0x0008
zLEVEL_TASK                         0x0002
zMSGQ_DOMAIN_ERROR                  200
zMSGQ_OBJECT_CONSTRAINT_ERROR       300
zMSGQ_OBJECT_CONSTRAINT_WARNING     100
zMSGQ_PROCESS_ACCEPT                1
zMSGQ_PROCESS_COMMIT                2
zMSGQ_PROCESS_DOMAIN                3
zMSGQ_SYSTEM_ERROR                  1000
zMULTIPLE                           0x00000100L
zNAME_AUTODROP                      0x0040   // For SetNameFromView
zNOENQUEUE                          0x00000400L
zNOI_OKAY                           0x00000200L
zOCE_ACTIVATE                       1
zOCE_ACTIVATE_EMPTY                 2
zOCE_COMMIT                         3
zOCE_DROPOI                         4
zOCE_STATE_FILE                     0x0001
zOCE_STATE_NOI                      0x0008
zOCE_STATE_SHUTDOWN                 0x0002
zOCE_STATE_SYSTEM                   0x0004
zPOS_ABS                            0x00000007L
zPOS_AFTER                          3
zPOS_BEFORE                         4
zPOS_FIRST                          1
zPOS_LAST                           2
zPOS_NEXT                           3
zPOS_PREV                           4
zPOS_RELATIVE                       0x00000008L
zQUAL_ATTR_NULL                     0x00008000L
zQUAL_DECIMAL                       0x00000200L
zQUAL_ENTITYATTR                    0x00001000L
zQUAL_ENTITYCSR                     0x00000800L
zQUAL_EQUAL                         0x00000010L
zQUAL_GREATER_THAN                  0x00000020L
zQUAL_INTEGER                       0x00000100L
zQUAL_LESS_THAN                     0x00000040L
zQUAL_NOT                           0x00000080L
zQUAL_OPERAND                       0x00009700L
zQUAL_OPERATOR                      0x000000F0L
zQUAL_SCOPE_OI                      0x00080000L
zQUAL_SELECTED                      0x00002000L
zQUAL_STRING                        0x00000400L
zQUAL_UNSELECTED                    0x00004000L
zQUAL_ZKEYS                         0x00040000L
zREADONLY                           0x00000800L
zRECURS                             0x00020000L
zREPOS_AFTER                        3
zREPOS_BEFORE                       4
zREPOS_FIRST                        1
zREPOS_LAST                         2
zREPOS_NEXT                         3
zREPOS_NONE                         0
zREPOS_PREV                         4
zSCOPE_OI                (zPCHAR) 0x0001  // C autocasts to zCPCHAR just fine
zSET_ALL                            3
zSET_INCR_CREATED                   0x0002
zSET_INCR_CURSORPOS                 0x0080
zSET_INCR_INCLUDED                  0x0020
zSET_INCR_NOT_INCLUDED              0x0040
zSET_INCR_NOT_RECORDOWNER           0x0400
zSET_INCR_NOT_RELOWNER              0x2000
zSET_INCR_NOT_UPDATED               0x0010
zSET_INCR_OWNERSHIP                 0x0100
zSET_INCR_PERSISTENT                0x0004
zSET_INCR_RECORDOWNER               0x0200
zSET_INCR_RELOWNER                  0x1000
zSET_INCR_UPDATED                   0x0008
zSET_KEYS                           1
zSET_NOTNULL                        2
zSET_NULL                           0
zSHOW_ZKEY                          1
zSINGLE                             0x00000000L
zTASK_COMMAND                       2
zTASK_DIALOG                        1
zTEST_CSR_RESULT                    0x00010000L
zTES_NONE                           0
zTES_SUBOBJECT_CHILD                1
zTES_SUBOBJECT_ROOT                 3
zTES_TEMPORAL                       2
zTYPE_BLOB                          'B'
zTYPE_DATETIME                      'T'
zTYPE_DECIMAL                       'M'
zTYPE_INTEGER                       'L'
zTYPE_PIC                           'P'
zTYPE_STRING                        'S'
zUPD_INCLUDE                        1
zUPD_NONPERSIST                     2
zUSE_DEFAULT_CONTEXT                0x0008
zVALIDATE                           0x0004
zVAR_NULL                           -1
zVAR_SET                            0
//./ END + 1

//./ ADD NAME=ObjectConstraintHandler
// Source Module=kzoedoc.h
//***********************************************************************
//
// ENTRY:  ObjectConstraintHandler
//
// OPERATIONS:
//
//         Invoked by the following operations:
//             ActivateEmptyObjectInstance    (post)
//             ActivateOI_FromFile            (post)
//             ActivateObjectInstance         (post)
//             CommitOI_ToFile                (pre)
//             CommitObjectInstanct           (pre)
//             DropObjectInstance             (pre)
//             DropView                       (pre)
//             SfActivateSysEmptyOI           (post)
//             SfActivateSysOI_FromFile       (post)
//
// PARAMETERS:
//
// 1. zView -
//       Identifies the View of the activity that triggered the
//       constraint handler.
//
// 2. lEventType -
//       Indicates the type of activity that triggered the constraint
//       handler.
//
//       - zOCE_ACTIVATE
//       - zOCE_ACTIVATE_EMPTY
//             Constraint handler invoked after activation activities
//             have been completed successfully.  If the constraint
//             handler returns a non-zero return code, the view is
//             dropped and the return code is passed back to the
//             original caller.
//
//       - zOCE_COMMIT
//             Constraint handler invoked after parameter validation,
//             but before commit processing.  If the constraint
//             handler returns a non-zero return code, no commit
//             processing is performed.  The return code is passed back
//             to the original caller.
//
//       - zOCE_DROPOI
//             Constraint handler invoked after parameter validation,
//             but before dropOI processing.  DropOI processing will
//             continue no matter what is returned by the constraint
//             handler.
//
// 3. nState -
//       Indicates somthin or other...
//       - zOCE_STATE_FILE
//             Indicates that the activate or commit, is for a file.
//       - zOCE_STATE_NOI
//             Used only on activating a file.  Indicates that file could
//             not be found to open, but option was specified that said,
//             'okay to have no object instance'.
//       - zOCE_STATE_SHUTDOWN
//             Indicates that the task associated with the view, is in
//             the process of being shutdown.
//       - zOCE_STATE_SYSTEM
//             The activate will associate this view with the system
//             task.
//
// RETURNS:
//                   0 - Constraint handler says okay.
//   zCONSTRAINT_ERROR - Constraint handler detected an error situation.
//         zCALL_ERROR - Operation failed.
//
//***********************************************************************
//./ END + 4
int OPERATION
ObjectConstraintHandler( zVIEW   zView,
                         zSHORT  nEvent,
                         zSHORT  nState )

//./ ADD NAME=EntityConstraintHandler
// Source Module=kzoedoc.h
//***********************************************************************
//
// ENTRY:  EntityConstraintHandler
//
// OPERATIONS:
//
//         Invoked by the following operations:
//             AcceptSubobject                 (pre)
//             CancelSubobject                 (pre)
//             CreateEntity                    (post)
//             CreateTemporalEntity            (post)
//             DeleteEntity                    (pre)
//             ExcludeEntity                   (pre)
//             IncludeSubobjectFromSubobject   (post)
//
// PARAMETERS:
//
// 1. zView -
//       Identifies the View of the event that triggered the
//       constraint handler.
//
// 2. szEntityName -
//       EntityName which has the constraint.
//
// 3. nEvent -
//       Indicates the type of event that triggered the constraint
//       handler.
//       - zECE_ACCEPT
//             Constraint handler invoked after parameter validation,
//             but before accept processing.  If the constraint
//             handler returns a non-zero return code, no accept
//             processing is performed.  The return code is passed back
//             to the original caller.
//
//       - zECE_CANCEL
//             Constraint handler invoked after parameter validation,
//             but before cancel processing.  If the constraint
//             handler returns a non-zero return code, no cancel
//             processing is performed.  The return code is passed back
//             to the original caller.
//
//       - zECE_CREATE
//             Constraint handler invoked after create activities
//             have been completed successfully.  If the constraint
//             handler returns a non-zero return code, the entity is
//             deleted, the cursor positions restored  and the return
//             code is passed back to the original caller.
//
//       - zECE_DELETE
//             Constraint handler invoked after parameter validation,
//             but before delete processing.  If the constraint
//             handler returns a non-zero return code, no delete
//             processing is performed.  The return code is passed back
//             to the original caller.
//
//       - zECE_EXCLUDE
//             Constraint handler invoked after parameter validation,
//             but before exclude processing.  If the constraint
//             handler returns a non-zero return code, no exclude
//             processing is performed.  The return code is passed back
//             to the original caller.
//
//       - zECE_INCLUDE
//             Constraint handler invoked after include activities
//             have been completed successfully.  If the constraint
//             handler returns a non-zero return code, the entity is
//             excluded, the cursor positions restored  and the return
//             code is passed back to the original caller.
//
// 4. nState -
//       0 for now.
//
// RETURNS:
//                   0 - Constraint handler says okay.
//   zCONSTRAINT_ERROR - Constraint handler detected an error situation.
//         zCALL_ERROR - Operation failed.
//
//***********************************************************************
//./ END + 5
int OPERATION
EntityConstraintHandler( zVIEW   zView,
                         zCPCHAR szEntityName,
                         zSHORT  nEvent,
                         zSHORT  nState )

//./ ADD NAME=DBHandlerOper
// Source Module=kzoedoc.h
//***********************************************************************
//
// ENTRY:  DBHandlerOper
//
// OPERATIONS:
//
//      DBH_Init           0x1000         // Function messages...
//      DBH_Term           0x2000
//      DBH_Object         0x4000
//      DBH_Entity         0x8000
//
//      DBH_TranBegin      0x0100         // Operation msgs for Object...
//      DBH_Commit         0x0200
//      DBH_Rollback       0x0400
//
//      DBH_Insert         0x0001         // Operation msgs for Entity...
//      DBH_InsertRel      0x0002
//      DBH_Delete         0x0004
//      DBH_DeleteRel      0x0008
//      DBH_Update         0x0010
//      DBH_Load           0x0020
//      DBH_StartGenkey    0x0040
//      DBH_GetGenkey      0x0080
//
//      DBH_LoadOI         0x0001         // DB handler indicators...
//      DBH_StoreOI        0x0002
//
//      zSQL_SQLBASE               'B'
//      zSQL_SQLSERVER             'S'
//      zDBH_RAIMADM               'R'
//      zDBHANDLER_WORK_VIEW_NAME  "Zeidon DBHandler Work Object"
//
//      zDBH_ONE_TO_MANY           'O'
//      zDBH_MANY_TO_ONE           'M'
//      zDBH_MANY_TO_MANY          '2'
//
//      zDBH_PARENT_IS_OWNER       'O'
//      zDBH_PARENT_IS_MEMBER      'M'
//
//      zDBH_PARENT_IS_SOURCE      'O'
//      zDBH_CHILD_IS_SOURCE       'M'
//
// PARAMETERS:
//
// 1. nMessage -
//
// 2. nIndicators -
//
// 3. lpViewOD -
//
// 4. lpViewEntity -
//
// 5. lpView -
//
// 6. lpQualView -
//
// 7. lpPtr -
//
// RETURNS:
//             0 - Operation successful
//   zCALL_ERROR - Operation failed
//
//
//***********************************************************************
//./ END + 8
zSHORT OPERATION
DBHandlerOper( zSHORT       nMessage,
               int          iIndicators,
               LPVIEWOD     lpViewOD,
               LPVIEWENTITY lpViewEntity,
               zVIEW        lpView,
               zVIEW        lpQualView,
               zPVOID       lpPtr )
