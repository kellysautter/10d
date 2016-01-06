/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadw7aw.c - Zeidon application operations
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
// LAST MESSAGE ID: AD10409
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2000.10.19    SR   Z2000 Size of path variables
   Modified size of string szFileSpec and szFileName in function
   ofnTZWDLGSO_CreateSourceCode because size of attributes ExecDir, MetaSrcDir
   and PgmSrcDir has been changed to 254 in datamodel.

  Length of Paths
  Changed size of, because in future it will be allowed to enter
  a path in the length of 254 characters.

1999.12.08  DC    Z10
   Corrected handling of memory length around SysAllocMemory at end of
   CreateSourceCode. Also moved the SysFreeMemory( selMemory1 ) statement
   so that it was only executed when selMemory1 was allocated.
   DKS added some minor formatting changes, such as using double slashes
   for comments.

1999.19.12  DKS   Z10   TB829
   Added 1 byte for total Template length.

1999.09.13  BL ALL      Autodesign
   line 1841, Fix for FH

1999.09.10  BL ALL      Autodesign
   insert SysAllocMemory for new AutoDesigner

1999.09.08  BL ALL      Autodesign
   update szTemplate[6000] to szTemplate[60000] in Operation
   ofnTZWDLGSO_CreateSourceCode

1999.08.19  DC 10a + 9j
   Modified CreateSourceCode for DeleteObjectEntity operation to not
   generate the delete of the entry in the list when there is not list.
   (When the Autodesign style is not list.)

1999.08.16  DC 10a
   Modified termination of CreateSourceCode so that last slash in comment of
   first generated operation would not be lost.

1999.05.01  DC 10a+9j  Autodesign
   Made many changes to support entries to user customization routines.

*** Old Order

97/04/14    DonC
   Added CheckExistenceOfEntity check on I_LOD_Entity to skip code when
   the operation is ListSub for Case 5.

   Modified CreateSourceCode to check for existence of entities before
   setting szIncludeVOR and szIncludeObject. The settings were not valid for
   main list.

98/01/14    DonC
   Modified AddQualToWhere to handle situation where ER_Attribute is null.

98/03/1998  DonC
   Modified ofnTZWDLGSO_CreateSourceCode for SelectSubObj... to not generate
   target view if it is the same as the list view.

01.07.1998  DonC
   Modified code generation of SelectSubObj... so that it would correctly
   handle the case where the ListViewObjRef and the IncludeViewObjRef are
   the same view.

02.07.1998  DonC
   Modified code generation to eliminate erroneous Zeidon core errors on
   some Case 6 situations.

03.07.1998  DonC
   Modified generation of ListSubObj operation for Case 4 to eliminate
   statements for activating the list object if the include views are
   the same as the view of the UIS itself.

28.07.1998  DonC
   Modified generation of ActivateMainList so that it would reposition on
   the list view according to the last update view, if there is one. (QS 56)

29.07.1998  DonC
   Modified AddQualToWhere to handle Identifier FactTypes that are
   relationships.

1999/02/11  DC
   Modified CreateSourceCode to initialize lpMemory to null because the
   SysReadFile wasn't initializing the area when the file being read was
   empty. Also Modified UserPostVariableExit to give 0 return code when
   no exit exists. (TB 501 and TB 502)

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"

