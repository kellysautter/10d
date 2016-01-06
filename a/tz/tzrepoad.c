/*
CHANGE LOG
2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
*/

#define zGLOBAL_DATA
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"

// LAST MESSAGE ID: RE00111

zLONG    DO_DOMAIN;
zLONG    DO_ERD;
zLONG    DO_SA;
zLONG    DO_TE;
zLONG    DO_LOD;
zLONG    DO_VOR;
zLONG    DO_PENV;
zLONG    DO_UIS;
zLONG    DO_DIALOG;
zLONG    DO_HEADER;
zLONG    DO_SRC;
zLONG    DO_OPER;

zLONG   MARK;
#define COMMIT     1
zLONG   VALIDATE;
zLONG   PROMPT;

zSHORT nFixes;


zOPER_EXPORT zSHORT OPERATION
zwTZREPOAD_PreBuild( zVIEW    vSubtask )
{
   zVIEW vRepoWK;

   ActivateEmptyObjectInstance( &vRepoWK, "TZREPOWK", vSubtask, zSINGLE );
   CreateEntity( vRepoWK, "TZREPOWK", zPOS_BEFORE );
   SetNameForView( vRepoWK, "TZREPOWK", vSubtask, zLEVEL_TASK );
   InitializeDefaultLPL( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZREPOAD_ExitDialog( zVIEW    vSubtask )
{

   TerminateLPLR( vSubtask );
   return( 0 );
}

zSHORT
fnSetMetaTypes( zVIEW    vSubtask, zSHORT sSet )
{
   SetCtrlState( vSubtask, "Header",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "Source",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "Domain",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "Operation",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "Model",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "SA",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "TE",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "LOD",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "VOR",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "PE",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "Dialog",
                    zCONTROL_STATUS_CHECKED, sSet );
   SetCtrlState( vSubtask, "UIS",
                    zCONTROL_STATUS_CHECKED, sSet );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZREPOAD_On( zVIEW    vSubtask )
{
   fnSetMetaTypes( vSubtask, 1 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZREPOAD_Off( zVIEW    vSubtask )
{
   fnSetMetaTypes( vSubtask, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZREPOAD_PostBuild( zVIEW    vSubtask )
{
   SetCtrlState( vSubtask, "Validate",
                    zCONTROL_STATUS_CHECKED, 1 );
   SetCtrlState( vSubtask, "Mark",
                    zCONTROL_STATUS_CHECKED, 1 );
   SetCtrlState( vSubtask, "Prompt",
                    zCONTROL_STATUS_CHECKED, 1 );
   fnSetMetaTypes( vSubtask, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZREPOAD_SelectMetaType( zVIEW    vSubtask )
{
   zVIEW vRepoWK;
   zVIEW vMetaList;
   zLONG lMetaType;

   GetViewByName( &vRepoWK, "TZREPOWK", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lMetaType, vRepoWK, "TZREPOWK", "MetaType" );
   while ( lMetaType < 2000 )
      lMetaType += 100;

   RetrieveViewForMetaList( vSubtask, &vMetaList,( zSHORT ) lMetaType );
   SetNameForView( vMetaList, "TZREPOML", vSubtask, zLEVEL_TASK );
   RefreshCtrl( vSubtask, "ListBox1" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZREPOAD_CompareMeta( zVIEW    vSubtask )
{
   zVIEW vRepoWK;
   zVIEW vMetaList;
   zVIEW vFileMeta;
   zVIEW vDB_Meta;
   zVIEW vKZDBHQUA;
   zVIEW vZeidonCM;
   zVIEW vTaskLPLR;
   zSHORT nRC;
   zLONG lMetaType;
   zLONG lZKey;
   zCHAR szZKey[ 16 ];

   if ( GetViewByName( &vMetaList, "TZREPOML", vSubtask, zLEVEL_TASK )
                                                      != zLEVEL_TASK ||
        SetCursorFirstSelectedEntity( vMetaList, "W_MetaDef", 0 ) <
                                                   zCURSOR_SET )
   {
      TraceLineS("(tzrepoad) Unable to Compare Meta!!!","");
      return( zCALL_ERROR );
   }

   GetViewByName( &vRepoWK, "TZREPOWK", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lMetaType, vRepoWK, "TZREPOWK", "MetaType" );
   do
   {
      if ( GetViewByName( &vDB_Meta, "DBMeta", vSubtask, zLEVEL_TASK ) == zLEVEL_TASK )
         DropObjectInstance( vDB_Meta );

      ActivateMetaOI( vSubtask, &vFileMeta, vMetaList, (zSHORT) lMetaType, zSINGLE );
      OrderOI_ByDefaultAttribs( vFileMeta );


      // Activate the database version of the object in question
      if ( GetViewByName( &vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK ) < 0 )
      {
         if ( ActivateEmptyObjectInstance ( &vKZDBHQUA, "KZDBHQUA",
                                          vMetaList,
                                          zSINGLE ) >= 0 )
         {
            SetNameForView( vKZDBHQUA, "KZDBHQUA", vSubtask, zLEVEL_TASK );
            CreateEntity( vKZDBHQUA, "EntitySpec", zPOS_AFTER );
            CreateEntity( vKZDBHQUA, "QualAttrib", zPOS_AFTER );
         }
         else
         {
            return( zCALL_ERROR );
         }
      }

      while ( lMetaType > 100 )
         lMetaType -= 100;

      GetIntegerFromAttribute( &lZKey, vMetaList, "W_MetaDef", "CPLR_ZKey" );
      SetAttributeFromString( vKZDBHQUA, "EntitySpec", "EntityName",
                              SRC_CMOD[ lMetaType ].szOD_ROOT );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "EntityName",
                              SRC_CMOD[ lMetaType ].szOD_ROOT );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib",
                              "AttributeName", "ZKey" );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Oper", "=" );
      zltoa( lZKey, szZKey, sizeof( szZKey ) );
      SetAttributeFromString( vKZDBHQUA, "QualAttrib", "Value", szZKey );
// OperationCount( 11 );
      ActivateObjectInstance( &vDB_Meta, SRC_CMOD[ lMetaType ].szOD,
                              vKZDBHQUA, vKZDBHQUA, zSINGLE );
// OperationCount( 0 );
      GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );

      // Get the view to TaskLPLR OI.
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      cfPostCheckout( (zSHORT) lMetaType, vZeidonCM, vDB_Meta, vTaskLPLR, 1 );

      SetNameForView( vFileMeta, "FileMeta", vSubtask, zLEVEL_TASK );
      SetNameForView( vDB_Meta, "DBMeta", vSubtask, zLEVEL_TASK );
      TraceLineI("(tzrepoad) Instance being compared type = ", lMetaType );
      DisplayEntityInstance( vMetaList, "W_MetaDef" );
      nRC = CompareOI_ToOI( vFileMeta, vDB_Meta );
      TraceLineI("(tzrepoad) results of compare = ", nRC );
      if ( nRC == 0 )
         DropObjectInstance( vDB_Meta );

   } while ( SetCursorNextSelectedEntity( vMetaList, "W_MetaDef", 0 ) >=
                                                            zCURSOR_SET );
   return( 0 );
}

// This operation goes through the whole application and prepares the
// system for inserting onto the database, it evaluates all entity
// instances and insures that all instances are marked for insertion and
// all relationships inserts have a corresponding instance in the
// owning object instance
//   zSOURCE_DIALOG_META  0
//   zSOURCE_LOD_META     1
//   zSOURCE_POD_META     2
//   zSOURCE_ERD_META     3
//   zSOURCE_GO_META      4
//   zSOURCE_SA_META      5
//   zSOURCE_UIS_META     6
//   zSOURCE_DOMAIN_META  7
//   zSOURCE_DTE_META     8
//   zSOURCE_DIL_META     9
//   zSOURCE_VOR_META    10
//   zSOURCE_SRC_META    11
//   zSOURCE_HDR_META    12
//   zSOURCE_PENV_META   13
zSHORT nTotalErrors;
void
IncrementErrorCount( zVIEW vSubtask, zPSHORT pnError )
{
   zCHAR szMsg[ 60 ];

   strcpy_s( szMsg, sizeof( szMsg ), "Total errors " );
   (*pnError)++;
   nTotalErrors++;
   zltoa( nTotalErrors, szMsg + 50, sizeof( szMsg ) - 50 );
   strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 50 ] );
   MB_SetMessage( vSubtask, 2, szMsg );
   TraceLineS("--------------------------- END of ERROR ---------------------","");
}

void
fnRecursLodEntityMarking( zVIEW vLod )
{
   zCHAR szWork[ 2 ];
   zCHAR szDerived[ 2 ];

   do
   {
      GetStringFromAttribute( szWork, sizeof( szWork ), vLod, "LOD_EntityParent", "Work" );
      GetStringFromAttribute( szDerived, sizeof( szDerived ), vLod, "LOD_EntityParent", "Derived" );
      if ( szWork[ 0 ] != 'Y' )
      {
         SetIncrementalUpdateFlags( vLod, "ER_EntityRec",
            zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED |
            zSET_INCR_CURSORPOS );
      }
      if (( szWork[ 0 ] != 'Y' ) || ( szDerived[ 0 ] != 'Y' ))
      {
         SetIncrementalUpdateFlags( vLod, "ER_RelLinkRec",
            zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED |
            zSET_INCR_CURSORPOS );
      }
      SetIncrementalUpdateFlags( vLod, "LOD_EntityConstraintOperRec",
                         zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
      if ( SetCursorFirstEntity( vLod, "LOD_AttributeRec", 0 ) ==
                                                zCURSOR_SET )
      {
         do
         {
            GetStringFromAttribute( szWork, sizeof( szWork ), vLod, "LOD_AttributeRec", "Work" );
            if ( szWork[ 0 ] == 'Y' )
            {
               SetIncrementalUpdateFlags( vLod, "DomainRec",
                  zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED |
                  zSET_INCR_CURSORPOS );
            }
            else
            {
               SetIncrementalUpdateFlags( vLod, "ER_AttributeRec",
                  zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED |
                  zSET_INCR_CURSORPOS );
            }
            SetIncrementalUpdateFlags( vLod, "LOD_AttrDerivationOperRec",
                               zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         } while ( SetCursorNextEntity( vLod, "LOD_AttributeRec", 0 ) ==
                                                            zCURSOR_SET );
      }
      if ( CheckExistenceOfEntity( vLod, "LOD_EntityChild" ) > zCURSOR_NULL )
      {
         SetViewToSubobject( vLod, "LOD_EntityChild" );
         fnRecursLodEntityMarking( vLod );
         ResetViewFromSubobject( vLod );
      }
   } while ( SetCursorNextEntity( vLod, "LOD_EntityParent", 0 ) ==
                                                      zCURSOR_SET );
}



void
fnRecursUIS_EntityValidation( zVIEW vSubtask,
                              zVIEW vUis,
                              zVIEW vPe,
                              zPSHORT pnError )
{
   zVIEW vUisWk;
   zVIEW vLod;
   zVIEW vLodList;
   zLONG lZKey;


   RetrieveViewForMetaList( vSubtask, &vLodList, zREFER_LOD_META );
   CreateViewFromViewForTask( &vUisWk, vUis, 0 );
   while ( ResetViewFromSubobject( vUisWk ) == 0 );
   SetCursorFirstEntity( vUisWk, "UIS_ViewObjRef", 0 );

   if ( SetCursorFirstEntity( vUis, "UIS_Entity", 0 ) >= zCURSOR_SET )
   do
   {
      if ( SetCursorFirstEntity( vUis, "WndStyle", "UIS_Entity" )
                                                      >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vUis, "WndStyle", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vPe, "WindowStyle", "ZKey",
                                             lZKey, 0 ) < zCURSOR_SET )
         {
            TraceLineS("(tzrepoad) UIS WndStyle not found in Presentation Environment","" );
            DisplayEntityInstance( vUisWk, "UI_Spec" );
            DisplayEntityInstance( vUis, "UIS_Entity" );
            DisplayEntityInstance( vUis, "WndDesign" );
            DisplayEntityInstance( vUis, "WndStyle" );
            IncrementErrorCount( vSubtask, pnError );
         }
      }
      if ( SetCursorFirstEntity( vUis, "LOD_Entity", 0 ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vUisWk, "UIS_LOD", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vLodList,
                     "W_MetaDef", "CPLR_ZKey", lZKey, 0 ) >= zCURSOR_SET )
         {
            ActivateMetaOI( vSubtask, &vLod, vLodList, zREFER_LOD_META, 0 );
            GetIntegerFromAttribute( &lZKey, vUis, "LOD_Entity", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vLod, "LOD_Entity",
                                                "ZKey", lZKey,
                                                0 ) != zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) UIS LOD_Entity not found in first UIS_ViewObjRef","" );
               DisplayEntityInstance( vUisWk, "UI_Spec" );
               DisplayEntityInstance( vUis, "UIS_Entity" );
               DisplayEntityInstance( vUis, "LOD_Entity" );
               DisplayEntityInstance( vUisWk, "UIS_ViewObjRef" );
               IncrementErrorCount( vSubtask, pnError );
            }
            // For each M_LOD_Attribute and L_LOD_Attribute, make sure it
            // exists in the LOD for the first UIS_ViewObjRef
            if ( SetCursorFirstEntity( vUis, "M_LOD_Attribute", 0 ) >= zCURSOR_SET )
            do
            {
               GetIntegerFromAttribute( &lZKey, vUis,
                                        "M_LOD_Attribute", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vLod, "LOD_Attribute",
                                                   "ZKey", lZKey,
                                                   "LOD" ) != zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) UIS M_LOD_Attribute not found in first UIS_ViewObjRef","" );
                  DisplayEntityInstance( vUisWk, "UI_Spec" );
                  DisplayEntityInstance( vUis, "UIS_Entity" );
                  DisplayEntityInstance( vUis, "M_LOD_Attribute" );
                  DisplayEntityInstance( vUis, "M_LOD_Entity" );
                  DisplayEntityInstance( vUisWk, "UIS_ViewObjRef" );
                  IncrementErrorCount( vSubtask, pnError );
               }
            }  while ( SetCursorNextEntity( vUis, "M_LOD_Attribute", 0 ) >= zCURSOR_SET );
            if ( SetCursorFirstEntity( vUis, "L_LOD_Attribute", 0 ) >= zCURSOR_SET )
            do
            {
               GetIntegerFromAttribute( &lZKey, vUis,
                                        "L_LOD_Attribute", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vLod, "LOD_Attribute",
                                                   "ZKey", lZKey,
                                                   "LOD" ) != zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) UIS L_LOD_Attribute not found in first UIS_ViewObjRef","" );
                  DisplayEntityInstance( vUisWk, "UI_Spec" );
                  DisplayEntityInstance( vUis, "UIS_Entity" );
                  DisplayEntityInstance( vUis, "L_LOD_Attribute" );
                  DisplayEntityInstance( vUis, "L_LOD_Entity" );
                  DisplayEntityInstance( vUisWk, "UIS_ViewObjRef" );
                  IncrementErrorCount( vSubtask, pnError );
               }
            }  while ( SetCursorNextEntity( vUis, "L_LOD_Attribute", 0 ) >= zCURSOR_SET );
            DropView( vLod );
         }

      }

      // Go recursively down levels to validate subordinate UIS Entities
      if ( CheckExistenceOfEntity( vUis, "UIS_ChildEntity" ) > zCURSOR_NULL )
      {
         SetViewToSubobject( vUis, "UIS_ChildEntity" );
         fnRecursUIS_EntityValidation( vSubtask, vUis, vPe, pnError );
         ResetViewFromSubobject( vUis );
      }
   }  while ( SetCursorNextEntity( vUis, "UIS_Entity", 0 ) >= zCURSOR_SET );

   DropView( vUisWk );
   DropView( vLodList );
}

