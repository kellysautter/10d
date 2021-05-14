#define zGLOBAL_DATA
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzctlopr.h"
#include "ZeidonOp.H"

// LAST MESSAGE ID: AD10628

/*
CHANGE LOG

2002.08.29  DGC
   Added an argument to ParseSource().

2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.10.19    SR   Z2000 Size of path variables
   Modified size of szFileName, szFileName and szFileSpec in function
   oTZWDLGSO_AutoDesignDialog and szFileName in function
   oTZWDLGSO_AutoPaintWindow because size of attributes ExecDir, MetaSrcDir and
   PgmSrcDir has been changed to 254 in datamodel.

2000.01.13   BL    Z10
   Modified oTZWDLGSO_InitializeAutoPaint and oTZWDLGSO_InitializeAutoDesign
   for new PPE handling

1999.10.19  SR,RG  Z9J,Z2000 Autodesign
   change memory allocation in function oTZWDLGSO_AutoDesignDialog
   from 15000 --> 60000

1999.08.19  BL ALL      Autodesign
   update szTemplate[6000] to szTemplate[60000] in Operation
   oTZWDLGSO_AutoDesignDialog

1999.08.17  BL ALL      Autodesign
   update file length to user customization routines (VML-file)

1999.05.01  DC 10a+9j  Autodesign
   Made many changes to support entries to user customization routines.
*/

zOPER_EXPORT zPVOID OPERATION
ofnTZWDLGSO_GetUserExitAddress( zVIEW vView,
                                zPCHAR szEntryName );
zOPER_EXPORT zLONG  OPERATION
fnPainterCall( zSHORT nMsg,    zVIEW vSubtask,
               zVIEW  vDialog, zPCHAR szParm, zLONG lMaxLth );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_SaveAttribSelection( zVIEW vSubtask,
                                 zPCHAR szEntityAttrib );
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_TerminateAutoDesign( zVIEW vSubtask );


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignIntoExistingWindow
//
// PURPOSE:  This Operation paints controls into the already existing
//           window.  The new controls are created at the top of the
//           window.  No window actions are created even if they exist
//           in the user spec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDsgnIntoExistWnd( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vAutoDesWk;
   zVIEW    vNewDialog;
   zVIEW    vWindow;
   zLONG    lRC;
   zCHAR    szWindowName[ 33 ];
   zCHAR    szObjectName[ 33 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_ANY );
   CreateViewFromViewForTask( &vNewDialog, vWindow, 0 );

   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vAutoDesWk,
                           "AutoDesignWork", "ObjectEntityName" );
   if ( szWindowName[ 0 ] == 0 )
      GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vNewDialog, "Window", "Tag" );

   if ( zstrcmp( szWindowName, "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10601", "Autodesigner",
                   "You must enter in an object entity name!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vAutoDesWk,
                           "AutoDesignWork", "ObjectName" );
   if ( zstrcmp( szObjectName, "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10602", "Autodesigner",
                   "You must enter in an object name because the style object is a temporary style!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   if ( vUserSpec <= 0 )
   {
      ActivateEmptyMetaOI( vSubtask, &vUserSpec, zSOURCE_UIS_META, zSINGLE );
      CreateMetaEntity( vSubtask, vUserSpec, "UI_Spec", zPOS_AFTER );
      SetNameForView( vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
      lRC = ofnTZWDLGSO_AddNewEntityToStyle( vWindow, vUserSpec,
                                             szObjectName, szWindowName );
      if ( lRC )
      {
         DropObjectInstance( vUserSpec );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 1 );
      }
   }

   lRC = ofnTZWDLGSO_AddStyleEssentials( vSubtask, vUserSpec );
   if ( lRC == -1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                           "AutoDesignNewWnd", "N" );
   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork",
                            "AutoDesignSubords", 0 );
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );

   // Include any VORs from vUserSpec to vNewDialog.
   ofnTZWDLGSO_AddDialogVORs( vSubtask, vUserSpec, vNewDialog, "UIS_ViewObjRef" );

   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vNewDialog, "Window", "Tag" );
   fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vWindow, 0, 0, 0 );
   lRC = ofnTZWDLGSO_AutoDesignWindow( vSubtask, vUserSpec,
                                       vNewDialog, vAutoDesWk );
   fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vWindow, 0, szWindowName, zsizeof( szWindowName ) );
   DropView( vNewDialog );

   oTZWDLGSO_TerminateAutoDesign( vWindow );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignWindowWithoutStyle
//
// PURPOSE:  This Operation auto designs the current window without
//           using a user spec created by the user.  In this case,
//           the user specifies(in the autodesign window dialog) what
//           object and what entity in the object is going to be used
//           for autodesigning.  The code for this operation then
//           creates a temporary user spec to autodesign from.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDsgnWndWOStyle( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vAutoDesWk;
   zVIEW    vNewDialog;
   zVIEW    vWindow;
   zLONG    lCL;
   zLONG    lCC;
   zLONG    lRC;
   zLONG    lMaxLength;
   zLONG    lLastLine;
   zCHAR    szWindowName[ 33 ];
   zCHAR    szObjectName[ 33 ];
   zCHAR    cFullSize[ 2 ];

   lCC = 5;
   lCL = 12;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_ANY );
   CreateViewFromViewForTask( &vNewDialog, vWindow, 0 );

   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vAutoDesWk,
                           "AutoDesignWork", "ObjectEntityName" );
   if ( zstrcmp( szWindowName, "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10603", "Autodesigner",
                   "You must enter in an object entity name!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vAutoDesWk,
                           "AutoDesignWork", "ObjectName" );
   if ( CompareAttributeToString( vAutoDesWk, "AutoDesignWork",
                                  "ObjectName", "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10604", "Autodesigner",
                   "You must enter in an object name because the style object is a temporary style!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   if ( vUserSpec <= 0 )
   {
      ActivateEmptyMetaOI( vSubtask, &vUserSpec, zSOURCE_UIS_META, zSINGLE );
      CreateMetaEntity( vSubtask, vUserSpec, "UI_Spec", zPOS_AFTER );
      SetNameForView( vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
      lRC = ofnTZWDLGSO_AddNewEntityToStyle( vWindow, vUserSpec, szObjectName, szWindowName );
      if ( lRC != 0 )
      {
         DropObjectInstance( vUserSpec );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 1 );
      }
   }

   lRC = ofnTZWDLGSO_AddStyleEssentials( vSubtask, vUserSpec );
   if ( lRC == -1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );

   // Include any VORs from vUserSpec to vNewDialog.
   ofnTZWDLGSO_AddDialogVORs( vSubtask, vUserSpec, vNewDialog, "UIS_ViewObjRef" );

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "AutoDesignNewWnd", "Y" );
   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vNewDialog, "Window", "Tag" );
   fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vWindow, 0, 0, 0 );
   lRC = ofnTZWDLGSO_AutoDesignWindow( vSubtask, vUserSpec, vNewDialog, vAutoDesWk );

   // Set the size of the window.
   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lLastLine, &lMaxLength );
   lMaxLength = lMaxLength + 5 * lCC;
   GetStringFromAttribute( cFullSize, zsizeof( cFullSize ), vUserSpec, "WndDesign", "FullWnd" );

   lRC = SetCursorFirstEntityByString( vNewDialog, "Control", "Tag", "DIL1", 0 );
   // If there is a DIL message, then set it to the size of the window.
   if ( lRC >= zCURSOR_SET )
   {
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 1 );
      if ( cFullSize[ 0 ] == 'Y' )
      {
         SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", 320 );
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lLastLine + 3 );
      }
      else
      {
         SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lMaxLength );
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lLastLine + 3 );
      }

      lLastLine = lLastLine + 31;
   }
   else
      lLastLine = lLastLine + 18;

   if ( cFullSize[ 0 ] != 'Y' )
   {
      SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lLastLine );
      SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lMaxLength );
   }
   else
   {
      SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", 200 );
      SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", 320 );
   }

   fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vWindow, 0, szWindowName, zsizeof( szWindowName ) );
   DropView( vNewDialog );

   oTZWDLGSO_TerminateAutoDesign( vWindow );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//   ENTRY:    AutoDesignWindowWithStyleObject
