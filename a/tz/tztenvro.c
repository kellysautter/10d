/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tztenvro.c -Zeidon application operations
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
// LAST MESSAGE ID: TE00117
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2003.04.23  DonC
   Modified oTZTENVRO_BuildTablRecsFromEMD so that GenerateUniqueName is not
   called when the TE_TableName is specified in the ER.
2001.06.07  BL  R55575
   Modified oTZTENVRO_BuildSQL_RelsFromEMD: check Cursor position for set value
2001.02.05  HH
   Move foreign key switching algorithm here.
2000.11.01  BL
   Modified oTZTENVRO_BuildTablRecsFromEMD: If Data Source a SAP Data Source
   and Entity not a SAP Entity, ignore this Entity
2000.10.27  BL
   New Error Work LOD structure for SAP Import
2000.10.10  BL
   New Handling for Generate TE Names
2000.08.22  RG  Z2000
   For Init Tables/Records: if the column name is embedded in quotation marks,
   delete them for DB2, otherwise we get runtime errors
2000.08.07  BL  TB
   Added oTZTENVRO_WriteErrorMsgToList for write all Error Messages to
   Error List Window, do not send a Message for an Error ( Bug TB 50831 )
   Set in window <Table/Record> Key Flag for Listbox <Column/Field> for Bug
   TB 51244
   Modified oTZTENVRO_BuildTablRecsFromEMD: if Data Source = NONE, return
2000.07.31  RG  Z2000
   Foreign key names will always be generated in upper case
2000.07.31  RG  Z2000
   While generating a name for many to many relation, change the entity-names
   to upper-case, because they aren't changed while initializing tables
2000.07.09  BL
   Replace Technical Environment through Physical Data Model
2000.06.09  BL   Z2000  TB 51059
   Modified oTZTENVRO_BuildSQL_RelsFromEMD
2000.05.22  BL   Z2000
   Modified oTZTENVRO_BuildSQL_RelsFromEMD: Fix for Entity Cursor is Null for
   menu item Build/Rebuild  Tables/Rels... If more than one DataSources exists
   and the message box will be answered with "YES"
2000.03.27  RG   Z2000
    Fix: If you called 'Init Tables/Records' for Siron catalogs after
         'Synchronize RelationShips' you got the Entity many times in the TE
2000.02.22  HH
    fix autoseq generation if relationship not in TE.
2000.02.02  RG   Z2000
    Set bModifyExistingTables in Loop for each table. It's needed, that we
    can add new tables including the identifier informations whithout to
    delete all tables from the datasource.
1999.09.03  DGC  10a
   Fixed bug reported by Doug--we weren't properly taking into account the
   DBH-specific OI when determining if the FKs should be generated with
   table names.

1999.09.03  RG Z9J, Z2000 VSAM
    'Init Tables': Update Tables for VSAM fixed

1999.08.10  RG Z9J,Z2000 VSAM
   In function oTZTENVRO_BuildTablRecsFromEMD flag bModifyExistingTables is
   always TRUE for Siron Catalogs because we have to execute Identifier
   processing for Siron Catalogs always

1999.08.05  RG Z9J,Z2000 VSAM
   Generate unique Tablename only for databases not for Siron Flat Files

18.02.97    DonC
   Corrected RecursiveRelSearch and ImplementSQL_RelLnk to generate TE
   correctly for a chain of Attributive entities.

21.02.97    DonC
   Corrected the generation of tables for a second DBMS when it is requested
   that only entities not in any DBMS be used to generate tables.

05.05.97    DonC
   Added the generation of AUTOSEQ fields in the TE when requested on
   ER_RelLink entities.

22.08.97    DonC
   Added setting of SQL_TableOwner in TE Table from TE_TableOwner in
   ER_Entity of ER.

16.10.1997  DonC
   Modified generation of foreign key names to use TE_ColumnName or
   Attribute Name if the Suffix name doesn't exist.

28.01.1998  DonC
   Modified oTZTENVRO_BuildTablRecsFromEMD so that a table resync into a
   DBMS_Source that has existing table entries will only add table and
   attribute entries, without processing identifiers.

25.03.1998  DonC
   Modified ofnTZTENVRO_ImplementSQL_RelLink to correct foreign key suffix
   handling for generated keys. Also improved error message TE00117.

15.07.1998  DonC
   Modified oTZTENVRO_BuildSQL_RelsFromEMD to order key entries made up
   of foreign keys.

14.12.1998  HH
   Not change a colum name, if it was entered in the ER(TE_ColumnName).

1998.03.09  DGC
   Changed how FK names were generated (added record name in some cases).

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"

//#define  TRACE_LINES 1
//#define  DEBUG_VIEWS 1

zOPER_EXPORT zSHORT OPERATION
CreateTE_MetaEntity( zVIEW  vSubtask,
                  zVIEW  lpView,
                  zPCHAR szEntityName,
                  zSHORT nPosition );

zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_ImplementSQL_RelLink( zVIEW    vSubtask,
                                  zVIEW    vDTE_c,
                                  zVIEW    vEMD,
                                  zVIEW    vDTE_p,
                                  zVIEW    vEMD_p,
                                  PFNGENERATENAME pfnGenerateName );

zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_ImplementNET_RelLink( zVIEW    vSubtask,
                                  zVIEW    vDTE_c,
                                  zVIEW    vEMD,
                                  zVIEW    vDTE_p,
                                  zVIEW    vEMD_p,
                                  PFNGENERATENAME pfnGenerateName );

zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_WriteErrorMsgToList( zVIEW  vSubtask,
                               zPCHAR szEntityName,
                               zPCHAR szErrorMsg );

zSHORT LOCALOPER
TranslateToUnderscoreCase( zPCHAR pchSrc, zPCHAR pchTarget, zCHAR  cMetaType );



static zSHORT
fnUpdateKeys( zVIEW  vDTE, zVIEW  vDTE_Copy1 );
static void
fnReferenceBaseKey( zVIEW vDTE );
static zSHORT
fnFK_SearchBaseKey( zVIEW  vDTE_BaseKey, zLONG  lZKey );

zCHAR sz[ 512 ];
#if DEBUG_VIEWS
zCHAR szDebug[ 512 ];
#endif

#define zIMPL_REL_BYPASS         0
#define zIMPL_REL_AsFkInSrc      1
#define zIMPL_REL_AsFkInTgt      2
#define zIMPL_REL_AsTablRec      3

#define zREL_IMPLD_NOT           0
#define zREL_IMPLD_AsFkInSrc     1
#define zREL_IMPLD_AsFkInTgt     2
#define zREL_IMPLD_AsTablRec     3


#define BUFF_SIZE 256
#define MAX_TABLENAME_LTH  32
#define MAX_COLUMNNAME_LTH       32



/*
   The basic algorithm is:
      1. Determine the TE_TablRec "key" from the choice
         of ER_EntityIdentifiers
            - 1st choice is "system maintained" with no relationships
            - 2nd choice is minimal attr's with no relationships
            - 3rd choice is "system maintained" with a relationship to
               an entity that has an identifier with no relationships
            - 4th choice is minimal attr's with a relationship to
               an entity that has an identifier with no relationships
            - 5th choice is minimal number of attr's and rel's
      1. Determine the TE_TablRec for the "parent" ER_Entity
      2. Add TE_FieldDataRel for each "key" in the "parent" table using a
         name that identifies the "relationship"
*/
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_BuildTablRecsFromEMD( zVIEW    vSubtask,
                                zVIEW    vDTE,
                                zVIEW    vEntpModel,
                                zPCHAR   szScopingEntityTE )
{
   zVIEW     vEMD;
   zVIEW     vDTE_save;
   zVIEW     vDTE3;
   zCHAR     szName[ 255 ];
   zSHORT    RESULT;
   zCHAR     szTemp[ 1026 ];
   LPLIBRARY hLibrary;
   zSHORT    (POPERATION pfnSetDataType) ( zVIEW, zBOOL );
   PFNGENERATENAME pfnGenerateName;
   zBOOL     bModifyExistingTables;
   zBOOL     bExists;
   zCHAR     szSironDB_Type[ 2 ];
   zCHAR     szCreateUnderscore[ 2 ];

   GetStringFromAttribute( szName, sizeof( szName ), vDTE, "TE_DBMS_Source", "GenerateExecutable" );

   if ( zstrcmp( szName, "" ) == 0 )
      return( -1 );

   hLibrary = SysLoadLibrary( vDTE, szName );
   if ( !hLibrary )
   {
      MessageSend( vDTE, "TE00101", "Physical Data Model",
                   "Couldn't load Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( zCALL_ERROR );
   }

   pfnGenerateName = (PFNGENERATENAME) SysGetProc( hLibrary, "GenerateName" );
   if ( !pfnGenerateName )
   {
      MessageSend( vDTE, "TE00102", "Physical Data Model",
                   "Couldn't find 'GenerateName' in Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SysFreeLibrary( vDTE, hLibrary );
      return( zCALL_ERROR );
   }

   pfnSetDataType = (zSHORT (__stdcall *)(zVIEW, zBOOL)) SysGetProc( hLibrary, "SetDataType" );
   if ( !pfnSetDataType )
   {
      MessageSend( vDTE, "TE00103", "Physical Data Model",
                   "Couldn't find 'SetDataType' in Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SysFreeLibrary( vDTE, hLibrary );
      return( zCALL_ERROR );
   }

   CreateViewFromViewForTask( &vEMD, vEntpModel, 0 );
   SetNameForView( vEMD, "vEMD_Init", vDTE, zLEVEL_TASK );
   CreateViewFromViewForTask( &vDTE_save, vDTE, 0 );
   SetNameForView( vDTE_save, "vDTE_save_Init", vDTE, zLEVEL_TASK );
   CreateViewFromViewForTask( &vDTE3, vDTE, 0 );
   SetNameForView( vDTE3, "vDTE3_Init", vDTE, zLEVEL_TASK );

   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type" );

   //=======================================================================
   //
   // Create a TE table for each ER entity.
   // Loop through all entities.
   //
   //=======================================================================
   for ( RESULT = SetCursorFirstEntity( vEMD, "ER_Entity", 0 );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vEMD, "ER_Entity", 0 ) )
   {
      // Ignore work entities.
      if ( CompareAttributeToString( vEMD, "ER_Entity", "Purpose", "W" ) == 0 )
         continue;

      // If Data Source a SAP Data Source and Entity not a SAP Entity, ignore
      // this Entity
      if ( *szSironDB_Type == 'S' &&
           CompareAttributeToInteger( vEMD, "ER_Entity", "SAP_Entity", 1 ) != 0 )
         continue;

      // Default: the entity already exists.
      bExists = TRUE;

      // Use SetCursorFirstEntityByAttr because SetCursorFirstEntityByEntityCsr
      // doesn't work in the right way.  2000.03.27  RG
      RESULT = SetCursorFirstEntityByAttr( vDTE3, "ER_Entity", "Name", vEMD, "ER_Entity", "Name", szScopingEntityTE );
      if ( RESULT < zCURSOR_SET )
      {
         // If RESULT = zCURSOR_UNCHANGED, it is possible, that the entity is already there. Check this here.
         if (CheckExistenceOfEntity( vDTE3, "ER_Entity") == zCURSOR_SET)
            RESULT = CompareAttributeToAttribute(vEMD, "ER_Entity", "Name", vDTE3, "ER_Entity", "Name");
         else
            RESULT = 1;

         if ( RESULT == zCALL_ERROR )
            return( zCALL_ERROR );

         if ( RESULT != 0 )
         {
            // Add the table entry, because it isn't there.
            SetCursorLastEntity( vDTE, "TE_TablRec", 0 );
            CreateTE_MetaEntity( vSubtask, vDTE, "TE_TablRec", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vDTE, "ER_Entity", vEMD, "ER_Entity", zPOS_AFTER );

            // If there is a TE Table name, use it.  Otherwise use the ER Entity name.
            GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_Entity", "TE_TableName" );
            if ( szName[ 0 ] == 0 )
            {
               GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_Entity", "Name" );
               GetStringFromAttribute( szCreateUnderscore, sizeof( szCreateUnderscore ), vDTE, "TE_DBMS_Source", "TranslateNamesToLowerUnderscore");
               if ( szCreateUnderscore[ 0 ] == 'Y' )
                  TranslateToUnderscoreCase( szName, szName, 'E' );

               ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE, szName, sizeof( szName ), 'E', "TE_TablRec", "Name", 0, pfnGenerateName );
            }
            else
               SetAttributeFromString( vDTE, "TE_TablRec", "Name", szName );

            SetAttributeFromAttribute( vDTE, "TE_TablRec", "Desc", vEMD, "ER_Entity",  "Desc" );
            SetAttributeFromAttribute( vDTE, "TE_TablRec", "SQL_TableOwner", vEMD, "ER_Entity",  "TE_TableOwner" );
            SetAttributeFromString( vDTE, "TE_TablRec", "DataOrRel", "D" );

            // If the table isn't found in the DBMS_Source we have to process identifiers.
            bModifyExistingTables = FALSE;

            // The entity didn't exist in TE before. It is new.
            bExists = FALSE;
         }
      }

      // If the Entity already exists, we have to set bModifyExistingTables
      if ( bExists )
      {
         GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
         if (*szSironDB_Type == 'F')
         {
             // Identifier processing must always run for Siron Catalogs
             bModifyExistingTables = FALSE;
         }
         else
         {
            // Identifier processing should not run, if only table attributes are added.
            bModifyExistingTables = TRUE;
         }
      }

      // Loop through each of the attributes to make sure they are there.
      // This will be done only if the ER_Entity exists under the current TE_DBMS_Source entity.
      RESULT = SetCursorFirstEntityByEntityCsr( vDTE, "ER_Entity", vEMD, "ER_Entity", "TE_DBMS_Source" );
      if ( RESULT >= zCURSOR_SET )
      {
         for ( RESULT = SetCursorFirstEntity( vEMD, "ER_Attribute", 0 );
               RESULT > zCURSOR_UNCHANGED;
               RESULT = SetCursorNextEntity( vEMD, "ER_Attribute", 0 ) )
         {
            RESULT = SetCursorFirstEntityByEntityCsr( vDTE, "ER_Attribute", vEMD, "ER_Attribute", "TE_TablRec" );
            // Don't worry about it if TE attribute already exists.
            if ( RESULT >= zCURSOR_SET )
               continue;

            // Skip if the ER_Attribute is a Work attribute.
            if ( CompareAttributeToString( vEMD, "ER_Attribute", "Work", "Y" ) == 0 )
               continue;

            CreateTE_MetaEntity( vSubtask, vDTE, "TE_FieldDataRel", zPOS_AFTER );

            // If there is a TE Column name, use it.  Otherwise use the ER Attribute name.
            GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_Attribute", "TE_ColumnName" );
            if ( *szName == 0 )
            {
               GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_Attribute", "Name" );
               GetStringFromAttribute( szCreateUnderscore, sizeof( szCreateUnderscore ), vDTE, "TE_DBMS_Source", "TranslateNamesToLowerUnderscore");
               if ( szCreateUnderscore[ 0 ] == 'Y' )
                  TranslateToUnderscoreCase( szName, szName, 'A' );
               ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE, szName, sizeof( szName ), 'A', "TE_FieldDataRel", "Name", 0, pfnGenerateName );
            }
            else
            {
               // If the column name is embedded in quotation marks, delete them for DB2, otherwise we get runtime errors
               if ( *szName == '\"' )
                  strcpy_s( szName, sizeof( szName ), szName + 1 );

               if ( szName[ strlen( szName ) - 1 ] == '\"' )
                  szName[ strlen( szName ) - 1 ] = 0;

               SetAttributeFromString( vDTE, "TE_FieldDataRel", "Name", szName );
            }

            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Desc", vEMD, "ER_Attribute", "Desc" );
            IncludeSubobjectFromSubobject( vDTE, "ER_Attribute", vEMD, "ER_Attribute", zPOS_AFTER );
            SetAttributeFromString( vDTE, "TE_FieldDataRel", "DataOrRelfieldOrSet", "D" );

            // Call operation to set the default data type.
            (*pfnSetDataType) ( vDTE, TRUE );

            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_SCALE", vEMD, "Domain", "DecimalFormat" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_NULLS", vEMD, "ER_Attribute", "NotNull" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Key", vEMD, "ER_Attribute", "ParticipatesInKey" );
         }

         if ( !bModifyExistingTables )
         {
            // We are processing new table entries so process Identifiers.
            for ( RESULT = SetCursorFirstEntity( vEMD, "ER_EntIdentifier", 0 );
                  RESULT > zCURSOR_UNCHANGED;
                  RESULT = SetCursorNextEntity( vEMD, "ER_EntIdentifier", 0 ) )
            {
               // Delete the Identifier entry if it currently exists.  Then re-add it.
               RESULT = SetCursorFirstEntityByEntityCsr( vDTE, "ER_EntIdentifier", vEMD, "ER_EntIdentifier", "TE_TablRec" );
               if ( RESULT >= zCURSOR_SET )
                  DeleteEntity( vDTE, "TE_TablRecKey", zREPOS_NONE );

               CreateTE_MetaEntity( vSubtask, vDTE, "TE_TablRecKey", zPOS_AFTER );


               // Use the TE Key Name for the IndexName.
               SetAttributeFromAttribute( vDTE, "TE_TablRecKey", "IndexName", vEMD, "ER_EntIdentifier", "TE_KeyName" );

               GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_EntIdentifier", "Name" );
               GetStringFromAttribute( szCreateUnderscore, sizeof( szCreateUnderscore ), vDTE, "TE_DBMS_Source", "TranslateNamesToLowerUnderscore");
               if ( szCreateUnderscore[ 0 ] == 'Y' )
                  TranslateToUnderscoreCase( szName, szName, 'A' );

               ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE, szName, sizeof( szName ), 'A', "TE_TablRecKey", "Name", 0, pfnGenerateName );

               // Set description.
               strcpy_s( szTemp, sizeof( szTemp ), "Implements " );
               GetStringFromAttribute( szTemp + zstrlen( szTemp ), sizeof( szTemp ) - zstrlen( szTemp ), vEMD, "ER_EntIdentifier", "Desc" );
               SetAttributeFromString( vDTE, "TE_TablRecKey", "Desc", szTemp );

               SetAttributeFromString( vDTE, "TE_TablRecKey", "Category", "9" );
               IncludeSubobjectFromSubobject( vDTE, "ER_EntIdentifier", vEMD, "ER_EntIdentifier", zPOS_AFTER );

               if ( CheckExistenceOfEntity( vEMD, "ER_FactType" ) >= zCURSOR_SET )
               {
                  if ( SetCursorFirstEntity( vEMD, "ER_RelLinkIdentifier", "ER_EntIdentifier" ) < zCURSOR_SET )
                  {
                     // We have a System Maintained Key with a FactType.
                     ofnTZTENVRO_BldPrimaryKeyAttr( vSubtask, vDTE, vEMD );
                  }
               }
               else
               {
                  if ( CompareAttributeToString( vEMD, "ER_EntIdentifier", "SystemMaintained", "Y" ) == 0 )
                  {
                     // We have a System Maintained Key with no FactType.
                     ofnTZTENVRO_BldPrimaryKeyToken( vSubtask, vDTE,  vEMD, pfnGenerateName );
                  }
                  else
                  {
                     strcpy_s( szTemp, sizeof( szTemp ), "Entity does not have a valid Key. " );
                     GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_Entity", "Name" );
                     oTZTENVRO_WriteErrorMsgToList( vEMD, szName, szTemp );
                  }
               }
            }

// Eliminated by DC on 1999.05.26 because there should be no reason to reorder
// Identifier information. All TE_TablRecKey entities except the first are
// for secondary identifiers and should not be a part of foreign key
// evaluation.
//            zgSortEntityWithinParent( zASCENDING, vDTE, "TE_TablRecKey", "Category", "" );
         }
      }
   }

   SetViewFromView( vDTE, vDTE_save );

   if ( !bModifyExistingTables )
   {
      // We are looping here through all the TablRecKey entries that have not
      // been processed.  The Category values were initialy set to "9" and were
      // reset to other values as they were processed as System Maintained Keys.
      for ( RESULT = SetCursorFirstEntityByString( vDTE, "TE_TablRecKey", "Category", "9", "TE_DBMS_Source" );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntityByString( vDTE, "TE_TablRecKey", "Category", "9", "TE_DBMS_Source" ) )
      {
         SetCursorFirstEntityByEntityCsr( vEMD, "ER_EntIdentifier", vDTE, "ER_EntIdentifier", "EntpER_Model" );
         ofnTZTENVRO_BldPrimaryKeyAttRel( vSubtask, vDTE, vEMD );
      }
   }

   DropMetaOI( vSubtask, vEMD );
   DropView( vDTE_save );
   DropView( vDTE3 );
   SysFreeLibrary( vDTE, hLibrary );

   return( 0 );

} // oTZTENVRO_BuildTablRecsFromEMD

zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_BldPrimaryKeyToken( zVIEW    vSubtask,
                                zVIEW    vDTE,
                                zVIEW    vEMD,
                                PFNGENERATENAME pfnGenerateName )
{
   // This routine is called when there is a System Maintained Key that
   // is not an attribute.

   zCHAR  szName[ 255 ];

   CreateTE_MetaEntity( vSubtask, vDTE, "TE_FieldDataRel", zPOS_FIRST );

   GetStringFromAttribute( szName, sizeof( szName ), vDTE, "TE_TablRec", "Name" );
   strcat_s( szName, sizeof( szName ), "_Token" );
//  ***ADD ???? from ERD.

   ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE, szName, sizeof( szName ), 'A', "TE_FieldDataRel", "Name", 0, pfnGenerateName );

   SetAttributeFromString( vDTE, "TE_FieldDataRel", "Desc", "Auto build key" );
   SetAttributeFromString( vDTE, "TE_FieldDataRel", "DataOrRelfieldOrSet", "D" );
   SetAttributeFromString( vDTE, "TE_FieldDataRel", "DataType", "L" );
   SetAttributeFromInteger( vDTE, "TE_FieldDataRel", "Length", 4 );
   SetAttributeFromString( vDTE, "TE_FieldDataRel", "SQL_NULLS", "Y" );
   SetAttributeFromString( vDTE, "TE_FieldDataRel", "Key", "Y" );
   IncludeSubobjectFromSubobject( vDTE, "TE_FieldDataRelKey", vDTE, "TE_FieldDataRel", zPOS_FIRST );

   SetAttributeFromString( vDTE, "TE_TablRecKey", "Category", "1" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_BldPrimaryKeyAttr( zVIEW    vSubtask,
                               zVIEW    vDTE,
                               zVIEW    vEMD )
{
   // This routine is called when there is a System Maintained Key that
   // has an attribute.  The routine includes a loop to give an error if
   // there is more than one attribute specified as part of the key.

   zSHORT   RESULT;
   zPCHAR   szName;

   for ( RESULT = SetCursorFirstEntity( vEMD, "ER_FactType", 0 );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vEMD, "ER_FactType", 0 ) )
   {
      if ( CheckExistenceOfEntity( vEMD, "ER_AttributeIdentifier" ) >= zCURSOR_SET )
      {
         SetCursorFirstEntityByEntityCsr( vDTE, "ER_Attribute", vEMD, "ER_AttributeIdentifier", "TE_TablRec" );
         SetAttributeFromString( vDTE, "TE_FieldDataRel", "Key", "Y" );
         IncludeSubobjectFromSubobject( vDTE, "TE_FieldDataRelKey", vDTE, "TE_FieldDataRel", zPOS_LAST );

         if ( CompareAttributeToString( vDTE, "TE_FieldDataRel", "SQL_NULLS", "Y" ) != 0 &&
              GetRelativeEntityNumber( vEMD, "ER_EntIdentifier", "", 0 ) == 0 )
         {
            strcpy_s( sz, sizeof( sz ), "Attribute '" );
            GetAddrForAttribute( &szName, vEMD, "ER_AttributeIdentifier", "Name" );
            strcat_s( sz, sizeof( sz ), szName );
            strcat_s( sz, sizeof( sz ), "' in Identifier '" );
            GetAddrForAttribute( &szName, vEMD, "ER_EntIdentifier", "Name" );
            strcat_s( sz, sizeof( sz ), szName );
            strcat_s( sz, sizeof( sz ), "' is specified as NOT required. " );
            strcat_s( sz, sizeof( sz ), "Physical key will be created as required. " );
            strcat_s( sz, sizeof( sz ), "Please correct ER Data Model." );
            GetAddrForAttribute( &szName, vEMD, "ER_Entity", "Name" );
            oTZTENVRO_WriteErrorMsgToList( vDTE, szName, sz );
            SetAttributeFromString( vDTE, "TE_FieldDataRel", "SQL_NULLS", "Y" );
         }
      }
      else
      {
         strcpy_s( sz, sizeof( sz ), "Invalid Identifier." );
         GetAddrForAttribute( &szName, vEMD, "ER_Entity", "Name" );
         oTZTENVRO_WriteErrorMsgToList( vDTE, szName, sz );
      }

   } // for ...

   SetAttributeFromString( vDTE, "TE_TablRecKey", "Category", "2" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_BldPrimaryKeyAttRel( zVIEW    vSubtask,
                                 zVIEW    vDTE,
                                 zVIEW    vEMD )
{
   // This routine is called for each relationship that is a part of the
   // Identifier.  It loops through each relationship within the Identifier
   // and builds TempRelKey entries that will later be used to build
   // foreign keys for the relationship.
   // In understanding the foreign key problem, we will define the terms
   // parent_entity and child_entity as follows:
   //   child_entity - This is the entity that will contain the foreign keys
   //                  pointing back to the parent_entity of the relationship.
   //   parent_entity - This is the entity that has key attributes which will
   //                  be stored as foreign keys in the chile_entity.
   // The routine would be simple if the keys of the parent_entity were always
   // attributes.  But a complexity occurs if those keys are themselves
   // relationships to other entities.  In fact, this situtation where a
   // parent_entity has keys that are relationships can occur recursively
   // until we finally get to an entity where all the keys are attributes.
   // Thus this routine calls itself recursively to follow the path back
   // to that entity and then returns back through the recursive structure
   // and builds the TempRelKey entries at each level that will later become
   // foreign keys for the relationship.  We capture three pieces of
   // information for each foreign key.
   //
   // 1. The ZKey of the final attribute in the chain that will be the
   //    source of the foreign key.
   // 2. The name of that same attribute. (for debugging purposes only)
   // 3. The ZKey of the RelType which defines the relationship for the
   //    foreign key.

   // TE_TablRec.Category Definitions.
   // 1 - System Maintained Key that is NOT an attribute.
   // 2 - System Maintained Key that IS an attribute.
   // 3 - Key that is made up of attributes and/or relationships.
   // 4 - Was once used but probably not any longer.
   // 9 - Temporary value assigned before real values are assigned.

   zVIEW    vDTE2;
   zVIEW    vEMD2;
   zSHORT   RESULT;
   zPCHAR   szName;

   CreateViewFromViewForTask( &vDTE2, vDTE, 0 );
   CreateViewFromViewForTask( &vEMD2, vEMD, 0 );

   for ( RESULT = SetCursorFirstEntity( vEMD, "ER_FactType", "" );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vEMD, "ER_FactType", "" ) )
   {
      // Process the condition where this part of the Identifier is simply an Attribute.
      if ( CheckExistenceOfEntity( vEMD, "ER_AttributeIdentifier" ) >= zCURSOR_SET )
      {
         SetCursorFirstEntityByEntityCsr( vDTE, "ER_Attribute", vEMD, "ER_AttributeIdentifier", "TE_TablRec" );
         SetAttributeFromString( vDTE, "TE_FieldDataRel", "Key", "Y" );
         IncludeSubobjectFromSubobject( vDTE, "TE_FieldDataRelKey", vDTE, "TE_FieldDataRel", zPOS_LAST );

         if ( CompareAttributeToString( vDTE, "TE_FieldDataRel", "SQL_NULLS", "Y" ) != 0 &&
              GetRelativeEntityNumber( vEMD, "ER_EntIdentifier", "", 0 ) == 0 )
         {
            strcpy_s( sz, sizeof( sz ), "Attribute '" );
            GetAddrForAttribute( &szName, vEMD, "ER_AttributeIdentifier", "Name" );
            strcat_s( sz, sizeof( sz ), szName );
            strcat_s( sz, sizeof( sz ), "' in Identifier '" );
            GetAddrForAttribute( &szName, vEMD, "ER_EntIdentifier", "Name" );
            strcat_s( sz, sizeof( sz ), szName );
            strcat_s( sz, sizeof( sz ), "' is specified as NOT required. " );
            strcat_s( sz, sizeof( sz ), "Physical key will be created as required. " );
            strcat_s( sz, sizeof( sz ), "Please correct ER Data Model." );
            GetAddrForAttribute( &szName, vEMD, "ER_Entity", "Name" );
            oTZTENVRO_WriteErrorMsgToList( vDTE, szName, sz );
            SetAttributeFromString( vDTE, "TE_FieldDataRel", "SQL_NULLS", "Y" );
         }

         continue;
      }

      // I believe the following condition should never happen.  It would
      // mean we had a FactType without either an Attribute or a
      // Relationship.  What we are doing here is simply skipping that entry.  (DonC)
      if ( CheckExistenceOfEntity( vEMD, "ER_Entity_Other_Identifier" ) < zCURSOR_SET )
         continue;

      // For the relationship, position on the associated ER_Entity in the TE.
      // At this time, we're looking for it only within the same DBMS_Source.
      if ( SetCursorFirstEntityByEntityCsr( vDTE2, "ER_Entity", vEMD, "ER_Entity_Other_Identifier", "TE_DBMS_Source" ) < zCURSOR_SET )
      {
         strcpy_s( sz, sizeof( sz ), "Cannot locate Entity in TE in same DS for Rel Key." );
         GetAddrForAttribute( &szName, vEMD, "ER_Entity_Other_Identifier", "Name" );
         oTZTENVRO_WriteErrorMsgToList( vDTE, szName, sz );
#if 0
         for ( RESULT = SetCursorFirstEntityByEntityCsr( vDTE2, "ER_Entity", vEMD, "ER_Entity_Other_Identifier", "TE_DB_Environ" );
               RESULT > zCURSOR_UNCHANGED;
               RESULT = SetCursorNextEntityByEntityCsr( vDTE2, "ER_Entity", vEMD, "ER_Entity_Other_Identifier", "TE_DB_Environ" ) )
         {
            if ( CompareAttributeToAttribute( vDTE, "TE_DBMS_Source", "DBMS", vDTE2, "TE_DBMS_Source", "DBMS" ) == 0 )
            {
               break;   // found Entity implemented in same DBMS
            }
         }

         if ( RESULT <= zCURSOR_UNCHANGED )
         {
            strcpy_s( sz, sizeof( sz ), "Cannot locate Entity in TE in same DS for Rel Key." );
            GetAddrForAttribute( &szName, vEMD, "ER_Entity_Other_Identifier", "Name" );
            oTZTENVRO_WriteErrorMsgToList( vDTE, szName, sz );
            continue;
         }
#endif
      }

      // If we've found a TE ER_Entity entry without an EntIdentifier, then
      // we have an error.
      if ( CheckExistenceOfEntity( vDTE2, "ER_EntIdentifier" ) < zCURSOR_SET )
      {
         strcpy_s( sz, sizeof( sz ), "ER Relationship has Entity without an Identifier." );
         GetAddrForAttribute( &szName, vEMD, "ER_Entity_Other_Identifier", "Name" );
         oTZTENVRO_WriteErrorMsgToList( vDTE, szName, sz );
         continue;
      }

      // Now that we're positioned on the parent entity within the TE,
      // process each TableRecKey.  It looks like we are looping until
      // we find a TablRecKey entry with a Category of either 1, 2 or 3.
      for ( RESULT = SetCursorFirstEntity( vDTE2, "TE_TablRecKey", "TE_TablRec" );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntity( vDTE2, "TE_TablRecKey", "TE_TablRec" ) )
      {
         zPCHAR psz;

         GetAddrForAttribute( &psz, vDTE2, "TE_TablRecKey", "Category" );
         if ( *psz == '1' || *psz == '2' || *psz == '3' )
            break;
      }

      // If a Category 1 or 2 entry was found, we have followed the chain
      // to the primary key (the ER_Entry without any relationships as a part
      // of the Identifier).
      // If a Category 3 entry was found, we have followed the chain to an
      // ER_Entity entry that has relationships as part of the primary key
      // and has already been processed and therefore has TempRelKey entries.

      // If the Category is other than 1, 2 or 3, we must call BldPrimaryKeyAttRel
      // recursively to process the parent entries.
      if ( RESULT < zCURSOR_SET )
      {
         // Now position on the corresponding entry in the ER and continue the
         // recursive process until we find an TablRecKey entry that has already
         // been processed.
         SetCursorFirstEntityByEntityCsr( vEMD2, "ER_EntIdentifier", vDTE2, "ER_EntIdentifier", "EntpER_Model" );
         ofnTZTENVRO_BldPrimaryKeyAttRel( vSubtask, vDTE2, vEMD2 );
      }


      // At this point the parent entries have been processed.  We will thus
      // first loop through any parent TE_FieldDataRelKey entries, which
      // are the non-relationship parts of the key and second loop through the
      // parent TempRelKey entries, which are the relationship parts of the key.
      for ( RESULT = SetCursorFirstEntity( vDTE2, "TE_FieldDataRelKey", 0 );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntity( vDTE2, "TE_FieldDataRelKey", 0 ) )
      {
         CreateEntity( vDTE, "TempRelKey", zPOS_AFTER );
         SetAttributeFromAttribute( vDTE, "TempRelKey", "EntityZKey", vEMD, "ER_Entity_Other_Identifier", "ZKey" );
         SetAttributeFromAttribute( vDTE, "TempRelKey", "KeyName", vDTE2, "TE_FieldDataRelKey", "Name" );
         SetAttributeFromAttribute( vDTE, "TempRelKey", "RelZKey", vEMD, "ER_RelLinkIdentifier", "ZKey" );
      }

      for ( RESULT = SetCursorFirstEntity( vDTE2, "TempRelKey", 0 );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntity( vDTE2, "TempRelKey", 0 ) )
      {
         CreateEntity( vDTE, "TempRelKey", zPOS_AFTER );
         SetAttributeFromAttribute( vDTE, "TempRelKey", "EntityZKey", vEMD, "ER_Entity_Other_Identifier", "ZKey" );
         SetAttributeFromAttribute( vDTE, "TempRelKey", "KeyName", vDTE2, "TempRelKey", "KeyName" );
         SetAttributeFromAttribute( vDTE, "TempRelKey", "RelZKey", vEMD,  "ER_RelLinkIdentifier", "ZKey" );
      }


   } // for each ER_FactType...

   SetAttributeFromString( vDTE, "TE_TablRecKey", "Category", "3" );
   DropView( vDTE2 );
   DropView( vEMD2 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_ImplementRelAs( zPSHORT nRelImplemented,
                            zVIEW vEMD,
                            zVIEW vEMD2,
                            zVIEW vDTE,
                            zVIEW vDTE2,
                            zVIEW vDTE_RelTR,
                            zVIEW vDTE_Save )
{
   zSHORT   RESULT;
   zSHORT   nEntsInDS;
   zCHAR    szRelDesc[ 200 ];
   zPCHAR   szDS;
   zBOOL    bSrcInDS,   bTgtInDS;
   zBOOL    bSrcIsMany, bTgtIsMany;
   zBOOL    bSrcIsReq,  bTgtIsReq;
   zBOOL    bSrcIsInID, bTgtIsInID;
   zBOOL    bRelImplemented;
   zBOOL    bRelImplementedInDS;
   zBOOL    bRelImplementedAsTablRec;
   zBOOL    bRelImplementedAsFkInSrc;
   zBOOL    bRelImplementedAsFkInTgt;

   zCHAR   szSrcEntity[ 255 ];
   zCHAR   szTgtEntity[ 255 ];
   zCHAR   szSrcRelLink[ 255 ];

   *nRelImplemented = zREL_IMPLD_NOT;

   // see if the relationship is between two entities implemented in the current Data Source or with same DBMSs.
   nEntsInDS = 0;

   if ( SetCursorFirstEntityByEntityCsr( vDTE, "ER_Entity", vEMD, "ER_Entity_2", "TE_DBMS_Source" ) >= zCURSOR_SET )
   {
      nEntsInDS++;
      bSrcInDS = TRUE;
   }
   else
   {
      bSrcInDS = FALSE;
      for ( RESULT = SetCursorFirstEntityByEntityCsr( vDTE, "ER_Entity", vEMD, "ER_Entity_2", "TE_DB_Environ" );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntityByEntityCsr( vDTE, "ER_Entity", vEMD, "ER_Entity_2", "TE_DB_Environ" ) )
      {
         if ( CompareAttributeToAttribute( vDTE, "TE_DBMS_Source", "DBMS", vDTE_Save, "TE_DBMS_Source", "DBMS" ) == 0 )
         {
            break;   // found Entity implemented in same DBMS
         }
      }

      if ( RESULT <= zCURSOR_UNCHANGED )
         return( zIMPL_REL_BYPASS );   // different DBMSs, so skip for now...
   }

   if ( SetCursorFirstEntityByEntityCsr( vDTE2, "ER_Entity", vEMD2, "ER_Entity_2", "TE_DBMS_Source" ) >= zCURSOR_SET )
   {
      nEntsInDS++;
      bTgtInDS = TRUE;
   }
   else
   {
      bTgtInDS = FALSE;
      for ( RESULT = SetCursorFirstEntityByEntityCsr( vDTE2, "ER_Entity", vEMD2, "ER_Entity_2", "TE_DB_Environ" );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntityByEntityCsr( vDTE2, "ER_Entity", vEMD2, "ER_Entity_2", "TE_DB_Environ" ) )
      {
         if ( CompareAttributeToAttribute( vDTE2, "TE_DBMS_Source", "DBMS", vDTE_Save, "TE_DBMS_Source", "DBMS" ) == 0 )
         {
            break;   // found Entity implemented in same DBMS
         }
      }

      if ( RESULT <= zCURSOR_UNCHANGED )
      {
         return( zIMPL_REL_BYPASS );   // different DBMSs, so skip for now...
      }
   }

   if ( nEntsInDS == 0 )
      return( zIMPL_REL_BYPASS ); // go to end of loop to process next RelType

   // see if the relationship is currently implemented in the current Data Source.
   bRelImplemented = FALSE;
   bRelImplementedInDS = FALSE;
   bRelImplementedAsTablRec = FALSE;;
   bRelImplementedAsFkInSrc = FALSE;;
   bRelImplementedAsFkInTgt = FALSE;;

   // Let's try to find as FK in Src TablRec
   if ( SetCursorFirstEntityByEntityCsr( vDTE, "ER_RelType_O", vEMD, "ER_RelType", "TE_TablRec" ) > zCURSOR_UNCHANGED )
   {
      bRelImplemented = TRUE;
      bRelImplementedAsFkInSrc = TRUE;
      *nRelImplemented = zREL_IMPLD_AsFkInSrc;
      if ( bSrcInDS )
         bRelImplementedInDS = TRUE;
   }
   else
   {
      // Let's try to find as FK in Tgt
      if ( SetCursorFirstEntityByEntityCsr( vDTE2, "ER_RelType_O", vEMD, "ER_RelType", "TE_TablRec" ) > zCURSOR_UNCHANGED )
      {
         bRelImplemented = TRUE;
         bRelImplementedAsFkInTgt = TRUE;
         *nRelImplemented = zREL_IMPLD_AsFkInTgt;
         if ( bTgtInDS )
            bRelImplementedInDS = TRUE;
      }
      else
      {
         // Let's try to find as a TablRec in the current DS.  If so, it's already been processed and we'll skip it.
         SetViewFromView( vDTE_RelTR, vDTE_Save );
         if ( SetCursorFirstEntityByEntityCsr( vDTE_RelTR, "ER_RelType", vEMD, "ER_RelType", "TE_DBMS_Source" ) >= zCURSOR_SET )
         {
//            bRelImplemented = TRUE;
//            bRelImplementedInDS = TRUE;
//            bRelImplementedAsTablRec = TRUE;
//            *nRelImplemented = zREL_IMPLD_AsTablRec;
              return( zIMPL_REL_BYPASS );
         }
         else
         {
            // Let's try to find as a TablRec in the Model for same DBMS
            for ( RESULT = SetCursorFirstEntityByEntityCsr( vDTE_RelTR, "ER_RelType", vEMD, "ER_RelType", "TE_DB_Environ" );
                  RESULT > zCURSOR_UNCHANGED;
                  RESULT = SetCursorNextEntityByEntityCsr( vDTE_RelTR, "ER_RelType", vEMD, "ER_RelType", "TE_DB_Environ" ) )
            {
               if ( CompareAttributeToAttribute( vDTE_RelTR, "TE_DBMS_Source", "DBMS", vDTE_Save, "TE_DBMS_Source", "DBMS" ) == 0 )
               {
                  break;   // found Rel implemented in same DBMS
               }
            }

            if ( RESULT > zCURSOR_UNCHANGED )
            {
               bRelImplemented = TRUE;
               bRelImplementedInDS = TRUE;
               bRelImplementedAsTablRec = TRUE;
               *nRelImplemented = zREL_IMPLD_AsTablRec;
            }
         }
      }
   }

   // if the Rel is implemented and either Entity is 'work', then delete the current Rel implementation and continue to next Rel
   if ( bRelImplemented )
   {
      if ( CompareAttributeToString( vEMD, "ER_Entity_2", "Purpose", "W" ) == 0  ||
           CompareAttributeToString( vEMD2, "ER_Entity_2", "Purpose", "W" ) == 0 )
      {
         if ( bRelImplementedAsTablRec )
            DeleteEntity( vDTE_RelTR, "TE_TablRec", zREPOS_PREV );
         else
         if ( bRelImplementedAsFkInSrc )
         {
            RESULT = SetCursorFirstEntityByEntityCsr( vDTE, "ER_RelType_O", vEMD, "ER_RelType", "TE_DBMS_Source" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
               // was NOT a MM rel so delete all FieldDataRec's used to implement
               DeleteEntity( vDTE, "TE_FieldDataRel", zREPOS_NONE );
               RESULT = SetCursorNextEntityByEntityCsr( vDTE, "ER_RelType_O", vEMD, "ER_RelType", "TE_TablRec" );
            }
         }
         else
         if ( bRelImplementedAsFkInSrc )
         {
            RESULT = SetCursorFirstEntityByEntityCsr( vDTE, "ER_RelType_O", vEMD, "ER_RelType", "TE_DBMS_Source" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
               // was NOT a MM rel so delete all FieldDataRec's used to
               // implement
               DeleteEntity( vDTE, "TE_FieldDataRel", zREPOS_NONE );
               RESULT = SetCursorNextEntityByEntityCsr( vDTE, "ER_RelType_O", vEMD, "ER_RelType", "TE_TablRec" );
            }
         }

         return( zIMPL_REL_BYPASS );   // skip to next Rel
      }
   }

   // Now determine what type of implementation of the Rel is to be done
   // based on the Cardinality and whether the enities are in the curr DS
   if ( CompareAttributeToInteger( vEMD, "ER_RelLink_2", "CardMax", 1 ) != 0 )
      bSrcIsMany = TRUE;
   else
      bSrcIsMany = FALSE;

   if ( CompareAttributeToInteger( vEMD2, "ER_RelLink_2", "CardMax", 1 ) != 0 )
      bTgtIsMany = TRUE;
   else
      bTgtIsMany = FALSE;


   if ( bSrcIsMany && bTgtIsMany )
   {
      if ( bRelImplemented && !bRelImplementedInDS )
      {
         strcpy_s( sz, sizeof( sz ), "Relationship: '" );

         SetCursorFirstEntityByEntityCsr( vEMD, "ER_Entity", vEMD, "ER_Entity_2", "" );
         GetStringFromAttribute( szSrcEntity, sizeof( szSrcEntity ), vEMD, "ER_Entity", "Name" );
         strcpy_s( szRelDesc, sizeof( szRelDesc ), szSrcEntity );
         strcat_s( szRelDesc, sizeof( szRelDesc ), " (" );
         GetStringFromAttribute( szSrcRelLink, sizeof( szSrcRelLink ), vEMD, "ER_RelLink_2", "Name" );
         strcat_s( szRelDesc, sizeof( szRelDesc ), szSrcRelLink );
         SetCursorFirstEntityByEntityCsr( vEMD2, "ER_Entity", vEMD2, "ER_Entity_2", "" );
         GetStringFromAttribute( szTgtEntity, sizeof( szTgtEntity ), vEMD2, "ER_Entity_2", "Name" );
         strcat_s( szRelDesc, sizeof( szRelDesc ), ") " );
         strcat_s( szRelDesc, sizeof( szRelDesc ), szTgtEntity );

         strcat_s( sz, sizeof( sz ), szRelDesc );
         strcat_s( sz, sizeof( sz ), "'\n is currently implemented in Data Source: '" );
         GetAddrForAttribute( &szDS, vDTE_RelTR, "TE_DBMS_Source", "Name" );
         strcat_s( sz, sizeof( sz ), szDS);
         strcat_s( sz, sizeof( sz ), "'.\n\n Table/Record '" );
         GetAddrForAttribute( &szDS, vDTE_RelTR, "TE_TablRec", "Name" );
         strcat_s( sz, sizeof( sz ), szDS);
         strcat_s( sz, sizeof( sz ), "'." );
         strcat_s( sz, sizeof( sz ), "\n\nRe-implement in the current Data Source?" );
         if (  MessagePrompt( vDTE_RelTR,  "TE00105", "Physical Data Model",
                              sz, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
         {
            return( zIMPL_REL_BYPASS );  // skip implementing this MM rel
         }
         else
         {
            strcpy_s( sz, sizeof( sz ), "Delete current implementation before re-implementing?" );
            if (  MessagePrompt( vDTE_RelTR,  "TE00111", "Physical Data Model",
                                 sz, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
            {
               *nRelImplemented = zREL_IMPLD_NOT;
            }
         }
      }

      return( zIMPL_REL_AsTablRec );
   }

   if ( bSrcIsMany )
   {
      if ( bRelImplemented && !bRelImplementedInDS )
         return( zIMPL_REL_BYPASS ); // skip for now, may want multiple impl's in future
      else
         return( zIMPL_REL_AsFkInTgt );
   }

   if ( bTgtIsMany )
   {
      if ( bRelImplemented && !bRelImplementedInDS )
         return( zIMPL_REL_BYPASS ); // skip for now, may want multiple impl's in future
      else
         return( zIMPL_REL_AsFkInSrc );
   }

   // we now must evaluate the 1-to-1 rel case for choosing where best
   // to place 'parentage'( and therefore the foreign key or ownership )

   // let's see if the 1-to-1 relationship is a "is a" subtype
   if ( CompareAttributeToString( vEMD, "ER_RelLink_2", "Name", "is a" ) == 0 ||
        CompareAttributeToString( vEMD2, "ER_RelLink_2", "Name", "is a" ) == 0 )
   {
      // quick fix to just reverse algorithm used below
      if ( CompareAttributeToString( vEMD, "ER_RelLink_2", "Name", "is a" ) == 0 )
         bSrcIsReq = FALSE;
      else
         bSrcIsReq = TRUE;

      if ( CompareAttributeToString( vEMD2, "ER_RelLink_2", "Name", "is a" ) == 0 )
         bTgtIsReq = FALSE;
      else
         bTgtIsReq = TRUE;
   }
   else
   {
      // let's see if the 1-to-1 relationship is required for either side
      if ( CompareAttributeToInteger( vEMD2, "ER_RelLink_2", "CardMin", 0 ) != 0 )
         bSrcIsReq = TRUE;
      else
         bSrcIsReq = FALSE;

      if ( CompareAttributeToInteger( vEMD, "ER_RelLink_2", "CardMin", 0 ) != 0 )
         bTgtIsReq = TRUE;
      else
         bTgtIsReq = FALSE;
   }

   if ( bSrcIsReq && bTgtIsReq )
   {
      // choose the one that is currently used for implementation (which means bypass if it is in a different DS) or simply
      // choose one that is in the current DS.
      if ( bRelImplemented )
      {
         if ( !bRelImplementedInDS )
            return( zIMPL_REL_BYPASS ); // skip for now, may want multiple impl's in future
         else
         {
            if ( bRelImplementedAsFkInSrc )
               return( zIMPL_REL_AsFkInSrc );
            else
               return( zIMPL_REL_AsFkInTgt );
         }
      }
      else
      {
         if ( bSrcInDS )
            return( zIMPL_REL_AsFkInSrc );
         else
            return( zIMPL_REL_AsFkInTgt );
      }
   }

   if ( bSrcIsReq )
   {
      // choose Tgt to implement
      // which means bypass if it is in a different DS
      if ( !bTgtInDS )
         return( zIMPL_REL_BYPASS ); // skip for now, may want multiple impl's in future
      else
         return( zIMPL_REL_AsFkInTgt );
   }

   if ( bTgtIsReq )
   {
      // choose Src to implement
      // which means bypass if it is in a different DS
      if ( !bSrcInDS )
         return( zIMPL_REL_BYPASS ); // skip for now, may want multiple impl's in future
      else
         return( zIMPL_REL_AsFkInSrc );
   }

   // let's see if the 1-to-1 relationship is used in an identifier for either side
   if ( SetCursorFirstEntityByEntityCsr( vEMD, "ER_RelLinkIdentifier", vEMD, "ER_RelLink_2", "ER_Entity" ) >= zCURSOR_SET ||
        SetCursorFirstEntityByEntityCsr( vEMD, "ER_RelLinkIdentifier", vEMD2, "ER_RelLink_2", "ER_Entity" ) >= zCURSOR_SET )
      bSrcIsInID = TRUE;
   else
      bSrcIsInID = FALSE;

   if ( SetCursorFirstEntityByEntityCsr( vEMD2, "ER_RelLinkIdentifier", vEMD, "ER_RelLink_2", "ER_Entity" ) >= zCURSOR_SET ||
        SetCursorFirstEntityByEntityCsr( vEMD2, "ER_RelLinkIdentifier", vEMD2, "ER_RelLink_2", "ER_Entity" ) >= zCURSOR_SET )
      bTgtIsInID = TRUE;
   else
      bTgtIsInID = FALSE;

   if ( bSrcIsInID && bTgtIsInID )
   {
      // choose the one that is currently used for implementation (which means bypass if it is in a different DS) or simply
      // choose one that is in the current DS.
      if ( bRelImplemented )
      {
         if ( !bRelImplementedInDS )
            return( zIMPL_REL_BYPASS ); // skip for now, may want multiple impl's in future

         if ( bRelImplementedAsFkInSrc )
            return( zIMPL_REL_AsFkInSrc );
         else
            return( zIMPL_REL_AsFkInTgt );
      }
      else
      {
         if ( bSrcInDS )
            return( zIMPL_REL_AsFkInSrc );
         else
            return( zIMPL_REL_AsFkInTgt );
      }
   }

   if ( bSrcIsInID )
   {
      // choose Tgt to implement
      // which means bypass if it is in a different DS
      if ( !bTgtInDS )
         return( zIMPL_REL_BYPASS );  // skip for now, may want multiple impl's in future
      else
         return( zIMPL_REL_AsFkInTgt );
   }

   if ( bTgtIsInID )
   {
      // choose Src to implement
      // which means bypass if it is in a different DS
      if ( !bSrcInDS )
         return( zIMPL_REL_BYPASS );  // skip for now, may want multiple impl's in future
      else
         return( zIMPL_REL_AsFkInSrc );
   }

   // At this point there is no reason to force "ownership" to one entity or
   // thother so simply choose one that is in the current DS
   if ( bRelImplemented )
   {
      if ( !bRelImplementedInDS )
         return( zIMPL_REL_BYPASS );  // skip for now, may want multiple impl's in future

      if ( bRelImplementedAsFkInSrc )
         return( zIMPL_REL_AsFkInSrc );
      else
         return( zIMPL_REL_AsFkInTgt );
   }
   else
   {
      if ( bSrcInDS )
         return( zIMPL_REL_AsFkInSrc );
      else
         return( zIMPL_REL_AsFkInTgt );
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_BuildAutoSeqField( zVIEW    vSubtask,
                             zVIEW    vDTE1,
                             zVIEW    vEMD,
                             PFNGENERATENAME pfnGenerateName )
{
   zCHAR szName[ 255 ];

   // Build the AutoSeq field as a column in the table.

   // Build it as last column in table.
   SetCursorLastEntity( vDTE1, "TE_FieldDataRel", 0 );

   // Create the AutoSeq entity and set attributes.
   CreateTE_MetaEntity( vSubtask, vDTE1, "TE_FieldDataRel", zPOS_LAST );
   strcpy_s( szName, sizeof( szName ), "AutoSeq" );

   ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE1, szName, sizeof( szName ), 'A', "TE_FieldDataRel", "Name", 0, pfnGenerateName );

   SetAttributeFromString( vDTE1, "TE_FieldDataRel", "Name", szName );
   SetAttributeFromString( vDTE1, "TE_FieldDataRel", "DataType", "L" );
   SetAttributeFromString( vDTE1, "TE_FieldDataRel", "Key", "N" );
   SetAttributeFromInteger( vDTE1, "TE_FieldDataRel", "Length", 4 );
   SetAttributeFromString( vDTE1, "TE_FieldDataRel", "DataOrRelfieldOrSet", "D" );

   // Include the RelLink under the AutoSeq field.
   IncludeSubobjectFromSubobject( vDTE1, "ER_RelLink", vEMD,  "ER_RelLink_2", zPOS_AFTER );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_BuildSQL_RelsFromEMD( zVIEW    vSubtask,
                                zVIEW    vDTE_Save,
                                zVIEW    vEntpModel )
{
   // This routine converts the TempRelLink entities into foreign keys
   // (created TE_FieldDataRel entities and included TE_FieldDataRelKey
   // entities).  It does this by repeatedly looping through the ERD RelType
   // entities and creating the foreign key entries until all TempRelLink
   // entities have been processed.  This loop within a loop is necessary
   // because foreign keys can be nested several levels.  To process each
   // RelType, which is a relationship between a parent ER_Entity and a child
   // ER_Entity, the parent ER_Entity must have already had its TempRelKey
   // entities converted to foreign keys.  This may not occur for all entries
   // during the first loop of RelType entities, it is necessary to keep
   // looping through the RelTypes until all are correctly processed.

   zVIEW     vEMD;
   zVIEW     vEMD2;
   zVIEW     vDTE1;       // Parent of relationship
   zVIEW     vDTE2;       // Child of relationship
   zVIEW     vDTE_RelTR;
   zSHORT    RESULT;
   zSHORT    nRelImplemented;
   zSHORT    nImpl;
   zSHORT    nOuterLoopCount = 0;
   zSHORT    nCount;
   zCHAR     szName[ 255 ];
   LPLIBRARY hLibrary;
   PFNGENERATENAME pfnGenerateName;
   zBOOL     bRelProcsIncomplete;

   GetStringFromAttribute( szName, sizeof( szName ), vDTE_Save, "TE_DBMS_Source", "GenerateExecutable" );

   hLibrary = SysLoadLibrary( vDTE_Save, szName );
   if ( !hLibrary )
   {
      MessageSend( vDTE_Save, "TE00112", "Physical Data Model",
                   "Couldn't load Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( zCALL_ERROR );
   }

   pfnGenerateName = (PFNGENERATENAME) SysGetProc( hLibrary, "GenerateName" );
   if ( !pfnGenerateName )
   {
      MessageSend( vDTE_Save, "TE00113", "Physical Data Model",
                   "Couldn't find 'GenerateName' in Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SysFreeLibrary( vDTE_Save, hLibrary );
      return( zCALL_ERROR );
   }

   CreateViewFromViewForTask( &vDTE1, vDTE_Save, 0 );
   CreateViewFromViewForTask( &vDTE2, vDTE1, 0 );
   CreateViewFromViewForTask( &vDTE_RelTR, vDTE1, 0 );
   CreateViewFromViewForTask( &vEMD, vEntpModel, 0 );
   CreateViewFromViewForTask( &vEMD2, vEMD, 0 );

   // Loop through all RelLinks and create an AutoSeq field for each RelLink that has AutoSeq = "Y".  The field created will be somewhat similar
   // to a foreign key in that it will not have an associated ER_Attribute and it will have an ER_RelLink included beneath it.
   for ( RESULT = SetCursorFirstEntity( vEMD, "ER_RelLink_2", "EntpER_Model" );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vEMD, "ER_RelLink_2", "EntpER_Model" ) )
   {
      if ( CompareAttributeToString( vEMD, "ER_RelLink_2", "AutoSeq", "Y" ) == 0 )

      {
         // Position on the child entity of the relationship.
         SetViewFromView( vEMD2, vEMD );
         if ( SetCursorNextEntity( vEMD2, "ER_RelLink_2", 0 ) < zCURSOR_SET )
            SetCursorPrevEntity( vEMD2, "ER_RelLink_2", 0 );

         // Only create AutoSeq field here if this is not many-to-many rel, which means one of the rels must have CardMax of 1.
         if ( CompareAttributeToInteger( vEMD,  "ER_RelLink_2", "CardMax", 1 ) == 0 ||
              CompareAttributeToInteger( vEMD2, "ER_RelLink_2", "CardMax", 1 ) == 0 )
         {
            zSHORT nRC;

            // Position on the associated TE_TablRec entity.
            // It is possible, that we have an "AUTOSEQ" on a
            //  relationship between two ER work entities, although
            //  that does not make any sense.
            // This case is skipped.
            nRC = SetCursorFirstEntityByEntityCsr( vDTE1, "ER_Entity", vEMD2, "ER_Entity_2", "TE_DBMS_Source" );
            if ( nRC < zCURSOR_SET )
               continue;

            // Go to build the AutoSeq field.
            oTZTENVRO_BuildAutoSeqField( vSubtask, vDTE1, vEMD, pfnGenerateName );
         }
      }
   }

   // Continue multiple loops through the ERD RelTypes until all relationships have been processed for foreign keys.
   bRelProcsIncomplete = TRUE;
   while ( bRelProcsIncomplete )
   {
      // Initialize the next pass as being complete.
      bRelProcsIncomplete = FALSE;
      nOuterLoopCount = nOuterLoopCount + 1;

      // Make a pass through the RelType entries.
      for ( RESULT = SetCursorFirstEntity( vEMD, "ER_RelType", 0 );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntity( vEMD, "ER_RelType", 0 ) )
      {
         SetViewFromView( vEMD2, vEMD );
         SetViewFromView( vDTE1, vDTE_Save );
         SetViewFromView( vDTE2, vDTE_Save );
         SetCursorNextEntity( vEMD2, "ER_RelLink_2", "" );

         nImpl = ofnTZTENVRO_ImplementRelAs( &nRelImplemented, vEMD, vEMD2, vDTE1, vDTE2, vDTE_RelTR, vDTE_Save );

         if ( nImpl == zIMPL_REL_BYPASS )
            continue;  // process next rel...

         // Only process the RelType entry if the foreign keys in the parent have already been processed.

         // Implement the rel in the Data Source based on "ownership".
         if ( nImpl == zIMPL_REL_AsFkInSrc && CheckExistenceOfEntity( vDTE2, "TempRelKey" ) < zCURSOR_SET )
         {
            ofnTZTENVRO_ImplementSQL_RelLink( vSubtask, vDTE1, vEMD, vDTE2, vEMD2, pfnGenerateName );
         }
         else
         if ( nImpl == zIMPL_REL_AsFkInTgt &&
              CheckExistenceOfEntity( vDTE1, "TempRelKey" ) < zCURSOR_SET )
         {
              ofnTZTENVRO_ImplementSQL_RelLink( vSubtask, vDTE2, vEMD2, vDTE1, vEMD, pfnGenerateName );
         }
         else
         if ( nImpl == zIMPL_REL_AsTablRec )
         {
            // this is generally for a many-to-many
            SetViewFromView( vDTE_RelTR, vDTE_Save );  // reset to current DS
            ofnTZTENVRO_CreateRelTablRec( vSubtask, vDTE1, vEMD, vDTE2, vEMD2, vDTE_RelTR, pfnGenerateName );
            ofnTZTENVRO_ImplementSQL_RelLink( vSubtask, vDTE_RelTR, vEMD2, vDTE1, vEMD, pfnGenerateName );
            ofnTZTENVRO_ImplementSQL_RelLink( vSubtask, vDTE_RelTR, vEMD, vDTE2, vEMD2, pfnGenerateName );
            // build the Keys by looping thru all attributes of the just created enitity (many-to-many --> all attributes should be keys
            for ( RESULT = SetCursorFirstEntity( vDTE_RelTR, "TE_FieldDataRel", 0 );
                  RESULT > zCURSOR_UNCHANGED;
                  RESULT = SetCursorNextEntity( vDTE_RelTR, "TE_FieldDataRel", 0 ) )
            {
               SetAttributeFromString( vDTE_RelTR, "TE_FieldDataRel", "Key", "Y" );
               IncludeSubobjectFromSubobject( vDTE_RelTR, "TE_FieldDataRelKey", vDTE_RelTR, "TE_FieldDataRel", zPOS_AFTER );
            }

#if 0
SetNameForView( vEMD, "***vEMD", vSubtask, zLEVEL_TASK );
SetNameForView( vEMD2, "***vEMD2", vSubtask, zLEVEL_TASK );
SetNameForView( vDTE2, "***vDTE2", vSubtask, zLEVEL_TASK );
SetNameForView( vDTE1, "***vDTE1", vSubtask, zLEVEL_TASK );
SetNameForView( vDTE_RelTR, "***vDTE_RelTR", vSubtask, zLEVEL_TASK );
IssueError( vEMD, 0, 0, "stop" );
#endif
            // If either side of the relationship uses Auto Sequencing, create an AutoSeq field.
            if ( CompareAttributeToString( vEMD, "ER_RelLink_2", "AutoSeq", "Y" ) == 0 )
            {
               // Go to build the AutoSeq field for the first side of the relationship.
               oTZTENVRO_BuildAutoSeqField( vSubtask, vDTE_RelTR, vEMD, pfnGenerateName );
            }

            SetCursorNextEntity( vEMD, "ER_RelLink_2", 0 );
            if ( CompareAttributeToString( vEMD, "ER_RelLink_2", "AutoSeq", "Y" ) == 0 )
            {
               // Go to build the AutoSeq field for the other side of the relationship.
               oTZTENVRO_BuildAutoSeqField( vSubtask, vDTE_RelTR, vEMD, pfnGenerateName );
            }
         }
         else
            if ( nOuterLoopCount >= 10 )
            {
               // If we have executed the outer loop 10 times, we will assume that we have some kind of a relationship loop,
               // that is a ring of entities where they each have a relationship to one of the other entities as a part of the key.
               strcpy_s( sz, sizeof( sz ), "Relationship is part of an Identifier loop. " );
               strcat_s( sz, sizeof( sz ), "From Entity = " );
               GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_Entity_2", "Name" );
               strcat_s( sz, sizeof( sz ), szName );
               strcat_s( sz, sizeof( sz ), ",  Relationship = " );
               GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_RelLink_2", "Name" );
               strcat_s( sz, sizeof( sz ), szName );
               strcat_s( sz, sizeof( sz ), ",  To Entity = " );
               GetStringFromAttribute( szName, sizeof( szName ), vEMD2, "ER_Entity_2", "Name" );
               strcat_s( sz, sizeof( sz ), szName );
               GetStringFromAttribute( szName, sizeof( szName ), vEMD, "ER_Entity_2", "Name" );
               oTZTENVRO_WriteErrorMsgToList( vDTE_Save, szName, sz );
               return( zCALL_ERROR );
            }
            else
               bRelProcsIncomplete = TRUE;
      }  // End of FOR EACH ER_RelType
   }


   // Order all the key fields from relationships according to the order
   // specified in the ER Identifiers. Key fields from Attributes in the
   // Identifiers are already ordered properly so we only need to process
   // those Identifiers in the ER that have FactTypes that are relationships.
   SetViewFromView( vDTE1, vDTE_Save );
   for ( RESULT = SetCursorFirstEntity( vEMD, "ER_EntIdentifier", "EntpER_Model" );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vEMD, "ER_EntIdentifier", "EntpER_Model" ) )
   {
      RESULT = SetCursorFirstEntityByString( vEMD, "ER_FactType", "Type", "REL", 0 );
      if ( RESULT >= zCURSOR_SET )
      {
         nCount = 1;
         SetCursorFirstEntityByAttr( vDTE1, "ER_Entity", "ZKey", vEMD,  "ER_Entity", "ZKey", "TE_DBMS_Source" );
         // The Identifier has relationships so set the correct order.
         for ( RESULT = SetCursorFirstEntity( vEMD, "ER_FactType", 0 );
               RESULT > zCURSOR_UNCHANGED;
               RESULT = SetCursorNextEntity( vEMD, "ER_FactType", 0 ) )
         {
            // First set a work attribute in the key entry to the relative
            // position of the corresponding ER_FactType within the Identifier.
            // Positioning logic depends on whether the ER_FactType if for a REL or an ATR.
            if ( CheckExistenceOfEntity( vEMD, "ER_RelLinkIdentifier" ) >= zCURSOR_SET )
            {
               // ER_FactType is REL

               // To position on the correct key entry, start by positioning on
               // the correct TE_FieldDataRel entities for the relationship.
               // Then position on the corresponding TE_FieldDataRelKey.
               RESULT = SetCursorFirstEntityByAttr( vDTE1, "ER_RelLink", "ZKey", vEMD, "ER_RelLinkIdentifier", "ZKey", "TE_TablRec" );
               while ( RESULT >= zCURSOR_SET )
               {
                  if ( SetCursorFirstEntityByAttr( vDTE1, "TE_FieldDataRelKey", "ZKey", vDTE1, "TE_FieldDataRel", "ZKey", 0 ) >= zCURSOR_SET )
                  {
                     SetAttributeFromInteger( vDTE1, "TE_FieldDataRelKey", "TempOrderNo", nCount );
                  }
                  nCount++;
                  RESULT = SetCursorNextEntityByAttr( vDTE1, "ER_RelLink", "ZKey", vEMD, "ER_RelLinkIdentifier", "ZKey", "TE_TablRec" );
               }
            }
            else
            {
               // ER_FactType is ATR

               // There is a single TE_FieldDataRelKey entry for an Attribute.
               // To position on it, first position on the correct TE_FieldDataRel entity.
               SetCursorFirstEntityByAttr( vDTE1, "ER_Attribute", "ZKey", vEMD,  "ER_AttributeIdentifier", "ZKey", "TE_TablRec" );
               if ( CheckExistenceOfEntity( vDTE1, "TE_FieldDataRelKey" ) >= zCURSOR_SET &&
                    CheckExistenceOfEntity( vDTE1, "TE_FieldDataRel" ) >= zCURSOR_SET )
               {
                  SetCursorFirstEntityByAttr( vDTE1, "TE_FieldDataRelKey", "ZKey", vDTE1, "TE_FieldDataRel", "ZKey", 0 );
                  SetAttributeFromInteger( vDTE1, "TE_FieldDataRelKey", "TempOrderNo", nCount );
               }
            }

            nCount++;
         }

         // Finally sort the entities by TempOrderNo.
         OrderEntityForView( vDTE1, "TE_FieldDataRelKey", "TempOrderNo A" );
      }
   }

   DropView( vDTE1 );
   DropView( vDTE2 );
   DropView( vDTE_RelTR );
   DropView( vEMD );
   DropView( vEMD2 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_BuildNET_RelsFromEMD( zVIEW    vSubtask,
                                zVIEW    vDTE_Save,
                                zVIEW    vEntpModel )
{
   zVIEW     vEMD, vEMD2;
   zVIEW     vDTE, vDTE2;
   zVIEW     vDTE_RelTR;
   zSHORT    RESULT;
   zSHORT    nRelImplemented;
   zSHORT    nImpl;
   zCHAR     szName[ 255 ];
   LPLIBRARY hLibrary;
   PFNGENERATENAME pfnGenerateName;

   // This routine builds relationships for Network databases.  It once was also used for SQL databases but handling
   // relationships for SQL was rewritten as a separate routine.

   GetStringFromAttribute( szName, sizeof( szName ), vDTE_Save, "TE_DBMS_Source", "GenerateExecutable" );

   hLibrary = SysLoadLibrary( vDTE_Save, szName );
   if ( !hLibrary )
   {
      MessageSend( vDTE_Save, "TE00115", "Physical Data Model",
                   "Couldn't load Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( zCALL_ERROR );
   }

   pfnGenerateName = (PFNGENERATENAME) SysGetProc( hLibrary, "GenerateName" );
   if ( !pfnGenerateName )
   {
      MessageSend( vDTE_Save, "TE00116", "Physical Data Model",
                   "Couldn't find 'GenerateName' in Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SysFreeLibrary( vDTE_Save, hLibrary );
      return( zCALL_ERROR );
   }

   CreateViewFromViewForTask( &vDTE, vDTE_Save, 0 );
   CreateViewFromViewForTask( &vDTE2, vDTE, 0 );
   CreateViewFromViewForTask( &vDTE_RelTR, vDTE, 0 );
   CreateViewFromViewForTask( &vEMD, vEntpModel, 0 );
   CreateViewFromViewForTask( &vEMD2, vEMD, 0 );

   for ( RESULT = SetCursorFirstEntity( vEMD, "ER_RelType", 0 );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vEMD, "ER_RelType", 0 ) )
   {
      SetViewFromView( vEMD2, vEMD );
      SetViewFromView( vDTE, vDTE_Save );
      SetViewFromView( vDTE2, vDTE_Save );
      RESULT = SetCursorNextEntity( vEMD2, "ER_RelLink_2", "" );

      nImpl = ofnTZTENVRO_ImplementRelAs( &nRelImplemented, vEMD, vEMD2, vDTE, vDTE2, vDTE_RelTR, vDTE_Save );
      if ( nImpl == zIMPL_REL_BYPASS )
         continue;  // process next rel...

      // At this point we have determined that we will implement the Rel in the current Data Source,
      // so we should delete it if it is currently implemented in any Data Source.
      if ( nRelImplemented == zREL_IMPLD_AsTablRec )
         DeleteEntity( vDTE_RelTR, "TE_TablRec", zREPOS_PREV );
      else
      if ( nRelImplemented == zREL_IMPLD_AsFkInSrc )
      {
         for ( RESULT = SetCursorFirstEntityByEntityCsr( vDTE, "ER_RelType_O", vEMD, "ER_RelType", "TE_DBMS_Source" );
               RESULT > zCURSOR_UNCHANGED;
               RESULT = SetCursorNextEntityByEntityCsr( vDTE, "ER_RelType_O", vEMD, "ER_RelType", "TE_TablRec" ) )
         {
            // was NOT a MM rel so delete all FieldDataRec's used to implement
            DeleteEntity( vDTE, "TE_FieldDataRel", zREPOS_NONE );
         }
      }
      else
      if ( nRelImplemented == zREL_IMPLD_AsFkInTgt )
      {
         RESULT = SetCursorFirstEntityByEntityCsr( vDTE2, "ER_RelType_O", vEMD2, "ER_RelType", "TE_DBMS_Source" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {  // was NOT a MM rel so delete all FieldDataRec's used to implement
            DeleteEntity( vDTE2, "TE_FieldDataRel", zREPOS_NONE );
            RESULT = SetCursorNextEntityByEntityCsr( vDTE2, "ER_RelType_O", vEMD2, "ER_RelType", "TE_TablRec" );
         }
      }

      // implement the rel in the Data Source based on "ownership".
      if ( nImpl == zIMPL_REL_AsFkInSrc )
         ofnTZTENVRO_ImplementNET_RelLink( vSubtask, vDTE, vEMD, vDTE2, vEMD2, pfnGenerateName );
      else
      if ( nImpl == zIMPL_REL_AsFkInTgt )
         ofnTZTENVRO_ImplementNET_RelLink( vSubtask, vDTE2, vEMD2, vDTE, vEMD, pfnGenerateName );
      else
      if ( nImpl == zIMPL_REL_AsTablRec )
      {
         // this is generally for a many-to-many
         SetViewFromView( vDTE_RelTR, vDTE_Save );  // reset to current DS
         ofnTZTENVRO_CreateRelTablRec( vSubtask, vDTE, vEMD, vDTE2, vEMD2, vDTE_RelTR, pfnGenerateName );
         ofnTZTENVRO_ImplementNET_RelLink( vSubtask, vDTE_RelTR, vEMD2, vDTE, vEMD, pfnGenerateName );
         ofnTZTENVRO_ImplementNET_RelLink( vSubtask, vDTE_RelTR, vEMD, vDTE2, vEMD2, pfnGenerateName );
      }
   }  // End of FOR EACH ER_RelType


   //
   // DGC 8/4/95
   //
   // In the group of TE_FieldDataRelKey entities is a list of the keys for
   // both physical entities involved in a relationship.  We need to go
   // through the TE_FieldDataRel entities and find a match.  If a match is
   // not found, then the attribute represented by the entity
   // TE_FieldDataRelKey is not part of the entity represented by
   // TE_FieldDataRel and therefore must be deleted.
   //

// try to handle any non-set keys ...
   for ( RESULT = SetCursorFirstEntity( vDTE, "TE_FieldDataRelKey", "TE_DBMS_Source" );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vDTE, "TE_FieldDataRelKey", "TE_DBMS_Source" ) )
   {
      SetViewFromView( vDTE2, vDTE );

      // if we don't find the key field in this table
      if ( SetCursorFirstEntityByEntityCsr( vDTE2, "TE_FieldDataRel", vDTE, "TE_FieldDataRelKey", 0 ) < zCURSOR_SET )
      {
         // Try to find a TE_FieldDataRel that matches TE_FieldDataRelKey.
         // If one is NOT found, then we exclude the TE_FieldDataRelKey
         // because it's not one of the attributes listed in TE_FieldDataRel.
         //
         // Note:  We use SetCursor-FIRST-Entity in the 'for' loop twice
         //(instead of a ..First.. and ..Next..) because we're looking for
         // a match of ...DataRelKey with ANY ...DataRel.

         for ( RESULT = SetCursorFirstEntityByEntityCsr( vDTE2, "TE_FieldDataRel", vDTE, "TE_FieldDataRelKey",  0 );
               RESULT < zCURSOR_SET;
               RESULT = SetCursorFirstEntityByEntityCsr( vDTE2, "TE_FieldDataRel", vDTE, "TE_FieldDataRelKey", 0 ) )
         {
            ExcludeEntity( vDTE, "TE_FieldDataRelKey", zREPOS_NEXT );

            if ( CheckExistenceOfEntity( vDTE, "TE_FieldDataRelKey" ) == zCURSOR_NULL )
               break;
         }
      }
   }  // end for each TE_FieldDataRelKey


   for ( RESULT = SetCursorFirstEntity( vEMD2, "ER_EntIdentifier", "EntpER_Model" );
         RESULT >= zCURSOR_SET;
         RESULT = SetCursorNextEntity( vEMD2, "ER_EntIdentifier", "EntpER_Model" ) )
   {
      for ( RESULT = SetCursorFirstEntity( vEMD2, "ER_RelLinkIdentifier", "ER_EntIdentifier" );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntity( vEMD2, "ER_RelLinkIdentifier", "ER_EntIdentifier" ) )
      {
         for ( RESULT = SetCursorFirstEntityByEntityCsr( vDTE2, "ER_RelLink", vEMD2, "ER_RelLinkIdentifier", "TE_DBMS_Source" );
               RESULT > zCURSOR_UNCHANGED;
               RESULT = SetCursorNextEntityByEntityCsr( vDTE2, "ER_RelLink", vEMD2, "ER_RelLinkIdentifier", "TE_DBMS_Source" ) )
         {
            if ( SetCursorFirstEntityByEntityCsr( vDTE2, "TE_FieldDataRelKey", vDTE2, "TE_FieldDataRel", "TE_TablRec" ) < zCURSOR_SET )
            {
               SetAttributeFromString( vDTE2, "TE_FieldDataRel", "Key", "Y" );
               IncludeSubobjectFromSubobject( vDTE2, "TE_FieldDataRelKey", vDTE2, "TE_FieldDataRel", zPOS_AFTER );
            }
         }  // end for each field implementing the rel
      }  // end for each Linked identifier
   }  // end for each ER identifier

   DropView( vDTE );
   DropView( vDTE2 );
   DropView( vDTE_RelTR );
   DropView( vEMD2 );
   DropView( vEMD );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_BuildRelationsFromEMD( zVIEW    vSubtask,
                                 zVIEW    vDTE_Save,
                                 zVIEW    vEntpModel )
{
   zSHORT   nRC;
   zVIEW    vTZTEDBLO;

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vDTE_Save, zLEVEL_TASK );
   SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS", vDTE_Save, "TE_DBMS_Source", "DBMS", 0 );

   if ( CompareAttributeToString( vDTE_Save, "TE_DBMS_Source", "Paradigm", "R" ) == 0 )
      nRC = oTZTENVRO_BuildSQL_RelsFromEMD( vSubtask, vDTE_Save, vEntpModel );
   else
      nRC = oTZTENVRO_BuildNET_RelsFromEMD( vSubtask, vDTE_Save, vEntpModel );

   return( nRC );
}


zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_ImplementSQL_RelLink( zVIEW    vSubtask,
                                  zVIEW    vDTE_c,
                                  zVIEW    vEMD,
                                  zVIEW    vDTE_p,
                                  zVIEW    vEMD_p,
                                  PFNGENERATENAME pfnGenerateName )
{
   zVIEW    vDTE_t;
   zVIEW    vTZTEDBLO;
   zCHAR    szDesc[ 255 ];
   zCHAR    szName[ 255 ];
   zSHORT   RESULT;
   zCHAR    szTemp[ 100 ];
   zCHAR    szTemp2[ 100 ];
   zLONG    lEntityZKey;
   zLONG    lRelZKey;
   zVIEW    vDBH_Data;
   zCHAR    szDBH_DataObjectName[ zZEIDON_NAME_LTH + 1 ];
   static int count = 0;

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vDTE_c, zLEVEL_TASK );

   // Look for the first TE_TablRecKey with Category less than 9.
   for ( RESULT = SetCursorFirstEntity( vDTE_p, "TE_TablRecKey", 0 );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vDTE_p, "TE_TablRecKey", 0 ) )
   {
      if ( CompareAttributeToString( vDTE_p, "TE_TablRecKey", "Category", "9" ) < 0 )
         break;
   }

   // If RESULT is < zCURSOR_SET then no Category found < 9.
   if ( RESULT < zCURSOR_SET )
   {
      strcpy_s( sz, sizeof( sz ), "Parent Entity does not have a valid Key. " );
      if ( CheckExistenceOfEntity( vDTE_p, "ER_Entity" ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( szDesc, sizeof( szDesc ), vDTE_p, "ER_Entity", "Name" );
      }
      else
      {
         strcat_s( sz, sizeof( sz ), " TE_TablRec: Name = " );
         GetStringFromAttribute( szDesc, sizeof( szDesc ), vDTE_p, "TE_TablRec", "Name" );
         strcat_s( sz, sizeof( sz ), szDesc );
      }
      oTZTENVRO_WriteErrorMsgToList( vDTE_c, szDesc, sz );
      return( 0 );
   }

   // Return if this relationship has already been processed.  We know this if there is a FieldDataRel with a subordinate ER_RelLink
   // that matches the vEMD_p.ER_RelLink_2 entity AND the FieldDataRel also has a FieldDataRelFK entity under it.  An AutoSeq entry
   // could have the same ER_RelLink but no FieldDataRelFK.
   for ( RESULT = SetCursorFirstEntityByEntityCsr( vDTE_c, "ER_RelLink", vEMD_p, "ER_RelLink_2", "TE_TablRec" );
         RESULT >= zCURSOR_SET;
         RESULT = SetCursorNextEntityByEntityCsr( vDTE_c, "ER_RelLink", vEMD_p, "ER_RelLink_2", "TE_TablRec" ) )
   {
      if ( CheckExistenceOfEntity( vDTE_c, "TE_FieldDataRel_PK" ) >= zCURSOR_SET )
      {
         return( 0 );
      }
   }

   SetOI_FromBlob( &vDBH_Data, szDBH_DataObjectName, vDTE_c, vDTE_p, "TE_DBMS_Source", "DBH_Data", zNOI_OKAY );

   // Prior to this code, the TempRelKey entities for DTE_p have been converted to TE_FieldDataRelKey entries,
   // which point to the corresponding foreign key in DTE_p.
   // This code must
   // 1.  Build the corresponding foreign key fields as TE_FieldDataRel
   //     entities under the current TE_TablRec entity in vDTE_c.
   // 2.  Delete the corresponding vDTE_c.TempRelKey record, if there is one.
   // 3.  Include the TE_FieldDataRel foreign key entity just built if the
   //     foreign key entity is a part of the Identifier of vDTE_c.

   for ( RESULT = SetCursorFirstEntity( vDTE_p, "TE_FieldDataRelKey", 0 );
         RESULT > zCURSOR_UNCHANGED;
         RESULT = SetCursorNextEntity( vDTE_p, "TE_FieldDataRelKey", 0 ) )
   {
      CreateTE_MetaEntity( vSubtask, vDTE_c, "TE_FieldDataRel", zPOS_LAST );
      SetMatchingAttributesByName( vDTE_c, "TE_FieldDataRel", vDTE_p, "TE_FieldDataRelKey", zSET_NULL );

      if ( CompareAttributeToString( vDTE_p, "TE_FieldDataRelKey", "DataOrRelfieldOrSet", "R" ) == 0 )
      {
         GetStringFromAttribute( szTemp2, sizeof( szTemp2 ), vDTE_c, "TE_FieldDataRel", "Name" );
         strcpy_s( szTemp, sizeof( szTemp ), szTemp2 );
      }
      else
      {
         // Generate TE foreign key name of Prefix + Column Name
         // Prefix (is optional):
         //    - is Option "TE_NoForeignKeyPrefix" = Y, do not create a Prefix
         //    - is Option "TE_NoForeignKeyPrefix" = N and TE_ForeignKeyPrefix
         //      does not exists, create the Default Prefix "FK_"
         //    - is Option "TE_NoForeignKeyPrefix" = N and TE_ForeignKeyPrefix
         //      exists, use TE_ForeignKeyPrefix
         // Column Name:
         //    - if TE_ForeignKeyName exists, use TE_ForeignKeyName
         //    - if TE_ForeignKeyName does not exists, use TE_ColumnName
         //    - if TE_ForeignKeyName and TE_ColumnName do not exist,
         //      use Name (logical Name of ER_Attribute)

         zCHAR szPrefix[ 255 ];
         zCHAR szColumnName[ 255 ];
         zCHAR szNoPrefix[ 2 ];

         CreateViewFromViewForTask( &vDTE_t, vDTE_p, 0 );
         SetCursorFirstEntityByAttr( vDTE_t, "TE_FieldDataRel", "ZKey", vDTE_p, "TE_FieldDataRelKey", "ZKey", 0 );
         GetStringFromAttribute( szNoPrefix, sizeof( szNoPrefix ), vEMD_p, "ER_RelLink_2", "TE_NoForeignKeyPrefix" );
         GetStringFromAttribute( szPrefix, sizeof( szPrefix ), vEMD_p, "ER_RelLink_2", "TE_ForeignKeyPrefix" );
         // get Foreign Key Column Name
         if ( CheckExistenceOfEntity( vDTE_t, "ER_Attribute" ) >= zCURSOR_SET )
         {
            GetStringFromAttribute( szColumnName, sizeof( szColumnName ), vDTE_t, "ER_Attribute", "TE_ForeignKeyName" );
            if ( *szColumnName == 0 )
            {
               GetStringFromAttribute( szColumnName, sizeof( szColumnName ), vDTE_t, "ER_Attribute", "TE_ColumnName" );
               if ( *szColumnName == 0 )
                  GetStringFromAttribute( szColumnName, sizeof( szColumnName ), vDTE_t, "ER_Attribute", "Name" );
            }
         }
         else
            GetStringFromAttribute( szColumnName, sizeof( szColumnName ), vDTE_c, "TE_FieldDataRel", "Name" );

         // create Default Prefix + Column Name
         if ( zstrcmp( szNoPrefix, "Y" ) != 0 && *szPrefix == 0 )
         {
            strcpy_s( szTemp, sizeof( szTemp ), "FK_" );
            strcat_s( szTemp, sizeof( szTemp ), szColumnName );
            /* Change foreign key name to upper-case. */
            SysTranslateString( szTemp, 'U' );
         }
         else
         {
            // if TE_NoForeignKeyPrefix = Y, do not generate a Prefix
            if ( zstrcmp( szNoPrefix, "Y" ) == 0 || *szPrefix == 0 )
               strcpy_s( szTemp, sizeof( szTemp ), szColumnName );
            else
            {
               strcpy_s( szTemp, sizeof( szTemp ), szPrefix );
               strcat_s( szTemp, sizeof( szTemp ), szColumnName );
            }
         }
         DropView( vDTE_t );
      }

      // If we have a DBH-specific object and if the object is the ODBC object then we'll determine whether
      // or not we should add table names from there.
      if ( vDBH_Data && zstrcmpi( szDBH_DataObjectName, "TZDBHODO" ) == 0 )
      {
         if ( CompareAttributeToString( vDBH_Data, "ODBC", "AddTableNameToFK_Override", "Y" ) == 0 )
         {
            GetStringFromAttribute( szName, sizeof( szName ), vDTE_p, "TE_TablRec", "Name" );
            strcat_s( szTemp, sizeof( szTemp ), "_" );
            strcat_s( szTemp, sizeof( szTemp ), szName );
         }
      }
      else
      if ( CompareAttributeToString( vTZTEDBLO, "TE_DBMS_Source", "DBS_FullFK_Name", "Y" ) == 0 )
      {
         GetStringFromAttribute( szName, sizeof( szName ), vDTE_p, "TE_TablRec", "Name" );
         strcat_s( szTemp, sizeof( szTemp ), "_" );
         strcat_s( szTemp, sizeof( szTemp ), szName );
      }

      // DGC 4/2/97  Blank out attribute Name so we don't get any frivolous Duplicate names.
      SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "Name", " " );

      ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE_c, szTemp, sizeof( szTemp ), 'A', "TE_FieldDataRel", "Name", 0, pfnGenerateName );

      SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "DataOrRelfieldOrSet", "R" );
      SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "Key", "" );

      if ( CheckExistenceOfEntity( vDTE_c, "ER_Entity" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToAttribute( vEMD_p, "ER_Entity_2", "ZKey", vDTE_c, "ER_Entity", "ZKey" ) == 0 )
         {
            if ( CompareAttributeToString( vEMD_p, "ER_RelLink_2", "CardMin", "0" ) == 0 )
               SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "SQL_NULLS", "" );
            else
               SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "SQL_NULLS", "Y" );
         }
         else
         {
            if ( CompareAttributeToString( vEMD, "ER_RelLink_2", "CardMin", "0" ) == 0 )
               SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "SQL_NULLS", "" );
            else
               SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "SQL_NULLS", "Y" );
         }
      }

      GetStringFromAttribute( szDesc, sizeof( szDesc ), vDTE_p, "TE_TablRec", "Name" );
      strcat_s( szDesc, sizeof( szDesc ), "(" );
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD_p, "ER_RelLink_2", "Name" );
      strcat_s( szDesc, sizeof( szDesc ), szTemp );
      strcat_s( szDesc, sizeof( szDesc ), " [" );
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD_p, "ER_RelLink_2", "CardMin" );
      strcat_s( szDesc, sizeof( szDesc ), szTemp );
      strcat_s( szDesc, sizeof( szDesc ), ":" );
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD_p, "ER_RelLink_2", "CardMax" );
      strcat_s( szDesc, sizeof( szDesc ), szTemp );
      strcat_s( szDesc, sizeof( szDesc ), "] ) " );
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vDTE_c, "TE_TablRec", "Name" );
      strcat_s( szDesc, sizeof( szDesc ), szTemp );
      SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "Desc", szDesc );

      IncludeSubobjectFromSubobject( vDTE_c, "ER_RelLink", vEMD_p, "ER_RelLink_2", zPOS_AFTER );

      // Make sure the TablRecKey entity exists.
      if ( CheckExistenceOfEntity( vDTE_c, "TE_TablRecKey" ) < zCURSOR_SET )
      {
         CreateTE_MetaEntity( vSubtask, vDTE_c, "TE_TablRecKey", zPOS_LAST );
         strcpy_s( szName, sizeof( szName ), "ID_" );
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vDTE_c, "TE_TablRec", "Name" );
         strcat_s( szName, sizeof( szName ), szTemp );
         ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE_c, szName, sizeof( szName ), 'A', "TE_TablRecKey", "Name", 0, pfnGenerateName );

         strcpy_s( szTemp, sizeof( szTemp ), "Implements " );
         GetStringFromAttribute( szTemp2, sizeof( szTemp2 ), vDTE_c, "TE_TablRec", "Name" );
         strcat_s( szTemp, sizeof( szTemp ), szTemp2 );
         SetAttributeFromString( vDTE_c, "TE_TablRecKey", "Desc", szTemp );
         SetAttributeFromString( vDTE_c, "TE_TablRecKey", "Category", "4" );
      }

      // Build relationship from the foreign key just created back to the
      // column on the parent that is the source of the value for the foreign key.
      IncludeSubobjectFromSubobject( vDTE_p, "TE_FieldDataRelAsFK", vDTE_c, "TE_FieldDataRel", zPOS_AFTER );

      // If the foreign key we just added is part of the Identifier (key),
      // then remove the corresponding TempRelKey entry and include the
      // TE_FieldDataRel entry just added as a TE_FieldDataRelKey entry.
      // We will identify the TempRelKey entity by both Entity ZKey and RelLink ZKey.
      GetIntegerFromAttribute( &lEntityZKey, vEMD_p, "ER_Entity_2", "ZKey" );
      GetIntegerFromAttribute( &lRelZKey, vEMD_p, "ER_RelLink_2", "ZKey" );
      for ( RESULT = SetCursorFirstEntity( vDTE_c, "TempRelKey", 0 );
            RESULT > zCURSOR_UNCHANGED;
            RESULT = SetCursorNextEntity( vDTE_c, "TempRelKey", 0 ) )
      {
         if ( CompareAttributeToInteger( vDTE_c, "TempRelKey", "EntityZKey", lEntityZKey ) == 0 &&
              CompareAttributeToInteger( vDTE_c, "TempRelKey", "RelZKey", lRelZKey ) == 0 )
         {
            // Add the foreign key just created as part of the identifier for the child entity.
            SetCursorLastEntity( vDTE_c, "TE_FieldDataRelKey", 0 );

            SetAttributeFromString( vDTE_c, "TE_FieldDataRel", "Key", "Y" );
            IncludeSubobjectFromSubobject( vDTE_c, "TE_FieldDataRelKey", vDTE_c, "TE_FieldDataRel", zPOS_AFTER );

            DeleteEntity( vDTE_c, "TempRelKey", zREPOS_NONE );
            break;
         }
      }

   }