void
fnRecursDialogControlValidation( zVIEW vSubtask,
                                 zVIEW vDialog,
                                 zVIEW vPe,
                                 zPSHORT pnError )
{
   zCHAR szTag[34];
   zVIEW vDlgWk;
   zVIEW vLod;
   zVIEW vLodList;
   zVIEW vViewObj;
   zLONG lZKey;

   RetrieveViewForMetaList( vSubtask, &vLodList, zREFER_LOD_META );
   CreateViewFromViewForTask( &vDlgWk, vDialog, 0 );
   while ( ResetViewFromSubobject( vDlgWk ) == 0 );
   if ( SetCursorFirstEntity( vDialog, "Control", 0 ) >= zCURSOR_SET )
   do
   {
      // Control validation, Validate control mapping
      if ( SetCursorFirstEntity( vDialog, "CtrlMap", 0 ) >= zCURSOR_SET )
      do
      {
         if ( SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute", 0 ) >= zCURSOR_SET )
         do
         {
            if ( SetCursorFirstEntity( vDialog, "CtrlMapView", 0 )
                                                      != zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Dialog control mapping found without a ViewObjRef","" );
               DisplayEntityInstance( vDlgWk, "Dialog" );
               DisplayEntityInstance( vDlgWk, "Window" );
               DisplayEntityInstance( vDialog, "Control" );
               DisplayEntityInstance( vDialog, "CtrlMap" );
               DisplayEntityInstance( vDialog, "CtrlMapLOD_Attribute" );
               DisplayEntityInstance( vDialog, "CtrlMapER_Attribute" );
               DisplayEntityInstance( vDialog, "CtrlMapRelatedEntity" );
               IncrementErrorCount( vSubtask, pnError );
            }
            else
            {
               if ( SetCursorFirstEntityByEntityCsr( vDlgWk, "ViewObjRef",
                     vDialog, "CtrlMapView", 0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) Dialog control mapping using VOR not included in Dialog","");
                  DisplayEntityInstancePath( vDialog, "CtrlMapView" );
                  IncrementErrorCount( vSubtask, pnError );
                  GetStringFromAttribute( szTag, sizeof( szTag ), vDialog,
                                          "CtrlMapView", "Name" );
                  if ( SetCursorFirstEntityByString( vDlgWk, "ViewObjRef",
                                                     "Name", szTag, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vDialog, "CtrlMapView", zREPOS_NONE );
                     IncludeSubobjectFromSubobject( vDialog, "CtrlMapView",
                                                    vDlgWk, "ViewObjRef",
                                                    zPOS_BEFORE );
                     TraceLineS("(tzrepoad) Problem logically fixed!","");
                     nFixes++;
                  }
               }
               if ( SetCursorFirstEntityByEntityCsr( vDlgWk, "ViewObjRef",
                     vDialog, "CtrlMapView", 0 ) >= zCURSOR_SET )
               {
                  GetIntegerFromAttribute( &lZKey, vDlgWk, "LOD", "ZKey" );
                  if ( SetCursorFirstEntityByInteger( vLodList, "W_MetaDef",
                                                      "CPLR_ZKey", lZKey,
                                                      0 ) >= zCURSOR_SET )
                  {
                     ActivateMetaOI( vSubtask, &vLod, vLodList, zREFER_LOD_META, 0 );
                     GetIntegerFromAttribute( &lZKey, vDialog,
                                              "CtrlMapLOD_Attribute",
                                              "ZKey" );
                     if ( SetCursorFirstEntityByInteger( vLod, "LOD_Attribute",
                                                         "ZKey", lZKey,
                                                         "LOD" ) < zCURSOR_SET )
                     {
                        TraceLineS("(tzrepoad) Dialog control mapping found for non-existent LOD Attr","");
                        DisplayEntityInstance( vDlgWk, "Dialog" );
                        DisplayEntityInstance( vDlgWk, "Window" );
                        DisplayEntityInstance( vDialog, "Control" );
                        DisplayEntityInstance( vDialog, "CtrlMap" );
                        DisplayEntityInstance( vDialog, "CtrlMapLOD_Attribute" );
                        DisplayEntityInstance( vDialog, "CtrlMapER_Attribute" );
                        DisplayEntityInstance( vDialog, "CtrlMapRelatedEntity" );
                        DisplayEntityInstance( vDlgWk, "LOD" );
                        IncrementErrorCount( vSubtask, pnError );
                        GetStringFromAttribute( szTag, sizeof( szTag ), vDialog,
                           "CtrlMapRelatedEntity", "Name" );
                        if ( SetCursorFirstEntityByString( vLod,
                              "LOD_Entity", "Name", szTag, 0 ) >= zCURSOR_SET )
                        {
                           GetStringFromAttribute( szTag, sizeof( szTag ), vDialog,
                              "CtrlMapER_Attribute", "Name" );
                           if ( SetCursorFirstEntityByString( vLod,
                                 "ER_Attribute", "Name", szTag,
                                 "LOD_Entity" ) >= zCURSOR_SET )
                           {
                              ExcludeEntity( vDialog, "CtrlMapLOD_Attribute", zREPOS_NONE );
                              IncludeSubobjectFromSubobject( vDialog, "CtrlMapLOD_Attribute",
                                                             vLod, "LOD_Attribute", zPOS_AFTER );
                              TraceLineS("(tzrepoad) Problem logically fixed!","");
                              nFixes++;
                           }
                        }
                     }
                     DropView( vLod );
                  }
               }
            }
         } while ( SetCursorNextEntity( vDialog, "CtrlMapLOD_Attribute", 0 ) >= zCURSOR_SET );
         if ( SetCursorFirstEntity( vDialog, "CtrlMapLOD_Entity", 0 ) >= zCURSOR_SET )
         do
         {
            CreateViewFromViewForTask( &vViewObj, vDialog, 0 );
            if ( CheckExistenceOfEntity( vViewObj, "CtrlMapView" )
                                                         != zCURSOR_SET )
            {
               if ( SetCursorFirstEntity( vViewObj,
                      "CtrlMapView", "Control" ) >= zCURSOR_SET )
               {
                  while ( SetCursorNextEntity( vViewObj,
                          "CtrlMapView", "Control" ) >= zCURSOR_SET );
               }
            }

            if ( CheckExistenceOfEntity( vViewObj, "CtrlMapView" )
                                                         != zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Dialog control mapping found without a ViewObjRef","" );
               DisplayEntityInstance( vDlgWk, "Dialog" );
               DisplayEntityInstance( vDlgWk, "Window" );
               DisplayEntityInstance( vDialog, "Control" );
               DisplayEntityInstance( vDialog, "CtrlMap" );
               DisplayEntityInstance( vDialog, "CtrlMapLOD_Entity" );
               IncrementErrorCount( vSubtask, pnError );
            }
            else
            {
               if ( SetCursorFirstEntityByEntityCsr( vDlgWk, "ViewObjRef",
                     vViewObj, "CtrlMapView", 0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) Dialog control mapping using VOR not included in Dialog","");
                  DisplayEntityInstancePath( vViewObj, "CtrlMapView" );
                  IncrementErrorCount( vSubtask, pnError );
                  GetStringFromAttribute( szTag, sizeof( szTag ), vViewObj,
                                          "CtrlMapView", "Name" );
                  if ( SetCursorFirstEntityByString( vDlgWk, "ViewObjRef",
                                                     "Name", szTag, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vViewObj, "CtrlMapView", zREPOS_NONE );
                     IncludeSubobjectFromSubobject( vViewObj, "CtrlMapView",
                                                    vDlgWk, "ViewObjRef",
                                                    zPOS_BEFORE );
                     TraceLineS("(tzrepoad) Problem logically fixed!","");
                     nFixes++;
                  }
               }
               if ( SetCursorFirstEntityByEntityCsr( vDlgWk, "ViewObjRef",
                     vViewObj, "CtrlMapView", 0 ) >= zCURSOR_SET )
               {
                  GetIntegerFromAttribute( &lZKey, vDlgWk, "LOD", "ZKey" );
                  if ( SetCursorFirstEntityByInteger( vLodList, "W_MetaDef",
                                                      "CPLR_ZKey", lZKey,
                                                      0 ) >= zCURSOR_SET )
                  {
                     ActivateMetaOI( vSubtask, &vLod, vLodList, zREFER_LOD_META, 0 );
                     GetIntegerFromAttribute( &lZKey, vDialog,
                                              "CtrlMapLOD_Entity", "ZKey" );
                     if ( SetCursorFirstEntityByInteger( vLod, "LOD_Entity",
                                                         "ZKey", lZKey,
                                                         "LOD" ) < zCURSOR_SET )
                     {
                        TraceLineS("(tzrepoad) Dialog control mapping found for non-existent LOD Entity","");
                        DisplayEntityInstancePath( vDialog, "CtrlMapLOD_Entity" );
                        DisplayEntityInstance( vDlgWk, "LOD" );
                        IncrementErrorCount( vSubtask, pnError );
                        GetStringFromAttribute( szTag, sizeof( szTag ), vDialog,
                           "CtrlMapLOD_Entity", "Name" );
                        if ( SetCursorFirstEntityByString( vLod,
                              "LOD_Entity", "Name", szTag, 0 ) >= zCURSOR_SET )
                        {
                           ExcludeEntity( vDialog, "CtrlMapLOD_Entity", zREPOS_NONE );
                           IncludeSubobjectFromSubobject( vDialog, "CtrlMapLOD_Entity",
                                                          vLod, "LOD_Entity", zPOS_AFTER );
                           TraceLineS("(tzrepoad) Problem logically fixed!","");
                           nFixes++;
                        }
                     }
                     DropView( vLod );
                  }
               }
            }
            DropView( vViewObj );
         } while ( SetCursorNextEntity( vDialog, "CtrlMapLOD_Entity", 0 ) >= zCURSOR_SET );
      } while ( SetCursorNextEntity( vDialog, "CtrlMap", 0 ) >= zCURSOR_SET );

      // Control validation, Validate actions under control events
      if ( SetCursorFirstEntity( vDialog, "EventAct", "Control" ) >= zCURSOR_SET )
      do
      {
         if ( SetCursorFirstEntityByEntityCsr( vDlgWk, "Action",
                      vDialog, "EventAct", 0 ) != zCURSOR_SET )
         {
            TraceLineS("(tzrepoad) Dialog EventAct references non-existent Action","");
            DisplayEntityInstance( vDlgWk, "Dialog" );
            DisplayEntityInstance( vDlgWk, "Window" );
            DisplayEntityInstance( vDialog, "Control" );
            DisplayEntityInstance( vDialog, "Event" );
            DisplayEntityInstance( vDialog, "EventAct" );
            IncrementErrorCount( vSubtask, pnError );
            GetStringFromAttribute( szTag, sizeof( szTag ), vDialog, "EventAct", "Tag" );
            if ( SetCursorFirstEntityByString( vDlgWk, "Action", "Tag",
                                               szTag, 0 ) >= zCURSOR_SET )
            {
               ExcludeEntity( vDialog, "EventAct", zREPOS_NONE );
               IncludeSubobjectFromSubobject( vDialog, "EventAct",
                                              vDlgWk, "Action", zPOS_AFTER );
               TraceLineS("(tzrepoad) Problem logically fixed!","");
               nFixes++;
            }
         }
      } while ( SetCursorNextEntity( vDialog, "EventAct", "Control" ) >= zCURSOR_SET );

      // Control validation, Validate presentation environment information
      if ( CheckExistenceOfEntity( vDialog, "CtrlColor" ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vDialog, "CtrlColor", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vPe, "Color", "ZKey",
                                             lZKey, 0 ) != zCURSOR_SET )
         {
            TraceLineS("(tzrepoad) Dialog Control Color not found in PE","" );
            DisplayEntityInstance( vDlgWk, "Dialog" );
            DisplayEntityInstance( vDlgWk, "Window" );
            DisplayEntityInstance( vDialog, "Control" );
            DisplayEntityInstance( vDialog, "CtrlColor" );
            IncrementErrorCount( vSubtask, pnError );
         }
      }

#if 0
      if ( CheckExistenceOfEntity( vDialog, "CtrlFont" ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vDialog, "CtrlFont", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vPe, "Color", "ZKey",  // this is WRONG ... DKS 5-14-93
 /* ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */   lZKey, 0 ) != zCURSOR_SET )
         {
            TraceLineS("(tzrepoad) Dialog Control Font not found in PE","" );
            DisplayEntityInstance( vDlgWk, "Dialog" );
            DisplayEntityInstance( vDlgWk, "Window" );
            DisplayEntityInstance( vDialog, "Control" );
            DisplayEntityInstance( vDialog, "CtrlFont" );
            IncrementErrorCount( vSubtask, pnError );
         }
      }
#endif

      // Control validation, Make sure CtrlDef exists and has a
      // corresponding entry in the PE
      if ( CheckExistenceOfEntity( vDialog, "ControlDef" ) == zCURSOR_NULL )
      {
         TraceLineS("(tzrepoad) Dialog control missing a PE CtrlDef","" );
         DisplayEntityInstance( vDlgWk, "Dialog" );
         DisplayEntityInstance( vDlgWk, "Window" );
         DisplayEntityInstance( vDialog, "Control" );
         IncrementErrorCount( vSubtask, pnError );
      }
      else
      {
         GetIntegerFromAttribute( &lZKey, vDialog, "ControlDef", "Key" );
         if ( SetCursorFirstEntityByInteger( vPe, "ControlDef", "Key",
                                             lZKey, 0 ) != zCURSOR_SET )
         {
            TraceLineS("(tzrepoad) Dialog CtrlDef undefined in PE","" );
            DisplayEntityInstance( vDlgWk, "Dialog" );
            DisplayEntityInstance( vDlgWk, "Window" );
            DisplayEntityInstance( vDialog, "Control" );
            DisplayEntityInstance( vDialog, "ControlDef" );
            IncrementErrorCount( vSubtask, pnError );
         }
      }

      // Go recursively down levels to validate subordinate controls
      if ( CheckExistenceOfEntity( vDialog, "CtrlCtrl" ) > zCURSOR_NULL )
      {
         SetViewToSubobject( vDialog, "CtrlCtrl" );
         fnRecursDialogControlValidation( vSubtask, vDialog, vPe, pnError );
         ResetViewFromSubobject( vDialog );
      }
   } while ( SetCursorNextEntity( vDialog, "Control", 0 ) ==
                                                      zCURSOR_SET );
   DropView( vDlgWk );
   DropView( vLodList );
}