zOPER_EXPORT zPVOID OPERATION
ofnTZWDLGSO_GetUserExitAddress( zVIEW vView,
                                zPCHAR szEntryName );

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION  CreateCodeLine (CL)
//
// PURPOSE:  This routine concatenates up to 5 strings together to create
//           a line of code in the target string.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CL( zPCHAR szTargetString,
    zPCHAR szParm1,
    zPCHAR szParm2,
    zPCHAR szParm3,
    zPCHAR szParm4,
    zPCHAR szParm5 )
{
   strcat_s( szTargetString, 60000, szParm1 );
   strcat_s( szTargetString, 60000, szParm2 );
   strcat_s( szTargetString, 60000, szParm3 );
   strcat_s( szTargetString, 60000, szParm4 );
   strcat_s( szTargetString, 60000, szParm5 );
   strcat_s( szTargetString, 60000, "\r\n" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AddQualToWhereSub( zPCHAR szTemplate,
                   zVIEW  vObject,
                   zPCHAR szObjectName,
                   zPCHAR szListVOR,
                   zPLONG plLoopCount,
                   zVIEW  vERD )
{
   // Recursively process each FactType, following the recursive
   // relationship path down as many levels as necessary.

   zVIEW  vTempERD;
   zCHAR  szWhereEntityName[ 33 ];
   zCHAR  szWhereAttributeName[ 33 ];
   zCHAR  szMsg[ 100 ];
   zSHORT nRC;

   // Make sure there is at least one FactType under Identifier.
   if ( CheckExistenceOfEntity( vERD, "ER_FactType" ) < zCURSOR_SET )
   {
      GetStringFromAttribute( szWhereEntityName, sizeof( szWhereEntityName ), vERD, "ER_Entity", "Name" );
      strcpy_s( szMsg, sizeof( szMsg ), "Entity '" );
      strcat_s( szMsg, sizeof( szMsg ), szWhereEntityName );
      strcat_s( szMsg, sizeof( szMsg ), "' has no Identifier entries. " );
      MessageSend( vERD, "AD10409", "Autodesigner",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   CreateViewFromViewForTask( &vTempERD, vERD, 0 );

   // Process a WHERE qual for each FactType.
   for ( nRC = SetCursorFirstEntity( vERD, "ER_FactType", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_FactType", 0 ) )
   {
      if ( CheckExistenceOfEntity( vERD, "ER_AttributeIdentifier" ) >= zCURSOR_SET )
      {
         // If we're not in first time through the loop, add the "AND" plus
         // the skip to new line.
         if ( *plLoopCount != 0 )
         {
            strcat_s( szTemplate, 60000, " AND" );
            strcat_s( szTemplate, 60000, "\r\n" );
         }

         // Process Type = Attribute
         // Position on corresponding ER_Entity and then on corresponding
         // LOD_Entity to get entity name.
         SetCursorFirstEntityByAttr( vTempERD, "ER_Attribute",           "ZKey",
                                     vERD,     "ER_AttributeIdentifier", "ZKey",
                                     "EntpER_Model" );
         SetCursorFirstEntityByAttr( vObject,  "ER_Entity", "ZKey",
                                     vTempERD, "ER_Entity", "ZKey", "LOD" );
         GetStringFromAttribute( szWhereAttributeName, sizeof( szWhereAttributeName ),
                                 vTempERD, "ER_Attribute", "Name" );
         GetStringFromAttribute( szWhereEntityName, sizeof( szWhereEntityName ),
                                 vObject, "LOD_Entity", "Name" );

         strcat_s( szTemplate, 60000, "            " );
         strcat_s( szTemplate, 60000, szObjectName );
         strcat_s( szTemplate, 60000, "." );
         strcat_s( szTemplate, 60000, szWhereEntityName );
         strcat_s( szTemplate, 60000, "." );
         strcat_s( szTemplate, 60000, szWhereAttributeName );
         strcat_s( szTemplate, 60000, "" );
         strcat_s( szTemplate, 60000, " = " );
         strcat_s( szTemplate, 60000, szListVOR );
         strcat_s( szTemplate, 60000, "." );
         strcat_s( szTemplate, 60000, szWhereEntityName );
         strcat_s( szTemplate, 60000, "." );
         strcat_s( szTemplate, 60000, szWhereAttributeName );

         (*plLoopCount)++;
      }

      if ( CheckExistenceOfEntity( vERD, "ER_RelLinkIdentifier" ) >= zCURSOR_SET )
      {
         // Process Type = Relationship
         // Position on ER_Entity which is target of relationship and then
         // call recursive subroutine to process the Identifier for that
         // entity.
         SetCursorFirstEntityByAttr( vTempERD, "ER_Entity",                  "ZKey",
                                     vERD,     "ER_Entity_Other_Identifier", "ZKey",
                                     "EntpER_Model" );

         AddQualToWhereSub( szTemplate, vObject, szObjectName,
                            szListVOR, plLoopCount, vTempERD );
      }
   }

   DropView( vTempERD );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION  AddQualToWhere
//
// PURPOSE:  This adds a multi-key qualification statement to a
//           a WHERE clause.  It is called when a detail object instance
//           is activated from a list.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
AddQualToWhere( zVIEW  vSubtask,
                zPCHAR szTemplate,
                zVIEW  vObject,
                zPCHAR szObjectName,
                zPCHAR szListVOR,
                zPCHAR szListEntityName )
{
   zVIEW  vERD;
   zVIEW  vTemp;
   zSHORT nRC;
   zLONG  lLoopCount;

   // Get the view to the ERD.
   RetrieveViewForMetaList( vSubtask, &vTemp, zREFER_ERD_META );
   nRC = ActivateMetaOI( vSubtask, &vERD, vObject, zREFER_ERD_META,
                         zSINGLE | zLEVEL_APPLICATION );
   SetNameForView( vERD, "*** FactType ERD", vSubtask, zLEVEL_TASK );

   // Position on the ERD entity that is the same as the root entity of the
   // LOD.
   SetCursorFirstEntityByAttr( vERD,    "ER_Entity", "ZKey",
                               vObject, "ER_Entity", "ZKey", 0 );

   lLoopCount = 0;

   // Call recursive subroutine that actually builds the components of the
   // WHERE.

   AddQualToWhereSub( szTemplate, vObject, szObjectName,
                      szListVOR, &lLoopCount, vERD );

   DropView( vERD );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION  UserPostVariableExit
//
// PURPOSE:  Call the User Exit routine UserPostVariableEntry
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UserPostVariableExit( zPCHAR szOperationName,
                      zPCHAR szTemplate,
                      zVIEW  vUserSpec,
                      zVIEW  vObject )
{
   zVIEW vUserSpecTemp;
   zVIEW vObjectTemp;
   zSHORT nRC;

   typedef  zSHORT (POPERATION PFN_USERPOSTVARIABLE) ( zVIEW, zPCHAR, zPCHAR, zVIEW );
   PFN_USERPOSTVARIABLE pfnUserPostVariableEntry;

   pfnUserPostVariableEntry = (PFN_USERPOSTVARIABLE)
            ofnTZWDLGSO_GetUserExitAddress( vUserSpec,
                                            "oTZADCSDO_UserPostVariableEntry" );
   if ( pfnUserPostVariableEntry != 0 )
   {
      CreateViewFromViewForTask( &vUserSpecTemp, vUserSpec, 0 );
      CreateViewFromViewForTask( &vObjectTemp, vObject, 0 );
      nRC = (*pfnUserPostVariableEntry) ( vUserSpecTemp,
                                          szOperationName,
                                          szTemplate,
                                          vObjectTemp );
      DropView( vUserSpecTemp);
      DropView( vObjectTemp);
   }
   else
      nRC = 0;

   return( nRC );
}


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION
// CreateSourceCode( zPCHAR )
//
// PURPOSE:  This routine searches the current editor code for a
//           given function call (szSourceCode).  If the function is
//           located in the code, then CreateSourceCode is exited.  If
//           the function is not located, then the function is created.
//           There are several different functions that can be created
//           in CreateSourceCode:
//
//           1. LoadWorkObj - This function loads in the transfer
//                            object specified in the auto design
//                            dialog spec.
//           2. ListAllSubObj - This function loads all the subobjects
//                              for a particular object structure into
//                              a listbox.
//           3. ListQualSubObj - This function loads all the subobjects
//                               for a particular object structure and
//                               within a certain qualification into a
//                               listbox. //// Probably no longer used
//           4. LoadObjForList - This function loads in a specific object
//                               instance and displays a particular
//                               subordinate entity in a listbox.
//           5. SelectSubObj   - This function includes specified subobject
//                               into the current OI.  This is done by
//                               either double clicking on an entry Integer the
//                               listbox or entering a key.
//           6. ActivateMainList - This function performs the Activate
//                               of the main list object.  It used to be in
//                               one of the .Hn files but has been moved here
//                               for more flexibility.
//           7. ListSubCase5 - This function Activates the List objects
//                             necessary for Case 5/6 includes.
//
//           8. UpdateExistingInstance - This function performs the Activate
//                                       of the main object instance from
//                                       position on the list object.
//
//           9. DeleteObjectEntityFromList - This function deletes an
//                                           entry from the list.
//
//          10. CreateNewInstance - This function creates a new instance
//                                  of the root entity.
//
//          11. DeleteInstance - This function deletes the OI from memory.
//
//          12. SaveInstance - This function commits the OI to the database.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateSourceCode( zVIEW  vSubtask,
                              zVIEW  vUserSpec,
                              zVIEW  vNewDialog,
                              zPCHAR szSourceCode )
{
   zVIEW vObject;
   zVIEW vObject2;
   zVIEW vCM_Obj;
   zVIEW vTempObj;
   zVIEW vTempUIS;
   zVIEW vLOD;
   zVIEW vTemp;
   zVIEW vTZADWWKO;
   zPCHAR lpMemory;
   zPCHAR lpMemory1;
   zLONG selMemory;
   zLONG selMemory1;
   int  nPatternLth;
   int  NotFound;
   long fh;
   int  nLth;
   int  ExitWhile;
   zSHORT nRC;
   zLONG lFindPos;
   zLONG TemplateLth;
   zLONG lBufferLth;
   zLONG lLth;
   zLONG lRC;
   zLONG lZKey;
   zLONG wSelect;
   zCHAR cLanguage;
   zCHAR cSubObjType;
   zCHAR szSubObjType[ 3 ];
   zCHAR szCardinality[ 15 ];
   zCHAR szTemplate[ 60000 ];
   zCHAR szIncludeObject[ 33 ];
   zCHAR szIncludeVOR[ 33 ];
   zCHAR szIncludeListVOR[ 33 ];
   zCHAR szObjectName[ 33 ];
   zCHAR szEntityName[ 33 ];
   zCHAR szParentEntityName[ 33 ];
   zCHAR szSubobjEntity[ 33 ];
   zCHAR szER_Entity[ 33 ];
   zCHAR szAttribName[ 33 ];
   zCHAR szListVOR[ 33 ];
   zCHAR szListObject[ 33 ];
   zCHAR szDialogName[ 33 ];
   zCHAR szEditboxName[ 33 ];
   zCHAR szListQualifier[ 256 ];
   zCHAR szSourceCodeName[ 50 ];
   zCHAR szFirstDataFieldName[ 10 ];
   zCHAR szObjectKeyName[ 256 ];
   zCHAR szWorkString[ 100 ];
   zCHAR szOperator[ 5 ];
   zCHAR szWindowName[ 33 ];
   zCHAR szFileName[ zBASE_FILENAME_LTH + 1 ];
   zCHAR szFileSpec[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR szCase[ 8 ];
   zCHAR szWkCase[ 8 ];
   zLONG lDelta1Table[ ASCIISIZE ];
   zLONG lDelta2Table[ MAXPATTERN ];
   zSHORT nMatchFlag;

// TraceLineS("IN", "CreateSourceCode W7" );

   cLanguage = 'Y';
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
   GetStringFromAttribute( szFirstDataFieldName, sizeof( szFirstDataFieldName ),
                           vUserSpec, "UIS_Entity", "FirstDataFieldName" );

   // Copy an entity name extension on the Operation name if the Operation
   // is one of the following:
   //                          ListAllSubObj
   //                          SelectSubObj
   strcpy_s( szSourceCodeName, sizeof( szSourceCodeName ), szSourceCode );
   if ( zstrcmp( szSourceCode, "ListAllSubObj" ) == 0 ||
        zstrcmp( szSourceCode, "SelectSubObj" ) == 0 )
   {
      strcat_s( szSourceCodeName, sizeof( szSourceCodeName ), szEntityName );
      szSourceCodeName[ 32 ] = 0;
   }

   // For ListSub of Case 5/6, create Operation Name from parent entity.
   if ( zstrcmp( szSourceCode, "ListSub" ) == 0 )
   {
      CreateViewFromViewForTask( &vTempUIS, vUserSpec, 0 );
      ResetViewFromSubobject( vTempUIS );
      GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vTempUIS, "LOD_Entity", "Name" );
      strcat_s( szSourceCodeName, sizeof( szSourceCodeName ), szWorkString );
      szSourceCodeName[ 32 ] = 0;
      DropView( vTempUIS );
   }

   // If the Source Code name is not found in the editor file, then when the
   // operation template is created, certain information must be known about the
   // Source Code (e.g. What window and action the Source Code is called from).
   GetStringFromAttribute( szWindowName, sizeof( szWindowName ), vNewDialog, "Window", "Tag" );

   // Get the currently selected directory.
   // THIS IS ASSUMING THAT THE SOURCE CODE AND THE .PWD ETC ARE IN
   // THE SAME DIRECTORY.  SEE IF THAT IS GOING TO CHANGE.
   GetViewByName( &vCM_Obj, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &vTempObj, "TZCMWKSO", vCM_Obj, zLEVEL_SUBTASK );

   // Borrow szFileName to permit conversion of environment strings.
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vTempObj, "LPLR", "PgmSrcDir" );
   if ( szFileName[ 0 ] == 0 )
   {
      GetStringFromAttribute( szFileName, sizeof( szFileName ), vTempObj, "LPLR", "MetaSrcDir" );
   }

   SysConvertEnvironmentString( szFileSpec, sizeof( szFileSpec ), szFileName );
   nLth = zstrlen( szFileSpec );
   if ( nLth > 0 )
   {
      if ( szFileSpec[ nLth - 1 ] != '\\' )
      {
         szFileSpec[ nLth++ ] = '\\';
         szFileSpec[ nLth ] = 0;
      }
   }

   // Get the name of the file to edit.
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vNewDialog, "Dialog", "DLL_Name" );

   // If EXE_NAME is blank then the file name is the wdod name.
   if ( szFileName[ 0 ] == 0 )
      GetStringFromAttribute( szFileName, sizeof( szFileName ), vNewDialog, "Dialog", "Tag" );

   strcat_s( szFileName, sizeof( szFileName ), ".VML" );
   strcat_s( szFileSpec, sizeof( szFileSpec ), szFileName );

   fh = SysOpenFile( vUserSpec, szFileSpec, COREFILE_EXIST );

   if ( fh == -1 )
   {
      fh = SysOpenFile( vUserSpec, szFileSpec, COREFILE_CREATE | COREFILE_UPDATE );
   }
   else
   {
      fh = SysOpenFile( vUserSpec, szFileSpec, COREFILE_UPDATE );
   }

   /* if fh is -1, then the file must be in use */
   if ( fh == -1 )
   {
      MessageSend( vSubtask, "AD10401", "Autodesigner",
                   "File is currently in use by another process",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   TraceLineS( "(DGC) Opened file = ", szFileSpec );

   // Find the length of the file.
#ifdef __WIN32__
   lLth = GetFileSize( (HANDLE) fh, 0 );
#else
   lLth = _llseek( (int) fh, 0L, 2 );
   _llseek( (int) fh, 0L, 0 );
#endif

// TraceLineI( "File length = ", lLth );

   // We don't think this extra 60000 is necessary ... but to be safe,
   // we will leave as is.  DKS/DC 1999/12/07
   selMemory = SysAllocMemory( (zCOREMEM) &lpMemory, lLth + 60000, 0,
                               zCOREMEM_ALLOC, 0 );
   *lpMemory = 0;  // Initialize to null.

   lRC = (zLONG) SysReadFile( vUserSpec, fh, lpMemory, (zUSHORT) lLth );

   // Read the file into the buffer.
   if ( lLth != lRC )
   {
      SysFreeMemory( selMemory );
      MessageSend( vSubtask, "AD10402", "Autodesigner",
                   szFileSpec,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SysCloseFile( vUserSpec, fh, 0 );
      return( -1 );
   }

   // If there is no text in this file, then add a line.
   if ( lLth < 1 )
   {
      strcpy_s( szTemplate, 60000, "\r\n" );
      lLth = zstrlen( szTemplate ) + 1;
   }
   else  // if the file length is greater than 0, remove the EOF
   {
      // Get rid of the EOF character.
      if ( lpMemory[ lLth - 1 ] == '\x1A' )
         lpMemory[ lLth - 1 ] = 0;
      else
         lLth++;
   }

   nPatternLth = zstrlen( szSourceCodeName );

   if ( nPatternLth > 1 )
   {
      // Builds a table to be used in qfind.  qfind is written to
      // ignore the delta tables when the length is less than 2.
      qbldelta( nPatternLth, szSourceCodeName, lDelta1Table, lDelta2Table );
   }

   ExitWhile = FALSE;
   wSelect = 0;

   // Find the operation name.
   while ( ExitWhile == FALSE )
   {
      lBufferLth = lLth - wSelect;
      // Find the search string in the buffer.
      lFindPos = qfind( nPatternLth,
                        szSourceCodeName,
                        lBufferLth,          // Buffer length
                        lpMemory + wSelect,  // Buffer text
                        lDelta1Table,
                        lDelta2Table );

      // If found, keep searching until you get the ')'.
      if ( lFindPos > -1 )
      {
         lFindPos = lFindPos + wSelect;
         wSelect = lFindPos + nPatternLth;

         // If found, see if it has the extension "( VIEW", because is
         // what an operation definition has.
         if ( zstrncmp( (lpMemory + wSelect), "( VIEW ", 7 ) == 0 )
         {
            ExitWhile = TRUE;
            NotFound  = FALSE;   // Operation is found
         }
      }
      else
      {
         // No more operations.
         NotFound = TRUE;
         ExitWhile = TRUE;
      }
   }

   // If the operation was not found, then create a prototype and
   // a template for the operation and put it in the editor text.
   if ( NotFound == TRUE )
   {
      GetStringFromAttribute( szDialogName, sizeof( szDialogName ), vNewDialog, "Dialog", "Tag" );
      if ( CheckExistenceOfEntity( vUserSpec, "UIS_Include" ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( szSubObjType, sizeof( szSubObjType ),
                                 vUserSpec, "UIS_Include", "SubObjectType" );
         cSubObjType = szSubObjType[ 0 ];
      }
      else
         cSubObjType = 0;

      // If the SubObjType is Case 4, then Reset back one UIS_Entity level
      // and get the name of the parent entity.
      if ( cSubObjType == '4' )
      {
         CreateViewFromViewForTask( &vTemp, vUserSpec, 0 );
         ResetViewFromSubobject( vTemp );
         GetStringFromAttribute( szParentEntityName, sizeof( szParentEntityName ),
                                 vTemp, "LOD_Entity", "Name" );
         DropView( vTemp );
      }

      GetStringFromAttribute( szWindowName, sizeof( szWindowName ), vNewDialog, "Window", "Tag" );
      CreateViewFromViewForTask( &vTempUIS, vUserSpec, 0 );
      ResetView( vTempUIS );
      GetStringFromAttribute( szObjectName, sizeof( szObjectName ), vTempUIS, "UIS_LOD", "Name" );
      DropView( vTempUIS );

      // Set up values for SelectSubObj...
      szIncludeListVOR[ 0 ] = 0;
      if ( CheckExistenceOfEntity( vUserSpec, "IncludeViewObjRef" ) >= zCURSOR_SET )
         GetStringFromAttribute( szIncludeVOR, sizeof( szIncludeVOR ), vUserSpec,
                                 "IncludeViewObjRef", "Name" );

      if ( CheckExistenceOfEntity( vUserSpec, "ListViewObjRef" ) >= zCURSOR_SET )
         GetStringFromAttribute( szIncludeListVOR, sizeof( szIncludeListVOR ), vUserSpec,
                                 "ListViewObjRef", "Name" );

      if ( CheckExistenceOfEntity( vUserSpec, "IncludeSubObjLOD" ) >= zCURSOR_SET )
         GetStringFromAttribute( szIncludeObject, sizeof( szIncludeObject ), vUserSpec,
                                 "IncludeSubObjLOD", "Name" );

      nRC = ActivateMetaOI_ByName( vSubtask, &vObject, 0,
                                   zREFER_LOD_META,
                                   zSINGLE | zLEVEL_APPLICATION,
                                   szObjectName, 0 );
      if ( nRC == -1 )
      {
         strcpy_s( szWorkString, sizeof( szWorkString ), "Activate failed on the Object - " );
         strcat_s( szWorkString, sizeof( szWorkString ), szIncludeObject );
         strcat_s( szWorkString, sizeof( szWorkString ), ", used in this style." );
         MessageSend( vSubtask, "AD10403", "Autodesigner",
                      szWorkString,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SysFreeMemory( selMemory );
         return( -1 );
      }

      // Get the ER_Entity name for this included entity in the user spec
      // LOD, and then find that ER_Entity in the LOD to be included, so
      // that we can get the LOD_Entity name for the included subobject.
      GetViewByName( &vLOD, "LOD_Object", vSubtask, zLEVEL_TASK );
      GetIntegerFromAttribute( &lZKey, vUserSpec, "LOD_Entity", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vLOD, "LOD_Entity", "ZKey", lZKey, 0 );
      GetStringFromAttribute( szER_Entity, sizeof( szER_Entity ), vLOD, "ER_Entity", "Name" );

      nRC = SetCursorFirstEntityByString( vObject, "ER_Entity",
                                          "Name", szER_Entity, "LOD" );

      if ( nRC < zCURSOR_SET )
         nRC = SetCursorFirstEntity( vUserSpec, "I_LOD_Attribute", "UIS_Entity" );

      if ( nRC != zCURSOR_NULL )
      {
         // Position on the entity in the LOD and then on the key attribute
         // for that entity.  Use that attribute name for szAttribName, which
         // will be used in the ACTIVATE qualifications.
         // If the Autodesign style is "General" (no list window), then
         // I_LOD_Entity does not exist, so use ER_Entity.  Otherwise, use
         // the I_LOD_Entity entry set up for the list.
         GetViewByName( &vTZADWWKO, "TZADWWKO", vSubtask, zLEVEL_TASK );
         lZKey = 0;
         if ( CompareAttributeToString( vTZADWWKO, "AutoDesignWork",
                                        "WDOD_Style", "G" ) == 0 )
            GetIntegerFromAttribute( &lZKey, vUserSpec, "ER_Entity", "ZKey" );
         else
         if ( CheckExistenceOfEntity( vUserSpec, "I_LOD_Entity" ) >= zCURSOR_SET )
            GetIntegerFromAttribute( &lZKey, vUserSpec, "I_LOD_Entity", "ZKey" );

         if ( lZKey )
         {
            SetCursorFirstEntityByInteger( vLOD, "LOD_Entity", "ZKey", lZKey, 0 );
            SetCursorFirstEntityByString( vLOD, "LOD_Attribute",
                                          "ParticipatesInKey", "Y", 0 );
            GetStringFromAttribute( szAttribName, sizeof( szAttribName ), vLOD,
                                    "ER_Attribute", "Name" );
         }
      }

      if ( CheckExistenceOfEntity( vUserSpec, "ListSubObjLOD" ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( szListObject, sizeof( szListObject ),
                                 vUserSpec, "ListSubObjLOD", "Name" );
      }

      if ( cSubObjType == 'K' )
      {
         strcpy_s( szListQualifier, sizeof( szListQualifier ), "szEntryField" );
      }
      else
      {
         if ( CheckExistenceOfEntity( vUserSpec, "ListViewObjRef" ) >= zCURSOR_SET )
         {
            GetStringFromAttribute( szListVOR, sizeof( szListVOR ),
                                    vUserSpec, "ListViewObjRef", "Name" );

            strcpy_s( szListQualifier, sizeof( szListQualifier ), szListVOR );
            strcat_s( szListQualifier, sizeof( szListQualifier ), "." );
            strcat_s( szListQualifier, sizeof( szListQualifier ), szEntityName );
            strcat_s( szListQualifier, sizeof( szListQualifier ), "." );
            strcat_s( szListQualifier, sizeof( szListQualifier ), szAttribName );
         }
         else
            szListVOR[ 0 ] = 0;
      }

      if ( cSubObjType == 'P' &&
           zstrcmp( szSourceCode, "ListQualSubObj" ) == 0 )
      {
         GetStringFromAttribute( szOperator, sizeof( szOperator ), vUserSpec, "UIS_Include", "SubObjectOperators" );
         if ( szOperator[ 0 ] == 0 )
            strcpy_s( szOperator, sizeof( szOperator ), "=" );

         strcpy_s( szObjectKeyName, sizeof( szObjectKeyName ), "" );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), szListVOR );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), "." );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), szEntityName );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), "." );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), szAttribName );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), " " );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), szOperator );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), " " );
         strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), "szEntryField" );
      // strcat_s( szObjectKeyName, sizeof( szObjectKeyName ), "\"" );
      }
      else
         strcpy_s( szObjectKeyName, sizeof( szObjectKeyName ), "\"\"" );

      /* Create an operation template. */
      /* Get the information from the editor object about where
         this source code is being called from(ex. from a button),
         and put that information into the operation template. */
      szTemplate[ 0 ] = 0;
      strcpy_s( szTemplate, 60000, " \x0D\x0A  \x0D\x0A"
                           "///////////////////////////////////////////////////////////////////////////// "
                           " \x0D\x0A \x0D\x0A   OPERATION: " );
      strcat_s( szTemplate, 60000, szSourceCodeName );
      strcat_s( szTemplate, 60000, " \x0D\x0A" );
      strcat_s( szTemplate, 60000, " \x0D\x0A" );
      strcat_s( szTemplate, 60000, "/////////////////////////////////////////////////////////////////////////////" );
      strcat_s( szTemplate, 60000, "\r\nDIALOG OPERATION \x0D\x0A" );
      strcat_s( szTemplate, 60000, szSourceCodeName );
      strcat_s( szTemplate, 60000, "( VIEW vSubtask )" );

      if ( zstrcmp( szSourceCode, "SelectSubObj" ) == 0 )
      {
         zCHAR szSetCursorObjectName[ 34 ];

         // First get the second object needed in the Activate to retrieve
         // the entity name from that object, which might be different than
         // the entity name in vObject.
         // In this case, we also recreate the szListQualifier string.
         nRC = ActivateMetaOI_ByName( vSubtask, &vObject2, 0,
                                      zREFER_LOD_META,
                                      zSINGLE | zLEVEL_APPLICATION,
                                      szIncludeObject, 0 );
         if ( nRC >= 0 )
         {
            GetIntegerFromAttribute( &lZKey, vObject, "ER_Entity", "ZKey" );
            SetCursorFirstEntityByInteger( vObject2, "ER_Entity", "ZKey",
                                           lZKey, "LOD" );
            GetStringFromAttribute( szSubobjEntity, sizeof( szSubobjEntity ), vObject2, "LOD_Entity", "Name" );
            strcpy_s( szListQualifier, sizeof( szListQualifier ), szListVOR );
            strcat_s( szListQualifier, sizeof( szListQualifier ), "." );
            strcat_s( szListQualifier, sizeof( szListQualifier ), szSubobjEntity );
            strcat_s( szListQualifier, sizeof( szListQualifier ), "." );
            strcat_s( szListQualifier, sizeof( szListQualifier ), szAttribName );
         }

         strcat_s( szTemplate, 60000, "\r\n" );

         // If the view for the include list is not the same as the view
         // for the include view itself, we will add the include view.
         if ( zstrcmp( szIncludeVOR, szIncludeListVOR ) != 0 )
         {
            strcat_s( szTemplate, 60000, "\r\n" );
            strcat_s( szTemplate, 60000, "   VIEW " );
            strcat_s( szTemplate, 60000, szIncludeVOR );
            strcat_s( szTemplate, 60000, " BASED ON LOD " );
            strcat_s( szTemplate, 60000, szIncludeObject );
         }

         // Now comes the VIEW statement that is the target of the include.
         // Only add it if it is differenct from the target.
         if ( zstrcmp( szObjectName, szListVOR ) != 0 )
         {
            strcat_s( szTemplate, 60000, "\r\n" );
            strcat_s( szTemplate, 60000, "   VIEW " );
            strcat_s( szTemplate, 60000, szObjectName );
            strcat_s( szTemplate, 60000, " REGISTERED AS " );
            strcat_s( szTemplate, 60000, szObjectName );
         }

         // Add the temporary view for the SET CURSOR statement.
         strcpy_s( szSetCursorObjectName, sizeof( szSetCursorObjectName ), szObjectName );
         strcat_s( szSetCursorObjectName, sizeof( szSetCursorObjectName ), "T" );
         szSetCursorObjectName[ 32 ] = 0;
         strcat_s( szTemplate, 60000, "\r\n" );
         strcat_s( szTemplate, 60000, "   VIEW " );
         strcat_s( szTemplate, 60000, szSetCursorObjectName );
         strcat_s( szTemplate, 60000, " BASED ON LOD " );
         strcat_s( szTemplate, 60000, szObjectName );

         // Add the view for the parent window subtask.
         strcat_s( szTemplate, 60000, "\r\n" );
         strcat_s( szTemplate, 60000, "   VIEW ParentWindow" );

         // Finally we have the VIEW statement for the list LOD.
         strcat_s( szTemplate, 60000, "\r\n" );
         strcat_s( szTemplate, 60000, "   VIEW " );
         strcat_s( szTemplate, 60000, szListVOR );
         strcat_s( szTemplate, 60000, " BASED ON LOD " );
         strcat_s( szTemplate, 60000, szListObject );

         /* If the include type is Key entry then create the code for
            GetCtrlText. */
         if ( cSubObjType == 'K' )
         {
            strcat_s( szTemplate, 60000, "\r\n\r\n" );
            strcat_s( szTemplate, 60000, "   STRING( 50 ) szEntryField \r\n\r\n" );
            strcat_s( szTemplate, 60000,
            "   GetCtrlText( vSubtask, \"EntryField\", szEntryField, 50 )" );
         }

         strcat_s( szTemplate, 60000, "\r\n\r\n" );

         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate, vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            strcat_s( szTemplate, 60000, "   GET VIEW " );
            strcat_s( szTemplate, 60000, szListVOR );
            strcat_s( szTemplate, 60000, " NAMED \"" );
            strcat_s( szTemplate, 60000, szListVOR );
            strcat_s( szTemplate, 60000, "\"" );

            strcat_s( szTemplate, 60000, "\r\n\r\n" );
            strcat_s( szTemplate, 60000, "   IF RESULT > 0 " );
            strcat_s( szTemplate, 60000, "\r\n" );
            strcat_s( szTemplate, 60000, "     IF " );
            strcat_s( szTemplate, 60000, szListVOR );
            strcat_s( szTemplate, 60000, "." );
            strcat_s( szTemplate, 60000, szSubobjEntity );
            strcat_s( szTemplate, 60000, " EXISTS " );

            // Add a check to see if the entity to be included already exists
            // in the target.
            // Currently, we will only do this for Case 3.
            if ( cSubObjType == '3' )
            {
               zVIEW  vTempObject;

               CreateViewFromViewForTask( &vTempObject, vObject, 0 );
               SetCursorFirstEntityByString( vTempObject, "LOD_Entity", "Name",
                                             szEntityName, 0 );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "       CreateViewFromViewForTask( " );
               strcat_s( szTemplate, 60000, szSetCursorObjectName );
               strcat_s( szTemplate, 60000, ", " );
               strcat_s( szTemplate, 60000, szObjectName );
               strcat_s( szTemplate, 60000, " )" );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "       SET CURSOR FIRST  " );
               strcat_s( szTemplate, 60000, szSetCursorObjectName );
               strcat_s( szTemplate, 60000, "." );
               strcat_s( szTemplate, 60000, szEntityName );
   #if 0
            // Add WITHIN for Case 4, since we must set cursor within the
            // parent of the Assoc entity.
            if ( cSubObjType == '4' )
            {
               zCHAR szWithinEntityName[ 33 ];
               zVIEW vUserSpecWithin;

               CreateViewFromViewForTask( &vUserSpecWithin, vUserSpec, 0 );
               ResetViewFromSubobject( vUserSpecWithin );
               ResetViewFromSubobject( vUserSpecWithin );
               GetStringFromAttribute( szWithinEntityName, sizeof( szWithinEntityName ),
                                       vUserSpecWithin, "LOD_Entity", "Name" );
               DropView( vUserSpecWithin );
               strcat_s( szTemplate, 60000, " WITHIN " );
               strcat_s( szTemplate, 60000, szObjectName );
               strcat_s( szTemplate, 60000, "." );
               strcat_s( szTemplate, 60000, szWithinEntityName );
            }
   #endif

               strcat_s( szTemplate, 60000, " WHERE " );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "             " );
               // Go to add the qualification to the where.
               AddQualToWhere( vSubtask, szTemplate, vTempObject, szSetCursorObjectName,
                               szListVOR, szSubobjEntity );
               DropView( vTempObject );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "       IF RESULT < zCURSOR_SET  " );
            }


            // If the view for the include list is not the same as the view
            // for the include view itself, we will add the Activate for the
            // include view.
            if ( zstrcmp( szIncludeVOR, szIncludeListVOR ) != 0 )
            {
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "         ACTIVATE " );
               strcat_s( szTemplate, 60000, szIncludeVOR );
               strcat_s( szTemplate, 60000, "  WHERE " );
               strcat_s( szTemplate, 60000, "\r\n" );

               // Go to add the qualification to the where.
               AddQualToWhere( vSubtask, szTemplate, vObject2, szIncludeVOR,
                               szListVOR, szSubobjEntity );

               strcat_s( szTemplate, 60000, "\r\n" );
               CL( szTemplate, "         IF RESULT < 0", "", "", "", "" );
               CL( szTemplate, "            MessageSend( vSubtask,", "", "", "", "" );
               CL( szTemplate, "                          \"AD002\",", "", "", "", "" );
               CL( szTemplate, "                          \"Select Processing\",", "", "", "", "" );
               CL( szTemplate, "                          \"Selected entry does not exist.\",", "", "", "", "" );
               CL( szTemplate, "                          zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )", "", "", "", "" );
               CL( szTemplate, "            RETURN -1 ", "", "", "", "" );
               CL( szTemplate, "         END", "", "", "", "" );
            }

            /* Get the cardinality */
            /* If this object is a work object then it will not have
               ER_RelLink, default szCardinality to many. */
            if ( CheckExistenceOfEntity( vObject, "ER_RelLink" ) == 0 )
               GetStringFromAttribute( szCardinality, sizeof( szCardinality ), vObject,
                                       "ER_RelLink", "CardMax" );
            else
               szCardinality[ 0 ] = 0; /* many, not 1 */

            if ( zstrcmp( szCardinality, "1" ) == 0 )
            {
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "         IF " );
               strcat_s( szTemplate, 60000, szObjectName );
               strcat_s( szTemplate, 60000, "." );
               strcat_s( szTemplate, 60000, szEntityName );
               strcat_s( szTemplate, 60000, " EXISTS" );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "            EXCLUDE " );
               strcat_s( szTemplate, 60000, szObjectName );
               strcat_s( szTemplate, 60000, "." );
               strcat_s( szTemplate, 60000, szEntityName );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "         END" );
            }

            strcat_s( szTemplate, 60000, "\r\n" );
            strcat_s( szTemplate, 60000, "         INCLUDE " );
            strcat_s( szTemplate, 60000, szObjectName );
            strcat_s( szTemplate, 60000, "." );
            strcat_s( szTemplate, 60000, szEntityName );
            strcat_s( szTemplate, 60000, " FROM ");
            strcat_s( szTemplate, 60000, szIncludeVOR );
            strcat_s( szTemplate, 60000, "." );
            strcat_s( szTemplate, 60000, szSubobjEntity );
            strcat_s( szTemplate, 60000, "\r\n" );

            // If the view for the include list is not the same as the view
            // for the include view itself, we will drop the include view.
            if ( zstrcmp( szIncludeVOR, szIncludeListVOR ) != 0 )
            {
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "         DropObjectInstance( " );
               strcat_s( szTemplate, 60000, szIncludeVOR );
               strcat_s( szTemplate, 60000, " )" );
               strcat_s( szTemplate, 60000, "\r\n\r\n" );
            }

            // Add unique code for Case 4.
            if ( cSubObjType == '4' )
            {
               strcat_s( szTemplate, 60000, "         AcceptSubobject( " );
               strcat_s( szTemplate, 60000, szObjectName );
               strcat_s( szTemplate, 60000, ", \"" );
               strcat_s( szTemplate, 60000, szParentEntityName );
               strcat_s( szTemplate, 60000, "\" )" );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "         IF nZRetCode = 0" );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "            GetParentWindow ( ParentWindow, vSubtask ) " );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "            RefreshWindow ( ParentWindow )" );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "            CreateTemporalEntity ( " );
               strcat_s( szTemplate, 60000, szObjectName );
               strcat_s( szTemplate, 60000, ", \"" );
               strcat_s( szTemplate, 60000, szParentEntityName );
               strcat_s( szTemplate, 60000, "\", zPOS_AFTER )" );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "             SetFocusToCtrl ( vSubtask, \"" );
               strcat_s( szTemplate, 60000, szFirstDataFieldName );
               strcat_s( szTemplate, 60000, "\" )" );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "         END" );
               strcat_s( szTemplate, 60000, "\r\n" );
            }

            // Add unique code for Case 3.
            if ( cSubObjType == '3' )
            {
               strcat_s( szTemplate, 60000, "         GetParentWindow ( ParentWindow, vSubtask ) " );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "         RefreshWindow ( ParentWindow )" );
               strcat_s( szTemplate, 60000, "\r\n" );
            }

            if ( cSubObjType == '3' )
            {
               strcat_s( szTemplate, 60000, "       END" );
               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "       DropView( " );
               strcat_s( szTemplate, 60000, szSetCursorObjectName );
               strcat_s( szTemplate, 60000, " )" );
               strcat_s( szTemplate, 60000, "\r\n" );
            }

            strcat_s( szTemplate, 60000, "     END" );
            strcat_s( szTemplate, 60000, "\r\n" );
            strcat_s( szTemplate, 60000, "   END" );
         }

         strcat_s( szTemplate, 60000, " \r\n\r\nEND  /* END OF OPERATION */" );
      }

      // We may use this on Next/Prev button processing.
      if ( zstrcmp( szSourceCode, "RefreshParentWindow" ) == 0 )
      {
         strcat_s( szTemplate, 60000, "\r\n" );
         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            strcat_s( szTemplate, 60000, "   NAME VIEW vSubtask \"ParentWindow\"" );
         }

         strcat_s( szTemplate, 60000, " \r\n\r\nEND  /* END OF OPERATION */" );
      }

      // New processing for root of object.
      if ( zstrcmp( szSourceCode, "ProcessNewOI" ) == 0 )
      {
         strcat_s( szTemplate, 60000, "\r\n" );
         strcat_s( szTemplate, 60000, "\r\n" );
         CL( szTemplate, "   VIEW ", szObjectName,
             " REGISTERED AS ", szObjectName, "" );
         strcat_s( szTemplate, 60000, "\r\n" );

         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            CL( szTemplate, "   COMMIT ", szObjectName, "", "", "" );
            CL( szTemplate, "   IF RESULT < 0", "", "", "", "" );
            CL( szTemplate, "      SetWindowActionBehavior ( vSubtask, zWAB_StayOnWindow, \"\", \"\" )", "", "", "", "" );
            CL( szTemplate, "      RETURN -1","","","","" );
            CL( szTemplate, "   END ", "", "", "", "" );
            strcat_s( szTemplate, 60000, "\r\n" );
            CL( szTemplate, "   DropView ( ", szObjectName, " )", "", "" );
            CL( szTemplate, "   CreateNewInstance ( vSubtask )", "", "", "", "" );

            // Get name of first edit box to be used on SetFocus.
            SetViewToSubobject( vNewDialog, "CtrlCtrl" );
            nRC = SetCursorFirstEntity( vNewDialog, "Control", 0 );
            while ( nRC >= zCURSOR_SET &&
                  CompareAttributeToString( vNewDialog, "ControlDef", "Tag", "EditBox" ) != 0 )
            {
               nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
            }
            GetStringFromAttribute( szEditboxName, sizeof( szEditboxName ),
                                    vNewDialog, "Control", "Tag" );
            ResetViewFromSubobject( vNewDialog );
            CL( szTemplate, "   SetFocusToCtrl ( vSubtask, \"", szEditboxName, "\" )", "", "" );
         }

         strcat_s( szTemplate, 60000, " \r\nEND  /* END OF OPERATION */" );
      }

      // Next and Previous processing for root of object.
      if ( zstrcmp( szSourceCode, "ProcessNextOI" ) == 0 ||
           zstrcmp( szSourceCode, "ProcessPreviousOI" ) == 0 )
      {
         strcat_s( szTemplate, 60000, "\r\n" );
         strcat_s( szTemplate, 60000, "\r\n" );
         CL( szTemplate, "   VIEW ", szListVOR, " REGISTERED AS ", szListVOR, "" );
         CL( szTemplate, "   VIEW ", szObjectName, " REGISTERED AS ", szObjectName, "" );
         strcat_s( szTemplate, 60000, "\r\n" );

         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            CL( szTemplate, "   COMMIT ", szObjectName, "", "", "" );
            CL( szTemplate, "   IF RESULT < 0", "", "", "", "" );
            CL( szTemplate, "      SetWindowActionBehavior ( vSubtask, zWAB_StayOnWindow, \"\", \"\" )", "", "", "", "" );
            CL( szTemplate, "      RETURN -1","","","","" );
            CL( szTemplate, "   END ", "", "", "", "" );
            strcat_s( szTemplate, 60000, "\r\n" );
            CL( szTemplate, "   DropView ( ", szObjectName, " )", "", "" );
            if ( zstrcmp( szSourceCode, "ProcessNextOI" ) == 0 )
               CL( szTemplate, "   SET CURSOR NEXT ", szListVOR, ".", szEntityName, "" );
            else
               CL( szTemplate, "   SET CURSOR PREVIOUS ", szListVOR, ".", szEntityName, "" );

            CL( szTemplate, "   UpdateExistingInstance ( vSubtask )", "", "", "", "" );
         }

         strcat_s( szTemplate, 60000, " \r\nEND  /* END OF OPERATION */" );
      }


      if ( zstrcmp( szSourceCode, "LoadObjForList" ) == 0 )
      {
         strcat_s( szTemplate, 60000, "\r\n\r\n" );
         strcat_s( szTemplate, 60000, "   VIEW " );
         strcat_s( szTemplate, 60000, szIncludeVOR );
         strcat_s( szTemplate, 60000, " BASED ON LOD " );
         strcat_s( szTemplate, 60000, szIncludeObject );
         strcat_s( szTemplate, 60000, "\r\n" );

         strcat_s( szTemplate, 60000, "   STRING( 50 ) szEntryField" );
         strcat_s( szTemplate, 60000, "\r\n\r\n" );

         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            strcat_s( szTemplate, 60000,
            "   GetCtrlText( vSubtask, \"EntryField\", szEntryField, 50 )" );
            strcat_s( szTemplate, 60000, "\r\n\r\n" );

            strcat_s( szTemplate, 60000, "   ACTIVATE " );
            strcat_s( szTemplate, 60000, szIncludeVOR );
            strcat_s( szTemplate, 60000, " WHERE " );
            strcat_s( szTemplate, 60000, szListQualifier );
            strcat_s( szTemplate, 60000, " = szEntryField" );
         }

         strcat_s( szTemplate, 60000, " \r\n\r\nEND  /* END OF OPERATION */" );
      }

      if ( zstrcmp ( szSourceCode, "ListAllSubObj" ) == 0  ||
           zstrcmp( szSourceCode, "ListQualSubObj" ) == 0 )
      {
         zVIEW vUserSpecRoot;
         zBOOL bListOI_DoesNotExist;

         // We will not create statements for activating the list object if
         // the include views are the same as the view of the UIS itself.
         CreateViewFromViewForTask( &vUserSpecRoot, vUserSpec, 0 );
         ResetView( vUserSpecRoot );
         if ( CheckExistenceOfEntity( vUserSpec, "ListViewObjRef" )    >= zCURSOR_SET &&
            CheckExistenceOfEntity( vUserSpec, "IncludeViewObjRef" ) >= zCURSOR_SET &&
            CompareAttributeToAttribute( vUserSpec,     "IncludeViewObjRef", "ZKey",
                                          vUserSpecRoot, "UIS_ViewObjRef",    "ZKey" ) == 0 &&
            CompareAttributeToAttribute( vUserSpec,     "ListViewObjRef", "ZKey",
                                         vUserSpecRoot, "UIS_ViewObjRef", "ZKey" ) == 0 )
         {
            bListOI_DoesNotExist = FALSE;
         }
         else
            bListOI_DoesNotExist = TRUE;

         DropView( vUserSpecRoot );

         // The following creates the VML for activating the list of entries to include.
         strcat_s( szTemplate, 60000, "\r\n" );
         if ( bListOI_DoesNotExist )
         {
            strcat_s( szTemplate, 60000, "\r\n" );
            strcat_s( szTemplate, 60000, "   VIEW " );
            strcat_s( szTemplate, 60000, szListVOR );
            strcat_s( szTemplate, 60000, " BASED ON LOD " );
            strcat_s( szTemplate, 60000, szListObject );
         }

         // Add unique code for Case 4.
         if ( cSubObjType == '4' )
         {
            strcat_s( szTemplate, 60000, "\r\n" );
            strcat_s( szTemplate, 60000, "   VIEW " );
            strcat_s( szTemplate, 60000, szObjectName );
            strcat_s( szTemplate, 60000, " REGISTERED AS " );
            strcat_s( szTemplate, 60000, szObjectName );
         }

         if ( bListOI_DoesNotExist )
         {
            if ( CompareAttributeToString( vUserSpec, "WndDesign",
                                           "ListQualFlag", "Y" ) == 0 )
            {
               // Activate SubObjList with Qualification on list.

               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "   STRING( 60 ) QualString " );
            }
         }
         strcat_s( szTemplate, 60000, "\r\n" );
         strcat_s( szTemplate, 60000, "\r\n" );


         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            if ( bListOI_DoesNotExist )
            {
               if ( CompareAttributeToString( vUserSpec, "WndDesign",
                                              "ListQualFlag", "Y" ) == 0 )
               {
                  // Activate SubObjList with Qualification on list.

                  zCHAR szWhereEntityName[ 33 ];
                  zCHAR szWhereAttributeName[ 33 ];
                  zCHAR szLikeFlag[ 2 ];

                  strcat_s( szTemplate, 60000, "   GetCtrlText( vSubtask, \"ListQual\", QualString, 60 )" );
//                  strcat_s( szTemplate, 60000, "\r\n" );
//                  strcat_s( szTemplate, 60000, "\r\n" );

                  strcat_s( szTemplate, 60000, "   IF QualString = \"\" " );
                  strcat_s( szTemplate, 60000, "\r\n" );
                  strcat_s( szTemplate, 60000, "      ACTIVATE " );
                  strcat_s( szTemplate, 60000, szListVOR );
                  strcat_s( szTemplate, 60000, " RootOnlyMultiple " );
                  strcat_s( szTemplate, 60000, "\r\n" );
                  strcat_s( szTemplate, 60000, "   ELSE" );
                  strcat_s( szTemplate, 60000, "\r\n" );

                  GetStringFromAttribute( szLikeFlag, sizeof( szLikeFlag ), vUserSpec,
                                          "WndDesign", "ListQualLikeFlag" );
                  if ( szLikeFlag[ 0 ] == 'L' )
                  {
                     strcat_s( szTemplate, 60000, "      QualString = QualString + \"%\" " );
                     strcat_s( szTemplate, 60000, "\r\n" );
                  }

                  strcat_s( szTemplate, 60000, "      ACTIVATE " );
                  strcat_s( szTemplate, 60000, szListVOR );
                  strcat_s( szTemplate, 60000, " RootOnlyMultiple WHERE " );
                  strcat_s( szTemplate, 60000, "\r\n" );

                  // Create the WHERE qualification from the first I_LOD_Attribute
                  // entry for the UIS_Entity.
                  SetCursorFirstEntity( vUserSpec, "I_LOD_Entity", "UIS_Entity" );
                  GetStringFromAttribute( szWhereEntityName, sizeof( szWhereEntityName ),
                                          vUserSpec, "I_LOD_Entity", "Name" );
                  GetStringFromAttribute( szWhereAttributeName, sizeof( szWhereAttributeName ),
                                          vUserSpec, "I_ER_Attribute", "Name" );
                  strcat_s( szTemplate, 60000, "               " );
                  strcat_s( szTemplate, 60000, szListVOR );
                  strcat_s( szTemplate, 60000, "." );
                  strcat_s( szTemplate, 60000, szWhereEntityName );
                  strcat_s( szTemplate, 60000, "." );
                  strcat_s( szTemplate, 60000, szWhereAttributeName );
                  strcat_s( szTemplate, 60000, "" );
                  if ( szLikeFlag[ 0 ] == 'L' )
                     strcat_s( szTemplate, 60000, " LIKE QualString " );
                  else
                     strcat_s( szTemplate, 60000, " >= QualString " );

                  strcat_s( szTemplate, 60000, "\r\n" );
                  strcat_s( szTemplate, 60000, "   END " );
               }
               else
               {

//                  strcat_s( szTemplate, 60000, "\r\n\r\n" );

                  // Activate SubObjList with NO Qualification on list.

                  strcat_s( szTemplate, 60000, "   ACTIVATE " );
                  strcat_s( szTemplate, 60000, szListVOR );
                  if ( CompareAttributeToString( vUserSpec, "UIS_Entity",
                                                 "RootOnlyList", "Y" ) == 0 )
                     strcat_s( szTemplate, 60000, " RootOnlyMultiple" );
                  else
                     strcat_s( szTemplate, 60000, " Multiple" );

                  if ( zstrcmp( szObjectKeyName, "\"\"" ) != 0 )
                  {
                     strcat_s( szTemplate, 60000, " WHERE\r\n                           " );
                     strcat_s( szTemplate, 60000, szObjectKeyName );
                  }
               }

               strcat_s( szTemplate, 60000, "\r\n\r\n" );
               strcat_s( szTemplate, 60000, "   NAME VIEW " );
               strcat_s( szTemplate, 60000, szListVOR );
               strcat_s( szTemplate, 60000, " \"" );
               strcat_s( szTemplate, 60000, szListVOR );
               strcat_s( szTemplate, 60000, "\"" );
            }

            // Add unique code for Case 4.
            if ( cSubObjType == '4' )
            {
               strcat_s( szTemplate, 60000, "\r\n" );

               // If there is no ListQual window, we will do the CreateTemporalEntity
               // here.  If there is a ListQual window, the Create was done on
               // the Prebuild event.
               if ( CompareAttributeToString( vUserSpec, "WndDesign",
                                              "ListQualFlag", "Y" ) != 0 )
               {
                  strcat_s( szTemplate, 60000, "\r\n" );
                  strcat_s( szTemplate, 60000, "   CreateTemporalEntity ( " );
                  strcat_s( szTemplate, 60000, szObjectName );
                  strcat_s( szTemplate, 60000, ", \"" );
                  strcat_s( szTemplate, 60000, szParentEntityName );
                  strcat_s( szTemplate, 60000, "\", zPOS_AFTER )" );
               }
            }
         }

         strcat_s( szTemplate, 60000, " \r\n\r\nEND  /* END OF OPERATION */" );
      }

      // ActivateMainForList operation
      if ( zstrcmp ( szSourceCode, "ActivateMainList" ) == 0 )
      {
         zCHAR szWhereEntityName[ 33 ];
         zCHAR szWhereAttributeName[ 33 ];
         zCHAR szLikeFlag[ 2 ];

         if ( CompareAttributeToString( vUserSpec, "WndDesign", "ListQualFlag",
                                        "Y" ) == 0 )
         {
            // ActivateMainList with Qualification on list.

            strcat_s( szTemplate, 60000, "\r\n\r\n" );
            CL( szTemplate, "   VIEW ", szListVOR, " BASED ON LOD ", szListObject, "" );
            CL( szTemplate, "   VIEW ", szObjectName, " BASED ON LOD ", szObjectName, "" );
            CL( szTemplate, "   STRING( 60 ) QualString ", "", "", "", "" );
            CL( szTemplate, "", "", "", "", "" );
         }
         else
         {
            // ActivateMainList with NO Qualification on list.

            // Note below the check for RootOnlyMutliple.  This is set in
            // the UserSpec when the Operation ActivateMainForList operation
            // is initially created.

            // The following creates the VML for activating the list of maint
            // entities.
            CL( szTemplate, "\r\n", "", "", "", "" );
            CL( szTemplate, "   VIEW ", szListVOR, " BASED ON LOD ", szListObject, "" );
            CL( szTemplate, "   VIEW ", szObjectName, " BASED ON LOD ", szObjectName, "" );
            CL( szTemplate, "", "", "", "", "" );
         }


         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode,
                                     szTemplate,
                                     vUserSpec,
                                     vObject );
         if ( nRC >= 0 )
         {
            if ( CompareAttributeToString( vUserSpec, "WndDesign", "ListQualFlag",
                                          "Y" ) == 0 )
            {
               // ActivateMainList with Qualification on list.

               CL( szTemplate, "   GetCtrlText( vSubtask, \"ListQual\", QualString, 60 )", "", "", "", "" );
               CL( szTemplate, "", "", "", "", "" );

               CL( szTemplate, "   IF QualString = \"\" ", "", "", "", "" );
               CL( szTemplate, "      ACTIVATE ", szListVOR, " RootOnlyMultiple ", "", "" );
               CL( szTemplate, "   ELSE", "", "", "", "" );

               GetStringFromAttribute( szLikeFlag, sizeof( szLikeFlag ), vUserSpec,
                                       "WndDesign", "ListQualLikeFlag" );
               if ( szLikeFlag[ 0 ] == 'L' )
               {
                  CL( szTemplate, "      QualString = QualString + \"%\" ", "", "", "", "" );
               }

               CL( szTemplate, "      ACTIVATE ", szListVOR, " RootOnlyMultiple WHERE ", "", "" );

               // Create the WHERE qualification from the first L_LOD_Attribute
               // entry for the UIS_Entity.
               SetCursorFirstEntity( vUserSpec, "L_LOD_Entity", "UIS_Entity" );
               GetStringFromAttribute( szWhereEntityName, sizeof( szWhereEntityName ),
                                       vUserSpec, "L_LOD_Entity", "Name" );
               GetStringFromAttribute( szWhereAttributeName, sizeof( szWhereAttributeName ),
                                       vUserSpec, "L_ER_Attribute", "Name" );
               strcat_s( szTemplate, 60000, "               " );
               strcat_s( szTemplate, 60000, szListVOR );
               strcat_s( szTemplate, 60000, "." );
               strcat_s( szTemplate, 60000, szWhereEntityName );
               strcat_s( szTemplate, 60000, "." );
               strcat_s( szTemplate, 60000, szWhereAttributeName );
               strcat_s( szTemplate, 60000, "" );
               if ( szLikeFlag[ 0 ] == 'L' )
                  strcat_s( szTemplate, 60000, " LIKE QualString " );
               else
                  strcat_s( szTemplate, 60000, " >= QualString " );

               strcat_s( szTemplate, 60000, "\r\n" );
               strcat_s( szTemplate, 60000, "   END " );
               strcat_s( szTemplate, 60000, "\r\n" );
            }
            else
            {
               // ActivateMainList with NO Qualification on list.

               // Note below the check for RootOnlyMutliple.  This is set in
               // the UserSpec when the Operation ActivateMainForList operation
               // is initially created.

               // The following creates the VML for activating the list of maint
               // entities.

               if ( CompareAttributeToString( vUserSpec, "UIS_Entity",
                                              "RootOnlyList", "Y" ) == 0 )
                  CL( szTemplate, "   ACTIVATE ", szListVOR, " RootOnlyMultiple ", "", "" );
               else
                  CL( szTemplate, "   ACTIVATE ", szListVOR, " Multiple ", "", "" );
            }

            // Create the WHERE qualification for repositioning the list view
            // on the entity last updated.
            SetCursorFirstEntity( vUserSpec, "LOD_Entity", "UIS_Entity" );
            GetStringFromAttribute( szWhereEntityName, sizeof( szWhereEntityName ),
                                    vUserSpec, "LOD_Entity", "Name" );

            CL( szTemplate, "", "", "", "", "" );
            CL( szTemplate, "   GET VIEW ", szObjectName, " NAMED \"", szObjectName, "\"" );
            CL( szTemplate, "   IF RESULT >= 0", "", "", "", "" );
            CL( szTemplate, "      IF ", szListVOR, ".", szWhereEntityName, " EXISTS" );
            CL( szTemplate, "         SET CURSOR FIRST ", szListVOR, ".", szWhereEntityName, " WHERE" );
            AddQualToWhere( vSubtask, szTemplate,
                           vObject,
                           szObjectName,
                           szListVOR, 0 );
            CL( szTemplate, "", "", "", "", "" );
            CL( szTemplate, "      END", "", "", "", "" );
            CL( szTemplate, "      DropView ( ", szObjectName, " )", "", "" );
            CL( szTemplate, "   END", "", "", "", "" );
            CL( szTemplate, "", "", "", "", "" );
            CL( szTemplate, "   NAME VIEW ", szListVOR, " \"", szListVOR, "\"" );
         }

         strcat_s( szTemplate, 60000, " \r\nEND  /* END OF OPERATION */" );
      }

      // UpdateExistingInstance operation
      if ( zstrcmp ( szSourceCode, "UpdateExistingInstance" ) == 0 )
      {
         // The following creates the VML for activating the main object
         // instance from the list object.
         strcat_s( szTemplate, 60000, "\r\n\r\n" );
         CL( szTemplate, "   VIEW ", szListVOR, " REGISTERED AS ", szListVOR, "" );
         CL( szTemplate, "   VIEW ", szObjectName, " BASED ON LOD ", szObjectName, "" );
         strcat_s( szTemplate, 60000, "\r\n" );

         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            CL( szTemplate, "   ACTIVATE ", szObjectName, "  Single WHERE ", "", "" );

            // Go to add the qualification to the where.
            AddQualToWhere( vSubtask, szTemplate, vObject, szObjectName, szListVOR, 0 );
            strcat_s( szTemplate, 60000, "\r\n\r\n" );
            CL( szTemplate, "   NAME VIEW ", szObjectName, " \"", szObjectName, "\"" );
         }

         strcat_s( szTemplate, 60000, " \r\nEND  /* END OF OPERATION */" );
      }

      // CreateNewInstance operation
      if ( zstrcmp ( szSourceCode, "CreateNewInstance" ) == 0 )
      {
         // The following creates the VML for activating an empty object
         // instance for the object.
         strcat_s( szTemplate, 60000, "\r\n\r\n" );
         CL( szTemplate, "   VIEW ", szObjectName, " BASED ON LOD ", szObjectName, "" );
         strcat_s( szTemplate, 60000, "\r\n" );

         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode,
                                     szTemplate,
                                     vUserSpec,
                                     vObject );
         if ( nRC >= 0 )
         {
            CL( szTemplate, "   ACTIVATE ", szObjectName, "  EMPTY", "", "" );
            CL( szTemplate, "   NAME VIEW ", szObjectName, " \"", szObjectName, "\"" );
            GetStringFromAttribute( szEntityName, sizeof( szEntityName ),
                                    vUserSpec, "LOD_Entity", "Name" );
            CL( szTemplate, "   CREATE ENTITY ", szObjectName, ".", szEntityName, "" );
         }

         strcat_s( szTemplate, 60000, " \r\nEND  /* END OF OPERATION */" );
      }

      // DeleteInstance operation
      if ( zstrcmp( szSourceCode, "DeleteInstance" ) == 0 )
      {
         // The following creates the VML for deleting the OI from memory.
         strcat_s( szTemplate, 60000, "\r\n\r\n" );
         CL( szTemplate, "   VIEW ", szObjectName, " REGISTERED AS ", szObjectName, "" );
         strcat_s( szTemplate, 60000, "\r\n" );

         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            CL( szTemplate, "   DropObjectInstance ( ", szObjectName, " )", "", "" );
         }

         strcat_s( szTemplate, 60000, " \r\nEND  /* END OF OPERATION */" );
      }

      // SaveInstance operation
      if ( zstrcmp ( szSourceCode, "SaveInstance" ) == 0 )
      {
         // The following creates the VML for deleting the OI from memory.
         strcat_s( szTemplate, 60000, "\r\n\r\n" );
         CL( szTemplate, "   VIEW ", szObjectName, " REGISTERED AS ", szObjectName, "" );
         strcat_s( szTemplate, 60000, "\r\n" );


         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            GetStringFromAttribute( szEntityName, sizeof( szEntityName ),
                                    vUserSpec, "LOD_Entity", "Name" );
            CL( szTemplate, "   AcceptSubobject ( ", szObjectName, ",\"", szEntityName, "\" )" );
            CL( szTemplate, "   IF nZRetCode < 0", "", "", "", "" );
            CL( szTemplate, "      // Message should have been sent by object entity constraint.", "", "", "", "" );
            CL( szTemplate, "      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, \"\", \"\" )", "", "", "", "" );
            CL( szTemplate, "      RETURN -1", "", "", "", "" );
            CL( szTemplate, "   END", "", "", "", "" );
            strcat_s( szTemplate, 60000, "\r\n" );
            CL( szTemplate, "   COMMIT ", szObjectName, "", "", "" );
            strcat_s( szTemplate, 60000, "\r\n" );
            CL( szTemplate, "   IF RESULT < 0", "", "", "", "" );
            CL( szTemplate, "      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, \"\", \"\" )", "", "", "", "" );
            CL( szTemplate, "   END", "", "", "", "" );
         }

         strcat_s( szTemplate, 60000, " \r\nEND  /* END OF OPERATION */" );
      }

      // DeleteObjectEntityFromList and DeleteObjectEntity
      if ( zstrcmp ( szSourceCode, "DeleteObjectEntityFromList" ) == 0 ||
           zstrcmp ( szSourceCode, "DeleteObjectEntity" ) == 0 )
      {
         zCHAR szWhereEntityName[ 33 ];

         // The following creates the VML for deleting an object instance
         // based on the entry selected from a list box or from the entity
         // currently being updated.
         strcat_s( szTemplate, 60000, "\r\n\r\n" );

         if ( szListVOR[ 0 ] != 0 )
            CL( szTemplate, "   VIEW    ", szListVOR, " REGISTERED AS ", szListVOR, "" );

         if ( zstrcmp ( szSourceCode, "DeleteObjectEntityFromList" ) == 0 )
         {
            CL( szTemplate, "   VIEW    ", szObjectName, " BASED ON LOD ", szObjectName, "" );
         }
         else
         {
            CL( szTemplate, "   VIEW    ", szObjectName, " REGISTERED AS ", szObjectName, "" );
         }

         CL( szTemplate, "   INTEGER nRC", "", "", "", "" );
         strcat_s( szTemplate, 60000, "\r\n" );


         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode,
                                     szTemplate,
                                     vUserSpec,
                                     vObject );
         if ( nRC >= 0 )
         {
            GetStringFromAttribute( szWhereEntityName, sizeof( szWhereEntityName ),
                                    vUserSpec, "LOD_Entity", "Name" );
            CL( szTemplate, "   nRC = MessagePrompt( vSubtask,", "", "", "", "" );
            CL( szTemplate, "                        \"AD001\",", "", "", "", "" );
            CL( szTemplate, "                        \"Delete Processing\",", "", "", "", "" );
            CL( szTemplate, "                        \"OK to continue with Delete?\",", "", "", "", "" );
            CL( szTemplate, "                        0,", "", "", "", "" );
            CL( szTemplate, "                        zBUTTONS_YESNO,", "", "", "", "" );
            CL( szTemplate, "                        zRESPONSE_NO,", "", "", "", "" );
            CL( szTemplate, "                        0 )", "", "", "", "" );
            CL( szTemplate, "   IF nRC = zRESPONSE_YES", "", "", "", "" );

            if ( zstrcmp ( szSourceCode, "DeleteObjectEntityFromList" ) == 0 )
            {
               CL( szTemplate, "", "", "", "", "" );
               CL( szTemplate, "      ACTIVATE ", szObjectName, "  Single WHERE ", "", "" );

               // Go to add the qualification to the where.
               AddQualToWhere( vSubtask, szTemplate,
                              vObject,
                              szObjectName,
                              szListVOR, 0 );

               CL( szTemplate, "", "", "", "", "" );
               CL( szTemplate, "      NAME VIEW ", szObjectName, " \"", szObjectName, "\"" );
               CL( szTemplate, "", "", "", "", "" );
            }

            CL( szTemplate, "      DELETE ENTITY ", szObjectName, ".", szWhereEntityName, "" );
            CL( szTemplate, "      COMMIT ", szObjectName, "", "", "" );
            CL( szTemplate, "", "", "", "", "" );
            CL( szTemplate, "      IF RESULT < 0", "", "", "", "" );
            CL( szTemplate, "         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, \"\", \"\" )", "", "", "", "" );
            CL( szTemplate, "         RETURN -1", "", "", "", "" );
            CL( szTemplate, "      ELSE", "", "", "", "" );

            if ( szListVOR[ 0 ] != 0 )
               CL( szTemplate, "         DELETE ENTITY ", szListVOR, ".", szWhereEntityName, "" );

            CL( szTemplate, "         DropView( ", szObjectName, " )", "", "" );
            CL( szTemplate, "      END", "", "", "", "" );
            CL( szTemplate, "", "", "", "", "" );
            CL( szTemplate, "   ELSE", "", "", "", "" );
            CL( szTemplate, "      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, \"\", \"\" )", "", "", "", "" );
            CL( szTemplate, "   END", "", "", "", "" );
            CL( szTemplate, "", "", "", "", "" );
         }

         strcat_s( szTemplate, 60000, " \r\nEND  /* END OF OPERATION */" );
      }

      // ListSub for Case 5/6 operation
      if ( zstrcmp ( szSourceCode, "ListSub" ) == 0 )
      {
         strcat_s( szTemplate, 60000, "\r\n" );

         // Loop for each sub entity to the Assoc entity and create a VIEW
         // entry for each Case 5/6 entity.
         nRC = SetCursorFirstEntity( vUserSpec, "UIS_Entity", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            GetStringFromAttribute( szCase, sizeof( szCase ), vUserSpec,
                                    "UIS_Entity", "AutodesignCaseType" );
            if ( szCase[ 0 ] == '5' || szCase[ 0 ] == '6' )
            {
               GetStringFromAttribute( szListVOR, sizeof( szListVOR ), vUserSpec,
                                       "ListViewObjRef", "Name" );
               GetStringFromAttribute( szListObject, sizeof( szListObject ), vUserSpec,
                                       "ListSubObjLOD", "Name" );

               // It is possible that two Case 5/6 entries use the same List view.
               // We must thus check to see if this List View has been used
               // prior.  We will do this by looping backwards from our current
               // UIS_Entity position to find a match.
               nMatchFlag = 0;
               CreateViewFromViewForTask( &vTempUIS, vUserSpec, 0 );
               nRC = SetCursorPrevEntity( vTempUIS, "UIS_Entity", 0 );
               while ( nRC >= zCURSOR_SET )
               {
                  GetStringFromAttribute( szWkCase, sizeof( szWkCase ),
                                          vTempUIS, "UIS_Entity", "AutodesignCaseType" );
                  if ( (szWkCase[ 0 ] == '5' || szWkCase[ 0 ] == '6') &&
                       CompareAttributeToAttribute( vTempUIS,
                                                    "ListViewObjRef",
                                                    "Name",
                                                     vUserSpec,
                                                    "ListViewObjRef",
                                                    "Name" ) == 0 )
                  {
                     nMatchFlag = 1;
                  }

                  nRC = SetCursorPrevEntity( vTempUIS, "UIS_Entity", 0 );
               }

               DropView( vTempUIS );

               if ( nMatchFlag == 0 )
               {
                  strcat_s( szTemplate, 60000, "\r\n" );
                  strcat_s( szTemplate, 60000, "   VIEW " );
                  strcat_s( szTemplate, 60000, szListVOR );
                  strcat_s( szTemplate, 60000, " BASED ON LOD " );
                  strcat_s( szTemplate, 60000, szListObject );
               }
            }

            nRC = SetCursorNextEntity( vUserSpec, "UIS_Entity", 0 );
         }

         strcat_s( szTemplate, 60000, "\r\n\r\n" );

         // Call the user exit, if it exists.
         nRC = UserPostVariableExit( szSourceCode, szTemplate,
                                     vUserSpec, vObject );
         if ( nRC >= 0 )
         {
            // Loop for each sub entity to the Assoc entity and create the IF
            // and ACTIVATE entries for each Case 5/6 entity.
            nRC = SetCursorFirstEntity( vUserSpec, "UIS_Entity", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               GetStringFromAttribute( szCase, sizeof( szCase ), vUserSpec,
                                       "UIS_Entity", "AutodesignCaseType" );
               if ( szCase[ 0 ] == '5' || szCase[ 0 ] == '6' )
               {
                  GetStringFromAttribute( szListVOR, sizeof( szListVOR ), vUserSpec,
                                          "ListViewObjRef", "Name" );
                  GetStringFromAttribute( szListObject, sizeof( szListObject ), vUserSpec,
                                          "ListSubObjLOD", "Name" );

                  // It is possible that two Case 5/6 entries use the same List view.
                  // We must thus check to see if this List View has been used
                  // prior.  We will do this by looping backwards from our current
                  // UIS_Entity position to find a match.
                  nMatchFlag = 0;
                  CreateViewFromViewForTask( &vTempUIS, vUserSpec, 0 );
                  nRC = SetCursorPrevEntity( vTempUIS, "UIS_Entity", 0 );
                  while ( nRC >= zCURSOR_SET )
                  {
                     GetStringFromAttribute( szWkCase, sizeof( szWkCase ),
                                             vTempUIS, "UIS_Entity", "AutodesignCaseType" );
                     if ( (szWkCase[ 0 ] == '5' || szWkCase[ 0 ] == '6') &&
                          CompareAttributeToAttribute( vTempUIS, "ListViewObjRef", "Name", vUserSpec,
                                                       "ListViewObjRef", "Name" ) == 0 )
                     {
                        nMatchFlag = 1;
                     }

                     nRC = SetCursorPrevEntity( vTempUIS, "UIS_Entity", 0 );
                  }

                  DropView( vTempUIS );

                  if ( nMatchFlag == 0 )
                  {
                     strcat_s( szTemplate, 60000, "   GET VIEW " );
                     strcat_s( szTemplate, 60000, szListVOR );
                     strcat_s( szTemplate, 60000, " NAMED \"" );
                     strcat_s( szTemplate, 60000, szListVOR );
                     strcat_s( szTemplate, 60000, "\"" );
                     strcat_s( szTemplate, 60000, "\r\n" );

                     strcat_s( szTemplate, 60000, "   IF RESULT < 0" );
                     strcat_s( szTemplate, 60000, "\r\n" );
                     strcat_s( szTemplate, 60000, "      ACTIVATE " );
                     strcat_s( szTemplate, 60000, szListVOR );
                     strcat_s( szTemplate, 60000, " RootOnlyMultiple" );

                     strcat_s( szTemplate, 60000, "\r\n" );
                     strcat_s( szTemplate, 60000, "      NAME VIEW " );
                     strcat_s( szTemplate, 60000, szListVOR );
                     strcat_s( szTemplate, 60000, " \"" );
                     strcat_s( szTemplate, 60000, szListVOR );
                     strcat_s( szTemplate, 60000, "\"" );
                     strcat_s( szTemplate, 60000, "\r\n" );
                     strcat_s( szTemplate, 60000, "   END" );
                     strcat_s( szTemplate, 60000, "\r\n\r\n" );
                  }
               }

               nRC = SetCursorNextEntity( vUserSpec, "UIS_Entity", 0 );
            }
         }

         strcat_s( szTemplate, 60000, " \r\n\r\nEND  /* END OF OPERATION */" );
         ResetViewFromSubobject( vUserSpec );
      }

      // lLth has the length of the generated code prior to the adding of
      // the current Operation code. We will thus add the length of the
      // Template to get the length of the new combined code.
      TemplateLth = zstrlen( szTemplate );
      lLth += TemplateLth;

      selMemory1 = SysAllocMemory( (zCOREMEM) &lpMemory1, lLth + 1, 0, zCOREMEM_ALLOC, 0 );

      strcpy_s( lpMemory1, lLth + 1, lpMemory );
      strcat_s( lpMemory1, lLth + 1, szTemplate );

      // Add the EOF character to buffer.
      lpMemory1[ lLth - 1 ] = '\x1A';

      // Reposition to the beginning of the file.