//IssueError( vDTE_c, 0,0, "End of DataRel loop" );

   if ( vDBH_Data )
      DropView( vDBH_Data );

   count++;
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_ImplementNET_RelLink( zVIEW    vSubtask,
                                  zVIEW    vDTE,
                                  zVIEW    vEMD,
                                  zVIEW    vDTE_p,
                                  zVIEW    vEMD_p,
                                  PFNGENERATENAME pfnGenerateName )
{
   zCHAR    szOwner[ 255 ];
   zCHAR    szMember[ 255 ];
   zCHAR    szSet[ 65 ];
   zCHAR    szDesc[ 255 ];
   zCHAR    szTemp[ 100 ];

   CreateTE_MetaEntity( vSubtask, vDTE, "TE_FieldDataRel", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vDTE, "ER_RelLink", vEMD_p, "ER_RelLink_2", zPOS_AFTER );
   SetAttributeFromString( vDTE, "TE_FieldDataRel", "DataOrRelfieldOrSet", "S" );

   GetStringFromAttribute( szOwner, sizeof( szOwner ), vDTE_p, "TE_TablRec", "Name" );
   GetStringFromAttribute( szMember, sizeof( szMember ), vDTE, "TE_TablRec", "Name" );
   GetStringFromAttribute( szSet, sizeof( szSet ), vEMD_p, "ER_RelLink_2", "Name" );
   sprintf_s( szDesc, sizeof( szDesc ), "%s_%s_%s", szOwner, szSet, szMember );

   // Generate a unique name for sets.  We need to make sure that the set name is unique across the entire DBMS.
   ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE, szDesc, sizeof( szDesc ), 'S', "TE_FieldDataRel", "Name", "TE_DBMS_Source", pfnGenerateName );

   GetStringFromAttribute( szDesc, sizeof( szDesc ), vDTE_p, "TE_TablRec", "Name" );
   strcat_s( szDesc, sizeof( szDesc ), "(" );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD_p, "ER_RelLink_2", "Name" );
   strcat_s( szDesc, sizeof( szDesc ), szTemp );
   strcat_s( szDesc, sizeof( szDesc ), " [" );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD, "ER_RelLink_2", "CardMax" );
   strcat_s( szDesc, sizeof( szDesc ), szTemp );
   strcat_s( szDesc, sizeof( szDesc ), ":" );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD_p, "ER_RelLink_2", "CardMax" );
   strcat_s( szDesc, sizeof( szDesc ), szTemp );
   strcat_s( szDesc, sizeof( szDesc ), "] ) " );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vDTE, "TE_TablRec", "Name" );
   strcat_s( szDesc, sizeof( szDesc ), szTemp );
   SetAttributeFromString( vDTE, "TE_FieldDataRel", "Desc", szDesc );
   return( 0 );
}

