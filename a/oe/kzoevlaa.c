#include "kzoephdr.h"

#include <math.h>
//
// MODULE NAME:  KZOEVLAA  -  Object services parameter validation functions
// DESCRIPTION:  This is the source file which contains Object services
//               functions which validate parameter calls
// OPERATIONS:   All functions which validate parameters
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    4/13/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2002.09.25  DGC
   Changed the way messages are displayed for nested core calls (per Phil).

2001.04.20  DGC
   Added flag to fnValidViewAttrib( ) to allow hidden attributes.

2000.08.11  DGC
   Added Blob functionality to fnCompareDebugChange( ).

2000.07.10  DGC
   Added SfCheckOI_Integrity.

2000.04.20  DGC
   Fixed crash when user tried to start an application that doesn't exist.

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

zVOID LOCALOPER
fnAddEntityToDebugChange( LPENTITYINSTANCE lpEntityInstance )
{
   LPVIEWENTITY  lpViewEntity;
   LPVIEWATTRIB  lpViewAttrib;
   LPDEBUGCHANGE lpDbgChg;
   zPVOID        hDbgChg;
   LPVIEWOI      lpViewOI;
   LPTASK        lpTask;

   if ( lpEntityInstance == 0 )
      return;

   // If the entity contains a DebugChange attribute we need to add it to
   // the list.  This will cause fnOperationCall and fnOperationReturn to
   // check to see if the attribute has been changed.
   lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
   if ( lpViewEntity->bDebugChange == FALSE )
      return;  // No DebugChange so skip it.

   lpViewOI = zGETPTR( lpEntityInstance->hViewOI );
   lpTask   = zGETPTR( lpViewOI->hTask );

   // Lock the DEBUGCHANGE mutex so that no other task will do a
   // compare while we're adding stuff.
   zLOCK_MUTEX( zMUTEX_DEBUGCHANGE );

   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      LPVIEWENTITY lpViewEntity;
      LPVIEWOD     lpViewOD;
      zCHAR        szMsg[ 100 ];

      if ( lpViewAttrib->bDebugChange == FALSE )
         continue;

      hDbgChg = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                  sizeof( DebugChangeRecord ), 1, 0,
                                  iDebugChange );
      if ( hDbgChg == 0 )
         return;

      lpDbgChg = zGETPTR( hDbgChg );
      lpDbgChg->hNextDebugChange = AnchorBlock->hFirstDebugChange;
      AnchorBlock->hFirstDebugChange = hDbgChg;

      // Copy the pertinent data to the structure.  Note that we don't copy
      // the current value here.  It is done in fnOperationReturn( ).
      lpDbgChg->hViewAttrib     = zGETHNDL( lpViewAttrib );
      lpDbgChg->hEntityInstance = zGETHNDL( lpEntityInstance );
   // if ( lpDbgChg->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "fnAddEntityToDebugChange", "UNSET_CSR", -1 );

      lpDbgChg->hViewOD         = lpViewOI->hViewOD;

      // Set compare flag to FALSE.  This will keep a comparison from
      // happening until the current Core operation is finished.  This
      // allows the attribute to be initialized.
      lpDbgChg->bCompare    = FALSE;

      lpViewEntity = zGETPTR( lpViewAttrib->hViewEntity );
      lpViewOD     = zGETPTR( lpDbgChg->hViewOD );
      sprintf_s( szMsg, sizeof( szMsg ), "%s.%s.%s", lpViewOD->szName,
                lpViewEntity->szName, lpViewAttrib->szName );
      TraceLineS( "Created DebugChange element for ", szMsg );
   }

   zUNLOCK_MUTEX( zMUTEX_DEBUGCHANGE );
}

zVOID LOCALOPER
fnRemoveEntityFromDebugChange( LPENTITYINSTANCE lpEntityInstance )
{
   LPVIEWATTRIB     lpViewAttrib;
   LPDEBUGCHANGE    lpDbgChg;
   LPDEBUGCHANGE    lpPrevDbgChg;
   LPENTITYINSTANCE hEntityInstance;

   if ( AnchorBlock->hFirstDebugChange == 0 )
      return;

   hEntityInstance = zGETHNDL( lpEntityInstance );

   zLOCK_MUTEX( zMUTEX_DEBUGCHANGE );

   // Find the DebugChange entries that match hRecord and delete them.
   // We'll skip the first DebugChange (for now) because it makes the
   // logic easier.
   lpPrevDbgChg = zGETPTR( AnchorBlock->hFirstDebugChange );
   lpDbgChg     = zGETPTR( lpPrevDbgChg->hNextDebugChange );
   while ( lpDbgChg )
   {
      LPDEBUGCHANGE lpTemp;

      lpTemp   = lpDbgChg;
      lpDbgChg = zGETPTR( lpDbgChg->hNextDebugChange );

      if ( lpTemp->hEntityInstance == hEntityInstance )
      {
         // We got a match.  Remove it.
         lpPrevDbgChg->hNextDebugChange = lpTemp->hNextDebugChange;

         lpViewAttrib = zGETPTR( lpTemp->hViewAttrib );
         if ( lpViewAttrib->cType == zTYPE_STRING &&
              lpTemp->lLastValue != 0 )
         {
            fnFreeDataspace( zGETPTR( (zPVOID) lpTemp->lLastValue ) );
         }

         fnFreeDataspace( lpTemp );
      }

      lpPrevDbgChg = zGETPTR( lpPrevDbgChg->hNextDebugChange );
   }

   // Now check the first one.
   lpDbgChg = zGETPTR( AnchorBlock->hFirstDebugChange );
   if ( lpDbgChg->hEntityInstance == hEntityInstance )
   {
      // We got a match.  Remove it.
      AnchorBlock->hFirstDebugChange = lpDbgChg->hNextDebugChange;

      lpViewAttrib = zGETPTR( lpDbgChg->hViewAttrib );
      if ( lpViewAttrib->cType == zTYPE_STRING &&
           lpDbgChg->lLastValue != 0 )
      {
         fnFreeDataspace( zGETPTR( (zPVOID) lpDbgChg->lLastValue ) );
      }

      fnFreeDataspace( lpDbgChg );
   }

   zUNLOCK_MUTEX( zMUTEX_DEBUGCHANGE );
}