void
fnRecursDialogOptionValidation( zVIEW vSubtask,
                                zVIEW vDialog,
                                zVIEW vPe,
                                zPSHORT pnError )
{
   zCHAR szTag[34];
   zVIEW vDlgWk;
   zVIEW vLod;
   zVIEW vLodList;
   zLONG lZKey;

   RetrieveViewForMetaList( vSubtask, &vLodList, zREFER_LOD_META );
   CreateViewFromViewForTask( &vDlgWk, vDialog, 0 );
   while ( ResetViewFromSubobject( vDlgWk ) == 0 );
   if ( SetCursorFirstEntity( vDialog, "Option", 0 ) >= zCURSOR_SET )
   do
   {

      // Option validation, Validate actions under option events
      if ( SetCursorFirstEntity( vDialog, "OptAct", 0 ) >= zCURSOR_SET )
      do
      {
         if ( SetCursorFirstEntityByEntityCsr( vDlgWk, "Action",
                      vDialog, "OptAct", 0 ) != zCURSOR_SET )
         {
            TraceLineS("(tzrepoad) Dialog OptAct references non-existent Action","");
            DisplayEntityInstance( vDlgWk, "Dialog" );
            DisplayEntityInstance( vDlgWk, "Window" );
            DisplayEntityInstance( vDlgWk, "Option" );
            DisplayEntityInstance( vDialog, "Option" );
            DisplayEntityInstance( vDialog, "OptAct" );
            IncrementErrorCount( vSubtask, pnError );
            GetStringFromAttribute( szTag, sizeof( szTag ), vDialog, "OptAct", "Tag" );
            if ( SetCursorFirstEntityByString( vDlgWk, "Action", "Tag",
                                               szTag, 0 ) >= zCURSOR_SET )
            {
               ExcludeEntity( vDialog, "OptAct", zREPOS_NONE );
               IncludeSubobjectFromSubobject( vDialog, "OptAct",
                                              vDlgWk, "Action", zPOS_AFTER );
               TraceLineS("(tzrepoad) Problem logically fixed!","");
               nFixes++;
            }
            else
            if ( !szTag[ 0 ] )
            {
               ExcludeEntity( vDialog, "OptAct", zREPOS_NONE );
               TraceLineS("(tzrepoad) Null OptAct deleted!","");
               nFixes++;
            }
         }
      } while ( SetCursorNextEntity( vDialog, "OptAct", 0 ) >= zCURSOR_SET );

      // Option validation, Make sure all mapping info is good
      if ( SetCursorFirstEntity( vDialog, "OptMap", 0 ) >= zCURSOR_SET )
      do
      {
         if ( SetCursorFirstEntity( vDialog, "OptMapLOD_Attribute", 0 ) >= zCURSOR_SET )
         do
         {
            if ( SetCursorFirstEntity( vDialog, "OptMapView", 0 )
                                                      != zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Dialog option mapping found without a ViewObjRef","" );
               DisplayEntityInstance( vDlgWk, "Dialog" );
               DisplayEntityInstance( vDlgWk, "Window" );
               DisplayEntityInstance( vDialog, "Control" );
               DisplayEntityInstance( vDialog, "OptMap" );
               DisplayEntityInstance( vDialog, "OptMapLOD_Attribute" );
               DisplayEntityInstance( vDialog, "OptMapER_Attribute" );
               DisplayEntityInstance( vDialog, "OptMapRelatedEntity" );
               IncrementErrorCount( vSubtask, pnError );
            }
            else
            {
               if ( SetCursorFirstEntityByEntityCsr( vDlgWk, "ViewObjRef",
                     vDialog, "OptMapView", 0 ) >= zCURSOR_SET )
               {
                  GetIntegerFromAttribute( &lZKey, vDlgWk, "LOD", "ZKey" );
                  if ( SetCursorFirstEntityByInteger( vLodList, "W_MetaDef",
                                                      "CPLR_ZKey", lZKey,
                                                      0 ) >= zCURSOR_SET )
                  {
                     ActivateMetaOI( vSubtask, &vLod, vLodList, zREFER_LOD_META, 0 );
                     GetIntegerFromAttribute( &lZKey, vDialog,
                                              "OptMapLOD_Attribute", "ZKey" );
                     if ( SetCursorFirstEntityByInteger( vLod, "LOD_Attribute",
                                                         "ZKey", lZKey,
                                                         "LOD" ) < zCURSOR_SET )
                     {
                        TraceLineS("(tzrepoad) Dialog option mapping found for non-existent LOD Attr","");
                        DisplayEntityInstance( vDlgWk, "Dialog" );
                        DisplayEntityInstance( vDlgWk, "Window" );
                        DisplayEntityInstance( vDialog, "OptMap" );
                        DisplayEntityInstance( vDialog, "OptMapLOD_Attribute" );
                        DisplayEntityInstance( vDialog, "OptMapER_Attribute" );
                        DisplayEntityInstance( vDialog, "OptMapRelatedEntity" );
                        DisplayEntityInstance( vDlgWk, "LOD" );
                        IncrementErrorCount( vSubtask, pnError );
                        GetStringFromAttribute( szTag, sizeof( szTag ), vDialog,
                           "OptMapRelatedEntity", "Name" );
                        if ( SetCursorFirstEntityByString( vLod,
                              "LOD_Entity", "Name", szTag, 0 ) >= zCURSOR_SET )
                        {
                           GetStringFromAttribute( szTag, sizeof( szTag ), vDialog,
                              "OptMapER_Attribute", "Name" );
                           if ( SetCursorFirstEntityByString( vLod,
                                 "ER_Attribute", "Name", szTag,
                                 "LOD_Entity" ) >= zCURSOR_SET )
                           {
                              ExcludeEntity( vDialog, "OptMapLOD_Attribute", zREPOS_NONE );
                              IncludeSubobjectFromSubobject( vDialog, "OptMapLOD_Attribute",
                                                             vLod, "LOD_Attribute", zPOS_AFTER );
                              TraceLineS("(tzrepoad) Problem logically fixed!","");
                              nFixes++;
                           }
                        }
                     }
                     DropView( vLod );
                  }
               }
               else
               {
                  TraceLineS("(tzrepoad) Dialog option mapping using VOR not included in Dialog","");
                  DisplayEntityInstancePath( vDialog, "OptMapView" );
                  IncrementErrorCount( vSubtask, pnError );
                  GetStringFromAttribute( szTag, sizeof( szTag ), vDialog,
                                          "OptMapView", "Name" );
                  if ( SetCursorFirstEntityByString( vDlgWk, "ViewObjRef",
                                                     "Name", szTag, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vDialog, "OptMapView", zREPOS_NONE );
                     IncludeSubobjectFromSubobject( vDialog, "OptMapView",
                                                    vDlgWk, "ViewObjRef",
                                                    zPOS_BEFORE );
                     TraceLineS("(tzrepoad) Problem logically fixed!","");
                     nFixes++;
                  }
               }
            }
         } while ( SetCursorNextEntity( vDialog, "OptMapLOD_Attribute", 0 ) >= zCURSOR_SET );
      } while ( SetCursorNextEntity( vDialog, "OptMap", 0 ) >= zCURSOR_SET );

      if ( CheckExistenceOfEntity( vDialog, "OptOpt" ) > zCURSOR_NULL )
      {
         SetViewToSubobject( vDialog, "OptOpt" );
         fnRecursDialogOptionValidation( vSubtask, vDialog, vPe, pnError );
         ResetViewFromSubobject( vDialog );
      }
   } while ( SetCursorNextEntity( vDialog, "Option", 0 ) ==
                                                      zCURSOR_SET );
   DropView( vDlgWk );
   DropView( vLodList );
}

zOPER_EXPORT zSHORT OPERATION
zwTZREPOAD_PrepareSystemForLoad( zVIEW    vSubtask )
{
   zBOOL  bFirst;
   zSHORT nError, nRC;
   zCHAR szMsg[ 128 ], szWork[ 33 ], szDerived[ 2 ];
   zVIEW vMetaList;
   zVIEW vMeta;
   zVIEW vDomainList;
   zVIEW vDomain;
   zVIEW vErdList;
   zVIEW vErd;
   zVIEW vPeList;
   zVIEW vPe;
   zVIEW vSaList;
   zVIEW vSa;
   zVIEW vTeList;
   zVIEW vTe;
   zVIEW vLodList;
   zVIEW vLod;
   zVIEW vVorList;
   zVIEW vVor;
   zVIEW vUisList;
   zVIEW vUis;
   zVIEW vDialogList;
   zVIEW vDialog;
   zVIEW vOperList;
   zVIEW vOper;
   zVIEW vWork;
   zLONG lZKey;

// DGC - DOMGRP
// SRC metas no longer valid
#if 0
   zVIEW vSrcList;
   zVIEW vSrc;
#endif

   zVIEW vHeaderList;
   zVIEW vHeader;

   VALIDATE = GetCtrlState( vSubtask, "Validate", zCONTROL_STATUS_CHECKED );
   MARK = GetCtrlState( vSubtask, "Mark", zCONTROL_STATUS_CHECKED );
   PROMPT = GetCtrlState( vSubtask, "Prompt", zCONTROL_STATUS_CHECKED );
   DO_HEADER = GetCtrlState( vSubtask, "Header", zCONTROL_STATUS_CHECKED );

// DGC - DOMGRP
// SRC metas no longer valid
#if 0
   DO_SRC = GetCtrlState( vSubtask, "Source", zCONTROL_STATUS_CHECKED );
#endif

   DO_DOMAIN = GetCtrlState( vSubtask, "Domain", zCONTROL_STATUS_CHECKED );
   DO_OPER = GetCtrlState( vSubtask, "Operation", zCONTROL_STATUS_CHECKED );
   DO_ERD = GetCtrlState( vSubtask, "Model", zCONTROL_STATUS_CHECKED );
   DO_SA = GetCtrlState( vSubtask, "SA", zCONTROL_STATUS_CHECKED );
   DO_TE = GetCtrlState( vSubtask, "TE", zCONTROL_STATUS_CHECKED );
   DO_LOD = GetCtrlState( vSubtask, "LOD", zCONTROL_STATUS_CHECKED );
   DO_VOR = GetCtrlState( vSubtask, "VOR", zCONTROL_STATUS_CHECKED );
   DO_PENV = GetCtrlState( vSubtask, "PE", zCONTROL_STATUS_CHECKED );
   DO_DIALOG = GetCtrlState( vSubtask, "Dialog", zCONTROL_STATUS_CHECKED );
   DO_UIS = GetCtrlState( vSubtask, "UIS", zCONTROL_STATUS_CHECKED );

   SetNameForView( vSubtask, "TZCM_DialogSubtask", vSubtask, zLEVEL_TASK );

   nTotalErrors = 0;
   MB_SetMessage( vSubtask, 2, "" );

   RetrieveViewForMetaList( vSubtask, &vDomainList, zSOURCE_DOMAIN_META );
   RetrieveViewForMetaList( vSubtask, &vErdList, zSOURCE_ERD_META );
   RetrieveViewForMetaList( vSubtask, &vSaList, zSOURCE_SA_META );
   RetrieveViewForMetaList( vSubtask, &vLodList, zSOURCE_LOD_META );
   RetrieveViewForMetaList( vSubtask, &vVorList, zSOURCE_VOR_META );
   RetrieveViewForMetaList( vSubtask, &vUisList, zSOURCE_UIS_META );
   RetrieveViewForMetaList( vSubtask, &vDialogList, zSOURCE_DIALOG_META );
   RetrieveViewForMetaList( vSubtask, &vPeList, zREFER_PENV_META );
   RetrieveViewForMetaList( vSubtask, &vTeList, zREFER_DTE_META );
   RetrieveViewForMetaList( vSubtask, &vOperList, zREFER_GO_META );
   RetrieveViewForMetaList( vSubtask, &vHeaderList, zREFER_HDR_META );

// DGC - DOMGRP
// SRC metas no longer valid
#if 0
   RetrieveViewForMetaList( vSubtask, &vSrcList, zREFER_SRC_META );
#endif

 if ( DO_DOMAIN )
 {
   // Domain marking, for each domain, activate it and mark each domain
   // for insertion
   vMetaList = vDomainList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating Domain " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vDomain, vMetaList, zSOURCE_DOMAIN_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Validating Domain " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );

#if 0   // DKS  undefined vSrcList ... DG ... should this be removed???
         if ( SetCursorFirstEntity( vDomain, "SourceFile", "Domain" )
                                                >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vDomain,
                                        "SourceFile", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vSrcList, "W_MetaDef",
                                                   "CPLR_ZKey", lZKey,
                                                   0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) SourceFile not found for Domain","" );
                  DisplayEntityInstance( vDomain, "SourceFile" );
                  DisplayEntityInstance( vDomain, "Domain" );
                  IncrementErrorCount( vSubtask, &nError );
                  ExcludeEntity( vDomain, "SourceFile", zREPOS_NONE );
                  TraceLineS("(tzrepoad) SourceFile Excluded!","");
                  nFixes++;
               }
            }  while ( SetCursorNextEntity( vDomain,
                        "SourceFile", "Domain" ) >= zCURSOR_SET );
         }