/*    the _new view is set upon return to the created TE_TablRec that is
      used as the intersection record to handle the M-M relationship   */
zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_CreateRelTablRec( zVIEW    vSubtask,
                              zVIEW    vDTE1,
                              zVIEW    vEMD1,
                              zVIEW    vDTE2,
                              zVIEW    vEMD2,
                              zVIEW    vDTE_new,
                              PFNGENERATENAME pfnGenerateName )
{
   zCHAR    szDesc[ 255 ];
   zCHAR    szTemp[ 255 ];

   SetCursorLastEntity( vDTE_new, "TE_TablRec", "" );
   CreateTE_MetaEntity( vSubtask, vDTE_new, "TE_TablRec", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vDTE_new, "ER_RelType", vEMD1, "ER_RelType", zPOS_AFTER );

   GetStringFromAttribute( szDesc, sizeof( szDesc ), vDTE2, "TE_TablRec", "Name" );
   strcat_s( szDesc, sizeof( szDesc ), "(" );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD2, "ER_RelLink_2", "Name" );
   strcat_s( szDesc, sizeof( szDesc ), szTemp );
   strcat_s( szDesc, sizeof( szDesc ), " [" );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD1, "ER_RelLink_2", "CardMax" );
   strcat_s( szDesc, sizeof( szDesc ), szTemp );
   strcat_s( szDesc, sizeof( szDesc ), ":" );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD2, "ER_RelLink_2", "CardMax" );
   strcat_s( szDesc, sizeof( szDesc ), szTemp );
   strcat_s( szDesc, sizeof( szDesc ), "] ) " );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vDTE1, "TE_TablRec", "Name" );
   strcat_s( szDesc, sizeof( szDesc ), szTemp );
   SetAttributeFromString( vDTE_new, "TE_TablRec", "Desc", szDesc );

   // If a table name was specified in ER, use it. Otherwise generate a name.
   GetStringFromAttribute( szDesc, sizeof( szDesc ), vEMD2, "ER_RelType", "TE_TableName" );
   if ( *szDesc == 0 )
   {
      strcpy_s( szDesc, sizeof( szDesc ), "Mm_" );
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vDTE2, "TE_TablRec", "Name" );
      /* Change entity-name to all upper-case. It is not longer changes while initializing Tables */
      SysTranslateString( szTemp, 'U' );
      strcat_s( szDesc, sizeof( szDesc ), szTemp );
      strcat_s( szDesc, sizeof( szDesc ), "_" );
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vEMD2, "ER_RelLink_2", "Name" );
      strcat_s( szDesc, sizeof( szDesc ), szTemp );
      strcat_s( szDesc, sizeof( szDesc ), "_" );
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vDTE1, "TE_TablRec", "Name" );
      // Change entity-name to all upper-case. It is not longer changes while initializing Tables
      SysTranslateString( szTemp, 'U' );
      strcat_s( szDesc, sizeof( szDesc ), szTemp );
   }

   ofnTZTENVRO_GenerateUniqueName( vSubtask, vDTE_new, szDesc, sizeof( szDesc ), 'E', "TE_TablRec", "Name", 0, pfnGenerateName );

   SetAttributeFromString( vDTE_new, "TE_TablRec", "DataOrRel", "R" );

   return( 0 );
}

