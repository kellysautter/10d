/*
CHANGE LOG
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2002.06.11 BL   R56704
   If TB DB Handler, disable Build Synch DDL
2002.03.12 FH
   avoid "ViewIsReadOnly" if a meta is not checked out.
2001.06.24 BL   R55312
   If create a new TE and Build Tables/Rels, then enable Action for Generate DDL
2001.02.19 BL   R54267
   Exists in ERD a Identifier without Attribute or Relationship, then
   Build Tables/Rels looped
2001.02.16  HH
   Foreign Key maintenance: Call Bianca's (SAP) code( now in TZTENVRO ).
2001.02.15    BL    R54697
   improve handling of return code from InitializeLPLR
2001.02.05  RG  Z2000
   Fixed Bug R54630: We need a new parameter for function BuildDDL
2000.12.06  BL
   Modified zwTZTEUPDD_InitMenu for check exists of Entity
2000.11.02  BL
   If SAP Data Source selected, disable all functions for update
2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.25  SR  Z2000 Size of path variables
   Modified size of size of all filename and path variables in whole module
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
2000.10.24  HH
   Improve Foreign Key maintenance, to allow ER attributes to be turned to FK.
2000.09.19  BL
   New Handling for Set Keep Physical Characteristics
   Delete functions for new Column and new Table
2000.08.23  DGC
   Fixed bug R53432.  Physical data was not being correctly set for FK's.
2000.08.07  BL  TB 51244, TB 50831
   if a field a part of Key, set Key Flag
   Added zwTZTEUPDD_DropErrorMsgView, zwTZTEUPDD_ClearErrorWindow
   for write all Error Messages to Error List Window, no Message for
   one Error ( Bug TB 50831 )
2000.08.04  BL  TB 50934, TB 51256
   Modified zwTZTEUPDD_BuildXODsOnServer, zwTZTEUPDD_BuildXODsOnLPLR
   remove deselection
   Modified zwTZTEUPDD_SelectDBMS: do not remove default owner
2000.07.09  BL
   Replace Technical Environment through Physical Data Model
2000.07.04  BL    Z10  Repository and Tools + TB 52978
   Set check out state in title. Is TE not checked out, disable all
   menu items and Buttons for update
2000.05.23  BL    Z2000
   Modified ForKeyM_SwitchForKeys for show original field sequence in window
   Foreign Key Maintenance
2000.05.22  BL    Z2000
   Modified zwfnTZTEUPDD_InitDTE, zwTZTEUPDD_SaveModelDTE, fnRebuildTables,
   zwTZTEUPDD_GenerateDDL, zwTZTEUPDD_BuildXODsLOD_List,
   zwTZTEUPDD_SyncRelationships, zwTZTEUPDD_InitMenu, zwTZTEUPDD_FieldAssignZKey,
   zwTZTEUPDD_DeleteSelectedColumn and added zwTZTEUPDD_DeleteField,
   zwTZTEUPDD_SaveSelectedField, zwTZTEUPDD_MoveField for show original filed sequence
   Added zwTZTEUPDD_NextTable zwTZTEUPDD_PrevTable, zwTZTEUPDD_TableRecAssignZKey,
   zwTZTEUPDD_DeleteTable, zwTZTEUPDD_SaveSelectedField for Refresh main window
2000.04.12  TMV   Z2000
   Function definition from
   zwTZTEUPDD_ReconstructEntitiesAndForeignKeys( vSubtask, vDTE, vEMD )
   to
   zwTZTEUPDD_ReconstructEntitiesAndForeignKeys( zVIEW vSubtask, zVIEW vDTE, zVIEW vEMD )
   to make source almost compilable
2000.04.12  DC   Z2000
   Made modifications and additions to support the "Keep Physical Characteristics"
   functionality for tables and fields.
2000.04.10  RG   Z2000
    We will not synchronize relationships for work entities
2000.04.07  RG   Z2000
    - Use function zwTZTEUPDD_ReconstructERForForeignKey recursive because
      of 'Record Types'
2000.04.06  RG   Z2000
    - oTZTENVRO_BuildRelationsFromEMD only the datasource is not for Siron Catalogs
    - Disable/Enable menu items for Siron Catalogs
    - Disable CheckBox 'Keep Physical Information' in Windows Table/Records and
      FieldData/Relationship for Siron Catalogs
    - Delete of entities in function fnRebuildTables not for Siron Catalogs
2000.03.30  DC   Z2000
   Made modifications and additions to support the "Keep Physical Characteristics"
   functionality for tables and fields.
2000.03.24  RG   Z2000
   Disabled DBHandler selection for a DataSource if SironFlatFiles have
   been selected first.
2000.03.06  TMV  Z2000
   changed zwTZTEUPDD_DeleteDataSource( zVIEW vSubtask ) to type of
   zOPER_EXPORT zSHORT OPERATION because it is used as a dialog operation.
2000.02.10  RG  Z2000
   Bugfix: If we delete selected entities from a datasource we have to delete
   the foreign keys from the related entities too.
1999.11.29  BL  Z10  new menu structure
   Modified Operation zwTZTEUPDD_MainRtnFromSubwnd for Refresh window TableRecord
   Added Operation zwTZTEUPDD_SetForeignKeyCaption: set Table Name in Caption
   Added Operation zwTZTEUPDD_InitMenuEdit for enable or disable edit menu items
   Modified Operation zwTZTEUPDD_DataSrcPopup, zwTZTEUPDD_TableRecPopup
   for enable and disable Popup items
   Added Operation zwTZTEUPDD_DeleteSelectedKeys and zwTZTEUPDD_DeleteSelectedColumn
   for delete all selected Keys/Column
   Modified Operation zwTZTEUPDD_DeleteSelectedEntity: delete all selected Tables
   Added Operation zwTZTEUPDD_NextDataSource, zwTZTEUPDD_PrevDataSource:
   load DB Handle for current Data Source
   Modified Operation zwfnTZTEUPDD_InitDTE for Caption Title and remove EnableAction
1999.11.28  BL  Z9J, Z10  VSAM
   Modified Operation zwTZTEUPDD_DeleteVsamTables for delete all selected Tables
   Added Operation zwTZTEUPDD_DeleteDataSource for Delete Data Source with VSAM
   DB Handler
   Added Operation zwTZTEUPDD_InitMenu and Modified Operation zwTZTEUPDD_TableRecPopup
   for enable and disable Menu items
   Addded Operation zwTZTEUPDD_SyncPrev
   Rename Operation zwTZTEUPDD_SyncCancel in zwTZTEUPDD_SyncNext
   Modified Operation zwTZTEUPDD_SyncRelationships, zwTZTEUPDD_SyncCancel,
   zwTZTEUPDD_SyncInitWorkObject and zwTZTEUPDD_SyncOK:
   1. Has TE 2 Data Sources, then you cannot call Synchronize Relationship
   2. Update Window function
1999.11.28  BL  Z10  new menu structure
   Added Operation zwTZTEUPDD_LoadTableDetail, zwTZTEUPDD_AcceptTableRecord,
   zwTZTEUPDD_CancelTableRecord: Window TableDetail is a modeless Window
   Added Operation zwTZTEUPDD_DisableButton and Modified Operation
   zwTZTEUPDD_SyncSelectC_Target, zwTZTEUPDD_SyncSelectC_Source and
   zwTZTEUPDD_SyncDeselectTarget, zwTZTEUPDD_SyncCancel and
   zwTZTEUPDD_SyncOK for enable or disable Button by Synchronize Relationship
   Modified Operation zwTZTEUPDD_SyncSelectC_Source: check exists of
   TargetKey
   Modified Operation FieldRelLink_SetDB_DataTypes for new design
1999.10.25  RG  Z9J, Z2000  VSAM
   - Fixed parameter 3 for function call DeleteEntity in function
     zwTZTEUPDD_DeleteDataSource
1999.10.20  RG  Z9J, Z2000  VSAM
   - We have to reconstrut the ER-Attributes while deleting the
     datasource too
1999.10.08  RG  Z2000
    Fix in operation zwTZTEUPDD_SyncSelectC_Source:
    Position Cursor on the correct target entity (compare operation:
    zwTZTEUPDD_SyncSelectC_Target)
1999.09.29  BL  Z2000
   update Operation zwTZTEUPDD_KeyAssignZKey and zwTZTEUPDD_FieldAssignZKey
   if a new table is created with a new Key or Column, Zeidon steps into
   an endless loop, because the Entity TE_TablRecKey does not exists
1999.09.22  RG  Z9J, Z2000  VSAM
   Added ability to set DBH-specific information from TE.
1999.09.03  RG  Z9J, Z2000  VSAM
   - Reconstruction of ER-Attributes while deleting tables from TE
     wasn't correct if TE_Tablename was used
1999.08.24  RG  Z9J, Z2000  VSAM
   - New Dialog and new functions for 'Detele all tables' for Siron
     catalogs. We need a workaround, that we can save the ERD without
     problems. (View: rxxxxx in Zeidon Object Threads is removed if
     a subwindow is closed. Therefore we ask: "Do you really want delete"
     for Siron catalogs in a subwindow and not in a message box.)
1999.08.13  RG  Z9J, Z2000  VSAM
   - We don't need the XSK-Files on servers any longer
1999.08.10  RG  Z9J, Z2000  VSAM
   - New Dialog Operation: DeleteSelectedEntity for databases and for
     Siron Flat Files
1999.08.09  RG  Z9J, Z2000  VSAM
   - Disable 'Init Tables' and 'Synchronize Relationships' if deleting
     all tables and enable those functions after saving the model
1999.08.06  RG  Z9J, Z2000  VSAM
   - New local function: reconstruct Foreign keys to attributes in
     datamodel for SironFlatFiles before deleting the tables from TE
1999.08.05  RG  Z9J, Z2000  VSAM
   - Enable 'Init Tables' for Siron Flat Files
   - Set DIL Message for function 'Synchronize Relationships'
   - Delete all functions for 'Synchronize Identifiers'
From here old version
05.05.1997    DonC
   Corrected InitDTE for graying options.  It was giving error message when
   no DBMS Source entity existed.
09.06.1997    DonC
   Add Build... operations to build XMD and XOD executables and send to
   Server.
13.06.1997    RG
   Add sending the Siron *.XSK-files in function zwTZTEUPDD_BuildXODsOnServer.
19.07.1997    DonC
   Modified zwTZTEUPDD_BuildXODsOnServer to send "complete" message to DIL
   at end and build work LODs as well as DB LODs.
08.10.1997    DonC
   Modified zwTZTEUPDD_TableRecAssignZKey to eliminate error when there are
   no TableRec entities.
08.10.1997    DonC
   Made modifications to shut down TE tool initialization if TE tool is
   already up.
11.10.1997    DonC
   Modified Postbuild, Next and Prev processing for column detail to make
   the partitions showing ER_Attribute and ER_RelLink information invisible
   when either of the information didn't exist.
06.11.1997    DonC
   Modified BuildXODsLOD_List to order LODs by name BuildXODsOnLPLR to not
   give error message when LOD does not have a POD.
12.11.197     DonC
   Modified BuildXMD_OnServer to give complete message when task is done.
24.01.1998    DonC
   Modified zwTZTEUPDD_BuildXODsOnLPLR to remove save of TE.
23.04.1998    DonC
   Added zwTZTEUPDD_BuildNetXODsOnLPLR to support new dialog function for
   building network XODs. (QS 89)
18.08.1998    DonC
   Modified call to InitializeDefaultLPL to check for error return code.
*/

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define KZOESYS_INCL
#define zNO_KZOEINCO    // to get the correct operation thru zGET_PTR
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


// We need to define MutexInfo as a dummy typedef.  DO NOT TRY TO USE THIS!
typedef long MutexInfo;

#define  zType_VSAM   'F'
#define  zType_SAP    'S'

// LAST MESSAGE ID:  TE00434