zVOID LOCALOPER
fnPrintDebugFromSameInstance( LPDEBUGCHANGE lpDbgChg )
{
   LPVIEWENTITY     lpViewEntity;
   LPVIEWATTRIB     lpViewAttrib;
   LPVIEWOD         lpViewOD;
   LPVIEWOI         lpViewOI;
   LPENTITYINSTANCE hEntityInstance = lpDbgChg->hEntityInstance;
   LPENTITYINSTANCE lpEntityInstance = zGETPTR( lpDbgChg->hEntityInstance );
   LPDEBUGCHANGE    lpTemp;

   zLOCK_MUTEX( zMUTEX_DEBUGCHANGE );

   for ( lpTemp = zGETPTR( AnchorBlock->hFirstDebugChange );
         lpTemp;
         lpTemp = zGETPTR( lpTemp->hNextDebugChange ) )
   {
      zPCHAR pchCurrent;
      zCHAR  szMsg[ 200 ];

      if ( lpTemp->bInProgress )
         continue;

      // bCompare will be FALSE if we don't want to compare this attribute.
      // bCompare is FALSE when we initialize the attribute.
      if ( lpTemp->bCompare == FALSE )
         continue;

      if ( lpTemp == lpDbgChg )
         continue;

      if ( lpTemp->hEntityInstance != hEntityInstance )
         continue;

      lpViewAttrib = zGETPTR( lpTemp->hViewAttrib );
      pchCurrent = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

      TraceLineS( "------- Attribute from same EI ----------", "" );
      lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
      lpViewOI     = zGETPTR( lpEntityInstance->hViewOI );
      lpViewOD     = zGETPTR( lpViewOI->hViewOD );
      sprintf_s( szMsg, sizeof( szMsg ), "%s.%s.%s", lpViewOD->szName,
                lpViewEntity->szName, lpViewAttrib->szName );
      TraceLineS( "Attribute = ", szMsg );

      switch ( lpViewAttrib->cType )
      {
         case zTYPE_INTEGER:
            TraceLineI( "Old value = ", lpTemp->lLastValue );
            TraceLineI( "New Value = ", *((zPLONG) pchCurrent) );
            break;

         case zTYPE_DECIMAL:
            break;

         case zTYPE_STRING:
         {
            zPCHAR pchLastValue = (zPCHAR) zGETPTR( lpTemp->lLastValue );

            if ( *pchCurrent == '\xff' )
            {
               pchCurrent++;
               pchCurrent = (zPCHAR) zGETPTR( *((zPPVOID) pchCurrent) );
            }

            TraceLineS( "Old value = ", pchLastValue );
            TraceLineS( "New Value = ", pchCurrent );

            break;
         }

         case zTYPE_BLOB:
            break;

         case zTYPE_DATETIME:
            break;
      }

      TraceLineS( "", "" );
   }

   zUNLOCK_MUTEX( zMUTEX_DEBUGCHANGE );
}

zVOID LOCALOPER
fnCompareDebugChange( zBOOL bOperationCall, LPTASK lpTask )
{
   LPDEBUGCHANGE lpDbgChg;
   LPVIEWATTRIB  lpViewAttrib;

   zLOCK_MUTEX( zMUTEX_DEBUGCHANGE );

   for ( lpDbgChg = zGETPTR( AnchorBlock->hFirstDebugChange );
         lpDbgChg;
         lpDbgChg = zGETPTR( lpDbgChg->hNextDebugChange ) )
   {
      zPCHAR pchCurrent;
      zBOOL  bDifference;

      lpViewAttrib = zGETPTR( lpDbgChg->hViewAttrib );
      pchCurrent = fnRecordForEntityAttr( zGETPTR( lpDbgChg->hEntityInstance ),
                                          lpViewAttrib );

      // If pchCurrent is NULL then we haven't created an attribute yet
      // so just skip it.
      if ( pchCurrent == 0 )
         continue;

      pchCurrent = pchCurrent + lpViewAttrib->ulRecordOffset;

      if ( lpDbgChg->bCompare == FALSE )
      {
         // If the bCompare flag is FALSE then we haven't set up the attribute
         // value yet.  We only do this in fnOperationExit so if we are in
         // fnOperationCall just continue to the next DebugChange.
         if ( bOperationCall )
            continue;

         // We only set up the attribute value if we are exiting the last
         // (or topmost) Zeidon operation.  This allows the attribute to
         // be initialized.
         if ( lpTask->nOperIdx > 0 )
            continue;

         // Copy the current value for later comparison.
         switch ( lpViewAttrib->cType )
         {
            case zTYPE_INTEGER:
               lpDbgChg->lLastValue = *((zPLONG) pchCurrent);
               break;

            case zTYPE_DECIMAL:
               break;

            case zTYPE_STRING:
               if ( *pchCurrent == '\xff' )
               {
                  pchCurrent++;
                  pchCurrent = (zPCHAR) zGETPTR( *((zPPVOID) pchCurrent) );
               }

               if ( *pchCurrent )
               {
                  LPTASK  lpMainTask = zGETPTR( AnchorBlock->hMainTask );

                  lpDbgChg->lLastValue = (zLONG)
                     fnStoreStringInDataspace( lpMainTask->hFirstDataHeader,
                                               pchCurrent );

                  if ( !zisalnum( *pchCurrent ) )
                     TraceLineS( "(DbgChg) Storing ", pchCurrent );
               }
               else
                  lpDbgChg->lLastValue = 0;

               break;

            case zTYPE_BLOB:
            {
               zULONG ulLth;
               zPVOID hndl, lpBlob;

               if ( *pchCurrent == '\xff' )
               {
                  LPTASK  lpMainTask = zGETPTR( AnchorBlock->hMainTask );
                  zPVOID  lp;

                  pchCurrent++;
                  ulLth = *((zPULONG) ((zCOREMEM) pchCurrent + 1));

                  hndl = fnAllocDataspace( lpMainTask->hFirstDataHeader,
                                           ulLth, 0, 0, iBlob );
                  lpBlob = zGETPTR( hndl );
                  lp = zGETPTR( *((zCOREMEM) pchCurrent) );
                  zmemcpy( lpBlob, lp, ulLth );
                  lpDbgChg->lLastValue = (zLONG) hndl;
                  lpDbgChg->ulLth = ulLth;
               }
               else
               {
                  lpDbgChg->lLastValue = 0;
                  lpDbgChg->ulLth = 0;
               }

               break;
            }

            case zTYPE_DATETIME:
               break;
         }

         lpDbgChg->bCompare = TRUE;

         // We're done with this lpDbgChg so go to the next one.
         continue;

      } // if ( lpDbgChg->bCompare == FALSE )...

      if ( lpDbgChg->bInProgress )
         continue;

      lpDbgChg->bInProgress = TRUE;

      // Compare the current value of the attribute with the one we saved.
      bDifference  = FALSE;

      switch ( lpViewAttrib->cType )
      {
         case zTYPE_INTEGER:
            if ( lpDbgChg->lLastValue != *((zPLONG) pchCurrent) )
            {
               bDifference = TRUE;
               TraceLineI( "Old value = ", lpDbgChg->lLastValue );
               TraceLineI( "New Value = ", *((zPLONG) pchCurrent) );
            }

            break;

         case zTYPE_DECIMAL:
            break;

         case zTYPE_STRING:
         {
            zPCHAR pchLastValue = (zPCHAR) zGETPTR( lpDbgChg->lLastValue );

            if ( *pchCurrent == '\xff' )
            {
               pchCurrent++;
               pchCurrent = (zPCHAR) zGETPTR( *((zPPVOID) pchCurrent) );
            }

            if ( pchCurrent && *pchCurrent == 0 )
               pchCurrent = 0;

            // If both values are null then they are equal.  They can only
            // be equal if they are both null.
            if ( pchCurrent == pchLastValue )
               break;

            if ( pchLastValue == 0 || pchCurrent == 0 ||
                 zstrcmp( pchLastValue, pchCurrent ) != 0 )
            {
               bDifference = TRUE;
               TraceLineS( "Old value = ", pchLastValue );
               TraceLineS( "New Value = ", pchCurrent );

               // Free the dataspace since we don't need it anymore.
               if ( pchLastValue )
               {
                  fnFreeDataspace( pchLastValue );
                  lpDbgChg->lLastValue = 0;
               }
            }

            break;
         }

         case zTYPE_BLOB:
         {
            zULONG ulLth;
            zPVOID lpBlob;
            zPVOID lpLastValue = (zPVOID) zGETPTR( lpDbgChg->lLastValue );

            if ( *pchCurrent == '\xff' )
            {
               pchCurrent++;
               lpBlob = zGETPTR( *((zCOREMEM) pchCurrent) );
               ulLth = *((zPULONG) ((zCOREMEM) pchCurrent + 1));
            }
            else
               ulLth = 0;

            // If both blobs have a zero length then they are equal.
            if ( ulLth == 0 && lpDbgChg->ulLth == 0 )
               break;

            if ( ulLth != lpDbgChg->ulLth ||
                 zmemcmp( lpLastValue, lpBlob, ulLth ) != 0 )
            {
               bDifference = TRUE;

               if ( ulLth != lpDbgChg->ulLth )
               {
                  TraceLineI( "Old lth = ", lpDbgChg->ulLth );
                  TraceLineI( "New lth = ", ulLth );
               }
               else
                  TraceLineS( "Same lth but different values", "" );

               TraceBuffer( "Old ==> ", lpLastValue, lpDbgChg->ulLth );
               TraceBuffer( "New ==> ", lpBlob, ulLth );

               // Free the dataspace since we don't need it anymore.
               if ( lpDbgChg->ulLth )
               {
                  fnFreeDataspace( lpLastValue );
                  lpDbgChg->lLastValue = 0;
                  lpDbgChg->ulLth      = 0;
               }
            }

            break;
         }

         case zTYPE_DATETIME:
            break;
      }

      if ( bDifference )
      {
         zCHAR        szMsg[ 200 ];
         LPVIEWOD     lpViewOD = zGETPTR( lpDbgChg->hViewOD );
         LPVIEWENTITY lpViewEntity = zGETPTR( lpViewAttrib->hViewEntity );

         // Change bCompare flag so we don't do any more compares.  This will
         // also cause a new copy of the value to stored in lpDbgChg.
         lpDbgChg->bCompare = FALSE;

         fnPrintDebugFromSameInstance( lpDbgChg );

         sprintf_s( szMsg, sizeof( szMsg ), "Operation%s: Core detected a change in the attribute "
                   "%s.%s.%s.  See Trace for old and new values.",
                   bOperationCall ? "Start" : "Exit", lpViewOD->szName,
                   lpViewEntity->szName, lpViewAttrib->szName );

         zUNLOCK_MUTEX( zMUTEX_DEBUGCHANGE );

         fnSysMessageBox( lpTask, "Debug Change", szMsg, 1 );

         zLOCK_MUTEX( zMUTEX_DEBUGCHANGE );

         // If we are in fnOperationExit and if the task oper idx is 0 then
         // we'll call ourselves recursively to set up the next compare.
         //if ( bOperationCall == FALSE && lpTask->nOperIdx == 0 )
         if ( bOperationCall == FALSE )
            fnCompareDebugChange( FALSE, lpTask );
      }

      lpDbgChg->bInProgress = FALSE;

   } // for ( lpDbgChg )...

   zUNLOCK_MUTEX( zMUTEX_DEBUGCHANGE );

} // fnCompareDebugChange

