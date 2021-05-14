#define zGLOBAL_DATA
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "ZeidonOp.H"

/* local prototypes */
zOPER_EXPORT zSHORT OPERATION
UpgradeDialog1( zVIEW    vSubtask,
                zVIEW    OrigW );
zOPER_EXPORT zSHORT OPERATION
UpgradeControl( zVIEW    vSubtask,
                zVIEW    OrigW,
                zVIEW    OrigWC,
                zVIEW    vType,
                zVIEW    vPE );
zOPER_EXPORT zSHORT OPERATION
UpgradeCtrlMap( zVIEW    vSubtask,
                zVIEW    OrigW,
                zVIEW    OrigWC );
zOPER_EXPORT zSHORT OPERATION
IncludeAttributeMapping( zVIEW    vSubtask,
                         zVIEW    OrigW,
                         zVIEW    OrigWC );
zOPER_EXPORT zSHORT OPERATION
UpgradeAction( zVIEW    vSubtask,
               zVIEW    OrigW );

zOPER_EXPORT zSHORT OPERATION
UpgradeDialog1( zVIEW    vSubtask,
                zVIEW    OrigW )
{
#if 0
   zVIEW    OrigWC;
   zVIEW    vPE;
   zVIEW    vType;
   zVIEW    LOD_List;
   zSHORT   nZRetCode;
   zLONG    lTempInteger_0;
   zSHORT   RESULT;

   nZRetCode = RetrieveViewForMetaList( vSubtask, &LOD_List, 2013 );
   nZRetCode = ActivateMetaOI( vSubtask, &vPE, LOD_List, 2013, zSINGLE );
   nZRetCode = ActivateOI_FromFile( &vType, "TZPACTLO", vSubtask, "c:\\10d\\W\\BIN\\SYS\\TZPACTLI.POR", zSINGLE );
   lTempInteger_0 = CheckExistenceOfEntity( OrigW, "SourceFile" );
   if ( lTempInteger_0 < 0 )
   {
      nZRetCode = CreateMetaEntity( vSubtask, OrigW, "SourceFile", zPOS_AFTER );
      SetAttributeFromAttribute( OrigW, "SourceFile", "Name", OrigW, "Dialog", "Tag" );
   }
   SetNameForView( vType, "vType", vSubtask, zLEVEL_TASK );
   SetNameForView( vPE, "vPE", vSubtask, zLEVEL_TASK );
   RESULT = SetCursorFirstEntity( OrigW, "Window", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = CreateViewFromViewForTask( &OrigWC, OrigW, 0 );
      nZRetCode = SetNameForView( OrigWC, "OrigWC", vSubtask, zLEVEL_TASK );
      RESULT = SetCursorFirstEntity( OrigW, "Action", "" );
//    while ( RESULT > zLEVEL_TASK )
      while ( RESULT >= 0 )
      {
         nZRetCode = UpgradeAction( vSubtask, OrigW );
         RESULT = SetCursorNextEntity( OrigW, "Action", "" );
      }
      RESULT = SetCursorFirstEntity( OrigWC, "Control", "" );
      while ( RESULT >= 0 )
      {
         nZRetCode = UpgradeControl( vSubtask, OrigW, OrigWC, vType, vPE );
         RESULT = SetCursorNextEntity( OrigWC, "Control", "" );
      }
      nZRetCode = DropView( OrigWC );
      RESULT = SetCursorNextEntity( OrigW, "Window", "" );
   }
#endif
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UpgradeControl( zVIEW    vSubtask,
                zVIEW    OrigW,
                zVIEW    OrigWC,
                zVIEW    vType,
                zVIEW    vPE )
{
#if 0
   zLONG    lType;
   zCHAR    szTag[ 35 ];
   zSHORT   RESULT;
   zSHORT   nZRetCode;

   RESULT = SetCursorFirstEntity( OrigWC, "CtrlCtrl", "" );
   while ( RESULT >= 0 )
   {
      nZRetCode = SetViewToSubobject( OrigWC, "CtrlCtrl" );
      nZRetCode = UpgradeControl( vSubtask, OrigW, OrigWC, vType, vPE );
      nZRetCode = ResetViewFromSubobject( OrigWC );
      RESULT = SetCursorNextEntity( OrigWC, "CtrlCtrl", "" );
   }
   RESULT = SetCursorFirstEntity( OrigWC, "CtrlMap", "" );
   while ( RESULT >= 0 )
   {
      nZRetCode = UpgradeCtrlMap( vSubtask, OrigW, OrigWC );
      RESULT = SetCursorNextEntity( OrigWC, "CtrlMap", "" );
   }
   // Upgrade Control to relate to ControlDef
   if ( CheckExistenceOfEntity ( OrigWC, "ControlDef" ) < 0 )
   {
      GetIntegerFromAttribute( &lType, OrigWC, "Control", "Type" );
      SetCursorFirstEntityByInteger( vType, "Control", "Type", lType, "" );
      GetStringFromAttribute( szTag, zsizeof( szTag ), vType, "Control", "Tag" );
      SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", szTag, "" );
      IncludeSubobjectFromSubobject( OrigWC, "ControlDef", vPE, "ControlDef", zPOS_AFTER );
      SetAttributeFromString ( OrigWC, "Control", "Type", "");
   }
#endif
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UpgradeCtrlMap( zVIEW    vSubtask,
                zVIEW    OrigW,
                zVIEW    OrigWC )
{
#if 0
   zLONG    lTempInteger_0;
   zSHORT   RESULT;
   zCHAR    szTempString_0[ 33 ];
   zSHORT   nZRetCode;
   zLONG    lTempInteger_1;
   zLONG    lTempInteger_2;

   lTempInteger_0 = CheckExistenceOfEntity( OrigWC, "CtrlMapView" );
   if ( lTempInteger_0 >= 0 )
   {
      nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC, "CtrlMapView", "Name" );
      RESULT = SetCursorFirstEntityByString( OrigW, "ViewObjRef", "Name", szTempString_0, "" );
   }
   else
   {
      if ( CompareAttributeToString ( OrigWC, "CtrlMap", "X_ViewName", "" ) != 0 )
      {
         nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC, "CtrlMap", "X_ViewName" );
         RESULT = SetCursorFirstEntityByString( OrigW, "ViewObjRef", "Name", szTempString_0, "" );
      }
   }
   lTempInteger_1 = CheckExistenceOfEntity( OrigWC, "CtrlMapView" );
   if ( lTempInteger_1 < 0 && CompareAttributeToString ( OrigWC, "CtrlMap", "X_ViewName", "" ) != 0 )
   {
      nZRetCode = IncludeSubobjectFromSubobject( OrigWC, "CtrlMapView", OrigW, "ViewObjRef", zPOS_AFTER );
   }
   lTempInteger_2 = CheckExistenceOfEntity( OrigWC, "CtrlMapLOD_Attribute" );
   if ( lTempInteger_2 < 0 && CompareAttributeToString ( OrigWC, "CtrlMap", "X_AttributeName", "" ) != 0 )
   {
      nZRetCode = IncludeAttributeMapping( vSubtask, OrigW, OrigWC );
   }
   nZRetCode = SetAttributeFromString( OrigWC, "CtrlMap", "X_ViewName", "" );
   nZRetCode = SetAttributeFromString( OrigWC, "CtrlMap", "X_EntityName", "" );
   nZRetCode = SetAttributeFromString( OrigWC, "CtrlMap", "X_AttributeName", "" );
   nZRetCode = SetAttributeFromString( OrigWC, "CtrlMap", "X_Context", "" );
#endif
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
IncludeAttributeMapping( zVIEW    vSubtask,
                         zVIEW    OrigW,
                         zVIEW    OrigWC )
{
#if 0
   zVIEW    vLOD;
   zVIEW    LOD_List;
   zVIEW    vDomain;
   zVIEW    DomainList;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zCHAR    szTempString_0[ 33 ];

   RetrieveViewForMetaList( vSubtask, &LOD_List, zREFER_LOD_META );
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigW, "LOD", "Name" );
   SetCursorFirstEntityByString( LOD_List, "W_MetaDef", "Name", szTempString_0, "" );
   ActivateMetaOI( vSubtask, &vLOD, LOD_List, zREFER_LOD_META, zSINGLE );
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC, "CtrlMap", "X_EntityName" );
   RESULT = SetCursorFirstEntityByString( vLOD, "LOD_Entity", "Name", szTempString_0, "" );
   if ( RESULT >= 0 )
   {
      nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC, "CtrlMap", "X_AttributeName" );
      RESULT = SetCursorFirstEntityByString( vLOD, "ER_Attribute", "Name", szTempString_0, "LOD_Entity" );
      if ( RESULT >= 0 )
      {
         nZRetCode = IncludeSubobjectFromSubobject( OrigWC, "CtrlMapLOD_Attribute", vLOD, "LOD_Attribute", zPOS_AFTER );
         if ( CompareAttributeToString ( OrigWC, "CtrlMap", "X_Context", "" ) != 0 )
         {
            nZRetCode = RetrieveViewForMetaList( vSubtask, &DomainList, zREFER_DOMAIN_META );
            nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vLOD, "Domain", "Name" );
            RESULT = SetCursorFirstEntityByString( DomainList, "W_MetaDef", "Name", szTempString_0, "" );
            nZRetCode = ActivateMetaOI( vSubtask, &vDomain, DomainList, zREFER_DOMAIN_META, zSINGLE );
            nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC, "CtrlMap", "X_Context" );
            RESULT = SetCursorFirstEntityByString( vDomain, "Context", "Name", szTempString_0, "" );
            if ( RESULT >= 0 )
            {
               nZRetCode = IncludeSubobjectFromSubobject( OrigWC, "CtrlMapContext", vDomain, "Context", zPOS_AFTER );
            }
            nZRetCode = DropMetaOI( vSubtask, vDomain );
            nZRetCode = DropView( DomainList );
         }
      }
   }

   nZRetCode = DropMetaOI( vSubtask, vLOD );
   nZRetCode = DropView( LOD_List );
#endif
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UpgradeAction( zVIEW    vSubtask,
               zVIEW    OrigW )
{
#if 0
   zSHORT   RESULT;
   zCHAR    szTempString_0[ 33 ];
   zSHORT   nZRetCode;

   if ( CompareAttributeToString( OrigW, "Action", "OperName", "" ) != 0 )
   {
      nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigW, "Action", "OperName" );
      RESULT = SetCursorFirstEntityByString( OrigW, "Operation", "Name", szTempString_0, "" );
      if ( RESULT < 0 )
      {
         nZRetCode = CreateMetaEntity( vSubtask, OrigW, "Operation", zPOS_AFTER );
         SetAttributeFromAttribute( OrigW, "Operation", "Name", OrigW, "Action", "OperName" );
         nZRetCode = IncludeSubobjectFromSubobject( OrigW, "SourceFileOfOperation", OrigW, "SourceFile", zPOS_AFTER );
         SetAttributeFromString( OrigW, "Action", "OperName", "");
      }
      nZRetCode = IncludeSubobjectFromSubobject( OrigW, "ActOper", OrigW, "Operation", zPOS_AFTER );
      SetAttributeFromString( OrigW, "Action", "OperName", "");
   }
#endif
   return( 0 );
}
