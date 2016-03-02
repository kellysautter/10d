/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzpnoldc.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// LAST MESSAGE ID: PN00301
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"



zOPER_EXPORT zSHORT OPERATION
PositionOnVOR( zVIEW  OrigW,
               zVIEW  NewW,
               zPCHAR pchViewName,
               zVIEW  vSubtask );
zOPER_EXPORT zSHORT OPERATION
CloneMenuAndOptions( zVIEW  OrigW,
                     zVIEW  NewW,
                     zVIEW  vSubtask );
zOPER_EXPORT zSHORT OPERATION
CloneAction( zVIEW    vSubtask,
             zVIEW    OrigW,
             zVIEW    NewW );
zOPER_EXPORT zSHORT OPERATION
CloneOption( zVIEW    OrigW,
             zVIEW    OrigWO,
             zVIEW    NewW,
             zVIEW    NewWO,
             zVIEW    vSubtask );

zOPER_EXPORT zSHORT OPERATION
CloneOptMap( zVIEW    OrigW,
             zVIEW    OrigWO,
             zVIEW    NewW,
             zVIEW    NewWO,
             zPCHAR   pchViewName,
             zLONG    lMaxLth,
             zVIEW    vSubtask );

zOPER_EXPORT zSHORT OPERATION
IncludeMapping ( zVIEW    vSubtask,
                 zVIEW    OrigW,
                 zVIEW    OrigWC,
                 zVIEW    NewW,
                 zVIEW    NewWC )
{
   zVIEW    vLOD;
   zVIEW    LOD_List;
   zVIEW    vDomain;
   zVIEW    DomainList;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zCHAR    szTempString_0[ 33 ];

   if ( CompareAttributeToString( OrigWC, "CtrlMap",
                                  "X_EntityName", "" ) != 0 )
   {
      nZRetCode = RetrieveViewForMetaList( vSubtask, &LOD_List, zREFER_LOD_META );
      nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), NewW,
                                          "LOD", "Name" );
      RESULT = SetCursorFirstEntityByString( LOD_List, "W_MetaDef",
                                             "Name", szTempString_0, "" );
      nZRetCode = ActivateMetaOI( vSubtask, &vLOD, LOD_List, zREFER_LOD_META, zSINGLE );
      nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC,
                                          "CtrlMap", "X_EntityName" );
      RESULT = SetCursorFirstEntityByString( vLOD, "LOD_Entity",
                                             "Name", szTempString_0, "" );
      if ( RESULT >= 0 )
      {
         if ( CompareAttributeToString( OrigWC, "CtrlMap",
                                        "X_AttributeName", "" ) == 0 )
         {
            nZRetCode = IncludeSubobjectFromSubobject( NewWC, "CtrlMapLOD_Entity",
                                                       vLOD, "LOD_Entity", zPOS_AFTER );
         }
         else
         {
            nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC, "CtrlMap", "X_AttributeName" );
            RESULT = SetCursorFirstEntityByString( vLOD, "ER_Attribute", "Name", szTempString_0, "LOD_Entity" );
            if ( RESULT >= 0 )
            {
               nZRetCode = IncludeSubobjectFromSubobject( NewWC, "CtrlMapLOD_Attribute", vLOD, "LOD_Attribute", zPOS_AFTER );
               if ( CompareAttributeToString( OrigWC, "CtrlMap", "X_Context", "" ) != 0 )
               {
                  nZRetCode = RetrieveViewForMetaList( vSubtask, &DomainList, zREFER_DOMAIN_META );
                  nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vLOD, "Domain", "Name" );
                  RESULT = SetCursorFirstEntityByString( DomainList, "W_MetaDef", "Name", szTempString_0, "" );
                  nZRetCode = ActivateMetaOI( vSubtask, &vDomain, DomainList, zREFER_DOMAIN_META, zSINGLE );
                  nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC, "CtrlMap", "X_Context" );
                  RESULT = SetCursorFirstEntityByString( vDomain, "Context", "Name", szTempString_0, "" );
                  if ( RESULT >= 0 )
                  {
                     nZRetCode = IncludeSubobjectFromSubobject( NewWC, "CtrlMapContext", vDomain, "Context", zPOS_AFTER );
                  }
                  nZRetCode = DropMetaOI( vSubtask, vDomain );
                  nZRetCode = DropView( DomainList );
               }
            }
         }
      }
      nZRetCode = DropMetaOI( vSubtask, vLOD );
      nZRetCode = DropView( LOD_List );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CloneCtrlMapO( zVIEW    OrigW,
               zVIEW    OrigWC,
               zVIEW    NewW,
               zVIEW    NewWC,
               zPCHAR   pchViewName,
               zLONG    lMaxLth,
               zVIEW    vSubtask )
{
   CreateMetaEntity( vSubtask, NewWC, "CtrlMap", zPOS_AFTER );
   SetMatchingAttributesByName( NewWC, "CtrlMap", OrigWC, "CtrlMap", zSET_NULL );
   if ( CompareAttributeToString( OrigWC, "CtrlMap", "X_ViewName", "" ) != 0 )
   {
      GetStringFromAttribute( pchViewName, lMaxLth, OrigWC, "CtrlMap", "X_ViewName" );
   }
   if ( ZeidonStringCompare( pchViewName, 1, 0, "", 1, 0, 33 ) )
   {
      if ( CompareAttributeToString( OrigWC, "CtrlMap", "X_EntityName", "" ) != 0 )
      {
         PositionOnVOR( OrigW, NewW, pchViewName, vSubtask );
         IncludeSubobjectFromSubobject( NewWC, "CtrlMapView", NewW, "ViewObjRef", zPOS_AFTER );
         IncludeMapping( vSubtask, OrigW, OrigWC, NewW, NewWC );
      }
      else
      {
         SetAttributeFromString( NewWC, "CtrlMap", "X_ViewName", "" );
         SetAttributeFromString( NewWC, "CtrlMap", "X_EntityName", "" );
         SetAttributeFromString( NewWC, "CtrlMap", "X_AttributeName", "" );
         SetAttributeFromString( NewWC, "CtrlMap", "X_Context", "" );
      }
   }
   else
   {
      SetAttributeFromString( NewWC, "CtrlMap", "X_ViewName", "" );
      SetAttributeFromString( NewWC, "CtrlMap", "X_EntityName", "" );
      SetAttributeFromString( NewWC, "CtrlMap", "X_AttributeName", "" );
      SetAttributeFromString( NewWC, "CtrlMap", "X_Context", "" );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CloneControlO( zVIEW    OrigW,
               zVIEW    OrigWC,
               zVIEW    NewW,
               zVIEW    NewWC,
               zVIEW    vSubtask )
{
   zCHAR    szViewName[ 33 ];
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zCHAR    szTempString_0[ 33 ];

   nZRetCode = CreateMetaEntity( vSubtask, NewWC, "Control", zPOS_AFTER );
   nZRetCode = SetMatchingAttributesByName( NewWC, "Control", OrigWC, "Control", zSET_NULL );
   RESULT = SetCursorFirstEntity( OrigWC, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = SetViewToSubobject( OrigWC, "CtrlCtrl" );
      nZRetCode = SetViewToSubobject( NewWC, "CtrlCtrl" );
      nZRetCode = CloneControlO( OrigW, OrigWC, NewW, NewWC, vSubtask );
      nZRetCode = ResetViewFromSubobject( OrigWC );
      nZRetCode = ResetViewFromSubobject( NewWC );
      RESULT = SetCursorNextEntity( OrigWC, "CtrlCtrl", "" );
   }
   nZRetCode = GetStringFromAttribute( szViewName, zsizeof( szViewName ), OrigWC, "CtrlMap", "X_ViewName" );
   RESULT = SetCursorFirstEntity( OrigWC, "CtrlMap", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = CloneCtrlMapO( OrigW, OrigWC, NewW, NewWC, szViewName, zsizeof( szViewName ), vSubtask );
      RESULT = SetCursorNextEntity( OrigWC, "CtrlMap", "" );
   }
   RESULT = SetCursorFirstEntity( OrigWC, "Event", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      if ( CompareAttributeToString( OrigWC, "EventAct", "Tag", "" ) != 0 )
      {
         nZRetCode = CreateMetaEntity( vSubtask, NewWC, "Event", zPOS_AFTER );
         nZRetCode = SetMatchingAttributesByName( NewWC, "Event", OrigWC, "Event", zSET_NULL );
         nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWC, "EventAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( NewW, "Action", "Tag", szTempString_0, "" );
         if ( RESULT >= 0 )
         {
            nZRetCode = IncludeSubobjectFromSubobject( NewWC, "EventAct", NewW, "Action", zPOS_AFTER );
         }
      }
      RESULT = SetCursorNextEntity( OrigWC, "Event", "" );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
MergeWindowComponentsO( zVIEW    vSubtask,
                        zVIEW    OrigW,
                        zVIEW    NewW )
{
   zVIEW    NewWC;
   zVIEW    OrigWC;
   zVIEW    TempW;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zCHAR    szTempString_0[ 33 ];

   nZRetCode = CreateViewFromViewForTask( &NewWC, NewW, 0 );
   nZRetCode = CreateViewFromViewForTask( &OrigWC, OrigW, 0 );
   nZRetCode = SetNameForView( NewWC, "NewWC", vSubtask, zLEVEL_TASK );
   nZRetCode = SetNameForView( OrigWC, "OrigWC", vSubtask, zLEVEL_TASK );
   RESULT = SetCursorFirstEntity( OrigW, "Action", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = CloneAction( OrigW, NewW, vSubtask );
      RESULT = SetCursorNextEntity( OrigW, "Action", "" );
   }
   RESULT = SetCursorFirstEntity( OrigWC, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = CloneControlO( OrigW, OrigWC, NewW, NewWC, vSubtask );
      RESULT = SetCursorNextEntity( OrigWC, "Control", "" );
   }
   nZRetCode = CreateViewFromViewForTask( &TempW, NewW, 0 );
   RESULT = SetCursorFirstEntity( OrigW, "WndEvent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, OrigW, "WndEvent", "Type" );
      RESULT = SetCursorFirstEntityByInteger( TempW, "WndEvent", "Type", lTempInteger_0, "" );
      if ( RESULT != zCURSOR_SET && CompareAttributeToString( OrigW, "WndAct", "Tag", "" ) != 0 )
      {
         nZRetCode = CreateMetaEntity( vSubtask, NewW, "WndEvent", zPOS_AFTER );
         nZRetCode = SetMatchingAttributesByName( NewW, "WndEvent", OrigW, "WndEvent", zSET_NULL );
         nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigW, "WndAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( NewW, "Action", "Tag", szTempString_0, "" );
         if ( RESULT >= 0 )
         {
            nZRetCode = IncludeSubobjectFromSubobject( NewW, "WndAct", NewW, "Action", zPOS_AFTER );
         }
      }
      RESULT = SetCursorNextEntity( OrigW, "WndEvent", "" );
   }
   nZRetCode = CloneMenuAndOptions( OrigW, NewW, vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OldCloneWindow( zVIEW  OrigW,
                zVIEW  NewW,
                zVIEW  vSubtask )
{
   zSHORT   nZRetCode;

   nZRetCode = CreateMetaEntity( vSubtask, NewW, "Window", zPOS_AFTER );
   nZRetCode = SetMatchingAttributesByName( NewW, "Window", OrigW, "Window", zSET_NULL );
   nZRetCode = MergeWindowComponentsO( vSubtask, OrigW, NewW );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PositionOnVOR( zVIEW  OrigW,
               zVIEW  NewW,
               zPCHAR pchViewName,
               zVIEW  vSubtask )
{
   zVIEW    vLOD;
   zVIEW    LOD_List;
   zVIEW    VOR;
   zVIEW    VOR_List;
   zCHAR    szViewNamePrompt[ 65 ];
   zCHAR    szObjectName[ 33 ];
   zSHORT   RESULT;
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 33 ];

   RESULT = SetCursorFirstEntityByString( NewW, "ViewObjRef", "Name", pchViewName, "" );
   if ( RESULT != 0 )
   {
      nZRetCode = RetrieveViewForMetaList( vSubtask, &VOR_List, zREFER_VOR_META );
      RESULT = SetCursorFirstEntityByString( VOR_List, "W_MetaDef", "Name", pchViewName, "" );
      if ( RESULT >= 0 )
      {
         nZRetCode = ActivateMetaOI( vSubtask, &VOR, VOR_List, zREFER_VOR_META, zSINGLE );
      }
      else
      {
         nZRetCode = ActivateEmptyMetaOI( vSubtask, &VOR, zSOURCE_VOR_META, zSINGLE );
         nZRetCode = CreateMetaEntity( vSubtask, VOR, "ViewObjRef", zPOS_AFTER );
         nZRetCode = SetAttributeFromString( VOR, "ViewObjRef", "Name", pchViewName );
         nZRetCode = RetrieveViewForMetaList( vSubtask, &LOD_List, zREFER_LOD_META );
         RESULT = SetCursorFirstEntityByString( OrigW, "ViewObjRef", "Name", pchViewName, "" );
         if ( RESULT >= 0 )
         {
            nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigW, "LOD", "Name" );
            RESULT = SetCursorFirstEntityByString( LOD_List, "W_MetaDef", "Name", szTempString_0, "" );
         }
         else
         {
            RESULT = -1;
            ZeidonStringCopy( szObjectName, 1, 0, "", 1, 0, 33 );
            while ( RESULT != zCURSOR_SET )
            {
               ZeidonStringCopy( szViewNamePrompt, 1, 0, "Enter LOD Name For View: ", 1, 0, 65 );
               ZeidonStringConcat( szViewNamePrompt, 1, 0, pchViewName, 1, 0, 65 );
               MessageSend( vSubtask, "PN00301", "Dialog",
                            szViewNamePrompt,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               RESULT = SetCursorFirstEntityByString( LOD_List, "W_MetaDef",
                                                      "Name", szObjectName, "" );
            }
         }

         ActivateMetaOI( vSubtask, &vLOD, LOD_List, zREFER_LOD_META, zSINGLE );
         IncludeSubobjectFromSubobject( VOR, "LOD", vLOD, "LOD", zPOS_AFTER );
         DropView( LOD_List );
         DropMetaOI( vSubtask, vLOD );
         CommitMetaOI( vSubtask, VOR, zSOURCE_VOR_META );
      }

      IncludeSubobjectFromSubobject( NewW, "ViewObjRef", VOR, "ViewObjRef", zPOS_AFTER );
      DropMetaOI( vSubtask, VOR );
      DropView( VOR_List );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CloneAction( zVIEW    OrigW,
             zVIEW    NewW,
             zVIEW    vSubtask )
{
   zSHORT   nZRetCode;

   if ( CompareAttributeToString( OrigW, "Action", "Tag", "" ) != 0 )
   {
      nZRetCode = CreateMetaEntity( vSubtask, NewW, "Action", zPOS_AFTER );
      nZRetCode = SetMatchingAttributesByName( NewW, "Action", OrigW,
                                               "Action", zSET_NULL );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CloneMenuAndOptions( zVIEW  OrigW,
                     zVIEW  NewW,
                     zVIEW  vSubtask )
{
   zVIEW    NewWO;
   zVIEW    OrigWO;
   zSHORT   RESULT;
   zLONG    lTemp;
   zCHAR    szTemp[ 33 ];

   RESULT = SetCursorFirstEntity( OrigW, "Menu", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      CreateMetaEntity( vSubtask, NewW, "Menu", zPOS_AFTER );
      SetMatchingAttributesByName( NewW, "Menu", OrigW, "Menu", zSET_NULL );
      CreateViewFromViewForTask( &NewWO, NewW, 0 );
      CreateViewFromViewForTask( &OrigWO, OrigW, 0 );
      SetNameForView( NewWO, "NewWO", vSubtask, zLEVEL_TASK );
      SetNameForView( OrigWO, "OrigWO", vSubtask, zLEVEL_TASK );
      RESULT = SetCursorFirstEntity( OrigWO, "Option", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         CloneOption( OrigW, OrigWO, NewW, NewWO, vSubtask );
         RESULT = SetCursorNextEntity( OrigWO, "Option", "" );
      }

      RESULT = SetCursorNextEntity( OrigW, "Menu", "" );
   }

   lTemp = CheckExistenceOfEntity( OrigW, "DfltMenu" );
   if ( lTemp == zCURSOR_SET )
   {
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), OrigW, "DfltMenu", "Tag" );
      RESULT = SetCursorFirstEntityByString( NewW, "Menu", "Tag",
                                             szTemp, "" );
      IncludeSubobjectFromSubobject( NewW, "DfltMenu", NewW,
                                     "Menu", zPOS_AFTER );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CloneOption( zVIEW    OrigW,
             zVIEW    OrigWO,
             zVIEW    NewW,
             zVIEW    NewWO,
             zVIEW    vSubtask )
{
   zCHAR    szViewName[ 33 ];
   zSHORT   RESULT;
   zLONG    lTemp;
   zCHAR    szTemp[ 33 ];

   CreateMetaEntity( vSubtask, NewWO, "Option", zPOS_AFTER );
   SetMatchingAttributesByName( NewWO, "Option", OrigWO, "Option", zSET_NULL );
   RESULT = SetCursorFirstEntity( OrigWO, "OptOpt", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      SetViewToSubobject( OrigWO, "OptOpt" );
      SetViewToSubobject( NewWO, "OptOpt" );
      CloneOption( OrigW, OrigWO, NewW, NewWO, vSubtask );
      ResetViewFromSubobject( OrigWO );
      ResetViewFromSubobject( NewWO );
      RESULT = SetCursorNextEntity( OrigWO, "OptOpt", "" );
   }

   GetStringFromAttribute( szViewName, zsizeof( szViewName ), OrigWO, "OptMap", "X_ViewName" );
   RESULT = SetCursorFirstEntity( OrigWO, "OptMap", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      CloneOptMap( OrigW, OrigWO, NewW, NewWO, szViewName, zsizeof( szViewName ), vSubtask );
      RESULT = SetCursorNextEntity( OrigWO, "OptMap", "" );
   }

   lTemp = CheckExistenceOfEntity( OrigWO, "OptAct" );
   if ( lTemp == zCURSOR_SET )
   {
      if ( CompareAttributeToString( OrigWO, "OptAct", "Tag", "" ) != 0 )
      {
         GetStringFromAttribute( szTemp, zsizeof( szTemp ), OrigWO, "OptAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( NewW, "Action", "Tag", szTemp, "" );
         if ( RESULT >= 0 )
         {
            IncludeSubobjectFromSubobject( NewWO, "OptAct", NewW, "Action", zPOS_AFTER );
         }
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CloneOptMap( zVIEW    OrigW,
             zVIEW    OrigWO,
             zVIEW    NewW,
             zVIEW    NewWO,
             zPCHAR   pchViewName,
             zLONG    lMaxLth,
             zVIEW    vSubtask )
{
   zVIEW    vLOD;
   zVIEW    LOD_List;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zCHAR    szTempString[ 33 ];

   nZRetCode = CreateMetaEntity( vSubtask, NewWO, "OptMap", zPOS_AFTER );
   nZRetCode = SetMatchingAttributesByName( NewWO, "OptMap", OrigWO, "OptMap", zSET_NULL );
   if ( CompareAttributeToString( OrigWO, "OptMap", "X_ViewName", "" ) != 0 )
   {
      nZRetCode = GetStringFromAttribute( pchViewName, lMaxLth, OrigWO, "OptMap", "X_ViewName" );
   }
   if ( ZeidonStringCompare( pchViewName, 1, 0, "", 1, 0, 33 ) )
   {
      if ( CompareAttributeToString( OrigWO, "OptMap", "X_AttributeName", "" ) != 0 )
      {
         nZRetCode = PositionOnVOR( OrigW, NewW, pchViewName, vSubtask );
         nZRetCode = IncludeSubobjectFromSubobject( NewWO, "OptMapView", NewW, "ViewObjRef", zPOS_AFTER );
         if ( CompareAttributeToString( OrigWO, "OptMap", "X_AttributeName", "" ) != 0 )
         {
            RetrieveViewForMetaList( vSubtask, &LOD_List, zREFER_LOD_META );
            GetStringFromAttribute( szTempString, zsizeof( szTempString ), NewW, "LOD", "Name" );
            SetCursorFirstEntityByString( LOD_List, "W_MetaDef", "Name", szTempString, "" );
            ActivateMetaOI( vSubtask, &vLOD, LOD_List, zREFER_LOD_META, zSINGLE );
            GetStringFromAttribute( szTempString, zsizeof( szTempString ), OrigWO, "OptMap", "X_EntityName" );
            RESULT = SetCursorFirstEntityByString( vLOD, "LOD_Entity", "Name", szTempString, "" );
            if ( RESULT >= 0 )
            {
               nZRetCode = GetStringFromAttribute( szTempString, zsizeof( szTempString ), OrigWO, "OptMap", "X_AttributeName" );
               RESULT = SetCursorFirstEntityByString( vLOD, "ER_Attribute", "Name", szTempString, "LOD_Entity" );
               if ( RESULT >= 0 )
               {
                  nZRetCode = IncludeSubobjectFromSubobject( NewWO, "OptMapLOD_Attribute", vLOD, "LOD_Attribute", zPOS_AFTER );
               }
            }

            DropMetaOI( vSubtask, vLOD );
            DropView( LOD_List );
         }
      }
      else
      {
         SetAttributeFromString( NewWO, "OptMap", "X_ViewName", "" );
         SetAttributeFromString( NewWO, "OptMap", "X_EntityName", "" );
         SetAttributeFromString( NewWO, "OptMap", "X_AttributeName", "" );
         SetAttributeFromString( NewWO, "OptMap", "X_Context", "" );
      }
   }
   else
   {
      SetAttributeFromString( NewWO, "OptMap", "X_ViewName", "" );
      SetAttributeFromString( NewWO, "OptMap", "X_EntityName", "" );
      SetAttributeFromString( NewWO, "OptMap", "X_AttributeName", "" );
      SetAttributeFromString( NewWO, "OptMap", "X_Context", "" );
   }
   return( 0 );
}
