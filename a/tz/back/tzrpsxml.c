#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZRPSRCO_XML_PageHeaderFooter( zVIEW     vReportDef,
                                zVIEW     vSourceOI,
                                zPCHAR    szOutputLine,
                                zLONG     lFileHandle,
                                zPCHAR    szIndentationValue );


static zSHORT
oTZRPSRCO_GenerateXMLLine( zVIEW     ReportObject,
                           zLONG     lFileHandle,
                           zPCHAR    szOutputLine );


static zSHORT
oTZRPSRCO_GenerateXML_CtlRecurs( zVIEW     vReportDef,
                                 zVIEW     vSourceOI,
                                 zPCHAR    szDrivingObjectViewName,
                                 zPCHAR    szOutputLine,
                                 zLONG     lFileHandle,
                                 zPCHAR    szIndentationValue );


//:TRANSFORMATION OPERATION
//:GenerateXSLT_XML( VIEW vSourceOI,
//:                  STRING ( 32 )  szReportName,
//:                  STRING ( 500 ) szXMLFileName,
//:                  STRING ( 32 )  szTopEntityName )

//:   VIEW vReportDef     BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_GenerateXSLT_XML( zVIEW     vSourceOI,
                            zPCHAR    szReportName,
                            zPCHAR    szXMLFileName,
                            zPCHAR    szTopEntityName )
{
   zVIEW     vReportDef = 0; 
   //:VIEW vReportLODRecurs BASED ON LOD TZRPSRCO
   zVIEW     vReportLODRecurs = 0; 
   //:VIEW vReportDefRoot BASED ON LOD TZRPSRCO
   zVIEW     vReportDefRoot = 0; 
   //:VIEW vSourceOIHier
   zVIEW     vSourceOIHier = 0; 
   //:VIEW vSourceOIDebug
   zVIEW     vSourceOIDebug = 0; 
   //:VIEW vLOD           BASED ON LOD TZZOLODO
   zVIEW     vLOD = 0; 
   //:VIEW vTaskLPLR  BASED ON LOD TZCMLPLO
   zVIEW     vTaskLPLR = 0; 
   //:STRING ( 200 )  szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 200 )  szFileName
   zCHAR     szFileName[ 201 ] = { 0 }; 
   //:STRING ( 32 )   szReturnedEntityName
   zCHAR     szReturnedEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )   szLastReturnedEntityName
   zCHAR     szLastReturnedEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )   szParentEntityName
   zCHAR     szParentEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )   szDrivingViewName
   zCHAR     szDrivingViewName[ 33 ] = { 0 }; 
   //:STRING ( 50 )   szIndentationValue
   zCHAR     szIndentationValue[ 51 ] = { 0 }; 
   //:STRING ( 50 )   szIndentationSubValue
   zCHAR     szIndentationSubValue[ 51 ] = { 0 }; 
   //:STRING ( 1 )    szInNodeFlag
   zCHAR     szInNodeFlag[ 2 ] = { 0 }; 
   //:STRING ( 5000 ) szOutputLine
   zCHAR     szOutputLine[ 5001 ] = { 0 }; 
   //:INTEGER         lFileHandle
   zLONG     lFileHandle = 0; 
   //:INTEGER         Indentation
   zLONG     Indentation = 0; 
   //:SHORT           ReturnedHierLevel
   zSHORT    ReturnedHierLevel = 0; 
   //:SHORT           LastHierLevel
   zSHORT    LastHierLevel = 0; 
   //:SHORT           nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 


   //:// Build an XSLT XML FO object for the passed in application object and Report Definition.

   //:// First we need to activate the report def...
   //:SysReadZeidonIni( -1, "[WorkStation]", "ResourcePath", szFileName )
   SysReadZeidonIni( -1, "[WorkStation]", "ResourcePath", szFileName, zsizeof( szFileName ) );
   //://szFileName = "c:\lplr\zencas\bin\"
   //:szFileName = szFileName + szReportName + ".xrp"
   ZeidonStringConcat( szFileName, 1, 0, szReportName, 1, 0, 201 );
   ZeidonStringConcat( szFileName, 1, 0, ".xrp", 1, 0, 201 );
   //:SfActivateSysOI_FromFile( vReportDef, "TZRPSRCO", vSourceOI, szFileName, zSINGLE )
   SfActivateSysOI_FromFile( &vReportDef, "TZRPSRCO", vSourceOI, szFileName, zSINGLE );

   //:// Get the Name of the Driving object, as we will need that in processing mapping.
   //:IF vReportDef.DrivingViewObjRef EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "DrivingViewObjRef" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szDrivingViewName = vReportDef.DrivingViewObjRef.Name
      GetVariableFromAttribute( szDrivingViewName, 0, 'S', 33, vReportDef, "DrivingViewObjRef", "Name", "", 0 );
      //:ELSE
   } 
   else
   { 
      //:szMsg = "The report must have a 'Driving View' set in the report details."
      ZeidonStringCopy( szMsg, 1, 0, "The report must have a 'Driving View' set in the report details.", 1, 0, 201 );
      //:MessageSend( vReportDef, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vReportDef, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Open XML output file.
   //:SysReadZeidonIni( -1, "[Workstation]", "XSLTDirectory", szFileName )
   SysReadZeidonIni( -1, "[Workstation]", "XSLTDirectory", szFileName, zsizeof( szFileName ) );
   //:szFileName = szFileName + szXMLFileName + ".xml"
   ZeidonStringConcat( szFileName, 1, 0, szXMLFileName, 1, 0, 201 );
   ZeidonStringConcat( szFileName, 1, 0, ".xml", 1, 0, 201 );
   //:lFileHandle = SysOpenFile( vReportDef, szFileName, COREFILE_WRITE )
   lFileHandle = SysOpenFile( vReportDef, szFileName, COREFILE_WRITE );
   //:IF lFileHandle < 0
   if ( lFileHandle < 0 )
   { 
      //:szMsg = "Cannot open XSLT Output File, " + szFileName
      ZeidonStringCopy( szMsg, 1, 0, "Cannot open XSLT Output File, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szFileName, 1, 0, 201 );
      //:MessageSend( vReportDef, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vReportDef, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Navigate the vSourceOI object/subobject hierarchically and create an XML node for each entity that has a corresponding
   //:// GroupSet entity in the Report Def.

   //:// Generate basic header.
   //:szOutputLine = "<?xml version=" + QUOTES + "1.0" + QUOTES + " encoding=" + QUOTES + "iso-8859-1" + QUOTES + "?>"
   ZeidonStringCopy( szOutputLine, 1, 0, "<?xml version=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "1.0", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " encoding=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "iso-8859-1", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "?>", 1, 0, 5001 );
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "<zOI>"
   ZeidonStringCopy( szOutputLine, 1, 0, "<zOI>", 1, 0, 5001 );
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

   //:// Process the page header. 
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PH"
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PH", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szOutputLine = "<PageHeader>"
      ZeidonStringCopy( szOutputLine, 1, 0, "<PageHeader>", 1, 0, 5001 );
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
      //:XML_PageHeaderFooter( vReportDef, vSourceOI, szOutputLine, lFileHandle, szIndentationValue )
      oTZRPSRCO_XML_PageHeaderFooter( vReportDef, vSourceOI, szOutputLine, lFileHandle, szIndentationValue );
      //:szOutputLine = "</PageHeader>"
      ZeidonStringCopy( szOutputLine, 1, 0, "</PageHeader>", 1, 0, 5001 );
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END

   //:szOutputLine = "   <GR_" + szTopEntityName + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "   <GR_", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, szTopEntityName, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

   //:// Process Top Entity Header, if it exists.
   //:CreateViewFromView( vReportLODRecurs, vReportDef )
   CreateViewFromView( &vReportLODRecurs, vReportDef );

   //:GenerateXML_LODRecurs( vReportDef, vReportLODRecurs, vSourceOI, szOutputLine, lFileHandle, szIndentationValue )
   oTZRPSRCO_GenerateXML_LODRecurs( vReportDef, vReportLODRecurs, vSourceOI, szOutputLine, lFileHandle, szIndentationValue );


   //:szOutputLine = "   </GR_" + szTopEntityName + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "   </GR_", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, szTopEntityName, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

   //:// Process the page footer. 
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PF"
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PF", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szOutputLine = "<PageFooter>"
      ZeidonStringCopy( szOutputLine, 1, 0, "<PageFooter>", 1, 0, 5001 );
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
      //:XML_PageHeaderFooter( vReportDef, vSourceOI, szOutputLine, lFileHandle, szIndentationValue )
      oTZRPSRCO_XML_PageHeaderFooter( vReportDef, vSourceOI, szOutputLine, lFileHandle, szIndentationValue );
      //:szOutputLine = "</PageFooter>"
      ZeidonStringCopy( szOutputLine, 1, 0, "</PageFooter>", 1, 0, 5001 );
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END

   //:// Terminate and close the XML file.
   //:szOutputLine = "</zOI>"
   ZeidonStringCopy( szOutputLine, 1, 0, "</zOI>", 1, 0, 5001 );
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
   //:SysCloseFile( vReportDef, lFileHandle, 0 )
   SysCloseFile( vReportDef, lFileHandle, 0 );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:XML_PageHeaderFooter( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                      VIEW vSourceOI,
//:                      STRING ( 5000 ) szOutputLine,
//:                      INTEGER         lFileHandle,
//:                      STRING ( 50 )   szIndentationValue )

//:   STRING ( 32 ) szEntityName
static zSHORT
oTZRPSRCO_XML_PageHeaderFooter( zVIEW     vReportDef,
                                zVIEW     vSourceOI,
                                zPCHAR    szOutputLine,
                                zLONG     lFileHandle,
                                zPCHAR    szIndentationValue )
{
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 50 ) szIndentationSubValue
   zCHAR     szIndentationSubValue[ 51 ] = { 0 }; 
   zSHORT    lTempInteger_0; 


   //:IF vReportDef.Control EXISTS 
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "Control" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szIndentationSubValue = szIndentationValue + "      "
      ZeidonStringCopy( szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
      ZeidonStringConcat( szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
      //:GenerateXML_CtlRecurs( vReportDef, vSourceOI, szEntityName, szOutputLine, lFileHandle, szIndentationSubValue )
      oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOI, szEntityName, szOutputLine, lFileHandle, szIndentationSubValue );
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenerateXML_LODRecurs( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                       VIEW vReportDefRecurs BASED ON LOD TZRPSRCO,
//:                       VIEW vSourceOI,
//:                       STRING ( 5000 ) szOutputLine,
//:                       INTEGER         lFileHandle,
//:                       STRING ( 50 )   szIndentationValue )

//:   STRING ( 32 ) szEntityName
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_GenerateXML_LODRecurs( zVIEW     vReportDef,
                                 zVIEW     vReportDefRecurs,
                                 zVIEW     vSourceOI,
                                 zPCHAR    szOutputLine,
                                 zLONG     lFileHandle,
                                 zPCHAR    szIndentationValue )
{
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 ) szDrivingViewName
   zCHAR     szDrivingViewName[ 33 ] = { 0 }; 
   //:STRING ( 50 ) szIndentationSubValue
   zCHAR     szIndentationSubValue[ 51 ] = { 0 }; 
   //:STRING ( 10 ) szCnt
   zCHAR     szCnt[ 11 ] = { 0 }; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 


   //:szDrivingViewName = vReportDef.DrivingViewObjRef.Name
   GetVariableFromAttribute( szDrivingViewName, 0, 'S', 33, vReportDef, "DrivingViewObjRef", "Name", "", 0 );

   //:// Build one level of the PartialReportEntity subobject, matching the structure of the Driving LOD and
   //:// setting the ReportDisplayFlag entity for any entity with a corresponding GroupSet entity in the report, or having
   //:// a PartialReportEntityChild entity with a corresponding GroupSet entity. The flag is set to "D" if the entity has
   //:// a corresponding GroupSet entity and to a "C" if it has a child with a corresponding GroupSet entity.

   //:FOR EACH vReportDefRecurs.PartialReportEntity 
   RESULT = SetCursorFirstEntity( vReportDefRecurs, "PartialReportEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:szEntityName = vReportDefRecurs.PartialReportEntity.Name 
      GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDefRecurs, "PartialReportEntity", "Name", "", 0 );
      //: 
      //:// If ReportDisplayFlag = "D" then this is an entity that is displayed on the report (there is a GroupSet).  
      //:// Otherwise, it's a parent entity where one of it's children is displayed on the report.                    
      //:IF vReportDefRecurs.PartialReportEntity.ReportDisplayFlag = "D"
      if ( CompareAttributeToString( vReportDefRecurs, "PartialReportEntity", "ReportDisplayFlag", "D" ) == 0 )
      { 

         //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szEntityName 
         RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szEntityName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
         } 

         //:   // There is an error because the report should have a GroupSet for this.          
         //:END

         //:nRC = SetCursorFirstEntity( vSourceOI, szEntityName, "" )
         nRC = SetCursorFirstEntity( vSourceOI, szEntityName, "" );

         //:// Process group header
         //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "gh"
         RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "gh", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szOutputLine = szIndentationValue + "   <" + szEntityName + "Header>"
            ZeidonStringCopy( szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "   <", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "Header>", 1, 0, 5001 );
            //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

            //:szIndentationSubValue = szIndentationValue + "      "
            ZeidonStringCopy( szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
            ZeidonStringConcat( szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
            //:GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue )
            oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue );

            //:szOutputLine = szIndentationValue + "   </" + szEntityName + "Header>"
            ZeidonStringCopy( szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "   </", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "Header>", 1, 0, 5001 );
            //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
         } 

         //:END

         //:// Go to generate an attribute entry for each attribute with mapping in the Detail Group of the GroupSet.
         //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "ga"
         RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "ga", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:         
            //:// Loop through vSourceOI for each entity
            //:LOOP WHILE nRC >= zCURSOR_SET                     
            while ( nRC >= zCURSOR_SET )
            { 

               //:szOutputLine = szIndentationValue + "   <" + szEntityName + ">"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "   <", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
               //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
               //:szIndentationSubValue = szIndentationValue + "   "
               ZeidonStringCopy( szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
               ZeidonStringConcat( szIndentationSubValue, 1, 0, "   ", 1, 0, 51 );

               //:GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue )
               oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue );

               //:// Check to see if there are child entities.
               //:IF vReportDefRecurs.PartialReportEntityChild EXISTS
               lTempInteger_0 = CheckExistenceOfEntity( vReportDefRecurs, "PartialReportEntityChild" );
               if ( lTempInteger_0 == 0 )
               { 

                  //:SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" )
                  SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" );
                  //:szIndentationSubValue = szIndentationValue + "      "
                  ZeidonStringCopy( szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
                  ZeidonStringConcat( szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
                  //:nRC = GenerateXML_LODRecurs( vReportDef,
                  //:                             vReportDefRecurs, vSourceOI, szOutputLine, lFileHandle, szIndentationSubValue )
                  nRC = oTZRPSRCO_GenerateXML_LODRecurs( vReportDef, vReportDefRecurs, vSourceOI, szOutputLine, lFileHandle, szIndentationSubValue );
                  //:ResetViewFromSubobject( vReportDefRecurs )
                  ResetViewFromSubobject( vReportDefRecurs );

                  //:// We need to reset on the report GroupSet/Group because it has been changed when we go down levels.
                  //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szEntityName
                  RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szEntityName, "" );
                  //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "ga"
                  RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "ga", "" );
               } 

               //:END

               //:// End of checking for child entities...               
               //:szOutputLine = szIndentationValue + "   </" + szEntityName + ">"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "   </", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
               //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

               //:nRC = SetCursorNextEntity( vSourceOI, szEntityName, "" )
               nRC = SetCursorNextEntity( vSourceOI, szEntityName, "" );
            } 

            //:END
         } 

         //:END

         //:// Process group footer.
         //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "gf"
         RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "gf", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szOutputLine = szIndentationValue + "   <" + szEntityName + "Footer>"
            ZeidonStringCopy( szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "   <", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "Footer>", 1, 0, 5001 );
            //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

            //:szIndentationSubValue = szIndentationValue + "      "
            ZeidonStringCopy( szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
            ZeidonStringConcat( szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
            //:GenerateXML_CtlRecurs( vReportDef, vSourceOI, szEntityName, szOutputLine, lFileHandle, szIndentationSubValue )
            oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOI, szEntityName, szOutputLine, lFileHandle, szIndentationSubValue );
            //:szOutputLine = szIndentationValue + "   </" + szEntityName + "Footer>"
            ZeidonStringCopy( szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "   </", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "Footer>", 1, 0, 5001 );
            //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
         } 

         //:END

         //:ELSE
      } 
      else
      { 
         //:// Maybe the parent isn't being displayed but a child might be, so we need to check...
         //:      
         //:// Loop through vSourceOI for each entity
         //:nRC = SetCursorFirstEntity( vSourceOI, szEntityName, "" )
         nRC = SetCursorFirstEntity( vSourceOI, szEntityName, "" );
         //:LOOP WHILE nRC >= zCURSOR_SET                     
         while ( nRC >= zCURSOR_SET )
         { 
            //:IF vReportDefRecurs.PartialReportEntityChild EXISTS
            lTempInteger_1 = CheckExistenceOfEntity( vReportDefRecurs, "PartialReportEntityChild" );
            if ( lTempInteger_1 == 0 )
            { 

               //:SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" )
               SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" );
               //:szIndentationSubValue = szIndentationValue + "      "
               ZeidonStringCopy( szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
               ZeidonStringConcat( szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
               //:nRC = GenerateXML_LODRecurs( vReportDef,
               //:                             vReportDefRecurs, vSourceOI, szOutputLine, lFileHandle, szIndentationSubValue )
               nRC = oTZRPSRCO_GenerateXML_LODRecurs( vReportDef, vReportDefRecurs, vSourceOI, szOutputLine, lFileHandle, szIndentationSubValue );
               //:ResetViewFromSubobject( vReportDefRecurs )
               ResetViewFromSubobject( vReportDefRecurs );
            } 

            //:   
            //:END
            //:nRC = SetCursorNextEntity( vSourceOI, szEntityName, "" )
            nRC = SetCursorNextEntity( vSourceOI, szEntityName, "" );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vReportDefRecurs, "PartialReportEntity", "" );
      //:END
   } 

   //:   
   //:END  // FOR EACH vReportDefRecurs.PartialReportEntity 
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
oTZRPSRCO_GenerateXMLLine( zVIEW     ReportObject,
                           zLONG     lFileHandle,
                           zPCHAR    szOutputLine )
{

   //:GenerateXMLLine( VIEW ReportObject BASED ON LOD TZRPSRCO,
   //:           INTEGER lFileHandle,
   //:           STRING ( 5000 ) szOutputLine )

   //:// This is just a SysWriteLine with an options TraceLineS statement.
   //://TraceLineS( "*** Line: ", szOutputLine )
   //:SysWriteLine( ReportObject, lFileHandle, szOutputLine )
   SysWriteLine( ReportObject, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:GenerateXML_CtlRecurs( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                       VIEW vSourceOI,
//:                       STRING ( 32 )   szDrivingObjectViewName,
//:                       STRING ( 5000 ) szOutputLine,
//:                       INTEGER         lFileHandle,
//:                       STRING ( 50 )   szIndentationValue )

//:   VIEW vMappingOI
static zSHORT
oTZRPSRCO_GenerateXML_CtlRecurs( zVIEW     vReportDef,
                                 zVIEW     vSourceOI,
                                 zPCHAR    szDrivingObjectViewName,
                                 zPCHAR    szOutputLine,
                                 zLONG     lFileHandle,
                                 zPCHAR    szIndentationValue )
{
   zVIEW     vMappingOI = 0; 
   //:STRING ( 32 )   szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )   szAttributeName
   zCHAR     szAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 )   szMappingName
   zCHAR     szMappingName[ 33 ] = { 0 }; 
   //:STRING ( 32 )   szContextName
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 5000 ) szReturnedAttributeValue
   zCHAR     szReturnedAttributeValue[ 5001 ] = { 0 }; 
   //:STRING ( 5000 ) szConvertedAttributeValue
   zCHAR     szConvertedAttributeValue[ 5001 ] = { 0 }; 
   //:STRING ( 200 )  szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 


   //:// Generate an attribute entry for each attribute with mapping in the GroupSet. Note that these are all at the same
   //:// hierarchical level since we're assuming no tables within a GroupSet. However, the controls in the GroupSet may well be
   //:// organized hierarchically and must thus be processed recursively.

   //:// Sort the Controls in position order so they will be in the same order as in the XSLT. (This probably isn't
   //:// necessary, but it will simply debugging.)
   //:OrderEntityForView( vReportDef, "Control", "PSDLG_Y A PSDLG_X A" )
   OrderEntityForView( vReportDef, "Control", "PSDLG_Y A PSDLG_X A" );

   //:// Process each Control. The processing rule is simple:
   //:// If the control has mapping, build an XML node.
   //:// If the control doesn't have mapping, ignore it.
   //:// The only issue is that we must step down a level if the Control has a subcontrol.

   //:FOR EACH vReportDef.Control
   RESULT = SetCursorFirstEntity( vReportDef, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vReportDef.CtrlCtrl EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "CtrlCtrl" );
      if ( lTempInteger_0 == 0 )
      { 
         //:SetViewToSubobject( vReportDef, "CtrlCtrl" )
         SetViewToSubobject( vReportDef, "CtrlCtrl" );
         //:GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingObjectViewName, szOutputLine, lFileHandle, szIndentationValue )
         oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingObjectViewName, szOutputLine, lFileHandle, szIndentationValue );
         //:ResetViewFromSubobject( vReportDef )
         ResetViewFromSubobject( vReportDef );
         //:ELSE
      } 
      else
      { 

         //:IF vReportDef.CtrlMapER_Attribute EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( vReportDef, "CtrlMapER_Attribute" );
         if ( lTempInteger_1 == 0 )
         { 
            //:// Format XML Line for Attribute.
            //:szReturnedAttributeValue = ""
            ZeidonStringCopy( szReturnedAttributeValue, 1, 0, "", 1, 0, 5001 );
            //:szConvertedAttributeValue = ""
            ZeidonStringCopy( szConvertedAttributeValue, 1, 0, "", 1, 0, 5001 );
            //:szAttributeName = vReportDef.CtrlMapER_Attribute.Name
            GetVariableFromAttribute( szAttributeName, 0, 'S', 33, vReportDef, "CtrlMapER_Attribute", "Name", "", 0 );
            //:szEntityName    = vReportDef.CtrlMapRelatedEntity.Name
            GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDef, "CtrlMapRelatedEntity", "Name", "", 0 );
            //:szContextName = ""
            ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
            //:IF vReportDef.CtrlMapContext EXISTS
            lTempInteger_2 = CheckExistenceOfEntity( vReportDef, "CtrlMapContext" );
            if ( lTempInteger_2 == 0 )
            { 
               //:szContextName = vReportDef.CtrlMapContext.Name  
               GetVariableFromAttribute( szContextName, 0, 'S', 33, vReportDef, "CtrlMapContext", "Name", "", 0 );
            } 

            //:END
            //:IF vReportDef.CtrlMapView.Name = szDrivingObjectViewName   // Check if mapping if from Driving Object View.
            if ( CompareAttributeToString( vReportDef, "CtrlMapView", "Name", szDrivingObjectViewName ) == 0 )
            { 
               //:GetStringFromAttributeByContext( szReturnedAttributeValue, vSourceOI, szEntityName, szAttributeName, szContextName, 5000 )
               GetStringFromAttributeByContext( szReturnedAttributeValue, vSourceOI, szEntityName, szAttributeName, szContextName, 5000 );
               //:ELSE
            } 
            else
            { 
               //:// Mapping is from separate view.
               //:szMappingName = vReportDef.CtrlMapView.Name
               GetVariableFromAttribute( szMappingName, 0, 'S', 33, vReportDef, "CtrlMapView", "Name", "", 0 );
               //:GET VIEW vMappingOI NAMED szMappingName
               RESULT = GetViewByName( &vMappingOI, szMappingName, vReportDef, zLEVEL_TASK );
               //:GetStringFromAttributeByContext( szReturnedAttributeValue, vMappingOI, szEntityName, szAttributeName, szContextName, 5000 )
               GetStringFromAttributeByContext( szReturnedAttributeValue, vMappingOI, szEntityName, szAttributeName, szContextName, 5000 );
            } 

            //:END

            //:ConvertXML_SpecialCharacters( vReportDef, szConvertedAttributeValue, szReturnedAttributeValue, 5000 )
            ConvertXML_SpecialCharacters( vReportDef, szConvertedAttributeValue, szReturnedAttributeValue, 5000 );

            //:szOutputLine = szIndentationValue + "  " + "<" + szEntityName + "." + szAttributeName + ">" + szConvertedAttributeValue
            ZeidonStringCopy( szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "<", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, ".", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szAttributeName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szConvertedAttributeValue, 1, 0, 5001 );
            //:szOutputLine = szOutputLine + "</" + szEntityName + "." + szAttributeName + ">"
            ZeidonStringConcat( szOutputLine, 1, 0, "</", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, ".", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szAttributeName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
            //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
         } 


         //:END
      } 

      RESULT = SetCursorNextEntity( vReportDef, "Control", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
