/**
    This file is part of the Zeidon Java Object Engine (Zeidon JOE).

    Zeidon JOE is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Zeidon JOE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Zeidon JOE.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2009-2010 QuinSoft
**/

package ;

import com.quinsoft.zeidon.ActivateFlags;
import com.quinsoft.zeidon.CursorPosition;
import com.quinsoft.zeidon.TaskQualification;
import com.quinsoft.zeidon.vml.VmlObjectOperations;
import com.quinsoft.zeidon.View;
import com.quinsoft.zeidon.ZeidonException;
import com.quinsoft.zeidon.vml.zVIEW;
import org.apache.commons.lang3.mutable.MutableDouble;
import org.apache.commons.lang3.mutable.MutableInt;

import .TZCM_OPR_Operation;
import .TZ__OPRS_Operation;
import .TZWDLGSO_Object;
import .TZWDLG2O_Object;

import com.quinsoft.zeidon.zeidonoperations.KZOEP1AA;
import com.quinsoft.zeidon.zeidonoperations.ZDRVROPR;

/**
   @author QuinSoft
**/

public class TZRPSRCO_Object extends VmlObjectOperations
{
   private final KZOEP1AA m_KZOEP1AA;
   private final ZDRVROPR m_ZDRVROPR;
   public TZRPSRCO_Object( View view )
   {
      super( view );
      m_KZOEP1AA = new KZOEP1AA( view );
      m_ZDRVROPR = new ZDRVROPR( view );
   }


//:TRANSFORMATION OPERATION
//:AutodesignReport( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                  VIEW vUserSpec  BASED ON LOD TZADCSDO,
//:                  VIEW vPE        BASED ON LOD TZPESRCO,
//:                  VIEW vSubtask )

//:   VIEW        vUserSpecRoot  BASED ON LOD TZADCSDO
public int 
oTZRPSRCO_AutodesignReport( View     vReportDef,
                            View     vUserSpec,
                            View     vPE,
                            View     vSubtask )
{
   zVIEW    vUserSpecRoot = new zVIEW( );
   //:VIEW        vDefaults BASED ON LOD TZADCSDO
   zVIEW    vDefaults = new zVIEW( );
   //:VIEW        vTaskLPLR BASED ON LOD TZCMLPLO
   zVIEW    vTaskLPLR = new zVIEW( );
   //:STRING ( 513 ) szFileName                        // zMAX_FILESPEC_LTH+1
   String   szFileName = null;
   //:STRING ( 257 ) szMsg                             // zSHORT_MESSAGE_LTH+1
   String   szMsg = null;
   //:INTEGER     lFile
   int      lFile = 0;
   //:INTEGER     lCurrentVerticalPos
   int      lCurrentVerticalPos = 0;
   //:INTEGER     lTopGroupMargin
   int      lTopGroupMargin = 0;
   //:INTEGER     lBottomHeaderMargin
   int      lBottomHeaderMargin = 0;
   //:INTEGER     lLeftGroupMargin
   int      lLeftGroupMargin = 0;
   //:INTEGER     lHeight
   int      lHeight = 0;
   //:INTEGER     lCurrentPosY
   int      lCurrentPosY = 0;
   //:INTEGER     lPageNumberPosX
   int      lPageNumberPosX = 0;
   //:INTEGER     lDefaultFontSize
   int      lDefaultFontSize = 0;
   //:INTEGER     lTitleSize
   int      lTitleSize = 0;
   //:INTEGER     lFontSize
   int      lFontSize = 0;
   //:INTEGER     lPageWidth
   int      lPageWidth = 0;
   //:INTEGER     lVerticalIncrement
   int      lVerticalIncrement = 0;
   //:DECIMAL     dVertScalingFactor
   double  dVertScalingFactor = 0.0;
   //:SHORT       nCharacterHeight
   int      nCharacterHeight = 0;
   //:SHORT       nCharacterWidth
   int      nCharacterWidth = 0;
   //:SHORT       nBoldFlag
   int      nBoldFlag = 0;
   //:SHORT       nItalicFlag
   int      nItalicFlag = 0;
   //:SHORT       nRC
   int      nRC = 0;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   String   szTempString_0 = null;
   int      lTempInteger_4 = 0;
   int      lTempInteger_5 = 0;
   int      lTempInteger_6 = 0;
   int      lTempInteger_7 = 0;
   int      lTempInteger_8 = 0;
   int      lTempInteger_9 = 0;
   int      lTempInteger_10 = 0;
   String   szTempString_1 = null;
   int      lTempInteger_11 = 0;
   int      lTempInteger_12 = 0;
   int      lTempInteger_13 = 0;
   int      lTempInteger_14 = 0;
   int      lTempInteger_15 = 0;
   int      lTempInteger_16 = 0;


   //:// Retrieve default information and name it for later use.  If the instance
   //:// doesn't exist, exit.
   //:GET VIEW vTaskLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( vTaskLPLR, "TaskLPLR", vReportDef, zLEVEL_TASK );
   //:szFileName = vTaskLPLR.LPLR.MetaSrcDir + "\Z__DFLT.BAS"
   {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
       GetStringFromAttribute( sb_szFileName, vTaskLPLR, "LPLR", "MetaSrcDir" );
   szFileName = sb_szFileName.toString( );}
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringConcat( sb_szFileName, 1, 0, "\\Z__DFLT.BAS", 1, 0, 514 );
   szFileName = sb_szFileName.toString( );}
   //:lFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ )
   try
   {
       lFile = m_KZOEP1AA.SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   //:IF lFile < 0
   if ( lFile < 0 )
   { 
      //:szMsg = "LPLR Defaults not found." + zNew_Line + "Exiting Autodesign."
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "LPLR Defaults not found.", 1, 0, 258 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, zNEW_LINE, 1, 0, 258 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, "Exiting Autodesign.", 1, 0, 258 );
      szMsg = sb_szMsg.toString( );}
      //:MessageSend( vSubtask, "AD10201", "Autodesigner",
      //:             "LPLR Defaults not found.\nExiting Autodesign.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "AD10201", "Autodesigner", "LPLR Defaults not found.\\nExiting Autodesign.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
      //:ELSE
   } 
   else
   { 
      //:SysCloseFile( vSubtask, lFile, 0 )
      try
      {
          m_KZOEP1AA.SysCloseFile( vSubtask, lFile, null );
      }
      catch ( Exception e )
      {
         throw ZeidonException.wrapException( e );
      }
      //:ActivateOI_FromFile( vDefaults, "TZADCSDO", vUserSpec,
      //:                     szFileName, zSINGLE )
      ActivateOI_FromFile( vDefaults, "TZADCSDO", vUserSpec, szFileName, zSINGLE );
      //:NAME VIEW vDefaults "TZ_Defaults"
      SetNameForView( vDefaults, "TZ_Defaults", null, zLEVEL_TASK );
   } 

   //:END

   //:// if Font Size does not exists, set size
   //:IF vDefaults.InitValues.ReportTextSize <= 0
   if ( CompareAttributeToInteger( vDefaults, "InitValues", "ReportTextSize", 0 ) <= 0 )
   { 
      //:szMsg = "No valid Default Font Size found. This Report" + zNew_Line
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "No valid Default Font Size found. This Report", 1, 0, 258 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, zNEW_LINE, 1, 0, 258 );
      szMsg = sb_szMsg.toString( );}
      //:szMsg = szMsg + "will be created with a Font Size 10."
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, "will be created with a Font Size 10.", 1, 0, 258 );
      szMsg = sb_szMsg.toString( );}
      //:nRC = MessagePrompt( vReportDef, "AD10202", "Autodesigner",
      //:                     szMsg, 1, zBUTTONS_OKCANCEL, zRESPONSE_OK,
      //:                     zICON_QUESTION   )
      nRC = MessagePrompt( vReportDef, "AD10202", "Autodesigner", szMsg, 1, zBUTTONS_OKCANCEL, zRESPONSE_OK, zICON_QUESTION );
      //:IF nRC = zRESPONSE_CANCEL
      if ( nRC == zRESPONSE_CANCEL )
      { 
         //:RETURN -1
         if(8==8)return( -1 );
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
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Page", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
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
   {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
       GetIntegerFromAttribute( mi_lTempInteger_0, vDefaults, "InitValues", "ReportTextIncrementY" );
   lTempInteger_0 = mi_lTempInteger_0.intValue( );}
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
   {MutableInt mi_lPageWidth = new MutableInt( lPageWidth );
       GetIntegerFromAttribute( mi_lPageWidth, vReportDef, "Page", "SizeX" );
   lPageWidth = mi_lPageWidth.intValue( );}
   //:lPageNumberPosX      = lPageWidth - 256
   lPageNumberPosX = lPageWidth - 256;
   //:lDefaultFontSize     = vDefaults.InitValues.ReportTextSize * 10
   {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
       GetIntegerFromAttribute( mi_lTempInteger_1, vDefaults, "InitValues", "ReportTextSize" );
   lTempInteger_1 = mi_lTempInteger_1.intValue( );}
   lDefaultFontSize = lTempInteger_1 * 10;
   //:lCurrentPosY         = 0
   lCurrentPosY = 0;

   //:// This transformation uses the User Interface Spec to build a report
   //:// definition.

   //:// First, we will build the whole Report Definition without specifying
   //:// any positions.  Then we will come back and determine positions.

   //:NAME VIEW vReportDef "***ReportDef"
   SetNameForView( vReportDef, "***ReportDef", null, zLEVEL_TASK );
   //:NAME VIEW vUserSpec "***UserSpec"
   SetNameForView( vUserSpec, "***UserSpec", null, zLEVEL_TASK );
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
   CreateViewFromViewForTask( vUserSpecRoot, vUserSpec, null );

   //:// Create Page Header GroupSet and Group with initial values.

   //:CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
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
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
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
      {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
             GetIntegerFromAttribute( mi_lTempInteger_3, vUserSpec, "UI_Spec", "HeaderFontSize" );
      lTempInteger_3 = mi_lTempInteger_3.intValue( );}
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
      lVerticalIncrement = (int) nCharacterHeight * dVertScalingFactor * 16;

      //:IF vUserSpec.UI_Spec.HeaderTitle != ""
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "HeaderTitle", "" ) != 0 )
      { 

         //:// Set up Page Header Text.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
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
         {StringBuilder sb_szTempString_0;
         if ( szTempString_0 == null )
            sb_szTempString_0 = new StringBuilder( 32 );
         else
            sb_szTempString_0 = new StringBuilder( szTempString_0 );
                   GetStringFromAttribute( sb_szTempString_0, vReportDef, "Control", "Text" );
         szTempString_0 = sb_szTempString_0.toString( );}
         {
          TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vReportDef );
          lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_0 );
          // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
         }
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
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );
      } 

      //:END

      //:IF vUserSpec.UI_Spec.HeaderPageNoFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "HeaderPageNoFlag", "Y" ) == 0 )
      { 

         //:// Set up Header Page Number.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
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
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );
      } 

      //:END

      //:// Set height of the group and groupset.  We will determine the Y size by
      //:// getting the last control (bottom in Group, though any control would do) and
      //:// adding BottomGroupMargin to it.
      //:lHeight = vReportDef.Control.PSDLG_Y + vReportDef.Control.SZDLG_Y +
      //:          lVerticalIncrement + (lBottomHeaderMargin * 2)
      {MutableInt mi_lTempInteger_7 = new MutableInt( lTempInteger_7 );
             GetIntegerFromAttribute( mi_lTempInteger_7, vReportDef, "Control", "PSDLG_Y" );
      lTempInteger_7 = mi_lTempInteger_7.intValue( );}
      {MutableInt mi_lTempInteger_8 = new MutableInt( lTempInteger_8 );
             GetIntegerFromAttribute( mi_lTempInteger_8, vReportDef, "Control", "SZDLG_Y" );
      lTempInteger_8 = mi_lTempInteger_8.intValue( );}
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
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
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
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
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
      {MutableInt mi_lTempInteger_10 = new MutableInt( lTempInteger_10 );
             GetIntegerFromAttribute( mi_lTempInteger_10, vUserSpec, "UI_Spec", "FooterFontSize" );
      lTempInteger_10 = mi_lTempInteger_10.intValue( );}
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
      lVerticalIncrement = (int) nCharacterHeight * dVertScalingFactor * 16;

      //:lCurrentVerticalPos = lTopGroupMargin
      lCurrentVerticalPos = lTopGroupMargin;

      //:IF vUserSpec.UI_Spec.FooterTitle != ""
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "FooterTitle", "" ) != 0 )
      { 

         //:// Set up Page Footer Text.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
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
         {StringBuilder sb_szTempString_1;
         if ( szTempString_1 == null )
            sb_szTempString_1 = new StringBuilder( 32 );
         else
            sb_szTempString_1 = new StringBuilder( szTempString_1 );
                   GetStringFromAttribute( sb_szTempString_1, vReportDef, "Control", "Text" );
         szTempString_1 = sb_szTempString_1.toString( );}
         {
          TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vReportDef );
          lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_1 );
          // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
         }
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
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );
      } 

      //:END

      //:IF vUserSpec.UI_Spec.FooterPageNoFlag = "Y"
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "FooterPageNoFlag", "Y" ) == 0 )
      { 

         //:// Set up Footer Page Number.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
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
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );
      } 

      //:END

      //:// Set height of the group and groupset.  We will determine the Y size by
      //:// getting the last control (bottom in Group, though any control would do) and
      //:// adding BottomGroupMargin to it.
      //:lHeight = vReportDef.Control.PSDLG_Y + vReportDef.Control.SZDLG_Y +
      //:          lVerticalIncrement + (lBottomHeaderMargin * 2)
      {MutableInt mi_lTempInteger_14 = new MutableInt( lTempInteger_14 );
             GetIntegerFromAttribute( mi_lTempInteger_14, vReportDef, "Control", "PSDLG_Y" );
      lTempInteger_14 = mi_lTempInteger_14.intValue( );}
      {MutableInt mi_lTempInteger_15 = new MutableInt( lTempInteger_15 );
             GetIntegerFromAttribute( mi_lTempInteger_15, vReportDef, "Control", "SZDLG_Y" );
      lTempInteger_15 = mi_lTempInteger_15.intValue( );}
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
         {MutableInt mi_lTempInteger_16 = new MutableInt( lTempInteger_16 );
                   GetIntegerFromAttribute( mi_lTempInteger_16, vReportDef, "GroupSet", "SZDLG_Y" );
         lTempInteger_16 = mi_lTempInteger_16.intValue( );}
         lCurrentVerticalPos = lCurrentVerticalPos + lTempInteger_16;
      } 

      RESULT = SetCursorNextEntity( vReportDef, "GroupSet", "" );

      //:END
   } 

   //:END
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
private int 
oTZRPSRCO_GenerateXRP_Recurs( View     NewReport,
                              View     DialogControl,
                              int      X_Position,
                              int      Y_Position,
                              int      GroupSize )
{
   zVIEW    TZPESRCO = new zVIEW( );
   int      RESULT = 0;
   //:INTEGER TextControlHeight
   int      TextControlHeight = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;
   int      lTempInteger_5 = 0;
   int      lTempInteger_6 = 0;
   int      lTempInteger_7 = 0;
   int      lTempInteger_8 = 0;
   int      lTempInteger_9 = 0;
   int      lTempInteger_10 = 0;
   int      lTempInteger_11 = 0;
   int      lTempInteger_12 = 0;
   int      lTempInteger_13 = 0;
   int      lTempInteger_14 = 0;
   int      lTempInteger_15 = 0;
   int      lTempInteger_16 = 0;
   int      lTempInteger_17 = 0;
   int      lTempInteger_18 = 0;
   int      lTempInteger_19 = 0;
   int      lTempInteger_20 = 0;
   int      lTempInteger_21 = 0;
   int      lTempInteger_22 = 0;
   int      lTempInteger_23 = 0;
   int      lTempInteger_24 = 0;
   int      lTempInteger_25 = 0;
   int      lTempInteger_26 = 0;
   int      lTempInteger_27 = 0;
   int      lTempInteger_28 = 0;

   RESULT = GetViewByName( TZPESRCO, "TZPESRCO", NewReport, zLEVEL_TASK );

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
         {MutableInt mi_X_Position = new MutableInt( X_Position );
                   GetIntegerFromAttribute( mi_X_Position, DialogControl, "Control", "PSDLG_X" );
         X_Position = mi_X_Position.intValue( );}
         //:Y_Position = DialogControl.Control.PSDLG_Y
         {MutableInt mi_Y_Position = new MutableInt( Y_Position );
                   GetIntegerFromAttribute( mi_Y_Position, DialogControl, "Control", "PSDLG_Y" );
         Y_Position = mi_Y_Position.intValue( );}
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
            {MutableInt mi_X_Position = new MutableInt( X_Position );
                         GetIntegerFromAttribute( mi_X_Position, DialogControl, "Control", "PSDLG_X" );
            X_Position = mi_X_Position.intValue( );}
            //:Y_Position = DialogControl.Control.PSDLG_Y
            {MutableInt mi_Y_Position = new MutableInt( Y_Position );
                         GetIntegerFromAttribute( mi_Y_Position, DialogControl, "Control", "PSDLG_Y" );
            Y_Position = mi_Y_Position.intValue( );}
            //:CreateMetaEntity( DialogControl, NewReport, "GroupSet", zPOS_AFTER )
            {
             TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
             m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "GroupSet", zPOS_AFTER );
             // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
            }
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
            {
             TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
             m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "Group", zPOS_AFTER );
             // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
            }
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
                  {
                   TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
                   m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                   // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
                  }
                  //:NewReport.Control.Tag = DialogControl.Control.Tag
                  SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                  //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                  {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
                                     GetIntegerFromAttribute( mi_lTempInteger_0, DialogControl, "Control", "PSDLG_X" );
                  lTempInteger_0 = mi_lTempInteger_0.intValue( );}
                  lTempInteger_1 = X_Position + lTempInteger_0;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_1 );
                  //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                  {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
                                     GetIntegerFromAttribute( mi_lTempInteger_2, DialogControl, "Control", "PSDLG_Y" );
                  lTempInteger_2 = mi_lTempInteger_2.intValue( );}
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
            {
             TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
             m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "Group", zPOS_AFTER );
             // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
            }
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
                  {
                   TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
                   m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                   // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
                  }
                  //:NewReport.Control.Tag = DialogControl.Control.Tag
                  SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                  //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                  {MutableInt mi_lTempInteger_4 = new MutableInt( lTempInteger_4 );
                                     GetIntegerFromAttribute( mi_lTempInteger_4, DialogControl, "Control", "PSDLG_X" );
                  lTempInteger_4 = mi_lTempInteger_4.intValue( );}
                  lTempInteger_5 = X_Position + lTempInteger_4;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_5 );
                  //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                  {MutableInt mi_lTempInteger_6 = new MutableInt( lTempInteger_6 );
                                     GetIntegerFromAttribute( mi_lTempInteger_6, DialogControl, "Control", "PSDLG_Y" );
                  lTempInteger_6 = mi_lTempInteger_6.intValue( );}
                  lTempInteger_7 = Y_Position + lTempInteger_6;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_7 );
                  //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                  SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                  //:NewReport.Control.SZDLG_Y = TextControlHeight
                  SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                  //:GroupSize = NewReport.Control.PSDLG_Y + NewReport.Control.SZDLG_Y
                  {MutableInt mi_lTempInteger_8 = new MutableInt( lTempInteger_8 );
                                     GetIntegerFromAttribute( mi_lTempInteger_8, NewReport, "Control", "PSDLG_Y" );
                  lTempInteger_8 = mi_lTempInteger_8.intValue( );}
                  {MutableInt mi_lTempInteger_9 = new MutableInt( lTempInteger_9 );
                                     GetIntegerFromAttribute( mi_lTempInteger_9, NewReport, "Control", "SZDLG_Y" );
                  lTempInteger_9 = mi_lTempInteger_9.intValue( );}
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
                  {
                   TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
                   m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                   // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
                  }
                  //:NewReport.Control.Tag = DialogControl.Control.Tag
                  SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                  //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                  {MutableInt mi_lTempInteger_11 = new MutableInt( lTempInteger_11 );
                                     GetIntegerFromAttribute( mi_lTempInteger_11, DialogControl, "Control", "PSDLG_X" );
                  lTempInteger_11 = mi_lTempInteger_11.intValue( );}
                  lTempInteger_12 = X_Position + lTempInteger_11;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_12 );
                  //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                  {MutableInt mi_lTempInteger_13 = new MutableInt( lTempInteger_13 );
                                     GetIntegerFromAttribute( mi_lTempInteger_13, DialogControl, "Control", "PSDLG_Y" );
                  lTempInteger_13 = mi_lTempInteger_13.intValue( );}
                  lTempInteger_14 = Y_Position + lTempInteger_13;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_14 );
                  //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                  SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                  //:NewReport.Control.SZDLG_Y = TextControlHeight
                  SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                  //:GroupSize = NewReport.Control.PSDLG_Y + NewReport.Control.SZDLG_Y
                  {MutableInt mi_lTempInteger_15 = new MutableInt( lTempInteger_15 );
                                     GetIntegerFromAttribute( mi_lTempInteger_15, NewReport, "Control", "PSDLG_Y" );
                  lTempInteger_15 = mi_lTempInteger_15.intValue( );}
                  {MutableInt mi_lTempInteger_16 = new MutableInt( lTempInteger_16 );
                                     GetIntegerFromAttribute( mi_lTempInteger_16, NewReport, "Control", "SZDLG_Y" );
                  lTempInteger_16 = mi_lTempInteger_16.intValue( );}
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
                     {
                      TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
                      m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                      // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
                     }
                     //:NewReport.Control.Tag = DialogControl.Control.Tag
                     SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                     //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                     {MutableInt mi_lTempInteger_17 = new MutableInt( lTempInteger_17 );
                                           GetIntegerFromAttribute( mi_lTempInteger_17, DialogControl, "Control", "PSDLG_X" );
                     lTempInteger_17 = mi_lTempInteger_17.intValue( );}
                     lTempInteger_18 = X_Position + lTempInteger_17;
                     SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_18 );
                     //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                     {MutableInt mi_lTempInteger_19 = new MutableInt( lTempInteger_19 );
                                           GetIntegerFromAttribute( mi_lTempInteger_19, DialogControl, "Control", "PSDLG_Y" );
                     lTempInteger_19 = mi_lTempInteger_19.intValue( );}
                     lTempInteger_20 = Y_Position + lTempInteger_19;
                     SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_20 );
                     //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                     SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                     //:NewReport.Control.SZDLG_Y = TextControlHeight
                     SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                     //:GroupSize = NewReport.Control.PSDLG_Y + NewReport.Control.SZDLG_Y
                     {MutableInt mi_lTempInteger_21 = new MutableInt( lTempInteger_21 );
                                           GetIntegerFromAttribute( mi_lTempInteger_21, NewReport, "Control", "PSDLG_Y" );
                     lTempInteger_21 = mi_lTempInteger_21.intValue( );}
                     {MutableInt mi_lTempInteger_22 = new MutableInt( lTempInteger_22 );
                                           GetIntegerFromAttribute( mi_lTempInteger_22, NewReport, "Control", "SZDLG_Y" );
                     lTempInteger_22 = mi_lTempInteger_22.intValue( );}
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
                  {
                   TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
                   m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "Control", zPOS_AFTER );
                   // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
                  }
                  //:NewReport.Control.Tag = DialogControl.Control.Tag
                  SetAttributeFromAttribute( NewReport, "Control", "Tag", DialogControl, "Control", "Tag" );
                  //:NewReport.Control.PSDLG_X = X_Position + DialogControl.Control.PSDLG_X
                  {MutableInt mi_lTempInteger_23 = new MutableInt( lTempInteger_23 );
                                     GetIntegerFromAttribute( mi_lTempInteger_23, DialogControl, "Control", "PSDLG_X" );
                  lTempInteger_23 = mi_lTempInteger_23.intValue( );}
                  lTempInteger_24 = X_Position + lTempInteger_23;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_X", lTempInteger_24 );
                  //:NewReport.Control.PSDLG_Y = Y_Position + DialogControl.Control.PSDLG_Y
                  {MutableInt mi_lTempInteger_25 = new MutableInt( lTempInteger_25 );
                                     GetIntegerFromAttribute( mi_lTempInteger_25, DialogControl, "Control", "PSDLG_Y" );
                  lTempInteger_25 = mi_lTempInteger_25.intValue( );}
                  lTempInteger_26 = Y_Position + lTempInteger_25;
                  SetAttributeFromInteger( NewReport, "Control", "PSDLG_Y", lTempInteger_26 );
                  //:NewReport.Control.SZDLG_X = DialogControl.Control.SZDLG_X
                  SetAttributeFromAttribute( NewReport, "Control", "SZDLG_X", DialogControl, "Control", "SZDLG_X" );
                  //:NewReport.Control.SZDLG_Y = TextControlHeight
                  SetAttributeFromInteger( NewReport, "Control", "SZDLG_Y", TextControlHeight );
                  //:GroupSize = NewReport.Control.PSDLG_Y + NewReport.Control.SZDLG_Y
                  {MutableInt mi_lTempInteger_27 = new MutableInt( lTempInteger_27 );
                                     GetIntegerFromAttribute( mi_lTempInteger_27, NewReport, "Control", "PSDLG_Y" );
                  lTempInteger_27 = mi_lTempInteger_27.intValue( );}
                  {MutableInt mi_lTempInteger_28 = new MutableInt( lTempInteger_28 );
                                     GetIntegerFromAttribute( mi_lTempInteger_28, NewReport, "Control", "SZDLG_Y" );
                  lTempInteger_28 = mi_lTempInteger_28.intValue( );}
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


//:TRANSFORMATION OPERATION
//:GenerateXSLT_PDF( VIEW vReportDef,
//:                  VIEW vSourceOI,
//:                  STRING ( 32 ) szTopEntityName,
//:                  INTEGER lFlag )
public int 
oTZRPSRCO_GenerateXSLT_PDF( View     vReportDef,
                            View     vSourceOI,
                            String   szTopEntityName,
                            int      lFlag )
{

   return( 0 );
//    // The first step is to build the XML object for the passed in application object and Report Definition.
//    //GenerateXSLT_XML( vReportDef, vSourceOI, szTopEntityName )
// END
} 


