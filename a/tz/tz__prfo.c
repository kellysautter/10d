/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tz__prfo.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
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
// LAST MESSAGE ID: PR001xx
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


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZ__PRFO_GetViewForProfile
//
// PURPOSE:   Get access to PRF View.  Activate if not loaded
//            and get at application level if loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZ__PRFO_GetViewToProfile( zPVIEW pvTZ__PRFO,
                            zPCHAR szToolID,
                            zVIEW  vSubtask,
                            zSHORT nCurrentOrReload )
{
   zVIEW  vT;
   zVIEW  vTmpSubtask;
   zVIEW  vLPLR;
   zCHAR  szLPLR[ 33 ];
   zCHAR  szPRF_FileName[ 257 ];
   zCHAR  szAppViewName[ 18 ] = "prfToolsForZeidon";
   zSHORT nRScope;
   zSHORT nRC;

   // See if PRF View is at the Application level.
   if ( SfCreateSubtask( &vTmpSubtask, vSubtask, "Zeidon_Tools" ) == zCALL_ERROR )
   {
      return( zCALL_ERROR );
   }

   if ( vTmpSubtask == 0 )
      return( zCALL_ERROR );

   nRScope = GetViewByName( &vT, szAppViewName, vTmpSubtask, zLEVEL_APPLICATION );
   if ( nRScope == zCALL_ERROR )
   {
      SfDropSubtask( vTmpSubtask, 0 );
      return( nRScope );
   }

   if ( nRScope == zLEVEL_APPLICATION &&
        nCurrentOrReload == zFORCE_RELOAD )
   {
      nRC = DropObjectInstance( vT );
      nRScope = -1;
   }

   if ( nRScope != zLEVEL_APPLICATION )
   {
      // Load it from a file or create a new one if file is missing.
      if ( SysGetEnvVar( szPRF_FileName, "ZEIDON", zsizeof( szPRF_FileName ) - 1 ) == 0 )
      {
         if ( szPRF_FileName[ zstrlen( szPRF_FileName ) ] != '\\' )
            strcat_s( szPRF_FileName, zsizeof( szPRF_FileName ), "\\" );

         strcat_s( szPRF_FileName, zsizeof(szPRF_FileName), "TZTOOLS.PRF" );
      }
      else
         return( -2 );

      // dks 2007.06.03 ... added zACTIVATE_SYSTEM in attempt to get tools to live in harmony with Tomcat.
      if ( ActivateOI_FromFile( &vT, "TZ__PRFO", vTmpSubtask, szPRF_FileName,
                                zSINGLE | zACTIVATE_SYSTEM | zLEVEL_SYSTEM /*zLEVEL_APPLICATION*/ | zNOI_OKAY | zIGNORE_ERRORS ) < 0 )
      {
         nRC = ActivateEmptyObjectInstance( &vT, "TZ__PRFO", vTmpSubtask,
                                            zSINGLE | zACTIVATE_SYSTEM | zLEVEL_SYSTEM /*zLEVEL_APPLICATION*/ );
         if ( nRC >= 0 )
         {
            CreateEntity( vT, "TZ", zPOS_AFTER );
            CreateEntity( vT, "EMD", zPOS_AFTER );
            CreateEntity( vT, "Model", zPOS_AFTER );
            CreateEntity( vT, "Submodel", zPOS_AFTER );
            CreateEntity( vT, "IDVIEW", zPOS_AFTER );
            CreateEntity( vT, "ATTVIEW", zPOS_AFTER );
            CreateEntity( vT, "RLLVIEW", zPOS_AFTER );
            CreateEntity( vT, "SA", zPOS_AFTER );
            CreateEntity( vT, "ZO", zPOS_AFTER );
            CreateEntity( vT, "WD", zPOS_AFTER );
            CreateEntity( vT, "TE", zPOS_AFTER );
            CreateEntity( vT, "DM", zPOS_AFTER );
            CreateEntity( vT, "OP", zPOS_AFTER );
            CreateEntity( vT, "MS", zPOS_AFTER );
            CreateEntity( vT, "VML", zPOS_AFTER );
            CreateEntity( vT, "OBR", zPOS_AFTER );
            CreateEntity( vT, "ED", zPOS_AFTER );
            SetNameForView( vT, szAppViewName, 0, zLEVEL_APPLICATION );
            nRC = CommitOI_ToFile( vT, szPRF_FileName, zASCII );
         }
         else
         {
            SfDropSubtask( vTmpSubtask, 0 );
            return( -1 );
         }
      }
      else
      {
         nRC = SetNameForView( vT, szAppViewName, 0, zLEVEL_APPLICATION );
         if ( CheckExistenceOfEntity( vT, "TZ" ) < zCURSOR_SET )
            CreateEntity( vT, "TZ", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "EMD" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "EMD", zPOS_AFTER );
            CreateEntity( vT, "Model", zPOS_AFTER );
            CreateEntity( vT, "Submodel", zPOS_AFTER );
            CreateEntity( vT, "IDVIEW", zPOS_AFTER );
            CreateEntity( vT, "ATTVIEW", zPOS_AFTER );
            CreateEntity( vT, "RLLVIEW", zPOS_AFTER );
            CreateEntity( vT, "ENTVIEW", zPOS_AFTER );
            CreateEntity( vT, "SA", zPOS_AFTER );
         }

         if ( CheckExistenceOfEntity( vT, "Model" ) < zCURSOR_SET )
            CreateEntity( vT, "Model", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "Submodel" ) < zCURSOR_SET )
            CreateEntity( vT, "Submodel", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "IDVIEW" ) < zCURSOR_SET )
            CreateEntity( vT, "IDVIEW", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "ATTVIEW" ) < zCURSOR_SET )
            CreateEntity( vT, "ATTVIEW", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "ENTVIEW" ) < zCURSOR_SET )
            CreateEntity( vT, "ENTVIEW", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "RLLVIEW" ) < zCURSOR_SET )
            CreateEntity( vT, "RLLVIEW", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "SA" ) < zCURSOR_SET )
            CreateEntity( vT, "SA", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "ZO" ) < zCURSOR_SET )
            CreateEntity( vT, "ZO", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "WD" ) < zCURSOR_SET )
            CreateEntity( vT, "WD", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "TE" ) < zCURSOR_SET )
            CreateEntity( vT, "TE", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "DM" ) < zCURSOR_SET )
            CreateEntity( vT, "DM", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "OP" ) < zCURSOR_SET )
            CreateEntity( vT, "OP", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "MS" ) < zCURSOR_SET )
            CreateEntity( vT, "MS", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "VML" ) < zCURSOR_SET )
            CreateEntity( vT, "VML", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "OBR" ) < zCURSOR_SET )
            CreateEntity( vT, "OBR", zPOS_AFTER );

         if ( CheckExistenceOfEntity( vT, "ED" ) < zCURSOR_SET )
            CreateEntity( vT, "ED", zPOS_AFTER );
      }

      GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( szToolID[ 0 ] && vLPLR )
      {
         GetStringFromAttribute( szLPLR, zsizeof( szLPLR ), vLPLR, "LPLR", "Name" );
         nRC = SetCursorFirstEntityByString( vT, "LPLR_Group",
                                             "Name", szLPLR, "" );
         if ( nRC < zCURSOR_SET )
            CreateEntity( vT, "LPLR_Group", zPOS_LAST );

         SetMatchingAttributesByName( vT, "LPLR_Group",
                                      vLPLR, "LPLR", zSET_ALL );
         nRC = SetNameForView( vT, szAppViewName, 0, zLEVEL_APPLICATION );
         if ( CheckExistenceOfEntity( vT, "EMD_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "EMD_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "EMD_LPLR",
//                                       vT, "EMD", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "SA_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "SA_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "SA_LPLR",
//                                       vT, "SA", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "ZO_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "ZO_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "ZO_LPLR",
//                                       vT, "ZO", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "WD_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "WD_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "WD_LPLR",
//                                       vT, "WD", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "TE_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "TE_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "TE_LPLR",
//                                       vT, "TE", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "DM_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "DM_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "DM_LPLR",
//                                       vT, "DM", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "OP_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "OP_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "OP_LPLR",
//                                       vT, "OP", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "MS_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "MS_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "MS_LPLR",
//                                       vT, "MS", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "VML_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "VML_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "VML_LPLR",
//                                       vT, "VML", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "OBR_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "OBR_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "OBR_LPLR",
//                                       vT, "OBR", zSET_NULL );
         }

         if ( CheckExistenceOfEntity( vT, "ED_LPLR" ) < zCURSOR_SET )
         {
            CreateEntity( vT, "ED_LPLR", zPOS_AFTER );
//          SetMatchingAttributesByName( vT, "ED_LPLR",
//                                       vT, "ED", zSET_NULL );
         }
      }
   }

   nRC = CreateViewFromViewForTask( pvTZ__PRFO, vT, vT );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZ__PRFO_CommitPRF
//
// PURPOSE:   Commit the current PRF to the TZTOOLS.PRF file.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZ__PRFO_CommitProfile( zVIEW vTZ__PRFO )
{
   zCHAR  szPRF_FileName[ 256 ];
   zSHORT nRC;

// Set up the PRF File Name

   if ( SysGetEnvVar( szPRF_FileName, "ZEIDON", 255 ) == 0 )
   {
      if ( szPRF_FileName[ zstrlen( szPRF_FileName ) ] != '\\' )
         strcat_s( szPRF_FileName, zsizeof( szPRF_FileName ), "\\" );

      strcat_s( szPRF_FileName, zsizeof( szPRF_FileName ), "TZTOOLS.PRF" );
   }
   else
      return( -2 );

   nRC = CommitOI_ToFile( vTZ__PRFO, szPRF_FileName, zASCII );
   return( nRC );
}

///////////////////////////// end of file  //////////////////////////////////
