#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

//:GLOBAL OPERATION
//:JSP_StaticSection1( INTEGER lFile,
//:                    STRING ( 10000 ) szWriteBuffer,
//:                    VIEW vDialog BASED ON LOD TZWDLGSO )

//:   STRING ( 64 )    szFormName
zOPER_EXPORT zSHORT OPERATION
JSP_StaticSection1( zLONG     lFile,
                    zPCHAR    szWriteBuffer,
                    zVIEW     vDialog )
{
   zCHAR     szFormName[ 65 ] = { 0 }; 


   //:   // STATIC SECTION

   //:szWriteBuffer = "zeidon.zView vAppSubtask; // View to this subtask"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "zeidon.zView vAppSubtask; // View to this subtask", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "zeidon.zView vKZXMLPGO = new zeidon.zView( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "zeidon.zView vKZXMLPGO = new zeidon.zView( );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "String strActionToProcess;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "String strActionToProcess;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "String strURL;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "String strURL;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );

   //:szWriteBuffer = "String strErrorMapValue;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "String strErrorMapValue;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "String strErrorColor;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "String strErrorColor;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "String strErrorMsg = '';"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "String strErrorMsg = '';", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "String strErrorFlag;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "String strErrorFlag;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "String strErrorValue;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "String strErrorValue;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );


   //:szWriteBuffer = "public String FindErrorFields( )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "public String FindErrorFields( )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "{"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "{", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   zeidon.zView vMsgQ = new zeidon.zView( ); // View to Message Queue"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   zeidon.zView vMsgQ = new zeidon.zView( ); // View to Message Queue", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   zeidon.zView v = new zeidon.zView( );     // View to Message Queue"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   zeidon.zView v = new zeidon.zView( );     // View to Message Queue", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "   String strErr;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   String strErr;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   int    iView;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   int    iView;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   int    nRC = 0;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   int    nRC = 0;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   short  nRepos = 0;  // zREPOS_NONE    0"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   short  nRepos = 0;  // zREPOS_NONE    0", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "                       // zREPOS_FIRST   1"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "                       // zREPOS_FIRST   1", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "                       // zREPOS_LAST    2"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "                       // zREPOS_LAST    2", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "                       // zREPOS_NEXT    3"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "                       // zREPOS_NEXT    3", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "                       // zREPOS_PREV    4"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "                       // zREPOS_PREV    4", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "   strErr = '';"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   strErr = '';", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   strErrorValue = '';"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   strErrorValue = '';", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   strErrorMsg = '';"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   strErrorMsg = '';", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:// szWriteBuffer = "   vAppSubtask.TraceLine( 'FindErrorFields for jsp =======>> ', '" + szFormName + "' );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:szWriteBuffer = "   iView = vAppSubtask.GetIntegerFromView( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   iView = vAppSubtask.GetIntegerFromView( );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   nRC = vMsgQ.GetView( '__MSGQ', vAppSubtask );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   nRC = vMsgQ.GetView( '__MSGQ', vAppSubtask );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   if ( nRC > 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( nRC > 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:// szWriteBuffer = "      vAppSubtask.TraceLine( 'FindErrorFields found __MSGQ', '' );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:szWriteBuffer = "      v.CreateViewFromView( vMsgQ );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      v.CreateViewFromView( vMsgQ );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:// szWriteBuffer = "      vAppSubtask.TraceLine( 'FindErrorFields __MSGQ View: ', iView );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:szWriteBuffer = "      nRC = v.SetCursorFirst( 'Task', 'Id', iView, '' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = v.SetCursorFirst( 'Task', 'Id', iView, '' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:// szWriteBuffer = "      vAppSubtask.TraceLine( 'FindErrorFields SetCursorFirst Task RC: ', nRC );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:szWriteBuffer = "      if ( nRC == 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC == 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         nRC = v.SetCursorFirst( 'QMsg' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = v.SetCursorFirst( 'QMsg' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:// szWriteBuffer = "         vAppSubtask.TraceLine( 'FindErrorFields SetCursorFirst RC: ', nRC );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:szWriteBuffer = "         if ( nRC == 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC == 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "            strErrorValue = vMsgQ.GetString( 'QMsg', 'ErrorMapValue' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "            strErrorValue = vMsgQ.GetString( 'QMsg', 'ErrorMapValue' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "            strErrorMsg = vMsgQ.GetString( 'QMsg', 'Text' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "            strErrorMsg = vMsgQ.GetString( 'QMsg', 'Text' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "            strErr = 'Y';"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "            strErr = 'Y';", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "         while ( nRC == 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         while ( nRC == 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         // v.DisplayEntityInstance( 'QMsg' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         // v.DisplayEntityInstance( 'QMsg' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "            v.DeleteEntity( 'QMsg', nRepos );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "            v.DeleteEntity( 'QMsg', nRepos );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "            nRC = v.SetCursorFirst( 'QMsg' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "            nRC = v.SetCursorFirst( 'QMsg' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         // nRC = v.SetCursorNext( 'QMsg' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         // nRC = v.SetCursorNext( 'QMsg' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         // vAppSubtask.TraceLine( 'FindErrorFields SetCursorNext RC: ', nRC );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         // vAppSubtask.TraceLine( 'FindErrorFields SetCursorNext RC: ', nRC );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:// szWriteBuffer = "         vAppSubtask.TraceLine( 'FindErrorFields ERROR ===> ', strErrorValue );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:szWriteBuffer = "      }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "      v.DropView( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      v.DropView( );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   // alert( strErrorValue );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   // alert( strErrorValue );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:// szWriteBuffer = "   vAppSubtask.TraceLine( 'FindErrorFields for jsp =======>> ', strErrorValue );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )

   //:szWriteBuffer = "   return( strErr );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   return( strErr );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "}"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "}", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "public String CheckError( String strWidgetCurr, String strErrorList )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "public String CheckError( String strWidgetCurr, String strErrorList )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "{"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "{", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   String strWidget;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   String strWidget;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   String strMapValue;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   String strMapValue;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   String str;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   String str;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   String strError;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   String strError;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "   int nPos;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   int nPos;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   int nLth;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   int nLth;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "   nPos = strErrorList.indexOf( strWidgetCurr );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   nPos = strErrorList.indexOf( strWidgetCurr );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   if ( nPos < 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( nPos < 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      return( '' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      return( '' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "   vAppSubtask.TraceLine( 'CheckError looking for Widget ====================>> ', strWidgetCurr );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vAppSubtask.TraceLine( 'CheckError looking for Widget ====================>> ', strWidgetCurr );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   vAppSubtask.TraceLine( '                          in ErrorList ===========>> ', strErrorList );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vAppSubtask.TraceLine( '                          in ErrorList ===========>> ', strErrorList );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   strError = strErrorList;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   strError = strErrorList;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   nPos = strError.indexOf( '\t' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   nPos = strError.indexOf( '\\t' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   while ( nPos >= 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   while ( nPos >= 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      strWidget = strError.substring( 0, nPos );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      strWidget = strError.substring( 0, nPos );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      nLth = strError.length( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      nLth = strError.length( );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      str = strError.substring( nPos + 1, nLth );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      str = strError.substring( nPos + 1, nLth );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   // vAppSubtask.TraceLine( 'CheckError TempStr ====>> ', str );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   // vAppSubtask.TraceLine( 'CheckError TempStr ====>> ', str );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      nPos = str.indexOf( '\t' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      nPos = str.indexOf( '\\t' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      strMapValue = str.substring( 0, nPos );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      strMapValue = str.substring( 0, nPos );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      nLth = str.length( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      nLth = str.length( );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      strError = str.substring( nPos + 1, nLth );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      strError = str.substring( nPos + 1, nLth );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   // vAppSubtask.TraceLine( 'CheckError Remaining ====>> ', strErrorList );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   // vAppSubtask.TraceLine( 'CheckError Remaining ====>> ', strErrorList );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      nPos = strError.indexOf( '\t' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      nPos = strError.indexOf( '\\t' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:// szWriteBuffer = "      vAppSubtask.TraceLine( 'CheckError Current ======>> ', strWidgetCurr );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:// szWriteBuffer = "      vAppSubtask.TraceLine( 'CheckError Widget =======>> ', strWidget );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:// szWriteBuffer = "      vAppSubtask.TraceLine( 'CheckError Value ========>> ', strMapValue );"
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   //:szWriteBuffer = "   // if ( strWidgetCurr.indexOf( strWidget ) >= 0 &&"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   // if ( strWidgetCurr.indexOf( strWidget ) >= 0 &&", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   //      strWidget.indexOf( strWidgetCurr ) >= 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   //      strWidget.indexOf( strWidgetCurr ) >= 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      if ( strWidgetCurr.equals( strWidget ) )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( strWidgetCurr.equals( strWidget ) )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         vAppSubtask.TraceLine( 'CheckError Found Widget ********>> ', strWidget );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         vAppSubtask.TraceLine( 'CheckError Found Widget ********>> ', strWidget );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "         vAppSubtask.TraceLine( 'CheckError Found Value  ********>> ', strMapValue );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         vAppSubtask.TraceLine( 'CheckError Found Value  ********>> ', strMapValue );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "         return( strMapValue );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         return( strMapValue );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "// strMapValue = '';"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "// strMapValue = '';", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   return( '' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   return( '' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "}"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "}", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );
   return( 0 );
// END
} 


