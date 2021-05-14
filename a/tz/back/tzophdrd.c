/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzophdrd.c -
// DESCRIPTION:
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        ??
// DATE:          ??
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   ??
// REVISION:      ??
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
*/

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#include "KZOENGAA.H"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>

#include "tz__oprs.h"
#include "tzlodopr.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"


// LAST MESSAGE ID: OP00113


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwfnPromptForSave
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnPromptForSave( zVIEW vSubtask )
{
   zVIEW vHEADERMETA;
   zSHORT nRC;

   nRC = zRESPONSE_NO;
   if ( GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( ObjectInstanceUpdatedFromFile( vHEADERMETA ) )
      {
         nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_GO,
                           zREFER_HDR_META,
                           vHEADERMETA, "HeaderFile", "Name", 0 );
         if ( nRC == zRESPONSE_YES )
            CommitMetaOI( vSubtask, vHEADERMETA, zSOURCE_HDR_META );
      }
   }
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_SortName( zVIEW vSubtask )
{
   zVIEW    vHEADERCM;

   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
   zgSortEntityWithinParent( zDESCENDING, vHEADERCM,
                              "W_MetaDef", "Name", "" );
   SetCursorFirstEntity( vHEADERCM, "W_MetaDef", "" );
   return( 0 );
}
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_SortDate( zVIEW vSubtask )
{
   zVIEW    vHEADERCM;

   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
   zgSortEntityWithinParent( zDESCENDING, vHEADERCM,
                              "W_MetaDef", "LastUpdateDate", "" );
   SetCursorFirstEntity( vHEADERCM, "W_MetaDef", "" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_PostBHeaderFileList( zVIEW vSubtask )
{
   zVIEW    vHEADERCM;
   zVIEW    vLPL;
   zVIEW    vProfileXFER;
   zSHORT   nRC;
   zCHAR    szLPL_Name[ 34 ];

   nRC = InitializeDefaultLPL( vSubtask );
   // get a view to the CM List of Meta Type for the Selection
   if ( GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK ) < 1 )
   {
      if ( nRC != zLEVEL_TASK )
      {
         RetrieveViewForMetaList( vSubtask, &vHEADERCM, zREFER_HDR_META );
         SetNameForView( vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
         zwTZOPHDRD_SortName( vSubtask );
      }
   }

   nRC = GetViewByName( &vLPL, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      GetStringFromAttribute( szLPL_Name, zsizeof( szLPL_Name ), vLPL, "LPLR", "Name" );
      SetWindowCaptionTitle( vSubtask, szLPL_Name,
                             "Header File Definition - [Untitled]" );
   }

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "OP", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZOPHDRD_MainRtnFromSubWnd
//
// PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated
//             the current EMD.
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_MainRtnFromSubWnd( zVIEW vSubtask )
{
   zVIEW   vCMLIST;
   zVIEW   vHEADERMETA;
   zVIEW   vLPL;
   zSHORT  nRC;
   zCHAR   szLPL_Name[ 60 ];
   zCHAR   szTitle[ 34 ];

   RetrieveViewForMetaList( vSubtask, &vCMLIST, zREFER_HDR_META );
   SetNameForView( vCMLIST, "HEADERCM", vSubtask, zLEVEL_TASK );

   nRC = GetViewByName( &vLPL, "TaskLPLR", vSubtask, zLEVEL_TASK );
   strcpy_s( szTitle, zsizeof( szTitle ), "[Untitled]" );
   if ( nRC > 0 )
   {
      GetStringFromAttribute( szLPL_Name, zsizeof( szLPL_Name ), vLPL, "LPLR", "Name" );
      strcat_s( szLPL_Name, zsizeof( szLPL_Name ), " - Header File Definition" );
      if ( GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK ) > 0 )
         GetStringFromAttribute( szTitle, zsizeof( szTitle ), vHEADERMETA, "HeaderFile", "Name" );
      SetWindowCaptionTitle( vSubtask, szLPL_Name, szTitle );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//
// OPERATION:  zwTZOPHDRD_OpenHeaderFile
//
// PURPOSE:    This Entry opens the header file
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_OpenHeaderFile( zVIEW vSubtask )
{
zVIEW vHEADERLIST;
zVIEW vHEADERMETA;

   GetViewByName( &vHEADERLIST, "HEADERCM", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vHEADERLIST, "W_MetaDef" ) >= zCURSOR_SET )
   {
      if ( GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK ) > 0 )
         DropMetaOI( vSubtask, vHEADERMETA );

      ActivateMetaOI( vSubtask, &vHEADERMETA, vHEADERLIST, zSOURCE_HDR_META, zSINGLE );
      SetNameForView( vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK );
   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//
// OPERATION:  zwTZOPHDRD_DeleteHeaderFile
//
// PURPOSE:    This Entry opens the header file
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_DeleteHeaderFile( zVIEW vSubtask )
{
zVIEW vHEADERLIST;
zCHAR szMsg[ 100 ];

   GetViewByName( &vHEADERLIST, "HEADERCM", vSubtask, zLEVEL_TASK );
   if ( GetSelectStateOfEntityForSet( vHEADERLIST, "W_MetaDef", 1 ) )
   {
      GetStringFromAttribute( szMsg + 50, zsizeof( szMsg ) - 50, vHEADERLIST,
                              "W_MetaDef", "Name" );
      strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete header " );
      strcat_s( szMsg, zsizeof( szMsg ), &szMsg[ 50 ] );
      strcat_s( szMsg, zsizeof( szMsg ), "?" );
      if ( MessagePrompt( vSubtask, "OP00101",
                          "Domain Management",
                          szMsg,
                          zBEEP, zBUTTONS_YESNO,
                          zRESPONSE_NO,      0 )  == zRESPONSE_NO )
      {
         return( 0 );
      }
      DeleteMetaOI( vSubtask, vHEADERLIST, zSOURCE_HDR_META );
   }
   else
   {
      MessageSend( vSubtask, "OP00102", "Operations",
                   "Please select a header for deletion.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_Popup1( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "Popup1", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
fnActivateHeader( zVIEW vSubtask, zPVIEW pvHEADERMETA, zSHORT lControl )
{
   zVIEW    vHEADERCM;
   zSHORT   nRC;

   // get a view to the CM List of Meta Type for the Selection
   nRC = GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
   if ( nRC != zLEVEL_TASK )
   {
      RetrieveViewForMetaList( vSubtask, &vHEADERCM, zREFER_HDR_META );
      SetNameForView( vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
   }

   nRC = CheckExistenceOfEntity( vHEADERCM, "W_MetaDef" );
   if ( nRC == 0 )
   {
      nRC = ActivateMetaOI( vSubtask, pvHEADERMETA, vHEADERCM, lControl,
                            zSINGLE | zLEVEL_APPLICATION );
      if ( nRC != 1 )
         return( -1 );

      SetNameForView( *pvHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK );
      return( 1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//
// OPERATION:  zwTZOPHDRD_SaveHeaderFile
//
// PURPOSE:    This Entry saves the header file
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_SaveHeaderFile( zVIEW vSubtask )
{
zVIEW vHEADERMETA;

   if ( GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK ) > 0 &&
        MiGetUpdateForView( vHEADERMETA ) )
   {
      CommitMetaOI( vSubtask, vHEADERMETA, zSOURCE_HDR_META );
   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//
// OPERATION:  zwTZOPHDRD_SaveHeaderFile
//
// PURPOSE:    This Entry saves the header file
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_PromptForSave( zVIEW vSubtask )
{
   if ( zwfnPromptForSave( vSubtask ) == zRESPONSE_CANCEL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZOPHDRD_SaveHeaderFile
//
// PURPOSE:    This Entry saves the header file
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_ExitDialog( zVIEW vSubtask )
{
   if ( zwfnPromptForSave( vSubtask ) == zRESPONSE_CANCEL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   TerminateLPLR( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
fnCommitMetaOI( zVIEW vSubtask, zVIEW vWindow )
{
   zVIEW  vHEADERMETA;
   zSHORT nRC;
   zCHAR  szName[ 32 ];
   zCHAR  szMessage[ 128 ];

   nRC =
      GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK );

   if ( nRC == zLEVEL_TASK )
   {
      nRC =
         CommitMetaOI( vSubtask, vHEADERMETA, zSOURCE_HDR_META );

      if ( nRC != 1 )
      {
         nRC =
            GetStringFromAttribute( szName, zsizeof( szName ), vHEADERMETA, "HeaderFile", "Name" );

         strcpy_s( szMessage, zsizeof( szMessage ), "Commit failed for Header File " );
         strcat_s( szMessage, zsizeof( szMessage ), szName );
         strcat_s( szMessage, zsizeof( szMessage ), "." );

         MessageSend( vSubtask, "OP00103", "Operations",
                      szMessage,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

         TerminateActionForError( vSubtask );
         return( -1 );
      }

      nRC =
         DropMetaOI( vSubtask, vHEADERMETA );
      return( 1 );
   }

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_SelectHeaderFile( zVIEW vSubtask )
{
   zVIEW    vHEADERMETA;
   zVIEW    vSOURCEMETA;
   zSHORT   nRC;
   zCHAR    szSrcType[ 2 ];

   nRC = GetViewByName( &vSOURCEMETA, "SOURCEMETA", vSubtask, zLEVEL_TASK );
   if ( nRC != zLEVEL_TASK )
      return( 0 );

   nRC = GetStringFromAttribute( szSrcType, zsizeof( szSrcType ), vSOURCEMETA,
                                 "SourceFile", "OwnerType" );
   if ( szSrcType[ 0 ] == 'G' )
   {
      nRC = GetViewByName( &vSOURCEMETA, "OPERATION", vSubtask, zLEVEL_TASK );
      if ( nRC != zLEVEL_TASK )
         return( 0 );
   }

   nRC = fnActivateHeader( vSubtask, &vHEADERMETA, zREFER_HDR_META );

   if ( nRC != 1 )
      return( -1 );

   nRC = CheckExistenceOfEntity( vSOURCEMETA, "HeaderFile" );
   if ( nRC == 0 )
   {
      nRC = ExcludeEntity( vSOURCEMETA, "HeaderFile", zREPOS_NONE );
   }

   nRC = IncludeSubobjectFromSubobject( vSOURCEMETA, "HeaderFile",
                                        vHEADERMETA, "HeaderFile", zPOS_LAST );
   nRC = DropMetaOI( vSubtask, vHEADERMETA );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_AddHeaderFile( zVIEW vSubtask )
{
   zVIEW    vHEADERMETA;
   zSHORT   nRC;

   nRC = fnCommitMetaOI( vSubtask, vSubtask );

   if ( nRC == -1 )
      return( -1 );

   nRC = ActivateEmptyMetaOI( vSubtask, &vHEADERMETA,
                              zSOURCE_HDR_META,
                              zSINGLE | zLEVEL_APPLICATION );
   if ( nRC != 1 )
      return( -1 );

   CreateMetaEntity( vSubtask, vHEADERMETA, "HeaderFile", zPOS_AFTER );
   SetNameForView( vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_UpdateHeaderFile( zVIEW vSubtask )
{
   zVIEW    vHEADERMETA;

   fnActivateHeader( vSubtask, &vHEADERMETA, zSOURCE_HDR_META );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HeaderFileOK( zVIEW vSubtask )
{
   fnCommitMetaOI( vSubtask, vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HeaderFileCancel( zVIEW vSubtask )
{
   zVIEW    vHEADERMETA;
   zSHORT   nRC;

   nRC =
      GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK );

   nRC =
      DropMetaOI( vSubtask, vHEADERMETA );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HeaderFileNext( zVIEW vSubtask )
{
   zVIEW    vHEADERMETA;

   fnCommitMetaOI( vSubtask, vSubtask );
   fnActivateHeader( vSubtask, &vHEADERMETA, zSOURCE_HDR_META );
   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HeaderFilePrev( zVIEW vSubtask )
{
   zVIEW    vHEADERMETA;

   fnCommitMetaOI( vSubtask, vSubtask );
   fnActivateHeader( vSubtask, &vHEADERMETA, zSOURCE_HDR_META );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HeaderFileDelete( zVIEW vSubtask )
{
   zVIEW    vHEADERMETA;
   zVIEW    vHEADERCM;
   zSHORT   nRC;
   zCHAR    szName[ 32 ];
   zCHAR    szMessage[ 128 ];
   zLONG    lZKey;

   nRC = GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK );

   if ( nRC != zLEVEL_TASK )
   {
      nRC = fnActivateHeader( vSubtask, &vHEADERMETA, zSOURCE_HDR_META );
      if ( nRC != 1 )
      {
         GetStringFromAttribute( szName, zsizeof( szName ), vHEADERMETA, "HeaderFile", "Name" );

         strcpy_s( szMessage, zsizeof( szMessage ), "Header File " );
         strcat_s( szMessage, zsizeof( szMessage ), szName );
         strcat_s( szMessage, zsizeof( szMessage ), " cannot be deleted." );

         MessageSend( vSubtask, "OP00104", "Operations",
                      szMessage,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

         TerminateActionForError( vSubtask );
         return( -1 );
      }
   }

   nRC = GetStringFromAttribute( szName, zsizeof( szName ), vHEADERMETA, "HeaderFile", "Name" );

   strcpy_s( szMessage, zsizeof( szMessage ), "Delete Header File " );
   strcat_s( szMessage, zsizeof( szMessage ), szName );
   strcat_s( szMessage, zsizeof( szMessage ), " from LPLR ?" );

   nRC = MessagePrompt( vSubtask, "OP00105",
                        "Domain Management",
                        szMessage,
                        zBEEP, zBUTTONS_YESNO,
                        zRESPONSE_NO, 0 );

   if ( nRC == zRESPONSE_YES )
   {
      RetrieveViewForMetaList( vSubtask, &vHEADERCM, zREFER_HDR_META );
      GetIntegerFromAttribute( &lZKey, vHEADERMETA, "HeaderFile", "ZKey" );
      SetCursorFirstEntityByInteger( vHEADERCM, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      DropMetaOI( vSubtask, vHEADERMETA );
      DeleteMetaOI( vSubtask, vHEADERCM, zSOURCE_HDR_META );
      RetrieveViewForMetaList( vSubtask, &vHEADERCM, zREFER_HDR_META );
      SetNameForView( vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
      SetCursorFirstEntity( vHEADERCM, "W_MetaDef", "" );

      if ( nRC <= zCURSOR_UNCHANGED )
         return( -1 );

      fnActivateHeader( vSubtask, &vHEADERMETA, zSOURCE_HDR_META );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZOPHDRD_HDR_Set_Caption
//
// ACTION:     Post-build of HDR_Select window
//
// PURPOSE:    Sets the Window caption to the title in the
//             ProfileXfer object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_Set_Caption( zVIEW    vSubtask )
{
   zVIEW    vProfileXFER;
   zCHAR    szWrk[ 256 ];

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szWrk, zsizeof( szWrk ), vProfileXFER, "OP", "Title" );
   SetWindowCaptionTitle( vSubtask, 0, szWrk );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_AddNew( zVIEW    vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vHDR_Upd;
   zLONG    lRC;
   zCHAR    szTempString_0[ 32 ];
   zCHAR    szDesc[ 256 ];
   zSHORT   nZRetCode;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ),
                                        vProfileXFER, "OP", "HeaderFileName" );
   if ( !szTempString_0[ 0 ] )
   {
      MessageSend( vSubtask, "OP00106", "Operations",
                   "Please enter a name for the Header.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   lRC = CheckExistenceOfMetaOI( vSubtask, szTempString_0, zSOURCE_HDR_META );
   if ( lRC == 1 )
   {
      MessageSend( vSubtask, "OP00107", "Operations",
                   "A Header File already exists by that name.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }
   else
   {
      ActivateEmptyMetaOI( vSubtask, &vHDR_Upd, zSOURCE_HDR_META,
                            zSINGLE | zLEVEL_APPLICATION );
      CreateMetaEntity( vSubtask, vHDR_Upd, "HeaderFile", zPOS_AFTER );
      SetAttributeFromAttribute( vHDR_Upd, "HeaderFile", "Name",
                                  vProfileXFER, "OP", "HeaderFileName" );
      GetCtrlText( vSubtask, "Desc", szDesc, 256 );
      SetAttributeFromString( vHDR_Upd, "HeaderFile", "Desc", szDesc );
      SetNameForView( vHDR_Upd, "HEADERMETA", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_ExcludeEntity( zVIEW    vSubtask )
{
   zVIEW    vPassedInByName;
   zVIEW    vProfileXFER;
   zVIEW    vHEADERCM;
   zCHAR    szHdr_TgtEntity[ 33 ];

// get a view to the CM List of Meta Type for the Selection
   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );

// get a view to the Xfer Info
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

// get the passed ViewName and EntityName of the target of the include
//     ( szHdr_TgtEntity used as temp string to save stack space )
   GetStringFromAttribute( szHdr_TgtEntity, zsizeof( szHdr_TgtEntity ),
                             vProfileXFER, "OP", "PassedViewName" );
   GetViewByName( &vPassedInByName, szHdr_TgtEntity,  vSubtask, zLEVEL_ANY );

// Now we can Exclude it, if it exists
   GetStringFromAttribute( szHdr_TgtEntity, zsizeof( szHdr_TgtEntity ),
                            vProfileXFER, "OP", "PassedEntityName" );
   if ( CheckExistenceOfEntity( vPassedInByName, szHdr_TgtEntity )
        == zCURSOR_SET )
   {
      ExcludeEntity( vPassedInByName, szHdr_TgtEntity, zREPOS_FIRST );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_IncludeEntity( zVIEW    vSubtask )
{
   zVIEW    vPassedInByName;
   zVIEW    vProfileXFER;
   zVIEW    vHEADERCM;
   zVIEW    vHDR_Ref;
   zSHORT   nRC;
   zCHAR    szViewName[ 33 ];
   zCHAR    szHdr_TgtEntity[ 33 ];

// get a view to the CM List of Meta Type for the Selection
   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );

// get a view to the Xfer Info
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

// get the passed ViewName and EntityName of the target of the include
   GetStringFromAttribute( szViewName, zsizeof( szViewName ),
                             vProfileXFER, "OP", "PassedViewName" );
//jGetViewByName( &vPassedInByName, szViewName,  vSubtask, zLEVEL_ANY );
   GetViewByName( &vPassedInByName, "OPERATION", vSubtask, zLEVEL_TASK );

// Now Exclude it, if it exists prior to including
//jGetStringFromAttribute( szHdr_TgtEntity, zsizeof( szHdr_TgtEntity ),
//j                         vProfileXFER, "OP", "PassedEntityName" );
//j   if ( CheckExistenceOfEntity( vPassedInByName, szHdr_TgtEntity ) == zCURSOR_SET )
   strcpy_s( szHdr_TgtEntity, zsizeof( szHdr_TgtEntity ), "HeaderFile" );

   if ( CheckExistenceOfEntity( vPassedInByName, szHdr_TgtEntity ) == zCURSOR_SET )
   {
      nRC = ExcludeEntity( vPassedInByName, szHdr_TgtEntity, zREPOS_FIRST );
   }

// Get a reference view for the include source and do the Include
   ActivateMetaOI( vSubtask, &vHDR_Ref, vHEADERCM, zREFER_HDR_META,
                   zSINGLE | zLEVEL_APPLICATION );
   if ( vHDR_Ref != 0 )
   {
      IncludeSubobjectFromSubobject( vPassedInByName, szHdr_TgtEntity,
                                      vHDR_Ref, "HeaderFile",
                                      zPOS_AFTER );
      // next "if" is temp until derived attr or code change
      //if( zstrcmp( szViewName, "TZZOLODO" ) != 0 )
      //{
      //   SetAttributeFromAttribute( vPassedInByName,
      //                              "Operation", "HeaderFile",
      //                              vHDR_Ref, "HeaderFile", "Name" );
      //}
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_ViewInitWindow( zVIEW    vSubtask )
{
   zVIEW    vHEADERCM;
   zVIEW    vHDR_Ref;

   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_ANY );
   ActivateMetaOI( vSubtask, &vHDR_Ref, vHEADERCM, zREFER_HDR_META,
                   zSINGLE | zLEVEL_APPLICATION );
   if ( vHDR_Ref != 0 )
   {
      SetNameForView( vHDR_Ref, "HDR", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_UpdateSelected( zVIEW    vSubtask )
{
   zVIEW    vHEADERCM;
   zVIEW    vHDR_Upd;

   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_ANY );
   ActivateMetaOI( vSubtask, &vHDR_Upd, vHEADERCM, zSOURCE_HDR_META,
                   zSINGLE | zLEVEL_APPLICATION );
   if ( vHDR_Upd != 0 )
   {
      SetNameForView( vHDR_Upd, "HDR", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_UpdateOK( zVIEW    vSubtask )
{
   zVIEW    vHDR_Upd;
   zSHORT   nRC;

   GetViewByName( &vHDR_Upd, "HDR", vSubtask, zLEVEL_ANY );

   if ( ObjectInstanceUpdatedFromFile( vHDR_Upd ) == 1 )
   {
      nRC = CommitMetaOI( vSubtask, vHDR_Upd, zSOURCE_HDR_META );
      if ( nRC == -1 )
      {
         MessageSend( vSubtask, "OP00108", "Operations",
                      "Header File Name not unique.\n\n"
                      "Rename or Delete from Header File List.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      }
   }

   DropMetaOI( vSubtask, vHDR_Upd );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_UpdateCancel( zVIEW    vSubtask )
{
   zVIEW    vHDR_Upd;

   GetViewByName( &vHDR_Upd, "HDR", vSubtask, zLEVEL_ANY );

   DropMetaOI( vSubtask, vHDR_Upd );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_UpdateNext( zVIEW    vSubtask )
{
   zVIEW    vHEADERCM;
   zVIEW    vHDR_Upd;
   zSHORT   nRC;

   GetViewByName( &vHDR_Upd, "HDR", vSubtask, zLEVEL_ANY );

   if ( ObjectInstanceUpdatedFromFile( vHDR_Upd ) == 1 )
   {
      nRC = CommitMetaOI( vSubtask, vHDR_Upd, zSOURCE_HDR_META );
      if ( nRC == -1 )
      {
         MessageSend( vSubtask, "OP00109", "Operations",
                      "Header File Name not unique.\n\n"
                      "Rename or Delete from Header File List.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 0 );
      }
   }

   DropView( vHDR_Upd );

// Now get the next one to update
   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_ANY );
   SetCursorNextEntity( vHEADERCM, "W_MetaDef", "" );
   ActivateMetaOI( vSubtask, &vHDR_Upd, vHEADERCM, zSOURCE_HDR_META,
                   zSINGLE | zLEVEL_APPLICATION );
   if ( vHDR_Upd != 0 )
   {
      SetNameForView( vHDR_Upd, "HDR", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_UpdatePrev( zVIEW    vSubtask )
{
   zVIEW    vHEADERCM;
   zVIEW    vHDR_Upd;
   zSHORT   nRC;

   GetViewByName( &vHDR_Upd, "HDR", vSubtask, zLEVEL_ANY );

   if ( ObjectInstanceUpdatedFromFile( vHDR_Upd ) == 1 )
   {
      nRC = CommitMetaOI( vSubtask, vHDR_Upd, zSOURCE_HDR_META );
      if ( nRC == -1 )
      {
         MessageSend( vSubtask, "OP00110", "Operations",
                      "Header File Name not unique.\n\n"
                      "Rename or Delete from Header File List.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 0 );
      }
   }

   DropView( vHDR_Upd );

// Now get the previous one to update
   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_ANY );
   SetCursorPrevEntity( vHEADERCM, "W_MetaDef", "" );
   ActivateMetaOI( vSubtask, &vHDR_Upd, vHEADERCM, zSOURCE_HDR_META,
                   zSINGLE | zLEVEL_APPLICATION );
   if ( vHDR_Upd != 0 )
   {
      SetNameForView( vHDR_Upd, "HDR", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_UpdateDelete( zVIEW    vSubtask )
{
   zVIEW    vHEADERCM;
   zVIEW    vHDR_Upd;
   zCHAR    szWork[ 128 ];

   GetViewByName( &vHDR_Upd, "HDR", vSubtask, zLEVEL_ANY );

// Prompt to confirm Delete
   strcpy_s( szWork, zsizeof( szWork ), "Delete Header File\n\n" );
   GetStringFromAttribute( szWork + zstrlen( szWork ), zsizeof( szWork ) - zstrlen( szWork ), vHDR_Upd, "HeaderFile", "Name" );
   if ( MessagePrompt( vSubtask, "OP00111",
                       "Domain Management",
                       szWork,
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_NO,      0 )  == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
   DeleteMetaOI( vSubtask, vHEADERCM, zSOURCE_HDR_META );

// Now get the next one to update
   SetCursorNextEntity( vHEADERCM, "W_MetaDef", "" );
   ActivateMetaOI( vSubtask, &vHDR_Upd, vHEADERCM, zSOURCE_HDR_META,
                   zSINGLE | zLEVEL_APPLICATION );
   if ( vHDR_Upd != 0 )
   {
      SetNameForView( vHDR_Upd, "HDR", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_InitCopyWindow( zVIEW    vSubtask )
{
   zVIEW    vHEADERCM;
   zVIEW    vProfileXFER;

   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   SetAttributeFromAttribute( vProfileXFER, "OP", "HeaderFileName",
                              vHEADERCM, "W_MetaDef", "Name" );
   SetAttributeFromAttribute( vProfileXFER, "OP", "HeaderFileDesc",
                              vHEADERCM, "W_MetaDef", "Desc" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_CreateByCopy( zVIEW    vSubtask )
{
   zVIEW    vHEADERCM;
   zVIEW    vProfileXFER;
   zVIEW    vHDR_Ref;
   zVIEW    vHDR_Upd;
   zSHORT   nRC;
   zCHAR    szHdrName[ 32 ];

   GetViewByName( &vHEADERCM, "HEADERCM", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szHdrName, zsizeof( szHdrName ),
                           vProfileXFER, "OP", "HeaderFileName" );
   nRC = CheckExistenceOfMetaOI( vSubtask, szHdrName, zREFER_HDR_META );
   if ( nRC == 1 )
   {
      MessageSend( vSubtask, "OP00112", "Operations",
                   "A Header File already exists by that name.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

// Get a reference view for the source of Copy
   ActivateMetaOI( vSubtask, &vHDR_Ref, vHEADERCM, zREFER_HDR_META,
                   zSINGLE | zLEVEL_APPLICATION );

// Get a updateable view for the target of Copy
   ActivateEmptyMetaOI( vSubtask, &vHDR_Upd, zSOURCE_HDR_META,
                        zSINGLE | zLEVEL_APPLICATION );

// Copy root of Meta
   nRC = CreateMetaEntity( vSubtask, vHDR_Upd, "HeaderFile", zPOS_AFTER );
   nRC = SetMatchingAttributesByName( vHDR_Upd, "HeaderFile",
                                      vHDR_Ref, "HeaderFile",
                                      zSET_NULL | zSET_NOTNULL );
   SetAttributeFromString( vHDR_Upd, "HeaderFile", "Name", szHdrName );
   SetAttributeFromAttribute( vHDR_Upd, "HeaderFile", "Desc",
                              vProfileXFER, "OP", "HeaderFileDesc" );

// Copy all sub entities
   nRC = SetCursorFirstEntity( vHDR_Ref, "DefinedItem", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vHDR_Upd, "DefinedItem", zPOS_LAST );
      nRC = SetMatchingAttributesByName( vHDR_Upd, "DefinedItem",
                                         vHDR_Ref, "DefinedItem",
                                         zSET_NULL | zSET_NOTNULL);

      nRC = SetCursorNextEntity( vHDR_Ref, "DefinedItem", "" );
   }

// Name updateable view for mapping
   SetNameForView( vHDR_Upd, "HDR", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_AddItem( zVIEW    vSubtask )
{
   zVIEW    vHEADERMETA;

   GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vHEADERMETA, "DefinedItem", zPOS_AFTER );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_DefItemAddNew( zVIEW    vSubtask )
{
   zVIEW    vHEADERMETA;

   GetViewByName( &vHEADERMETA, "HEADERMETA", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vHEADERMETA, "DefinedItem", zPOS_AFTER );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_DefUpdateNext( zVIEW    vSubtask )
{
   zVIEW    vHDR_Upd;

   GetViewByName( &vHDR_Upd, "HDR", vSubtask, zLEVEL_ANY );

   SetCursorNextEntity( vHDR_Upd, "DefinedItem", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_DefUpdatePrev( zVIEW    vSubtask )
{
   zVIEW    vHDR_Upd;

   GetViewByName( &vHDR_Upd, "HDR", vSubtask, zLEVEL_ANY );

   SetCursorPrevEntity( vHDR_Upd, "DefinedItem", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HDR_DefUpdateDelete( zVIEW    vSubtask )
{
   zVIEW    vHDR_Upd;
   zCHAR    szWork[ 128 ];

   GetViewByName( &vHDR_Upd, "HDR", vSubtask, zLEVEL_ANY );

// Prompt to confirm Delete
   strcpy_s( szWork, zsizeof( szWork ), "Delete Defined Item\n\n" );
   GetStringFromAttribute( szWork + zstrlen( szWork ), zsizeof( szWork ) - zstrlen( szWork ), vHDR_Upd, "DefinedItem", "ExternalValue" );
   if ( MessagePrompt( vSubtask, "OP00113", "Domain Management", szWork,
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 )  == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   DeleteEntity( vHDR_Upd, "DefinedItem", zREPOS_NEXT );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION   zwTZOPHDRD_HelpAbout
//
// PURPOSE:    This Entry does the About
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HelpAbout( zVIEW vSubtask )
{

   IssueToolAbout( vSubtask, zTOOL_HDR, __FILE__, __DATE__, __TIME__ );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZOPHDRD_HelpBasics
//
// PURPOSE:    This Entry transfers to basic help
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZOPHDRD_HelpOnHelp
//
// PURPOSE:    This Entry transfers to HelpOnHelp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZOPHDRD_HelpContents
//
// PURPOSE:    This Entry transfers to help Contents
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOPHDRD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

#ifdef __cplusplus
}
#endif
