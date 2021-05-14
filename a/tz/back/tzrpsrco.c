#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
#include "TZ__OPRS.H" 
#include "tzlodopr.h" 
#include "TZVMLIP.H" 
#include "TZ__OPRS.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZRPSRCO_GenerateXRP_Recurs( zVIEW     NewReport,
                              zVIEW     DialogControl,
                              zLONG     X_Position,
                              zLONG     Y_Position,
                              zLONG     GroupSize );


static zVOID
oTZRPSRCO_GenerateXRP_CtrlMap( zVIEW     NewReport,
                               zVIEW     DialogControl );


static zSHORT
oTZRPSRCO_CopyControl( zVIEW     NewR,
                       zVIEW     NewRC,
                       zVIEW     OrigR,
                       zVIEW     OrigRC,
                       zVIEW     vPE,
                       zVIEW     SourceLPLR,
                       zVIEW     vSubtask );


static zSHORT
oTZRPSRCO_AdjustGroupSize( zVIEW     vReportDef,
                           zLONG     lPageWidth );


static zSHORT
oTZRPSRCO_CreateGrpSetForEntity( zVIEW     vSubtask,
                                 zVIEW     vReportDef,
                                 zVIEW     vUserSpec,
                                 zVIEW     vUserSpecRoot,
                                 zVIEW     vPE_Control );


static zLONG
oTZRPSRCO_GetMaxAttributeSize( zVIEW     vUserSpec,
                               zPCHAR    szPositionType );


//:TRANSFORMATION OPERATION
//:AutodesignReport( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                  VIEW vUserSpec  BASED ON LOD TZADCSDO,
//:                  VIEW vPE        BASED ON LOD TZPESRCO,
//:                  VIEW vSubtask )

//:   VIEW        vUserSpecRoot  BASED ON LOD TZADCSDO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_AutodesignReport( zVIEW     vReportDef,
                            zVIEW     vUserSpec,
                            zVIEW     vPE,
                            zVIEW     vSubtask )
{
   zVIEW     vUserSpecRoot = 0; 
   //:VIEW        vDefaults BASED ON LOD TZADCSDO
   zVIEW     vDefaults = 0; 
   //:VIEW        vTaskLPLR BASED ON LOD TZCMLPLO
   zVIEW     vTaskLPLR = 0; 
   //:STRING ( 513 ) szFileName                        // zMAX_FILESPEC_LTH+1
   zCHAR     szFileName[ 514 ] = { 0 }; 
   //:STRING ( 257 ) szMsg                             // zSHORT_MESSAGE_LTH+1
   zCHAR     szMsg[ 258 ] = { 0 }; 
   //:INTEGER     lFile
   zLONG     lFile = 0; 
   //:INTEGER     lCurrentVerticalPos
   zLONG     lCurrentVerticalPos = 0; 
   //:INTEGER     lTopGroupMargin
   zLONG     lTopGroupMargin = 0; 
   //:INTEGER     lBottomHeaderMargin
   zLONG     lBottomHeaderMargin = 0; 
   //:INTEGER     lLeftGroupMargin
   zLONG     lLeftGroupMargin = 0; 
   //:INTEGER     lHeight
   zLONG     lHeight = 0; 
   //:INTEGER     lCurrentPosY
   zLONG     lCurrentPosY = 0; 
   //:INTEGER     lPageNumberPosX
   zLONG     lPageNumberPosX = 0; 
   //:INTEGER     lDefaultFontSize
   zLONG     lDefaultFontSize = 0; 
   //:INTEGER     lTitleSize
   zLONG     lTitleSize = 0; 
   //:INTEGER     lFontSize
   zLONG     lFontSize = 0; 
   //:INTEGER     lPageWidth
   zLONG     lPageWidth = 0; 
   //:INTEGER     lVerticalIncrement
   zLONG     lVerticalIncrement = 0; 
   //:DECIMAL     dVertScalingFactor
   ZDecimal  dVertScalingFactor = 0.0; 
   //:SHORT       nCharacterHeight
   zSHORT    nCharacterHeight = 0; 
   //:SHORT       nCharacterWidth
   zSHORT    nCharacterWidth = 0; 
   //:SHORT       nBoldFlag
   zSHORT    nBoldFlag = 0; 
   //:SHORT       nItalicFlag
   zSHORT    nItalicFlag = 0; 
   //:SHORT       nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zCHAR     szTempString_0[ 255 ]; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zLONG     lTempInteger_8; 
   zLONG     lTempInteger_9; 
   zLONG     lTempInteger_10; 
   zCHAR     szTempString_1[ 255 ]; 
   zLONG     lTempInteger_11; 
   zLONG     lTempInteger_12; 
   zLONG     lTempInteger_13; 
   zLONG     lTempInteger_14; 
   zLONG     lTempInteger_15; 
   zLONG     lTempInteger_16; 


   //:// Retrieve default information and name it for later use.  If the instance
   //:// doesn't exist, exit.
   //:GET VIEW vTaskLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vReportDef, zLEVEL_TASK );
   //:szMsg = vTaskLPLR.LPLR.MetaSrcDir + "\Z__DFLT.BAS"  // borrow szMsg for a second
   GetStringFromAttribute( szMsg, zsizeof( szMsg ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( szMsg, 1, 0, "\\Z__DFLT.BAS", 1, 0, 258 );
   //:SysConvertEnvironmentString( szFileName, szMsg )
   SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), szMsg );
   //:lFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ )
   lFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   //:IF lFile < 0
   if ( lFile < 0 )
   { 
      //:szMsg = "LPLR Defaults not found." + zNew_Line + "Exiting Autodesign."
      ZeidonStringCopy( szMsg, 1, 0, "LPLR Defaults not found.", 1, 0, 258 );
      ZeidonStringConcat( szMsg, 1, 0, zNEW_LINE, 1, 0, 258 );
      ZeidonStringConcat( szMsg, 1, 0, "Exiting Autodesign.", 1, 0, 258 );
      //:MessageSend( vSubtask, "AD10201", "Autodesigner",
      //:             "LPLR Defaults not found.\nExiting Autodesign.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "AD10201", "Autodesigner", "LPLR Defaults not found.\\nExiting Autodesign.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
      //:ELSE
   } 
   else
   { 
      //:SysCloseFile( vSubtask, lFile, 0 )
      SysCloseFile( vSubtask, lFile, 0 );
      //:ActivateOI_FromFile( vDefaults, "TZADCSDO", vUserSpec,
      //:                     szFileName, zSINGLE )
      ActivateOI_FromFile( &vDefaults, "TZADCSDO", vUserSpec, szFileName, zSINGLE );
      //:NAME VIEW vDefaults "TZ_Defaults"
      SetNameForView( vDefaults, "TZ_Defaults", 0, zLEVEL_TASK );
   } 

   //:END

   //:// if Font Size does not exists, set size
   //:IF vDefaults.InitValues.ReportTextSize <= 0
   if ( CompareAttributeToInteger( vDefaults, "InitValues", "ReportTextSize", 0 ) <= 0 )
   { 
      //:szMsg = "No valid Default Font Size found. This Report" + zNew_Line
      ZeidonStringCopy( szMsg, 1, 0, "No valid Default Font Size found. This Report", 1, 0, 258 );
      ZeidonStringConcat( szMsg, 1, 0, zNEW_LINE, 1, 0, 258 );
      //:szMsg = szMsg + "will be created with a Font Size 10."
      ZeidonStringConcat( szMsg, 1, 0, "will be created with a Font Size 10.", 1, 0, 258 );
      //:nRC = MessagePrompt( vReportDef, "AD10202", "Autodesigner",
      //:                     szMsg, 1, zBUTTONS_OKCANCEL, zRESPONSE_OK,
      //:                     zICON_QUESTION   )
      nRC = MessagePrompt( vReportDef, "AD10202", "Autodesigner", szMsg, 1, zBUTTONS_OKCANCEL, zRESPONSE_OK, zICON_QUESTION );
      //:IF nRC = zRESPONSE_CANCEL
      if ( nRC == zRESPONSE_CANCEL )
      { 
         //:RETURN -1
         return( -1 );
         //:ELSE
      } 
      else
      { 
         //:vDefaults.InitValues.ReportTextSize = 10
         SetAttributeFromInteger( vDefaults, "InitValues", "ReportTextSize", 10 );
      } 

      //:END
   } 

   //:END

   //:// Delete entries for the existing ReportDef.
   //:DELETE ENTITY vReportDef.Page
   RESULT = DeleteEntity( vReportDef, "Page", zPOS_NEXT );
   //:CreateMetaEntity( vSubtask, vReportDef, "Page", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vReportDef, "Page", zPOS_AFTER );
   //:vReportDef.Page.Tag = vReportDef.Report.Tag
   SetAttributeFromAttribute( vReportDef, "Page", "Tag", vReportDef, "Report", "Tag" );

   //:// DEM - We will set the page sizes in the report def.
   //:// This is temporary code only and should be removed when
   //:// a final soln is arrived at. In the future, The default dialog
   //:// for autodesign should allow the page size to be set. For now,
   //:// we just set them here for a 8 1/2 x 11 page size. Sizes are in
   //:// 256ths of an inch
   //:vReportDef.Page.SizeX = 2176
   SetAttributeFromInteger( vReportDef, "Page", "SizeX", 2176 );
   //:vReportDef.Page.SizeY = 2816
   SetAttributeFromInteger( vReportDef, "Page", "SizeY", 2816 );

   //:// Set up default information.
   //:dVertScalingFactor   = vDefaults.InitValues.ReportTextIncrementY * 16
   GetIntegerFromAttribute( &lTempInteger_0, vDefaults, "InitValues", "ReportTextIncrementY" );
   dVertScalingFactor = lTempInteger_0 * 16;
   //:lVerticalIncrement   = 0
   lVerticalIncrement = 0;
   //:lTopGroupMargin      = 5 * 16
   lTopGroupMargin = 5 * 16;
   //:lBottomHeaderMargin  = 2 * 16
   lBottomHeaderMargin = 2 * 16;
   //:lLeftGroupMargin     = 5 * 16
   lLeftGroupMargin = 5 * 16;
   //:lPageWidth           = vReportDef.Page.SizeX
   GetIntegerFromAttribute( &lPageWidth, vReportDef, "Page", "SizeX" );
   //:lPageNumberPosX      = lPageWidth - 256
   lPageNumberPosX = lPageWidth - 256;
   //:lDefaultFontSize     = vDefaults.InitValues.ReportTextSize * 10
   GetIntegerFromAttribute( &lTempInteger_1, vDefaults, "InitValues", "ReportTextSize" );
   lDefaultFontSize = lTempInteger_1 * 10;
   //:lCurrentPosY         = 0
   lCurrentPosY = 0;

   //:// This transformation uses the User Interface Spec to build a report
   //:// definition.

   //:// First, we will build the whole Report Definition without specifying
   //:// any positions.  Then we will come back and determine positions.

   //:NAME VIEW vReportDef "***ReportDef"
   SetNameForView( vReportDef, "***ReportDef", 0, zLEVEL_TASK );
   //:NAME VIEW vUserSpec "***UserSpec"
   SetNameForView( vUserSpec, "***UserSpec", 0, zLEVEL_TASK );
   //:vReportDef.Report.Unit = 1
   SetAttributeFromInteger( vReportDef, "Report", "Unit", 1 );
   //:vReportDef.Report.ReportTextSize = lDefaultFontSize
   SetAttributeFromInteger( vReportDef, "Report", "ReportTextSize", lDefaultFontSize );

   //:// Add the ViewObjRef that will be used by all Controls on the report.
   //:IF vReportDef.ViewObjRef EXISTS
   lTempInteger_2 = CheckExistenceOfEntity( vReportDef, "ViewObjRef" );
   if ( lTempInteger_2 == 0 )
   { 
      //:EXCLUDE vReportDef.ViewObjRef
      RESULT = ExcludeEntity( vReportDef, "ViewObjRef", zREPOS_AFTER );
   } 

   //:END
   //:INCLUDE vReportDef.ViewObjRef FROM vUserSpec.UIS_ViewObjRef
   RESULT = IncludeSubobjectFromSubobject( vReportDef, "ViewObjRef", vUserSpec, "UIS_ViewObjRef", zPOS_AFTER );

   //:// Create a root view of the UserSpec for the inclusion of ViewObjRef.
   //:CreateViewFromViewForTask( vUserSpecRoot, vUserSpec, 0 )
   CreateViewFromViewForTask( &vUserSpecRoot, vUserSpec, 0 );

   //:// Create Page Header GroupSet and Group with initial values.

   //:CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER );
   //:vReportDef.GroupSet.Tag      = "PageHeader"
   SetAttributeFromString( vReportDef, "GroupSet", "Tag", "PageHeader" );
   //:vReportDef.GroupSet.Type     = "PH"
   SetAttributeFromString( vReportDef, "GroupSet", "Type", "PH" );
   //:vReportDef.GroupSet.PSDLG_X  = 0
   SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_X", 0 );
   //:vReportDef.GroupSet.PSDLG_Y  = 64
   SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", 64 );
   //:vReportDef.GroupSet.SZDLG_X  = vReportDef.Page.SizeX
   SetAttributeFromAttribute( vReportDef, "GroupSet", "SZDLG_X", vReportDef, "Page", "SizeX" );
   //:vReportDef.GroupSet.SZDLG_Y  = 320
   SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", 320 );
   //:vReportDef.GroupSet.PPE_Type = 3000
   SetAttributeFromInteger( vReportDef, "GroupSet", "PPE_Type", 3000 );

   //:CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
   //:vReportDef.Group.Tag         = "PageHeader"
   SetAttributeFromString( vReportDef, "Group", "Tag", "PageHeader" );
   //:vReportDef.Group.Type        = "ga"
   SetAttributeFromString( vReportDef, "Group", "Type", "ga" );
   //:vReportDef.Group.SubType     = "0"
   SetAttributeFromString( vReportDef, "Group", "SubType", "0" );
   //:vReportDef.Group.PSDLG_X     = 0
   SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
   //:vReportDef.Group.PSDLG_Y     = 64
   SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", 64 );
   //:vReportDef.Group.SZDLG_X     = vReportDef.Page.SizeX
   SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
   //:vReportDef.Group.SZDLG_Y     = 128
   SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", 128 );
   //:vReportDef.Group.PPE_Type    = 3010
   SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );

   //:lCurrentVerticalPos = lTopGroupMargin
   lCurrentVerticalPos = lTopGroupMargin;

   //:// Create Page Header Controls, if requested.  Headers are requested
   //:// by either having something entered in the Title or by requesting Paging.

   //:IF vUserSpec.UI_Spec.HeaderTitle != "" OR
   //:   vUserSpec.UI_Spec.HeaderPageNoFlag = "Y"
   if ( CompareAttributeToString( vUserSpec, "UI_Spec", "HeaderTitle", "" ) != 0 || CompareAttributeToString( vUserSpec, "UI_Spec", "HeaderPageNoFlag", "Y" ) == 0 )
   { 

      //:// Set the FontSize, Bold and Italic flags for the Header group.
      //:lFontSize = vUserSpec.UI_Spec.HeaderFontSize * 10
      GetIntegerFromAttribute( &lTempInteger_3, vUserSpec, "UI_Spec", "HeaderFontSize" );
      lFontSize = lTempInteger_3 * 10;
      //:IF vUserSpec.UI_Spec.HeaderBoldFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "HeaderBoldFlag", "Y" ) == 0 )
      { 
         //:nBoldFlag = TRUE
         nBoldFlag = TRUE;
         //:ELSE
      } 
      else
      { 
         //:nBoldFlag = FALSE
         nBoldFlag = FALSE;
      } 

      //:END
      //:IF vUserSpec.UI_Spec.HeaderItalicFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "HeaderItalicFlag", "Y" ) == 0 )
      { 
         //:nItalicFlag = TRUE
         nItalicFlag = TRUE;
         //:ELSE
      } 
      else
      { 
         //:nItalicFlag = FALSE
         nItalicFlag = FALSE;
      } 

      //:END

      //:// Set font size and character width and height.
      //:IF lFontSize = 0
      if ( lFontSize == 0 )
      { 
         //:lFontSize = lDefaultFontSize
         lFontSize = lDefaultFontSize;
      } 

      //:END
      //:nCharacterHeight   = ((lFontSize * 16) / 720) + 1
      nCharacterHeight = ( ( lFontSize * 16 ) / 720 ) + 1;
      //:nCharacterWidth    = ((lFontSize * 16) / 720)
      nCharacterWidth = ( ( lFontSize * 16 ) / 720 );
      //:lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16
      lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16;

      //:IF vUserSpec.UI_Spec.HeaderTitle != ""
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "HeaderTitle", "" ) != 0 )
      { 

         //:// Set up Page Header Text.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:vReportDef.Control.Text    = vUserSpec.UI_Spec.HeaderTitle
         SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "UI_Spec", "HeaderTitle" );
         //:vReportDef.Control.Tag     = "PageHeaderText"
         SetAttributeFromString( vReportDef, "Control", "Tag", "PageHeaderText" );
         //:vReportDef.Control.PSDLG_X = lLeftGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lLeftGroupMargin );
         //:vReportDef.Control.PSDLG_Y = lTopGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lTopGroupMargin );
         //:SET CURSOR FIRST vPE.ControlDef WHERE
         //:                 vPE.ControlDef.Tag = "Rpt_Text"
         RESULT = SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "Rpt_Text", "" );
         //:INCLUDE vReportDef.ControlDef FROM vPE.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE, "ControlDef", zPOS_AFTER );
         //:lTitleSize = GetStringLength( vReportDef.Control.Text )
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vReportDef, "Control", "Text" );
         lTitleSize = GetStringLength( szTempString_0 );
         //:vReportDef.Control.SZDLG_X = lTitleSize * nCharacterWidth * 16
         lTempInteger_4 = lTitleSize * nCharacterWidth * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_4 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_5 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_5 );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );
      } 

      //:END

      //:IF vUserSpec.UI_Spec.HeaderPageNoFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "HeaderPageNoFlag", "Y" ) == 0 )
      { 

         //:// Set up Header Page Number.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:vReportDef.Control.Tag     = "PageNumber"
         SetAttributeFromString( vReportDef, "Control", "Tag", "PageNumber" );
         //:vReportDef.Control.PSDLG_X = lPageNumberPosX
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lPageNumberPosX );
         //:vReportDef.Control.PSDLG_Y = lTopGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lTopGroupMargin );
         //:INCLUDE vReportDef.ControlDef FROM vPE.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE, "ControlDef", zPOS_AFTER );
         //:vReportDef.Control.SZDLG_X = 160 // Just a guess right now.
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", 160 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_6 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_6 );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );
      } 

      //:END

      //:// Set height of the group and groupset.  We will determine the Y size by
      //:// getting the last control (bottom in Group, though any control would do) and
      //:// adding BottomGroupMargin to it.
      //:lHeight = vReportDef.Control.PSDLG_Y + vReportDef.Control.SZDLG_Y +
      //:          lVerticalIncrement + (lBottomHeaderMargin * 2)
      GetIntegerFromAttribute( &lTempInteger_7, vReportDef, "Control", "PSDLG_Y" );
      GetIntegerFromAttribute( &lTempInteger_8, vReportDef, "Control", "SZDLG_Y" );
      lHeight = lTempInteger_7 + lTempInteger_8 + lVerticalIncrement + ( lBottomHeaderMargin * 2 );
      //:vReportDef.Group.SZDLG_Y    = lHeight
      SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lHeight );
      //:vReportDef.GroupSet.SZDLG_Y = lHeight + 64
      lTempInteger_9 = lHeight + 64;
      SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lTempInteger_9 );
      //:lCurrentPosY = lHeight
      lCurrentPosY = lHeight;
   } 

   //:END

   //:// Create Page Footer GroupSet and Group with initial values.

   //:CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER );
   //:vReportDef.GroupSet.Tag      = "PageFooter"
   SetAttributeFromString( vReportDef, "GroupSet", "Tag", "PageFooter" );
   //:vReportDef.GroupSet.Type     = "PF"
   SetAttributeFromString( vReportDef, "GroupSet", "Type", "PF" );
   //:vReportDef.GroupSet.PSDLG_X  = 0
   SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_X", 0 );
   //:vReportDef.GroupSet.PSDLG_Y  = 64
   SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", 64 );
   //:vReportDef.GroupSet.SZDLG_X  = vReportDef.Page.SizeX
   SetAttributeFromAttribute( vReportDef, "GroupSet", "SZDLG_X", vReportDef, "Page", "SizeX" );
   //:vReportDef.GroupSet.SZDLG_Y  = 320
   SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", 320 );
   //:vReportDef.GroupSet.PPE_Type = 3000
   SetAttributeFromInteger( vReportDef, "GroupSet", "PPE_Type", 3000 );

   //:CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
   //:vReportDef.Group.Tag         = "PageFooter"
   SetAttributeFromString( vReportDef, "Group", "Tag", "PageFooter" );
   //:vReportDef.Group.Type        = "ga"
   SetAttributeFromString( vReportDef, "Group", "Type", "ga" );
   //:vReportDef.Group.SubType     = "0"
   SetAttributeFromString( vReportDef, "Group", "SubType", "0" );
   //:vReportDef.Group.PSDLG_X     = 0
   SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
   //:vReportDef.Group.PSDLG_Y     = 64
   SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", 64 );
   //:vReportDef.Group.SZDLG_X     = vReportDef.Page.SizeX
   SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
   //:vReportDef.Group.SZDLG_Y     = 128
   SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", 128 );
   //:vReportDef.Group.PPE_Type    = 3010
   SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );

   //:// Create Page Footer Controls, if requested.  Footers are requested
   //:// by either having something entered in the Title or by requesting Paging.

   //:IF vUserSpec.UI_Spec.FooterTitle != "" OR
   //:   vUserSpec.UI_Spec.FooterPageNoFlag = "Y"
   if ( CompareAttributeToString( vUserSpec, "UI_Spec", "FooterTitle", "" ) != 0 || CompareAttributeToString( vUserSpec, "UI_Spec", "FooterPageNoFlag", "Y" ) == 0 )
   { 

      //:// Set the FontSize, Bold and Italic flags for the Footer group.
      //:lFontSize = vUserSpec.UI_Spec.FooterFontSize * 10
      GetIntegerFromAttribute( &lTempInteger_10, vUserSpec, "UI_Spec", "FooterFontSize" );
      lFontSize = lTempInteger_10 * 10;
      //:IF vUserSpec.UI_Spec.FooterBoldFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "FooterBoldFlag", "Y" ) == 0 )
      { 
         //:nBoldFlag = TRUE
         nBoldFlag = TRUE;
         //:ELSE
      } 
      else
      { 
         //:nBoldFlag = FALSE
         nBoldFlag = FALSE;
      } 

      //:END
      //:IF vUserSpec.UI_Spec.FooterItalicFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "FooterItalicFlag", "Y" ) == 0 )
      { 
         //:nItalicFlag = TRUE
         nItalicFlag = TRUE;
         //:ELSE
      } 
      else
      { 
         //:nItalicFlag = FALSE
         nItalicFlag = FALSE;
      } 

      //:END

      //:// Set font size and character width and height.
      //:IF lFontSize = 0
      if ( lFontSize == 0 )
      { 
         //:lFontSize = lDefaultFontSize
         lFontSize = lDefaultFontSize;
      } 

      //:END
      //:nCharacterHeight   = ((lFontSize * 16) / 720) + 1
      nCharacterHeight = ( ( lFontSize * 16 ) / 720 ) + 1;
      //:nCharacterWidth    = ((lFontSize * 16) / 720)
      nCharacterWidth = ( ( lFontSize * 16 ) / 720 );
      //:lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16
      lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16;

      //:lCurrentVerticalPos = lTopGroupMargin
      lCurrentVerticalPos = lTopGroupMargin;

      //:IF vUserSpec.UI_Spec.FooterTitle != ""
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "FooterTitle", "" ) != 0 )
      { 

         //:// Set up Page Footer Text.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:vReportDef.Control.Text    = vUserSpec.UI_Spec.FooterTitle
         SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "UI_Spec", "FooterTitle" );
         //:vReportDef.Control.Tag     = "PageFooterText"
         SetAttributeFromString( vReportDef, "Control", "Tag", "PageFooterText" );
         //:vReportDef.Control.PSDLG_X = lLeftGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lLeftGroupMargin );
         //:vReportDef.Control.PSDLG_Y = lTopGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lTopGroupMargin );
         //:SET CURSOR FIRST vPE.ControlDef WHERE
         //:                 vPE.ControlDef.Tag = "Rpt_Text"
         RESULT = SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "Rpt_Text", "" );
         //:INCLUDE vReportDef.ControlDef FROM vPE.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE, "ControlDef", zPOS_AFTER );
         //:lTitleSize = GetStringLength( vReportDef.Control.Text )
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vReportDef, "Control", "Text" );
         lTitleSize = GetStringLength( szTempString_1 );
         //:vReportDef.Control.SZDLG_X = lTitleSize * nCharacterWidth * 16
         lTempInteger_11 = lTitleSize * nCharacterWidth * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_11 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_12 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_12 );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );
      } 

      //:END

      //:IF vUserSpec.UI_Spec.FooterPageNoFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "FooterPageNoFlag", "Y" ) == 0 )
      { 

         //:// Set up Footer Page Number.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:vReportDef.Control.Tag     = "PageNumber"
         SetAttributeFromString( vReportDef, "Control", "Tag", "PageNumber" );
         //:vReportDef.Control.PSDLG_X = lPageNumberPosX
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lPageNumberPosX );
         //:vReportDef.Control.PSDLG_Y = lTopGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lTopGroupMargin );
         //:INCLUDE vReportDef.ControlDef FROM vPE.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE, "ControlDef", zPOS_AFTER );
         //:vReportDef.Control.SZDLG_X = 160  // Just a guess right now.
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", 160 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_13 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_13 );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );
      } 

      //:END

      //:// Set height of the group and groupset.  We will determine the Y size by
      //:// getting the last control (bottom in Group, though any control would do) and
      //:// adding BottomGroupMargin to it.
      //:lHeight = vReportDef.Control.PSDLG_Y + vReportDef.Control.SZDLG_Y +
      //:          lVerticalIncrement + (lBottomHeaderMargin * 2)
      GetIntegerFromAttribute( &lTempInteger_14, vReportDef, "Control", "PSDLG_Y" );
      GetIntegerFromAttribute( &lTempInteger_15, vReportDef, "Control", "SZDLG_Y" );
      lHeight = lTempInteger_14 + lTempInteger_15 + lVerticalIncrement + ( lBottomHeaderMargin * 2 );
      //:vReportDef.Group.SZDLG_Y = lHeight
      SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lHeight );
      //:vReportDef.GroupSet.SZDLG_Y = lHeight
      SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lHeight );
   } 


   //:END

   //:AutodesignReportEntity( vSubtask, vReportDef, vUserSpec, vUserSpecRoot, vPE )
   oTZRPSRCO_AutodesignReportEntity( vSubtask, vReportDef, vUserSpec, vUserSpecRoot, vPE );

   //:// Re-adjust vertical positions of the groupsets.
   //:// Dont adjust the page footer groupset (PF) should it exist
   //:// and rely on the painter and printer to handle its positioning
   //:lCurrentVerticalPos = 0
   lCurrentVerticalPos = 0;
   //:FOR EACH vReportDef.GroupSet
   RESULT = SetCursorFirstEntity( vReportDef, "GroupSet", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vReportDef.GroupSet.Type != "PF"
      if ( CompareAttributeToString( vReportDef, "GroupSet", "Type", "PF" ) != 0 )
      { 

         //:vReportDef.GroupSet.PSDLG_Y = lCurrentVerticalPos
         SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", lCurrentVerticalPos );
         //:lCurrentVerticalPos = lCurrentVerticalPos +
         //:                                       vReportDef.GroupSet.SZDLG_Y
         GetIntegerFromAttribute( &lTempInteger_16, vReportDef, "GroupSet", "SZDLG_Y" );
         lCurrentVerticalPos = lCurrentVerticalPos + lTempInteger_16;
      } 

      RESULT = SetCursorNextEntity( vReportDef, "GroupSet", "" );

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CreatePageFooter( VIEW vReportDef BASED ON LOD TZRPSRCO )

//:   VIEW vDrivingLOD      REGISTERED AS DrivingLOD
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_CreatePageFooter( zVIEW     vReportDef )
{
   zVIEW     vDrivingLOD = 0; 
   zSHORT    RESULT; 
   //:STRING ( 32 ) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szName
   zCHAR     szName[ 33 ] = { 0 }; 
   //:INTEGER lControl
   zLONG     lControl = 0; 
   //:INTEGER lGroupPosY
   zLONG     lGroupPosY = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 

   RESULT = GetViewByName( &vDrivingLOD, "DrivingLOD", vReportDef, zLEVEL_TASK );

   //:TraceLineS("*** In CreateEntityGroupSet *****", "")
   TraceLineS( "*** In CreateEntityGroupSet *****", "" );

   //:szEntityName = vReportDef.DrivingViewObjRef.wGroupSetName
   GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "DrivingViewObjRef", "wGroupSetName", "", 0 );
   //:TraceLineS("*** Group Set Name ***** ", "PageFooter" )
   TraceLineS( "*** Group Set Name ***** ", "PageFooter" );

   //:// First check if the entity already exists.
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PF"
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PF", "" );

   //:// Build the GroupSet for the report from the selected entity
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:TraceLineS("*** Creating Page Header GroupSet *** ", "")
      TraceLineS( "*** Creating Page Header GroupSet *** ", "" );

      //:CREATE ENTITY vReportDef.GroupSet
      RESULT = CreateEntity( vReportDef, "GroupSet", zPOS_AFTER );
      //:vReportDef.GroupSet.Type = "PF"
      SetAttributeFromString( vReportDef, "GroupSet", "Type", "PF" );
      //:vReportDef.GroupSet.Tag = "PageFooter"
      SetAttributeFromString( vReportDef, "GroupSet", "Tag", "PageFooter" );

      //:vReportDef.GroupSet.PSDLG_X  = 0
      SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_X", 0 );
      //:vReportDef.GroupSet.PSDLG_Y  = 4 * 16
      lTempInteger_0 = 4 * 16;
      SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", lTempInteger_0 );
      //:vReportDef.GroupSet.SZDLG_X  = vReportDef.Page.SizeX
      SetAttributeFromAttribute( vReportDef, "GroupSet", "SZDLG_X", vReportDef, "Page", "SizeX" );
      //:// KJS - I don't know what PPE_Type is...
      //:vReportDef.GroupSet.PPE_Type    = 3000
      SetAttributeFromInteger( vReportDef, "GroupSet", "PPE_Type", 3000 );


      //:CREATE ENTITY vReportDef.Group
      RESULT = CreateEntity( vReportDef, "Group", zPOS_AFTER );
      //:vReportDef.Group.Tag = "Footer"
      SetAttributeFromString( vReportDef, "Group", "Tag", "Footer" );
      //:vReportDef.Group.Type = "ga"
      SetAttributeFromString( vReportDef, "Group", "Type", "ga" );

      //:// Should I add a couple of default fields like a text field and page number?


      //:vReportDef.GroupSet.SZDLG_Y  = lGroupPosY + 100
      lTempInteger_1 = lGroupPosY + 100;
      SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lTempInteger_1 );
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CreatePageHeader( VIEW vReportDef BASED ON LOD TZRPSRCO )

