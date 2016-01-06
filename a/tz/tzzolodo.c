/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzzolodo.c - Zeidon application operations
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
// LAST MESSAGE ID: ZO00218
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

/*
CHANGE LOG
2001.05.24  BL   JOINS
   Added oTZZOLODO_DeleteAllPOD_Entities, ofnTZZOLODO_DeleteAllPOD_Entities,
   for delete all Join Definietions
   Added oTZZOLODO_DeleteOldPOD_Entities, ofnTZZOLODO_DeleteOldPOD_Entities
   for delete all POD_Entities with TE_DBMS_SourceForEntity, where this included
   DataSource does not exists in the TE

2000.06.20  BL
   Modified oTZZOLODO_AddEntity for show original attribute sequence

2000.05.21  BL
   Modified oTZZOLODO_SaveLOD_WithTE for show original attribute sequence

97.08.04    DonC
   Added reusable routine, oTZZOLODO_SaveLOD_WithTE, and corrected save of
   all LODs to only build LODs that had a POD and existed in the DBMS_Source
   that had cursor position.

28.05.97    RG
   Merged function 'oTZZOLODO_SaveAllLODsWithTE' with version 9f

04.09.1997  DonC
   Modified constraint processing for SeqPriority values.


16.12.1997  DonC
   Modified ConstraintOperACC to replace old code with a validation on VML
   Operation Names. Also modified the LOD TZZOLODO to trigger the constraint
   on an Operation accept.

19.01.1998  DonC
   Modified oTZZOLODO_ConstraintRecurs2 to remove an unnecessary SeqPriority
   constraint.

25.04.1998  DonC
   Deleted operation oTZZOLODO_SaveAllLODsWithTE because it was no longer used.

*/

zOPER_EXPORT zSHORT OPERATION
ofnTZZOLODO_RefLODChildEnt( zVIEW    vSubtask,
                            zVIEW    vTZZOLOD1,
                            zVIEW    vTZEREMD1,
                            zVIEW    vTZZOLODO,
                            zPCHAR   szLOD_Name );

zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SortAttributes( zVIEW vTZZOLODO );

static void
ofnTZZOLODO_DeleteAllPOD_Entities( zVIEW vTZZOLODO );

static void
ofnTZZOLODO_DeleteOldPOD_Entities( zVIEW vTZZOLODO, zVIEW vTZTENVRO );

zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_RefreshLOD_FromEMD( zVIEW    vSubtask,
                              zVIEW    vLOD,
                              zVIEW    vEntpModel )
{
   zVIEW    vTZZOLOD1;
   zVIEW    vTZZOLOD2;
   zVIEW    vTZZOLODW;
   zVIEW    vTZEREMD1;
// zVIEW    vTZEREMD2;
// zVIEW    vDomain;
// zVIEW    vT;
   zSHORT   nRC;
   zLONG    lEntityZKey;
   zLONG    lAttributeZKey;
// zLONG    lDomailZKey;
   zCHAR    szLOD_Name[ 33 ];
// zCHAR    szDomainName[ 33 ];
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zLONG    lTempInteger_1;
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 33 ];
   zCHAR    szTempString_1[ 33 ];
