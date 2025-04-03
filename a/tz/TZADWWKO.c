#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "TZVMLIP.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZADWWKO_ReturnAttrControlType( zVIEW     AnyView,
                                 zPCHAR    szDomainName,
                                 zPCHAR    szDomainType,
                                 zPCHAR    szControlType,
                                 zPCHAR    szUpdateFlag );


//:TRANSFORMATION OPERATION
//:AddAD_GoExclActions( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                     VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                     VIEW AD_Base   BASED ON LOD TZWDLGSO )

//:   VIEW TaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_GoExclActions( zVIEW     TZADWWKO,
                               zVIEW     TZWINDOWL,
                               zVIEW     AD_Base )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szSourceOperationName
   zCHAR     szSourceOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTargetOperationName
   zCHAR     szTargetOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szEntityName
   zCHAR     szEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szCompare
   zCHAR     szCompare[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceFileDirectory
   zCHAR     szSourceFileDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTargetFileDirectory
   zCHAR     szTargetFileDirectory[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength 
   zLONG     nLength = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_ListInclActions( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     TZWINDOWL,
                                 zVIEW     AD_BaseCtl,
                                 zVIEW     AD_Base )
{
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 255 ]; 
   zSHORT    lTempInteger_1; 

   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddAD_ListMGP_Actions( VIEW TZADWWKO   BASED ON LOD  TZADWWKO,
//:                       VIEW TZCONTROL  BASED ON LOD  TZWDLGSO,
//:                       VIEW TZWINDOWL  BASED ON LOD  TZWDLGSO,
//:                       VIEW AD_BaseCtl BASED ON LOD  TZWDLGSO,
//:                       VIEW AD_Base    BASED ON LOD  TZWDLGSO )

//:   VIEW TaskLPLR    REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_ListMGP_Actions( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     TZWINDOWL,
                                 zVIEW     AD_BaseCtl,
                                 zVIEW     AD_Base )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKOSub BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKOSub = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompare
   zCHAR     szCompare[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_MainV
   zCHAR     szAD_MainV[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_ListE
   zCHAR     szAD_ListE[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_MGP
   zCHAR     szAD_MGP[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_MGPO
   zCHAR     szAD_MGPO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_MGPRoot
   zCHAR     szAD_MGPRoot[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength  
   zLONG     nLength = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_0; 

   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddAD_ListUpdActions( VIEW TZADWWKO   BASED ON LOD  TZADWWKO,
//:                      VIEW TZWINDOWL  BASED ON LOD  TZWDLGSO,
//:                      VIEW AD_Base    BASED ON LOD  TZWDLGSO,
//:                      STRING ( 50 ) szGroupType )

//:   VIEW TaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_ListUpdActions( zVIEW     TZADWWKO,
                                zVIEW     TZWINDOWL,
                                zVIEW     AD_Base,
                                zPCHAR    szGroupType )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szSourceOperationName
   zCHAR     szSourceOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTargetOperationName
   zCHAR     szTargetOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szEntityName
   zCHAR     szEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szEntityName2  
   zCHAR     szEntityName2[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 6 )   szCompareOption
   zCHAR     szCompareOption[ 7 ] = { 0 }; 
   //:STRING ( 256 ) szCompare
   zCHAR     szCompare[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceFileDirectory
   zCHAR     szSourceFileDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTargetFileDirectory
   zCHAR     szTargetFileDirectory[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength 
   zLONG     nLength = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_3[ 33 ]; 

 
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddFindRangeInitValues( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                        VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                        INTEGER nFileIn,
//:                        INTEGER nFileOut )

//:   INTEGER        nRC
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddFindRangeInitValues( zVIEW     TZADWWKO,
                                  zVIEW     TZWINDOW,
                                  zLONG     nFileIn,
                                  zLONG     nFileOut )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddListMapping( VIEW TZADWWKO   BASED ON LOD TZADWWKO,
//:                VIEW TZCONTROL  BASED ON LOD TZWDLGSO,
//:                VIEW TZWINDOWL  BASED ON LOD TZWDLGSO,
//:                VIEW MappingLOD BASED ON LOD TZZOLODO,
//:                STRING ( 50 ) szGridMappingEntityName,
//:                STRING ( 50 ) szGridMappingViewName,
//:                STRING ( 10 ) szMappingPrefix )

//:   VIEW TZPESRCO  REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddListMapping( zVIEW     TZADWWKO,
                          zVIEW     TZCONTROL,
                          zVIEW     TZWINDOWL,
                          zVIEW     MappingLOD,
                          zPCHAR    szGridMappingEntityName,
                          zPCHAR    szGridMappingViewName,
                          zPCHAR    szMappingPrefix )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddRegisteredViewName( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                       VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                       VIEW TZZOLODO BASED ON LOD TZZOLODO,
//:                       STRING ( 32 ) szObjectViewName )

//:   VIEW TZWDVORO BASED ON LOD TZWDVORO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddRegisteredViewName( zVIEW     TZADWWKO,
                                 zVIEW     TZWINDOW,
                                 zVIEW     TZZOLODO,
                                 zPCHAR    szObjectViewName )
{

   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AutodesignFindPages( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                     VIEW ViewToWindow,
//:                     STRING ( 50 ) szFindType )

//:   VIEW TZADWWKOT   REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AutodesignFindPages( zVIEW     TZADWWKO,
                               zVIEW     ViewToWindow,
                               zPCHAR    szFindType )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:BuildAD_GroupPotList( VIEW TZADWWKO    BASED ON LOD TZADWWKO,
//:                      VIEW SelectedLOD BASED ON LOD TZZOLODO,
//:                      STRING ( 32 ) szStartEntityName,
//:                      STRING ( 1 ) szFlatListNameSuffix )

//:   VIEW RecursiveLOD BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_BuildAD_GroupPotList( zVIEW     TZADWWKO,
                                zVIEW     SelectedLOD,
                                zPCHAR    szStartEntityName,
                                zPCHAR    szFlatListNameSuffix )
{
   return( 0 );
//    
// END
} 


//:TRANSFORMATION OPERATION
//:BuildAD_GroupPotListR( VIEW TZADWWKO     BASED ON LOD TZADWWKO,
//:                       VIEW MainLOD      BASED ON LOD TZZOLODO,
//:                       VIEW RecursiveLOD BASED ON LOD TZZOLODO,
//:                       STRING ( 32 ) szStartEntityName,     // The Entity Name at which recursive processing starts
//:                       STRING ( 1 )  szFlatListNameSuffix,  // ESG_FlatListPotentialAttribute suffix as explained below
//:                       INTEGER       lCurrentLevel,         // The current recursive level of the LOD_ParentEntity
//:                       INTEGER       lStartLevel,           // The recursive level at which attributes will be copied
//:                       STRING ( 50 ) szIndentSpaces )       // Spaces for indenting the name shown in the list. They begin with lStartLevel

//:   STRING ( 80 ) szIndentEntityName
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_BuildAD_GroupPotListR( zVIEW     TZADWWKO,
                                 zVIEW     MainLOD,
                                 zVIEW     RecursiveLOD,
                                 zPCHAR    szStartEntityName,
                                 zPCHAR    szFlatListNameSuffix,
                                 zLONG     lCurrentLevel,
                                 zLONG     lStartLevel,
                                 zPCHAR    szIndentSpaces )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CloneControlAD( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                VIEW TZCONTROL  BASED ON LOD TZWDLGSO,
//:                VIEW TZWINDOWL  BASED ON LOD TZWDLGSO,
//:                VIEW AD_BaseCtl BASED ON LOD TZWDLGSO )

//:   VIEW TZPESRCO REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_CloneControlAD( zVIEW     TZADWWKO,
                          zVIEW     TZCONTROL,
                          zVIEW     TZWINDOWL,
                          zVIEW     AD_BaseCtl )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CloneSimpleControl( VIEW TZADWWKO    BASED ON LOD TZADWWKO,
//:                    VIEW vControlSrc BASED ON LOD TZWDLGSO,
//:                    VIEW vControlTgt BASED ON LOD TZWDLGSO )

//:   VIEW TZPESRCO   REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_CloneSimpleControl( zVIEW     TZADWWKO,
                              zVIEW     vControlSrc,
                              zVIEW     vControlTgt )
{

   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ConvertHeaderText( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                   VIEW TZCONTROL BASED ON LOD TZWDLGSO )

//:   VIEW TZCtlTemp BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_ConvertHeaderText( zVIEW     TZADWWKO,
                             zVIEW     TZCONTROL )
{

   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DeleteActionOperation( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                       STRING ( 32 ) szActionName,
//:                       STRING ( 32 ) szOperationName )

//:   VIEW TZCtlTemp BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_DeleteActionOperation( zVIEW     TZADWWKO,
                                 zVIEW     TZWINDOWL,
                                 zPCHAR    szActionName,
                                 zPCHAR    szOperationName )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DeleteControlByName( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                     VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                     STRING ( 32 ) szControlName )

//:   VIEW TZCtlTemp BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_DeleteControlByName( zVIEW     TZADWWKO,
                               zVIEW     TZWINDOWL,
                               zPCHAR    szControlName )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DeleteOperationVML( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                    STRING ( 32 )  szOperationName,
//:                    STRING ( 200 ) szVML_DirectoryFileName )

//:   STRING ( 256 ) szVML_Statement
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_DeleteOperationVML( zVIEW     TZADWWKO,
                              zPCHAR    szOperationName,
                              zPCHAR    szVML_DirectoryFileName )
{

   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenAD_GroupNames( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                  VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                  STRING ( 20 ) szGroupArea,
//:                  STRING ( 32 ) szEntityName,
//:                  INTEGER nLevel )

//:   STRING ( 1 ) szLevel
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenAD_GroupNames( zVIEW     TZADWWKO,
                            zVIEW     TZCONTROL,
                            zPCHAR    szGroupArea,
                            zPCHAR    szEntityName,
                            zLONG     nLevel )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenDetailGrpBootstrap( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                       VIEW TZCURWND  BASED ON LOD TZWDLGSO,
//:                       STRING ( 20 ) szType )
//:                       

//:   VIEW TZPESRCO   REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenDetailGrpBootstrap( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     TZCURWND,
                                 zPCHAR    szType )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenDetailGrpBootstrpG( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                       VIEW UpdateLOD BASED ON LOD TZZOLODO,
//:                       STRING ( 20 ) szType )

//:   VIEW TZPESRCO   REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenDetailGrpBootstrpG( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     UpdateLOD,
                                 zPCHAR    szType )
{
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenDetailGrpBootstrpR( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                       VIEW UpdateLOD BASED ON LOD TZZOLODO,
//:                       STRING ( 20 ) szType )

//:   VIEW TZPESRCO   REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenDetailGrpBootstrpR( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     UpdateLOD,
                                 zPCHAR    szType )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOW   REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW AD_Base    BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW TZCONTROL2 BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL2 = 0; 
   //:VIEW TZWDVORO   BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:STRING ( 50 ) szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szPromptControlName
   zCHAR     szPromptControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szDataControlName
   zCHAR     szDataControlName[ 51 ] = { 0 }; 
   //:STRING ( 10 ) szCurrentRow
   zCHAR     szCurrentRow[ 11 ] = { 0 }; 
   //:STRING ( 10 ) szCurrentPair
   zCHAR     szCurrentPair[ 11 ] = { 0 }; 
   //:INTEGER CurrentRow
   zLONG     CurrentRow = 0; 
   //:INTEGER LastRow
   zLONG     LastRow = 0; 
   //:INTEGER RowCount
   zLONG     RowCount = 0; 
   //:INTEGER DetailPairCount
   zLONG     DetailPairCount = 0; 
   //:INTEGER NewYPosition
   zLONG     NewYPosition = 0; 
   //:INTEGER NewXPosition
   zLONG     NewXPosition = 0; 
   //:INTEGER GroupSizeY
   zLONG     GroupSizeY = 0; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 255 ]; 
   zCHAR     szTempString_4[ 255 ]; 
   zCHAR     szTempString_5[ 255 ]; 
   zCHAR     szTempString_6[ 255 ]; 
   zCHAR     szTempString_7[ 255 ]; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zCHAR     szTempString_8[ 33 ]; 
   zLONG     lTempInteger_8; 
   zLONG     lTempInteger_9; 
   zLONG     lTempInteger_10; 
   zLONG     lTempInteger_11; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", TZADWWKO, zLEVEL_TASK );

   //:// Generate a Detail Group of Prompt / Mapping Control for Bootstrap from TZADWWKO.DetailMappingLOD_Attribute entries..
   //:// We will effectively loop through all uniques rows and then the Prompt/Mapping Control pairs within them.

   //:// Default any null Row values so that they appear on next row from the last.
   //:LastRow = 0
   LastRow = 0;
   //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADWWKO.DetailMappingLOD_Attribute.GroupRow = ""
      if ( CompareAttributeToString( TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow", "" ) == 0 )
      { 
         //:LastRow = LastRow + 1
         LastRow = LastRow + 1;
         //:TZADWWKO.DetailMappingLOD_Attribute.GroupRow = LastRow
         SetAttributeFromInteger( TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow", LastRow );
         //:ELSE
      } 
      else
      { 
         //:LastRow = TZADWWKO.DetailMappingLOD_Attribute.GroupRow
         GetIntegerFromAttribute( &LastRow, TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow" );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      //:END
   } 

   //:END

   //:// Loop through all DetailMappingLOD_Attribute entries creating Bootstrap Prompt/Mapping pairs for each.card-body
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:LastRow         = 0
   LastRow = 0;
   //:RowCount        = 0
   RowCount = 0;
   //:DetailPairCount = 0
   DetailPairCount = 0;
   //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CurrentRow = TZADWWKO.DetailMappingLOD_Attribute.GroupRow 
      GetIntegerFromAttribute( &CurrentRow, TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow" );
      //:IF CurrentRow != LastRow
      if ( CurrentRow != LastRow )
      { 

         //:IF CurrentRow > 1
         if ( CurrentRow > 1 )
         { 
            //:// If this is any Row after the first, we need to reset the view from the Detail Pair Group.
            //:ResetViewFromSubobject( TZCONTROL )
            ResetViewFromSubobject( TZCONTROL );
         } 

         //:END

         //:// To determine new Y position, add last Y length to last Y position plus increment.
         //:NewYPosition = TZCONTROL.Control.PSDLG_Y + TZCONTROL.Control.SZDLG_Y + 6
         GetIntegerFromAttribute( &lTempInteger_0, TZCONTROL, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lTempInteger_1, TZCONTROL, "Control", "SZDLG_Y" );
         NewYPosition = lTempInteger_0 + lTempInteger_1 + 6;
         //:NewXPosition = 3
         NewXPosition = 3;

         //:// It's a new row, so create the Row GroupBox.
         //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
         CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
         //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "GroupBox"
         RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "GroupBox", "" );
         //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
         RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
         //:zIntegerToString( szCurrentRow, 10, CurrentRow )
         zIntegerToString( szCurrentRow, 10, CurrentRow );
         //:TZCONTROL.Control.Tag       = "Row" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentRow 
         GetVariableFromAttribute( szTempString_1, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
         ZeidonStringCopy( szTempString_0, 1, 0, "Row", 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, szCurrentRow, 1, 0, 33 );
         SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
         //:TZCONTROL.Control.CSS_Class = "row"
         SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "row" );
         //:TZCONTROL.Control.PSDLG_X   = 4
         SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", 4 );
         //:TZCONTROL.Control.PSDLG_Y   = NewYPosition
         SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", NewYPosition );
         //:TZCONTROL.Control.SZDLG_X   = 480
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 480 );
         //:TZCONTROL.Control.SZDLG_Y   = 30
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 30 );
         //:LastRow = CurrentRow
         LastRow = CurrentRow;
         //:DetailPairCount = 0
         DetailPairCount = 0;

         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )    // Step down to Pair level.
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      } 

      //:END
      //:DetailPairCount = DetailPairCount + 1
      DetailPairCount = DetailPairCount + 1;
      //:zIntegerToString( szCurrentPair, 10, DetailPairCount )
      zIntegerToString( szCurrentPair, 10, DetailPairCount );

      //:// Create GroupBox for Detail Pair
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "GroupBox"
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "GroupBox", "" );
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
      //:TZCONTROL.Control.Tag       = "Pair" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentRow + szCurrentPair
      GetVariableFromAttribute( szTempString_3, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      ZeidonStringCopy( szTempString_2, 1, 0, "Pair", 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szTempString_3, 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szCurrentRow, 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szCurrentPair, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_2 );
      //:TZCONTROL.Control.CSS_Class = "col-4"
      SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "col-4" );
      //:TZCONTROL.Control.PSDLG_X = NewXPosition
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", NewXPosition );
      //:TZCONTROL.Control.PSDLG_Y = 4
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 4 );
      //:TZCONTROL.Control.SZDLG_X = 150
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 150 );
      //:TZCONTROL.Control.SZDLG_Y = 21
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 21 );
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )    // Step down to Detail Pair GroupBox level.
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );

      //:// Generate Prompt/Detail Control Names 
      //:zIntegerToString( szCurrentRow, 10, CurrentRow )
      zIntegerToString( szCurrentRow, 10, CurrentRow );
      //:zIntegerToString( szCurrentPair, 10, DetailPairCount )
      zIntegerToString( szCurrentPair, 10, DetailPairCount );
      //:szPromptControlName = "P_" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentRow + szCurrentPair
      GetVariableFromAttribute( szTempString_4, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      ZeidonStringCopy( szPromptControlName, 1, 0, "P_", 1, 0, 51 );
      ZeidonStringConcat( szPromptControlName, 1, 0, szTempString_4, 1, 0, 51 );
      ZeidonStringConcat( szPromptControlName, 1, 0, szCurrentRow, 1, 0, 51 );
      ZeidonStringConcat( szPromptControlName, 1, 0, szCurrentPair, 1, 0, 51 );
      //:szDataControlName   = "M_" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentRow + szCurrentPair
      GetVariableFromAttribute( szTempString_5, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      ZeidonStringCopy( szDataControlName, 1, 0, "M_", 1, 0, 51 );
      ZeidonStringConcat( szDataControlName, 1, 0, szTempString_5, 1, 0, 51 );
      ZeidonStringConcat( szDataControlName, 1, 0, szCurrentRow, 1, 0, 51 );
      ZeidonStringConcat( szDataControlName, 1, 0, szCurrentPair, 1, 0, 51 );
      //:TraceLineS( "*** Prompt Group: ", TZADWWKO.EntitySubGroup.GroupAreaName )
      GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), TZADWWKO, "EntitySubGroup", "GroupAreaName" );
      TraceLineS( "*** Prompt Group: ", szTempString_6 );

      //:// Create Prompt 
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "Text"
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "Text", "" );
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
      //:TZCONTROL.Control.Text              = TZADWWKO.DetailMappingLOD_Attribute.PromptValue 
      SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZADWWKO, "DetailMappingLOD_Attribute", "PromptValue" );
      //:TZCONTROL.Control.Tag               = szPromptControlName
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szPromptControlName );
      //:TZCONTROL.Control.CSS_Class         = "input-group" 
      SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "input-group" );
      //:TZCONTROL.Control.WebHTML5Attribute = "style="+ QUOTES + "min-width: 112px"+ QUOTES
      ZeidonStringCopy( szTempString_7, 1, 0, "style=", 1, 0, 255 );
      ZeidonStringConcat( szTempString_7, 1, 0, QUOTES, 1, 0, 255 );
      ZeidonStringConcat( szTempString_7, 1, 0, "min-width: 112px", 1, 0, 255 );
      ZeidonStringConcat( szTempString_7, 1, 0, QUOTES, 1, 0, 255 );
      SetAttributeFromString( TZCONTROL, "Control", "WebHTML5Attribute", szTempString_7 );
      //:TZCONTROL.Control.WebCtrlLabelLink  = szDataControlName
      SetAttributeFromString( TZCONTROL, "Control", "WebCtrlLabelLink", szDataControlName );
      //:TZCONTROL.Control.PSDLG_X           = 5
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", 5 );
      //:TZCONTROL.Control.PSDLG_Y           = 7
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 7 );
      //:TZCONTROL.Control.SZDLG_X           = 60
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 60 );
      //:TZCONTROL.Control.SZDLG_Y           = 11
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 11 );

      //:// Create Detail entry 
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
      //:// Control Type depends on DetailMappingLOD_Attribute setting.
      //:// Also, in Bootstrap a Calendar is treated as an EditBox with special CSS_Class & HTML5 values.
      //:szControlType = TZADWWKO.DetailMappingLOD_Attribute.ControlType 
      GetVariableFromAttribute( szControlType, 0, 'S', 51, TZADWWKO, "DetailMappingLOD_Attribute", "ControlType", "", 0 );
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = szControlType
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szControlType, "" );

      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
      //:TZCONTROL.Control.Tag     = szDataControlName
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szDataControlName );
      //:TZCONTROL.Control.PSDLG_X = 65
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", 65 );
      //:TZCONTROL.Control.PSDLG_Y = 7
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 7 );
      //:TZCONTROL.Control.SZDLG_X = 70
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 70 );
      //:TZCONTROL.Control.SZDLG_Y = 11
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 11 );

      //:// If the Control is an MLEdit, increase both X and Y size of the Control and the 2 outer Groupboxes.
      //:IF szControlType = "MLEdit"  
      if ( ZeidonStringCompare( szControlType, 1, 0, "MLEdit", 1, 0, 51 ) == 0 )
      { 
         //:TZCONTROL.Control.SZDLG_X   = 140        // Increase Control X size by 70.
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 140 );
         //:TZCONTROL.Control.SZDLG_Y   = 22         // Increase Control Y size by 11.
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 22 );
         //:TZCONTROL.Control.CSS_Class = "col-12"   // Force Control onto next whole line.
         SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "col-12" );
         //:CreateViewFromView( TZCONTROL2, TZCONTROL )
         CreateViewFromView( &TZCONTROL2, TZCONTROL );
         //:ResetViewFromSubobject( TZCONTROL2 )  // Back up to Prompt/Control pair.
         ResetViewFromSubobject( TZCONTROL2 );
         //:TZCONTROL2.Control.SZDLG_X = TZCONTROL2.Control.SZDLG_X + 70   // Increase Control X size by 70.
         GetIntegerFromAttribute( &lTempInteger_2, TZCONTROL2, "Control", "SZDLG_X" );
         lTempInteger_3 = lTempInteger_2 + 70;
         SetAttributeFromInteger( TZCONTROL2, "Control", "SZDLG_X", lTempInteger_3 );
         //:TZCONTROL2.Control.SZDLG_Y = TZCONTROL2.Control.SZDLG_Y + 11   // Increase Control Y size by 11.
         GetIntegerFromAttribute( &lTempInteger_4, TZCONTROL2, "Control", "SZDLG_Y" );
         lTempInteger_5 = lTempInteger_4 + 11;
         SetAttributeFromInteger( TZCONTROL2, "Control", "SZDLG_Y", lTempInteger_5 );
         //:ResetViewFromSubobject( TZCONTROL2 )  // Back up to Row Groupbox and only increase Y size.
         ResetViewFromSubobject( TZCONTROL2 );
         //:TZCONTROL2.Control.SZDLG_Y = TZCONTROL2.Control.SZDLG_Y + 11   // Increase Control Y size by 11.
         GetIntegerFromAttribute( &lTempInteger_6, TZCONTROL2, "Control", "SZDLG_Y" );
         lTempInteger_7 = lTempInteger_6 + 11;
         SetAttributeFromInteger( TZCONTROL2, "Control", "SZDLG_Y", lTempInteger_7 );
         //:DropView( TZCONTROL2 )
         DropView( TZCONTROL2 );
      } 

      //:END

      //:// If the Control is a CheckBox, use parameters from the Checkbox definition from  AD_Base ControlTemplates.
      //:IF szControlType = "CheckBox"
      if ( ZeidonStringCompare( szControlType, 1, 0, "CheckBox", 1, 0, 51 ) == 0 )
      { 
         //:GET VIEW AD_Base NAMED "AD_Base"
         RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
         //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ControlTemplates"
         RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ControlTemplates", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZADWWKO,0,0, "AD_Base Window 'ControlTemplates' is missing." )
            IssueError( TZADWWKO, 0, 0, "AD_Base Window 'ControlTemplates' is missing." );
            //:RETURN 2
            return( 2 );
         } 

         //:END
         //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "CheckBox"
         RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "CheckBox", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZADWWKO,0,0, "AD_Base 'CheckBox' template is missing." )
            IssueError( TZADWWKO, 0, 0, "AD_Base 'CheckBox' template is missing." );
            //:RETURN 2
            return( 2 );
         } 

         //:END
         //:TZCONTROL.Control.CtrlBOI              = AD_Base.CtrlCtrl.CtrlBOI 
         SetAttributeFromAttribute( TZCONTROL, "Control", "CtrlBOI", AD_Base, "CtrlCtrl", "CtrlBOI" );
         //:TZCONTROL.Control.Subtype              = AD_Base.CtrlCtrl.Subtype 
         SetAttributeFromAttribute( TZCONTROL, "Control", "Subtype", AD_Base, "CtrlCtrl", "Subtype" );
         //:TZCONTROL.Control.ExtendedStyle        = AD_Base.CtrlCtrl.ExtendedStyle 
         SetAttributeFromAttribute( TZCONTROL, "Control", "ExtendedStyle", AD_Base, "CtrlCtrl", "ExtendedStyle" );
         //:TZCONTROL.Control.RadioOrCheckboxValue = AD_Base.CtrlCtrl.RadioOrCheckboxValue 
         SetAttributeFromAttribute( TZCONTROL, "Control", "RadioOrCheckboxValue", AD_Base, "CtrlCtrl", "RadioOrCheckboxValue" );
      } 

      //:END

      //:// If the Control is a Calendar, use parameters from the Calendar definition, "CurrentCalendar", from  AD_Base ControlTemplates.
      //:IF szControlType = "Calendar"
      if ( ZeidonStringCompare( szControlType, 1, 0, "Calendar", 1, 0, 51 ) == 0 )
      { 
         //:GET VIEW AD_Base NAMED "AD_Base"
         RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
         //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ControlTemplates"
         RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ControlTemplates", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZADWWKO,0,0, "AD_Base Window 'ControlTemplates' is missing." )
            IssueError( TZADWWKO, 0, 0, "AD_Base Window 'ControlTemplates' is missing." );
            //:RETURN 2
            return( 2 );
         } 

         //:END
         //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "CurrentCalendar"
         RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "CurrentCalendar", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZADWWKO,0,0, "AD_Base 'CurrentCalendar' template is missing." )
            IssueError( TZADWWKO, 0, 0, "AD_Base 'CurrentCalendar' template is missing." );
            //:RETURN 2
            return( 2 );
         } 

         //:END
         //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
         SetViewToSubobject( AD_Base, "CtrlCtrl" );
         //:IF TZCONTROL.ControlDef.Tag != AD_Base.ControlDef.Tag
         if ( CompareAttributeToAttribute( TZCONTROL, "ControlDef", "Tag", AD_Base, "ControlDef", "Tag" ) != 0 )
         { 
            //:EXCLUDE TZCONTROL.ControlDef NONE
            RESULT = ExcludeEntity( TZCONTROL, "ControlDef", zREPOS_NONE );
            //:INCLUDE TZCONTROL.ControlDef FROM AD_Base.ControlDef
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", AD_Base, "ControlDef", zPOS_AFTER );
         } 

         //:END
         //:TZCONTROL.Control.ExtendedStyle     = AD_Base.Control.ExtendedStyle 
         SetAttributeFromAttribute( TZCONTROL, "Control", "ExtendedStyle", AD_Base, "Control", "ExtendedStyle" );
         //:TZCONTROL.Control.CSS_Class         = AD_Base.Control.CSS_Class 
         SetAttributeFromAttribute( TZCONTROL, "Control", "CSS_Class", AD_Base, "Control", "CSS_Class" );
         //:TZCONTROL.Control.WebHTML5Attribute = AD_Base.Control.WebHTML5Attribute 
         SetAttributeFromAttribute( TZCONTROL, "Control", "WebHTML5Attribute", AD_Base, "Control", "WebHTML5Attribute" );
         //:ResetViewFromSubobject( AD_Base )
         ResetViewFromSubobject( AD_Base );
      } 

      //:END

      //:// Create Mapping Values.
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "CtrlMap", zPOS_AFTER )  
      CreateMetaEntity( TZADWWKO, TZCONTROL, "CtrlMap", zPOS_AFTER );
      //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOW.ViewObjRef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );
      //:SET CURSOR FIRST UpdateLOD.LOD_Entity WHERE UpdateLOD.LOD_Entity.Name   = TZADWWKO.DetailMappingLOD_Entity.Name 
      GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), TZADWWKO, "DetailMappingLOD_Entity", "Name" );
      RESULT = SetCursorFirstEntityByString( UpdateLOD, "LOD_Entity", "Name", szTempString_8, "" );
      //:SET CURSOR FIRST UpdateLOD.ER_Attribute WITHIN UpdateLOD.LOD_Entity 
      //:                                      WHERE UpdateLOD.ER_Attribute.Name = TZADWWKO.DetailMappingER_Attribute.Name 
      GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), TZADWWKO, "DetailMappingER_Attribute", "Name" );
      RESULT = SetCursorFirstEntityByString( UpdateLOD, "ER_Attribute", "Name", szTempString_8, "LOD_Entity" );
      //:IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", UpdateLOD, "LOD_Attribute", zPOS_AFTER )
      IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", UpdateLOD, "LOD_Attribute", zPOS_AFTER );

      //:ResetViewFromSubobject( TZCONTROL )    // Reset back to the Detail Pair GroupBox level.
      ResetViewFromSubobject( TZCONTROL );

      //:// Set up the next X starting position in case the next pair is in the same Row.
      //:NewXPosition = TZCONTROL.Control.PSDLG_X + TZCONTROL.Control.SZDLG_X + 3
      GetIntegerFromAttribute( &lTempInteger_8, TZCONTROL, "Control", "PSDLG_X" );
      GetIntegerFromAttribute( &lTempInteger_9, TZCONTROL, "Control", "SZDLG_X" );
      NewXPosition = lTempInteger_8 + lTempInteger_9 + 3;
      RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   } 

   //:END
   //:ResetViewFromSubobject( TZCONTROL )   // Reset back to Row.
   ResetViewFromSubobject( TZCONTROL );

   //:// Determine Outer Group Y size from last Row.
   //:GroupSizeY = TZCONTROL.Control.PSDLG_Y + TZCONTROL.Control.SZDLG_Y + 6  
   GetIntegerFromAttribute( &lTempInteger_10, TZCONTROL, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_11, TZCONTROL, "Control", "SZDLG_Y" );
   GroupSizeY = lTempInteger_10 + lTempInteger_11 + 6;

   //:// Reset back to "card-body" GroupBox and set size.
   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   //:TZCONTROL.Control.SZDLG_Y = GroupSizeY
   SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", GroupSizeY );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenEntitySubGroup( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                   VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                   VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                   VIEW AD_Base   BASED ON LOD TZWDLGSO,