//:   VIEW vDrivingLOD      REGISTERED AS DrivingLOD
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_CreatePageHeader( zVIEW     vReportDef )
{
   zVIEW     vDrivingLOD = 0; 
   zSHORT    RESULT; 
   //:STRING ( 32 ) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szName
   zCHAR     szName[ 33 ] = { 0 }; 
   //:INTEGER lControl
   zLONG     lControl = 0; 
   //:INTEGER lGroupPosY
   zLONG     lGroupPosY = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 

   RESULT = GetViewByName( &vDrivingLOD, "DrivingLOD", vReportDef, zLEVEL_TASK );

   //:TraceLineS("*** In CreateEntityGroupSet *****", "")
   TraceLineS( "*** In CreateEntityGroupSet *****", "" );

   //:szEntityName = vReportDef.DrivingViewObjRef.wGroupSetName
   GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "DrivingViewObjRef", "wGroupSetName", "", 0 );
   //:TraceLineS("*** Group Set Name ***** ", "PageHeader" )
   TraceLineS( "*** Group Set Name ***** ", "PageHeader" );

   //:// First check if the entity already exists.
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PH"
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PH", "" );

   //:// Build the GroupSet for the report from the selected entity
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:TraceLineS("*** Creating Page Header GroupSet *** ", "")
      TraceLineS( "*** Creating Page Header GroupSet *** ", "" );

      //:CREATE ENTITY vReportDef.GroupSet
      RESULT = CreateEntity( vReportDef, "GroupSet", zPOS_AFTER );
      //:vReportDef.GroupSet.Tag = "PageHeader"
      SetAttributeFromString( vReportDef, "GroupSet", "Tag", "PageHeader" );
      //:vReportDef.GroupSet.Type = "PH"
      SetAttributeFromString( vReportDef, "GroupSet", "Type", "PH" );

      //:vReportDef.GroupSet.PSDLG_X  = 0
      SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_X", 0 );
      //:vReportDef.GroupSet.PSDLG_Y  = 4 * 16
      lTempInteger_0 = 4 * 16;
      SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", lTempInteger_0 );
      //:vReportDef.GroupSet.SZDLG_X  = vReportDef.Page.SizeX
      SetAttributeFromAttribute( vReportDef, "GroupSet", "SZDLG_X", vReportDef, "Page", "SizeX" );
      //:// KJS - I don't know what PPE_Type is...
      //:vReportDef.GroupSet.PPE_Type    = 3000
      SetAttributeFromInteger( vReportDef, "GroupSet", "PPE_Type", 3000 );


      //:CREATE ENTITY vReportDef.Group
      RESULT = CreateEntity( vReportDef, "Group", zPOS_AFTER );
      //:vReportDef.Group.Tag = "Header"
      SetAttributeFromString( vReportDef, "Group", "Tag", "Header" );
      //:vReportDef.Group.Type = "ga"
      SetAttributeFromString( vReportDef, "Group", "Type", "ga" );

      //:// Should I add a couple of default fields like a heading and page number?


      //:vReportDef.GroupSet.SZDLG_Y  = lGroupPosY + 100
      lTempInteger_1 = lGroupPosY + 100;
      SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lTempInteger_1 );
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DeleteGroupSet( VIEW vReportDef BASED ON LOD TZRPSRCO )

//:   VIEW vReportOutliner  BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_DeleteGroupSet( zVIEW     vReportDef )
{
   zVIEW     vReportOutliner = 0; 
   //:VIEW vReportDefRecurs BASED ON LOD TZRPSRCO
   zVIEW     vReportDefRecurs = 0; 
   //:VIEW vDrivingLOD      BASED ON LOD TZZOLODO
   zVIEW     vDrivingLOD = 0; 
   //:STRING ( 32 ) szEntity
   zCHAR     szEntity[ 33 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:TraceLineS("*** DeleteGroupSet *** ", "")
   TraceLineS( "*** DeleteGroupSet *** ", "" );

   //:nRC = GetViewByName( vReportOutliner, "TZRPTOUTLINER", vReportDef, zLEVEL_TASK )
   nRC = GetViewByName( &vReportOutliner, "TZRPTOUTLINER", vReportDef, zLEVEL_TASK );

   //:IF nRC >= 0
   if ( nRC >= 0 )
   { 
      //:szEntity = vReportOutliner.PartialReportEntity.Name
      GetVariableFromAttribute( szEntity, 0, 'S', 33, vReportOutliner, "PartialReportEntity", "Name", "", 0 );
      //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szEntity
      RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szEntity, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:TraceLineS("*** DELETING groupset *** ", szEntity )
         TraceLineS( "*** DELETING groupset *** ", szEntity );
         //:DeleteEntity( vReportDef, "GroupSet", zREPOS_NEXT )
         DeleteEntity( vReportDef, "GroupSet", zREPOS_NEXT );
      } 

      //:END
   } 

   //:END

   //:DropView( vReportOutliner )
   DropView( vReportOutliner );

   //:nRC = GetViewByName( vDrivingLOD, "DrivingLOD", vReportDef, zLEVEL_TASK )
   nRC = GetViewByName( &vDrivingLOD, "DrivingLOD", vReportDef, zLEVEL_TASK );

   //:// Need to look at attributes as well?
   //:IF vReportDef.PartialReportEntity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "PartialReportEntity" );
   if ( lTempInteger_0 == 0 )
   { 
      //:DELETE ENTITY vReportDef.PartialReportEntity
      RESULT = DeleteEntity( vReportDef, "PartialReportEntity", zPOS_NEXT );
   } 

   //:END

   //:CreateViewFromView( vReportDefRecurs, vReportDef )
   CreateViewFromView( &vReportDefRecurs, vReportDef );
   //:NAME VIEW vReportDefRecurs "vReportDefRecurs"
   SetNameForView( vReportDefRecurs, "vReportDefRecurs", 0, zLEVEL_TASK );

   //:nRC = CrtePartialLODStructR( vReportDef,
   //:                             vReportDefRecurs,
   //:                             vDrivingLOD )
   nRC = oTZRPSRCO_CrtePartialLODStructR( vReportDef, vReportDefRecurs, vDrivingLOD );

   //://DropView( vReportDefRecurs )

   //:// Now run through and delete all entities with no D or S.
   //://CreateViewFromView( vReportDefRecurs, vReportDef )
   //:DeleteNonDisplayEntsR( vReportDefRecurs )
   oTZRPSRCO_DeleteNonDisplayEntsR( vReportDefRecurs );
   //:DropView( vReportDefRecurs )
   DropView( vReportDefRecurs );

   //:CreateViewFromView( vReportOutliner, vReportDef )
   CreateViewFromView( &vReportOutliner, vReportDef );
   //:NAME VIEW  vReportOutliner  "TZRPTOUTLINER"
   SetNameForView( vReportOutliner, "TZRPTOUTLINER", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CreateFieldsForGrpSet( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                       VIEW vDrivingLOD BASED ON LOD TZZOLODO )

//:   VIEW vPE_Control   BASED ON LOD TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_CreateFieldsForGrpSet( zVIEW     vReportDef,
                                 zVIEW     vDrivingLOD )
{
   zVIEW     vPE_Control = 0; 
   //:VIEW       vDefaults BASED ON LOD TZADCSDO
   zVIEW     vDefaults = 0; 
   //:STRING(32) szNameWork
   zCHAR     szNameWork[ 33 ] = { 0 }; 
   //:STRING(32) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:INTEGER    lVerticalPos
   zLONG     lVerticalPos = 0; 
   //:INTEGER    lHorizontalPos
   zLONG     lHorizontalPos = 0; 
   //:INTEGER    lLongestPrompt
   zLONG     lLongestPrompt = 0; 
   //:INTEGER    lTopGroupMargin
   zLONG     lTopGroupMargin = 0; 
   //:INTEGER    lBottomGroupMargin
   zLONG     lBottomGroupMargin = 0; 
   //:INTEGER    lBottomHeaderMargin
   zLONG     lBottomHeaderMargin = 0; 
   //:INTEGER    lLeftGroupMargin
   zLONG     lLeftGroupMargin = 0; 
   //:INTEGER    lRightGroupMargin
   zLONG     lRightGroupMargin = 0; 
   //:INTEGER    lVerticalIncrement     // space betwen lines
   zLONG     lVerticalIncrement = 0; 
   //:INTEGER    lHorizontalIncrement
   zLONG     lHorizontalIncrement = 0; 
   //:INTEGER    lAttributeCnt
   zLONG     lAttributeCnt = 0; 
   //:INTEGER    lLevelIndentation
   zLONG     lLevelIndentation = 0; 
   //:INTEGER    lGroupPosY
   zLONG     lGroupPosY = 0; 
   //:INTEGER    lGroupSizeY
   zLONG     lGroupSizeY = 0; 
   //:INTEGER    lTitleSize
   zLONG     lTitleSize = 0; 
   //:INTEGER    lDefaultFontSize
   zLONG     lDefaultFontSize = 0; 
   //:INTEGER    lPageWidth
   zLONG     lPageWidth = 0; 
   //:INTEGER    lFontSize
   zLONG     lFontSize = 0; 
   //:DECIMAL    dHorzScalingFactor
   ZDecimal  dHorzScalingFactor = 0.0; 
   //:DECIMAL    dVertScalingFactor
   ZDecimal  dVertScalingFactor = 0.0; 
   //:SHORT      nCharacterWidth
   zSHORT    nCharacterWidth = 0; 
   //:SHORT      nCharacterHeight
   zSHORT    nCharacterHeight = 0; 
   //:SHORT      nBoldFlag
   zSHORT    nBoldFlag = 0; 
   //:SHORT      nItalicFlag
   zSHORT    nItalicFlag = 0; 
   //:INTEGER    lAttributeSize
   zLONG     lAttributeSize = 0; 
   //:STRING(2)  szDomainDataType
   zCHAR     szDomainDataType[ 3 ] = { 0 }; 
   //:STRING(2)  szAttributePrefix
   zCHAR     szAttributePrefix[ 3 ] = { 0 }; 
   //:STRING(2)  szPositioningType
   zCHAR     szPositioningType[ 3 ] = { 0 }; 
   //:INTEGER    nControlEndPosition
   zLONG     nControlEndPosition = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zCHAR     szTempString_1[ 33 ]; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zLONG     lTempInteger_8; 



   //:IF vReportDef.DrivingViewObjRef.wGroupHeader = "Y"
   if ( CompareAttributeToString( vReportDef, "DrivingViewObjRef", "wGroupHeader", "Y" ) == 0 )
   { 

      //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "gh"
      RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "gh", "" );

      //:// Add Header Group
      //://CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER )
      //:CREATE ENTITY vReportDef.Group
      RESULT = CreateEntity( vReportDef, "Group", zPOS_AFTER );
      //:vReportDef.Group.Tag                  = "GroupHeader"
      SetAttributeFromString( vReportDef, "Group", "Tag", "GroupHeader" );
      //:vReportDef.Group.Type                 = "gh"
      SetAttributeFromString( vReportDef, "Group", "Type", "gh" );
      //:vReportDef.Group.PSDLG_X              = 0
      SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
      //:vReportDef.Group.PSDLG_Y              = lGroupPosY
      SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
      //:vReportDef.Group.SZDLG_X              = vReportDef.Page.SizeX
      SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
      //:vReportDef.Group.PPE_Type             = 3010
      SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );
      //://vReportDef.Group.ForceHeaderPerEntity = vUserSpec.UIS_Entity.ForceHeaderPerEntity

      //:lVerticalPos = lTopGroupMargin
      lVerticalPos = lTopGroupMargin;

      //:// Initialize our horizontal and vertical positions.  Actually,
      //:// the vertical position won't change.
      //:lHorizontalPos = lLeftGroupMargin
      lHorizontalPos = lLeftGroupMargin;

      //:// Start the attribute count with 1 for the generation of unique names.
      //:lAttributeCnt         = 1
      lAttributeCnt = 1;

      //:FOR EACH vDrivingLOD.LOD_AttributeRec
      RESULT = SetCursorFirstEntity( vDrivingLOD, "LOD_AttributeRec", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:// For each attribute create the title. Control Type is 10 for title.
         //://CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         //:CREATE ENTITY vReportDef.Control
         RESULT = CreateEntity( vReportDef, "Control", zPOS_AFTER );
         //:ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 )
         ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 );
         //:szNameWork                 = vDrivingLOD.ER_AttributeRec.Name
         GetVariableFromAttribute( szNameWork, 0, 'S', 33, vDrivingLOD, "ER_AttributeRec", "Name", "", 0 );
         //:     vReportDef.Control.Tag     = "T_" + szAttributePrefix + "_" + szNameWork[1:26]
         ZeidonStringCopy( szTempString_0, 1, 0, "T_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, szAttributePrefix, 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, "_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_0, 1, 0, szNameWork, 1, 26, 33 );
         SetAttributeFromString( vReportDef, "Control", "Tag", szTempString_0 );
         //:vReportDef.Control.Type    = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:IF vDrivingLOD.ER_AttributeRec.ListPaintLit != ""
         if ( CompareAttributeToString( vDrivingLOD, "ER_AttributeRec", "ListPaintLit", "" ) != 0 )
         { 
            //:vReportDef.Control.Text = vDrivingLOD.ER_AttributeRec.ListPaintLit
            SetAttributeFromAttribute( vReportDef, "Control", "Text", vDrivingLOD, "ER_AttributeRec", "ListPaintLit" );
            //:ELSE
         } 
         else
         { 
            //:vReportDef.Control.Text = vDrivingLOD.ER_AttributeRec.Name
            SetAttributeFromAttribute( vReportDef, "Control", "Text", vDrivingLOD, "ER_AttributeRec", "Name" );
         } 

         //:END

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, FALSE,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, FALSE, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //://lTitleSize                 = GetMaxAttributeSize( vUserSpec, "H" )
         //:lTitleSize                 = lTitleSize * nCharacterWidth
         lTitleSize = lTitleSize * nCharacterWidth;
         //:vReportDef.Control.SZDLG_X = lTitleSize * 16
         lTempInteger_0 = lTitleSize * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_0 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_1 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_1 );

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );

         //:lHorizontalPos  = lHorizontalPos + (lTitleSize * 16) +
         //:                                               lHorizontalIncrement
         lHorizontalPos = lHorizontalPos + ( lTitleSize * 16 ) + lHorizontalIncrement;
         //:lAttributeCnt = lAttributeCnt + 1
         lAttributeCnt = lAttributeCnt + 1;
         RESULT = SetCursorNextEntity( vDrivingLOD, "LOD_AttributeRec", "" );
      } 


      //:END

      //:// Determine the size of the Group.
      //:// 1. We will determine the X size by getting the last control (right most in
      //://    Group) and adding its size and RightGroupMargin to it.
      //:// 2. We will determine the Y size by getting the last control (bottom in Group,
      //://    though any control would do) and adding BottomGroupMargin to it.
      //:SET CURSOR LAST vReportDef.Control
      RESULT = SetCursorLastEntity( vReportDef, "Control", "" );
      //:lGroupSizeY = vReportDef.Control.PSDLG_Y + vReportDef.Control.SZDLG_Y +
      //:              lVerticalIncrement + lBottomHeaderMargin
      GetIntegerFromAttribute( &lTempInteger_2, vReportDef, "Control", "PSDLG_Y" );
      GetIntegerFromAttribute( &lTempInteger_3, vReportDef, "Control", "SZDLG_Y" );
      lGroupSizeY = lTempInteger_2 + lTempInteger_3 + lVerticalIncrement + lBottomHeaderMargin;
      //:vReportDef.Group.SZDLG_Y    = lGroupSizeY
      SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lGroupSizeY );
      //:vReportDef.GroupSet.SZDLG_Y = lGroupSizeY
      SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lGroupSizeY );

      //:// update lGroupPosY
      //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
      GetIntegerFromAttribute( &lTempInteger_4, vReportDef, "Group", "SZDLG_Y" );
      lGroupPosY = lGroupPosY + lTempInteger_4;

      //:// We will now adjust the Group control and all the subcontrols by a common
      //:// factor so that they will all appear on the page.
      //:AdjustGroupSize( vReportDef, lPageWidth )
      oTZRPSRCO_AdjustGroupSize( vReportDef, lPageWidth );
   } 


   //:END


   //:// *** REGULAR ATTRIBUTES  ***

   //:// Set the Bold and Italic flags for the main group.
   //:nBoldFlag = FALSE
   nBoldFlag = FALSE;

   //:// Set font size and character width and height.
   //://lFontSize = vUserSpec.UIS_Entity.FontSize * 10
   //:lFontSize = 1 * 10  // KJS test
   lFontSize = 1 * 10;
   //:IF lFontSize = 0
   if ( lFontSize == 0 )
   { 
      //:lFontSize = lDefaultFontSize
      lFontSize = lDefaultFontSize;
   } 

   //:END
   //:nCharacterWidth  = (lFontSize * 16) / 720
   nCharacterWidth = ( lFontSize * 16 ) / 720;
   //:nCharacterHeight = ((lFontSize * 16) / 720) + 1
   nCharacterHeight = ( ( lFontSize * 16 ) / 720 ) + 1;
   //://   lVerticalIncrement = nCharacterHeight / 2
   //://   lVerticalIncrement = 0  // DEM -guess work
   //:lHorizontalIncrement = nCharacterWidth  * dHorzScalingFactor * 16
   lHorizontalIncrement = nCharacterWidth * dHorzScalingFactor * 16;
   //:lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16
   lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16;

   //:// Build the Group for the attribute Controls (and the titles, if
   //:// positioning is vertical).
   //://CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER )
   //:CREATE ENTITY vReportDef.Group
   RESULT = CreateEntity( vReportDef, "Group", zPOS_AFTER );
   //:vReportDef.Group.Tag      = "Detail"
   SetAttributeFromString( vReportDef, "Group", "Tag", "Detail" );
   //:vReportDef.Group.Type     = "ga"
   SetAttributeFromString( vReportDef, "Group", "Type", "ga" );
   //:vReportDef.Group.PSDLG_X  = 0
   SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
   //:vReportDef.Group.PSDLG_Y  = lGroupPosY
   SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
   //:vReportDef.Group.SZDLG_X  = vReportDef.Page.SizeX
   SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
   //:vReportDef.Group.PPE_Type = 3010
   SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );

   //:// Start the attribute count with 1 for the generation of unique names.
   //:lAttributeCnt = 1
   lAttributeCnt = 1;

   //:// Initialize positions.
   //:lVerticalPos     = lTopGroupMargin
   lVerticalPos = lTopGroupMargin;
   //:lHorizontalPos   = lLeftGroupMargin
   lHorizontalPos = lLeftGroupMargin;

   //:// Loop through all the attributes creating both Title controls and Attribute controls.
   //:// We use the same code for both horizontal position and vertical positioning, the
   //:// only difference being the positioning of both types of controls.
   //:FOR EACH vDrivingLOD.LOD_AttributeRec
   RESULT = SetCursorFirstEntity( vDrivingLOD, "LOD_AttributeRec", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// The ControlDef is always Text at this time.
      //:SET CURSOR FIRST vPE_Control.ControlDef WHERE
      //:                 vPE_Control.ControlDef.Tag = "Rpt_Text"
      RESULT = SetCursorFirstEntityByString( vPE_Control, "ControlDef", "Tag", "Rpt_Text", "" );

      //:// Set up szNameWork and szAttributePrefix for use in creating Tags.
      //:szNameWork = vDrivingLOD.ER_AttributeRec.Name
      GetVariableFromAttribute( szNameWork, 0, 'S', 33, vDrivingLOD, "ER_AttributeRec", "Name", "", 0 );
      //:ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 )
      ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 );

      //:// For each attribute create the control. Control Type is 20 for control.
      //://CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
      //:CREATE ENTITY vReportDef.Control
      RESULT = CreateEntity( vReportDef, "Control", zPOS_AFTER );
      //:vReportDef.Control.Tag  = "A_" + szAttributePrefix + "_" + szNameWork[1:26]
      ZeidonStringCopy( szTempString_1, 1, 0, "A_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, szAttributePrefix, 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, "_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_1, 1, 0, szNameWork, 1, 26, 33 );
      SetAttributeFromString( vReportDef, "Control", "Tag", szTempString_1 );
      //:vReportDef.Control.Type = 20
      SetAttributeFromInteger( vReportDef, "Control", "Type", 20 );

      //:// Right justify decimals, shorts and longs.  Subtype = 16777216 sets the
      //:// bit zCONTROL_RIGHTJUSTIFY.
      //://szDomainDataType = vUserSpec.M_Domain.DataType
      //:IF szDomainDataType = "M" OR
      //:   szDomainDataType = "N" OR
      //:   szDomainDataType = "L"
      if ( ZeidonStringCompare( szDomainDataType, 1, 0, "M", 1, 0, 3 ) == 0 || ZeidonStringCompare( szDomainDataType, 1, 0, "N", 1, 0, 3 ) == 0 || ZeidonStringCompare( szDomainDataType, 1, 0, "L", 1, 0, 3 ) == 0 )
      { 
         //:   vReportDef.Control.Subtype = 16777216
         SetAttributeFromInteger( vReportDef, "Control", "Subtype", 16777216 );
      } 

      //:END

      //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
      RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );

      //://CreateMetaEntity( vSubtask, vReportDef, "CtrlMap", zPOS_AFTER )
      //:CREATE ENTITY vReportDef.CtrlMap
      RESULT = CreateEntity( vReportDef, "CtrlMap", zPOS_AFTER );
      //:INCLUDE vReportDef.CtrlMapLOD_Attribute FROM vDrivingLOD.LOD_AttributeRec
      RESULT = IncludeSubobjectFromSubobject( vReportDef, "CtrlMapLOD_Attribute", vDrivingLOD, "LOD_AttributeRec", zPOS_AFTER );
      //:INCLUDE vReportDef.CtrlMapView FROM vReportDef.DrivingViewObjRef
      RESULT = IncludeSubobjectFromSubobject( vReportDef, "CtrlMapView", vReportDef, "DrivingViewObjRef", zPOS_AFTER );

      //:// KJS - Assuming we are always doing horizontal...
      //:// Now determine the control position and increment the positions, based on
      //:// horizontal or vertical positioning.
      //://IF szPositioningType = "H"
      //:   vReportDef.Control.PSDLG_X = lHorizontalPos
      SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
      //:   vReportDef.Control.PSDLG_Y = lVerticalPos
      SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
      //:   //lAttributeSize             = GetMaxAttributeSize( vUserSpec, "H" )
      //:   lAttributeSize             = lAttributeSize * nCharacterWidth
      lAttributeSize = lAttributeSize * nCharacterWidth;
      //:   vReportDef.Control.SZDLG_X = lAttributeSize * 16
      lTempInteger_5 = lAttributeSize * 16;
      SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_5 );
      //:   vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
      lTempInteger_6 = nCharacterHeight * 16;
      SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_6 );

      //:   lHorizontalPos = lHorizontalPos + (lAttributeSize * 16) +
      //:                                                   lHorizontalIncrement
      lHorizontalPos = lHorizontalPos + ( lAttributeSize * 16 ) + lHorizontalIncrement;
      //://END

      //:// Set Bold and Italic flags and Fontsize.
      //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, FALSE,
      //:                      FALSE, FALSE, 0, lFontSize,
      //:                      0, 0, 0, 0, 0, 0, 0 )
      TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, FALSE, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

      //:lAttributeCnt = lAttributeCnt + 1
      lAttributeCnt = lAttributeCnt + 1;
      RESULT = SetCursorNextEntity( vDrivingLOD, "LOD_AttributeRec", "" );
   } 


   //:END

   //://IF szPositioningType = "H"
   //:   // update vertical position
   //:   lVerticalPos = lVerticalPos + lVerticalIncrement +
   //:                                 vReportDef.Control.SZDLG_Y
   GetIntegerFromAttribute( &lTempInteger_7, vReportDef, "Control", "SZDLG_Y" );
   lVerticalPos = lVerticalPos + lVerticalIncrement + lTempInteger_7;
   //://END

   //:// Determine the size of the Group.
   //:// 1. We will determine the X size by getting the largest end of any control
   //://    in the Group (ControlSize + ControlPosX) and adding RightGroupMargin to it.
   //:// 2. We will determine the Y size by getting the last control (bottom in Group)
   //://    and adding BottomGroupMargin to it.

   //:// If positioning is vertical add a bit of space to open up between
   //:// instances.
   //:   lGroupSizeY = lVerticalPos + lVerticalIncrement + lBottomGroupMargin
   lGroupSizeY = lVerticalPos + lVerticalIncrement + lBottomGroupMargin;

   //:vReportDef.Group.SZDLG_Y = lGroupSizeY
   SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lGroupSizeY );

   //:// update lGroupPosY
   //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
   GetIntegerFromAttribute( &lTempInteger_8, vReportDef, "Group", "SZDLG_Y" );
   lGroupPosY = lGroupPosY + lTempInteger_8;

   //:// We will now adjust the Group control and all the subcontrols by a common
   //:// factor so that they will all appear on the page.
   //:AdjustGroupSize( vReportDef, lPageWidth )
   oTZRPSRCO_AdjustGroupSize( vReportDef, lPageWidth );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DeleteNonDisplayEntsR( VIEW vReportDefRecurs BASED ON LOD TZRPSRCO )