// zLONG    lTempInteger_2;

   CreateViewFromViewForTask( &vTZZOLOD1, vLOD, 0 );
   CreateViewFromViewForTask( &vTZEREMD1, vEntpModel, 0 );

   nZRetCode = GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ), vTZZOLOD1, "LOD", "Name" );
   RESULT = SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityParent", "" );
   lTempInteger_0 = CheckExistenceOfEntity( vTZZOLOD1, "ER_EntityRec" );
   if ( lTempInteger_0 == 0 )
   {
      nZRetCode = GetIntegerFromAttribute( &lTempInteger_1, vTZZOLOD1, "ER_EntityRec", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Entity", "ZKey", lTempInteger_1, "" );
      if ( RESULT <  zCURSOR_SET )
      {
       zCHAR  szMsg[ 500 ];

       ZeidonStringCopy( szMsg, 1, 0, "Unable to find Entity in ER.\n\n", 1, 0, 300 );
         ZeidonStringConcat( szMsg, 1, 0, szLOD_Name, 1, 0, 300 );
         ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
         nZRetCode = GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTZZOLOD1, "LOD_EntityParent", "Name" );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 300 );
         ZeidonStringConcat( szMsg, 1, 0, " \n\n", 1, 0, 300 );
         ZeidonStringConcat( szMsg, 1, 0, "Correct LOD Entity[ YES ] \n", 1, 0, 300 );
         ZeidonStringConcat( szMsg, 1, 0, "Bypass LOD Entity[ NO ] \n", 1, 0, 300 );
         ZeidonStringConcat( szMsg, 1, 0, "Cancel LOD Refresh[ CANCEL ] ", 1, 0, 300 );
         nRC = MessagePrompt( vTZZOLOD1, "ZO00201", "Operations", szMsg,
                             zBEEP, zBUTTONS_YESNOCANCEL, zRESPONSE_YES,  0 );
         if ( nRC == zRESPONSE_YES )
         {
            ZeidonStringCopy( szMsg, 1, 0, "Unable to find Entity in ER.\n\n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, szLOD_Name, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
            nZRetCode = GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTZZOLOD1, "LOD_EntityParent", "Name" );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n\n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Delete LOD Entity[ YES ] \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "     CAUTION - Child Entities will be deleted. \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Make LOD Entity Work[ NO ] ", 1, 0, 300 );
            nRC = MessagePrompt( vTZZOLOD1, "ZO00202",
                                 "Operations",
                                 szMsg,
                                 zBEEP, zBUTTONS_YESNO,
                                 zRESPONSE_YES,  0 );
            if ( nRC == zRESPONSE_YES )
            {
               nZRetCode = DeleteEntity( vTZZOLOD1, "LOD_EntityParent",
                                          zREPOS_NONE );
               return( nRC );
            }
            else
            {
               nRC = ExcludeEntity( vTZZOLOD1, "ER_EntityRec", zREPOS_NONE );
               nRC = ExcludeEntity( vTZZOLOD1, "ER_RelLinkRec", zREPOS_NONE );
               nZRetCode = SetAttributeFromString( vTZZOLOD1, "LOD_EntityParent", "Work", "Y" );
               RESULT = SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityChild", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               {
                  nRC = CreateViewFromViewForTask( &vTZZOLOD2, vTZZOLOD1, 0 );
                  nRC = SetViewToSubobject( vTZZOLOD2, "LOD_EntityChild" );
                  nRC = ExcludeEntity( vTZZOLOD2, "ER_RelLinkRec", zREPOS_NONE );
                  nZRetCode = SetAttributeFromString( vTZZOLOD2, "LOD_EntityParent", "Derived", "Y" );
                  RESULT = SetCursorNextEntity( vTZZOLOD1, "LOD_EntityChild", "" );
               }
               RESULT = SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityChild", "" );
            }
         }
         else
         {
            if ( nRC == zRESPONSE_CANCEL )
            {
               return( nRC );
            }
         }
      }
   }
   RESULT = SetCursorFirstEntity( vTZZOLOD1, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      if ( (CompareAttributeToString( vTZZOLOD1, "LOD_EntityParent", "Work", "Y" ) == 0) &&
           (CompareAttributeToString( vTZZOLOD1, "LOD_AttributeRec", "Work", "Y" ) != 0) )
      {
         // Work Attr Code here.
         CreateViewFromViewForTask( &vTZZOLODW, vTZZOLOD1, 0 );
         nRC = CreateMetaEntity( vSubtask, vTZZOLOD1, "ER_AttributeRec", zPOS_AFTER );
         SetMatchingAttributesByName( vTZZOLOD1, "ER_AttributeRec",
                                      vTZZOLODW, "ER_AttributeRec",
                                      zSET_NULL | zSET_NOTNULL );
         nRC = IncludeSubobjectFromSubobject( vTZZOLOD1, "DomainRec", vTZZOLODW, "DomainRec", zPOS_AFTER );
         nRC = ExcludeEntity( vTZZOLODW, "ER_AttributeRec", zREPOS_NEXT );
         nZRetCode = SetAttributeFromString( vTZZOLOD1, "LOD_AttributeRec", "Work", "Y" );
      }
      if ( CompareAttributeToString( vTZZOLOD1, "LOD_AttributeRec", "Work", "Y" ) != 0 )
      {
         nZRetCode = GetIntegerFromAttribute( &lEntityZKey, vTZZOLOD1, "ER_EntityRec", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Entity", "ZKey", lEntityZKey, "" );
         nZRetCode = GetIntegerFromAttribute( &lAttributeZKey, vTZZOLOD1, "ER_AttributeRec", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Attribute", "ZKey", lAttributeZKey, "" );
         if ( RESULT <  zCURSOR_SET )
         {
          zCHAR  szMsg[ 500 ];

         ZeidonStringCopy( szMsg, 1, 0, "Unable to find Attribute in ER.\n\n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "LOD:       ", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, szLOD_Name, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Entity:    ", 1, 0, 300 );
            nZRetCode = GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTZZOLOD1, "LOD_EntityParent", "Name" );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Attribute: ", 1, 0, 300 );
            nZRetCode = GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vTZZOLOD1, "ER_AttributeRec", "Name" );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n\n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Delete LOD Attribute[ YES ] \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Make LOD Attribute Work[ NO ] ", 1, 0, 300 );
            nRC = MessagePrompt( vTZZOLOD1, "ZO00203", "Operations", szMsg,
                                 zBEEP, zBUTTONS_YESNO, zRESPONSE_YES,  0 );
            if ( nRC == zRESPONSE_YES )
            {
               nZRetCode = DeleteEntity( vTZZOLOD1, "LOD_AttributeRec",
                                          zREPOS_NONE );
               RESULT = SetCursorNextEntity( vTZZOLOD1, "LOD_AttributeRec", "" );
               continue;
            }
            else
            {
               /* Work Attr Code here */
               CreateViewFromViewForTask( &vTZZOLODW, vTZZOLOD1, 0 );
               nRC = CreateMetaEntity( vSubtask, vTZZOLOD1, "ER_AttributeRec", zPOS_AFTER );
               SetMatchingAttributesByName( vTZZOLOD1, "ER_AttributeRec",
                                            vTZZOLODW, "ER_AttributeRec",
                                            zSET_NULL | zSET_NOTNULL );
               nRC = IncludeSubobjectFromSubobject( vTZZOLOD1, "DomainRec", vTZZOLODW, "DomainRec", zPOS_AFTER );
               nRC = ExcludeEntity( vTZZOLODW, "ER_AttributeRec", zREPOS_NEXT );
               nZRetCode = SetAttributeFromString( vTZZOLOD1, "LOD_AttributeRec", "Work", "Y" );
            }
         }
      }

      RESULT = SetCursorNextEntity( vTZZOLOD1, "LOD_AttributeRec", "" );
   }

   nRC = CheckExistenceOfEntity( vTZZOLOD1, "LOD_EntityChild" );
   if ( (nRC >= zCURSOR_SET) )
   {
      nRC = ofnTZZOLODO_RefLODChildEnt( vSubtask, vTZZOLOD1,
                                        vTZEREMD1, vLOD, szLOD_Name );
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZZOLODO_RefLODChildEnt( zVIEW    vSubtask,
                            zVIEW    vTZZOLOD1, zVIEW vTZEREMD1,
                            zVIEW    vTZZOLODO, zPCHAR szLOD_Name )
{
// zVIEW    vTZZOLODO;
   zVIEW    vTZZOLOD2;
   zVIEW    vTZZOLODW;
// zVIEW    vTZEREMD2;
// zVIEW    vDomain;
// zVIEW    vT;
   zSHORT   nRC;
   zLONG    lEntityZKey;
   zLONG    lAttributeZKey;
// zLONG    lDomailZKey;
// zCHAR    szDomainName[ 33 ];
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zLONG    lTempInteger_1;
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 33 ];
   zCHAR    szTempString_1[ 33 ];
   zCHAR    szTempString_2[ 33 ];
   zLONG    lTempInteger_2;
   zLONG    lTempInteger_3;
   zLONG    lTempInteger_4;
   zLONG    lTempInteger_5;

   CreateViewFromViewForTask( &vTZZOLOD2, vTZZOLOD1, 0 );
   RESULT = CheckExistenceOfEntity( vTZZOLOD1, "LOD_EntityChild" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nRC = SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
      lTempInteger_0 = CheckExistenceOfEntity( vTZZOLOD1, "ER_EntityRec" );
      if ( lTempInteger_0 == 0 )
      {
         nZRetCode = GetIntegerFromAttribute( &lTempInteger_1, vTZZOLOD1, "ER_EntityRec", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Entity", "ZKey", lTempInteger_1, "" );
         if ( RESULT < zCURSOR_SET )
         {
         zCHAR  szMsg[ 500 ];

         ZeidonStringCopy( szMsg, 1, 0, "Unable to find Entity in ER.\n\n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, szLOD_Name, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
            nZRetCode = GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTZZOLOD1, "LOD_EntityParent", "Name" );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n\n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Correct LOD Entity[ YES ] \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Bypass LOD Entity[ NO ] \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Cancel LOD Refresh[ CANCEL ] ", 1, 0, 300 );
            nRC = MessagePrompt( vTZZOLOD1, "ZO00204", "Operations", szMsg,
                                 zBEEP, zBUTTONS_YESNO, zRESPONSE_YES,  0 );
            if ( nRC == zRESPONSE_YES )
            {
               ZeidonStringCopy( szMsg, 1, 0, "Unable to find Entity in ER.\n\n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, szLOD_Name, 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
               nZRetCode = GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTZZOLOD1, "LOD_EntityParent", "Name" );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, " \n\n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, "Delete LOD Entity[ YES ] \n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, "     CAUTION - Child Entities will be lost on Delete \n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, "Make LOD Entity Work[ NO ] ", 1, 0, 300 );
               nRC = MessagePrompt( vTZZOLOD1, "ZO00205", "Operations", szMsg,
                                    zBEEP, zBUTTONS_YESNO, zRESPONSE_YES,  0 );
               if ( nRC == zRESPONSE_YES )
               {
                  nZRetCode = DeleteEntity( vTZZOLOD1, "LOD_EntityParent", zREPOS_NONE );
                  return( nRC );
               }
               else
               {
                  nRC = ExcludeEntity( vTZZOLOD1, "ER_EntityRec", zREPOS_NONE );
                  nRC = ExcludeEntity( vTZZOLOD1, "ER_RelLinkRec", zREPOS_NONE );
                  nZRetCode = SetAttributeFromString( vTZZOLOD1, "LOD_EntityParent", "Work", "Y" );
                  RESULT = SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityChild", "" );
                  while ( RESULT > zCURSOR_UNCHANGED )
                  {
                     nRC = CreateViewFromViewForTask( &vTZZOLOD2, vTZZOLOD1, 0 );
                     nRC = SetViewToSubobject( vTZZOLOD2, "LOD_EntityChild" );
                     nRC = ExcludeEntity( vTZZOLOD2, "ER_RelLinkRec", zREPOS_NONE );
                     nZRetCode = SetAttributeFromString( vTZZOLOD2, "LOD_EntityParent", "Derived", "Y" );
                     RESULT = SetCursorNextEntity( vTZZOLOD1, "LOD_EntityChild", "" );
                  }
                  RESULT = SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityChild", "" );
               }
            }
            else
            {
               if ( nRC == zRESPONSE_CANCEL )
               {
                  return( nRC );
               }
            }
         }
      }
      lTempInteger_2 = CheckExistenceOfEntity( vTZZOLOD1, "ER_RelLinkRec" );
      if ( lTempInteger_2 == 0 )
      {
         nZRetCode = GetIntegerFromAttribute( &lTempInteger_3, vTZZOLOD2, "ER_EntityRec", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Entity", "ZKey", lTempInteger_3, "" );
         nZRetCode = GetIntegerFromAttribute( &lTempInteger_4, vTZZOLOD1, "ER_RelLinkRec", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_RelLink", "ZKey", lTempInteger_4, "" );
         if ( RESULT >= zCURSOR_SET )
         {
            nZRetCode = GetIntegerFromAttribute( &lTempInteger_5, vTZZOLOD1, "ER_EntityRec", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Entity_Other", "ZKey", lTempInteger_5, "" );
         }
         if ( RESULT < zCURSOR_SET )
         {
          zCHAR  szMsg[ 500 ];

         ZeidonStringCopy( szMsg, 1, 0, "Unable to find RelLink in ER.\n\n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "LOD:           ", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, szLOD_Name, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Parent Entity: ", 1, 0, 300 );
            nZRetCode = GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTZZOLOD2, "LOD_EntityParent", "Name" );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Relationship:  ", 1, 0, 300 );
            nZRetCode = GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vTZZOLOD1, "ER_RelLinkRec", "Name" );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Child Entity:  ", 1, 0, 300 );
            nZRetCode = GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), vTZZOLOD1, "LOD_EntityParent", "Name" );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_2, 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, " \n\n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Make LOD Entity Derived[ YES ] \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Bypass LOD Entity[ NO ] \n", 1, 0, 300 );
            ZeidonStringConcat( szMsg, 1, 0, "Cancel LOD Refresh[ CANCEL ] ", 1, 0, 300 );
            nRC = MessagePrompt( vTZZOLOD1, "ZO00206", "Operations", szMsg,
                                 zBEEP, zBUTTONS_YESNOCANCEL, zRESPONSE_YES,  0 );
            if ( nRC == zRESPONSE_YES )
            {
               nRC = ExcludeEntity( vTZZOLOD1, "ER_RelLinkRec", zREPOS_NONE );
               nZRetCode = SetAttributeFromString( vTZZOLOD1, "LOD_EntityParent", "Derived", "Y" );
            }
            else
            {
               if ( nRC == zRESPONSE_CANCEL )
               {
                  return( nRC );
               }
            }
         }
      }
      RESULT = SetCursorFirstEntity( vTZZOLOD1, "LOD_AttributeRec", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         if (( CompareAttributeToString( vTZZOLOD1, "LOD_EntityParent", "Work", "Y" ) == 0 ) &&
            ( CompareAttributeToString( vTZZOLOD1, "LOD_AttributeRec", "Work", "Y" ) != 0 ))
         {
               /* Work Attr Code here */
            CreateViewFromViewForTask( &vTZZOLODW, vTZZOLOD1, 0 );
            nRC = CreateMetaEntity( vSubtask, vTZZOLOD1, "ER_AttributeRec", zPOS_AFTER );
            SetMatchingAttributesByName( vTZZOLOD1, "ER_AttributeRec",
                                         vTZZOLODW, "ER_AttributeRec",
                                         zSET_NULL | zSET_NOTNULL );
            nRC = IncludeSubobjectFromSubobject( vTZZOLOD1, "DomainRec", vTZZOLODW, "DomainRec", zPOS_AFTER );
            nRC = ExcludeEntity( vTZZOLODW, "ER_AttributeRec", zREPOS_NEXT );
            nZRetCode = SetAttributeFromString( vTZZOLOD1, "LOD_AttributeRec", "Work", "Y" );
         }
         if ( CompareAttributeToString( vTZZOLOD1, "LOD_AttributeRec", "Work", "Y" ) != 0 )
         {
            nZRetCode = GetIntegerFromAttribute( &lEntityZKey, vTZZOLOD1, "ER_EntityRec", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Entity", "ZKey", lEntityZKey, "" );
            nZRetCode = GetIntegerFromAttribute( &lAttributeZKey, vTZZOLOD1, "ER_AttributeRec", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Attribute", "ZKey", lAttributeZKey, "" );
            if ( RESULT <  zCURSOR_SET )
            {
            zCHAR  szMsg[ 500 ];

            ZeidonStringCopy( szMsg, 1, 0, "Unable to find Attribute in ER.\n\n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, "LOD:       ", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, szLOD_Name, 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, "Entity:    ", 1, 0, 300 );
               nZRetCode = GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTZZOLOD1, "LOD_EntityParent", "Name" );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, " \n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, "Attribute: ", 1, 0, 300 );
               nZRetCode = GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vTZZOLOD1, "ER_AttributeRec", "Name" );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, " \n\n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, "Delete LOD Attribute [ YES ] \n", 1, 0, 300 );
               ZeidonStringConcat( szMsg, 1, 0, "Make LOD Attribute Work[ NO ] ", 1, 0, 300 );
               nRC = MessagePrompt( vTZZOLOD1, "ZO00207",
                                    "Operations",
                                    szMsg,
                                    zBEEP, zBUTTONS_YESNO,
                                    zRESPONSE_YES,  0 );
               if ( nRC == zRESPONSE_YES )
               {
                  nZRetCode = DeleteEntity( vTZZOLOD1, "LOD_AttributeRec",
                                             zREPOS_NONE );
                  RESULT = SetCursorNextEntity( vTZZOLOD1, "LOD_AttributeRec", "" );
                  continue;
               }
               else
               {
               /* Work Attr Code here */
                  CreateViewFromViewForTask( &vTZZOLODW, vTZZOLOD1, 0 );
                  nRC = CreateMetaEntity( vSubtask, vTZZOLOD1, "ER_AttributeRec", zPOS_AFTER );
                  SetMatchingAttributesByName( vTZZOLOD1, "ER_AttributeRec",
                                               vTZZOLODW, "ER_AttributeRec",
                                               zSET_NULL | zSET_NOTNULL );
                  nRC = IncludeSubobjectFromSubobject( vTZZOLOD1, "DomainRec", vTZZOLODW, "DomainRec", zPOS_AFTER );
                  nRC = ExcludeEntity( vTZZOLODW, "ER_AttributeRec", zREPOS_NEXT );
                  nZRetCode = SetAttributeFromString( vTZZOLOD1, "LOD_AttributeRec", "Work", "Y" );
               }
            }
         }

         RESULT = SetCursorNextEntity( vTZZOLOD1, "LOD_AttributeRec", "" );
      }

      nRC = CheckExistenceOfEntity( vTZZOLOD1, "LOD_EntityChild" );
      if ( nRC >= zCURSOR_SET )
      {
         nRC = ofnTZZOLODO_RefLODChildEnt( vSubtask, vTZZOLOD1, vTZEREMD1,
                                           vTZZOLODO, szLOD_Name );
         if ( nRC == zRESPONSE_CANCEL )
         {
            return( nRC );
         }
      }

      nRC = ResetViewFromSubobject( vTZZOLOD1 );
      RESULT = SetCursorNextEntity( vTZZOLOD1, "LOD_EntityChild", "" );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_SaveAllLOD's
//
// PURPOSE:   General Function to save all LOD's and XOD's to reflect
//            the current state of the ER.
//
// TRIGGLODED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddEnt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SaveAllLODs( zVIEW vSubtask, zPCHAR szType )
{
   zVIEW  vTZZOLODO;
   zVIEW  vTZZOXODO;
   zVIEW  vTZZOLODO_LIST;
   zVIEW  vTZEREMDO_REF;
   zVIEW  vWindow;
   zSHORT nRC;
   zSHORT nLODisRefer;
   zSHORT nLODisActive;
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szMsg[ 128 ];

   // Get access to ER Object
   nRC = GetViewByName( &vTZEREMDO_REF, "TZEREMDO_REF", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      nRC = RetrieveViewForMetaList( vSubtask, &vTZZOLODO_LIST, zREFER_ERD_META );
      nRC = ActivateMetaOI( vSubtask, &vTZEREMDO_REF, vTZZOLODO_LIST,
                            zREFER_ERD_META, zSINGLE | zLEVEL_APPLICATION );
      nRC = SetNameForView( vTZEREMDO_REF, "TZEREMDO_REF", vSubtask, zLEVEL_TASK );
   }

   // Get access to LOD List Object
   nRC = RetrieveViewForMetaList( vSubtask, &vTZZOLODO_LIST, zREFER_LOD_META );

   // Loop for all LOD's in LPLR
   for ( nRC = SetCursorFirstEntity( vTZZOLODO_LIST, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZZOLODO_LIST, "W_MetaDef", "" ) )
   {
      // Determine if LOD is Active
      if ( CompareAttributeToInteger( vTZZOLODO_LIST, "W_MetaDef",
                                      "Status", 1 ) == 0 )
      {
         nLODisActive = 1;
         nLODisRefer  = 0;
      }
      else
      {
         nLODisActive = 0;
         nLODisRefer  = 1;
      }

      // Activate LOD
      if ( nLODisActive )
      {
         nRC = ActivateMetaOI( vSubtask, &vTZZOLODO, vTZZOLODO_LIST,
                               zSOURCE_LOD_META, zSINGLE | zLEVEL_APPLICATION );
      }
      else
      {
         nRC = ActivateMetaOI( vSubtask, &vTZZOLODO, vTZZOLODO_LIST,
                               zREFER_LOD_META, zSINGLE | zLEVEL_APPLICATION );
      }

      nRC = GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ), vTZZOLODO, "LOD", "Name" );

      // Send message to active window.
      nRC = GetViewByName( &vWindow, "TE_Window", vSubtask, zLEVEL_TASK );
      if ( nRC >= 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Saving LOD: " );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         MB_SetMessage( vWindow, 1, szMsg );
      }

      // Refresh LOD to current ER Model
      nRC = oTZZOLODO_RefreshLOD_FromEMD( vSubtask, vTZZOLODO, vTZEREMDO_REF );

      // Make sure the LOD is not empty
      nRC = CheckExistenceOfEntity( vTZZOLODO, "LOD" );
      if ( nRC < zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Unable to Save empty LOD.\n" );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         MessageSend( vSubtask, "ZO00208", "Operations",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }

      // Save the LOD to a file and if successful Build/Save XOD
      if ( nLODisActive )
      {
         nRC = CommitMetaOI( vSubtask, vTZZOLODO, zSOURCE_LOD_META );
      }
      else
      {
         nRC = CommitDependentOI( vSubtask, vTZZOLODO, zREFER_LOD_META );
      }

      DropMetaOI( vSubtask, vTZZOLODO );

      // Get access to the newly built XOD
      nRC = GetViewByName( &vTZZOXODO, "TZZOXODO", vSubtask, zLEVEL_TASK );
      if ( nRC < 1 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "(oTZZOLODO_SaveAllLODs) Unable to Access XOD.  XOD build error." );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         MessageSend( vSubtask, "ZO00209", "Operations",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }

   } // END for each W_MetaDef

   DropView( vTZEREMDO_REF );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_SaveLOD_WithTE
//
// PURPOSE:   Commit and build a particular LOD with a TE.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SaveLOD_WithTE( zVIEW vSubtask,
                          zVIEW vTZZOLODO,
                          zVIEW vTZTENVRO_REF )
{
   zSHORT nRC;
   zVIEW  vTZWDVORO;
   zCHAR  szLOD_Name[ 33 ];

   // If POD references a TE, exclude it so that a new include can be done
   if ( CheckExistenceOfEntity ( vTZZOLODO, "TE_DB_Environ" ) >= zCURSOR_SET )
      ExcludeEntity ( vTZZOLODO, "TE_DB_Environ", zREPOS_NONE );

   IncludeSubobjectFromSubobject( vTZZOLODO, "TE_DB_Environ",
                                  vTZTENVRO_REF, "TE_DB_Environ",
                                  zPOS_FIRST );

   // Make sure the TE_SourceZKey attribute is set because it determines
   // what DBMS_Source is used in building the XOD.
   SetAttributeFromAttribute ( vTZZOLODO, "POD", "TE_SourceZKey",
                               vTZTENVRO_REF, "TE_DBMS_Source", "ZKey" );

   // Sort Attributes in original sequence
   oTZZOLODO_SortAttributes( vTZZOLODO );

   // Save LOD
   nRC = CommitMetaOI( vSubtask, vTZZOLODO, zSOURCE_LOD_META );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "ZO00214", "Zeidon Object Maintenance",
                   "Unable to Save LOD to LPLR.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }


   // Build default VOR by Name of LOD if it does not exist
   GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ), vTZZOLODO, "LOD", "Name" );
   nRC = ActivateMetaOI_ByName( vSubtask, &vTZWDVORO, 0,
                                zREFER_VOR_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                szLOD_Name, 0 );
   if ( nRC < 0 )
   {
      ActivateEmptyMetaOI( vSubtask, &vTZWDVORO, zSOURCE_VOR_META,
                            zSINGLE | zLEVEL_APPLICATION );
      CreateMetaEntity( vSubtask, vTZWDVORO, "ViewObjRef", zPOS_AFTER );
      SetAttributeFromAttribute ( vTZWDVORO, "ViewObjRef", "Name",
                                  vTZZOLODO, "LOD", "Name" );
      SetAttributeFromString ( vTZWDVORO, "ViewObjRef", "Desc",
                               "VOR automatically built from LOD Dialog" );
      IncludeSubobjectFromSubobject ( vTZWDVORO, "LOD",
                                      vTZZOLODO, "LOD",
                                      zPOS_AFTER );
      nRC = CommitMetaOI( vSubtask, vTZWDVORO, zSOURCE_VOR_META );
      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "ZO00215", "Zeidon Object Maintenance",
                     "Unable to Save VOR to LPLR.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
      {
         DropMetaOI( vSubtask, vTZWDVORO );
      }

   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: ofnTZZOLODO_AddEntity
//
// PURPOSE:   General Function to Add an Entity to a LOD.  The parm
//            szType will be one of the following;
//                    W - Add Work Entity
//                    E - Add Entity from ER Entity(DERIVED)
//                    R - Add Entity from ER RelLink
//
// TRIGGLODED BY:
//    WINDOW: TZZOLODD
//    ACTION: BUTTON
//    NAME: AddEnt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_AddEntity( zVIEW  vSubtask,
                     zVIEW vTZZOLODO,
                     zPCHAR szType )
{
   zVIEW vTZZOLOD1;
   zVIEW vTZZOLOD2;
   zVIEW vTZEREMDO;
   zVIEW vTZEREMDO2;
   zSHORT nRC;
   zSHORT nRC2;
   zUSHORT uLevel;
   zLONG  lIndentLvl;
   zLONG  lCount;
   zSHORT nPosition;
   zCHAR  szEntityName[ 33 ];
   zCHAR  szEntityNameIndent[ 133 ];
   zCHAR  szWorkType[ 2 ];
   zLONG  lZKey;
   zLONG  lSrcZKey;
   zLONG  lTgtZKey = -1;

   strcpy_s( szWorkType, sizeof( szWorkType ), szType );

//  Don't access ER or SA on Work Entity
   if ( zstrcmp( szWorkType, "W" ) != 0 )
   {
   //  Get access to ER Object
      nRC = GetViewByName( &vTZEREMDO, "TZEREMDO", vTZZOLODO, zLEVEL_TASK );
      if ( nRC < 1 )
      {
         MessageSend( vTZZOLODO, "ZO00212", "Operations",
                      "TZEREMDO view does not exist.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
   }

//  Position on LOD for LOD Entity Add
   nRC = GetIntegerFromAttribute( &lSrcZKey, vTZZOLODO, "LOD_Entity", "ZKey" );
   nRC = CreateViewFromViewForTask( &vTZZOLOD1, vTZZOLODO, 0 );
   nRC = SetCursorFirstEntity( vTZZOLOD1, "LOD_EntityParent", "" );
   nRC = DefineHierarchicalCursor( vTZZOLOD1, "LOD" );
   while ( lSrcZKey != lTgtZKey )
   {
      nRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName, vTZZOLOD1 );
      if ( nRC >= zCURSOR_SET_RECURSIVECHILD )
      {
         nRC = SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );
         GetIntegerFromAttribute( &lTgtZKey, vTZZOLOD1, "LOD_EntityParent", "ZKey" );
      }
      if ( zstrcmp( szEntityName, "LOD_EntityParent" ) == 0 )
      {
         GetIntegerFromAttribute( &lTgtZKey, vTZZOLOD1, "LOD_EntityParent", "ZKey" );
      }
   }

   DropHierarchicalCursor( vTZZOLOD1 );

   // Create LOD Entity and set the Name
   if ( zstrcmp( szWorkType, "W" ) == 0 )
      CreateTemporalMetaEntity( vSubtask, vTZZOLOD1, "LOD_EntityChild", zPOS_LAST );
   else
      CreateMetaEntity( vSubtask, vTZZOLOD1, "LOD_EntityChild", zPOS_LAST );

   if ( zstrcmp( szWorkType, "R" ) == 0 )
   {
      SetAttributeFromAttribute( vTZZOLOD1, "LOD_EntityChild", "Name",
                                 vTZEREMDO, "ER_Entity_Other", "Name" );
      SetAttributeFromAttribute( vTZZOLOD1, "LOD_EntityChild", "Desc",
                                 vTZEREMDO, "ER_Entity_Other", "Desc" );
   }
   else
   {
      if ( zstrcmp( szWorkType, "E" ) == 0 )
      {
         SetAttributeFromAttribute( vTZZOLOD1, "LOD_EntityChild", "Name",
                                    vTZEREMDO, "ER_Entity", "Name" );
         SetAttributeFromAttribute( vTZZOLOD1, "LOD_EntityChild", "Desc",
                                    vTZEREMDO, "ER_Entity", "Desc" );
      }
//    else
//    {
//       SetAttributeFromString( vTZZOLOD1, "LOD_EntityChild", "Name", "New" );
//    }
   }

//  Set the IndentLvl and IndentName
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vTZZOLODO, "LOD_EntityChild", "Name" );
   lIndentLvl = (uLevel - 1) * 3;
   SetAttributeFromInteger( vTZZOLOD1, "LOD_EntityChild", "IndentLvl", lIndentLvl );
   lCount = 0;
   while ( lIndentLvl > lCount )
   {
      szEntityNameIndent[ lCount++ ] = ' ';
   }
   strcpy_s( szEntityNameIndent + lIndentLvl, sizeof( szEntityNameIndent ) - lIndentLvl, szEntityName );
   SetAttributeFromString( vTZZOLOD1, "LOD_EntityChild", "IndentName", szEntityNameIndent );