#if 1  // for debugging purposes only
zPCHAR g_Operations[ 556 ] =
   {
// 1-50     registration and system stuff 0 is RESERVED!
      "?", "", "", "", "", "", "", "", "", "",   //  0-9
      "(10)SfCreateSubtask",
      "(11)SfDropSubtask",
      "(12)DisableTask",
      "(13)EnableTask",
      "", "", "", "", "", "",                    //  14-19
      "(20)GetApplDirectoryFromView",
      "(21)SfGetApplicationForSubtask",
      "(22)SfSetApplicationForSubtask",
      "(23)SfGetDefaultContextForDomain",
      "(24)SfGetTaskInfo",
      "(25)SfAllocTaskMemory",
      "(26)SfFreeTaskMemory",
      "(27)SfReAllocTaskMemory",
      "(28)SfCheckOI_Integrity",
      "(29)SetDatabaseUser",
      "", "", "", "", "", "", "", "", "", "",    //  30-39
      "", "", "", "", "", "", "", "", "", "",    //  40-49

// 50-100   Instance activation
      "(50)ActivateEmptyObjectInstance",
      "",
      "(52)ActivateObjectInstance",
      "",
      "(54)ActivateOI_FromFile",
      "(55)ActivateOI_FromOI_ForTask",
      "",
      "",
      "",
      "",
      "(60)CommitObjectInstance",
      "(61)CommitOI_ToFile",
      "(62)CommitMultipleOIs",
      "(63)DropViewCluster",
      "(64)AddToViewCluster",
      "(65)GenerateQualFromEntityList",
      "",
      "",
      "",
      "",
      "(70)DropObjectInstance",
      "", "", "", "", "", "", "", "", "", "",    //  71-80
      "", "", "", "", "", "", "", "", "", "",    //  81-90
      "", "", "", "", "", "", "", "", "",        //  91-99

// 100-150  Entity creation/deletion etc
      "(100)CheckExistenceOfEntity",
      "(101)CreateEntity",
      "(102)CreateTemporalSubobjectVersion",
      "(103)AcceptSubobject",
      "(104)CancelSubobject",
      "(105)IncludeSubobjectFromSubobject",
      "(106)DeleteEntity",
      "(107)ExcludeEntity",
      "(108)GetEntityKey",
      "",
      "(110)MoveSubobject",
      "",
      "(112)GetSelectStateOfEntity",
      "(113)SetAllSelectStatesForEntity",
      "(114)SetSelectStateOfEntity",
      "(115)CreateTemporalEntity",
      "(116)RelinkInstanceToInstance",
      "(117)LoadEntity",
      "(118)ObjectInstanceUpdated",
      "(119)SetIncrementalUpdateFlags",
      "(120)GetIncrementalUpdateFlags",
      "(121)RelinkAllSubobjectsForOI",
      "(122)OrderEntityForView",
      "(123)CountEntitiesForView",
      "(124)OrderEntitiesByDefaultAttribs",
      "(125)OrderOI_ByDefaultAttribs",
      "(126)DropEntity",
      "(127)AcceptAllTemporalSubobjects",
      "(128)CancelAllTemporalSubobjects",
      "(129)SetSelectSetForView",
      "(130)ObjectInstanceUpdatedFromFile",
      "(131)CountEntitiesForViewUnderParent",
      "(132)CheckEntityInView",
      "(133)IncludeSubobjectFromSubobjectEx",
      "(134)SubobjectVersionUpdated",
      "", "", "", "", "",                        //  135-139
      "", "", "", "", "", "", "", "", "", "",    //  140-149

// 150-200  Cursor navigation
      "(150)SetCursorNextEntity",
      "(151)SetCursorPrevEntity",
      "(152)SetCursorFirstEntity",
      "(153)SetCursorLastEntity",
      "", "", "", "", "", "",                    //  154-159
      "", "", "", "", "", "", "", "", "", "",    //  160-169
      "(170)SetCursorFirstEntityByString",
      "(171)SetCursorNextEntityByString",
      "(172)SetCursorFirstEntityByInteger",
      "(173)SetCursorNextEntityByInteger",
      "(174)SetCursorFirstEntityByDecimal",
      "(175)SetCursorNextEntityByDecimal",
      "(176)SetCursorFirstEntityByAttr",
      "(177)SetCursorNextEntityByAttr",
      "(178)SetCursorFirstEntityByEntityCs",
      "(179)SetCursorNextEntityByEntityCsr",
      "(180)SetCursorFirstSelectedEntity",
      "(181)SetCursorNextSelectedEntity",
      "(182)SetCursorRelativeEntity",
      "(183)GetRelativeEntityNumber",
      "(184)SetEntityCursor",
      "",
      "(186)GetAbsolutePositionForEntity",
      "(187)SetCursorAbsolutePosition",
      "", "",                                    //  188-189
//
// Note: All non hierarchical operations should have
//       id's that are less than iDefineHierarchicalCursor,
//       otherwise an error will occur in fnValidateCursorParameters
//
      "(190)DefineHierarchicalCursor",
      "(191)DropHierarchicalCursor",
      "(192)SetCursorNextEntityHierarchica",
      "(193)GetEntityNameForHierarchicalCsr",
      "", "", "", "", "", "",                    //  194-199

// 200-249  View management
      "(200)CreateViewFromViewForTask",
      "(201)SetViewFromView",
      "(202)SetViewToSubobject",
      "(203)ResetViewFromSubobject",
      "(204)SetNameForView",
      "(205)DropNameForView",
      "(206)GetViewByName",
      "(207)SetSubtaskView",
      "(208)GetNameForView",
      "(209)SfGetFirstSubtaskView",
      "(210)SfGetNextSubtaskView",
      "(211)ResetView",
      "(212)DropView",
      "(213)SfTransferView",
      "(214)SfLockView",
      "(215)SfActivateSysOI_FromFile",
      "(216)SfActivateSysEmptyOI",
      "(217)SetViewReadOnly",
      "(218)SfGetFirstNamedView",
      "(219)SfGetNextNamedView",
      "(220)SfCreateSysViewFromView",
      "(221)ResetViewPositions",
      "(222)SetViewFlags",
      "(223)DropViewObject",
      "(224)GetViewFlags",
      "(225)SfSetApplicationTask",
      "", "", "", "",                            //  226-229
      "", "", "", "", "", "", "", "", "", "",    //  230-239
      "", "", "", "", "", "", "", "", "", "",    //  240-249

// 250-300  Attribute retrieval/setting
      "(250)GetStringFromAttribute",
      "(251)GetIntegerFromAttribute",
      "(252)GetDecimalFromAttribute",
      "(253)GetBlobFromAttribute",
      "(254)GetAttributeFlags",
      "(255)AttributeUpdated",
      "", "",                                    //  256-257
      "(258)GetStructFromEntityAttrs",
      "(259)GetAddrForAttribute",
      "(260)SetAttributeFromString",
      "(261)SetAttributeFromInteger",
      "(262)SetAttributeFromDecimal",
      "(263)SetAttributeFromBlob",
      "(264)SetAttributeFromAttribute",
      "(265)SetAttributeFromCurrentDate",
      "(266)AddToAttributeFromInteger",
      "(267)AddToAttributeFromDecimal",
      "(268)AddToAttributeFromAttribute",
      "",
      "(270)CompareAttributeToString",
      "(271)CompareAttributeToInteger",
      "(272)CompareAttributeToDecimal",
      "(273)CompareAttributeToAttribute",
      "(274)GetAttributeLength",
      "(275)SetMatchingAttributesByName",
      "(276)SetBlobAttributeFromAttribute",
      "(277)SetAttributeFromVariable",
      "(278)CompareAttributeToVariable",
      "(279)GetVariableFromAttribute",
      "(280)GetFirstTableEntryForAttribute",
      "(281)GetNextTableEntryForAttribute",
      "(282)SetAttributeValueNext",
      "(283)SetAttributeValuePrev",
      "(284)StoreValueInRecord",
      "(285)AddToAttributeFromVariable",
      "(286)GetValueFromRecord",
      "(287)StoreStringInRecord",
      "(288)GetStringFromRecord",
      "(289)GetActualAttributeLength",
      "(290)GetAddrFromRecord",
      "(291)SetBlobFromEntityAttributes",
      "(292)SetEntityAttributesFromBlob",
      "(293)GetAttributeDisplayLength",
      "(294)SetBlobFromOI",
      "(295)SetOI_FromBlob",
      "(296)SetBlobFromFile",
      "(297)WriteBlobToFile",
      "(298)StoreValueInDerivedAttribute",
      "(299)StoreStringInDerivedAttribute",
      "", "", "", "", "", "", "", "", "", "",    //  300-309
      "", "", "", "", "", "", "", "", "", "",    //  310-319
      "", "", "", "", "", "", "", "", "", "",    //  320-329
      "", "", "", "", "", "", "", "", "", "",    //  330-339
      "", "", "", "", "", "", "", "", "", "",    //  340-349

// 350-359  Domain operations
      "(350)TableEntryExtToInt",
      "(351)TableEntryIntToExt",
      "(352)TableEntryForInternalValue",

      "", "", "", "", "", "", "",                //  353-359

// 360-369 Message Object operations
      "(360)MessageSend",
      "(361)MessagePrompt",
      "(362)MessagePromptForInput",
      "(363)MessagePresent",

      "", "", "", "", "", "",                    //  364-369

      "(370)UfAddToDateTime",

// 371-399 open

      "", "", "", "", "", "", "", "", "",        //  371-379
      "", "", "", "", "", "", "", "", "", "",    //  380-309
      "", "", "", "", "", "", "", "", "", "",    //  390-399

// 400-449  Tracing operations
      "(400)DisplayEntityInstance",
      "(401)DisplayObjectInstance",
      "(402)DisplayCursorInfo",
      "(403)GetTaskDBHandlerTraceLevel",
      "(404)SetTaskDBHandlerTraceLevel",
      "(405)DisplayEntityInstancePath",
      "(406)CompareEntityToEntity",
      "(407)CompareOI_ToOI",
      "", "",                                    //  408-409
      "", "", "", "", "", "", "", "", "", "",    //  410-419
      "", "", "", "", "", "", "", "", "", "",    //  420-429
      "", "", "", "", "", "", "", "", "", "",    //  430-439
      "", "", "", "", "", "", "", "", "", "",    //  440-449

// 450-499  Meta Inquiry operations
      "(450)MiGetParentEntityNameForView",
      "(451)MiGetInstanceID_ForView",
      "(452)MiGetUpdateForView",
      "(453)MiGetUpdateForViewEntityAttr",
      "(454)MiGetTemporalStateOfEntity",
      "(455)MiGetDateTimeForOI",
      "(456)MiGetERTokenForEntity",
      "(457)MiGetKeyFromInstance",
      "(458)MiGetViewEntityForView",
      "(459)MiGetCardinalityForEntity",
      "(460)MiGetObjectNameForView",
      "(461)MiGetOI_ReleaseForView",
      "(462)MiSetOI_ReleaseForView",
      "(463)MiCompareOI_ReleaseToRelease",
      "(464)MiGetActivateFlags",
      "(465)MiHasPersistentChanges",
      "(466)MiSetInstanceUpdateFlag",
      "", "", "",                                //  467-469
      "", "", "", "", "", "", "", "", "", "",    //  470-479
      "", "", "", "", "", "", "", "", "", "",    //  480-489
      "", "", "", "", "", "", "", "", "", "",    //  490-499

// 500-549  Network operations
      "(500)NetActivateOI",
      "(501)NetCommitOI",
      "(502)NetClose",
      "(503)NetGetTraceLevel",
      "(504)NetListen",
      "(505)NetProcessMessage",
      "(506)NetSetTraceLevel",
      "(507)NetStartup",
      "(508)NetStatus",
      "(509)NetStopListen",
      "(510)NetCommitOI_ToFile",
      "(511)NetActivateOI_FromFile",
      "(512)NetSendBootstrapFile",
      "(513)NetCallOperation",
      "(514)NetGetLocalHostAddress",
      "(515)NetSendFile",

      "", "", "", "",                            //  516-519
      "", "", "", "", "", "", "", "", "", "",    //  520-529
      "", "", "", "", "", "", "", "", "", "",    //  530-539
      "", "", "", "", "", "", "", "", "", "",    //  540-549

// 550-559  XML operations
      "(550)WriteOI_ToXML",
      "(551)ActivateOI_FromXML_File",
      "(552)CommitOI_ToXML_File",
      "(553)WriteSubobjectToXML",
      "(554)CommitSubobjectToXML_File",
   };