//:   INTEGER nRC
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_DeleteNonDisplayEntsR( zVIEW     vReportDefRecurs )
{
   zLONG     nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:FOR EACH vReportDefRecurs.PartialReportEntity
   RESULT = SetCursorFirstEntity( vReportDefRecurs, "PartialReportEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:IF vReportDefRecurs.PartialReportEntity.ReportDisplayFlag = ""
      if ( CompareAttributeToString( vReportDefRecurs, "PartialReportEntity", "ReportDisplayFlag", "" ) == 0 )
      { 
         //:DELETE ENTITY vReportDefRecurs.PartialReportEntity NONE
         RESULT = DeleteEntity( vReportDefRecurs, "PartialReportEntity", zREPOS_NONE );
         //:ELSE
      } 
      else
      { 
         //:// Process any LOD subentities.
         //:IF vReportDefRecurs.PartialReportEntityChild EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( vReportDefRecurs, "PartialReportEntityChild" );
         if ( lTempInteger_0 == 0 )
         { 
            //:SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" )
            SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" );
            //:nRC = DeleteNonDisplayEntsR( vReportDefRecurs )
            nRC = oTZRPSRCO_DeleteNonDisplayEntsR( vReportDefRecurs );
            //:ResetViewFromSubobject( vReportDefRecurs )
            ResetViewFromSubobject( vReportDefRecurs );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vReportDefRecurs, "PartialReportEntity", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CrtePartialLODStructR( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                       VIEW vReportDefRecurs BASED ON LOD TZRPSRCO,
//:                       VIEW vDrivingLOD      BASED ON LOD TZZOLODO )

//:   STRING ( 32 ) szDrivingEntityName
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_CrtePartialLODStructR( zVIEW     vReportDef,
                                 zVIEW     vReportDefRecurs,
                                 zVIEW     vDrivingLOD )
{
   zCHAR     szDrivingEntityName[ 33 ] = { 0 }; 
   //:STRING ( 1 )  szHasGroupSetFlag
   zCHAR     szHasGroupSetFlag[ 2 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:// Build one level of the PartialReportEntity subobject, matching the structure of the Driving LOD and
   //:// setting the ReportDisplayFlag entity for any entity with a corresponding GroupSet entity in the report, or having
   //:// a PartialReportEntityChild entity with a corresponding GroupSet entity. The flag is set to "D" if the entity has
   //:// a corresponding GroupSet entity and to a "C" if it has a child with a corresponding GroupSet entity.

   //:FOR EACH vDrivingLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( vDrivingLOD, "LOD_EntityParent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:szDrivingEntityName = vDrivingLOD.LOD_EntityParent.Name
      GetVariableFromAttribute( szDrivingEntityName, 0, 'S', 33, vDrivingLOD, "LOD_EntityParent", "Name", "", 0 );

      //:TraceLineS("*** Creating PartialReportEntity - ", szDrivingEntityName )
      TraceLineS( "*** Creating PartialReportEntity - ", szDrivingEntityName );

      //:// Create corresponding PartialReportEntity entity.
      //:CREATE ENTITY vReportDefRecurs.PartialReportEntity
      RESULT = CreateEntity( vReportDefRecurs, "PartialReportEntity", zPOS_AFTER );
      //:vReportDefRecurs.PartialReportEntity.Name = szDrivingEntityName
      SetAttributeFromString( vReportDefRecurs, "PartialReportEntity", "Name", szDrivingEntityName );

      //:// Process corresponding report GroupSet entity, if there is one
      //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szDrivingEntityName
      RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szDrivingEntityName, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:vReportDefRecurs.PartialReportEntity.ReportDisplayFlag = "D"
         SetAttributeFromString( vReportDefRecurs, "PartialReportEntity", "ReportDisplayFlag", "D" );
         //:szHasGroupSetFlag = "Y"
         ZeidonStringCopy( szHasGroupSetFlag, 1, 0, "Y", 1, 0, 2 );
      } 

      //:END

      //:// Process any LOD subentities.
      //:IF vDrivingLOD.LOD_EntityChild EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vDrivingLOD, "LOD_EntityChild" );
      if ( lTempInteger_0 == 0 )
      { 
         //:SetViewToSubobject( vDrivingLOD, "LOD_EntityChild" )
         SetViewToSubobject( vDrivingLOD, "LOD_EntityChild" );
         //:SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" )
         SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" );
         //:nRC = CrtePartialLODStructR( vReportDef,
         //:                             vReportDefRecurs,
         //:                             vDrivingLOD )
         nRC = oTZRPSRCO_CrtePartialLODStructR( vReportDef, vReportDefRecurs, vDrivingLOD );
         //:ResetViewFromSubobject( vDrivingLOD )
         ResetViewFromSubobject( vDrivingLOD );
         //:ResetViewFromSubobject( vReportDefRecurs )
         ResetViewFromSubobject( vReportDefRecurs );

         //:// If the current entity does not have the Display Flag set to "D" but the return code indicates a child
         //:// entity does have its Display Flag set to "D", then set the Display Flag to "S" to indicate this condition.
         //:IF vReportDefRecurs.PartialReportEntity.ReportDisplayFlag = "" AND nRC = 1
         if ( CompareAttributeToString( vReportDefRecurs, "PartialReportEntity", "ReportDisplayFlag", "" ) == 0 && nRC == 1 )
         { 
            //:vReportDefRecurs.PartialReportEntity.ReportDisplayFlag = "S"
            SetAttributeFromString( vReportDefRecurs, "PartialReportEntity", "ReportDisplayFlag", "S" );
         } 

         //:END

         //:// If any child has its Display Flag set, so indicate to the calling operation.
         //:IF nRC = 1
         if ( nRC == 1 )
         { 
            //: szHasGroupSetFlag = "Y"
            ZeidonStringCopy( szHasGroupSetFlag, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vDrivingLOD, "LOD_EntityParent", "" );
      //:END
   } 

   //:END

   //:IF szHasGroupSetFlag = "Y"
   if ( ZeidonStringCompare( szHasGroupSetFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:RETURN 1     // Indicate a child has a corresponding GroupSet entity.
      return( 1 );
      //:ELSE
   } 
   else
   { 
      //:RETURN 0
      return( 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:CreateEntityGroupSet( VIEW vReportDef  BASED ON LOD TZRPSRCO )

//:   VIEW vDrivingLOD      REGISTERED AS DrivingLOD
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_CreateEntityGroupSet( zVIEW     vReportDef )
{
   zVIEW     vDrivingLOD = 0; 
   zSHORT    RESULT; 
   //:VIEW vReportDefRecurs BASED ON LOD  TZRPSRCO
   zVIEW     vReportDefRecurs = 0; 
   //:STRING ( 32 ) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szName
   zCHAR     szName[ 33 ] = { 0 }; 
   //:INTEGER lControl
   zLONG     lControl = 0; 
   //:INTEGER lGroupPosY
   zLONG     lGroupPosY = 0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zSHORT    lTempInteger_5; 

   RESULT = GetViewByName( &vDrivingLOD, "DrivingLOD", vReportDef, zLEVEL_TASK );

   //:TraceLineS("*** In CreateEntityGroupSet *****", "")
   TraceLineS( "*** In CreateEntityGroupSet *****", "" );

   //:szEntityName = vReportDef.DrivingViewObjRef.wGroupSetName
   GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "DrivingViewObjRef", "wGroupSetName", "", 0 );
   //:TraceLineS("*** Group Set Name ***** ", szEntityName)
   TraceLineS( "*** Group Set Name ***** ", szEntityName );

   //:// First check if the entity already exists.
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szEntityName
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szEntityName, "" );

   //:// Build the GroupSet for the report from the selected entity
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:TraceLineS("*** Creating GroupSet *** ", "")
      TraceLineS( "*** Creating GroupSet *** ", "" );

      //:CREATE ENTITY vReportDef.GroupSet
      RESULT = CreateEntity( vReportDef, "GroupSet", zPOS_AFTER );
      //:vReportDef.GroupSet.Tag = szEntityName
      SetAttributeFromString( vReportDef, "GroupSet", "Tag", szEntityName );

      //:vReportDef.GroupSet.PSDLG_X  = 0
      SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_X", 0 );
      //:vReportDef.GroupSet.PSDLG_Y  = 4 * 16
      lTempInteger_0 = 4 * 16;
      SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", lTempInteger_0 );
      //:vReportDef.GroupSet.SZDLG_X  = vReportDef.Page.SizeX
      SetAttributeFromAttribute( vReportDef, "GroupSet", "SZDLG_X", vReportDef, "Page", "SizeX" );
      //:// KJS - I don't know what PPE_Type is...
      //:vReportDef.GroupSet.PPE_Type    = 3000
      SetAttributeFromInteger( vReportDef, "GroupSet", "PPE_Type", 3000 );


      //:IF vReportDef.DrivingViewObjRef.wGroupSetType = "PH" OR vReportDef.DrivingViewObjRef.wPageFooter = "PF"
      if ( CompareAttributeToString( vReportDef, "DrivingViewObjRef", "wGroupSetType", "PH" ) == 0 || CompareAttributeToString( vReportDef, "DrivingViewObjRef", "wPageFooter", "PF" ) == 0 )
      { 

         //:vReportDef.GroupSet.Type = vReportDef.DrivingViewObjRef.wGroupSetType
         SetAttributeFromAttribute( vReportDef, "GroupSet", "Type", vReportDef, "DrivingViewObjRef", "wGroupSetType" );
         //:IF vReportDef.GroupSet.Type = "PH"
         if ( CompareAttributeToString( vReportDef, "GroupSet", "Type", "PH" ) == 0 )
         { 
            //:szName = "PageHeader"
            ZeidonStringCopy( szName, 1, 0, "PageHeader", 1, 0, 33 );
            //:ELSE
         } 
         else
         { 
            //:szName = "PageFooter"
            ZeidonStringCopy( szName, 1, 0, "PageFooter", 1, 0, 33 );
         } 

         //:END
         //:IF vReportDef.GroupSet.Tag = ""
         if ( CompareAttributeToString( vReportDef, "GroupSet", "Tag", "" ) == 0 )
         { 
            //:vReportDef.GroupSet.Tag = szName
            SetAttributeFromString( vReportDef, "GroupSet", "Tag", szName );
         } 

         //:END
         //:CREATE ENTITY vReportDef.Group
         RESULT = CreateEntity( vReportDef, "Group", zPOS_AFTER );
         //:vReportDef.Group.Tag = "Header"
         SetAttributeFromString( vReportDef, "Group", "Tag", "Header" );
         //:vReportDef.Group.Type = "ga"
         SetAttributeFromString( vReportDef, "Group", "Type", "ga" );

         //:ELSE
      } 
      else
      { 
         //:vReportDef.GroupSet.Type = "E"
         SetAttributeFromString( vReportDef, "GroupSet", "Type", "E" );
         //:lGroupPosY = 0
         lGroupPosY = 0;

         //:// See if there is a group header
         //:IF vReportDef.DrivingViewObjRef.wGroupHeader = "Y"
         if ( CompareAttributeToString( vReportDef, "DrivingViewObjRef", "wGroupHeader", "Y" ) == 0 )
         { 
            //:CREATE ENTITY vReportDef.Group
            RESULT = CreateEntity( vReportDef, "Group", zPOS_AFTER );
            //:vReportDef.Group.Tag = "Header"
            SetAttributeFromString( vReportDef, "Group", "Tag", "Header" );
            //:vReportDef.Group.Type = "gh"
            SetAttributeFromString( vReportDef, "Group", "Type", "gh" );
            //:vReportDef.Group.PSDLG_X              = 0
            SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
            //:vReportDef.Group.PSDLG_Y              = lGroupPosY
            SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
            //:vReportDef.Group.SZDLG_X              = vReportDef.Page.SizeX
            SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
            //:vReportDef.Group.SZDLG_Y = 50
            SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", 50 );
            //:vReportDef.Group.PPE_Type             = 3010
            SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );
            //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
            GetIntegerFromAttribute( &lTempInteger_1, vReportDef, "Group", "SZDLG_Y" );
            lGroupPosY = lGroupPosY + lTempInteger_1;
         } 

         //:END

         //:// Create group detail
         //:CREATE ENTITY vReportDef.Group
         RESULT = CreateEntity( vReportDef, "Group", zPOS_AFTER );
         //:vReportDef.Group.Tag = "Detail"
         SetAttributeFromString( vReportDef, "Group", "Tag", "Detail" );
         //:vReportDef.Group.Type = "ga"
         SetAttributeFromString( vReportDef, "Group", "Type", "ga" );
         //:vReportDef.Group.PSDLG_X              = 0
         SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
         //:vReportDef.Group.PSDLG_Y              = lGroupPosY
         SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
         //:vReportDef.Group.SZDLG_X              = vReportDef.Page.SizeX
         SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
         //:vReportDef.Group.SZDLG_Y = 50
         SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", 50 );
         //:vReportDef.Group.PPE_Type             = 3010
         SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );
         //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
         GetIntegerFromAttribute( &lTempInteger_2, vReportDef, "Group", "SZDLG_Y" );
         lGroupPosY = lGroupPosY + lTempInteger_2;

         //:// See if there is a group footer.
         //:IF vReportDef.DrivingViewObjRef.wGroupFooter = "Y"
         if ( CompareAttributeToString( vReportDef, "DrivingViewObjRef", "wGroupFooter", "Y" ) == 0 )
         { 
            //:CREATE ENTITY vReportDef.Group
            RESULT = CreateEntity( vReportDef, "Group", zPOS_AFTER );
            //:vReportDef.Group.Tag = "Footer"
            SetAttributeFromString( vReportDef, "Group", "Tag", "Footer" );
            //:vReportDef.Group.Type = "gf"
            SetAttributeFromString( vReportDef, "Group", "Type", "gf" );
            //:vReportDef.Group.PSDLG_X              = 0
            SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
            //:vReportDef.Group.PSDLG_Y              = lGroupPosY
            SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
            //:vReportDef.Group.SZDLG_X              = vReportDef.Page.SizeX
            SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
            //:vReportDef.Group.SZDLG_Y = 50
            SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", 50 );
            //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
            GetIntegerFromAttribute( &lTempInteger_3, vReportDef, "Group", "SZDLG_Y" );
            lGroupPosY = lGroupPosY + lTempInteger_3;
            //:vReportDef.Group.PPE_Type             = 3010
            SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );
         } 

         //:END
      } 

      //:END

      //:vReportDef.GroupSet.SZDLG_Y  = lGroupPosY + 100
      lTempInteger_4 = lGroupPosY + 100;
      SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lTempInteger_4 );
   } 


   //:END




   //:// Need to look at attributes as well?
   //:IF vReportDef.PartialReportEntity EXISTS
   lTempInteger_5 = CheckExistenceOfEntity( vReportDef, "PartialReportEntity" );
   if ( lTempInteger_5 == 0 )
   { 
      //:DELETE ENTITY vReportDef.PartialReportEntity
      RESULT = DeleteEntity( vReportDef, "PartialReportEntity", zPOS_NEXT );
   } 

   //:END

   //:CreateViewFromView( vReportDefRecurs, vReportDef )
   CreateViewFromView( &vReportDefRecurs, vReportDef );
   //:NAME VIEW vReportDefRecurs "vReportDefRecurs"
   SetNameForView( vReportDefRecurs, "vReportDefRecurs", 0, zLEVEL_TASK );

   //:nRC = CrtePartialLODStructR( vReportDef,
   //:                             vReportDefRecurs,
   //:                             vDrivingLOD )
   nRC = oTZRPSRCO_CrtePartialLODStructR( vReportDef, vReportDefRecurs, vDrivingLOD );

   //://DropView( vReportDefRecurs )

   //:// Now run through and delete all entities with no D or S.
   //://CreateViewFromView( vReportDefRecurs, vReportDef )
   //:DeleteNonDisplayEntsR( vReportDefRecurs )
   oTZRPSRCO_DeleteNonDisplayEntsR( vReportDefRecurs );
   //:DropView( vReportDefRecurs )
   DropView( vReportDefRecurs );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:GenerateXRP_Recurs( VIEW NewReport     BASED ON LOD TZRPSRCO,
//:                    VIEW DialogControl BASED ON LOD TZWDLGSO,
//:                    INTEGER X_Position,
//:                    INTEGER Y_Position,
//:                    INTEGER GroupSize )

//:   VIEW TZPESRCO REGISTERED AS TZPESRCO
static zSHORT
oTZRPSRCO_GenerateXRP_Recurs( zVIEW     NewReport,
                              zVIEW     DialogControl,
                              zLONG     X_Position,
                              zLONG     Y_Position,
                              zLONG     GroupSize )
{
   zVIEW     TZPESRCO = 0; 
   zSHORT    RESULT; 
   //:INTEGER TextControlHeight
   zLONG     TextControlHeight = 0; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zLONG     lTempInteger_8; 
   zLONG     lTempInteger_9; 
   zSHORT    lTempInteger_10; 
   zLONG     lTempInteger_11; 
   zLONG     lTempInteger_12; 
   zLONG     lTempInteger_13; 
   zLONG     lTempInteger_14; 
   zLONG     lTempInteger_15; 
   zLONG     lTempInteger_16; 
   zLONG     lTempInteger_17; 
   zLONG     lTempInteger_18; 
   zLONG     lTempInteger_19; 
   zLONG     lTempInteger_20; 
   zLONG     lTempInteger_21; 
   zLONG     lTempInteger_22; 
   zLONG     lTempInteger_23; 
   zLONG     lTempInteger_24; 
   zLONG     lTempInteger_25; 
   zLONG     lTempInteger_26; 
   zLONG     lTempInteger_27; 
   zLONG     lTempInteger_28; 

   RESULT = GetViewByName( &TZPESRCO, "TZPESRCO", NewReport, zLEVEL_TASK );

   //:// Sort the Controls in position order and position on TZPESRCO on Rpt_Text ControlDef.
   //:OrderEntityForView( DialogControl, "Control", "PSDLG_Y A PSDLG_X A" )
   OrderEntityForView( DialogControl, "Control", "PSDLG_Y A PSDLG_X A" );
   //:SET CURSOR FIRST TZPESRCO.ControlDef WHERE TZPESRCO.ControlDef.Tag = "Rpt_Text"
   RESULT = SetCursorFirstEntityByString( TZPESRCO, "ControlDef", "Tag", "Rpt_Text", "" );
   //:TextControlHeight = 37
   TextControlHeight = 37;

   //:// Process each Control.
   //:// For a Group Control, we will step down a level, but set X and Y positions as an addition of the position of
   //:// the Group and the position of each Control.
   //:// SpreadSheet Controls will have their own processing, because they will create unique Report Groups.
   //:// Button Controls will be ignored.
   //:// Checkbox Controls will be converted to two Text Controls, one without mapping (a prompt) and the other with mapping.
   //:// Combobox, Calendar and Edit Controls will be changed to Text Controls with mapping.
   //:// Text Controls will be processed as Text Controls, either with or without mapping.

   //:FOR EACH DialogControl.Control
   RESULT = SetCursorFirstEntity( DialogControl, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF DialogControl.ControlDef.Tag = "GroupBox"
      if ( CompareAttributeToString( DialogControl, "ControlDef", "Tag", "GroupBox" ) == 0 )
      { 
         //:// GroupBox
         //:X_Position = DialogControl.Control.PSDLG_X
         GetIntegerFromAttribute( &X_Position, DialogControl, "Control", "PSDLG_X" );
         //:Y_Position = DialogControl.Control.PSDLG_Y
         GetIntegerFromAttribute( &Y_Position, DialogControl, "Control", "PSDLG_Y" );
         //:SetViewToSubobject( DialogControl, "CtrlCtrl" )
         SetViewToSubobject( DialogControl, "CtrlCtrl" );
         //:GenerateXRP_Recurs( NewReport, DialogControl, X_Position, Y_Position, GroupSize )
         oTZRPSRCO_GenerateXRP_Recurs( NewReport, DialogControl, X_Position, Y_Position, GroupSize );
         //:ResetViewFromSubobject( DialogControl )
         ResetViewFromSubobject( DialogControl );
         //:ELSE
      } 
      else
      { 

         //:IF DialogControl.ControlDef.Tag = "SS"
         if ( CompareAttributeToString( DialogControl, "ControlDef", "Tag", "SS" ) == 0 )
         { 
            //:// SpreadSheet
            //:X_Position = DialogControl.Control.PSDLG_X
            GetIntegerFromAttribute( &X_Position, DialogControl, "Control", "PSDLG_X" );
            //:Y_Position = DialogControl.Control.PSDLG_Y
            GetIntegerFromAttribute( &Y_Position, DialogControl, "Control", "PSDLG_Y" );
            //:CreateMetaEntity( DialogControl, NewReport, "GroupSet", zPOS_AFTER )
            CreateMetaEntity( DialogControl, NewReport, "GroupSet", zPOS_AFTER );
            //:IF DialogControl.CtrlMapLOD_Entity.Name = ""
            if ( CompareAttributeToString( DialogControl, "CtrlMapLOD_Entity", "Name", "" ) == 0 )
            { 
               //:MessageSend( DialogControl, "", "Build Report", "SS Control has no mapping.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( DialogControl, "", "Build Report", "SS Control has no mapping.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
               //:ELSE
            } 
            else
            { 
               //:NewReport.GroupSet.Tag     = DialogControl.CtrlMapLOD_Entity.Name
               SetAttributeFromAttribute( NewReport, "GroupSet", "Tag", DialogControl, "CtrlMapLOD_Entity", "Name" );
            } 

            //:END
            //:NewReport.GroupSet.Type    = "E"
            SetAttributeFromString( NewReport, "GroupSet", "Type", "E" );
            //:NewReport.GroupSet.PSDLG_X = 0
            SetAttributeFromInteger( NewReport, "GroupSet", "PSDLG_X", 0 );
            //:NewReport.GroupSet.PSDLG_Y = 0
            SetAttributeFromInteger( NewReport, "GroupSet", "PSDLG_Y", 0 );
            //:NewReport.GroupSet.SZDLG_X = 2176
            SetAttributeFromInteger( NewReport, "GroupSet", "SZDLG_X", 2176 );
            //:NewReport.GroupSet.SZDLG_Y = 0    // Size here
            SetAttributeFromInteger( NewReport, "GroupSet", "SZDLG_Y", 0 );

            //:// Create Group Header for SS column Titles.
            //:CreateMetaEntity( DialogControl, NewReport, "Group", zPOS_AFTER )
            CreateMetaEntity( DialogControl, NewReport, "Group", zPOS_AFTER );
            //:NewReport.Group.Tag     = "GroupHeader"
            SetAttributeFromString( NewReport, "Group", "Tag", "GroupHeader" );
            //:NewReport.Group.Type    = "gh"
            SetAttributeFromString( NewReport, "Group", "Type", "gh" );
            //:NewReport.Group.SubType = 0
            SetAttributeFromInteger( NewReport, "Group", "SubType", 0 );
            //:NewReport.Group.PSDLG_X = 0
            SetAttributeFromInteger( NewReport, "Group", "PSDLG_X", 0 );
            //:NewReport.Group.PSDLG_Y = 0
            SetAttributeFromInteger( NewReport, "Group", "PSDLG_Y", 0 );
            //:NewReport.Group.SZDLG_X = 2176
            SetAttributeFromInteger( NewReport, "Group", "SZDLG_X", 2176 );
            //:NewReport.Group.SZDLG_Y = 30
            SetAttributeFromInteger( NewReport, "Group", "SZDLG_Y", 30 );

            //:// Create Header Controls for SS column Titles.
            //:SetViewToSubobject( DialogControl, "CtrlCtrl" )
            SetViewToSubobject( DialogControl, "CtrlCtrl" );
            //:FOR EACH DialogControl.Control
            RESULT = SetCursorFirstEntity( DialogControl, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:IF DialogControl.ControlDef.Tag != "PushBtn"
               if ( CompareAttributeToString( DialogControl, "ControlDef", "Tag", "PushBtn" ) != 0 )
               { 
                  //:CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER )
                  CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                  //:NewReport.Control.Tag = DialogControl.Control.Tag
                  SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                  //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                  GetIntegerFromAttribute( &lTempInteger_0, DialogControl, "Control", "PSDLG_X" );
                  lTempInteger_1 = X_Position + lTempInteger_0;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_1 );
                  //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                  GetIntegerFromAttribute( &lTempInteger_2, DialogControl, "Control", "PSDLG_Y" );
                  lTempInteger_3 = Y_Position + lTempInteger_2;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_3 );
                  //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                  SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                  //:NewReport.Control.SZDLG_Y = TextControlHeight
                  SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                  //:NewReport.Control.Type = 20
                  SetAttributeFromInteger( NewReport, "Control", "Type", 20 );
                  //:INCLUDE NewReport.ControlDef FROM TZPESRCO.ControlDef
                  RESULT = IncludeSubobjectFromSubobject( NewReport, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
                  //:SET CURSOR NEXT DialogControl.CtrlMapLOD_Attribute WITHIN DialogControl.Control
                  RESULT = SetCursorNextEntity( DialogControl, "CtrlMapLOD_Attribute", "Control" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:// Column title is a variable.
                     //:GenerateXRP_CtrlMap( NewReport, DialogControl )
                     oTZRPSRCO_GenerateXRP_CtrlMap( NewReport, DialogControl );
                     //:ELSE
                  } 
                  else
                  { 
                     //:// Column title is a constant.
                     //:NewReport.Control.Text    = DialogControl.Control.Text
                     SetAttributeFromAttribute( NewReport, "Control", "Text", DialogControl, "Control", "Text" );
                  } 

                  //:END
               } 

               RESULT = SetCursorNextEntity( DialogControl, "Control", "" );
               //:END
            } 

            //:END
            //:ResetViewFromSubobject( DialogControl )
            ResetViewFromSubobject( DialogControl );

            //:// Create a single Detail Group.
            //:CreateMetaEntity( DialogControl, NewReport, "Group", zPOS_AFTER )
            CreateMetaEntity( DialogControl, NewReport, "Group", zPOS_AFTER );
            //:NewReport.Group.Tag     = "Detail"
            SetAttributeFromString( NewReport, "Group", "Tag", "Detail" );
            //:NewReport.Group.Type    = "ga"
            SetAttributeFromString( NewReport, "Group", "Type", "ga" );
            //:NewReport.Group.PSDLG_X = 0
            SetAttributeFromInteger( NewReport, "Group", "PSDLG_X", 0 );
            //:NewReport.Group.PSDLG_Y = 0
            SetAttributeFromInteger( NewReport, "Group", "PSDLG_Y", 0 );
            //:NewReport.Group.SZDLG_X = 2176
            SetAttributeFromInteger( NewReport, "Group", "SZDLG_X", 2176 );
            //:NewReport.Group.SZDLG_Y = 30
            SetAttributeFromInteger( NewReport, "Group", "SZDLG_Y", 30 );

            //:// Create Text Controls for each Spreadsheet subcontrol, except a PushButton.
            //:SetViewToSubobject( DialogControl, "CtrlCtrl" )
            SetViewToSubobject( DialogControl, "CtrlCtrl" );
            //:FOR EACH DialogControl.Control
            RESULT = SetCursorFirstEntity( DialogControl, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:IF DialogControl.ControlDef.Tag != "PushBtn"
               if ( CompareAttributeToString( DialogControl, "ControlDef", "Tag", "PushBtn" ) != 0 )
               { 
                  //:CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER )
                  CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                  //:NewReport.Control.Tag = DialogControl.Control.Tag
                  SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                  //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                  GetIntegerFromAttribute( &lTempInteger_4, DialogControl, "Control", "PSDLG_X" );
                  lTempInteger_5 = X_Position + lTempInteger_4;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_5 );
                  //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                  GetIntegerFromAttribute( &lTempInteger_6, DialogControl, "Control", "PSDLG_Y" );
                  lTempInteger_7 = Y_Position + lTempInteger_6;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_7 );
                  //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                  SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                  //:NewReport.Control.SZDLG_Y = TextControlHeight
                  SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                  //:GroupSize = NewReport.Control.PSDLG_Y + NewReport.Control.SZDLG_Y
                  GetIntegerFromAttribute( &lTempInteger_8, NewReport, "Control", "PSDLG_Y" );
                  GetIntegerFromAttribute( &lTempInteger_9, NewReport, "Control", "SZDLG_Y" );
                  GroupSize = lTempInteger_8 + lTempInteger_9;
                  //:NewReport.Control.Type = 20
                  SetAttributeFromInteger( NewReport, "Control", "Type", 20 );
                  //:INCLUDE NewReport.ControlDef FROM TZPESRCO.ControlDef
                  RESULT = IncludeSubobjectFromSubobject( NewReport, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
                  //:GenerateXRP_CtrlMap( NewReport, DialogControl )
                  oTZRPSRCO_GenerateXRP_CtrlMap( NewReport, DialogControl );
               } 

               RESULT = SetCursorNextEntity( DialogControl, "Control", "" );
               //:END
            } 

            //:END
            //:ResetViewFromSubobject( DialogControl )
            ResetViewFromSubobject( DialogControl );

            //:ELSE
         } 
         else
         { 

            //:IF DialogControl.CtrlMap EXISTS
            lTempInteger_10 = CheckExistenceOfEntity( DialogControl, "CtrlMap" );
            if ( lTempInteger_10 == 0 )
            { 
               //:IF DialogControl.ControlDef.Tag = "ComboBox" OR
               //:DialogControl.ControlDef.Tag = "Calendar" OR
               //:DialogControl.ControlDef.Tag = "EditBox" OR
               //:DialogControl.ControlDef.Tag = "Text"
               if ( CompareAttributeToString( DialogControl, "ControlDef", "Tag", "ComboBox" ) == 0 || CompareAttributeToString( DialogControl, "ControlDef", "Tag", "Calendar" ) == 0 ||
                    CompareAttributeToString( DialogControl, "ControlDef", "Tag", "EditBox" ) == 0 || CompareAttributeToString( DialogControl, "ControlDef", "Tag", "Text" ) == 0 )
               { 

                  //:// Mapped Controls which will be converted to Text.
                  //:CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER )
                  CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                  //:NewReport.Control.Tag = DialogControl.Control.Tag
                  SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                  //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                  GetIntegerFromAttribute( &lTempInteger_11, DialogControl, "Control", "PSDLG_X" );
                  lTempInteger_12 = X_Position + lTempInteger_11;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_12 );
                  //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                  GetIntegerFromAttribute( &lTempInteger_13, DialogControl, "Control", "PSDLG_Y" );
                  lTempInteger_14 = Y_Position + lTempInteger_13;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_14 );
                  //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                  SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                  //:NewReport.Control.SZDLG_Y = TextControlHeight
                  SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                  //:GroupSize = NewReport.Control.PSDLG_Y + NewReport.Control.SZDLG_Y
                  GetIntegerFromAttribute( &lTempInteger_15, NewReport, "Control", "PSDLG_Y" );
                  GetIntegerFromAttribute( &lTempInteger_16, NewReport, "Control", "SZDLG_Y" );
                  GroupSize = lTempInteger_15 + lTempInteger_16;
                  //:NewReport.Control.Type = 20
                  SetAttributeFromInteger( NewReport, "Control", "Type", 20 );
                  //:INCLUDE NewReport.ControlDef FROM TZPESRCO.ControlDef
                  RESULT = IncludeSubobjectFromSubobject( NewReport, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
                  //:GenerateXRP_CtrlMap( NewReport, DialogControl )
                  oTZRPSRCO_GenerateXRP_CtrlMap( NewReport, DialogControl );

                  //:ELSE
               } 
               else
               { 
                  //:IF DialogControl.ControlDef.Tag = "CheckBox"
                  if ( CompareAttributeToString( DialogControl, "ControlDef", "Tag", "CheckBox" ) == 0 )
                  { 
                     //:// CheckBox
                     //:CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER )
                     CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                     //:NewReport.Control.Tag = DialogControl.Control.Tag
                     SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                     //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                     GetIntegerFromAttribute( &lTempInteger_17, DialogControl, "Control", "PSDLG_X" );
                     lTempInteger_18 = X_Position + lTempInteger_17;
                     SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_18 );
                     //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                     GetIntegerFromAttribute( &lTempInteger_19, DialogControl, "Control", "PSDLG_Y" );
                     lTempInteger_20 = Y_Position + lTempInteger_19;
                     SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_20 );
                     //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                     SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                     //:NewReport.Control.SZDLG_Y = TextControlHeight
                     SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                     //:GroupSize = NewReport.Control.PSDLG_Y + NewReport.Control.SZDLG_Y
                     GetIntegerFromAttribute( &lTempInteger_21, NewReport, "Control", "PSDLG_Y" );
                     GetIntegerFromAttribute( &lTempInteger_22, NewReport, "Control", "SZDLG_Y" );
                     GroupSize = lTempInteger_21 + lTempInteger_22;
                     //:NewReport.Control.Type = 20
                     SetAttributeFromInteger( NewReport, "Control", "Type", 20 );
                     //:INCLUDE NewReport.ControlDef FROM TZPESRCO.ControlDef
                     RESULT = IncludeSubobjectFromSubobject( NewReport, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
                     //:GenerateXRP_CtrlMap( NewReport, DialogControl )
                     oTZRPSRCO_GenerateXRP_CtrlMap( NewReport, DialogControl );
                  } 


                  //:END
               } 

               //:END
               //:ELSE
            } 
            else
            { 

               //:IF DialogControl.ControlDef.Tag = "Text"
               if ( CompareAttributeToString( DialogControl, "ControlDef", "Tag", "Text" ) == 0 )
               { 
                  //:// Non-mapped Text Control (Prompt)
                  //:CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER )
                  CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                  //:NewReport.Control.Tag = DialogControl.Control.Tag
                  SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                  //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                  GetIntegerFromAttribute( &lTempInteger_23, DialogControl, "Control", "PSDLG_X" );
                  lTempInteger_24 = X_Position + lTempInteger_23;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_24 );
                  //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                  GetIntegerFromAttribute( &lTempInteger_25, DialogControl, "Control", "PSDLG_Y" );
                  lTempInteger_26 = Y_Position + lTempInteger_25;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_26 );
                  //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                  SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                  //:NewReport.Control.SZDLG_Y = TextControlHeight
                  SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                  //:GroupSize = NewReport.Control.PSDLG_Y + NewReport.Control.SZDLG_Y
                  GetIntegerFromAttribute( &lTempInteger_27, NewReport, "Control", "PSDLG_Y" );
                  GetIntegerFromAttribute( &lTempInteger_28, NewReport, "Control", "SZDLG_Y" );
                  GroupSize = lTempInteger_27 + lTempInteger_28;
                  //:NewReport.Control.Type = 10
                  SetAttributeFromInteger( NewReport, "Control", "Type", 10 );
                  //:INCLUDE NewReport.ControlDef FROM TZPESRCO.ControlDef
                  RESULT = IncludeSubobjectFromSubobject( NewReport, "ControlDef", TZPESRCO, "ControlDef", zPOS_AFTER );
               } 


               //:END
            } 

            //:END
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( DialogControl, "Control", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zVOID
oTZRPSRCO_GenerateXRP_CtrlMap( zVIEW     NewReport,
                               zVIEW     DialogControl )
{
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 

   //:GenerateXRP_CtrlMap( VIEW NewReport     BASED ON LOD TZRPSRCO,
   //:                  VIEW DialogControl BASED ON LOD TZWDLGSO )

   //:// Build the Report CtrlMap subobject from the Dialog subobject.
   //:CreateMetaEntity( DialogControl, NewReport, "CtrlMap", zPOS_AFTER )
   CreateMetaEntity( DialogControl, NewReport, "CtrlMap", zPOS_AFTER );
   //:NewReport.CtrlMap.Tag = DialogControl.CtrlMap.Tag
   SetAttributeFromAttribute( NewReport, "CtrlMap", "Tag", DialogControl, "CtrlMap", "Tag" );
   //:IF DialogControl.CtrlMapLOD_Attribute EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( DialogControl, "CtrlMapLOD_Attribute" );
   if ( lTempInteger_0 == 0 )
   { 
      //:INCLUDE NewReport.CtrlMapLOD_Attribute FROM DialogControl.CtrlMapLOD_Attribute
      RESULT = IncludeSubobjectFromSubobject( NewReport, "CtrlMapLOD_Attribute", DialogControl, "CtrlMapLOD_Attribute", zPOS_AFTER );
   } 

   //:END
   //:IF DialogControl.CtrlMapLOD_Entity EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( DialogControl, "CtrlMapLOD_Entity" );
   if ( lTempInteger_1 == 0 )
   { 
      //:INCLUDE NewReport.CtrlMapLOD_Entity FROM DialogControl.CtrlMapLOD_Entity
      RESULT = IncludeSubobjectFromSubobject( NewReport, "CtrlMapLOD_Entity", DialogControl, "CtrlMapLOD_Entity", zPOS_AFTER );
   } 

   //:END
   //:IF DialogControl.CtrlMapContext EXISTS
   lTempInteger_2 = CheckExistenceOfEntity( DialogControl, "CtrlMapContext" );
   if ( lTempInteger_2 == 0 )
   { 
      //:INCLUDE NewReport.CtrlMapContext FROM DialogControl.CtrlMapContext
      RESULT = IncludeSubobjectFromSubobject( NewReport, "CtrlMapContext", DialogControl, "CtrlMapContext", zPOS_AFTER );
   } 

   //:END
   //:IF DialogControl.CtrlMapView EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( DialogControl, "CtrlMapView" );
   if ( lTempInteger_3 == 0 )
   { 
      //:INCLUDE NewReport.CtrlMapView FROM DialogControl.CtrlMapView
      RESULT = IncludeSubobjectFromSubobject( NewReport, "CtrlMapView", DialogControl, "CtrlMapView", zPOS_AFTER );
   } 

   //:END
   return;
// END
} 


//:TRANSFORMATION OPERATION
//:InitReportStructure( VIEW vReportDef BASED ON LOD TZRPSRCO )

//:   VIEW vReportDefRecurs BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_InitReportStructure( zVIEW     vReportDef )
{
   zVIEW     vReportDefRecurs = 0; 
   //:VIEW vDrivingLOD      BASED ON LOD TZZOLODO
   zVIEW     vDrivingLOD = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   //:SHORT iOrder
   zSHORT    iOrder = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:TraceLineS("*** InitReportStructure *** ", "")
   TraceLineS( "*** InitReportStructure *** ", "" );

   //:// If the ReportEntity subobject within the DrivingViewObjRef doesn't exist, build it here from the
   //:// GroupSet subobject and the LOD structure.

   //:// Make sure that a Driving Object has been defined.
   //:/*
   //:IF vReportDef.DrivingViewObjRef DOES NOT EXIST
   //:   MessageSend( vReportDef, "", "Initialize Report Structure",
   //:                "A Driving Report Object view has not been defined.",
   //:                zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
   //:   RETURN -1
   //:END
   //:*/
   //:IF vReportDef.DrivingViewObjRef DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "DrivingViewObjRef" );
   if ( lTempInteger_0 != 0 )
   { 
      //:RETURN 0
      return( 0 );
   } 

   //:END

   //:// Just return if the subobject already exists.
   //:// KJS - Wondering if we should rebuild this everytime...
   //:IF vReportDef.FullReportEntity EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( vReportDef, "FullReportEntity" );
   if ( lTempInteger_1 == 0 )
   { 
      //:DELETE ENTITY vReportDef.FullReportEntity
      RESULT = DeleteEntity( vReportDef, "FullReportEntity", zPOS_NEXT );
   } 

   //:   //RETURN 0
   //:END
   //:IF vReportDef.PartialReportEntity EXISTS
   lTempInteger_2 = CheckExistenceOfEntity( vReportDef, "PartialReportEntity" );
   if ( lTempInteger_2 == 0 )
   { 
      //:DELETE ENTITY vReportDef.PartialReportEntity
      RESULT = DeleteEntity( vReportDef, "PartialReportEntity", zPOS_NEXT );
   } 

   //:   //RETURN 0
   //:END

   //:// I want to create an order for the GroupSets
   //:OrderEntityForView( vReportDef, "GroupSet", "PSDLG_Y A" )
   OrderEntityForView( vReportDef, "GroupSet", "PSDLG_Y A" );
   //:iOrder = 1
   iOrder = 1;
   //:FOR EACH vReportDef.GroupSet
   RESULT = SetCursorFirstEntity( vReportDef, "GroupSet", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:vReportDef.GroupSet.Order = iOrder
      SetAttributeFromInteger( vReportDef, "GroupSet", "Order", iOrder );
      RESULT = SetCursorNextEntity( vReportDef, "GroupSet", "" );
   } 

   //:END
   //:// Not sure I need to do this...
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PF"
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PF", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// Setting to a random high number
      //:vReportDef.GroupSet.Order = 100
      SetAttributeFromInteger( vReportDef, "GroupSet", "Order", 100 );
   } 

   //:END

   //:IssueError( vReportDef,0,0, "start" )
   IssueError( vReportDef, 0, 0, "start" );

   //:// Get Driving LOD
   //:nRC = ActivateMetaOI_ByName( vReportDef, vDrivingLOD, 0, zREFER_LOD_META,
   //:                             zSINGLE, vReportDef.DrivingLOD.Name, 0 )
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vReportDef, "DrivingLOD", "Name" );
   nRC = ActivateMetaOI_ByName( vReportDef, &vDrivingLOD, 0, zREFER_LOD_META, zSINGLE, szTempString_0, 0 );
   //:// KJS - WHY DON"T WE JUST USE vDrivingLOD? Can't remember why we need all stored in the report?
   //:TraceLineS("*** Activate DrivingLOD *** ",   vReportDef.DrivingLOD.Name )
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vReportDef, "DrivingLOD", "Name" );
   TraceLineS( "*** Activate DrivingLOD *** ", szTempString_1 );
   //:TraceLineI("*** Activate returncode *** ", nRC )
   TraceLineI( "*** Activate returncode *** ", (zLONG) nRC );

   //:IF nRC >= 0
   if ( nRC >= 0 )
   { 
      //:// Set up recursive Report view and process LOD recursively to create Report Structure subobject, setting
      //:// the ReportDisplayFlag to indicate if an entity is to drive a GroupSet on the report.
      //:NAME VIEW vDrivingLOD "DrivingLOD"
      SetNameForView( vDrivingLOD, "DrivingLOD", 0, zLEVEL_TASK );
      //:CreateViewFromView( vReportDefRecurs, vReportDef )
      CreateViewFromView( &vReportDefRecurs, vReportDef );
      //:NAME VIEW vReportDefRecurs "vReportDefRecurs"
      SetNameForView( vReportDefRecurs, "vReportDefRecurs", 0, zLEVEL_TASK );

      //:nRC = InitReportStructureR( vReportDef,
      //:                            vReportDefRecurs,
      //:                            vDrivingLOD )
      nRC = oTZRPSRCO_InitReportStructureR( vReportDef, vReportDefRecurs, vDrivingLOD );

      //:DropView( vReportDefRecurs )
      DropView( vReportDefRecurs );

      //:CreateViewFromView( vReportDefRecurs, vReportDef )
      CreateViewFromView( &vReportDefRecurs, vReportDef );
      //:NAME VIEW vReportDefRecurs "vReportDefRecurs"
      SetNameForView( vReportDefRecurs, "vReportDefRecurs", 0, zLEVEL_TASK );

      //:TraceLineS("*** Before CrtePartialLODStructR *** ", "")
      TraceLineS( "*** Before CrtePartialLODStructR *** ", "" );

      //:nRC = CrtePartialLODStructR( vReportDef,
      //:                             vReportDefRecurs,
      //:                             vDrivingLOD )
      nRC = oTZRPSRCO_CrtePartialLODStructR( vReportDef, vReportDefRecurs, vDrivingLOD );

      //:DropView( vReportDefRecurs )
      DropView( vReportDefRecurs );

      //:// Now run through and delete all entities with no D or S.
      //:CreateViewFromView( vReportDefRecurs, vReportDef )
      CreateViewFromView( &vReportDefRecurs, vReportDef );
      //:DeleteNonDisplayEntsR( vReportDefRecurs )
      oTZRPSRCO_DeleteNonDisplayEntsR( vReportDefRecurs );
      //:DropView( vReportDefRecurs )
      DropView( vReportDefRecurs );
      //:SET CURSOR FIRST vReportDef.FullReportEntity
      RESULT = SetCursorFirstEntity( vReportDef, "FullReportEntity", "" );
      //:SET CURSOR FIRST vReportDef.PartialReportEntity
      RESULT = SetCursorFirstEntity( vReportDef, "PartialReportEntity", "" );
   } 


   //:END
   //:CreateViewFromViewForTask( vReportDefRecurs, vReportDef, 0 )
   CreateViewFromViewForTask( &vReportDefRecurs, vReportDef, 0 );
   //:NAME VIEW  vReportDefRecurs  "TZRPTOUTLINER"
   SetNameForView( vReportDefRecurs, "TZRPTOUTLINER", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:InitReportStructureR( VIEW vReportDef       BASED ON LOD TZRPSRCO,
//:                      VIEW vReportDefRecurs BASED ON LOD TZRPSRCO,
//:                      VIEW vDrivingLOD      BASED ON LOD TZZOLODO )

//:   STRING ( 32 ) szDrivingEntityName
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_InitReportStructureR( zVIEW     vReportDef,
                                zVIEW     vReportDefRecurs,
                                zVIEW     vDrivingLOD )
{
   zCHAR     szDrivingEntityName[ 33 ] = { 0 }; 
   //:STRING ( 1 )  szHasGroupSetFlag
   zCHAR     szHasGroupSetFlag[ 2 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:// Build one level of the FullReportEntity subobject, matching the structure of the Driving LOD and
   //:// setting the ReportDisplayFlag entity for any entity with a corresponding GroupSet entity in the report, or having
   //:// a FullReportEntityChild entity with a corresponding GroupSet entity. The flag is set to "D" if the entity has
   //:// a corresponding GroupSet entity and to a "C" if it has a child with a corresponding GroupSet entity.

   //:FOR EACH vDrivingLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( vDrivingLOD, "LOD_EntityParent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:szDrivingEntityName = vDrivingLOD.LOD_EntityParent.Name
      GetVariableFromAttribute( szDrivingEntityName, 0, 'S', 33, vDrivingLOD, "LOD_EntityParent", "Name", "", 0 );

      //:// Create corresponding FullReportEntity entity.
      //:CREATE ENTITY vReportDefRecurs.FullReportEntity
      RESULT = CreateEntity( vReportDefRecurs, "FullReportEntity", zPOS_AFTER );
      //:vReportDefRecurs.FullReportEntity.Name = szDrivingEntityName
      SetAttributeFromString( vReportDefRecurs, "FullReportEntity", "Name", szDrivingEntityName );
      //:IssueError( vReportDef,0,0, szDrivingEntityName )
      IssueError( vReportDef, 0, 0, szDrivingEntityName );

      //:// Process corresponding report GroupSet entity, if there is one
      //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szDrivingEntityName
      RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szDrivingEntityName, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:vReportDefRecurs.FullReportEntity.ReportDisplayFlag = "D"
         SetAttributeFromString( vReportDefRecurs, "FullReportEntity", "ReportDisplayFlag", "D" );
         //:szHasGroupSetFlag = "Y"
         ZeidonStringCopy( szHasGroupSetFlag, 1, 0, "Y", 1, 0, 2 );
      } 

      //:END

      //:// Process any LOD subentities.
      //:IF vDrivingLOD.LOD_EntityChild EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vDrivingLOD, "LOD_EntityChild" );
      if ( lTempInteger_0 == 0 )
      { 
         //:SetViewToSubobject( vDrivingLOD, "LOD_EntityChild" )
         SetViewToSubobject( vDrivingLOD, "LOD_EntityChild" );
         //:SetViewToSubobject( vReportDefRecurs, "FullReportEntityChild" )
         SetViewToSubobject( vReportDefRecurs, "FullReportEntityChild" );
         //:nRC = InitReportStructureR( vReportDef,
         //:                            vReportDefRecurs,
         //:                            vDrivingLOD )
         nRC = oTZRPSRCO_InitReportStructureR( vReportDef, vReportDefRecurs, vDrivingLOD );
         //:ResetViewFromSubobject( vDrivingLOD )
         ResetViewFromSubobject( vDrivingLOD );
         //:ResetViewFromSubobject( vReportDefRecurs )
         ResetViewFromSubobject( vReportDefRecurs );

         //:// If the current entity does not have the Display Flag set to "D" but the return code indicates a child
         //:// entity does have its Display Flag set to "D", then set the Display Flag to "S" to indicate this condition.
         //:IF vReportDefRecurs.FullReportEntity.ReportDisplayFlag = "" AND nRC = 1
         if ( CompareAttributeToString( vReportDefRecurs, "FullReportEntity", "ReportDisplayFlag", "" ) == 0 && nRC == 1 )
         { 
            //:vReportDefRecurs.FullReportEntity.ReportDisplayFlag = "S"
            SetAttributeFromString( vReportDefRecurs, "FullReportEntity", "ReportDisplayFlag", "S" );
         } 

         //:END

         //:// If any child has its Display Flag set, so indicate to the calling operation.
         //:IF nRC = 1
         if ( nRC == 1 )
         { 
            //: szHasGroupSetFlag = "Y"
            ZeidonStringCopy( szHasGroupSetFlag, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vDrivingLOD, "LOD_EntityParent", "" );
      //:END
   } 

   //:END

   //:IF szHasGroupSetFlag = "Y"
   if ( ZeidonStringCompare( szHasGroupSetFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:RETURN 1     // Indicate a child has a corresponding GroupSet entity.
      return( 1 );
      //:ELSE
   } 
   else
   { 
      //:RETURN 0
      return( 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:BuildReportDispObject( VIEW vReportDefSource BASED ON LOD TZRPSRCO )

//:   VIEW  vReportDefTarget BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_BuildReportDispObject( zVIEW     vReportDefSource )
{
   zVIEW     vReportDefTarget = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 


   //:// Build the PartialLOD_Entity Report subobject from the FullReportEntity subobject.
   //:CreateViewFromView( vReportDefTarget, vReportDefSource )
   CreateViewFromView( &vReportDefTarget, vReportDefSource );

   //:nRC = BuildReportDispObjectR( vReportDefSource,
   //:                              vReportDefTarget )
   nRC = oTZRPSRCO_BuildReportDispObjectR( vReportDefSource, vReportDefTarget );

   //:DropView( vReportDefTarget )
   DropView( vReportDefTarget );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:BuildReportDispObjectR( VIEW vReportDefSource BASED ON LOD TZRPSRCO,
//:                        VIEW vReportDefTarget BASED ON LOD TZRPSRCO )
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_BuildReportDispObjectR( zVIEW     vReportDefSource,
                                  zVIEW     vReportDefTarget )
{

   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenerateXRP( VIEW NewReport    BASED ON LOD TZRPSRCO,
//:             VIEW DialogWindow BASED ON LOD TZWDLGSO)

//:   VIEW vTaskLPLR        BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_GenerateXRP( zPVIEW    NewReport,
                       zVIEW     DialogWindow )
{
   zVIEW     vTaskLPLR = 0; 
   //:VIEW DialogWindowHier BASED ON LOD TZWDLGSO
   zVIEW     DialogWindowHier = 0; 
   //:VIEW TestReport
   zVIEW     TestReport = 0; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 64 )  szReportName
   zCHAR     szReportName[ 65 ] = { 0 }; 
   //:STRING ( 32 )  szDialogTag
   zCHAR     szDialogTag[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szReturnedEntityName
   zCHAR     szReturnedEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szFirstEntityName
   zCHAR     szFirstEntityName[ 33 ] = { 0 }; 
   //:STRING ( 1 )   szFoundFlag
   zCHAR     szFoundFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szNoInitalGroupSetFlag
   zCHAR     szNoInitalGroupSetFlag[ 2 ] = { 0 }; 
   //:STRING ( 256 ) szHTML_DirectoryName
   zCHAR     szHTML_DirectoryName[ 257 ] = { 0 }; 
   //:STRING ( 500 ) szDirectoryFileName
   zCHAR     szDirectoryFileName[ 501 ] = { 0 }; 
   //:INTEGER        X_Position
   zLONG     X_Position = 0; 
   //:INTEGER        Y_Position
   zLONG     Y_Position = 0; 
   //:INTEGER        GroupSize
   zLONG     GroupSize = 0; 
   //:SHORT          lReturnedLevel
   zSHORT    lReturnedLevel = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_1[ 33 ]; 


   //:// Build an XRP object from the Dialog Window passed.

   //:// Initialize XRP. (Report and Page entities)
   //:szDialogTag = DialogWindow.Dialog.Tag
   GetVariableFromAttribute( szDialogTag, 0, 'S', 33, DialogWindow, "Dialog", "Tag", "", 0 );
   //:szReportName = szDialogTag + DialogWindow.Window.Tag
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, DialogWindow, "Window", "Tag", "", 0 );
   ZeidonStringCopy( szReportName, 1, 0, szDialogTag, 1, 0, 65 );
   ZeidonStringConcat( szReportName, 1, 0, szTempString_0, 1, 0, 65 );
   //:ActivateEmptyObjectInstance( NewReport, "TZRPSRCO", DialogWindow, zSINGLE )
   ActivateEmptyObjectInstance( NewReport, "TZRPSRCO", DialogWindow, zSINGLE );
   //:NAME VIEW NewReport "NewReportXRP"
   SetNameForView( *NewReport, "NewReportXRP", 0, zLEVEL_TASK );
   //:CreateMetaEntity( DialogWindow, NewReport, "Report", zPOS_AFTER )
   CreateMetaEntity( DialogWindow, *NewReport, "Report", zPOS_AFTER );
   //:NewReport.Report.Tag = szDialogTag
   SetAttributeFromString( *NewReport, "Report", "Tag", szDialogTag );
   //:NewReport.Report.ReportTextSize = 100
   SetAttributeFromInteger( *NewReport, "Report", "ReportTextSize", 100 );
   //:CreateMetaEntity( DialogWindow, NewReport, "Page", zPOS_AFTER )
   CreateMetaEntity( DialogWindow, *NewReport, "Page", zPOS_AFTER );
   //:NewReport.Page.Tag = szReportName
   SetAttributeFromString( *NewReport, "Page", "Tag", szReportName );
   //:NewReport.Page.SizeX = 2176
   SetAttributeFromInteger( *NewReport, "Page", "SizeX", 2176 );
   //:NewReport.Page.SizeY = 2816
   SetAttributeFromInteger( *NewReport, "Page", "SizeY", 2816 );

   //:// ViewObjRef
   //:// Build one for each ViewObjRef referenced in the Window. Note that we're not just including
   //:// every ViewObjRef from the Dialog.
   //:// The first ViewObjRef will be for the view that will drive the Report.
   //:// The PrintPageEntity will define the first entity passed to the report for printing.
   //:SET CURSOR FIRST DialogWindow.Control
   RESULT = SetCursorFirstEntity( DialogWindow, "Control", "" );
   //:CreateViewFromView( DialogWindowHier, DialogWindow )
   CreateViewFromView( &DialogWindowHier, DialogWindow );
   //:DefineHierarchicalCursor( DialogWindowHier, "Window" )
   DefineHierarchicalCursor( DialogWindowHier, "Window" );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, DialogWindowHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, DialogWindowHier );
   //:LOOP WHILE nRC >= zCURSOR_SET AND szReturnedEntityName != "Window"
   while ( nRC >= zCURSOR_SET && ZeidonStringCompare( szReturnedEntityName, 1, 0, "Window", 1, 0, 33 ) != 0 )
   { 
      //:IF szReturnedEntityName = "CtrlMapView"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlMapView", 1, 0, 33 ) == 0 )
      { 
         //:SET CURSOR FIRST NewReport.ViewObjRef WHERE NewReport.ViewObjRef.ZKey = DialogWindowHier.CtrlMapView.ZKey
         GetIntegerFromAttribute( &lTempInteger_0, DialogWindowHier, "CtrlMapView", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( *NewReport, "ViewObjRef", "ZKey", lTempInteger_0, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR FIRST DialogWindow.ViewObjRef WHERE DialogWindow.ViewObjRef.ZKey = DialogWindowHier.CtrlMapView.ZKey
            GetIntegerFromAttribute( &lTempInteger_1, DialogWindowHier, "CtrlMapView", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( DialogWindow, "ViewObjRef", "ZKey", lTempInteger_1, "" );
            //:INCLUDE NewReport.ViewObjRef FROM DialogWindow.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( *NewReport, "ViewObjRef", DialogWindow, "ViewObjRef", zPOS_AFTER );
            //:IF NewReport.Report.WebPrintPageEntity = ""
            if ( CompareAttributeToString( *NewReport, "Report", "WebPrintPageEntity", "" ) == 0 )
            { 
               //:IF DialogWindowHier.CtrlMapRelatedEntity EXISTS
               lTempInteger_2 = CheckExistenceOfEntity( DialogWindowHier, "CtrlMapRelatedEntity" );
               if ( lTempInteger_2 == 0 )
               { 
                  //:NewReport.Report.WebPrintPageEntity = DialogWindowHier.CtrlMapRelatedEntity.Name
                  SetAttributeFromAttribute( *NewReport, "Report", "WebPrintPageEntity", DialogWindowHier, "CtrlMapRelatedEntity", "Name" );
                  //:ELSE
               } 
               else
               { 
                  //:NewReport.Report.WebPrintPageEntity = DialogWindowHier.CtrlMapLOD_Entity.Name
                  SetAttributeFromAttribute( *NewReport, "Report", "WebPrintPageEntity", DialogWindowHier, "CtrlMapLOD_Entity", "Name" );
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, DialogWindowHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, DialogWindowHier );
   } 

   //:END
   //:DropView( DialogWindowHier )
   DropView( DialogWindowHier );

   //:// First GroupSet
   //:// Build for first Control being mapped on the Window.
   //:// Because the first Control being mapped may be within a Group Control, we will process the Window hierarchically.
   //:// If the first Control is a Spreadsheet, we will not create the initial GroupSet, but only create the one
   //:// associated with the Spreadsheet later.
   //:SET CURSOR FIRST DialogWindow.Control
   RESULT = SetCursorFirstEntity( DialogWindow, "Control", "" );
   //:CreateViewFromView( DialogWindowHier, DialogWindow )
   CreateViewFromView( &DialogWindowHier, DialogWindow );
   //:DefineHierarchicalCursor( DialogWindowHier, "Window" )
   DefineHierarchicalCursor( DialogWindowHier, "Window" );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, DialogWindowHier )
   nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, DialogWindowHier );
   //:LOOP WHILE nRC >= zCURSOR_SET AND szFoundFlag = "" AND szReturnedEntityName != "Window"
   while ( nRC >= zCURSOR_SET && ZeidonStringCompare( szFoundFlag, 1, 0, "", 1, 0, 2 ) == 0 && ZeidonStringCompare( szReturnedEntityName, 1, 0, "Window", 1, 0, 33 ) != 0 )
   { 
      //:IF szReturnedEntityName = "CtrlMapRelatedEntity"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlMapRelatedEntity", 1, 0, 33 ) == 0 )
      { 
         //:szFoundFlag = "Y"
         ZeidonStringCopy( szFoundFlag, 1, 0, "Y", 1, 0, 2 );
         //:szFirstEntityName = DialogWindowHier.CtrlMapRelatedEntity.Name
         GetVariableFromAttribute( szFirstEntityName, 0, 'S', 33, DialogWindowHier, "CtrlMapRelatedEntity", "Name", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:IF szReturnedEntityName = "CtrlMapLOD_Entity"
         if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlMapLOD_Entity", 1, 0, 33 ) == 0 )
         { 
            //:szFoundFlag = "Y"
            ZeidonStringCopy( szFoundFlag, 1, 0, "Y", 1, 0, 2 );
            //:IF DialogWindowHier.ControlDef.Tag = "SS"
            if ( CompareAttributeToString( DialogWindowHier, "ControlDef", "Tag", "SS" ) == 0 )
            { 
               //:szNoInitalGroupSetFlag = "Y"
               ZeidonStringCopy( szNoInitalGroupSetFlag, 1, 0, "Y", 1, 0, 2 );
               //:ELSE
            } 
            else
            { 
               //:szFirstEntityName = DialogWindowHier.CtrlMapLOD_Entity.Name
               GetVariableFromAttribute( szFirstEntityName, 0, 'S', 33, DialogWindowHier, "CtrlMapLOD_Entity", "Name", "", 0 );
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, DialogWindowHier )
      nRC = SetCursorNextEntityHierarchical( (zPUSHORT) &lReturnedLevel, szReturnedEntityName, DialogWindowHier );
   } 

   //:END
   //:IF nRC < zCURSOR_SET
   if ( nRC < zCURSOR_SET )
   { 
      //:szMsg = "No mapped Controls exist for Window, " + DialogWindow.Window.Tag + "." + NEW_LINE +
      //:        "No report XRP will be generated."
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, DialogWindow, "Window", "Tag", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "No mapped Controls exist for Window, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, ".", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, "No report XRP will be generated.", 1, 0, 201 );
      //:MessageSend( DialogWindow, "", "Build Report", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( DialogWindow, "", "Build Report", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   //:IF szNoInitalGroupSetFlag = ""
   if ( ZeidonStringCompare( szNoInitalGroupSetFlag, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:CreateMetaEntity( DialogWindow, NewReport, "GroupSet", zPOS_AFTER )
      CreateMetaEntity( DialogWindow, *NewReport, "GroupSet", zPOS_AFTER );
      //:NewReport.GroupSet.Tag     = szFirstEntityName
      SetAttributeFromString( *NewReport, "GroupSet", "Tag", szFirstEntityName );
      //:NewReport.GroupSet.Type    = "E"
      SetAttributeFromString( *NewReport, "GroupSet", "Type", "E" );
      //:NewReport.GroupSet.PSDLG_X = 0
      SetAttributeFromInteger( *NewReport, "GroupSet", "PSDLG_X", 0 );
      //:NewReport.GroupSet.PSDLG_Y = 0
      SetAttributeFromInteger( *NewReport, "GroupSet", "PSDLG_Y", 0 );
      //:NewReport.GroupSet.SZDLG_X = 2176
      SetAttributeFromInteger( *NewReport, "GroupSet", "SZDLG_X", 2176 );
      //:NewReport.GroupSet.SZDLG_Y = 0    // Size here
      SetAttributeFromInteger( *NewReport, "GroupSet", "SZDLG_Y", 0 );

      //:// Create a single Detail Group.
      //:CreateMetaEntity( DialogWindow, NewReport, "Group", zPOS_AFTER )
      CreateMetaEntity( DialogWindow, *NewReport, "Group", zPOS_AFTER );
      //:NewReport.Group.Tag     = "Detail"
      SetAttributeFromString( *NewReport, "Group", "Tag", "Detail" );
      //:NewReport.Group.Type    = "ga"
      SetAttributeFromString( *NewReport, "Group", "Type", "ga" );
      //:NewReport.Group.PSDLG_X = 0
      SetAttributeFromInteger( *NewReport, "Group", "PSDLG_X", 0 );
      //:NewReport.Group.PSDLG_Y = 0
      SetAttributeFromInteger( *NewReport, "Group", "PSDLG_Y", 0 );
      //:NewReport.Group.SZDLG_X = 2176
      SetAttributeFromInteger( *NewReport, "Group", "SZDLG_X", 2176 );
   } 

   //:   // NewReport.Group.SZDLG_Y will be determined by the position of the last Control.
   //:END
   //:DropView( DialogWindowHier )
   DropView( DialogWindowHier );

   //:// Process each Control.
   //:// The processing will be done inside a recursive sub operation, so that we can handle Controls inside Groups.
   //:X_Position = 0
   X_Position = 0;
   //:Y_Position = 0
   Y_Position = 0;
   //:GenerateXRP_Recurs( NewReport, DialogWindow, X_Position, Y_Position, GroupSize )
   oTZRPSRCO_GenerateXRP_Recurs( *NewReport, DialogWindow, X_Position, Y_Position, GroupSize );
   //:NewReport.Group.SZDLG_Y = GroupSize
   SetAttributeFromInteger( *NewReport, "Group", "SZDLG_Y", GroupSize );

   //:// Write out the XRP to a subdirectory of the LPLR.
   //:SysReadZeidonIni( -1, "[Workstation]", "HTML_Reports", szHTML_DirectoryName )
   SysReadZeidonIni( -1, "[Workstation]", "HTML_Reports", szHTML_DirectoryName, zsizeof( szHTML_DirectoryName ) );
   //:IF szHTML_DirectoryName = ""
   if ( ZeidonStringCompare( szHTML_DirectoryName, 1, 0, "", 1, 0, 257 ) == 0 )
   { 
      //:szHTML_DirectoryName = "\HTML_Reports\"
      ZeidonStringCopy( szHTML_DirectoryName, 1, 0, "\\HTML_Reports\\", 1, 0, 257 );
   } 

   //:END

   //:GET VIEW vTaskLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", DialogWindow, zLEVEL_TASK );
   //:szDirectoryFileName = vTaskLPLR.LPLR.ExecDir + szHTML_DirectoryName + szReportName + ".XRP"
   GetStringFromAttribute( szDirectoryFileName, zsizeof( szDirectoryFileName ), vTaskLPLR, "LPLR", "ExecDir" );
   ZeidonStringConcat( szDirectoryFileName, 1, 0, szHTML_DirectoryName, 1, 0, 501 );
   ZeidonStringConcat( szDirectoryFileName, 1, 0, szReportName, 1, 0, 501 );
   ZeidonStringConcat( szDirectoryFileName, 1, 0, ".XRP", 1, 0, 501 );

   //:CommitOI_ToFile( NewReport, szDirectoryFileName, zASCII )
   CommitOI_ToFile( *NewReport, szDirectoryFileName, zASCII );
   //:ActivateOI_FromFile( TestReport, "TZRPSRCO", vTaskLPLR, szDirectoryFileName, zSINGLE )
   ActivateOI_FromFile( &TestReport, "TZRPSRCO", vTaskLPLR, szDirectoryFileName, zSINGLE );
   //:NAME VIEW  TestReport "TestReport"
   SetNameForView( TestReport, "TestReport", 0, zLEVEL_TASK );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ChangeRepFileContents( VIEW ViewToInstance BASED ON LOD TZRPSRCO,
//:                       VIEW CurrentLPLR BASED ON LOD TZCMLPLO,
//:                       STRING ( 513 ) szPathName1,
//:                       STRING ( 513 ) szPathName2,
//:                       STRING ( 129 ) szFileName,
//:                       STRING ( 4 )   szFileExtension,
//:                       SHORT nFunction,
//:                       VIEW  vSubtask )

//:   INTEGER lFHandle
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_ChangeRepFileContents( zVIEW     ViewToInstance,
                                 zVIEW     CurrentLPLR,
                                 zPCHAR    szPathName1,
                                 zPCHAR    szPathName2,
                                 zPCHAR    szFileName,
                                 zPCHAR    szFileExtension,
                                 zSHORT    nFunction,
                                 zVIEW     vSubtask )
{
   zLONG     lFHandle = 0; 
   //:INTEGER lFHandleTmp
   zLONG     lFHandleTmp = 0; 
   //:INTEGER lRC
   zLONG     lRC = 0; 
   //:INTEGER nLineNumber
   zLONG     nLineNumber = 0; 

   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:SHORT   nLen
   zSHORT    nLen = 0; 

   //:SHORT   nPosStart
   zSHORT    nPosStart = 0; 
   //:SHORT   nPosBefore
   zSHORT    nPosBefore = 0; 
   //:SHORT   nPosBeyond
   zSHORT    nPosBeyond = 0; 

   //:STRING ( 2049 )  szMsg           // 2*zLONG_MESSAGE_LTH+1; szMsg includes szFullName
   zCHAR     szMsg[ 2050 ] = { 0 }; 
   //:STRING ( 10000 ) szLine
   zCHAR     szLine[ 10001 ] = { 0 }; 
   //:STRING ( 2049 )  szNewPath       // 4*zMAX_FILESPEC_LTH+1
   zCHAR     szNewPath[ 2050 ] = { 0 }; 
   //:STRING ( 1025 )  szTmpFile       // 2*zMAX_FILESPEC_LTH+1
   zCHAR     szTmpFile[ 1026 ] = { 0 }; 
   //:STRING ( 1025 )  szFullName      // 2*zMAX_FILESPEC_LTH+1
   zCHAR     szFullName[ 1026 ] = { 0 }; 
   //:STRING ( 1025 )  szFullNameXSQ   // 2*zMAX_FILESPEC_LTH+1
   zCHAR     szFullNameXSQ[ 1026 ] = { 0 }; 

   //:STRING ( 10 )    szOldName
   zCHAR     szOldName[ 11 ] = { 0 }; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:// Open the file which should be changed
   //:IF szFileExtension = "REP"
   if ( ZeidonStringCompare( szFileExtension, 1, 0, "REP", 1, 0, 5 ) == 0 )
   { 
      //:szFullName    = szPathName1 + "\" + szFileName + "." + szFileExtension
      ZeidonStringCopy( szFullName, 1, 0, szPathName1, 1, 0, 1026 );
      ZeidonStringConcat( szFullName, 1, 0, "\\", 1, 0, 1026 );
      ZeidonStringConcat( szFullName, 1, 0, szFileName, 1, 0, 1026 );
      ZeidonStringConcat( szFullName, 1, 0, ".", 1, 0, 1026 );
      ZeidonStringConcat( szFullName, 1, 0, szFileExtension, 1, 0, 1026 );
      //:szFullNameXSQ = szPathName2 + "\" + szFileName
      ZeidonStringCopy( szFullNameXSQ, 1, 0, szPathName2, 1, 0, 1026 );
      ZeidonStringConcat( szFullNameXSQ, 1, 0, "\\", 1, 0, 1026 );
      ZeidonStringConcat( szFullNameXSQ, 1, 0, szFileName, 1, 0, 1026 );
      //:ELSE
   } 
   else
   { 
      //:szFullName = szPathName2 + "\" + szFileName + "." + szFileExtension
      ZeidonStringCopy( szFullName, 1, 0, szPathName2, 1, 0, 1026 );
      ZeidonStringConcat( szFullName, 1, 0, "\\", 1, 0, 1026 );
      ZeidonStringConcat( szFullName, 1, 0, szFileName, 1, 0, 1026 );
      ZeidonStringConcat( szFullName, 1, 0, ".", 1, 0, 1026 );
      ZeidonStringConcat( szFullName, 1, 0, szFileExtension, 1, 0, 1026 );
   } 

   //:END
   //:lFHandle = SysOpenFile( vSubtask, szFullName, COREFILE_READ )
   lFHandle = SysOpenFile( vSubtask, szFullName, COREFILE_READ );
   //:IF lFHandle = -1
   if ( lFHandle == -1 )
   { 
      //:szMsg = "Cannot open File " + szFullName
      ZeidonStringCopy( szMsg, 1, 0, "Cannot open File ", 1, 0, 2050 );
      ZeidonStringConcat( szMsg, 1, 0, szFullName, 1, 0, 2050 );
      //:MessageSend( vSubtask, "WD00501", "ChangeRepFileContents",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vSubtask, "WD00501", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// We need a new file to save the changes
   //:szTmpFile = szPathName1 + "\" + "Temp" + "." + szFileExtension
   ZeidonStringCopy( szTmpFile, 1, 0, szPathName1, 1, 0, 1026 );
   ZeidonStringConcat( szTmpFile, 1, 0, "\\", 1, 0, 1026 );
   ZeidonStringConcat( szTmpFile, 1, 0, "Temp", 1, 0, 1026 );
   ZeidonStringConcat( szTmpFile, 1, 0, ".", 1, 0, 1026 );
   ZeidonStringConcat( szTmpFile, 1, 0, szFileExtension, 1, 0, 1026 );
   //:lFHandleTmp = SysOpenFile( vSubtask, szTmpFile, COREFILE_WRITE )
   lFHandleTmp = SysOpenFile( vSubtask, szTmpFile, COREFILE_WRITE );
   //:IF lFHandleTmp = -1
   if ( lFHandleTmp == -1 )
   { 
      //:szMsg = "Cannot open temp. File " + szTmpFile
      ZeidonStringCopy( szMsg, 1, 0, "Cannot open temp. File ", 1, 0, 2050 );
      ZeidonStringConcat( szMsg, 1, 0, szTmpFile, 1, 0, 2050 );
      //:MessageSend( vSubtask, "WD00502", "ChangeRepFileContents",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vSubtask, "WD00502", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:nLineNumber = 0
   nLineNumber = 0;
   //:lRC = zSysReadLine( vSubtask, szLine, lFHandle, 10000 )
   lRC = zSysReadLine( vSubtask, szLine, lFHandle, 10000 );
   //:LOOP WHILE lRC = 1
   while ( lRC == 1 )
   { 

      //:// Count the lines
      //:nLineNumber = nLineNumber + 1
      nLineNumber = nLineNumber + 1;

      //:IF ( szFileExtension = "XSQ" )
      if ( ZeidonStringCompare( szFileExtension, 1, 0, "XSQ", 1, 0, 5 ) == 0 )
      { 
         //:// We change the LPLR-Name in XSQ-File
         //:// We have to find the line LPLR = LPLR-Name
         //:nLen = zstrlen( "LPLR" )
         nLen = zstrlen( "LPLR" );
         //:nRC  = zstrncmpi( "LPLR", szLine, nLen )
         nRC = zstrncmpi( "LPLR", szLine, nLen );
         //:IF nRC = 0
         if ( nRC == 0 )
         { 
            //:// We found line with definition of LPLR-Name
            //:// Copy the new name in that line and stop looping
            //:szLine = "LPLR = " + CurrentLPLR.LPLR.Name
            GetVariableFromAttribute( szTempString_0, 0, 'S', 33, CurrentLPLR, "LPLR", "Name", "", 0 );
            ZeidonStringCopy( szLine, 1, 0, "LPLR = ", 1, 0, 10001 );
            ZeidonStringConcat( szLine, 1, 0, szTempString_0, 1, 0, 10001 );
         } 

         //:END
      } 

      //:END

      //:IF ( szFileExtension = "REP" )
      if ( ZeidonStringCompare( szFileExtension, 1, 0, "REP", 1, 0, 5 ) == 0 )
      { 
         //:// We change pathnames and LPLR-name in REP-File
         //:CASE nLineNumber
         switch( nLineNumber )
         { 

            //:OF 1:
            case 1 :
               //:// The first line contains the pathnames
               //:// Now we change the pathnames in that line.
               //:// a. for *.REP-File
               //:nPosStart  = zSearchSubString( szLine, "REP", "f", 0 )
               nPosStart = zSearchSubString( szLine, "REP", "f", 0 );
               //:nPosBefore = zSearchSubString( szLine, ","  , "b", nPosStart )
               nPosBefore = zSearchSubString( szLine, ",", "b", nPosStart );
               //:nPosBeyond = zSearchSubString( szLine, ","  , "f", nPosStart )
               nPosBeyond = zSearchSubString( szLine, ",", "f", nPosStart );

               //:/* Replace the pathname */
               //:szNewPath = szFullName
               ZeidonStringCopy( szNewPath, 1, 0, szFullName, 1, 0, 2050 );
               //:zSearchAndReplace( szNewPath, 2049, "\", "\\")
               zSearchAndReplace( szNewPath, 2049, "\\", "\\\\" );
               //:nRC = zReplaceSubString( szLine, nPosBefore + 1, nPosBeyond, szNewPath )
               nRC = zReplaceSubString( szLine, nPosBefore + 1, nPosBeyond, szNewPath );
               //:IF nRC = -1
               if ( nRC == -1 )
               { 
                  //:szMsg = "Not enough memory to change the path in file " + szFullName
                  ZeidonStringCopy( szMsg, 1, 0, "Not enough memory to change the path in file ", 1, 0, 2050 );
                  ZeidonStringConcat( szMsg, 1, 0, szFullName, 1, 0, 2050 );
                  //:MessageSend( vSubtask, "WD00503", "ChangeRepFileContents",
                  //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "WD00503", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:RETURN -1
                  return( -1 );
               } 

               //:END

               //:// b. for *.XSQ-File
               //:// The filename needs extension "XSQ"
               //:szNewPath = szFullNameXSQ + "." + "XSQ"
               ZeidonStringCopy( szNewPath, 1, 0, szFullNameXSQ, 1, 0, 2050 );
               ZeidonStringConcat( szNewPath, 1, 0, ".", 1, 0, 2050 );
               ZeidonStringConcat( szNewPath, 1, 0, "XSQ", 1, 0, 2050 );
               //:zSearchAndReplace (szNewPath, 2049, "\", "\\")
               zSearchAndReplace( szNewPath, 2049, "\\", "\\\\" );

               //:nPosStart  = zSearchSubString( szLine, "XSQ", "f", 0 )
               nPosStart = zSearchSubString( szLine, "XSQ", "f", 0 );
               //:nPosBefore = zSearchSubString( szLine, ","  , "b", nPosStart )
               nPosBefore = zSearchSubString( szLine, ",", "b", nPosStart );
               //:nPosBeyond = zSearchSubString( szLine, ","  , "f", nPosStart )
               nPosBeyond = zSearchSubString( szLine, ",", "f", nPosStart );

               //:/* Replace the pathname */
               //:nRC = zReplaceSubString( szLine, nPosBefore + 1, nPosBeyond, szNewPath )
               nRC = zReplaceSubString( szLine, nPosBefore + 1, nPosBeyond, szNewPath );
               //:IF nRC = -1
               if ( nRC == -1 )
               { 
                  //:szMsg = "Not enough memory to change the path in file " + szFullName
                  ZeidonStringCopy( szMsg, 1, 0, "Not enough memory to change the path in file ", 1, 0, 2050 );
                  ZeidonStringConcat( szMsg, 1, 0, szFullName, 1, 0, 2050 );
                  //:MessageSend( vSubtask, "WD00504", "ChangeRepFileContents",
                  //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "WD00504", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:RETURN -1
                  return( -1 );
               } 

               //:END
               //:// If we do the changes for SAVE AS we have to change the Reportname
               //:// First we need the old name to find it for changes at other positions
               //:// in the report
               //:IF nFunction = zCHANGE_REP_SAVEAS
               if ( nFunction == zCHANGE_REP_SAVEAS )
               { 
                  //:// The first entry between the parentheses is the Reportname
                  //:// We have to change it here
                  //:nPosStart = zSearchSubString( szLine, "(,", "f", 0 )
                  nPosStart = zSearchSubString( szLine, "(,", "f", 0 );
                  //:nPosStart = nPosStart + 2
                  nPosStart = nPosStart + 2;
                  //:nPosBeyond = zSearchSubString( szLine, "," , "f", nPosStart )
                  nPosBeyond = zSearchSubString( szLine, ",", "f", nPosStart );
                  //:nLen = nPosBeyond - nPosStart
                  nLen = nPosBeyond - nPosStart;
                  //:lRC = ZeidonStringCopy(szOldName, 1, 0, szLine, nPosStart + 1, nLen, 10 )
                  lRC = ZeidonStringCopy( szOldName, 1, 0, szLine, (zLONG) nPosStart + 1, (zLONG) nLen, 10 );
                  //:IF lRC < 0
                  if ( lRC < 0 )
                  { 
                     //:RETURN -1
                     return( -1 );
                  } 

                  //:END
                  //:nRC = zReplaceSubString( szLine, nPosStart, nPosBeyond, szFileName )
                  nRC = zReplaceSubString( szLine, nPosStart, nPosBeyond, szFileName );
                  //:IF nRC = -1
                  if ( nRC == -1 )
                  { 
                     //:szMsg = "Not enough memory to change the reportname in file " + szFullName
                     ZeidonStringCopy( szMsg, 1, 0, "Not enough memory to change the reportname in file ", 1, 0, 2050 );
                     ZeidonStringConcat( szMsg, 1, 0, szFullName, 1, 0, 2050 );
                     //:MessageSend( vSubtask, "WD00506", "ChangeRepFileContents",
                     //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                     MessageSend( vSubtask, "WD00506", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                     //:RETURN -1
                     return( -1 );
                  } 

                  //:END
                  //:// We have to find the reportname the second time between the parenthese in line 1
                  //:nPosStart  = zSearchSubString( szLine, szOldName, "f", 0 )
                  nPosStart = zSearchSubString( szLine, szOldName, "f", 0 );
                  //:nPosBeyond = zSearchSubString( szLine, ";", "f", nPosStart )
                  nPosBeyond = zSearchSubString( szLine, ";", "f", nPosStart );
                  //:nRC = zReplaceSubString( szLine, nPosStart, nPosBeyond, szFileName )
                  nRC = zReplaceSubString( szLine, nPosStart, nPosBeyond, szFileName );
                  //:IF nRC = -1
                  if ( nRC == -1 )
                  { 
                     //:szMsg = "Not enough memory to change the reportname in file " + szFullName
                     ZeidonStringCopy( szMsg, 1, 0, "Not enough memory to change the reportname in file ", 1, 0, 2050 );
                     ZeidonStringConcat( szMsg, 1, 0, szFullName, 1, 0, 2050 );
                     //:MessageSend( vSubtask, "WD00507", "ChangeRepFileContents",
                     //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                     MessageSend( vSubtask, "WD00507", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                     //:RETURN -1
                     return( -1 );
                  } 

                  //:END
               } 

               //:END
               break ;

            //:OF 2:
            case 2 :
               //:// Line 2 contains the LPLR name
               //:nPosStart  = zSearchSubString( szLine, ";;)", "f", 0 )
               nPosStart = zSearchSubString( szLine, ";;)", "f", 0 );
               //:nPosBefore = zSearchSubString( szLine, ","  , "b", nPosStart )
               nPosBefore = zSearchSubString( szLine, ",", "b", nPosStart );
               //:nRC = zReplaceSubString (szLine, nPosBefore+1, nPosStart, CurrentLPLR.LPLR.Name )
               GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), CurrentLPLR, "LPLR", "Name" );
               nRC = zReplaceSubString( szLine, nPosBefore + 1, nPosStart, szTempString_1 );
               //:IF nRC = -1
               if ( nRC == -1 )
               { 
                  //:szMsg = "Not enough memory to change the LPLR name in file " + szFullName
                  ZeidonStringCopy( szMsg, 1, 0, "Not enough memory to change the LPLR name in file ", 1, 0, 2050 );
                  ZeidonStringConcat( szMsg, 1, 0, szFullName, 1, 0, 2050 );
                  //:MessageSend( vSubtask, "WD00505", "ChangeRepFileContents",
                  //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "WD00505", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:RETURN -1
                  return( -1 );
               } 

               //:END
               break ;
            //:OTHERWISE :
            default  :
               //:IF nFunction = zCHANGE_REP_SAVEAS
               if ( nFunction == zCHANGE_REP_SAVEAS )
               { 
                  //:// Search the Reportname in the other lines to change it
                  //:// if we do the changes for SAVE AS
                  //:nPosStart = zSearchSubString( szLine, szOldName, "f", 0 )
                  nPosStart = zSearchSubString( szLine, szOldName, "f", 0 );
                  //:IF nPosStart != -1
                  if ( nPosStart != -1 )
                  { 
                     //:nPosBeyond = zSearchSubString( szLine, ";", "f", nPosStart )
                     nPosBeyond = zSearchSubString( szLine, ";", "f", nPosStart );
                     //:nRC = zReplaceSubString( szLine, nPosStart, nPosBeyond, szFileName )
                     nRC = zReplaceSubString( szLine, nPosStart, nPosBeyond, szFileName );
                     //:IF nRC = -1
                     if ( nRC == -1 )
                     { 
                        //:szMsg = "Not enough memory to change the reportname in file " + szFullName
                        ZeidonStringCopy( szMsg, 1, 0, "Not enough memory to change the reportname in file ", 1, 0, 2050 );
                        ZeidonStringConcat( szMsg, 1, 0, szFullName, 1, 0, 2050 );
                        //:MessageSend( vSubtask, "WD00508", "ChangeRepFileContents",
                        //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                        MessageSend( vSubtask, "WD00508", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                        //:RETURN -1
                        return( -1 );
                     } 

                     //:END
                  } 

                  //:END
               } 

               //:END
               break ;
         } 

         //:END
      } 

      //:END

      //:SysWriteLine( vSubtask,lFHandleTmp, szLine )
      SysWriteLine( vSubtask, lFHandleTmp, szLine );
      //:lRC = zSysReadLine( vSubtask, szLine, lFHandle, 10000 )
      lRC = zSysReadLine( vSubtask, szLine, lFHandle, 10000 );
   } 


   //:END

   //:SysCloseFile( vSubtask, lFHandle, 0 )
   SysCloseFile( vSubtask, lFHandle, 0 );
   //:SysCloseFile( vSubtask, lFHandleTmp, 0 )
   SysCloseFile( vSubtask, lFHandleTmp, 0 );

   //:// The changes are now in the temporary file. Rename it for the system
   //:SysRenameFile( vSubtask, szTmpFile, szFullName, TRUE )
   SysRenameFile( vSubtask, szTmpFile, szFullName, TRUE );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:CopyControl( VIEW NewR   BASED ON LOD TZWDLGSO,
//:             VIEW NewRC  BASED ON LOD TZWDLGSO,
//:             VIEW OrigR  BASED ON LOD TZWDLGSO,
//:             VIEW OrigRC BASED ON LOD TZWDLGSO,
//:             VIEW vPE    BASED ON LOD TZPESRCO,
//:             VIEW SourceLPLR,
//:             VIEW vSubtask )

//:   SHORT        nRC
static zSHORT
oTZRPSRCO_CopyControl( zVIEW     NewR,
                       zVIEW     NewRC,
                       zVIEW     OrigR,
                       zVIEW     OrigRC,
                       zVIEW     vPE,
                       zVIEW     SourceLPLR,
                       zVIEW     vSubtask )
{
   zSHORT    nRC = 0; 
   //:STRING (128) szMsg
   zCHAR     szMsg[ 129 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 


   //:// Note that the views that are passed are considered as TZWDLGSO objects.
   //:// This is so that we can use the CloneCtrlMap operation for the TZWDLGSO LOD. Since
   //:// the subobject is the same, there is no problem in assuming the object instance
   //:// passed is of that LOD type.

   //:CreateMetaEntity( vSubtask, NewRC, "Control", zPOS_AFTER )
   CreateMetaEntity( vSubtask, NewRC, "Control", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewRC,  "Control",
   //:                             OrigRC, "Control", zSET_NULL )
   SetMatchingAttributesByName( NewRC, "Control", OrigRC, "Control", zSET_NULL );

   //:SET CURSOR FIRST vPE.ControlDef WHERE
   //:                 vPE.ControlDef.Tag = OrigRC.ControlDef.Tag
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigRC, "ControlDef", "Tag" );
   RESULT = SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", szTempString_0, "" );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:IncludeSubobjectFromSubobject( NewRC, "ControlDef",
      //:                               vPE,   "ControlDef", zPOS_AFTER )
      IncludeSubobjectFromSubobject( NewRC, "ControlDef", vPE, "ControlDef", zPOS_AFTER );

      //:// To create the CtrlMap substructure, call the reusable routine in the
      //:// TZWDLGSO LOD.
      //:FOR EACH OrigRC.CtrlMap
      RESULT = SetCursorFirstEntity( OrigRC, "CtrlMap", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:nRC = CloneCtrlMap( NewR, NewRC, OrigR, OrigRC, SourceLPLR, vSubtask )
         nRC = oTZWDLGSO_CloneCtrlMap( NewR, NewRC, OrigR, OrigRC, SourceLPLR, vSubtask );
         //:IF nRC = -1
         if ( nRC == -1 )
         { 
            //:RETURN -1
            return( -1 );
         } 

         RESULT = SetCursorNextEntity( OrigRC, "CtrlMap", "" );
         //:END
      } 

      //:END

      //:ELSE
   } 
   else
   { 
      //:szMsg = "ControlDef not found for Control: " + NewRC.Control.Tag
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, NewRC, "Control", "Tag", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "ControlDef not found for Control: ", 1, 0, 129 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 129 );
      //:MessageSend( vSubtask, "WD00501", "Report Migrate",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "WD00501", "Report Migrate", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
   } 


   //:END

   //:FOR EACH OrigRC.CtrlCtrl
   RESULT = SetCursorFirstEntity( OrigRC, "CtrlCtrl", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( NewRC, "CtrlCtrl" )
      SetViewToSubobject( NewRC, "CtrlCtrl" );
      //:SetViewToSubobject( OrigRC, "CtrlCtrl" )
      SetViewToSubobject( OrigRC, "CtrlCtrl" );
      //:CopyControl( NewR,
      //:             NewRC,
      //:             OrigR,
      //:             OrigRC,
      //:             vPE,
      //:             SourceLPLR, vSubtask )
      oTZRPSRCO_CopyControl( NewR, NewRC, OrigR, OrigRC, vPE, SourceLPLR, vSubtask );
      //:ResetViewFromSubobject( OrigRC )
      ResetViewFromSubobject( OrigRC );
      //:ResetViewFromSubobject( NewRC )
      ResetViewFromSubobject( NewRC );
      RESULT = SetCursorNextEntity( OrigRC, "CtrlCtrl", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ReportCopy( VIEW NewReport  BASED ON LOD TZRPSRCO,
//:            VIEW OrigReport BASED ON LOD TZRPSRCO,
//:            VIEW SourceLPLR BASED ON LOD TZCMLPLO,
//:            VIEW vSubtask )

//:   VIEW NewReportRecursive   BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_ReportCopy( zPVIEW    NewReport,
                      zVIEW     OrigReport,
                      zVIEW     SourceLPLR,
                      zVIEW     vSubtask )
{
   zVIEW     NewReportRecursive = 0; 
   //:VIEW OrigReportRecursive  BASED ON LOD TZRPSRCO
   zVIEW     OrigReportRecursive = 0; 
   //:VIEW NewDialog            BASED ON LOD TZWDLGSO
   zVIEW     NewDialog = 0; 
   //:VIEW vPE                  BASED ON LOD TZPESRCO
   zVIEW     vPE = 0; 
   //:VIEW PE_List              BASED ON LOD TZCMLPLO
   zVIEW     PE_List = 0; 
   //:STRING (100)   szMG_ErrorMessage
   zCHAR     szMG_ErrorMessage[ 101 ] = { 0 }; 
   //:STRING ( 33)   szReportName
   zCHAR     szReportName[ 34 ] = { 0 }; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 


   //:// Create the new report empty and add the root.
   //:ActivateEmptyMetaOI( vSubtask, NewReport, zSOURCE_REPORT_META, zSINGLE )
   ActivateEmptyMetaOI( vSubtask, NewReport, zSOURCE_REPORT_META, zSINGLE );
   //:NAME VIEW NewReport "NewReport"
   SetNameForView( *NewReport, "NewReport", 0, zLEVEL_TASK );
   //:CreateMetaEntity( vSubtask, NewReport, "Report", zPOS_AFTER )
   CreateMetaEntity( vSubtask, *NewReport, "Report", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewReport,  "Report",
   //:                             OrigReport, "Report", zSET_NULL )
   SetMatchingAttributesByName( *NewReport, "Report", OrigReport, "Report", zSET_NULL );

   //://  Get View to Presentation Environment.
   //:IF GetViewByName( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) < 0
   lTempInteger_0 = GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   if ( lTempInteger_0 < 0 )
   { 
      //:RetrieveViewForMetaList( vSubtask, PE_List, zREFER_PENV_META )
      RetrieveViewForMetaList( vSubtask, &PE_List, zREFER_PENV_META );
      //:szReportName = OrigReport.Report.Tag
      GetVariableFromAttribute( szReportName, 0, 'S', 34, OrigReport, "Report", "Tag", "", 0 );
      //:szMG_ErrorMessage = "Report (" + szReportName + ") aborted."
      ZeidonStringCopy( szMG_ErrorMessage, 1, 0, "Report (", 1, 0, 101 );
      ZeidonStringConcat( szMG_ErrorMessage, 1, 0, szReportName, 1, 0, 101 );
      ZeidonStringConcat( szMG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 101 );
      //:nRC = LoadZeidonPPE( vSubtask, vPE, zREFER_PENV_META, PE_List,
      //:                     "Configuration Management", szMG_ErrorMessage )
      nRC = LoadZeidonPPE( vSubtask, &vPE, zREFER_PENV_META, PE_List, "Configuration Management", szMG_ErrorMessage );
      //:DropView( PE_List )
      DropView( PE_List );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN -16
         return( -16 );
      } 

      //:END

      //:SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK )
      SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   } 

   //:END

   //:// Copy each Registered View Name that is used by the Report.
   //:FOR EACH OrigReport.ViewObjRef
   RESULT = SetCursorFirstEntity( OrigReport, "ViewObjRef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// The following routine not only positions on the VOR but also
      //:// includes it, if it doesn't already exist.
      //:// We use NewDialog instead of NewReport as first parameter in order to
      //:// identify the LOD where the operation exists.
      //:NewDialog = NewReport
      NewDialog = *NewReport;
      //:PositionOnVOR( NewDialog,  OrigReport,
      //:               SourceLPLR, OrigReport.ViewObjRef.Name, vSubtask )
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigReport, "ViewObjRef", "Name" );
      oTZWDLGSO_PositionOnVOR( NewDialog, OrigReport, SourceLPLR, szTempString_0, vSubtask );
      RESULT = SetCursorNextEntity( OrigReport, "ViewObjRef", "" );
   } 

   //:END

   //:// Build the Page substructure.
   //:FOR EACH OrigReport.Page
   RESULT = SetCursorFirstEntity( OrigReport, "Page", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:CreateMetaEntity( vSubtask, NewReport, "Page", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewReport, "Page", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewReport,  "Page",
      //:                             OrigReport, "Page", zSET_NULL )
      SetMatchingAttributesByName( *NewReport, "Page", OrigReport, "Page", zSET_NULL );
      //:NewReport.Page.Tag = NewReport.Report.Tag
      SetAttributeFromAttribute( *NewReport, "Page", "Tag", *NewReport, "Report", "Tag" );

      //:FOR EACH OrigReport.GroupSet
      RESULT = SetCursorFirstEntity( OrigReport, "GroupSet", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:CreateMetaEntity( vSubtask, NewReport, "GroupSet", zPOS_AFTER )
         CreateMetaEntity( vSubtask, *NewReport, "GroupSet", zPOS_AFTER );
         //:SetMatchingAttributesByName( NewReport,  "GroupSet",
         //:                             OrigReport, "GroupSet", zSET_NULL )
         SetMatchingAttributesByName( *NewReport, "GroupSet", OrigReport, "GroupSet", zSET_NULL );

         //:FOR EACH OrigReport.Group
         RESULT = SetCursorFirstEntity( OrigReport, "Group", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 

            //:CreateMetaEntity( vSubtask, NewReport, "Group", zPOS_AFTER )
            CreateMetaEntity( vSubtask, *NewReport, "Group", zPOS_AFTER );
            //:SetMatchingAttributesByName( NewReport,  "Group",
            //:                             OrigReport, "Group", zSET_NULL )
            SetMatchingAttributesByName( *NewReport, "Group", OrigReport, "Group", zSET_NULL );
            //:FOR EACH OrigReport.Control
            RESULT = SetCursorFirstEntity( OrigReport, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateViewFromViewForTask( NewReportRecursive, NewReport, 0 )
               CreateViewFromViewForTask( &NewReportRecursive, *NewReport, 0 );
               //:CreateViewFromViewForTask( OrigReportRecursive, OrigReport, 0 )
               CreateViewFromViewForTask( &OrigReportRecursive, OrigReport, 0 );
               //:CopyControl( NewReport,
               //:             NewReportRecursive,
               //:             OrigReport,
               //:             OrigReportRecursive,
               //:             vPE,
               //:             SourceLPLR,
               //:             vSubtask )
               oTZRPSRCO_CopyControl( *NewReport, NewReportRecursive, OrigReport, OrigReportRecursive, vPE, SourceLPLR, vSubtask );
               //:DropView( NewReportRecursive )
               DropView( NewReportRecursive );
               //:DropView( OrigReportRecursive )
               DropView( OrigReportRecursive );
               RESULT = SetCursorNextEntity( OrigReport, "Control", "" );
            } 

            RESULT = SetCursorNextEntity( OrigReport, "Group", "" );
            //:END
         } 

         RESULT = SetCursorNextEntity( OrigReport, "GroupSet", "" );

         //:END
      } 

      RESULT = SetCursorNextEntity( OrigReport, "Page", "" );
      //:END
   } 

   //:END
   //:RETURN 0
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ReportRelinkDelete( VIEW vReport BASED ON LOD TZRPSRCO, VIEW vSubtask )

//:   VIEW         vLastLOD     BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_ReportRelinkDelete( zVIEW     vReport,
                              zVIEW     vSubtask )
{
   zVIEW     vLastLOD = 0; 
   //:VIEW         vVOR         BASED ON LOD TZWDVORO
   zVIEW     vVOR = 0; 
   //:VIEW         vLOD_LPLR    BASED ON LOD TZCMLPLO
   zVIEW     vLOD_LPLR = 0; 
   //:VIEW         vRecursive   BASED ON LOD TZRPSRCO
   zVIEW     vRecursive = 0; 
   //:VIEW         vDialog      BASED ON LOD TZWDLGSO
   zVIEW     vDialog = 0; 
   //:INTEGER      lLastViewZKey
   zLONG     lLastViewZKey = 0; 
   //:STRING (128) szMsg
   zCHAR     szMsg[ 129 ] = { 0 }; 
   //:SHORT        nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 


   //:RetrieveViewForMetaList( vSubtask, vLOD_LPLR, zREFER_LOD_META )
   RetrieveViewForMetaList( vSubtask, &vLOD_LPLR, zREFER_LOD_META );
   //:lLastViewZKey = 0
   lLastViewZKey = 0;
   //:// NAME VIEW vReport "vReport"
   //:SetNameForView( vReport, "vReport", vSubtask, zLEVEL_TASK )
   SetNameForView( vReport, "vReport", vSubtask, zLEVEL_TASK );

   //:FOR EACH vReport.ViewObjRef
   RESULT = SetCursorFirstEntity( vReport, "ViewObjRef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF lLastViewZKey != 0
      if ( lLastViewZKey != 0 )
      { 
         //:DropMetaOI( vSubtask, vLastLOD )
         DropMetaOI( vSubtask, vLastLOD );
         //:lLastViewZKey = 0
         lLastViewZKey = 0;
      } 

      //:END

      //:nRC = ActivateMetaOI_ByZKey( vSubtask, vVOR, 0, zREFER_VOR_META,
      //:                             zSINGLE, vReport.ViewObjRef.ZKey, 0 )
      GetIntegerFromAttribute( &lTempInteger_0, vReport, "ViewObjRef", "ZKey" );
      nRC = ActivateMetaOI_ByZKey( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, lTempInteger_0, 0 );
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:nRC = ActivateMetaOI_ByName( vSubtask, vLastLOD, 0, zREFER_LOD_META,
         //:                             zSINGLE, vVOR.LOD.Name, 0 )
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vVOR, "LOD", "Name" );
         nRC = ActivateMetaOI_ByName( vSubtask, &vLastLOD, 0, zREFER_LOD_META, zSINGLE, szTempString_0, 0 );
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:lLastViewZKey = vReport.ViewObjRef.ZKey
            GetIntegerFromAttribute( &lLastViewZKey, vReport, "ViewObjRef", "ZKey" );
            //:NAME VIEW vLastLOD "vLastLOD"
            SetNameForView( vLastLOD, "vLastLOD", 0, zLEVEL_TASK );
            //:// Relink ViewObjRef & LOD
            //://RelinkInstanceToInstance( vReport, "ViewObjRef", vVOR, "ViewObjRef" )
            //://RelinkInstanceToInstance( vReport, "LOD", vLastLOD, "LOD" )


            //:// Since RelinkInstanceToInstance does not reset the include flags,
            //:// use RelinkAllSubobjectsForOI for relinking entities that need those include
            //:// flags reset.  When a version of RelinkInstanceToInstance is created
            //:// that resets those flags, the following code can be removed.
            //:RelinkAllSubobjectsForOI( vReport, "ViewObjRef", vVOR, "ViewObjRef" )
            RelinkAllSubobjectsForOI( vReport, "ViewObjRef", vVOR, "ViewObjRef" );
            //:RelinkAllSubobjectsForOI( vReport, "CtrlMapLOD_Attribute", vLastLOD, "LOD_Attribute" )
            RelinkAllSubobjectsForOI( vReport, "CtrlMapLOD_Attribute", vLastLOD, "LOD_Attribute" );

            //:ELSE
         } 
         else
         { 
            //:szMsg = "Deleting Report View: " + vReport.ViewObjRef.Name +
            //:        "|Missing LOD: " + vReport.LOD.Name
            GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vReport, "ViewObjRef", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "Deleting Report View: ", 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, "Missing LOD: ", 1, 0, 129 );
            GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vReport, "LOD", "Name", "", 0 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_2, 1, 0, 129 );
            //:MessageSend( vSubtask, "WD00501", "Report Relink",
            //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "WD00501", "Report Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:EXCLUDE vReport.ViewObjRef NONE
            RESULT = ExcludeEntity( vReport, "ViewObjRef", zREPOS_NONE );
         } 

         //:END
         //:DropMetaOI( vSubtask, vVOR )
         DropMetaOI( vSubtask, vVOR );
         //:ELSE
      } 
      else
      { 
         //:nRC = ActivateMetaOI_ByName( vSubtask, vVOR, 0, zREFER_VOR_META,
         //:                             zSINGLE, vReport.ViewObjRef.Name , 0 )
         GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), vReport, "ViewObjRef", "Name" );
         nRC = ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, szTempString_3, 0 );
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:EXCLUDE vReport.ViewObjRef NONE
            RESULT = ExcludeEntity( vReport, "ViewObjRef", zREPOS_NONE );
            //:INCLUDE vReport.ViewObjRef FROM vVOR.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( vReport, "ViewObjRef", vVOR, "ViewObjRef", zPOS_AFTER );
            //:DropMetaOI( vSubtask, vVOR )
            DropMetaOI( vSubtask, vVOR );
            //:ELSE
         } 
         else
         { 
            //:szMsg = "Deleting Report View: " + vReport.ViewObjRef.Name +
            //:        "|Missing Registered View."
            GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vReport, "ViewObjRef", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "Deleting Report View: ", 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_4, 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, "Missing Registered View.", 1, 0, 129 );
            //:MessageSend( vSubtask, "WD00502", "Report Relink",
            //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "WD00502", "Report Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:EXCLUDE vReport.ViewObjRef NONE
            RESULT = ExcludeEntity( vReport, "ViewObjRef", zREPOS_NONE );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vReport, "ViewObjRef", "" );
      //:END
   } 

   //:END

   //:FOR EACH vReport.Group WITHIN vReport.Report
   RESULT = SetCursorFirstEntity( vReport, "Group", "Report" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateViewFromViewForTask( vRecursive, vReport, 0 )
      CreateViewFromViewForTask( &vRecursive, vReport, 0 );
      //:NAME VIEW vRecursive "vRecursive"
      SetNameForView( vRecursive, "vRecursive", 0, zLEVEL_TASK );
      //:FOR EACH vRecursive.Control
      RESULT = SetCursorFirstEntity( vRecursive, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:// Use Dialog view in ControlRelinkDelete call so that operation is
         //:// found under the TZWDLGSO object.
         //:vDialog = vReport
         vDialog = vReport;
         //:ControlRelinkDelete( vDialog, vRecursive, vLastLOD,
         //:                     lLastViewZKey, vLOD_LPLR, vSubtask )
         oTZWDLGSO_ControlRelinkDelete( vDialog, vRecursive, &vLastLOD, &lLastViewZKey, vLOD_LPLR, vSubtask );
         RESULT = SetCursorNextEntity( vRecursive, "Control", "" );
      } 

      //:END
      //:DropView( vRecursive )
      DropView( vRecursive );
      RESULT = SetCursorNextEntity( vReport, "Group", "Report" );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:ReportMigrate( VIEW          NewReport  BASED ON LOD TZRPSRCO,
//:               STRING ( 32 ) ReportName,
//:               VIEW          SourceLPLR BASED ON LOD TZCMLPLO,
//:               VIEW          vSubtask )

//:   VIEW SourceLPLR2 BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_ReportMigrate( zPVIEW    NewReport,
                         zPCHAR    ReportName,
                         zVIEW     SourceLPLR,
                         zVIEW     vSubtask )
{
   zVIEW     SourceLPLR2 = 0; 
   //:VIEW CurrentLPLR BASED ON LOD TZCMLPLO
   zVIEW     CurrentLPLR = 0; 
   //:VIEW OrigReport  BASED ON LOD TZRPSRCO
   zVIEW     OrigReport = 0; 

   //:STRING ( 513 ) SourceFileName           // zMAX_FILESPEC_LTH + 1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 32 )  SourceName
   zCHAR     SourceName[ 33 ] = { 0 }; 
   //:STRING ( 513 ) SourceFileName1          // zMAX_FILESPEC_LTH + 1
   zCHAR     SourceFileName1[ 514 ] = { 0 }; 
   //:STRING ( 513 ) SourceFileName2          // zMAX_FILESPEC_LTH + 1
   zCHAR     SourceFileName2[ 514 ] = { 0 }; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zCHAR     szTempString_3[ 255 ]; 


   //:// Activate existing source meta OrigReport
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + ReportName + ".PRP"
   GetStringFromAttribute( SourceFileName, zsizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ReportName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PRP", 1, 0, 514 );

   //:ActivateOI_FromFile( OrigReport, "TZRPSRCO", SourceLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &OrigReport, "TZRPSRCO", SourceLPLR, SourceFileName, 8192 );
   //:// 8192 is zIGNORE_ATTRIB_ERRORS
   //:NAME VIEW OrigReport "OrigReport"
   SetNameForView( OrigReport, "OrigReport", 0, zLEVEL_TASK );

   //:CreateViewFromViewForTask( SourceLPLR2, SourceLPLR, 0 )
   CreateViewFromViewForTask( &SourceLPLR2, SourceLPLR, 0 );

   //:// Call operation that will actually do the work of creating the report.
   //:nRC = ReportCopy( NewReport, OrigReport, SourceLPLR2, vSubtask )
   nRC = oTZRPSRCO_ReportCopy( NewReport, OrigReport, SourceLPLR2, vSubtask );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:RETURN nRC
      return( nRC );
   } 

   //:END

   //:// If this Report Definition is for a Siron Report, copy the related files
   //:// which have extensions .REP and .XSQ.
   //:IF NewReport.Report.SironName != ""
   if ( CompareAttributeToString( *NewReport, "Report", "SironName", "" ) != 0 )
   { 
      //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zSOURCE_ERD_META ) // Get a view for directory info.
      RetrieveViewForMetaList( vSubtask, &CurrentLPLR, zSOURCE_ERD_META );
      //:ResetViewFromSubobject( CurrentLPLR ) // Get visibility to root.
      ResetViewFromSubobject( CurrentLPLR );
      //:SourceName = NewReport.Report.SironName
      GetVariableFromAttribute( SourceName, 0, 'S', 33, *NewReport, "Report", "SironName", "", 0 );

      //:// Copy REP file.
      //:SourceFileName1 = SourceLPLR.LPLR.PgmSrcDir + "\" + SourceName + ".REP"
      GetStringFromAttribute( SourceFileName1, zsizeof( SourceFileName1 ), SourceLPLR, "LPLR", "PgmSrcDir" );
      ZeidonStringConcat( SourceFileName1, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName1, 1, 0, SourceName, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName1, 1, 0, ".REP", 1, 0, 514 );
      //:SourceFileName2 = CurrentLPLR.LPLR.PgmSrcDir + "\" + SourceName + ".REP"
      GetStringFromAttribute( SourceFileName2, zsizeof( SourceFileName2 ), CurrentLPLR, "LPLR", "PgmSrcDir" );
      ZeidonStringConcat( SourceFileName2, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName2, 1, 0, SourceName, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName2, 1, 0, ".REP", 1, 0, 514 );
      //:SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE )
      SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE );
      //:ChangeRepFileContents( OrigReport, CurrentLPLR, CurrentLPLR.LPLR.PgmSrcDir, CurrentLPLR.LPLR.ExecDir,
      //:                       SourceName, "REP", 0, vSubtask )
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), CurrentLPLR, "LPLR", "PgmSrcDir" );
      GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), CurrentLPLR, "LPLR", "ExecDir" );
      oTZRPSRCO_ChangeRepFileContents( OrigReport, CurrentLPLR, szTempString_0, szTempString_1, SourceName, "REP", 0, vSubtask );

      //:// Copy XSQ file.
      //:SourceFileName1 = SourceLPLR.LPLR.ExecDir + "\" + SourceName + ".XSQ"
      GetStringFromAttribute( SourceFileName1, zsizeof( SourceFileName1 ), SourceLPLR, "LPLR", "ExecDir" );
      ZeidonStringConcat( SourceFileName1, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName1, 1, 0, SourceName, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName1, 1, 0, ".XSQ", 1, 0, 514 );
      //:SourceFileName2 = CurrentLPLR.LPLR.ExecDir + "\" + SourceName + ".XSQ"
      GetStringFromAttribute( SourceFileName2, zsizeof( SourceFileName2 ), CurrentLPLR, "LPLR", "ExecDir" );
      ZeidonStringConcat( SourceFileName2, 1, 0, "\\", 1, 0, 514 );
      ZeidonStringConcat( SourceFileName2, 1, 0, SourceName, 1, 0, 514 );
      ZeidonStringConcat( SourceFileName2, 1, 0, ".XSQ", 1, 0, 514 );
      //:SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE )
      SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE );
      //:ChangeRepFileContents( OrigReport, CurrentLPLR, CurrentLPLR.LPLR.PgmSrcDir, CurrentLPLR.LPLR.ExecDir,
      //:                       SourceName, "XSQ", 0, vSubtask )
      GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), CurrentLPLR, "LPLR", "PgmSrcDir" );
      GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), CurrentLPLR, "LPLR", "ExecDir" );
      oTZRPSRCO_ChangeRepFileContents( OrigReport, CurrentLPLR, szTempString_2, szTempString_3, SourceName, "XSQ", 0, vSubtask );
   } 

   //:END

   //:// Clean up views.
   //:DropMetaOI( vSubtask, OrigReport )
   DropMetaOI( vSubtask, OrigReport );
   //:DropView( SourceLPLR2 )
   DropView( SourceLPLR2 );

   //:// Finally, commit the Report Def.
   //:CommitMetaOI( vSubtask, NewReport, zSOURCE_REPORT_META )
   CommitMetaOI( vSubtask, *NewReport, zSOURCE_REPORT_META );
   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:AdjustGroupSize( VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:                 INTEGER lPageWidth )

//:   VIEW    vTempReport
static zSHORT
oTZRPSRCO_AdjustGroupSize( zVIEW     vReportDef,
                           zLONG     lPageWidth )
{
   zVIEW     vTempReport = 0; 
   //:INTEGER lGroupMaxPosX
   zLONG     lGroupMaxPosX = 0; 
   //:INTEGER lLeftMargin
   zLONG     lLeftMargin = 0; 
   //:INTEGER lCtrlPos
   zLONG     lCtrlPos = 0; 
   //:INTEGER lCtrlSize
   zLONG     lCtrlSize = 0; 
   //:INTEGER lRC
   zLONG     lRC = 0; 
   //:INTEGER nSkipFirstFlag
   zLONG     nSkipFirstFlag = 0; 
   //:DECIMAL a1
   ZDecimal  a1 = 0.0; 
   //:DECIMAL b1
   ZDecimal  b1 = 0.0; 
   //:DECIMAL b2
   ZDecimal  b2 = 0.0; 
   //:DECIMAL dFactor
   ZDecimal  dFactor = 0.0; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 


   //:// The purpose of this routine is adjust the X positions and sizes of
   //:// the controls in a group so that they will fit onto the page.  It is
   //:// clearly possible that the user will specify too many attributes for
   //:// an entity to be able to be displayed horizontally.  In this case,
   //:// CreateGroup(Set)ForEntity will continue to add controls as if the
   //:// there is no limit with regards to page width.  Then, it will call
   //:// this routine.  Instead of simply chopping off the controls that
   //:// were specified to the right of the page end, we will compress
   //:// all of the X positions and sizes so that all of the controls can
   //:// be seen, even if this makes them too small to be useful. We will
   //:// let the user decide after the autodesign whether to adjust the
   //:// sizes and positions that we have generated or to use vertical
   //:// spacing instead.


   //:// If we have a header group with header text on one line
   //:// and attribute headers on another, don't adjust the first
   //:// control which is the header text
   //:nSkipFirstFlag = 0   // 0  false
   nSkipFirstFlag = 0;
   //:IF vReportDef.Group.Type = "gh"
   if ( CompareAttributeToString( vReportDef, "Group", "Type", "gh" ) == 0 )
   { 
      //:SetCursorFirstEntity( vReportDef, "Control", "" )
      SetCursorFirstEntity( vReportDef, "Control", "" );
      //:CreateViewFromViewForTask( vTempReport, vReportDef, 0 )
      CreateViewFromViewForTask( &vTempReport, vReportDef, 0 );
      //:lRC = SetCursorNextEntity( vTempReport, "Control", "" )
      lRC = SetCursorNextEntity( vTempReport, "Control", "" );
      //:IF lRC >= zCURSOR_SET
      if ( lRC >= zCURSOR_SET )
      { 
         //:IF vReportDef.Control.PSDLG_Y < vTempReport.Control.PSDLG_Y
         if ( CompareAttributeToAttribute( vReportDef, "Control", "PSDLG_Y", vTempReport, "Control", "PSDLG_Y" ) < 0 )
         { 
            //:nSkipFirstFlag = 1  // 1 = true
            nSkipFirstFlag = 1;
         } 

         //:END
      } 

      //:END
      //:DropView( vTempReport )
      DropView( vTempReport );
      //:SetCursorLastEntity( vReportDef, "Control", "" )
      SetCursorLastEntity( vReportDef, "Control", "" );
   } 

   //:END


   //:// Only adjust if Group size plus position is greater that the page width.
   //:lGroupMaxPosX = vReportDef.Control.PSDLG_X + vReportDef.Control.SZDLG_X
   GetIntegerFromAttribute( &lTempInteger_0, vReportDef, "Control", "PSDLG_X" );
   GetIntegerFromAttribute( &lTempInteger_1, vReportDef, "Control", "SZDLG_X" );
   lGroupMaxPosX = lTempInteger_0 + lTempInteger_1;
   //:TraceLineS("", "" )
   TraceLineS( "", "" );
   //:TraceLineI( "--->lPageWidth  : ", lPageWidth )
   TraceLineI( "--->lPageWidth  : ", lPageWidth );
   //:TraceLineI( "--->lGroupMaxPosX : ", lGroupMaxPosX )
   TraceLineI( "--->lGroupMaxPosX : ", lGroupMaxPosX );
   //:IF lGroupMaxPosX > lPageWidth
   if ( lGroupMaxPosX > lPageWidth )
   { 

      //:// We won't adjust the position of the first control, only the size.
      //:SetCursorFirstEntity( vReportDef, "Control", "" )
      SetCursorFirstEntity( vReportDef, "Control", "" );
      //:IF nSkipFirstFlag > 0  // 0 = false
      if ( nSkipFirstFlag > 0 )
      { 
         //:SetCursorNextEntity( vReportDef, "Control", "" )
         SetCursorNextEntity( vReportDef, "Control", "" );
      } 

      //:END
      //:lLeftMargin = vReportDef.Control.PSDLG_X
      GetIntegerFromAttribute( &lLeftMargin, vReportDef, "Control", "PSDLG_X" );
      //:TraceLineI( "--->lLeftMargin : ", lLeftMargin )
      TraceLineI( "--->lLeftMargin : ", lLeftMargin );

      //:// set the scaling factor
      //:a1 = lLeftMargin
      a1 = lLeftMargin;
      //:b1 = lGroupMaxPosX
      b1 = lGroupMaxPosX;
      //:b2 = lPageWidth
      b2 = lPageWidth;
      //:dFactor = (b2 - a1) / (b1 - a1)
      dFactor = ( b2 - a1 ) / ( b1 - a1 );

      //:// adjust the size and hold onto curr position and curr size
      //:// The conversion from Decimal to Integer will truncate, so
      //:// we add 0.5 to force rounding off
      //:lCtrlSize = (vReportDef.Control.SZDLG_X * dFactor) + 0.5
      GetIntegerFromAttribute( &lTempInteger_2, vReportDef, "Control", "SZDLG_X" );
      lCtrlSize = ( lTempInteger_2 * dFactor ) + 0.5;
      //:vReportDef.Control.SZDLG_X = lCtrlSize
      SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lCtrlSize );
      //:lCtrlPos = vReportDef.Control.PSDLG_X
      GetIntegerFromAttribute( &lCtrlPos, vReportDef, "Control", "PSDLG_X" );
      //:TraceLineS("", "" )
      TraceLineS( "", "" );
      //:TraceLineI( "--->lCtrlPos : ", lCtrlPos )
      TraceLineI( "--->lCtrlPos : ", lCtrlPos );
      //:TraceLineI( "--->lCtrlSize : ", lCtrlSize )
      TraceLineI( "--->lCtrlSize : ", lCtrlSize );

      //:// adjust both pos and size for remaining controls
      //:lRC = SetCursorNextEntity( vReportDef, "Control", "" )
      lRC = SetCursorNextEntity( vReportDef, "Control", "" );
      //:LOOP WHILE lRC >= zCURSOR_SET
      while ( lRC >= zCURSOR_SET )
      { 

         //:// set the new pos
         //:vReportDef.Control.PSDLG_X = lCtrlPos + lCtrlSize
         lTempInteger_3 = lCtrlPos + lCtrlSize;
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lTempInteger_3 );

         //:// adjust the size and hold onto curr position and curr size
         //:// The conversion from Decimal to Integer will truncate so,
         //:// we add 0.5 to force rounding off
         //:lCtrlSize = (vReportDef.Control.SZDLG_X * dFactor) + 0.5
         GetIntegerFromAttribute( &lTempInteger_4, vReportDef, "Control", "SZDLG_X" );
         lCtrlSize = ( lTempInteger_4 * dFactor ) + 0.5;
         //:vReportDef.Control.SZDLG_X = lCtrlSize
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lCtrlSize );
         //:lCtrlPos = vReportDef.Control.PSDLG_X
         GetIntegerFromAttribute( &lCtrlPos, vReportDef, "Control", "PSDLG_X" );
         //:TraceLineS("", "" )
         TraceLineS( "", "" );
         //:TraceLineI( "--->lCtrlPos : ", lCtrlPos )
         TraceLineI( "--->lCtrlPos : ", lCtrlPos );
         //:TraceLineI( "--->lCtrlSize : ", lCtrlSize )
         TraceLineI( "--->lCtrlSize : ", lCtrlSize );

         //:// advance the loop
         //:lRC = SetCursorNextEntity( vReportDef, "Control", "" )
         lRC = SetCursorNextEntity( vReportDef, "Control", "" );
      } 

      //:END
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_AutodesignReportEntity( zVIEW     vSubtask,
                                  zVIEW     vReportDef,
                                  zVIEW     vUserSpec,
                                  zVIEW     vUserSpecRoot,
                                  zVIEW     vPE )
{
   zSHORT    RESULT; 

   //:AutodesignReportEntity( VIEW vSubtask,
   //:                     VIEW vReportDef    BASED ON LOD TZRPSRCO,
   //:                     VIEW vUserSpec     BASED ON LOD TZADCSDO,
   //:                     VIEW vUserSpecRoot BASED ON LOD TZADCSDO,
   //:                     VIEW vPE           BASED ON LOD TZPESRCO )

   //:// If autodesign was requested for this entity, go format the group control and
   //:// subcontrols for it.
   //:IF vUserSpec.UIS_Entity.AutodesignCaseType != "N"
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "AutodesignCaseType", "N" ) != 0 )
   { 
      //:CreateGrpSetForEntity( vSubtask, vReportDef, vUserSpec, vUserSpecRoot, vPE )
      oTZRPSRCO_CreateGrpSetForEntity( vSubtask, vReportDef, vUserSpec, vUserSpecRoot, vPE );
   } 

   //:END

   //:// Then process each of the subentities in the UserSpec.
   //:FOR EACH vUserSpec.UIS_ChildEntity
   RESULT = SetCursorFirstEntity( vUserSpec, "UIS_ChildEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( vUserSpec, "UIS_ChildEntity" )
      SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      //:AutodesignReportEntity( vSubtask, vReportDef, vUserSpec, vUserSpecRoot, vPE )
      oTZRPSRCO_AutodesignReportEntity( vSubtask, vReportDef, vUserSpec, vUserSpecRoot, vPE );
      //:ResetViewFromSubobject( vUserSpec )
      ResetViewFromSubobject( vUserSpec );
      RESULT = SetCursorNextEntity( vUserSpec, "UIS_ChildEntity", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:CreateGrpSetForEntity( VIEW vSubtask,
//:                       VIEW vReportDef    BASED ON LOD TZRPSRCO,
//:                       VIEW vUserSpec     BASED ON LOD TZADCSDO,
//:                       VIEW vUserSpecRoot BASED ON LOD TZADCSDO,
//:                       VIEW vPE_Control   BASED ON LOD TZPESRCO )

//:   VIEW       vDefaults BASED ON LOD TZADCSDO
static zSHORT
oTZRPSRCO_CreateGrpSetForEntity( zVIEW     vSubtask,
                                 zVIEW     vReportDef,
                                 zVIEW     vUserSpec,
                                 zVIEW     vUserSpecRoot,
                                 zVIEW     vPE_Control )
{
   zVIEW     vDefaults = 0; 
   //:STRING(32) szNameWork
   zCHAR     szNameWork[ 33 ] = { 0 }; 
   //:STRING(32) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:INTEGER    lVerticalPos
   zLONG     lVerticalPos = 0; 
   //:INTEGER    lHorizontalPos
   zLONG     lHorizontalPos = 0; 
   //:INTEGER    lLongestPrompt
   zLONG     lLongestPrompt = 0; 
   //:INTEGER    lTopGroupMargin
   zLONG     lTopGroupMargin = 0; 
   //:INTEGER    lBottomGroupMargin
   zLONG     lBottomGroupMargin = 0; 
   //:INTEGER    lBottomHeaderMargin
   zLONG     lBottomHeaderMargin = 0; 
   //:INTEGER    lLeftGroupMargin
   zLONG     lLeftGroupMargin = 0; 
   //:INTEGER    lRightGroupMargin
   zLONG     lRightGroupMargin = 0; 
   //:INTEGER    lVerticalIncrement     // space betwen lines
   zLONG     lVerticalIncrement = 0; 
   //:INTEGER    lHorizontalIncrement
   zLONG     lHorizontalIncrement = 0; 
   //:INTEGER    lAttributeCnt
   zLONG     lAttributeCnt = 0; 
   //:INTEGER    lLevelIndentation
   zLONG     lLevelIndentation = 0; 
   //:INTEGER    lGroupPosY
   zLONG     lGroupPosY = 0; 
   //:INTEGER    lGroupSizeY
   zLONG     lGroupSizeY = 0; 
   //:INTEGER    lTitleSize
   zLONG     lTitleSize = 0; 
   //:INTEGER    lDefaultFontSize
   zLONG     lDefaultFontSize = 0; 
   //:INTEGER    lPageWidth
   zLONG     lPageWidth = 0; 
   //:INTEGER    lFontSize
   zLONG     lFontSize = 0; 
   //:DECIMAL    dHorzScalingFactor
   ZDecimal  dHorzScalingFactor = 0.0; 
   //:DECIMAL    dVertScalingFactor
   ZDecimal  dVertScalingFactor = 0.0; 
   //:SHORT      nCharacterWidth
   zSHORT    nCharacterWidth = 0; 
   //:SHORT      nCharacterHeight
   zSHORT    nCharacterHeight = 0; 
   //:SHORT      nBoldFlag
   zSHORT    nBoldFlag = 0; 
   //:SHORT      nItalicFlag
   zSHORT    nItalicFlag = 0; 
   //:INTEGER    lAttributeSize
   zLONG     lAttributeSize = 0; 
   //:STRING(2)  szDomainDataType
   zCHAR     szDomainDataType[ 3 ] = { 0 }; 
   //:STRING(2)  szAttributePrefix
   zCHAR     szAttributePrefix[ 3 ] = { 0 }; 
   //:STRING(2)  szPositioningType
   zCHAR     szPositioningType[ 3 ] = { 0 }; 
   //:INTEGER    nControlEndPosition
   zLONG     nControlEndPosition = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zCHAR     szTempString_2[ 33 ]; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 
   zLONG     lTempInteger_8; 
   zLONG     lTempInteger_9; 
   zLONG     lTempInteger_10; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 255 ]; 
   zLONG     lTempInteger_11; 
   zLONG     lTempInteger_12; 
   zLONG     lTempInteger_13; 
   zLONG     lTempInteger_14; 
   zCHAR     szTempString_5[ 61 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zLONG     lTempInteger_15; 
   zLONG     lTempInteger_16; 
   zCHAR     szTempString_8[ 33 ]; 
   zLONG     lTempInteger_17; 
   zLONG     lTempInteger_18; 
   zLONG     lTempInteger_19; 
   zLONG     lTempInteger_20; 
   zLONG     lTempInteger_21; 
   zLONG     lTempInteger_22; 
   zLONG     lTempInteger_23; 
   zLONG     lTempInteger_24; 
   zLONG     lTempInteger_25; 
   zLONG     lTempInteger_26; 
   zSHORT    lTempInteger_27; 
   zLONG     lTempInteger_28; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 255 ]; 
   zLONG     lTempInteger_29; 
   zLONG     lTempInteger_30; 
   zCHAR     szTempString_11[ 33 ]; 
   zLONG     lTempInteger_31; 
   zLONG     lTempInteger_32; 
   zLONG     lTempInteger_33; 
   zLONG     lTempInteger_34; 


   //:// Set up default information.
   //:GET VIEW vDefaults NAMED "TZ_Defaults"
   RESULT = GetViewByName( &vDefaults, "TZ_Defaults", vSubtask, zLEVEL_TASK );
   //:dHorzScalingFactor    = vDefaults.InitValues.ReportTextIncrementX
   GetDecimalFromAttribute( &dHorzScalingFactor, vDefaults, "InitValues", "ReportTextIncrementX" );
   //:lHorizontalIncrement  = 0
   lHorizontalIncrement = 0;
   //:dVertScalingFactor    = vDefaults.InitValues.ReportTextIncrementY
   GetDecimalFromAttribute( &dVertScalingFactor, vDefaults, "InitValues", "ReportTextIncrementY" );
   //:lVerticalIncrement    = 0
   lVerticalIncrement = 0;
   //:lDefaultFontSize      = vDefaults.InitValues.ReportTextSize * 10
   GetIntegerFromAttribute( &lTempInteger_0, vDefaults, "InitValues", "ReportTextSize" );
   lDefaultFontSize = lTempInteger_0 * 10;
   //:lTopGroupMargin       = 1 * 16
   lTopGroupMargin = 1 * 16;
   //:lBottomGroupMargin    = 16  // DEM - guess work
   lBottomGroupMargin = 16;
   //:lBottomHeaderMargin   = 2 * 16
   lBottomHeaderMargin = 2 * 16;
   //:lRightGroupMargin     = 5 * 16
   lRightGroupMargin = 5 * 16;
   //:lLeftGroupMargin      = 5 * 16
   lLeftGroupMargin = 5 * 16;
   //:lPageWidth            = vReportDef.Page.SizeX
   GetIntegerFromAttribute( &lPageWidth, vReportDef, "Page", "SizeX" );
   //:lGroupPosY            = 4 * 16
   lGroupPosY = 4 * 16;
   //:lLevelIndentation     = vUserSpecRoot.UI_Spec.LevelIndentation
   GetIntegerFromAttribute( &lLevelIndentation, vUserSpecRoot, "UI_Spec", "LevelIndentation" );
   //:lLeftGroupMargin      = lLeftGroupMargin +
   //:                 (lLevelIndentation * vUserSpec.LOD_Entity.IndentLvl) * 16
   GetIntegerFromAttribute( &lTempInteger_1, vUserSpec, "LOD_Entity", "IndentLvl" );
   lLeftGroupMargin = lLeftGroupMargin + ( lLevelIndentation * lTempInteger_1 ) * 16;

   //:// Set EntityName for debug purposes, so we will know in tracing what entity we
   //:// are working with.
   //:szEntityName = vUserSpec.LOD_Entity.Name
   GetVariableFromAttribute( szEntityName, 0, 'S', 33, vUserSpec, "LOD_Entity", "Name", "", 0 );

   //:// The ControlDef is currently defaulted to Text for all subcontrols.
   //:SET CURSOR FIRST vPE_Control.ControlDef WHERE
   //:                 vPE_Control.ControlDef.Tag = "Rpt_Text"
   RESULT = SetCursorFirstEntityByString( vPE_Control, "ControlDef", "Tag", "Rpt_Text", "" );

   //:// If the positioning type is blank, force it to horizontal.
   //:szPositioningType    = vUserSpec.UIS_Entity.ReportOrientationFlag
   GetVariableFromAttribute( szPositioningType, 0, 'S', 3, vUserSpec, "UIS_Entity", "ReportOrientationFlag", "", 0 );
   //:IF szPositioningType = ""
   if ( ZeidonStringCompare( szPositioningType, 1, 0, "", 1, 0, 3 ) == 0 )
   { 
      //:szPositioningType = "V"
      ZeidonStringCopy( szPositioningType, 1, 0, "V", 1, 0, 3 );
   } 

   //:END


   //:// *** HEADERS  ***

   //:// Set up GroupSet
   //:CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER );
   //:vReportDef.GroupSet.Tag      = szEntityName
   SetAttributeFromString( vReportDef, "GroupSet", "Tag", szEntityName );
   //:vReportDef.GroupSet.Type     = "E"
   SetAttributeFromString( vReportDef, "GroupSet", "Type", "E" );
   //:vReportDef.GroupSet.PSDLG_X  = 0
   SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_X", 0 );
   //:vReportDef.GroupSet.PSDLG_Y  = 4 * 16
   lTempInteger_2 = 4 * 16;
   SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", lTempInteger_2 );
   //:vReportDef.GroupSet.SZDLG_X  = vReportDef.Page.SizeX
   SetAttributeFromAttribute( vReportDef, "GroupSet", "SZDLG_X", vReportDef, "Page", "SizeX" );
   //:vReportDef.GroupSet.PPE_Type = 3000
   SetAttributeFromInteger( vReportDef, "GroupSet", "PPE_Type", 3000 );

   //:// Set the FontSize, Bold and Italic flags for the Header group.
   //:IF vUserSpec.UIS_Entity.HeaderText != "" OR szPositioningType = "H"
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "HeaderText", "" ) != 0 || ZeidonStringCompare( szPositioningType, 1, 0, "H", 1, 0, 3 ) == 0 )
   { 
      //:lFontSize = vUserSpec.UIS_Entity.HeaderFontSize * 10
      GetIntegerFromAttribute( &lTempInteger_3, vUserSpec, "UIS_Entity", "HeaderFontSize" );
      lFontSize = lTempInteger_3 * 10;
      //:IF vUserSpec.UIS_Entity.HeaderBoldFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "HeaderBoldFlag", "Y" ) == 0 )
      { 
         //:nBoldFlag = TRUE
         nBoldFlag = TRUE;
         //:ELSE
      } 
      else
      { 
         //:nBoldFlag = FALSE
         nBoldFlag = FALSE;
      } 

      //:END
      //:IF vUserSpec.UIS_Entity.HeaderItalicFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "HeaderItalicFlag", "Y" ) == 0 )
      { 
         //:nItalicFlag = TRUE
         nItalicFlag = TRUE;
         //:ELSE
      } 
      else
      { 
         //:nItalicFlag = FALSE
         nItalicFlag = FALSE;
      } 

      //:END

      //:// Set font size and character width and height.
      //:IF lFontSize = 0
      if ( lFontSize == 0 )
      { 
         //:lFontSize = lDefaultFontSize
         lFontSize = lDefaultFontSize;
      } 

      //:END
      //:nCharacterWidth      = (lFontSize * 16) / 720
      nCharacterWidth = ( lFontSize * 16 ) / 720;
      //:nCharacterHeight     = ((lFontSize * 16) / 720) + 1
      nCharacterHeight = ( ( lFontSize * 16 ) / 720 ) + 1;
      //:lHorizontalIncrement = nCharacterWidth  * dHorzScalingFactor * 16
      lHorizontalIncrement = nCharacterWidth * dHorzScalingFactor * 16;
      //:lVerticalIncrement   = nCharacterHeight * dVertScalingFactor * 16
      lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16;
   } 

   //:END

   //:// If the attributes for the entity are to be placed horizontally, then
   //:// put the titles (prompts) in their own Header group.
   //:// If there is a Header Text for the entity, use it in either case.
   //:IF szPositioningType = "H"
   if ( ZeidonStringCompare( szPositioningType, 1, 0, "H", 1, 0, 3 ) == 0 )
   { 

      //:// Add Header Group
      //:CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
      //:vReportDef.Group.Tag                  = "GroupHeader"
      SetAttributeFromString( vReportDef, "Group", "Tag", "GroupHeader" );
      //:vReportDef.Group.Type                 = "gh"
      SetAttributeFromString( vReportDef, "Group", "Type", "gh" );
      //:vReportDef.Group.PSDLG_X              = 0
      SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
      //:vReportDef.Group.PSDLG_Y              = lGroupPosY
      SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
      //:vReportDef.Group.SZDLG_X              = vReportDef.Page.SizeX
      SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
      //:vReportDef.Group.PPE_Type             = 3010
      SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );
      //:vReportDef.Group.ForceHeaderPerEntity = vUserSpec.UIS_Entity.ForceHeaderPerEntity
      SetAttributeFromAttribute( vReportDef, "Group", "ForceHeaderPerEntity", vUserSpec, "UIS_Entity", "ForceHeaderPerEntity" );

      //:lVerticalPos = lTopGroupMargin
      lVerticalPos = lTopGroupMargin;

      //:// Add Header Text, if there is any.
      //:IF vUserSpec.UIS_Entity.HeaderText != ""
      if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "HeaderText", "" ) != 0 )
      { 
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:szNameWork = "H_" + vUserSpec.LOD_Entity.Name
         GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vUserSpec, "LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( szNameWork, 1, 0, "H_", 1, 0, 33 );
         ZeidonStringConcat( szNameWork, 1, 0, szTempString_0, 1, 0, 33 );
         //:vReportDef.Control.Type    = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:vReportDef.Control.Text    = vUserSpec.UIS_Entity.HeaderText
         SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "UIS_Entity", "HeaderText" );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lLeftGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lLeftGroupMargin );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lTitleSize                 = GetStringLength( vUserSpec.UIS_Entity.HeaderText )
         GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vUserSpec, "UIS_Entity", "HeaderText" );
         lTitleSize = GetStringLength( szTempString_1 );
         //:vReportDef.Control.SZDLG_X = lTitleSize * nCharacterWidth * 16
         lTempInteger_4 = lTitleSize * nCharacterWidth * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_4 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_5 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_5 );

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );

         //:lVerticalPos = lVerticalPos +
         //:                        (nCharacterHeight * 16) + lVerticalIncrement
         lVerticalPos = lVerticalPos + ( nCharacterHeight * 16 ) + lVerticalIncrement;
      } 

      //:END

      //:// Initialize our horizontal and vertical positions.  Actually,
      //:// the vertical position won't change.
      //:lHorizontalPos = lLeftGroupMargin
      lHorizontalPos = lLeftGroupMargin;

      //:// Start the attribute count with 1 for the generation of unique names.
      //:lAttributeCnt         = 1
      lAttributeCnt = 1;

      //:FOR EACH vUserSpec.M_LOD_Attribute
      RESULT = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:// For each attribute create the title. Control Type is 10 for title.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 )
         ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 );
         //:szNameWork                 = vUserSpec.M_ER_Attribute.Name
         GetVariableFromAttribute( szNameWork, 0, 'S', 33, vUserSpec, "M_ER_Attribute", "Name", "", 0 );
         //:     vReportDef.Control.Tag     = "T_" + szAttributePrefix + "_" + szNameWork[1:26]
         ZeidonStringCopy( szTempString_2, 1, 0, "T_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_2, 1, 0, szAttributePrefix, 1, 0, 33 );
         ZeidonStringConcat( szTempString_2, 1, 0, "_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_2, 1, 0, szNameWork, 1, 26, 33 );
         SetAttributeFromString( vReportDef, "Control", "Tag", szTempString_2 );
         //:vReportDef.Control.Type    = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:IF vUserSpec.M_ER_Attribute.ListPaintLit != ""
         if ( CompareAttributeToString( vUserSpec, "M_ER_Attribute", "ListPaintLit", "" ) != 0 )
         { 
            //:vReportDef.Control.Text = vUserSpec.M_ER_Attribute.ListPaintLit
            SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "M_ER_Attribute", "ListPaintLit" );
            //:ELSE
         } 
         else
         { 
            //:vReportDef.Control.Text = vUserSpec.M_ER_Attribute.Name
            SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "M_ER_Attribute", "Name" );
         } 

         //:END

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lTitleSize                 = GetMaxAttributeSize( vUserSpec, "H" )
         lTitleSize = oTZRPSRCO_GetMaxAttributeSize( vUserSpec, "H" );
         //:lTitleSize                 = lTitleSize * nCharacterWidth
         lTitleSize = lTitleSize * nCharacterWidth;
         //:vReportDef.Control.SZDLG_X = lTitleSize * 16
         lTempInteger_6 = lTitleSize * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_6 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_7 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_7 );

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );

         //:lHorizontalPos  = lHorizontalPos + (lTitleSize * 16) +
         //:                                               lHorizontalIncrement
         lHorizontalPos = lHorizontalPos + ( lTitleSize * 16 ) + lHorizontalIncrement;
         //:lAttributeCnt = lAttributeCnt + 1
         lAttributeCnt = lAttributeCnt + 1;
         RESULT = SetCursorNextEntity( vUserSpec, "M_LOD_Attribute", "" );
      } 


      //:END

      //:// Determine the size of the Group.
      //:// 1. We will determine the X size by getting the last control (right most in
      //://    Group) and adding its size and RightGroupMargin to it.
      //:// 2. We will determine the Y size by getting the last control (bottom in Group,
      //://    though any control would do) and adding BottomGroupMargin to it.
      //:SET CURSOR LAST vReportDef.Control
      RESULT = SetCursorLastEntity( vReportDef, "Control", "" );
      //:lGroupSizeY = vReportDef.Control.PSDLG_Y + vReportDef.Control.SZDLG_Y +
      //:              lVerticalIncrement + lBottomHeaderMargin
      GetIntegerFromAttribute( &lTempInteger_8, vReportDef, "Control", "PSDLG_Y" );
      GetIntegerFromAttribute( &lTempInteger_9, vReportDef, "Control", "SZDLG_Y" );
      lGroupSizeY = lTempInteger_8 + lTempInteger_9 + lVerticalIncrement + lBottomHeaderMargin;
      //:vReportDef.Group.SZDLG_Y    = lGroupSizeY
      SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lGroupSizeY );
      //:vReportDef.GroupSet.SZDLG_Y = lGroupSizeY
      SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lGroupSizeY );

      //:// update lGroupPosY
      //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
      GetIntegerFromAttribute( &lTempInteger_10, vReportDef, "Group", "SZDLG_Y" );
      lGroupPosY = lGroupPosY + lTempInteger_10;

      //:// We will now adjust the Group control and all the subcontrols by a common
      //:// factor so that they will all appear on the page.
      //:AdjustGroupSize( vReportDef, lPageWidth )
      oTZRPSRCO_AdjustGroupSize( vReportDef, lPageWidth );

      //:ELSE
   } 
   else
   { 
      //:// This is condition where the positioning of the group is not horizontal
      //:// and there are thus no attribute headers.  We do need to check, however,
      //:// if there is Header Text defined for the entity.
      //:IF vUserSpec.UIS_Entity.HeaderText != ""
      if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "HeaderText", "" ) != 0 )
      { 
         //:// Create the group control.
         //:// Add the Header Group
         //:CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
         //:vReportDef.Group.Tag      = "GroupHeader"
         SetAttributeFromString( vReportDef, "Group", "Tag", "GroupHeader" );
         //:vReportDef.Group.Type     = "gh"
         SetAttributeFromString( vReportDef, "Group", "Type", "gh" );
         //:vReportDef.Group.PSDLG_X  = 0
         SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
         //:vReportDef.Group.PSDLG_Y  = lGroupPosY
         SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
         //:vReportDef.Group.SZDLG_X  = vReportDef.Page.SizeX
         SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
         //:vReportDef.Group.PPE_Type = 3010
         SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );
         //:vReportDef.Group.ForceHeaderPerEntity = vUserSpec.UIS_Entity.ForceHeaderPerEntity
         SetAttributeFromAttribute( vReportDef, "Group", "ForceHeaderPerEntity", vUserSpec, "UIS_Entity", "ForceHeaderPerEntity" );

         //:lVerticalPos = lTopGroupMargin
         lVerticalPos = lTopGroupMargin;

         //:// Create the Text control for the header value.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:szNameWork = "H_" + vUserSpec.LOD_Entity.Name
         GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vUserSpec, "LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( szNameWork, 1, 0, "H_", 1, 0, 33 );
         ZeidonStringConcat( szNameWork, 1, 0, szTempString_3, 1, 0, 33 );
         //:vReportDef.Control.Type    = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:vReportDef.Control.Text    = vUserSpec.UIS_Entity.HeaderText
         SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "UIS_Entity", "HeaderText" );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lLeftGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lLeftGroupMargin );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lTitleSize = GetStringLength( vUserSpec.UIS_Entity.HeaderText )
         GetStringFromAttribute( szTempString_4, zsizeof( szTempString_4 ), vUserSpec, "UIS_Entity", "HeaderText" );
         lTitleSize = GetStringLength( szTempString_4 );
         //:vReportDef.Control.SZDLG_X = lTitleSize * nCharacterWidth * 16
         lTempInteger_11 = lTitleSize * nCharacterWidth * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_11 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_12 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_12 );

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );

         //:lHorizontalIncrement = nCharacterWidth  * dHorzScalingFactor
         lHorizontalIncrement = nCharacterWidth * dHorzScalingFactor;
         //:lVerticalIncrement = nCharacterHeight * dVertScalingFactor
         lVerticalIncrement = nCharacterHeight * dVertScalingFactor;
         //:lVerticalPos = lVerticalPos + nCharacterHeight + lVerticalIncrement
         lVerticalPos = lVerticalPos + nCharacterHeight + lVerticalIncrement;

         //:// Determine the size of the Group.
         //:// 1. We will determine the X size by getting the last control (right most in
         //://    Group) and adding its size and RightGroupMargin to it.
         //:// 2. We will determine the Y size by getting the last control (bottom in Group,
         //://    though any control would do) and adding BottomGroupMargin to it.
         //:SET CURSOR LAST vReportDef.CtrlCtrl
         RESULT = SetCursorLastEntity( vReportDef, "CtrlCtrl", "" );
         //:lGroupSizeY = lVerticalPos + lBottomGroupMargin
         lGroupSizeY = lVerticalPos + lBottomGroupMargin;
         //:vReportDef.Group.SZDLG_Y = lGroupSizeY
         SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lGroupSizeY );

         //:// update lGroupPosY
         //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
         GetIntegerFromAttribute( &lTempInteger_13, vReportDef, "Group", "SZDLG_Y" );
         lGroupPosY = lGroupPosY + lTempInteger_13;

         //:// We will now adjust the Group control and all the subcontrols by a common
         //:// factor so that they will all appear on the page.
         //:AdjustGroupSize( vReportDef, lPageWidth )
         oTZRPSRCO_AdjustGroupSize( vReportDef, lPageWidth );
      } 


      //:END
   } 


   //:END


   //:// *** REGULAR ATTRIBUTES  ***

   //:// Set the Bold and Italic flags for the main group.
   //:IF vUserSpec.UIS_Entity.BoldFlag = "Y"
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "BoldFlag", "Y" ) == 0 )
   { 
      //:nBoldFlag = TRUE
      nBoldFlag = TRUE;
      //:ELSE
   } 
   else
   { 
      //:nBoldFlag = FALSE
      nBoldFlag = FALSE;
   } 

   //:END
   //:IF vUserSpec.UIS_Entity.ItalicFlag = "Y"
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "ItalicFlag", "Y" ) == 0 )
   { 
      //:nItalicFlag = TRUE
      nItalicFlag = TRUE;
      //:ELSE
   } 
   else
   { 
      //:nItalicFlag = FALSE
      nItalicFlag = FALSE;
   } 

   //:END

   //:// Set font size and character width and height.
   //:lFontSize = vUserSpec.UIS_Entity.FontSize * 10
   GetIntegerFromAttribute( &lTempInteger_14, vUserSpec, "UIS_Entity", "FontSize" );
   lFontSize = lTempInteger_14 * 10;
   //:IF lFontSize = 0
   if ( lFontSize == 0 )
   { 
      //:lFontSize = lDefaultFontSize
      lFontSize = lDefaultFontSize;
   } 

   //:END
   //:nCharacterWidth  = (lFontSize * 16) / 720
   nCharacterWidth = ( lFontSize * 16 ) / 720;
   //:nCharacterHeight = ((lFontSize * 16) / 720) + 1
   nCharacterHeight = ( ( lFontSize * 16 ) / 720 ) + 1;
   //://   lVerticalIncrement = nCharacterHeight / 2
   //://   lVerticalIncrement = 0  // DEM -guess work
   //:lHorizontalIncrement = nCharacterWidth  * dHorzScalingFactor * 16
   lHorizontalIncrement = nCharacterWidth * dHorzScalingFactor * 16;
   //:lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16
   lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16;

   //:// If the attributes for the entity are to be placed vertically, loop
   //:// through all the attributes and determine the longest prompt.  We will use
   //:// values in the following order.
   //://    PromptPaintLth if specified
   //://    Length of PromptPaintLit if specified
   //://    Length of Attribute Name
   //:IF szPositioningType = "V"
   if ( ZeidonStringCompare( szPositioningType, 1, 0, "V", 1, 0, 3 ) == 0 )
   { 
      //:lLongestPrompt = 0
      lLongestPrompt = 0;
      //:FOR EACH vUserSpec.M_LOD_Attribute
      RESULT = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF vUserSpec.M_ER_Attribute.PromptPaintLit != ""
         if ( CompareAttributeToString( vUserSpec, "M_ER_Attribute", "PromptPaintLit", "" ) != 0 )
         { 
            //:lTitleSize = GetStringLength( vUserSpec.M_ER_Attribute.PromptPaintLit )
            GetStringFromAttribute( szTempString_5, zsizeof( szTempString_5 ), vUserSpec, "M_ER_Attribute", "PromptPaintLit" );
            lTitleSize = GetStringLength( szTempString_5 );
            //:ELSE
         } 
         else
         { 
            //:lTitleSize = GetStringLength( vUserSpec.M_ER_Attribute.Name )
            GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), vUserSpec, "M_ER_Attribute", "Name" );
            lTitleSize = GetStringLength( szTempString_6 );
         } 

         //:END
         //:IF lLongestPrompt < lTitleSize
         if ( lLongestPrompt < lTitleSize )
         { 
            //:lLongestPrompt = lTitleSize
            lLongestPrompt = lTitleSize;
         } 

         RESULT = SetCursorNextEntity( vUserSpec, "M_LOD_Attribute", "" );
         //:END
      } 

      //:END
   } 

   //:END
   //:lLongestPrompt = lLongestPrompt * nCharacterWidth
   lLongestPrompt = lLongestPrompt * nCharacterWidth;


   //:// Build the Group for the attribute Controls (and the titles, if
   //:// positioning is vertical).
   //:CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
   //:vReportDef.Group.Tag      = "Detail"
   SetAttributeFromString( vReportDef, "Group", "Tag", "Detail" );
   //:vReportDef.Group.Type     = "ga"
   SetAttributeFromString( vReportDef, "Group", "Type", "ga" );
   //:vReportDef.Group.PSDLG_X  = 0
   SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
   //:vReportDef.Group.PSDLG_Y  = lGroupPosY
   SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
   //:vReportDef.Group.SZDLG_X  = vReportDef.Page.SizeX
   SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
   //:vReportDef.Group.PPE_Type = 3010
   SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );

   //:/*???
   //:// Indicate the horizontal or vertical placement using Type = 10 for
   //:// vertical and 20 for horizontal.
   //:IF szPositioningType = "H"
   //:   vReportDef.Control.Type = 20       // Horizontal positioning
   //:ELSE
   //:   vReportDef.Control.Type = 10       // Vertical positioning
   //:END
   //:???*/

   //:// Start the attribute count with 1 for the generation of unique names.
   //:lAttributeCnt = 1
   lAttributeCnt = 1;

   //:// Initialize positions.
   //:lVerticalPos     = lTopGroupMargin
   lVerticalPos = lTopGroupMargin;
   //:lHorizontalPos   = lLeftGroupMargin
   lHorizontalPos = lLeftGroupMargin;

   //:// Loop through all the attributes creating both Title controls and Attribute controls.
   //:// We use the same code for both horizontal position and vertical positioning, the
   //:// only difference being the positioning of both types of controls.
   //:FOR EACH vUserSpec.M_LOD_Attribute
   RESULT = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// The ControlDef is always Text at this time.
      //:SET CURSOR FIRST vPE_Control.ControlDef WHERE
      //:                 vPE_Control.ControlDef.Tag = "Rpt_Text"
      RESULT = SetCursorFirstEntityByString( vPE_Control, "ControlDef", "Tag", "Rpt_Text", "" );

      //:// Set up szNameWork and szAttributePrefix for use in creating Tags.
      //:szNameWork = vUserSpec.M_ER_Attribute.Name
      GetVariableFromAttribute( szNameWork, 0, 'S', 33, vUserSpec, "M_ER_Attribute", "Name", "", 0 );
      //:ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 )
      ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 );

      //:// If positioning is vertical, add the title.  If positioning is horizontal,
      //:// the titles were added as their own group.  Control Type is 10 for title.
      //:IF szPositioningType = "V"
      if ( ZeidonStringCompare( szPositioningType, 1, 0, "V", 1, 0, 3 ) == 0 )
      { 
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:vReportDef.Control.Tag = "T_" + szAttributePrefix + "_" + szNameWork[1:26]
         ZeidonStringCopy( szTempString_7, 1, 0, "T_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_7, 1, 0, szAttributePrefix, 1, 0, 33 );
         ZeidonStringConcat( szTempString_7, 1, 0, "_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_7, 1, 0, szNameWork, 1, 26, 33 );
         SetAttributeFromString( vReportDef, "Control", "Tag", szTempString_7 );
         //:vReportDef.Control.Type = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:IF vUserSpec.M_ER_Attribute.PromptPaintLit != ""
         if ( CompareAttributeToString( vUserSpec, "M_ER_Attribute", "PromptPaintLit", "" ) != 0 )
         { 
            //:vReportDef.Control.Text = vUserSpec.M_ER_Attribute.PromptPaintLit
            SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "M_ER_Attribute", "PromptPaintLit" );
            //:ELSE
         } 
         else
         { 
            //:vReportDef.Control.Text = vUserSpec.M_ER_Attribute.Name
            SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "M_ER_Attribute", "Name" );
         } 

         //:END

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:vReportDef.Control.SZDLG_X = lLongestPrompt * 16
         lTempInteger_15 = lLongestPrompt * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_15 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_16 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_16 );

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );
      } 


      //:END

      //:// For each attribute create the control. Control Type is 20 for control.
      //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
      //:vReportDef.Control.Tag  = "A_" + szAttributePrefix + "_" + szNameWork[1:26]
      ZeidonStringCopy( szTempString_8, 1, 0, "A_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_8, 1, 0, szAttributePrefix, 1, 0, 33 );
      ZeidonStringConcat( szTempString_8, 1, 0, "_", 1, 0, 33 );
      ZeidonStringConcat( szTempString_8, 1, 0, szNameWork, 1, 26, 33 );
      SetAttributeFromString( vReportDef, "Control", "Tag", szTempString_8 );
      //:vReportDef.Control.Type = 20
      SetAttributeFromInteger( vReportDef, "Control", "Type", 20 );

      //:// Right justify decimals, shorts and longs.  Subtype = 16777216 sets the
      //:// bit zCONTROL_RIGHTJUSTIFY.
      //:szDomainDataType = vUserSpec.M_Domain.DataType
      GetVariableFromAttribute( szDomainDataType, 0, 'S', 3, vUserSpec, "M_Domain", "DataType", "", 0 );
      //:IF szDomainDataType = "M" OR
      //:   szDomainDataType = "N" OR
      //:   szDomainDataType = "L"
      if ( ZeidonStringCompare( szDomainDataType, 1, 0, "M", 1, 0, 3 ) == 0 || ZeidonStringCompare( szDomainDataType, 1, 0, "N", 1, 0, 3 ) == 0 || ZeidonStringCompare( szDomainDataType, 1, 0, "L", 1, 0, 3 ) == 0 )
      { 
         //:   vReportDef.Control.Subtype = 16777216
         SetAttributeFromInteger( vReportDef, "Control", "Subtype", 16777216 );
      } 

      //:END

      //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
      RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );

      //:CreateMetaEntity( vSubtask, vReportDef, "CtrlMap", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vReportDef, "CtrlMap", zPOS_AFTER );
      //:INCLUDE vReportDef.CtrlMapLOD_Attribute FROM vUserSpec.M_LOD_Attribute
      RESULT = IncludeSubobjectFromSubobject( vReportDef, "CtrlMapLOD_Attribute", vUserSpec, "M_LOD_Attribute", zPOS_AFTER );
      //:INCLUDE vReportDef.CtrlMapView FROM vUserSpecRoot.UIS_ViewObjRef
      RESULT = IncludeSubobjectFromSubobject( vReportDef, "CtrlMapView", vUserSpecRoot, "UIS_ViewObjRef", zPOS_AFTER );

      //:// Now determine the control position and increment the positions, based on
      //:// horizontal or vertical positioning.
      //:IF szPositioningType = "H"
      if ( ZeidonStringCompare( szPositioningType, 1, 0, "H", 1, 0, 3 ) == 0 )
      { 
         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lAttributeSize             = GetMaxAttributeSize( vUserSpec, "H" )
         lAttributeSize = oTZRPSRCO_GetMaxAttributeSize( vUserSpec, "H" );
         //:lAttributeSize             = lAttributeSize * nCharacterWidth
         lAttributeSize = lAttributeSize * nCharacterWidth;
         //:vReportDef.Control.SZDLG_X = lAttributeSize * 16
         lTempInteger_17 = lAttributeSize * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_17 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_18 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_18 );

         //:lHorizontalPos = lHorizontalPos + (lAttributeSize * 16) +
         //:                                                lHorizontalIncrement
         lHorizontalPos = lHorizontalPos + ( lAttributeSize * 16 ) + lHorizontalIncrement;
         //:ELSE
      } 
      else
      { 
         //:vReportDef.Control.PSDLG_X =
         //:      lHorizontalPos + (lLongestPrompt * 16) + lHorizontalIncrement
         lTempInteger_19 = lHorizontalPos + ( lLongestPrompt * 16 ) + lHorizontalIncrement;
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lTempInteger_19 );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lAttributeSize             = GetMaxAttributeSize( vUserSpec, "V" )
         lAttributeSize = oTZRPSRCO_GetMaxAttributeSize( vUserSpec, "V" );
         //:lAttributeSize             = lAttributeSize * nCharacterWidth * 16
         lAttributeSize = lAttributeSize * nCharacterWidth * 16;
         //:// trim the attribute size,if needed, to fit onto page
         //:nControlEndPosition = lAttributeSize + vReportDef.Control.PSDLG_X
         GetIntegerFromAttribute( &lTempInteger_20, vReportDef, "Control", "PSDLG_X" );
         nControlEndPosition = lAttributeSize + lTempInteger_20;
         //:IF nControlEndPosition > vReportDef.Page.SizeX
         if ( CompareAttributeToInteger( vReportDef, "Page", "SizeX", nControlEndPosition ) < 0 )
         { 
            //:lAttributeSize = vReportDef.Page.SizeX -
            //:                                       vReportDef.Control.PSDLG_X
            GetIntegerFromAttribute( &lTempInteger_21, vReportDef, "Page", "SizeX" );
            GetIntegerFromAttribute( &lTempInteger_22, vReportDef, "Control", "PSDLG_X" );
            lAttributeSize = lTempInteger_21 - lTempInteger_22;
         } 

         //:END
         //:vReportDef.Control.SZDLG_X = lAttributeSize
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lAttributeSize );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_23 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_23 );

         //:// update vertical position
         //:lVerticalPos = lVerticalPos + lVerticalIncrement +
         //:                              vReportDef.Control.SZDLG_Y
         GetIntegerFromAttribute( &lTempInteger_24, vReportDef, "Control", "SZDLG_Y" );
         lVerticalPos = lVerticalPos + lVerticalIncrement + lTempInteger_24;
      } 

      //:END

      //:// Set Bold and Italic flags and Fontsize.
      //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
      //:                      FALSE, FALSE, 0, lFontSize,
      //:                      0, 0, 0, 0, 0, 0, 0 )
      TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

      //:lAttributeCnt = lAttributeCnt + 1
      lAttributeCnt = lAttributeCnt + 1;
      RESULT = SetCursorNextEntity( vUserSpec, "M_LOD_Attribute", "" );
   } 


   //:END

   //:IF szPositioningType = "H"
   if ( ZeidonStringCompare( szPositioningType, 1, 0, "H", 1, 0, 3 ) == 0 )
   { 
      //:// update vertical position
      //:lVerticalPos = lVerticalPos + lVerticalIncrement +
      //:                              vReportDef.Control.SZDLG_Y
      GetIntegerFromAttribute( &lTempInteger_25, vReportDef, "Control", "SZDLG_Y" );
      lVerticalPos = lVerticalPos + lVerticalIncrement + lTempInteger_25;
   } 

   //:END

   //:// Determine the size of the Group.
   //:// 1. We will determine the X size by getting the largest end of any control
   //://    in the Group (ControlSize + ControlPosX) and adding RightGroupMargin to it.
   //:// 2. We will determine the Y size by getting the last control (bottom in Group)
   //://    and adding BottomGroupMargin to it.

   //:// If positioning is vertical add a bit of space to open up between
   //:// instances.
   //:IF szPositioningType = "H"
   if ( ZeidonStringCompare( szPositioningType, 1, 0, "H", 1, 0, 3 ) == 0 )
   { 
      //:lGroupSizeY = lVerticalPos + lVerticalIncrement + lBottomGroupMargin
      lGroupSizeY = lVerticalPos + lVerticalIncrement + lBottomGroupMargin;
      //:ELSE
   } 
   else
   { 
      //:lGroupSizeY = lVerticalPos + lVerticalIncrement + lBottomGroupMargin * 2
      lGroupSizeY = lVerticalPos + lVerticalIncrement + lBottomGroupMargin * 2;
   } 

   //:END

   //:vReportDef.Group.SZDLG_Y = lGroupSizeY
   SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lGroupSizeY );

   //:// update lGroupPosY
   //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
   GetIntegerFromAttribute( &lTempInteger_26, vReportDef, "Group", "SZDLG_Y" );
   lGroupPosY = lGroupPosY + lTempInteger_26;

   //:// We will now adjust the Group control and all the subcontrols by a common
   //:// factor so that they will all appear on the page.
   //:AdjustGroupSize( vReportDef, lPageWidth )
   oTZRPSRCO_AdjustGroupSize( vReportDef, lPageWidth );


   //:// *** FOOTERS  ***

   //:// If a Footer is requested, loop through all the Footer attributes creating
   //:// Attribute controls.  The Footer will be created if there are Footer
   //:// attributes or a Footer Text.
   //:IF vUserSpec.UIS_Entity.FooterText != "" OR
   //:   vUserSpec.L_LOD_Attribute EXISTS
   lTempInteger_27 = CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" );
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "FooterText", "" ) != 0 || lTempInteger_27 == 0 )
   { 

      //:// Set the FontSize, Bold and Italic flags for the Footer group.
      //:lFontSize = vUserSpec.UIS_Entity.FooterFontSize * 10
      GetIntegerFromAttribute( &lTempInteger_28, vUserSpec, "UIS_Entity", "FooterFontSize" );
      lFontSize = lTempInteger_28 * 10;
      //:IF vUserSpec.UIS_Entity.FooterBoldFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "FooterBoldFlag", "Y" ) == 0 )
      { 
         //:nBoldFlag = TRUE
         nBoldFlag = TRUE;
         //:ELSE
      } 
      else
      { 
         //:nBoldFlag = FALSE
         nBoldFlag = FALSE;
      } 

      //:END
      //:IF vUserSpec.UIS_Entity.FooterItalicFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "FooterItalicFlag", "Y" ) == 0 )
      { 
         //:nItalicFlag = TRUE
         nItalicFlag = TRUE;
         //:ELSE
      } 
      else
      { 
         //:nItalicFlag = FALSE
         nItalicFlag = FALSE;
      } 

      //:END

      //:// Set font size and character width and height.
      //:IF lFontSize = 0
      if ( lFontSize == 0 )
      { 
         //:lFontSize = lDefaultFontSize
         lFontSize = lDefaultFontSize;
      } 

      //:END
      //:nCharacterWidth  = (lFontSize * 16) / 720
      nCharacterWidth = ( lFontSize * 16 ) / 720;
      //:nCharacterHeight = ((lFontSize * 16) / 720) + 1
      nCharacterHeight = ( ( lFontSize * 16 ) / 720 ) + 1;

      //:// Set up horizontal position
      //:lHorizontalPos = lLeftGroupMargin
      lHorizontalPos = lLeftGroupMargin;

      //:// Add the Footer Group
      //:CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
      //:vReportDef.Group.Tag      = "GroupFooter"
      SetAttributeFromString( vReportDef, "Group", "Tag", "GroupFooter" );
      //:vReportDef.Group.Type     = "gf"
      SetAttributeFromString( vReportDef, "Group", "Type", "gf" );
      //:vReportDef.Group.PSDLG_X  = 0
      SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", 0 );
      //:vReportDef.Group.PSDLG_Y  = lGroupPosY
      SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lGroupPosY );
      //:vReportDef.Group.SZDLG_X  = vReportDef.Page.SizeX
      SetAttributeFromAttribute( vReportDef, "Group", "SZDLG_X", vReportDef, "Page", "SizeX" );
      //:vReportDef.Group.PPE_Type = 3010
      SetAttributeFromInteger( vReportDef, "Group", "PPE_Type", 3010 );

      //:lVerticalPos = lTopGroupMargin * 3
      lVerticalPos = lTopGroupMargin * 3;

      //:// Add the Footer Text if it exists.
      //:IF vUserSpec.UIS_Entity.FooterText != ""
      if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "FooterText", "" ) != 0 )
      { 
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:szNameWork = "F_" + vUserSpec.LOD_Entity.Name
         GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vUserSpec, "LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( szNameWork, 1, 0, "F_", 1, 0, 33 );
         ZeidonStringConcat( szNameWork, 1, 0, szTempString_9, 1, 0, 33 );
         //:vReportDef.Control.Type    = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:vReportDef.Control.Text    = vUserSpec.UIS_Entity.FooterText
         SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "UIS_Entity", "FooterText" );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lTitleSize = GetStringLength( vUserSpec.UIS_Entity.FooterText )
         GetStringFromAttribute( szTempString_10, zsizeof( szTempString_10 ), vUserSpec, "UIS_Entity", "FooterText" );
         lTitleSize = GetStringLength( szTempString_10 );
         //:lTitleSize = nCharacterWidth * lTitleSize
         lTitleSize = nCharacterWidth * lTitleSize;
         //:vReportDef.Control.SZDLG_X = lTitleSize * 16
         lTempInteger_29 = lTitleSize * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_29 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_30 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_30 );

         //:lHorizontalPos = lHorizontalPos + lTitleSize + lHorizontalIncrement
         lHorizontalPos = lHorizontalPos + lTitleSize + lHorizontalIncrement;

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );
      } 

      //:END
      //:FOR EACH vUserSpec.L_LOD_Attribute
      RESULT = SetCursorFirstEntity( vUserSpec, "L_LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:// Set up szNameWork and szAttributePrefix for use in creating Tags.
         //:szNameWork = vUserSpec.M_ER_Attribute.Name
         GetVariableFromAttribute( szNameWork, 0, 'S', 33, vUserSpec, "M_ER_Attribute", "Name", "", 0 );
         //:ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 )
         ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 );

         //:// For each attribute create the control. Control Type is 20 for control.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
         //:vReportDef.Control.Tag  = "A_" + szAttributePrefix + "_" + szNameWork[1:26]
         ZeidonStringCopy( szTempString_11, 1, 0, "A_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_11, 1, 0, szAttributePrefix, 1, 0, 33 );
         ZeidonStringConcat( szTempString_11, 1, 0, "_", 1, 0, 33 );
         ZeidonStringConcat( szTempString_11, 1, 0, szNameWork, 1, 26, 33 );
         SetAttributeFromString( vReportDef, "Control", "Tag", szTempString_11 );
         //:vReportDef.Control.Type = 20
         SetAttributeFromInteger( vReportDef, "Control", "Type", 20 );

         //:// Right justify decimals, shorts and longs.  Subtype = 16777216 sets the
         //:// bit zCONTROL_RIGHTJUSTIFY.
         //:szDomainDataType = vUserSpec.L_Domain.DataType
         GetVariableFromAttribute( szDomainDataType, 0, 'S', 3, vUserSpec, "L_Domain", "DataType", "", 0 );
         //:IF szDomainDataType = "M" OR
         //:   szDomainDataType = "N" OR
         //:   szDomainDataType = "L"
         if ( ZeidonStringCompare( szDomainDataType, 1, 0, "M", 1, 0, 3 ) == 0 || ZeidonStringCompare( szDomainDataType, 1, 0, "N", 1, 0, 3 ) == 0 || ZeidonStringCompare( szDomainDataType, 1, 0, "L", 1, 0, 3 ) == 0 )
         { 

            //:   vReportDef.Control.Subtype = 16777216
            SetAttributeFromInteger( vReportDef, "Control", "Subtype", 16777216 );
         } 


         //:END

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );

         //:CreateMetaEntity( vSubtask, vReportDef, "CtrlMap", zPOS_AFTER )
         CreateMetaEntity( vSubtask, vReportDef, "CtrlMap", zPOS_AFTER );
         //:INCLUDE vReportDef.CtrlMapLOD_Attribute FROM vUserSpec.L_LOD_Attribute
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "CtrlMapLOD_Attribute", vUserSpec, "L_LOD_Attribute", zPOS_AFTER );
         //:INCLUDE vReportDef.CtrlMapView FROM vUserSpecRoot.UIS_ViewObjRef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "CtrlMapView", vUserSpecRoot, "UIS_ViewObjRef", zPOS_AFTER );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:// The control position is always horizontal for Footer attributes.
         //:// Right now it doesn't look possible to position them correctly, so we
         //:// will just use the same algorithm as for regular horizontal attributes.
         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lAttributeSize             = GetMaxAttributeSize( vUserSpec, "H" )
         lAttributeSize = oTZRPSRCO_GetMaxAttributeSize( vUserSpec, "H" );
         //:lAttributeSize = lAttributeSize * nCharacterWidth * 16
         lAttributeSize = lAttributeSize * nCharacterWidth * 16;
         //:vReportDef.Control.SZDLG_X = lAttributeSize
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lAttributeSize );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_31 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_31 );

         //:lHorizontalPos = lHorizontalPos + lAttributeSize + lHorizontalIncrement
         lHorizontalPos = lHorizontalPos + lAttributeSize + lHorizontalIncrement;

         //:lAttributeCnt = lAttributeCnt + 1
         lAttributeCnt = lAttributeCnt + 1;
         RESULT = SetCursorNextEntity( vUserSpec, "L_LOD_Attribute", "" );
      } 

      //:END

      //:// update lVerticalPos to point to bottom of controls
      //:lVerticalPos = lVerticalPos + vReportDef.Control.SZDLG_Y
      GetIntegerFromAttribute( &lTempInteger_32, vReportDef, "Control", "SZDLG_Y" );
      lVerticalPos = lVerticalPos + lTempInteger_32;

      //:// Determine the size of the Group.
      //:// 1. We will determine the X size by getting the last control (right most in
      //://    Group) and adding its size and RightGroupMargin to it.
      //:// 2. We will determine the Y size by getting the last control (bottom in Group,
      //://    though any control would do) and adding BottomGroupMargin to it.
      //:SET CURSOR LAST vReportDef.Control
      RESULT = SetCursorLastEntity( vReportDef, "Control", "" );
      //:lGroupSizeY = lVerticalPos + lVerticalIncrement + lBottomGroupMargin
      lGroupSizeY = lVerticalPos + lVerticalIncrement + lBottomGroupMargin;
      //:vReportDef.Group.SZDLG_Y = lGroupSizeY
      SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lGroupSizeY );

      //:// update lGroupPosY
      //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
      GetIntegerFromAttribute( &lTempInteger_33, vReportDef, "Group", "SZDLG_Y" );
      lGroupPosY = lGroupPosY + lTempInteger_33;

      //:// We will now adjust the Group control and all the subcontrols by a common
      //:// factor so that they will all appear on the page.
      //:AdjustGroupSize( vReportDef, lPageWidth )
      oTZRPSRCO_AdjustGroupSize( vReportDef, lPageWidth );
   } 


   //:END

   //:// Compute and set GroupSet Y size
   //:lGroupSizeY = 0
   lGroupSizeY = 0;
   //:FOR EACH vReportDef.Group
   RESULT = SetCursorFirstEntity( vReportDef, "Group", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:lGroupSizeY = lGroupSizeY + vReportDef.Group.SZDLG_Y
      GetIntegerFromAttribute( &lTempInteger_34, vReportDef, "Group", "SZDLG_Y" );
      lGroupSizeY = lGroupSizeY + lTempInteger_34;
      RESULT = SetCursorNextEntity( vReportDef, "Group", "" );
   } 

   //:END
   //:vReportDef.GroupSet.SZDLG_Y = lGroupSizeY
   SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lGroupSizeY );

   //:// We will now adjust the Group control and all the subcontrols by a common
   //:// factor so that they will all appear on the page.
   //:AdjustGroupSize( vReportDef, lPageWidth )
   oTZRPSRCO_AdjustGroupSize( vReportDef, lPageWidth );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:GetMaxAttributeSize( VIEW      vUserSpec BASED ON LOD TZADCSDO,
//:                     STRING(1) szPositionType )

//:   INTEGER lTitleSize
static zLONG
oTZRPSRCO_GetMaxAttributeSize( zVIEW     vUserSpec,
                               zPCHAR    szPositionType )
{
   zLONG     lTitleSize = 0; 
   //:INTEGER lAttributeSize
   zLONG     lAttributeSize = 0; 
   //:INTEGER lReturnSize
   zLONG     lReturnSize = 0; 
   //:INTEGER lRC
   zLONG     lRC = 0; 
   //:INTEGER lCharWidth
   zLONG     lCharWidth = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 61 ]; 


   //:// Initially, we will default character width to a constant.
   //:lCharWidth = 1
   lCharWidth = 1;

   //:// If the positioning is horizontal, we will consider the Prompt length in
   //:// determining the attribute size.  If the positioning is vertical, we will
   //:// not consider the prompt length.
   //:IF szPositionType = "H"
   if ( ZeidonStringCompare( szPositionType, 1, 0, "H", 1, 0, 2 ) == 0 )
   { 
      //:IF vUserSpec.M_ER_Attribute.ListPaintLth != 0
      if ( CompareAttributeToInteger( vUserSpec, "M_ER_Attribute", "ListPaintLth", 0 ) != 0 )
      { 
         //:// If a list length was specified in data model, use that length and forget
         //:// about the rest.
         //:lRC = vUserSpec.M_ER_Attribute.ListPaintLth
         GetIntegerFromAttribute( &lRC, vUserSpec, "M_ER_Attribute", "ListPaintLth" );
         //:RETURN lRC
         return( lRC );
         //:ELSE
      } 
      else
      { 
         //:IF vUserSpec.M_ER_Attribute.ListPaintLit = ""
         if ( CompareAttributeToString( vUserSpec, "M_ER_Attribute", "ListPaintLit", "" ) == 0 )
         { 
            //:lTitleSize = GetStringLength( vUserSpec.M_ER_Attribute.Name )
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vUserSpec, "M_ER_Attribute", "Name" );
            lTitleSize = GetStringLength( szTempString_0 );
            //:ELSE
         } 
         else
         { 
            //:lTitleSize = GetStringLength( vUserSpec.M_ER_Attribute.ListPaintLit )
            GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vUserSpec, "M_ER_Attribute", "ListPaintLit" );
            lTitleSize = GetStringLength( szTempString_1 );
         } 

         //:END
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:IF vUserSpec.M_ER_Attribute.PromptPaintLth  != 0
      if ( CompareAttributeToInteger( vUserSpec, "M_ER_Attribute", "PromptPaintLth", 0 ) != 0 )
      { 
         //:// If a list length was specified in data model, use that length and forget
         //:// about the rest.
         //:lRC = vUserSpec.M_ER_Attribute.PromptPaintLth
         GetIntegerFromAttribute( &lRC, vUserSpec, "M_ER_Attribute", "PromptPaintLth" );
         //:RETURN lRC
         return( lRC );
         //:ELSE
      } 
      else
      { 
         //:lTitleSize = 0
         lTitleSize = 0;
      } 

      //:END
   } 

   //:END

   //:IF vUserSpec.M_Domain.DataType = "M"
   if ( CompareAttributeToString( vUserSpec, "M_Domain", "DataType", "M" ) == 0 )
   { 
      //:lAttributeSize = 10      // Use size of 10 for default decimal
      lAttributeSize = 10;
      //:ELSE
   } 
   else
   { 
      //:IF vUserSpec.M_Domain.DataType = "S"
      if ( CompareAttributeToString( vUserSpec, "M_Domain", "DataType", "S" ) == 0 )
      { 
         //:IF vUserSpec.M_ER_Attribute.Lth = 0
         if ( CompareAttributeToInteger( vUserSpec, "M_ER_Attribute", "Lth", 0 ) == 0 )
         { 
            //:lAttributeSize = vUserSpec.M_Domain.MaxStringLth
            GetIntegerFromAttribute( &lAttributeSize, vUserSpec, "M_Domain", "MaxStringLth" );
            //:ELSE
         } 
         else
         { 
            //:lAttributeSize = vUserSpec.M_ER_Attribute.Lth
            GetIntegerFromAttribute( &lAttributeSize, vUserSpec, "M_ER_Attribute", "Lth" );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:IF vUserSpec.M_Domain.DataType = "T"
         if ( CompareAttributeToString( vUserSpec, "M_Domain", "DataType", "T" ) == 0 )
         { 
            //:lAttributeSize = 10   // Use size of 10 for date
            lAttributeSize = 10;
            //:ELSE
         } 
         else
         { 
            //:lAttributeSize = 6    // Use size of 6 for default Integer
            lAttributeSize = 6;
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:IF lAttributeSize > lTitleSize
   if ( lAttributeSize > lTitleSize )
   { 
      //:lReturnSize = lAttributeSize
      lReturnSize = lAttributeSize;
      //:ELSE
   } 
   else
   { 
      //:lReturnSize = lTitleSize
      lReturnSize = lTitleSize;
   } 

   //:END

   //:// The max size returned will be 20 when the length is computed.  Note in
   //:// the code above that if the PaintLth is used, we don't set a max.
   //:IF lReturnSize > 20
   if ( lReturnSize > 20 )
   { 
      //:lReturnSize = 20
      lReturnSize = 20;
   } 

   //:END

   //:// Finally alter the ReturnSize, which is currently in number of characters,
   //:// into real report character size
   //://   lReturnSize = lReturnSize * lCharWidth

   //:RETURN lReturnSize
   return( lReturnSize );
// END
} 


 
#ifdef __cplusplus
}
#endif