//:GLOBAL OPERATION
//:JSP_StaticMainSection( INTEGER lFile,
//:                       STRING ( 10000 ) szWriteBuffer,
//:                       VIEW vDialog BASED ON LOD TZWDLGSO )

//:   VIEW vLPLR        BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
JSP_StaticMainSection( zLONG     lFile,
                       zPCHAR    szWriteBuffer,
                       zVIEW     vDialog )
{
   zVIEW     vLPLR = 0; 
   zCHAR     szTempString_0[ 33 ]; 


   //:// MAIN SECTION HEADER

   //:GetViewByName( vLPLR, "TaskLPLR", vDialog, zLEVEL_TASK )
   GetViewByName( &vLPLR, "TaskLPLR", vDialog, zLEVEL_TASK );

   //:szWriteBuffer = "<%"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "zeidon.zView vMsgQ = new zeidon.zView( ); // View to Message Queue"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "zeidon.zView vMsgQ = new zeidon.zView( ); // View to Message Queue", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "String strLastPage;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "String strLastPage;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "int nOptRC;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "int nOptRC;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "int nRC = 0;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "int nRC = 0;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:// vAppSubtask
   //:szWriteBuffer = "// Check to see if the Zeidon subtask view already exists.  If not, create"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "// Check to see if the Zeidon subtask view already exists.  If not, create", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "// it and copy it into the application object."
   ZeidonStringCopy( szWriteBuffer, 1, 0, "// it and copy it into the application object.", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "vAppSubtask = (zeidon.zView) session.getAttribute( 'ZeidonSubtask' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "vAppSubtask = (zeidon.zView) session.getAttribute( 'ZeidonSubtask' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "if ( vAppSubtask == null )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "if ( vAppSubtask == null )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "{"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "{", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   vAppSubtask = new zeidon.zView( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vAppSubtask = new zeidon.zView( );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   vAppSubtask.RegisterZeidonApplication( '//" + vLPLR.LPLR.Name + "' );"
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vLPLR, "LPLR", "Name", "", 0 );
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vAppSubtask.RegisterZeidonApplication( '//", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_0, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   session.setAttribute( 'ZeidonSubtask', vAppSubtask );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   session.setAttribute( 'ZeidonSubtask', vAppSubtask );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "}"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "}", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:// vKZXMLPGO
   //:szWriteBuffer = "nRC = vKZXMLPGO.GetView( 'KZXMLPGO', vAppSubtask );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "nRC = vKZXMLPGO.GetView( 'KZXMLPGO', vAppSubtask );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "if ( nRC <= 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "if ( nRC <= 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "{"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "{", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   vKZXMLPGO.InitWebSessionObject( vAppSubtask, 'TestUserID' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vKZXMLPGO.InitWebSessionObject( vAppSubtask, 'TestUserID' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   vKZXMLPGO.SetName( 'KZXMLPGO' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vKZXMLPGO.SetName( 'KZXMLPGO' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "}"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "}", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:// Temporary traces to understand how Error Object is used.
   //:szWriteBuffer = "nRC = vMsgQ.GetView( '__MSGQ', vAppSubtask );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "nRC = vMsgQ.GetView( '__MSGQ', vAppSubtask );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "if ( nRC > 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "if ( nRC > 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   vAppSubtask.TraceLine( '***Message Object Check: FOUND __MSGQ', '' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vAppSubtask.TraceLine( '***Message Object Check: FOUND __MSGQ', '' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "else"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "else", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   vAppSubtask.TraceLine( '***Message Object Check: NOT found __MSGQ', '' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vAppSubtask.TraceLine( '***Message Object Check: NOT found __MSGQ', '' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "vAppSubtask.SetDefaultViewForActiveTask( 2 );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "vAppSubtask.SetDefaultViewForActiveTask( 2 );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:// MAIN SECTION ACTION PROCESSING

   //:szWriteBuffer = "strActionToProcess = (String) request.getParameter( 'zAction' );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "strActionToProcess = (String) request.getParameter( 'zAction' );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:szWriteBuffer = "if ( strActionToProcess != null )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "if ( strActionToProcess != null )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "{"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "{", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );

   //:szWriteBuffer = "   vAppSubtask.TraceLine( 'Process Action: ', strActionToProcess );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   vAppSubtask.TraceLine( 'Process Action: ', strActionToProcess );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );

   //:// Process Error Object.
   //:szWriteBuffer = "   // Delete the message object if error on last interation."
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   // Delete the message object if error on last interation.", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   nRC = vMsgQ.GetView( '__MSGQ', vAppSubtask );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   nRC = vMsgQ.GetView( '__MSGQ', vAppSubtask );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "   if ( nRC > 0 )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( nRC > 0 )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 0 );
   //:szWriteBuffer = "      vMsgQ.DropObjectInstance( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      vMsgQ.DropObjectInstance( );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "'", 1 );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
