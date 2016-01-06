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

package com.quinsoft.zeidon.zeidontools;

import com.quinsoft.zeidon.ActivateFlags;
import com.quinsoft.zeidon.CursorPosition;
import com.quinsoft.zeidon.TaskQualification;
import com.quinsoft.zeidon.vml.VmlObjectOperations;
import com.quinsoft.zeidon.View;
import com.quinsoft.zeidon.ZeidonException;
import com.quinsoft.zeidon.vml.zVIEW;
import org.apache.commons.lang3.mutable.MutableDouble;
import org.apache.commons.lang3.mutable.MutableInt;

import com.quinsoft.zeidon.zeidontools.KZRPSRCO_Operation;

import com.quinsoft.zeidon.zeidonoperations.KZOEP1AA;

/**
   @author QuinSoft
**/

public class TZRPSXML_Object extends VmlObjectOperations
{
   private final KZOEP1AA m_KZOEP1AA;
   public TZRPSXML_Object( View view )
   {
      super( view );
      m_KZOEP1AA = new KZOEP1AA( view );
   }


//:TRANSFORMATION OPERATION
//:GenerateXSLT_XML( VIEW vSourceOI,
//:                  STRING ( 32 )  szReportName,
//:                  STRING ( 500 ) szXMLFileName,
//:                  STRING ( 32 )  szTopEntityName )

//:   VIEW vReportDef     BASED ON LOD TZRPSRCO
public int 
oTZRPSRCO_GenerateXSLT_XML( View     vSourceOI,
                            String   szReportName,
                            String   szXMLFileName,
                            String   szTopEntityName )
{
   zVIEW    vReportDef = new zVIEW( );
   //:VIEW vReportDefRoot BASED ON LOD TZRPSRCO
   zVIEW    vReportDefRoot = new zVIEW( );
   //:VIEW vSourceOIHier
   zVIEW    vSourceOIHier = new zVIEW( );
   //:VIEW vSourceOIDebug
   zVIEW    vSourceOIDebug = new zVIEW( );
   //:VIEW vLOD           BASED ON LOD TZZOLODO
   zVIEW    vLOD = new zVIEW( );
   //:VIEW vTaskLPLR  BASED ON LOD TZCMLPLO
   zVIEW    vTaskLPLR = new zVIEW( );
   //:STRING ( 200 )  szMsg
   String   szMsg = null;
   //:STRING ( 200 )  szFileName
   String   szFileName = null;
   //:STRING ( 32 )   szReturnedEntityName
   String   szReturnedEntityName = null;
   //:STRING ( 32 )   szLastReturnedEntityName
   String   szLastReturnedEntityName = null;
   //:STRING ( 32 )   szParentEntityName
   String   szParentEntityName = null;
   //:STRING ( 32 )   szDrivingViewName
   String   szDrivingViewName = null;
   //:STRING ( 50 )   szIndentationValue
   String   szIndentationValue = null;
   //:STRING ( 50 )   szIndentationSubValue
   String   szIndentationSubValue = null;
   //:STRING ( 1 )    szInNodeFlag
   String   szInNodeFlag = null;
   //:STRING ( 5000 ) szOutputLine
   String   szOutputLine = null;
   //:INTEGER         lFileHandle
   int      lFileHandle = 0;
   //:INTEGER         Indentation
   int      Indentation = 0;
   //:SHORT           ReturnedHierLevel
   int      ReturnedHierLevel = 0;
   //:SHORT           LastHierLevel
   int      LastHierLevel = 0;
   //:SHORT           nRC
   int      nRC = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      RESULT = 0;
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


   //:// Build an XSLT XML FO object for the passed in application object and Report Definition.

   //:// First we need to activate the report def...
   //://szFileName = "c:\temp\mSAProfE.lod"
   //://GET VIEW vTaskLPLR NAMED "TaskLPLR"
   //://szFileName = vTaskLPLR.LPLR.ExecDir + "\"
   //:szFileName = "c:\lplr\zencas\bin\"
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringCopy( sb_szFileName, 1, 0, "c:\\lplr\\zencas\\bin\\", 1, 0, 201 );
   szFileName = sb_szFileName.toString( );}
   //:szFileName = szFileName + szReportName + ".xrp"
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringConcat( sb_szFileName, 1, 0, szReportName, 1, 0, 201 );
   szFileName = sb_szFileName.toString( );}
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringConcat( sb_szFileName, 1, 0, ".xrp", 1, 0, 201 );
   szFileName = sb_szFileName.toString( );}
   //://ActivateOI_FromFile( vReportDef, "TZRPSRCO", vSourceOI, szFileName, zSINGLE )
   //:SfActivateSysOI_FromFile( vReportDef, "TZRPSRCO", vSourceOI, szFileName, zSINGLE )
   SfActivateSysOI_FromFile( vReportDef, "TZRPSRCO", vSourceOI, szFileName, zSINGLE );

   //:// Get the Name of the Driving object, as we will need that in processing mapping.
   //:IF vReportDef.DrivingViewObjRef EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vReportDef, "DrivingViewObjRef" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szDrivingViewName = vReportDef.ViewObjRef.Name
      {MutableInt mi_lTempInteger_1 = new MutableInt( lTempInteger_1 );
      StringBuilder sb_szDrivingViewName;
      if ( szDrivingViewName == null )
         sb_szDrivingViewName = new StringBuilder( 32 );
      else
         sb_szDrivingViewName = new StringBuilder( szDrivingViewName );
             GetVariableFromAttribute( sb_szDrivingViewName, mi_lTempInteger_1, 'S', 33, vReportDef, "ViewObjRef", "Name", "", 0 );
      lTempInteger_1 = mi_lTempInteger_1.intValue( );
      szDrivingViewName = sb_szDrivingViewName.toString( );}
      //:ELSE
   } 
   else
   { 
      //:szMsg = "The report must have a 'Driving View' set in the report details."
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "The report must have a 'Driving View' set in the report details.", 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
      //:MessageSend( vReportDef, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vReportDef, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
   } 

   //:END

   //:// Open XML output file.
   //://szFileName = "c:\temp\XSLT_OutputTest.xml"
   //:SysReadZeidonIni( -1, "[App.Zencas]", "XSLTDirectory", szFileName )
   {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
       m_KZOEP1AA.SysReadZeidonIni( -1, "[App.Zencas]", "XSLTDirectory", sb_szFileName );
   szFileName = sb_szFileName.toString( );}
   //:szFileName = szFileName + szXMLFileName + ".xml"
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringConcat( sb_szFileName, 1, 0, szXMLFileName, 1, 0, 201 );
   szFileName = sb_szFileName.toString( );}
    {StringBuilder sb_szFileName;
   if ( szFileName == null )
      sb_szFileName = new StringBuilder( 32 );
   else
      sb_szFileName = new StringBuilder( szFileName );
      ZeidonStringConcat( sb_szFileName, 1, 0, ".xml", 1, 0, 201 );
   szFileName = sb_szFileName.toString( );}
   //:lFileHandle = SysOpenFile( vReportDef, szFileName, COREFILE_WRITE )
   try
   {
       lFileHandle = m_KZOEP1AA.SysOpenFile( vReportDef, szFileName, COREFILE_WRITE );
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
      //:MessageSend( vReportDef, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vReportDef, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
   } 

   //:END

   //:// Make sure that the subobject entity name passed identifies a GroupSet in the Report Def.
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szTopEntityName
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szTopEntityName, "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:szMsg = "The Subobject Entity Name passed in does not match a Group Set Name in the Report Definition."
       {StringBuilder sb_szMsg;
      if ( szMsg == null )
         sb_szMsg = new StringBuilder( 32 );
      else
         sb_szMsg = new StringBuilder( szMsg );
            ZeidonStringCopy( sb_szMsg, 1, 0, "The Subobject Entity Name passed in does not match a Group Set Name in the Report Definition.", 1, 0, 201 );
      szMsg = sb_szMsg.toString( );}
      //:MessageSend( vReportDef, "", "Generate XSLT",
      //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
      MessageSend( vReportDef, "", "Generate XSLT", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      //:RETURN -1
      if(8==8)return( -1 );
   } 

   //:END

   //:// Navigate the vSourceOI object/subobject hierarchically and create an XML node for each entity that has a corresponding
   //:// GroupSet entity in the Report Def.

   //:// Generate basic header.
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
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
   //:szOutputLine = "<zOI>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "<zOI>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

   //:szOutputLine = "   <GR_" + szTopEntityName + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   <GR_", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szTopEntityName, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

   //:// Process Top Entity Header, if it exists.

   //:// Set to the first GroupSet which if it has a header would be the report header.
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szTopEntityName
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szTopEntityName, "" );
   //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "gh"
   RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "gh", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szOutputLine = szIndentationValue + "   <Header>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "   <Header>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

      //:szIndentationSubValue = szIndentationValue + "      "
       {StringBuilder sb_szIndentationSubValue;
      if ( szIndentationSubValue == null )
         sb_szIndentationSubValue = new StringBuilder( 32 );
      else
         sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
            ZeidonStringCopy( sb_szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
      szIndentationSubValue = sb_szIndentationSubValue.toString( );}
       {StringBuilder sb_szIndentationSubValue;
      if ( szIndentationSubValue == null )
         sb_szIndentationSubValue = new StringBuilder( 32 );
      else
         sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
            ZeidonStringConcat( sb_szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
      szIndentationSubValue = sb_szIndentationSubValue.toString( );}
      //:GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue )
      oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue );

      //:szOutputLine = szIndentationValue + "   </Header>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "   </Header>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END

   //:// Loop through each of the topmost entities (which could be a root) based on the subobject entity name passed in,
   //:// processing each subobject hierarchically.

   //:nRC = SetCursorFirstEntity( vSourceOI, szTopEntityName, "" )
   nRC = SetCursorFirstEntity( vSourceOI, szTopEntityName, "" );

   //:LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 

      //:// Position on Top Entity Report Group.
      //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szTopEntityName
      RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szTopEntityName, "" );

      //:// Open Top Entity.
      //:szOutputLine = "   <" + szTopEntityName + ">"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, "   <", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szTopEntityName, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

      //:szIndentationValue = "      "
       {StringBuilder sb_szIndentationValue;
      if ( szIndentationValue == null )
         sb_szIndentationValue = new StringBuilder( 32 );
      else
         sb_szIndentationValue = new StringBuilder( szIndentationValue );
            ZeidonStringCopy( sb_szIndentationValue, 1, 0, "      ", 1, 0, 51 );
      szIndentationValue = sb_szIndentationValue.toString( );}

      //:// Note that we are already positioned on the Group Set for the topmost entity.

      //:// Go to generate an attribute entry for each attribute with mapping in the GroupSet.
      //:GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationValue )
      oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationValue );

      //:// Close Top Entity.
      //:szOutputLine = "   </" + szTopEntityName + ">"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, "   </", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, szTopEntityName, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

      //:// Process each subentity Hierarchically
      //:CreateViewFromView( vSourceOIHier, vSourceOI )
      CreateViewFromView( vSourceOIHier, vSourceOI );
      //:NAME VIEW vSourceOIHier "vSourceOIHier"
      SetNameForView( vSourceOIHier, "vSourceOIHier", null, zLEVEL_TASK );
      //:DefineHierarchicalCursor( vSourceOIHier, szTopEntityName )
      DefineHierarchicalCursor( vSourceOIHier, szTopEntityName );

      //:nRC = SetCursorNextEntityHierarchical( ReturnedHierLevel, szReturnedEntityName, vSourceOIHier )
      {StringBuilder sb_szReturnedEntityName;
      if ( szReturnedEntityName == null )
         sb_szReturnedEntityName = new StringBuilder( 32 );
      else
         sb_szReturnedEntityName = new StringBuilder( szReturnedEntityName );
      MutableInt mi_ReturnedHierLevel = new MutableInt( ReturnedHierLevel );
             nRC = SetCursorNextEntityHierarchical( mi_ReturnedHierLevel, sb_szReturnedEntityName, vSourceOIHier );
      szReturnedEntityName = sb_szReturnedEntityName.toString( );
      ReturnedHierLevel = mi_ReturnedHierLevel.intValue( );}

      //:LOOP WHILE nRC >= zCURSOR_SET AND szReturnedEntityName != szTopEntityName
      while ( nRC >= zCURSOR_SET && ZeidonStringCompare( szReturnedEntityName, 1, 0, szTopEntityName, 1, 0, 33 ) != 0 )
      { 

         //:// Process any Footer from the last group, if there was one AND we're NOT at a lower hierarchical level.
         //:IF szLastReturnedEntityName != szReturnedEntityName AND
         //:   ReturnedHierLevel >= LastHierLevel AND
         //:   szInNodeFlag = "Y"
         if ( ZeidonStringCompare( szLastReturnedEntityName, 1, 0, szReturnedEntityName, 1, 0, 33 ) != 0 && ReturnedHierLevel >= LastHierLevel && ZeidonStringCompare( szInNodeFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 

            //:// Close any Entity Tag Pairs whose Hierarchical level is the same or less than this one.
            //:IF vReportDef.XML_Parent EXISTS
            lTempInteger_2 = CheckExistenceOfEntity( vReportDef, "XML_Parent" );
            if ( lTempInteger_2 == 0 )
            { 
               //:LOOP WHILE vReportDef.XML_Parent EXISTS AND ReturnedHierLevel <= vReportDef.XML_Parent.HierarchicalLevel
               lTempInteger_3 = CheckExistenceOfEntity( vReportDef, "XML_Parent" );
               while ( lTempInteger_3 == 0 && CompareAttributeToInteger( vReportDef, "XML_Parent", "HierarchicalLevel", ReturnedHierLevel ) >= 0 )
               { 
                  //:// Format Entity Terminator Tag.
                  //:szParentEntityName = vReportDef.XML_Parent.EntityName
                  {MutableInt mi_lTempInteger_4 = new MutableInt( lTempInteger_4 );
                  StringBuilder sb_szParentEntityName;
                  if ( szParentEntityName == null )
                     sb_szParentEntityName = new StringBuilder( 32 );
                  else
                     sb_szParentEntityName = new StringBuilder( szParentEntityName );
                                     GetVariableFromAttribute( sb_szParentEntityName, mi_lTempInteger_4, 'S', 33, vReportDef, "XML_Parent", "EntityName", "", 0 );
                  lTempInteger_4 = mi_lTempInteger_4.intValue( );
                  szParentEntityName = sb_szParentEntityName.toString( );}

                  //:Indentation = vReportDef.XML_Parent.HierarchicalLevel - 1
                  {MutableInt mi_lTempInteger_5 = new MutableInt( lTempInteger_5 );
                                     GetIntegerFromAttribute( mi_lTempInteger_5, vReportDef, "XML_Parent", "HierarchicalLevel" );
                  lTempInteger_5 = mi_lTempInteger_5.intValue( );}
                  Indentation = lTempInteger_5 - 1;
                  //:szIndentationValue = ""
                   {StringBuilder sb_szIndentationValue;
                  if ( szIndentationValue == null )
                     sb_szIndentationValue = new StringBuilder( 32 );
                  else
                     sb_szIndentationValue = new StringBuilder( szIndentationValue );
                                    ZeidonStringCopy( sb_szIndentationValue, 1, 0, "", 1, 0, 51 );
                  szIndentationValue = sb_szIndentationValue.toString( );}
                  //:LOOP WHILE Indentation > 0
                  while ( Indentation > 0 )
                  { 
                     //:szIndentationValue = szIndentationValue + "   "
                      {StringBuilder sb_szIndentationValue;
                     if ( szIndentationValue == null )
                        sb_szIndentationValue = new StringBuilder( 32 );
                     else
                        sb_szIndentationValue = new StringBuilder( szIndentationValue );
                                          ZeidonStringConcat( sb_szIndentationValue, 1, 0, "   ", 1, 0, 51 );
                     szIndentationValue = sb_szIndentationValue.toString( );}
                     //:Indentation = Indentation - 1
                     Indentation = Indentation - 1;
                  } 

                  //:END

                  //:szOutputLine = szIndentationValue + "   </" + szParentEntityName + ">"
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, "   </", 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, szParentEntityName, 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                  //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

                  //:// If this isn't a repeat of the last entity, close the last entity Group.
                  //:// Also, create Footer, if there is one.
                  //:IF szLastReturnedEntityName != szReturnedEntityName
                  if ( ZeidonStringCompare( szLastReturnedEntityName, 1, 0, szReturnedEntityName, 1, 0, 33 ) != 0 )
                  { 

                     //:// Create Footer if there is one.
                     //:// Process Header if there is one.
                     //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "gf"
                     RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "gf", "" );
                     //:IF RESULT >= zCURSOR_SET
                     if ( RESULT >= zCURSOR_SET )
                     { 
                        //:szOutputLine = szIndentationValue + "   <Footer>"
                         {StringBuilder sb_szOutputLine;
                        if ( szOutputLine == null )
                           sb_szOutputLine = new StringBuilder( 32 );
                        else
                           sb_szOutputLine = new StringBuilder( szOutputLine );
                                                ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                        szOutputLine = sb_szOutputLine.toString( );}
                         {StringBuilder sb_szOutputLine;
                        if ( szOutputLine == null )
                           sb_szOutputLine = new StringBuilder( 32 );
                        else
                           sb_szOutputLine = new StringBuilder( szOutputLine );
                                                ZeidonStringConcat( sb_szOutputLine, 1, 0, "   <Footer>", 1, 0, 5001 );
                        szOutputLine = sb_szOutputLine.toString( );}
                        //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                        oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

                        //:szIndentationSubValue = szIndentationValue + "      "
                         {StringBuilder sb_szIndentationSubValue;
                        if ( szIndentationSubValue == null )
                           sb_szIndentationSubValue = new StringBuilder( 32 );
                        else
                           sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
                                                ZeidonStringCopy( sb_szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
                        szIndentationSubValue = sb_szIndentationSubValue.toString( );}
                         {StringBuilder sb_szIndentationSubValue;
                        if ( szIndentationSubValue == null )
                           sb_szIndentationSubValue = new StringBuilder( 32 );
                        else
                           sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
                                                ZeidonStringConcat( sb_szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
                        szIndentationSubValue = sb_szIndentationSubValue.toString( );}
                        //:GenerateXML_CtlRecurs( vReportDef, vSourceOIHier, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue )
                        oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOIHier, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue );
                        //:szOutputLine = szIndentationValue + "   </Footer>"
                         {StringBuilder sb_szOutputLine;
                        if ( szOutputLine == null )
                           sb_szOutputLine = new StringBuilder( 32 );
                        else
                           sb_szOutputLine = new StringBuilder( szOutputLine );
                                                ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                        szOutputLine = sb_szOutputLine.toString( );}
                         {StringBuilder sb_szOutputLine;
                        if ( szOutputLine == null )
                           sb_szOutputLine = new StringBuilder( 32 );
                        else
                           sb_szOutputLine = new StringBuilder( szOutputLine );
                                                ZeidonStringConcat( sb_szOutputLine, 1, 0, "   </Footer>", 1, 0, 5001 );
                        szOutputLine = sb_szOutputLine.toString( );}
                        //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                        oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
                     } 

                     //:END

                     //:// Close Group.
                     //:szOutputLine = szIndentationValue + "</GR_" + szParentEntityName + ">"
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringConcat( sb_szOutputLine, 1, 0, "</GR_", 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringConcat( sb_szOutputLine, 1, 0, szParentEntityName, 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                     //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
                  } 

                  //:END

                  //:DELETE ENTITY vReportDef.XML_Parent
                  RESULT = DeleteEntity( vReportDef, "XML_Parent", zPOS_NEXT );
                  lTempInteger_3 = CheckExistenceOfEntity( vReportDef, "XML_Parent" );
               } 

               //:END
            } 

            //:END

            //:szInNodeFlag = ""      // Turn off flag that indicates we are in a node.
             {StringBuilder sb_szInNodeFlag;
            if ( szInNodeFlag == null )
               sb_szInNodeFlag = new StringBuilder( 32 );
            else
               sb_szInNodeFlag = new StringBuilder( szInNodeFlag );
                        ZeidonStringCopy( sb_szInNodeFlag, 1, 0, "", 1, 0, 2 );
            szInNodeFlag = sb_szInNodeFlag.toString( );}
         } 


         //:END

         //:// If the Entity returned has an attribute to display, create XML begin/termination pair and process attributes.
         //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szReturnedEntityName
         RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szReturnedEntityName, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 

            //:// The Entity needs a Group node.

            //:szInNodeFlag = "Y"      // Turn on flag that indicates we are in a node for Footer processing.
             {StringBuilder sb_szInNodeFlag;
            if ( szInNodeFlag == null )
               sb_szInNodeFlag = new StringBuilder( 32 );
            else
               sb_szInNodeFlag = new StringBuilder( szInNodeFlag );
                        ZeidonStringCopy( sb_szInNodeFlag, 1, 0, "Y", 1, 0, 2 );
            szInNodeFlag = sb_szInNodeFlag.toString( );}

            //:// Close any Entity Tag Pairs whose Hierarchical level is the same or less than this one.
            //:IF vReportDef.XML_Parent EXISTS
            lTempInteger_6 = CheckExistenceOfEntity( vReportDef, "XML_Parent" );
            if ( lTempInteger_6 == 0 )
            { 
               //:LOOP WHILE vReportDef.XML_Parent EXISTS AND ReturnedHierLevel <= vReportDef.XML_Parent.HierarchicalLevel
               lTempInteger_7 = CheckExistenceOfEntity( vReportDef, "XML_Parent" );
               while ( lTempInteger_7 == 0 && CompareAttributeToInteger( vReportDef, "XML_Parent", "HierarchicalLevel", ReturnedHierLevel ) >= 0 )
               { 
                  //:// Format Entity Terminator Tag.
                  //:szParentEntityName = vReportDef.XML_Parent.EntityName
                  {MutableInt mi_lTempInteger_8 = new MutableInt( lTempInteger_8 );
                  StringBuilder sb_szParentEntityName;
                  if ( szParentEntityName == null )
                     sb_szParentEntityName = new StringBuilder( 32 );
                  else
                     sb_szParentEntityName = new StringBuilder( szParentEntityName );
                                     GetVariableFromAttribute( sb_szParentEntityName, mi_lTempInteger_8, 'S', 33, vReportDef, "XML_Parent", "EntityName", "", 0 );
                  lTempInteger_8 = mi_lTempInteger_8.intValue( );
                  szParentEntityName = sb_szParentEntityName.toString( );}

                  //:Indentation = vReportDef.XML_Parent.HierarchicalLevel - 1
                  {MutableInt mi_lTempInteger_9 = new MutableInt( lTempInteger_9 );
                                     GetIntegerFromAttribute( mi_lTempInteger_9, vReportDef, "XML_Parent", "HierarchicalLevel" );
                  lTempInteger_9 = mi_lTempInteger_9.intValue( );}
                  Indentation = lTempInteger_9 - 1;
                  //:szIndentationValue = ""
                   {StringBuilder sb_szIndentationValue;
                  if ( szIndentationValue == null )
                     sb_szIndentationValue = new StringBuilder( 32 );
                  else
                     sb_szIndentationValue = new StringBuilder( szIndentationValue );
                                    ZeidonStringCopy( sb_szIndentationValue, 1, 0, "", 1, 0, 51 );
                  szIndentationValue = sb_szIndentationValue.toString( );}
                  //:LOOP WHILE Indentation > 0
                  while ( Indentation > 0 )
                  { 
                     //:szIndentationValue = szIndentationValue + "   "
                      {StringBuilder sb_szIndentationValue;
                     if ( szIndentationValue == null )
                        sb_szIndentationValue = new StringBuilder( 32 );
                     else
                        sb_szIndentationValue = new StringBuilder( szIndentationValue );
                                          ZeidonStringConcat( sb_szIndentationValue, 1, 0, "   ", 1, 0, 51 );
                     szIndentationValue = sb_szIndentationValue.toString( );}
                     //:Indentation = Indentation - 1
                     Indentation = Indentation - 1;
                  } 

                  //:END

                  //:szOutputLine = szIndentationValue + "   </" + szParentEntityName + ">"
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, "   </", 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, szParentEntityName, 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                  //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

                  //:// If this isn't a repeat of the last entity, close the last entity Group.
                  //:// Also, create Footer, if there is one.
                  //:IF szLastReturnedEntityName != szReturnedEntityName
                  if ( ZeidonStringCompare( szLastReturnedEntityName, 1, 0, szReturnedEntityName, 1, 0, 33 ) != 0 )
                  { 

                     //:// Close Group.
                     //:szOutputLine = szIndentationValue + "</GR_" + szParentEntityName + ">"
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringConcat( sb_szOutputLine, 1, 0, "</GR_", 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringConcat( sb_szOutputLine, 1, 0, szParentEntityName, 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                     //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
                  } 

                  //:END

                  //:DELETE ENTITY vReportDef.XML_Parent
                  RESULT = DeleteEntity( vReportDef, "XML_Parent", zPOS_NEXT );
                  lTempInteger_7 = CheckExistenceOfEntity( vReportDef, "XML_Parent" );
               } 

               //:END
            } 

            //:END

            //:// Determine Indentation Characteristics.
            //:Indentation = ReturnedHierLevel - 1
            Indentation = ReturnedHierLevel - 1;
            //:szIndentationValue = ""
             {StringBuilder sb_szIndentationValue;
            if ( szIndentationValue == null )
               sb_szIndentationValue = new StringBuilder( 32 );
            else
               sb_szIndentationValue = new StringBuilder( szIndentationValue );
                        ZeidonStringCopy( sb_szIndentationValue, 1, 0, "", 1, 0, 51 );
            szIndentationValue = sb_szIndentationValue.toString( );}
            //:LOOP WHILE Indentation > 0
            while ( Indentation > 0 )
            { 
               //:szIndentationValue = szIndentationValue + "   "
                {StringBuilder sb_szIndentationValue;
               if ( szIndentationValue == null )
                  sb_szIndentationValue = new StringBuilder( 32 );
               else
                  sb_szIndentationValue = new StringBuilder( szIndentationValue );
                              ZeidonStringConcat( sb_szIndentationValue, 1, 0, "   ", 1, 0, 51 );
               szIndentationValue = sb_szIndentationValue.toString( );}
               //:Indentation = Indentation - 1
               Indentation = Indentation - 1;
            } 

            //:END

            //:// Format Opening Group and Entity Tags
            //:// Generate Group entry if the last Entity WAS NOT the same as the current Entity. Also add the Termination Entity in that case.
            //:SET CURSOR FIRST vReportDef.XML_Parent WHERE vReportDef.XML_Parent.EntityName = szReturnedEntityName
            RESULT = SetCursorFirstEntityByString( vReportDef, "XML_Parent", "EntityName", szReturnedEntityName, "" );
            //:IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 

               //:// If this entity isn't the same as the last one, also create an open Group entry and process a header..
               //:IF szLastReturnedEntityName != szReturnedEntityName
               if ( ZeidonStringCompare( szLastReturnedEntityName, 1, 0, szReturnedEntityName, 1, 0, 33 ) != 0 )
               { 

                  //:// Create open Group.
                  //:szOutputLine = szIndentationValue + "<GR_" + szReturnedEntityName + ">"
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, "<GR_", 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, szReturnedEntityName, 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                   {StringBuilder sb_szOutputLine;
                  if ( szOutputLine == null )
                     sb_szOutputLine = new StringBuilder( 32 );
                  else
                     sb_szOutputLine = new StringBuilder( szOutputLine );
                                    ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
                  szOutputLine = sb_szOutputLine.toString( );}
                  //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                  oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

                  //:// Process Header if there is one.
                  //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "gh"
                  RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "gh", "" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:szOutputLine = szIndentationValue + "   <Header>"
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringConcat( sb_szOutputLine, 1, 0, "   <Header>", 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                     //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

                     //:szIndentationSubValue = szIndentationValue + "      "
                      {StringBuilder sb_szIndentationSubValue;
                     if ( szIndentationSubValue == null )
                        sb_szIndentationSubValue = new StringBuilder( 32 );
                     else
                        sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
                                          ZeidonStringCopy( sb_szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
                     szIndentationSubValue = sb_szIndentationSubValue.toString( );}
                      {StringBuilder sb_szIndentationSubValue;
                     if ( szIndentationSubValue == null )
                        sb_szIndentationSubValue = new StringBuilder( 32 );
                     else
                        sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
                                          ZeidonStringConcat( sb_szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
                     szIndentationSubValue = sb_szIndentationSubValue.toString( );}
                     //:GenerateXML_CtlRecurs( vReportDef, vSourceOIHier, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue )
                     oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOIHier, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue );

                     //:szOutputLine = szIndentationValue + "   </Header>"
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                      {StringBuilder sb_szOutputLine;
                     if ( szOutputLine == null )
                        sb_szOutputLine = new StringBuilder( 32 );
                     else
                        sb_szOutputLine = new StringBuilder( szOutputLine );
                                          ZeidonStringConcat( sb_szOutputLine, 1, 0, "   </Header>", 1, 0, 5001 );
                     szOutputLine = sb_szOutputLine.toString( );}
                     //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
                     oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
                  } 

                  //:END
               } 

               //:END

               //:// Add work entity for handling later close of the group..
               //:SET CURSOR LAST vReportDef.XML_Parent
               RESULT = SetCursorLastEntity( vReportDef, "XML_Parent", "" );
               //:CREATE ENTITY vReportDef.XML_Parent
               RESULT = CreateEntity( vReportDef, "XML_Parent", zPOS_AFTER );
               //:vReportDef.XML_Parent.EntityName        = szReturnedEntityName
               SetAttributeFromString( vReportDef, "XML_Parent", "EntityName", szReturnedEntityName );
               //:vReportDef.XML_Parent.HierarchicalLevel = ReturnedHierLevel
               SetAttributeFromInteger( vReportDef, "XML_Parent", "HierarchicalLevel", ReturnedHierLevel );
            } 

            //:END
            //:szOutputLine = szIndentationValue + "   <" + szReturnedEntityName + ">"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "   <", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szReturnedEntityName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

            //:// Go to generate an attribute entry for each attribute with mapping in the Detail Group of the GroupSet.
            //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "ga"
            RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "ga", "" );
            //:szIndentationSubValue = szIndentationValue + "   "
             {StringBuilder sb_szIndentationSubValue;
            if ( szIndentationSubValue == null )
               sb_szIndentationSubValue = new StringBuilder( 32 );
            else
               sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
                        ZeidonStringCopy( sb_szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
            szIndentationSubValue = sb_szIndentationSubValue.toString( );}
             {StringBuilder sb_szIndentationSubValue;
            if ( szIndentationSubValue == null )
               sb_szIndentationSubValue = new StringBuilder( 32 );
            else
               sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
                        ZeidonStringConcat( sb_szIndentationSubValue, 1, 0, "   ", 1, 0, 51 );
            szIndentationSubValue = sb_szIndentationSubValue.toString( );}

            //:GenerateXML_CtlRecurs( vReportDef, vSourceOIHier, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue )
            oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOIHier, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue );
         } 


         //:END

         //:szLastReturnedEntityName = szReturnedEntityName   // We need to keep the last entity name for comparison.
          {StringBuilder sb_szLastReturnedEntityName;
         if ( szLastReturnedEntityName == null )
            sb_szLastReturnedEntityName = new StringBuilder( 32 );
         else
            sb_szLastReturnedEntityName = new StringBuilder( szLastReturnedEntityName );
                  ZeidonStringCopy( sb_szLastReturnedEntityName, 1, 0, szReturnedEntityName, 1, 0, 33 );
         szLastReturnedEntityName = sb_szLastReturnedEntityName.toString( );}
         //:nRC = SetCursorNextEntityHierarchical( ReturnedHierLevel, szReturnedEntityName, vSourceOIHier )
         {StringBuilder sb_szReturnedEntityName;
         if ( szReturnedEntityName == null )
            sb_szReturnedEntityName = new StringBuilder( 32 );
         else
            sb_szReturnedEntityName = new StringBuilder( szReturnedEntityName );
         MutableInt mi_ReturnedHierLevel = new MutableInt( ReturnedHierLevel );
                   nRC = SetCursorNextEntityHierarchical( mi_ReturnedHierLevel, sb_szReturnedEntityName, vSourceOIHier );
         szReturnedEntityName = sb_szReturnedEntityName.toString( );
         ReturnedHierLevel = mi_ReturnedHierLevel.intValue( );}
      } 

      //:END
      //:DropView( vSourceOIHier )
      DropView( vSourceOIHier );

      //:// Write out any Remaining Tag Terminators in XML_Parent.
      //:SET CURSOR LAST vReportDef.XML_Parent
      RESULT = SetCursorLastEntity( vReportDef, "XML_Parent", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:LOOP WHILE vReportDef.XML_Parent EXISTS
         lTempInteger_10 = CheckExistenceOfEntity( vReportDef, "XML_Parent" );
         while ( lTempInteger_10 == 0 )
         { 
            //:// Format Entity Terminator Tag.
            //:szParentEntityName = vReportDef.XML_Parent.EntityName
            {MutableInt mi_lTempInteger_11 = new MutableInt( lTempInteger_11 );
            StringBuilder sb_szParentEntityName;
            if ( szParentEntityName == null )
               sb_szParentEntityName = new StringBuilder( 32 );
            else
               sb_szParentEntityName = new StringBuilder( szParentEntityName );
                         GetVariableFromAttribute( sb_szParentEntityName, mi_lTempInteger_11, 'S', 33, vReportDef, "XML_Parent", "EntityName", "", 0 );
            lTempInteger_11 = mi_lTempInteger_11.intValue( );
            szParentEntityName = sb_szParentEntityName.toString( );}

            //:Indentation = vReportDef.XML_Parent.HierarchicalLevel - 1
            {MutableInt mi_lTempInteger_12 = new MutableInt( lTempInteger_12 );
                         GetIntegerFromAttribute( mi_lTempInteger_12, vReportDef, "XML_Parent", "HierarchicalLevel" );
            lTempInteger_12 = mi_lTempInteger_12.intValue( );}
            Indentation = lTempInteger_12 - 1;
            //:szIndentationValue = ""
             {StringBuilder sb_szIndentationValue;
            if ( szIndentationValue == null )
               sb_szIndentationValue = new StringBuilder( 32 );
            else
               sb_szIndentationValue = new StringBuilder( szIndentationValue );
                        ZeidonStringCopy( sb_szIndentationValue, 1, 0, "", 1, 0, 51 );
            szIndentationValue = sb_szIndentationValue.toString( );}
            //:LOOP WHILE Indentation > 0
            while ( Indentation > 0 )
            { 
               //:szIndentationValue = szIndentationValue + "   "
                {StringBuilder sb_szIndentationValue;
               if ( szIndentationValue == null )
                  sb_szIndentationValue = new StringBuilder( 32 );
               else
                  sb_szIndentationValue = new StringBuilder( szIndentationValue );
                              ZeidonStringConcat( sb_szIndentationValue, 1, 0, "   ", 1, 0, 51 );
               szIndentationValue = sb_szIndentationValue.toString( );}
               //:Indentation = Indentation - 1
               Indentation = Indentation - 1;
            } 

            //:END

            //:szOutputLine = szIndentationValue + "   </" + szParentEntityName + ">"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "   </", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szParentEntityName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
            //:szOutputLine = szIndentationValue + "</GR_" + szParentEntityName + ">"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "</GR_", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szParentEntityName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
            oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
            //:DELETE ENTITY vReportDef.XML_Parent
            RESULT = DeleteEntity( vReportDef, "XML_Parent", zPOS_NEXT );
            lTempInteger_10 = CheckExistenceOfEntity( vReportDef, "XML_Parent" );
         } 

         //:END
      } 

      //:END

      //:nRC = SetCursorNextEntity( vSourceOI, szTopEntityName, "" )
      nRC = SetCursorNextEntity( vSourceOI, szTopEntityName, "" );
   } 

   //:END

   //:// Reposition on Top Entity Report Group for Footer.
   //:SET CURSOR FIRST vReportDef.GroupSet WHERE vReportDef.GroupSet.Tag = szTopEntityName
   RESULT = SetCursorFirstEntityByString( vReportDef, "GroupSet", "Tag", szTopEntityName, "" );

   //:// Process Top Entity Footer, if it exists.
   //:SET CURSOR FIRST vReportDef.Group WHERE vReportDef.Group.Type = "gf"
   RESULT = SetCursorFirstEntityByString( vReportDef, "Group", "Type", "gf", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szOutputLine = szIndentationValue + "   <Footer>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "   <Footer>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

      //:szIndentationSubValue = szIndentationValue + "      "
       {StringBuilder sb_szIndentationSubValue;
      if ( szIndentationSubValue == null )
         sb_szIndentationSubValue = new StringBuilder( 32 );
      else
         sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
            ZeidonStringCopy( sb_szIndentationSubValue, 1, 0, szIndentationValue, 1, 0, 51 );
      szIndentationSubValue = sb_szIndentationSubValue.toString( );}
       {StringBuilder sb_szIndentationSubValue;
      if ( szIndentationSubValue == null )
         sb_szIndentationSubValue = new StringBuilder( 32 );
      else
         sb_szIndentationSubValue = new StringBuilder( szIndentationSubValue );
            ZeidonStringConcat( sb_szIndentationSubValue, 1, 0, "      ", 1, 0, 51 );
      szIndentationSubValue = sb_szIndentationSubValue.toString( );}
      //:GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue )
      oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOI, szDrivingViewName, szOutputLine, lFileHandle, szIndentationSubValue );

      //:szOutputLine = szIndentationValue + "   </Footer>"
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
       {StringBuilder sb_szOutputLine;
      if ( szOutputLine == null )
         sb_szOutputLine = new StringBuilder( 32 );
      else
         sb_szOutputLine = new StringBuilder( szOutputLine );
            ZeidonStringConcat( sb_szOutputLine, 1, 0, "   </Footer>", 1, 0, 5001 );
      szOutputLine = sb_szOutputLine.toString( );}
      //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
      oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
   } 

   //:END
   //:szOutputLine = "   </GR_" + szTopEntityName + ">"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "   </GR_", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, szTopEntityName, 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );

   //:// Terminate and close the XML file.
   //:szOutputLine = "</zOI>"
    {StringBuilder sb_szOutputLine;
   if ( szOutputLine == null )
      sb_szOutputLine = new StringBuilder( 32 );
   else
      sb_szOutputLine = new StringBuilder( szOutputLine );
      ZeidonStringCopy( sb_szOutputLine, 1, 0, "</zOI>", 1, 0, 5001 );
   szOutputLine = sb_szOutputLine.toString( );}
   //:GenerateXMLLine( vReportDef, lFileHandle, szOutputLine )
   oTZRPSRCO_GenerateXMLLine( vReportDef, lFileHandle, szOutputLine );
   //:SysCloseFile( vReportDef, lFileHandle, 0 )
   try
   {
       m_KZOEP1AA.SysCloseFile( vReportDef, lFileHandle, null );
   }
   catch ( Exception e )
   {
      throw ZeidonException.wrapException( e );
   }
   return( 0 );
// END
} 