zCHAR szWorkMessage[ zMAX_MESSAGE_LTH + 1 ];
zCHAR sz[ zSHORT_MESSAGE_LTH + 1 ];

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SaveModelDTE( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwfnTZTEUPDD_InitDTE( zVIEW vSubtask,
                      zVIEW vCM_List );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SortByName( zVIEW    vSubtask );

zOPER_EXPORT zSHORT OPERATION
oTZTEMPMO_Create( zPVIEW   vMPM,
                  zVIEW    vEMD_in,
                  zVIEW    vTE_in );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncInitWorkObject( zVIEW vSubtask,
                               zVIEW vDTE,
                               zVIEW vEMD );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SelectDBMS( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
ForKeyM_Prebuild( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ReconstructER_Attributes( zVIEW vSubtask, zVIEW vDTE, zVIEW vEMD );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SetERDUpdateFlag( zVIEW vSubtask, zPVIEW pvTE_Work, zVIEW vDTE );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ReconstructSelectedEntity( zVIEW vSubtask, zVIEW vDTE, zVIEW vEMD );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ReconstructERForForeignKey( zVIEW vSubtask, zVIEW vDTE, zVIEW vEMD );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SetERAttributeWithDomain( zVIEW  vSubtask,
                                     zVIEW vDTE_Work, zVIEW vEMD );

zSHORT /* DIALOG */  OPERATION
zwTZTEUPDD_ReconstructEntitiesAndForeignKeys( zVIEW vSubtask, zVIEW vDTE, zVIEW vEMD );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SetFactType( zVIEW vSubtask, zVIEW vDTE_Work, zVIEW vEMD );

zSHORT /* LOCAL */  OPERATION
zwTZTEUPDD_WindowDeleteVsam( zVIEW vSubtask, zLONG ullDelete );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DisableButton( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncOK( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_InitMenu( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_NewFieldSequence( zVIEW vDTE );

zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_DelFieldFromSequence( zVIEW vDTE );

zOPER_EXPORT zSHORT OPERATION
FieldRelLink_SetDB_DataTypes( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
SetTitleWithCheckOutState( zVIEW    vSubtask,
                            zPCHAR  szToolname,
                            zPCHAR  szLodname,
                            zVIEW   vToolView,
                            zPCHAR  szEntityname,
                            zLONG   lType );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_LoadPopup( zVIEW  vSubtask,
                      zPCHAR szEntityName,
                      zPCHAR szActionNameForNew,
                      zPCHAR szActionNameForDelete,
                      zPCHAR szPopupName );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_DeleteSubobject( zVIEW vSubtask,
                            zSHORT nObject,
                            zPCHAR szEntityName,
                            zSHORT nCloseWindow );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_RefreshWindows( zVIEW  vSubtask,
                           zPCHAR szWindowName,
                           zPCHAR szEntityName );

zOPER_EXPORT zLONG OPERATION
zGetStringWithoutBlank( zPCHAR   szSourceString,
                        zPCHAR   pszTargetString );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_AcceptSubobject( zVIEW  vSubtask,
                            zPCHAR szEntityName,
                            zPCHAR szMessageText,
                            zSHORT bCheck );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_SaveViewCursor( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_CloseTablRecordWindow( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_KeyListPostbuild( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_RemoveMultiSelection( zVIEW vSubtask,
                                 zPCHAR szEntityName,
                                 zPCHAR szWindowName );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_ClearErrorWindow( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DS_TemporaryGraying( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DataSrc_Prebuild( zVIEW vSubtask );

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZTEUPDD_InitTool
// PURPOSE:    This Entry switches to a NEW screen if no EMD loaded.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_InitTool( zVIEW vSubtask )
{
   zVIEW       vT;
   zVIEW       vTZTEDBLO;
   zSHORT      nRC;

   nRC = oTZ__PRFO_GetViewToProfile( &vT, "TE", vSubtask, zCURRENT_OI );
   if ( nRC >= 0 )
      SetNameForView( vT, "ProfileXFER", vSubtask, zLEVEL_TASK );

   ActivateOI_FromFile( &vTZTEDBLO, "tztedblo", vSubtask, "tztedblo.xdl",
                        zMULTIPLE );
   SetNameForView( vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   if ( InitializeDefaultLPL( vSubtask ) < 0 )
      return( -1 );

   // Always have a Named List of Meta's!
   // This is done to be consistent with other dialogs.  The only time
   // that the named List will not exist is during a SwitchLPLR.
   // This fact is used a "flag" to the RtnFromSubwnd logic.
   RetrieveViewForMetaList( vSubtask, &vT, zSOURCE_DTE_META );
   SetNameForView( vT, "CM_DTE_List", vSubtask, zLEVEL_TASK );

   nRC = zwfnTZTEUPDD_InitDTE( vSubtask, vT );
   if ( nRC < 0 )
   {
      // Abort if error in InitDTE, which likely occurs because TE tool
      // is already up.
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
      return( 0 );
   }

   // DGC 12/4/96  Following is short term fix for new-style dbhandler stuff.
   // It can be removed soon.
#if 1
   GetViewByName( &vT, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   for ( nRC = SetCursorFirstEntity( vT, "TE_DBMS_Source", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vT, "TE_DBMS_Source", 0 ) )
   {
      zPCHAR psz;

      GetAddrForAttribute( &psz, vT, "TE_DBMS_Source", "DBMS" );
      if ( psz && zstrlen( psz ) == 1 )
      {
         switch ( *psz )
         {
            case 'B':
               SetAttributeFromString( vT, "TE_DBMS_Source", "DBMS",
                                       "SqlBase" );
               break;

            case 'M':
               SetAttributeFromString( vT, "TE_DBMS_Source", "DBMS",
                                       "SqlServer" );
               break;

            case 'V':
               SetAttributeFromString( vT, "TE_DBMS_Source", "DBMS",
                                       "DB Raima" );
               break;

            case 'S':
               SetAttributeFromString( vT, "TE_DBMS_Source", "DBMS",
                                       "Siron" );
               break;

            case 'N':
               SetAttributeFromString( vT, "TE_DBMS_Source", "DBMS",
                                       "Network Server" );
               break;
         }

         SetAttributeFromString( vT, "TE_DBMS_Source", "Server", "" );
         if ( SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS",
                                          vT, "TE_DBMS_Source", "DBMS",
                                          0 ) >= zCURSOR_SET )
         {
            zwTZTEUPDD_SelectDBMS( vSubtask );
         }
      }
   }
#endif

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwfnTZTEUPDD_InitDTE( zVIEW vSubtask,
                      zVIEW vCM_List )
{
   zVIEW       vDTE;
   zVIEW       vDTE_Old;
   zVIEW       vLPLR;
   LPVIEWCSR   lpViewCsr;
   LPVIEWOI    lpViewOI;

   if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   // Load the "MODEL" DTE in the LPLR
   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) < 0 )
   {
      oTZTENVRO_GetUpdViewForDTE_P( vSubtask, &vDTE );
      if ( vDTE )
      {
         SetNameForView( vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
         // Set window Title with check out state
         SetTitleWithCheckOutState( vSubtask, "Physical Data Model",
                                    "TE_DB_Environ", vDTE,
                                    "TE_DB_Environ", zSOURCE_DTE_META );

         // if field sequence does not exist, set it and remove update flag
         oTZTENVRO_SetFieldSequence( vDTE );
         lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vDTE->hViewCsr );
         lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
         lpViewOI->bUpdatedFile = FALSE;

         // if PE not checked out, set TE View read only ->
         // the user cannot update the values in Detail Windows
         if ( !ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ))
            SetViewReadOnly( vDTE );

         if ( GetViewByName( &vDTE_Old, "TE_DB_Environ_old", vSubtask, zLEVEL_TASK ) > 0 )
            DropView( vDTE_Old );
      }
      else
         return( -1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZTEUPDD_MainRtnFromSubwnd
// PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated
//             the current EMD.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_MainRtnFromSubwnd( zVIEW vSubtask )
{
   zVIEW       vT;
   zVIEW       vDTE;
   zVIEW       vWindow;
   zSHORT      nRC;
   zSHORT      nRC2;
   zVIEW       vDTE_Old;

   // Is DTE List Available or we lost our DTE view??
   nRC = GetViewByName( &vT, "CM_DTE_List", vSubtask, zLEVEL_TASK );
   nRC2 = GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 || nRC2 < 1 )
   {    // no - return from SwitchLPLR
        // First, Drop the current DTE view if there
      if ( nRC2 > 0 )
      {
         nRC2 = DropView( vDTE );
      }
        // Now get a new List view and name it
      if ( RetrieveViewForMetaList( vSubtask, &vT, zSOURCE_DTE_META ) > 0 )
         SetNameForView( vT, "CM_DTE_List", vSubtask, zLEVEL_TASK );
   }

   // Now do the right thing to init the new LPLR TE
   zwfnTZTEUPDD_InitDTE( vSubtask, vT );

   //if window TableRecord open
   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow &&
        GetViewByName( &vDTE_Old, "TE_DB_Environ_old", vSubtask, zLEVEL_TASK ) > 0 &&
        GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetCursorFirstEntityByEntityCsr( vDTE, "TE_TablRec", vDTE_Old,
                                       "TE_TablRec", "" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZTEUPDD_ListRtnFromSubwnd
// PURPOSE:    This Entry re-loads an EMD if a SwitchLPLR has invalidated
//             the current EMD.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ListRtnFromSubwnd( zVIEW vSubtask )
{
   zVIEW  vT;
   zVIEW  vDTE;
   zSHORT nRC;
   zSHORT nRC2;

// Is DTE List Available or we lost our DTE view??
   nRC = GetViewByName( &vT, "CM_DTE_List", vSubtask, zLEVEL_TASK );
   nRC2 = GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 || nRC2 < 1 )
   {
      // no - return from SwitchLPLR
      // First, Drop the current DTE view if there
      if ( nRC2 > 0 )
      {
         nRC2 = DropView( vDTE );
      }

      // Now get a new List view and name it
      nRC = RetrieveViewForMetaList( vSubtask, &vT, zSOURCE_DTE_META );
      nRC = SetNameForView( vT, "CM_DTE_List", vSubtask, zLEVEL_TASK );

      // Now do the right thing to init the new LPLR TE
      zwfnTZTEUPDD_InitDTE( vSubtask, vT );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZTEUPDD_SwitchLPLR
// PURPOSE:    This Entry switches to a new LPLR.  All named views to
//             the current LPLR file List must be droped first.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SwitchLPLR( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zSHORT nRC;
   zVIEW  vT;

   // Get Access to current Physical Model Object and prompt for Save
   nRC = GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      if ( zwTZTEUPDD_CloseTablRecordWindow( vSubtask ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
         return( -1 );
      }

      if ( MiGetUpdateForView( vDTE ) == 1  &&
           ObjectInstanceUpdatedFromFile( vDTE ) == 1 )
      {
         strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "Do you wish to save current Physical Model?" );
         nRC = MessagePrompt( vDTE, "TE00410", "Physical Data Model",
                              szWorkMessage,
                              zBEEP,        zBUTTONS_YESNOCANCEL,
                              zRESPONSE_NO, zICON_QUESTION );
         if ( nRC == zRESPONSE_CANCEL )
         {
            SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZTEUPDD", "Main" );
            return( 0 );
         }
         else
         {
            if ( nRC == zRESPONSE_YES )
            {
               zwTZTEUPDD_SaveModelDTE( vDTE );
            }
         }
      }
   }

   // Drop view to DTE File List
   nRC = GetViewByName( &vT, "CM_DTE_List", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      nRC = DropView( vT );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZTEUPDD_HelpAbout
// PURPOSE:    This Entry does the About
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_HelpAbout( zVIEW vSubtask )
{
   IssueToolAbout( vSubtask, zTOOL_DTE, __FILE__, __DATE__, __TIME__ );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZTEUPDD_HelpBasics
// PURPOSE:    This Entry transfers to basic help
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZTEUPDD_HelpOnHelp
// PURPOSE:    This Entry transfers to HelpOnHelp
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZTEUPDD_HelpContents
// PURPOSE:    This Entry transfers to help Contents
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SaveModelDTE( zVIEW  vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vDTE0;
   zVIEW    vEMD;
   zVIEW    vTE_Work;
   zCHAR    szER_UpdateFlag[ 2 ];
   zSHORT   nRC;

   // Get Access to Enterprise Model Object
   // Use a work view so as not to modify position on TE_DBMS_Source.
   nRC = GetViewByName( &vDTE0, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "Unable to Access Physical Model. "
                "Model must be opened to Save." );
      MessageSend( vSubtask, "TE00401", "Physical Data Model",
                   szWorkMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   if ( zwTZTEUPDD_CloseTablRecordWindow( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      return( -1 );
   }

   CreateViewFromViewForTask( &vDTE, vDTE0, 0 );

   // Make sure object is not empty
   nRC = CheckExistenceOfEntity( vDTE, "TE_DB_Environ" );
   if (  nRC < zCURSOR_SET )
   {
      return( -1 );
   }

   // Sort Attributes in original sequence
   oTZTENVRO_SortFields( vDTE, vSubtask );

   nRC = CommitMetaOI( vSubtask, vDTE, zSOURCE_DTE_META );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "TE00420", "Physical Data Model",
                   "Unable to save Physical Environment.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   else
   {
      // save XDM here in the future..................
   }
   DropView( vDTE );

   // Save the ER if it was updated as a part of Identifier or Relationship
   // Syncronization.
   nRC = GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      GetStringFromAttribute( szER_UpdateFlag, sizeof( szER_UpdateFlag ), vTE_Work, "TE_Work", "ER_UpdateFlag" );
      if ( szER_UpdateFlag[ 0 ] == 'Y' )
      {
         // Get the ERD View
         nRC = GetViewByName( &vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );
         CommitMetaOI( vSubtask, vEMD, zSOURCE_ERD_META );
         DropView (vEMD);
      }
      DropView (vTE_Work);
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ExitSaveModelDTE(    zVIEW    vSubtask )
{
   zVIEW    vDTE;
   zSHORT   nRC;

   // Get Access to current Enterprise Model Object and prompt for Save
   nRC = GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      if ( zwTZTEUPDD_CloseTablRecordWindow( vSubtask ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
         return( -1 );
      }

      if ( MiGetUpdateForView( vDTE ) == 1  &&
           ObjectInstanceUpdatedFromFile( vDTE ) == 1 )
      {
         nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_DTE,
                             zREFER_DTE_META,
                             vDTE, "TE_DB_Environ", "Name", 0 );
         if ( nRC == zRESPONSE_CANCEL )
         {
            SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, "TZTEUPDD", "Main" );
            return( 0 );
         }
         else
         {
            if ( nRC == zRESPONSE_YES )
            {
               nRC = zwTZTEUPDD_SaveModelDTE( vSubtask );
               if ( nRC < 0 )
               {
                  SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "TZTEUPDD", "Main" );
                  return( -1 );
               }
            }
         }
      }
   }

   nRC = TerminateLPLR( vSubtask );

   return( 0 );
}

/*
   This function finds the ZKey of the ER attribute that corresponds to the
   current physical key (TE_FieldDataRel_PK).
   We pass in two views for performance reasons.  This way we don't have to
   create/drop a new view everytime we are called.
   NOTE: This function changes the cursor positions of vDTE.
*/
zLONG LOCALOPER
fnFindER_ZKeyForPhysicalKey( zVIEW vDTE_Orig, zVIEW vDTE )
{
   zLONG   lZKey = -1;
   zSHORT  nRC;

   SetViewFromView( vDTE, vDTE_Orig );

   // Keep looping until we find the ER attribute.
   do
   {
      // Get the ZKey of the physical key.
      GetIntegerFromAttribute( &lZKey, vDTE, "TE_FieldDataRel_PK", "ZKey" );

      // Now find the Field with the corresponding ZKey.
      nRC = SetCursorFirstEntityByInteger( vDTE, "TE_FieldDataRel", "ZKey",
                                           lZKey, "TE_DBMS_Source" );

   } while ( nRC >= zCURSOR_SET &&
             CheckExistenceOfEntity( vDTE, "ER_Attribute" ) < zCURSOR_SET );

   if ( nRC >= zCURSOR_SET )
      GetIntegerFromAttribute( &lZKey, vDTE, "ER_Attribute", "ZKey" );

   return( lZKey );
}

zSHORT LOCALOPER
fnRebuildTables( zVIEW vSubtask,
                 zVIEW vDTE,
                 zVIEW vEMD,
                 zPCHAR pszTE_EntityName )
{
   zVIEW  vDTE_Old;
   zVIEW  vDTE_OldTemp;
   zVIEW  vDTE_NewTemp;
   zSHORT nRC;

   zCHAR szSironDB_Type[ 2 ];

   // Drop Error List View
   zwTZTEUPDD_ClearErrorWindow( vSubtask );

   // First create a copy of the TE.
   ActivateOI_FromOI_ForTask( &vDTE_Old, vDTE, 0, zMULTIPLE );
   SetCursorFirstEntityByAttr( vDTE_Old, "TE_DBMS_Source", "ZKey",
                               vDTE,     "TE_DBMS_Source", "ZKey", 0 );

   CreateViewFromViewForTask( &vDTE_OldTemp, vDTE_Old, 0 );
   CreateViewFromViewForTask( &vDTE_NewTemp, vDTE, 0 );

   // Get the SironDB_Type because we only have to rebuild the tables for
   // Siron catalogs
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");

   // Run through all the tables and columns and delete them.
   // Don't delete the tables for Siron Catalogs
   if ( *szSironDB_Type != zType_VSAM )
   {
      for ( nRC = SetCursorFirstEntity( vDTE, "TE_TablRec", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDTE, "TE_TablRec", 0 ) )
         DeleteEntity( vDTE, "TE_TablRec", zREPOS_NONE );
   }

   // Rebuild the entities
   if ( oTZTENVRO_BuildTablRecsFromEMD( vSubtask, vDTE,
                                        vEMD, pszTE_EntityName ) < 0 )
   {
      DropView( vDTE_Old );
      DropView( vDTE_OldTemp );
      DropView( vDTE_NewTemp );
      return( -1 );
   }

   // Do not Rebuild relationships for Siron Catalogs
   // We don't need to reset physical information from the copy
   // for Siron catalogs
   if ( *szSironDB_Type != zType_VSAM )
   {
      // Rebuild the relationships.
      oTZTENVRO_BuildRelationsFromEMD( vSubtask, vDTE, vEMD );

      // Reset some of the physical information from the copy.
      for ( nRC = SetCursorFirstEntity( vDTE_Old, "TE_TablRec", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDTE_Old, "TE_TablRec", 0 ) )
      {
         // If the TablRec in the TE copy has the flag set and if the entity
         // is in the new TE copy physical attributes.

         if ( CheckExistenceOfEntity( vDTE_Old,
                                      "ER_Entity" ) == zCURSOR_SET &&
              SetCursorFirstEntityByAttr( vDTE, "ER_Entity", "ZKey",
                                          vDTE_Old, "ER_Entity", "ZKey",
                                          "TE_DBMS_Source" ) >= zCURSOR_SET ||
              CheckExistenceOfEntity( vDTE_Old,
                                      "ER_RelType" ) == zCURSOR_SET &&
              SetCursorFirstEntityByAttr( vDTE, "ER_RelType", "ZKey",
                                          vDTE_Old, "ER_RelType", "ZKey",
                                          "TE_DBMS_Source" ) >= zCURSOR_SET )
         {
            SetAttributeFromAttribute( vDTE, "TE_TablRec",
                                       "KeepPhysCharsFlagForAllColumns",
                                        vDTE_Old, "TE_TablRec",
                                        "KeepPhysCharsFlagForAllColumns" );

            if ( CompareAttributeToString( vDTE_Old, "TE_TablRec",
                                           "KeepPhysicalCharacteristicsFlag",
                                           "Y" ) == 0 )
            {
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "Name",
                                          vDTE_Old, "TE_TablRec", "Name" );
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "SQL_TableQual",
                                          vDTE_Old, "TE_TablRec", "SQL_TableQual" );
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "SQL_TableOwner",
                                          vDTE_Old, "TE_TablRec", "SQL_TableOwner" );
               SetAttributeFromAttribute( vDTE, "TE_TablRec", "Desc",
                                          vDTE_Old, "TE_TablRec", "Desc" );
               SetAttributeFromAttribute( vDTE, "TE_TablRec",
                                          "KeepPhysicalCharacteristicsFlag",
                                          vDTE_Old, "TE_TablRec",
                                          "KeepPhysicalCharacteristicsFlag" );
            }
         }

         // Do the same thing with the TE fields.
         for ( nRC = SetCursorFirstEntityByString( vDTE_Old, "TE_FieldDataRel",
                                                   "KeepPhysicalCharacteristicsFlag",
                                                   "Y", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByString( vDTE_Old, "TE_FieldDataRel",
                                                  "KeepPhysicalCharacteristicsFlag",
                                                  "Y", 0 ) )
         {
            // Make sure attr exists in new copy.
            if ( CheckExistenceOfEntity( vDTE_Old,
                                         "ER_Attribute" ) == zCURSOR_SET )
            {
               if ( SetCursorFirstEntityByAttr( vDTE, "ER_Attribute", "ZKey",
                                                vDTE_Old, "ER_Attribute", "ZKey",
                                                "TE_TablRec" ) < zCURSOR_SET )
               {
                  // This ER_Attribute no longer exists so skip it.
                  continue;
               }
            } // if ER_Attribute exists...
            else
            {
               zBOOL bFound = FALSE;
               zLONG lOldZKey;

               // Get the ZKey of the originating ER Attribute.
               lOldZKey = fnFindER_ZKeyForPhysicalKey( vDTE_Old, vDTE_OldTemp );

// KJS 10/16/14 - I get an error here when I rebuild tables/rels for ZENCAS with "Keep Physical" flag set.
// It's on DOMAINVALUE and the first attribute (TE_FieldDataRel) is DOMAINVALUE_TOKEN which has no
// ER_Attribute or ER_RelLink. I wouldn't think I would get an error becuase it's SetCursorFirst...
// Not exactly sure what to do, so I am putting in this comment.


               // We have to find the FK in the new TE that matches the old
               // TE in both relationship and originating parent key.
               for ( nRC = SetCursorFirstEntityByAttr( vDTE, "ER_RelLink", "ZKey",
                                                       vDTE_Old, "ER_RelLink",
                                                       "ZKey",
                                                       "TE_TablRec" );
                     nRC >= zCURSOR_SET;
                     nRC = SetCursorNextEntityByAttr( vDTE, "ER_RelLink", "ZKey",
                                                      vDTE_Old, "ER_RelLink",
                                                      "ZKey",
                                                      "TE_TablRec" ) )
               {
                  // Check to see if the ZKey for the ER attribute in the new
                  // TE matches the one for the old TE.  If they do then we've
                  // found our man.
                  if ( fnFindER_ZKeyForPhysicalKey( vDTE,
                                                    vDTE_NewTemp ) == lOldZKey )
                  {
                     bFound = TRUE;
                     break;
                  }
               }

               if ( !bFound )
                  continue; // This Relationshiop no longer exists so skip it.
            }

            // Copy physical information.
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Name",
                                       vDTE_Old, "TE_FieldDataRel", "Name" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Desc",
                                       vDTE_Old, "TE_FieldDataRel", "Desc" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Key",
                                       vDTE_Old, "TE_FieldDataRel", "Key" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_SCALE",
                                       vDTE_Old, "TE_FieldDataRel", "SQL_SCALE" );
            // The following statement was removed by DonC on 12/10/2014 as null flag shouldn't be kept.
            //SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_NULLS",
            //                           vDTE_Old, "TE_FieldDataRel", "SQL_NULLS" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_LABEL",
                                       vDTE_Old, "TE_FieldDataRel", "SQL_LABEL" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "SQL_REMARKS",
                                       vDTE_Old, "TE_FieldDataRel", "SQL_REMARKS" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel",
                                       "KeepPhysicalCharacteristicsFlag",
                                       vDTE_Old, "TE_FieldDataRel",
                                       "KeepPhysicalCharacteristicsFlag" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "DataType",
                                       vDTE_Old, "TE_FieldDataRel", "DataType" );
            SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "Length",
                                       vDTE_Old, "TE_FieldDataRel", "Length" );
         }
      }
   }

   DropView( vDTE_Old );
   DropView( vDTE_OldTemp );
   DropView( vDTE_NewTemp );

   // if field sequence does not exist, set it
   oTZTENVRO_SetFieldSequence( vDTE );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_RebuildTablesRels( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vEMD;
   zVIEW    vErrorList;
   zVIEW    vWindow;
   zPCHAR   szDS;
   zSHORT   nRC;

   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY ) < 0 )
      return( -1 );

   if ( zwTZTEUPDD_CloseTablRecordWindow( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      return( -1 );
   }

   SetNameForView( vDTE, "vDTE_Init", vSubtask, zLEVEL_TASK );

   if ( oTZEREMDO_GetRefViewForER( vSubtask, &vEMD, zCURRENT_OI ) < 0 )
      return( -1 );

   // We can't init table recs with a Remote Server DBMS.
   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "Paradigm",
                                  "S" ) == 0 )
   {
      MessageSend( vSubtask, "TE00423", "Physical Data Model",
                   "Can't add Table Records for Remote Servers",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   if ( CountEntitiesForView( vDTE, "TE_DBMS_Source" ) > 1 )
   {
      GetAddrForAttribute( &szDS, vDTE, "TE_DBMS_Source", "Name" );
      strcpy_s( sz, sizeof( sz ), "Do you want to initialize Data Source '" );
      strcat_s( sz, sizeof( sz ), szDS );
      strcat_s( sz, sizeof( sz ), "' with all Entities in the ER Model that:" );
      strcat_s( sz, sizeof( sz ), "\n\tare NOT in ANY Data Source" );
      strcat_s( sz, sizeof( sz ), "\n\t\t -- OR -- " );
      strcat_s( sz, sizeof( sz ), "\n\tare NOT in '" );
      strcat_s( sz, sizeof( sz ), szDS );
      strcat_s( sz, sizeof( sz ), "'?" );
      strcat_s( sz, sizeof( sz ), "\n\n" );
      strcat_s( sz, sizeof( sz ), "\tNot in Any DS ---- [ YES ]\n" );
      strcat_s( sz, sizeof( sz ), "\tNot in '" );
      strcat_s( sz, sizeof( sz ), szDS );
      strcat_s( sz, sizeof( sz ), "' --- [ NO ]\n" );
      nRC = MessagePrompt( vDTE, "TE00411", "Physical Data Model",
                           sz,
                           zBEEP,         zBUTTONS_YESNOCANCEL,
                           zRESPONSE_YES, zICON_QUESTION );
      if ( nRC == zRESPONSE_YES )
         fnRebuildTables( vSubtask, vDTE, vEMD, "TE_DB_Environ" );
      else
      {
         if ( nRC == zRESPONSE_CANCEL )
         {
            SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError,
                                    0, 0 );
            return( 0 );
         }
         else
            fnRebuildTables( vSubtask, vDTE, vEMD, "TE_DBMS_Source" );
      }
   }
   else
      fnRebuildTables( vSubtask, vDTE, vEMD, "TE_DB_Environ" );

   // Send Error List Window
   if ( GetViewByName( &vErrorList, "TZTEERR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      RefreshWindow( vSubtask );    // Refresh Main Window
      SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow,
                               "TZTEUPDD", "ErrorList" );
      GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ),
      RefreshWindow( vWindow );    // Refresh Error List
   }

   return( 0 );
}

typedef zSHORT (POPERATION zFARPROC_VVPLV)( zVIEW, zVIEW, zPCHAR, zLONG, zVIEW );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_GenerateDDL( zVIEW vSubtask )
{
   zVIEW     vTZTEDBLO;
   zVIEW     vDTE;
   zVIEW     vEMD;
   LPLIBRARY hLibrary;
   zFARPROC_VVPLV pfn;
// zSHORT    (POPERATION pfn)( zVIEW, zVIEW, zPCHAR, zLONG, zVIEW );
   zCHAR     szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR     szTempFileName[ zMAX_FILESPEC_LTH + 1 ];

   // vTZTEDBLO not currently used but some day it should be.
   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );
   oTZEREMDO_GetRefViewForER( vSubtask, &vEMD, zCURRENT_OI );

   if ( !vEMD || !vDTE || !vTZTEDBLO )
      return( 0 );

   GetStringFromAttribute( szFileName, sizeof( szFileName ), vDTE, "TE_DBMS_Source", "GenerateExecutable" );
   hLibrary = SysLoadLibrary( vSubtask, szFileName );
   if ( hLibrary )
   {
      pfn = (zFARPROC_VVPLV) SysGetProc( hLibrary, "BuildDDL" );
      if ( pfn )
      {
         zVIEW vTaskLPLR;

         // Sort Attributes in original sequence
         oTZTENVRO_SortFields( vDTE, vSubtask );

         GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
         GetStringFromAttribute( szTempFileName, sizeof( szTempFileName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
         SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szTempFileName );
         SysAppendcDirSep( szFileName );
         (*pfn) ( vDTE, vEMD, szFileName, sizeof( szFileName ), vSubtask );
      }
      else
         MessageSend( vSubtask, "TE00422", "Physical Data Model",
                      "Couldn't find 'BuildDDL' in Generater Executable",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      SysFreeLibrary( vSubtask, hLibrary );
   }
   else
      MessageSend( vSubtask, "TE00421", "Physical Data Model",
                   "Couldn't load Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   return( 0 );
}

typedef zSHORT (POPERATION zFARPROC_VVVPLV)( zVIEW, zVIEW, zVIEW, zPCHAR, zLONG, zVIEW );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_GenerateSyncDDL( zVIEW vSubtask )
{
   zVIEW     vTZTEDBLO;
   zVIEW     vDTE;
   zVIEW     vEMD;
   LPLIBRARY hLibrary;
   zFARPROC_VVVPLV pfn;
// zSHORT    (POPERATION pfn)( zVIEW, zVIEW, zVIEW, zPCHAR, zLONG, zVIEW );
   zCHAR     szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR     szTempFileName[ zMAX_FILESPEC_LTH + 1 ];

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );
   oTZEREMDO_GetRefViewForER( vSubtask, &vEMD, zCURRENT_OI );

   if ( !vEMD || !vDTE || !vTZTEDBLO )
      return( 0 );

   GetStringFromAttribute( szFileName, sizeof( szFileName ),
                           vDTE, "TE_DBMS_Source", "GenerateExecutable" );
   hLibrary = SysLoadLibrary( vSubtask, szFileName );
   if ( hLibrary )
   {
      pfn = (zFARPROC_VVVPLV) SysGetProc( hLibrary, "BuildSyncDDL" );
      if ( pfn )
      {
         zVIEW vTaskLPLR;

         GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
         GetStringFromAttribute( szTempFileName, sizeof( szTempFileName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
         SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szTempFileName );
         SysAppendcDirSep( szFileName );
         (*pfn)( vDTE, vEMD, vTZTEDBLO, szFileName, sizeof( szFileName ), vSubtask );
      }
      else
         MessageSend( vSubtask, "TE00422", "Physical Data Model",
                      "Couldn't find 'BuildSyncDDL' in Generater Executable",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      SysFreeLibrary( vSubtask, hLibrary );
   }
   else
      MessageSend( vSubtask, "TE00421", "Physical Data Model",
                   "Couldn't load Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   return( 0 );
}

typedef zSHORT (POPERATION zFARPROC_VV)( zVIEW, zVIEW );

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ImportDB_Data( zVIEW vSubtask )
{
   zVIEW     vTZTEDBLO;
   zVIEW     vDTE;
   zVIEW     vEMD;
   LPLIBRARY hLibrary;
   zFARPROC_VV pfn;
// zSHORT    (POPERATION pfn)( zVIEW, zVIEW );
   zCHAR     szFileName[  zMAX_FILENAME_LTH + 1 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   // This function is only supported for Raima DB.
   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS",
                                  "DB Raima" ) != 0 )
   {
      MessageSend( vSubtask, "TE00431", "Physical Data Model",
                   "This function is currently valid only for DB Raima.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   // vTZTEDBLO not currently used but some day it should be.
   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   oTZEREMDO_GetRefViewForER( vSubtask, &vEMD, zCURRENT_OI );

   if ( !vEMD || !vDTE || !vTZTEDBLO )
      return( 0 );

   GetStringFromAttribute( szFileName, sizeof( szFileName ),
                           vDTE, "TE_DBMS_Source", "GenerateExecutable" );
   hLibrary = SysLoadLibrary( vSubtask, szFileName );
   if ( hLibrary )
   {
      pfn = (zFARPROC_VV) SysGetProc( hLibrary, "ImportDB_Data" );
      if ( pfn )
         (*pfn) ( vDTE, vEMD );
      else
         MessageSend( vSubtask, "TE00422", "Physical Data Model",
                      "Couldn't find 'ImportDB_Data' in Generater Executable",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      SysFreeLibrary( vSubtask, hLibrary );
   }
   else
      MessageSend( vSubtask, "TE00421", "Physical Data Model",
                   "Couldn't load Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   return( 0 );

} // zwTZTEUPDD_ImportDB_Data

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SortDBs( zVIEW    vSubtask )
{
   zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   OrderEntityForView( vDTE, "TE_DBMS_Source", "Name A" );

   return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SortTRsByName( zVIEW    vSubtask )
{
zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   OrderEntityForView( vDTE, "TE_TablRec", "Name A" );

   return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SortTRsByDesc( zVIEW    vSubtask )
{
zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   OrderEntityForView( vDTE, "TE_TablRec", "Desc A" );

   return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SortTRsByOwner( zVIEW    vSubtask )
{
zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   OrderEntityForView( vDTE, "TE_TablRec", "SQL_TableOwner A Name A" );

   return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SortByName( zVIEW    vSubtask )
{
zVIEW  vT;
zSHORT nRC;

   nRC = GetViewByName( &vT, "CM_DTE_List", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zASCENDING, vT,
                                   "W_MetaDef", "Name", "" );
   nRC = SetCursorFirstSelectedEntity( vT, "W_MetaDef", "" );
   if ( nRC != zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vT, "W_MetaDef", "" );
      nRC = SetSelectStateOfEntityForSet( vT, "W_MetaDef", 1, 1 );
   }

   return( 0 );

}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SortByDate( zVIEW    vSubtask )
{
zVIEW  vT;
zSHORT nRC;

   nRC = GetViewByName( &vT, "CM_DTE_List", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zDESCENDING, vT,
                                   "W_MetaDef", "LastUpdateDate", "" );
   nRC = SetCursorFirstSelectedEntity( vT, "W_MetaDef", "" );
   if ( nRC != zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vT, "W_MetaDef", "" );
      nRC = SetSelectStateOfEntityForSet( vT, "W_MetaDef", 1, 1 );
   }

   return( 0 );

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteAllTablRecs
//
// This function deletes all entity - entries in TE
// For Siron Catalogs it starts the reconstruction of the ER-Attributes.
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DeleteAllTablRecs( zVIEW    vSubtask )
{
   zVIEW    vDTE;
   zSHORT   nZRetCode;
   zSHORT   nRC;
   zCHAR    szSironDB_Type[ 2 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   if ( zwTZTEUPDD_CloseTablRecordWindow( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      return( -1 );
   }

   // We need the DB-Type because Siron Catalogs must be handled in a special way
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
   if ( *szSironDB_Type == zType_VSAM )
      return( zwTZTEUPDD_WindowDeleteVsam (vSubtask, 1));

   // No Siron catalogs than any other database
   strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "OK to delete all Tables/Records "
             "from the Data Source" );
   nRC = MessagePrompt( vSubtask, "TE00414", "Physical Data Model",
                        szWorkMessage,
                        zBEEP,        zBUTTONS_YESNO,
                        zRESPONSE_NO, zICON_QUESTION );

   if ( nRC != zRESPONSE_YES )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   /* Delete all tables from the DataSource */
   nZRetCode = SetCursorFirstEntity( vDTE, "TE_TablRec", "" );
   while ( nZRetCode >= zCURSOR_SET )
   {
      nZRetCode = DeleteEntity( vDTE, "TE_TablRec", zREPOS_NONE );
      nZRetCode = SetCursorFirstEntity( vDTE, "TE_TablRec", "" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZTEUPDD_DataSrcPopup
// PURPOSE:   Show Popup Menu for DataSrc ListBox
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DataSrcPopup( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vWindow;
   zSHORT nEnable;
   zSHORT IsCheckedOut = 0;
   zCHAR  szSironDB_Type[ 2 ];
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      return( 0 );

   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( CheckExistenceOfEntity( vDTE, "TE_DBMS_Source" ) >= zCURSOR_SET )
   {
      // Disable the Gen DDL option if the DBMS Source is network or if
      // there is no DBMS Source.
      if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS",
                                     "Network Server" ) == 0 ||
           CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS",
                                     "" ) == 0 ||
           CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS",
                                     "None" ) == 0 )
      {
         nEnable = 0;
      }
      else
      {
         GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
         if ( *szSironDB_Type == zType_VSAM || *szSironDB_Type == zType_SAP )
            nEnable = 0;
         else
         if ( CheckExistenceOfEntity( vDTE, "TE_TablRec" ) >= zCURSOR_SET )
            nEnable = 1;
         else
            nEnable = 0;
      }
      if ( ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ) == 1 )
         IsCheckedOut = 1;
   }
   else
   {
      nEnable = 0;
   }

   EnableAction( vSubtask, "GenerateDDL", nEnable );
   EnableAction( vSubtask, "DataSrcDelete", IsCheckedOut );

   CreateTrackingPopupMenu( vSubtask, "DataSrc", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_TableRecPopup( zVIEW vSubtask )
{
   zwTZTEUPDD_LoadPopup( vSubtask, "TE_TablRec", "", "TableDelete", "TableRec" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_KeyPopup( zVIEW vSubtask )
{
   zwTZTEUPDD_LoadPopup( vSubtask, "TE_TablRecKey", "KeyAdd", "KeyDelete", "Key" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ColumnFieldPopup( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zCHAR  szSironDB_Type[ 2 ];
   zBOOL  bSetTableKeepPhysChars = FALSE;

   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");

      if ( *szSironDB_Type != zType_VSAM &&
           *szSironDB_Type != zType_SAP &&
           ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ) == 1 &&
           CheckExistenceOfEntity( vDTE, "TE_FieldDataRel" ) >= zCURSOR_SET )
      {
         bSetTableKeepPhysChars = TRUE;
      }
   }

   EnableAction( vSubtask, "SetFieldKeepPhysChars", bSetTableKeepPhysChars );

   zwTZTEUPDD_LoadPopup( vSubtask, "TE_FieldDataRel", "", "FieldDelete", "ColumnField" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: zwTZTEUPDD_DataSrcAssignZKey
// PURPOSE:   Set ZKey after automatic Create Temporal Entity
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DataSrcAssignZKey( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( vDTE &&
        CheckExistenceOfEntity( vDTE, "TE_DBMS_Source" ) >= zCURSOR_SET )
   {
      if ( zwTZTEUPDD_CloseTablRecordWindow( vSubtask ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
         return( -1 );
      }

      if ( zwTZTEUPDD_AcceptSubobject( vSubtask,
                                       "TE_DBMS_Source",
                                       "Data Source",
                                       1 ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vDTE, "TE_DBMS_Source", zPOS_AFTER );
   zwTZTEUPDD_DataSrc_Prebuild( vSubtask );
   zwTZTEUPDD_DS_TemporaryGraying( vSubtask );

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SetParadigmFromDBMS( zVIEW vSubtask )
{
   zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( vDTE )
   {
      if ( CompareAttributeToString ( vDTE, "TE_DBMS_Source", "DBMS", "B" ) == 0 ||
           CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS", "M" ) == 0 )
      {
         SetAttributeFromString( vDTE, "TE_DBMS_Source", "Paradigm", "R" );
      }
      else
      if ( CompareAttributeToString ( vDTE, "TE_DBMS_Source", "DBMS", "V" ) == 0 )
      {
         SetAttributeFromString( vDTE, "TE_DBMS_Source", "Paradigm", "N" );
      }

      if ( zwTZTEUPDD_AcceptSubobject( vSubtask,
                                       "TE_DBMS_Source",
                                       "Data Source",
                                       1 ) < 0 )
      {
         return( -1 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
DisplayDBH_MainDialog( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vTZTEDBLO;
   zCHAR  szDialogName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  szWindowName[ zZEIDON_NAME_LTH + 1 ];

   GetViewByName( &vDTE,      "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZTEDBLO, "TZTEDBLO",      vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByString( vTZTEDBLO, "DBS_SubDialog", "Type",
                                 "TE_Main", 0 );

   GetStringFromAttribute( szDialogName, sizeof( szDialogName ), vTZTEDBLO, "DBS_SubDialog", "Dialog" );
   GetStringFromAttribute( szWindowName, sizeof( szWindowName ), vTZTEDBLO, "DBS_SubDialog", "Window" );

   SetWindowActionBehavior( vSubtask,
                            zWAB_StartModalSubwindow + zWAB_ProcessImmediateAction,
                            szDialogName, szWindowName );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DS_TemporaryGraying( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vTZTEDBLO;
   zSHORT nEnable = 0;
   zCHAR  szSironDB_Type[ 2 ];

   GetViewByName( &vDTE,      "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZTEDBLO, "TZTEDBLO",      vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "DisplayOwner", "Y" ) == 0 )
      SetCtrlState( vSubtask, "DefaultOwner", zCONTROL_STATUS_ENABLED, 1 );
   else
      SetCtrlState( vSubtask, "DefaultOwner", zCONTROL_STATUS_ENABLED, 0 );

   // Check to see if the DBMS type has a subdialog for setting DBH data.
   if ( SetCursorFirstEntityByString( vTZTEDBLO, "DBS_SubDialog", "Type",
                                      "TE_Main", 0 ) >= zCURSOR_SET &&
         ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ) == 1 )
   {
      nEnable = 1;
   }

   SetCtrlState( vSubtask, "SetDBH", zCONTROL_STATUS_ENABLED, nEnable );

   // Check if the DBHandler Typ is Siron Flat Files or SAP. Then we can't
   // change the DBHandler
   nEnable = 0;
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type" );
   if ( (*szSironDB_Type != zType_VSAM &&
         *szSironDB_Type != zType_SAP) &&
         ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ) == 1 )
   {
      nEnable = 1;
   }

   SetCtrlState( vSubtask, "cbDBHandler", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "KeepPhysChar", zCONTROL_STATUS_ENABLED, nEnable );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_KeyAssignZKey( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vWindow;

   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( CheckExistenceOfEntity( vDTE, "TE_TablRecKey" ) >= zCURSOR_SET )
      {
         if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_TablRecKey",
                                          "Key", 1 ) < 0 )
         {
            return( -1 );
         }

         SetSelectStateOfEntity( vDTE, "TE_TablRecKey", 0 );
      }

      CreateTemporalMetaEntity( vSubtask, vDTE, "TE_TablRecKey", zPOS_AFTER );

      SetSelectStateOfEntity( vDTE, "TE_TablRecKey", 1 );
      zwTZTEUPDD_KeyListPostbuild( vSubtask );

      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" ) >= 0 )
         RefreshWindow( vWindow );

      SetFocusToCtrl( vSubtask, "edName" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DisableKeyWndForSAP
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_DisableKeyWndForSAP( zVIEW vSubtask )
{
   SetCtrlState( vSubtask, "edName", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Category", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "edIndexName", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "New", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
} // zwTZTEUPDD_DisableKeyWndForSAP

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_KeyListPostbuild( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zSHORT   nRC;
   zCHAR    szSironDB_Type[ 2 ];
   zBOOL    nEnable = FALSE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");

   LB_SetDisplaySelectSet( vSubtask, "TableFieldList", 2 );
   SetAllSelStatesForEntityForSet( vDTE, "TE_FieldDataRel", 1, 2, "");

   nRC = SetCursorFirstEntity( vDTE, "TE_FieldDataRelKey", "" );
   while ( nRC == zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByEntityCsr( vDTE, "TE_FieldDataRel",
                                             vDTE, "TE_FieldDataRelKey", "" );
      if ( nRC >= zCURSOR_SET )
      {
         nRC = SetSelectStateOfEntityForSet( vDTE, "TE_FieldDataRel", 0, 2 );
      }

      nRC = SetCursorNextEntity( vDTE, "TE_FieldDataRelKey", "" );
   }
   nRC = SetCursorFirstEntity( vDTE, "TE_FieldDataRelKey", "" );

   SetSelectSetForView( vDTE, 2 );
   nRC = SetCursorFirstSelectedEntity( vDTE, "TE_FieldDataRel", "" );
   SetSelectSetForView( vDTE, 1 );

   if ( ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ) &&
        *szSironDB_Type != zType_SAP )
   {
      nEnable = TRUE;
   }

   EnableAction( vSubtask, "KeyListInclude", nEnable );
   EnableAction( vSubtask, "KeyListExclude", nEnable );

   // Disable the New Key Button if the DBMS Source is NONE
   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source",
                                  "DBMS", "" ) == 0 ||
        CompareAttributeToString( vDTE, "TE_DBMS_Source",
                                  "DBMS", "None" ) == 0 ||
        !ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ) )
   {
      SetCtrlState( vSubtask, "New", zCONTROL_STATUS_ENABLED, 0 );
   }
   else
   {
      SetCtrlState( vSubtask, "New", zCONTROL_STATUS_ENABLED, 1 );
   }

   // if Data Source a SAP Data Source, disable all Controls
   if ( *szSironDB_Type == zType_SAP )
      zwTZTEUPDD_DisableKeyWndForSAP( vSubtask );

   zwTZTEUPDD_SaveViewCursor( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_KeyListInclude( zVIEW vSubtask )
{
   zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   if ( GetSelectStateOfEntityForSet( vDTE, "TE_FieldDataRel", 1 ) )
   {
      IncludeSubobjectFromSubobject( vDTE, "TE_FieldDataRelKey",
                                     vDTE, "TE_FieldDataRel", zPOS_AFTER );
      SetSelectStateOfEntityForSet( vDTE, "TE_FieldDataRel", 0, 2 );
      SetSelectStateOfEntityForSet( vDTE, "TE_FieldDataRel", 0, 1 );
      SetSelectSetForView( vDTE, 2 );
      if ( SetCursorNextSelectedEntity( vDTE, "TE_FieldDataRel", 0 ) < zCURSOR_SET )
      {
         SetEntityCursor( vDTE, "TE_FieldDataRel", 0,
                          zPOS_PREV | zQUAL_SELECTED, 0, 0, 0, 0, 0, 0 );
      }

      SetSelectSetForView( vDTE, 1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_KeyListExclude( zVIEW vSubtask )
{
   zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   if ( GetSelectStateOfEntityForSet( vDTE, "TE_FieldDataRelKey", 1 ) )
   {
      if ( GetSelectStateOfEntityForSet( vDTE, "TE_FieldDataRelKey", 1 ) )
      {
         SetCursorFirstEntityByEntityCsr( vDTE, "TE_FieldDataRel",
                                          vDTE, "TE_FieldDataRelKey", "" );
         SetSelectStateOfEntityForSet( vDTE, "TE_FieldDataRel", 1, 2 );
         ExcludeEntity( vDTE, "TE_FieldDataRelKey", zREPOS_NEXT );
      }
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SelectDBMS( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vTZTEDBLO;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "Paradigm",
                              vTZTEDBLO, "TE_DBMS_Source", "Paradigm" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "DBMS",
                              vTZTEDBLO, "TE_DBMS_Source", "DBMS" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "GenerateExecutable",
                              vTZTEDBLO, "TE_DBMS_Source", "GenerateExecutable" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "ConectionType",
                              vTZTEDBLO, "TE_DBMS_Source", "ConectionType" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "SironDB_Type",
                              vTZTEDBLO, "TE_DBMS_Source", "SironDB_Type" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "SironDB_Subtype",
                              vTZTEDBLO, "TE_DBMS_Source", "SironDB_Subtype" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "Executable",
                              vTZTEDBLO, "TE_DBMS_Source", "Executable" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "DisplayOwner",
                              vTZTEDBLO, "TE_DBMS_Source", "DisplayOwner" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "DisplayDBMS_ID",
                              vTZTEDBLO, "TE_DBMS_Source", "DisplayDBMS_ID" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "Network",
                              vTZTEDBLO, "TE_DBMS_Source", "Network" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "GenKeyExecutable",
                              vTZTEDBLO, "TE_DBMS_Source", "GenKeyExecutable" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "MaxFixedAttrLth",
                              vTZTEDBLO, "TE_DBMS_Source", "MaxFixedAttrLth" );
   SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "OverflowAttrLth",
                              vTZTEDBLO, "TE_DBMS_Source", "OverflowAttrLth" );

   // Call oper to gray/enable stuff.
   zwTZTEUPDD_DS_TemporaryGraying( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncRelationships( zVIEW vSubtask )
{
  // The purpose of this routine is to synchronize the logical relationships of the ER with
  // the foreign keys of the TE.  This is done by looping through all ER_RelTypes in the
  // ER and having the operator specify physical relationship information as follows.

  // Each RelType is evaluated to make sure a corresponding ER_RelLink and
  // ER_RelType_O entry exists.  In not, one is built, along with relating the embedded keys
  // through the TE_FieldDataRelAsFK entity.  The routine does not check the validity of
  // TE_FieldDataRelAsFK entries that already exist.
  // The process is only started in this routine, because when a relationship is found
  // that needs processing, we must exit the routine to the window at which the operator
  // makes foreign key selection for the relationship.  The loop then continues with an
  // operation from that window.

   zVIEW    vDTE;
   zVIEW    vLPLR;
   zVIEW    vEMD;
   zSHORT   nRC;
   zLONG    lZKey;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_CloseTablRecordWindow( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      return( -1 );
   }

   // Sort Attributes in original sequence
   oTZTENVRO_SortFields( vDTE, vSubtask );

   nRC = GetViewByName( &vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vLPLR, zSOURCE_ERD_META );
      ActivateMetaOI( vSubtask, &vEMD, vLPLR, zSOURCE_ERD_META, zCURRENT_OI );
      SetNameForView( vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );
      DropView( vLPLR );
   }

   // Start the loop to sync the relationships.  Whenever we find a relationship that needs
   // to be processed, we will break this routine to go to the window for which the operator will
   // select the correct foreign keys for the relationship.
   // If we don't find such a relationship, we will return a message to the operator and stay
   // on the current window.

   nRC = SetCursorFirstEntity( vEMD, "ER_RelType", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vEMD, "ER_RelType", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vDTE, "ER_RelType_O", "ZKey",
                                           lZKey, "TE_DBMS_Source" );
      if ( nRC < zCURSOR_SET )
      {
         // Once we find a relationship to be processed, we just terminate to
         // go to the sync window for processing.
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                  "TZTEUPDD", "SyncRels" );
         return( 0 );
      }
      nRC = SetCursorNextEntity( vEMD, "ER_RelType", 0 );
   }

   MessageSend( vSubtask, "TE00415", "Physical Data Model",
                "No Relationships need to be syncronized.",
                zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncPreBuild( zVIEW vSubtask )
{
   // Build the work object TE_Work.  We will use the SyncInitWorkObject to do
   // most of the processing.  Here, we will only position on the correct ER_Entity
   // when one of the entities has a Max Cardinality > 1, which means it is the one
   // containing the foreign keys.
   // Note that we are ignoring many-to-many relationships at this time.

   zVIEW    vDTE;
   zVIEW    vEMD;
   zSHORT   nRC;
   zCHAR    szMaxCardinality[ 9 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );

   // If the first ER_RelLink_2 does not have a max cardinality of 1,
   // position on the other.  This seems the opposite to what is intuitive,
   // but the maximum cardinality of ER_RelLink_2 is in the direction of the
   // ER_Entity to the ER_RelLink_2.
   // Thus for a one-to-many relationship of: Test1 "has mult" Test2, the
   // max cardinality of ER_RelLink_2 for the Test1 entity is 1.

   nRC = SetCursorFirstEntity( vEMD, "ER_RelLink_2", 0 );
   GetStringFromAttribute( szMaxCardinality, sizeof( szMaxCardinality ),
                           vEMD, "ER_RelLink_2", "CardMax" );

   if ( szMaxCardinality[ 0 ] != '1' )
      SetCursorNextEntity( vEMD, "ER_RelLink_2", 0 );

   zwTZTEUPDD_SyncInitWorkObject( vSubtask, vDTE, vEMD );

   return( 0 );
}

zSHORT  /* LOCAL */  OPERATION
zwTZTEUPDD_SyncInitWorkObject( zVIEW vSubtask,
                               zVIEW vDTE,
                               zVIEW vEMD )
{
   // Build the TE work object.

   zVIEW    vTE_Work;
   zVIEW    vDTE2;
   zVIEW    vEMD2;
   zSHORT   nRC;
   zLONG    lZKey;
   zCHAR    szFieldType[ 2 ];
   zCHAR    szTargetEntityName[ 33 ];
   zCHAR    szRelationshipName[ 33 ];
   zCHAR    szRelationshipText[ 254 ];

   // Use a work view of vDTE so as not to modify vDTE's position.
   CreateViewFromViewForTask( &vDTE2, vDTE, 0 );
   SetNameForView( vDTE2, "vDTE2", vSubtask, zLEVEL_TASK );

   // Initialize TE_Work object if it doesn't already exist.
   if ( GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vTE_Work, "TZTEWRKO", vDTE, zSINGLE );
      SetNameForView( vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );
      CreateEntity( vTE_Work, "TE_Work", zPOS_AFTER );
   }
   else
   {
      // Clean it up, if it already exists.
      nRC = SetCursorFirstEntity( vTE_Work, "TargetKey", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         ExcludeEntity( vTE_Work, "TargetKey", zREPOS_NONE );
         nRC = SetCursorNextEntity( vTE_Work, "TargetKey", 0 );
      }
      nRC = SetCursorFirstEntity( vTE_Work, "CandidateTargetKey", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         ExcludeEntity( vTE_Work, "CandidateTargetKey", zREPOS_NONE );
         nRC = SetCursorNextEntity( vTE_Work, "CandidateTargetKey", 0 );
      }
      nRC = SetCursorFirstEntity( vTE_Work, "CandidateSourceKey", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         ExcludeEntity( vTE_Work, "CandidateSourceKey", zREPOS_NONE );
         nRC = SetCursorNextEntity( vTE_Work, "CandidateSourceKey", 0 );
      }
   }

   // Save the target and source ZKey values, along with the relationship text..
   SetAttributeFromAttribute( vTE_Work, "TE_Work", "TargetER_EntityZKey",
                              vEMD, "ER_Entity_2", "ZKey" );
   GetStringFromAttribute( szTargetEntityName, sizeof( szTargetEntityName ), vEMD, "ER_Entity_2", "Name" );
   CreateViewFromViewForTask( &vEMD2, vEMD, 0 );
   nRC = SetCursorPrevEntity( vEMD2, "ER_RelLink_2", 0 );
   if ( nRC < zCURSOR_SET )
      SetCursorNextEntity( vEMD2, "ER_RelLink_2", 0 );
   SetAttributeFromAttribute( vTE_Work, "TE_Work", "SourceER_EntityZKey",
                              vEMD2, "ER_Entity_2", "ZKey" );
   GetStringFromAttribute( szRelationshipName, sizeof( szRelationshipName ), vEMD2, "ER_RelLink_2", "Name" );
   GetStringFromAttribute( szRelationshipText, sizeof( szRelationshipText ), vEMD2, "ER_Entity_2", "Name" );
   strcat_s( szRelationshipText, sizeof( szRelationshipText ), " - " );
   strcat_s( szRelationshipText, sizeof( szRelationshipText ), szRelationshipName );
   strcat_s( szRelationshipText, sizeof( szRelationshipText ), " - " );
   strcat_s( szRelationshipText, sizeof( szRelationshipText ), szTargetEntityName );
   SetAttributeFromString( vTE_Work, "TE_Work", "RelationshipText",
                           szRelationshipText );
   DropView( vEMD2 );

   // First build the Candidate Target foreign key entries.
   GetIntegerFromAttribute( &lZKey, vEMD, "ER_Entity_2", "ZKey" );
   SetCursorFirstEntityByInteger( vDTE2, "ER_Entity", "ZKey", lZKey, "TE_DBMS_Source" );
   nRC = SetCursorFirstEntity( vDTE2, "TE_FieldDataRel", 0 );
   while( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szFieldType, sizeof( szFieldType ), vDTE2, "TE_FieldDataRel", "DataOrRelfieldOrSet" );
      if ( szFieldType[ 0 ] == 'R' )
         IncludeSubobjectFromSubobject( vTE_Work, "CandidateTargetKey",
                                        vDTE2,    "TE_FieldDataRel", zPOS_AFTER );

      nRC = SetCursorNextEntity( vDTE2, "TE_FieldDataRel", 0 );
   }

   // Next build the Candidate Source foreign key entries.
   // Position on the other ER_Entity_2 entity
   nRC = SetCursorPrevEntity( vEMD, "ER_RelLink_2", 0 );
   if ( nRC < zCURSOR_SET)
      SetCursorNextEntity( vEMD, "ER_RelLink_2", 0 );
   GetIntegerFromAttribute( &lZKey, vEMD, "ER_Entity_2", "ZKey" );
   SetCursorFirstEntityByInteger( vDTE2, "ER_Entity", "ZKey", lZKey, "TE_DBMS_Source" );
   SetCursorFirstEntity( vDTE2, "TE_TablRecKey", 0 );
   nRC = SetCursorFirstEntity( vDTE2, "TE_FieldDataRelKey", 0 );
   while( nRC >= zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vTE_Work, "CandidateSourceKey",
                                     vDTE2,     "TE_FieldDataRelKey", zPOS_AFTER );
      nRC = SetCursorNextEntity( vDTE2, "TE_FieldDataRelKey", 0 );
   }

   DropView( vDTE2 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncPrev( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vEMD;
   zSHORT   nRC;
   zLONG    lZKey;

   //BL, 1999.11.28 read Data
   nRC = zwTZTEUPDD_SyncOK( vSubtask );
   if ( nRC )
      return( 0 );

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );

   // Don't do any includes but continue looping to the next relationship.
   // As with SyncRelationships, above, whenever we find a relationship that needs
   // to be processed, we will break this routine to go to the window for which the operator will
   // select the correct foreign keys for the relationship.
   // If we don't find such a relationship, we will return to the calling window.

   nRC = SetCursorPrevEntity( vEMD, "ER_RelType", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // We will not synchronize relationships for work entities
      while ( CompareAttributeToString(vEMD, "ER_Entity_2", "Purpose", "W") == 0 &&
              nRC >= zCURSOR_SET )
      {
         nRC = SetCursorPrevEntity( vEMD, "ER_RelType", 0 );
      }

      GetIntegerFromAttribute( &lZKey, vEMD, "ER_RelType", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vDTE, "ER_RelType_O", "ZKey",
                                           lZKey, "TE_DBMS_Source" );
      if ( nRC < zCURSOR_SET )
      {
         // Once we find a relationship to be processed, we set up the work
         // object and terminate to go to the sync window for further processing.
         zwTZTEUPDD_SyncPreBuild( vSubtask );
         //BL, 1999.11.28
         zwTZTEUPDD_DisableButton( vSubtask );
         return( 0 );
      }

      nRC = SetCursorPrevEntity( vEMD, "ER_RelType", 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncNext( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vEMD;
   zSHORT   nRC;
   zLONG    lZKey;

   //BL, 1999.11.28 read Data
   nRC = zwTZTEUPDD_SyncOK( vSubtask );
   if ( nRC )
      return( 0 );

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );

   // Don't do any includes but continue looping to the next relationship.
   // As with SyncRelationships, above, whenever we find a relationship that needs
   // to be processed, we will break this routine to go to the window for which the operator will
   // select the correct foreign keys for the relationship.
   // If we don't find such a relationship, we will return to the calling window.

   nRC = SetCursorNextEntity( vEMD, "ER_RelType", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // We will not synchronize relationships for work entities
      while ( CompareAttributeToString( vEMD, "ER_Entity_2", "Purpose", "W" ) == 0 &&
              nRC >= zCURSOR_SET )
      {
         nRC = SetCursorNextEntity( vEMD, "ER_RelType", 0 );
      }

      GetIntegerFromAttribute( &lZKey, vEMD, "ER_RelType", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vDTE, "ER_RelType_O", "ZKey",
                                           lZKey, "TE_DBMS_Source" );
      if ( nRC < zCURSOR_SET )
      {
         // Once we find a relationship to be processed, we set up the work
         // object and terminate to go to the sync window for further processing.
         zwTZTEUPDD_SyncPreBuild( vSubtask );
         //BL, 1999.11.28
         zwTZTEUPDD_DisableButton( vSubtask );
         return( 0 );
      }

      nRC = SetCursorNextEntity( vEMD, "ER_RelType", 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncSelectC_Target( zVIEW vSubtask )
{
   // Select a Candidate Target Key and make it one of the selected Target Keys.
   // Then refresh the two related list boxes.

   zVIEW    vTE_Work;

   GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );
   SetCursorLastEntity( vTE_Work, "TargetKey", 0 );
   IncludeSubobjectFromSubobject( vTE_Work, "TargetKey",
                                  vTE_Work, "CandidateTargetKey", zPOS_AFTER );
   ExcludeEntity( vTE_Work, "CandidateTargetKey", zREPOS_PREV );

   //BL, 1999.11.28
   zwTZTEUPDD_DisableButton( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncSelectC_Source( zVIEW vSubtask )
{
   // Select a Candidate Source Key and make it one of the selected Source Keys.
   // Then refresh the two related list boxes.

   zVIEW    vTE_Work;
   zSHORT   nRC;

   GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );
   nRC = SetCursorLastEntity( vTE_Work, "TargetKey", 0 );

   //BL, 1999.11.28 check exists ParentEntity( = TargetKey)
   if ( nRC >= zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vTE_Work, "SourceKey",
                                     vTE_Work, "CandidateSourceKey", zPOS_AFTER );
      ExcludeEntity( vTE_Work, "CandidateSourceKey", zREPOS_PREV );
   }

   //BL, 1999.11.28
   zwTZTEUPDD_DisableButton( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncDeselectTarget( zVIEW vSubtask )
{
   // Remove a Selected target entry and put them back to Candidate entries.

   zVIEW    vTE_Work;
   zSHORT   nRC;

   GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );

   nRC = CheckExistenceOfEntity( vTE_Work, "SourceKey" );
   if ( nRC >= zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vTE_Work, "CandidateSourceKey",
                                     vTE_Work, "SourceKey", zPOS_AFTER );
      ExcludeEntity( vTE_Work, "SourceKey", zREPOS_PREV );
   }

   nRC = CheckExistenceOfEntity( vTE_Work, "TargetKey" );
   if ( nRC >= zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vTE_Work, "CandidateTargetKey",
                                     vTE_Work, "TargetKey", zPOS_AFTER );
      ExcludeEntity( vTE_Work, "TargetKey", zREPOS_PREV );
   }

   //BL, 1999.11.28
   zwTZTEUPDD_DisableButton( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SyncOK( zVIEW vSubtask )
{
   zVIEW    vTargetDTE;
   zVIEW    vSourceDTE;
   zVIEW    vEMD;
   zVIEW    vTE_Work;
   zSHORT   nRC;
   zLONG    lZKey;

   GetViewByName( &vTargetDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );

   // First make sure that there is at least one TargetKey selected and
   // that every TargetKey has a corresponding SourceKey.
   if ( CheckExistenceOfEntity( vTE_Work, "TargetKey" ) < zCURSOR_SET )
   {
      return( 0 );
   }
   nRC = SetCursorFirstEntity( vTE_Work, "TargetKey", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vTE_Work, "SourceKey" ) < zCURSOR_SET )
      {
         MessageSend( vSubtask, "TE00418", "Physical Data Model",
                      "Target key is selected without associated source key.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
         return( -1 );
      }

      nRC = SetCursorNextEntity( vTE_Work, "TargetKey", 0 );
   }

   // Use the TE_Work object to determine what ER_RelLink and TE_FieldDataRelAsFK entries
   // to include in the DTE.  Then continue looping through additional relationships.

   // At the beginning, we position on the target entity table.  We then create
   // a second position, so that we are positioned on both target and source.
   GetIntegerFromAttribute( &lZKey, vTE_Work, "TE_Work", "TargetER_EntityZKey" );
   SetCursorFirstEntityByInteger( vTargetDTE, "ER_Entity", "ZKey",
                                  lZKey, "TE_DBMS_Source" );
   CreateViewFromViewForTask( &vSourceDTE, vTargetDTE, 0 );
   GetIntegerFromAttribute( &lZKey, vTE_Work, "TE_Work", "SourceER_EntityZKey" );
   SetCursorFirstEntityByInteger( vSourceDTE, "ER_Entity", "ZKey",
                                  lZKey, "TE_DBMS_Source" );

   // For each TarketKey/SourceKey pair, include both an ER_RelLink entry under the
   // target and a TE_FieldDataRelAsFK entry under a source, the latter pointing back to the
   // corresponding target key.
   // Also remove the corresponding ER_Attribute from the data model, because a foreign key should
   // not be visible to the logical data model.

   nRC = SetCursorFirstEntity( vTE_Work, "TargetKey", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = CheckExistenceOfEntity( vTE_Work, "SourceKey" );
      if ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vTE_Work, "TargetKey", "ZKey" );
         SetCursorFirstEntityByInteger( vTargetDTE, "TE_FieldDataRel",
                                        "ZKey", lZKey, 0 );
         IncludeSubobjectFromSubobject( vTargetDTE, "ER_RelLink",
                                        vEMD, "ER_RelLink_2", zPOS_AFTER );
         GetIntegerFromAttribute( &lZKey, vTE_Work, "SourceKey", "ZKey" );
         SetCursorFirstEntityByInteger( vSourceDTE, "TE_FieldDataRelKey", "ZKey",
                                        lZKey, "TE_TablRec" );
         IncludeSubobjectFromSubobject( vSourceDTE, "TE_FieldDataRelAsFK",
                                        vTargetDTE, "TE_FieldDataRel", zPOS_AFTER );

         // If the ER_RelLink is not already included with the target entity
         // in the ER, include it here.  But we first must position on correct
         // ER_Entity.
         GetIntegerFromAttribute( &lZKey, vTargetDTE, "ER_Entity", "ZKey" );
         SetCursorFirstEntityByInteger( vEMD, "ER_Entity", "ZKey",
                                        lZKey, 0 );
         GetIntegerFromAttribute( &lZKey, vEMD, "ER_RelLink_2", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( vEMD, "ER_RelLinkIdentifier", "ZKey",
                                              lZKey, "ER_Entity" );
         if ( nRC < zCURSOR_SET )
         {
            CreateMetaEntity( vSubtask, vEMD, "ER_FactType", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vEMD, "ER_RelLinkIdentifier",
                                           vEMD, "ER_RelLink_2", zPOS_AFTER );
            SetAttributeFromString( vTE_Work, "TE_Work", "ER_UpdateFlag", "Y" );
         }

         // For the foreign key just processed, we want to exclude the
         // ER_Attribute from the TE and delete the ER_Attribute from the ERD.
         // We are positioned on the correct TE_FieldDataRel and the
         // ERD ER_Entity.  We must position on the correct ER_Attribute
         // before the delete.
         GetIntegerFromAttribute( &lZKey, vTargetDTE, "ER_Attribute", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( vEMD, "ER_Attribute", "ZKey",
                                              lZKey, "ER_Entity" );
         ExcludeEntity( vTargetDTE, "ER_Attribute", zREPOS_NONE );
         DeleteEntity( vEMD, "ER_Attribute", zREPOS_NONE );
      }

      nRC = SetCursorNextEntity( vTE_Work, "TargetKey", 0 );
   }

   // As with SyncRelationships, above, whenever we find a relationship that
   // needs to be processed, we will break this routine to go to the window
   // for which the operator will select the correct foreign keys for the
   // relationship.  If we don't find such a relationship, we will return to
   // the calling window.

   nRC = SetCursorNextEntity( vEMD, "ER_RelType", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // We will not synchronize relationships for work entities
      while ( CompareAttributeToString( vEMD, "ER_Entity_2",
                                        "Purpose", "W" ) == 0 &&
              nRC >= zCURSOR_SET )
      {
         nRC = SetCursorNextEntity( vEMD, "ER_RelType", 0 );
      }

      GetIntegerFromAttribute( &lZKey, vEMD, "ER_RelType", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vTargetDTE, "ER_RelType_O", "ZKey",
                                           lZKey, "TE_DBMS_Source" );
      if ( nRC < zCURSOR_SET )
      {
         // Once we find a relationship to be processed, we set up the work
         // object and terminate to go to the sync window for further processing.
         zwTZTEUPDD_SyncPreBuild( vSubtask );
         //BL, 1999.11.28
         zwTZTEUPDD_DisableButton( vSubtask );
         return( 1 );
      }

      nRC = SetCursorNextEntity( vEMD, "ER_RelType", 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DataSrc_Prebuild( zVIEW vSubtask )
{
   zVIEW  vTZTEDBLO;
   zVIEW  vDTE;

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS",
                                    vDTE, "TE_DBMS_Source", "DBMS",
                                    0 ) < zCURSOR_SET )
   {
      SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "DBMS",
                                    "None", 0 );
   }

   return( 0 );
}

/*
   User selected new data type for a field/column.  Copy data type from
   physical DB data type list to the field/column entity and call the
   DB-handler routine to set the length.
*/
typedef zSHORT (POPERATION zFARPROC_VB)( zVIEW, zBOOL );

zOPER_EXPORT zSHORT OPERATION
DataTypeSelected( zVIEW vSubtask )
{
   zVIEW     vTZTEDBLO;
   zVIEW     vDTE;
   LPLIBRARY hLibrary;
   zFARPROC_VB pfn;
// zSHORT    (POPERATION pfn) ( zVIEW, zBOOL );
   zCHAR     szFileName[  zMAX_FILENAME_LTH + 1 ];

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   SetAttributeFromAttribute( vDTE, "TE_FieldDataRel", "DataType",
                              vTZTEDBLO, "DB_DataTypes", "InternalName" );

   GetStringFromAttribute( szFileName, sizeof( szFileName ),
                           vDTE, "TE_DBMS_Source", "GenerateExecutable" );
   hLibrary = SysLoadLibrary( vSubtask, szFileName );
   if ( hLibrary )
   {
      pfn = (zFARPROC_VB) SysGetProc( hLibrary, "SetDataType" );
      if ( pfn )
         // Call SetDataType to just set the length.
         // FALSE means don't set data type.
         (*pfn) ( vDTE, FALSE );
      else
         MessageSend( vSubtask, "TE00422", "Physical Data Model",
                      "Couldn't find 'SetDataType' in Generater Executable",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      SysFreeLibrary( vSubtask, hLibrary );
   }
   else
      MessageSend( vSubtask, "TE00421", "Physical Data Model",
                   "Couldn't load Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TableRecord_Postbuild( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vWindow;
   zSHORT nEnable    = 1;
   zSHORT nEnableSAP = 1;
   zCHAR  szSironDB_Type[ 2 ];

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "DataSrc" ) >= 0 )
      zwTZTEUPDD_RemoveMultiSelection( vSubtask, "TE_TablRec", "DataSrc" );
   else
      zwTZTEUPDD_RemoveMultiSelection( vSubtask, "TE_TablRec", "Main" );

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "DisplayOwner", "Y" ) == 0 )
      SetCtrlState( vSubtask, "Owner", zCONTROL_STATUS_ENABLED, 1 );
   else
      SetCtrlState( vSubtask, "Owner", zCONTROL_STATUS_ENABLED, 0 );

   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "DisplayDBMS_ID", "Y" ) == 0 )
      SetCtrlState( vSubtask, "DBMS_ID", zCONTROL_STATUS_ENABLED, 1 );
   else
      SetCtrlState( vSubtask, "DBMS_ID", zCONTROL_STATUS_ENABLED, 0 );

   // Disable Keep Physical Information for Siron Catalogs or SAP
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
   if ( *szSironDB_Type == zType_VSAM || *szSironDB_Type == zType_SAP )
   {
      nEnable = 0;

      if ( *szSironDB_Type == zType_SAP )
         nEnableSAP = 0;
   }

   SetCtrlState( vSubtask, "KeepPhysChar", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "KeepPhysCharForAllColumns", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "edName", zCONTROL_STATUS_ENABLED, nEnableSAP );
   SetCtrlState( vSubtask, "DataOrRel", zCONTROL_STATUS_ENABLED, nEnableSAP );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, nEnableSAP );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FieldRelLink_SetDB_DataTypes( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vTZTEDBLO;
   zSHORT nVisible;


   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "DisplayDBMS_ID",
                                  "Y" ) == 0 )
   {
      SetCtrlState( vSubtask, "Offset",  zCONTROL_STATUS_ENABLED, 1 );
      SetCtrlState( vSubtask, "DBMS ID", zCONTROL_STATUS_ENABLED, 1 );
   }
   else
   {
      SetCtrlState( vSubtask, "Offset",  zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "DBMS ID", zCONTROL_STATUS_ENABLED, 0 );
   }

   SetCursorFirstEntityByAttr( vTZTEDBLO, "DB_DataTypes", "InternalName",
                               vDTE, "TE_FieldDataRel", "DataType", 0 );

   // If no RelLink entity, make the box and min/max literals invisible.
   if ( CheckExistenceOfEntity( vDTE, "ER_RelLink" ) < zCURSOR_SET )
      nVisible = 0;
   else
      nVisible = 1;

   SetCtrlState( vSubtask, "ER_RelLinkMin", zCONTROL_STATUS_VISIBLE, nVisible );
   SetCtrlState( vSubtask, "ER_RelLinkMax", zCONTROL_STATUS_VISIBLE, nVisible );
   SetCtrlState( vSubtask, "txtER_RelLinkMin", zCONTROL_STATUS_VISIBLE, nVisible );
   SetCtrlState( vSubtask, "txtER_RelLinkMax", zCONTROL_STATUS_VISIBLE, nVisible );
   SetCtrlState( vSubtask, "ER_Rel", zCONTROL_STATUS_VISIBLE, nVisible );
   SetCtrlState( vSubtask, "txtER_Rel", zCONTROL_STATUS_VISIBLE, nVisible );

   // If no Attribute entity, make the box invisible.
   if ( CheckExistenceOfEntity( vDTE, "ER_Attribute" ) < zCURSOR_SET )
      nVisible = 0;
   else
      nVisible = 1;

   SetCtrlState( vSubtask, "ER_Attr", zCONTROL_STATUS_VISIBLE, nVisible );
   SetCtrlState( vSubtask, "txtER_Attr", zCONTROL_STATUS_VISIBLE, nVisible );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DisableFieldWndForSAP
//
//    if Data Source a SAP Data Source, disable all Controls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_DisableFieldWndForSAP( zVIEW vSubtask )
{
   SetCtrlState( vSubtask, "edName", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "DataOrRelfield", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "DataType", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Length", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "KeepPhysChar", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Null", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Scale", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Label", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Remarks", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
} // zwTZTEUPDD_DisableFieldWndForSAP



zOPER_EXPORT zSHORT OPERATION
FieldRelLink_Postbuild( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zBOOL  nEnable = FALSE;
   zCHAR  szSironDB_Type[ 2 ];

   zwTZTEUPDD_RemoveMultiSelection( vSubtask, "TE_FieldDataRel", "TableRecord" );

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   FieldRelLink_SetDB_DataTypes( vSubtask );

   // Disable Keep Physical Information for Siron Catalogs and SAP
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");

   if ( *szSironDB_Type == zType_VSAM || *szSironDB_Type == zType_SAP )
      SetCtrlState( vSubtask, "KeepPhysChar", zCONTROL_STATUS_ENABLED, FALSE );
   else
      SetCtrlState( vSubtask, "KeepPhysChar", zCONTROL_STATUS_ENABLED, TRUE );

   if ( ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ))
      nEnable = TRUE;

   SetCtrlState( vSubtask, "DataType", zCONTROL_STATUS_ENABLED, nEnable );

   // if Data Source a SAP Data Source, disable all Controls
   if ( *szSironDB_Type == zType_SAP )
      zwTZTEUPDD_DisableFieldWndForSAP( vSubtask );

   zwTZTEUPDD_SaveViewCursor( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FieldRelLink_PrevFieldRelLink( zVIEW vSubtask )
{
   zVIEW vTE;
   zVIEW vWindow;

   GetViewByName( &vTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask,
                                    "TE_FieldDataRel",
                                    "Field Data / Relationship",
                                     0 ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vTE, "TE_FieldDataRel", 0 );

   SetCursorPrevEntity( vTE, "TE_FieldDataRel", 0 );
   CreateTemporalSubobjectVersion( vTE, "TE_FieldDataRel" );

   FieldRelLink_SetDB_DataTypes( vSubtask );
   SetSelectStateOfEntity( vTE, "TE_FieldDataRel", 1 );

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      RefreshCtrl( vWindow, "lbColumnList" );

   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
FieldRelLink_NextFieldRelLink( zVIEW vSubtask )
{
   zVIEW vTE;
   zVIEW vWindow;

   GetViewByName( &vTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask,
                                    "TE_FieldDataRel",
                                    "Field Data / Relationship",
                                     0 ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vTE, "TE_FieldDataRel", 0 );

   SetCursorNextEntity( vTE, "TE_FieldDataRel", 0 );
   CreateTemporalSubobjectVersion( vTE, "TE_FieldDataRel" );

   FieldRelLink_SetDB_DataTypes( vSubtask );
   SetSelectStateOfEntity( vTE, "TE_FieldDataRel", 1 );

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      RefreshCtrl( vWindow, "lbColumnList" );

   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ForKeyM_Prebuild( zVIEW vSubtask )
{
   zVIEW vDTE;
   zVIEW vDTE_List;

   // Set up TE_DTEnviron named view for listing all fields under a
   // table for potential users for switching foreign keys.
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDTE_List, vDTE, 0 );
   SetCursorFirstEntity( vDTE_List, "TE_FieldDataRel", 0 );
   SetNameForView( vDTE_List, "TE_DTEnviron", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ForKeyM_SwitchForKeys( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vDTE_List;

   // Switch foreign keys from the TE_FieldDataRel entity in the view
   // "TE_DB_Environ" (the current foreign key) to the TE_FieldDataRel
   // entity in the view "TE_DTEnviron" (the new field selected in the list).

   // vDTE points to the target FK
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   // vDTE_List points to the source FK
   GetViewByName( &vDTE_List, "TE_DTEnviron", vSubtask, zLEVEL_TASK );

   // Return if the entity we are on (vDTE.TE_FieldDataRel) does not have
   // a parent key (in which case it is not a foreign key).
   if ( CheckExistenceOfEntity( vDTE, "TE_FieldDataRel_PK" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "TE00424", "Physical Data Model",
                   "The current field is not a foreign key.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      return( 0 );
   }

   // Return if the entity selected in list is the same as the entity we
   // are updating.
   if ( CompareAttributeToAttribute( vDTE,      "TE_FieldDataRel", "ZKey",
                                     vDTE_List, "TE_FieldDataRel", "ZKey" ) == 0 )
   {
      MessageSend( vSubtask, "TE00425", "Physical Data Model",
                   "Entry selected is same as entry being updated.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      return( 0 );
   }

   // do the FK switch
   oTZTENVRO_SwitchForeignKeys( vDTE, vDTE_List );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_BuildXODsLOD_List( zVIEW vSubtask )
{
   zVIEW vLOD_List;
   zVIEW vDTE;

   // Create a view that lists all LODs.
   RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
   OrderEntityForView( vLOD_List, "W_MetaDef", "Name A" );
   SetNameForView( vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );

   // Sort Attributes in original sequence
   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      oTZTENVRO_SortFields( vDTE, vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SelectAllLODs( zVIEW vSubtask )
{
   zVIEW vLOD_List;

   GetViewByName( &vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   SetAllSelectStatesForEntity( vLOD_List, "W_MetaDef", 1, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_BuildXOD_Init( zVIEW  vSubtask,
                          zPCHAR szServer )
{
   zVIEW  vLPLR;
   zVIEW  vAppl;
   zSHORT nRC;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szAppName[ 32 ];
   zCHAR  szServerSubDir[ 33 ];
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zVIEW  vApplSubtask;

   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Start the network if necessary.  Use zeidon and hzkzoe as constants
   // for now.
   nRC = NetStatus( vSubtask, "zeidon" );
   if ( nRC == 0 )
   {
      nRC = NetStartup( vSubtask, "zeidon", "hzkzoe" );
      if ( nRC )
      {
         MessageSend( vSubtask, "TE00427", "TE Build Executables",
                      "Network cannot be started.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }
   }

   // First get the application name and make sure it exists in the
   // zeidon.app object on the server.  If it isn't there, add it.
   GetStringFromAttribute( szAppName, sizeof( szAppName ), vLPLR, "LPLR", "Name" );
   strcpy_s( szFileName, sizeof( szFileName ), "zeidon.app" );
   SfCreateSubtask( &vApplSubtask, vSubtask, "Zeidon System" );
   nRC = NetActivateOI_FromFile( 0, szServer, &vAppl, "KZAPPLOO",
                                 vApplSubtask, szFileName, zSINGLE );
   if ( nRC >= 0 )
   {
      nRC = SetCursorFirstEntityByString( vAppl, "APPLICATION", "APP_NAME",
                                          szAppName, 0 );
      if ( nRC >= zCURSOR_SET )
         DropObjectInstance( vAppl );
      else
      {
         CreateEntity( vAppl, "APPLICATION", zPOS_LAST );
         SetAttributeFromString( vAppl, "APPLICATION", "APP_NAME", szAppName );
         strcpy_s( szServerSubDir, sizeof( szServerSubDir ), szAppName );
         strcat_s( szServerSubDir, sizeof( szServerSubDir ), "/" );
         SetAttributeFromString( vAppl, "APPLICATION", "APP_DLL",
                                 szServerSubDir );
         SetAttributeFromString( vAppl, "APPLICATION", "APP_ADOBIN",
                                 szServerSubDir );
         SetAttributeFromString( vAppl, "APPLICATION", "APP_LOCAL",
                                 szServerSubDir );
         TraceLineS( "*** Committing server file: ", szFileName );
         strcpy_s( szMsg, sizeof( szMsg ), "Sending ZEIDON.APP: " );
         MB_SetMessage( vSubtask, 1, szMsg );

         // Send OI to server.
         nRC = NetCommitOI_ToFile( 0, szServer, vAppl, szFileName, zASCII );
         if ( nRC )
         {
            TraceLineI( "NetCommitOI_ToFile nRC: ", nRC );
            TraceLineS( "FileName: ", szFileName );
            MessageSend( vSubtask, "TE00428", "TE Build Executables",
                         "Error on NetCommitOI_ToFile.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }

         DropObjectInstance( vAppl );
      }
   }
   else
   {
      SfDropSubtask( vApplSubtask, 0 );
      MessageSend( vSubtask, "TE00429", "TE Build Executables",
                   "ZEIDON.APP cannot be activated.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   SfDropSubtask( vApplSubtask, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_BuildXODsOnServer( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vLPLR;
   zVIEW  vLOD_List;
   zVIEW  vLOD;
   zVIEW  vXOD;
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szServer[ 33 ];

   // First make sure the network is started and the application is
   // initialized on the Server.
   zmemset( szServer, 0, sizeof( szServer ) );
   if ( zwTZTEUPDD_BuildXOD_Init( vSubtask, szServer ) < 0 )
      return( -1 );

   // For each selected LOD, build the XOD using the current DBMS and
   // send it to the Server.

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstSelectedEntity( vLOD_List, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vLOD_List, "W_MetaDef", "" ) )
   {
      GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ),
                              vLOD_List, "W_MetaDef", "Name" );

      nRC = ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zCURRENT_OI );
      if ( nRC < 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Could not Activate LOD: " );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         strcat_s( szMsg, sizeof( szMsg ), ".\nAborting Build" );
         MessageSend( vSubtask, "TE00426", "Physical Data Model",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
      else
      {
         // Send message that we are building the LOD.
         strcpy_s( szMsg, sizeof( szMsg ), "Building executable for LOD: " );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         strcat_s( szMsg, sizeof( szMsg ), "." );
         MB_SetMessage( vSubtask, 1, szMsg );

         // Make sure the TE_SourceZKey attribute is set because it determines
         // what DBMS_Source is used in building the XOD.
         if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET )
            SetAttributeFromAttribute( vLOD, "POD",            "TE_SourceZKey",
                                       vDTE, "TE_DBMS_Source", "ZKey" );

         // Build the XOD in memory
         oTZZOXODO_SaveXOD( vSubtask, vLOD );
         SetNameForView( vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
         DropMetaOI( vSubtask, vLOD );

         // Send the XOD to the Server.
         GetViewByName( &vXOD, "TZZOXODO", vSubtask, zLEVEL_TASK );
         GetStringFromAttribute( szFileName, sizeof( szFileName ), vLPLR, "LPLR", "Name" );
         strcat_s( szFileName, sizeof( szFileName ), "/" );
         strcat_s( szFileName, sizeof( szFileName ), szLOD_Name );
         strcat_s( szFileName, sizeof( szFileName ), ".XOD" );
         TraceLineS( "*** Committing server file: ", szFileName );

         nRC = NetCommitOI_ToFile( 0, szServer, vXOD, szFileName, zASCII );
         if ( nRC < 0 )
         {
            TraceLineI( "NetCommitOI_ToFile nRC: ", nRC );
            TraceLineS( "FileName: ", szFileName );
            MessageSend( vSubtask, "TE00428", "TE Build Executables",
                         "Error on NetCommitOI_ToFile.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }
      }
   }

   MB_SetMessage( vSubtask, 1, "Build of XODs complete." );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_BuildXODsOnLPLR( zVIEW vSubtask )
{
   // This routine does not actually save LODs, but only builds the XODs on the LPLR.

   zVIEW  vDTE;
   zVIEW  vLPLR;
   zVIEW  vLOD_List;
   zVIEW  vLOD;
   zVIEW  vXOD;
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szTemp[ zMAX_FILESPEC_LTH + 1 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Save Subtask for use by XOD build routine.
   SetNameForView( vSubtask, "TE_Window", vSubtask, zLEVEL_TASK );

   // For each selected LOD, build the XOD using the current DBMS and commit as file to LPLR, unless the LOD does not have a POD.
   // Note that we are not looking to see if the DBMS identified in the POD.TE_SourceZKey matches the current DBMS. We will save
   // selected LODs with the current DBMS no matter how they were saved last time.

   for ( nRC = SetCursorFirstSelectedEntity( vLOD_List, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vLOD_List, "W_MetaDef", "" ) )
   {
      GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ), vLOD_List, "W_MetaDef", "Name" );

      nRC = ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zCURRENT_OI );
      if ( nRC < 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Could not Activate LOD: " );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         strcat_s( szMsg, sizeof( szMsg ), ".\nAborting Build" );
         MessageSend( vSubtask, "TE00426", "Physical Data Model",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
      else
      {
         // Send message that we are building the LOD.
         strcpy_s( szMsg, sizeof( szMsg ), "Building executable for LOD: " );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         strcat_s( szMsg, sizeof( szMsg ), "." );
         MB_SetMessage( vSubtask, 1, szMsg );

         // Make sure the TE_SourceZKey attribute is set because it determines what DBMS_Source is used in building the XOD.
         if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET )
            SetAttributeFromAttribute( vLOD, "POD", "TE_SourceZKey", vDTE, "TE_DBMS_Source", "ZKey" );

         // Build the XOD in memory.
         oTZZOXODO_SaveXOD( vSubtask, vLOD );
         SetNameForView( vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
         GetViewByName( &vXOD, "TZZOXODO", vSubtask, zLEVEL_TASK );
         // 09/08/15 - We think we do not want to keep the vLOD in cached memory so instead
         // of doing a DropMetaOI, we are going to do a DropObjectInstance
         //DropMetaOI( vSubtask, vLOD );
         DropObjectInstance( vLOD );

         // Commit the XOD to LPLR file.
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vLPLR, "LPLR", "ExecDir" );
         SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szTemp );
         ofnTZCMWKSO_AppendSlash( szFileName );
         strcat_s( szFileName, sizeof( szFileName ), szLOD_Name );
         strcat_s( szFileName, sizeof( szFileName ), ".XOD" );
         TraceLineS( "*** Committing workstation file: ", szFileName );
      // DisplayObjectInstance( vXOD, "", "" );
         CommitOI_ToFile( vXOD, szFileName, zSINGLE );
         // 09/08/15 - Adding drop of the xod view.
         DropObjectInstance( vXOD );
      }
   }

   MB_SetMessage( vSubtask, 1, "Build of XODs complete." );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_BuildNetXODsOnLPLR( zVIEW vSubtask )
{
   // This routine is the same as BuildXODsOnLPLR except that it always
   // builds network XODs regardless of the DB Source entry selected.

   zVIEW  vDTE;
   zVIEW  vTZTEDBLO;
   zCHAR  szOrigDBMS_Name[ 33 ];
   zCHAR  szNetworkName[ 33 ];
   zSHORT nRC;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );

   // We must first determine the DBMS and Network attribute values to be
   // set prior to building the XODs.
   // In determinining the DBMS value, we will use the following order:
   // 1. If the DBMS Source entity is already for a network, we will use
   //    current values.
   // 2. Otherwise, if there is a DefaultNetwork value specified in the INI
   //    file, we will use that value to position on the corresponding entry
   //    in the DB Handler object.
   // 3. If neither of the above is true, we will use "Network Server" for
   //    the DBMS value for positioning on the DB Handler object.

   // Save current DBMS value.
   GetStringFromAttribute( szOrigDBMS_Name, sizeof( szOrigDBMS_Name ), vDTE, "TE_DBMS_Source", "DBMS" );

   // Position on corresponding DBMS entry in DB Handler object.
   SetCursorFirstEntityByAttr( vTZTEDBLO, "TE_DBMS_Source", "DBMS", vDTE, "TE_DBMS_Source", "DBMS", 0 );

   // Check if current DBMS is a Network specification. We wiil know this because the attribute "Network" value will be other than null.
   if ( CompareAttributeToString( vTZTEDBLO, "TE_DBMS_Source", "Network", "" ) != 0 )
   {
      // Current specification is Network, so just use it.
      zwTZTEUPDD_BuildXODsOnLPLR( vSubtask );
   }
   else
   {
      // Current specification is not Network, get Default Network value from
      // INI and then locate in DB Handler object. Don't use it if it wasn't
      // found in DB Handler object.
      SysReadZeidonIni( -1, "[Network]", "DefaultNetwork", szNetworkName, sizeof( szNetworkName ) );
      if ( szNetworkName[ 0 ] != 0 )
      {
         nRC = SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "Network", szNetworkName, 0 );
         if ( nRC < zCURSOR_SET )
         {
            MessageSend( vSubtask, "TE00426", "Physical Data Model",
                         "Default Network value not found in DB Handler object; using internal default.",
                         zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            szNetworkName[ 0 ] = 0;
         }
         // The else condition means we're positioned on the Default Network.
      }

      if ( szNetworkName[ 0 ] == 0 )
      {
         // Use default network for Ton Beller (DBMS = "Network Server")
         SetCursorFirstEntityByString( vTZTEDBLO, "TE_DBMS_Source", "DBMS", "Network Server", 0 );
      }

     if ( !ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ))
        SetViewUpdate( vDTE );

      SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "DBMS", vTZTEDBLO, "TE_DBMS_Source", "DBMS" );
      SetAttributeFromAttribute( vDTE, "TE_DBMS_Source", "Network", vTZTEDBLO, "TE_DBMS_Source", "Network" );

      zwTZTEUPDD_BuildXODsOnLPLR( vSubtask );

      // Reset values back to what they were.

      SetAttributeFromString( vDTE, "TE_DBMS_Source", "DBMS", szOrigDBMS_Name );
      SetAttributeFromString( vDTE, "TE_DBMS_Source", "Network", "" );

      if ( !ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ))
        SetViewReadOnly( vDTE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_RebuildLODsOnLPLR( zVIEW vSubtask )
{
   // This routine saves all the selected LODs in the LPLR.

   zVIEW  vDTE;
   zVIEW  vLPLR;
   zVIEW  vLOD_List;
   zVIEW  vLOD;
   zVIEW  vXOD;
   zSHORT nRC;
   zCHAR  szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szTemp[ zMAX_FILESPEC_LTH + 1 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD_List, "TZZOLFLO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // Save Subtask for use by XOD build routine.
   SetNameForView( vSubtask, "TE_Window", vSubtask, zLEVEL_TASK );

   // For each selected LOD, build the XOD using the current DBMS and
   // commit as file to LPLR, unless the LOD does not have a POD.
   // Note that we are not looking to see if the DBMS identified
   // in the POD.TE_SourceZKey matches the current DBMS. We will save
   // selected LODs with the current DBMS no matter how they were saved last time.

   for ( nRC = SetCursorFirstSelectedEntity( vLOD_List, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vLOD_List, "W_MetaDef", "" ) )
   {
      GetStringFromAttribute( szLOD_Name, sizeof( szLOD_Name ), vLOD_List, "W_MetaDef", "Name" );

      nRC = ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zCURRENT_OI );
      if ( nRC < 0 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Could not Activate LOD: " );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         strcat_s( szMsg, sizeof( szMsg ), ".\nAborting Build" );
         MessageSend( vSubtask, "TE00426", "Physical Data Model",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
      else
      {
         // Send message that we are building the LOD.
         strcpy_s( szMsg, sizeof( szMsg ), "Building executable for LOD: " );
         strcat_s( szMsg, sizeof( szMsg ), szLOD_Name );
         strcat_s( szMsg, sizeof( szMsg ), "." );
         MB_SetMessage( vSubtask, 1, szMsg );

         // Make sure the TE_SourceZKey attribute is set because it determines what DBMS_Source is used in building the XOD.
         if ( CheckExistenceOfEntity( vLOD, "POD" ) >= zCURSOR_SET )
            SetAttributeFromAttribute( vLOD, "POD", "TE_SourceZKey", vDTE, "TE_DBMS_Source", "ZKey" );

         // Build the XOD in memory.
         oTZZOXODO_SaveXOD( vSubtask, vLOD );
         SetNameForView( vLOD, "TZZOLODO", vSubtask, zLEVEL_TASK );
         GetViewByName( &vXOD, "TZZOXODO", vSubtask, zLEVEL_TASK );

         // Commit the XOD to LPLR file.
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vLPLR, "LPLR", "ExecDir" );
         SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szTemp );
         ofnTZCMWKSO_AppendSlash( szFileName );
         strcat_s( szFileName, sizeof( szFileName ), szLOD_Name );
         strcat_s( szFileName, sizeof( szFileName ), ".XOD" );
         TraceLineS( "*** Committing workstation file: ", szFileName );
         CommitOI_ToFile( vXOD, szFileName, zSINGLE );

         // Commit the LOD to the LPLR file.
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vLPLR, "LPLR", "MetaSrcDir" );
         SysConvertEnvironmentString( szFileName, sizeof( szFileName ), szTemp );
         ofnTZCMWKSO_AppendSlash( szFileName );
         strcat_s( szFileName, sizeof( szFileName ), szLOD_Name );
         strcat_s( szFileName, sizeof( szFileName ), ".LOD" );
         TraceLineS( "*** Committing workstation file: ", szFileName );
         CommitOI_ToFile( vLOD, szFileName, zSINGLE );
         DropMetaOI( vSubtask, vLOD );
      }
   }

   MB_SetMessage( vSubtask, 1, "Build of XODs complete." );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_BuildXMD_OnServer( zVIEW vSubtask )
{
   zVIEW  vLPLR;
   zVIEW  vXMD;
   zSHORT nRC;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szServer[ 33 ];

   // First make sure the network is started and the application is initialized on the Server.
   zmemset (szServer,0,sizeof(szServer));
   if ( zwTZTEUPDD_BuildXOD_Init( vSubtask, szServer ) < 0 )
      return( -1 );

   // Send the XDM itself to the Server.
   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vLPLR, "LPLR", "ExecDir" );
   strcat_s( szFileName, sizeof( szFileName ), "\\ZEIDON.XDM" );
   TraceLineS( "*** Activating client file: ", szFileName );
   nRC = ActivateOI_FromFile( &vXMD, "TZDMXGPO", vSubtask, szFileName, zSINGLE );

   if ( nRC >= 0 )
   {
      GetStringFromAttribute( szFileName, sizeof( szFileName ), vLPLR, "LPLR", "Name" );
      strcat_s( szFileName, sizeof( szFileName ), "/ZEIDON.XDM" );
      TraceLineS( "*** Committing server file: ", szFileName );
      MB_SetMessage( vSubtask, 1, "Sending XDM" );

      // Send OI to server.
      nRC = NetCommitOI_ToFile( 0, szServer, vXMD, szFileName, zASCII );
      if ( nRC )
      {
         TraceLineI( "NetCommitOI_ToFile nRC: ", nRC );
         TraceLineS( "FileName: ", szFileName );
            MessageSend( vSubtask, "TE00428", "TE Build Executables",
                         "Error on NetCommitOI_ToFile.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }

      MB_SetMessage( vSubtask, 1, "Build of XDM complete." );
   }

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_ReconstructER_Attribues
//
// This function reconstructs all ER_Attributes which have been deleted for foreign keys for
// all catalogs.
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ReconstructER_Attributes( zVIEW vSubtask, zVIEW vDTE, zVIEW vEMD)
{
   zSHORT nRC;
   zSHORT nRC1;

   // For all catalogs
   nRC1 = SetCursorFirstEntity( vDTE, "TE_TablRec", 0 );
   while ( nRC1 >= zCURSOR_SET )
   {
      // Reconstruction for the actual Entity
      nRC = zwTZTEUPDD_ReconstructSelectedEntity( vSubtask, vDTE, vEMD );
      if (nRC != 0)
      {
         DropView( vEMD );
         return( nRC );
      }

      nRC1 = SetCursorNextEntity( vDTE, "TE_TablRec", 0 );
   }

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteSelectedEntity
//
// This function deletes the selected Enity from the TE.
// For Siron catalogs it starts the reconstruction of the ER_Attributes for Foreign Keys
// In this case there are two situations:
// 1. The entity contains hidden ER_Attributes. They have to be opened.
//    function: zwTZTEUPDD_ReconstructSelectedEntity
// 2. The Key attribute of the Entity is Foreign key for another Entity
//    function: zwTZTEUPDD_ReconstructERForForeignKey
// For both functions we have to proof all attributes of the actual entity.
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DeleteSelectedEntity( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vDTE_Del;
   zSHORT   nZRetCode;
   zSHORT   nRC;
   zSHORT   RESULT;
   zCHAR    szSironDB_Type[ 2 ];
   zLONG    lEntityKey;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   // We need the DB-Type because Siron Catalogs must be handled in a special way
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
   if ( *szSironDB_Type == zType_VSAM )
      return( zwTZTEUPDD_WindowDeleteVsam (vSubtask, 0));

   // No Siron catalogs than any other database
   strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "OK to delete selected Tables/Records "
             "from the Data Source" );
   nRC = MessagePrompt( vSubtask, "TE00414", "Physical Data Model",
                        szWorkMessage,
                        zBEEP, zBUTTONS_YESNO,
                        zRESPONSE_NO, zICON_QUESTION );

   if ( nRC != zRESPONSE_YES )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Create a new view to delete the foreign keys
   CreateViewFromViewForTask( &vDTE_Del, vDTE, 0 );
   SetNameForView( vDTE_Del, "vDTE_Del", vSubtask, zLEVEL_TASK );

   // Delete the actual selected Entities from the DataSource
   for ( nRC = SetCursorFirstSelectedEntity( vDTE, "TE_TablRec", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDTE, "TE_TablRec", "" ) )
   {
      // Get the ZKEY from the actual ER_Entity
      GetIntegerFromAttribute( &lEntityKey, vDTE, "ER_Entity", "ZKey" );

      // First we have to delete the foreign keys from the related entities
      // Search the actual foreign key for all entities
      for ( nRC = SetCursorFirstEntity( vDTE_Del, "TE_FieldDataRel", "TE_DBMS_Source" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDTE_Del, "TE_FieldDataRel", "TE_DBMS_Source" ) )
      {
          RESULT = SetCursorFirstEntityByInteger( vDTE_Del, "ER_EntityRelLink", "ZKey", lEntityKey, "" );
          if ( RESULT == zCURSOR_SET )
          {
            // Here we found a foreign key and we delete it
            nZRetCode = DeleteEntity( vDTE_Del, "TE_FieldDataRel", zREPOS_NONE );
          }
      }

      // Delete now the actual Entity from the DataSource
      nZRetCode = DeleteEntity( vDTE, "TE_TablRec", zREPOS_NONE );
   }

   DropView (vDTE_Del);

   if ( SetCursorPrevEntity( vDTE, "TE_TablRec", "" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vDTE, "TE_TablRec", 1 );

   zwTZTEUPDD_RefreshWindows( vSubtask, "TableRecord", "TE_TablRec" );

   return( 0 );
} // zwTZTEUPDD_DeleteSelectedEntity

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SetERDUpdateFlag( zVIEW vSubtask, zPVIEW pvTE_Work, zVIEW vDTE )
{
   zSHORT nRC;

   nRC = GetViewByName( pvTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      // Activate WorkLOD to set sign: ERD was changed
      ActivateEmptyObjectInstance( pvTE_Work, "TZTEWRKO", vDTE, zSINGLE );
      SetNameForView( *pvTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );
      CreateEntity( *pvTE_Work, "TE_Work", zPOS_AFTER );
   }
   // Set Updateflag for saving the ERD
   SetAttributeFromString (*pvTE_Work, "TE_Work", "ER_UpdateFlag", "Y");

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_ReconstructSelectedEntity
//
// This function reconstructs all ER_Attributes which have been deleted for
// foreign keys for a special catalog.
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ReconstructSelectedEntity( zVIEW vSubtask, zVIEW vDTE, zVIEW vEMD )
{
    zSHORT nRC2;
    zSHORT nRC;
    zVIEW  vDTE_Work;

    CreateViewFromViewForTask( &vDTE_Work, vDTE, 0 );
    SetNameForView( vDTE_Work, "vDTE_Work", vDTE, zLEVEL_TASK );

    // For all attributes of the actual catalog
    nRC2 = SetCursorFirstEntity( vDTE_Work, "TE_FieldDataRel", 0 );
    while ( nRC2 >= zCURSOR_SET )
    {
       if ( CheckExistenceOfEntity( vDTE_Work, "TE_FieldDataRel_PK" ) >= zCURSOR_SET )
       {
          // If the attribute is a foreign key, reconstruct the attribute
          // in the ER-Model.  Set the ER-Attribute and the Domain.
          nRC = zwTZTEUPDD_SetERAttributeWithDomain( vSubtask, vDTE_Work, vEMD );
          if ( nRC < 0 )
          {
             DropView( vDTE_Work );
             return( zCALL_ERROR );
          }

          // Delete all instances of ER_FactType where ER_FactType.Name == "REL"
          nRC = SetCursorFirstEntityByString( vEMD, "ER_FactType", "Type", "REL", 0 );
          while ( nRC >= zCURSOR_SET )
          {
              DeleteEntity( vEMD, "ER_FactType", zREPOS_NONE );
              nRC = SetCursorFirstEntityByString( vEMD, "ER_FactType", "Type", "REL", 0 );
          }

          // If the attribute is a key attribute create a new instance of ER_FactType where ER_FactType = "ATR".
          // Include ER_Entity into ER_AttributeIdentifier.
          zwTZTEUPDD_SetFactType( vSubtask, vDTE_Work, vEMD );
       }

       nRC2 = SetCursorNextEntity( vDTE_Work, "TE_FieldDataRel", 0 );
    }

    DropView( vDTE_Work );
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_ReconstructERForForeignKey
//
// This function tests if the key attribute of the actual entity is a foreign key for another
// entity. Then it reconstructs the ER_Attributes for the last entity.
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_ReconstructERForForeignKey( zVIEW vSubtask, zVIEW vDTE, zVIEW vEMD)
{
   zSHORT nRC;
   zVIEW  vDTE_Work;
   zVIEW  vDTE2;

   zCHAR  szCatalogName[33];
   zCHAR  szAttributeName[35];

   CreateViewFromViewForTask(&vDTE_Work, vDTE, 0 );
   SetNameForView(vDTE_Work, "vDTE_Work", vDTE, zLEVEL_TASK );

   nRC = SetCursorFirstEntity(vDTE_Work, "TE_FieldDataRel", 0);
   while (nRC >= zCURSOR_SET)
   {
      // Do we have a key attribute
      if (CompareAttributeToString (vDTE_Work, "TE_FieldDataRel", "Key", "Y") == 0)
      {
         // Is the attribute used as foreign key
         if (CheckExistenceOfEntity (vDTE_Work, "TE_FieldDataRelFK_1") >= zCURSOR_SET)
         {
            // Notice: the catalogname and the attributename to use it later.
            GetStringFromAttribute( szCatalogName, sizeof( szCatalogName ), vDTE_Work, "TE_TablRec", "Name");
            GetStringFromAttribute( szAttributeName, sizeof( szAttributeName ), vDTE_Work, "TE_FieldDataRel", "Name");

            CreateViewFromViewForTask(&vDTE2, vDTE_Work, 0 );
            SetNameForView(vDTE2, "vDTE2", vDTE_Work, zLEVEL_TASK );

            // Search for all other entities: The key-attribute is it used as foreign key ???
            nRC = SetCursorFirstEntity(vDTE2, "TE_TablRec", 0);
            while (nRC >= zCURSOR_SET)
            {
               nRC = CompareAttributeToString( vDTE2, "TE_TablRec", "Name", szCatalogName);
               if ( nRC && nRC != zCALL_ERROR )
               {
                  // For each attribute of the entity
                  nRC = SetCursorFirstEntity( vDTE2, "TE_FieldDataRel", 0 );
                  while ( nRC >= zCURSOR_SET )
                  {
                     if ( CheckExistenceOfEntity(vDTE2, "TE_FieldDataRel_PK") >= zCURSOR_SET &&
                          CheckExistenceOfEntity(vDTE2, "ER_EntityRelLink") >= zCURSOR_SET &&
                          CompareAttributeToString( vDTE2, "ER_EntityRelLink", "Name", szCatalogName) == 0 )
                     {
                        // If the attribute is a foreign key compare its name with the original
                        // Set the ER-Attribute and the Domain
                        nRC = CompareAttributeToString( vDTE2, "TE_FieldDataRel_PK", "Name", szAttributeName );
                        if ( nRC == 0 )
                        {
                           // We found a foreign key and we have to reconstruct it in the ER
                           nRC = zwTZTEUPDD_SetERAttributeWithDomain( vSubtask, vDTE2, vEMD );
                           if (nRC < 0)
                           {
                              DropView( vDTE2 );
                              return( zCALL_ERROR );
                           }
                           // Delete the instance of ER_FactType where ER_FactType.Name == "REL"
                           // and the ER_Entity_Other_Identifier.Name == the catalog which should be deleted
                           nRC = SetCursorFirstEntityByString( vEMD, "ER_FactType", "Type", "REL", 0 );
                           while (nRC >= zCURSOR_SET)
                           {
                               nRC = CompareAttributeToString( vEMD, "ER_Entity_Other_Identifier", "Name", szCatalogName);
                               if ( nRC == 0 )
                               {
                                  DeleteEntity( vEMD, "ER_FactType", zREPOS_NONE );
                                  break;
                               }
                               nRC = SetCursorNextEntityByString( vEMD, "ER_FactType", "Type", "REL", 0 );
                           }
                           // If the attribute is a key attribute create a new instance of ER_FactType where ER_FactType = "ATR"
                           // Include ER_Entity into ER_AttributeIdentifier
                           zwTZTEUPDD_SetFactType( vSubtask, vDTE2, vEMD );

                           // Now Delete the Relationship in TE and Include the ER_Attribute again.
                           ExcludeEntity( vDTE2, "ER_RelLink", zREPOS_NONE );
                           if ( CheckExistenceOfEntity( vDTE2, "ER_Attribute" ) >= zCURSOR_SET)
                              ExcludeEntity( vDTE2, "ER_Attribute", zREPOS_NONE );

                           IncludeSubobjectFromSubobject( vDTE2, "ER_Attribute", vEMD, "ER_Attribute", zPOS_AFTER );

                           // Exclude the ForeignKey fields of the parent
                           ExcludeEntity( vDTE_Work, "TE_FieldDataRelFK_1", zREPOS_NONE );
                           if (CheckExistenceOfEntity( vDTE_Work, "TE_FieldDataRelAsFK") >= zCURSOR_SET )
                              ExcludeEntity( vDTE_Work, "TE_FieldDataRelAsFK", zREPOS_NONE );

                           zwTZTEUPDD_ReconstructERForForeignKey( vSubtask, vDTE2, vEMD );

                           break;
                        }
                     }
                     nRC = SetCursorNextEntity (vDTE2, "TE_FieldDataRel", 0);
                  }
               }
               nRC = SetCursorNextEntity (vDTE2, "TE_TablRec", 0);
            }
            DropView (vDTE2);
         }
      }

      nRC = SetCursorNextEntity (vDTE_Work, "TE_FieldDataRel", 0);
   }

   DropView (vDTE_Work);
   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_SetERAttributeWithDomain
//
// This function creates the ER_Attributes and Domains while reconstructing for Siron catalogs
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SetERAttributeWithDomain( zVIEW  vSubtask,
                                     zVIEW vDTE_Work, zVIEW vEMD )
{
   zSHORT nRC;
   zCHAR  szDomainName[ 256 ];
   zCHAR  szDataType[ 2 ];
   zVIEW  vDomain;

   // First position the ER-Model on the actual catalog. We find the catalogname in TE in Entity "TE_TableRec"
   nRC = SetCursorFirstEntityByAttr( vEMD, "ER_Entity", "Name", vDTE_Work, "ER_Entity", "Name", 0 );

   // Now create a new ER_Attribute in the Datamodel and set all relevant data
   // from the TE
   CreateMetaEntity( vSubtask,vEMD, "ER_Attribute", zPOS_FIRST);
   SetAttributeFromAttribute( vEMD, "ER_Attribute", "Name", vDTE_Work, "TE_FieldDataRel", "Name" );
   SetAttributeFromAttribute( vEMD, "ER_Attribute", "Lth", vDTE_Work, "TE_FieldDataRel", "Length" );
   SetAttributeFromAttribute( vEMD, "ER_Attribute", "NotNull", vDTE_Work, "TE_FieldDataRel", "SQL_NULLS" );
   SetAttributeFromAttribute( vEMD, "ER_Attribute", "ParticipatesInKey", vDTE_Work, "TE_FieldDataRel", "Key" );

   // Include the Domains
   memset( szDomainName, 0, 256 );
   GetStringFromAttribute( szDataType, sizeof( szDataType ), vDTE_Work, "TE_FieldDataRel", "DataType");
   switch (*szDataType)
   {
      case 'S': strcpy_s( szDomainName, sizeof( szDomainName ), "Text" );
                break;
      case 'L': strcpy_s( szDomainName, sizeof( szDomainName ), "Integer" );
                break;
      case 'M': strcpy_s( szDomainName, sizeof( szDomainName ), "Decimal" );
                break;
      default:  break;
   }

   nRC = ActivateMetaOI_ByName( vSubtask, &vDomain, 0, zREFER_DOMAIN_META,
                                zSINGLE | zLEVEL_APPLICATION, szDomainName, zCURRENT_OI );
   if ( nRC < 0 )
      return( zCALL_ERROR );

   if ( CheckExistenceOfEntity( vEMD, "Domain") >= zCURSOR_SET )
      ExcludeEntity( vEMD, "Domain", zREPOS_NONE );

   IncludeSubobjectFromSubobject( vEMD, "Domain", vDomain, "Domain", zPOS_AFTER );
   DropMetaOI( vSubtask, vDomain );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SetFactType( zVIEW vSubtask, zVIEW vDTE_Work, zVIEW vEMD)
{
   zCHAR szKeyKen[ 2 ];

   GetStringFromAttribute( szKeyKen, sizeof( szKeyKen ), vDTE_Work, "TE_FieldDataRel", "Key");
   if ( *szKeyKen == 'Y' )
   {
      CreateMetaEntity( vSubtask,vEMD, "ER_FactType", zPOS_FIRST );
      SetAttributeFromString(   vEMD, "ER_FactType", "Type", "ATR" );
      if ( CheckExistenceOfEntity( vEMD, "ER_AttributeIdentifier" ) >= zCURSOR_SET )
         ExcludeEntity( vEMD, "ER_AttributeIdentifier", zREPOS_NONE );

      IncludeSubobjectFromSubobject( vEMD, "ER_AttributeIdentifier", vEMD, "ER_Attribute", zPOS_AFTER );
   }

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_PreBuildDelete
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_PreBuildDelete( zVIEW vSubtask )
{
   zVIEW vDelWork;
   zLONG lDelAllTables;

   GetViewByName( &vDelWork, "TZSIDEWK", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute (&lDelAllTables, vDelWork, "TZSIDEWK", "wkAllTables");

   switch (lDelAllTables)
   {
      case 0: /* Only delete selected table */
           strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "If you Delete the selected Entity "
                    "from the Data Source, ERD and TE will be changed. "
                    "Do you want to continue ?");
           break;

      case 1: /* Delete all Tables */
           strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "If you Delete all Tables/Records "
                     "from the Data Source, ERD and TE will be changed. "
                     "Do you want to continue ?");
           break;

      case 2: /* Delete the DataSource */
           strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "If you Delete the DataSource, ERD and TE will "
                     "be changed. Do you want to continue ?");
           break;

      default:
           return( zCALL_ERROR );

   }

   SetAttributeFromString (vDelWork, "TZSIDEWK", "wkShowMessage", szWorkMessage);

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteVsamTables
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DeleteVsamTables( zVIEW  vSubtask )
{

   zVIEW  vDelWork;
   zVIEW  vLPLR;
   zVIEW  vEMD;
   zVIEW  vDTE;
   zVIEW  vTE_Work;
   zLONG  lDelAllTables;
   zSHORT nRC;
   zSHORT nZRetCode;
   zCHAR  szWindowName[ 33 ];
   zCHAR  szEntityName[ 33 ];

   GetViewByName( &vDelWork, "TZSIDEWK", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );
   SetNameForView( vDTE, "vDTE", vSubtask, zLEVEL_TASK );

   strcpy_s( szWindowName, sizeof( szWindowName ), "" );
   strcpy_s( szEntityName, sizeof( szEntityName ), "" );

   // For Siron Flat Files we have to reconstruct Foreign keys to
   // ER Attributes.  Get the ERD
   nRC = GetViewByName( &vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vLPLR, zSOURCE_ERD_META );
      ActivateMetaOI( vSubtask, &vEMD, vLPLR, zSOURCE_ERD_META, zCURRENT_OI );
      SetNameForView( vEMD, "TZEREMDO_U", vSubtask, zLEVEL_TASK );
      DropView( vLPLR );
   }

   GetIntegerFromAttribute( &lDelAllTables, vDelWork, "TZSIDEWK", "wkAllTables" );

   switch ( lDelAllTables )
   {
      case 0: /* Delete only the selected tables */
         nZRetCode = SetCursorFirstSelectedEntity( vDTE, "TE_TablRec", "" );
         while ( nZRetCode >= zCURSOR_SET )
         {
            // Reconstruct the actual Entity and its ForeignKeys in the ERD
            nRC = zwTZTEUPDD_ReconstructEntitiesAndForeignKeys( vSubtask, vDTE, vEMD );
            if ( nRC )
               return( zCALL_ERROR );

            // Now the changes in the ERD are done. Delete the actual Table from the DataSource.
            nZRetCode = DeleteEntity( vDTE, "TE_TablRec", zREPOS_NONE );
            nZRetCode = SetCursorNextSelectedEntity( vDTE, "TE_TablRec", "" );
         }

         strcpy_s( szWindowName, sizeof( szWindowName ), "TableRecord" );
         strcpy_s( szEntityName, sizeof( szEntityName ), "TE_TablRec" );
         strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "TE and ERD have been changed. Before using function "
                   "'Init Tables' you have to save the TE and the ERD !" );

         break;

      case 1: /* Delete all tables */
           // For Siron Flat Files we have to reconstruct Foreign keys to
           // ER Attributes
           nRC = zwTZTEUPDD_ReconstructER_Attributes( vSubtask, vDTE, vEMD );
           if (nRC != 0)
           {
              strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "ER Attributes cannot be recontructed from Foreign Keys");
              nRC = MessagePrompt( vSubtask, "TE00414", "Physical Data Model",
                                   szWorkMessage, zBEEP, zBUTTONS_OK,
                                   zRESPONSE_OK, zICON_ERROR );
              return( zCALL_ERROR );
           }

           /* Delete all tables from the DataSource */
           nZRetCode = SetCursorFirstEntity( vDTE, "TE_TablRec", "" );
           while ( nZRetCode >= zCURSOR_SET )
           {
               nZRetCode = DeleteEntity( vDTE, "TE_TablRec", zREPOS_NONE );
               nZRetCode = SetCursorFirstEntity( vDTE, "TE_TablRec", "" );
           }

           strcpy_s( szWindowName, sizeof( szWindowName ), "TableRecord" );
           strcpy_s( szEntityName, sizeof( szEntityName ), "TE_TablRec" );
           strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "TE and ERD have been changed. Before using function "
                     "'Init Tables' you have to save the TE and the ERD !" );

           break;

      case 2: /* Delete DataSource */
           // For Siron Flat Files we have to reconstruct Foreign keys to
           // ER Attributes
           nRC = zwTZTEUPDD_ReconstructER_Attributes( vSubtask, vDTE, vEMD );
           if ( nRC )
           {
              strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "ER Attributes cannot be recontructed from Foreign Keys");
              nRC = MessagePrompt( vSubtask, "TE00414", "Physical Data Model",
                                   szWorkMessage, zBEEP, zBUTTONS_OK,
                                   zRESPONSE_OK, zICON_ERROR );
              return( zCALL_ERROR );
           }

           /* Delete the DataSource */
           nZRetCode = DeleteEntity( vDTE, "TE_DBMS_Source", zREPOS_PREV );
           strcpy_s( szWindowName, sizeof( szWindowName ), "DataSrc" );
           strcpy_s( szEntityName, sizeof( szEntityName ), "TE_DBMS_Source" );
           strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "TE and ERD have been changed. Before using any function "
                     "you have to save the TE and the ERD !" );

           break;

      default:
           return( zCALL_ERROR );
   }

   // Set Update Flag and Save ERD and TE
   zwTZTEUPDD_SetERDUpdateFlag( vSubtask, &vTE_Work, vDTE );

   // TE and ERD are changed. Before it is possible to use function
   // 'Init Tables' for Siron catalogs again, the TE and the ERD have
   // to be saved. Give a message to the user and disable the function
   // in the menu.
   nRC = MessagePrompt( vSubtask, "TE00414", "Physical Data Model",
                        szWorkMessage,
                        zBEEP, zBUTTONS_OK, zRESPONSE_OK, zICON_INFORMATION);

   DropView (vDelWork);  /* Drop the WorkView */

   zwTZTEUPDD_RefreshWindows( vSubtask, szWindowName, szEntityName );

   return( 0 );

} // zwTZTEUPDD_DeleteVsamTables

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_WindowDeleteVsam
//
//   Parameter:  ullDelete  = 2  Delete DataSource
//                          = 1  Delete all tables
//                          = 0  Delete selected table
zSHORT /* LOCAL */  OPERATION
zwTZTEUPDD_WindowDeleteVsam( zVIEW vSubtask, zLONG ullDelete )
{
   zSHORT nRC;
   zSHORT nZRetCode;
   zVIEW  TZSIDEWK;

   // We need WorkLod and Workentity to know if all tables should be deleted
   // or only the selected Table.
   nRC = ActivateEmptyObjectInstance( &TZSIDEWK, "TZSIDEWK", vSubtask, zMULTIPLE );
   nZRetCode = SetNameForView( TZSIDEWK, "TZSIDEWK", vSubtask, zLEVEL_TASK );
   CreateEntity( TZSIDEWK, "TZSIDEWK", zPOS_AFTER );

   SetAttributeFromInteger( TZSIDEWK, "TZSIDEWK", "wkAllTables", ullDelete );

   // We open a new window, because the Reference view to the ERD is
   // dropped while closing a subwindow. If we don't close a subwindow
   // we cannot save the ERD.
   nRC = SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessSubwindow,
                                  "TZTEUPDD", "DelTables" );
   return( nRC );

} // zwTZTEUPDD_WindowDeleteVsam

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteDataSource
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DeleteDataSource( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vWindow;
   zSHORT   nCloseWindow = 0;
   zCHAR    szSironDB_Type[ 2 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   GetSubtaskForWindowName( vSubtask, &vWindow, "DataSrc" );
   if ( vWindow )
      nCloseWindow = 1;

   // We need the DB-Type because Siron Catalogs must be handled in a special way
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
   if ( *szSironDB_Type == zType_VSAM )
   {
      return( zwTZTEUPDD_WindowDeleteVsam( vSubtask, 2 ) );
   }

   // No Siron catalogs than any other database
   zwTZTEUPDD_DeleteSubobject( vSubtask, zENAME_DTE_DATASOURCE, "TE_DBMS_Source", nCloseWindow );

   if ( CheckExistenceOfEntity( vDTE, "TE_DBMS_Source" ) >= zCURSOR_SET )
   {
      zwTZTEUPDD_DataSrc_Prebuild( vSubtask );
      zwTZTEUPDD_DS_TemporaryGraying( vSubtask );
   }

   return( 0 );

} // zwTZTEUPDD_DeleteDataSource

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DisableButton
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DisableButton( zVIEW vSubtask )
{
   zVIEW    vTE_Work;
   zSHORT   nEnable;

   GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK );

   nEnable = 0;

   if ( (CheckExistenceOfEntity( vTE_Work, "CandidateSourceKey" ) == zCURSOR_SET) &&
        (CheckExistenceOfEntity( vTE_Work, "TargetKey" ) == zCURSOR_SET) )
   {
      nEnable = 1;
   }

   SetCtrlState( vSubtask, "pbIncludeSourceKey", zCONTROL_STATUS_ENABLED, nEnable );

   return( 0 );

} // zwTZTEUPDD_DisableButton

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_RefreshTableDetail
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_RefreshTableDetail( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vWindow;
   zVIEW    vDTE_Old;
   zVIEW    vProfileXFER;
   zLONG    lnewZKey;
   zVIEW    vMain;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   //if Window TableDetail not open ( is a modeless Window )?
   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   GetSubtaskForWindowName( vSubtask, &vMain, "Main" );

   if ( vWindow &&
        GetViewByName( &vDTE_Old, "TE_DB_Environ_old", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetSelectStateOfEntity( vDTE, "TE_TablRec", 0 );

      if ( CompareAttributeToAttribute( vDTE, "TE_TablRec", "ZKey", vDTE_Old, "TE_TablRec", "ZKey" ) == 0 )
      {
         return( 0 );
      }

      GetIntegerFromAttribute( &lnewZKey, vDTE, "TE_TablRec", "ZKey" );
      if ( SetCursorFirstEntityByEntityCsr( vDTE, "TE_TablRec", vDTE_Old, "TE_TablRec", "" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vProfileXFER, "TE", "Name", "Error" ) != 0 )
            MapWindow( vWindow );

         SetSelectStateOfEntity( vDTE, "TE_TablRec", 0 );
         if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_TablRec", "Table / Record", 1 ) < 0 )
         {
            SetSelectStateOfEntity( vDTE, "TE_TablRec", 1 );
            RefreshCtrl( vMain, "TablRecList" );

            if ( CompareAttributeToString( vProfileXFER, "TE", "Name", "Error" ) != 0 )
               SetAttributeFromString( vProfileXFER, "TE", "Name", "Error" );
            else
               SetAttributeFromString( vProfileXFER, "TE", "Name", "" );

            return( -1 );
         }

         SetAttributeFromString( vProfileXFER, "TE", "Name", "" );
         SetCursorFirstEntityByInteger( vDTE, "TE_TablRec", "ZKey", lnewZKey, "TE_DB_Environ" );

         CreateTemporalSubobjectVersion( vDTE, "TE_TablRec" );
         SetSelectStateOfEntity( vDTE, "TE_TablRec", 1 );

         RefreshWindow( vWindow );
         SetFocusToCtrl( vWindow, "edName" );
      }
   }

   return( 0 );
} // zwTZTEUPDD_RefreshTableDetail


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_LoadTableDetail
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_LoadTableDetail( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   //if Window TableDetail not open ( is a modeless Window )?
   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );

   if ( !vWindow )
   {
      CreateTemporalSubobjectVersion( vDTE, "TE_TablRec" );
      SetSelectStateOfEntity( vDTE, "TE_TablRec", 1 );
   }

   return( 0 );
} // zwTZTEUPDD_LoadTableDetail


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_SaveViewCursor
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_SaveViewCursor( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vDTE_Old;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDTE_Old, "TE_DB_Environ_old", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropView( vDTE_Old );
   }

   CreateViewFromViewForTask( &vDTE_Old, vDTE, 0 );
   SetNameForView( vDTE_Old, "TE_DB_Environ_old", vSubtask, zLEVEL_TASK );

   return( 0 );
} // zwTZTEUPDD_SaveViewCursor


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_AcceptTableRecord
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_AcceptTableRecord( zVIEW vSubtask )
{
   zVIEW   vTE_DB_Environ;

   GetViewByName( &vTE_DB_Environ, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_TablRec", "Table / Record", 1 ) < 0 )
   {
      return( -1 );
   }

   zwTZTEUPDD_RefreshWindows( vSubtask, "TableRecord", "TE_TablRec" );

   return( 0 );
} // zwTZTEUPDD_AcceptTableRecord


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_CancelTableRecord
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_CancelTableRecord( zVIEW vSubtask )
{
   zVIEW   vTE_DB_Environ;
   zVIEW   vProfileXFER;

   GetViewByName( &vTE_DB_Environ, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   CancelSubobject( vTE_DB_Environ, "TE_TablRec" );

   SetAttributeFromString( vProfileXFER, "TE", "Name", "" );

   return( 0 );
} // zwTZTEUPDD_CancelTableRecord


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_NextDataSource
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_NextDataSource( zVIEW vSubtask )
{
   zVIEW   vTE_DB_Environ;
   zVIEW   vWindow;

   GetViewByName( &vTE_DB_Environ, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_DBMS_Source", "Data Source", 1 ) < 0 )
   {
      return( -1 );
   }

   SetCursorNextEntity( vTE_DB_Environ, "TE_DBMS_Source", "" );

   CreateTemporalSubobjectVersion( vTE_DB_Environ, "TE_DBMS_Source" );
   zwTZTEUPDD_DataSrc_Prebuild( vSubtask );
   zwTZTEUPDD_DS_TemporaryGraying( vSubtask );

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZTEUPDD_NextDataSource


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_PrevDataSource
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_PrevDataSource( zVIEW vSubtask )
{
   zVIEW   vTE_DB_Environ;
   zVIEW   vWindow;

   GetViewByName( &vTE_DB_Environ, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_DBMS_Source", "Data Source", 1 ) < 0 )
   {
      return( -1 );
   }

   SetCursorPrevEntity( vTE_DB_Environ, "TE_DBMS_Source", "" );

   CreateTemporalSubobjectVersion( vTE_DB_Environ, "TE_DBMS_Source" );
   zwTZTEUPDD_DataSrc_Prebuild( vSubtask );
   zwTZTEUPDD_DS_TemporaryGraying( vSubtask );

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZTEUPDD_PrevDataSource


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteSelectedColumn
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DeleteSelectedColumn( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vWindow;
   zSHORT   nRC;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "OK to delete selected Columns/Fields" );
   nRC = MessagePrompt( vSubtask, "TE00414", "Physical Data Model",
                        szWorkMessage,
                        zBEEP, zBUTTONS_YESNO,
                        zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      // Delete the selected Column
      for ( nRC = SetCursorFirstSelectedEntity( vDTE, "TE_FieldDataRel", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vDTE, "TE_FieldDataRel", "" ) )
      {
         // set new field sequence
         oTZTENVRO_DelFieldFromSequence( vDTE );

         DeleteEntity( vDTE, "TE_FieldDataRel", zREPOS_NONE );
      }

      //Refresh Listbox lbColumnList in Window TableRecord
      GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
      if ( vWindow )
         RefreshCtrl( vWindow, "lbColumnList" );
   }

   return( 0 );
} // zwTZTEUPDD_DeleteSelectedColumn


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteSelectedKeys
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_DeleteSelectedKeys( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zSHORT   nRC;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );

   strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "OK to delete selected Keys" );
   nRC = MessagePrompt( vSubtask, "TE00414", "Physical Data Model",
                        szWorkMessage,
                        zBEEP, zBUTTONS_YESNO,
                        zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      // Delete the selected Column
      for ( nRC = SetCursorFirstSelectedEntity( vDTE, "TE_TablRecKey", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vDTE, "TE_TablRecKey", "" ) )
      {
         DeleteEntity( vDTE, "TE_TablRecKey", zREPOS_NONE );
      }

      // Set Key Flags for TE_FieldDataRel
      oTZTENVRO_SetFieldDataRelKeyFlag( vDTE );
   }

   return( 0 );
} // zwTZTEUPDD_DeleteSelectedKeys


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_InitMenuDisableAll
//
static zSHORT
zwTZTEUPDD_InitMenuDisableAll( zVIEW vSubtask )
{
   SetOptionState( vSubtask, "NewDataSource", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddKey", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateTable/Record", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateKey", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateColumn/Field", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "GoToForKey", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteTable/Record", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteKey", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteColumn/Field", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAll", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "BuildTablRec", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "SyncRelationships", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "GenerateDDL", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "SyncDDL", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "ImportData", zOPTION_STATUS_ENABLED, 0 );

   return( 0 );
} // zwTZTEUPDD_InitMenuDisableAll


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_InitMenuForVSAM
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_InitMenuForVSAM( zVIEW  vSubtask,
                            zVIEW  vDTE,
                            zSHORT nIsCheckedOut )
{
   zVIEW  vTE_Work;
   zCHAR  szER_UpdateFlag[ 2 ];

   if ( nIsCheckedOut == 1 )
   {
      SetOptionState( vSubtask, "BuildTablRec", zOPTION_STATUS_ENABLED, 1 );

      if ( CheckExistenceOfEntity( vDTE, "TE_TablRec" ) >= zCURSOR_SET )
         SetOptionState( vSubtask, "SyncRelationships", zOPTION_STATUS_ENABLED, 1 );

      if ( GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         GetStringFromAttribute( szER_UpdateFlag, sizeof( szER_UpdateFlag ), vTE_Work, "TE_Work", "ER_UpdateFlag" );

         if ( szER_UpdateFlag[ 0 ] == 'Y' )
         {
            SetOptionState( vSubtask, "BuildTablRec", zOPTION_STATUS_ENABLED, 0 );
            SetOptionState( vSubtask, "SyncRelationships", zOPTION_STATUS_ENABLED, 0 );
         }
      }
   }

   return( 0 );
} // zwTZTEUPDD_InitMenuForVSAM


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_InitMenuForTableWnd
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_InitMenuForTableWnd( zVIEW  vSubtask,
                                zVIEW  vDTE,
                                zSHORT nIsCheckedOut,
                                zPCHAR szSironDB_Type )
{
   if ( nIsCheckedOut == 1 )
   {
      // Disable the New Column/Key option if the DBMS Source is NONE
      if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS", "" ) != 0  &&
           CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS", "None" ) != 0 &&
           *szSironDB_Type != zType_SAP )
      {
         if ( CheckExistenceOfEntity( vDTE, "TE_FieldDataRel" ) >= zCURSOR_SET )
            SetOptionState( vSubtask, "AddKey", zOPTION_STATUS_ENABLED, 1 );
      }
   }

   if ( CheckExistenceOfEntity( vDTE, "TE_TablRecKey" ) >= zCURSOR_SET  &&
        SetCursorFirstSelectedEntity( vDTE, "TE_TablRecKey", "" ) >= zCURSOR_SET )
   {
      SetOptionState( vSubtask, "UpdateKey", zOPTION_STATUS_ENABLED, 1 );

      if ( nIsCheckedOut == 1 && *szSironDB_Type != zType_SAP )
         SetOptionState( vSubtask, "DeleteKey", zOPTION_STATUS_ENABLED, 1 );
   }

   if ( CheckExistenceOfEntity( vDTE, "TE_FieldDataRel" ) >= zCURSOR_SET  &&
        SetCursorFirstSelectedEntity( vDTE, "TE_FieldDataRel", "" ) >= zCURSOR_SET )
   {
      SetOptionState( vSubtask, "UpdateColumn/Field", zOPTION_STATUS_ENABLED, 1 );

      if ( nIsCheckedOut == 1 && *szSironDB_Type != zType_SAP )
         SetOptionState( vSubtask, "DeleteColumn/Field", zOPTION_STATUS_ENABLED, 1 );
   }

   return( 0 );
} // zwTZTEUPDD_InitMenuForTableWnd



////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_InitMenuEdit
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_InitMenuEdit( zVIEW  vSubtask,
                         zVIEW  vWindow,
                         zVIEW  vDTE,
                         zSHORT nIsCheckedOut,
                         zPCHAR szSironDB_Type )
{
   if ( nIsCheckedOut == 1 )
   {
      SetOptionState( vSubtask, "NewDataSource", zOPTION_STATUS_ENABLED, 1 );
      if ( CheckExistenceOfEntity( vDTE, "TE_DBMS_Source" ) >= zCURSOR_SET )
         SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 1 );

      if ( CheckExistenceOfEntity( vDTE, "TE_TablRec" ) >= zCURSOR_SET &&
           *szSironDB_Type != zType_SAP )
      {
         SetOptionState( vSubtask, "DeleteAll", zOPTION_STATUS_ENABLED, 1 );
         if ( SetCursorFirstSelectedEntity( vDTE, "TE_TablRec", "" ) >= zCURSOR_SET &&
              vWindow == 0 )
         {
            SetOptionState( vSubtask, "DeleteTable/Record", zOPTION_STATUS_ENABLED, 1 );
            SetOptionState( vSubtask, "GoToForKey", zOPTION_STATUS_ENABLED, 1 );
         }
      }
   }

   if ( SetCursorFirstSelectedEntity( vDTE, "TE_TablRec", "" ) >= zCURSOR_SET )
      SetOptionState( vSubtask, "UpdateTable/Record", zOPTION_STATUS_ENABLED, 1 );

   // if window "Tables/Records" open
   if ( vWindow )
      zwTZTEUPDD_InitMenuForTableWnd( vSubtask, vDTE, nIsCheckedOut, szSironDB_Type );

   return( 0 );

} // zwTZTEUPDD_InitMenuEdit


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_InitMenu
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_InitMenu( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vLPLR;
   zVIEW  vWindow;
   zSHORT nIsCheckedOut = 0;
   zCHAR  szSironDB_Type[ 2 ];

   zwTZTEUPDD_InitMenuDisableAll( vSubtask );

   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) < 0 )
   {
      if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
         SetOptionState( vSubtask, "NewDataSource", zOPTION_STATUS_ENABLED, 0 );
      else
         SetOptionState( vSubtask, "NewDataSource", zOPTION_STATUS_ENABLED, 1 );

      return( 0 );
   }

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META );

   if ( CheckExistenceOfEntity( vDTE, "TE_DBMS_Source" ) >= zCURSOR_SET )
      GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
   else
      szSironDB_Type[ 0 ] = 0;

   zwTZTEUPDD_InitMenuEdit( vSubtask, vWindow, vDTE, nIsCheckedOut, szSironDB_Type );

   // Disable the Init options for Siron flat files.
   if ( CheckExistenceOfEntity( vDTE, "TE_DBMS_Source" ) < zCURSOR_SET )
      return( 0 );

   if ( *szSironDB_Type == zType_VSAM )
   {
      zwTZTEUPDD_InitMenuForVSAM( vSubtask, vDTE, nIsCheckedOut );
   }
   else
   {
      if ( nIsCheckedOut == 1 && *szSironDB_Type != zType_SAP )
         SetOptionState( vSubtask, "BuildTablRec", zOPTION_STATUS_ENABLED, 1 );

      if ( CheckExistenceOfEntity( vDTE, "TE_TablRec" ) >= zCURSOR_SET )
      {
         SetOptionState( vSubtask, "GenerateDDL", zOPTION_STATUS_ENABLED, 1 );
         EnableAction( vSubtask, "GenerateDDL", 1 );
         // if not TB DB Handler, enable Build Synch DDL
         if ( !*szSironDB_Type )
            SetOptionState( vSubtask, "SyncDDL", zOPTION_STATUS_ENABLED, 1 );
      }

      // if the DBMS Source from type network or without type or from type SAP R3,
      // disable the Gen DDL option
      if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS", "Network Server" ) == 0 ||
           CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS", "" ) == 0 ||
           CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS", "None" ) == 0 ||
           *szSironDB_Type == zType_SAP )
      {
         SetOptionState( vSubtask, "GenerateDDL", zOPTION_STATUS_ENABLED, 0 );
      }
   }

   return( 0 );

} // zwTZTEUPDD_InitMenu



////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_SetForeignKeyCaption
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_SetForeignKeyCaption( zVIEW vSubtask )
{
   zCHAR  szTableName[ 33 ];
   zVIEW  vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTableName, sizeof( szTableName ), vDTE, "TE_TablRec", "Name" );
   SetWindowCaptionTitle( vSubtask, 0, szTableName );

   return( 0 );

} //zwTZTEUPDD_SetForeignKeyCaption

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_KeepPhysCharField
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_KeepPhysCharField( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zSHORT nRC;
   zCHAR  szPhysChar[ 2 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   // Attribute is mapped to menu item.
   // Through multi selection it will be changed the last record only.
   // Is was a Bug. Please don't change this code.
   if ( CompareAttributeToString( vDTE, "TE_FieldDataRel", "KeepPhysicalCharacteristicsFlag", "Y" ) == 0 )
   {
      strcpy_s( szPhysChar, sizeof( szPhysChar ), "Y" );
   }
   else
   {
      strcpy_s( szPhysChar, sizeof( szPhysChar ), "" );
   }

   // For each selected field, set the KeepPhysChars attribute to Y.
   for ( nRC = SetCursorFirstSelectedEntity( vDTE, "TE_FieldDataRel", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDTE, "TE_FieldDataRel", 0 ) )
   {
      SetAttributeFromString( vDTE, "TE_FieldDataRel", "KeepPhysicalCharacteristicsFlag", szPhysChar );
   }

   return( 0 );

} // zwTZTEUPDD_KeepPhysCharField

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_KeepPhysCharTable
//
zOPER_EXPORT zSHORT OPERATION
zwTZTEUPDD_KeepPhysCharTable( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zSHORT nRC;
   zCHAR  szPhysChar[ 2 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   // Attribute is mapped to menu item.
   // Through multi selection it will be changed the last record only.
   // Is was a Bug. Please do'nt changed this code.
   if ( CompareAttributeToString( vDTE, "TE_TablRec", "KeepPhysicalCharacteristicsFlag", "Y" ) == 0 )
   {
      strcpy_s( szPhysChar, sizeof( szPhysChar ), "Y" );
   }
   else
   {
      strcpy_s( szPhysChar, sizeof( szPhysChar ), "" );
   }

   // For each selected field, set the KeepPhysChars attribute to Y.
   for ( nRC = SetCursorFirstSelectedEntity( vDTE, "TE_TablRec", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDTE, "TE_TablRec", 0 ) )
   {
      SetAttributeFromString( vDTE, "TE_TablRec", "KeepPhysicalCharacteristicsFlag", szPhysChar );
   }

   return( 0 );

} // zwTZTEUPDD_KeepPhysCharTable

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_ReconstructEntitiesAndForeignKeys
//
zSHORT /* DIALOG */  OPERATION
zwTZTEUPDD_ReconstructEntitiesAndForeignKeys( zVIEW vSubtask,
                                              zVIEW vDTE, zVIEW vEMD )
{
   // Reconstruct the actual Entity in the ERD.
   zSHORT nRC = zwTZTEUPDD_ReconstructSelectedEntity( vSubtask, vDTE, vEMD );
   if ( nRC )
   {
      strcpy_s( szWorkMessage, sizeof( szWorkMessage ), "ER Attributes cannot be recontructed from Foreign Keys");
      nRC = MessagePrompt( vSubtask, "TE00414", "Physical Data Model",
                           szWorkMessage, zBEEP, zBUTTONS_OK,
                           zRESPONSE_OK, zICON_ERROR);
      DropView( vEMD );
      nRC = zCALL_ERROR;
   }
   else
   {
      // Proof: Is the key attribute of the actual Entity a Foreign key in
      // another Entity, then reconstuct the datafield for this entity.
      nRC = zwTZTEUPDD_ReconstructERForForeignKey( vSubtask, vDTE, vEMD );
      if ( nRC )
      {
         strcpy_s( szWorkMessage, sizeof( szWorkMessage ),
                  "ER Attributes cannot be recontructed from Foreign Keys" );
         nRC = MessagePrompt( vSubtask, "TE00414",
                              "Physical Data Model",
                              szWorkMessage, zBEEP, zBUTTONS_OK,
                              zRESPONSE_OK, zICON_ERROR );
         DropView( vEMD );
         nRC = zCALL_ERROR;
      }
   }

   return( nRC );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_NextTable
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_NextTable( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vParentWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_TablRec", "Table / Record", 1 ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vDTE, "TE_TablRec", 0 );

   SetCursorNextEntity( vDTE, "TE_TablRec", "" );
   CreateTemporalSubobjectVersion( vDTE, "TE_TablRec" );

   SetSelectStateOfEntity( vDTE, "TE_TablRec", 1 );
   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZTEUPDD_NextTable


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_PrevTable
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_PrevTable( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vParentWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_TablRec", "Table / Record", 1 ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vDTE, "TE_TablRec", 0 );

   SetCursorPrevEntity( vDTE, "TE_TablRec", "" );
   CreateTemporalSubobjectVersion( vDTE, "TE_TablRec" );

   SetSelectStateOfEntity( vDTE, "TE_TablRec", 1 );
   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZTEUPDD_PrevTable


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteTable
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_DeleteTable( zVIEW vSubtask )
{
   zVIEW    vProfileXFER;
   zVIEW    vDTE;
   zVIEW    vDTE_Del;
   zVIEW    vWindow;
   zSHORT   nZRetCode;
   zSHORT   nRC;
   zSHORT   RESULT;
   zCHAR    szSironDB_Type[ 2 ];
   zLONG    lEntityKey;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_ANY );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );

   // We need the DB-Type because Siron Catalogs must be handled in a special way
   GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");
   if ( *szSironDB_Type == zType_VSAM )
   {
      SetAttributeFromString( vProfileXFER, "TE", "Name", "" );
      return( zwTZTEUPDD_WindowDeleteVsam( vSubtask, 0 ));
   }

   // No Siron catalogs than any other database
   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DTE,
                       zENAME_DTE_TABLE,
                       vDTE, "TE_TablRec", "Name", 0 );

   if ( nRC != zRESPONSE_YES )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   // Create a new view to delete the foreign keys
   CreateViewFromViewForTask(&vDTE_Del, vDTE, 0 );
   SetNameForView( vDTE_Del, "vDTE_Del", vSubtask, zLEVEL_TASK );

   // Get the ZKEY from the actual ER_Entity
   GetIntegerFromAttribute (  &lEntityKey, vDTE, "ER_Entity", "ZKey");

   // First we have to delete the foreign keys from the related entities
   // Search the actual foreign key for all entities
   for ( nRC = SetCursorFirstEntity( vDTE_Del, "TE_FieldDataRel", "TE_DBMS_Source" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDTE_Del, "TE_FieldDataRel", "TE_DBMS_Source" ) )
   {
       RESULT = SetCursorFirstEntityByInteger (vDTE_Del, "ER_EntityRelLink", "ZKey", lEntityKey, "");
       if (RESULT == zCURSOR_SET)
       {
         // Here we found a foreign key and we delete it
         nZRetCode = DeleteEntity( vDTE_Del, "TE_FieldDataRel", zREPOS_NONE );
       }
   }

   // Delete now the actual Entity from the DataSource
   nZRetCode = DeleteEntity( vDTE, "TE_TablRec", zREPOS_NONE );

   DropView (vDTE_Del);

   SetCursorPrevEntity( vDTE, "TE_TablRec", "" );

   if ( CheckExistenceOfEntity( vDTE, "TE_TablRec" ) == zCURSOR_NULL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh | zWAB_ProcessImmediateAction, 0, 0 );
   }
   else
   {
      SetSelectStateOfEntity( vDTE, "TE_TablRec", 1 );
      SetFocusToCtrl( vSubtask, "edName" );
   }

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );

   SetAttributeFromString( vProfileXFER, "TE", "Name", "" );

   return( 0 );
} // zwTZTEUPDD_DeleteTable



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteField
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_DeleteField( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   strcpy_s( szWorkMessage, sizeof( szWorkMessage ),
            "OK to continue with Delete of Field Date / Relationship ?");
   if ( MessagePrompt( vSubtask, "TE00433", "Physical Data Model",
                       szWorkMessage,
                       zBEEP,        zBUTTONS_YESNO,
                       zRESPONSE_NO, zICON_QUESTION ) != zRESPONSE_YES )
   {
      return( 0 );
   }

   // set new field sequence
   oTZTENVRO_DelFieldFromSequence( vDTE );

   DeleteEntity( vDTE, "TE_FieldDataRel", zREPOS_NONE );

   SetCursorPrevEntity( vDTE, "TE_FieldDataRel", "" );

   if ( CheckExistenceOfEntity( vDTE, "TE_FieldDataRel" ) < zCURSOR_SET )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   }
   else
   {
      FieldRelLink_SetDB_DataTypes( vSubtask );
      SetSelectStateOfEntity( vDTE, "TE_FieldDataRel", 1 );
      SetFocusToCtrl( vSubtask, "edName" );
   }

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      RefreshCtrl( vWindow, "lbColumnList" );

   return( 0 );
} // zwTZTEUPDD_DeleteField


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_SaveSelectedField
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_SaveSelectedField( zVIEW vSubtask )
{
   zVIEW  vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ))
   {
      SetAttributeFromAttribute( vDTE, "TE_TablRec", "wkSelectedFieldForDragSort",
                                 vDTE, "TE_FieldDataRel", "wkSequence" );
   }

   return( 0 );
} // zwTZTEUPDD_SaveSelectedField


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_MoveField
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_MoveField( zVIEW vSubtask )
{
   zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   oTZTENVRO_MoveFieldInSequence( vDTE );

   return( 0 );
} // zwTZTEUPDD_MoveField



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_LoadPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_LoadPopup( zVIEW  vSubtask,
                      zPCHAR szEntityName,
                      zPCHAR szActionNameForNew,
                      zPCHAR szActionNameForDelete,
                      zPCHAR szPopupName )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vDTE;
   zCHAR  szSironDB_Type[ 2 ];
   zBOOL  bNew        = FALSE;
   zBOOL  bDelete     = FALSE;
   zSHORT nComponentIsCheckedOut = 0;

   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nComponentIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META );
      GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");

      if ( CheckExistenceOfEntity( vDTE, szEntityName ) >= zCURSOR_SET )
      {
         if ( nComponentIsCheckedOut == 1 && *szSironDB_Type != zType_SAP )
            bDelete = TRUE;
      }

      if ( nComponentIsCheckedOut == 1 )
      {
         if ( *szSironDB_Type != zType_SAP )
            bNew = TRUE;

         if ( zstrcmp( szPopupName, "Key" ) == 0 &&
              CheckExistenceOfEntity( vDTE, "TE_FieldDataRel" ) < zCURSOR_SET )
         {
             bNew = FALSE;
         }
         // Disable the New Column option if the DBMS Source is NONE
         if ( (CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS", "" ) == 0  ||
               CompareAttributeToString( vDTE, "TE_DBMS_Source", "DBMS", "None" ) == 0) &&
              (zstrcmp( szPopupName, "Key" ) == 0 ||
               zstrcmp( szPopupName, "ColumnField" ) == 0) )
         {
            bNew = FALSE;
         }
      }

      if ( zstrcmp( szActionNameForNew, "" ) != 0 )
         EnableAction( vSubtask, szActionNameForNew, bNew );

      EnableAction( vSubtask, szActionNameForDelete, bDelete );

      CreateTrackingPopupMenu( vSubtask, szPopupName, pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // zwTZTEUPDD_LoadPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteSubobject
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_DeleteSubobject( zVIEW vSubtask,
                            zSHORT nObject,
                            zPCHAR szEntityName,
                            zSHORT nCloseWindow )
{
   zVIEW  vDTE;
   zVIEW  vWindow;
   zSHORT nRC;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DTE, nObject, vDTE, szEntityName, "Name", 0 );

   if ( nRC == zRESPONSE_YES )
   {
      nRC = DeleteEntity( vDTE, szEntityName, zREPOS_NONE );
      SetCursorPrevEntity( vDTE, szEntityName, "" );

      if ( nCloseWindow == 1 &&
           CheckExistenceOfEntity( vDTE, szEntityName ) == zCURSOR_NULL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh | zWAB_ProcessImmediateAction, 0, 0 );
      }
      else
      {
         SetSelectStateOfEntity( vDTE, szEntityName, 1 );
         SetFocusToCtrl( vSubtask, "edName" );
      }

      GetParentWindow( &vWindow, vSubtask );
      RefreshWindow( vWindow );
   }

   return( 0 );
} // zwTZTEUPDD_DeleteSubobject



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_RefreshWindows
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_RefreshWindows( zVIEW  vSubtask,
                           zPCHAR szWindowName,
                           zPCHAR szEntityName )
{
   zSHORT   nCloseWindow = 0;
   zVIEW    vWindow;
   zVIEW    vParentWindow;
   zVIEW    vDTE;

   if ( zstrcmp( szWindowName, "" ) == 0 || zstrcmp( szEntityName, "" ) == 0 )
      return( 0 );

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );
   GetSubtaskForWindowName( vSubtask, &vWindow, szWindowName );

   if ( vWindow )
   {
      nCloseWindow = 1;
   }

   if ( nCloseWindow == 1 &&
        CheckExistenceOfEntity( vDTE, szEntityName ) == zCURSOR_NULL )
   {
      SetWindowActionBehavior( vWindow, zWAB_ReturnToParentWithRefresh | zWAB_ProcessImmediateAction, 0, 0 );
   }
   else
   {
      RefreshWindow( vWindow );
      SetFocusToCtrl( vWindow, "edName" );
   }

   if ( zstrcmp( szWindowName, "TableRecord" ) == 0 )
   {
      GetParentWindow( &vParentWindow, vWindow );
      if ( vParentWindow )
      {
         SetSelectStateOfEntity( vDTE, szEntityName, 1 );
         RefreshWindow( vParentWindow );
      }
   }

   return( 0 );
} // zwTZTEUPDD_RefreshWindows


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_AcceptSubobject
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_AcceptSubobject( zVIEW  vSubtask,
                            zPCHAR szEntityName,
                            zPCHAR szMessageText,
                            zSHORT bCheck )
{
   zVIEW    vDTE;
   zVIEW    vDTECopy;
   zCHAR    szNewName[ 256 ];
   zCHAR    szMessage[ 100 ];
   zSHORT   nRC;
   zVIEW    vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_ANY );
   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szNewName, sizeof( szNewName ), vDTE, szEntityName, "Name" );
   zGetStringWithoutBlank( szNewName, szNewName );

   if ( zstrcmp( szNewName, "") == 0 )
   {
      strcpy_s( szMessage, sizeof( szMessage ), szMessageText );
      strcat_s( szMessage, sizeof( szMessage ), " Name is required.");
      if ( CompareAttributeToString( vProfileXFER, "TE", "Name", "Error" ) != 0 )
      {
         MessageSend( vSubtask, "PE00114", "Physical Data Model",
                      szMessage,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }

      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edName" );
      return( -1 );
   }
   else
   {
     if (ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_ERD_META ))
         SetAttributeFromString( vDTE, szEntityName, "Name", szNewName );
   }

   if ( bCheck == 1 )
   {
      CreateViewFromViewForTask( &vDTECopy, vDTE, 0 );
      nRC = SetCursorFirstEntityByString( vDTECopy, szEntityName, "Name", szNewName, "" );
      nRC = SetCursorNextEntityByString( vDTECopy, szEntityName, "Name", szNewName, "" );
      DropView( vDTECopy );

      if ( nRC == zCURSOR_SET )
      {
         strcpy_s( szMessage, sizeof( szMessage ), "A " );
         strcat_s( szMessage, sizeof( szMessage ), szMessageText );
         strcat_s( szMessage, sizeof( szMessage ), " already exists by this name!" );
         if ( CompareAttributeToString( vProfileXFER, "TE", "Name", "Error" ) != 0 )
         {
            MessageSend( vSubtask, "TE00433", "Physical Data Model",
                         szMessage,
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }

         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edName" );
         return( -1 );
      }
   }

   if ( AcceptSubobject( vDTE, szEntityName ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
} // zwTZTEUPDD_AcceptSubobject


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_MainTableRecPopup
//
//
//    *szSironDB_Type == zType_VSAM  = Siron Flat Files
//    *szSironDB_Type == zType_SAP   = SAP Data Source
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_MainTableRecPopup( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vTE_Work;
   zVIEW  vWindow;
   zCHAR  szSironDB_Type[ 2 ];
   zCHAR  szER_UpdateFlag[ 2 ];
   zBOOL  bOpen       = FALSE;
   zBOOL  bDelete     = FALSE;
   zBOOL  bSetTableKeepPhysChars   = FALSE;
   zBOOL  bSetColumnsKeepPhysChars = FALSE;
   zBOOL  bBuildTablRec            = FALSE;
   zBOOL  bSyncRelationships       = FALSE;
   zSHORT nComponentIsCheckedOut   = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      return( 0 );

   if ( GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) > 0 &&
        CheckExistenceOfEntity( vDTE, "TE_DBMS_Source" ) >= zCURSOR_SET )
   {
      nComponentIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META );

      GetStringFromAttribute( szSironDB_Type, sizeof( szSironDB_Type ), vDTE, "TE_DBMS_Source", "SironDB_Type");

      if ( CheckExistenceOfEntity( vDTE, "TE_TablRec" ) >= zCURSOR_SET )
      {
         bOpen = TRUE;
         if ( nComponentIsCheckedOut == 1 && *szSironDB_Type != zType_SAP )
            bDelete = TRUE;
      }

      if ( nComponentIsCheckedOut == 1 )
      {
         if ( *szSironDB_Type == zType_VSAM )
         {
            bBuildTablRec = TRUE;
            if ( CheckExistenceOfEntity( vDTE, "TE_TablRec" ) >= zCURSOR_SET )
               bSyncRelationships = TRUE;

            if ( GetViewByName( &vTE_Work, "TZTEWRKO", vSubtask, zLEVEL_TASK ) >= 0 )
            {
               GetStringFromAttribute( szER_UpdateFlag, sizeof( szER_UpdateFlag ), vTE_Work, "TE_Work", "ER_UpdateFlag" );
               if ( szER_UpdateFlag[ 0 ] == 'Y' )
               {
                  bBuildTablRec = FALSE;
                  bSyncRelationships = FALSE;
               }
            }
         }
         else
         if ( *szSironDB_Type != zType_SAP )
         {
            bBuildTablRec = TRUE;

            if ( CheckExistenceOfEntity( vDTE, "TE_TablRec" ) >= zCURSOR_SET )
               bSetTableKeepPhysChars = TRUE;

            if ( CheckExistenceOfEntity( vDTE, "TE_FieldDataRel" ) >= zCURSOR_SET )
               bSetColumnsKeepPhysChars = TRUE;
         }
      }

      EnableAction( vSubtask, "GoToForKeyList", bDelete );
      EnableAction( vSubtask, "DeleteTablRec", bDelete );
      EnableAction( vSubtask, "DeleteAllTablRecs", bDelete );
      EnableAction( vSubtask, "SetTableKeepPhysChars", bSetTableKeepPhysChars );
      EnableAction( vSubtask, "SetAllColumnKeepPhysChars", bSetColumnsKeepPhysChars );
      EnableAction( vSubtask, "RebuildTablesRelationships", bBuildTablRec );
      EnableAction( vSubtask, "SyncRelationship", bSyncRelationships );

      CreateTrackingPopupMenu( vSubtask, "TableRec", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // zwTZTEUPDD_MainTableRecPopup



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_AcceptFieldRS
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_AcceptFieldRS( zVIEW vSubtask )
{
   zVIEW   vWindow;
   zVIEW   vTE_DB_Environ;

   GetViewByName( &vTE_DB_Environ, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_FieldDataRel", "Field Data / Relationship", 0 ) < 0 )
   {
      return( -1 );
   }

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      RefreshCtrl( vWindow, "lbColumnList" );

   return( 0 );
} // zwTZTEUPDD_AcceptFieldRS


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_AcceptKeyData
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_AcceptKeyData( zVIEW vSubtask )
{
   zVIEW   vWindow;
   zVIEW   vTE_DB_Environ;

   GetViewByName( &vTE_DB_Environ, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_TablRecKey", "Key", 1 ) < 0 )
   {
      return( -1 );
   }

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      RefreshCtrl( vWindow, "lbKeyList" );

   return( 0 );
} // zwTZTEUPDD_AcceptKeyData


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_NextKey
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_NextKey( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_TablRecKey", "Key", 1 ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vDTE, "TE_TablRecKey", 0 );

   SetCursorNextEntity( vDTE, "TE_TablRecKey", "" );
   CreateTemporalSubobjectVersion( vDTE, "TE_TablRecKey" );

   SetSelectStateOfEntity( vDTE, "TE_TablRecKey", 1 );
   zwTZTEUPDD_KeyListPostbuild( vSubtask );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" ) >= 0 )
      RefreshWindow( vWindow );

   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZTEUPDD_NextKey


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_PrevKey
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_PrevKey( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_AcceptSubobject( vSubtask, "TE_TablRecKey", "Key", 1 ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vDTE, "TE_TablRecKey", 0 );

   SetCursorPrevEntity( vDTE, "TE_TablRecKey", "" );
   CreateTemporalSubobjectVersion( vDTE, "TE_TablRecKey" );

   SetSelectStateOfEntity( vDTE, "TE_TablRecKey", 1 );
   zwTZTEUPDD_KeyListPostbuild( vSubtask );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" ) >= 0 )
      RefreshWindow( vWindow );

   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZTEUPDD_PrevKey


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DeleteKey
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_DeleteKey( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vWindow;
   zSHORT nRC;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DTE,
                       zENAME_DTE_KEY,
                       vDTE, "TE_TablRecKey", "Name", 0 );

   if ( nRC == zRESPONSE_YES )
   {
      nRC = DeleteEntity( vDTE, "TE_TablRecKey", zREPOS_NONE );
      SetCursorPrevEntity( vDTE, "TE_TablRecKey", "" );

      // Set Key Flags for TE_FieldDataRel
      oTZTENVRO_SetFieldDataRelKeyFlag( vDTE );

      if ( CheckExistenceOfEntity( vDTE, "TE_TablRecKey" ) == zCURSOR_NULL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
      }
      else
      {
         CreateTemporalSubobjectVersion( vDTE, "TE_TablRecKey" );
         SetSelectStateOfEntity( vDTE, "TE_TablRecKey", 1 );
         zwTZTEUPDD_KeyListPostbuild( vSubtask );
         SetFocusToCtrl( vSubtask, "edName" );
      }

      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" ) >= 0 )
         RefreshWindow( vWindow );
   }

   return( 0 );
} // zwTZTEUPDD_DeleteKey


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_CancelKeyData
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_CancelKeyData( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   CancelSubobject( vDTE, "TE_TablRecKey" );

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      RefreshCtrl( vWindow, "lbKeyList" );

   return( 0 );
} // zwTZTEUPDD_CancelKeyData


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_CancelField
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_CancelField( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   CancelSubobject( vDTE, "TE_FieldDataRel" );

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
      RefreshCtrl( vWindow, "lbColumnList" );

   return( 0 );
} // zwTZTEUPDD_CancelField


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_CloseTablRecordWindow
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_CloseTablRecordWindow( zVIEW vSubtask )
{
   zVIEW vWindow;

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow )
   {
      MapWindow( vWindow );
      if ( zwTZTEUPDD_AcceptSubobject( vWindow, "TE_TablRec", "Table / Record", 1 ) < 0 )
      {
         SetFocusToCtrl( vWindow, "edName" );
         RefreshWindow( vWindow );
         return( -1 );
      }
      else
      {
         SetWindowActionBehavior( vWindow, zWAB_ReturnToTopWindow |
                                  zWAB_ProcessImmediateAction |
                                  zWAB_ProcessImmediateReturn, 0, 0 );
      }
   }

   return( 0 );
} // zwTZTEUPDD_CloseTablRecordWindow


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DataSrcDetail
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_DataSrcDetail( zVIEW vSubtask )
{
   zVIEW    vDTE;
   zVIEW    vWindow;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );

   if ( !vWindow )
   {
      CreateTemporalSubobjectVersion( vDTE, "TE_DBMS_Source" );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
   }

   return( 0 );
} // zwTZTEUPDD_DataSrcDetail



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DataSrcSingleClick
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_DataSrcSingleClick( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vDTE;
   zVIEW  vDTE_Old;

   //if window TableRecord open
   GetSubtaskForWindowName( vSubtask, &vWindow, "TableRecord" );
   if ( vWindow &&
        GetViewByName( &vDTE_Old, "TE_DB_Environ_old", vSubtask, zLEVEL_TASK ) > 0 &&
        GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetSelectStateOfEntity( vDTE, "TE_DBMS_Source", 0 );
      SetCursorFirstEntityByEntityCsr( vDTE, "TE_DBMS_Source", vDTE_Old, "TE_DBMS_Source", "TE_DB_Environ" );
      SetCursorFirstEntityByEntityCsr( vDTE, "TE_TablRec", vDTE_Old, "TE_TablRec", "" );
      SetSelectStateOfEntity( vDTE, "TE_DBMS_Source", 1 );
      RefreshCtrl( vSubtask, "TE_DBMS_Source" );
   }

   if ( vWindow == 0 )
      RefreshWindow( vSubtask );

   return( 0 );
} // zwTZTEUPDD_DataSrcSingleClick


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DataSrcDetailMenu
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_DataSrcDetailMenu( zVIEW vSubtask )
{
   zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( zwTZTEUPDD_CloseTablRecordWindow( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      return( -1 );
   }

   CreateTemporalSubobjectVersion( vDTE, "TE_DBMS_Source" );

   return( 0 );
} // zwTZTEUPDD_DataSrcDetailMenu


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_ForeignKeyPostbuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_ForeignKeyPostbuild( zVIEW vSubtask )
{
   zVIEW    vDTE;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( ComponentIsCheckedOut( vSubtask, vDTE, zSOURCE_DTE_META ))
      SetCtrlState( vSubtask, "pbSwitchForeignKey", zCONTROL_STATUS_ENABLED, 1 );
   else
      SetCtrlState( vSubtask, "pbSwitchForeignKey", zCONTROL_STATUS_ENABLED, 0 );

   return( 0 );
} // zwTZTEUPDD_ForeignKeyPostbuild


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_RemoveMultiSelection
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZTEUPDD_RemoveMultiSelection( zVIEW vSubtask,
                                 zPCHAR szEntityName,
                                 zPCHAR szWindowName )
{
   zVIEW  vDTE;
   zVIEW  vDTECopy;
   zVIEW  vWindow;
   zSHORT nRC;

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDTECopy, vDTE, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vDTE, szEntityName, "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDTE, szEntityName, "" ) )
   {
      nRC =SetSelectStateOfEntity( vDTE, szEntityName, 0 );
   }

   SetCursorFirstEntityByAttr( vDTE, szEntityName, "ZKey", vDTECopy, szEntityName, "ZKey", "" );
   DropView( vDTECopy );

   SetSelectStateOfEntity( vDTE, szEntityName, 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, szWindowName ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
} // zwTZTEUPDD_RemoveMultiSelection


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_KeyListPrebuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_KeyListPrebuild( zVIEW vSubtask )
{
   zwTZTEUPDD_RemoveMultiSelection( vSubtask, "TE_TablRecKey", "TableRecord" );

   return( 0 );
} // zwTZTEUPDD_KeyListPrebuild


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_DropErrorMsgView
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_DropErrorMsgView( zVIEW vSubtask )
{
   zVIEW  vErrorList;

   if ( GetViewByName( &vErrorList, "TZTEERR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      DropObjectInstance( vErrorList );
   }

   return( 0 );
} // zwTZTEUPDD_DropErrorMsgView


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_ClearErrorWindow
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_ClearErrorWindow( zVIEW vSubtask )
{
   zVIEW  vErrorList;
   zVIEW  vWindow;
   zSHORT nRC;

   if ( GetViewByName( &vErrorList, "TZTEERR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "ErrorList" ) >= 0 )
      {
         for ( nRC = SetCursorFirstEntity( vErrorList, "ErrorMsg", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vErrorList, "ErrorMsg", 0 ) )
         {
            DeleteEntity( vErrorList, "ErrorMsg", zREPOS_NONE );
         }
         RefreshWindow( vWindow );
      }
      else
      {
         DropObjectInstance( vErrorList );
      }
   }

   return( 0 );
} // zwTZTEUPDD_ClearErrorWindow


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_KeepPhysCharAllColumn
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_KeepPhysCharAllColumn( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vDTECopy;
   zSHORT nRC;
   zCHAR  szPhysChar[ 2 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vDTE, "TE_TablRec",
                                  "KeepPhysCharsFlagForAllColumns",
                                  "Y" ) == 0 )
   {
      strcpy_s( szPhysChar, sizeof( szPhysChar ), "Y" );
   }
   else
   {
      strcpy_s( szPhysChar, sizeof( szPhysChar ), "" );
   }

   CreateViewFromViewForTask( &vDTECopy, vDTE, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vDTE, "TE_TablRec", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDTE, "TE_TablRec", 0 ) )
   {
      SetAttributeFromString( vDTE, "TE_TablRec", "KeepPhysCharsFlagForAllColumns", szPhysChar );

      for ( nRC = SetCursorFirstEntity( vDTE, "TE_FieldDataRel", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDTE, "TE_FieldDataRel", 0 ) )
      {
         SetAttributeFromString( vDTE, "TE_FieldDataRel", "KeepPhysicalCharacteristicsFlag", szPhysChar );
      }
   }

   // set Cursor to original position
   SetCursorFirstEntityByAttr( vDTE, "TE_TablRec", "ZKey", vDTECopy, "TE_TablRec", "ZKey", "" );
   SetCursorFirstEntityByAttr( vDTE, "TE_FieldDataRel", "ZKey", vDTECopy, "TE_FieldDataRel", "ZKey", "" );

   DropView( vDTECopy );

   return( 0 );
} // zwTZTEUPDD_KeepPhysCharAllColumn



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZTEUPDD_KeepPhysCharAll
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZTEUPDD_KeepPhysCharAll( zVIEW vSubtask )
{
   zVIEW  vDTE;
   zVIEW  vDTECopy;
   zSHORT nRC;
   zCHAR  szPhysChar[ 2 ];

   GetViewByName( &vDTE, "TE_DB_Environ", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vDTE, "TE_DBMS_Source", "KeepPhysicalCharacteristicsFlag", "Y" ) == 0 )
   {
      strcpy_s( szPhysChar, sizeof( szPhysChar ), "Y" );
   }
   else
   {
      strcpy_s( szPhysChar, sizeof( szPhysChar ), "" );
   }

   CreateViewFromViewForTask( &vDTECopy, vDTE, 0 );

   for ( nRC = SetCursorFirstEntity( vDTECopy, "TE_TablRec", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDTECopy, "TE_TablRec", 0 ) )
   {
      SetAttributeFromString( vDTECopy, "TE_TablRec", "KeepPhysicalCharacteristicsFlag", szPhysChar );
      SetAttributeFromString( vDTECopy, "TE_TablRec", "KeepPhysCharsFlagForAllColumns", szPhysChar );

      for ( nRC = SetCursorFirstEntity( vDTECopy, "TE_FieldDataRel", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDTECopy, "TE_FieldDataRel", 0 ) )
      {
         SetAttributeFromString( vDTECopy, "TE_FieldDataRel", "KeepPhysicalCharacteristicsFlag", szPhysChar );
      }
   }

   DropView( vDTECopy );

   return( 0 );
} // zwTZTEUPDD_KeepPhysCharAll


#ifdef __cplusplus
}
#endif