#endif

         // Drop the view to the dialog
         if ( nFixes )
         {
            ResetView( vDomain );
            CommitMetaOI( vSubtask, vDomain, zSOURCE_DOMAIN_META );
         }

         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vDomain, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00101",
                                 "Laucher",
                                 szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL,
                                 zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vDomain );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_DOMAIN_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
         {
            // Drop the view to the dialog
            DropMetaOI( vSubtask, vDomain );
         }
      } while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 )
                                                           >= zCURSOR_SET );
   }

   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating Domain " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vMeta, vDomainList, zSOURCE_DOMAIN_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking Domain " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         SetIncrementalUpdateFlags( vMeta, 0,
                              zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         SetIncrementalUpdateFlags( vMeta, "SourceFile",
                              zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "Operation",
                              zSET_INCR_INCLUDED );
         if ( SetCursorFirstEntity( vMeta, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vMeta, "Z_MetaDef", zREPOS_NONE );
         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting Domain " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                    "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vMeta, zSOURCE_DOMAIN_META );
         }
         // Drop the update view
         DropView( vMeta );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 )
                                                           >= zCURSOR_SET );
   }
 }

 if ( DO_ERD )
 {
   if ( GetViewByName( &vErd, "REFERD", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vErd );
   // Activate the ER Model
   MB_SetMessage( vSubtask, 1, "Activating the ER Model" );
   ActivateMetaOI( vSubtask, &vErd, vErdList, zSOURCE_ERD_META, 0 );

   // Validate er model, make sure every domain in the model also
   // exists in the domain list
   vMeta = vErd;
   vMetaList = vErdList;
   if ( VALIDATE )
   {
      nError = 0;
      nFixes = 0;
      MB_SetMessage( vSubtask, 1, "Validating the ER Model" );
      TraceLineS("(tzrepoad) ","Validating the ER Model" );
      if ( SetCursorFirstEntity( vErd, "Domain", "EntpER_Model" )
           >= zCURSOR_SET )
      do
      {
         // Retrieve the ZKey of the domain and look it up in
         // the domain meta list
         GetIntegerFromAttribute( &lZKey, vErd, "Domain", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vDomainList, "W_MetaDef",
                                             "CPLR_ZKey", lZKey,
                                             0 ) < zCURSOR_SET )
         {
            TraceLineS("(tzrepoad) Domain not found for ER Attr","" );
            DisplayObjectInstance( vErd, "ER_Attribute", 0 );
            IncrementErrorCount( vSubtask, &nError );
         }

      } while ( SetCursorNextEntity( vErd, "Domain", "EntpER_Model" )
                                                            >= zCURSOR_SET );
      nError -= nFixes;
      if ( nError && PROMPT )
      {
         SetNameForView( vErd, "ERRORMETA", vSubtask, zLEVEL_TASK );
         zltoa( nError, szMsg, sizeof( szMsg ) );
         strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
         nRC = MessagePrompt( vSubtask, "RE00102",
                              "Laucher",
                              szMsg,
                              zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_NO,  0 );
         if ( nRC == zRESPONSE_YES )
         {
            DeleteMetaOI( vSubtask, vMetaList, zSOURCE_ERD_META );
            SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 );
         }
         else
         if ( nRC == zRESPONSE_CANCEL )
         {
            SfCreateSubtask( &vSubtask, vSubtask, 0 );
            SfDropSubtask( vSubtask, 1 );
            return( 0 );
         }
      }
   }

   // Enterprise model marking, mark all entities for insert except
   // domains, mark all domains for include
   if ( MARK )
   {
      if ( SetCursorFirstEntity( vErd, "Domain", "EntpER_Model" ) >= zCURSOR_SET )
      {
         MB_SetMessage( vSubtask, 1, "Marking the ER Model" );
         SetIncrementalUpdateFlags( vMeta, 0, zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vMeta, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vMeta, "Z_MetaDef", zREPOS_NONE );
         SetIncrementalUpdateFlags( vMeta, "Domain", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ER_AttributeIdentifier", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ER_RelLinkIdentifier", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ER_Entity_2", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         if ( COMMIT )
         {
            MB_SetMessage( vSubtask, 1, "Commiting the ER Model" );
            CommitMetaOI( vSubtask, vMeta, zSOURCE_ERD_META );
         }
      }
   }

   // Drop the update view to the ERD and get a reference view
   DropMetaOI( vSubtask, vErd );
 }
   ActivateMetaOI( vSubtask, &vErd, vErdList, zREFER_ERD_META, 0 );
   SetNameForView( vErd, "REFERD", vSubtask, zLEVEL_TASK );


 if ( DO_SA )
 {
   // SA
   vMetaList = vSaList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 )
           >= zCURSOR_SET )

      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating SA " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vSa, vMetaList, zSOURCE_SA_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Validating SA " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         TraceLineS("(tzrepoad) ", szMsg );

         GetIntegerFromAttribute( &lZKey, vSa, "EntpER_Model", "ZKey" );
         if ( CompareAttributeToInteger( vErd, "EntpER_Model", "ZKey", lZKey ) != 0 )
         {
            TraceLineS("(tzrepoad) SA EntpER_Model ZKey mismatch","" );
            DisplayEntityInstance( vSa, "SubjectArea" );
            DisplayEntityInstance( vSa, "EntpER_Model" );
            DisplayEntityInstance( vErd, "EntpER_Model" );
            IncrementErrorCount( vSubtask, &nError );
         }
         else
         {
            if ( SetCursorFirstEntity( vSa, "ER_Entity", "SubjectArea" )
                 >= zCURSOR_SET )
            {
               do
               {
                  GetIntegerFromAttribute( &lZKey, vSa, "ER_Entity", "ZKey" );
                  if ( SetCursorFirstEntityByInteger( vErd, "ER_Entity", "ZKey", lZKey, "EntpER_Model" ) < zCURSOR_SET )
                  {
                     TraceLineS("(tzrepoad) SA Entity not found in model","" );
                     DisplayEntityInstance( vSa, "SubjectArea" );
                     DisplayEntityInstance( vSa, "SA_Entity" );
                     DisplayEntityInstance( vSa, "ER_Entity" );
                     IncrementErrorCount( vSubtask, &nError );

                     DeleteEntity( vSa, "SA_Entity", zREPOS_NONE );
                     TraceLineS("(tzrepoad) SA Entity deleted!","");
                     nFixes++;
                  }
               }  while ( SetCursorNextEntity( vSa, "ER_Entity", "SubjectArea" ) >= zCURSOR_SET );
            }
            if ( SetCursorFirstEntity( vSa, "ER_RelType", "SubjectArea" )
                 >= zCURSOR_SET )
            {
               do
               {
                  GetIntegerFromAttribute( &lZKey, vSa, "ER_RelType", "ZKey" );
                  if ( SetCursorFirstEntityByInteger( vErd, "ER_RelType", "ZKey", lZKey, "EntpER_Model" ) < zCURSOR_SET )
                  {
                     TraceLineS("(tzrepoad) SA RelType not found in model","" );
                     DisplayEntityInstance( vSa, "SubjectArea" );
                     DisplayEntityInstance( vSa, "SA_RelType" );
                     DisplayEntityInstance( vSa, "ER_RelType" );
                     IncrementErrorCount( vSubtask, &nError );

                     DeleteEntity( vSa, "SA_RelType", zREPOS_NONE );
                     TraceLineS("(tzrepoad) SA Reltype deleted!","");
                     nFixes++;
                  }
               }  while ( SetCursorNextEntity( vSa, "ER_RelType", "SubjectArea" ) >= zCURSOR_SET );
            }
         }

         if ( nFixes )
            CommitMetaOI( vSubtask, vSa, zSOURCE_SA_META );

         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vSa, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00103", "Laucher", szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL, zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vSa );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_SA_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
         {
            DropMetaOI( vSubtask, vSa );
         }

      }  while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }

   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )

      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating SA " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vMeta, vMetaList, zSOURCE_SA_META, 0 );

         // Patch
         if ( SetCursorFirstEntity( vMeta, "ER_RelType", "SubjectArea" ) >= zCURSOR_SET )
         {
            do
            {
               zgSortEntityWithinParent( zASCENDING, vMeta, "ER_RelLink_2", "ZKey", 0 );

            } while ( SetCursorNextEntity( vMeta, "ER_RelType", "SubjectArea" ) >= zCURSOR_SET );
         }

         strcpy_s( szMsg, sizeof( szMsg ), "Marking SA " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );

         SetIncrementalUpdateFlags( vMeta, 0,
                              zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vMeta, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vMeta, "Z_MetaDef", zREPOS_NONE );
         SetIncrementalUpdateFlags( vMeta, "ER_Entity", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "OwnedER_Entity", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ER_RelType", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "OwnedER_RelType", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "EntpER_Model", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting SA " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vMeta, zSOURCE_SA_META );
         }
         DropMetaOI( vSubtask, vMeta );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
 }

 if ( DO_TE )
 {
   // TE
   vMetaList = vTeList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )

      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating TE " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vTe, vMetaList, zSOURCE_DTE_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Validating TE " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         TraceLineS( "(tzrepoad) ", szMsg );

         if ( SetCursorFirstEntity( vTe, "ER_Attribute", "TE_DB_Environ" ) >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vTe, "ER_Attribute", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vErd, "ER_Attribute",
                                                   "ZKey", lZKey,
                                                   "EntpER_Model" ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) TE Attribute not found in model","" );
                  DisplayEntityInstance( vTe, "TE_DB_Environ" );
                  DisplayEntityInstance( vTe, "TE_DBMS_Source" );
                  DisplayEntityInstance( vTe, "TE_TablRec" );
                  DisplayEntityInstance( vTe, "TE_FieldDataRel" );
                  DisplayEntityInstance( vTe, "ER_Attribute" );
                  DisplayEntityInstance( vTe, "ER_Entity" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }  while ( SetCursorNextEntity( vTe, "ER_Attribute", "TE_DB_Environ" ) >= zCURSOR_SET );
         }

         if ( SetCursorFirstEntity( vTe, "ER_RelLink", "TE_DB_Environ" ) >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vTe, "ER_RelLink", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vErd, "ER_RelLink", "ZKey", lZKey, "EntpER_Model" ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) TE RelLink not found in model","" );
                  DisplayEntityInstance( vTe, "TE_DB_Environ" );
                  DisplayEntityInstance( vTe, "TE_DBMS_Source" );
                  DisplayEntityInstance( vTe, "TE_TablRec" );
                  DisplayEntityInstance( vTe, "TE_FieldDataRel" );
                  DisplayEntityInstance( vTe, "ER_RelLink" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }  while ( SetCursorNextEntity( vTe, "ER_RelLink", "TE_DB_Environ" ) >= zCURSOR_SET );
         }

         if ( SetCursorFirstEntity( vTe, "ER_Entity", "TE_DB_Environ" ) >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vTe, "ER_Entity", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vErd, "ER_Entity", "ZKey", lZKey, "EntpER_Model" ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) TE ER_Entity not found in model","" );
                  DisplayEntityInstance( vTe, "TE_DB_Environ" );
                  DisplayEntityInstance( vTe, "TE_DBMS_Source" );
                  DisplayEntityInstance( vTe, "TE_TablRec" );
                  DisplayEntityInstance( vTe, "ER_Entity" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }  while ( SetCursorNextEntity( vTe, "ER_Entity", "TE_DB_Environ" ) >= zCURSOR_SET );
         }

         if ( SetCursorFirstEntity( vTe, "ER_RelType", "TE_DB_Environ" ) >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vTe, "ER_RelType", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vErd, "ER_RelType",
                                                   "ZKey", lZKey,
                                                   "EntpER_Model" ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) TE ER_RelType not found in model","" );
                  DisplayEntityInstance( vTe, "TE_DB_Environ" );
                  DisplayEntityInstance( vTe, "TE_DBMS_Source" );
                  DisplayEntityInstance( vTe, "TE_TablRec" );
                  DisplayEntityInstance( vTe, "ER_RelType" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }  while ( SetCursorNextEntity( vTe,"ER_RelType", "TE_DB_Environ" ) >= zCURSOR_SET );
         }

         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vTe, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00104",
                                 "Laucher",
                                 szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL,
                                 zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vTe );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_DTE_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
            DropMetaOI( vSubtask, vTe );
      }  while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }


   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 )
           >= zCURSOR_SET )

      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating TE " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vMeta, vMetaList, zSOURCE_DTE_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking TE " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );

         SetIncrementalUpdateFlags( vMeta, 0, zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vMeta, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vMeta, "Z_MetaDef", zREPOS_NONE );
         SetIncrementalUpdateFlags( vMeta, "ER_Attribute", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ER_RelLink", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ER_Entity", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ER_RelType", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ER_EntIdentifier", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "TE_FieldDataRelKey", zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "TE_FieldDataRelAsFK", zSET_INCR_INCLUDED );