#endif

//./ ADD NAME=fnOperationCall
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnOperationCall
//
//  PURPOSE:    To validate the task and check to see if Object
//              services is active for the task;
//
//  lpTaskView is a view that can be used to find the current task.
//  lFlag   zVALID_VIEW         - perform fnValidView on lpTaskView
//          zVALID_SUBTASK_VIEW - perform fnValidSubtaskView on lpTaskView
//          zVALID_VIEW_OBJECT  - perform fnValidViewObject on lpTaskView
//          zVALID_VIEW_CSR     - perform fnValidViewCsr on lpTaskView
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
LPTASK
fnOperationCall( zSHORT    nOperationID,
                 zVIEW     lpTaskView,
                 zLONG     lFlag )
{
   LPTASK    lpTask = 0;
   zPLONG    plCount;

   if ( AnchorBlock == 0 )
      return( 0 );

   if ( g_vRealTaskView )  // to fix object browser problem
      lpTaskView = g_vRealTaskView;

   if ( lpTaskView == 0 || lpTaskView->hTask == 0 ||
        (lpTask = zGETPTR( lpTaskView->hTask )) == 0 )
   {
      zCHAR  szMessage[ 32 ];

      TraceLine( "KZOEE101 - Invalid view 0x%08x for Task: 0x%08x",
                 lpTaskView, lpTaskView ? lpTaskView->hTask : 0 );

      SysReadZeidonIni( -1, "[Debug]", "BombZDr", szMessage, sizeof( szMessage ) );
      if ( szMessage[ 0 ] == 'Y' )
      {
         int k = 0;
         k /= k;
      }

      if ( AnchorBlock )
      {
         if ( lpTask == 0 )
            lpTask = zGETPTR( AnchorBlock->hMainTask );

         if ( lpTaskView == 0 )
            lpTaskView = zGETPTR( lpTask->hFirstView );

         //  "KZOEE101 - Invalid View"
         fnIssueCoreError( lpTask, lpTaskView, 8, 101, 0, 0, 0 );
      }

      return( 0 );
   }

   if ( lpTask->bDisable && nOperationID != iEnableTask )
   {
      //  "KZOEE017 - Task is Disabled"
   // fnIssueCoreError( zGETPTR( lpTaskView->hTask ), lpTaskView,
   //                   16, 17, 0, 0, 0 );
   // TraceLineS( "fnOperationCall: ", "2" );
      return( 0 );
   }

// if ( ((lFlag & zVALID_VIEW) && lpTaskView == 0) ||  // don/dks 2006.04.27
   if ( ((lFlag & zVALID_VIEW) && fnValidView( lpTask, lpTaskView ) == 0) ||
        ((lFlag & zVALID_VIEW_CSR) == zVALID_VIEW_CSR &&
          fnValidViewCsr( lpTask, lpTaskView ) == 0) ||
        ((lFlag & zVALID_VIEW_OBJECT) == zVALID_VIEW_OBJECT &&
          fnValidViewObject( lpTask, lpTaskView ) == 0) ||
        ((lFlag & zVALID_SUBTASK_VIEW) == zVALID_SUBTASK_VIEW &&
          fnValidSubtaskView( lpTask, lpTaskView ) == 0) )
   {
      return( 0 );
   }

#if 0  // this happens quite a bit
   if ( nOperationID == iDropViewCluster )
   {
      zVIEW v = GetDefaultViewForActiveTask( );
   // if ( v && v->hTask != lpTaskView->hTask )
      {
         TraceLine( "FNOPERATIONCALL ID: %d-%s  Task: 0x%08x   TaskIn: 0x%08x   PreIdx: %d",
                    nOperationID, g_Operations[ nOperationID ],
                    v ? v->hTask : 0, lpTaskView->hTask, lpTask->nOperIdx );
      }
   }
#endif

   if ( lpTask->bAudit ) // is operation auditing enabled
   {
   // TraceLineS( "fnOperationCall: ", "Audit" );
      if ( lpTask->nOperIdx < -1 )
      {
      // lpTask->nOperIdx /= lpTask->nOperIdx - lpTask->nOperIdx;
         lpTask->nOperIdx = -1;

         //  "KZOEE013 - Internal operation call error"
         TraceLineS( "KZOEE013 - Internal operation call error: ",
                     "Auditing error" );
         fnIssueCoreError( zGETPTR( lpTaskView->hTask ), lpTaskView,
                           16, 13, 0, 0, 0 );
      }

      lpTask->nOperIdx++;
      if ( lpTask->nOperIdx <= 9 )
         lpTask->nOper[ lpTask->nOperIdx ] = nOperationID;
      else
      if ( lpTask->nOperIdx > 15 )
      {
         TraceLineS( "Zeidon Core Internal Error: ",
                     "Too many nested Core calls" );
         return( 0 );
      }
      else
      if ( lpTask->nOperIdx == 15 )
      {
         TraceLineS( "fnOperationCall: ", "Too many nested Core calls" );
         SysMessageBox( lpTaskView, "Zeidon Core Internal Error",
                        " Too many nested Core calls. "
                        "(This is the last message.  "
                        "Subsequent errors will be traced only)", 0 );
         return( 0 );
      }
      else
      {
         TraceLineS( "fnOperationCall: ", "Too many nested Core calls2" );
         SysMessageBox( lpTaskView, "Zeidon Core Internal Error",
                        " Too many nested Core calls", 0 );
         return( 0 );
      }

      // Is operation call counting enabled.
      if ( lpTask->hCountBuf && lpTask->bCount )
      {
         plCount = (zPLONG) zGETPTR( lpTask->hCountBuf );
         plCount += (2 * nOperationID);
         if ( lpTask->nOperIdx )
            plCount++;

         (*plCount)++;
      }
   }
   else
   {
      lpTask->nOperIdx++;
   }

#if 0  // this happens quite a bit
// if ( nOperationID == iDropViewCluster )
   {
      zVIEW v = GetDefaultViewForActiveTask( );
      TraceLine( "fnOperationCall ID: %d-%s  Task: 0x%08x   TaskIn: 0x%08x   Idx: %d",
                 nOperationID, g_Operations[ nOperationID ],
                 v ? v->hTask : 0, lpTaskView->hTask, lpTask->nOperIdx );
   }
#endif

   if ( AnchorBlock->hFirstDebugChange )
   {
   // TraceLineS( "fnOperationCall: ", "FirstDebugChange" );
      fnCompareDebugChange( TRUE, lpTask );
   }

// TraceLineS( "fnOperationCall: ", "Return" );
   return( lpTask );
}

