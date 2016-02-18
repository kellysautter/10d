/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzersaso.c - Zeidon application operations
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
// LAST MESSAGE ID: ER00302
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
oTZERSASO_RefreshSA_FromEMD( zVIEW    vSubjectArea,
                             zVIEW    vEntpModel )
{
   zSHORT   nRC;
   zSHORT   RESULT;

   RESULT = SetCursorFirstEntity( vSubjectArea, "SA_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nRC = oTZERSASO_RefreshOneSA_Entity( vSubjectArea, vEntpModel );
      RESULT = SetCursorNextEntity( vSubjectArea, "SA_Entity", "" );
   }
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZERSASO_RefreshOneSA_Entity( zVIEW    vSubjectArea,
                               zVIEW    vEntpModel )
{
   zVIEW    vEntpModel_Wrk;
   zSHORT   nRC;
   zLONG    lEntityZKey;
   zLONG    lRelLinkZKey;
   zCHAR    szEntityName[ 32 ];
   zSHORT   nZRetCode;
   zSHORT   RESULT;

   nRC = CreateViewFromViewForTask( &vEntpModel_Wrk, vEntpModel, 0 );
   nZRetCode = GetIntegerFromAttribute( &lEntityZKey, vSubjectArea, "ER_Entity", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vEntpModel_Wrk, "ER_Entity", "ZKey", lEntityZKey, "" );
   if ( ( nRC > zCURSOR_UNCHANGED ) )
   {
      nRC = ExcludeEntity( vSubjectArea, "ER_Entity", zREPOS_NONE );
      nRC = IncludeSubobjectFromSubobject( vSubjectArea, "ER_Entity", vEntpModel_Wrk, "ER_Entity", zPOS_AFTER );
      RESULT = SetCursorFirstEntity( vEntpModel_Wrk, "ER_RelLink", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         nZRetCode = GetIntegerFromAttribute( &lRelLinkZKey, vEntpModel_Wrk, "ER_RelLink", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( vSubjectArea, "ER_RelLink", "ZKey", lRelLinkZKey, "ER_Entity" );
         nRC = IncludeSubobjectFromSubobject( vSubjectArea, "ER_RelLink_Other", vEntpModel_Wrk, "ER_RelLink_Other", zPOS_AFTER );
         RESULT = SetCursorNextEntity( vEntpModel_Wrk, "ER_RelLink", "" );
      }
      nRC = 0;
   }
   else
   {
      nZRetCode = GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vSubjectArea, "ER_Entity", "Name" );
      nRC = SetCursorFirstEntityByString( vEntpModel_Wrk, "ER_Entity", "Name", szEntityName, "" );
      if ( ( nRC > zCURSOR_UNCHANGED ) )
      {
         nRC = ExcludeEntity( vSubjectArea, "ER_Entity", zREPOS_NONE );
         nRC = IncludeSubobjectFromSubobject( vSubjectArea, "ER_Entity", vEntpModel_Wrk, "ER_Entity", zPOS_AFTER );
         RESULT = SetCursorFirstEntity( vEntpModel_Wrk, "ER_RelLink", "ER_Entity" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
            nZRetCode = GetIntegerFromAttribute( &lRelLinkZKey, vEntpModel_Wrk, "ER_RelLink", "ZKey" );
            nRC = SetCursorFirstEntityByInteger( vSubjectArea, "ER_RelLink", "ZKey", lRelLinkZKey, "ER_Entity" );
            nRC = IncludeSubobjectFromSubobject( vSubjectArea, "ER_RelLink_Other", vEntpModel_Wrk, "ER_RelLink_Other", zPOS_AFTER );
            RESULT = SetCursorNextEntity( vEntpModel_Wrk, "ER_RelLink", "ER_Entity" );
         }
         nRC = 0;
      }
      else
      {
         nRC = DeleteEntity( vSubjectArea, "SA_Entity", zREPOS_NONE );
         nRC = 1;
      }
   }

   DropView( vEntpModel_Wrk );
   return( nRC );
}

#if 1
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_AddOneSA_EntityFrER( zVIEW vSubtask,
                               zVIEW vSA,
                               zVIEW vER )
{
   zVIEW vER2;
   zVIEW vSA2;
   zVIEW vSA3;
   zLONG lZKey, lZKey1, lZKey2;

   GetIntegerFromAttribute( &lZKey, vER, "ER_Entity", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vSA, "ER_Entity", "ZKey", lZKey, "SubjectArea" ) >= zCURSOR_SET )
   {
      return( 1 );
   }

   CreateMetaEntity( vSubtask, vSA, "SA_Entity", zPOS_LAST );
   IncludeSubobjectFromSubobject( vSA, "ER_Entity", vER, "ER_Entity", zPOS_FIRST );

   CreateViewFromViewForTask( &vER2, vER, 0 );
   CreateViewFromViewForTask( &vSA2, vSA, 0 );
   CreateViewFromViewForTask( &vSA3, vSA, 0 );
   if ( SetCursorFirstEntityByInteger( vER, "ER_Entity_2", "ZKey", lZKey, "EntpER_Model" ) >= zCURSOR_SET )
   {
      do
      {
         SetCursorPrevEntity( vER, "ER_RelLink_2", 0 );
         GetIntegerFromAttribute( &lZKey1, vER, "ER_Entity_2", "ZKey" );
         SetViewFromView( vER2, vER );
         SetCursorNextEntity( vER2, "ER_RelLink_2", 0 );
         GetIntegerFromAttribute( &lZKey2, vER2, "ER_Entity_2", "ZKey" );
         if ( SetCursorFirstEntityByInteger( vSA2, "ER_Entity", "ZKey", lZKey1, "SubjectArea" ) >= zCURSOR_SET &&
              SetCursorFirstEntityByInteger( vSA3, "ER_Entity", "ZKey", lZKey2, "SubjectArea" ) >= zCURSOR_SET )
         {
            CreateMetaEntity( vSubtask, vSA, "SA_RelType", zPOS_LAST );
            IncludeSubobjectFromSubobject( vSA, "ER_RelType", vER, "ER_RelType", zPOS_BEFORE );
            SetCursorLastEntity( vSA2, "ER_RelLink", 0 );
            IncludeSubobjectFromSubobject( vSA2, "ER_RelLink", vER, "ER_RelLink_2", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vSA2, "ER_RelLink_Other", vER2, "ER_RelLink_2", zPOS_AFTER );
            SetCursorLastEntity( vSA3, "ER_RelLink", 0 );
            IncludeSubobjectFromSubobject( vSA3, "ER_RelLink", vER2, "ER_RelLink_2", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vSA3, "ER_RelLink_Other", vER, "ER_RelLink_2", zPOS_AFTER );
         }
         SetCursorNextEntity( vER, "ER_RelLink_2", 0 );
      } while ( SetCursorNextEntityByInteger( vER, "ER_Entity_2", "ZKey", lZKey, "EntpER_Model" ) >= zCURSOR_SET );
   }
   DropView( vER2 );
   DropView( vSA2 );
   DropView( vSA3 );
   return( 0 );
}
#else
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_AddOneSA_EntityFrER( zVIEW vTZERSASO,
                               zVIEW vTZEREMDO )
{
   zVIEW vTZEREMDO_Wrk;
   zVIEW vTZERSASO_Wrk;
   zSHORT nRC;
   zLONG  lEntityZKey;
   zLONG  lZKey;

   nRC = CreateViewFromViewForTask( &vTZEREMDO_Wrk, vEntpModel, 0 );
   nRC = CreateViewFromViewForTask( &vTZERSASO_Wrk, vTZERSASO, 0 );

   // Add SA_Entity and Include ER_Entity for each ER_Entity in the ER_Model
   // Position on Subject Area for each ER_Entity from the ER Model

   // First, Create SA_Entity in the Subject Area
   CreateMetaEntity( vSubtask, vTZERSASO, "SA_Entity", zPOS_AFTER );
   // Now see if the ER_Entity is allready in the Subject Area
   nRC = GetIntegerFromAttribute( &lEntityZKey, vTZEREMDO_Wrk, "ER_Entity", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vTZERSASO_Wrk, "ER_Entity", "ZKey", lEntityZKey, "SubjectArea" );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      // copy positioning from current SA_Entity and then Delete it
      SetMatchingAttributesByName( vTZERSASO, "SA_Entity", vTZERSASO_Wrk, "SA_Entity", zSET_NULL );
      DeleteEntity( vTZERSASO_Wrk, "SA_Entity", zREPOS_NONE );
   }

   // Create ER_Entity, ER_Relink, and ER_RelType_1 in the Subject Area
   // by including ER_Entity from the ER_Model.
   nRC = IncludeSubobjectFromSubobject( vTZERSASO, "ER_Entity", vTZEREMDO_Wrk, "ER_Entity", zPOS_AFTER );

   // Create ER_RelLink_Other and ER_Entity_Other in the Subject Area by including
   // ER_RelLink_Other from the ER_Model.  Position is obtained first on the
   // ER_RelType_1 in the SA from the linked ER_RelType_1 from the ER Model.
   for ( nRC = SetCursorFirstEntity( vTZERSASO, "ER_RelType_1", "ER_Entity" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERSASO, "ER_RelType_1", "ER_Entity" ) )
   {
      SetCursorFirstEntityByEntityCsr( vTZEREMDO_Wrk, "ER_RelType_1", vTZERSASO, "ER_RelType_1", "ER_Entity" );
      IncludeSubobjectFromSubobject( vTZERSASO, "ER_RelLink_Other", vTZEREMDO_Wrk, "ER_RelLink_Other", zPOS_AFTER );
      //  Create any missing RelTypes / RelLinks
      oTZERSASO_CompleteRelCreate( vTZERSASO );

   }  // End of FOR each SA_RelType_1

   return( 0 );
}
#endif