//:                   STRING ( 50 ) szGroupType,
//:                   STRING ( 50 ) szAD_Area )

//:   VIEW AD_BaseCtl  BASED ON LOD  TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenEntitySubGroup( zVIEW     TZADWWKO,
                             zVIEW     TZCONTROL,
                             zVIEW     TZWINDOWL,
                             zVIEW     AD_Base,
                             zPCHAR    szGroupType,
                             zPCHAR    szAD_Area )
{
   zVIEW     AD_BaseCtl = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW ReturnedLOD BASED ON LOD  TZZOLODO
   zVIEW     ReturnedLOD = 0; 
   //:VIEW TZCONTROL2  BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL2 = 0; 
   //:VIEW TZCtlHier   BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlHier = 0; 
   //:STRING ( 50 )  szLOD_ListEntityName
   zCHAR     szLOD_ListEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupName
   zCHAR     szGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateGroupName
   zCHAR     szTemplateGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedEntityName 
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateWindow
   zCHAR     szTemplateWindow[ 51 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   //:INTEGER lInitialLevel
   zLONG     lInitialLevel = 0; 
   //:INTEGER lReturnedLevel 
   zLONG     lReturnedLevel = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_2[ 33 ]; 


   //:// Generate an Entity Subgroup based on the Group Type passed in.
   //:// The structure we are generating is assumed to be Bootstrap.

   //:CreateViewFromView( AD_BaseCtl, AD_Base )
   CreateViewFromView( &AD_BaseCtl, AD_Base );
   //:NAME VIEW  AD_BaseCtl "AD_BaseCtl"
   SetNameForView( AD_BaseCtl, "AD_BaseCtl", 0, zLEVEL_TASK );
   //:IF TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Icons"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Icons" ) == 0 )
   { 
      //:szTemplateWindow = "ObjectUpdateGroupsIcons" 
      ZeidonStringCopy( szTemplateWindow, 1, 0, "ObjectUpdateGroupsIcons", 1, 0, 51 );
      //:ELSE
   } 
   else
   { 
      //:szTemplateWindow = "ObjectUpdateGroups"
      ZeidonStringCopy( szTemplateWindow, 1, 0, "ObjectUpdateGroups", 1, 0, 51 );
   } 

   //:END
   //:SET CURSOR FIRST AD_BaseCtl.Window WHERE AD_BaseCtl.Window.Tag = szTemplateWindow
   RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Window", "Tag", szTemplateWindow, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "The template window, " + szTemplateWindow + ", could not be found in AD_Base."
      ZeidonStringCopy( szMsg, 1, 0, "The template window, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szTemplateWindow, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ", could not be found in AD_Base.", 1, 0, 201 );
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", TZADWWKO, zLEVEL_TASK );

   //:IF szGroupType = "ListGroupOnly" OR 
   //:   szGroupType = "ListGroupWUpdatePage" OR 
   //:   szGroupType = "ListGroupWDetailPage" OR 
   //:   szGroupType = "ListGroupwMGP"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 ||
        ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 51 ) == 0 )
   { 

      //:// Position on correct AD_Base Group to copy.
      //:IF szGroupType = "ListGroupOnly"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 )
      { 
         //:szTemplateGroupName = "GroupListOnly"
         ZeidonStringCopy( szTemplateGroupName, 1, 0, "GroupListOnly", 1, 0, 51 );
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szTemplateGroupName
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szTemplateGroupName, "" );
         //:ELSE
      } 
      else
      { 
         //:IF szGroupType = "ListGroupWUpdatePage"
         if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 )
         { 
            //:szTemplateGroupName = "GroupUpdateList"
            ZeidonStringCopy( szTemplateGroupName, 1, 0, "GroupUpdateList", 1, 0, 51 );
            //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szTemplateGroupName
            RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szTemplateGroupName, "" );
            //:ELSE
         } 
         else
         { 
            //:IF szGroupType = "ListGroupWDetailPage"
            if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 )
            { 
               //:szTemplateGroupName = "GroupDetailList"
               ZeidonStringCopy( szTemplateGroupName, 1, 0, "GroupDetailList", 1, 0, 51 );
               //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szTemplateGroupName
               RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szTemplateGroupName, "" );
               //:ELSE
            } 
            else
            { 
               //:IF szGroupType = "ListGroupwMGP"
               if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 51 ) == 0 )
               { 
                  //:szTemplateGroupName = "GroupMGP_List"
                  ZeidonStringCopy( szTemplateGroupName, 1, 0, "GroupMGP_List", 1, 0, 51 );
                  //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szTemplateGroupName
                  RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szTemplateGroupName, "" );
                  //:ELSE
               } 
               else
               { 
                  //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
                  //:    "Invalid List Type.",
                  //:    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                  MessageSend( TZADWWKO, "", "Autodesign Subdialog", "Invalid List Type.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                  //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
                  SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
                  //:RETURN -2
                  return( -2 );
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:szMsg = szTemplateGroupName + " Template Control doesn't exist."
         ZeidonStringCopy( szMsg, 1, 0, szTemplateGroupName, 1, 0, 201 );
         ZeidonStringConcat( szMsg, 1, 0, " Template Control doesn't exist.", 1, 0, 201 );
         //:MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// Build the Grid Control with subcontrols.

      //:// First clone basic structure at the current position.
      //:nRC = CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl )
      nRC = oTZADWWKO_CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl );

      //:// Set top Group Tag and Text.
      //:szGroupName = TZADWWKO.EntitySubGroup.GroupAreaName
      GetVariableFromAttribute( szGroupName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      //:TZCONTROL.Control.Tag  = "Group" + szGroupName + "List"    // Used for debugging.
      ZeidonStringCopy( szTempString_0, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szGroupName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, "List", 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
      //:TZCONTROL.Control.Text = ""
      SetAttributeFromString( TZCONTROL, "Control", "Text", "" );

      //:// Set Control Start Y Position to 10. That will be the correct position if this is the first Group on
      //:// the page. If it's not the first Group, the position will be reset later.
      //:TZCONTROL.Control.PSDLG_Y = 10
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 10 );

      //:IF szGroupType = "ListGroupwMGP"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 51 ) == 0 )
      { 
         //:// Process the GOTO__AreaMGP Action and Operation for MGP.
         //:AddAD_ListMGP_Actions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
         oTZADWWKO_AddAD_ListMGP_Actions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );
         //:ELSE
      } 
      else
      { 
         //:// Process the Actions and Operations tied to the List Detail or Update Groups.
         //:AddAD_ListUpdActions( TZADWWKO, TZWINDOWL, AD_Base, szGroupType )
         oTZADWWKO_AddAD_ListUpdActions( TZADWWKO, TZWINDOWL, AD_Base, szGroupType );
      } 

      //:END

      //:// Build the list mapping entries for Grid. Note that the AddListMapping operation handles any parent Groups
      //:// that contain the Grid.
      //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity.Name
      GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
      //:GetStringFromAttribute( szViewName, UpdateLOD, "LOD", "Name" )   // View Name is same as LOD Name.
      GetStringFromAttribute( szViewName, zsizeof( szViewName ), UpdateLOD, "LOD", "Name" );
      //:AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" )
      oTZADWWKO_AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" );

      //:// Convert Header Data.
      //:ConvertHeaderText( TZADWWKO, TZCONTROL )
      oTZADWWKO_ConvertHeaderText( TZADWWKO, TZCONTROL );
   } 


   //:END

   //:// DETAIL GROUP

   //:IF szGroupType = "DetailGroup" OR szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroup", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 )
   { 

      //:// Position on correct AD_Base Group to copy.
      //:IF szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupDetailwInclude"
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupDetailwInclude", "" );
         //:ELSE
      } 
      else
      { 
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupDetail"
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupDetail", "" );
      } 

      //:END
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
         //:             "DetailGroupUpdate Control doesn't exist.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( TZADWWKO, "", "Autodesign Subdialog", "DetailGroupUpdate Control doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// Build the Detail control Group.

      //:// Because the GenDetailGrpBootstrap routine uses the DetailMapping suboject, we will copy the ESG_DetailLOD_Attribute subobject
      //:// entries to the DetailMapping suboject.
      //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:EXCLUDE TZADWWKO.DetailMappingLOD_Attribute  NONE 
         RESULT = ExcludeEntity( TZADWWKO, "DetailMappingLOD_Attribute", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      } 

      //:END
      //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:INCLUDE TZADWWKO.DetailMappingLOD_Attribute FROM TZADWWKO.ESG_DetailLOD_Attribute 
         RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zSET_ALL )  
         SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zSET_ALL );
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      } 

      //:END

      //:// Clone Control at current position.
      //:nRC = CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl )
      nRC = oTZADWWKO_CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl );

      //:// Set top Group Tag and Text.
      //:szGroupName = TZADWWKO.EntitySubGroup.GroupAreaName
      GetVariableFromAttribute( szGroupName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      //:TZCONTROL.Control.Tag  = "Group" + szGroupName + "Detail"    // Used for debugging.
      ZeidonStringCopy( szTempString_1, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, szGroupName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, "Detail", 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_1 );
      //:TZCONTROL.Control.Text = ""
      SetAttributeFromString( TZCONTROL, "Control", "Text", "" );

      //:// Set Control Start Y Position to 10. That will be the correct position if this is the first Group on
      //:// the page. If it's not the first Group, the position will be reset later.
      //:TZCONTROL.Control.PSDLG_Y = 10
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 10 );

      //:// After the Clone, we need to step into the inner-most and last subGroup.
      //:LOOP WHILE TZCONTROL.CtrlCtrl EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
      while ( lTempInteger_0 == 0 )
      { 
         //:nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
         nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:SET CURSOR LAST TZCONTROL.Control
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
      } 

      //:END

      //:GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "MultiGroup" )
      oTZADWWKO_GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "MultiGroup" );

      //:// If this is GroupDetailwInclude, alter the Select Action, GOTO_SelectDetail, with Area Name & current Dialog plus include Window name 
      //:// for the Include Window that will be generated later.
      //:IF szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 

         //:// Go to process GOTO and EXCLUDE Actions and Operations.
         //:AddAD_GoExclActions( TZADWWKO, TZWINDOWL, AD_Base )
         oTZADWWKO_AddAD_GoExclActions( TZADWWKO, TZWINDOWL, AD_Base );
      } 


      //:END
   } 

   //:END

   //:// LIST INCLUDE GROUP

   //:IF szGroupType = "ListGroupwInclude" OR szGroupType = "ListGroupwFindInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
   { 

      //:// Build Group List with transfer to Select/Include Page.
      //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupIncludeList"
      RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupIncludeList", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
         //:             "ListGroupWIncludePage Control doesn't exist.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( TZADWWKO, "", "Autodesign Subdialog", "ListGroupWIncludePage Control doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// Clone Control at current position.
      //:nRC = CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl )
      nRC = oTZADWWKO_CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl );

      //:// Set top Group Tag and Text.
      //:szGroupName = TZADWWKO.EntitySubGroup.GroupAreaName
      GetVariableFromAttribute( szGroupName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      //:TZCONTROL.Control.Tag  = "Group" + szGroupName + "Incl"     // Used for debugging.
      ZeidonStringCopy( szTempString_2, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szGroupName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, "Incl", 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_2 );
      //:TZCONTROL.Control.Text = ""
      SetAttributeFromString( TZCONTROL, "Control", "Text", "" );

      //:// Set Control Start Y Position to 10. That will be the correct position if this is the first Group on
      //:// the page. If it's not the first Group, the position will be reset later.
      //:TZCONTROL.Control.PSDLG_Y = 10
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 10 );

      //:// Process the 2 Actions tied to the New and Delete/Remove buttons.
      //:AddAD_ListInclActions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
      oTZADWWKO_AddAD_ListInclActions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );

      //:// Build the list mapping entries.
      //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity.Name
      GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
      //:TraceLineS( "*** Before AddListMapping List Group Include Update Page: ", szLOD_ListEntityName )
      TraceLineS( "*** Before AddListMapping List Group Include Update Page: ", szLOD_ListEntityName );
      //:GetStringFromAttribute( szViewName, UpdateLOD, "LOD", "Name" )   // View Name is same as LOD Name.
      GetStringFromAttribute( szViewName, zsizeof( szViewName ), UpdateLOD, "LOD", "Name" );
      //:AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" )
      oTZADWWKO_AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" );

      //:// Convert Header Data.
      //:ConvertHeaderText( TZADWWKO, TZCONTROL )
      oTZADWWKO_ConvertHeaderText( TZADWWKO, TZCONTROL );

      //:nRC = ResetViewFromSubobject( TZCONTROL )    // Make sure we reset view to the top.
      nRC = ResetViewFromSubobject( TZCONTROL );
      //:LOOP WHILE nRC = 0
      while ( nRC == 0 )
      { 
         //:nRC = ResetViewFromSubobject( TZCONTROL )
         nRC = ResetViewFromSubobject( TZCONTROL );
      } 

      //:END
   } 


   //:END

   //:DropView( AD_BaseCtl )
   DropView( AD_BaseCtl );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenIncludePage( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                VIEW AD_Base   BASED ON LOD TZWDLGSO,
//:                VIEW ViewToWindow )

//:   VIEW TaskLPLR    REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenIncludePage( zVIEW     TZADWWKO,
                          zVIEW     TZWINDOWL,
                          zVIEW     AD_Base,
                          zVIEW     ViewToWindow )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOW    REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZWINDOW2   BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW2 = 0; 
   //:VIEW TZWINDOW3   BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW3 = 0; 
   //:VIEW TZCONTROL   BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL = 0; 
   //:VIEW AD_BaseSrc  BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseSrc = 0; 
   //:VIEW AD_BaseCtl  BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseCtl = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW SrcInclLOD  BASED ON LOD  TZZOLODO
   zVIEW     SrcInclLOD = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW TZWDVORO    BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:VIEW AD_BaseUC   BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseUC = 0; 
   //:STRING ( 50 )  szWindowName
   zCHAR     szWindowName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupName
   zCHAR     szGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Suffix
   zCHAR     szAD_Suffix[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_AreaTitle
   zCHAR     szAD_AreaTitle[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedObjectName
   zCHAR     szReturnedObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szLOD_ListEntityName
   zCHAR     szLOD_ListEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlTag
   zCHAR     szControlTag[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingObject
   zCHAR     szMappingObject[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingEntity
   zCHAR     szMappingEntity[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingAttribute
   zCHAR     szMappingAttribute[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSearchObjectName
   zCHAR     szSearchObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSourceFileName
   zCHAR     szSourceFileName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclEntS 
   zCHAR     szInclEntS[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclObjS 
   zCHAR     szInclObjS[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclEntT 
   zCHAR     szInclEntT[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclObjT
   zCHAR     szInclObjT[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclSuff
   zCHAR     szInclSuff[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchO 
   zCHAR     szAD_SchO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchV
   zCHAR     szAD_SchV[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetO 
   zCHAR     szAD_RetO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetV
   zCHAR     szAD_RetV[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateWindow
   zCHAR     szTemplateWindow[ 51 ] = { 0 }; 
   //:STRING ( 6 )   szCompareOption
   zCHAR     szCompareOption[ 7 ] = { 0 }; 
   //:STRING ( 1 )   szOption
   zCHAR     szOption[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szGroupIncludeType
   zCHAR     szGroupIncludeType[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szFoundFlag
   zCHAR     szFoundFlag[ 2 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 256 ) szControlText
   zCHAR     szControlText[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szOriginalVML
   zCHAR     szOriginalVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCheckForOperation
   zCHAR     szCheckForOperation[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCompare
   zCHAR     szCompare[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCompareInit
   zCHAR     szCompareInit[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nNextPositionY
   zLONG     nNextPositionY = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength  
   zLONG     nLength = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", TZADWWKO, zLEVEL_TASK );

   //:// GROUP DETAIL INCLUDE PAGE

   //:// This operation builds the object include page both with Find processing and without it.

   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );

   //:CreateViewFromView( TZWINDOW2, TZWINDOWL )   // We will create TZWINDOW2 view because TZWINDOWL is somehow not holding position.
   CreateViewFromView( &TZWINDOW2, TZWINDOWL );
   //:NAME VIEW TZWINDOW2 "TZWINDOW2"
   SetNameForView( TZWINDOW2, "TZWINDOW2", 0, zLEVEL_TASK );
   //:   
   //:// First clone the GroupListInclude or GroupListFindInclude Window depending on GroupType.
   //:IF TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Icons"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Icons" ) == 0 )
   { 
      //:IF szGroupType = "DetailGroupwInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 )
      { 
         //:szTemplateWindow = "GroupListIncludeIcons"
         ZeidonStringCopy( szTemplateWindow, 1, 0, "GroupListIncludeIcons", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szTemplateWindow = "GroupListFindIncludeIcons"
         ZeidonStringCopy( szTemplateWindow, 1, 0, "GroupListFindIncludeIcons", 1, 0, 51 );
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "DetailGroupwInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 )
      { 
         //:szTemplateWindow = "GroupListInclude"
         ZeidonStringCopy( szTemplateWindow, 1, 0, "GroupListInclude", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szTemplateWindow = "GroupListFindInclude"
         ZeidonStringCopy( szTemplateWindow, 1, 0, "GroupListFindInclude", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = szTemplateWindow
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", szTemplateWindow, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "The template window, " + szTemplateWindow + ", could not be found in AD_Base."
      ZeidonStringCopy( szMsg, 1, 0, "The template window, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szTemplateWindow, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ", could not be found in AD_Base.", 1, 0, 201 );
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:SET CURSOR LAST TZWINDOW2.Window  
   RESULT = SetCursorLastEntity( TZWINDOW2, "Window", "" );
   //:nRC = CloneWindow( TZWINDOW2, AD_Base, TaskLPLR, ViewToWindow )
   nRC = oTZWDLGSO_CloneWindow( TZWINDOW2, AD_Base, TaskLPLR, ViewToWindow );

   //:szWindowName = TZADWWKO.ESG_LOD_Entity.Name + "Include"
   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), TZADWWKO, "ESG_LOD_Entity", "Name" );
   ZeidonStringConcat( szWindowName, 1, 0, "Include", 1, 0, 51 );
   //:TZWINDOW2.Window.Tag     = szWindowName
   SetAttributeFromString( TZWINDOW2, "Window", "Tag", szWindowName );
   //:TZWINDOW2.Window.Caption = szWindowName
   SetAttributeFromString( TZWINDOW2, "Window", "Caption", szWindowName );

   //:// If the Group Type is for Find, build the search Group.
   //:IF szGroupType = "DetailGroupwFindInclude" OR szGroupType = "ListGroupwFindInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
   { 
      //:// Build the Search Group
      //:// This code is very similar to the code in AutoDesignFindPages.

      //:// Begin by making sure a ViewObjRef entry exists for the Search LOD.
      //:GET VIEW SearchLOD NAMED "ESGL_SearchLOD"
      RESULT = GetViewByName( &SearchLOD, "ESGL_SearchLOD", TZADWWKO, zLEVEL_TASK );
      //:szSearchObjectName = SearchLOD.LOD.Name 
      GetVariableFromAttribute( szSearchObjectName, 0, 'S', 51, SearchLOD, "LOD", "Name", "", 0 );
      //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szSearchObjectName
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szSearchObjectName, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
         //:AddRegisteredViewName( TZADWWKO, TZWINDOW, SearchLOD, szSearchObjectName )
         oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, SearchLOD, szSearchObjectName );
      } 

      //:END
      //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
      RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", TZADWWKO, zLEVEL_TASK );
      //:DropNameForView( UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK )   // We'll temporarily drop the name because we want to reuse it.
      DropNameForView( UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
      //:NAME VIEW SearchLOD "TZZOLODO_Update"
      SetNameForView( SearchLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );

      //:// Because the GenDetailGrpBootstrap routine uses the DetailMapping... subobject, we'll copy the Search subobject to that subobject.
      //:// Also set the Area Name to "Find" as it's used in generating Control Tags.
      //://TZADWWKO.EntitySubGroup.GroupAreaName = "Find"
      //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:EXCLUDE TZADWWKO.DetailMappingLOD_Attribute  NONE 
         RESULT = ExcludeEntity( TZADWWKO, "DetailMappingLOD_Attribute", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      } 

      //:END
      //:FOR EACH TZADWWKO.ESG_SearchLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:INCLUDE TZADWWKO.DetailMappingLOD_Attribute FROM TZADWWKO.ESG_SearchLOD_Attribute 
         RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_SearchLOD_Attribute", zPOS_AFTER );
         //:// Set work attributes.
         //:SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_SearchLOD_Attribute", zSET_NULL )  
         SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_SearchLOD_Attribute", zSET_NULL );
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      } 

      //:END

      //:// Modify Header Text named "txtSearch" within Search Group by replacing "_InclEntT" string with GroupAreaTitle.
      //:nRC = PositionOnControlByTag( TZADWWKO, TZCONTROL, TZWINDOW2, "txtSearch" )
      nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCONTROL, TZWINDOW2, "txtSearch" );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:NAME VIEW TZCONTROL "TZCONTROLAD"
         SetNameForView( TZCONTROL, "TZCONTROLAD", 0, zLEVEL_TASK );
         //:szControlText = TZCONTROL.Control.Text 
         GetVariableFromAttribute( szControlText, 0, 'S', 257, TZCONTROL, "Control", "Text", "", 0 );
         //:szAD_AreaTitle = TZADWWKO.EntitySubGroup.GroupAreaTitle 
         GetVariableFromAttribute( szAD_AreaTitle, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaTitle", "", 0 );
         //:zSearchAndReplace( szControlText, 256, "_InclEntT", szAD_AreaTitle )
         zSearchAndReplace( szControlText, 256, "_InclEntT", szAD_AreaTitle );
         //:TZCONTROL.Control.Text = szControlText
         SetAttributeFromString( TZCONTROL, "Control", "Text", szControlText );
         //:DropView( TZCONTROL )
         DropView( TZCONTROL );
      } 

      //:END

      //:// Locate Group GroupBoxSearch for generating Search Fields.
      //:nRC = PositionOnControlByTag( TZADWWKO, TZCONTROL, TZWINDOW2, "GroupBoxSearch" )
      nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCONTROL, TZWINDOW2, "GroupBoxSearch" );
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:// Generate the Search Controls
         //:GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "Search Fields" )
         oTZADWWKO_GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "Search Fields" );
         //:DropNameForView( SearchLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK )
         DropNameForView( SearchLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
         //:NAME VIEW UpdateLOD "TZZOLODO_Update"    // Rename the UpdateLOD aince we reused the name for the SearchLOD.
         SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
         //:DropView( TZCONTROL )
         DropView( TZCONTROL );
         //:ELSE
      } 
      else
      { 
         //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
         //:          "Include Find Search Group, GroupBoxSearch, not found in AD_Base.",
         //:          zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( TZADWWKO, "", "Autodesign Subdialog", "Include Find Search Group, GroupBoxSearch, not found in AD_Base.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
   } 

   //:   
   //:END

   //:// Build the Select List Grid,

   //:// Begin by activating the Returned LOD and making sure a ViewObjRef entry exists for it.
   //:GET VIEW SrcInclLOD NAMED "ESGL_IncludeLOD"   
   RESULT = GetViewByName( &SrcInclLOD, "ESGL_IncludeLOD", TZADWWKO, zLEVEL_TASK );
   //:szViewName = TZADWWKO.ESG_ListIncludeW_MetaDef.Name   // View Name is in ESG_ListIncludeW_MetaDef.
   GetVariableFromAttribute( szViewName, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szViewName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szViewName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
      //:AddRegisteredViewName( TZADWWKO, TZWINDOW, SrcInclLOD, szViewName )
      oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, SrcInclLOD, szViewName );
   } 

   //:END
   //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity2.Name    // List Entity Name was selected on interface.
   GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity2", "Name", "", 0 );

   //:// Position on the template Grid, GridSelectList, and then build out subcontrols using AddListMapping.
   //:nRC = PositionOnControlByTag( TZADWWKO, TZCONTROL, TZWINDOW2, "GridSelectList" )
   nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCONTROL, TZWINDOW2, "GridSelectList" );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:          "Grid GridSelectList not found.",
      //:          zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "Grid GridSelectList not found.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:nRC = ResetViewFromSubobject( TZCONTROL )    // We're now on the Grid, so back up to the GroupBox.
   nRC = ResetViewFromSubobject( TZCONTROL );
   //:AddListMapping( TZADWWKO, TZCONTROL, TZWINDOW2, SrcInclLOD, szLOD_ListEntityName, szViewName, "ESG_List2" )
   oTZADWWKO_AddListMapping( TZADWWKO, TZCONTROL, TZWINDOW2, SrcInclLOD, szLOD_ListEntityName, szViewName, "ESG_List2" );

   //:// Add the correct Entity mapping for Grid Control itself.
   //:IF TZCONTROL.CtrlMapLOD_Entity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapLOD_Entity" );
   if ( lTempInteger_0 == 0 )
   { 
      //:EXCLUDE TZCONTROL.CtrlMapLOD_Entity  
      RESULT = ExcludeEntity( TZCONTROL, "CtrlMapLOD_Entity", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE TZCONTROL.CtrlMapLOD_Entity FROM TZADWWKO.ESG_LOD_Entity2 
   RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Entity", TZADWWKO, "ESG_LOD_Entity2", zPOS_AFTER );
   //:DropView( TZCONTROL )
   DropView( TZCONTROL );

   //:// Set up Operation variable values.
   //:szInclObjT = TZADWWKO.W_MetaDefUpdateObject.Name            // Target Include Object Name is update object.
   GetVariableFromAttribute( szInclObjT, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
   //:szInclEntT = TZADWWKO.ESG_TargetLOD_Entity.Name             // Target Include Entity Name is user specified
   GetVariableFromAttribute( szInclEntT, 0, 'S', 51, TZADWWKO, "ESG_TargetLOD_Entity", "Name", "", 0 );
   //:szAD_RetV  = TZADWWKO.ESG_ListIncludeW_MetaDef.Name         // Source Include View Name is user specified 
   GetVariableFromAttribute( szAD_RetV, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
   //:szAD_RetO  = TZADWWKO.ESG_ListIncludeW_MetaDef.wObjectName  // Source Include Object Name was set earlier
   GetVariableFromAttribute( szAD_RetO, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "wObjectName", "", 0 );
   //:szInclEntS = TZADWWKO.ESG_SourceLOD_Entity.Name             // Source Include Entity Name is user specified 
   GetVariableFromAttribute( szInclEntS, 0, 'S', 51, TZADWWKO, "ESG_SourceLOD_Entity", "Name", "", 0 );
   //:szInclSuff = TZADWWKO.EntitySubGroup.GroupAreaName          // Suffix to be added to the end of each Action and Operation.
   GetVariableFromAttribute( szInclSuff, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:IF TZADWWKO.ESG_ListSearchW_MetaDef EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListSearchW_MetaDef" );
   if ( lTempInteger_1 == 0 )
   { 
      //:szAD_SchV = TZADWWKO.ESG_ListSearchW_MetaDef.Name        // Search View name
      GetVariableFromAttribute( szAD_SchV, 0, 'S', 51, TZADWWKO, "ESG_ListSearchW_MetaDef", "Name", "", 0 );
      //:szAD_SchO = TZADWWKO.ESG_ListSearchW_MetaDef.wObjectName // Search Object Name was set earlier
      GetVariableFromAttribute( szAD_SchO, 0, 'S', 51, TZADWWKO, "ESG_ListSearchW_MetaDef", "wObjectName", "", 0 );
      //:ELSE
   } 
   else
   { 
      //:szAD_SchV = ""   // Search view and object weren't specified for Include page without Find.
      ZeidonStringCopy( szAD_SchV, 1, 0, "", 1, 0, 51 );
      //:szAD_SchO = ""
      ZeidonStringCopy( szAD_SchO, 1, 0, "", 1, 0, 51 );
   } 

   //:END

   //:// Alter names for all Actions to add suffix _InclEnt, which is the Include Object Name.
   //:FOR EACH TZWINDOW2.Action 
   RESULT = SetCursorFirstEntity( TZWINDOW2, "Action", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = TZWINDOW2.Action.Tag
      GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOW2, "Action", "Tag", "", 0 );
      //:zSearchAndReplace( szActionName, 50, "_InclSuff", szInclSuff )
      zSearchAndReplace( szActionName, 50, "_InclSuff", szInclSuff );
      //:TZWINDOW2.Action.Tag = szActionName 
      SetAttributeFromString( TZWINDOW2, "Action", "Tag", szActionName );
      RESULT = SetCursorNextEntity( TZWINDOW2, "Action", "" );
   } 

   //:END

   //:// Alter names for the Operations in Dialog that go with the Actions, of which there is currently only one.
   //:// Because the current Operations are under the Source entry, ObjIncl2 or ObjIncl2 from the CloneWindow, we will actually
   //:// create new Operations under the main Source entry and in the end delete the ObjIncl entry.
   //:SET CURSOR FIRST TZWINDOW.SourceFile  
   RESULT = SetCursorFirstEntity( TZWINDOW, "SourceFile", "" );
   //:SET CURSOR LAST TZWINDOW.Operation  
   RESULT = SetCursorLastEntity( TZWINDOW, "Operation", "" );
   //:CreateViewFromView( TZWINDOW3, TZWINDOW )
   CreateViewFromView( &TZWINDOW3, TZWINDOW );
   //:NAME VIEW TZWINDOW3 "TZWINDOW3"
   SetNameForView( TZWINDOW3, "TZWINDOW3", 0, zLEVEL_TASK );
   //:// Position on the SourceFile for the new Operations.
   //:SET CURSOR LAST TZWINDOW3.SourceFile
   RESULT = SetCursorLastEntity( TZWINDOW3, "SourceFile", "" );
   //:FOR EACH TZWINDOW3.Operation 
   RESULT = SetCursorFirstEntity( TZWINDOW3, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = TZWINDOW3.Operation.Name 
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, TZWINDOW3, "Operation", "Name", "", 0 );
      //:zSearchAndReplace( szOperationName, 50, "_InclSuff", szInclSuff )
      zSearchAndReplace( szOperationName, 50, "_InclSuff", szInclSuff );
      //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZWINDOW, "Operation", TZWINDOW3, "Operation", zSET_NULL )
      SetMatchingAttributesByName( TZWINDOW, "Operation", TZWINDOW3, "Operation", zSET_NULL );
      //:FOR EACH TZWINDOW3.Parameter 
      RESULT = SetCursorFirstEntity( TZWINDOW3, "Parameter", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZWINDOW, "Parameter", TZWINDOW3, "Parameter", zSET_NULL )
         SetMatchingAttributesByName( TZWINDOW, "Parameter", TZWINDOW3, "Parameter", zSET_NULL );
         RESULT = SetCursorNextEntity( TZWINDOW3, "Parameter", "" );
      } 

      //:END
      //:TZWINDOW.Operation.Name = szOperationName
      SetAttributeFromString( TZWINDOW, "Operation", "Name", szOperationName );
      //:SET CURSOR FIRST TZWINDOW2.Action WHERE TZWINDOW2.Action.Tag = szOperationName
      RESULT = SetCursorFirstEntityByString( TZWINDOW2, "Action", "Tag", szOperationName, "" );
      //:EXCLUDE TZWINDOW2.ActOper 
      RESULT = ExcludeEntity( TZWINDOW2, "ActOper", zREPOS_AFTER );
      //:INCLUDE TZWINDOW2.ActOper FROM TZWINDOW.Operation 
      RESULT = IncludeSubobjectFromSubobject( TZWINDOW2, "ActOper", TZWINDOW, "Operation", zPOS_AFTER );
      RESULT = SetCursorNextEntity( TZWINDOW3, "Operation", "" );
   } 

   //:END
   //:DELETE ENTITY TZWINDOW3.SourceFile     // Delete the ObjIncl Source entry.
   RESULT = DeleteEntity( TZWINDOW3, "SourceFile", zPOS_NEXT );
   //:DropView( TZWINDOW3 )
   DropView( TZWINDOW3 );

   //:// Check if the Operations have already been copied to the main Source File. If not, copy them with the corrected names.
   //:// Check to see if the Operation, "      Include_InclSuff", exists in the VML File, where _InclSuff is replaced by the specified Suffix.
   //:// Note that the Operation Name starts in column 1.
   //:IF szGroupType = "DetailGroupwFindInclude" OR szGroupType = "ListGroupwFindInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
   { 
      //:szCheckForOperation = "SELECT_FindIncl" + szInclSuff
      ZeidonStringCopy( szCheckForOperation, 1, 0, "SELECT_FindIncl", 1, 0, 257 );
      ZeidonStringConcat( szCheckForOperation, 1, 0, szInclSuff, 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szCheckForOperation = "SELECT_Include" + szInclSuff
      ZeidonStringCopy( szCheckForOperation, 1, 0, "SELECT_Include", 1, 0, 257 );
      ZeidonStringConcat( szCheckForOperation, 1, 0, szInclSuff, 1, 0, 257 );
   } 

   //:END
   //:nLength = zGetStringLen( szCheckForOperation )
   nLength = zGetStringLen( szCheckForOperation );
   //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szFindVML = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
   ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
   ZeidonStringConcat( szFindVML, 1, 0, szTempString_0, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
   //:nFileIn   = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ );
   //:nRC2 = -1
   nRC2 = -1;
   //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
   nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
   //:LOOP WHILE nRC = 1 AND nRC2 != 0
   while ( nRC == 1 && nRC2 != 0 )
   { 
      //://zstrncpy( szCompare, szVML_Statement, nLength )
      //zstrncpy( szCompare, szVML_Statement, nLength );
      //:nRC2 = zstrcmp( szCompare, szCheckForOperation )
      nRC2 = zstrcmp( szCompare, szCheckForOperation );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
   } 

   //:END
   //:SysCloseFile( TZWINDOW, nFileIn, 0 )
   SysCloseFile( TZWINDOW, nFileIn, 0 );
   //:IF nRC2 != 0     // Operation was not found.
   if ( nRC2 != 0 )
   { 
      //:// Copy the VML from ObjIncl.VML or ObjIncl.VML and append it to the current Find VML
      //:// Which VML code version to use depends on whether or not this is a Find subpage.
      //:IF szGroupType = "DetailGroupwFindInclude" OR szGroupType = "ListGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 
         //:szOriginalVML     = "c:\lplr\AD_Base\ObjIncl.VML"
         ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjIncl.VML", 1, 0, 257 );
         //:TraceLineS( "*** Source: ", "ObjIncl" )
         TraceLineS( "*** Source: ", "ObjIncl" );
         //:ELSE
      } 
      else
      { 
         //:szOriginalVML     = "c:\lplr\AD_Base\ObjIncl2.VML"
         ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjIncl2.VML", 1, 0, 257 );
         //:TraceLineS( "*** Source: ", "ObjIncl2" )
         TraceLineS( "*** Source: ", "ObjIncl2" );
      } 

      //:END
      //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
      GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
      //:szFindVML = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
      ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
      ZeidonStringConcat( szFindVML, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );

      //:// Set szGroupIncludeType based on szGroupType. This will be used later in the VML copy loop.
      //:IF szGroupType = "ListGroupwInclude" OR szGroupType = "ListGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 
         //:szGroupIncludeType = "L"
         ZeidonStringCopy( szGroupIncludeType, 1, 0, "L", 1, 0, 2 );
         //:ELSE
      } 
      else
      { 
         //:szGroupIncludeType = "D"
         ZeidonStringCopy( szGroupIncludeType, 1, 0, "D", 1, 0, 2 );
      } 

      //:END

      //:// Copy the ObjIncl VML to the end of the Find VML, converting the variable characters in the process.
      //:nFileIn   = SysOpenFile( TZWINDOW, szOriginalVML, COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOW, szOriginalVML, COREFILE_READ );
      //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND )
      nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND );
      //:IF nFileOut < 0  
      if ( nFileOut < 0 )
      { 
         //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE )
         nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE );
      } 

      //:END
      //:szCompareInit = "   // Start FindRangeInitialization"
      ZeidonStringCopy( szCompareInit, 1, 0, "   // Start FindRangeInitialization", 1, 0, 257 );
      //:nLength       = zGetStringLen( szCompareInit )
      nLength = zGetStringLen( szCompareInit );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 

         //:// Add FindRange code if it exists in the VML Source.
         //://zstrncpy( szCompare, szVML_Statement, nLength )    // Copy the necsssary characters of the line to the compare field.
         ////zstrncpy( szCompare, szVML_Statement, nLength );
         //:nRC2 = zstrcmp( szCompare, szCompareInit )
         nRC2 = zstrcmp( szCompare, szCompareInit );
         //:IF nRC2 = 0
         if ( nRC2 == 0 )
         { 
            //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
            SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
            //:// Add appropriate Range initialization statements.
            //:// We first need to copy 
            //:FOR TZADWWKO.FlatSelectedSearchAttribute 
            RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:DELETE ENTITY TZADWWKO.FlatSelectedSearchAttribute NONE  
               RESULT = DeleteEntity( TZADWWKO, "FlatSelectedSearchAttribute", zREPOS_NONE );
               RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
            } 

            //:END
            //:FOR TZADWWKO.ESG_SearchLOD_Attribute  
            RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CREATE ENTITY TZADWWKO.FlatSelectedSearchAttribute 
               RESULT = CreateEntity( TZADWWKO, "FlatSelectedSearchAttribute", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZADWWKO, "FlatSelectedSearchAttribute", TZADWWKO, "ESG_SearchLOD_Attribute", zSET_ALL )
               SetMatchingAttributesByName( TZADWWKO, "FlatSelectedSearchAttribute", TZADWWKO, "ESG_SearchLOD_Attribute", zSET_ALL );
               //:TZADWWKO.FlatSelectedSearchAttribute.EntityName    = TZADWWKO.ESG_SearchLOD_Entity.Name 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "EntityName", TZADWWKO, "ESG_SearchLOD_Entity", "Name" );
               //:TZADWWKO.FlatSelectedSearchAttribute.AttributeName = TZADWWKO.ESG_SearchER_Attribute.Name 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "AttributeName", TZADWWKO, "ESG_SearchER_Attribute", "Name" );
               //:INCLUDE TZADWWKO.FlatSearchLOD_Attribute FROM TZADWWKO.ESG_SearchLOD_Attribute 
               RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatSearchLOD_Attribute", TZADWWKO, "ESG_SearchLOD_Attribute", zPOS_AFTER );
               RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
            } 

            //:END
            //:AddFindRangeInitValues( TZADWWKO, TZWINDOW, nFileIn, nFileOut )
            oTZADWWKO_AddFindRangeInitValues( TZADWWKO, TZWINDOW, nFileIn, nFileOut );
            //:ELSE
         } 
         else
         { 

            //:// The SELECT_Include_InclSuff operation has 2 versions depending on whether the initial Group is Detail or List.
            //:// The 2 versions (sets of code) are identified by the characters "Start List Include" and "Start Detial Include", 
            //:// with an "Option End" statement concluding the 2 versions.
            //:// Once we encounter the first Option statement, we will do the read and write loop here until we get to the end
            //:// statement.
            //://zstrncpy( szCompareOption, szVML_Statement, 6 )
            ////zstrncpy( szCompareOption, szVML_Statement, 6 );
            //:nRC2 = zstrcmp( szCompareOption, "Option" )
            nRC2 = zstrcmp( szCompareOption, "Option" );
            //:IF nRC2 = 0
            if ( nRC2 == 0 )
            { 
               //:// We will handle the List and Detail options separately.
               //:IF szGroupType = "ListGroupwFindInclude"
               if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
               { 
                  //:// For List, we will copy the first set and skip the second.
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                  //:szCompareOption = ""
                  ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
                  //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
                  while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
                  { 
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT )  // Target Include Entity name, such as "CombinedMailingPerson"
                     zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT );
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS )  // Source Include Entity name, such as "Person"
                     zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS );
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT )  // TargetInclude Object name, such as "mPerson"
                     zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT );
                     //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN",  szAD_RetV )   // Returned / Source Include View name such as lPerson.
                     zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetV );
                     //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
                     SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
                     //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
                     nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                     //://zstrncpy( szCompareOption, szVML_Statement, 6 )
                     //zstrncpy( szCompareOption, szVML_Statement, 6 );
                  } 

                  //:END
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                  //:// Now skip to the next Option statement.
                  //:szCompareOption = ""
                  ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
                  //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
                  while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
                  { 
                     //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
                     nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                     //://zstrncpy( szCompareOption, szVML_Statement, 6 )
                     //zstrncpy( szCompareOption, szVML_Statement, 6 );
                  } 

                  //:END
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the last Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );

                  //:ELSE
               } 
               else
               { 
                  //:// For Detail, we will skip the first set and copy the second.
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                  //:szCompareOption = ""
                  ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
                  //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
                  while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
                  { 
                     //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
                     nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                     //://zstrncpy( szCompareOption, szVML_Statement, 6 )
                     //zstrncpy( szCompareOption, szVML_Statement, 6 );
                  } 

                  //:END
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                  //:szCompareOption = ""
                  ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
                  //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
                  while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
                  { 
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT )  // Target Include Entity name, such as "CombinedMailingPerson"
                     zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT );
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS )  // Source Include Entity name, such as "Person"
                     zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS );
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT )  // TargetInclude Object name, such as "mPerson"
                     zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT );
                     //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN",  szAD_RetV )   // Returned / Source Include View name such as lPerson.
                     zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetV );
                     //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
                     SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
                     //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
                     nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                     //://zstrncpy( szCompareOption, szVML_Statement, 6 )
                     //zstrncpy( szCompareOption, szVML_Statement, 6 );
                  } 

                  //:END
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
               } 


               //:END
            } 

            //:END

            //:zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT )  // Target Include Entity name, such as "CombinedMailingPerson"
            zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT );
            //:zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS )  // Source Include Entity name, such as "Person"
            zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS );
            //:zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT )  // TargetInclude Object name, such as "mPerson"
            zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN",  szAD_RetV )   // Returned / Source Include View name such as lPerson.
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetV );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetO",  szAD_RetO )   // Returned / Source Include object name such as lPerson.
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetO", szAD_RetO );
            //:zSearchAndReplace( szVML_Statement, 256, "_InclSuff", szInclSuff )  // Operation Suffix such as "CombinedMail"
            zSearchAndReplace( szVML_Statement, 256, "_InclSuff", szInclSuff );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchN",  szAD_SchV )   // Search View Name if this is the Find option.
            zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchV );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchO",  szAD_SchO )   // Search Object Name if this is the Find option.
            zSearchAndReplace( szVML_Statement, 256, "_AD_SchO", szAD_SchO );

            //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
            SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
         } 

         //:END
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:   
      //:END

      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:SysCloseFile( TZWINDOW, nFileOut, 0 )
      SysCloseFile( TZWINDOW, nFileOut, 0 );
   } 

   //:END

   //:// Resize and reposition the Group Boxes on the Window
   //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOW2 )
   oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOW2 );
   //:// Rename some Controls to avoid duplicate Tags.
   //:SET CURSOR FIRST TZWINDOWL.Control
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   //:RenameAD_Controls( TZADWWKO, TZWINDOWL ) 
   oTZADWWKO_RenameAD_Controls( TZADWWKO, TZWINDOWL );
   //:DropView( TZWINDOW2 )
   DropView( TZWINDOW2 );
   return( 0 );