//  Build subordinate Entities under the LOD_Entity
   SetViewToSubobject( vTZZOLOD1, "LOD_EntityChild" );

//  Build ER_Entity for Relationship or ER Entity Add Type
   if ( zstrcmp( szWorkType, "E" ) == 0 )
      IncludeSubobjectFromSubobject( vTZZOLOD1, "ER_EntityRec", vTZEREMDO, "ER_Entity", zPOS_AFTER );

//  Build ER_Entity for Relationship Add Type
   if ( zstrcmp( szWorkType, "R" ) == 0 )
   {
      IncludeSubobjectFromSubobject( vTZZOLOD1, "ER_EntityRec", vTZEREMDO, "ER_Entity_Other", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vTZZOLOD1, "ER_RelLinkRec", vTZEREMDO, "ER_RelLink", zPOS_AFTER );
      if ( CompareAttributeToAttribute( vTZEREMDO, "ER_RelLink_Other", "ZKey",
                                        vTZEREMDO, "ER_RelLink", "ZKey" ) == 1 )
      {
         SetAttributeFromString( vTZZOLOD1, "LOD_EntityParent", "RelLinkIndicator", "L" );
      }
      else
      {
         SetAttributeFromString( vTZZOLOD1, "LOD_EntityParent", "RelLinkIndicator", "H" );
      }
   }

