#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "ZDRVROPR.H"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZEIDONOP.H"

static zSHORT
oTZRPSRCO_GenerateReportJSP( zVIEW     vReportDef,
                             zVIEW     vSubtask,
                             zVIEW     vSourceOI,
                             zPCHAR    szReportDefName,
                             zPCHAR    szSubobjectEntityName,
                             zLONG     lPrintPreviewFlag,
                             zLONG     lPrintDialogFlag,
                             zLONG     lPrintFlags ) ;


//:TRANSFORMATION OPERATION
static zSHORT
oTZRPSRCO_GenerateReportJSP( zVIEW     vReportDef,
                             zVIEW     vSubtask,
                             zVIEW     vSourceOI,
                             zPCHAR    szReportDefName,
                             zPCHAR    szSubobjectEntityName,
                             zLONG     lPrintPreviewFlag,
                             zLONG     lPrintDialogFlag,
                             zLONG     lPrintFlags )
{
   zSHORT    nZRetCode;
   zLONG     lZRetCode;

   //:GenerateReportJSP( VIEW    vReportDef BASED ON LOD TZRPSRCO,
   //:                VIEW    vSubtask,
   //:                VIEW    vSourceOI,
   //:                STRING ( 32 ) szReportDefName,
   //:                STRING ( 32 ) szSubobjectEntityName,
   //:                INTEGER lPrintPreviewFlag,
   //:                INTEGER lPrintDialogFlag,
   //:                INTEGER lPrintFlags )

   //:NAME VIEW vSourceOI  "TZ_SourceOrigOI"
   nZRetCode = SetNameForView( vSourceOI, "TZ_SourceOrigOI", 0, zLEVEL_TASK );
   //:NAME VIEW vReportDef "TZ_ReportDef"
   nZRetCode = SetNameForView( vReportDef, "TZ_ReportDef", 0, zLEVEL_TASK );

   //:// These values are stored for future reference ... the only place
   //:// they are set.
   //:vReportDef.Report.WK_SubobjectEntityName = szSubobjectEntityName
   nZRetCode = SetAttributeFromString( vReportDef, "Report",
      "WK_SubobjectEntityName", szSubobjectEntityName );
   //:vReportDef.Report.PrintFlags = lPrintFlags
   nZRetCode = SetAttributeFromInteger( vReportDef, "Report", "PrintFlags",
      lPrintFlags );

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
   //:OpenReport( vSubtask, "KZRPSRCO", "oTZRPSRCO_FormatDocPage",
   //:            lPrintFlags, TRUE, TRUE, lPrintDialogFlag,
   //:            szReportDefName, szSubobjectEntityName )
   lZRetCode = OpenReport( vSubtask, "KZRPSRCO", "oTZRPSRCO_FormatDocPage",
      lPrintFlags, TRUE, TRUE, lPrintDialogFlag, szReportDefName,
      szSubobjectEntityName );
   return( 0 );
// END
}



#ifdef __cplusplus
}
#endif
