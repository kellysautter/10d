#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "ZDRVROPR.H"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZEIDONOP.H"

static zSHORT
oTZRPSRCO_fnSetCtrlPrintValues( zVIEW     vReportDef,
                                zPCHAR    szText,
                                zLONG     lPage,
                                zLONG     lPosX,
                                zLONG     lPosY );


static zSHORT
oTZRPSRCO_fnPageStateRemove( zVIEW     vReportDef,
                             zLONG     lPageNbr );


static zSHORT
oTZRPSRCO_PrintPage( zVIEW     vSubtask,
                     zVIEW     vSourceHierOI,
                     zVIEW     vReportDef,
                     zLONG     lPageNbr,
                     zLONG     lPageWidth,
                     zLONG     lPageHeight,
                     zPCHAR    szRptGroupSetTag,
                     zPCHAR    szRptGroupTag,
                     zLONG     lFlags );


static zSHORT
oTZRPSRCO_FormatGroupOnPage( zVIEW     vReportDef,
                             zVIEW     vSourceHierOI,
                             zVIEW     vSubtask,
                             zPLONG    plNewPosX,
                             zPLONG    plNewPosY,
                             zLONG     lCurrentPosX,
                             zLONG     lCurrentPosY,
                             zLONG     lRemainingSpaceOnPageY,
                             zLONG     lContinuedGroupFlag,
                             zLONG     lPageWidth,
                             zLONG     lPageHeight,
                             zLONG     lOrigRemainingSpaceOnPageY,
                             zLONG     lFooterPosPageBottom );


static zSHORT
oTZRPSRCO_ProcessFooters( zVIEW     vReportDef,
                          zVIEW     vSubtask,
                          zPCHAR    szCurrentEntityName,
                          zLONG     lReturnLevel,
                          zLONG     lRemainingSpaceOnPageY,
                          zLONG     lBottomOfPage,
                          zPLONG    plCurrentPosX,
                          zPLONG    plCurrentPosY,
                          zPLONG    plRepeatHorizontal,
                          zLONG     lPageWidth,
                          zLONG     lPageHeight,
                          zLONG     bForcePageBreak );


static zSHORT
oTZRPSRCO_ProcessPageFooter( zVIEW     vReportDef,
                             zVIEW     vSourceHierOI,
                             zVIEW     vSubtask,
                             zPLONG    plNewPosX,
                             zPLONG    plNewPosY,
                             zLONG     lCurrentPosX,
                             zLONG     lCurrentPosY,
                             zLONG     lPageWidth,
                             zLONG     lPageHeight,
                             zLONG     lFlags );


static zSHORT
oTZRPSRCO_GroupSuppressReason( zVIEW     vReportDef,
                               zLONG     lPageNbr,
                               zLONG     lTotalPageCnt );


static zSHORT
oTZRPSRCO_fnFormatGroupOnPage( zVIEW     vReportDef,
                               zVIEW     vSourceHierOI,
                               zVIEW     vSubtask,
                               zPLONG    plNewPosX,
                               zPLONG    plNewPosY,
                               zLONG     lCurrentPosX,
                               zLONG     lCurrentPosY,
                               zLONG     lRemainingSpaceOnPageY,
                               zLONG     lContinuedGroupFlag,
                               zLONG     lPageWidth,
                               zLONG     lPageHeight,
                               zLONG     lOrigRemainingSpaceOnPageY,
                               zLONG     lFooterPosPageBottom );


static zSHORT
oTZRPSRCO_ProcessPgDetailHeadrs( zVIEW     vReportDef,
                                 zVIEW     vSourceHierOI,
                                 zPCHAR    szCurrentEntityName,
                                 zVIEW     vSubtask,
                                 zPLONG    plNewPosX,
                                 zPLONG    plNewPosY,
                                 zLONG     lCurrentPosX,
                                 zLONG     lCurrentPosY,
                                 zPLONG    lRemainingSpaceOnPageY,
                                 zLONG     lBottomOfPage,
                                 zLONG     lPageWidth,
                                 zLONG     lPageHeight );


static zSHORT
oTZRPSRCO_fnSetGroupCtrlStatus( zVIEW     vReportDefIn,
                                zSHORT    nStatus );


static zSHORT
oTZRPSRCO_fnPageStateAdd( zVIEW     vReportDef,
                          zLONG     lPageNbr );


//:TRANSFORMATION OPERATION
//:FormatDocPage( VIEW    vSubtask,
//:               INTEGER lPageNbr,
//:               INTEGER lPageWidth,
//:               INTEGER lPageHeight )

//:   VIEW         vReportDef         BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_FormatDocPage( zVIEW     vSubtask,
                         zLONG     lPageNbr,
                         zLONG     lPageWidth,
                         zLONG     lPageHeight )
{
   zVIEW     vReportDef = 0;
   //:VIEW         vReportDef2        BASED ON LOD TZRPSRCO
   zVIEW     vReportDef2 = 0;
   //:VIEW         vSourceOI
   zVIEW     vSourceOI = 0;
   //:// VIEW         vDeferredOI
   //:INTEGER      bForcePageBreak
   zLONG     bForcePageBreak = 0;
   //:VIEW         vFooterDidNotFit
   zVIEW     vFooterDidNotFit = 0;
   //:VIEW         vSourceHierOI
   zVIEW     vSourceHierOI = 0;
   //:VIEW         vSourceCurrentHierOI
   zVIEW     vSourceCurrentHierOI = 0;
   //:VIEW         vSourcePreviousHierOI
   zVIEW     vSourcePreviousHierOI = 0;
   //:// VIEW         vFooterHierOI
   //:VIEW         vTempOI
   zVIEW     vTempOI = 0;
   //:VIEW         vTemp
   zVIEW     vTemp = 0;
   //:INTEGER      lTempView
   zLONG     lTempView = 0;
   //:STRING (32)  szCurrentEntityName
   zCHAR     szCurrentEntityName[ 33 ] = { 0 };
   //:STRING (32)  szSubobjectEntityName
   zCHAR     szSubobjectEntityName[ 33 ] = { 0 };
   //:STRING (32)  szFaceName      // LF_FACENAME is 32
   zCHAR     szFaceName[ 33 ] = { 0 };
   //:STRING (256) szViewName
   zCHAR     szViewName[ 257 ] = { 0 };
   //:INTEGER      lReturnLevel
   zLONG     lReturnLevel = 0;
   //:SHORT        nPageCnt
   zSHORT    nPageCnt = 0;
   //:SHORT        nRC
   zSHORT    nRC = 0;
   //:SHORT        nCase
   zSHORT    nCase = 0;
   //:SHORT        nPreviousPageNbr
   zSHORT    nPreviousPageNbr = 0;
   //:INTEGER      lStartLevel
   zLONG     lStartLevel = 0;
   //:INTEGER      lPrintFlags
   zLONG     lPrintFlags = 0;
   //:INTEGER      lWork
   zLONG     lWork = 0;
   //:INTEGER      lDefaultFontSize
   zLONG     lDefaultFontSize = 0;
   //:INTEGER      lFontSize
   zLONG     lFontSize = 0;
   //:INTEGER      lEntityKey
   zLONG     lEntityKey = 0;
   //:INTEGER      lNextPagePrinted
   zLONG     lNextPagePrinted = 0;
   //:INTEGER      lCurrentPagePrinted
   zLONG     lCurrentPagePrinted = 0;
   //:INTEGER      lPreviousPagePrinted
   zLONG     lPreviousPagePrinted = 0;
   //:STRING (32)  szHierEntityName
   zCHAR     szHierEntityName[ 33 ] = { 0 };
   //:STRING (32)  szRptEntityName
   zCHAR     szRptEntityName[ 33 ] = { 0 };
   //:STRING (32)  szRptGroupSetTag
   zCHAR     szRptGroupSetTag[ 33 ] = { 0 };
   //:STRING (32)  szRptGroupTag
   zCHAR     szRptGroupTag[ 33 ] = { 0 };
   //:STRING (32)  szRptContinuedControlTag
   zCHAR     szRptContinuedControlTag[ 33 ] = { 0 };
   //:INTEGER      lMultiLineTextPos
   zLONG     lMultiLineTextPos = 0;
   //:INTEGER      lContinuedGroupFlag
   zLONG     lContinuedGroupFlag = 0;
   //:INTEGER      lHierRC
   zLONG     lHierRC = 0;
   //:INTEGER      lLastLevel
   zLONG     lLastLevel = 0;
   //:INTEGER      bValidView
   zLONG     bValidView = 0;
   //:INTEGER      lRptPos
   zLONG     lRptPos = 0;
   zSHORT    lTempInteger_0;
   zSHORT    RESULT;


   //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
   //://UNDEF    TraceLineI( "FormatDocPage Nbr: ", lPageNbr )
   //://UNDEF END

   //:IF lPageNbr < 0  // here on BeginPrint (-1) or EndPrint (-3) or
   if ( lPageNbr < 0 )
   {
      //:              //  BeginPrintPreview (-2) or EndPrintPreview (-4)

      //:// IF ( lPageNbr = -1 AND
      //://      vReportDef.Report.TotalPageCnt != 0 )
      //://
      //://    vReportDef.Report.TotalPageCnt = 0
      //://    lPageNbr = 0
      //:// ELSE
      //:   RETURN 0
      return( 0 );
   }

   //:// END
   //:END

   //:// Get the named views of the report definition and of the OI to be
   //:// printed.  If these do not exist, we don't belong here ... so get out
   //:// GET VIEW vReportDef NAMED "TZ_ReportDef"
   //:// GET VIEW vSourceOI NAMED "TZ_SourceOrigOI"
   //:// GET VIEW vSourceHierOI NAMED "TZ_SourceHierOI"
   //:szViewName = "TZ_ReportDef"
   ZeidonStringCopy( szViewName, 1, 0, "TZ_ReportDef", 1, 0, 257 );
   //:nRC = GetViewByName( vReportDef, szViewName, vSubtask, zLEVEL_ANY )
   nRC = GetViewByName( &vReportDef, szViewName, vSubtask, zLEVEL_ANY );
   //:szViewName = "TZ_SourceOrigOI"
   ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceOrigOI", 1, 0, 257 );
   //:nRC = GetViewByName( vSourceOI, szViewName, vSubtask, zLEVEL_ANY )
   nRC = GetViewByName( &vSourceOI, szViewName, vSubtask, zLEVEL_ANY );
   //:szViewName = "TZ_SourceHierOI"
   ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceHierOI", 1, 0, 257 );
   //:nRC = GetViewByName( vSourceHierOI, szViewName, vSubtask, zLEVEL_ANY )
   nRC = GetViewByName( &vSourceHierOI, szViewName, vSubtask, zLEVEL_ANY );

   //:IF vSourceHierOI != 0
   if ( vSourceHierOI != 0 )
   {
      //:bValidView = IsValidView( vSourceHierOI )
      bValidView = IsValidView( vSourceHierOI );
      //:IF bValidView = 0
      if ( bValidView == 0 )
      {
         //:bValidView = bValidView / bValidView
         bValidView = bValidView / bValidView;
      }

      //:END
   }

   //:END

   //:// IF nRC <= 0
   //://    vSourceHierOI = 0  <<=== this is done by GetViewByName
   //:// END

   //:// GET VIEW vSourceCurrentHierOI NAMED "TZ_SourceCurrentHierOI"
   //:// GET VIEW vSourcePreviousHierOI NAMED "TZ_SourcePreviousHierOI"
   //:szViewName = "TZ_SourceCurrentHierOI"
   ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceCurrentHierOI", 1, 0, 257 );
   //:nRC = GetViewByName( vSourceCurrentHierOI, szViewName, vSubtask, zLEVEL_ANY )
   nRC = GetViewByName( &vSourceCurrentHierOI, szViewName, vSubtask, zLEVEL_ANY );
   //:szViewName = "TZ_SourcePreviousHierOI"
   ZeidonStringCopy( szViewName, 1, 0, "TZ_SourcePreviousHierOI", 1, 0, 257 );
   //:nRC = GetViewByName( vSourcePreviousHierOI, szViewName, vSubtask, zLEVEL_ANY )
   nRC = GetViewByName( &vSourcePreviousHierOI, szViewName, vSubtask, zLEVEL_ANY );

   //://IF (vReportDef = 0 OR vSourceOI = 0 OR
   //://    (lPageNbr > 1 AND vSourceHierOI = 0))
   //:IF ( vReportDef = 0 OR vSourceOI = 0 )
   if ( vReportDef == 0 || vSourceOI == 0 )
   {

      //:TraceLineS( "PrintPage views dropped!", "" )
      TraceLineS( "PrintPage views dropped!", "" );
      //:RETURN -1   // get out of Dodge pronto!!!
      return( -1 );
   }


   //:END

   //:// Initialize variables.
   //:szSubobjectEntityName = vReportDef.Report.WK_SubobjectEntityName
   GetVariableFromAttribute( szSubobjectEntityName, 0, 'S', 33, vReportDef, "Report", "WK_SubobjectEntityName", "", 0 );
   //:lPrintFlags           = vReportDef.Report.PrintFlags
   GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );
   //:lCurrentPagePrinted   = vReportDef.Report.CurrentPageNbr
   GetIntegerFromAttribute( &lCurrentPagePrinted, vReportDef, "Report", "CurrentPageNbr" );
   //:lNextPagePrinted      = lCurrentPagePrinted + 1
   lNextPagePrinted = lCurrentPagePrinted + 1;
   //:lPreviousPagePrinted  = lCurrentPagePrinted - 1
   lPreviousPagePrinted = lCurrentPagePrinted - 1;
   //:nPreviousPageNbr      = lPageNbr - 1
   nPreviousPageNbr = lPageNbr - 1;

   //:lEntityKey = -1
   lEntityKey = -1;
   //:lHierRC = zCURSOR_NULL
   lHierRC = zCURSOR_NULL;
   //:lStartLevel = 0
   lStartLevel = 0;
   //:lLastLevel = 0
   lLastLevel = 0;
   //:lMultiLineTextPos = 0
   lMultiLineTextPos = 0;
   //:lContinuedGroupFlag = 0
   lContinuedGroupFlag = 0;
   //:szCurrentEntityName = szSubobjectEntityName
   ZeidonStringCopy( szCurrentEntityName, 1, 0, szSubobjectEntityName, 1, 0, 33 );
   //:szRptGroupSetTag = ""
   ZeidonStringCopy( szRptGroupSetTag, 1, 0, "", 1, 0, 33 );
   //:szRptGroupTag = ""
   ZeidonStringCopy( szRptGroupTag, 1, 0, "", 1, 0, 33 );
   //:szRptContinuedControlTag = ""
   ZeidonStringCopy( szRptContinuedControlTag, 1, 0, "", 1, 0, 33 );
   //:szHierEntityName = ""
   ZeidonStringCopy( szHierEntityName, 1, 0, "", 1, 0, 33 );
   //:szRptEntityName = "Report"
   ZeidonStringCopy( szRptEntityName, 1, 0, "Report", 1, 0, 33 );
   //:lRptPos = 0
   lRptPos = 0;
   //:nRC = zCURSOR_NULL
   nRC = zCURSOR_NULL;
   //:vFooterDidNotFit = 0
   vFooterDidNotFit = 0;
   //:vReportDef.Report.FooterDidNotFit = 0
   SetAttributeFromInteger( vReportDef, "Report", "FooterDidNotFit", 0 );

   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////
   //://
   //:// Keep track of levels and entity names in case we come back to a
   //:// page (print preview).  Note that the initial state of the current
   //:// page comes from the final state of the previous page.  Level
   //:// information for a given page is maintained at that page.
   //://
   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////

   //:// Force at least one PageNbr entity to exist (page number zero).
   //:IF vReportDef.PageState DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "PageState" );
   if ( lTempInteger_0 != 0 )
   {

      //:// The state for page number zero will never change.
      //:fnPageStateAdd( vReportDef, 0 )
      oTZRPSRCO_fnPageStateAdd( vReportDef, 0 );

      //:// If it's the first time on page 1, create the default font.
      //:lDefaultFontSize = vReportDef.Report.ReportTextSize
      GetIntegerFromAttribute( &lDefaultFontSize, vReportDef, "Report", "ReportTextSize" );
      //:szViewName = "ReportFont"
      ZeidonStringCopy( szViewName, 1, 0, "ReportFont", 1, 0, 257 );
      //:GetWorkstationApplicationValues( vSubtask, szViewName,
      //:                                 szFaceName, 32, lFontSize,
      //:                                 lWork, lWork, lWork, lWork,
      //:                                 lWork, lWork, lWork, lWork, lWork )
      GetWorkstationApplicationValues( vSubtask, szViewName, szFaceName, 32, &lFontSize, &lWork, &lWork, &lWork, &lWork, &lWork, &lWork, &lWork, &lWork, &lWork );
      //:IF lDefaultFontSize = 0
      if ( lDefaultFontSize == 0 )
      {
         //:lDefaultFontSize = lFontSize
         lDefaultFontSize = lFontSize;
      }

      //:END

      //:CreateFontForReport( vSubtask, "default", lDefaultFontSize,
      //:                     FALSE, FALSE, FALSE, FALSE,
      //:                     -16777216, -16777216, 0, szFaceName )
      CreateFontForReport( vSubtask, "default", lDefaultFontSize, FALSE, FALSE, FALSE, FALSE, -16777216, -16777216, 0, szFaceName );
   }

   //:END

   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////
   //:// There are five possible cases:
   //:// 1. We are starting report at page 1.
   //:// 2. We are starting report at later than page 1.
   //:// 3. We are going to a next page.
   //:// 4. We are staying on the same page (for example zoom).
   //:// 5. We are going to the previous page and we have a view to previous
   //://    page position.
   //:// 6. We are going to the previous page and we DON'T have a view to
   //://    previous page position.
   //:// 7. We are only counting pages.

   //:// For case 1, we will go to INITIALIZE PAGE ONE. In this case we are
   //:// simply starting with the first page.  vSourceOI is pointing to
   //:// beginning of data for report.

   //:// For case 2, we will first go to INITIALIZE PAGE ONE and then begin
   //:// Phantom Print to skip pages until we get to the first page to be
   //:// printed. vSourceOI is pointing to beginning of data for report.

   //:// For case 3, we will use vSourceHierOI and go to INITIALIZE PAGE
   //:// INFORMATION FOR NEXT PAGE, CURRENT PAGE OR SINGLE PREVIOUS PAGE to
   //:// initialize information to next page. vSourceHierOI is pointing to
   //:// beginning of data for next page.

   //:// For case 4, we will use vSourceCurrentHierOI and go to INITIALIZE PAGE
   //:// INFORMATION FOR NEXT PAGE, CURRENT PAGE OR SINGLE PREVIOUS PAGE.
   //:// vSourceCurrentHierOI is pointing to the beginning of data for current
   //:// page.

   //:// For case 5, we will allow stepping back one page using view
   //:// vSourcePreviousHierOI by going to INITIALIZE PAGE INFORMATION FOR
   //:// NEXT PAGE, CURRENT PAGE OR SINGLE PREVIOUS PAGE.

   //:// For case 6, we did not have a view to the previous page so we must use
   //:// Phantom Print to start on page one and skip from page one to the
   //:// previous page. We will thus go to INITIALIZE PAGE ONE to start the
   //:// process.

   //:// For case 7, we will first go to INITIALIZE PAGE ONE and then begin
   //:// Phantom Print to skip all pages.
   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////

   //:// Determine cases.
   //:IF lPageNbr = 1
   if ( lPageNbr == 1 )
   {

      //:// 1. We are starting report at page 1.
      //:nCase = 1
      nCase = 1;
   }

   //:END

   //:IF lPageNbr > 1 AND lNextPagePrinted = 1
   if ( lPageNbr > 1 && lNextPagePrinted == 1 )
   {

      //:// 2. We are starting report at later than page 1.
      //:nCase = 2
      nCase = 2;
   }

   //:END

   //:IF lPageNbr > 1 AND lNextPagePrinted = lPageNbr
   if ( lPageNbr > 1 && lNextPagePrinted == lPageNbr )
   {

      //:// 3. We are going to a next page.
      //://    Reset Previous and Current views.
      //://    Only drop Previous view if it is different from Current view.
      //://    If it is the same, we must have paged back and then forward.
      //://    The page back eliminated a separate Previous view.
      //:nCase = 3
      nCase = 3;
      //:IF vSourcePreviousHierOI != vSourceCurrentHierOI
      if ( vSourcePreviousHierOI != vSourceCurrentHierOI )
      {
         //://UNDVV  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDVV     TraceLineV( "(nCase=3) DropView( vSourcePreviousHierOI ): ",
         //://UNDVV                 vSourcePreviousHierOI )
         //://UNDVV  END
         //:// DropViewTrace( "Dropping view (1): ", vSourcePreviousHierOI )
         //:DropView( vSourcePreviousHierOI )
         DropView( vSourcePreviousHierOI );
         //:vSourcePreviousHierOI = 0
         vSourcePreviousHierOI = 0;
      }

      //:END

      //:vSourcePreviousHierOI = vSourceCurrentHierOI
      vSourcePreviousHierOI = vSourceCurrentHierOI;
      //:NAME VIEW vSourcePreviousHierOI "TZ_SourcePreviousHierOI"
      SetNameForView( vSourcePreviousHierOI, "TZ_SourcePreviousHierOI", 0, zLEVEL_TASK );
      //:// CreateViewTrace( "Creating view (1): ", vSourceCurrentHierOI, vSourceHierOI, 0 )
      //:CreateViewFromViewForTask( vSourceCurrentHierOI, vSourceHierOI, 0 )
      CreateViewFromViewForTask( &vSourceCurrentHierOI, vSourceHierOI, 0 );
      //:NAME VIEW vSourceCurrentHierOI "TZ_SourceCurrentHierOI"
      SetNameForView( vSourceCurrentHierOI, "TZ_SourceCurrentHierOI", 0, zLEVEL_TASK );
   }

   //:END

   //:IF lPageNbr > 1 AND lCurrentPagePrinted = lPageNbr
   if ( lPageNbr > 1 && lCurrentPagePrinted == lPageNbr )
   {

      //:// 4. We are staying on the same page (for example zoom).
      //://    Reset Next view to Current.
      //:nCase = 4
      nCase = 4;

      //://UNDVV  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
      //://UNDVV     TraceLineV( "(nCase=4) DropView( vSourceHierOI ): ",
      //://UNDVV                 vSourceHierOI )
      //://UNDVV  END
      //:// DropViewTrace( "Dropping view (2): ", vSourceHierOI )
      //:DropView( vSourceHierOI )
      DropView( vSourceHierOI );
      //:// CreateViewTrace( "Creating view (2): ", vSourceHierOI, vSourceCurrentHierOI, 0 )
      //:CreateViewFromViewForTask( vSourceHierOI, vSourceCurrentHierOI, 0 )
      CreateViewFromViewForTask( &vSourceHierOI, vSourceCurrentHierOI, 0 );
      //:NAME VIEW vSourceHierOI "TZ_SourceHierOI"
      SetNameForView( vSourceHierOI, "TZ_SourceHierOI", 0, zLEVEL_TASK );
      //:SET CURSOR FIRST vReportDef.PageState WHERE
      //:                 vReportDef.PageState.PageNbr = nPreviousPageNbr
      RESULT = SetCursorFirstEntityByInteger( vReportDef, "PageState", "PageNbr", nPreviousPageNbr, "" );
      //:szCurrentEntityName = vReportDef.PageState.CurrentEntityName
      GetVariableFromAttribute( szCurrentEntityName, 0, 'S', 33, vReportDef, "PageState", "CurrentEntityName", "", 0 );
      //:DefineHierarchicalCursor( vSourceHierOI, szCurrentEntityName )
      DefineHierarchicalCursor( vSourceHierOI, szCurrentEntityName );
      //:SET CURSOR FIRST vReportDef.GroupSet WHERE
      //:                 vReportDef.GroupSet.Tag = szCurrentEntityName
      RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szCurrentEntityName, "" );
   }

   //:END

   //:IF lPageNbr > 1 AND
   //:   lPageNbr < lCurrentPagePrinted
   if ( lPageNbr > 1 && lPageNbr < lCurrentPagePrinted )
   {

      //:IF vSourceCurrentHierOI != vSourcePreviousHierOI
      if ( vSourceCurrentHierOI != vSourcePreviousHierOI )
      {

         //:// 5. We are going to the previous page and we have a view to
         //://    previous page position.  Reset Current and Next views.
         //://    Previous view remains the same.
         //:nCase = 5
         nCase = 5;

         //://UNDVV  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDVV     lTempView = MakeIntegerFromView( vSourceHierOI )
         //://UNDVV     TraceLineV( "(nCase=5) DropView( vSourceHierOI ): ",
         //://UNDVV                 vSourceHierOI )
         //://UNDVV  END
         //:// DropViewTrace( "Dropping view (3): ", vSourceHierOI )
         //:DropView( vSourceHierOI )
         DropView( vSourceHierOI );
         //:// CreateViewTrace( "Creating view (3): ", vSourceHierOI, vSourcePreviousHierOI, 0 )
         //:CreateViewFromViewForTask( vSourceHierOI, vSourcePreviousHierOI, 0 )
         CreateViewFromViewForTask( &vSourceHierOI, vSourcePreviousHierOI, 0 );
         //:NAME VIEW vSourceHierOI "TZ_SourceHierOI"
         SetNameForView( vSourceHierOI, "TZ_SourceHierOI", 0, zLEVEL_TASK );
         //:nPreviousPageNbr = nPreviousPageNbr
         nPreviousPageNbr = nPreviousPageNbr;
         //:SET CURSOR FIRST vReportDef.PageState WHERE
         //:                 vReportDef.PageState.PageNbr = nPreviousPageNbr
         RESULT = SetCursorFirstEntityByInteger( vReportDef, "PageState", "PageNbr", nPreviousPageNbr, "" );
         //:szCurrentEntityName = vReportDef.PageState.CurrentEntityName
         GetVariableFromAttribute( szCurrentEntityName, 0, 'S', 33, vReportDef, "PageState", "CurrentEntityName", "", 0 );
         //:DefineHierarchicalCursor( vSourceHierOI, szCurrentEntityName )
         DefineHierarchicalCursor( vSourceHierOI, szCurrentEntityName );
         //:SET CURSOR FIRST vReportDef.GroupSet WHERE
         //:                 vReportDef.GroupSet.Tag = szCurrentEntityName
         RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szCurrentEntityName, "" );
         //://UNDVV  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDVV     TraceLineV( "(nCase=5) DropView( vSourceCurrentHierOI ): ",
         //://UNDVV                 vSourceCurrentHierOI )
         //://UNDVV  END
         //:// DropViewTrace( "Dropping view (4): ", vSourceCurrentHierOI )
         //:DropView( vSourceCurrentHierOI )
         DropView( vSourceCurrentHierOI );
         //:vSourceCurrentHierOI = vSourcePreviousHierOI
         vSourceCurrentHierOI = vSourcePreviousHierOI;
         //:NAME VIEW vSourceCurrentHierOI "TZ_SourceCurrentHierOI"
         SetNameForView( vSourceCurrentHierOI, "TZ_SourceCurrentHierOI", 0, zLEVEL_TASK );

         //:ELSE
      }
      else
      {

         //:// 6. We are going to the previous page and we DON'T have a view to
         //://    previous page position.
         //:nCase = 6
         nCase = 6;
      }


      //:END
   }

   //:END

   //:IF lPageNbr = 0
   if ( lPageNbr == 0 )
   {

      //:// 7. We are only counting pages.
      //:nCase = 7
      nCase = 7;
   }

   //:END

   //:IF vSourceHierOI != 0
   if ( vSourceHierOI != 0 )
   {
      //:bValidView = IsValidView( vSourceHierOI )
      bValidView = IsValidView( vSourceHierOI );
      //:IF bValidView = 0
      if ( bValidView == 0 )
      {
         //:bValidView = bValidView / bValidView
         bValidView = bValidView / bValidView;
      }

      //:END
   }

   //:END

   //:IF nCase = 3 OR
   //:   nCase = 4 OR
   //:   nCase = 5
   if ( nCase == 3 || nCase == 4 || nCase == 5 )
   {

      //:// Cases 3, 4 and 5

      //:// INITIALIZE PAGE INFORMATION FOR NEXT PAGE, CURRENT PAGE, OR
      //:// SINGLE PREVIOUS PAGE

      //:// Delete all state entities for this page and beyond.
      //:fnPageStateRemove( vReportDef, lPageNbr )
      oTZRPSRCO_fnPageStateRemove( vReportDef, lPageNbr );

      //:// Retrieve state information from the report definition for the
      //:// current page (from the final state of the previous page).
      //:SET CURSOR FIRST vReportDef.PageState WHERE
      //:                 vReportDef.PageState.PageNbr = nPreviousPageNbr
      RESULT = SetCursorFirstEntityByInteger( vReportDef, "PageState", "PageNbr", nPreviousPageNbr, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      {
         //:lEntityKey = vReportDef.PageState.AbsolutePos
         GetIntegerFromAttribute( &lEntityKey, vReportDef, "PageState", "AbsolutePos" );
         //:bForcePageBreak = vReportDef.PageState.ForcePageBreak
         GetIntegerFromAttribute( &bForcePageBreak, vReportDef, "PageState", "ForcePageBreak" );
         //:vFooterDidNotFit = vReportDef.PageState.FooterDidNotFit
         GetIntegerFromAttribute( (zLONG *) &vFooterDidNotFit, vReportDef, "PageState", "FooterDidNotFit" );

         //:// IF vFooterDidNotFit != 0
         //://    bValidView = IsValidView( vFooterDidNotFit )
         //://    IF bValidView = FALSE
         //://       NAME VIEW vReportDef "DKS_PageState.FooterDidNotFit"
         //://    // bValidView = bValidView / bValidView
         //://       TraceLineV( "FooterDidNotFit View is Invalid (0): ", vFooterDidNotFit )
         //://    // SysMessageBox( vSubtask, "PageState.FooterDidNotFit", "Invalid View 0", TRUE )
         //://    ELSE
         //://       TraceLineV( "FooterDidNotFit View is Valid (0): ", vFooterDidNotFit )
         //://    END
         //:// END

         //:lHierRC = vReportDef.PageState.HierReturnCode
         GetIntegerFromAttribute( &lHierRC, vReportDef, "PageState", "HierReturnCode" );
         //:lStartLevel = vReportDef.PageState.StartLevel
         GetIntegerFromAttribute( &lStartLevel, vReportDef, "PageState", "StartLevel" );
         //:lLastLevel = vReportDef.PageState.LastLevel
         GetIntegerFromAttribute( &lLastLevel, vReportDef, "PageState", "LastLevel" );
         //:lMultiLineTextPos = vReportDef.PageState.MultiLineTextPosEndPage
         GetIntegerFromAttribute( &lMultiLineTextPos, vReportDef, "PageState", "MultiLineTextPosEndPage" );
         //:lContinuedGroupFlag = vReportDef.PageState.ContinuedGroupFlagEndPage
         GetIntegerFromAttribute( &lContinuedGroupFlag, vReportDef, "PageState", "ContinuedGroupFlagEndPage" );
         //:szRptContinuedControlTag = vReportDef.PageState.ContinuedControlTagEndPage
         GetVariableFromAttribute( szRptContinuedControlTag, 0, 'S', 33, vReportDef, "PageState", "ContinuedControlTagEndPage", "", 0 );
         //:szCurrentEntityName = vReportDef.PageState.CurrentEntityName
         GetVariableFromAttribute( szCurrentEntityName, 0, 'S', 33, vReportDef, "PageState", "CurrentEntityName", "", 0 );
         //:szRptGroupSetTag = vReportDef.PageState.GroupSetTag
         GetVariableFromAttribute( szRptGroupSetTag, 0, 'S', 33, vReportDef, "PageState", "GroupSetTag", "", 0 );
         //:szRptGroupTag = vReportDef.PageState.GroupTag
         GetVariableFromAttribute( szRptGroupTag, 0, 'S', 33, vReportDef, "PageState", "GroupTag", "", 0 );
         //:nRC = zCURSOR_SET
         nRC = zCURSOR_SET;
      }

      //:END

      //:// Initialize state for this page.
      //:fnPageStateAdd( vReportDef, lPageNbr )
      oTZRPSRCO_fnPageStateAdd( vReportDef, lPageNbr );
      //:vReportDef.Report.CurrentPageNbr = lPageNbr
      SetAttributeFromInteger( vReportDef, "Report", "CurrentPageNbr", lPageNbr );

      //:// Restore current page state information to the report definition.
      //:vReportDef.Report.AbsolutePos = lEntityKey
      SetAttributeFromInteger( vReportDef, "Report", "AbsolutePos", lEntityKey );
      //:vReportDef.Report.ForcePageBreak = bForcePageBreak
      SetAttributeFromInteger( vReportDef, "Report", "ForcePageBreak", bForcePageBreak );

      //:vTemp = vReportDef.Report.FooterDidNotFit
      GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "Report", "FooterDidNotFit" );
      //:IF vTemp != 0
      if ( vTemp != 0 )
      {
         //:// DropViewTrace( "Dropping view (4a): ", vTemp )
         //:DropView( vTemp )
         DropView( vTemp );
         //:vTemp = 0
         vTemp = 0;
      }

      //:END

      //:IF vFooterDidNotFit != 0
      if ( vFooterDidNotFit != 0 )
      {
         //:// bValidView = IsValidView( vFooterDidNotFit )
         //:// IF bValidView = FALSE
         //:// // bValidView = bValidView / bValidView
         //://    TraceLineV( "FooterDidNotFit View is Invalid (1): ", vFooterDidNotFit )
         //:// // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 1", TRUE )
         //:// ELSE
         //://    TraceLineV( "FooterDidNotFit View is Valid (1): ", vFooterDidNotFit )
         //:// END

         //:// CreateViewTrace( "Creating view (4a): ", vTemp, vFooterDidNotFit, 0 )
         //:CreateViewFromViewForTask( vTemp, vFooterDidNotFit, 0 )
         CreateViewFromViewForTask( &vTemp, vFooterDidNotFit, 0 );
         //:lTempView = MakeIntegerFromView( vTemp )
         lTempView = MakeIntegerFromView( vTemp );

         //:ELSE
      }
      else
      {

         //:lTempView = 0
         lTempView = 0;
      }


      //:END

      //:vReportDef.Report.FooterDidNotFit = lTempView
      SetAttributeFromInteger( vReportDef, "Report", "FooterDidNotFit", lTempView );
      //:vReportDef.Report.HierReturnCode = lHierRC
      SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lHierRC );
      //:vReportDef.Report.StartLevel = lStartLevel
      SetAttributeFromInteger( vReportDef, "Report", "StartLevel", lStartLevel );
      //:vReportDef.Report.LastLevel = lLastLevel
      SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lLastLevel );
      //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
      SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
      //:vReportDef.Report.GroupSetTag = szRptGroupSetTag
      SetAttributeFromString( vReportDef, "Report", "GroupSetTag", szRptGroupSetTag );
      //:vReportDef.Report.GroupTag = szRptGroupTag
      SetAttributeFromString( vReportDef, "Report", "GroupTag", szRptGroupTag );

      //:ELSE
   }
   else
   {

      //:// Cases 1, 2, 6 and 7

      //:// INITIALIZE PAGE ONE

      //:// We get here for every case except for paging to the next page.
      //:// Note: this includes when skipping to a particular page to print.
      //:// On page one, we don't want any existing vSourceHierOI view, so if
      //:// it exists from prior report, delete it.
      //:// If vSourceHierOI = 0, we have no data for the report but will have
      //:// gotton here anyway.
      //:IF vSourceHierOI != 0
      if ( vSourceHierOI != 0 )
      {
         //:DropHierarchicalCursor( vSourceHierOI )
         DropHierarchicalCursor( vSourceHierOI );
         //://UNDVV  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDVV     TraceLineV( "(Cases 1, 2, 6 and 7) DropView( vSourceHierOI ): ",
         //://UNDVV                 vSourceHierOI )
         //://UNDVV  END

         //:// DropViewTrace( "Dropping view (5): ", vSourceHierOI )
         //:DropView( vSourceHierOI )
         DropView( vSourceHierOI );
         //:vSourceHierOI = 0
         vSourceHierOI = 0;
      }

      //:END

      //:// Create a special view for hierarchical processing.
      //:nRC = CheckExistenceOfEntity( vSourceOI, szSubobjectEntityName )
      nRC = CheckExistenceOfEntity( vSourceOI, szSubobjectEntityName );
      //:IF nRC = 0
      if ( nRC == 0 )
      {
         //:// CreateViewTrace( "Creating view (4): ", vSourceHierOI, vSourceOI, 0 )
         //:CreateViewFromViewForTask( vSourceHierOI, vSourceOI, 0 )
         CreateViewFromViewForTask( &vSourceHierOI, vSourceOI, 0 );
         //:DefineHierarchicalCursor( vSourceHierOI, szSubobjectEntityName )
         DefineHierarchicalCursor( vSourceHierOI, szSubobjectEntityName );
         //:NAME VIEW vSourceHierOI "TZ_SourceHierOI"
         SetNameForView( vSourceHierOI, "TZ_SourceHierOI", 0, zLEVEL_TASK );

         //:// Drop Previous and Current views, if they exist.
         //:// GET VIEW vSourceCurrentHierOI NAMED "TZ_SourceCurrentHierOI"
         //:szViewName = "TZ_SourceCurrentHierOI"
         ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceCurrentHierOI", 1, 0, 257 );
         //:nRC = GetViewByName( vSourceCurrentHierOI, szViewName,
         //:                     vSubtask, zLEVEL_ANY )
         nRC = GetViewByName( &vSourceCurrentHierOI, szViewName, vSubtask, zLEVEL_ANY );
         //:IF nRC > 0
         if ( nRC > 0 )
         {
            //://UNDVV     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
            //://UNDVV        TraceLineV( "(NAMED TZ_SourceCurrentHierOI) "
            //://UNDVV                      "DropView( vSourceCurrentHierOI ): ",
            //://UNDVV                    vSourceCurrentHierOI )
            //://UNDVV     END
            //:// DropViewTrace( "Dropping view (6): ", vSourceCurrentHierOI )
            //:DropView( vSourceCurrentHierOI )
            DropView( vSourceCurrentHierOI );
            //:vSourceCurrentHierOI = 0
            vSourceCurrentHierOI = 0;
         }

         //:END

         //:// GET VIEW vSourcePreviousHierOI NAMED "TZ_SourcePreviousHierOI"
         //:szViewName = "TZ_SourcePreviousHierOI"
         ZeidonStringCopy( szViewName, 1, 0, "TZ_SourcePreviousHierOI", 1, 0, 257 );
         //:nRC = GetViewByName( vSourcePreviousHierOI, szViewName,
         //:                     vSubtask, zLEVEL_ANY )
         nRC = GetViewByName( &vSourcePreviousHierOI, szViewName, vSubtask, zLEVEL_ANY );
         //:IF nRC > 0
         if ( nRC > 0 )
         {
            //://UNDVV     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
            //://UNDVV        TraceLineV( "(NAMED TZ_SourcePreviousHierOI) "
            //://UNDVV                      "DropView( vSourcePreviousHierOI ): ",
            //://UNDVV                    vSourcePreviousHierOI )
            //://UNDVV     END
            //:// DropViewTrace( "Dropping view (7): ", vSourcePreviousHierOI )
            //:DropView( vSourcePreviousHierOI )
            DropView( vSourcePreviousHierOI );
            //:vSourcePreviousHierOI = 0
            vSourcePreviousHierOI = 0;
         }

         //:END

         //:// Initialize Previous and Current views.
         //:// CreateViewTrace( "Creating view (6): ", vSourceCurrentHierOI, vSourceHierOI, 0 )
         //:CreateViewFromViewForTask( vSourceCurrentHierOI, vSourceHierOI, 0 )
         CreateViewFromViewForTask( &vSourceCurrentHierOI, vSourceHierOI, 0 );
         //:NAME VIEW vSourceCurrentHierOI "TZ_SourceCurrentHierOI"
         SetNameForView( vSourceCurrentHierOI, "TZ_SourceCurrentHierOI", 0, zLEVEL_TASK );
         //:// CreateViewTrace( "Creating view (7): ", vSourcePreviousHierOI, vSourceHierOI, 0 )
         //:CreateViewFromViewForTask( vSourcePreviousHierOI, vSourceHierOI, 0 )
         CreateViewFromViewForTask( &vSourcePreviousHierOI, vSourceHierOI, 0 );
         //:NAME VIEW vSourcePreviousHierOI "TZ_SourcePreviousHierOI"
         SetNameForView( vSourcePreviousHierOI, "TZ_SourcePreviousHierOI", 0, zLEVEL_TASK );
         //:ELSE
      }
      else
      {
         //:// If we get here, we didn't have any data and we want to print a
         //:// page with no details.  Our trigger for this will be a zero value
         //:// for vSourceHierOI.
         //:TraceLineS( "Report Entity not found: ", szSubobjectEntityName )
         TraceLineS( "Report Entity not found: ", szSubobjectEntityName );
         //:vSourceHierOI = 0
         vSourceHierOI = 0;
      }

      //:END

      //:// Initialize lNextPagePrinted to be page one.
      //:lNextPagePrinted = 1
      lNextPagePrinted = 1;

      //:lMultiLineTextPos = 0
      lMultiLineTextPos = 0;
      //:lContinuedGroupFlag = 0
      lContinuedGroupFlag = 0;

      //:// Before going to format the first page on the report, position on the
      //:// first entity in vSourceHierOI that is to be formatted on the report
      //:// and on the corresponding ReportDef entry.
      //:ResetView( vReportDef )
      ResetView( vReportDef );
      //:szCurrentEntityName = szSubobjectEntityName
      ZeidonStringCopy( szCurrentEntityName, 1, 0, szSubobjectEntityName, 1, 0, 33 );
      //:SET CURSOR FIRST vReportDef.GroupSet WHERE
      //:                 vReportDef.GroupSet.Tag = szSubobjectEntityName
      RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szSubobjectEntityName, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      {
         //:SET CURSOR FIRST vReportDef.Group WHERE
         //:                 vReportDef.Group.Type = "ga"
         RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "ga", "" );
         //:ELSE
      }
      else
      {
      }

      //:// TraceLineS( "FormatDocPage unable to locate GroupSet: ",
      //://             szCurrentEntityName )
      //:END

      //:// Delete all state entities except for page 0.
      //:fnPageStateRemove( vReportDef, 1 )
      oTZRPSRCO_fnPageStateRemove( vReportDef, 1 );

      //:// Set up initial information, particularly for the hierarchical entity
      //:// on which we are positioned at the beginning of page one.
      //:// vSourceHierOI was set to zero if we were printing an empty page
      //:// (meaning there was no data to print).
      //:IF vSourceHierOI != 0
      if ( vSourceHierOI != 0 )
      {
         //:GetEntityKeyForHierarchicalCsr( lStartLevel, szCurrentEntityName,
         //:                                lEntityKey, vSourceHierOI )
         GetEntityKeyForHierarchicalCsr( &lStartLevel, szCurrentEntityName, (zPULONG) &lEntityKey, vSourceHierOI );
         //:ELSE
      }
      else
      {
         //:lReturnLevel = 1
         lReturnLevel = 1;
         //:lStartLevel  = 1
         lStartLevel = 1;
         //:lEntityKey = -1
         lEntityKey = -1;
         //:szHierEntityName = ""
         ZeidonStringCopy( szHierEntityName, 1, 0, "", 1, 0, 33 );
      }

      //:END

      //:vReportDef.Report.AbsolutePos = lEntityKey
      SetAttributeFromInteger( vReportDef, "Report", "AbsolutePos", lEntityKey );
      //:vReportDef.Report.ForcePageBreak = bForcePageBreak
      SetAttributeFromInteger( vReportDef, "Report", "ForcePageBreak", bForcePageBreak );

      //:vTemp = vReportDef.Report.FooterDidNotFit
      GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "Report", "FooterDidNotFit" );
      //:IF vTemp != 0
      if ( vTemp != 0 )
      {
         //:// DropViewTrace( "Dropping view (4b): ", vTemp )
         //:DropView( vTemp )
         DropView( vTemp );
         //:vTemp = 0
         vTemp = 0;
      }

      //:END

      //:IF vFooterDidNotFit != 0
      if ( vFooterDidNotFit != 0 )
      {
         //:// bValidView = IsValidView( vFooterDidNotFit )
         //:// IF bValidView = FALSE
         //:// // bValidView = bValidView / bValidView
         //://    TraceLineV( "FooterDidNotFit View is Invalid (2): ", vFooterDidNotFit )
         //:// // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 2", TRUE )
         //:// ELSE
         //://    TraceLineV( "FooterDidNotFit View is Valid (2): ", vFooterDidNotFit )
         //:// END

         //:// CreateViewTrace( "Creating view (4b): ", vTemp, vFooterDidNotFit, 0 )
         //:CreateViewFromViewForTask( vTemp, vFooterDidNotFit, 0 )
         CreateViewFromViewForTask( &vTemp, vFooterDidNotFit, 0 );
         //:lTempView = MakeIntegerFromView( vTemp )
         lTempView = MakeIntegerFromView( vTemp );

         //:ELSE
      }
      else
      {

         //:lTempView = 0
         lTempView = 0;
      }


      //:END

      //:vReportDef.Report.FooterDidNotFit = lTempView
      SetAttributeFromInteger( vReportDef, "Report", "FooterDidNotFit", lTempView );
      //:vReportDef.Report.HierReturnCode = lStartLevel
      SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lStartLevel );
      //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
      SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
      //:vReportDef.Report.StartLevel = lStartLevel
      SetAttributeFromInteger( vReportDef, "Report", "StartLevel", lStartLevel );
      //:vReportDef.Report.LastLevel = lStartLevel
      SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lStartLevel );
      //:vReportDef.Report.CurrentPageNbr = 1
      SetAttributeFromInteger( vReportDef, "Report", "CurrentPageNbr", 1 );
      //:vReportDef.Report.GroupSetTag = szRptGroupSetTag
      SetAttributeFromString( vReportDef, "Report", "GroupSetTag", szRptGroupSetTag );
      //:vReportDef.Report.GroupTag = szRptGroupTag
      SetAttributeFromString( vReportDef, "Report", "GroupTag", szRptGroupTag );
      //:SetMatchingAttributesByName( vReportDef, "PageState",
      //:                             vReportDef, "Report", zSET_ALL )
      SetMatchingAttributesByName( vReportDef, "PageState", vReportDef, "Report", zSET_ALL );
   }


   //://UNDEF IF szHierEntityName != szCurrentEntityName
   //://UNDEF    TraceLineS( "Reset Position entity names differ: ",
   //://UNDEF                szHierEntityName )
   //://UNDEF    TraceLineS( "                  CurrentEntityName ",
   //://UNDEF                szCurrentEntityName )
   //://UNDEF END

   //:   // The code to create a Footer was deleted here by DC on 21.02.01.

   //:END

   //:nPageCnt = 0  // count pages for Phantom mode
   nPageCnt = 0;
   //:nRC = 1       // Initialize nRC to indicate there are pages to print.
   nRC = 1;

   //://////////////////////////////////////////////////////////////////////////
   //://
   //:// PHANTOM PRINT
   //://
   //://////////////////////////////////////////////////////////////////////////

   //:IF nCase = 2 OR
   //:   nCase = 6 OR
   //:   nCase = 7
   if ( nCase == 2 || nCase == 6 || nCase == 7 )
   {

      //:// Cases 2, 6 and 7

      //:// For case 2 and 6, we need to skip pages until we get to lPageNbr.
      //:// For case 7, we need to go through all pages

      //:IF nCase = 7
      if ( nCase == 7 )
      {
         //:lNextPagePrinted = -1
         lNextPagePrinted = -1;
      }

      //:END

      //:SetPhantomMode( vSubtask, 1 )  // print phantom pages as necessary
      SetPhantomMode( vSubtask, 1 );
      //:LOOP WHILE lNextPagePrinted < lPageNbr AND nRC > 0
      while ( lNextPagePrinted < lPageNbr && nRC > 0 )
      {

         //:nPageCnt = nPageCnt + 1
         nPageCnt = nPageCnt + 1;

         //:IF lNextPagePrinted > 0
         if ( lNextPagePrinted > 0 )
         {
            //:lWork = lNextPagePrinted
            lWork = lNextPagePrinted;
            //:ELSE
         }
         else
         {
            //:lWork = nPageCnt
            lWork = nPageCnt;
         }

         //:END

         //:SET CURSOR FIRST vReportDef.PageState WHERE
         //:                 vReportDef.PageState.PageNbr = lWork
         RESULT = SetCursorFirstEntityByInteger( vReportDef, "PageState", "PageNbr", lWork, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         {
            //:fnPageStateAdd( vReportDef, lWork )
            oTZRPSRCO_fnPageStateAdd( vReportDef, lWork );
         }

         //:END

         //:////////////////////////////////////////////////////////////////////
         //://
         //:// Print the page.
         //://
         //:////////////////////////////////////////////////////////////////////
         //:// Use the page number as specified in the call to this function.
         //://UNDEF TraceLineI( "Phantom mode for page: ", lWork )
         //:vReportDef.Report.CurrentPageNbr = lWork
         SetAttributeFromInteger( vReportDef, "Report", "CurrentPageNbr", lWork );
         //:nRC = PrintPage( vSubtask, vSourceHierOI, vReportDef,
         //:                 lWork, lPageWidth, lPageHeight,
         //:                 szRptGroupSetTag, szRptGroupTag,
         //:                 -2147483648 )  // 0x80000000  phantom print page counting
         nRC = oTZRPSRCO_PrintPage( vSubtask, vSourceHierOI, vReportDef, lWork, lPageWidth, lPageHeight, szRptGroupSetTag, szRptGroupTag, 0x80000000 );

         //:// Reset Previous and Current views.
         //://UNDVV     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDVV        TraceLineV( "(Reset Previous) DropView( vSourcePreviousHierOI ): ",
         //://UNDVV                    vSourcePreviousHierOI )
         //://UNDVV     END
         //:// DropViewTrace( "Dropping view (7a): ", vSourcePreviousHierOI )
         //:DropView( vSourcePreviousHierOI )
         DropView( vSourcePreviousHierOI );
         //:vSourcePreviousHierOI = vSourceCurrentHierOI
         vSourcePreviousHierOI = vSourceCurrentHierOI;
         //:// CreateViewTrace( "Creating view (8): ", vSourceCurrentHierOI, vSourceHierOI, 0 )
         //:CreateViewFromViewForTask( vSourceCurrentHierOI, vSourceHierOI, 0 )
         CreateViewFromViewForTask( &vSourceCurrentHierOI, vSourceHierOI, 0 );

         //:// Set up cursor information for the next page.
         //:IF ( nRC > 0 )
         if ( nRC > 0 )
         {

            //:GetEntityKeyForHierarchicalCsr( lReturnLevel, szHierEntityName,
            //:                                lEntityKey, vSourceHierOI )
            GetEntityKeyForHierarchicalCsr( &lReturnLevel, szHierEntityName, (zPULONG) &lEntityKey, vSourceHierOI );
            //://UNDEF  TraceLineS( "          Entity Name: ", szHierEntityName )
            //://UNDEF  TraceLineI( "          Entity Key: ", lEntityKey )

            //:// Retrieve initialization information from the report definition.
            //:// lEntityKey = vReportDef.Report.AbsolutePos
            //:bForcePageBreak = vReportDef.Report.ForcePageBreak
            GetIntegerFromAttribute( &bForcePageBreak, vReportDef, "Report", "ForcePageBreak" );
            //:vFooterDidNotFit = vReportDef.Report.FooterDidNotFit
            GetIntegerFromAttribute( (zLONG *) &vFooterDidNotFit, vReportDef, "Report", "FooterDidNotFit" );

            //:// IF vFooterDidNotFit != 0
            //://    bValidView = IsValidView( vFooterDidNotFit )
            //://    IF bValidView = FALSE
            //://       NAME VIEW vReportDef "DKS_PageState.FooterDidNotFit"
            //://    // bValidView = bValidView / bValidView
            //://       TraceLineV( "FooterDidNotFit View is Invalid (3): ", vFooterDidNotFit )
            //://    // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 3", TRUE )
            //://    ELSE
            //://       TraceLineV( "FooterDidNotFit View is Valid (3): ", vFooterDidNotFit )
            //://    END
            //:// END

            //:lHierRC = vReportDef.Report.HierReturnCode
            GetIntegerFromAttribute( &lHierRC, vReportDef, "Report", "HierReturnCode" );
            //:lStartLevel = vReportDef.Report.StartLevel
            GetIntegerFromAttribute( &lStartLevel, vReportDef, "Report", "StartLevel" );
            //:lLastLevel = vReportDef.Report.LastLevel
            GetIntegerFromAttribute( &lLastLevel, vReportDef, "Report", "LastLevel" );
            //:lMultiLineTextPos = vReportDef.PageState.MultiLineTextPosEndPage
            GetIntegerFromAttribute( &lMultiLineTextPos, vReportDef, "PageState", "MultiLineTextPosEndPage" );
            //:lContinuedGroupFlag =
            //:                  vReportDef.PageState.ContinuedGroupFlagEndPage
            GetIntegerFromAttribute( &lContinuedGroupFlag, vReportDef, "PageState", "ContinuedGroupFlagEndPage" );
            //:szRptContinuedControlTag =
            //:                 vReportDef.PageState.ContinuedControlTagEndPage
            GetVariableFromAttribute( szRptContinuedControlTag, 0, 'S', 33, vReportDef, "PageState", "ContinuedControlTagEndPage", "", 0 );
            //:// szCurrentEntityName = vReportDef.Report.CurrentEntityName

            //:// IF lPageNbr = 4 OR lPageNbr = 5
            //://    TraceLineI( "FormatDocPage Standard PageNbr: ", lPageNbr )
            //:// END

            //:// Initialize state for next page
            //:vReportDef.PageState.AbsolutePos = lEntityKey
            SetAttributeFromInteger( vReportDef, "PageState", "AbsolutePos", lEntityKey );
            //:vReportDef.PageState.ForcePageBreak = bForcePageBreak
            SetAttributeFromInteger( vReportDef, "PageState", "ForcePageBreak", bForcePageBreak );

            //:vTemp = vReportDef.PageState.FooterDidNotFit
            GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "PageState", "FooterDidNotFit" );
            //:IF vTemp != 0
            if ( vTemp != 0 )
            {
               //:// DropViewTrace( "Dropping view (4c): ", vTemp )
               //:DropView( vTemp )
               DropView( vTemp );
               //:vTemp = 0
               vTemp = 0;
            }

            //:END

            //:IF vFooterDidNotFit != 0
            if ( vFooterDidNotFit != 0 )
            {
               //:// bValidView = IsValidView( vFooterDidNotFit )
               //:// IF bValidView = FALSE
               //:// // bValidView = bValidView / bValidView
               //://    TraceLineV( "FooterDidNotFit View is Invalid (4): ", vFooterDidNotFit )
               //:// // SysMessageBox( vSubtask, "PageState.FooterDidNotFit", "Invalid View 4", TRUE )
               //:// ELSE
               //://    TraceLineV( "FooterDidNotFit View is Valid (4): ", vFooterDidNotFit )
               //:// END

               //:// CreateViewTrace( "Creating view (4c): ", vTemp, vFooterDidNotFit, 0 )
               //:CreateViewFromViewForTask( vTemp, vFooterDidNotFit, 0 )
               CreateViewFromViewForTask( &vTemp, vFooterDidNotFit, 0 );
               //:lTempView = MakeIntegerFromView( vTemp )
               lTempView = MakeIntegerFromView( vTemp );

               //:ELSE
            }
            else
            {

               //:lTempView = 0
               lTempView = 0;
            }


            //:END

            //:vReportDef.PageState.FooterDidNotFit = lTempView
            SetAttributeFromInteger( vReportDef, "PageState", "FooterDidNotFit", lTempView );
            //:vReportDef.PageState.HierReturnCode = lHierRC
            SetAttributeFromInteger( vReportDef, "PageState", "HierReturnCode", lHierRC );
            //:vReportDef.PageState.StartLevel = lStartLevel
            SetAttributeFromInteger( vReportDef, "PageState", "StartLevel", lStartLevel );
            //:vReportDef.PageState.LastLevel = lLastLevel
            SetAttributeFromInteger( vReportDef, "PageState", "LastLevel", lLastLevel );
            //:vReportDef.PageState.CurrentEntityName = szHierEntityName
            SetAttributeFromString( vReportDef, "PageState", "CurrentEntityName", szHierEntityName );
            //:vReportDef.PageState.GroupSetTag = szRptGroupSetTag
            SetAttributeFromString( vReportDef, "PageState", "GroupSetTag", szRptGroupSetTag );
            //:vReportDef.PageState.GroupTag = szRptGroupTag
            SetAttributeFromString( vReportDef, "PageState", "GroupTag", szRptGroupTag );
         }


         //:END

         //://UNDEF TraceLineI( "Phantom mode ======= RC: ", nRC )

         //:IF lPageNbr > 0
         if ( lPageNbr > 0 )
         {
            //:lNextPagePrinted = lNextPagePrinted + 1
            lNextPagePrinted = lNextPagePrinted + 1;
         }

         //:END
      }

      //:END

      //:SetPhantomMode( vSubtask, 0 ) // Reset from Phantom mode to Real mode.
      SetPhantomMode( vSubtask, 0 );

      //:// Name Previous and Current views.
      //:NAME VIEW vSourcePreviousHierOI "TZ_SourcePreviousHierOI"
      SetNameForView( vSourcePreviousHierOI, "TZ_SourcePreviousHierOI", 0, zLEVEL_TASK );
      //:NAME VIEW vSourceCurrentHierOI  "TZ_SourceCurrentHierOI"
      SetNameForView( vSourceCurrentHierOI, "TZ_SourceCurrentHierOI", 0, zLEVEL_TASK );
   }


   //:END

   //:// Return with the page count when only counting pages.
   //:IF nCase = 7
   if ( nCase == 7 )
   {

      //:nRC = IsFlagSequenceSet( lPrintFlags, 8 )
      nRC = IsFlagSequenceSet( lPrintFlags, 8 );
      //:IF nRC != 0
      if ( nRC != 0 )
      {
         //:SetCursorFirstEntity( vSourceOI, szSubobjectEntityName, 0 )
         SetCursorFirstEntity( vSourceOI, szSubobjectEntityName, 0 );
      }

      //:END

      //://UNDVV     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
      //://UNDVV        TraceLineV( "(nCase=7) DropView( vSourcePreviousHierOI ): ",
      //://UNDVV                    vSourcePreviousHierOI )
      //://UNDVV     END
      //:// DropViewTrace( "Dropping view (8): ", vSourcePreviousHierOI )
      //:DropView( vSourcePreviousHierOI )
      DropView( vSourcePreviousHierOI );
      //:vSourcePreviousHierOI = 0
      vSourcePreviousHierOI = 0;

      //://UNDVV     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
      //://UNDVV        TraceLineV( "(nCase=7) DropView( vSourceCurrentHierOI ): ",
      //://UNDVV                    vSourceCurrentHierOI )
      //://UNDVV     END

      //:// DropViewTrace( "Dropping view (9): ", vSourceCurrentHierOI )
      //:DropView( vSourceCurrentHierOI )
      DropView( vSourceCurrentHierOI );
      //:vSourceCurrentHierOI = 0
      vSourceCurrentHierOI = 0;

      //:vReportDef.Report.TotalPageCnt = nPageCnt
      SetAttributeFromInteger( vReportDef, "Report", "TotalPageCnt", nPageCnt );
      //:RETURN 0
      return( 0 );
   }

   //:END

   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////
   //://
   //:// PRINT REAL PAGES
   //://
   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////

   //:// nRC is greater than zero when there are pages to print.

   //:IF vSourceHierOI != 0
   if ( vSourceHierOI != 0 )
   {
      //:bValidView = IsValidView( vSourceHierOI )
      bValidView = IsValidView( vSourceHierOI );
      //:IF bValidView = 0
      if ( bValidView == 0 )
      {
         //:bValidView = bValidView / bValidView
         bValidView = bValidView / bValidView;
      }

      //:END
   }

   //:END

   //:IF nRC > 0
   if ( nRC > 0 )
   {

      //://UNDEF  TraceLineI( "Standard mode for page: ", lPageNbr )

      //:SET CURSOR FIRST vReportDef.PageState WHERE
      //:                 vReportDef.PageState.PageNbr = lPageNbr
      RESULT = SetCursorFirstEntityByInteger( vReportDef, "PageState", "PageNbr", lPageNbr, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      {
         //:fnPageStateAdd( vReportDef, lPageNbr )
         oTZRPSRCO_fnPageStateAdd( vReportDef, lPageNbr );
      }

      //:END

      //:///////////////////////////////////////////////////////////////////////
      //://
      //:// Print the page.
      //://
      //:///////////////////////////////////////////////////////////////////////
      //:vReportDef.Report.CurrentPageNbr = lPageNbr
      SetAttributeFromInteger( vReportDef, "Report", "CurrentPageNbr", lPageNbr );
      //:nRC = PrintPage( vSubtask, vSourceHierOI, vReportDef,
      //:                 lPageNbr, lPageWidth, lPageHeight,
      //:                 szRptGroupSetTag, szRptGroupTag,
      //:                 0 )  // PRINT REAL PAGES
      nRC = oTZRPSRCO_PrintPage( vSubtask, vSourceHierOI, vReportDef, lPageNbr, lPageWidth, lPageHeight, szRptGroupSetTag, szRptGroupTag, 0 );

      //:lPrintFlags = vReportDef.Report.PrintFlags
      GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );
      //:lWork = IsFlagSequenceSet( lPrintFlags, 256 )    // HTML
      lWork = IsFlagSequenceSet( lPrintFlags, 256 );
      //:IF lWork = 0
      if ( lWork == 0 )
      {
         //:lWork = IsFlagSequenceSet( lPrintFlags, 128 ) // CSV
         lWork = IsFlagSequenceSet( lPrintFlags, 128 );
      }

      //:END

      //:IF lWork != 0
      if ( lWork != 0 )
      {
         //:lPrintFlags = SetFlagSequence( lPrintFlags,
         //:                               384,  // turn off HTML and CSV
         //:                               0 )
         lPrintFlags = SetFlagSequence( lPrintFlags, 384, 0 );
         //:vReportDef.Report.PrintFlags = lPrintFlags
         SetAttributeFromInteger( vReportDef, "Report", "PrintFlags", lPrintFlags );

         //:CloseReport( vSubtask, 3 )
         CloseReport( vSubtask, 3 );
         //:RETURN 0   // we are all done
         return( 0 );
      }

      //:END

      //:// Set up cursor information for the next page, if there is one.
      //:IF ( nRC > 0 )
      if ( nRC > 0 )
      {

         //:GetEntityKeyForHierarchicalCsr( lReturnLevel, szHierEntityName,
         //:                                lEntityKey, vSourceHierOI )
         GetEntityKeyForHierarchicalCsr( &lReturnLevel, szHierEntityName, (zPULONG) &lEntityKey, vSourceHierOI );

         //:// Retrieve initialization information from the report definition.
         //:// lEntityKey = vReportDef.Report.AbsolutePos
         //:bForcePageBreak = vReportDef.Report.ForcePageBreak
         GetIntegerFromAttribute( &bForcePageBreak, vReportDef, "Report", "ForcePageBreak" );
         //:vFooterDidNotFit = vReportDef.Report.FooterDidNotFit
         GetIntegerFromAttribute( (zLONG *) &vFooterDidNotFit, vReportDef, "Report", "FooterDidNotFit" );

         //:// IF vFooterDidNotFit != 0
         //://    bValidView = IsValidView( vFooterDidNotFit )
         //://    IF bValidView = FALSE
         //://       NAME VIEW vReportDef "DKS_PageState.FooterDidNotFit"
         //://    // bValidView = bValidView / bValidView
         //://       TraceLineV( "FooterDidNotFit View is Invalid (5): ", vFooterDidNotFit )
         //://    // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 5", TRUE )
         //://    ELSE
         //://       TraceLineV( "FooterDidNotFit View is Valid (5): ", vFooterDidNotFit )
         //://    END
         //:// END

         //:lHierRC = vReportDef.Report.HierReturnCode
         GetIntegerFromAttribute( &lHierRC, vReportDef, "Report", "HierReturnCode" );
         //:lStartLevel = vReportDef.Report.StartLevel
         GetIntegerFromAttribute( &lStartLevel, vReportDef, "Report", "StartLevel" );
         //:lLastLevel = vReportDef.Report.LastLevel
         GetIntegerFromAttribute( &lLastLevel, vReportDef, "Report", "LastLevel" );
         //:// szCurrentEntityName = vReportDef.Report.CurrentEntityName

         //://UNDEF  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDEF     TraceLineS( "FormatDocPage **END**: ",
         //://UNDEF                 "After PrintPage indicates more pages." )
         //://UNDEF     TraceLineI( "                 Page #: ", lPageNbr )
         //://UNDEF     TraceLineS( "       Hier Entity Name: ", szHierEntityName )
         //://UNDEF     TraceLineS( "       Curr Entity Name: ", szCurrentEntityName )
         //://UNDEF     TraceLineI( "            EntityKey: ", lEntityKey )
         //://UNDEF     TraceLineI( "    MultiLineTextPosEnd: ",
         //://UNDEF                 vReportDef.PageState.MultiLineTextPosEndPage )
         //://UNDEF     TraceLineI( "  ContinuedGroupFlagEnd: ",
         //://UNDEF                 vReportDef.PageState.ContinuedGroupFlagEndPage )
         //://UNDEF  // DisplayEntityInstance( vSourceHierOI, szHierEntityName )
         //://UNDEF  END

         //:// Initialize state for next page
         //:vReportDef.PageState.AbsolutePos = lEntityKey
         SetAttributeFromInteger( vReportDef, "PageState", "AbsolutePos", lEntityKey );
         //:vReportDef.PageState.ForcePageBreak = bForcePageBreak
         SetAttributeFromInteger( vReportDef, "PageState", "ForcePageBreak", bForcePageBreak );

         //:vTemp = vReportDef.PageState.FooterDidNotFit
         GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "PageState", "FooterDidNotFit" );
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:// DropViewTrace( "Dropping view (4a): ", vTemp )
            //:DropView( vTemp )
            DropView( vTemp );
            //:vTemp = 0
            vTemp = 0;
         }

         //:END

         //:IF vFooterDidNotFit != 0
         if ( vFooterDidNotFit != 0 )
         {
            //:// bValidView = IsValidView( vFooterDidNotFit )
            //:// IF bValidView = FALSE
            //:// // bValidView = bValidView / bValidView
            //://    TraceLineV( "FooterDidNotFit View is Invalid (6): ", vFooterDidNotFit )
            //:// // SysMessageBox( vSubtask, "PageState.FooterDidNotFit", "Invalid View 6", TRUE )
            //:// ELSE
            //://    TraceLineV( "FooterDidNotFit View is Valid (6): ", vFooterDidNotFit )
            //:// END

            //:// CreateViewTrace( "Creating view (4d): ", vTemp, vFooterDidNotFit, 0 )
            //:CreateViewFromViewForTask( vTemp, vFooterDidNotFit, 0 )
            CreateViewFromViewForTask( &vTemp, vFooterDidNotFit, 0 );
            //:lTempView = MakeIntegerFromView( vTemp )
            lTempView = MakeIntegerFromView( vTemp );

            //:ELSE
         }
         else
         {

            //:lTempView = 0
            lTempView = 0;
         }


         //:END

         //:vReportDef.PageState.FooterDidNotFit = lTempView
         SetAttributeFromInteger( vReportDef, "PageState", "FooterDidNotFit", lTempView );
         //:vReportDef.PageState.HierReturnCode = lHierRC
         SetAttributeFromInteger( vReportDef, "PageState", "HierReturnCode", lHierRC );
         //:vReportDef.PageState.StartLevel = lStartLevel
         SetAttributeFromInteger( vReportDef, "PageState", "StartLevel", lStartLevel );
         //:vReportDef.PageState.LastLevel = lLastLevel
         SetAttributeFromInteger( vReportDef, "PageState", "LastLevel", lLastLevel );
         //:vReportDef.PageState.CurrentEntityName = szHierEntityName
         SetAttributeFromString( vReportDef, "PageState", "CurrentEntityName", szHierEntityName );
         //:vReportDef.PageState.GroupSetTag = szRptGroupSetTag
         SetAttributeFromString( vReportDef, "PageState", "GroupSetTag", szRptGroupSetTag );
         //:vReportDef.PageState.GroupTag = szRptGroupTag
         SetAttributeFromString( vReportDef, "PageState", "GroupTag", szRptGroupTag );
      }


      //:END
   }


   //://UNDEF TraceLineI( "FormatDocPage End PageNbr: ", lPageNbr )
   //://UNDEF TraceLineI( " Standard mode ******* RC: ", nRC )

   //:END

   //:RETURN nRC
   return( nRC );
// END
}


//:LOCAL OPERATION
static zSHORT
oTZRPSRCO_fnSetCtrlPrintValues( zVIEW     vReportDef,
                                zPCHAR    szText,
                                zLONG     lPage,
                                zLONG     lPosX,
                                zLONG     lPosY )
{

   //:fnSetCtrlPrintValues( VIEW vReportDef BASED ON LOD TZRPSRCO,
   //:                   STRING ( 255 ) szText,
   //:                   INTEGER lPage,
   //:                   INTEGER lPosX,
   //:                   INTEGER lPosY )

   //:vReportDef.Control.wLastPrintText = szText
   SetAttributeFromString( vReportDef, "Control", "wLastPrintText", szText );
   //:vReportDef.Control.wLastPrintPage = lPage
   SetAttributeFromInteger( vReportDef, "Control", "wLastPrintPage", lPage );
   //:vReportDef.Control.wLastPrintPosX = lPosX
   SetAttributeFromInteger( vReportDef, "Control", "wLastPrintPosX", lPosX );
   //:vReportDef.Control.wLastPrintPosY = lPosY
   SetAttributeFromInteger( vReportDef, "Control", "wLastPrintPosY", lPosY );
   return( 0 );
// END
}


//:LOCAL OPERATION
//:fnPageStateRemove( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                   INTEGER lPageNbr )

//:   VIEW vReportDef2 BASED ON LOD TZRPSRCO
static zSHORT
oTZRPSRCO_fnPageStateRemove( zVIEW     vReportDef,
                             zLONG     lPageNbr )
{
   zVIEW     vReportDef2 = 0;
   //:VIEW vTemp
   zVIEW     vTemp = 0;
   zSHORT    RESULT;


   //:IF lPageNbr <= 0
   if ( lPageNbr <= 0 )
   {
      //:// The state for page number zero will never change.
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:// Delete all state entities for this page and beyond.
   //:FOR EACH vReportDef.PageState
   RESULT = SetCursorFirstEntity( vReportDef, "PageState", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      //:IF vReportDef.PageState.PageNbr >= lPageNbr
      if ( CompareAttributeToInteger( vReportDef, "PageState", "PageNbr", lPageNbr ) >= 0 )
      {
         //:FOR EACH vReportDef.FooterAtLevel
         RESULT = SetCursorFirstEntity( vReportDef, "FooterAtLevel", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
            //:vTemp = vReportDef.FooterAtLevel.SourceOI_Hier
            GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "FooterAtLevel", "SourceOI_Hier" );
            //://UNDVV     nRC = IsValidView( vTemp )
            //://UNDVV     IF nRC = FALSE
            //://UNDVV        TraceLineV( "(PageNbr>=lPageNbr) PageState/FooterAtLevel "
            //://UNDVV                    "invalid view: ", vTemp )
            //://UNDVV        DisplayEntityInstance( vReportDef, "PageState" )
            //://UNDVV        DisplayEntityInstance( vReportDef, "FooterAtLevel" )
            //://UNDVV     ELSE
            //:   vReportDef.FooterAtLevel.SourceOI_Hier = 0
            SetAttributeFromInteger( vReportDef, "FooterAtLevel", "SourceOI_Hier", 0 );
            //:// DropViewTrace( "Dropping view (33): ", vTemp )
            //:   DropView( vTemp )
            DropView( vTemp );
            //:   vTemp = 0
            vTemp = 0;
            RESULT = SetCursorNextEntity( vReportDef, "FooterAtLevel", "" );
         }

         //://UNDVV     END
         //:END

         //:vTemp = vReportDef.PageState.FooterDidNotFit
         GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "PageState", "FooterDidNotFit" );
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:// DropViewTrace( "Dropping view (4g): ", vTemp )
            //:DropView( vTemp )
            DropView( vTemp );
            //:vTemp = 0
            vTemp = 0;
         }

         //:END

         //:DELETE ENTITY vReportDef.PageState NONE
         RESULT = DeleteEntity( vReportDef, "PageState", zREPOS_NONE );
      }

      RESULT = SetCursorNextEntity( vReportDef, "PageState", "" );
      //:END
   }

   //:END

   //:SET CURSOR LAST vReportDef.PageState
   RESULT = SetCursorLastEntity( vReportDef, "PageState", "" );

   //:RETURN 0
   return( 0 );
// END
}


//:LOCAL OPERATION
//:PrintPage( VIEW    vSubtask,
//:           VIEW    vSourceHierOI,
//:           VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:           INTEGER lPageNbr,
//:           INTEGER lPageWidth,
//:           INTEGER lPageHeight,
//:           STRING (32) szRptGroupSetTag,
//:           STRING (32) szRptGroupTag,
//:           INTEGER lFlags )

//:   VIEW        vFooterHierOI
static zSHORT
oTZRPSRCO_PrintPage( zVIEW     vSubtask,
                     zVIEW     vSourceHierOI,
                     zVIEW     vReportDef,
                     zLONG     lPageNbr,
                     zLONG     lPageWidth,
                     zLONG     lPageHeight,
                     zPCHAR    szRptGroupSetTag,
                     zPCHAR    szRptGroupTag,
                     zLONG     lFlags )
{
   zVIEW     vFooterHierOI = 0;
   //:INTEGER     bForcePageBreak
   zLONG     bForcePageBreak = 0;
   //:VIEW        vFooterDidNotFit
   zVIEW     vFooterDidNotFit = 0;
   //:VIEW        vTempOI
   zVIEW     vTempOI = 0;
   //:VIEW        vTemp
   zVIEW     vTemp = 0;
   //:INTEGER     lCurrentPosX
   zLONG     lCurrentPosX = 0;
   //:INTEGER     lCurrentPosY
   zLONG     lCurrentPosY = 0;
   //:VIEW        vHeaderDef         BASED ON LOD TZRPSRCO
   zVIEW     vHeaderDef = 0;
   //:VIEW        vPageHeaderDef     BASED ON LOD TZRPSRCO
   zVIEW     vPageHeaderDef = 0;
   //:VIEW        vFooterDef         BASED ON LOD TZRPSRCO
   zVIEW     vFooterDef = 0;
   //:STRING (32) szCurrentEntityName
   zCHAR     szCurrentEntityName[ 33 ] = { 0 };
   //:STRING (32) szHoldEntityName
   zCHAR     szHoldEntityName[ 33 ] = { 0 };
   //:STRING (32) szSubobjectEntityName
   zCHAR     szSubobjectEntityName[ 33 ] = { 0 };
   //:STRING (32) szLastHeaderEntityNameAtLevel
   zCHAR     szLastHeaderEntityNameAtLevel[ 33 ] = { 0 };
   //:SHORT       nReturnLevel      // parameter for function
   zSHORT    nReturnLevel = 0;
   //:                              // SetCursorNextEntityHierarchical
   //:                              // lReturnLevel will be set to this value
   //:STRING (32) szRepeatHorizontalEntityName
   zCHAR     szRepeatHorizontalEntityName[ 33 ] = { 0 };
   //:INTEGER     lTotalPages
   zLONG     lTotalPages = 0;
   //:INTEGER     lEntityKey
   zLONG     lEntityKey = 0;
   //:INTEGER     lPosYHold
   zLONG     lPosYHold = 0;
   //:INTEGER     lRepeatHorizontalHold
   zLONG     lRepeatHorizontalHold = 0;
   //:INTEGER     lRepeatHorizontal
   zLONG     lRepeatHorizontal = 0;
   //:INTEGER     lRepeatHorizontalTemp
   zLONG     lRepeatHorizontalTemp = 0;
   //:INTEGER     lRepeatHorizontalFlags
   zLONG     lRepeatHorizontalFlags = 0;
   //:INTEGER     lRepeatHorizontalColumn
   zLONG     lRepeatHorizontalColumn = 0;
   //:INTEGER     lRepeatHorizontalIndent
   zLONG     lRepeatHorizontalIndent = 0;
   //:INTEGER     lRepeatHorizontalMargin
   zLONG     lRepeatHorizontalMargin = 0;
   //:INTEGER     lRepeatHorizontalExtent
   zLONG     lRepeatHorizontalExtent = 0;
   //:INTEGER     lRepeatHorizontalNewspaper
   zLONG     lRepeatHorizontalNewspaper = 0;
   //:INTEGER     lViewPortBottomPrev
   zLONG     lViewPortBottomPrev = 0;
   //:INTEGER     lViewPortTop
   zLONG     lViewPortTop = 0;
   //:INTEGER     lViewPortBottom
   zLONG     lViewPortBottom = 0;
   //:INTEGER     lViewPortLeft
   zLONG     lViewPortLeft = 0;
   //:INTEGER     lViewPortRight
   zLONG     lViewPortRight = 0;
   //:INTEGER     lReturnLevel
   zLONG     lReturnLevel = 0;
   //:INTEGER     lAbsolutePosition
   zLONG     lAbsolutePosition = 0;
   //:INTEGER     lHierRC
   zLONG     lHierRC = 0;
   //:INTEGER     lLastLevel
   zLONG     lLastLevel = 0;
   //:INTEGER     lTempLevel
   zLONG     lTempLevel = 0;
   //:INTEGER     lIncrPosX
   zLONG     lIncrPosX = 0;
   //:INTEGER     lNewPosX
   zLONG     lNewPosX = 0;
   //:INTEGER     lNewPosY
   zLONG     lNewPosY = 0;
   //:INTEGER     lBottomOfPage
   zLONG     lBottomOfPage = 0;
   //:INTEGER     lFooterPosY
   zLONG     lFooterPosY = 0;
   //:INTEGER     lRemainingSpaceOnPageX
   zLONG     lRemainingSpaceOnPageX = 0;
   //:INTEGER     lRemainingSpaceOnPageY
   zLONG     lRemainingSpaceOnPageY = 0;
   //:INTEGER     lOrigRemainingSpaceOnPageX
   zLONG     lOrigRemainingSpaceOnPageX = 0;
   //:INTEGER     lOrigRemainingSpaceOnPageY
   zLONG     lOrigRemainingSpaceOnPageY = 0;
   //:INTEGER     lGroupTotal
   zLONG     lGroupTotal = 0;
   //:INTEGER     lFooterHeight
   zLONG     lFooterHeight = 0;
   //:INTEGER     lContinuedGroupFlag
   zLONG     lContinuedGroupFlag = 0;
   //:INTEGER     lTempView
   zLONG     lTempView = 0;
   //:INTEGER     lTemp
   zLONG     lTemp = 0;
   //:INTEGER     lDksLevel
   zLONG     lDksLevel = 0;
   //:INTEGER     lDksKey
   zLONG     lDksKey = 0;
   //:INTEGER     lResetPageNbr
   zLONG     lResetPageNbr = 0;
   //:INTEGER     lResetTotalPageCnt
   zLONG     lResetTotalPageCnt = 0;
   //:STRING (32) szPageNumber
   zCHAR     szPageNumber[ 33 ] = { 0 };
   //:INTEGER     lStartLevel
   zLONG     lStartLevel = 0;
   //:INTEGER     lPrintFlags
   zLONG     lPrintFlags = 0;
   //:INTEGER     lPrintHTML
   zLONG     lPrintHTML = 0;
   //:INTEGER     lMultiRootFlag
   zLONG     lMultiRootFlag = 0;
   //:INTEGER     lPrintFooterUsingHeaderView
   zLONG     lPrintFooterUsingHeaderView = 0;
   //:INTEGER     lFirstEntityOnReportFlag
   zLONG     lFirstEntityOnReportFlag = 0;
   //:INTEGER     bValidView
   zLONG     bValidView = 0;
   //:STRING (32) szHierEntityName
   zCHAR     szHierEntityName[ 33 ] = { 0 };
   //:INTEGER     nGroupSuppressedForPage
   zLONG     nGroupSuppressedForPage = 0;
   //:INTEGER     nRC_GP
   zLONG     nRC_GP = 0;
   //:STRING (32) szINI
   zCHAR     szINI[ 33 ] = { 0 };
   //:SHORT       nRC
   zSHORT    nRC = 0;
   zSHORT    RESULT;
   zSHORT    lTempInteger_0;
   zSHORT    lTempInteger_1;
   zLONG     lTempInteger_2;
   zLONG     lTempInteger_3;

   //://UNDEF INTEGER       lFirstTime
   //://UNDEF INTEGER       lTempValue

   //://UNDEF lFirstTime = 0
   //:NAME VIEW vReportDef "CurrentReportDef"
   SetNameForView( vReportDef, "CurrentReportDef", 0, zLEVEL_TASK );
   //:lRepeatHorizontal = 0
   lRepeatHorizontal = 0;
   //:lRepeatHorizontalIndent = 0
   lRepeatHorizontalIndent = 0;
   //:lCurrentPosX = 0
   lCurrentPosX = 0;
   //:lCurrentPosY = 0
   lCurrentPosY = 0;
   //:lStartLevel = vReportDef.Report.StartLevel
   GetIntegerFromAttribute( &lStartLevel, vReportDef, "Report", "StartLevel" );
   //:lPrintFlags = vReportDef.Report.PrintFlags
   GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );

   //:nRC = IsFlagSequenceSet( lPrintFlags, 256 )
   nRC = IsFlagSequenceSet( lPrintFlags, 256 );
   //:IF nRC != 0
   if ( nRC != 0 )
   {
      //:lPrintHTML = 1
      lPrintHTML = 1;
      //:lPageHeight = 2147483647  // BIG Number!!!
      lPageHeight = 2147483647;
      //:ELSE
   }
   else
   {
      //:lPrintHTML = 0
      lPrintHTML = 0;
   }

   //:END

   //:nRC = IsFlagSequenceSet( lPrintFlags, 2048 ) // print PageFooter using PageHeader view
   nRC = IsFlagSequenceSet( lPrintFlags, 2048 );
   //:IF nRC != 0
   if ( nRC != 0 )
   {
      //:lPrintFooterUsingHeaderView = 1
      lPrintFooterUsingHeaderView = 1;
      //:ELSE
   }
   else
   {
      //:lPrintFooterUsingHeaderView = 0
      lPrintFooterUsingHeaderView = 0;
   }

   //:END

   //:IF vSourceHierOI != 0
   if ( vSourceHierOI != 0 )
   {
      //:bValidView = IsValidView( vSourceHierOI )
      bValidView = IsValidView( vSourceHierOI );
      //:IF bValidView = 0
      if ( bValidView == 0 )
      {
         //:SysMessageBox( vSubtask, "Report Writer", "Invalid Hierarchical View", TRUE )
         SysMessageBox( vSubtask, "Report Writer", "Invalid Hierarchical View", TRUE );
         //:bValidView = bValidView / bValidView
         bValidView = bValidView / bValidView;
      }

      //:END
   }

   //:END

   //:szSubobjectEntityName = vReportDef.Report.WK_SubobjectEntityName
   GetVariableFromAttribute( szSubobjectEntityName, 0, 'S', 33, vReportDef, "Report", "WK_SubobjectEntityName", "", 0 );
   //:lHierRC = vReportDef.Report.HierReturnCode
   GetIntegerFromAttribute( &lHierRC, vReportDef, "Report", "HierReturnCode" );
   //:bForcePageBreak = vReportDef.Report.ForcePageBreak
   GetIntegerFromAttribute( &bForcePageBreak, vReportDef, "Report", "ForcePageBreak" );
   //:vFooterDidNotFit = vReportDef.Report.FooterDidNotFit
   GetIntegerFromAttribute( (zLONG *) &vFooterDidNotFit, vReportDef, "Report", "FooterDidNotFit" );

   //:// IF vFooterDidNotFit != 0
   //://    bValidView = IsValidView( vFooterDidNotFit )
   //://    IF bValidView = FALSE
   //://    // bValidView = bValidView / bValidView
   //://       TraceLineV( "FooterDidNotFit View is Invalid (7): ", vFooterDidNotFit )
   //://    // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 7", TRUE )
   //://    ELSE
   //://       TraceLineV( "FooterDidNotFit View is Valid (7): ", vFooterDidNotFit )
   //://    END
   //:// END

   //:// IF lPageNbr = 1 OR vReportDef.Report.ForcePageBreak = "Y"
   //:IF vSourceHierOI != 0
   if ( vSourceHierOI != 0 )
   {
      //:CreateViewFromViewForTask( vTemp, vSourceHierOI, 0 )
      CreateViewFromViewForTask( &vTemp, vSourceHierOI, 0 );
      //:GetEntityKeyForHierarchicalCsr( lReturnLevel, szCurrentEntityName,
      //:                                lEntityKey,  vSourceHierOI )
      GetEntityKeyForHierarchicalCsr( &lReturnLevel, szCurrentEntityName, (zPULONG) &lEntityKey, vSourceHierOI );

      //:SetEntityCursor( vTemp, szCurrentEntityName, 0,
      //:                 zQUAL_ENTITYKEY + zPOS_FIRST,
      //:                 lEntityKey, 0, 0, 0, 0, 0 )
      SetEntityCursor( vTemp, szCurrentEntityName, 0, zQUAL_ENTITYKEY + zPOS_FIRST, &lEntityKey, 0, 0, 0, 0, 0 );
      //:lEntityKey = GetEntityKey( vTemp, szSubobjectEntityName )
      lEntityKey = GetEntityKey( vTemp, szSubobjectEntityName );
      //:DropView( vTemp )
      DropView( vTemp );

      //:ELSE
   }
   else
   {
      //:lEntityKey = 0
      lEntityKey = 0;
   }

   //:END

   //:lTemp = lFlags + 1073741824  // + 0x40000000 ==> set up page numbering
   lTemp = lFlags + 1073741824;
   //:lResetTotalPageCnt = lTotalPages
   lResetTotalPageCnt = lTotalPages;
   //:lResetPageNbr = HandlePageNumbering( vSubtask, szPageNumber, lEntityKey,
   //:                                     lPageNbr, lResetTotalPageCnt, lTemp )
   lResetPageNbr = HandlePageNumbering( vSubtask, szPageNumber, lEntityKey, lPageNbr, &lResetTotalPageCnt, lTemp );

   //:lLastLevel = vReportDef.Report.LastLevel
   GetIntegerFromAttribute( &lLastLevel, vReportDef, "Report", "LastLevel" );
   //:lReturnLevel = lLastLevel
   lReturnLevel = lLastLevel;

   //:// szCurrentEntityName = ""
   //:szCurrentEntityName = vReportDef.Report.CurrentEntityName
   GetVariableFromAttribute( szCurrentEntityName, 0, 'S', 33, vReportDef, "Report", "CurrentEntityName", "", 0 );

   //:IF lPageNbr = 1
   if ( lPageNbr == 1 )
   {
      //:lFirstEntityOnReportFlag = 1
      lFirstEntityOnReportFlag = 1;
      //:ELSE
   }
   else
   {
      //:lFirstEntityOnReportFlag = 0
      lFirstEntityOnReportFlag = 0;

      //://DKS  DisplayFooterInfo( vReportDef,
      //://DKS                     "PrintPage FooterAtLevel @PageNbr: ",
      //://DKS                     lPageNbr )

      //:// We want to delete any FooterAtLevel that has not been set to
      //:// Complete = 'Y', since the entity is created as a place holder.
      //:// Once Complete has been turned to 'Y', the  FooterAtLevel is
      //:// "good", or if Complete is blank, the FooterAtLevel entity was
      //:// created at a point where it is known the footer is required
      //:// at an appropriate future condition.
      //:// CreateViewTrace( "Creating view (9): ", vFooterDef, vReportDef, 0 )
      //:CreateViewFromViewForTask( vFooterDef, vReportDef, 0 )
      CreateViewFromViewForTask( &vFooterDef, vReportDef, 0 );
      //:SET CURSOR FIRST vFooterDef.FooterAtLevel WHERE
      //:    vFooterDef.FooterAtLevel.Complete = "N"
      RESULT = SetCursorFirstEntityByString( vFooterDef, "FooterAtLevel", "Complete", "N", "" );
      //:LOOP WHILE RESULT >= zCURSOR_SET
      while ( RESULT >= zCURSOR_SET )
      {

         //:vTemp = vFooterDef.FooterAtLevel.SourceOI_Hier
         GetIntegerFromAttribute( (zLONG *) &vTemp, vFooterDef, "FooterAtLevel", "SourceOI_Hier" );
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:// DropViewTrace( "Dropping view (10a): ", vTemp )
            //:DropView( vTemp )
            DropView( vTemp );
            //:vTemp = 0
            vTemp = 0;
         }

         //:END

         //:DELETE ENTITY vFooterDef.FooterAtLevel
         RESULT = DeleteEntity( vFooterDef, "FooterAtLevel", zPOS_NEXT );
         //:SET CURSOR FIRST vFooterDef.FooterAtLevel WHERE
         //:    vFooterDef.FooterAtLevel.Complete = "N"
         RESULT = SetCursorFirstEntityByString( vFooterDef, "FooterAtLevel", "Complete", "N", "" );
      }

      //:END

      //:// Set up a Footer entity if one is defined for this entity.
      //:SET CURSOR FIRST vFooterDef.GroupSet WHERE
      //:                 vFooterDef.GroupSet.Tag = szCurrentEntityName
      RESULT = SetCursorFirstEntityByString( vFooterDef, "GroupSet", "Tag", szCurrentEntityName, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      {
         //:SET CURSOR FIRST vFooterDef.Group WHERE
         //:                 vFooterDef.Group.Type = "gf"
         RESULT = SetCursorFirstEntityByString( vFooterDef, "Group", "Type", "gf", "" );
      }

      //:END

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      {
         //:SET CURSOR FIRST vFooterDef.FooterAtLevel WHERE
         //:    vFooterDef.FooterAtLevel.EntityName = szCurrentEntityName
         RESULT = SetCursorFirstEntityByString( vFooterDef, "FooterAtLevel", "EntityName", szCurrentEntityName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         {
            //:CREATE ENTITY vFooterDef.FooterAtLevel FIRST
            RESULT = CreateEntity( vFooterDef, "FooterAtLevel", zPOS_FIRST );
            //:vFooterDef.FooterAtLevel.Level = lReturnLevel
            SetAttributeFromInteger( vFooterDef, "FooterAtLevel", "Level", lReturnLevel );
            //:vFooterDef.FooterAtLevel.EntityName = szCurrentEntityName
            SetAttributeFromString( vFooterDef, "FooterAtLevel", "EntityName", szCurrentEntityName );
            //:// CreateViewTrace( "Creating view (10): ", vFooterHierOI, vSourceHierOI, 0 )
            //:CreateViewFromViewForTask( vFooterHierOI, vSourceHierOI, 0 )
            CreateViewFromViewForTask( &vFooterHierOI, vSourceHierOI, 0 );

            //:bValidView = IsValidView( vFooterHierOI )
            bValidView = IsValidView( vFooterHierOI );
            //:IF bValidView = FALSE
            if ( bValidView == FALSE )
            {
               //:bValidView = bValidView / bValidView
               bValidView = bValidView / bValidView;
            }

            //:END

            //:SetAttributeFromView( vFooterDef, "FooterAtLevel",
            //:                      "SourceOI_Hier", vFooterHierOI )
            SetAttributeFromView( vFooterDef, "FooterAtLevel", "SourceOI_Hier", vFooterHierOI );
            //:vFooterDef.FooterAtLevel.Complete = "N"  // this used to say vReportDef... dks 2005.06.22
            SetAttributeFromString( vFooterDef, "FooterAtLevel", "Complete", "N" );
         }

         //:END
      }

      //:END
      //:// DropViewTrace( "Dropping view (10): ", vFooterDef )
      //:DropView( vFooterDef )
      DropView( vFooterDef );
      //:vFooterDef = 0
      vFooterDef = 0;
   }

   //:END

   //://UNDEF TraceLineI( "PrintPage Nbr: ", lPageNbr )
   //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
   //://UNDEF    TraceLineI( "   StartLevel: ", lStartLevel )
   //://UNDEF    TraceLineI( "   PrintFlags: ", lPrintFlags )
   //://UNDEF    TraceLineI( "   LastLevel : ", lLastLevel )
   //://UNDEF    TraceLineS( "   SubobjectEntityName: ", szSubobjectEntityName )
   //://UNDEF END

   //:lTotalPages = vReportDef.Report.TotalPageCnt
   GetIntegerFromAttribute( &lTotalPages, vReportDef, "Report", "TotalPageCnt" );
   //:lTemp = 1073741824  // 0x40000000 ==> set up page numbering
   lTemp = 1073741824;
   //:lResetTotalPageCnt = lTotalPages
   lResetTotalPageCnt = lTotalPages;
   //:lResetPageNbr = HandlePageNumbering( vSubtask, szPageNumber, 0,
   //:                                     lPageNbr, lResetTotalPageCnt, lTemp )
   lResetPageNbr = HandlePageNumbering( vSubtask, szPageNumber, 0, lPageNbr, &lResetTotalPageCnt, lTemp );

   //:// Set up the height of the page.  If there is to be a Page Footer,
   //:// subtract that size from the page height.
   //:// Also set the PosY value for the Footer, since it will not depend on
   //:// the prior Group.
   //:// CreateViewTrace( "Creating view (11): ", vFooterDef, vReportDef, 0 )
   //:CreateViewFromViewForTask( vFooterDef, vReportDef, 0 )
   CreateViewFromViewForTask( &vFooterDef, vReportDef, 0 );
   //:lBottomOfPage = lPageHeight
   lBottomOfPage = lPageHeight;
   //:SET CURSOR FIRST vFooterDef.GroupSet
   //:           WHERE vFooterDef.GroupSet.Type = "PF"
   RESULT = SetCursorFirstEntityByString( vFooterDef, "GroupSet", "Type", "PF", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   {
      //:// Check each footer Group and use it if it is not suppressed.
      //:FOR EACH vFooterDef.Group
      //:      WHERE vFooterDef.Group.Type = "ga"
      //:         OR vFooterDef.Group.Type = "pf"
      RESULT = SetCursorFirstEntity( vFooterDef, "Group", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         if ( CompareAttributeToString( vFooterDef, "Group", "Type", "ga" ) == 0 || CompareAttributeToString( vFooterDef, "Group", "Type", "pf" ) == 0 )
         {
            //:nGroupSuppressedForPage = GroupSuppressReason( vFooterDef, lResetPageNbr,
            //:                                            lResetTotalPageCnt )
            nGroupSuppressedForPage = oTZRPSRCO_GroupSuppressReason( vFooterDef, lResetPageNbr, lResetTotalPageCnt );
            //:IF nGroupSuppressedForPage <= 0  // Group with no controls reserves space
            if ( nGroupSuppressedForPage <= 0 )
            {
               //:lFooterHeight = vFooterDef.Group.SZDLG_Y
               GetIntegerFromAttribute( &lFooterHeight, vFooterDef, "Group", "SZDLG_Y" );
               //:lBottomOfPage = lPageHeight - lFooterHeight
               lBottomOfPage = lPageHeight - lFooterHeight;

               //:IF lPrintHTML = 1
               if ( lPrintHTML == 1 )
               {
                  //:lFooterPosY = lFooterHeight
                  lFooterPosY = lFooterHeight;
                  //:ELSE
               }
               else
               {
                  //:lFooterPosY = lBottomOfPage
                  lFooterPosY = lBottomOfPage;
               }

               //:END
            }

         }

         RESULT = SetCursorNextEntity( vFooterDef, "Group", "" );
         //:END
      }

      //:END
   }

   //:END

   //:lRemainingSpaceOnPageX = lPageWidth
   lRemainingSpaceOnPageX = lPageWidth;
   //:lRemainingSpaceOnPageY = lBottomOfPage
   lRemainingSpaceOnPageY = lBottomOfPage;
   //:// DropViewTrace( "Dropping view (11): ", vFooterDef )
   //:DropView( vFooterDef )
   DropView( vFooterDef );
   //:vFooterDef = 0
   vFooterDef = 0;

   //:// When starting a page, the first thing to do is process a Page Header,
   //:// if there is one.  Also set the Page Number control, if there is one.
   //:// CreateViewTrace( "Creating view (12): ", vPageHeaderDef, vReportDef, 0 )
   //:CreateViewFromViewForTask( vPageHeaderDef, vReportDef, 0 )
   CreateViewFromViewForTask( &vPageHeaderDef, vReportDef, 0 );
   //:IF lPrintFooterUsingHeaderView > 0
   if ( lPrintFooterUsingHeaderView > 0 )
   {
      //:CreateViewFromViewForTask( vTempOI, vSourceHierOI, 0 )
      CreateViewFromViewForTask( &vTempOI, vSourceHierOI, 0 );
      //:NAME VIEW vTempOI "-->HeaderViewForFooter"
      SetNameForView( vTempOI, "-->HeaderViewForFooter", 0, zLEVEL_TASK );
   }

   //:END

   //:SET CURSOR FIRST vPageHeaderDef.GroupSet WHERE
   //:                 vPageHeaderDef.GroupSet.Type = "PH"
   RESULT = SetCursorFirstEntityByString( vPageHeaderDef, "GroupSet", "Type", "PH", "" );
   //:LOOP WHILE RESULT >= zCURSOR_SET
   while ( RESULT >= zCURSOR_SET )
   {
      //:// Check if subtype specified for page header.
      //:nGroupSuppressedForPage = GroupSuppressReason( vPageHeaderDef, lResetPageNbr,
      //:                                               lResetTotalPageCnt )
      nGroupSuppressedForPage = oTZRPSRCO_GroupSuppressReason( vPageHeaderDef, lResetPageNbr, lResetTotalPageCnt );

      //:// Only print header stuff if not suppressed.
      //:IF nGroupSuppressedForPage = 0 //Note: empty Group does NOT reserve space.
      if ( nGroupSuppressedForPage == 0 )
      {
         //:SET CURSOR FIRST vPageHeaderDef.Control WHERE
         //:                 vPageHeaderDef.Control.Tag = "PageNumber"
         RESULT = SetCursorFirstEntityByString( vPageHeaderDef, "Control", "Tag", "PageNumber", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         {
            //:vPageHeaderDef.Control.Text = szPageNumber
            SetAttributeFromString( vPageHeaderDef, "Control", "Text", szPageNumber );
         }

         //:END

         //://UNDEF  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDEF     TraceLineS( "FormatGroupOnPage 1: ",
         //://UNDEF                 vPageHeaderDef.Group.Tag )
         //://UNDEF  END

         //:vTempOI = vSourceHierOI
         vTempOI = vSourceHierOI;
         //:IF lPageNbr > 1
         if ( lPageNbr > 1 )
         {
            //:vTemp = vReportDef.Report.FooterDidNotFit
            GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "Report", "FooterDidNotFit" );
            //:IF vTemp != 0
            if ( vTemp != 0 )
            {

               //:vTempOI = vTemp
               vTempOI = vTemp;
            }

            //:// bValidView = IsValidView( vTempOI )
            //:// IF bValidView = FALSE
            //:// // bValidView = bValidView / bValidView
            //://    TraceLineV( "FooterDidNotFit View is Invalid (8): ", vTempOI )
            //:// // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 8", TRUE )
            //:// ELSE
            //://    TraceLineV( "FooterDidNotFit View is Valid (8): ", vTempOI )
            //:// END
            //:END
         }

         //:END

         //:FormatGroupOnPage( vPageHeaderDef, vTempOI, vSubtask,
         //:                   lNewPosX, lNewPosY,
         //:                   lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY,
         //:                   0, // continued Group flag 0 ==> not continued
         //:                   lPageWidth, lPageHeight, lRemainingSpaceOnPageY,
         //:                   lBottomOfPage )
         oTZRPSRCO_FormatGroupOnPage( vPageHeaderDef, vTempOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, 0, lPageWidth, lPageHeight, lRemainingSpaceOnPageY, lBottomOfPage );
         //:lNewPosX = 0
         lNewPosX = 0;
         //:lCurrentPosY = lNewPosY
         lCurrentPosY = lNewPosY;
         //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
         lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;
      }

      //:END

      //:SET CURSOR NEXT vPageHeaderDef.Group
      RESULT = SetCursorNextEntity( vPageHeaderDef, "Group", "" );
   }

   //:END

   //:vTempOI = vReportDef.Report.FooterDidNotFit
   GetIntegerFromAttribute( (zLONG *) &vTempOI, vReportDef, "Report", "FooterDidNotFit" );

   //:// IF vTempOI != 0
   //://    bValidView = IsValidView( vTempOI )
   //://    IF bValidView = FALSE
   //://    // bValidView = bValidView / bValidView
   //://       TraceLineV( "FooterDidNotFit View is Invalid (9): ", vTempOI )
   //://    // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 9", TRUE )
   //://    ELSE
   //://       TraceLineV( "FooterDidNotFit View is Valid (9): ", vTempOI )
   //://    END
   //:// END

   //:lPosYHold = lCurrentPosY
   lPosYHold = lCurrentPosY;
   //:IF lPageNbr > 1 AND vTempOI != 0 AND vReportDef.FooterAtLevel EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "FooterAtLevel" );
   if ( lPageNbr > 1 && vTempOI != 0 && lTempInteger_0 == 0 )
   {

      //:// Go to reusable routine to process any necessary footers.
      //:lReturnLevel = vReportDef.Report.LastLevel
      GetIntegerFromAttribute( &lReturnLevel, vReportDef, "Report", "LastLevel" );
      //:ProcessFooters( vReportDef, vSubtask, szCurrentEntityName,
      //:                lReturnLevel, lRemainingSpaceOnPageY,
      //:                lBottomOfPage, lCurrentPosX, lCurrentPosY,
      //:                lRepeatHorizontalHold,
      //:                lPageWidth, lPageHeight, 0 )
      oTZRPSRCO_ProcessFooters( vReportDef, vSubtask, szCurrentEntityName, lReturnLevel, lRemainingSpaceOnPageY, lBottomOfPage, &lCurrentPosX, &lCurrentPosY, &lRepeatHorizontalHold, lPageWidth, lPageHeight, 0 );

      //:IF vReportDef.Report.ForcePageBreak = 0
      if ( CompareAttributeToInteger( vReportDef, "Report", "ForcePageBreak", 0 ) == 0 )
      {

         //:vTemp = vReportDef.Report.FooterDidNotFit
         GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "Report", "FooterDidNotFit" );
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:// DropViewTrace( "Dropping view (4d): ", vTemp )
            //:DropView( vTemp )
            DropView( vTemp );
            //:vTemp = 0
            vTemp = 0;
         }

         //:END

         //:vReportDef.Report.FooterDidNotFit = 0
         SetAttributeFromInteger( vReportDef, "Report", "FooterDidNotFit", 0 );

         //:ELSE
      }
      else
      {

         //:// Process a Page Footer if necessary before returning.
         //:IF lPrintHTML = 1
         if ( lPrintHTML == 1 )
         {
            //:lPageHeight = lCurrentPosY + lFooterPosY
            lPageHeight = lCurrentPosY + lFooterPosY;
            //:lFooterPosY = lCurrentPosY
            lFooterPosY = lCurrentPosY;
         }

         //:END

         //:ProcessPageFooter( vReportDef, vTempOI, vSubtask,
         //:                   lNewPosX, lNewPosY, lCurrentPosX, lFooterPosY,
         //:                   lPageWidth, lPageHeight, lFlags )
         oTZRPSRCO_ProcessPageFooter( vReportDef, vTempOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lFooterPosY, lPageWidth, lPageHeight, lFlags );

         //:vTemp = vReportDef.Report.FooterDidNotFit
         GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "Report", "FooterDidNotFit" );
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:// DropViewTrace( "Dropping view (4e): ", vTemp )
            //:DropView( vTemp )
            DropView( vTemp );
            //:vTemp = 0
            vTemp = 0;
         }

         //:END

         //:vReportDef.Report.FooterDidNotFit = 0
         SetAttributeFromInteger( vReportDef, "Report", "FooterDidNotFit", 0 );
         //:vReportDef.Report.ForcePageBreak = 0
         SetAttributeFromInteger( vReportDef, "Report", "ForcePageBreak", 0 );

         //:RETURN 1 // return indicating that there are MORE pages
         return( 1 );
      }


      //:END
      //:ELSE
   }
   else
   {
      //:lRepeatHorizontalHold = 0
      lRepeatHorizontalHold = 0;
   }

   //:END

   //:szLastHeaderEntityNameAtLevel = szCurrentEntityName
   ZeidonStringCopy( szLastHeaderEntityNameAtLevel, 1, 0, szCurrentEntityName, 1, 0, 33 );
   //:IF szCurrentEntityName != ""
   if ( ZeidonStringCompare( szCurrentEntityName, 1, 0, "", 1, 0, 33 ) != 0 )
   {
      //:ProcessPgDetailHeadrs( vPageHeaderDef, vSourceHierOI,
      //:                       szCurrentEntityName,
      //:                       vSubtask, lNewPosX, lNewPosY,
      //:                       lCurrentPosX, lCurrentPosY,
      //:                       lRemainingSpaceOnPageY, lBottomOfPage,
      //:                       lPageWidth, lPageHeight )
      oTZRPSRCO_ProcessPgDetailHeadrs( vPageHeaderDef, vSourceHierOI, szCurrentEntityName, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lCurrentPosY, &lRemainingSpaceOnPageY, lBottomOfPage, lPageWidth, lPageHeight );
      //:lNewPosX = 0
      lNewPosX = 0;

      //:// If headers were formatted, update current values and indicate that
      //:// a header was formatted.
      //:IF lCurrentPosY != lNewPosY
      if ( lCurrentPosY != lNewPosY )
      {
         //:lCurrentPosY = lNewPosY
         lCurrentPosY = lNewPosY;
         //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
         lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;
      }

      //:END
   }

   //:END

   //:szLastHeaderEntityNameAtLevel = vReportDef.Report.CurrentEntityName
   GetVariableFromAttribute( szLastHeaderEntityNameAtLevel, 0, 'S', 33, vReportDef, "Report", "CurrentEntityName", "", 0 );
   //:// DropViewTrace( "Dropping view (12): ", vPageHeaderDef )
   //:DropView( vPageHeaderDef )
   DropView( vPageHeaderDef );
   //:vPageHeaderDef = 0
   vPageHeaderDef = 0;

   //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
   //://UNDEF    TraceLineI( "FormatGroupOnPage 1 RemainingSpaceOnPage: ",
   //://UNDEF                lRemainingSpaceOnPageY )
   //://UNDEF END

   //:lOrigRemainingSpaceOnPageY = lRemainingSpaceOnPageY
   lOrigRemainingSpaceOnPageY = lRemainingSpaceOnPageY;

   //:// MULTI EDIT
   //:// Complete the formatting of a Group that was only partially
   //:// completed on the last page.  If such a Group exists, the
   //:// vReportDef.Report.ContinuedGroupFlag value will be > 0.
   //:nRC_GP = -2  // Indicates that FormatGroupOnPage not yet called
   nRC_GP = -2;
   //:IF vReportDef.PageState.ContinuedGroupFlagStartPage > 0
   if ( CompareAttributeToInteger( vReportDef, "PageState", "ContinuedGroupFlagStartPage", 0 ) > 0 )
   {

      //:// First, for a continued group, we may need to print group headers.
      //:// 01Dec01 - NO - Is done properly above after November fixes
      //:// BUT JUST IN CASE allow an INI to continue to use this code
      //:SysReadZeidonIni( -1, "[Debug]",
      //:                  "IgnoreReportFix011201(ExtraDtlHdr)", szINI )
      SysReadZeidonIni( -1, "[Debug]", "IgnoreReportFix011201(ExtraDtlHdr)", szINI, sizeof( szINI ) );
      //:IF szINI = "Y"
      if ( ZeidonStringCompare( szINI, 1, 0, "Y", 1, 0, 33 ) == 0 )
      {
         //:// szCurrentEntityName = vReportDef.Report.CurrentEntityName
         //:// CreateViewTrace( "Creating view (13): ", vHeaderDef, vReportDef, 0 )
         //:CreateViewFromViewForTask( vHeaderDef, vReportDef, 0 )
         CreateViewFromViewForTask( &vHeaderDef, vReportDef, 0 );
         //:SET CURSOR FIRST vHeaderDef.GroupSet WHERE
         //:                 vHeaderDef.GroupSet.Tag = szCurrentEntityName
         RESULT = SetCursorFirstEntityByString( vHeaderDef, "GroupSet", "Tag", szCurrentEntityName, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         {
            //:SET CURSOR FIRST vHeaderDef.Group WHERE
            //:                 vHeaderDef.Group.Type = "gh"
            RESULT = SetCursorFirstEntityByString( vHeaderDef, "Group", "Type", "gh", "" );
            //:ELSE
         }
         else
         {
            //:TraceLineS( "PrintPage unable to locate GroupSet: ",
            //:            szCurrentEntityName )
            TraceLineS( "PrintPage unable to locate GroupSet: ", szCurrentEntityName );
         }

         //:END

         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         {
            //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
            //://UNDEF        TraceLineS( "FormatGroupOnPage MULTI EDIT header 2: ",
            //://UNDEF                    vHeaderDef.Group.Tag )
            //://UNDEF     END

            //:FormatGroupOnPage( vHeaderDef, vSourceHierOI, vSubtask,
            //:                   lNewPosX, lNewPosY,
            //:                   lCurrentPosX, lCurrentPosY,
            //:                   lRemainingSpaceOnPageY,
            //:                   0, // continued Group flag 0 ==> not continued
            //:                   lPageWidth, lPageHeight,
            //:                   lOrigRemainingSpaceOnPageY,
            //:                   lBottomOfPage )
            oTZRPSRCO_FormatGroupOnPage( vHeaderDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, 0, lPageWidth, lPageHeight, lOrigRemainingSpaceOnPageY, lBottomOfPage );
            //:lNewPosX = 0
            lNewPosX = 0;
            //:lCurrentPosY = lNewPosY
            lCurrentPosY = lNewPosY;
            //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
            lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;
         }

         //:END

         //:// DropViewTrace( "Dropping view (13): ", vHeaderDef )
         //:DropView( vHeaderDef )
         DropView( vHeaderDef );
         //:vHeaderDef = 0
         vHeaderDef = 0;
      }

      //:END

      //:// ==============================================
      //:// At this point we can print the continued group

      //:szRptGroupSetTag = vReportDef.GroupSet.Tag
      GetVariableFromAttribute( szRptGroupSetTag, 0, 'S', 33, vReportDef, "GroupSet", "Tag", "", 0 );
      //:szRptGroupTag = vReportDef.Group.Tag
      GetVariableFromAttribute( szRptGroupTag, 0, 'S', 33, vReportDef, "Group", "Tag", "", 0 );
      //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
      //://UNDEF    TraceLineS( "FormatGroupOnPage MULTI EDIT 3: ",
      //://UNDEF                vReportDef.Group.Tag )
      //://UNDEF END
      //:nRC_GP = FormatGroupOnPage( vReportDef, vSourceHierOI, vSubtask,
      //:                            lNewPosX, lNewPosY,
      //:                            lCurrentPosX, lCurrentPosY,
      //:                            lRemainingSpaceOnPageY,
      //:                            1, // continued Group flag 1 ==> continued
      //:                            lPageWidth, lPageHeight,
      //:                            lOrigRemainingSpaceOnPageY,
      //:                            lBottomOfPage )
      nRC_GP = oTZRPSRCO_FormatGroupOnPage( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, 1, lPageWidth, lPageHeight, lOrigRemainingSpaceOnPageY, lBottomOfPage );
      //:lNewPosX = 0
      lNewPosX = 0;
      //:ELSE
   }
   else
   {
      //://====================
      //:// Group was not continued to this page
      //://====================
      //:// When we start a page, we assume that the Entity in vSourceHierOI is
      //:// to be formatted on the Page and that vReportDef is positioned on the
      //:// correct Group.  This was determined by the FormatSubobjectOnDoc
      //:// routine at the beginning or by the FormatDocPage routine for the
      //:// last page.  A FooterAtLevel entity exists only if a Footer could
      //:// not fit on the last page.
      //:IF vReportDef.FooterAtLevel EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vReportDef, "FooterAtLevel" );
      if ( lTempInteger_1 == 0 )
      {

         //:// Go to reusable routine to process any necessary footers.
         //:lReturnLevel = vReportDef.Report.LastLevel
         GetIntegerFromAttribute( &lReturnLevel, vReportDef, "Report", "LastLevel" );
         //:ProcessFooters( vReportDef, vSubtask, szCurrentEntityName,
         //:                lReturnLevel, lRemainingSpaceOnPageY,
         //:                lBottomOfPage, lCurrentPosX, lCurrentPosY,
         //:                lTemp,
         //:                lPageWidth, lPageHeight, 0 )
         oTZRPSRCO_ProcessFooters( vReportDef, vSubtask, szCurrentEntityName, lReturnLevel, lRemainingSpaceOnPageY, lBottomOfPage, &lCurrentPosX, &lCurrentPosY, &lTemp, lPageWidth, lPageHeight, 0 );
      }


      //:END

      //:// If we are not on page one, format the last Group from the prior
      //:// page, if the return code was ok.
      //:// Now format the last Group, if return code was ok, including a Header.
      //:IF lHierRC >= zCURSOR_SET AND lPageNbr != 1
      if ( lHierRC >= zCURSOR_SET && lPageNbr != 1 )
      {
         //:// Only process the entity if there is a GroupSet by the same name
         //:// as the Entity.
         //:SET CURSOR FIRST vReportDef.Group WHERE
         //:                 vReportDef.Group.Type = "ga"
         RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "ga", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         {

            //:szRptGroupSetTag = vReportDef.GroupSet.Tag
            GetVariableFromAttribute( szRptGroupSetTag, 0, 'S', 33, vReportDef, "GroupSet", "Tag", "", 0 );
            //:szRptGroupTag = vReportDef.Group.Tag
            GetVariableFromAttribute( szRptGroupTag, 0, 'S', 33, vReportDef, "Group", "Tag", "", 0 );

            //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
            //://UNDEF        TraceLineS( "Format the last GroupSet: ", szRptGroupSetTag )
            //://UNDEF        TraceLineS( "                   Group: ", szRptGroupTag )
            //://UNDEF        IF vReportDef.PageState.ContinuedGroupFlagStartPage > 0
            //://UNDEF           TraceLineI( "     Continued: ",
            //://UNDEF                       vReportDef.PageState.ContinuedGroupFlagStartPage )
            //://UNDEF        ELSE
            //://UNDEF           TraceLineI( " Not Continued: ",
            //://UNDEF                       vReportDef.PageState.ContinuedGroupFlagStartPage )
            //://UNDEF        END
            //://UNDEF
            //://UNDEF        TraceLineV( " vSourceHierOI: ", vSourceHierOI )
            //://UNDEF     END

            //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
            //://UNDEF        TraceLineS( "FormatGroupOnPage 5: ",
            //://UNDEF                    vReportDef.Group.Tag )
            //://UNDEF     END

            //:vTemp = vReportDef.Report.FooterDidNotFit
            GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "Report", "FooterDidNotFit" );
            //:IF vTemp = 0
            if ( vTemp == 0 )
            {
               //:vTempOI = vSourceHierOI
               vTempOI = vSourceHierOI;
               //:lRepeatHorizontal = vReportDef.GroupSet.RepeatHorizontal
               GetIntegerFromAttribute( &lRepeatHorizontal, vReportDef, "GroupSet", "RepeatHorizontal" );
               //:ELSE
            }
            else
            {
               //:vTempOI = vTemp
               vTempOI = vTemp;

               //:// IF vTempOI != 0
               //://    bValidView = IsValidView( vTempOI )
               //://    IF bValidView = FALSE
               //://    // bValidView = bValidView / bValidView
               //://       TraceLineV( "FooterDidNotFit View is Invalid (10): ", vTempOI )
               //://    // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 10", TRUE )
               //://    ELSE
               //://       TraceLineV( "FooterDidNotFit View is Valid (10): ", vTempOI )
               //://    END
               //:// END

               //:lRepeatHorizontal = 0
               lRepeatHorizontal = 0;
            }

            //:END

            //:IF lRepeatHorizontal = 0
            if ( lRepeatHorizontal == 0 )
            {
               //:nRC_GP = FormatGroupOnPage( vReportDef, vTempOI, vSubtask,
               //:                            lNewPosX, lNewPosY,
               //:                            lCurrentPosX, lCurrentPosY,
               //:                            lRemainingSpaceOnPageY,
               //:                            0, // continued Group flag 0 ==> not continued
               //:                            lPageWidth, lPageHeight,
               //:                            lOrigRemainingSpaceOnPageY,
               //:                            lBottomOfPage )
               nRC_GP = oTZRPSRCO_FormatGroupOnPage( vReportDef, vTempOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, 0, lPageWidth, lPageHeight, lOrigRemainingSpaceOnPageY, lBottomOfPage );
               //:lNewPosX = 0
               lNewPosX = 0;
            }

            //:END
         }

         //:END
      }

      //:END
   }

   //:END

   //:// Check the return code from the FormatGroupOnPage for the Detail
   //:// Group to see if it was split to a new page or we need to get the
   //:// next data entity.
   //:IF nRC_GP = -2
   if ( nRC_GP == -2 )
   {
      //:// FormatGroupOnPage was not called above in the start of page
      //:// processing (usually Page 1).  So do nothing.
      //:ELSE
   }
   else
   {
      //:IF nRC_GP = -1
      if ( nRC_GP == -1 )
      {
         //://*****  MULTI EDIT
         //:// If the return code from FormatGroupOnPage is -1, then the Group
         //:// could not fit on the page due to a multi-line text field. For
         //:// this case process any page footers and return. FormatGroupOnPage
         //:// has already set up things for continuing on the next page.

         //:// Process a Page Footer if necessary before returning.
         //:IF lPrintHTML = 1
         if ( lPrintHTML == 1 )
         {
            //:lPageHeight = lCurrentPosY + lFooterPosY
            lPageHeight = lCurrentPosY + lFooterPosY;
            //:lFooterPosY = lCurrentPosY
            lFooterPosY = lCurrentPosY;
         }

         //:END

         //:ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask,
         //:                lNewPosX, lNewPosY, lCurrentPosX, lFooterPosY,
         //:                lPageWidth, lPageHeight, lFlags )
         oTZRPSRCO_ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lFooterPosY, lPageWidth, lPageHeight, lFlags );

         //:// Return with an indication that there are more Pages and save
         //:// the CurrentEntityName, last HierReturnCode and ReturnLevel.
         //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
         SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
         //:vReportDef.Report.HierReturnCode = lHierRC
         SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lHierRC );
         //:vReportDef.Report.LastLevel = lReturnLevel
         SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );

         //:RETURN 1 // return indicating that there are MORE pages
         return( 1 );

         //:ELSE
      }
      else
      {
         //://UNDEF  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDEF     TraceLineI( "Setting ReturnLevel: ", lLastLevel )
         //://UNDEF     TraceLineS( "   szLastHeaderEntityNameAtLevel? ",
         //://UNDEF                 szLastHeaderEntityNameAtLevel )
         //://UNDEF     TraceLineS( "szCurrentEntityName? ", szCurrentEntityName )
         //://UNDEF  END

         //:// szLastHeaderEntityNameAtLevel = szCurrentEntityName
         //:lCurrentPosY = lNewPosY
         lCurrentPosY = lNewPosY;
         //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
         lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;

         //:// IF vSourceHierOI != 0
         //://    lHierRC = SetCursorNextEntityHierarchical( nReturnLevel,
         //://                                               szCurrentEntityName,
         //://                                               vSourceHierOI )
         //://    lReturnLevel = nReturnLevel

         //:IF vSourceHierOI != 0
         if ( vSourceHierOI != 0 )
         {
            //:// We want to save the level of the last entity on which we have
            //:// position anytime we read a new entity.  In the past, we saved
            //:// the level (above) only when we processed a detail Group.
            //:/*dks*/  lLastLevel = lReturnLevel  // save Level of the last entity
            lLastLevel = lReturnLevel;
            //:                        // on which we had position.

            //://>      IF lHierRC >= zCURSOR_SET

            //:// dks/pas 2005.11.17 - start
            //:// This code is added to cause a detail footer to be processed
            //:// when necessary.  If there are sub-entities to the entity
            //:// being processed, but none of them are "report entities", ignore
            //:// them and get to the next "reportable" entity.
            //:szHoldEntityName = szCurrentEntityName
            ZeidonStringCopy( szHoldEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 );
            //:lHierRC = zCURSOR_SET
            lHierRC = zCURSOR_SET;
            //:lTempLevel = lLastLevel + 1
            lTempLevel = lLastLevel + 1;
            //:LOOP WHILE lHierRC >= zCURSOR_SET AND lTempLevel > lLastLevel
            while ( lHierRC >= zCURSOR_SET && lTempLevel > lLastLevel )
            {

               //:lHierRC = SetCursorNextEntityHierarchical( nReturnLevel,
               //:                                        szCurrentEntityName,
               //:                                        vSourceHierOI )
               lHierRC = SetCursorNextEntityHierarchical( (zPUSHORT) &nReturnLevel, szCurrentEntityName, vSourceHierOI );
               //:IF lHierRC >= zCURSOR_SET
               if ( lHierRC >= zCURSOR_SET )
               {

                  //:// For recursive subentity, step down a level.
                  //:IF lHierRC = zCURSOR_SET_RECURSIVECHILD
                  if ( lHierRC == zCURSOR_SET_RECURSIVECHILD )
                  {
                     //:SetViewToSubobject( vSourceHierOI, szCurrentEntityName )
                     SetViewToSubobject( vSourceHierOI, szCurrentEntityName );
                     //:GetEntityNameForHierarchicalCsr( nReturnLevel, szCurrentEntityName, lAbsolutePosition, vSourceHierOI )
                     GetEntityNameForHierarchicalCsr( (zPUSHORT) &nReturnLevel, szCurrentEntityName, (zPULONG) &lAbsolutePosition, vSourceHierOI );
                  }

                  //:END

                  //:GetEntityKeyForHierarchicalCsr( lDksLevel, szCurrentEntityName,
                  //:                             lDksKey,  vSourceHierOI )
                  GetEntityKeyForHierarchicalCsr( &lDksLevel, szCurrentEntityName, (zPULONG) &lDksKey, vSourceHierOI );
                  //://TraceLine( "Processing Entity: %s   Key: %d", szCurrentEntityName, lDksKey )

                  //:lTempLevel = nReturnLevel
                  lTempLevel = nReturnLevel;
                  //:IF lTempLevel > lLastLevel
                  if ( lTempLevel > lLastLevel )
                  {

                     //:// Only process the entity if there is a GroupSet whose Tag matches
                     //:// the name of the Entity.
                     //:SET CURSOR FIRST vReportDef.GroupSet WHERE
                     //:              vReportDef.GroupSet.Tag = szCurrentEntityName
                     RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szCurrentEntityName, "" );
                     //:IF RESULT >= zCURSOR_SET
                     if ( RESULT >= zCURSOR_SET )
                     {

                        //:lReturnLevel = nReturnLevel
                        lReturnLevel = nReturnLevel;
                        //:lTempLevel = lLastLevel
                        lTempLevel = lLastLevel;
                        //:SET CURSOR FIRST vReportDef.GroupSet WHERE
                        //:              vReportDef.GroupSet.Tag = szHoldEntityName
                        RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szHoldEntityName, "" );
                     }

                     //:END
                  }

                  //:END
               }

               //:END
            }

            //:END
            //:// dks/pas 2005.11.17 - end

            //:// dks/dgc 2007.2.28 - start
            //:// Process a Group Footer if necessary before continuing.
            //:IF szHoldEntityName != szCurrentEntityName AND lTempLevel < lLastLevel
            if ( ZeidonStringCompare( szHoldEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 ) != 0 && lTempLevel < lLastLevel )
            {

               //:// Note that we are using lTempLevel in the call below instead of the normal lReturnLevel as we're
               //:// concerned that we not step on lReturnLevel, which seems to be used quite a bit at the beginning of a page.
               //:ProcessFooters( vReportDef, vSubtask, szHoldEntityName,
               //:             lTempLevel, lRemainingSpaceOnPageY,
               //:             lBottomOfPage, lCurrentPosX, lCurrentPosY,
               //:             lRepeatHorizontalHold, lPageWidth, lPageHeight, 0 )
               oTZRPSRCO_ProcessFooters( vReportDef, vSubtask, szHoldEntityName, lTempLevel, lRemainingSpaceOnPageY, lBottomOfPage, &lCurrentPosX, &lCurrentPosY, &lRepeatHorizontalHold, lPageWidth, lPageHeight, 0 );
            }

            //:END
            //:// dks/dgc 2007.2.28 - end

            //://>      END

            //://UNDEF  IF lHierRC >= zCURSOR_SET
            //://UNDEF     TraceLineS( "DisplayEI SetCursorNextEntityHierarchical 3", "" )
            //://UNDEF     DisplayEntityInstance( vSourceHierOI, szCurrentEntityName )
            //://UNDEF  END
            //:ELSE
         }
         else
         {
            //:lHierRC = zCURSOR_NULL
            lHierRC = zCURSOR_NULL;
         }

         //:END
      }


      //://    TraceLineI( "PrintPage b============== PageNbr: ", lPageNbr )
      //://    TraceLineS( "   Processing hierarchical entity: ",
      //://                szCurrentEntityName )
      //:// // TraceLineI( "          Hierarchical ReturnCode: ",
      //:// //             lHierRC )
      //://    GetEntityKeyForHierarchicalCsr( lReturnLevel, szHierEntityName,
      //://                                    lEntityKey,  vSourceHierOI )
      //://    TraceLineS( "              AbsHier  EntityName: ", szHierEntityName )
      //:// // TraceLineI( "              AbsHier  Position: ", lEntityKey )

      //://UNDEF  TraceLineS( "Processing hierarchical entity: ",
      //://UNDEF              szCurrentEntityName )

      //:END END
   }


   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////
   //://
   //:// Now continue to loop hierarchically through the entities in the
   //:// subobject and process headers, main Groups and footers for each
   //:// entity, as defined in the Report Def.
   //:// 1. Process an entity only if there is a GroupSet whose Tag matches the
   //://    name of the Entity.
   //:// 2. A main Group is processed if there is a Group of subtype "ga" in
   //://    the ReportDef.
   //:// 3. A header Group is processed for the first instance of an entity
   //://    under its parent if there is a Group of Type "gh".  This is
   //://    determined by the changing of the CurrentEntityName at a particular
   //://    level or by going down a level.
   //:// 4. A footer Group is processed for the last instance of an entity under
   //://    its parent if there is a Group of Type "gf".  A problem here is that
   //://    multiple Footers might need to be processed when we position on a
   //://    new entity.  To handle this, an instance of FooterAtLevel was
   //://    created on the first instance of each entity that would later
   //://    require a Footer.  They were created in reverse order by Level.
   //://    Thus our logic for processing a Footer is to see if an instance of
   //://    FooterAtLevel exists for a Level higher than the level of the
   //://    current entity.
   //://
   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////
   //://////////////////////////////////////////////////////////////////////////

   //:// TraceLineX( "PrintPage lPrintFlags: ", lPrintFlags )
   //:lMultiRootFlag = SetFlagSequence( lPrintFlags,
   //:                                  1048576, // 0x00100000  check this bit
   //:                                  0 )      // Nonzero ==> multiple
   lMultiRootFlag = SetFlagSequence( lPrintFlags, 1048576, 0 );
   //:                                           // roots were requested.

   //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
   //://UNDEF    TraceLineS( "LOOP WHILE GroupSet Tag: ", szRptGroupSetTag )
   //://UNDEF    TraceLineS( "              Group Tag: ", szRptGroupTag )
   //://UNDEF    TraceLineI( "         HierReturnCode: ", lHierRC )
   //://UNDEF    TraceLineI( "            ReturnLevel: ", lReturnLevel )
   //://UNDEF    TraceLineI( "             StartLevel: ", lStartLevel )
   //://UNDEF    TraceLineI( "              TestFlags: ", lMultiRootFlag )
   //://UNDEF    TraceLineS( "    szCurrentEntityName: ", szCurrentEntityName )
   //://UNDEF    TraceLineS( "  szSubobjectEntityName: ", szSubobjectEntityName )
   //://UNDEF END

   //:// For any group that is repeating horizontally, we assume all setup
   //:// of the "view port" is done for each page.
   //:// Clear the "view port" stack.
   //:nRC = 1
   nRC = 1;
   //:LOOP WHILE nRC > 0
   while ( nRC > 0 )
   {
      //:nRC = PopReportViewPort( vSubtask, szRepeatHorizontalEntityName,
      //:                         lRepeatHorizontal, lRepeatHorizontalColumn,
      //:                         lRepeatHorizontalIndent, lRepeatHorizontalMargin,
      //:                         lRepeatHorizontalExtent, lRepeatHorizontalFlags,
      //:                         lViewPortBottomPrev, lViewPortTop, lViewPortLeft,
      //:                         lViewPortBottom, lViewPortRight )
      nRC = (zSHORT) PopReportViewPort( vSubtask, szRepeatHorizontalEntityName, &lRepeatHorizontal, &lRepeatHorizontalColumn, &lRepeatHorizontalIndent, &lRepeatHorizontalMargin, &lRepeatHorizontalExtent, &lRepeatHorizontalFlags, &lViewPortBottomPrev,
              &lViewPortTop, &lViewPortLeft, &lViewPortBottom, &lViewPortRight );
   }

   //:END

   //:// Now (continue to) loop hierarchically through the entities.
   //:LOOP WHILE lHierRC >= zCURSOR_SET AND
   //:           lReturnLevel >= lStartLevel AND
   //:           (lMultiRootFlag != 0 OR
   //:            lFirstEntityOnReportFlag = 1 OR
   //:            szCurrentEntityName != szSubobjectEntityName)
   while ( lHierRC >= zCURSOR_SET && lReturnLevel >= lStartLevel && ( lMultiRootFlag != 0 || lFirstEntityOnReportFlag == 1 || ZeidonStringCompare( szCurrentEntityName, 1, 0, szSubobjectEntityName, 1, 0, 33 ) != 0 ) )
   {

      //:lFirstEntityOnReportFlag = 0
      lFirstEntityOnReportFlag = 0;

      //:// Only process the entity if there is a GroupSet whose Tag matches
      //:// the name of the Entity.
      //:SET CURSOR FIRST vReportDef.GroupSet WHERE
      //:                 vReportDef.GroupSet.Tag = szCurrentEntityName
      RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szCurrentEntityName, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      {

         //:// Added 2006.10.30 by DonC, because we were not always positioned on the detail group.
         //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "ga"
         RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "ga", "" );

         //:// Added 2005.04.03 by DKS
         //:// The following code was added because Group Headers were not being
         //:// generated for an entity that was printed on a previous page, even
         //:// when a "higher level" entity was printed.
         //:// Null any LastHeaderEntityAtLevel entries for entities that exist
         //:// at a lower level than the current entity. This will cause the
         //:// trigger of headers for those entities.
         //:IF lReturnLevel < lLastLevel
         if ( lReturnLevel < lLastLevel )
         {

            //:// CreateViewTrace( "Creating view (14): ", vHeaderDef, vReportDef, 0 )
            //:CreateViewFromViewForTask( vHeaderDef, vReportDef, 0 )
            CreateViewFromViewForTask( &vHeaderDef, vReportDef, 0 );
            //:FOR EACH vHeaderDef.LastHeaderEntityAtLevel
            RESULT = SetCursorFirstEntity( vHeaderDef, "LastHeaderEntityAtLevel", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
               //:IF vHeaderDef.LastHeaderEntityAtLevel.Level > lReturnLevel
               if ( CompareAttributeToInteger( vHeaderDef, "LastHeaderEntityAtLevel", "Level", lReturnLevel ) > 0 )
               {
                  //:vHeaderDef.LastHeaderEntityAtLevel.EntityName = ""
                  SetAttributeFromString( vHeaderDef, "LastHeaderEntityAtLevel", "EntityName", "" );
               }

               RESULT = SetCursorNextEntity( vHeaderDef, "LastHeaderEntityAtLevel", "" );
               //:END
            }

            //:END

            //:// DropViewTrace( "Dropping view (14): ", vHeaderDef )
            //:DropView( vHeaderDef )
            DropView( vHeaderDef );
            //:vHeaderDef = 0
            vHeaderDef = 0;
         }

         //:END

         //:// If the RepeatHorizontal value is not the same as the previous
         //:// value, then we are entering a new GroupSet and need to
         //:// reinitialize the "repeat" settings.
         //:PopReportViewPort( vSubtask, szRepeatHorizontalEntityName,
         //:                   lRepeatHorizontal, lRepeatHorizontalColumn,
         //:                   lRepeatHorizontalIndent, lRepeatHorizontalMargin,
         //:                   lRepeatHorizontalExtent, lRepeatHorizontalFlags,
         //:                   lViewPortBottomPrev, lViewPortTop, lViewPortLeft,
         //:                   lViewPortBottom, lViewPortRight )
         PopReportViewPort( vSubtask, szRepeatHorizontalEntityName, &lRepeatHorizontal, &lRepeatHorizontalColumn, &lRepeatHorizontalIndent, &lRepeatHorizontalMargin, &lRepeatHorizontalExtent, &lRepeatHorizontalFlags, &lViewPortBottomPrev,
                        &lViewPortTop, &lViewPortLeft, &lViewPortBottom, &lViewPortRight );

         //:lRepeatHorizontalTemp = vReportDef.GroupSet.RepeatHorizontal
         GetIntegerFromAttribute( &lRepeatHorizontalTemp, vReportDef, "GroupSet", "RepeatHorizontal" );
         //:IF lRepeatHorizontalTemp != lRepeatHorizontal
         if ( lRepeatHorizontalTemp != lRepeatHorizontal )
         {

            //:// Clear the "view port" stack.
            //:nRC = 1
            nRC = 1;
            //:LOOP WHILE nRC > 0
            while ( nRC > 0 )
            {
               //:nRC = PopReportViewPort( vSubtask, szRepeatHorizontalEntityName,
               //:                         lRepeatHorizontal, lRepeatHorizontalColumn,
               //:                         lRepeatHorizontalIndent, lRepeatHorizontalMargin,
               //:                         lRepeatHorizontalExtent, lRepeatHorizontalFlags,
               //:                         lViewPortBottomPrev, lViewPortTop, lViewPortLeft,
               //:                         lViewPortBottom, lViewPortRight )
               nRC = (zSHORT) PopReportViewPort( vSubtask, szRepeatHorizontalEntityName, &lRepeatHorizontal, &lRepeatHorizontalColumn, &lRepeatHorizontalIndent, &lRepeatHorizontalMargin, &lRepeatHorizontalExtent, &lRepeatHorizontalFlags,
                       &lViewPortBottomPrev, &lViewPortTop, &lViewPortLeft, &lViewPortBottom, &lViewPortRight );
            }

            //:END

            //:lRepeatHorizontal = lRepeatHorizontalTemp
            lRepeatHorizontal = lRepeatHorizontalTemp;
            //:lRepeatHorizontalIndent = vReportDef.GroupSet.RepeatIndent
            GetIntegerFromAttribute( &lRepeatHorizontalIndent, vReportDef, "GroupSet", "RepeatIndent" );
            //:lCurrentPosX = lRepeatHorizontalIndent
            lCurrentPosX = lRepeatHorizontalIndent;
            //:lViewPortLeft = 0
            lViewPortLeft = 0;
            //:lViewPortBottomPrev = lCurrentPosY
            lViewPortBottomPrev = lCurrentPosY;
            //:lViewPortTop = lCurrentPosY
            lViewPortTop = lCurrentPosY;
            //:lViewPortRight = lPageWidth
            lViewPortRight = lPageWidth;
            //:lViewPortBottom = lBottomOfPage
            lViewPortBottom = lBottomOfPage;

            //:IF lRepeatHorizontalHold != 0 AND lRepeatHorizontalHold = lRepeatHorizontalTemp
            if ( lRepeatHorizontalHold != 0 && lRepeatHorizontalHold == lRepeatHorizontalTemp )
            {
               //:lViewPortTop = lPosYHold
               lViewPortTop = lPosYHold;
               //:lRepeatHorizontalHold = 0
               lRepeatHorizontalHold = 0;
            }

            //:END

            //:IF lRepeatHorizontal = 0
            if ( lRepeatHorizontal == 0 )
            {

               //:lRepeatHorizontalFlags = 0
               lRepeatHorizontalFlags = 0;
               //:lRepeatHorizontalColumn = 0
               lRepeatHorizontalColumn = 0;
               //:lRepeatHorizontalIndent = 0
               lRepeatHorizontalIndent = 0;
               //:lRepeatHorizontalMargin = 0
               lRepeatHorizontalMargin = 0;
               //:lRepeatHorizontalExtent = 0
               lRepeatHorizontalExtent = 0;
               //:szRepeatHorizontalEntityName = ""
               ZeidonStringCopy( szRepeatHorizontalEntityName, 1, 0, "", 1, 0, 33 );

               //:ELSE
            }
            else
            {

               //:// SysReadZeidonIni( -1, "[Workstation]",
               //://                   "RepeatHorizontalIndent", szINI )
               //:// lRepeatHorizontalIndent = zatol( szINI )
               //:lRepeatHorizontalMargin = vReportDef.GroupSet.RepeatMargin
               GetIntegerFromAttribute( &lRepeatHorizontalMargin, vReportDef, "GroupSet", "RepeatMargin" );
               //:lRepeatHorizontalFlags = vReportDef.GroupSet.Flags
               GetIntegerFromAttribute( &lRepeatHorizontalFlags, vReportDef, "GroupSet", "Flags" );
               //:lRepeatHorizontalExtent = 0
               lRepeatHorizontalExtent = 0;
               //:lRepeatHorizontalColumn = 1
               lRepeatHorizontalColumn = 1;
               //:lRepeatHorizontalNewspaper =
               //:               IsFlagSequenceSet( lRepeatHorizontalFlags, 1 )
               lRepeatHorizontalNewspaper = IsFlagSequenceSet( lRepeatHorizontalFlags, 1 );
               //:IF lRepeatHorizontalNewspaper != 0
               if ( lRepeatHorizontalNewspaper != 0 )
               {
                  //:IF szRepeatHorizontalEntityName != szCurrentEntityName
                  if ( ZeidonStringCompare( szRepeatHorizontalEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 ) != 0 )
                  {
                  }

                  //:END
               }

               //:END

               //:lNewPosX = lViewPortLeft + lRepeatHorizontalIndent
               lNewPosX = lViewPortLeft + lRepeatHorizontalIndent;
            }


            //:END

            //:szRepeatHorizontalEntityName = szCurrentEntityName
            ZeidonStringCopy( szRepeatHorizontalEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 );

            //:ELSE
         }
         else
         {

            //:// lRepeatHorizontal = lRepeatHorizontalTemp

            //:IF lRepeatHorizontal = 0
            if ( lRepeatHorizontal == 0 )
            {

               //:lViewPortLeft = lCurrentPosX
               lViewPortLeft = lCurrentPosX;
               //:lViewPortBottomPrev = lCurrentPosY
               lViewPortBottomPrev = lCurrentPosY;
               //:lViewPortTop = lCurrentPosY
               lViewPortTop = lCurrentPosY;
               //:lViewPortRight = lPageWidth
               lViewPortRight = lPageWidth;
               //:lViewPortBottom = lBottomOfPage
               lViewPortBottom = lBottomOfPage;
               //:lRepeatHorizontalFlags = 0
               lRepeatHorizontalFlags = 0;
               //:lRepeatHorizontalColumn = 0
               lRepeatHorizontalColumn = 0;
               //:lRepeatHorizontalIndent = 0
               lRepeatHorizontalIndent = 0;
               //:lRepeatHorizontalMargin = 0
               lRepeatHorizontalMargin = 0;
               //:lRepeatHorizontalExtent = 0
               lRepeatHorizontalExtent = 0;
               //:szRepeatHorizontalEntityName = ""
               ZeidonStringCopy( szRepeatHorizontalEntityName, 1, 0, "", 1, 0, 33 );

               //:ELSE
            }
            else
            {

               //:IF lRepeatHorizontalColumn > 1
               if ( lRepeatHorizontalColumn > 1 )
               {

                  //:lNewPosX = lRepeatHorizontalExtent + lRepeatHorizontalMargin
                  lNewPosX = lRepeatHorizontalExtent + lRepeatHorizontalMargin;
                  //:lNewPosX = lNewPosX * (lRepeatHorizontalColumn - 1)
                  lNewPosX = lNewPosX * ( lRepeatHorizontalColumn - 1 );
                  //:lNewPosX = lNewPosX + lRepeatHorizontalIndent
                  lNewPosX = lNewPosX + lRepeatHorizontalIndent;
                  //:lTemp = lNewPosX + lRepeatHorizontalExtent
                  lTemp = lNewPosX + lRepeatHorizontalExtent;
                  //:IF lTemp > lPageWidth AND lRepeatHorizontalColumn < lRepeatHorizontal
                  if ( lTemp > lPageWidth && lRepeatHorizontalColumn < lRepeatHorizontal )
                  {
                     //:lNewPosX = lViewPortLeft + lRepeatHorizontalIndent
                     lNewPosX = lViewPortLeft + lRepeatHorizontalIndent;
                     //:lViewPortTop = lCurrentPosY
                     lViewPortTop = lCurrentPosY;
                     //:lRepeatHorizontalColumn = 1
                     lRepeatHorizontalColumn = 1;
                     //:ELSE
                  }
                  else
                  {
                     //:IF lRepeatHorizontalNewspaper = 0
                     if ( lRepeatHorizontalNewspaper == 0 )
                     {
                        //:lNewPosY = lViewPortBottomPrev
                        lNewPosY = lViewPortBottomPrev;
                     }

                     //:END

                     //:lCurrentPosY = lNewPosY
                     lCurrentPosY = lNewPosY;
                     //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
                     lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;
                  }

                  //:END

                  //:ELSE
               }
               else
               {
                  //:lNewPosX = lViewPortLeft + lRepeatHorizontalIndent
                  lNewPosX = lViewPortLeft + lRepeatHorizontalIndent;
               }

               //:END

               //:lCurrentPosX = lNewPosX
               lCurrentPosX = lNewPosX;
               //:lCurrentPosY = lNewPosY
               lCurrentPosY = lNewPosY;
            }


            //:END
         }

         //:END

         //:// Force a page break if this entity requests ForcePageBreak.
         //:IF vReportDef.Group.ForcePageBreak = "Y" AND
         //:   lOrigRemainingSpaceOnPageY != lRemainingSpaceOnPageY
         if ( CompareAttributeToString( vReportDef, "Group", "ForcePageBreak", "Y" ) == 0 && lOrigRemainingSpaceOnPageY != lRemainingSpaceOnPageY )
         {

            //:// This entity requests a page break for each entity instance.
            //:// Thus process any page footers and return.

            //:// Go to process Group Footers.
            //:ProcessFooters( vReportDef, vSubtask, szCurrentEntityName,
            //:                lReturnLevel, lRemainingSpaceOnPageY,
            //:                lBottomOfPage, lCurrentPosX, lCurrentPosY,
            //:                lTemp,
            //:                lPageWidth, lPageHeight, 1 )
            oTZRPSRCO_ProcessFooters( vReportDef, vSubtask, szCurrentEntityName, lReturnLevel, lRemainingSpaceOnPageY, lBottomOfPage, &lCurrentPosX, &lCurrentPosY, &lTemp, lPageWidth, lPageHeight, 1 );

            //:// Process a Page Footer if necessary before returning.
            //:IF lPrintHTML = 1
            if ( lPrintHTML == 1 )
            {
               //:lPageHeight = lCurrentPosY + lFooterPosY
               lPageHeight = lCurrentPosY + lFooterPosY;
               //:lFooterPosY = lCurrentPosY
               lFooterPosY = lCurrentPosY;
            }

            //:END

            //:ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask,
            //:                   lNewPosX, lNewPosY, lCurrentPosX, lFooterPosY,
            //:                   lPageWidth, lPageHeight, lFlags )
            oTZRPSRCO_ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lFooterPosY, lPageWidth, lPageHeight, lFlags );

            //:// Return with an indication that there are more Pages and
            //:// save the CurrentEntityName, last HierReturnCode and
            //:// ReturnLevel.
            //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
            SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
            //:vReportDef.Report.HierReturnCode = lHierRC
            SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lHierRC );
            //:vReportDef.Report.LastLevel = lReturnLevel
            SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );

            //:RETURN 1 // return indicating that there are MORE pages
            return( 1 );
         }

         //:END

         //:// *** PROCESS FOOTER ***

         //:// Go to reusable routine to process any necessary footers.  A
         //:// return code of 1 indicates that not all of the necessary
         //:// Footers could fit on the page.
         //:nRC = 1
         nRC = 1;
         //:LOOP WHILE nRC = 1
         while ( nRC == 1 )
         {

            //:lViewPortBottomPrev = lCurrentPosY
            lViewPortBottomPrev = lCurrentPosY;
            //:nRC = ProcessFooters( vReportDef, vSubtask, szCurrentEntityName,
            //:                      lReturnLevel, lRemainingSpaceOnPageY,
            //:                      lBottomOfPage, lCurrentPosX, lCurrentPosY,
            //:                      lTemp,
            //:                      lPageWidth, lPageHeight, 0 )
            nRC = oTZRPSRCO_ProcessFooters( vReportDef, vSubtask, szCurrentEntityName, lReturnLevel, lRemainingSpaceOnPageY, lBottomOfPage, &lCurrentPosX, &lCurrentPosY, &lTemp, lPageWidth, lPageHeight, 0 );
            //:IF nRC = 1
            if ( nRC == 1 )
            {

               //:IF lRepeatHorizontal = 0 OR lRepeatHorizontalColumn >= lRepeatHorizontal
               if ( lRepeatHorizontal == 0 || lRepeatHorizontalColumn >= lRepeatHorizontal )
               {

                  //:// Process a Page Footer if necessary before returning.
                  //:IF lPrintHTML = 1
                  if ( lPrintHTML == 1 )
                  {
                     //:lPageHeight = lCurrentPosY + lFooterPosY
                     lPageHeight = lCurrentPosY + lFooterPosY;
                     //:lFooterPosY = lCurrentPosY
                     lFooterPosY = lCurrentPosY;
                  }

                  //:END

                  //:ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask,
                  //:                   lNewPosX, lNewPosY, lCurrentPosX, lFooterPosY,
                  //:                   lPageWidth, lPageHeight, lFlags )
                  oTZRPSRCO_ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lFooterPosY, lPageWidth, lPageHeight, lFlags );

                  //:// Return with an indication that there are more Pages and save
                  //:// the CurrentEntityName, last HierReturnCode and ReturnLevel.
                  //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
                  SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
                  //:vReportDef.Report.HierReturnCode = lHierRC
                  SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lHierRC );
                  //:vReportDef.Report.LastLevel = lReturnLevel
                  SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );

                  //:RETURN 1 // return indicating that there are MORE pages
                  return( 1 );

                  //:ELSE
               }
               else
               {

                  //:lRepeatHorizontalColumn = lRepeatHorizontalColumn + 1
                  lRepeatHorizontalColumn = lRepeatHorizontalColumn + 1;
                  //:lNewPosX = lRepeatHorizontalExtent + lRepeatHorizontalMargin
                  lNewPosX = lRepeatHorizontalExtent + lRepeatHorizontalMargin;
                  //:lNewPosX = lNewPosX * (lRepeatHorizontalColumn - 1)
                  lNewPosX = lNewPosX * ( lRepeatHorizontalColumn - 1 );
                  //:lNewPosX = lNewPosX + lRepeatHorizontalIndent
                  lNewPosX = lNewPosX + lRepeatHorizontalIndent;
                  //:lCurrentPosX = lNewPosX
                  lCurrentPosX = lNewPosX;
                  //:lNewPosY = lViewPortTop
                  lNewPosY = lViewPortTop;
                  //:lCurrentPosY = lNewPosY
                  lCurrentPosY = lNewPosY;

                  //:// We don't want to say that the footer did not fit in
                  //:// this case since we are going to the next column.
                  //:vTemp = vReportDef.Report.FooterDidNotFit
                  GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "Report", "FooterDidNotFit" );
                  //:IF vTemp != 0
                  if ( vTemp != 0 )
                  {
                     //:DropView( vTemp )
                     DropView( vTemp );
                     //:vReportDef.Report.FooterDidNotFit = 0
                     SetAttributeFromInteger( vReportDef, "Report", "FooterDidNotFit", 0 );
                  }

                  //:END
               }

               //:END
            }

            //:END
         }

         //:END

         //:// If the current entity is going to require a Footer later, set
         //:// up the necessary information to process it, if it does not
         //:// already exist.
         //:SET CURSOR FIRST vReportDef.Group WHERE
         //:                 vReportDef.Group.Type = "gf"
         RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "gf", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         {

            //:SET CURSOR FIRST vReportDef.FooterAtLevel WHERE
            //:    vReportDef.FooterAtLevel.EntityName = szCurrentEntityName
            RESULT = SetCursorFirstEntityByString( vReportDef, "FooterAtLevel", "EntityName", szCurrentEntityName, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            {

               //:CREATE ENTITY vReportDef.FooterAtLevel FIRST
               RESULT = CreateEntity( vReportDef, "FooterAtLevel", zPOS_FIRST );
               //:vReportDef.FooterAtLevel.Level = lReturnLevel
               SetAttributeFromInteger( vReportDef, "FooterAtLevel", "Level", lReturnLevel );
               //:vReportDef.FooterAtLevel.EntityName = szCurrentEntityName
               SetAttributeFromString( vReportDef, "FooterAtLevel", "EntityName", szCurrentEntityName );
               //:// CreateViewTrace( "Creating view (15): ", vFooterHierOI, vSourceHierOI, 0 )
               //:CreateViewFromViewForTask( vFooterHierOI, vSourceHierOI, 0 )
               CreateViewFromViewForTask( &vFooterHierOI, vSourceHierOI, 0 );
               //:SetAttributeFromView( vReportDef, "FooterAtLevel",
               //:                      "SourceOI_Hier", vFooterHierOI )
               SetAttributeFromView( vReportDef, "FooterAtLevel", "SourceOI_Hier", vFooterHierOI );
            }

            //://UNDVV        TraceLineV( "Creating FooterAtLevel with view: ", vFooterHierOI )
            //://UNDVV        DisplayEntityInstance( vReportDef, "PageState" )
            //://UNDVV        DisplayEntityInstance( vReportDef, "FooterAtLevel" )
            //:END
         }

         //:END

         //:// Get set up for Header check.  We're keeping track of the last
         //:// entity processed at each LOD level.
         //:SET CURSOR FIRST vReportDef.LastHeaderEntityAtLevel WHERE
         //:       vReportDef.LastHeaderEntityAtLevel.Level = lReturnLevel
         RESULT = SetCursorFirstEntityByInteger( vReportDef, "LastHeaderEntityAtLevel", "Level", lReturnLevel, "" );

         //://xNDEF  // This can cause a skip of a detail Group when defined
         //://xNDEF  IF lPageNbr <= 2 AND lFirstTime < 2
         //://xNDEF     TraceLineI( "Looking for ReturnLevel: ", lReturnLevel )
         //://xNDEF     TraceLineI( "                     RC: ", RESULT )
         //://xNDEF     IF lFirstTime < 1
         //://xNDEF        SET CURSOR FIRST vReportDef.Report
         //://xNDEF        LOOP WHILE RESULT >= 0
         //://xNDEF           DisplayEntityInstance( vReportDef, "Report" )
         //://xNDEF           SET CURSOR FIRST vReportDef.LastHeaderEntityAtLevel
         //://xNDEF           LOOP WHILE RESULT >= 0
         //://xNDEF              DisplayEntityInstance( vReportDef,
         //://xNDEF                                     "LastHeaderEntityAtLevel" )
         //://xNDEF              SET CURSOR NEXT vReportDef.LastHeaderEntityAtLevel
         //://xNDEF           END
         //://xNDEF        SET CURSOR NEXT vReportDef.Report
         //://xNDEF        END
         //://xNDEF     END
         //://xNDEF
         //://xNDEF     lFirstTime = lFirstTime + 1
         //://xNDEF
         //://xNDEF     SET CURSOR FIRST vReportDef.LastHeaderEntityAtLevel WHERE
         //://xNDEF           vReportDef.LastHeaderEntityAtLevel.Level = lReturnLevel
         //://xNDEF
         //://xNDEF  END

         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         {

            //:CREATE ENTITY vReportDef.LastHeaderEntityAtLevel
            RESULT = CreateEntity( vReportDef, "LastHeaderEntityAtLevel", zPOS_AFTER );
            //:vReportDef.LastHeaderEntityAtLevel.Level = lReturnLevel
            SetAttributeFromInteger( vReportDef, "LastHeaderEntityAtLevel", "Level", lReturnLevel );
            //:vReportDef.LastHeaderEntityAtLevel.EntityName = ""
            SetAttributeFromString( vReportDef, "LastHeaderEntityAtLevel", "EntityName", "" );
            //:szLastHeaderEntityNameAtLevel = ""
            ZeidonStringCopy( szLastHeaderEntityNameAtLevel, 1, 0, "", 1, 0, 33 );

            //:ELSE
         }
         else
         {
            //:/*dks*/     szLastHeaderEntityNameAtLevel =
            //:/*dks*/                       vReportDef.LastHeaderEntityAtLevel.EntityName
            GetVariableFromAttribute( szLastHeaderEntityNameAtLevel, 0, 'S', 33, vReportDef, "LastHeaderEntityAtLevel", "EntityName", "", 0 );
         }

         //:END

         //:// Added 11/15/03 by DonC
         //:// The following code was added because Group Headers were not being
         //:// generated except for the first set of twins on a page. You would
         //:// get a parent group printed, followed by children without a header.
         //:// Null any LastHeaderEntityAtLevel entries for entities that exist
         //:// at a lower level than the current entity. This will cause the
         //:// trigger of headers for those entities.
         //:// CreateViewTrace( "Creating view (16): ", vHeaderDef, vReportDef, 0 )
         //:CreateViewFromViewForTask( vHeaderDef, vReportDef, 0 )
         CreateViewFromViewForTask( &vHeaderDef, vReportDef, 0 );
         //:FOR EACH vHeaderDef.LastHeaderEntityAtLevel
         RESULT = SetCursorFirstEntity( vHeaderDef, "LastHeaderEntityAtLevel", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
            //:IF vHeaderDef.LastHeaderEntityAtLevel.Level > lReturnLevel
            if ( CompareAttributeToInteger( vHeaderDef, "LastHeaderEntityAtLevel", "Level", lReturnLevel ) > 0 )
            {
               //:vHeaderDef.LastHeaderEntityAtLevel.EntityName = ""
               SetAttributeFromString( vHeaderDef, "LastHeaderEntityAtLevel", "EntityName", "" );
            }

            RESULT = SetCursorNextEntity( vHeaderDef, "LastHeaderEntityAtLevel", "" );
            //:END
         }

         //:END

         //:// FOR EACH vHeaderDef.PageState
         //://    DisplayObjectInstance( vHeaderDef, "PageState", 0 )
         //:// END

         //:// DropViewTrace( "Dropping view (15): ", vHeaderDef )
         //:DropView( vHeaderDef )
         DropView( vHeaderDef );
         //:vHeaderDef = 0
         vHeaderDef = 0;

         //://xNDEF  IF lPageNbr <= 2 AND lFirstTime < 2
         //://xNDEF     TraceLineS( "===>>> szLastHeaderEntityNameAtLevel? ",
         //://xNDEF                 szLastHeaderEntityNameAtLevel )
         //://xNDEF  END

         //:// *** PROCESS HEADER ***

         //:// See if we need to put a Header in front of this Group.  The logic
         //:// here is to put a Header only before the first entity instance of
         //:// a set of twins, unless the ForceHeaderPerEntity flag is set.  In
         //:// that case, put a Header before every entity instance.
         //:// CreateViewTrace( "Creating view (17): ", vHeaderDef, vReportDef, 0 )
         //:CreateViewFromViewForTask( vHeaderDef, vReportDef, 0 )
         CreateViewFromViewForTask( &vHeaderDef, vReportDef, 0 );
         //:SET CURSOR FIRST vHeaderDef.Group WHERE
         //:                 vHeaderDef.Group.Type = "gh"
         RESULT = SetCursorFirstEntityByString( vHeaderDef, "Group", "Type", "gh", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         {

            //:IF lLastLevel < lReturnLevel OR
            //:   szLastHeaderEntityNameAtLevel != szCurrentEntityName OR  // ******
            //:   vHeaderDef.Group.ForceHeaderPerEntity = "Y"
            if ( lLastLevel < lReturnLevel || ZeidonStringCompare( szLastHeaderEntityNameAtLevel, 1, 0, szCurrentEntityName, 1, 0, 33 ) != 0 || CompareAttributeToString( vHeaderDef, "Group", "ForceHeaderPerEntity", "Y" ) == 0 )
            {

               //:// IF lLastLevel < lReturnLevel OR
               //:IF vHeaderDef.Group.ForceHeaderPerEntity = "Y"
               if ( CompareAttributeToString( vHeaderDef, "Group", "ForceHeaderPerEntity", "Y" ) == 0 )
               {

                  //:SET CURSOR FIRST vReportDef.FooterAtLevel WHERE
                  //:   vReportDef.FooterAtLevel.EntityName = szCurrentEntityName
                  RESULT = SetCursorFirstEntityByString( vReportDef, "FooterAtLevel", "EntityName", szCurrentEntityName, "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  {
                     //:vReportDef.FooterAtLevel.Complete = "Y"
                     SetAttributeFromString( vReportDef, "FooterAtLevel", "Complete", "Y" );
                  }

                  //:END
               }

               //:END

               //:// lLastLevel = lReturnLevel
               //:szLastHeaderEntityNameAtLevel = szCurrentEntityName
               ZeidonStringCopy( szLastHeaderEntityNameAtLevel, 1, 0, szCurrentEntityName, 1, 0, 33 );
               //:/*dks*/        vReportDef.LastHeaderEntityAtLevel.EntityName = szCurrentEntityName
               SetAttributeFromString( vReportDef, "LastHeaderEntityAtLevel", "EntityName", szCurrentEntityName );

               //:// Make sure there is enough space on the Page for both
               //:// the Header and the Attribute Group.  If not, we will
               //:// simply return here.  A return code of 1 means there
               //:// is more data to put on the next page.
               //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
               lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;
               //://UNDEF        TraceLineI( "RemainingSpaceOnPage ===> ", lRemainingSpaceOnPageY )
               //://UNDEF        lGroupTotal = vHeaderDef.Group.MinimumExtent +
               //://UNDEF                                     vReportDef.Group.MinimumExtent
               //://UNDEF        TraceLineI( "     GroupTotal MinExtent ", lGroupTotal )
               //:lGroupTotal = vHeaderDef.Group.SZDLG_Y + vReportDef.Group.SZDLG_Y
               GetIntegerFromAttribute( &lTempInteger_2, vHeaderDef, "Group", "SZDLG_Y" );
               GetIntegerFromAttribute( &lTempInteger_3, vReportDef, "Group", "SZDLG_Y" );
               lGroupTotal = lTempInteger_2 + lTempInteger_3;
               //://UNDEF        TraceLineI( "     GroupTotal SZDLG_Y=> ", lGroupTotal )
               //:IF lRemainingSpaceOnPageY < lGroupTotal
               if ( lRemainingSpaceOnPageY < lGroupTotal )
               {

                  //:IF lRepeatHorizontal = 0 OR lRepeatHorizontalColumn >= lRepeatHorizontal
                  if ( lRepeatHorizontal == 0 || lRepeatHorizontalColumn >= lRepeatHorizontal )
                  {

                     //:// Process a Page Footer if necessary before returning.
                     //:IF lPrintHTML = 1
                     if ( lPrintHTML == 1 )
                     {
                        //:lPageHeight = lCurrentPosY + lFooterPosY
                        lPageHeight = lCurrentPosY + lFooterPosY;
                        //:lFooterPosY = lCurrentPosY
                        lFooterPosY = lCurrentPosY;
                     }

                     //:END

                     //:ProcessPageFooter( vReportDef, vSourceHierOI,
                     //:                   vSubtask, lNewPosX, lNewPosY,
                     //:                   lCurrentPosX, lFooterPosY,
                     //:                   lPageWidth, lPageHeight, lFlags )
                     oTZRPSRCO_ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lFooterPosY, lPageWidth, lPageHeight, lFlags );

                     //:// Return with an indication that there are more
                     //:// Pages and save the CurrentEntityName, last
                     //:// HierReturnCode and ReturnLevel.
                     //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
                     SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
                     //:vReportDef.Report.HierReturnCode = lHierRC
                     SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lHierRC );
                     //:vReportDef.Report.LastLevel = lReturnLevel
                     SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );

                     //:RETURN 1 // return indicating there are MORE pages
                     return( 1 );
                  }


                  //:// ELSE  this was removed so when splitting a group, the  // dks 2005.07.04
                  //://       RepeatHorizontalColumn was not incremented until needed!
                  //://    lRepeatHorizontalColumn = lRepeatHorizontalColumn + 1
                  //://    lNewPosY = lViewPortTop

                  //:END
               }

               //:END

               //://UNDEF        IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
               //://UNDEF           TraceLineI( "Page 2 GroupHeader RC: ", RESULT )
               //://UNDEF           TraceLineI( "            LastLevel: ", lLastLevel )
               //://UNDEF           TraceLineI( "          ReturnLevel: ", lReturnLevel )
               //://UNDEF           TraceLineS( "     szLastHeaderEntityNameAtLevel: ",
               //://UNDEF                       szLastHeaderEntityNameAtLevel )
               //://UNDEF           TraceLineS( "  szCurrentEntityName: ", szCurrentEntityName )
               //://UNDEF        END

               //:// Put Header on Page.
               //://UNDEF        IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
               //://UNDEF           TraceLineS( "FormatGroupOnPage 6: ",
               //://UNDEF                       vHeaderDef.Group.Tag )
               //://UNDEF        END

               //:// If this is not the first Group Header on the page, format
               //:// it.  If it is the first Group Header on the page, it was
               //:// formatted earlier.
               //:FormatGroupOnPage( vHeaderDef, vSourceHierOI, vSubtask,
               //:                   lNewPosX, lNewPosY, lCurrentPosX, lCurrentPosY,
               //:                   lRemainingSpaceOnPageY,
               //:                   0, // continued Group flag 0 = not continued
               //:                   lPageWidth, lPageHeight,
               //:                   lOrigRemainingSpaceOnPageY,
               //:                   lBottomOfPage )
               oTZRPSRCO_FormatGroupOnPage( vHeaderDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, 0, lPageWidth, lPageHeight, lOrigRemainingSpaceOnPageY, lBottomOfPage );

               //:IF lRepeatHorizontalColumn = 1
               if ( lRepeatHorizontalColumn == 1 )
               {
                  //:lTemp = lNewPosX - lRepeatHorizontalIndent
                  lTemp = lNewPosX - lRepeatHorizontalIndent;
                  //:IF lRepeatHorizontalExtent < lTemp
                  if ( lRepeatHorizontalExtent < lTemp )
                  {
                     //:lRepeatHorizontalExtent = lTemp
                     lRepeatHorizontalExtent = lTemp;
                  }

                  //:END
               }

               //:END

               //:// lNewPosX = 0  // this should depend on the "horizontal repeat" settings ???
               //:lNewPosX = lRepeatHorizontalIndent
               lNewPosX = lRepeatHorizontalIndent;
               //:lCurrentPosY = lNewPosY
               lCurrentPosY = lNewPosY;
               //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
               lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;
            }

            //:END
         }

         //:END

         //:// DropViewTrace( "Dropping view (16): ", vHeaderDef )
         //:DropView( vHeaderDef )
         DropView( vHeaderDef );
         //:vHeaderDef = 0
         vHeaderDef = 0;

         //:// Format the Attribute information for the entity, if it is
         //:// defined in the Report Def.
         //:SET CURSOR FIRST vReportDef.Group WHERE
         //:                 vReportDef.Group.Type = "ga"
         RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "ga", "" );

         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         {

            //:// *** PROCESS REGULAR GROUP ***

            //:// Put Attribute data on Page, if there is enough room.
            //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
            lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;

            //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)     /*TracePages*/
            //://UNDEF        TraceLineS( "FormatGroupOnPage REGULAR GROUP GroupSet: ",
            //://UNDEF                    szRptGroupSetTag )
            //://UNDEF        TraceLineS( "                                   Group: ",
            //://UNDEF                    szRptGroupTag )
            //://UNDEF        lTempValue = vReportDef.Group.Extent
            //://UNDEF        TraceLineI( "                    RemainingSpaceOnPage: ",
            //://UNDEF                    lRemainingSpaceOnPageY )
            //://UNDEF        TraceLineI( "                            Group.Extent: ",
            //://UNDEF                    lTempValue )
            //://UNDEF     END

            //:szRptGroupSetTag = vReportDef.GroupSet.Tag
            GetVariableFromAttribute( szRptGroupSetTag, 0, 'S', 33, vReportDef, "GroupSet", "Tag", "", 0 );
            //:szRptGroupTag = vReportDef.Group.Tag
            GetVariableFromAttribute( szRptGroupTag, 0, 'S', 33, vReportDef, "Group", "Tag", "", 0 );

            //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)  /*TracePages*/
            //://UNDEF        TraceLineS( "FormatGroupOnPage 7: ",
            //://UNDEF                    vReportDef.Group.Tag )
            //://UNDEF     END

            //:lContinuedGroupFlag = 0 //  0 ==> not continued
            lContinuedGroupFlag = 0;
            //:nRC = 0
            nRC = 0;
            //:LOOP WHILE nRC = 0
            while ( nRC == 0 )
            {

               //:IF lRemainingSpaceOnPageY > vReportDef.Group.SZDLG_Y OR
               //:   vReportDef.Group.AllowPageBreak = "Y" OR
               //:   lPrintHTML = 1 OR
               //:   lRemainingSpaceOnPageY = lOrigRemainingSpaceOnPageY
               if ( CompareAttributeToInteger( vReportDef, "Group", "SZDLG_Y", lRemainingSpaceOnPageY ) < 0 || CompareAttributeToString( vReportDef, "Group", "AllowPageBreak", "Y" ) == 0 || lPrintHTML == 1 ||
                    lRemainingSpaceOnPageY == lOrigRemainingSpaceOnPageY )
               {

                  //:lViewPortBottomPrev = lNewPosY
                  lViewPortBottomPrev = lNewPosY;
                  //:nRC_GP = FormatGroupOnPage( vReportDef, vSourceHierOI,
                  //:                            vSubtask, lNewPosX, lNewPosY,
                  //:                            lCurrentPosX, lCurrentPosY,
                  //:                            lRemainingSpaceOnPageY,
                  //:                            lContinuedGroupFlag,
                  //:                            lPageWidth, lPageHeight,
                  //:                            lOrigRemainingSpaceOnPageY,
                  //:                            lBottomOfPage )
                  nRC_GP = oTZRPSRCO_FormatGroupOnPage( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, lContinuedGroupFlag, lPageWidth, lPageHeight, lOrigRemainingSpaceOnPageY,
                        lBottomOfPage );
                  //:ELSE
               }
               else
               {
                  //:nRC_GP = -2
                  nRC_GP = -2;
               }

               //:END


               //:vReportDef.PageState.MultiLineTextPosStartPage = 0
               SetAttributeFromInteger( vReportDef, "PageState", "MultiLineTextPosStartPage", 0 );
               //:vReportDef.PageState.ContinuedControlTagStartPage = ""
               SetAttributeFromString( vReportDef, "PageState", "ContinuedControlTagStartPage", "" );
               //:vReportDef.PageState.ContinuedGroupFlagStartPage = 0
               SetAttributeFromInteger( vReportDef, "PageState", "ContinuedGroupFlagStartPage", 0 );
               //:vReportDef.PageState.ContinuedControlTagStartPage = ""
               SetAttributeFromString( vReportDef, "PageState", "ContinuedControlTagStartPage", "" );
               //:lContinuedGroupFlag = 0
               lContinuedGroupFlag = 0;

               //:nRC = 1
               nRC = 1;
               //:IF lRepeatHorizontal != 0
               if ( lRepeatHorizontal != 0 )
               {

                  //:IF lRepeatHorizontalColumn = 1
                  if ( lRepeatHorizontalColumn == 1 )
                  {

                     //:lTemp = lNewPosX - lRepeatHorizontalIndent
                     lTemp = lNewPosX - lRepeatHorizontalIndent;
                     //:IF lRepeatHorizontalExtent < lTemp
                     if ( lRepeatHorizontalExtent < lTemp )
                     {

                        //:lRepeatHorizontalExtent = lTemp
                        lRepeatHorizontalExtent = lTemp;
                     }


                     //:END
                  }


                  //:END

                  //:IF lRepeatHorizontalNewspaper = 0  // ==> horizontal first
                  if ( lRepeatHorizontalNewspaper == 0 )
                  {

                     //:lNewPosX = lRepeatHorizontalExtent + lRepeatHorizontalMargin
                     lNewPosX = lRepeatHorizontalExtent + lRepeatHorizontalMargin;
                     //:lNewPosX = lNewPosX * lRepeatHorizontalColumn
                     lNewPosX = lNewPosX * lRepeatHorizontalColumn;
                     //:lNewPosX = lNewPosX + lRepeatHorizontalIndent
                     lNewPosX = lNewPosX + lRepeatHorizontalIndent;
                     //:lTemp = lNewPosX + lRepeatHorizontalExtent
                     lTemp = lNewPosX + lRepeatHorizontalExtent;
                     //:lRepeatHorizontalColumn = lRepeatHorizontalColumn + 1
                     lRepeatHorizontalColumn = lRepeatHorizontalColumn + 1;

                     //:nRC = 1
                     nRC = 1;
                     //:szHoldEntityName = szCurrentEntityName
                     ZeidonStringCopy( szHoldEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 );
                     //:LOOP WHILE vSourceHierOI != 0 AND nRC = 1 AND
                     //:           lHierRC >= zCURSOR_SET
                     while ( vSourceHierOI != 0 && nRC == 1 && lHierRC >= zCURSOR_SET )
                     {

                        //:lHierRC = SetCursorNextEntityHierarchical( nReturnLevel,
                        //:                                           szCurrentEntityName,
                        //:                                           vSourceHierOI )
                        lHierRC = SetCursorNextEntityHierarchical( (zPUSHORT) &nReturnLevel, szCurrentEntityName, vSourceHierOI );
                        //:IF lHierRC >= zCURSOR_SET
                        if ( lHierRC >= zCURSOR_SET )
                        {

                           //:// For recursive subentity, step down a level.
                           //:IF lHierRC = zCURSOR_SET_RECURSIVECHILD
                           if ( lHierRC == zCURSOR_SET_RECURSIVECHILD )
                           {
                              //:SetViewToSubobject( vSourceHierOI, szCurrentEntityName )
                              SetViewToSubobject( vSourceHierOI, szCurrentEntityName );
                              //:GetEntityNameForHierarchicalCsr( nReturnLevel, szCurrentEntityName, lAbsolutePosition, vSourceHierOI )
                              GetEntityNameForHierarchicalCsr( (zPUSHORT) &nReturnLevel, szCurrentEntityName, (zPULONG) &lAbsolutePosition, vSourceHierOI );
                           }

                           //:END

                           //:GetEntityKeyForHierarchicalCsr( lDksLevel, szCurrentEntityName,
                           //:                                lDksKey,  vSourceHierOI )
                           GetEntityKeyForHierarchicalCsr( &lDksLevel, szCurrentEntityName, (zPULONG) &lDksKey, vSourceHierOI );
                        }

                        //:   //TraceLine( "Processing Entity: %s   Key: %d", szCurrentEntityName, lDksKey )
                        //:END

                        //:IF szCurrentEntityName = szHoldEntityName
                        if ( ZeidonStringCompare( szCurrentEntityName, 1, 0, szHoldEntityName, 1, 0, 33 ) == 0 )
                        {
                           //:nRC = 0
                           nRC = 0;
                        }

                        //:END
                     }


                     //:END

                     //:IF nRC = 0   // we located another "repeat" entity
                     if ( nRC == 0 )
                     {

                        //:IF lTemp < lPageWidth  // this one fits on current "line"
                        if ( lTemp < lPageWidth )
                        {

                           //:lNewPosY = lViewPortBottomPrev
                           lNewPosY = lViewPortBottomPrev;
                           //:lCurrentPosY = lNewPosY
                           lCurrentPosY = lNewPosY;
                           //:lRemainingSpaceOnPageY = lBottomOfPage - lNewPosY
                           lRemainingSpaceOnPageY = lBottomOfPage - lNewPosY;
                           //:// lRemainingSpaceOnPageY = lOrigRemainingSpaceOnPageY
                           //:lCurrentPosX = lNewPosX
                           lCurrentPosX = lNewPosX;

                           //:ELSE
                        }
                        else
                        {

                           //:lRemainingSpaceOnPageY = lBottomOfPage - lNewPosY
                           lRemainingSpaceOnPageY = lBottomOfPage - lNewPosY;
                           //:IF lRemainingSpaceOnPageY > vReportDef.Group.SZDLG_Y
                           if ( CompareAttributeToInteger( vReportDef, "Group", "SZDLG_Y", lRemainingSpaceOnPageY ) < 0 )
                           {

                              //:// We need to move to next "line".
                              //:lNewPosX = lRepeatHorizontalIndent
                              lNewPosX = lRepeatHorizontalIndent;
                              //:lCurrentPosX = lRepeatHorizontalIndent
                              lCurrentPosX = lRepeatHorizontalIndent;
                              //:lRepeatHorizontalColumn = 1
                              lRepeatHorizontalColumn = 1;
                              //:lCurrentPosY = lNewPosY
                              lCurrentPosY = lNewPosY;
                              //:lOrigRemainingSpaceOnPageY = lRemainingSpaceOnPageY
                              lOrigRemainingSpaceOnPageY = lRemainingSpaceOnPageY;

                              //:ELSE
                           }
                           else
                           {

                              //:nRC = 1  // does not fit on this page
                              nRC = 1;
                              //:nRC_GP = -1
                              nRC_GP = -1;
                           }


                           //:END
                        }

                        //:END
                        //:ELSE
                     }
                     else
                     {

                        //:// We didn't locate any more "repeat" entities.
                        //:nRC_GP = 0
                        nRC_GP = 0;
                     }


                     //:END

                     //:ELSE
                  }
                  else
                  {

                     //:// This is the Newspaper case.
                     //:IF nRC_GP = -1 OR  //*****  MULTI EDIT
                     //:   nRC_GP = -2     // Did not fit
                     if ( nRC_GP == -1 || nRC_GP == -2 )
                     {

                        //:lNewPosX = lRepeatHorizontalExtent + lRepeatHorizontalMargin
                        lNewPosX = lRepeatHorizontalExtent + lRepeatHorizontalMargin;
                        //:lNewPosX = lNewPosX * lRepeatHorizontalColumn
                        lNewPosX = lNewPosX * lRepeatHorizontalColumn;
                        //:lNewPosX = lNewPosX + lRepeatHorizontalIndent
                        lNewPosX = lNewPosX + lRepeatHorizontalIndent;
                        //:lTemp = lNewPosX + lRepeatHorizontalExtent
                        lTemp = lNewPosX + lRepeatHorizontalExtent;
                        //:lRepeatHorizontalColumn = lRepeatHorizontalColumn + 1
                        lRepeatHorizontalColumn = lRepeatHorizontalColumn + 1;
                        //:IF (lTemp < lPageWidth AND
                        //:    lRepeatHorizontalColumn <= lRepeatHorizontal) OR
                        //:   lRepeatHorizontalColumn = lRepeatHorizontal
                        if ( ( lTemp < lPageWidth && lRepeatHorizontalColumn <= lRepeatHorizontal ) || lRepeatHorizontalColumn == lRepeatHorizontal )
                        {

                           //:IF nRC_GP = -1
                           if ( nRC_GP == -1 )
                           {

                              //:vReportDef.PageState.MultiLineTextPosStartPage =
                              //:           vReportDef.PageState.MultiLineTextPosEndPage
                              SetAttributeFromAttribute( vReportDef, "PageState", "MultiLineTextPosStartPage", vReportDef, "PageState", "MultiLineTextPosEndPage" );
                              //:vReportDef.PageState.ContinuedControlTagStartPage =
                              //:           vReportDef.PageState.ContinuedControlTagEndPage
                              SetAttributeFromAttribute( vReportDef, "PageState", "ContinuedControlTagStartPage", vReportDef, "PageState", "ContinuedControlTagEndPage" );
                              //:vReportDef.PageState.ContinuedGroupFlagStartPage =
                              //:           vReportDef.PageState.ContinuedGroupFlagEndPage
                              SetAttributeFromAttribute( vReportDef, "PageState", "ContinuedGroupFlagStartPage", vReportDef, "PageState", "ContinuedGroupFlagEndPage" );
                              //:vReportDef.PageState.ContinuedControlTagStartPage =
                              //:           vReportDef.PageState.ContinuedControlTagEndPage
                              SetAttributeFromAttribute( vReportDef, "PageState", "ContinuedControlTagStartPage", vReportDef, "PageState", "ContinuedControlTagEndPage" );
                              //:lContinuedGroupFlag = 1
                              lContinuedGroupFlag = 1;
                           }


                           //:END

                           //:lNewPosY = lViewPortTop
                           lNewPosY = lViewPortTop;
                           //:lRemainingSpaceOnPageY = lOrigRemainingSpaceOnPageY
                           lRemainingSpaceOnPageY = lOrigRemainingSpaceOnPageY;
                           //:lCurrentPosX = lNewPosX
                           lCurrentPosX = lNewPosX;
                           //:lCurrentPosY = lNewPosY
                           lCurrentPosY = lNewPosY;
                           //:nRC = 0
                           nRC = 0;
                           //:nRC_GP = 0
                           nRC_GP = 0;

                           //:ELSE
                        }
                        else
                        {

                           //:lNewPosX = lViewPortLeft
                           lNewPosX = lViewPortLeft;
                           //:lRepeatHorizontalColumn = 1
                           lRepeatHorizontalColumn = 1;
                           //:nRC = 1
                           nRC = 1;
                        }


                        //:END
                     }


                     //:END
                  }

                  //:END
               }

               //:END

               //:IF nRC = 1
               if ( nRC == 1 )
               {

                  //:// lNewPosX = 0   // reset for next group
                  //:lNewPosX = lRepeatHorizontalIndent   // reset for next group
                  lNewPosX = lRepeatHorizontalIndent;

                  //:// This code is executed when we are processing a detail
                  //:// Group.  We want to save the level any time we read a new
                  //:// entity, not only when we format a Group detail.  So we
                  //:// now save the level below (see IF vSourceHierOI != 0).
                  //:// lLastLevel = lReturnLevel  // save Level of the last entity
                  //:                           // on which we had position.
                  //:IF nRC_GP = -1 OR //*****  MULTI EDIT
                  //:   nRC_GP = -2    // did not fit ... AND lRepeatHorizontal != 0)
                  if ( nRC_GP == -1 || nRC_GP == -2 )
                  {

                     //:// If the return code from FormatGroupOnPage is -1, then
                     //:// the Group could not fit on the page due to a multi-line
                     //:// text field.  For this case process any page footers and
                     //:// return.  FormatGroupOnPage has already set up things for
                     //:// continuing on the next page.

                     //:// Process a Page Footer if necessary before returning.
                     //:IF lPrintHTML = 1
                     if ( lPrintHTML == 1 )
                     {
                        //:lPageHeight = lCurrentPosY + lFooterPosY
                        lPageHeight = lCurrentPosY + lFooterPosY;
                        //:lFooterPosY = lCurrentPosY
                        lFooterPosY = lCurrentPosY;
                     }

                     //:END

                     //:ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask,
                     //:                   lNewPosX, lNewPosY, lCurrentPosX,
                     //:                   lFooterPosY, lPageWidth, lPageHeight,
                     //:                   lFlags )
                     oTZRPSRCO_ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lFooterPosY, lPageWidth, lPageHeight, lFlags );

                     //:// Return with an indication that there are more Pages and
                     //:// save the CurrentEntityName, last HierReturnCode and
                     //:// ReturnLevel.
                     //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
                     SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
                     //:vReportDef.Report.HierReturnCode = lHierRC
                     SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lHierRC );
                     //:vReportDef.Report.LastLevel = lReturnLevel
                     SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );

                     //:RETURN 1 // return indicating that there are MORE pages
                     return( 1 );

                     //:ELSE
                  }
                  else
                  {
                     //:lCurrentPosY = lNewPosY
                     lCurrentPosY = lNewPosY;
                     //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
                     lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;
                  }

                  //:END
               }

               //:END
            }

            //:END
         }

         //:END

         //:PushReportViewPort( vSubtask, szRepeatHorizontalEntityName,
         //:                    lRepeatHorizontal, lRepeatHorizontalColumn,
         //:                    lRepeatHorizontalIndent, lRepeatHorizontalMargin,
         //:                    lRepeatHorizontalExtent, lRepeatHorizontalFlags,
         //:                    lViewPortBottomPrev, lViewPortTop, lViewPortLeft,
         //:                    lViewPortBottom, lViewPortRight )
         PushReportViewPort( vSubtask, szRepeatHorizontalEntityName, lRepeatHorizontal, lRepeatHorizontalColumn, lRepeatHorizontalIndent, lRepeatHorizontalMargin, lRepeatHorizontalExtent, lRepeatHorizontalFlags, lViewPortBottomPrev,
                        lViewPortTop, lViewPortLeft, lViewPortBottom, lViewPortRight );

         //:ELSE
      }
      else
      {
      }


      //:// TraceLineS( "PrintPage3 unable to locate GroupSet: ",
      //://             szCurrentEntityName )

      //:END

      //:IF vSourceHierOI != 0
      if ( vSourceHierOI != 0 )
      {
         //:// We want to save the level of the last entity on which we have
         //:// position anytime we read a new entity.  In the past, we saved
         //:// the level (above) only when we processed a detail Group.
         //:/*dks*/  lLastLevel = lReturnLevel  // save Level of the last entity
         lLastLevel = lReturnLevel;
         //:                           // on which we had position.

         //://>      IF lHierRC >= zCURSOR_SET

         //:// dks/pas 2005.11.17 - start
         //:// This code is added to cause a detail footer to be processed
         //:// when necessary.  If there are sub-entities to the entity
         //:// being processed, but none of them are "report entities", ignore
         //:// them and get to the next "reportable" entity.
         //:szHoldEntityName = szCurrentEntityName
         ZeidonStringCopy( szHoldEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 );
         //:lHierRC = zCURSOR_SET
         lHierRC = zCURSOR_SET;
         //:lTempLevel = lLastLevel + 1
         lTempLevel = lLastLevel + 1;
         //:LOOP WHILE lHierRC >= zCURSOR_SET AND lTempLevel > lLastLevel
         while ( lHierRC >= zCURSOR_SET && lTempLevel > lLastLevel )
         {

            //:lHierRC = SetCursorNextEntityHierarchical( nReturnLevel,
            //:                                           szCurrentEntityName,
            //:                                           vSourceHierOI )
            lHierRC = SetCursorNextEntityHierarchical( (zPUSHORT) &nReturnLevel, szCurrentEntityName, vSourceHierOI );
            //:IF lHierRC >= zCURSOR_SET
            if ( lHierRC >= zCURSOR_SET )
            {

               //:// For recursive subentity, step down a level.
               //:IF lHierRC = zCURSOR_SET_RECURSIVECHILD
               if ( lHierRC == zCURSOR_SET_RECURSIVECHILD )
               {
                  //:SetViewToSubobject( vSourceHierOI, szCurrentEntityName )
                  SetViewToSubobject( vSourceHierOI, szCurrentEntityName );
                  //:GetEntityNameForHierarchicalCsr( nReturnLevel, szCurrentEntityName, lAbsolutePosition, vSourceHierOI )
                  GetEntityNameForHierarchicalCsr( (zPUSHORT) &nReturnLevel, szCurrentEntityName, (zPULONG) &lAbsolutePosition, vSourceHierOI );
               }

               //:END

               //:GetEntityKeyForHierarchicalCsr( lDksLevel, szCurrentEntityName,
               //:                                lDksKey,  vSourceHierOI )
               GetEntityKeyForHierarchicalCsr( &lDksLevel, szCurrentEntityName, (zPULONG) &lDksKey, vSourceHierOI );
               //://TraceLine( "Processing Entity: %s   Key: %d", szCurrentEntityName, lDksKey )

               //:lTempLevel = nReturnLevel
               lTempLevel = nReturnLevel;
               //:IF lTempLevel > lLastLevel
               if ( lTempLevel > lLastLevel )
               {

                  //:// Only process the entity if there is a GroupSet whose Tag matches
                  //:// the name of the Entity.
                  //:SET CURSOR FIRST vReportDef.GroupSet WHERE
                  //:                 vReportDef.GroupSet.Tag = szCurrentEntityName
                  RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szCurrentEntityName, "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  {

                     //:lTempLevel = lLastLevel
                     lTempLevel = lLastLevel;
                     //:SET CURSOR FIRST vReportDef.GroupSet WHERE
                     //:                 vReportDef.GroupSet.Tag = szHoldEntityName
                     RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szHoldEntityName, "" );
                  }

                  //:END
               }

               //:END
            }

            //:END
         }

         //:END
         //:// dks/pas 2005.11.17 - end

         //://>      END

         //:// IF lHierRC < zCURSOR_SET OR
         //://    (nReturnLevel = lReturnLevel AND
         //://     szCurrentEntityName = vReportDef.Report.CurrentEntityName)
         //://
         //://    IF vDeferredOI != 0
         //://       DropView( vDeferredOI )
         //://       vDeferredOI = 0
         //://    END
         //://
         //://    vReportDef.Report.DeferredOI = 0
         //://
         //:// END

         //:lReturnLevel = nReturnLevel
         lReturnLevel = nReturnLevel;

         //:// dks/pas 2005.11.17 - start
         //:// Process a Group Footer if necessary before returning.
         //:IF szHoldEntityName != szCurrentEntityName
         if ( ZeidonStringCompare( szHoldEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 ) != 0 )
         {

            //:ProcessFooters( vReportDef, vSubtask, szHoldEntityName,
            //:                lReturnLevel, lRemainingSpaceOnPageY,
            //:                lBottomOfPage, lCurrentPosX, lCurrentPosY,
            //:                lRepeatHorizontalHold, lPageWidth, lPageHeight, 0 )
            oTZRPSRCO_ProcessFooters( vReportDef, vSubtask, szHoldEntityName, lReturnLevel, lRemainingSpaceOnPageY, lBottomOfPage, &lCurrentPosX, &lCurrentPosY, &lRepeatHorizontalHold, lPageWidth, lPageHeight, 0 );
         }

         //:END
         //:// dks/pas 2005.11.17 - end

         //://UNDEF  IF lHierRC >= zCURSOR_SET
         //://UNDEF     TraceLineS( "DisplayEI SetCursorNextEntityHierarchical 5", "" )
         //://UNDEF     DisplayEntityInstance( vSourceHierOI, szCurrentEntityName )
         //://UNDEF  END

         //:ELSE
      }
      else
      {
         //:lHierRC = zCURSOR_NULL
         lHierRC = zCURSOR_NULL;
      }

      //:END

      //:IF lHierRC < zCURSOR_SET
      if ( lHierRC < zCURSOR_SET )
      {

         //:szCurrentEntityName = ""
         ZeidonStringCopy( szCurrentEntityName, 1, 0, "", 1, 0, 33 );
         //:szHierEntityName = ""
         ZeidonStringCopy( szHierEntityName, 1, 0, "", 1, 0, 33 );
         //:lReturnLevel = 0
         lReturnLevel = 0;

         //:ELSE
      }
      else
      {

         //:// I think this is the correct place to store state information???
         //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
         SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
         //:vReportDef.Report.HierReturnCode = lHierRC
         SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lHierRC );
         //:vReportDef.Report.LastLevel = lReturnLevel
         SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );
      }

      //:// vReportDef.Report.ForcePageBreak = bForcePageBreak
      //:// lTempView = MakeIntegerFromView( vFooterDidNotFit )
      //:// vReportDef.Report.FooterDidNotFit = lTempView

      //://xSET CURSOR FIRST vReportDef.FooterAtLevel WHERE
      //://x    vReportDef.FooterAtLevel.EntityName = szCurrentEntityName
      //://xIF RESULT >= zCURSOR_SET
      //://x   vReportDef.FooterAtLevel.Complete = "Y"
      //://xEND

      //://UNDEF  TraceLineS( "Processing hierarchical entity: ",
      //://UNDEF              szCurrentEntityName )
      //://UNDEF  TraceLineS( "SubobjectEntityName: ", szSubobjectEntityName )
      //://UNDEF  TraceLineI( "HierReturnCode: ", lHierRC )
      //://UNDEF  TraceLineI( "ReturnLevel: ", lReturnLevel )
      //://UNDEF  TraceLineI( "StartLevel: ", lStartLevel )

      //:END

      //:lRepeatHorizontalHold = 0
      lRepeatHorizontalHold = 0;
   }


   //:END

   //:// Go to reusable routine to process any remaining footers.  A return code
   //:// of 1 indicates that not all of the necessary Footers could fit on the
   //:// page.  lReturnLevel is 0 to force all remaining Footers.
   //:lReturnLevel = 0
   lReturnLevel = 0;
   //:vReportDef.Report.LastLevel = lReturnLevel
   SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );
   //:nRC = ProcessFooters( vReportDef, vSubtask, szCurrentEntityName,
   //:                      lReturnLevel, lRemainingSpaceOnPageY,
   //:                      lBottomOfPage, lCurrentPosX, lCurrentPosY,
   //:                      lTemp,
   //:                      lPageWidth, lPageHeight, 0 )
   nRC = oTZRPSRCO_ProcessFooters( vReportDef, vSubtask, szCurrentEntityName, lReturnLevel, lRemainingSpaceOnPageY, lBottomOfPage, &lCurrentPosX, &lCurrentPosY, &lTemp, lPageWidth, lPageHeight, 0 );
   //:IF nRC = 1
   if ( nRC == 1 )
   {
      //:// Process a Page Footer if necessary before returning.
      //:IF lPrintHTML = 1
      if ( lPrintHTML == 1 )
      {
         //:lPageHeight = lCurrentPosY + lFooterPosY
         lPageHeight = lCurrentPosY + lFooterPosY;
         //:lFooterPosY = lCurrentPosY
         lFooterPosY = lCurrentPosY;
      }

      //:END

      //:ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask,
      //:                   lNewPosX, lNewPosY, lCurrentPosX, lFooterPosY,
      //:                   lPageWidth, lPageHeight, lFlags )
      oTZRPSRCO_ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lFooterPosY, lPageWidth, lPageHeight, lFlags );

      //:// Return with an indication that there are more Pages and save
      //:// the CurrentEntityName, last HierReturnCode and ReturnLevel.
      //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
      SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
      //:vReportDef.Report.HierReturnCode = lHierRC
      SetAttributeFromInteger( vReportDef, "Report", "HierReturnCode", lHierRC );
      //:vReportDef.Report.LastLevel = lReturnLevel
      SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );

      //:RETURN 1 // return indicating that there are MORE pages
      return( 1 );
   }


   //:END

   //:// Process a Page Footer if necessary before returning.
   //:IF lPrintHTML = 1
   if ( lPrintHTML == 1 )
   {
      //:lPageHeight = lCurrentPosY + lFooterPosY
      lPageHeight = lCurrentPosY + lFooterPosY;
      //:lFooterPosY = lCurrentPosY
      lFooterPosY = lCurrentPosY;
   }

   //:END

   //:ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask,
   //:                   lNewPosX, lNewPosY, lCurrentPosX, lFooterPosY,
   //:                   lPageWidth, lPageHeight, lFlags )
   oTZRPSRCO_ProcessPageFooter( vReportDef, vSourceHierOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lFooterPosY, lPageWidth, lPageHeight, lFlags );

   //:// We cannot do this since we need the view to reprint a page in preview
   //:// mode.
   //:// IF vSourceHierOI != 0
   //://    DropView( vSourceHierOI ) // Done with hierarchical view, so drop it.
   //:// END

   //:RETURN 0  // return indicating that this is the LAST page
   return( 0 );
// END
}


//:TRANSFORMATION OPERATION
//:FormatGroupOnPage( VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:                   VIEW    vSourceHierOI,
//:                   VIEW    vSubtask,
//:                   INTEGER plNewPosX,
//:                   INTEGER plNewPosY,
//:                   INTEGER lCurrentPosX,    // position of Group on page
//:                   INTEGER lCurrentPosY,    // position of Group on page
//:                   INTEGER lRemainingSpaceOnPageY,
//:                   INTEGER lContinuedGroupFlag,
//:                   INTEGER lPageWidth,
//:                   INTEGER lPageHeight,
//:                   INTEGER lOrigRemainingSpaceOnPageY,
//:                   INTEGER lFooterPosPageBottom )

//:   VIEW        vReportDefTemp
static zSHORT
oTZRPSRCO_FormatGroupOnPage( zVIEW     vReportDef,
                             zVIEW     vSourceHierOI,
                             zVIEW     vSubtask,
                             zPLONG    plNewPosX,
                             zPLONG    plNewPosY,
                             zLONG     lCurrentPosX,
                             zLONG     lCurrentPosY,
                             zLONG     lRemainingSpaceOnPageY,
                             zLONG     lContinuedGroupFlag,
                             zLONG     lPageWidth,
                             zLONG     lPageHeight,
                             zLONG     lOrigRemainingSpaceOnPageY,
                             zLONG     lFooterPosPageBottom )
{
   zVIEW     vReportDefTemp = 0;
   //:INTEGER     lContinuedGroupSave
   zLONG     lContinuedGroupSave = 0;
   //:INTEGER     lOrigPosY         // original y-position of control on page
   zLONG     lOrigPosY = 0;
   //:INTEGER     lOrigRemainingSpace
   zLONG     lOrigRemainingSpace = 0;
   //:INTEGER     lFlags            // Group Flags attribute
   zLONG     lFlags = 0;
   //:STRING (32) szTag
   zCHAR     szTag[ 33 ] = { 0 };
   //:SHORT       nRC
   zSHORT    nRC = 0;


   //:lFlags = vReportDef.Group.Flags
   GetIntegerFromAttribute( &lFlags, vReportDef, "Group", "Flags" );
   //:IF lFlags = 1  // this is an Overlay Group
   if ( lFlags == 1 )
   {
      //:RETURN 0
      return( 0 );
   }

   //:END

   //://
   //:// char szWorkString[ 256 ];
   //:// char szTag1[ 64 ];
   //:// char szTag2[ 64 ];
   //:// GetStringFromAttribute( szTag1, vReportDef, "GroupSet", "Tag" );
   //:// GetStringFromAttribute( szTag2, vReportDef, "Group", "Tag" );
   //:// if ( zstrcmp( szTag2, "GroupFooter" ) == 0 ||
   //://      zstrcmp( szTag2, "GroupHeader" ) == 0 )
   //:// {
   //://    wsprintf( szWorkString, "FormatGroupOnPage GroupSet: %s   Group: %s",
   //://              szTag1, szTag2 );
   //://    TraceLineS( szWorkString, "" );
   //:// }
   //://
   //:lFlags = lContinuedGroupFlag
   lFlags = lContinuedGroupFlag;
   //:IF lContinuedGroupFlag < 0
   if ( lContinuedGroupFlag < 0 )
   {
      //:TraceLineI( "FormatGroupOnPage ContinuedGroupFlag: ", lContinuedGroupFlag )
      TraceLineI( "FormatGroupOnPage ContinuedGroupFlag: ", lContinuedGroupFlag );
      //:lContinuedGroupFlag = 0
      lContinuedGroupFlag = 0;
   }

   //:END

   //:// CreateViewTrace( "Creating view (20): ", vReportDefTemp, vReportDef, 0 )
   //:CreateViewFromViewForTask( vReportDefTemp, vReportDef, 0 )
   CreateViewFromViewForTask( &vReportDefTemp, vReportDef, 0 );
   //:GetStringFromAttribute( szTag, vReportDef, "Group", "Tag" )
   GetStringFromAttribute( szTag, sizeof( szTag ), vReportDef, "Group", "Tag" );
   //:nRC = SetCursorFirstEntityByString( vReportDefTemp, "Group",
   //:                                    "OverlayTag", szTag, "" )
   nRC = SetCursorFirstEntityByString( vReportDefTemp, "Group", "OverlayTag", szTag, "" );
   //:IF nRC >= zCURSOR_SET
   if ( nRC >= zCURSOR_SET )
   {
      //:lContinuedGroupSave = vReportDef.PageState.ContinuedGroupFlagStartPage
      GetIntegerFromAttribute( &lContinuedGroupSave, vReportDef, "PageState", "ContinuedGroupFlagStartPage" );
      //:lOrigPosY = lCurrentPosY
      lOrigPosY = lCurrentPosY;
      //:lOrigRemainingSpace = lRemainingSpaceOnPageY
      lOrigRemainingSpace = lRemainingSpaceOnPageY;
      //:LOOP WHILE nRC >= zCURSOR_SET
      while ( nRC >= zCURSOR_SET )
      {
         //:fnFormatGroupOnPage( vReportDefTemp, vSourceHierOI, vSubtask,
         //:                     plNewPosX, plNewPosY, lCurrentPosX, lCurrentPosY,
         //:                     lRemainingSpaceOnPageY, lContinuedGroupFlag,
         //:                     lPageWidth, lPageHeight,
         //:                     lOrigRemainingSpaceOnPageY,
         //:                     lFooterPosPageBottom )
         oTZRPSRCO_fnFormatGroupOnPage( vReportDefTemp, vSourceHierOI, vSubtask, plNewPosX, plNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, lContinuedGroupFlag, lPageWidth, lPageHeight, lOrigRemainingSpaceOnPageY,
                        lFooterPosPageBottom );
         //:nRC = SetCursorNextEntityByString( vReportDefTemp, "Group",
         //:                                   "OverlayTag", szTag, "" )
         nRC = SetCursorNextEntityByString( vReportDefTemp, "Group", "OverlayTag", szTag, "" );
      }

      //:END

      //:vReportDef.PageState.ContinuedGroupFlagStartPage = lContinuedGroupSave
      SetAttributeFromInteger( vReportDef, "PageState", "ContinuedGroupFlagStartPage", lContinuedGroupSave );
      //:lCurrentPosY = lOrigPosY
      lCurrentPosY = lOrigPosY;
      //:lRemainingSpaceOnPageY = lOrigRemainingSpace
      lRemainingSpaceOnPageY = lOrigRemainingSpace;
   }

   //:END

   //:// DropViewTrace( "Dropping view (25): ", vReportDefTemp )
   //:DropView( vReportDefTemp )
   DropView( vReportDefTemp );
   //:vReportDefTemp = 0
   vReportDefTemp = 0;

   //:lContinuedGroupFlag = lFlags
   lContinuedGroupFlag = lFlags;
   //:nRC = fnFormatGroupOnPage( vReportDef, vSourceHierOI, vSubtask,
   //:                           plNewPosX, plNewPosY,
   //:                           lCurrentPosX, lCurrentPosY,
   //:                           lRemainingSpaceOnPageY, lContinuedGroupFlag,
   //:                           lPageWidth, lPageHeight,
   //:                           lOrigRemainingSpaceOnPageY,
   //:                           lFooterPosPageBottom )
   nRC = oTZRPSRCO_fnFormatGroupOnPage( vReportDef, vSourceHierOI, vSubtask, plNewPosX, plNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, lContinuedGroupFlag, lPageWidth, lPageHeight, lOrigRemainingSpaceOnPageY, lFooterPosPageBottom );

   //:RETURN nRC
   return( nRC );
// END
}


//:TRANSFORMATION OPERATION
//:FormatSubobjectForDef( VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:                       VIEW    vSubtask,
//:                       VIEW    vSourceOI,
//:                       STRING ( 32 ) szReportDefName,
//:                       STRING ( 32 ) szSubobjectEntityName,
//:                       INTEGER lPrintPreviewFlag,
//:                       INTEGER lPrintDialogFlag,
//:                       INTEGER lPrintFlags ) //  0 - Print to printer.
//:                                             // +1 - Print Preview.
//:                                             // +2 - Print Dialog.
//:                                             // +4 - Force 0, 1, or 2 ... do not
//:                                             //      use OE settings.
//:                                             // +8 - Print multiple entities at
//:                                             //      top level.
//:                                             // +16 - Close subtask when closing
//:                                             //       the print preview window.
//:                                             // +32 - Count the number of pages
//:                                             //       prior to printing
//:                                             // +64 - After printing is complete,
//:                                             //       drop vSourceOI.
//:                                             // +128 - Create CSV file.
//:                                             // +256 - Create HTML file.
//:                                             // +512 - Fit To Page (not implemented)
//:                                             // +1024 - Reset Paging
//:                                             // +2048 - Print PageFooter using PageHeader view
//:                                             // +8192 - Trace OI
//:                                             // +16384 - Cause MessageBox
//:                                             // +32768 - Drive by ReportDef instead of SourceOI

//:   INTEGER bValidView
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_FormatSubobjectForDef( zVIEW     vReportDef,
                                 zVIEW     vSubtask,
                                 zVIEW     vSourceOI,
                                 zPCHAR    szReportDefName,
                                 zPCHAR    szSubobjectEntityName,
                                 zLONG     lPrintPreviewFlag,
                                 zLONG     lPrintDialogFlag,
                                 zLONG     lPrintFlags )
{
   zLONG     bValidView = 0;
   //:INTEGER nRC
   zLONG     nRC = 0;


   //:NAME VIEW vSourceOI  "TZ_SourceOrigOI"
   SetNameForView( vSourceOI, "TZ_SourceOrigOI", 0, zLEVEL_TASK );
   //:NAME VIEW vReportDef "TZ_ReportDef"
   SetNameForView( vReportDef, "TZ_ReportDef", 0, zLEVEL_TASK );
   //:nRC = IsFlagSequenceSet( lPrintFlags, 8192 )  // Trace OI
   nRC = IsFlagSequenceSet( lPrintFlags, 8192 );
   //:IF nRC != 0
   if ( nRC != 0 )
   {
      //:DisplayObjectInstance( vSourceOI, "", "" )
      DisplayObjectInstance( vSourceOI, "", "" );
   }

   //:END

   //:nRC = IsFlagSequenceSet( lPrintFlags, 16384 ) // Cause MessageBox
   nRC = IsFlagSequenceSet( lPrintFlags, 16384 );
   //:IF nRC != 0
   if ( nRC != 0 )
   {
      //:SysMessageBox( vSubtask, "TZ_SourceOrigOI Entity", szSubobjectEntityName, TRUE )
      SysMessageBox( vSubtask, "TZ_SourceOrigOI Entity", szSubobjectEntityName, TRUE );
   }

   //:END

   //:// These values are stored for future reference ... the only place
   //:// they are set.
   //:vReportDef.Report.WK_SubobjectEntityName = szSubobjectEntityName
   SetAttributeFromString( vReportDef, "Report", "WK_SubobjectEntityName", szSubobjectEntityName );
   //:vReportDef.Report.PrintFlags = lPrintFlags
   SetAttributeFromInteger( vReportDef, "Report", "PrintFlags", lPrintFlags );

   //:// bValidView = bValidView / bValidView
   //:// SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View -1", TRUE )

   //:// Call the OpenReport routine which will process all pages for the report,
   //:// calling routine, FormatDocOnPage, to format each page of the report.
   //:// We will determine if we need to force Preview based on a flag in the
   //:// Profile object, set in the Object Browser dialog.  If it is not set on
   //:// or if the Profile object doesn't exist, we will go with the Preview
   //:// flag value passed to this routine.
   //://
   //:// lPrintFlags = SetFlagSequence( lPrintFlags,
   //://                                65535,  // 0x0000ffff  turn off high bits
   //://                                0 )
   //:lPrintFlags = lPrintFlags + lPrintPreviewFlag
   lPrintFlags = lPrintFlags + lPrintPreviewFlag;

   //:// zREPORT_PRINT2CSV             0x00000080   (128)
   //:// zREPORT_PRINT2HTML            0x00000100   (256)
   //:nRC = IsFlagSequenceSet( lPrintFlags, 128 ) // zREPORT_PRINT2CSV
   nRC = IsFlagSequenceSet( lPrintFlags, 128 );
   //:IF nRC != 0
   if ( nRC != 0 )
   {
      //:PrintReportToCSV( vSubtask, vSourceOI, szSubobjectEntityName,
      //:                  szReportDefName, szReportDefName, lPrintFlags )
      PrintReportToCSV( vSubtask, vSourceOI, szSubobjectEntityName, szReportDefName, szReportDefName, lPrintFlags );
      //:ELSE
   }
   else
   {
      //:nRC = IsFlagSequenceSet( lPrintFlags, 256 ) // zREPORT_PRINT2HTML
      nRC = IsFlagSequenceSet( lPrintFlags, 256 );
      //:IF nRC != 0
      if ( nRC != 0 )
      {
         //:PrintReportToHTML( vSubtask, vSourceOI, szSubobjectEntityName,
         //:                   "", szReportDefName, lPrintFlags )
         PrintReportToHTML( vSubtask, vSourceOI, szSubobjectEntityName, "", szReportDefName, lPrintFlags );

         //:ELSE
      }
      else
      {
         //:OpenReport( vSubtask, "KZRPSRCO", "oTZRPSRCO_FormatDocPage",
         //:            lPrintFlags, TRUE, TRUE, lPrintDialogFlag,
         //:            szReportDefName, szSubobjectEntityName )
         OpenReport( vSubtask, "KZRPSRCO", "oTZRPSRCO_FormatDocPage", lPrintFlags, TRUE, TRUE, lPrintDialogFlag, szReportDefName, szSubobjectEntityName );
      }

      //:END
   }

   //:END
   return( 0 );
// END
}


//:LOCAL OPERATION
//:ProcessFooters( VIEW        vReportDef BASED ON LOD TZRPSRCO,
//:                VIEW        vSubtask,
//:                STRING (32) szCurrentEntityName,
//:                INTEGER     lReturnLevel,
//:                INTEGER     lRemainingSpaceOnPageY,
//:                INTEGER     lBottomOfPage,
//:                INTEGER     plCurrentPosX,
//:                INTEGER     plCurrentPosY,
//:                INTEGER     plRepeatHorizontal,
//:                INTEGER     lPageWidth,
//:                INTEGER     lPageHeight,
//:                INTEGER     bForcePageBreak )

//:   VIEW        vFooterDef BASED ON LOD TZRPSRCO
static zSHORT
oTZRPSRCO_ProcessFooters( zVIEW     vReportDef,
                          zVIEW     vSubtask,
                          zPCHAR    szCurrentEntityName,
                          zLONG     lReturnLevel,
                          zLONG     lRemainingSpaceOnPageY,
                          zLONG     lBottomOfPage,
                          zPLONG    plCurrentPosX,
                          zPLONG    plCurrentPosY,
                          zPLONG    plRepeatHorizontal,
                          zLONG     lPageWidth,
                          zLONG     lPageHeight,
                          zLONG     bForcePageBreak )
{
   zVIEW     vFooterDef = 0;
   //:VIEW        vTempDef   BASED ON LOD TZRPSRCO
   zVIEW     vTempDef = 0;
   //:VIEW        vTemp
   zVIEW     vTemp = 0;
   //:VIEW        vFooterHierOI
   zVIEW     vFooterHierOI = 0;
   //:INTEGER     lOrigPosX
   zLONG     lOrigPosX = 0;
   //:INTEGER     lNewPosX
   zLONG     lNewPosX = 0;
   //:INTEGER     lNewPosY
   zLONG     lNewPosY = 0;
   //:INTEGER     lGroupTotal
   zLONG     lGroupTotal = 0;
   //:INTEGER     lTempView
   zLONG     lTempView = 0;
   //:INTEGER     bValidView
   zLONG     bValidView = 0;
   //:INTEGER     nFooterLoop
   zLONG     nFooterLoop = 0;
   //:INTEGER     nLevel
   zLONG     nLevel = 0;
   //:STRING (32) szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 };
   //:SHORT       nRC
   zSHORT    nRC = 0;
   zSHORT    RESULT;


   //:plRepeatHorizontal = 0
   *plRepeatHorizontal = 0;

   //:// *** PROCESS FOOTERS AS NECESSARY ***

   //:// A footer Group is processed for the last instance of an entity under
   //:// its parent if there is a Group of Type "gf".  A problem here is that
   //:// multiple Footers might need to be processed when we position on a new
   //:// entity.  To handle this, an instance of FooterAtLevel was created on
   //:// the first instance of each entity that would later require a Footer.
   //:// They were created in reverse order by Level.  Thus our logic for
   //:// processing a Footer is to see if an instance of FooterAtLevel
   //:// exists for a Level higher than the level of the current entity.

   //:// A return code of -1 indicates that no Footers needed to be processed.
   //:// A return code of 0 indicates that Footers were processed and all would
   //:// fit on page.
   //:// A return code of 1 indicates that not all of the Footers would fit on
   //:// the page and that they will have to be added to the next page.

   //:// See if we need to process a Footer for the last entity Group.
   //:SET CURSOR FIRST vReportDef.FooterAtLevel WHERE
   //:                 vReportDef.FooterAtLevel.Level >= lReturnLevel
   RESULT = SetCursorFirstEntity( vReportDef, "FooterAtLevel", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   {
      while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToInteger( vReportDef, "FooterAtLevel", "Level", lReturnLevel ) < 0 ) )
      {
         RESULT = SetCursorNextEntity( vReportDef, "FooterAtLevel", "" );
      }

   }

   //:// If the current level is the same as the FooterAtLevel, we will check to
   //:// see if the entity names are the same.
   //:nFooterLoop = RESULT
   nFooterLoop = RESULT;
   //:IF nFooterLoop >= zCURSOR_SET
   if ( nFooterLoop >= zCURSOR_SET )
   {
      //:IF vReportDef.FooterAtLevel.Level = lReturnLevel AND
      //:   vReportDef.FooterAtLevel.EntityName = szCurrentEntityName AND
      //:   vReportDef.FooterAtLevel.Complete != "Y"
      if ( CompareAttributeToInteger( vReportDef, "FooterAtLevel", "Level", lReturnLevel ) == 0 && CompareAttributeToString( vReportDef, "FooterAtLevel", "EntityName", szCurrentEntityName ) == 0 &&
           CompareAttributeToString( vReportDef, "FooterAtLevel", "Complete", "Y" ) != 0 )
      {

         //:nFooterLoop = -1
         nFooterLoop = -1;
      }


      //:END
   }

   //:END

   //:lOrigPosX = plCurrentPosX
   lOrigPosX = *plCurrentPosX;

   //:// Now loop to process each Footer.
   //:LOOP WHILE nFooterLoop >= zCURSOR_SET
   while ( nFooterLoop >= zCURSOR_SET )
   {
      //:GetViewFromAttribute( vFooterHierOI, vReportDef,
      //:                      "FooterAtLevel", "SourceOI_Hier" )
      GetViewFromAttribute( &vFooterHierOI, vReportDef, "FooterAtLevel", "SourceOI_Hier" );
      //:bValidView = IsValidView( vFooterHierOI )
      bValidView = IsValidView( vFooterHierOI );
      //:IF bValidView = FALSE
      if ( bValidView == FALSE )
      {
         //:bValidView = bValidView / bValidView
         bValidView = bValidView / bValidView;
      }

      //:END
      //://UNDVV IF bValidView = FALSE
      //://UNDVV    TraceLineV( "(nFooterLoop) PageState/FooterAtLevel invalid view: ",
      //://UNDVV                vFooterHierOI )
      //://UNDVV    DisplayEntityInstance( vReportDef, "PageState" )
      //://UNDVV    DisplayEntityInstance( vReportDef, "FooterAtLevel" )
      //://UNDVV END
      //:// CreateViewTrace( "Creating view (22): ", vFooterDef, vReportDef, 0 )
      //:CreateViewFromViewForTask( vFooterDef, vReportDef, 0 )
      CreateViewFromViewForTask( &vFooterDef, vReportDef, 0 );
      //:szEntityName = vReportDef.FooterAtLevel.EntityName
      GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "FooterAtLevel", "EntityName", "", 0 );
      //:SET CURSOR FIRST vFooterDef.GroupSet WHERE
      //:    vFooterDef.GroupSet.Tag = szEntityName
      RESULT = SetCursorFirstEntityByString( vFooterDef, "GroupSet", "Tag", szEntityName, "" );

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      {
         //:SET CURSOR FIRST vFooterDef.Group WHERE
         //:    vFooterDef.Group.Type = "gf"
         RESULT = SetCursorFirstEntityByString( vFooterDef, "Group", "Type", "gf", "" );
         //:ELSE
      }
      else
      {
         //:TraceLineS( "ProcessFooters unable to locate GroupSet: ",
         //:            szEntityName )
         TraceLineS( "ProcessFooters unable to locate GroupSet: ", szEntityName );
      }

      //:END

      //:IF vReportDef.PageState.ForcePageBreak != 0
      if ( CompareAttributeToInteger( vReportDef, "PageState", "ForcePageBreak", 0 ) != 0 )
      {
         //:nRC = 1
         nRC = 1;
      }

      //:END

      //:// Make sure there is enough space on the Page for the Footer.  If
      //:// not, set up for the next Page.
      //:lRemainingSpaceOnPageY = lBottomOfPage - plCurrentPosY
      lRemainingSpaceOnPageY = lBottomOfPage - *plCurrentPosY;
      //:lGroupTotal = vFooterDef.Group.SZDLG_Y
      GetIntegerFromAttribute( &lGroupTotal, vFooterDef, "Group", "SZDLG_Y" );
      //:IF lRemainingSpaceOnPageY < lGroupTotal
      if ( lRemainingSpaceOnPageY < lGroupTotal )
      {

         //:// Return with an indication that there are more Footers, but there
         //:// was not enough room to process them all.  Then save the last
         //:// ReturnLevel.

         //:vReportDef.FooterAtLevel.Complete = "Y"
         SetAttributeFromString( vReportDef, "FooterAtLevel", "Complete", "Y" );
         //:vReportDef.Report.LastLevel = lReturnLevel
         SetAttributeFromInteger( vReportDef, "Report", "LastLevel", lReturnLevel );
         //:vReportDef.Report.ForcePageBreak = bForcePageBreak
         SetAttributeFromInteger( vReportDef, "Report", "ForcePageBreak", bForcePageBreak );

         //:vTemp = vReportDef.Report.FooterDidNotFit
         GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "Report", "FooterDidNotFit" );
         //:IF vTemp != 0
         if ( vTemp != 0 )
         {
            //:// DropViewTrace( "Dropping view (4f): ", vTemp )
            //:DropView( vTemp )
            DropView( vTemp );
            //:vTemp = 0
            vTemp = 0;
         }

         //:END

         //:// vReportDef.Report.FooterDidNotFit = vReportDef.FooterAtLevel.SourceOI_Hier
         //:GetViewFromAttribute( vFooterHierOI, vReportDef,
         //:                      "FooterAtLevel", "SourceOI_Hier" )
         GetViewFromAttribute( &vFooterHierOI, vReportDef, "FooterAtLevel", "SourceOI_Hier" );

         //:IF vFooterHierOI != 0
         if ( vFooterHierOI != 0 )
         {

            //:// bValidView = IsValidView( vFooterHierOI )
            //:// IF bValidView = FALSE
            //:// // bValidView = bValidView / bValidView
            //://    TraceLineV( "FooterDidNotFit View is Invalid (11): ", vFooterHierOI )
            //:// // SysMessageBox( vSubtask, "Report.FooterDidNotFit", "Invalid View 11", TRUE )
            //:// ELSE
            //://    TraceLineV( "FooterDidNotFit View is Valid (11): ", vFooterHierOI )
            //:// END

            //:// CreateViewTrace( "Creating view (4h): ", vTemp, vFooterHierOI, 0 )
            //:CreateViewFromViewForTask( vTemp, vFooterHierOI, 0 )
            CreateViewFromViewForTask( &vTemp, vFooterHierOI, 0 );
            //:lTempView = MakeIntegerFromView( vTemp )
            lTempView = MakeIntegerFromView( vTemp );

            //:ELSE
         }
         else
         {

            //:lTempView = 0
            lTempView = 0;
         }


         //:END

         //:vReportDef.Report.FooterDidNotFit = lTempView
         SetAttributeFromInteger( vReportDef, "Report", "FooterDidNotFit", lTempView );
         //:// DropViewTrace( "Dropping view (26a): ", vFooterDef )
         //:DropView( vFooterDef )
         DropView( vFooterDef );
         //:vFooterDef = 0
         vFooterDef = 0;

         //:RETURN 1 // return indicating that there are MORE pages
         return( 1 );
      }


      //:END

      //:// Put Footer on Page.
      //:IF bValidView = TRUE
      if ( bValidView == TRUE )
      {

         //:plRepeatHorizontal = vFooterDef.GroupSet.RepeatHorizontal
         GetIntegerFromAttribute( plRepeatHorizontal, vFooterDef, "GroupSet", "RepeatHorizontal" );
         //:lNewPosX = lOrigPosX
         lNewPosX = lOrigPosX;
         //:FormatGroupOnPage( vFooterDef, vFooterHierOI, vSubtask,
         //:                   lNewPosX, lNewPosY, plCurrentPosX, plCurrentPosY,
         //:                   lRemainingSpaceOnPageY,
         //:                   0,   // continued Group flag 0 = not continued
         //:                   lPageWidth, lPageHeight, lRemainingSpaceOnPageY,
         //:                   lBottomOfPage )
         oTZRPSRCO_FormatGroupOnPage( vFooterDef, vFooterHierOI, vSubtask, &lNewPosX, &lNewPosY, *plCurrentPosX, *plCurrentPosY, lRemainingSpaceOnPageY, 0, lPageWidth, lPageHeight, lRemainingSpaceOnPageY, lBottomOfPage );

         //:lNewPosX = 0  // can't have footers repeating horizontally
         lNewPosX = 0;
         //:plCurrentPosY = lNewPosY
         *plCurrentPosY = lNewPosY;
         //:lRemainingSpaceOnPageY = lBottomOfPage - plCurrentPosY
         lRemainingSpaceOnPageY = lBottomOfPage - *plCurrentPosY;
         //:ELSE
      }
      else
      {
         //:TraceLineS( "Footer has been Skipped.....", "" )
         TraceLineS( "Footer has been Skipped.....", "" );
      }

      //:END

      //:// Delete the FooterAtLevel that we processed in this loop.
      //:vTemp = vReportDef.FooterAtLevel.SourceOI_Hier
      GetIntegerFromAttribute( (zLONG *) &vTemp, vReportDef, "FooterAtLevel", "SourceOI_Hier" );
      //:IF vTemp != 0
      if ( vTemp != 0 )
      {
         //:IF vTemp = vFooterHierOI
         if ( vTemp == vFooterHierOI )
         {
            //:vFooterHierOI = 0
            vFooterHierOI = 0;
         }

         //:END

         //:// DropViewTrace( "Dropping view (26b): ", vTemp )
         //:DropView( vTemp )
         DropView( vTemp );
         //:vTemp = 0
         vTemp = 0;
         //:vReportDef.FooterAtLevel.SourceOI_Hier = 0
         SetAttributeFromInteger( vReportDef, "FooterAtLevel", "SourceOI_Hier", 0 );
      }

      //:END

      //:DELETE ENTITY vReportDef.FooterAtLevel NONE
      RESULT = DeleteEntity( vReportDef, "FooterAtLevel", zREPOS_NONE );

      //:// DropViewTrace( "Dropping view (26): ", vFooterDef )
      //:DropView( vFooterDef )
      DropView( vFooterDef );
      //:vFooterDef = 0
      vFooterDef = 0;

      //:// Drop the view to the footer.
      //:// If we are processing a continued footer at the beginning of a page
      //:// unless it exists on a FooterAtLevel for the previous page.
      //:// CreateViewTrace( "Creating view (23): ", vTempDef, vReportDef, 0 )
      //:CreateViewFromViewForTask( vTempDef, vReportDef, 0 )
      CreateViewFromViewForTask( &vTempDef, vReportDef, 0 );
      //:SET CURSOR PREVIOUS vTempDef.PageState
      RESULT = SetCursorPrevEntity( vTempDef, "PageState", "" );
      //:SET CURSOR FIRST vTempDef.FooterAtLevel
      //:    WHERE vTempDef.FooterAtLevel.SourceOI_Hier = lTempView
      RESULT = SetCursorFirstEntityByInteger( vTempDef, "FooterAtLevel", "SourceOI_Hier", lTempView, "" );
      //:IF RESULT < zCURSOR_SET AND vFooterHierOI != 0
      if ( RESULT < zCURSOR_SET && vFooterHierOI != 0 )
      {
         //://UNDVV  bValidView = IsValidView( vFooterHierOI )
         //://UNDVV  IF bValidView = FALSE
         //://UNDVV     TraceLineV( "(nFooterLoop2) PageState/FooterAtLevel invalid view: ",
         //://UNDVV                 vFooterHierOI )
         //://UNDVV     DisplayEntityInstance( vTempDef, "PageState" )
         //://UNDVV     DisplayEntityInstance( vTempDef, "FooterAtLevel" )
         //://UNDVV  ELSE
         //:// DropViewTrace( "Dropping view (27): ", vFooterHierOI )
         //:   DropView( vFooterHierOI )
         DropView( vFooterHierOI );
         //:   vFooterHierOI = 0
         vFooterHierOI = 0;
      }

      //://UNDVV  END
      //:END

      //:// DropViewTrace( "Dropping view (28): ", vTempDef )
      //:DropView( vTempDef )
      DropView( vTempDef );
      //:vTempDef = 0
      vTempDef = 0;

      //:// Position on the next Footer to be processed, using same logic as
      //:// preceeded this loop.
      //:SET CURSOR NEXT vReportDef.FooterAtLevel WHERE
      //:                vReportDef.FooterAtLevel.Level >= lReturnLevel
      RESULT = SetCursorNextEntity( vReportDef, "FooterAtLevel", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      {
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToInteger( vReportDef, "FooterAtLevel", "Level", lReturnLevel ) < 0 ) )
         {
            RESULT = SetCursorNextEntity( vReportDef, "FooterAtLevel", "" );
         }

      }

      //:nFooterLoop = RESULT
      nFooterLoop = RESULT;
      //:// IF nFooterLoop >= zCURSOR_SET       DKS?
      //://
      //://    IF vReportDef.FooterAtLevel.Level = lReturnLevel AND
      //://       vReportDef.FooterAtLevel.EntityName != szCurrentEntityName
      //://
      //://       nFooterLoop = -1
      //://
      //://    END
      //:// END                                 DKS?

      //:IF nFooterLoop >= zCURSOR_SET
      if ( nFooterLoop >= zCURSOR_SET )
      {
         //:nLevel = vReportDef.FooterAtLevel.Level
         GetIntegerFromAttribute( &nLevel, vReportDef, "FooterAtLevel", "Level" );
         //:szEntityName = vReportDef.FooterAtLevel.EntityName
         GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "FooterAtLevel", "EntityName", "", 0 );

         //:IF nLevel < lReturnLevel
         if ( nLevel < lReturnLevel )
         {
            //:nFooterLoop = -1
            nFooterLoop = -1;
            //:ELSE
         }
         else
         {

            //:IF nLevel > lReturnLevel
            if ( nLevel > lReturnLevel )
            {
               //:ELSE
            }
            else
            {

               //:LOOP WHILE nFooterLoop >= zCURSOR_SET AND
               //:           nLevel = lReturnLevel AND
               //:           szEntityName != szCurrentEntityName
               while ( nFooterLoop >= zCURSOR_SET && nLevel == lReturnLevel && ZeidonStringCompare( szEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 ) != 0 )
               {

                  //:SET CURSOR NEXT vReportDef.FooterAtLevel
                  RESULT = SetCursorNextEntity( vReportDef, "FooterAtLevel", "" );
                  //:nFooterLoop = RESULT
                  nFooterLoop = RESULT;
                  //:IF nFooterLoop >= zCURSOR_SET
                  if ( nFooterLoop >= zCURSOR_SET )
                  {
                     //:nLevel = vReportDef.FooterAtLevel.Level
                     GetIntegerFromAttribute( &nLevel, vReportDef, "FooterAtLevel", "Level" );
                     //:szEntityName = vReportDef.FooterAtLevel.EntityName
                     GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "FooterAtLevel", "EntityName", "", 0 );
                     //:IF nLevel < lReturnLevel
                     if ( nLevel < lReturnLevel )
                     {
                        //:nFooterLoop = -1
                        nFooterLoop = -1;
                        //:ELSE
                     }
                     else
                     {
                        //:IF szEntityName = szCurrentEntityName AND
                        //:   vReportDef.FooterAtLevel.Complete != "Y"
                        if ( ZeidonStringCompare( szEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 ) == 0 && CompareAttributeToString( vReportDef, "FooterAtLevel", "Complete", "Y" ) != 0 )
                        {

                           //:nFooterLoop = -1
                           nFooterLoop = -1;
                        }


                        //:END
                     }

                     //:END
                  }

                  //:END
               }

               //:END

               //:IF nFooterLoop >= zCURSOR_SET
               if ( nFooterLoop >= zCURSOR_SET )
               {
                  //:IF nLevel < lReturnLevel OR
                  //:   szEntityName != szCurrentEntityName OR
                  //:   vReportDef.FooterAtLevel.Complete != "Y"
                  if ( nLevel < lReturnLevel || ZeidonStringCompare( szEntityName, 1, 0, szCurrentEntityName, 1, 0, 33 ) != 0 || CompareAttributeToString( vReportDef, "FooterAtLevel", "Complete", "Y" ) != 0 )
                  {

                     //:nFooterLoop = -1
                     nFooterLoop = -1;
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
   }

   //:END

   //:// The normal return code of 0 indicates that there was room on the page
   //:// for all the Footers and that all were processed.
   //:RETURN nRC
   return( nRC );
// END
}


//:LOCAL OPERATION
//:ProcessPageFooter( VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:                   VIEW    vSourceHierOI,
//:                   VIEW    vSubtask,
//:                   INTEGER plNewPosX,
//:                   INTEGER plNewPosY,
//:                   INTEGER lCurrentPosX,
//:                   INTEGER lCurrentPosY,
//:                   INTEGER lPageWidth,
//:                   INTEGER lPageHeight,
//:                   INTEGER lFlags )

//:   VIEW        vPageFooterDef BASED ON LOD TZRPSRCO
static zSHORT
oTZRPSRCO_ProcessPageFooter( zVIEW     vReportDef,
                             zVIEW     vSourceHierOI,
                             zVIEW     vSubtask,
                             zPLONG    plNewPosX,
                             zPLONG    plNewPosY,
                             zLONG     lCurrentPosX,
                             zLONG     lCurrentPosY,
                             zLONG     lPageWidth,
                             zLONG     lPageHeight,
                             zLONG     lFlags )
{
   zVIEW     vPageFooterDef = 0;
   //:VIEW        vTempOI
   zVIEW     vTempOI = 0;
   //:STRING (32) szPageNumber
   zCHAR     szPageNumber[ 33 ] = { 0 };
   //:INTEGER     nGroupSuppressedForPage
   zLONG     nGroupSuppressedForPage = 0;
   //:INTEGER     lPageNbr
   zLONG     lPageNbr = 0;
   //:INTEGER     lResetPageNbr
   zLONG     lResetPageNbr = 0;
   //:INTEGER     lResetTotalPageCnt
   zLONG     lResetTotalPageCnt = 0;
   //:INTEGER     lTotalPages
   zLONG     lTotalPages = 0;
   //:INTEGER     lPrintFlags
   zLONG     lPrintFlags = 0;
   //:INTEGER     lPrintHTML
   zLONG     lPrintHTML = 0;
   //:INTEGER     lTemp
   zLONG     lTemp = 0;
   //:SHORT       nRC
   zSHORT    nRC = 0;
   zSHORT    RESULT;


   //:lPrintFlags = vReportDef.Report.PrintFlags
   GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );
   //:nRC = IsFlagSequenceSet( lPrintFlags, 256 )
   nRC = IsFlagSequenceSet( lPrintFlags, 256 );
   //:IF nRC != 0
   if ( nRC != 0 )
   {
      //:lPrintHTML = 1
      lPrintHTML = 1;
      //:ELSE
   }
   else
   {
      //:lPrintHTML = 0
      lPrintHTML = 0;
   }

   //:END

   //:lPrintFlags = vReportDef.Report.PrintFlags
   GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );
   //:nRC = IsFlagSequenceSet( lPrintFlags, 256 )
   nRC = IsFlagSequenceSet( lPrintFlags, 256 );
   //:nRC = IsFlagSequenceSet( lPrintFlags, 2048 ) // print PageFooter using PageHeader view
   nRC = IsFlagSequenceSet( lPrintFlags, 2048 );
   //:IF nRC != 0
   if ( nRC != 0 )
   {
      //:GET VIEW vTempOI NAMED "-->HeaderViewForFooter"
      RESULT = GetViewByName( &vTempOI, "-->HeaderViewForFooter", vReportDef, zLEVEL_TASK );
      //:IF vTempOI != 0
      if ( vTempOI != 0 )
      {
         //:vSourceHierOI = vTempOI
         vSourceHierOI = vTempOI;
      }

      //:END
   }

   //:END

   //:plNewPosX = 0      // no matter what, the Page Footer starts at zero
   *plNewPosX = 0;
   //:lCurrentPosX = 0
   lCurrentPosX = 0;

   //:// This routine is called at the end of a page to process a Page Footer,
   //:// if there is one.
   //:// CreateViewTrace( "Creating view (24): ", vPageFooterDef, vReportDef, 0 )
   //:CreateViewFromViewForTask( vPageFooterDef, vReportDef, 0 )
   CreateViewFromViewForTask( &vPageFooterDef, vReportDef, 0 );
   //:lPageNbr = vPageFooterDef.Report.CurrentPageNbr
   GetIntegerFromAttribute( &lPageNbr, vPageFooterDef, "Report", "CurrentPageNbr" );
   //:lTotalPages = vPageFooterDef.Report.TotalPageCnt
   GetIntegerFromAttribute( &lTotalPages, vPageFooterDef, "Report", "TotalPageCnt" );
   //:lTemp = 1073741824  // 0x40000000 ==> set up page numbering
   lTemp = 1073741824;
   //:lResetTotalPageCnt = lTotalPages
   lResetTotalPageCnt = lTotalPages;
   //:lResetPageNbr = HandlePageNumbering( vSubtask, szPageNumber, 0,
   //:                                     lPageNbr, lResetTotalPageCnt, lTemp )
   lResetPageNbr = HandlePageNumbering( vSubtask, szPageNumber, 0, lPageNbr, &lResetTotalPageCnt, lTemp );

   //:SET CURSOR FIRST vPageFooterDef.GroupSet WHERE
   //:                 vPageFooterDef.GroupSet.Type = "PF"
   RESULT = SetCursorFirstEntityByString( vPageFooterDef, "GroupSet", "Type", "PF", "" );
   //:LOOP WHILE RESULT >= zCURSOR_SET
   while ( RESULT >= zCURSOR_SET )
   {
      //:// Check if subtype specified for page footer.
      //:nGroupSuppressedForPage = GroupSuppressReason( vPageFooterDef, lResetPageNbr,
      //:                                               lResetTotalPageCnt )
      nGroupSuppressedForPage = oTZRPSRCO_GroupSuppressReason( vPageFooterDef, lResetPageNbr, lResetTotalPageCnt );

      //:// Only process the footer if it is not suppressed.
      //:IF nGroupSuppressedForPage = 0
      if ( nGroupSuppressedForPage == 0 )
      {
         //:SET CURSOR FIRST vPageFooterDef.Control WHERE
         //:                 vPageFooterDef.Control.Tag = "PageNumber"
         RESULT = SetCursorFirstEntityByString( vPageFooterDef, "Control", "Tag", "PageNumber", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         {
            //:vPageFooterDef.Control.Text = szPageNumber
            SetAttributeFromString( vPageFooterDef, "Control", "Text", szPageNumber );
         }

         //:END

         //:FormatGroupOnPage( vPageFooterDef, vSourceHierOI, vSubtask,
         //:                   plNewPosX, plNewPosY,
         //:                   lCurrentPosX, lCurrentPosY, 99999,
         //:                   0,   // continued Group flag 0 ==> not continued
         //:                   lPageWidth, lPageHeight, 99999, lPageHeight )
         oTZRPSRCO_FormatGroupOnPage( vPageFooterDef, vSourceHierOI, vSubtask, plNewPosX, plNewPosY, lCurrentPosX, lCurrentPosY, 99999, 0, lPageWidth, lPageHeight, 99999, lPageHeight );
         //:lCurrentPosY = plNewPosY
         lCurrentPosY = *plNewPosY;
      }

      //:END

      //:SET CURSOR NEXT vPageFooterDef.Group
      RESULT = SetCursorNextEntity( vPageFooterDef, "Group", "" );
   }

   //:END

   //:// DropViewTrace( "Dropping view (29): ", vPageFooterDef )
   //:DropView( vPageFooterDef )
   DropView( vPageFooterDef );
   //:vPageFooterDef = 0
   vPageFooterDef = 0;

   //:IF vTempOI != 0
   if ( vTempOI != 0 )
   {
      //:DropView( vTempOI )
      DropView( vTempOI );
   }

   //:END
   return( 0 );
// END
}


//:LOCAL OPERATION
//:GroupSuppressReason( VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:                     INTEGER lPageNbr,
//:                     INTEGER lTotalPageCnt )

//:   STRING (1) szGroupSubType
static zSHORT
oTZRPSRCO_GroupSuppressReason( zVIEW     vReportDef,
                               zLONG     lPageNbr,
                               zLONG     lTotalPageCnt )
{
   zCHAR     szGroupSubType[ 2 ] = { 0 };
   //:SHORT      nGroupSuppressedForPage
   zSHORT    nGroupSuppressedForPage = 0;
   zSHORT    lTempInteger_0;
   zSHORT    lTempInteger_1;


   //: // Check if subtype specified for page header.
   //:IF vReportDef.Group EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "Group" );
   if ( lTempInteger_0 == 0 )
   {
      //:szGroupSubType = vReportDef.Group.SubType
      GetVariableFromAttribute( szGroupSubType, 0, 'S', 2, vReportDef, "Group", "SubType", "", 0 );
      //:IF lPageNbr > 1 AND szGroupSubType = "1"  // FirstPageOnly
      if ( lPageNbr > 1 && ZeidonStringCompare( szGroupSubType, 1, 0, "1", 1, 0, 2 ) == 0 )
      {
         //:nGroupSuppressedForPage = 1
         nGroupSuppressedForPage = 1;
         //:ELSE
      }
      else
      {
         //:IF lPageNbr < 2 AND szGroupSubType = "2"  // StartingOnPage2
         if ( lPageNbr < 2 && ZeidonStringCompare( szGroupSubType, 1, 0, "2", 1, 0, 2 ) == 0 )
         {
            //:nGroupSuppressedForPage = 2
            nGroupSuppressedForPage = 2;
            //:ELSE
         }
         else
         {
            //:IF szGroupSubType = "3"  // LastPageOnly
            if ( ZeidonStringCompare( szGroupSubType, 1, 0, "3", 1, 0, 2 ) == 0 )
            {
               //:IF lPageNbr = 0 OR           // suppress for all pages when counting
               //:lPageNbr < lTotalPageCnt  // suppress when not on last page
               if ( lPageNbr == 0 || lPageNbr < lTotalPageCnt )
               {
                  //:nGroupSuppressedForPage = 3
                  nGroupSuppressedForPage = 3;
                  //:ELSE
               }
               else
               {
                  //:nGroupSuppressedForPage = 0
                  nGroupSuppressedForPage = 0;
               }

               //:END
               //:// TraceLineS ( "Group subtype of 'LastPageOnly' is no longer ",
               //://              "supported - use a work entity at end of object." )
               //:// nGroupSuppressedForPage = 0
               //:ELSE
            }
            else
            {
               //:nGroupSuppressedForPage = 0
               nGroupSuppressedForPage = 0;
            }

            //:END END END
         }

      }

      //:ELSE
   }
   else
   {
      //:nGroupSuppressedForPage = 9  // No Group in Group set??
      nGroupSuppressedForPage = 9;
   }

   //:END

   //:// Finally, Only print Group if there are any controls to process.
   //:IF nGroupSuppressedForPage = 0
   if ( nGroupSuppressedForPage == 0 )
   {
      //:IF vReportDef.Control DOES NOT EXIST
      lTempInteger_1 = CheckExistenceOfEntity( vReportDef, "Control" );
      if ( lTempInteger_1 != 0 )
      {
         //:nGroupSuppressedForPage = -1
         nGroupSuppressedForPage = -1;
      }

      //:END
   }

   //:END

   //:RETURN nGroupSuppressedForPage
   return( nGroupSuppressedForPage );
// END
}


//:TRANSFORMATION OPERATION
//:fnFormatGroupOnPage( VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:                     VIEW    vSourceHierOI,
//:                     VIEW    vSubtask,
//:                     INTEGER plNewPosX,
//:                     INTEGER plNewPosY,
//:                     INTEGER lCurrentPosX,    // position of Group on page
//:                     INTEGER lCurrentPosY,    // position of Group on page
//:                     INTEGER lRemainingSpaceOnPageY,
//:                     INTEGER lContinuedGroupFlag,
//:                     INTEGER lPageWidth,
//:                     INTEGER lPageHeight,
//:                     INTEGER lOrigRemainingSpaceOnPageY,
//:                     INTEGER lFooterPosPageBottom )

//:   VIEW         vSourceOI
static zSHORT
oTZRPSRCO_fnFormatGroupOnPage( zVIEW     vReportDef,
                               zVIEW     vSourceHierOI,
                               zVIEW     vSubtask,
                               zPLONG    plNewPosX,
                               zPLONG    plNewPosY,
                               zLONG     lCurrentPosX,
                               zLONG     lCurrentPosY,
                               zLONG     lRemainingSpaceOnPageY,
                               zLONG     lContinuedGroupFlag,
                               zLONG     lPageWidth,
                               zLONG     lPageHeight,
                               zLONG     lOrigRemainingSpaceOnPageY,
                               zLONG     lFooterPosPageBottom )
{
   zVIEW     vSourceOI = 0;
   //:VIEW         vReportTmp
   zVIEW     vReportTmp = 0;
   //:VIEW         vMappingOI
   zVIEW     vMappingOI = 0;
   //:VIEW         vSourceOrigOI
   zVIEW     vSourceOrigOI = 0;
   //:INTEGER      lOrigPosY         // original y-position of control on page
   zLONG     lOrigPosY = 0;
   //:INTEGER      lRemainingSpaceOnPageStart
   zLONG     lRemainingSpaceOnPageStart = 0;
   //:INTEGER      lPagePosMaxX      // position of control on page
   zLONG     lPagePosMaxX = 0;
   //:INTEGER      lPagePosX         // position of control on page
   zLONG     lPagePosX = 0;
   //:INTEGER      lPagePosY         // position of control on page
   zLONG     lPagePosY = 0;
   //:INTEGER      lPagePosEndX      // end position of control on page
   zLONG     lPagePosEndX = 0;
   //:INTEGER      lPagePosEndY      // end position of control on page
   zLONG     lPagePosEndY = 0;
   //:INTEGER      lEndPosY          // bottom of multi-line control
   zLONG     lEndPosY = 0;
   //:INTEGER      lNextWord         // begining of next word to print in multi-line text
   zLONG     lNextWord = 0;
   //:INTEGER      lMultiLineAdjustment // Adjustment +/- to posY to deal with multi-line
   zLONG     lMultiLineAdjustment = 0;
   //:INTEGER      lMultiLineIncrement  // Amount multi-line added to Group size
   zLONG     lMultiLineIncrement = 0;
   //:INTEGER      lMaxSizeY   // temp value for max y size for all controls on same line
   zLONG     lMaxSizeY = 0;
   //:INTEGER      lMultiLineSizeY   // temp value for actual size of multi-line
   zLONG     lMultiLineSizeY = 0;
   //:                               // control after print
   //:INTEGER      lAdjust
   zLONG     lAdjust = 0;
   //:INTEGER      lTemp
   zLONG     lTemp = 0;
   //:INTEGER      lStartBottomAdjustment
   zLONG     lStartBottomAdjustment = 0;
   //:INTEGER      lContinuedGroupAdjustment
   zLONG     lContinuedGroupAdjustment = 0;
   //:INTEGER      lRangeBottom
   zLONG     lRangeBottom = 0;
   //:INTEGER      lRemainingSpaceForCurrentLine
   zLONG     lRemainingSpaceForCurrentLine = 0;
   //:INTEGER      lTextAddress // will hold the address of print text in multi-line case
   zLONG     lTextAddress = 0;
   //:INTEGER      lCtrlBOI
   zLONG     lCtrlBOI = 0;
   //:STRING (32766) szWorkString
   zCHAR     szWorkString[ 32767 ] = { 0 };
   //:STRING (256) szViewName
   zCHAR     szViewName[ 257 ] = { 0 };
   //:STRING (32)  szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 };
   //:STRING (32)  szAttributeName
   zCHAR     szAttributeName[ 33 ] = { 0 };
   //:STRING (32)  szContextName
   zCHAR     szContextName[ 33 ] = { 0 };
   //:STRING (32)  szFaceName      // LF_FACENAME is 32
   zCHAR     szFaceName[ 33 ] = { 0 };
   //:STRING (32)  szCtrlTag
   zCHAR     szCtrlTag[ 33 ] = { 0 };
   //:STRING (10)  szQueryOperation
   zCHAR     szQueryOperation[ 11 ] = { 0 };
   //:STRING (256) szClass
   zCHAR     szClass[ 257 ] = { 0 };
   //:STRING (32)  szYN
   zCHAR     szYN[ 33 ] = { 0 };
   //:SHORT        nRectangleFlag
   zSHORT    nRectangleFlag = 0;
   //:SHORT        nRoundRectFlag
   zSHORT    nRoundRectFlag = 0;
   //:SHORT        nEllipseFlag
   zSHORT    nEllipseFlag = 0;
   //:SHORT        nDiamondFlag
   zSHORT    nDiamondFlag = 0;
   //:SHORT        nHorizontalFlag
   zSHORT    nHorizontalFlag = 0;
   //:SHORT        nVerticalFlag
   zSHORT    nVerticalFlag = 0;
   //:SHORT        nDiagDescendFlag
   zSHORT    nDiagDescendFlag = 0;
   //:SHORT        nDiagAscendFlag
   zSHORT    nDiagAscendFlag = 0;
   //:SHORT        nMultiLineFlag
   zSHORT    nMultiLineFlag = 0;
   //:SHORT        nBoldFlag
   zSHORT    nBoldFlag = 0;
   //:SHORT        nItalicFlag
   zSHORT    nItalicFlag = 0;
   //:SHORT        nStrikeoutFlag
   zSHORT    nStrikeoutFlag = 0;
   //:SHORT        nUnderlineFlag
   zSHORT    nUnderlineFlag = 0;
   //:STRING (2)   szOverrideHide
   zCHAR     szOverrideHide[ 3 ] = { 0 };
   //:SHORT        nHide
   zSHORT    nHide = 0;
   //:INTEGER      lJustify
   zLONG     lJustify = 0;
   //:INTEGER      lWork
   zLONG     lWork = 0;
   //:INTEGER      lFontSize
   zLONG     lFontSize = 0;
   //:INTEGER      lDefaultFontSize
   zLONG     lDefaultFontSize = 0;
   //:INTEGER      lTextColor
   zLONG     lTextColor = 0;
   //:INTEGER      lTextBkColor
   zLONG     lTextBkColor = 0;
   //:INTEGER      lBorderStyle
   zLONG     lBorderStyle = 0;
   //:INTEGER      lBorderColor
   zLONG     lBorderColor = 0;
   //:INTEGER      lEscapement
   zLONG     lEscapement = 0;
   //:INTEGER      lType
   zLONG     lType = 0;
   //:INTEGER      lPenWidth
   zLONG     lPenWidth = 0;
   //:INTEGER      lPenType
   zLONG     lPenType = 0;
   //:INTEGER      lPenColor
   zLONG     lPenColor = 0;
   //:INTEGER      lRoundValue
   zLONG     lRoundValue = 0;
   //:INTEGER      lBackgroundColor
   zLONG     lBackgroundColor = 0;
   //:INTEGER      lFlags            // Shape Flags attribute
   zLONG     lFlags = 0;
   //:INTEGER      lPrintFlags
   zLONG     lPrintFlags = 0;
   //:INTEGER      lPrintHTML
   zLONG     lPrintHTML = 0;
   //:INTEGER      lPrintCSV
   zLONG     lPrintCSV = 0;
   //:INTEGER      lColumnCSV
   zLONG     lColumnCSV = 0;
   //:SHORT        bTopOfPage
   zSHORT    bTopOfPage = 0;
   //:SHORT        bCurrentGroupCanContinue
   zSHORT    bCurrentGroupCanContinue = 0;
   //:SHORT        bProcessedMultiLine
   zSHORT    bProcessedMultiLine = 0;
   //:SHORT        nSkipFlag
   zSHORT    nSkipFlag = 0;
   //:SHORT        nLoop1
   zSHORT    nLoop1 = 0;

   //:SHORT        nRptExitEvent
   zSHORT    nRptExitEvent = 0;
   //:SHORT        nRptExitReturn
   zSHORT    nRptExitReturn = 0;
   //:STRING (32)  szRptExitOper
   zCHAR     szRptExitOper[ 33 ] = { 0 };

   //:INTEGER      lLastPage
   zLONG     lLastPage = 0;
   //:INTEGER      lPageNbr
   zLONG     lPageNbr = 0;
   //:INTEGER      lTotalPageCnt
   zLONG     lTotalPageCnt = 0;
   //:SHORT        nRC
   zSHORT    nRC = 0;

   //://UNDEF INTEGER lTempView
   //:STRING (32) szRptGroupSetTag
   zCHAR     szRptGroupSetTag[ 33 ] = { 0 };
   //:STRING (32) szRptGroupTag
   zCHAR     szRptGroupTag[ 33 ] = { 0 };
   zSHORT    lTempInteger_0;
   zLONG     lTempInteger_1;
   zSHORT    lTempInteger_2;
   zSHORT    lTempInteger_3;
   zLONG     lTempInteger_4;
   zLONG     lTempInteger_5;
   zLONG     lTempInteger_6;
   zSHORT    RESULT;
   zSHORT    lTempInteger_7;
   zSHORT    lTempInteger_8;
   zLONG     lTempInteger_9;
   zLONG     lTempInteger_10;
   zLONG     lTempInteger_11;
   zSHORT    lTempInteger_12;
   zLONG     lTempInteger_13;
   zCHAR     szTempString_0[ 33 ];
   zCHAR     szTempString_1[ 33 ];
   zCHAR     szTempString_2[ 33 ];
   zLONG     lTempInteger_14;
   zSHORT    lTempInteger_15;
   zSHORT    lTempInteger_16;
   zSHORT    lTempInteger_17;
   zLONG     lTempInteger_18;
   zLONG     lTempInteger_19;
   zSHORT    lTempInteger_20;
   zLONG     lTempInteger_21;

   //:szRptGroupSetTag = vReportDef.GroupSet.Tag
   GetVariableFromAttribute( szRptGroupSetTag, 0, 'S', 33, vReportDef, "GroupSet", "Tag", "", 0 );
   //:szRptGroupTag = vReportDef.Group.Tag
   GetVariableFromAttribute( szRptGroupTag, 0, 'S', 33, vReportDef, "Group", "Tag", "", 0 );
   //://UNDEF
   //://UNDEF TraceLineI( "Formatting GroupSet/Group RemainingSpace: ",
   //://UNDEF             lRemainingSpaceOnPageY )
   //://UNDEF TraceLineS( "GroupSet: ", szRptGroupSetTag )
   //://UNDEF TraceLineS( "   Group: ", szRptGroupTag )


   //:szWorkString = vReportDef.Report.Tag
   GetVariableFromAttribute( szWorkString, 0, 'S', 32767, vReportDef, "Report", "Tag", "", 0 );
   //:szWorkString = szWorkString + "PrintDraftWithoutForm"
   ZeidonStringConcat( szWorkString, 1, 0, "PrintDraftWithoutForm", 1, 0, 32767 );
   //:SysReadZeidonIni( -1, "[Workstation]", szWorkString, szOverrideHide )
   SysReadZeidonIni( -1, "[Workstation]", szWorkString, szOverrideHide, sizeof( szOverrideHide ) );

   //:lPrintFlags = vReportDef.Report.PrintFlags
   GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );
   //:nRC = IsFlagSequenceSet( lPrintFlags, 256 )
   nRC = IsFlagSequenceSet( lPrintFlags, 256 );
   //:IF nRC = 0
   if ( nRC == 0 )
   {
      //:lPrintHTML = 0
      lPrintHTML = 0;
      //:nRC = IsFlagSequenceSet( lPrintFlags, 128 )
      nRC = IsFlagSequenceSet( lPrintFlags, 128 );
      //:IF nRC = 0
      if ( nRC == 0 )
      {
         //:lPrintCSV = 0
         lPrintCSV = 0;
         //:ELSE
      }
      else
      {
         //:lPrintCSV = 1
         lPrintCSV = 1;
         //:IF vReportDef.Control EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "Control" );
         if ( lTempInteger_0 == 0 )
         {
            //:lRangeBottom = vReportDef.Control.RangeBottom
            GetIntegerFromAttribute( &lRangeBottom, vReportDef, "Control", "RangeBottom" );
         }

         //:END
      }

      //:END
      //:ELSE
   }
   else
   {
      //:lPrintHTML = 1
      lPrintHTML = 1;
      //:lPrintCSV = 0
      lPrintCSV = 0;
   }

   //:END

   //:lPageNbr = vReportDef.Report.CurrentPageNbr
   GetIntegerFromAttribute( &lPageNbr, vReportDef, "Report", "CurrentPageNbr" );
   //:lTotalPageCnt = vReportDef.Report.TotalPageCnt
   GetIntegerFromAttribute( &lTotalPageCnt, vReportDef, "Report", "TotalPageCnt" );

   //:// TraceLineI( "Footer PageNumber: ", lPageNbr )
   //:// TraceLineI( "Footer TotalPages: ", lTotalPageCnt )
   //:IF (lTotalPageCnt > 0 AND lPageNbr = lTotalPageCnt) OR
   //:   lPrintHTML != 0 OR lPrintCSV != 0
   if ( ( lTotalPageCnt > 0 && lPageNbr == lTotalPageCnt ) || lPrintHTML != 0 || lPrintCSV != 0 )
   {

      //:lLastPage = 1
      lLastPage = 1;
      //:ELSE
   }
   else
   {
      //:lLastPage = 0
      lLastPage = 0;
   }

   //:END

   //:lPagePosMaxX = 0
   lPagePosMaxX = 0;
   //:plNewPosX = lCurrentPosX
   *plNewPosX = lCurrentPosX;
   //:lMultiLineIncrement = 0
   lMultiLineIncrement = 0;
   //:lOrigPosY = lCurrentPosY
   lOrigPosY = lCurrentPosY;
   //:lRemainingSpaceOnPageStart = lRemainingSpaceOnPageY
   lRemainingSpaceOnPageStart = lRemainingSpaceOnPageY;
   //:lRangeBottom = 1
   lRangeBottom = 1;
   //:lStartBottomAdjustment = 0
   lStartBottomAdjustment = 0;
   //:lContinuedGroupAdjustment = 0
   lContinuedGroupAdjustment = 0;
   //:bProcessedMultiLine = 0
   bProcessedMultiLine = 0;
   //:IF lRemainingSpaceOnPageY = lOrigRemainingSpaceOnPageY
   if ( lRemainingSpaceOnPageY == lOrigRemainingSpaceOnPageY )
   {
      //:bTopOfPage = TRUE
      bTopOfPage = TRUE;
      //:ELSE
   }
   else
   {
      //:bTopOfPage = FALSE
      bTopOfPage = FALSE;
   }

   //:END
   //:IF vReportDef.GroupSet.Type = "E" AND    // entity GroupSet
   //:   vReportDef.Group.Type = "ga"   // entity Group detail
   if ( CompareAttributeToString( vReportDef, "GroupSet", "Type", "E" ) == 0 && CompareAttributeToString( vReportDef, "Group", "Type", "ga" ) == 0 )
   {

      //:bCurrentGroupCanContinue = TRUE
      bCurrentGroupCanContinue = TRUE;
      //:ELSE
   }
   else
   {
      //:bCurrentGroupCanContinue = FALSE
      bCurrentGroupCanContinue = FALSE;
   }

   //:END

   //:// Position of controls as stored in the Report Def are relative
   //:// to the beginning of Group.  The actual position of the Group on
   //:// on the page (i.e. relative to the beginning of the page rather than
   //:// the beginning of the Group set) is passed in by lCurrentPosX and
   //:// lCurrentPosY.
   //://
   //:// Both of the print routines used below require positions on the
   //:// actual page.  For that reason, we calculate the positions of the
   //:// controls on the page in lPagePosX and lPagePosY.
   //://
   //:// One problem here is dealing with multi-line text controls.
   //:// With multi-line text controls, the size in the Y direction
   //:// cannot be determined ahead of time.  The size is determined
   //:// by DrawMultiLineTextToReport and is returned as a parameter.
   //://
   //:// To compensate for this ambiguity, an adjustment factor is
   //:// calculated whenever we print multi-line text.  This factor
   //:// is added to all control Y positions and may be negative
   //:// if the amount of space used is actually less than the allotted
   //:// size recorded in the report def.
   //:lMultiLineAdjustment = 0
   lMultiLineAdjustment = 0;
   //:lRemainingSpaceForCurrentLine = 0
   lRemainingSpaceForCurrentLine = 0;

   //:// Get the default font size.
   //:lDefaultFontSize = vReportDef.Report.ReportTextSize
   GetIntegerFromAttribute( &lDefaultFontSize, vReportDef, "Report", "ReportTextSize" );
   //:szWorkString = "ReportFont"
   ZeidonStringCopy( szWorkString, 1, 0, "ReportFont", 1, 0, 32767 );
   //:nRC = GetWorkstationApplicationValues( vSubtask, szWorkString,
   //:                                       szFaceName, 32, lFontSize,
   //:                                       lWork, lWork, lWork, lWork,
   //:                                       lWork, lWork, lWork, lWork, lWork )
   nRC = GetWorkstationApplicationValues( vSubtask, szWorkString, szFaceName, 32, &lFontSize, &lWork, &lWork, &lWork, &lWork, &lWork, &lWork, &lWork, &lWork, &lWork );
   //:IF lDefaultFontSize = 0
   if ( lDefaultFontSize == 0 )
   {
      //:lDefaultFontSize = lFontSize
      lDefaultFontSize = lFontSize;
   }

   //:END

   //:// We will use a temporary view for vSourceHierOI because the operation,
   //:// GetStringFromAttributeByContext calls operations that will not allow
   //:// a hierarchical cursor.
   //:IF vSourceHierOI != 0
   if ( vSourceHierOI != 0 )
   {
      //:// CreateViewTrace( "Creating view (18): ", vSourceOI, vSourceHierOI, 0 )
      //:CreateViewFromViewForTask( vSourceOI, vSourceHierOI, 0 )
      CreateViewFromViewForTask( &vSourceOI, vSourceHierOI, 0 );
      //:NAME VIEW vSourceOI "-->vSourceI2"
      SetNameForView( vSourceOI, "-->vSourceI2", 0, zLEVEL_TASK );
      //:ELSE
   }
   else
   {
      //:vSourceOI = 0
      vSourceOI = 0;
   }

   //:END

   //:// Loop through the Text controls and format each on the Report page.
   //:// If we are formatting a continued Group (from the previous page),
   //:// we will assume that the views to vReportDef and vSourceHierOI still
   //:// point to the correct entities.  If we are formatting a new Group,
   //:// we will start with the first control for the Group.

   //:IF lContinuedGroupFlag > 0
   if ( lContinuedGroupFlag > 0 )
   {
      //:// Subtract the position of the first continued control to
      //:// adjust for starting in the middle of a Group. recall that
      //:// control positions are relative to the beginning of the Group
      //:// which here is passed in as lCurrentPosY.
      //:lMultiLineAdjustment = lMultiLineAdjustment - vReportDef.Control.PSDLG_Y
      GetIntegerFromAttribute( &lTempInteger_1, vReportDef, "Control", "PSDLG_Y" );
      lMultiLineAdjustment = lMultiLineAdjustment - lTempInteger_1;
      //:lContinuedGroupFlag = vReportDef.PageState.ContinuedGroupFlagStartPage
      GetIntegerFromAttribute( &lContinuedGroupFlag, vReportDef, "PageState", "ContinuedGroupFlagStartPage" );
      //:IF lContinuedGroupFlag = 1
      if ( lContinuedGroupFlag == 1 )
      {
         //:lStartBottomAdjustment = vReportDef.Control.RangeBottom
         GetIntegerFromAttribute( &lStartBottomAdjustment, vReportDef, "Control", "RangeBottom" );
         //:ELSE
      }
      else
      {
         //:lStartBottomAdjustment = vReportDef.Control.RangeTop
         GetIntegerFromAttribute( &lStartBottomAdjustment, vReportDef, "Control", "RangeTop" );
      }

      //:   // NOTE: above eliminates the space between ranges after a page break
      //:   //       we may need to also set the MultilineINCREMENT to be negative
      //:   //       by the amount of this space to ensure proper control position.
      //:END

      //:bProcessedMultiLine = 1
      bProcessedMultiLine = 1;
      //:// need to position the ReportDef to the continued control
      //:// since group header / footer processing could have moved it.
      //:// nLoop1 = zCURSOR_SET
      //:szWorkString = vReportDef.PageState.ContinuedControlTagStartPage
      GetVariableFromAttribute( szWorkString, 0, 'S', 32767, vReportDef, "PageState", "ContinuedControlTagStartPage", "", 0 );
      //:nLoop1 = SetCursorFirstEntityByString( vReportDef, "Control", "Tag",
      //:                                       szWorkString, "" )
      nLoop1 = SetCursorFirstEntityByString( vReportDef, "Control", "Tag", szWorkString, "" );
      //:IF nLoop1 < zCURSOR_SET
      if ( nLoop1 < zCURSOR_SET )
      {
         //:TraceLineS( "Report Internal Error: Continuation control not found.", "" )
         TraceLineS( "Report Internal Error: Continuation control not found.", "" );
         //:TraceLineS( "     GroupSet: ", szRptGroupSetTag )
         TraceLineS( "     GroupSet: ", szRptGroupSetTag );
         //:TraceLineS( "        Group: ", szRptGroupTag )
         TraceLineS( "        Group: ", szRptGroupTag );
         //:TraceLineS( "      Control: ", szWorkString )
         TraceLineS( "      Control: ", szWorkString );
      }

      //://UNDEF ELSE
      //://UNDEF  TraceLineS( "Continuation control set to : ", szWorkString )
      //://UNDEF  TraceLineS( "                    GroupSet: ", szRptGroupSetTag )
      //://UNDEF  TraceLineS( "                       Group: ", szRptGroupTag )
      //:END
      //:nRptExitEvent = zRGE_EVENT_GROUP_CONTINUE
      nRptExitEvent = zRGE_EVENT_GROUP_CONTINUE;
      //:ELSE
   }
   else
   {
      //:nLoop1 = SetCursorFirstEntity( vReportDef, "Control", "" )
      nLoop1 = SetCursorFirstEntity( vReportDef, "Control", "" );
      //:nRptExitEvent = zRGE_EVENT_PREPRINT
      nRptExitEvent = zRGE_EVENT_PREPRINT;
      //:fnSetGroupCtrlStatus( vReportDef,
      //:                      zRCONTROL_STATUS_NOTPRINTED )  //not yet printed
      oTZRPSRCO_fnSetGroupCtrlStatus( vReportDef, zRCONTROL_STATUS_NOTPRINTED );
   }

   //:END

   //:szRptExitOper = vReportDef.Group.wOperName
   GetVariableFromAttribute( szRptExitOper, 0, 'S', 33, vReportDef, "Group", "wOperName", "", 0 );
   //:nRptExitReturn = InvokeReportExitCode( vSubtask, vReportDef,
   //:                                       szRptExitOper, nRptExitEvent,
   //:                                       lPageNbr, vSourceOI )
   nRptExitReturn = InvokeReportExitCode( vSubtask, vReportDef, szRptExitOper, nRptExitEvent, lPageNbr, vSourceOI );
   //:IF nRptExitReturn = zRGE_RETURN_SUPPRESS
   if ( nRptExitReturn == zRGE_RETURN_SUPPRESS )
   {
      //://UNDEF TraceLineI( "Exit Suppressed Formatting GroupSet/Group at PosY: ",
      //://UNDEF             lCurrentPosY )
      //://UNDEF TraceLineS( "     GroupSet: ", szRptGroupSetTag )
      //://UNDEF TraceLineS( "        Group: ", szRptGroupTag )
      //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
      //://UNDEF    TraceLineS( "FormatGroupOnPage returning 0", "" )
      //://UNDEF END

      //:plNewPosY = lCurrentPosY
      *plNewPosY = lCurrentPosY;
      //:IF vSourceOI != 0
      if ( vSourceOI != 0 )
      {
         //:// DropViewTrace( "Dropping view (17): ", vSourceOI )
         //:DropView( vSourceOI )
         DropView( vSourceOI );
         //:vSourceOI = 0
         vSourceOI = 0;
      }

      //:END

      //:RETURN 0
      return( 0 );
   }

   //:END
   //:IF nRptExitReturn = zRGE_RETURN_NEWPAGE
   if ( nRptExitReturn == zRGE_RETURN_NEWPAGE )
   {
      //:IF nRptExitEvent = zRGE_EVENT_GROUP_CONTINUE
      if ( nRptExitEvent == zRGE_EVENT_GROUP_CONTINUE )
      {
         //:TraceLineS( "zRGE_RETURN_NEWPAGE return from Exit ignored for ",
         //:            "zRGE_EVENT_GROUP_CONTINUE" )
         TraceLineS( "zRGE_RETURN_NEWPAGE return from Exit ignored for ", "zRGE_EVENT_GROUP_CONTINUE" );
         //:ELSE
      }
      else
      {
         //://UNDEF TraceLineI( "Exit Forced New Page GroupSet/Group at PosY: ",
         //://UNDEF             lCurrentPosY )
         //://UNDEF TraceLineS( "     GroupSet: ", szRptGroupSetTag )
         //://UNDEF TraceLineS( "        Group: ", szRptGroupTag )
         //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
         //://UNDEF    TraceLineS( "FormatGroupOnPage returning -1", "" )
         //://UNDEF END

         //:vReportDef.PageState.MultiLineTextPosEndPage = 0
         SetAttributeFromInteger( vReportDef, "PageState", "MultiLineTextPosEndPage", 0 );
         //:vReportDef.PageState.ContinuedGroupFlagEndPage = 2
         SetAttributeFromInteger( vReportDef, "PageState", "ContinuedGroupFlagEndPage", 2 );

         //:lRemainingSpaceOnPageY = 0
         lRemainingSpaceOnPageY = 0;
         //:plNewPosY = lFooterPosPageBottom
         *plNewPosY = lFooterPosPageBottom;

         //:IF vSourceOI != 0
         if ( vSourceOI != 0 )
         {
            //:// DropViewTrace( "Dropping view (18): ", vSourceOI )
            //:DropView( vSourceOI )
            DropView( vSourceOI );
            //:vSourceOI = 0
            vSourceOI = 0;
         }

         //:END

         //:RETURN -1
         return( -1 );
      }

      //:END
   }

   //:END

   //:// We will adjust lRemainingSpaceOnPageY by lRemainingSpaceAdjustment when
   //:// we have finished the loop.  lRemainingSpaceAdjustment is set to the
   //:// size of the current Group plus any multi-line increments.

   //:// Before we start the loop, we adjust lRemainingSpaceOnPageY by
   //:// the y pos of the 1st control. We assume here that this routine
   //:// will not get called unless there was room to print something and
   //:// thus, lRemainingSpaceOnPageY will get reset in some fashion anyway.
   //://xlRemainingSpaceOnPageY = lRemainingSpaceOnPageY - vReportDef.Control.PSDLG_Y

   //:// nRC = CreateViewTrace( "Creating view (19): ", vReportTmp, vReportDef, 0 )
   //:nRC = CreateViewFromViewForTask( vReportTmp, vReportDef, 0 )
   nRC = CreateViewFromViewForTask( &vReportTmp, vReportDef, 0 );
   //:LOOP WHILE nLoop1 = zCURSOR_SET
   while ( nLoop1 == zCURSOR_SET )
   {

      //:lWork = vReportDef.Control.Subtype
      GetIntegerFromAttribute( &lWork, vReportDef, "Control", "Subtype" );
      //:IF szOverrideHide = "Y"
      if ( ZeidonStringCompare( szOverrideHide, 1, 0, "Y", 1, 0, 3 ) == 0 )
      {
         //:nHide = 0
         nHide = 0;
         //:ELSE
      }
      else
      {
         //:nHide = IsFlagSequenceSet( lWork, 4096 )  // 0x00001000
         nHide = IsFlagSequenceSet( lWork, 4096 );
      }

      //:END

      //:IF lPrintCSV = 1
      if ( lPrintCSV == 1 )
      {

         //:lWork = vReportDef.Control.RangeBottom
         GetIntegerFromAttribute( &lWork, vReportDef, "Control", "RangeBottom" );
         //:IF lWork != lRangeBottom
         if ( lWork != lRangeBottom )
         {
            //:PrintTextToCSV( vSubtask, 0, 0 )
            PrintTextToCSV( vSubtask, 0, 0 );
         }

         //:END

         //:lRangeBottom = lWork
         lRangeBottom = lWork;

         //:lColumnCSV = vReportDef.Control.ColumnCSV
         GetIntegerFromAttribute( &lColumnCSV, vReportDef, "Control", "ColumnCSV" );
         //:IF lColumnCSV > 0
         if ( lColumnCSV > 0 )
         {

            //:IF vReportDef.CtrlMapView EXISTS
            lTempInteger_2 = CheckExistenceOfEntity( vReportDef, "CtrlMapView" );
            if ( lTempInteger_2 == 0 )
            {

               //:// Get text value to put on report AND GET AN ADDRESS TO IT.
               //:// If there is mapping on the report (determined by the existence
               //:// of the CtrlMapView entity), use it to get the text value.
               //:// Get text value to put on report AND STORE IT IN szWorkString.
               //:szEntityName = vReportDef.CtrlMapRelatedEntity.Name
               GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "CtrlMapRelatedEntity", "Name", "", 0 );
               //:szAttributeName = vReportDef.CtrlMapER_Attribute.Name
               GetVariableFromAttribute( szAttributeName, 0, 'S', 33, vReportDef, "CtrlMapER_Attribute", "Name", "", 0 );
               //:IF vReportDef.CtrlMapContext EXISTS
               lTempInteger_3 = CheckExistenceOfEntity( vReportDef, "CtrlMapContext" );
               if ( lTempInteger_3 == 0 )
               {
                  //:szContextName = vReportDef.CtrlMapContext.Name
                  GetVariableFromAttribute( szContextName, 0, 'S', 33, vReportDef, "CtrlMapContext", "Name", "", 0 );
                  //:ELSE
               }
               else
               {
                  //:szContextName = ""
                  ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
               }

               //:END

               //:// For mapping, we can have either vSourceOI, which is for
               //:// the hier view driving the report, or any other mapping
               //:// view.  We will determine this by getting the mapping view
               //:// for the control and comparing it to the original hier view.
               //:szViewName = vReportDef.CtrlMapView.Name
               GetVariableFromAttribute( szViewName, 0, 'S', 257, vReportDef, "CtrlMapView", "Name", "", 0 );
               //:nRC = GetViewByName( vMappingOI, szViewName,
               //:                  vSubtask, zLEVEL_ANY )
               nRC = GetViewByName( &vMappingOI, szViewName, vSubtask, zLEVEL_ANY );
               //:IF nRC >= 0
               if ( nRC >= 0 )
               {
                  //:// GET VIEW vSourceOrigOI NAMED "TZ_SourceOrigOI"
                  //:szViewName = "TZ_SourceOrigOI"
                  ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceOrigOI", 1, 0, 257 );
                  //:nRC = GetViewByName( vSourceOrigOI, szViewName,
                  //:                  vSubtask, zLEVEL_ANY )
                  nRC = GetViewByName( &vSourceOrigOI, szViewName, vSubtask, zLEVEL_ANY );
                  //:IF vMappingOI = vSourceOrigOI
                  if ( vMappingOI == vSourceOrigOI )
                  {
                     //:vMappingOI = vSourceOI
                     vMappingOI = vSourceOI;
                  }

                  //:END
                  //:ELSE
               }
               else
               {
                  //:szWorkString = "Report cannot get view by name for View.Entity.Attribute: " +
                  //:                        szViewName + "." + szEntityName + "." + szAttributeName
                  ZeidonStringCopy( szWorkString, 1, 0, "Report cannot get view by name for View.Entity.Attribute: ", 1, 0, 32767 );
                  ZeidonStringConcat( szWorkString, 1, 0, szViewName, 1, 0, 32767 );
                  ZeidonStringConcat( szWorkString, 1, 0, ".", 1, 0, 32767 );
                  ZeidonStringConcat( szWorkString, 1, 0, szEntityName, 1, 0, 32767 );
                  ZeidonStringConcat( szWorkString, 1, 0, ".", 1, 0, 32767 );
                  ZeidonStringConcat( szWorkString, 1, 0, szAttributeName, 1, 0, 32767 );
                  //:TraceLineS( szWorkString, "" )
                  TraceLineS( szWorkString, "" );
               }

               //:// szWorkString = ""
               //:END

               //:IF vReportDef.Control.wQueryOperation != ""
               if ( CompareAttributeToString( vReportDef, "Control", "wQueryOperation", "" ) != 0 )
               {
                  //:// Query Operation
                  //:szQueryOperation = vReportDef.Control.wQueryOperation
                  GetVariableFromAttribute( szQueryOperation, 0, 'S', 11, vReportDef, "Control", "wQueryOperation", "", 0 );

                  //:// The following code creates the pointer, lTextAddress, as
                  //:// explained below.
                  //:lTextAddress = MakeIntegerFromPointer( szWorkString )
                  lTextAddress = MakeIntegerFromPointer( szWorkString );
                  //:ComputeQueryOperationValue( vReportDef, vMappingOI, szWorkString,
                  //:                         szQueryOperation, szEntityName,
                  //:                         szAttributeName )
                  ComputeQueryOperationValue( vReportDef, vMappingOI, szWorkString, szQueryOperation, szEntityName, szAttributeName );
                  //:ELSE
               }
               else
               {
                  //:// Regular Mapping
                  //:IF vMappingOI = 0
                  if ( vMappingOI == 0 )
                  {
                     //:nRC = 1
                     nRC = 1;
                     //:ELSE
                  }
                  else
                  {
                     //:nRC = CheckExistenceOfEntity( vMappingOI, szEntityName )
                     nRC = CheckExistenceOfEntity( vMappingOI, szEntityName );
                  }

                  //:END

                  //:IF nRC = 0
                  if ( nRC == 0 )
                  {
                     //:GetAttributeDisplayLength( lWork, vMappingOI,
                     //:                        szEntityName, szAttributeName, "" )
                     GetAttributeDisplayLength( (zPULONG) &lWork, vMappingOI, szEntityName, szAttributeName, "" );
                     //:IF lWork > 32764
                     if ( lWork > 32764 )
                     {
                        //:GetAddrForAttribute( lTextAddress, vMappingOI,
                        //:                  szEntityName, szAttributeName )
                        GetAddrForAttribute( &lTextAddress, vMappingOI, szEntityName, szAttributeName );
                        //:ELSE
                     }
                     else
                     {
                        //:// The following code creates the pointer, lTextAddress,
                        //:// to szWorkString, because lTextAddress is the variable
                        //:// used later in a call.
                        //:lTextAddress = MakeIntegerFromPointer( szWorkString )
                        lTextAddress = MakeIntegerFromPointer( szWorkString );
                        //:GetStringFromAttributeByContext( szWorkString, vMappingOI,
                        //:                              szEntityName,
                        //:                              szAttributeName,
                        //:                              "", 32764 )
                        GetStringFromAttributeByContext( szWorkString, vMappingOI, szEntityName, szAttributeName, "", 32764 );
                     }

                     //:END
                     //:ELSE
                  }
                  else
                  {
                     //:lWork = 0
                     lWork = 0;
                     //:lTextAddress = 0
                     lTextAddress = 0;
                     //:szWorkString = ""
                     ZeidonStringCopy( szWorkString, 1, 0, "", 1, 0, 32767 );
                  }

                  //:END
               }

               //:END

               //:ELSE  // Ctrl has no Mapping.
            }
            else
            {

               //:szWorkString = vReportDef.Control.Text
               GetVariableFromAttribute( szWorkString, 0, 'S', 32767, vReportDef, "Control", "Text", "", 0 );
            }


            //:END

            //:IF nHide = 0
            if ( nHide == 0 )
            {
               //:PrintTextToCSV( vSubtask, szWorkString, lColumnCSV )
               PrintTextToCSV( vSubtask, szWorkString, lColumnCSV );
               //:ELSE
            }
            else
            {
               //:PrintTextToCSV( vSubtask, "", lColumnCSV )
               PrintTextToCSV( vSubtask, "", lColumnCSV );
            }

            //:END
         }


         //:END  // IF lColumnCSV > 0

         //:ELSE
      }
      else
      {

         //:// Check to see if the next control will fit onto the page.
         //:// Since controls can be placed horizontally in an overlapping
         //:// manner, we must consider the longest of the overlapping controls.
         //:// This was taken into consideration in the Painter by computing
         //:// Control.RangeBottom.  We will also consider whether the last
         //:// controls in the Group were actually truncated by the Group size
         //:// itself.

         //:// Find the max y size on this line.
         //:lWork = vReportDef.Control.RangeBottom
         GetIntegerFromAttribute( &lWork, vReportDef, "Control", "RangeBottom" );

         //:IF lPrintHTML = 1   // this is to put in line breaks
         if ( lPrintHTML == 1 )
         {
            //:IF lWork != lRangeBottom
            if ( lWork != lRangeBottom )
            {
               //:PrintTextToHTML( vSubtask, vMappingOI, 0, "",
               //:              0, 0, 0, 0, 0, 0, 0, 0, 0 )
               PrintTextToHTML( vSubtask, vMappingOI, 0, "", 0, 0, 0, 0, 0, 0, 0, 0, 0 );
            }

            //:END
         }

         //:END

         //:lRangeBottom = lWork
         lRangeBottom = lWork;
         //:lMaxSizeY = vReportDef.Group.SZDLG_Y
         GetIntegerFromAttribute( &lMaxSizeY, vReportDef, "Group", "SZDLG_Y" );
         //:IF lMaxSizeY > lRangeBottom
         if ( lMaxSizeY > lRangeBottom )
         {
            //:lMaxSizeY = lRangeBottom
            lMaxSizeY = lRangeBottom;
         }

         //:END

         //:lMaxSizeY = lMaxSizeY - lStartBottomAdjustment
         lMaxSizeY = lMaxSizeY - lStartBottomAdjustment;
         //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/ AND
         //://UNDEF     lStartBottomAdjustment != 0
         //://UNDEF    TraceLineI( ">> lMaxSizeY decremented to: ", lMaxSizeY )
         //://UNDEF    TraceLineI( "   by lStartBottomAdjustment: ",
         //://UNDEF                lStartBottomAdjustment )
         //://UNDEF END

         //:lMaxSizeY = lMaxSizeY + lMultiLineIncrement
         lMaxSizeY = lMaxSizeY + lMultiLineIncrement;
         //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/ AND
         //://UNDEF     lMultiLineIncrement != 0
         //://UNDEF    TraceLineI( ">> lMaxSizeY incremented to: ", lMaxSizeY )
         //://UNDEF    TraceLineI( "      by MultiLineIncrement: ", lMultiLineIncrement )
         //://UNDEF END

         //:// If there isn't room for the current control (unless we are at the
         //:// top of the page), return to caller, indicating that the Group is
         //:// only partly processed.  This could occur because a multi-line
         //:// control forced the other controls in the Group off the page.  The
         //:// -1 return code indicates a continued Group.
         //:// Also set the multi-line current position value in the Page entity to
         //:// 0 to indicate we are not breaking in the middle of a multi-line text
         //:// control.
         //:IF bTopOfPage = FALSE AND // OLD WAS ==> AND lOrigRemainingSpace < lMaxSizeY
         //:(lRemainingSpaceOnPageStart < lMaxSizeY OR
         //: lRemainingSpaceOnPageY < 1) // this is an extra safety check
         if ( bTopOfPage == FALSE && ( lRemainingSpaceOnPageStart < lMaxSizeY || lRemainingSpaceOnPageY < 1 ) )
         {

            //:vReportDef.PageState.MultiLineTextPosEndPage = 0
            SetAttributeFromInteger( vReportDef, "PageState", "MultiLineTextPosEndPage", 0 );
            //:vReportDef.PageState.ContinuedGroupFlagEndPage = 2
            SetAttributeFromInteger( vReportDef, "PageState", "ContinuedGroupFlagEndPage", 2 );
            //:vReportDef.PageState.ContinuedControlTagEndPage = vReportDef.Control.Tag
            SetAttributeFromAttribute( vReportDef, "PageState", "ContinuedControlTagEndPage", vReportDef, "Control", "Tag" );

            //:// Nothing is printed so this is our new Y pos.
            //:plNewPosY = lCurrentPosY
            *plNewPosY = lCurrentPosY;

            //:nRptExitEvent = zRGE_EVENT_GROUP_SPLITTING
            nRptExitEvent = zRGE_EVENT_GROUP_SPLITTING;
            //:nRptExitReturn = InvokeReportExitCode( vSubtask, vReportDef,
            //:                                    szRptExitOper, nRptExitEvent,
            //:                                    lPageNbr, vSourceOI )
            nRptExitReturn = InvokeReportExitCode( vSubtask, vReportDef, szRptExitOper, nRptExitEvent, lPageNbr, vSourceOI );
            //:IF nRptExitReturn != 0
            if ( nRptExitReturn != 0 )
            {
               //:TraceLineI( "Non-zero return from Exit ignored for zRGE_EVENT_GROUP_SPLITTING: rc=",
               //:         nRptExitReturn )
               TraceLineI( "Non-zero return from Exit ignored for zRGE_EVENT_GROUP_SPLITTING: rc=", (zLONG) nRptExitReturn );
            }

            //:END

            //:IF vSourceOI != 0
            if ( vSourceOI != 0 )
            {
               //:// DropViewTrace( "Dropping view (19): ", vSourceOI )
               //:DropView( vSourceOI )
               DropView( vSourceOI );
               //:vSourceOI = 0
               vSourceOI = 0;
            }

            //:END

            //://UNDEF  IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
            //://UNDEF     TraceLineI( "FormatGroupOnPage returning -1 MaxSizeY: ",
            //://UNDEF                 lMaxSizeY )
            //://UNDEF     TraceLineI( "                            RangeBottom: ",
            //://UNDEF                 lRangeBottom )
            //://UNDEF     TraceLineI( "                     MultiLineIncrement: ",
            //://UNDEF                 lMultiLineIncrement )
            //://UNDEF     TraceLineI( "                   RemainingSpaceOnPage: ",
            //://UNDEF                 lRemainingSpaceOnPageY )
            //://UNDEF  END

            //:// DropViewTrace( "Dropping view (20): ", vReportTmp )
            //:DropView( vReportTmp )
            DropView( vReportTmp );
            //:vReportTmp = 0
            vReportTmp = 0;

            //:RETURN -1
            return( -1 );
         }


         //:END

         //://UNDEF szWorkString = vReportDef.Control.Tag

         //:lPagePosX = vReportDef.Control.PSDLG_X + vReportDef.Group.PSDLG_X
         GetIntegerFromAttribute( &lTempInteger_4, vReportDef, "Control", "PSDLG_X" );
         GetIntegerFromAttribute( &lTempInteger_5, vReportDef, "Group", "PSDLG_X" );
         lPagePosX = lTempInteger_4 + lTempInteger_5;
         //:lPagePosX = lPagePosX + plNewPosX
         lPagePosX = lPagePosX + *plNewPosX;
         //:IF lPagePosMaxX < lPagePosX
         if ( lPagePosMaxX < lPagePosX )
         {
            //:lPagePosMaxX = lPagePosX
            lPagePosMaxX = lPagePosX;
         }

         //:END

         //:lPagePosY = vReportDef.Control.PSDLG_Y + lMultiLineAdjustment +
         //:         lOrigPosY + lMultiLineIncrement
         GetIntegerFromAttribute( &lTempInteger_6, vReportDef, "Control", "PSDLG_Y" );
         lPagePosY = lTempInteger_6 + lMultiLineAdjustment + lOrigPosY + lMultiLineIncrement;

         //://UNDEF TraceLineS( "Current Ctrl: ", szWorkString )
         //://UNDEF TraceLineI( " CurrentPosY: ", lCurrentPosY )
         //://UNDEF lWork = vReportDef.Control.RangeTop
         //://UNDEF TraceLineI( " RangeTop: ", lWork )
         //://UNDEF TraceLineI( " RangeBottom: ", lRangeBottom )

         //:// IF the SuppressEmptyTextControls flag has been set for the report
         //:// and IF this Group is a detail Group
         //:// and IF this Group is vertical
         //:// and IF there is another control on the same line as the current ctrl
         //:// and IF that control has data mapping
         //:// and IF the text string that is mapped to the control is empty
         //:// THEN... we have a set of controls which should be skipped.
         //:// So, to make the skip happen, we will print nothing and we will adjust
         //:// the run-time adjustment, lMultiLineAdjustment, in a negative manner.
         //:nSkipFlag = 0   // 0 = false
         nSkipFlag = 0;
         //:SetViewFromView( vReportTmp, vReportDef )
         SetViewFromView( vReportTmp, vReportDef );
         //:SET CURSOR NEXT vReportTmp.Control
         RESULT = SetCursorNextEntity( vReportTmp, "Control", "" );
         //:IF RESULT >= zCURSOR_SET AND
         //:vReportDef.Report.SuppressEmptyTextFields = "Y" AND
         //:vReportDef.Group.Type = "ga" AND
         //:vReportDef.Control.Type = 10 AND // 10 vertical controls
         //:vReportTmp.CtrlMapView EXISTS
         lTempInteger_7 = CheckExistenceOfEntity( vReportTmp, "CtrlMapView" );
         if ( RESULT >= zCURSOR_SET && CompareAttributeToString( vReportDef, "Report", "SuppressEmptyTextFields", "Y" ) == 0 && CompareAttributeToString( vReportDef, "Group", "Type", "ga" ) == 0 &&
              CompareAttributeToInteger( vReportDef, "Control", "Type", 10 ) == 0 && lTempInteger_7 == 0 )
         {

            //:szEntityName = vReportTmp.CtrlMapRelatedEntity.Name
            GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportTmp, "CtrlMapRelatedEntity", "Name", "", 0 );
            //:szAttributeName = vReportTmp.CtrlMapER_Attribute.Name
            GetVariableFromAttribute( szAttributeName, 0, 'S', 33, vReportTmp, "CtrlMapER_Attribute", "Name", "", 0 );
            //:IF vReportDef.CtrlMapContext EXISTS
            lTempInteger_8 = CheckExistenceOfEntity( vReportDef, "CtrlMapContext" );
            if ( lTempInteger_8 == 0 )
            {
               //:szContextName = vReportDef.CtrlMapContext.Name
               GetVariableFromAttribute( szContextName, 0, 'S', 33, vReportDef, "CtrlMapContext", "Name", "", 0 );
               //:ELSE
            }
            else
            {
               //:szContextName = ""
               ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
            }

            //:END

            //:// For mapping, we can have either vSourceOI, which is for the hier
            //:// view driving the report, or any other mapping view.  We will
            //:// determine this by getting the mapping view for the control and
            //:// comparing it to the original hier view.
            //:szViewName = vReportTmp.CtrlMapView.Name
            GetVariableFromAttribute( szViewName, 0, 'S', 257, vReportTmp, "CtrlMapView", "Name", "", 0 );
            //:nSkipFlag = 1
            nSkipFlag = 1;
            //:// GET VIEW vMappingOI NAMED szViewName
            //:nRC = GetViewByName( vMappingOI, szViewName, vSubtask, zLEVEL_ANY )
            nRC = GetViewByName( &vMappingOI, szViewName, vSubtask, zLEVEL_ANY );
            //:IF nRC >= 0
            if ( nRC >= 0 )
            {
               //:// GET VIEW vSourceOrigOI NAMED "TZ_SourceOrigOI"
               //:szViewName = "TZ_SourceOrigOI"
               ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceOrigOI", 1, 0, 257 );
               //:RESULT = GetViewByName( vSourceOrigOI, szViewName, vSubtask, zLEVEL_ANY )
               RESULT = GetViewByName( &vSourceOrigOI, szViewName, vSubtask, zLEVEL_ANY );
               //:IF vMappingOI = vSourceOrigOI
               if ( vMappingOI == vSourceOrigOI )
               {
                  //:vMappingOI = vSourceOI
                  vMappingOI = vSourceOI;
               }

               //:END

               //:IF vMappingOI = 0
               if ( vMappingOI == 0 )
               {
                  //:nRC = 1
                  nRC = 1;
                  //:ELSE
               }
               else
               {
                  //:nRC = CheckExistenceOfEntity( vMappingOI, szEntityName )
                  nRC = CheckExistenceOfEntity( vMappingOI, szEntityName );
               }

               //:END

               //:IF nRC = 0
               if ( nRC == 0 )
               {
                  //:GetStringFromAttributeByContext( szWorkString, vMappingOI,
                  //:                              szEntityName, szAttributeName,
                  //:                              szContextName, 32764 )
                  GetStringFromAttributeByContext( szWorkString, vMappingOI, szEntityName, szAttributeName, szContextName, 32764 );
                  //:IF szWorkString != ""
                  if ( ZeidonStringCompare( szWorkString, 1, 0, "", 1, 0, 32767 ) != 0 )
                  {
                     //:nSkipFlag = 0     // 0 = don't skip
                     nSkipFlag = 0;
                  }

                  //:END
               }

               //:END
            }

            //:END
         }

         //:END

         //:// If we need to skip this line and both controls on this line
         //:IF nSkipFlag > 0
         if ( nSkipFlag > 0 )
         {
            //:lMultiLineAdjustment = lMultiLineAdjustment -
            //:                                      vReportDef.Control.SZDLG_Y
            GetIntegerFromAttribute( &lTempInteger_9, vReportDef, "Control", "SZDLG_Y" );
            lMultiLineAdjustment = lMultiLineAdjustment - lTempInteger_9;
            //:SetCursorNextEntity( vReportDef, "Control", "" )
            SetCursorNextEntity( vReportDef, "Control", "" );

            //:ELSE
         }
         else
         {
            //:lPagePosEndX = lPagePosX + vReportDef.Control.SZDLG_X
            GetIntegerFromAttribute( &lTempInteger_10, vReportDef, "Control", "SZDLG_X" );
            lPagePosEndX = lPagePosX + lTempInteger_10;
            //:lPagePosEndY = lPagePosY + vReportDef.Control.SZDLG_Y
            GetIntegerFromAttribute( &lTempInteger_11, vReportDef, "Control", "SZDLG_Y" );
            lPagePosEndY = lPagePosY + lTempInteger_11;
            //:IF lPagePosMaxX < lPagePosEndX
            if ( lPagePosMaxX < lPagePosEndX )
            {
               //:lPagePosMaxX = lPagePosEndX
               lPagePosMaxX = lPagePosEndX;
            }

            //:END

            //://UNDEF TraceLineI( " Current Ctrl StartPos Y: ", lPagePosY )

            //:szCtrlTag = vReportDef.Control.Tag
            GetVariableFromAttribute( szCtrlTag, 0, 'S', 33, vReportDef, "Control", "Tag", "", 0 );
            //:IF lLastPage = 1
            if ( lLastPage == 1 )
            {
               //:lTemp = 0
               lTemp = 0;
               //:ELSE
            }
            else
            {
               //:// TraceLineS( "FormatGroup Ctrl: ", szCtrlTag )
               //:lTemp = ZeidonStringFind( szCtrlTag, 1, "LPO_" )  // LastPageOnly
               lTemp = ZeidonStringFind( szCtrlTag, 1, "LPO_" );
            }

            //:// TraceLineI( "         LPO Pos: ", lTemp )
            //:END

            //:IF lTemp > 0
            if ( lTemp > 0 )
            {
               //:// do nothing here ... we are skipping this control
               //:ELSE
            }
            else
            {
               //:IF vReportDef.ControlDef.Key = 3040   // Bitmap
               if ( CompareAttributeToInteger( vReportDef, "ControlDef", "Key", 3040 ) == 0 )
               {

                  //:// TraceLineI( "Bitmap lPagePosX: ", lPagePosX )
                  //:// TraceLineI( "Bitmap lPagePosY: ", lPagePosY )
                  //:// TraceLineI( "Bitmap lPagePosEndX: ", lPagePosEndX )
                  //:// TraceLineI( "Bitmap lPagePosEndY: ", lPagePosEndY )

                  //:// Print the specified bitmap
                  //:IF nHide = 0
                  if ( nHide == 0 )
                  {
                     //:szEntityName = "Control"
                     ZeidonStringCopy( szEntityName, 1, 0, "Control", 1, 0, 33 );
                     //:szAttributeName = "CtrlBOI"
                     ZeidonStringCopy( szAttributeName, 1, 0, "CtrlBOI", 1, 0, 33 );
                     //:GetAddrForAttribute( lCtrlBOI, vReportDef,
                     //:            szEntityName, szAttributeName )
                     GetAddrForAttribute( &lCtrlBOI, vReportDef, szEntityName, szAttributeName );

                     //:IF lPrintHTML = 1
                     if ( lPrintHTML == 1 )
                     {
                        //:nRC = PrintBitmapToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                        //:                lPagePosEndX, lPagePosEndY, lCtrlBOI )
                        nRC = PrintBitmapToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lCtrlBOI );
                        //:ELSE
                     }
                     else
                     {
                        //:nRC = DrawBitmapToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                        //:                 lPagePosEndX, lPagePosEndY, lCtrlBOI )
                        nRC = DrawBitmapToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lCtrlBOI );
                     }

                     //:END
                  }

                  //:END

                  //:vReportDef.Control.wPrintStatus = 3  // zRCONTROL_STATUS_PRINTED
                  SetAttributeFromInteger( vReportDef, "Control", "wPrintStatus", 3 );
                  //:fnSetCtrlPrintValues( vReportDef, "", lPageNbr,
                  //:             lPagePosX, lPagePosY )
                  oTZRPSRCO_fnSetCtrlPrintValues( vReportDef, "", lPageNbr, lPagePosX, lPagePosY );
                  //:ELSE
               }
               else
               {
                  //:IF vReportDef.ControlDef.Key = 3050   // Shape
                  if ( CompareAttributeToInteger( vReportDef, "ControlDef", "Key", 3050 ) == 0 )
                  {

                     //:// TraceLineI( "Shape lPagePosX: ", lPagePosX )
                     //:// TraceLineI( "Shape lPagePosY: ", lPagePosY )
                     //:// TraceLineI( "Shape lPagePosEndX: ", lPagePosEndX )
                     //:// TraceLineI( "Shape lPagePosEndY: ", lPagePosEndY )

                     //:// Print the specified shape
                     //:nRC = SP_GetShapeCtrlBOI( vReportDef,
                     //:              nRectangleFlag, nRoundRectFlag,
                     //:              nEllipseFlag, nDiamondFlag,
                     //:              nHorizontalFlag, nVerticalFlag,
                     //:              nDiagDescendFlag, nDiagAscendFlag,
                     //:              lPenWidth, lPenType,
                     //:              lPenColor, lRoundValue,
                     //:              lBackgroundColor, lFlags )
                     nRC = SP_GetShapeCtrlBOI( vReportDef, &nRectangleFlag, &nRoundRectFlag, &nEllipseFlag, &nDiamondFlag, &nHorizontalFlag, &nVerticalFlag, &nDiagDescendFlag, &nDiagAscendFlag, &lPenWidth, &lPenType, &lPenColor, &lRoundValue,
                        &lBackgroundColor, &lFlags );

                     //:vReportDef.Control.wPrintStatus = 3  // zRCONTROL_STATUS_PRINTED
                     SetAttributeFromInteger( vReportDef, "Control", "wPrintStatus", 3 );
                     //:fnSetCtrlPrintValues( vReportDef, "", lPageNbr,
                     //:          lPagePosX, lPagePosY )
                     oTZRPSRCO_fnSetCtrlPrintValues( vReportDef, "", lPageNbr, lPagePosX, lPagePosY );

                     //:IF nRectangleFlag != 0
                     if ( nRectangleFlag != 0 )
                     {

                        //:nRC = IsFlagSequenceSet( lFlags, 1 )
                        nRC = IsFlagSequenceSet( lFlags, 1 );
                        //:IF nRC != 0
                        if ( nRC != 0 )
                        {
                           //:lWork = lPagePosEndY
                           lWork = lPagePosEndY;
                           //:lPagePosEndY = lFooterPosPageBottom - 10
                           lPagePosEndY = lFooterPosPageBottom - 10;
                        }

                        //:END

                        //:IF lPrintHTML = 0
                        if ( lPrintHTML == 0 )
                        {
                           //:IF nHide = 0
                           if ( nHide == 0 )
                           {
                              //:DrawRectangleToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                              //:           lPagePosEndX, lPagePosEndY,
                              //:           lPenWidth, lPenType,
                              //:           lPenColor, 0, lBackgroundColor )
                              DrawRectangleToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 0, lBackgroundColor );
                           }

                           //:END
                           //:ELSE
                        }
                        else
                        {
                           //:IF nHide = 0
                           if ( nHide == 0 )
                           {
                              //:PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                              //:     lPagePosEndX, lPagePosEndY,
                              //:     lPenWidth, lPenType,
                              //:     lPenColor, 0 )
                              PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 0 );
                           }

                           //:END
                        }

                        //:END

                        //:IF nRC != 0
                        if ( nRC != 0 )
                        {
                           //:lPagePosEndY = lWork
                           lPagePosEndY = lWork;
                        }

                        //:END
                     }

                     //:END

                     //:IF nRoundRectFlag != 0
                     if ( nRoundRectFlag != 0 )
                     {

                        //:IF lRoundValue <= 0
                        if ( lRoundValue <= 0 )
                        {
                           //:lRoundValue = 16
                           lRoundValue = 16;
                        }

                        //:END

                        //:nRC = IsFlagSequenceSet( lFlags, 1 )
                        nRC = IsFlagSequenceSet( lFlags, 1 );
                        //:IF nRC != 0
                        if ( nRC != 0 )
                        {
                           //:lWork = lPagePosEndY
                           lWork = lPagePosEndY;
                           //:lPagePosEndY = lFooterPosPageBottom - 10
                           lPagePosEndY = lFooterPosPageBottom - 10;
                        }

                        //:END

                        //:IF lPrintHTML = 0
                        if ( lPrintHTML == 0 )
                        {
                           //:IF nHide = 0
                           if ( nHide == 0 )
                           {
                              //:DrawRectangleToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                              //:           lPagePosEndX, lPagePosEndY,
                              //:           lPenWidth, lPenType,
                              //:           lPenColor, lRoundValue, lBackgroundColor )
                              DrawRectangleToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, lRoundValue, lBackgroundColor );
                           }

                           //:END
                           //:ELSE
                        }
                        else
                        {
                           //:IF nHide = 0
                           if ( nHide == 0 )
                           {
                              //:PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                              //:     lPagePosEndX, lPagePosEndY,
                              //:     lPenWidth, lPenType,
                              //:     lPenColor, 0 )
                              PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 0 );
                           }

                           //:END
                        }

                        //:END

                        //:IF nRC != 0
                        if ( nRC != 0 )
                        {
                           //:lPagePosEndY = lWork
                           lPagePosEndY = lWork;
                        }

                        //:END
                     }

                     //:END

                     //:IF nEllipseFlag != 0 AND nHide = 0
                     if ( nEllipseFlag != 0 && nHide == 0 )
                     {

                        //:nRC = IsFlagSequenceSet( lFlags, 1 )
                        nRC = IsFlagSequenceSet( lFlags, 1 );
                        //:IF nRC != 0
                        if ( nRC != 0 )
                        {
                           //:lWork = lPagePosEndY
                           lWork = lPagePosEndY;
                           //:lPagePosEndY = lFooterPosPageBottom - 10
                           lPagePosEndY = lFooterPosPageBottom - 10;
                        }

                        //:END

                        //:IF lPrintHTML = 0
                        if ( lPrintHTML == 0 )
                        {
                           //:DrawEllipseToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                           //:         lPagePosEndX, lPagePosEndY,
                           //:         lPenWidth, lPenType, lPenColor,
                           //:         lBackgroundColor )
                           DrawEllipseToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, lBackgroundColor );
                           //:ELSE
                        }
                        else
                        {
                           //:PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                           //:     lPagePosEndX, lPagePosEndY,
                           //:     lPenWidth, lPenType,
                           //:     lPenColor, 0 )
                           PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 0 );
                        }

                        //:END

                        //:IF nRC != 0
                        if ( nRC != 0 )
                        {
                           //:lPagePosEndY = lWork
                           lPagePosEndY = lWork;
                        }

                        //:END
                     }

                     //:END

                     //:IF nDiamondFlag != 0 AND nHide = 0
                     if ( nDiamondFlag != 0 && nHide == 0 )
                     {
                        //:nRC = IsFlagSequenceSet( lFlags, 1 )
                        nRC = IsFlagSequenceSet( lFlags, 1 );
                        //:IF nRC != 0
                        if ( nRC != 0 )
                        {
                           //:lWork = lPagePosEndY
                           lWork = lPagePosEndY;
                           //:lPagePosEndY = lFooterPosPageBottom - 10
                           lPagePosEndY = lFooterPosPageBottom - 10;
                        }

                        //:END

                        //:IF lPrintHTML = 0
                        if ( lPrintHTML == 0 )
                        {
                           //:DrawRectangleToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                           //:           lPagePosEndX, lPagePosEndY,
                           //:           lPenWidth, lPenType,
                           //:           lPenColor, -1, lBackgroundColor )
                           DrawRectangleToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, -1, lBackgroundColor );
                           //:ELSE
                        }
                        else
                        {
                           //:PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                           //:     lPagePosEndX, lPagePosEndY,
                           //:     lPenWidth, lPenType,
                           //:     lPenColor, 0 )
                           PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 0 );
                        }

                        //:END

                        //:IF nRC != 0
                        if ( nRC != 0 )
                        {
                           //:lPagePosEndY = lWork
                           lPagePosEndY = lWork;
                        }

                        //:END
                     }

                     //:END

                     //:IF nHorizontalFlag != 0 AND nHide = 0
                     if ( nHorizontalFlag != 0 && nHide == 0 )
                     {
                        //:IF lPrintHTML = 1
                        if ( lPrintHTML == 1 )
                        {
                           //:PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                           //:     lPagePosEndX, lPagePosEndY,
                           //:     lPenWidth, lPenType,
                           //:     lPenColor, 0 )
                           PrintLineToHTML( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 0 );
                           //:ELSE
                        }
                        else
                        {
                           //:DrawLineToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                           //:      lPagePosEndX, lPagePosEndY,
                           //:      lPenWidth, lPenType,
                           //:      lPenColor, 0 )
                           DrawLineToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 0 );
                        }

                        //:END
                     }

                     //:END

                     //:IF nVerticalFlag != 0 AND nHide = 0
                     if ( nVerticalFlag != 0 && nHide == 0 )
                     {
                        //:DrawLineToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                        //:      lPagePosEndX, lPagePosEndY,
                        //:      lPenWidth, lPenType,
                        //:      lPenColor, 1 )
                        DrawLineToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 1 );
                     }

                     //:END

                     //:IF nDiagDescendFlag != 0 AND nHide = 0
                     if ( nDiagDescendFlag != 0 && nHide == 0 )
                     {
                        //:DrawLineToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                        //:      lPagePosEndX, lPagePosEndY,
                        //:      lPenWidth, lPenType,
                        //:      lPenColor, 2 )
                        DrawLineToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 2 );
                     }

                     //:END

                     //:IF nDiagAscendFlag != 0 AND nHide = 0
                     if ( nDiagAscendFlag != 0 && nHide == 0 )
                     {
                        //:DrawLineToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY,
                        //:      lPagePosEndX, lPagePosEndY,
                        //:      lPenWidth, lPenType,
                        //:      lPenColor, 3 )
                        DrawLineToReport( vSubtask, vSourceOI, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lPenWidth, lPenType, lPenColor, 3 );
                     }

                     //:END

                     //:ELSE
                  }
                  else
                  {
                     //:// If the Control has FontSize or Bold or Italic flags set,
                     //:// create a temporary font with those characteristics and use it.
                     //:// If not, use the default font.
                     //:nRC = TX_GetTextRptCtrlBOI( vReportDef, nMultiLineFlag, nBoldFlag,
                     //:                nItalicFlag, nStrikeoutFlag,
                     //:                nUnderlineFlag, lJustify,
                     //:                lFontSize, lTextColor, lTextBkColor,
                     //:                lBorderStyle, lBorderColor,
                     //:                lEscapement, lType, szFaceName )
                     nRC = TX_GetTextRptCtrlBOI( vReportDef, &nMultiLineFlag, &nBoldFlag, &nItalicFlag, &nStrikeoutFlag, &nUnderlineFlag, &lJustify, &lFontSize, &lTextColor, &lTextBkColor, &lBorderStyle, &lBorderColor, &lEscapement, &lType, szFaceName );

                     //:// If the return code is <= 0, the CtrlBOI is null.
                     //:IF nRC <= 0
                     if ( nRC <= 0 )
                     {
                        //:SelectFontForReport( vSubtask, "default" )
                        SelectFontForReport( vSubtask, "default" );
                        //:ELSE
                     }
                     else
                     {
                        //:IF lFontSize = 0
                        if ( lFontSize == 0 )
                        {
                           //:lFontSize = lDefaultFontSize
                           lFontSize = lDefaultFontSize;
                        }

                        //:END

                        //:// TraceLineI( "FormatGroupOnPage Font size: ", lFontSize )

                        //:lEscapement = 0 - lEscapement
                        lEscapement = 0 - lEscapement;
                        //:CreateFontForReport( vSubtask, "control", lFontSize,
                        //:         nBoldFlag, nItalicFlag,
                        //:         nUnderlineFlag, nStrikeoutFlag,
                        //:         lTextColor, lTextBkColor,
                        //:         lEscapement, szFaceName )
                        CreateFontForReport( vSubtask, "control", lFontSize, nBoldFlag, nItalicFlag, nUnderlineFlag, nStrikeoutFlag, lTextColor, lTextBkColor, lEscapement, szFaceName );
                        //:SelectFontForReport( vSubtask, "control" )
                        SelectFontForReport( vSubtask, "control" );
                     }

                     //:END

                     //:// PRINT TEXT

                     //:// If Multi-line text.
                     //:IF nMultiLineFlag = 1 AND vReportDef.CtrlMapView EXISTS
                     lTempInteger_12 = CheckExistenceOfEntity( vReportDef, "CtrlMapView" );
                     if ( nMultiLineFlag == 1 && lTempInteger_12 == 0 )
                     {

                        //:// Get text value to put on report AND GET AN ADDRESS TO IT.
                        //:// If there is mapping on the report (determined by the existence
                        //:// of the CtrlMapView entity), use it to get the text value.
                        //:// Otherwise, do no print at all.
                        //:szEntityName = vReportDef.CtrlMapRelatedEntity.Name
                        GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "CtrlMapRelatedEntity", "Name", "", 0 );
                        //:szAttributeName = vReportDef.CtrlMapER_Attribute.Name
                        GetVariableFromAttribute( szAttributeName, 0, 'S', 33, vReportDef, "CtrlMapER_Attribute", "Name", "", 0 );

                        //:// For mapping, we can have either vSourceOI, which is for
                        //:// the hier view driving the report, or any other mapping
                        //:// view.  We will determine this by getting the mapping view
                        //:// for the control and comparing it to the original hier view.
                        //:szViewName = vReportDef.CtrlMapView.Name
                        GetVariableFromAttribute( szViewName, 0, 'S', 257, vReportDef, "CtrlMapView", "Name", "", 0 );
                        //:// GET VIEW vMappingOI NAMED szViewName
                        //:nRC = GetViewByName( vMappingOI, szViewName, vSubtask, zLEVEL_ANY )
                        nRC = GetViewByName( &vMappingOI, szViewName, vSubtask, zLEVEL_ANY );
                        //:IF nRC >= 0
                        if ( nRC >= 0 )
                        {
                           //:// GET VIEW vSourceOrigOI NAMED "TZ_SourceOrigOI"
                           //:szViewName = "TZ_SourceOrigOI"
                           ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceOrigOI", 1, 0, 257 );
                           //:nRC = GetViewByName( vSourceOrigOI, szViewName,
                           //:         vSubtask, zLEVEL_ANY )
                           nRC = GetViewByName( &vSourceOrigOI, szViewName, vSubtask, zLEVEL_ANY );
                           //:IF vMappingOI = vSourceOrigOI
                           if ( vMappingOI == vSourceOrigOI )
                           {
                              //:vMappingOI = vSourceOI
                              vMappingOI = vSourceOI;
                           }

                           //:END
                           //:ELSE
                        }
                        else
                        {
                           //:szWorkString = "Report cannot get view by name: " + szViewName
                           ZeidonStringCopy( szWorkString, 1, 0, "Report cannot get view by name: ", 1, 0, 32767 );
                           ZeidonStringConcat( szWorkString, 1, 0, szViewName, 1, 0, 32767 );
                           //:TraceLineS( szWorkString, "" )
                           TraceLineS( szWorkString, "" );
                        }

                        //:// szWorkString = ""
                        //:END

                        //:IF vMappingOI = 0
                        if ( vMappingOI == 0 )
                        {
                           //:nRC = 1
                           nRC = 1;
                           //:ELSE
                        }
                        else
                        {
                           //:nRC = CheckExistenceOfEntity( vMappingOI, szEntityName )
                           nRC = CheckExistenceOfEntity( vMappingOI, szEntityName );
                        }

                        //:END

                        //:IF nRC = 0
                        if ( nRC == 0 )
                        {
                           //://UNDEF        IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                           //://UNDEF           TraceLineS( " --> MultiLine Case : ", "TextAddress OK." )
                           //://UNDEF        END
                           //:GetAttributeDisplayLength( lWork, vMappingOI,
                           //:               szEntityName, szAttributeName, "" )
                           GetAttributeDisplayLength( (zPULONG) &lWork, vMappingOI, szEntityName, szAttributeName, "" );
                           //:IF lWork > 32764
                           if ( lWork > 32764 )
                           {
                              //:GetAddrForAttribute( lTextAddress, vMappingOI,
                              //:         szEntityName, szAttributeName )
                              GetAddrForAttribute( &lTextAddress, vMappingOI, szEntityName, szAttributeName );
                              //:ELSE
                           }
                           else
                           {
                              //:lTextAddress = MakeIntegerFromPointer( szWorkString )
                              lTextAddress = MakeIntegerFromPointer( szWorkString );
                              //:GetStringFromAttributeByContext( szWorkString, vMappingOI,
                              //:                     szEntityName,
                              //:                     szAttributeName,
                              //:                     "", 32764 )
                              GetStringFromAttributeByContext( szWorkString, vMappingOI, szEntityName, szAttributeName, "", 32764 );
                           }

                           //:END
                           //:ELSE
                        }
                        else
                        {
                           //://UNDEF        IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                           //://UNDEF           TraceLineS( " --> MultiLine Case : ", "TextAddress = 0" )
                           //://UNDEF        END

                           //:nRC = 0  // added to permit print of entire HTML when a view
                           nRC = 0;
                           //:// did not exist for the report ... dks 2005.12.22
                           //:lWork = 0
                           lWork = 0;
                           //:lTextAddress = 0
                           lTextAddress = 0;
                           //:szWorkString = ""
                           ZeidonStringCopy( szWorkString, 1, 0, "", 1, 0, 32767 );
                        }

                        //:END

                        //:// If we are printing a continued Group, start printing
                        //:// at the first unprinted word. Otherwise, start printing
                        //:// at the beginning of the text.
                        //:lNextWord = 0
                        lNextWord = 0;
                        //:IF vReportDef.PageState.ContinuedGroupFlagStartPage > 0 AND
                        //:lContinuedGroupFlag > 0
                        if ( CompareAttributeToInteger( vReportDef, "PageState", "ContinuedGroupFlagStartPage", 0 ) > 0 && lContinuedGroupFlag > 0 )
                        {

                           //:IF bCurrentGroupCanContinue = TRUE
                           if ( bCurrentGroupCanContinue == TRUE )
                           {
                              //:lNextWord = vReportDef.PageState.MultiLineTextPosStartPage
                              GetIntegerFromAttribute( &lNextWord, vReportDef, "PageState", "MultiLineTextPosStartPage" );
                           }

                           //:END
                        }

                        //:END

                        //:// Do the print!  On return, lEndPosY will contain the amount of
                        //:// space taken on the page by the multi-line text.
                        //:// lPagePosEndX = lPagePosX + vReportDef.Control.SZDLG_X
                        //:// Set lPagePosEndY to limit the amount of space to take.  This
                        //:// is currently the same as the end of the page, but when we add
                        //:// the option to prevent multiline text to grow beyond the size
                        //:// of the text box defined, this limit will change
                        //:lPagePosEndY = lFooterPosPageBottom
                        lPagePosEndY = lFooterPosPageBottom;

                        //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                        //://UNDEF        TraceLineS( " --> Text for control : ", szWorkString )
                        //://UNDEF        TraceLineI( " -->     start offset : ", lNextWord )
                        //://UNDEF        TraceLineI( " --> RemainingSpaceOnPage: ", lRemainingSpaceOnPageY )
                        //://UNDEF        TraceLineI( " -->            lPagePosY: ", lPagePosY )
                        //://UNDEF        TraceLineI( " -->         lPagePosEndY: ", lPagePosEndY )
                        //://UNDEF     END

                        //:fnSetCtrlPrintValues( vReportDef, szWorkString, lPageNbr,
                        //:          lPagePosX, lPagePosY )
                        oTZRPSRCO_fnSetCtrlPrintValues( vReportDef, szWorkString, lPageNbr, lPagePosX, lPagePosY );

                        //:IF lPrintHTML = 1
                        if ( lPrintHTML == 1 )
                        {
                           //:IF nHide = 0
                           if ( nHide == 0 )
                           {
                              //:szClass = vReportDef.Control.CSS_Class
                              GetVariableFromAttribute( szClass, 0, 'S', 257, vReportDef, "Control", "CSS_Class", "", 0 );
                              //:IF szClass != ""
                              if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) != 0 )
                              {
                                 //:szYN = vReportDef.Control.CSS_Font
                                 GetVariableFromAttribute( szYN, 0, 'S', 33, vReportDef, "Control", "CSS_Font", "", 0 );
                                 //:IF szYN = "Y"
                                 if ( ZeidonStringCompare( szYN, 1, 0, "Y", 1, 0, 33 ) == 0 )
                                 {
                                    //:lType = lType + 16
                                    lType = lType + 16;
                                 }

                                 //:END
                                 //:szYN = vReportDef.Control.CSS_Position
                                 GetVariableFromAttribute( szYN, 0, 'S', 33, vReportDef, "Control", "CSS_Position", "", 0 );
                                 //:IF szYN = "Y"
                                 if ( ZeidonStringCompare( szYN, 1, 0, "Y", 1, 0, 33 ) == 0 )
                                 {
                                    //:lType = lType + 32
                                    lType = lType + 32;
                                 }

                                 //:END
                                 //:szYN = vReportDef.Control.CSS_Size
                                 GetVariableFromAttribute( szYN, 0, 'S', 33, vReportDef, "Control", "CSS_Size", "", 0 );
                                 //:IF szYN = "Y"
                                 if ( ZeidonStringCompare( szYN, 1, 0, "Y", 1, 0, 33 ) == 0 )
                                 {
                                    //:lType = lType + 64
                                    lType = lType + 64;
                                 }

                                 //:END
                              }

                              //:END

                              //:PrintTextToHTML( vSubtask, vMappingOI, szWorkString,
                              //:     szClass, lPagePosX, lPagePosY,
                              //:     lPagePosEndX, lPagePosEndY, lJustify,
                              //:     lBorderStyle, lBorderColor, lEscapement,
                              //:     lType )
                              PrintTextToHTML( vSubtask, vMappingOI, szWorkString, szClass, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lJustify, lBorderStyle, lBorderColor, lEscapement, lType );
                           }

                           //:END
                           //:ELSE
                        }
                        else
                        {
                           //:IF lTextAddress = 0 OR nHide = 1
                           if ( lTextAddress == 0 || nHide == 1 )
                           {
                              //:nRC = DrawMultiLineTextToReport( vSubtask, vMappingOI,
                              //:                  "", lWork,
                              //:                  lPagePosX, lPagePosY,
                              //:                  lPagePosEndX, lPagePosEndY,
                              //:                  lJustify, lBorderStyle,
                              //:                  lBorderColor, lEscapement,
                              //:                  lType, lEndPosY, lNextWord )
                              nRC = DrawMultiLineTextToReport( vSubtask, vMappingOI, "", lWork, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lJustify, lBorderStyle, lBorderColor, lEscapement, lType, &lEndPosY, &lNextWord );
                              //:ELSE
                           }
                           else
                           {
                              //:nRC = DrawMultiLineTextToReport( vSubtask, vMappingOI,
                              //:                  lTextAddress, lWork,
                              //:                  lPagePosX, lPagePosY,
                              //:                  lPagePosEndX, lPagePosEndY,
                              //:                  lJustify, lBorderStyle,
                              //:                  lBorderColor, lEscapement,
                              //:                  lType, lEndPosY, lNextWord )
                              nRC = DrawMultiLineTextToReport( vSubtask, vMappingOI, lTextAddress, lWork, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lJustify, lBorderStyle, lBorderColor, lEscapement, lType, &lEndPosY, &lNextWord );
                           }

                           //:END
                        }

                        //:END

                        //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                        //://UNDEF        TraceLineI( "     MultiLine EndPos Y: ", lEndPosY )
                        //://UNDEF     END


                        //:// If the end position is less than that determined by control
                        //:// size, use control size instead to get more consistent looking
                        //:// line spacing.
                        //:lTemp = lPagePosY + vReportDef.Control.SZDLG_Y
                        GetIntegerFromAttribute( &lTempInteger_13, vReportDef, "Control", "SZDLG_Y" );
                        lTemp = lPagePosY + lTempInteger_13;
                        //:IF lTemp > lEndPosY
                        if ( lTemp > lEndPosY )
                        {
                           //:lEndPosY = lTemp
                           lEndPosY = lTemp;
                           //:ELSE
                        }
                        else
                        {
                           //:// Otherwise, the multi-line text took up more than one line
                           //:// of space so we capture adjustment values.
                           //:lEndPosY = lEndPosY + 1 // a classic fudge factor to open up
                           lEndPosY = lEndPosY + 1;
                           //:            // space between lines
                           //:lTemp = lEndPosY - lTemp
                           lTemp = lEndPosY - lTemp;
                           //:// Adjust the size of the set of controls being printed
                           //:lMaxSizeY = lMaxSizeY + lTemp
                           lMaxSizeY = lMaxSizeY + lTemp;
                           //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                           //://UNDEF        TraceLineI( " --> increase lMaxSizeY to ", lMaxSizeY )
                           //://UNDEF     END
                           //:lMultiLineIncrement = lMultiLineIncrement + lTemp
                           lMultiLineIncrement = lMultiLineIncrement + lTemp;
                        }

                        //:// lMultiLineAdjustment = lMultiLineAdjustment + lTemp
                        //:END

                        //:// In either case, record the size of this line ignoring previous
                        //:// controls on this line
                        //:lMultiLineSizeY = lEndPosY - lPagePosY
                        lMultiLineSizeY = lEndPosY - lPagePosY;

                        //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                        //://UNDEF        TraceLineI( " --> lPagePosY = ", lPagePosY )
                        //://UNDEF        TraceLineI( " --> lEndPosY = ", lEndPosY  )
                        //://UNDEF        TraceLineI( " --> lMultiLineIncrement = ", lMultiLineIncrement )
                        //://UNDEF        TraceLineI( " --> lMultiLineAdjustment = ",
                        //://UNDEF                    lMultiLineAdjustment )
                        //://UNDEF     END

                        //:// If the text we just printed didn't fit on the page, make sure it
                        //:// it is for an entity Group detail - otherwise, don't split.
                        //:IF nRC = 1
                        if ( nRC == 1 )
                        {
                           //:IF bCurrentGroupCanContinue = FALSE
                           if ( bCurrentGroupCanContinue == FALSE )
                           {
                              //:TraceLineS( "WARNING: a multiline text field can ONLY be ",
                              //:"continued for an entity Detail Group." )
                              TraceLineS( "WARNING: a multiline text field can ONLY be ", "continued for an entity Detail Group." );
                              //:TraceLineS( "         GroupSet: ", vReportDef.GroupSet.Tag )
                              GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vReportDef, "GroupSet", "Tag" );
                              TraceLineS( "         GroupSet: ", szTempString_0 );
                              //:TraceLineS( "         Group   : ", vReportDef.Group.Tag )
                              GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), vReportDef, "Group", "Tag" );
                              TraceLineS( "         Group   : ", szTempString_1 );
                              //:TraceLineS( "         Control : ", vReportDef.Control.Tag )
                              GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), vReportDef, "Control", "Tag" );
                              TraceLineS( "         Control : ", szTempString_2 );
                              //:TraceLineI( "         Text truncated to (chars): ",
                              //:lNextWord )
                              TraceLineI( "         Text truncated to (chars): ", lNextWord );
                              //:nRC = 0  // turn off continuation
                              nRC = 0;
                           }

                           //:END
                        }

                        //:END
                        //:// If the text we just printed didn't fit on the page, return now.
                        //:IF nRC = 1
                        if ( nRC == 1 )
                        {
                           //:// Store offset to first unprinted word in multi-line text.
                           //:vReportDef.PageState.MultiLineTextPosEndPage = lNextWord
                           SetAttributeFromInteger( vReportDef, "PageState", "MultiLineTextPosEndPage", lNextWord );
                           //:// TraceLineI( "FormatGroupOnPage MultiLineTextPos: ", lNextWord )
                           //:vReportDef.PageState.ContinuedGroupFlagEndPage = 1
                           SetAttributeFromInteger( vReportDef, "PageState", "ContinuedGroupFlagEndPage", 1 );
                           //:vReportDef.PageState.ContinuedControlTagEndPage =
                           //:                            vReportDef.Control.Tag
                           SetAttributeFromAttribute( vReportDef, "PageState", "ContinuedControlTagEndPage", vReportDef, "Control", "Tag" );

                           //:plNewPosY = lOrigPosY + vReportDef.Group.SZDLG_Y +
                           //:                              lMultiLineAdjustment
                           GetIntegerFromAttribute( &lTempInteger_14, vReportDef, "Group", "SZDLG_Y" );
                           *plNewPosY = lOrigPosY + lTempInteger_14 + lMultiLineAdjustment;
                           //:vReportDef.Control.wPrintStatus = 2 // zRCONTROL_STATUS_PARTIALPRINT
                           SetAttributeFromInteger( vReportDef, "Control", "wPrintStatus", 2 );
                           //:nRptExitEvent = zRGE_EVENT_GROUP_SPLITTING
                           nRptExitEvent = zRGE_EVENT_GROUP_SPLITTING;
                           //:nRptExitReturn = InvokeReportExitCode( vSubtask, vReportDef,
                           //:                           szRptExitOper,
                           //:                           nRptExitEvent,
                           //:                           lPageNbr, vSourceOI )
                           nRptExitReturn = InvokeReportExitCode( vSubtask, vReportDef, szRptExitOper, nRptExitEvent, lPageNbr, vSourceOI );
                           //:IF nRptExitReturn != 0
                           if ( nRptExitReturn != 0 )
                           {
                              //:TraceLineI( "Non-zero return from Exit ignored for zRGE_EVENT_GROUP_SPLITTING: rc=",
                              //:nRptExitReturn )
                              TraceLineI( "Non-zero return from Exit ignored for zRGE_EVENT_GROUP_SPLITTING: rc=", (zLONG) nRptExitReturn );
                           }

                           //:END
                           //:IF vSourceOI != 0
                           if ( vSourceOI != 0 )
                           {
                              //://UNDVV           IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                              //://UNDVV              TraceLineV( "(FormatGroupOnPage.MultiLine) DropView( vSourceOI ): ",
                              //://UNDVV                          vSourceOI )
                              //://UNDVV           END
                              //:// DropViewTrace( "Dropping view (21): ", vSourceOI )
                              //:DropView( vSourceOI )
                              DropView( vSourceOI );
                              //:vSourceOI = 0
                              vSourceOI = 0;
                           }

                           //:END

                           //://UNDEF        IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                           //://UNDEF           TraceLineS( "FormatGroupOnPage returning -1", " MultiLine" )
                           //://UNDEF        END

                           //:// DropViewTrace( "Dropping view (22): ", vReportTmp )
                           //:DropView( vReportTmp )
                           DropView( vReportTmp );
                           //:vReportTmp = 0
                           vReportTmp = 0;

                           //:RETURN -1
                           return( -1 );

                           //:ELSE
                        }
                        else
                        {

                           //:// Clear offset that may have been stored in previous iteration
                           //:IF bCurrentGroupCanContinue = TRUE
                           if ( bCurrentGroupCanContinue == TRUE )
                           {
                              //:vReportDef.PageState.MultiLineTextPosEndPage = 0
                              SetAttributeFromInteger( vReportDef, "PageState", "MultiLineTextPosEndPage", 0 );
                              //:// PAS 08Nov01 - make sure we don't continue the Group.
                              //:vReportDef.PageState.ContinuedGroupFlagEndPage = 0
                              SetAttributeFromInteger( vReportDef, "PageState", "ContinuedGroupFlagEndPage", 0 );
                           }

                           //:END

                           //:vReportDef.Control.wPrintStatus = 3  // zRCONTROL_STATUS_PRINTED
                           SetAttributeFromInteger( vReportDef, "Control", "wPrintStatus", 3 );
                        }

                        //:END

                        //:ELSE  // Not MultiLine Text ... or if it is, it has no Mapping.
                     }
                     else
                     {

                        //:// Get text value to put on report AND STORE IT IN szWorkString.
                        //:// If there is mapping on the report, use it to get the text
                        //:// value.  (We will determine this by the existence of the
                        //:// entity CtrlMapView.)  If there is no mapping on the report,
                        //:// use the Text attribute value from the ReportDef.
                        //:IF vReportDef.CtrlMapView EXISTS
                        lTempInteger_15 = CheckExistenceOfEntity( vReportDef, "CtrlMapView" );
                        if ( lTempInteger_15 == 0 )
                        {

                           //:szEntityName = vReportDef.CtrlMapRelatedEntity.Name
                           GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "CtrlMapRelatedEntity", "Name", "", 0 );
                           //:szAttributeName = vReportDef.CtrlMapER_Attribute.Name
                           GetVariableFromAttribute( szAttributeName, 0, 'S', 33, vReportDef, "CtrlMapER_Attribute", "Name", "", 0 );
                           //:IF vReportDef.CtrlMapContext EXISTS
                           lTempInteger_16 = CheckExistenceOfEntity( vReportDef, "CtrlMapContext" );
                           if ( lTempInteger_16 == 0 )
                           {
                              //:szContextName = vReportDef.CtrlMapContext.Name
                              GetVariableFromAttribute( szContextName, 0, 'S', 33, vReportDef, "CtrlMapContext", "Name", "", 0 );
                              //:ELSE
                           }
                           else
                           {
                              //:szContextName = ""
                              ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
                           }

                           //:END

                           //:// For mapping, we can have either vSourceOI, which is for
                           //:// the hier view driving the report, or any other mapping
                           //:// view.  We will determine this by getting the mapping view
                           //:// for the control and comparing it to the original hier view.
                           //:szViewName = vReportDef.CtrlMapView.Name
                           GetVariableFromAttribute( szViewName, 0, 'S', 257, vReportDef, "CtrlMapView", "Name", "", 0 );
                           //:// GET VIEW vMappingOI NAMED szViewName
                           //:nRC = GetViewByName( vMappingOI, szViewName,
                           //:         vSubtask, zLEVEL_ANY )
                           nRC = GetViewByName( &vMappingOI, szViewName, vSubtask, zLEVEL_ANY );
                           //:IF nRC >= 0
                           if ( nRC >= 0 )
                           {
                              //:// GET VIEW vSourceOrigOI NAMED "TZ_SourceOrigOI"
                              //:szViewName = "TZ_SourceOrigOI"
                              ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceOrigOI", 1, 0, 257 );
                              //:nRC = GetViewByName( vSourceOrigOI, szViewName,
                              //:         vSubtask, zLEVEL_ANY )
                              nRC = GetViewByName( &vSourceOrigOI, szViewName, vSubtask, zLEVEL_ANY );
                              //:IF vMappingOI = vSourceOrigOI
                              if ( vMappingOI == vSourceOrigOI )
                              {
                                 //:vMappingOI = vSourceOI
                                 vMappingOI = vSourceOI;
                              }

                              //:END

                              //:IF vReportDef.Control.wQueryOperation != ""
                              if ( CompareAttributeToString( vReportDef, "Control", "wQueryOperation", "" ) != 0 )
                              {
                                 //:// Query Operation
                                 //:szQueryOperation = vReportDef.Control.wQueryOperation
                                 GetVariableFromAttribute( szQueryOperation, 0, 'S', 11, vReportDef, "Control", "wQueryOperation", "", 0 );
                                 //:ComputeQueryOperationValue( vReportDef, vMappingOI,
                                 //:                szWorkString,
                                 //:                szQueryOperation,
                                 //:                szEntityName,
                                 //:                szAttributeName )
                                 ComputeQueryOperationValue( vReportDef, vMappingOI, szWorkString, szQueryOperation, szEntityName, szAttributeName );
                                 //:ELSE
                              }
                              else
                              {
                                 //:// Regular Mapping
                                 //:IF vMappingOI = 0
                                 if ( vMappingOI == 0 )
                                 {
                                    //:nRC = 1
                                    nRC = 1;
                                    //:ELSE
                                 }
                                 else
                                 {
                                    //:nRC = CheckExistenceOfEntity( vMappingOI,
                                    //:                  szEntityName )
                                    nRC = CheckExistenceOfEntity( vMappingOI, szEntityName );
                                 }

                                 //:END
                                 //:IF nRC = 0
                                 if ( nRC == 0 )
                                 {
                                    //:nRC = GetStringFromAttributeByContext( szWorkString,
                                    //:                           vMappingOI,
                                    //:                           szEntityName,
                                    //:                           szAttributeName,
                                    //:                           szContextName,
                                    //:                           32764 )
                                    nRC = GetStringFromAttributeByContext( szWorkString, vMappingOI, szEntityName, szAttributeName, szContextName, 32764 );
                                    //:IF nRC < 0
                                    if ( nRC < 0 )
                                    {
                                       //:szWorkString = "Report cannot get View.Entity.Attribute.Context: " +
                                       //:   szViewName + "." + szEntityName + "." + szAttributeName + "." + szContextName
                                       ZeidonStringCopy( szWorkString, 1, 0, "Report cannot get View.Entity.Attribute.Context: ", 1, 0, 32767 );
                                       ZeidonStringConcat( szWorkString, 1, 0, szViewName, 1, 0, 32767 );
                                       ZeidonStringConcat( szWorkString, 1, 0, ".", 1, 0, 32767 );
                                       ZeidonStringConcat( szWorkString, 1, 0, szEntityName, 1, 0, 32767 );
                                       ZeidonStringConcat( szWorkString, 1, 0, ".", 1, 0, 32767 );
                                       ZeidonStringConcat( szWorkString, 1, 0, szAttributeName, 1, 0, 32767 );
                                       ZeidonStringConcat( szWorkString, 1, 0, ".", 1, 0, 32767 );
                                       ZeidonStringConcat( szWorkString, 1, 0, szContextName, 1, 0, 32767 );
                                       //:TraceLineS( szWorkString, "" )
                                       TraceLineS( szWorkString, "" );
                                       //:szWorkString = ""
                                       ZeidonStringCopy( szWorkString, 1, 0, "", 1, 0, 32767 );
                                    }

                                    //:END
                                    //:ELSE
                                 }
                                 else
                                 {
                                    //:szWorkString = ""
                                    ZeidonStringCopy( szWorkString, 1, 0, "", 1, 0, 32767 );
                                 }

                                 //:END
                              }

                              //:END
                              //:ELSE
                           }
                           else
                           {
                              //:szWorkString = "Report cannot get view by name for View.Entity.Attribute: " +
                              //:            szViewName + "." + szEntityName + "." + szAttributeName
                              ZeidonStringCopy( szWorkString, 1, 0, "Report cannot get view by name for View.Entity.Attribute: ", 1, 0, 32767 );
                              ZeidonStringConcat( szWorkString, 1, 0, szViewName, 1, 0, 32767 );
                              ZeidonStringConcat( szWorkString, 1, 0, ".", 1, 0, 32767 );
                              ZeidonStringConcat( szWorkString, 1, 0, szEntityName, 1, 0, 32767 );
                              ZeidonStringConcat( szWorkString, 1, 0, ".", 1, 0, 32767 );
                              ZeidonStringConcat( szWorkString, 1, 0, szAttributeName, 1, 0, 32767 );
                              //:TraceLineS( szWorkString, "" )
                              TraceLineS( szWorkString, "" );
                              //:TraceLineS( szWorkString, "" )
                              TraceLineS( szWorkString, "" );
                           }

                           //:// szWorkString = ""
                           //:END
                           //:ELSE
                        }
                        else
                        {
                           //:szWorkString = vReportDef.Control.Text
                           GetVariableFromAttribute( szWorkString, 0, 'S', 32767, vReportDef, "Control", "Text", "", 0 );
                        }

                        //:END

                        //:// TraceLineS( " --> Single Line Case : ", "" )
                        //:// TraceLineS( " --> Text for control : ", szWorkString )
                        //:lMultiLineSizeY = 0
                        lMultiLineSizeY = 0;

                        //:// lPagePosEndX = lPagePosX + vReportDef.Control.SZDLG_X
                        //:// lPagePosEndY = lPagePosY + vReportDef.Control.SZDLG_Y

                        //://          // Prior to putting the Text on the report, we will check if the
                        //://          // text is to be right justified (the Control Subtype is 16777216,
                        //://          // which is zCONTROL_RIGHTJUSTIFY).
                        //:// we now   IF vReportDef.Control.Subtype = 167772
                        //:// determine      // the final parameter defines the type of justification.
                        //:// justification  // 1 = right justify
                        //:// above          // 2 = center justify

                        //:fnSetCtrlPrintValues( vReportDef, szWorkString, lPageNbr,
                        //:          lPagePosX, lPagePosY )
                        oTZRPSRCO_fnSetCtrlPrintValues( vReportDef, szWorkString, lPageNbr, lPagePosX, lPagePosY );

                        //:IF nHide = 1
                        if ( nHide == 1 )
                        {
                           //:szWorkString = ""
                           ZeidonStringCopy( szWorkString, 1, 0, "", 1, 0, 32767 );
                        }

                        //:END

                        //:IF lPrintHTML = 1
                        if ( lPrintHTML == 1 )
                        {
                           //:IF nHide = 0
                           if ( nHide == 0 )
                           {
                              //:szClass = vReportDef.Control.CSS_Class
                              GetVariableFromAttribute( szClass, 0, 'S', 257, vReportDef, "Control", "CSS_Class", "", 0 );
                              //:IF szClass != ""
                              if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) != 0 )
                              {
                                 //:szYN = vReportDef.Control.CSS_Font
                                 GetVariableFromAttribute( szYN, 0, 'S', 33, vReportDef, "Control", "CSS_Font", "", 0 );
                                 //:IF szYN = "Y"
                                 if ( ZeidonStringCompare( szYN, 1, 0, "Y", 1, 0, 33 ) == 0 )
                                 {
                                    //:lType = lType + 16
                                    lType = lType + 16;
                                 }

                                 //:END
                                 //:szYN = vReportDef.Control.CSS_Position
                                 GetVariableFromAttribute( szYN, 0, 'S', 33, vReportDef, "Control", "CSS_Position", "", 0 );
                                 //:IF szYN = "Y"
                                 if ( ZeidonStringCompare( szYN, 1, 0, "Y", 1, 0, 33 ) == 0 )
                                 {
                                    //:lType = lType + 32
                                    lType = lType + 32;
                                 }

                                 //:END
                                 //:szYN = vReportDef.Control.CSS_Size
                                 GetVariableFromAttribute( szYN, 0, 'S', 33, vReportDef, "Control", "CSS_Size", "", 0 );
                                 //:IF szYN = "Y"
                                 if ( ZeidonStringCompare( szYN, 1, 0, "Y", 1, 0, 33 ) == 0 )
                                 {
                                    //:lType = lType + 64
                                    lType = lType + 64;
                                 }

                                 //:END
                              }

                              //:END

                              //:PrintTextToHTML( vSubtask, vMappingOI, szWorkString,
                              //:     szClass, lPagePosX, lPagePosY,
                              //:     lPagePosEndX, lPagePosEndY, lJustify,
                              //:     lBorderStyle, lBorderColor, lEscapement,
                              //:     lType )
                              PrintTextToHTML( vSubtask, vMappingOI, szWorkString, szClass, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lJustify, lBorderStyle, lBorderColor, lEscapement, lType );
                           }

                           //:END
                           //:ELSE
                        }
                        else
                        {
                           //:// It could still be multi-line with no Mapping.
                           //:IF nMultiLineFlag = 1
                           if ( nMultiLineFlag == 1 )
                           {

                              //:lType = lType + 1
                              lType = lType + 1;
                              //:lJustify = lJustify + 512  // add word break flag
                              lJustify = lJustify + 512;

                              //:lWork = zstrlen( szWorkString )
                              lWork = zstrlen( szWorkString );
                              //:DrawMultiLineTextToReport( vSubtask, vMappingOI,
                              //:            szWorkString, lWork,
                              //:            lPagePosX, lPagePosY,
                              //:            lPagePosEndX, lPagePosEndY,
                              //:            lJustify, lBorderStyle,
                              //:            lBorderColor, lEscapement,
                              //:            lType, lEndPosY, lNextWord )
                              DrawMultiLineTextToReport( vSubtask, vMappingOI, szWorkString, lWork, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lJustify, lBorderStyle, lBorderColor, lEscapement, lType, &lEndPosY, &lNextWord );

                              //:ELSE
                           }
                           else
                           {

                              //:IF nMultiLineFlag = 2  // Fixed height
                              if ( nMultiLineFlag == 2 )
                              {
                                 //:lType = 2
                                 lType = 2;
                              }

                              //:END

                              //:DrawTextToReport( vSubtask, vMappingOI, szWorkString,
                              //:   lPagePosX, lPagePosY,
                              //:   lPagePosEndX, lPagePosEndY, lJustify,
                              //:   lBorderStyle, lBorderColor, lEscapement,
                              //:   lType )
                              DrawTextToReport( vSubtask, vMappingOI, szWorkString, lPagePosX, lPagePosY, lPagePosEndX, lPagePosEndY, lJustify, lBorderStyle, lBorderColor, lEscapement, lType );
                           }

                           //:END
                        }

                        //:END

                        //:vReportDef.Control.wPrintStatus = 3  // zRCONTROL_STATUS_PRINTED
                        SetAttributeFromInteger( vReportDef, "Control", "wPrintStatus", 3 );
                     }

                     //://          END
                     //:END

                     //:// The following section of code will set the remaining space
                     //:// on the page. The tricky part here is that more than one
                     //:// control can be on the same the same line (i.e.  overlap other
                     //:// controls in the Y direction).  The algorithm goes like this:
                     //://
                     //:// If the next control is on the same line as the current control
                     //://    If control is the first control on the line
                     //://       Get RemainingSpaceForCurrentLine
                     //://    Otherwise, there was a previous control on the line
                     //://       Take min of previous RemainSpaceForCurrentLine and
                     //://       RemainingSpace based on current control
                     //:// Otherwise, the current control is the last on the current line
                     //://    If control is the first control on the line
                     //://       Get RemainingSpaceForPage
                     //://    Otherwise, there was a previous control on the line
                     //://       Take min of previous RemainSpaceForCurrentLine and
                     //://       RemainingSpace based on current control
                     //://
                     //:// NOTE: this code assumes that there will only be one multi-line
                     //:// text control in any set of overlapping controls.

                     //:SetViewFromView( vReportTmp, vReportDef )
                     SetViewFromView( vReportTmp, vReportDef );
                     //:IF SetCursorNextEntity( vReportTmp,
                     //:            "Control", "" ) = zCURSOR_SET AND
                     //:vReportTmp.Control.RangeBottom = lRangeBottom
                     lTempInteger_17 = SetCursorNextEntity( vReportTmp, "Control", "" );
                     if ( lTempInteger_17 == zCURSOR_SET && CompareAttributeToInteger( vReportTmp, "Control", "RangeBottom", lRangeBottom ) == 0 )
                     {

                        //:lAdjust = vReportDef.Control.PSDLG_Y + vReportDef.Control.SZDLG_Y
                        GetIntegerFromAttribute( &lTempInteger_18, vReportDef, "Control", "PSDLG_Y" );
                        GetIntegerFromAttribute( &lTempInteger_19, vReportDef, "Control", "SZDLG_Y" );
                        lAdjust = lTempInteger_18 + lTempInteger_19;
                        //:lRemainingSpaceForCurrentLine = lRemainingSpaceOnPageY -
                        //:         (lRangeBottom - lAdjust) - lMultiLineSizeY
                        lRemainingSpaceForCurrentLine = lRemainingSpaceOnPageY - ( lRangeBottom - lAdjust ) - lMultiLineSizeY;

                        //:ELSE
                     }
                     else
                     {

                        //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                        //://UNDEF        TraceLineI( "FormatGroupOnPage Change RemainingSpaceOnPage: ",
                        //://UNDEF                    lRemainingSpaceOnPageY )
                        //://UNDEF        TraceLineI( "                   lRemainingSpaceOnPageStart: ",
                        //://UNDEF                    lRemainingSpaceOnPageStart )
                        //://UNDEF        TraceLineI( "                                     MaxSizeY: ",
                        //://UNDEF                    lMaxSizeY )
                        //://UNDEF        TraceLineI( "                                Next MaxSizeY: ",
                        //://UNDEF                    vReportTmp.Control.RangeBottom )
                        //://UNDEF     END

                        //:// lRemainingSpaceOnPageY = lRemainingSpaceOnPageY - lMaxSizeY
                        //:lRemainingSpaceOnPageY = lRemainingSpaceOnPageStart - lMaxSizeY
                        lRemainingSpaceOnPageY = lRemainingSpaceOnPageStart - lMaxSizeY;
                        //:lMultiLineSizeY = 0
                        lMultiLineSizeY = 0;
                        //:lRemainingSpaceForCurrentLine = 0
                        lRemainingSpaceForCurrentLine = 0;
                        //:bTopOfPage = FALSE   // if adjusting RemainingSpace, then
                        bTopOfPage = FALSE;
                     }

                     //:            // can't be at top

                     //://UNDEF     IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
                     //://UNDEF        TraceLineI( "                    New  RemainingSpaceOnPage: ",
                     //://UNDEF                    lRemainingSpaceOnPageY )
                     //://UNDEF     END

                     //:END
                  }

                  //:END // else of SkipLastPage
               }

               //:END // else
            }

            //:END // else
         }

         //:END // else
      }

      //:END // else of: IF lPrintHTML = 1 OR lPrintCSV = 1

      //:// increment loop
      //:nLoop1 = SetCursorNextEntity( vReportDef, "Control", "" )
      nLoop1 = SetCursorNextEntity( vReportDef, "Control", "" );
   }


   //:END   // LOOP WHILE nLoop1 = zCURSOR_SET

   //:IF lPrintCSV = 1
   if ( lPrintCSV == 1 )
   {
      //:IF vReportDef.Control EXISTS
      lTempInteger_20 = CheckExistenceOfEntity( vReportDef, "Control" );
      if ( lTempInteger_20 == 0 )
      {
         //:PrintTextToCSV( vSubtask, 0, 0 )  // end of line
         PrintTextToCSV( vSubtask, 0, 0 );
      }

      //:END
   }

   //:END

   //:nRptExitEvent = zRGE_EVENT_POSTPRINT
   nRptExitEvent = zRGE_EVENT_POSTPRINT;
   //:nRptExitReturn = InvokeReportExitCode( vSubtask, vReportDef,
   //:                                       szRptExitOper, nRptExitEvent,
   //:                                       lPageNbr, vSourceOI )
   nRptExitReturn = InvokeReportExitCode( vSubtask, vReportDef, szRptExitOper, nRptExitEvent, lPageNbr, vSourceOI );
   //:fnSetGroupCtrlStatus( vReportDef, zRCONTROL_STATUS_GROUPINACTIVE )
   oTZRPSRCO_fnSetGroupCtrlStatus( vReportDef, zRCONTROL_STATUS_GROUPINACTIVE );
   //:// DropViewTrace( "Dropping view (23): ", vReportTmp )
   //:DropView( vReportTmp )
   DropView( vReportTmp );
   //:vReportTmp = 0
   vReportTmp = 0;

   //:IF vSourceOI != 0
   if ( vSourceOI != 0 )
   {
      //://UNDVV IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
      //://UNDVV    TraceLineV( "(FormatGroupOnPage.end) DropView( vSourceOI ): ",
      //://UNDVV                vSourceOI )
      //://UNDVV END
      //:// DropViewTrace( "Dropping view (24): ", vSourceOI )
      //:DropView( vSourceOI )
      DropView( vSourceOI );
      //:vSourceOI = 0
      vSourceOI = 0;
   }

   //:END

   //:IF nRptExitReturn = zRGE_RETURN_NEWPAGE
   if ( nRptExitReturn == zRGE_RETURN_NEWPAGE )
   {
      //://UNDEF TraceLineI( "Exit Forced New Page after Group at PosY: ",
      //://UNDEF             lCurrentPosY )

      //:lRemainingSpaceOnPageY = 0
      lRemainingSpaceOnPageY = 0;
      //:plNewPosY = lFooterPosPageBottom
      *plNewPosY = lFooterPosPageBottom;

      //:ELSE
   }
   else
   {
      //:plNewPosY = lOrigPosY + vReportDef.Group.SZDLG_Y +
      //:            lMultiLineAdjustment + lMultiLineIncrement
      GetIntegerFromAttribute( &lTempInteger_21, vReportDef, "Group", "SZDLG_Y" );
      *plNewPosY = lOrigPosY + lTempInteger_21 + lMultiLineAdjustment + lMultiLineIncrement;
   }

   //:END

   //://UNDEF IF (lPageNbr >= 1 AND lPageNbr <= 4)/*TracePages*/
   //://UNDEF    TraceLineS( "FormatGroupOnPage returning 0", "" )
   //://UNDEF END

   //://UNDEF TraceLineI( "Formatting GroupSet/Group NewPosY: ", plNewPosY )
   //://UNDEF TraceLineS( "GroupSet: ", szRptGroupSetTag )
   //://UNDEF TraceLineS( "   Group: ", szRptGroupTag )

   //:// TraceLineI( "Adding PagePos: ", lPagePosMaxX );
   //:// plNewPosX = plNewPosX + lPagePosMaxX
   //:plNewPosX = lPagePosMaxX
   *plNewPosX = lPagePosMaxX;

   //:RETURN 0
   return( 0 );
// END
}


//:LOCAL OPERATION
//:ProcessPgDetailHeadrs( VIEW    vReportDef BASED ON LOD TZRPSRCO,
//:                       VIEW    vSourceHierOI,
//:                       STRING ( 32 ) szCurrentEntityName,
//:                       VIEW    vSubtask,
//:                       INTEGER plNewPosX,
//:                       INTEGER plNewPosY,
//:                       INTEGER lCurrentPosX,
//:                       INTEGER lCurrentPosY,
//:                       INTEGER lRemainingSpaceOnPageY,
//:                       INTEGER lBottomOfPage,
//:                       INTEGER lPageWidth,
//:                       INTEGER lPageHeight )

//:   VIEW        vHeaderDef BASED ON LOD TZRPSRCO
static zSHORT
oTZRPSRCO_ProcessPgDetailHeadrs( zVIEW     vReportDef,
                                 zVIEW     vSourceHierOI,
                                 zPCHAR    szCurrentEntityName,
                                 zVIEW     vSubtask,
                                 zPLONG    plNewPosX,
                                 zPLONG    plNewPosY,
                                 zLONG     lCurrentPosX,
                                 zLONG     lCurrentPosY,
                                 zPLONG    lRemainingSpaceOnPageY,
                                 zLONG     lBottomOfPage,
                                 zLONG     lPageWidth,
                                 zLONG     lPageHeight )
{
   zVIEW     vHeaderDef = 0;
   //:VIEW        vSourceOI
   zVIEW     vSourceOI = 0;
   //:STRING (32) szParentEntityName
   zCHAR     szParentEntityName[ 33 ] = { 0 };
   zSHORT    RESULT;


   //:plNewPosY = lCurrentPosY
   *plNewPosY = lCurrentPosY;

   //:// We will use a temporary view for vSourceHierOI because the operation,
   //:// GetStringFromAttributeByContext calls operations that will not allow
   //:// a hierarchical cursor.
   //:IF vSourceHierOI = 0
   if ( vSourceHierOI == 0 )
   {
      //:RETURN
      return( 0 );
   }

   //:END

   //:// CreateViewTrace( "Creating view (26): ", vSourceOI, vSourceHierOI, 0 )
   //:CreateViewFromViewForTask( vSourceOI, vSourceHierOI, 0 )
   CreateViewFromViewForTask( &vSourceOI, vSourceHierOI, 0 );
   //:MiGetParentEntityNameForView( szParentEntityName, vSourceOI,
   //:                              szCurrentEntityName )
   MiGetParentEntityNameForView( szParentEntityName, vSourceOI, szCurrentEntityName );
   //:// DropViewTrace( "Dropping view (30): ", vSourceOI )
   //:DropView( vSourceOI )
   DropView( vSourceOI );
   //:vSourceOI = 0
   vSourceOI = 0;

   //:IF szParentEntityName != ""
   if ( ZeidonStringCompare( szParentEntityName, 1, 0, "", 1, 0, 33 ) != 0 )
   {

      //:ProcessPgDetailHeadrs( vReportDef, vSourceHierOI,
      //:                       szParentEntityName, vSubtask,
      //:                       plNewPosX, plNewPosY,
      //:                       lCurrentPosX, lCurrentPosY,
      //:                       lRemainingSpaceOnPageY, lBottomOfPage,
      //:                       lPageWidth, lPageHeight )
      oTZRPSRCO_ProcessPgDetailHeadrs( vReportDef, vSourceHierOI, szParentEntityName, vSubtask, plNewPosX, plNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, lBottomOfPage, lPageWidth, lPageHeight );
      //:lCurrentPosY = plNewPosY
      lCurrentPosY = *plNewPosY;
      //:lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY
      *lRemainingSpaceOnPageY = lBottomOfPage - lCurrentPosY;

      //:// CreateViewTrace( "Creating view (27): ", vHeaderDef, vReportDef, 0 )
      //:CreateViewFromViewForTask( vHeaderDef, vReportDef, 0 )
      CreateViewFromViewForTask( &vHeaderDef, vReportDef, 0 );
      //:SET CURSOR FIRST vHeaderDef.GroupSet WHERE
      //:                 vHeaderDef.GroupSet.Tag = szCurrentEntityName
      RESULT = SetCursorFirstEntityByString( vHeaderDef, "GroupSet", "Tag", szCurrentEntityName, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      {
         //:SET CURSOR FIRST vHeaderDef.Group WHERE
         //:                 vHeaderDef.Group.Type = "gh"
         RESULT = SetCursorFirstEntityByString( vHeaderDef, "Group", "Type", "gh", "" );
         //:ELSE
      }
      else
      {
         //:TraceLineS( "ProcessPgDetailHeadrs unable to locate GroupSet: ",
         //:            szCurrentEntityName )
         TraceLineS( "ProcessPgDetailHeadrs unable to locate GroupSet: ", szCurrentEntityName );
      }

      //:END

      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      {
         //:FormatGroupOnPage( vHeaderDef, vSourceHierOI, vSubtask,
         //:                   plNewPosX, plNewPosY,
         //:                   lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY,
         //:                   0,   // continued Group flag 0 = not continued
         //:                   lPageWidth, lPageHeight, lRemainingSpaceOnPageY,
         //:                   lBottomOfPage )
         oTZRPSRCO_FormatGroupOnPage( vHeaderDef, vSourceHierOI, vSubtask, plNewPosX, plNewPosY, lCurrentPosX, lCurrentPosY, *lRemainingSpaceOnPageY, 0, lPageWidth, lPageHeight, *lRemainingSpaceOnPageY, lBottomOfPage );
         //:vReportDef.Report.CurrentEntityName = szCurrentEntityName
         SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szCurrentEntityName );
      }

      //:END

      //:// DropViewTrace( "Dropping view (31): ", vHeaderDef )
      //:DropView( vHeaderDef )
      DropView( vHeaderDef );
      //:vHeaderDef = 0
      vHeaderDef = 0;
   }

   //:END
   return( 0 );
// END
}


//:LOCAL OPERATION
//:fnSetGroupCtrlStatus( VIEW vReportDefIn BASED ON LOD TZRPSRCO,
//:                      SHORT nStatus )

//:   VIEW vReportDef BASED ON LOD TZRPSRCO
static zSHORT
oTZRPSRCO_fnSetGroupCtrlStatus( zVIEW     vReportDefIn,
                                zSHORT    nStatus )
{
   zVIEW     vReportDef = 0;
   zSHORT    RESULT;


   //:// CreateViewTrace( "Creating view (28): ", vReportDef, vReportDefIn, 0 )
   //:CreateViewFromViewForTask( vReportDef, vReportDefIn, 0 )
   CreateViewFromViewForTask( &vReportDef, vReportDefIn, 0 );

   //:FOR EACH vReportDef.Control
   RESULT = SetCursorFirstEntity( vReportDef, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      //:vReportDef.Control.wPrintStatus = nStatus
      SetAttributeFromInteger( vReportDef, "Control", "wPrintStatus", nStatus );
      RESULT = SetCursorNextEntity( vReportDef, "Control", "" );
   }

   //:END

   //:// DropViewTrace( "Dropping view (32): ", vReportDef )
   //:DropView( vReportDef )
   DropView( vReportDef );
   //:vReportDef = 0
   vReportDef = 0;
   return( 0 );
// END
}


//:LOCAL OPERATION
//:fnPageStateAdd( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                INTEGER lPageNbr )

//:   VIEW vReportDef2 BASED ON LOD TZRPSRCO
static zSHORT
oTZRPSRCO_fnPageStateAdd( zVIEW     vReportDef,
                          zLONG     lPageNbr )
{
   zVIEW     vReportDef2 = 0;
   //:VIEW vTempView
   zVIEW     vTempView = 0;
   //:INTEGER nPrevPageNbr
   zLONG     nPrevPageNbr = 0;
   //:INTEGER lTempView
   zLONG     lTempView = 0;
   //:INTEGER bValidView
   zLONG     bValidView = 0;
   zSHORT    RESULT;


   //:IF lPageNbr <= 0
   if ( lPageNbr <= 0 )
   {
      //:// The state for page number zero will never change.
      //:CREATE ENTITY vReportDef.PageState FIRST
      RESULT = CreateEntity( vReportDef, "PageState", zPOS_FIRST );
      //:vReportDef.PageState.PageNbr = 0
      SetAttributeFromInteger( vReportDef, "PageState", "PageNbr", 0 );
      //:vReportDef.PageState.StartLevel = 0
      SetAttributeFromInteger( vReportDef, "PageState", "StartLevel", 0 );
      //:vReportDef.PageState.LastLevel = 0
      SetAttributeFromInteger( vReportDef, "PageState", "LastLevel", 0 );
      //:vReportDef.PageState.HierReturnCode = zCURSOR_NULL
      SetAttributeFromInteger( vReportDef, "PageState", "HierReturnCode", zCURSOR_NULL );
      //:vReportDef.PageState.CurrentEntityName =
      //:                               vReportDef.Report.WK_SubobjectEntityName
      SetAttributeFromAttribute( vReportDef, "PageState", "CurrentEntityName", vReportDef, "Report", "WK_SubobjectEntityName" );
      //:vReportDef.PageState.AbsolutePos = -1
      SetAttributeFromInteger( vReportDef, "PageState", "AbsolutePos", -1 );
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:fnPageStateRemove( vReportDef, lPageNbr )
   oTZRPSRCO_fnPageStateRemove( vReportDef, lPageNbr );
   //:nPrevPageNbr = lPageNbr - 1
   nPrevPageNbr = lPageNbr - 1;
   //:SET CURSOR FIRST vReportDef.PageState
   //:       WHERE vReportDef.PageState.PageNbr = nPrevPageNbr
   RESULT = SetCursorFirstEntityByInteger( vReportDef, "PageState", "PageNbr", nPrevPageNbr, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   {
      //:TraceLineI( "Internal Error: fnPageStateAdd() cannot find page ",
      //:            nPrevPageNbr )
      TraceLineI( "Internal Error: fnPageStateAdd() cannot find page ", nPrevPageNbr );
      //:RETURN -1
      return( -1 );
   }

   //:END

   //:// Initialize state for this page
   //:// CreateViewTrace( "Creating view (29): ", vReportDef2, vReportDef, 0 )
   //:CreateViewFromViewForTask( vReportDef2, vReportDef, 0 )
   CreateViewFromViewForTask( &vReportDef2, vReportDef, 0 );
   //:CREATE ENTITY vReportDef.PageState LAST
   RESULT = CreateEntity( vReportDef, "PageState", zPOS_LAST );
   //:vReportDef.PageState.PageNbr = lPageNbr
   SetAttributeFromInteger( vReportDef, "PageState", "PageNbr", lPageNbr );
   //:vReportDef.PageState.MultiLineTextPosStartPage =
   //:           vReportDef2.PageState.MultiLineTextPosEndPage
   SetAttributeFromAttribute( vReportDef, "PageState", "MultiLineTextPosStartPage", vReportDef2, "PageState", "MultiLineTextPosEndPage" );
   //:vReportDef.PageState.ContinuedGroupFlagStartPage =
   //:           vReportDef2.PageState.ContinuedGroupFlagEndPage
   SetAttributeFromAttribute( vReportDef, "PageState", "ContinuedGroupFlagStartPage", vReportDef2, "PageState", "ContinuedGroupFlagEndPage" );
   //:vReportDef.PageState.ContinuedControlTagStartPage =
   //:           vReportDef2.PageState.ContinuedControlTagEndPage
   SetAttributeFromAttribute( vReportDef, "PageState", "ContinuedControlTagStartPage", vReportDef2, "PageState", "ContinuedControlTagEndPage" );
   //:FOR EACH vReportDef2.LastHeaderEntityAtLevel
   RESULT = SetCursorFirstEntity( vReportDef2, "LastHeaderEntityAtLevel", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      //:CREATE ENTITY vReportDef.LastHeaderEntityAtLevel
      RESULT = CreateEntity( vReportDef, "LastHeaderEntityAtLevel", zPOS_AFTER );
      //:SetMatchingAttributesByName( vReportDef,  "LastHeaderEntityAtLevel",
      //:                             vReportDef2, "LastHeaderEntityAtLevel",
      //:                             zSET_NULL )
      SetMatchingAttributesByName( vReportDef, "LastHeaderEntityAtLevel", vReportDef2, "LastHeaderEntityAtLevel", zSET_NULL );
      RESULT = SetCursorNextEntity( vReportDef2, "LastHeaderEntityAtLevel", "" );
   }

   //:END
   //:FOR EACH vReportDef2.FooterAtLevel
   RESULT = SetCursorFirstEntity( vReportDef2, "FooterAtLevel", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      //:CREATE ENTITY vReportDef.FooterAtLevel
      RESULT = CreateEntity( vReportDef, "FooterAtLevel", zPOS_AFTER );
      //:SetMatchingAttributesByName( vReportDef,  "FooterAtLevel",
      //:                             vReportDef2, "FooterAtLevel",
      //:                             zSET_NULL )
      SetMatchingAttributesByName( vReportDef, "FooterAtLevel", vReportDef2, "FooterAtLevel", zSET_NULL );

      //:// Make sure that the new entity has an independent view to
      //:// eliminate DropView problems later.
      //:lTempView = vReportDef.FooterAtLevel.SourceOI_Hier
      GetIntegerFromAttribute( &lTempView, vReportDef, "FooterAtLevel", "SourceOI_Hier" );
      //:MakeViewFromInteger( vTempView, lTempView )
      MakeViewFromInteger( &vTempView, lTempView );

      //:bValidView = IsValidView( vTempView )
      bValidView = IsValidView( vTempView );
      //:IF bValidView = FALSE
      if ( bValidView == FALSE )
      {
         //:bValidView = bValidView / bValidView
         bValidView = bValidView / bValidView;
      }

      //:END

      //:// CreateViewTrace( "Creating view (30): ", vTempView, vTempView, 0 )
      //:CreateViewFromViewForTask( vTempView, vTempView, 0 )
      CreateViewFromViewForTask( &vTempView, vTempView, 0 );
      //:lTempView = MakeIntegerFromView( vTempView )
      lTempView = MakeIntegerFromView( vTempView );
      //:vReportDef.FooterAtLevel.SourceOI_Hier = lTempView
      SetAttributeFromInteger( vReportDef, "FooterAtLevel", "SourceOI_Hier", lTempView );
      RESULT = SetCursorNextEntity( vReportDef2, "FooterAtLevel", "" );
   }

   //:END

   //:// DropViewTrace( "Dropping view (34): ", vReportDef2 )
   //:DropView( vReportDef2 )
   DropView( vReportDef2 );
   //:vReportDef2 = 0
   vReportDef2 = 0;

   //:SET CURSOR LAST vReportDef.PageState
   RESULT = SetCursorLastEntity( vReportDef, "PageState", "" );

   //:RETURN 0
   return( 0 );
// END
}


//:TRANSFORMATION OPERATION
//:FormatReportGroupSets( VIEW    vSubtask,
//:                       INTEGER lPageNbr,
//:                       INTEGER lPageWidth,
//:                       INTEGER lPageHeight )


//:   STRING ( 256 ) szViewName
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_FormatReportGroupSets( zVIEW     vSubtask,
                                 zLONG     lPageNbr,
                                 zLONG     lPageWidth,
                                 zLONG     lPageHeight )
{
   zCHAR     szViewName[ 257 ] = { 0 };
   //:STRING ( 32 )  szSubobjectEntityName
   zCHAR     szSubobjectEntityName[ 33 ] = { 0 };
   //:VIEW     vSourceOI
   zVIEW     vSourceOI = 0;
   //:VIEW     vReportDef
   zVIEW     vReportDef = 0;
   //:INTEGER  lPrintFlags
   zLONG     lPrintFlags = 0;
   //:INTEGER  lCurrentPosX
   zLONG     lCurrentPosX = 0;
   //:INTEGER  lCurrentPosY
   zLONG     lCurrentPosY = 0;
   //:INTEGER  lNewPosX
   zLONG     lNewPosX = 0;
   //:INTEGER  lNewPosY
   zLONG     lNewPosY = 0;
   //:INTEGER  lRemainingSpaceOnPageY
   zLONG     lRemainingSpaceOnPageY = 0;
   //:INTEGER  lBottomOfPage
   zLONG     lBottomOfPage = 0;
   //:SHORT    nPrintMultiple
   zSHORT    nPrintMultiple = 0;
   //:SHORT    nRC
   zSHORT    nRC = 0;
   zSHORT    RESULT;


   //:IF lPageNbr <= 0  // we only care about page 1
   if ( lPageNbr <= 0 )
   {
      //:RETURN 0
      return( 0 );
   }

   //:END

   //:lRemainingSpaceOnPageY = 2147483647  // BIG Number!!!
   lRemainingSpaceOnPageY = 2147483647;

   //:szViewName = "TZ_ReportDef"
   ZeidonStringCopy( szViewName, 1, 0, "TZ_ReportDef", 1, 0, 257 );
   //:GetViewByName( vReportDef, szViewName, vSubtask, zLEVEL_ANY )
   GetViewByName( &vReportDef, szViewName, vSubtask, zLEVEL_ANY );
   //:CreateViewFromViewForTask( vReportDef, vReportDef, vSubtask )
   CreateViewFromViewForTask( &vReportDef, vReportDef, vSubtask );
   //:ResetView( vReportDef )
   ResetView( vReportDef );

   //:szSubobjectEntityName = vReportDef.Report.WK_SubobjectEntityName
   GetVariableFromAttribute( szSubobjectEntityName, 0, 'S', 33, vReportDef, "Report", "WK_SubobjectEntityName", "", 0 );
   //:lPrintFlags = vReportDef.Report.PrintFlags
   GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );

   //:// The state for page number zero will never change.
   //:fnPageStateAdd( vReportDef, 0 )
   oTZRPSRCO_fnPageStateAdd( vReportDef, 0 );

   //:szViewName = "TZ_SourceOrigOI"
   ZeidonStringCopy( szViewName, 1, 0, "TZ_SourceOrigOI", 1, 0, 257 );
   //:GetViewByName( vSourceOI, szViewName, vSubtask, zLEVEL_ANY )
   GetViewByName( &vSourceOI, szViewName, vSubtask, zLEVEL_ANY );
   //:CreateViewFromViewForTask( vSourceOI, vSourceOI, vSubtask )
   CreateViewFromViewForTask( &vSourceOI, vSourceOI, vSubtask );

   //:nRC = IsFlagSequenceSet( lPrintFlags, 8 ) // print multiple entities
   nRC = IsFlagSequenceSet( lPrintFlags, 8 );
   //:IF nRC > 0
   if ( nRC > 0 )
   {
      //:nPrintMultiple = 1
      nPrintMultiple = 1;
      //:nRC = SetCursorFirstEntity( vSourceOI, szSubobjectEntityName, 0 )
      nRC = SetCursorFirstEntity( vSourceOI, szSubobjectEntityName, 0 );
      //:ELSE
   }
   else
   {
      //:nRC = 1
      nRC = 1;
   }

   //:END

   //:LOOP WHILE nRC >= 0
   while ( nRC >= 0 )
   {

      //:// Process each Page entity.
      //:SET CURSOR FIRST vReportDef.Page
      RESULT = SetCursorFirstEntity( vReportDef, "Page", "" );
      //:LOOP WHILE RESULT >= 0
      while ( RESULT >= 0 )
      {

         //:// Process each GroupSet entity.
         //:SET CURSOR FIRST vReportDef.GroupSet
         RESULT = SetCursorFirstEntity( vReportDef, "GroupSet", "" );
         //:LOOP WHILE RESULT >= 0
         while ( RESULT >= 0 )
         {

            //:// Process each Group entity.
            //:SET CURSOR FIRST vReportDef.Group
            RESULT = SetCursorFirstEntity( vReportDef, "Group", "" );
            //:LOOP WHILE RESULT >= 0
            while ( RESULT >= 0 )
            {

               //:FormatGroupOnPage( vReportDef, vSourceOI, vSubtask,
               //:                   lNewPosX, lNewPosY,
               //:                   lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY,
               //:                   32768, // continued Group flag 0x8000 = 32768 ==> WebPageReport
               //:                   lPageWidth, lPageHeight, lRemainingSpaceOnPageY,
               //:                   lBottomOfPage )
               oTZRPSRCO_FormatGroupOnPage( vReportDef, vSourceOI, vSubtask, &lNewPosX, &lNewPosY, lCurrentPosX, lCurrentPosY, lRemainingSpaceOnPageY, 32768, lPageWidth, lPageHeight, lRemainingSpaceOnPageY, lBottomOfPage );
               //:SET CURSOR NEXT vReportDef.Group
               RESULT = SetCursorNextEntity( vReportDef, "Group", "" );
            }

            //:END

            //:SET CURSOR NEXT vReportDef.GroupSet
            RESULT = SetCursorNextEntity( vReportDef, "GroupSet", "" );
         }

         //:END

         //:SET CURSOR NEXT vReportDef.Page
         RESULT = SetCursorNextEntity( vReportDef, "Page", "" );
      }

      //:END

      //:IF nPrintMultiple = 1
      if ( nPrintMultiple == 1 )
      {
         //:nRC = SetCursorNextEntity( vSourceOI, szSubobjectEntityName, 0 )
         nRC = SetCursorNextEntity( vSourceOI, szSubobjectEntityName, 0 );
         //:ELSE
      }
      else
      {
         //:nRC = -1
         nRC = -1;
      }

      //:END
   }

   //:END

   //:DropView( vSourceOI )
   DropView( vSourceOI );
   //:DropView( vReportDef )
   DropView( vReportDef );
   //:RETURN 0
   return( 0 );
// END
}



#ifdef __cplusplus
}
#endif