typedef zVOID (POPERATION zFARPROC_RemoveFunc)( zVIEW, zPCHAR, zBOOL, zLONG );

zOPER_EXPORT zSHORT OPERATION
oTZERSASO_RemoveOneSA_Entity( zVIEW vTZERSASO )
{
#if 1
   zVIEW vTZERSASO_Wk;
   zVIEW vSAVIEW;
   zSHORT nRC;
   zPCHAR szName;
   zCHAR szMessage[ 256 ];
   LPLIBRARY hLibrary = 0;  // used for dynamic call to ER control func's
   zFARPROC_RemoveFunc lpfn = 0;

   GetViewByName( &vSAVIEW, "SAVIEW", vTZERSASO, zLEVEL_TASK ); // Get SAVIEW if wnd open

   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" );
   strcpy_s( szMessage, sizeof( szMessage ), "OK to remove Entity from SA ?\n\n" );
   strcat_s( szMessage, sizeof( szMessage ), "Entity:  " );
   strcat_s( szMessage, sizeof( szMessage ), szName );
   nRC = MessagePrompt( vTZERSASO, "ER00301",
                        "E/R Model Maintainance", szMessage,
                        zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 );
   if ( nRC != zRESPONSE_YES )
   {
      return( -1 );
   }

   // Step 1, Remove all relationships from the subject area where the
   //         SA Entity to be deleted is the target of a relationship
   //         down the derived path.
   CreateViewFromViewForTask( &vTZERSASO_Wk, vTZERSASO, 0 );
   for ( nRC = SetCursorFirstEntityByEntityCsr( vTZERSASO_Wk, "ER_Entity_Other", vTZERSASO, "ER_Entity", "SubjectArea" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByEntityCsr( vTZERSASO_Wk, "ER_Entity_Other", vTZERSASO, "ER_Entity", "SubjectArea" ) )
   {
      ExcludeEntity( vTZERSASO_Wk, "ER_RelLink", zREPOS_NONE );
   }
   DropView( vTZERSASO_Wk );

   // Step 2, Delete all SA_RelTypes in which the subject area entity
   //         participates and tell the diagram to blast the relatinship
   //         line associated with the entity.
   for ( nRC = SetCursorFirstEntityByEntityCsr( vTZERSASO, "ER_Entity_2", vTZERSASO, "ER_Entity", "SubjectArea" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByEntityCsr( vTZERSASO, "ER_Entity_2", vTZERSASO, "ER_Entity", "SubjectArea" ) )
   {
      // Tell the diagram to blast the relationship and then delete it!
      if ( vSAVIEW )
      {
         if ( lpfn == 0 )   // only get func addr once
         {
            lpfn =(zFARPROC_RemoveFunc) GetOperationDynamicCallAddress( vSAVIEW, (LPLPLIBRARY) &hLibrary,
                                                                        "TZCTL", "RemoveRelationship", "(tzersaso)" );
         }
         if ( lpfn )
           (*lpfn)( vSAVIEW, "ERD", 0, 0 );
      }
      DeleteEntity( vTZERSASO, "SA_RelType", zREPOS_NONE );
   }  // End of FOR each SA_RelType_1

   // Step 3, Delete the SA Entity after telling the diagram to remove it
   if ( vSAVIEW )
   {
      lpfn =(zFARPROC_RemoveFunc) GetOperationDynamicCallAddress( vSAVIEW,(LPLPLIBRARY) &hLibrary,
                                                                  "TZCTL", "RemoveEntity", "(tzersaso)" );
      if ( lpfn )
        (*lpfn)( vSAVIEW, "ERD", 0, 0 );
   }
   DeleteEntity( vTZERSASO, "SA_Entity", zREPOS_NONE );

   return( 0 );
}
#else
// zVIEW vTZERSASO;
   zVIEW vSAVIEW;
   zSHORT nRC;
   zPCHAR szName;
   zCHAR szMessage[ 256 ];
   zLONG  lZKey;
   LPLIBRARY hLibrary = 0;  // used for dynamic call to ER control func's
   zFARPROC_RemoveFunc lpfn = 0;

   GetViewByName( &vSAVIEW, "SAVIEW", vSubtask, zLEVEL_TASK ); Get SAVIEW if wnd open

   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" );
   strcpy_s( szMessage, sizeof( szMessage ), "OK to remove Entity from SA ?\n\n" );
   strcat_s( szMessage, sizeof( szMessage ), "Entity: " );
   strcat_s( szMessage, sizeof( szMessage ), szName );
   nRC = MessagePrompt( vTZERSASO,                 "ER00302",
                        "E/R Model Maintainance", szMessage,
                        zBEEP,                    zBUTTONS_YESNO,
                        zRESPONSE_NO,             0 );
   if ( nRC != zRESPONSE_YES )
   {
      return( -1 );
   }

   // First step is to remove all ER_RelLink's that are connected to this
   // SA_Entity.  Also, remove any SA_RelType's that are no longer needed
   // because both SA_Entities are gone.
   for ( nRC = SetCursorFirstEntity( vTZERSASO, "ER_RelType_1", "ER_Entity" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZERSASO, "ER_RelType_1", "ER_Entity" ) )
   {
      GetIntegerFromAttribute( &lZKey, vTZERSASO, "ER_RelLink", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vTZERSASO, "ER_RelLink_2", "ZKey", lZKey, "SubjectArea" );
      if ( nRC >= zCURSOR_SET )
      {
         if ( vSAVIEW )
         {
            if ( lpfn == 0 )   // only get func addr once
            {
               lpfn =(zFARPROC_RemoveFunc) GetOperationDynamicCallAddress( vSAVIEW, (LPLPLIBRARY) &hLibrary,
                                                                           "TZCTL", "RemoveRelationship", "(tzersaso)" );
            }
            if ( lpfn )
              (*lpfn)( vSAVIEW, "ERD", 0, 0 );
         }
         nRC = ExcludeEntity( vTZERSASO, "ER_RelLink_2", zREPOS_PREV );
         if ( nRC < zCURSOR_SET )
         {
            DeleteEntity( vTZERSASO, "SA_RelType", zREPOS_PREV );
         }
         else
         {
            SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramBendPosX", 0 );
            SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramBendPosY", 0 );
            SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramE1PosX", 0 );
            SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramE1PosY", 0 );
            SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramE2PosX", 0 );
            SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramE2PosY", 0 );
         }
      }
   }  // End of FOR each SA_RelType_1

   // Now we can delete the SA_Entity
   if ( vSAVIEW )
   {
      lpfn =(zFARPROC_RemoveFunc) GetOperationDynamicCallAddress( vSAVIEW,(LPLPLIBRARY) &hLibrary,
                                                                  "TZCTL", "RemoveEntity", "(tzersaso)" );
      if ( lpfn )
        (*lpfn)( vSAVIEW, "ERD", 0, 0 );
   }
   DeleteEntity( vTZERSASO, "SA_Entity", zREPOS_NONE );

   return( 0 );
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  oTZERSASO_CompleteRelCreate
//
// PURPOSE:    This operation adds the "right" side (SA_RelType) from
//             the "left" side (ER_RelLink under SA_Entity).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_CompleteRelCreate( zVIEW vSubtask,
                             zVIEW vTZERSASO )
{
   zSHORT nRC;
   zLONG  lZKey;
   zPCHAR szName;

   OperatorPrompt( vSubtask, "Subject Area",
                   "oTZERSASO_CompleteRelCreate - This code is supposed to be disabled and never called again. However, a call "
                   "has taken place as a result of your last action! Do you promise to run and tell Gig (or Don) NOW?",
                   0, zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
   return( 0 );

   GetAddrForAttribute( &szName, vTZERSASO, "ER_Entity", "Name" ); // debug

   // Create missing RelTypes / RelLinks
   GetIntegerFromAttribute( &lZKey, vTZERSASO, "ER_RelType_1", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vTZERSASO, "ER_RelType", "ZKey", lZKey, "SubjectArea" );
   if ( nRC < zCURSOR_SET )
   {
      if ( MiGetUpdateForView( vTZERSASO ) == 0 )
      {
         TraceLineS( "SA_Complete(1): =========== ER_Entity: ", szName );  // debug only
         TraceLineI( "SA_Complete(2): ReadOnly View. ---- SA_RelType not created ZKey=", lZKey );
         return( 0 );
      }
      TraceLineS( "SA_Complete(1): =========== ER_Entity: ", szName );  // debug only
      TraceLineI( "SA_Complete(2): ReadOnly View. ---- SA_RelType created ZKey=", lZKey );
      CreateMetaEntity( vSubtask, vTZERSASO, "SA_RelType", zPOS_LAST );
      IncludeSubobjectFromSubobject( vTZERSASO, "ER_RelType", vTZERSASO, "ER_RelType_1", zPOS_AFTER );
   }

   GetIntegerFromAttribute( &lZKey, vTZERSASO, "ER_RelLink", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vTZERSASO, "ER_RelLink_2", "ZKey", lZKey, "" );
   if ( nRC < zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vTZERSASO, "ER_RelLink_2", vTZERSASO, "ER_RelLink", zPOS_AFTER );
      SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramBendPosX", 0 );
      SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramBendPosY", 0 );
      SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramE1PosX", 0 );
      SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramE1PosY", 0 );
      SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramE2PosX", 0 );
      SetAttributeFromInteger( vTZERSASO, "SA_RelType", "ER_DiagramE2PosY", 0 );
   }

   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