// DGC 12/4/96  Removed following entities from object.
#if 0
         SetIncrementalUpdateFlags( vMeta, "TE_TablRecAccessed", zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "TE_TablRecStorage", zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "TE_DBMS_SourceAvail", zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "SubjectArea", zSET_INCR_INCLUDED );
#endif
         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting TE " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vMeta, zSOURCE_DTE_META );
         }
         DropMetaOI( vSubtask, vMeta );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
 }


 if ( DO_LOD )
 {
   // LOD
   vMetaList = vLodList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating LOD " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vLod, vLodList, zSOURCE_LOD_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Validating LOD " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         TraceLineS("(tzrepoad) ", szMsg );

         // LOD Validation 1, make sure all domains in the LOD exist in the domain list
         SetCursorFirstEntity( vLod, "Domain", "LOD" );
         do
         {
            // Retrieve the ZKey of the domain and look it up in the domain meta list
            GetIntegerFromAttribute( &lZKey, vLod, "Domain", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vDomainList, "W_MetaDef",
                                                "CPLR_ZKey", lZKey,
                                                0 ) < zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Domain not found for LOD Attr","" );
               DisplayObjectInstance( vLod, "LOD_Attribute", 0 );
               IncrementErrorCount( vSubtask, &nError );
            }

         } while ( SetCursorNextEntity( vLod, "Domain", "LOD" ) >= zCURSOR_SET );

         // LOD Validation 2, make sure all LOD Entities which aren't
         // work entities have a corresponding entity in the model
         SetCursorFirstEntity( vLod, "LOD_Entity", 0 );
         do
         {
            // Retrieve the ZKey of the ER_Entity and look it up in
            // the model
            GetStringFromAttribute( szWork, sizeof( szWork ), vLod, "LOD_Entity", "Work" );
            lZKey = 0;
            if ( CheckExistenceOfEntity( vLod, "ER_Entity" ) == zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lZKey, vLod, "ER_Entity", "ZKey" );
               if ( szWork[ 0 ] == 'Y' )
               {
                  TraceLineS("(tzrepoad) LOD Work Entity with ER Entity found","" );
                  DisplayEntityInstance( vLod, "LOD" );
                  DisplayEntityInstance( vLod, "LOD_Entity" );
                  DisplayEntityInstance( vLod, "ER_Entity" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }
            else
            if ( szWork[ 0 ] != 'Y' )
            {
               TraceLineS("(tzrepoad) LOD Entity w/o ER Entity found","" );
               DisplayEntityInstance( vLod, "LOD" );
               DisplayEntityInstance( vLod, "LOD_Entity" );
               IncrementErrorCount( vSubtask, &nError );
               szWork[ 0 ] = 'Y';
            }

            if ( SetCursorFirstEntityByInteger( vErd, "ER_Entity", "ZKey", lZKey, 0 ) < zCURSOR_SET )
            {
               if ( szWork[ 0 ] != 'Y' )
               {
                  TraceLineS("(tzrepoad) LOD Entity not found in ER Model","" );
                  DisplayEntityInstance( vLod, "LOD" );
                  DisplayEntityInstance( vLod, "LOD_Entity" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }
            else
            {
               if ( szWork[ 0 ] == 'Y' )
               {
                  TraceLineS("(tzrepoad) LOD Work Entity found in ER Model","" );
                  DisplayEntityInstance( vLod, "LOD" );
                  DisplayEntityInstance( vLod, "LOD_Entity" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }

            // Retrieve the ZKey of the ER_RelLink and look it up in
            // the model
            GetStringFromAttribute( szWork, sizeof( szWork ), vLod, "LOD_Entity", "Work" );
            GetStringFromAttribute( szDerived, sizeof( szDerived ), vLod, "LOD_Entity", "Derived" );
            lZKey = 0;
            if ( CheckExistenceOfEntity( vLod, "ER_RelLink" ) == zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lZKey, vLod, "ER_RelLink", "ZKey" );
               if ( szWork[ 0 ] == 'Y' )
               {
                  TraceLineS("(tzrepoad) LOD Work Entity with ER RelLink found","" );
                  DisplayEntityInstance( vLod, "LOD" );
                  DisplayEntityInstance( vLod, "LOD_Entity" );
                  DisplayEntityInstance( vLod, "ER_RelLink" );
                  IncrementErrorCount( vSubtask, &nError );
               }
               if ( szDerived[ 0 ] == 'Y' )
               {
                  TraceLineS("(tzrepoad) LOD Derived Entity with ER RelLink found","" );
                  DisplayEntityInstance( vLod, "LOD" );
                  DisplayEntityInstance( vLod, "LOD_Entity" );
                  DisplayEntityInstance( vLod, "ER_RelLink" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }
            else
            if (( szWork[ 0 ] != 'Y' ) && ( szDerived[ 0 ] != 'Y' ) &&
               ( CompareAttributeToInteger( vLod, "LOD_Entity", "IndentLvl", 0 ) != 0 ))
            {
               TraceLineS("(tzrepoad) LOD Entity w/o ER RelLink found","" );
               DisplayEntityInstance( vLod, "LOD" );
               DisplayEntityInstance( vLod, "LOD_Entity" );
               IncrementErrorCount( vSubtask, &nError );
               szWork[ 0 ] = 'Y';
            }
            if ( SetCursorFirstEntityByInteger( vErd, "ER_RelLink", "ZKey", lZKey, "EntpER_Model" ) < zCURSOR_SET )
            {
               if ( (szWork[ 0 ] != 'Y') && (szDerived[ 0 ] != 'Y') &&
                    (CompareAttributeToInteger( vLod, "LOD_Entity", "IndentLvl", 0 ) != 0) )
               {
                  TraceLineS("(tzrepoad) LOD Entity not found in ER Model","" );
                  DisplayEntityInstance( vLod, "LOD" );
                  DisplayEntityInstance( vLod, "LOD_Entity" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }
            else
            {
               if (( szWork[ 0 ] == 'Y' ) || ( szDerived[ 0 ] == 'Y' ))
               {
                  TraceLineS("(tzrepoad) LOD Work or Derived Entity found in ER Model","" );
                  DisplayEntityInstance( vLod, "LOD" );
                  DisplayEntityInstance( vLod, "LOD_Entity" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }

            // For each attribute in the LOD Entity, make sure that it
            // exists in the model or doesn't exist in the model based on its state
            if ( SetCursorFirstEntity( vLod, "LOD_Attribute", 0 ) >= zCURSOR_SET )
            do
            {
               // Retrieve the ZKey of the domain and look it up in
               // the domain meta list
               GetStringFromAttribute( szWork, sizeof( szWork ), vLod, "LOD_Attribute", "Work" );
               lZKey = 0;
               if ( CheckExistenceOfEntity( vLod, "ER_Attribute" ) == zCURSOR_SET )
               {
                  GetIntegerFromAttribute( &lZKey, vLod, "ER_Attribute", "ZKey" );
               }
               else
               if ( szWork[ 0 ] != 'Y' )
               {
                  TraceLineS("(tzrepoad) LOD Attribute w/o ER Attr found","" );
                  DisplayEntityInstance( vLod, "LOD" );
                  DisplayEntityInstance( vLod, "LOD_Entity" );
                  DisplayEntityInstance( vLod, "LOD_Attribute" );
                  IncrementErrorCount( vSubtask, &nError );
                  szWork[ 0 ] = 'Y';
               }

               if ( SetCursorFirstEntityByInteger( vErd, "ER_Attribute",
                                                   "ZKey", lZKey,
                                                   "EntpER_Model" ) < zCURSOR_SET )
               {
                  if ( szWork[ 0 ] != 'Y' )
                  {
                     TraceLineS("(tzrepoad) LOD Attribute not found in ER Model","" );
                     DisplayEntityInstance( vLod, "LOD" );
                     DisplayEntityInstance( vLod, "LOD_Entity" );
                     DisplayEntityInstance( vLod, "LOD_Attribute" );
                     DisplayEntityInstance( vLod, "ER_Attribute" );
                     IncrementErrorCount( vSubtask, &nError );
                  }
               }
               else
               {
                  if ( szWork[ 0 ] == 'Y' )
                  {
                     TraceLineS("(tzrepoad) LOD Work Attribute found in ER Model","" );
                     DisplayEntityInstance( vLod, "LOD" );
                     DisplayEntityInstance( vLod, "LOD_Entity" );
                     DisplayEntityInstance( vLod, "LOD_Attribute" );
                     DisplayEntityInstance( vLod, "ER_Attribute" );
                     IncrementErrorCount( vSubtask, &nError );
                  }
               }
            } while ( SetCursorNextEntity( vLod, "LOD_Attribute", 0 )
                                                            >= zCURSOR_SET );
         } while ( SetCursorNextEntity( vLod, "LOD_Entity", 0 )
                                                         >= zCURSOR_SET );

#if 0   // DKS  undefined vSrcList ... DG ... should this be removed???
         if ( SetCursorFirstEntity( vLod, "SourceFile", "LOD" ) >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vLod, "SourceFile", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vSrcList, "W_MetaDef", "CPLR_ZKey", lZKey, 0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) SourceFile not found for Lod","" );
                  DisplayEntityInstance( vLod, "SourceFile" );
                  DisplayEntityInstance( vLod, "LOD" );
                  IncrementErrorCount( vSubtask, &nError );
                  ExcludeEntity( vLod, "SourceFile", zREPOS_NONE );
                  TraceLineS("(tzrepoad) SourceFile Excluded!","");
                  nFixes++;
               }
            }  while ( SetCursorNextEntity( vLod, "SourceFile", "LOD" ) >= zCURSOR_SET );
         }
#endif

         // Drop the view to the lod
         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vLod, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00105",
                                 "Laucher",
                                 szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL,
                                 zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vLod );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_LOD_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
            DropMetaOI( vSubtask, vLod );
      }  while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 )
           >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating LOD " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vLod, vLodList, zSOURCE_LOD_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking LOD " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         SetIncrementalUpdateFlags( vLod, 0, zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vLod, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vLod, "Z_MetaDef", zREPOS_NONE );
         SetIncrementalUpdateFlags( vLod, "DomainRec", zSET_INCR_PERSISTENT | zSET_INCR_NOT_INCLUDED );
         SetIncrementalUpdateFlags( vLod, "TE_DB_Environ", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         fnRecursLodEntityMarking( vLod );

         SetIncrementalUpdateFlags( vLod, "SourceFile", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vLod, "LOD_ConstraintOper", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting LOD " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vLod, zSOURCE_LOD_META );
         }

         // Drop the view to the lod
         DropMetaOI( vSubtask, vLod );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
 }

 if ( DO_UIS )
 {
   if ( GetViewByName( &vPe, "TZPESRCO", vSubtask, zLEVEL_TASK ) < 1 )
   {
      ActivateMetaOI( vSubtask, &vPe, vPeList, zREFER_PENV_META, 0 );
      SetNameForView( vPe, "TZPESRCO", vSubtask, zLEVEL_TASK );
   }
   // UIS
   vMetaList = vUisList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating UIS " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vUis, vMetaList, zSOURCE_UIS_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Validating UIS " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         TraceLineS("(tzrepoad) ", szMsg );

         // UIS Validation
         fnRecursUIS_EntityValidation( vSubtask, vUis, vPe, &nError );
         while ( ResetViewFromSubobject( vUis ) == 0 );

         // Validate default window design
         if ( SetCursorFirstEntity( vUis, "DfltWndStyle", "UI_Spec" ) >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lZKey, vUis, "DfltWndStyle", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vPe, "WindowStyle", "ZKey", lZKey, 0 ) < zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) UIS DfltWndStyle not found in Presentation Environment","" );
               DisplayEntityInstance( vUis, "UI_Spec" );
               DisplayEntityInstance( vUis, "DfltWndDesign" );
               DisplayEntityInstance( vUis, "DfltWndStyle" );
               IncrementErrorCount( vSubtask, &nError );
            }
         }

         if ( SetCursorFirstEntity( vUis, "UIS_ViewObjRef", 0 ) >= zCURSOR_SET )
         do
         {
            GetIntegerFromAttribute( &lZKey, vUis, "UIS_ViewObjRef", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vVorList, "W_MetaDef",
                                                "CPLR_ZKey", lZKey, 0 ) < zCURSOR_SET )
            {
               vDialog = 0;   // initialize to something DKS 2000.03.09?
               TraceLineS("(tzrepoad) UIS_ViewObjRef not found for UIS","" );
               DisplayEntityInstance( vDialog, "UI_Spec" );  // uninitialized???
               DisplayEntityInstance( vDialog, "UIS_LOD" );
               IncrementErrorCount( vSubtask, &nError );
            }
         }
         while ( SetCursorNextEntity( vUis, "UIS_ViewObjRef", 0 ) >= zCURSOR_SET );

         if ( SetCursorFirstEntity( vUis, "ListVOR", 0 ) >= zCURSOR_SET )
         do
         {
            GetIntegerFromAttribute( &lZKey, vUis, "ListVOR", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vVorList, "W_MetaDef",
                                                "CPLR_ZKey", lZKey, 0 ) < zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) ListVOR not found for UIS","" );
               DisplayEntityInstance( vDialog, "UI_Spec" );
               DisplayEntityInstance( vDialog, "UIS_LOD" );
               IncrementErrorCount( vSubtask, &nError );
            }
         }
         while ( SetCursorNextEntity( vUis, "ListVOR", 0 ) >= zCURSOR_SET );

         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vUis, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00106", "Laucher", szMsg, zBEEP,
                                 zBUTTONS_YESNOCANCEL, zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vUis );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_UIS_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
            DropMetaOI( vSubtask, vUis );
      }  while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }

   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating UIS " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vUis, vMetaList, zSOURCE_UIS_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking UIS " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );

         // UIS Marking
         SetIncrementalUpdateFlags( vUis, 0,
                              zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vUis, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vUis, "Z_MetaDef", zREPOS_NONE );
         SetIncrementalUpdateFlags( vUis, "LOD_Entity", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vUis, "WndStyle", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vUis, "M_LOD_Attribute", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vUis, "L_LOD_Attribute", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vUis, "SubObjViewObjRef", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vUis, "DfltWndStyle", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vUis, "UIS_ViewObjRef", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vUis, "ListVOR", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting UIS " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vUis, zSOURCE_UIS_META );
         }
         DropMetaOI( vSubtask, vUis );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
 }


 if ( DO_PENV )
 {
   // Presentation Environment marking
//PETTIT
   if ( GetViewByName( &vPe, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 )
      DropMetaOI( vSubtask, vPe );

   vMetaList = vPeList;
   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 )
           >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating Presentation Env " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vMeta, vPeList, zSOURCE_PENV_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking Presentation Env " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         SetIncrementalUpdateFlags( vMeta, 0, zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vMeta, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vMeta, "Z_MetaDef", zREPOS_NONE );
         SetIncrementalUpdateFlags( vMeta, "ValidChild", zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ValidParent", zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ConvertsSelfToChild", zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vMeta, "ConvertableFrom", zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vMeta, "WndColor", zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vMeta, "WndFont", zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vMeta, "CtrlColor", zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vMeta, "CtrlFont", zSET_INCR_INCLUDED );
         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting Presentation Env " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vMeta, zSOURCE_PENV_META );
         }

         // Drop the update view
         DropView( vMeta );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
 }

 if ( DO_VOR )
 {
   // VOR
   vMetaList = vVorList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 )
           >= zCURSOR_SET )
      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating VOR " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vVor, vMetaList, zREFER_VOR_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Validating VOR " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcpy_s( szWork, sizeof( szWork ), &szMsg[ 80 ] );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         TraceLineS("(tzrepoad) ", szMsg );

         if ( PROMPT )
         {
            CreateViewFromViewForTask( &vWork, vMetaList, 0 );
            if ( SetCursorNextEntityByString( vWork, "W_MetaDef", "Name",
                                              szWork, 0 ) >= zCURSOR_SET &&
                 MessagePrompt( vSubtask, "RE00107", "Laucher",
                                "Duplicate Vors found, shall I Delete them?",
                                 zBEEP, zBUTTONS_YESNO,
                                 zRESPONSE_NO,  0 )   == zRESPONSE_YES )
            {
               do
               {
                  DeleteMetaOI( vSubtask, vWork, zSOURCE_VOR_META );
                  SetViewFromView( vWork, vMetaList );

               } while ( SetCursorNextEntityByString( vWork, "W_MetaDef",
                                                      "Name", szWork,
                                                      0 ) >= zCURSOR_SET );
            }

            DropView( vWork );
         }

         // VOR Validation 1, make sure the LOD Exists and is in the
         // LOD List
         if ( CheckExistenceOfEntity( vVor, "LOD" ) >= zCURSOR_SET )
         {
            // Retrieve the ZKey of the LOD and look it up in
            // the LOD list
            GetIntegerFromAttribute( &lZKey, vVor, "LOD", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vLodList, "W_MetaDef",
                                                "CPLR_ZKey", lZKey,
                                                0 ) < zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) LOD not found for VOR","" );
               DisplayObjectInstance( vVor, 0, 0 );
               IncrementErrorCount( vSubtask, &nError );
            }
         }
         else
         {
            TraceLineS("(tzrepoad) VOR does not contain a LOD","" );
            DisplayObjectInstance( vVor, 0, 0 );
            IncrementErrorCount( vSubtask, &nError );
         }

         // Drop the view to the lod
         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vVor, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00108", "Laucher", szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL, zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vVor );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_VOR_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
            DropMetaOI( vSubtask, vVor );
      }  while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }

   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 )
           >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating VOR " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vVor, vMetaList, zSOURCE_VOR_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking VOR " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         SetIncrementalUpdateFlags( vVor, 0, zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vVor, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vVor, "Z_MetaDef", zREPOS_NONE );
         SetIncrementalUpdateFlags( vVor, "LOD", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );


         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting VOR " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vVor, zSOURCE_VOR_META );
         }

         // Drop the view to the lod
         DropMetaOI( vSubtask, vVor );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
 }


 if ( DO_DIALOG )
 {
   if ( GetViewByName( &vPe, "TZPESRCO", vSubtask, zLEVEL_TASK ) < 1 )
   {
      ActivateMetaOI( vSubtask, &vPe, vPeList, zREFER_PENV_META, 0 );
      SetNameForView( vPe, "TZPESRCO", vSubtask, zLEVEL_TASK );
   }
   // Dialog
   vMetaList = vDialogList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 )
           >= zCURSOR_SET )
      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating Dialog " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vDialog, vMetaList, zSOURCE_DIALOG_META, 0 );
         nFixes = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Validating Dialog " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         TraceLineS("(tzrepoad) ", szMsg );

         // Validation, make sure the DfltWnd is a window in the dialog
         if ( SetCursorFirstEntity( vDialog, "DfltWnd", 0 ) >= zCURSOR_SET &&
              SetCursorFirstEntityByEntityCsr( vDialog, "Window",
                     vDialog, "DfltWnd", 0 ) != zCURSOR_SET )
         {
            TraceLineS("(tzrepoad) Default window not found for Dialog","" );
            DisplayEntityInstance( vDialog, "Dialog" );
            DisplayEntityInstance( vDialog, "DfltWnd" );
            IncrementErrorCount( vSubtask, &nError );
            GetStringFromAttribute( szMsg, sizeof( szMsg ), vDialog, "DfltWnd", "Tag" );
            if ( SetCursorFirstEntityByString( vDialog, "Window", "Tag",
                                               szMsg, 0 ) >= zCURSOR_SET )
            {
               ExcludeEntity( vDialog, "DfltWnd", zREPOS_NONE );
               IncludeSubobjectFromSubobject( vDialog, "DfltWnd",
                                              vDialog, "Window", zPOS_AFTER );
               nFixes++;
               TraceLineS("(tzrepoad) Problem logically fixed!","");
            }
         }

         // Validation, make sure every ViewObjRef in the dialog is
         // in the VOR list
         if ( SetCursorFirstEntity( vDialog, "ViewObjRef", 0 ) >= zCURSOR_SET )
         {
            do
            {
               // Retrieve the ZKey of the ViewObjRef and look it up in
               // the VOR list
               GetIntegerFromAttribute( &lZKey, vDialog, "ViewObjRef", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vVorList, "W_MetaDef",
                                                   "CPLR_ZKey", lZKey,
                                                   0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) VOR not found for Dialog","" );
                  DisplayEntityInstance( vDialog, "Dialog" );
                  DisplayEntityInstance( vDialog, "ViewObjRef" );
                  DisplayEntityInstance( vDialog, "LOD" );
                  IncrementErrorCount( vSubtask, &nError );
                  // See if we can fix the problem
                  GetStringFromAttribute( szWork, sizeof( szWork ), vDialog,
                                          "ViewObjRef", "Name" );
                  if ( SetCursorFirstEntityByString( vVorList, "W_MetaDef",
                                                     "Name", szWork,
                                                     0 ) >= zCURSOR_SET &&
                       ActivateMetaOI( vSubtask, &vVor, vVorList, zREFER_VOR_META, 0 ) >= 0 )
                  {
                     ExcludeEntity( vDialog, "ViewObjRef", zREPOS_NONE );
                     IncludeSubobjectFromSubobject( vDialog, "ViewObjRef",
                                                    vVor, "ViewObjRef",
                                                    zPOS_BEFORE );
                     DropMetaOI( vSubtask, vVor );
                     TraceLineS("(tzrepoad) problem logically fixed!","" );
                     nFixes++;
                  }
               }
            }  while ( SetCursorNextEntity( vDialog, "ViewObjRef",
                                            0 ) >= zCURSOR_SET );
         }

         // Navigate each window in the dialog and call recursive option
         // and control validation functions
         if ( SetCursorFirstEntity( vDialog, "Window", 0 ) >= zCURSOR_SET )
         do
         {
            fnRecursDialogControlValidation( vSubtask, vDialog, vPe, &nError );
            while ( ResetViewFromSubobject( vDialog ) == 0 );


            // Window validation, validate that all WndActs exist
            if ( SetCursorFirstEntity( vDialog, "WndAct", "Window" ) >= zCURSOR_SET )
            do
            {
               if ( SetCursorFirstEntityByEntityCsr( vDialog, "Action",
                           vDialog, "WndAct", 0 ) != zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) Dialog WndAct references non-existent Action","");
                  DisplayEntityInstance( vDialog, "Dialog" );
                  DisplayEntityInstance( vDialog, "Window" );
                  DisplayEntityInstance( vDialog, "WndEvent" );
                  DisplayEntityInstance( vDialog, "WndAct" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            } while ( SetCursorNextEntity( vDialog, "WndAct", "Window" ) >= zCURSOR_SET );

            // Validate Action LOD Entity
            if ( SetCursorFirstEntity( vDialog, "ActMap",
                                       "Window" ) >= zCURSOR_SET )
            {
               do
               {
                  if ( SetCursorFirstEntity( vDialog, "ActMapLOD_Entity", 0 ) >= zCURSOR_SET )
                  do
                  {
                     if ( SetCursorFirstEntity( vDialog, "ActMapView", 0 )
                                                               != zCURSOR_SET )
                     {
                        TraceLineS("(tzrepoad) Window ActMap found without a ViewObjRef","" );
                        DisplayEntityInstance( vDialog, "Dialog" );
                        DisplayEntityInstance( vDialog, "Window" );
                        DisplayEntityInstance( vDialog, "ActMap" );
                        DisplayEntityInstance( vDialog, "ActMapLOD_Entity" );
                        IncrementErrorCount( vSubtask, &nError );
                     }
                     else
                     {
                        if ( SetCursorFirstEntityByEntityCsr( vDialog, "ViewObjRef",
                              vDialog, "ActMapView", 0 ) >= zCURSOR_SET )
                        {
                           GetIntegerFromAttribute( &lZKey, vDialog,
                                                    "LOD", "ZKey" );
                           if ( SetCursorFirstEntityByInteger( vLodList,
                                             "W_MetaDef", "CPLR_ZKey", lZKey, 0 )
                                                                  >= zCURSOR_SET )
                           {
                              ActivateMetaOI( vSubtask, &vLod, vLodList, zREFER_LOD_META, 0 );
                              GetIntegerFromAttribute( &lZKey, vDialog,
                                 "ActMapLOD_Entity", "ZKey" );
                              if ( SetCursorFirstEntityByInteger( vLod,
                                    "LOD_Entity", "ZKey", lZKey, 0 ) < zCURSOR_SET )
                              {
                                 TraceLineS("(tzrepoad) ActMap LOD Entity not found in corresponding LOD","");
                                 DisplayEntityInstancePath( vDialog, "ActMapLOD_Entity" );
                                 DisplayEntityInstance( vDialog, "LOD" );
                                 IncrementErrorCount( vSubtask, &nError );
                                 GetStringFromAttribute( szWork, sizeof( szWork ), vDialog,
                                             "ActMapLOD_Entity", "Name" );
                                 if ( SetCursorFirstEntityByString( vLod,
                                      "LOD_Entity", "Name", szWork, 0 ) >= zCURSOR_SET )
                                 {
                                    ExcludeEntity( vDialog,
                                         "ActMapLOD_Entity", zREPOS_NONE );
                                    IncludeSubobjectFromSubobject( vDialog,
                                         "ActMapLOD_Entity", vLod, "LOD_Entity",
                                         zPOS_AFTER );
                                    TraceLineS("(tzrepoad) Problem logically fixed!","");
                                    nFixes++;
                                 }
                              }
                              DropView( vLod );
                           }
                        }
                        else
                        {
                           TraceLineS("(tzrepoad) Dialog action mapping using VOR not included in Dialog","");
                           DisplayEntityInstancePath( vDialog, "ActMapLOD_Entity" );
                           DisplayEntityInstance( vDialog, "ActMapView" );
                           IncrementErrorCount( vSubtask, &nError );
                           GetStringFromAttribute( szWork, sizeof( szWork ), vDialog,
                                                   "ActMapView", "Name" );
                           if ( SetCursorFirstEntityByString( vDialog, "ViewObjRef",
                                                            "Name", szWork, 0 ) >= zCURSOR_SET )
                           {
                              ExcludeEntity( vDialog, "ActMapView", zREPOS_NONE );
                              IncludeSubobjectFromSubobject( vDialog, "ActMapView",
                                                            vDialog, "ViewObjRef",
                                                            zPOS_BEFORE );
                              TraceLineS("(tzrepoad) Problem logically fixed!","");
                              nFixes++;
                           }
                        }
                     }
                  } while ( SetCursorNextEntity( vDialog, "ActMapLOD_Entity", 0 ) >= zCURSOR_SET );
#if( 0 )
                  while ( SetCursorNextEntity( vDialog, "ActMap", 0 ) >= zCURSOR_SET )
                  {
                     // Multiple actmaps found, delete them
                     TraceLineS("(tzrepoad) EXTRA ActMap Entities found in dialog","" );
                     DisplayEntityInstance( vDialog, "Dialog" );
                     DisplayEntityInstance( vDialog, "Window" );
                     DisplayEntityInstance( vDialog, "Action" );
                     DisplayEntityInstance( vDialog, "ActMap" );
                     IncrementErrorCount( vSubtask, &nError );
                     DeleteEntity( vDialog, "ActMap", zREPOS_NONE );
                     nFixes++;
                     TraceLineS("(tzrepoad) ActMap deleted!","");
                  }
#endif
               } while ( SetCursorNextEntity( vDialog, "ActMap", "Window" )
                                    >= zCURSOR_SET );
            }


// DGC - DOMGRP
// SRC metas no longer valid
#if 0
            // Validate Action Operation
            if ( SetCursorFirstEntity( vDialog, "Action", 0 ) >= zCURSOR_SET )
            do
            {
               if ( SetCursorFirstEntity( vDialog, "ActOper", 0 ) >= zCURSOR_SET )
               {
                  if ( CheckExistenceOfEntity( vDialog, "SourceFileForOper" ) != zCURSOR_SET )
                  {
                     TraceLineS("(tzrepoad) Operation found without a SourceFile","" );
                     DisplayEntityInstance( vDialog, "ActOper" );
                     IncrementErrorCount( vSubtask, &nError );
                  }
                  else
                  {
                     GetIntegerFromAttribute( &lZKey, vDialog,
                                              "SourceFileForOper", "ZKey" );
                     if ( SetCursorFirstEntityByInteger( vSrcList, "W_MetaDef",
                                                         "CPLR_ZKey", lZKey,
                                                         0 ) < zCURSOR_SET )
                     {
                        TraceLineS("(tzrepoad) Operation found from non-existent SourceFile","" );
                        DisplayEntityInstancePath( vDialog, "SourceFileForOper" );
                        IncrementErrorCount( vSubtask, &nError );
                     }
                     else
                     {
                        ActivateMetaOI( vSubtask, &vSrc, vSrcList, zREFER_SRC_META, 0 );
                        GetIntegerFromAttribute( &lZKey, vDialog,
                                                 "ActOper", "ZKey" );
                        if ( SetCursorFirstEntityByInteger( vSrc, "Operation",
                                                            "ZKey", lZKey, 0 ) < zCURSOR_SET )
                        {
                           TraceLineS("(tzrepoad) ActOper not found in corresponding SourceFile","" );
                           DisplayEntityInstancePath( vDialog, "SourceFileForOper" );
                           IncrementErrorCount( vSubtask, &nError );
                        }
                        DropView( vSrc );
                     }
                  }
                  while ( SetCursorNextEntity( vDialog, "ActOper", 0 )
                                 >= zCURSOR_SET )
                  {
                     TraceLineS("(tzrepoad) Extra ActOper found for Action","" );
                     IncrementErrorCount( vSubtask, &nError );
                     ExcludeEntity( vDialog, "ActOper", zREPOS_NONE );
                     TraceLineS("(tzrepoad) Extra ActOper Excluded!","" );
                  }
               }
            } while ( SetCursorNextEntity( vDialog, "Action", 0 ) >= zCURSOR_SET );
#endif

            fnRecursDialogOptionValidation( vSubtask, vDialog, vPe, &nError );
            while ( ResetViewFromSubobject( vDialog ) == 0 );

            // Validate DfltMenu
            if ( SetCursorFirstEntity( vDialog, "DfltMenu", 0 ) >= zCURSOR_SET &&
                 SetCursorFirstEntityByEntityCsr( vDialog, "Menu",
                     vDialog, "DfltMenu", 0 ) != zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Dialog Default Menu not found","" );
               DisplayEntityInstance( vDialog, "Dialog" );
               DisplayEntityInstance( vDialog, "Window" );
               DisplayEntityInstance( vDialog, "DfltMenu" );
               IncrementErrorCount( vSubtask, &nError );
            }

            // Validate WndCaptionMapping
            if ( SetCursorFirstEntity( vDialog, "WndCaptionMap", 0 ) >= zCURSOR_SET )
            do
            {
               if ( SetCursorFirstEntity( vDialog, "WndCaptionMapLOD_Attribute", 0 ) >= zCURSOR_SET )
               do
               {
                  if ( SetCursorFirstEntity( vDialog, "WndCaptionMapView", 0 )
                                                            != zCURSOR_SET )
                  {
                     TraceLineS("(tzrepoad) Dialog Caption mapping found without a ViewObjRef","" );
                     DisplayEntityInstance( vDialog, "Dialog" );
                     DisplayEntityInstance( vDialog, "Window" );
                     DisplayEntityInstance( vDialog, "WndCaptionMap" );
                     DisplayEntityInstance( vDialog, "WndCaptionMapLOD_Attribute" );
                     DisplayEntityInstance( vDialog, "WndCaptionMapER_Attribute" );
                     DisplayEntityInstance( vDialog, "WndCaptionMapRelatedEntity" );
                     IncrementErrorCount( vSubtask, &nError );
                  }
                  else
                  {
                     if ( SetCursorFirstEntityByEntityCsr( vDialog, "ViewObjRef",
                           vDialog, "WndCaptionMapView", 0 ) >= zCURSOR_SET )
                     {
                        GetIntegerFromAttribute( &lZKey, vDialog,
                                                 "LOD", "ZKey" );
                        if ( SetCursorFirstEntityByInteger( vLodList, "W_MetaDef",
                                                            "CPLR_ZKey", lZKey,
                                                            0 ) >= zCURSOR_SET )
                        {
                           ActivateMetaOI( vSubtask, &vLod, vLodList, zREFER_LOD_META, 0 );
                           GetIntegerFromAttribute( &lZKey, vDialog,
                                      "WndCaptionMapLOD_Attribute", "ZKey" );
                           if ( SetCursorFirstEntityByInteger( vLod,
                                                               "LOD_Attribute",
                                                               "ZKey", lZKey,
                                                               "LOD" ) < zCURSOR_SET )
                           {
                              TraceLineS("(tzrepoad) Dialog Caption mapping found for non-existent LOD Attr","");
                              DisplayEntityInstance( vDialog, "Dialog" );
                              DisplayEntityInstance( vDialog, "Window" );
                              DisplayEntityInstance( vDialog, "WndCaptionMap" );
                              DisplayEntityInstance( vDialog, "WndCaptionMapLOD_Attribute" );
                              DisplayEntityInstance( vDialog, "WndCaptionMapER_Attribute" );
                              DisplayEntityInstance( vDialog, "WndCaptionMapRelatedEntity" );
                              DisplayEntityInstance( vDialog, "LOD" );
                              IncrementErrorCount( vSubtask, &nError );
                           }
                           DropView( vLod );
                        }
                     }
                     else
                     {
                        TraceLineS("(tzrepoad) Dialog caption mapping using VOR not included in Dialog","");
                        DisplayEntityInstance( vDialog, "Dialog" );
                        DisplayEntityInstance( vDialog, "Window" );
                        DisplayEntityInstance( vDialog, "WndCaptionMap" );
                        DisplayEntityInstance( vDialog, "WndCaptionMapLOD_Attribute" );
                        DisplayEntityInstance( vDialog, "WndCaptionMapER_Attribute" );
                        DisplayEntityInstance( vDialog, "WndCaptionMapRelatedEntity" );
                        DisplayEntityInstance( vDialog, "WndCaptionMapView" );
                        IncrementErrorCount( vSubtask, &nError );
                     }
                  }
               } while ( SetCursorNextEntity( vDialog, "WndCaptionMapLOD_Attribute", 0 ) >= zCURSOR_SET );
            } while ( SetCursorNextEntity( vDialog, "WndCaptionMap", 0 ) >= zCURSOR_SET );

#if 0
            // Validate window physical override information in the PE
            if ( CheckExistenceOfEntity( vDialog, "WndColor" ) >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lZKey, vDialog, "WndColor", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vPe, "Color", "ZKey",
                                                   lZKey, 0 ) != zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) Window Color not found in PE","" );
                  DisplayEntityInstance( vDialog, "Dialog" );
                  DisplayEntityInstance( vDialog, "Window" );
                  DisplayEntityInstance( vDialog, "WndColor" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }
#endif
            if ( CheckExistenceOfEntity( vDialog, "WndStyle" ) >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lZKey, vDialog, "WndStyle", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vPe, "WindowStyle", "ZKey",
                                                   lZKey, 0 ) != zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) Window Style not found in PE","" );
                  DisplayEntityInstance( vDialog, "Dialog" );
                  DisplayEntityInstance( vDialog, "Window" );
                  DisplayEntityInstance( vDialog, "WndStyle" );
                  IncrementErrorCount( vSubtask, &nError );
                  GetStringFromAttribute( szWork, sizeof( szWork ), vDialog, "WndStyle", "Tag" );
                  if ( SetCursorFirstEntityByString( vPe, "WindowStyle", "Tag",
                                                     szWork, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vDialog, "WndStyle", zREPOS_NONE );
                     IncludeSubobjectFromSubobject( vDialog, "WndStyle",
                        vPe, "WindowStyle", zPOS_AFTER );
                     nFixes++;
                     TraceLineS("(tzrepoad) Problem logically fixed!","");
                  }

               }
             }
             else
             {
                TraceLineS("(tzrepoad) Window Style not defined for window","" );
                DisplayEntityInstance( vDialog, "Dialog" );
                DisplayEntityInstance( vDialog, "Window" );
                IncrementErrorCount( vSubtask, &nError );
             }
         }  while ( SetCursorNextEntity( vDialog, "Window", 0 )
                                                      >= zCURSOR_SET );