//  Build Attributes for Relationship or ER Entity Add Type
   if (( zstrcmp( szWorkType, "R" ) == 0 ) ||
      ( zstrcmp( szWorkType, "E" ) == 0 ))
   {
      if ( zstrcmp( szWorkType, "R" ) == 0 )
      {
         GetIntegerFromAttribute( &lZKey, vTZEREMDO, "ER_Entity_Other", "ZKey" );
      }
      else
      {
         GetIntegerFromAttribute( &lZKey, vTZEREMDO, "ER_Entity", "ZKey" );
      }

      CreateViewFromViewForTask( &vTZEREMDO2, vTZEREMDO, 0 );
      SetCursorFirstEntityByInteger( vTZEREMDO2, "ER_Entity", "ZKey", lZKey, "" );
      nRC = SetCursorFirstEntity( vTZEREMDO2, "ER_Attribute", "" );
      while ( nRC >= zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vTZZOLOD1, "LOD_AttributeRec", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vTZZOLOD1, "ER_AttributeRec", vTZEREMDO2, "ER_Attribute", zPOS_AFTER );
         nRC = SetCursorNextEntity( vTZEREMDO2, "ER_Attribute", "" );
      }

      DropView( vTZEREMDO2 );
   }


//  Build Flat Side representation of LOD Entity just added on rec side

//  Position on Flat Side for correct sequencing of Entities as they
//  are added.
   CreateViewFromViewForTask( &vTZZOLOD2, vTZZOLOD1, 0 );
   do
   {
      nRC = ResetViewFromSubobject( vTZZOLOD2 );
      nRC2 = SetCursorNextEntity( vTZZOLOD2, "LOD_EntityParent", "" );
   }
   while (( nRC == 0 ) &&( nRC2 <  zCURSOR_SET ) );
   if ( nRC2 <  zCURSOR_SET )
   {
      nPosition = zPOS_AFTER;
      SetCursorLastEntity( vTZZOLODO, "LOD_Entity", "" );
   }
   else
   {
      nPosition = zPOS_BEFORE;
      SetCursorFirstEntityByEntityCsr( vTZZOLODO, "LOD_Entity",
                                       vTZZOLOD2, "LOD_EntityParent",
                                       "" );
   }

