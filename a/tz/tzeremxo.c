/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzeremxo.c - Zeidon application operations
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
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
/*
CHANGE LOG

2000.10.24  SR  Z2000  Size of path variables
   Modified size of .szFileSpec in function oTZEREMXO_BuildExecutableModel
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
*/


#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


zOPER_EXPORT zSHORT OPERATION
oTZEREMXO_BuildExecutableModel( zVIEW  vSubtask,
                                zPVIEW vReturnExecModel,
                                zVIEW  vModel,
                                zVIEW  vTE );

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  oTZEREMXO_BuildExecutableModel
//
// PURPOSE:    This Entry takes the model view and an optional
//             TE View associated with the Model and builds
//             and executable form of the model for XOD load
//             validation in CORE.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMXO_BuildExecutableModel( zVIEW  vSubtask, zPVIEW vpReturnExecModel,
                                zVIEW  vModel, zVIEW  vDTE )
{
   zVIEW vExecModel;
   zVIEW vTaskLPLR;
   zVIEW vDTE2;
   zVIEW vCM_List;
   zLONG lIntegerTok;
   zCHAR szFileSpec[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nNbrEnts;
   zSHORT nNbrAtts;
   zSHORT nNbrRels;

   // Zero the return pointer.
   if ( vpReturnExecModel )
      *vpReturnExecModel = 0;

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vModel, zLEVEL_TASK ) < 1 )
      return( 0 );

   GetStringFromAttribute( szFileSpec, zsizeof( szFileSpec ), vTaskLPLR, "LPLR", "ExecDir" );
   strcat_s( szFileSpec, zsizeof( szFileSpec ), "\\ZEIDON.XMD" );

   // If the Model has no entities, return an Error.
   if ( SetCursorFirstEntity( vModel, "ER_Entity", 0 ) < zCURSOR_SET )
      return( zCALL_ERROR );

   // Activate an Empty instance.
   if ( ActivateEmptyObjectInstance( &vExecModel, "TZEREMXO",
                                     vModel, zSINGLE ) < 0 )
   {
      return( zCALL_ERROR );
   }

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   SetCursorFirstEntityByString( vCM_List, "W_MetaDef",
                                 "Name", "Integer", 0 );
   GetIntegerFromAttribute( &lIntegerTok, vCM_List, "W_MetaDef", "CPLR_ZKey" );
   DropView( vCM_List );

   // The Model is valid and we have created an empty instance of the
   // executable model object, now instantiate the object
   if ( vpReturnExecModel )
      *vpReturnExecModel = vExecModel;

   CreateEntity( vExecModel, "MODEL", zPOS_AFTER );
   nNbrEnts = 0;

   // Now instantiate the object.
   CreateViewFromViewForTask( &vDTE2, vDTE, 0 );
   do
   {
      CreateEntity( vExecModel, "ENTITY", zPOS_AFTER );
      nNbrAtts = 0;
      if ( SetCursorFirstEntity( vModel, "ER_Attribute", 0 ) >= zCURSOR_SET )
      {
         do
         {
            CreateEntity( vExecModel, "ATTRIB", zPOS_AFTER );
            SetAttributeFromAttribute( vExecModel, "ATTRIB", "ZKEY",
                                       vModel, "ER_Attribute", "ZKey" );
            SetAttributeFromAttribute( vExecModel, "ATTRIB", "NAME",
                                       vModel, "ER_Attribute", "Name" );
            SetAttributeFromAttribute( vExecModel, "ATTRIB", "LTH",
                                       vModel, "ER_Attribute", "Lth" );
            SetAttributeFromAttribute( vExecModel, "ATTRIB", "APDM_TOK",
                                       vModel, "Domain", "ZKey" );
            nNbrAtts++;
         }  while ( SetCursorNextEntity( vModel, "ER_Attribute", 0 ) >= zCURSOR_SET );

         SetAttributeFromInteger( vExecModel, "ENTITY", "NumAtts", nNbrAtts );
      }

      // Add any Foreign Key Attrs based on TE.
      if ( SetCursorFirstEntityByEntityCsr( vDTE, "ER_Entity",
                                            vModel, "ER_Entity",
                                            "TE_DB_Environ" ) >= zCURSOR_SET )
      {
         if ( SetCursorFirstEntity( vDTE, "TE_FieldDataRel", 0 ) >= zCURSOR_SET )
         {
            do
            {
               if ( CheckExistenceOfEntity( vDTE, "TE_FieldDataRel_PK" )
                                                             >= zCURSOR_SET )
               {
                  CreateEntity( vExecModel, "ATTRIB", zPOS_AFTER );
                  SetAttributeFromAttribute( vExecModel, "ATTRIB", "ZKEY",
                                             vDTE, "TE_FieldDataRel", "ZKey" );
                  SetAttributeFromAttribute( vExecModel, "ATTRIB", "NAME",
                                             vDTE, "TE_FieldDataRel", "Name" );
                  SetAttributeFromAttribute( vExecModel, "ATTRIB", "LTH",
                                             vDTE, "TE_FieldDataRel", "Length" );
                  SetCursorFirstEntityByEntityCsr( vDTE2, "TE_FieldDataRel",
                                                   vDTE, "TE_FieldDataRel_PK",
                                                   "TE_DB_Environ" );

                  if ( CheckExistenceOfEntity( vDTE2, "Domain" ) < zCURSOR_SET )
                  {
                     SetAttributeFromInteger( vExecModel, "ATTRIB", "APDM_TOK",
                                              lIntegerTok );
                  }
                  else
                  {
                     SetAttributeFromAttribute( vExecModel, "ATTRIB", "APDM_TOK",
                                                vDTE2, "Domain", "ZKey" );
                  }

                  nNbrAtts++;
               }

            }  while ( SetCursorNextEntity( vDTE, "TE_FieldDataRel",
                                            0 ) >= zCURSOR_SET );
            SetAttributeFromInteger( vExecModel, "ENTITY", "NumAtts", nNbrAtts );
         }
      }

      SetAttributeFromAttribute( vExecModel, "ENTITY", "ZKEY",
                                 vModel, "ER_Entity", "ZKey" );
      nNbrEnts++;
   }  while ( SetCursorNextEntity( vModel, "ER_Entity", 0 ) >= zCURSOR_SET );

   SetAttributeFromInteger( vExecModel, "MODEL", "NumEnts", nNbrEnts );
   DropView( vDTE2 );

   // Now collect the relationship type information.
   nNbrRels = 0;
   if ( SetCursorFirstEntity( vModel, "ER_RelType", 0 ) >= zCURSOR_SET )
   do
   {
      CreateEntity( vExecModel, "REL", zPOS_AFTER );
      SetAttributeFromAttribute( vExecModel, "REL", "ZKEY",
                                 vModel, "ER_RelType", "ZKey" );
      nNbrRels++;
   } while ( SetCursorNextEntity( vModel, "ER_RelType", 0 ) >= zCURSOR_SET );

   SetAttributeFromInteger( vExecModel, "MODEL", "NumRels", nNbrRels );
   CommitOI_ToFile( vExecModel, szFileSpec, zASCII );
   if ( vpReturnExecModel )
      ResetView( vExecModel );
   else
      DropObjectInstance( vExecModel );

   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