//
//   PURPOSE:  Given a user spec dialog, and the entity from which to
//             to start autodesigning, this function creates the window
//             fields and window actions indicated in the user spec, onto
//             the current window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDsgnWndWStyle( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vAutoDesWk;
   zVIEW    vNewDialog;
   zVIEW    vWindow;
   zLONG    lCL;
   zLONG    lCC;
   zLONG    lRC;
   zLONG    lMaxLength;
   zLONG    lLastLine;
   zCHAR    szWindowName[ 33 ];
   zCHAR    cFullSize[ 2 ];

   lCC = 5;
   lCL = 12;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vNewDialog, vWindow, 0 );

   lRC = ofnTZWDLGSO_LoadStyleInfo( vWindow, vUserSpec, vNewDialog );
   if ( lRC == -1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                           "AutoDesignNewWnd", "Y" );
   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork",
                            "AutoDesignSubords", 0 );
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );

   // Include any VORs from vUserSpec to vNewDialog.
   ofnTZWDLGSO_AddDialogVORs( vSubtask, vUserSpec, vNewDialog, "UIS_ViewObjRef" );

   fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vWindow, 0, 0, 0 );
   lRC = ofnTZWDLGSO_AutoDesignWindow( vSubtask, vUserSpec, vNewDialog, vAutoDesWk );

   // Set the size of the window.
   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lLastLine, &lMaxLength );
   lMaxLength = lMaxLength + 5 * lCC;
   GetStringFromAttribute( cFullSize, zsizeof( cFullSize ), vUserSpec, "WndDesign", "FullWnd" );

   lRC = SetCursorFirstEntityByString( vNewDialog, "Control", "Tag", "DIL1", 0 );

   // If there is a DIL message, then set it to the size of the window.
   if ( lRC >= 0 )
   {
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 1 );
      if ( cFullSize[ 0 ] == 'Y' )
      {
         SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", 320 );
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lLastLine + 3 );
      }
      else
      {
         SetAttributeFromInteger( vNewDialog, "Control",
                                  "SZDLG_X", lMaxLength );
         SetAttributeFromInteger( vNewDialog, "Control",
                                  "PSDLG_Y", lLastLine + 3 );
      }

      lLastLine = lLastLine + 19;
   }
   else
      lLastLine = lLastLine + 18;

   if ( cFullSize[ 0 ] != 'Y' )
   {
      SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lLastLine );
      SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lMaxLength );
   }
   else
   {
      SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", 200 );
      SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", 320 );
   }

   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vNewDialog, "Window", "Tag" );
   fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vWindow, 0, szWindowName, zsizeof( szWindowName ) );
   DropView( vNewDialog );

   oTZWDLGSO_TerminateAutoDesign( vWindow );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignListWindow
//
// PURPOSE:  Given a user spec dialog, and the entity from which to
//           to start autodesigning, this function creates the window
//           fields and window action indicated in the user spec onto
//           the current window.  It also creates listboxes for any
//           subordinate enitites on the current window.  No windows
//           are created for the subordinate entities.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDsgnListWnd( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vNewDialog;
   zVIEW    vAutoDesWk;
   zVIEW    vWindow;
   zSHORT   nRC;
   zCHAR    szWindowName[ 33 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_ANY );
   CreateViewFromViewForTask( &vNewDialog, vWindow, 0 );

   nRC = ofnTZWDLGSO_LoadStyleInfo( vWindow, vUserSpec, vNewDialog );
   if ( nRC == -1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                           "AutoDesignNewWnd", "Y" );
   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork",
                            "AutoDesignSubords", 0 );
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );
   fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vWindow, 0, 0, 0 );
// nRC = ofnTZWDLGSO_AutoDsgnWndWLists( vWindow, vUserSpec, vNewDialog );

   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vNewDialog, "Window", "Tag" );
   fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vWindow, 0, szWindowName, zsizeof( szWindowName ) );
   DropView( vNewDialog );

   oTZWDLGSO_TerminateAutoDesign( vWindow );
   return( nRC );
}