// Include Recursive Representation to Flat Representation
   IncludeSubobjectFromSubobject( vTZZOLODO, "LOD_Entity", vTZZOLOD1, "LOD_EntityParent", nPosition );

   // Set Attribute Sequence for new Entity
   nRC = SetCursorFirstEntity( vTZZOLODO, "LOD_Attribute", "" );
   while ( nRC >= zCURSOR_SET )
   {
      oTZZOLODO_NewAttributeSequence( vTZZOLODO );
      nRC = SetCursorNextEntity( vTZZOLODO, "LOD_Attribute", "" );
   }
   SetCursorFirstEntity( vTZZOLODO, "LOD_Attribute", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: ofnTZZOLODO_DeriveKey
//
// PURPOSE:   Derived function to set ParticipatesInKey for LOD_Attribute
//            and LOD_AttributeRec Entities in TZZOLODO
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_DeriveKey( zVIEW            vTZZOLODO,
                     LPVIEWENTITY     lpViewEntity,
                     LPVIEWATTRIB     lpViewAttrib,
                     zSHORT           nMsg )
{
   zVIEW vTZEREMDO_REF;
   zVIEW vT;
// zPCHAR  szAttr;
   zPCHAR  szEntityER;
   zPCHAR  szAttribER;
   zSHORT nRC;

   // Set up the Entity and Attribute names for use in SetCurosor operations
   // based on which LOD Entity is using the derivation algorithm.
   if ( zstrcmp( lpViewEntity->szName, "LOD_AttributeRec" ) == 0 )
   {
      szEntityER = "ER_EntityRec";
      szAttribER = "ER_AttributeRec";
   }
   else
   {
      szEntityER = "ER_Entity";
      szAttribER = "ER_Attribute";
   }

   // Get access to ER Object.
   nRC = GetViewByName( &vTZEREMDO_REF, "TZEREMDO_REF", vTZZOLODO, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      nRC = RetrieveViewForMetaList( vTZZOLODO, &vT, zREFER_ERD_META );
      if ( CheckExistenceOfEntity( vT, "W_MetaDef" ) >= zCURSOR_SET )
      {
         ActivateMetaOI( vTZZOLODO, &vTZEREMDO_REF, vT,
                         zREFER_ERD_META,
                         zSINGLE | zLEVEL_APPLICATION );
         nRC = SetNameForView( vTZEREMDO_REF, "TZEREMDO_REF", vTZZOLODO, zLEVEL_TASK );
      }
      else
         nRC = -1;
   }

   // Derive Participates in Key.
   if ( nRC >= 0 &&
        CheckExistenceOfEntity( vTZZOLODO, szEntityER ) >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO_REF, "ER_Entity",
                                             vTZZOLODO, szEntityER, "" );
      if ( nRC >= zCURSOR_SET )
      {
         // now set the LOD_Attribute.ParticipatesInKey from
         //             ER_Attribute.ParticipatesInKey.
         // the folowing "correct" method is replaced for speed
#if 0
         SetCursorFirstEntityByEntityCsr( vTZEREMDO_REF, "ER_Attribute",
                                          vTZZOLODO, szAttributeER, "" );
         nRC = GetAddrForAttribute( &szAttr, vTZEREMDO_REF, "ER_Attribute",
                                    "ParticipatesInKey" );
         StoreValueInRecord( vTZZOLODO, lpViewEntity, lpViewAttrib,
                             szAttr, 0 );
#else
         SetCursorFirstEntity( vTZEREMDO_REF, "ER_EntIdentifier", "" );
         nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO_REF, "ER_AttributeIdentifier",
                                                vTZZOLODO, szAttribER,
                                                "ER_EntIdentifier" );
         if ( nRC >= zCURSOR_SET )
         {
            StoreValueInRecord( vTZZOLODO, lpViewEntity, lpViewAttrib, "Y", 0 );
         }
         else
         {
            StoreValueInRecord( vTZZOLODO, lpViewEntity, lpViewAttrib, "N", 0 );
         }
#endif
      }
   }
   else
   {
      StoreValueInRecord( vTZZOLODO, lpViewEntity, lpViewAttrib, "", 0 );
      nRC = -1;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_DIL_Inherited
//
// PURPOSE:   Temporary only.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_DIL_Inherited( zVIEW            vIn,
                         LPVIEWENTITY     lpViewEntity,
                         LPVIEWATTRIB     lpViewAttrib,
                         zSHORT           nMsg )
{

   // This routine is a no-op because there is no way to determine what DIL message
   // will be inherited from the Domain as the Context from which the DIL message
   // would be inherited is unknown until the Context is determined.

   // It should eventually be deleted, along with the derived attribute when no
   // portable files exist with the attribute value.

   StoreValueInRecord( vIn, lpViewEntity, lpViewAttrib, "", 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_DIL_TextMsg
//
// PURPOSE:   Temporary only.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_DIL_TextMsg( zVIEW            vIn,
                       LPVIEWENTITY     lpViewEntity,
                       LPVIEWATTRIB     lpViewAttrib,
                       zSHORT           nMsg )
{
   // This is a temporary operation that should be deleted, along with the
   // derived attribute when no portable file exists that include the attribute
   // value.

   if ( nMsg == zDERIVED_GET )
   {

      StoreValueInRecord( vIn, lpViewEntity, lpViewAttrib, "", 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_ConstraintRecurs1( zVIEW   vSubtask,
                             zVIEW   vLOD,
                             zVIEW   vERD,
                             zPCHAR  szLOD_Name)
{
   zVIEW vParent;
   zCHAR szMsg[1000];
   zCHAR szParentLOD_Name[ 33 ];
   zCHAR szChildLOD_Name[ 33 ];
   zCHAR szRelLinkName[ 33 ];
   zSHORT nRC;


   //  Loop for all LOD_Children in LPLR
   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_EntityParent", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_EntityParent", "" ) )
   {
      // just take these which are creatable, no work-entities, no derived entities
      // and which habe related entities in the ERD and
      // the ER-Entity is not a work-entity
      if ( CompareAttributeToString( vLOD, "LOD_EntityParent", "Create", "Y" ) == 0 &&
           CompareAttributeToString( vLOD, "LOD_EntityParent", "Work", "Y" ) != 0 &&
           CompareAttributeToString( vLOD, "LOD_EntityParent", "Derived", "Y" ) != 0 &&
           SetCursorFirstEntityByEntityCsr( vERD, "ER_Entity",
                                            vLOD, "ER_EntityRec", "" ) >= zCURSOR_SET &&
           CompareAttributeToString( vERD, "ER_Entity", "Purpose", "W" ) != 0 )
      {
         // Create view looking at parent of current LOD_EntityParent.
         CreateViewFromViewForTask( &vParent, vLOD, 0 );
         ResetViewFromSubobject( vParent );
         GetStringFromAttribute( szParentLOD_Name, sizeof( szParentLOD_Name ), vLOD, "LOD_EntityParent", "Name" );
         SetViewToSubobject( vLOD, "LOD_EntityChild" );

         // loop thru all the ER-related Entities
         for ( nRC = SetCursorFirstEntity( vERD, "ER_RelLink", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vERD, "ER_RelLink", "" ) )
         {


            // is the minimum card > 0 ?
            if ( CompareAttributeToString( vERD, "ER_RelLink", "CardMin", "0" ) != 0 &&
                 // ist the ER-related Entity equal to the lod-parent-entity ?
                 CompareAttributeToAttribute( vERD, "ER_Entity_Other", "ZKey",
                                              vParent, "ER_EntityRec", "ZKey") != 0 )

            {
               // position on the LOD_ChildEntity which matches the other ER_Entity,
               // take just these which are no work-entities
               for ( nRC = SetCursorFirstEntity( vLOD, "LOD_EntityParent", "" );
                     nRC >= zCURSOR_SET &&
                     ( CompareAttributeToString( vLOD, "LOD_EntityParent", "Work", "Y" ) == 0 ||
                       CompareAttributeToString( vLOD, "LOD_EntityParent", "Derived", "Y" ) == 0 ||
                       CompareAttributeToAttribute( vLOD, "ER_EntityRec", "ZKey",
                                                    vERD, "ER_Entity_Other", "ZKey") != 0 );
                     nRC = SetCursorNextEntity( vLOD, "LOD_EntityParent", "" ) );

               // if the old was not found or is not includable or creatable
               // issue an error
               if ( nRC < zCURSOR_SET ||
                    (CompareAttributeToString( vLOD, "LOD_EntityParent", "Include", "Y" ) != 0 &&
                     CompareAttributeToString( vLOD, "LOD_EntityParent", "Create", "Y" ) != 0 ) )
               {
                   strcpy_s( szMsg, sizeof( szMsg ), "A creatable LOD Entity is missing a child (includable or creatable) ");
                   strcat_s( szMsg, sizeof( szMsg ), " for a related ERD Entity with min cardinality > 0.\n\n" );
                   strcat_s( szMsg, sizeof( szMsg ), "LOD: \t \t \t" );

                   strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
                   strcat_s( szMsg, sizeof( szMsg ), "\n \nCreateable LOD Entity: \t" );
                   strcat_s( szMsg, sizeof( szMsg ), szParentLOD_Name );
                   strcat_s( szMsg, sizeof( szMsg ), "\nrelationship with card > 0: \t" );
                   GetStringFromAttribute( szRelLinkName, sizeof( szRelLinkName ), vERD, "ER_RelLink", "Name" );
                   strcat_s( szMsg, sizeof( szMsg ), szRelLinkName );
                   GetStringFromAttribute( szChildLOD_Name, sizeof( szChildLOD_Name ), vERD, "ER_Entity_Other", "Name" );
                   strcat_s( szMsg, sizeof( szMsg ), "\nMissing related ERD Entity: \t" );
                   strcat_s( szMsg, sizeof( szMsg ), szChildLOD_Name );
                   MessageSend( vSubtask, "ZO00208", "LOD Constraints",
                                szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
               }
            }
         }

         DropView( vParent );
         if ( CheckExistenceOfEntity( vLOD, "LOD_EntityParent" ) >= zCURSOR_SET )
            // Process next level recursively.
            oTZZOLODO_ConstraintRecurs1( vSubtask, vLOD, vERD, szLOD_Name );

         ResetViewFromSubobject( vLOD );

      }
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_ConstraintRecurs2( zVIEW   vSubtask,
                             zVIEW   vLOD,
                             zVIEW   vERD,
                             zPCHAR  szLOD_Name)
{
   zCHAR  szMsg[1000];
   zCHAR  szEntityName[ 33 ];
//   zCHAR  szTempName[ 33 ];
   zCHAR  szAktPriority[ 10 ];
   zPCHAR pchAttribList[ 20 ];
   zCHAR  szAttribList[ 255 ];
   zSHORT i;
   zSHORT nMissingNr;
   zLONG  lAktPriority;
   zLONG  lMaxPriority;
   zSHORT nRC;


   //  Loop for all LOD_Children in LPLR
   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_EntityParent", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_EntityParent", "" ) )
   {
      // just take these which are normal ER, no work-entities, no derived
      // entities and which have related entities in the ERD and
      // the ER-Entity is not a work-entity
      if ( CompareAttributeToString( vLOD, "LOD_EntityParent", "Work", "Y" ) != 0 &&
           CompareAttributeToString( vLOD, "LOD_EntityParent", "Derived", "Y" ) != 0 &&
           SetCursorFirstEntityByEntityCsr( vERD, "ER_Entity",
                                            vLOD, "ER_EntityRec", "" ) >= zCURSOR_SET &&
           CompareAttributeToString( vERD, "ER_Entity", "Purpose", "W" ) != 0 )
      {

         // CONSTRAINT for SeqPriority
         //
         // 1. If the maximum cardinality of an entity is not 1, make sure all
         //    attributes with SeqPriority specified have them specified in
         //    sequential numbers 1 thru n.
         // 2. For the root entity, check the SeqPriority of attributes for
         //    any child entities with maximum cardinality 1.  They must be:
         //    1) in sequential order 1 thru n, along with any SeqPriority
         //       specifications of the root entity.
         //    2) for a key attribute of the child entity (in other words, a
         //       foreign key of the root entity).

         GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vLOD, "LOD_EntityParent", "Name" );

         lMaxPriority = 0;

         // initialize pchAttribList
         for ( i =1; i < 20; i++ )
            pchAttribList[ i ] = 0;

         // loop through all the Attributes in the Entity to fill
         // AttribList.  For the root entity, also loop through child
         // entities with CardMax of 1.

         for ( nRC = SetCursorFirstEntity( vLOD, "LOD_AttributeRec", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vLOD, "LOD_AttributeRec", "" ) )
         {
            GetIntegerFromAttribute( &lAktPriority, vLOD, "LOD_AttributeRec", "SeqPriority" );
            // just take the attributes with a Priotity > 0, < 16 (only those aer valid)
            if ( lAktPriority > 0 &&
               lAktPriority < 16 )
            {
               GetAddrForAttribute( pchAttribList + lAktPriority,
                                    vLOD, "ER_AttributeRec", "Name" );

               if ( lAktPriority > lMaxPriority )
                  lMaxPriority = lAktPriority;
            }
         }

         if ( CheckExistenceOfEntity( vLOD, "LOD_EntityParent" ) >= zCURSOR_SET &&
              CompareAttributeToInteger( vLOD, "LOD_EntityParent", "IndentLvl",
                                         0 ) == 0 )
         {
            // This is root entity.
            SetViewToSubobject( vLOD, "LOD_EntityChild" );


            for ( nRC = SetCursorFirstEntity( vLOD, "LOD_EntityParent", "" );
                  nRC >= zCURSOR_SET;
                  nRC = SetCursorNextEntity( vLOD, "LOD_EntityParent", "" ) )
            {
               // Process only for child entities of CardMax 1.
               if ( CheckExistenceOfEntity( vLOD, "ER_RelLinkRec" ) >= zCURSOR_SET &&
                    CompareAttributeToString( vLOD, "ER_RelLinkRec", "CardMax",
                                              "1" ) == 0 )
               {
                  for ( nRC = SetCursorFirstEntity( vLOD, "LOD_AttributeRec", "" );
                        nRC >= zCURSOR_SET;
                        nRC = SetCursorNextEntity( vLOD, "LOD_AttributeRec", "" ) )
                  {
                     GetIntegerFromAttribute( &lAktPriority, vLOD, "LOD_AttributeRec", "SeqPriority" );
                     // just take the attributes with a Priotity > 0, < 16 (only those aer valid)
                     if ( lAktPriority > 0 &&
                        lAktPriority < 16 )
                     {
                        GetAddrForAttribute( pchAttribList + lAktPriority,
                                             vLOD, "ER_AttributeRec", "Name" );

                        if ( lAktPriority > lMaxPriority )
                           lMaxPriority = lAktPriority;

#if 0
              *** Deleted following code 19.01.1998 because it isn't really
                  necessary to check that the Attribute is part of the
                  Identifier.
                        // Check that attribute with the SeqPriority value is
                        // an identifier and thus a potential foreign key in
                        // the parent.
                        SetCursorFirstEntityByEntityCsr( vERD, "ER_Entity",
                                                         vLOD, "ER_EntityRec", "" );
                        if ( SetCursorFirstEntityByEntityCsr( vERD, "ER_AttributeIdentifier",
                                                            vLOD, "ER_AttributeRec", "ER_EntIdentifier" )
                                                            >= zCURSOR_SET )
                        {
                           GetAddrForAttribute( pchAttribList + lAktPriority,
                                                vLOD, "ER_AttributeRec", "Name" );

                           if ( lAktPriority > lMaxPriority )
                              lMaxPriority = lAktPriority;
                        }
                        else
                        {
                           // This condition then is an error.
                           strcpy_s( szMsg, sizeof( szMsg ), "The following attribute has a SeqPriority value but is " );
                           strcat_s( szMsg, sizeof( szMsg ), "not an identifier nor a part of an entity with multiple " );
                           strcat_s( szMsg, sizeof( szMsg ), "occurrences under its parent.  The SeqPriority value is ignored.\n" );
                           strcat_s( szMsg, sizeof( szMsg ), "\n\nEntity: \t" );
                           GetStringFromAttribute( szTempName, sizeof( szTempName ),
                                                   vLOD, "LOD_EntityParent", "Name" );
                           strcat_s( szMsg, sizeof( szMsg ), szTempName );
                           strcat_s( szMsg, sizeof( szMsg ), "\n\nAttribute: \t" );
                           GetStringFromAttribute( szTempName, sizeof( szTempName ),
                                                   vLOD, "ER_AttributeRec", "Name" );
                           strcat_s( szMsg, sizeof( szMsg ), szTempName );

                           MessageSend( vSubtask, "ZO00216", "LOD Constraints",
                                       szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );

                        }
#endif
                     }
                  }
               }
            }

            ResetViewFromSubobject( vLOD );
         }

         strcpy_s( szAttribList, sizeof( szAttribList ), "" );
         nMissingNr = 0;

         // Create a formatted String with the SeuqencePriorities and the AttributeNames:
         for ( i = 1; i <= lMaxPriority; i++ )
         {
            zltoa( i, szAktPriority, sizeof( szAktPriority ) );
            strcat_s( szAttribList, sizeof( szAttribList ), szAktPriority );
            strcat_s( szAttribList, sizeof( szAttribList ), " \t" );

            if ( pchAttribList[ i ] != 0 )
               strcat_s( szAttribList, sizeof( szAttribList ), pchAttribList[ i ] );
            else
            {
               strcat_s( szAttribList, sizeof( szAttribList ), "*** missing ***" );
               nMissingNr = i;
            }
            strcat_s( szAttribList, sizeof( szAttribList ), " \n" );
         }

         strcpy_s( szMsg, sizeof( szMsg ), "\nEntity: \t" );
         strcat_s( szMsg, sizeof( szMsg ), szEntityName );
         strcat_s( szMsg, sizeof( szMsg ), "\n" );
         if ( lMaxPriority != 0 )
         {
            strcat_s( szMsg, sizeof( szMsg ), "\n\nPriority\tAttribute\n" );
            strcat_s( szMsg, sizeof( szMsg ),     "------------\t--------------\n" );
            strcat_s( szMsg, sizeof( szMsg ), szAttribList );
         }

#if 0
              *** Deleted following code 19.01.1998 because it isn't necessary
                  any longer and sometimes generates erroneous message.
         // If there is a number in between missing, give error, but only
         // if the entity is a root or the MaxCard is not 1.
         if ( nMissingNr != 0 &&
              ( CompareAttributeToInteger( vLOD, "LOD_EntityParent", "IndentLvl",
                                         0 ) == 0 ||
                CompareAttributeToString( vLOD, "ER_RelLinkRec", "CardMax",
                                          "1" ) != 0 ) )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "The SequencePriority chain is broken. " );
            strcat_s( szMsg, sizeof( szMsg ), "\nPlease change the SequencePriorities of the Attributes of this Entity." );

            strcat_s( szMsg, sizeof( szMsg ), "\n\nEntity: \t" );
            strcat_s( szMsg, sizeof( szMsg ), szEntityName );
            strcat_s( szMsg, sizeof( szMsg ), "\n\nPriority\tAttribute\n" );
            strcat_s( szMsg, sizeof( szMsg ),     "------------\t--------------\n" );
            strcat_s( szMsg, sizeof( szMsg ), szAttribList );

            strcat_s( szMsg, sizeof( szMsg ), "\n\n------------------------------------------\nMissing Priority value: " );
            zltoa( nMissingNr, szAktPriority, sizeof( szAktPriority ) );
            strcat_s( szMsg, sizeof( szMsg ), szAktPriority );

            MessageSend( vSubtask, "ZO00208", "LOD Constraints",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );

            // if the save of the LOD with the invalid PrioritySequence
            // should be permitted, then activate the following lines:
            //return( -1 );
         }
#endif

         if ( CheckExistenceOfEntity( vLOD, "LOD_EntityParent" ) >= zCURSOR_SET )
         {
            // Process next level recursively.

            SetViewToSubobject( vLOD, "LOD_EntityChild" );

            oTZZOLODO_ConstraintRecurs2( vSubtask, vLOD, vERD, szLOD_Name );

            ResetViewFromSubobject( vLOD );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_Constraint
//
// PURPOSE:   This constraint handles ACTIVATE and ACTIVATEEMPTY events to
//            create a work object that has all operations organized by type.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_Constraint( zVIEW   vSubtask,
                      zVIEW   vLOD,
                      zSHORT  nEvent,
                      zSHORT  nState )  // zOCE_ACTIVATE              1
                                        // zOCE_ACTIVATE_EMPTY        2
                                        // zOCE_COMMIT                3
                                        // zOCE_DROPOI                4
{
   zVIEW  vCM_List;
   zVIEW  vERD;
   zCHAR  szLOD_Name[ 33 ];
   zSHORT nRC;

   // Process the event.
   switch( nEvent )
   {
      case zOCE_COMMIT:

         // Don't execute the code below if there is no root entity instance,
         // which means the LOD has been deleted.
         if ( CheckExistenceOfEntity( vLOD, "LOD" ) >= zCURSOR_SET )
         {
            // Only execute the following constraint if we are inside a Tool,
            // not in System Administration.  We will know this by the existence
            // of the TaskLPLR named view.
            if ( GetViewByName( &vCM_List, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
            {
               nRC = RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_ERD_META );

               // look whether the ERD is there ...
               if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
               {
                  ActivateMetaOI( vSubtask, &vERD, vCM_List,
                                  zREFER_ERD_META, zSINGLE | zLEVEL_APPLICATION );

                  // Call the recursive routine that will process all LOD Entities to verify that
                  // any creatable entities have children for all ER Entities with min cardinality > 0.
                  GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ), vLOD, "LOD", "Name" );

                  nRC = oTZZOLODO_ConstraintRecurs1( vSubtask, vLOD, vERD, szLOD_Name );
                  if ( nRC < 0 )
                     return( nRC );

                  nRC = oTZZOLODO_ConstraintRecurs2( vSubtask, vLOD, vERD, szLOD_Name );
                  if ( nRC < 0 )
                     return( nRC );
               }
            }
         }

         break;

      case zOCE_DROPOI:
         break;

      case zOCE_ACTIVATE:
         break;

      case zOCE_ACTIVATE_EMPTY:
         break;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_ConstraintOperACC
//
// PURPOSE:   This constraint handles ACCEPT of an Operation Entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_ConstraintOperACC( zVIEW   vSubtask,
                             zVIEW   vIn,
                             zPCHAR  pchEntityName,
                             zSHORT  nEvent,
                             zSHORT  nState )
{
   zVIEW  vMetaView;
   zCHAR  szOperName[ 33 ];

   // process the event
   switch( nEvent )
   {
      case zECE_CANCEL:
      case zECE_CREATE:
      case zECE_DELETE:
      case zECE_EXCLUDE:
      case zECE_INCLUDE:
         break;

      case zECE_ACCEPT:

         // If the source file is of type VML and the object for this operation
         // is a LOD (ie., not Dialog, etc.), then make sure that the Operation
         // If the source file is of type VML, then make sure that the Operation
         // Name is not greater than 22 characters.  This is because the
         // generated C will add a prefix to the name of oXXXXXXXX_ where
         // XXXXXXXX is the LOD name.  The prefix is necessary to make Operation
         // Names unique since two different LODs can each have the same
         // Operation Name at the VML level.

         // We will know vIn is a LOD if there is a named view "TZZOLODO".

         if ( GetViewByName( &vMetaView, "TZZOLODO", vSubtask, zLEVEL_TASK ) >= 0 &&
              CompareAttributeToString( vIn, "SourceFile", "LanguageType", "V" ) == 0 )
         {
            GetStringFromAttribute( szOperName, sizeof( szOperName ), vIn, "Operation", "Name" );
            if ( zstrlen( szOperName ) > 22 )
            {
               MessageSend( vSubtask, "ZO00217", "Operations",
                           "The Operation Name for a VML operation cannot be greater than 22 characters.",
                           zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               return( -1 );
            }
         }

         break;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_DerivedGenName
//
// PURPOSE:   The Operation Name for a derived operation in VML is the
//            concatenation of the LOD name and the original
//            operation name.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_DerivedGenName( zVIEW  LODView,
                          LPVIEWENTITY EntityStructure,
                          LPVIEWATTRIB AttributeStructure,
                          zSHORT GetOrSetFlag )
{
   zVIEW  vTemp;
   zCHAR  szTempName1[ 64 ];
   zCHAR  szTempName2[ 32 ];
   zCHAR  szType[ 4 ];

   GetStringFromAttribute( szType, sizeof( szType ), LODView, "SourceFileForDARec", "Extension" );
   if ( szType[ 0 ] == 'V' )
   {
      szTempName1[ 0 ] = 'o';
      CreateViewFromViewForTask( &vTemp, LODView, 0 );
      ResetView( vTemp );
      GetStringFromAttribute( szTempName1 + 1, sizeof( szTempName1 ) - 1, vTemp, "LOD", "Name" );
      DropView( vTemp );
      strcat_s( szTempName1, sizeof( szTempName1 ), "_" );
      GetStringFromAttribute( szTempName2, sizeof( szTempName2 ), LODView,
                              "LOD_AttrDerivationOperRec", "Name" );
      strcat_s( szTempName1, sizeof( szTempName1 ), szTempName2 );
   }
   else
   {
      GetStringFromAttribute( szTempName1, sizeof( szTempName1 ), LODView, "LOD_AttrDerivationOperRec", "Name" );
   }

   StoreValueInRecord( LODView, EntityStructure, AttributeStructure, szTempName1, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_OCOPERGenName
//
// PURPOSE:   The Operation Name for an object constraint in VML is the
//            concatenation of the LOD name and the original
//            operation name.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_OCOPERGenName( zVIEW  LODView,
                         LPVIEWENTITY EntityStructure,
                         LPVIEWATTRIB AttributeStructure,
                         zSHORT GetOrSetFlag )
{
   zVIEW  vTemp;
   zCHAR  szTempName1[ 64 ];
   zCHAR  szTempName2[ 32 ];
   zCHAR  szType[ 4 ];

   GetStringFromAttribute( szType, sizeof( szType ), LODView, "SourceFileForOCOper", "Extension" );
   if ( szType[ 0 ] == 'V' )
   {
      szTempName1[ 0 ] = 'o';
      CreateViewFromViewForTask( &vTemp, LODView, 0 );
      ResetView( vTemp );
      GetStringFromAttribute( szTempName1 + 1, sizeof( szTempName1 ) - 1, vTemp, "LOD", "Name" );
      DropView( vTemp );
      strcat_s( szTempName1, sizeof( szTempName1 ), "_" );
      GetStringFromAttribute( szTempName2, sizeof( szTempName2 ), LODView, "LOD_ConstraintOper", "Name" );
      strcat_s( szTempName1, sizeof( szTempName1 ), szTempName2 );
   }
   else
      GetStringFromAttribute( szTempName1, sizeof( szTempName1 ), LODView, "LOD_ConstraintOper", "Name" );


   StoreValueInRecord( LODView, EntityStructure, AttributeStructure, szTempName1, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZZOLODO_ECOPERGenName
//
// PURPOSE:   The Operation Name for an entity constraint in VML is the
//            concatenation of the LOD name and the original
//            operation name.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_ECOPERGenName( zVIEW  LODView,
                          LPVIEWENTITY EntityStructure,
                          LPVIEWATTRIB AttributeStructure,
                          zSHORT GetOrSetFlag )
{
   zVIEW  vTemp;
   zCHAR  szTempName1[ 64 ];
   zCHAR  szTempName2[ 32 ];
   zCHAR  szType[ 4 ];

   GetStringFromAttribute( szType, sizeof( szType ), LODView, "SourceFileForECOperRec", "Extension" );
   if ( szType[ 0 ] == 'V' )
   {
      szTempName1[ 0 ] = 'o';
      CreateViewFromViewForTask( &vTemp, LODView, 0 );
      ResetView( vTemp );
      GetStringFromAttribute( szTempName1 + 1, sizeof( szTempName1 ) - 1, vTemp, "LOD", "Name" );
      DropView( vTemp );
      strcat_s( szTempName1, sizeof( szTempName1 ), "_" );
      GetStringFromAttribute( szTempName2, sizeof( szTempName2 ), LODView, "LOD_EntityConstraintOperRec", "Name" );
      strcat_s( szTempName1, sizeof( szTempName1 ), szTempName2 );
   }
   else
      GetStringFromAttribute( szTempName1, sizeof( szTempName1 ), LODView, "LOD_EntityConstraintOperRec", "Name" );


   StoreValueInRecord( LODView, EntityStructure, AttributeStructure, szTempName1, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT  /* DERIVED ATTRIBUTE */  OPERATION
oTZZOLODO_OperationGenName( zVIEW        LODView,
                            LPVIEWENTITY EntityStructure,
                            LPVIEWATTRIB AttributeStructure,
                            zSHORT       GetOrSetFlag )
{
   zVIEW  vTemp;
   zCHAR  szTempName1[ 64 ];
   zCHAR  szTempName2[ 32 ];
   zCHAR  szType[ 4 ];

   GetStringFromAttribute( szType, sizeof( szType ), LODView, "SourceFile", "Extension" );
   if ( szType[ 0 ] == 'V' )
   {
      szTempName1[ 0 ] = 'o';
      CreateViewFromViewForTask( &vTemp, LODView, 0 );
      ResetView( vTemp );
      GetStringFromAttribute( szTempName1 + 1, sizeof( szTempName1 ) - 1, vTemp, "LOD", "Name" );
      DropView( vTemp );
      strcat_s( szTempName1, sizeof( szTempName1 ), "_" );
      GetStringFromAttribute( szTempName2, sizeof( szTempName2 ), LODView, "Operation", "Name" );
      strcat_s( szTempName1, sizeof( szTempName1 ), szTempName2 );
   }
   else
      GetStringFromAttribute( szTempName1, sizeof( szTempName1 ), LODView, "Operation", "Name" );


   StoreValueInRecord( LODView, EntityStructure, AttributeStructure, szTempName1, 0 );

   return( 0 );
}

static void
ofnTZZOLODO_DeleteAllPOD_Entities( zVIEW vTZZOLODO )
{
   zSHORT nRC;

   for ( nRC = SetCursorFirstEntity( vTZZOLODO, "POD_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZZOLODO, "POD_Entity", "" ) )
   {
      DeleteEntity( vTZZOLODO, "POD_Entity", zREPOS_NONE );
   }

   nRC = CheckExistenceOfEntity ( vTZZOLODO, "LOD_EntityChild" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetViewToSubobject( vTZZOLODO, "LOD_EntityChild" );
      ofnTZZOLODO_DeleteAllPOD_Entities( vTZZOLODO );

      nRC = ResetViewFromSubobject( vTZZOLODO );
      nRC = SetCursorNextEntity( vTZZOLODO, "LOD_EntityChild", "" );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: oTZZOLODO_DeleteAllPOD_Entities
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*TRANSFORMATION */  OPERATION
oTZZOLODO_DeleteAllPOD_Entities( zVIEW vTZZOLODO )
{
   zVIEW  vLOD;

   if ( vTZZOLODO == 0 )
      return( -1 );

   CreateViewFromViewForTask( &vLOD, vTZZOLODO, 0 );

   ResetView( vLOD );
   ofnTZZOLODO_DeleteAllPOD_Entities( vLOD );

   DropView( vLOD );
   return( 0 );
} // oTZZOLODO_DeleteAllPOD_Entities


static void
ofnTZZOLODO_DeleteOldPOD_Entities( zVIEW vTZZOLODO, zVIEW vTZTENVRO )
{
   zSHORT  nRC;
   zLONG   lZKey;

   for ( nRC = SetCursorFirstEntity( vTZZOLODO, "POD_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZZOLODO, "POD_Entity", "" ) )
   {
      GetIntegerFromAttribute( &lZKey, vTZZOLODO, "TE_DBMS_SourceForEntity", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vTZTENVRO, "TE_DBMS_Source", "ZKey", lZKey, "" ) < zCURSOR_SET )
      {
         DeleteEntity( vTZZOLODO, "POD_Entity", zREPOS_NONE );
      }
   }

   nRC = CheckExistenceOfEntity ( vTZZOLODO, "LOD_EntityChild" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetViewToSubobject( vTZZOLODO, "LOD_EntityChild" );
      ofnTZZOLODO_DeleteOldPOD_Entities( vTZZOLODO, vTZTENVRO );

      nRC = ResetViewFromSubobject( vTZZOLODO );
      nRC = SetCursorNextEntity( vTZZOLODO, "LOD_EntityChild", "" );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: oTZZOLODO_DeleteOldPOD_Entities
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*TRANSFORMATION */  OPERATION
oTZZOLODO_DeleteOldPOD_Entities( zVIEW vTZZOLODO,
                                 zVIEW vTZTENVRO )
{
   zVIEW  vLOD;
   zVIEW  vDTE;

   if ( vTZZOLODO == 0 || vTZTENVRO == 0 )
      return( -1 );

   CreateViewFromViewForTask( &vLOD, vTZZOLODO, 0 );
   CreateViewFromViewForTask( &vDTE, vTZTENVRO, 0 );

   ResetView( vLOD );

   if ( CheckExistenceOfEntity( vDTE, "TE_DBMS_Source" ) < zCURSOR_SET )
      oTZZOLODO_DeleteAllPOD_Entities( vLOD );
   else
      ofnTZZOLODO_DeleteOldPOD_Entities( vLOD, vDTE );

   DropView( vLOD );
   DropView( vDTE );

   return( 0 );
} // oTZZOLODO_DeleteOldPOD_Entities

zSHORT LOCALOPER
fnRetrieveNextToken( zPPCHAR        ppchInput,
                     zPCHAR         pchDefaultEntityName,
                     zPCHAR         pchEntityName,
                     zPCHAR         pchToken,
                     zPCHAR         pchContextName )
{
   zSHORT n;
   zPCHAR pchExpr = *ppchInput;

   *pchEntityName  = 0;
   *pchContextName = 0;

   // Get the token.
   for ( n = 0; zisalnum( *pchExpr ); pchExpr++ )
      pchToken[ n++ ] = *pchExpr;
   pchToken[ n ] = 0;

   // If the next zCHAR is a period then the the token is an entity name followed
   // by an attribute name.
   if ( *pchExpr == '.' )
   {
      strcpy_s( pchEntityName, 33, pchToken );

      pchExpr++;   // Skip the '.'

      // Get the attr name.
      for ( n = 0; zisalnum( *pchExpr ); pchExpr++ )
         pchToken[ n++ ] = *pchExpr;
      pchToken[ n ] = 0;
   }

   // Check to see if the character immediately following the attribute name
   // is a [ to indicate that a context is to be used for the get of the attribute.
   if ( *pchExpr == '[' )
   {
      pchExpr++;

      // Skip whitespaces.
      while ( zisspace( *pchExpr ) )
         pchExpr++;

      // Retrieve the context name.  We allow any non-space characters to
      // be part of the context name.
      for ( n = 0;
            *pchExpr && !zisspace( *pchExpr ) && *pchExpr != ']';
            pchExpr++ )
      {
         pchContextName[ n++ ] = *pchExpr;
      }
      pchContextName[ n ] = 0;

      // Skip whitespaces.
      while ( zisspace( *pchExpr ) )
         pchExpr++;

      if ( *pchExpr == ']' )
         pchExpr++;
      else
         return( zCALL_ERROR );
   }
   else
     *pchContextName = 0;  // No context.

   if ( *pchEntityName == 0 )
      strcpy_s( pchEntityName, 33, pchDefaultEntityName );

   *ppchInput = pchExpr;

   return( 0 );
}

typedef zLONG (POPERATION PFNPARSER)
           ( zVIEW  lpVSSubtask,
             zVIEW  vInSourceMeta,
             zLONG  lIlParseAction,
             zLONG  lIlParseLimit,
             zLONG  lIlParseSource,
             zPCHAR szSourceFileName,
             zPCHAR szInOperationName,
             zLONG  lMetaType,
             zLONG  lControl );

#define TOKEN_INQUOTE   1
#define TOKEN_IDENT     2
#define TOKEN_CMD       3

#define CMD_SUM         1


/////////////////////////////////////////////////////////////////////////////
//
// Generate a temporary VML file from the derived expression, parse it, and
// store the resulting XPG in the XOD.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT LOCALOPER
fnParseDerivedExpression( zVIEW vTZZOLODO )
{
   zLONG  f = 0;
   zCHAR  szVML[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szErrFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szSourceDirectory[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szEntityName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  szAttribName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  szContextName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  szLine[ 5000 ];
// zPCHAR pchName;
   zPCHAR pchType;
   zPCHAR pchExpr;
   zPCHAR pch;
   zPCHAR pchSourceEntityName;
   zPCHAR pchSourceAttribName;
   zLONG  lRC;
   zLONG  lLth;
   zSHORT nReturn = zCALL_ERROR;
   zLONG  idx;
   zVIEW  vXPG;
   zVIEW  vTaskLPLR;
   zVIEW  vTZZOLODO_Temp;
   zVIEW  vSubtask = 0;
   zBOOL  bUseXPG;
   zSHORT nExpectedToken;
   zSHORT nArgCount;
   zCHAR  szFuncName[ 500 ];

   static PFNPARSER pfnParser = 0;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szSourceDirectory, sizeof( szSourceDirectory ), vTaskLPLR, "LPLR", "PgmSrcDir" );

   // Create a unique temp VML name using the task ID.
   sprintf_s( szVML, sizeof( szVML ), "zDE%lx", SysGetTaskFromView( GetDefaultViewForActiveTask( ) ) );
          // SysGetTaskFromView( vTZZOLODO ) );
   sprintf_s( szFileName, sizeof( szFileName ), "%s\\%s.VML", szSourceDirectory, szVML );
   sprintf_s( szErrFileName, sizeof( szFileName ), "%s\\%s.ERR", szSourceDirectory, szVML );

   // Set the error attributes to null.
   SetAttributeFromString( vTZZOLODO, "ER_Attribute", "GeneratedVML", "" );
   SetAttributeFromString( vTZZOLODO, "ER_Attribute", "ParseErrorTxt", "" );

   // Generate the temp VML file.
   f = SysOpenFile( vTZZOLODO, szFileName, COREFILE_WRITE );
   if ( f == -1 )
      return( zCALL_ERROR );

   // Create a copy of the LOD so we can change the cursors without impacting
   // anything.
   CreateViewFromViewForTask( &vTZZOLODO_Temp, vTZZOLODO, 0 );

   SysWriteLine( vTZZOLODO, f, "GLOBAL OPERATION" );
   SysWriteLine( vTZZOLODO, f, "DerivedFunction( VIEW          vSubtask," );
   SysWriteLine( vTZZOLODO, f, "                 VIEW          vView )" );

   // Get the data type of the attribute.
   GetAddrForAttribute( &pchType, vTZZOLODO, "Domain", "DataType" );
   switch ( *pchType )
   {
      case zTYPE_INTEGER:
         SysWriteLine( vTZZOLODO, f, "   INTEGER value" );
         break;

      case zTYPE_DECIMAL:
         SysWriteLine( vTZZOLODO, f, "   DECIMAL value" );
         break;

      default:
         if ( *pchType == zTYPE_DATETIME )
            lLth = 30;
         else
            GetIntegerFromAttribute( &lLth, vTZZOLODO, "ER_Attribute", "Lth" );

         sprintf_s( szLine, sizeof( szLine ), "   STRING( %ld ) value", lLth );
         SysWriteLine( vTZZOLODO, f, szLine );
         break;
   }

   strcpy_s( szLine, sizeof( szLine ), "   value = " );
   GetAddrForAttribute( &pchExpr, vTZZOLODO, "ER_Attribute", "DerivedExpression" );
   GetAddrForAttribute( &pchSourceEntityName, vTZZOLODO, "LOD_Entity", "Name" );
   GetAddrForAttribute( &pchSourceAttribName, vTZZOLODO, "ER_Attribute", "Name" );

   // We only need to use the generated VML/XPG if the expression is more complex
   // then e.a + e.a +...  As we go through the derived expression we'll keep
   // track of anything more complex than that.
   bUseXPG = FALSE;
   for ( pch = pchExpr; *pch; pch++ )
   {
      // If the zCHAR is a symbol more complex than + or - then we need to use the VML.
      if ( strchr( "/*().", *pch ) > 0 )
         bUseXPG = TRUE;
   }

   // Initialize the token tracker.
   nExpectedToken = TOKEN_IDENT;

   // Loop through the expression and copy values to the concat buffer.
   // Anything in quotes is copied verbatim.  Anything not in quotes had
   // better be an attribute name or + signs.
   // 'idx' points to the null-terminator in szConcatBuffer.
   for ( idx = zstrlen( szLine ); *pchExpr; )
   {
      // Check to see if we are inside a quoted string.  If so we just copy
      // the character from the expression to the buffer.
      if ( nExpectedToken == TOKEN_INQUOTE )
      {
         if ( *pchExpr == '\"' )
         {
            nExpectedToken = TOKEN_IDENT;

            // We'll terminate the buffer here.
            szLine[ idx ] = 0;
         }

         // Copy the quote.
         szLine[ idx++ ] = *pchExpr++;
         continue;
      }

      // OK, we're not in a quote or context name.  Check to see if we are starting a quoted section.
      if ( *pchExpr == '\"' )
      {
         szLine[ idx++ ] = *pchExpr++;
         nExpectedToken = TOKEN_INQUOTE;
         continue;
      }

      // Check for built-in commands.
      if ( *pchExpr == '@' )
      {
         pchExpr++;        // Skip the '@'.
         nExpectedToken = TOKEN_CMD;

         bUseXPG = TRUE;

         if ( fnRetrieveNextToken( &pchExpr, pchSourceEntityName,
                                   szEntityName, szFuncName, szContextName ) < 0 )
            goto EndOfFunction;

         _strupr_s( szFuncName, sizeof( szFuncName ) );

         // =================
         // do some error checking here.
         // szEntityName and szContextName should be "".
         // =================

         nArgCount = 0;

         // Add the function call.
         switch ( *pchType )
         {
            case zTYPE_INTEGER:
               sprintf_s( szLine + idx, sizeof( szLine ) - idx, "zi%s( vView, ", szFuncName );
               break;

            case zTYPE_DECIMAL:
               sprintf_s( szLine + idx, sizeof( szLine ) - idx, "zd%s( vView, ", szFuncName );
               break;
         }

         idx += zstrlen( szLine + idx );

         // Skip whitespaces and the open paren.
         while ( isspace( *pchExpr ) )
            pchExpr++;

         if ( *pchExpr != '(' )
         {
            zCHAR szMsg[ 5000 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Error parsing derived VML for '%s.%s'. "
                       "'(' expected after @%s command.", pchSourceEntityName,
                       pchSourceAttribName, szFuncName );

            SetAttributeFromString( vTZZOLODO, "ER_Attribute", "ParseErrorTxt", szMsg );

            MessageSend( vTZZOLODO, "ZO00317", "Operations",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

            goto EndOfFunction;
         }

         pchExpr++;

         while ( isspace( *pchExpr ) )
            pchExpr++;

         continue;
      }

      // If the symbol is not alpha we'll just copy it and move on.
      if ( !isalpha( *pchExpr ) )
      {
         // If we find a closing paren we are no longer in a CMD.
         if ( *pch == ')' && nExpectedToken == TOKEN_CMD )
         {
            nExpectedToken = TOKEN_IDENT;

            if ( nArgCount < 2 || nArgCount > 3 )
            {
               zCHAR szMsg[ 5000 ];

               sprintf_s( szMsg, sizeof( szMsg ), "Error parsing derived VML for '%s.%s'. "
                          "Incorrect number of arguments specified for %s command.",
                          pchSourceEntityName, pchSourceAttribName, szFuncName );

               SetAttributeFromString( vTZZOLODO, "ER_Attribute", "ParseErrorTxt", szMsg );

               MessageSend( vTZZOLODO, "ZO00317", "Operations",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

               goto EndOfFunction;
            }

            // If only 2 arguments where specified, then we need to add the default scoping entity.
            if ( nArgCount == 2 )
            {
               strcat_s( szLine, sizeof( szLine ), ", 0 " );
               idx += zstrlen( szLine + idx );
            }
         }

         szLine[ idx++ ] = *pchExpr++;
         continue;
      }

      // At this point pchExpr had better be pointing to an attribute name.
      // Retrieve the name.
      if ( fnRetrieveNextToken( &pchExpr, pchSourceEntityName,
                                szEntityName, szAttribName, szContextName ) < 0 )
         goto EndOfFunction;

      // The simple interpreter can't handle entity names.  If one was specified then use the XPG.
      if ( zstrcmp( szEntityName, pchSourceEntityName ) != 0 )
         bUseXPG = TRUE;

      // Verify that the Entity name and attribute name are valid.
      if ( !bUseXPG &&
           SetCursorFirstEntityByString( vTZZOLODO_Temp, "LOD_Entity", "Name",
                                         szEntityName, 0 ) < zCURSOR_SET )
      {
         zCHAR szMsg[ 5000 ];

         sprintf_s( szMsg, sizeof( szMsg ), "Error parsing derived VML for '%s.%s'. "
                    "Entity '%s' does not exist.", pchSourceEntityName,
                    pchSourceAttribName, szEntityName );

         SetAttributeFromString( vTZZOLODO, "ER_Attribute", "ParseErrorTxt", szMsg );

         MessageSend( vTZZOLODO, "ZO00317", "Operations",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

         goto EndOfFunction;
      }

      if ( !bUseXPG &&
           SetCursorFirstEntityByString( vTZZOLODO_Temp, "ER_Attribute", "Name",
                                         szAttribName, "LOD_Entity" ) < zCURSOR_SET )
      {
         zCHAR szMsg[ 5000 ];

         sprintf_s( szMsg, sizeof( szMsg ), "Error parsing derived VML for '%s.%s'. "
                    "Attrib '%s' does not exist.", pchSourceEntityName,
                    pchSourceAttribName, szAttribName );

         SetAttributeFromString( vTZZOLODO, "ER_Attribute", "ParseErrorTxt", szMsg );

         MessageSend( vTZZOLODO, "ZO00318", "Operations",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

         goto EndOfFunction;
      }

      // Find the next non-space.
      for ( pch = pchExpr; isspace( *pch ); pch++ )
         ;

      // If the next zCHAR is an open paren, then szAttribName is really a
      // function name.
      if ( *pch == '(' )
         strcpy_s( szLine + idx, sizeof( szLine ) - idx, szAttribName );
      else
      if ( nExpectedToken == TOKEN_CMD )
      {
         // Add the argument to the CMD.
         sprintf_s( szLine + idx, sizeof( szLine ) - idx, " \"%s\"", szAttribName );
         nArgCount++;
      }
      else
         sprintf_s( szLine + idx, sizeof( szLine ) - idx, "vView.%s.%s", szEntityName, szAttribName );

      idx += zstrlen( szLine + idx );

   } // for ( idx... )

   // If the expression is a simple one we can use the old "interpreter"
   // instead of the VML interpreter.
   if ( !bUseXPG )
   {
      nReturn = 0;

      // Set the XPG attribute to NULL.
      SetAttributeFromBlob( vTZZOLODO, "ER_Attribute", "DerivedExpressionXPG", "", 0 );
      goto EndOfFunction;
   }

   szLine[ idx ] = 0;

   SysWriteLine( vTZZOLODO, f, szLine );

   if ( *pchType == zTYPE_STRING )
      SysWriteLine( vTZZOLODO, f, "   StoreStringInDerivedAttribute( vView, " );
   else
      SysWriteLine( vTZZOLODO, f, "   StoreValueInDerivedAttribute( vView, " );

   sprintf_s( szLine, sizeof( szLine ), "                                  \"%s\",", pchSourceEntityName );
   SysWriteLine( vTZZOLODO, f, szLine );

   sprintf_s( szLine, sizeof( szLine ), "                                  \"%s\",", pchSourceAttribName );
   SysWriteLine( vTZZOLODO, f, szLine );

   if ( *pchType == zTYPE_STRING )
      SysWriteLine( vTZZOLODO, f, "                                   value )" );
   else
      SysWriteLine( vTZZOLODO, f, "                                   value, 0 )" );

   SysWriteLine( vTZZOLODO, f, "END" );

   SysCloseFile( vTZZOLODO, f, 0 );
   f = -1;

   SfCreateSubtask( &vSubtask, vTZZOLODO, "Zeidon_Tools" );

   // Create a temp SourceFile in the LOD
   CreateEntity( vTZZOLODO, "SourceFile", zPOS_LAST );
   SetAttributeFromString( vTZZOLODO, "SourceFile", "Name", szVML );

   // We get ParseSource() function dynmically because of linker problems.
   if ( pfnParser == 0 )
   {
      LPLIBRARY hLibrary = SysLoadLibrary( vSubtask, "TZVMPOPR" );

      if ( hLibrary == 0 )
      {
         SysMessageBox( vSubtask, "Zeidon Interpretor Error",
                        "Could not locate TZVMLOPR.DLL", TRUE );
      }
      else
         pfnParser = (PFNPARSER) SysGetProc( hLibrary, "ParseSource" );
   }

   if ( pfnParser != 0 )
      lRC = (*pfnParser) ( vSubtask, vTZZOLODO, zPARSE_ACTION_FULL,
                           zPARSE_LIMIT_SOURCE, zPARSE_SOURCE_FILE, szVML, "",
                           zREFER_LOD_META, zPARSE_DONOTWRITEXPG );

   GetViewByName( &vXPG, "XPG", vSubtask, zLEVEL_TASK );
   if ( lRC >= 0 )
   {
      SetBlobFromOI( vTZZOLODO, "ER_Attribute", "DerivedExpressionXPG", vXPG, zCOMPRESSED );
      nReturn = 0;
   }
   else
   {
      SetBlobFromFile( vTZZOLODO, "ER_Attribute", "GeneratedVML", szFileName );
      SetBlobFromFile( vTZZOLODO, "ER_Attribute", "ParseErrorTxt", szErrFileName );
      SysMessageBox( vTZZOLODO, "DGC", "Parse failed", 0 );
   }

   if ( vXPG )
      DropView( vXPG );

   GetViewByName( &vXPG, "TZVMEROO", vSubtask, zLEVEL_TASK );
   if ( vXPG )
      DropView( vXPG );

   // Don't need the generated VML anymore so delete it.  Delete the .err file too.
   SysOpenFile( vTZZOLODO, szFileName, COREFILE_DELETE );
   SysOpenFile( vTZZOLODO, szErrFileName, COREFILE_DELETE );

   // The parse caused a SourceFile entity to be created.  Delete it from the
   // LOD.
   SetCursorFirstEntityByString( vTZZOLODO, "SourceFile", "Name", szVML, 0 );
   DeleteEntity( vTZZOLODO, "SourceFile", zREPOS_PREV );

EndOfFunction:
   if ( f != -1 )
      SysCloseFile( vTZZOLODO, f, 0 );

   // Drop the temp copy.
   DropView( vTZZOLODO_Temp );

   if ( vSubtask != 0 )
      SfDropSubtask( vSubtask, 0 );

   return( nReturn );
}

zOPER_EXPORT zSHORT /*TRANSFORMATION */  OPERATION
oTZZOLODO_ParseDerivedExpr( zVIEW vTZZOLODO,
                            zVIEW vSubtask )
{
   if ( fnParseDerivedExpression( vTZZOLODO ) < 0 )
      return( zCALL_ERROR );
   else
      return( 0 );
}