zVOID LOCALOPER
ofnTZTENVRO_GenerateUniqueName( zVIEW  vSubtask,
                                zVIEW  vDTE,
                                zPCHAR pszName,
                                zLONG  lMaxNameLth,
                                zCHAR  cType,
                                zPCHAR pszEntity,
                                zPCHAR pszAttribute,
                                zPCHAR pszScopingEntity,
                                PFNGENERATENAME pfnGenerateName )
{
   zVIEW  vDTE2;
   zSHORT RESULT;
   zCHAR  szSironDB_Type[ 2 ];
// zCHAR  szCreateUnderscore[ 2 ];

   // Generate unique name only for databases, not for Siron Flat Files
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
   if ( *szSironDB_Type != 'F' )
   {
      //GetStringFromAttribute( szCreateUnderscore, sizeof( szCreateUnderscore ), vDTE, "TE_DBMS_Source", "TranslateNamesToLowerUnderscore");
      //if ( szCreateUnderscore[ 0 ] == 'Y' )
      //   TranslateToUnderscoreCase( pszName, pszName, cType );

      CreateViewFromViewForTask( &vDTE2, vDTE, 0 );
      (*pfnGenerateName) ( vDTE, pszName, lMaxNameLth, cType );

      RESULT = SetCursorFirstEntityByString( vDTE2, pszEntity, pszAttribute, pszName, pszScopingEntity );
      while ( RESULT >= zCURSOR_SET )
      {
         (*pfnGenerateName) ( vDTE, pszName, lMaxNameLth, 'X' );

         RESULT = SetCursorFirstEntityByString( vDTE2, pszEntity, pszAttribute, pszName, pszScopingEntity );
      }

      DropView( vDTE2 );
   }
   SetAttributeFromString( vDTE, pszEntity, pszAttribute, pszName );

   return;
}