//:LOCAL OPERATION
private int 
oTZRPSRCO_GenerateLine( View     ReportObject,
                        int      lFileHandle,
                        String   szOutputLine )
{

   //:GenerateLine( VIEW ReportObject BASED ON LOD TZRPSRCO,
   //:           INTEGER lFileHandle,
   //:           STRING ( 5000 ) szOutputLine )

   //:// This is just a SysWriteLine with an options TraceLineS statement.
   //:TraceLineS( "*** Line: ", szOutputLine )
   TraceLineS( "*** Line: ", szOutputLine );
   //:SysWriteLine( ReportObject, lFileHandle, szOutputLine )
   try
   {
       m_KZOEP1AA.SysWriteLine( ReportObject, lFileHandle, szOutputLine );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ConvertPxToInchesCH( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                     STRING ( 20 ) ReturnedString,
//:                     INTEGER lValueInPixles )

//:   DECIMAL dValueInInches
private int 
oTZRPSRCO_ConvertPxToInchesCH( View     vReportDefRoot,
                               StringBuilder   ReturnedString,
                               int      lValueInPixles )
{
   double  dValueInInches = 0.0;
   //:DECIMAL dDecimal0
   double  dDecimal0 = 0.0;
   //:INTEGER lValueInInches
   int      lValueInInches = 0;


   //:// Convert pixles integer value to decimal 2 as a returned string.
   //://lValueInInches = ( lValueInPixles * 100 ) / 256
   //:lValueInInches = ( lValueInPixles * 100 ) / 280
   lValueInInches = ( lValueInPixles * 100 ) / 280;
   //:dDecimal0 = lValueInInches
   dDecimal0 = lValueInInches;
   //:dValueInInches = dDecimal0 / 100
   dValueInInches = dDecimal0 / 100;
   //:ReturnedString = dValueInInches
   ZeidonStringConvertFromNumber( ReturnedString, 1, 0, 20, 0, dValueInInches, "D" );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_EntityRecur( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                  VIEW vLOD           BASED ON LOD TZZOLODO,
//:                  INTEGER lFileHandle,
//:                  STRING ( 5000 ) szOutputLine,
//:                  STRING ( 32 )  szTopEntityName )

//:   VIEW vReportDef BASED ON LOD TZRPSRCO
public int 
oTZRPSRCO_XSLT_EntityRecur( View     vReportDefRoot,
                            View     vLOD,
                            int      lFileHandle,
                            String   szOutputLine,
                            String   szTopEntityName )
{
   zVIEW    vReportDef = new zVIEW( );
   //:INTEGER PosY
   int      PosY = 0;
   //:INTEGER CurrentWidth
   int      CurrentWidth = 0;
   //:INTEGER ColumnCount
   int      ColumnCount = 0;
   //:INTEGER CellPadding
   int      CellPadding = 0;
   //:INTEGER GroupHierarchicalLevel
   int      GroupHierarchicalLevel = 0;
   //:STRING ( 50 )  szIndentationChars
   String   szIndentationChars = null;
   //:STRING ( 50 )  szGroupSetName
   String   szGroupSetName = null;
   //:STRING ( 10 )  szPageWidth
   String   szPageWidth = null;
   //:STRING ( 10 )  szCurrentWidth
   String   szCurrentWidth = null;
   //:STRING ( 1 )   szSingleRowFlag
   String   szSingleRowFlag = null;
   //:STRING ( 32 )  szEntityName
   String   szEntityName = null;
   //:STRING ( 32 )  szAttributeName
   String   szAttributeName = null;
   //:STRING ( 20 )  szCellPadding
   String   szCellPadding = null;
   //:STRING ( 20 )  szCellPaddingName
   String   szCellPaddingName = null;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;


   //:szPageWidth = "7.5in"
    {StringBuilder sb_szPageWidth;
   if ( szPageWidth == null )
      sb_szPageWidth = new StringBuilder( 32 );
   else
      sb_szPageWidth = new StringBuilder( szPageWidth );
      ZeidonStringCopy( sb_szPageWidth, 1, 0, "7.5in", 1, 0, 11 );
   szPageWidth = sb_szPageWidth.toString( );}
   //:szCellPaddingName = "DefaultCellPadding"
    {StringBuilder sb_szCellPaddingName;
   if ( szCellPaddingName == null )
      sb_szCellPaddingName = new StringBuilder( 32 );
   else
      sb_szCellPaddingName = new StringBuilder( szCellPaddingName );
      ZeidonStringCopy( sb_szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 21 );
   szCellPaddingName = sb_szCellPaddingName.toString( );}

   //://IssueError( vReportDefRoot,0,0, "In EntityR" )

   //:// Process each vLOD.LOD_EntityParent within the recursive LOD structure.
   //:FOR EACH vLOD.LOD_EntityParent
   RESULT = SetCursorFirstEntity( vLOD, "LOD_EntityParent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// See if this entity is defined for a GroupSet and if so, process it.
      //:szEntityName   = vLOD.LOD_EntityParent.Name
      {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
      StringBuilder sb_szEntityName;
      if ( szEntityName == null )
         sb_szEntityName = new StringBuilder( 32 );
      else
         sb_szEntityName = new StringBuilder( szEntityName );
             GetVariableFromAttribute( sb_szEntityName, mi_lTempInteger_0, 'S', 33, vLOD, "LOD_EntityParent", "Name", "", 0 );
      lTempInteger_0 = mi_lTempInteger_0.intValue( );
      szEntityName = sb_szEntityName.toString( );}
      //:SET CURSOR FIRST vReportDefRoot.GroupSet WHERE vReportDefRoot.GroupSet.Type = "E"
      //:             AND vReportDefRoot.GroupSet.Tag = szEntityName
      RESULT = SetCursorFirstEntity( vReportDefRoot, "GroupSet", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( vReportDefRoot, "GroupSet", "Type", "E" ) != 0 || CompareAttributeToString( vReportDefRoot, "GroupSet", "Tag", szEntityName ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( vReportDefRoot, "GroupSet", "" );
         } 

      } 

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 

         //:// GROUPSET

         //:szIndentationChars = "    "    // Base indentation will be 10 characters.
          {StringBuilder sb_szIndentationChars;
         if ( szIndentationChars == null )
            sb_szIndentationChars = new StringBuilder( 32 );
         else
            sb_szIndentationChars = new StringBuilder( szIndentationChars );
                  ZeidonStringCopy( sb_szIndentationChars, 1, 0, "    ", 1, 0, 51 );
         szIndentationChars = sb_szIndentationChars.toString( );}
         //:GenerateLine( vReportDefRoot, lFileHandle, " " )    // Blank line
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );

         //:// Open template statement
         //:IF szEntityName = szTopEntityName
         if ( ZeidonStringCompare( szEntityName, 1, 0, szTopEntityName, 1, 0, 33 ) == 0 )
         { 
            //:szGroupSetName = szEntityName    // Top Entity doesn't have GR_ prefix.
             {StringBuilder sb_szGroupSetName;
            if ( szGroupSetName == null )
               sb_szGroupSetName = new StringBuilder( 32 );
            else
               sb_szGroupSetName = new StringBuilder( szGroupSetName );
                        ZeidonStringCopy( sb_szGroupSetName, 1, 0, szEntityName, 1, 0, 51 );
            szGroupSetName = sb_szGroupSetName.toString( );}
            //:ELSE
         } 
         else
         { 
            //:szGroupSetName = "GR_" + szEntityName
             {StringBuilder sb_szGroupSetName;
            if ( szGroupSetName == null )
               sb_szGroupSetName = new StringBuilder( 32 );
            else
               sb_szGroupSetName = new StringBuilder( szGroupSetName );
                        ZeidonStringCopy( sb_szGroupSetName, 1, 0, "GR_", 1, 0, 51 );
            szGroupSetName = sb_szGroupSetName.toString( );}
             {StringBuilder sb_szGroupSetName;
            if ( szGroupSetName == null )
               sb_szGroupSetName = new StringBuilder( 32 );
            else
               sb_szGroupSetName = new StringBuilder( szGroupSetName );
                        ZeidonStringConcat( sb_szGroupSetName, 1, 0, szEntityName, 1, 0, 51 );
            szGroupSetName = sb_szGroupSetName.toString( );}
         } 

         //:END
         //:szOutputLine = "<xsl:template match=" + QUOTES + szGroupSetName + QUOTES + ">"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, "<xsl:template match=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, szGroupSetName, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

         //:// Initialize Work Group.
         //:// Note that GroupHierarchicalLevel is the Control level with the vReportDef object, not the entity level within the LOD.
         //:GroupHierarchicalLevel = 1
         GroupHierarchicalLevel = 1;
         //:IF vReportDefRoot.XSLT_WorkGroup DOES NOT EXIST
         lTempInteger_1 = CheckExistenceOfEntity( vReportDefRoot, "XSLT_WorkGroup" );
         if ( lTempInteger_1 != 0 )
         { 
            //:CREATE ENTITY vReportDefRoot.XSLT_WorkGroup
            RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkGroup", zPOS_AFTER );
         } 

         //:END
         //:vReportDefRoot.XSLT_WorkGroup.RecursiveNumber = GroupHierarchicalLevel
         SetAttributeFromInteger( vReportDefRoot, "XSLT_WorkGroup", "RecursiveNumber", GroupHierarchicalLevel );

         //:// Process each Group within the GroupSet.

         //:// Open block statement
         //:szOutputLine = "  <fo:block font-size=" + QUOTES + "9pt" + QUOTES + ">"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <fo:block font-size=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "9pt", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:GenerateLine( vReportDefRoot, lFileHandle, " " )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );

         //:SET CURSOR FIRST vReportDefRoot.Group WHERE vReportDefRoot.Group.Type = "gh"
         RESULT = SetCursorFirstEntityByString( vReportDefRoot, "Group", "Type", "gh", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 

            //:// Group is Header.

            //:// Generate Entity Header Comment.
            //:GenerateLine( vReportDefRoot, lFileHandle, " " )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
            //:szOutputLine = "  <!-- ENTITY HEADER -->"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <!-- ENTITY HEADER -->", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
            //:GenerateLine( vReportDefRoot, lFileHandle, " " )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );

            //:// Go to process the Header Group as a group of all text controls.
            //:XSLT_HeaderFooter( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szIndentationChars, "Header" )
            oTZRPSRCO_XSLT_HeaderFooter( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szIndentationChars, "Header" );
         } 


         //:END

         //:SET CURSOR FIRST vReportDefRoot.Group WHERE vReportDefRoot.Group.Type = "ga"
         RESULT = SetCursorFirstEntityByString( vReportDefRoot, "Group", "Type", "ga", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 

            //:// Group is Entity

            //:// Generate Entity Detail Comment.
            //:GenerateLine( vReportDefRoot, lFileHandle, " " )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
            //:szOutputLine = "  <!-- ENTITY DETAIL -->"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <!-- ENTITY DETAIL -->", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
            //:GenerateLine( vReportDefRoot, lFileHandle, " " )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );

            //:// Make sure Controls are in row/column order.
            //:OrderEntityForView( vReportDefRoot, "Control", "PSDLG_Y A PSDLG_X A" )
            OrderEntityForView( vReportDefRoot, "Control", "PSDLG_Y A PSDLG_X A" );

            //:// Check if all controls are on a single line, without a subcontrol.
            //://    If so, we will create a table entry with a column for each control.
            //://    If not, we will create a table entry with a single cell.
            //:szSingleRowFlag = "Y"
             {StringBuilder sb_szSingleRowFlag;
            if ( szSingleRowFlag == null )
               sb_szSingleRowFlag = new StringBuilder( 32 );
            else
               sb_szSingleRowFlag = new StringBuilder( szSingleRowFlag );
                        ZeidonStringCopy( sb_szSingleRowFlag, 1, 0, "Y", 1, 0, 2 );
            szSingleRowFlag = sb_szSingleRowFlag.toString( );}
            //:PosY = vReportDefRoot.Control.PSDLG_Y
            {MutableInt mi_PosY = new MutableInt( PosY );
                         GetIntegerFromAttribute( mi_PosY, vReportDefRoot, "Control", "PSDLG_Y" );
            PosY = mi_PosY.intValue( );}
            //:FOR EACH vReportDefRoot.Control
            RESULT = SetCursorFirstEntity( vReportDefRoot, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:IF vReportDefRoot.CtrlCtrl EXISTS OR vReportDefRoot.Control.PSDLG_Y != PosY
               lTempInteger_2 = CheckExistenceOfEntity( vReportDefRoot, "CtrlCtrl" );
               if ( lTempInteger_2 == 0 || CompareAttributeToInteger( vReportDefRoot, "Control", "PSDLG_Y", PosY ) != 0 )
               { 
                  //:szSingleRowFlag = "N"
                   {StringBuilder sb_szSingleRowFlag;
                  if ( szSingleRowFlag == null )
                     sb_szSingleRowFlag = new StringBuilder( 32 );
                  else
                     sb_szSingleRowFlag = new StringBuilder( szSingleRowFlag );
                                    ZeidonStringCopy( sb_szSingleRowFlag, 1, 0, "N", 1, 0, 2 );
                  szSingleRowFlag = sb_szSingleRowFlag.toString( );}
               } 

               RESULT = SetCursorNextEntity( vReportDefRoot, "Control", "" );
               //:END
            } 

            //:END

            //:IF szSingleRowFlag = "N"
            if ( ZeidonStringCompare( szSingleRowFlag, 1, 0, "N", 1, 0, 2 ) == 0 )
            { 

               //:// Go to process more complex Group.
               //:CreateViewFromView( vReportDef, vReportDefRoot )
               CreateViewFromView( vReportDef, vReportDefRoot );
               //:NAME VIEW vReportDef "vReportDefTop"
               SetNameForView( vReportDef, "vReportDefTop", null, zLEVEL_TASK );
               //:XSLT_GroupRecur( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel )
               oTZRPSRCO_XSLT_GroupRecur( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel );
               //:DropView( vReportDef )
               DropView( vReportDef );

               //:ELSE
            } 
            else
            { 

               //:// Detail entity has single row.

               //:// Build work subobject, with a single row and columns.
               //:CREATE ENTITY vReportDefRoot.XSLT_WorkRow
               RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkRow", zPOS_AFTER );
               //:vReportDefRoot.XSLT_WorkRow.PSDLG_Y = vReportDefRoot.Control.PSDLG_Y
               SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y", vReportDefRoot, "Control", "PSDLG_Y" );
               //:FOR EACH vReportDefRoot.Control
               RESULT = SetCursorFirstEntity( vReportDefRoot, "Control", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:CREATE ENTITY vReportDefRoot.XSLT_WorkColumn
                  RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkColumn", zPOS_AFTER );
                  //:vReportDefRoot.XSLT_WorkColumn.ControlZKey = vReportDefRoot.Control.ZKey
                  SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkColumn", "ControlZKey", vReportDefRoot, "Control", "ZKey" );
                  //:vReportDefRoot.XSLT_WorkColumn.ControlType = vReportDefRoot.ControlDef.Tag
                  SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkColumn", "ControlType", vReportDefRoot, "ControlDef", "Tag" );
                  //:SetMatchingAttributesByName( vReportDefRoot, "XSLT_WorkColumn",  vReportDefRoot, "Control", zSET_NULL )
                  SetMatchingAttributesByName( vReportDefRoot, "XSLT_WorkColumn", vReportDefRoot, "Control", zSET_NULL );
                  RESULT = SetCursorNextEntity( vReportDefRoot, "Control", "" );
               } 

               //:END

               //:// Go to build the table.
               //:XSLT_GroupTable( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szIndentationChars, 1, szEntityName )
               oTZRPSRCO_XSLT_GroupTable( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szIndentationChars, 1, szEntityName );
            } 


            //:END
         } 


         //:END

         //:SET CURSOR FIRST vReportDefRoot.Group WHERE vReportDefRoot.Group.Type = "gf"
         RESULT = SetCursorFirstEntityByString( vReportDefRoot, "Group", "Type", "gf", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 

            //:// Group is Footer.

            //:// Generate Entity Footer Comment.
            //:GenerateLine( vReportDefRoot, lFileHandle, " " )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
            //:szOutputLine = "  <!-- ENTITY FOOTER -->"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <!-- ENTITY FOOTER -->", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
            //:GenerateLine( vReportDefRoot, lFileHandle, " " )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );

            //:// Go to process the Footer Group as a group of all text controls.
            //:XSLT_HeaderFooter( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szIndentationChars, "Footer" )
            oTZRPSRCO_XSLT_HeaderFooter( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szIndentationChars, "Footer" );
         } 


         //:END

         //:// Close block statement
         //:szOutputLine = "  </fo:block>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, "  </fo:block>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );


         //:// Close template statement
         //:szOutputLine = "</xsl:template>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, "</xsl:template>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
      } 


      //:END

      //:// Process Sub Entity if it exists.
      //:IF vLOD.LOD_EntityChild EXISTS
      lTempInteger_3 = CheckExistenceOfEntity( vLOD, "LOD_EntityChild" );
      if ( lTempInteger_3 == 0 )
      { 
         //:SetViewToSubobject( vLOD, "LOD_EntityChild" )
         SetViewToSubobject( vLOD, "LOD_EntityChild" );
         //:GroupHierarchicalLevel = GroupHierarchicalLevel + 1   // Indicate going down one more level.
         GroupHierarchicalLevel = GroupHierarchicalLevel + 1;
         //:XSLT_EntityRecur( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szTopEntityName )
         oTZRPSRCO_XSLT_EntityRecur( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szTopEntityName );
         //:GroupHierarchicalLevel = GroupHierarchicalLevel - 1
         GroupHierarchicalLevel = GroupHierarchicalLevel - 1;
         //:ResetViewFromSubobject( vLOD )
         ResetViewFromSubobject( vLOD );
      } 

      RESULT = SetCursorNextEntity( vLOD, "LOD_EntityParent", "" );
      //:END
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_HeaderFooter( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                   VIEW vLOD           BASED ON LOD TZZOLODO,
//:                   INTEGER lFileHandle,
//:                   STRING ( 5000 ) szOutputLine,
//:                   STRING ( 50 )   szIndentationChars,
//:                   STRING ( 32 )   szForEachName )

//:   STRING ( 20 )  szPageWidth
public int 
oTZRPSRCO_XSLT_HeaderFooter( View     vReportDefRoot,
                             View     vLOD,
                             int      lFileHandle,
                             String   szOutputLine,
                             String   szIndentationChars,
                             String   szForEachName )
{
   String   szPageWidth = null;
   //:STRING ( 50 )  szSubIndentationChars
   String   szSubIndentationChars = null;


   //:szPageWidth = "7.5"
    {StringBuilder sb_szPageWidth;
   if ( szPageWidth == null )
      sb_szPageWidth = new StringBuilder( 32 );
   else
      sb_szPageWidth = new StringBuilder( szPageWidth );
      ZeidonStringCopy( sb_szPageWidth, 1, 0, "7.5", 1, 0, 21 );
   szPageWidth = sb_szPageWidth.toString( );}

   //:// The Header/Footer starts block, table, ... table-cell.

   //:// Block
   //:szOutputLine = szIndentationChars + ""
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:block border=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "normal solid black", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:// Table statement
   //:szOutputLine = szIndentationChars + "<fo:table table-layout=" + QUOTES + "fixed" +
   //:               QUOTES + " width=" + QUOTES + szPageWidth + "in" +
   //:               QUOTES + " margin-top=" + QUOTES + "0" +  "in" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table table-layout=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "fixed", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " width=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szPageWidth, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "0", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:// Column, body, for-each,
   //:szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szPageWidth + "in" + QUOTES + "/>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szPageWidth, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "<fo:table-body>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-body>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "  <xsl:for-each select=" + QUOTES + szForEachName + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "  <xsl:for-each select=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szForEachName, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "  <fo:table-row>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "  <fo:table-row>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "    <fo:table-cell>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "    <fo:table-cell>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:// Go to process the controls as an all-text group.
   //:szSubIndentationChars = szIndentationChars + "      "
    {StringBuilder sb_szSubIndentationChars;
   if ( szSubIndentationChars == null )
      sb_szSubIndentationChars = new StringBuilder( 32 );
   else
      sb_szSubIndentationChars = new StringBuilder( szSubIndentationChars );
      ZeidonStringCopy( sb_szSubIndentationChars, 1, 0, szIndentationChars, 1, 0, 51 );
   szSubIndentationChars = sb_szSubIndentationChars.toString( );}
    {StringBuilder sb_szSubIndentationChars;
   if ( szSubIndentationChars == null )
      sb_szSubIndentationChars = new StringBuilder( 32 );
   else
      sb_szSubIndentationChars = new StringBuilder( szSubIndentationChars );
      ZeidonStringConcat( sb_szSubIndentationChars, 1, 0, "      ", 1, 0, 51 );
   szSubIndentationChars = sb_szSubIndentationChars.toString( );}
   //:XSLT_AllTextGroup( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szSubIndentationChars )
   oTZRPSRCO_XSLT_AllTextGroup( vReportDefRoot, vLOD, lFileHandle, szOutputLine, szSubIndentationChars );

   //:// Close cell, row, ... table, block.

   //:szOutputLine = szIndentationChars + "    </fo:table-cell>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "  </fo:table-row>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "  </fo:table-row>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "   </xsl:for-each>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "   </xsl:for-each>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "</fo:table-body>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:table-body>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "</fo:table>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:table>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:szOutputLine = szIndentationChars + "</fo:block>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_SubTextGroup( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                   VIEW vReportDef  BASED ON LOD TZRPSRCO,
//:                   VIEW vLOD           BASED ON LOD TZZOLODO,
//:                   INTEGER lFileHandle,
//:                   STRING ( 5000 ) szOutputLine,
//:                   STRING ( 50 )  szIndentationChars,
//:                   INTEGER GroupHierarchicalLevel )

//:   VIEW vReportDef3 BASED ON LOD TZRPSRCO
public int 
oTZRPSRCO_XSLT_SubTextGroup( View     vReportDefRoot,
                             View     vReportDef,
                             View     vLOD,
                             int      lFileHandle,
                             String   szOutputLine,
                             String   szIndentationChars,
                             int      GroupHierarchicalLevel )
{
   zVIEW    vReportDef3 = new zVIEW( );
   //:STRING ( 20 )  szCurrentWidth
   String   szCurrentWidth = null;
   //:STRING ( 20 )  szPageWidth
   String   szPageWidth = null;
   //:STRING ( 30 )  szCellPaddingName
   String   szCellPaddingName = null;
   //:STRING ( 50 )  szAttributeMappingName
   String   szAttributeMappingName = null;
   //:STRING ( 256 ) szConstantArea
   String   szConstantArea = null;
   //:STRING ( 50 )  szReportDefName
   String   szReportDefName = null;
   //:STRING ( 50 )  szSubIndentationChars
   String   szSubIndentationChars = null;
   //:STRING ( 50 )  szCellPadding
   String   szCellPadding = null;
   //:STRING ( 20 )  szTableMarginTop
   String   szTableMarginTop = null;
   //:STRING ( 20 )  szTablePaddingLeft
   String   szTablePaddingLeft = null;
   //:STRING ( 20 )  szTableWidth
   String   szTableWidth = null;
   //:STRING ( 20 )  szPaddingTop
   String   szPaddingTop = null;
   //:STRING ( 5 )   szColumnCount
   String   szColumnCount = null;
   //:STRING ( 200 ) szMsg
   String   szMsg = null;
   //:INTEGER       CurrentWidth
   int      CurrentWidth = 0;
   //:INTEGER       CurrentRowPositionX
   int      CurrentRowPositionX = 0;
   //:INTEGER       CurrentPositionY
   int      CurrentPositionY = 0;
   //:INTEGER       LastPositionY
   int      LastPositionY = 0;
   //:INTEGER       LastPositionRowY
   int      LastPositionRowY = 0;
   //:INTEGER       LastPositionX
   int      LastPositionX = 0;
   //:INTEGER       LastWidthX
   int      LastWidthX = 0;
   //:INTEGER       FirstColumnPosition
   int      FirstColumnPosition = 0;
   //:INTEGER       TopGroupPosition
   int      TopGroupPosition = 0;
   //:INTEGER       ColumnCount
   int      ColumnCount = 0;
   //:INTEGER       FirstColumnCount
   int      FirstColumnCount = 0;
   //:INTEGER       TextColumnCount
   int      TextColumnCount = 0;
   //:INTEGER       CellPadding
   int      CellPadding = 0;
   //:INTEGER       RowCount
   int      RowCount = 0;
   //:INTEGER       TablePaddingLeft
   int      TablePaddingLeft = 0;
   //:INTEGER       TableMarginTop
   int      TableMarginTop = 0;
   //:INTEGER       TableWidth
   int      TableWidth = 0;
   //:INTEGER       MaximumWidth
   int      MaximumWidth = 0;
   //:INTEGER       PaddingTop
   int      PaddingTop = 0;
   //:INTEGER       ControlCount
   int      ControlCount = 0;
   int      RESULT = 0;
   String   szTempString_0 = null;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;


   //:// Build a Table for all the Control entities, which are here considered to be all Text Controls, with the same number
   //:// of Text entries in EVERY row. This differs from the tables built in XSLT_GroupTable as follows:
   //:// In XSLT_GroupTable, every row of Control entries is built as a "single row table".
   //:// In this routine, because every row has the same number of Controls, all the Control entries are built as one table
   //:// with multiple rows.

   //:// Process each Rpt_Text Control by generating a table-row statement for each unique row (Y position) and a table-cell
   //:// statement for each control.
   //:// Note that all the rows must have the same number of columns, or an error message will be returned to the use, as each
   //:// row must have the same number of cells.

   //:NAME VIEW vReportDefRoot "vReportDefRootSubText"
   SetNameForView( vReportDefRoot, "vReportDefRootSubText", null, zLEVEL_TASK );

   //:szCellPaddingName = "DefaultCellPadding"
    {StringBuilder sb_szCellPaddingName;
   if ( szCellPaddingName == null )
      sb_szCellPaddingName = new StringBuilder( 32 );
   else
      sb_szCellPaddingName = new StringBuilder( szCellPaddingName );
      ZeidonStringCopy( sb_szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 31 );
   szCellPaddingName = sb_szCellPaddingName.toString( );}

   //:// Increment hierarchical level for creating XSLT_WorkGroup entry.
   //:GroupHierarchicalLevel = GroupHierarchicalLevel + 1
   GroupHierarchicalLevel = GroupHierarchicalLevel + 1;

   //:// Build work subobject, which organizes controls in rows and columns.
   //:LastPositionY = -1
   LastPositionY = -1;
   //:CREATE ENTITY vReportDefRoot.XSLT_WorkGroup
   RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkGroup", zPOS_AFTER );
   //:vReportDefRoot.XSLT_WorkGroup.RecursiveNumber = GroupHierarchicalLevel
   SetAttributeFromInteger( vReportDefRoot, "XSLT_WorkGroup", "RecursiveNumber", GroupHierarchicalLevel );
   //:ControlCount = 0
   ControlCount = 0;
   //:FOR EACH vReportDef.Control
   RESULT = SetCursorFirstEntity( vReportDef, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:ControlCount = ControlCount + 1
      ControlCount = ControlCount + 1;
      //:IF vReportDef.ControlDef.Tag = "Rpt_Text" OR vReportDef.ControlDef.Tag = "Rpt_PicGraphic"
      if ( CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_Text" ) == 0 || CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_PicGraphic" ) == 0 )
      { 
         //:CurrentPositionY = vReportDef.Control.PSDLG_Y
         {MutableInt mi_CurrentPositionY = new MutableInt( CurrentPositionY );
                   GetIntegerFromAttribute( mi_CurrentPositionY, vReportDef, "Control", "PSDLG_Y" );
         CurrentPositionY = mi_CurrentPositionY.intValue( );}
         //:IF CurrentPositionY != LastPositionY
         if ( CurrentPositionY != LastPositionY )
         { 
            //:CREATE ENTITY vReportDefRoot.XSLT_WorkRow
            RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkRow", zPOS_AFTER );
            //:vReportDefRoot.XSLT_WorkRow.PSDLG_Y = vReportDef.Control.PSDLG_Y
            SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y", vReportDef, "Control", "PSDLG_Y" );
         } 

         //:END
         //:LastPositionY = CurrentPositionY
         LastPositionY = CurrentPositionY;
         //:CREATE ENTITY vReportDefRoot.XSLT_WorkColumn
         RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkColumn", zPOS_AFTER );
         //:vReportDefRoot.XSLT_WorkColumn.ControlZKey = vReportDef.Control.ZKey
         SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkColumn", "ControlZKey", vReportDef, "Control", "ZKey" );
         //:vReportDefRoot.XSLT_WorkColumn.ControlType = vReportDef.ControlDef.Tag
         SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkColumn", "ControlType", vReportDef, "ControlDef", "Tag" );
         //:SetMatchingAttributesByName( vReportDefRoot, "XSLT_WorkColumn",  vReportDef, "Control", zSET_NULL )
         SetMatchingAttributesByName( vReportDefRoot, "XSLT_WorkColumn", vReportDef, "Control", zSET_NULL );
      } 

      RESULT = SetCursorNextEntity( vReportDef, "Control", "" );
      //:END
   } 

   //:END

   //:// Check if there is only a single Control under the parent, in which case we don't need to build a table here.
   //:IF ControlCount = 1
   if ( ControlCount == 1 )
   { 

      //:// There is only a single Control, so skip table.
      //:XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel )
      oTZRPSRCO_XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel );

      //:ELSE
   } 
   else
   { 

      //:// Build table, since there are multiple Controls.
      //:// We will build a single table statement, with a table-column statement for each column (remember, each row has
      //:// the same number of columns) and a table-row and table-cell statement for each row.

      //:// Check that all rows have the same number of columns.
      //:SET CURSOR FIRST vReportDefRoot.XSLT_WorkRow
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      //:ColumnCount = 0
      ColumnCount = 0;
      //:FOR EACH vReportDefRoot.XSLT_WorkColumn
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:ColumnCount = ColumnCount + 1
         ColumnCount = ColumnCount + 1;
         RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
      } 

      //:END
      //:FirstColumnCount = ColumnCount
      FirstColumnCount = ColumnCount;
      //:FOR EACH vReportDefRoot.XSLT_WorkRow
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:ColumnCount = 0
         ColumnCount = 0;
         //:FOR EACH vReportDefRoot.XSLT_WorkColumn
         RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:ColumnCount = ColumnCount + 1
            ColumnCount = ColumnCount + 1;
            RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         } 

         //:END
         //:IF ColumnCount != FirstColumnCount
         if ( ColumnCount != FirstColumnCount )
         { 
            //:szColumnCount = ColumnCount
             {StringBuilder sb_szColumnCount;
            if ( szColumnCount == null )
               sb_szColumnCount = new StringBuilder( 32 );
            else
               sb_szColumnCount = new StringBuilder( szColumnCount );
                        ZeidonStringConvertFromNumber( sb_szColumnCount, 1, 0, 5, ColumnCount, (double) 0.0, "I" );
            szColumnCount = sb_szColumnCount.toString( );}
            //:szMsg = "Unequal number of columns containing control, " + vReportDef.Control.Tag + NEW_LINE +
            //:        "Number of Columns generating error: " + szColumnCount
            {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
            StringBuilder sb_szTempString_0;
            if ( szTempString_0 == null )
               sb_szTempString_0 = new StringBuilder( 32 );
            else
               sb_szTempString_0 = new StringBuilder( szTempString_0 );
                         GetVariableFromAttribute( sb_szTempString_0, mi_lTempInteger_0, 'S', 33, vReportDef, "Control", "Tag", "", 0 );
            lTempInteger_0 = mi_lTempInteger_0.intValue( );
            szTempString_0 = sb_szTempString_0.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringCopy( sb_szMsg, 1, 0, "Unequal number of columns containing control, ", 1, 0, 201 );
            szMsg = sb_szMsg.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, szTempString_0, 1, 0, 201 );
            szMsg = sb_szMsg.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, NEW_LINE, 1, 0, 201 );
            szMsg = sb_szMsg.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, "Number of Columns generating error: ", 1, 0, 201 );
            szMsg = sb_szMsg.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, szColumnCount, 1, 0, 201 );
            szMsg = sb_szMsg.toString( );}
            //:MessageSend( vReportDef, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vReportDef, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         } 

         RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
         //:END
      } 

      //:END

      //:// Build open block & table statements.

      //:// We'll get width from parent.
      //:CreateViewFromView( vReportDef3, vReportDef )
      CreateViewFromView( vReportDef3, vReportDef );
      //:ResetViewFromSubobject( vReportDef3 )
      ResetViewFromSubobject( vReportDef3 );
      //:TableWidth = vReportDef3.Control.SZDLG_X
      {MutableInt mi_TableWidth = new MutableInt( TableWidth );
             GetIntegerFromAttribute( mi_TableWidth, vReportDef3, "Control", "SZDLG_X" );
      TableWidth = mi_TableWidth.intValue( );}
      //:DropView( vReportDef3 )
      DropView( vReportDef3 );
      //:// MarginTop will come from Y position of first Control.
      //:SET CURSOR FIRST vReportDefRoot.XSLT_WorkRow
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      //:TableMarginTop = vReportDefRoot.XSLT_WorkRow.PSDLG_Y
      {MutableInt mi_TableMarginTop = new MutableInt( TableMarginTop );
             GetIntegerFromAttribute( mi_TableMarginTop, vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y" );
      TableMarginTop = mi_TableMarginTop.intValue( );}
      //:XSLT_BuildTable( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel, TableMarginTop, TableWidth )
      oTZRPSRCO_XSLT_BuildTable( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel, TableMarginTop, TableWidth );

      //:// Build the table-column statments from columns in the first row.
      //:// As noted above, we only need to process columns from the first row as all rows should have the same number of columns.
      //:// However, we want the maximum width of the corresponding controls, so we will compute that first.
      //:FOR EACH vReportDefRoot.XSLT_WorkRow
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:ColumnCount = 0
         ColumnCount = 0;
         //:FOR EACH vReportDefRoot.XSLT_WorkColumn
         RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:ColumnCount = ColumnCount + 1
            ColumnCount = ColumnCount + 1;
            //:SET CURSOR FIRST vReportDefRoot.XSLT_ColumnWidth WHERE vReportDefRoot.XSLT_ColumnWidth.Index = ColumnCount
            RESULT = SetCursorFirstEntityByInteger( vReportDefRoot, "XSLT_ColumnWidth", "Index", ColumnCount, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:CREATE ENTITY vReportDefRoot.XSLT_ColumnWidth
               RESULT = CreateEntity( vReportDefRoot, "XSLT_ColumnWidth", zPOS_AFTER );
               //:vReportDefRoot.XSLT_ColumnWidth.Index = ColumnCount
               SetAttributeFromInteger( vReportDefRoot, "XSLT_ColumnWidth", "Index", ColumnCount );
            } 

            //:END
            //:IF ColumnCount = 1
            if ( ColumnCount == 1 )
            { 
               //:MaximumWidth = vReportDefRoot.XSLT_WorkColumn.SZDLG_X + vReportDefRoot.XSLT_WorkColumn.PSDLG_X
               {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
                               GetIntegerFromAttribute( mi_lTempInteger_1, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
               lTempInteger_1 = mi_lTempInteger_1.intValue( );}
               {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
                               GetIntegerFromAttribute( mi_lTempInteger_2, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
               lTempInteger_2 = mi_lTempInteger_2.intValue( );}
               MaximumWidth = lTempInteger_1 + lTempInteger_2;
               //:ELSE
            } 
            else
            { 
               //:MaximumWidth = vReportDefRoot.XSLT_WorkColumn.SZDLG_X
               {MutableInt mi_MaximumWidth = new MutableInt( MaximumWidth );
                               GetIntegerFromAttribute( mi_MaximumWidth, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
               MaximumWidth = mi_MaximumWidth.intValue( );}
            } 

            //:END
            //:IF MaximumWidth > vReportDefRoot.XSLT_ColumnWidth.MaximumWidth
            if ( CompareAttributeToInteger( vReportDefRoot, "XSLT_ColumnWidth", "MaximumWidth", MaximumWidth ) < 0 )
            { 
               //:vReportDefRoot.XSLT_ColumnWidth.MaximumWidth = MaximumWidth
               SetAttributeFromInteger( vReportDefRoot, "XSLT_ColumnWidth", "MaximumWidth", MaximumWidth );
            } 

            RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
            //:END
         } 

         RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
         //:END
      } 

      //:END
      //:SET CURSOR FIRST vReportDefRoot.XSLT_WorkRow
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      //:ColumnCount = 0
      ColumnCount = 0;
      //:FOR EACH vReportDefRoot.XSLT_WorkColumn
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:ColumnCount = ColumnCount + 1
         ColumnCount = ColumnCount + 1;
         //:SET CURSOR FIRST vReportDefRoot.XSLT_ColumnWidth WHERE vReportDefRoot.XSLT_ColumnWidth.Index = ColumnCount
         RESULT = SetCursorFirstEntityByInteger( vReportDefRoot, "XSLT_ColumnWidth", "Index", ColumnCount, "" );

         //:CreateViewFromView( vReportDef3, vReportDefRoot )
         CreateViewFromView( vReportDef3, vReportDefRoot );
         //:NAME VIEW vReportDef3 "vReportDef3SubText"
         SetNameForView( vReportDef3, "vReportDef3SubText", null, zLEVEL_TASK );
         //:IF ColumnCount = 1
         if ( ColumnCount == 1 )
         { 
            //:// The width of the first column is the starting position of the next column, if there is one.
            //:// Otherwise it's the width of the single control plus its X position.
            //:// In either case, it needs to be big enough to handle any padding characters.
            //:SET CURSOR NEXT vReportDef3.XSLT_WorkColumn
            RESULT = SetCursorNextEntity( vReportDef3, "XSLT_WorkColumn", "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:// There is a next column, so use its X position as the first column width.
               //:CurrentWidth = vReportDef3.XSLT_WorkColumn.PSDLG_X
               {MutableInt mi_CurrentWidth = new MutableInt( CurrentWidth );
                               GetIntegerFromAttribute( mi_CurrentWidth, vReportDef3, "XSLT_WorkColumn", "PSDLG_X" );
               CurrentWidth = mi_CurrentWidth.intValue( );}
               //:ELSE
            } 
            else
            { 
               //:// There is no next column, so use computed width from above.
               //:CurrentWidth = vReportDefRoot.XSLT_ColumnWidth.MaximumWidth
               {MutableInt mi_CurrentWidth = new MutableInt( CurrentWidth );
                               GetIntegerFromAttribute( mi_CurrentWidth, vReportDefRoot, "XSLT_ColumnWidth", "MaximumWidth" );
               CurrentWidth = mi_CurrentWidth.intValue( );}
            } 

            //:END
            //:ELSE
         } 
         else
         { 
            //:// The width is the difference between the starting position of this control and the starting position of the next control.
            //:// If there is no next control, the width will be the actual width of the control computed above.
            //:SET CURSOR NEXT vReportDef3.XSLT_WorkColumn
            RESULT = SetCursorNextEntity( vReportDef3, "XSLT_WorkColumn", "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:CurrentWidth  = vReportDef3.XSLT_WorkColumn.PSDLG_X - vReportDefRoot.XSLT_WorkColumn.PSDLG_X
               {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
                               GetIntegerFromAttribute( mi_lTempInteger_3, vReportDef3, "XSLT_WorkColumn", "PSDLG_X" );
               lTempInteger_3 = mi_lTempInteger_3.intValue( );}
               {MutableInt mi_lTempInteger_4 = new MutableInt( lTempInteger_4 );
                               GetIntegerFromAttribute( mi_lTempInteger_4, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
               lTempInteger_4 = mi_lTempInteger_4.intValue( );}
               CurrentWidth = lTempInteger_3 - lTempInteger_4;
               //:ELSE
            } 
            else
            { 
               //:CurrentWidth  = vReportDefRoot.XSLT_ColumnWidth.MaximumWidth
               {MutableInt mi_CurrentWidth = new MutableInt( CurrentWidth );
                               GetIntegerFromAttribute( mi_CurrentWidth, vReportDefRoot, "XSLT_ColumnWidth", "MaximumWidth" );
               CurrentWidth = mi_CurrentWidth.intValue( );}
            } 

            //:END
         } 

         //:END
         //:ConvertPxToInchesCH( vReportDef, szCurrentWidth, CurrentWidth )
         {StringBuilder sb_szCurrentWidth;
         if ( szCurrentWidth == null )
            sb_szCurrentWidth = new StringBuilder( 32 );
         else
            sb_szCurrentWidth = new StringBuilder( szCurrentWidth );
                   oTZRPSRCO_ConvertPxToInchesCH( vReportDef, sb_szCurrentWidth, CurrentWidth );
         szCurrentWidth = sb_szCurrentWidth.toString( );}
         //:DropView( vReportDef3 )
         DropView( vReportDef3 );

         //:szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szCurrentWidth + "in" + QUOTES + "/>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, szCurrentWidth, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
      } 


      //:END

      //:// We also have to build the table-body statement.
      //:szOutputLine = szIndentationChars + "<fo:table-body>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-body>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

      //:// Build table-row and table-cell statements.
      //:RowCount = 0
      RowCount = 0;
      //:FOR EACH vReportDefRoot.XSLT_WorkRow
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:// Build open table-row statement.
         //:szOutputLine = szIndentationChars + "  <fo:table-row>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "  <fo:table-row>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

         //:// Build the table-cell statements.
         //:TextColumnCount = 0
         TextColumnCount = 0;
         //:FOR EACH vReportDefRoot.XSLT_WorkColumn
         RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 

            //:// Cell padding is only used for first column in row.
            //:TextColumnCount = TextColumnCount + 1
            TextColumnCount = TextColumnCount + 1;
            //:IF TextColumnCount = 1
            if ( TextColumnCount == 1 )
            { 
               //:// Determine padding for 1st column.
               //:CellPadding = vReportDefRoot.XSLT_WorkColumn.PSDLG_X
               {MutableInt mi_CellPadding = new MutableInt( CellPadding );
                               GetIntegerFromAttribute( mi_CellPadding, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
               CellPadding = mi_CellPadding.intValue( );}
               //:ConvertPxToInchesCH( vReportDef, szCellPadding, CellPadding )
               {StringBuilder sb_szCellPadding;
               if ( szCellPadding == null )
                  sb_szCellPadding = new StringBuilder( 32 );
               else
                  sb_szCellPadding = new StringBuilder( szCellPadding );
                               oTZRPSRCO_ConvertPxToInchesCH( vReportDef, sb_szCellPadding, CellPadding );
               szCellPadding = sb_szCellPadding.toString( );}
               //:ELSE
            } 
            else
            { 
               //:szCellPadding = "0"
                {StringBuilder sb_szCellPadding;
               if ( szCellPadding == null )
                  sb_szCellPadding = new StringBuilder( 32 );
               else
                  sb_szCellPadding = new StringBuilder( szCellPadding );
                              ZeidonStringCopy( sb_szCellPadding, 1, 0, "0", 1, 0, 51 );
               szCellPadding = sb_szCellPadding.toString( );}
            } 

            //:END

            //:szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES +
            //:               " padding-left=" + QUOTES + szCellPadding + "in" + QUOTES + ">"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, " padding-left=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szCellPadding, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

            //:// Build block and data statements.
            //:XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel )
            oTZRPSRCO_XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel );

            //:// Close table-cell
            //:szOutputLine = szIndentationChars + "    </fo:table-cell>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
            RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         } 


         //:END

         //:// Build close table-row statement.
         //:szOutputLine = szIndentationChars + "  </fo:table-row>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "  </fo:table-row>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
         RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      } 


      //:END

      //:// Build close table-body, table and block statements.
      //:szOutputLine = szIndentationChars + "</fo:table-body>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:table-body>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + "</fo:table>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:table>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + "</fo:block>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END

   //:// Remove the Work Group for this hierarchical level.
   //:DropNameForView( vReportDefRoot, "vReportDefRootSubText", vReportDef, zLEVEL_TASK )
   DropNameForView( vReportDefRoot, "vReportDefRootSubText", vReportDef, zLEVEL_TASK );
   //:DELETE ENTITY vReportDefRoot.XSLT_WorkGroup
   RESULT = DeleteEntity( vReportDefRoot, "XSLT_WorkGroup", zPOS_NEXT );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_BuildBlockData( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                     VIEW vReportDef     BASED ON LOD TZRPSRCO,
//:                     VIEW vLOD           BASED ON LOD TZZOLODO,
//:                     INTEGER lFileHandle,
//:                     STRING ( 5000 ) szOutputLine,
//:                     STRING ( 50 )  szIndentationChars,
//:                     INTEGER GroupHierarchicalLevel )

//:   STRING ( 20 )  szPaddingTop
public int 
oTZRPSRCO_XSLT_BuildBlockData( View     vReportDefRoot,
                               View     vReportDef,
                               View     vLOD,
                               int      lFileHandle,
                               String   szOutputLine,
                               String   szIndentationChars,
                               int      GroupHierarchicalLevel )
{
   String   szPaddingTop = null;
   //:STRING ( 32 )  szAttributeMappingName
   String   szAttributeMappingName = null;
   //:STRING ( 256 ) szConstantArea
   String   szConstantArea = null;
   //:INTEGER        PaddingTop
   int      PaddingTop = 0;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   String   szTempString_0 = null;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;


   //:// Block and data statement is different depending on whether the ControlDef is Text or Graphic and whether or not there is mapping for the Control.
   //:SET CURSOR FIRST vReportDef.Control WHERE vReportDef.Control.ZKey = vReportDefRoot.XSLT_WorkColumn.ControlZKey
   {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
       GetIntegerFromAttribute( mi_lTempInteger_0, vReportDefRoot, "XSLT_WorkColumn", "ControlZKey" );
   lTempInteger_0 = mi_lTempInteger_0.intValue( );}
   RESULT = SetCursorFirstEntityByInteger( vReportDef, "Control", "ZKey", lTempInteger_0, "" );
   //:IF vReportDef.ControlDef.Tag = "Rpt_PicGraphic"
   if ( CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_PicGraphic" ) == 0 )
   { 
      //:// Control is graphic.
      //:szConstantArea = vReportDef.Control.Text
      {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
      StringBuilder sb_szConstantArea;
      if ( szConstantArea == null )
         sb_szConstantArea = new StringBuilder( 32 );
      else
         sb_szConstantArea = new StringBuilder( szConstantArea );
             GetVariableFromAttribute( sb_szConstantArea, mi_lTempInteger_1, 'S', 257, vReportDef, "Control", "Text", "", 0 );
      lTempInteger_1 = mi_lTempInteger_1.intValue( );
      szConstantArea = sb_szConstantArea.toString( );}
      //:PaddingTop = vReportDef.Control.PSDLG_Y
      {MutableInt mi_PaddingTop = new MutableInt( PaddingTop );
             GetIntegerFromAttribute( mi_PaddingTop, vReportDef, "Control", "PSDLG_Y" );
      PaddingTop = mi_PaddingTop.intValue( );}
      //:ConvertPxToInchesCH( vReportDef, szPaddingTop, PaddingTop )
      {StringBuilder sb_szPaddingTop;
      if ( szPaddingTop == null )
         sb_szPaddingTop = new StringBuilder( 32 );
      else
         sb_szPaddingTop = new StringBuilder( szPaddingTop );
             oTZRPSRCO_ConvertPxToInchesCH( vReportDef, sb_szPaddingTop, PaddingTop );
      szPaddingTop = sb_szPaddingTop.toString( );}
      //:szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + ">"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:block border=", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "normal solid black", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + "  <fo:external-graphic src=" + QUOTES + szConstantArea + QUOTES +
      //:                                    " padding-top=" + QUOTES + szPaddingTop + "in" + QUOTES + "/>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "  <fo:external-graphic src=", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szConstantArea, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, " padding-top=", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szPaddingTop, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + "</fo:block>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:ELSE
   } 
   else
   { 
      //:// Control is considered Text.
      //:IF vReportDef.CtrlMapER_Attribute EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( vReportDef, "CtrlMapER_Attribute" );
      if ( lTempInteger_2 == 0 )
      { 
         //:szAttributeMappingName = vReportDef.CtrlMapRelatedEntity.Name + "." + vReportDef.CtrlMapER_Attribute.Name
         {StringBuilder sb_szAttributeMappingName;
         if ( szAttributeMappingName == null )
            sb_szAttributeMappingName = new StringBuilder( 32 );
         else
            sb_szAttributeMappingName = new StringBuilder( szAttributeMappingName );
                   GetStringFromAttribute( sb_szAttributeMappingName, vReportDef, "CtrlMapRelatedEntity", "Name" );
         szAttributeMappingName = sb_szAttributeMappingName.toString( );}
          {StringBuilder sb_szAttributeMappingName;
         if ( szAttributeMappingName == null )
            sb_szAttributeMappingName = new StringBuilder( 32 );
         else
            sb_szAttributeMappingName = new StringBuilder( szAttributeMappingName );
                  ZeidonStringConcat( sb_szAttributeMappingName, 1, 0, ".", 1, 0, 33 );
         szAttributeMappingName = sb_szAttributeMappingName.toString( );}
         {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
         StringBuilder sb_szTempString_0;
         if ( szTempString_0 == null )
            sb_szTempString_0 = new StringBuilder( 32 );
         else
            sb_szTempString_0 = new StringBuilder( szTempString_0 );
                   GetVariableFromAttribute( sb_szTempString_0, mi_lTempInteger_3, 'S', 33, vReportDef, "CtrlMapER_Attribute", "Name", "", 0 );
         lTempInteger_3 = mi_lTempInteger_3.intValue( );
         szTempString_0 = sb_szTempString_0.toString( );}
          {StringBuilder sb_szAttributeMappingName;
         if ( szAttributeMappingName == null )
            sb_szAttributeMappingName = new StringBuilder( 32 );
         else
            sb_szAttributeMappingName = new StringBuilder( szAttributeMappingName );
                  ZeidonStringConcat( sb_szAttributeMappingName, 1, 0, szTempString_0, 1, 0, 33 );
         szAttributeMappingName = sb_szAttributeMappingName.toString( );}
         //:szOutputLine = szIndentationChars + "      <fo:block ><xsl:value-of select=" + QUOTES +
         //:               szAttributeMappingName + QUOTES + "/></fo:block>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "      <fo:block ><xsl:value-of select=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, szAttributeMappingName, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "/></fo:block>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
         //:ELSE
      } 
      else
      { 
         //:szConstantArea = vReportDef.Control.Text
         {MutableInt mi_lTempInteger_4 = new MutableInt( lTempInteger_4 );
         StringBuilder sb_szConstantArea;
         if ( szConstantArea == null )
            sb_szConstantArea = new StringBuilder( 32 );
         else
            sb_szConstantArea = new StringBuilder( szConstantArea );
                   GetVariableFromAttribute( sb_szConstantArea, mi_lTempInteger_4, 'S', 257, vReportDef, "Control", "Text", "", 0 );
         lTempInteger_4 = mi_lTempInteger_4.intValue( );
         szConstantArea = sb_szConstantArea.toString( );}
         //:szOutputLine = szIndentationChars + "      <fo:block>" + szConstantArea + "</fo:block>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "      <fo:block>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, szConstantArea, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_BuildTable( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                 VIEW vReportDef     BASED ON LOD TZRPSRCO,
//:                 VIEW vLOD           BASED ON LOD TZZOLODO,
//:                 INTEGER lFileHandle,
//:                 STRING ( 5000 ) szOutputLine,
//:                 STRING ( 50 )  szIndentationChars,
//:                 INTEGER GroupHierarchicalLevel,
//:                 INTEGER TableMarginTop,
//:                 INTEGER TableWidth )

//:   STRING ( 20 )  szTableWidth
public int 
oTZRPSRCO_XSLT_BuildTable( View     vReportDefRoot,
                           View     vReportDef,
                           View     vLOD,
                           int      lFileHandle,
                           String   szOutputLine,
                           String   szIndentationChars,
                           int      GroupHierarchicalLevel,
                           int      TableMarginTop,
                           int      TableWidth )
{
   String   szTableWidth = null;
   //:STRING ( 20 )  szTableMarginTop
   String   szTableMarginTop = null;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;


   //:// Build block and table statement.
   //:SET CURSOR FIRST vReportDef.Control WHERE vReportDef.Control.ZKey = vReportDefRoot.XSLT_WorkColumn.ControlZKey
   {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
       GetIntegerFromAttribute( mi_lTempInteger_0, vReportDefRoot, "XSLT_WorkColumn", "ControlZKey" );
   lTempInteger_0 = mi_lTempInteger_0.intValue( );}
   RESULT = SetCursorFirstEntityByInteger( vReportDef, "Control", "ZKey", lTempInteger_0, "" );

   //:ConvertPxToInchesCH( vReportDefRoot, szTableMarginTop, TableMarginTop )
   {StringBuilder sb_szTableMarginTop;
   if ( szTableMarginTop == null )
      sb_szTableMarginTop = new StringBuilder( 32 );
   else
      sb_szTableMarginTop = new StringBuilder( szTableMarginTop );
       oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, sb_szTableMarginTop, TableMarginTop );
   szTableMarginTop = sb_szTableMarginTop.toString( );}
   //:ConvertPxToInchesCH( vReportDefRoot, szTableWidth, TableWidth )
   {StringBuilder sb_szTableWidth;
   if ( szTableWidth == null )
      sb_szTableWidth = new StringBuilder( 32 );
   else
      sb_szTableWidth = new StringBuilder( szTableWidth );
       oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, sb_szTableWidth, TableWidth );
   szTableWidth = sb_szTableWidth.toString( );}

   //:// Table Block - Initially, we'll make the table block visible.
   //:szOutputLine = szIndentationChars + ""
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:block border=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "normal solid black", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:// Table statement
   //:szOutputLine = szIndentationChars + "<fo:table table-layout=" + QUOTES + "fixed" +
   //:               QUOTES + " width=" + QUOTES + szTableWidth + "in" +
   //:               QUOTES + " margin-top=" + QUOTES + szTableMarginTop +  "in" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table table-layout=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "fixed", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " width=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szTableWidth, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szTableMarginTop, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_AllTextGroup( VIEW vReportDef     BASED ON LOD TZRPSRCO,
//:                   VIEW vLOD           BASED ON LOD TZZOLODO,
//:                   INTEGER lFileHandle,
//:                   STRING ( 5000 ) szOutputLine,
//:                   STRING ( 50 )   szIndentationChars )

//:   VIEW vReportDef3 BASED ON LOD TZRPSRCO
public int 
oTZRPSRCO_XSLT_AllTextGroup( View     vReportDef,
                             View     vLOD,
                             int      lFileHandle,
                             String   szOutputLine,
                             String   szIndentationChars )
{
   zVIEW    vReportDef3 = new zVIEW( );
   //:STRING ( 20 )  szCurrentWidth
   String   szCurrentWidth = null;
   //:STRING ( 20 )  szPageWidth
   String   szPageWidth = null;
   //:STRING ( 30 )  szCellPaddingName
   String   szCellPaddingName = null;
   //:STRING ( 50 )  szAttributeMappingName
   String   szAttributeMappingName = null;
   //:STRING ( 256 ) szConstantArea
   String   szConstantArea = null;
   //:STRING ( 50 )  szCellPadding
   String   szCellPadding = null;
   //:STRING ( 20 )  szTableMarginTop
   String   szTableMarginTop = null;
   //:STRING ( 20 )  szTablePaddingLeft
   String   szTablePaddingLeft = null;
   //:STRING ( 20 )  szTableWidth
   String   szTableWidth = null;
   //:INTEGER       CurrentWidth
   int      CurrentWidth = 0;
   //:INTEGER       CurrentPositionY
   int      CurrentPositionY = 0;
   //:INTEGER       LastPositionY
   int      LastPositionY = 0;
   //:INTEGER       FirstColumnPosition
   int      FirstColumnPosition = 0;
   //:INTEGER       TopGroupPosition
   int      TopGroupPosition = 0;
   //:INTEGER       ColumnCount
   int      ColumnCount = 0;
   //:INTEGER       TextColumnCount
   int      TextColumnCount = 0;
   //:INTEGER       RowCount
   int      RowCount = 0;
   //:INTEGER       TablePaddingLeft
   int      TablePaddingLeft = 0;
   //:INTEGER       TableMarginTop
   int      TableMarginTop = 0;
   //:INTEGER       TableWidth
   int      TableWidth = 0;
   //:INTEGER       LastRowPosition
   int      LastRowPosition = 0;
   //:INTEGER       LastRowHeight
   int      LastRowHeight = 0;
   int      RESULT = 0;


   //:// Process a Group that is only made up of text Controls at the first level, as opposed to text controls that are subcontrols
   //:// under a parent Group or Shape.
   //:// All Rpt_Text controls are analyzed and organized by rows, with each row being handled as a single row table.

   //:// We assume the controls are text.

   //:szCellPaddingName = "DefaultCellPadding"
    {StringBuilder sb_szCellPaddingName;
   if ( szCellPaddingName == null )
      sb_szCellPaddingName = new StringBuilder( 32 );
   else
      sb_szCellPaddingName = new StringBuilder( szCellPaddingName );
      ZeidonStringCopy( sb_szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 31 );
   szCellPaddingName = sb_szCellPaddingName.toString( );}
   //:NAME VIEW vReportDef "vReportDefAllText"
   SetNameForView( vReportDef, "vReportDefAllText", null, zLEVEL_TASK );

   //:// Build work subobject, which organizes controls in rows and columns.
   //:LastPositionY = -1
   LastPositionY = -1;
   //:FOR EACH vReportDef.Control
   RESULT = SetCursorFirstEntity( vReportDef, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vReportDef.ControlDef.Tag = "Rpt_Text"
      if ( CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_Text" ) == 0 )
      { 
         //:CurrentPositionY = vReportDef.Control.PSDLG_Y
         {MutableInt mi_CurrentPositionY = new MutableInt( CurrentPositionY );
                   GetIntegerFromAttribute( mi_CurrentPositionY, vReportDef, "Control", "PSDLG_Y" );
         CurrentPositionY = mi_CurrentPositionY.intValue( );}
         //:IF CurrentPositionY != LastPositionY
         if ( CurrentPositionY != LastPositionY )
         { 
            //:CREATE ENTITY vReportDef.XSLT_WorkRow
            RESULT = CreateEntity( vReportDef, "XSLT_WorkRow", zPOS_AFTER );
            //:vReportDef.XSLT_WorkRow.PSDLG_Y = vReportDef.Control.PSDLG_Y
            SetAttributeFromAttribute( vReportDef, "XSLT_WorkRow", "PSDLG_Y", vReportDef, "Control", "PSDLG_Y" );
         } 

         //:END
         //:LastPositionY = CurrentPositionY
         LastPositionY = CurrentPositionY;
         //:CREATE ENTITY vReportDef.XSLT_WorkColumn
         RESULT = CreateEntity( vReportDef, "XSLT_WorkColumn", zPOS_AFTER );
         //:vReportDef.XSLT_WorkColumn.ControlZKey = vReportDef.Control.ZKey
         SetAttributeFromAttribute( vReportDef, "XSLT_WorkColumn", "ControlZKey", vReportDef, "Control", "ZKey" );
         //:vReportDef.XSLT_WorkColumn.ControlType = vReportDef.ControlDef.Tag
         SetAttributeFromAttribute( vReportDef, "XSLT_WorkColumn", "ControlType", vReportDef, "ControlDef", "Tag" );
         //:SetMatchingAttributesByName( vReportDef, "XSLT_WorkColumn",  vReportDef, "Control", zSET_NULL )
         SetMatchingAttributesByName( vReportDef, "XSLT_WorkColumn", vReportDef, "Control", zSET_NULL );
      } 

      RESULT = SetCursorNextEntity( vReportDef, "Control", "" );
      //:END
   } 

   //:END

   //:// Process Row/Column Structure to generate Tables and Columns.

   //:TopGroupPosition = vReportDef.XSLT_WorkColumn.PSDLG_Y
   {MutableInt mi_TopGroupPosition = new MutableInt( TopGroupPosition );
       GetIntegerFromAttribute( mi_TopGroupPosition, vReportDef, "XSLT_WorkColumn", "PSDLG_Y" );
   TopGroupPosition = mi_TopGroupPosition.intValue( );}
   //:RowCount = 0
   RowCount = 0;

   //:FOR EACH vReportDef.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDef, "XSLT_WorkRow", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Build Row & Cell statements. Each work row becomes a single table with a single row.

      //:RowCount = RowCount + 1
      RowCount = RowCount + 1;

      //:// Go to build the table.
      //:XSLT_GroupTable( vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, RowCount, "" )
      oTZRPSRCO_XSLT_GroupTable( vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, RowCount, "" );
      RESULT = SetCursorNextEntity( vReportDef, "XSLT_WorkRow", "" );
   } 


   //:END

   //:// Remove the Work Rows.
   //:FOR EACH vReportDef.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDef, "XSLT_WorkRow", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY vReportDef.XSLT_WorkRow NONE
      RESULT = DeleteEntity( vReportDef, "XSLT_WorkRow", zREPOS_NONE );
      RESULT = SetCursorNextEntity( vReportDef, "XSLT_WorkRow", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_GroupTable( VIEW vReportDef     BASED ON LOD TZRPSRCO,
//:                 VIEW vLOD           BASED ON LOD TZZOLODO,
//:                 INTEGER lFileHandle,
//:                 STRING ( 5000 ) szOutputLine,
//:                 STRING ( 50 )   szIndentationChars,
//:                 INTEGER         RowCount,
//:                 STRING ( 32 )   szForEachName )

//:   VIEW vReportDef3 BASED ON LOD TZRPSRCO
public int 
oTZRPSRCO_XSLT_GroupTable( View     vReportDef,
                           View     vLOD,
                           int      lFileHandle,
                           String   szOutputLine,
                           String   szIndentationChars,
                           int      RowCount,
                           String   szForEachName )
{
   zVIEW    vReportDef3 = new zVIEW( );
   //:STRING ( 20 )  szCurrentWidth
   String   szCurrentWidth = null;
   //:STRING ( 20 )  szPageWidth
   String   szPageWidth = null;
   //:STRING ( 30 )  szCellPaddingName
   String   szCellPaddingName = null;
   //:STRING ( 50 )  szAttributeMappingName
   String   szAttributeMappingName = null;
   //:STRING ( 256 ) szConstantArea
   String   szConstantArea = null;
   //:STRING ( 50 )  szCellPadding
   String   szCellPadding = null;
   //:STRING ( 20 )  szTableMarginTop
   String   szTableMarginTop = null;
   //:STRING ( 20 )  szTablePaddingLeft
   String   szTablePaddingLeft = null;
   //:STRING ( 20 )  szTableWidth
   String   szTableWidth = null;
   //:INTEGER       CurrentWidth
   int      CurrentWidth = 0;
   //:INTEGER       CurrentPositionY
   int      CurrentPositionY = 0;
   //:INTEGER       LastPositionY
   int      LastPositionY = 0;
   //:INTEGER       FirstColumnPosition
   int      FirstColumnPosition = 0;
   //:INTEGER       TopGroupPosition
   int      TopGroupPosition = 0;
   //:INTEGER       TextColumnCount
   int      TextColumnCount = 0;
   //:INTEGER       TablePaddingLeft
   int      TablePaddingLeft = 0;
   //:INTEGER       TableMarginTop
   int      TableMarginTop = 0;
   //:INTEGER       TableWidth
   int      TableWidth = 0;
   //:INTEGER       LastRowPosition
   int      LastRowPosition = 0;
   //:INTEGER       LastRowHeight
   int      LastRowHeight = 0;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;
   int      lTempInteger_5 = 0;
   int      lTempInteger_6 = 0;
   String   szTempString_0 = null;
   int      lTempInteger_7 = 0;
   int      lTempInteger_8 = 0;


   //:// Build Table

   //:// Build open table and column statements.
   //:// Open a new table.

   //:szCellPaddingName = "DefaultCellPadding"
    {StringBuilder sb_szCellPaddingName;
   if ( szCellPaddingName == null )
      sb_szCellPaddingName = new StringBuilder( 32 );
   else
      sb_szCellPaddingName = new StringBuilder( szCellPaddingName );
      ZeidonStringCopy( sb_szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 31 );
   szCellPaddingName = sb_szCellPaddingName.toString( );}

   //:// Compute the following values:

   //:// The value of Margin Top depends on whether this is the first row or any other row.
   //:// 1. The first row is simply the Y Position.
   //:// 2. Any later row is the Y Position minus the previous Row position and Row height (which is Y size of first control).
   //:IF RowCount = 1
   if ( RowCount == 1 )
   { 
      //:TableMarginTop  = vReportDef.XSLT_WorkRow.PSDLG_Y
      {MutableInt mi_TableMarginTop = new MutableInt( TableMarginTop );
             GetIntegerFromAttribute( mi_TableMarginTop, vReportDef, "XSLT_WorkRow", "PSDLG_Y" );
      TableMarginTop = mi_TableMarginTop.intValue( );}
      //:ELSE
   } 
   else
   { 
      //:CreateViewFromView( vReportDef3, vReportDef )
      CreateViewFromView( vReportDef3, vReportDef );
      //:SET CURSOR PREVIOUS vReportDef3.XSLT_WorkRow
      RESULT = SetCursorPrevEntity( vReportDef3, "XSLT_WorkRow", "" );
      //:LastRowPosition = vReportDef3.XSLT_WorkRow.PSDLG_Y
      {MutableInt mi_LastRowPosition = new MutableInt( LastRowPosition );
             GetIntegerFromAttribute( mi_LastRowPosition, vReportDef3, "XSLT_WorkRow", "PSDLG_Y" );
      LastRowPosition = mi_LastRowPosition.intValue( );}
      //:LastRowHeight   = vReportDef3.XSLT_WorkColumn.SZDLG_Y
      {MutableInt mi_LastRowHeight = new MutableInt( LastRowHeight );
             GetIntegerFromAttribute( mi_LastRowHeight, vReportDef3, "XSLT_WorkColumn", "SZDLG_Y" );
      LastRowHeight = mi_LastRowHeight.intValue( );}
      //:TableMarginTop  = vReportDef.XSLT_WorkRow.PSDLG_Y - LastRowPosition - LastRowHeight
      {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
             GetIntegerFromAttribute( mi_lTempInteger_0, vReportDef, "XSLT_WorkRow", "PSDLG_Y" );
      lTempInteger_0 = mi_lTempInteger_0.intValue( );}
      TableMarginTop = lTempInteger_0 - LastRowPosition - LastRowHeight;
      //:IF TableMarginTop < 0
      if ( TableMarginTop < 0 )
      { 
         //:TableMarginTop = 0
         TableMarginTop = 0;
      } 

      //:END
      //:DropView( vReportDef3 )
      DropView( vReportDef3 );
   } 

   //:END
   //:ConvertPxToInchesCH( vReportDef, szTableMarginTop, TableMarginTop )
   {StringBuilder sb_szTableMarginTop;
   if ( szTableMarginTop == null )
      sb_szTableMarginTop = new StringBuilder( 32 );
   else
      sb_szTableMarginTop = new StringBuilder( szTableMarginTop );
       oTZRPSRCO_ConvertPxToInchesCH( vReportDef, sb_szTableMarginTop, TableMarginTop );
   szTableMarginTop = sb_szTableMarginTop.toString( );}

   //:// Compute Padding Left for first column of the row.
   //:// We'll assume 100 is normal indentation and anything after that must shift the columns right.
   //:SET CURSOR FIRST vReportDef.XSLT_WorkColumn
   RESULT = SetCursorFirstEntity( vReportDef, "XSLT_WorkColumn", "" );
   //:FirstColumnPosition = vReportDef.XSLT_WorkColumn.PSDLG_X
   {MutableInt mi_FirstColumnPosition = new MutableInt( FirstColumnPosition );
       GetIntegerFromAttribute( mi_FirstColumnPosition, vReportDef, "XSLT_WorkColumn", "PSDLG_X" );
   FirstColumnPosition = mi_FirstColumnPosition.intValue( );}
   //:TablePaddingLeft = FirstColumnPosition - 100
   TablePaddingLeft = FirstColumnPosition - 100;
   //:IF TablePaddingLeft < 0
   if ( TablePaddingLeft < 0 )
   { 
      //:TablePaddingLeft = 0
      TablePaddingLeft = 0;
   } 

   //:END
   //:ConvertPxToInchesCH( vReportDef, szTablePaddingLeft, TablePaddingLeft )
   {StringBuilder sb_szTablePaddingLeft;
   if ( szTablePaddingLeft == null )
      sb_szTablePaddingLeft = new StringBuilder( 32 );
   else
      sb_szTablePaddingLeft = new StringBuilder( szTablePaddingLeft );
       oTZRPSRCO_ConvertPxToInchesCH( vReportDef, sb_szTablePaddingLeft, TablePaddingLeft );
   szTablePaddingLeft = sb_szTablePaddingLeft.toString( );}

   //:// Table Width
   //:SET CURSOR LAST vReportDef.XSLT_WorkColumn
   RESULT = SetCursorLastEntity( vReportDef, "XSLT_WorkColumn", "" );
   //:TableWidth = vReportDef.XSLT_WorkColumn.PSDLG_X + vReportDef.XSLT_WorkColumn.SZDLG_X
   {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
       GetIntegerFromAttribute( mi_lTempInteger_1, vReportDef, "XSLT_WorkColumn", "PSDLG_X" );
   lTempInteger_1 = mi_lTempInteger_1.intValue( );}
   {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
       GetIntegerFromAttribute( mi_lTempInteger_2, vReportDef, "XSLT_WorkColumn", "SZDLG_X" );
   lTempInteger_2 = mi_lTempInteger_2.intValue( );}
   TableWidth = lTempInteger_1 + lTempInteger_2;
   //:ConvertPxToInchesCH( vReportDef, szTableWidth, TableWidth )
   {StringBuilder sb_szTableWidth;
   if ( szTableWidth == null )
      sb_szTableWidth = new StringBuilder( 32 );
   else
      sb_szTableWidth = new StringBuilder( szTableWidth );
       oTZRPSRCO_ConvertPxToInchesCH( vReportDef, sb_szTableWidth, TableWidth );
   szTableWidth = sb_szTableWidth.toString( );}

   //:// Table Block - Initially, we'll make the table block visible.
   //:szOutputLine = szIndentationChars + ""
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:block border=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "normal solid black", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Table statement
   //:szOutputLine = szIndentationChars + "<fo:table table-layout=" + QUOTES + "fixed" +
   //:               QUOTES + " width=" + QUOTES + szTableWidth + "in" +
   //:               QUOTES + " margin-top=" + QUOTES + szTableMarginTop +  "in" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table table-layout=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "fixed", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " width=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szTableWidth, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szTableMarginTop, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Column Statements
   //:// Generate Padding Column, if TablePaddingLeft is > 0.
   //:IF TablePaddingLeft > 0
   if ( TablePaddingLeft > 0 )
   { 
      //:szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szTablePaddingLeft + "in" + QUOTES + "/>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szTablePaddingLeft, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END

   //:// Generate Column Statement for each work Column entry.
   //:FOR EACH vReportDef.XSLT_WorkColumn
   RESULT = SetCursorFirstEntity( vReportDef, "XSLT_WorkColumn", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:// The width is the difference between the starting position of this control and the starting position of the next control.
      //:// If there is no next control, the width will be the actual width of the control.
      //:CreateViewFromView( vReportDef3, vReportDef )
      CreateViewFromView( vReportDef3, vReportDef );
      //:SET CURSOR NEXT vReportDef3.XSLT_WorkColumn
      RESULT = SetCursorNextEntity( vReportDef3, "XSLT_WorkColumn", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:CurrentWidth  = vReportDef3.XSLT_WorkColumn.PSDLG_X - vReportDef.XSLT_WorkColumn.PSDLG_X
         {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
                   GetIntegerFromAttribute( mi_lTempInteger_3, vReportDef3, "XSLT_WorkColumn", "PSDLG_X" );
         lTempInteger_3 = mi_lTempInteger_3.intValue( );}
         {MutableInt mi_lTempInteger_4 = new MutableInt( lTempInteger_4 );
                   GetIntegerFromAttribute( mi_lTempInteger_4, vReportDef, "XSLT_WorkColumn", "PSDLG_X" );
         lTempInteger_4 = mi_lTempInteger_4.intValue( );}
         CurrentWidth = lTempInteger_3 - lTempInteger_4;
         //:ELSE
      } 
      else
      { 
         //:CurrentWidth  = vReportDef.XSLT_WorkColumn.SZDLG_X
         {MutableInt mi_CurrentWidth = new MutableInt( CurrentWidth );
                   GetIntegerFromAttribute( mi_CurrentWidth, vReportDef, "XSLT_WorkColumn", "SZDLG_X" );
         CurrentWidth = mi_CurrentWidth.intValue( );}
      } 

      //:END
      //:DropView( vReportDef3 )
      DropView( vReportDef3 );
      //:ConvertPxToInchesCH( vReportDef, szCurrentWidth, CurrentWidth )
      {StringBuilder sb_szCurrentWidth;
      if ( szCurrentWidth == null )
         sb_szCurrentWidth = new StringBuilder( 32 );
      else
         sb_szCurrentWidth = new StringBuilder( szCurrentWidth );
             oTZRPSRCO_ConvertPxToInchesCH( vReportDef, sb_szCurrentWidth, CurrentWidth );
      szCurrentWidth = sb_szCurrentWidth.toString( );}
      //:szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szCurrentWidth + "in" + QUOTES + "/>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szCurrentWidth, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      RESULT = SetCursorNextEntity( vReportDef, "XSLT_WorkColumn", "" );
   } 

   //:END

   //:// Generate table-body statement, table-row and block.
   //:szOutputLine = szIndentationChars + "<fo:table-body>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-body>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// If this is for a repeating Entity (based on For-Each flag, create for-each statement.
   //:IF szForEachName != ""
   if ( ZeidonStringCompare( szForEachName, 1, 0, "", 1, 0, 33 ) != 0 )
   { 
      //:szOutputLine = szIndentationChars + "<xsl:for-each select=" + QUOTES + szForEachName + QUOTES + ">"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "<xsl:for-each select=", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szForEachName, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END

   //:szOutputLine = szIndentationChars + "  <fo:table-row>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "  <fo:table-row>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Generate the table-cell statements, which will contain either the text from the control or the attribute mapping data.

   //:// Generate Padding Cell, if TablePaddingLeft is > 0.
   //:IF TablePaddingLeft > 0
   if ( TablePaddingLeft > 0 )
   { 
      //:szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES + ">"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

      //:szOutputLine = szIndentationChars + "      <fo:block></fo:block>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "      <fo:block></fo:block>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

      //:szOutputLine = szIndentationChars + "    </fo:table-cell>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END

   //:TextColumnCount = 0
   TextColumnCount = 0;
   //:FOR EACH vReportDef.XSLT_WorkColumn
   RESULT = SetCursorFirstEntity( vReportDef, "XSLT_WorkColumn", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES + ">"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

      //:// Block statement is different depending on whether or not there is mapping for the Control.
      //:// Also, any Control 1) without mapping, 2) with null Text value and 3) with SZDLG_Y <= 21.
      //:SET CURSOR FIRST vReportDef.Control WHERE vReportDef.Control.ZKey = vReportDef.XSLT_WorkColumn.ControlZKey
      {MutableInt mi_lTempInteger_5 = new MutableInt( lTempInteger_5 );
             GetIntegerFromAttribute( mi_lTempInteger_5, vReportDef, "XSLT_WorkColumn", "ControlZKey" );
      lTempInteger_5 = mi_lTempInteger_5.intValue( );}
      RESULT = SetCursorFirstEntityByInteger( vReportDef, "Control", "ZKey", lTempInteger_5, "" );
      //:IF vReportDef.CtrlMapER_Attribute EXISTS
      lTempInteger_6 = CheckExistenceOfEntity( vReportDef, "CtrlMapER_Attribute" );
      if ( lTempInteger_6 == 0 )
      { 
         //:szAttributeMappingName = vReportDef.CtrlMapRelatedEntity.Name + "." + vReportDef.CtrlMapER_Attribute.Name
         {StringBuilder sb_szAttributeMappingName;
         if ( szAttributeMappingName == null )
            sb_szAttributeMappingName = new StringBuilder( 32 );
         else
            sb_szAttributeMappingName = new StringBuilder( szAttributeMappingName );
                   GetStringFromAttribute( sb_szAttributeMappingName, vReportDef, "CtrlMapRelatedEntity", "Name" );
         szAttributeMappingName = sb_szAttributeMappingName.toString( );}
          {StringBuilder sb_szAttributeMappingName;
         if ( szAttributeMappingName == null )
            sb_szAttributeMappingName = new StringBuilder( 32 );
         else
            sb_szAttributeMappingName = new StringBuilder( szAttributeMappingName );
                  ZeidonStringConcat( sb_szAttributeMappingName, 1, 0, ".", 1, 0, 51 );
         szAttributeMappingName = sb_szAttributeMappingName.toString( );}
         {MutableInt mi_lTempInteger_7 = new MutableInt( lTempInteger_7 );
         StringBuilder sb_szTempString_0;
         if ( szTempString_0 == null )
            sb_szTempString_0 = new StringBuilder( 32 );
         else
            sb_szTempString_0 = new StringBuilder( szTempString_0 );
                   GetVariableFromAttribute( sb_szTempString_0, mi_lTempInteger_7, 'S', 33, vReportDef, "CtrlMapER_Attribute", "Name", "", 0 );
         lTempInteger_7 = mi_lTempInteger_7.intValue( );
         szTempString_0 = sb_szTempString_0.toString( );}
          {StringBuilder sb_szAttributeMappingName;
         if ( szAttributeMappingName == null )
            sb_szAttributeMappingName = new StringBuilder( 32 );
         else
            sb_szAttributeMappingName = new StringBuilder( szAttributeMappingName );
                  ZeidonStringConcat( sb_szAttributeMappingName, 1, 0, szTempString_0, 1, 0, 51 );
         szAttributeMappingName = sb_szAttributeMappingName.toString( );}
         //:szOutputLine = szIndentationChars + "      <fo:block ><xsl:value-of select=" + QUOTES +
         //:               szAttributeMappingName + QUOTES + "/></fo:block>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "      <fo:block ><xsl:value-of select=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, szAttributeMappingName, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "/></fo:block>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
         //:ELSE
      } 
      else
      { 
         //:IF vReportDef.Control.Text = "" AND vReportDef.Control.SZDLG_Y <= 21
         if ( CompareAttributeToString( vReportDef, "Control", "Text", "" ) == 0 && CompareAttributeToInteger( vReportDef, "Control", "SZDLG_Y", 21 ) <= 0 )
         { 
            //:// Build a Leader line.
            //:szOutputLine = szIndentationChars + "      <fo:block>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "      <fo:block>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

            //:szOutputLine = szIndentationChars + "      <fo:leader leader-pattern=" + QUOTES + "rule" + QUOTES +
            //:               " leader-length.optimum=" + QUOTES + "100%" + QUOTES + " rule-style=" + QUOTES +
            //:               "solid" + QUOTES + " rule-thickness=" + QUOTES + "1pt" + QUOTES + "/>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "      <fo:leader leader-pattern=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "rule", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, " leader-length.optimum=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "100%", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, " rule-style=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "solid", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, " rule-thickness=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "1pt", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

            //:szOutputLine = szIndentationChars + "      </fo:block>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "      </fo:block>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

            //:ELSE
         } 
         else
         { 
            //:szConstantArea = vReportDef.Control.Text
            {MutableInt mi_lTempInteger_8 = new MutableInt( lTempInteger_8 );
            StringBuilder sb_szConstantArea;
            if ( szConstantArea == null )
               sb_szConstantArea = new StringBuilder( 32 );
            else
               sb_szConstantArea = new StringBuilder( szConstantArea );
                         GetVariableFromAttribute( sb_szConstantArea, mi_lTempInteger_8, 'S', 257, vReportDef, "Control", "Text", "", 0 );
            lTempInteger_8 = mi_lTempInteger_8.intValue( );
            szConstantArea = sb_szConstantArea.toString( );}
            //:szOutputLine = szIndentationChars + "      <fo:block>" + szConstantArea + "</fo:block>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "      <fo:block>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szConstantArea, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
         } 

         //:END
      } 

      //:END

      //:// Close table-cell
      //:szOutputLine = szIndentationChars + "    </fo:table-cell>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      RESULT = SetCursorNextEntity( vReportDef, "XSLT_WorkColumn", "" );
   } 


   //:END

   //:// Build close table statements
   //:szOutputLine = szIndentationChars + "  </fo:table-row>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "  </fo:table-row>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:IF szForEachName != ""
   if ( ZeidonStringCompare( szForEachName, 1, 0, "", 1, 0, 33 ) != 0 )
   { 
      //:szOutputLine = szIndentationChars + "  </xsl:for-each>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "  </xsl:for-each>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END

   //:szOutputLine = szIndentationChars + "</fo:table-body>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:table-body>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + "</fo:table>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:table>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + "</fo:block>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_GroupRecur( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                 VIEW vReportDef     BASED ON LOD TZRPSRCO,
//:                 VIEW vLOD           BASED ON LOD TZZOLODO,
//:                 INTEGER lFileHandle,
//:                 STRING ( 5000 ) szOutputLine,
//:                 STRING ( 50 )   szIndentationChars,
//:                 INTEGER GroupHierarchicalLevel )

//:   VIEW vReportDefRoot2 BASED ON LOD TZRPSRCO
public int 
oTZRPSRCO_XSLT_GroupRecur( View     vReportDefRoot,
                           View     vReportDef,
                           View     vLOD,
                           int      lFileHandle,
                           String   szOutputLine,
                           String   szIndentationChars,
                           int      GroupHierarchicalLevel )
{
   zVIEW    vReportDefRoot2 = new zVIEW( );
   //:VIEW vReportDef3     BASED ON LOD TZRPSRCO
   zVIEW    vReportDef3 = new zVIEW( );
   //:STRING ( 20 )  szCurrentWidth
   String   szCurrentWidth = null;
   //:STRING ( 20 )  szPageWidth
   String   szPageWidth = null;
   //:STRING ( 30 )  szCellPaddingName
   String   szCellPaddingName = null;
   //:STRING ( 50 )  szAttributeMappingName
   String   szAttributeMappingName = null;
   //:STRING ( 50 )  szReportDefName
   String   szReportDefName = null;
   //:STRING ( 50 )  szSubIndentationChars
   String   szSubIndentationChars = null;
   //:STRING ( 50 )  szCellPadding
   String   szCellPadding = null;
   //:STRING ( 20 )  szTableMarginTop
   String   szTableMarginTop = null;
   //:STRING ( 20 )  szTablePaddingLeft
   String   szTablePaddingLeft = null;
   //:STRING ( 20 )  szTableWidth
   String   szTableWidth = null;
   //:STRING ( 5 )   szColumnCount
   String   szColumnCount = null;
   //:STRING ( 1 )   szSubControlType
   String   szSubControlType = null;
   //:STRING ( 1 )   szControlType
   String   szControlType = null;
   //:STRING ( 2 )   szHierarchicalLevel
   String   szHierarchicalLevel = null;
   //:STRING ( 20 )  szSubObjectName
   String   szSubObjectName = null;
   //:STRING ( 200 ) szMsg
   String   szMsg = null;
   //:INTEGER       CurrentWidth
   int      CurrentWidth = 0;
   //:INTEGER       CurrentRowPositionX
   int      CurrentRowPositionX = 0;
   //:INTEGER       CurrentPositionY
   int      CurrentPositionY = 0;
   //:INTEGER       LastPositionY
   int      LastPositionY = 0;
   //:INTEGER       LastPositionRowY
   int      LastPositionRowY = 0;
   //:INTEGER       LastPositionX
   int      LastPositionX = 0;
   //:INTEGER       LastWidthX
   int      LastWidthX = 0;
   //:INTEGER       FirstColumnPosition
   int      FirstColumnPosition = 0;
   //:INTEGER       TopGroupPosition
   int      TopGroupPosition = 0;
   //:INTEGER       ColumnCount
   int      ColumnCount = 0;
   //:INTEGER       FirstColumnCount
   int      FirstColumnCount = 0;
   //:INTEGER       TextColumnCount
   int      TextColumnCount = 0;
   //:INTEGER       CellPadding
   int      CellPadding = 0;
   //:INTEGER       RowCount
   int      RowCount = 0;
   //:INTEGER       TablePaddingLeft
   int      TablePaddingLeft = 0;
   //:INTEGER       TableMarginTop
   int      TableMarginTop = 0;
   //:INTEGER       TableWidth
   int      TableWidth = 0;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   String   szTempString_0 = null;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;
   int      lTempInteger_5 = 0;
   int      lTempInteger_6 = 0;


   //:// Process Controls that are all Shapes or Groups.
   //:// This is a recursive routine as a Shape/Group can contain another Shape/Group.
   //:// NOTE THAT WE ASSUME ALL CONTROLS AT THIS LEVEL ARE ALL SHAPE/GROUP CONTROLS OR ALL TEXT/ICON CONTROLS.

   //:// Increment hierarchical level for creating XSLT_WorkGroup entry.
   //:GroupHierarchicalLevel = GroupHierarchicalLevel + 1
   GroupHierarchicalLevel = GroupHierarchicalLevel + 1;

   //:// Make sure Controls are in row/column order.
   //:OrderEntityForView( vReportDef, "Control", "PSDLG_Y A PSDLG_X A" )
   OrderEntityForView( vReportDef, "Control", "PSDLG_Y A PSDLG_X A" );

   //:szPageWidth = "7.5in"
    {StringBuilder sb_szPageWidth;
   if ( szPageWidth == null )
      sb_szPageWidth = new StringBuilder( 32 );
   else
      sb_szPageWidth = new StringBuilder( szPageWidth );
      ZeidonStringCopy( sb_szPageWidth, 1, 0, "7.5in", 1, 0, 21 );
   szPageWidth = sb_szPageWidth.toString( );}
   //:szCellPaddingName = "DefaultCellPadding"
    {StringBuilder sb_szCellPaddingName;
   if ( szCellPaddingName == null )
      sb_szCellPaddingName = new StringBuilder( 32 );
   else
      sb_szCellPaddingName = new StringBuilder( szCellPaddingName );
      ZeidonStringCopy( sb_szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 31 );
   szCellPaddingName = sb_szCellPaddingName.toString( );}

   //:// Build work subobject, which organizes controls in rows and columns.
   //:// Note that we will only process Shape/Group controls with subcontrols.
   //:// Also, if any control is a Shape/Group, we assume all are.
   //:// First determine if any are a Shape/Group.
   //:szControlType = ""
    {StringBuilder sb_szControlType;
   if ( szControlType == null )
      sb_szControlType = new StringBuilder( 32 );
   else
      sb_szControlType = new StringBuilder( szControlType );
      ZeidonStringCopy( sb_szControlType, 1, 0, "", 1, 0, 2 );
   szControlType = sb_szControlType.toString( );}
   //:FOR EACH vReportDef.Control
   RESULT = SetCursorFirstEntity( vReportDef, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vReportDef.CtrlCtrl EXISTS AND vReportDef.ControlDef.Tag = "Rpt_Shape"
      lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "CtrlCtrl" );
      if ( lTempInteger_0 == 0 && CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_Shape" ) == 0 )
      { 
         //:szControlType = "G"
          {StringBuilder sb_szControlType;
         if ( szControlType == null )
            sb_szControlType = new StringBuilder( 32 );
         else
            sb_szControlType = new StringBuilder( szControlType );
                  ZeidonStringCopy( sb_szControlType, 1, 0, "G", 1, 0, 2 );
         szControlType = sb_szControlType.toString( );}
      } 

      RESULT = SetCursorNextEntity( vReportDef, "Control", "" );
      //:END
   } 

   //:END

   //:// If this level of Controls has NO Shapes/Groups, we will process as either AllText or SubText, based on whether we're in here for the first time.
   //:IF szControlType = ""
   if ( ZeidonStringCompare( szControlType, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:IF GroupHierarchicalLevel > 2
      if ( GroupHierarchicalLevel > 2 )
      { 
         //:CreateViewFromView( vReportDefRoot2, vReportDefRoot )
         CreateViewFromView( vReportDefRoot2, vReportDefRoot );
         //:szHierarchicalLevel = GroupHierarchicalLevel
          {StringBuilder sb_szHierarchicalLevel;
         if ( szHierarchicalLevel == null )
            sb_szHierarchicalLevel = new StringBuilder( 32 );
         else
            sb_szHierarchicalLevel = new StringBuilder( szHierarchicalLevel );
                  ZeidonStringConvertFromNumber( sb_szHierarchicalLevel, 1, 0, 2, GroupHierarchicalLevel, (double) 0.0, "I" );
         szHierarchicalLevel = sb_szHierarchicalLevel.toString( );}
         //:szSubObjectName = "vReportDef" + szHierarchicalLevel
          {StringBuilder sb_szSubObjectName;
         if ( szSubObjectName == null )
            sb_szSubObjectName = new StringBuilder( 32 );
         else
            sb_szSubObjectName = new StringBuilder( szSubObjectName );
                  ZeidonStringCopy( sb_szSubObjectName, 1, 0, "vReportDef", 1, 0, 21 );
         szSubObjectName = sb_szSubObjectName.toString( );}
          {StringBuilder sb_szSubObjectName;
         if ( szSubObjectName == null )
            sb_szSubObjectName = new StringBuilder( 32 );
         else
            sb_szSubObjectName = new StringBuilder( szSubObjectName );
                  ZeidonStringConcat( sb_szSubObjectName, 1, 0, szHierarchicalLevel, 1, 0, 21 );
         szSubObjectName = sb_szSubObjectName.toString( );}
         //:SetNameForView( vReportDefRoot2, szSubObjectName, vReportDefRoot, zLEVEL_TASK )
         SetNameForView( vReportDefRoot2, szSubObjectName, vReportDefRoot, zLEVEL_TASK );
         //:SetViewToSubobject( vReportDef, "CtrlCtrl" )
         SetViewToSubobject( vReportDef, "CtrlCtrl" );
         //:szSubIndentationChars = szIndentationChars + "      "
          {StringBuilder sb_szSubIndentationChars;
         if ( szSubIndentationChars == null )
            sb_szSubIndentationChars = new StringBuilder( 32 );
         else
            sb_szSubIndentationChars = new StringBuilder( szSubIndentationChars );
                  ZeidonStringCopy( sb_szSubIndentationChars, 1, 0, szIndentationChars, 1, 0, 51 );
         szSubIndentationChars = sb_szSubIndentationChars.toString( );}
          {StringBuilder sb_szSubIndentationChars;
         if ( szSubIndentationChars == null )
            sb_szSubIndentationChars = new StringBuilder( 32 );
         else
            sb_szSubIndentationChars = new StringBuilder( szSubIndentationChars );
                  ZeidonStringConcat( sb_szSubIndentationChars, 1, 0, "      ", 1, 0, 51 );
         szSubIndentationChars = sb_szSubIndentationChars.toString( );}
         //:XSLT_SubTextGroup( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel )
         oTZRPSRCO_XSLT_SubTextGroup( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel );
         //:ResetViewFromSubobject( vReportDef )
         ResetViewFromSubobject( vReportDef );
         //:DropView( vReportDefRoot2 )
         DropView( vReportDefRoot2 );
         //:ELSE
      } 
      else
      { 
         //:XSLT_AllTextGroup( vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars )
         oTZRPSRCO_XSLT_AllTextGroup( vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars );
      } 

      //:END

      //:ELSE
   } 
   else
   { 

      //:// At least some of the Controls are Shapes/Groups, so dontinue processing as Group.

      //:// Build the WorkGroup for organizing the Controls in tables.
      //:LastPositionY = -1
      LastPositionY = -1;
      //:CREATE ENTITY vReportDefRoot.XSLT_WorkGroup
      RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkGroup", zPOS_AFTER );
      //:vReportDefRoot.XSLT_WorkGroup.RecursiveNumber = GroupHierarchicalLevel
      SetAttributeFromInteger( vReportDefRoot, "XSLT_WorkGroup", "RecursiveNumber", GroupHierarchicalLevel );
      //:FOR EACH vReportDef.Control
      RESULT = SetCursorFirstEntity( vReportDef, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF szControlType = "G" AND vReportDef.CtrlCtrl EXISTS AND vReportDef.ControlDef.Tag = "Rpt_Shape"
         lTempInteger_1 = CheckExistenceOfEntity( vReportDef, "CtrlCtrl" );
         if ( ZeidonStringCompare( szControlType, 1, 0, "G", 1, 0, 2 ) == 0 && lTempInteger_1 == 0 && CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_Shape" ) == 0 )
         { 

            //:CurrentPositionY = vReportDef.Control.PSDLG_Y
            {MutableInt mi_CurrentPositionY = new MutableInt( CurrentPositionY );
                         GetIntegerFromAttribute( mi_CurrentPositionY, vReportDef, "Control", "PSDLG_Y" );
            CurrentPositionY = mi_CurrentPositionY.intValue( );}
            //:IF CurrentPositionY != LastPositionY
            if ( CurrentPositionY != LastPositionY )
            { 
               //:CREATE ENTITY vReportDefRoot.XSLT_WorkRow
               RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkRow", zPOS_AFTER );
               //:vReportDefRoot.XSLT_WorkRow.PSDLG_Y = vReportDef.Control.PSDLG_Y
               SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y", vReportDef, "Control", "PSDLG_Y" );
               //:vReportDefRoot.XSLT_WorkRow.Tag     = vReportDef.Control.Tag + "Parent"
               {StringBuilder sb_szTempString_0;
               if ( szTempString_0 == null )
                  sb_szTempString_0 = new StringBuilder( 32 );
               else
                  sb_szTempString_0 = new StringBuilder( szTempString_0 );
                               GetStringFromAttribute( sb_szTempString_0, vReportDef, "Control", "Tag" );
               szTempString_0 = sb_szTempString_0.toString( );}
                {StringBuilder sb_szTempString_0;
               if ( szTempString_0 == null )
                  sb_szTempString_0 = new StringBuilder( 32 );
               else
                  sb_szTempString_0 = new StringBuilder( szTempString_0 );
                              ZeidonStringConcat( sb_szTempString_0, 1, 0, "Parent", 1, 0, 33 );
               szTempString_0 = sb_szTempString_0.toString( );}
               SetAttributeFromString( vReportDefRoot, "XSLT_WorkRow", "Tag", szTempString_0 );
            } 

            //:END
            //:LastPositionY = CurrentPositionY
            LastPositionY = CurrentPositionY;
            //:CREATE ENTITY vReportDefRoot.XSLT_WorkColumn
            RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkColumn", zPOS_AFTER );
            //:vReportDefRoot.XSLT_WorkColumn.ControlZKey = vReportDef.Control.ZKey
            SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkColumn", "ControlZKey", vReportDef, "Control", "ZKey" );
            //:vReportDefRoot.XSLT_WorkColumn.ControlType = vReportDef.ControlDef.Tag
            SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkColumn", "ControlType", vReportDef, "ControlDef", "Tag" );
            //:SetMatchingAttributesByName( vReportDefRoot, "XSLT_WorkColumn",  vReportDef, "Control", zSET_NULL )
            SetMatchingAttributesByName( vReportDefRoot, "XSLT_WorkColumn", vReportDef, "Control", zSET_NULL );
         } 

         RESULT = SetCursorNextEntity( vReportDef, "Control", "" );
         //:END
      } 

      //:END

      //:// Name Views for Debugging.
      //:szHierarchicalLevel = GroupHierarchicalLevel
       {StringBuilder sb_szHierarchicalLevel;
      if ( szHierarchicalLevel == null )
         sb_szHierarchicalLevel = new StringBuilder( 32 );
      else
         sb_szHierarchicalLevel = new StringBuilder( szHierarchicalLevel );
            ZeidonStringConvertFromNumber( sb_szHierarchicalLevel, 1, 0, 2, GroupHierarchicalLevel, (double) 0.0, "I" );
      szHierarchicalLevel = sb_szHierarchicalLevel.toString( );}
      //:szSubObjectName = "vReportDefWorkGroup" + szHierarchicalLevel
       {StringBuilder sb_szSubObjectName;
      if ( szSubObjectName == null )
         sb_szSubObjectName = new StringBuilder( 32 );
      else
         sb_szSubObjectName = new StringBuilder( szSubObjectName );
            ZeidonStringCopy( sb_szSubObjectName, 1, 0, "vReportDefWorkGroup", 1, 0, 21 );
      szSubObjectName = sb_szSubObjectName.toString( );}
       {StringBuilder sb_szSubObjectName;
      if ( szSubObjectName == null )
         sb_szSubObjectName = new StringBuilder( 32 );
      else
         sb_szSubObjectName = new StringBuilder( szSubObjectName );
            ZeidonStringConcat( sb_szSubObjectName, 1, 0, szHierarchicalLevel, 1, 0, 21 );
      szSubObjectName = sb_szSubObjectName.toString( );}
      //:NAME VIEW vReportDefRoot szSubObjectName
      SetNameForView( vReportDefRoot, szSubObjectName, null, zLEVEL_TASK );
      //:szSubObjectName = "vReportDefControl" + szHierarchicalLevel
       {StringBuilder sb_szSubObjectName;
      if ( szSubObjectName == null )
         sb_szSubObjectName = new StringBuilder( 32 );
      else
         sb_szSubObjectName = new StringBuilder( szSubObjectName );
            ZeidonStringCopy( sb_szSubObjectName, 1, 0, "vReportDefControl", 1, 0, 21 );
      szSubObjectName = sb_szSubObjectName.toString( );}
       {StringBuilder sb_szSubObjectName;
      if ( szSubObjectName == null )
         sb_szSubObjectName = new StringBuilder( 32 );
      else
         sb_szSubObjectName = new StringBuilder( szSubObjectName );
            ZeidonStringConcat( sb_szSubObjectName, 1, 0, szHierarchicalLevel, 1, 0, 21 );
      szSubObjectName = sb_szSubObjectName.toString( );}
      //:NAME VIEW vReportDef szSubObjectName
      SetNameForView( vReportDef, szSubObjectName, null, zLEVEL_TASK );

      //:// Process Row/Column Work Structure to generate Tables and Columns.

      //:TopGroupPosition = vReportDefRoot.XSLT_WorkColumn.PSDLG_Y
      {MutableInt mi_TopGroupPosition = new MutableInt( TopGroupPosition );
             GetIntegerFromAttribute( mi_TopGroupPosition, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_Y" );
      TopGroupPosition = mi_TopGroupPosition.intValue( );}
      //:RowCount = 0
      RowCount = 0;

      //:FOR EACH vReportDefRoot.XSLT_WorkRow
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:RowCount = RowCount + 1
         RowCount = RowCount + 1;

         //:// Build open table and column statements.
         //:   // Open a new table.

         //:   // If this is the topmost Group within the parent, we'll use it's Y position as the margin top. Otherwise,
         //:   // the margin top will be zero.
         //:   IF RowCount = 1
         if ( RowCount == 1 )
         { 
            //:   TableMarginTop  = vReportDefRoot.XSLT_WorkRow.PSDLG_Y
            {MutableInt mi_TableMarginTop = new MutableInt( TableMarginTop );
                         GetIntegerFromAttribute( mi_TableMarginTop, vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y" );
            TableMarginTop = mi_TableMarginTop.intValue( );}
            //:ELSE
         } 
         else
         { 
            //:   TableMarginTop  = 0
            TableMarginTop = 0;
         } 

         //:   END
         //:   ConvertPxToInchesCH( vReportDefRoot, szTableMarginTop, TableMarginTop )
         {StringBuilder sb_szTableMarginTop;
         if ( szTableMarginTop == null )
            sb_szTableMarginTop = new StringBuilder( 32 );
         else
            sb_szTableMarginTop = new StringBuilder( szTableMarginTop );
                   oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, sb_szTableMarginTop, TableMarginTop );
         szTableMarginTop = sb_szTableMarginTop.toString( );}

         //:   // Compute the following values:
         //:   //     Table Padding Left (used to create a dummy column to shift first table column right as necessary.
         //:   //     Table Width (total of Padding Left and all sub Controls, which is determined from last column)
         //:   SET CURSOR FIRST vReportDefRoot.XSLT_WorkColumn
         RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         //:   FirstColumnPosition = vReportDefRoot.XSLT_WorkColumn.PSDLG_X
         {MutableInt mi_FirstColumnPosition = new MutableInt( FirstColumnPosition );
                   GetIntegerFromAttribute( mi_FirstColumnPosition, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
         FirstColumnPosition = mi_FirstColumnPosition.intValue( );}
         //:   // Table Padding Left is generated for the first column within a table or page row.
         //:   // If this is the first column on the page row, (which is determined by the hierarchical level being 2), then we assume there
         //:   // are 100 positions of normal indentation.
         //:   IF GroupHierarchicalLevel = 2
         if ( GroupHierarchicalLevel == 2 )
         { 
            //:   IF FirstColumnPosition > 100   // We'll assume 100 is normal indentation and anything after that must shift the columns right.
            if ( FirstColumnPosition > 100 )
            { 
               //:   TablePaddingLeft = FirstColumnPosition - 100
               TablePaddingLeft = FirstColumnPosition - 100;
               //:   ConvertPxToInchesCH( vReportDefRoot, szTablePaddingLeft, TablePaddingLeft )
               {StringBuilder sb_szTablePaddingLeft;
               if ( szTablePaddingLeft == null )
                  sb_szTablePaddingLeft = new StringBuilder( 32 );
               else
                  sb_szTablePaddingLeft = new StringBuilder( szTablePaddingLeft );
                               oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, sb_szTablePaddingLeft, TablePaddingLeft );
               szTablePaddingLeft = sb_szTablePaddingLeft.toString( );}
            } 

            //:   END
            //:ELSE
         } 
         else
         { 
            //:   TablePaddingLeft = FirstColumnPosition
            TablePaddingLeft = FirstColumnPosition;
            //:   ConvertPxToInchesCH( vReportDefRoot, szTablePaddingLeft, TablePaddingLeft )
            {StringBuilder sb_szTablePaddingLeft;
            if ( szTablePaddingLeft == null )
               sb_szTablePaddingLeft = new StringBuilder( 32 );
            else
               sb_szTablePaddingLeft = new StringBuilder( szTablePaddingLeft );
                         oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, sb_szTablePaddingLeft, TablePaddingLeft );
            szTablePaddingLeft = sb_szTablePaddingLeft.toString( );}
         } 

         //:   END

         //:   // Table Width
         //:   SET CURSOR LAST vReportDefRoot.XSLT_WorkColumn
         RESULT = SetCursorLastEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         //:   TableWidth = vReportDefRoot.XSLT_WorkColumn.PSDLG_X + vReportDefRoot.XSLT_WorkColumn.SZDLG_X
         {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
                   GetIntegerFromAttribute( mi_lTempInteger_2, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
         lTempInteger_2 = mi_lTempInteger_2.intValue( );}
         {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
                   GetIntegerFromAttribute( mi_lTempInteger_3, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
         lTempInteger_3 = mi_lTempInteger_3.intValue( );}
         TableWidth = lTempInteger_2 + lTempInteger_3;
         //:   ConvertPxToInchesCH( vReportDefRoot, szTableWidth, TableWidth )
         {StringBuilder sb_szTableWidth;
         if ( szTableWidth == null )
            sb_szTableWidth = new StringBuilder( 32 );
         else
            sb_szTableWidth = new StringBuilder( szTableWidth );
                   oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, sb_szTableWidth, TableWidth );
         szTableWidth = sb_szTableWidth.toString( );}

         //:   // Table Block - Initially, we'll make the table block visible.
         //:   szOutputLine = szIndentationChars + ""
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:   szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + ">"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:block border=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "normal solid black", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:   szOutputLine = szIndentationChars + ""
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

         //:   // Table statement
         //:   szOutputLine = szIndentationChars + "<fo:table table-layout=" + QUOTES + "fixed" +
         //:                  QUOTES + " width=" + QUOTES + szTableWidth + "in" +
         //:                  QUOTES + " margin-top=" + QUOTES + szTableMarginTop +  "in" + QUOTES + ">"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table table-layout=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "fixed", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, " width=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, szTableWidth, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, szTableMarginTop, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

         //:   // Column Statements

         //:   // Generate Padding Column, if TablePaddingLeft is > 0.
         //:   IF TablePaddingLeft > 0
         if ( TablePaddingLeft > 0 )
         { 
            //:   szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szTablePaddingLeft + "in" + QUOTES + "/>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szTablePaddingLeft, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         } 

         //:   END

         //:   // Generate Column Statement for each work Column entry.
         //:   FOR EACH vReportDefRoot.XSLT_WorkColumn
         RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 

            //:   // The width is the difference between the starting position of this control and the starting position of the next control.
            //:   // If there is no next control, the width will be the actual width of the control.
            //:   CreateViewFromView( vReportDef3, vReportDefRoot )
            CreateViewFromView( vReportDef3, vReportDefRoot );
            //:   SET CURSOR NEXT vReportDef3.XSLT_WorkColumn
            RESULT = SetCursorNextEntity( vReportDef3, "XSLT_WorkColumn", "" );
            //:   IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:   CurrentWidth  = vReportDef3.XSLT_WorkColumn.PSDLG_X - vReportDefRoot.XSLT_WorkColumn.PSDLG_X
               {MutableInt mi_lTempInteger_4 = new MutableInt( lTempInteger_4 );
                               GetIntegerFromAttribute( mi_lTempInteger_4, vReportDef3, "XSLT_WorkColumn", "PSDLG_X" );
               lTempInteger_4 = mi_lTempInteger_4.intValue( );}
               {MutableInt mi_lTempInteger_5 = new MutableInt( lTempInteger_5 );
                               GetIntegerFromAttribute( mi_lTempInteger_5, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
               lTempInteger_5 = mi_lTempInteger_5.intValue( );}
               CurrentWidth = lTempInteger_4 - lTempInteger_5;
               //:ELSE
            } 
            else
            { 
               //:   CurrentWidth  = vReportDefRoot.XSLT_WorkColumn.SZDLG_X
               {MutableInt mi_CurrentWidth = new MutableInt( CurrentWidth );
                               GetIntegerFromAttribute( mi_CurrentWidth, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
               CurrentWidth = mi_CurrentWidth.intValue( );}
            } 

            //:   END
            //:   DropView( vReportDef3 )
            DropView( vReportDef3 );
            //:   ConvertPxToInchesCH( vReportDefRoot, szCurrentWidth, CurrentWidth )
            {StringBuilder sb_szCurrentWidth;
            if ( szCurrentWidth == null )
               sb_szCurrentWidth = new StringBuilder( 32 );
            else
               sb_szCurrentWidth = new StringBuilder( szCurrentWidth );
                         oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, sb_szCurrentWidth, CurrentWidth );
            szCurrentWidth = sb_szCurrentWidth.toString( );}

            //:   szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szCurrentWidth + "in" + QUOTES + "/>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szCurrentWidth, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "in", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
            RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         } 

         //:   END

         //:   // Generate table-body statement, table-row and block.
         //:   szOutputLine = szIndentationChars + "<fo:table-body>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "<fo:table-body>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

         //:   szOutputLine = szIndentationChars + "  <fo:table-row>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "  <fo:table-row>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

         //:   // Generate the table-cell statements, which will actually end up calling this recursive routine, XSLT_GroupRecur, to
         //:   // process sub-controls.

         //:   // Generate Padding Cell, if TablePaddingLeft is > 0.
         //:   IF TablePaddingLeft > 0
         if ( TablePaddingLeft > 0 )
         { 
            //:   szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES + ">"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

            //:   szOutputLine = szIndentationChars + "      <fo:block></fo:block>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "      <fo:block></fo:block>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

            //:   szOutputLine = szIndentationChars + "    </fo:table-cell>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         } 

         //:   END

         //:   FOR EACH vReportDefRoot.XSLT_WorkColumn
         RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:   szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES + ">"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

            //:   // Process Sub Control Entities.
            //:   SET CURSOR FIRST vReportDef.Control WHERE vReportDef.Control.ZKey = vReportDefRoot.XSLT_WorkColumn.ControlZKey
            {MutableInt mi_lTempInteger_6 = new MutableInt( lTempInteger_6 );
                         GetIntegerFromAttribute( mi_lTempInteger_6, vReportDefRoot, "XSLT_WorkColumn", "ControlZKey" );
            lTempInteger_6 = mi_lTempInteger_6.intValue( );}
            RESULT = SetCursorFirstEntityByInteger( vReportDef, "Control", "ZKey", lTempInteger_6, "" );
            //:   // Create new view so that we can save our current position.
            //:   CreateViewFromView( vReportDefRoot2, vReportDefRoot )
            CreateViewFromView( vReportDefRoot2, vReportDefRoot );
            //:   SetViewToSubobject( vReportDef, "CtrlCtrl" )
            SetViewToSubobject( vReportDef, "CtrlCtrl" );
            //:   szSubIndentationChars = szIndentationChars + "      "
             {StringBuilder sb_szSubIndentationChars;
            if ( szSubIndentationChars == null )
               sb_szSubIndentationChars = new StringBuilder( 32 );
            else
               sb_szSubIndentationChars = new StringBuilder( szSubIndentationChars );
                        ZeidonStringCopy( sb_szSubIndentationChars, 1, 0, szIndentationChars, 1, 0, 51 );
            szSubIndentationChars = sb_szSubIndentationChars.toString( );}
             {StringBuilder sb_szSubIndentationChars;
            if ( szSubIndentationChars == null )
               sb_szSubIndentationChars = new StringBuilder( 32 );
            else
               sb_szSubIndentationChars = new StringBuilder( szSubIndentationChars );
                        ZeidonStringConcat( sb_szSubIndentationChars, 1, 0, "      ", 1, 0, 51 );
            szSubIndentationChars = sb_szSubIndentationChars.toString( );}

            //:   IF vReportDef.ControlDef.Tag = "Rpt_Shape"
            if ( CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_Shape" ) == 0 )
            { 
               //:   // Call XSLT_GroupRecur recursively.
               //:   XSLT_GroupRecur( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel )
               oTZRPSRCO_XSLT_GroupRecur( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel );
               //:ELSE
            } 
            else
            { 
               //:   // Call XSLT_SubTextGroup to format data.
               //:   XSLT_SubTextGroup( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel )
               oTZRPSRCO_XSLT_SubTextGroup( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel );
            } 

            //:   END


            //:   ResetViewFromSubobject( vReportDef )
            ResetViewFromSubobject( vReportDef );
            //:   DropView( vReportDefRoot2 )
            DropView( vReportDefRoot2 );

            //:   // Close table-cell
            //:   szOutputLine = szIndentationChars + "    </fo:table-cell>"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
            RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         } 

         //:   END


         //:   // Build close table statements
         //:   szOutputLine = szIndentationChars + "  </fo:table-row>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "  </fo:table-row>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:   szOutputLine = szIndentationChars + "</fo:table-body>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:table-body>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:   szOutputLine = szIndentationChars + "</fo:table>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:table>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:   szOutputLine = szIndentationChars + ""
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:   szOutputLine = szIndentationChars + "</fo:block>"
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:   szOutputLine = szIndentationChars + ""
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
          {StringBuilder sb_szOutputLine;
         if ( szOutputLine == null )
            sb_szOutputLine = new StringBuilder( 32 );
         else
            sb_szOutputLine = new StringBuilder( szOutputLine );
                  ZeidonStringConcat( sb_szOutputLine, 1, 0, "", 1, 0, 5001 );
         szOutputLine = sb_szOutputLine.toString( );}
         //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      } 


      //:END
   } 

   //:END

   //:// Remove the Work Group for this hierarchical level.
   //:DELETE ENTITY vReportDefRoot.XSLT_WorkGroup
   RESULT = DeleteEntity( vReportDefRoot, "XSLT_WorkGroup", zPOS_NEXT );
   return( 0 );
// END
} 


//:LOCAL OPERATION
private void 
oTZRPSRCO_GenerateXRP_CtrlMap( View     NewReport,
                               View     DialogControl )
{
   int      lTempInteger_0 = 0;
   int      RESULT = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;

   //:GenerateXRP_CtrlMap( VIEW NewReport     BASED ON LOD TZRPSRCO,
   //:                  VIEW DialogControl BASED ON LOD TZWDLGSO )

   //:// Build the Report CtrlMap subobject from the Dialog subobject.
   //:CreateMetaEntity( DialogControl, NewReport, "CtrlMap", zPOS_AFTER )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogControl );
    m_TZCM_OPR_Operation.CreateMetaEntity( DialogControl, NewReport, "CtrlMap", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
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
//:GenerateXSLT( VIEW SourceReport BASED ON LOD TZRPSRCO )

//:   VIEW vReportDef BASED ON LOD TZRPSRCO
public int 
oTZRPSRCO_GenerateXSLT( View     SourceReport )
{
   zVIEW    vReportDef = new zVIEW( );
   //:VIEW vLOD       BASED ON LOD TZZOLODO
   zVIEW    vLOD = new zVIEW( );
   //:VIEW vTaskLPLR  BASED ON LOD TZCMLPLO
   zVIEW    vTaskLPLR = new zVIEW( );
   //:STRING ( 32 )   szTopEntityName
   String   szTopEntityName = null;
   //:STRING ( 200 )  szMsg
   String   szMsg = null;
   //:STRING ( 200 )  szFileName
   String   szFileName = null;
   //:STRING ( 5000 ) szOutputLine
   String   szOutputLine = null;
   //:STRING ( 200 )  szPathName
   String   szPathName = null;
   //:INTEGER         lFileHandle
   int      lFileHandle = 0;
   //:SHORT           nRC
   int      nRC = 0;
   String   szTempString_0 = null;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      RESULT = 0;
   String   szTempString_1 = null;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;


   //:// Build an XSLT object from the Dialog Window passed.
   //:vReportDef = SourceReport
   SetViewFromView( vReportDef, SourceReport );

   //:// Open XSLT output file.
   //:SysReadZeidonIni( -1, "[App.Zencas]", "XSLTDirectory", szPathName )
   {StringBuilder sb_szPathName;
   if ( szPathName == null )
      sb_szPathName = new StringBuilder( 32 );
   else
      sb_szPathName = new StringBuilder( szPathName );
       m_KZOEP1AA.SysReadZeidonIni( -1, "[App.Zencas]", "XSLTDirectory", sb_szPathName );
   szPathName = sb_szPathName.toString( );}

   //://szFileName = "c:\temp\XSLT_OutputTest.xsl"
   //:szFileName = szPathName + vReportDef.Report.Tag + ".xsl"
   {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
   StringBuilder sb_szTempString_0;
   if ( szTempString_0 == null )
      sb_szTempString_0 = new StringBuilder( 32 );
   else
      sb_szTempString_0 = new StringBuilder( szTempString_0 );
       GetVariableFromAttribute( sb_szTempString_0, mi_lTempInteger_0, 'S', 9, vReportDef, "Report", "Tag", "", 0 );
   lTempInteger_0 = mi_lTempInteger_0.intValue( );
   szTempString_0 = sb_szTempString_0.toString( );}
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringCopy( sb_szFileName, 1, 0, szPathName, 1, 0, 201 );
   szFileName = sb_szFileName.toString( );}
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringConcat( sb_szFileName, 1, 0, szTempString_0, 1, 0, 201 );
   szFileName = sb_szFileName.toString( );}
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringConcat( sb_szFileName, 1, 0, ".xsl", 1, 0, 201 );
   szFileName = sb_szFileName.toString( );}
   //:lFileHandle = SysOpenFile( SourceReport, szFileName, COREFILE_WRITE )
   try
   {
       lFileHandle = m_KZOEP1AA.SysOpenFile( SourceReport, szFileName, COREFILE_WRITE );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   //:IF lFileHandle < 0
   if ( lFileHandle < 0 )
   { 
      //:szMsg = "Cannot open XSLT Output File, " + szFileName
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "Cannot open XSLT Output File, ", 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, szFileName, 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
      //:MessageSend( SourceReport, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( SourceReport, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
   } 

   //:END

   //:// Activate test PRP file.
   //:// THIS WAS ONLY FOR TESTING...
   //://szFileName = "c:\lplr\zencas\rSABill.prp"
   //://ActivateOI_FromFile( vReportDef, "TZRPSRCO", SourceReport, szFileName, zSINGLE )
   //://NAME VIEW vReportDef "TZRPSRCO_XSLT"

   //:// Activate LOD that goes with PRP file. The registered view that is also the entity DrivingViewObjRef.
   //:IF vReportDef.DrivingViewObjRef EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( vReportDef, "DrivingViewObjRef" );
   if ( lTempInteger_1 == 0 )
   { 
      //://szFileName = "c:\temp\mSAProfE.lod"
      //:GET VIEW vTaskLPLR NAMED "TaskLPLR"
      RESULT = GetViewByName( vTaskLPLR, "TaskLPLR", SourceReport, zLEVEL_TASK );
      //:szFileName = vTaskLPLR.LPLR.MetaSrcDir + "\"
      {StringBuilder sb_szFileName;
      if ( szFileName == null )
         sb_szFileName = new StringBuilder( 32 );
      else
         sb_szFileName = new StringBuilder( szFileName );
             GetStringFromAttribute( sb_szFileName, vTaskLPLR, "LPLR", "MetaSrcDir" );
      szFileName = sb_szFileName.toString( );}
       {StringBuilder sb_szFileName;
      if ( szFileName == null )
         sb_szFileName = new StringBuilder( 32 );
      else
         sb_szFileName = new StringBuilder( szFileName );
            ZeidonStringConcat( sb_szFileName, 1, 0, "\\", 1, 0, 201 );
      szFileName = sb_szFileName.toString( );}
      //:szFileName = szFileName + vReportDef.DrivingLOD.Name + ".lod"
      {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
      StringBuilder sb_szTempString_1;
      if ( szTempString_1 == null )
         sb_szTempString_1 = new StringBuilder( 32 );
      else
         sb_szTempString_1 = new StringBuilder( szTempString_1 );
             GetVariableFromAttribute( sb_szTempString_1, mi_lTempInteger_2, 'S', 9, vReportDef, "DrivingLOD", "Name", "", 0 );
      lTempInteger_2 = mi_lTempInteger_2.intValue( );
      szTempString_1 = sb_szTempString_1.toString( );}
       {StringBuilder sb_szFileName;
      if ( szFileName == null )
         sb_szFileName = new StringBuilder( 32 );
      else
         sb_szFileName = new StringBuilder( szFileName );
            ZeidonStringConcat( sb_szFileName, 1, 0, szTempString_1, 1, 0, 201 );
      szFileName = sb_szFileName.toString( );}
       {StringBuilder sb_szFileName;
      if ( szFileName == null )
         sb_szFileName = new StringBuilder( 32 );
      else
         sb_szFileName = new StringBuilder( szFileName );
            ZeidonStringConcat( sb_szFileName, 1, 0, ".lod", 1, 0, 201 );
      szFileName = sb_szFileName.toString( );}
      //:ActivateOI_FromFile( vLOD, "TZZOLODO", SourceReport, szFileName, zSINGLE )
      ActivateOI_FromFile( vLOD, "TZZOLODO", SourceReport, szFileName, zSINGLE );
      //:NAME VIEW vLOD "LOD_XSLT"
      SetNameForView( vLOD, "LOD_XSLT", null, zLEVEL_TASK );
      //:ELSE
   } 
   else
   { 
      //:szMsg = "One of the registered views needs to be set as the 'Driving View'. "
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "One of the registered views needs to be set as the 'Driving View'. ", 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
      //:MessageSend( SourceReport, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( SourceReport, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
   } 

   //:END

   //:// Go to build XSLT Header.
   //:XSLT_MainHeader( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_XSLT_MainHeader( vReportDef, lFileHandle, szOutputLine );

   //:// Process any Page Header and Footer.
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PH"
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PH", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
   } 

   //:   // Process Page Header

   //:END
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PF"
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PF", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
   } 

   //:   // Process Page Footer

   //:END

   //:// Process the LOD entities recursively, processing each entity that has a corresponding GroupSet in the Report.
   //:// Because the Top Entity (Entity which drives the report) is processed somewhat differently, we need to identify
   //:// that entity name and pass it to the subroutine.
   //:// There is a problem, however, in that the Painter seems to be generating an initial GroupSet named, "Rpt_GroupSet1". So
   //:// we need to skip over that GroupSet, if it exists.
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag != "Rpt_GroupSet1"
   RESULT = SetCursorFirstEntity( vReportDef, "GroupSet", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   { 
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( vReportDef, "GroupSet", "Tag", "Rpt_GroupSet1" ) == 0 ) )
      { 
         RESULT = SetCursorNextEntity( vReportDef, "GroupSet", "" );
      } 

   } 

   //:szTopEntityName = vReportDef.GroupSet.Tag
   {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
   StringBuilder sb_szTopEntityName;
   if ( szTopEntityName == null )
      sb_szTopEntityName = new StringBuilder( 32 );
   else
      sb_szTopEntityName = new StringBuilder( szTopEntityName );
       GetVariableFromAttribute( sb_szTopEntityName, mi_lTempInteger_3, 'S', 33, vReportDef, "GroupSet", "Tag", "", 0 );
   lTempInteger_3 = mi_lTempInteger_3.intValue( );
   szTopEntityName = sb_szTopEntityName.toString( );}
   //:CREATE ENTITY vReportDef.XSLT_WorkGroup
   RESULT = CreateEntity( vReportDef, "XSLT_WorkGroup", zPOS_AFTER );
   //:vReportDef.XSLT_WorkGroup.RecursiveNumber = 1
   SetAttributeFromInteger( vReportDef, "XSLT_WorkGroup", "RecursiveNumber", 1 );
   //:NAME VIEW vReportDef "vReportDefRoot"
   SetNameForView( vReportDef, "vReportDefRoot", null, zLEVEL_TASK );
   //:XSLT_EntityRecur( vReportDef, vLOD, lFileHandle, szOutputLine, szTopEntityName )
   oTZRPSRCO_XSLT_EntityRecur( vReportDef, vLOD, lFileHandle, szOutputLine, szTopEntityName );
   //:DELETE ENTITY vReportDef.XSLT_WorkGroup
   RESULT = DeleteEntity( vReportDef, "XSLT_WorkGroup", zPOS_NEXT );

   //:// Close StyleSheet.
   //:szOutputLine = "</xsl:stylesheet>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "</xsl:stylesheet>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:SysCloseFile( SourceReport, lFileHandle, 0 )
   try
   {
       m_KZOEP1AA.SysCloseFile( SourceReport, lFileHandle, null );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
public int 
oTZRPSRCO_XSLT_MainHeader( View     vReportDef,
                           int      lFileHandle,
                           String   szOutputLine )
{

   //:XSLT_MainHeader( VIEW vReportDef BASED ON LOD TZRPSRCO,
   //:              INTEGER lFileHandle,
   //:              STRING ( 5000 ) szOutputLine )

   //:// Generate the XSLT Header statements.

   //:// Build the XML file header.
   //:szOutputLine = "<?xml version=" + QUOTES + "1.0" + QUOTES + " encoding=" + QUOTES + "iso-8859-1" + QUOTES + "?>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "<?xml version=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "1.0", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " encoding=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "iso-8859-1", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "?>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:SysWriteLine( vReportDef, lFileHandle, szOutputLine )
   try
   {
       m_KZOEP1AA.SysWriteLine( vReportDef, lFileHandle, szOutputLine );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   //:szOutputLine = "<xsl:stylesheet version=" + QUOTES + "1.0" + QUOTES
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "<xsl:stylesheet version=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "1.0", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  xmlns:xsl=" + QUOTES + "http://www.w3.org/1999/XSL/Transform" + QUOTES
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  xmlns:xsl=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "http://www.w3.org/1999/XSL/Transform", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  xmlns:fo=" + QUOTES + "http://www.w3.org/1999/XSL/Format" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  xmlns:fo=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "http://www.w3.org/1999/XSL/Format", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <xsl:output method=" + QUOTES + "xml" + QUOTES + " indent=" + QUOTES + "yes" + QUOTES + "/>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <xsl:output method=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "xml", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " indent=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "yes", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Build Default Padding <xsl:attribute-set.
   //:szOutputLine = "<xsl:attribute-set name=" + QUOTES + "DefaultCellPadding" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "<xsl:attribute-set name=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "DefaultCellPadding", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <xsl:attribute name=" + QUOTES + "padding-left" + QUOTES + ">5px</xsl:attribute>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <xsl:attribute name=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "padding-left", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">5px</xsl:attribute>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <xsl:attribute name=" + QUOTES + "padding-top" + QUOTES + ">2px</xsl:attribute>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <xsl:attribute name=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "padding-top", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">2px</xsl:attribute>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <xsl:attribute name=" + QUOTES + "padding-bottom" + QUOTES + ">2px</xsl:attribute>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <xsl:attribute name=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "padding-bottom", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">2px</xsl:attribute>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "</xsl:attribute-set>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "</xsl:attribute-set>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Build base template and root statements.
   //:szOutputLine = "<xsl:template match=" + QUOTES + "/" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "<xsl:template match=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "/", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "<fo:root>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "<fo:root>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = " <fo:layout-master-set>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, " <fo:layout-master-set>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <fo:simple-page-master master-name=" + QUOTES + "aoa-page" + QUOTES + " page-height=" + QUOTES + "11in" + QUOTES
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  <fo:simple-page-master master-name=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "aoa-page", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " page-height=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "11in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   page-width=" + QUOTES + "8.5in" + QUOTES + " margin-top=" + QUOTES + ".5in" + QUOTES + " margin-bottom=" + QUOTES + ".5in" + QUOTES
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   page-width=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "8.5in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ".5in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " margin-bottom=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ".5in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   margin-left=" + QUOTES + ".5in" + QUOTES + " margin-right=" + QUOTES + ".5in" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   margin-left=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ".5in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " margin-right=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ".5in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:region-body margin-top=" + QUOTES + ".1in" + QUOTES + " margin-bottom=" + QUOTES + ".5in" + QUOTES + "/>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   <fo:region-body margin-top=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ".1in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " margin-bottom=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ".5in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:region-before extent=" + QUOTES + ".5in" + QUOTES + " />"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   <fo:region-before extent=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ".5in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " />", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:region-after extent=" + QUOTES + ".35in" + QUOTES + " />"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   <fo:region-after extent=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ".35in", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " />", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  </fo:simple-page-master>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  </fo:simple-page-master>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = " </fo:layout-master-set>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, " </fo:layout-master-set>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Build Page Sequence.
   //:szOutputLine = " <fo:page-sequence master-reference=" + QUOTES + "aoa-page" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, " <fo:page-sequence master-reference=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "aoa-page", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:static-content flow-name=" + QUOTES + "xsl-region-before" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   <fo:static-content flow-name=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "xsl-region-before", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "     <fo:block text-align=" + QUOTES + "center" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "     <fo:block text-align=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "center", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "       <fo:block font-size=" + QUOTES + "9pt" + QUOTES + " text-align=" + QUOTES + "right" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "       <fo:block font-size=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "9pt", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " text-align=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "right", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "          Page <fo:page-number/> of <fo:page-number-citation ref-id=" + QUOTES + "last-page" + QUOTES + "/>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "          Page <fo:page-number/> of <fo:page-number-citation ref-id=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "last-page", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "       </fo:block>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "       </fo:block>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "     </fo:block>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "     </fo:block>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   </fo:static-content>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   </fo:static-content>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:static-content flow-name=" + QUOTES + "xsl-region-after" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   <fo:static-content flow-name=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "xsl-region-after", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "     <fo:block font-size=" + QUOTES + "8pt" + QUOTES + " text-align=" + QUOTES + "center" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "     <fo:block font-size=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "8pt", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " text-align=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "center", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "       <fo:leader leader-pattern=" + QUOTES + "rule" + QUOTES + " leader-length.optimum=" + QUOTES +
   //:               "100%" + QUOTES + " rule-style=" + QUOTES + "double" + QUOTES + " rule-thickness=" + QUOTES + "1pt" + QUOTES + "/>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "       <fo:leader leader-pattern=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "rule", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " leader-length.optimum=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "100%", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " rule-style=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "double", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, " rule-thickness=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "1pt", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "       <xsl:value-of select=" + QUOTES + "footer/fbody" + QUOTES + "/>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "       <xsl:value-of select=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "footer/fbody", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "/>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "     </fo:block>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "     </fo:block>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   </fo:static-content>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   </fo:static-content>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Build Flow.
   //:szOutputLine = "   <fo:flow flow-name=" + QUOTES + "xsl-region-body" + QUOTES + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   <fo:flow flow-name=", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, "xsl-region-body", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "     <xsl:apply-templates/>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "     <xsl:apply-templates/>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   </fo:flow>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   </fo:flow>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Close Page, Root and Template.
   //:szOutputLine = " </fo:page-sequence>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, " </fo:page-sequence>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "</fo:root> "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "</fo:root> ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "</xsl:template>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "</xsl:template>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  "
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenerateXRP( VIEW NewReport    BASED ON LOD TZRPSRCO,
//:             VIEW DialogWindow BASED ON LOD TZWDLGSO)

//:   VIEW vTaskLPLR        BASED ON LOD TZCMLPLO
public int 
oTZRPSRCO_GenerateXRP( zVIEW    NewReport,
                       View     DialogWindow )
{
   zVIEW    vTaskLPLR = new zVIEW( );
   //:VIEW DialogWindowHier BASED ON LOD TZWDLGSO
   zVIEW    DialogWindowHier = new zVIEW( );
   //:VIEW TestReport
   zVIEW    TestReport = new zVIEW( );
   //:STRING ( 200 ) szMsg
   String   szMsg = null;
   //:STRING ( 64 )  szReportName
   String   szReportName = null;
   //:STRING ( 32 )  szDialogTag
   String   szDialogTag = null;
   //:STRING ( 32 )  szReturnedEntityName
   String   szReturnedEntityName = null;
   //:STRING ( 32 )  szFirstEntityName
   String   szFirstEntityName = null;
   //:STRING ( 1 )   szFoundFlag
   String   szFoundFlag = null;
   //:STRING ( 1 )   szNoInitalGroupSetFlag
   String   szNoInitalGroupSetFlag = null;
   //:STRING ( 256 ) szHTML_DirectoryName
   String   szHTML_DirectoryName = null;
   //:STRING ( 500 ) szDirectoryFileName
   String   szDirectoryFileName = null;
   //:INTEGER        X_Position
   int      X_Position = 0;
   //:INTEGER        Y_Position
   int      Y_Position = 0;
   //:INTEGER        GroupSize
   int      GroupSize = 0;
   //:SHORT          lReturnedLevel
   int      lReturnedLevel = 0;
   //:SHORT          nRC
   int      nRC = 0;
   int      lTempInteger_0 = 0;
   String   szTempString_0 = null;
   int      lTempInteger_1 = 0;
   int      RESULT = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;
   int      lTempInteger_5 = 0;
   int      lTempInteger_6 = 0;
   String   szTempString_1 = null;
   int      lTempInteger_7 = 0;


   //:// Build an XRP object from the Dialog Window passed.

   //:// Initialize XRP. (Report and Page entities)
   //:szDialogTag = DialogWindow.Dialog.Tag
   {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
   StringBuilder sb_szDialogTag;
   if ( szDialogTag == null )
      sb_szDialogTag = new StringBuilder( 32 );
   else
      sb_szDialogTag = new StringBuilder( szDialogTag );
       GetVariableFromAttribute( sb_szDialogTag, mi_lTempInteger_0, 'S', 33, DialogWindow, "Dialog", "Tag", "", 0 );
   lTempInteger_0 = mi_lTempInteger_0.intValue( );
   szDialogTag = sb_szDialogTag.toString( );}
   //:szReportName = szDialogTag + DialogWindow.Window.Tag
   {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
   StringBuilder sb_szTempString_0;
   if ( szTempString_0 == null )
      sb_szTempString_0 = new StringBuilder( 32 );
   else
      sb_szTempString_0 = new StringBuilder( szTempString_0 );
       GetVariableFromAttribute( sb_szTempString_0, mi_lTempInteger_1, 'S', 33, DialogWindow, "Window", "Tag", "", 0 );
   lTempInteger_1 = mi_lTempInteger_1.intValue( );
   szTempString_0 = sb_szTempString_0.toString( );}
    {StringBuilder sb_szReportName;
   if ( szReportName == null )
      sb_szReportName = new StringBuilder( 32 );
   else
      sb_szReportName = new StringBuilder( szReportName );
      ZeidonStringCopy( sb_szReportName, 1, 0, szDialogTag, 1, 0, 65 );
   szReportName = sb_szReportName.toString( );}
    {StringBuilder sb_szReportName;
   if ( szReportName == null )
      sb_szReportName = new StringBuilder( 32 );
   else
      sb_szReportName = new StringBuilder( szReportName );
      ZeidonStringConcat( sb_szReportName, 1, 0, szTempString_0, 1, 0, 65 );
   szReportName = sb_szReportName.toString( );}
   //:ActivateEmptyObjectInstance( NewReport, "TZRPSRCO", DialogWindow, zSINGLE )
   ActivateEmptyObjectInstance( NewReport, "TZRPSRCO", DialogWindow, zSINGLE );
   //:NAME VIEW NewReport "NewReportXRP"
   SetNameForView( NewReport, "NewReportXRP", null, zLEVEL_TASK );
   //:CreateMetaEntity( DialogWindow, NewReport, "Report", zPOS_AFTER )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogWindow );
    m_TZCM_OPR_Operation.CreateMetaEntity( DialogWindow, NewReport, "Report", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
   //:NewReport.Report.Tag = szDialogTag
   SetAttributeFromString( NewReport, "Report", "Tag", szDialogTag );
   //:NewReport.Report.ReportTextSize = 100
   SetAttributeFromInteger( NewReport, "Report", "ReportTextSize", 100 );
   //:CreateMetaEntity( DialogWindow, NewReport, "Page", zPOS_AFTER )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogWindow );
    m_TZCM_OPR_Operation.CreateMetaEntity( DialogWindow, NewReport, "Page", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
   //:NewReport.Page.Tag = szReportName
   SetAttributeFromString( NewReport, "Page", "Tag", szReportName );
   //:NewReport.Page.SizeX = 2176
   SetAttributeFromInteger( NewReport, "Page", "SizeX", 2176 );
   //:NewReport.Page.SizeY = 2816
   SetAttributeFromInteger( NewReport, "Page", "SizeY", 2816 );

   //:// ViewObjRef
   //:// Build one for each ViewObjRef referenced in the Window. Note that we're not just including
   //:// every ViewObjRef from the Dialog.
   //:// The first ViewObjRef will be for the view that will drive the Report.
   //:// The PrintPageEntity will define the first entity passed to the report for printing.
   //:SET CURSOR FIRST DialogWindow.Control
   RESULT = SetCursorFirstEntity( DialogWindow, "Control", "" );
   //:CreateViewFromView( DialogWindowHier, DialogWindow )
   CreateViewFromView( DialogWindowHier, DialogWindow );
   //:DefineHierarchicalCursor( DialogWindowHier, "Window" )
   DefineHierarchicalCursor( DialogWindowHier, "Window" );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, DialogWindowHier )
   {StringBuilder sb_szReturnedEntityName;
   if ( szReturnedEntityName == null )
      sb_szReturnedEntityName = new StringBuilder( 32 );
   else
      sb_szReturnedEntityName = new StringBuilder( szReturnedEntityName );
   MutableInt mi_lReturnedLevel = new MutableInt( lReturnedLevel );
       nRC = SetCursorNextEntityHierarchical( mi_lReturnedLevel, sb_szReturnedEntityName, DialogWindowHier );
   szReturnedEntityName = sb_szReturnedEntityName.toString( );
   lReturnedLevel = mi_lReturnedLevel.intValue( );}
   //:LOOP WHILE nRC >= zCURSOR_SET AND szReturnedEntityName != "Window"
   while ( nRC >= zCURSOR_SET && ZeidonStringCompare( szReturnedEntityName, 1, 0, "Window", 1, 0, 33 ) != 0 )
   { 
      //:IF szReturnedEntityName = "CtrlMapView"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlMapView", 1, 0, 33 ) == 0 )
      { 
         //:SET CURSOR FIRST NewReport.ViewObjRef WHERE NewReport.ViewObjRef.ZKey = DialogWindowHier.CtrlMapView.ZKey
         {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
                   GetIntegerFromAttribute( mi_lTempInteger_2, DialogWindowHier, "CtrlMapView", "ZKey" );
         lTempInteger_2 = mi_lTempInteger_2.intValue( );}
         RESULT = SetCursorFirstEntityByInteger( NewReport, "ViewObjRef", "ZKey", lTempInteger_2, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:SET CURSOR FIRST DialogWindow.ViewObjRef WHERE DialogWindow.ViewObjRef.ZKey = DialogWindowHier.CtrlMapView.ZKey
            {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
                         GetIntegerFromAttribute( mi_lTempInteger_3, DialogWindowHier, "CtrlMapView", "ZKey" );
            lTempInteger_3 = mi_lTempInteger_3.intValue( );}
            RESULT = SetCursorFirstEntityByInteger( DialogWindow, "ViewObjRef", "ZKey", lTempInteger_3, "" );
            //:INCLUDE NewReport.ViewObjRef FROM DialogWindow.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( NewReport, "ViewObjRef", DialogWindow, "ViewObjRef", zPOS_AFTER );
            //:IF NewReport.Report.WebPrintPageEntity = ""
            if ( CompareAttributeToString( NewReport, "Report", "WebPrintPageEntity", "" ) == 0 )
            { 
               //:IF DialogWindowHier.CtrlMapRelatedEntity EXISTS
               lTempInteger_4 = CheckExistenceOfEntity( DialogWindowHier, "CtrlMapRelatedEntity" );
               if ( lTempInteger_4 == 0 )
               { 
                  //:NewReport.Report.WebPrintPageEntity = DialogWindowHier.CtrlMapRelatedEntity.Name
                  SetAttributeFromAttribute( NewReport, "Report", "WebPrintPageEntity", DialogWindowHier, "CtrlMapRelatedEntity", "Name" );
                  //:ELSE
               } 
               else
               { 
                  //:NewReport.Report.WebPrintPageEntity = DialogWindowHier.CtrlMapLOD_Entity.Name
                  SetAttributeFromAttribute( NewReport, "Report", "WebPrintPageEntity", DialogWindowHier, "CtrlMapLOD_Entity", "Name" );
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, DialogWindowHier )
      {StringBuilder sb_szReturnedEntityName;
      if ( szReturnedEntityName == null )
         sb_szReturnedEntityName = new StringBuilder( 32 );
      else
         sb_szReturnedEntityName = new StringBuilder( szReturnedEntityName );
      MutableInt mi_lReturnedLevel = new MutableInt( lReturnedLevel );
             nRC = SetCursorNextEntityHierarchical( mi_lReturnedLevel, sb_szReturnedEntityName, DialogWindowHier );
      szReturnedEntityName = sb_szReturnedEntityName.toString( );
      lReturnedLevel = mi_lReturnedLevel.intValue( );}
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
   CreateViewFromView( DialogWindowHier, DialogWindow );
   //:DefineHierarchicalCursor( DialogWindowHier, "Window" )
   DefineHierarchicalCursor( DialogWindowHier, "Window" );
   //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, DialogWindowHier )
   {StringBuilder sb_szReturnedEntityName;
   if ( szReturnedEntityName == null )
      sb_szReturnedEntityName = new StringBuilder( 32 );
   else
      sb_szReturnedEntityName = new StringBuilder( szReturnedEntityName );
   MutableInt mi_lReturnedLevel = new MutableInt( lReturnedLevel );
       nRC = SetCursorNextEntityHierarchical( mi_lReturnedLevel, sb_szReturnedEntityName, DialogWindowHier );
   szReturnedEntityName = sb_szReturnedEntityName.toString( );
   lReturnedLevel = mi_lReturnedLevel.intValue( );}
   //:LOOP WHILE nRC >= zCURSOR_SET AND szFoundFlag = "" AND szReturnedEntityName != "Window"
   while ( nRC >= zCURSOR_SET && ZeidonStringCompare( szFoundFlag, 1, 0, "", 1, 0, 2 ) == 0 && ZeidonStringCompare( szReturnedEntityName, 1, 0, "Window", 1, 0, 33 ) != 0 )
   { 
      //:IF szReturnedEntityName = "CtrlMapRelatedEntity"
      if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlMapRelatedEntity", 1, 0, 33 ) == 0 )
      { 
         //:szFoundFlag = "Y"
          {StringBuilder sb_szFoundFlag;
         if ( szFoundFlag == null )
            sb_szFoundFlag = new StringBuilder( 32 );
         else
            sb_szFoundFlag = new StringBuilder( szFoundFlag );
                  ZeidonStringCopy( sb_szFoundFlag, 1, 0, "Y", 1, 0, 2 );
         szFoundFlag = sb_szFoundFlag.toString( );}
         //:szFirstEntityName = DialogWindowHier.CtrlMapRelatedEntity.Name
         {MutableInt mi_lTempInteger_5 = new MutableInt( lTempInteger_5 );
         StringBuilder sb_szFirstEntityName;
         if ( szFirstEntityName == null )
            sb_szFirstEntityName = new StringBuilder( 32 );
         else
            sb_szFirstEntityName = new StringBuilder( szFirstEntityName );
                   GetVariableFromAttribute( sb_szFirstEntityName, mi_lTempInteger_5, 'S', 33, DialogWindowHier, "CtrlMapRelatedEntity", "Name", "", 0 );
         lTempInteger_5 = mi_lTempInteger_5.intValue( );
         szFirstEntityName = sb_szFirstEntityName.toString( );}
         //:ELSE
      } 
      else
      { 
         //:IF szReturnedEntityName = "CtrlMapLOD_Entity"
         if ( ZeidonStringCompare( szReturnedEntityName, 1, 0, "CtrlMapLOD_Entity", 1, 0, 33 ) == 0 )
         { 
            //:szFoundFlag = "Y"
             {StringBuilder sb_szFoundFlag;
            if ( szFoundFlag == null )
               sb_szFoundFlag = new StringBuilder( 32 );
            else
               sb_szFoundFlag = new StringBuilder( szFoundFlag );
                        ZeidonStringCopy( sb_szFoundFlag, 1, 0, "Y", 1, 0, 2 );
            szFoundFlag = sb_szFoundFlag.toString( );}
            //:IF DialogWindowHier.ControlDef.Tag = "SS"
            if ( CompareAttributeToString( DialogWindowHier, "ControlDef", "Tag", "SS" ) == 0 )
            { 
               //:szNoInitalGroupSetFlag = "Y"
                {StringBuilder sb_szNoInitalGroupSetFlag;
               if ( szNoInitalGroupSetFlag == null )
                  sb_szNoInitalGroupSetFlag = new StringBuilder( 32 );
               else
                  sb_szNoInitalGroupSetFlag = new StringBuilder( szNoInitalGroupSetFlag );
                              ZeidonStringCopy( sb_szNoInitalGroupSetFlag, 1, 0, "Y", 1, 0, 2 );
               szNoInitalGroupSetFlag = sb_szNoInitalGroupSetFlag.toString( );}
               //:ELSE
            } 
            else
            { 
               //:szFirstEntityName = DialogWindowHier.CtrlMapLOD_Entity.Name
               {MutableInt mi_lTempInteger_6 = new MutableInt( lTempInteger_6 );
               StringBuilder sb_szFirstEntityName;
               if ( szFirstEntityName == null )
                  sb_szFirstEntityName = new StringBuilder( 32 );
               else
                  sb_szFirstEntityName = new StringBuilder( szFirstEntityName );
                               GetVariableFromAttribute( sb_szFirstEntityName, mi_lTempInteger_6, 'S', 33, DialogWindowHier, "CtrlMapLOD_Entity", "Name", "", 0 );
               lTempInteger_6 = mi_lTempInteger_6.intValue( );
               szFirstEntityName = sb_szFirstEntityName.toString( );}
            } 

            //:END
         } 

         //:END
      } 

      //:END
      //:nRC = SetCursorNextEntityHierarchical( lReturnedLevel, szReturnedEntityName, DialogWindowHier )
      {StringBuilder sb_szReturnedEntityName;
      if ( szReturnedEntityName == null )
         sb_szReturnedEntityName = new StringBuilder( 32 );
      else
         sb_szReturnedEntityName = new StringBuilder( szReturnedEntityName );
      MutableInt mi_lReturnedLevel = new MutableInt( lReturnedLevel );
             nRC = SetCursorNextEntityHierarchical( mi_lReturnedLevel, sb_szReturnedEntityName, DialogWindowHier );
      szReturnedEntityName = sb_szReturnedEntityName.toString( );
      lReturnedLevel = mi_lReturnedLevel.intValue( );}
   } 

   //:END
   //:IF nRC < zCURSOR_SET
   if ( nRC < zCURSOR_SET )
   { 
      //:szMsg = "No mapped Controls exist for Window, " + DialogWindow.Window.Tag + "." + NEW_LINE +
      //:        "No report XRP will be generated."
      {MutableInt mi_lTempInteger_7 = new MutableInt( lTempInteger_7 );
      StringBuilder sb_szTempString_1;
      if ( szTempString_1 == null )
         sb_szTempString_1 = new StringBuilder( 32 );
      else
         sb_szTempString_1 = new StringBuilder( szTempString_1 );
             GetVariableFromAttribute( sb_szTempString_1, mi_lTempInteger_7, 'S', 33, DialogWindow, "Window", "Tag", "", 0 );
      lTempInteger_7 = mi_lTempInteger_7.intValue( );
      szTempString_1 = sb_szTempString_1.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "No mapped Controls exist for Window, ", 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, szTempString_1, 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, ".", 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, NEW_LINE, 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, "No report XRP will be generated.", 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
      //:MessageSend( DialogWindow, "", "Build Report", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( DialogWindow, "", "Build Report", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
   } 

   //:END
   //:IF szNoInitalGroupSetFlag = ""
   if ( ZeidonStringCompare( szNoInitalGroupSetFlag, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:CreateMetaEntity( DialogWindow, NewReport, "GroupSet", zPOS_AFTER )
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogWindow );
       m_TZCM_OPR_Operation.CreateMetaEntity( DialogWindow, NewReport, "GroupSet", zPOS_AFTER );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
      //:NewReport.GroupSet.Tag     = szFirstEntityName
      SetAttributeFromString( NewReport, "GroupSet", "Tag", szFirstEntityName );
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

      //:// Create a single Detail Group.
      //:CreateMetaEntity( DialogWindow, NewReport, "Group", zPOS_AFTER )
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( DialogWindow );
       m_TZCM_OPR_Operation.CreateMetaEntity( DialogWindow, NewReport, "Group", zPOS_AFTER );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
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
   oTZRPSRCO_GenerateXRP_Recurs( NewReport, DialogWindow, X_Position, Y_Position, GroupSize );
   //:NewReport.Group.SZDLG_Y = GroupSize
   SetAttributeFromInteger( NewReport, "Group", "SZDLG_Y", GroupSize );

   //:// Write out the XRP to a subdirectory of the LPLR.
   //:SysReadZeidonIni( -1, "[Workstation]", "HTML_Reports", szHTML_DirectoryName )
   {StringBuilder sb_szHTML_DirectoryName;
   if ( szHTML_DirectoryName == null )
      sb_szHTML_DirectoryName = new StringBuilder( 32 );
   else
      sb_szHTML_DirectoryName = new StringBuilder( szHTML_DirectoryName );
       m_KZOEP1AA.SysReadZeidonIni( -1, "[Workstation]", "HTML_Reports", sb_szHTML_DirectoryName );
   szHTML_DirectoryName = sb_szHTML_DirectoryName.toString( );}
   //:IF szHTML_DirectoryName = ""
   if ( ZeidonStringCompare( szHTML_DirectoryName, 1, 0, "", 1, 0, 257 ) == 0 )
   { 
      //:szHTML_DirectoryName = "\HTML_Reports\"
       {StringBuilder sb_szHTML_DirectoryName;
      if ( szHTML_DirectoryName == null )
         sb_szHTML_DirectoryName = new StringBuilder( 32 );
      else
         sb_szHTML_DirectoryName = new StringBuilder( szHTML_DirectoryName );
            ZeidonStringCopy( sb_szHTML_DirectoryName, 1, 0, "\\HTML_Reports\\", 1, 0, 257 );
      szHTML_DirectoryName = sb_szHTML_DirectoryName.toString( );}
   } 

   //:END

   //:GET VIEW vTaskLPLR NAMED "TaskLPLR"
   RESULT = GetViewByName( vTaskLPLR, "TaskLPLR", DialogWindow, zLEVEL_TASK );
   //:szDirectoryFileName = vTaskLPLR.LPLR.ExecDir + szHTML_DirectoryName + szReportName + ".XRP"
   {StringBuilder sb_szDirectoryFileName;
   if ( szDirectoryFileName == null )
      sb_szDirectoryFileName = new StringBuilder( 32 );
   else
      sb_szDirectoryFileName = new StringBuilder( szDirectoryFileName );
       GetStringFromAttribute( sb_szDirectoryFileName, vTaskLPLR, "LPLR", "ExecDir" );
   szDirectoryFileName = sb_szDirectoryFileName.toString( );}
    {StringBuilder sb_szDirectoryFileName;
   if ( szDirectoryFileName == null )
      sb_szDirectoryFileName = new StringBuilder( 32 );
   else
      sb_szDirectoryFileName = new StringBuilder( szDirectoryFileName );
      ZeidonStringConcat( sb_szDirectoryFileName, 1, 0, szHTML_DirectoryName, 1, 0, 501 );
   szDirectoryFileName = sb_szDirectoryFileName.toString( );}
    {StringBuilder sb_szDirectoryFileName;
   if ( szDirectoryFileName == null )
      sb_szDirectoryFileName = new StringBuilder( 32 );
   else
      sb_szDirectoryFileName = new StringBuilder( szDirectoryFileName );
      ZeidonStringConcat( sb_szDirectoryFileName, 1, 0, szReportName, 1, 0, 501 );
   szDirectoryFileName = sb_szDirectoryFileName.toString( );}
    {StringBuilder sb_szDirectoryFileName;
   if ( szDirectoryFileName == null )
      sb_szDirectoryFileName = new StringBuilder( 32 );
   else
      sb_szDirectoryFileName = new StringBuilder( szDirectoryFileName );
      ZeidonStringConcat( sb_szDirectoryFileName, 1, 0, ".XRP", 1, 0, 501 );
   szDirectoryFileName = sb_szDirectoryFileName.toString( );}

   //:CommitOI_ToFile( NewReport, szDirectoryFileName, zASCII )
   CommitOI_ToFile( NewReport, szDirectoryFileName, zASCII );
   //:ActivateOI_FromFile( TestReport, "TZRPSRCO", vTaskLPLR, szDirectoryFileName, zSINGLE )
   ActivateOI_FromFile( TestReport, "TZRPSRCO", vTaskLPLR, szDirectoryFileName, zSINGLE );
   //:NAME VIEW  TestReport "TestReport"
   SetNameForView( TestReport, "TestReport", null, zLEVEL_TASK );
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
public int 
oTZRPSRCO_ChangeRepFileContents( View     ViewToInstance,
                                 View     CurrentLPLR,
                                 String   szPathName1,
                                 String   szPathName2,
                                 String   szFileName,
                                 String   szFileExtension,
                                 int      nFunction,
                                 View     vSubtask )
{
   int      lFHandle = 0;
   //:INTEGER lFHandleTmp
   int      lFHandleTmp = 0;
   //:INTEGER lRC
   int      lRC = 0;
   //:INTEGER nLineNumber
   int      nLineNumber = 0;

   //:SHORT   nRC
   int      nRC = 0;
   //:SHORT   nLen
   int      nLen = 0;

   //:SHORT   nPosStart
   int      nPosStart = 0;
   //:SHORT   nPosBefore
   int      nPosBefore = 0;
   //:SHORT   nPosBeyond
   int      nPosBeyond = 0;

   //:STRING ( 2049 )  szMsg           // 2*zLONG_MESSAGE_LTH+1; szMsg includes szFullName
   String   szMsg = null;
   //:STRING ( 10000 ) szLine
   String   szLine = null;
   //:STRING ( 2049 )  szNewPath       // 4*zMAX_FILESPEC_LTH+1
   String   szNewPath = null;
   //:STRING ( 1025 )  szTmpFile       // 2*zMAX_FILESPEC_LTH+1
   String   szTmpFile = null;
   //:STRING ( 1025 )  szFullName      // 2*zMAX_FILESPEC_LTH+1
   String   szFullName = null;
   //:STRING ( 1025 )  szFullNameXSQ   // 2*zMAX_FILESPEC_LTH+1
   String   szFullNameXSQ = null;

   //:STRING ( 10 )    szOldName
   String   szOldName = null;
   String   szTempString_0 = null;
   int      lTempInteger_0 = 0;
   String   szTempString_1 = null;


   //:// Open the file which should be changed
   //:IF szFileExtension = "REP"
   if ( ZeidonStringCompare( szFileExtension, 1, 0, "REP", 1, 0, 5 ) == 0 )
   { 
      //:szFullName    = szPathName1 + "\" + szFileName + "." + szFileExtension
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringCopy( sb_szFullName, 1, 0, szPathName1, 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringConcat( sb_szFullName, 1, 0, "\\", 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringConcat( sb_szFullName, 1, 0, szFileName, 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringConcat( sb_szFullName, 1, 0, ".", 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringConcat( sb_szFullName, 1, 0, szFileExtension, 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
      //:szFullNameXSQ = szPathName2 + "\" + szFileName
       {StringBuilder sb_szFullNameXSQ;
      if ( szFullNameXSQ == null )
         sb_szFullNameXSQ = new StringBuilder( 32 );
      else
         sb_szFullNameXSQ = new StringBuilder( szFullNameXSQ );
            ZeidonStringCopy( sb_szFullNameXSQ, 1, 0, szPathName2, 1, 0, 1026 );
      szFullNameXSQ = sb_szFullNameXSQ.toString( );}
       {StringBuilder sb_szFullNameXSQ;
      if ( szFullNameXSQ == null )
         sb_szFullNameXSQ = new StringBuilder( 32 );
      else
         sb_szFullNameXSQ = new StringBuilder( szFullNameXSQ );
            ZeidonStringConcat( sb_szFullNameXSQ, 1, 0, "\\", 1, 0, 1026 );
      szFullNameXSQ = sb_szFullNameXSQ.toString( );}
       {StringBuilder sb_szFullNameXSQ;
      if ( szFullNameXSQ == null )
         sb_szFullNameXSQ = new StringBuilder( 32 );
      else
         sb_szFullNameXSQ = new StringBuilder( szFullNameXSQ );
            ZeidonStringConcat( sb_szFullNameXSQ, 1, 0, szFileName, 1, 0, 1026 );
      szFullNameXSQ = sb_szFullNameXSQ.toString( );}
      //:ELSE
   } 
   else
   { 
      //:szFullName = szPathName2 + "\" + szFileName + "." + szFileExtension
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringCopy( sb_szFullName, 1, 0, szPathName2, 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringConcat( sb_szFullName, 1, 0, "\\", 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringConcat( sb_szFullName, 1, 0, szFileName, 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringConcat( sb_szFullName, 1, 0, ".", 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
       {StringBuilder sb_szFullName;
      if ( szFullName == null )
         sb_szFullName = new StringBuilder( 32 );
      else
         sb_szFullName = new StringBuilder( szFullName );
            ZeidonStringConcat( sb_szFullName, 1, 0, szFileExtension, 1, 0, 1026 );
      szFullName = sb_szFullName.toString( );}
   } 

   //:END
   //:lFHandle = SysOpenFile( vSubtask, szFullName, COREFILE_READ )
   try
   {
       lFHandle = m_KZOEP1AA.SysOpenFile( vSubtask, szFullName, COREFILE_READ );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   //:IF lFHandle = -1
   if ( lFHandle == -1 )
   { 
      //:szMsg = "Cannot open File " + szFullName
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "Cannot open File ", 1, 0, 2050 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, szFullName, 1, 0, 2050 );
      szMsg = sb_szMsg.toString( );}
      //:MessageSend( vSubtask, "WD00501", "ChangeRepFileContents",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vSubtask, "WD00501", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
   } 

   //:END

   //:// We need a new file to save the changes
   //:szTmpFile = szPathName1 + "\" + "Temp" + "." + szFileExtension
    {StringBuilder sb_szTmpFile;
   if ( szTmpFile == null )
      sb_szTmpFile = new StringBuilder( 32 );
   else
      sb_szTmpFile = new StringBuilder( szTmpFile );
      ZeidonStringCopy( sb_szTmpFile, 1, 0, szPathName1, 1, 0, 1026 );
   szTmpFile = sb_szTmpFile.toString( );}
    {StringBuilder sb_szTmpFile;
   if ( szTmpFile == null )
      sb_szTmpFile = new StringBuilder( 32 );
   else
      sb_szTmpFile = new StringBuilder( szTmpFile );
      ZeidonStringConcat( sb_szTmpFile, 1, 0, "\\", 1, 0, 1026 );
   szTmpFile = sb_szTmpFile.toString( );}
    {StringBuilder sb_szTmpFile;
   if ( szTmpFile == null )
      sb_szTmpFile = new StringBuilder( 32 );
   else
      sb_szTmpFile = new StringBuilder( szTmpFile );
      ZeidonStringConcat( sb_szTmpFile, 1, 0, "Temp", 1, 0, 1026 );
   szTmpFile = sb_szTmpFile.toString( );}
    {StringBuilder sb_szTmpFile;
   if ( szTmpFile == null )
      sb_szTmpFile = new StringBuilder( 32 );
   else
      sb_szTmpFile = new StringBuilder( szTmpFile );
      ZeidonStringConcat( sb_szTmpFile, 1, 0, ".", 1, 0, 1026 );
   szTmpFile = sb_szTmpFile.toString( );}
    {StringBuilder sb_szTmpFile;
   if ( szTmpFile == null )
      sb_szTmpFile = new StringBuilder( 32 );
   else
      sb_szTmpFile = new StringBuilder( szTmpFile );
      ZeidonStringConcat( sb_szTmpFile, 1, 0, szFileExtension, 1, 0, 1026 );
   szTmpFile = sb_szTmpFile.toString( );}
   //:lFHandleTmp = SysOpenFile( vSubtask, szTmpFile, COREFILE_WRITE )
   try
   {
       lFHandleTmp = m_KZOEP1AA.SysOpenFile( vSubtask, szTmpFile, COREFILE_WRITE );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   //:IF lFHandleTmp = -1
   if ( lFHandleTmp == -1 )
   { 
      //:szMsg = "Cannot open temp. File " + szTmpFile
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "Cannot open temp. File ", 1, 0, 2050 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, szTmpFile, 1, 0, 2050 );
      szMsg = sb_szMsg.toString( );}
      //:MessageSend( vSubtask, "WD00502", "ChangeRepFileContents",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vSubtask, "WD00502", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
   } 

   //:END

   //:nLineNumber = 0
   nLineNumber = 0;
   //:lRC = zSysReadLine( vSubtask, szLine, lFHandle, 10000 )
   try
   {
       {StringBuilder sb_szLine;
   if ( szLine == null )
      sb_szLine = new StringBuilder( 32 );
   else
      sb_szLine = new StringBuilder( szLine );
       lRC = zSysReadLine( vSubtask, sb_szLine, lFHandle, 10000 );
   szLine = sb_szLine.toString( );}
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
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
            {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
            StringBuilder sb_szTempString_0;
            if ( szTempString_0 == null )
               sb_szTempString_0 = new StringBuilder( 32 );
            else
               sb_szTempString_0 = new StringBuilder( szTempString_0 );
                         GetVariableFromAttribute( sb_szTempString_0, mi_lTempInteger_0, 'S', 9, CurrentLPLR, "LPLR", "Name", "", 0 );
            lTempInteger_0 = mi_lTempInteger_0.intValue( );
            szTempString_0 = sb_szTempString_0.toString( );}
             {StringBuilder sb_szLine;
            if ( szLine == null )
               sb_szLine = new StringBuilder( 32 );
            else
               sb_szLine = new StringBuilder( szLine );
                        ZeidonStringCopy( sb_szLine, 1, 0, "LPLR = ", 1, 0, 10001 );
            szLine = sb_szLine.toString( );}
             {StringBuilder sb_szLine;
            if ( szLine == null )
               sb_szLine = new StringBuilder( 32 );
            else
               sb_szLine = new StringBuilder( szLine );
                        ZeidonStringConcat( sb_szLine, 1, 0, szTempString_0, 1, 0, 10001 );
            szLine = sb_szLine.toString( );}
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
                {StringBuilder sb_szNewPath;
               if ( szNewPath == null )
                  sb_szNewPath = new StringBuilder( 32 );
               else
                  sb_szNewPath = new StringBuilder( szNewPath );
                              ZeidonStringCopy( sb_szNewPath, 1, 0, szFullName, 1, 0, 2050 );
               szNewPath = sb_szNewPath.toString( );}
               //:zSearchAndReplace( szNewPath, 2049, "\", "\\")
               {StringBuilder sb_szNewPath;
               if ( szNewPath == null )
                  sb_szNewPath = new StringBuilder( 32 );
               else
                  sb_szNewPath = new StringBuilder( szNewPath );
                               zSearchAndReplace( sb_szNewPath, 2049, "\\", "\\\\" );
               szNewPath = sb_szNewPath.toString( );}
               //:nRC = zReplaceSubString( szLine, nPosBefore + 1, nPosBeyond, szNewPath )
               {StringBuilder sb_szLine;
               if ( szLine == null )
                  sb_szLine = new StringBuilder( 32 );
               else
                  sb_szLine = new StringBuilder( szLine );
                               nRC = zReplaceSubString( sb_szLine, nPosBefore + 1, nPosBeyond, szNewPath );
               szLine = sb_szLine.toString( );}
               //:IF nRC = -1
               if ( nRC == -1 )
               { 
                  //:szMsg = "Not enough memory to change the path in file " + szFullName
                   {StringBuilder sb_szMsg;
                  if ( szMsg == null )
                     sb_szMsg = new StringBuilder( 32 );
                  else
                     sb_szMsg = new StringBuilder( szMsg );
                                    ZeidonStringCopy( sb_szMsg, 1, 0, "Not enough memory to change the path in file ", 1, 0, 2050 );
                  szMsg = sb_szMsg.toString( );}
                   {StringBuilder sb_szMsg;
                  if ( szMsg == null )
                     sb_szMsg = new StringBuilder( 32 );
                  else
                     sb_szMsg = new StringBuilder( szMsg );
                                    ZeidonStringConcat( sb_szMsg, 1, 0, szFullName, 1, 0, 2050 );
                  szMsg = sb_szMsg.toString( );}
                  //:MessageSend( vSubtask, "WD00503", "ChangeRepFileContents",
                  //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "WD00503", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:RETURN -1
                  if(8==8)return( -1 );
               } 

               //:END

               //:// b. for *.XSQ-File
               //:// The filename needs extension "XSQ"
               //:szNewPath = szFullNameXSQ + "." + "XSQ"
                {StringBuilder sb_szNewPath;
               if ( szNewPath == null )
                  sb_szNewPath = new StringBuilder( 32 );
               else
                  sb_szNewPath = new StringBuilder( szNewPath );
                              ZeidonStringCopy( sb_szNewPath, 1, 0, szFullNameXSQ, 1, 0, 2050 );
               szNewPath = sb_szNewPath.toString( );}
                {StringBuilder sb_szNewPath;
               if ( szNewPath == null )
                  sb_szNewPath = new StringBuilder( 32 );
               else
                  sb_szNewPath = new StringBuilder( szNewPath );
                              ZeidonStringConcat( sb_szNewPath, 1, 0, ".", 1, 0, 2050 );
               szNewPath = sb_szNewPath.toString( );}
                {StringBuilder sb_szNewPath;
               if ( szNewPath == null )
                  sb_szNewPath = new StringBuilder( 32 );
               else
                  sb_szNewPath = new StringBuilder( szNewPath );
                              ZeidonStringConcat( sb_szNewPath, 1, 0, "XSQ", 1, 0, 2050 );
               szNewPath = sb_szNewPath.toString( );}
               //:zSearchAndReplace (szNewPath, 2049, "\", "\\")
               {StringBuilder sb_szNewPath;
               if ( szNewPath == null )
                  sb_szNewPath = new StringBuilder( 32 );
               else
                  sb_szNewPath = new StringBuilder( szNewPath );
                               zSearchAndReplace( sb_szNewPath, 2049, "\\", "\\\\" );
               szNewPath = sb_szNewPath.toString( );}

               //:nPosStart  = zSearchSubString( szLine, "XSQ", "f", 0 )
               nPosStart = zSearchSubString( szLine, "XSQ", "f", 0 );
               //:nPosBefore = zSearchSubString( szLine, ","  , "b", nPosStart )
               nPosBefore = zSearchSubString( szLine, ",", "b", nPosStart );
               //:nPosBeyond = zSearchSubString( szLine, ","  , "f", nPosStart )
               nPosBeyond = zSearchSubString( szLine, ",", "f", nPosStart );

               //:/* Replace the pathname */
               //:nRC = zReplaceSubString( szLine, nPosBefore + 1, nPosBeyond, szNewPath )
               {StringBuilder sb_szLine;
               if ( szLine == null )
                  sb_szLine = new StringBuilder( 32 );
               else
                  sb_szLine = new StringBuilder( szLine );
                               nRC = zReplaceSubString( sb_szLine, nPosBefore + 1, nPosBeyond, szNewPath );
               szLine = sb_szLine.toString( );}
               //:IF nRC = -1
               if ( nRC == -1 )
               { 
                  //:szMsg = "Not enough memory to change the path in file " + szFullName
                   {StringBuilder sb_szMsg;
                  if ( szMsg == null )
                     sb_szMsg = new StringBuilder( 32 );
                  else
                     sb_szMsg = new StringBuilder( szMsg );
                                    ZeidonStringCopy( sb_szMsg, 1, 0, "Not enough memory to change the path in file ", 1, 0, 2050 );
                  szMsg = sb_szMsg.toString( );}
                   {StringBuilder sb_szMsg;
                  if ( szMsg == null )
                     sb_szMsg = new StringBuilder( 32 );
                  else
                     sb_szMsg = new StringBuilder( szMsg );
                                    ZeidonStringConcat( sb_szMsg, 1, 0, szFullName, 1, 0, 2050 );
                  szMsg = sb_szMsg.toString( );}
                  //:MessageSend( vSubtask, "WD00504", "ChangeRepFileContents",
                  //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "WD00504", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:RETURN -1
                  if(8==8)return( -1 );
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
                  {StringBuilder sb_szOldName;
                  if ( szOldName == null )
                     sb_szOldName = new StringBuilder( 32 );
                  else
                     sb_szOldName = new StringBuilder( szOldName );
                                     lRC = ZeidonStringCopy( sb_szOldName, 1, 0, szLine, nPosStart + 1, nLen, 10 );
                  szOldName = sb_szOldName.toString( );}
                  //:IF lRC < 0
                  if ( lRC < 0 )
                  { 
                     //:RETURN -1
                     if(8==8)return( -1 );
                  } 

                  //:END
                  //:nRC = zReplaceSubString( szLine, nPosStart, nPosBeyond, szFileName )
                  {StringBuilder sb_szLine;
                  if ( szLine == null )
                     sb_szLine = new StringBuilder( 32 );
                  else
                     sb_szLine = new StringBuilder( szLine );
                                     nRC = zReplaceSubString( sb_szLine, nPosStart, nPosBeyond, szFileName );
                  szLine = sb_szLine.toString( );}
                  //:IF nRC = -1
                  if ( nRC == -1 )
                  { 
                     //:szMsg = "Not enough memory to change the reportname in file " + szFullName
                      {StringBuilder sb_szMsg;
                     if ( szMsg == null )
                        sb_szMsg = new StringBuilder( 32 );
                     else
                        sb_szMsg = new StringBuilder( szMsg );
                                          ZeidonStringCopy( sb_szMsg, 1, 0, "Not enough memory to change the reportname in file ", 1, 0, 2050 );
                     szMsg = sb_szMsg.toString( );}
                      {StringBuilder sb_szMsg;
                     if ( szMsg == null )
                        sb_szMsg = new StringBuilder( 32 );
                     else
                        sb_szMsg = new StringBuilder( szMsg );
                                          ZeidonStringConcat( sb_szMsg, 1, 0, szFullName, 1, 0, 2050 );
                     szMsg = sb_szMsg.toString( );}
                     //:MessageSend( vSubtask, "WD00506", "ChangeRepFileContents",
                     //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                     MessageSend( vSubtask, "WD00506", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                     //:RETURN -1
                     if(8==8)return( -1 );
                  } 

                  //:END
                  //:// We have to find the reportname the second time between the parenthese in line 1
                  //:nPosStart  = zSearchSubString( szLine, szOldName, "f", 0 )
                  nPosStart = zSearchSubString( szLine, szOldName, "f", 0 );
                  //:nPosBeyond = zSearchSubString( szLine, ";", "f", nPosStart )
                  nPosBeyond = zSearchSubString( szLine, ";", "f", nPosStart );
                  //:nRC = zReplaceSubString( szLine, nPosStart, nPosBeyond, szFileName )
                  {StringBuilder sb_szLine;
                  if ( szLine == null )
                     sb_szLine = new StringBuilder( 32 );
                  else
                     sb_szLine = new StringBuilder( szLine );
                                     nRC = zReplaceSubString( sb_szLine, nPosStart, nPosBeyond, szFileName );
                  szLine = sb_szLine.toString( );}
                  //:IF nRC = -1
                  if ( nRC == -1 )
                  { 
                     //:szMsg = "Not enough memory to change the reportname in file " + szFullName
                      {StringBuilder sb_szMsg;
                     if ( szMsg == null )
                        sb_szMsg = new StringBuilder( 32 );
                     else
                        sb_szMsg = new StringBuilder( szMsg );
                                          ZeidonStringCopy( sb_szMsg, 1, 0, "Not enough memory to change the reportname in file ", 1, 0, 2050 );
                     szMsg = sb_szMsg.toString( );}
                      {StringBuilder sb_szMsg;
                     if ( szMsg == null )
                        sb_szMsg = new StringBuilder( 32 );
                     else
                        sb_szMsg = new StringBuilder( szMsg );
                                          ZeidonStringConcat( sb_szMsg, 1, 0, szFullName, 1, 0, 2050 );
                     szMsg = sb_szMsg.toString( );}
                     //:MessageSend( vSubtask, "WD00507", "ChangeRepFileContents",
                     //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                     MessageSend( vSubtask, "WD00507", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                     //:RETURN -1
                     if(8==8)return( -1 );
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
               {StringBuilder sb_szTempString_1;
               if ( szTempString_1 == null )
                  sb_szTempString_1 = new StringBuilder( 32 );
               else
                  sb_szTempString_1 = new StringBuilder( szTempString_1 );
                               GetStringFromAttribute( sb_szTempString_1, CurrentLPLR, "LPLR", "Name" );
               szTempString_1 = sb_szTempString_1.toString( );}
               {StringBuilder sb_szLine;
               if ( szLine == null )
                  sb_szLine = new StringBuilder( 32 );
               else
                  sb_szLine = new StringBuilder( szLine );
                               nRC = zReplaceSubString( sb_szLine, nPosBefore + 1, nPosStart, szTempString_1 );
               szLine = sb_szLine.toString( );}
               //:IF nRC = -1
               if ( nRC == -1 )
               { 
                  //:szMsg = "Not enough memory to change the LPLR name in file " + szFullName
                   {StringBuilder sb_szMsg;
                  if ( szMsg == null )
                     sb_szMsg = new StringBuilder( 32 );
                  else
                     sb_szMsg = new StringBuilder( szMsg );
                                    ZeidonStringCopy( sb_szMsg, 1, 0, "Not enough memory to change the LPLR name in file ", 1, 0, 2050 );
                  szMsg = sb_szMsg.toString( );}
                   {StringBuilder sb_szMsg;
                  if ( szMsg == null )
                     sb_szMsg = new StringBuilder( 32 );
                  else
                     sb_szMsg = new StringBuilder( szMsg );
                                    ZeidonStringConcat( sb_szMsg, 1, 0, szFullName, 1, 0, 2050 );
                  szMsg = sb_szMsg.toString( );}
                  //:MessageSend( vSubtask, "WD00505", "ChangeRepFileContents",
                  //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                  MessageSend( vSubtask, "WD00505", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                  //:RETURN -1
                  if(8==8)return( -1 );
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
                     {StringBuilder sb_szLine;
                     if ( szLine == null )
                        sb_szLine = new StringBuilder( 32 );
                     else
                        sb_szLine = new StringBuilder( szLine );
                                           nRC = zReplaceSubString( sb_szLine, nPosStart, nPosBeyond, szFileName );
                     szLine = sb_szLine.toString( );}
                     //:IF nRC = -1
                     if ( nRC == -1 )
                     { 
                        //:szMsg = "Not enough memory to change the reportname in file " + szFullName
                         {StringBuilder sb_szMsg;
                        if ( szMsg == null )
                           sb_szMsg = new StringBuilder( 32 );
                        else
                           sb_szMsg = new StringBuilder( szMsg );
                                                ZeidonStringCopy( sb_szMsg, 1, 0, "Not enough memory to change the reportname in file ", 1, 0, 2050 );
                        szMsg = sb_szMsg.toString( );}
                         {StringBuilder sb_szMsg;
                        if ( szMsg == null )
                           sb_szMsg = new StringBuilder( 32 );
                        else
                           sb_szMsg = new StringBuilder( szMsg );
                                                ZeidonStringConcat( sb_szMsg, 1, 0, szFullName, 1, 0, 2050 );
                        szMsg = sb_szMsg.toString( );}
                        //:MessageSend( vSubtask, "WD00508", "ChangeRepFileContents",
                        //:          szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
                        MessageSend( vSubtask, "WD00508", "ChangeRepFileContents", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
                        //:RETURN -1
                        if(8==8)return( -1 );
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
      try
      {
          m_KZOEP1AA.SysWriteLine( vSubtask, lFHandleTmp, szLine );
      }
      catch ( Exception e )
      {
         throw ZeidonException.wrapException( e );
      }
      //:lRC = zSysReadLine( vSubtask, szLine, lFHandle, 10000 )
      try
      {
          {StringBuilder sb_szLine;
      if ( szLine == null )
         sb_szLine = new StringBuilder( 32 );
      else
         sb_szLine = new StringBuilder( szLine );
             lRC = zSysReadLine( vSubtask, sb_szLine, lFHandle, 10000 );
      szLine = sb_szLine.toString( );}
      }
      catch ( Exception e )
      {
         throw ZeidonException.wrapException( e );
      }
   } 


   //:END

   //:SysCloseFile( vSubtask, lFHandle, 0 )
   try
   {
       m_KZOEP1AA.SysCloseFile( vSubtask, lFHandle, null );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   //:SysCloseFile( vSubtask, lFHandleTmp, 0 )
   try
   {
       m_KZOEP1AA.SysCloseFile( vSubtask, lFHandleTmp, null );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }

   //:// The changes are now in the temporary file. Rename it for the system
   //:SysRenameFile( vSubtask, szTmpFile, szFullName, TRUE )
   m_KZOEP1AA.SysRenameFile( vSubtask, szTmpFile, szFullName, TRUE );
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
private int 
oTZRPSRCO_CopyControl( View     NewR,
                       View     NewRC,
                       View     OrigR,
                       View     OrigRC,
                       View     vPE,
                       View     SourceLPLR,
                       View     vSubtask )
{
   int      nRC = 0;
   //:STRING (128) szMsg
   String   szMsg = null;
   int      RESULT = 0;
   String   szTempString_0 = null;
   int      lTempInteger_0 = 0;


   //:// Note that the views that are passed are considered as TZWDLGSO objects.
   //:// This is so that we can use the CloneCtrlMap operation for the TZWDLGSO LOD. Since
   //:// the subobject is the same, there is no problem in assuming the object instance
   //:// passed is of that LOD type.

   //:CreateMetaEntity( vSubtask, NewRC, "Control", zPOS_AFTER )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, NewRC, "Control", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
   //:SetMatchingAttributesByName( NewRC,  "Control",
   //:                             OrigRC, "Control", zSET_NULL )
   SetMatchingAttributesByName( NewRC, "Control", OrigRC, "Control", zSET_NULL );

   //:SET CURSOR FIRST vPE.ControlDef WHERE
   //:                 vPE.ControlDef.Tag = OrigRC.ControlDef.Tag
   {StringBuilder sb_szTempString_0;
   if ( szTempString_0 == null )
      sb_szTempString_0 = new StringBuilder( 32 );
   else
      sb_szTempString_0 = new StringBuilder( szTempString_0 );
       GetStringFromAttribute( sb_szTempString_0, OrigRC, "ControlDef", "Tag" );
   szTempString_0 = sb_szTempString_0.toString( );}
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
         {
          TZWDLGSO_Object m_TZWDLGSO_Object = new TZWDLGSO_Object( NewR );
          nRC = m_TZWDLGSO_Object.oTZWDLGSO_CloneCtrlMap( NewR, NewRC, OrigR, OrigRC, SourceLPLR, vSubtask );
          // m_TZWDLGSO_Object = null;  // permit gc  (unnecessary)
         }
         //:IF nRC = -1
         if ( nRC == -1 )
         { 
            //:RETURN -1
            if(8==8)return( -1 );
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
      {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
      StringBuilder sb_szTempString_0;
      if ( szTempString_0 == null )
         sb_szTempString_0 = new StringBuilder( 32 );
      else
         sb_szTempString_0 = new StringBuilder( szTempString_0 );
             GetVariableFromAttribute( sb_szTempString_0, mi_lTempInteger_0, 'S', 33, NewRC, "Control", "Tag", "", 0 );
      lTempInteger_0 = mi_lTempInteger_0.intValue( );
      szTempString_0 = sb_szTempString_0.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "ControlDef not found for Control: ", 1, 0, 129 );
      szMsg = sb_szMsg.toString( );}
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringConcat( sb_szMsg, 1, 0, szTempString_0, 1, 0, 129 );
      szMsg = sb_szMsg.toString( );}
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
public int 
oTZRPSRCO_ReportCopy( zVIEW    NewReport,
                      View     OrigReport,
                      View     SourceLPLR,
                      View     vSubtask )
{
   zVIEW    NewReportRecursive = new zVIEW( );
   //:VIEW OrigReportRecursive  BASED ON LOD TZRPSRCO
   zVIEW    OrigReportRecursive = new zVIEW( );
   //:VIEW NewDialog            BASED ON LOD TZWDLGSO
   zVIEW    NewDialog = new zVIEW( );
   //:VIEW vPE                  BASED ON LOD TZPESRCO
   zVIEW    vPE = new zVIEW( );
   //:VIEW PE_List              BASED ON LOD TZCMLPLO
   zVIEW    PE_List = new zVIEW( );
   //:STRING (100)   szMG_ErrorMessage
   String   szMG_ErrorMessage = null;
   //:STRING ( 33)   szReportName
   String   szReportName = null;
   //:SHORT          nRC
   int      nRC = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      RESULT = 0;
   String   szTempString_0 = null;


   //:// Create the new report empty and add the root.
   //:ActivateEmptyMetaOI( vSubtask, NewReport, zSOURCE_REPORT_META, zSINGLE )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.ActivateEmptyMetaOI( vSubtask, NewReport, zSOURCE_REPORT_META, zSINGLE );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
   //:NAME VIEW NewReport "NewReport"
   SetNameForView( NewReport, "NewReport", null, zLEVEL_TASK );
   //:CreateMetaEntity( vSubtask, NewReport, "Report", zPOS_AFTER )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, NewReport, "Report", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
   //:SetMatchingAttributesByName( NewReport,  "Report",
   //:                             OrigReport, "Report", zSET_NULL )
   SetMatchingAttributesByName( NewReport, "Report", OrigReport, "Report", zSET_NULL );

   //://  Get View to Presentation Environment.
   //:IF GetViewByName( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) < 0
   lTempInteger_0 = GetViewByName( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   if ( lTempInteger_0 < 0 )
   { 
      //:RetrieveViewForMetaList( vSubtask, PE_List, zREFER_PENV_META )
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       m_TZCM_OPR_Operation.RetrieveViewForMetaList( vSubtask, PE_List, zREFER_PENV_META );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
      //:szReportName = OrigReport.Report.Tag
      {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
      StringBuilder sb_szReportName;
      if ( szReportName == null )
         sb_szReportName = new StringBuilder( 32 );
      else
         sb_szReportName = new StringBuilder( szReportName );
             GetVariableFromAttribute( sb_szReportName, mi_lTempInteger_1, 'S', 34, OrigReport, "Report", "Tag", "", 0 );
      lTempInteger_1 = mi_lTempInteger_1.intValue( );
      szReportName = sb_szReportName.toString( );}
      //:szMG_ErrorMessage = "Report (" + szReportName + ") aborted."
       {StringBuilder sb_szMG_ErrorMessage;
      if ( szMG_ErrorMessage == null )
         sb_szMG_ErrorMessage = new StringBuilder( 32 );
      else
         sb_szMG_ErrorMessage = new StringBuilder( szMG_ErrorMessage );
            ZeidonStringCopy( sb_szMG_ErrorMessage, 1, 0, "Report (", 1, 0, 101 );
      szMG_ErrorMessage = sb_szMG_ErrorMessage.toString( );}
       {StringBuilder sb_szMG_ErrorMessage;
      if ( szMG_ErrorMessage == null )
         sb_szMG_ErrorMessage = new StringBuilder( 32 );
      else
         sb_szMG_ErrorMessage = new StringBuilder( szMG_ErrorMessage );
            ZeidonStringConcat( sb_szMG_ErrorMessage, 1, 0, szReportName, 1, 0, 101 );
      szMG_ErrorMessage = sb_szMG_ErrorMessage.toString( );}
       {StringBuilder sb_szMG_ErrorMessage;
      if ( szMG_ErrorMessage == null )
         sb_szMG_ErrorMessage = new StringBuilder( 32 );
      else
         sb_szMG_ErrorMessage = new StringBuilder( szMG_ErrorMessage );
            ZeidonStringConcat( sb_szMG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 101 );
      szMG_ErrorMessage = sb_szMG_ErrorMessage.toString( );}
      //:nRC = LoadZeidonPPE( vSubtask, vPE, zREFER_PENV_META, PE_List,
      //:                     "Configuration Management", szMG_ErrorMessage )
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       nRC = m_TZCM_OPR_Operation.LoadZeidonPPE( vSubtask, vPE, zREFER_PENV_META, PE_List, "Configuration Management", szMG_ErrorMessage );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
      //:DropView( PE_List )
      DropView( PE_List );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN -16
         if(8==8)return( -16 );
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
      SetViewFromView( NewDialog, NewReport );
      //:PositionOnVOR( NewDialog,  OrigReport,
      //:               SourceLPLR, OrigReport.ViewObjRef.Name, vSubtask )
      {StringBuilder sb_szTempString_0;
      if ( szTempString_0 == null )
         sb_szTempString_0 = new StringBuilder( 32 );
      else
         sb_szTempString_0 = new StringBuilder( szTempString_0 );
             GetStringFromAttribute( sb_szTempString_0, OrigReport, "ViewObjRef", "Name" );
      szTempString_0 = sb_szTempString_0.toString( );}
      {
       TZWDLGSO_Object m_TZWDLGSO_Object = new TZWDLGSO_Object( NewDialog );
       m_TZWDLGSO_Object.oTZWDLGSO_PositionOnVOR( NewDialog, OrigReport, SourceLPLR, szTempString_0, vSubtask );
       // m_TZWDLGSO_Object = null;  // permit gc  (unnecessary)
      }
      RESULT = SetCursorNextEntity( OrigReport, "ViewObjRef", "" );
   } 

   //:END

   //:// Build the Page substructure.
   //:FOR EACH OrigReport.Page
   RESULT = SetCursorFirstEntity( OrigReport, "Page", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:CreateMetaEntity( vSubtask, NewReport, "Page", zPOS_AFTER )
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, NewReport, "Page", zPOS_AFTER );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
      //:SetMatchingAttributesByName( NewReport,  "Page",
      //:                             OrigReport, "Page", zSET_NULL )
      SetMatchingAttributesByName( NewReport, "Page", OrigReport, "Page", zSET_NULL );
      //:NewReport.Page.Tag = NewReport.Report.Tag
      SetAttributeFromAttribute( NewReport, "Page", "Tag", NewReport, "Report", "Tag" );

      //:FOR EACH OrigReport.GroupSet
      RESULT = SetCursorFirstEntity( OrigReport, "GroupSet", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:CreateMetaEntity( vSubtask, NewReport, "GroupSet", zPOS_AFTER )
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, NewReport, "GroupSet", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:SetMatchingAttributesByName( NewReport,  "GroupSet",
         //:                             OrigReport, "GroupSet", zSET_NULL )
         SetMatchingAttributesByName( NewReport, "GroupSet", OrigReport, "GroupSet", zSET_NULL );

         //:FOR EACH OrigReport.Group
         RESULT = SetCursorFirstEntity( OrigReport, "Group", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 

            //:CreateMetaEntity( vSubtask, NewReport, "Group", zPOS_AFTER )
            {
             TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
             m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, NewReport, "Group", zPOS_AFTER );
             // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
            }
            //:SetMatchingAttributesByName( NewReport,  "Group",
            //:                             OrigReport, "Group", zSET_NULL )
            SetMatchingAttributesByName( NewReport, "Group", OrigReport, "Group", zSET_NULL );
            //:FOR EACH OrigReport.Control
            RESULT = SetCursorFirstEntity( OrigReport, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:CreateViewFromViewForTask( NewReportRecursive, NewReport, 0 )
               CreateViewFromViewForTask( NewReportRecursive, NewReport, null );
               //:CreateViewFromViewForTask( OrigReportRecursive, OrigReport, 0 )
               CreateViewFromViewForTask( OrigReportRecursive, OrigReport, null );
               //:CopyControl( NewReport,
               //:             NewReportRecursive,
               //:             OrigReport,
               //:             OrigReportRecursive,
               //:             vPE,
               //:             SourceLPLR,
               //:             vSubtask )
               oTZRPSRCO_CopyControl( NewReport, NewReportRecursive, OrigReport, OrigReportRecursive, vPE, SourceLPLR, vSubtask );
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
public int 
oTZRPSRCO_ReportRelinkDelete( View     vReport,
                              View     vSubtask )
{
   zVIEW    vLastLOD = new zVIEW( );
   //:VIEW         vVOR         BASED ON LOD TZWDVORO
   zVIEW    vVOR = new zVIEW( );
   //:VIEW         vLOD_LPLR    BASED ON LOD TZCMLPLO
   zVIEW    vLOD_LPLR = new zVIEW( );
   //:VIEW         vRecursive   BASED ON LOD TZRPSRCO
   zVIEW    vRecursive = new zVIEW( );
   //:VIEW         vDialog      BASED ON LOD TZWDLGSO
   zVIEW    vDialog = new zVIEW( );
   //:INTEGER      lLastViewZKey
   int      lLastViewZKey = 0;
   //:STRING (128) szMsg
   String   szMsg = null;
   //:SHORT        nRC
   int      nRC = 0;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;
   String   szTempString_0 = null;
   String   szTempString_1 = null;
   int      lTempInteger_1 = 0;
   String   szTempString_2 = null;
   int      lTempInteger_2 = 0;
   String   szTempString_3 = null;
   String   szTempString_4 = null;
   int      lTempInteger_3 = 0;


   //:RetrieveViewForMetaList( vSubtask, vLOD_LPLR, zREFER_LOD_META )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.RetrieveViewForMetaList( vSubtask, vLOD_LPLR, zREFER_LOD_META );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
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
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.DropMetaOI( vSubtask, vLastLOD );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:lLastViewZKey = 0
         lLastViewZKey = 0;
      } 

      //:END

      //:nRC = ActivateMetaOI_ByZKey( vSubtask, vVOR, 0, zREFER_VOR_META,
      //:                             zSINGLE, vReport.ViewObjRef.ZKey, 0 )
      {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
             GetIntegerFromAttribute( mi_lTempInteger_0, vReport, "ViewObjRef", "ZKey" );
      lTempInteger_0 = mi_lTempInteger_0.intValue( );}
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       nRC = m_TZCM_OPR_Operation.ActivateMetaOI_ByZKey( vSubtask, vVOR, null, zREFER_VOR_META, zSINGLE, lTempInteger_0, 0 );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:nRC = ActivateMetaOI_ByName( vSubtask, vLastLOD, 0, zREFER_LOD_META,
         //:                             zSINGLE, vVOR.LOD.Name, 0 )
         {StringBuilder sb_szTempString_0;
         if ( szTempString_0 == null )
            sb_szTempString_0 = new StringBuilder( 32 );
         else
            sb_szTempString_0 = new StringBuilder( szTempString_0 );
                   GetStringFromAttribute( sb_szTempString_0, vVOR, "LOD", "Name" );
         szTempString_0 = sb_szTempString_0.toString( );}
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          nRC = m_TZCM_OPR_Operation.ActivateMetaOI_ByName( vSubtask, vLastLOD, null, zREFER_LOD_META, zSINGLE, szTempString_0, 0 );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:lLastViewZKey = vReport.ViewObjRef.ZKey
            {MutableInt mi_lLastViewZKey = new MutableInt( lLastViewZKey );
                         GetIntegerFromAttribute( mi_lLastViewZKey, vReport, "ViewObjRef", "ZKey" );
            lLastViewZKey = mi_lLastViewZKey.intValue( );}
            //:NAME VIEW vLastLOD "vLastLOD"
            SetNameForView( vLastLOD, "vLastLOD", null, zLEVEL_TASK );
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
            {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
            StringBuilder sb_szTempString_1;
            if ( szTempString_1 == null )
               sb_szTempString_1 = new StringBuilder( 32 );
            else
               sb_szTempString_1 = new StringBuilder( szTempString_1 );
                         GetVariableFromAttribute( sb_szTempString_1, mi_lTempInteger_1, 'S', 9, vReport, "ViewObjRef", "Name", "", 0 );
            lTempInteger_1 = mi_lTempInteger_1.intValue( );
            szTempString_1 = sb_szTempString_1.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringCopy( sb_szMsg, 1, 0, "Deleting Report View: ", 1, 0, 129 );
            szMsg = sb_szMsg.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, szTempString_1, 1, 0, 129 );
            szMsg = sb_szMsg.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, "Missing LOD: ", 1, 0, 129 );
            szMsg = sb_szMsg.toString( );}
            {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
            StringBuilder sb_szTempString_2;
            if ( szTempString_2 == null )
               sb_szTempString_2 = new StringBuilder( 32 );
            else
               sb_szTempString_2 = new StringBuilder( szTempString_2 );
                         GetVariableFromAttribute( sb_szTempString_2, mi_lTempInteger_2, 'S', 9, vReport, "LOD", "Name", "", 0 );
            lTempInteger_2 = mi_lTempInteger_2.intValue( );
            szTempString_2 = sb_szTempString_2.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, szTempString_2, 1, 0, 129 );
            szMsg = sb_szMsg.toString( );}
            //:MessageSend( vSubtask, "WD00501", "Report Relink",
            //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "WD00501", "Report Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:EXCLUDE vReport.ViewObjRef NONE
            RESULT = ExcludeEntity( vReport, "ViewObjRef", zREPOS_NONE );
         } 

         //:END
         //:DropMetaOI( vSubtask, vVOR )
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.DropMetaOI( vSubtask, vVOR );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:ELSE
      } 
      else
      { 
         //:nRC = ActivateMetaOI_ByName( vSubtask, vVOR, 0, zREFER_VOR_META,
         //:                             zSINGLE, vReport.ViewObjRef.Name , 0 )
         {StringBuilder sb_szTempString_3;
         if ( szTempString_3 == null )
            sb_szTempString_3 = new StringBuilder( 32 );
         else
            sb_szTempString_3 = new StringBuilder( szTempString_3 );
                   GetStringFromAttribute( sb_szTempString_3, vReport, "ViewObjRef", "Name" );
         szTempString_3 = sb_szTempString_3.toString( );}
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          nRC = m_TZCM_OPR_Operation.ActivateMetaOI_ByName( vSubtask, vVOR, null, zREFER_VOR_META, zSINGLE, szTempString_3, 0 );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:EXCLUDE vReport.ViewObjRef NONE
            RESULT = ExcludeEntity( vReport, "ViewObjRef", zREPOS_NONE );
            //:INCLUDE vReport.ViewObjRef FROM vVOR.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( vReport, "ViewObjRef", vVOR, "ViewObjRef", zPOS_AFTER );
            //:DropMetaOI( vSubtask, vVOR )
            {
             TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
             m_TZCM_OPR_Operation.DropMetaOI( vSubtask, vVOR );
             // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
            }
            //:ELSE
         } 
         else
         { 
            //:szMsg = "Deleting Report View: " + vReport.ViewObjRef.Name +
            //:        "|Missing Registered View."
            {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
            StringBuilder sb_szTempString_4;
            if ( szTempString_4 == null )
               sb_szTempString_4 = new StringBuilder( 32 );
            else
               sb_szTempString_4 = new StringBuilder( szTempString_4 );
                         GetVariableFromAttribute( sb_szTempString_4, mi_lTempInteger_3, 'S', 33, vReport, "ViewObjRef", "Name", "", 0 );
            lTempInteger_3 = mi_lTempInteger_3.intValue( );
            szTempString_4 = sb_szTempString_4.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringCopy( sb_szMsg, 1, 0, "Deleting Report View: ", 1, 0, 129 );
            szMsg = sb_szMsg.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, szTempString_4, 1, 0, 129 );
            szMsg = sb_szMsg.toString( );}
             {StringBuilder sb_szMsg;
            if ( szMsg == null )
               sb_szMsg = new StringBuilder( 32 );
            else
               sb_szMsg = new StringBuilder( szMsg );
                        ZeidonStringConcat( sb_szMsg, 1, 0, "Missing Registered View.", 1, 0, 129 );
            szMsg = sb_szMsg.toString( );}
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
      CreateViewFromViewForTask( vRecursive, vReport, null );
      //:NAME VIEW vRecursive "vRecursive"
      SetNameForView( vRecursive, "vRecursive", null, zLEVEL_TASK );
      //:FOR EACH vRecursive.Control
      RESULT = SetCursorFirstEntity( vRecursive, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:// Use Dialog view in ControlRelinkDelete call so that operation is
         //:// found under the TZWDLGSO object.
         //:vDialog = vReport
         SetViewFromView( vDialog, vReport );
         //:ControlRelinkDelete( vDialog, vRecursive, vLastLOD,
         //:                     lLastViewZKey, vLOD_LPLR, vSubtask )
         {
          TZWDLGSO_Object m_TZWDLGSO_Object = new TZWDLGSO_Object( vDialog );
          {MutableInt mi_lLastViewZKey = new MutableInt( lLastViewZKey );
                   m_TZWDLGSO_Object.oTZWDLGSO_ControlRelinkDelete( vDialog, vRecursive, vLastLOD, mi_lLastViewZKey, vLOD_LPLR, vSubtask );
         lLastViewZKey = mi_lLastViewZKey.intValue( );}
          // m_TZWDLGSO_Object = null;  // permit gc  (unnecessary)
         }
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
public int 
oTZRPSRCO_ReportMigrate( zVIEW    NewReport,
                         String   ReportName,
                         View     SourceLPLR,
                         View     vSubtask )
{
   zVIEW    SourceLPLR2 = new zVIEW( );
   //:VIEW CurrentLPLR BASED ON LOD TZCMLPLO
   zVIEW    CurrentLPLR = new zVIEW( );
   //:VIEW OrigReport  BASED ON LOD TZRPSRCO
   zVIEW    OrigReport = new zVIEW( );

   //:STRING ( 513 ) SourceFileName           // zMAX_FILESPEC_LTH + 1
   String   SourceFileName = null;
   //:STRING ( 32 )  SourceName
   String   SourceName = null;
   //:STRING ( 513 ) SourceFileName1          // zMAX_FILESPEC_LTH + 1
   String   SourceFileName1 = null;
   //:STRING ( 513 ) SourceFileName2          // zMAX_FILESPEC_LTH + 1
   String   SourceFileName2 = null;
   //:SHORT          nRC
   int      nRC = 0;
   int      lTempInteger_0 = 0;
   String   szTempString_0 = null;
   String   szTempString_1 = null;
   String   szTempString_2 = null;
   String   szTempString_3 = null;


   //:// Activate existing source meta OrigReport
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + ReportName + ".PRP"
   {StringBuilder sb_SourceFileName;
   if ( SourceFileName == null )
      sb_SourceFileName = new StringBuilder( 32 );
   else
      sb_SourceFileName = new StringBuilder( SourceFileName );
       GetStringFromAttribute( sb_SourceFileName, SourceLPLR, "LPLR", "MetaSrcDir" );
   SourceFileName = sb_SourceFileName.toString( );}
    {StringBuilder sb_SourceFileName;
   if ( SourceFileName == null )
      sb_SourceFileName = new StringBuilder( 32 );
   else
      sb_SourceFileName = new StringBuilder( SourceFileName );
      ZeidonStringConcat( sb_SourceFileName, 1, 0, "\\", 1, 0, 514 );
   SourceFileName = sb_SourceFileName.toString( );}
    {StringBuilder sb_SourceFileName;
   if ( SourceFileName == null )
      sb_SourceFileName = new StringBuilder( 32 );
   else
      sb_SourceFileName = new StringBuilder( SourceFileName );
      ZeidonStringConcat( sb_SourceFileName, 1, 0, ReportName, 1, 0, 514 );
   SourceFileName = sb_SourceFileName.toString( );}
    {StringBuilder sb_SourceFileName;
   if ( SourceFileName == null )
      sb_SourceFileName = new StringBuilder( 32 );
   else
      sb_SourceFileName = new StringBuilder( SourceFileName );
      ZeidonStringConcat( sb_SourceFileName, 1, 0, ".PRP", 1, 0, 514 );
   SourceFileName = sb_SourceFileName.toString( );}

   //:ActivateOI_FromFile( OrigReport, "TZRPSRCO", SourceLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( OrigReport, "TZRPSRCO", SourceLPLR, SourceFileName, 8192 );
   //:// 8192 is zIGNORE_ATTRIB_ERRORS
   //:NAME VIEW OrigReport "OrigReport"
   SetNameForView( OrigReport, "OrigReport", null, zLEVEL_TASK );

   //:CreateViewFromViewForTask( SourceLPLR2, SourceLPLR, 0 )
   CreateViewFromViewForTask( SourceLPLR2, SourceLPLR, null );

   //:// Call operation that will actually do the work of creating the report.
   //:nRC = ReportCopy( NewReport, OrigReport, SourceLPLR2, vSubtask )
   nRC = oTZRPSRCO_ReportCopy( NewReport, OrigReport, SourceLPLR2, vSubtask );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:RETURN nRC
      if(8==8)return( nRC );
   } 

   //:END

   //:// If this Report Definition is for a Siron Report, copy the related files
   //:// which have extensions .REP and .XSQ.
   //:IF NewReport.Report.SironName != ""
   if ( CompareAttributeToString( NewReport, "Report", "SironName", "" ) != 0 )
   { 
      //:RetrieveViewForMetaList( vSubtask, CurrentLPLR, zSOURCE_ERD_META ) // Get a view for directory info.
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       m_TZCM_OPR_Operation.RetrieveViewForMetaList( vSubtask, CurrentLPLR, zSOURCE_ERD_META );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
      //:ResetViewFromSubobject( CurrentLPLR ) // Get visibility to root.
      ResetViewFromSubobject( CurrentLPLR );
      //:SourceName = NewReport.Report.SironName
      {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
      StringBuilder sb_SourceName;
      if ( SourceName == null )
         sb_SourceName = new StringBuilder( 32 );
      else
         sb_SourceName = new StringBuilder( SourceName );
             GetVariableFromAttribute( sb_SourceName, mi_lTempInteger_0, 'S', 33, NewReport, "Report", "SironName", "", 0 );
      lTempInteger_0 = mi_lTempInteger_0.intValue( );
      SourceName = sb_SourceName.toString( );}

      //:// Copy REP file.
      //:SourceFileName1 = SourceLPLR.LPLR.PgmSrcDir + "\" + SourceName + ".REP"
      {StringBuilder sb_SourceFileName1;
      if ( SourceFileName1 == null )
         sb_SourceFileName1 = new StringBuilder( 32 );
      else
         sb_SourceFileName1 = new StringBuilder( SourceFileName1 );
             GetStringFromAttribute( sb_SourceFileName1, SourceLPLR, "LPLR", "PgmSrcDir" );
      SourceFileName1 = sb_SourceFileName1.toString( );}
       {StringBuilder sb_SourceFileName1;
      if ( SourceFileName1 == null )
         sb_SourceFileName1 = new StringBuilder( 32 );
      else
         sb_SourceFileName1 = new StringBuilder( SourceFileName1 );
            ZeidonStringConcat( sb_SourceFileName1, 1, 0, "\\", 1, 0, 514 );
      SourceFileName1 = sb_SourceFileName1.toString( );}
       {StringBuilder sb_SourceFileName1;
      if ( SourceFileName1 == null )
         sb_SourceFileName1 = new StringBuilder( 32 );
      else
         sb_SourceFileName1 = new StringBuilder( SourceFileName1 );
            ZeidonStringConcat( sb_SourceFileName1, 1, 0, SourceName, 1, 0, 514 );
      SourceFileName1 = sb_SourceFileName1.toString( );}
       {StringBuilder sb_SourceFileName1;
      if ( SourceFileName1 == null )
         sb_SourceFileName1 = new StringBuilder( 32 );
      else
         sb_SourceFileName1 = new StringBuilder( SourceFileName1 );
            ZeidonStringConcat( sb_SourceFileName1, 1, 0, ".REP", 1, 0, 514 );
      SourceFileName1 = sb_SourceFileName1.toString( );}
      //:SourceFileName2 = CurrentLPLR.LPLR.PgmSrcDir + "\" + SourceName + ".REP"
      {StringBuilder sb_SourceFileName2;
      if ( SourceFileName2 == null )
         sb_SourceFileName2 = new StringBuilder( 32 );
      else
         sb_SourceFileName2 = new StringBuilder( SourceFileName2 );
             GetStringFromAttribute( sb_SourceFileName2, CurrentLPLR, "LPLR", "PgmSrcDir" );
      SourceFileName2 = sb_SourceFileName2.toString( );}
       {StringBuilder sb_SourceFileName2;
      if ( SourceFileName2 == null )
         sb_SourceFileName2 = new StringBuilder( 32 );
      else
         sb_SourceFileName2 = new StringBuilder( SourceFileName2 );
            ZeidonStringConcat( sb_SourceFileName2, 1, 0, "\\", 1, 0, 514 );
      SourceFileName2 = sb_SourceFileName2.toString( );}
       {StringBuilder sb_SourceFileName2;
      if ( SourceFileName2 == null )
         sb_SourceFileName2 = new StringBuilder( 32 );
      else
         sb_SourceFileName2 = new StringBuilder( SourceFileName2 );
            ZeidonStringConcat( sb_SourceFileName2, 1, 0, SourceName, 1, 0, 514 );
      SourceFileName2 = sb_SourceFileName2.toString( );}
       {StringBuilder sb_SourceFileName2;
      if ( SourceFileName2 == null )
         sb_SourceFileName2 = new StringBuilder( 32 );
      else
         sb_SourceFileName2 = new StringBuilder( SourceFileName2 );
            ZeidonStringConcat( sb_SourceFileName2, 1, 0, ".REP", 1, 0, 514 );
      SourceFileName2 = sb_SourceFileName2.toString( );}
      //:SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE )
      try
      {
          m_KZOEP1AA.SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE );
      }
      catch ( Exception e )
      {
         throw ZeidonException.wrapException( e );
      }
      //:ChangeRepFileContents( OrigReport, CurrentLPLR, CurrentLPLR.LPLR.PgmSrcDir, CurrentLPLR.LPLR.ExecDir,
      //:                       SourceName, "REP", 0, vSubtask )
      {StringBuilder sb_szTempString_0;
      if ( szTempString_0 == null )
         sb_szTempString_0 = new StringBuilder( 32 );
      else
         sb_szTempString_0 = new StringBuilder( szTempString_0 );
             GetStringFromAttribute( sb_szTempString_0, CurrentLPLR, "LPLR", "PgmSrcDir" );
      szTempString_0 = sb_szTempString_0.toString( );}
      {StringBuilder sb_szTempString_1;
      if ( szTempString_1 == null )
         sb_szTempString_1 = new StringBuilder( 32 );
      else
         sb_szTempString_1 = new StringBuilder( szTempString_1 );
             GetStringFromAttribute( sb_szTempString_1, CurrentLPLR, "LPLR", "ExecDir" );
      szTempString_1 = sb_szTempString_1.toString( );}
      oTZRPSRCO_ChangeRepFileContents( OrigReport, CurrentLPLR, szTempString_0, szTempString_1, SourceName, "REP", 0, vSubtask );

      //:// Copy XSQ file.
      //:SourceFileName1 = SourceLPLR.LPLR.ExecDir + "\" + SourceName + ".XSQ"
      {StringBuilder sb_SourceFileName1;
      if ( SourceFileName1 == null )
         sb_SourceFileName1 = new StringBuilder( 32 );
      else
         sb_SourceFileName1 = new StringBuilder( SourceFileName1 );
             GetStringFromAttribute( sb_SourceFileName1, SourceLPLR, "LPLR", "ExecDir" );
      SourceFileName1 = sb_SourceFileName1.toString( );}
       {StringBuilder sb_SourceFileName1;
      if ( SourceFileName1 == null )
         sb_SourceFileName1 = new StringBuilder( 32 );
      else
         sb_SourceFileName1 = new StringBuilder( SourceFileName1 );
            ZeidonStringConcat( sb_SourceFileName1, 1, 0, "\\", 1, 0, 514 );
      SourceFileName1 = sb_SourceFileName1.toString( );}
       {StringBuilder sb_SourceFileName1;
      if ( SourceFileName1 == null )
         sb_SourceFileName1 = new StringBuilder( 32 );
      else
         sb_SourceFileName1 = new StringBuilder( SourceFileName1 );
            ZeidonStringConcat( sb_SourceFileName1, 1, 0, SourceName, 1, 0, 514 );
      SourceFileName1 = sb_SourceFileName1.toString( );}
       {StringBuilder sb_SourceFileName1;
      if ( SourceFileName1 == null )
         sb_SourceFileName1 = new StringBuilder( 32 );
      else
         sb_SourceFileName1 = new StringBuilder( SourceFileName1 );
            ZeidonStringConcat( sb_SourceFileName1, 1, 0, ".XSQ", 1, 0, 514 );
      SourceFileName1 = sb_SourceFileName1.toString( );}
      //:SourceFileName2 = CurrentLPLR.LPLR.ExecDir + "\" + SourceName + ".XSQ"
      {StringBuilder sb_SourceFileName2;
      if ( SourceFileName2 == null )
         sb_SourceFileName2 = new StringBuilder( 32 );
      else
         sb_SourceFileName2 = new StringBuilder( SourceFileName2 );
             GetStringFromAttribute( sb_SourceFileName2, CurrentLPLR, "LPLR", "ExecDir" );
      SourceFileName2 = sb_SourceFileName2.toString( );}
       {StringBuilder sb_SourceFileName2;
      if ( SourceFileName2 == null )
         sb_SourceFileName2 = new StringBuilder( 32 );
      else
         sb_SourceFileName2 = new StringBuilder( SourceFileName2 );
            ZeidonStringConcat( sb_SourceFileName2, 1, 0, "\\", 1, 0, 514 );
      SourceFileName2 = sb_SourceFileName2.toString( );}
       {StringBuilder sb_SourceFileName2;
      if ( SourceFileName2 == null )
         sb_SourceFileName2 = new StringBuilder( 32 );
      else
         sb_SourceFileName2 = new StringBuilder( SourceFileName2 );
            ZeidonStringConcat( sb_SourceFileName2, 1, 0, SourceName, 1, 0, 514 );
      SourceFileName2 = sb_SourceFileName2.toString( );}
       {StringBuilder sb_SourceFileName2;
      if ( SourceFileName2 == null )
         sb_SourceFileName2 = new StringBuilder( 32 );
      else
         sb_SourceFileName2 = new StringBuilder( SourceFileName2 );
            ZeidonStringConcat( sb_SourceFileName2, 1, 0, ".XSQ", 1, 0, 514 );
      SourceFileName2 = sb_SourceFileName2.toString( );}
      //:SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE )
      try
      {
          m_KZOEP1AA.SysCopyFile( vSubtask, SourceFileName1, SourceFileName2, TRUE );
      }
      catch ( Exception e )
      {
         throw ZeidonException.wrapException( e );
      }
      //:ChangeRepFileContents( OrigReport, CurrentLPLR, CurrentLPLR.LPLR.PgmSrcDir, CurrentLPLR.LPLR.ExecDir,
      //:                       SourceName, "XSQ", 0, vSubtask )
      {StringBuilder sb_szTempString_2;
      if ( szTempString_2 == null )
         sb_szTempString_2 = new StringBuilder( 32 );
      else
         sb_szTempString_2 = new StringBuilder( szTempString_2 );
             GetStringFromAttribute( sb_szTempString_2, CurrentLPLR, "LPLR", "PgmSrcDir" );
      szTempString_2 = sb_szTempString_2.toString( );}
      {StringBuilder sb_szTempString_3;
      if ( szTempString_3 == null )
         sb_szTempString_3 = new StringBuilder( 32 );
      else
         sb_szTempString_3 = new StringBuilder( szTempString_3 );
             GetStringFromAttribute( sb_szTempString_3, CurrentLPLR, "LPLR", "ExecDir" );
      szTempString_3 = sb_szTempString_3.toString( );}
      oTZRPSRCO_ChangeRepFileContents( OrigReport, CurrentLPLR, szTempString_2, szTempString_3, SourceName, "XSQ", 0, vSubtask );
   } 

   //:END

   //:// Clean up views.
   //:DropMetaOI( vSubtask, OrigReport )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.DropMetaOI( vSubtask, OrigReport );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
   //:DropView( SourceLPLR2 )
   DropView( SourceLPLR2 );

   //:// Finally, commit the Report Def.
   //:CommitMetaOI( vSubtask, NewReport, zSOURCE_REPORT_META )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CommitMetaOI( vSubtask, NewReport, zSOURCE_REPORT_META );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:AdjustGroupSize( VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:                 INTEGER lPageWidth )

//:   VIEW    vTempReport
private int 
oTZRPSRCO_AdjustGroupSize( View     vReportDef,
                           int      lPageWidth )
{
   zVIEW    vTempReport = new zVIEW( );
   //:INTEGER lGroupMaxPosX
   int      lGroupMaxPosX = 0;
   //:INTEGER lLeftMargin
   int      lLeftMargin = 0;
   //:INTEGER lCtrlPos
   int      lCtrlPos = 0;
   //:INTEGER lCtrlSize
   int      lCtrlSize = 0;
   //:INTEGER lRC
   int      lRC = 0;
   //:INTEGER nSkipFirstFlag
   int      nSkipFirstFlag = 0;
   //:DECIMAL a1
   double  a1 = 0.0;
   //:DECIMAL b1
   double  b1 = 0.0;
   //:DECIMAL b2
   double  b2 = 0.0;
   //:DECIMAL dFactor
   double  dFactor = 0.0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;


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
      CreateViewFromViewForTask( vTempReport, vReportDef, null );
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
   {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
       GetIntegerFromAttribute( mi_lTempInteger_0, vReportDef, "Control", "PSDLG_X" );
   lTempInteger_0 = mi_lTempInteger_0.intValue( );}
   {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
       GetIntegerFromAttribute( mi_lTempInteger_1, vReportDef, "Control", "SZDLG_X" );
   lTempInteger_1 = mi_lTempInteger_1.intValue( );}
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
      {MutableInt mi_lLeftMargin = new MutableInt( lLeftMargin );
             GetIntegerFromAttribute( mi_lLeftMargin, vReportDef, "Control", "PSDLG_X" );
      lLeftMargin = mi_lLeftMargin.intValue( );}
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
      {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
             GetIntegerFromAttribute( mi_lTempInteger_2, vReportDef, "Control", "SZDLG_X" );
      lTempInteger_2 = mi_lTempInteger_2.intValue( );}
      lCtrlSize = ( lTempInteger_2 * dFactor ) + 0.5;
      //:vReportDef.Control.SZDLG_X = lCtrlSize
      SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lCtrlSize );
      //:lCtrlPos = vReportDef.Control.PSDLG_X
      {MutableInt mi_lCtrlPos = new MutableInt( lCtrlPos );
             GetIntegerFromAttribute( mi_lCtrlPos, vReportDef, "Control", "PSDLG_X" );
      lCtrlPos = mi_lCtrlPos.intValue( );}
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
         {MutableInt mi_lTempInteger_4 = new MutableInt( lTempInteger_4 );
                   GetIntegerFromAttribute( mi_lTempInteger_4, vReportDef, "Control", "SZDLG_X" );
         lTempInteger_4 = mi_lTempInteger_4.intValue( );}
         lCtrlSize = ( lTempInteger_4 * dFactor ) + 0.5;
         //:vReportDef.Control.SZDLG_X = lCtrlSize
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lCtrlSize );
         //:lCtrlPos = vReportDef.Control.PSDLG_X
         {MutableInt mi_lCtrlPos = new MutableInt( lCtrlPos );
                   GetIntegerFromAttribute( mi_lCtrlPos, vReportDef, "Control", "PSDLG_X" );
         lCtrlPos = mi_lCtrlPos.intValue( );}
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
public int 
oTZRPSRCO_AutodesignReportEntity( View     vSubtask,
                                  View     vReportDef,
                                  View     vUserSpec,
                                  View     vUserSpecRoot,
                                  View     vPE )
{
   int      RESULT = 0;

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
private int 
oTZRPSRCO_CreateGrpSetForEntity( View     vSubtask,
                                 View     vReportDef,
                                 View     vUserSpec,
                                 View     vUserSpecRoot,
                                 View     vPE_Control )
{
   zVIEW    vDefaults = new zVIEW( );
   //:STRING(32) szNameWork
   String   szNameWork = null;
   //:STRING(32) szEntityName
   String   szEntityName = null;
   //:INTEGER    lVerticalPos
   int      lVerticalPos = 0;
   //:INTEGER    lHorizontalPos
   int      lHorizontalPos = 0;
   //:INTEGER    lLongestPrompt
   int      lLongestPrompt = 0;
   //:INTEGER    lTopGroupMargin
   int      lTopGroupMargin = 0;
   //:INTEGER    lBottomGroupMargin
   int      lBottomGroupMargin = 0;
   //:INTEGER    lBottomHeaderMargin
   int      lBottomHeaderMargin = 0;
   //:INTEGER    lLeftGroupMargin
   int      lLeftGroupMargin = 0;
   //:INTEGER    lRightGroupMargin
   int      lRightGroupMargin = 0;
   //:INTEGER    lVerticalIncrement     // space betwen lines
   int      lVerticalIncrement = 0;
   //:INTEGER    lHorizontalIncrement
   int      lHorizontalIncrement = 0;
   //:INTEGER    lAttributeCnt
   int      lAttributeCnt = 0;
   //:INTEGER    lLevelIndentation
   int      lLevelIndentation = 0;
   //:INTEGER    lGroupPosY
   int      lGroupPosY = 0;
   //:INTEGER    lGroupSizeY
   int      lGroupSizeY = 0;
   //:INTEGER    lTitleSize
   int      lTitleSize = 0;
   //:INTEGER    lDefaultFontSize
   int      lDefaultFontSize = 0;
   //:INTEGER    lPageWidth
   int      lPageWidth = 0;
   //:INTEGER    lFontSize
   int      lFontSize = 0;
   //:DECIMAL    dHorzScalingFactor
   double  dHorzScalingFactor = 0.0;
   //:DECIMAL    dVertScalingFactor
   double  dVertScalingFactor = 0.0;
   //:SHORT      nCharacterWidth
   int      nCharacterWidth = 0;
   //:SHORT      nCharacterHeight
   int      nCharacterHeight = 0;
   //:SHORT      nBoldFlag
   int      nBoldFlag = 0;
   //:SHORT      nItalicFlag
   int      nItalicFlag = 0;
   //:INTEGER    lAttributeSize
   int      lAttributeSize = 0;
   //:STRING(2)  szDomainDataType
   String   szDomainDataType = null;
   //:STRING(2)  szAttributePrefix
   String   szAttributePrefix = null;
   //:STRING(2)  szPositioningType
   String   szPositioningType = null;
   //:INTEGER    nControlEndPosition
   int      nControlEndPosition = 0;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;
   int      lTempInteger_5 = 0;
   String   szTempString_0 = null;
   int      lTempInteger_6 = 0;
   String   szTempString_1 = null;
   int      lTempInteger_7 = 0;
   int      lTempInteger_8 = 0;
   int      lTempInteger_9 = 0;
   String   szTempString_2 = null;
   int      lTempInteger_10 = 0;
   int      lTempInteger_11 = 0;
   int      lTempInteger_12 = 0;
   int      lTempInteger_13 = 0;
   int      lTempInteger_14 = 0;
   String   szTempString_3 = null;
   int      lTempInteger_15 = 0;
   String   szTempString_4 = null;
   int      lTempInteger_16 = 0;
   int      lTempInteger_17 = 0;
   int      lTempInteger_18 = 0;
   int      lTempInteger_19 = 0;
   String   szTempString_5 = null;
   String   szTempString_6 = null;
   int      lTempInteger_20 = 0;
   String   szTempString_7 = null;
   int      lTempInteger_21 = 0;
   int      lTempInteger_22 = 0;
   String   szTempString_8 = null;
   int      lTempInteger_23 = 0;
   int      lTempInteger_24 = 0;
   int      lTempInteger_25 = 0;
   int      lTempInteger_26 = 0;
   int      lTempInteger_27 = 0;
   int      lTempInteger_28 = 0;
   int      lTempInteger_29 = 0;
   int      lTempInteger_30 = 0;
   int      lTempInteger_31 = 0;
   int      lTempInteger_32 = 0;
   int      lTempInteger_33 = 0;
   int      lTempInteger_34 = 0;
   int      lTempInteger_35 = 0;
   String   szTempString_9 = null;
   int      lTempInteger_36 = 0;
   String   szTempString_10 = null;
   int      lTempInteger_37 = 0;
   int      lTempInteger_38 = 0;
   int      lTempInteger_39 = 0;
   String   szTempString_11 = null;
   int      lTempInteger_40 = 0;
   int      lTempInteger_41 = 0;
   int      lTempInteger_42 = 0;
   int      lTempInteger_43 = 0;
   int      lTempInteger_44 = 0;


   //:// Set up default information.
   //:GET VIEW vDefaults NAMED "TZ_Defaults"
   RESULT = GetViewByName( vDefaults, "TZ_Defaults", vSubtask, zLEVEL_TASK );
   //:dHorzScalingFactor    = vDefaults.InitValues.ReportTextIncrementX
   {MutableDouble md_dHorzScalingFactor = new MutableDouble( dHorzScalingFactor );
       GetDecimalFromAttribute( md_dHorzScalingFactor, vDefaults, "InitValues", "ReportTextIncrementX" );
   dHorzScalingFactor = md_dHorzScalingFactor.doubleValue( );}
   //:lHorizontalIncrement  = 0
   lHorizontalIncrement = 0;
   //:dVertScalingFactor    = vDefaults.InitValues.ReportTextIncrementY
   {MutableDouble md_dVertScalingFactor = new MutableDouble( dVertScalingFactor );
       GetDecimalFromAttribute( md_dVertScalingFactor, vDefaults, "InitValues", "ReportTextIncrementY" );
   dVertScalingFactor = md_dVertScalingFactor.doubleValue( );}
   //:lVerticalIncrement    = 0
   lVerticalIncrement = 0;
   //:lDefaultFontSize      = vDefaults.InitValues.ReportTextSize * 10
   {MutableInt mi_lTempInteger_0 = new MutableInt( lTempInteger_0 );
       GetIntegerFromAttribute( mi_lTempInteger_0, vDefaults, "InitValues", "ReportTextSize" );
   lTempInteger_0 = mi_lTempInteger_0.intValue( );}
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
   {MutableInt mi_lPageWidth = new MutableInt( lPageWidth );
       GetIntegerFromAttribute( mi_lPageWidth, vReportDef, "Page", "SizeX" );
   lPageWidth = mi_lPageWidth.intValue( );}
   //:lGroupPosY            = 4 * 16
   lGroupPosY = 4 * 16;
   //:lLevelIndentation     = vUserSpecRoot.UI_Spec.LevelIndentation
   {MutableInt mi_lLevelIndentation = new MutableInt( lLevelIndentation );
       GetIntegerFromAttribute( mi_lLevelIndentation, vUserSpecRoot, "UI_Spec", "LevelIndentation" );
   lLevelIndentation = mi_lLevelIndentation.intValue( );}
   //:lLeftGroupMargin      = lLeftGroupMargin +
   //:                 (lLevelIndentation * vUserSpec.LOD_Entity.IndentLvl) * 16
   {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
       GetIntegerFromAttribute( mi_lTempInteger_1, vUserSpec, "LOD_Entity", "IndentLvl" );
   lTempInteger_1 = mi_lTempInteger_1.intValue( );}
   lLeftGroupMargin = lLeftGroupMargin + ( lLevelIndentation * lTempInteger_1 ) * 16;

   //:// Set EntityName for debug purposes, so we will know in tracing what entity we
   //:// are working with.
   //:szEntityName = vUserSpec.LOD_Entity.Name
   {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
   StringBuilder sb_szEntityName;
   if ( szEntityName == null )
      sb_szEntityName = new StringBuilder( 32 );
   else
      sb_szEntityName = new StringBuilder( szEntityName );
       GetVariableFromAttribute( sb_szEntityName, mi_lTempInteger_2, 'S', 33, vUserSpec, "LOD_Entity", "Name", "", 0 );
   lTempInteger_2 = mi_lTempInteger_2.intValue( );
   szEntityName = sb_szEntityName.toString( );}

   //:// The ControlDef is currently defaulted to Text for all subcontrols.
   //:SET CURSOR FIRST vPE_Control.ControlDef WHERE
   //:                 vPE_Control.ControlDef.Tag = "Rpt_Text"
   RESULT = SetCursorFirstEntityByString( vPE_Control, "ControlDef", "Tag", "Rpt_Text", "" );

   //:// If the positioning type is blank, force it to horizontal.
   //:szPositioningType    = vUserSpec.UIS_Entity.ReportOrientationFlag
   {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
   StringBuilder sb_szPositioningType;
   if ( szPositioningType == null )
      sb_szPositioningType = new StringBuilder( 32 );
   else
      sb_szPositioningType = new StringBuilder( szPositioningType );
       GetVariableFromAttribute( sb_szPositioningType, mi_lTempInteger_3, 'S', 3, vUserSpec, "UIS_Entity", "ReportOrientationFlag", "", 0 );
   lTempInteger_3 = mi_lTempInteger_3.intValue( );
   szPositioningType = sb_szPositioningType.toString( );}
   //:IF szPositioningType = ""
   if ( ZeidonStringCompare( szPositioningType, 1, 0, "", 1, 0, 3 ) == 0 )
   { 
      //:szPositioningType = "V"
       {StringBuilder sb_szPositioningType;
      if ( szPositioningType == null )
         sb_szPositioningType = new StringBuilder( 32 );
      else
         sb_szPositioningType = new StringBuilder( szPositioningType );
            ZeidonStringCopy( sb_szPositioningType, 1, 0, "V", 1, 0, 3 );
      szPositioningType = sb_szPositioningType.toString( );}
   } 

   //:END


   //:// *** HEADERS  ***

   //:// Set up GroupSet
   //:CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER )
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "GroupSet", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
   //:vReportDef.GroupSet.Tag      = szEntityName
   SetAttributeFromString( vReportDef, "GroupSet", "Tag", szEntityName );
   //:vReportDef.GroupSet.Type     = "E"
   SetAttributeFromString( vReportDef, "GroupSet", "Type", "E" );
   //:vReportDef.GroupSet.PSDLG_X  = 0
   SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_X", 0 );
   //:vReportDef.GroupSet.PSDLG_Y  = 4 * 16
   lTempInteger_4 = 4 * 16;
   SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", lTempInteger_4 );
   //:vReportDef.GroupSet.SZDLG_X  = vReportDef.Page.SizeX
   SetAttributeFromAttribute( vReportDef, "GroupSet", "SZDLG_X", vReportDef, "Page", "SizeX" );
   //:vReportDef.GroupSet.PPE_Type = 3000
   SetAttributeFromInteger( vReportDef, "GroupSet", "PPE_Type", 3000 );

   //:// Set the FontSize, Bold and Italic flags for the Header group.
   //:IF vUserSpec.UIS_Entity.HeaderText != "" OR szPositioningType = "H"
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "HeaderText", "" ) != 0 || ZeidonStringCompare( szPositioningType, 1, 0, "H", 1, 0, 3 ) == 0 )
   { 
      //:lFontSize = vUserSpec.UIS_Entity.HeaderFontSize * 10
      {MutableInt mi_lTempInteger_5 = new MutableInt( lTempInteger_5 );
             GetIntegerFromAttribute( mi_lTempInteger_5, vUserSpec, "UIS_Entity", "HeaderFontSize" );
      lTempInteger_5 = mi_lTempInteger_5.intValue( );}
      lFontSize = lTempInteger_5 * 10;
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
      lHorizontalIncrement = (int) nCharacterWidth * dHorzScalingFactor * 16;
      //:lVerticalIncrement   = nCharacterHeight * dVertScalingFactor * 16
      lVerticalIncrement = (int) nCharacterHeight * dVertScalingFactor * 16;
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
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
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
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:szNameWork = "H_" + vUserSpec.LOD_Entity.Name
         {MutableInt mi_lTempInteger_6 = new MutableInt( lTempInteger_6 );
         StringBuilder sb_szTempString_0;
         if ( szTempString_0 == null )
            sb_szTempString_0 = new StringBuilder( 32 );
         else
            sb_szTempString_0 = new StringBuilder( szTempString_0 );
                   GetVariableFromAttribute( sb_szTempString_0, mi_lTempInteger_6, 'S', 33, vUserSpec, "LOD_Entity", "Name", "", 0 );
         lTempInteger_6 = mi_lTempInteger_6.intValue( );
         szTempString_0 = sb_szTempString_0.toString( );}
          {StringBuilder sb_szNameWork;
         if ( szNameWork == null )
            sb_szNameWork = new StringBuilder( 32 );
         else
            sb_szNameWork = new StringBuilder( szNameWork );
                  ZeidonStringCopy( sb_szNameWork, 1, 0, "H_", 1, 0, 33 );
         szNameWork = sb_szNameWork.toString( );}
          {StringBuilder sb_szNameWork;
         if ( szNameWork == null )
            sb_szNameWork = new StringBuilder( 32 );
         else
            sb_szNameWork = new StringBuilder( szNameWork );
                  ZeidonStringConcat( sb_szNameWork, 1, 0, szTempString_0, 1, 0, 33 );
         szNameWork = sb_szNameWork.toString( );}
         //:vReportDef.Control.Type    = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:vReportDef.Control.Text    = vUserSpec.UIS_Entity.HeaderText
         SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "UIS_Entity", "HeaderText" );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lLeftGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lLeftGroupMargin );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lTitleSize                 = GetStringLength( vUserSpec.UIS_Entity.HeaderText )
         {StringBuilder sb_szTempString_1;
         if ( szTempString_1 == null )
            sb_szTempString_1 = new StringBuilder( 32 );
         else
            sb_szTempString_1 = new StringBuilder( szTempString_1 );
                   GetStringFromAttribute( sb_szTempString_1, vUserSpec, "UIS_Entity", "HeaderText" );
         szTempString_1 = sb_szTempString_1.toString( );}
         {
          TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vSubtask );
          lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_1 );
          // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
         }
         //:vReportDef.Control.SZDLG_X = lTitleSize * nCharacterWidth * 16
         lTempInteger_7 = lTitleSize * nCharacterWidth * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_7 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_8 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_8 );

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
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 )
         {
          TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vSubtask );
          m_TZ__OPRS_Operation.ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 );
          // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
         }
         //:szNameWork                 = vUserSpec.M_ER_Attribute.Name
         {MutableInt mi_lTempInteger_9 = new MutableInt( lTempInteger_9 );
         StringBuilder sb_szNameWork;
         if ( szNameWork == null )
            sb_szNameWork = new StringBuilder( 32 );
         else
            sb_szNameWork = new StringBuilder( szNameWork );
                   GetVariableFromAttribute( sb_szNameWork, mi_lTempInteger_9, 'S', 33, vUserSpec, "M_ER_Attribute", "Name", "", 0 );
         lTempInteger_9 = mi_lTempInteger_9.intValue( );
         szNameWork = sb_szNameWork.toString( );}
         //:     vReportDef.Control.Tag     = "T_" + szAttributePrefix + "_" + szNameWork[1:26]
          {StringBuilder sb_szTempString_2;
         if ( szTempString_2 == null )
            sb_szTempString_2 = new StringBuilder( 32 );
         else
            sb_szTempString_2 = new StringBuilder( szTempString_2 );
                  ZeidonStringCopy( sb_szTempString_2, 1, 0, "T_", 1, 0, 33 );
         szTempString_2 = sb_szTempString_2.toString( );}
          {StringBuilder sb_szTempString_2;
         if ( szTempString_2 == null )
            sb_szTempString_2 = new StringBuilder( 32 );
         else
            sb_szTempString_2 = new StringBuilder( szTempString_2 );
                  ZeidonStringConcat( sb_szTempString_2, 1, 0, szAttributePrefix, 1, 0, 33 );
         szTempString_2 = sb_szTempString_2.toString( );}
          {StringBuilder sb_szTempString_2;
         if ( szTempString_2 == null )
            sb_szTempString_2 = new StringBuilder( 32 );
         else
            sb_szTempString_2 = new StringBuilder( szTempString_2 );
                  ZeidonStringConcat( sb_szTempString_2, 1, 0, "_", 1, 0, 33 );
         szTempString_2 = sb_szTempString_2.toString( );}
          {StringBuilder sb_szTempString_2;
         if ( szTempString_2 == null )
            sb_szTempString_2 = new StringBuilder( 32 );
         else
            sb_szTempString_2 = new StringBuilder( szTempString_2 );
                  ZeidonStringConcat( sb_szTempString_2, 1, 0, szNameWork, 1, 26, 33 );
         szTempString_2 = sb_szTempString_2.toString( );}
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
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lTitleSize                 = GetMaxAttributeSize( vUserSpec, "H" )
         lTitleSize = oTZRPSRCO_GetMaxAttributeSize( vUserSpec, "H" );
         //:lTitleSize                 = lTitleSize * nCharacterWidth
         lTitleSize = lTitleSize * nCharacterWidth;
         //:vReportDef.Control.SZDLG_X = lTitleSize * 16
         lTempInteger_10 = lTitleSize * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_10 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_11 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_11 );

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
      {MutableInt mi_lTempInteger_12 = new MutableInt( lTempInteger_12 );
             GetIntegerFromAttribute( mi_lTempInteger_12, vReportDef, "Control", "PSDLG_Y" );
      lTempInteger_12 = mi_lTempInteger_12.intValue( );}
      {MutableInt mi_lTempInteger_13 = new MutableInt( lTempInteger_13 );
             GetIntegerFromAttribute( mi_lTempInteger_13, vReportDef, "Control", "SZDLG_Y" );
      lTempInteger_13 = mi_lTempInteger_13.intValue( );}
      lGroupSizeY = lTempInteger_12 + lTempInteger_13 + lVerticalIncrement + lBottomHeaderMargin;
      //:vReportDef.Group.SZDLG_Y    = lGroupSizeY
      SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lGroupSizeY );
      //:vReportDef.GroupSet.SZDLG_Y = lGroupSizeY
      SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lGroupSizeY );

      //:// update lGroupPosY
      //:lGroupPosY = lGroupPosY + vReportDef.Group.SZDLG_Y
      {MutableInt mi_lTempInteger_14 = new MutableInt( lTempInteger_14 );
             GetIntegerFromAttribute( mi_lTempInteger_14, vReportDef, "Group", "SZDLG_Y" );
      lTempInteger_14 = mi_lTempInteger_14.intValue( );}
      lGroupPosY = lGroupPosY + lTempInteger_14;

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
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
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
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:szNameWork = "H_" + vUserSpec.LOD_Entity.Name
         {MutableInt mi_lTempInteger_15 = new MutableInt( lTempInteger_15 );
         StringBuilder sb_szTempString_3;
         if ( szTempString_3 == null )
            sb_szTempString_3 = new StringBuilder( 32 );
         else
            sb_szTempString_3 = new StringBuilder( szTempString_3 );
                   GetVariableFromAttribute( sb_szTempString_3, mi_lTempInteger_15, 'S', 33, vUserSpec, "LOD_Entity", "Name", "", 0 );
         lTempInteger_15 = mi_lTempInteger_15.intValue( );
         szTempString_3 = sb_szTempString_3.toString( );}
          {StringBuilder sb_szNameWork;
         if ( szNameWork == null )
            sb_szNameWork = new StringBuilder( 32 );
         else
            sb_szNameWork = new StringBuilder( szNameWork );
                  ZeidonStringCopy( sb_szNameWork, 1, 0, "H_", 1, 0, 33 );
         szNameWork = sb_szNameWork.toString( );}
          {StringBuilder sb_szNameWork;
         if ( szNameWork == null )
            sb_szNameWork = new StringBuilder( 32 );
         else
            sb_szNameWork = new StringBuilder( szNameWork );
                  ZeidonStringConcat( sb_szNameWork, 1, 0, szTempString_3, 1, 0, 33 );
         szNameWork = sb_szNameWork.toString( );}
         //:vReportDef.Control.Type    = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:vReportDef.Control.Text    = vUserSpec.UIS_Entity.HeaderText
         SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "UIS_Entity", "HeaderText" );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lLeftGroupMargin
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lLeftGroupMargin );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lTitleSize = GetStringLength( vUserSpec.UIS_Entity.HeaderText )
         {StringBuilder sb_szTempString_4;
         if ( szTempString_4 == null )
            sb_szTempString_4 = new StringBuilder( 32 );
         else
            sb_szTempString_4 = new StringBuilder( szTempString_4 );
                   GetStringFromAttribute( sb_szTempString_4, vUserSpec, "UIS_Entity", "HeaderText" );
         szTempString_4 = sb_szTempString_4.toString( );}
         {
          TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vSubtask );
          lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_4 );
          // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
         }
         //:vReportDef.Control.SZDLG_X = lTitleSize * nCharacterWidth * 16
         lTempInteger_16 = lTitleSize * nCharacterWidth * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_16 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_17 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_17 );

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );

         //:lHorizontalIncrement = nCharacterWidth  * dHorzScalingFactor
         lHorizontalIncrement = (int) nCharacterWidth * dHorzScalingFactor;
         //:lVerticalIncrement = nCharacterHeight * dVertScalingFactor
         lVerticalIncrement = (int) nCharacterHeight * dVertScalingFactor;
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
         {MutableInt mi_lTempInteger_18 = new MutableInt( lTempInteger_18 );
                   GetIntegerFromAttribute( mi_lTempInteger_18, vReportDef, "Group", "SZDLG_Y" );
         lTempInteger_18 = mi_lTempInteger_18.intValue( );}
         lGroupPosY = lGroupPosY + lTempInteger_18;

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
   {MutableInt mi_lTempInteger_19 = new MutableInt( lTempInteger_19 );
       GetIntegerFromAttribute( mi_lTempInteger_19, vUserSpec, "UIS_Entity", "FontSize" );
   lTempInteger_19 = mi_lTempInteger_19.intValue( );}
   lFontSize = lTempInteger_19 * 10;
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
   lHorizontalIncrement = (int) nCharacterWidth * dHorzScalingFactor * 16;
   //:lVerticalIncrement = nCharacterHeight * dVertScalingFactor * 16
   lVerticalIncrement = (int) nCharacterHeight * dVertScalingFactor * 16;

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
            {StringBuilder sb_szTempString_5;
            if ( szTempString_5 == null )
               sb_szTempString_5 = new StringBuilder( 32 );
            else
               sb_szTempString_5 = new StringBuilder( szTempString_5 );
                         GetStringFromAttribute( sb_szTempString_5, vUserSpec, "M_ER_Attribute", "PromptPaintLit" );
            szTempString_5 = sb_szTempString_5.toString( );}
            {
             TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vSubtask );
             lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_5 );
             // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
            }
            //:ELSE
         } 
         else
         { 
            //:lTitleSize = GetStringLength( vUserSpec.M_ER_Attribute.Name )
            {StringBuilder sb_szTempString_6;
            if ( szTempString_6 == null )
               sb_szTempString_6 = new StringBuilder( 32 );
            else
               sb_szTempString_6 = new StringBuilder( szTempString_6 );
                         GetStringFromAttribute( sb_szTempString_6, vUserSpec, "M_ER_Attribute", "Name" );
            szTempString_6 = sb_szTempString_6.toString( );}
            {
             TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vSubtask );
             lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_6 );
             // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
            }
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
   {
    TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
    m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
    // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
   }
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
      {MutableInt mi_lTempInteger_20 = new MutableInt( lTempInteger_20 );
      StringBuilder sb_szNameWork;
      if ( szNameWork == null )
         sb_szNameWork = new StringBuilder( 32 );
      else
         sb_szNameWork = new StringBuilder( szNameWork );
             GetVariableFromAttribute( sb_szNameWork, mi_lTempInteger_20, 'S', 33, vUserSpec, "M_ER_Attribute", "Name", "", 0 );
      lTempInteger_20 = mi_lTempInteger_20.intValue( );
      szNameWork = sb_szNameWork.toString( );}
      //:ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 )
      {
       TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vSubtask );
       m_TZ__OPRS_Operation.ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 );
       // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
      }

      //:// If positioning is vertical, add the title.  If positioning is horizontal,
      //:// the titles were added as their own group.  Control Type is 10 for title.
      //:IF szPositioningType = "V"
      if ( ZeidonStringCompare( szPositioningType, 1, 0, "V", 1, 0, 3 ) == 0 )
      { 
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:vReportDef.Control.Tag = "T_" + szAttributePrefix + "_" + szNameWork[1:26]
          {StringBuilder sb_szTempString_7;
         if ( szTempString_7 == null )
            sb_szTempString_7 = new StringBuilder( 32 );
         else
            sb_szTempString_7 = new StringBuilder( szTempString_7 );
                  ZeidonStringCopy( sb_szTempString_7, 1, 0, "T_", 1, 0, 33 );
         szTempString_7 = sb_szTempString_7.toString( );}
          {StringBuilder sb_szTempString_7;
         if ( szTempString_7 == null )
            sb_szTempString_7 = new StringBuilder( 32 );
         else
            sb_szTempString_7 = new StringBuilder( szTempString_7 );
                  ZeidonStringConcat( sb_szTempString_7, 1, 0, szAttributePrefix, 1, 0, 33 );
         szTempString_7 = sb_szTempString_7.toString( );}
          {StringBuilder sb_szTempString_7;
         if ( szTempString_7 == null )
            sb_szTempString_7 = new StringBuilder( 32 );
         else
            sb_szTempString_7 = new StringBuilder( szTempString_7 );
                  ZeidonStringConcat( sb_szTempString_7, 1, 0, "_", 1, 0, 33 );
         szTempString_7 = sb_szTempString_7.toString( );}
          {StringBuilder sb_szTempString_7;
         if ( szTempString_7 == null )
            sb_szTempString_7 = new StringBuilder( 32 );
         else
            sb_szTempString_7 = new StringBuilder( szTempString_7 );
                  ZeidonStringConcat( sb_szTempString_7, 1, 0, szNameWork, 1, 26, 33 );
         szTempString_7 = sb_szTempString_7.toString( );}
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
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:vReportDef.Control.SZDLG_X = lLongestPrompt * 16
         lTempInteger_21 = lLongestPrompt * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_21 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_22 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_22 );

         //:INCLUDE vReportDef.ControlDef FROM vPE_Control.ControlDef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "ControlDef", vPE_Control, "ControlDef", zPOS_AFTER );
      } 


      //:END

      //:// For each attribute create the control. Control Type is 20 for control.
      //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
      //:vReportDef.Control.Tag  = "A_" + szAttributePrefix + "_" + szNameWork[1:26]
       {StringBuilder sb_szTempString_8;
      if ( szTempString_8 == null )
         sb_szTempString_8 = new StringBuilder( 32 );
      else
         sb_szTempString_8 = new StringBuilder( szTempString_8 );
            ZeidonStringCopy( sb_szTempString_8, 1, 0, "A_", 1, 0, 33 );
      szTempString_8 = sb_szTempString_8.toString( );}
       {StringBuilder sb_szTempString_8;
      if ( szTempString_8 == null )
         sb_szTempString_8 = new StringBuilder( 32 );
      else
         sb_szTempString_8 = new StringBuilder( szTempString_8 );
            ZeidonStringConcat( sb_szTempString_8, 1, 0, szAttributePrefix, 1, 0, 33 );
      szTempString_8 = sb_szTempString_8.toString( );}
       {StringBuilder sb_szTempString_8;
      if ( szTempString_8 == null )
         sb_szTempString_8 = new StringBuilder( 32 );
      else
         sb_szTempString_8 = new StringBuilder( szTempString_8 );
            ZeidonStringConcat( sb_szTempString_8, 1, 0, "_", 1, 0, 33 );
      szTempString_8 = sb_szTempString_8.toString( );}
       {StringBuilder sb_szTempString_8;
      if ( szTempString_8 == null )
         sb_szTempString_8 = new StringBuilder( 32 );
      else
         sb_szTempString_8 = new StringBuilder( szTempString_8 );
            ZeidonStringConcat( sb_szTempString_8, 1, 0, szNameWork, 1, 26, 33 );
      szTempString_8 = sb_szTempString_8.toString( );}
      SetAttributeFromString( vReportDef, "Control", "Tag", szTempString_8 );
      //:vReportDef.Control.Type = 20
      SetAttributeFromInteger( vReportDef, "Control", "Type", 20 );

      //:// Right justify decimals, shorts and longs.  Subtype = 16777216 sets the
      //:// bit zCONTROL_RIGHTJUSTIFY.
      //:szDomainDataType = vUserSpec.M_Domain.DataType
      {MutableInt mi_lTempInteger_23 = new MutableInt( lTempInteger_23 );
      StringBuilder sb_szDomainDataType;
      if ( szDomainDataType == null )
         sb_szDomainDataType = new StringBuilder( 32 );
      else
         sb_szDomainDataType = new StringBuilder( szDomainDataType );
             GetVariableFromAttribute( sb_szDomainDataType, mi_lTempInteger_23, 'S', 3, vUserSpec, "M_Domain", "DataType", "", 0 );
      lTempInteger_23 = mi_lTempInteger_23.intValue( );
      szDomainDataType = sb_szDomainDataType.toString( );}
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
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "CtrlMap", zPOS_AFTER );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
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
         lTempInteger_24 = lAttributeSize * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_24 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_25 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_25 );

         //:lHorizontalPos = lHorizontalPos + (lAttributeSize * 16) +
         //:                                                lHorizontalIncrement
         lHorizontalPos = lHorizontalPos + ( lAttributeSize * 16 ) + lHorizontalIncrement;
         //:ELSE
      } 
      else
      { 
         //:vReportDef.Control.PSDLG_X =
         //:      lHorizontalPos + (lLongestPrompt * 16) + lHorizontalIncrement
         lTempInteger_26 = lHorizontalPos + ( lLongestPrompt * 16 ) + lHorizontalIncrement;
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lTempInteger_26 );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lAttributeSize             = GetMaxAttributeSize( vUserSpec, "V" )
         lAttributeSize = oTZRPSRCO_GetMaxAttributeSize( vUserSpec, "V" );
         //:lAttributeSize             = lAttributeSize * nCharacterWidth * 16
         lAttributeSize = lAttributeSize * nCharacterWidth * 16;
         //:// trim the attribute size,if needed, to fit onto page
         //:nControlEndPosition = lAttributeSize + vReportDef.Control.PSDLG_X
         {MutableInt mi_lTempInteger_27 = new MutableInt( lTempInteger_27 );
                   GetIntegerFromAttribute( mi_lTempInteger_27, vReportDef, "Control", "PSDLG_X" );
         lTempInteger_27 = mi_lTempInteger_27.intValue( );}
         nControlEndPosition = lAttributeSize + lTempInteger_27;
         //:IF nControlEndPosition > vReportDef.Page.SizeX
         if ( CompareAttributeToInteger( vReportDef, "Page", "SizeX", nControlEndPosition ) < 0 )
         { 
            //:lAttributeSize = vReportDef.Page.SizeX -
            //:                                       vReportDef.Control.PSDLG_X
            {MutableInt mi_lTempInteger_28 = new MutableInt( lTempInteger_28 );
                         GetIntegerFromAttribute( mi_lTempInteger_28, vReportDef, "Page", "SizeX" );
            lTempInteger_28 = mi_lTempInteger_28.intValue( );}
            {MutableInt mi_lTempInteger_29 = new MutableInt( lTempInteger_29 );
                         GetIntegerFromAttribute( mi_lTempInteger_29, vReportDef, "Control", "PSDLG_X" );
            lTempInteger_29 = mi_lTempInteger_29.intValue( );}
            lAttributeSize = lTempInteger_28 - lTempInteger_29;
         } 

         //:END
         //:vReportDef.Control.SZDLG_X = lAttributeSize
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lAttributeSize );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_30 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_30 );

         //:// update vertical position
         //:lVerticalPos = lVerticalPos + lVerticalIncrement +
         //:                              vReportDef.Control.SZDLG_Y
         {MutableInt mi_lTempInteger_31 = new MutableInt( lTempInteger_31 );
                   GetIntegerFromAttribute( mi_lTempInteger_31, vReportDef, "Control", "SZDLG_Y" );
         lTempInteger_31 = mi_lTempInteger_31.intValue( );}
         lVerticalPos = lVerticalPos + lVerticalIncrement + lTempInteger_31;
      } 

      //:END

      //:// Set Bold and Italic flags and Fontsize.
      //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
      //:                      FALSE, FALSE, 0, lFontSize,
      //:                      0, 0, 0, 0, 0, 0, 0 )
      m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

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
      {MutableInt mi_lTempInteger_32 = new MutableInt( lTempInteger_32 );
             GetIntegerFromAttribute( mi_lTempInteger_32, vReportDef, "Control", "SZDLG_Y" );
      lTempInteger_32 = mi_lTempInteger_32.intValue( );}
      lVerticalPos = lVerticalPos + lVerticalIncrement + lTempInteger_32;
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
   {MutableInt mi_lTempInteger_33 = new MutableInt( lTempInteger_33 );
       GetIntegerFromAttribute( mi_lTempInteger_33, vReportDef, "Group", "SZDLG_Y" );
   lTempInteger_33 = mi_lTempInteger_33.intValue( );}
   lGroupPosY = lGroupPosY + lTempInteger_33;

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
   lTempInteger_34 = CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" );
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "FooterText", "" ) != 0 || lTempInteger_34 == 0 )
   { 

      //:// Set the FontSize, Bold and Italic flags for the Footer group.
      //:lFontSize = vUserSpec.UIS_Entity.FooterFontSize * 10
      {MutableInt mi_lTempInteger_35 = new MutableInt( lTempInteger_35 );
             GetIntegerFromAttribute( mi_lTempInteger_35, vUserSpec, "UIS_Entity", "FooterFontSize" );
      lTempInteger_35 = mi_lTempInteger_35.intValue( );}
      lFontSize = lTempInteger_35 * 10;
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
      {
       TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
       m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Group", zPOS_AFTER );
       // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
      }
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
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:szNameWork = "F_" + vUserSpec.LOD_Entity.Name
         {MutableInt mi_lTempInteger_36 = new MutableInt( lTempInteger_36 );
         StringBuilder sb_szTempString_9;
         if ( szTempString_9 == null )
            sb_szTempString_9 = new StringBuilder( 32 );
         else
            sb_szTempString_9 = new StringBuilder( szTempString_9 );
                   GetVariableFromAttribute( sb_szTempString_9, mi_lTempInteger_36, 'S', 33, vUserSpec, "LOD_Entity", "Name", "", 0 );
         lTempInteger_36 = mi_lTempInteger_36.intValue( );
         szTempString_9 = sb_szTempString_9.toString( );}
          {StringBuilder sb_szNameWork;
         if ( szNameWork == null )
            sb_szNameWork = new StringBuilder( 32 );
         else
            sb_szNameWork = new StringBuilder( szNameWork );
                  ZeidonStringCopy( sb_szNameWork, 1, 0, "F_", 1, 0, 33 );
         szNameWork = sb_szNameWork.toString( );}
          {StringBuilder sb_szNameWork;
         if ( szNameWork == null )
            sb_szNameWork = new StringBuilder( 32 );
         else
            sb_szNameWork = new StringBuilder( szNameWork );
                  ZeidonStringConcat( sb_szNameWork, 1, 0, szTempString_9, 1, 0, 33 );
         szNameWork = sb_szNameWork.toString( );}
         //:vReportDef.Control.Type    = 10
         SetAttributeFromInteger( vReportDef, "Control", "Type", 10 );
         //:vReportDef.Control.Text    = vUserSpec.UIS_Entity.FooterText
         SetAttributeFromAttribute( vReportDef, "Control", "Text", vUserSpec, "UIS_Entity", "FooterText" );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

         //:vReportDef.Control.PSDLG_X = lHorizontalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lHorizontalPos );
         //:vReportDef.Control.PSDLG_Y = lVerticalPos
         SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lVerticalPos );
         //:lTitleSize = GetStringLength( vUserSpec.UIS_Entity.FooterText )
         {StringBuilder sb_szTempString_10;
         if ( szTempString_10 == null )
            sb_szTempString_10 = new StringBuilder( 32 );
         else
            sb_szTempString_10 = new StringBuilder( szTempString_10 );
                   GetStringFromAttribute( sb_szTempString_10, vUserSpec, "UIS_Entity", "FooterText" );
         szTempString_10 = sb_szTempString_10.toString( );}
         {
          TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vSubtask );
          lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_10 );
          // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
         }
         //:lTitleSize = nCharacterWidth * lTitleSize
         lTitleSize = nCharacterWidth * lTitleSize;
         //:vReportDef.Control.SZDLG_X = lTitleSize * 16
         lTempInteger_37 = lTitleSize * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lTempInteger_37 );
         //:vReportDef.Control.SZDLG_Y = nCharacterHeight * 16
         lTempInteger_38 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_38 );

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
         {MutableInt mi_lTempInteger_39 = new MutableInt( lTempInteger_39 );
         StringBuilder sb_szNameWork;
         if ( szNameWork == null )
            sb_szNameWork = new StringBuilder( 32 );
         else
            sb_szNameWork = new StringBuilder( szNameWork );
                   GetVariableFromAttribute( sb_szNameWork, mi_lTempInteger_39, 'S', 33, vUserSpec, "M_ER_Attribute", "Name", "", 0 );
         lTempInteger_39 = mi_lTempInteger_39.intValue( );
         szNameWork = sb_szNameWork.toString( );}
         //:ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 )
         {
          TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vSubtask );
          m_TZ__OPRS_Operation.ConvertIntegerToString( lAttributeCnt, szAttributePrefix, 2 );
          // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
         }

         //:// For each attribute create the control. Control Type is 20 for control.
         //:CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER )
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "Control", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:vReportDef.Control.Tag  = "A_" + szAttributePrefix + "_" + szNameWork[1:26]
          {StringBuilder sb_szTempString_11;
         if ( szTempString_11 == null )
            sb_szTempString_11 = new StringBuilder( 32 );
         else
            sb_szTempString_11 = new StringBuilder( szTempString_11 );
                  ZeidonStringCopy( sb_szTempString_11, 1, 0, "A_", 1, 0, 33 );
         szTempString_11 = sb_szTempString_11.toString( );}
          {StringBuilder sb_szTempString_11;
         if ( szTempString_11 == null )
            sb_szTempString_11 = new StringBuilder( 32 );
         else
            sb_szTempString_11 = new StringBuilder( szTempString_11 );
                  ZeidonStringConcat( sb_szTempString_11, 1, 0, szAttributePrefix, 1, 0, 33 );
         szTempString_11 = sb_szTempString_11.toString( );}
          {StringBuilder sb_szTempString_11;
         if ( szTempString_11 == null )
            sb_szTempString_11 = new StringBuilder( 32 );
         else
            sb_szTempString_11 = new StringBuilder( szTempString_11 );
                  ZeidonStringConcat( sb_szTempString_11, 1, 0, "_", 1, 0, 33 );
         szTempString_11 = sb_szTempString_11.toString( );}
          {StringBuilder sb_szTempString_11;
         if ( szTempString_11 == null )
            sb_szTempString_11 = new StringBuilder( 32 );
         else
            sb_szTempString_11 = new StringBuilder( szTempString_11 );
                  ZeidonStringConcat( sb_szTempString_11, 1, 0, szNameWork, 1, 26, 33 );
         szTempString_11 = sb_szTempString_11.toString( );}
         SetAttributeFromString( vReportDef, "Control", "Tag", szTempString_11 );
         //:vReportDef.Control.Type = 20
         SetAttributeFromInteger( vReportDef, "Control", "Type", 20 );

         //:// Right justify decimals, shorts and longs.  Subtype = 16777216 sets the
         //:// bit zCONTROL_RIGHTJUSTIFY.
         //:szDomainDataType = vUserSpec.L_Domain.DataType
         {MutableInt mi_lTempInteger_40 = new MutableInt( lTempInteger_40 );
         StringBuilder sb_szDomainDataType;
         if ( szDomainDataType == null )
            sb_szDomainDataType = new StringBuilder( 32 );
         else
            sb_szDomainDataType = new StringBuilder( szDomainDataType );
                   GetVariableFromAttribute( sb_szDomainDataType, mi_lTempInteger_40, 'S', 3, vUserSpec, "L_Domain", "DataType", "", 0 );
         lTempInteger_40 = mi_lTempInteger_40.intValue( );
         szDomainDataType = sb_szDomainDataType.toString( );}
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
         {
          TZCM_OPR_Operation m_TZCM_OPR_Operation = new TZCM_OPR_Operation( vSubtask );
          m_TZCM_OPR_Operation.CreateMetaEntity( vSubtask, vReportDef, "CtrlMap", zPOS_AFTER );
          // m_TZCM_OPR_Operation = null;  // permit gc  (unnecessary)
         }
         //:INCLUDE vReportDef.CtrlMapLOD_Attribute FROM vUserSpec.L_LOD_Attribute
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "CtrlMapLOD_Attribute", vUserSpec, "L_LOD_Attribute", zPOS_AFTER );
         //:INCLUDE vReportDef.CtrlMapView FROM vUserSpecRoot.UIS_ViewObjRef
         RESULT = IncludeSubobjectFromSubobject( vReportDef, "CtrlMapView", vUserSpecRoot, "UIS_ViewObjRef", zPOS_AFTER );

         //:// Set Bold and Italic flags and Fontsize.
         //:TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag,
         //:                      FALSE, FALSE, 0, lFontSize,
         //:                      0, 0, 0, 0, 0, 0, 0 )
         m_ZDRVROPR.TX_SetTextRptCtrlBOI( vReportDef, FALSE, nBoldFlag, nItalicFlag, FALSE, FALSE, 0, lFontSize, 0, 0, 0, 0, 0, 0, 0 );

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
         lTempInteger_41 = nCharacterHeight * 16;
         SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lTempInteger_41 );

         //:lHorizontalPos = lHorizontalPos + lAttributeSize + lHorizontalIncrement
         lHorizontalPos = lHorizontalPos + lAttributeSize + lHorizontalIncrement;

         //:lAttributeCnt = lAttributeCnt + 1
         lAttributeCnt = lAttributeCnt + 1;
         RESULT = SetCursorNextEntity( vUserSpec, "L_LOD_Attribute", "" );
      } 

      //:END

      //:// update lVerticalPos to point to bottom of controls
      //:lVerticalPos = lVerticalPos + vReportDef.Control.SZDLG_Y
      {MutableInt mi_lTempInteger_42 = new MutableInt( lTempInteger_42 );
             GetIntegerFromAttribute( mi_lTempInteger_42, vReportDef, "Control", "SZDLG_Y" );
      lTempInteger_42 = mi_lTempInteger_42.intValue( );}
      lVerticalPos = lVerticalPos + lTempInteger_42;

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
      {MutableInt mi_lTempInteger_43 = new MutableInt( lTempInteger_43 );
             GetIntegerFromAttribute( mi_lTempInteger_43, vReportDef, "Group", "SZDLG_Y" );
      lTempInteger_43 = mi_lTempInteger_43.intValue( );}
      lGroupPosY = lGroupPosY + lTempInteger_43;

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
      {MutableInt mi_lTempInteger_44 = new MutableInt( lTempInteger_44 );
             GetIntegerFromAttribute( mi_lTempInteger_44, vReportDef, "Group", "SZDLG_Y" );
      lTempInteger_44 = mi_lTempInteger_44.intValue( );}
      lGroupSizeY = lGroupSizeY + lTempInteger_44;
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
private int  
oTZRPSRCO_GetMaxAttributeSize( View     vUserSpec,
                               String   szPositionType )
{
   int      lTitleSize = 0;
   //:INTEGER lAttributeSize
   int      lAttributeSize = 0;
   //:INTEGER lReturnSize
   int      lReturnSize = 0;
   //:INTEGER lRC
   int      lRC = 0;
   //:INTEGER lCharWidth
   int      lCharWidth = 0;
   String   szTempString_0 = null;
   String   szTempString_1 = null;


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
         {MutableInt mi_lRC = new MutableInt( lRC );
                   GetIntegerFromAttribute( mi_lRC, vUserSpec, "M_ER_Attribute", "ListPaintLth" );
         lRC = mi_lRC.intValue( );}
         //:RETURN lRC
         if(8==8)return( lRC );
         //:ELSE
      } 
      else
      { 
         //:IF vUserSpec.M_ER_Attribute.ListPaintLit = ""
         if ( CompareAttributeToString( vUserSpec, "M_ER_Attribute", "ListPaintLit", "" ) == 0 )
         { 
            //:lTitleSize = GetStringLength( vUserSpec.M_ER_Attribute.Name )
            {StringBuilder sb_szTempString_0;
            if ( szTempString_0 == null )
               sb_szTempString_0 = new StringBuilder( 32 );
            else
               sb_szTempString_0 = new StringBuilder( szTempString_0 );
                         GetStringFromAttribute( sb_szTempString_0, vUserSpec, "M_ER_Attribute", "Name" );
            szTempString_0 = sb_szTempString_0.toString( );}
            {
             TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vUserSpec );
             lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_0 );
             // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
            }
            //:ELSE
         } 
         else
         { 
            //:lTitleSize = GetStringLength( vUserSpec.M_ER_Attribute.ListPaintLit )
            {StringBuilder sb_szTempString_1;
            if ( szTempString_1 == null )
               sb_szTempString_1 = new StringBuilder( 32 );
            else
               sb_szTempString_1 = new StringBuilder( szTempString_1 );
                         GetStringFromAttribute( sb_szTempString_1, vUserSpec, "M_ER_Attribute", "ListPaintLit" );
            szTempString_1 = sb_szTempString_1.toString( );}
            {
             TZ__OPRS_Operation m_TZ__OPRS_Operation = new TZ__OPRS_Operation( vUserSpec );
             lTitleSize = m_TZ__OPRS_Operation.GetStringLength( szTempString_1 );
             // m_TZ__OPRS_Operation = null;  // permit gc  (unnecessary)
            }
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
         {MutableInt mi_lRC = new MutableInt( lRC );
                   GetIntegerFromAttribute( mi_lRC, vUserSpec, "M_ER_Attribute", "PromptPaintLth" );
         lRC = mi_lRC.intValue( );}
         //:RETURN lRC
         if(8==8)return( lRC );
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
            {MutableInt mi_lAttributeSize = new MutableInt( lAttributeSize );
                         GetIntegerFromAttribute( mi_lAttributeSize, vUserSpec, "M_Domain", "MaxStringLth" );
            lAttributeSize = mi_lAttributeSize.intValue( );}
            //:ELSE
         } 
         else
         { 
            //:lAttributeSize = vUserSpec.M_ER_Attribute.Lth
            {MutableInt mi_lAttributeSize = new MutableInt( lAttributeSize );
                         GetIntegerFromAttribute( mi_lAttributeSize, vUserSpec, "M_ER_Attribute", "Lth" );
            lAttributeSize = mi_lAttributeSize.intValue( );}
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



}