//    
// END 
} 


//:TRANSFORMATION OPERATION
//:GenListDetailPage( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                   VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                   VIEW AD_Base   BASED ON LOD TZWDLGSO,
//:                   VIEW ViewToWindow )

//:   VIEW TZCONTROL  BASED ON LOD  TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenListDetailPage( zVIEW     TZADWWKO,
                             zVIEW     TZWINDOWL,
                             zVIEW     AD_Base,
                             zVIEW     ViewToWindow )
{
   zVIEW     TZCONTROL = 0; 
   //:VIEW TaskLPLR   REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL2 BASED ON LOD TZWDLGSO
   zVIEW     TZWINDOWL2 = 0; 
   //:STRING ( 50 ) szWindowName
   zCHAR     szWindowName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   zLONG     lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// GROUP LIST DETAIL PAGE

   //:CreateViewFromView( TZWINDOWL2, TZWINDOWL )
   CreateViewFromView( &TZWINDOWL2, TZWINDOWL );
   //:NAME VIEW TZWINDOWL2 "TZWINDOWL2"
   SetNameForView( TZWINDOWL2, "TZWINDOWL2", 0, zLEVEL_TASK );
   //:   
   //:// First clone the GroupListUpdate or GroupListDetail Window at the end of the Dialog.
   //:// The difference between the two is that the Update has Entity operations.
   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
   //:IF szGroupType = "ListGroupWUpdatePage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 )
   { 
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "GroupListUpdate"
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "GroupListUpdate", "" );
      //:ELSE
   } 
   else
   { 
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "GroupListDetail"
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "GroupListDetail", "" );
   } 

   //:END
   //:SET CURSOR LAST TZWINDOWL2.Window  
   RESULT = SetCursorLastEntity( TZWINDOWL2, "Window", "" );
   //:nRC = CloneWindow( TZWINDOWL2, AD_Base, TaskLPLR, ViewToWindow )
   nRC = oTZWDLGSO_CloneWindow( TZWINDOWL2, AD_Base, TaskLPLR, ViewToWindow );
   //:szWindowName = TZADWWKO.EntitySubGroup.GroupAreaName + "Detail"
   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), TZADWWKO, "EntitySubGroup", "GroupAreaName" );
   ZeidonStringConcat( szWindowName, 1, 0, "Detail", 1, 0, 51 );
   //:TZWINDOWL2.Window.Tag     = szWindowName
   SetAttributeFromString( TZWINDOWL2, "Window", "Tag", szWindowName );
   //:TZWINDOWL2.Window.Caption = szWindowName
   SetAttributeFromString( TZWINDOWL2, "Window", "Caption", szWindowName );
   //:SET CURSOR FIRST TZWINDOWL.Window WHERE TZWINDOWL.Window.ZKey = TZWINDOWL2.Window.ZKey  
   GetIntegerFromAttribute( &lTempInteger_0, TZWINDOWL2, "Window", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( TZWINDOWL, "Window", "ZKey", lTempInteger_0, "" );

   //:// Delete the SourceFile entry just cloned because the Operations for this page were created during
   //:// generation of the parent List Group.
   //:DELETE ENTITY TZWINDOWL2.SourceFile  
   RESULT = DeleteEntity( TZWINDOWL2, "SourceFile", zPOS_NEXT );

   //:// Build the Detail control Group set.assuming the format is Bootstrap.
   //:// Because the GenDetailGrpBootstrap routine uses the DetailMapping suboject, we will copy the ESG_Detail subobject
   //:// entries to the DetailMapping suboject.
   //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:EXCLUDE TZADWWKO.DetailMappingLOD_Attribute  NONE 
      RESULT = ExcludeEntity( TZADWWKO, "DetailMappingLOD_Attribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   } 

   //:END

   //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:INCLUDE TZADWWKO.DetailMappingLOD_Attribute FROM TZADWWKO.ESG_DetailLOD_Attribute 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zPOS_AFTER );
      //:// Set work attributes.
      //:SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zSET_NULL )  
      SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zSET_NULL );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
   } 

   //:END

   //:// Create TZCONTROL and position it on the last / innermost Group of the nested set of Groups in the window, which is where
   //:// the data Controls will be built.
   //:CreateViewFromView( TZCONTROL, TZWINDOWL2 )  
   CreateViewFromView( &TZCONTROL, TZWINDOWL2 );
   //:NAME VIEW TZCONTROL "TZCONTROLAD"
   SetNameForView( TZCONTROL, "TZCONTROLAD", 0, zLEVEL_TASK );
   //:SET CURSOR LAST TZCONTROL.Control
   RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
   //:LOOP WHILE TZCONTROL.CtrlCtrl EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   while ( lTempInteger_1 == 0 )
   { 
      //:nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:SET CURSOR LAST TZCONTROL.Control
      RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
      lTempInteger_1 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   } 

   //:END

   //:GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL2, "MultiGroup" )
   oTZADWWKO_GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL2, "MultiGroup" );

   //:// Readjust the position of each GroupBox based on the size and position of the previous GroupBox.
   //:// This is a little more complex for Bootstrap, so we will call a sub operation for that case.
   //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL2 )
   oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL2 );

   //:// Process Actions depending on page type.
   //:szAD_Area = TZADWWKO.EntitySubGroup.GroupAreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:IF szGroupType = "ListGroupWUpdatePage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 )
   { 
      //:// Accept and Cancel Actions.
      //:// Convert Action Names and tie to Operations of the same name.
      //:// The Operations were created when the initial List Group was built.
      //:FOR EACH TZWINDOWL2.Action 
      RESULT = SetCursorFirstEntity( TZWINDOWL2, "Action", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:szActionName = TZWINDOWL2.Action.Tag 
         GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL2, "Action", "Tag", "", 0 );
         //:zSearchAndReplace( szActionName, 256, "_AD_Area",  szAD_Area )        // Area Name
         zSearchAndReplace( szActionName, 256, "_AD_Area", szAD_Area );
         //:TZWINDOWL2.Action.Tag = szActionName
         SetAttributeFromString( TZWINDOWL2, "Action", "Tag", szActionName );
         //:SET CURSOR FIRST TZWINDOWL.Operation WHERE TZWINDOWL.Operation.Name = szActionName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Operation", "Name", szActionName, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:INCLUDE TZWINDOWL2.ActOper FROM TZWINDOWL.Operation 
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL2, "ActOper", TZWINDOWL, "Operation", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( TZWINDOWL2, "Action", "" );
         //:END 
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// Return Action
      //:// Simply modify Action to include Area Name.
      //:TZWINDOWL2.Action.Tag = "RETURN_" + szAD_Area
      ZeidonStringCopy( szTempString_0, 1, 0, "RETURN_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szAD_Area, 1, 0, 33 );
      SetAttributeFromString( TZWINDOWL2, "Action", "Tag", szTempString_0 );
   } 

   //:END

   //:DropView( TZWINDOWL2 )
   DropView( TZWINDOWL2 );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:InitGridSelectAttribs( VIEW TZADWWKO   BASED ON LOD TZADWWKO,
//:                       VIEW TempCtrl   BASED ON LOD TZWDLGSO )

//:   VIEW TaskLPLR  REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_InitGridSelectAttribs( zVIEW     TZADWWKO,
                                 zVIEW     TempCtrl )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TempCtrl2 BASED ON LOD  TZWDLGSO
   zVIEW     TempCtrl2 = 0; 
   //:VIEW TZZOLFLO  BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW UpdateVOR BASED ON LOD  TZWDVORO
   zVIEW     UpdateVOR = 0; 
   //:STRING ( 20 ) szControlDefTag
   zCHAR     szControlDefTag[ 21 ] = { 0 }; 
   //:STRING ( 20 ) szControlType
   zCHAR     szControlType[ 21 ] = { 0 }; 
   //:STRING ( 50 ) szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER RowCount
   zLONG     RowCount = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_1[ 255 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Set up existing mapping for Grid.

   //:// If we're not positioned on the Grid, step down to it.\
   //:IF TempCtrl.ControlDef.Tag != "Grid"
   if ( CompareAttributeToString( TempCtrl, "ControlDef", "Tag", "Grid" ) != 0 )
   { 
      //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )   // Step down to Grid
      SetViewToSubobject( TempCtrl, "CtrlCtrl" );
   } 

   //:END
   //:TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = "Y"
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "Y" );
   //://TZADWWKO.EntitySubGroup.GroupType                     = "ListGroupOnly"
   //:TZADWWKO.EntitySubGroup.ReturnedObjectRootEntityName  = TempCtrl.CtrlMapLOD_Entity.Name 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "ReturnedObjectRootEntityName", TempCtrl, "CtrlMapLOD_Entity", "Name" );
   //:TZADWWKO.EntitySubGroup.AutodesignGroupTemplateName   = "Simple Grid"
   SetAttributeFromString( TZADWWKO, "EntitySubGroup", "AutodesignGroupTemplateName", "Simple Grid" );
   //:CREATE ENTITY TZADWWKO.AutodesignGroupTemplate 
   RESULT = CreateEntity( TZADWWKO, "AutodesignGroupTemplate", zPOS_AFTER );
   //:TZADWWKO.AutodesignGroupTemplate.GroupName = "Simple Grid"
   SetAttributeFromString( TZADWWKO, "AutodesignGroupTemplate", "GroupName", "Simple Grid" );

   //:// Activate Object View and LOD.
   //:szViewName = TempCtrl.CtrlMapView.Name 
   GetVariableFromAttribute( szViewName, 0, 'S', 51, TempCtrl, "CtrlMapView", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( TZADWWKO, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 )
   nRC = ActivateMetaOI_ByName( TZADWWKO, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 );
   //:NAME VIEW UpdateVOR "UpdateObjectViewRef"
   SetNameForView( UpdateVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );
   //:IF TZADWWKO.W_MetaDefUpdateView EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateView" );
   if ( lTempInteger_0 == 0 )
   { 
      //:EXCLUDE TZADWWKO.W_MetaDefUpdateView  
      RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateView", zREPOS_AFTER );
   } 

   //:END 
   //:SET CURSOR FIRST TaskLPLR.W_MetaType WHERE TaskLPLR.W_MetaType.Type = 9   // Read view of Registered View
   RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 9, "" );
   //:SET CURSOR FIRST TaskLPLR.W_MetaDef  WHERE TaskLPLR.W_MetaDef.Name  = UpdateVOR.ViewObjRef.Name  
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateVOR, "ViewObjRef", "Name" );
   RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
   //:INCLUDE TZADWWKO.W_MetaDefUpdateView  FROM TaskLPLR.W_MetaDef 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateView", TaskLPLR, "W_MetaDef", zPOS_AFTER );
   //:szLOD_Name = UpdateVOR.LOD.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, UpdateVOR, "LOD", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( TZADWWKO, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( TZADWWKO, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW UpdateLOD "TZZOLODO_Update"
   SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST TaskLPLR.W_MetaType  WHERE TaskLPLR.W_MetaType.Type = 7   // Read view of LOD
   RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 7, "" );
   //:SET CURSOR FIRST TaskLPLR.W_MetaDef   WHERE TaskLPLR.W_MetaDef.Name  = UpdateLOD.LOD.Name 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD", "Name" );
   RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
   //:IF TZADWWKO.W_MetaDefUpdateObject EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateObject" );
   if ( lTempInteger_1 == 0 )
   { 
      //:EXCLUDE TZADWWKO.W_MetaDefUpdateObject 
      RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateObject", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE TZADWWKO.W_MetaDefUpdateObject FROM TaskLPLR.W_MetaDef 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateObject", TaskLPLR, "W_MetaDef", zPOS_AFTER );

   //:// Build the list of selected attributes for the new Grid and delete Grid subcontrols except for the first and buttons.
   //:INCLUDE TZADWWKO.ESG_LOD_Entity FROM TempCtrl.CtrlMapLOD_Entity 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_LOD_Entity", TempCtrl, "CtrlMapLOD_Entity", zPOS_AFTER );
   //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )    // Step down to Grid subcontrol
   SetViewToSubobject( TempCtrl, "CtrlCtrl" );
   //:Count = 0
   Count = 0;
   //:FOR EACH TempCtrl.Control 
   RESULT = SetCursorFirstEntity( TempCtrl, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:Count = Count + 1
      Count = Count + 1;
      //:IF TempCtrl.CtrlMapLOD_Attribute EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( TempCtrl, "CtrlMapLOD_Attribute" );
      if ( lTempInteger_2 == 0 )
      { 
         //:// Make sure there wasn't a duplicate Subcontrol.
         //:SET CURSOR FIRST TZADWWKO.ESG_ListLOD_Attribute WHERE TZADWWKO.ESG_ListLOD_Attribute.PromptValue = TempCtrl.Control.Text
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TempCtrl, "Control", "Text" );
         RESULT = SetCursorFirstEntityByString( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", szTempString_1, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR LAST TZADWWKO.ESG_ListLOD_Attribute
            RESULT = SetCursorLastEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
            //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TempCtrl.CtrlMapLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TempCtrl, "CtrlMapLOD_Attribute", zPOS_AFTER );
            //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue = TempCtrl.Control.Text 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TempCtrl, "Control", "Text" );

            //:// ControlType of "" becomes either Text or EditBox Control depending on WebControlProperty.
            //:szControlType = TempCtrl.ControlDef.Tag
            GetVariableFromAttribute( szControlType, 0, 'S', 21, TempCtrl, "ControlDef", "Tag", "", 0 );
            //:IF szControlType = "GridEditCtl"
            if ( ZeidonStringCompare( szControlType, 1, 0, "GridEditCtl", 1, 0, 21 ) == 0 )
            { 
               //:SET CURSOR FIRST TempCtrl.WebControlProperty WHERE TempCtrl.WebControlProperty.Name = "Edit"
               RESULT = SetCursorFirstEntityByString( TempCtrl, "WebControlProperty", "Name", "Edit", "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "EditBox"
                  SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "EditBox" );
                  //:ELSE
               } 
               else
               { 
                  //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "Text"
                  SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "Text" );
               } 

               //:END 
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = szControlType
               SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", szControlType );
            } 

            //:END
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TempCtrl, "Control", "" );
      //:END 
   } 

   //:END
   //:ResetViewFromSubobject( TempCtrl )
   ResetViewFromSubobject( TempCtrl );

   //:// Build the list of selectable LOD Entities for a potential list box.
   //:// We have one entry in the list for each Entity in the Update Object.
   //:// First remove any current entries then rebuild them.
   //:FOR EACH TZADWWKO.PotentialLOD_Entity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "PotentialLOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:EXCLUDE TZADWWKO.PotentialLOD_Entity NONE 
      RESULT = ExcludeEntity( TZADWWKO, "PotentialLOD_Entity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "PotentialLOD_Entity", "" );
   } 

   //:END
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:INCLUDE TZADWWKO.PotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "PotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      RESULT = SetCursorNextEntity( UpdateLOD, "LOD_Entity", "" );
   } 

   //:END

   //:// Delete any current potential attributes and then go to build them.
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE  
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   } 

   //:END
   //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD_EntityParent", "Name" );
   oTZADWWKO_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_0, "" );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_InitializeL_SymbolType( zVIEW     TZADWWKO )
{
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 

   //:InitializeL_SymbolType( VIEW TZADWWKO BASED ON LOD TZADWWKO )

   //:// Initialize ListboxSymbolType entries if they don't already exist.
   //:IF TZADWWKO.ListboxSymbolType DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ListboxSymbolType" );
   if ( lTempInteger_0 != 0 )
   { 
      //:TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Buttons"
      SetAttributeFromString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Buttons" );
      //:CREATE ENTITY TZADWWKO.ListboxSymbolType 
      RESULT = CreateEntity( TZADWWKO, "ListboxSymbolType", zPOS_AFTER );
      //:TZADWWKO.ListboxSymbolType.Name = "Buttons"
      SetAttributeFromString( TZADWWKO, "ListboxSymbolType", "Name", "Buttons" );
      //:CREATE ENTITY TZADWWKO.ListboxSymbolType 
      RESULT = CreateEntity( TZADWWKO, "ListboxSymbolType", zPOS_AFTER );
      //:TZADWWKO.ListboxSymbolType.Name = "Icons" 
      SetAttributeFromString( TZADWWKO, "ListboxSymbolType", "Name", "Icons" );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:PositionOnControlByTag( VIEW TZADWWKO       BASED ON LOD TZADWWKO,
//:                        VIEW ReturnedView   BASED ON LOD TZWDLGSO,
//:                        VIEW OuterStartView BASED ON LOD TZWDLGSO,
//:                        STRING ( 50 ) szControlTagName )

//:   VIEW TZCtlHier BASED ON LOD  TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_PositionOnControlByTag( zVIEW     TZADWWKO,
                                  zPVIEW    ReturnedView,
                                  zVIEW     OuterStartView,
                                  zPCHAR    szControlTagName )
{
   zVIEW     TZCtlHier = 0; 
   //:STRING ( 50 ) szReturnedEntityName
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:SHORT lReturnedLevel
   zSHORT    lReturnedLevel = 0; 
   //:SHORT lInitialLevel
   zSHORT    lInitialLevel = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 


   //:CreateViewFromView( TZCtlHier, OuterStartView )
   CreateViewFromView( &TZCtlHier, OuterStartView );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:lInitialLevel = lReturnedLevel
   lInitialLevel = lReturnedLevel;
   //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel
   while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "CtrlCtrl"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlCtrl", 1, 0, 51 ) == 0 )
      { 
         //:szControlDef = TZCtlHier.Control.Tag 
         GetVariableFromAttribute( szControlDef, 0, 'S', 51, TZCtlHier, "Control", "Tag", "", 0 );
         //:IF szControlDef = szControlTagName
         if ( ZeidonStringCompare( szControlDef, 1, 0, szControlTagName, 1, 0, 51 ) == 0 )
         { 
            //:// This is a Text Control, so convert any _Section or _SectionTitle characters
            //:CreateViewFromView( ReturnedView, TZCtlHier )
            CreateViewFromView( ReturnedView, TZCtlHier );
            //:DropView( TZCtlHier )
            DropView( TZCtlHier );
            //:RETURN 0
            return( 0 );
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )
   DropView( TZCtlHier );
   //:RETURN -1
   return( -1 );
// END
} 


//:TRANSFORMATION OPERATION
//:RenameAD_Controls( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                   VIEW TZWINDOW BASED ON LOD TZWDLGSO )

//:   STRING ( 5 ) szPrefix
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_RenameAD_Controls( zVIEW     TZADWWKO,
                             zVIEW     TZWINDOW )
{
   zCHAR     szPrefix[ 6 ] = { 0 }; 
   //:STRING ( 5 ) szCount
   zCHAR     szCount[ 6 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   zSHORT    RESULT; 


   //:// Rename GroupBox, Grid and PushBtn controls to avoid duplicate generated names.
   //:// We will leave the name of the top level Group control and rename the the Groupbox controls under it, except for
   //:// those with CSS_Class "header" or "collapse show".
   //:// The rest will be a combination of GBxxx were xxx is a number string made up of the sequential Group Number
   //:// at each level

   //:// We'll only perform this logic for any top-level GroupBox.
   //:// Also, if there's only one GroupBox entry at this level, we will rename it "GB" and process it's subgroups.
   //:// If there are multiple  GroupBox entries, we will start renaming them with number suffix's.
   //:Count = 0
   Count = 0;
   //:FOR EACH TZWINDOW.Control
   RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZWINDOW.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:Count = Count + 1
         Count = Count + 1;
      } 

      RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
      //:END
   } 

   //:END
   //:IF Count > 0
   if ( Count > 0 )
   { 
      //:IF Count = 1
      if ( Count == 1 )
      { 
         //:// Rename single top-level GroupBox "GB".
         //:SET CURSOR FIRST TZWINDOW.ControlDef WITHIN TZWINDOW.Window 
         //:           WHERE TZWINDOW.ControlDef.Tag = "GroupBox" 
         RESULT = SetCursorFirstEntityByString( TZWINDOW, "ControlDef", "Tag", "GroupBox", "Window" );
         //:TZWINDOW.Control.Tag = "GB" 
         SetAttributeFromString( TZWINDOW, "Control", "Tag", "GB" );
         //:SetViewToSubobject( TZWINDOW, "CtrlCtrl" )
         SetViewToSubobject( TZWINDOW, "CtrlCtrl" );
      } 

      //:END

      //:// Process multiple GroupBox entries. Skip any entry of CSS_Class "header".
      //:Count = 0
      Count = 0;
      //:FOR EACH TZWINDOW.Control 
      RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF TZWINDOW.Control.CSS_Class != "header"
         if ( CompareAttributeToString( TZWINDOW, "Control", "CSS_Class", "header" ) != 0 )
         { 
            //:Count = Count + 1
            Count = Count + 1;
            //:zIntegerToString( szCount, 5, Count )
            zIntegerToString( szCount, 5, Count );
            //:szPrefix = "GB" + szCount
            ZeidonStringCopy( szPrefix, 1, 0, "GB", 1, 0, 6 );
            ZeidonStringConcat( szPrefix, 1, 0, szCount, 1, 0, 6 );
            //:TZWINDOW.Control.Tag = szPrefix
            SetAttributeFromString( TZWINDOW, "Control", "Tag", szPrefix );
            //:RenameAD_ControlsR( TZADWWKO, TZWINDOW, szPrefix )
            oTZADWWKO_RenameAD_ControlsR( TZADWWKO, TZWINDOW, szPrefix );
         } 

         RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
         //:END
      } 

      //:END
      //:ResetViewFromSubobject( TZWINDOW )
      ResetViewFromSubobject( TZWINDOW );
      //:ResetViewFromSubobject( TZWINDOW )
      ResetViewFromSubobject( TZWINDOW );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:RenameAD_ControlsR( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                    VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                    STRING ( 50 ) szPrefix )

//:   STRING ( 50 ) szControlName
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_RenameAD_ControlsR( zVIEW     TZADWWKO,
                              zVIEW     TZWINDOW,
                              zPCHAR    szPrefix )
{
   zCHAR     szControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szNewControlName
   zCHAR     szNewControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLevel
   zCHAR     szLevel[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLocalPrefix
   zCHAR     szLocalPrefix[ 51 ] = { 0 }; 
   //:STRING ( 20 ) szCompare
   zCHAR     szCompare[ 21 ] = { 0 }; 
   //:STRING ( 5 )  szCount
   zCHAR     szCount[ 6 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER SubCount
   zLONG     SubCount = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:// Recursive routine to name GroupBox, Grid and PushBtn controls to avoid duplicate generated names.
   //:IF TZWINDOW.CtrlCtrl EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZWINDOW, "CtrlCtrl" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetViewToSubobject( TZWINDOW, "CtrlCtrl" )
      SetViewToSubobject( TZWINDOW, "CtrlCtrl" );
      //:Count = 0
      Count = 0;
      //:FOR EACH TZWINDOW.Control 
      RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:Count = Count + 1
         Count = Count + 1;
         //:zIntegerToString( szCount, 5, Count )
         zIntegerToString( szCount, 5, Count );
         //:szLocalPrefix = szPrefix + szCount
         ZeidonStringCopy( szLocalPrefix, 1, 0, szPrefix, 1, 0, 51 );
         ZeidonStringConcat( szLocalPrefix, 1, 0, szCount, 1, 0, 51 );
         //:IF TZWINDOW.ControlDef.Tag = "GroupBox"
         if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "GroupBox" ) == 0 )
         { 
            //:IF TZWINDOW.Control.CSS_Class != "collapse show"  // Skip the collapsable Group because we need to keep it's existing name.
            if ( CompareAttributeToString( TZWINDOW, "Control", "CSS_Class", "collapse show" ) != 0 )
            { 
               //:// Add the Prefix and TopCount to the outside GroupBox Tag.
               //:szNewControlName = szLocalPrefix
               ZeidonStringCopy( szNewControlName, 1, 0, szLocalPrefix, 1, 0, 51 );
               //:TZWINDOW.Control.Tag = szNewControlName
               SetAttributeFromString( TZWINDOW, "Control", "Tag", szNewControlName );
            } 

            //:END
            //:IF TZWINDOW.CtrlCtrl EXISTS
            lTempInteger_1 = CheckExistenceOfEntity( TZWINDOW, "CtrlCtrl" );
            if ( lTempInteger_1 == 0 )
            { 
               //:RenameAD_ControlsR( TZADWWKO, TZWINDOW, szNewControlName )
               oTZADWWKO_RenameAD_ControlsR( TZADWWKO, TZWINDOW, szNewControlName );
            } 

            //:END
            //:ELSE
         } 
         else
         { 

            //:IF TZWINDOW.ControlDef.Tag = "PushBtn"
            if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "PushBtn" ) == 0 )
            { 
               //:// Add the Prefix after the letters, "PushBtn"
               //:szNewControlName = "PushBtn" + szLocalPrefix
               ZeidonStringCopy( szNewControlName, 1, 0, "PushBtn", 1, 0, 51 );
               ZeidonStringConcat( szNewControlName, 1, 0, szLocalPrefix, 1, 0, 51 );
               //:TZWINDOW.Control.Tag = szNewControlName
               SetAttributeFromString( TZWINDOW, "Control", "Tag", szNewControlName );
               //:ELSE
            } 
            else
            { 

               //:IF TZWINDOW.ControlDef.Tag = "Grid"
               if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "Grid" ) == 0 )
               { 
                  //:// Add Prefix after "Grid" or >GridSub"
                  //:TZWINDOW.Control.Tag = "Grid" + szLocalPrefix 
                  ZeidonStringCopy( szTempString_0, 1, 0, "Grid", 1, 0, 33 );
                  ZeidonStringConcat( szTempString_0, 1, 0, szLocalPrefix, 1, 0, 33 );
                  SetAttributeFromString( TZWINDOW, "Control", "Tag", szTempString_0 );
                  //:SetViewToSubobject( TZWINDOW, "CtrlCtrl" )
                  SetViewToSubobject( TZWINDOW, "CtrlCtrl" );
                  //:SubCount = 0
                  SubCount = 0;
                  //:FOR EACH TZWINDOW.Control 
                  RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
                  while ( RESULT > zCURSOR_UNCHANGED )
                  { 
                     //:SubCount = SubCount + 1
                     SubCount = SubCount + 1;
                     //:zIntegerToString( szCount, 5, SubCount )
                     zIntegerToString( szCount, 5, SubCount );
                     //:TZWINDOW.Control.Tag = "GridSub" + szLocalPrefix + szCount
                     ZeidonStringCopy( szTempString_1, 1, 0, "GridSub", 1, 0, 33 );
                     ZeidonStringConcat( szTempString_1, 1, 0, szLocalPrefix, 1, 0, 33 );
                     ZeidonStringConcat( szTempString_1, 1, 0, szCount, 1, 0, 33 );
                     SetAttributeFromString( TZWINDOW, "Control", "Tag", szTempString_1 );
                     RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
                  } 

                  //:END
                  //:ResetViewFromSubobject( TZWINDOW )
                  ResetViewFromSubobject( TZWINDOW );
               } 

               //:END
            } 

            //:END
         } 

         RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
         //:END
      } 

      //:END
      //:ResetViewFromSubobject( TZWINDOW )
      ResetViewFromSubobject( TZWINDOW );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ResizeReposGroupBoxes( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZWINDOWL BASED ON LOD TZWDLGSO )