//./ ADD NAME=TranslateToUnderscoreCase
/////////////////////////////////////////////////////////////////////////////
//
//   ENTRY:    TranslateToUnderscoreCase
//
//   PROTOTYPE:
//        void OPERATION
//        TranslateToUnderscoreCase( zPCHAR pchSrc,
//                                   zPCHAR pchTarget,
//                                   zSHORT targetLth )
//
//   PURPOSE:  Converts a string from Camel Casing to underscores.  E.g.:
//
//                  ThisIsCamelCase to this_is_camel_case
//                  ThisIsHTTPCase to this_is_http_case
//                  ThisIsHTTP to this_is_http
//                  ThisIsATest to this_is_a_test
//                  nocamel_case to nocamel_case
//                  noLeadingUpper to no_leading_upper
//
//   RETURNS: 0 or zCALL_ERROR
//
//////////////////////////////////////////////////////////////////////////////

//TranslateToUnderscoreCase( zPCHAR pchSrc, zPCHAR pchTarget, zSHORT targetLth )
zSHORT LOCALOPER
TranslateToUnderscoreCase( zPCHAR pchSrc, zPCHAR pchTarget, zCHAR  cMetaType )
{
   zVIEW  vDBH_Data = 0;
   //zCHAR  szDBH_DataObjectName[ zZEIDON_NAME_LTH + 1 ];
   //zCHAR  szCreateUnderscore[ 2 ];
   zLONG  l;
   zPCHAR targetEnd; // = pchTarget + targetLth - 1; // Point to last char in target buffer.
   zPCHAR original = pchSrc;
   zPCHAR p;
   zCHAR  sz[ 256 ];
   static zSHORT nMaxLth;      // Used to keep track of duplicate names.

   switch ( cMetaType )
   {
      case 'E':      // Meta type is Entity

         // Try getting the max length from the DBH-Specific OI.  If one is not specified use the default length.
         if ( vDBH_Data && GetIntegerFromAttribute( &l, vDBH_Data, "ODBC", "MaxTableNameLength" ) != -1 )
            nMaxLth = (zSHORT) l;
         else
            nMaxLth = MAX_TABLENAME_LTH;

         break;

      case 'A':      // Meta type is Attribute

         // Try getting the max length from the DBH-Specific OI.  If one is not specified use the default length.
         if ( vDBH_Data && GetIntegerFromAttribute( &l, vDBH_Data, "ODBC", "MaxColumnNameLength" ) != -1 )
            nMaxLth = (zSHORT) l;
         else
            nMaxLth = MAX_COLUMNNAME_LTH;

         break;
   }

   if ( nMaxLth >= BUFF_SIZE )
      nMaxLth = BUFF_SIZE - 1;

   targetEnd = pchTarget + nMaxLth - 1;

   // Defensive programming...
   *targetEnd = 0;

   if ( zstrlen( pchSrc ) >= (zULONG) nMaxLth )
      return zCALL_ERROR;

   strcpy_s( sz, sizeof( sz ), pchSrc );

   //p = pchTarget;
   p = (zPCHAR) &sz;

   // Copy first char to target.
   *p++ = *pchSrc++;
   while ( *pchSrc != 0 )
   {
      // Do we have enough space to add two more chars?
      if ( p + 2 > targetEnd )
         return -16;

      if ( *pchSrc >= 'A' && *pchSrc <= 'Z' )
      {
         // We have a capital letter.  Is previous letter upper?
         if ( *(p-1) < 'A' || *(p-1) > 'Z' )
         {
            // No, so copy _.
            *p++ = '_';
         }
         else
         {
            // Previous letter is also upper.  Insert _ if next char is lower.
            if ( ( *(pchSrc+1) < 'A' || *(pchSrc+1) > 'Z' ) && *(pchSrc+1) != 0 )
               *p++ = '_';
         }
      }
      *p++ = *pchSrc++;
   }
//
   *p++ = 0;

   // Now convert to all lower.
   SysTranslateString( sz, 'L' );
   strcpy_s( pchTarget, nMaxLth, sz );

   return 0;
}


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZTENVRO_GetUpdViewForDTE
//
// PURPOSE:   Get access to TE View for Update.  Activate if not loaded
//            and get at application level if loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_GetUpdViewForDTE_P( zVIEW vSubtask, zPVIEW pvTZTENVRO )
{
   zVIEW  vDTE;
   zVIEW  vLPL;
   zVIEW  vCM_List;
   zSHORT nRC;
   zCHAR  sz[ 255 ];

   *pvTZTENVRO = 0;

   nRC = RetrieveViewForMetaList( vSubtask, &vCM_List, zSOURCE_DTE_META );

#if 1
   // This code was added by DonC on 4/21/96 to handle single TE.
   // It replaced the code below.
   // Note that it will activate the TE even if it is only a REFER view.

   nRC = CheckExistenceOfEntity( vCM_List, "W_MetaDef" );
   if ( nRC < zCURSOR_SET )
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DTE_META );

   nRC = CheckExistenceOfEntity( vCM_List, "W_MetaDef" );
   if ( nRC >= zCURSOR_SET )
   {
      nRC = ActivateMetaOI( vSubtask, &vDTE, vCM_List, zSOURCE_DTE_META, zSINGLE | zACTIVATE_ROOTONLY );
      if ( nRC < 0 )
         return( -2 );
   }
   else
   {
      GetViewByName( &vLPL, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( vLPL )
         GetStringFromAttribute( sz, sizeof( sz ), vLPL, "LPLR", "Name" );

      ActivateEmptyMetaOI( vSubtask, &vDTE, zSOURCE_DTE_META, zSINGLE | zLEVEL_APPLICATION );
      if ( vDTE == 0 )
         return( -2 );

      CreateTE_MetaEntity( vSubtask, vDTE, "TE_DB_Environ", zPOS_AFTER );
      SetAttributeFromString( vDTE, "TE_DB_Environ", "Name", sz );
      SetAttributeFromAttribute( vDTE, "TE_DB_Environ", "Desc", vLPL, "LPLR", "Desc" );
      SetAttributeFromString( vDTE, "TE_DB_Environ", "Type", "P" );
   }

#else
   The following code was eliminated by DonC on 4/21/96 because it was set
   up for multiple TEs, which were are currently not using.
// Get LPL Name
   GetViewByName( &vLPL, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vLPL )
      GetStringFromAttribute( sz, sizeof( sz ), vLPL, "LPLR", "Name" );

   nRC = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name", sz, "" );
   if ( nRC < zCURSOR_SET )
   {
      TruncateName8( sz );
      nRC = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name", sz, "" );
   }

   if ( nRC < zCURSOR_SET )
   {  // search all TE's to find one that is the Pysical Model
      nRC = SetCursorFirstEntity( vCM_List, "W_MetaDef", "" );
      while ( nRC >= zCURSOR_SET )
      {
         ActivateMetaOI( vSubtask, &vDTE, vCM_List, zSOURCE_DTE_META,
                        zSINGLE | zACTIVATE_ROOTONLY );
         if ( vDTE &&
              CompareAttributeToString( vDTE, "TE_DB_Environ",
                         "Type", "P" ) == 0 )
         {
            DropMetaOI( vSubtask, vDTE );
            break;
         }

         DropMetaOI( vSubtask, vDTE );
         nRC = SetCursorNextEntity( vCM_List, "W_MetaDef", "" );
      }
   }

   if ( nRC < zCURSOR_SET )
   {  // initialize an empty TE skeleton for LPLR
      nRC = oTZTENVRO_GetRefViewForDTE_P( vSubtask, pvTZTENVRO );
      if ( nRC >= 0 )
         return( -1 );  // return the REFER view instead.

      ActivateEmptyMetaOI( vSubtask, &vDTE, zSOURCE_DTE_META,
                     zSINGLE | zLEVEL_APPLICATION );
      if ( !vDTE )
         return( -2 );

      CreateTE_MetaEntity( vSubtask, vDTE, "TE_DB_Environ", zPOS_AFTER );
      SetAttributeFromString( vDTE, "TE_DB_Environ", "Name", sz );
      SetAttributeFromAttribute( vDTE, "TE_DB_Environ", "Desc",
                                 vLPL, "LPLR", "Desc" );
      SetAttributeFromString( vDTE, "TE_DB_Environ", "Type", "P" );
   }
   else
   {
      nRC = ActivateMetaOI( vSubtask, &vDTE, vCM_List, zSOURCE_DTE_META,
                            zSINGLE | zLEVEL_APPLICATION );
      if ( nRC != 1 )
         return( -2 );
   }
#endif

   *pvTZTENVRO = vDTE;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZTENVRO_GetRefViewForDTE_P
//
// PURPOSE:   Get access to TE View for Reference.  Activate if not loaded
//            and get at application level if loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_GetRefViewForDTE_P( zVIEW vSubtask, zPVIEW pvTZTENVRO )
{
   zVIEW  vDTE;
   zVIEW  vLPL;
   zVIEW  vCM_List;
   zSHORT nRC;
   zCHAR  sz[ 255 ];

   *pvTZTENVRO = 0;

   nRC = RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DTE_META );

// Get LPL Name
   GetViewByName( &vLPL, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vLPL )
      GetStringFromAttribute( sz, sizeof( sz ), vLPL, "LPLR", "Name" );

   nRC = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name", sz, "" );
   if ( nRC < zCURSOR_SET )
   {
      TruncateName8( sz );
      nRC = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name", sz, "" );
   }

   if ( nRC < zCURSOR_SET )
   {  // search all TE's to find one that is the Pysical Model
      nRC = SetCursorFirstEntity( vCM_List, "W_MetaDef", "" );
      while ( nRC >= zCURSOR_SET )
      {
         ActivateMetaOI( vSubtask, &vDTE, vCM_List, zREFER_DTE_META, zSINGLE | zACTIVATE_ROOTONLY );
         if ( vDTE && CompareAttributeToString( vDTE, "TE_DB_Environ", "Type", "P" ) == 0 )
         {
            DropMetaOI( vSubtask, vDTE );
            break;
         }

         DropMetaOI( vSubtask, vDTE );
         nRC = SetCursorNextEntity( vCM_List, "W_MetaDef", "" );
      }
   }

   if ( nRC < zCURSOR_SET )
   {  // can't give a reference view to non-existing model
      return( -2 );
   }
   else
   {
      ActivateMetaOI( vSubtask, &vDTE, vCM_List, zREFER_DTE_META, zSINGLE | zLEVEL_APPLICATION );
      if ( !vDTE )
         return( -2 );
   }

   *pvTZTENVRO = vDTE;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: oTZTENVRO_WriteErrorMsgToList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
oTZTENVRO_WriteErrorMsgToList( zVIEW  vSubtask,
                               zPCHAR szEntityName,
                               zPCHAR szErrorMsg )
{
   zVIEW  vErrorList;

   if ( GetViewByName( &vErrorList, "TZTEERR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vErrorList, "TZTEERR", vSubtask, zMULTIPLE );
      SetNameForView( vErrorList, "TZTEERR", vSubtask, zLEVEL_TASK );
   }

   CreateEntity( vErrorList, "ErrorMsg", zPOS_AFTER );
   SetAttributeFromString( vErrorList, "ErrorMsg", "EntityName", szEntityName );
   SetAttributeFromString( vErrorList, "ErrorMsg", "MessageText", szErrorMsg );

   return( 0 );
} // oTZTENVRO_WriteErrorMsgToList


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: oTZTENVRO_SwitchForeignKeys
//
// Switch foreign keys, so that the target foreign key implements both relationships
//  and the source foreign key gets deleted.
//
// Algorithm written by BL, originally in tzersapd.c, now used by tzteupdd, too.
//
// vDTE is pointing to the TE_FieldDataRel which is the foreign key beeing
//  switched off (old).
// vDTE_Copy1 is pointing to the TE_FieldDataRel which is the target of
//  the switch (new). It will implement more than 1 ER_Rellink then.
//
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_SwitchForeignKeys( zVIEW  vDTE,
                             zVIEW  vDTE_Copy1 )
{
   fnUpdateKeys( vDTE, vDTE_Copy1 );

   // As the Target FK is for more than 1 relationship, we want the TE_FieldDataRel_PK to be the Base Key (having an ER attribute).
   fnReferenceBaseKey( vDTE_Copy1 );
   if ( SetCursorFirstEntityByAttr( vDTE_Copy1, "ER_RelLink", "ZKey", vDTE, "ER_RelLink", "ZKey", "" ) < zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vDTE_Copy1, "ER_RelLink", vDTE, "ER_RelLink", zPOS_AFTER );
   }

   ExcludeEntity( vDTE, "ER_RelLink", zREPOS_NONE );

   // set new field sequence
   oTZTENVRO_DelFieldFromSequence( vDTE );

   // Delete the original foreign key.
   DeleteEntity( vDTE, "TE_FieldDataRel", zREPOS_NONE );

   return( 0 );

} // oTZTENVRO_SwitchForeignKeys


// vDTE is pointing to the TE_FieldDataRel FK to be switched off (old).  vDTE_Copy1 is pointing to the TE_FieldDataRel FK(new), which must
// to be added to the key, if not already there.
static zSHORT
fnUpdateKeys( zVIEW  vDTE, zVIEW  vDTE_Copy1 )
{
   zVIEW   vDTE_Copy2;
   zSHORT  nRC;

   CreateViewFromViewForTask( &vDTE_Copy2, vDTE, 0 );

   // If the original foreign key was a part of the identifier, replace the original TE_FieldDataRelKey entry with
   // the new one and include all of the foreign keys referencing it to the new entry.
   if ( SetCursorFirstEntityByEntityCsr( vDTE, "TE_FieldDataRelKey", vDTE, "TE_FieldDataRel", "TE_TablRec" ) >= zCURSOR_SET )
   {
      // If the new foreign key is not already a part of the Identifier, create it.
      if ( SetCursorFirstEntityByEntityCsr( vDTE_Copy2, "TE_FieldDataRelKey", vDTE_Copy1, "TE_FieldDataRel", "TE_TablRec" ) < zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vDTE_Copy2, "TE_FieldDataRelKey", vDTE_Copy1, "TE_FieldDataRel", zPOS_BEFORE );
      }

       // Switch any foreign keys that used the original.
      for ( nRC = SetCursorFirstEntity( vDTE, "TE_FieldDataRelAsFK", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDTE, "TE_FieldDataRelAsFK", 0 ) )
      {
         IncludeSubobjectFromSubobject( vDTE_Copy2, "TE_FieldDataRelAsFK", vDTE,  "TE_FieldDataRelAsFK", zPOS_AFTER );
         ExcludeEntity( vDTE, "TE_FieldDataRelAsFK", zREPOS_NONE );
      }
   }

   DropView( vDTE_Copy2 );
   return( 0 );

} // fnUpdateKeys