#if 0   // DKS  undefined vSrcList ... DG ... should this be removed???
         if ( SetCursorFirstEntity( vDialog, "SourceFile", "Dialog" )
                                                >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vDialog,
                                       "SourceFile", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vSrcList, "W_MetaDef",
                                                   "CPLR_ZKey", lZKey,
                                                   0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) SourceFile not found for Dialog","" );
                  DisplayEntityInstance( vDialog, "SourceFile" );
                  DisplayEntityInstance( vDialog, "Dialog" );
                  IncrementErrorCount( vSubtask, &nError );
                  ExcludeEntity( vDialog, "SourceFile", zREPOS_NONE );
                  TraceLineS("(tzrepoad) SourceFile Excluded!","");
                  nFixes++;
               }
            }  while ( SetCursorNextEntity( vDialog,
                        "SourceFile", "Dialog" ) >= zCURSOR_SET );
         }
#endif

         // Drop the view to the dialog
         if ( nFixes )
         {
            ResetView( vDialog );
            CommitMetaOI( vSubtask, vDialog, zSOURCE_DIALOG_META );
         }
         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vDialog, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00109", "Laucher", szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL, zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vDialog );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_DIALOG_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
            DropMetaOI( vSubtask, vDialog );
      }  while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }


   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 )
           >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating dialog " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vDialog, vMetaList, zSOURCE_DIALOG_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking Dialog " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         SetIncrementalUpdateFlags( vDialog, 0, zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vDialog, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vDialog, "Z_MetaDef", zREPOS_NONE );

         SetIncrementalUpdateFlags( vDialog, "DfltWnd", zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "CtrlMapLOD_Attribute", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "CtrlMapLOD_Entity", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "CtrlMapContext", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "CtrlMapView", zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "EventAct", zSET_INCR_INCLUDED );

      // SetIncrementalUpdateFlags( vDialog, "CtrlColor", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vDialog, "CtrlColorDef", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vDialog, "CtrlFont", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vDialog, "CtrlFontDef", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "ControlDef", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "WndAct", zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "ActMapLOD_Entity", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "ActMapView", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "ActOper", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "OptAct", zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "OptMapLOD_Attribute", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "OptMapContext", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "OptMapView", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "DfltMenu", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "WndCaptionMapLOD_Attribute", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "WndCaptionMapContext", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vDialog, "WndCaptionMapView", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

      // SetIncrementalUpdateFlags( vDialog, "WndColor", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vDialog, "WndColorDef", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

      // SetIncrementalUpdateFlags( vDialog, "WndFont", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
      // SetIncrementalUpdateFlags( vDialog, "WndFont", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "WndStyle", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "HotAct", zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "ViewObjRef", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );

         SetIncrementalUpdateFlags( vDialog, "SourceFile", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );


         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting dialog " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vDialog, zSOURCE_DIALOG_META );
         }

         // Drop the view to the dialog
         DropMetaOI( vSubtask, vDialog );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
 }

 if ( DO_HEADER )
 {
//   zSOURCE_HDR_META
   // Header3GL
   nError = 0;
   vMetaList = vHeaderList;
   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating header " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vHeader, vMetaList, zSOURCE_HDR_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking Header " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         SetIncrementalUpdateFlags( vHeader, 0,
                              zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vHeader, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vHeader, "Z_MetaDef", zREPOS_NONE );
         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting header " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                    "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vHeader, zSOURCE_HDR_META );
         }

         // Drop the view to the dialog
         DropMetaOI( vSubtask, vHeader );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 )
                                                           >= zCURSOR_SET );
   }
 }


