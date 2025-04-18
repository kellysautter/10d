#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "TZVMLIP.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZADWWKO_ReturnAttrControlType( zVIEW     AnyView,
                                 zPCHAR    szDomainName,
                                 zPCHAR    szDomainType,
                                 zPCHAR    szControlType,
                                 zPCHAR    szUpdateFlag );


//:TRANSFORMATION OPERATION
//:AddAD_Dashbord( VIEW TZADWWKO   BASED ON LOD TZADWWKO,
//:                VIEW TZWINDOWL  BASED ON LOD TZWDLGSO,
//:                VIEW TZCONTROL  BASED ON LOD TZWDLGSO,
//:                VIEW AD_Base    BASED ON LOD TZWDLGSO,
//:                VIEW AD_BaseCtl BASED ON LOD TZWDLGSO )

//:   VIEW TaskLPLR    REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_Dashbord( zVIEW     TZADWWKO,
                          zVIEW     TZWINDOWL,
                          zVIEW     TZCONTROL,
                          zVIEW     AD_Base,
                          zVIEW     AD_BaseCtl )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZCtlHier   BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlHier = 0; 
   //:VIEW TZCtlTemp   BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlTemp = 0; 
   //:VIEW TaskLPLR2   BASED ON LOD  TZCMLPLO
   zVIEW     TaskLPLR2 = 0; 
   //:VIEW mDashBDfLOD BASED ON LOD  TZZOLODO
   zVIEW     mDashBDfLOD = 0; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOrigName
   zCHAR     szOrigName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAreaName
   zCHAR     szAreaName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAreaTitle
   zCHAR     szAreaTitle[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_DBE_Name
   zCHAR     szAD_DBE_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedEntityName 
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateWindow
   zCHAR     szTemplateWindow[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlName
   zCHAR     szControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szDashboardName
   zCHAR     szDashboardName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompare
   zCHAR     szCompare[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCSS_Class
   zCHAR     szCSS_Class[ 51 ] = { 0 }; 
   //:STRING ( 80 )  szReturnDialogPage
   zCHAR     szReturnDialogPage[ 81 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTargetVML
   zCHAR     szTargetVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceVML
   zCHAR     szSourceVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szControlText
   zCHAR     szControlText[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szMsg
   zCHAR     szMsg[ 257 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength 
   zLONG     nLength = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   //:INTEGER lInitialLevel
   zLONG     lInitialLevel = 0; 
   //:SHORT   lReturnedLevel 
   zSHORT    lReturnedLevel = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Generate the Static Dashboard Group.

   //:// Check that the wQueryR dialog exists.
   //:CreateViewFromView( TaskLPLR2, TaskLPLR )
   CreateViewFromView( &TaskLPLR2, TaskLPLR );
   //:NAME VIEW TaskLPLR2 "TaskLPLR2"
   SetNameForView( TaskLPLR2, "TaskLPLR2", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST TaskLPLR2.W_MetaType WHERE TaskLPLR2.W_MetaType.Type = 11   // 11 is read view of Dialogs.
   RESULT = SetCursorFirstEntityByInteger( TaskLPLR2, "W_MetaType", "Type", 11, "" );
   //:SET CURSOR FIRST TaskLPLR2.W_MetaDef WHERE TaskLPLR2.W_MetaDef.Name = "wQueryR" 
   RESULT = SetCursorFirstEntityByString( TaskLPLR2, "W_MetaDef", "Name", "wQueryR", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:             "A Dashboard Name has not been specified.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "A Dashboard Name has not been specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:DropView( TaskLPLR2 )
      DropView( TaskLPLR2 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// A Dashboard Name must be specfied.
   //:IF TZADWWKO.EntitySubGroup.DashboardName = ""
   if ( CompareAttributeToString( TZADWWKO, "EntitySubGroup", "DashboardName", "" ) == 0 )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:             "A Dashboard Name has not been specified.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "A Dashboard Name has not been specified.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupDashboardEntry"
   RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupDashboardEntry", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:             "DashboardEntry Control doesn't exist in AD_Base.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "DashboardEntry Control doesn't exist in AD_Base.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Activate mDashBDf LOD and add ViewObjRef as necessary.
   //:nRC = ActivateMetaOI_ByName( TZADWWKO, mDashBDfLOD, 0, zREFER_LOD_META, zSINGLE, "mDashBDf", 0 )
   nRC = ActivateMetaOI_ByName( TZADWWKO, &mDashBDfLOD, 0, zREFER_LOD_META, zSINGLE, "mDashBDf", 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Dashboard",
      //:             "LOD mDashBDf does not exist in LPLR.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Dashboard", "LOD mDashBDf does not exist in LPLR.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:NAME VIEW mDashBDfLOD "mDashBDfTemp"
   SetNameForView( mDashBDfLOD, "mDashBDfTemp", 0, zLEVEL_TASK );
   //:szDashboardName = TZADWWKO.EntitySubGroup.DashboardName 
   GetVariableFromAttribute( szDashboardName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "DashboardName", "", 0 );
   //:SET CURSOR FIRST TZWINDOWL.ViewObjRef WHERE TZWINDOWL.ViewObjRef.Name = szDashboardName
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ViewObjRef", "Name", szDashboardName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:AddRegisteredViewName( TZADWWKO, TZWINDOWL, mDashBDfLOD, szDashboardName )
      oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOWL, mDashBDfLOD, szDashboardName );
   } 

   //:END

   //:// Clone Control at current position. Note that Clone does not copy Action Operations.
   //:nRC = CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl )
   nRC = oTZADWWKO_CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl );

   //:// Get Group Area Name and Title values.
   //:szAreaName         = TZADWWKO.EntitySubGroup.GroupAreaName 
   GetVariableFromAttribute( szAreaName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:szAreaTitle        = TZADWWKO.EntitySubGroup.GroupAreaTitle
   GetVariableFromAttribute( szAreaTitle, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaTitle", "", 0 );
   //:szReturnDialogPage = TZWINDOWL.Dialog.Tag + TZWINDOWL.Window.Tag 
   GetStringFromAttribute( szReturnDialogPage, zsizeof( szReturnDialogPage ), TZWINDOWL, "Dialog", "Tag" );
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZWINDOWL, "Window", "Tag" );
   ZeidonStringConcat( szReturnDialogPage, 1, 0, szTempString_0, 1, 0, 81 );

   //:// Change data/names in controls as follows:
   //://   Set Mapping Name for TxtLastRun1 control which contains Last Run Date.
   //://      The mapping is to mDashBDf.DashboardDefinition.wLastRunDateTime1.
   //://   Set "Chart" Group Tag to Dashboard Name.
   //://   Convert _Section and _SectionTitle values for any Text Controls to Section and Title names.
   //://   Convert Group data for CSS Class of "collapse show"
   //:// Process the controls hierarchically looking for any specially named Controls as documented below.
   //:CreateViewFromView( TZCtlHier, TZCONTROL )
   CreateViewFromView( &TZCtlHier, TZCONTROL );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:lInitialLevel = lReturnedLevel
   lInitialLevel = lReturnedLevel;
   //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel 
   while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "CtrlCtrl"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlCtrl", 1, 0, 51 ) == 0 )
      { 
         //:// We are positioned on correct Control.
         //:szControlName = TZCtlHier.Control.Tag 
         GetVariableFromAttribute( szControlName, 0, 'S', 51, TZCtlHier, "Control", "Tag", "", 0 );

         //:// Process each special control type.
         //:IF szControlName = "TextLastRunPrompt" OR szControlName = "TextLastRunValue"
         if ( ZeidonStringCompare( szControlName, 1, 0, "TextLastRunPrompt", 1, 0, 51 ) == 0 || ZeidonStringCompare( szControlName, 1, 0, "TextLastRunValue", 1, 0, 51 ) == 0 )
         { 
            //:// First convert the Tag values of necessary controls.
            //:CreateViewFromView( TZCtlTemp, TZCtlHier )
            CreateViewFromView( &TZCtlTemp, TZCtlHier );
            //:NAME VIEW TZCtlTemp "TZCtlTemp"
            SetNameForView( TZCtlTemp, "TZCtlTemp", 0, zLEVEL_TASK );
            //:TZCtlTemp.Control.Tag = szControlName + TZADWWKO.EntitySubGroup.DashboardRunTimeSuffix  
            GetVariableFromAttribute( szTempString_2, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "DashboardRunTimeSuffix", "", 0 );
            ZeidonStringCopy( szTempString_1, 1, 0, szControlName, 1, 0, 33 );
            ZeidonStringConcat( szTempString_1, 1, 0, szTempString_2, 1, 0, 33 );
            SetAttributeFromString( TZCtlTemp, "Control", "Tag", szTempString_1 );

            //:// Next, process TextLastRunValue entry.
            //:IF szControlName = "TextLastRunValue"
            if ( ZeidonStringCompare( szControlName, 1, 0, "TextLastRunValue", 1, 0, 51 ) == 0 )
            { 
               //:// Set mapping to mDashBDf.DashboardDefinition.wLastRunDateTimeX where X is correct Suffix
               //:// Position on correct Run Date/Time attribute.
               //:SET CURSOR FIRST mDashBDfLOD.LOD_Entity WHERE mDashBDfLOD.LOD_Entity.Name = "DashboardDefinition" 
               RESULT = SetCursorFirstEntityByString( mDashBDfLOD, "LOD_Entity", "Name", "DashboardDefinition", "" );
               //:SET CURSOR FIRST mDashBDfLOD.ER_Attribute WITHIN mDashBDfLOD.LOD_Entity 
               //:           WHERE mDashBDfLOD.ER_Attribute.Name = "wLastRunDateTime1"  // The Attribute is always wLastRunDateTime1.
               RESULT = SetCursorFirstEntityByString( mDashBDfLOD, "ER_Attribute", "Name", "wLastRunDateTime1", "LOD_Entity" );
               //:INCLUDE TZCtlTemp.CtrlMapView FROM TZWINDOWL.ViewObjRef 
               RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
               //:INCLUDE TZCtlTemp.CtrlMapLOD_Attribute FROM mDashBDfLOD.LOD_Attribute 
               RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapLOD_Attribute", mDashBDfLOD, "LOD_Attribute", zPOS_AFTER );
            } 

            //:END
            //:DropView( TZCtlTemp )
            DropView( TZCtlTemp );
            //:ELSE
         } 
         else
         { 
            //:IF szControlName = "Chart"
            if ( ZeidonStringCompare( szControlName, 1, 0, "Chart", 1, 0, 51 ) == 0 )
            { 
               //:// Set Control Tag to Dashboard Name.
               //:CreateViewFromView( TZCtlTemp, TZCtlHier )
               CreateViewFromView( &TZCtlTemp, TZCtlHier );
               //:NAME VIEW TZCtlTemp "TZCtlTemp"
               SetNameForView( TZCtlTemp, "TZCtlTemp", 0, zLEVEL_TASK );
               //:TZCtlTemp.Control.Tag = szDashboardName
               SetAttributeFromString( TZCtlTemp, "Control", "Tag", szDashboardName );
               //:DropView( TZCtlTemp )
               DropView( TZCtlTemp );
               //:ELSE
            } 
            else
            { 
               //:IF szControlName = "txt_Section"
               if ( ZeidonStringCompare( szControlName, 1, 0, "txt_Section", 1, 0, 51 ) == 0 )
               { 
                  //:// Alter Tag and Text below.
                  //:CreateViewFromView( TZCtlTemp, TZCtlHier )
                  CreateViewFromView( &TZCtlTemp, TZCtlHier );
                  //:NAME VIEW TZCtlTemp "TZCtlTemp"
                  SetNameForView( TZCtlTemp, "TZCtlTemp", 0, zLEVEL_TASK );

                  //:// Header Tag
                  //:TZCtlTemp.Control.Tag = "txt" + szAreaName
                  ZeidonStringCopy( szTempString_3, 1, 0, "txt", 1, 0, 33 );
                  ZeidonStringConcat( szTempString_3, 1, 0, szAreaName, 1, 0, 33 );
                  SetAttributeFromString( TZCtlTemp, "Control", "Tag", szTempString_3 );

                  //:// Header Text
                  //:szControlText = TZCtlTemp.Control.Text 
                  GetVariableFromAttribute( szControlText, 0, 'S', 257, TZCtlTemp, "Control", "Text", "", 0 );
                  //:zSearchAndReplace( szControlText, 200, "_SectionTitle", szAreaTitle )  // Title must be replaced first because it also has _Section it it.
                  zSearchAndReplace( szControlText, 200, "_SectionTitle", szAreaTitle );
                  //:zSearchAndReplace( szControlText, 200, "_Section", szAreaName )
                  zSearchAndReplace( szControlText, 200, "_Section", szAreaName );
                  //:TZCtlTemp.Control.Text = szControlText
                  SetAttributeFromString( TZCtlTemp, "Control", "Text", szControlText );
                  //:DropView( TZCtlTemp )
                  DropView( TZCtlTemp );
                  //:ELSE
               } 
               else
               { 
                  //:szCSS_Class = TZCtlHier.Control.CSS_Class 
                  GetVariableFromAttribute( szCSS_Class, 0, 'S', 51, TZCtlHier, "Control", "CSS_Class", "", 0 );
                  //:IF szCSS_Class = "collapse show"
                  if ( ZeidonStringCompare( szCSS_Class, 1, 0, "collapse show", 1, 0, 51 ) == 0 )
                  { 
                     //:CreateViewFromView( TZCtlTemp, TZCtlHier )
                     CreateViewFromView( &TZCtlTemp, TZCtlHier );
                     //:NAME VIEW TZCtlTemp "TZCtlTemp"
                     SetNameForView( TZCtlTemp, "TZCtlTemp", 0, zLEVEL_TASK );

                     //:// Set the GroupBox Tag for the "collapse show" Group to AreaName.
                     //:TZCtlTemp.Control.Tag = szAreaName
                     SetAttributeFromString( TZCtlTemp, "Control", "Tag", szAreaName );

                     //:// Convert the HTML5 text to replace _Section with AreaName.
                     //:szControlText = TZCtlTemp.Control.WebHTML5Attribute 
                     GetVariableFromAttribute( szControlText, 0, 'S', 257, TZCtlTemp, "Control", "WebHTML5Attribute", "", 0 );
                     //:zSearchAndReplace( szControlText, 200, "_Section", szAreaName )
                     zSearchAndReplace( szControlText, 200, "_Section", szAreaName );
                     //:TZCtlTemp.Control.WebHTML5Attribute = szControlText
                     SetAttributeFromString( TZCtlTemp, "Control", "WebHTML5Attribute", szControlText );

                     //:DropView( TZCtlTemp )
                     DropView( TZCtlTemp );
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END

      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )
   DropView( TZCtlHier );
   //:DropObjectInstance( mDashBDfLOD )
   DropObjectInstance( mDashBDfLOD );

   //:// Add the 2 Actions and 3 Operations for the Dashboard Entry.
   //:Count = 0
   Count = 0;
   //:LOOP WHILE Count < 3
   while ( Count < 3 )
   { 

      //:// Set Action/Oper orginal name.
      //:Count = Count + 1
      Count = Count + 1;
      //:IF Count = 1
      if ( Count == 1 )
      { 
         //:szOrigName = "GOTO_ShowDBE__AD_Area"
         ZeidonStringCopy( szOrigName, 1, 0, "GOTO_ShowDBE__AD_Area", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:IF Count = 2
         if ( Count == 2 )
         { 
            //:szOrigName = "GOTO_RunDBE__AD_Area"
            ZeidonStringCopy( szOrigName, 1, 0, "GOTO_RunDBE__AD_Area", 1, 0, 51 );
            //:ELSE
         } 
         else
         { 
            //:szOrigName = "INIT_DBE__AD_Area"
            ZeidonStringCopy( szOrigName, 1, 0, "INIT_DBE__AD_Area", 1, 0, 51 );
         } 

         //:END
      } 

      //:END

      //:// Convert Action Name which is also used for Operation without an action.
      //:szActionName = szOrigName
      ZeidonStringCopy( szActionName, 1, 0, szOrigName, 1, 0, 51 );
      //:zSearchAndReplace( szActionName, 50, "_AD_Area", szAreaName )
      zSearchAndReplace( szActionName, 50, "_AD_Area", szAreaName );

      //:// Convert Actions for first 2 entries
      //:IF Count < 3
      if ( Count < 3 )
      { 
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = szOrigName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szOrigName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:TraceLineS( "*** No Action match on ", szOrigName )
            TraceLineS( "*** No Action match on ", szOrigName );
            //:IssueError( TZWINDOWL,0,0, "Programming Error: No Action match for Dashboard Actions." )
            IssueError( TZWINDOWL, 0, 0, "Programming Error: No Action match for Dashboard Actions." );
            //:RETURN 
            return( 0 );
         } 

         //:END
         //:TZWINDOWL.Action.Tag = szActionName
         SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
      } 

      //:END

      //:// Position on Operation and convert name.
      //:SET CURSOR FIRST AD_Base.Operation WITHIN AD_Base.Dialog 
      //:           WHERE AD_Base.Operation.Name = szOrigName
      RESULT = SetCursorFirstEntityByString( AD_Base, "Operation", "Name", szOrigName, "Dialog" );
      //:SET CURSOR LAST TZWINDOWL.Operation
      RESULT = SetCursorLastEntity( TZWINDOWL, "Operation", "" );
      //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER );
      //:TZWINDOWL.Operation.Name = szActionName
      SetAttributeFromString( TZWINDOWL, "Operation", "Name", szActionName );
      //:SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL )
      SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL );
      //:FOR EACH AD_Base.Parameter 
      RESULT = SetCursorFirstEntity( AD_Base, "Parameter", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER )
         CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL )
         SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL );
         RESULT = SetCursorNextEntity( AD_Base, "Parameter", "" );
      } 

      //:END

      //:// For the first 2 Actions, include the Operation under the Action.
      //:IF Count < 3
      if ( Count < 3 )
      { 
         //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOWL.Operation 
         RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOWL, "Operation", zPOS_AFTER );
      } 

      //:END
   } 

   //:END

   //:// Determine if Operation VML Code for this Dashboard entry already exists.
   //:// We will just check the Run operation to determine this.
   //:// nRC2 will be 0 for a match.
   //:szSourceDirectory  = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szSourceVML        = szSourceDirectory + "\" + TZWINDOWL.SourceFile.Name + ".VML"
   ZeidonStringCopy( szSourceVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szSourceVML, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_4, 0, 'S', 33, TZWINDOWL, "SourceFile", "Name", "", 0 );
   ZeidonStringConcat( szSourceVML, 1, 0, szTempString_4, 1, 0, 257 );
   ZeidonStringConcat( szSourceVML, 1, 0, ".VML", 1, 0, 257 );
   //:szOperationName = "GOTO_RunDBE_" + szAreaName
   ZeidonStringCopy( szOperationName, 1, 0, "GOTO_RunDBE_", 1, 0, 51 );
   ZeidonStringConcat( szOperationName, 1, 0, szAreaName, 1, 0, 51 );
   //:nLength = zGetStringLen( szOperationName )
   nLength = zGetStringLen( szOperationName );
   //:nFileIn   = SysOpenFile( TZWINDOWL, szSourceVML, COREFILE_READ ) 
   nFileIn = SysOpenFile( TZWINDOWL, szSourceVML, COREFILE_READ );
   //:nRC2 = -1 
   nRC2 = -1;
   //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 ) 
   nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
   //:LOOP WHILE nRC = 1 AND nRC2 != 0
   while ( nRC == 1 && nRC2 != 0 )
   { 
      //:strncpy_s( szCompare, szVML_Statement, nLength )
      strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
      //:nRC2 = zstrcmp( szCompare, szOperationName )
      nRC2 = zstrcmp( szCompare, szOperationName );
      //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
   } 

   //:END
   //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
   SysCloseFile( TZWINDOWL, nFileIn, 0 );

   //:// Generate the VML code if the operation was not found.
   //:IF nRC2 != 0
   if ( nRC2 != 0 )
   { 
      //:// Copy Operation VML Code from ObjDashboard.VML file to the end of the target VML, converting the variable characters in the process.
      //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
      GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
      //:szTargetVML       = szSourceDirectory + "\" + TZWINDOWL.SourceFile.Name + ".VML"
      ZeidonStringCopy( szTargetVML, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szTargetVML, 1, 0, "\\", 1, 0, 257 );
      GetVariableFromAttribute( szTempString_5, 0, 'S', 33, TZWINDOWL, "SourceFile", "Name", "", 0 );
      ZeidonStringConcat( szTargetVML, 1, 0, szTempString_5, 1, 0, 257 );
      ZeidonStringConcat( szTargetVML, 1, 0, ".VML", 1, 0, 257 );
      //:szAD_DBE_Name = TZADWWKO.EntitySubGroup.DashboardName 
      GetVariableFromAttribute( szAD_DBE_Name, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "DashboardName", "", 0 );
      //:nFileIn   = SysOpenFile( TZWINDOWL, "c:\lplr\AD_Base\ObjDashboard.VML", COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOWL, "c:\\lplr\\AD_Base\\ObjDashboard.VML", COREFILE_READ );
      //:nFileOut  = SysOpenFile( TZWINDOWL, szTargetVML, COREFILE_APPEND )  
      nFileOut = SysOpenFile( TZWINDOWL, szTargetVML, COREFILE_APPEND );
      //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 ) 
      nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_Area",           szAreaName )         // Group Area Name
         zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAreaName );
         //:zSearchAndReplace( szVML_Statement, 256, "_DashboardName",     szAD_DBE_Name )      // Dashboard Name
         zSearchAndReplace( szVML_Statement, 256, "_DashboardName", szAD_DBE_Name );
         //:zSearchAndReplace( szVML_Statement, 256, "_CurrentDialogPage", szReturnDialogPage ) // Combination Dialog & Window Name
         zSearchAndReplace( szVML_Statement, 256, "_CurrentDialogPage", szReturnDialogPage );
         //:SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement )
         SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement );
         //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
      SysCloseFile( TZWINDOWL, nFileIn, 0 );
      //:SysCloseFile( TZWINDOWL, nFileOut, 0 )
      SysCloseFile( TZWINDOWL, nFileOut, 0 );

      //:// Send the User a message that the call to the INIT operation must be added to the Postbuild operation for this
      //:// page/window.
      //:szMsg = "The following statement must be added to the Postbuild operation for this page/window to call the INIT " +
      //:        "operation during postbuild processing:" + NEW_LINE + "INIT_DBE_" + szAreaName + "( ViewToWindow )"
      ZeidonStringCopy( szMsg, 1, 0, "The following statement must be added to the Postbuild operation for this page/window to call the INIT ", 1, 0, 257 );
      ZeidonStringConcat( szMsg, 1, 0, "operation during postbuild processing:", 1, 0, 257 );
      ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 257 );
      ZeidonStringConcat( szMsg, 1, 0, "INIT_DBE_", 1, 0, 257 );
      ZeidonStringConcat( szMsg, 1, 0, szAreaName, 1, 0, 257 );
      ZeidonStringConcat( szMsg, 1, 0, "( ViewToWindow )", 1, 0, 257 );
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddAD_GoExclActions( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                     VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                     VIEW AD_Base   BASED ON LOD TZWDLGSO )

//:   VIEW TaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_GoExclActions( zVIEW     TZADWWKO,
                               zVIEW     TZWINDOWL,
                               zVIEW     AD_Base )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szSourceOperationName
   zCHAR     szSourceOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTargetOperationName
   zCHAR     szTargetOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szEntityName
   zCHAR     szEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szCompare
   zCHAR     szCompare[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceFileDirectory
   zCHAR     szSourceFileDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTargetFileDirectory
   zCHAR     szTargetFileDirectory[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength 
   zLONG     nLength = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Process GOTO_SelectDetail Action
   //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = "GOTO_SelectDetail"
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", "GOTO_SelectDetail", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:             "GOTO_SelectDetail Action doesn't exist in template.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "GOTO_SelectDetail Action doesn't exist in template.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:szAD_Area = TZADWWKO.EntitySubGroup.GroupAreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:TZWINDOWL.Action.Tag = "GOTO_Select" + szAD_Area
   ZeidonStringCopy( szTempString_0, 1, 0, "GOTO_Select", 1, 0, 33 );
   ZeidonStringConcat( szTempString_0, 1, 0, szAD_Area, 1, 0, 33 );
   SetAttributeFromString( TZWINDOWL, "Action", "Tag", szTempString_0 );
   //:TZWINDOWL.Action.DialogName = TZWINDOWL.Dialog.Tag 
   SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOWL, "Dialog", "Tag" );
   //:TZWINDOWL.Action.WindowName = TZADWWKO.ESG_LOD_Entity.Name + "Include"
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
   ZeidonStringConcat( szTempString_1, 1, 0, "Include", 1, 0, 33 );
   SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_1 );

   //:// Process EXCLUDE_For_AD_Area Action 
   //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = "EXCLUDE_For_AD_Area"
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", "EXCLUDE_For_AD_Area", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:             "EXCLUDE_For_AD_Area Action doesn't exist in template.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "EXCLUDE_For_AD_Area Action doesn't exist in template.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:szActionName = "EXCLUDE_For" + szAD_Area
   ZeidonStringCopy( szActionName, 1, 0, "EXCLUDE_For", 1, 0, 51 );
   ZeidonStringConcat( szActionName, 1, 0, szAD_Area, 1, 0, 51 );
   //:TZWINDOWL.Action.Tag = szActionName
   SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );

   //:// Create Operation and include in Action.
   //:SET CURSOR FIRST AD_Base.Operation WITHIN AD_Base.Dialog 
   //:           WHERE AD_Base.Operation.Name = "EXCLUDE_For_AD_Area"
   RESULT = SetCursorFirstEntityByString( AD_Base, "Operation", "Name", "EXCLUDE_For_AD_Area", "Dialog" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:             "Operation EXCLUDE_For_AD_Area doesn't exist in template.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "Operation EXCLUDE_For_AD_Area doesn't exist in template.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END  
   //:SET CURSOR LAST TZWINDOWL.Operation   
   RESULT = SetCursorLastEntity( TZWINDOWL, "Operation", "" );
   //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER )
   CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER );
   //:TZWINDOWL.Operation.Name = szActionName
   SetAttributeFromString( TZWINDOWL, "Operation", "Name", szActionName );
   //:SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL )
   SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL );
   //:FOR EACH AD_Base.Parameter 
   RESULT = SetCursorFirstEntity( AD_Base, "Parameter", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL )
      SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL );
      RESULT = SetCursorNextEntity( AD_Base, "Parameter", "" );
   } 

   //:END 
   //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOWL.Operation 
   RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOWL, "Operation", zPOS_AFTER );


   //:// COPY AND CONVERT EXCLUDE_For_AD_Area OPERATION VML.

   //:// Source Directory is from AD_Base
   //:szSourceFileDirectory = "C:\LPLR\AD_Base\ObjExcl.VML"
   ZeidonStringCopy( szSourceFileDirectory, 1, 0, "C:\\LPLR\\AD_Base\\ObjExcl.VML", 1, 0, 257 );

   //:// Target Directory is current LPLR.
   //:szTargetFileDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szTargetFileDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szTargetFileDirectory = szTargetFileDirectory + "\" + TZWINDOWL.SourceFile.Name + ".VML"
   ZeidonStringConcat( szTargetFileDirectory, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZWINDOWL, "SourceFile", "Name", "", 0 );
   ZeidonStringConcat( szTargetFileDirectory, 1, 0, szTempString_2, 1, 0, 257 );
   ZeidonStringConcat( szTargetFileDirectory, 1, 0, ".VML", 1, 0, 257 );

   //:// Check if Exclude operation is already in target VML file.
   //:szTargetOperationName = szActionName
   ZeidonStringCopy( szTargetOperationName, 1, 0, szActionName, 1, 0, 51 );
   //:nLength = zGetStringLen( szTargetOperationName )
   nLength = zGetStringLen( szTargetOperationName );
   //:nFileIn = SysOpenFile( TZWINDOWL, szTargetFileDirectory, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOWL, szTargetFileDirectory, COREFILE_READ );
   //:nRC2    = -1
   nRC2 = -1;
   //:IF nFileIn > 0 
   if ( nFileIn > 0 )
   { 
      //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 AND nRC2 != 0
      while ( nRC == 1 && nRC2 != 0 )
      { 
         //:strncpy_s( szCompare, szVML_Statement, nLength )
         strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
         //:nRC2 = zstrcmp( szCompare, szTargetOperationName )
         nRC2 = zstrcmp( szCompare, szTargetOperationName );
         //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
      SysCloseFile( TZWINDOWL, nFileIn, 0 );
      //:ELSE 
   } 
   else
   { 
      //:IssueError( TZADWWKO,0,0, "Target VML doesn't exist yet." )
      IssueError( TZADWWKO, 0, 0, "Target VML doesn't exist yet." );
      //:RETURN   // Don't continue if the VML hasn't been created yet
      return( 0 );
   } 

   //:END

   //:IF nRC2 != 0     // Operation was not found.
   if ( nRC2 != 0 )
   { 

      //:// The Exclude Operation was not found in the target Source File, so copy source VML to end of target VML,
      //:// converting the variable characters in the process.
      //:szEntityName = TZADWWKO.ESG_LOD_Entity.Name 
      GetVariableFromAttribute( szEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
      //:szAD_UpdObj  = TZADWWKO.W_MetaDefUpdateObject.Name
      GetVariableFromAttribute( szAD_UpdObj, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:nFileIn   = SysOpenFile( TZWINDOWL, szSourceFileDirectory, COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOWL, szSourceFileDirectory, COREFILE_READ );
      //:nFileOut  = SysOpenFile( TZWINDOWL, szTargetFileDirectory, COREFILE_APPEND )
      nFileOut = SysOpenFile( TZWINDOWL, szTargetFileDirectory, COREFILE_APPEND );
      //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 
         //:// Loop through source vml and convert variable data before copying to target.
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_Area",  szAD_Area )        // Area Name
         zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO",  szAD_UpdObj )      // Entity Name
         zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdObj );
         //:zSearchAndReplace( szVML_Statement, 256, "_EntityName",  szEntityName )  // Update Object Name
         zSearchAndReplace( szVML_Statement, 256, "_EntityName", szEntityName );

         //:SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement )
         SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement );

         //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      } 

      //:END

      //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
      SysCloseFile( TZWINDOWL, nFileIn, 0 );
      //:SysCloseFile( TZWINDOWL, nFileOut, 0 )
      SysCloseFile( TZWINDOWL, nFileOut, 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddAD_ListInclActions( VIEW TZADWWKO   BASED ON LOD  TZADWWKO,
//:                       VIEW TZCONTROL  BASED ON LOD  TZWDLGSO,
//:                       VIEW TZWINDOWL  BASED ON LOD  TZWDLGSO,
//:                       VIEW AD_BaseCtl BASED ON LOD  TZWDLGSO,
//:                       VIEW AD_Base    BASED ON LOD  TZWDLGSO )

//:   VIEW TaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_ListInclActions( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     TZWINDOWL,
                                 zVIEW     AD_BaseCtl,
                                 zVIEW     AD_Base )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOrigName
   zCHAR     szOrigName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompare
   zCHAR     szCompare[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szEntityName
   zCHAR     szEntityName[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength 
   zLONG     nLength = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Process CrtlCrtl entries recursively for building Actions for AD New and Delete/Remove entries.
   //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
   SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:SET CURSOR FIRST TZCONTROL.Control  
   RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
   //:FOR EACH AD_BaseCtl.Control
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF AD_BaseCtl.Control.Text = "New" OR AD_BaseCtl.Control.Text = "Delete" OR AD_BaseCtl.Control.Text = "Remove"
      if ( CompareAttributeToString( AD_BaseCtl, "Control", "Text", "New" ) == 0 || CompareAttributeToString( AD_BaseCtl, "Control", "Text", "Delete" ) == 0 || CompareAttributeToString( AD_BaseCtl, "Control", "Text", "Remove" ) == 0 )
      { 
         //:// Update the New Action if necessary and tie it to the Control.
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = AD_BaseCtl.EventAct.Tag 
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "EventAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szTempString_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:TraceLineS( "*** No Action match on ", AD_BaseCtl.EventAct.Tag )
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "EventAct", "Tag" );
            TraceLineS( "*** No Action match on ", szTempString_0 );
            //:IssueError( TZWINDOWL,0,0, "Programming Error: No Action match for Update Actions." )
            IssueError( TZWINDOWL, 0, 0, "Programming Error: No Action match for Update Actions." );
            //:RETURN 
            return( 0 );
         } 

         //:END

         //:// If the Action is New, set the Dialog and Window names to the correct update Window.
         //:IF AD_BaseCtl.Control.Text = "New" 
         if ( CompareAttributeToString( AD_BaseCtl, "Control", "Text", "New" ) == 0 )
         { 
            //:TZWINDOWL.Action.DialogName = TZWINDOWL.Dialog.Tag 
            SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOWL, "Dialog", "Tag" );
            //:TZWINDOWL.Action.WindowName = TZADWWKO.ESG_LOD_Entity.Name + "Include"
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TZADWWKO, "ESG_LOD_Entity", "Name" );
            ZeidonStringConcat( szTempString_1, 1, 0, "Include", 1, 0, 33 );
            SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_1 );
         } 

         //:END

         //:// Add Suffix to Action Name.
         //:szActionName = TZWINDOWL.Action.Tag
         GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL, "Action", "Tag", "", 0 );
         //:szOrigName   = szActionName
         ZeidonStringCopy( szOrigName, 1, 0, szActionName, 1, 0, 51 );
         //:szAD_Area    = TZADWWKO.EntitySubGroup.IncludeOperationSuffix
         GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "IncludeOperationSuffix", "", 0 );
         //:zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area )
         zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area );
         //:TZWINDOWL.Action.Tag = szActionName
         SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );

         //:// If the Action has an operation, add it, including the VML.
         //:// The Operation Name is the same as the Action Name.
         //:SET CURSOR FIRST AD_Base.Action WHERE AD_Base.Action.Tag = szOrigName
         RESULT = SetCursorFirstEntityByString( AD_Base, "Action", "Tag", szOrigName, "" );
         //:IF AD_Base.ActOper EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( AD_Base, "ActOper" );
         if ( lTempInteger_0 == 0 )
         { 
            //:SET CURSOR FIRST AD_Base.Operation WITHIN AD_Base.Dialog 
            //:           WHERE AD_Base.Operation.Name = szOrigName
            RESULT = SetCursorFirstEntityByString( AD_Base, "Operation", "Name", szOrigName, "Dialog" );
            //:SET CURSOR LAST TZWINDOWL.Operation
            RESULT = SetCursorLastEntity( TZWINDOWL, "Operation", "" );
            //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER );
            //:TZWINDOWL.Operation.Name = szActionName
            SetAttributeFromString( TZWINDOWL, "Operation", "Name", szActionName );
            //:SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL );
            //:FOR EACH AD_Base.Parameter 
            RESULT = SetCursorFirstEntity( AD_Base, "Parameter", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER )
               CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL )
               SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL );
               RESULT = SetCursorNextEntity( AD_Base, "Parameter", "" );
            } 

            //:END
            //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOWL.Operation 
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOWL, "Operation", zPOS_AFTER );

            //:// Check to see if the Operation exists in the VML File.
            //:// Note that the Operation Name starts in column 1.
            //:szCompare = szOrigName
            ZeidonStringCopy( szCompare, 1, 0, szOrigName, 1, 0, 51 );
            //:nLength = zGetStringLen( szActionName )
            nLength = zGetStringLen( szActionName );
            //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
            GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
            //:szFindVML = szSourceDirectory + "\" + TZWINDOWL.Dialog.Tag + ".VML"
            ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
            ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
            GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZWINDOWL, "Dialog", "Tag", "", 0 );
            ZeidonStringConcat( szFindVML, 1, 0, szTempString_2, 1, 0, 257 );
            ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
            //:nFileIn   = SysOpenFile( TZWINDOWL, szFindVML, COREFILE_READ )
            nFileIn = SysOpenFile( TZWINDOWL, szFindVML, COREFILE_READ );
            //:nRC2 = -1
            nRC2 = -1;
            //:IF nFileIn > 0   // Don't continue if the VML hasn't been created yet.
            if ( nFileIn > 0 )
            { 
               //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
               nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
               //:LOOP WHILE nRC = 1 AND nRC2 != 0
               while ( nRC == 1 && nRC2 != 0 )
               { 
                  //:strncpy_s( szCompare, szVML_Statement, nLength )
                  strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
                  //:nRC2 = zstrcmp( szCompare, szActionName )
                  nRC2 = zstrcmp( szCompare, szActionName );
                  //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
                  nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
               } 

               //:END
               //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
               SysCloseFile( TZWINDOWL, nFileIn, 0 );
            } 

            //:END

            //:IF nRC2 != 0     // Operation was not found.
            if ( nRC2 != 0 )
            { 

               //:// The Operation was not found in the Source File, so copy the VML

               //:// Copy the source VML to the end of the target Find VML, converting the variable characters in the process.
               //:szAD_UpdObj  = TZADWWKO.W_MetaDefUpdateObject.Name 
               GetVariableFromAttribute( szAD_UpdObj, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
               //:szEntityName = TZADWWKO.ESG_LOD_Entity.Name 
               GetVariableFromAttribute( szEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
               //:nFileIn   = SysOpenFile( TZWINDOWL, "c:\lplr\AD_Base\ObjExcl.VML", COREFILE_READ ) 
               nFileIn = SysOpenFile( TZWINDOWL, "c:\\lplr\\AD_Base\\ObjExcl.VML", COREFILE_READ );
               //:nFileOut  = SysOpenFile( TZWINDOWL, szFindVML, COREFILE_APPEND )
               nFileOut = SysOpenFile( TZWINDOWL, szFindVML, COREFILE_APPEND );
               //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 ) 
               nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
               //:LOOP WHILE nRC = 1 
               while ( nRC == 1 )
               { 
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_Area",    szAD_Area )      // Suffix
                  zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area );
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO",    szAD_UpdObj )    // Update Object Name
                  zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdObj );
                  //:zSearchAndReplace( szVML_Statement, 256, "_EntityName", szEntityName )   // Entity Name
                  zSearchAndReplace( szVML_Statement, 256, "_EntityName", szEntityName );
                  //:SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement )
                  SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement );
                  //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
                  nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
               } 

               //:END

               //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
               SysCloseFile( TZWINDOWL, nFileIn, 0 );
               //:SysCloseFile( TZWINDOWL, nFileOut, 0 )
               SysCloseFile( TZWINDOWL, nFileOut, 0 );
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:IF AD_BaseCtl.CtrlCtrl EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( AD_BaseCtl, "CtrlCtrl" );
         if ( lTempInteger_1 == 0 )
         { 
            //:AddAD_ListInclActions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
            oTZADWWKO_AddAD_ListInclActions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );
         } 

         //:END
      } 

      //:END
      //:SET CURSOR NEXT TZCONTROL.Control  
      RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
      RESULT = SetCursorNextEntity( AD_BaseCtl, "Control", "" );
   } 

   //:END
   //:ResetViewFromSubobject( AD_BaseCtl )
   ResetViewFromSubobject( AD_BaseCtl );
   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddAD_ListMGP_Actions( VIEW TZADWWKO   BASED ON LOD  TZADWWKO,
//:                       VIEW TZCONTROL  BASED ON LOD  TZWDLGSO,
//:                       VIEW TZWINDOWL  BASED ON LOD  TZWDLGSO,
//:                       VIEW AD_BaseCtl BASED ON LOD  TZWDLGSO,
//:                       VIEW AD_Base    BASED ON LOD  TZWDLGSO )

//:   VIEW TaskLPLR    REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_ListMGP_Actions( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     TZWINDOWL,
                                 zVIEW     AD_BaseCtl,
                                 zVIEW     AD_Base )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZADWWKOSub BASED ON LOD  TZADWWKO
   zVIEW     TZADWWKOSub = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompare
   zCHAR     szCompare[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_MainV
   zCHAR     szAD_MainV[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_ListE
   zCHAR     szAD_ListE[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_ID 
   zCHAR     szAD_ID[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_MGP
   zCHAR     szAD_MGP[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_MGPO
   zCHAR     szAD_MGPO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_MGPRoot
   zCHAR     szAD_MGPRoot[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength  
   zLONG     nLength = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Check for Update LOD, which is the main LOD for the multi-group page for both Update and Detail.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_UpdateMGP"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_UpdateMGP", TZADWWKO, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:             "The Update Object has not been selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "The Update Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Process the GOTO__AreaMGP Action and Operation.
   //:GET VIEW TZADWWKOSub NAMED "TZADWWKOSub"
   RESULT = GetViewByName( &TZADWWKOSub, "TZADWWKOSub", TZADWWKO, zLEVEL_TASK );
   //:szAD_Area    = TZADWWKOSub.AutodesignSubdialog.AD_AreaName
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKOSub, "AutodesignSubdialog", "AD_AreaName", "", 0 );
   //:szActionName = "GOTO_" + szAD_Area + "MGP"
   ZeidonStringCopy( szActionName, 1, 0, "GOTO_", 1, 0, 51 );
   ZeidonStringConcat( szActionName, 1, 0, szAD_Area, 1, 0, 51 );
   ZeidonStringConcat( szActionName, 1, 0, "MGP", 1, 0, 51 );
   //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = "GOTO__MGP"
   RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", "GOTO__MGP", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:IssueError( TZWINDOWL,0,0, "Programming Error: No Action match for GOTO__MGP Action." )
      IssueError( TZWINDOWL, 0, 0, "Programming Error: No Action match for GOTO__MGP Action." );
      //:RETURN 
      return( 0 );
   } 

   //:END
   //:TZWINDOWL.Action.Tag = szActionName
   SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
   //:TZWINDOWL.Action.DialogName = TZWINDOWL.Dialog.Tag 
   SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOWL, "Dialog", "Tag" );
   //:TZWINDOWL.Action.WindowName = szAD_Area + "Detail"
   ZeidonStringCopy( szTempString_0, 1, 0, szAD_Area, 1, 0, 33 );
   ZeidonStringConcat( szTempString_0, 1, 0, "Detail", 1, 0, 33 );
   SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_0 );

   //:// Copy Operation and include under Action.
   //:SET CURSOR FIRST AD_Base.Operation WITHIN AD_Base.Dialog 
   //:           WHERE AD_Base.Operation.Name = "GOTO__MGP"
   RESULT = SetCursorFirstEntityByString( AD_Base, "Operation", "Name", "GOTO__MGP", "Dialog" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:IssueError( TZWINDOWL,0,0, "Programming Error: No Operation match for GOTO__MGP Operation." )
      IssueError( TZWINDOWL, 0, 0, "Programming Error: No Operation match for GOTO__MGP Operation." );
      //:RETURN 
      return( 0 );
   } 

   //:END
   //:SET CURSOR LAST TZWINDOWL.Operation
   RESULT = SetCursorLastEntity( TZWINDOWL, "Operation", "" );
   //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER )
   CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER );
   //:szOperationName = szActionName
   ZeidonStringCopy( szOperationName, 1, 0, szActionName, 1, 0, 51 );
   //:TZWINDOWL.Operation.Name = szOperationName
   SetAttributeFromString( TZWINDOWL, "Operation", "Name", szOperationName );
   //:SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL )
   SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL );
   //:FOR EACH AD_Base.Parameter 
   RESULT = SetCursorFirstEntity( AD_Base, "Parameter", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER )
      CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL )
      SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL );
      RESULT = SetCursorNextEntity( AD_Base, "Parameter", "" );
   } 

   //:END
   //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOWL.Operation 
   RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOWL, "Operation", zPOS_AFTER );

   //:// Check to see if the GOTO Operation exists in the VML File. Operation Name is same as Action Name
   //:// Note that the Operation Name starts in column 1.
   //:nLength = zGetStringLen( szOperationName )
   nLength = zGetStringLen( szOperationName );
   //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szFindVML = szSourceDirectory + "\" + TZWINDOWL.Dialog.Tag + ".VML"
   ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZWINDOWL, "Dialog", "Tag", "", 0 );
   ZeidonStringConcat( szFindVML, 1, 0, szTempString_1, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
   //:nFileIn   = SysOpenFile( TZWINDOWL, szFindVML, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOWL, szFindVML, COREFILE_READ );
   //:nRC2 = -1
   nRC2 = -1;
   //:IF nFileIn > 0   // Don't continue if the VML hasn't been created yet.
   if ( nFileIn > 0 )
   { 
      //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 AND nRC2 != 0
      while ( nRC == 1 && nRC2 != 0 )
      { 
         //:strncpy_s( szCompare, szVML_Statement, nLength )
         strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
         //:nRC2 = zstrcmp( szCompare, szOperationName )
         nRC2 = zstrcmp( szCompare, szOperationName );
         //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
      SysCloseFile( TZWINDOWL, nFileIn, 0 );
   } 

   //:END

   //:IF nRC2 != 0     // Operation was not found.
   if ( nRC2 != 0 )
   { 

      //:// The Operation was not found in the Source File, so copy the VML
      //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir
      GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
      //:szFindVML         = szSourceDirectory + "\" + TZWINDOWL.Dialog.Tag + ".VML" 
      ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZWINDOWL, "Dialog", "Tag", "", 0 );
      ZeidonStringConcat( szFindVML, 1, 0, szTempString_2, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );

      //:// Set variables to be used in VML conversion.
      //:// Check to see if an Activate entity was specifified. If so use it. Otherwise use the List entity.
      //:IF TZADWWKO.ESG_LOD_Entity2 EXISTS    // Check to see if an Activate Entity was specifified.
      lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_LOD_Entity2" );
      if ( lTempInteger_0 == 0 )
      { 
         //:szAD_ListE = TZADWWKO.ESG_LOD_Entity2.Name
         GetVariableFromAttribute( szAD_ListE, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity2", "Name", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:szAD_ListE = TZADWWKO.ESG_LOD_Entity.Name
         GetVariableFromAttribute( szAD_ListE, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
      } 

      //:END
      //:szAD_MainV   = TZADWWKO.W_MetaDefUpdateView.Name 
      GetVariableFromAttribute( szAD_MainV, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateView", "Name", "", 0 );
      //:szAD_MGP     = TZADWWKOSub.W_MetaDefUpdateView.Name 
      GetVariableFromAttribute( szAD_MGP, 0, 'S', 51, TZADWWKOSub, "W_MetaDefUpdateView", "Name", "", 0 );
      //:szAD_MGPO    = TZADWWKOSub.W_MetaDefUpdateObject.Name 
      GetVariableFromAttribute( szAD_MGPO, 0, 'S', 51, TZADWWKOSub, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:szAD_MGPRoot = UpdateLOD.LOD_EntityParent.Name 
      GetVariableFromAttribute( szAD_MGPRoot, 0, 'S', 51, UpdateLOD, "LOD_EntityParent", "Name", "", 0 );
      //:ReturnID_NameForEntity( TZADWWKO, szAD_ID, szAD_ListE, UpdateLOD )  // Get Identifier Name for UpdateLOD LOD & Entity szAD_ListE
      oTZADWWKO_ReturnID_NameForEntity( TZADWWKO, szAD_ID, szAD_ListE, UpdateLOD );

      //:// Copy the source VML to the end of the Find VML, converting the variable characters in the process.
      //:nFileIn   = SysOpenFile( TZWINDOWL, "c:\lplr\AD_Base\ObjMGP.VML", COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOWL, "c:\\lplr\\AD_Base\\ObjMGP.VML", COREFILE_READ );
      //:nFileOut  = SysOpenFile( TZWINDOWL, szFindVML, COREFILE_APPEND )
      nFileOut = SysOpenFile( TZWINDOWL, szFindVML, COREFILE_APPEND );
      //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 ) 
      nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 
         //:zSearchAndReplace( szVML_Statement, 256, "_Area",     szAD_Area )    // MGP Area
         zSearchAndReplace( szVML_Statement, 256, "_Area", szAD_Area );
         //:zSearchAndReplace( szVML_Statement, 256, "_ADMainV",  szAD_MainV )   // Main Page View
         zSearchAndReplace( szVML_Statement, 256, "_ADMainV", szAD_MainV );
         //:zSearchAndReplace( szVML_Statement, 256, "_ADListE",  szAD_ListE )   // Entity Name from Main page listbox
         zSearchAndReplace( szVML_Statement, 256, "_ADListE", szAD_ListE );
         //:zSearchAndReplace( szVML_Statement, 256, "_MGPV",     szAD_MGP )     // Area Name for MGP Page Spec
         zSearchAndReplace( szVML_Statement, 256, "_MGPV", szAD_MGP );
         //:zSearchAndReplace( szVML_Statement, 256, "_MGPO",     szAD_MGPO )    // LOD Name for MGP Page Spec
         zSearchAndReplace( szVML_Statement, 256, "_MGPO", szAD_MGPO );
         //:zSearchAndReplace( szVML_Statement, 256, "_MGPRoot",  szAD_MGPRoot ) // Root Entity Name for the LOD above
         zSearchAndReplace( szVML_Statement, 256, "_MGPRoot", szAD_MGPRoot );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_ID",    szAD_ID )      // Update Identifier Name for object and entity above.
         zSearchAndReplace( szVML_Statement, 256, "_AD_ID", szAD_ID );

         //:SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement )
         SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement );
         //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      } 

      //:END

      //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
      SysCloseFile( TZWINDOWL, nFileIn, 0 );
      //:SysCloseFile( TZWINDOWL, nFileOut, 0 )
      SysCloseFile( TZWINDOWL, nFileOut, 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddAD_ListUpdActions( VIEW TZADWWKO   BASED ON LOD  TZADWWKO,
//:                      VIEW TZWINDOWL  BASED ON LOD  TZWDLGSO,
//:                      VIEW AD_Base    BASED ON LOD  TZWDLGSO,
//:                      VIEW UpdateLOD  BASED ON LOD  TZZOLODO,
//:                      STRING ( 50 ) szGroupType )

//:   VIEW TaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddAD_ListUpdActions( zVIEW     TZADWWKO,
                                zVIEW     TZWINDOWL,
                                zVIEW     AD_Base,
                                zVIEW     UpdateLOD,
                                zPCHAR    szGroupType )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szSourceOperationName
   zCHAR     szSourceOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTargetOperationName
   zCHAR     szTargetOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_ID  
   zCHAR     szAD_ID[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szEntityName
   zCHAR     szEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szEntityName2  
   zCHAR     szEntityName2[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 6 )   szCompareOption
   zCHAR     szCompareOption[ 7 ] = { 0 }; 
   //:STRING ( 256 ) szCompare
   zCHAR     szCompare[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceFileDirectory
   zCHAR     szSourceFileDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTargetFileDirectory
   zCHAR     szTargetFileDirectory[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength 
   zLONG     nLength = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_3[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Process Actions and Operations for List Detail and List Update Groups.
   //:// For szGroupType = ListGroupWDetailPage, simply modify the corresponding Action Name. for ListGroupWUpdatePage, add operations as necessary.

   //:// Process ListGroupWDetailPage first.
   //:IF szGroupType = "ListGroupWDetailPage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 )
   { 
      //:// Simply modify the Return Action name to add AD Area at end.
      //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = "GOTO_DTL_" 
      RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", "GOTO_DTL_", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:szAD_Area = TZADWWKO.EntitySubGroup.GroupAreaName
         GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
         //:TZWINDOWL.Action.Tag = "GOTO_DTL_" + szAD_Area
         ZeidonStringCopy( szTempString_0, 1, 0, "GOTO_DTL_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, szAD_Area, 1, 0, 33 );
         SetAttributeFromString( TZWINDOWL, "Action", "Tag", szTempString_0 );
         //:TZWINDOWL.Action.DialogName = TZWINDOWL.Dialog.Tag 
         SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOWL, "Dialog", "Tag" );
         //:TZWINDOWL.Action.WindowName = szAD_Area + "Detail"
         ZeidonStringCopy( szTempString_1, 1, 0, szAD_Area, 1, 0, 33 );
         ZeidonStringConcat( szTempString_1, 1, 0, "Detail", 1, 0, 33 );
         SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_1 );
      } 

      //:END
      //:RETURN 
      return( 0 );
   } 

   //:END

   //:// The rest is for ListGroupWUpdatePage Group.

   //:// Process Operations first because they will later be used by Actions.

   //:// If the List New/Update/Delete operations aren't already in the VML File for the TZWINDOWL SourceFile directory, copy the
   //:// source file operations from AD_Base SubObjUpdat file, converting the variable data in the process.
   //:// Note that we will be copying 5 operations, the New/Update/Delete operations for the Listbox and the OK/Cancel for the
   //:// related Update page.

   //:// Source Directory is from AD_Base
   //:szSourceFileDirectory = "C:\LPLR\AD_Base\SubObjUpdat.VML"
   ZeidonStringCopy( szSourceFileDirectory, 1, 0, "C:\\LPLR\\AD_Base\\SubObjUpdat.VML", 1, 0, 257 );

   //:// Target Directory is current LPLR.
   //:szTargetFileDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szTargetFileDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szTargetFileDirectory = szTargetFileDirectory + "\" + TZWINDOWL.SourceFile.Name + ".VML"
   ZeidonStringConcat( szTargetFileDirectory, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZWINDOWL, "SourceFile", "Name", "", 0 );
   ZeidonStringConcat( szTargetFileDirectory, 1, 0, szTempString_2, 1, 0, 257 );
   ZeidonStringConcat( szTargetFileDirectory, 1, 0, ".VML", 1, 0, 257 );

   //:// Check if GOTO_UPD_AD_Area operation is already in target VML file.
   //:szAD_Area = TZADWWKO.EntitySubGroup.GroupAreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:szTargetOperationName = "GOTO_UPD_" + szAD_Area
   ZeidonStringCopy( szTargetOperationName, 1, 0, "GOTO_UPD_", 1, 0, 51 );
   ZeidonStringConcat( szTargetOperationName, 1, 0, szAD_Area, 1, 0, 51 );
   //:nLength = zGetStringLen( szTargetOperationName )
   nLength = zGetStringLen( szTargetOperationName );
   //:nFileIn = SysOpenFile( TZWINDOWL, szTargetFileDirectory, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOWL, szTargetFileDirectory, COREFILE_READ );
   //:nRC2    = -1
   nRC2 = -1;
   //:IF nFileIn > 0 
   if ( nFileIn > 0 )
   { 
      //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 AND nRC2 != 0
      while ( nRC == 1 && nRC2 != 0 )
      { 
         //:strncpy_s( szCompare, szVML_Statement, nLength )
         strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
         //:nRC2 = zstrcmp( szCompare, szTargetOperationName )
         nRC2 = zstrcmp( szCompare, szTargetOperationName );
         //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
      SysCloseFile( TZWINDOWL, nFileIn, 0 );
      //:ELSE 
   } 
   else
   { 
      //:IssueError( TZADWWKO,0,0, "Target VML doesn't exist yet." )
      IssueError( TZADWWKO, 0, 0, "Target VML doesn't exist yet." );
      //:RETURN   // Don't continue if the VML hasn't been created yet
      return( 0 );
   } 

   //:END

   //:IF nRC2 != 0     // Operation was not found.
   if ( nRC2 != 0 )
   { 

      //:// The GOTO_UPD_AD_Area Operation was not found in the target Source File, so copy source VML to end of target VML,
      //:// converting the variable characters in the process.
      //:szEntityName = TZADWWKO.ESG_LOD_Entity.Name 
      GetVariableFromAttribute( szEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
      //:szAD_UpdObj  = TZADWWKO.W_MetaDefUpdateObject.Name
      GetVariableFromAttribute( szAD_UpdObj, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:IF TZADWWKO.ESG_LOD_Entity2 EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_LOD_Entity2" );
      if ( lTempInteger_0 == 0 )
      { 
         //:szEntityName2 = TZADWWKO.ESG_LOD_Entity2.Name
         GetVariableFromAttribute( szEntityName2, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity2", "Name", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:szEntityName2 = ""
         ZeidonStringCopy( szEntityName2, 1, 0, "", 1, 0, 51 );
      } 

      //:END
      //:ReturnID_NameForEntity( TZADWWKO, szAD_ID, szEntityName, UpdateLOD )  // Get Identifier Name for UpdateLOD LOD & Entity szEntityName
      oTZADWWKO_ReturnID_NameForEntity( TZADWWKO, szAD_ID, szEntityName, UpdateLOD );
      //:nFileIn   = SysOpenFile( TZWINDOWL, szSourceFileDirectory, COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOWL, szSourceFileDirectory, COREFILE_READ );
      //:nFileOut  = SysOpenFile( TZWINDOWL, szTargetFileDirectory, COREFILE_APPEND )
      nFileOut = SysOpenFile( TZWINDOWL, szTargetFileDirectory, COREFILE_APPEND );
      //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 
         //:// Loop through source vml and convert variable data before copying to target.
         //:// There could be optional statements in the Delete operation to check for existence of the ESG_LOD_Entity2.
         //:// If so we need to check for them here and either include them or skip them.
         //:strncpy_s( szCompareOption, szVML_Statement, 6 )
         strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
         //:nRC2 = zstrcmp( szCompareOption, "Option" )
         nRC2 = zstrcmp( szCompareOption, "Option" );
         //:IF nRC2 = 0
         if ( nRC2 == 0 )
         { 
            //:// Start of optional statements, so loop through them and either copy or skip.
            //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )  // Skip the Option Start statement.
            nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
            //:szCompareOption = ""
            ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
            //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
            while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
            { 
               //:strncpy_s( szCompareOption, szVML_Statement, 6 )
               strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
               //:IF szEntityName2 != ""
               if ( ZeidonStringCompare( szEntityName2, 1, 0, "", 1, 0, 51 ) != 0 )
               { 
                  //:// Convert the line and write it out.
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO",       szAD_UpdObj )    // Update Object Name
                  zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdObj );
                  //:zSearchAndReplace( szVML_Statement, 256, "_EntityNameChk", szEntityName2 )  // EntityChk Name
                  zSearchAndReplace( szVML_Statement, 256, "_EntityNameChk", szEntityName2 );
                  //:zSearchAndReplace( szVML_Statement, 256, "_EntityName",    szEntityName )   // Entity Name
                  zSearchAndReplace( szVML_Statement, 256, "_EntityName", szEntityName );
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_ID",         szAD_ID )        // Update Identifier Name for object and entity above.
                  zSearchAndReplace( szVML_Statement, 256, "_AD_ID", szAD_ID );
                  //:SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement ) 
                  SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement );
               } 

               //:END
               //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
               nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
               //:strncpy_s( szCompareOption, szVML_Statement, 6 )
               strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
            } 

            //:END
            //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )  // Skip the Option End statement.
            nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
         } 

         //:END
         //:// Process regular line.
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_Area",    szAD_Area )     // Area Name
         zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area );
         //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO",    szAD_UpdObj )   // Update Object Name
         zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdObj );
         //:zSearchAndReplace( szVML_Statement, 256, "_EntityName", szEntityName )  // Entity Name
         zSearchAndReplace( szVML_Statement, 256, "_EntityName", szEntityName );

         //:SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement )
         SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement );

         //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
      } 

      //:END

      //:SysCloseFile( TZWINDOWL, nFileIn, 0 )
      SysCloseFile( TZWINDOWL, nFileIn, 0 );
      //:SysCloseFile( TZWINDOWL, nFileOut, 0 )
      SysCloseFile( TZWINDOWL, nFileOut, 0 );

      //:// Also make sure the Operation entries exist for the target SourceFile subobject.
      //:SET CURSOR FIRST TZWINDOWL.Operation WHERE TZWINDOWL.Operation.Name = szTargetOperationName
      RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Operation", "Name", szTargetOperationName, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:// The Operations don't exist in target so copy them.
         //:SET CURSOR LAST TZWINDOWL.Operation  
         RESULT = SetCursorLastEntity( TZWINDOWL, "Operation", "" );
         //:SET CURSOR FIRST AD_Base.SourceFile WHERE AD_Base.SourceFile.Name = "SubObjUpdat"
         RESULT = SetCursorFirstEntityByString( AD_Base, "SourceFile", "Name", "SubObjUpdat", "" );
         //:FOR EACH AD_Base.Operation 
         RESULT = SetCursorFirstEntity( AD_Base, "Operation", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:szOperationName = AD_Base.Operation.Name    // We need to convert the Operation Name.
            GetVariableFromAttribute( szOperationName, 0, 'S', 51, AD_Base, "Operation", "Name", "", 0 );
            //:zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area )   
            zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area );
            //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Operation", zPOS_AFTER );
            //:TZWINDOWL.Operation.Name = szOperationName
            SetAttributeFromString( TZWINDOWL, "Operation", "Name", szOperationName );
            //:SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOWL, "Operation", AD_Base, "Operation", zSET_NULL );
            //:FOR EACH AD_Base.Parameter 
            RESULT = SetCursorFirstEntity( AD_Base, "Parameter", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER )
               CreateMetaEntity( TZWINDOWL, TZWINDOWL, "Parameter", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL )
               SetMatchingAttributesByName( TZWINDOWL, "Parameter", AD_Base, "Parameter", zSET_NULL );
               RESULT = SetCursorNextEntity( AD_Base, "Parameter", "" );
            } 

            RESULT = SetCursorNextEntity( AD_Base, "Operation", "" );
            //:END 
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:// Process each of the 3 Actions and corresponding Operation as necessary.
   //:// The Operations were created above.
   //:FOR EACH TZWINDOWL.Action 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Action", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = TZWINDOWL.Action.Tag 
      GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL, "Action", "Tag", "", 0 );
      //:IF szActionName = "GOTO_UPD__AD_Area" OR 
      //:   szActionName = "GOTO_NewUPD__AD_Area"
      if ( ZeidonStringCompare( szActionName, 1, 0, "GOTO_UPD__AD_Area", 1, 0, 51 ) == 0 || ZeidonStringCompare( szActionName, 1, 0, "GOTO_NewUPD__AD_Area", 1, 0, 51 ) == 0 )
      { 

         //:TZWINDOWL.Action.DialogName = TZWINDOWL.Dialog.Tag 
         SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOWL, "Dialog", "Tag" );
         //:TZWINDOWL.Action.WindowName = szAD_Area + "Detail"
         ZeidonStringCopy( szTempString_3, 1, 0, szAD_Area, 1, 0, 33 );
         ZeidonStringConcat( szTempString_3, 1, 0, "Detail", 1, 0, 33 );
         SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_3 );
      } 

      //:END
      //:IF szActionName = "GOTO_UPD__AD_Area" OR 
      //:   szActionName = "GOTO_NewUPD__AD_Area" OR 
      //:   szActionName = "DELETE_UPD__AD_Area"
      if ( ZeidonStringCompare( szActionName, 1, 0, "GOTO_UPD__AD_Area", 1, 0, 51 ) == 0 || ZeidonStringCompare( szActionName, 1, 0, "GOTO_NewUPD__AD_Area", 1, 0, 51 ) == 0 ||
           ZeidonStringCompare( szActionName, 1, 0, "DELETE_UPD__AD_Area", 1, 0, 51 ) == 0 )
      { 

         //:zSearchAndReplace( szActionName, 256, "_AD_Area",  szAD_Area )
         zSearchAndReplace( szActionName, 256, "_AD_Area", szAD_Area );
         //:TZWINDOWL.Action.Tag = szActionName
         SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
         //:SET CURSOR FIRST TZWINDOWL.Operation WHERE TZWINDOWL.Operation.Name = szActionName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Operation", "Name", szActionName, "" );
         //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOWL.Operation 
         RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOWL, "Operation", zPOS_AFTER );
      } 

      RESULT = SetCursorNextEntity( TZWINDOWL, "Action", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddFindRangeInitValues( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                        VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                        INTEGER nFileIn,
//:                        INTEGER nFileOut )

//:   INTEGER        nRC
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddFindRangeInitValues( zVIEW     TZADWWKO,
                                  zVIEW     TZWINDOW,
                                  zLONG     nFileIn,
                                  zLONG     nFileOut )
{
   zLONG     nRC = 0; 
   //:INTEGER        nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER        nLength
   zLONG     nLength = 0; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szCompare
   zCHAR     szCompare[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompareInit
   zCHAR     szCompareInit[ 51 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 


   //:// First skip any existing statements, since they need to be replaced.
   //:// We will skip statements until we get to the "   // End Initialization" statement.
   //:szCompareInit = "   // End Initialization"
   ZeidonStringCopy( szCompareInit, 1, 0, "   // End Initialization", 1, 0, 51 );
   //:nLength       = zGetStringLen( szCompareInit )
   nLength = zGetStringLen( szCompareInit );
   //:nRC2 = -1
   nRC2 = -1;
   //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
   nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
   //:LOOP WHILE nRC = 1 AND nRC2 != 0
   while ( nRC == 1 && nRC2 != 0 )
   { 
      //:strncpy_s( szCompare, szVML_Statement, nLength )    // Copy the necsssary characters of the line to the compare field.
      strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
      //:nRC2 = zstrcmp( szCompare, szCompareInit )
      nRC2 = zstrcmp( szCompare, szCompareInit );
      //:IF nRC2 != 0
      if ( nRC2 != 0 )
      { 
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END
   } 

   //:END

   //:// Generate the VML wXferO.FindRangeSetAttributes entries for the specified Range entries.

   //:// Don't generate anything except End statement if no Range entries were specified.
   //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute WHERE TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType != ""
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      } 

   } 

   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:SysWriteLine( TZWINDOW, nFileOut, "   // End Initialization" )
      SysWriteLine( TZWINDOW, nFileOut, "   // End Initialization" );
      //:RETURN
      return( 0 );
   } 

   //:END

   //:// Generate Delete Statements for work entries and comment Statements.
   //:SysWriteLine( TZWINDOW, nFileOut, "   // Delete any current entries." )
   SysWriteLine( TZWINDOW, nFileOut, "   // Delete any current entries." );
   //:SysWriteLine( TZWINDOW, nFileOut, "   FOR EACH wXferO.FindRangeSetAttributes " )
   SysWriteLine( TZWINDOW, nFileOut, "   FOR EACH wXferO.FindRangeSetAttributes " );
   //:SysWriteLine( TZWINDOW, nFileOut, "      DELETE ENTITY wXferO.FindRangeSetAttributes NONE  " )
   SysWriteLine( TZWINDOW, nFileOut, "      DELETE ENTITY wXferO.FindRangeSetAttributes NONE  " );
   //:SysWriteLine( TZWINDOW, nFileOut, "   END" )
   SysWriteLine( TZWINDOW, nFileOut, "   END" );

   //:// Create work entries, RangeTempTargetEntity, from FlatSelectedSearchAttribute entries.
   //:FOR EACH TZADWWKO.RangeTempTargetEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "RangeTempTargetEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.RangeTempTargetEntity NONE
      RESULT = DeleteEntity( TZADWWKO, "RangeTempTargetEntity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "RangeTempTargetEntity", "" );
   } 

   //:END
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute   
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType != ""
      if ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "" ) != 0 )
      { 
         //:SET CURSOR FIRST TZADWWKO.RangeTempTargetEntity 
         //:           WHERE TZADWWKO.RangeTempTargetEntity.SearchEntityName    = TZADWWKO.FlatSelectedSearchAttribute.EntityName 
         //:             AND TZADWWKO.RangeTempTargetEntity.SearchAttributeName = TZADWWKO.FlatSelectedSearchAttribute.RangeTargetAttributeName 
         RESULT = SetCursorFirstEntity( TZADWWKO, "RangeTempTargetEntity", "" );
         if ( RESULT > zCURSOR_UNCHANGED )
         { 
            while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( TZADWWKO, "RangeTempTargetEntity", "SearchEntityName", TZADWWKO, "FlatSelectedSearchAttribute", "EntityName" ) != 0 ||
                    CompareAttributeToAttribute( TZADWWKO, "RangeTempTargetEntity", "SearchAttributeName", TZADWWKO, "FlatSelectedSearchAttribute", "RangeTargetAttributeName" ) != 0 ) )
            { 
               RESULT = SetCursorNextEntity( TZADWWKO, "RangeTempTargetEntity", "" );
            } 

         } 

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CREATE ENTITY TZADWWKO.RangeTempTargetEntity 
            RESULT = CreateEntity( TZADWWKO, "RangeTempTargetEntity", zPOS_AFTER );
            //:TZADWWKO.RangeTempTargetEntity.SearchEntityName    = TZADWWKO.FlatSelectedSearchAttribute.EntityName 
            SetAttributeFromAttribute( TZADWWKO, "RangeTempTargetEntity", "SearchEntityName", TZADWWKO, "FlatSelectedSearchAttribute", "EntityName" );
            //:TZADWWKO.RangeTempTargetEntity.SearchAttributeName = TZADWWKO.FlatSelectedSearchAttribute.RangeTargetAttributeName 
            SetAttributeFromAttribute( TZADWWKO, "RangeTempTargetEntity", "SearchAttributeName", TZADWWKO, "FlatSelectedSearchAttribute", "RangeTargetAttributeName" );
         } 

         //:END
         //:IF TZADWWKO.FlatSelectedSearchAttribute.RangeFromToType = "From"
         if ( CompareAttributeToString( TZADWWKO, "FlatSelectedSearchAttribute", "RangeFromToType", "From" ) == 0 )
         { 
            //:TZADWWKO.RangeTempTargetEntity.FromValueAttributeName = TZADWWKO.FlatSelectedSearchAttribute.AttributeName  
            SetAttributeFromAttribute( TZADWWKO, "RangeTempTargetEntity", "FromValueAttributeName", TZADWWKO, "FlatSelectedSearchAttribute", "AttributeName" );
            //:ELSE
         } 
         else
         { 
            //:TZADWWKO.RangeTempTargetEntity.ToValueAttributeName = TZADWWKO.FlatSelectedSearchAttribute.AttributeName 
            SetAttributeFromAttribute( TZADWWKO, "RangeTempTargetEntity", "ToValueAttributeName", TZADWWKO, "FlatSelectedSearchAttribute", "AttributeName" );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
      //:END
   } 

   //:END

   //:// Generate the Range setting entries into the current position of the output file
   //:FOR EACH TZADWWKO.RangeTempTargetEntity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "RangeTempTargetEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szVML_Statement = "   CREATE ENTITY wXferO.FindRangeSetAttributes"
      ZeidonStringCopy( szVML_Statement, 1, 0, "   CREATE ENTITY wXferO.FindRangeSetAttributes", 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      //:szVML_Statement = "   wXferO.FindRangeSetAttributes.SearchEntityName       = " + QUOTES + TZADWWKO.RangeTempTargetEntity.SearchEntityName + QUOTES
      ZeidonStringCopy( szVML_Statement, 1, 0, "   wXferO.FindRangeSetAttributes.SearchEntityName       = ", 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZADWWKO, "RangeTempTargetEntity", "SearchEntityName", "", 0 );
      ZeidonStringConcat( szVML_Statement, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      //:szVML_Statement = "   wXferO.FindRangeSetAttributes.SearchAttributeName    = " + QUOTES + TZADWWKO.RangeTempTargetEntity.SearchAttributeName + QUOTES
      ZeidonStringCopy( szVML_Statement, 1, 0, "   wXferO.FindRangeSetAttributes.SearchAttributeName    = ", 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZADWWKO, "RangeTempTargetEntity", "SearchAttributeName", "", 0 );
      ZeidonStringConcat( szVML_Statement, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      //:szVML_Statement = "   wXferO.FindRangeSetAttributes.FromValueAttributeName = " + QUOTES + TZADWWKO.RangeTempTargetEntity.FromValueAttributeName + QUOTES
      ZeidonStringCopy( szVML_Statement, 1, 0, "   wXferO.FindRangeSetAttributes.FromValueAttributeName = ", 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZADWWKO, "RangeTempTargetEntity", "FromValueAttributeName", "", 0 );
      ZeidonStringConcat( szVML_Statement, 1, 0, szTempString_2, 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      //:szVML_Statement = "   wXferO.FindRangeSetAttributes.ToValueAttributeName   = " + QUOTES + TZADWWKO.RangeTempTargetEntity.ToValueAttributeName + QUOTES
      ZeidonStringCopy( szVML_Statement, 1, 0, "   wXferO.FindRangeSetAttributes.ToValueAttributeName   = ", 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, TZADWWKO, "RangeTempTargetEntity", "ToValueAttributeName", "", 0 );
      ZeidonStringConcat( szVML_Statement, 1, 0, szTempString_3, 1, 0, 257 );
      ZeidonStringConcat( szVML_Statement, 1, 0, QUOTES, 1, 0, 257 );
      //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
      SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
      RESULT = SetCursorNextEntity( TZADWWKO, "RangeTempTargetEntity", "" );
   } 

   //:END

   //:// Conclude with the End Initialization statement.
   //:szVML_Statement = "   // End Initialization" 
   ZeidonStringCopy( szVML_Statement, 1, 0, "   // End Initialization", 1, 0, 257 );
   //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
   SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddListMapping( VIEW TZADWWKO   BASED ON LOD TZADWWKO,
//:                VIEW TZCONTROL  BASED ON LOD TZWDLGSO,
//:                VIEW TZWINDOWL  BASED ON LOD TZWDLGSO,
//:                VIEW MappingLOD BASED ON LOD TZZOLODO,
//:                STRING ( 50 ) szGridMappingEntityName,
//:                STRING ( 50 ) szGridMappingViewName,
//:                STRING ( 10 ) szMappingPrefix )

//:   VIEW TZPESRCO  REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddListMapping( zVIEW     TZADWWKO,
                          zVIEW     TZCONTROL,
                          zVIEW     TZWINDOWL,
                          zVIEW     MappingLOD,
                          zPCHAR    szGridMappingEntityName,
                          zPCHAR    szGridMappingViewName,
                          zPCHAR    szMappingPrefix )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:VIEW vCtrl1Pos BASED ON LOD  TZWDLGSO
   zVIEW     vCtrl1Pos = 0; 
   //:STRING ( 32 ) szESG_ListLOD_Attribute
   zCHAR     szESG_ListLOD_Attribute[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szESG_ListER_Attribute
   zCHAR     szESG_ListER_Attribute[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szESG_ListLOD_Entity
   zCHAR     szESG_ListLOD_Entity[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szER_AttributeName
   zCHAR     szER_AttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szLOD_EntityName
   zCHAR     szLOD_EntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szLOD_Name
   zCHAR     szLOD_Name[ 33 ] = { 0 }; 
   //:STRING ( 50 ) szPrompt
   zCHAR     szPrompt[ 51 ] = { 0 }; 
   //:INTEGER nControlCount
   zLONG     nControlCount = 0; 
   //:INTEGER nPosition
   zLONG     nPosition = 0; 
   //:INTEGER nTotalLengths
   zLONG     nTotalLengths = 0; 
   //:INTEGER nGridLength
   zLONG     nGridLength = 0; 
   //:INTEGER nControlDiff
   zLONG     nControlDiff = 0; 
   //:INTEGER nTotalControls
   zLONG     nTotalControls = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zSHORT    lTempInteger_8; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZADWWKO, zLEVEL_TASK );

   //:// The prefix (szMappingPrefix) passed allows this operation to handle any subobject with the ESG_ListLOD_Attribute structure.
   //:// In the code below the regular VML statement referencing the ESG_ListLOD_Attribute structure names is commented out and 
   //:// followed by a statement using a name created from the prefix, which are:
   //://    szESG_ListLOD_Attribute
   //://    szESG_ListER_Attribute
   //://    szESG_ListLOD_Entity

   //:NAME VIEW MappingLOD "MappingLOD"
   SetNameForView( MappingLOD, "MappingLOD", 0, zLEVEL_TASK );

   //:// Build names from szMappingPrefix.
   //:szESG_ListLOD_Attribute = szMappingPrefix + "LOD_Attribute"
   ZeidonStringCopy( szESG_ListLOD_Attribute, 1, 0, szMappingPrefix, 1, 0, 33 );
   ZeidonStringConcat( szESG_ListLOD_Attribute, 1, 0, "LOD_Attribute", 1, 0, 33 );
   //:szESG_ListER_Attribute  = szMappingPrefix + "ER_Attribute"
   ZeidonStringCopy( szESG_ListER_Attribute, 1, 0, szMappingPrefix, 1, 0, 33 );
   ZeidonStringConcat( szESG_ListER_Attribute, 1, 0, "ER_Attribute", 1, 0, 33 );
   //:szESG_ListLOD_Entity    = szMappingPrefix + "LOD_Entity"
   ZeidonStringCopy( szESG_ListLOD_Entity, 1, 0, szMappingPrefix, 1, 0, 33 );
   ZeidonStringConcat( szESG_ListLOD_Entity, 1, 0, "LOD_Entity", 1, 0, 33 );

   //:// Process CrtlCrtl entries recursively to find Grid Control. Then adding Grid mapping.
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:FOR EACH TZCONTROL.Control
   RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZCONTROL.ControlDef.Tag = "Grid"
      if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "Grid" ) == 0 )
      { 

         //:// We are positioned on the generated Grid Control.
         //:// We first need to change the Grid mapping to the object and entity being processed.
         //:GetStringFromAttribute( szLOD_Name, MappingLOD, "LOD", "Name" )
         GetStringFromAttribute( szLOD_Name, zsizeof( szLOD_Name ), MappingLOD, "LOD", "Name" );
         //:SET CURSOR FIRST TZWINDOWL.ViewObjRef WHERE TZWINDOWL.ViewObjRef.Name = szGridMappingViewName 
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ViewObjRef", "Name", szGridMappingViewName, "" );
         //:IF TZCONTROL.CtrlMap EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlMap" );
         if ( lTempInteger_0 == 0 )
         { 
            //:IF TZCONTROL.CtrlMapView EXISTS
            lTempInteger_1 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapView" );
            if ( lTempInteger_1 == 0 )
            { 
               //:EXCLUDE TZCONTROL.CtrlMapView 
               RESULT = ExcludeEntity( TZCONTROL, "CtrlMapView", zREPOS_AFTER );
               //:EXCLUDE TZCONTROL.CtrlMapLOD_Entity
               RESULT = ExcludeEntity( TZCONTROL, "CtrlMapLOD_Entity", zREPOS_AFTER );
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
            CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
         } 

         //:END
         //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOWL.ViewObjRef
         RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
         //:SetCursorFirstEntityByString( MappingLOD, "LOD_Entity", "Name", szGridMappingEntityName, "" )  // Position of List Entity
         SetCursorFirstEntityByString( MappingLOD, "LOD_Entity", "Name", szGridMappingEntityName, "" );
         //:IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Entity", MappingLOD, "LOD_Entity", zPOS_AFTER ) 
         IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Entity", MappingLOD, "LOD_Entity", zPOS_AFTER );
         //:nGridLength = TZCONTROL.Control.SZDLG_X 
         GetIntegerFromAttribute( &nGridLength, TZCONTROL, "Control", "SZDLG_X" );

         //:// Next, we now need to replace the first Grid item with those selected in ESG_ListLOD_Attribute
         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" ) 
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:CreateViewFromView( vCtrl1Pos, TZCONTROL )   // vCtrl1Pos is positioned on first entry in Grid.
         CreateViewFromView( &vCtrl1Pos, TZCONTROL );
         //:NAME VIEW vCtrl1Pos "vCtrl1Pos"
         SetNameForView( vCtrl1Pos, "vCtrl1Pos", 0, zLEVEL_TASK );
         //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = vCtrl1Pos.ControlDef.Tag 
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vCtrl1Pos, "ControlDef", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szTempString_0, "" );
         //:nControlCount = 0
         nControlCount = 0;
         //:nRC = SetCursorFirstEntity( TZADWWKO, szESG_ListLOD_Attribute, "" )
         nRC = SetCursorFirstEntity( TZADWWKO, szESG_ListLOD_Attribute, "" );
         //:LOOP WHILE nRC >= zCURSOR_SET 
         while ( nRC >= zCURSOR_SET )
         { 
            //:nControlCount = nControlCount + 1
            nControlCount = nControlCount + 1;
            //:IF nControlCount = 1
            if ( nControlCount == 1 )
            { 
               //:// For first entry, remove current mapping if it exists or create CtrlMap.
               //:IF TZCONTROL.CtrlMap EXISTS
               lTempInteger_2 = CheckExistenceOfEntity( TZCONTROL, "CtrlMap" );
               if ( lTempInteger_2 == 0 )
               { 
                  //:IF TZCONTROL.CtrlMapView EXISTS
                  lTempInteger_3 = CheckExistenceOfEntity( TZCONTROL, "CtrlMapView" );
                  if ( lTempInteger_3 == 0 )
                  { 
                     //:EXCLUDE TZCONTROL.CtrlMapView 
                     RESULT = ExcludeEntity( TZCONTROL, "CtrlMapView", zREPOS_AFTER );
                     //:EXCLUDE TZCONTROL.CtrlMapLOD_Attribute
                     RESULT = ExcludeEntity( TZCONTROL, "CtrlMapLOD_Attribute", zREPOS_AFTER );
                  } 

                  //:END
                  //:ELSE
               } 
               else
               { 
                  //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
                  CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
               } 

               //:END
               //:ELSE
            } 
            else
            { 
               //:// After first entry we are creating Control..
               //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER )
               CreateMetaEntity( TZWINDOWL, TZCONTROL, "Control", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZCONTROL, "Control", vCtrl1Pos, "Control", zSET_NULL )
               SetMatchingAttributesByName( TZCONTROL, "Control", vCtrl1Pos, "Control", zSET_NULL );
               //:CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER )
               CreateMetaEntity( TZWINDOWL, TZCONTROL, "CtrlMap", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZCONTROL, "CtrlMap", vCtrl1Pos, "CtrlMap", zSET_NULL )
               SetMatchingAttributesByName( TZCONTROL, "CtrlMap", vCtrl1Pos, "CtrlMap", zSET_NULL );
               //:INCLUDE TZCONTROL.ControlDef FROM vCtrl1Pos.ControlDef
               RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", vCtrl1Pos, "ControlDef", zPOS_AFTER );
            } 

            //:   
            //:END       

            //:// Position on list ctrl mapping LOD Entity and Attribute for CtrlMap include.
            //:GetStringFromAttribute( szER_AttributeName, TZADWWKO, szESG_ListER_Attribute, "Name" )
            GetStringFromAttribute( szER_AttributeName, zsizeof( szER_AttributeName ), TZADWWKO, szESG_ListER_Attribute, "Name" );
            //:GetStringFromAttribute( szLOD_EntityName,   TZADWWKO, szESG_ListLOD_Entity, "Name" )
            GetStringFromAttribute( szLOD_EntityName, zsizeof( szLOD_EntityName ), TZADWWKO, szESG_ListLOD_Entity, "Name" );
            //:TZCONTROL.Control.Tag  = "GridCtl" + szER_AttributeName 
            ZeidonStringCopy( szTempString_0, 1, 0, "GridCtl", 1, 0, 33 );
            ZeidonStringConcat( szTempString_0, 1, 0, szER_AttributeName, 1, 0, 33 );
            SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );

            //:GetStringFromAttribute( szPrompt, TZADWWKO, szESG_ListLOD_Attribute, "PromptValue" )
            GetStringFromAttribute( szPrompt, zsizeof( szPrompt ), TZADWWKO, szESG_ListLOD_Attribute, "PromptValue" );
            //:TZCONTROL.Control.Text = szPrompt 
            SetAttributeFromString( TZCONTROL, "Control", "Text", szPrompt );

            //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOWL.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
            //:SetCursorFirstEntityByString( MappingLOD, "LOD_Entity", "Name", szLOD_EntityName, "" )  // Position of List Entity
            SetCursorFirstEntityByString( MappingLOD, "LOD_Entity", "Name", szLOD_EntityName, "" );
            //:SetCursorFirstEntityByString( MappingLOD, "ER_Attribute", "Name", szER_AttributeName, "LOD_Entity" )  // Position of List Entity
            SetCursorFirstEntityByString( MappingLOD, "ER_Attribute", "Name", szER_AttributeName, "LOD_Entity" );
            //:IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", MappingLOD, "LOD_Attribute", zPOS_AFTER )
            IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", MappingLOD, "LOD_Attribute", zPOS_AFTER );

            //:nRC = SetCursorNextEntity( TZADWWKO, szESG_ListLOD_Attribute, "" )
            nRC = SetCursorNextEntity( TZADWWKO, szESG_ListLOD_Attribute, "" );
         } 

         //:END
         //:DropView( vCtrl1Pos )
         DropView( vCtrl1Pos );

         //:// Determine the total of Control lengths in case we need to shrink them to make them fit on the Grid.
         //:nTotalLengths  = 0
         nTotalLengths = 0;
         //:nTotalControls = 0
         nTotalControls = 0;
         //:FOR EACH TZCONTROL.Control 
         RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:nTotalLengths  = nTotalLengths + TZCONTROL.Control.SZDLG_X 
            GetIntegerFromAttribute( &lTempInteger_4, TZCONTROL, "Control", "SZDLG_X" );
            nTotalLengths = nTotalLengths + lTempInteger_4;
            //:IF TZCONTROL.ControlDef.Tag = "GridEditCtl"
            if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "GridEditCtl" ) == 0 )
            { 
               //:nTotalControls = nTotalControls + 1
               nTotalControls = nTotalControls + 1;
            } 

            RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
            //:END
         } 

         //:END 
         //:IF nGridLength < nTotalLengths      // nGridLength was set earlier in this operation.
         if ( nGridLength < nTotalLengths )
         { 
            //:// We need to subtrack the difference from the length of each Control
            //:nControlDiff = ((nTotalLengths - nGridLength) / nTotalControls ) + 1    // Round up
            nControlDiff = ( ( nTotalLengths - nGridLength ) / nTotalControls ) + 1;
            //:FOR EACH TZCONTROL.Control 
            RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:IF TZCONTROL.ControlDef.Tag = "GridEditCtl"
               if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "GridEditCtl" ) == 0 )
               { 
                  //:TZCONTROL.Control.SZDLG_X = TZCONTROL.Control.SZDLG_X - nControlDiff
                  GetIntegerFromAttribute( &lTempInteger_5, TZCONTROL, "Control", "SZDLG_X" );
                  lTempInteger_6 = lTempInteger_5 - nControlDiff;
                  SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", lTempInteger_6 );
               } 

               RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
               //:END
            } 

            //:END
         } 

         //:END

         //:// Compute the X position for each entry in the Grid.
         //:nPosition = 0
         nPosition = 0;
         //:FOR EACH TZCONTROL.Control 
         RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:TZCONTROL.Control.PSDLG_X = nPosition
            SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", nPosition );
            //:nPosition = nPosition + TZCONTROL.Control.SZDLG_X 
            GetIntegerFromAttribute( &lTempInteger_7, TZCONTROL, "Control", "SZDLG_X" );
            nPosition = nPosition + lTempInteger_7;
            RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
         } 

         //:END    
         //:ELSE
      } 
      else
      { 
         //:IF TZCONTROL.CtrlCtrl EXISTS
         lTempInteger_8 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
         if ( lTempInteger_8 == 0 )
         { 
            //:AddListMapping( TZADWWKO, TZCONTROL, 
            //:                TZWINDOWL, 
            //:                MappingLOD, 
            //:                szGridMappingEntityName, 
            //:                szGridMappingViewName, 
            //:                szMappingPrefix )
            oTZADWWKO_AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, MappingLOD, szGridMappingEntityName, szGridMappingViewName, szMappingPrefix );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
      //:END  
   } 

   //:END
   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AddRegisteredViewName( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                       VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                       VIEW TZZOLODO BASED ON LOD TZZOLODO,
//:                       STRING ( 32 ) szObjectViewName )

//:   VIEW TZWDVORO BASED ON LOD TZWDVORO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AddRegisteredViewName( zVIEW     TZADWWKO,
                                 zVIEW     TZWINDOW,
                                 zVIEW     TZZOLODO,
                                 zPCHAR    szObjectViewName )
{
   zVIEW     TZWDVORO = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    RESULT; 


   //:// Activate the Registered View and include it in the Dialog, creating it if necessary.
   //:nRC = ActivateMetaOI_ByName( TZADWWKO, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szObjectViewName, 0 )
   nRC = ActivateMetaOI_ByName( TZADWWKO, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szObjectViewName, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:// Create the ViewObjRef.
      //:ActivateEmptyMetaOI( TZWINDOW, TZWDVORO, zSOURCE_VOR_META, zSINGLE )
      ActivateEmptyMetaOI( TZWINDOW, &TZWDVORO, zSOURCE_VOR_META, zSINGLE );
      //:NAME VIEW TZWDVORO "TZWDVORO_ToInclude"
      SetNameForView( TZWDVORO, "TZWDVORO_ToInclude", 0, zLEVEL_TASK );
      //:CreateMetaEntity( TZWINDOW, TZWDVORO, "ViewObjRef", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWDVORO, "ViewObjRef", zPOS_AFTER );
      //:TZWDVORO.ViewObjRef.Name = szObjectViewName
      SetAttributeFromString( TZWDVORO, "ViewObjRef", "Name", szObjectViewName );
      //:TZWDVORO.ViewObjRef.Desc = "Created during AD Build"
      SetAttributeFromString( TZWDVORO, "ViewObjRef", "Desc", "Created during AD Build" );
      //:INCLUDE TZWDVORO.LOD FROM TZZOLODO.LOD 
      RESULT = IncludeSubobjectFromSubobject( TZWDVORO, "LOD", TZZOLODO, "LOD", zPOS_AFTER );
      //:CommitMetaOI( TZWINDOW, TZWDVORO, zSOURCE_VOR_META )
      CommitMetaOI( TZWINDOW, TZWDVORO, zSOURCE_VOR_META );
      //:ELSE
   } 
   else
   { 
      //:NAME VIEW TZWDVORO "TZWDVORO_ToInclude"
      SetNameForView( TZWDVORO, "TZWDVORO_ToInclude", 0, zLEVEL_TASK );
   } 

   //:END
   //:INCLUDE TZWINDOW.ViewObjRef FROM TZWDVORO.ViewObjRef 
   RESULT = IncludeSubobjectFromSubobject( TZWINDOW, "ViewObjRef", TZWDVORO, "ViewObjRef", zPOS_AFTER );
   //:DropObjectInstance( TZWDVORO )
   DropObjectInstance( TZWDVORO );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:AutodesignFindPages( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                     VIEW ViewToWindow,
//:                     STRING ( 50 ) szFindType )

//:   VIEW TZADWWKOT   REGISTERED AS TZADWWKO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_AutodesignFindPages( zVIEW     TZADWWKO,
                               zVIEW     ViewToWindow,
                               zPCHAR    szFindType )
{
   zVIEW     TZADWWKOT = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL   REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:VIEW TZWINDOW    BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZCONTROL   BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL = 0; 
   //:VIEW TZCONTROL2  BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL2 = 0; 
   //:VIEW TZCtlHier   BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlHier = 0; 
   //:VIEW TZCtlTemp   BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlTemp = 0; 
   //:VIEW TaskLPLR    REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   //:VIEW CRMBase     BASED ON LOD  TZWDLGSO
   zVIEW     CRMBase = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW ReturnedLOD BASED ON LOD  TZZOLODO
   zVIEW     ReturnedLOD = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW lConList    BASED ON LOD  TZZOLODO
   zVIEW     lConList = 0; 
   //:VIEW mConList    BASED ON LOD  TZZOLODO
   zVIEW     mConList = 0; 
   //:VIEW mUser       BASED ON LOD  TZZOLODO
   zVIEW     mUser = 0; 
   //:VIEW TZWDVORO    BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:VIEW AD_Base     BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW FindBaseSrc BASED ON LOD  TZWDLGSO
   zVIEW     FindBaseSrc = 0; 
   //:STRING ( 50 )  szAD_Suffix
   zCHAR     szAD_Suffix[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSearchObjectName
   zCHAR     szSearchObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedViewName
   zCHAR     szReturnedViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedRootEntityName
   zCHAR     szReturnedRootEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szER_AttributeName
   zCHAR     szER_AttributeName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szLOD_EntityName
   zCHAR     szLOD_EntityName[ 51 ] = { 0 }; 
   //:STRING ( 200 ) szCompare
   zCHAR     szCompare[ 201 ] = { 0 }; 
   //:STRING ( 50 )  szCompareInit
   zCHAR     szCompareInit[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCompareOption
   zCHAR     szCompareOption[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupTitle
   zCHAR     szGroupTitle[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szWindowName
   zCHAR     szWindowName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewObjRefName
   zCHAR     szViewObjRefName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSourceFileName
   zCHAR     szSourceFileName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchN  // Search object name such as fPerson
   zCHAR     szAD_SchN[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetN  // Returned object/view name such as lPerson
   zCHAR     szAD_RetN[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetE  // Returned object include Entity Name
   zCHAR     szAD_RetE[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetA  // Returned View.Entity.Attribute for display attribute
   zCHAR     szAD_RetA[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdO  // Update object name such as mPerson
   zCHAR     szAD_UpdO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdR  // Update object root entity name such as Person
   zCHAR     szAD_UpdR[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_CLTP  // Update ContactListType value
   zCHAR     szAD_CLTP[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_ID    // Identifier Attribute Name for an Entity
   zCHAR     szAD_ID[ 51 ] = { 0 }; 
   //:STRING ( 200 ) szAD_EMRA  // Update the Email Recipient value in case Email is specified.
   zCHAR     szAD_EMRA[ 201 ] = { 0 }; 
   //:STRING ( 200 ) szAD_DspN  // Update the view.entity.attribute value from the first ContactListItem mapping entry.
   zCHAR     szAD_DspN[ 201 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchEME  // Exact Match Entity Name
   zCHAR     szAD_SchEME[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchEMA  // Exact Match Attribute Name
   zCHAR     szAD_SchEMA[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UserID  // Identifier for User entity.
   zCHAR     szAD_UserID[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_CL_ID   // Identifier for ContactList entity.
   zCHAR     szAD_CL_ID[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCount
   zCHAR     szCount[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szPrompt
   zCHAR     szPrompt[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedEntityName
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlTag
   zCHAR     szControlTag[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szNewGridViewName
   zCHAR     szNewGridViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szFindAreaWindow
   zCHAR     szFindAreaWindow[ 51 ] = { 0 }; 
   //:STRING ( 200 ) szControlText
   zCHAR     szControlText[ 201 ] = { 0 }; 
   //:STRING ( 1 )   szCopyVML_Flag
   zCHAR     szCopyVML_Flag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szCopyExact
   zCHAR     szCopyExact[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szFoundFlag
   zCHAR     szFoundFlag[ 2 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindBaseVML
   zCHAR     szFindBaseVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szDirectoryFileName
   zCHAR     szDirectoryFileName[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szMsg
   zCHAR     szMsg[ 257 ] = { 0 }; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLastPosition
   zLONG     nLastPosition = 0; 
   //:INTEGER nPromptLength
   zLONG     nPromptLength = 0; 
   //:INTEGER nPromptSize
   zLONG     nPromptSize = 0; 
   //:INTEGER nLength
   zLONG     nLength = 0; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:SHORT   nRC2
   zSHORT    nRC2 = 0; 
   //:SHORT   lReturnedLevel
   zSHORT    lReturnedLevel = 0; 
   //:SHORT   lInitialLevel
   zSHORT    lInitialLevel = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zSHORT    lTempInteger_5; 
   zCHAR     szTempString_5[ 33 ]; 
   zSHORT    lTempInteger_6; 
   zCHAR     szTempString_6[ 33 ]; 

   RESULT = GetViewByName( &TZADWWKOT, "TZADWWKO", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Build the Windows for Object Find and Area Maintenance from the AD_Base dialog, particularly the FindArea window template.
   //:// The build steps are as follows in adding windows to the current Dialog:
   //:// 1. Copy FindArea window to create Find_AD_Area window.
   //:// 2. Convert _AD_Suff and _AD_Area characters in ObjectFind Action.Tag and Operation.Name for Find_AD_Area window.
   //:// 3. Build the Search fields group from the selected attributes in TZADWWKO.FlatSelectedSearchAttribute.
   //:// 4. Add the Controls and mapping for the GroupSearchReturnedList grid.
   //:// 5. If this is CRM, modify the view names for the Grids of Contact Lists for this Area.
   //:// 6. Add VML Operations for Find.

   //:// Initialize data.

   //:// If View TZWINDOW doesn't exist, create it from TZWINDOWL. TZWINDOW doesn't exist if a Window isn't open.
   //:GET VIEW TZWINDOW NAMED "TZWINDOW"
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", TZADWWKO, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:CreateViewFromView( TZWINDOW, TZWINDOWL )
      CreateViewFromView( &TZWINDOW, TZWINDOWL );
      //:NAME VIEW TZWINDOW "TZWINDOW"
      SetNameForView( TZWINDOW, "TZWINDOW", 0, zLEVEL_TASK );
   } 

   //:END

   //:SET CURSOR LAST TZWINDOWL.Window    // Position at end of target Dialog.
   RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
   //:szAD_Suffix = TZADWWKO.AutodesignSubdialog.CRM_AreaSuffix 
   GetVariableFromAttribute( szAD_Suffix, 0, 'S', 51, TZADWWKO, "AutodesignSubdialog", "CRM_AreaSuffix", "", 0 );
   //:szAD_Area   = TZADWWKO.AutodesignSubdialog.CRM_AreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "AutodesignSubdialog", "CRM_AreaName", "", 0 );
   //:IF szAD_Suffix = "" OR szAD_Area = ""
   if ( ZeidonStringCompare( szAD_Suffix, 1, 0, "", 1, 0, 51 ) == 0 || ZeidonStringCompare( szAD_Area, 1, 0, "", 1, 0, 51 ) == 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "Both a Find Area Name and a Find Area Suffix must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Both a Find Area Name and a Find Area Suffix must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Check for Search LOD.
   //:GET VIEW SearchLOD NAMED "TZZOLOD1"
   RESULT = GetViewByName( &SearchLOD, "TZZOLOD1", TZADWWKO, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The Search Object has not been selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Search Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Check for Update LOD.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", TZADWWKO, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The Update Object has not been selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Update Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Check for Returned LOD if this is NOT "CRM". For CRM, ReturnedLOD is same as SearchLOD.
   //:IF TZADWWKO.AutodesignSubdialog.FindType = "CRM"
   if ( CompareAttributeToString( TZADWWKO, "AutodesignSubdialog", "FindType", "CRM" ) == 0 )
   { 
      //:ReturnedLOD = SearchLOD
      ReturnedLOD = SearchLOD;
      //:ELSE
   } 
   else
   { 
      //:GET VIEW ReturnedLOD NAMED "TZZOLODO_Return"
      RESULT = GetViewByName( &ReturnedLOD, "TZZOLODO_Return", TZADWWKO, zLEVEL_TASK );
      //:IF RESULT < 0
      if ( RESULT < 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "The Returned Object has not been selected.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The Returned Object has not been selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
   } 

   //:END

   //:// Check for other Update and Returned Object Data.
   //:IF TZADWWKO.LOD_EntityUpdateObjectRoot      DOES NOT EXIST OR
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "LOD_EntityUpdateObjectRoot" );
   //:   TZADWWKO.LOD_EntityReturnedObjectInclude DOES NOT EXIST 
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "LOD_EntityReturnedObjectInclude" );
   if ( lTempInteger_0 != 0 || lTempInteger_1 != 0 )
   { 

      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "All Update and Returned Entity/Attribute entries must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "All Update and Returned Entity/Attribute entries must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Make sure at least one Search Attribute has been selected.
   //:SET CURSOR FIRST TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "At least one Search Attribute must be selected.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "At least one Search Attribute must be selected.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// If this is a CRM Find, activate the CRMBase dialog.
   //:// Note that the directory for the Autodesign templates CAN BE specified in TaskLPLR.LPLR.AutodesignBase.
   //:IF szFindType = "CRM"
   if ( ZeidonStringCompare( szFindType, 1, 0, "CRM", 1, 0, 51 ) == 0 )
   { 
      //:szDirectoryFileName = TaskLPLR.LPLR.AutodesignBase 
      GetVariableFromAttribute( szDirectoryFileName, 0, 'S', 257, TaskLPLR, "LPLR", "AutodesignBase", "", 0 );
      //:IF szDirectoryFileName = ""
      if ( ZeidonStringCompare( szDirectoryFileName, 1, 0, "", 1, 0, 257 ) == 0 )
      { 
         //:szDirectoryFileName = "C:\LPLR\AD_Base"
         ZeidonStringCopy( szDirectoryFileName, 1, 0, "C:\\LPLR\\AD_Base", 1, 0, 257 );
      } 

      //:END
      //:szDirectoryFileName = szDirectoryFileName + "\CRMBase.PWD"
      ZeidonStringConcat( szDirectoryFileName, 1, 0, "\\CRMBase.PWD", 1, 0, 257 );
      //:nRC = ActivateOI_FromFile( CRMBase, "TZWDLGSO", ViewToWindow, szDirectoryFileName, zSINGLE )
      nRC = ActivateOI_FromFile( &CRMBase, "TZWDLGSO", ViewToWindow, szDirectoryFileName, zSINGLE );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "CRMBase PWD could not be activated.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "CRMBase PWD could not be activated.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:NAME VIEW CRMBase "CRMBase"
      SetNameForView( CRMBase, "CRMBase", 0, zLEVEL_TASK );
   } 

   //:END

   //:// 1. Copy FindArea window to create Find_AD_Area window.
   //:IF szFindType = "CRM"
   if ( ZeidonStringCompare( szFindType, 1, 0, "CRM", 1, 0, 51 ) == 0 )
   { 
      //:SET CURSOR FIRST CRMBase.Window WHERE CRMBase.Window.Tag = "FindArea" 
      RESULT = SetCursorFirstEntityByString( CRMBase, "Window", "Tag", "FindArea", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "The FindArea Window could not be found in CRMBase.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The FindArea Window could not be found in CRMBase.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SET CURSOR LAST TZWINDOWL.Window  
      RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
      //:nRC = CloneWindow( TZWINDOWL, CRMBase, TaskLPLR, ViewToWindow )
      nRC = oTZWDLGSO_CloneWindow( TZWINDOWL, CRMBase, TaskLPLR, ViewToWindow );
      //:TraceLineI( "*** CloneWindow RC: ", nRC )
      TraceLineI( "*** CloneWindow RC: ", (zLONG) nRC );
      //:szWindowName = "Find" + szAD_Area
      ZeidonStringCopy( szWindowName, 1, 0, "Find", 1, 0, 51 );
      ZeidonStringConcat( szWindowName, 1, 0, szAD_Area, 1, 0, 51 );
      //:TZWINDOWL.Window.Tag     = szWindowName
      SetAttributeFromString( TZWINDOWL, "Window", "Tag", szWindowName );
      //:TZWINDOWL.Window.Caption = szWindowName
      SetAttributeFromString( TZWINDOWL, "Window", "Caption", szWindowName );

      //:// If the IncludeEmailMergeDocsOption attribute is NOT set, delete the "MergeSelectedOption" 
      //:// button and "GOTO_MergeDocsFor_AD_Area" Action from the window.
      //:IF TZADWWKO.AutoDesignWork.IncludeEmailMergeDocsOption = ""
      if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "IncludeEmailMergeDocsOption", "" ) == 0 )
      { 
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = "GOTO_MergeDocsFor_AD_Area"
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", "GOTO_MergeDocsFor_AD_Area", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:// The Action exists in the template, so first delete the button.
            //:DeleteControlByName( TZADWWKO, TZWINDOWL, "MergeSelectedOption" )
            oTZADWWKO_DeleteControlByName( TZADWWKO, TZWINDOWL, "MergeSelectedOption" );
            //:// Then delete the Action on which we're positioned.
            //:DELETE ENTITY TZWINDOWL.Action  
            RESULT = DeleteEntity( TZWINDOWL, "Action", zPOS_NEXT );
         } 

         //:END 
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:GET VIEW AD_Base NAMED "AD_Base"
      RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
      //:IF TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Icons"
      if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Icons" ) == 0 )
      { 
         //:szFindAreaWindow = "FindAreaIcons" 
         ZeidonStringCopy( szFindAreaWindow, 1, 0, "FindAreaIcons", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szFindAreaWindow = "FindArea"
         ZeidonStringCopy( szFindAreaWindow, 1, 0, "FindArea", 1, 0, 51 );
      } 

      //:END
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = szFindAreaWindow
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", szFindAreaWindow, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:szMsg = "The template window, " + szFindAreaWindow + ", could not be found in AD_Base."
         ZeidonStringCopy( szMsg, 1, 0, "The template window, ", 1, 0, 257 );
         ZeidonStringConcat( szMsg, 1, 0, szFindAreaWindow, 1, 0, 257 );
         ZeidonStringConcat( szMsg, 1, 0, ", could not be found in AD_Base.", 1, 0, 257 );
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog", szMsg,
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:SET CURSOR LAST TZWINDOWL.Window  
      RESULT = SetCursorLastEntity( TZWINDOWL, "Window", "" );
      //:nRC = CloneWindow( TZWINDOWL, AD_Base, TaskLPLR, ViewToWindow )
      nRC = oTZWDLGSO_CloneWindow( TZWINDOWL, AD_Base, TaskLPLR, ViewToWindow );
      //:TraceLineI( "*** CloneWindow RC: ", nRC )
      TraceLineI( "*** CloneWindow RC: ", (zLONG) nRC );
      //:szWindowName = "Find" + szAD_Area
      ZeidonStringCopy( szWindowName, 1, 0, "Find", 1, 0, 51 );
      ZeidonStringConcat( szWindowName, 1, 0, szAD_Area, 1, 0, 51 );
      //:TZWINDOWL.Window.Tag     = szWindowName
      SetAttributeFromString( TZWINDOWL, "Window", "Tag", szWindowName );
      //:TZWINDOWL.Window.Caption = szWindowName
      SetAttributeFromString( TZWINDOWL, "Window", "Caption", szWindowName );
   } 

   //:END

   //:// The Find dialog should have the SourceFile by the same name.
   //:// If it doesn't, we'll create it here.
   //:SET CURSOR FIRST TZWINDOW.SourceFile WHERE TZWINDOW.SourceFile.Name = TZWINDOW.Dialog.Tag 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZWINDOW, "Dialog", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "SourceFile", "Name", szTempString_0, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWINDOW, "SourceFile", zPOS_AFTER );
      //:TZWINDOW.SourceFile.Name = TZWINDOW.Dialog.Tag 
      SetAttributeFromAttribute( TZWINDOW, "SourceFile", "Name", TZWINDOW, "Dialog", "Tag" );
      //:TZWINDOW.SourceFile.Extension    = "VML"
      SetAttributeFromString( TZWINDOW, "SourceFile", "Extension", "VML" );
      //:TZWINDOW.SourceFile.LanguageType = "V"
      SetAttributeFromString( TZWINDOW, "SourceFile", "LanguageType", "V" );
   } 

   //:END 

   //:// 2. Convert _AD_Suff and _AD_Area characters in FindArea Action.Tag and Operation.Name for Find_AD_Area window.
   //://    Also add Dialog and Window names to PICK__AD_Area Action
   //:FOR EACH TZWINDOWL.Action 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Action", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = TZWINDOWL.Action.Tag 
      GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL, "Action", "Tag", "", 0 );
      //:IF szActionName = "PICK__AD_Area" OR szActionName = "NEW__AD_Area"
      if ( ZeidonStringCompare( szActionName, 1, 0, "PICK__AD_Area", 1, 0, 51 ) == 0 || ZeidonStringCompare( szActionName, 1, 0, "NEW__AD_Area", 1, 0, 51 ) == 0 )
      { 
         //:TZWINDOWL.Action.DialogName = TZWINDOW.Dialog.Tag 
         SetAttributeFromAttribute( TZWINDOWL, "Action", "DialogName", TZWINDOW, "Dialog", "Tag" );
         //:TZWINDOWL.Action.WindowName = szAD_Area + "Detail"
         ZeidonStringCopy( szTempString_0, 1, 0, szAD_Area, 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, "Detail", 1, 0, 33 );
         SetAttributeFromString( TZWINDOWL, "Action", "WindowName", szTempString_0 );
      } 

      //:END
      //:zSearchAndReplace( szActionName, 50, "_AD_Suff", szAD_Suffix )
      zSearchAndReplace( szActionName, 50, "_AD_Suff", szAD_Suffix );
      //:zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szActionName, 50, "_AD_Area", szAD_Area );
      //:TZWINDOWL.Action.Tag = szActionName
      SetAttributeFromString( TZWINDOWL, "Action", "Tag", szActionName );
      RESULT = SetCursorNextEntity( TZWINDOWL, "Action", "" );
   } 

   //:END
   //:FOR EACH TZWINDOWL.Operation 
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = TZWINDOWL.Operation.Name 
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, TZWINDOWL, "Operation", "Name", "", 0 );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Suff", szAD_Suffix )
      zSearchAndReplace( szOperationName, 50, "_AD_Suff", szAD_Suffix );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area );
      //:TZWINDOWL.Operation.Name = szOperationName
      SetAttributeFromString( TZWINDOWL, "Operation", "Name", szOperationName );
      RESULT = SetCursorNextEntity( TZWINDOWL, "Operation", "" );
   } 

   //:END

   //:// The Clone should create an Operation Source entry by the name of "FindBase".or "CRMBase".
   //:// Thus we need to move the Operations from that source to the current dialog source.
   //:// We also need to reinclude the moved Operation into the corresponding Window Action.
   //:CreateViewFromView( FindBaseSrc, TZWINDOW )
   CreateViewFromView( &FindBaseSrc, TZWINDOW );
   //:NAME VIEW FindBaseSrc "FindBaseSrc"
   SetNameForView( FindBaseSrc, "FindBaseSrc", 0, zLEVEL_TASK );
   //:IF szFindType = "CRM"
   if ( ZeidonStringCompare( szFindType, 1, 0, "CRM", 1, 0, 51 ) == 0 )
   { 
      //:szSourceFileName = "CRMBase"
      ZeidonStringCopy( szSourceFileName, 1, 0, "CRMBase", 1, 0, 51 );
      //:ELSE
   } 
   else
   { 
      //:szSourceFileName = "FindBase"
      ZeidonStringCopy( szSourceFileName, 1, 0, "FindBase", 1, 0, 51 );
   } 

   //:END
   //:SET CURSOR FIRST FindBaseSrc.SourceFile WHERE FindBaseSrc.SourceFile.Name = szSourceFileName
   RESULT = SetCursorFirstEntityByString( FindBaseSrc, "SourceFile", "Name", szSourceFileName, "" );
   //:FOR EACH FindBaseSrc.Operation 
   RESULT = SetCursorFirstEntity( FindBaseSrc, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = FindBaseSrc.Operation.Name
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, FindBaseSrc, "Operation", "Name", "", 0 );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Suff", szAD_Suffix )
      zSearchAndReplace( szOperationName, 50, "_AD_Suff", szAD_Suffix );
      //:zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area )
      zSearchAndReplace( szOperationName, 50, "_AD_Area", szAD_Area );
      //:SET CURSOR FIRST TZWINDOW.Operation WHERE TZWINDOW.Operation.Name = szOperationName 
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "Operation", "Name", szOperationName, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:// The Operation exists so check if it's tied to an Action. If so, exclude any current Operation and include the new.
         //:SET CURSOR FIRST TZWINDOWL.ActOper WITHIN TZWINDOWL.Window WHERE TZWINDOWL.ActOper.Name = szOperationName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ActOper", "Name", szOperationName, "Window" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:EXCLUDE TZWINDOWL.ActOper 
            RESULT = ExcludeEntity( TZWINDOWL, "ActOper", zREPOS_AFTER );
            //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOW.Operation  
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOW, "Operation", zPOS_AFTER );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// The Operation doesn't exist, so create and tie it to the Action.
         //:SET CURSOR LAST TZWINDOW.Operation
         RESULT = SetCursorLastEntity( TZWINDOW, "Operation", "" );
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZWINDOW, "Operation", FindBaseSrc, "Operation", zSET_NULL )
         SetMatchingAttributesByName( TZWINDOW, "Operation", FindBaseSrc, "Operation", zSET_NULL );
         //:FOR EACH FindBaseSrc.Parameter 
         RESULT = SetCursorFirstEntity( FindBaseSrc, "Parameter", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER )
            CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZWINDOW, "Parameter", FindBaseSrc, "Parameter", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOW, "Parameter", FindBaseSrc, "Parameter", zSET_NULL );
            RESULT = SetCursorNextEntity( FindBaseSrc, "Parameter", "" );
         } 

         //:END
         //:// If the Operation is for an Action in the new window, exclude the original and include the new.
         //:SET CURSOR FIRST TZWINDOWL.ActOper WITHIN TZWINDOWL.Window WHERE TZWINDOWL.ActOper.Name = szOperationName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "ActOper", "Name", szOperationName, "Window" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:EXCLUDE TZWINDOWL.ActOper 
            RESULT = ExcludeEntity( TZWINDOWL, "ActOper", zREPOS_AFTER );
            //:INCLUDE TZWINDOWL.ActOper FROM TZWINDOW.Operation  
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL, "ActOper", TZWINDOW, "Operation", zPOS_AFTER );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( FindBaseSrc, "Operation", "" );
      //:END 
   } 

   //:END
   //:DELETE ENTITY FindBaseSrc.SourceFile  
   RESULT = DeleteEntity( FindBaseSrc, "SourceFile", zPOS_NEXT );
   //:DropView( FindBaseSrc )
   DropView( FindBaseSrc );

   //:// Convert the Header Text if there is a Group named "search-header".
   //:CreateViewFromView( TZCtlHier, TZWINDOWL )
   CreateViewFromView( &TZCtlHier, TZWINDOWL );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:szFoundFlag = ""
   ZeidonStringCopy( szFoundFlag, 1, 0, "", 1, 0, 2 );
   //:LOOP WHILE nRC >= zCURSOR_SET AND szFoundFlag = ""
   while ( nRC >= zCURSOR_SET && ZeidonStringCompare( szFoundFlag, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "CtrlCtrl"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlCtrl", 1, 0, 51 ) == 0 )
      { 
         //:szControlTag = TZCtlHier.Control.Tag 
         GetVariableFromAttribute( szControlTag, 0, 'S', 51, TZCtlHier, "Control", "Tag", "", 0 );
         //:IF szControlTag = "search-header" 
         if ( ZeidonStringCompare( szControlTag, 1, 0, "search-header", 1, 0, 51 ) == 0 )
         { 
            //:CreateViewFromView( TZCONTROL, TZCtlHier )
            CreateViewFromView( &TZCONTROL, TZCtlHier );
            //:NAME VIEW TZCONTROL "TZCONTROLAD"
            SetNameForView( TZCONTROL, "TZCONTROLAD", 0, zLEVEL_TASK );
            //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )  // Step into the Text field.
            SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
            //:szControlText = TZCONTROL.Control.Text
            GetVariableFromAttribute( szControlText, 0, 'S', 201, TZCONTROL, "Control", "Text", "", 0 );
            //:zSearchAndReplace( szControlText, 200, "_AD_Area", szAD_Area )
            zSearchAndReplace( szControlText, 200, "_AD_Area", szAD_Area );
            //:TZCONTROL.Control.Text = szControlText
            SetAttributeFromString( TZCONTROL, "Control", "Text", szControlText );
            //:szFoundFlag = "Y"
            ZeidonStringCopy( szFoundFlag, 1, 0, "Y", 1, 0, 2 );
            //:DropView( TZCONTROL )
            DropView( TZCONTROL );
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )   
   DropView( TZCtlHier );
   //:   
   //:   
   //:// 3. Build the Search fields group from the selected attributes in TZADWWKO.FlatSelectedSearchAttribute.
   //://    In order to reuse the GenDetailGrpBootstrap operation that uses TZADWWKO.DetailMappingLOD_Attribute, we will copy the 
   //://    TZADWWKO.FlatSelectedSearchAttribute entries to create TZADWWKO.DetailMappingLOD_Attribute entries.
   //://    We will also name the SearchLOD "TZZOLODO_Update" because that defines the mapping object used in GenDetailGrpBootstrap. 

   //://    Begin by making sure a ViewObjRef entry exists for the Search LOD and for mCurrentUser.
   //:szSearchObjectName = SearchLOD.LOD.Name 
   GetVariableFromAttribute( szSearchObjectName, 0, 'S', 51, SearchLOD, "LOD", "Name", "", 0 );
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szSearchObjectName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szSearchObjectName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
      //:AddRegisteredViewName( TZADWWKO, TZWINDOW, SearchLOD, szSearchObjectName )
      oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, SearchLOD, szSearchObjectName );
   } 

   //:END
   //:NAME VIEW SearchLOD "TZZOLODO_Update"
   SetNameForView( SearchLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = "mCurrentUser"
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", "mCurrentUser", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
      //:nRC = ActivateMetaOI_ByName( TZADWWKO, mUser, 0, zSOURCE_LOD_META, zSINGLE, "mUser", 0 )
      nRC = ActivateMetaOI_ByName( TZADWWKO, &mUser, 0, zSOURCE_LOD_META, zSINGLE, "mUser", 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
         //:             "LOD mUser does not exist in LPLR.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( ViewToWindow, "", "Autodesign Subdialog", "LOD mUser does not exist in LPLR.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
      //:NAME VIEW mUser "mUser"
      SetNameForView( mUser, "mUser", 0, zLEVEL_TASK );
      //:AddRegisteredViewName( TZADWWKO, TZWINDOW, mUser, "mCurrentUser" )
      oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, mUser, "mCurrentUser" );
      //:DropObjectInstance( mUser )
      DropObjectInstance( mUser );
   } 

   //:END
   //:NAME VIEW SearchLOD "TZZOLODO_Update"
   SetNameForView( SearchLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );

   //:// Because the GenDetailGrpBootstrap routine uses the DetailMapping... subobject, we'll copy the Search subobject to that subobject.
   //:// Also set the Area Name to "Find" as it's used in generating Control Tags.
   //:IF TZADWWKO.EntitySubGroup DOES NOT EXIST 
   lTempInteger_2 = CheckExistenceOfEntity( TZADWWKO, "EntitySubGroup" );
   if ( lTempInteger_2 != 0 )
   { 
      //:CREATE ENTITY TZADWWKO.EntitySubGroup  
      RESULT = CreateEntity( TZADWWKO, "EntitySubGroup", zPOS_AFTER );
   } 

   //:END
   //:TZADWWKO.EntitySubGroup.GroupAreaName = "Find"
   SetAttributeFromString( TZADWWKO, "EntitySubGroup", "GroupAreaName", "Find" );
   //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:EXCLUDE TZADWWKO.DetailMappingLOD_Attribute  NONE 
      RESULT = ExcludeEntity( TZADWWKO, "DetailMappingLOD_Attribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   } 

   //:END
   //:FOR EACH TZADWWKO.FlatSelectedSearchAttribute
   RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:INCLUDE TZADWWKO.DetailMappingLOD_Attribute FROM TZADWWKO.FlatSearchLOD_Attribute 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "FlatSearchLOD_Attribute", zPOS_AFTER );
      //:// Set work attributes.
      //:SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_NULL )  
      SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "FlatSelectedSearchAttribute", zSET_NULL );
      RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
   } 

   //:END

   //:// Step down into the GroupBox that will hold the generated search fields.
   //:// This is currently supporting only Bootstrap.
   //:// Do this by locating the Group, "SearchGroup".
   //:CreateViewFromView( TZCtlHier, TZWINDOWL )
   CreateViewFromView( &TZCtlHier, TZWINDOWL );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:lInitialLevel = lReturnedLevel
   lInitialLevel = lReturnedLevel;
   //:szFoundFlag = ""
   ZeidonStringCopy( szFoundFlag, 1, 0, "", 1, 0, 2 );
   //:LOOP WHILE nRC >= zCURSOR_SET AND szFoundFlag = ""
   while ( nRC >= zCURSOR_SET && ZeidonStringCompare( szFoundFlag, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "CtrlCtrl"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlCtrl", 1, 0, 51 ) == 0 )
      { 
         //:szControlTag = TZCtlHier.Control.Tag 
         GetVariableFromAttribute( szControlTag, 0, 'S', 51, TZCtlHier, "Control", "Tag", "", 0 );
         //:IF szControlTag = "SearchGroup"
         if ( ZeidonStringCompare( szControlTag, 1, 0, "SearchGroup", 1, 0, 51 ) == 0 )
         { 
            //:szFoundFlag = "Y"
            ZeidonStringCopy( szFoundFlag, 1, 0, "Y", 1, 0, 2 );
            //:CreateViewFromView( TZCONTROL, TZCtlHier )
            CreateViewFromView( &TZCONTROL, TZCtlHier );
            //:NAME VIEW TZCONTROL "TZCONTROLAD"
            SetNameForView( TZCONTROL, "TZCONTROLAD", 0, zLEVEL_TASK );
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )
   DropView( TZCtlHier );
   //:IF szFoundFlag = ""
   if ( ZeidonStringCompare( szFoundFlag, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:// The SearchGroup Control wasn't found.
      //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
      //:             "The SearchGroup was not found in AD_Base or CRMBase Find Area. The process is aborted",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( ViewToWindow, "", "Autodesign Subdialog", "The SearchGroup was not found in AD_Base or CRMBase Find Area. The process is aborted", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Generate the Search Controls
   //:GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "Search Fields" )
   oTZADWWKO_GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "Search Fields" );
   //:DropNameForView( SearchLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK )
   DropNameForView( SearchLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
   //:NAME VIEW UpdateLOD "TZZOLODO_Update"    // Rename the UpdateLOD aince we reused the name for the SearchLOD.
   SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
   //:DropView( TZCONTROL )
   DropView( TZCONTROL );

   //:// Resize and reposition the Group Boxes on the Window
   //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL )
   oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL );


   //:// 4. Add the Controls and mapping for the Returned List Grid. 
   //://    For CRM, this is the Grid NOT named "GridContactLists"
   //://    For other, it is the only Grid.
   //://    Thus we can process any Grid NOT named "GridContactLists"

   //:// Process the controls hierarchically looking for the Grids.
   //:CreateViewFromView( TZCtlHier, TZWINDOWL )
   CreateViewFromView( &TZCtlHier, TZWINDOWL );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:lInitialLevel = lReturnedLevel
   lInitialLevel = lReturnedLevel;
   //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel 
   while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "CtrlCtrl"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlCtrl", 1, 0, 51 ) == 0 )
      { 
         //:szControlDef = TZCtlHier.ControlDef.Tag 
         GetVariableFromAttribute( szControlDef, 0, 'S', 51, TZCtlHier, "ControlDef", "Tag", "", 0 );
         //:szControlTag = TZCtlHier.Control.Tag 
         GetVariableFromAttribute( szControlTag, 0, 'S', 51, TZCtlHier, "Control", "Tag", "", 0 );
         //:IF szControlDef = "Grid" AND szControlTag != "GridContactLists" 
         if ( ZeidonStringCompare( szControlDef, 1, 0, "Grid", 1, 0, 51 ) == 0 && ZeidonStringCompare( szControlTag, 1, 0, "GridContactLists", 1, 0, 51 ) != 0 )
         { 
            //:CreateViewFromView( TZCtlTemp, TZCtlHier )
            CreateViewFromView( &TZCtlTemp, TZCtlHier );
            //:NAME VIEW TZCtlTemp "TZCtlTemp"
            SetNameForView( TZCtlTemp, "TZCtlTemp", 0, zLEVEL_TASK );

            //:// The processing of the Grid depends on the type.
            //:IF szFindType = "CRM"
            if ( ZeidonStringCompare( szFindType, 1, 0, "CRM", 1, 0, 51 ) == 0 )
            { 
               //:// Position on mConlist ObjView entry for later include.
               //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = "mConList"
               RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", "mConList", "" );
               //:SetViewToSubobject( TZCtlTemp, "CtrlCtrl" )
               SetViewToSubobject( TZCtlTemp, "CtrlCtrl" );
               //:CreateViewFromView( TZCONTROL2, TZCtlTemp )  // TZCONTROL2 is positioned on first subControl for inclusion of ControlDef.
               CreateViewFromView( &TZCONTROL2, TZCtlTemp );
               //:NAME VIEW TZCONTROL2 "TZCONTROL2"
               SetNameForView( TZCONTROL2, "TZCONTROL2", 0, zLEVEL_TASK );
               //:SET CURSOR NEXT TZCONTROL2.Control   // Position on 2nd Control, which is the sample type. 
               RESULT = SetCursorNextEntity( TZCONTROL2, "Control", "" );
               //:// Add the subcontrols for mConList.
               //:szReturnedRootEntityName = TZADWWKO.LOD_EntityReturnedObjectInclude.Name
               GetVariableFromAttribute( szReturnedRootEntityName, 0, 'S', 51, TZADWWKO, "LOD_EntityReturnedObjectInclude", "Name", "", 0 );
               //:szViewObjRefName = "mConList"
               ZeidonStringCopy( szViewObjRefName, 1, 0, "mConList", 1, 0, 51 );
               //:nRC = ActivateMetaOI_ByName( TZADWWKO, mConList, 0, zSOURCE_LOD_META, zSINGLE, "mConList", 0 )
               nRC = ActivateMetaOI_ByName( TZADWWKO, &mConList, 0, zSOURCE_LOD_META, zSINGLE, "mConList", 0 );
               //:IF nRC < 0
               if ( nRC < 0 )
               { 
                  //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
                  //:             "LOD mConList does not exist in LPLR.",
                  //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                  MessageSend( ViewToWindow, "", "Autodesign Subdialog", "LOD mConList does not exist in LPLR.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                  //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
                  SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
                  //:RETURN -2
                  return( -2 );
               } 

               //:END
               //:NAME VIEW mConList "mConList"
               SetNameForView( mConList, "mConList", 0, zLEVEL_TASK );

               //:// We will skip processing the first control, which is the select CheckBox, except to increment the starting position.
               //:nLastPosition = TZCtlTemp.Control.SZDLG_X   
               GetIntegerFromAttribute( &nLastPosition, TZCtlTemp, "Control", "SZDLG_X" );
               //:SET CURSOR NEXT TZCtlTemp.Control
               RESULT = SetCursorNextEntity( TZCtlTemp, "Control", "" );
               //:Count = 0
               Count = 0;
               //:FOR EACH TZADWWKO.FlatSelectedListAttribute 
               RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:Count = Count + 1
                  Count = Count + 1;
                  //:IF Count = 1   
                  if ( Count == 1 )
                  { 
                     //:// The first mapping entry already exists, except that it is mapping incorrectly.
                     //:// So, we will exclude the current mapping (if it exists) and continue on to regular mapping.
                     //:IF TZCtlTemp.CtrlMapLOD_Attribute EXISTS
                     lTempInteger_3 = CheckExistenceOfEntity( TZCtlTemp, "CtrlMapLOD_Attribute" );
                     if ( lTempInteger_3 == 0 )
                     { 
                        //:EXCLUDE TZCtlTemp.CtrlMapLOD_Attribute  
                        RESULT = ExcludeEntity( TZCtlTemp, "CtrlMapLOD_Attribute", zREPOS_AFTER );
                     } 

                     //:END
                     //:IF TZCtlTemp.CtrlMapView EXISTS
                     lTempInteger_4 = CheckExistenceOfEntity( TZCtlTemp, "CtrlMapView" );
                     if ( lTempInteger_4 == 0 )
                     { 
                        //:EXCLUDE TZCtlTemp.CtrlMapView  
                        RESULT = ExcludeEntity( TZCtlTemp, "CtrlMapView", zREPOS_AFTER );
                     } 

                     //:END

                     //:// Save the view.entity.attribute for use later in setting the mConlist.ContactListItem.Display value.
                     //:// The value is in the form mConlist.entityname.attributename
                     //:szAD_DspN = "mConList." + TZADWWKO.FlatSelectedListAttribute.EntityName + "." + TZADWWKO.FlatSelectedListAttribute.AttributeName 
                     GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZADWWKO, "FlatSelectedListAttribute", "EntityName", "", 0 );
                     ZeidonStringCopy( szAD_DspN, 1, 0, "mConList.", 1, 0, 201 );
                     ZeidonStringConcat( szAD_DspN, 1, 0, szTempString_1, 1, 0, 201 );
                     ZeidonStringConcat( szAD_DspN, 1, 0, ".", 1, 0, 201 );
                     GetVariableFromAttribute( szTempString_2, 0, 'S', 33, TZADWWKO, "FlatSelectedListAttribute", "AttributeName", "", 0 );
                     ZeidonStringConcat( szAD_DspN, 1, 0, szTempString_2, 1, 0, 201 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:// After first entry we are creating Control..
                     //:CreateMetaEntity( TZWINDOWL, TZCtlTemp, "Control", zPOS_AFTER )
                     CreateMetaEntity( TZWINDOWL, TZCtlTemp, "Control", zPOS_AFTER );
                     //:SetMatchingAttributesByName( TZCtlTemp, "Control", TZCONTROL2, "Control", zSET_NULL )
                     SetMatchingAttributesByName( TZCtlTemp, "Control", TZCONTROL2, "Control", zSET_NULL );
                     //:CreateMetaEntity( TZWINDOWL, TZCtlTemp, "CtrlMap", zPOS_AFTER )
                     CreateMetaEntity( TZWINDOWL, TZCtlTemp, "CtrlMap", zPOS_AFTER );
                     //:SetMatchingAttributesByName( TZCtlTemp, "CtrlMap", TZCONTROL2, "CtrlMap", zSET_NULL )
                     SetMatchingAttributesByName( TZCtlTemp, "CtrlMap", TZCONTROL2, "CtrlMap", zSET_NULL );
                     //:INCLUDE TZCtlTemp.ControlDef FROM TZCONTROL2.ControlDef
                     RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "ControlDef", TZCONTROL2, "ControlDef", zPOS_AFTER );
                  } 

                  //:END

                  //:// Process mapping of all entries.
                  //:INCLUDE TZCtlTemp.CtrlMapView FROM TZWINDOWL.ViewObjRef
                  RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapView", TZWINDOWL, "ViewObjRef", zPOS_AFTER );
                  //:zIntegerToString( szCount, 10, Count ) 
                  zIntegerToString( szCount, 10, Count );
                  //:TZCtlTemp.Control.Tag = "ConListGridCtl" + szCount 
                  ZeidonStringCopy( szTempString_3, 1, 0, "ConListGridCtl", 1, 0, 33 );
                  ZeidonStringConcat( szTempString_3, 1, 0, szCount, 1, 0, 33 );
                  SetAttributeFromString( TZCtlTemp, "Control", "Tag", szTempString_3 );
                  //:szPrompt = TZADWWKO.FlatSelectedListAttribute.PromptValue 
                  GetVariableFromAttribute( szPrompt, 0, 'S', 51, TZADWWKO, "FlatSelectedListAttribute", "PromptValue", "", 0 );
                  //:TZCtlTemp.Control.Text = szPrompt   
                  SetAttributeFromString( TZCtlTemp, "Control", "Text", szPrompt );

                  //:// Position on mConList mapping Entity and Attribute and include.under CtrlMap.
                  //:szER_AttributeName = TZADWWKO.FlatSelectedListAttribute.AttributeName 
                  GetVariableFromAttribute( szER_AttributeName, 0, 'S', 51, TZADWWKO, "FlatSelectedListAttribute", "AttributeName", "", 0 );
                  //:szLOD_EntityName   = TZADWWKO.FlatSelectedListAttribute.EntityName 
                  GetVariableFromAttribute( szLOD_EntityName, 0, 'S', 51, TZADWWKO, "FlatSelectedListAttribute", "EntityName", "", 0 );
                  //:SetCursorFirstEntityByString( mConList, "LOD_Entity", "Name", szLOD_EntityName, "" )  // Position of List Entity
                  SetCursorFirstEntityByString( mConList, "LOD_Entity", "Name", szLOD_EntityName, "" );
                  //:SetCursorFirstEntityByString( mConList, "ER_Attribute", "Name", szER_AttributeName, "LOD_Entity" )  // Position of List Entity
                  SetCursorFirstEntityByString( mConList, "ER_Attribute", "Name", szER_AttributeName, "LOD_Entity" );
                  //:IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapLOD_Attribute", mConList, "LOD_Attribute", zPOS_AFTER )
                  IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapLOD_Attribute", mConList, "LOD_Attribute", zPOS_AFTER );

                  //:// Determine position and size. Size will be number of characters plus 5 blanks times 4 units per character.
                  //:TZCtlTemp.Control.PSDLG_X = nLastPosition
                  SetAttributeFromInteger( TZCtlTemp, "Control", "PSDLG_X", nLastPosition );
                  //:nPromptLength = zGetStringLen( szPrompt )
                  nPromptLength = zGetStringLen( szPrompt );
                  //:nPromptSize = (( nPromptLength + 5 ) * 4 )
                  nPromptSize = ( ( nPromptLength + 5 ) * 4 );
                  //:TZCtlTemp.Control.SZDLG_X = nPromptSize
                  SetAttributeFromInteger( TZCtlTemp, "Control", "SZDLG_X", nPromptSize );
                  //:nLastPosition = nLastPosition + nPromptSize    // Set up the position for the next control.
                  nLastPosition = nLastPosition + nPromptSize;
                  RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
               } 

               //:END
               //:ResetViewFromSubobject( TZCtlTemp )
               ResetViewFromSubobject( TZCtlTemp );
               //:DropView( TZCONTROL2 )
               DropView( TZCONTROL2 );
               //:DropView( mConList )
               DropView( mConList );

               //:ELSE
            } 
            else
            { 
               //:// Add the list mapping for a regular returned list.
               //:szReturnedRootEntityName = TZADWWKO.LOD_EntityReturnedObjectInclude.Name
               GetVariableFromAttribute( szReturnedRootEntityName, 0, 'S', 51, TZADWWKO, "LOD_EntityReturnedObjectInclude", "Name", "", 0 );
               //:GetStringFromAttribute( szViewObjRefName, ReturnedLOD, "LOD", "Name" )   // View Name is Returned LOD Name
               GetStringFromAttribute( szViewObjRefName, zsizeof( szViewObjRefName ), ReturnedLOD, "LOD", "Name" );
               //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szViewObjRefName
               RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szViewObjRefName, "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
                  //:AddRegisteredViewName( TZADWWKO, TZWINDOW, ReturnedLOD, szViewObjRefName )
                  oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, ReturnedLOD, szViewObjRefName );
               } 

               //:END

               //:// Modify Grid Mapping View.
               //:EXCLUDE TZCtlTemp.CtrlMapView 
               RESULT = ExcludeEntity( TZCtlTemp, "CtrlMapView", zREPOS_AFTER );
               //:INCLUDE TZCtlTemp.CtrlMapView FROM TZWINDOW.ViewObjRef 
               RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );
               //:EXCLUDE TZCtlTemp.CtrlMapLOD_Entity 
               RESULT = ExcludeEntity( TZCtlTemp, "CtrlMapLOD_Entity", zREPOS_AFTER );
               //:INCLUDE TZCtlTemp.CtrlMapLOD_Entity FROM ReturnedLOD.LOD_EntityParent 
               RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapLOD_Entity", ReturnedLOD, "LOD_EntityParent", zPOS_AFTER );

               //:// Process subcontrol mapping.
               //:SetViewToSubobject( TZCtlTemp, "CtrlCtrl" )
               SetViewToSubobject( TZCtlTemp, "CtrlCtrl" );
               //:CreateViewFromView( TZCONTROL2, TZCtlTemp )  // TZCONTROL2 is positioned on first subControl for inclusion of ControlDef.
               CreateViewFromView( &TZCONTROL2, TZCtlTemp );
               //:NAME VIEW TZCONTROL2 "TZCONTROL2" 
               SetNameForView( TZCONTROL2, "TZCONTROL2", 0, zLEVEL_TASK );
               //:Count = 0
               Count = 0;
               //:FOR EACH TZADWWKO.FlatSelectedListAttribute 
               RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:Count = Count + 1
                  Count = Count + 1;
                  //:IF Count = 1 
                  if ( Count == 1 )
                  { 
                     //:// For the 1st entry, we don't need to create the control, just change it's mapping.
                     //:EXCLUDE TZCtlTemp.CtrlMapLOD_Attribute 
                     RESULT = ExcludeEntity( TZCtlTemp, "CtrlMapLOD_Attribute", zREPOS_AFTER );
                     //:EXCLUDE TZCtlTemp.CtrlMapView   
                     RESULT = ExcludeEntity( TZCtlTemp, "CtrlMapView", zREPOS_AFTER );
                     //:ELSE 
                  } 
                  else
                  { 
                     //:// After first entry we are creating Control..
                     //:CreateMetaEntity( TZWINDOWL, TZCtlTemp, "Control", zPOS_AFTER )
                     CreateMetaEntity( TZWINDOWL, TZCtlTemp, "Control", zPOS_AFTER );
                     //:SetMatchingAttributesByName( TZCtlTemp, "Control", TZCONTROL2, "Control", zSET_NULL )
                     SetMatchingAttributesByName( TZCtlTemp, "Control", TZCONTROL2, "Control", zSET_NULL );
                     //:CreateMetaEntity( TZWINDOWL, TZCtlTemp, "CtrlMap", zPOS_AFTER )
                     CreateMetaEntity( TZWINDOWL, TZCtlTemp, "CtrlMap", zPOS_AFTER );
                     //:SetMatchingAttributesByName( TZCtlTemp, "CtrlMap", TZCONTROL2, "CtrlMap", zSET_NULL )
                     SetMatchingAttributesByName( TZCtlTemp, "CtrlMap", TZCONTROL2, "CtrlMap", zSET_NULL );
                     //:INCLUDE TZCtlTemp.ControlDef FROM TZCONTROL2.ControlDef
                     RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "ControlDef", TZCONTROL2, "ControlDef", zPOS_AFTER );
                  } 

                  //:END

                  //:// Process mapping of all entries.
                  //:INCLUDE TZCtlTemp.CtrlMapView FROM TZWINDOW.ViewObjRef
                  RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );
                  //:zIntegerToString( szCount, 10, Count ) 
                  zIntegerToString( szCount, 10, Count );
                  //:TZCtlTemp.Control.Tag = "ListGridCtl" + szCount 
                  ZeidonStringCopy( szTempString_4, 1, 0, "ListGridCtl", 1, 0, 33 );
                  ZeidonStringConcat( szTempString_4, 1, 0, szCount, 1, 0, 33 );
                  SetAttributeFromString( TZCtlTemp, "Control", "Tag", szTempString_4 );
                  //:szPrompt = TZADWWKO.FlatSelectedListAttribute.PromptValue 
                  GetVariableFromAttribute( szPrompt, 0, 'S', 51, TZADWWKO, "FlatSelectedListAttribute", "PromptValue", "", 0 );
                  //:TZCtlTemp.Control.Text = szPrompt   
                  SetAttributeFromString( TZCtlTemp, "Control", "Text", szPrompt );

                  //:// Position on ReturnedLOD mapping Entity and Attribute and include.under CtrlMap.
                  //:szER_AttributeName = TZADWWKO.FlatSelectedListAttribute.AttributeName 
                  GetVariableFromAttribute( szER_AttributeName, 0, 'S', 51, TZADWWKO, "FlatSelectedListAttribute", "AttributeName", "", 0 );
                  //:szLOD_EntityName   = TZADWWKO.FlatSelectedListAttribute.EntityName 
                  GetVariableFromAttribute( szLOD_EntityName, 0, 'S', 51, TZADWWKO, "FlatSelectedListAttribute", "EntityName", "", 0 );
                  //:SetCursorFirstEntityByString( ReturnedLOD, "LOD_Entity", "Name", szLOD_EntityName, "" )  // Position of List Entity
                  SetCursorFirstEntityByString( ReturnedLOD, "LOD_Entity", "Name", szLOD_EntityName, "" );
                  //:SetCursorFirstEntityByString( ReturnedLOD, "ER_Attribute", "Name", szER_AttributeName, "LOD_Entity" )  // Position of List Entity
                  SetCursorFirstEntityByString( ReturnedLOD, "ER_Attribute", "Name", szER_AttributeName, "LOD_Entity" );
                  //:IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapLOD_Attribute", ReturnedLOD, "LOD_Attribute", zPOS_AFTER )
                  IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapLOD_Attribute", ReturnedLOD, "LOD_Attribute", zPOS_AFTER );

                  //:// Determine position and size. Size will be number of characters plus 5 blanks times 4 units per character.
                  //:TZCtlTemp.Control.PSDLG_X = nLastPosition
                  SetAttributeFromInteger( TZCtlTemp, "Control", "PSDLG_X", nLastPosition );
                  //:nPromptLength = zGetStringLen( szPrompt )
                  nPromptLength = zGetStringLen( szPrompt );
                  //:nPromptSize = (( nPromptLength + 5 ) * 4 )
                  nPromptSize = ( ( nPromptLength + 5 ) * 4 );
                  //:TZCtlTemp.Control.SZDLG_X = nPromptSize
                  SetAttributeFromInteger( TZCtlTemp, "Control", "SZDLG_X", nPromptSize );
                  //:nLastPosition = nLastPosition + nPromptSize    // Set up the position for the next control
                  nLastPosition = nLastPosition + nPromptSize;
                  RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
               } 

               //:END
               //:// Set position of any next control (which would be a button) to next position.
               //:SET CURSOR NEXT TZCtlTemp.Control
               RESULT = SetCursorNextEntity( TZCtlTemp, "Control", "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:TZCtlTemp.Control.PSDLG_X = nLastPosition
                  SetAttributeFromInteger( TZCtlTemp, "Control", "PSDLG_X", nLastPosition );
               } 

               //:END
               //:ResetViewFromSubobject( TZCtlTemp )
               ResetViewFromSubobject( TZCtlTemp );
               //:DropView( TZCONTROL2 )
               DropView( TZCONTROL2 );
            } 

            //:END

            //:DropView( TZCtlTemp )
            DropView( TZCtlTemp );
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )
   DropView( TZCtlHier );


   //:// 5. If this is CRM, modify the view names for the Grids of Contact Lists and Results for this Area.
   //:// We will process the window/page hierarchical looking for the 2 Grids.
   //:IF szFindType = "CRM"
   if ( ZeidonStringCompare( szFindType, 1, 0, "CRM", 1, 0, 51 ) == 0 )
   { 

      //:// Process the controls hierarchically looking for the Grids.
      //:CreateViewFromView( TZCtlHier, TZWINDOWL )
      CreateViewFromView( &TZCtlHier, TZWINDOWL );
      //:DefineHierarchicalCursor( TZCtlHier, "Control" )
      DefineHierarchicalCursor( TZCtlHier, "Control" );
      //:NAME VIEW TZCtlHier "TZCtlHier"
      SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
      //:lInitialLevel = lReturnedLevel
      lInitialLevel = lReturnedLevel;
      //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel 
      while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel )
      { 
         //:IF nRC = zCURSOR_SET_RECURSIVECHILD
         if ( nRC == zCURSOR_SET_RECURSIVECHILD )
         { 
            //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
            SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
         } 

         //:END
         //:IF szReturnedEntityName = "CtrlCtrl"
         if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlCtrl", 1, 0, 51 ) == 0 )
         { 
            //:szControlTag = TZCtlHier.Control.Tag 
            GetVariableFromAttribute( szControlTag, 0, 'S', 51, TZCtlHier, "Control", "Tag", "", 0 );
            //:IF szControlTag = "GridContactLists" OR szControlTag = "GridResultsList"
            if ( ZeidonStringCompare( szControlTag, 1, 0, "GridContactLists", 1, 0, 51 ) == 0 || ZeidonStringCompare( szControlTag, 1, 0, "GridResultsList", 1, 0, 51 ) == 0 )
            { 
               //:CreateViewFromView( TZCtlTemp, TZCtlHier )
               CreateViewFromView( &TZCtlTemp, TZCtlHier );
               //:NAME VIEW TZCtlTemp "TZCtlTemp"
               SetNameForView( TZCtlTemp, "TZCtlTemp", 0, zLEVEL_TASK );

               //:IF szControlTag = "GridContactLists"
               if ( ZeidonStringCompare( szControlTag, 1, 0, "GridContactLists", 1, 0, 51 ) == 0 )
               { 
                  //:// GridContactLists
                  //:// Change view for the list of Contact Lists for the User.
                  //:// The new view has name mConListxxxLST where xxx is the Suffix for the Area for this Find.
                  //:// The view name is the template that is being changed is just mConList.
                  //:szNewGridViewName = "mConList" + szAD_Suffix + "LST"
                  ZeidonStringCopy( szNewGridViewName, 1, 0, "mConList", 1, 0, 51 );
                  ZeidonStringConcat( szNewGridViewName, 1, 0, szAD_Suffix, 1, 0, 51 );
                  ZeidonStringConcat( szNewGridViewName, 1, 0, "LST", 1, 0, 51 );
                  //:ELSE
               } 
               else
               { 
                  //:// GridResultsList
                  //:// Change view for the list of Contact Lists for the User.
                  //:// The new view has name mConListxxx where xxx is the Suffix for the Area for this Find.
                  //:// The view name is the template that is being changed is just mConList.
                  //:szNewGridViewName = "mConList" + szAD_Suffix
                  ZeidonStringCopy( szNewGridViewName, 1, 0, "mConList", 1, 0, 51 );
                  ZeidonStringConcat( szNewGridViewName, 1, 0, szAD_Suffix, 1, 0, 51 );
               } 

               //:END

               //:// Add the ViewObjRef if necessary.
               //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szNewGridViewName
               RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szNewGridViewName, "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
                  //:nRC = ActivateMetaOI_ByName( TZADWWKO, mConList, 0, zREFER_LOD_META, zSINGLE, "mConList", 0 )
                  nRC = ActivateMetaOI_ByName( TZADWWKO, &mConList, 0, zREFER_LOD_META, zSINGLE, "mConList", 0 );
                  //:IF nRC < 0
                  if ( nRC < 0 )
                  { 
                     //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
                     //:             "LOD mConList does not exist in LPLR.",
                     //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                     MessageSend( ViewToWindow, "", "Autodesign Subdialog", "LOD mConList does not exist in LPLR.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                     //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
                     SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
                     //:RETURN -2
                     return( -2 );
                  } 

                  //:END
                  //:NAME VIEW mConList "mConListTemp"
                  SetNameForView( mConList, "mConListTemp", 0, zLEVEL_TASK );
                  //:AddRegisteredViewName( TZADWWKO, TZWINDOW, mConList, szNewGridViewName )
                  oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, mConList, szNewGridViewName );
                  //:DropObjectInstance( mConList )
                  DropObjectInstance( mConList );
               } 

               //:END
               //:// First change for Grid control and then subcontrols.
               //:EXCLUDE TZCtlTemp.CtrlMapView 
               RESULT = ExcludeEntity( TZCtlTemp, "CtrlMapView", zREPOS_AFTER );
               //:INCLUDE TZCtlTemp.CtrlMapView FROM TZWINDOW.ViewObjRef
               RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );
               //:SetViewToSubobject( TZCtlTemp, "CtrlCtrl" )
               SetViewToSubobject( TZCtlTemp, "CtrlCtrl" );
               //:FOR EACH TZCtlTemp.Control 
               RESULT = SetCursorFirstEntity( TZCtlTemp, "Control", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:IF TZCtlTemp.CtrlMapView EXISTS
                  lTempInteger_5 = CheckExistenceOfEntity( TZCtlTemp, "CtrlMapView" );
                  if ( lTempInteger_5 == 0 )
                  { 
                     //:EXCLUDE TZCtlTemp.CtrlMapView 
                     RESULT = ExcludeEntity( TZCtlTemp, "CtrlMapView", zREPOS_AFTER );
                     //:INCLUDE TZCtlTemp.CtrlMapView FROM TZWINDOW.ViewObjRef
                     RESULT = IncludeSubobjectFromSubobject( TZCtlTemp, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );
                  } 

                  RESULT = SetCursorNextEntity( TZCtlTemp, "Control", "" );
                  //:END
               } 

               //:END
               //:DropView( TZCtlTemp )
               DropView( TZCtlTemp );
            } 

            //:END
         } 

         //:END
         //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
         nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
      } 

      //:END
      //:DropView( TZCtlHier )
      DropView( TZCtlHier );
   } 


   //:END

   //:// 6. Add VML Operations for Find.

   //:// Copy VML if operation code is not already in the target VML.
   //:// We will check if it doesn't already exist by looking for the last operation currently in the Dialog in the VML (which was created in 2. above)..
   //:// If that operation is there, the others should also be there and we don't need to check for each operation separately.
   //:szCopyVML_Flag    = ""
   ZeidonStringCopy( szCopyVML_Flag, 1, 0, "", 1, 0, 2 );
   //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:IF szFindType = "CRM"
   if ( ZeidonStringCompare( szFindType, 1, 0, "CRM", 1, 0, 51 ) == 0 )
   { 
      //:szFindBaseVML = "c:\lplr\AD_Base\CRMBase.VML"
      ZeidonStringCopy( szFindBaseVML, 1, 0, "c:\\lplr\\AD_Base\\CRMBase.VML", 1, 0, 257 );
      //:szOperationName   = "PostBuildFind" + szAD_Area
      ZeidonStringCopy( szOperationName, 1, 0, "PostBuildFind", 1, 0, 51 );
      ZeidonStringConcat( szOperationName, 1, 0, szAD_Area, 1, 0, 51 );
      //:ELSE
   } 
   else
   { 
      //:szFindBaseVML = "c:\lplr\AD_Base\FindBase.VML"
      ZeidonStringCopy( szFindBaseVML, 1, 0, "c:\\lplr\\AD_Base\\FindBase.VML", 1, 0, 257 );
      //:szOperationName   = "PostBuildObjectFind" + szAD_Area
      ZeidonStringCopy( szOperationName, 1, 0, "PostBuildObjectFind", 1, 0, 51 );
      ZeidonStringConcat( szOperationName, 1, 0, szAD_Area, 1, 0, 51 );
   } 

   //:END
   //:nLength   = zGetStringLen( szOperationName )
   nLength = zGetStringLen( szOperationName );
   //:szFindVML = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
   ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_5, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
   ZeidonStringConcat( szFindVML, 1, 0, szTempString_5, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
   //:nFileIn   = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ );
   //:IF nFileIn >= 0  
   if ( nFileIn >= 0 )
   { 
      //:// The file exists, so check if the operations already exist. We only need to check the one.
      //:nRC  = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:nRC2 = -1
      nRC2 = -1;
      //:LOOP WHILE nRC = 1 AND nRC2 != 0
      while ( nRC == 1 && nRC2 != 0 )
      { 
         //:strncpy_s( szCompare, szVML_Statement, nLength )    // Copy the necsssary characters of the line to the compare field.
         strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
         //:nRC2 = zstrcmp( szCompare, szOperationName )
         nRC2 = (zSHORT) zstrcmp( szCompare, szOperationName );
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:IF nRC2 != 0
      if ( nRC2 != 0 )
      { 
         //:// The file exists but doesn't have the Find operations, so set flag for copy and open output file for APPEND.
         //:szCopyVML_Flag = "Y"
         ZeidonStringCopy( szCopyVML_Flag, 1, 0, "Y", 1, 0, 2 );
         //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND )
         nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND );
         //:TraceLineI( "*** nFileOut Append: ", nFileOut )
         TraceLineI( "*** nFileOut Append: ", nFileOut );
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// The file doesn't exist, so set flag for copy and open output file for WRITE.
      //:szCopyVML_Flag = "Y"
      ZeidonStringCopy( szCopyVML_Flag, 1, 0, "Y", 1, 0, 2 );
      //:nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE )
      nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE );
   } 

   //:END

   //:IF szCopyVML_Flag = "Y"
   if ( ZeidonStringCompare( szCopyVML_Flag, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:// The Operations are NOT already in the target, so copy them.

      //:// Copy the VML from FindBase.VML and append it to the current Find VML
      //:// Copy the FindBase VML to the end of the Find VML, converting the _AD_Area and _AD_Suff characters in the process.
      //:// Also generate the Range Statement values as needed in place of the characters, "   InitializeFindRangeSets()", in the source VML.
      //:nFileIn   = SysOpenFile( TZWINDOW, szFindBaseVML, COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOW, szFindBaseVML, COREFILE_READ );
      //:szAD_SchN = TZADWWKO.W_MetaDefSearchObject.Name 
      GetVariableFromAttribute( szAD_SchN, 0, 'S', 51, TZADWWKO, "W_MetaDefSearchObject", "Name", "", 0 );
      //:szAD_RetE = TZADWWKO.LOD_EntityReturnedObjectInclude.Name   // Root entity of returned object
      GetVariableFromAttribute( szAD_RetE, 0, 'S', 51, TZADWWKO, "LOD_EntityReturnedObjectInclude", "Name", "", 0 );
      //:szAD_UpdO = TZADWWKO.W_MetaDefUpdateObject.Name 
      GetVariableFromAttribute( szAD_UpdO, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
      //:szAD_UpdR = TZADWWKO.LOD_EntityUpdateObjectRoot.Name 
      GetVariableFromAttribute( szAD_UpdR, 0, 'S', 51, TZADWWKO, "LOD_EntityUpdateObjectRoot", "Name", "", 0 );
      //:ReturnID_NameForEntity( TZADWWKO, szAD_ID, szAD_UpdR, ReturnedLOD )  // Get Identifier Name for Returned LOD & Entity szAD_UpdR
      oTZADWWKO_ReturnID_NameForEntity( TZADWWKO, szAD_ID, szAD_UpdR, ReturnedLOD );
      //:szAD_EMRA = TZADWWKO.AutoDesignWork.RecipientEmailAddressMapping 
      GetVariableFromAttribute( szAD_EMRA, 0, 'S', 201, TZADWWKO, "AutoDesignWork", "RecipientEmailAddressMapping", "", 0 );
      //:SET CURSOR FIRST TZADWWKO.FlatSelectedListAttribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedListAttribute", "" );
      //:szAD_RetA = TZADWWKO.FlatSelectedListAttribute.AttributeName  
      GetVariableFromAttribute( szAD_RetA, 0, 'S', 51, TZADWWKO, "FlatSelectedListAttribute", "AttributeName", "", 0 );
      //:// Returned object view name is different for CRM, where it is the Search Object plus "R"
      //:IF szFindType = "CRM"
      if ( ZeidonStringCompare( szFindType, 1, 0, "CRM", 1, 0, 51 ) == 0 )
      { 
         //:szAD_RetN = szAD_SchN + "R"
         ZeidonStringCopy( szAD_RetN, 1, 0, szAD_SchN, 1, 0, 51 );
         ZeidonStringConcat( szAD_RetN, 1, 0, "R", 1, 0, 51 );
         //:szAD_CLTP = TZADWWKO.ContactListTypeDomain.InternalType 
         GetVariableFromAttribute( szAD_CLTP, 0, 'S', 51, TZADWWKO, "ContactListTypeDomain", "InternalType", "", 0 );

         //:nRC = ActivateMetaOI_ByName( TZADWWKO, mUser, 0, zREFER_LOD_META, zSINGLE, "mUser", 0 )
         nRC = ActivateMetaOI_ByName( TZADWWKO, &mUser, 0, zREFER_LOD_META, zSINGLE, "mUser", 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
            //:             "LOD mUser does not exist in LPLR.",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Autodesign Subdialog", "LOD mUser does not exist in LPLR.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END
         //:NAME VIEW mUser "mUserTemp"
         SetNameForView( mUser, "mUserTemp", 0, zLEVEL_TASK );
         //:nRC = ActivateMetaOI_ByName( TZADWWKO, mConList, 0, zREFER_LOD_META, zSINGLE, "mConList", 0 )
         nRC = ActivateMetaOI_ByName( TZADWWKO, &mConList, 0, zREFER_LOD_META, zSINGLE, "mConList", 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
            //:             "LOD mConList does not exist in LPLR.",
            //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( ViewToWindow, "", "Autodesign Subdialog", "LOD mConList does not exist in LPLR.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0,0 )
            SetWindowActionBehavior( ViewToWindow, zWAB_StayOnWindow, 0, 0 );
            //:RETURN -2
            return( -2 );
         } 

         //:END
         //:NAME VIEW mConList "mConListTemp"
         SetNameForView( mConList, "mConListTemp", 0, zLEVEL_TASK );
         //:ReturnID_NameForEntity( TZADWWKO, szAD_UserID, "User", mUser )           // Get Identifier Name for mUser.User
         oTZADWWKO_ReturnID_NameForEntity( TZADWWKO, szAD_UserID, "User", mUser );
         //:ReturnID_NameForEntity( TZADWWKO, szAD_CL_ID, "ContactList", mConList )  // Get Identifier Name for mConList.ContactList
         oTZADWWKO_ReturnID_NameForEntity( TZADWWKO, szAD_CL_ID, "ContactList", mConList );
         //:DropObjectInstance( mUser )
         DropObjectInstance( mUser );
         //:DropObjectInstance( mConList )
         DropObjectInstance( mConList );
         //:ELSE
      } 
      else
      { 
         //:szAD_RetN = TZADWWKO.W_MetaDefReturnedObject.Name 
         GetVariableFromAttribute( szAD_RetN, 0, 'S', 51, TZADWWKO, "W_MetaDefReturnedObject", "Name", "", 0 );
      } 

      //:END
      //:   
      //:// Make sure a ViewObjRef entry exists for the szAD_RetN view unless this is CRM..)
      //:IF szFindType != "CRM"
      if ( ZeidonStringCompare( szFindType, 1, 0, "CRM", 1, 0, 51 ) != 0 )
      { 
         //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szAD_RetN
         RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szAD_RetN, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
            //:AddRegisteredViewName( TZADWWKO, TZWINDOW, ReturnedLOD, szAD_RetN )
            oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, ReturnedLOD, szAD_RetN );
         } 

         //:END
      } 

      //:END

      //:// Copy and convert Find VML.
      //:szCompareInit = "   // Start FindRangeInitialization"
      ZeidonStringCopy( szCompareInit, 1, 0, "   // Start FindRangeInitialization", 1, 0, 51 );
      //:nLength       = zGetStringLen( szCompareInit )
      nLength = zGetStringLen( szCompareInit );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 

         //:// Check for Option Start/End characters for optionally copying Exact Match Code.
         //:strncpy_s( szCompareOption, szVML_Statement, 6 )
         strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
         //:nRC2 = zstrcmp( szCompareOption, "Option" )
         nRC2 = (zSHORT) zstrcmp( szCompareOption, "Option" );
         //:IF nRC2 = 0
         if ( nRC2 == 0 )
         { 
            //:// Start of optional statements, so loop through them and either copy or skip.
            //:IF TZADWWKO.SelectedExactMatchAttribute EXISTS
            lTempInteger_6 = CheckExistenceOfEntity( TZADWWKO, "SelectedExactMatchAttribute" );
            if ( lTempInteger_6 == 0 )
            { 
               //:szCopyExact = "Y"
               ZeidonStringCopy( szCopyExact, 1, 0, "Y", 1, 0, 2 );
               //:szAD_SchEME = TZADWWKO.PotentialExactMatchLOD_Entity.Name 
               GetVariableFromAttribute( szAD_SchEME, 0, 'S', 51, TZADWWKO, "PotentialExactMatchLOD_Entity", "Name", "", 0 );
               //:szAD_SchEMA = TZADWWKO.PotentialExactMatchER_Attribute.Name 
               GetVariableFromAttribute( szAD_SchEMA, 0, 'S', 51, TZADWWKO, "PotentialExactMatchER_Attribute", "Name", "", 0 );
               //:ELSE
            } 
            else
            { 
               //:szCopyExact = ""
               ZeidonStringCopy( szCopyExact, 1, 0, "", 1, 0, 2 );
            } 

            //:END
            //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )  // Skip the Option Start statement.
            nRC = (zSHORT) zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
            //:strncpy_s( szCompareOption, szVML_Statement, 6 )
            strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
            //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
            while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 51 ) != 0 )
            { 
               //:IF szCopyExact = "Y"
               if ( ZeidonStringCompare( szCopyExact, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:// Convert the line and write it out.
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchN",              szAD_SchN )    // Find Object Name
                  zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchN );
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchExactEntity",    szAD_SchEME )  // Exact Match Entity
                  zSearchAndReplace( szVML_Statement, 256, "_AD_SchExactEntity", szAD_SchEME );
                  //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchExactAttribute", szAD_SchEMA )  // Exact Match Attribute
                  zSearchAndReplace( szVML_Statement, 256, "_AD_SchExactAttribute", szAD_SchEMA );
                  //:SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement ) 
                  SysWriteLine( TZWINDOWL, nFileOut, szVML_Statement );
               } 

               //:END
               //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )
               nRC = (zSHORT) zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
               //:strncpy_s( szCompareOption, szVML_Statement, 6 )
               strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
            } 

            //:END
            //:nRC = zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 )  // Skip the Option End statement.
            nRC = (zSHORT) zSysReadLine( TZWINDOWL, szVML_Statement, nFileIn, 256 );
         } 

         //:END

         //:// Check for character string, "   // Start FindRangeInitialization", and generate Range statements following it.
         //:strncpy_s( szCompare, szVML_Statement, nLength )    // Copy the necsssary characters of the line to the compare field.
         strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
         //:nRC2 = zstrcmp( szCompare, szCompareInit )
         nRC2 = (zSHORT) zstrcmp( szCompare, szCompareInit );
         //:IF nRC2 = 0
         if ( nRC2 == 0 )
         { 
            //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
            SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
            //:// Add appropriate Range initialization statements.
            //:AddFindRangeInitValues( TZADWWKO, TZWINDOW, nFileIn, nFileOut )
            oTZADWWKO_AddFindRangeInitValues( TZADWWKO, TZWINDOW, nFileIn, nFileOut );
            //:ELSE
         } 
         else
         { 
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_Suff", szAD_Suffix )  // Suffix for mConList and lConList..LST
            zSearchAndReplace( szVML_Statement, 256, "_AD_Suff", szAD_Suffix );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area )    // Area name, such as "Person", "Driver" or "Student"
            zSearchAndReplace( szVML_Statement, 256, "_AD_Area", szAD_Area );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchN )    // Search object name such as fPerson
            zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchN );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetN )    // Returned object name, same as szAD_RetN
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetN );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetE", szAD_RetE )    // Returned object include entity name such as Person
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetE", szAD_RetE );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetA", szAD_RetA )    // Returned object first Attribute Name
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetA", szAD_RetA );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdO )    // Update object name such as mPerson
            zSearchAndReplace( szVML_Statement, 256, "_AD_UpdO", szAD_UpdO );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_UpdR", szAD_UpdR )    // Update object root entity name such as Person
            zSearchAndReplace( szVML_Statement, 256, "_AD_UpdR", szAD_UpdR );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_ID",   szAD_ID )      // Update Identifier Name for entity above.
            zSearchAndReplace( szVML_Statement, 256, "_AD_ID", szAD_ID );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_CLTP", szAD_CLTP )    // Update Contact List Type value.
            zSearchAndReplace( szVML_Statement, 256, "_AD_CLTP", szAD_CLTP );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_EMRA", szAD_EMRA )    // Update Recipient Email Address.
            zSearchAndReplace( szVML_Statement, 256, "_AD_EMRA", szAD_EMRA );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_DspN", szAD_DspN )    // Update mConList.ContactListItem.DisplayName setting
            zSearchAndReplace( szVML_Statement, 256, "_AD_DspN", szAD_DspN );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_UserID", szAD_UserID )  // Modify User Identifier
            zSearchAndReplace( szVML_Statement, 256, "_AD_UserID", szAD_UserID );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_CL_ID", szAD_CL_ID )    // Modify ContactList Identifier
            zSearchAndReplace( szVML_Statement, 256, "_AD_CL_ID", szAD_CL_ID );
            //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
            SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
         } 

         //:END 
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = (zSHORT) zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:END

      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:SysCloseFile( TZWINDOW, nFileOut, 0 )
      SysCloseFile( TZWINDOW, nFileOut, 0 );
   } 

   //:END

   //:// If the Email Option is not selected, delete the Email Control, Action and Operation from the Dialog and VML.
   //:// Note that if the Email Option was selected, the GOTO_EmailSelected_AD_Area operation was already updated above with
   //:// the Recipient Email Address variable, _AD_RecipientEmail. Also the Action already specifies the email Dialog/Window values.
   //:IF TZADWWKO.AutoDesignWork.SendEmailType = ""
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SendEmailType", "" ) == 0 )
   { 
      //:// Delete Control
      //:DeleteControlByName( TZADWWKO, TZWINDOWL, "EmailSelectedOption" )
      oTZADWWKO_DeleteControlByName( TZADWWKO, TZWINDOWL, "EmailSelectedOption" );

      //:// Delete Action and Operation from Dialog.
      //:szActionName    = "GOTO_EmailSelected" + szAD_Area
      ZeidonStringCopy( szActionName, 1, 0, "GOTO_EmailSelected", 1, 0, 51 );
      ZeidonStringConcat( szActionName, 1, 0, szAD_Area, 1, 0, 51 );
      //:szOperationName = "GOTO_EmailSelected" + szAD_Area
      ZeidonStringCopy( szOperationName, 1, 0, "GOTO_EmailSelected", 1, 0, 51 );
      ZeidonStringConcat( szOperationName, 1, 0, szAD_Area, 1, 0, 51 );
      //:DeleteActionOperation( TZADWWKO, TZWINDOWL, szActionName, szOperationName )
      oTZADWWKO_DeleteActionOperation( TZADWWKO, TZWINDOWL, szActionName, szOperationName );

      //:// Delete Operation from VML
      //:szDirectoryFileName = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
      ZeidonStringCopy( szDirectoryFileName, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szDirectoryFileName, 1, 0, "\\", 1, 0, 257 );
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
      ZeidonStringConcat( szDirectoryFileName, 1, 0, szTempString_6, 1, 0, 257 );
      ZeidonStringConcat( szDirectoryFileName, 1, 0, ".VML", 1, 0, 257 );
      //:szOperationName = "GOTO_EmailSelected" + szAD_Area
      ZeidonStringCopy( szOperationName, 1, 0, "GOTO_EmailSelected", 1, 0, 51 );
      ZeidonStringConcat( szOperationName, 1, 0, szAD_Area, 1, 0, 51 );
      //:DeleteOperationVML( TZADWWKO, szOperationName, szDirectoryFileName )
      oTZADWWKO_DeleteOperationVML( TZADWWKO, szOperationName, szDirectoryFileName );
   } 

   //:END 

   //:// Go to initialize the necessary Dialog Properties if they haven't already been set.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
   //:SetDialogProperties( TZADWWKO, TZWINDOW, AD_Base )
   oTZADWWKO_SetDialogProperties( TZADWWKO, TZWINDOW, AD_Base );

   //:// Make sure that no EntitySubGroup entities are left as they can create problems later.
   //:FOR EACH TZADWWKO.EntitySubGroup 
   RESULT = SetCursorFirstEntity( TZADWWKO, "EntitySubGroup", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.EntitySubGroup NONE  
      RESULT = DeleteEntity( TZADWWKO, "EntitySubGroup", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "EntitySubGroup", "" );
   } 

   //:END

   //:MessageSend( ViewToWindow, "", "Autodesign Subdialog",
   //:             "Generate Find Dialog is complete.",
   //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
   MessageSend( ViewToWindow, "", "Autodesign Subdialog", "Generate Find Dialog is complete.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:BuildAD_GroupPotList( VIEW TZADWWKO    BASED ON LOD TZADWWKO,
//:                      VIEW SelectedLOD BASED ON LOD TZZOLODO,
//:                      STRING ( 32 ) szStartEntityName,
//:                      STRING ( 1 ) szFlatListNameSuffix )

//:   VIEW RecursiveLOD BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_BuildAD_GroupPotList( zVIEW     TZADWWKO,
                                zVIEW     SelectedLOD,
                                zPCHAR    szStartEntityName,
                                zPCHAR    szFlatListNameSuffix )
{
   zVIEW     RecursiveLOD = 0; 


   //:// Begin the recursive calls to BuildAD_GroupPotListR to copy attributes recursively starting with szStartEntityName.
   //:CreateViewFromView( RecursiveLOD, SelectedLOD )
   CreateViewFromView( &RecursiveLOD, SelectedLOD );
   //:NAME VIEW RecursiveLOD "RecursiveLOD"
   SetNameForView( RecursiveLOD, "RecursiveLOD", 0, zLEVEL_TASK );
   //:BuildAD_GroupPotListR( TZADWWKO, SelectedLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, 1, 0, "" )
   oTZADWWKO_BuildAD_GroupPotListR( TZADWWKO, SelectedLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, 1, 0, "" );
   //:DropView( RecursiveLOD )
   DropView( RecursiveLOD );
   return( 0 );
//    
// END
} 


//:TRANSFORMATION OPERATION
//:BuildAD_GroupPotListR( VIEW TZADWWKO     BASED ON LOD TZADWWKO,
//:                       VIEW MainLOD      BASED ON LOD TZZOLODO,
//:                       VIEW RecursiveLOD BASED ON LOD TZZOLODO,
//:                       STRING ( 32 ) szStartEntityName,     // The Entity Name at which recursive processing starts
//:                       STRING ( 1 )  szFlatListNameSuffix,  // ESG_FlatListPotentialAttribute suffix as explained below
//:                       INTEGER       lCurrentLevel,         // The current recursive level of the LOD_ParentEntity
//:                       INTEGER       lStartLevel,           // The recursive level at which attributes will be copied
//:                       STRING ( 50 ) szIndentSpaces )       // Spaces for indenting the name shown in the list. They begin with lStartLevel

//:   STRING ( 80 ) szIndentEntityName
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_BuildAD_GroupPotListR( zVIEW     TZADWWKO,
                                 zVIEW     MainLOD,
                                 zVIEW     RecursiveLOD,
                                 zPCHAR    szStartEntityName,
                                 zPCHAR    szFlatListNameSuffix,
                                 zLONG     lCurrentLevel,
                                 zLONG     lStartLevel,
                                 zPCHAR    szIndentSpaces )
{
   zCHAR     szIndentEntityName[ 81 ] = { 0 }; 
   //:STRING ( 32 ) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szAttributeName
   zCHAR     szAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szFlatListPotAttributeName
   zCHAR     szFlatListPotAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szESG_FlatLOD_AttributeName
   zCHAR     szESG_FlatLOD_AttributeName[ 33 ] = { 0 }; 
   //:STRING ( 90 ) szPromptText
   zCHAR     szPromptText[ 91 ] = { 0 }; 
   //:STRING ( 90 ) szPromptTextSource
   zCHAR     szPromptTextSource[ 91 ] = { 0 }; 
   //:STRING ( 10 ) szControlType
   zCHAR     szControlType[ 11 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 2 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 2 ]; 


   //:// Build the ESG_FlatListPotentialAttribute entries from the subobject starting with Entity Name, szStartEntityName, and continuing
   //:// for all entities with a higher recursive level.
   //:// Which ESG_FlatListPotentialAttribute subobject to use depends upon the suffix passed of null, "1" or "2".

   //:szEntityName       = RecursiveLOD.LOD_EntityParent.Name 
   GetVariableFromAttribute( szEntityName, 0, 'S', 33, RecursiveLOD, "LOD_EntityParent", "Name", "", 0 );
   //:szIndentEntityName = RecursiveLOD.LOD_EntityParent.IndentName 
   GetVariableFromAttribute( szIndentEntityName, 0, 'S', 81, RecursiveLOD, "LOD_EntityParent", "IndentName", "", 0 );

   //:// Look for the Start Entity Name to set lStartLevel and begin copying attributes.
   //:// The lStartLevel will be zero until we've come across the Start Entity Name.
   //:IF lStartLevel = 0 AND RecursiveLOD.LOD_EntityParent.Name = szStartEntityName
   if ( lStartLevel == 0 && CompareAttributeToString( RecursiveLOD, "LOD_EntityParent", "Name", szStartEntityName ) == 0 )
   { 
      //:lStartLevel = lCurrentLevel
      lStartLevel = lCurrentLevel;
   } 

   //:END
   //:IF lStartLevel > 0 AND lCurrentLevel >= lStartLevel
   if ( lStartLevel > 0 && lCurrentLevel >= lStartLevel )
   { 

      //:szFlatListPotAttributeName  = "ESG_FlatListPotentialAttribute" + szFlatListNameSuffix
      ZeidonStringCopy( szFlatListPotAttributeName, 1, 0, "ESG_FlatListPotentialAttribute", 1, 0, 33 );
      ZeidonStringConcat( szFlatListPotAttributeName, 1, 0, szFlatListNameSuffix, 1, 0, 33 );
      //:szESG_FlatLOD_AttributeName = "ESG_FlatLOD_Attribute"          + szFlatListNameSuffix
      ZeidonStringCopy( szESG_FlatLOD_AttributeName, 1, 0, "ESG_FlatLOD_Attribute", 1, 0, 33 );
      ZeidonStringConcat( szESG_FlatLOD_AttributeName, 1, 0, szFlatListNameSuffix, 1, 0, 33 );

      //:// Position on the LOD_Entity for the current LOD_EntityParent for copying Attributes because the order of the LOD_AttributeRec
      //:// entities isn't always correct.
      //:SET CURSOR FIRST MainLOD.LOD_Entity WHERE MainLOD.LOD_Entity.ZKey = RecursiveLOD.LOD_EntityParent.ZKey  
      GetIntegerFromAttribute( &lTempInteger_0, RecursiveLOD, "LOD_EntityParent", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( MainLOD, "LOD_Entity", "ZKey", lTempInteger_0, "" );
      //:FOR EACH MainLOD.LOD_Attribute
      RESULT = SetCursorFirstEntity( MainLOD, "LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateEntity( TZADWWKO, szFlatListPotAttributeName, zPOS_AFTER )
         CreateEntity( TZADWWKO, szFlatListPotAttributeName, zPOS_AFTER );
         //:szAttributeName = MainLOD.ER_Attribute.Name
         GetVariableFromAttribute( szAttributeName, 0, 'S', 33, MainLOD, "ER_Attribute", "Name", "", 0 );
         //:SetMatchingAttributesByName( TZADWWKO, szFlatListPotAttributeName, MainLOD, "LOD_Entity", zSET_ALL )
         SetMatchingAttributesByName( TZADWWKO, szFlatListPotAttributeName, MainLOD, "LOD_Entity", zSET_ALL );
         //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "EntityName", szEntityName )
         SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "EntityName", szEntityName );
         //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "AttributeName", szAttributeName )
         SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "AttributeName", szAttributeName );
         //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "IndentEntityName", szIndentEntityName )
         SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "IndentEntityName", szIndentEntityName );
         //:IncludeSubobjectFromSubobject( TZADWWKO, szESG_FlatLOD_AttributeName, MainLOD, "LOD_Attribute", zPOS_AFTER )
         IncludeSubobjectFromSubobject( TZADWWKO, szESG_FlatLOD_AttributeName, MainLOD, "LOD_Attribute", zPOS_AFTER );

         //:// Create Control Type based on Update characteristic of Entity and Attribute Domain.
         //:// A Domain of Y/N will create a Checkbox even if the entity is not updatable.
         //:IF MainLOD.LOD_EntityParent.Update = "Y"
         if ( CompareAttributeToString( MainLOD, "LOD_EntityParent", "Update", "Y" ) == 0 )
         { 
            //:ReturnAttrControlType( MainLOD, MainLOD.Domain.Name, MainLOD.Domain.DomainType, szControlType, "Y" )
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), MainLOD, "Domain", "Name" );
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), MainLOD, "Domain", "DomainType" );
            oTZADWWKO_ReturnAttrControlType( MainLOD, szTempString_0, szTempString_1, szControlType, "Y" );
            //:ELSE
         } 
         else
         { 
            //:ReturnAttrControlType( MainLOD, MainLOD.Domain.Name, MainLOD.Domain.DomainType, szControlType, "" )
            GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), MainLOD, "Domain", "Name" );
            GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), MainLOD, "Domain", "DomainType" );
            oTZADWWKO_ReturnAttrControlType( MainLOD, szTempString_2, szTempString_3, szControlType, "" );
         } 

         //:END
         //://TZADWWKO.ESG_FlatListPotentialAttribute.ControlType = szControlType
         //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "ControlType", szControlType )
         SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "ControlType", szControlType );

         //:// The Data Width will always be set to 30.
         //:SetAttributeFromInteger( TZADWWKO, szFlatListPotAttributeName, "DataWidth", 5 )
         SetAttributeFromInteger( TZADWWKO, szFlatListPotAttributeName, "DataWidth", 5 );

         //:// Use Prompt Values if specified. Otherwise use Attribute Name.
         //:// If PromptPaintLit is specified, we'll set both PromptValue and ListTitleValue to it and then override ListTitleValue if
         //:// it is also set.
         //:IF MainLOD.ER_Attribute.PromptPaintLit != ""
         if ( CompareAttributeToString( MainLOD, "ER_Attribute", "PromptPaintLit", "" ) != 0 )
         { 
            //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "PromptValue", MainLOD, "ER_Attribute", "PromptPaintLit" )
            SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "PromptValue", MainLOD, "ER_Attribute", "PromptPaintLit" );
            //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", MainLOD, "ER_Attribute", "PromptPaintLit" )
            SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", MainLOD, "ER_Attribute", "PromptPaintLit" );
            //:ELSE
         } 
         else
         { 
            //:szPromptTextSource = szAttributeName
            ZeidonStringCopy( szPromptTextSource, 1, 0, szAttributeName, 1, 0, 91 );
            //:InsertSpacesInPrompt( szPromptText, TZADWWKO, szPromptTextSource, 90 )
            InsertSpacesInPrompt( szPromptText, TZADWWKO, szPromptTextSource, 90 );
            //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "PromptValue", szPromptText )
            SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "PromptValue", szPromptText );
         } 

         //:END
         //:IF MainLOD.ER_Attribute.ListPaintLit != ""
         if ( CompareAttributeToString( MainLOD, "ER_Attribute", "ListPaintLit", "" ) != 0 )
         { 
            //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", MainLOD, "ER_Attribute", "ListPaintLit" )
            SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", MainLOD, "ER_Attribute", "ListPaintLit" );
            //:ELSE
         } 
         else
         { 
            //:// Don't set it here if it was set from PromtValue.
            //:IF MainLOD.ER_Attribute.PromptPaintLit = ""
            if ( CompareAttributeToString( MainLOD, "ER_Attribute", "PromptPaintLit", "" ) == 0 )
            { 
               //:szPromptTextSource = szAttributeName
               ZeidonStringCopy( szPromptTextSource, 1, 0, szAttributeName, 1, 0, 91 );
               //:InsertSpacesInPrompt( szPromptText, TZADWWKO, szPromptTextSource, 90 )
               InsertSpacesInPrompt( szPromptText, TZADWWKO, szPromptTextSource, 90 );
               //:SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", szPromptText )
               SetAttributeFromString( TZADWWKO, szFlatListPotAttributeName, "ListTitleValue", szPromptText );
            } 

            //:END
         } 

         //:END
         //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "PromptLength", MainLOD, "ER_Attribute", "PromptPaintLth" )
         SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "PromptLength", MainLOD, "ER_Attribute", "PromptPaintLth" );
         //:SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleLength", MainLOD, "ER_Attribute", "ListPaintLth" )
         SetAttributeFromAttribute( TZADWWKO, szFlatListPotAttributeName, "ListTitleLength", MainLOD, "ER_Attribute", "ListPaintLth" );
         RESULT = SetCursorNextEntity( MainLOD, "LOD_Attribute", "" );
      } 

      //:END
   } 

   //:END

   //:// Process subentities.
   //:FOR EACH RecursiveLOD.LOD_EntityChild
   RESULT = SetCursorFirstEntity( RecursiveLOD, "LOD_EntityChild", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( RecursiveLOD, "LOD_EntityChild" )
      SetViewToSubobject( RecursiveLOD, "LOD_EntityChild" );
      //:BuildAD_GroupPotListR( TZADWWKO, MainLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, lCurrentLevel, lStartLevel, szIndentSpaces )
      oTZADWWKO_BuildAD_GroupPotListR( TZADWWKO, MainLOD, RecursiveLOD, szStartEntityName, szFlatListNameSuffix, lCurrentLevel, lStartLevel, szIndentSpaces );
      //:ResetViewFromSubobject( RecursiveLOD )
      ResetViewFromSubobject( RecursiveLOD );
      RESULT = SetCursorNextEntity( RecursiveLOD, "LOD_EntityChild", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CloneControlAD( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                VIEW TZCONTROL  BASED ON LOD TZWDLGSO,
//:                VIEW TZWINDOWL  BASED ON LOD TZWDLGSO,
//:                VIEW AD_BaseCtl BASED ON LOD TZWDLGSO )

//:   VIEW TZPESRCO REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_CloneControlAD( zVIEW     TZADWWKO,
                          zVIEW     TZCONTROL,
                          zVIEW     TZWINDOWL,
                          zVIEW     AD_BaseCtl )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZADWWKO, zLEVEL_TASK );

   //:// Clone the Control from AD_BaseCtl into TZCONTROL, without copying any mapping.

   //:// Copy this Control.
   //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
   CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
   //:SetMatchingAttributesByName( TZCONTROL,  "Control",
   //:                             AD_BaseCtl, "Control", zSET_NULL )
   SetMatchingAttributesByName( TZCONTROL, "Control", AD_BaseCtl, "Control", zSET_NULL );
   //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = AD_BaseCtl.ControlDef.Tag 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "ControlDef", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szTempString_0, "" );
   //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
   RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
   //:FOR EACH AD_BaseCtl.CtrlMap 
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "CtrlMap", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "CtrlMap", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "CtrlMap", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZCONTROL,  "CtrlMap",
      //:                             AD_BaseCtl, "CtrlMap", zSET_NULL )
      SetMatchingAttributesByName( TZCONTROL, "CtrlMap", AD_BaseCtl, "CtrlMap", zSET_NULL );
      RESULT = SetCursorNextEntity( AD_BaseCtl, "CtrlMap", "" );
   } 

   //:END
   //:FOR EACH AD_BaseCtl.WebControlProperty 
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "WebControlProperty", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "WebControlProperty", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "WebControlProperty", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZCONTROL,  "WebControlProperty",
      //:                             AD_BaseCtl, "WebControlProperty", zSET_NULL )
      SetMatchingAttributesByName( TZCONTROL, "WebControlProperty", AD_BaseCtl, "WebControlProperty", zSET_NULL );
      RESULT = SetCursorNextEntity( AD_BaseCtl, "WebControlProperty", "" );
   } 

   //:END

   //:// Clone each Event, as long as there is an Action.
   //:FOR EACH AD_BaseCtl.Event
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "Event", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF AD_BaseCtl.EventAct EXISTS AND AD_BaseCtl.EventAct.Tag != ""
      lTempInteger_0 = CheckExistenceOfEntity( AD_BaseCtl, "EventAct" );
      if ( lTempInteger_0 == 0 && CompareAttributeToString( AD_BaseCtl, "EventAct", "Tag", "" ) != 0 )
      { 
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = AD_BaseCtl.EventAct.Tag
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), AD_BaseCtl, "EventAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szTempString_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CreateMetaEntity( TZADWWKO, TZWINDOWL, "Action", zPOS_AFTER )
            CreateMetaEntity( TZADWWKO, TZWINDOWL, "Action", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZWINDOWL,  "Action",
            //:                             AD_BaseCtl, "EventAct", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOWL, "Action", AD_BaseCtl, "EventAct", zSET_NULL );
         } 

         //:END
         //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Event", zPOS_AFTER )
         CreateMetaEntity( TZADWWKO, TZCONTROL, "Event", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZCONTROL,  "Event",
         //:                             AD_BaseCtl, "Event", zSET_NULL )
         SetMatchingAttributesByName( TZCONTROL, "Event", AD_BaseCtl, "Event", zSET_NULL );
         //:IncludeSubobjectFromSubobject( TZCONTROL, "EventAct",
         //:                               TZWINDOWL, "Action", zPOS_AFTER )
         IncludeSubobjectFromSubobject( TZCONTROL, "EventAct", TZWINDOWL, "Action", zPOS_AFTER );
      } 

      RESULT = SetCursorNextEntity( AD_BaseCtl, "Event", "" );
      //:END
   } 

   //:END

   //:// Clone subcontrols recursively.
   //:FOR EACH AD_BaseCtl.CtrlCtrl
   RESULT = SetCursorFirstEntity( AD_BaseCtl, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" )
      SetViewToSubobject( AD_BaseCtl, "CtrlCtrl" );
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" ) 
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:nRC = CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl )
      nRC = oTZADWWKO_CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl );
      //:ResetViewFromSubobject( AD_BaseCtl )
      ResetViewFromSubobject( AD_BaseCtl );
      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      RESULT = SetCursorNextEntity( AD_BaseCtl, "CtrlCtrl", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CloneSimpleControl( VIEW TZADWWKO    BASED ON LOD TZADWWKO,
//:                    VIEW vControlSrc BASED ON LOD TZWDLGSO,
//:                    VIEW vControlTgt BASED ON LOD TZWDLGSO )

//:   VIEW TZPESRCO   REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_CloneSimpleControl( zVIEW     TZADWWKO,
                              zVIEW     vControlSrc,
                              zVIEW     vControlTgt )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL  REGISTERED AS TZWINDOWL
   zVIEW     TZWINDOWL = 0; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOWL, "TZWINDOWL", TZADWWKO, zLEVEL_TASK );

   //:// Clone the Control from vControlTgt into vControlSrc, without copying any mapping.

   //:// Copy this Control.
   //:CreateMetaEntity( TZADWWKO, vControlTgt, "Control", zPOS_AFTER )
   CreateMetaEntity( TZADWWKO, vControlTgt, "Control", zPOS_AFTER );
   //:SetMatchingAttributesByName( vControlTgt, "Control",
   //:                             vControlSrc, "Control", zSET_NULL )
   SetMatchingAttributesByName( vControlTgt, "Control", vControlSrc, "Control", zSET_NULL );
   //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = vControlSrc.ControlDef.Tag 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vControlSrc, "ControlDef", "Tag" );
   RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szTempString_0, "" );
   //:INCLUDE vControlTgt.ControlDef FROM TZPESRCO.ControlDef 
   RESULT = IncludeSubobjectFromSubobject( vControlTgt, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
   //:/*FOR EACH vControlSrc.CtrlMap 
   //:   CreateMetaEntity( TZADWWKO, vControlTgt, "CtrlMap", zPOS_AFTER )
   //:   SetMatchingAttributesByName( vControlTgt,  "CtrlMap",
   //:                                vControlSrc, "CtrlMap", zSET_NULL )
   //:END*/
   //:FOR EACH vControlSrc.WebControlProperty 
   RESULT = SetCursorFirstEntity( vControlSrc, "WebControlProperty", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( TZADWWKO, vControlTgt, "WebControlProperty", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, vControlTgt, "WebControlProperty", zPOS_AFTER );
      //:SetMatchingAttributesByName( vControlTgt, "WebControlProperty",
      //:                             vControlSrc, "WebControlProperty", zSET_NULL )
      SetMatchingAttributesByName( vControlTgt, "WebControlProperty", vControlSrc, "WebControlProperty", zSET_NULL );
      RESULT = SetCursorNextEntity( vControlSrc, "WebControlProperty", "" );
   } 

   //:END

   //:// Clone each Event, as long as there is an Action.
   //:FOR EACH vControlSrc.Event
   RESULT = SetCursorFirstEntity( vControlSrc, "Event", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vControlSrc.EventAct EXISTS AND vControlSrc.EventAct.Tag != ""
      lTempInteger_0 = CheckExistenceOfEntity( vControlSrc, "EventAct" );
      if ( lTempInteger_0 == 0 && CompareAttributeToString( vControlSrc, "EventAct", "Tag", "" ) != 0 )
      { 
         //:SET CURSOR FIRST TZWINDOWL.Action WHERE TZWINDOWL.Action.Tag = vControlSrc.EventAct.Tag
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vControlSrc, "EventAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Action", "Tag", szTempString_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:CreateMetaEntity( TZADWWKO, TZWINDOWL, "Action", zPOS_AFTER )
            CreateMetaEntity( TZADWWKO, TZWINDOWL, "Action", zPOS_AFTER );
            //:SetMatchingAttributesByName( TZWINDOWL,   "Action",
            //:                             vControlSrc, "EventAct", zSET_NULL )
            SetMatchingAttributesByName( TZWINDOWL, "Action", vControlSrc, "EventAct", zSET_NULL );
         } 

         //:END
         //:CreateMetaEntity( TZADWWKO, vControlTgt, "Event", zPOS_AFTER )
         CreateMetaEntity( TZADWWKO, vControlTgt, "Event", zPOS_AFTER );
         //:SetMatchingAttributesByName( vControlTgt, "Event",
         //:                             vControlSrc, "Event", zSET_NULL )
         SetMatchingAttributesByName( vControlTgt, "Event", vControlSrc, "Event", zSET_NULL );
         //:IncludeSubobjectFromSubobject( vControlTgt, "EventAct",
         //:                               TZWINDOWL,   "Action", zPOS_AFTER )
         IncludeSubobjectFromSubobject( vControlTgt, "EventAct", TZWINDOWL, "Action", zPOS_AFTER );
      } 

      RESULT = SetCursorNextEntity( vControlSrc, "Event", "" );
      //:END
   } 

   //:END

   //:// Clone subcontrols recursively.
   //:FOR EACH vControlSrc.CtrlCtrl
   RESULT = SetCursorFirstEntity( vControlSrc, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( vControlSrc, "CtrlCtrl" )
      SetViewToSubobject( vControlSrc, "CtrlCtrl" );
      //:SetViewToSubobject( vControlTgt, "CtrlCtrl" ) 
      SetViewToSubobject( vControlTgt, "CtrlCtrl" );
      //:nRC = CloneSimpleControl( TZADWWKO, vControlSrc, vControlTgt )
      nRC = oTZADWWKO_CloneSimpleControl( TZADWWKO, vControlSrc, vControlTgt );
      //:ResetViewFromSubobject( vControlSrc )
      ResetViewFromSubobject( vControlSrc );
      //:ResetViewFromSubobject( vControlTgt )
      ResetViewFromSubobject( vControlTgt );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN nRC
         return( nRC );
      } 

      RESULT = SetCursorNextEntity( vControlSrc, "CtrlCtrl", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ConvertHeaderText( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                   VIEW TZCONTROL BASED ON LOD TZWDLGSO )

//:   VIEW TZCtlTemp BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_ConvertHeaderText( zVIEW     TZADWWKO,
                             zVIEW     TZCONTROL )
{
   zVIEW     TZCtlTemp = 0; 
   //:VIEW TZCtlHier BASED ON LOD TZWDLGSO
   zVIEW     TZCtlHier = 0; 
   //:STRING ( 50 )  szAreaName
   zCHAR     szAreaName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAreaTitle
   zCHAR     szAreaTitle[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedEntityName
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCSS_Class
   zCHAR     szCSS_Class[ 51 ] = { 0 }; 
   //:STRING ( 200 ) szControlText
   zCHAR     szControlText[ 201 ] = { 0 }; 
   //:STRING ( 200 ) szHeaderTag
   zCHAR     szHeaderTag[ 201 ] = { 0 }; 
   //:SHORT lReturnedLevel
   zSHORT    lReturnedLevel = 0; 
   //:SHORT lInitialLevel
   zSHORT    lInitialLevel = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 


   //:// BOOTSTRAP OPTION 1 (Group with "collapse show" and header text)

   //:// This operation is used only for modifying the header text in a particular option of Bootstrap and for setting
   //:// values in the "collapse show" group that goes with it.
   //:// The Group template for this option has the List or Detail Group just generated imbedded within one or more parent Groups.
   //:// This code first positions back to the parent "card" Group.
   //:// It then searches both for the header text and collapse/show Group.
   //:// For the header text, it replaces modifiable variables _Section and _SectionTitle with Group Area Name and Group Area Title.
   //:// For the "collapse show" group, it modifies the Control Tag and HTML5 values.

   //:// This will be done by processing all the subControls of the "card" Group hierarchially to locate the Controls needing to be converted.

   //:// Back up to the "card Groupbox as necesary.
   //:IF TZCONTROL.Control.CSS_Class != "card"
   if ( CompareAttributeToString( TZCONTROL, "Control", "CSS_Class", "card" ) != 0 )
   { 
      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
      //:IF TZCONTROL.Control.CSS_Class != "card"
      if ( CompareAttributeToString( TZCONTROL, "Control", "CSS_Class", "card" ) != 0 )
      { 
         //:ResetViewFromSubobject( TZCONTROL )
         ResetViewFromSubobject( TZCONTROL );
         //:IF TZCONTROL.Control.CSS_Class != "card"
         if ( CompareAttributeToString( TZCONTROL, "Control", "CSS_Class", "card" ) != 0 )
         { 
            //:ResetViewFromSubobject( TZCONTROL )
            ResetViewFromSubobject( TZCONTROL );
            //:IF TZCONTROL.Control.CSS_Class != "card"
            if ( CompareAttributeToString( TZCONTROL, "Control", "CSS_Class", "card" ) != 0 )
            { 
               //:ResetViewFromSubobject( TZCONTROL )
               ResetViewFromSubobject( TZCONTROL );
            } 

            //:END
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:szAreaName  = TZADWWKO.EntitySubGroup.GroupAreaName 
   GetVariableFromAttribute( szAreaName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:szAreaTitle = TZADWWKO.EntitySubGroup.GroupAreaTitle
   GetVariableFromAttribute( szAreaTitle, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaTitle", "", 0 );

   //:// Process the controls hierarchically looking for the Text Controls..
   //:CreateViewFromView( TZCtlHier, TZCONTROL )
   CreateViewFromView( &TZCtlHier, TZCONTROL );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:lInitialLevel = lReturnedLevel
   lInitialLevel = lReturnedLevel;
   //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel 
   while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "CtrlCtrl"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlCtrl", 1, 0, 51 ) == 0 )
      { 
         //:szControlDef = TZCtlHier.ControlDef.Tag 
         GetVariableFromAttribute( szControlDef, 0, 'S', 51, TZCtlHier, "ControlDef", "Tag", "", 0 );
         //:IF szControlDef = "Text"
         if ( ZeidonStringCompare( szControlDef, 1, 0, "Text", 1, 0, 51 ) == 0 )
         { 
            //:// This is a Text Control, so convert any _Section or _SectionTitle characters
            //:CreateViewFromView( TZCtlTemp, TZCtlHier )
            CreateViewFromView( &TZCtlTemp, TZCtlHier );
            //:NAME VIEW TZCtlTemp "TZCtlTemp"
            SetNameForView( TZCtlTemp, "TZCtlTemp", 0, zLEVEL_TASK );

            //:// Header Tag
            //:szHeaderTag = TZCtlTemp.Control.Tag 
            GetVariableFromAttribute( szHeaderTag, 0, 'S', 201, TZCtlTemp, "Control", "Tag", "", 0 );
            //:zSearchAndReplace( szHeaderTag, 200, "_Section", szAreaName )
            zSearchAndReplace( szHeaderTag, 200, "_Section", szAreaName );
            //:TZCtlTemp.Control.Tag = szHeaderTag
            SetAttributeFromString( TZCtlTemp, "Control", "Tag", szHeaderTag );

            //:// Header Text
            //:szControlText = TZCtlTemp.Control.Text 
            GetVariableFromAttribute( szControlText, 0, 'S', 201, TZCtlTemp, "Control", "Text", "", 0 );
            //:zSearchAndReplace( szControlText, 200, "_SectionTitle", szAreaTitle )  // Title must be replaced first because it also has _Section it it.
            zSearchAndReplace( szControlText, 200, "_SectionTitle", szAreaTitle );
            //:zSearchAndReplace( szControlText, 200, "_Section", szAreaName )
            zSearchAndReplace( szControlText, 200, "_Section", szAreaName );
            //:TZCtlTemp.Control.Text = szControlText
            SetAttributeFromString( TZCtlTemp, "Control", "Text", szControlText );

            //:DropView( TZCtlTemp )
            DropView( TZCtlTemp );
            //:ELSE
         } 
         else
         { 
            //:IF szControlDef = "GroupBox"
            if ( ZeidonStringCompare( szControlDef, 1, 0, "GroupBox", 1, 0, 51 ) == 0 )
            { 
               //:szCSS_Class = TZCtlHier.Control.CSS_Class 
               GetVariableFromAttribute( szCSS_Class, 0, 'S', 51, TZCtlHier, "Control", "CSS_Class", "", 0 );
               //:IF szCSS_Class = "collapse show"
               if ( ZeidonStringCompare( szCSS_Class, 1, 0, "collapse show", 1, 0, 51 ) == 0 )
               { 
                  //:CreateViewFromView( TZCtlTemp, TZCtlHier )
                  CreateViewFromView( &TZCtlTemp, TZCtlHier );
                  //:NAME VIEW TZCtlTemp "TZCtlTemp"
                  SetNameForView( TZCtlTemp, "TZCtlTemp", 0, zLEVEL_TASK );

                  //:// Set the GroupBox Tag for the "collapse show" Group to AreaName.
                  //:TZCtlTemp.Control.Tag = szAreaName
                  SetAttributeFromString( TZCtlTemp, "Control", "Tag", szAreaName );

                  //:// Convert the HTML5 text to replace _Section with AreaName.
                  //:szControlText = TZCtlTemp.Control.WebHTML5Attribute 
                  GetVariableFromAttribute( szControlText, 0, 'S', 201, TZCtlTemp, "Control", "WebHTML5Attribute", "", 0 );
                  //:zSearchAndReplace( szControlText, 200, "_Section", szAreaName )
                  zSearchAndReplace( szControlText, 200, "_Section", szAreaName );
                  //:TZCtlTemp.Control.WebHTML5Attribute = szControlText
                  SetAttributeFromString( TZCtlTemp, "Control", "WebHTML5Attribute", szControlText );

                  //:DropView( TZCtlTemp )
                  DropView( TZCtlTemp );
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END

      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )
   DropView( TZCtlHier );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DeleteActionOperation( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                       STRING ( 32 ) szActionName,
//:                       STRING ( 32 ) szOperationName )

//:   VIEW TZCtlTemp BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_DeleteActionOperation( zVIEW     TZADWWKO,
                                 zVIEW     TZWINDOWL,
                                 zPCHAR    szActionName,
                                 zPCHAR    szOperationName )
{
   zVIEW     TZCtlTemp = 0; 
   zSHORT    RESULT; 


   //:// Delete the Action and Operation with the current Window for the names passed in to this operation.
   //:CreateViewFromView( TZCtlTemp, TZWINDOWL )
   CreateViewFromView( &TZCtlTemp, TZWINDOWL );
   //:SET CURSOR FIRST TZCtlTemp.Action WHERE TZCtlTemp.Action.Tag = szActionName
   RESULT = SetCursorFirstEntityByString( TZCtlTemp, "Action", "Tag", szActionName, "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:DELETE ENTITY TZCtlTemp.Action NONE 
      RESULT = DeleteEntity( TZCtlTemp, "Action", zREPOS_NONE );
   } 

   //:END
   //:SET CURSOR FIRST TZCtlTemp.Operation WHERE TZCtlTemp.Operation.Name = szOperationName
   RESULT = SetCursorFirstEntityByString( TZCtlTemp, "Operation", "Name", szOperationName, "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:DELETE ENTITY TZCtlTemp.szOperationName NONE 
      RESULT = DeleteEntity( TZCtlTemp, szOperationName, zREPOS_NONE );
   } 

   //:END

   //:DropView( TZCtlTemp )
   DropView( TZCtlTemp );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DeleteControlByName( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                     VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                     STRING ( 32 ) szControlName )

//:   VIEW TZCtlTemp BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_DeleteControlByName( zVIEW     TZADWWKO,
                               zVIEW     TZWINDOWL,
                               zPCHAR    szControlName )
{
   zVIEW     TZCtlTemp = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 


   //:// Delete the identified Control within the current Window.
   //:nRC = PositionOnControlByTag( TZADWWKO, TZCtlTemp, TZWINDOWL, szControlName )
   nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCtlTemp, TZWINDOWL, szControlName );
   //:IF nRC = 0
   if ( nRC == 0 )
   { 
      //:DELETE ENTITY TZCtlTemp.Control NONE
      RESULT = DeleteEntity( TZCtlTemp, "Control", zREPOS_NONE );
      //:DropView( TZCtlTemp )
      DropView( TZCtlTemp );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DeleteOperationVML( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                    STRING ( 32 )  szOperationName,
//:                    STRING ( 200 ) szVML_DirectoryFileName )

//:   STRING ( 256 ) szVML_Statement
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_DeleteOperationVML( zVIEW     TZADWWKO,
                              zPCHAR    szOperationName,
                              zPCHAR    szVML_DirectoryFileName )
{
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 32 )  szCompare
   zCHAR     szCompare[ 33 ] = { 0 }; 
   //:STRING ( 1 )   szInOperationFlag
   zCHAR     szInOperationFlag[ 2 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER Length
   zLONG     Length = 0; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER StartOperationLine
   zLONG     StartOperationLine = 0; 


   //:// Search through the VML content of the file passed in and delete the VML for the specified operation.
   //:// The complexity is that once we locate the operation, we must back up 7 lines and start deleting with
   //:// the blank line before the comments
   //:// Thus, we will make an initial pass copying each line to a temp file and determining on what line the operation name starts.
   //:// Then make a second pass in which we copy each line except that we skip all lines starting with the line number of
   //:// the operation to be skipped 

   //:Length = zGetStringLen( szOperationName )
   Length = zGetStringLen( szOperationName );
   //:nFileIn  = SysOpenFile( TZADWWKO, szVML_DirectoryFileName, COREFILE_READ )
   nFileIn = SysOpenFile( TZADWWKO, szVML_DirectoryFileName, COREFILE_READ );
   //:nFileOut = SysOpenFile( TZADWWKO, "c:\Temp\TempFile.vml", COREFILE_WRITE )
   nFileOut = SysOpenFile( TZADWWKO, "c:\\Temp\\TempFile.vml", COREFILE_WRITE );
   //:nRC2 = -1
   nRC2 = -1;
   //:IF nFileIn > 0   // Don't continue if the VML file doesn't exist.
   if ( nFileIn > 0 )
   { 
      //:Count = 0
      Count = 0;
      //:nRC = zSysReadLine( TZADWWKO, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZADWWKO, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 
         //:SysWriteLine( TZADWWKO, nFileOut, szVML_Statement )
         SysWriteLine( TZADWWKO, nFileOut, szVML_Statement );
         //:Count = Count + 1
         Count = Count + 1;
         //:strncpy_s( szCompare, szVML_Statement, Length )
         strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, Length );
         //:nRC2 = zstrcmp( szCompare, szOperationName )
         nRC2 = zstrcmp( szCompare, szOperationName );
         //:IF nRC2 = 0
         if ( nRC2 == 0 )
         { 
            //:StartOperationLine = Count - 7    // We will back up 7 lines to before the comment lines for the operation
            StartOperationLine = Count - 7;
         } 

         //:END
         //:nRC = zSysReadLine( TZADWWKO, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZADWWKO, szVML_Statement, nFileIn, 256 );
      } 

      //:END
      //:SysCloseFile( TZADWWKO, nFileIn, 0 )
      SysCloseFile( TZADWWKO, nFileIn, 0 );
      //:SysCloseFile( TZADWWKO, nFileOut, 0 )
      SysCloseFile( TZADWWKO, nFileOut, 0 );
      //:ELSE
   } 
   else
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:          "The file for DeleteOperationVML cannot be opened.",
      //:          zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "The file for DeleteOperationVML cannot be opened.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:// Now copy the lines from the work file back to the main file except for skipping the lines starting with the StartOperationLine
   //:// value and continuing to the "END" statement for the operation.
   //:nFileOut = SysOpenFile( TZADWWKO, szVML_DirectoryFileName, COREFILE_WRITE )
   nFileOut = SysOpenFile( TZADWWKO, szVML_DirectoryFileName, COREFILE_WRITE );
   //:nFileIn  = SysOpenFile( TZADWWKO, "c:\Temp\TempFile.vml", COREFILE_READ )
   nFileIn = SysOpenFile( TZADWWKO, "c:\\Temp\\TempFile.vml", COREFILE_READ );
   //:nRC2 = -1
   nRC2 = -1;
   //:szInOperationFlag = ""
   ZeidonStringCopy( szInOperationFlag, 1, 0, "", 1, 0, 2 );
   //:Count = 0
   Count = 0;
   //:nRC = zSysReadLine( TZADWWKO, szVML_Statement, nFileIn, 256 )
   nRC = zSysReadLine( TZADWWKO, szVML_Statement, nFileIn, 256 );
   //:LOOP WHILE nRC = 1 
   while ( nRC == 1 )
   { 
      //:Count = Count + 1
      Count = Count + 1;
      //:IF Count = StartOperationLine
      if ( Count == StartOperationLine )
      { 
         //:szInOperationFlag = "Y"
         ZeidonStringCopy( szInOperationFlag, 1, 0, "Y", 1, 0, 2 );
      } 

      //:END
      //:IF szInOperationFlag = ""
      if ( ZeidonStringCompare( szInOperationFlag, 1, 0, "", 1, 0, 2 ) == 0 )
      { 
         //:// Since we're not inside the operation, copy the line.
         //:SysWriteLine( TZADWWKO, nFileOut, szVML_Statement )
         SysWriteLine( TZADWWKO, nFileOut, szVML_Statement );
      } 

      //:END
      //:// See if we've come across the end of the Operation yet.
      //:strncpy_s( szCompare, szVML_Statement, 3 )   
      strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, 3 );
      //:nRC2 = zstrcmp( szCompare, "END" )
      nRC2 = zstrcmp( szCompare, "END" );
      //:IF nRC2 = 0
      if ( nRC2 == 0 )
      { 
         //:// Indicate we've come to the end of the Operation. (This is OK if we've come to the end of any Operation.)
         //:szInOperationFlag = ""
         ZeidonStringCopy( szInOperationFlag, 1, 0, "", 1, 0, 2 );
      } 

      //:END
      //:nRC = zSysReadLine( TZADWWKO, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZADWWKO, szVML_Statement, nFileIn, 256 );
   } 

   //:END
   //:SysCloseFile( TZADWWKO, nFileIn, 0 )
   SysCloseFile( TZADWWKO, nFileIn, 0 );
   //:SysCloseFile( TZADWWKO, nFileOut, 0 )
   SysCloseFile( TZADWWKO, nFileOut, 0 );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenAD_GroupNames( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                  VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                  STRING ( 20 ) szGroupArea,
//:                  STRING ( 32 ) szEntityName,
//:                  INTEGER nLevel )

//:   STRING ( 1 ) szLevel
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenAD_GroupNames( zVIEW     TZADWWKO,
                            zVIEW     TZCONTROL,
                            zPCHAR    szGroupArea,
                            zPCHAR    szEntityName,
                            zLONG     nLevel )
{
   zCHAR     szLevel[ 2 ] = { 0 }; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 


   //:// Recursive routine to set Tag and Title of each Group set for Area generation.
   //://szLevel = nLevel    This statement gave compile error.
   //:zIntegerToString( szLevel, 2, nLevel )
   zIntegerToString( szLevel, 2, nLevel );
   //:IF nLevel = 1
   if ( nLevel == 1 )
   { 
      //:// Top level sets both Tag and Title.
      //:TZCONTROL.Control.Tag  = "Group" + szEntityName + szLevel
      ZeidonStringCopy( szTempString_0, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szEntityName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szLevel, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
      //:ELSE
   } 
   else
   { 
      //:// Other levels only set Tag.
      //:TZCONTROL.Control.Tag  = "Group" + szEntityName + szLevel
      ZeidonStringCopy( szTempString_1, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, szEntityName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, szLevel, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_1 );
   } 

   //:END

   //:// If CtrlCtrl exists and is GroupBox, also process it recursively.
   //:IF TZCONTROL.CtrlCtrl EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" ) 
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:IF TZCONTROL.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( TZCONTROL, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:nLevel = nLevel + 1
         nLevel = nLevel + 1;
         //:GenAD_GroupNames( TZADWWKO, TZCONTROL, szGroupArea, szEntityName, nLevel )
         oTZADWWKO_GenAD_GroupNames( TZADWWKO, TZCONTROL, szGroupArea, szEntityName, nLevel );
      } 

      //:END
      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenDetailGrpBootstrap( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                       VIEW TZCURWND  BASED ON LOD TZWDLGSO,
//:                       STRING ( 20 ) szType )
//:                       

//:   VIEW TZPESRCO   REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenDetailGrpBootstrap( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     TZCURWND,
                                 zPCHAR    szType )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOW   REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW AD_Base    BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW AD_BaseCtl BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseCtl = 0; 
   //:VIEW UpdateLOD  BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW TZWDVORO   BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:VIEW TZCONTROLB BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROLB = 0; 
   //:STRING ( 256 ) szControlText
   zCHAR     szControlText[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szPromptControlName
   zCHAR     szPromptControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szDataControlName
   zCHAR     szDataControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_AreaTitle
   zCHAR     szAD_AreaTitle[ 51 ] = { 0 }; 
   //:STRING ( 10 )  szCurrentRow
   zCHAR     szCurrentRow[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szCurrentPair
   zCHAR     szCurrentPair[ 11 ] = { 0 }; 
   //:STRING ( 1 )   szGroupOrRowType
   zCHAR     szGroupOrRowType[ 2 ] = { 0 }; 
   //:INTEGER CurrentRow
   zLONG     CurrentRow = 0; 
   //:INTEGER LastRow
   zLONG     LastRow = 0; 
   //:INTEGER RowCount
   zLONG     RowCount = 0; 
   //:INTEGER DetailPairCount
   zLONG     DetailPairCount = 0; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", TZADWWKO, zLEVEL_TASK );

   //:// Generate a Detail Group of Prompt / Mapping Control for Bootstrap from TZADWWKO.DetailMappingLOD_Attribute entries.
   //:// This operation supports Prompt/Control pairs organized as either Rows or Groups, depending on the settings of 
   //:// GroupRow or GroupNumber. If neither is specified, GroupNumber is assumed and is defaulted to one Group.
   //:// The special handling of GroupRow or GroupNumber is handled by a special suboperation for each type.

   //:// Begin by accepting the current GroupBox since it may be versioned.
   //:AcceptSubobject( TZCONTROL, "Control" )
   AcceptSubobject( TZCONTROL, "Control" );
   //:  
   //:// Get AD_Base object (which is already the Dialog for the correct framework).and position on the correct Window by Type.
   //:// For those not included in the IF, we're already correctly positioned.
   //:GET VIEW AD_Base NAMED "AD_Base"
   RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
   //:IF szType = "UpdateGroup" OR szType = "NewGroup" OR szType = "MultiGroup" OR szType = "SubPageGroup"
   if ( ZeidonStringCompare( szType, 1, 0, "UpdateGroup", 1, 0, 21 ) == 0 || ZeidonStringCompare( szType, 1, 0, "NewGroup", 1, 0, 21 ) == 0 || ZeidonStringCompare( szType, 1, 0, "MultiGroup", 1, 0, 21 ) == 0 ||
        ZeidonStringCompare( szType, 1, 0, "SubPageGroup", 1, 0, 21 ) == 0 )
   { 
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "UpdateGroupTemplates" 
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "UpdateGroupTemplates", "" );
   } 

   //:END
   //:CreateViewFromView( AD_BaseCtl, AD_Base )
   CreateViewFromView( &AD_BaseCtl, AD_Base );
   //:NAME VIEW AD_BaseCtl "AD_BaseCtl"
   SetNameForView( AD_BaseCtl, "AD_BaseCtl", 0, zLEVEL_TASK );

   //:// Special Group Processing based on Type as follows:
   //:// UpdateGroup - We are rebuilding controls for an existing Group, so delete current CtrlCtrl entries.
   //:// NewGroup - We are building controls for a new Group, so copy the Group from one in AD_Base.
   //:// MultiGroup - We are building controls for a Group as part of multiple Groups, so the current Group is already set up correctly.
   //:// SubPageGroup - This is the first group of a SubPage and is already set up correctoy.
   //:IF szType = "UpdateGroup"
   if ( ZeidonStringCompare( szType, 1, 0, "UpdateGroup", 1, 0, 21 ) == 0 )
   { 
      //:// We are modifying an existing Group, so delete all current entries.
      //:FOR EACH TZCONTROL.CtrlCtrl 
      RESULT = SetCursorFirstEntity( TZCONTROL, "CtrlCtrl", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:DELETE ENTITY TZCONTROL.CtrlCtrl NONE  
         RESULT = DeleteEntity( TZCONTROL, "CtrlCtrl", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZCONTROL, "CtrlCtrl", "" );
      } 

      //:END
      //:// The Group is class card-body for Bootstrap.
      //:TZCONTROL.Control.CSS_Class = "card-body"
      SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "card-body" );

      //:ELSE
   } 
   else
   { 
      //:IF szType = "NewGroup"
      if ( ZeidonStringCompare( szType, 1, 0, "NewGroup", 1, 0, 21 ) == 0 )
      { 
         //:// Replace the current Group with the base Detail Group from AD_Base.  
         //:DELETE ENTITY TZCONTROL.Control 
         RESULT = DeleteEntity( TZCONTROL, "Control", zPOS_NEXT );
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupDetailUpdate" 
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupDetailUpdate", "" );
         //:nRC = CloneSimpleControl( TZADWWKO, AD_BaseCtl, TZCONTROL )
         nRC = oTZADWWKO_CloneSimpleControl( TZADWWKO, AD_BaseCtl, TZCONTROL );
      } 

      //:END
   } 

   //:END

   //:// Make sure that Registered View exists in Dialog.
   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", TZADWWKO, zLEVEL_TASK );
   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = UpdateLOD.LOD.Name 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD", "Name" );
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szTempString_0, "" );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:// Activate the Registered View and include it in the Dialog. It will have been created when the object was created.
      //:nRC = ActivateMetaOI_ByName( TZADWWKO, TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, TZADWWKO.W_MetaDefUpdateObject.Name, 0 )
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), TZADWWKO, "W_MetaDefUpdateObject", "Name" );
      nRC = ActivateMetaOI_ByName( TZADWWKO, &TZWDVORO, 0, zSOURCE_VOR_META, zSINGLE, szTempString_0, 0 );
      //:NAME VIEW TZWDVORO "UpdateObjectViewRef"
      SetNameForView( TZWDVORO, "UpdateObjectViewRef", 0, zLEVEL_TASK );
      //:INCLUDE TZWINDOW.ViewObjRef FROM TZWDVORO.ViewObjRef 
      RESULT = IncludeSubobjectFromSubobject( TZWINDOW, "ViewObjRef", TZWDVORO, "ViewObjRef", zPOS_AFTER );
      //:DropObjectInstance( TZWDVORO )
      DropObjectInstance( TZWDVORO );
   } 

   //:END

   //:// The remaining formatting depends on whether the request is for formatting by Row or Group.
   //:SET CURSOR FIRST TZADWWKO.DetailMappingLOD_Attribute WHERE TZADWWKO.DetailMappingLOD_Attribute.GroupRow != ""
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow", "" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      } 

   } 

   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// Type is Row.
      //:szGroupOrRowType = "R"
      ZeidonStringCopy( szGroupOrRowType, 1, 0, "R", 1, 0, 2 );
      //:GenDetailGrpBootstrpR( TZADWWKO, TZCONTROL, UpdateLOD, szType )
      oTZADWWKO_GenDetailGrpBootstrpR( TZADWWKO, TZCONTROL, UpdateLOD, szType );
      //:ELSE
   } 
   else
   { 
      //:// Type is Group.
      //:szGroupOrRowType = "G"
      ZeidonStringCopy( szGroupOrRowType, 1, 0, "G", 1, 0, 2 );
      //:GenDetailGrpBootstrpG( TZADWWKO, TZCONTROL, UpdateLOD, szType )
      oTZADWWKO_GenDetailGrpBootstrpG( TZADWWKO, TZCONTROL, UpdateLOD, szType );
   } 

   //:END

   //:// For the SubPageGroup, convert the group title first using SubPage as the ConvertHeaderText operation uses regular Title value.
   //:// When ConvertHeaderText is called below, the Title will already be modified.
   //:IF szType = "SubPageGroup"
   if ( ZeidonStringCompare( szType, 1, 0, "SubPageGroup", 1, 0, 21 ) == 0 )
   { 
      //:nRC = PositionOnControlByTag( TZADWWKO, TZCONTROLB, TZCURWND, "txt_Section" )
      nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCONTROLB, TZCURWND, "txt_Section" );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:NAME VIEW TZCONTROLB "TZCONTROLSUB"
         SetNameForView( TZCONTROLB, "TZCONTROLSUB", 0, zLEVEL_TASK );
         //:szControlText = TZCONTROLB.Control.Text 
         GetVariableFromAttribute( szControlText, 0, 'S', 257, TZCONTROLB, "Control", "Text", "", 0 );
         //:szAD_AreaTitle = TZADWWKO.EntitySubGroup.SubPageGroupAreaTitle 
         GetVariableFromAttribute( szAD_AreaTitle, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "SubPageGroupAreaTitle", "", 0 );
         //:zSearchAndReplace( szControlText, 256, "_SectionTitle", szAD_AreaTitle )
         zSearchAndReplace( szControlText, 256, "_SectionTitle", szAD_AreaTitle );
         //:TZCONTROLB.Control.Text = szControlText
         SetAttributeFromString( TZCONTROLB, "Control", "Text", szControlText );
         //:DropView( TZCONTROLB )
         DropView( TZCONTROLB );
      } 

      //:END
   } 

   //:END

   //:// Convert header text if this is a MultiGroup
   //:IF szType = "MultiGroup" OR szType = "SubPageGroup"
   if ( ZeidonStringCompare( szType, 1, 0, "MultiGroup", 1, 0, 21 ) == 0 || ZeidonStringCompare( szType, 1, 0, "SubPageGroup", 1, 0, 21 ) == 0 )
   { 
      //:ConvertHeaderText( TZADWWKO, TZCONTROL )
      oTZADWWKO_ConvertHeaderText( TZADWWKO, TZCONTROL );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenDetailGrpBootstrpG( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                       VIEW UpdateLOD BASED ON LOD TZZOLODO,
//:                       STRING ( 20 ) szType )

//:   VIEW TZPESRCO   REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenDetailGrpBootstrpG( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     UpdateLOD,
                                 zPCHAR    szType )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOW   REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW AD_Base    BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW TZWDVORO   BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:STRING ( 50 ) szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szPromptControlName
   zCHAR     szPromptControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szDataControlName
   zCHAR     szDataControlName[ 51 ] = { 0 }; 
   //:STRING ( 10 ) szCurrentGroup
   zCHAR     szCurrentGroup[ 11 ] = { 0 }; 
   //:STRING ( 10 ) szCurrentRow
   zCHAR     szCurrentRow[ 11 ] = { 0 }; 
   //:STRING ( 5 )  szGroupWidth
   zCHAR     szGroupWidth[ 6 ] = { 0 }; 
   //:INTEGER CurrentGroup
   zLONG     CurrentGroup = 0; 
   //:INTEGER HighestGroup
   zLONG     HighestGroup = 0; 
   //:INTEGER GroupWidth
   zLONG     GroupWidth = 0; 
   //:INTEGER GroupWidthPixels
   zLONG     GroupWidthPixels = 0; 
   //:INTEGER GroupHeight
   zLONG     GroupHeight = 0; 
   //:INTEGER MaxGroupHeight
   zLONG     MaxGroupHeight = 0; 
   //:INTEGER CurrentRow
   zLONG     CurrentRow = 0; 
   //:INTEGER CurrentRowPos
   zLONG     CurrentRowPos = 0; 
   //:INTEGER PromptPosX
   zLONG     PromptPosX = 0; 
   //:INTEGER PromptWidth
   zLONG     PromptWidth = 0; 
   //:INTEGER DataPosX
   zLONG     DataPosX = 0; 
   //:INTEGER DataWidth
   zLONG     DataWidth = 0; 
   //:INTEGER LastPosition
   zLONG     LastPosition = 0; 
   //:INTEGER GroupSizeY
   zLONG     GroupSizeY = 0; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_3[ 255 ]; 
   zCHAR     szTempString_4[ 255 ]; 
   zCHAR     szTempString_5[ 255 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", TZADWWKO, zLEVEL_TASK );

   //:// Generate a Detail Group of Prompt / Mapping Control for Bootstrap from TZADWWKO.DetailMappingLOD_Attribute entries..
   //:// We will effectively loop through all uniques rows and then the Prompt/Mapping Control pairs within them.
   //:// Note that it is currently not valid to have more than 4 Group values, which should have been validated earlier.

   //:// If the GroupNumbers are empty, default them all to 1.
   //:SET CURSOR FIRST TZADWWKO.DetailMappingLOD_Attribute WHERE TZADWWKO.DetailMappingLOD_Attribute.GroupNumber = ""
   RESULT = SetCursorFirstEntityByString( TZADWWKO, "DetailMappingLOD_Attribute", "GroupNumber", "", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// At least one entry is null, so make them all 1.
      //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:TZADWWKO.DetailMappingLOD_Attribute.GroupNumber = 1
         SetAttributeFromInteger( TZADWWKO, "DetailMappingLOD_Attribute", "GroupNumber", 1 );
         RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      } 

      //:END
   } 

   //:END 

   //:// Determine highest GroupNumber.
   //:HighestGroup = 0
   HighestGroup = 0;
   //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADWWKO.DetailMappingLOD_Attribute.GroupNumber > HighestGroup
      if ( CompareAttributeToInteger( TZADWWKO, "DetailMappingLOD_Attribute", "GroupNumber", HighestGroup ) > 0 )
      { 
         //:HighestGroup = TZADWWKO.DetailMappingLOD_Attribute.GroupNumber 
         GetIntegerFromAttribute( &HighestGroup, TZADWWKO, "DetailMappingLOD_Attribute", "GroupNumber" );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      //:END
   } 

   //:END

   //:// Determine CSS_Class "col-x" value based on GroupNumber, which is full Bootstrap width (12) / HighestGroup.
   //:GroupWidth = 12 / HighestGroup
   GroupWidth = 12 / HighestGroup;
   //:zIntegerToString( szGroupWidth, 5, GroupWidth )
   zIntegerToString( szGroupWidth, 5, GroupWidth );

   //:// The Group Width in pixels is GroupWidth of Bootstrap * 40
   //:GroupWidthPixels = GroupWidth * 40 + 6
   GroupWidthPixels = GroupWidth * 40 + 6;

   //:// Create a Bootstrap Groupbox entry for each Group Number that will hold the Prompt/Data control pairs..
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:CurrentGroup = 0
   CurrentGroup = 0;
   //:LastPosition = 4
   LastPosition = 4;
   //:LOOP WHILE CurrentGroup < HighestGroup
   while ( CurrentGroup < HighestGroup )
   { 
      //:CurrentGroup = CurrentGroup + 1
      CurrentGroup = CurrentGroup + 1;
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "GroupBox"
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "GroupBox", "" );
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
      //:zIntegerToString( szCurrentGroup, 5, CurrentGroup )
      zIntegerToString( szCurrentGroup, 5, CurrentGroup );
      //:TZCONTROL.Control.Tag       = "Group" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentGroup 
      GetVariableFromAttribute( szTempString_1, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      ZeidonStringCopy( szTempString_0, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szCurrentGroup, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
      //:TZCONTROL.Control.CSS_Class = "col-" + szGroupWidth
      ZeidonStringCopy( szTempString_2, 1, 0, "col-", 1, 0, 255 );
      ZeidonStringConcat( szTempString_2, 1, 0, szGroupWidth, 1, 0, 255 );
      SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", szTempString_2 );
      //:TZCONTROL.Control.PSDLG_Y   = 10    // All Groups start at the same Y value.
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 10 );
      //:TZCONTROL.Control.SZDLG_X = GroupWidthPixels
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", GroupWidthPixels );
      //:TZCONTROL.Control.PSDLG_X = LastPosition
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", LastPosition );
      //:LastPosition = LastPosition + GroupWidthPixels
      LastPosition = LastPosition + GroupWidthPixels;
   } 

   //:END

   //:// Loop through all DetailMappingLOD_Attribute entries creating Bootstrap Prompt/Data control pairs inside the
   //:// associated Groupbox entry.
   //:// The Prompt and Data widths will be a constant and will be a little less than half the Groupbox width.
   //:// Their positions will be related constants.
   //:PromptPosX  = 6
   PromptPosX = 6;
   //:PromptWidth = (GroupWidthPixels / 3) - 18
   PromptWidth = ( GroupWidthPixels / 3 ) - 18;
   //:DataWidth   = ( PromptWidth * 3 ) / 2   // Data width will be half again as much as Prompt width. This helps with MLEdit Controls.
   DataWidth = ( PromptWidth * 3 ) / 2;
   //:DataPosX    = 6 + PromptWidth + 6
   DataPosX = 6 + PromptWidth + 6;

   //:CurrentGroup = 0
   CurrentGroup = 0;
   //:SET CURSOR FIRST TZCONTROL.Control    // Position on first Groupbox.
   RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
   //:LOOP WHILE CurrentGroup < HighestGroup
   while ( CurrentGroup < HighestGroup )
   { 
      //:CurrentGroup   = CurrentGroup + 1
      CurrentGroup = CurrentGroup + 1;
      //:zIntegerToString( szCurrentGroup, 5, CurrentGroup )
      zIntegerToString( szCurrentGroup, 5, CurrentGroup );
      //:CurrentRow     = 0
      CurrentRow = 0;
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:// Each time the CurrentGroup changes, create the surrounding Groupbox
         //:IF TZADWWKO.DetailMappingLOD_Attribute.GroupNumber = CurrentGroup
         if ( CompareAttributeToInteger( TZADWWKO, "DetailMappingLOD_Attribute", "GroupNumber", CurrentGroup ) == 0 )
         { 
            //:// The entry is in the Current Group, so add the Prompt/Data pair.
            //:CurrentRow = CurrentRow + 1    // Determine the row we're formatting.
            CurrentRow = CurrentRow + 1;

            //:// If this is any Row but the 1st, the position depends on the Y position and size of the previous Row.
            //:IF CurrentRow = 1
            if ( CurrentRow == 1 )
            { 
               //:CurrentRowPos = 5
               CurrentRowPos = 5;
               //:ELSE
            } 
            else
            { 
               //:// Compute value from the current Control.
               //:CurrentRowPos = TZCONTROL.Control.PSDLG_Y +TZCONTROL.Control.SZDLG_Y + 1
               GetIntegerFromAttribute( &lTempInteger_0, TZCONTROL, "Control", "PSDLG_Y" );
               GetIntegerFromAttribute( &lTempInteger_1, TZCONTROL, "Control", "SZDLG_Y" );
               CurrentRowPos = lTempInteger_0 + lTempInteger_1 + 1;
            } 

            //:END

            //:// Generate Prompt/Detail Control Names 
            //:zIntegerToString( szCurrentRow, 5, CurrentRow )
            zIntegerToString( szCurrentRow, 5, CurrentRow );
            //:szPromptControlName = "P_" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentGroup + szCurrentRow 
            GetVariableFromAttribute( szTempString_3, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
            ZeidonStringCopy( szPromptControlName, 1, 0, "P_", 1, 0, 51 );
            ZeidonStringConcat( szPromptControlName, 1, 0, szTempString_3, 1, 0, 51 );
            ZeidonStringConcat( szPromptControlName, 1, 0, szCurrentGroup, 1, 0, 51 );
            ZeidonStringConcat( szPromptControlName, 1, 0, szCurrentRow, 1, 0, 51 );
            //:szDataControlName   = "M_" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentGroup + szCurrentRow
            GetVariableFromAttribute( szTempString_4, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
            ZeidonStringCopy( szDataControlName, 1, 0, "M_", 1, 0, 51 );
            ZeidonStringConcat( szDataControlName, 1, 0, szTempString_4, 1, 0, 51 );
            ZeidonStringConcat( szDataControlName, 1, 0, szCurrentGroup, 1, 0, 51 );
            ZeidonStringConcat( szDataControlName, 1, 0, szCurrentRow, 1, 0, 51 );

            //:// Create Prompt 
            //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
            CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
            //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "Text"
            RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "Text", "" );
            //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
            //:TZCONTROL.Control.Text              = TZADWWKO.DetailMappingLOD_Attribute.PromptValue 
            SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZADWWKO, "DetailMappingLOD_Attribute", "PromptValue" );
            //:TZCONTROL.Control.Tag               = szPromptControlName
            SetAttributeFromString( TZCONTROL, "Control", "Tag", szPromptControlName );
            //:TZCONTROL.Control.CSS_Class         = "input-group" 
            SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "input-group" );
            //:TZCONTROL.Control.WebHTML5Attribute = "style="+ QUOTES + "min-width: 112px"+ QUOTES
            ZeidonStringCopy( szTempString_5, 1, 0, "style=", 1, 0, 255 );
            ZeidonStringConcat( szTempString_5, 1, 0, QUOTES, 1, 0, 255 );
            ZeidonStringConcat( szTempString_5, 1, 0, "min-width: 112px", 1, 0, 255 );
            ZeidonStringConcat( szTempString_5, 1, 0, QUOTES, 1, 0, 255 );
            SetAttributeFromString( TZCONTROL, "Control", "WebHTML5Attribute", szTempString_5 );
            //:TZCONTROL.Control.WebCtrlLabelLink  = szDataControlName
            SetAttributeFromString( TZCONTROL, "Control", "WebCtrlLabelLink", szDataControlName );
            //:TZCONTROL.Control.PSDLG_X           = 5
            SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", 5 );
            //:TZCONTROL.Control.PSDLG_Y           = CurrentRowPos
            SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", CurrentRowPos );
            //:TZCONTROL.Control.SZDLG_X           = PromptWidth
            SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", PromptWidth );
            //:TZCONTROL.Control.SZDLG_Y           = 10
            SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 10 );

            //:// Create Detail entry 
            //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
            CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
            //:// Control Type depends on DetailMappingLOD_Attribute setting.
            //:// Also, in Bootstrap a Calendar is treated as an EditBox with special CSS_Class & HTML5 values. ????
            //:szControlType = TZADWWKO.DetailMappingLOD_Attribute.ControlType 
            GetVariableFromAttribute( szControlType, 0, 'S', 51, TZADWWKO, "DetailMappingLOD_Attribute", "ControlType", "", 0 );
            //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = szControlType
            RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szControlType, "" );

            //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
            //:TZCONTROL.Control.Tag     = szDataControlName
            SetAttributeFromString( TZCONTROL, "Control", "Tag", szDataControlName );
            //:TZCONTROL.Control.PSDLG_X = DataPosX
            SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", DataPosX );
            //:TZCONTROL.Control.PSDLG_Y = CurrentRowPos
            SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", CurrentRowPos );
            //:TZCONTROL.Control.SZDLG_X = DataWidth
            SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", DataWidth );
            //:TZCONTROL.Control.SZDLG_Y = 10
            SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 10 );

            //:// If the Control is an MLEdit, increase both X and Y size of the Control and the Pair Groupbox.
            //:IF szControlType = "MLEdit"
            if ( ZeidonStringCompare( szControlType, 1, 0, "MLEdit", 1, 0, 51 ) == 0 )
            { 
               //:TZCONTROL.Control.SZDLG_X   = 140        // Increase Control X size by 70.
               SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 140 );
               //:TZCONTROL.Control.SZDLG_Y   = 22         // Increase Control Y size by 11.
               SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 22 );
               //:TZCONTROL.Control.CSS_Class = "col-12"   // Force Control onto next whole line.
               SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "col-12" );
            } 

            //:END

            //:// If the Control is a CheckBox, set the CtrlBOI value from the AD_Base template.
            //:IF szControlType = "CheckBox"
            if ( ZeidonStringCompare( szControlType, 1, 0, "CheckBox", 1, 0, 51 ) == 0 )
            { 
               //:GET VIEW AD_Base NAMED "AD_Base"
               RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
               //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ControlTemplates"
               RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ControlTemplates", "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:IssueError( TZADWWKO,0,0, "AD_Base Window 'ControlTemplates' is missing." )
                  IssueError( TZADWWKO, 0, 0, "AD_Base Window 'ControlTemplates' is missing." );
                  //:RETURN 2
                  return( 2 );
               } 

               //:END
               //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "CheckBox"
               RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "CheckBox", "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:IssueError( TZADWWKO,0,0, "AD_Base 'CheckBox' template is missing." )
                  IssueError( TZADWWKO, 0, 0, "AD_Base 'CheckBox' template is missing." );
                  //:RETURN 2
                  return( 2 );
               } 

               //:END
               //:TZCONTROL.Control.CtrlBOI              = AD_Base.CtrlCtrl.CtrlBOI 
               SetAttributeFromAttribute( TZCONTROL, "Control", "CtrlBOI", AD_Base, "CtrlCtrl", "CtrlBOI" );
               //:TZCONTROL.Control.Subtype              = AD_Base.CtrlCtrl.Subtype 
               SetAttributeFromAttribute( TZCONTROL, "Control", "Subtype", AD_Base, "CtrlCtrl", "Subtype" );
               //:TZCONTROL.Control.ExtendedStyle        = AD_Base.CtrlCtrl.ExtendedStyle 
               SetAttributeFromAttribute( TZCONTROL, "Control", "ExtendedStyle", AD_Base, "CtrlCtrl", "ExtendedStyle" );
               //:TZCONTROL.Control.RadioOrCheckboxValue = AD_Base.CtrlCtrl.RadioOrCheckboxValue 
               SetAttributeFromAttribute( TZCONTROL, "Control", "RadioOrCheckboxValue", AD_Base, "CtrlCtrl", "RadioOrCheckboxValue" );
            } 

            //:END

            //:// If the Control is a Calendar, use parameters from the Calendar definition, "CurrentCalendar", from  AD_Base ControlTemplates.
            //:IF szControlType = "Calendar"
            if ( ZeidonStringCompare( szControlType, 1, 0, "Calendar", 1, 0, 51 ) == 0 )
            { 
               //:GET VIEW AD_Base NAMED "AD_Base"
               RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
               //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ControlTemplates"
               RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ControlTemplates", "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:IssueError( TZADWWKO,0,0, "AD_Base Window 'ControlTemplates' is missing." )
                  IssueError( TZADWWKO, 0, 0, "AD_Base Window 'ControlTemplates' is missing." );
                  //:RETURN 2
                  return( 2 );
               } 

               //:END
               //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "CurrentCalendar"
               RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "CurrentCalendar", "" );
               //:IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //:IssueError( TZADWWKO,0,0, "AD_Base 'CurrentCalendar' template is missing." )
                  IssueError( TZADWWKO, 0, 0, "AD_Base 'CurrentCalendar' template is missing." );
                  //:RETURN 2
                  return( 2 );
               } 

               //:END
               //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
               SetViewToSubobject( AD_Base, "CtrlCtrl" );
               //:IF TZCONTROL.ControlDef.Tag != AD_Base.ControlDef.Tag
               if ( CompareAttributeToAttribute( TZCONTROL, "ControlDef", "Tag", AD_Base, "ControlDef", "Tag" ) != 0 )
               { 
                  //:EXCLUDE TZCONTROL.ControlDef NONE
                  RESULT = ExcludeEntity( TZCONTROL, "ControlDef", zREPOS_NONE );
                  //:INCLUDE TZCONTROL.ControlDef FROM AD_Base.ControlDef
                  RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", AD_Base, "ControlDef", zPOS_AFTER );
               } 

               //:END
               //:TZCONTROL.Control.ExtendedStyle     = AD_Base.Control.ExtendedStyle 
               SetAttributeFromAttribute( TZCONTROL, "Control", "ExtendedStyle", AD_Base, "Control", "ExtendedStyle" );
               //:TZCONTROL.Control.CSS_Class         = AD_Base.Control.CSS_Class 
               SetAttributeFromAttribute( TZCONTROL, "Control", "CSS_Class", AD_Base, "Control", "CSS_Class" );
               //:TZCONTROL.Control.WebHTML5Attribute = AD_Base.Control.WebHTML5Attribute 
               SetAttributeFromAttribute( TZCONTROL, "Control", "WebHTML5Attribute", AD_Base, "Control", "WebHTML5Attribute" );
               //:ResetViewFromSubobject( AD_Base )
               ResetViewFromSubobject( AD_Base );
            } 

            //:END

            //:// Create Mapping Values.
            //:CreateMetaEntity( TZADWWKO, TZCONTROL, "CtrlMap", zPOS_AFTER )  
            CreateMetaEntity( TZADWWKO, TZCONTROL, "CtrlMap", zPOS_AFTER );
            //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOW.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );
            //:SET CURSOR FIRST UpdateLOD.LOD_Entity WHERE UpdateLOD.LOD_Entity.Name   = TZADWWKO.DetailMappingLOD_Entity.Name 
            GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), TZADWWKO, "DetailMappingLOD_Entity", "Name" );
            RESULT = SetCursorFirstEntityByString( UpdateLOD, "LOD_Entity", "Name", szTempString_6, "" );
            //:SET CURSOR FIRST UpdateLOD.ER_Attribute WITHIN UpdateLOD.LOD_Entity 
            //:           WHERE UpdateLOD.ER_Attribute.Name = TZADWWKO.DetailMappingER_Attribute.Name 
            GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), TZADWWKO, "DetailMappingER_Attribute", "Name" );
            RESULT = SetCursorFirstEntityByString( UpdateLOD, "ER_Attribute", "Name", szTempString_6, "LOD_Entity" );
            //:IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", UpdateLOD, "LOD_Attribute", zPOS_AFTER )
            IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", UpdateLOD, "LOD_Attribute", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
         //:END
      } 

      //:END

      //:// The Groupbox height is the position of the last control pair + their height plus a margin.
      //:GroupHeight = TZCONTROL.Control.PSDLG_Y + TZCONTROL.Control.SZDLG_Y + 10
      GetIntegerFromAttribute( &lTempInteger_2, TZCONTROL, "Control", "PSDLG_Y" );
      GetIntegerFromAttribute( &lTempInteger_3, TZCONTROL, "Control", "SZDLG_Y" );
      GroupHeight = lTempInteger_2 + lTempInteger_3 + 10;
      //:ResetViewFromSubobject( TZCONTROL )
      ResetViewFromSubobject( TZCONTROL );
      //:TZCONTROL.Control.SZDLG_Y = GroupHeight
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", GroupHeight );
      //:SET CURSOR NEXT TZCONTROL.Control    // Position on next Groupbox 
      RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
   } 

   //:END

   //:// Now set all Groupbox entries to the same max height.
   //:MaxGroupHeight = 0
   MaxGroupHeight = 0;
   //:FOR EACH TZCONTROL.Control 
   RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZCONTROL.Control.SZDLG_Y > MaxGroupHeight
      if ( CompareAttributeToInteger( TZCONTROL, "Control", "SZDLG_Y", MaxGroupHeight ) > 0 )
      { 
         //:MaxGroupHeight = TZCONTROL.Control.SZDLG_Y 
         GetIntegerFromAttribute( &MaxGroupHeight, TZCONTROL, "Control", "SZDLG_Y" );
      } 

      RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
      //:END
   } 

   //:END
   //:FOR EACH TZCONTROL.Control 
   RESULT = SetCursorFirstEntity( TZCONTROL, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:TZCONTROL.Control.SZDLG_Y = MaxGroupHeight 
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", MaxGroupHeight );
      RESULT = SetCursorNextEntity( TZCONTROL, "Control", "" );
   } 

   //:END

   //:// Compute the outer Group Y size from the last Control.
   //:GroupSizeY = TZCONTROL.Control.PSDLG_Y + TZCONTROL.Control.SZDLG_Y + 6 
   GetIntegerFromAttribute( &lTempInteger_4, TZCONTROL, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_5, TZCONTROL, "Control", "SZDLG_Y" );
   GroupSizeY = lTempInteger_4 + lTempInteger_5 + 6;

   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   //:TZCONTROL.Control.SZDLG_Y = GroupSizeY
   SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", GroupSizeY );
   //:TZCONTROL.Control.CSS_Class = "card-body row" // The outer Group needs to have "row" added.
   SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "card-body row" );

   //:nRC = ResetViewFromSubobject( TZCONTROL )     // Reset back to the GroupBox that started the Autodesign.
   nRC = ResetViewFromSubobject( TZCONTROL );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenDetailGrpBootstrpR( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                       VIEW UpdateLOD BASED ON LOD TZZOLODO,
//:                       STRING ( 20 ) szType )

//:   VIEW TZPESRCO   REGISTERED AS TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenDetailGrpBootstrpR( zVIEW     TZADWWKO,
                                 zVIEW     TZCONTROL,
                                 zVIEW     UpdateLOD,
                                 zPCHAR    szType )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOW   REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW AD_Base    BASED ON LOD  TZWDLGSO
   zVIEW     AD_Base = 0; 
   //:VIEW TZCONTROL2 BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL2 = 0; 
   //:VIEW TZWDVORO   BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:STRING ( 50 ) szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szPromptControlName
   zCHAR     szPromptControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szDataControlName
   zCHAR     szDataControlName[ 51 ] = { 0 }; 
   //:STRING ( 10 ) szCurrentRow
   zCHAR     szCurrentRow[ 11 ] = { 0 }; 
   //:STRING ( 10 ) szCurrentPair
   zCHAR     szCurrentPair[ 11 ] = { 0 }; 
   //:INTEGER CurrentRow
   zLONG     CurrentRow = 0; 
   //:INTEGER LastRow
   zLONG     LastRow = 0; 
   //:INTEGER RowCount
   zLONG     RowCount = 0; 
   //:INTEGER DetailPairCount
   zLONG     DetailPairCount = 0; 
   //:INTEGER NewYPosition
   zLONG     NewYPosition = 0; 
   //:INTEGER NewXPosition
   zLONG     NewXPosition = 0; 
   //:INTEGER GroupSizeY
   zLONG     GroupSizeY = 0; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 255 ]; 
   zCHAR     szTempString_4[ 255 ]; 
   zCHAR     szTempString_5[ 255 ]; 
   zCHAR     szTempString_6[ 255 ]; 
   zCHAR     szTempString_7[ 255 ]; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zCHAR     szTempString_8[ 33 ]; 
   zLONG     lTempInteger_8; 
   zLONG     lTempInteger_9; 
   zLONG     lTempInteger_10; 
   zLONG     lTempInteger_11; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", TZADWWKO, zLEVEL_TASK );

   //:// Generate a Detail Group of Prompt / Mapping Control for Bootstrap from TZADWWKO.DetailMappingLOD_Attribute entries..
   //:// We will effectively loop through all uniques rows and then the Prompt/Mapping Control pairs within them.

   //:// Default any null Row values so that they appear on next row from the last.
   //:LastRow = 0
   LastRow = 0;
   //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZADWWKO.DetailMappingLOD_Attribute.GroupRow = ""
      if ( CompareAttributeToString( TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow", "" ) == 0 )
      { 
         //:LastRow = LastRow + 1
         LastRow = LastRow + 1;
         //:TZADWWKO.DetailMappingLOD_Attribute.GroupRow = LastRow
         SetAttributeFromInteger( TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow", LastRow );
         //:ELSE
      } 
      else
      { 
         //:LastRow = TZADWWKO.DetailMappingLOD_Attribute.GroupRow
         GetIntegerFromAttribute( &LastRow, TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow" );
      } 

      RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      //:END
   } 

   //:END

   //:// Loop through all DetailMappingLOD_Attribute entries creating Bootstrap Prompt/Mapping pairs for each.card-body
   //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
   SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
   //:LastRow         = 0
   LastRow = 0;
   //:RowCount        = 0
   RowCount = 0;
   //:DetailPairCount = 0
   DetailPairCount = 0;
   //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CurrentRow = TZADWWKO.DetailMappingLOD_Attribute.GroupRow 
      GetIntegerFromAttribute( &CurrentRow, TZADWWKO, "DetailMappingLOD_Attribute", "GroupRow" );
      //:IF CurrentRow != LastRow
      if ( CurrentRow != LastRow )
      { 

         //:IF CurrentRow > 1
         if ( CurrentRow > 1 )
         { 
            //:// If this is any Row after the first, we need to reset the view from the Detail Pair Group.
            //:ResetViewFromSubobject( TZCONTROL )
            ResetViewFromSubobject( TZCONTROL );
         } 

         //:END

         //:// To determine new Y position, add last Y length to last Y position plus increment.
         //:NewYPosition = TZCONTROL.Control.PSDLG_Y + TZCONTROL.Control.SZDLG_Y + 6
         GetIntegerFromAttribute( &lTempInteger_0, TZCONTROL, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lTempInteger_1, TZCONTROL, "Control", "SZDLG_Y" );
         NewYPosition = lTempInteger_0 + lTempInteger_1 + 6;
         //:NewXPosition = 3
         NewXPosition = 3;

         //:// It's a new row, so create the Row GroupBox.
         //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
         CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
         //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "GroupBox"
         RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "GroupBox", "" );
         //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
         RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
         //:zIntegerToString( szCurrentRow, 10, CurrentRow )
         zIntegerToString( szCurrentRow, 10, CurrentRow );
         //:TZCONTROL.Control.Tag       = "Row" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentRow 
         GetVariableFromAttribute( szTempString_1, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
         ZeidonStringCopy( szTempString_0, 1, 0, "Row", 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, szCurrentRow, 1, 0, 33 );
         SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
         //:TZCONTROL.Control.CSS_Class = "row"
         SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "row" );
         //:TZCONTROL.Control.PSDLG_X   = 4
         SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", 4 );
         //:TZCONTROL.Control.PSDLG_Y   = NewYPosition
         SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", NewYPosition );
         //:TZCONTROL.Control.SZDLG_X   = 480
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 480 );
         //:TZCONTROL.Control.SZDLG_Y   = 30
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 30 );
         //:LastRow = CurrentRow
         LastRow = CurrentRow;
         //:DetailPairCount = 0
         DetailPairCount = 0;

         //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )    // Step down to Pair level.
         SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      } 

      //:END
      //:DetailPairCount = DetailPairCount + 1
      DetailPairCount = DetailPairCount + 1;
      //:zIntegerToString( szCurrentPair, 10, DetailPairCount )
      zIntegerToString( szCurrentPair, 10, DetailPairCount );

      //:// Create GroupBox for Detail Pair
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "GroupBox"
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "GroupBox", "" );
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
      //:TZCONTROL.Control.Tag       = "Pair" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentRow + szCurrentPair
      GetVariableFromAttribute( szTempString_3, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      ZeidonStringCopy( szTempString_2, 1, 0, "Pair", 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szTempString_3, 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szCurrentRow, 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szCurrentPair, 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_2 );
      //:TZCONTROL.Control.CSS_Class = "col-4"
      SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "col-4" );
      //:TZCONTROL.Control.PSDLG_X = NewXPosition
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", NewXPosition );
      //:TZCONTROL.Control.PSDLG_Y = 4
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 4 );
      //:TZCONTROL.Control.SZDLG_X = 150
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 150 );
      //:TZCONTROL.Control.SZDLG_Y = 21
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 21 );
      //:SetViewToSubobject( TZCONTROL, "CtrlCtrl" )    // Step down to Detail Pair GroupBox level.
      SetViewToSubobject( TZCONTROL, "CtrlCtrl" );

      //:// Generate Prompt/Detail Control Names 
      //:zIntegerToString( szCurrentRow, 10, CurrentRow )
      zIntegerToString( szCurrentRow, 10, CurrentRow );
      //:zIntegerToString( szCurrentPair, 10, DetailPairCount )
      zIntegerToString( szCurrentPair, 10, DetailPairCount );
      //:szPromptControlName = "P_" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentRow + szCurrentPair
      GetVariableFromAttribute( szTempString_4, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      ZeidonStringCopy( szPromptControlName, 1, 0, "P_", 1, 0, 51 );
      ZeidonStringConcat( szPromptControlName, 1, 0, szTempString_4, 1, 0, 51 );
      ZeidonStringConcat( szPromptControlName, 1, 0, szCurrentRow, 1, 0, 51 );
      ZeidonStringConcat( szPromptControlName, 1, 0, szCurrentPair, 1, 0, 51 );
      //:szDataControlName   = "M_" + TZADWWKO.EntitySubGroup.GroupAreaName + szCurrentRow + szCurrentPair
      GetVariableFromAttribute( szTempString_5, 0, 'S', 255, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      ZeidonStringCopy( szDataControlName, 1, 0, "M_", 1, 0, 51 );
      ZeidonStringConcat( szDataControlName, 1, 0, szTempString_5, 1, 0, 51 );
      ZeidonStringConcat( szDataControlName, 1, 0, szCurrentRow, 1, 0, 51 );
      ZeidonStringConcat( szDataControlName, 1, 0, szCurrentPair, 1, 0, 51 );
      //:TraceLineS( "*** Prompt Group: ", TZADWWKO.EntitySubGroup.GroupAreaName )
      GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), TZADWWKO, "EntitySubGroup", "GroupAreaName" );
      TraceLineS( "*** Prompt Group: ", szTempString_6 );

      //:// Create Prompt 
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "Text"
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "Text", "" );
      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
      //:TZCONTROL.Control.Text              = TZADWWKO.DetailMappingLOD_Attribute.PromptValue 
      SetAttributeFromAttribute( TZCONTROL, "Control", "Text", TZADWWKO, "DetailMappingLOD_Attribute", "PromptValue" );
      //:TZCONTROL.Control.Tag               = szPromptControlName
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szPromptControlName );
      //:TZCONTROL.Control.CSS_Class         = "input-group" 
      SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "input-group" );
      //:TZCONTROL.Control.WebHTML5Attribute = "style="+ QUOTES + "min-width: 112px"+ QUOTES
      ZeidonStringCopy( szTempString_7, 1, 0, "style=", 1, 0, 255 );
      ZeidonStringConcat( szTempString_7, 1, 0, QUOTES, 1, 0, 255 );
      ZeidonStringConcat( szTempString_7, 1, 0, "min-width: 112px", 1, 0, 255 );
      ZeidonStringConcat( szTempString_7, 1, 0, QUOTES, 1, 0, 255 );
      SetAttributeFromString( TZCONTROL, "Control", "WebHTML5Attribute", szTempString_7 );
      //:TZCONTROL.Control.WebCtrlLabelLink  = szDataControlName
      SetAttributeFromString( TZCONTROL, "Control", "WebCtrlLabelLink", szDataControlName );
      //:TZCONTROL.Control.PSDLG_X           = 5
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", 5 );
      //:TZCONTROL.Control.PSDLG_Y           = 7
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 7 );
      //:TZCONTROL.Control.SZDLG_X           = 60
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 60 );
      //:TZCONTROL.Control.SZDLG_Y           = 11
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 11 );

      //:// Create Detail entry 
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER )
      CreateMetaEntity( TZADWWKO, TZCONTROL, "Control", zPOS_AFTER );
      //:// Control Type depends on DetailMappingLOD_Attribute setting.
      //:// Also, in Bootstrap a Calendar is treated as an EditBox with special CSS_Class & HTML5 values.
      //:szControlType = TZADWWKO.DetailMappingLOD_Attribute.ControlType 
      GetVariableFromAttribute( szControlType, 0, 'S', 51, TZADWWKO, "DetailMappingLOD_Attribute", "ControlType", "", 0 );
      //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = szControlType
      RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", szControlType, "" );

      //:INCLUDE TZCONTROL.ControlDef FROM TZPESRCO.ControlDef 
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
      //:TZCONTROL.Control.Tag     = szDataControlName
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szDataControlName );
      //:TZCONTROL.Control.PSDLG_X = 65
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_X", 65 );
      //:TZCONTROL.Control.PSDLG_Y = 7
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 7 );
      //:TZCONTROL.Control.SZDLG_X = 70
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 70 );
      //:TZCONTROL.Control.SZDLG_Y = 11
      SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 11 );

      //:// If the Control is an MLEdit, increase both X and Y size of the Control and the 2 outer Groupboxes.
      //:IF szControlType = "MLEdit"  
      if ( ZeidonStringCompare( szControlType, 1, 0, "MLEdit", 1, 0, 51 ) == 0 )
      { 
         //:TZCONTROL.Control.SZDLG_X   = 140        // Increase Control X size by 70.
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_X", 140 );
         //:TZCONTROL.Control.SZDLG_Y   = 22         // Increase Control Y size by 11.
         SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", 22 );
         //:TZCONTROL.Control.CSS_Class = "col-12"   // Force Control onto next whole line.
         SetAttributeFromString( TZCONTROL, "Control", "CSS_Class", "col-12" );
         //:CreateViewFromView( TZCONTROL2, TZCONTROL )
         CreateViewFromView( &TZCONTROL2, TZCONTROL );
         //:ResetViewFromSubobject( TZCONTROL2 )  // Back up to Prompt/Control pair.
         ResetViewFromSubobject( TZCONTROL2 );
         //:TZCONTROL2.Control.SZDLG_X = TZCONTROL2.Control.SZDLG_X + 70   // Increase Control X size by 70.
         GetIntegerFromAttribute( &lTempInteger_2, TZCONTROL2, "Control", "SZDLG_X" );
         lTempInteger_3 = lTempInteger_2 + 70;
         SetAttributeFromInteger( TZCONTROL2, "Control", "SZDLG_X", lTempInteger_3 );
         //:TZCONTROL2.Control.SZDLG_Y = TZCONTROL2.Control.SZDLG_Y + 11   // Increase Control Y size by 11.
         GetIntegerFromAttribute( &lTempInteger_4, TZCONTROL2, "Control", "SZDLG_Y" );
         lTempInteger_5 = lTempInteger_4 + 11;
         SetAttributeFromInteger( TZCONTROL2, "Control", "SZDLG_Y", lTempInteger_5 );
         //:ResetViewFromSubobject( TZCONTROL2 )  // Back up to Row Groupbox and only increase Y size.
         ResetViewFromSubobject( TZCONTROL2 );
         //:TZCONTROL2.Control.SZDLG_Y = TZCONTROL2.Control.SZDLG_Y + 11   // Increase Control Y size by 11.
         GetIntegerFromAttribute( &lTempInteger_6, TZCONTROL2, "Control", "SZDLG_Y" );
         lTempInteger_7 = lTempInteger_6 + 11;
         SetAttributeFromInteger( TZCONTROL2, "Control", "SZDLG_Y", lTempInteger_7 );
         //:DropView( TZCONTROL2 )
         DropView( TZCONTROL2 );
      } 

      //:END

      //:// If the Control is a CheckBox, use parameters from the Checkbox definition from  AD_Base ControlTemplates.
      //:IF szControlType = "CheckBox"
      if ( ZeidonStringCompare( szControlType, 1, 0, "CheckBox", 1, 0, 51 ) == 0 )
      { 
         //:GET VIEW AD_Base NAMED "AD_Base"
         RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
         //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ControlTemplates"
         RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ControlTemplates", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZADWWKO,0,0, "AD_Base Window 'ControlTemplates' is missing." )
            IssueError( TZADWWKO, 0, 0, "AD_Base Window 'ControlTemplates' is missing." );
            //:RETURN 2
            return( 2 );
         } 

         //:END
         //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "CheckBox"
         RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "CheckBox", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZADWWKO,0,0, "AD_Base 'CheckBox' template is missing." )
            IssueError( TZADWWKO, 0, 0, "AD_Base 'CheckBox' template is missing." );
            //:RETURN 2
            return( 2 );
         } 

         //:END
         //:TZCONTROL.Control.CtrlBOI              = AD_Base.CtrlCtrl.CtrlBOI 
         SetAttributeFromAttribute( TZCONTROL, "Control", "CtrlBOI", AD_Base, "CtrlCtrl", "CtrlBOI" );
         //:TZCONTROL.Control.Subtype              = AD_Base.CtrlCtrl.Subtype 
         SetAttributeFromAttribute( TZCONTROL, "Control", "Subtype", AD_Base, "CtrlCtrl", "Subtype" );
         //:TZCONTROL.Control.ExtendedStyle        = AD_Base.CtrlCtrl.ExtendedStyle 
         SetAttributeFromAttribute( TZCONTROL, "Control", "ExtendedStyle", AD_Base, "CtrlCtrl", "ExtendedStyle" );
         //:TZCONTROL.Control.RadioOrCheckboxValue = AD_Base.CtrlCtrl.RadioOrCheckboxValue 
         SetAttributeFromAttribute( TZCONTROL, "Control", "RadioOrCheckboxValue", AD_Base, "CtrlCtrl", "RadioOrCheckboxValue" );
      } 

      //:END

      //:// If the Control is a Calendar, use parameters from the Calendar definition, "CurrentCalendar", from  AD_Base ControlTemplates.
      //:IF szControlType = "Calendar"
      if ( ZeidonStringCompare( szControlType, 1, 0, "Calendar", 1, 0, 51 ) == 0 )
      { 
         //:GET VIEW AD_Base NAMED "AD_Base"
         RESULT = GetViewByName( &AD_Base, "AD_Base", TZADWWKO, zLEVEL_TASK );
         //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "ControlTemplates"
         RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "ControlTemplates", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZADWWKO,0,0, "AD_Base Window 'ControlTemplates' is missing." )
            IssueError( TZADWWKO, 0, 0, "AD_Base Window 'ControlTemplates' is missing." );
            //:RETURN 2
            return( 2 );
         } 

         //:END
         //:SET CURSOR FIRST AD_Base.CtrlCtrl WHERE AD_Base.CtrlCtrl.Tag = "CurrentCalendar"
         RESULT = SetCursorFirstEntityByString( AD_Base, "CtrlCtrl", "Tag", "CurrentCalendar", "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:IssueError( TZADWWKO,0,0, "AD_Base 'CurrentCalendar' template is missing." )
            IssueError( TZADWWKO, 0, 0, "AD_Base 'CurrentCalendar' template is missing." );
            //:RETURN 2
            return( 2 );
         } 

         //:END
         //:SetViewToSubobject( AD_Base, "CtrlCtrl" )
         SetViewToSubobject( AD_Base, "CtrlCtrl" );
         //:IF TZCONTROL.ControlDef.Tag != AD_Base.ControlDef.Tag
         if ( CompareAttributeToAttribute( TZCONTROL, "ControlDef", "Tag", AD_Base, "ControlDef", "Tag" ) != 0 )
         { 
            //:EXCLUDE TZCONTROL.ControlDef NONE
            RESULT = ExcludeEntity( TZCONTROL, "ControlDef", zREPOS_NONE );
            //:INCLUDE TZCONTROL.ControlDef FROM AD_Base.ControlDef
            RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "ControlDef", AD_Base, "ControlDef", zPOS_AFTER );
         } 

         //:END
         //:TZCONTROL.Control.ExtendedStyle     = AD_Base.Control.ExtendedStyle 
         SetAttributeFromAttribute( TZCONTROL, "Control", "ExtendedStyle", AD_Base, "Control", "ExtendedStyle" );
         //:TZCONTROL.Control.CSS_Class         = AD_Base.Control.CSS_Class 
         SetAttributeFromAttribute( TZCONTROL, "Control", "CSS_Class", AD_Base, "Control", "CSS_Class" );
         //:TZCONTROL.Control.WebHTML5Attribute = AD_Base.Control.WebHTML5Attribute 
         SetAttributeFromAttribute( TZCONTROL, "Control", "WebHTML5Attribute", AD_Base, "Control", "WebHTML5Attribute" );
         //:ResetViewFromSubobject( AD_Base )
         ResetViewFromSubobject( AD_Base );
      } 

      //:END

      //:// Create Mapping Values.
      //:CreateMetaEntity( TZADWWKO, TZCONTROL, "CtrlMap", zPOS_AFTER )  
      CreateMetaEntity( TZADWWKO, TZCONTROL, "CtrlMap", zPOS_AFTER );
      //:INCLUDE TZCONTROL.CtrlMapView FROM TZWINDOW.ViewObjRef
      RESULT = IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapView", TZWINDOW, "ViewObjRef", zPOS_AFTER );
      //:SET CURSOR FIRST UpdateLOD.LOD_Entity WHERE UpdateLOD.LOD_Entity.Name   = TZADWWKO.DetailMappingLOD_Entity.Name 
      GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), TZADWWKO, "DetailMappingLOD_Entity", "Name" );
      RESULT = SetCursorFirstEntityByString( UpdateLOD, "LOD_Entity", "Name", szTempString_8, "" );
      //:SET CURSOR FIRST UpdateLOD.ER_Attribute WITHIN UpdateLOD.LOD_Entity 
      //:                                      WHERE UpdateLOD.ER_Attribute.Name = TZADWWKO.DetailMappingER_Attribute.Name 
      GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), TZADWWKO, "DetailMappingER_Attribute", "Name" );
      RESULT = SetCursorFirstEntityByString( UpdateLOD, "ER_Attribute", "Name", szTempString_8, "LOD_Entity" );
      //:IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", UpdateLOD, "LOD_Attribute", zPOS_AFTER )
      IncludeSubobjectFromSubobject( TZCONTROL, "CtrlMapLOD_Attribute", UpdateLOD, "LOD_Attribute", zPOS_AFTER );

      //:ResetViewFromSubobject( TZCONTROL )    // Reset back to the Detail Pair GroupBox level.
      ResetViewFromSubobject( TZCONTROL );

      //:// Set up the next X starting position in case the next pair is in the same Row.
      //:NewXPosition = TZCONTROL.Control.PSDLG_X + TZCONTROL.Control.SZDLG_X + 3
      GetIntegerFromAttribute( &lTempInteger_8, TZCONTROL, "Control", "PSDLG_X" );
      GetIntegerFromAttribute( &lTempInteger_9, TZCONTROL, "Control", "SZDLG_X" );
      NewXPosition = lTempInteger_8 + lTempInteger_9 + 3;
      RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   } 

   //:END
   //:ResetViewFromSubobject( TZCONTROL )   // Reset back to Row.
   ResetViewFromSubobject( TZCONTROL );

   //:// Determine Outer Group Y size from last Row.
   //:GroupSizeY = TZCONTROL.Control.PSDLG_Y + TZCONTROL.Control.SZDLG_Y + 6  
   GetIntegerFromAttribute( &lTempInteger_10, TZCONTROL, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_11, TZCONTROL, "Control", "SZDLG_Y" );
   GroupSizeY = lTempInteger_10 + lTempInteger_11 + 6;

   //:// Reset back to "card-body" GroupBox and set size.
   //:ResetViewFromSubobject( TZCONTROL )
   ResetViewFromSubobject( TZCONTROL );
   //:TZCONTROL.Control.SZDLG_Y = GroupSizeY
   SetAttributeFromInteger( TZCONTROL, "Control", "SZDLG_Y", GroupSizeY );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenEntitySubGroup( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                   VIEW TZCONTROL BASED ON LOD TZWDLGSO,
//:                   VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                   VIEW AD_Base   BASED ON LOD TZWDLGSO,
//:                   STRING ( 50 ) szGroupType,
//:                   STRING ( 50 ) szAD_Area )

//:   VIEW TaskLPLR    REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenEntitySubGroup( zVIEW     TZADWWKO,
                             zVIEW     TZCONTROL,
                             zVIEW     TZWINDOWL,
                             zVIEW     AD_Base,
                             zPCHAR    szGroupType,
                             zPCHAR    szAD_Area )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW AD_BaseCtl  BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseCtl = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW TZCONTROL2  BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL2 = 0; 
   //:VIEW TZCtlHier   BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlHier = 0; 
   //:VIEW TZCtlTemp   BASED ON LOD  TZWDLGSO
   zVIEW     TZCtlTemp = 0; 
   //:VIEW TaskLPLR2   BASED ON LOD  TZCMLPLO
   zVIEW     TaskLPLR2 = 0; 
   //:STRING ( 50 )  szLOD_ListEntityName
   zCHAR     szLOD_ListEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupName
   zCHAR     szGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateGroupName
   zCHAR     szTemplateGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedEntityName 
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateWindow
   zCHAR     szTemplateWindow[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlName
   zCHAR     szControlName[ 51 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   //:INTEGER lInitialLevel
   zLONG     lInitialLevel = 0; 
   //:INTEGER lReturnedLevel 
   zLONG     lReturnedLevel = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_2[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Generate an Entity Subgroup based on the Group Type passed in.
   //:// The structure we are generating is assumed to be Bootstrap.

   //:IF TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Icons"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Icons" ) == 0 )
   { 
      //:szTemplateWindow = "ObjectUpdateGroupsIcons" 
      ZeidonStringCopy( szTemplateWindow, 1, 0, "ObjectUpdateGroupsIcons", 1, 0, 51 );
      //:ELSE
   } 
   else
   { 
      //:szTemplateWindow = "ObjectUpdateGroups"
      ZeidonStringCopy( szTemplateWindow, 1, 0, "ObjectUpdateGroups", 1, 0, 51 );
   } 

   //:END
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = szTemplateWindow
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", szTemplateWindow, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "The template window, " + szTemplateWindow + ", could not be found in AD_Base."
      ZeidonStringCopy( szMsg, 1, 0, "The template window, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szTemplateWindow, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ", could not be found in AD_Base.", 1, 0, 201 );
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END

   //:CreateViewFromView( AD_BaseCtl, AD_Base )
   CreateViewFromView( &AD_BaseCtl, AD_Base );
   //:NAME VIEW  AD_BaseCtl "AD_BaseCtl"
   SetNameForView( AD_BaseCtl, "AD_BaseCtl", 0, zLEVEL_TASK );

   //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
   RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", TZADWWKO, zLEVEL_TASK );

   //:IF szGroupType = "ListGroupOnly" OR 
   //:   szGroupType = "ListGroupWUpdatePage" OR 
   //:   szGroupType = "ListGroupWDetailPage" OR 
   //:   szGroupType = "ListGroupwMGP"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 ||
        ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 51 ) == 0 )
   { 

      //:// Position on correct AD_Base Group to copy.
      //:IF szGroupType = "ListGroupOnly"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) == 0 )
      { 
         //:szTemplateGroupName = "GroupListOnly"
         ZeidonStringCopy( szTemplateGroupName, 1, 0, "GroupListOnly", 1, 0, 51 );
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szTemplateGroupName
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szTemplateGroupName, "" );
         //:ELSE
      } 
      else
      { 
         //:IF szGroupType = "ListGroupWUpdatePage"
         if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 )
         { 
            //:szTemplateGroupName = "GroupUpdateList"
            ZeidonStringCopy( szTemplateGroupName, 1, 0, "GroupUpdateList", 1, 0, 51 );
            //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szTemplateGroupName
            RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szTemplateGroupName, "" );
            //:ELSE
         } 
         else
         { 
            //:IF szGroupType = "ListGroupWDetailPage"
            if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWDetailPage", 1, 0, 51 ) == 0 )
            { 
               //:szTemplateGroupName = "GroupDetailList"
               ZeidonStringCopy( szTemplateGroupName, 1, 0, "GroupDetailList", 1, 0, 51 );
               //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szTemplateGroupName
               RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szTemplateGroupName, "" );
               //:ELSE
            } 
            else
            { 
               //:IF szGroupType = "ListGroupwMGP"
               if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 51 ) == 0 )
               { 
                  //:szTemplateGroupName = "GroupMGP_List"
                  ZeidonStringCopy( szTemplateGroupName, 1, 0, "GroupMGP_List", 1, 0, 51 );
                  //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = szTemplateGroupName
                  RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", szTemplateGroupName, "" );
                  //:ELSE
               } 
               else
               { 
                  //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
                  //:    "Invalid List Type.",
                  //:    zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
                  MessageSend( TZADWWKO, "", "Autodesign Subdialog", "Invalid List Type.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                  //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
                  SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
                  //:RETURN -2
                  return( -2 );
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:szMsg = szTemplateGroupName + " Template Control doesn't exist."
         ZeidonStringCopy( szMsg, 1, 0, szTemplateGroupName, 1, 0, 201 );
         ZeidonStringConcat( szMsg, 1, 0, " Template Control doesn't exist.", 1, 0, 201 );
         //:MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// Build the Grid Control with subcontrols.

      //:// First clone basic structure at the current position.
      //:nRC = CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl )
      nRC = oTZADWWKO_CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl );

      //:// Set top Group Tag and Text.
      //:szGroupName = TZADWWKO.EntitySubGroup.GroupAreaName
      GetVariableFromAttribute( szGroupName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      //:TZCONTROL.Control.Tag  = "Group" + szGroupName + "List"    // Used for debugging.
      ZeidonStringCopy( szTempString_0, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szGroupName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, "List", 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_0 );
      //:TZCONTROL.Control.Text = ""
      SetAttributeFromString( TZCONTROL, "Control", "Text", "" );

      //:// Set Control Start Y Position to 10. That will be the correct position if this is the first Group on
      //:// the page. If it's not the first Group, the position will be reset later.
      //:TZCONTROL.Control.PSDLG_Y = 10
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 10 );

      //:IF szGroupType = "ListGroupwMGP"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwMGP", 1, 0, 51 ) == 0 )
      { 
         //:// Process the GOTO__AreaMGP Action and Operation for MGP.
         //:AddAD_ListMGP_Actions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
         oTZADWWKO_AddAD_ListMGP_Actions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );
         //:ELSE
      } 
      else
      { 
         //:// Process the Actions and Operations tied to the List Detail or Update Groups.
         //:// Don't execute for ListOnly groups, which have no Actions.
         //:IF szGroupType != "ListGroupOnly"
         if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupOnly", 1, 0, 51 ) != 0 )
         { 
            //:AddAD_ListUpdActions( TZADWWKO, TZWINDOWL, AD_Base, UpdateLOD, szGroupType )
            oTZADWWKO_AddAD_ListUpdActions( TZADWWKO, TZWINDOWL, AD_Base, UpdateLOD, szGroupType );
         } 

         //:END
      } 

      //:END

      //:// Build the list mapping entries for Grid. Note that the AddListMapping operation handles any parent Groups
      //:// that contain the Grid.
      //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity.Name
      GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
      //:GetStringFromAttribute( szViewName, UpdateLOD, "LOD", "Name" )   // View Name is same as LOD Name.
      GetStringFromAttribute( szViewName, zsizeof( szViewName ), UpdateLOD, "LOD", "Name" );
      //:AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" )
      oTZADWWKO_AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" );

      //:// Convert Header Data.
      //:ConvertHeaderText( TZADWWKO, TZCONTROL )
      oTZADWWKO_ConvertHeaderText( TZADWWKO, TZCONTROL );
   } 


   //:END

   //:// DETAIL GROUP

   //:IF szGroupType = "DetailGroup" OR szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroup", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 )
   { 

      //:// Position on correct AD_Base Group to copy.
      //:IF szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupDetailwInclude"
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupDetailwInclude", "" );
         //:ELSE
      } 
      else
      { 
         //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupDetail"
         RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupDetail", "" );
      } 

      //:END
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
         //:             "DetailGroupUpdate Control doesn't exist.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( TZADWWKO, "", "Autodesign Subdialog", "DetailGroupUpdate Control doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// Build the Detail control Group.

      //:// Because the GenDetailGrpBootstrap routine uses the DetailMapping suboject, we will copy the ESG_DetailLOD_Attribute subobject
      //:// entries to the DetailMapping suboject.
      //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:EXCLUDE TZADWWKO.DetailMappingLOD_Attribute  NONE 
         RESULT = ExcludeEntity( TZADWWKO, "DetailMappingLOD_Attribute", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      } 

      //:END
      //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:INCLUDE TZADWWKO.DetailMappingLOD_Attribute FROM TZADWWKO.ESG_DetailLOD_Attribute 
         RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zSET_ALL )  
         SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zSET_ALL );
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
      } 

      //:END

      //:// Clone Control at current position.
      //:nRC = CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl )
      nRC = oTZADWWKO_CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl );

      //:// Set top Group Tag and Text.
      //:szGroupName = TZADWWKO.EntitySubGroup.GroupAreaName
      GetVariableFromAttribute( szGroupName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      //:TZCONTROL.Control.Tag  = "Group" + szGroupName + "Detail"    // Used for debugging.
      ZeidonStringCopy( szTempString_1, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, szGroupName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, "Detail", 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_1 );
      //:TZCONTROL.Control.Text = ""
      SetAttributeFromString( TZCONTROL, "Control", "Text", "" );

      //:// Set Control Start Y Position to 10. That will be the correct position if this is the first Group on
      //:// the page. If it's not the first Group, the position will be reset later.
      //:TZCONTROL.Control.PSDLG_Y = 10
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 10 );

      //:// After the Clone, we need to step into the inner-most and last subGroup.
      //:LOOP WHILE TZCONTROL.CtrlCtrl EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
      while ( lTempInteger_0 == 0 )
      { 
         //:nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
         nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
         //:SET CURSOR LAST TZCONTROL.Control
         RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
         lTempInteger_0 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
      } 

      //:END

      //:GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "MultiGroup" )
      oTZADWWKO_GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "MultiGroup" );

      //:// If this is GroupDetailwInclude, alter the Select Action, GOTO_SelectDetail, with Area Name & current Dialog plus include Window name 
      //:// for the Include Window that will be generated later.
      //:IF szGroupType = "DetailGroupwInclude" OR szGroupType = "DetailGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 

         //:// Go to process GOTO and EXCLUDE Actions and Operations.
         //:AddAD_GoExclActions( TZADWWKO, TZWINDOWL, AD_Base )
         oTZADWWKO_AddAD_GoExclActions( TZADWWKO, TZWINDOWL, AD_Base );
      } 


      //:END
   } 

   //:END

   //:// LIST INCLUDE GROUP

   //:IF szGroupType = "ListGroupwInclude" OR szGroupType = "ListGroupwFindInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
   { 

      //:// Build Group List with transfer to Select/Include Page.
      //:SET CURSOR FIRST AD_BaseCtl.Control WHERE AD_BaseCtl.Control.Tag = "GroupIncludeList"
      RESULT = SetCursorFirstEntityByString( AD_BaseCtl, "Control", "Tag", "GroupIncludeList", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
         //:             "ListGroupWIncludePage Control doesn't exist.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( TZADWWKO, "", "Autodesign Subdialog", "ListGroupWIncludePage Control doesn't exist.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
         SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END

      //:// Clone Control at current position. Note that Clone does not copy Action Operations.
      //:nRC = CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl )
      nRC = oTZADWWKO_CloneControlAD( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl );

      //:// Set top Group Tag and Text.
      //:szGroupName = TZADWWKO.EntitySubGroup.GroupAreaName
      GetVariableFromAttribute( szGroupName, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
      //:TZCONTROL.Control.Tag  = "Group" + szGroupName + "Incl"     // Used for debugging.
      ZeidonStringCopy( szTempString_2, 1, 0, "Group", 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, szGroupName, 1, 0, 33 );
      ZeidonStringConcat( szTempString_2, 1, 0, "Incl", 1, 0, 33 );
      SetAttributeFromString( TZCONTROL, "Control", "Tag", szTempString_2 );
      //:TZCONTROL.Control.Text = ""
      SetAttributeFromString( TZCONTROL, "Control", "Text", "" );

      //:// Set Control Start Y Position to 10. That will be the correct position if this is the first Group on
      //:// the page. If it's not the first Group, the position will be reset later.
      //:TZCONTROL.Control.PSDLG_Y = 10
      SetAttributeFromInteger( TZCONTROL, "Control", "PSDLG_Y", 10 );

      //:// Process the 2 Actions tied to the New and Delete/Remove buttons.
      //:AddAD_ListInclActions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base )
      oTZADWWKO_AddAD_ListInclActions( TZADWWKO, TZCONTROL, TZWINDOWL, AD_BaseCtl, AD_Base );

      //:// Build the list mapping entries.
      //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity.Name
      GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity", "Name", "", 0 );
      //:TraceLineS( "*** Before AddListMapping List Group Include Update Page: ", szLOD_ListEntityName )
      TraceLineS( "*** Before AddListMapping List Group Include Update Page: ", szLOD_ListEntityName );
      //:GetStringFromAttribute( szViewName, UpdateLOD, "LOD", "Name" )   // View Name is same as LOD Name.
      GetStringFromAttribute( szViewName, zsizeof( szViewName ), UpdateLOD, "LOD", "Name" );
      //:AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" )
      oTZADWWKO_AddListMapping( TZADWWKO, TZCONTROL, TZWINDOWL, UpdateLOD, szLOD_ListEntityName, szViewName, "ESG_List" );

      //:// Convert Header Data.
      //:ConvertHeaderText( TZADWWKO, TZCONTROL )
      oTZADWWKO_ConvertHeaderText( TZADWWKO, TZCONTROL );

      //:nRC = ResetViewFromSubobject( TZCONTROL )    // Make sure we reset view to the top.
      nRC = ResetViewFromSubobject( TZCONTROL );
      //:LOOP WHILE nRC = 0
      while ( nRC == 0 )
      { 
         //:nRC = ResetViewFromSubobject( TZCONTROL )
         nRC = ResetViewFromSubobject( TZCONTROL );
      } 

      //:END
   } 


   //:END

   //:// DASHBOARD ENTRY TZCONTROL,

   //:IF szGroupType = "DetailGroupDBE"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupDBE", 1, 0, 51 ) == 0 )
   { 

      //:// Build Group Dashboard Entry. 
      //:// Process Mapping, Actions & Operations.
      //:AddAD_Dashbord( TZADWWKO,TZWINDOWL, TZCONTROL, AD_Base, AD_BaseCtl )
      oTZADWWKO_AddAD_Dashbord( TZADWWKO, TZWINDOWL, TZCONTROL, AD_Base, AD_BaseCtl );
   } 


   //:END

   //:DropView( AD_BaseCtl )
   DropView( AD_BaseCtl );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenIncludePage( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                VIEW AD_Base   BASED ON LOD TZWDLGSO,
//:                VIEW ViewToWindow )

//:   VIEW TaskLPLR    REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenIncludePage( zVIEW     TZADWWKO,
                          zVIEW     TZWINDOWL,
                          zVIEW     AD_Base,
                          zVIEW     ViewToWindow )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOW    REGISTERED AS TZWINDOW
   zVIEW     TZWINDOW = 0; 
   //:VIEW TZWINDOW2   BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW2 = 0; 
   //:VIEW TZWINDOW3   BASED ON LOD  TZWDLGSO
   zVIEW     TZWINDOW3 = 0; 
   //:VIEW TZCONTROL   BASED ON LOD  TZWDLGSO
   zVIEW     TZCONTROL = 0; 
   //:VIEW AD_BaseSrc  BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseSrc = 0; 
   //:VIEW AD_BaseCtl  BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseCtl = 0; 
   //:VIEW UpdateLOD   BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW SrcInclLOD  BASED ON LOD  TZZOLODO
   zVIEW     SrcInclLOD = 0; 
   //:VIEW SearchLOD   BASED ON LOD  TZZOLODO
   zVIEW     SearchLOD = 0; 
   //:VIEW TZWDVORO    BASED ON LOD  TZWDVORO
   zVIEW     TZWDVORO = 0; 
   //:VIEW AD_BaseUC   BASED ON LOD  TZWDLGSO
   zVIEW     AD_BaseUC = 0; 
   //:STRING ( 50 )  szWindowName
   zCHAR     szWindowName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupName
   zCHAR     szGroupName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Suffix
   zCHAR     szAD_Suffix[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_AreaTitle
   zCHAR     szAD_AreaTitle[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szOperationName
   zCHAR     szOperationName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReturnedObjectName
   zCHAR     szReturnedObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szLOD_ListEntityName
   zCHAR     szLOD_ListEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlTag
   zCHAR     szControlTag[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingObject
   zCHAR     szMappingObject[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingEntity
   zCHAR     szMappingEntity[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szMappingAttribute
   zCHAR     szMappingAttribute[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSearchObjectName
   zCHAR     szSearchObjectName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSourceFileName
   zCHAR     szSourceFileName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclEntS 
   zCHAR     szInclEntS[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclObjS 
   zCHAR     szInclObjS[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclEntT 
   zCHAR     szInclEntT[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclObjT
   zCHAR     szInclObjT[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szInclSuff
   zCHAR     szInclSuff[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchO 
   zCHAR     szAD_SchO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_SchV
   zCHAR     szAD_SchV[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetO 
   zCHAR     szAD_RetO[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_RetV
   zCHAR     szAD_RetV[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_UpdObj
   zCHAR     szAD_UpdObj[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szAD_ID    
   zCHAR     szAD_ID[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szTemplateWindow
   zCHAR     szTemplateWindow[ 51 ] = { 0 }; 
   //:STRING ( 6 )   szCompareOption
   zCHAR     szCompareOption[ 7 ] = { 0 }; 
   //:STRING ( 1 )   szOption
   zCHAR     szOption[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szGroupIncludeType
   zCHAR     szGroupIncludeType[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szFoundFlag
   zCHAR     szFoundFlag[ 2 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 256 ) szControlText
   zCHAR     szControlText[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSourceDirectory
   zCHAR     szSourceDirectory[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szOriginalVML
   zCHAR     szOriginalVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szVML_Statement
   zCHAR     szVML_Statement[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szFindVML
   zCHAR     szFindVML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCheckForOperation
   zCHAR     szCheckForOperation[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCompare
   zCHAR     szCompare[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCompareInit
   zCHAR     szCompareInit[ 257 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nRC2
   zLONG     nRC2 = 0; 
   //:INTEGER nNextPositionY
   zLONG     nNextPositionY = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   //:INTEGER nFileIn
   zLONG     nFileIn = 0; 
   //:INTEGER nFileOut
   zLONG     nFileOut = 0; 
   //:INTEGER nLength  
   zLONG     nLength = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );
   RESULT = GetViewByName( &TZWINDOW, "TZWINDOW", TZADWWKO, zLEVEL_TASK );

   //:// GROUP DETAIL INCLUDE PAGE

   //:// This operation builds the object include page both with Find processing and without it.

   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );

   //:CreateViewFromView( TZWINDOW2, TZWINDOWL )   // We will create TZWINDOW2 view because TZWINDOWL is somehow not holding position.
   CreateViewFromView( &TZWINDOW2, TZWINDOWL );
   //:NAME VIEW TZWINDOW2 "TZWINDOW2"
   SetNameForView( TZWINDOW2, "TZWINDOW2", 0, zLEVEL_TASK );
   //:   
   //:// First clone the GroupListInclude or GroupListFindInclude Window depending on GroupType.
   //:IF TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Icons"
   if ( CompareAttributeToString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Icons" ) == 0 )
   { 
      //:IF szGroupType = "DetailGroupwInclude" OR szGroupType = "ListGroupwInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 )
      { 
         //:szTemplateWindow = "GroupListIncludeIcons"
         ZeidonStringCopy( szTemplateWindow, 1, 0, "GroupListIncludeIcons", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szTemplateWindow = "GroupListFindIncludeIcons"
         ZeidonStringCopy( szTemplateWindow, 1, 0, "GroupListFindIncludeIcons", 1, 0, 51 );
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:IF szGroupType = "DetailGroupwInclude" OR szGroupType = "ListGroupwInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 )
      { 
         //:szTemplateWindow = "GroupListInclude" 
         ZeidonStringCopy( szTemplateWindow, 1, 0, "GroupListInclude", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szTemplateWindow = "GroupListFindInclude"
         ZeidonStringCopy( szTemplateWindow, 1, 0, "GroupListFindInclude", 1, 0, 51 );
      } 

      //:END
   } 

   //:END
   //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = szTemplateWindow
   RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", szTemplateWindow, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "The template window, " + szTemplateWindow + ", could not be found in AD_Base."
      ZeidonStringCopy( szMsg, 1, 0, "The template window, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szTemplateWindow, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ", could not be found in AD_Base.", 1, 0, 201 );
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:SET CURSOR LAST TZWINDOW2.Window  
   RESULT = SetCursorLastEntity( TZWINDOW2, "Window", "" );
   //:nRC = CloneWindow( TZWINDOW2, AD_Base, TaskLPLR, ViewToWindow )
   nRC = oTZWDLGSO_CloneWindow( TZWINDOW2, AD_Base, TaskLPLR, ViewToWindow );

   //:szWindowName = TZADWWKO.ESG_LOD_Entity.Name + "Include"
   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), TZADWWKO, "ESG_LOD_Entity", "Name" );
   ZeidonStringConcat( szWindowName, 1, 0, "Include", 1, 0, 51 );
   //:TZWINDOW2.Window.Tag     = szWindowName
   SetAttributeFromString( TZWINDOW2, "Window", "Tag", szWindowName );
   //:TZWINDOW2.Window.Caption = szWindowName
   SetAttributeFromString( TZWINDOW2, "Window", "Caption", szWindowName );

   //:// If the Group Type is for Find, build the search Group.
   //:IF szGroupType = "DetailGroupwFindInclude" OR szGroupType = "ListGroupwFindInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
   { 
      //:// Build the Search Group
      //:// This code is very similar to the code in AutoDesignFindPages.

      //:// Begin by making sure a ViewObjRef entry exists for the Search LOD.
      //:GET VIEW SearchLOD NAMED "ESGL_SearchLOD"
      RESULT = GetViewByName( &SearchLOD, "ESGL_SearchLOD", TZADWWKO, zLEVEL_TASK );
      //:szSearchObjectName = SearchLOD.LOD.Name 
      GetVariableFromAttribute( szSearchObjectName, 0, 'S', 51, SearchLOD, "LOD", "Name", "", 0 );
      //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szSearchObjectName
      RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szSearchObjectName, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
         //:AddRegisteredViewName( TZADWWKO, TZWINDOW, SearchLOD, szSearchObjectName )
         oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, SearchLOD, szSearchObjectName );
      } 

      //:END
      //:GET VIEW UpdateLOD NAMED "TZZOLODO_Update"
      RESULT = GetViewByName( &UpdateLOD, "TZZOLODO_Update", TZADWWKO, zLEVEL_TASK );
      //:DropNameForView( UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK )   // We'll temporarily drop the name because we want to reuse it.
      DropNameForView( UpdateLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
      //:NAME VIEW SearchLOD "TZZOLODO_Update"
      SetNameForView( SearchLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );

      //:// Because the GenDetailGrpBootstrap routine uses the DetailMapping... subobject, we'll copy the Search subobject to that subobject.
      //:// Also set the Area Name to "Find" as it's used in generating Control Tags.
      //://TZADWWKO.EntitySubGroup.GroupAreaName = "Find"
      //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:EXCLUDE TZADWWKO.DetailMappingLOD_Attribute  NONE 
         RESULT = ExcludeEntity( TZADWWKO, "DetailMappingLOD_Attribute", zREPOS_NONE );
         RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
      } 

      //:END
      //:FOR EACH TZADWWKO.ESG_SearchLOD_Attribute 
      RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:INCLUDE TZADWWKO.DetailMappingLOD_Attribute FROM TZADWWKO.ESG_SearchLOD_Attribute 
         RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_SearchLOD_Attribute", zPOS_AFTER );
         //:// Set work attributes.
         //:SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_SearchLOD_Attribute", zSET_NULL )  
         SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_SearchLOD_Attribute", zSET_NULL );
         RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
      } 

      //:END

      //:// Modify Header Text named "txtSearch" within Search Group by replacing "_InclEntT" string with GroupAreaTitle.
      //:nRC = PositionOnControlByTag( TZADWWKO, TZCONTROL, TZWINDOW2, "txtSearch" )
      nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCONTROL, TZWINDOW2, "txtSearch" );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:NAME VIEW TZCONTROL "TZCONTROLAD"
         SetNameForView( TZCONTROL, "TZCONTROLAD", 0, zLEVEL_TASK );
         //:szControlText = TZCONTROL.Control.Text 
         GetVariableFromAttribute( szControlText, 0, 'S', 257, TZCONTROL, "Control", "Text", "", 0 );
         //:szAD_AreaTitle = TZADWWKO.EntitySubGroup.GroupAreaTitle 
         GetVariableFromAttribute( szAD_AreaTitle, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaTitle", "", 0 );
         //:zSearchAndReplace( szControlText, 256, "_InclEntT", szAD_AreaTitle )
         zSearchAndReplace( szControlText, 256, "_InclEntT", szAD_AreaTitle );
         //:TZCONTROL.Control.Text = szControlText
         SetAttributeFromString( TZCONTROL, "Control", "Text", szControlText );
         //:DropView( TZCONTROL )
         DropView( TZCONTROL );
      } 

      //:END

      //:// Locate Group GroupBoxSearch for generating Search Fields.
      //:nRC = PositionOnControlByTag( TZADWWKO, TZCONTROL, TZWINDOW2, "GroupBoxSearch" )
      nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCONTROL, TZWINDOW2, "GroupBoxSearch" );
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:// Generate the Search Controls
         //:GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "Search Fields" )
         oTZADWWKO_GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL, "Search Fields" );
         //:DropNameForView( SearchLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK )
         DropNameForView( SearchLOD, "TZZOLODO_Update", ViewToWindow, zLEVEL_TASK );
         //:NAME VIEW UpdateLOD "TZZOLODO_Update"    // Rename the UpdateLOD aince we reused the name for the SearchLOD.
         SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
         //:DropView( TZCONTROL )
         DropView( TZCONTROL );
         //:ELSE
      } 
      else
      { 
         //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
         //:          "Include Find Search Group, GroupBoxSearch, not found in AD_Base.",
         //:          zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( TZADWWKO, "", "Autodesign Subdialog", "Include Find Search Group, GroupBoxSearch, not found in AD_Base.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:RETURN -2
         return( -2 );
      } 

      //:END
   } 

   //:   
   //:END

   //:// Build the Select List Grid,

   //:// Begin by activating the Returned LOD and making sure a ViewObjRef entry exists for it.
   //:// Note that we are assuming here that the View Name is the same as the LOD Name.

   //:szViewName = TZADWWKO.ESG_ListIncludeW_MetaDef.Name   // View Name is in ESG_ListIncludeW_MetaDef.
   GetVariableFromAttribute( szViewName, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( ViewToWindow, SrcInclLOD, 0, zREFER_LOD_META, zSINGLE, szViewName, 0 )
   nRC = ActivateMetaOI_ByName( ViewToWindow, &SrcInclLOD, 0, zREFER_LOD_META, zSINGLE, szViewName, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:szMsg = "The View Name specified, " + szViewName + ", is not a valid LOD Name."
      ZeidonStringCopy( szMsg, 1, 0, "The View Name specified, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szViewName, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ", is not a valid LOD Name.", 1, 0, 201 );
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:NAME VIEW SrcInclLOD "ESGL_IncludeLOD"
   SetNameForView( SrcInclLOD, "ESGL_IncludeLOD", 0, zLEVEL_TASK );

   //:SET CURSOR FIRST TZWINDOW.ViewObjRef WHERE TZWINDOW.ViewObjRef.Name = szViewName
   RESULT = SetCursorFirstEntityByString( TZWINDOW, "ViewObjRef", "Name", szViewName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:// The following operation creates the ViewObjRef as necessary and includes it in the Dialog.
      //:AddRegisteredViewName( TZADWWKO, TZWINDOW, SrcInclLOD, szViewName )
      oTZADWWKO_AddRegisteredViewName( TZADWWKO, TZWINDOW, SrcInclLOD, szViewName );
   } 

   //:END
   //:szLOD_ListEntityName = TZADWWKO.ESG_LOD_Entity2.Name     // List Entity Name was selected on interface.
   GetVariableFromAttribute( szLOD_ListEntityName, 0, 'S', 51, TZADWWKO, "ESG_LOD_Entity2", "Name", "", 0 );

   //:// Position on the template Grid, GridSelectList, and then build out subcontrols using AddListMapping.
   //:nRC = PositionOnControlByTag( TZADWWKO, TZCONTROL, TZWINDOW2, "GridSelectList" )
   nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCONTROL, TZWINDOW2, "GridSelectList" );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:          "Grid GridSelectList not found.",
      //:          zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "Grid GridSelectList not found.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END
   //:nRC = ResetViewFromSubobject( TZCONTROL )    // We're now on the Grid, so back up to the GroupBox.
   nRC = ResetViewFromSubobject( TZCONTROL );
   //:AddListMapping( TZADWWKO, TZCONTROL, TZWINDOW2, SrcInclLOD, szLOD_ListEntityName, szViewName, "ESG_List2" )
   oTZADWWKO_AddListMapping( TZADWWKO, TZCONTROL, TZWINDOW2, SrcInclLOD, szLOD_ListEntityName, szViewName, "ESG_List2" );

   //:DropView( TZCONTROL )
   DropView( TZCONTROL );

   //:// Modify the Section Title on the Results Group.
   //:nRC = PositionOnControlByTag( TZADWWKO, TZCONTROL, TZWINDOW2, "txtResults" )
   nRC = oTZADWWKO_PositionOnControlByTag( TZADWWKO, &TZCONTROL, TZWINDOW2, "txtResults" );
   //:IF nRC = 0
   if ( nRC == 0 )
   { 
      //:NAME VIEW TZCONTROL "TZCONTROLAD"
      SetNameForView( TZCONTROL, "TZCONTROLAD", 0, zLEVEL_TASK );
      //:szControlText = TZCONTROL.Control.Text 
      GetVariableFromAttribute( szControlText, 0, 'S', 257, TZCONTROL, "Control", "Text", "", 0 );
      //:szAD_AreaTitle = TZADWWKO.EntitySubGroup.SubPageGroupAreaTitle 
      GetVariableFromAttribute( szAD_AreaTitle, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "SubPageGroupAreaTitle", "", 0 );
      //:zSearchAndReplace( szControlText, 256, "_SectionTitle", szAD_AreaTitle )
      zSearchAndReplace( szControlText, 256, "_SectionTitle", szAD_AreaTitle );
      //:TZCONTROL.Control.Text = szControlText
      SetAttributeFromString( TZCONTROL, "Control", "Text", szControlText );
      //:DropView( TZCONTROL )
      DropView( TZCONTROL );
   } 

   //:END

   //:// Set up Operation variable values.
   //:szInclObjT = TZADWWKO.W_MetaDefUpdateObject.Name                    // Target Include Object Name is update object.
   GetVariableFromAttribute( szInclObjT, 0, 'S', 51, TZADWWKO, "W_MetaDefUpdateObject", "Name", "", 0 );
   //:szInclEntT = TZADWWKO.ESG_TargetLOD_Entity.Name                     // Target Include Entity Name is user specified
   GetVariableFromAttribute( szInclEntT, 0, 'S', 51, TZADWWKO, "ESG_TargetLOD_Entity", "Name", "", 0 );
   //:szAD_RetV  = TZADWWKO.ESG_ListIncludeW_MetaDef.Name                 // Source Include View Name is user specified 
   GetVariableFromAttribute( szAD_RetV, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "Name", "", 0 );
   //:szAD_RetO  = TZADWWKO.ESG_ListIncludeW_MetaDef.wObjectName          // Source Include Object Name was set earlier
   GetVariableFromAttribute( szAD_RetO, 0, 'S', 51, TZADWWKO, "ESG_ListIncludeW_MetaDef", "wObjectName", "", 0 );
   //:szInclEntS = TZADWWKO.ESG_SourceLOD_Entity.Name                     // Source Include Entity Name is user specified 
   GetVariableFromAttribute( szInclEntS, 0, 'S', 51, TZADWWKO, "ESG_SourceLOD_Entity", "Name", "", 0 );
   //:ReturnID_NameForEntity( TZADWWKO, szAD_ID, szInclEntS, SrcInclLOD ) // Get Identifier Name for Source Entity (We assume Source is ER Entity)
   oTZADWWKO_ReturnID_NameForEntity( TZADWWKO, szAD_ID, szInclEntS, SrcInclLOD );
   //:szInclSuff = TZADWWKO.EntitySubGroup.GroupAreaName                  // Suffix to be added to the end of each Action and Operation.
   GetVariableFromAttribute( szInclSuff, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:IF TZADWWKO.ESG_ListSearchW_MetaDef EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ESG_ListSearchW_MetaDef" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szAD_SchV = TZADWWKO.ESG_ListSearchW_MetaDef.Name                // Search View name
      GetVariableFromAttribute( szAD_SchV, 0, 'S', 51, TZADWWKO, "ESG_ListSearchW_MetaDef", "Name", "", 0 );
      //:szAD_SchO = TZADWWKO.ESG_ListSearchW_MetaDef.wObjectName         // Search Object Name was set earlier
      GetVariableFromAttribute( szAD_SchO, 0, 'S', 51, TZADWWKO, "ESG_ListSearchW_MetaDef", "wObjectName", "", 0 );
      //:ELSE
   } 
   else
   { 
      //:szAD_SchV = ""   // Search view and object weren't specified for Include page without Find.
      ZeidonStringCopy( szAD_SchV, 1, 0, "", 1, 0, 51 );
      //:szAD_SchO = ""
      ZeidonStringCopy( szAD_SchO, 1, 0, "", 1, 0, 51 );
   } 

   //:END

   //:// Alter names for all Actions to add suffix _InclEnt, which is the Include Object Name.
   //:FOR EACH TZWINDOW2.Action 
   RESULT = SetCursorFirstEntity( TZWINDOW2, "Action", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = TZWINDOW2.Action.Tag
      GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOW2, "Action", "Tag", "", 0 );
      //:zSearchAndReplace( szActionName, 50, "_InclSuff", szInclSuff )
      zSearchAndReplace( szActionName, 50, "_InclSuff", szInclSuff );
      //:TZWINDOW2.Action.Tag = szActionName 
      SetAttributeFromString( TZWINDOW2, "Action", "Tag", szActionName );
      RESULT = SetCursorNextEntity( TZWINDOW2, "Action", "" );
   } 

   //:END

   //:// Alter names for the Operations in Dialog that go with the Actions, of which there is currently only one.
   //:// Because the current Operations are under the Source entry, ObjIncl or ObjIncl2 from the CloneWindow, we will actually
   //:// create new Operations under the main Source entry and in the end delete the ObjIncl entry.
   //:SET CURSOR FIRST TZWINDOW.SourceFile  
   RESULT = SetCursorFirstEntity( TZWINDOW, "SourceFile", "" );
   //:SET CURSOR LAST TZWINDOW.Operation  
   RESULT = SetCursorLastEntity( TZWINDOW, "Operation", "" );
   //:CreateViewFromView( TZWINDOW3, TZWINDOW )
   CreateViewFromView( &TZWINDOW3, TZWINDOW );
   //:NAME VIEW TZWINDOW3 "TZWINDOW3"
   SetNameForView( TZWINDOW3, "TZWINDOW3", 0, zLEVEL_TASK );
   //:// Position on the SourceFile for the new Operations.
   //:SET CURSOR LAST TZWINDOW3.SourceFile
   RESULT = SetCursorLastEntity( TZWINDOW3, "SourceFile", "" );
   //:FOR EACH TZWINDOW3.Operation 
   RESULT = SetCursorFirstEntity( TZWINDOW3, "Operation", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szOperationName = TZWINDOW3.Operation.Name 
      GetVariableFromAttribute( szOperationName, 0, 'S', 51, TZWINDOW3, "Operation", "Name", "", 0 );
      //:zSearchAndReplace( szOperationName, 50, "_InclSuff", szInclSuff )
      zSearchAndReplace( szOperationName, 50, "_InclSuff", szInclSuff );
      //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER )
      CreateMetaEntity( TZWINDOW, TZWINDOW, "Operation", zPOS_AFTER );
      //:SetMatchingAttributesByName( TZWINDOW, "Operation", TZWINDOW3, "Operation", zSET_NULL )
      SetMatchingAttributesByName( TZWINDOW, "Operation", TZWINDOW3, "Operation", zSET_NULL );
      //:FOR EACH TZWINDOW3.Parameter 
      RESULT = SetCursorFirstEntity( TZWINDOW3, "Parameter", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER )
         CreateMetaEntity( TZWINDOW, TZWINDOW, "Parameter", zPOS_AFTER );
         //:SetMatchingAttributesByName( TZWINDOW, "Parameter", TZWINDOW3, "Parameter", zSET_NULL )
         SetMatchingAttributesByName( TZWINDOW, "Parameter", TZWINDOW3, "Parameter", zSET_NULL );
         RESULT = SetCursorNextEntity( TZWINDOW3, "Parameter", "" );
      } 

      //:END
      //:TZWINDOW.Operation.Name = szOperationName
      SetAttributeFromString( TZWINDOW, "Operation", "Name", szOperationName );
      //:SET CURSOR FIRST TZWINDOW2.Action WHERE TZWINDOW2.Action.Tag = szOperationName
      RESULT = SetCursorFirstEntityByString( TZWINDOW2, "Action", "Tag", szOperationName, "" );
      //:EXCLUDE TZWINDOW2.ActOper 
      RESULT = ExcludeEntity( TZWINDOW2, "ActOper", zREPOS_AFTER );
      //:INCLUDE TZWINDOW2.ActOper FROM TZWINDOW.Operation 
      RESULT = IncludeSubobjectFromSubobject( TZWINDOW2, "ActOper", TZWINDOW, "Operation", zPOS_AFTER );
      RESULT = SetCursorNextEntity( TZWINDOW3, "Operation", "" );
   } 

   //:END
   //:DELETE ENTITY TZWINDOW3.SourceFile     // Delete the ObjIncl Source entry.
   RESULT = DeleteEntity( TZWINDOW3, "SourceFile", zPOS_NEXT );
   //:DropView( TZWINDOW3 )
   DropView( TZWINDOW3 );

   //:// Check if the Operations have already been copied to the main Source File. If not, copy them with the corrected names.
   //:// Check to see if the Operation, "      Include_InclSuff", exists in the VML File, where _InclSuff is replaced by the specified Suffix.
   //:// Note that the Operation Name starts in column 1.
   //:IF szGroupType = "DetailGroupwFindInclude" OR szGroupType = "ListGroupwFindInclude"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
   { 
      //:szCheckForOperation = "SELECT_FindIncl" + szInclSuff
      ZeidonStringCopy( szCheckForOperation, 1, 0, "SELECT_FindIncl", 1, 0, 257 );
      ZeidonStringConcat( szCheckForOperation, 1, 0, szInclSuff, 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szCheckForOperation = "SELECT_Include" + szInclSuff
      ZeidonStringCopy( szCheckForOperation, 1, 0, "SELECT_Include", 1, 0, 257 );
      ZeidonStringConcat( szCheckForOperation, 1, 0, szInclSuff, 1, 0, 257 );
   } 

   //:END
   //:nLength = zGetStringLen( szCheckForOperation )
   nLength = zGetStringLen( szCheckForOperation );
   //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
   GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
   //:szFindVML = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
   ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
   ZeidonStringConcat( szFindVML, 1, 0, szTempString_0, 1, 0, 257 );
   ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );
   //:nFileIn   = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ )
   nFileIn = SysOpenFile( TZWINDOW, szFindVML, COREFILE_READ );
   //:nRC2 = -1
   nRC2 = -1;
   //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
   nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
   //:LOOP WHILE nRC = 1 AND nRC2 != 0
   while ( nRC == 1 && nRC2 != 0 )
   { 
      //:strncpy_s( szCompare, szVML_Statement, nLength )
      strncpy_s( szCompare, zsizeof(szCompare), zsizeof(szCompare), szVML_Statement, nLength );
      //:nRC2 = zstrcmp( szCompare, szCheckForOperation )
      nRC2 = zstrcmp( szCompare, szCheckForOperation );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
   } 

   //:END
   //:SysCloseFile( TZWINDOW, nFileIn, 0 )
   SysCloseFile( TZWINDOW, nFileIn, 0 );
   //:IF nRC2 != 0     // Operation was not found.
   if ( nRC2 != 0 )
   { 
      //:// Copy the VML from ObjIncl.VML or ObjIncl2.VML and append it to the current Find VML
      //:// Which VML code version to use depends on whether or not this is a Find subpage.
      //:IF szGroupType = "DetailGroupwFindInclude" OR szGroupType = "ListGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "DetailGroupwFindInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 
         //:szOriginalVML = "c:\lplr\AD_Base\ObjIncl.VML"
         ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjIncl.VML", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szOriginalVML = "c:\lplr\AD_Base\ObjIncl2.VML"
         ZeidonStringCopy( szOriginalVML, 1, 0, "c:\\lplr\\AD_Base\\ObjIncl2.VML", 1, 0, 257 );
      } 

      //:END
      //:szSourceDirectory = TaskLPLR.LPLR.PgmSrcDir 
      GetVariableFromAttribute( szSourceDirectory, 0, 'S', 257, TaskLPLR, "LPLR", "PgmSrcDir", "", 0 );
      //:szFindVML = szSourceDirectory + "\" + TZWINDOW.Dialog.Tag + ".VML"
      ZeidonStringCopy( szFindVML, 1, 0, szSourceDirectory, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, "\\", 1, 0, 257 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, TZWINDOW, "Dialog", "Tag", "", 0 );
      ZeidonStringConcat( szFindVML, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szFindVML, 1, 0, ".VML", 1, 0, 257 );

      //:// Set szGroupIncludeType based on szGroupType. This will be used later in the VML copy loop.
      //:IF szGroupType = "ListGroupwInclude" OR szGroupType = "ListGroupwFindInclude"
      if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 )
      { 
         //:szGroupIncludeType = "L"
         ZeidonStringCopy( szGroupIncludeType, 1, 0, "L", 1, 0, 2 );
         //:ELSE
      } 
      else
      { 
         //:szGroupIncludeType = "D"
         ZeidonStringCopy( szGroupIncludeType, 1, 0, "D", 1, 0, 2 );
      } 

      //:END

      //:// Copy the ObjIncl VML to the end of the Find VML, converting the variable characters in the process.
      //:nFileIn   = SysOpenFile( TZWINDOW, szOriginalVML, COREFILE_READ ) 
      nFileIn = SysOpenFile( TZWINDOW, szOriginalVML, COREFILE_READ );
      //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND )
      nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_APPEND );
      //:IF nFileOut < 0  
      if ( nFileOut < 0 )
      { 
         //:nFileOut  = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE )
         nFileOut = SysOpenFile( TZWINDOW, szFindVML, COREFILE_WRITE );
      } 

      //:END
      //:szCompareInit = "   // Start FindRangeInitialization"
      ZeidonStringCopy( szCompareInit, 1, 0, "   // Start FindRangeInitialization", 1, 0, 257 );
      //:nLength       = zGetStringLen( szCompareInit )
      nLength = zGetStringLen( szCompareInit );
      //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
      nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      //:LOOP WHILE nRC = 1 
      while ( nRC == 1 )
      { 

         //:// Add FindRange code if it exists in the VML Source.
         //:strncpy_s( szCompare, szVML_Statement, nLength )    // Copy the necsssary characters of the line to the compare field.
         strncpy_s( szCompare, zsizeof(szCompare), szVML_Statement, nLength );
         //:nRC2 = zstrcmp( szCompare, szCompareInit )
         nRC2 = zstrcmp( szCompare, szCompareInit );
         //:IF nRC2 = 0
         if ( nRC2 == 0 )
         { 
            //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
            SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
            //:// Add appropriate Range initialization statements.
            //:// We first need to copy 
            //:FOR TZADWWKO.FlatSelectedSearchAttribute 
            RESULT = SetCursorFirstEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:DELETE ENTITY TZADWWKO.FlatSelectedSearchAttribute NONE  
               RESULT = DeleteEntity( TZADWWKO, "FlatSelectedSearchAttribute", zREPOS_NONE );
               RESULT = SetCursorNextEntity( TZADWWKO, "FlatSelectedSearchAttribute", "" );
            } 

            //:END
            //:FOR TZADWWKO.ESG_SearchLOD_Attribute  
            RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CREATE ENTITY TZADWWKO.FlatSelectedSearchAttribute 
               RESULT = CreateEntity( TZADWWKO, "FlatSelectedSearchAttribute", zPOS_AFTER );
               //:SetMatchingAttributesByName( TZADWWKO, "FlatSelectedSearchAttribute", TZADWWKO, "ESG_SearchLOD_Attribute", zSET_ALL )
               SetMatchingAttributesByName( TZADWWKO, "FlatSelectedSearchAttribute", TZADWWKO, "ESG_SearchLOD_Attribute", zSET_ALL );
               //:TZADWWKO.FlatSelectedSearchAttribute.EntityName    = TZADWWKO.ESG_SearchLOD_Entity.Name 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "EntityName", TZADWWKO, "ESG_SearchLOD_Entity", "Name" );
               //:TZADWWKO.FlatSelectedSearchAttribute.AttributeName = TZADWWKO.ESG_SearchER_Attribute.Name 
               SetAttributeFromAttribute( TZADWWKO, "FlatSelectedSearchAttribute", "AttributeName", TZADWWKO, "ESG_SearchER_Attribute", "Name" );
               //:INCLUDE TZADWWKO.FlatSearchLOD_Attribute FROM TZADWWKO.ESG_SearchLOD_Attribute 
               RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "FlatSearchLOD_Attribute", TZADWWKO, "ESG_SearchLOD_Attribute", zPOS_AFTER );
               RESULT = SetCursorNextEntity( TZADWWKO, "ESG_SearchLOD_Attribute", "" );
            } 

            //:END
            //:AddFindRangeInitValues( TZADWWKO, TZWINDOW, nFileIn, nFileOut )
            oTZADWWKO_AddFindRangeInitValues( TZADWWKO, TZWINDOW, nFileIn, nFileOut );
            //:ELSE
         } 
         else
         { 

            //:// The SELECT_Include_InclSuff operation has 2 versions depending on whether the initial Group is Detail or List.
            //:// The 2 versions (sets of code) are identified by the characters "Start List Include" and "Start Detial Include", 
            //:// with an "Option End" statement concluding the 2 versions.
            //:// Once we encounter the first Option statement, we will do the read and write loop here until we get to the end
            //:// statement.
            //:strncpy_s( szCompareOption, szVML_Statement, 6 )
            strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
            //:nRC2 = zstrcmp( szCompareOption, "Option" )
            nRC2 = zstrcmp( szCompareOption, "Option" );
            //:IF nRC2 = 0
            if ( nRC2 == 0 )
            { 
               //:// We will handle the List and Detail options separately.
               //:IF szGroupType = "ListGroupwFindInclude" OR szGroupType = "ListGroupwInclude"
               if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwFindInclude", 1, 0, 51 ) == 0 || ZeidonStringCompare( szGroupType, 1, 0, "ListGroupwInclude", 1, 0, 51 ) == 0 )
               { 
                  //:// For List, we will copy the first set and skip the second.
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                  //:szCompareOption = ""
                  ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
                  //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
                  while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
                  { 
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT )  // Target Include Entity name, such as "CombinedMailingPerson"
                     zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT );
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS )  // Source Include Entity name, such as "Person"
                     zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS );
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT )  // TargetInclude Object name, such as "mPerson"
                     zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT );
                     //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN",  szAD_RetV )   // Returned / Source Include View name such as lPerson.
                     zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetV );
                     //:zSearchAndReplace( szVML_Statement, 256, "_AD_ID",    szAD_ID )     // Update Identifier Name for entity above.
                     zSearchAndReplace( szVML_Statement, 256, "_AD_ID", szAD_ID );
                     //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
                     SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
                     //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
                     nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                     //:strncpy_s( szCompareOption, szVML_Statement, 6 )
                     strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
                  } 

                  //:END
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                  //:// Now skip to the next Option statement.
                  //:szCompareOption = ""
                  ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
                  //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
                  while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
                  { 
                     //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
                     nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                     //:strncpy_s( szCompareOption, szVML_Statement, 6 )
                     strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
                  } 

                  //:END
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the last Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );

                  //:ELSE
               } 
               else
               { 
                  //:// For Detail, we will skip the first set and copy the second.
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                  //:szCompareOption = ""
                  ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
                  //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
                  while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
                  { 
                     //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
                     nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                     //:strncpy_s( szCompareOption, szVML_Statement, 6 )
                     strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
                  } 

                  //:END
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                  //:szCompareOption = ""
                  ZeidonStringCopy( szCompareOption, 1, 0, "", 1, 0, 7 );
                  //:LOOP WHILE nRC = 1 AND szCompareOption != "Option"
                  while ( nRC == 1 && ZeidonStringCompare( szCompareOption, 1, 0, "Option", 1, 0, 7 ) != 0 )
                  { 
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT )  // Target Include Entity name, such as "CombinedMailingPerson"
                     zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT );
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS )  // Source Include Entity name, such as "Person"
                     zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS );
                     //:zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT )  // TargetInclude Object name, such as "mPerson"
                     zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT );
                     //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN",  szAD_RetV )   // Returned / Source Include View name such as lPerson.
                     zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetV );
                     //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
                     SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
                     //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
                     nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
                     //:strncpy_s( szCompareOption, szVML_Statement, 6 )
                     strncpy_s( szCompareOption, zsizeof(szCompareOption), szVML_Statement, 6 );
                  } 

                  //:END
                  //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )  // Skip the Option statement.
                  nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
               } 


               //:END
            } 

            //:END

            //:zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT )  // Target Include Entity name, such as "CombinedMailingPerson"
            zSearchAndReplace( szVML_Statement, 256, "_InclEntT", szInclEntT );
            //:zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS )  // Source Include Entity name, such as "Person"
            zSearchAndReplace( szVML_Statement, 256, "_InclEntS", szInclEntS );
            //:zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT )  // TargetInclude Object name, such as "mPerson"
            zSearchAndReplace( szVML_Statement, 256, "_InclObjT", szInclObjT );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetN",  szAD_RetV )   // Returned / Source Include View name such as lPerson.
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetN", szAD_RetV );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_RetO",  szAD_RetO )   // Returned / Source Include object name such as lPerson.
            zSearchAndReplace( szVML_Statement, 256, "_AD_RetO", szAD_RetO );
            //:zSearchAndReplace( szVML_Statement, 256, "_InclSuff", szInclSuff )  // Operation Suffix such as "CombinedMail"
            zSearchAndReplace( szVML_Statement, 256, "_InclSuff", szInclSuff );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchN",  szAD_SchV )   // Search View Name if this is the Find option.
            zSearchAndReplace( szVML_Statement, 256, "_AD_SchN", szAD_SchV );
            //:zSearchAndReplace( szVML_Statement, 256, "_AD_SchO",  szAD_SchO )   // Search Object Name if this is the Find option.
            zSearchAndReplace( szVML_Statement, 256, "_AD_SchO", szAD_SchO );

            //:SysWriteLine( TZWINDOW, nFileOut, szVML_Statement )
            SysWriteLine( TZWINDOW, nFileOut, szVML_Statement );
         } 

         //:END
         //:nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 )
         nRC = zSysReadLine( TZWINDOW, szVML_Statement, nFileIn, 256 );
      } 

      //:   
      //:END

      //:SysCloseFile( TZWINDOW, nFileIn, 0 )
      SysCloseFile( TZWINDOW, nFileIn, 0 );
      //:SysCloseFile( TZWINDOW, nFileOut, 0 )
      SysCloseFile( TZWINDOW, nFileOut, 0 );
   } 

   //:END

   //:// Resize and reposition the Group Boxes on the Window
   //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOW2 )
   oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOW2 );
   //:// Rename some Controls to avoid duplicate Tags.
   //:SET CURSOR FIRST TZWINDOWL.Control
   RESULT = SetCursorFirstEntity( TZWINDOWL, "Control", "" );
   //:RenameAD_Controls( TZADWWKO, TZWINDOWL ) 
   oTZADWWKO_RenameAD_Controls( TZADWWKO, TZWINDOWL );
   //:DropView( TZWINDOW2 )
   DropView( TZWINDOW2 );
   return( 0 );
//    
// END 
} 


//:TRANSFORMATION OPERATION
//:GenListDetailPage( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                   VIEW TZWINDOWL BASED ON LOD TZWDLGSO,
//:                   VIEW AD_Base   BASED ON LOD TZWDLGSO,
//:                   VIEW ViewToWindow )

//:   VIEW TZCONTROL  BASED ON LOD  TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_GenListDetailPage( zVIEW     TZADWWKO,
                             zVIEW     TZWINDOWL,
                             zVIEW     AD_Base,
                             zVIEW     ViewToWindow )
{
   zVIEW     TZCONTROL = 0; 
   //:VIEW TaskLPLR   REGISTERED AS TaskLPLR
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TZWINDOWL2 BASED ON LOD TZWDLGSO
   zVIEW     TZWINDOWL2 = 0; 
   //:STRING ( 50 ) szWindowName
   zCHAR     szWindowName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szGroupType
   zCHAR     szGroupType[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szAD_Area
   zCHAR     szAD_Area[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   //:INTEGER nLevel
   zLONG     nLevel = 0; 
   zLONG     lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// GROUP LIST DETAIL PAGE

   //:CreateViewFromView( TZWINDOWL2, TZWINDOWL )
   CreateViewFromView( &TZWINDOWL2, TZWINDOWL );
   //:NAME VIEW TZWINDOWL2 "TZWINDOWL2"
   SetNameForView( TZWINDOWL2, "TZWINDOWL2", 0, zLEVEL_TASK );
   //:   
   //:// First clone the GroupListUpdate or GroupListDetail Window at the end of the Dialog.
   //:// The difference between the two is that the Update has Entity operations.
   //:szGroupType = TZADWWKO.EntitySubGroup.GroupType 
   GetVariableFromAttribute( szGroupType, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupType", "", 0 );
   //:IF szGroupType = "ListGroupWUpdatePage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 )
   { 
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "GroupListUpdate"
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "GroupListUpdate", "" );
      //:ELSE
   } 
   else
   { 
      //:SET CURSOR FIRST AD_Base.Window WHERE AD_Base.Window.Tag = "GroupListDetail"
      RESULT = SetCursorFirstEntityByString( AD_Base, "Window", "Tag", "GroupListDetail", "" );
   } 

   //:END
   //:SET CURSOR LAST TZWINDOWL2.Window  
   RESULT = SetCursorLastEntity( TZWINDOWL2, "Window", "" );
   //:nRC = CloneWindow( TZWINDOWL2, AD_Base, TaskLPLR, ViewToWindow )
   nRC = oTZWDLGSO_CloneWindow( TZWINDOWL2, AD_Base, TaskLPLR, ViewToWindow );
   //:szWindowName = TZADWWKO.EntitySubGroup.GroupAreaName + "Detail"
   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), TZADWWKO, "EntitySubGroup", "GroupAreaName" );
   ZeidonStringConcat( szWindowName, 1, 0, "Detail", 1, 0, 51 );
   //:TZWINDOWL2.Window.Tag     = szWindowName
   SetAttributeFromString( TZWINDOWL2, "Window", "Tag", szWindowName );
   //:TZWINDOWL2.Window.Caption = szWindowName
   SetAttributeFromString( TZWINDOWL2, "Window", "Caption", szWindowName );
   //:SET CURSOR FIRST TZWINDOWL.Window WHERE TZWINDOWL.Window.ZKey = TZWINDOWL2.Window.ZKey  
   GetIntegerFromAttribute( &lTempInteger_0, TZWINDOWL2, "Window", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( TZWINDOWL, "Window", "ZKey", lTempInteger_0, "" );

   //:// Delete the SourceFile entry just cloned because the Operations for this page were created during
   //:// generation of the parent List Group.
   //:DELETE ENTITY TZWINDOWL2.SourceFile  
   RESULT = DeleteEntity( TZWINDOWL2, "SourceFile", zPOS_NEXT );

   //:// Build the Detail control Group set.assuming the format is Bootstrap.
   //:// Because the GenDetailGrpBootstrap routine uses the DetailMapping suboject, we will copy the ESG_Detail subobject
   //:// entries to the DetailMapping suboject.
   //:FOR EACH TZADWWKO.DetailMappingLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:EXCLUDE TZADWWKO.DetailMappingLOD_Attribute  NONE 
      RESULT = ExcludeEntity( TZADWWKO, "DetailMappingLOD_Attribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "DetailMappingLOD_Attribute", "" );
   } 

   //:END

   //:FOR EACH TZADWWKO.ESG_DetailLOD_Attribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:INCLUDE TZADWWKO.DetailMappingLOD_Attribute FROM TZADWWKO.ESG_DetailLOD_Attribute 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zPOS_AFTER );
      //:// Set work attributes.
      //:SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zSET_NULL )  
      SetMatchingAttributesByName( TZADWWKO, "DetailMappingLOD_Attribute", TZADWWKO, "ESG_DetailLOD_Attribute", zSET_NULL );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_DetailLOD_Attribute", "" );
   } 

   //:END

   //:// Create TZCONTROL and position it on the last / innermost Group of the nested set of Groups in the window, which is where
   //:// the data Controls will be built.
   //:CreateViewFromView( TZCONTROL, TZWINDOWL2 )  
   CreateViewFromView( &TZCONTROL, TZWINDOWL2 );
   //:NAME VIEW TZCONTROL "TZCONTROLAD"
   SetNameForView( TZCONTROL, "TZCONTROLAD", 0, zLEVEL_TASK );
   //:SET CURSOR LAST TZCONTROL.Control
   RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
   //:LOOP WHILE TZCONTROL.CtrlCtrl EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   while ( lTempInteger_1 == 0 )
   { 
      //:nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" )
      nRC = SetViewToSubobject( TZCONTROL, "CtrlCtrl" );
      //:SET CURSOR LAST TZCONTROL.Control
      RESULT = SetCursorLastEntity( TZCONTROL, "Control", "" );
      lTempInteger_1 = CheckExistenceOfEntity( TZCONTROL, "CtrlCtrl" );
   } 

   //:END

   //:GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL2, "SubPageGroup" )
   oTZADWWKO_GenDetailGrpBootstrap( TZADWWKO, TZCONTROL, TZWINDOWL2, "SubPageGroup" );

   //:// Readjust the position of each GroupBox based on the size and position of the previous GroupBox.
   //:// This is a little more complex for Bootstrap, so we will call a sub operation for that case.
   //:ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL2 )
   oTZADWWKO_ResizeReposGroupBoxes( TZADWWKO, TZWINDOWL2 );

   //:// Process Actions depending on page type.
   //:szAD_Area = TZADWWKO.EntitySubGroup.GroupAreaName 
   GetVariableFromAttribute( szAD_Area, 0, 'S', 51, TZADWWKO, "EntitySubGroup", "GroupAreaName", "", 0 );
   //:IF szGroupType = "ListGroupWUpdatePage"
   if ( ZeidonStringCompare( szGroupType, 1, 0, "ListGroupWUpdatePage", 1, 0, 51 ) == 0 )
   { 
      //:// Accept and Cancel Actions.
      //:// Convert Action Names and tie to Operations of the same name.
      //:// The Operations were created when the initial List Group was built.
      //:FOR EACH TZWINDOWL2.Action 
      RESULT = SetCursorFirstEntity( TZWINDOWL2, "Action", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:szActionName = TZWINDOWL2.Action.Tag 
         GetVariableFromAttribute( szActionName, 0, 'S', 51, TZWINDOWL2, "Action", "Tag", "", 0 );
         //:zSearchAndReplace( szActionName, 256, "_AD_Area",  szAD_Area )        // Area Name
         zSearchAndReplace( szActionName, 256, "_AD_Area", szAD_Area );
         //:TZWINDOWL2.Action.Tag = szActionName
         SetAttributeFromString( TZWINDOWL2, "Action", "Tag", szActionName );
         //:SET CURSOR FIRST TZWINDOWL.Operation WHERE TZWINDOWL.Operation.Name = szActionName
         RESULT = SetCursorFirstEntityByString( TZWINDOWL, "Operation", "Name", szActionName, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:INCLUDE TZWINDOWL2.ActOper FROM TZWINDOWL.Operation 
            RESULT = IncludeSubobjectFromSubobject( TZWINDOWL2, "ActOper", TZWINDOWL, "Operation", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( TZWINDOWL2, "Action", "" );
         //:END 
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// Return Action
      //:// Simply modify Action to include Area Name.
      //:TZWINDOWL2.Action.Tag = "RETURN_" + szAD_Area
      ZeidonStringCopy( szTempString_0, 1, 0, "RETURN_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_0, 1, 0, szAD_Area, 1, 0, 33 );
      SetAttributeFromString( TZWINDOWL2, "Action", "Tag", szTempString_0 );
   } 

   //:END

   //:DropView( TZWINDOWL2 )
   DropView( TZWINDOWL2 );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:InitGridSelectAttribs( VIEW TZADWWKO   BASED ON LOD TZADWWKO,
//:                       VIEW TempCtrl   BASED ON LOD TZWDLGSO )

//:   VIEW TaskLPLR  REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_InitGridSelectAttribs( zVIEW     TZADWWKO,
                                 zVIEW     TempCtrl )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TempCtrl2 BASED ON LOD  TZWDLGSO
   zVIEW     TempCtrl2 = 0; 
   //:VIEW TZZOLFLO  BASED ON LOD  TZCMLPLO
   zVIEW     TZZOLFLO = 0; 
   //:VIEW UpdateLOD BASED ON LOD  TZZOLODO
   zVIEW     UpdateLOD = 0; 
   //:VIEW UpdateVOR BASED ON LOD  TZWDVORO
   zVIEW     UpdateVOR = 0; 
   //:STRING ( 20 ) szControlDefTag
   zCHAR     szControlDefTag[ 21 ] = { 0 }; 
   //:STRING ( 20 ) szControlType
   zCHAR     szControlType[ 21 ] = { 0 }; 
   //:STRING ( 50 ) szViewName
   zCHAR     szViewName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLOD_Name
   zCHAR     szLOD_Name[ 51 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER RowCount
   zLONG     RowCount = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_1[ 255 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Set up existing mapping for Grid.

   //:// If we're not positioned on the Grid, step down to it.
   //:IF TempCtrl.ControlDef.Tag != "Grid"
   if ( CompareAttributeToString( TempCtrl, "ControlDef", "Tag", "Grid" ) != 0 )
   { 
      //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )   // Step down to Grid
      SetViewToSubobject( TempCtrl, "CtrlCtrl" );
   } 

   //:END
   //:TZADWWKO.AutoDesignWork.AutodesignGroupCtrlsExistFlag = "Y"
   SetAttributeFromString( TZADWWKO, "AutoDesignWork", "AutodesignGroupCtrlsExistFlag", "Y" );
   //://TZADWWKO.EntitySubGroup.GroupType                     = "ListGroupOnly"
   //:TZADWWKO.EntitySubGroup.ReturnedObjectRootEntityName  = TempCtrl.CtrlMapLOD_Entity.Name 
   SetAttributeFromAttribute( TZADWWKO, "EntitySubGroup", "ReturnedObjectRootEntityName", TempCtrl, "CtrlMapLOD_Entity", "Name" );
   //:TZADWWKO.EntitySubGroup.AutodesignGroupTemplateName   = "Simple Grid"
   SetAttributeFromString( TZADWWKO, "EntitySubGroup", "AutodesignGroupTemplateName", "Simple Grid" );
   //:CREATE ENTITY TZADWWKO.AutodesignGroupTemplate 
   RESULT = CreateEntity( TZADWWKO, "AutodesignGroupTemplate", zPOS_AFTER );
   //:TZADWWKO.AutodesignGroupTemplate.GroupName = "Simple Grid"
   SetAttributeFromString( TZADWWKO, "AutodesignGroupTemplate", "GroupName", "Simple Grid" );

   //:// Activate Object View and LOD.
   //:szViewName = TempCtrl.CtrlMapView.Name 
   GetVariableFromAttribute( szViewName, 0, 'S', 51, TempCtrl, "CtrlMapView", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( TZADWWKO, UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 )
   nRC = ActivateMetaOI_ByName( TZADWWKO, &UpdateVOR, 0, zREFER_VOR_META, zSINGLE, szViewName, 0 );
   //:NAME VIEW UpdateVOR "UpdateObjectViewRef"
   SetNameForView( UpdateVOR, "UpdateObjectViewRef", 0, zLEVEL_TASK );
   //:IF TZADWWKO.W_MetaDefUpdateView EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateView" );
   if ( lTempInteger_0 == 0 )
   { 
      //:EXCLUDE TZADWWKO.W_MetaDefUpdateView  
      RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateView", zREPOS_AFTER );
   } 

   //:END 
   //:SET CURSOR FIRST TaskLPLR.W_MetaType WHERE TaskLPLR.W_MetaType.Type = 9   // Read view of Registered View
   RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 9, "" );
   //:SET CURSOR FIRST TaskLPLR.W_MetaDef  WHERE TaskLPLR.W_MetaDef.Name  = UpdateVOR.ViewObjRef.Name  
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateVOR, "ViewObjRef", "Name" );
   RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
   //:INCLUDE TZADWWKO.W_MetaDefUpdateView  FROM TaskLPLR.W_MetaDef 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateView", TaskLPLR, "W_MetaDef", zPOS_AFTER );
   //:szLOD_Name = UpdateVOR.LOD.Name 
   GetVariableFromAttribute( szLOD_Name, 0, 'S', 51, UpdateVOR, "LOD", "Name", "", 0 );
   //:nRC = ActivateMetaOI_ByName( TZADWWKO, UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 )
   nRC = ActivateMetaOI_ByName( TZADWWKO, &UpdateLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
   //:NAME VIEW UpdateLOD "TZZOLODO_Update"
   SetNameForView( UpdateLOD, "TZZOLODO_Update", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST TaskLPLR.W_MetaType  WHERE TaskLPLR.W_MetaType.Type = 7   // Read view of LOD
   RESULT = SetCursorFirstEntityByInteger( TaskLPLR, "W_MetaType", "Type", 7, "" );
   //:SET CURSOR FIRST TaskLPLR.W_MetaDef   WHERE TaskLPLR.W_MetaDef.Name  = UpdateLOD.LOD.Name 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD", "Name" );
   RESULT = SetCursorFirstEntityByString( TaskLPLR, "W_MetaDef", "Name", szTempString_0, "" );
   //:IF TZADWWKO.W_MetaDefUpdateObject EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( TZADWWKO, "W_MetaDefUpdateObject" );
   if ( lTempInteger_1 == 0 )
   { 
      //:EXCLUDE TZADWWKO.W_MetaDefUpdateObject 
      RESULT = ExcludeEntity( TZADWWKO, "W_MetaDefUpdateObject", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE TZADWWKO.W_MetaDefUpdateObject FROM TaskLPLR.W_MetaDef 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "W_MetaDefUpdateObject", TaskLPLR, "W_MetaDef", zPOS_AFTER );

   //:// Build the list of selected attributes for the new Grid and delete Grid subcontrols except for the first and buttons.
   //:INCLUDE TZADWWKO.ESG_LOD_Entity FROM TempCtrl.CtrlMapLOD_Entity 
   RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_LOD_Entity", TempCtrl, "CtrlMapLOD_Entity", zPOS_AFTER );
   //:SetViewToSubobject( TempCtrl, "CtrlCtrl" )    // Step down to Grid subcontrol
   SetViewToSubobject( TempCtrl, "CtrlCtrl" );
   //:Count = 0
   Count = 0;
   //:FOR EACH TempCtrl.Control 
   RESULT = SetCursorFirstEntity( TempCtrl, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:Count = Count + 1
      Count = Count + 1;
      //:IF TempCtrl.CtrlMapLOD_Attribute EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( TempCtrl, "CtrlMapLOD_Attribute" );
      if ( lTempInteger_2 == 0 )
      { 
         //:// Make sure there wasn't a duplicate Subcontrol.
         //:SET CURSOR FIRST TZADWWKO.ESG_ListLOD_Attribute WHERE TZADWWKO.ESG_ListLOD_Attribute.PromptValue = TempCtrl.Control.Text
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), TempCtrl, "Control", "Text" );
         RESULT = SetCursorFirstEntityByString( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", szTempString_1, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR LAST TZADWWKO.ESG_ListLOD_Attribute
            RESULT = SetCursorLastEntity( TZADWWKO, "ESG_ListLOD_Attribute", "" );
            //:INCLUDE TZADWWKO.ESG_ListLOD_Attribute FROM TempCtrl.CtrlMapLOD_Attribute 
            RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "ESG_ListLOD_Attribute", TempCtrl, "CtrlMapLOD_Attribute", zPOS_AFTER );
            //:TZADWWKO.ESG_ListLOD_Attribute.PromptValue = TempCtrl.Control.Text 
            SetAttributeFromAttribute( TZADWWKO, "ESG_ListLOD_Attribute", "PromptValue", TempCtrl, "Control", "Text" );

            //:// ControlType of "" becomes either Text or EditBox Control depending on WebControlProperty.
            //:szControlType = TempCtrl.ControlDef.Tag
            GetVariableFromAttribute( szControlType, 0, 'S', 21, TempCtrl, "ControlDef", "Tag", "", 0 );
            //:IF szControlType = "GridEditCtl"
            if ( ZeidonStringCompare( szControlType, 1, 0, "GridEditCtl", 1, 0, 21 ) == 0 )
            { 
               //:SET CURSOR FIRST TempCtrl.WebControlProperty WHERE TempCtrl.WebControlProperty.Name = "Edit"
               RESULT = SetCursorFirstEntityByString( TempCtrl, "WebControlProperty", "Name", "Edit", "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "EditBox"
                  SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "EditBox" );
                  //:ELSE
               } 
               else
               { 
                  //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = "Text"
                  SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", "Text" );
               } 

               //:END 
               //:ELSE
            } 
            else
            { 
               //:TZADWWKO.ESG_ListLOD_Attribute.ControlType = szControlType
               SetAttributeFromString( TZADWWKO, "ESG_ListLOD_Attribute", "ControlType", szControlType );
            } 

            //:END
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( TempCtrl, "Control", "" );
      //:END 
   } 

   //:END
   //:ResetViewFromSubobject( TempCtrl )
   ResetViewFromSubobject( TempCtrl );

   //:// Build the list of selectable LOD Entities for a potential list box.
   //:// We have one entry in the list for each Entity in the Update Object.
   //:// First remove any current entries then rebuild them.
   //:FOR EACH TZADWWKO.PotentialLOD_Entity 
   RESULT = SetCursorFirstEntity( TZADWWKO, "PotentialLOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:EXCLUDE TZADWWKO.PotentialLOD_Entity NONE 
      RESULT = ExcludeEntity( TZADWWKO, "PotentialLOD_Entity", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "PotentialLOD_Entity", "" );
   } 

   //:END
   //:FOR EACH UpdateLOD.LOD_Entity 
   RESULT = SetCursorFirstEntity( UpdateLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:INCLUDE TZADWWKO.PotentialLOD_Entity FROM UpdateLOD.LOD_Entity 
      RESULT = IncludeSubobjectFromSubobject( TZADWWKO, "PotentialLOD_Entity", UpdateLOD, "LOD_Entity", zPOS_AFTER );
      RESULT = SetCursorNextEntity( UpdateLOD, "LOD_Entity", "" );
   } 

   //:END

   //:// Delete any current potential attributes and then go to build them.
   //:FOR EACH TZADWWKO.ESG_FlatListPotentialAttribute 
   RESULT = SetCursorFirstEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY TZADWWKO.ESG_FlatListPotentialAttribute NONE  
      RESULT = DeleteEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", zREPOS_NONE );
      RESULT = SetCursorNextEntity( TZADWWKO, "ESG_FlatListPotentialAttribute", "" );
   } 

   //:END
   //:BuildAD_GroupPotList( TZADWWKO, UpdateLOD, UpdateLOD.LOD_EntityParent.Name, "" )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), UpdateLOD, "LOD_EntityParent", "Name" );
   oTZADWWKO_BuildAD_GroupPotList( TZADWWKO, UpdateLOD, szTempString_0, "" );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_InitializeL_SymbolType( zVIEW     TZADWWKO )
{
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 

   //:InitializeL_SymbolType( VIEW TZADWWKO BASED ON LOD TZADWWKO )

   //:// Initialize ListboxSymbolType entries if they don't already exist.
   //:IF TZADWWKO.ListboxSymbolType DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( TZADWWKO, "ListboxSymbolType" );
   if ( lTempInteger_0 != 0 )
   { 
      //:TZADWWKO.AutoDesignWork.SelectedListboxSymbolType = "Buttons"
      SetAttributeFromString( TZADWWKO, "AutoDesignWork", "SelectedListboxSymbolType", "Buttons" );
      //:CREATE ENTITY TZADWWKO.ListboxSymbolType 
      RESULT = CreateEntity( TZADWWKO, "ListboxSymbolType", zPOS_AFTER );
      //:TZADWWKO.ListboxSymbolType.Name = "Buttons"
      SetAttributeFromString( TZADWWKO, "ListboxSymbolType", "Name", "Buttons" );
      //:CREATE ENTITY TZADWWKO.ListboxSymbolType 
      RESULT = CreateEntity( TZADWWKO, "ListboxSymbolType", zPOS_AFTER );
      //:TZADWWKO.ListboxSymbolType.Name = "Icons" 
      SetAttributeFromString( TZADWWKO, "ListboxSymbolType", "Name", "Icons" );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:PositionOnControlByTag( VIEW TZADWWKO       BASED ON LOD TZADWWKO,
//:                        VIEW ReturnedView   BASED ON LOD TZWDLGSO,
//:                        VIEW OuterStartView BASED ON LOD TZWDLGSO,
//:                        STRING ( 50 ) szControlTagName )

//:   VIEW TZCtlHier BASED ON LOD  TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_PositionOnControlByTag( zVIEW     TZADWWKO,
                                  zPVIEW    ReturnedView,
                                  zVIEW     OuterStartView,
                                  zPCHAR    szControlTagName )
{
   zVIEW     TZCtlHier = 0; 
   //:STRING ( 50 ) szReturnedEntityName
   zCHAR     szReturnedEntityName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szControlDef
   zCHAR     szControlDef[ 51 ] = { 0 }; 
   //:SHORT lReturnedLevel
   zSHORT    lReturnedLevel = 0; 
   //:SHORT lInitialLevel
   zSHORT    lInitialLevel = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 


   //:CreateViewFromView( TZCtlHier, OuterStartView )
   CreateViewFromView( &TZCtlHier, OuterStartView );
   //:DefineHierarchicalCursor( TZCtlHier, "Control" )
   DefineHierarchicalCursor( TZCtlHier, "Control" );
   //:NAME VIEW TZCtlHier "TZCtlHier"
   SetNameForView( TZCtlHier, "TZCtlHier", 0, zLEVEL_TASK );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   //:lInitialLevel = lReturnedLevel
   lInitialLevel = lReturnedLevel;
   //:LOOP WHILE nRC >= zCURSOR_SET AND lReturnedLevel >= lInitialLevel
   while ( nRC >= zCURSOR_SET && lReturnedLevel >= lInitialLevel )
   { 
      //:IF nRC = zCURSOR_SET_RECURSIVECHILD
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      { 
         //:SetViewToSubobject( TZCtlHier, "CtrlCtrl" )
         SetViewToSubobject( TZCtlHier, "CtrlCtrl" );
      } 

      //:END
      //:IF szReturnedEntityName = "CtrlCtrl"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlCtrl", 1, 0, 51 ) == 0 )
      { 
         //:szControlDef = TZCtlHier.Control.Tag 
         GetVariableFromAttribute( szControlDef, 0, 'S', 51, TZCtlHier, "Control", "Tag", "", 0 );
         //:IF szControlDef = szControlTagName
         if ( ZeidonStringCompare( szControlDef, 1, 0, szControlTagName, 1, 0, 51 ) == 0 )
         { 
            //:// This is a Text Control, so convert any _Section or _SectionTitle characters
            //:CreateViewFromView( ReturnedView, TZCtlHier )
            CreateViewFromView( ReturnedView, TZCtlHier );
            //:DropView( TZCtlHier )
            DropView( TZCtlHier );
            //:RETURN 0
            return( 0 );
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, TZCtlHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, TZCtlHier );
   } 

   //:END
   //:DropView( TZCtlHier )
   DropView( TZCtlHier );
   //:RETURN -1
   return( -1 );
// END
} 


//:TRANSFORMATION OPERATION
//:RenameAD_Controls( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                   VIEW TZWINDOW BASED ON LOD TZWDLGSO )

//:   STRING ( 5 ) szPrefix
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_RenameAD_Controls( zVIEW     TZADWWKO,
                             zVIEW     TZWINDOW )
{
   zCHAR     szPrefix[ 6 ] = { 0 }; 
   //:STRING ( 5 ) szCount
   zCHAR     szCount[ 6 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   zSHORT    RESULT; 


   //:// Rename GroupBox, Grid and PushBtn controls to avoid duplicate generated names.
   //:// We will leave the name of the top level Group control and rename the the Groupbox controls under it, except for
   //:// those with CSS_Class "header" or "collapse show".
   //:// The rest will be a combination of GBxxx were xxx is a number string made up of the sequential Group Number
   //:// at each level

   //:// We'll only perform this logic for any top-level GroupBox.
   //:// Also, if there's only one GroupBox entry at this level, we will rename it "GB" and process it's subgroups.
   //:// If there are multiple  GroupBox entries, we will start renaming them with number suffix's.
   //:Count = 0
   Count = 0;
   //:FOR EACH TZWINDOW.Control
   RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF TZWINDOW.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:Count = Count + 1
         Count = Count + 1;
      } 

      RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
      //:END
   } 

   //:END
   //:SET CURSOR FIRST TZWINDOW.Control   // Go back to first Control in case there were multiple.
   RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
   //:IF Count > 0
   if ( Count > 0 )
   { 
      //:IF Count = 1
      if ( Count == 1 )
      { 
         //:// Rename single top-level GroupBox "GB".
         //:SET CURSOR FIRST TZWINDOW.ControlDef WITHIN TZWINDOW.Window 
         //:           WHERE TZWINDOW.ControlDef.Tag = "GroupBox" 
         RESULT = SetCursorFirstEntityByString( TZWINDOW, "ControlDef", "Tag", "GroupBox", "Window" );
         //:TZWINDOW.Control.Tag = "GB" 
         SetAttributeFromString( TZWINDOW, "Control", "Tag", "GB" );
         //:SetViewToSubobject( TZWINDOW, "CtrlCtrl" )
         SetViewToSubobject( TZWINDOW, "CtrlCtrl" );
      } 

      //:END

      //:// Process multiple GroupBox entries. Skip any entry of CSS_Class "header".
      //:Count = 0
      Count = 0;
      //:FOR EACH TZWINDOW.Control 
      RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF TZWINDOW.Control.CSS_Class != "header"
         if ( CompareAttributeToString( TZWINDOW, "Control", "CSS_Class", "header" ) != 0 )
         { 
            //:Count = Count + 1
            Count = Count + 1;
            //:zIntegerToString( szCount, 5, Count )
            zIntegerToString( szCount, 5, Count );
            //:szPrefix = "GB" + szCount
            ZeidonStringCopy( szPrefix, 1, 0, "GB", 1, 0, 6 );
            ZeidonStringConcat( szPrefix, 1, 0, szCount, 1, 0, 6 );
            //:TZWINDOW.Control.Tag = szPrefix
            SetAttributeFromString( TZWINDOW, "Control", "Tag", szPrefix );
            //:RenameAD_ControlsR( TZADWWKO, TZWINDOW, szPrefix )
            oTZADWWKO_RenameAD_ControlsR( TZADWWKO, TZWINDOW, szPrefix );
         } 

         RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
         //:END
      } 

      //:END
      //:ResetViewFromSubobject( TZWINDOW )
      ResetViewFromSubobject( TZWINDOW );
      //:ResetViewFromSubobject( TZWINDOW )
      ResetViewFromSubobject( TZWINDOW );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:RenameAD_ControlsR( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                    VIEW TZWINDOW BASED ON LOD TZWDLGSO,
//:                    STRING ( 50 ) szPrefix )

//:   STRING ( 50 ) szControlName
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_RenameAD_ControlsR( zVIEW     TZADWWKO,
                              zVIEW     TZWINDOW,
                              zPCHAR    szPrefix )
{
   zCHAR     szControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szNewControlName
   zCHAR     szNewControlName[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLevel
   zCHAR     szLevel[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szLocalPrefix
   zCHAR     szLocalPrefix[ 51 ] = { 0 }; 
   //:STRING ( 20 ) szCompare
   zCHAR     szCompare[ 21 ] = { 0 }; 
   //:STRING ( 5 )  szCount
   zCHAR     szCount[ 6 ] = { 0 }; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:INTEGER SubCount
   zLONG     SubCount = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:// Recursive routine to name GroupBox, Grid and PushBtn controls to avoid duplicate generated names.
   //:IF TZWINDOW.CtrlCtrl EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( TZWINDOW, "CtrlCtrl" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetViewToSubobject( TZWINDOW, "CtrlCtrl" )
      SetViewToSubobject( TZWINDOW, "CtrlCtrl" );
      //:Count = 0
      Count = 0;
      //:FOR EACH TZWINDOW.Control 
      RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:Count = Count + 1
         Count = Count + 1;
         //:zIntegerToString( szCount, 5, Count )
         zIntegerToString( szCount, 5, Count );
         //:szLocalPrefix = szPrefix + szCount
         ZeidonStringCopy( szLocalPrefix, 1, 0, szPrefix, 1, 0, 51 );
         ZeidonStringConcat( szLocalPrefix, 1, 0, szCount, 1, 0, 51 );
         //:IF TZWINDOW.ControlDef.Tag = "GroupBox"
         if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "GroupBox" ) == 0 )
         { 
            //:// On 1/29/2025 DonC eliminate the check below because it failed to rename GroupBox controls that we used for positioning.
            //:// Don't rename a GroupBox if it has no subcontrols as it is special & needs to keep its name.
            //://IF TZWINDOW.CtrlCtrl EXISTS
            //:   // Skip the collapsable or header Group because we need to keep it's existing name.
            //:   IF TZWINDOW.Control.CSS_Class != "collapse show" AND 
            //:      TZWINDOW.Control.CSS_Class != "header"  
            if ( CompareAttributeToString( TZWINDOW, "Control", "CSS_Class", "collapse show" ) != 0 && CompareAttributeToString( TZWINDOW, "Control", "CSS_Class", "header" ) != 0 )
            { 
               //:   // Add the Prefix and TopCount to the outside GroupBox Tag.
               //:   szNewControlName = szLocalPrefix
               ZeidonStringCopy( szNewControlName, 1, 0, szLocalPrefix, 1, 0, 51 );
               //:   TZWINDOW.Control.Tag = szNewControlName
               SetAttributeFromString( TZWINDOW, "Control", "Tag", szNewControlName );
            } 

            //:   END
            //:   
            //:   // Continue processing any subcontrols.
            //:   IF TZWINDOW.CtrlCtrl EXISTS
            lTempInteger_1 = CheckExistenceOfEntity( TZWINDOW, "CtrlCtrl" );
            if ( lTempInteger_1 == 0 )
            { 
               //:   RenameAD_ControlsR( TZADWWKO, TZWINDOW, szNewControlName )
               oTZADWWKO_RenameAD_ControlsR( TZADWWKO, TZWINDOW, szNewControlName );
            } 

            //:   END
            //://END
            //:ELSE
         } 
         else
         { 

            //:IF TZWINDOW.ControlDef.Tag = "PushBtn"
            if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "PushBtn" ) == 0 )
            { 
               //:// Add the Prefix after the letters, "PushBtn"
               //:szNewControlName = "PushBtn" + szLocalPrefix
               ZeidonStringCopy( szNewControlName, 1, 0, "PushBtn", 1, 0, 51 );
               ZeidonStringConcat( szNewControlName, 1, 0, szLocalPrefix, 1, 0, 51 );
               //:TZWINDOW.Control.Tag = szNewControlName
               SetAttributeFromString( TZWINDOW, "Control", "Tag", szNewControlName );
               //:ELSE
            } 
            else
            { 

               //:IF TZWINDOW.ControlDef.Tag = "Grid"
               if ( CompareAttributeToString( TZWINDOW, "ControlDef", "Tag", "Grid" ) == 0 )
               { 
                  //:// Add Prefix after "Grid" or >GridSub"
                  //:TZWINDOW.Control.Tag = "Grid" + szLocalPrefix 
                  ZeidonStringCopy( szTempString_0, 1, 0, "Grid", 1, 0, 33 );
                  ZeidonStringConcat( szTempString_0, 1, 0, szLocalPrefix, 1, 0, 33 );
                  SetAttributeFromString( TZWINDOW, "Control", "Tag", szTempString_0 );
                  //:SetViewToSubobject( TZWINDOW, "CtrlCtrl" )
                  SetViewToSubobject( TZWINDOW, "CtrlCtrl" );
                  //:SubCount = 0
                  SubCount = 0;
                  //:FOR EACH TZWINDOW.Control 
                  RESULT = SetCursorFirstEntity( TZWINDOW, "Control", "" );
                  while ( RESULT > zCURSOR_UNCHANGED )
                  { 
                     //:SubCount = SubCount + 1
                     SubCount = SubCount + 1;
                     //:zIntegerToString( szCount, 5, SubCount )
                     zIntegerToString( szCount, 5, SubCount );
                     //:TZWINDOW.Control.Tag = "GridSub" + szLocalPrefix + szCount
                     ZeidonStringCopy( szTempString_1, 1, 0, "GridSub", 1, 0, 33 );
                     ZeidonStringConcat( szTempString_1, 1, 0, szLocalPrefix, 1, 0, 33 );
                     ZeidonStringConcat( szTempString_1, 1, 0, szCount, 1, 0, 33 );
                     SetAttributeFromString( TZWINDOW, "Control", "Tag", szTempString_1 );
                     RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
                  } 

                  //:END
                  //:ResetViewFromSubobject( TZWINDOW )
                  ResetViewFromSubobject( TZWINDOW );
               } 

               //:END
            } 

            //:END
         } 

         RESULT = SetCursorNextEntity( TZWINDOW, "Control", "" );
         //:END
      } 

      //:END
      //:ResetViewFromSubobject( TZWINDOW )
      ResetViewFromSubobject( TZWINDOW );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ResizeReposGroupBoxes( VIEW TZADWWKO  BASED ON LOD TZADWWKO,
//:                       VIEW TZWINDOWL BASED ON LOD TZWDLGSO )

//:   VIEW vGroup BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_ResizeReposGroupBoxes( zVIEW     TZADWWKO,
                                 zVIEW     TZWINDOWL )
{
   zVIEW     vGroup = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 


   //:// RECOMPUTE GROUP SIZES AND POSITIONS.
   //:// Call Recursive operation to compute size and reposition subcontrols.
   //:// However, first step down past the first two outer control levels.
   //:CreateViewFromView( vGroup, TZWINDOWL )
   CreateViewFromView( &vGroup, TZWINDOWL );
   //:NAME VIEW vGroup "vGroup"
   SetNameForView( vGroup, "vGroup", 0, zLEVEL_TASK );
   //:SET CURSOR LAST vGroup.Control   // Make sure we're on outer Group, which should be last control.
   RESULT = SetCursorLastEntity( vGroup, "Control", "" );
   //:ResizeReposRecurs( vGroup )
   oTZADWWKO_ResizeReposRecurs( vGroup );

   //:// Compute the size of the outer Group, which is determined from the last subcontrol.
   //:SET CURSOR LAST vGroup.CtrlCtrl
   RESULT = SetCursorLastEntity( vGroup, "CtrlCtrl", "" );
   //:vGroup.Control.SZDLG_Y = vGroup.CtrlCtrl.PSDLG_Y + vGroup.CtrlCtrl.SZDLG_Y + 10
   GetIntegerFromAttribute( &lTempInteger_0, vGroup, "CtrlCtrl", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_1, vGroup, "CtrlCtrl", "SZDLG_Y" );
   lTempInteger_2 = lTempInteger_0 + lTempInteger_1 + 10;
   SetAttributeFromInteger( vGroup, "Control", "SZDLG_Y", lTempInteger_2 );
   //:DropView( vGroup )
   DropView( vGroup );
   return( 0 );
//    
// END
} 


//:LOCAL OPERATION
//:ResizeReposRecurs( VIEW vGroup BASED ON LOD TZWDLGSO )

//:   INTEGER CurrentPositionY
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_ResizeReposRecurs( zVIEW     vGroup )
{
   zLONG     CurrentPositionY = 0; 
   //:INTEGER LastOriginalPositionY
   zLONG     LastOriginalPositionY = 0; 
   //:INTEGER LargestControlSizeX
   zLONG     LargestControlSizeX = 0; 
   //:INTEGER CurrentControlSizeX
   zLONG     CurrentControlSizeX = 0; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 20 )  szGroupClass
   zCHAR     szGroupClass[ 21 ] = { 0 }; 
   //:STRING ( 4 )   szCol4
   zCHAR     szCol4[ 5 ] = { 0 }; 
   //:STRING ( 5 )   szInteger
   zCHAR     szInteger[ 6 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 


   //:// Process nested GroupBox Controls recursively recomputing the size and position of each based on the size of subcontrols.
   //:// Exit if there are no subcontrols or if any subcontrol is not a GroupBox.

   //:IF vGroup.CtrlCtrl DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( vGroup, "CtrlCtrl" );
   if ( lTempInteger_0 != 0 )
   { 
      //:RETURN 1
      return( 1 );
      //:ELSE
   } 
   else
   { 
      //:SetViewToSubobject( vGroup, "CtrlCtrl" )
      SetViewToSubobject( vGroup, "CtrlCtrl" );
      //:FOR EACH vGroup.Control 
      RESULT = SetCursorFirstEntity( vGroup, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF vGroup.ControlDef.Tag != "GroupBox"
         if ( CompareAttributeToString( vGroup, "ControlDef", "Tag", "GroupBox" ) != 0 )
         { 
            //:ResetViewFromSubobject( vGroup )
            ResetViewFromSubobject( vGroup );
            //:RETURN 1
            return( 1 );
         } 

         RESULT = SetCursorNextEntity( vGroup, "Control", "" );
         //:END
      } 

      //:END
      //:ResetViewFromSubobject( vGroup )
      ResetViewFromSubobject( vGroup );
   } 

   //:END

   //:// Process each GroupBox subcontrol to determine its size and position relative to this GroupBox.
   //:CurrentPositionY      = 10
   CurrentPositionY = 10;
   //:LastOriginalPositionY = -1
   LastOriginalPositionY = -1;
   //:FOR EACH vGroup.CtrlCtrl  
   RESULT = SetCursorFirstEntity( vGroup, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( vGroup, "CtrlCtrl" )
      SetViewToSubobject( vGroup, "CtrlCtrl" );
      //:szGroupClass = vGroup.Control.CSS_Class 
      GetVariableFromAttribute( szGroupClass, 0, 'S', 21, vGroup, "Control", "CSS_Class", "", 0 );
      //:szCol4 = szGroupClass[1:4]
      ZeidonStringCopy( szCol4, 1, 0, szGroupClass, 1, 4, 5 );
      //:// Only process Groups that don't have a subcontrol of CSS_Class = col-xxx
      //:IF vGroup.ControlDef.Tag = "GroupBox" AND szCol4 != "col-" AND szGroupClass != "col"
      if ( CompareAttributeToString( vGroup, "ControlDef", "Tag", "GroupBox" ) == 0 && ZeidonStringCompare( szCol4, 1, 0, "col-", 1, 0, 5 ) != 0 && ZeidonStringCompare( szGroupClass, 1, 0, "col", 1, 0, 21 ) != 0 )
      { 
         //:ResizeReposRecurs( vGroup )
         oTZADWWKO_ResizeReposRecurs( vGroup );
      } 

      //:END 
      //:ResetViewFromSubobject( vGroup )
      ResetViewFromSubobject( vGroup );

      //:// Determine Y position of each based on the previous GroupBox size and position.
      //:// Don't process Groups than have col-xxx subgroups as they don't need it and may also be side by side.
      //:IF szCol4 != "col-" AND szGroupClass != "col"
      if ( ZeidonStringCompare( szCol4, 1, 0, "col-", 1, 0, 5 ) != 0 && ZeidonStringCompare( szGroupClass, 1, 0, "col", 1, 0, 21 ) != 0 )
      { 
         //:LastOriginalPositionY = vGroup.CtrlCtrl.PSDLG_Y
         GetIntegerFromAttribute( &LastOriginalPositionY, vGroup, "CtrlCtrl", "PSDLG_Y" );
         //:vGroup.CtrlCtrl.PSDLG_Y = CurrentPositionY
         SetAttributeFromInteger( vGroup, "CtrlCtrl", "PSDLG_Y", CurrentPositionY );
         //:CurrentPositionY = CurrentPositionY + vGroup.CtrlCtrl.SZDLG_Y + 10
         GetIntegerFromAttribute( &lTempInteger_1, vGroup, "CtrlCtrl", "SZDLG_Y" );
         CurrentPositionY = CurrentPositionY + lTempInteger_1 + 10;
      } 

      RESULT = SetCursorNextEntity( vGroup, "CtrlCtrl", "" );
      //:END
   } 

   //:END

   //:// Determine the Y size of this GroupBox based on the size and position of the last subGroupBox + 10.
   //:SET CURSOR LAST vGroup.CtrlCtrl
   RESULT = SetCursorLastEntity( vGroup, "CtrlCtrl", "" );
   //:vGroup.Control.SZDLG_Y = vGroup.CtrlCtrl.PSDLG_Y + vGroup.CtrlCtrl.SZDLG_Y + 10
   GetIntegerFromAttribute( &lTempInteger_2, vGroup, "CtrlCtrl", "PSDLG_Y" );
   GetIntegerFromAttribute( &lTempInteger_3, vGroup, "CtrlCtrl", "SZDLG_Y" );
   lTempInteger_4 = lTempInteger_2 + lTempInteger_3 + 10;
   SetAttributeFromInteger( vGroup, "Control", "SZDLG_Y", lTempInteger_4 );

   //:// Determine the X size of this GroupBox based on the position and size of the subGroupBox entries.
   //:LargestControlSizeX = 0
   LargestControlSizeX = 0;
   //:FOR EACH vGroup.CtrlCtrl  
   RESULT = SetCursorFirstEntity( vGroup, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CurrentControlSizeX = vGroup.CtrlCtrl.PSDLG_X + vGroup.CtrlCtrl.SZDLG_X 
      GetIntegerFromAttribute( &lTempInteger_5, vGroup, "CtrlCtrl", "PSDLG_X" );
      GetIntegerFromAttribute( &lTempInteger_6, vGroup, "CtrlCtrl", "SZDLG_X" );
      CurrentControlSizeX = lTempInteger_5 + lTempInteger_6;
      //:IF CurrentControlSizeX > LargestControlSizeX
      if ( CurrentControlSizeX > LargestControlSizeX )
      { 
         //:LargestControlSizeX = CurrentControlSizeX
         LargestControlSizeX = CurrentControlSizeX;
      } 

      RESULT = SetCursorNextEntity( vGroup, "CtrlCtrl", "" );
      //:END
   } 

   //:END
   //:vGroup.Control.SZDLG_X = LargestControlSizeX + 10
   lTempInteger_7 = LargestControlSizeX + 10;
   SetAttributeFromInteger( vGroup, "Control", "SZDLG_X", lTempInteger_7 );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ReturnAttrControlType( VIEW AnyView,
//:                       STRING ( 32 ) szDomainName,
//:                       STRING ( 32 ) szDomainType,
//:                       STRING ( 10 ) szControlType,
//:                       STRING ( 1 )  szUpdateFlag )
static zSHORT
oTZADWWKO_ReturnAttrControlType( zVIEW     AnyView,
                                 zPCHAR    szDomainName,
                                 zPCHAR    szDomainType,
                                 zPCHAR    szControlType,
                                 zPCHAR    szUpdateFlag )
{


   //:// Return the Control Type based on the Domain and whether or not the Update Flag is set.
   //:// A Domain of Y/N always returns a Checkbox regardless of Update Flag.
   //:IF szDomainName = "Y/N"
   if ( ZeidonStringCompare( szDomainName, 1, 0, "Y/N", 1, 0, 33 ) == 0 )
   { 
      //:// Domain is Y/N.
      //:szControlType   = "CheckBox"
      ZeidonStringCopy( szControlType, 1, 0, "CheckBox", 1, 0, 11 );
      //:ELSE
   } 
   else
   { 
      //:IF szUpdateFlag = "Y"
      if ( ZeidonStringCompare( szUpdateFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:IF szDomainName = "Date" OR szDomainName = "DateTime"
         if ( ZeidonStringCompare( szDomainName, 1, 0, "Date", 1, 0, 33 ) == 0 || ZeidonStringCompare( szDomainName, 1, 0, "DateTime", 1, 0, 33 ) == 0 )
         { 
            //:// A Domain of Date will make the control a Calendar.
            //:szControlType = "Calendar"
            ZeidonStringCopy( szControlType, 1, 0, "Calendar", 1, 0, 11 );
            //:ELSE
         } 
         else
         { 
            //:IF szDomainType = "T"
            if ( ZeidonStringCompare( szDomainType, 1, 0, "T", 1, 0, 33 ) == 0 )
            { 
               //:// The Domain is a table, so make control a Combobox.
               //:szControlType = "ComboBox"
               ZeidonStringCopy( szControlType, 1, 0, "ComboBox", 1, 0, 11 );
               //:ELSE
            } 
            else
            { 
               //:// If not a table, make control an Editbox.
               //:szControlType = "EditBox"
               ZeidonStringCopy( szControlType, 1, 0, "EditBox", 1, 0, 11 );
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// The entity is not updatable, so Control Type is "Text"
         //:szControlType = "Text"
         ZeidonStringCopy( szControlType, 1, 0, "Text", 1, 0, 11 );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_SetDialogProperties( zVIEW     TZADWWKO,
                               zVIEW     TargetDialog,
                               zVIEW     SourceDialog )
{

   //:SetDialogProperties( VIEW TZADWWKO BASED ON LOD TZADWWKO,
   //:                  VIEW TargetDialog BASED ON LOD  TZWDLGSO,
   //:                  VIEW SourceDialog BASED ON LOD  TZWDLGSO )

   //:// Initialize the necessary Dialog Properties if they haven't already been set.
   //:IF TargetDialog.Dialog.WEB_PageHeadInclude = ""
   if ( CompareAttributeToString( TargetDialog, "Dialog", "WEB_PageHeadInclude", "" ) == 0 )
   { 
      //:SetMatchingAttributesByName( TargetDialog, "Dialog", SourceDialog, "Dialog", zSET_NULL )
      SetMatchingAttributesByName( TargetDialog, "Dialog", SourceDialog, "Dialog", zSET_NULL );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ReturnID_NameForEntity( VIEW TZADWWKO BASED ON LOD TZADWWKO,
//:                        STRING ( 50 ) szReturnedID_Name,
//:                        STRING ( 50 ) szEntityName,
//:                        VIEW ReturnedLOD BASED ON LOD TZZOLODO )

//:   VIEW TaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
oTZADWWKO_ReturnID_NameForEntity( zVIEW     TZADWWKO,
                                  zPCHAR    szReturnedID_Name,
                                  zPCHAR    szEntityName,
                                  zVIEW     ReturnedLOD )
{
   zVIEW     TaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW TempLPLR BASED ON LOD  TZCMLPLO
   zVIEW     TempLPLR = 0; 
   //:VIEW vERD     BASED ON LOD  TZEREMDO
   zVIEW     vERD = 0; 
   //:STRING ( 50 ) szER_EntityName
   zCHAR     szER_EntityName[ 51 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &TaskLPLR, "TaskLPLR", TZADWWKO, zLEVEL_TASK );

   //:// Activate the vERD view as necessary and use it get the Identifier Name for the Entity Name passed in.
   //:CreateViewFromView( TempLPLR, TaskLPLR )
   CreateViewFromView( &TempLPLR, TaskLPLR );
   //:NAME VIEW TempLPLR "TempLPLR"
   SetNameForView( TempLPLR, "TempLPLR", 0, zLEVEL_TASK );
   //:NAME VIEW ReturnedLOD "ReturnedLOD"
   SetNameForView( ReturnedLOD, "ReturnedLOD", 0, zLEVEL_TASK );
   //:SET CURSOR FIRST TempLPLR.W_MetaType WHERE TempLPLR.W_MetaType.Type = 4   // ERD View
   RESULT = SetCursorFirstEntityByInteger( TempLPLR, "W_MetaType", "Type", 4, "" );
   //:nRC = ActivateMetaOI( TZADWWKO, vERD, TempLPLR, zREFER_ERD_META, 0 )
   nRC = ActivateMetaOI( TZADWWKO, &vERD, TempLPLR, zREFER_ERD_META, 0 );
   //:NAME VIEW vERD "AutodesignERD"
   SetNameForView( vERD, "AutodesignERD", 0, zLEVEL_TASK );

   //:// Get the ER_Entity Name from the LOD to use in positioning on the ER Entity.
   //:SET CURSOR FIRST ReturnedLOD.LOD_Entity WHERE ReturnedLOD.LOD_Entity.Name = szEntityName
   RESULT = SetCursorFirstEntityByString( ReturnedLOD, "LOD_Entity", "Name", szEntityName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:MessageSend( TZADWWKO, "", "Autodesign Subdialog",
      //:             "Invalid Entity Name for ReturnID_NameForEntity.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( TZADWWKO, "", "Autodesign Subdialog", "Invalid Entity Name for ReturnID_NameForEntity.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0,0 )
      SetWindowActionBehavior( TZADWWKO, zWAB_StayOnWindow, 0, 0 );
      //:RETURN -2
      return( -2 );
   } 

   //:END 
   //:SET CURSOR FIRST vERD.ER_Entity WHERE vERD.ER_Entity.Name = ReturnedLOD.ER_Entity.Name 
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), ReturnedLOD, "ER_Entity", "Name" );
   RESULT = SetCursorFirstEntityByString( vERD, "ER_Entity", "Name", szTempString_0, "" );
   //:SET CURSOR FIRST vERD.ER_EntIdentifier WHERE vERD.ER_EntIdentifier.SystemMaintained = "Y"
   RESULT = SetCursorFirstEntityByString( vERD, "ER_EntIdentifier", "SystemMaintained", "Y", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szReturnedID_Name = vERD.ER_EntIdentifier.Name 
      GetVariableFromAttribute( szReturnedID_Name, 0, 'S', 51, vERD, "ER_EntIdentifier", "Name", "", 0 );
   } 

   //:END 
   //:DropObjectInstance( vERD )
   DropObjectInstance( vERD );
   //:DropView( TempLPLR )
   DropView( TempLPLR );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