// vDTE is pointing to a Foreign Key (TE_FileDataRel) which should have the Base Key (ER attribute) as TE_FileDataRel_PK.
// If necessary, we have to exclude the FK under the "old" key and include it under the Base Key.
static void
fnReferenceBaseKey( zVIEW vDTE )
{
   zVIEW vDTE2;
   zLONG lZKey, lCurrentZKey, lBaseZKey;
   zSHORT nRC;

   if ( CompareAttributeToString( vDTE,
           "TE_FieldDataRel_PK", "DataOrRelfieldOrSet", "D" ) == 0 )
      return; // we are already on Base Key

   CreateViewFromViewForTask( &vDTE2, vDTE, 0 );
   // get the ZKey of current FK.
   GetIntegerFromAttribute( &lCurrentZKey, vDTE2, "TE_FieldDataRel", "ZKey" );
   // let vDTE2 point to the Base Key TE_FieldDataRel
   GetIntegerFromAttribute( &lZKey, vDTE2, "TE_FieldDataRel_PK", "ZKey" );
   fnFK_SearchBaseKey( vDTE2, lZKey );
   // get the ZKey of the Base Key
   GetIntegerFromAttribute( &lBaseZKey, vDTE2, "TE_FieldDataRel", "ZKey" );
   // Exclude the current FK, wherever it occurs as FK under a PK
   nRC = SetCursorFirstEntityByInteger( vDTE2, "TE_FieldDataRelAsFK", "ZKey", lCurrentZKey, "TE_DBMS_Source" );
   while ( nRC >= zCURSOR_SET )
   {
      ExcludeEntity( vDTE2, "TE_FieldDataRelAsFK", zREPOS_NONE );
      nRC = SetCursorNextEntityByInteger( vDTE2, "TE_FieldDataRelAsFK", "ZKey", lCurrentZKey, "TE_DBMS_Source" );
   }
   // include the current FK under the Base Key
   nRC = SetCursorFirstEntityByInteger( vDTE2, "TE_FieldDataRelKey", "ZKey", lBaseZKey, "TE_DBMS_Source" );
   // is it possible that the FK is already here?
   //  probably not, but to be sure ...
   nRC = SetCursorFirstEntityByEntityCsr( vDTE2, "TE_FieldDataRelAsFK", vDTE, "TE_FieldDataRel", 0 );
   if ( nRC < zCURSOR_SET )
   {
      // not there, include it
      nRC = SetCursorLastEntity( vDTE2, "TE_FieldDataRelAsFK", 0 );
      IncludeSubobjectFromSubobject( vDTE2, "TE_FieldDataRelAsFK", vDTE, "TE_FieldDataRel", zPOS_AFTER );
   }
   DropView( vDTE2 );

} // fnReferenceBaseKey

// the vDTE_Copy is positioned on the TE_FielDataRel, which is the Base Key
//  for a given Foreign Key (Input lZKey).
static zSHORT
fnFK_SearchBaseKey( zVIEW  vDTE_BaseKey, zLONG  lZKey )
{
   zSHORT  nRC;

   nRC = SetCursorFirstEntityByInteger( vDTE_BaseKey, "TE_FieldDataRel", "ZKey", lZKey, "TE_DBMS_Source" );

   while ( nRC >= zCURSOR_SET &&
           CompareAttributeToString( vDTE_BaseKey, "TE_FieldDataRel", "DataOrRelfieldOrSet", "D" ) != 0 )
   {
      GetIntegerFromAttribute( &lZKey, vDTE_BaseKey, "TE_FieldDataRel_PK", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vDTE_BaseKey, "TE_FieldDataRel", "ZKey", lZKey, "TE_DBMS_Source" );
   }

   return( 0 );
} // fnFK_SearchBaseKey

///////////////////////////// end of file  //////////////////////////////////