// DGC - DOMGRP
// SRC metas no longer valid
#if 0
 if ( DO_SRC )
 {
//   zSOURCE_SRC_META    11
   // SourceFile
   vMetaList = vSrcList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating Source " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vSrc, vMetaList, zSOURCE_SRC_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Validating Source " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );

         GetIntegerFromAttribute( &lZKey, vSrc, "SourceFile", "ZKey" );
         if ( SetCursorFirstEntity( vSrc, "Dialog", "SourceFile" ) >= zCURSOR_SET )
         {
            while ( SetCursorFirstEntity( vSrc, "LOD", "SourceFile" ) >= zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) LOD found in Dialog source","" );
               IncrementErrorCount( vSubtask, &nError );
               DisplayEntityInstancePath( vSrc, "LOD" );
               GetIntegerFromAttribute( &lZKey2, vSrc, "LOD", "ZKey" );
               DropEntity( vSrc, "LOD", zREPOS_NONE );
               nFixes++;
               if ( SetCursorFirstEntityByInteger( vLodList, "W_MetaDef", "CPLR_ZKey", lZKey2, 0 ) >= zCURSOR_SET )
               {
                  ActivateMetaOI( vSubtask, &vLod, vLodList, zSOURCE_LOD_META, 0 );
                  if ( SetCursorFirstEntityByInteger( vLod,
                           "SourceFile", "ZKey", lZKey2, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vLod, "SourceFile", 0 );
                     CommitMetaOI( vSubtask, vLod, zSOURCE_LOD_META );
                  }
                  DropMetaOI( vSubtask, vLod );
               }
               TraceLineS("(tzrepoad) LOD Dropped!","");
            }
            while ( SetCursorFirstEntity( vSrc, "Domain", "SourceFile" ) >= zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Domain found in Dialog source","" );
               IncrementErrorCount( vSubtask, &nError );
               DisplayEntityInstancePath( vSrc, "Domain" );
               GetIntegerFromAttribute( &lZKey2, vSrc, "Domain", "ZKey" );
               DropEntity( vSrc, "Domain", zREPOS_NONE );
               nFixes++;
               if ( SetCursorFirstEntityByInteger( vDomainList, "W_MetaDef", "CPLR_ZKey", lZKey2, 0 ) >= zCURSOR_SET )
               {
                  ActivateMetaOI( vSubtask, &vDomain, vDomainList, zSOURCE_DOMAIN_META, 0 );
                  if ( SetCursorFirstEntityByInteger( vDomain, "SourceFile", "ZKey", lZKey2, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vDomain, "SourceFile", 0 );
                     CommitMetaOI( vSubtask, vDomain, zSOURCE_DOMAIN_META );
                  }
                  DropMetaOI( vSubtask, vDomain );
               }
               TraceLineS("(tzrepoad) Domain Dropped!","");
            }
            while ( SetCursorNextEntity( vSrc, "Dialog", "SourceFile" ) >= zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Multiple dialogs found in Dialog source","" );
               IncrementErrorCount( vSubtask, &nError );
               DisplayEntityInstancePath( vSrc, "Dialog" );
               GetIntegerFromAttribute( &lZKey2, vSrc, "Dialog", "ZKey" );
               DropEntity( vSrc, "Dialog", zREPOS_NONE );
               nFixes++;
               if ( SetCursorFirstEntityByInteger( vDialogList, "W_MetaDef", "CPLR_ZKey", lZKey2, 0 ) >= zCURSOR_SET )
               {
                  ActivateMetaOI( vSubtask, &vDialog, vDialogList, zSOURCE_DIALOG_META, 0 );
                  if ( SetCursorFirstEntityByInteger( vDialog, "SourceFile", "ZKey", lZKey2, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vDialog, "SourceFile", 0 );
                     CommitMetaOI( vSubtask, vDialog, zSOURCE_DIALOG_META );
                  }
                  DropMetaOI( vSubtask, vDialog );
               }
               TraceLineS("(tzrepoad) Dialog Dropped!","");
            }
         }
         else
         if ( SetCursorFirstEntity( vSrc, "LOD", "SourceFile" ) >= zCURSOR_SET )
         {
            while ( SetCursorFirstEntity( vSrc, "Domain", "SourceFile" ) >= zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Domain found in LOD source","" );
               IncrementErrorCount( vSubtask, &nError );
               DisplayEntityInstancePath( vSrc, "Domain" );
               GetIntegerFromAttribute( &lZKey2, vSrc, "Domain", "ZKey" );
               DropEntity( vSrc, "Domain", zREPOS_NONE );
               nFixes++;
               if ( SetCursorFirstEntityByInteger( vDomainList, "W_MetaDef", "CPLR_ZKey", lZKey2, 0 ) >= zCURSOR_SET )
               {
                  ActivateMetaOI( vSubtask, &vDomain, vDomainList, zSOURCE_DOMAIN_META, 0 );
                  if ( SetCursorFirstEntityByInteger( vDomain, "SourceFile", "ZKey", lZKey2, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vDomain, "SourceFile", 0 );
                     CommitMetaOI( vSubtask, vDomain, zSOURCE_DOMAIN_META );
                  }
                  DropMetaOI( vSubtask, vDomain );
               }
               TraceLineS("(tzrepoad) Domain Dropped!","");
            }
            while ( SetCursorNextEntity( vSrc, "LOD", "SourceFile" ) >= zCURSOR_SET )
            {
               TraceLineS("(tzrepoad) Multiple LOD's found in LOD source","" );
               IncrementErrorCount( vSubtask, &nError );
               DisplayEntityInstancePath( vSrc, "LOD" );
               GetIntegerFromAttribute( &lZKey2, vSrc, "LOD", "ZKey" );
               DropEntity( vSrc, "LOD", zREPOS_NONE );
               nFixes++;
               if ( SetCursorFirstEntityByInteger( vLodList, "W_MetaDef", "CPLR_ZKey", lZKey2, 0 ) >= zCURSOR_SET )
               {
                  ActivateMetaOI( vSubtask, &vLod, vLodList, zSOURCE_LOD_META, 0 );
                  if ( SetCursorFirstEntityByInteger( vLod, "SourceFile", "ZKey", lZKey2, 0 ) >= zCURSOR_SET )
                  {
                     ExcludeEntity( vLod, "SourceFile", 0 );
                     CommitMetaOI( vSubtask, vLod, zSOURCE_LOD_META );
                  }
                  DropMetaOI( vSubtask, vDialog );
               }
               TraceLineS("(tzrepoad) LOD Dropped!","");
            }
         }
         if ( SetCursorFirstEntity( vSrc, "HeaderFile", "SourceFile" ) >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey2, vSrc, "HeaderFile", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vHeaderList, "W_MetaDef", "CPLR_ZKey", lZKey2, 0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) Header file not found in LPLR","" );
                  IncrementErrorCount( vSubtask, &nError );
                  DisplayEntityInstancePath( vSrc, "HeaderFile" );
                  ExcludeEntity( vSrc, "HeaderFile", zREPOS_NONE );
                  nFixes++;
                  TraceLineS("(tzrepoad) Header file excluded!!!","" );
               }
            } while ( SetCursorNextEntity( vSrc, "HeaderFile", "SourceFile" ) >= zCURSOR_SET );
         }

         // Drop the view to the Source
         if ( nFixes )
         {
            ResetView( vSrc );
            CommitMetaOI( vSubtask, vSrc, zSOURCE_SRC_META );
         }

         // Drop the view to the dialog
         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vSrc, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00110", "Laucher", szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL, zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vSrc );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_SRC_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
            DropMetaOI( vSubtask, vSrc );
      }  while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }

   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating SourceFile " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vSrc, vMetaList, zSOURCE_SRC_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking SourceFile " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                 "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         SetIncrementalUpdateFlags( vSrc, 0,
                              zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vSrc, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vSrc, "Z_MetaDef", zREPOS_NONE );

         SetIncrementalUpdateFlags( vSrc, "GlobalOperation",
                              zSET_INCR_NOT_INCLUDED );
         SetIncrementalUpdateFlags( vSrc, "Domain", zSET_INCR_NOT_INCLUDED );
         SetIncrementalUpdateFlags( vSrc, "LOD", zSET_INCR_NOT_INCLUDED );
         SetIncrementalUpdateFlags( vSrc, "Dialog", zSET_INCR_NOT_INCLUDED );
         SetIncrementalUpdateFlags( vSrc, "HeaderFile",
                               zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting SourceFile " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList,
                                    "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vSrc, zSOURCE_SRC_META );
         }

         // Drop the view to the dialog
         DropMetaOI( vSubtask, vSrc );
      }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }
 }