//:LOCAL OPERATION
private int 
oTZRPSRCO_GenerateXMLLine( View     ReportObject,
                           int      lFileHandle,
                           String   szOutputLine )
{

   //:GenerateXMLLine( VIEW ReportObject BASED ON LOD TZRPSRCO,
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
//:GenerateXML_CtlRecurs( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                       VIEW vSourceOIHier,
//:                       STRING ( 32 )   szDrivingObjectViewName,
//:                       STRING ( 5000 ) szOutputLine,
//:                       INTEGER         lFileHandle,
//:                       STRING ( 50 )   szIndentationValue )

//:   VIEW vMappingOI
private int 
oTZRPSRCO_GenerateXML_CtlRecurs( View     vReportDef,
                                 View     vSourceOIHier,
                                 String   szDrivingObjectViewName,
                                 String   szOutputLine,
                                 int      lFileHandle,
                                 String   szIndentationValue )
{
   zVIEW    vMappingOI = new zVIEW( );
   //:VIEW vSourceOI
   zVIEW    vSourceOI = new zVIEW( );
   //:STRING ( 32 )   szEntityName
   String   szEntityName = null;
   //:STRING ( 32 )   szAttributeName
   String   szAttributeName = null;
   //:STRING ( 32 )   szMappingName
   String   szMappingName = null;
   //:STRING ( 5000 ) szReturnedAttributeValue
   String   szReturnedAttributeValue = null;
   //:STRING ( 5000 ) szConvertedAttributeValue
   String   szConvertedAttributeValue = null;
   //:STRING ( 200 )  szMsg
   String   szMsg = null;
   int      RESULT = 0;
   int      lTempInteger_0 = 0;
   int      lTempInteger_1 = 0;
   int      lTempInteger_2 = 0;
   int      lTempInteger_3 = 0;
   int      lTempInteger_4 = 0;


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
         //:GenerateXML_CtlRecurs( vReportDef, vSourceOIHier, szDrivingObjectViewName, szOutputLine, lFileHandle, szIndentationValue )
         oTZRPSRCO_GenerateXML_CtlRecurs( vReportDef, vSourceOIHier, szDrivingObjectViewName, szOutputLine, lFileHandle, szIndentationValue );
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
             {StringBuilder sb_szReturnedAttributeValue;
            if ( szReturnedAttributeValue == null )
               sb_szReturnedAttributeValue = new StringBuilder( 32 );
            else
               sb_szReturnedAttributeValue = new StringBuilder( szReturnedAttributeValue );
                        ZeidonStringCopy( sb_szReturnedAttributeValue, 1, 0, "", 1, 0, 5001 );
            szReturnedAttributeValue = sb_szReturnedAttributeValue.toString( );}
            //:szConvertedAttributeValue = ""
             {StringBuilder sb_szConvertedAttributeValue;
            if ( szConvertedAttributeValue == null )
               sb_szConvertedAttributeValue = new StringBuilder( 32 );
            else
               sb_szConvertedAttributeValue = new StringBuilder( szConvertedAttributeValue );
                        ZeidonStringCopy( sb_szConvertedAttributeValue, 1, 0, "", 1, 0, 5001 );
            szConvertedAttributeValue = sb_szConvertedAttributeValue.toString( );}
            //:szAttributeName = vReportDef.CtrlMapER_Attribute.Name
            {MutableInt mi_lTempInteger_2 = new MutableInt( lTempInteger_2 );
            StringBuilder sb_szAttributeName;
            if ( szAttributeName == null )
               sb_szAttributeName = new StringBuilder( 32 );
            else
               sb_szAttributeName = new StringBuilder( szAttributeName );
                         GetVariableFromAttribute( sb_szAttributeName, mi_lTempInteger_2, 'S', 33, vReportDef, "CtrlMapER_Attribute", "Name", "", 0 );
            lTempInteger_2 = mi_lTempInteger_2.intValue( );
            szAttributeName = sb_szAttributeName.toString( );}
            //:szEntityName    = vReportDef.CtrlMapRelatedEntity.Name
            {MutableInt mi_lTempInteger_3 = new MutableInt( lTempInteger_3 );
            StringBuilder sb_szEntityName;
            if ( szEntityName == null )
               sb_szEntityName = new StringBuilder( 32 );
            else
               sb_szEntityName = new StringBuilder( szEntityName );
                         GetVariableFromAttribute( sb_szEntityName, mi_lTempInteger_3, 'S', 33, vReportDef, "CtrlMapRelatedEntity", "Name", "", 0 );
            lTempInteger_3 = mi_lTempInteger_3.intValue( );
            szEntityName = sb_szEntityName.toString( );}
            //:IF vReportDef.CtrlMapView.Name = szDrivingObjectViewName   // Check if mapping if from Driving Object View.
            if ( CompareAttributeToString( vReportDef, "CtrlMapView", "Name", szDrivingObjectViewName ) == 0 )
            { 
               //:// Mapping is from hierarchical view.
               //:CreateViewFromView( vSourceOI, vSourceOIHier )
               CreateViewFromView( vSourceOI, vSourceOIHier );
               //:GetStringFromAttributeByContext( szReturnedAttributeValue, vSourceOI, szEntityName, szAttributeName, "", 5000 )
               {StringBuilder sb_szReturnedAttributeValue;
               if ( szReturnedAttributeValue == null )
                  sb_szReturnedAttributeValue = new StringBuilder( 32 );
               else
                  sb_szReturnedAttributeValue = new StringBuilder( szReturnedAttributeValue );
                               GetStringFromAttributeByContext( sb_szReturnedAttributeValue, vSourceOI, szEntityName, szAttributeName, "", 5000 );
               szReturnedAttributeValue = sb_szReturnedAttributeValue.toString( );}
               //:DropView( vSourceOI )
               DropView( vSourceOI );
               //:ELSE
            } 
            else
            { 
               //:// Mapping is from separate view.
               //:szMappingName = vReportDef.CtrlMapView.Name
               {MutableInt mi_lTempInteger_4 = new MutableInt( lTempInteger_4 );
               StringBuilder sb_szMappingName;
               if ( szMappingName == null )
                  sb_szMappingName = new StringBuilder( 32 );
               else
                  sb_szMappingName = new StringBuilder( szMappingName );
                               GetVariableFromAttribute( sb_szMappingName, mi_lTempInteger_4, 'S', 33, vReportDef, "CtrlMapView", "Name", "", 0 );
               lTempInteger_4 = mi_lTempInteger_4.intValue( );
               szMappingName = sb_szMappingName.toString( );}
               //:GET VIEW vMappingOI NAMED szMappingName
               RESULT = GetViewByName( vMappingOI, szMappingName, vReportDef, zLEVEL_TASK );
               //:GetStringFromAttributeByContext( szReturnedAttributeValue, vMappingOI, szEntityName, szAttributeName, "", 5000 )
               {StringBuilder sb_szReturnedAttributeValue;
               if ( szReturnedAttributeValue == null )
                  sb_szReturnedAttributeValue = new StringBuilder( 32 );
               else
                  sb_szReturnedAttributeValue = new StringBuilder( szReturnedAttributeValue );
                               GetStringFromAttributeByContext( sb_szReturnedAttributeValue, vMappingOI, szEntityName, szAttributeName, "", 5000 );
               szReturnedAttributeValue = sb_szReturnedAttributeValue.toString( );}
            } 

            //:END

            //:ConvertXML_SpecialCharacters( vReportDef, szConvertedAttributeValue, szReturnedAttributeValue, 5000 )
            {
             KZRPSRCO_Operation m_KZRPSRCO_Operation = new KZRPSRCO_Operation( vReportDef );
             {StringBuilder sb_szConvertedAttributeValue;
            if ( szConvertedAttributeValue == null )
               sb_szConvertedAttributeValue = new StringBuilder( 32 );
            else
               sb_szConvertedAttributeValue = new StringBuilder( szConvertedAttributeValue );
                         m_KZRPSRCO_Operation.ConvertXML_SpecialCharacters( vReportDef, sb_szConvertedAttributeValue, szReturnedAttributeValue, 5000 );
            szConvertedAttributeValue = sb_szConvertedAttributeValue.toString( );}
             // m_KZRPSRCO_Operation = null;  // permit gc  (unnecessary)
            }
            //://szConvertedAttributeValue = szReturnedAttributeValue

            //:szOutputLine = szIndentationValue + "  " + "<" + szEntityName + "." + szAttributeName + ">" + szConvertedAttributeValue
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringCopy( sb_szOutputLine, 1, 0, szIndentationValue, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "  ", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "<", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ".", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szAttributeName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szConvertedAttributeValue, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
            //:szOutputLine = szOutputLine + "</" + szEntityName + "." + szAttributeName + ">"
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, "</", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szEntityName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ".", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, szAttributeName, 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
             {StringBuilder sb_szOutputLine;
            if ( szOutputLine == null )
               sb_szOutputLine = new StringBuilder( 32 );
            else
               sb_szOutputLine = new StringBuilder( szOutputLine );
                        ZeidonStringConcat( sb_szOutputLine, 1, 0, ">", 1, 0, 5001 );
            szOutputLine = sb_szOutputLine.toString( );}
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



}