//./ ADD NAME=fnOperationReturn
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnOperationReturn
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
void
fnOperationReturn( zSHORT    nOperationID,
                   LPTASK    lpTask )
{
#if 0  // this happens quite a bit
// if ( nOperationID == iDropViewCluster )
   {
      zVIEW v = GetDefaultViewForActiveTask( );
   // if ( v && v->hTask != zGETHNDL( lpTask ) )
      {
         TraceLine( "fnOperationReturn ID: %d-%s   Task: 0x%08x, TaskIn: 0x%08x   Idx: %d",
                    nOperationID, g_Operations[ nOperationID ],
                    v ? v->hTask : 0, lpTask ? zGETHNDL( lpTask ) : 0,
                    lpTask->nOperIdx );
      }
   }
#endif

   if ( lpTask )
   {
      if ( lpTask->bAudit ) // is operation auditing enabled
      {
         int k;

         k = lpTask->nOperIdx;
         if ( k >= 0 &&
              (nOperationID == 0 || lpTask->nOper[ k ] == nOperationID) )
         {
            lpTask->nOperIdx--;
         }
         else   // Try and recover lost index
         {
            //  "KZOEE013 - Internal operation call error"
            TraceLine( "KZOEE013 - Internal operation (%d) %s call error: "
                       "Recovering lost index: %d for Task: 0x%08x",
                       nOperationID,
                       g_Operations[ nOperationID ], k, zGETHNDL( lpTask ) );
            fnSysMessageBox( lpTask, "Zeidon Core Error",
                             "KZOEE013 - Internal operation call error.", 1 );
            while ( k >= 0 && lpTask->nOper[ k ] != nOperationID )
               k--;

            if ( k >= 0 )
               lpTask->nOperIdx--;
         }
      }
      else
      {
         lpTask->nOperIdx--;
      }
   }

#if 0  // this happens quite a bit
   if ( nOperationID == iDropViewCluster )
   {
      zVIEW v = GetDefaultViewForActiveTask( );
   // if ( v && v->hTask != zGETHNDL( lpTask ) )
      {
         TraceLine( "FNOPERATIONRETURN ID: %d-%s   Task: 0x%08x, TaskIn: 0x%08x   Idx: %d",
                    nOperationID, g_Operations[ nOperationID ],
                    v ? v->hTask : 0, lpTask ? zGETHNDL( lpTask ) : 0,
                    lpTask->nOperIdx );
      }
   }
#endif

   if ( lpTask && AnchorBlock->hFirstDebugChange )
      fnCompareDebugChange( FALSE, lpTask );
}