#endif

 if ( DO_OPER )
 {
//   zSOURCE_GO_META    4
   vMetaList = vOperList;
   if ( VALIDATE )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         nError = 0;
         nFixes = 0;
         bFirst = 0;
         strcpy_s( szMsg, sizeof( szMsg ), "Activating Oper " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vOper, vMetaList, zSOURCE_GO_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Validating Oper " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );


#if 0   // DKS  undefined vSrcList ... DG ... should this be removed???
         if ( SetCursorFirstEntity( vOper, "SourceFile", "Operation" ) >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vOper, "SourceFile", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vSrcList, "W_MetaDef",
                                                   "CPLR_ZKey", lZKey, 0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) SourceFile not found for operation","" );
                  DisplayEntityInstancePath( vOper, "SourceFile" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }  while ( SetCursorNextEntity( vOper, "SourceFile",
                                            "Operation" ) >= zCURSOR_SET );
         }
         else
         {
            TraceLineS("(tzrepoad) Operation has NO SourceFile","" );
            IncrementErrorCount( vSubtask, &nError );
            DisplayEntityInstancePath( vOper, "Operation" );
         }

         if ( SetCursorFirstEntity( vOper, "HeaderFile", "Operation" ) >= zCURSOR_SET )
         {
            do
            {
               GetIntegerFromAttribute( &lZKey, vOper, "HeaderFile", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vSrcList, "W_MetaDef",
                                                   "CPLR_ZKey", lZKey, 0 ) < zCURSOR_SET )
               {
                  TraceLineS("(tzrepoad) HeaderFile not found for operation","" );
                  DisplayEntityInstancePath( vOper, "HeaderFile" );
                  IncrementErrorCount( vSubtask, &nError );
               }
            }  while ( SetCursorNextEntity( vOper, "HeaderFile", "Operation" ) >= zCURSOR_SET );
         }
         else
         {
            TraceLineS("(tzrepoad) Operation has NO HeaderFile","" );
            IncrementErrorCount( vSubtask, &nError );
            DisplayEntityInstancePath( vOper, "Operation" );
         }
#endif

         // Drop the view to the dialog
         if ( nFixes )
         {
            ResetView( vOper );
            CommitMetaOI( vSubtask, vOper, zSOURCE_GO_META );
         }

         // Drop the view to the dialog
         nError -= nFixes;
         if ( nError && PROMPT )
         {
            SetNameForView( vOper, "ERRORMETA", vSubtask, zLEVEL_TASK );
            zltoa( nError, szMsg, sizeof( szMsg ) );
            strcat_s( szMsg, sizeof( szMsg ), " Errors were encountered, DeleteMetaOI?" );
            nRC = MessagePrompt( vSubtask, "RE00111", "Laucher", szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL, zRESPONSE_NO,  0 );
            DropMetaOI( vSubtask, vOper );
            if ( nRC == zRESPONSE_YES )
            {
               DeleteMetaOI( vSubtask, vMetaList, zSOURCE_GO_META );
               if ( SetCursorPrevEntity( vMetaList, "W_MetaDef", 0 ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
                     bFirst = 1;
               }
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SfCreateSubtask( &vSubtask, vSubtask, 0 );
               SfDropSubtask( vSubtask, 1 );
               return( 0 );
            }
         }
         else
            DropMetaOI( vSubtask, vOper );
      }  while ( bFirst || SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
   }

   if ( MARK )
   {
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET )
      do
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Activating Oper " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         ActivateMetaOI( vSubtask, &vOper, vMetaList, zSOURCE_GO_META, 0 );
         strcpy_s( szMsg, sizeof( szMsg ), "Marking Oper " );
         GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
         strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
         MB_SetMessage( vSubtask, 1, szMsg );
         SetIncrementalUpdateFlags( vOper, 0, zSET_INCR_OWNERSHIP | zSET_INCR_CREATED );
         if ( SetCursorFirstEntity( vOper, "Z_MetaDef", 0 ) == zCURSOR_SET )
            ExcludeEntity( vOper, "Z_MetaDef", zREPOS_NONE );

         SetIncrementalUpdateFlags( vOper, "SourceFile", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         SetIncrementalUpdateFlags( vOper, "HeaderFile", zSET_INCR_PERSISTENT | zSET_INCR_INCLUDED );
         if ( COMMIT )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Commiting Oper " );
            GetStringFromAttribute( szMsg + 80, sizeof( szMsg ) - 80, vMetaList, "W_MetaDef", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), &szMsg[ 80 ] );
            MB_SetMessage( vSubtask, 1, szMsg );
            CommitMetaOI( vSubtask, vOper, zSOURCE_GO_META );
         }

         // Drop the view to the dialog
         DropMetaOI( vSubtask, vOper );
       }  while ( SetCursorNextEntity( vMetaList, "W_MetaDef", 0 ) >= zCURSOR_SET );
     }
   }

   return( 0 );
}