#ifdef __WIN32__
      // Reposition to the beginning of the file.
      SetFilePointer( (HANDLE) fh, 0, 0, FILE_BEGIN );

      WriteFile( (HANDLE) fh, lpMemory1, lLth, (LPDWORD) &lRC, 0 );
#else
      // Reposition to the beginning of the file.
      _llseek( (int) fh, 0L, SEEK_SET );

      // Truncate the file by writing 0 bytes.
      _lwrite( (int) fh, (zPCHAR) "", 0 );
      lRC = (zLONG) _lwrite((int)fh, lpMemory1, (zUSHORT) lLth );
#endif

      // Write out the contents of the buffer to the file.
      if ( lLth != lRC )
      {
         strcat_s( szFileSpec, sizeof( szFileSpec ), "\n\nAttempt to write " );
         zltoa( lLth, szFileSpec + zstrlen( szFileSpec ), sizeof( szFileSpec ) - zstrlen( szFileSpec ) );
         strcat_s( szFileSpec, sizeof( szFileSpec ), " bytes." );
         MessageSend( vSubtask, "AD10405", "Autodesigner",
                      szFileSpec,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }

      DropView( vObject );
      SysFreeMemory( selMemory1 );
   }

   SysFreeMemory( selMemory );
   SysCloseFile( vUserSpec, fh, 0 );

// TraceLineS("END", "CreateSourceCode W7" );
   return( 0 );
}