//./ ADD NAME=fnValidView
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidView
//
//  ASSUMES:    Task passed is valid
//
//  RETURNS:    1 - View valid
//              0 - View invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnValidView( LPTASK lpTask, zVIEW lpView )
{
   if ( lpView )
   {
      if ( lpTask && lpTask->bDebug )   // Debug level validation
      {
#if 1
         if ( SysValidAddress( (zPVOID) lpView, (zLONG) sizeof( ViewRecord ), 1 ) )
         {
            if ( lpView->nTableID == iView )
               return( 1 );
         }
#else
         LPTASK    lpSearchTask;
         zVIEW     lpSearchView;
         LPAPP     lpSearchApp;

         // Check application bit.  If it is set, then the view is part of
         // an application, not a task--search for the view in all the
         // applications.  If it is not set, then look for the view in the
         // task.

         // Check the views for this task, in order to try to find a
         // matching view.
         lpSearchView = zGETPTR( lpTask->hFirstView );
         while ( lpSearchView )
         {
            if ( lpSearchView == lpView )
               return( 1 );

            lpSearchView = zGETPTR( lpSearchView->hNextView );
         }

         // If the view was not found in the current task, look in the
         // Application, for the task, for the view.
         if ( lpSearchView == 0 )
         {
            LPAPP lpApp = zGETPTR( lpTask->hApp );

            lpSearchView = zGETPTR( lpApp->hFirstView );
            while ( lpSearchView )
            {
               // If a match was found for views, then lpView is ok.
               if ( lpSearchView == lpView )
                  return( 1 );

               // Point lpSearchView to next view in chain.
               lpSearchView = zGETPTR( lpSearchView->hNextView );
            }
         }

         // If the view was not found in the application for the current
         // task, look in all tasks for the view.
         if ( lpSearchView == 0 )
         {
            // Make sure nobody else is deleting a task.
            fnStartBrowseOfTaskList( zGETHNDL( lpTask ), FALSE );

            // Point to the first task.
            lpSearchTask = zGETPTR( AnchorBlock->hFirstTask );

            // While there are tasks, look through the view chain for each
            // task, looking for a view match.
            while ( lpSearchTask )
            {
               if ( lpSearchTask != lpTask )
               {
                  lpSearchView = zGETPTR( lpSearchTask->hFirstView );
                  while ( lpSearchView )
                  {
                     if ( lpSearchView == lpView )
                     {
                        fnEndBrowseOfTaskList( FALSE );
                        return( 1 );
                     }

                     lpSearchView = zGETPTR( lpSearchView->hNextView );
                  }
               }

               lpSearchTask = zGETPTR( lpSearchTask->hNextTask );
            }

            fnEndBrowseOfTaskList( FALSE );
         }

         // If the view has not been found, look in all apps for the view.
         if ( lpSearchView == 0 )
         {
            zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

            // Set lpSearchApp to first application.
            lpSearchApp = zGETPTR( AnchorBlock->hFirstApp );

            // While there are applications, look through the view chain for
            // each application looking for a view match.
            while ( lpSearchApp )
            {
               lpSearchView = zGETPTR( lpSearchApp->hFirstView );
               while ( lpSearchView )
               {
                  // If a match was found for views, then lpView is ok.
                  if ( lpSearchView == lpView )
                     return( 1 );

                  // Point lpSearchView to next view in chain.
                  lpSearchView = zGETPTR( lpSearchView->hNextView );
               }

               // Point lpSearchApp to next app in chain.
               lpSearchApp = zGETPTR( lpSearchApp->hNextApp );
            }

            zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
         }
#endif
      }     // if ( lpTask->bDebug )...
      else                   // Non-Debug level checking
      {
         if ( lpView->nTableID == iView )
            return( 1 );
      }
   }

   if ( lpTask->bTransientTask == FALSE )
   {
      zCHAR szMessage[ 32 ];

      TraceLine( "CheckValidView found Invalid view: 0x%08x", lpView );
      SysReadZeidonIni( -1, "[Debug]", "BombZDr", szMessage, sizeof( szMessage ) );
      if ( szMessage[ 0 ] == 'Y' )
      {
         int k = 0;
         k /= k;
      }
   }

   //  "KZOEE101 - Invalid View"
   fnIssueCoreError( lpTask, lpView, 8, 101, 0, 0, 0 );

//?fnOperationReturn( 0, lpTask, 0 );  dks???  2004.11.05
   return( 0 );  // return error
}

//./ ADD NAME=fnValidSubtaskView
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidSubtaskView
//
//  ASSUMES:    View passed is a valid subtask view
//
//  RETURNS:    1 - View valid
//              0 - View invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnValidSubtaskView( LPTASK lpTask, zVIEW  lpView )
{
   if ( fnValidView( lpTask, lpView ) )
   {
      if ( lpView->hSubtask == 0 )
      {
         //  "KZOEE107 - Invalid Subtask View"
         fnIssueCoreError( lpTask, lpView, 8, 107, 0, 0, 0 );

      // fnOperationReturn( 0, lpTask, 0 );
         return( 0 );  // return error
      }
      else
         return( 1 );
   }

   return( 0 );
}