//:   VIEW vGroup BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_ResizeReposGroupBoxes( zVIEW     TZADWWKO,
                                 zVIEW     TZWINDOWL )
{
   zVIEW     vGroup = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 


   //:// RECOMPUTE GROUP SIZES AND POSITIONS.
   //:// Call Recursive operation to compute size and reposition subcontrols.
   //:// However, first step down past the first two outer control levels.
   //:CreateViewFromView( vGroup, TZWINDOWL )
   CreateViewFromView( &vGroup, TZWINDOWL );
   //:NAME VIEW vGroup "vGroup"
   SetNameForView( vGroup, "vGroup", 0, zLEVEL_TASK );
   //:SET CURSOR LAST vGroup.Control   // Make sure we're on outer Group, which should be last control.
   RESULT = SetCursorLastEntity( vGroup, "Control", "" );
   //:ResizeReposRecurs( vGroup )
   oTZADWWKO_ResizeReposRecurs( vGroup );

   //:// Compute the size of the outer Group, which is determined from the last subcontrol.
   //:SET CURSOR LAST vGroup.CtrlCtrl
   RESULT = SetCursorLastEntity( vGroup, "CtrlCtrl", "" );
   //:vGroup.Control.SZDLG_Y = vGroup.CtrlCtrl.PSDLG_Y + vGroup.CtrlCtrl.SZDLG_Y + 10
   GetIntegerFromAttribute( &lTempInteger_0, vGroup, "CtrlCtrl", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_1, vGroup, "CtrlCtrl", "SZDLG_Y" );
   lTempInteger_2 = lTempInteger_0 + lTempInteger_1 + 10;
   SetAttributeFromInteger( vGroup, "Control", "SZDLG_Y", lTempInteger_2 );
   //:DropView( vGroup )
   DropView( vGroup );
   return( 0 );
//    
// END
} 