#if 0
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDsgnSubordinates
//
// PURPOSE:  Given a user spec dialog, and the entity from which to
//           to start autodesigning, this function creates the window
//           fields and window actions indicated in the user spec onto
//           the current window.  It also creates listboxes for any
//           subordinate entities on the current window and then creates
//           windows for the subordinate entities and autodesigns them.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDsgnSubordinates( zVIEW vSubtask )
{
   zVIEW   vUserSpec;
   zVIEW   vAutoDesWk;
   zVIEW   vNewDialog;
   zVIEW   vSubtask;
   zVIEW   vWindowList;
   zLONG   lRC;
   zCHAR   szWindowName[ 33 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_ANY );
   CreateViewFromViewForTask( &vNewDialog, vSubtask, 0 );
   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vNewDialog, "Window", "Tag" );

   nRC = ofnTZWDLGSO_LoadStyleInfo( vSubtask, vUserSpec, vNewDialog );
   if ( nRC == -1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork",
                            "FirstWindow", 1 ); /* TRUE */
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "AutoDesignNewWnd", "Y" );
   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork",
                             "AutoDesignSubords", 1 );  /* TRUE */

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );

   // Include any VORs from vUserSpec to vNewDialog.
   ofnTZWDLGSO_AddDialogVORs( vSubtask, vUserSpec, vNewDialog, "UIS_ViewObjRef" );

   fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vSubtask, 0, 0, 0 );
   nRC = ofnTZWDLGSO_AutoDsgnWndAndSubs( vSubtask, vUserSpec, vNewDialog, vAutoDesWk );

   fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, szWindowName, zsizeof( szWindowName ) );

   // Refresh the window list in the event the window name has changed
   if ( GetViewByName( &vWindowList, "TZWND_LIST", vSubtask, zLEVEL_TASK ) > 0 )
      RefreshCtrl( vWindowList, "ListBox1" );

   DropView( vNewDialog );

   oTZWDLGSO_TerminateAutoDesign( vSubtask );
   return( 0 );
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: AutoDesignWDOD
//
// PURPOSE:  This function gets called when the user hits the auto design
//           wdod button.  The user has selected in the dialog style
//           what type of wdod is going to be auto designed(general
//           wdod, top level wdod or list wdod) and this function calls
//           the appropriate sub-function to create this wdod style entry
//           window.  Then the template code is created for the wdod and
//           then AutoDesignSubordinates is called which creates all
//           the subwindows.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDesignDialog( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vUserSpecRoot;
   zVIEW    vNewDialog;
   zVIEW    vAutoDesWk;
   zVIEW    vWindow;
   zVIEW    vTempWindow;
   zVIEW    vWindowList;
   zVIEW    vDefaults;
   zVIEW    vTaskLPLR;
   zVIEW    vT1;
   zVIEW    vT2;
   zSHORT   nRC;
   zCHAR    szWindowName[ 33 ];
   zCHAR    szDialogName[ 33 ];
   zCHAR    szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szFileSpec[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    cDialogStyle[ 2 ];
   zLONG    hSourceFlag;
   zLONG    lGroupboxRightMargin;
   zLONG    lGroupboxBottomMargin;
   zLONG    lFile;
   typedef  zSHORT (POPERATION PFN_USERINITIALIZE) ( zVIEW, zVIEW );
   PFN_USERINITIALIZE pfnUserInitialize;
   typedef  zSHORT (POPERATION PFN_USERTERMINATION) ( zVIEW, zVIEW, zPCHAR );
   PFN_USERTERMINATION pfnUserTermination;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWindow, "TZOPENDLG", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vWindow, "SourceFile" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10605", "Autodesigner",
                   "The dialog must have a source file!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   // Make sure that the Dialog has new or checked out status.  We will
   // do this by trying to get position on the zSOURCE_DIALOG_META entry of
   // the current Dialog.  The TZDIALOGS view is already positioned on the
   // zREFER_DIALOG_META entry of the current Dialog.
   GetViewByName( &vT1, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szDialogName, zsizeof( szDialogName ),
                           vT1, "Dialog", "Tag" );
   GetViewByName( &vT1, "TZDIALOGS", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vT1, "W_MetaDef" ) == zCURSOR_SET &&
        CompareAttributeToString( vT1, "W_MetaDef", "Name",
                                  szDialogName ) == 0 )
   {
      CreateViewFromViewForTask( &vT2, vT1, 0 );
      nRC = SetCursorFirstEntityByInteger( vT2, "W_MetaType", "Type",
                                           zSOURCE_DIALOG_META, 0 );
      if ( nRC >= zCURSOR_SET )
         nRC = SetCursorFirstEntityByString( vT2, "W_MetaDef", "Name",
                                             szDialogName, 0 );
      DropView( vT2 );
      if ( nRC < zCURSOR_SET )
      {
         MessageSend( vSubtask, "AD10625", "Autodesigner",
                      "The dialog has not been checked out for update!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 1 );
      }
   }

   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vWindow, 0, 0, 0 );

   // Delete Operations
   nRC = SetCursorFirstEntity( vWindow, "Operation", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      DeleteEntity( vWindow, "Operation", zREPOS_NONE );
      nRC = SetCursorNextEntity( vWindow, "Operation", 0 );
   }

   ofnTZWDLGSO_DeleteWindows( vSubtask, vWindow );

   CreateViewFromViewForTask( &vNewDialog, vWindow, 0 );

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "SaveRoutineFlagForRootEntity", "S" );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "DeleteRoutineFlagForRootEntity", "D" );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "CancelRoutineFlagForRootEntity", "C" );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "AutoDesignNewWnd", "Y" );
   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork", "AutoDesignSubords", 1 ); /* TRUE */
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "AutoDesignWDOD", "Y" );
   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork", "FirstWindow", 1 ); /* TRUE */
   GetStringFromAttribute( cDialogStyle, zsizeof( cDialogStyle ), vAutoDesWk, "AutoDesignWork", "WDOD_Style" );

   // Activate the Defaults object that will be used in autodesigning
   // windows.
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   strcat_s( szFileName, zsizeof( szFileName ), "\\Z__DFLT.BAS" );
   lFile = SysOpenFile( vWindow, szFileName, COREFILE_READ );
   if ( lFile < 0 )
   {
      MessageSend( vSubtask, "AD10201", "Autodesigner",
                   "LPLR Defaults (Z__DFLT.BAS) not found.\nExiting Autodesign.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   else
   {
      SysCloseFile( vSubtask, lFile, 0 );
      ActivateOI_FromFile( &vDefaults, "TZADCSDO", vNewDialog,
                           szFileName, zSINGLE );
   }

   // Before calling the chain that will autodesign all the subwindows,
   // compute the Groupbox right and bottom margin values that will be
   // used in designing Groupboxes.
   // The right margin will be the button width plus the constant 5.
   // The bottom margin will be the button height plus the constant 3.
   GetIntegerFromAttribute( &lGroupboxRightMargin,
                            vDefaults, "InitValues", "ButtonWidth" );
   lGroupboxRightMargin = lGroupboxRightMargin + 5;

   GetIntegerFromAttribute( &lGroupboxBottomMargin,
                            vDefaults, "InitValues", "ButtonHeight" );
   lGroupboxRightMargin = lGroupboxRightMargin + 3;

   SetAttributeFromInteger( vDefaults, "InitValues", "GroupboxRightMargin",
                            lGroupboxRightMargin );
   SetAttributeFromInteger( vDefaults, "InitValues", "GroupboxBottomMargin",
                            lGroupboxBottomMargin );

   // //* Temporary Code //*
   // Qualification style ("Q") is not currently valid.
   if ( cDialogStyle[ 0 ] == 'Q' )
   {
      MessageSend( vSubtask, "TEMP", "Autodesigner",
                   "The qualified list option is not active at this time.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   nRC = ofnTZWDLGSO_LoadStyleInfo( vSubtask, vUserSpec, vNewDialog );
   if ( nRC == -1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   // Check if there is an existing .VML file for the Dialog.  If so, ask
   // the user if he wants to overlay the existing source file.  If not,
   // Autodesign the Dialog without overlaying the file.
   // Code above has already retrieved the Dialog name.
   GetViewByName( &vT1, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szFileSpec, zsizeof( szFileSpec ), vT1, "LPLR", "PgmSrcDir" );
   SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), szFileSpec );

   strcat_s( szFileName, zsizeof( szFileName ), "\\" );
   strcat_s( szFileName, zsizeof( szFileName ), szDialogName );
   strcat_s( szFileName, zsizeof( szFileName ), ".VML" );
   hSourceFlag = SysOpenFile( vSubtask, szFileName, COREFILE_EXIST );
   if ( hSourceFlag >= 0 )
   {
      nRC = MessagePrompt( vSubtask, "AD10626", "Autodesigner",
                           "Do you want to overlay the current VML source?",
                           zBEEP, zRESPONSE_YES, 0, 0 );
      if ( nRC == zRESPONSE_YES )
      {
         // Deleting the old file will cause new operations to be created.
         SysOpenFile( vSubtask, szFileName, COREFILE_DELETE );
         hSourceFlag = -1;
      }
   }

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );

   // Call the initial User exit point operation.
   pfnUserInitialize = (PFN_USERINITIALIZE)
            ofnTZWDLGSO_GetUserExitAddress( vWindow,
                                            "oTZADCSDO_UserInitializeEntry" );
   if ( pfnUserInitialize != 0 )
   {
      zVIEW vUserSpecTemp;
      zVIEW vNewDialogTemp;

      CreateViewFromViewForTask( &vUserSpecTemp, vUserSpec, 0 );
      CreateViewFromViewForTask( &vNewDialogTemp, vNewDialog, 0 );
      (*pfnUserInitialize)( vUserSpecTemp, vNewDialogTemp );
      DropView( vUserSpecTemp );
      DropView( vNewDialogTemp );
   }

   // Create a root view of UserSpec for later recursive processing.
   CreateViewFromViewForTask( &vUserSpecRoot, vUserSpec, 0 );
   SetNameForView( vUserSpecRoot, "UserSpecRoot", vSubtask, zLEVEL_TASK );

   // Include any VORs from vUserSpec to vNewDialog.
   ofnTZWDLGSO_AddDialogVORs( vSubtask, vUserSpec, vNewDialog, "UIS_ViewObjRef" );

   if ( cDialogStyle[ 0 ] == 'G' )
   {
      ofnTZWDLGSO_CreateGenericDialog( vUserSpec, vNewDialog );
   }
   else
   {
      oTZADCSDO_AutoDesignListWindow( vWindow, vUserSpec,
                                      vNewDialog, vDefaults );
   }

   if ( nRC < 0 )
   {
      return( -1 );
   }

   // Add ActivateMainList and UpdateExistingInstance operations, unless
   // no list window was requested ( cDialogStyle = 'G' ).
   if ( cDialogStyle[ 0 ] != 'G' )
   {
      // Add the Operation VML for the Main List to the source code.
      ofnTZWDLGSO_CreateSourceCode( vSubtask, vUserSpec, vNewDialog,
                                    "ActivateMainList" );

      // Add the Operation VML for the Main Activate to the source code.
      ofnTZWDLGSO_CreateSourceCode( vSubtask, vUserSpec, vNewDialog,
                                    "UpdateExistingInstance" );
   }

   nRC = ofnTZWDLGSO_AutoDsgnWndAndSubs( vWindow, vUserSpec, vNewDialog,
                                         vDefaults, vAutoDesWk );
   if ( nRC < 0 )
   {
      return( -1 );
   }

   // Call the final User exit point operation entry for Dialog modification.
   pfnUserTermination = (PFN_USERTERMINATION)
          ofnTZWDLGSO_GetUserExitAddress( vWindow,
                                          "oTZADCSDO_UserTerminationEntry" );
   if ( pfnUserTermination != 0 )
   {
      zVIEW   vCM_Obj;
      zVIEW   vTempObj;
      zVIEW   vUserSpecTemp;
      zVIEW   vNewDialogTemp;
      zCHAR   szTemplate[60000];
      zPCHAR  lpMemory;
      zLONG   selMemory;
      zLONG   fh;
      zLONG   lLth;
      zLONG   lRC;
      zSHORT  nLth;

      // Set up szTemplate to null.
      szTemplate[ 0 ] = 0;

      CreateViewFromViewForTask( &vUserSpecTemp, vUserSpec, 0 );
      CreateViewFromViewForTask( &vNewDialogTemp, vNewDialog, 0 );
      (*pfnUserTermination) ( vUserSpecTemp, vNewDialogTemp, szTemplate );
      DropView( vUserSpecTemp);
      DropView( vNewDialogTemp);

      // If the Termination routine added VML code, then concatenate it to the existing code.
      if ( szTemplate[ 0 ] != 0 )
      {
         GetViewByName( &vCM_Obj, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );
         GetViewByName( &vTempObj, "TZCMWKSO", vCM_Obj, zLEVEL_SUBTASK );

         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTempObj, "LPLR", "PgmSrcDir" );
         if ( szFileName[ 0 ] == 0 )
         {
            GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTempObj, "LPLR", "MetaSrcDir" );
         }

         SysConvertEnvironmentString( szFileSpec, zsizeof( szFileSpec ), szFileName );
         nLth = (zSHORT) zstrlen( szFileSpec );
         if ( nLth > 0 )
         {
            if ( szFileSpec[ nLth - 1 ] != '\\' )
            {
               szFileSpec[ nLth++ ] = '\\';
               szFileSpec[ nLth ] = 0;
            }
         }

         // Get the file name and open the file.
         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vNewDialog, "Dialog", "DLL_Name" );

         // If the name is null, then use the Dialog name.
         if ( szFileName[ 0 ] == 0 )
            GetStringFromAttribute( szFileName, zsizeof( szFileName ), vNewDialog, "Dialog", "Tag" );

         strcat_s( szFileName, zsizeof( szFileName ), ".VML" );

         strcat_s( szFileSpec, zsizeof( szFileSpec ), szFileName );

         fh = SysOpenFile( vSubtask, szFileSpec, COREFILE_UPDATE );

         lLth = GetFileSize( (HANDLE) fh, 0 );

         TraceLineI( "File length = ", lLth );
         selMemory = SysAllocMemory((zCOREMEM) &lpMemory, lLth + 60000, 0,
                                    zCOREMEM_ALLOC, 0 );
         *lpMemory = 0;  // Initialize to null.
         lRC = (zLONG) SysReadFile( vSubtask, fh, lpMemory, (zUSHORT) lLth );

         // read the file into the buffer
         if ( lLth != lRC )
         {
            SysFreeMemory( selMemory );
            MessageSend( vSubtask, "AD10402", "Autodesigner",
                         "Error in reading VML file.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            SysCloseFile( vSubtask, fh, 0 );
            return( -1 );
         }

         // Get rid of the EOF character.
         if ( lpMemory[ lLth - 1 ] == '\x1A' )
            lpMemory[ lLth - 1 ] = 0;

         // Concatenate the new VML, add the EOF zCHAR and write the string back.
         strcat_s( lpMemory, lLth + 60000, szTemplate );
         lLth = zstrlen( lpMemory );
         lpMemory[ lLth - 1 ] = '\x1A';
         SetFilePointer( (HANDLE) fh, 0, 0, FILE_BEGIN );
         WriteFile( (HANDLE) fh, lpMemory, lLth, &lRC, 0 );
         SysFreeMemory( selMemory );
         SysCloseFile( vSubtask, fh, 0 );
      }
   }

   SetCursorFirstEntity( vNewDialog, "Window", "" );
   SetViewFromView( vWindow, vNewDialog );
   DropView( vNewDialog );

   if ( cDialogStyle[ 0 ] == 'G' )
   {
      GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vUserSpec, "WndDesign", "WndName" );
      if ( szWindowName[ 0 ] == 0 )
         GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vUserSpec, "LOD_Entity", "Name" );
   }
   else
   {
      // The list window name (formerly AutoDsgnLoadWindow) is the Entity
      // name, followed by the characters "_List".
      GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vUserSpec, "LOD_Entity", "Name" );
      strcat_s( szWindowName, zsizeof( szWindowName ), "_List" );
   }

   // Refresh the window list in the event the window name has changed
   if ( GetViewByName( &vWindowList, "TZWND_LIST", vSubtask, zLEVEL_TASK ) > 0 )
      RefreshCtrl( vWindowList, "ListBox1" );

   fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vWindow, 0, szWindowName, zsizeof( szWindowName ) );

   // Make sure that the SourceFile entity indicates VML.
   SetAttributeFromString( vWindow, "SourceFile", "LanguageType", "V" );
   SetAttributeFromString( vWindow, "SourceFile", "Extension", "VML" );

   // Parse the VML code, if it was generated.
   if ( hSourceFlag < 0 )
   {
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vWindow, "SourceFile", "Name" );

      // Create a temporary view of the dialog because the parser will drop it.
      CreateViewFromViewForTask( &vTempWindow, vWindow, 0 );
      nRC = (zSHORT) ParseSource( vWindow, vTempWindow, 2, 3, 1, szFileName,
                                  "", zREFER_DIALOG_META, 0, "C" );

      if ( nRC == -1 )
      {
         MessageSend( vSubtask, "VM10625", "Autodesigner",
                      "Parse completed with Errors.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
      {
         MessageSend( vSubtask, "VM10626", "Autodesigner",
                      "Parse completed successfully.",
                      zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      }
   }

   oTZWDLGSO_TerminateAutoDesign( vWindow );

   // Make sure the TZWINDOWL view is reset to the new TZWINDOW OI.
   nRC = GetViewByName( &vT1, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      DropView( vT1 );

   CreateViewFromViewForTask( &vT1, vWindow, 0 );
   SetNameForView( vT1, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   // Make sure we have valid position on Primary window.
   SetCursorFirstEntity( vT1, "DfltWnd", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SelectObjectEntity
//
// PURPOSE:  This function gets an ENTITY name from the style object
//           and sets ObjectEntityName in the autodesign work object
//           to it.  This is done because sometimes when the user is
//           autodesigning a window or a window with subordinates,
//           the current window has a different name than any of the
//           ENTITY names in the style object.  When this occurs, the
//           user must specify an ENTITY name from which to start
//           autodesigning.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SelectObjectEntity( zVIEW vSubtask )
{
   zVIEW    vStructObj;
   zVIEW    vAutoDesWk;

   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vStructObj, "EntityList", vSubtask, zLEVEL_TASK );
   SetAttributeFromAttribute( vAutoDesWk, "AutoDesignWork",
                              "ObjectEntityName",
                              vStructObj, "W_Entity", "Name" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GetObjectEntities
//
// PURPOSE:  This function retrieves all the ENTITYs from the object
//           used in a given style definition and puts them in a
//           list.  This is so the user can select which entity to
//           start autodesigning from.  This code is called from the
//           button "Select Object Entity...".
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_GetObjectEntities( zVIEW vSubtask )
{
   zVIEW    vObject;
   zVIEW    vVOR;
   zVIEW    vAutoDesWk;
   zVIEW    vStructObj;
   zVIEW    vFileObj;
   zCHAR    szObjectName[ 33 ];
   zCHAR    szWorkString[ 256 ];
   zSHORT   nRC;

   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vAutoDesWk,
                           "AutoDesignWork", "ObjectName" );

   if ( szObjectName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "AD10606", "Autodesigner",
                   "A VOR name must be entered!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_VOR_META );
   nRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef",
                                              "Name", szObjectName, "" );
   if ( nRC <= zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "There is no VOR with the name " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szObjectName );
      MessageSend( vSubtask, "AD10607", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   nRC = ActivateMetaOI( vSubtask, &vVOR, vFileObj, zREFER_VOR_META, zSINGLE );
   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vVOR, "LOD", "Name" );

   nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_LOD_META );
   nRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef",
                                             "Name", szObjectName, "" );
   nRC = ActivateMetaOI( vSubtask, &vObject, vFileObj, zREFER_LOD_META, zSINGLE );
   if ( nRC == -1 )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "Check-out failed on object - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szObjectName );
      MessageSend( vSubtask, "AD10608", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   ActivateEmptyObjectInstance( &vStructObj, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vStructObj, "EntityList", vSubtask, zLEVEL_TASK );

   CreateEntity( vStructObj, "W_EntityList", zPOS_AFTER );

   nRC = SetCursorFirstEntity( vObject, "LOD_Entity", "" );

   // Position on the correct UIS_Entity.
   while ( nRC > zCURSOR_UNCHANGED )
   {
      CreateEntity( vStructObj, "W_Entity", zPOS_AFTER );
      SetAttributeFromAttribute( vStructObj, "W_Entity", "Name",
                                 vObject, "LOD_Entity", "Name" );
      nRC = SetCursorNextEntity( vObject, "LOD_Entity", "" );
   }

// ofnTZWDLGSO_BuildEntStructList( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GetObjectEntities
//
// PURPOSE:  This function retrieves all the ENTITYs from the object
//           used in a given style definition and puts them in a
//           list.  This is so the user can select which entity to
//           start autodesigning from.  This code is called from the
//           button "Select Object Entity...".
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_GetStyleEntities( zVIEW  vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vTempSpec;
   zVIEW    vWorkObj;
   zVIEW    vStructObj;
   zVIEW    vFileObj;
   zCHAR    szStyleName[ 33 ];
   zCHAR    szEntity[ 33 ];
   zCHAR    szWorkString[ 256 ];
   zSHORT   lLevel;
   zSHORT   nRC;

   GetViewByName( &vWorkObj, "TZPNTROO", vSubtask, zLEVEL_ANY );

   nRC = GetStringFromAttribute( szStyleName, zsizeof( szStyleName ), vWorkObj,
                                        "Palette", "UIS_Name" );

   if ( szStyleName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "AD10609", "Autodesigner",
                   "No style name has been selected!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 1 );
   }

   nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_UIS_META );
   nRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef",
                                       "Name", szStyleName, "" );
   nRC = ActivateMetaOI( vSubtask, &vUserSpec, vFileObj, zSOURCE_UIS_META, zSINGLE );
   if ( nRC == -1 )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "Check-out failed on dialog spec - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szStyleName );
      MessageSend( vSubtask, "AD10610", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 1 );
   }

   ActivateEmptyObjectInstance( &vStructObj, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vStructObj, "EntityList", vSubtask, zLEVEL_TASK );

   ResetView( vUserSpec );
   CreateEntity( vStructObj, "W_EntityList", zPOS_AFTER );

   CreateViewFromViewForTask( &vTempSpec, vUserSpec, 0 );
   DefineHierarchicalCursor( vUserSpec, "UI_Spec" );
   nRC = SetCursorNextEntityHierarchical( &lLevel, szEntity, vUserSpec );

   // Position on the correct UIS_Entity
   while ( nRC != zCURSOR_UNCHANGED )
   {
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      {
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      }

      if ( zstrcmp( szEntity, "UIS_Entity" ) == 0 ||
           zstrcmp( szEntity, "UIS_ChildEntity" ) == 0 )
      {
         CreateEntity( vStructObj, "W_Entity", zPOS_AFTER );
         GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec,
                                 "LOD_Entity", "Name" );
         SetAttributeFromAttribute( vStructObj, "W_Entity", "Name",
                                    vUserSpec, "LOD_Entity", "Name" );
      }

      nRC = SetCursorNextEntityHierarchical( &lLevel, szEntity,
                                                   vUserSpec );
   }

   DropMetaOI( vSubtask, vTempSpec );
   DropHierarchicalCursor( vUserSpec );
   DropMetaOI( vSubtask, vUserSpec );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SaveAttributeSelection
//
// PURPOSE:  Save the ATTRIBs selected from ZSTROOD to the current
//           ENTITY in vUserSpec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SaveSelMaintAttribs( zVIEW vSubtask )
{
   ofnTZWDLGSO_SaveAttribSelection( vSubtask, "M_LOD_Attribute" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SaveAttributeSelection
//
// PURPOSE:  Save the ATTRIBs selected from ZSTROOD to the current
//           ENTITY in vUserSpec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_SaveAttribSelection( zVIEW vSubtask,
                                 zPCHAR szEntityAttrib )
{
   zVIEW  vStructObj;
   zVIEW  vUserSpec;
   zVIEW  vObjectObj;
   zSHORT RESULT1;
   zSHORT RESULT2;
   zSHORT nRC;
   zLONG  lAttribZKey;

   GetViewByName( &vStructObj, "FlatList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   RESULT1 = SetCursorFirstEntity( vStructObj, "W_Entity", "" );
   while ( RESULT1 > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &lAttribZKey, vStructObj,
                               "W_Entity", "LOD_EntityZKey" );
      nRC = SetCursorFirstEntityByInteger( vObjectObj, "LOD_Entity",
                                           "ZKey", lAttribZKey, "LOD" );

      RESULT2 = SetCursorFirstEntity( vStructObj, "W_Attribute", "" );
      while ( RESULT2 > zCURSOR_UNCHANGED )
      {
         GetIntegerFromAttribute( &lAttribZKey, vStructObj, "W_Attribute",
                                  "LOD_AttributeZKey" );
         SetCursorFirstEntityByInteger( vObjectObj, "LOD_Attribute",
                                        "ZKey", lAttribZKey, "" );

         // See if this entity already exists in the userspec.  If it
         // does, then do not include it in.
         nRC = SetCursorFirstEntityByInteger( vUserSpec, szEntityAttrib,
                                              "ZKey", lAttribZKey, "" );
         if ( nRC != zCURSOR_SET )
         {
            IncludeSubobjectFromSubobject( vUserSpec, szEntityAttrib,
                                           vObjectObj, "LOD_Attribute",
                                           zPOS_AFTER );
         }

         RESULT2 = SetCursorNextEntity( vStructObj, "W_Attribute", "" );
      }

      RESULT1 = SetCursorNextEntity( vStructObj, "W_Entity", "" );
   }

   DropObjectInstance( vStructObj );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ClearAllAttributes
//
// PURPOSE:  Clear all the M_LOD... attributes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_ClearAllAttributes( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zSHORT nRC;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", "" );
   while ( nRC > zCURSOR_UNCHANGED )
      nRC = ExcludeEntity( vUserSpec, "M_LOD_Attribute", zREPOS_NEXT );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoPaintWindow
//
// PURPOSE:  Given a user spec dialog, and the entity from which to
//           to start autodesigning, this function creates the window
//           fields and window actions indicated in the user spec, onto
//           the current window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoPaintWindow( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vAutoDesWk;
   zVIEW    vWindow;
   zVIEW    vDefaults;
   zVIEW    vTaskLPLR;
   zLONG    lFile;
   zCHAR    szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szTemp[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nRC;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vAutoDesWk,
                           "AutoDesignWork", "ObjectName" );
   TraceLineS( "AutoPaintWindow ObjectName: ", szFileName );

   // Make sure User Spec is positioned on first M_LOD_Attribute because
   // AutoDesignGroupboxM will start painting where ever the cursor is
   // currently positioned.
   // Also give error if there is not at least one attribute.
   nRC = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", 0 );
   if ( nRC < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10628", "Autodesigner",
                   "No attributes were selected to design.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Activate the Defaults object that will be used in autodesigning
   // subwindows.
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   strcat_s( szTemp, zsizeof( szTemp ), "\\Z__DFLT.BAS" );
   SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), szTemp );
   lFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( lFile < 0 )
   {
      MessageSend( vSubtask, "AD10201", "Autodesigner",
                   "LPLR Defaults not found.\nExiting Autodesign.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( -1 );
   }
   else
   {
      SysCloseFile( vSubtask, lFile, 0 );
      ActivateOI_FromFile( &vDefaults, "TZADCSDO", vSubtask,
                           szFileName, zSINGLE );
   }

   // Use the standard Groupbox paint routine to paint the controls into
   // the window.
// DisplayObjectInstance( vUserSpec, 0, 0 );
// MessageBox( 0, "Text1", "Caption1", MB_OK );
   oTZWDLGSO_AutoDesignGroupboxM( vSubtask, vUserSpec, vWindow, vDefaults,
                                  vAutoDesWk, "R" );

   // Get rid of the User Spec view.
   DropView( vUserSpec );

   // Finally show the controls in the Painter.
   fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vWindow, 0, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GetWindowAttibutes
//
// PURPOSE:  This function
//           style object and loads in the object definition associated
//           with this style.  This is so that the user can update
//           the MAINTATTRIB and LISTATTRIB attributes and make changes
//           to the WndDesign information.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_GetWindowAttributes( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vObjectObj;
   zVIEW    vStructObj;
   zVIEW    vAutoDesWk;
//   zVIEW    vVOR;
   zVIEW    vWindow;
   zCHAR    szObjectName[ 33 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_ANY );

   // Verify that an object was selected.
   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vAutoDesWk,
                           "AutoDesignWork", "ObjectName" );
   if ( zstrcmp( szObjectName, "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10612", "Autodesigner",
                   "You must enter in an object name !",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   // Initialize the flat list view.
   ActivateEmptyObjectInstance( &vStructObj, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vStructObj, "FlatList", vSubtask, zLEVEL_TASK );
   CreateEntity( vStructObj, "W_EntityList", zPOS_AFTER );

   // Activate the LOD selected.
   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vWindow, "LOD", "Name" );
   ActivateMetaOI_ByName( vSubtask, &vObjectObj, 0,
                          zREFER_LOD_META,
                          zSINGLE | zLEVEL_APPLICATION,
                          szObjectName, 0 );
   SetNameForView( vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );

   // Make sure we are positioned on the correct VOR in the Dialog.
   SetCursorFirstEntityByString( vWindow, "LOD", "Name", szObjectName, "Dialog" );

   // Add the selected ViewObjRef to the User Spec if it isn't already there.
   if ( SetCursorFirstEntityByString( vUserSpec,    "UIS_LOD", "Name",
                                      szObjectName, "UI_Spec" ) < zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vUserSpec, "UIS_ViewObjRef",
                                     vWindow,  "ViewObjRef", zPOS_AFTER );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    InitializeAutoPaint
//
// PURPOSE:  This operation intiializes the AutoPaint facility
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_InitializeAutoPaint( zVIEW vSubtask )
{
   zVIEW    vNewDialog;
   zVIEW    vAutoDesWk;
   zVIEW    vCtrlDef;
   zVIEW    vList;
   zVIEW    vUserSpec;
   zCHAR    szParentName[ 33 ];
   zCHAR    szTag[ 33 ];
   zSHORT   nRC;

   nRC = GetViewByName( &vNewDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   /* If nRC is -1 then there is no window and perhaps no dialog currently
      activated in the painter.  There must be a window to autodesign so
      give the user an error. */
   if ( nRC == -1 )
   {
      MessageSend( vSubtask, "AD10615", "Autodesigner",
                   "No Window exists.  There must be a window to autodesign!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      return( 1 );
   }

   // Set up the PE view.
   nRC = GetViewByName( &vCtrlDef, "TZADCTLD", vSubtask, zLEVEL_TASK );
   if ( nRC == -1 )
   {
      RetrieveViewForMetaList( vSubtask, &vList, zREFER_PENV_META );

      nRC = LoadZeidonPPE( vSubtask, &vCtrlDef, zREFER_PENV_META, vList,
                           "Autodesigner",
                           "Autodesigner aborted.");
      if ( nRC < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
         return( -1 );
      }

      SetNameForView( vCtrlDef, "TZADCTLD", vSubtask, zLEVEL_TASK );
   }

   // Make sure the UserSpec object is deleted.
   if ( GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vUserSpec );

   ActivateEmptyObjectInstance( &vAutoDesWk, "TZADWWKO", vSubtask, zSINGLE );
   SetNameForView( vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   CreateEntity( vAutoDesWk, "AutoDesignWork", zPOS_AFTER );
   strcpy_s( szParentName, zsizeof( szParentName ), "Window" );
   strcpy_s( szTag, zsizeof( szTag ), "Tag" );
   SetAttributeFromAttribute( vAutoDesWk, "AutoDesignWork",
                               "ObjectEntityName",
                               vNewDialog, "Window", "Tag" );
   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork",
                             "AutoDesignSubords", 0 );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "SaveRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "CancelRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "DeleteRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "NextRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "PrevRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "AutoDsgnListDialog", "N" );

   // Initialize a User Spec.
   ActivateEmptyMetaOI( vSubtask, &vUserSpec, zSOURCE_UIS_META, zSINGLE );
   CreateMetaEntity( vSubtask, vUserSpec, "UI_Spec", zPOS_AFTER );
   SetNameForView( vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vUserSpec, "UIS_Entity", zPOS_AFTER );
   CreateMetaEntity( vSubtask, vUserSpec, "WndDesign", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GetWindowActions
//
// PURPOSE:  This function positions on the proper ENTITY in the
//           style object and loads in the object definition associated
//           with this style.  This is so that the user can update
//           the MAINTATTRIB and LISTATTRIB attributes and make changes
//           to the WndDesign information.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_GetWindowActions( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vAutoDesWk;
   zCHAR    szEntityName[ 33 ];
   zCHAR    szObjectName[ 33 ];
   zSHORT   nRC;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vAutoDesWk,
                            "AutoDesignWork", "ObjectEntityName" );
   if ( zstrcmp( szEntityName, "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10613", "Autodesigner",
                   "You must enter in an object entity name!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vAutoDesWk,
                            "AutoDesignWork", "ObjectName" );
   if ( zstrcmp( szObjectName, "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10614", "Autodesigner",
                   "You must enter in an object name!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   if ( vUserSpec <= 0 )
   {
      ActivateEmptyMetaOI( vSubtask, &vUserSpec, zSOURCE_UIS_META, zSINGLE );
      CreateMetaEntity( vSubtask, vUserSpec, "UI_Spec", zPOS_AFTER );
      nRC = ofnTZWDLGSO_AddNewEntityToStyle( vSubtask, vUserSpec,
                                              szObjectName, szEntityName );
      if ( nRC == -1 )
      {
         DropObjectInstance( vUserSpec );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 1 );
      }

      SetNameForView( vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   }

   if ( CheckExistenceOfEntity ( vUserSpec, "WndDesign" ) != 0 )
   {
      CreateMetaEntity( vSubtask, vUserSpec, "WndDesign", zPOS_AFTER );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SaveAttributeSelection
//
// PURPOSE:  Save the ATTRIBs selected from ZSTROOD to the current
//           ENTITY in vUserSpec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SaveSelListAttribs( zVIEW vSubtask )
{
   ofnTZWDLGSO_SaveAttribSelection( vSubtask, "L_LOD_Attribute" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LoadAutoDesignObject
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_LoadAutoDesignObject( zVIEW vSubtask )
{
   zVIEW  vAutoDesWk;
   zVIEW  vLOD_List;
   zVIEW  vLOD;
   zVIEW  vVOR_List;
   zVIEW  vVOR;
   zCHAR  szVOR[ 33 ];
   zCHAR  szLOD[ 33 ];
   zSHORT nRC;

   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vVOR_List, "AD_VOR_List", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szVOR, zsizeof( szVOR ), vAutoDesWk, "AutoDesignWork", "ObjectName" );
   nRC = SetCursorFirstEntityByString( vVOR_List, "W_MetaDef",
                                       "Name", szVOR, "" );
   nRC = ActivateMetaOI( vSubtask, &vVOR, vVOR_List, zREFER_VOR_META, zSINGLE );
   if ( nRC < zCURSOR_SET )
      return( 1 );

   GetStringFromAttribute( szLOD, zsizeof( szLOD ), vVOR, "LOD", "Name" );

   RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
   nRC = SetCursorFirstEntityByString( vLOD_List, "W_MetaDef",
                                       "Name", szLOD, "" );
   nRC = ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zSINGLE );
   if ( nRC >= zCURSOR_SET )
      SetNameForView( vLOD, "LOD_Object", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SelectPaintEntity
//
// PURPOSE:  This function Selects and de-Selects an Entity
//           for painting in the Select Attributes Window of
//           the WDOD KZADWIAW.
//
//           When an entity is double-clicked, the attribute list
//           is either created( if no attributes are currently under
//           the clicked on entity ) or deleted( if attributes do
//           exist for the entity ).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SelectPaintEntity( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vObjectObj;
   zVIEW    vObjectObjOrig;
   zVIEW    vStructObj;
   zSHORT   nRC;
   zLONG    lZKey;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vStructObj, "FlatList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vObjectObjOrig, "LOD_Object", vSubtask, zLEVEL_TASK );

   // Use temp view so as not to lose position.
   CreateViewFromViewForTask( &vObjectObj, vObjectObjOrig, 0 );

   nRC = SetCursorFirstSelectedEntity( vObjectObj, "ER_Attribute", "LOD" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vObjectObj,
                               "LOD_Entity", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vStructObj, "W_Entity",
                                           "LOD_EntityZKey", lZKey, 0 );
      if ( nRC < zCURSOR_SET )
      {
         CreateEntity( vStructObj, "W_Entity", zPOS_AFTER );
         SetAttributeFromAttribute( vStructObj, "W_Entity",   "Name",
                                    vObjectObj, "LOD_Entity", "Name" );
         SetAttributeFromAttribute( vStructObj, "W_Entity",   "LOD_EntityZKey",
                                    vObjectObj, "LOD_Entity", "ZKey" );
      }

      GetIntegerFromAttribute( &lZKey, vObjectObj,
                               "LOD_Attribute", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vStructObj, "W_Attribute",
                                           "LOD_AttributeZKey", lZKey, 0 );

      // The attribute is not already in the list, so add it.
      if ( nRC != zCURSOR_SET )
      {
         CreateEntity( vStructObj, "W_Attribute", zPOS_AFTER );
         SetAttributeFromAttribute( vStructObj,  "W_Attribute",   "LOD_AttributeZKey",
                                     vObjectObj, "LOD_Attribute", "ZKey" );
         SetAttributeFromAttribute( vStructObj, "W_Attribute",  "Name",
                                    vObjectObj, "ER_Attribute", "Name" );
      }

      nRC = SetCursorNextSelectedEntity( vObjectObj, "ER_Attribute", "LOD" );
   }

   RefreshCtrl( vSubtask, "ATTRIB" );
   DropView( vObjectObj );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    DeletePaintAttribute
//
// PURPOSE:  This function Deletes an Attribute from the Paint
//           Attribute List.
//
//           Paint Attributes are deleted by double-clicking on
//           attributes to be deleted from the right-hand Attribute
//           list in the Select Attributes window in the WDOD
//           KZADWIAW.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_DeletePaintAttribute( zVIEW vSubtask )
{
   zVIEW    vStructObj;

   GetViewByName( &vStructObj, "FlatList", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vStructObj, "W_Attribute" ) == 0 )
      DeleteEntity( vStructObj, "W_Attribute", zREPOS_NEXT );

   RefreshCtrl( vSubtask, "ATTRIB" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    InitializeAutoDesign
//
// PURPOSE:  This operation intiializes the AutoDesign/Paint facility
//
// DESCRIPTION:  The AutoDesign Communication Object TZADWWKO is loaded
//               and intialized with the current active WDOD( either
//               KZADWOAW for AutoDesign from the main window or
//               KZADWIAW for AutoDesign from the Update WDOD Window ).
//
//               The the AutoDesign Active flag is set in the System
//               Object so the Object List dialogue will know which
//               WDOD to transfer to when an object is selected for
//               AutoDesign.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_InitializeAutoDesign( zVIEW vSubtask )
{
   zVIEW    vNewDialog;
   zVIEW    vAutoDesWk;
   zVIEW    vCtrlDef;
   zVIEW    vList;
   zVIEW    vFileUIS;
   zVIEW    vFileVOR;
   zCHAR    szParentName[ 33 ];
   zCHAR    szTag[ 33 ];
   zSHORT   nRC;

   nRC = GetViewByName( &vNewDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   // If nRC is -1 then there is no window and perhaps no dialog currently
   // activated in the painter.  There must be a window to autodesign so
   // give the user an error.
   if ( nRC == -1 )
   {
      MessageSend( vSubtask, "AD10615", "Autodesigner",
                   "No Window exists.  There must be a window to autodesign!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      return( 1 );
   }

   ActivateEmptyObjectInstance( &vAutoDesWk, "TZADWWKO", vSubtask, zSINGLE );
   SetNameForView( vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   CreateEntity( vAutoDesWk, "AutoDesignWork", zPOS_AFTER );
   strcpy_s( szParentName, zsizeof( szParentName ), "Window" );
   strcpy_s( szTag, zsizeof( szTag ), "Tag" );
   SetAttributeFromAttribute( vAutoDesWk, "AutoDesignWork",
                               "ObjectEntityName",
                               vNewDialog, "Window", "Tag" );
   SetAttributeFromInteger( vAutoDesWk, "AutoDesignWork",
                             "AutoDesignSubords", 0 );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "SaveRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "CancelRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "DeleteRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "NextRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "PrevRoutineFlagForRootEntity", " " );
   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                            "AutoDsgnListDialog", "N" );

   nRC = GetViewByName( &vCtrlDef, "TZADCTLD", vSubtask, zLEVEL_TASK );
   if ( nRC == -1 )
   {
      RetrieveViewForMetaList( vSubtask, &vList, zREFER_PENV_META );
      nRC = LoadZeidonPPE( vSubtask, &vCtrlDef, zREFER_PENV_META, vList,
                           "Autodesigner",
                           "Autodesigner aborted.");
      if ( nRC < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
         return( -1 );
      }

      SetNameForView( vCtrlDef, "TZADCTLD", vSubtask, zLEVEL_TASK );
   }

   nRC = GetViewByName( &vFileUIS, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( nRC == -1 )
   {
      nRC = RetrieveViewForMetaList( vSubtask, &vFileUIS, zREFER_UIS_META );
      SetNameForView( vFileUIS, "UI_SpecList", vSubtask, zLEVEL_TASK );
   }

   nRC = GetViewByName( &vFileVOR, "AD_VOR_List", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
   {
      // Check for the Object File List
      RetrieveViewForMetaList( vSubtask, &vFileVOR, zREFER_VOR_META );
      SetNameForView( vFileVOR, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    TerminateAutoDesign
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_TerminateAutoDesign( zVIEW vSubtask )
{
   zVIEW    vViewObj;

   // Delete the subordinate entity instance.
   GetViewByName( &vViewObj, "USSubEnt", vSubtask, zLEVEL_ANY );
   if ( vViewObj > 0 )
      DropNameForView( vViewObj, "USSubEnt", vSubtask, zLEVEL_TASK );

   // Delete the dialog specification instance.
   GetViewByName( &vViewObj, "User_Spec", vSubtask, zLEVEL_ANY );
   if ( vViewObj > 0 )
      DropObjectInstance( vViewObj );

   GetViewByName( &vViewObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   if ( vViewObj > 0 )
      DropMetaOI( vSubtask, vViewObj );

   // Delete the entity list instance.
   GetViewByName( &vViewObj, "EntityList", vSubtask, zLEVEL_ANY );
   if ( vViewObj > 0 )
      DropView( vViewObj );

   // Delete the autodesign communication instance.
   GetViewByName( &vViewObj, "TZADWWKO", vSubtask, zLEVEL_ANY );
   if ( vViewObj > 0 )
      DropView( vViewObj );

   // Delete the flat list of entities instance.
   GetViewByName( &vViewObj, "FlatList", vSubtask, zLEVEL_ANY );
   if ( vViewObj > 0 )
      DropView( vViewObj );

   // Delete the source file view.
   GetViewByName( &vViewObj, "TZADSRCO", vSubtask, zLEVEL_ANY );
   if ( vViewObj > 0 )
      DropView( vViewObj );

   GetViewByName( &vViewObj, "AD_VOR_List", vSubtask, zLEVEL_ANY );
   if ( vViewObj > 0 )
      DropView( vViewObj );

   GetViewByName( &vViewObj, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( vViewObj > 0 )
      DropView( vViewObj );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: LoadObjectFileList
//
// PURPOSE:  Checks whether the Object File List for Mapping has been
//           loaded, if it hasn't then the Object File List is loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_LoadObjectFileList( zVIEW vSubtask )
{
   zVIEW  vFileObj;
   zSHORT nRC;

   nRC = GetViewByName( &vFileObj, "AD_VOR_List", vSubtask, zLEVEL_ANY );

   if ( nRC == -1 )
   {
      // Check for the Object File List
      RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_VOR_META );
      SetNameForView( vFileObj, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_LoadObjLstForAutoDsgn( zVIEW vSubtask )
{
   zVIEW    vFileObj;
   zSHORT   nRC;

   nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_UIS_META );
   SetNameForView( vFileObj, "UI_SpecList", vSubtask, zLEVEL_TASK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SelectTheObject
//
// PURPOSE:  If the user is auto designing a window or group without a
//           style dialog then the window or group must be autodesigned
//           from a specified object.  This function sets the selected
//           object name into the auto design object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SelectTheObject( zVIEW vSubtask )
{
   zVIEW vFileObj;
   zVIEW vAutoDesWk;

   GetViewByName( &vFileObj, "AD_VOR_List", vSubtask, zLEVEL_ANY );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vFileObj, "W_MetaDef" ) != 0 )
   {
      // InformationMessage( "No OBJECTs exist.  You must create one.!" );
      MessageSend( vSubtask, "AD10617", "Autodesigner",
                   "No VORs exist.  You must create one.!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( 1 );
   }

   SetAttributeFromAttribute( vAutoDesWk, "AutoDesignWork", "ObjectName",
                              vFileObj, "W_MetaDef", "Name" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SelectObjectForAutoDesign
//
// PURPOSE:  In order to auto design, the user must select a dialog
//           style to auto design from.  This function sets the dialog
//           style name that the user selects into the work object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SelectObjForAutoDsgn( zVIEW vSubtask )
{
   zVIEW    vFileObj;
   zVIEW    vWorkObj;

   GetViewByName( &vWorkObj, "TZPNTROO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vFileObj, "UI_SpecList", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity ( vFileObj, "W_MetaDef" ) != 0 )
   {
      MessageSend( vSubtask, "AD10618", "Autodesigner",
                   "No Dialog Specification exist.  You must create one !",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( 1 );
   }

   SetAttributeFromAttribute( vWorkObj, "Palette", "UIS_Name", vFileObj, "W_MetaDef", "Name" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GrayMenuItems
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADWIAW_GrayMenuItems( zVIEW vSubtask )
{
   zVIEW vUserSpec;
   zCHAR cActionType[ 2 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( cActionType, zsizeof( cActionType ), vUserSpec, "WndDesign", "ActionType" );

   if ( cActionType[ 0 ] == 'O' )
   {
      SetCtrlState( vSubtask, "MenuTitle", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "MenuTitleText", zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      SetCtrlState( vSubtask, "MenuTitle", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MenuTitleText", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}