//./ ADD NAME=fnValidViewObject
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidViewObject
//
//  ASSUMES:    View has a valid view object associated with it
//
//  RETURNS:    1 - View valid
//              0 - View invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnValidViewObject( LPTASK lpTask, zVIEW lpView )
{
   if ( fnValidView( lpTask, lpView ) )
   {
      if ( lpView->hViewOD == 0 )
      {
         //  "KZOEE102 - Invalid View, view is hidden"
         fnIssueCoreError( lpTask, lpView, 8, 102, 0, 0, 0 );

      // fnOperationReturn( 0, lpTask, 0 );
         return( 0 );  // return error
      }
      else
         return( 1 );
   }

   return( 0 );
}

//./ ADD NAME=fnValidViewCsr
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidViewCsr
//
//  ASSUMES:    View has a set of cursors
//
//  RETURNS:    1 - View valid
//              0 - View invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnValidViewCsr( LPTASK lpTask, zVIEW  lpView )
{
   if ( fnValidViewObject( lpTask, lpView ) )
   {
      if ( lpView->hViewCsr == 0 )
      {
         LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

         //  "KZOEE109 - Invalid View, view contains no instance"
         fnIssueCoreError( lpTask, lpView, 8, 109, 0, lpViewOD->szName, 0 );

      // fnOperationReturn( 0, lpTask, 0 );
         return( 0 );  // return error
      }
      else
         return( 1 );
   }

   return( 0 );
}

//./ ADD NAME=fnValidViewEntity
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidViewEntity
//
//  ASSUMES:    Entity name is valid
//
//  RETURNS:    lpViewEntity and lpViewEntityCsr
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
LPVIEWENTITY OPERATION
fnValidViewEntity( LPVIEWENTITYCSR *pvReturnViewEntityCsr,
                   zVIEW  lpView, zCPCHAR cpcEntityName, zLONG lFlags )
{
   LPVIEWCSR         lpViewCsr;
   LPVIEWOD          lpViewOD;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   zLONG             lInputTok;

   *pvReturnViewEntityCsr = 0;  // initialize
   if ( lpView == 0 )
   {
      TraceLine( "fnValidViewEntity - Invalid View (0x%08x) for Entity: %s",
                 lpView, cpcEntityName );
      return( 0 );
   }

   // Get the view csr for the view.
   lpViewCsr = zGETPTR( lpView->hViewCsr );

   // Get the view object definition for the view.
   lpViewOD = zGETPTR( lpView->hViewOD );

   // Validate that the entity exists.
   lpViewEntity    = zGETPTR( lpViewOD->hFirstOD_Entity );
   lpViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );

   // If the root cursor is not the first cursor, skip ViewEntities
   // until we get to the entity matching the root cursor.
   if ( lpViewEntityCsr != zGETPTR( lpViewCsr->hFirstOD_EntityCsr ) )
   {
      LPVIEWENTITYCSR lpSearchViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );
      while ( lpSearchViewEntityCsr != lpViewEntityCsr )
      {
         lpViewEntity = zGETPTR( lpViewEntity->hNextHier );
         lpSearchViewEntityCsr++;
      }
   }

   if ( *cpcEntityName == '\xff' )
   {
      lInputTok = *((zPLONG) (cpcEntityName + 1));
      while ( lpViewEntityCsr && lpViewEntity->lEREntTok != lInputTok )
      {
         lpViewEntity    = zGETPTR( lpViewEntity->hNextHier );
         lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
      }
   }
   else
   {
      while ( lpViewEntityCsr &&
              zstrcmp( lpViewEntity->szName, cpcEntityName ) != 0 )
      {
         lpViewEntity    = zGETPTR( lpViewEntity->hNextHier );
         lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
      }
   }

   // If entity found return pointer.
   if ( lpViewEntityCsr )
   {
      *pvReturnViewEntityCsr = lpViewEntityCsr;
      return( lpViewEntity );
   }

   // Note, the name may not be invalid, it may simply be out of view because
   // of a call to SetViewToSubobject
   //  "KZOEE103 - Invalid Entity name for View"
   TraceLine( "fnValidViewEntity - Invalid Entity name: %s   for View (0x%08x)   OD: %s",
              cpcEntityName, lpView, lpViewOD->szName );
// DisplayObjectInstance( lpView, 0, 0 );
   if ( (lFlags & 1) == 0 )
   {
      fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 103, 0,
                        lpViewOD->szName,
                        TrueName( cpcEntityName, zSHOW_ZKEY ) );
   }

// fnOperationReturn( 0, 0, 0 );
   return( 0 );  // return error
}

//./ ADD NAME=fnValidViewAttrib
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidViewAttrib
//
//  ASSUMES:    Attribute name is valid
//
//  RETURNS:    lpViewAttrib
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
LPVIEWATTRIB
fnValidViewAttrib( zVIEW         lpView,
                   LPVIEWENTITY  lpViewEntity,
                   zCPCHAR       cpcAttribName,
                   zLONG         lControl )
{
   LPVIEWATTRIB lpViewAttrib;
   zLONG        lInputTok;

   // Get first attribute for entity.
   lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
   if ( *cpcAttribName == '\xff' )
   {
      lInputTok = *((zPLONG) (cpcAttribName + 1) );
      while ( lpViewAttrib && (lpViewAttrib->bHidden ||
                               lpViewAttrib->lERAttTok != lInputTok) )
      {
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );
      }
   }
   else
   {
      // Look for the attribute.
      while ( lpViewAttrib )
      {
         if ( zstrcmp( lpViewAttrib->szName, cpcAttribName ) == 0 )
         {
            // We found it.  If the attribute is hidden check to see if we
            // allow hidden attributes.
            if ( lpViewAttrib->bHidden && (lControl & zALLOW_HIDDEN) == 0 )
               lpViewAttrib = 0; // No hidden attributes so set to NULL.

            break;
         }

         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );
      }
   }

   // If attribute not found return error.
   if ( lpViewAttrib )
      return( lpViewAttrib );
   else
   {
      zCHAR    szLOD_Entity_Attribute[ 256 ];
      LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

      //  "KZOEE104 - Invalid Attribute name for LOD Entity"
      sprintf_s( szLOD_Entity_Attribute, sizeof( szLOD_Entity_Attribute ), "[View: 0x%08x] %s.%s.%s",
                 (zULONG) lpView, lpViewOD->szName, lpViewEntity->szName,
                 TrueName( cpcAttribName, zSHOW_ZKEY ) );
      TraceLineS( "fnValidViewAttrib error: ", szLOD_Entity_Attribute );
      fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 104,
                        0, szLOD_Entity_Attribute, 0 );

   // fnOperationReturn( 0, 0, 0 );
      return( 0 );  // return error
   }
}

//./ ADD NAME=TrueName
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   TrueName
//
//  PURPOSE:    Determines if szName is a pointer to a ZKey.  If it is
//              not, szName is returned unchanged.
//
//              If szName is a ZKey pointer then NameForZKey is invoked.
//              The returned Name from NameForZKey is returned in szName.
//              If zSHOW_ZKEY is specified in nControl then the ZKey (in
//              string format) will be appended to the szName buffer.
//
//              If no name is returned then the ZKey is converted to a
//              string and that is returned in szName.
//
//  RETURNS:    See purpose.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zPCHAR OPERATION
TrueName( zCPCHAR cpcName, zSHORT nControl )
{
   zLONG    lZKey;
   static zCHAR szWorkString[ 50 ]; // ??? should be removed sometime (dks)

   // If the input is not a pointer to a ZKey, just return cpcName as is.
   if ( cpcName == 0 || (cpcName && *cpcName != '\xff') )
      return( (zPCHAR) cpcName );

   // Extract the ZKey from the input.
   lZKey = *((zPLONG) (cpcName + 1) );
   if ( NameForZKey( szWorkString, lZKey ) && nControl & zSHOW_ZKEY )
   {
      strcat_s( szWorkString, sizeof( szWorkString ), " (" );
      zltoa( lZKey, szWorkString + zstrlen( szWorkString ), sizeof( szWorkString ) - zstrlen( szWorkString ) );
      strcat_s( szWorkString, sizeof( szWorkString ), ")" );
   }

   return( szWorkString );
}