//:LOCAL OPERATION
//:ResizeReposRecurs( VIEW vGroup BASED ON LOD TZWDLGSO )

//:   INTEGER CurrentPositionY
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_ResizeReposRecurs( zVIEW     vGroup )
{
   zLONG     CurrentPositionY = 0; 
   //:INTEGER LastOriginalPositionY
   zLONG     LastOriginalPositionY = 0; 
   //:INTEGER LargestControlSizeX
   zLONG     LargestControlSizeX = 0; 
   //:INTEGER CurrentControlSizeX
   zLONG     CurrentControlSizeX = 0; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 20 )  szGroupClass
   zCHAR     szGroupClass[ 21 ] = { 0 }; 
   //:STRING ( 4 )   szCol4
   zCHAR     szCol4[ 5 ] = { 0 }; 
   //:STRING ( 5 )   szInteger
   zCHAR     szInteger[ 6 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 


   //:// Process nested GroupBox Controls recursively recomputing the size and position of each based on the size of subcontrols.
   //:// Exit if there are no subcontrols or if any subcontrol is not a GroupBox.

   //:IF vGroup.CtrlCtrl DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( vGroup, "CtrlCtrl" );
   if ( lTempInteger_0 != 0 )
   { 
      //:RETURN 1
      return( 1 );
      //:ELSE
   } 
   else
   { 
      //:SetViewToSubobject( vGroup, "CtrlCtrl" )
      SetViewToSubobject( vGroup, "CtrlCtrl" );
      //:FOR EACH vGroup.Control 
      RESULT = SetCursorFirstEntity( vGroup, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF vGroup.ControlDef.Tag != "GroupBox"
         if ( CompareAttributeToString( vGroup, "ControlDef", "Tag", "GroupBox" ) != 0 )
         { 
            //:ResetViewFromSubobject( vGroup )
            ResetViewFromSubobject( vGroup );
            //:RETURN 1
            return( 1 );
         } 

         RESULT = SetCursorNextEntity( vGroup, "Control", "" );
         //:END
      } 

      //:END
      //:ResetViewFromSubobject( vGroup )
      ResetViewFromSubobject( vGroup );
   } 

   //:END

   //:// Process each GroupBox subcontrol to determine its size and position relative to this GroupBox.
   //:CurrentPositionY      = 10
   CurrentPositionY = 10;
   //:LastOriginalPositionY = -1
   LastOriginalPositionY = -1;
   //:FOR EACH vGroup.CtrlCtrl  
   RESULT = SetCursorFirstEntity( vGroup, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( vGroup, "CtrlCtrl" )
      SetViewToSubobject( vGroup, "CtrlCtrl" );
      //:szGroupClass = vGroup.Control.CSS_Class 
      GetVariableFromAttribute( szGroupClass, 0, 'S', 21, vGroup, "Control", "CSS_Class", "", 0 );
      //:szCol4 = szGroupClass[1:4]
      ZeidonStringCopy( szCol4, 1, 0, szGroupClass, 1, 4, 5 );
      //:// Only process Groups that don't have a subcontrol of CSS_Class = col-xxx
      //:IF vGroup.ControlDef.Tag = "GroupBox" AND szCol4 != "col-" AND szGroupClass != "col"
      if ( CompareAttributeToString( vGroup, "ControlDef", "Tag", "GroupBox" ) == 0 && ZeidonStringCompare( szCol4, 1, 0, "col-", 1, 0, 5 ) != 0 && ZeidonStringCompare( szGroupClass, 1, 0, "col", 1, 0, 21 ) != 0 )
      { 
         //:ResizeReposRecurs( vGroup )
         oTZADWWKO_ResizeReposRecurs( vGroup );
      } 

      //:END 
      //:ResetViewFromSubobject( vGroup )
      ResetViewFromSubobject( vGroup );

      //:// Determine Y position of each based on the previous GroupBox size and position.
      //:// Don't process Groups than have col-xxx subgroups as they don't need it and may also be side by side.
      //:IF szCol4 != "col-" AND szGroupClass != "col"
      if ( ZeidonStringCompare( szCol4, 1, 0, "col-", 1, 0, 5 ) != 0 && ZeidonStringCompare( szGroupClass, 1, 0, "col", 1, 0, 21 ) != 0 )
      { 
         //:LastOriginalPositionY = vGroup.CtrlCtrl.PSDLG_Y
         GetIntegerFromAttribute( &LastOriginalPositionY, vGroup, "CtrlCtrl", "PSDLG_Y" );
         //:vGroup.CtrlCtrl.PSDLG_Y = CurrentPositionY
         SetAttributeFromInteger( vGroup, "CtrlCtrl", "PSDLG_Y", CurrentPositionY );
         //:CurrentPositionY = CurrentPositionY + vGroup.CtrlCtrl.SZDLG_Y + 10
         GetIntegerFromAttribute( &lTempInteger_1, vGroup, "CtrlCtrl", "SZDLG_Y" );
         CurrentPositionY = CurrentPositionY + lTempInteger_1 + 10;
      } 

      RESULT = SetCursorNextEntity( vGroup, "CtrlCtrl", "" );
      //:END
   } 

   //:END

   //:// Determine the Y size of this GroupBox based on the size and position of the last subGroupBox + 10.
   //:SET CURSOR LAST vGroup.CtrlCtrl
   RESULT = SetCursorLastEntity( vGroup, "CtrlCtrl", "" );
   //:vGroup.Control.SZDLG_Y = vGroup.CtrlCtrl.PSDLG_Y + vGroup.CtrlCtrl.SZDLG_Y + 10
   GetIntegerFromAttribute( &lTempInteger_2, vGroup, "CtrlCtrl", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_3, vGroup, "CtrlCtrl", "SZDLG_Y" );
   lTempInteger_4 = lTempInteger_2 + lTempInteger_3 + 10;
   SetAttributeFromInteger( vGroup, "Control", "SZDLG_Y", lTempInteger_4 );

   //:// Determine the X size of this GroupBox based on the position and size of the subGroupBox entries.
   //:LargestControlSizeX = 0
   LargestControlSizeX = 0;
   //:FOR EACH vGroup.CtrlCtrl  
   RESULT = SetCursorFirstEntity( vGroup, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CurrentControlSizeX = vGroup.CtrlCtrl.PSDLG_X + vGroup.CtrlCtrl.SZDLG_X 
      GetIntegerFromAttribute( &lTempInteger_5, vGroup, "CtrlCtrl", "PSDLG_X" );
      GetIntegerFromAttribute( &lTempInteger_6, vGroup, "CtrlCtrl", "SZDLG_X" );
      CurrentControlSizeX = lTempInteger_5 + lTempInteger_6;
      //:IF CurrentControlSizeX > LargestControlSizeX
      if ( CurrentControlSizeX > LargestControlSizeX )
      { 
         //:LargestControlSizeX = CurrentControlSizeX
         LargestControlSizeX = CurrentControlSizeX;
      } 

      RESULT = SetCursorNextEntity( vGroup, "CtrlCtrl", "" );
      //:END
   } 

   //:END
   //:vGroup.Control.SZDLG_X = LargestControlSizeX + 10
   lTempInteger_7 = LargestControlSizeX + 10;
   SetAttributeFromInteger( vGroup, "Control", "SZDLG_X", lTempInteger_7 );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ReturnAttrControlType( VIEW AnyView,
//:                       STRING ( 32 ) szDomainName,
//:                       STRING ( 32 ) szDomainType,
//:                       STRING ( 10 ) szControlType,
//:                       STRING ( 1 )  szUpdateFlag )
static zSHORT
oTZADWWKO_ReturnAttrControlType( zVIEW     AnyView,
                                 zPCHAR    szDomainName,
                                 zPCHAR    szDomainType,
                                 zPCHAR    szControlType,
                                 zPCHAR    szUpdateFlag )
{


   //:// Return the Control Type based on the Domain and whether or not the Update Flag is set.
   //:// A Domain of Y/N always returns a Checkbox regardless of Update Flag.
   //:IF szDomainName = "Y/N"
   if ( ZeidonStringCompare( szDomainName, 1, 0, "Y/N", 1, 0, 33 ) == 0 )
   { 
      //:// Domain is Y/N.
      //:szControlType   = "CheckBox"
      ZeidonStringCopy( szControlType, 1, 0, "CheckBox", 1, 0, 11 );
      //:ELSE
   } 
   else
   { 
      //:IF szUpdateFlag = "Y"
      if ( ZeidonStringCompare( szUpdateFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:IF szDomainName = "Date" OR szDomainName = "DateTime"
         if ( ZeidonStringCompare( szDomainName, 1, 0, "Date", 1, 0, 33 ) == 0 || ZeidonStringCompare( szDomainName, 1, 0, "DateTime", 1, 0, 33 ) == 0 )
         { 
            //:// A Domain of Date will make the control a Calendar.
            //:szControlType = "Calendar"
            ZeidonStringCopy( szControlType, 1, 0, "Calendar", 1, 0, 11 );
            //:ELSE
         } 
         else
         { 
            //:IF szDomainType = "T"
            if ( ZeidonStringCompare( szDomainType, 1, 0, "T", 1, 0, 33 ) == 0 )
            { 
               //:// The Domain is a table, so make control a Combobox.
               //:szControlType = "ComboBox"
               ZeidonStringCopy( szControlType, 1, 0, "ComboBox", 1, 0, 11 );
               //:ELSE
            } 
            else
            { 
               //:// If not a table, make control an Editbox.
               //:szControlType = "EditBox"
               ZeidonStringCopy( szControlType, 1, 0, "EditBox", 1, 0, 11 );
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// The entity is not updatable, so Control Type is "Text"
         //:szControlType = "Text"
         ZeidonStringCopy( szControlType, 1, 0, "Text", 1, 0, 11 );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_SetDialogProperties( zVIEW     TZADWWKO,
                               zVIEW     TargetDialog,
                               zVIEW     SourceDialog )
{

   //:SetDialogProperties( VIEW TZADWWKO BASED ON LOD TZADWWKO,
   //:                  VIEW TargetDialog BASED ON LOD  TZWDLGSO,
   //:                  VIEW SourceDialog BASED ON LOD  TZWDLGSO )

   //:// Initialize the necessary Dialog Properties if they haven't already been set.
   //:IF TargetDialog.Dialog.WEB_PageHeadInclude = ""
   if ( CompareAttributeToString( TargetDialog, "Dialog", "WEB_PageHeadInclude", "" ) == 0 )
   { 
      //:SetMatchingAttributesByName( TargetDialog, "Dialog", SourceDialog, "Dialog", zSET_NULL )
      SetMatchingAttributesByName( TargetDialog, "Dialog", SourceDialog, "Dialog", zSET_NULL );
   } 

   //:END
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
