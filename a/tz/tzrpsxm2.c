#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
#include "TZVMLIP.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZRPSRCO_GenerateLine( zVIEW     ReportObject,
                        zLONG     lFileHandle,
                        zPCHAR    szOutputLine );


static zSHORT
oTZRPSRCO_ConvertPxToInchesCH( zVIEW     vReportDefRoot,
                               zPCHAR    ReturnedString,
                               zLONG     lValueInPixles );


//:TRANSFORMATION OPERATION
//:XSLT_BuildCtrlsInRows( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                   VIEW vReportDef     BASED ON LOD TZRPSRCO,
//:                   VIEW vLOD           BASED ON LOD TZZOLODO,
//:                   INTEGER lFileHandle,
//:                   STRING ( 5000 ) szOutputLine,
//:                   STRING ( 50 )   szIndentationChars )

//:   VIEW vReportDef3 BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_BuildCtrlsInRows( zVIEW     vReportDefRoot,
                                 zVIEW     vReportDef,
                                 zVIEW     vLOD,
                                 zLONG     lFileHandle,
                                 zPCHAR    szOutputLine,
                                 zPCHAR    szIndentationChars )
{
   zVIEW     vReportDef3 = 0; 
   //:STRING ( 20 )  szCurrentWidth
   zCHAR     szCurrentWidth[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szPageWidth
   zCHAR     szPageWidth[ 21 ] = { 0 }; 
   //:STRING ( 30 )  szCellPaddingName
   zCHAR     szCellPaddingName[ 31 ] = { 0 }; 
   //:STRING ( 50 )  szAttributeMappingName
   zCHAR     szAttributeMappingName[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szConstantArea
   zCHAR     szConstantArea[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szCellPadding
   zCHAR     szCellPadding[ 51 ] = { 0 }; 
   //:STRING ( 20 )  szTableMarginTop
   zCHAR     szTableMarginTop[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTablePaddingLeft
   zCHAR     szTablePaddingLeft[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTableWidth
   zCHAR     szTableWidth[ 21 ] = { 0 }; 
   //:INTEGER       CurrentWidth
   zLONG     CurrentWidth = 0; 
   //:INTEGER       CurrentPositionY
   zLONG     CurrentPositionY = 0; 
   //:INTEGER       LastPositionY
   zLONG     LastPositionY = 0; 
   //:INTEGER       FirstColumnPosition
   zLONG     FirstColumnPosition = 0; 
   //:INTEGER       TopGroupPosition
   zLONG     TopGroupPosition = 0; 
   //:INTEGER       ColumnCount
   zLONG     ColumnCount = 0; 
   //:INTEGER       TextColumnCount
   zLONG     TextColumnCount = 0; 
   //:INTEGER       RowCount
   zLONG     RowCount = 0; 
   //:INTEGER       TablePaddingLeft
   zLONG     TablePaddingLeft = 0; 
   //:INTEGER       TableMarginTop
   zLONG     TableMarginTop = 0; 
   //:INTEGER       TableWidth
   zLONG     TableWidth = 0; 
   //:INTEGER       LastRowPosition
   zLONG     LastRowPosition = 0; 
   //:INTEGER       LastRowHeight
   zLONG     LastRowHeight = 0; 
   zSHORT    RESULT; 


   //:szOutputLine = szIndentationChars + "<!-- In XSLT_BuildCtrlsInRows -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<!-- In XSLT_BuildCtrlsInRows -->", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Process a Group that is only made up of text Controls at the first level, as opposed to text controls that are subcontrols
   //:// under a parent Group or Shape.
   //:// All Rpt_Text controls are analyzed and organized by rows, with each row being handled as a single row table.

   //:// We assume the controls are text.

   //:szCellPaddingName = "DefaultCellPadding"
   ZeidonStringCopy( szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 31 );
   //:NAME VIEW vReportDef "vReportDefAllText"
   SetNameForView( vReportDef, "vReportDefAllText", 0, zLEVEL_TASK );

   //:// Process Row/Column Structure to generate Tables and Columns.

   //:TopGroupPosition = vReportDefRoot.XSLT_WorkColumn.PSDLG_Y
   GetIntegerFromAttribute( &TopGroupPosition, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_Y" );
   //:RowCount = 0
   RowCount = 0;

   //:FOR EACH vReportDefRoot.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Build Row & Cell statements. Each work row becomes a single table with a single row.

      //:RowCount = RowCount + 1
      RowCount = RowCount + 1;

      //:// Go to build the table.
      //:XSLT_GroupTable( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars, RowCount, "" )
      oTZRPSRCO_XSLT_GroupTable( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars, RowCount, "" );
      RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   } 


   //:END

   //:// Remove the Work Rows.
   //:FOR EACH vReportDefRoot.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:DELETE ENTITY vReportDefRoot.XSLT_WorkRow NONE
      RESULT = DeleteEntity( vReportDefRoot, "XSLT_WorkRow", zREPOS_NONE );
      RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   } 

   //:END
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
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_BuildBlockData( zVIEW     vReportDefRoot,
                               zVIEW     vReportDef,
                               zVIEW     vLOD,
                               zLONG     lFileHandle,
                               zPCHAR    szOutputLine,
                               zPCHAR    szIndentationChars,
                               zLONG     GroupHierarchicalLevel )
{
   zCHAR     szPaddingTop[ 21 ] = { 0 }; 
   //:STRING ( 256 ) szAttributeMappingName
   zCHAR     szAttributeMappingName[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szConstantArea
   zCHAR     szConstantArea[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szStyle
   zCHAR     szStyle[ 257 ] = { 0 }; 
   //:STRING ( 10 )  szLineWidth
   zCHAR     szLineWidth[ 11 ] = { 0 }; 
   //:INTEGER        PaddingTop
   zLONG     PaddingTop = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 


   //:szOutputLine = szIndentationChars + "<!-- In XSLT_BuildBlockData -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<!-- In XSLT_BuildBlockData -->", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// KJS 12/13/12 - Don't think I need to set cursor because we do that before we get here...
   //:// Block and data statement is different depending on whether the ControlDef is Text or Graphic and whether or not there is mapping for the Control.
   //:SET CURSOR FIRST vReportDef.Control WHERE vReportDef.Control.ZKey = vReportDefRoot.XSLT_WorkColumn.ControlZKey
   GetIntegerFromAttribute( &lTempInteger_0, vReportDefRoot, "XSLT_WorkColumn", "ControlZKey" );
   RESULT = SetCursorFirstEntityByInteger( vReportDef, "Control", "ZKey", lTempInteger_0, "" );
   //:// KJS 11/28/12
   //:IF RESULT >= zCURSOR_SET 
   if ( RESULT >= zCURSOR_SET )
   { 

      //:XSLT_BuildControl( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars )
      oTZRPSRCO_XSLT_BuildControl( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars );
   } 


   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_BuildControl( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                   VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                   INTEGER lFileHandle,
//:                   STRING ( 5000 ) szOutputLine,
//:                   STRING ( 50 )  szIndentationChars )

//:   STRING ( 20 )  szPaddingTop
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_BuildControl( zVIEW     vReportDefRoot,
                             zVIEW     vReportDef,
                             zLONG     lFileHandle,
                             zPCHAR    szOutputLine,
                             zPCHAR    szIndentationChars )
{
   zCHAR     szPaddingTop[ 21 ] = { 0 }; 
   //:STRING ( 256 ) szAttributeMappingName
   zCHAR     szAttributeMappingName[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szConstantArea
   zCHAR     szConstantArea[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szStyle
   zCHAR     szStyle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szStyleLeader
   zCHAR     szStyleLeader[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szAttribPrefix  // szAttribPrefix is only for the Page Header and Page Footer
   zCHAR     szAttribPrefix[ 257 ] = { 0 }; 
   //:STRING ( 10 )  szLineWidth
   zCHAR     szLineWidth[ 11 ] = { 0 }; 
   //:INTEGER        PaddingTop
   zLONG     PaddingTop = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:// This is for page heading/footing
   //://IF vReportDefRoot.XSLT_WorkGroup.RecursiveNumber = -1
   //:// WHEN WE ARE DOWN RECURSIVELY, WE CAN"T SEE Report. NOT SURE WHAT TO DO

   //:IF vReportDefRoot.Report.wOriginalViewName != ""   
   if ( CompareAttributeToString( vReportDefRoot, "Report", "wOriginalViewName", "" ) != 0 )
   { 
      //:szAttribPrefix = vReportDefRoot.Report.wOriginalViewName 
      GetVariableFromAttribute( szAttribPrefix, 0, 'S', 257, vReportDefRoot, "Report", "wOriginalViewName", "", 0 );
      //:ELSE
   } 
   else
   { 
      //:szAttribPrefix = ""
      ZeidonStringCopy( szAttribPrefix, 1, 0, "", 1, 0, 257 );
   } 

   //:END

   //:                
   //:IF vReportDef.ControlDef.Tag = "Rpt_PicGraphic"
   if ( CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_PicGraphic" ) == 0 )
   { 
      //:// Control is graphic.
      //:szConstantArea = vReportDef.Control.WebFileName 
      GetVariableFromAttribute( szConstantArea, 0, 'S', 257, vReportDef, "Control", "WebFileName", "", 0 );
      //:PaddingTop = vReportDef.Control.PSDLG_Y
      GetIntegerFromAttribute( &PaddingTop, vReportDef, "Control", "PSDLG_Y" );
      //:ConvertPxToInchesCH( vReportDef, szPaddingTop, PaddingTop )
      oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szPaddingTop, PaddingTop );
      //://szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + " > <!-- XSLT_BuildBlockData-->"
      //:szOutputLine = szIndentationChars + "<fo:block " + szStyle + " > <!-- XSLT_BuildControl-->"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "<fo:block ", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szStyle, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, " > <!-- XSLT_BuildControl-->", 1, 0, 5001 );
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + "  <fo:external-graphic src=" + QUOTES + szConstantArea + QUOTES +
      //:                                    " padding-top=" + QUOTES + szPaddingTop + "in" + QUOTES 
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "  <fo:external-graphic src=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szConstantArea, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, " padding-top=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szPaddingTop, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:                                    
      //:szOutputLine = szIndentationChars + "     content-width=" + QUOTES + "scale-to-fit" + QUOTES
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "     content-width=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "scale-to-fit", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + "     content-height=" + QUOTES + "scale-to-fit" + QUOTES
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "     content-height=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "scale-to-fit", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + "     width=" + QUOTES + "100%" + QUOTES + " scaling=" + QUOTES + "uniform" + QUOTES + "/>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "     width=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "100%", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, " scaling=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "uniform", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "/>", 1, 0, 5001 );
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + "</fo:block>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

      //:ELSE
   } 
   else
   { 
      //:IF vReportDef.ControlDef.Tag = "Rpt_Shape" 
      if ( CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_Shape" ) == 0 )
      { 
         //:// If we get here, this is a shape without anything inside. Don't see why this would happen but check for it anyway.         
         //://TraceLineS("**** XSLT_GetStyleShape *** ", "" )
         //:XSLT_GetStyleShape( vReportDef, szStyle )
         oTZRPSRCO_XSLT_GetStyleShape( vReportDef, szStyle );
         //:IF vReportDef.Control.BorderBottom = 1
         if ( CompareAttributeToInteger( vReportDef, "Control", "BorderBottom", 1 ) == 0 )
         { 
            //:szStyleLeader = "<fo:leader />"
            ZeidonStringCopy( szStyleLeader, 1, 0, "<fo:leader />", 1, 0, 257 );
            //:ELSE
         } 
         else
         { 
            //:szStyleLeader = ""
            ZeidonStringCopy( szStyleLeader, 1, 0, "", 1, 0, 257 );
         } 

         //:END
         //:szOutputLine = szIndentationChars + "      <fo:block " + szStyle + ">" + szStyleLeader + "</fo:block> <!-- XSLT_BuildControl Empty Shape -->"
         ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, "      <fo:block ", 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, szStyle, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, szStyleLeader, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, "</fo:block> <!-- XSLT_BuildControl Empty Shape -->", 1, 0, 5001 );
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
         //:ELSE 
      } 
      else
      { 
         //:// Control is considered Text.
         //:XSLT_GetStyleText( vReportDef, szStyle )
         oTZRPSRCO_XSLT_GetStyleText( vReportDef, szStyle );

         //:// Text control has mapping
         //:IF vReportDef.CtrlMapER_Attribute EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "CtrlMapER_Attribute" );
         if ( lTempInteger_0 == 0 )
         { 
            //:szAttributeMappingName = szAttribPrefix + vReportDef.CtrlMapRelatedEntity.Name + "." + vReportDef.CtrlMapER_Attribute.Name
            GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vReportDef, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringCopy( szAttributeMappingName, 1, 0, szAttribPrefix, 1, 0, 257 );
            ZeidonStringConcat( szAttributeMappingName, 1, 0, szTempString_0, 1, 0, 257 );
            ZeidonStringConcat( szAttributeMappingName, 1, 0, ".", 1, 0, 257 );
            GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vReportDef, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szAttributeMappingName, 1, 0, szTempString_1, 1, 0, 257 );
            //:szOutputLine = szIndentationChars + "      <fo:block " + szStyle + "><xsl:value-of select=" + QUOTES +
            //:            szAttributeMappingName + QUOTES + "/></fo:block> <!-- XSLT_BuildControl-->"
            ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "      <fo:block ", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szStyle, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "><xsl:value-of select=", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szAttributeMappingName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "/></fo:block> <!-- XSLT_BuildControl-->", 1, 0, 5001 );
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
            //:ELSE
         } 
         else
         { 

            //:IF vReportDef.Control.Tag = "PageNumber"
            if ( CompareAttributeToString( vReportDef, "Control", "Tag", "PageNumber" ) == 0 )
            { 
               //:// This is the PageNumber 
               //://szOutputLine = szIndentationChars + "      <fo:block " + szStyle + " text-align=" + QUOTES + "right" + QUOTES + "> <!-- XSLT_BuildControl -->"
               //:szOutputLine = szIndentationChars + "      <fo:block " + szStyle + "> <!-- XSLT_BuildControl -->"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "      <fo:block ", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, szStyle, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "> <!-- XSLT_BuildControl -->", 1, 0, 5001 );
               //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
               //:IF vReportDef.Report.CountPages = "Y"
               if ( CompareAttributeToString( vReportDef, "Report", "CountPages", "Y" ) == 0 )
               { 
                  //:szOutputLine = szIndentationChars + "      Page <fo:page-number/> of <fo:page-number-citation ref-id=" + QUOTES + "last-page" + QUOTES + "/>"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "      Page <fo:page-number/> of <fo:page-number-citation ref-id=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "last-page", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "/>", 1, 0, 5001 );
                  //:ELSE 
               } 
               else
               { 
                  //:szOutputLine = szIndentationChars + "      Page <fo:page-number/> "
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "      Page <fo:page-number/> ", 1, 0, 5001 );
               } 

               //:END
               //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

               //:szOutputLine = szIndentationChars + "      </fo:block>"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "      </fo:block>", 1, 0, 5001 );
               //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

               //:ELSE
            } 
            else
            { 
               //:// We are assuming that if this is a very thin text control, we must want there to be a line (should I not look at border?)
               //:IF vReportDef.Control.Text = "" AND vReportDef.Control.SZDLG_Y <= 21
               if ( CompareAttributeToString( vReportDef, "Control", "Text", "" ) == 0 && CompareAttributeToInteger( vReportDef, "Control", "SZDLG_Y", 21 ) <= 0 )
               { 
                  //:// Build a Leader line.
                  //:szOutputLine = szIndentationChars + "      <fo:block>  <!-- XSLT_BuildControl -->"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "      <fo:block>  <!-- XSLT_BuildControl -->", 1, 0, 5001 );
                  //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

                  //:szOutputLine = szIndentationChars + "      <fo:leader leader-pattern=" + QUOTES + "rule" + QUOTES +
                  //:         " leader-length.optimum=" + QUOTES + "100%" + QUOTES + " rule-style=" + QUOTES +
                  //:         "solid" + QUOTES + " rule-thickness=" + QUOTES + "1pt" + QUOTES + "/>"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "      <fo:leader leader-pattern=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "rule", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, " leader-length.optimum=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "100%", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, " rule-style=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "solid", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, " rule-thickness=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "1pt", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "/>", 1, 0, 5001 );
                  //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

                  //:szOutputLine = szIndentationChars + "      </fo:block>"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "      </fo:block>", 1, 0, 5001 );
                  //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

                  //:ELSE
               } 
               else
               { 
                  //:szConstantArea = vReportDef.Control.Text
                  GetVariableFromAttribute( szConstantArea, 0, 'S', 257, vReportDef, "Control", "Text", "", 0 );
                  //:szOutputLine = szIndentationChars + "      <fo:block " + szStyle + ">" + szConstantArea + "</fo:block> <!-- XSLT_BuildControl -->"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "      <fo:block ", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, szStyle, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, szConstantArea, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "</fo:block> <!-- XSLT_BuildControl -->", 1, 0, 5001 );
                  //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
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
   return( 0 );
//    
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
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_BuildTable( zVIEW     vReportDefRoot,
                           zVIEW     vReportDef,
                           zVIEW     vLOD,
                           zLONG     lFileHandle,
                           zPCHAR    szOutputLine,
                           zPCHAR    szIndentationChars,
                           zLONG     GroupHierarchicalLevel,
                           zLONG     TableMarginTop,
                           zLONG     TableWidth )
{
   zCHAR     szTableWidth[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTableMarginTop
   zCHAR     szTableMarginTop[ 21 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 


   //:szOutputLine = szIndentationChars + "<!-- In XSLT_BuildTable -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<!-- In XSLT_BuildTable -->", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );


   //:// Build block and table statement.
   //:SET CURSOR FIRST vReportDef.Control WHERE vReportDef.Control.ZKey = vReportDefRoot.XSLT_WorkColumn.ControlZKey
   GetIntegerFromAttribute( &lTempInteger_0, vReportDefRoot, "XSLT_WorkColumn", "ControlZKey" );
   RESULT = SetCursorFirstEntityByInteger( vReportDef, "Control", "ZKey", lTempInteger_0, "" );

   //:szOutputLine = szIndentationChars + "<!-- XSLT_BuildTable Control " + vReportDef.Control.Tag + " -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<!-- XSLT_BuildTable Control ", 1, 0, 5001 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vReportDef, "Control", "Tag", "", 0 );
   ZeidonStringConcat( szOutputLine, 1, 0, szTempString_0, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " -->", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// KJS 11/28/12
   //:IF RESULT >= zCURSOR_SET 
   if ( RESULT >= zCURSOR_SET )
   { 

      //:szOutputLine = szIndentationChars + "<!-- XSLT_BuildTable Control " + vReportDef.Control.Tag + " -->"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "<!-- XSLT_BuildTable Control ", 1, 0, 5001 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vReportDef, "Control", "Tag", "", 0 );
      ZeidonStringConcat( szOutputLine, 1, 0, szTempString_1, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, " -->", 1, 0, 5001 );
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + ""
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
      //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );


      //:ConvertPxToInchesCH( vReportDefRoot, szTableMarginTop, TableMarginTop )
      oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, szTableMarginTop, TableMarginTop );
      //:ConvertPxToInchesCH( vReportDefRoot, szTableWidth, TableWidth )
      oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, szTableWidth, TableWidth );

      //:// Table Block - Initially, we'll make the table block visible.
      //:szOutputLine = szIndentationChars + ""
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
      //://szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + " > <!-- XSLT_BuildTable -->"
      //:szOutputLine = szIndentationChars + "<fo:block > <!-- XSLT_BuildTable -->"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "<fo:block > <!-- XSLT_BuildTable -->", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
      //:szOutputLine = szIndentationChars + ""
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

      //:// Table statement
      //:szOutputLine = szIndentationChars + "<fo:table table-layout=" + QUOTES + "fixed" +
      //:            QUOTES + " width=" + QUOTES + "100%" +
      //:            //QUOTES + " width=" + QUOTES + szTableWidth + "in" +
      //:            QUOTES + " margin-top=" + QUOTES + szTableMarginTop +  "in" + QUOTES + ">"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table table-layout=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "fixed", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, " width=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "100%", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szTableMarginTop, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   } 

   //:END

   //:szOutputLine = szIndentationChars + "<!-- END OF  XSLT_BuildTable -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<!-- END OF  XSLT_BuildTable -->", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_EntityRecur( VIEW vReportDefRoot    BASED ON LOD TZRPSRCO,
//:                  VIEW vReportDefRecurs  BASED ON LOD TZRPSRCO,
//:                  VIEW vLOD              BASED ON LOD TZZOLODO,
//:                  INTEGER lFileHandle,
//:                  STRING ( 5000 ) szOutputLine,
//:                  STRING ( 50 )   szIndentationChars,
//:                  STRING ( 1000 ) szStructurePath )

//:   VIEW vReportDef BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_EntityRecur( zVIEW     vReportDefRoot,
                            zVIEW     vReportDefRecurs,
                            zVIEW     vLOD,
                            zLONG     lFileHandle,
                            zPCHAR    szOutputLine,
                            zPCHAR    szIndentationChars,
                            zPCHAR    szStructurePath )
{
   zVIEW     vReportDef = 0; 
   //:INTEGER PosY
   zLONG     PosY = 0; 
   //:INTEGER CurrentWidth
   zLONG     CurrentWidth = 0; 
   //:INTEGER ColumnCount
   zLONG     ColumnCount = 0; 
   //:INTEGER CellPadding
   zLONG     CellPadding = 0; 
   //:INTEGER GroupHierarchicalLevel
   zLONG     GroupHierarchicalLevel = 0; 
   //:INTEGER i
   zLONG     i = 0; 
   //:STRING ( 50 )  szGroupSetName
   zCHAR     szGroupSetName[ 51 ] = { 0 }; 
   //:STRING ( 10 )  szPageWidth
   zCHAR     szPageWidth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szCurrentWidth
   zCHAR     szCurrentWidth[ 11 ] = { 0 }; 
   //:STRING ( 1 )   szSingleRowFlag
   zCHAR     szSingleRowFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szForEach
   zCHAR     szForEach[ 2 ] = { 0 }; 
   //:STRING ( 32 )  szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szAttributeName
   zCHAR     szAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 20 )  szCellPadding
   zCHAR     szCellPadding[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szCellPaddingName
   zCHAR     szCellPaddingName[ 21 ] = { 0 }; 
   //:STRING ( 50 )  szIndentationSubValue
   zCHAR     szIndentationSubValue[ 51 ] = { 0 }; 
   //:STRING ( 1000 ) szStructure
   zCHAR     szStructure[ 1001 ] = { 0 }; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 


   //:szPageWidth = "7.5in"
   ZeidonStringCopy( szPageWidth, 1, 0, "7.5in", 1, 0, 11 );
   //:szCellPaddingName = "DefaultCellPadding"
   ZeidonStringCopy( szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 21 );

   //://IssueError( vReportDefRoot,0,0, "In EntityR" )

   //:// Process each PartialReportEntity within the recursive LOD structure.
   //:FOR EACH vReportDefRecurs.PartialReportEntity 
   RESULT = SetCursorFirstEntity( vReportDefRecurs, "PartialReportEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:szEntityName = vReportDefRecurs.PartialReportEntity.Name 
      GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDefRecurs, "PartialReportEntity", "Name", "", 0 );
      //://szStructure = szStructurePath + "/" + szEntityName
      //:      
      //:IF vReportDefRecurs.PartialReportEntity.ReportDisplayFlag = "D"
      if ( CompareAttributeToString( vReportDefRecurs, "PartialReportEntity", "ReportDisplayFlag", "D" ) == 0 )
      { 

         //:szStructure = szStructurePath + "/" + szEntityName
         ZeidonStringCopy( szStructure, 1, 0, szStructurePath, 1, 0, 1001 );
         ZeidonStringConcat( szStructure, 1, 0, "/", 1, 0, 1001 );
         ZeidonStringConcat( szStructure, 1, 0, szEntityName, 1, 0, 1001 );

         //:// We don't want to show any of this GroupSet, if there are no xml values for this entity (node).
         //:GenerateLine( vReportDefRoot, lFileHandle, " " )    // Blank line
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
         //:szOutputLine = "<!-- Only execute this GroupSet section if  " + szEntityName + " exists in xml. -->"
         ZeidonStringCopy( szOutputLine, 1, 0, "<!-- Only execute this GroupSet section if  ", 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, " exists in xml. -->", 1, 0, 5001 );
         //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //://szOutputLine = "<xsl:if test=" + QUOTES + szStructure + QUOTES + ">"
         //:szOutputLine = "<xsl:if test=" + QUOTES + szEntityName + QUOTES + ">"
         ZeidonStringCopy( szOutputLine, 1, 0, "<xsl:if test=", 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
         //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:     
         //:SET CURSOR FIRST vReportDefRoot.GroupSet WHERE vReportDefRoot.GroupSet.Tag = szEntityName 
         RESULT = SetCursorFirstEntityByString( vReportDefRoot, "GroupSet", "Tag", szEntityName, "" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
         } 

         //:   // There is an error because the report should have a GroupSet for this.          
         //:END

         //:// Only continue building this xsl section if the user painted a control is this section.
         //:IF vReportDefRoot.Control EXISTS 
         lTempInteger_0 = CheckExistenceOfEntity( vReportDefRoot, "Control" );
         if ( lTempInteger_0 == 0 )
         { 

            //:szOutputLine = "  <!-- XSLT_EntityRecur " + szEntityName + " -->"
            ZeidonStringCopy( szOutputLine, 1, 0, "  <!-- XSLT_EntityRecur ", 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, " -->", 1, 0, 5001 );
            //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

            //://szIndentationChars = "    "    // Base indentation will be 10 characters.
            //:GenerateLine( vReportDefRoot, lFileHandle, " " )    // Blank line
            oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );

            //:// KJS 12/05/12 - I was going to position recurs on report entity we are looking at but I don't see
            //:// at the moment where we even use vLOD except that we pass it to different operations.
            //:i = 1
            i = 1;
            //:LOOP WHILE i <= 3
            while ( i <= 3 )
            { 

               //:IF i = 1 
               if ( i == 1 )
               { 
                  //:SET CURSOR FIRST vReportDefRoot.Group WHERE vReportDefRoot.Group.Type = "gh"
                  RESULT = SetCursorFirstEntityByString( vReportDefRoot, "Group", "Type", "gh", "" );
                  //:ELSE
               } 
               else
               { 
                  //:IF i = 2
                  if ( i == 2 )
                  { 
                     //:SET CURSOR FIRST vReportDefRoot.Group WHERE vReportDefRoot.Group.Type = "ga"
                     RESULT = SetCursorFirstEntityByString( vReportDefRoot, "Group", "Type", "ga", "" );
                     //:ELSE             
                  } 
                  else
                  { 
                     //:SET CURSOR FIRST vReportDefRoot.Group WHERE vReportDefRoot.Group.Type = "gf"
                     RESULT = SetCursorFirstEntityByString( vReportDefRoot, "Group", "Type", "gf", "" );
                  } 

                  //:END 
               } 

               //:END

               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 

                  //:// Initialize Work Group.
                  //:// Note that GroupHierarchicalLevel is the Control level with the vReportDef object, not the entity level within the LOD.
                  //:GroupHierarchicalLevel = 1
                  GroupHierarchicalLevel = 1;

                  //:FOR EACH vReportDefRoot.XSLT_WorkGroup
                  RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkGroup", "" );
                  while ( RESULT > zCURSOR_UNCHANGED )
                  { 
                     //:DELETE ENTITY vReportDefRoot.XSLT_WorkGroup NONE  
                     RESULT = DeleteEntity( vReportDefRoot, "XSLT_WorkGroup", zREPOS_NONE );
                     RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkGroup", "" );
                  } 

                  //:END

                  //:CREATE ENTITY vReportDefRoot.XSLT_WorkGroup 
                  RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkGroup", zPOS_AFTER );

                  //:vReportDefRoot.XSLT_WorkGroup.RecursiveNumber = GroupHierarchicalLevel
                  SetAttributeFromInteger( vReportDefRoot, "XSLT_WorkGroup", "RecursiveNumber", GroupHierarchicalLevel );
                  //:vReportDefRoot.XSLT_WorkGroup.EntityName = szEntityName
                  SetAttributeFromString( vReportDefRoot, "XSLT_WorkGroup", "EntityName", szEntityName );
                  //:            
                  //:szForEach = "N"
                  ZeidonStringCopy( szForEach, 1, 0, "N", 1, 0, 2 );

                  //:IF vReportDefRoot.Group.Type = "gh"
                  if ( CompareAttributeToString( vReportDefRoot, "Group", "Type", "gh" ) == 0 )
                  { 

                     //:// Group is Header.

                     //:// Generate Entity Header Comment.
                     //:GenerateLine( vReportDefRoot, lFileHandle, " " )
                     oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
                     //:szOutputLine = "  <!-- ENTITY HEADER -->"
                     ZeidonStringCopy( szOutputLine, 1, 0, "  <!-- ENTITY HEADER -->", 1, 0, 5001 );
                     //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                     //:GenerateLine( vReportDefRoot, lFileHandle, " " )
                     oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
                     //:szEntityName = szEntityName + "Header"      
                     ZeidonStringConcat( szEntityName, 1, 0, "Header", 1, 0, 33 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:IF vReportDefRoot.Group.Type = "ga" 
                     if ( CompareAttributeToString( vReportDefRoot, "Group", "Type", "ga" ) == 0 )
                     { 
                        //:// Generate Entity Detail Comment.
                        //:GenerateLine( vReportDefRoot, lFileHandle, " " )
                        oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
                        //:szOutputLine = "  <!-- ENTITY DETAIL -->"
                        ZeidonStringCopy( szOutputLine, 1, 0, "  <!-- ENTITY DETAIL -->", 1, 0, 5001 );
                        //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                        oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                        //:GenerateLine( vReportDefRoot, lFileHandle, " " )
                        oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
                        //:szEntityName = vReportDefRoot.GroupSet.Tag
                        GetVariableFromAttribute( szEntityName, 0, 'S', 33, vReportDefRoot, "GroupSet", "Tag", "", 0 );
                        //:IF vReportDefRecurs.PartialReportEntityChild EXISTS
                        lTempInteger_1 = CheckExistenceOfEntity( vReportDefRecurs, "PartialReportEntityChild" );
                        if ( lTempInteger_1 == 0 )
                        { 
                           //:szForEach = "Y"
                           ZeidonStringCopy( szForEach, 1, 0, "Y", 1, 0, 2 );
                        } 

                        //:END
                        //:ELSE
                     } 
                     else
                     { 
                        //:IF vReportDefRoot.Group.Type = "gf" 
                        if ( CompareAttributeToString( vReportDefRoot, "Group", "Type", "gf" ) == 0 )
                        { 
                           //:// Generate Entity Footer Comment.
                           //:GenerateLine( vReportDefRoot, lFileHandle, " " )
                           oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
                           //:szOutputLine = "  <!-- ENTITY FOOTER -->"
                           ZeidonStringCopy( szOutputLine, 1, 0, "  <!-- ENTITY FOOTER -->", 1, 0, 5001 );
                           //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                           oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                           //:GenerateLine( vReportDefRoot, lFileHandle, " " )
                           oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, " " );
                           //:szEntityName = szEntityName + "Footer"
                           ZeidonStringConcat( szEntityName, 1, 0, "Footer", 1, 0, 33 );
                        } 

                        //:END
                     } 

                     //:END
                  } 

                  //:END

                  //:// Make sure Controls are in row/column order.
                  //:OrderEntityForView( vReportDefRoot, "Control", "PSDLG_Y A PSDLG_X A" )
                  OrderEntityForView( vReportDefRoot, "Control", "PSDLG_Y A PSDLG_X A" );

                  //:// Check if all controls are on a single line, without a subcontrol.
                  //://    If so, we will create a table entry with a column for each control.
                  //://    If not, we will create a table entry with a single cell.
                  //:szSingleRowFlag = "Y"
                  ZeidonStringCopy( szSingleRowFlag, 1, 0, "Y", 1, 0, 2 );
                  //:IF vReportDefRoot.Control EXISTS
                  lTempInteger_2 = CheckExistenceOfEntity( vReportDefRoot, "Control" );
                  if ( lTempInteger_2 == 0 )
                  { 
                     //:PosY = vReportDefRoot.Control.PSDLG_Y
                     GetIntegerFromAttribute( &PosY, vReportDefRoot, "Control", "PSDLG_Y" );
                     //:FOR EACH vReportDefRoot.Control
                     RESULT = SetCursorFirstEntity( vReportDefRoot, "Control", "" );
                     while ( RESULT > zCURSOR_UNCHANGED )
                     { 
                        //:IF vReportDefRoot.CtrlCtrl EXISTS OR vReportDefRoot.Control.PSDLG_Y != PosY
                        lTempInteger_3 = CheckExistenceOfEntity( vReportDefRoot, "CtrlCtrl" );
                        if ( lTempInteger_3 == 0 || CompareAttributeToInteger( vReportDefRoot, "Control", "PSDLG_Y", PosY ) != 0 )
                        { 
                           //:szSingleRowFlag = "N"
                           ZeidonStringCopy( szSingleRowFlag, 1, 0, "N", 1, 0, 2 );
                        } 

                        RESULT = SetCursorNextEntity( vReportDefRoot, "Control", "" );
                        //:END
                     } 

                     //:END
                  } 

                  //:END
                  //:IF szSingleRowFlag = "N"
                  if ( ZeidonStringCompare( szSingleRowFlag, 1, 0, "N", 1, 0, 2 ) == 0 )
                  { 
                     //:// Go to process more complex Group.

                     //:// Loop through all the entities... (we do this in XSLT_GroupTable if all the controls are on
                     //:// one line but if not, then we still want to build this for each entity).
                     //:szOutputLine = "    <xsl:for-each select=" + QUOTES + szEntityName + QUOTES + ">"
                     ZeidonStringCopy( szOutputLine, 1, 0, "    <xsl:for-each select=", 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
                     //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                     //:      
                     //:CreateViewFromView( vReportDef, vReportDefRoot )
                     CreateViewFromView( &vReportDef, vReportDefRoot );
                     //:NAME VIEW vReportDef "vReportDefTop"
                     SetNameForView( vReportDef, "vReportDefTop", 0, zLEVEL_TASK );
                     //:XSLT_GroupRecur( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel )
                     oTZRPSRCO_XSLT_GroupRecur( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel );
                     //:DropView( vReportDef )
                     DropView( vReportDef );

                     //:IF szForEach = "N" 
                     if ( ZeidonStringCompare( szForEach, 1, 0, "N", 1, 0, 2 ) == 0 )
                     { 
                        //:szOutputLine = "    </xsl:for-each>"
                        ZeidonStringCopy( szOutputLine, 1, 0, "    </xsl:for-each>", 1, 0, 5001 );
                        //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )    
                        oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                     } 

                     //:END
                     //:ELSE
                  } 
                  else
                  { 
                     //:CreateViewFromView( vReportDef, vReportDefRoot )
                     CreateViewFromView( &vReportDef, vReportDefRoot );
                     //:// Detail entity has single row.
                     //:// Build work subobject, with a single row and columns.
                     //:CREATE ENTITY vReportDefRoot.XSLT_WorkRow
                     RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkRow", zPOS_AFTER );
                     //:IF vReportDefRoot.Control EXISTS 
                     lTempInteger_4 = CheckExistenceOfEntity( vReportDefRoot, "Control" );
                     if ( lTempInteger_4 == 0 )
                     { 
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
                     } 

                     //:END
                     //:IF szForEach = "Y" 
                     if ( ZeidonStringCompare( szForEach, 1, 0, "Y", 1, 0, 2 ) == 0 )
                     { 
                        //:szOutputLine = "    <xsl:for-each select=" + QUOTES + szEntityName + QUOTES + ">"
                        ZeidonStringCopy( szOutputLine, 1, 0, "    <xsl:for-each select=", 1, 0, 5001 );
                        ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                        ZeidonStringConcat( szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
                        ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                        ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
                        //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                        oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                        //:XSLT_GroupTable( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars, 1, "" )
                        oTZRPSRCO_XSLT_GroupTable( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars, 1, "" );
                        //:ELSE
                     } 
                     else
                     { 
                        //:// Go to build the table.
                        //:XSLT_GroupTable( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars, 1, szEntityName )
                        oTZRPSRCO_XSLT_GroupTable( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars, 1, szEntityName );
                     } 

                     //:END
                     //:DropView( vReportDef )
                     DropView( vReportDef );
                  } 

                  //:END               
               } 

               //:END // IF SET CURSOR To header/detail/footer
               //:i = i + 1
               i = i + 1;
            } 

            //:END  // FOR EACH vReportDefRoot.Group

            //:// KJS 01/22/13 *****
            //:// Process any LOD subentities.
            //:IF vReportDefRecurs.PartialReportEntityChild EXISTS
            lTempInteger_5 = CheckExistenceOfEntity( vReportDefRecurs, "PartialReportEntityChild" );
            if ( lTempInteger_5 == 0 )
            { 

               //:SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" )
               SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" );
               //:szIndentationSubValue = szIndentationChars + "      "
               ZeidonStringCopy( szIndentationSubValue, 1, 0, szIndentationChars, 1, 0, 51 );
               ZeidonStringConcat( szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
               //:XSLT_EntityRecur( vReportDefRoot, vReportDefRecurs, vLOD, lFileHandle, szOutputLine, szIndentationSubValue, szStructure )
               oTZRPSRCO_XSLT_EntityRecur( vReportDefRoot, vReportDefRecurs, vLOD, lFileHandle, szOutputLine, szIndentationSubValue, szStructure );

               //:ResetViewFromSubobject( vReportDefRecurs )
               ResetViewFromSubobject( vReportDefRecurs );

               //:IF szForEach = "Y" 
               if ( ZeidonStringCompare( szForEach, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:szOutputLine = "    </xsl:for-each>"
                  ZeidonStringCopy( szOutputLine, 1, 0, "    </xsl:for-each>", 1, 0, 5001 );
                  //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
               } 

               //:END

               //:// I think when we get here, we need to reset on the report GroupSet/Group because it has been changed when we go down levels.
               //:SET CURSOR FIRST vReportDefRoot.GroupSet WHERE vReportDefRoot.GroupSet.Tag = szEntityName
               RESULT = SetCursorFirstEntityByString( vReportDefRoot, "GroupSet", "Tag", szEntityName, "" );
               //:SET CURSOR FIRST vReportDefRoot.Group WHERE vReportDefRoot.Group.Type = "ga"
               RESULT = SetCursorFirstEntityByString( vReportDefRoot, "Group", "Type", "ga", "" );
            } 

            //:END
         } 

         //:END // IF vReportDefRoot.Control EXISTS

         //:szOutputLine = "</xsl:if>"
         ZeidonStringCopy( szOutputLine, 1, 0, "</xsl:if>", 1, 0, 5001 );
         //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
         //:ELSE
      } 
      else
      { 
         //:szStructure = szStructurePath + ""
         ZeidonStringCopy( szStructure, 1, 0, szStructurePath, 1, 0, 1001 );
         ZeidonStringConcat( szStructure, 1, 0, "", 1, 0, 1001 );
         //:// This entity might not be on the report but a sub entity might be so check.
         //:SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" )
         SetViewToSubobject( vReportDefRecurs, "PartialReportEntityChild" );
         //:szIndentationSubValue = szIndentationChars + "      "
         ZeidonStringCopy( szIndentationSubValue, 1, 0, szIndentationChars, 1, 0, 51 );
         ZeidonStringConcat( szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
         //:XSLT_EntityRecur( vReportDefRoot, vReportDefRecurs, vLOD, lFileHandle, szOutputLine, szIndentationSubValue, szStructure )
         oTZRPSRCO_XSLT_EntityRecur( vReportDefRoot, vReportDefRecurs, vLOD, lFileHandle, szOutputLine, szIndentationSubValue, szStructure );

         //:ResetViewFromSubobject( vReportDefRecurs )
         ResetViewFromSubobject( vReportDefRecurs );
      } 

      RESULT = SetCursorNextEntity( vReportDefRecurs, "PartialReportEntity", "" );
      //:END // IF vReportDefRecurs.PartialReportEntity.ReportDisplayFlag = "D"
   } 


   //:END  // FOR EACH vReportDefRoot.GroupSet WHERE vReportDefRoot.GroupSet.Type = "E"
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_GetStyleShape( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                    STRING ( 256 ) szStyle )

//:   SHORT          nMultiLineFlag
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_GetStyleShape( zVIEW     vReportDef,
                              zPCHAR    szStyle )
{
   zSHORT    nMultiLineFlag = 0; 
   //:SHORT          nBoldFlag
   zSHORT    nBoldFlag = 0; 
   //:SHORT          nItalicFlag
   zSHORT    nItalicFlag = 0; 
   //:SHORT          nStrikeoutFlag
   zSHORT    nStrikeoutFlag = 0; 
   //:SHORT          nUnderlineFlag
   zSHORT    nUnderlineFlag = 0; 
   //:INTEGER        lJustify
   zLONG     lJustify = 0; 
   //:INTEGER        lFontSize
   zLONG     lFontSize = 0; 
   //:INTEGER        lTextColor
   zLONG     lTextColor = 0; 
   //:INTEGER        lTextBkColor
   zLONG     lTextBkColor = 0; 
   //:INTEGER        lBorderStyle
   zLONG     lBorderStyle = 0; 
   //:INTEGER        lBorderColor
   zLONG     lBorderColor = 0; 
   //:INTEGER        lEscapement
   zLONG     lEscapement = 0; 
   //:INTEGER        lType
   zLONG     lType = 0; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 
   //:DECIMAL        lBorder
   ZDecimal  lBorder = 0.0; 
   //:STRING ( 32 )  szFaceName
   zCHAR     szFaceName[ 33 ] = { 0 }; 
   //:STRING ( 10 )  szBorderWidth
   zCHAR     szBorderWidth[ 11 ] = { 0 }; 
   //:STRING ( 50 ) szAlign
   zCHAR     szAlign[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szBorderStyle
   zCHAR     szBorderStyle[ 51 ] = { 0 }; 
   //:STRING ( 25 ) szFontSize
   zCHAR     szFontSize[ 26 ] = { 0 }; 
   //:STRING ( 2 )  szWidth
   zCHAR     szWidth[ 3 ] = { 0 }; 
   //:STRING ( 500 ) szMsg
   zCHAR     szMsg[ 501 ] = { 0 }; 
   zCHAR     szTempString_0[ 33 ]; 


   //:TraceLineS("**** XSLT_GetStyleShape *** ", vReportDef.Control.Tag )
   GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vReportDef, "Control", "Tag" );
   TraceLineS( "**** XSLT_GetStyleShape *** ", szTempString_0 );

   //:szStyle = ""
   ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 257 );

   //:SetRptShapeFromBlob( vReportDef )
   SetRptShapeFromBlob( vReportDef );

   //:// Do we only show border if line width is greater than zero? 
   //:IF vReportDef.Control.BorderWidth > 0
   if ( CompareAttributeToInteger( vReportDef, "Control", "BorderWidth", 0 ) > 0 )
   { 

      //:// Seems like the border is a little too thick. Going to try halving what is specified.
      //:lBorder = vReportDef.Control.BorderWidth
      GetDecimalFromAttribute( &lBorder, vReportDef, "Control", "BorderWidth" );
      //:lBorder = lBorder / 2
      lBorder = lBorder / 2;
      //:SysConvertDecimalToString( lBorder, szBorderWidth, 1 )
      SysConvertDecimalToString( &lBorder, szBorderWidth, 1 );
      //://SysConvertDecimalToString( DOUBLE /* pDecimal */,
      //://                           STRING /* ReturnString */,
      //://                           INTEGER /* nPrecision */ )
      //://zIntegerToString( szBorderWidth, 25, lFontSize )
      //://szBorderWidth = vReportDef.Control.BorderWidth 


      //://TraceLineS("**** BorderWidth *** ", szBorderWidth)
      //:szStyle = szStyle + "border-width=" + QUOTES + szBorderWidth + QUOTES + " "
      ZeidonStringConcat( szStyle, 1, 0, "border-width=", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, szBorderWidth, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );

      //:IF vReportDef.Control.BorderStyle = 0
      if ( CompareAttributeToInteger( vReportDef, "Control", "BorderStyle", 0 ) == 0 )
      { 
         //://TraceLineS("**** Border SOLID *** ", szBorderWidth)
         //:szBorderStyle = "solid" 
         ZeidonStringCopy( szBorderStyle, 1, 0, "solid", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:IF vReportDef.Control.BorderStyle = 1
         if ( CompareAttributeToInteger( vReportDef, "Control", "BorderStyle", 1 ) == 0 )
         { 
            //://TraceLineS("**** Border dashed *** ", szBorderWidth)
            //:szBorderStyle = "dashed" 
            ZeidonStringCopy( szBorderStyle, 1, 0, "dashed", 1, 0, 51 );
            //:ELSE
         } 
         else
         { 
            //:IF vReportDef.Control.BorderStyle = 2
            if ( CompareAttributeToInteger( vReportDef, "Control", "BorderStyle", 2 ) == 0 )
            { 
               //://TraceLineS("**** Border dotted *** ", szBorderWidth)
               //:szBorderStyle = "dotted" 
               ZeidonStringCopy( szBorderStyle, 1, 0, "dotted", 1, 0, 51 );
               //:ELSE
            } 
            else
            { 
               //://TraceLineS("**** Border SOLID *** ", szBorderWidth)
               //:szBorderStyle = "solid" 
               ZeidonStringCopy( szBorderStyle, 1, 0, "solid", 1, 0, 51 );
            } 

            //:END 
         } 

         //:END 
      } 

      //:END

      //:// Maybe put in color later...
      //:// KJS 01/31/13 - This is cheesy but I'm going for it for now...
      //:// The only other report color I know being used is red... which seems to be 255
      //:// SetRptShapeFromBlob sets wLineColorText and wLineColorInt.
      //:IF vReportDef.Control.wLineColorInt = 255 
      if ( CompareAttributeToInteger( vReportDef, "Control", "wLineColorInt", 255 ) == 0 )
      { 
         //:szStyle = szStyle + "border-color=" + QUOTES + "red" + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "border-color=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, "red", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szStyle = szStyle + "border-color=" + QUOTES + "black" + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "border-color=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, "black", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
      } 

      //:END

      //:// If the "Horizontal" checkbox is checked, then we have set BorderBottom to 1 in SetRptShapeFromBlob.
      //:IF vReportDef.Control.BorderBottom = 1
      if ( CompareAttributeToInteger( vReportDef, "Control", "BorderBottom", 1 ) == 0 )
      { 
         //:szStyle = szStyle + "border-bottom-style=" + QUOTES + szBorderStyle + QUOTES + " "         
         ZeidonStringConcat( szStyle, 1, 0, "border-bottom-style=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szBorderStyle, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szStyle = szStyle + "border-style=" + QUOTES + szBorderStyle + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "border-style=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szBorderStyle, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
      } 

      //:END
   } 

   //:   
   //:   
   //:END  
   return( 0 );
//    
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_GetStyleText( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                   STRING ( 256 ) szStyle )

//:   SHORT          nMultiLineFlag
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_GetStyleText( zVIEW     vReportDef,
                             zPCHAR    szStyle )
{
   zSHORT    nMultiLineFlag = 0; 
   //:SHORT          nBoldFlag
   zSHORT    nBoldFlag = 0; 
   //:SHORT          nItalicFlag
   zSHORT    nItalicFlag = 0; 
   //:SHORT          nStrikeoutFlag
   zSHORT    nStrikeoutFlag = 0; 
   //:SHORT          nUnderlineFlag
   zSHORT    nUnderlineFlag = 0; 
   //:INTEGER        lJustify
   zLONG     lJustify = 0; 
   //:INTEGER        lFontSize
   zLONG     lFontSize = 0; 
   //:INTEGER        lTextColor
   zLONG     lTextColor = 0; 
   //:INTEGER        lTextBkColor
   zLONG     lTextBkColor = 0; 
   //:INTEGER        lBorderStyle
   zLONG     lBorderStyle = 0; 
   //:INTEGER        lBorderColor
   zLONG     lBorderColor = 0; 
   //:INTEGER        lEscapement
   zLONG     lEscapement = 0; 
   //:INTEGER        lType
   zLONG     lType = 0; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 
   //:STRING ( 32 )  szFaceName
   zCHAR     szFaceName[ 33 ] = { 0 }; 
   //:STRING ( 2 )  szBorderWidth
   zCHAR     szBorderWidth[ 3 ] = { 0 }; 
   //:STRING ( 50 ) szAlign
   zCHAR     szAlign[ 51 ] = { 0 }; 
   //:STRING ( 50 ) szBorderStyle
   zCHAR     szBorderStyle[ 51 ] = { 0 }; 
   //:STRING ( 25 ) szFontSize
   zCHAR     szFontSize[ 26 ] = { 0 }; 
   //:STRING ( 2 )  szWidth
   zCHAR     szWidth[ 3 ] = { 0 }; 
   //:STRING ( 500 ) szMsg
   zCHAR     szMsg[ 501 ] = { 0 }; 


   //:szStyle = ""
   ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 257 );

   //:nRC = TX_GetTextRptCtrlBOI( vReportDef, nMultiLineFlag, nBoldFlag,
   //:                            nItalicFlag, nStrikeoutFlag,
   //:                            nUnderlineFlag, lJustify,
   //:                            lFontSize, lTextColor, lTextBkColor,
   //:                            lBorderStyle, lBorderColor,
   //:                            lEscapement, lType, szFaceName )
   nRC = TX_GetTextRptCtrlBOI( vReportDef, &nMultiLineFlag, &nBoldFlag, &nItalicFlag, &nStrikeoutFlag, &nUnderlineFlag, &lJustify, &lFontSize, &lTextColor, &lTextBkColor, &lBorderStyle, &lBorderColor, &lEscapement, &lType, szFaceName );
   //:                            

   //://TraceLineI("**** lBorderStyle *** ", lBorderStyle )

   //:// Text Alignment
   //:/*
   //:szAlign = vReportDef.Control.TextAlignHorizontal
   //:// Should I give an error message if TextAlignHorizontal is not "Right", "Left" or "Center" (I have had problems since
   //:// putting in mapping for this).
   //:IF szAlign != "" AND szAlign != "right" AND szAlign != "center" AND szAlign != "left"
   //:   szMsg = "Alignment for control " + vReportDef.Control.Tag + " is wrong. Please set this again."  
   //:   MessageSend( vReportDef, "Text Alignment", "Autodesigner",
   //:                szMsg,
   //:                zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
   //:END
   //:IF szAlign != ""
   //:   szStyle = szStyle + "text-align=" + QUOTES + szAlign + QUOTES + " "
   //:END
   //:*/

   //:// If we have a new line char. we need to have the following...
   //:IF nMultiLineFlag > 0
   if ( nMultiLineFlag > 0 )
   { 
      //:szStyle = szStyle + "linefeed-treatment=" + QUOTES + "preserve" + QUOTES + " "    
      ZeidonStringConcat( szStyle, 1, 0, "linefeed-treatment=", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, "preserve", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
   } 

   //:END

   //:IF lBorderStyle != 0 
   if ( lBorderStyle != 0 )
   { 

      //:nRC = GetBorderLineWidth( lBorderStyle, szBorderWidth )
      nRC = GetBorderLineWidth( lBorderStyle, szBorderWidth );
      //://TraceLineS("*** Line Width *** ", szBorderWidth )
      //:IF szBorderWidth = "" OR szBorderWidth = "0"
      if ( ZeidonStringCompare( szBorderWidth, 1, 0, "", 1, 0, 3 ) == 0 || ZeidonStringCompare( szBorderWidth, 1, 0, "0", 1, 0, 3 ) == 0 )
      { 
         //:szBorderWidth = "1"
         ZeidonStringCopy( szBorderWidth, 1, 0, "1", 1, 0, 3 );
      } 

      //:END

      //:szStyle = szStyle + "border-width=" + QUOTES + szBorderWidth + QUOTES + " "
      ZeidonStringConcat( szStyle, 1, 0, "border-width=", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, szBorderWidth, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );


      //:szBorderStyle = ""
      ZeidonStringCopy( szBorderStyle, 1, 0, "", 1, 0, 51 );
      //:nRC = IsFlagSequenceSet( lBorderStyle, 1 )
      nRC = IsFlagSequenceSet( lBorderStyle, 1 );
      //:IF nRC > 0 
      if ( nRC > 0 )
      { 
         //://TraceLineI("**** lBorderStyle DASH is SET *** ", nRC)
         //:szBorderStyle = "dashed" 
         ZeidonStringCopy( szBorderStyle, 1, 0, "dashed", 1, 0, 51 );
      } 

      //:END
      //:nRC = IsFlagSequenceSet( lBorderStyle, 2 )
      nRC = IsFlagSequenceSet( lBorderStyle, 2 );
      //:IF nRC > 0 
      if ( nRC > 0 )
      { 
         //://TraceLineI("**** lBorderStyle DOTTED is SET *** ", nRC)
         //:szBorderStyle = "dotted" 
         ZeidonStringCopy( szBorderStyle, 1, 0, "dotted", 1, 0, 51 );
      } 

      //:END
      //:IF szBorderStyle = ""
      if ( ZeidonStringCompare( szBorderStyle, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szBorderStyle = "solid" 
         ZeidonStringCopy( szBorderStyle, 1, 0, "solid", 1, 0, 51 );
      } 

      //:END


      //:nRC = IsFlagSequenceSet( lBorderStyle, 256 )
      nRC = IsFlagSequenceSet( lBorderStyle, 256 );
      //:IF nRC > 0 
      if ( nRC > 0 )
      { 
         //://TraceLineI("**** lBorderStyle Top is SET *** ", nRC)
         //:szStyle = szStyle + "border-top-style=" + QUOTES + szBorderStyle + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "border-top-style=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szBorderStyle, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
      } 

      //:END
      //:nRC = IsFlagSequenceSet( lBorderStyle, 512 )
      nRC = IsFlagSequenceSet( lBorderStyle, 512 );
      //:IF nRC > 0 
      if ( nRC > 0 )
      { 
         //://TraceLineI("**** lBorderStyle Left is SET *** ", nRC)
         //:szStyle = szStyle + "border-left-style=" + QUOTES + szBorderStyle + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "border-left-style=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szBorderStyle, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
      } 

      //:END
      //:nRC = IsFlagSequenceSet( lBorderStyle, 1024 )
      nRC = IsFlagSequenceSet( lBorderStyle, 1024 );
      //:IF nRC > 0 
      if ( nRC > 0 )
      { 
         //://TraceLineI("**** lBorderStyle Bottom is SET *** ", nRC)
         //:szStyle = szStyle + "border-bottom-style=" + QUOTES + szBorderStyle + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "border-bottom-style=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szBorderStyle, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
      } 

      //:END
      //:nRC = IsFlagSequenceSet( lBorderStyle, 2048 )
      nRC = IsFlagSequenceSet( lBorderStyle, 2048 );
      //:IF nRC > 0 
      if ( nRC > 0 )
      { 
         //://TraceLineI("**** lBorderStyle Right is SET *** ", nRC)
         //:szStyle = szStyle + "border-right-style=" + QUOTES + szBorderStyle + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "border-right-style=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szBorderStyle, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
      } 

      //:END
   } 

   //:END

   //:IF lJustify != 0
   if ( lJustify != 0 )
   { 
      //:nRC = IsFlagSequenceSet( lJustify, 2 )
      nRC = IsFlagSequenceSet( lJustify, 2 );
      //:IF nRC != 0
      if ( nRC != 0 )
      { 
         //:szStyle = szStyle + "text-align=" + QUOTES + "center" + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "text-align=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, "center", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:nRC = IsFlagSequenceSet( lJustify, 4 )
         nRC = IsFlagSequenceSet( lJustify, 4 );
         //:IF nRC != 0
         if ( nRC != 0 )
         { 
            //:szStyle = szStyle + "text-align=" + QUOTES + "right" + QUOTES + " "
            ZeidonStringConcat( szStyle, 1, 0, "text-align=", 1, 0, 257 );
            ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
            ZeidonStringConcat( szStyle, 1, 0, "right", 1, 0, 257 );
            ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
            ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:/*
   //:// Do we only show border if line width is greater than zero? 
   //:IF vReportDef.Control.BorderWidth > 0

   //:   szBorderWidth = vReportDef.Control.BorderWidth 
   //:   szStyle = szStyle + "border-width=" + QUOTES + szBorderWidth + QUOTES + " "
   //:   
   //:   IF vReportDef.Control.BorderStyle = 0
   //:      szBorderStyle = "solid" 
   //:   ELSE
   //:   IF vReportDef.Control.BorderStyle = 1
   //:      szBorderStyle = "dashed" 
   //:   ELSE
   //:   IF vReportDef.Control.BorderStyle = 2
   //:      szBorderStyle = "dotted" 
   //:   ELSE
   //:      szBorderStyle = "solid" 
   //:   END 
   //:   END 
   //:   END
   //:   
   //:   //IF vReportDef.Control.Tag = "Rpt_Text8" OR  vReportDef.Control.Tag = "Rpt_Text13"
   //:   
   //:   // Maybe put in color later...
   //:   szStyle = szStyle + "border-color=" + QUOTES + "black" + QUOTES + " "
   //:   
   //:   IF vReportDef.Control.BorderTop = 0 AND vReportDef.Control.BorderBottom = 0
   //:      szStyle = szStyle + "border-style=" + QUOTES + szBorderStyle + QUOTES + " "
   //:   ELSE
   //:   IF vReportDef.Control.BorderTop = 1 
   //:      szStyle = szStyle + "border-top-style=" + QUOTES + szBorderStyle + QUOTES + " "
   //:   END
   //:   IF vReportDef.Control.BorderBottom = 1
   //:      szStyle = szStyle + "border-bottom-style=" + QUOTES + szBorderStyle + QUOTES + " "
   //:   END   
   //:   END
   //:   
   //:   //END
   //:END 
   //:*/ 

   //:IF lTextColor > 0
   if ( lTextColor > 0 )
   { 
      //:TraceLineI("**** lTextColor  *** ", lTextColor ) 
      TraceLineI( "**** lTextColor  *** ", lTextColor );
   } 

   //:END
   //:IF lTextBkColor > 0
   if ( lTextBkColor > 0 )
   { 
      //:TraceLineI("**** lTextBkColor  *** ", lTextBkColor ) 
      TraceLineI( "**** lTextBkColor  *** ", lTextBkColor );
   } 

   //:END

   //://IF vReportDef.Control.FontSize > 0
   //:IF lFontSize > 0
   if ( lFontSize > 0 )
   { 
      //:lFontSize = lFontSize / 10
      lFontSize = lFontSize / 10;
      //:IF lFontSize != 9 
      if ( lFontSize != 9 )
      { 
         //:zIntegerToString( szFontSize, 25, lFontSize )
         zIntegerToString( szFontSize, 25, lFontSize );
         //:szStyle = szStyle + "font-size=" + QUOTES + szFontSize + "pt" + QUOTES + " "
         ZeidonStringConcat( szStyle, 1, 0, "font-size=", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szFontSize, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, "pt", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
      } 

      //:END
   } 

   //:END   

   //://IF vReportDef.Control.TextBold = "Y"
   //:IF nBoldFlag != 0
   if ( nBoldFlag != 0 )
   { 
      //:szStyle = szStyle + "font-weight=" + QUOTES + "bold" + QUOTES + " "
      ZeidonStringConcat( szStyle, 1, 0, "font-weight=", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, "bold", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
   } 

   //:END   

   //://IF vReportDef.Control.TextItalic = "Y"
   //:IF nItalicFlag != 0 
   if ( nItalicFlag != 0 )
   { 
      //:szStyle = szStyle + "font-style=" + QUOTES + "italic" + QUOTES + " "
      ZeidonStringConcat( szStyle, 1, 0, "font-style=", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, "italic", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
   } 

   //:END   

   //://IF vReportDef.Control.TextUnderline = "Y"
   //:IF nUnderlineFlag != 0
   if ( nUnderlineFlag != 0 )
   { 
      //:szStyle = szStyle + "text-decoration=" + QUOTES + "underline" + QUOTES + " "
      ZeidonStringConcat( szStyle, 1, 0, "text-decoration=", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, "underline", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, " ", 1, 0, 257 );
   } 

   //:END   
   return( 0 );
//    
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
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_GroupRecur( zVIEW     vReportDefRoot,
                           zVIEW     vReportDef,
                           zVIEW     vLOD,
                           zLONG     lFileHandle,
                           zPCHAR    szOutputLine,
                           zPCHAR    szIndentationChars,
                           zLONG     GroupHierarchicalLevel )
{
   zVIEW     vReportDefRoot2 = 0; 
   //:VIEW vReportDef3     BASED ON LOD TZRPSRCO
   zVIEW     vReportDef3 = 0; 
   //:STRING ( 20 )  szCurrentWidth
   zCHAR     szCurrentWidth[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szPageWidth
   zCHAR     szPageWidth[ 21 ] = { 0 }; 
   //:STRING ( 30 )  szCellPaddingName
   zCHAR     szCellPaddingName[ 31 ] = { 0 }; 
   //:STRING ( 50 )  szAttributeMappingName
   zCHAR     szAttributeMappingName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szReportDefName
   zCHAR     szReportDefName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSubIndentationChars
   zCHAR     szSubIndentationChars[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCellPadding
   zCHAR     szCellPadding[ 51 ] = { 0 }; 
   //:STRING ( 20 )  szTableMarginTop
   zCHAR     szTableMarginTop[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTablePaddingLeft
   zCHAR     szTablePaddingLeft[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTableWidth
   zCHAR     szTableWidth[ 21 ] = { 0 }; 
   //:STRING ( 5 )   szColumnCount
   zCHAR     szColumnCount[ 6 ] = { 0 }; 
   //:STRING ( 1 )   szSubControlType
   zCHAR     szSubControlType[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szShapeGrpExists
   zCHAR     szShapeGrpExists[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szColumnCountEqual
   zCHAR     szColumnCountEqual[ 2 ] = { 0 }; 
   //:STRING ( 2 )   szHierarchicalLevel
   zCHAR     szHierarchicalLevel[ 3 ] = { 0 }; 
   //:STRING ( 20 )  szSubObjectName
   zCHAR     szSubObjectName[ 21 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 256 ) szStyle
   zCHAR     szStyle[ 257 ] = { 0 }; 
   //:STRING ( 10 )  szLineWidth
   zCHAR     szLineWidth[ 11 ] = { 0 }; 
   //:INTEGER       CurrentWidth
   zLONG     CurrentWidth = 0; 
   //:INTEGER       CurrentRowPositionX
   zLONG     CurrentRowPositionX = 0; 
   //:INTEGER       CurrentPositionY
   zLONG     CurrentPositionY = 0; 
   //:INTEGER       LastPositionY
   zLONG     LastPositionY = 0; 
   //:INTEGER       LastPositionRowY
   zLONG     LastPositionRowY = 0; 
   //:INTEGER       LastPositionX
   zLONG     LastPositionX = 0; 
   //:INTEGER       LastWidthX
   zLONG     LastWidthX = 0; 
   //:INTEGER       TopGroupPosition
   zLONG     TopGroupPosition = 0; 
   //:INTEGER       ColumnCount
   zLONG     ColumnCount = 0; 
   //:INTEGER       ControlCount
   zLONG     ControlCount = 0; 
   //:INTEGER       FirstColumnCount
   zLONG     FirstColumnCount = 0; 
   //:INTEGER       TextColumnCount
   zLONG     TextColumnCount = 0; 
   //:INTEGER       CellPadding
   zLONG     CellPadding = 0; 
   //:INTEGER       RowCount
   zLONG     RowCount = 0; 
   //:INTEGER       TablePaddingLeft
   zLONG     TablePaddingLeft = 0; 
   //:INTEGER       TableMarginTop
   zLONG     TableMarginTop = 0; 
   //:INTEGER       TableWidth
   zLONG     TableWidth = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zCHAR     szTempString_1[ 33 ]; 



   //:szOutputLine = szIndentationChars + "<!-- In XSLT_GroupRecur -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<!-- In XSLT_GroupRecur -->", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

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
   ZeidonStringCopy( szPageWidth, 1, 0, "7.5in", 1, 0, 21 );
   //:szCellPaddingName = "DefaultCellPadding"
   ZeidonStringCopy( szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 31 );

   //:// Also, if any control is a Shape/Group, we assume all are.
   //:// First determine if there are any Shape/Group.
   //:szShapeGrpExists = ""
   ZeidonStringCopy( szShapeGrpExists, 1, 0, "", 1, 0, 2 );
   //:FOR EACH vReportDef.Control
   RESULT = SetCursorFirstEntity( vReportDef, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vReportDef.CtrlCtrl EXISTS AND vReportDef.ControlDef.Tag = "Rpt_Shape"
      lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "CtrlCtrl" );
      if ( lTempInteger_0 == 0 && CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_Shape" ) == 0 )
      { 
         //:szShapeGrpExists = "Y"
         ZeidonStringCopy( szShapeGrpExists, 1, 0, "Y", 1, 0, 2 );
      } 

      RESULT = SetCursorNextEntity( vReportDef, "Control", "" );
      //:END
   } 

   //:END

   //:// Build the WorkGroup for organizing the Controls in tables.
   //:// Build work subobject, which organizes controls in rows and columns.
   //:// Note that we will only process Shape/Group controls with subcontrols.
   //:ControlCount = 0
   ControlCount = 0;
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
      //:ControlCount = ControlCount + 1
      ControlCount = ControlCount + 1;
      //:CurrentPositionY = vReportDef.Control.PSDLG_Y
      GetIntegerFromAttribute( &CurrentPositionY, vReportDef, "Control", "PSDLG_Y" );
      //:IF CurrentPositionY != LastPositionY
      if ( CurrentPositionY != LastPositionY )
      { 
         //:CREATE ENTITY vReportDefRoot.XSLT_WorkRow
         RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkRow", zPOS_AFTER );
         //:vReportDefRoot.XSLT_WorkRow.PSDLG_Y = vReportDef.Control.PSDLG_Y
         SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y", vReportDef, "Control", "PSDLG_Y" );
         //:vReportDefRoot.XSLT_WorkRow.Tag     = vReportDef.Control.Tag + "Parent"
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vReportDef, "Control", "Tag" );
         ZeidonStringConcat( szTempString_0, 1, 0, "Parent", 1, 0, 33 );
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
      RESULT = SetCursorNextEntity( vReportDef, "Control", "" );
   } 

   //:END

   //:// Now see if every row has equal counts, if so then build as a table.
   //:// KJS 02/07/13 - I am just realizing that every row could have the same columns but
   //:// perhaps they are not in the same x position, which then we wouldn't want to build as a table.
   //:// But for now I am going to keep this...
   //:SET CURSOR FIRST vReportDefRoot.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   //:ColumnCount = 0
   ColumnCount = 0;
   //:szColumnCountEqual = "Y"
   ZeidonStringCopy( szColumnCountEqual, 1, 0, "Y", 1, 0, 2 );
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
         //:szColumnCountEqual = "N"
         ZeidonStringCopy( szColumnCountEqual, 1, 0, "N", 1, 0, 2 );
      } 

      RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      //:END
   } 

   //:END

   //:// If there is no Shape control, and there is only one control, then build just that one.
   //:// I'm also now wondering if we should build the table even if there is one control because we would need the spacing.
   //:IF szShapeGrpExists = "" AND ControlCount = 1
   if ( ZeidonStringCompare( szShapeGrpExists, 1, 0, "", 1, 0, 2 ) == 0 && ControlCount == 1 )
   { 

      //:// There is only a single Control, so skip table.
      //:XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel )
      oTZRPSRCO_XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel );

      //:ELSE
   } 
   else
   { 
      //:// No shape controls but the column nbrs in each row are equal so build as one table.
      //:IF szShapeGrpExists = "" AND szColumnCountEqual = "Y"
      if ( ZeidonStringCompare( szShapeGrpExists, 1, 0, "", 1, 0, 2 ) == 0 && ZeidonStringCompare( szColumnCountEqual, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 

         //:szOutputLine = szIndentationChars + "<!-- NO Rpt_Shape Equal column nbr -->"
         ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, "<!-- NO Rpt_Shape Equal column nbr -->", 1, 0, 5001 );
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
         //:szOutputLine = szIndentationChars + ""
         ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

         //:szSubIndentationChars = szIndentationChars + "      "
         ZeidonStringCopy( szSubIndentationChars, 1, 0, szIndentationChars, 1, 0, 51 );
         ZeidonStringConcat( szSubIndentationChars, 1, 0, "      ", 1, 0, 51 );
         //:XSLT_BuildCtrlsAsTable( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel )
         oTZRPSRCO_XSLT_BuildCtrlsAsTable( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel );
         //:ELSE
      } 
      else
      { 
         //:IF  szShapeGrpExists = "" AND szColumnCountEqual = "N" 
         if ( ZeidonStringCompare( szShapeGrpExists, 1, 0, "", 1, 0, 2 ) == 0 && ZeidonStringCompare( szColumnCountEqual, 1, 0, "N", 1, 0, 2 ) == 0 )
         { 

            //:szOutputLine = szIndentationChars + "<!-- No Rpt_Shape calling XSLT_BuildCtrlsInRows each line new Table -->"
            ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "<!-- No Rpt_Shape calling XSLT_BuildCtrlsInRows each line new Table -->", 1, 0, 5001 );
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
            //:szOutputLine = szIndentationChars + ""
            ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
            ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
            //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

            //:// Build each row as a separate table.
            //:XSLT_BuildCtrlsInRows( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars )
            oTZRPSRCO_XSLT_BuildCtrlsInRows( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars );
            //:ELSE    
         } 
         else
         { 

            //:// At least some of the Controls are Shapes/Groups, so continue processing as Group.

            //:// Name Views for Debugging.
            //:szHierarchicalLevel = GroupHierarchicalLevel
            ZeidonStringConvertFromNumber( szHierarchicalLevel, 1, 0, 2, GroupHierarchicalLevel, (ZDecimal) 0.0, "I" );
            //:szSubObjectName = "vReportDefWorkGroup" + szHierarchicalLevel
            ZeidonStringCopy( szSubObjectName, 1, 0, "vReportDefWorkGroup", 1, 0, 21 );
            ZeidonStringConcat( szSubObjectName, 1, 0, szHierarchicalLevel, 1, 0, 21 );
            //:NAME VIEW vReportDefRoot szSubObjectName
            SetNameForView( vReportDefRoot, szSubObjectName, 0, zLEVEL_TASK );
            //:szSubObjectName = "vReportDefControl" + szHierarchicalLevel
            ZeidonStringCopy( szSubObjectName, 1, 0, "vReportDefControl", 1, 0, 21 );
            ZeidonStringConcat( szSubObjectName, 1, 0, szHierarchicalLevel, 1, 0, 21 );
            //:NAME VIEW vReportDef szSubObjectName
            SetNameForView( vReportDef, szSubObjectName, 0, zLEVEL_TASK );

            //:// Process Row/Column Work Structure to generate Tables and Columns.

            //:TopGroupPosition = vReportDefRoot.XSLT_WorkColumn.PSDLG_Y
            GetIntegerFromAttribute( &TopGroupPosition, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_Y" );
            //:RowCount = 0
            RowCount = 0;

            //:FOR EACH vReportDefRoot.XSLT_WorkRow
            RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 

               //:RowCount = RowCount + 1
               RowCount = RowCount + 1;

               //:// Build open table and column statements.
               //:// Open a new table.

               //:// If this is the topmost Group within the parent, we'll use it's Y position as the margin top. Otherwise,
               //:// the margin top will be zero.
               //:IF RowCount = 1
               if ( RowCount == 1 )
               { 
                  //:TableMarginTop  = vReportDefRoot.XSLT_WorkRow.PSDLG_Y
                  GetIntegerFromAttribute( &TableMarginTop, vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y" );
                  //:ELSE
               } 
               else
               { 
                  //:TableMarginTop  = 0
                  TableMarginTop = 0;
               } 

               //:END
               //:ConvertPxToInchesCH( vReportDefRoot, szTableMarginTop, TableMarginTop )
               oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, szTableMarginTop, TableMarginTop );

               //:// Compute the following values:
               //://     Table Padding Left (used to create a dummy column to shift first table column right as necessary.
               //://     Table Width (total of Padding Left and all sub Controls, which is determined from last column)
               //:SET CURSOR FIRST vReportDefRoot.XSLT_WorkColumn
               RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
               //:// KJS 11/28/12
               //:IF RESULT >= zCURSOR_SET 
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:TablePaddingLeft = vReportDefRoot.XSLT_WorkColumn.PSDLG_X
                  GetIntegerFromAttribute( &TablePaddingLeft, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
                  //:ConvertPxToInchesCH( vReportDefRoot, szTablePaddingLeft, TablePaddingLeft )
                  oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, szTablePaddingLeft, TablePaddingLeft );
               } 

               //:END

               //:// Table Width
               //:SET CURSOR LAST vReportDefRoot.XSLT_WorkColumn
               RESULT = SetCursorLastEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
               //:TableWidth = vReportDefRoot.XSLT_WorkColumn.PSDLG_X + vReportDefRoot.XSLT_WorkColumn.SZDLG_X
               GetIntegerFromAttribute( &lTempInteger_1, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
               GetIntegerFromAttribute( &lTempInteger_2, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
               TableWidth = lTempInteger_1 + lTempInteger_2;
               //:ConvertPxToInchesCH( vReportDefRoot, szTableWidth, TableWidth )
               oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, szTableWidth, TableWidth );

               //:// Table Block - Initially, we'll make the table block visible.
               //:szOutputLine = szIndentationChars + ""
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
               //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

               //:// KJS 12/12/12 - Taking this out for the moment and putting it right after we see that the control we are
               //:// on is a Rpt_Shape.  see what happens...
               //://szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + " > <!-- XSLT_GroupRecur For Row -->"
               //://GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               //://szOutputLine = szIndentationChars + ""
               //://GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )

               //:// Table statement
               //:szOutputLine = szIndentationChars + "<fo:table table-layout=" + QUOTES + "fixed" +
               //:            QUOTES + " width=" + QUOTES + "100%" +
               //:            QUOTES + " margin-top=" + QUOTES + szTableMarginTop +  "in" + QUOTES + "> <!-- In XSLT_GroupRecur -->"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table table-layout=", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "fixed", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, " width=", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "100%", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, szTableMarginTop, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "> <!-- In XSLT_GroupRecur -->", 1, 0, 5001 );
               //:            //QUOTES + " width=" + QUOTES + szTableWidth + "in" +
               //:            //QUOTES + " margin-top=" + QUOTES + szTableMarginTop +  "in" + QUOTES + ">"
               //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

               //:// Column Statements

               //:// Generate Padding Column, if TablePaddingLeft is > 0.
               //:IF TablePaddingLeft > 0
               if ( TablePaddingLeft > 0 )
               { 
                  //:szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szTablePaddingLeft + "in" + QUOTES + "/> <!-- This is just for padding -->"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, szTablePaddingLeft, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "/> <!-- This is just for padding -->", 1, 0, 5001 );
                  //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
               } 

               //:END

               //:// Get Width of table?.
               //:FOR EACH vReportDefRoot.XSLT_WorkColumn
               RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 

                  //:// The width is the difference between the starting position of this control and the starting position of the next control.
                  //:// If there is no next control, the width will be the actual width of the control.
                  //:CreateViewFromView( vReportDef3, vReportDefRoot )
                  CreateViewFromView( &vReportDef3, vReportDefRoot );
                  //:SET CURSOR NEXT vReportDef3.XSLT_WorkColumn
                  RESULT = SetCursorNextEntity( vReportDef3, "XSLT_WorkColumn", "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:CurrentWidth  = vReportDef3.XSLT_WorkColumn.PSDLG_X - vReportDefRoot.XSLT_WorkColumn.PSDLG_X
                     GetIntegerFromAttribute( &lTempInteger_3, vReportDef3, "XSLT_WorkColumn", "PSDLG_X" );
                     GetIntegerFromAttribute( &lTempInteger_4, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
                     CurrentWidth = lTempInteger_3 - lTempInteger_4;
                     //:ELSE
                  } 
                  else
                  { 
                     //:CurrentWidth  = vReportDefRoot.XSLT_WorkColumn.SZDLG_X
                     GetIntegerFromAttribute( &CurrentWidth, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
                  } 

                  //:END
                  //:DropView( vReportDef3 )
                  DropView( vReportDef3 );
                  //:ConvertPxToInchesCH( vReportDefRoot, szCurrentWidth, CurrentWidth )
                  oTZRPSRCO_ConvertPxToInchesCH( vReportDefRoot, szCurrentWidth, CurrentWidth );

                  //:szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szCurrentWidth + "in" + QUOTES + "/>"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, szCurrentWidth, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "/>", 1, 0, 5001 );
                  //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                  RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
               } 

               //:END

               //:// Generate table-body statement, table-row and block.
               //:szOutputLine = szIndentationChars + "<fo:table-body>"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table-body>", 1, 0, 5001 );
               //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

               //:szOutputLine = szIndentationChars + "  <fo:table-row>"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "  <fo:table-row>", 1, 0, 5001 );
               //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

               //:// Generate the table-cell statements, which will actually end up calling this recursive routine, XSLT_GroupRecur, to
               //:// process sub-controls.

               //:// Generate Padding Cell, if TablePaddingLeft is > 0.
               //:IF TablePaddingLeft > 0
               if ( TablePaddingLeft > 0 )
               { 
                  //:szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES + ">"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
                  //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

                  //:szOutputLine = szIndentationChars + "      <fo:block></fo:block> <!-- Just for padding -->"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "      <fo:block></fo:block> <!-- Just for padding -->", 1, 0, 5001 );
                  //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

                  //:szOutputLine = szIndentationChars + "    </fo:table-cell>"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
                  //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
               } 

               //:END

               //:FOR EACH vReportDefRoot.XSLT_WorkColumn
               RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               { 
                  //:szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES + ">"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
                  //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

                  //:// Process Sub Control Entities.
                  //:SET CURSOR FIRST vReportDef.Control WHERE vReportDef.Control.ZKey = vReportDefRoot.XSLT_WorkColumn.ControlZKey
                  GetIntegerFromAttribute( &lTempInteger_5, vReportDefRoot, "XSLT_WorkColumn", "ControlZKey" );
                  RESULT = SetCursorFirstEntityByInteger( vReportDef, "Control", "ZKey", lTempInteger_5, "" );

                  //:IF vReportDef.ControlDef.Tag = "Rpt_Shape" AND vReportDef.CtrlCtrl EXISTS 
                  lTempInteger_6 = CheckExistenceOfEntity( vReportDef, "CtrlCtrl" );
                  if ( CompareAttributeToString( vReportDef, "ControlDef", "Tag", "Rpt_Shape" ) == 0 && lTempInteger_6 == 0 )
                  { 

                     //:szOutputLine = szIndentationChars + "    <!-- ** Rpt_Shape " + vReportDef.Control.Tag + " ** -->"
                     ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, "    <!-- ** Rpt_Shape ", 1, 0, 5001 );
                     GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vReportDef, "Control", "Tag", "", 0 );
                     ZeidonStringConcat( szOutputLine, 1, 0, szTempString_1, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, " ** -->", 1, 0, 5001 );
                     //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

                     //:// KJS 12/12/12 - Moved this here from above...
                     //:XSLT_GetStyleShape( vReportDef, szStyle )
                     oTZRPSRCO_XSLT_GetStyleShape( vReportDef, szStyle );

                     //:// KJS 12/12/12 - Moved this here from above so we could check the control for a border.
                     //://szOutputLine = szIndentationChars + "    <fo:block border=" + QUOTES + "normal solid black" + QUOTES + " > <!-- XSLT_GroupRecur For Row -->"
                     //:szOutputLine = szIndentationChars + "    <fo:block " + szStyle + " > <!-- XSLT_GroupRecur For Row -->"
                     ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, "    <fo:block ", 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, szStyle, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, " > <!-- XSLT_GroupRecur For Row -->", 1, 0, 5001 );
                     //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                     //:szOutputLine = szIndentationChars + ""
                     ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
                     //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

                     //:// Position on subcontrols
                     //:CreateViewFromView( vReportDefRoot2, vReportDefRoot )
                     CreateViewFromView( &vReportDefRoot2, vReportDefRoot );
                     //:SetViewToSubobject( vReportDef, "CtrlCtrl" )
                     SetViewToSubobject( vReportDef, "CtrlCtrl" );
                     //:szSubIndentationChars = szIndentationChars + "      "
                     ZeidonStringCopy( szSubIndentationChars, 1, 0, szIndentationChars, 1, 0, 51 );
                     ZeidonStringConcat( szSubIndentationChars, 1, 0, "      ", 1, 0, 51 );

                     //:// Feel like here I should just look at see if this has sub controls. If so, then 
                     //:// Create new view so that we can save our current position.


                     //:// Call XSLT_GroupRecur recursively.
                     //:XSLT_GroupRecur( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel )
                     oTZRPSRCO_XSLT_GroupRecur( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel );

                     //:ResetViewFromSubobject( vReportDef )
                     ResetViewFromSubobject( vReportDef );
                     //:DropView( vReportDefRoot2 )
                     DropView( vReportDefRoot2 );

                     //:// Should we be getting rid of the workgroup that was used in recursive?

                     //:szOutputLine = szIndentationChars + "    </fo:block>"
                     ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, "    </fo:block>", 1, 0, 5001 );
                     //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                     //:szOutputLine = szIndentationChars + ""
                     ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                     ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
                     //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                     //:ELSE
                  } 
                  else
                  { 
                     //:// I don't think there is anything else is there besides Text or Graphic? Maybe an empty Rpt_Shape?
                     //:// Call XSLT_BuildCtrlsAsTable to format data.
                     //://XSLT_BuildCtrlsAsTable( vReportDefRoot2, vReportDef, vLOD, lFileHandle, szOutputLine, szSubIndentationChars, GroupHierarchicalLevel )
                     //:XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel )
                     oTZRPSRCO_XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel );
                  } 

                  //:END

                  //:// Close table-cell
                  //:szOutputLine = szIndentationChars + "    </fo:table-cell>"
                  ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
                  ZeidonStringConcat( szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
                  //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
                  RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
               } 

               //:END


               //:// Build close table statements
               //:szOutputLine = szIndentationChars + "  </fo:table-row>"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "  </fo:table-row>", 1, 0, 5001 );
               //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
               //:szOutputLine = szIndentationChars + "</fo:table-body>"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "</fo:table-body>", 1, 0, 5001 );
               //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
               //:szOutputLine = szIndentationChars + "</fo:table>"
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "</fo:table>", 1, 0, 5001 );
               //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
               //:szOutputLine = szIndentationChars + ""
               ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
               ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
               //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
               oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
               RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
            } 

            //:// KJS 12/12/12 - taking out...
            //://szOutputLine = szIndentationChars + "</fo:block>"
            //://GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
            //://szOutputLine = szIndentationChars + ""
            //://GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )

            //:END // FOR EACH vReportDefRoot.XSLT_WorkRow

            //:// Remove the Work Group for this hierarchical level.
            //:DELETE ENTITY vReportDefRoot.XSLT_WorkGroup
            RESULT = DeleteEntity( vReportDefRoot, "XSLT_WorkGroup", zPOS_NEXT );
         } 

         //:END  // Rpt_Shapes exist
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_GroupTable( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                 VIEW vReportDef     BASED ON LOD TZRPSRCO,
//:                 INTEGER lFileHandle,
//:                 STRING ( 5000 ) szOutputLine,
//:                 STRING ( 50 )   szIndentationChars,
//:                 INTEGER         RowCount,
//:                 STRING ( 32 )   szForEachName )

//:   VIEW vReportDef3 BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_GroupTable( zVIEW     vReportDefRoot,
                           zVIEW     vReportDef,
                           zLONG     lFileHandle,
                           zPCHAR    szOutputLine,
                           zPCHAR    szIndentationChars,
                           zLONG     RowCount,
                           zPCHAR    szForEachName )
{
   zVIEW     vReportDef3 = 0; 
   //:STRING ( 20 )  szCurrentWidth
   zCHAR     szCurrentWidth[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szPageWidth
   zCHAR     szPageWidth[ 21 ] = { 0 }; 
   //:STRING ( 30 )  szCellPaddingName
   zCHAR     szCellPaddingName[ 31 ] = { 0 }; 
   //:STRING ( 256 ) szAttributeMappingName
   zCHAR     szAttributeMappingName[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szConstantArea
   zCHAR     szConstantArea[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szAttribPrefix
   zCHAR     szAttribPrefix[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szStyle
   zCHAR     szStyle[ 257 ] = { 0 }; 
   //:STRING ( 10 )  szLineWidth
   zCHAR     szLineWidth[ 11 ] = { 0 }; 
   //:STRING ( 50 )  szCellPadding
   zCHAR     szCellPadding[ 51 ] = { 0 }; 
   //:STRING ( 20 )  szTableMarginTop
   zCHAR     szTableMarginTop[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTablePaddingLeft
   zCHAR     szTablePaddingLeft[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTableWidth
   zCHAR     szTableWidth[ 21 ] = { 0 }; 
   //:INTEGER       CurrentWidth
   zLONG     CurrentWidth = 0; 
   //:INTEGER       CurrentPositionY
   zLONG     CurrentPositionY = 0; 
   //:INTEGER       LastPositionY
   zLONG     LastPositionY = 0; 
   //:INTEGER       FirstColumnPosition
   zLONG     FirstColumnPosition = 0; 
   //:INTEGER       TopGroupPosition
   zLONG     TopGroupPosition = 0; 
   //:INTEGER       TextColumnCount
   zLONG     TextColumnCount = 0; 
   //:INTEGER       TablePaddingLeft
   zLONG     TablePaddingLeft = 0; 
   //:INTEGER       TableMarginTop
   zLONG     TableMarginTop = 0; 
   //:INTEGER       TableWidth
   zLONG     TableWidth = 0; 
   //:INTEGER       LastRowPosition
   zLONG     LastRowPosition = 0; 
   //:INTEGER       LastRowHeight
   zLONG     LastRowHeight = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 


   //:// Build Table

   //:// Build open table and column statements.
   //:// Open a new table.
   //:szOutputLine = szIndentationChars + "<!-- In XSLT_GroupTable -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<!-- In XSLT_GroupTable -->", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );


   //:szCellPaddingName = "DefaultCellPadding"
   ZeidonStringCopy( szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 31 );

   //:// Compute the following values:

   //:// The value of Margin Top depends on whether this is the first row or any other row.
   //:// 1. The first row is simply the Y Position.
   //:// 2. Any later row is the Y Position minus the previous Row position and Row height (which is Y size of first control).
   //:IF RowCount = 1
   if ( RowCount == 1 )
   { 
      //:TableMarginTop  = vReportDefRoot.XSLT_WorkRow.PSDLG_Y
      GetIntegerFromAttribute( &TableMarginTop, vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y" );
      //:ELSE
   } 
   else
   { 
      //:CreateViewFromView( vReportDef3, vReportDefRoot )
      CreateViewFromView( &vReportDef3, vReportDefRoot );
      //:SET CURSOR PREVIOUS vReportDef3.XSLT_WorkRow
      RESULT = SetCursorPrevEntity( vReportDef3, "XSLT_WorkRow", "" );
      //:LastRowPosition = vReportDef3.XSLT_WorkRow.PSDLG_Y
      GetIntegerFromAttribute( &LastRowPosition, vReportDef3, "XSLT_WorkRow", "PSDLG_Y" );
      //:LastRowHeight   = vReportDef3.XSLT_WorkColumn.SZDLG_Y
      GetIntegerFromAttribute( &LastRowHeight, vReportDef3, "XSLT_WorkColumn", "SZDLG_Y" );
      //:TableMarginTop  = vReportDefRoot.XSLT_WorkRow.PSDLG_Y - LastRowPosition - LastRowHeight
      GetIntegerFromAttribute( &lTempInteger_0, vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y" );
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
   oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szTableMarginTop, TableMarginTop );

   //:// Compute Padding Left for first column of the row.
   //:// We'll assume 100 is normal indentation and anything after that must shift the columns right.
   //:SET CURSOR FIRST vReportDefRoot.XSLT_WorkColumn
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
   //:FirstColumnPosition = vReportDefRoot.XSLT_WorkColumn.PSDLG_X
   GetIntegerFromAttribute( &FirstColumnPosition, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
   //://TablePaddingLeft = FirstColumnPosition - 100
   //:TablePaddingLeft = FirstColumnPosition
   TablePaddingLeft = FirstColumnPosition;
   //:TraceLineI("*** XSLT_GroupTable TablePaddingLeft *** ", TablePaddingLeft )
   TraceLineI( "*** XSLT_GroupTable TablePaddingLeft *** ", TablePaddingLeft );
   //:IF TablePaddingLeft < 0
   if ( TablePaddingLeft < 0 )
   { 
      //:TablePaddingLeft = 0
      TablePaddingLeft = 0;
   } 

   //:END
   //:ConvertPxToInchesCH( vReportDef, szTablePaddingLeft, TablePaddingLeft )
   oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szTablePaddingLeft, TablePaddingLeft );

   //:// Table Width
   //:SET CURSOR LAST vReportDefRoot.XSLT_WorkColumn
   RESULT = SetCursorLastEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
   //:TableWidth = vReportDefRoot.XSLT_WorkColumn.PSDLG_X + vReportDefRoot.XSLT_WorkColumn.SZDLG_X
   GetIntegerFromAttribute( &lTempInteger_1, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
   GetIntegerFromAttribute( &lTempInteger_2, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
   TableWidth = lTempInteger_1 + lTempInteger_2;
   //:ConvertPxToInchesCH( vReportDef, szTableWidth, TableWidth )
   oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szTableWidth, TableWidth );

   //:// Table Block - Initially, we'll make the table block visible.
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //://szOutputLine = szIndentationChars + "<fo:block border=" + QUOTES + "normal solid black" + QUOTES + " > <!-- XSLT_GroupTable -->"
   //:szOutputLine = szIndentationChars + "<fo:block > <!-- XSLT_GroupTable -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<fo:block > <!-- XSLT_GroupTable -->", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:// Table statement
   //:szOutputLine = szIndentationChars + "<fo:table table-layout=" + QUOTES + "fixed" +
   //:               QUOTES + " width=" + QUOTES + "100%" +
   //:               QUOTES + " margin-top=" + QUOTES + szTableMarginTop +  "in" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table table-layout=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "fixed", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " width=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "100%", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, szTableMarginTop, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:               //QUOTES + " width=" + QUOTES + szTableWidth + "in" +
   //:               //QUOTES + " margin-top=" + QUOTES + szTableMarginTop +  "in" + QUOTES + ">"
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:// Column Statements
   //:// Generate Padding Column, if TablePaddingLeft is > 0.
   //:IF TablePaddingLeft > 0
   if ( TablePaddingLeft > 0 )
   { 
      //:szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szTablePaddingLeft + "in" + QUOTES + "/> <!-- This is just for padding -->"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szTablePaddingLeft, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "/> <!-- This is just for padding -->", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   } 

   //:END

   //:// Generate Column Statement for each work Column entry.
   //:FOR EACH vReportDefRoot.XSLT_WorkColumn
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:// The width is the difference between the starting position of this control and the starting position of the next control.
      //:// If there is no next control, the width will be the actual width of the control.
      //:CreateViewFromView( vReportDef3, vReportDefRoot )
      CreateViewFromView( &vReportDef3, vReportDefRoot );
      //:SET CURSOR NEXT vReportDef3.XSLT_WorkColumn
      RESULT = SetCursorNextEntity( vReportDef3, "XSLT_WorkColumn", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:CurrentWidth  = vReportDef3.XSLT_WorkColumn.PSDLG_X - vReportDefRoot.XSLT_WorkColumn.PSDLG_X
         GetIntegerFromAttribute( &lTempInteger_3, vReportDef3, "XSLT_WorkColumn", "PSDLG_X" );
         GetIntegerFromAttribute( &lTempInteger_4, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
         CurrentWidth = lTempInteger_3 - lTempInteger_4;
         //:ELSE
      } 
      else
      { 
         //:CurrentWidth  = vReportDefRoot.XSLT_WorkColumn.SZDLG_X
         GetIntegerFromAttribute( &CurrentWidth, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
      } 

      //:END
      //:DropView( vReportDef3 )
      DropView( vReportDef3 );
      //:ConvertPxToInchesCH( vReportDef, szCurrentWidth, CurrentWidth )
      oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szCurrentWidth, CurrentWidth );
      //:szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szCurrentWidth + "in" + QUOTES + "/>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szCurrentWidth, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "/>", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
      RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
   } 

   //:END

   //:// Generate table-body statement, table-row and block.
   //:szOutputLine = szIndentationChars + "<fo:table-body>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table-body>", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:// If this is for a repeating Entity (based on For-Each flag, create for-each statement.
   //:IF szForEachName != ""
   if ( ZeidonStringCompare( szForEachName, 1, 0, "", 1, 0, 33 ) != 0 )
   { 
      //:szOutputLine = szIndentationChars + "<xsl:for-each select=" + QUOTES + szForEachName + QUOTES + ">"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "<xsl:for-each select=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szForEachName, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   } 

   //:END

   //:szOutputLine = szIndentationChars + "  <fo:table-row>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "  <fo:table-row>", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:// Generate the table-cell statements, which will contain either the text from the control or the attribute mapping data.

   //:// Generate Padding Cell, if TablePaddingLeft is > 0.
   //:IF TablePaddingLeft > 0
   if ( TablePaddingLeft > 0 )
   { 
      //:szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES + ">"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

      //:szOutputLine = szIndentationChars + "      <fo:block></fo:block>  <!-- Just for padding -->"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "      <fo:block></fo:block>  <!-- Just for padding -->", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

      //:szOutputLine = szIndentationChars + "    </fo:table-cell>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   } 

   //:END

   //:/*   
   //:// This is for page heading/footing
   //://IF vReportDefRoot.XSLT_WorkGroup.RecursiveNumber = -1
   //:IF vReportDefRoot.XSLT_WorkGroup.EntityName = "PageHeader" OR vReportDefRoot.XSLT_WorkGroup.EntityName = "PageFooter" 
   //:   szAttribPrefix = "zOI/" + vLOD.LOD_EntityParent.Name + "/"
   //:ELSE
   //:   szAttribPrefix = ""
   //:END
   //:*/

   //:TextColumnCount = 0
   TextColumnCount = 0;
   //:FOR EACH vReportDefRoot.XSLT_WorkColumn
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES + ">"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

      //:// Block statement is different depending on whether or not there is mapping for the Control.
      //:// Also, any Control 1) without mapping, 2) with null Text value and 3) with SZDLG_Y <= 21.
      //:SET CURSOR FIRST vReportDef.Control WHERE vReportDef.Control.ZKey = vReportDefRoot.XSLT_WorkColumn.ControlZKey
      GetIntegerFromAttribute( &lTempInteger_5, vReportDefRoot, "XSLT_WorkColumn", "ControlZKey" );
      RESULT = SetCursorFirstEntityByInteger( vReportDef, "Control", "ZKey", lTempInteger_5, "" );
      //:// KJS 11/28/12
      //:IF RESULT >= zCURSOR_SET 
      if ( RESULT >= zCURSOR_SET )
      { 

         //:XSLT_BuildControl( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars )
         oTZRPSRCO_XSLT_BuildControl( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars );
      } 


      //:END

      //:// Close table-cell
      //:szOutputLine = szIndentationChars + "    </fo:table-cell>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
      RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
   } 


   //:END

   //:// Build close table statements
   //:szOutputLine = szIndentationChars + "  </fo:table-row>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "  </fo:table-row>", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:IF szForEachName != ""
   if ( ZeidonStringCompare( szForEachName, 1, 0, "", 1, 0, 33 ) != 0 )
   { 
      //:szOutputLine = szIndentationChars + "  </xsl:for-each>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "  </xsl:for-each>", 1, 0, 5001 );
      //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   } 

   //:END

   //:szOutputLine = szIndentationChars + "</fo:table-body>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "</fo:table-body>", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + "</fo:table>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "</fo:table>", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + "</fo:block>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
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

//:   VIEW vReportDef BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_HeaderFooter( zVIEW     vReportDefRoot,
                             zVIEW     vLOD,
                             zLONG     lFileHandle,
                             zPCHAR    szOutputLine,
                             zPCHAR    szIndentationChars,
                             zPCHAR    szForEachName )
{
   zVIEW     vReportDef = 0; 
   //:INTEGER PosY
   zLONG     PosY = 0; 
   //:INTEGER CurrentWidth
   zLONG     CurrentWidth = 0; 
   //:INTEGER ColumnCount
   zLONG     ColumnCount = 0; 
   //:INTEGER CellPadding
   zLONG     CellPadding = 0; 
   //:INTEGER GroupHierarchicalLevel
   zLONG     GroupHierarchicalLevel = 0; 
   //:STRING ( 50 )  szGroupSetName
   zCHAR     szGroupSetName[ 51 ] = { 0 }; 
   //:STRING ( 10 )  szPageWidth
   zCHAR     szPageWidth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szCurrentWidth
   zCHAR     szCurrentWidth[ 11 ] = { 0 }; 
   //:STRING ( 1 )   szSingleRowFlag
   zCHAR     szSingleRowFlag[ 2 ] = { 0 }; 
   //:STRING ( 32 )  szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szAttributeName
   zCHAR     szAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 20 )  szCellPadding
   zCHAR     szCellPadding[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szCellPaddingName
   zCHAR     szCellPaddingName[ 21 ] = { 0 }; 
   //:STRING ( 50 )  szSubIndentationChars
   zCHAR     szSubIndentationChars[ 51 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 


   //:szPageWidth = "7.5"
   ZeidonStringCopy( szPageWidth, 1, 0, "7.5", 1, 0, 11 );

   //:// The Header/Footer starts block, table, ... table-cell.

   //:// Block
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + "<fo:block font-size=" + QUOTES + "9pt" + QUOTES + "> <!-- XSLT_HeaderFooter -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<fo:block font-size=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "9pt", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "> <!-- XSLT_HeaderFooter -->", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );

   //:IF vReportDefRoot.Control EXISTS 
   lTempInteger_0 = CheckExistenceOfEntity( vReportDefRoot, "Control" );
   if ( lTempInteger_0 == 0 )
   { 

      //:   // Make sure Controls are in row/column order.
      //:   OrderEntityForView( vReportDefRoot, "Control", "PSDLG_Y A PSDLG_X A" )
      OrderEntityForView( vReportDefRoot, "Control", "PSDLG_Y A PSDLG_X A" );

      //:   // Check if all controls are on a single line, without a subcontrol.
      //:   //    If so, we will create a table entry with a column for each control.
      //:   //    If not, we will create a table entry with a single cell.
      //:   szSingleRowFlag = "Y"
      ZeidonStringCopy( szSingleRowFlag, 1, 0, "Y", 1, 0, 2 );
      //:   IF vReportDefRoot.Control EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vReportDefRoot, "Control" );
      if ( lTempInteger_1 == 0 )
      { 
         //:   PosY = vReportDefRoot.Control.PSDLG_Y
         GetIntegerFromAttribute( &PosY, vReportDefRoot, "Control", "PSDLG_Y" );
         //:   FOR EACH vReportDefRoot.Control
         RESULT = SetCursorFirstEntity( vReportDefRoot, "Control", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:   IF vReportDefRoot.CtrlCtrl EXISTS OR vReportDefRoot.Control.PSDLG_Y != PosY
            lTempInteger_2 = CheckExistenceOfEntity( vReportDefRoot, "CtrlCtrl" );
            if ( lTempInteger_2 == 0 || CompareAttributeToInteger( vReportDefRoot, "Control", "PSDLG_Y", PosY ) != 0 )
            { 
               //:   szSingleRowFlag = "N"
               ZeidonStringCopy( szSingleRowFlag, 1, 0, "N", 1, 0, 2 );
            } 

            RESULT = SetCursorNextEntity( vReportDefRoot, "Control", "" );
            //:   END
         } 

         //:   END
      } 

      //:   END
      //:   IF szSingleRowFlag = "N"
      if ( ZeidonStringCompare( szSingleRowFlag, 1, 0, "N", 1, 0, 2 ) == 0 )
      { 
         //:   // Go to process more complex Group.
         //:                           
         //:   CreateViewFromView( vReportDef, vReportDefRoot )
         CreateViewFromView( &vReportDef, vReportDefRoot );
         //:   NAME VIEW vReportDef "vReportDefTop"
         SetNameForView( vReportDef, "vReportDefTop", 0, zLEVEL_TASK );
         //:   XSLT_GroupRecur( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel )
         oTZRPSRCO_XSLT_GroupRecur( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel );
         //:   DropView( vReportDef )
         DropView( vReportDef );
         //:   
         //:ELSE
      } 
      else
      { 
         //:   CreateViewFromView( vReportDef, vReportDefRoot )
         CreateViewFromView( &vReportDef, vReportDefRoot );
         //:   // Detail entity has single row.
         //:   // Build work subobject, with a single row and columns.
         //:   CREATE ENTITY vReportDefRoot.XSLT_WorkRow
         RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkRow", zPOS_AFTER );
         //:   IF vReportDefRoot.Control EXISTS 
         lTempInteger_3 = CheckExistenceOfEntity( vReportDefRoot, "Control" );
         if ( lTempInteger_3 == 0 )
         { 
            //:   vReportDefRoot.XSLT_WorkRow.PSDLG_Y = vReportDefRoot.Control.PSDLG_Y
            SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y", vReportDefRoot, "Control", "PSDLG_Y" );
            //:   FOR EACH vReportDefRoot.Control
            RESULT = SetCursorFirstEntity( vReportDefRoot, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            { 
               //:   CREATE ENTITY vReportDefRoot.XSLT_WorkColumn
               RESULT = CreateEntity( vReportDefRoot, "XSLT_WorkColumn", zPOS_AFTER );
               //:   vReportDefRoot.XSLT_WorkColumn.ControlZKey = vReportDefRoot.Control.ZKey
               SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkColumn", "ControlZKey", vReportDefRoot, "Control", "ZKey" );
               //:   vReportDefRoot.XSLT_WorkColumn.ControlType = vReportDefRoot.ControlDef.Tag
               SetAttributeFromAttribute( vReportDefRoot, "XSLT_WorkColumn", "ControlType", vReportDefRoot, "ControlDef", "Tag" );
               //:   SetMatchingAttributesByName( vReportDefRoot, "XSLT_WorkColumn",  vReportDefRoot, "Control", zSET_NULL )
               SetMatchingAttributesByName( vReportDefRoot, "XSLT_WorkColumn", vReportDefRoot, "Control", zSET_NULL );
               RESULT = SetCursorNextEntity( vReportDefRoot, "Control", "" );
            } 

            //:   END
         } 

         //:   END
         //:   // Go to build the table.
         //:   XSLT_GroupTable( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars, 1, "" )
         oTZRPSRCO_XSLT_GroupTable( vReportDefRoot, vReportDef, lFileHandle, szOutputLine, szIndentationChars, 1, "" );
         //:   DropView( vReportDef )
         DropView( vReportDef );
      } 

      //:   END
   } 

   //:END

   //:szOutputLine = szIndentationChars + "</fo:block>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
   //:GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_MainHeader( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                 INTEGER lFileHandle,
//:                 STRING ( 5000 ) szOutputLine )
//:                 
//:   VIEW vLOD BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_MainHeader( zVIEW     vReportDef,
                           zLONG     lFileHandle,
                           zPCHAR    szOutputLine )
{
   zVIEW     vLOD = 0; 
   //:STRING ( 20 ) szHeaderHeight
   zCHAR     szHeaderHeight[ 21 ] = { 0 }; 
   //:STRING ( 20 ) szFooterHeight
   zCHAR     szFooterHeight[ 21 ] = { 0 }; 
   //:INTEGER       HeaderHeight
   zLONG     HeaderHeight = 0; 
   //:INTEGER       FooterHeight
   zLONG     FooterHeight = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 


   //:GET VIEW  vLOD  NAMED "LOD_XSLT"
   RESULT = GetViewByName( &vLOD, "LOD_XSLT", vReportDef, zLEVEL_TASK );

   //:// Generate the XSLT Header statements.

   //:// Build the XML file header.
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
   //:SysWriteLine( vReportDef, lFileHandle, szOutputLine )
   SysWriteLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "<xsl:stylesheet version=" + QUOTES + "1.0" + QUOTES
   ZeidonStringCopy( szOutputLine, 1, 0, "<xsl:stylesheet version=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "1.0", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  xmlns:xsl=" + QUOTES + "http://www.w3.org/1999/XSL/Transform" + QUOTES
   ZeidonStringCopy( szOutputLine, 1, 0, "  xmlns:xsl=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "http://www.w3.org/1999/XSL/Transform", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  xmlns:fo=" + QUOTES + "http://www.w3.org/1999/XSL/Format" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "  xmlns:fo=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "http://www.w3.org/1999/XSL/Format", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <xsl:output method=" + QUOTES + "xml" + QUOTES + " indent=" + QUOTES + "yes" + QUOTES + "/>"
   ZeidonStringCopy( szOutputLine, 1, 0, "  <xsl:output method=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "xml", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " indent=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "yes", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "/>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Build Default Padding <xsl:attribute-set.
   //:szOutputLine = "<xsl:attribute-set name=" + QUOTES + "DefaultCellPadding" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "<xsl:attribute-set name=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "DefaultCellPadding", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <xsl:attribute name=" + QUOTES + "padding-left" + QUOTES + ">5px</xsl:attribute>"
   ZeidonStringCopy( szOutputLine, 1, 0, "  <xsl:attribute name=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "padding-left", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">5px</xsl:attribute>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <xsl:attribute name=" + QUOTES + "padding-top" + QUOTES + ">2px</xsl:attribute>"
   ZeidonStringCopy( szOutputLine, 1, 0, "  <xsl:attribute name=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "padding-top", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">2px</xsl:attribute>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <xsl:attribute name=" + QUOTES + "padding-bottom" + QUOTES + ">2px</xsl:attribute>"
   ZeidonStringCopy( szOutputLine, 1, 0, "  <xsl:attribute name=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "padding-bottom", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">2px</xsl:attribute>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "</xsl:attribute-set>"
   ZeidonStringCopy( szOutputLine, 1, 0, "</xsl:attribute-set>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Build base template and root statements.
   //:szOutputLine = "<xsl:template match=" + QUOTES + "/" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "<xsl:template match=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "/", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "<fo:root>"
   ZeidonStringCopy( szOutputLine, 1, 0, "<fo:root>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = " <fo:layout-master-set>"
   ZeidonStringCopy( szOutputLine, 1, 0, " <fo:layout-master-set>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  <fo:simple-page-master master-name=" + QUOTES + vReportDef.Report.Tag + "-page" + QUOTES + " page-height=" + QUOTES + "11in" + QUOTES
   ZeidonStringCopy( szOutputLine, 1, 0, "  <fo:simple-page-master master-name=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vReportDef, "Report", "Tag", "", 0 );
   ZeidonStringConcat( szOutputLine, 1, 0, szTempString_0, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "-page", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " page-height=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "11in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   page-width=" + QUOTES + "8.5in" + QUOTES + " margin-top=" + QUOTES + ".5in" + QUOTES + " margin-bottom=" + QUOTES + ".5in" + QUOTES
   ZeidonStringCopy( szOutputLine, 1, 0, "   page-width=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "8.5in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " margin-top=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ".5in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " margin-bottom=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ".5in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //://szOutputLine = "   margin-left=" + QUOTES + ".5in" + QUOTES + " margin-right=" + QUOTES + ".5in" + QUOTES + ">"
   //:szOutputLine = "   margin-left=" + QUOTES + "0in" + QUOTES + " margin-right=" + QUOTES + "0in" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "   margin-left=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "0in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " margin-right=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "0in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PH"    
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PH", "" );
   //:IF RESULT >= zCURSOR_SET 
   if ( RESULT >= zCURSOR_SET )
   { 

      //:HeaderHeight = vReportDef.Group.SZDLG_Y 
      GetIntegerFromAttribute( &HeaderHeight, vReportDef, "Group", "SZDLG_Y" );
      //:ConvertPxToInchesCH( vReportDef, szHeaderHeight, HeaderHeight )
      oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szHeaderHeight, HeaderHeight );
   } 

   //:END

   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PF"    
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PF", "" );
   //:IF RESULT >= zCURSOR_SET 
   if ( RESULT >= zCURSOR_SET )
   { 

      //:FooterHeight = vReportDef.Group.SZDLG_Y 
      GetIntegerFromAttribute( &FooterHeight, vReportDef, "Group", "SZDLG_Y" );
      //:ConvertPxToInchesCH( vReportDef, szFooterHeight, FooterHeight )
      oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szFooterHeight, FooterHeight );
   } 

   //:END

   //:szOutputLine = "   <fo:region-body margin-top=" + QUOTES + szHeaderHeight + "in" + QUOTES
   ZeidonStringCopy( szOutputLine, 1, 0, "   <fo:region-body margin-top=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, szHeaderHeight, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   //:szOutputLine = szOutputLine + " margin-bottom=" + QUOTES + szFooterHeight + "in" + QUOTES + "/>"
   ZeidonStringConcat( szOutputLine, 1, 0, " margin-bottom=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, szFooterHeight, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "/>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:region-before extent=" + QUOTES + szHeaderHeight + "in" + QUOTES + " />"
   ZeidonStringCopy( szOutputLine, 1, 0, "   <fo:region-before extent=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, szHeaderHeight, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " />", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:region-after extent=" + QUOTES + szFooterHeight + "in" + QUOTES + " />"
   ZeidonStringCopy( szOutputLine, 1, 0, "   <fo:region-after extent=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, szFooterHeight, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " />", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  </fo:simple-page-master>"
   ZeidonStringCopy( szOutputLine, 1, 0, "  </fo:simple-page-master>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = " </fo:layout-master-set>"
   ZeidonStringCopy( szOutputLine, 1, 0, " </fo:layout-master-set>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Build Page Sequence.
   //:szOutputLine = " <fo:page-sequence master-reference=" + QUOTES + vReportDef.Report.Tag + "-page" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, " <fo:page-sequence master-reference=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vReportDef, "Report", "Tag", "", 0 );
   ZeidonStringConcat( szOutputLine, 1, 0, szTempString_1, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "-page", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:static-content flow-name=" + QUOTES + "xsl-region-before" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "   <fo:static-content flow-name=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "xsl-region-before", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //://szOutputLine = "     <fo:block text-align=" + QUOTES + "center" + QUOTES + ">"
   //:szOutputLine = "     <fo:block>"
   ZeidonStringCopy( szOutputLine, 1, 0, "     <fo:block>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );


   //:// Process Page Header    
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PH"    
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PH", "" );
   //:IF RESULT >= zCURSOR_SET 
   if ( RESULT >= zCURSOR_SET )
   { 

      //:IF vReportDef.Control EXISTS 
      lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "Control" );
      if ( lTempInteger_0 == 0 )
      { 

         //:FOR EACH vReportDef.XSLT_WorkGroup
         RESULT = SetCursorFirstEntity( vReportDef, "XSLT_WorkGroup", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:DELETE ENTITY vReportDef.XSLT_WorkGroup NONE  
            RESULT = DeleteEntity( vReportDef, "XSLT_WorkGroup", zREPOS_NONE );
            RESULT = SetCursorNextEntity( vReportDef, "XSLT_WorkGroup", "" );
         } 

         //:END
         //:CREATE ENTITY vReportDef.XSLT_WorkGroup 
         RESULT = CreateEntity( vReportDef, "XSLT_WorkGroup", zPOS_AFTER );

         //:// Using this to show that it is the PageHeader or PageFooter
         //:vReportDef.XSLT_WorkGroup.RecursiveNumber = -1
         SetAttributeFromInteger( vReportDef, "XSLT_WorkGroup", "RecursiveNumber", -1 );
         //:vReportDef.Report.wOriginalViewName = "zOI/PageHeader/"
         SetAttributeFromString( vReportDef, "Report", "wOriginalViewName", "zOI/PageHeader/" );

         //:szOutputLine = "  <!-- Page Header  -->"
         ZeidonStringCopy( szOutputLine, 1, 0, "  <!-- Page Header  -->", 1, 0, 5001 );
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

         //:XSLT_HeaderFooter( vReportDef, vLOD, lFileHandle, szOutputLine, "      ", "PageHeader" )
         oTZRPSRCO_XSLT_HeaderFooter( vReportDef, vLOD, lFileHandle, szOutputLine, "      ", "PageHeader" );
      } 

      //:END
   } 

   //:END

   //:szOutputLine = "     </fo:block>"
   ZeidonStringCopy( szOutputLine, 1, 0, "     </fo:block>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   </fo:static-content>"
   ZeidonStringCopy( szOutputLine, 1, 0, "   </fo:static-content>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   <fo:static-content flow-name=" + QUOTES + "xsl-region-after" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "   <fo:static-content flow-name=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "xsl-region-after", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "     <fo:block font-size=" + QUOTES + "8pt" + QUOTES + " >"
   ZeidonStringCopy( szOutputLine, 1, 0, "     <fo:block font-size=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "8pt", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, " >", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Process Page Footer
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Type = "PF"
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Type", "PF", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 

      //:IF vReportDef.Control EXISTS 
      lTempInteger_1 = CheckExistenceOfEntity( vReportDef, "Control" );
      if ( lTempInteger_1 == 0 )
      { 

         //:FOR EACH vReportDef.XSLT_WorkGroup
         RESULT = SetCursorFirstEntity( vReportDef, "XSLT_WorkGroup", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:DELETE ENTITY vReportDef.XSLT_WorkGroup NONE  
            RESULT = DeleteEntity( vReportDef, "XSLT_WorkGroup", zREPOS_NONE );
            RESULT = SetCursorNextEntity( vReportDef, "XSLT_WorkGroup", "" );
         } 

         //:END
         //:   
         //:CREATE ENTITY vReportDef.XSLT_WorkGroup 
         RESULT = CreateEntity( vReportDef, "XSLT_WorkGroup", zPOS_AFTER );
         //: 
         //:vReportDef.XSLT_WorkGroup.RecursiveNumber = -1
         SetAttributeFromInteger( vReportDef, "XSLT_WorkGroup", "RecursiveNumber", -1 );

         //:vReportDef.Report.wOriginalViewName = "zOI/PageFooter/"
         SetAttributeFromString( vReportDef, "Report", "wOriginalViewName", "zOI/PageFooter/" );
         //:szOutputLine = "  <!-- Page Footer  -->"
         ZeidonStringCopy( szOutputLine, 1, 0, "  <!-- Page Footer  -->", 1, 0, 5001 );
         //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

         //:XSLT_HeaderFooter( vReportDef, vLOD, lFileHandle, szOutputLine, "      ", "PageFooter" )
         oTZRPSRCO_XSLT_HeaderFooter( vReportDef, vLOD, lFileHandle, szOutputLine, "      ", "PageFooter" );
      } 

      //:END
   } 

   //:END

   //:// Set this back to nothing because we don't need an attribute prefix.
   //:vReportDef.Report.wOriginalViewName = ""
   SetAttributeFromString( vReportDef, "Report", "wOriginalViewName", "" );

   //:szOutputLine = "     </fo:block>"
   ZeidonStringCopy( szOutputLine, 1, 0, "     </fo:block>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   </fo:static-content>"
   ZeidonStringCopy( szOutputLine, 1, 0, "   </fo:static-content>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Build Flow.
   //:szOutputLine = "   <fo:flow flow-name=" + QUOTES + "xsl-region-body" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "   <fo:flow flow-name=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "xsl-region-body", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "     <xsl:apply-templates/>"
   ZeidonStringCopy( szOutputLine, 1, 0, "     <xsl:apply-templates/>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:// We need this declaration of "last-page" so that the ref-id in page-number-citation works.
   //:szOutputLine = "     <fo:block id=" + QUOTES + "last-page" + QUOTES + "></fo:block>"
   ZeidonStringCopy( szOutputLine, 1, 0, "     <fo:block id=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "last-page", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "></fo:block>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "   </fo:flow>"
   ZeidonStringCopy( szOutputLine, 1, 0, "   </fo:flow>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Close Page, Root and Template.
   //:szOutputLine = " </fo:page-sequence>"
   ZeidonStringCopy( szOutputLine, 1, 0, " </fo:page-sequence>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "</fo:root> "
   ZeidonStringCopy( szOutputLine, 1, 0, "</fo:root> ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "</xsl:template>"
   ZeidonStringCopy( szOutputLine, 1, 0, "</xsl:template>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "  "
   ZeidonStringCopy( szOutputLine, 1, 0, "  ", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:XSLT_BuildCtrlsAsTable( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                   VIEW vReportDef  BASED ON LOD TZRPSRCO,
//:                   VIEW vLOD           BASED ON LOD TZZOLODO,
//:                   INTEGER lFileHandle,
//:                   STRING ( 5000 ) szOutputLine,
//:                   STRING ( 50 )  szIndentationChars,
//:                   INTEGER GroupHierarchicalLevel )

//:   VIEW vReportDef3 BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_XSLT_BuildCtrlsAsTable( zVIEW     vReportDefRoot,
                                  zVIEW     vReportDef,
                                  zVIEW     vLOD,
                                  zLONG     lFileHandle,
                                  zPCHAR    szOutputLine,
                                  zPCHAR    szIndentationChars,
                                  zLONG     GroupHierarchicalLevel )
{
   zVIEW     vReportDef3 = 0; 
   //:STRING ( 20 )  szCurrentWidth
   zCHAR     szCurrentWidth[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szPageWidth
   zCHAR     szPageWidth[ 21 ] = { 0 }; 
   //:STRING ( 30 )  szCellPaddingName
   zCHAR     szCellPaddingName[ 31 ] = { 0 }; 
   //:STRING ( 50 )  szAttributeMappingName
   zCHAR     szAttributeMappingName[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szConstantArea
   zCHAR     szConstantArea[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szReportDefName
   zCHAR     szReportDefName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSubIndentationChars
   zCHAR     szSubIndentationChars[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szCellPadding
   zCHAR     szCellPadding[ 51 ] = { 0 }; 
   //:STRING ( 20 )  szTableMarginTop
   zCHAR     szTableMarginTop[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTablePaddingLeft
   zCHAR     szTablePaddingLeft[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szTableWidth
   zCHAR     szTableWidth[ 21 ] = { 0 }; 
   //:STRING ( 20 )  szPaddingTop
   zCHAR     szPaddingTop[ 21 ] = { 0 }; 
   //:STRING ( 5 )   szColumnCount
   zCHAR     szColumnCount[ 6 ] = { 0 }; 
   //:STRING ( 200 ) szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:INTEGER       CurrentWidth
   zLONG     CurrentWidth = 0; 
   //:INTEGER       CurrentRowPositionX
   zLONG     CurrentRowPositionX = 0; 
   //:INTEGER       CurrentPositionY
   zLONG     CurrentPositionY = 0; 
   //:INTEGER       LastPositionY
   zLONG     LastPositionY = 0; 
   //:INTEGER       LastPositionRowY
   zLONG     LastPositionRowY = 0; 
   //:INTEGER       LastPositionX
   zLONG     LastPositionX = 0; 
   //:INTEGER       LastWidthX
   zLONG     LastWidthX = 0; 
   //:INTEGER       FirstColumnPosition
   zLONG     FirstColumnPosition = 0; 
   //:INTEGER       TopGroupPosition
   zLONG     TopGroupPosition = 0; 
   //:INTEGER       ColumnCount
   zLONG     ColumnCount = 0; 
   //:INTEGER       FirstColumnCount
   zLONG     FirstColumnCount = 0; 
   //:INTEGER       TextColumnCount
   zLONG     TextColumnCount = 0; 
   //:INTEGER       CellPadding
   zLONG     CellPadding = 0; 
   //:INTEGER       RowCount
   zLONG     RowCount = 0; 
   //:INTEGER       TablePaddingLeft
   zLONG     TablePaddingLeft = 0; 
   //:INTEGER       TableMarginTop
   zLONG     TableMarginTop = 0; 
   //:INTEGER       TableWidth
   zLONG     TableWidth = 0; 
   //:INTEGER       MaximumWidth
   zLONG     MaximumWidth = 0; 
   //:INTEGER       PaddingTop
   zLONG     PaddingTop = 0; 
   //:INTEGER       ControlCount
   zLONG     ControlCount = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 


   //:TraceLineS("*** XSLT_BuildCtrlsAsTable *** ", "")
   TraceLineS( "*** XSLT_BuildCtrlsAsTable *** ", "" );
   //:szOutputLine = szIndentationChars + "<!-- In XSLT_BuildCtrlsAsTable -->"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<!-- In XSLT_BuildCtrlsAsTable -->", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = szIndentationChars + ""
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );


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
   SetNameForView( vReportDefRoot, "vReportDefRootSubText", 0, zLEVEL_TASK );

   //:szCellPaddingName = "DefaultCellPadding"
   ZeidonStringCopy( szCellPaddingName, 1, 0, "DefaultCellPadding", 1, 0, 31 );

   //:// Increment hierarchical level for creating XSLT_WorkGroup entry.
   //:GroupHierarchicalLevel = GroupHierarchicalLevel + 1
   GroupHierarchicalLevel = GroupHierarchicalLevel + 1;

   //:vReportDefRoot.XSLT_WorkGroup.RecursiveNumber = GroupHierarchicalLevel
   SetAttributeFromInteger( vReportDefRoot, "XSLT_WorkGroup", "RecursiveNumber", GroupHierarchicalLevel );

   //:   // Build table, since there are multiple Controls.
   //:   // We will build a single table statement, with a table-column statement for each column (remember, each row has
   //:   // the same number of columns) and a table-row and table-cell statement for each row.

   //:   // Check that all rows have the same number of columns.
   //:   /*
   //:   SET CURSOR FIRST vReportDefRoot.XSLT_WorkRow
   //:   ColumnCount = 0
   //:   FOR EACH vReportDefRoot.XSLT_WorkColumn
   //:      ColumnCount = ColumnCount + 1
   //:   END
   //:   FirstColumnCount = ColumnCount
   //:   FOR EACH vReportDefRoot.XSLT_WorkRow
   //:      ColumnCount = 0
   //:      FOR EACH vReportDefRoot.XSLT_WorkColumn
   //:         ColumnCount = ColumnCount + 1
   //:      END
   //:      IF ColumnCount != FirstColumnCount
   //:         szColumnCount = ColumnCount
   //:         szMsg = "Unequal number of columns containing control, " + vReportDef.Control.Tag + NEW_LINE +
   //:                 "Number of Columns generating error: " + szColumnCount
   //:         MessageSend( vReportDef, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
   //:      END
   //:   END
   //:   */

   //:   // Build open block & table statements.

   //:   // We'll get width from parent.
   //:   CreateViewFromView( vReportDef3, vReportDef )
   CreateViewFromView( &vReportDef3, vReportDef );
   //:   ResetViewFromSubobject( vReportDef3 )
   ResetViewFromSubobject( vReportDef3 );
   //:   TableWidth = vReportDef3.Control.SZDLG_X
   GetIntegerFromAttribute( &TableWidth, vReportDef3, "Control", "SZDLG_X" );
   //:   DropView( vReportDef3 )
   DropView( vReportDef3 );
   //:   // MarginTop will come from Y position of first Control.
   //:   SET CURSOR FIRST vReportDefRoot.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   //:   // KJS 11/28/12
   //:   IF RESULT >= zCURSOR_SET 
   if ( RESULT >= zCURSOR_SET )
   { 
      //:TableMarginTop = vReportDefRoot.XSLT_WorkRow.PSDLG_Y
      GetIntegerFromAttribute( &TableMarginTop, vReportDefRoot, "XSLT_WorkRow", "PSDLG_Y" );
      //:XSLT_BuildTable( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel, TableMarginTop, TableWidth )
      oTZRPSRCO_XSLT_BuildTable( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel, TableMarginTop, TableWidth );
   } 

   //:   END

   //:   // Build the table-column statments from columns in the first row.
   //:   // As noted above, we only need to process columns from the first row as all rows should have the same number of columns.
   //:   // However, we want the maximum width of the corresponding controls, so we will compute that first.
   //:   FOR EACH vReportDefRoot.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:   ColumnCount = 0
      ColumnCount = 0;
      //:   FOR EACH vReportDefRoot.XSLT_WorkColumn
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:   ColumnCount = ColumnCount + 1
         ColumnCount = ColumnCount + 1;
         //:   SET CURSOR FIRST vReportDefRoot.XSLT_ColumnWidth WHERE vReportDefRoot.XSLT_ColumnWidth.Index = ColumnCount
         RESULT = SetCursorFirstEntityByInteger( vReportDefRoot, "XSLT_ColumnWidth", "Index", ColumnCount, "" );
         //:   IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:   CREATE ENTITY vReportDefRoot.XSLT_ColumnWidth
            RESULT = CreateEntity( vReportDefRoot, "XSLT_ColumnWidth", zPOS_AFTER );
            //:   vReportDefRoot.XSLT_ColumnWidth.Index = ColumnCount
            SetAttributeFromInteger( vReportDefRoot, "XSLT_ColumnWidth", "Index", ColumnCount );
         } 

         //:   END
         //:   IF ColumnCount = 1
         if ( ColumnCount == 1 )
         { 
            //:   MaximumWidth = vReportDefRoot.XSLT_WorkColumn.SZDLG_X + vReportDefRoot.XSLT_WorkColumn.PSDLG_X
            GetIntegerFromAttribute( &lTempInteger_0, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
            GetIntegerFromAttribute( &lTempInteger_1, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
            MaximumWidth = lTempInteger_0 + lTempInteger_1;
            //:ELSE
         } 
         else
         { 
            //:   MaximumWidth = vReportDefRoot.XSLT_WorkColumn.SZDLG_X
            GetIntegerFromAttribute( &MaximumWidth, vReportDefRoot, "XSLT_WorkColumn", "SZDLG_X" );
         } 

         //:   END
         //:   IF MaximumWidth > vReportDefRoot.XSLT_ColumnWidth.MaximumWidth
         if ( CompareAttributeToInteger( vReportDefRoot, "XSLT_ColumnWidth", "MaximumWidth", MaximumWidth ) < 0 )
         { 
            //:   vReportDefRoot.XSLT_ColumnWidth.MaximumWidth = MaximumWidth
            SetAttributeFromInteger( vReportDefRoot, "XSLT_ColumnWidth", "MaximumWidth", MaximumWidth );
         } 

         RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
         //:   END
      } 

      RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
      //:   END
   } 

   //:   END
   //:   SET CURSOR FIRST vReportDefRoot.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   //:   ColumnCount = 0
   ColumnCount = 0;
   //:   FOR EACH vReportDefRoot.XSLT_WorkColumn
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:   ColumnCount = ColumnCount + 1
      ColumnCount = ColumnCount + 1;
      //:   SET CURSOR FIRST vReportDefRoot.XSLT_ColumnWidth WHERE vReportDefRoot.XSLT_ColumnWidth.Index = ColumnCount
      RESULT = SetCursorFirstEntityByInteger( vReportDefRoot, "XSLT_ColumnWidth", "Index", ColumnCount, "" );

      //:   CreateViewFromView( vReportDef3, vReportDefRoot )
      CreateViewFromView( &vReportDef3, vReportDefRoot );
      //:   NAME VIEW vReportDef3 "vReportDef3SubText"
      SetNameForView( vReportDef3, "vReportDef3SubText", 0, zLEVEL_TASK );
      //:   IF ColumnCount = 1
      if ( ColumnCount == 1 )
      { 
         //:   // The width of the first column is the starting position of the next column, if there is one.
         //:   // Otherwise it's the width of the single control plus its X position.
         //:   // In either case, it needs to be big enough to handle any padding characters.
         //:   SET CURSOR NEXT vReportDef3.XSLT_WorkColumn
         RESULT = SetCursorNextEntity( vReportDef3, "XSLT_WorkColumn", "" );
         //:   IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:   // There is a next column, so use its X position as the first column width.
            //:   CurrentWidth = vReportDef3.XSLT_WorkColumn.PSDLG_X
            GetIntegerFromAttribute( &CurrentWidth, vReportDef3, "XSLT_WorkColumn", "PSDLG_X" );
            //:ELSE
         } 
         else
         { 
            //:   // There is no next column, so use computed width from above.
            //:   CurrentWidth = vReportDefRoot.XSLT_ColumnWidth.MaximumWidth
            GetIntegerFromAttribute( &CurrentWidth, vReportDefRoot, "XSLT_ColumnWidth", "MaximumWidth" );
         } 

         //:   END
         //:ELSE
      } 
      else
      { 
         //:   // The width is the difference between the starting position of this control and the starting position of the next control.
         //:   // If there is no next control, the width will be the actual width of the control computed above.
         //:   SET CURSOR NEXT vReportDef3.XSLT_WorkColumn
         RESULT = SetCursorNextEntity( vReportDef3, "XSLT_WorkColumn", "" );
         //:   IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:   CurrentWidth  = vReportDef3.XSLT_WorkColumn.PSDLG_X - vReportDefRoot.XSLT_WorkColumn.PSDLG_X
            GetIntegerFromAttribute( &lTempInteger_2, vReportDef3, "XSLT_WorkColumn", "PSDLG_X" );
            GetIntegerFromAttribute( &lTempInteger_3, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
            CurrentWidth = lTempInteger_2 - lTempInteger_3;
            //:ELSE
         } 
         else
         { 
            //:   CurrentWidth  = vReportDefRoot.XSLT_ColumnWidth.MaximumWidth
            GetIntegerFromAttribute( &CurrentWidth, vReportDefRoot, "XSLT_ColumnWidth", "MaximumWidth" );
         } 

         //:   END
      } 

      //:   END
      //:   ConvertPxToInchesCH( vReportDef, szCurrentWidth, CurrentWidth )
      oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szCurrentWidth, CurrentWidth );
      //:   DropView( vReportDef3 )
      DropView( vReportDef3 );

      //:   szOutputLine = szIndentationChars + "<fo:table-column column-width=" + QUOTES + szCurrentWidth + "in" + QUOTES + "/>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table-column column-width=", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, szCurrentWidth, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "/>", 1, 0, 5001 );
      //:   GenerateLine( vReportDefRoot, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDefRoot, lFileHandle, szOutputLine );
      RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
   } 


   //:   END

   //:   // We also have to build the table-body statement.
   //:   szOutputLine = szIndentationChars + "<fo:table-body>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "<fo:table-body>", 1, 0, 5001 );
   //:   GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:   // Build table-row and table-cell statements.
   //:   RowCount = 0
   RowCount = 0;
   //:   FOR EACH vReportDefRoot.XSLT_WorkRow
   RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:   // Build open table-row statement.
      //:   szOutputLine = szIndentationChars + "  <fo:table-row>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "  <fo:table-row>", 1, 0, 5001 );
      //:   GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

      //:   // Build the table-cell statements.
      //:   TextColumnCount = 0
      TextColumnCount = 0;
      //:   FOR EACH vReportDefRoot.XSLT_WorkColumn
      RESULT = SetCursorFirstEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:   // Cell padding is only used for first column in row.
         //:   TextColumnCount = TextColumnCount + 1
         TextColumnCount = TextColumnCount + 1;
         //:   IF TextColumnCount = 1
         if ( TextColumnCount == 1 )
         { 
            //:   // Determine padding for 1st column.
            //:   CellPadding = vReportDefRoot.XSLT_WorkColumn.PSDLG_X
            GetIntegerFromAttribute( &CellPadding, vReportDefRoot, "XSLT_WorkColumn", "PSDLG_X" );
            //:   ConvertPxToInchesCH( vReportDef, szCellPadding, CellPadding )
            oTZRPSRCO_ConvertPxToInchesCH( vReportDef, szCellPadding, CellPadding );
            //:ELSE
         } 
         else
         { 
            //:   szCellPadding = "0"
            ZeidonStringCopy( szCellPadding, 1, 0, "0", 1, 0, 51 );
         } 

         //:   END

         //:   szOutputLine = szIndentationChars + "    <fo:table-cell xsl:use-attribute-sets=" + QUOTES + szCellPaddingName + QUOTES +
         //:                  " padding-left=" + QUOTES + szCellPadding + "in" + QUOTES + ">"
         ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, "    <fo:table-cell xsl:use-attribute-sets=", 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, szCellPaddingName, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, " padding-left=", 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, szCellPadding, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, "in", 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
         //:   GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

         //:   // Build block and data statements.
         //:   XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel )
         oTZRPSRCO_XSLT_BuildBlockData( vReportDefRoot, vReportDef, vLOD, lFileHandle, szOutputLine, szIndentationChars, GroupHierarchicalLevel );

         //:   // Close table-cell
         //:   szOutputLine = szIndentationChars + "    </fo:table-cell>"
         ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
         ZeidonStringConcat( szOutputLine, 1, 0, "    </fo:table-cell>", 1, 0, 5001 );
         //:   GenerateLine( vReportDef, lFileHandle, szOutputLine )
         oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
         RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkColumn", "" );
      } 


      //:   END

      //:   // Build close table-row statement.
      //:   szOutputLine = szIndentationChars + "  </fo:table-row>"
      ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
      ZeidonStringConcat( szOutputLine, 1, 0, "  </fo:table-row>", 1, 0, 5001 );
      //:   GenerateLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
      RESULT = SetCursorNextEntity( vReportDefRoot, "XSLT_WorkRow", "" );
   } 


   //:   END

   //:   // Build close table-body, table and block statements.
   //:   szOutputLine = szIndentationChars + "</fo:table-body>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "</fo:table-body>", 1, 0, 5001 );
   //:   GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:   szOutputLine = szIndentationChars + "</fo:table>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "</fo:table>", 1, 0, 5001 );
   //:   GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:   szOutputLine = szIndentationChars + "</fo:block>"
   ZeidonStringCopy( szOutputLine, 1, 0, szIndentationChars, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "</fo:block>", 1, 0, 5001 );
   //:   GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Remove the Work Group for this hierarchical level.
   //:DropNameForView( vReportDefRoot, "vReportDefRootSubText", vReportDef, zLEVEL_TASK )
   DropNameForView( vReportDefRoot, "vReportDefRootSubText", vReportDef, zLEVEL_TASK );
   //:DELETE ENTITY vReportDefRoot.XSLT_WorkGroup
   RESULT = DeleteEntity( vReportDefRoot, "XSLT_WorkGroup", zPOS_NEXT );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenerateXSLT( VIEW SourceReport BASED ON LOD TZRPSRCO )

//:   VIEW vReportDef    BASED ON LOD TZRPSRCO
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_GenerateXSLT( zVIEW     SourceReport )
{
   zVIEW     vReportDef = 0; 
   //:VIEW vReportRecurs BASED ON LOD TZRPSRCO
   zVIEW     vReportRecurs = 0; 
   //:VIEW vLOD          BASED ON LOD TZZOLODO
   zVIEW     vLOD = 0; 
   //:VIEW vTaskLPLR     BASED ON LOD TZCMLPLO
   zVIEW     vTaskLPLR = 0; 
   //:STRING ( 32 )   szTopEntityName
   zCHAR     szTopEntityName[ 33 ] = { 0 }; 
   //:STRING ( 50 )   szGroupSetName
   zCHAR     szGroupSetName[ 51 ] = { 0 }; 
   //:STRING ( 200 )  szMsg
   zCHAR     szMsg[ 201 ] = { 0 }; 
   //:STRING ( 200 )  szFileName
   zCHAR     szFileName[ 201 ] = { 0 }; 
   //:STRING ( 5000 ) szOutputLine
   zCHAR     szOutputLine[ 5001 ] = { 0 }; 
   //:STRING ( 1000 ) szStructure
   zCHAR     szStructure[ 1001 ] = { 0 }; 
   //:STRING ( 50 )   szIndentationChars
   zCHAR     szIndentationChars[ 51 ] = { 0 }; 
   //:STRING ( 200 )  szPathName
   zCHAR     szPathName[ 201 ] = { 0 }; 
   //:INTEGER         lFileHandle
   zLONG     lFileHandle = 0; 
   //:SHORT           nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 


   //:// Build an XSLT object from the Dialog Window passed.
   //:vReportDef = SourceReport
   vReportDef = SourceReport;

   //:// Open XSLT output file.
   //:SysReadZeidonIni( -1, "[Workstation]", "XSLTDirectory", szPathName )
   SysReadZeidonIni( -1, "[Workstation]", "XSLTDirectory", szPathName, sizeof( szPathName ) );

   //://szFileName = "c:\temp\XSLT_OutputTest.xsl"
   //:szFileName = szPathName + vReportDef.Report.Tag + ".xsl"
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vReportDef, "Report", "Tag", "", 0 );
   ZeidonStringCopy( szFileName, 1, 0, szPathName, 1, 0, 201 );
   ZeidonStringConcat( szFileName, 1, 0, szTempString_0, 1, 0, 201 );
   ZeidonStringConcat( szFileName, 1, 0, ".xsl", 1, 0, 201 );
   //:lFileHandle = SysOpenFile( SourceReport, szFileName, COREFILE_WRITE )
   lFileHandle = SysOpenFile( SourceReport, szFileName, COREFILE_WRITE );
   //:IF lFileHandle < 0
   if ( lFileHandle < 0 )
   { 
      //:szMsg = "Cannot open XSLT Output File, " + szFileName
      ZeidonStringCopy( szMsg, 1, 0, "Cannot open XSLT Output File, ", 1, 0, 201 );
      ZeidonStringConcat( szMsg, 1, 0, szFileName, 1, 0, 201 );
      //:MessageSend( SourceReport, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( SourceReport, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Activate LOD that goes with PRP file. The registered view that is also the entity DrivingViewObjRef.
   //:IF vReportDef.DrivingViewObjRef EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "DrivingViewObjRef" );
   if ( lTempInteger_0 == 0 )
   { 
      //://szFileName = "c:\temp\mSAProfE.lod"
      //:GET VIEW vTaskLPLR NAMED "TaskLPLR"
      RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", SourceReport, zLEVEL_TASK );
      //:szFileName = vTaskLPLR.LPLR.MetaSrcDir + "\"
      GetStringFromAttribute( szFileName, sizeof( szFileName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, 201 );
      //:szFileName = szFileName + vReportDef.DrivingLOD.Name + ".lod"
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vReportDef, "DrivingLOD", "Name", "", 0 );
      ZeidonStringConcat( szFileName, 1, 0, szTempString_1, 1, 0, 201 );
      ZeidonStringConcat( szFileName, 1, 0, ".lod", 1, 0, 201 );
      //:ActivateOI_FromFile( vLOD, "TZZOLODO", SourceReport, szFileName, zSINGLE )
      ActivateOI_FromFile( &vLOD, "TZZOLODO", SourceReport, szFileName, zSINGLE );
      //:NAME VIEW vLOD "LOD_XSLT"
      SetNameForView( vLOD, "LOD_XSLT", 0, zLEVEL_TASK );
      //:ELSE
   } 
   else
   { 
      //:szMsg = "One of the registered views needs to be set as the 'Driving View'. "
      ZeidonStringCopy( szMsg, 1, 0, "One of the registered views needs to be set as the 'Driving View'. ", 1, 0, 201 );
      //:MessageSend( SourceReport, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( SourceReport, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:SysCloseFile( SourceReport, lFileHandle, 0 )
      SysCloseFile( SourceReport, lFileHandle, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// Go to build XSLT Header lines and PageHeader and PageFooter.
   //:XSLT_MainHeader( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_XSLT_MainHeader( vReportDef, lFileHandle, szOutputLine );

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

   //:IF vReportDef.PartialReportEntity EXISTS 
   lTempInteger_1 = CheckExistenceOfEntity( vReportDef, "PartialReportEntity" );
   if ( lTempInteger_1 == 0 )
   { 
      //:szTopEntityName = vReportDef.PartialReportEntity.Name 
      GetVariableFromAttribute( szTopEntityName, 0, 'S', 33, vReportDef, "PartialReportEntity", "Name", "", 0 );
      //:ELSE
   } 
   else
   { 
      //:MessageSend( SourceReport, "You can't generate to xslt without any GroupSets.", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( SourceReport, "You can't generate to xslt without any GroupSets.", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN 2                   
      return( 2 );
   } 

   //:END
   //://szTopEntityName = vReportDef.GroupSet.Tag
   //:// KJS 12/11/12 - Going to set the TopENtityName to CurrentEntityName (don't see this being used anywhere) so I can use it
   //:// elsewhere in the code.
   //:vReportDef.Report.CurrentEntityName = szTopEntityName
   SetAttributeFromString( vReportDef, "Report", "CurrentEntityName", szTopEntityName );

   //:IF vReportDef.XSLT_WorkGroup EXISTS
   lTempInteger_2 = CheckExistenceOfEntity( vReportDef, "XSLT_WorkGroup" );
   if ( lTempInteger_2 == 0 )
   { 
      //:DELETE ENTITY vReportDef.XSLT_WorkGroup
      RESULT = DeleteEntity( vReportDef, "XSLT_WorkGroup", zPOS_NEXT );
   } 

   //:END

   //:// Open template statement
   //:szGroupSetName = "GR_" + szTopEntityName
   ZeidonStringCopy( szGroupSetName, 1, 0, "GR_", 1, 0, 51 );
   ZeidonStringConcat( szGroupSetName, 1, 0, szTopEntityName, 1, 0, 51 );
   //:szOutputLine = "<xsl:template match=" + QUOTES + szGroupSetName + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "<xsl:template match=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, szGroupSetName, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "  <fo:block font-size=" + QUOTES + "9pt" + QUOTES + ">"
   ZeidonStringCopy( szOutputLine, 1, 0, "  <fo:block font-size=", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, "9pt", 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, QUOTES, 1, 0, 5001 );
   ZeidonStringConcat( szOutputLine, 1, 0, ">", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );
   //:GenerateLine( vReportDef, lFileHandle, " " )   
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, " " );

   //:CREATE ENTITY vReportDef.XSLT_WorkGroup
   RESULT = CreateEntity( vReportDef, "XSLT_WorkGroup", zPOS_AFTER );
   //:vReportDef.XSLT_WorkGroup.RecursiveNumber = 1
   SetAttributeFromInteger( vReportDef, "XSLT_WorkGroup", "RecursiveNumber", 1 );
   //:NAME VIEW vReportDef "vReportDefRoot"
   SetNameForView( vReportDef, "vReportDefRoot", 0, zLEVEL_TASK );
   //:CreateViewFromView( vReportRecurs, vReportDef )
   CreateViewFromView( &vReportRecurs, vReportDef );

   //:szStructure = "/zOI/" + szGroupSetName
   ZeidonStringCopy( szStructure, 1, 0, "/zOI/", 1, 0, 1001 );
   ZeidonStringConcat( szStructure, 1, 0, szGroupSetName, 1, 0, 1001 );

   //:szIndentationChars = "    "    // Base indentation will be 10 characters.
   ZeidonStringCopy( szIndentationChars, 1, 0, "    ", 1, 0, 51 );
   //:XSLT_EntityRecur( vReportDef, vReportRecurs, vLOD, lFileHandle, szOutputLine, szIndentationChars, szStructure )
   oTZRPSRCO_XSLT_EntityRecur( vReportDef, vReportRecurs, vLOD, lFileHandle, szOutputLine, szIndentationChars, szStructure );
   //:DELETE ENTITY vReportDef.XSLT_WorkGroup
   RESULT = DeleteEntity( vReportDef, "XSLT_WorkGroup", zPOS_NEXT );

   //:szOutputLine = "  </fo:block>"
   ZeidonStringCopy( szOutputLine, 1, 0, "  </fo:block>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:// Close template statement
   //:szOutputLine = "</xsl:template>"
   ZeidonStringCopy( szOutputLine, 1, 0, "</xsl:template>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );


   //:// Close StyleSheet.
   //:szOutputLine = "</xsl:stylesheet>"
   ZeidonStringCopy( szOutputLine, 1, 0, "</xsl:stylesheet>", 1, 0, 5001 );
   //:GenerateLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateLine( vReportDef, lFileHandle, szOutputLine );

   //:SysCloseFile( SourceReport, lFileHandle, 0 )
   SysCloseFile( SourceReport, lFileHandle, 0 );

   //:IF vReportDef.XSLT_WorkGroup EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( vReportDef, "XSLT_WorkGroup" );
   if ( lTempInteger_3 == 0 )
   { 
      //:DELETE ENTITY vReportDef.XSLT_WorkGroup
      RESULT = DeleteEntity( vReportDef, "XSLT_WorkGroup", zPOS_NEXT );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
oTZRPSRCO_GenerateLine( zVIEW     ReportObject,
                        zLONG     lFileHandle,
                        zPCHAR    szOutputLine )
{

   //:GenerateLine( VIEW ReportObject BASED ON LOD TZRPSRCO,
   //:           INTEGER lFileHandle,
   //:           STRING ( 5000 ) szOutputLine )

   //:// This is just a SysWriteLine with an options TraceLineS statement.
   //://TraceLineS( "*** Line: ", szOutputLine )
   //:SysWriteLine( ReportObject, lFileHandle, szOutputLine )
   SysWriteLine( ReportObject, lFileHandle, szOutputLine );
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:GenerateXSLT_PDF( VIEW vReportDef,
//:                  VIEW vSourceOI,
//:                  STRING ( 32 ) szTopEntityName,
//:                  INTEGER lFlag )
zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_GenerateXSLT_PDF( zVIEW     vReportDef,
                            zVIEW     vSourceOI,
                            zPCHAR    szTopEntityName,
                            zLONG     lFlag )
{

   return( 0 );
//    // The first step is to build the XML object for the passed in application object and Report Definition.
//    //GenerateXSLT_XML( vReportDef, vSourceOI, szTopEntityName )
// END
} 


//:LOCAL OPERATION
//:ConvertPxToInchesCH( VIEW vReportDefRoot BASED ON LOD TZRPSRCO,
//:                     STRING ( 20 ) ReturnedString,
//:                     INTEGER lValueInPixles )

//:   DECIMAL dValueInInches
static zSHORT
oTZRPSRCO_ConvertPxToInchesCH( zVIEW     vReportDefRoot,
                               zPCHAR    ReturnedString,
                               zLONG     lValueInPixles )
{
   ZDecimal  dValueInInches = 0.0; 
   //:DECIMAL dDecimal0
   ZDecimal  dDecimal0 = 0.0; 
   //:INTEGER lValueInInches
   zLONG     lValueInInches = 0; 


   //:// Convert pixles integer value to decimal 2 as a returned string.
   //:lValueInInches = ( lValueInPixles * 100 ) / 256
   lValueInInches = ( lValueInPixles * 100 ) / 256;
   //://lValueInInches = ( lValueInPixles * 100 ) / 256
   //://lValueInInches = ( lValueInPixles * 100 ) / 280
   //:dDecimal0 = lValueInInches
   dDecimal0 = lValueInInches;
   //:dValueInInches = dDecimal0 / 100
   dValueInInches = dDecimal0 / 100;
   //:ReturnedString = dValueInInches
   ZeidonStringConvertFromNumber( ReturnedString, 1, 0, 20, 0, dValueInInches, "D" );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