//./ ADD NAME=NameForZKey
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   NameForZKey
//
//  PURPOSE:    Determines the name for the input ZKey.  If the input
//              ZKey cannot be found then the ZKey is converted to a
//              string, and that is returned.
//
//  RETURNS:    1 - Name for ZKey returned.
//              0 - ZKey converted to string and returned.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
NameForZKey( zPCHAR pchReturnString, zLONG lZKey )
{
   zltoa( lZKey, pchReturnString, 18 );
   return( 0 );
}

//./ ADD NAME=fnValidNumericString
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidNumericString
//
//  PURPOSE:    Validates that the input string contains a simple numeric
//              value and if okay, returns the converted value.
//
//  RETURNS:    1 - valid numeric value, value returned.
//              0 - input string contains non-numeric characters or
//                  is a compound numeric expression.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnValidNumericString( zPLONG  lplReturnValue, zCPCHAR cpcString )
{
   zPCHAR   cptr;
   zBOOL    bSpaceEncountered = 0;
   zBOOL    bFirstDigitEncountered = 0;

   cptr = (zPCHAR) cpcString;

   while ( *cptr )
   {
      switch ( *cptr )
      {
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            if ( bSpaceEncountered )
               return( 0 );

            bFirstDigitEncountered = TRUE;
            break;

         case '-':
         case '+':
            if ( bFirstDigitEncountered )
               return( 0 );
            else
               bFirstDigitEncountered = TRUE;

            break;

         case ' ':
            if ( bFirstDigitEncountered )
               bSpaceEncountered = TRUE;

            break;

         default:
            return( 0 );
      }

      cptr++;
   }

   if ( lplReturnValue )
      *lplReturnValue = zatol( cpcString );

   return( 1 );
}

//./ ADD NAME=fnValidDecimalString
// Source Module=kzoevlaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnValidDecimalString
//
//  PURPOSE:    Validates that the input string contains a simple decimal
//              value and if okay, returns the converted value.
//
//  RETURNS:    1 - valid numeric value, value returned.
//              0 - input string contains non-numeric characters or
//                  is a compound numeric expression.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnValidDecimalString( zPDECIMAL lpdReturnValue,
                      zCPCHAR   cpcString )
{
   zPCHAR   cptr;
   zBOOL    bSpaceEncountered = 0;
   zBOOL    bFirstDigitEncountered = 0;
   zBOOL    bDecimalEncountered = 0;
   zCHAR    cDec; // the decimal point character, internationally speaking...
   LPINTERNATIONAL lpInternational = zGETPTR( AnchorBlock->hInternational );

   cDec = lpInternational->szDecimal[ 0 ];
   cptr = (zPCHAR) cpcString;

   while ( *cptr )
   {
      switch ( *cptr )
      {
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            if ( bSpaceEncountered )
               return( 0 );

            bFirstDigitEncountered = TRUE;
            break;

         case '-':
         case '+':
            if ( bFirstDigitEncountered )
               return( 0 );
            else
               bFirstDigitEncountered = TRUE;

            break;

         case ' ':
            if ( bFirstDigitEncountered )
               bSpaceEncountered = TRUE;
            break;

         default:
            if ( *cptr == cDec )
            {
               if ( bSpaceEncountered )
                  return( 0 );

               if ( bDecimalEncountered )
                  return( 0 );

               bDecimalEncountered = TRUE;
               bFirstDigitEncountered = TRUE;
               break;
            }

            return( 0 );
      }

      cptr++;
   }

   if ( lpdReturnValue )
      SysConvertStringToDecimal( (zPCHAR) cpcString, lpdReturnValue );

   return( 1 );
}

/*
   Should only be used internally by QuinSoft.
   Checks to make sure that the view is a well-formed OI.  Currently checks
   for:
      o  View, ViewOD, ViewOI, and ViewCsr have valid table IDs.
      o  OI has < 1000000 entities.
      o  Each EI in the view has a valid link chain.
*/
zSHORT OPERATION
SfCheckOI_Integrity( zVIEW lpView, zLONG lControl )
{
   LPTASK            lpCurrentTask;
   LPVIEWOD          lpViewOD;
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zLONG             nEntityCount;
   zSHORT            nRC = -1;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfCheckOI_Integrity, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpViewOD = zGETPTR( lpView->hViewOD );
   if ( lpViewOD == 0 || lpViewOD->nTableID != iViewOD )
   {
      SysMessageBox( lpView, "OI Integrity",
                     "View does not have a valid OD", 1 );
      goto EndOfFunction;
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   if ( lpViewCsr == 0 || lpViewCsr->nTableID != iViewCsr )
   {
      SysMessageBox( lpView, "OI Integrity",
                     "View does not have a valid ViewCsr", 1 );
      goto EndOfFunction;
   }

   lpViewOI = zGETPTR( lpViewCsr->hViewOI );
   if ( lpViewOI == 0 || lpViewOI->nTableID != iViewOI )
   {
      SysMessageBox( lpView, "OI Integrity",
                     "View does not have a valid View OI", 1 );
      goto EndOfFunction;
   }

   for ( lpEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance ),
               nEntityCount = 0;
         lpEntityInstance;
         lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier ),
               nEntityCount++ )
   {
      LPENTITYINSTANCE lpLinked;
      LPVIEWENTITY     lpViewEntity;
      zLONG            nLinkedCount;

      if ( nEntityCount > 1000000 )
      {
         SysMessageBox( lpView, "OI Integrity", "Too many entities", 1 );
         goto EndOfFunction;
      }

      if ( lpEntityInstance->nTableID != iEntityInstance )
      {
         TraceLineI( "OI Integrity--EI's hier # = ", nEntityCount );
         SysMessageBox( lpView, "OI Integrity",
                        "View has an invalid entity", 1 );
         goto EndOfFunction;
      }

      lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

      if ( lpEntityInstance->hNextLinked )
      {
         for ( lpLinked = zGETPTR( lpEntityInstance->hNextLinked ), nLinkedCount = 0;
               lpLinked != lpEntityInstance;
               lpLinked = zGETPTR( lpLinked->hNextLinked ), nLinkedCount++ )
         {
            if ( nLinkedCount > 1000 )
            {
               TraceLineI( "OI Integrity--EI's hier # = ", nEntityCount );
               SysMessageBox( lpView, "OI Integrity",
                              "Too many linked EIs", 1 );
               goto EndOfFunction;
            }

            if ( lpLinked->nTableID != iEntityInstance )
            {
               TraceLineI( "OI Integrity--EI's hier # = ", nEntityCount );
               SysMessageBox( lpView, "OI Integrity",
                              "Invalid linked EI", 1 );
               goto EndOfFunction;
            }
         }
      }

   } // for each lpEntityInstance...

   nRC = 0;

EndOfFunction:
   fnOperationReturn( iSfCheckOI_Integrity, lpCurrentTask );
   return( nRC );
}
