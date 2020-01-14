#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZVMLIP.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

//:GLOBAL OPERATION
//:BuildSideNavSectionJ( VIEW vDialog,
//:                      VIEW vDialogRoot BASED ON LOD TZWDLGSO,
//:                      INTEGER lFile,
//:                      STRING (1) szReusableMenu,
//:                      VIEW vDialogMenu BASED ON LOD TZWDLGSO )

//:   STRING ( 10000 ) szWriteBuffer
zOPER_EXPORT zSHORT OPERATION
BuildSideNavSectionJ( zVIEW     vDialog,
                      zVIEW     vDialogRoot,
                      zLONG     lFile,
                      zPCHAR    szReusableMenu,
                      zVIEW     vDialogMenu )
{
   zCHAR     szWriteBuffer[ 10001 ] = { 0 }; 
   //:STRING ( 500 )   szNavigationTitle
   zCHAR     szNavigationTitle[ 501 ] = { 0 }; 
   //:STRING ( 500 )   szText
   zCHAR     szText[ 501 ] = { 0 }; 
   //:STRING ( 256 )   szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 5 )     szSrch
   zCHAR     szSrch[ 6 ] = { 0 }; 
   //:STRING ( 50 )    szHTML_Name
   zCHAR     szHTML_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 )    szDialogName
   zCHAR     szDialogName[ 51 ] = { 0 }; 
   //:STRING ( 50 )    szOptionTag
   zCHAR     szOptionTag[ 51 ] = { 0 }; 
   //:STRING ( 50 )    szMenuName
   zCHAR     szMenuName[ 51 ] = { 0 }; 
   //://STRING ( 34 )    szActionName
   //:STRING ( 100 )   szActionName
   zCHAR     szActionName[ 101 ] = { 0 }; 
   //:STRING ( 1 )     szStyleIsjMobile
   zCHAR     szStyleIsjMobile[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szStyleIsBootstrap
   zCHAR     szStyleIsBootstrap[ 2 ] = { 0 }; 
   //:STRING ( 256 )   szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 )   szClass2
   zCHAR     szClass2[ 257 ] = { 0 }; 
   //:INTEGER          ActionType
   zLONG     ActionType = 0; 
   //:SHORT            nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 255 ]; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_4[ 255 ]; 
   zCHAR     szTempString_5[ 255 ]; 
   zLONG     lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 255 ]; 
   zCHAR     szTempString_8[ 255 ]; 
   zCHAR     szTempString_9[ 255 ]; 

   //:// SIDE NAVIGATION BAR
   //://TraceLineS( "*** BuildSideNavSection Window *** ", vDialog.Window.Tag )

   //:szStyleIsjMobile = ""
   ZeidonStringCopy( szStyleIsjMobile, 1, 0, "", 1, 0, 2 );
   //:IF vDialog.WndStyle.Tag = "jMobile Window" OR vDialog.Window.WEB_JSPGenerationPositioning = "J" OR 
   //:   ( vDialogRoot.Dialog.WEB_JSPGenerationPositioning = "J" AND vDialog.Window.WEB_JSPGenerationPositioning = "" )
   if ( CompareAttributeToString( vDialog, "WndStyle", "Tag", "jMobile Window" ) == 0 || CompareAttributeToString( vDialog, "Window", "WEB_JSPGenerationPositioning", "J" ) == 0 ||
        ( CompareAttributeToString( vDialogRoot, "Dialog", "WEB_JSPGenerationPositioning", "J" ) == 0 && CompareAttributeToString( vDialog, "Window", "WEB_JSPGenerationPositioning", "" ) == 0 ) )
   { 
      //:szStyleIsjMobile = "Y"
      ZeidonStringCopy( szStyleIsjMobile, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END      
   //:szStyleIsBootstrap = ""
   ZeidonStringCopy( szStyleIsBootstrap, 1, 0, "", 1, 0, 2 );
   //:IF vDialog.Window.WEB_JSPGenerationPositioning = "B" OR 
   //:   ( vDialogRoot.Dialog.WEB_JSPGenerationPositioning = "B" AND vDialog.Window.WEB_JSPGenerationPositioning = "" )
   if ( CompareAttributeToString( vDialog, "Window", "WEB_JSPGenerationPositioning", "B" ) == 0 || ( CompareAttributeToString( vDialogRoot, "Dialog", "WEB_JSPGenerationPositioning", "B" ) == 0 &&
        CompareAttributeToString( vDialog, "Window", "WEB_JSPGenerationPositioning", "" ) == 0 ) )
   { 
      //:szStyleIsBootstrap = "Y"
      ZeidonStringCopy( szStyleIsBootstrap, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END 
   //:     
   //:IF vDialogRoot.Menu EXISTS 
   lTempInteger_0 = CheckExistenceOfEntity( vDialogRoot, "Menu" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szMenuName = vDialogRoot.Menu.Tag 
      GetVariableFromAttribute( szMenuName, 0, 'S', 51, vDialogRoot, "Menu", "Tag", "", 0 );
      //:IF vDialogRoot.Menu.CSS_Class != ""
      if ( CompareAttributeToString( vDialogRoot, "Menu", "CSS_Class", "" ) != 0 )
      { 
         //:szClass = " class=^" + vDialogRoot.Menu.CSS_Class + "^ " 
         GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vDialogRoot, "Menu", "CSS_Class", "", 0 );
         ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
      } 

      //:END
   } 

   //:END

   //:szWriteBuffer = ""
   ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:IF szStyleIsBootstrap = ""
   if ( ZeidonStringCompare( szStyleIsBootstrap, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:szWriteBuffer = "<div id=^leftcontent^>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div id=^leftcontent^>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:szWriteBuffer = "<!-- Side Navigation *********************** -->"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<!-- Side Navigation *********************** -->", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "<div id=^sidenavigation^" + szClass + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div id=^sidenavigation^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "<nav id=^sidebar^ class=^sidebar^>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<nav id=^sidebar^ class=^sidebar^>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "<%@ include file=^./include/sidemenubootstrap.inc^ %>"    
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%@ include file=^./include/sidemenubootstrap.inc^ %>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "<div class=^sidebar-content^>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div class=^sidebar-content^>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   } 

   //:   //szWriteBuffer = "<div class=^sidebar-user^>"
   //:   //WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:   //szWriteBuffer = "</div>"
   //:   //WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:END
   //:IF szStyleIsjMobile = "Y" 
   if ( ZeidonStringCompare( szStyleIsjMobile, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:szWriteBuffer = "   <ul id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szClass + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   <ul id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:IF szStyleIsBootstrap = "Y"
      if ( ZeidonStringCompare( szStyleIsBootstrap, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:szWriteBuffer = "   <ul  id=^available-apps^ class=^sidebar-nav^>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <ul  id=^available-apps^ class=^sidebar-nav^>", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "   <ol id=^" + szMenuName + "^ name=^" + szMenuName + "^>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <ol id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^>", 1, 0, 10001 );
      } 

      //:END
   } 

   //:END
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );


   //://FOR EACH vDialogRoot.OptAct WITHIN vDialogRoot.Menu
   //:FOR EACH vDialogRoot.Option WITHIN vDialogRoot.Menu
   RESULT = SetCursorFirstEntity( vDialogRoot, "Option", "Menu" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //://szNavigationTitle = vDialogRoot.Option.Text
      //:// KJS 09/30/16 - Language Conversion.
      //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
      //:// So for now, I am always using, if the flag is set.
      //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:szText = vDialogRoot.Option.Text
         GetVariableFromAttribute( szText, 0, 'S', 501, vDialogRoot, "Option", "Text", "", 0 );
         //:szTextID = vDialogRoot.Option.LangID
         GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialogRoot, "Option", "LangID", "", 0 );
         //:zSearchAndReplace( szText, 500, "\", "\\" )
         zSearchAndReplace( szText, 500, "\\", "\\\\" );
         //:szSrch = "\" + QUOTES 
         ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
         ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
         //:zSearchAndReplace( szText, 500, QUOTES, szSrch )
         zSearchAndReplace( szText, 500, QUOTES, szSrch );
         //:IF szTextID = ""
         if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^)%>" 
            ZeidonStringCopy( szNavigationTitle, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, szText, 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, "^)%>", 1, 0, 501 );
            //:ELSE 
         } 
         else
         { 
            //:szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^, ^" + szTextID + "^)%>" 
            ZeidonStringCopy( szNavigationTitle, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, szText, 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, "^, ^", 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, szTextID, 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, "^)%>", 1, 0, 501 );
         } 

         //:END
         //://szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^)%>" 
         //:ELSE
      } 
      else
      { 
         //:szNavigationTitle = vDialogRoot.Option.Text
         GetVariableFromAttribute( szNavigationTitle, 0, 'S', 501, vDialogRoot, "Option", "Text", "", 0 );
      } 

      //:END

      //:IF vDialogRoot.OptAct EXISTS      
      lTempInteger_1 = CheckExistenceOfEntity( vDialogRoot, "OptAct" );
      if ( lTempInteger_1 == 0 )
      { 
         //:szDialogName = vDialogRoot.OptAct.DialogName
         GetVariableFromAttribute( szDialogName, 0, 'S', 51, vDialogRoot, "OptAct", "DialogName", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:szDialogName = ""
         ZeidonStringCopy( szDialogName, 1, 0, "", 1, 0, 51 );
      } 

      //:END
      //:IF szDialogName = ""
      if ( ZeidonStringCompare( szDialogName, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szDialogName = "NoDialogName"
         ZeidonStringCopy( szDialogName, 1, 0, "NoDialogName", 1, 0, 51 );
      } 

      //:END

      //:IF szReusableMenu = "Y"
      if ( ZeidonStringCompare( szReusableMenu, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:szMenuName = "sm" + vDialogRoot.Option.Tag
         GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialogRoot, "Option", "Tag", "", 0 );
         ZeidonStringCopy( szMenuName, 1, 0, "sm", 1, 0, 51 );
         ZeidonStringConcat( szMenuName, 1, 0, szTempString_1, 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szMenuName = vDialogRoot.Option.Tag
         GetVariableFromAttribute( szMenuName, 0, 'S', 51, vDialogRoot, "Option", "Tag", "", 0 );
      } 

      //:END

      //:// szHTML_Name = szDialogName + vDialogRoot.OptAct.WindowName + ".jsp"
      //:IF vDialogRoot.OptAct EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( vDialogRoot, "OptAct" );
      if ( lTempInteger_2 == 0 )
      { 

         //:// Because the menu actions can be created on separate windows from the control
         //:// actions, we need to make sure these actions are unique.  We will prefix a
         //:// "m" to the main menu actions and prefix a "sm" to the side menu actions.
         //:IF szReusableMenu = "Y"
         if ( ZeidonStringCompare( szReusableMenu, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:szActionName = "onclick=^" + "sm" + vDialogRoot.OptAct.Tag + "()^"
            ZeidonStringCopy( szActionName, 1, 0, "onclick=^", 1, 0, 101 );
            ZeidonStringConcat( szActionName, 1, 0, "sm", 1, 0, 101 );
            GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialogRoot, "OptAct", "Tag", "", 0 );
            ZeidonStringConcat( szActionName, 1, 0, szTempString_2, 1, 0, 101 );
            ZeidonStringConcat( szActionName, 1, 0, "()^", 1, 0, 101 );
            //://szActionName = "sm" + vDialogRoot.OptAct.Tag
            //:// lLth = zstrlen( szActionName )
            //:// IF lLth > 32
            //://    MessageSend( vDialog, "", "Reusable Menu Action Name Length > 32",
            //://                 szActionName,
            //://                 zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            //:// END
            //:ELSE
         } 
         else
         { 
            //:szActionName = "onclick=^" + vDialogRoot.OptAct.Tag + "()^"
            GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialogRoot, "OptAct", "Tag", "", 0 );
            ZeidonStringCopy( szActionName, 1, 0, "onclick=^", 1, 0, 101 );
            ZeidonStringConcat( szActionName, 1, 0, szTempString_3, 1, 0, 101 );
            ZeidonStringConcat( szActionName, 1, 0, "()^", 1, 0, 101 );
         } 

         //:   //szActionName = vDialogRoot.OptAct.Tag
         //:END
         //:ActionType = vDialogRoot.OptAct.Type
         GetIntegerFromAttribute( &ActionType, vDialogRoot, "OptAct", "Type" );
         //:ELSE
      } 
      else
      { 
         //:szActionName = ""
         ZeidonStringCopy( szActionName, 1, 0, "", 1, 0, 101 );
         //:ActionType = 0
         ActionType = 0;
      } 

      //:END

      //:// If the window we are currently generating is the same window that this
      //:// side menu points to, then we want to set a class on this option.  The
      //:// reason is so that we can tell which side menu option the user has selected.
      //:IF vDialogRoot.OptAct EXISTS AND vDialogRoot.OptAct.WindowName = vDialog.Window.Tag     
      lTempInteger_3 = CheckExistenceOfEntity( vDialogRoot, "OptAct" );
      if ( lTempInteger_3 == 0 && CompareAttributeToAttribute( vDialogRoot, "OptAct", "WindowName", vDialog, "Window", "Tag" ) == 0 )
      { 
         //://IF vDialogRoot.OptAct.WindowName = vDialog.Window.Tag
         //:szClass2 = " class=^sideselected^ "
         ZeidonStringCopy( szClass2, 1, 0, " class=^sideselected^ ", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szClass2 = ""
         ZeidonStringCopy( szClass2, 1, 0, "", 1, 0, 257 );
      } 

      //:END

      //:IF vDialogRoot.Option.CSS_Class != "" 
      if ( CompareAttributeToString( vDialogRoot, "Option", "CSS_Class", "" ) != 0 )
      { 
         //:szClass = " class=^" + vDialogRoot.Option.CSS_Class + "^"
         GetVariableFromAttribute( szTempString_4, 0, 'S', 255, vDialogRoot, "Option", "CSS_Class", "", 0 );
         ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, szTempString_4, 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szClass = ""
         ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
      } 

      //:END

      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szOptionTag = vDialogRoot.Option.Tag
      GetVariableFromAttribute( szOptionTag, 0, 'S', 51, vDialogRoot, "Option", "Tag", "", 0 );
      //:szWriteBuffer = "   csrRC = vKZXMLPGO.cursor( ^DisableMenuOption^ ).setFirst( ^MenuOptionName^, ^" + szOptionTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   csrRC = vKZXMLPGO.cursor( ^DisableMenuOption^ ).setFirst( ^MenuOptionName^, ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szOptionTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( !csrRC.isSet() ) //if ( nRC < 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( !csrRC.isSet() ) //if ( nRC < 0 )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:IF szStyleIsBootstrap = ""
      if ( ZeidonStringCompare( szStyleIsBootstrap, 1, 0, "", 1, 0, 2 ) == 0 )
      { 
         //://szWriteBuffer = "       <li id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szClass + "><a href=^#^ " + szClass2 + " onclick=^" + szActionName + "()^>" + szNavigationTitle + "</a></li>"
         //:szWriteBuffer = "       <li id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szClass + "><a href=^#^ " + szClass2 + " " + szActionName + ">" + szNavigationTitle + "</a></li>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "       <li id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "><a href=^#^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass2, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szNavigationTitle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</a></li>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:ELSE
      } 
      else
      { 
         //:IF vDialogRoot.Option.WebHTML5Attribute != ""
         if ( CompareAttributeToString( vDialogRoot, "Option", "WebHTML5Attribute", "" ) != 0 )
         { 
            //:szText = "<i class=^align-middle mr-2 fas fa-fw " + vDialogRoot.Option.WebHTML5Attribute  + "^></i>"
            GetVariableFromAttribute( szTempString_5, 0, 'S', 255, vDialogRoot, "Option", "WebHTML5Attribute", "", 0 );
            ZeidonStringCopy( szText, 1, 0, "<i class=^align-middle mr-2 fas fa-fw ", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szTempString_5, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^></i>", 1, 0, 501 );
            //:ELSE
         } 
         else
         { 
            //:szText = ""
            ZeidonStringCopy( szText, 1, 0, "", 1, 0, 501 );
         } 

         //:END
         //:szWriteBuffer = "       <li class=^sidebar-item^ id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szClass + ">"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "       <li class=^sidebar-item^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         //://szWriteBuffer = szWriteBuffer + "<a class=^sidebar-link^ href=^#^ " +  " onclick=^" + szActionName + "()^><i class=^align-middle mr-2 fas fa-fw " + szText  + "^></i> <span class=^align-middle^>" + szNavigationTitle + "</span></a></li>"
         //:szWriteBuffer = szWriteBuffer + "<a class=^sidebar-link^ href=^#^ " +  " " + szActionName + ">" + szText + " <span class=^align-middle^>" + szNavigationTitle + "</span></a></li>"
         ZeidonStringConcat( szWriteBuffer, 1, 0, "<a class=^sidebar-link^ href=^#^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " <span class=^align-middle^>", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szNavigationTitle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</span></a></li>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 


      //:END

      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      RESULT = SetCursorNextEntity( vDialogRoot, "Option", "Menu" );
   } 

   //:END

   //:// This is for building a default menu?
   //:// Trying to get this file to compile.
   //:IF vDialogMenu != 0
   if ( vDialogMenu != 0 )
   { 
      //:CreateViewFromViewForTask( vDialogMenu, vDialogMenu, 0 )
      CreateViewFromViewForTask( &vDialogMenu, vDialogMenu, 0 );
      //:SET CURSOR FIRST vDialogMenu.Menu WHERE vDialogMenu.Menu.ZKey = vDialogMenu.DfltMenu.ZKey
      GetIntegerFromAttribute( &lTempInteger_4, vDialogMenu, "DfltMenu", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vDialogMenu, "Menu", "ZKey", lTempInteger_4, "" );
      //:FOR EACH vDialogMenu.OptAct WITHIN vDialogMenu.Menu
      RESULT = SetCursorFirstEntity( vDialogMenu, "OptAct", "Menu" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //://szNavigationTitle = vDialogMenu.Option.Text
         //:// KJS 09/30/16 - Language Conversion.
         //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
         //:// So for now, I am always using, if the flag is set.
         //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
         if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
         { 
            //:szText = vDialogMenu.Option.Text
            GetVariableFromAttribute( szText, 0, 'S', 501, vDialogMenu, "Option", "Text", "", 0 );
            //:szTextID = vDialogMenu.Option.LangID
            GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialogMenu, "Option", "LangID", "", 0 );
            //:zSearchAndReplace( szText, 500, "\", "\\" )
            zSearchAndReplace( szText, 500, "\\", "\\\\" );
            //:szSrch = "\" + QUOTES 
            ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
            ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
            //:zSearchAndReplace( szText, 500, QUOTES, szSrch )
            zSearchAndReplace( szText, 500, QUOTES, szSrch );
            //:IF szTextID = ""
            if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
            { 
               //:szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^)%>" 
               ZeidonStringCopy( szNavigationTitle, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
               ZeidonStringConcat( szNavigationTitle, 1, 0, szText, 1, 0, 501 );
               ZeidonStringConcat( szNavigationTitle, 1, 0, "^)%>", 1, 0, 501 );
               //:ELSE 
            } 
            else
            { 
               //:szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^, ^" + szTextID + "^)%>" 
               ZeidonStringCopy( szNavigationTitle, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
               ZeidonStringConcat( szNavigationTitle, 1, 0, szText, 1, 0, 501 );
               ZeidonStringConcat( szNavigationTitle, 1, 0, "^, ^", 1, 0, 501 );
               ZeidonStringConcat( szNavigationTitle, 1, 0, szTextID, 1, 0, 501 );
               ZeidonStringConcat( szNavigationTitle, 1, 0, "^)%>", 1, 0, 501 );
            } 

            //:END
            //://szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^)%>" 
            //:ELSE
         } 
         else
         { 
            //:szNavigationTitle = vDialogMenu.Option.Text
            GetVariableFromAttribute( szNavigationTitle, 0, 'S', 501, vDialogMenu, "Option", "Text", "", 0 );
         } 

         //:END
         //:szDialogName = vDialogMenu.OptAct.DialogName
         GetVariableFromAttribute( szDialogName, 0, 'S', 51, vDialogMenu, "OptAct", "DialogName", "", 0 );
         //:IF szDialogName = ""
         if ( ZeidonStringCompare( szDialogName, 1, 0, "", 1, 0, 51 ) == 0 )
         { 
            //:szDialogName = "NoDialogName"
            ZeidonStringCopy( szDialogName, 1, 0, "NoDialogName", 1, 0, 51 );
         } 

         //:END

         //:// szHTML_Name = szDialogName + vDialogMenu.OptAct.WindowName + ".jsp"
         //:IF vDialogMenu.OptAct EXISTS
         lTempInteger_5 = CheckExistenceOfEntity( vDialogMenu, "OptAct" );
         if ( lTempInteger_5 == 0 )
         { 
            //:szActionName = "onclick=^" + vDialogMenu.OptAct.Tag + "()^"
            GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialogMenu, "OptAct", "Tag", "", 0 );
            ZeidonStringCopy( szActionName, 1, 0, "onclick=^", 1, 0, 101 );
            ZeidonStringConcat( szActionName, 1, 0, szTempString_6, 1, 0, 101 );
            ZeidonStringConcat( szActionName, 1, 0, "()^", 1, 0, 101 );
            //://szActionName = vDialogMenu.OptAct.Tag
            //:ActionType = vDialogMenu.OptAct.Type
            GetIntegerFromAttribute( &ActionType, vDialogMenu, "OptAct", "Type" );
            //:ELSE
         } 
         else
         { 
            //:szActionName = ""
            ZeidonStringCopy( szActionName, 1, 0, "", 1, 0, 101 );
            //:ActionType = 0
            ActionType = 0;
         } 

         //:END

         //://szMenuName = vDialogRoot.Option.Tag         
         //:szMenuName = vDialogMenu.Option.Tag         
         GetVariableFromAttribute( szMenuName, 0, 'S', 51, vDialogMenu, "Option", "Tag", "", 0 );

         //:// If the window we are currently generating is the same window that this
         //:// side menu points to, then we want to set a class on this option.  The
         //:// reason is so that we can tell which side menu option the user has selected.
         //:IF vDialogMenu.OptAct.WindowName = vDialog.Window.Tag
         if ( CompareAttributeToAttribute( vDialogMenu, "OptAct", "WindowName", vDialog, "Window", "Tag" ) == 0 )
         { 
            //:szClass2 = "class=^sideselected^ "
            ZeidonStringCopy( szClass2, 1, 0, "class=^sideselected^ ", 1, 0, 257 );
            //:ELSE
         } 
         else
         { 
            //:szClass2 = ""
            ZeidonStringCopy( szClass2, 1, 0, "", 1, 0, 257 );
         } 

         //:END

         //:IF vDialogMenu.Option.CSS_Class != "" 
         if ( CompareAttributeToString( vDialogMenu, "Option", "CSS_Class", "" ) != 0 )
         { 
            //:szClass = "class=^" + vDialogMenu.Option.CSS_Class + "^"
            GetVariableFromAttribute( szTempString_7, 0, 'S', 255, vDialogMenu, "Option", "CSS_Class", "", 0 );
            ZeidonStringCopy( szClass, 1, 0, "class=^", 1, 0, 257 );
            ZeidonStringConcat( szClass, 1, 0, szTempString_7, 1, 0, 257 );
            ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
            //:ELSE
         } 
         else
         { 
            //:szClass = ""
            ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
         } 

         //:END

         //:szWriteBuffer = "<%"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szOptionTag = vDialogMenu.Option.Tag
         GetVariableFromAttribute( szOptionTag, 0, 'S', 51, vDialogMenu, "Option", "Tag", "", 0 );
         //:szOptionTag = vDialogMenu.Option.Tag
         GetVariableFromAttribute( szOptionTag, 0, 'S', 51, vDialogMenu, "Option", "Tag", "", 0 );
         //:szWriteBuffer = "   csrRC = vKZXMLPGO.cursor( ^DisableMenuOption^ ).setFirst( ^MenuOptionName^, ^" + szOptionTag + "^ );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   csrRC = vKZXMLPGO.cursor( ^DisableMenuOption^ ).setFirst( ^MenuOptionName^, ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szOptionTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   if ( !csrRC.isSet( ) )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( !csrRC.isSet( ) )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "%>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:IF szStyleIsBootstrap = ""
         if ( ZeidonStringCompare( szStyleIsBootstrap, 1, 0, "", 1, 0, 2 ) == 0 )
         { 
            //://szWriteBuffer = "       <li id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szClass + "><a href=^#^ " + szClass2 + " onclick=^" + szActionName + "()^>" + szNavigationTitle + "</a></li>"
            //:szWriteBuffer = "       <li id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szClass + "><a href=^#^ " + szClass2 + " " + szActionName + ">" + szNavigationTitle + "</a></li>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "       <li id=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "><a href=^#^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClass2, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szNavigationTitle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</a></li>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:ELSE
         } 
         else
         { 
            //://szText = vDialogMenu.Option.WebHTML5Attribute
            //:IF vDialogMenu.Option.WebHTML5Attribute != ""
            if ( CompareAttributeToString( vDialogMenu, "Option", "WebHTML5Attribute", "" ) != 0 )
            { 
               //:szText = "<i class=^align-middle mr-2 fas fa-fw " + vDialogMenu.Option.WebHTML5Attribute  + "^></i>"
               GetVariableFromAttribute( szTempString_8, 0, 'S', 255, vDialogMenu, "Option", "WebHTML5Attribute", "", 0 );
               ZeidonStringCopy( szText, 1, 0, "<i class=^align-middle mr-2 fas fa-fw ", 1, 0, 501 );
               ZeidonStringConcat( szText, 1, 0, szTempString_8, 1, 0, 501 );
               ZeidonStringConcat( szText, 1, 0, "^></i>", 1, 0, 501 );
               //:ELSE
            } 
            else
            { 
               //:szText = ""
               ZeidonStringCopy( szText, 1, 0, "", 1, 0, 501 );
            } 

            //:END
            //:szWriteBuffer = "       <li class=^sidebar-item^ id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szClass + ">"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "       <li class=^sidebar-item^ id=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
            //://szWriteBuffer = szWriteBuffer + "<a class=^sidebar-link^ href=^#^ " +  " onclick=^" + szActionName + "()^><i class=^align-middle mr-2 fas fa-fw " + szText + "^></i> <span class=^align-middle^>" + szNavigationTitle + "</span></a></li>"
            //:szWriteBuffer = szWriteBuffer + "<a class=^sidebar-link^ href=^#^ " +  " " + szActionName + ">" + szText + " <span class=^align-middle^>" + szNavigationTitle + "</span></a></li>"
            ZeidonStringConcat( szWriteBuffer, 1, 0, "<a class=^sidebar-link^ href=^#^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " <span class=^align-middle^>", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szNavigationTitle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</span></a></li>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         } 


         //:END

         //:szWriteBuffer = "<%"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "%>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
         RESULT = SetCursorNextEntity( vDialogMenu, "OptAct", "Menu" );
      } 

      //:END

      //:DropView( vDialogMenu )
      DropView( vDialogMenu );
   } 

   //:END

   //:// Division ending for Navigation Bar.
   //:IF szStyleIsjMobile = "Y" OR szStyleIsBootstrap = "Y"
   if ( ZeidonStringCompare( szStyleIsjMobile, 1, 0, "Y", 1, 0, 2 ) == 0 || ZeidonStringCompare( szStyleIsBootstrap, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:szWriteBuffer = "   </ul>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   </ul>", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "   </ol>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   </ol>", 1, 0, 10001 );
   } 

   //:END
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:IF szStyleIsBootstrap = ""
   if ( ZeidonStringCompare( szStyleIsBootstrap, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:szWriteBuffer = "</div> <!-- sidenavigation -->"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</div> <!-- sidenavigation -->", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   } 

   //:END
   //:IF vDialogRoot.Dialog.WEB_LeftSideInclude != ""
   if ( CompareAttributeToString( vDialogRoot, "Dialog", "WEB_LeftSideInclude", "" ) != 0 )
   { 
      //://szWriteBuffer = "<%@include file=^./include/leftcontent.inc^ %>"
      //:szWriteBuffer = "<!-- left content include file -->"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<!-- left content include file -->", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "<%@include file=^" + vDialogRoot.Dialog.WEB_LeftSideInclude + "^ %>"
      GetVariableFromAttribute( szTempString_9, 0, 'S', 255, vDialogRoot, "Dialog", "WEB_LeftSideInclude", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%@include file=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ %>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   } 

   //:END
   //:szWriteBuffer = "</div>  <!-- leftcontent -->"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</div>  <!-- leftcontent -->", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:IF szStyleIsBootstrap = "Y"
   if ( ZeidonStringCompare( szStyleIsBootstrap, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:szWriteBuffer = "</nav>  <!-- collapse side nav -->"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</nav>  <!-- collapse side nav -->", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   } 

   //:END
   //:szWriteBuffer = ""
   ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   return( 0 );
//    /*
//    szCSS_Class = vDialog.Window.CSS_Class
//    IF szCSS_Class = ""
//       // Begin the division for the main body content.
//       szWriteBuffer = "<div id=^content^>"
//       WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
//    END
//    */
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_Action( VIEW vDialog     BASED ON LOD TZWDLGSO,
//:                VIEW vDialogRoot BASED ON LOD TZWDLGSO,
//:                INTEGER          lFile,
//:                STRING ( 10000 ) szWriteBuffer,
//:                STRING ( 255 )   szFormName,
//:                STRING ( 34 )    szActionName,
//:                INTEGER          lTraceLevel )

//:   VIEW vLPLR       BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
GenJSPJ_Action( zVIEW     vDialog,
                zVIEW     vDialogRoot,
                zLONG     lFile,
                zPCHAR    szWriteBuffer,
                zPCHAR    szFormName,
                zPCHAR    szActionName,
                zLONG     lTraceLevel )
{
   zVIEW     vLPLR = 0; 
   //:VIEW vDialogTemp BASED ON LOD TZWDLGSO
   zVIEW     vDialogTemp = 0; 
   //:VIEW vDialogCtrl BASED ON LOD TZWDLGSO
   zVIEW     vDialogCtrl = 0; 
   //:STRING ( 10000 ) szJavaScript
   zCHAR     szJavaScript[ 10001 ] = { 0 }; 
   //:STRING ( 32 )    szJavaScriptLocation
   zCHAR     szJavaScriptLocation[ 33 ] = { 0 }; 
   //:STRING ( 64 )    szSystemIniApplName
   zCHAR     szSystemIniApplName[ 65 ] = { 0 }; 
   //:STRING ( 100 )   szViewName
   zCHAR     szViewName[ 101 ] = { 0 }; 
   //:STRING ( 100 )   szEntityName
   zCHAR     szEntityName[ 101 ] = { 0 }; 
   //:STRING ( 100 )   szEntityNameR
   zCHAR     szEntityNameR[ 101 ] = { 0 }; 
   //:STRING ( 100 )   szScopingName
   zCHAR     szScopingName[ 101 ] = { 0 }; 
   //:STRING ( 100 )   szContextName
   zCHAR     szContextName[ 101 ] = { 0 }; 
   //:STRING ( 64 )    szDlgTagRoot
   zCHAR     szDlgTagRoot[ 65 ] = { 0 }; 
   //:STRING ( 100 )   szWndTagRoot
   zCHAR     szWndTagRoot[ 101 ] = { 0 }; 
   //:STRING ( 100 )   szCtrlTag
   zCHAR     szCtrlTag[ 101 ] = { 0 }; 
   //:STRING ( 100 )   szDlgTag
   zCHAR     szDlgTag[ 101 ] = { 0 }; 
   //:STRING ( 64 )    szWndTag
   zCHAR     szWndTag[ 65 ] = { 0 }; 
   //:STRING ( 64 )    szOperName
   zCHAR     szOperName[ 65 ] = { 0 }; 
   //:STRING ( 100 )   szDownloadViewName
   zCHAR     szDownloadViewName[ 101 ] = { 0 }; 
   //:STRING ( 100 )   szUploadViewName
   zCHAR     szUploadViewName[ 101 ] = { 0 }; 
   //:STRING ( 64 )    szWAB
   zCHAR     szWAB[ 65 ] = { 0 }; 
   //:STRING ( 10 )    szMaxStringLth
   zCHAR     szMaxStringLth[ 11 ] = { 0 }; 
   //:STRING ( 50 )    szPrivateKey
   zCHAR     szPrivateKey[ 51 ] = { 0 }; 
   //:STRING ( 50 )    szPublicKey
   zCHAR     szPublicKey[ 51 ] = { 0 }; 
   //:STRING ( 3 )     szBuffer
   zCHAR     szBuffer[ 4 ] = { 0 }; 
   //:STRING ( 1 )     InsertBlankFlag
   zCHAR     InsertBlankFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szCommentAdded
   zCHAR     szCommentAdded[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szDialogClassCreated
   zCHAR     szDialogClassCreated[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szUploadTransferFileFlag
   zCHAR     szUploadTransferFileFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szPosDataCreated
   zCHAR     szPosDataCreated[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szWebRedirection
   zCHAR     szWebRedirection[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szRepeatingGroupFlag
   zCHAR     szRepeatingGroupFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szProductionMode
   zCHAR     szProductionMode[ 2 ] = { 0 }; 
   //:STRING ( 1 )     szGridScopeOI
   zCHAR     szGridScopeOI[ 2 ] = { 0 }; 
   //:INTEGER          lControl
   zLONG     lControl = 0; 
   //:INTEGER          lMaxStringLth
   zLONG     lMaxStringLth = 0; 
   //:INTEGER          lActionType
   zLONG     lActionType = 0; 
   //:INTEGER          lAutoSubAction
   zLONG     lAutoSubAction = 0; 
   //:INTEGER          lAutoNextPrev
   zLONG     lAutoNextPrev = 0; 
   //:INTEGER          lSubtypeX
   zLONG     lSubtypeX = 0; 
   //:SHORT            nViewNameLth
   zSHORT    nViewNameLth = 0; 
   //:SHORT            nGridParent
   zSHORT    nGridParent = 0; 
   //:SHORT            nSystemSort
   zSHORT    nSystemSort = 0; 
   //:SHORT            nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 33 ]; 
   zCHAR     szTempString_13[ 33 ]; 
   zCHAR     szTempString_14[ 33 ]; 
   zCHAR     szTempString_15[ 33 ]; 
   zCHAR     szTempString_16[ 33 ]; 
   zCHAR     szTempString_17[ 33 ]; 
   zCHAR     szTempString_18[ 33 ]; 
   zCHAR     szTempString_19[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zSHORT    lTempInteger_7; 
   zSHORT    lTempInteger_8; 
   zSHORT    lTempInteger_9; 
   zSHORT    lTempInteger_10; 
   zCHAR     szTempString_20[ 33 ]; 
   zCHAR     szTempString_21[ 33 ]; 
   zCHAR     szTempString_22[ 33 ]; 
   zCHAR     szTempString_23[ 33 ]; 
   zSHORT    lTempInteger_11; 
   zSHORT    lTempInteger_12; 
   zSHORT    lTempInteger_13; 
   zSHORT    lTempInteger_14; 
   zCHAR     szTempString_24[ 33 ]; 
   zCHAR     szTempString_25[ 33 ]; 
   zCHAR     szTempString_26[ 33 ]; 
   zCHAR     szTempString_27[ 33 ]; 
   zCHAR     szTempString_28[ 33 ]; 
   zCHAR     szTempString_29[ 33 ]; 
   zSHORT    lTempInteger_15; 
   zCHAR     szTempString_30[ 33 ]; 
   zCHAR     szTempString_31[ 33 ]; 
   zCHAR     szTempString_32[ 33 ]; 
   zCHAR     szTempString_33[ 33 ]; 
   zCHAR     szTempString_34[ 33 ]; 
   zCHAR     szTempString_35[ 33 ]; 
   zCHAR     szTempString_36[ 33 ]; 
   zCHAR     szTempString_37[ 33 ]; 
   zCHAR     szTempString_38[ 33 ]; 
   zCHAR     szTempString_39[ 33 ]; 
   zCHAR     szTempString_40[ 33 ]; 
   zCHAR     szTempString_41[ 33 ]; 
   zCHAR     szTempString_42[ 33 ]; 
   zCHAR     szTempString_43[ 33 ]; 
   zCHAR     szTempString_44[ 33 ]; 
   zCHAR     szTempString_45[ 33 ]; 
   zCHAR     szTempString_46[ 33 ]; 
   zCHAR     szTempString_47[ 33 ]; 
   zCHAR     szTempString_48[ 33 ]; 
   zCHAR     szTempString_49[ 33 ]; 
   zCHAR     szTempString_50[ 33 ]; 
   zCHAR     szTempString_51[ 33 ]; 


   //:InsertBlankFlag = "N"
   ZeidonStringCopy( InsertBlankFlag, 1, 0, "N", 1, 0, 2 );

   //://TraceLineS("*** GenJSPJ_Action *** ", "")

   //:// KJS 12/16/16 - I switched "WL_QC( vDialog" to "WL_QC( vDialogRoot".
   //:// The reason is because sometimes we call this operation when looking at resusable actions from a different dialog.
   //:// If so, vDialog (gotten from ActivateMetaOI_ByZKey) is not always associated with the open file that we are generating to. Because of this, the actions don't
   //:// get created. So changing to vDialogRoot which was build off of the original vDialog.

   //:szWebRedirection = ""
   ZeidonStringCopy( szWebRedirection, 1, 0, "", 1, 0, 2 );
   //:GetViewByName( vLPLR, "TaskLPLR", vDialog, zLEVEL_TASK )
   GetViewByName( &vLPLR, "TaskLPLR", vDialog, zLEVEL_TASK );
   //:szSystemIniApplName = "[App." + vLPLR.LPLR.Name + "]"
   GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vLPLR, "LPLR", "Name", "", 0 );
   ZeidonStringCopy( szSystemIniApplName, 1, 0, "[App.", 1, 0, 65 );
   ZeidonStringConcat( szSystemIniApplName, 1, 0, szTempString_0, 1, 0, 65 );
   ZeidonStringConcat( szSystemIniApplName, 1, 0, "]", 1, 0, 65 );
   //:SysReadZeidonIni( -1, szSystemIniApplName, "WebRedirection", szWebRedirection )
   SysReadZeidonIni( -1, szSystemIniApplName, "WebRedirection", szWebRedirection, zsizeof( szWebRedirection ) );

   //:szWriteBuffer = "   while ( bDone == false && StringUtils.equals( strActionToProcess, ^" + szActionName + "^ ) )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   while ( bDone == false && StringUtils.equals( strActionToProcess, ^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ) )", 1, 0, 10001 );
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "      bDone = true;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      bDone = true;", 1, 0, 10001 );
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "      VmlOperation.SetZeidonSessionAttribute( session, task, ^" + szFormName + "^, strActionToProcess );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      VmlOperation.SetZeidonSessionAttribute( session, task, ^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szFormName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^, strActionToProcess );", 1, 0, 10001 );
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

   //:szDialogClassCreated = "N"
   ZeidonStringCopy( szDialogClassCreated, 1, 0, "N", 1, 0, 2 );
   //:szDlgTagRoot = vDialogRoot.Dialog.Tag
   GetVariableFromAttribute( szDlgTagRoot, 0, 'S', 65, vDialogRoot, "Dialog", "Tag", "", 0 );
   //:szWndTagRoot = vDialogRoot.Window.Tag
   GetVariableFromAttribute( szWndTagRoot, 0, 'S', 101, vDialogRoot, "Window", "Tag", "", 0 );
   //:szJavaScript = vDialog.Action.WebJavaScript
   GetVariableFromAttribute( szJavaScript, 0, 'S', 10001, vDialog, "Action", "WebJavaScript", "", 0 );
   //:szJavaScriptLocation = vDialog.Action.WebJavaScriptLocation
   GetVariableFromAttribute( szJavaScriptLocation, 0, 'S', 33, vDialog, "Action", "WebJavaScriptLocation", "", 0 );

   //:IF szJavaScript != "" AND szJavaScriptLocation = "Pre-Mapping"
   if ( ZeidonStringCompare( szJavaScript, 1, 0, "", 1, 0, 10001 ) != 0 && ZeidonStringCompare( szJavaScriptLocation, 1, 0, "Pre-Mapping", 1, 0, 33 ) == 0 )
   { 
      //:szWriteBuffer = "      " + szJavaScript + "  // " + szJavaScriptLocation
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScript, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "  // ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScriptLocation, 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
   } 

   //:END

   //://**********************************
   //:szProductionMode = "" // this was something I tried but not using here...
   ZeidonStringCopy( szProductionMode, 1, 0, "", 1, 0, 2 );

   //:// KJS 11/05/09
   //:// If we unregister zeidon at the end of building this window, then when the user clicks an action,
   //:// we need to call any Prebuild/Posbuild operations (that would build objects used on this page) before we
   //:// call the action selected.
   //:IF vDialogRoot.Window.WEB_UnregisterZeidonWindow = "Y"
   if ( CompareAttributeToString( vDialogRoot, "Window", "WEB_UnregisterZeidonWindow", "Y" ) == 0 )
   { 
      //:CreateViewFromViewForTask( vDialogTemp, vDialogRoot, 0 )
      CreateViewFromViewForTask( &vDialogTemp, vDialogRoot, 0 );
      //:szCommentAdded = "N"
      ZeidonStringCopy( szCommentAdded, 1, 0, "N", 1, 0, 2 );
      //:FOR EACH vDialogTemp.Action
      RESULT = SetCursorFirstEntity( vDialogTemp, "Action", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:FOR EACH vDialogTemp.ActWndEvent
         RESULT = SetCursorFirstEntity( vDialogTemp, "ActWndEvent", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:IF vDialogTemp.ActWndEvent.Type = 1 OR vDialogTemp.ActWndEvent.Type = 2
            if ( CompareAttributeToInteger( vDialogTemp, "ActWndEvent", "Type", 1 ) == 0 || CompareAttributeToInteger( vDialogTemp, "ActWndEvent", "Type", 2 ) == 0 )
            { 

               //:IF vDialogTemp.ActOper EXISTS
               lTempInteger_0 = CheckExistenceOfEntity( vDialogTemp, "ActOper" );
               if ( lTempInteger_0 == 0 )
               { 

                  //:// Don't want to add this comment twice if there is an operation for both prebuild and postbuild.
                  //:IF szCommentAdded = "N"
                  if ( ZeidonStringCompare( szCommentAdded, 1, 0, "N", 1, 0, 2 ) == 0 )
                  { 
                     //:szWriteBuffer = "      // Prebuild/Postbuild Operations."
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Prebuild/Postbuild Operations.", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      // These are called because we Unregister Zeidon when this page is finished loading, so"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      // These are called because we Unregister Zeidon when this page is finished loading, so", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      // these operations need to be called before any action code (for recreating objects etc)."
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      // these operations need to be called before any action code (for recreating objects etc).", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szCommentAdded = "Y"
                     ZeidonStringCopy( szCommentAdded, 1, 0, "Y", 1, 0, 2 );
                  } 

                  //:END

                  //:// In order to call a dialog operation we need to create an instance of the dialog class.  We only want to do this once.
                  //:IF szDialogClassCreated = "N"
                  if ( ZeidonStringCompare( szDialogClassCreated, 1, 0, "N", 1, 0, 2 ) == 0 )
                  { 
                     //:IF szDlgTagRoot != vDialogTemp.Dialog.Tag
                     if ( CompareAttributeToString( vDialogTemp, "Dialog", "Tag", szDlgTagRoot ) != 0 )
                     { 
                        //:szWriteBuffer = "      " +  vDialogTemp.Dialog.Tag + "_Dialog " + vDialogTemp.Dialog.Tag + " = new " +
                        //:                vDialogTemp.Dialog.Tag + "_Dialog( vKZXMLPGO );"
                        GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialogTemp, "Dialog", "Tag", "", 0 );
                        ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "_Dialog ", 1, 0, 10001 );
                        GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialogTemp, "Dialog", "Tag", "", 0 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, " = new ", 1, 0, 10001 );
                        GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialogTemp, "Dialog", "Tag", "", 0 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "_Dialog( vKZXMLPGO );", 1, 0, 10001 );
                        //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                        WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

                        //:szDialogClassCreated = "Y"
                        ZeidonStringCopy( szDialogClassCreated, 1, 0, "Y", 1, 0, 2 );
                     } 

                     //:END
                  } 

                  //:END

                  //:// KJS 08/06/14 - I had put a try/catch around the operation call because it seemed better than getting the exception
                  //:// crash. But that might be true in production but when developing, we'd really like to see the stack trace. Want to make
                  //:// this an ini setting but for now I think I am just going to make the operation call and skip the try/catch.
                  //:szProductionMode = ""
                  ZeidonStringCopy( szProductionMode, 1, 0, "", 1, 0, 2 );
                  //:IF szProductionMode = "Y"
                  if ( ZeidonStringCompare( szProductionMode, 1, 0, "Y", 1, 0, 2 ) == 0 )
                  { 

                     //:szWriteBuffer = "      try"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      try", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      {"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "         nOptRC = " + vDialogTemp.Dialog.Tag + "." + vDialogTemp.ActOper.Name + "( new zVIEW( vKZXMLPGO ) );"
                     GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialogTemp, "Dialog", "Tag", "", 0 );
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         nOptRC = ", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
                     GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialogTemp, "ActOper", "Name", "", 0 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, "( new zVIEW( vKZXMLPGO ) );", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      }"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      catch (Exception e)"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      catch (Exception e)", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      {"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "         // Set the error return code."
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Set the error return code.", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "         nOptRC = 2;"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         nOptRC = 2;", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "         strVMLError = ^<br><br>*** Error running Operation " + vDialogTemp.ActOper.Name + ", Please contact the helpdesk: ^ + e.getMessage();"     
                     GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialogTemp, "ActOper", "Name", "", 0 );
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         strVMLError = ^<br><br>*** Error running Operation ", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, ", Please contact the helpdesk: ^ + e.getMessage();", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "         task.log().error( strVMLError, e );"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log().error( strVMLError, e );", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      }"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

                     //://szWriteBuffer = "      nOptRC = " + vDialogTemp.Dialog.Tag + "." + vDialogTemp.ActOper.Name + "( new zVIEW( vKZXMLPGO ) );"
                     //://WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     //:szWriteBuffer = "      if ( nOptRC == 2 )"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nOptRC == 2 )", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      {"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "         nRC = 2;  // do the ^error^ redirection"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 2;  // do the ^error^ redirection", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "         session.setAttribute( ^ZeidonError^, ^Y^ );"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         session.setAttribute( ^ZeidonError^, ^Y^ );", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "         break;"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         break;", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                     //:szWriteBuffer = "      }"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:szWriteBuffer = "      nOptRC = " + vDialogTemp.Dialog.Tag + "." + vDialogTemp.ActOper.Name + "( new zVIEW( vKZXMLPGO ) );"
                     GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialogTemp, "Dialog", "Tag", "", 0 );
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      nOptRC = ", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
                     GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialogTemp, "ActOper", "Name", "", 0 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, "( new zVIEW( vKZXMLPGO ) );", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  } 

                  //:   
                  //:END
               } 

               //:END
            } 

            RESULT = SetCursorNextEntity( vDialogTemp, "ActWndEvent", "" );
            //:END
         } 

         RESULT = SetCursorNextEntity( vDialogTemp, "Action", "" );
         //:END
      } 

      //:END
      //:DropView ( vDialogTemp )
      DropView( vDialogTemp );
   } 


   //:END

   //://***********************************

   //:// Keep position on the Control that triggers the Action, if it exists.
   //:CreateViewFromView( vDialogCtrl, vDialog )
   CreateViewFromView( &vDialogCtrl, vDialog );
   //:NAME VIEW vDialogCtrl "vDialogCtrl"
   SetNameForView( vDialogCtrl, "vDialogCtrl", 0, zLEVEL_TASK );
   //:lControl = zQUAL_STRING + zPOS_FIRST + zRECURS
   lControl = zQUAL_STRING + zPOS_FIRST + zRECURS;
   //:nRC = SetEntityCursor( vDialogCtrl, "EventAct", "Tag", lControl, szActionName, "", "", 0, "Window", "" )
   nRC = SetEntityCursor( vDialogCtrl, "EventAct", "Tag", lControl, szActionName, "", "", 0, "Window", "" );
   //:IF nRC < zCURSOR_SET
   if ( nRC < zCURSOR_SET )
   { 
      //:DropView( vDialogCtrl )
      DropView( vDialogCtrl );
      //:vDialogCtrl = 0
      vDialogCtrl = 0;
   } 

   //:END

   //:lActionType = vDialog.Action.Type
   GetIntegerFromAttribute( &lActionType, vDialog, "Action", "Type" );
   //:IF lActionType = zWAB_StayOnWindow OR
   //:   lActionType = zWAB_StayOnWindowWithRefresh OR
   //:   lActionType = zWAB_StayOnWindowWebRefresh OR
   //:   lActionType = zWAB_StayOnWindowRefreshNP
   if ( lActionType == zWAB_StayOnWindow || lActionType == zWAB_StayOnWindowWithRefresh || lActionType == zWAB_StayOnWindowWebRefresh || lActionType == zWAB_StayOnWindowRefreshNP )
   { 
      //:szWAB = "zWAB_StayOnWindowWithRefresh"
      ZeidonStringCopy( szWAB, 1, 0, "zWAB_StayOnWindowWithRefresh", 1, 0, 65 );
      //:ELSE
   } 
   else
   { 
      //:IF lActionType = zWAB_StartModelessSubwindow OR
      //:   lActionType = zWAB_StartModalSubwindow OR
      //:   lActionType = zWAB_StartModalSubwindowNP
      if ( lActionType == zWAB_StartModelessSubwindow || lActionType == zWAB_StartModalSubwindow || lActionType == zWAB_StartModalSubwindowNP )
      { 
         //:szWAB = "zWAB_StartModalSubwindow"
         ZeidonStringCopy( szWAB, 1, 0, "zWAB_StartModalSubwindow", 1, 0, 65 );
         //:ELSE
      } 
      else
      { 
         //:IF lActionType = zWAB_ReplaceWindowWithModelessWindow OR
         //:   lActionType = zWAB_ReplaceWindowWithModalWindow
         if ( lActionType == zWAB_ReplaceWindowWithModelessWindow || lActionType == zWAB_ReplaceWindowWithModalWindow )
         { 
            //:szWAB = "zWAB_ReplaceWindowWithModalWindow"
            ZeidonStringCopy( szWAB, 1, 0, "zWAB_ReplaceWindowWithModalWindow", 1, 0, 65 );
            //:ELSE
         } 
         else
         { 
            //:IF lActionType = zWAB_ReturnToParent OR
            //:   lActionType = zWAB_ReturnToParentWithRefresh
            if ( lActionType == zWAB_ReturnToParent || lActionType == zWAB_ReturnToParentWithRefresh )
            { 
               //:szWAB = "zWAB_ReturnToParent"
               ZeidonStringCopy( szWAB, 1, 0, "zWAB_ReturnToParent", 1, 0, 65 );
               //:ELSE
            } 
            else
            { 
               //:IF lActionType = zWAB_StartTopWindow
               if ( lActionType == zWAB_StartTopWindow )
               { 
                  //:szWAB = "zWAB_StartTopWindow"
                  ZeidonStringCopy( szWAB, 1, 0, "zWAB_StartTopWindow", 1, 0, 65 );
                  //:ELSE
               } 
               else
               { 
                  //:IF lActionType = zWAB_ResetTopWindow
                  if ( lActionType == zWAB_ResetTopWindow )
                  { 
                     //:szWAB = "zWAB_ResetTopWindow"
                     ZeidonStringCopy( szWAB, 1, 0, "zWAB_ResetTopWindow", 1, 0, 65 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:szWAB = ""
                     ZeidonStringCopy( szWAB, 1, 0, "", 1, 0, 65 );
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

   //:IF lActionType = zWAB_StartBrowserHTML_Page
   if ( lActionType == zWAB_StartBrowserHTML_Page )
   { 
      //:// KJS 02/09/2012 - I'm not sure what this is doing... does it even work?
      //:// Causing error in JOE so I am commenting out for now.
      //:szWriteBuffer = "      strURL = task.MakeWebFileName( 0 ) + ^.html^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      strURL = task.MakeWebFileName( 0 ) + ^.html^;", 1, 0, 10001 );
      //://WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )

      //:szWriteBuffer = "      task.dropName( strURL );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.dropName( strURL );", 1, 0, 10001 );
      //://WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
      //:ELSE
   } 
   else
   { 
      //:IF lActionType = zWAB_ProcessDownloadFile
      if ( lActionType == zWAB_ProcessDownloadFile )
      { 
         //:// Download File constants
         //:szWriteBuffer = "      String sDownloadFile = null;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      String sDownloadFile = null;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      String sSourceFileNameOnly = null;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      String sSourceFileNameOnly = null;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      String sExtension = null;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      String sExtension = null;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      int l;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      int l;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      int x;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      int x;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

         //:// Download File mapping view: Mapping must be specified for download processing.
         //:IF vDialogCtrl = 0
         if ( vDialogCtrl == 0 )
         { 
            //:MessageSend( vDialog, "", "Download File Generation",
            //:                         "The Control with 'Download File' action could not be located.",
            //:                         zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
            MessageSend( vDialog, "", "Download File Generation", "The Control with 'Download File' action could not be located.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            //:ELSE
         } 
         else
         { 
            //:IF vDialogCtrl.CtrlMapER_Attribute DOES NOT EXIST
            lTempInteger_1 = CheckExistenceOfEntity( vDialogCtrl, "CtrlMapER_Attribute" );
            if ( lTempInteger_1 != 0 )
            { 
               //:MessageSend( vDialog, "", "Download File Generation",
               //:                         "The Control with 'Download File' action must specify mapping for the source file name.",
               //:                         zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
               MessageSend( vDialog, "", "Download File Generation", "The Control with 'Download File' action must specify mapping for the source file name.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
               //:ELSE
            } 
            else
            { 
               //:szDownloadViewName = vDialogCtrl.CtrlMapView.Name
               GetVariableFromAttribute( szDownloadViewName, 0, 'S', 101, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
               //:szWriteBuffer = "      View " + szDownloadViewName + ";"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      View ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szDownloadViewName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ";", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
            } 

            //:END
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:// We need to determine if this Action is on a Group for File Transfer to Server, because we will not
   //:// generate the call to DoInputMapping in that case.
   //:szUploadTransferFileFlag = ""
   ZeidonStringCopy( szUploadTransferFileFlag, 1, 0, "", 1, 0, 2 );
   //:IF vDialogCtrl > 0
   if ( vDialogCtrl > 0 )
   { 
      //:CreateViewFromView( vDialogTemp, vDialogCtrl )
      CreateViewFromView( &vDialogTemp, vDialogCtrl );
      //:ResetViewFromSubobject( vDialogTemp )
      ResetViewFromSubobject( vDialogTemp );
      //:SET CURSOR FIRST vDialogTemp.WebControlProperty WHERE vDialogTemp.WebControlProperty.Name = "File Transfer to Server"
      RESULT = SetCursorFirstEntityByString( vDialogTemp, "WebControlProperty", "Name", "File Transfer to Server", "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:SET CURSOR FIRST vDialogTemp.WebControlProperty WHERE vDialogTemp.WebControlProperty.Name = "File Upload with DateTime"
         RESULT = SetCursorFirstEntityByString( vDialogTemp, "WebControlProperty", "Name", "File Upload with DateTime", "" );
      } 

      //:END
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:SET CURSOR FIRST vDialogTemp.WebControlProperty WHERE vDialogTemp.WebControlProperty.Name = "File Upload w/o DateTime"
         RESULT = SetCursorFirstEntityByString( vDialogTemp, "WebControlProperty", "Name", "File Upload w/o DateTime", "" );
      } 

      //:END
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:szUploadTransferFileFlag = "Y"
         ZeidonStringCopy( szUploadTransferFileFlag, 1, 0, "Y", 1, 0, 2 );
      } 

      //:END
      //:DropView( vDialogTemp )
      DropView( vDialogTemp );
   } 

   //:END

   //:// If this Action is for an Upload to Server File Transfer, we won't do normal input mapping, but we will map in the full target file name so
   //:// that we can process it, since we may not know the suffix from the source or even the file name from the source.
   //:IF szUploadTransferFileFlag = "Y"
   if ( ZeidonStringCompare( szUploadTransferFileFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 

      //://If there is mapping on the edit ("file") control, we want to put the
      //://filename that is being uploaded into that mapping.

      //:CreateViewFromView( vDialogTemp, vDialogCtrl )
      CreateViewFromView( &vDialogTemp, vDialogCtrl );

      //:   // Aadit, wants there to be an upload file limit, so that for email attachment, the file isn't too big to bring
      //:   // down the mail server. This really needs to be an updatable field...
      //:   szWriteBuffer = "      if ( iFileLth > 10485760 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( iFileLth > 10485760 )", 1, 0, 10001 );
      //:   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         VmlOperation.CreateMessage( task, ^" + szCtrlTag + "^, ^This file is too large!^, ^This file is too large!^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         VmlOperation.CreateMessage( task, ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^This file is too large!^, ^This file is too large!^ );", 1, 0, 10001 );
      //:   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         break;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         break;", 1, 0, 10001 );
      //:   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //://I wanted to do the "SET CURSOR..." but because Control doesn't really have a parent
      //://the c code that gets generated is only looking for ControlDef under Control (not all controls) and so
      //://it is not finding it.
      //://SET CURSOR FIRST vDialogTemp.Control WHERE vDialogTemp.ControlDef.Tag = "EditBox"
      //:FOR EACH vDialogTemp.Control
      RESULT = SetCursorFirstEntity( vDialogTemp, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 

         //:IF vDialogTemp.ControlDef.Tag = "EditBox"
         if ( CompareAttributeToString( vDialogTemp, "ControlDef", "Tag", "EditBox" ) == 0 )
         { 
            //:                            //256
            //://Get the mapping
            //:SET CURSOR FIRST vDialogTemp.CtrlMapLOD_Attribute WITHIN vDialogTemp.Control
            RESULT = SetCursorFirstEntity( vDialogTemp, "CtrlMapLOD_Attribute", "Control" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:szUploadViewName = vDialogTemp.CtrlMapView.Name
               GetVariableFromAttribute( szUploadViewName, 0, 'S', 101, vDialogTemp, "CtrlMapView", "Name", "", 0 );
               //:szCtrlTag = vDialogTemp.Control.Tag
               GetVariableFromAttribute( szCtrlTag, 0, 'S', 101, vDialogTemp, "Control", "Tag", "", 0 );
               //:szWriteBuffer = "      View " + szUploadViewName + ";"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      View ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szUploadViewName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ";", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
               //:szWriteBuffer = "      " + vDialogTemp.CtrlMapView.Name + " = " +
               //:                "task.getViewByName( ^" + vDialogTemp.CtrlMapView.Name + "^ );"
               GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      if ( VmlOperation.isValid( " + vDialogTemp.CtrlMapView.Name + " ) == false )"
               GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( VmlOperation.isValid( ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         task.log( ).info( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).info( ^Invalid View: ^ + ^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      else"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      {"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         nRC = " + vDialogTemp.CtrlMapView.Name + ".cursor( ^" +
               //:                vDialogTemp.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
               GetVariableFromAttribute( szTempString_12, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_13, 0, 'S', 33, vDialogTemp, "CtrlMapRelatedEntity", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_13, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         if ( nRC >= 0 )"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         {"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );


               //:// KJS 08/06/14 - I had put a try/catch around the operation call because it seemed better than getting the exception
               //:// crash. But that might be true in production but when developing, we'd really like to see the stack trace. Want to make
               //:// this an ini setting but for now I think I am just going to make the operation call and skip the try/catch.
               //:szProductionMode = ""
               ZeidonStringCopy( szProductionMode, 1, 0, "", 1, 0, 2 );
               //:IF szProductionMode = "Y"
               if ( ZeidonStringCompare( szProductionMode, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:// try
                  //:szWriteBuffer = "            try"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            try", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "            {"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "               " +
                  //:       vDialogTemp.CtrlMapView.Name +
                  //:       ".cursor( ^" + vDialogTemp.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
                  //:       vDialogTemp.CtrlMapER_Attribute.Name + "^ ).setValue( strInputFileName, ^^ );"
                  GetVariableFromAttribute( szTempString_14, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "               ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_14, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
                  GetVariableFromAttribute( szTempString_15, 0, 'S', 33, vDialogTemp, "CtrlMapRelatedEntity", "Name", "", 0 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_15, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
                  GetVariableFromAttribute( szTempString_16, 0, 'S', 33, vDialogTemp, "CtrlMapER_Attribute", "Name", "", 0 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_16, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setValue( strInputFileName, ^^ );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "            }"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:// catch
                  //:szWriteBuffer = "            catch ( InvalidAttributeValueException e )"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            catch ( InvalidAttributeValueException e )", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "            {"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "               nRC = 2;  // do the ^error^ redirection"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "               nRC = 2;  // do the ^error^ redirection", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:// KJS 02/27/12 Specifically using strTag...
                  //:szWriteBuffer = "               VmlOperation.CreateMessage( task, ^" + szCtrlTag + "^, e.getReason( ), strInputFileName );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "               VmlOperation.CreateMessage( task, ^", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^, e.getReason( ), strInputFileName );", 1, 0, 10001 );
                  //:// szWriteBuffer = "            VmlOperation.CreateMessage( task, ^" + szCtrlTag + "^, e.getReason( ), strMapValue );"
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "               break;"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "               break;", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "            }"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:// end of try/catch

                  //:szWriteBuffer = "         }"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "      }"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:ELSE
               } 
               else
               { 
                  //:szWriteBuffer = "               " +
                  //:       vDialogTemp.CtrlMapView.Name +
                  //:       ".cursor( ^" + vDialogTemp.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
                  //:       vDialogTemp.CtrlMapER_Attribute.Name + "^ ).setValue( strInputFileName, ^^ );"
                  GetVariableFromAttribute( szTempString_17, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "               ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_17, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
                  GetVariableFromAttribute( szTempString_18, 0, 'S', 33, vDialogTemp, "CtrlMapRelatedEntity", "Name", "", 0 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_18, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
                  GetVariableFromAttribute( szTempString_19, 0, 'S', 33, vDialogTemp, "CtrlMapER_Attribute", "Name", "", 0 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_19, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setValue( strInputFileName, ^^ );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "         }"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "      }"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               } 

               //:END
            } 


            //:END
         } 

         RESULT = SetCursorNextEntity( vDialogTemp, "Control", "" );
         //:END
      } 

      //:END
      //:DropView( vDialogTemp )
      DropView( vDialogTemp );

      //:ELSE
   } 
   else
   { 
      //:IF lActionType = zWAB_StayOnWindowWebRefresh
      if ( lActionType == zWAB_StayOnWindowWebRefresh )
      { 
         //:// DKS 2011.09.15 - We want to insert any java code that the
         //:// user has entered for this particular action at this point.
         //:IF szJavaScript != "" AND szJavaScriptLocation = ""
         if ( ZeidonStringCompare( szJavaScript, 1, 0, "", 1, 0, 10001 ) != 0 && ZeidonStringCompare( szJavaScriptLocation, 1, 0, "", 1, 0, 33 ) == 0 )
         { 
            //:szWriteBuffer = "   // Javascript code entered by user."
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   // Javascript code entered by user.", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = szJavaScript
            ZeidonStringCopy( szWriteBuffer, 1, 0, szJavaScript, 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "   // END of Javascript code entered by user."
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   // END of Javascript code entered by user.", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
         } 

         //:END
      } 

      //:END


      //:// Input Mapping Option
      //:IF vDialog.Action.NoMap != "Y"
      if ( CompareAttributeToString( vDialog, "Action", "NoMap", "Y" ) != 0 )
      { 
         //:szWriteBuffer = "      // Input Mapping"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Input Mapping", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      nRC = DoInputMapping( request, session, application, false );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = DoInputMapping( request, session, application, false );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      if ( nRC < 0 )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC < 0 )", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         break;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         break;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
      } 

      //:END
   } 

   //:END

   //:// Check if this is an action for reCAPTCHA.  If so, add the code...
   //:IF lActionType = 35 OR lActionType = 55
   if ( lActionType == 35 || lActionType == 55 )
   { 
      //:// We need to get the control for this action so we can retrieve the private and public keys.
      //:CreateViewFromView( vDialogTemp, vDialog )
      CreateViewFromView( &vDialogTemp, vDialog );
      //:lControl = zQUAL_STRING + zPOS_FIRST + zRECURS
      lControl = zQUAL_STRING + zPOS_FIRST + zRECURS;
      //:szCtrlTag = "reCAPTCHA"
      ZeidonStringCopy( szCtrlTag, 1, 0, "reCAPTCHA", 1, 0, 101 );
      //:IF SetEntityCursor( vDialogTemp, "WebControlProperty", "Name", lControl,
      //:                    szCtrlTag, "", "", 0, "Window", "" ) >= zCURSOR_SET
      lTempInteger_2 = SetEntityCursor( vDialogTemp, "WebControlProperty", "Name", lControl, szCtrlTag, "", "", 0, "Window", "" );
      if ( lTempInteger_2 >= zCURSOR_SET )
      { 
         //:szPublicKey = vDialogTemp.Control.WebreCAPTCHAPublicKey
         GetVariableFromAttribute( szPublicKey, 0, 'S', 51, vDialogTemp, "Control", "WebreCAPTCHAPublicKey", "", 0 );
         //:szPrivateKey = vDialogTemp.Control.WebreCAPTCHAPrivateKey
         GetVariableFromAttribute( szPrivateKey, 0, 'S', 51, vDialogTemp, "Control", "WebreCAPTCHAPrivateKey", "", 0 );
      } 


      //:END
      //:DropView( vDialogTemp )
      DropView( vDialogTemp );

      //:// create recaptcha without <noscript> tags
      //:szWriteBuffer = "      ReCaptcha captcha = ReCaptchaFactory.newReCaptcha( ^" + szPublicKey + "^, ^" + szPrivateKey + "^, false);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ReCaptcha captcha = ReCaptchaFactory.newReCaptcha( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szPublicKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szPrivateKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^, false);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      ReCaptchaResponse response2 = captcha.checkAnswer( request.getRemoteAddr( ), request.getParameter( ^recaptcha_challenge_field^ ), request.getParameter( ^recaptcha_response_field^ ) );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ReCaptchaResponse response2 = captcha.checkAnswer( request.getRemoteAddr( ), request.getParameter( ^recaptcha_challenge_field^ ), request.getParameter( ^recaptcha_response_field^ ) );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = ""
      ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if (response2.isValid( ))"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if (response2.isValid( ))", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         task.log( ).info( ^reCAPTCHA Success^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).info( ^reCAPTCHA Success^ );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         // The user has not entered in the correct response, do not go further."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         // The user has not entered in the correct response, do not go further.", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         strErrorTitle = ^INCORRECT RECAPTCHA^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorTitle = ^INCORRECT RECAPTCHA^;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         strErrorMsg = ^You typed the reCaptcha image incorrectly.  Please try again.^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorMsg = ^You typed the reCaptcha image incorrectly.  Please try again.^;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         // No redirection, we are staying on this page."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         // No redirection, we are staying on this page.", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         nRC = 0;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 0;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         break;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         break;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
   } 

   //:END

   //://KJS 11/15/07 - If there is a button or hottext on a grid, then we should position on the correct
   //://entity whether or not there is an operation.

   //://We don't want to create the code to position on the correct
   //://list entity twice.  So keep track...
   //:szPosDataCreated = "N"
   ZeidonStringCopy( szPosDataCreated, 1, 0, "N", 1, 0, 2 );

   //://TraceLineS("*** Action Loop *** ", vDialog.Action.Tag  )
   //://Loop through each ActCtrl to see if there is a button or hot text
   //://on a grid that calls this action.  If so, then we need to
   //://get position on the correct item in the list.  This is the
   //://code to set position.  If there is more than one button/hottext
   //://in this list that calls the same action, make sure we don't
   //://create this code twice.
   //:FOR EACH vDialog.ActCtrl WITHIN vDialog.Action
   RESULT = SetCursorFirstEntity( vDialog, "ActCtrl", "Action" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:szCtrlTag = vDialog.ActCtrl.Tag
      GetVariableFromAttribute( szCtrlTag, 0, 'S', 101, vDialog, "ActCtrl", "Tag", "", 0 );
      //:CreateViewFromViewForTask( vDialogTemp, vDialogRoot, 0 )
      CreateViewFromViewForTask( &vDialogTemp, vDialogRoot, 0 );
      //:NAME VIEW vDialogTemp "vDialogTemp"
      SetNameForView( vDialogTemp, "vDialogTemp", 0, zLEVEL_TASK );

      //:// KJS 11/08/12 - Used to only have to do the following for grid but now need to do this
      //:// for any control within a repeating group that has an action...
      //:lControl = zQUAL_STRING + zPOS_FIRST + zRECURS
      lControl = zQUAL_STRING + zPOS_FIRST + zRECURS;
      //:IF SetEntityCursor( vDialogTemp, "Control", "Tag", lControl,
      //:                    szCtrlTag, "", "", 0,
      //:                    "Window", "" ) >= zCURSOR_SET 
      lTempInteger_3 = SetEntityCursor( vDialogTemp, "Control", "Tag", lControl, szCtrlTag, "", "", 0, "Window", "" );
      if ( lTempInteger_3 >= zCURSOR_SET )
      { 

         //:// Identify if this Control is inside a Grid. Or if this control is inside a repeating group (or maybe both).
         //:nRC = 0
         nRC = 0;
         //:nGridParent = 0
         nGridParent = 0;
         //:szGridScopeOI = ""
         ZeidonStringCopy( szGridScopeOI, 1, 0, "", 1, 0, 2 );
         //:szBuffer = ""
         ZeidonStringCopy( szBuffer, 1, 0, "", 1, 0, 4 );
         //:// For correct setEntityKey placement later, we need to know if this action is on a repeating group (even if it's within a grid). Because of this
         //:// we need to keep resetting the view from subobject because we could be down several groups.
         //://LOOP WHILE nRC = 0 AND ( nGridParent = 0 OR szRepeatingGroupFlag = "" )
         //:LOOP WHILE nRC = 0 AND szRepeatingGroupFlag = ""
         while ( nRC == 0 && ZeidonStringCompare( szRepeatingGroupFlag, 1, 0, "", 1, 0, 2 ) == 0 )
         { 
            //://TraceLineS("*** LOOP for szRepeatingGroup *** ", "")
            //:nRC = ResetViewFromSubobject( vDialogTemp )
            nRC = ResetViewFromSubobject( vDialogTemp );
            //:IF nRC = 0 AND vDialogTemp.ControlDef EXISTS
            lTempInteger_4 = CheckExistenceOfEntity( vDialogTemp, "ControlDef" );
            if ( nRC == 0 && lTempInteger_4 == 0 )
            { 
               //:IF vDialogTemp.ControlDef.Tag = "GroupBox"
               if ( CompareAttributeToString( vDialogTemp, "ControlDef", "Tag", "GroupBox" ) == 0 )
               { 
                  //:szRepeatingGroupFlag = ""
                  ZeidonStringCopy( szRepeatingGroupFlag, 1, 0, "", 1, 0, 2 );
                  //:IF vDialogTemp.Control.WebCtrlType = "RepeatingGroup"
                  if ( CompareAttributeToString( vDialogTemp, "Control", "WebCtrlType", "RepeatingGroup" ) == 0 )
                  { 
                     //:szRepeatingGroupFlag = "Y"
                     ZeidonStringCopy( szRepeatingGroupFlag, 1, 0, "Y", 1, 0, 2 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:SET CURSOR FIRST vDialogTemp.WebControlProperty WHERE vDialogTemp.WebControlProperty.Name = "Repeating Group"
                     RESULT = SetCursorFirstEntityByString( vDialogTemp, "WebControlProperty", "Name", "Repeating Group", "" );
                     //:IF RESULT >= zCURSOR_SET
                     if ( RESULT >= zCURSOR_SET )
                     { 
                        //:szRepeatingGroupFlag = "Y"
                        ZeidonStringCopy( szRepeatingGroupFlag, 1, 0, "Y", 1, 0, 2 );
                        //://TraceLineS("**** REPEATING GROUP Y *** ", "")
                        //:// KJS 09/23/13 - Try getting the view for this
                        //:IF vDialogTemp.CtrlMapView EXISTS AND
                        lTempInteger_5 = CheckExistenceOfEntity( vDialogTemp, "CtrlMapView" );
                        //:   vDialogTemp.CtrlMapLOD_Entity EXISTS
                        lTempInteger_6 = CheckExistenceOfEntity( vDialogTemp, "CtrlMapLOD_Entity" );
                        if ( lTempInteger_5 == 0 && lTempInteger_6 == 0 )
                        { 

                           //:szEntityNameR = vDialogTemp.CtrlMapLOD_Entity.Name
                           GetVariableFromAttribute( szEntityNameR, 0, 'S', 101, vDialogTemp, "CtrlMapLOD_Entity", "Name", "", 0 );
                        } 

                        //:END
                        //:szBuffer = "   "
                        ZeidonStringCopy( szBuffer, 1, 0, "   ", 1, 0, 4 );
                     } 

                     //:END
                  } 

                  //:END
               } 

               //:END

               //:IF vDialogTemp.ControlDef.Tag = "Grid" OR szRepeatingGroupFlag = "Y"
               if ( CompareAttributeToString( vDialogTemp, "ControlDef", "Tag", "Grid" ) == 0 || ZeidonStringCompare( szRepeatingGroupFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:// If the parent is a grid, then get the View and Entity now (we might be resetting the subobject and lose
                  //:// our position on the grid) so it can be used further down.
                  //:IF vDialogTemp.CtrlMapView EXISTS AND
                  lTempInteger_7 = CheckExistenceOfEntity( vDialogTemp, "CtrlMapView" );
                  //:   vDialogTemp.CtrlMapLOD_Entity EXISTS AND nGridParent = 0
                  lTempInteger_8 = CheckExistenceOfEntity( vDialogTemp, "CtrlMapLOD_Entity" );
                  if ( lTempInteger_7 == 0 && lTempInteger_8 == 0 && nGridParent == 0 )
                  { 
                     //:szViewName = vDialogTemp.CtrlMapView.Name
                     GetVariableFromAttribute( szViewName, 0, 'S', 101, vDialogTemp, "CtrlMapView", "Name", "", 0 );
                     //:szEntityName = vDialogTemp.CtrlMapLOD_Entity.Name
                     GetVariableFromAttribute( szEntityName, 0, 'S', 101, vDialogTemp, "CtrlMapLOD_Entity", "Name", "", 0 );
                     //:// See if there is a scoping entity.
                     //:SET CURSOR NEXT vDialogTemp.CtrlMap
                     RESULT = SetCursorNextEntity( vDialogTemp, "CtrlMap", "" );
                     //:IF RESULT >= zCURSOR_SET
                     if ( RESULT >= zCURSOR_SET )
                     { 
                        //:IF vDialogTemp.CtrlMapLOD_Entity EXISTS
                        lTempInteger_9 = CheckExistenceOfEntity( vDialogTemp, "CtrlMapLOD_Entity" );
                        if ( lTempInteger_9 == 0 )
                        { 
                           //:szScopingName = vDialogTemp.CtrlMapLOD_Entity.Name
                           GetVariableFromAttribute( szScopingName, 0, 'S', 101, vDialogTemp, "CtrlMapLOD_Entity", "Name", "", 0 );
                        } 

                        //:END
                     } 

                     //:END
                  } 

                  //:END
                  //:// The parent of the this control is a Grid, so indicate for later.
                  //:// KJS 03/18/16 - I am thinking we want to do this for a listbox (if we are changing to <ul> from <select)
                  //:IF vDialogTemp.ControlDef.Tag = "Grid" OR vDialogTemp.ControlDef.Tag = "ListBox"
                  if ( CompareAttributeToString( vDialogTemp, "ControlDef", "Tag", "Grid" ) == 0 || CompareAttributeToString( vDialogTemp, "ControlDef", "Tag", "ListBox" ) == 0 )
                  { 
                     //:nGridParent = 1
                     nGridParent = 1;
                     //:// KJS 01/13/15 - Check if the grid is using Scope OI
                     //:lSubtypeX = vDialogTemp.Control.ExtendedStyle
                     GetIntegerFromAttribute( &lSubtypeX, vDialogTemp, "Control", "ExtendedStyle" );
                     //:lSubtypeX = IsFlagSequenceSet( lSubtypeX, zSS_SCOPE_OI )
                     lSubtypeX = IsFlagSequenceSet( lSubtypeX, zSS_SCOPE_OI );
                     //:IF lSubtypeX = 0
                     if ( lSubtypeX == 0 )
                     { 
                        //:szGridScopeOI = "Y"
                        ZeidonStringCopy( szGridScopeOI, 1, 0, "Y", 1, 0, 2 );
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

         //://If we successfully reset from the subcontrols and we are on a grid.
         //:IF nGridParent = 1 OR szRepeatingGroupFlag = "Y"
         if ( nGridParent == 1 || ZeidonStringCompare( szRepeatingGroupFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 

            //://TraceLineS("*** szRepeatingGroupFlag *** ", szRepeatingGroupFlag )
            //://TraceLineI("*** nGridParent ***          ", nGridParent )
            //://TraceLineS("*** szPosDataCreated ***     ", szPosDataCreated )

            //:// Because we are going to be looking again later in the generation process at actions and whether or not
            //:// they are inside a grid or repeating group, set a flag now so we don't have to do this later.
            //:vDialog.Action.ParentIsGridOrGrp = "Y"
            SetAttributeFromString( vDialog, "Action", "ParentIsGridOrGrp", "Y" );

            //:// szPosDataCreated is just to make sure we don't create this code twice.
            //:IF szPosDataCreated = "N"
            if ( ZeidonStringCompare( szPosDataCreated, 1, 0, "N", 1, 0, 2 ) == 0 )
            { 

               //:szPosDataCreated = "Y"
               ZeidonStringCopy( szPosDataCreated, 1, 0, "Y", 1, 0, 2 );

               //://TraceLineS("*** Calling EntityKey code *** ","")

               //:szWriteBuffer = "      // Position on the entity that was selected in the grid."
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Position on the entity that was selected in the grid.", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

               //:// We have a "select button" or HotText in a list, so we need to set
               //:// up the code to set position on the proper list item.
               //://szViewName = vDialogTemp.CtrlMapView.Name
               //://szEntityName = vDialogTemp.CtrlMapLOD_Entity.Name
               //:szWriteBuffer = "      String strEntityKey = (String) request.getParameter( ^zTableRowSelect^ );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      String strEntityKey = (String) request.getParameter( ^zTableRowSelect^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //://If this action is for the download button or for the upload button, we may have already created
               //://a view for this view name.  Don't create the view unless it's for a different view name.
               //:IF  ( szViewName != szDownloadViewName AND szViewName != szUploadViewName )
               if ( ZeidonStringCompare( szViewName, 1, 0, szDownloadViewName, 1, 0, 101 ) != 0 && ZeidonStringCompare( szViewName, 1, 0, szUploadViewName, 1, 0, 101 ) != 0 )
               { 
                  //:szWriteBuffer = "      View " + szViewName + ";"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      View ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, ";", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               } 

               //:END

               //:szWriteBuffer = "      " + szViewName + " = " + "task.getViewByName( ^" + szViewName + "^ );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

               //:szWriteBuffer = "      if ( VmlOperation.isValid( " + szViewName + " ) )"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( VmlOperation.isValid( ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      {"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

               //://KJS Trace
               //:IF (lTraceLevel = 1)
               if ( lTraceLevel == 1 )
               { 
                  //:szWriteBuffer = "         task.log( ).info( ^Got " + szViewName + " View Key: ^ + strEntityKey );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).info( ^Got ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, " View Key: ^ + strEntityKey );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               } 

               //:END
               //:szWriteBuffer = "         lEKey = java.lang.Long.parseLong( strEntityKey );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         lEKey = java.lang.Long.parseLong( strEntityKey );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         csrRC = " + szViewName + ".cursor( ^" + szEntityName + "^ ).setByEntityKey( lEKey );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         csrRC = ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setByEntityKey( lEKey );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

               //:szWriteBuffer = "         if ( !csrRC.isSet() )"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( !csrRC.isSet() )", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         {"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "            boolean bFound = false;"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "            boolean bFound = false;", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:// Because this link might be inside a grid that is inside a repeating group, we need to loop through
               //:// all the entities that the repeating group loops through to see if the entity key exists. Not just
               //:// the current entity we are on.
               //:IF szRepeatingGroupFlag = "Y"
               if ( ZeidonStringCompare( szRepeatingGroupFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:szWriteBuffer = "            CursorResult csrRCRptGrp = null;"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            CursorResult csrRCRptGrp = null;", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "            csrRCRptGrp = " + szViewName + ".cursor( ^" + szEntityNameR + "^ ).setFirst( );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            csrRCRptGrp = ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityNameR, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirst( );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "            while ( csrRCRptGrp.isSet() && !bFound )"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            while ( csrRCRptGrp.isSet() && !bFound )", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "            {"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               } 

               //:END 
               //:IF nGridParent = 1 //AND szScopingName != ""
               if ( nGridParent == 1 )
               { 


                  //:// If we are using SCOPE_OI on the grid, then we want to use setFirstWithinOi.
                  //:lSubtypeX = vDialog.Control.ExtendedStyle
                  GetIntegerFromAttribute( &lSubtypeX, vDialog, "Control", "ExtendedStyle" );
                  //:lSubtypeX = IsFlagSequenceSet( lSubtypeX, zSS_SCOPE_OI )
                  lSubtypeX = IsFlagSequenceSet( lSubtypeX, zSS_SCOPE_OI );
                  //:IF szGridScopeOI = "Y"
                  if ( ZeidonStringCompare( szGridScopeOI, 1, 0, "Y", 1, 0, 2 ) == 0 )
                  { 

                     //:IF szScopingName = ""  
                     if ( ZeidonStringCompare( szScopingName, 1, 0, "", 1, 0, 101 ) == 0 )
                     { 
                        //:szWriteBuffer = szBuffer + "            csrRCk = " + szViewName + ".cursor( ^" + szEntityName + "^ ).setFirst( );"
                        ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "            csrRCk = ", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirst( );", 1, 0, 10001 );
                        //:ELSE
                     } 
                     else
                     { 
                        //:szWriteBuffer = szBuffer + "            csrRCk = " + szViewName + ".cursor( ^" + szEntityName + "^ ).setFirst(^" + szScopingName + "^ );"
                        ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "            csrRCk = ", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirst(^", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szScopingName, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
                     } 

                     //:END
                     //:ELSE
                  } 
                  else
                  { 
                     //:   szWriteBuffer = szBuffer + "            csrRCk = " + szViewName + ".cursor( ^" + szEntityName + "^ ).setFirstWithinOi( );"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, "            csrRCk = ", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirstWithinOi( );", 1, 0, 10001 );
                  } 

                  //:END
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = szBuffer + "            while ( csrRCk.isSet() && !bFound )"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "            while ( csrRCk.isSet() && !bFound )", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = szBuffer + "            {"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               } 

               //:END

               //:szWriteBuffer = szBuffer + "               lEKey = " + szViewName + ".cursor( ^" + szEntityName + "^ ).getEntityKey( );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "               lEKey = ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getEntityKey( );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = szBuffer + "               strKey = Long.toString( lEKey );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "               strKey = Long.toString( lEKey );", 1, 0, 10001 );
               //://szWriteBuffer = szBuffer + "               csrRC = " + szViewName + ".cursor( ^" + szEntityName + "^ ).setByEntityKey( lEKey );"
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //://szWriteBuffer = szBuffer + "               if ( csrRC.isSet() )"
               //:szWriteBuffer = szBuffer + "               if ( StringUtils.equals( strKey, strEntityKey ) )"
               ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "               if ( StringUtils.equals( strKey, strEntityKey ) )", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = szBuffer + "               {"
               ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "               {", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = szBuffer + "                  // Stop while loop because we have positioned on the correct entity."
               ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "                  // Stop while loop because we have positioned on the correct entity.", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = szBuffer + "                  bFound = true;"
               ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "                  bFound = true;", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = szBuffer + "               }"
               ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "               }", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:IF nGridParent = 1 //AND szScopingName != ""
               if ( nGridParent == 1 )
               { 
                  //:szWriteBuffer = szBuffer + "               else"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "               else", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = szBuffer + "                  csrRCk = " + szViewName + ".cursor( ^" + szEntityName + "^ ).setNextContinue( );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "                  csrRCk = ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setNextContinue( );", 1, 0, 10001 );
                  //://szWriteBuffer = "                  csrRCk = " + szViewName + ".cursor( ^" + szScopingName + "^ ).setNextContinue( );"
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = szBuffer + "            } // Grid"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, szBuffer, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "            } // Grid", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               } 

               //:END
               //:IF szRepeatingGroupFlag = "Y"
               if ( ZeidonStringCompare( szRepeatingGroupFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:szWriteBuffer = "               if ( !bFound )"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "               if ( !bFound )", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "                  csrRCRptGrp = " + szViewName + ".cursor( ^" + szEntityNameR + "^ ).setNextContinue( );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "                  csrRCRptGrp = ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityNameR, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setNextContinue( );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "            } // Repeating group"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            } // Repeating group", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               } 

               //:END
               //:szWriteBuffer = "         }"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

               //:szWriteBuffer = "      }"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
               //://KJS Trace
               //:IF (lTraceLevel = 1)
               if ( lTraceLevel == 1 )
               { 
                  //:szWriteBuffer = "      else"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "         task.log( ).info( ^Did not get " + szViewName + " View Key: ^ + strEntityKey );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).info( ^Did not get ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, " View Key: ^ + strEntityKey );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
               } 

               //:END
            } 

            //:END
         } 

         //:END  // IF nGridParent = 1
      } 

      //:END  // IF SetEntityCursor ...

      //:DropView( vDialogTemp )
      DropView( vDialogTemp );
      RESULT = SetCursorNextEntity( vDialog, "ActCtrl", "Action" );
   } 


   //:END  // FOR EACH vDialog.ActCtrl WITHIN vDialog.Action

   //:// Operation Option
   //:IF vDialog.ActOper EXISTS //AND lActionType != zWAB_StartJasperPDF_Page
   lTempInteger_10 = CheckExistenceOfEntity( vDialog, "ActOper" );
   if ( lTempInteger_10 == 0 )
   { 

      //:szWriteBuffer = "      // Action Operation"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Action Operation", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //://FOR EACH Dialog.ActCtrl.Tag code from about was here.

      //:szWriteBuffer = "      nRC = 0;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 0;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //://KJS Trace
      //:IF (lTraceLevel = 1)
      if ( lTraceLevel == 1 )
      { 
         //:szWriteBuffer = "      task.log( ).info( ^CallDialogOperation: " + vDialog.Dialog.Tag + "." + vDialog.ActOper.Name + "  ^ );"
         GetVariableFromAttribute( szTempString_20, 0, 'S', 33, vDialog, "Dialog", "Tag", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.log( ).info( ^CallDialogOperation: ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_20, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_21, 0, 'S', 33, vDialog, "ActOper", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_21, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "  ^ );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:// In order to call a dialog operation we need to create an instance of the dialog class.  We only want to do this once.
      //://szDlgTag = vDialog.Dialog.Tag
      //:// KJS 10/08/15 - Because this could be in a different source file... I will try referencing the source file name.
      //:szDlgTag = vDialog.SourceFileForOper.Name 
      GetVariableFromAttribute( szDlgTag, 0, 'S', 101, vDialog, "SourceFileForOper", "Name", "", 0 );
      //:// szWndTag = vDialog.Window.Tag
      //:szOperName = vDialog.ActOper.Name
      GetVariableFromAttribute( szOperName, 0, 'S', 65, vDialog, "ActOper", "Name", "", 0 );
      //:IF szDialogClassCreated = "N"
      if ( ZeidonStringCompare( szDialogClassCreated, 1, 0, "N", 1, 0, 2 ) == 0 )
      { 

         //:IF szDlgTagRoot != vDialog.Dialog.Tag
         if ( CompareAttributeToString( vDialog, "Dialog", "Tag", szDlgTagRoot ) != 0 )
         { 
            //:szWriteBuffer = "      " +  szDlgTag + "_Dialog " + szDlgTag + " = new " +  szDlgTag + "_Dialog( vKZXMLPGO );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "_Dialog ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " = new ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "_Dialog( vKZXMLPGO );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         } 

         //:// szDialogClassCreated = "Y"
         //:END
      } 

      //:END

      //:IF szJavaScript != "" AND szJavaScriptLocation = "Pre-Operation"
      if ( ZeidonStringCompare( szJavaScript, 1, 0, "", 1, 0, 10001 ) != 0 && ZeidonStringCompare( szJavaScriptLocation, 1, 0, "Pre-Operation", 1, 0, 33 ) == 0 )
      { 
         //:szWriteBuffer = "      " + szJavaScript + "  // " + szJavaScriptLocation
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScript, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "  // ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScriptLocation, 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
      } 

      //:END

      //:// szWriteBuffer = "      task.log().info( ^ZeidonOperation: " + szDlgTag + "." + szOperName + " called from " + szFormName + ".jsp^ );"  // keep us informed of where we are ...  dks 2011.11.11
      //:szWriteBuffer = "      VmlOperation.SetZeidonSessionAttribute( null, task, ^" + szFormName + "^, ^" + szDlgTag + "." + szOperName + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      VmlOperation.SetZeidonSessionAttribute( null, task, ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szFormName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szOperName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:// KJS 08/06/14 - I had put a try/catch around the operation call because it seemed better than getting the exception
      //:// crash. But that might be true in production but when developing, we'd really like to see the stack trace. Want to make
      //:// this an ini setting but for now I think I am just going to make the operation call and skip the try/catch.
      //:IF szProductionMode = "Y"
      if ( ZeidonStringCompare( szProductionMode, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:szWriteBuffer = "      try"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      try", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:IF vDialog.SourceFileForOper.LanguageType = "S"
         if ( CompareAttributeToString( vDialog, "SourceFileForOper", "LanguageType", "S" ) == 0 )
         { 
            //:szWriteBuffer = "         nOptRC = " + szDlgTag + "." + szOperName + "( new com.quinsoft.zeidon.scala.View( vKZXMLPGO ) );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nOptRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szOperName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "( new com.quinsoft.zeidon.scala.View( vKZXMLPGO ) );", 1, 0, 10001 );
            //:ELSE 
         } 
         else
         { 
            //:szWriteBuffer = "         nOptRC = " + szDlgTag + "." + szOperName + "( new zVIEW( vKZXMLPGO ) );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nOptRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szOperName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "( new zVIEW( vKZXMLPGO ) );", 1, 0, 10001 );
         } 

         //:END
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      catch (Exception e)"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      catch (Exception e)", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         // Set the error return code."
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Set the error return code.", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         nOptRC = 2;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         nOptRC = 2;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         strVMLError = ^<br><br>*** Error running Operation " + szOperName + ": ^ + e.getMessage();"     
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strVMLError = ^<br><br>*** Error running Operation ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szOperName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + e.getMessage();", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         task.log().info( strVMLError );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log().info( strVMLError );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:ELSE
      } 
      else
      { 
         //:IF vDialog.SourceFileForOper.LanguageType = "S"
         if ( CompareAttributeToString( vDialog, "SourceFileForOper", "LanguageType", "S" ) == 0 )
         { 
            //:szWriteBuffer = "      nOptRC = " + szDlgTag + "." + szOperName + "( new com.quinsoft.zeidon.scala.View( vKZXMLPGO ) );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nOptRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szOperName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "( new com.quinsoft.zeidon.scala.View( vKZXMLPGO ) );", 1, 0, 10001 );
            //:ELSE 
         } 
         else
         { 
            //:szWriteBuffer = "      nOptRC = " + szDlgTag + "." + szOperName + "( new zVIEW( vKZXMLPGO ) );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nOptRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szOperName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "( new zVIEW( vKZXMLPGO ) );", 1, 0, 10001 );
         } 

         //:END
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //://szWriteBuffer = "      nOptRC = " + szDlgTag + "." + szOperName + "( new zVIEW( vKZXMLPGO ) );"
      //://WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )

      //:// Handle any Next Window processing from Operation.
      //:szWriteBuffer = "      if ( nOptRC == 2 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nOptRC == 2 )", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         nRC = 2;  // do the ^error^ redirection"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 2;  // do the ^error^ redirection", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         session.setAttribute( ^ZeidonError^, ^Y^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         session.setAttribute( ^ZeidonError^, ^Y^ );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         break;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         break;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );

      //:IF szWebRedirection = "Y"
      if ( ZeidonStringCompare( szWebRedirection, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 

         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      else"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      if ( nOptRC == 1 )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nOptRC == 1 )", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         // Dynamic Next Window"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Dynamic Next Window", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         strNextJSP_Name = " + szDlgTagRoot + ".GetWebRedirection( vKZXMLPGO );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strNextJSP_Name = ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTagRoot, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".GetWebRedirection( vKZXMLPGO );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

         //:IF szJavaScript != "" AND szJavaScriptLocation = "Post Operation"
         if ( ZeidonStringCompare( szJavaScript, 1, 0, "", 1, 0, 10001 ) != 0 && ZeidonStringCompare( szJavaScriptLocation, 1, 0, "Post Operation", 1, 0, 33 ) == 0 )
         { 
            //:szWriteBuffer = "      " + szJavaScript + "  // " + szJavaScriptLocation
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScript, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "  // ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScriptLocation, 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
         } 

         //:END

         //:IF szWAB != ""
         if ( ZeidonStringCompare( szWAB, 1, 0, "", 1, 0, 65 ) != 0 )
         { 

            //:szWriteBuffer = "      if ( strNextJSP_Name.equals( ^^ ) )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( strNextJSP_Name.equals( ^^ ) )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         // Next Window"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Next Window", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //:// In order to call a dialog operation we need to create an instance of the dialog class.  We
            //:// only want to do this once.
            //:// IF szDialogClassCreated = "N"
            //://
            //://    szWriteBuffer = "      " +  szDlgTagRoot + "_Dialog " + vDialog.Dialog.Tag + " = new " +
            //://                    szDlgTagRoot + "_Dialog( vKZXMLPGO );"
            //://    WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            //://
            //://    szDialogClassCreated = "Y"
            //:// END

            //:szWriteBuffer = "         strNextJSP_Name = " + szDlgTagRoot + ".SetWebRedirection( vKZXMLPGO, " + szDlgTagRoot + "." + szWAB + ", ^"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strNextJSP_Name = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTagRoot, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".SetWebRedirection( vKZXMLPGO, ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTagRoot, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szWAB, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ", ^", 1, 0, 10001 );
            //:szWriteBuffer = szWriteBuffer + vDialog.Action.DialogName + "^, ^" + vDialog.Action.WindowName + "^ );"
            GetVariableFromAttribute( szTempString_22, 0, 'S', 33, vDialog, "Action", "DialogName", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_22, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_23, 0, 'S', 33, vDialog, "Action", "WindowName", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_23, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:szWriteBuffer = "      strURL = response.encodeRedirectURL( strNextJSP_Name );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      strURL = response.encodeRedirectURL( strNextJSP_Name );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      nRC = 1;  // do the redirection"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 1;  // do the redirection", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //://KJS Trace
            //:IF (lTraceLevel = 1)
            if ( lTraceLevel == 1 )
            { 
               //:szWriteBuffer = "         task.TraceLine( ^Next Window Redirect to: ^, strURL );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.TraceLine( ^Next Window Redirect to: ^, strURL );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END

            //:ELSE
         } 
         else
         { 

            //:szWriteBuffer = "      // Next Window unknown"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Next Window unknown", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
         } 


         //:END

         //:ELSE  // not using WebDirection
      } 
      else
      { 

         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

         //:IF szJavaScript != "" AND szJavaScriptLocation = "Post Operation"
         if ( ZeidonStringCompare( szJavaScript, 1, 0, "", 1, 0, 10001 ) != 0 && ZeidonStringCompare( szJavaScriptLocation, 1, 0, "Post Operation", 1, 0, 33 ) == 0 )
         { 
            //:szWriteBuffer = "      " + szJavaScript + "  // " + szJavaScriptLocation
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScript, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "  // ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScriptLocation, 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
         } 

         //:END

         //:IF lActionType = zWAB_StartBrowserPDF_Page
         if ( lActionType == zWAB_StartBrowserPDF_Page )
         { 
            //:// If we are copying to pdf, then we need to have a wait routine, otherwise, the jsp tries to open
            //:// the pdf file before it's actually created.
            //:szWriteBuffer = "      try{ "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      try{ ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         Thread.sleep(5000);"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         Thread.sleep(5000);", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }catch(InterruptedException ie){ "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }catch(InterruptedException ie){ ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         out.println( ^error^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         out.println( ^error^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      } "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      } ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
         } 

         //:END

         //:// I know that if we are starting a pdf or html window then we do not need to do this.
         //:// There are probably more like Popup windows...
         //:IF  lActionType != zWAB_StartBrowserHTML_Page AND lActionType != zWAB_StartBrowserPDF_Page AND
         //:    lActionType != zWAB_StartJasperPDF_Page AND lActionType != 49 // zWAB_StartJasperXLS_Page
         if ( lActionType != zWAB_StartBrowserHTML_Page && lActionType != zWAB_StartBrowserPDF_Page && lActionType != zWAB_StartJasperPDF_Page && lActionType != 49 )
         { 

            //:// Handle any Next Window processing from Operation.
            //:szWriteBuffer = "      // Dynamic Next Window"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Dynamic Next Window", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      nRC = vKZXMLPGO.cursor( ^NextDialogWindow^ ).checkExistenceOfEntity( ).toInt();"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = vKZXMLPGO.cursor( ^NextDialogWindow^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      if ( nRC >= 0 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         strDialogName = vKZXMLPGO.cursor( ^NextDialogWindow^ ).getAttribute( ^DialogName^ ).getString( ^^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strDialogName = vKZXMLPGO.cursor( ^NextDialogWindow^ ).getAttribute( ^DialogName^ ).getString( ^^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         strWindowName = vKZXMLPGO.cursor( ^NextDialogWindow^ ).getAttribute( ^WindowName^ ).getString( ^^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strWindowName = vKZXMLPGO.cursor( ^NextDialogWindow^ ).getAttribute( ^WindowName^ ).getString( ^^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         strNextJSP_Name = strDialogName + strWindowName + ^.jsp^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strNextJSP_Name = strDialogName + strWindowName + ^.jsp^;", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         vKZXMLPGO.cursor( ^NextDialogWindow^ ).deleteEntity( CursorPosition.NEXT );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         vKZXMLPGO.cursor( ^NextDialogWindow^ ).deleteEntity( CursorPosition.NEXT );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         strURL = response.encodeRedirectURL( strNextJSP_Name );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strURL = response.encodeRedirectURL( strNextJSP_Name );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //://KJS Trace
            //:IF (lTraceLevel = 1)
            if ( lTraceLevel == 1 )
            { 
               //:szWriteBuffer = "         task.log( ).info( ^Next Window Redirect to: ^ + strURL );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).info( ^Next Window Redirect to: ^ + strURL );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END

            //:// Handle creation of return page for Function Call StartSubwindow.
            //:szWriteBuffer = "         nRC = vKZXMLPGO.cursor( ^NextDialogWindow^ ).checkExistenceOfEntity( ).toInt();"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = vKZXMLPGO.cursor( ^NextDialogWindow^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         if ( nRC >= 0 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            strFunctionCall = vKZXMLPGO.cursor( ^NextDialogWindow^ ).getAttribute( ^FunctionCall^ ).getString( ^^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            strFunctionCall = vKZXMLPGO.cursor( ^NextDialogWindow^ ).getAttribute( ^FunctionCall^ ).getString( ^^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         else"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            strFunctionCall = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            strFunctionCall = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:szWriteBuffer = "         if ( strFunctionCall != null && StringUtils.equals( strFunctionCall, ^StartSubwindow^ ) )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( strFunctionCall != null && StringUtils.equals( strFunctionCall, ^StartSubwindow^ ) )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            vKZXMLPGO.cursor( ^PagePath^ ).createEntity( CursorPosition.NEXT );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            vKZXMLPGO.cursor( ^PagePath^ ).createEntity( CursorPosition.NEXT );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            vKZXMLPGO.cursor( ^PagePath^ ).getAttribute( ^LastPageName^ ).setValue( ^" + szFormName + "^, ^^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            vKZXMLPGO.cursor( ^PagePath^ ).getAttribute( ^LastPageName^ ).setValue( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szFormName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:szWriteBuffer = "         nRC = 1;  // do the redirection"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 1;  // do the redirection", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         break;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         break;", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:ELSE
         } 
         else
         { 

            //:IF szWAB != ""
            if ( ZeidonStringCompare( szWAB, 1, 0, "", 1, 0, 65 ) != 0 )
            { 

               //:szWriteBuffer = "      // Next Window"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Next Window", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

               //:// dks 2016.04.21 - Adding sort support ... on the action, there is still the need to add javascript code similar to the following:
               //:// document.wSLCMarketingStatement.zTableRowSelect.value = buildSortTableHtml( "mSubLC", "S_MarketingUsageOrdering", "GridMarketingUsage", ["Usage Type","Usage Name"] );
               //:szDlgTag = vDialog.Action.DialogName
               GetVariableFromAttribute( szDlgTag, 0, 'S', 101, vDialog, "Action", "DialogName", "", 0 );
               //:szWndTag = vDialog.Action.WindowName
               GetVariableFromAttribute( szWndTag, 0, 'S', 65, vDialog, "Action", "WindowName", "", 0 );

               //:IF szDlgTag = "wSystem" AND szWndTag = "DragDropSort"
               if ( ZeidonStringCompare( szDlgTag, 1, 0, "wSystem", 1, 0, 101 ) == 0 && ZeidonStringCompare( szWndTag, 1, 0, "DragDropSort", 1, 0, 65 ) == 0 )
               { 
                  //:nSystemSort = 1
                  nSystemSort = 1;
                  //:szWriteBuffer = "      // We are borrowing zTableRowSelect and this code is hardwired for the moment.  javascript code similar to the following must be added to the action:"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      // We are borrowing zTableRowSelect and this code is hardwired for the moment.  javascript code similar to the following must be added to the action:", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "      // document.wSLCMarketingStatement.zTableRowSelect.value = buildSortTableHtml( ^mSubLC^, ^S_MarketingUsageOrdering^, ^GridMarketingUsage^, [^Usage Type^,^Usage Name^] );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      // document.wSLCMarketingStatement.zTableRowSelect.value = buildSortTableHtml( ^mSubLC^, ^S_MarketingUsageOrdering^, ^GridMarketingUsage^, [^Usage Type^,^Usage Name^] );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "      wWebXA = task.getViewByName( ^wWebXfer^ );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      wWebXA = task.getViewByName( ^wWebXfer^ );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "      String strHtml = (String) request.getParameter( ^zTableRowSelect^ );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      String strHtml = (String) request.getParameter( ^zTableRowSelect^ );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "      wWebXA.cursor( ^Root^ ).getAttribute( ^HTML^ ).setValue( strHtml, ^^ );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      wWebXA.cursor( ^Root^ ).getAttribute( ^HTML^ ).setValue( strHtml, ^^ );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:szWriteBuffer = "      // We are borrowing zTableRowSelect and the code above is hardwired for the moment"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      // We are borrowing zTableRowSelect and the code above is hardwired for the moment", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
               } 

               //:// ELSE
               //://    szWriteBuffer = "      // 3 Dlg: " + szDlgTag + "  Wnd: " + szWndTag
               //://    WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
               //:END

               //:szWriteBuffer = "      strNextJSP_Name = " + szDlgTagRoot + ".SetWebRedirection( vKZXMLPGO, " + szDlgTagRoot + "." + szWAB + ", ^"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      strNextJSP_Name = ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTagRoot, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".SetWebRedirection( vKZXMLPGO, ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTagRoot, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szWAB, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ", ^", 1, 0, 10001 );
               //:szWriteBuffer = szWriteBuffer + szDlgTag + "^, ^" + szWndTag + "^ );"
               ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szWndTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      strURL = response.encodeRedirectURL( strNextJSP_Name );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      strURL = response.encodeRedirectURL( strNextJSP_Name );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      nRC = 1;  // do the redirection"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 1;  // do the redirection", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

               //://KJS Trace
               //:IF (lTraceLevel = 1)
               if ( lTraceLevel == 1 )
               { 
                  //:szWriteBuffer = "         task.TraceLine( ^Next Window Redirect to: ^, strURL );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.TraceLine( ^Next Window Redirect to: ^, strURL );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               } 

               //:END

               //:ELSE
            } 
            else
            { 

               //:szWriteBuffer = "      // Next Window unknown"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Next Window unknown", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
            } 


            //:END
         } 


         //:END   // IF  lActionType != zWAB_StartBrowserHTML_Page
      } 


      //:END

      //:ELSE
   } 
   else
   { 
      //:// No operation code

      //:IF szJavaScript != "" AND szJavaScriptLocation = "Pre-Operation"
      if ( ZeidonStringCompare( szJavaScript, 1, 0, "", 1, 0, 10001 ) != 0 && ZeidonStringCompare( szJavaScriptLocation, 1, 0, "Pre-Operation", 1, 0, 33 ) == 0 )
      { 
         //:szWriteBuffer = "      " + szJavaScript + "  // " + szJavaScriptLocation
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScript, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "  // ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScriptLocation, 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
      } 

      //:END

      //:// Add Object Functions Accept, Cancel, Create Temporal Entity and Create Temporal Subobject Version.
      //:// Also looking at the auto setNext/Prev
      //:IF vDialog.ActMap EXISTS
      lTempInteger_11 = CheckExistenceOfEntity( vDialog, "ActMap" );
      if ( lTempInteger_11 == 0 )
      { 
         //:lAutoNextPrev = vDialog.Action.SB_NextPrev
         GetIntegerFromAttribute( &lAutoNextPrev, vDialog, "Action", "SB_NextPrev" );
         //:lAutoSubAction = vDialog.Action.SB_SubAction 
         GetIntegerFromAttribute( &lAutoSubAction, vDialog, "Action", "SB_SubAction" );
      } 

      //:END
      //:IF lAutoSubAction = 1 OR    // Accept
      //:   lAutoSubAction = 2 OR    // Cancel
      //:   lAutoSubAction = 4 OR    // Delete
      //:   lAutoSubAction = 268435460 OR    // Delete with Confirmation (although we aren't doing any confirmation)
      //:   lAutoSubAction = 8 OR    // Exclude
      //:   lAutoSubAction = 268435464 OR    // Exclude with Confirmation (although we aren't doing any confirmation)
      //:   lAutoSubAction = 16 OR   // Create
      //:   lAutoSubAction = 32 OR   // Create Temporal Entity
      //:   lAutoSubAction = 64 OR   // Create Subobject Version
      //:   lAutoNextPrev = 128 OR   // Next
      //:   lAutoNextPrev = 256      // Previous
      if ( lAutoSubAction == 1 || lAutoSubAction == 2 || lAutoSubAction == 4 || lAutoSubAction == 268435460 || lAutoSubAction == 8 || lAutoSubAction == 268435464 || lAutoSubAction == 16 || lAutoSubAction == 32 || lAutoSubAction == 64 || lAutoNextPrev == 128 || lAutoNextPrev == 256 )
      { 

         //:szWriteBuffer = "      // Action Auto Object Function"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Action Auto Object Function", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      nRC = 0;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 0;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

         //:// Let's put a try/catch around the auto action.
         //:szWriteBuffer = "      try"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      try", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

         //:// Get View and Entity names from the Action.
         //:IF vDialog.ActMapView EXISTS
         lTempInteger_12 = CheckExistenceOfEntity( vDialog, "ActMapView" );
         if ( lTempInteger_12 == 0 )
         { 
            //:szViewName = vDialog.ActMapView.Name 
            GetVariableFromAttribute( szViewName, 0, 'S', 101, vDialog, "ActMapView", "Name", "", 0 );
         } 

         //:END
         //:IF vDialog.ActMapLOD_Entity EXISTS
         lTempInteger_13 = CheckExistenceOfEntity( vDialog, "ActMapLOD_Entity" );
         if ( lTempInteger_13 == 0 )
         { 
            //:szEntityName = vDialog.ActMapLOD_Entity.Name 
            GetVariableFromAttribute( szEntityName, 0, 'S', 101, vDialog, "ActMapLOD_Entity", "Name", "", 0 );
         } 

         //:END

         //:// See if a scoping entity has been specified, and if so, use it.
         //:CreateViewFromView( vDialogTemp, vDialog )
         CreateViewFromView( &vDialogTemp, vDialog );
         //:SET CURSOR NEXT vDialogTemp.ActMap
         RESULT = SetCursorNextEntity( vDialogTemp, "ActMap", "" );
         //:IF RESULT >= zCURSOR_SET AND vDialogTemp.ActMapLOD_Entity EXISTS
         lTempInteger_14 = CheckExistenceOfEntity( vDialogTemp, "ActMapLOD_Entity" );
         if ( RESULT >= zCURSOR_SET && lTempInteger_14 == 0 )
         { 
            //:szScopingName = "^" + vDialogTemp.ActMapLOD_Entity.Name + "^"
            GetVariableFromAttribute( szTempString_24, 0, 'S', 33, vDialogTemp, "ActMapLOD_Entity", "Name", "", 0 );
            ZeidonStringCopy( szScopingName, 1, 0, "^", 1, 0, 101 );
            ZeidonStringConcat( szScopingName, 1, 0, szTempString_24, 1, 0, 101 );
            ZeidonStringConcat( szScopingName, 1, 0, "^", 1, 0, 101 );
            //:ELSE
         } 
         else
         { 
            //:szScopingName = ""
            ZeidonStringCopy( szScopingName, 1, 0, "", 1, 0, 101 );
         } 

         //:END
         //:DropView(vDialogTemp)
         DropView( vDialogTemp );


         //:// We don't want the getViewByName if it was generated above with the code:
         //:// IF nGridParent = 1 OR szRepeatingGroupFlag = "Y"
         //:IF nGridParent = 1 OR szRepeatingGroupFlag = "Y"
         if ( nGridParent == 1 || ZeidonStringCompare( szRepeatingGroupFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:ELSE
         } 
         else
         { 
            //:szWriteBuffer = "      View " + szViewName + " = task.getViewByName( ^" + szViewName + "^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      View ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " = task.getViewByName( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         } 

         //:END

         //:szWriteBuffer = "      EntityCursor cursor = " + szViewName + ".cursor( ^" + szEntityName + "^ );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      EntityCursor cursor = ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szViewName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

         //:// If the user has selected the auto next/prev.
         //:IF ( lAutoNextPrev = 128 OR lAutoNextPrev = 256 ) AND lAutoSubAction = 0
         if ( ( lAutoNextPrev == 128 || lAutoNextPrev == 256 ) && lAutoSubAction == 0 )
         { 
            //:szWriteBuffer = "      if ( cursor.isNull() )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( cursor.isNull() )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         nRC = 0;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 0;", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      else"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:IF lAutoNextPrev = 128
            if ( lAutoNextPrev == 128 )
            { 
               //:szWriteBuffer = "         cursor.setNext( " + szScopingName + " );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         cursor.setNext( ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szScopingName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:ELSE 
            } 
            else
            { 
               //:szWriteBuffer = "         cursor.setPrev( " + szScopingName + " );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         cursor.setPrev( ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szScopingName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         } 

         //:END

         //:// Add Accept or Cancel statement or delete or exclude.        
         //:IF lAutoSubAction = 1 OR lAutoSubAction = 2 OR lAutoSubAction = 4 OR lAutoSubAction = 8 OR 
         //:   lAutoSubAction = 268435460 OR lAutoSubAction = 268435464 
         if ( lAutoSubAction == 1 || lAutoSubAction == 2 || lAutoSubAction == 4 || lAutoSubAction == 8 || lAutoSubAction == 268435460 || lAutoSubAction == 268435464 )
         { 
            //:szWriteBuffer = "      if ( cursor.isNull() )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( cursor.isNull() )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         nRC = 0;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 0;", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      else"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //:IF lAutoSubAction = 1 OR lAutoSubAction = 2 
            if ( lAutoSubAction == 1 || lAutoSubAction == 2 )
            { 
               //:szWriteBuffer = "         if ( cursor.isVersioned( ) )"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( cursor.isVersioned( ) )", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         {"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:   
               //:IF lAutoSubAction = 1
               if ( lAutoSubAction == 1 )
               { 
                  //:// Accept
                  //:szWriteBuffer = "            cursor.acceptSubobject( );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "            cursor.acceptSubobject( );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:ELSE
               } 
               else
               { 
                  //:IF lAutoSubAction = 2             
                  if ( lAutoSubAction == 2 )
                  { 
                     //:// Cancel
                     //:szWriteBuffer = "            cursor.cancelSubobject( );"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "            cursor.cancelSubobject( );", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  } 

                  //:END
               } 

               //:END
               //:szWriteBuffer = "         }"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:ELSE
            } 
            else
            { 
               //:IF lAutoSubAction = 4 OR lAutoSubAction = 268435460
               if ( lAutoSubAction == 4 || lAutoSubAction == 268435460 )
               { 
                  //:// Delete
                  //:szWriteBuffer = "         cursor.deleteEntity( CursorPosition.NEXT );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "         cursor.deleteEntity( CursorPosition.NEXT );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  //:ELSE
               } 
               else
               { 
                  //:IF lAutoSubAction = 8 OR lAutoSubAction = 268435464
                  if ( lAutoSubAction == 8 || lAutoSubAction == 268435464 )
                  { 
                     //:// Exclude
                     //:szWriteBuffer = "         cursor.excludeEntity( CursorPosition.NEXT );"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "         cursor.excludeEntity( CursorPosition.NEXT );", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
                  } 

                  //:END
               } 

               //:END
            } 

            //:END

            //:szWriteBuffer = "         nRC = 0;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 0;", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
            //:ELSE
         } 
         else
         { 
            //:IF lAutoSubAction = 16
            if ( lAutoSubAction == 16 )
            { 
               //:// Create Entity
               //:szWriteBuffer = "      cursor.createEntity( );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      cursor.createEntity( );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
               //:ELSE 
            } 
            else
            { 
               //:IF lAutoSubAction = 32
               if ( lAutoSubAction == 32 )
               { 
                  //:// Create Temporal Entity
                  //:szWriteBuffer = "      cursor.createTemporalEntity( );"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "      cursor.createTemporalEntity( );", 1, 0, 10001 );
                  //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
                  WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
                  //:ELSE
               } 
               else
               { 
                  //:IF lAutoSubAction = 64
                  if ( lAutoSubAction == 64 )
                  { 
                     //:// Create Subobject Version
                     //:szWriteBuffer = "      cursor.createTemporalSubobjectVersion( );"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "      cursor.createTemporalSubobjectVersion( );", 1, 0, 10001 );
                     //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
                     WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 

         //:END
         //:szWriteBuffer = "      }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      catch ( Exception e )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      catch ( Exception e )", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         nRC = 2;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 2;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         VmlOperation.CreateMessage( task, ^" + vDialog.Action.Tag + "^, e.getMessage( ), ^^ );"
         GetVariableFromAttribute( szTempString_25, 0, 'S', 33, vDialog, "Action", "Tag", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         VmlOperation.CreateMessage( task, ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_25, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^, e.getMessage( ), ^^ );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         break;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         break;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 

      //:   //szWriteBuffer = "   }  // end of auto action"
      //:   //WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )

      //:END

      //:IF szWAB != ""
      if ( ZeidonStringCompare( szWAB, 1, 0, "", 1, 0, 65 ) != 0 )
      { 
         //:IF szWebRedirection = "Y"
         if ( ZeidonStringCompare( szWebRedirection, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 

            //:szWriteBuffer = "      // Next Window"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Next Window", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:// dks 2016.04.21 - Adding sort support ... on the action, there is still the need to add javascript code similar to the following:
            //:// document.wSLCMarketingStatement.zTableRowSelect.value = buildSortTableHtml( "mSubLC", "S_MarketingUsageOrdering", "GridMarketingUsage", ["Usage Type","Usage Name"] );
            //:szDlgTag = vDialog.Action.DialogName
            GetVariableFromAttribute( szDlgTag, 0, 'S', 101, vDialog, "Action", "DialogName", "", 0 );
            //:szWndTag = vDialog.Action.WindowName
            GetVariableFromAttribute( szWndTag, 0, 'S', 65, vDialog, "Action", "WindowName", "", 0 );

            //:IF szDlgTag = "wSystem" AND szWndTag = "DragDropSort"
            if ( ZeidonStringCompare( szDlgTag, 1, 0, "wSystem", 1, 0, 101 ) == 0 && ZeidonStringCompare( szWndTag, 1, 0, "DragDropSort", 1, 0, 65 ) == 0 )
            { 
               //:nSystemSort = 1
               nSystemSort = 1;
               //:szWriteBuffer = "      // We are borrowing zTableRowSelect and this code is hardwired for the moment.  javascript code similar to the following must be added to the action:"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      // We are borrowing zTableRowSelect and this code is hardwired for the moment.  javascript code similar to the following must be added to the action:", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      // document.wSLCMarketingStatement.zTableRowSelect.value = buildSortTableHtml( ^mSubLC^, ^S_MarketingUsageOrdering^, ^GridMarketingUsage^, [^Usage Type^,^Usage Name^] );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      // document.wSLCMarketingStatement.zTableRowSelect.value = buildSortTableHtml( ^mSubLC^, ^S_MarketingUsageOrdering^, ^GridMarketingUsage^, [^Usage Type^,^Usage Name^] );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      wWebXA = task.getViewByName( ^wWebXfer^ );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      wWebXA = task.getViewByName( ^wWebXfer^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      String strHtml = (String) request.getParameter( ^zTableRowSelect^ );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      String strHtml = (String) request.getParameter( ^zTableRowSelect^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      wWebXA.cursor( ^Root^ ).getAttribute( ^HTML^ ).setValue( strHtml, ^^ );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      wWebXA.cursor( ^Root^ ).getAttribute( ^HTML^ ).setValue( strHtml, ^^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      // We are borrowing zTableRowSelect and the code above is hardwired for the moment"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      // We are borrowing zTableRowSelect and the code above is hardwired for the moment", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
            } 

            //:// ELSE
            //://    szWriteBuffer = "      // 4 Dlg: " + szDlgTag + "  Wnd: " + szWndTag
            //://    WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            //:END

            //:szWriteBuffer = "      strNextJSP_Name = " + szDlgTagRoot + ".SetWebRedirection( vKZXMLPGO, " + szDlgTagRoot + "." + szWAB + ", ^"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      strNextJSP_Name = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTagRoot, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".SetWebRedirection( vKZXMLPGO, ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTagRoot, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szWAB, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ", ^", 1, 0, 10001 );
            //:szWriteBuffer = szWriteBuffer + szDlgTag + "^, ^" + szWndTag + "^ );"
            ZeidonStringConcat( szWriteBuffer, 1, 0, szDlgTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szWndTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      strURL = response.encodeRedirectURL( strNextJSP_Name );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      strURL = response.encodeRedirectURL( strNextJSP_Name );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      nRC = 1;  // do the redirection"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 1;  // do the redirection", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //://KJS Trace
            //:IF (lTraceLevel = 1)
            if ( lTraceLevel == 1 )
            { 
               //:szWriteBuffer = "         task.TraceLine( ^Next Window Redirect to: ^, strURL );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.TraceLine( ^Next Window Redirect to: ^, strURL );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END
         } 


         //:END
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "      // Next Window unknown"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Next Window unknown", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
      } 

      //:END

      //:IF szJavaScript != "" AND szJavaScriptLocation = "Post Operation"
      if ( ZeidonStringCompare( szJavaScript, 1, 0, "", 1, 0, 10001 ) != 0 && ZeidonStringCompare( szJavaScriptLocation, 1, 0, "Post Operation", 1, 0, 33 ) == 0 )
      { 
         //:szWriteBuffer = "      " + szJavaScript + "  // " + szJavaScriptLocation
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScript, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "  // ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szJavaScriptLocation, 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
      } 

      //:END
   } 


   //:END    // IF vDialog.ActOper EXISTS ... End of calling an operation

   //:IF szWebRedirection = "Y"
   if ( ZeidonStringCompare( szWebRedirection, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 

      //:IF lActionType = zWAB_StartBrowserHTML_Page
      if ( lActionType == zWAB_StartBrowserHTML_Page )
      { 
         //:   // KJS 02/09/12 - In JOE task.setName is failing, not sure what this code is
         //:   // even doing so I am commenting this out for now...
         //:   szWriteBuffer = "      task.setName( strURL );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.setName( strURL );", 1, 0, 10001 );
         //:   //WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )

         //:   // KJS 03/05/08 - This was "nRC = 2 //do the error redirection" and I'm not sure why.
         //:   // That resubmitted the current page and I assume this was done thinking this was how
         //:   // the report page would be called but since the report popup is determined in AfterPageLoaded, we
         //:   // don't need to resubmit the page.  Just keep building this current page.
         //:   szWriteBuffer = "      nRC = 0;  // Don't need to redirect for popup window"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 0;  // Don't need to redirect for popup window", 1, 0, 10001 );
         //:   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:IF lActionType = zWAB_StartBrowserPDF_Page
      if ( lActionType == zWAB_StartBrowserPDF_Page )
      { 
         //:   // KJS 03/05/08 - This was "nRC = 2 //do the error redirection" and I'm not sure why.
         //:   // That resubmitted the current page and I assume this was done thinking this was how
         //:   // the report page would be called but since the report popup is determined in AfterPageLoaded, we
         //:   // don't need to resubmit the page.  Just keep building this current page.
         //:   szWriteBuffer = "      nRC = 0;  // Don't need to redirect for popup window"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 0;  // Don't need to redirect for popup window", 1, 0, 10001 );
         //:   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:ELSE
   } 
   else
   { 

      //:// Next Window Option
      //:IF lActionType = zWAB_StartModelessSubwindow          OR
      //:   lActionType = zWAB_StartModalSubwindow             OR
      //:   lActionType = zWAB_StartModalSubwindowNP           OR
      //:   lActionType = zWAB_ReplaceWindowWithModelessWindow OR
      //:   lActionType = zWAB_ReplaceWindowWithModalWindow    OR
      //:   lActionType = zWAB_StartTopWindow                  OR
      //:   lActionType = zWAB_ResetTopWindow                  OR
      //:   lActionType = zWAB_StartBrowserHTML_Page           OR
      //:   lActionType = zWAB_StartNewApp                     OR  //reCAPTCHA StartModalSubwindow
      //:   lActionType = zWAB_ReCAPTCHA  //reCAPTCHA ReplaceModalWindow
      if ( lActionType == zWAB_StartModelessSubwindow || lActionType == zWAB_StartModalSubwindow || lActionType == zWAB_StartModalSubwindowNP || lActionType == zWAB_ReplaceWindowWithModelessWindow || lActionType == zWAB_ReplaceWindowWithModalWindow || lActionType == zWAB_StartTopWindow || lActionType == zWAB_ResetTopWindow || lActionType == zWAB_StartBrowserHTML_Page || lActionType == zWAB_StartNewApp || lActionType == zWAB_ReCAPTCHA )
      { 
         //:IF InsertBlankFlag = "Y"
         if ( ZeidonStringCompare( InsertBlankFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:szWriteBuffer = ""
            ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:InsertBlankFlag = "N"
            ZeidonStringCopy( InsertBlankFlag, 1, 0, "N", 1, 0, 2 );
         } 

         //:END

         //:szWriteBuffer = "      // Next Window"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Next Window", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

         //:IF lActionType = zWAB_StartBrowserHTML_Page
         if ( lActionType == zWAB_StartBrowserHTML_Page )
         { 
            //:// KJS 02/09/12 - In JOE task.setName is failing, not sure what this code is
            //:// even doing so I am commenting this out for now...
            //:szWriteBuffer = "      task.setName( strURL );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.setName( strURL );", 1, 0, 10001 );
            //://WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            //:szWriteBuffer = "      strURL = response.encodeRedirectURL( strURL );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      strURL = response.encodeRedirectURL( strURL );", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 
            //:szWriteBuffer = "      strURL = response.encodeRedirectURL( ^" +
            //:             vDialog.Action.DialogName + vDialog.Action.WindowName + ".jsp^ );"
            GetVariableFromAttribute( szTempString_26, 0, 'S', 33, vDialog, "Action", "DialogName", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      strURL = response.encodeRedirectURL( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_26, 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_27, 0, 'S', 33, vDialog, "Action", "WindowName", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_27, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".jsp^ );", 1, 0, 10001 );
         } 

         //:END
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

         //://KJS Trace
         //:IF (lTraceLevel = 1)
         if ( lTraceLevel == 1 )
         { 
            //:szWriteBuffer = "      task.log( ).info( ^Next Window Redirect to: ^ + strURL );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.log( ).info( ^Next Window Redirect to: ^ + strURL );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         } 

         //:END

         //:IF lActionType = zWAB_StartModelessSubwindow OR
         //:   lActionType = zWAB_StartModalSubwindow    OR
         //:   lActionType = zWAB_StartModalSubwindowNP  OR
         //:   lActionType = zWAB_StartNewApp //reCAPTCHA StartModalSubwindow
         if ( lActionType == zWAB_StartModelessSubwindow || lActionType == zWAB_StartModalSubwindow || lActionType == zWAB_StartModalSubwindowNP || lActionType == zWAB_StartNewApp )
         { 
            //:// Next Window is subwindow. Set up current window for return.
            //:szWriteBuffer = "      vKZXMLPGO.cursor( ^PagePath^ ).createEntity( CursorPosition.NEXT );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      vKZXMLPGO.cursor( ^PagePath^ ).createEntity( CursorPosition.NEXT );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      vKZXMLPGO.cursor( ^PagePath^ ).getAttribute( ^LastPageName^ ).setValue( ^" +
            //:             szFormName + "^, ^^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      vKZXMLPGO.cursor( ^PagePath^ ).getAttribute( ^LastPageName^ ).setValue( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szFormName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         } 

         //:END

         //:IF lActionType = zWAB_StartBrowserHTML_Page
         if ( lActionType == zWAB_StartBrowserHTML_Page )
         { 
            //:// KJS 03/05/08 - This was "nRC = 2 //do the error redirection" and I'm not sure why.
            //:// That resubmitted the current page and I assume this was done thinking this was how
            //:// the report page would be called but since the report popup is determined in AfterPageLoaded, we
            //:// don't need to resubmit the page.  Just keep building this current page.
            //:szWriteBuffer = "      nRC = 0;  // Don't need to redirect for popup window"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 0;  // Don't need to redirect for popup window", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 
            //:szWriteBuffer = "      nRC = 1;  // do the redirection"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 1;  // do the redirection", 1, 0, 10001 );
         } 

         //:END
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 


      //:END  //End of Next Window Option

      //:// This Action is Startup Web Popup WITH mapping, so set zOpenPopupWindow to the next page jsp.
      //:IF ( lActionType = zWAB_StartModalWebPopup OR lActionType = zWAB_StartModelessWebPopup ) // KJS 07/11/12 AND vDialog.Action.NoMap = ""
      if ( lActionType == zWAB_StartModalWebPopup || lActionType == zWAB_StartModelessWebPopup )
      { 
         //://This gets the value for the popup window so that in AfterPageLoaded, we can get this
         //://value and pop up the window.
         //:szWriteBuffer = "      strOpenPopupWindow = request.getParameter( ^zOpenPopupWindow^ );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strOpenPopupWindow = request.getParameter( ^zOpenPopupWindow^ );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      strPopupWindowSZX = request.getParameter( ^zPopupWindowSZX^ );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strPopupWindowSZX = request.getParameter( ^zPopupWindowSZX^ );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      strPopupWindowSZY = request.getParameter( ^zPopupWindowSZY^ );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strPopupWindowSZY = request.getParameter( ^zPopupWindowSZY^ );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      nRC = 0;  // No redirection, we want to open the popup"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 0;  // No redirection, we want to open the popup", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:// Same Window Refresh option
      //:IF lActionType = zWAB_StayOnWindowWithRefresh OR lActionType = zWAB_StayOnWindowRefreshNP
      if ( lActionType == zWAB_StayOnWindowWithRefresh || lActionType == zWAB_StayOnWindowRefreshNP )
      { 
         //:szWriteBuffer = "      // Stay on Window with Refresh"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Stay on Window with Refresh", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      strURL = response.encodeRedirectURL( ^" + vDialogRoot.Dialog.Tag + vDialogRoot.Window.Tag + ".jsp^ );"
         GetVariableFromAttribute( szTempString_28, 0, 'S', 33, vDialogRoot, "Dialog", "Tag", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strURL = response.encodeRedirectURL( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_28, 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_29, 0, 'S', 33, vDialogRoot, "Window", "Tag", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_29, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".jsp^ );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      nRC = 1;  // do the redirection"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 1;  // do the redirection", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:// Return From Window Option
      //:IF lActionType = zWAB_ReturnToParent OR
      //:   lActionType = zWAB_ReturnToParentWithRefresh
      if ( lActionType == zWAB_ReturnToParent || lActionType == zWAB_ReturnToParentWithRefresh )
      { 

         //:IF InsertBlankFlag = "Y"
         if ( ZeidonStringCompare( InsertBlankFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:szWriteBuffer = ""
            ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         } 

         //:END

         //:szWriteBuffer = "      // Return to Last Window"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Return to Last Window", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      nRC = vKZXMLPGO.cursor( ^PagePath^ ).checkExistenceOfEntity( ).toInt();"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = vKZXMLPGO.cursor( ^PagePath^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      if ( nRC >= 0 )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         strLastPage = vKZXMLPGO.cursor( ^PagePath^ ).getAttribute( ^LastPageName^ ).getString( ^^ );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strLastPage = vKZXMLPGO.cursor( ^PagePath^ ).getAttribute( ^LastPageName^ ).getString( ^^ );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         vKZXMLPGO.cursor( ^PagePath^ ).deleteEntity( CursorPosition.PREV );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         vKZXMLPGO.cursor( ^PagePath^ ).deleteEntity( CursorPosition.PREV );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         strLastPage = strLastPage + ^.jsp^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strLastPage = strLastPage + ^.jsp^;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      else"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         strLastPage = ^" + szFormName + ".jsp^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strLastPage = ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szFormName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".jsp^;", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

         //:szWriteBuffer = "      strURL = response.encodeRedirectURL( strLastPage );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strURL = response.encodeRedirectURL( strLastPage );", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

         //://KJS Trace
         //:IF (lTraceLevel = 1)
         if ( lTraceLevel == 1 )
         { 
            //:szWriteBuffer = "      task.log( ).info( ^Return Redirect to: ^ + strURL );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.log( ).info( ^Return Redirect to: ^ + strURL );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         } 

         //:END
         //:// szWriteBuffer = "      return;"
         //:szWriteBuffer = "      nRC = 1;  // do the redirection"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = 1;  // do the redirection", 1, 0, 10001 );
         //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END
   } 

   //:END

   //:// Download File option.
   //:IF lActionType = zWAB_ProcessDownloadFile
   if ( lActionType == zWAB_ProcessDownloadFile )
   { 

      //:// Note that we need vDialogCtrl to be positioned on Control that triggers the Action to get the attribute mapping from that Control.
      //:// This was evaluated earlier.

      //:IF vDialogCtrl > 0
      if ( vDialogCtrl > 0 )
      { 
         //:IF vDialogCtrl.CtrlMapER_Attribute EXISTS
         lTempInteger_15 = CheckExistenceOfEntity( vDialogCtrl, "CtrlMapER_Attribute" );
         if ( lTempInteger_15 == 0 )
         { 

            //:szWriteBuffer = "  "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "  ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      // Download File Processing"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Download File Processing", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //:/*
            //:szWriteBuffer = "      sBrowser = request.getHeader( ^User-Agent^ );"
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            //:szWriteBuffer = "      boolean isFirefox = ( sBrowser != null && sBrowser.indexOf( ^Firefox/^ ) != -1 );"
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            //:szWriteBuffer = "      boolean isMSIE = ( sBrowser != null && sBrowser.indexOf( ^MSIE^ ) != -1 );"
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            //:szWriteBuffer = "      boolean isOpera = ( sBrowser != null && sBrowser.indexOf( ^Opera^ ) != -1 );"
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            //:*/

            //:szWriteBuffer = "      " + vDialogCtrl.CtrlMapView.Name + " = " + "task.getViewByName( ^" + vDialogCtrl.CtrlMapView.Name + "^ );"
            GetVariableFromAttribute( szTempString_30, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_30, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_31, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_31, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      if ( VmlOperation.isValid( " + vDialogCtrl.CtrlMapView.Name + " ) )"
            GetVariableFromAttribute( szTempString_32, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( VmlOperation.isValid( ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_32, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         nRC = " + vDialogCtrl.CtrlMapView.Name + ".cursor( ^" +
            //:                                    vDialogCtrl.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
            GetVariableFromAttribute( szTempString_33, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_33, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_34, 0, 'S', 33, vDialogCtrl, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_34, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         if ( nRC >= 0 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //:lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
            GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
            //:IF lMaxStringLth <= 0
            if ( lMaxStringLth <= 0 )
            { 
               //:szMaxStringLth = "254"
               ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
               //:ELSE
            } 
            else
            { 
               //:szMaxStringLth = lMaxStringLth
               ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
            } 

            //:END

            //:szWriteBuffer = "            sDownloadFile = " + vDialogCtrl.CtrlMapView.Name + ".cursor( ^" + vDialogCtrl.CtrlMapRelatedEntity.Name +
            //:                "^ ).getAttribute( ^" + vDialogCtrl.CtrlMapER_Attribute.Name + "^ ).getString( ^^ );"
            GetVariableFromAttribute( szTempString_35, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            sDownloadFile = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_35, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_36, 0, 'S', 33, vDialogCtrl, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_36, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_37, 0, 'S', 33, vDialogCtrl, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_37, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            " + vDialogCtrl.CtrlMapView.Name + ".cursor( ^" + vDialogCtrl.CtrlMapRelatedEntity.Name +
            //:                "^ ).getAttribute( ^" + vDialogCtrl.CtrlMapER_Attribute.Name + "^ ).setValue( ^^ );"
            GetVariableFromAttribute( szTempString_38, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_38, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_39, 0, 'S', 33, vDialogCtrl, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_39, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_40, 0, 'S', 33, vDialogCtrl, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_40, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setValue( ^^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //://KJS Trace
            //:IF (lTraceLevel = 1)
            if ( lTraceLevel == 1 )
            { 
               //:szWriteBuffer = "            task.log( ).info( ^Download File Name: ^ + sDownloadFile );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).info( ^Download File Name: ^ + sDownloadFile );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END

            //:szWriteBuffer = "         }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //://KJS Trace
            //:IF (lTraceLevel = 1)
            if ( lTraceLevel == 1 )
            { 
               //:szWriteBuffer = "         else"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "            " + vDialogCtrl.CtrlMapView.Name + ".log( ).info( ^Entity does not exist: ^, ^" +
               //:                                 vDialogCtrl.CtrlMapView.Name + "." +
               //:                                 vDialogCtrl.CtrlMapRelatedEntity.Name + "^ );"
               GetVariableFromAttribute( szTempString_41, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
               ZeidonStringCopy( szWriteBuffer, 1, 0, "            ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_41, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".log( ).info( ^Entity does not exist: ^, ^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_42, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_42, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_43, 0, 'S', 33, vDialogCtrl, "CtrlMapRelatedEntity", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_43, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END

            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //://KJS Trace
            //:IF (lTraceLevel = 1)
            if ( lTraceLevel == 1 )
            { 
               //:szWriteBuffer = "      else"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         " + vDialogCtrl.CtrlMapView.Name + ".log( ).info( ^Invalid View: ^ ^" + vDialogCtrl.CtrlMapView.Name + "^ );"
               GetVariableFromAttribute( szTempString_44, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_44, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".log( ).info( ^Invalid View: ^ ^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_45, 0, 'S', 33, vDialogCtrl, "CtrlMapView", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_45, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END

            //:szWriteBuffer = "         // Replace any quotes."
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Replace any quotes.", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:szWriteBuffer = "         sDownloadFile = sDownloadFile.replaceAll( ^\^^, ^^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         sDownloadFile = sDownloadFile.replaceAll( ^\\^^, ^^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //:szWriteBuffer = "         l = sDownloadFile.lastIndexOf( ^.^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         l = sDownloadFile.lastIndexOf( ^.^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         x = sDownloadFile.lastIndexOf( ^\\^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         x = sDownloadFile.lastIndexOf( ^\\\\^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         if ( x == -1 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( x == -1 )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            x = sDownloadFile.lastIndexOf( ^/^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            x = sDownloadFile.lastIndexOf( ^/^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:szWriteBuffer = "         // If x > l then the filename has no extension and"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // If x > l then the filename has no extension and", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         // there must be a period in the directory name."
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // there must be a period in the directory name.", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         // Right now we are not using sExtension but just"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Right now we are not using sExtension but just", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         // in case we want to change the encType I want"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // in case we want to change the encType I want", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         // to capture this."
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // to capture this.", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         if (x > l)"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if (x > l)", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            sExtension = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            sExtension = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         else"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            sExtension = sDownloadFile.substring( l+1 );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            sExtension = sDownloadFile.substring( l+1 );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         // Get the filename only without directories."
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Get the filename only without directories.", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         sSourceFileNameOnly = sDownloadFile.substring( x + 1 );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         sSourceFileNameOnly = sDownloadFile.substring( x + 1 );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:szWriteBuffer = "         File InputFile = new File( sDownloadFile );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         File InputFile = new File( sDownloadFile );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         FileInputStream fis = new FileInputStream( InputFile );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         FileInputStream fis = new FileInputStream( InputFile );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         try"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         try", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

            //://KJS Trace
            //:IF (lTraceLevel = 1)
            if ( lTraceLevel == 1 )
            { 
               //:szWriteBuffer = "            task.log( ).info( ^Download File: ^ + ^Setting outputStream and attaching file^ );"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).info( ^Download File: ^ + ^Setting outputStream and attaching file^ );", 1, 0, 10001 );
               //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END

            //:szWriteBuffer = "            String contenttype = ^application/octet-stream^;  "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            String contenttype = ^application/octet-stream^;  ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            response.setContentType(contenttype);"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            response.setContentType(contenttype);", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            response.setHeader( ^Content-Disposition^, ^attachment; filename=^ + sSourceFileNameOnly + ^;^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            response.setHeader( ^Content-Disposition^, ^attachment; filename=^ + sSourceFileNameOnly + ^;^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            ServletOutputStream outStream = response.getOutputStream( );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            ServletOutputStream outStream = response.getOutputStream( );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            int c = 0;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            int c = 0;", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            while ( (c=fis.read( )) != -1 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            while ( (c=fis.read( )) != -1 )", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "               outStream.write( c );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "               outStream.write( c );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            } "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            } ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            outStream.flush( );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            outStream.flush( );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            outStream.close( );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            outStream.close( );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         catch (Exception e) "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         catch (Exception e) ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         { "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         { ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            if ( InputFile != null && InputFile.exists( ) ) "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( InputFile != null && InputFile.exists( ) ) ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            { "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            { ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "               //TraceLine"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "               //TraceLine", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "               task.log( ).info( ^Error downloading file: ^ + sDownloadFile );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "               task.log( ).info( ^Error downloading file: ^ + sDownloadFile );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            } "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            } ", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:szWriteBuffer = "         fis.close( );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         fis.close( );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );

            //:// The end of the Action is the same as "Same Window Refresh" above.
            //:szWriteBuffer = "         // Stay on Window with Refresh"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Stay on Window with Refresh", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         strURL = response.encodeRedirectURL( ^" + vDialogRoot.Dialog.Tag + vDialogRoot.Window.Tag + ".jsp^ );"
            GetVariableFromAttribute( szTempString_46, 0, 'S', 33, vDialogRoot, "Dialog", "Tag", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strURL = response.encodeRedirectURL( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_46, 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_47, 0, 'S', 33, vDialogRoot, "Window", "Tag", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_47, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".jsp^ );", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         nRC = 1;  // do the redirection"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = 1;  // do the redirection", 1, 0, 10001 );
            //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
         } 

         //:// szWriteBuffer = "      } "
         //:// WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
         //:END
      } 

      //:END
   } 

   //:END // Download File option.

   //:// KJS 09/17/14 - Run a jasper report and open in PDF.
   //:IF lActionType = zWAB_StartJasperPDF_Page
   if ( lActionType == zWAB_StartJasperPDF_Page )
   { 
      //:szWriteBuffer = "         try"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         try", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:// KJS 09/16/14 - Right now running jasper reports is a new thing and I'm not positive we will use it for much more than
      //:// the 1098T report. We need an operation that returns a jasperPrint object, which at the moment we can only do in java (
      //:// (otherwise we would need to have a data type JasperPrint in VML), so I am going to assume that we will look for an
      //:// operation in ZGLOBAL1 and that the operation we are looking for has the same name as the Action name.
      //:szWriteBuffer = "            // At the moment, in order to call a jasper report, there needs to be an operation in"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            // At the moment, in order to call a jasper report, there needs to be an operation in", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            // ZGLOBAL1 with the same name as this action name. And it must return a JasperPrint object."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            // ZGLOBAL1 with the same name as this action name. And it must return a JasperPrint object.", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            // So the filling of the jasper report must be done there (JasperFillManager.fillReport()."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            // So the filling of the jasper report must be done there (JasperFillManager.fillReport().", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            ZGLOBAL1_Operation m_ZGLOBAL1_Operation = new ZGLOBAL1_Operation( vKZXMLPGO );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            ZGLOBAL1_Operation m_ZGLOBAL1_Operation = new ZGLOBAL1_Operation( vKZXMLPGO );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            net.sf.jasperreports.engine.JasperPrint jasperPrint = m_ZGLOBAL1_Operation." + vDialog.Action.Tag +
      //:                "( new zVIEW( vKZXMLPGO ));"
      GetVariableFromAttribute( szTempString_48, 0, 'S', 33, vDialog, "Action", "Tag", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            net.sf.jasperreports.engine.JasperPrint jasperPrint = m_ZGLOBAL1_Operation.", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_48, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "( new zVIEW( vKZXMLPGO ));", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            byte[] file = null;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            byte[] file = null;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            if (jasperPrint != null) "
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            if (jasperPrint != null) ", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               file = net.sf.jasperreports.engine.JasperExportManager.exportReportToPdf(jasperPrint);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               file = net.sf.jasperreports.engine.JasperExportManager.exportReportToPdf(jasperPrint);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               response.setContentType(^application/pdf^);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               response.setContentType(^application/pdf^);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               response.setContentLength(file.length);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               response.setContentLength(file.length);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "               // Append a datetime stamp to the filename to ensure that this is a unique name."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               // Append a datetime stamp to the filename to ensure that this is a unique name.", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               DateFormat dateFormat = new SimpleDateFormat(^yyyymmddhhmmsss^);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               DateFormat dateFormat = new SimpleDateFormat(^yyyymmddhhmmsss^);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               String strFileName = ^report^ + dateFormat.format(new java.util.Date()) + ^.pdf^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               String strFileName = ^report^ + dateFormat.format(new java.util.Date()) + ^.pdf^;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               String strParam = ^attachment; filename=\^^ + strFileName + ^\^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               String strParam = ^attachment; filename=\\^^ + strFileName + ^\\^^;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 ) 
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "               response.setHeader(^Content-disposition^, strParam);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               response.setHeader(^Content-disposition^, strParam);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
      //:        
      //:szWriteBuffer = "               response.getOutputStream().write(file);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               response.getOutputStream().write(file);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               response.getOutputStream().flush();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               response.getOutputStream().flush();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               response.getOutputStream().close();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               response.getOutputStream().close();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         catch (Exception e)"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         catch (Exception e)", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            e.printStackTrace();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            e.printStackTrace();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            // Set the error return code."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            // Set the error return code.", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            nOptRC = 2;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            nOptRC = 2;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            strVMLError = ^<br><br>*** Error running Operation " + vDialog.Action.Tag + ": ^ + e.getMessage();"
      GetVariableFromAttribute( szTempString_49, 0, 'S', 33, vDialog, "Action", "Tag", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            strVMLError = ^<br><br>*** Error running Operation ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_49, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + e.getMessage();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            task.log().info( strVMLError );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log().info( strVMLError );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
   } 


   //:END

   //:// KJS 07/26/16 - Run a jasper report and open in Excel.
   //:IF lActionType = 49 //zWAB_StartJasperXLS_Page
   if ( lActionType == 49 )
   { 
      //:szWriteBuffer = "         try"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         try", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:// KJS 09/16/14 - Right now running jasper reports is a new thing and I'm not positive we will use it for much more than
      //:// the 1098T report. We need an operation that returns a jasperPrint object, which at the moment we can only do in java (
      //:// (otherwise we would need to have a data type JasperPrint in VML), so I am going to assume that we will look for an
      //:// operation in ZGLOBAL1 and that the operation we are looking for has the same name as the Action name.
      //:szWriteBuffer = "            // At the moment, in order to call a jasper report, there needs to be an operation in"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            // At the moment, in order to call a jasper report, there needs to be an operation in", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            // ZGLOBAL1 with the same name as this action name. And it must return a JasperPrint object."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            // ZGLOBAL1 with the same name as this action name. And it must return a JasperPrint object.", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            // So the filling of the jasper report must be done there (JasperFillManager.fillReport()."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            // So the filling of the jasper report must be done there (JasperFillManager.fillReport().", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            ZGLOBAL1_Operation m_ZGLOBAL1_Operation = new ZGLOBAL1_Operation( vKZXMLPGO );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            ZGLOBAL1_Operation m_ZGLOBAL1_Operation = new ZGLOBAL1_Operation( vKZXMLPGO );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            net.sf.jasperreports.engine.JasperPrint jasperPrint = m_ZGLOBAL1_Operation." + vDialog.Action.Tag +
      //:                "( new zVIEW( vKZXMLPGO ));" 
      GetVariableFromAttribute( szTempString_50, 0, 'S', 33, vDialog, "Action", "Tag", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            net.sf.jasperreports.engine.JasperPrint jasperPrint = m_ZGLOBAL1_Operation.", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_50, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "( new zVIEW( vKZXMLPGO ));", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            if (jasperPrint != null) "
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            if (jasperPrint != null) ", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "               JRXlsxExporter exporter = new JRXlsxExporter();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               JRXlsxExporter exporter = new JRXlsxExporter();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               exporter.setParameter(JRXlsExporterParameter.JASPER_PRINT, jasperPrint);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               exporter.setParameter(JRXlsExporterParameter.JASPER_PRINT, jasperPrint);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "               // Append a datetime stamp to the filename to ensure that this is a unique name."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               // Append a datetime stamp to the filename to ensure that this is a unique name.", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               DateFormat dateFormat = new SimpleDateFormat(^yyyymmddhhmmsss^);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               DateFormat dateFormat = new SimpleDateFormat(^yyyymmddhhmmsss^);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               String strDirFileName = vKZXMLPGO.cursor(^Session^).getAttribute(^PrintFileName^).getString();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               String strDirFileName = vKZXMLPGO.cursor(^Session^).getAttribute(^PrintFileName^).getString();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               if ( strDirFileName.equals(^^) )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               if ( strDirFileName.equals(^^) )", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "                  strDirFileName = ^c:\\temp\\export^ + dateFormat.format(new java.util.Date()) + ^.xlsx^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "                  strDirFileName = ^c:\\\\temp\\\\export^ + dateFormat.format(new java.util.Date()) + ^.xlsx^;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               else", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "                  vKZXMLPGO.cursor(^Session^).getAttribute(^PrintFileName^).setValue(^^);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "                  vKZXMLPGO.cursor(^Session^).getAttribute(^PrintFileName^).setValue(^^);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
      //:    
      //:szWriteBuffer = "               int x = strDirFileName.lastIndexOf(^\\^);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               int x = strDirFileName.lastIndexOf(^\\\\^);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               // If no backslash, try forward slash"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               // If no backslash, try forward slash", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               if ( x == -1 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               if ( x == -1 )", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "                  x = strDirFileName.lastIndexOf(^/^);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "                  x = strDirFileName.lastIndexOf(^/^);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               String strFileName = strDirFileName.substring(x + 1);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               String strFileName = strDirFileName.substring(x + 1);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )      
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "               String strParam = ^attachment; filename=\^^ + strFileName + ^\^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               String strParam = ^attachment; filename=\\^^ + strFileName + ^\\^^;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //://szWriteBuffer = "               String strDirFileName = ^c:/temp/^ + strFileName;"
      //://WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )

      //:szWriteBuffer = "               exporter.setParameter(JRXlsExporterParameter.OUTPUT_FILE_NAME, strDirFileName);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               exporter.setParameter(JRXlsExporterParameter.OUTPUT_FILE_NAME, strDirFileName);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               exporter.exportReport();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               exporter.exportReport();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "               File f=new File(strDirFileName);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               File f=new File(strDirFileName);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               FileInputStream fin = new FileInputStream(f);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               FileInputStream fin = new FileInputStream(f);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               ServletOutputStream outStream = response.getOutputStream();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               ServletOutputStream outStream = response.getOutputStream();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               response.setContentType(^application/vnd.ms-excel^);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               response.setContentType(^application/vnd.ms-excel^);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               response.setHeader(^Content-Disposition^, strParam);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               response.setHeader(^Content-Disposition^, strParam);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               byte[] buffer = new byte[1024];"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               byte[] buffer = new byte[1024];", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               int n = 0;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               int n = 0;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "               while ((n = fin.read(buffer)) != -1) "
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               while ((n = fin.read(buffer)) != -1) ", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "                  outStream.write(buffer, 0, n);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "                  outStream.write(buffer, 0, n);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "                  System.out.println(buffer);"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "                  System.out.println(buffer);", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               outStream.flush();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               outStream.flush();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               fin.close();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               fin.close();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               outStream.close();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               outStream.close();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         catch (Exception e)"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         catch (Exception e)", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            e.printStackTrace();"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            e.printStackTrace();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            // Set the error return code."
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            // Set the error return code.", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            nOptRC = 2;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            nOptRC = 2;", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            strVMLError = ^<br><br>*** Error running Operation " + vDialog.Action.Tag + ": ^ + e.getMessage();"
      GetVariableFromAttribute( szTempString_51, 0, 'S', 33, vDialog, "Action", "Tag", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            strVMLError = ^<br><br>*** Error running Operation ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_51, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + e.getMessage();", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            task.log().info( strVMLError );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log().info( strVMLError );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
   } 


   //:END

   //:szWriteBuffer = "      break;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      break;", 1, 0, 10001 );
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
   //:IF vDialogCtrl > 0
   if ( vDialogCtrl > 0 )
   { 
      //:DropView( vDialogCtrl )
      DropView( vDialogCtrl );
   } 

   //:END

   //:RETURN nSystemSort
   return( nSystemSort );
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteGridRadio( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                       INTEGER          lFile,
//:                       STRING ( 10000 ) szWriteBuffer,
//:                       STRING ( 50 )    szIndent,
//:                       STRING ( 32 )    szRadioGroupName,
//:                       STRING ( 1 )     szNoPositioning,
//:                       INTEGER          lOffsetX,
//:                       INTEGER          lOffsetY,
//:                       STRING ( 100 )   szRepeatGrpKey )

//:   VIEW vDialogRoot BASED ON LOD TZWDLGSO
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteGridRadio( zVIEW     vDialog,
                       zLONG     lFile,
                       zPCHAR    szWriteBuffer,
                       zPCHAR    szIndent,
                       zPCHAR    szRadioGroupName,
                       zPCHAR    szNoPositioning,
                       zLONG     lOffsetX,
                       zLONG     lOffsetY,
                       zPCHAR    szRepeatGrpKey )
{
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCtrlTag
   zCHAR     szCtrlTag[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 500 ) szText
   zCHAR     szText[ 501 ] = { 0 }; 
   //:STRING ( 500 ) szText2
   zCHAR     szText2[ 501 ] = { 0 }; 
   //:STRING ( 256 ) szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 300 ) szAbsoluteStyle
   zCHAR     szAbsoluteStyle[ 301 ] = { 0 }; 
   //:STRING ( 5 )   szSrch
   zCHAR     szSrch[ 6 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szPosition
   zCHAR     szPosition[ 101 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 32 )  szActionName
   zCHAR     szActionName[ 33 ] = { 0 }; 
   //:STRING ( 1 )   szFirstTimeThru
   zCHAR     szFirstTimeThru[ 2 ] = { 0 }; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 
   //:INTEGER        LastY_Pos
   zLONG     LastY_Pos = 0; 
   zCHAR     szTempString_0[ 255 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_1[ 51 ]; 
   zCHAR     szTempString_2[ 51 ]; 
   zCHAR     szTempString_3[ 51 ]; 


   //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
   GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );

   //:      // Initialize the szAbsoluteStyle variable with X and Y values.

   //:   IF vDialog.Control.CSS_Class != "" 
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) != 0 )
   { 
      //:   szClass = "class=^" + vDialog.Control.CSS_Class + "^ "
      GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, "class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:   szClass = ""
      ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
   } 

   //:   END

   //:      // Initialize the szAbsoluteStyle variable with X and Y values.
   //:      //CreateAbsolStyleString( vDialog, szAbsoluteStyle, lOffsetX, lOffsetY, "" )

   //:   IF szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:   CreateNoPosStyleString( vDialog, szStyle, "")
      CreateNoPosStyleString( vDialog, szStyle, "" );
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" // No style information.
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
         //:ELSE
      } 
      else
      { 
         //:CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
         CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" );
      } 

      //:END
   } 

   //:   END

   //:         szWriteBuffer = "<td>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<td>", 1, 0, 10001 );
   //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );


   //:         szRadioGroupName = vDialog.Control.Tag
   GetVariableFromAttribute( szRadioGroupName, 0, 'S', 33, vDialog, "Control", "Tag", "", 0 );
   //:         szWriteBuffer = "<% strErrorMapValue = ^^;%>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strErrorMapValue = ^^;%>", 1, 0, 10001 );
   //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:         //szWriteBuffer = "String strgrp" + szRadioGroupName + " = ^" + szRadioGroupName + "::<%=strEntityKey%>^" + strEntityKey;"
   //:         //WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )

   //:         //szWriteBuffer = "String strtmp" + szRadioGroupName + " = ^gb" + szRadioGroupName + "::" + szRepeatGrpKey + "^ + strEntityKey;"
   //:         //WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:         //szWriteBuffer = "String strtmp" + szRadioGroupName + " = ^tbl" + szRadioGroupName + "^ + strEntityKey;"
   //:         //WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )

   //:/*
   //:         IF vDialog.CtrlMapView EXISTS
   //:            szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
   //:                            "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:            szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:            szWriteBuffer = "      task.log( ).info( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:            szWriteBuffer = "   else"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:            szWriteBuffer = "   {"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:            szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
   //:                            vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:            szWriteBuffer = "      if ( nRC >= 0 )"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:            szWriteBuffer = "         str" + szRadioGroupName + "Value = " +
   //:                            vDialog.CtrlMapView.Name + ".cursor( ^" +
   //:                            vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
   //:                            vDialog.CtrlMapER_Attribute.Name + "^ ).getString( );"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:            szWriteBuffer = "   }"
   //:            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   //:         END
   //:         */
   //:         
   //:         //szWriteBuffer = "%>"
   //:         //WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )

   //:         OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
   OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" );
   //:         SetViewToSubobject( vDialog, "CtrlCtrl" )
   SetViewToSubobject( vDialog, "CtrlCtrl" );
   //:         szIndent = szIndent + "   "
   ZeidonStringConcat( szIndent, 1, 0, "   ", 1, 0, 51 );


   //:// Going to set the "id" of this outer div to the radiogrp tag, since each individual button has an id of the button tag and the NAME of the radio group tag.
   //:// This makes hiding/disabling easier.
   //://szWriteBuffer = "<div name=^gb" + szRadioGroupName + "::<%=strEntityKey%>^ id=^gb" + szRadioGroupName + "::<%=strEntityKey%>^ " + szClass + ">"
   //:szWriteBuffer = "<div name=^gb" + szRadioGroupName + "::<%=strEntityKey%>^ id=^" + szRadioGroupName + "::<%=strEntityKey%>^ " + szClass + ">"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<div name=^gb", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^ id=^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^ ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );


   //:// KJS 09/02/13 - Adding table around radio buttons. This was already done for C jsps...
   //:LastY_Pos = 0
   LastY_Pos = 0;
   //:szFirstTimeThru = "Y"
   ZeidonStringCopy( szFirstTimeThru, 1, 0, "Y", 1, 0, 2 );
   //:IF vDialog.Control EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDialog, "Control" );
   if ( lTempInteger_0 == 0 )
   { 
      //:LastY_Pos = vDialog.Control.PSDLG_Y
      GetIntegerFromAttribute( &LastY_Pos, vDialog, "Control", "PSDLG_Y" );
   } 

   //:END

   //:szWriteBuffer = "   <table style=^margin:0px;padding:0px;^  name=^tbl" + szRadioGroupName + "::<%=strEntityKey%>^ id=^tbl" + szRadioGroupName + "::<%=strEntityKey%>^>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   <table style=^margin:0px;padding:0px;^  name=^tbl", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^ id=^tbl", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   <tr style=^border:0px^>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   <tr style=^border:0px^>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:// Process the individual Radio Buttons for a Radio Button Group.
   //:FOR EACH vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = ""
      ZeidonStringCopy( szActionName, 1, 0, "", 1, 0, 33 );
      //:IF vDialog.EventAct EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vDialog, "EventAct" );
      if ( lTempInteger_1 == 0 )
      { 
         //:szActionName = vDialog.EventAct.Tag
         GetVariableFromAttribute( szActionName, 0, 'S', 33, vDialog, "EventAct", "Tag", "", 0 );
         //:nRC = zstrcmpi( szActionName, "alt-f4" )
         nRC = zstrcmpi( szActionName, "alt-f4" );
         //:IF nRC = 0
         if ( nRC == 0 )
         { 
            //:szActionName = "AltF4"
            ZeidonStringCopy( szActionName, 1, 0, "AltF4", 1, 0, 33 );
         } 

         //:END
      } 

      //:END

      //:szCtrlTag = vDialog.Control.Tag
      GetVariableFromAttribute( szCtrlTag, 0, 'S', 257, vDialog, "Control", "Tag", "", 0 );

      //:szWriteBuffer = "<% strErrorMapValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strErrorMapValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( StringUtils.equals( str" + szRadioGroupName + ", ^" +
      //:                         vDialog.Control.RadioOrCheckboxValue + "^ ) )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( StringUtils.equals( str", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ", ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ) )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      strErrorMapValue = ^checked=\^checked\^^;  %>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      strErrorMapValue = ^checked=\\^checked\\^^;  %>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:CreateTabIndexString( vDialog, szTabIndex )
      CreateTabIndexString( vDialog, szTabIndex );

      //:// If this window is being generated with no positioning then
      //:// do not create the position in the style.
      //:IF szNoPositioning = ""
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
      { 
         //:CreateAbsolPosString( vDialog, szPosition, lOffsetX, lOffsetY )
         CreateAbsolPosString( vDialog, szPosition, lOffsetX, lOffsetY );
      } 

      //:END
      //://CreateAbsolPosString( vDialog, szPosition, 0, 0 )
      //:CreateDisabledString( vDialog, szDisabled )
      CreateDisabledString( vDialog, szDisabled );
      //:FixStyleString( szStyle, szPosition, "", "" )
      FixStyleString( szStyle, szPosition, "", "" );

      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" // No style information
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
      } 

      //:END

      //:// If LastY_Pos = -100 then this is the first time thru...
      //:IF ( ( LastY_Pos < vDialog.Control.PSDLG_Y ) )
      if ( ( CompareAttributeToInteger( vDialog, "Control", "PSDLG_Y", LastY_Pos ) > 0 ) )
      { 
         //:szWriteBuffer = "   </tr>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   </tr>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   <tr style=^border:0px^>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <tr style=^border:0px^>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:ELSE
      } 
      else
      { 
         //:// KJS 07/22/10 - Currently for relative positioning, the radio buttons are being created one right after the other
         //:// (on the same line), no matter how I paint them on the screen).  Really to look at that but since we hardly ever
         //:// use them, I am for a fast fix going to append "&nbsp&nbsp&nbsp" after each button, to give some spacing.
         //:IF szNoPositioning = "Y" AND szFirstTimeThru = "N"
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 && ZeidonStringCompare( szFirstTimeThru, 1, 0, "N", 1, 0, 2 ) == 0 )
         { 
            //:szWriteBuffer = "      <td style=^border:0px;white-space:nowrap^>&nbsp&nbsp&nbsp</td>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^border:0px;white-space:nowrap^>&nbsp&nbsp&nbsp</td>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         } 

         //:END
      } 

      //:END

      //://szWriteBuffer = "<% strtmp" + szRadioGroupName + " = ^" + szCtrlTag + "::" + szRepeatGrpKey + "^ + strEntityKey;%>"
      //://WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )

      //:szStyle = szStyle + szTabIndex
      ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );
      //:// szStyle = " style=^position:absolute;LEFT:" + szX + ";TOP:" + szY + ";^ "
      //:IF  szActionName != ""
      if ( ZeidonStringCompare( szActionName, 1, 0, "", 1, 0, 33 ) != 0 )
      { 
         //:szWriteBuffer = "      <td style=^width:15px;border:0px;^><input type=^radio^ name=^" + szRadioGroupName + "::<%=strEntityKey%>^ id=^" + szCtrlTag + "::<%=strEntityKey%>^" + 
         //:                " <%=strDisabled%> value=^" + vDialog.Control.RadioOrCheckboxValue +
         //:                "^ <%=strErrorMapValue%> " + szStyle + szDisabled + " onclick=^" + szActionName + "( this.id )^  ></td>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^width:15px;border:0px;^><input type=^radio^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " <%=strDisabled%> value=^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_2, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strErrorMapValue%> ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "( this.id )^  ></td>", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "      <td style=^width:15px;border:0px;^><input type=^radio^ name=^" + szRadioGroupName + "::<%=strEntityKey%>^ id=^" + szCtrlTag + "::<%=strEntityKey%>^" + 
         //:                " <%=strDisabled%> value=^" + vDialog.Control.RadioOrCheckboxValue +
         //:                "^ <%=strErrorMapValue%> " + szStyle + szDisabled + " ></td>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^width:15px;border:0px;^><input type=^radio^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " <%=strDisabled%> value=^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_3, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strErrorMapValue%> ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " ></td>", 1, 0, 10001 );
      } 

      //:END

      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:IF szNoPositioning = "Y"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:CreateNoPosStyleString( vDialog, szStyle, "")
         CreateNoPosStyleString( vDialog, szStyle, "" );
         //:ELSE
      } 
      else
      { 
         //:IF szNoPositioning = "S"
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
         { 
            //:szStyle = "" // No style information.
            ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
            //:ELSE
         } 
         else
         { 
            //:CreateAbsolStyleString( vDialog, szStyle, 30 + lOffsetX, lOffsetY, "" )
            CreateAbsolStyleString( vDialog, szStyle, 30 + lOffsetX, lOffsetY, "" );
         } 

         //:END
      } 

      //:END

      //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
      //:szTitleHTML = ""
      ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
      //:szTitle = vDialog.Control.DIL_Text
      GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
      //:IF szTitle != ""
      if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
      { 
         //:szTitleHTML = " title=^" + szTitle + "^ "
         ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
         ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
         ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
      } 

      //:END

      //:// KJS 09/30/16 - Language Conversion.
      //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
      //:// So for now, I am always using, if the flag is set.
      //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:szText2 = vDialog.Control.Text
         GetVariableFromAttribute( szText2, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
         //:szTextID = vDialog.Control.LangID
         GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
         //:zSearchAndReplace( szText2, 1000, "\", "\\" )
         zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
         //:szSrch = "\" + QUOTES 
         ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
         ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
         //:zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
         zSearchAndReplace( szText2, 1000, QUOTES, szSrch );
         //:szSrch = ""
         ZeidonStringCopy( szSrch, 1, 0, "", 1, 0, 6 );
         //:IF szTextID = ""
         if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
            //:ELSE 
         } 
         else
         { 
            //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
         } 

         //:END
         //://szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
         //:ELSE
      } 
      else
      { 
         //:szText = vDialog.Control.Text
         GetVariableFromAttribute( szText, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
      } 

      //:END

      //:szClass = vDialog.Control.CSS_Class
      GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
      //:IF szClass = ""
      if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
      { 
         //:szWriteBuffer = "      <td style=^border:0px;white-space:nowrap^><span " + szTitleHTML + szStyle + ">" + szText + "</span></td>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^border:0px;white-space:nowrap^><span ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</span></td>", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "      <td style=^border:0px;white-space:nowrap^><span class=^" + szClass + "^ " + szTitleHTML + szStyle + ">" + szText + "</span></td>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^border:0px;white-space:nowrap^><span class=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</span></td>", 1, 0, 10001 );
      } 

      //:END

      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:LastY_Pos = vDialog.Control.PSDLG_Y
      GetIntegerFromAttribute( &LastY_Pos, vDialog, "Control", "PSDLG_Y" );
      //:szFirstTimeThru = "N"
      ZeidonStringCopy( szFirstTimeThru, 1, 0, "N", 1, 0, 2 );
      RESULT = SetCursorNextEntity( vDialog, "Control", "" );
   } 

   //:END
   //:szWriteBuffer = "   </tr>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   </tr>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   </table>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   </table>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );



   //:         ResetViewFromSubobject( vDialog )
   ResetViewFromSubobject( vDialog );
   //:         szWriteBuffer = "</div>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</div>", 1, 0, 10001 );
   //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:         szWriteBuffer = "</td>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</td>", 1, 0, 10001 );
   //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:         szWriteBuffer = ""
   ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteFileTransferForm( VIEW vDialog,
//:                              VIEW vDialogRoot,
//:                              INTEGER lFileJSP,
//:                              STRING ( 10000 ) szWriteBuffer,
//:                              STRING ( 32 ) szIndent,
//:                              STRING ( 1 ) szNoPositioning,
//:                              INTEGER          lOffsetX,
//:                              INTEGER          lOffsetY )

//:   VIEW vDialogTemp  BASED ON LOD TZWDLGSO
zOPER_EXPORT zSHORT OPERATION
GenJSPJ_CrteFileTransferForm( zVIEW     vDialog,
                              zVIEW     vDialogRoot,
                              zLONG     lFileJSP,
                              zPCHAR    szWriteBuffer,
                              zPCHAR    szIndent,
                              zPCHAR    szNoPositioning,
                              zLONG     lOffsetX,
                              zLONG     lOffsetY )
{
   zVIEW     vDialogTemp = 0; 
   //:STRING ( 64 )    szFormName
   zCHAR     szFormName[ 65 ] = { 0 }; 
   //:STRING ( 50 )    szCSS_Class
   zCHAR     szCSS_Class[ 51 ] = { 0 }; 
   //:STRING ( 256 )   szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 32 )    szCtrlTag
   zCHAR     szCtrlTag[ 33 ] = { 0 }; 
   //:STRING ( 300 )   szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 34 )    szActionTag
   zCHAR     szActionTag[ 35 ] = { 0 }; 
   //:STRING ( 50 )    szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 500 )   szText
   zCHAR     szText[ 501 ] = { 0 }; 
   //:STRING ( 256 )   szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 5 )     szSrch
   zCHAR     szSrch[ 6 ] = { 0 }; 
   //:STRING ( 500 )   szText2
   zCHAR     szText2[ 501 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szHTML5Attr
   zCHAR     szHTML5Attr[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szPlaceholder
   zCHAR     szPlaceholder[ 257 ] = { 0 }; 
   //:STRING ( 16 )  szWidth
   zCHAR     szWidth[ 17 ] = { 0 }; 
   //:STRING ( 16 )  szHeight
   zCHAR     szHeight[ 17 ] = { 0 }; 
   //:STRING ( 1 )   szGroupBoxAbsPosFlag
   zCHAR     szGroupBoxAbsPosFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szGridGroupingFlag
   zCHAR     szGridGroupingFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szGroupIsTable
   zCHAR     szGroupIsTable[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szIncludeFileFlag
   zCHAR     szIncludeFileFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szNoHeightFlag
   zCHAR     szNoHeightFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szNoWidthFlag
   zCHAR     szNoWidthFlag[ 2 ] = { 0 }; 
   //:INTEGER          lControl
   zLONG     lControl = 0; 
   //:INTEGER          lSizeX
   zLONG     lSizeX = 0; 
   //:INTEGER          lSizeY
   zLONG     lSizeY = 0; 
   //:INTEGER          lTrace
   zLONG     lTrace = 0; 
   //:INTEGER        lHeight
   zLONG     lHeight = 0; 
   //:INTEGER        lWidth
   zLONG     lWidth = 0; 
   //:INTEGER        LastY_Pos
   zLONG     LastY_Pos = 0; 
   //:INTEGER        LastY_Height
   zLONG     LastY_Height = 0; 
   //:INTEGER        LastY_PosGroup
   zLONG     LastY_PosGroup = 0; 
   //:INTEGER        LastX_Pos
   zLONG     LastX_Pos = 0; 
   //:INTEGER        LastX_Width
   zLONG     LastX_Width = 0; 
   //:INTEGER        MaxY_HeightForLine
   zLONG     MaxY_HeightForLine = 0; 
   //:INTEGER        X_Size
   zLONG     X_Size = 0; 
   //:INTEGER        Y_Size
   zLONG     Y_Size = 0; 
   //:INTEGER        X_Pos
   zLONG     X_Pos = 0; 
   //:INTEGER        Y_Pos
   zLONG     Y_Pos = 0; 
   //:SHORT            nRC
   zSHORT    nRC = 0; 

   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:STRING ( 100 )  szSize
   zCHAR     szSize[ 101 ] = { 0 }; 
   //:STRING ( 100 )  szPosition
   zCHAR     szPosition[ 101 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zLONG     lTempInteger_3; 
   zCHAR     szTempString_11[ 255 ]; 
   zCHAR     szTempString_12[ 255 ]; 
   zCHAR     szTempString_13[ 255 ]; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zCHAR     szTempString_14[ 255 ]; 
   zSHORT    lTempInteger_7; 
   zCHAR     szTempString_15[ 33 ]; 



   //:// KJS 11/07/16 - For file transfer, since it doesn't place the controls correctly (if there are more controls than the
   //:// edit and button), we are going to try, closing the previous <form> and creating the form for the upload. Any controls
   //:// that are after the "file upload" groupbox, are going to be placed in this form. Because on the actions, we submit the
   //:// original (first) form, this doesn't seem to be a problem, but we will see...
   //:szWriteBuffer = "</form>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</form>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );

   //:// Check to see if this GroupBox has a WebControlProperty of "Table".  If so, then we will create this
   //:// as a table and try and position what is in this group box as a table.
   //:szGroupIsTable = ""
   ZeidonStringCopy( szGroupIsTable, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Table"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Table", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szGroupIsTable = "Y"
      ZeidonStringCopy( szGroupIsTable, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //:// A groupbox might want the children to be absolutely positioned.  Allow for this.
   //:szGroupBoxAbsPosFlag = ""
   ZeidonStringCopy( szGroupBoxAbsPosFlag, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Absolute Pos"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Absolute Pos", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szGroupBoxAbsPosFlag = "Y"
      ZeidonStringCopy( szGroupBoxAbsPosFlag, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END
   //:  
   //:// There are times when the user wants to paint a div that has no height so it will grow with the
   //:// data within it.  This flags the div will have no height.
   //:szNoHeightFlag = ""
   ZeidonStringCopy( szNoHeightFlag, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "No Height"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "No Height", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szNoHeightFlag = "Y"
      ZeidonStringCopy( szNoHeightFlag, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //:// There are times when the user wants to paint a div that has no width so it will grow with the
   //:// data within it.  This flags the div will have no width.
   //:szNoWidthFlag = ""
   ZeidonStringCopy( szNoWidthFlag, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "No Width"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "No Width", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szNoWidthFlag = "Y"
      ZeidonStringCopy( szNoWidthFlag, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //:PIX_PER_DU( vDialog, dDLUnits )
   PIX_PER_DU( vDialog, &dDLUnits );

   //:// FORM GENERATION FOR FILE TRANSFER GROUPS
   //:// Create Form statements for each Group of type "File Transfer to Server".
   //:// Note that each Group requires a subcontrol of type EditBox and another of type PushBtn.
   //:CreateViewFromView( vDialogTemp, vDialog )
   CreateViewFromView( &vDialogTemp, vDialog );
   //:NAME VIEW vDialogTemp "vDialogTemp"
   SetNameForView( vDialogTemp, "vDialogTemp", 0, zLEVEL_TASK );

   //:// Generate Form Statement for Group, with hidden field zAction.
   //:szWriteBuffer = ""
   ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
   //:WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
   //:szFormName = vDialogTemp.Control.Tag
   GetVariableFromAttribute( szFormName, 0, 'S', 65, vDialogTemp, "Control", "Tag", "", 0 );
   //:szWriteBuffer = "<form name=^" + szFormName + "^  id=^" + szFormName + "^ method=^post^ enctype=^multipart/form-data^>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<form name=^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szFormName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^  id=^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szFormName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ method=^post^ enctype=^multipart/form-data^>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   <input name=^zAction^ id=^zAction^ type=^hidden^ value=^HELP^>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   <input name=^zAction^ id=^zAction^ type=^hidden^ value=^HELP^>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 );

   //:   //CreateAbsolStyleString( vDialogTemp, szStyle, lSizeX, lSizeY, "" )
   //:   // Initialize the szStyle variable with X and Y values.
   //:   IF szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 

      //:   lWidth = vDialog.Control.SZDLG_X * dDLUnits
      GetIntegerFromAttribute( &lTempInteger_0, vDialog, "Control", "SZDLG_X" );
      lWidth = lTempInteger_0 * dDLUnits;
      //:   lHeight = vDialog.Control.SZDLG_Y * dDLUnits
      GetIntegerFromAttribute( &lTempInteger_1, vDialog, "Control", "SZDLG_Y" );
      lHeight = lTempInteger_1 * dDLUnits;
      //:   zIntegerToString( szWidth, 10, lWidth )
      zIntegerToString( szWidth, 10, lWidth );
      //:   zIntegerToString( szHeight, 10, lHeight )
      zIntegerToString( szHeight, 10, lHeight );

      //:   // If szGroupBoxAbsPosFlag = "Y" then we are saying that the children of this group box will
      //:   // be absolutely positioned.
      //:   IF szGroupBoxAbsPosFlag = "Y"
      if ( ZeidonStringCompare( szGroupBoxAbsPosFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 

         //:   //szNoPositioningHold = szNoPositioning
         //:   //szNoPositioning = ""  // we want absolute positioning on all child controls
         //:   
         //:   IF szNoHeightFlag = "Y" // We might not want the groupbox to have a height (so it grows)
         if ( ZeidonStringCompare( szNoHeightFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:   szStyle = "style=^float:left;position:relative; width:" + szWidth + "px;^"
            ZeidonStringCopy( szStyle, 1, 0, "style=^float:left;position:relative; width:", 1, 0, 301 );
            ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
            ZeidonStringConcat( szStyle, 1, 0, "px;^", 1, 0, 301 );
            //:ELSE
         } 
         else
         { 
            //:   //zIntegerToString( szHeight, 10, lHeight * dDLUnits )
            //:   szStyle = "style=^float:left;position:relative; width:" + szWidth + "px; height:" + szHeight + "px;^"
            ZeidonStringCopy( szStyle, 1, 0, "style=^float:left;position:relative; width:", 1, 0, 301 );
            ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
            ZeidonStringConcat( szStyle, 1, 0, "px; height:", 1, 0, 301 );
            ZeidonStringConcat( szStyle, 1, 0, szHeight, 1, 0, 301 );
            ZeidonStringConcat( szStyle, 1, 0, "px;^", 1, 0, 301 );
         } 

         //:   END                      
         //:ELSE 
      } 
      else
      { 
         //:IF szNoHeightFlag = "Y" AND szNoWidthFlag = "Y"
         if ( ZeidonStringCompare( szNoHeightFlag, 1, 0, "Y", 1, 0, 2 ) == 0 && ZeidonStringCompare( szNoWidthFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:szStyle = ""
            ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
            //:ELSE
         } 
         else
         { 
            //:IF szNoHeightFlag = "Y"
            if ( ZeidonStringCompare( szNoHeightFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
            { 
               //:// We might not want the groupbox to have a height (so it grows)
               //:szStyle = "style=^width:" + szWidth + "px;^"
               ZeidonStringCopy( szStyle, 1, 0, "style=^width:", 1, 0, 301 );
               ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
               ZeidonStringConcat( szStyle, 1, 0, "px;^", 1, 0, 301 );
               //:ELSE
            } 
            else
            { 
               //:IF szNoWidthFlag = "Y"
               if ( ZeidonStringCompare( szNoWidthFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:// We might not want the groupbox to have a width (so it grows)
                  //:szStyle = "style=^height:" + szHeight + "px;^"
                  ZeidonStringCopy( szStyle, 1, 0, "style=^height:", 1, 0, 301 );
                  ZeidonStringConcat( szStyle, 1, 0, szHeight, 1, 0, 301 );
                  ZeidonStringConcat( szStyle, 1, 0, "px;^", 1, 0, 301 );
                  //:ELSE
               } 
               else
               { 
                  //:CreateNoPosStyleString( vDialog, szStyle, "" )
                  CreateNoPosStyleString( vDialog, szStyle, "" );
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      //:   END
      //:ELSE
   } 
   else
   { 
      //:IF  szNoPositioning = "S" // We are not creating any style information.
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = ""
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
         //:ELSE
      } 
      else
      { 
         //:CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
         CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" );
      } 

      //:END
   } 

   //:   END

   //:   szClass = ""
   ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
   //:   IF vDialogTemp.Control.CSS_Class != ""
   if ( CompareAttributeToString( vDialogTemp, "Control", "CSS_Class", "" ) != 0 )
   { 
      //:   szClass = " class=^" + vDialogTemp.Control.CSS_Class + "^"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vDialogTemp, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
   } 

   //:   END
   //:   szWriteBuffer = "<div " + szStyle + szClass + ">  <!-- " + vDialogTemp.Control.Tag + " --> "
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<div ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, ">  <!-- ", 1, 0, 10001 );
   GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialogTemp, "Control", "Tag", "", 0 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " --> ", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );

   //:   // Get Group positions for adding to subcontrols.
   //:   //lSizeX = vDialogTemp.Control.PSDLG_X * dDLUnits
   //:   //lSizeY = vDialogTemp.Control.PSDLG_Y * dDLUnits

   //:   // Generate hidden field zTargetFileName, with JSP to retrieve name.
   //:   // We are currently positioned on the GroupBox and need to reposition on the subcontrol Button
   //:   // to generate the FileName mapping..
   //:   szWriteBuffer = "<%"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
   //:   szWriteBuffer = "   strErrorMapValue = ^^;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   strErrorMapValue = ^^;", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );

   //:   SetViewToSubobject( vDialogTemp, "CtrlCtrl" )
   SetViewToSubobject( vDialogTemp, "CtrlCtrl" );
   //:   FOR EACH vDialogTemp.Control
   RESULT = SetCursorFirstEntity( vDialogTemp, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:   IF vDialogTemp.ControlDef.Tag = "PushBtn"
      if ( CompareAttributeToString( vDialogTemp, "ControlDef", "Tag", "PushBtn" ) == 0 )
      { 
         //:   IF vDialogTemp.CtrlMapER_Attribute EXISTS
         lTempInteger_2 = CheckExistenceOfEntity( vDialogTemp, "CtrlMapER_Attribute" );
         if ( lTempInteger_2 == 0 )
         { 
            //:   // The mapping for the Push Button specifies the attribute that contains the Target File Name (without suffix).

            //:// szWriteBuffer = "   strSessionId = session.getId( );"
            //:// WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 )

            //:   szWriteBuffer = "   " + vDialogTemp.CtrlMapView.Name + " = " +
            //:                   "task.getViewByName( ^" + vDialogTemp.CtrlMapView.Name + "^ );"
            GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialogTemp.CtrlMapView.Name + " ) )"
            GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );

            //:   szWriteBuffer = "   {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "      nRC = " + vDialogTemp.CtrlMapView.Name + ".cursor( ^" +
            //:                   vDialogTemp.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
            GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialogTemp, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "      if ( nRC >= 0 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "         strErrorMapValue = " +
            //:                   vDialogTemp.CtrlMapView.Name + ".cursor(^" + vDialogTemp.CtrlMapRelatedEntity.Name +
            //:                   "^).getAttribute( ^" +
            //:                   vDialogTemp.CtrlMapER_Attribute.Name + "^ ).getString( ^^ );"
            GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialogTemp, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorMapValue = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor(^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialogTemp, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^).getAttribute( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialogTemp, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^^ );", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "   }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );

            //:   IF (lTrace = 1)
            if ( lTrace == 1 )
            { 
               //:   szWriteBuffer = "   else"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
               //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
               //:   szWriteBuffer = "      task.log().info( ^Invalid View: ^ + ^" + vDialogTemp.Control.Tag + "^ );"
               GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialogTemp, "Control", "Tag", "", 0 );
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.log().info( ^Invalid View: ^ + ^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
               //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            } 

            //:   END
         } 


         //:   END
      } 

      RESULT = SetCursorNextEntity( vDialogTemp, "Control", "" );
      //:   END
   } 

   //:   END
   //:   szWriteBuffer = "%>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
   //:   szWriteBuffer = "   <input name=^zTargetFileName^ id=^zTargetFileName^ type=^hidden^ value=^<%=strErrorMapValue%>^ >"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   <input name=^zTargetFileName^ id=^zTargetFileName^ type=^hidden^ value=^<%=strErrorMapValue%>^ >", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 );

   //:   IF szGroupIsTable = "Y" 
   if ( ZeidonStringCompare( szGroupIsTable, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:   szWriteBuffer = "<table>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<table>", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "<tr>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<tr>", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "<td>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<td>", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
   } 

   //:   END

   //:   // Process EditBox and PushBtn as subcontrols.
   //:   FOR EACH vDialogTemp.Control
   RESULT = SetCursorFirstEntity( vDialogTemp, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:   IF vDialogTemp.ControlDef.Tag = "EditBox"
      if ( CompareAttributeToString( vDialogTemp, "ControlDef", "Tag", "EditBox" ) == 0 )
      { 
         //:   szCtrlTag     = vDialogTemp.Control.Tag
         GetVariableFromAttribute( szCtrlTag, 0, 'S', 33, vDialogTemp, "Control", "Tag", "", 0 );
         //:   szControlType = vDialogTemp.ControlDef.Tag
         GetVariableFromAttribute( szControlType, 0, 'S', 51, vDialogTemp, "ControlDef", "Tag", "", 0 );
         //:   szWriteBuffer = "   <% /* " + szCtrlTag + ":" + szControlType + " */ %>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <% /* ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ":", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szControlType, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " */ %>", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );

         //:   //CreateAbsolStyleString( vDialogTemp, szStyle, lSizeX, lSizeY, "" )
         //:   zIntegerToString( szWidth, 10, vDialogTemp.Control.SZDLG_X * dDLUnits )
         GetIntegerFromAttribute( &lTempInteger_3, vDialogTemp, "Control", "SZDLG_X" );
         zIntegerToString( szWidth, 10, lTempInteger_3 * dDLUnits );
         //:   szSize = "width:" + szWidth + "px;"
         ZeidonStringCopy( szSize, 1, 0, "width:", 1, 0, 101 );
         ZeidonStringConcat( szSize, 1, 0, szWidth, 1, 0, 101 );
         ZeidonStringConcat( szSize, 1, 0, "px;", 1, 0, 101 );
         //:   // If zNoPositioning is null then we are using absolute positioning for controls
         //:   IF  szNoPositioning = ""
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
         { 
            //:   CreateAbsolPosString( vDialogTemp, szPosition, lOffsetX, lOffsetY )
            CreateAbsolPosString( vDialogTemp, szPosition, lOffsetX, lOffsetY );
         } 

         //:   END
         //:   
         //:   IF szNoPositioning = "S" 
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
         { 
            //:   szStyle = ""
            ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
            //:ELSE 
         } 
         else
         { 
            //:   FixStyleString( szStyle, szSize, szPosition,  "<%=strErrorColor%>"  )
            FixStyleString( szStyle, szSize, szPosition, "<%=strErrorColor%>" );
         } 

         //:   END
         //:   
         //:   //szStyle = szStyle + szTabIndex
         //:   szTitleHTML = ""
         ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
         //:   szTitle = vDialogTemp.Control.DIL_Text
         GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialogTemp, "Control", "DIL_Text", "", 0 );
         //:   IF szTitle != ""
         if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
         { 
            //:   szTitleHTML = " title=^" + szTitle + "^ "
            ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
            ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
            ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
         } 

         //:   END
         //:   
         //:   szPlaceholder = ""
         ZeidonStringCopy( szPlaceholder, 1, 0, "", 1, 0, 257 );
         //:   IF vDialogTemp.Control.WebPlaceholder != ""
         if ( CompareAttributeToString( vDialogTemp, "Control", "WebPlaceholder", "" ) != 0 )
         { 
            //:   szPlaceholder = " placeholder=^" + vDialogTemp.Control.WebPlaceholder + "^ "
            GetVariableFromAttribute( szTempString_11, 0, 'S', 255, vDialogTemp, "Control", "WebPlaceholder", "", 0 );
            ZeidonStringCopy( szPlaceholder, 1, 0, " placeholder=^", 1, 0, 257 );
            ZeidonStringConcat( szPlaceholder, 1, 0, szTempString_11, 1, 0, 257 );
            ZeidonStringConcat( szPlaceholder, 1, 0, "^ ", 1, 0, 257 );
         } 

         //:   END
         //:   
         //:   szHTML5Attr = ""
         ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
         //:   IF vDialogTemp.Control.WebHTML5Attribute != ""
         if ( CompareAttributeToString( vDialogTemp, "Control", "WebHTML5Attribute", "" ) != 0 )
         { 
            //:   szHTML5Attr = " " + vDialogTemp.Control.WebHTML5Attribute + " "
            GetVariableFromAttribute( szTempString_12, 0, 'S', 255, vDialogTemp, "Control", "WebHTML5Attribute", "", 0 );
            ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
            ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_12, 1, 0, 257 );
            ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
         } 

         //:   END

         //:   szClass = ""
         ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
         //:   IF vDialogTemp.Control.CSS_Class != ""
         if ( CompareAttributeToString( vDialogTemp, "Control", "CSS_Class", "" ) != 0 )
         { 
            //:   szClass = " class=^" + vDialogTemp.Control.CSS_Class + "^ "
            GetVariableFromAttribute( szTempString_13, 0, 'S', 255, vDialogTemp, "Control", "CSS_Class", "", 0 );
            ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
            ZeidonStringConcat( szClass, 1, 0, szTempString_13, 1, 0, 257 );
            ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
         } 

         //:   END
         //:   
         //:   // For some reason with the <input type="file" control, the width property doesn't
         //:   // seem to size the width correctly.  Use the "size" attribute instead.  The weird thing
         //:   // is that if I don't include the width in style, then the "Browse" button that gets
         //:   // created is not inline with the field that displays the upload file. ???!!!
         //:   zIntegerToString( szWidth, 10, vDialogTemp.Control.SZDLG_X / 3.5 )
         GetIntegerFromAttribute( &lTempInteger_4, vDialogTemp, "Control", "SZDLG_X" );
         zIntegerToString( szWidth, 10, lTempInteger_4 / (ZDecimal) 3.5 );

         //:   szWriteBuffer = "   <input name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^" + szClass + szHTML5Attr + szTitleHTML + szPlaceholder + " size=^" + szWidth + "^ " + szStyle + " type=^file^  >"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <input name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szPlaceholder, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " size=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szWidth, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " type=^file^  >", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 );

         //:   LastY_Pos    = vDialogTemp.Control.PSDLG_Y
         GetIntegerFromAttribute( &LastY_Pos, vDialogTemp, "Control", "PSDLG_Y" );
         //:   LastY_Height = vDialogTemp.Control.SZDLG_Y
         GetIntegerFromAttribute( &LastY_Height, vDialogTemp, "Control", "SZDLG_Y" );
         //:   LastX_Pos    = vDialogTemp.Control.PSDLG_X
         GetIntegerFromAttribute( &LastX_Pos, vDialogTemp, "Control", "PSDLG_X" );
         //:   LastX_Width  = vDialogTemp.Control.SZDLG_X
         GetIntegerFromAttribute( &LastX_Width, vDialogTemp, "Control", "SZDLG_X" );
      } 

      //:   END
      //:   // We are just assuming that the "Unload" button is positioned after the edit field.
      //:   IF vDialogTemp.ControlDef.Tag = "PushBtn"
      if ( CompareAttributeToString( vDialogTemp, "ControlDef", "Tag", "PushBtn" ) == 0 )
      { 

         //:   IF szGroupIsTable = "Y" AND ( vDialogTemp.Control.PSDLG_Y > LastY_Pos )
         if ( ZeidonStringCompare( szGroupIsTable, 1, 0, "Y", 1, 0, 2 ) == 0 && ( CompareAttributeToInteger( vDialogTemp, "Control", "PSDLG_Y", LastY_Pos ) > 0 ) )
         { 
            //:   szWriteBuffer = "</td>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "</td>", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "</tr>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "</tr>", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "<tr>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<tr>", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "<td>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<td>", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:ELSE
         } 
         else
         { 
            //:IF szGroupIsTable = "Y" AND ( vDialogTemp.Control.PSDLG_Y <= LastY_Pos ) 
            if ( ZeidonStringCompare( szGroupIsTable, 1, 0, "Y", 1, 0, 2 ) == 0 && ( CompareAttributeToInteger( vDialogTemp, "Control", "PSDLG_Y", LastY_Pos ) <= 0 ) )
            { 
               //:szWriteBuffer = "</td>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "</td>", 1, 0, 10001 );
               //:WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "<td>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "<td>", 1, 0, 10001 );
               //:WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )            
               WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            } 

            //:END
         } 

         //:   END

         //:   // Determine if there is blank space in between the y coordinates of the last
         //:   // control and the current control.  If so, then we need to put in a div as a spacer control.
         //:   IF szNoPositioning = "Y" AND szGroupIsTable = "" AND ( vDialogTemp.Control.PSDLG_Y > LastY_Pos )
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 && ZeidonStringCompare( szGroupIsTable, 1, 0, "", 1, 0, 2 ) == 0 && ( CompareAttributeToInteger( vDialogTemp, "Control", "PSDLG_Y", LastY_Pos ) > 0 ) )
         { 

            //:      Y_Size = ( vDialogTemp.Control.PSDLG_Y - ( LastY_Pos + LastY_Height ) ) * dDLUnits
            GetIntegerFromAttribute( &lTempInteger_5, vDialogTemp, "Control", "PSDLG_Y" );
            Y_Size = ( lTempInteger_5 - ( LastY_Pos + LastY_Height ) ) * dDLUnits;
            //:      X_Size = 10
            X_Size = 10;
            //:      zIntegerToString( szHeight, 16, Y_Size )
            zIntegerToString( szHeight, 16, Y_Size );
            //:      zIntegerToString( szWidth, 16, X_Size )
            zIntegerToString( szWidth, 16, X_Size );
            //:      szWriteBuffer = ""
            ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
            //:      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:      szWriteBuffer = " <!-- This is added as a line spacer -->"
            ZeidonStringCopy( szWriteBuffer, 1, 0, " <!-- This is added as a line spacer -->", 1, 0, 10001 );
            //:      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:      szWriteBuffer = "<div style=^height:" + szHeight + "px;width:" + szWidth + "px;^></div>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<div style=^height:", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szHeight, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "px;width:", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szWidth, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "px;^></div>", 1, 0, 10001 );
            //:      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
         } 

         //:   END
         //:   IF szNoPositioning = "Y" AND szGroupIsTable = "" AND ( vDialogTemp.Control.PSDLG_X > 0 )
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 && ZeidonStringCompare( szGroupIsTable, 1, 0, "", 1, 0, 2 ) == 0 && ( CompareAttributeToInteger( vDialogTemp, "Control", "PSDLG_X", 0 ) > 0 ) )
         { 

            //:      X_Size = ( vDialogTemp.Control.PSDLG_X ) * dDLUnits
            GetIntegerFromAttribute( &lTempInteger_6, vDialogTemp, "Control", "PSDLG_X" );
            X_Size = ( lTempInteger_6 ) * dDLUnits;
            //:      Y_Size = 1
            Y_Size = 1;
            //:      zIntegerToString( szHeight, 16, Y_Size )
            zIntegerToString( szHeight, 16, Y_Size );
            //:      zIntegerToString( szWidth, 16, X_Size )
            zIntegerToString( szWidth, 16, X_Size );
            //:      szWriteBuffer = ""
            ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
            //:      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:      szWriteBuffer = " <!-- This is added as a width spacer -->"
            ZeidonStringCopy( szWriteBuffer, 1, 0, " <!-- This is added as a width spacer -->", 1, 0, 10001 );
            //:      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:      szWriteBuffer = "<div style=^height:1px;width:" + szWidth + "px;float:left;^></div>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<div style=^height:1px;width:", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szWidth, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "px;float:left;^></div>", 1, 0, 10001 );
            //:      WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
         } 

         //:      //szWriteBuffer = "<div style=^height:1px;width:" + szWidth + "px;float:left;^></div>   <!-- Width Spacer -->"
         //:      //WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   END
         //:            
         //:   szCtrlTag     = vDialogTemp.Control.Tag
         GetVariableFromAttribute( szCtrlTag, 0, 'S', 33, vDialogTemp, "Control", "Tag", "", 0 );
         //:   szControlType = vDialogTemp.ControlDef.Tag
         GetVariableFromAttribute( szControlType, 0, 'S', 51, vDialogTemp, "ControlDef", "Tag", "", 0 );
         //:   szWriteBuffer = "   <% /* " + szCtrlTag + ":" + szControlType + " */ %>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <% /* ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ":", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szControlType, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " */ %>", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );

         //:   //CreateAbsolStyleString( vDialogTemp, szStyle, lSizeX, lSizeY, "" )
         //:   IF szNoPositioning = "Y"
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:   CreateNoPosStyleString( vDialogTemp, szStyle, "" )
            CreateNoPosStyleString( vDialogTemp, szStyle, "" );
            //:/*
            //:// KJS 01/26/17 - Trying to take out the button height for Aadit. I'm not sure we really want to do this
            //:// but I am just going to put it in temporarily.
            //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
            //://zIntegerToString( szHeight, 10, vDialog.Control.SZDLG_Y * dDLUnits )
            //:szStyle = "style=^width:" + szWidth + "px;^"
            //:*/
            //:ELSE
         } 
         else
         { 
            //:IF szNoPositioning = "S" // No style information
            if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
            { 
               //:szStyle = "" 
               ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
               //:ELSE 
            } 
            else
            { 
               //:CreateAbsolStyleString( vDialogTemp, szStyle, lOffsetX, lOffsetY, "" )
               CreateAbsolStyleString( vDialogTemp, szStyle, lOffsetX, lOffsetY, "" );
            } 

            //:END
         } 

         //:   END
         //:   
         //:   //If we put a title on the control, when the mouse is hovered over the control, this text will display.
         //:   szTitleHTML = ""
         ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
         //:   szTitle = vDialogTemp.Control.DIL_Text
         GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialogTemp, "Control", "DIL_Text", "", 0 );
         //:   IF szTitle != ""
         if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
         { 
            //:   szTitleHTML = " title=^" + szTitle + "^ "
            ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
            ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
            ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
         } 

         //:   END

         //:   szHTML5Attr = ""
         ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
         //:   IF vDialogTemp.Control.WebHTML5Attribute != ""
         if ( CompareAttributeToString( vDialogTemp, "Control", "WebHTML5Attribute", "" ) != 0 )
         { 
            //:   szHTML5Attr = " " + vDialogTemp.Control.WebHTML5Attribute + " "
            GetVariableFromAttribute( szTempString_14, 0, 'S', 255, vDialogTemp, "Control", "WebHTML5Attribute", "", 0 );
            ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
            ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_14, 1, 0, 257 );
            ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
         } 

         //:   END

         //:   szActionTag = ""
         ZeidonStringCopy( szActionTag, 1, 0, "", 1, 0, 35 );
         //:   IF vDialogTemp.EventAct EXISTS
         lTempInteger_7 = CheckExistenceOfEntity( vDialogTemp, "EventAct" );
         if ( lTempInteger_7 == 0 )
         { 
            //:   szActionTag = vDialogTemp.EventAct.Tag
            GetVariableFromAttribute( szActionTag, 0, 'S', 35, vDialogTemp, "EventAct", "Tag", "", 0 );
         } 

         //:   END
         //:   szClass = vDialogTemp.Control.CSS_Class
         GetVariableFromAttribute( szClass, 0, 'S', 257, vDialogTemp, "Control", "CSS_Class", "", 0 );
         //:   
         //:   IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
         if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
         { 
            //:   szText2 = vDialogTemp.Control.Text
            GetVariableFromAttribute( szText2, 0, 'S', 501, vDialogTemp, "Control", "Text", "", 0 );
            //:   szTextID = vDialog.Control.LangID
            GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
            //:   zSearchAndReplace( szText2, 1000, "\", "\\" )
            zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
            //:   szSrch = "\" + QUOTES 
            ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
            ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
            //:   zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
            zSearchAndReplace( szText2, 1000, QUOTES, szSrch );
            //:   szSrch = ""
            ZeidonStringCopy( szSrch, 1, 0, "", 1, 0, 6 );
            //:   IF szTextID = ""
            if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
            { 
               //:   szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
               ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
               ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
               ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
               //:ELSE 
            } 
            else
            { 
               //:   szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
               ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
               ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
               ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 501 );
               ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 501 );
               ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
            } 

            //:   END
            //:   //szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
            //:ELSE
         } 
         else
         { 
            //:   szText = vDialogTemp.Control.Text
            GetVariableFromAttribute( szText, 0, 'S', 501, vDialogTemp, "Control", "Text", "", 0 );
         } 

         //:   END
         //:   
         //:   //IF szNoPositioning = "Y"
         //:   //   szWriteBuffer = "<div style=^float:left;^>"
         //:   //   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
         //:   //END
         //:   IF szClass = ""
         if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:   //szWriteBuffer = "   <input type=^submit^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ " + " value=^" + vDialogTemp.Control.Text + "^" +
            //:   //                " onclick=^" + szActionTag + "()^ " + szStyle + " >"
            //:   szWriteBuffer = "   <button type=^button^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ " +
            //:                   " onclick=^" + szActionTag + "()^ " + szStyle + szTitleHTML + szHTML5Attr + " >" + szText + "</button>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   <button type=^button^ name=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szActionTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "()^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " >", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</button>", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 
            //:   //szWriteBuffer = "   <input class=^" + szClass + "^ type=^submit^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ " + " value=^" + vDialogTemp.Control.Text + "^" +
            //:   //                " onclick=^" + szActionTag + "()^ " + szStyle + " >"
            //:   szWriteBuffer = "   <button type=^button^ class=^" + szClass + "^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ " +
            //:                   " onclick=^" + szActionTag + "()^ " + szStyle + szTitleHTML + szHTML5Attr + " >" + szText + "</button>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   <button type=^button^ class=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szActionTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "()^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " >", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</button>", 1, 0, 10001 );
         } 

         //:   END
         //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 );
         //:   //IF szNoPositioning = "Y"
         //:   //   szWriteBuffer = "</div>"
         //:   //   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
         //:   //END
         //:   IF szGroupIsTable = "Y" 
         if ( ZeidonStringCompare( szGroupIsTable, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:   szWriteBuffer = "</td>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "</td>", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "</tr>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "</tr>", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "</table>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "</table>", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );
         } 

         //:   END
      } 

      RESULT = SetCursorNextEntity( vDialogTemp, "Control", "" );
      //:   END
   } 

   //:   END
   //:   ResetViewFromSubobject( vDialogTemp )
   ResetViewFromSubobject( vDialogTemp );

   //:   szWriteBuffer = "</div>  <!-- " + vDialogTemp.Control.Tag + " --> "
   GetVariableFromAttribute( szTempString_15, 0, 'S', 33, vDialogTemp, "Control", "Tag", "", 0 );
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</div>  <!-- ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_15, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " --> ", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 0 );

   //:   //szWriteBuffer = "</form>"
   //:   //WL_QC( vDialog, lFileJSP, szWriteBuffer, "^", 1 )

   //:   //lControl = zQUAL_STRING + zPOS_NEXT + zRECURS
   //:   //szCtrlTag = "File Transfer to Server"
   //:   //nRC = SetEntityCursor( vDialogTemp, "WebControlProperty", "Name", lControl,
   //:   //                       szCtrlTag, "", "", 0, "Window", "" )
   //://END
   //:DropView( vDialogTemp )
   DropView( vDialogTemp );
   return( 0 );
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteListBoxAsList( VIEW vDialog,
//:                           INTEGER lFile,
//:                           STRING ( 10000 ) szWriteBuffer,
//:                           STRING ( 50 )    szIndent,
//:                           STRING ( 32 )    szCtrlTag,
//:                           STRING ( 1 )     szNoPositioning,
//:                           INTEGER lOffsetX,
//:                           INTEGER lOffsetY )

//:   VIEW vScope      BASED ON LOD TZWDLGSO  // identifies scoping entity for list entries
zOPER_EXPORT zSHORT OPERATION
GenJSPJ_CrteListBoxAsList( zVIEW     vDialog,
                           zLONG     lFile,
                           zPCHAR    szWriteBuffer,
                           zPCHAR    szIndent,
                           zPCHAR    szCtrlTag,
                           zPCHAR    szNoPositioning,
                           zLONG     lOffsetX,
                           zLONG     lOffsetY )
{
   zVIEW     vScope = 0; 
   //:VIEW vDialogRoot BASED ON LOD TZWDLGSO  // identifies scoping entity for list entries
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 32 )  szContextName
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szScopingEntityName
   zCHAR     szScopingEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szListEntityName
   zCHAR     szListEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szAttributeName
   zCHAR     szAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szjMobile
   zCHAR     szjMobile[ 33 ] = { 0 }; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szHTML5Attr
   zCHAR     szHTML5Attr[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) ListBoxDoubleClickAction
   zCHAR     ListBoxDoubleClickAction[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szSubCtrlTag
   zCHAR     szSubCtrlTag[ 257 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 10 )  szMaxStringLth
   zCHAR     szMaxStringLth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szWidth
   zCHAR     szWidth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szSelectAction
   zCHAR     szSelectAction[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szSelectFunction
   zCHAR     szSelectFunction[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szHeight
   zCHAR     szHeight[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szX_Pos
   zCHAR     szX_Pos[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szY_Pos
   zCHAR     szY_Pos[ 11 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:INTEGER        lSubtype
   zLONG     lSubtype = 0; 
   //:INTEGER        lMaxStringLth
   zLONG     lMaxStringLth = 0; 
   zCHAR     szTempString_0[ 1026 ]; 
   zCHAR     szTempString_1[ 1026 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zCHAR     szTempString_2[ 1026 ]; 
   zCHAR     szTempString_3[ 1026 ]; 
   zCHAR     szTempString_4[ 1026 ]; 
   zSHORT    lTempInteger_6; 
   zSHORT    lTempInteger_7; 
   zSHORT    lTempInteger_8; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 1026 ]; 
   zCHAR     szTempString_7[ 1026 ]; 
   zSHORT    lTempInteger_9; 
   zSHORT    lTempInteger_10; 
   zCHAR     szTempString_8[ 1026 ]; 
   zSHORT    lTempInteger_11; 
   zCHAR     szTempString_9[ 1026 ]; 
   zCHAR     szTempString_10[ 1026 ]; 
   zCHAR     szTempString_11[ 1026 ]; 
   zCHAR     szTempString_12[ 1026 ]; 
   zSHORT    lTempInteger_12; 
   zCHAR     szTempString_13[ 1026 ]; 


   //:szStyle = "width:" + szWidth + "px; height:" + szHeight + "px;"
   ZeidonStringCopy( szStyle, 1, 0, "width:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px; height:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szHeight, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );
   //:IF vDialog.Control.CSS_Class = ""
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) == 0 )
   { 
      //:szClass = ""
      ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szClass = " class=^" + vDialog.Control.CSS_Class + "^"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 1026, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
   } 

   //:END

   //:szHTML5Attr = ""
   ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
   //:IF vDialog.Control.WebHTML5Attribute != ""
   if ( CompareAttributeToString( vDialog, "Control", "WebHTML5Attribute", "" ) != 0 )
   { 
      //:szHTML5Attr = " " + vDialog.Control.WebHTML5Attribute + " "
      GetVariableFromAttribute( szTempString_1, 0, 'S', 1026, vDialog, "Control", "WebHTML5Attribute", "", 0 );
      ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
   } 

   //:END

   //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = " title=^" + szTitle + "^ "
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:CreateTabIndexString( vDialog, szTabIndex )
   CreateTabIndexString( vDialog, szTabIndex );
   //:// We will set the height to the constant 20 px.
   //:PIX_PER_DU( vDialog, dDLUnits )
   PIX_PER_DU( vDialog, &dDLUnits );
   //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
   GetIntegerFromAttribute( &lTempInteger_0, vDialog, "Control", "SZDLG_X" );
   zIntegerToString( szWidth, 10, lTempInteger_0 * dDLUnits );
   //:zIntegerToString( szHeight, 10, vDialog.Control.SZDLG_Y * dDLUnits )
   GetIntegerFromAttribute( &lTempInteger_1, vDialog, "Control", "SZDLG_Y" );
   zIntegerToString( szHeight, 10, lTempInteger_1 * dDLUnits );
   //:szStyle = " style=^width:" + szWidth + "px;height:" + szHeight + "px;"
   ZeidonStringCopy( szStyle, 1, 0, " style=^width:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px;height:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szHeight, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );

   //:// If szNoPositioning is null then we are using absolute positioning on controls.
   //:IF szNoPositioning = ""
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
   { 

      //:zIntegerToString( szX_Pos, 10, vDialog.Control.PSDLG_X * dDLUnits )
      GetIntegerFromAttribute( &lTempInteger_2, vDialog, "Control", "PSDLG_X" );
      zIntegerToString( szX_Pos, 10, lTempInteger_2 * dDLUnits );
      //:zIntegerToString( szY_Pos, 10, vDialog.Control.PSDLG_Y * dDLUnits )
      GetIntegerFromAttribute( &lTempInteger_3, vDialog, "Control", "PSDLG_Y" );
      zIntegerToString( szY_Pos, 10, lTempInteger_3 * dDLUnits );
      //:szStyle = szStyle + "position:absolute;" + "left:" + szX_Pos + "px;" + "top:" + szY_Pos + "px;"
      ZeidonStringConcat( szStyle, 1, 0, "position:absolute;", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "left:", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, szX_Pos, 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "top:", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, szY_Pos, 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );
      //://CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" 
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
      } 

      //:END
   } 

   //:END

   //:IF szStyle != ""
   if ( ZeidonStringCompare( szStyle, 1, 0, "", 1, 0, 301 ) != 0 )
   { 
      //:szStyle = szStyle + "^ " 
      ZeidonStringConcat( szStyle, 1, 0, "^ ", 1, 0, 301 );
   } 

   //:END
   //:szStyle = szStyle + szTabIndex
   ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );

   //:szWriteBuffer = "<% strErrorMapValue = ^^;  %>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strErrorMapValue = ^^;  %>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:szSelectFunction = "OnClick"
   ZeidonStringCopy( szSelectFunction, 1, 0, "OnClick", 1, 0, 11 );

   //:   // Set cursor to the scoping entity if one exists
   //:   CreateViewFromViewForTask( vScope, vDialog, 0 )
   CreateViewFromViewForTask( &vScope, vDialog, 0 );
   //:   SET CURSOR NEXT vScope.CtrlMap
   RESULT = SetCursorNextEntity( vScope, "CtrlMap", "" );
   //:   
   //:// Put in data-role if this is for a jmobile. kkk
   //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
   GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );
   //:IF vDialogRoot.WndStyle.Tag = "jMobile Window"
   if ( CompareAttributeToString( vDialogRoot, "WndStyle", "Tag", "jMobile Window" ) == 0 )
   { 
      //:szjMobile = " data-role=^listview^ "
      ZeidonStringCopy( szjMobile, 1, 0, " data-role=^listview^ ", 1, 0, 33 );
      //:szWriteBuffer = "   <ul id=^" + szCtrlTag + "^ name=^" + szCtrlTag + "^" + szClass + szjMobile + szHTML5Attr + szStyle + szDisabled + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   <ul id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szjMobile, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "   <ol id=^" + szCtrlTag + "^ name=^" + szCtrlTag + "^" + szClass + szjMobile + szHTML5Attr + szStyle + szDisabled + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   <ol id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szjMobile, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
   } 

   //:END      

   //://szWriteBuffer = "   <ol id=^" + szCtrlTag + "^ name=^" + szCtrlTag + "^" + szClass + szjMobile + szHTML5Attr + szStyle + szDisabled + ">"
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:   szWriteBuffer = "<%"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:   IF vDialog.CtrlMapView EXISTS
   lTempInteger_4 = CheckExistenceOfEntity( vDialog, "CtrlMapView" );
   if ( lTempInteger_4 == 0 )
   { 


      //:   // See if a list entity has been specified, and if so, use it.
      //:   // Since this is a listbox I believe this should always be specified.
      //:   IF vDialog.CtrlMapLOD_Entity EXISTS
      lTempInteger_5 = CheckExistenceOfEntity( vDialog, "CtrlMapLOD_Entity" );
      if ( lTempInteger_5 == 0 )
      { 
         //:   szListEntityName = vDialog.CtrlMapLOD_Entity.Name
         GetVariableFromAttribute( szListEntityName, 0, 'S', 33, vDialog, "CtrlMapLOD_Entity", "Name", "", 0 );
      } 

      //:   END

      //:   szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
      //:                   "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_2, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_3, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) )"
      GetVariableFromAttribute( szTempString_4, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   
      //:   szWriteBuffer = "      long   lEntityKey;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      long   lEntityKey;", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      String strEntityKey = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      String strEntityKey = ^^;", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )                  
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   
      //:   // Set position on the text field inside the listbox.
      //:   IF vDialog.CtrlCtrl EXISTS
      lTempInteger_6 = CheckExistenceOfEntity( vDialog, "CtrlCtrl" );
      if ( lTempInteger_6 == 0 )
      { 
         //:   SetViewToSubobject( vDialog, "CtrlCtrl" )
         SetViewToSubobject( vDialog, "CtrlCtrl" );
         //:   szSubCtrlTag = vDialog.Control.Tag
         GetVariableFromAttribute( szSubCtrlTag, 0, 'S', 257, vDialog, "Control", "Tag", "", 0 );
         //:   szWriteBuffer = "      String str" + szSubCtrlTag + " = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      String str", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " = ^^;", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   IF vDialog.CtrlMapLOD_Attribute EXISTS
         lTempInteger_7 = CheckExistenceOfEntity( vDialog, "CtrlMapLOD_Attribute" );
         if ( lTempInteger_7 == 0 )
         { 
            //:      // If the Grid entry requests URL Mapping, generate the variable definition here.
            //:      SET CURSOR FIRST vDialog.CtrlMap WHERE vDialog.CtrlMap.Tag = "Hyperlink URL"
            RESULT = SetCursorFirstEntityByString( vDialog, "CtrlMap", "Tag", "Hyperlink URL", "" );
            //:      IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:      // Define variable for URL Mapping.
               //:      szWriteBuffer = "      String str" + szSubCtrlTag + "_URL = ^^;"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      String str", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "_URL = ^^;", 1, 0, 10001 );
               //:      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            } 

            //:      END
         } 

         //:   END
      } 

      //:   END
      //:   ResetViewFromSubobject( vDialog )
      ResetViewFromSubobject( vDialog );

      //:   szWriteBuffer = "      View v" + szCtrlTag + ";"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      View v", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ";", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:   //szWriteBuffer = "      strComboCurrentValue = ^^;"
      //:   //WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )

      //:   szAttributeName = ""
      ZeidonStringCopy( szAttributeName, 1, 0, "", 1, 0, 33 );
      //:   szEntityName = ""
      ZeidonStringCopy( szEntityName, 1, 0, "", 1, 0, 33 );
      //:   szContextName = ""
      ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );


      //:// Getting view

      //:   // See if a scoping entity has been specified, and if so, use it.
      //:   szScopingEntityName = ""
      ZeidonStringCopy( szScopingEntityName, 1, 0, "", 1, 0, 33 );
      //:   lSubtype = vDialog.Control.Subtype
      GetIntegerFromAttribute( &lSubtype, vDialog, "Control", "Subtype" );
      //:   lSubtype = IsFlagSequenceSet( lSubtype, zLISTBOX_SCOPE_OI )
      lSubtype = IsFlagSequenceSet( lSubtype, zLISTBOX_SCOPE_OI );
      //:   IF lSubtype = 0
      if ( lSubtype == 0 )
      { 
         //:   IF vScope.CtrlMapLOD_Entity EXISTS
         lTempInteger_8 = CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" );
         if ( lTempInteger_8 == 0 )
         { 
            //:   szScopingEntityName = "^" + vScope.CtrlMapLOD_Entity.Name + "^"
            GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vScope, "CtrlMapLOD_Entity", "Name", "", 0 );
            ZeidonStringCopy( szScopingEntityName, 1, 0, "^", 1, 0, 33 );
            ZeidonStringConcat( szScopingEntityName, 1, 0, szTempString_5, 1, 0, 33 );
            ZeidonStringConcat( szScopingEntityName, 1, 0, "^", 1, 0, 33 );
         } 

         //:   END
      } 

      //:   END

      //:   szWriteBuffer = "      v" + szCtrlTag + " = " + vDialog.CtrlMapView.Name + ".newView( );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      v", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_6, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".newView( );", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:   //szWriteBuffer = "      ComboCount = 0;"
      //:   //WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      //:   //szWriteBuffer = "      strComboSelectedValue = ^0^;"
      //:   //WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )

      //:   IF lSubtype = 0
      if ( lSubtype == 0 )
      { 
         //:   szWriteBuffer = "      csrRC = v" + szCtrlTag + ".cursor( ^" +
         //:                                szListEntityName + "^ ).setFirst( " + szScopingEntityName + " );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      csrRC = v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirst( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szScopingEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " );", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:   szWriteBuffer = "      csrRC = v" + szCtrlTag + ".cursor( ^" + szListEntityName + "^ ).setFirstWithinOi( );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      csrRC = v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirstWithinOi( );", 1, 0, 10001 );
      } 

      //:   END

      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      while ( csrRC.isSet() )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      while ( csrRC.isSet() )", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );


      //:   szWriteBuffer = "         lEntityKey = v" + szCtrlTag +
      //:                   ".cursor( ^" + vDialog.CtrlMapLOD_Entity.Name + "^ ).getEntityKey( );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         lEntityKey = v", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_7, 0, 'S', 1026, vDialog, "CtrlMapLOD_Entity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getEntityKey( );", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         strEntityKey = Long.toString( lEntityKey );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strEntityKey = Long.toString( lEntityKey );", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:   // Set position on the text field inside the listbox.
      //:   SetViewToSubobject( vDialog, "CtrlCtrl" )
      SetViewToSubobject( vDialog, "CtrlCtrl" );
      //:   IF vDialog.CtrlMapLOD_Attribute EXISTS
      lTempInteger_9 = CheckExistenceOfEntity( vDialog, "CtrlMapLOD_Attribute" );
      if ( lTempInteger_9 == 0 )
      { 

         //:   szAttributeName = vDialog.CtrlMapER_Attribute.Name
         GetVariableFromAttribute( szAttributeName, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
         //:   szEntityName = vDialog.CtrlMapRelatedEntity.Name
         GetVariableFromAttribute( szEntityName, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );

         //:   szWriteBuffer = "         nRC = v" + szCtrlTag + ".cursor( ^" +
         //:                   szEntityName + "^ ).checkExistenceOfEntity( ).toInt();"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   szWriteBuffer = "         if ( nRC >= 0 )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   szWriteBuffer = "         {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   IF vDialog.CtrlMapContext EXISTS
         lTempInteger_10 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
         if ( lTempInteger_10 == 0 )
         { 
            //:   szContextName = vDialog.CtrlMapContext.Name
            GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
         } 

         //:   END

         //:   lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
         GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
         //:   IF lMaxStringLth <= 0
         if ( lMaxStringLth <= 0 )
         { 
            //:   szMaxStringLth = "254"
            ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
            //:ELSE
         } 
         else
         { 
            //:   szMaxStringLth = lMaxStringLth
            ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
         } 

         //:   END

         //:   szWriteBuffer = "            str" + szSubCtrlTag + " = v" + szCtrlTag +
         //:            ".cursor( ^" +
         //:            szEntityName + "^ ).getAttribute( ^" +
         //:            szAttributeName + "^ ).getString( ^" + szContextName + "^ );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            str", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " = v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szAttributeName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:   // KJS 02/15/11 - Thinking I need to add code to change values returned as null to ""
         //:   szWriteBuffer = "            if ( str" + szSubCtrlTag + " == null )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( str", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " == null )", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   szWriteBuffer = "               str" + szSubCtrlTag + " = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               str", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " = ^^;", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   
         //:   
         //:   // If the Grid entry requests URL Mapping, generate the code here.
         //:   SET CURSOR FIRST vDialog.CtrlMap WHERE vDialog.CtrlMap.Tag = "Hyperlink URL"
         RESULT = SetCursorFirstEntityByString( vDialog, "CtrlMap", "Tag", "Hyperlink URL", "" );
         //:   IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:   //szControlTag = "str" + vDialog.Control.Tag + "_URL"
            //:   szWriteBuffer = "            str" + szSubCtrlTag + "_URL = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            str", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "_URL = ^^;", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "            nRC = v" + szCtrlTag +
            //:                ".cursor( ^" + vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            nRC = v", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_8, 0, 'S', 1026, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:   szWriteBuffer = "            if ( nRC >= 0 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( nRC >= 0 )", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:   lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
            GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
            //:   IF lMaxStringLth <= 0
            if ( lMaxStringLth <= 0 )
            { 
               //:   szMaxStringLth = "254"
               ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
               //:ELSE
            } 
            else
            { 
               //:   szMaxStringLth = lMaxStringLth
               ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
            } 

            //:   END
            //:   IF vDialog.CtrlMapContext EXISTS
            lTempInteger_11 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
            if ( lTempInteger_11 == 0 )
            { 
               //:   szContextName = vDialog.CtrlMapContext.Name
               GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
               //:ELSE
            } 
            else
            { 
               //:   szContextName = ""
               ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
            } 

            //:   END
            //:   szWriteBuffer = "               str" + szSubCtrlTag + "_URL = v" + szCtrlTag +
            //:                   ".cursor( ^" + vDialog.CtrlMapRelatedEntity.Name +
            //:                   "^ ).getAttribute( ^" + vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" + szContextName + "^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "               str", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "_URL = v", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_9, 0, 'S', 1026, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_10, 0, 'S', 1026, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
         } 

         //:   END
         //:               
         //:   szWriteBuffer = "      }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

         //:/****************************************         */

         //://szWriteBuffer = "       <li><a href=^#^ " + szClass + " onclick=^" + szActionName + "( )^>" + szNavigationTitle + "</a></li>"
         //:// In the case of jmobile, I am putting the HTML5 and class on the <a href
         //://IF vDialogRoot.WndStyle.Tag = "jMobile Window"
         //://szWriteBuffer = "       <li id=^" + szCtrlTag + "^ name=^" + szCtrlTag + "^><a href=^#^ " + szHTML5Attr + szClass + " onclick=^" + szActionName + "()^>" + szNavigationTitle + "</a></li>"
         //://ELSE
         //://szWriteBuffer = "       <li id=^" + szCtrlTag + "^ name=^" + szCtrlTag + "^" + szHTML5Attr + szClass + "><a href=^#^ " + " onclick=^" + szActionName + "()^>" + szNavigationTitle + "</a></li>"
         //://END
         //://WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )

         //:   szWriteBuffer = "%>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:   IF vDialog.Control.CSS_Class = ""
         if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) == 0 )
         { 
            //:   szClass = ""
            ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
            //:ELSE
         } 
         else
         { 
            //:   szClass = " class=^" + vDialog.Control.CSS_Class + "^"
            GetVariableFromAttribute( szTempString_11, 0, 'S', 1026, vDialog, "Control", "CSS_Class", "", 0 );
            ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
            ZeidonStringConcat( szClass, 1, 0, szTempString_11, 1, 0, 257 );
            ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
         } 

         //:   END

         //:   szHTML5Attr = ""
         ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
         //:   IF vDialog.Control.WebHTML5Attribute != ""
         if ( CompareAttributeToString( vDialog, "Control", "WebHTML5Attribute", "" ) != 0 )
         { 
            //:   szHTML5Attr = " " + vDialog.Control.WebHTML5Attribute + " "
            GetVariableFromAttribute( szTempString_12, 0, 'S', 1026, vDialog, "Control", "WebHTML5Attribute", "", 0 );
            ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
            ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_12, 1, 0, 257 );
            ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
         } 

         //:   END

         //:   //If we put a title on the control, when the mouse is hovered over the control, this text will display.
         //:   szTitleHTML = ""
         ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
         //:   szTitle = vDialog.Control.DIL_Text
         GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
         //:   IF szTitle != ""
         if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
         { 
            //:   szTitleHTML = " title=^" + szTitle + "^ "
            ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
            ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
            ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
         } 

         //:   END
         //:               
         //:   IF vDialog.EventAct EXISTS
         lTempInteger_12 = CheckExistenceOfEntity( vDialog, "EventAct" );
         if ( lTempInteger_12 == 0 )
         { 
            //:   IF vDialog.EventAct.Type = 45    // Type 45 is Hyperlink Action.
            if ( CompareAttributeToInteger( vDialog, "EventAct", "Type", 45 ) == 0 )
            { 
               //:   // For a Hyperlink in a Grid, we assume there is ALWAYS both "Displayed Text" mapping and
               //:   // "Hyperlink URL" mapping, which have been set above.
               //:   // THIS DOESN"T WORK AT THE MOMENT
               //:   szWriteBuffer = "   <li id=^" + szSubCtrlTag + "::<%=strEntityKey%>^ name=^" + szSubCtrlTag + "::<%=strEntityKey%>^" + szHTML5Attr + szClass  
               ZeidonStringCopy( szWriteBuffer, 1, 0, "   <li id=^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^ name=^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
               //:   szWriteBuffer = szWriteBuffer + "><a href=^<%=str" + szSubCtrlTag + "_URL%>^ " + szTitleHTML + " target=^_blank^><%=str" + szSubCtrlTag 
               ZeidonStringConcat( szWriteBuffer, 1, 0, "><a href=^<%=str", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "_URL%>^ ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " target=^_blank^><%=str", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
               //:   szWriteBuffer = szWriteBuffer + "%></a></li>"
               ZeidonStringConcat( szWriteBuffer, 1, 0, "%></a></li>", 1, 0, 10001 );
               //:ELSE
            } 
            else
            { 
               //:   // Put the class html5 on <a ref. I know we want this for window style jmobile but not sure we want this all the time? Then we would do above line?
               //:   szWriteBuffer = "   <li id=^" + szSubCtrlTag + "::<%=strEntityKey%>^ name=^" + szSubCtrlTag + "::<%=strEntityKey%>^>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "   <li id=^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^ name=^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^>", 1, 0, 10001 );
               //:   szWriteBuffer = szWriteBuffer + "<a href=^#^" + szHTML5Attr + szClass + " onclick=^" + vDialog.EventAct.Tag + "( this.id )^"
               ZeidonStringConcat( szWriteBuffer, 1, 0, "<a href=^#^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_13, 0, 'S', 1026, vDialog, "EventAct", "Tag", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_13, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "( this.id )^", 1, 0, 10001 );
               //:   szWriteBuffer = szWriteBuffer + szTitleHTML + "><%=str" + szSubCtrlTag + "%></a></li>"
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "><%=str", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "%></a></li>", 1, 0, 10001 );
            } 

            //:   END
            //:ELSE
         } 
         else
         { 
            //:   // Regular default Grid subcontrol.
            //:   szWriteBuffer = "   <li id=^" + szSubCtrlTag + "::<%=strEntityKey%>^ name=^" + szSubCtrlTag + "::<%=strEntityKey%>^><%=str" + szSubCtrlTag + "%></li>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   <li id=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^ name=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "::<%=strEntityKey%>^><%=str", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szSubCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "%></li>", 1, 0, 10001 );
         } 

         //:   END
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

         //:   szWriteBuffer = "<%"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:/**********************************************/         
         //:ELSE
      } 
      else
      { 
         //:   szWriteBuffer = "      strComboCurrentValue = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strComboCurrentValue = ^^;", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      } 

      //:   END
      //:   ResetViewFromSubobject( vDialog )
      ResetViewFromSubobject( vDialog );
      //:      szWriteBuffer = "         csrRC =  v" + szCtrlTag +
      //:                      ".cursor( ^" + szListEntityName +
      //:                      "^ ).setNextContinue( );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         csrRC =  v", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setNextContinue( );", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   DropView( vScope )
      DropView( vScope );
   } 

   //:   END

   //:   szWriteBuffer = "      }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   //:   szWriteBuffer = "      v" + szCtrlTag + ".drop( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      v", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, ".drop( );", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:   szWriteBuffer = "   }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:   szWriteBuffer = "%>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:IF vDialogRoot.WndStyle.Tag = "jMobile Window"
   if ( CompareAttributeToString( vDialogRoot, "WndStyle", "Tag", "jMobile Window" ) == 0 )
   { 
      //:szWriteBuffer = "</ul>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</ul>", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "</ol>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</ol>", 1, 0, 10001 );
   } 

   //:END
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   return( 0 );
// END
} 


//:GLOBAL OPERATION
//:BuildMenuListJ( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                VIEW vDialogRoot BASED ON LOD TZWDLGSO,
//:                INTEGER lFile,
//:                STRING (50) szMenuName )

//:   STRING ( 10000 ) szWriteBuffer
zOPER_EXPORT zSHORT OPERATION
BuildMenuListJ( zVIEW     vDialog,
                zVIEW     vDialogRoot,
                zLONG     lFile,
                zPCHAR    szMenuName )
{
   zCHAR     szWriteBuffer[ 10001 ] = { 0 }; 
   //:STRING ( 500 )   szNavigationTitle
   zCHAR     szNavigationTitle[ 501 ] = { 0 }; 
   //:STRING ( 500 )   szText
   zCHAR     szText[ 501 ] = { 0 }; 
   //:STRING ( 256 )   szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 5 )     szSrch
   zCHAR     szSrch[ 6 ] = { 0 }; 
   //:STRING ( 50 )    szHTML_Name
   zCHAR     szHTML_Name[ 51 ] = { 0 }; 
   //:STRING ( 50 )    szDialogName
   zCHAR     szDialogName[ 51 ] = { 0 }; 
   //:STRING ( 50 )    szOptionTag
   zCHAR     szOptionTag[ 51 ] = { 0 }; 
   //://STRING ( 50 )    szMenuName
   //:STRING ( 34 )    szActionName
   zCHAR     szActionName[ 35 ] = { 0 }; 
   //:STRING ( 256 )   szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 )   szClass2
   zCHAR     szClass2[ 257 ] = { 0 }; 
   //:STRING ( 256 )   szHTML5Attr
   zCHAR     szHTML5Attr[ 257 ] = { 0 }; 
   //:INTEGER          ActionType
   zLONG     ActionType = 0; 
   //:SHORT            nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_2[ 255 ]; 
   zCHAR     szTempString_3[ 255 ]; 


   //:IF vDialogRoot.Menu EXISTS 
   lTempInteger_0 = CheckExistenceOfEntity( vDialogRoot, "Menu" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szMenuName = vDialogRoot.Menu.Tag 
      GetVariableFromAttribute( szMenuName, 0, 'S', 51, vDialogRoot, "Menu", "Tag", "", 0 );
      //:IF vDialogRoot.Menu.CSS_Class != ""
      if ( CompareAttributeToString( vDialogRoot, "Menu", "CSS_Class", "" ) != 0 )
      { 
         //:szClass = " class=^" + vDialogRoot.Menu.CSS_Class + "^ " 
         GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vDialogRoot, "Menu", "CSS_Class", "", 0 );
         ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
      } 

      //:END
   } 

   //:END
   //:szHTML5Attr = ""
   ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
   //:IF vDialogRoot.Menu.WebHTML5Attribute != ""
   if ( CompareAttributeToString( vDialogRoot, "Menu", "WebHTML5Attribute", "" ) != 0 )
   { 
      //:szHTML5Attr = " " + vDialogRoot.Menu.WebHTML5Attribute + " "
      GetVariableFromAttribute( szTempString_1, 0, 'S', 255, vDialogRoot, "Menu", "WebHTML5Attribute", "", 0 );
      ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
   } 

   //:END

   //:// KJS 11/05/18 - To be ADA compliant, we are supposed to use <ol not <ul. But when using jMobile and "data-role", we need to
   //:// use <ul>. Does not render correctly using <ol>.
   //:IF vDialogRoot.WndStyle.Tag = "jMobile Window"
   if ( CompareAttributeToString( vDialogRoot, "WndStyle", "Tag", "jMobile Window" ) == 0 )
   { 
      //:szWriteBuffer = "<div data-role=^listview^ id=^div" + szMenuName + "^" + szClass + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div data-role=^listview^ id=^div", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   <ul id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szHTML5Attr + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   <ul id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "<div id=^div" + szMenuName + "^" + szClass + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div id=^div", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   <ol id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szHTML5Attr + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   <ol id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
   } 

   //:END      
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

   //://szWriteBuffer = "   <ul id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szClass + ">"
   //://WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )

   //:FOR EACH vDialogRoot.OptAct WITHIN vDialogRoot.Menu
   RESULT = SetCursorFirstEntity( vDialogRoot, "OptAct", "Menu" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //://szNavigationTitle = vDialogRoot.Option.Text
      //:// KJS 09/30/16 - Language Conversion.
      //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
      //:// So for now, I am always using, if the flag is set.
      //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:szText = vDialogRoot.Option.Text
         GetVariableFromAttribute( szText, 0, 'S', 501, vDialogRoot, "Option", "Text", "", 0 );
         //:szTextID = vDialogRoot.Option.LangID
         GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialogRoot, "Option", "LangID", "", 0 );
         //:zSearchAndReplace( szText, 500, "\", "\\" )
         zSearchAndReplace( szText, 500, "\\", "\\\\" );
         //:szSrch = "\" + QUOTES 
         ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
         ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
         //:zSearchAndReplace( szText, 500, QUOTES, szSrch )
         zSearchAndReplace( szText, 500, QUOTES, szSrch );
         //:IF szTextID = ""
         if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^)%>" 
            ZeidonStringCopy( szNavigationTitle, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, szText, 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, "^)%>", 1, 0, 501 );
            //:ELSE 
         } 
         else
         { 
            //:szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^, ^" + szTextID + "^)%>" 
            ZeidonStringCopy( szNavigationTitle, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, szText, 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, "^, ^", 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, szTextID, 1, 0, 501 );
            ZeidonStringConcat( szNavigationTitle, 1, 0, "^)%>", 1, 0, 501 );
         } 

         //:END
         //://szNavigationTitle = "<%=LangConv.getLanguageText(^" + szText + "^)%>" 
         //:ELSE
      } 
      else
      { 
         //:szNavigationTitle = vDialogRoot.Option.Text
         GetVariableFromAttribute( szNavigationTitle, 0, 'S', 501, vDialogRoot, "Option", "Text", "", 0 );
      } 

      //:END
      //:szDialogName = vDialogRoot.OptAct.DialogName
      GetVariableFromAttribute( szDialogName, 0, 'S', 51, vDialogRoot, "OptAct", "DialogName", "", 0 );
      //:IF szDialogName = ""
      if ( ZeidonStringCompare( szDialogName, 1, 0, "", 1, 0, 51 ) == 0 )
      { 
         //:szDialogName = "NoDialogName"
         ZeidonStringCopy( szDialogName, 1, 0, "NoDialogName", 1, 0, 51 );
      } 

      //:END

      //://IF szReusableMenu = "Y"
      //://   szMenuName = "sm" + vDialogRoot.Option.Tag
      //://ELSE
      //:   szMenuName = vDialogRoot.Option.Tag
      GetVariableFromAttribute( szMenuName, 0, 'S', 51, vDialogRoot, "Option", "Tag", "", 0 );
      //://END

      //:// szHTML_Name = szDialogName + vDialogRoot.OptAct.WindowName + ".jsp"
      //:IF vDialogRoot.OptAct EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vDialogRoot, "OptAct" );
      if ( lTempInteger_1 == 0 )
      { 

         //:szActionName = vDialogRoot.OptAct.Tag
         GetVariableFromAttribute( szActionName, 0, 'S', 35, vDialogRoot, "OptAct", "Tag", "", 0 );
         //:ActionType = vDialogRoot.OptAct.Type
         GetIntegerFromAttribute( &ActionType, vDialogRoot, "OptAct", "Type" );
         //:ELSE
      } 
      else
      { 
         //:szActionName = ""
         ZeidonStringCopy( szActionName, 1, 0, "", 1, 0, 35 );
         //:ActionType = 0
         ActionType = 0;
      } 

      //:END

      //:// If the window we are currently generating is the same window that this
      //:// side menu points to, then we want to set a class on this option.  The
      //:// reason is so that we can tell which side menu option the user has selected.
      //://IF vDialogRoot.OptAct.WindowName = vDialog.Window.Tag
      //://   szClass2 = " class=^sideselected^ "
      //://ELSE
      //:   szClass2 = ""
      ZeidonStringCopy( szClass2, 1, 0, "", 1, 0, 257 );
      //://END

      //:IF vDialogRoot.Option.CSS_Class != "" 
      if ( CompareAttributeToString( vDialogRoot, "Option", "CSS_Class", "" ) != 0 )
      { 
         //:szClass = " class=^" + vDialogRoot.Option.CSS_Class + "^"
         GetVariableFromAttribute( szTempString_2, 0, 'S', 255, vDialogRoot, "Option", "CSS_Class", "", 0 );
         ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, szTempString_2, 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szClass = ""
         ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
      } 

      //:END

      //:szHTML5Attr = ""
      ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
      //:IF vDialogRoot.Option.WebHTML5Attribute != ""
      if ( CompareAttributeToString( vDialogRoot, "Option", "WebHTML5Attribute", "" ) != 0 )
      { 
         //:szHTML5Attr = " " + vDialogRoot.Option.WebHTML5Attribute + " "
         GetVariableFromAttribute( szTempString_3, 0, 'S', 255, vDialogRoot, "Option", "WebHTML5Attribute", "", 0 );
         ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
         ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_3, 1, 0, 257 );
         ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
      } 

      //:END

      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:szOptionTag = vDialogRoot.Option.Tag
      GetVariableFromAttribute( szOptionTag, 0, 'S', 51, vDialogRoot, "Option", "Tag", "", 0 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   csrRC = vKZXMLPGO.cursor( ^DisableMenuOption^ ).setFirst( ^MenuOptionName^, ^" + szOptionTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   csrRC = vKZXMLPGO.cursor( ^DisableMenuOption^ ).setFirst( ^MenuOptionName^, ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szOptionTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( !csrRC.isSet() ) //if ( nRC < 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( !csrRC.isSet() ) //if ( nRC < 0 )", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //://szWriteBuffer = "       <li><a href=^#^ " + szClass + " onclick=^" + szActionName + "( )^>" + szNavigationTitle + "</a></li>"
      //:// In the case of jmobile, I am putting the HTML5 and class on the <a href
      //:IF vDialogRoot.WndStyle.Tag = "jMobile Window"
      if ( CompareAttributeToString( vDialogRoot, "WndStyle", "Tag", "jMobile Window" ) == 0 )
      { 
         //:szWriteBuffer = "       <li id=^" + szMenuName + "^ name=^" + szMenuName + "^><a href=^#^ " + szHTML5Attr + szClass + szClass2 + " onclick=^" + szActionName + "()^>" + szNavigationTitle + "</a></li>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "       <li id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^><a href=^#^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass2, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "()^>", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szNavigationTitle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</a></li>", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "       <li id=^" + szMenuName + "^ name=^" + szMenuName + "^" + szHTML5Attr + szClass + "><a href=^#^ " + szClass2 + " onclick=^" + szActionName + "()^>" + szNavigationTitle + "</a></li>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "       <li id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szMenuName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "><a href=^#^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass2, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "()^>", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szNavigationTitle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</a></li>", 1, 0, 10001 );
      } 

      //:END
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
      RESULT = SetCursorNextEntity( vDialogRoot, "OptAct", "Menu" );
   } 

   //:END

   //:// Division ending for Navigation Bar.
   //:IF vDialogRoot.WndStyle.Tag = "jMobile Window"
   if ( CompareAttributeToString( vDialogRoot, "WndStyle", "Tag", "jMobile Window" ) == 0 )
   { 
      //:szWriteBuffer = "   </ul>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   </ul>", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "   </ol>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   </ol>", 1, 0, 10001 );
   } 

   //:END
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "</div> <!-- menu list end -->"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</div> <!-- menu list end -->", 1, 0, 10001 );
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 1 );
   //:szWriteBuffer = ""
   ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
   //:WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialogRoot, lFile, szWriteBuffer, "^", 0 );
   return( 0 );
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrtePushButton( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                        VIEW vGroupParent BASED ON LOD TZWDLGSO,
//:                     INTEGER          lFile,
//:                     STRING ( 10000 ) szWriteBuffer,
//:                     STRING ( 50 )    szIndent,
//:                     STRING ( 32 )    szCtrlTag,
//:                     STRING ( 1 )     szTableRowFlag,
//:                     STRING ( 20 )    szJustify,
//:                     STRING ( 1 )     szNoPositioning,
//:                     INTEGER          lOffsetX,
//:                     INTEGER          lOffsetY,
//:                     STRING ( 100 )   szRepeatGrpKey )

//:   // Format an PushButton control.

//:   VIEW vDialogRoot BASED ON LOD TZWDLGSO
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrtePushButton( zVIEW     vDialog,
                        zVIEW     vGroupParent,
                        zLONG     lFile,
                        zPCHAR    szWriteBuffer,
                        zPCHAR    szIndent,
                        zPCHAR    szCtrlTag,
                        zPCHAR    szTableRowFlag,
                        zPCHAR    szJustify,
                        zPCHAR    szNoPositioning,
                        zLONG     lOffsetX,
                        zLONG     lOffsetY,
                        zPCHAR    szRepeatGrpKey )
{
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 32 )  szContextName
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 300 ) szValue
   zCHAR     szValue[ 301 ] = { 0 }; 
   //:STRING ( 600 ) szBlob
   zCHAR     szBlob[ 601 ] = { 0 }; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szHTML5Attr
   zCHAR     szHTML5Attr[ 257 ] = { 0 }; 
   //:STRING ( 300 ) szType
   zCHAR     szType[ 301 ] = { 0 }; 
   //:STRING ( 500 ) szText
   zCHAR     szText[ 501 ] = { 0 }; 
   //:STRING ( 500 ) szText2
   zCHAR     szText2[ 501 ] = { 0 }; 
   //:STRING ( 256 ) szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 5 )   szSrch
   zCHAR     szSrch[ 6 ] = { 0 }; 
   //:STRING ( 50 )  szParentWebType
   zCHAR     szParentWebType[ 51 ] = { 0 }; 
   //:STRING ( 256 ) szTerm
   zCHAR     szTerm[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szAction
   zCHAR     szAction[ 257 ] = { 0 }; 
   //:STRING ( 600 ) szWidget
   zCHAR     szWidget[ 601 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 600 ) szEditActionCode
   zCHAR     szEditActionCode[ 601 ] = { 0 }; 
   //:STRING ( 32 )  szActionName
   zCHAR     szActionName[ 33 ] = { 0 }; 
   //:STRING ( 10 )  szMaxStringLth
   zCHAR     szMaxStringLth[ 11 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:STRING ( 1 )   szWCP_Hidden
   zCHAR     szWCP_Hidden[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szCreateSessionVariable
   zCHAR     szCreateSessionVariable[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szBrowseFile
   zCHAR     szBrowseFile[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szHyperlink
   zCHAR     szHyperlink[ 2 ] = { 0 }; 
   //:INTEGER        lMaxStringLth
   zLONG     lMaxStringLth = 0; 
   //:INTEGER        lStyleX
   zLONG     lStyleX = 0; 
   //:INTEGER        lTemp
   zLONG     lTemp = 0; 
   //:SHORT          bMapping
   zSHORT    bMapping = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 

   //:// For testing no height on input boxes
   //:STRING ( 10 )  szWidth
   zCHAR     szWidth[ 11 ] = { 0 }; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:STRING ( 100 )  szSize
   zCHAR     szSize[ 101 ] = { 0 }; 
   //:STRING ( 100 )  szPosition
   zCHAR     szPosition[ 101 ] = { 0 }; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 255 ]; 
   zCHAR     szTempString_13[ 255 ]; 
   zCHAR     szTempString_14[ 255 ]; 


   //://TraceLineS("*** GenJSPJ_CrtePushButton *** ", "")

   //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )
   GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );

   //:IF szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:CreateNoPosStyleString( vDialog, szStyle, "" )
      CreateNoPosStyleString( vDialog, szStyle, "" );
      //:// KJS 01/26/17 - Trying to take out the button height for Aadit. I'm not sure we really want to do this
      //:// but I am just going to put it in temporarily.
      //:/*
      //://CreateNoPosStyleString( vDialog, szStyle, "" )
      //:PIX_PER_DU( vDialog, dDLUnits )
      //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
      //://zIntegerToString( szHeight, 10, vDialog.Control.SZDLG_Y * dDLUnits )
      //:szStyle = "style=^width:" + szWidth + "px;^"
      //:*/
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S" // No style information
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" 
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
         //:ELSE 
      } 
      else
      { 
         //:CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
         CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" );
      } 

      //:END
   } 

   //:END
   //:   
   //:szHyperlink = ""
   ZeidonStringCopy( szHyperlink, 1, 0, "", 1, 0, 2 );
   //:szActionName = ""
   ZeidonStringCopy( szActionName, 1, 0, "", 1, 0, 33 );
   //:IF vDialog.EventAct EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDialog, "EventAct" );
   if ( lTempInteger_0 == 0 )
   { 
      //:IF vDialog.EventAct.Type = 45    // Type 45 is Hyperlink Action.
      if ( CompareAttributeToInteger( vDialog, "EventAct", "Type", 45 ) == 0 )
      { 
         //:szHyperlink = "Y"
         ZeidonStringCopy( szHyperlink, 1, 0, "Y", 1, 0, 2 );
         //:ELSE
      } 
      else
      { 
         //:szActionName = vDialog.EventAct.Tag
         GetVariableFromAttribute( szActionName, 0, 'S', 33, vDialog, "EventAct", "Tag", "", 0 );
         //:nRC = zstrcmpi( szActionName, "alt-f4" )
         nRC = zstrcmpi( szActionName, "alt-f4" );
         //:IF nRC = 0
         if ( nRC == 0 )
         { 
            //:szActionName = "AltF4"
            ZeidonStringCopy( szActionName, 1, 0, "AltF4", 1, 0, 33 );
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:CreateTabIndexString( vDialog, szTabIndex )
   CreateTabIndexString( vDialog, szTabIndex );
   //:szStyle = szStyle + szTabIndex
   ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );
   //:         
   //:// KJS 10/20/15 - If there is mapping for the button text, we need to use that.
   //:SET CURSOR FIRST vDialog.CtrlMapLOD_Attribute WITHIN vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute", "Control" );
   //:IF RESULT >= zCURSOR_SET 
   if ( RESULT >= zCURSOR_SET )
   { 
      //:bMapping = 1
      bMapping = 1;
      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      " + vDialog.CtrlMapView.Name + " = " + "task.getViewByName( ^" +
      //:                vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_0, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         task.log( ).debug( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).debug( ^Invalid View: ^ + ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         if ( nRC >= 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:IF vDialog.CtrlMapContext EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
      if ( lTempInteger_1 == 0 )
      { 
         //:szContextName = vDialog.CtrlMapContext.Name
         GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:szContextName = ""
         ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
      } 

      //:END

      //:// Add try/catch
      //:szWriteBuffer = "            try"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            try", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
      GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
      //:IF lMaxStringLth <= 0
      if ( lMaxStringLth <= 0 )
      { 
         //:szMaxStringLth = "254"
         ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
         //:ELSE
      } 
      else
      { 
         //:szMaxStringLth = lMaxStringLth
         ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
      } 

      //:END
      //:szWriteBuffer = "               strErrorMapValue = " +
      //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
      //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
      //:                szContextName + "^ );"
      GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               strErrorMapValue = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:IF vDialog.Control.WebCtrlType = "escapeHTML"
      if ( CompareAttributeToString( vDialog, "Control", "WebCtrlType", "escapeHTML" ) == 0 )
      { 
         //:szWriteBuffer = "               task.log().debug( ^" + szCtrlTag + " prior to unescape: ^ + strErrorMapValue );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               task.log().debug( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " prior to unescape: ^ + strErrorMapValue );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "               strErrorMapValue = StringEscapeUtils.unescapeHtml4( strErrorMapValue );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               strErrorMapValue = StringEscapeUtils.unescapeHtml4( strErrorMapValue );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "               task.log().debug( ^" + szCtrlTag + " after unescape: ^ + strErrorMapValue );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               task.log().debug( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " after unescape: ^ + strErrorMapValue );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:szWriteBuffer = "            }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            catch (Exception e)"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            catch (Exception e)", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               out.println(^There is an error on " + szCtrlTag + ": ^ + e.getMessage());"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               out.println(^There is an error on ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + e.getMessage());", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               task.log().error( ^*** Error on ctrl " + szCtrlTag + "^, e );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               task.log().error( ^*** Error on ctrl ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^, e );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:/***************** END OF try/catch **********************/

      //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
      //:szWriteBuffer = "            if ( strErrorMapValue == null )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( strErrorMapValue == null )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               strErrorMapValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               strErrorMapValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:szWriteBuffer = "            task.log( ).debug( ^" + vDialog.CtrlMapRelatedEntity.Name + "." +
      //:                vDialog.CtrlMapER_Attribute.Name + ": ^ + strErrorMapValue );"
      GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).debug( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + strErrorMapValue );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            task.log( ).debug( ^Entity does not exist for " + szCtrlTag + ": ^ + ^" +
      //:                vDialog.CtrlMapView.Name +
      //:                "." + vDialog.CtrlMapRelatedEntity.Name + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).debug( ^Entity does not exist for ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );

      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:   szWriteBuffer = "      if ( strErrorMapValue == ^^ )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( strErrorMapValue == ^^ )", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         strErrorMapValue = ^" + vDialog.Control.Text + "^;"
      GetVariableFromAttribute( szTempString_12, 0, 'S', 255, vDialog, "Control", "Text", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorMapValue = ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //://szText = "<%=strErrorMapValue%>"

      //:// KJS 09/30/16 - Language Conversion.
      //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
      //:// So for now, I am always using, if the flag is set.
      //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:szText = "<%=LangConv.getLanguageText(strErrorMapValue)%>" 
         ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(strErrorMapValue)%>", 1, 0, 501 );
         //:ELSE
      } 
      else
      { 
         //:szText = "<%=strErrorMapValue%>"
         ZeidonStringCopy( szText, 1, 0, "<%=strErrorMapValue%>", 1, 0, 501 );
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //://szText = vDialog.Control.Text
      //:// KJS 09/30/16 - Language Conversion.
      //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
      //:// So for now, I am always using, if the flag is set.
      //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:szText2 = vDialog.Control.Text
         GetVariableFromAttribute( szText2, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
         //:szTextID = vDialog.Control.LangID
         GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
         //:zSearchAndReplace( szText2, 1000, "\", "\\" )
         zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
         //:szSrch = "\" + QUOTES 
         ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
         ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
         //:zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
         zSearchAndReplace( szText2, 1000, QUOTES, szSrch );
         //:szSrch = ""
         ZeidonStringCopy( szSrch, 1, 0, "", 1, 0, 6 );
         //:IF szTextID = ""
         if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
            //:ELSE 
         } 
         else
         { 
            //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
         } 

         //:END
         //://szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
         //:ELSE
      } 
      else
      { 
         //:szText = vDialog.Control.Text
         GetVariableFromAttribute( szText, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
      } 

      //:END
   } 


   //:END

   //://szText = vDialog.Control.Text
   //:// Set Class as either null or with Class value.
   //:IF vDialog.Control.CSS_Class = ""
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) == 0 )
   { 
      //:szClass = ""
      ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szClass = "class=^" + vDialog.Control.CSS_Class + "^ "
      GetVariableFromAttribute( szTempString_13, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, "class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_13, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = " title=^" + szTitle + "^ "
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:szHTML5Attr = ""
   ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
   //:IF vDialog.Control.WebHTML5Attribute != ""
   if ( CompareAttributeToString( vDialog, "Control", "WebHTML5Attribute", "" ) != 0 )
   { 
      //:szHTML5Attr = " " + vDialog.Control.WebHTML5Attribute + " "
      GetVariableFromAttribute( szTempString_14, 0, 'S', 255, vDialog, "Control", "WebHTML5Attribute", "", 0 );
      ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_14, 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
   } 

   //:END

   //:szWidget = "<button type=^button^ " + szClass + szTitleHTML + szHTML5Attr + "name=^"
   ZeidonStringCopy( szWidget, 1, 0, "<button type=^button^ ", 1, 0, 601 );
   ZeidonStringConcat( szWidget, 1, 0, szClass, 1, 0, 601 );
   ZeidonStringConcat( szWidget, 1, 0, szTitleHTML, 1, 0, 601 );
   ZeidonStringConcat( szWidget, 1, 0, szHTML5Attr, 1, 0, 601 );
   ZeidonStringConcat( szWidget, 1, 0, "name=^", 1, 0, 601 );
   //:szTerm = szText + "</button>"
   ZeidonStringCopy( szTerm, 1, 0, szText, 1, 0, 257 );
   ZeidonStringConcat( szTerm, 1, 0, "</button>", 1, 0, 257 );
   //:szText = ""
   ZeidonStringCopy( szText, 1, 0, "", 1, 0, 501 );
   //:   
   //:// Action is different if this is a repeating group.
   //:IF szRepeatGrpKey = ""
   if ( ZeidonStringCompare( szRepeatGrpKey, 1, 0, "", 1, 0, 101 ) == 0 )
   { 
      //:szAction = szActionName + "( )"
      ZeidonStringCopy( szAction, 1, 0, szActionName, 1, 0, 257 );
      ZeidonStringConcat( szAction, 1, 0, "( )", 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szAction = szActionName + "( '" + szCtrlTag + szRepeatGrpKey + "' )"
      ZeidonStringCopy( szAction, 1, 0, szActionName, 1, 0, 257 );
      ZeidonStringConcat( szAction, 1, 0, "( '", 1, 0, 257 );
      ZeidonStringConcat( szAction, 1, 0, szCtrlTag, 1, 0, 257 );
      ZeidonStringConcat( szAction, 1, 0, szRepeatGrpKey, 1, 0, 257 );
      ZeidonStringConcat( szAction, 1, 0, "' )", 1, 0, 257 );
   } 

   //:END

   //:szParentWebType = ""
   ZeidonStringCopy( szParentWebType, 1, 0, "", 1, 0, 51 );
   //:// KJS 01/30/14 - I'm thinking "Relative Positioning" is old. I only use this in one pwd and when I take it
   //:// out and use "Div/No Height", the results look exactly the same so I am going to take this off grid controls.
   //:IF vGroupParent != 0
   if ( vGroupParent != 0 )
   { 
      //://*
      //:SET CURSOR FIRST vGroupParent.WebControlProperty WHERE vGroupParent.WebControlProperty.Name = "Relative Positioning"
      RESULT = SetCursorFirstEntityByString( vGroupParent, "WebControlProperty", "Name", "Relative Positioning", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:szParentWebType = "RelativePos"
         ZeidonStringCopy( szParentWebType, 1, 0, "RelativePos", 1, 0, 51 );
         //:ELSE
      } 
      else
      { 
         //:szParentWebType = vGroupParent.Control.WebCtrlType
         GetVariableFromAttribute( szParentWebType, 0, 'S', 51, vGroupParent, "Control", "WebCtrlType", "", 0 );
      } 

      //:END
   } 

   //:   //*/
   //:   //szParentWebType = vGroupParent.Control.WebCtrlType
   //:END


   //:// KJS 05/18/17 - Trying... if button has an action of link html, then we will use
   //:// the mapping as the link text.
   //:IF szHyperlink = "Y"
   if ( ZeidonStringCompare( szHyperlink, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 

      //://<button class="buttonlinkdirections"  data-theme="b" data-icon="navigation" data-inline="true" data-iconpos="right" ><a href="<%=strTextURL_Value%>" target="_blank">Directions2</a></button>
      //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:szText2 = vDialog.Control.Text
         GetVariableFromAttribute( szText2, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
         //:szTextID = vDialog.Control.LangID
         GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
         //:zSearchAndReplace( szText2, 1000, "\", "\\" )
         zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
         //:szSrch = "\" + QUOTES 
         ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
         ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
         //:zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
         zSearchAndReplace( szText2, 1000, QUOTES, szSrch );
         //:szSrch = ""
         ZeidonStringCopy( szSrch, 1, 0, "", 1, 0, 6 );
         //:IF szTextID = ""
         if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
            //:ELSE 
         } 
         else
         { 
            //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:szText = vDialog.Control.Text
         GetVariableFromAttribute( szText, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
      } 

      //:END
      //:szWriteBuffer = szWidget + szCtrlTag + "^ id=^" + szCtrlTag + "^" + szStyle + " ><a href=^<%=strErrorMapValue%>^ target=^_blank^>" + 
      //:                szText + "</a></button>"    
      ZeidonStringCopy( szWriteBuffer, 1, 0, szWidget, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ><a href=^<%=strErrorMapValue%>^ target=^_blank^>", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "</a></button>", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:IF szParentWebType = "Div" OR szParentWebType = "DivScroll" OR
      //:szParentWebType = "Span"
      if ( ZeidonStringCompare( szParentWebType, 1, 0, "Div", 1, 0, 51 ) == 0 || ZeidonStringCompare( szParentWebType, 1, 0, "DivScroll", 1, 0, 51 ) == 0 || ZeidonStringCompare( szParentWebType, 1, 0, "Span", 1, 0, 51 ) == 0 )
      { 

         //:szWriteBuffer = "<p></p>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<p></p>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:CreateSizeString( vDialog, szSize )
         CreateSizeString( vDialog, szSize );
         //:szWriteBuffer = szWidget + szCtrlTag + "^ id=^" + szCtrlTag + "^ value=^" + szText + "^ onclick=^" +
         //:             szAction + "^ " + "style=^" + szSize + "^>" + szTerm
         ZeidonStringCopy( szWriteBuffer, 1, 0, szWidget, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ value=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ onclick=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szAction, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "style=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSize, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^>", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTerm, 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = szWidget + szCtrlTag + "^ id=^" + szCtrlTag + "^ value=^" + szText + "^ onclick=^" +
         //:             szAction + "^ " + szStyle + ">" + szTerm
         ZeidonStringCopy( szWriteBuffer, 1, 0, szWidget, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ value=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ onclick=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szAction, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTerm, 1, 0, 10001 );
      } 

      //:END
   } 

   //:END
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteMLEdit( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                     INTEGER          lFile,
//:                     STRING ( 10000 ) szWriteBuffer,
//:                     STRING ( 50 )    szIndent,
//:                     STRING ( 32 )    szCtrlTag,
//:                     STRING ( 1 )     szTableRowFlag,
//:                     STRING ( 20 )    szJustify,
//:                     STRING ( 1 )     szNoPositioning,
//:                     INTEGER          lOffsetX,
//:                     INTEGER          lOffsetY,
//:                     STRING ( 100 )   szRepeatGrpKey )

//:   VIEW vLPLR       BASED ON LOD TZCMLPLO
zOPER_EXPORT zSHORT OPERATION
GenJSPJ_CrteMLEdit( zVIEW     vDialog,
                    zLONG     lFile,
                    zPCHAR    szWriteBuffer,
                    zPCHAR    szIndent,
                    zPCHAR    szCtrlTag,
                    zPCHAR    szTableRowFlag,
                    zPCHAR    szJustify,
                    zPCHAR    szNoPositioning,
                    zLONG     lOffsetX,
                    zLONG     lOffsetY,
                    zPCHAR    szRepeatGrpKey )
{
   zVIEW     vLPLR = 0; 
   //:VIEW vDialogRoot BASED ON LOD TZWDLGSO
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 32 )  szLPLR_Name
   zCHAR     szLPLR_Name[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szContextName
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szSize
   zCHAR     szSize[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szWidth
   zCHAR     szWidth[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szHeight
   zCHAR     szHeight[ 51 ] = { 0 }; 
   //:STRING ( 300 ) szAbsoluteStyle
   zCHAR     szAbsoluteStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 50 )  szTabIndex
   zCHAR     szTabIndex[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 500 ) szActionCode
   zCHAR     szActionCode[ 501 ] = { 0 }; 
   //:STRING ( 256 ) szText
   zCHAR     szText[ 257 ] = { 0 }; 
   //:STRING ( 10 )  szMaxStringLth
   zCHAR     szMaxStringLth[ 11 ] = { 0 }; 
   //:STRING ( 1 )   szTinyMCEFlag
   zCHAR     szTinyMCEFlag[ 2 ] = { 0 }; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:INTEGER        X_Size
   zLONG     X_Size = 0; 
   //:INTEGER        Y_Size
   zLONG     Y_Size = 0; 
   //:INTEGER        Size
   zLONG     Size = 0; 
   //:INTEGER        lMaxStringLth
   zLONG     lMaxStringLth = 0; 
   //:INTEGER        lStyleX
   zLONG     lStyleX = 0; 
   //:INTEGER        lTemp
   zLONG     lTemp = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 33 ]; 
   zCHAR     szTempString_13[ 255 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_14[ 255 ]; 


   //://TraceLineS("**** GenJSPJ_CrteMLEdit **** ", "")

   //:SET CURSOR FIRST vDialog.CtrlMapLOD_Attribute WITHIN vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute", "Control" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   // " + szControlType + ": " + szCtrlTag
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   // ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szControlType, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   strErrorMapValue = VmlOperation.CheckError( ^" + szCtrlTag + "^, strError );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   strErrorMapValue = VmlOperation.CheckError( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^, strError );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( !StringUtils.isBlank( strErrorMapValue ) )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( !StringUtils.isBlank( strErrorMapValue ) )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( StringUtils.equals( strErrorFlag, ^Y^ ) )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( StringUtils.equals( strErrorFlag, ^Y^ ) )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         strErrorColor = ^color:red;^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorColor = ^color:red;^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      strErrorColor = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      strErrorColor = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      " + vDialog.CtrlMapView.Name + " = " + "task.getViewByName( ^" +
      //:                vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_0, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         task.log( ).info( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).info( ^Invalid View: ^ + ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
      GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );
      //:// KJS 03/13/18 - If the view is read only, then we want to set the combo box to disabled.
      //:// We only do this if the zeidon.ini has UseZeidonControlActionsView
      //:IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y" 
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesControlActionsView", "Y" ) == 0 )
      { 
         //:szWriteBuffer = "         strDisabled = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strDisabled = ^^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         if ( " + vDialog.CtrlMapView.Name + ".isReadOnly( ) )"
         GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".isReadOnly( ) )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "            strDisabled = ^disabled^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            strDisabled = ^disabled^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END      

      //:szWriteBuffer = "         nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         if ( nRC >= 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:IF vDialog.CtrlMapContext EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
      if ( lTempInteger_0 == 0 )
      { 
         //:szContextName = vDialog.CtrlMapContext.Name
         GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:szContextName = ""
         ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
      } 

      //:END

      //:lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
      GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
      //:IF lMaxStringLth <= 0
      if ( lMaxStringLth <= 0 )
      { 
         //:szMaxStringLth = "254"
         ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
         //:ELSE
      } 
      else
      { 
         //:szMaxStringLth = lMaxStringLth
         ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
      } 

      //:END

      //:szWriteBuffer = "            strErrorMapValue = " +
      //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
      //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
      //:                szContextName + "^ );"
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            strErrorMapValue = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
      //:szWriteBuffer = "            if ( strErrorMapValue == null )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( strErrorMapValue == null )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               strErrorMapValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               strErrorMapValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:szWriteBuffer = "            task.log( ).info( ^" + vDialog.CtrlMapRelatedEntity.Name + "." +
      //:                vDialog.CtrlMapER_Attribute.Name + ": ^ + strErrorMapValue );"
      GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).info( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + strErrorMapValue );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            task.log( ).info( ^Entity does not exist for " + szCtrlTag + ": ^ + ^" +
      //:                vDialog.CtrlMapView.Name +
      //:                "." + vDialog.CtrlMapRelatedEntity.Name + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).info( ^Entity does not exist for ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_12, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:lStyleX = vDialog.Control.ExtendedStyle
      GetIntegerFromAttribute( &lStyleX, vDialog, "Control", "ExtendedStyle" );
      //:lTemp = IsFlagSequenceSet( lStyleX, zCONTROLX_PREFILL )
      lTemp = IsFlagSequenceSet( lStyleX, zCONTROLX_PREFILL );
      //:IF lTemp != 0
      if ( lTemp != 0 )
      { 
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
         //:szWriteBuffer = "      if ( strErrorMapValue.length( ) == 0 )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( strErrorMapValue.length( ) == 0 )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         strErrorMapValue = ^" + vDialog.Control.Text + "^;"
         GetVariableFromAttribute( szTempString_13, 0, 'S', 255, vDialog, "Control", "Text", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorMapValue = ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_13, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^;", 1, 0, 10001 );
      } 

      //:END

      //:// This doesn't work, but something like it may be necessary to remove special characters ... dks 2012.04.25
      //:// strErrorMapValue = strErrorMapValue.replace("/&/g", "&amp;").replace("/>/g", "&gt;").replace("/</g", "&lt;").replace("/\"/g", "&quot;").replace("/%/g", "&#037;");

      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   } 


   //:END // SET CURSOR FIRST vDialog.CtrlMapLOD_Attribute WITHIN vDialog.Control

   //:// *********************  NEW ******************

   //:// If there is an Event for the MLEdit, build code to insert in <textarea statement.
   //:szActionCode = ""
   ZeidonStringCopy( szActionCode, 1, 0, "", 1, 0, 501 );
   //:FOR EACH vDialog.Event
   RESULT = SetCursorFirstEntity( vDialog, "Event", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vDialog.EventAct EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vDialog, "EventAct" );
      if ( lTempInteger_1 == 0 )
      { 
         //:szActionName = vDialog.EventAct.Tag
         GetVariableFromAttribute( szActionName, 0, 'S', 51, vDialog, "EventAct", "Tag", "", 0 );
         //:IF vDialog.Event.Type = 32
         if ( CompareAttributeToInteger( vDialog, "Event", "Type", 32 ) == 0 )
         { 
            //:szActionCode = szActionCode + " onfocus=^" + szActionName + "( )^ "
            ZeidonStringConcat( szActionCode, 1, 0, " onfocus=^", 1, 0, 501 );
            ZeidonStringConcat( szActionCode, 1, 0, szActionName, 1, 0, 501 );
            ZeidonStringConcat( szActionCode, 1, 0, "( )^ ", 1, 0, 501 );
            //:ELSE
         } 
         else
         { 
            //:IF vDialog.Event.Type = 64
            if ( CompareAttributeToInteger( vDialog, "Event", "Type", 64 ) == 0 )
            { 
               //:szActionCode = szActionCode + " onblur=^" + szActionName + "( )^ "
               ZeidonStringConcat( szActionCode, 1, 0, " onblur=^", 1, 0, 501 );
               ZeidonStringConcat( szActionCode, 1, 0, szActionName, 1, 0, 501 );
               ZeidonStringConcat( szActionCode, 1, 0, "( )^ ", 1, 0, 501 );
               //:ELSE
            } 
            else
            { 
               //:IF vDialog.Event.Type = 1024
               if ( CompareAttributeToInteger( vDialog, "Event", "Type", 1024 ) == 0 )
               { 
                  //:szActionCode = szActionCode + " onchange=^" + szActionName + "( )^ "
                  ZeidonStringConcat( szActionCode, 1, 0, " onchange=^", 1, 0, 501 );
                  ZeidonStringConcat( szActionCode, 1, 0, szActionName, 1, 0, 501 );
                  ZeidonStringConcat( szActionCode, 1, 0, "( )^ ", 1, 0, 501 );
                  //:ELSE
               } 
               else
               { 
                  //:IF vDialog.Event.Type = 1
                  if ( CompareAttributeToInteger( vDialog, "Event", "Type", 1 ) == 0 )
                  { 
                     //:szActionCode = szActionCode + " onkeydown=^" + szActionName + "( )^ "
                     ZeidonStringConcat( szActionCode, 1, 0, " onkeydown=^", 1, 0, 501 );
                     ZeidonStringConcat( szActionCode, 1, 0, szActionName, 1, 0, 501 );
                     ZeidonStringConcat( szActionCode, 1, 0, "( )^ ", 1, 0, 501 );
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( vDialog, "Event", "" );
      //:END
   } 

   //:END

   //:// *********************  END NEW ******************

   //:IF szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:CreateNoPosStyleString( vDialog, szAbsoluteStyle, "" )
      CreateNoPosStyleString( vDialog, szAbsoluteStyle, "" );
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S" // No style information
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szAbsoluteStyle = "" 
         ZeidonStringCopy( szAbsoluteStyle, 1, 0, "", 1, 0, 301 );
         //:ELSE 
      } 
      else
      { 
         //:CreateAbsolStyleString( vDialog, szAbsoluteStyle, lOffsetX, lOffsetY, "" )
         CreateAbsolStyleString( vDialog, szAbsoluteStyle, lOffsetX, lOffsetY, "" );
      } 

      //:END
   } 

   //:END

   //:/* 10d code Doug has
   //:PIX_PER_DU( vDialog, dDLUnits )
   //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
   //:zIntegerToString( szHeight, 10, vDialog.Control.SZDLG_Y * dDLUnits )
   //:*/

   //:IF vDialog.Control.VisibleBorder = "Y" AND  szNoPositioning != "S"
   if ( CompareAttributeToString( vDialog, "Control", "VisibleBorder", "Y" ) == 0 && ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) != 0 )
   { 
      //:zAppendQuotedString( szAbsoluteStyle, "border:solid;border-width:4px;border-style:groove;", "style=", "^" )
      zAppendQuotedString( szAbsoluteStyle, "border:solid;border-width:4px;border-style:groove;", "style=", "^" );
   } 

   //:// KJS 01/26/17 - Taking out the border, doesn't seem to really be doing anything and it conflicts with css.
   //://ELSE
   //://   zAppendQuotedString( szStyle, "border:solid;border-width:2px;border-style:groove;", "style=", "^" )
   //:END

   //:szTinyMCEFlag = ""
   ZeidonStringCopy( szTinyMCEFlag, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "wysiwyg TinyMCE"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "wysiwyg TinyMCE", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szTinyMCEFlag = "Y"
      ZeidonStringCopy( szTinyMCEFlag, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //:// dks 2016.04.08 - trying placeholder
   //:// If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = " title=^" + szTitle + "^"
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^", 1, 0, 257 );
   } 

   //:END
   //:szTitle = vDialog.Control.WebPlaceholder
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "WebPlaceholder", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = szTitleHTML + " placeholder=^" + szTitle + "^"
      ZeidonStringConcat( szTitleHTML, 1, 0, " placeholder=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^", 1, 0, 257 );
   } 

   //:END
   //://szTitle = vDialog.Control.Placeholder
   //:IF szTitle = "" AND vDialog.Control.Placeholder != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) == 0 && CompareAttributeToString( vDialog, "Control", "Placeholder", "" ) != 0 )
   { 
      //:szTitleHTML = szTitleHTML + " placeholder=^" + szTitle + "^"
      ZeidonStringConcat( szTitleHTML, 1, 0, " placeholder=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^", 1, 0, 257 );
   } 

   //:END

   //:IF vDialog.Control.WebCtrlType = "wysiwygEditor" OR szTinyMCEFlag = "Y"
   if ( CompareAttributeToString( vDialog, "Control", "WebCtrlType", "wysiwygEditor" ) == 0 || ZeidonStringCompare( szTinyMCEFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 

      //:// CreateTabIndexString( vDialog, szTabIndex )
      //:// CreateAbsolStyleString( vDialog, szAbsoluteStyle, lOffsetX, lOffsetY, "" )
      //:// szAbsoluteStyle = szAbsoluteStyle + szTabIndex

      //:szClass = vDialog.Control.CSS_Class
      GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
      //:IF szClass = ""
      if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
      { 
         //:GetViewByName( vLPLR, "TaskLPLR", vDialog, zLEVEL_TASK )
         GetViewByName( &vLPLR, "TaskLPLR", vDialog, zLEVEL_TASK );
         //:szText = vLPLR.LPLR.Name
         GetVariableFromAttribute( szText, 0, 'S', 257, vLPLR, "LPLR", "Name", "", 0 );
         //:szLPLR_Name = "[App." + szText + "]"
         ZeidonStringCopy( szLPLR_Name, 1, 0, "[App.", 1, 0, 33 );
         ZeidonStringConcat( szLPLR_Name, 1, 0, szText, 1, 0, 33 );
         ZeidonStringConcat( szLPLR_Name, 1, 0, "]", 1, 0, 33 );
         //:SysReadZeidonIni( -1, szLPLR_Name, "TinyMCEClass", szClass )
         SysReadZeidonIni( -1, szLPLR_Name, "TinyMCEClass", szClass, zsizeof( szClass ) );
         //:IF szClass = ""
         if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:szClass = "mceSimple"  // "mceSimple" is TinyMCE default
            ZeidonStringCopy( szClass, 1, 0, "mceSimple", 1, 0, 257 );
         } 

         //:END
      } 

      //:END

      //:nRC = zSearchSubString( szClass, "mceSimpleZeidon", "f", 0 )
      nRC = zSearchSubString( szClass, "mceSimpleZeidon", "f", 0 );
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:szWriteBuffer = "<div style=^background-color:#eee;border:1px solid #042;width:" + szWidth + "px;height:" + szHeight + "px;position:absolute;left:0px;top:0px;overflow:auto;^>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<div style=^background-color:#eee;border:1px solid #042;width:", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szWidth, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "px;height:", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szHeight, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "px;position:absolute;left:0px;top:0px;overflow:auto;^>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "<div class=^" + szClass + "^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ style=^width:" + szWidth + "px;height:" + szHeight + "px;position:absolute;left:0px;top:0px;^><%=strErrorMapValue%></div></div>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<div class=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ style=^width:", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szWidth, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "px;height:", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szHeight, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "px;position:absolute;left:0px;top:0px;^><%=strErrorMapValue%></div></div>", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "<textarea name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey +
         //:                "^ <%=strDisabled%> class=^" + szClass + "^ " + szTitleHTML + szAbsoluteStyle + szActionCode + ">" +
         //:             // "^ rows="15" cols="80" style="width: 80%">" +
         //:                "<%=strErrorMapValue%></textarea>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<textarea name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> class=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "<%=strErrorMapValue%></textarea>", 1, 0, 10001 );
      } 

      //:END

      //:ELSE
   } 
   else
   { 
      //:szClass = ""
      ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
      //:IF vDialog.Control.CSS_Class != ""
      if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) != 0 )
      { 
         //:szClass = vDialog.Control.CSS_Class
         GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
         //:szClass = " class=^" + vDialog.Control.CSS_Class + "^"
         GetVariableFromAttribute( szTempString_14, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
         ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, szTempString_14, 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
      } 

      //:END
      //:CreateTabIndexString( vDialog, szTabIndex )
      CreateTabIndexString( vDialog, szTabIndex );
      //:szWriteBuffer = "<textarea name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> " + szTitleHTML +
      //:                szAbsoluteStyle + szTabIndex + szClass + szActionCode + " wrap=^wrap^>" +
      //:                "<%=strErrorMapValue%></textarea>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<textarea name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTabIndex, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szActionCode, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " wrap=^wrap^>", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "<%=strErrorMapValue%></textarea>", 1, 0, 10001 );
   } 


   //:END
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   return( 0 );
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteListBox( VIEW vDialog,
//:                    INTEGER lFile,
//:                    STRING ( 10000 ) szWriteBuffer,
//:                    STRING ( 50 )    szIndent,
//:                    STRING ( 32 )    szCtrlTag,
//:                    STRING ( 1 )     szNoPositioning,
//:                    INTEGER lOffsetX,
//:                    INTEGER lOffsetY )

//:   VIEW vScope  BASED ON LOD TZWDLGSO  // identifies scoping entity for list entries
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteListBox( zVIEW     vDialog,
                     zLONG     lFile,
                     zPCHAR    szWriteBuffer,
                     zPCHAR    szIndent,
                     zPCHAR    szCtrlTag,
                     zPCHAR    szNoPositioning,
                     zLONG     lOffsetX,
                     zLONG     lOffsetY )
{
   zVIEW     vScope = 0; 
   //:STRING ( 32 )  szContextName
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szScopingEntityName
   zCHAR     szScopingEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szListEntityName
   zCHAR     szListEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szAttributeName
   zCHAR     szAttributeName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szEntityName
   zCHAR     szEntityName[ 33 ] = { 0 }; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) ListBoxDoubleClickAction
   zCHAR     ListBoxDoubleClickAction[ 257 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 10 )  szMaxStringLth
   zCHAR     szMaxStringLth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szWidth
   zCHAR     szWidth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szSelectAction
   zCHAR     szSelectAction[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szSelectFunction
   zCHAR     szSelectFunction[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szHeight
   zCHAR     szHeight[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szX_Pos
   zCHAR     szX_Pos[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szY_Pos
   zCHAR     szY_Pos[ 11 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:INTEGER        lSubtype
   zLONG     lSubtype = 0; 
   //:INTEGER        lMaxStringLth
   zLONG     lMaxStringLth = 0; 
   zCHAR     szTempString_0[ 1026 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_4; 
   zCHAR     szTempString_1[ 1026 ]; 
   zSHORT    lTempInteger_5; 
   zSHORT    lTempInteger_6; 
   zCHAR     szTempString_2[ 1026 ]; 
   zCHAR     szTempString_3[ 1026 ]; 
   zCHAR     szTempString_4[ 1026 ]; 
   zSHORT    lTempInteger_7; 
   zCHAR     szTempString_5[ 1026 ]; 
   zSHORT    lTempInteger_8; 
   zCHAR     szTempString_6[ 1026 ]; 
   zSHORT    lTempInteger_9; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 1026 ]; 


   //:szStyle = "width:" + szWidth + "px; height:" + szHeight + "px;"
   ZeidonStringCopy( szStyle, 1, 0, "width:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px; height:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szHeight, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );
   //:IF vDialog.Control.CSS_Class = ""
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) == 0 )
   { 
      //:szClass = ""
      ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szClass = "class=^" + vDialog.Control.CSS_Class + "^"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 1026, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, "class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
   } 

   //:END

   //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = " title=^" + szTitle + "^ "
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:CreateTabIndexString( vDialog, szTabIndex )
   CreateTabIndexString( vDialog, szTabIndex );
   //:// We will set the height to the constant 20 px.
   //:PIX_PER_DU( vDialog, dDLUnits )
   PIX_PER_DU( vDialog, &dDLUnits );
   //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
   GetIntegerFromAttribute( &lTempInteger_0, vDialog, "Control", "SZDLG_X" );
   zIntegerToString( szWidth, 10, lTempInteger_0 * dDLUnits );
   //:zIntegerToString( szHeight, 10, vDialog.Control.SZDLG_Y * dDLUnits )
   GetIntegerFromAttribute( &lTempInteger_1, vDialog, "Control", "SZDLG_Y" );
   zIntegerToString( szHeight, 10, lTempInteger_1 * dDLUnits );
   //:szStyle = "style=^width:" + szWidth + "px;height:" + szHeight + "px;"
   ZeidonStringCopy( szStyle, 1, 0, "style=^width:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px;height:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szHeight, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );

   //:// If szNoPositioning is null then we are using absolute positioning on controls.
   //:IF szNoPositioning = ""
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
   { 

      //:zIntegerToString( szX_Pos, 10, vDialog.Control.PSDLG_X * dDLUnits )
      GetIntegerFromAttribute( &lTempInteger_2, vDialog, "Control", "PSDLG_X" );
      zIntegerToString( szX_Pos, 10, lTempInteger_2 * dDLUnits );
      //:zIntegerToString( szY_Pos, 10, vDialog.Control.PSDLG_Y * dDLUnits )
      GetIntegerFromAttribute( &lTempInteger_3, vDialog, "Control", "PSDLG_Y" );
      zIntegerToString( szY_Pos, 10, lTempInteger_3 * dDLUnits );
      //:szStyle = szStyle + "position:absolute;" + "left:" + szX_Pos + "px;" + "top:" + szY_Pos + "px;"
      ZeidonStringConcat( szStyle, 1, 0, "position:absolute;", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "left:", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, szX_Pos, 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "top:", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, szY_Pos, 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );
      //://CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" 
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
      } 

      //:END
   } 

   //:END

   //:szStyle = szStyle + "^ " + szTabIndex
   ZeidonStringConcat( szStyle, 1, 0, "^ ", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );

   //:szWriteBuffer = "<% strErrorMapValue = ^^;  %>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strErrorMapValue = ^^;  %>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:szSelectFunction = "OnClick"
   ZeidonStringCopy( szSelectFunction, 1, 0, "OnClick", 1, 0, 11 );


   //:   // Check if DoubleClick Event and Action exist and handle accordingly.
   //:   ListBoxDoubleClickAction = ""
   ZeidonStringCopy( ListBoxDoubleClickAction, 1, 0, "", 1, 0, 257 );
   //:   SET CURSOR FIRST vDialog.Event WHERE vDialog.Event.Type = 2
   RESULT = SetCursorFirstEntityByInteger( vDialog, "Event", "Type", 2, "" );
   //:   IF RESULT >= zCURSOR_SET AND vDialog.EventAct EXISTS
   lTempInteger_4 = CheckExistenceOfEntity( vDialog, "EventAct" );
   if ( RESULT >= zCURSOR_SET && lTempInteger_4 == 0 )
   { 
      //:   ListBoxDoubleClickAction = " ondblclick=^" + vDialog.EventAct.Tag + "( )^ "
      GetVariableFromAttribute( szTempString_1, 0, 'S', 1026, vDialog, "EventAct", "Tag", "", 0 );
      ZeidonStringCopy( ListBoxDoubleClickAction, 1, 0, " ondblclick=^", 1, 0, 257 );
      ZeidonStringConcat( ListBoxDoubleClickAction, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( ListBoxDoubleClickAction, 1, 0, "( )^ ", 1, 0, 257 );
   } 

   //:   END


   //:   // Set cursor to the scoping entity if one exists
   //:   CreateViewFromViewForTask( vScope, vDialog, 0 )
   CreateViewFromViewForTask( &vScope, vDialog, 0 );
   //:   SET CURSOR NEXT vScope.CtrlMap
   RESULT = SetCursorNextEntity( vScope, "CtrlMap", "" );

   //:   szWriteBuffer = "<select " + szClass + " name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ size=^4^ " + szTitleHTML +
   //:                   szStyle + szDisabled + " onclick=^" + szCtrlTag + szSelectFunction + "( )^" + ListBoxDoubleClickAction + ">"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<select ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " name=^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ size=^4^ ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szSelectFunction, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "( )^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, ListBoxDoubleClickAction, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:   szWriteBuffer = "<%"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:   IF vDialog.CtrlMapView EXISTS
   lTempInteger_5 = CheckExistenceOfEntity( vDialog, "CtrlMapView" );
   if ( lTempInteger_5 == 0 )
   { 


      //:   // See if a list entity has been specified, and if so, use it.
      //:   // Since this is a listbox I believe this should always be specified.
      //:   IF vDialog.CtrlMapLOD_Entity EXISTS
      lTempInteger_6 = CheckExistenceOfEntity( vDialog, "CtrlMapLOD_Entity" );
      if ( lTempInteger_6 == 0 )
      { 
         //:   szListEntityName = vDialog.CtrlMapLOD_Entity.Name
         GetVariableFromAttribute( szListEntityName, 0, 'S', 33, vDialog, "CtrlMapLOD_Entity", "Name", "", 0 );
      } 

      //:   END

      //:   szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
      //:                   "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_2, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_3, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) )"
      GetVariableFromAttribute( szTempString_4, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      strComboCurrentValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      strComboCurrentValue = ^^;", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      View v" + szCtrlTag + ";"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      View v", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ";", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:   szAttributeName = ""
      ZeidonStringCopy( szAttributeName, 1, 0, "", 1, 0, 33 );
      //:   szEntityName = ""
      ZeidonStringCopy( szEntityName, 1, 0, "", 1, 0, 33 );
      //:   szContextName = ""
      ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );

      //:   // Set position on the text field inside the listbox.
      //:   SetViewToSubobject( vDialog, "CtrlCtrl" )
      SetViewToSubobject( vDialog, "CtrlCtrl" );
      //:   IF vDialog.CtrlMapLOD_Attribute EXISTS
      lTempInteger_7 = CheckExistenceOfEntity( vDialog, "CtrlMapLOD_Attribute" );
      if ( lTempInteger_7 == 0 )
      { 

         //:   szAttributeName = vDialog.CtrlMapER_Attribute.Name
         GetVariableFromAttribute( szAttributeName, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
         //:   szEntityName = vDialog.CtrlMapRelatedEntity.Name
         GetVariableFromAttribute( szEntityName, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );

         //:   szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
         //:                   szEntityName + "^ ).checkExistenceOfEntity( ).toInt();"
         GetVariableFromAttribute( szTempString_5, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   szWriteBuffer = "      if ( nRC >= 0 )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   szWriteBuffer = "      {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   IF vDialog.CtrlMapContext EXISTS
         lTempInteger_8 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
         if ( lTempInteger_8 == 0 )
         { 
            //:   szContextName = vDialog.CtrlMapContext.Name
            GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
         } 

         //:   END

         //:   lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
         GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
         //:   IF lMaxStringLth <= 0
         if ( lMaxStringLth <= 0 )
         { 
            //:   szMaxStringLth = "254"
            ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
            //:ELSE
         } 
         else
         { 
            //:   szMaxStringLth = lMaxStringLth
            ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
         } 

         //:   END

         //:   szWriteBuffer = "         strComboCurrentValue = " + vDialog.CtrlMapView.Name +
         //:            ".cursor( ^" +
         //:            szEntityName + "^ ).getAttribute( ^" +
         //:            szAttributeName + "^ ).getString( ^" + szContextName + "^ );"
         GetVariableFromAttribute( szTempString_6, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strComboCurrentValue = ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szAttributeName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:   // KJS 02/15/11 - Thinking I need to add code to change values returned as null to ""
         //:   szWriteBuffer = "         if ( strComboCurrentValue == null )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( strComboCurrentValue == null )", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   szWriteBuffer = "            strComboCurrentValue = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            strComboCurrentValue = ^^;", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   szWriteBuffer = "      }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

         //:ELSE
      } 
      else
      { 
         //:   szWriteBuffer = "      strComboCurrentValue = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strComboCurrentValue = ^^;", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      } 

      //:   END
      //:   ResetViewFromSubobject( vDialog )
      ResetViewFromSubobject( vDialog );

      //:   // See if a scoping entity has been specified, and if so, use it.
      //:   szScopingEntityName = ""
      ZeidonStringCopy( szScopingEntityName, 1, 0, "", 1, 0, 33 );
      //:   lSubtype = vDialog.Control.Subtype
      GetIntegerFromAttribute( &lSubtype, vDialog, "Control", "Subtype" );
      //:   lSubtype = IsFlagSequenceSet( lSubtype, zLISTBOX_SCOPE_OI )
      lSubtype = IsFlagSequenceSet( lSubtype, zLISTBOX_SCOPE_OI );
      //:   IF lSubtype = 0
      if ( lSubtype == 0 )
      { 
         //:   IF vScope.CtrlMapLOD_Entity EXISTS
         lTempInteger_9 = CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" );
         if ( lTempInteger_9 == 0 )
         { 
            //:   szScopingEntityName = "^" + vScope.CtrlMapLOD_Entity.Name + "^"
            GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vScope, "CtrlMapLOD_Entity", "Name", "", 0 );
            ZeidonStringCopy( szScopingEntityName, 1, 0, "^", 1, 0, 33 );
            ZeidonStringConcat( szScopingEntityName, 1, 0, szTempString_7, 1, 0, 33 );
            ZeidonStringConcat( szScopingEntityName, 1, 0, "^", 1, 0, 33 );
         } 

         //:   END
      } 

      //:   END

      //:   szWriteBuffer = "      v" + szCtrlTag + " = " + vDialog.CtrlMapView.Name + ".newView( );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      v", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_8, 0, 'S', 1026, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".newView( );", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:   szWriteBuffer = "      ComboCount = 0;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ComboCount = 0;", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      strComboSelectedValue = ^0^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      strComboSelectedValue = ^0^;", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:   IF lSubtype = 0
      if ( lSubtype == 0 )
      { 
         //:   szWriteBuffer = "      csrRC = v" + szCtrlTag + ".cursor( ^" +
         //:                                szListEntityName + "^ ).setFirst( " + szScopingEntityName + " );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      csrRC = v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirst( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szScopingEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " );", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:   szWriteBuffer = "      csrRC = v" + szCtrlTag + ".cursor( ^" + szListEntityName + "^ ).setFirstWithinOi( );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      csrRC = v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirstWithinOi( );", 1, 0, 10001 );
      } 

      //:   END

      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      while ( csrRC.isSet() )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      while ( csrRC.isSet() )", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:   IF szAttributeName != ""
      if ( ZeidonStringCompare( szAttributeName, 1, 0, "", 1, 0, 33 ) != 0 )
      { 

         //:   szWriteBuffer = "         strErrorMapValue = v" + szCtrlTag +
         //:                   ".cursor( ^" +
         //:                   szEntityName + "^ ).getAttribute( ^" +
         //:                   szAttributeName + "^ ).getString( ^" +
         //:                   szContextName + "^ );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorMapValue = v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szAttributeName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:   // KJS 02/15/11 - Thinking I need to add code to change values returned as null to ""
         //:   szWriteBuffer = "         if ( strErrorMapValue == null )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( strErrorMapValue == null )", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:   szWriteBuffer = "            strErrorMapValue = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            strErrorMapValue = ^^;", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      } 


      //:   END

      //:   szWriteBuffer = "         if ( StringUtils.equals( strComboCurrentValue, strErrorMapValue ) )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( StringUtils.equals( strComboCurrentValue, strErrorMapValue ) )", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "            <option selected=^selected^><%=strErrorMapValue%></option>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            <option selected=^selected^><%=strErrorMapValue%></option>", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "            strComboSelectedValue = Integer.toString( ComboCount );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            strComboSelectedValue = Integer.toString( ComboCount );", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "            <option><%=strErrorMapValue%></option>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            <option><%=strErrorMapValue%></option>", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:   szWriteBuffer = "         ComboCount++;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         ComboCount++;", 1, 0, 10001 );
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:// IF lSubtype = 0  now always do setNextContinue here for performance reasons dks/dgc 2011.05.03
      //:      szWriteBuffer = "         csrRC =  v" + szCtrlTag +
      //:                      ".cursor( ^" + szListEntityName +
      //:                      "^ ).setNextContinue( );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         csrRC =  v", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setNextContinue( );", 1, 0, 10001 );
      //:                      //"^ ).setNext( " + szScopingEntityName + " );"
      //:// ELSE
      //://    szWriteBuffer = "         nRC =  v" + szCtrlTag +
      //://                    ".cursor( ^" + szListEntityName + "^ ).setNextWithinOi( );"
      //:// END

      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:   DropView( vScope )
      DropView( vScope );
   } 

   //:   END

   //:   szWriteBuffer = "      }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   //:   szWriteBuffer = "      v" + szCtrlTag + ".drop( );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      v", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, ".drop( );", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:   szWriteBuffer = "   }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:   szWriteBuffer = "%>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:   szWriteBuffer = "<input name=^h" + szCtrlTag + "^ id=^h" + szCtrlTag + "^ type=^hidden^ value=^<%=strComboSelectedValue%>^ >"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<input name=^h", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^h", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ type=^hidden^ value=^<%=strComboSelectedValue%>^ >", 1, 0, 10001 );
   //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:szWriteBuffer = "</select>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</select>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteJPG( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                INTEGER          lFile,
//:                STRING ( 10000 ) szWriteBuffer,
//:                STRING ( 50 )    szIndent,
//:                STRING ( 32 )    szCtrlTag,
//:                STRING ( 1 )     szNoPositioning,
//:                INTEGER          lOffsetX,
//:                INTEGER          lOffsetY )

//:   // Format a BITMAP or JPG control.

//:   STRING ( 32 )  szContextName
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteJPG( zVIEW     vDialog,
                 zLONG     lFile,
                 zPCHAR    szWriteBuffer,
                 zPCHAR    szIndent,
                 zPCHAR    szCtrlTag,
                 zPCHAR    szNoPositioning,
                 zLONG     lOffsetX,
                 zLONG     lOffsetY )
{
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szText
   zCHAR     szText[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szActionName
   zCHAR     szActionName[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szxx
   zCHAR     szxx[ 257 ] = { 0 }; 
   //:STRING ( 1 )   szObjectFlag
   zCHAR     szObjectFlag[ 2 ] = { 0 }; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 255 ]; 
   zSHORT    RESULT; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 261 ]; 
   zCHAR     szTempString_13[ 261 ]; 


   //:CreateTabIndexString( vDialog, szTabIndex )
   CreateTabIndexString( vDialog, szTabIndex );

   //:// If this window is being generated with no positioning then
   //:// do not create the position in the style.
   //:IF szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:CreateNoPosStyleString( vDialog, szStyle, "" )
      CreateNoPosStyleString( vDialog, szStyle, "" );
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" // No style information.
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
         //:ELSE 
      } 
      else
      { 
         //:CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
         CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" );
      } 

      //:END
   } 

   //:END

   //://CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
   //:szStyle = szStyle + szTabIndex
   ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );

   //:szActionName = ""
   ZeidonStringCopy( szActionName, 1, 0, "", 1, 0, 257 );
   //:IF vDialog.EventAct EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDialog, "EventAct" );
   if ( lTempInteger_0 == 0 )
   { 
      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szActionName = szActionName + "<a href=^#^ " 
         ZeidonStringConcat( szActionName, 1, 0, "<a href=^#^ ", 1, 0, 257 );
         //:ELSE 
      } 
      else
      { 
         //:szActionName = szActionName + "<a href=^#^ style=^display:block;width:100%;height:100%;text-decoration:none;^ " 
         ZeidonStringConcat( szActionName, 1, 0, "<a href=^#^ style=^display:block;width:100%;height:100%;text-decoration:none;^ ", 1, 0, 257 );
      } 

      //:END
      //:szActionName = szActionName + " onclick=^"
      ZeidonStringConcat( szActionName, 1, 0, " onclick=^", 1, 0, 257 );
      //:szActionName = szActionName + vDialog.EventAct.Tag
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDialog, "EventAct", "Tag", "", 0 );
      ZeidonStringConcat( szActionName, 1, 0, szTempString_0, 1, 0, 257 );
      //:szActionName = szActionName + "( )^ "
      ZeidonStringConcat( szActionName, 1, 0, "( )^ ", 1, 0, 257 );
      //:nRC = zstrcmpi( szActionName, "alt-f4" )
      nRC = zstrcmpi( szActionName, "alt-f4" );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:szActionName = " onclick=^AltF4( )^ "
         ZeidonStringCopy( szActionName, 1, 0, " onclick=^AltF4( )^ ", 1, 0, 257 );
      } 

      //:END
      //:szActionName = szActionName + ">"
      ZeidonStringConcat( szActionName, 1, 0, ">", 1, 0, 257 );
   } 

   //:END

   //:szClass = ""
   ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
   //:IF vDialog.Control.CSS_Class != ""
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) != 0 )
   { 
      //:szClass = " class=^" + vDialog.Control.CSS_Class + "^ "
      GetVariableFromAttribute( szTempString_1, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = " title=^" + szTitle + "^ "
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END
   //:szText = ""
   ZeidonStringCopy( szText, 1, 0, "", 1, 0, 257 );
   //:IF  vDialog.Control.Text != ""
   if ( CompareAttributeToString( vDialog, "Control", "Text", "" ) != 0 )
   { 
      //:szText = " alt=^" + vDialog.Control.Text + "^ "
      GetVariableFromAttribute( szTempString_2, 0, 'S', 255, vDialog, "Control", "Text", "", 0 );
      ZeidonStringCopy( szText, 1, 0, " alt=^", 1, 0, 257 );
      ZeidonStringConcat( szText, 1, 0, szTempString_2, 1, 0, 257 );
      ZeidonStringConcat( szText, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:szObjectFlag = ""
   ZeidonStringCopy( szObjectFlag, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "<object>"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "<object>", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szObjectFlag = "Y"
      ZeidonStringCopy( szObjectFlag, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //://There might be mapping for this control which means we should get the
   //://image from the mapping, or there might be an image file name, so we wouldn't
   //://use the mapping.
   //:SET CURSOR FIRST vDialog.CtrlMapLOD_Attribute WITHIN vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute", "Control" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 

      //:szWriteBuffer = "<% strErrorMapValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strErrorMapValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
      //:                "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
      GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      " + vDialog.CtrlMapView.Name +
      //:         ".log( ).debug( ^Invalid View: ^, ^" + szCtrlTag + "^ );"
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".log( ).debug( ^Invalid View: ^, ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
      GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( nRC >= 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         strErrorMapValue = " +
      //:                vDialog.CtrlMapView.Name +
      //:                ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
      //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
      //:                szContextName + "^ );"
      GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorMapValue = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to ""
      //:szWriteBuffer = "         if ( strErrorMapValue == null )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( strErrorMapValue == null )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            strErrorMapValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            strErrorMapValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:IF szObjectFlag = ""
      if ( ZeidonStringCompare( szObjectFlag, 1, 0, "", 1, 0, 2 ) == 0 )
      { 
         //:szWriteBuffer = szActionName + "<img src=^<%=strErrorMapValue%>^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ " + szStyle + szTitleHTML + szClass + szText + ">"
         ZeidonStringCopy( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "<img src=^<%=strErrorMapValue%>^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = szActionName + "<object data=^<%=strErrorMapValue%>^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ " + szStyle + szTitleHTML + szClass + szText + "></object>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "<object data=^<%=strErrorMapValue%>^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "></object>", 1, 0, 10001 );
      } 

      //:END
      //:IF szActionName != ""
      if ( ZeidonStringCompare( szActionName, 1, 0, "", 1, 0, 257 ) != 0 )
      { 
         //:szWriteBuffer = szWriteBuffer + "</a>" 
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</a>", 1, 0, 10001 );
      } 

      //:END
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:ELSE
   } 
   else
   { 
      //://Not getting image from mapping, a file name has been specified.
      //:IF vDialog.Control.WebFileName != ""
      if ( CompareAttributeToString( vDialog, "Control", "WebFileName", "" ) != 0 )
      { 
         //:IF szObjectFlag = ""
         if ( ZeidonStringCompare( szObjectFlag, 1, 0, "", 1, 0, 2 ) == 0 )
         { 
            //:// szWriteBuffer = "<a href=^#^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ ><img src=^" +
            //://    vDialog.Control.WebFileName + "^ " + szStyle + szTitleHTML + "></a>"
            //:szWriteBuffer = szActionName + "<img src=^" + vDialog.Control.WebFileName + "^  name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ " +
            //:                  szStyle + szTitleHTML + szClass + szText + ">"
            ZeidonStringCopy( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "<img src=^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_12, 0, 'S', 261, vDialog, "Control", "WebFileName", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^  name=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 
            //:szWriteBuffer = szActionName + "<object data=^" + vDialog.Control.WebFileName + "^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ " +
            //:                szStyle + szTitleHTML + szClass + szText + "></object>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "<object data=^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_13, 0, 'S', 261, vDialog, "Control", "WebFileName", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_13, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "></object>", 1, 0, 10001 );
         } 

         //:// szWriteBuffer = "<a href=^#^ name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^ ^onclick=^" +
         //://                 szActionName + "( )^><img src=^" + vDialog.Control.WebFileName + "^ " + szStyle + szTitleHTML + "></a>"
         //:END
         //:IF szActionName != ""
         if ( ZeidonStringCompare( szActionName, 1, 0, "", 1, 0, 257 ) != 0 )
         { 
            //:szWriteBuffer = szWriteBuffer + "</a>" 
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</a>", 1, 0, 10001 );
         } 

         //:END
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
         //:ELSE
      } 
      else
      { 
         //://Neither mapping nor filename was specified.
         //:szWriteBuffer = "//There was no information on where to get the image.  Nothing created."
         ZeidonStringCopy( szWriteBuffer, 1, 0, "//There was no information on where to get the image.  Nothing created.", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      } 

      //:END
   } 

   //:END
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteText( VIEW vDialog      BASED ON LOD TZWDLGSO,
//:                  VIEW vGroupParent BASED ON LOD TZWDLGSO,
//:                  VIEW vDialogRoot  BASED ON LOD TZWDLGSO,
//:                  INTEGER          lFile,
//:                  STRING ( 10000 ) szWriteBuffer,
//:                  STRING ( 50 )    szIndent,
//:                  STRING ( 32 )    szCtrlTag,
//:                  STRING ( 20 )    szJustify,
//:                  STRING ( 1 )     szNoPositioning,
//:                  INTEGER          lOffsetX,
//:                  INTEGER          lOffsetY,
//:                  STRING ( 100 )   szRepeatGrpKey )

//:   // Format a TEXT control.

//:   STRING ( 32 )  szContextName
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteText( zVIEW     vDialog,
                  zVIEW     vGroupParent,
                  zVIEW     vDialogRoot,
                  zLONG     lFile,
                  zPCHAR    szWriteBuffer,
                  zPCHAR    szIndent,
                  zPCHAR    szCtrlTag,
                  zPCHAR    szJustify,
                  zPCHAR    szNoPositioning,
                  zLONG     lOffsetX,
                  zLONG     lOffsetY,
                  zPCHAR    szRepeatGrpKey )
{
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClassHTML
   zCHAR     szClassHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 265 ) szHTML5Attr
   zCHAR     szHTML5Attr[ 266 ] = { 0 }; 
   //:STRING ( 1000 ) szText
   zCHAR     szText[ 1001 ] = { 0 }; 
   //:STRING ( 1000 ) szText2
   zCHAR     szText2[ 1001 ] = { 0 }; 
   //:STRING ( 256 ) szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 10 )  szMaxStringLth
   zCHAR     szMaxStringLth[ 11 ] = { 0 }; 
   //:STRING ( 20 )  szGroupParentType
   zCHAR     szGroupParentType[ 21 ] = { 0 }; 
   //:STRING ( 256 ) szHTMLCtrlID
   zCHAR     szHTMLCtrlID[ 257 ] = { 0 }; 
   //:STRING ( 10 )  szX_Pos
   zCHAR     szX_Pos[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szY_Pos
   zCHAR     szY_Pos[ 11 ] = { 0 }; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:INTEGER        lMaxStringLth
   zLONG     lMaxStringLth = 0; 
   //:INTEGER        lSubtypeX
   zLONG     lSubtypeX = 0; 
   //:INTEGER        lSubtype
   zLONG     lSubtype = 0; 
   //:INTEGER        lTemp
   zLONG     lTemp = 0; 
   //:SHORT          nMultiLineFlag
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
   //:STRING ( 32 )  szFaceName
   zCHAR     szFaceName[ 33 ] = { 0 }; 
   //:STRING ( 256 ) szLabelFor
   zCHAR     szLabelFor[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szDecoration
   zCHAR     szDecoration[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTempStyle
   zCHAR     szTempStyle[ 257 ] = { 0 }; 
   //:STRING ( 1 )   szIsLabel
   zCHAR     szIsLabel[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szIsTextOnly
   zCHAR     szIsTextOnly[ 2 ] = { 0 }; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_13[ 33 ]; 
   zCHAR     szTempString_14[ 33 ]; 
   zCHAR     szTempString_15[ 33 ]; 
   zCHAR     szTempString_16[ 33 ]; 
   zCHAR     szTempString_17[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_18[ 33 ]; 
   zCHAR     szTempString_19[ 33 ]; 
   zCHAR     szTempString_20[ 33 ]; 
   zCHAR     szTempString_21[ 33 ]; 
   zCHAR     szTempString_22[ 33 ]; 
   zCHAR     szTempString_23[ 33 ]; 
   zLONG     lTempInteger_3; 
   zCHAR     szTempString_24[ 255 ]; 
   zCHAR     szTempString_25[ 33 ]; 
   zCHAR     szTempString_26[ 33 ]; 
   zCHAR     szTempString_27[ 255 ]; 


   //:// If there is a web property of "Text Only", then we are not putting a span/label around anything we are simply using
   //:// what is in the text field which is probably an html tag like <h1>.
   //:szIsTextOnly = ""
   ZeidonStringCopy( szIsTextOnly, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Text Only"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Text Only", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szIsTextOnly = "Y"
      ZeidonStringCopy( szIsTextOnly, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //:// KJS 09/30/16 - Language Conversion.
   //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
   //:// So for now, I am always using, if the flag is set.
   //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y" AND szIsTextOnly = ""
   if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 && ZeidonStringCompare( szIsTextOnly, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:// Seems like we need to check the text, and if the text has " then we shouldn't use the getLanguageText. Because one example we have
      //:// is where the text is <img src="./images/admi..." border="0".
      //:szText2 = vDialog.Control.Text
      GetVariableFromAttribute( szText2, 0, 'S', 1001, vDialog, "Control", "Text", "", 0 );
      //:szTextID = vDialog.Control.LangID
      GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
      //:zSearchAndReplace( szText2, 1000, "\", "\\" )
      zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
      //:szStyle = "\" + QUOTES 
      ZeidonStringCopy( szStyle, 1, 0, "\\", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, QUOTES, 1, 0, 301 );
      //:zSearchAndReplace( szText2, 1000, QUOTES, szStyle )
      zSearchAndReplace( szText2, 1000, QUOTES, szStyle );
      //:szStyle = ""
      ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
      //:IF szTextID = ""
      if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
      { 
         //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
         ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 1001 );
         ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 1001 );
         ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 1001 );
         //:ELSE 
      } 
      else
      { 
         //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
         ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 1001 );
         ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 1001 );
         ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 1001 );
         ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 1001 );
         ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 1001 );
      } 

      //:END
      //://szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
      //:ELSE
   } 
   else
   { 
      //:szText = vDialog.Control.Text
      GetVariableFromAttribute( szText, 0, 'S', 1001, vDialog, "Control", "Text", "", 0 );
   } 

   //:END

   //:CreateTabIndexString( vDialog, szTabIndex )
   CreateTabIndexString( vDialog, szTabIndex );

   //:szHTMLCtrlID = " id=^" + szCtrlTag + szRepeatGrpKey + "^ name=^" + szCtrlTag + szRepeatGrpKey + "^ "
   ZeidonStringCopy( szHTMLCtrlID, 1, 0, " id=^", 1, 0, 257 );
   ZeidonStringConcat( szHTMLCtrlID, 1, 0, szCtrlTag, 1, 0, 257 );
   ZeidonStringConcat( szHTMLCtrlID, 1, 0, szRepeatGrpKey, 1, 0, 257 );
   ZeidonStringConcat( szHTMLCtrlID, 1, 0, "^ name=^", 1, 0, 257 );
   ZeidonStringConcat( szHTMLCtrlID, 1, 0, szCtrlTag, 1, 0, 257 );
   ZeidonStringConcat( szHTMLCtrlID, 1, 0, szRepeatGrpKey, 1, 0, 257 );
   ZeidonStringConcat( szHTMLCtrlID, 1, 0, "^ ", 1, 0, 257 );

   //:szHTML5Attr = ""
   ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 266 );
   //:IF vDialog.Control.WebHTML5Attribute != ""
   if ( CompareAttributeToString( vDialog, "Control", "WebHTML5Attribute", "" ) != 0 )
   { 
      //:szHTML5Attr = " " + vDialog.Control.WebHTML5Attribute + " "
      GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vDialog, "Control", "WebHTML5Attribute", "", 0 );
      ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 266 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_0, 1, 0, 266 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 266 );
   } 

   //:END

   //:IF  szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:CreateNoPosStyleString( vDialog, szStyle, "" )
      CreateNoPosStyleString( vDialog, szStyle, "" );
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S" // No style information
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" 
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
         //:ELSE 
      } 
      else
      { 
         //:CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
         CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" );
      } 

      //:END
   } 

   //:END

   //:nRC = TX_GetTextRptCtrlBOI( vDialog, nMultiLineFlag, nBoldFlag,
   //:                            nItalicFlag, nStrikeoutFlag,
   //:                            nUnderlineFlag, lJustify,
   //:                            lFontSize, lTextColor, lTextBkColor,
   //:                            lBorderStyle, lBorderColor,
   //:                            lEscapement, lType, szFaceName )
   nRC = TX_GetTextRptCtrlBOI( vDialog, &nMultiLineFlag, &nBoldFlag, &nItalicFlag, &nStrikeoutFlag, &nUnderlineFlag, &lJustify, &lFontSize, &lTextColor, &lTextBkColor, &lBorderStyle, &lBorderColor, &lEscapement, &lType, szFaceName );

   //:// If the return code is <= 0, the CtrlBOI is null.
   //:szDecoration = ""
   ZeidonStringCopy( szDecoration, 1, 0, "", 1, 0, 257 );
   //:IF nRC > 0
   if ( nRC > 0 )
   { 

      //:IF nItalicFlag != 0
      if ( nItalicFlag != 0 )
      { 
         //:szDecoration = szDecoration + "font-style:italic;"
         ZeidonStringConcat( szDecoration, 1, 0, "font-style:italic;", 1, 0, 257 );
      } 

      //:END

      //:IF nBoldFlag != 0
      if ( nBoldFlag != 0 )
      { 
         //:szDecoration = szDecoration + "font-weight:bold;"
         ZeidonStringConcat( szDecoration, 1, 0, "font-weight:bold;", 1, 0, 257 );
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
            //:szDecoration = szDecoration + "text-align:center;"
            ZeidonStringConcat( szDecoration, 1, 0, "text-align:center;", 1, 0, 257 );
            //:ELSE
         } 
         else
         { 
            //:nRC = IsFlagSequenceSet( lJustify, 4 )
            nRC = IsFlagSequenceSet( lJustify, 4 );
            //:IF nRC != 0
            if ( nRC != 0 )
            { 
               //:szDecoration = szDecoration + "text-align:right;"
               ZeidonStringConcat( szDecoration, 1, 0, "text-align:right;", 1, 0, 257 );
            } 

            //:END
         } 

         //:END
      } 

      //:END

      //:IF nUnderlineFlag != 0 AND nStrikeoutFlag != 0
      if ( nUnderlineFlag != 0 && nStrikeoutFlag != 0 )
      { 
         //:szDecoration = szDecoration + "text-decoration:underline line-through;"
         ZeidonStringConcat( szDecoration, 1, 0, "text-decoration:underline line-through;", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:IF nUnderlineFlag != 0
         if ( nUnderlineFlag != 0 )
         { 
            //:szDecoration = szDecoration + "text-decoration:underline;"
            ZeidonStringConcat( szDecoration, 1, 0, "text-decoration:underline;", 1, 0, 257 );
         } 

         //:END

         //:IF nStrikeoutFlag != 0
         if ( nStrikeoutFlag != 0 )
         { 
            //:szDecoration = szDecoration + "text-decoration:line-through;"
            ZeidonStringConcat( szDecoration, 1, 0, "text-decoration:line-through;", 1, 0, 257 );
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //://KJS 10/19/07 - Added title.
   //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = " title=^" + szTitle + "^ "
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:IF szNoPositioning != "S" 
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) != 0 )
   { 
      //:zAppendQuotedString( szStyle, szDecoration, "style=", "^" )
      zAppendQuotedString( szStyle, szDecoration, "style=", "^" );
   } 

   //:END
   //:szStyle = szStyle + szTabIndex
   ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );
   //:IF vDialog.Control.CSS_Class != ""
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) != 0 )
   { 
      //:szClassHTML = "class=^" + vDialog.Control.CSS_Class + "^ "
      GetVariableFromAttribute( szTempString_1, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClassHTML, 1, 0, "class=^", 1, 0, 257 );
      ZeidonStringConcat( szClassHTML, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szClassHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:// Check if a Group control as parent of this control requests relative postioning either through the
   //:// older "Span" setting or the newer "RelativePos" request.
   //:szGroupParentType = ""
   ZeidonStringCopy( szGroupParentType, 1, 0, "", 1, 0, 21 );
   //:IF vGroupParent != 0
   if ( vGroupParent != 0 )
   { 
      //:// Override Style if parent group requests relative positioning
      //:// KJS 01/30/14 - I'm thinking "Relative Positioning" is old. I only use this in one pwd and when I take it
      //:// out and use "Div/No Height", the results look exactly the same so I am going to take this off grid controls.
      //:///*
      //:SET CURSOR FIRST vGroupParent.WebControlProperty WHERE vGroupParent.WebControlProperty.Name = "Relative Positioning"
      RESULT = SetCursorFirstEntityByString( vGroupParent, "WebControlProperty", "Name", "Relative Positioning", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:szGroupParentType = "RelativePos"
         ZeidonStringCopy( szGroupParentType, 1, 0, "RelativePos", 1, 0, 21 );
         //:ELSE
      } 
      else
      { 
         //:szGroupParentType = vGroupParent.Control.WebCtrlType
         GetVariableFromAttribute( szGroupParentType, 0, 'S', 21, vGroupParent, "Control", "WebCtrlType", "", 0 );
      } 

      //:END
      //://*/
      //://szGroupParentType = vGroupParent.Control.WebCtrlType
      //:IF szGroupParentType = "RelativePos" OR szGroupParentType = "Span"
      if ( ZeidonStringCompare( szGroupParentType, 1, 0, "RelativePos", 1, 0, 21 ) == 0 || ZeidonStringCompare( szGroupParentType, 1, 0, "Span", 1, 0, 21 ) == 0 )
      { 
         //:szStyle = "style=^position:relative;^"
         ZeidonStringCopy( szStyle, 1, 0, "style=^position:relative;^", 1, 0, 301 );
      } 

      //:END
   } 

   //:END

   //:// Process variable Text mapping.
   //:SET CURSOR FIRST vDialog.CtrlMap WHERE vDialog.CtrlMap.Tag = "Displayed Text"
   RESULT = SetCursorFirstEntityByString( vDialog, "CtrlMap", "Tag", "Displayed Text", "" );
   //:IF RESULT < zCURSOR_SET
   if ( RESULT < zCURSOR_SET )
   { 
      //:SET CURSOR FIRST vDialog.CtrlMapLOD_Attribute WITHIN vDialog.Control
      //:           WHERE vDialog.CtrlMap.Tag = ""
      RESULT = SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute", "Control" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToString( vDialog, "CtrlMap", "Tag", "" ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( vDialog, "CtrlMapLOD_Attribute", "Control" );
         } 

      } 

   } 

   //:END
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:// There is mapping of the Display Text value.
      //:szWriteBuffer = "<% strTextDisplayValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strTextDisplayValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
      //:                "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      task.log( ).debug( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.log( ).debug( ^Invalid View: ^ + ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
      GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( nRC >= 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:IF vDialog.CtrlMapContext EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
      if ( lTempInteger_0 == 0 )
      { 
         //:szContextName = vDialog.CtrlMapContext.Name
         GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:szContextName = ""
         ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
      } 

      //:END

      //:/**********************************/
      //:// Add try/catch
      //:szWriteBuffer = "      try"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      try", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:IF vDialog.CtrlMapER_Domain.DataType = "T" OR vDialog.CtrlMapER_Domain.DataType = "D"
      if ( CompareAttributeToString( vDialog, "CtrlMapER_Domain", "DataType", "T" ) == 0 || CompareAttributeToString( vDialog, "CtrlMapER_Domain", "DataType", "D" ) == 0 )
      { 
         //:szWriteBuffer = "         strTextDisplayValue = " +
         //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
         //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
         //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
         //:                szContextName + "^ );"
         GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strTextDisplayValue = ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
         GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
         //:IF lMaxStringLth <= 0
         if ( lMaxStringLth <= 0 )
         { 
            //:szMaxStringLth = "254"
            ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
            //:ELSE
         } 
         else
         { 
            //:szMaxStringLth = lMaxStringLth
            ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
         } 

         //:END

         //:szWriteBuffer = "         strTextDisplayValue = " +
         //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
         //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
         //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
         //:                szContextName + "^ );"
         GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strTextDisplayValue = ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_12, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      } 

      //:END

      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      catch (Exception e)"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      catch (Exception e)", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //://szWriteBuffer = "         strBuildPageError = e.getMessage();"
      //:szWriteBuffer = "         out.println(^There is an error on " + szCtrlTag + ": ^ + e.getMessage());"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         out.println(^There is an error on ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + e.getMessage());", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         task.log().info( ^*** Error on ctrl " + szCtrlTag + "^ + e.getMessage() );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log().info( ^*** Error on ctrl ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ + e.getMessage() );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:/***************** END OF try/catch **********************/

      //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
      //:szWriteBuffer = "         if ( strTextDisplayValue == null )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( strTextDisplayValue == null )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            strTextDisplayValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            strTextDisplayValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:// KJS 09/30/16 - Language Conversion.
      //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
      //:// So for now, I am always using, if the flag is set.
      //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:szText = "<%=LangConv.getLanguageText(strTextDisplayValue)%>" 
         ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(strTextDisplayValue)%>", 1, 0, 1001 );
         //:ELSE
      } 
      else
      { 
         //:szText = "<%=strTextDisplayValue%>"
         ZeidonStringCopy( szText, 1, 0, "<%=strTextDisplayValue%>", 1, 0, 1001 );
      } 

      //:END
   } 


   //:END

   //:IF vDialog.EventAct EXISTS    // There is only one EventAct, which is HotText.
   lTempInteger_1 = CheckExistenceOfEntity( vDialog, "EventAct" );
   if ( lTempInteger_1 == 0 )
   { 
      //:IF vDialog.EventAct.Type = 45    // Type 45 is Hyperlink Action.
      if ( CompareAttributeToInteger( vDialog, "EventAct", "Type", 45 ) == 0 )
      { 
         //:// The Text triggers a Hyperlink to a constant or variable URL address
         //:SET CURSOR FIRST vDialog.CtrlMap WHERE vDialog.CtrlMap.Tag = "Hyperlink URL"
         RESULT = SetCursorFirstEntityByString( vDialog, "CtrlMap", "Tag", "Hyperlink URL", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:// There is mapping of the Hyperlink URL value.
            //:szWriteBuffer = "<% strTextURL_Value = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strTextURL_Value = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
            //:                "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
            GetVariableFromAttribute( szTempString_13, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_13, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_14, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_14, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
            GetVariableFromAttribute( szTempString_15, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_15, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      task.log( ).debug( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.log( ).debug( ^Invalid View: ^ + ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "   else"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "   {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
            //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
            GetVariableFromAttribute( szTempString_16, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_16, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_17, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_17, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      if ( nRC >= 0 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:IF vDialog.CtrlMapContext EXISTS
            lTempInteger_2 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
            if ( lTempInteger_2 == 0 )
            { 
               //:szContextName = vDialog.CtrlMapContext.Name
               GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
               //:ELSE
            } 
            else
            { 
               //:szContextName = ""
               ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
            } 

            //:END

            //:IF vDialog.CtrlMapER_Domain.DataType = "T" OR vDialog.CtrlMapER_Domain.DataType = "D"
            if ( CompareAttributeToString( vDialog, "CtrlMapER_Domain", "DataType", "T" ) == 0 || CompareAttributeToString( vDialog, "CtrlMapER_Domain", "DataType", "D" ) == 0 )
            { 
               //:szWriteBuffer = "         strTextURL_Value = " +
               //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
               //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
               //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
               //:                szContextName + "^ );"
               GetVariableFromAttribute( szTempString_18, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         strTextURL_Value = ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_18, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_19, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_19, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_20, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_20, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
               //:ELSE
            } 
            else
            { 
               //:lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
               GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
               //:IF lMaxStringLth <= 0
               if ( lMaxStringLth <= 0 )
               { 
                  //:szMaxStringLth = "254"
                  ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
                  //:ELSE
               } 
               else
               { 
                  //:szMaxStringLth = lMaxStringLth
                  ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
               } 

               //:END

               //:szWriteBuffer = "         strTextURL_Value = " +
               //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
               //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
               //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
               //:                szContextName + "^ );"
               GetVariableFromAttribute( szTempString_21, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         strTextURL_Value = ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_21, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_22, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_22, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
               GetVariableFromAttribute( szTempString_23, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_23, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            } 

            //:END

            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
            //:szWriteBuffer = "         if ( strTextURL_Value == null )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( strTextURL_Value == null )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            strTextURL_Value = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            strTextURL_Value = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "   }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "%>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = ""
            ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //://KJS 10/19/07 - Added title.

            //:// The Hyperlink URL value is a variable.
            //:szWriteBuffer = "<a href=^<%=strTextURL_Value%>^  " + szHTMLCtrlID + szClassHTML + szTitleHTML + szStyle + " target=^_blank^>" + szText + "</a>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<a href=^<%=strTextURL_Value%>^  ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClassHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " target=^_blank^>", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</a>", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 

            //:// The Hyperlink URL value is a constant in the Action.
            //:SET CURSOR FIRST vDialogRoot.Action WHERE vDialogRoot.Action.ZKey = vDialog.EventAct.ZKey
            GetIntegerFromAttribute( &lTempInteger_3, vDialog, "EventAct", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vDialogRoot, "Action", "ZKey", lTempInteger_3, "" );
            //:IF vDialogRoot.Action.WebHTML_TransferAddress = ""
            if ( CompareAttributeToString( vDialogRoot, "Action", "WebHTML_TransferAddress", "" ) == 0 )
            { 
               //:MessageSend( vDialog, "", "XSL Generation",
               //:             "A Hyperlink Action without URL mapping requires a 'Link to HTML Address' value.",
               //:             zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               MessageSend( vDialog, "", "XSL Generation", "A Hyperlink Action without URL mapping requires a 'Link to HTML Address' value.", zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            } 

            //:END

            //:szWriteBuffer = ""
            ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //://KJS 10/19/07 - Added title.
            //:szWriteBuffer = "<a href=^" + vDialogRoot.Action.WebHTML_TransferAddress + "^  " + szHTMLCtrlID + szClassHTML + szTitleHTML + szStyle + " target=^_blank^>" + szText + "</a>"
            GetVariableFromAttribute( szTempString_24, 0, 'S', 255, vDialogRoot, "Action", "WebHTML_TransferAddress", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<a href=^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_24, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^  ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClassHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " target=^_blank^>", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</a>", 1, 0, 10001 );
         } 


         //:END
         //:ELSE
      } 
      else
      { 
         //:// This is HotText with regular Action, rather than HyperLink.
         //:szWriteBuffer = ""
         ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //://KJS 10/17/2007 Jeff says we need "javascript:" before the action name for href actions.  I think this
         //://is the place to put it but I confess I'm not exactly sure...
         //://KJS 10/19/07 - Added title.
         //://KJS 09/24/10 - Now realizing that Jeff was incorrect about the javascript: and it shouldn't be used (http://jibbering.com/faq/#FAQ4_24).
         //://Changing them to use the # and onclick.
         //:IF szRepeatGrpKey = ""
         if ( ZeidonStringCompare( szRepeatGrpKey, 1, 0, "", 1, 0, 101 ) == 0 )
         { 
            //:szWriteBuffer = "<a href=^#^" + szHTMLCtrlID + " onclick=^" + vDialog.EventAct.Tag + "( );^ " + szClassHTML + szTitleHTML + szStyle + ">" + szText + "</a>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<a href=^#^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_25, 0, 'S', 33, vDialog, "EventAct", "Tag", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_25, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "( );^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClassHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</a>", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 
            //:szWriteBuffer = "<a href=^#^" + szHTMLCtrlID + "  onclick=^" + vDialog.EventAct.Tag + "( '" + szCtrlTag + szRepeatGrpKey + "' );^ " + szClassHTML + szTitleHTML + szStyle + ">" + szText + "</a>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<a href=^#^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "  onclick=^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_26, 0, 'S', 33, vDialog, "EventAct", "Tag", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_26, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "( '", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "' );^ ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClassHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</a>", 1, 0, 10001 );
         } 

         //:END
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:// Regular Text Control (No HotText)
      //:szWriteBuffer = ""
      ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:szIsLabel = ""
      ZeidonStringCopy( szIsLabel, 1, 0, "", 1, 0, 2 );
      //:szLabelFor = ""
      ZeidonStringCopy( szLabelFor, 1, 0, "", 1, 0, 257 );
      //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Label"
      RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Label", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:szIsLabel = "Y"
         ZeidonStringCopy( szIsLabel, 1, 0, "Y", 1, 0, 2 );
         //:IF vDialog.Control.WebCtrlLabelLink != ""
         if ( CompareAttributeToString( vDialog, "Control", "WebCtrlLabelLink", "" ) != 0 )
         { 
            //:// Tie the label to an input control.
            //:szLabelFor = " for=^" + vDialog.Control.WebCtrlLabelLink + "^ " 
            GetVariableFromAttribute( szTempString_27, 0, 'S', 255, vDialog, "Control", "WebCtrlLabelLink", "", 0 );
            ZeidonStringCopy( szLabelFor, 1, 0, " for=^", 1, 0, 257 );
            ZeidonStringConcat( szLabelFor, 1, 0, szTempString_27, 1, 0, 257 );
            ZeidonStringConcat( szLabelFor, 1, 0, "^ ", 1, 0, 257 );
         } 

         //:END
      } 

      //:END

      //:IF szIsTextOnly = "Y"
      if ( ZeidonStringCompare( szIsTextOnly, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:szWriteBuffer = szText
         ZeidonStringCopy( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         //:ELSE     
      } 
      else
      { 
         //:// If szNoPositioning is null then we are using absolute positioning when creating controls.
         //:IF  szNoPositioning = ""
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
         { 

            //:// PIX_PER_DU( vDialog, dDLUnits )
            //:// zIntegerToString( szX_Pos, 10, vDialog.Control.PSDLG_X * dDLUnits )
            //:// zIntegerToString( szY_Pos, 10, vDialog.Control.PSDLG_Y * dDLUnits )
            //:// szTempStyle = "position:absolute;left:" + szX_Pos + "px;" + "top:" + szY_Pos + "px;^ "
            //:zAppendQuotedString( szStyle, szDecoration, "style=", "^" )
            zAppendQuotedString( szStyle, szDecoration, "style=", "^" );
            //:szWriteBuffer = "<label " + szClassHTML + szHTMLCtrlID + szTitleHTML + szStyle + ">" + szText + "</label>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<label ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szClassHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "</label>", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 

            //:// If there is a "Label" web control property than make this a label, otherwise, just use <span>.
            //:IF szIsLabel = "Y"
            if ( ZeidonStringCompare( szIsLabel, 1, 0, "Y", 1, 0, 2 ) == 0 )
            { 
               //:szWriteBuffer = "<label " + szClassHTML + szHTMLCtrlID + szHTML5Attr + szLabelFor + szTitleHTML + szStyle + ">" + szText + "</label>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "<label ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szClassHTML, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szLabelFor, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "</label>", 1, 0, 10001 );
               //:ELSE 
            } 
            else
            { 
               //:szWriteBuffer = "<span " + szClassHTML + szHTMLCtrlID + szTitleHTML + szHTML5Attr + szStyle + ">" + szText + "</span>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "<span ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szClassHTML, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, "</span>", 1, 0, 10001 );
            } 

            //:END
         } 


         //:END
      } 

      //:END
   } 

   //:END

   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteEditBox( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                     INTEGER          lFile,
//:                     STRING ( 10000 ) szWriteBuffer,
//:                     STRING ( 50 )    szIndent,
//:                     STRING ( 32 )    szCtrlTag,
//:                     STRING ( 1 )     szTableRowFlag,
//:                     STRING ( 20 )    szJustify,
//:                     STRING ( 1 )     szNoPositioning,
//:                     INTEGER          lOffsetX,
//:                     INTEGER          lOffsetY,
//:                     STRING ( 100 )   szRepeatGrpKey )

//:   // Format an EDITBOX control.

//:   VIEW vDialogRoot BASED ON LOD TZWDLGSO
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteEditBox( zVIEW     vDialog,
                     zLONG     lFile,
                     zPCHAR    szWriteBuffer,
                     zPCHAR    szIndent,
                     zPCHAR    szCtrlTag,
                     zPCHAR    szTableRowFlag,
                     zPCHAR    szJustify,
                     zPCHAR    szNoPositioning,
                     zLONG     lOffsetX,
                     zLONG     lOffsetY,
                     zPCHAR    szRepeatGrpKey )
{
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 32 )  szContextName
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 300 ) szValue
   zCHAR     szValue[ 301 ] = { 0 }; 
   //:STRING ( 600 ) szBlob
   zCHAR     szBlob[ 601 ] = { 0 }; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szHTML5Attr
   zCHAR     szHTML5Attr[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szPlaceholder
   zCHAR     szPlaceholder[ 257 ] = { 0 }; 
   //:STRING ( 300 ) szType
   zCHAR     szType[ 301 ] = { 0 }; 
   //:STRING ( 64 )  szWidget
   zCHAR     szWidget[ 65 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 600 ) szEditActionCode
   zCHAR     szEditActionCode[ 601 ] = { 0 }; 
   //:STRING ( 32 )  szActionName
   zCHAR     szActionName[ 33 ] = { 0 }; 
   //:STRING ( 10 )  szMaxStringLth
   zCHAR     szMaxStringLth[ 11 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:STRING ( 20 )  szAutoCompleteOff
   zCHAR     szAutoCompleteOff[ 21 ] = { 0 }; 
   //:STRING ( 1 )   szWCP_Hidden
   zCHAR     szWCP_Hidden[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szCreateSessionVariable
   zCHAR     szCreateSessionVariable[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szNoOutputMapping
   zCHAR     szNoOutputMapping[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szBrowseFile
   zCHAR     szBrowseFile[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szNumber
   zCHAR     szNumber[ 2 ] = { 0 }; 

   //:INTEGER        lMaxStringLth
   zLONG     lMaxStringLth = 0; 
   //:INTEGER        lStyleX
   zLONG     lStyleX = 0; 
   //:INTEGER        lTemp
   zLONG     lTemp = 0; 
   //:SHORT          bMapping
   zSHORT    bMapping = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 

   //:// For testing no height on input boxes
   //:STRING ( 10 )  szWidth
   zCHAR     szWidth[ 11 ] = { 0 }; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:STRING ( 100 )  szSize
   zCHAR     szSize[ 101 ] = { 0 }; 
   //:STRING ( 100 )  szPosition
   zCHAR     szPosition[ 101 ] = { 0 }; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 33 ]; 
   zCHAR     szTempString_13[ 255 ]; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_14[ 255 ]; 
   zCHAR     szTempString_15[ 255 ]; 
   zCHAR     szTempString_16[ 255 ]; 
   zCHAR     szTempString_17[ 255 ]; 
   zCHAR     szTempString_18[ 255 ]; 


   //://TraceLineS("*** GenJSPJ_CrteEditBox *** ", "")

   //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
   GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );

   //:CreateTabIndexString( vDialog, szTabIndex )
   CreateTabIndexString( vDialog, szTabIndex );
   //://CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "<%=strErrorColor%>" )
   //:// KJS 07/08 - We placed DOCTYPE at the top of our html pages now.  For some reason, because of this
   //:// even though the pixel height is exactly the same as before, it shows up higher in the browser.
   //:// Because of this we have taken out the height for now and the input height will be determined in the
   //:// CSS.

   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "No Width"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "No Width", "" );
   //:IF szNoPositioning = "Y" AND RESULT < zCURSOR_SET
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 && RESULT < zCURSOR_SET )
   { 
      //:PIX_PER_DU( vDialog, dDLUnits )
      PIX_PER_DU( vDialog, &dDLUnits );
      //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
      GetIntegerFromAttribute( &lTempInteger_0, vDialog, "Control", "SZDLG_X" );
      zIntegerToString( szWidth, 10, lTempInteger_0 * dDLUnits );
      //:szSize = "width:" + szWidth + "px;"
      ZeidonStringCopy( szSize, 1, 0, "width:", 1, 0, 101 );
      ZeidonStringConcat( szSize, 1, 0, szWidth, 1, 0, 101 );
      ZeidonStringConcat( szSize, 1, 0, "px;", 1, 0, 101 );
   } 

   //:END
   //:// If zNoPositioning is null then we are using absolute positioning for controls
   //:IF  szNoPositioning = ""
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:CreateAbsolPosString( vDialog, szPosition, lOffsetX, lOffsetY )
      CreateAbsolPosString( vDialog, szPosition, lOffsetX, lOffsetY );
   } 

   //:END

   //:IF szNoPositioning = "S" // No style information but I think we need the error color.
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
   { 
      //:szStyle = "style=^<%=strErrorColor%>^"
      ZeidonStringCopy( szStyle, 1, 0, "style=^<%=strErrorColor%>^", 1, 0, 301 );
      //:ELSE 
   } 
   else
   { 
      //:FixStyleString( szStyle, szSize, szPosition,  "<%=strErrorColor%>"  )
      FixStyleString( szStyle, szSize, szPosition, "<%=strErrorColor%>" );
   } 

   //:END

   //:szStyle = szStyle + szTabIndex
   ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );

   //:// If there is a WebControlProperty for "Create Session Variable", then create a session variable
   //:// with the value of this edit box with the Control Tag as the name.
   //:szCreateSessionVariable = "" 
   ZeidonStringCopy( szCreateSessionVariable, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Create Session Variable"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Create Session Variable", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szCreateSessionVariable = "Y"
      ZeidonStringCopy( szCreateSessionVariable, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //:// There may be times when we do not want to show the mapping of a field.  Like for instance a password field.  If the user types
   //:// in the wrong password, then we don't want to leave what was already typed we want the edit box to be blank.  This is
   //:// for xss problems (cross-scripting attacks).   
   //:szNoOutputMapping = "" 
   ZeidonStringCopy( szNoOutputMapping, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "No Output Mapping"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "No Output Mapping", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szNoOutputMapping = "Y"
      ZeidonStringCopy( szNoOutputMapping, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //:szAutoCompleteOff = "" 
   ZeidonStringCopy( szAutoCompleteOff, 1, 0, "", 1, 0, 21 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Autocomplete Off"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Autocomplete Off", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szAutoCompleteOff = " autocomplete=^off^"
      ZeidonStringCopy( szAutoCompleteOff, 1, 0, " autocomplete=^off^", 1, 0, 21 );
   } 

   //:END

   //:szNumber = "" 
   ZeidonStringCopy( szNumber, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Number"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Number", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szNumber = "Y"
      ZeidonStringCopy( szNumber, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END

   //:// We have an Upload file on the group box, but what if we aren't uploading the file, we just want the user
   //:// to be able to select a directory/file name.
   //:szBrowseFile = "" 
   ZeidonStringCopy( szBrowseFile, 1, 0, "", 1, 0, 2 );
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Browse File"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Browse File", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //://TraceLineS("*** Browse File is Y *** ", "" )
      //:szBrowseFile = "Y"
      ZeidonStringCopy( szBrowseFile, 1, 0, "Y", 1, 0, 2 );
   } 

   //:END


   //:// szWriteBuffer = "<% strErrorMapValue = ^^; %>"  extraneous???
   //:// WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )           dks 2005.01.11

   //:SET CURSOR FIRST vDialog.CtrlMapLOD_Attribute WITHIN vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute", "Control" );
   //:IF RESULT >= zCURSOR_SET AND szNoOutputMapping = ""
   if ( RESULT >= zCURSOR_SET && ZeidonStringCompare( szNoOutputMapping, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:bMapping = 1
      bMapping = 1;
      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   strErrorMapValue = VmlOperation.CheckError( ^" + szCtrlTag + "^, strError );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   strErrorMapValue = VmlOperation.CheckError( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^, strError );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( !StringUtils.isBlank( strErrorMapValue ) )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( !StringUtils.isBlank( strErrorMapValue ) )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( StringUtils.equals( strErrorFlag, ^Y^ ) )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( StringUtils.equals( strErrorFlag, ^Y^ ) )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         strErrorColor = ^color:red;^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorColor = ^color:red;^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      strErrorColor = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      strErrorColor = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      " + vDialog.CtrlMapView.Name + " = " + "task.getViewByName( ^" +
      //:                vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_0, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         task.log( ).debug( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).debug( ^Invalid View: ^ + ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         if ( nRC >= 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:IF vDialog.CtrlMapContext EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
      if ( lTempInteger_1 == 0 )
      { 
         //:szContextName = vDialog.CtrlMapContext.Name
         GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
         //:ELSE
      } 
      else
      { 
         //:szContextName = ""
         ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
      } 

      //:END

      //:/**********************************/
      //:// Add try/catch
      //:szWriteBuffer = "            try"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            try", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:lMaxStringLth = vDialog.CtrlMapER_Domain.MaxStringLth
      GetIntegerFromAttribute( &lMaxStringLth, vDialog, "CtrlMapER_Domain", "MaxStringLth" );
      //:IF lMaxStringLth <= 0
      if ( lMaxStringLth <= 0 )
      { 
         //:szMaxStringLth = "254"
         ZeidonStringCopy( szMaxStringLth, 1, 0, "254", 1, 0, 11 );
         //:ELSE
      } 
      else
      { 
         //:szMaxStringLth = lMaxStringLth
         ZeidonStringConvertFromNumber( szMaxStringLth, 1, 0, 10, lMaxStringLth, (ZDecimal) 0.0, "I" );
      } 

      //:END

      //:szWriteBuffer = "               strErrorMapValue = " +
      //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
      //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
      //:                szContextName + "^ );"
      GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               strErrorMapValue = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesControlActionsView", "Y" ) == 0 )
      { 
         //:szWriteBuffer = "                  strDisabled = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "                  strDisabled = ^^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "               if ( " + vDialog.CtrlMapView.Name + ".isReadOnly( ) )"
         GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               if ( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".isReadOnly( ) )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //://szWriteBuffer = "                  strCtrlActionsView = strCtrlActionsView + ^" + szCtrlTag + ",Disabled,^;"
         //://szWriteBuffer = "                  strCtrlActions = strCtrlActions + ^" + szCtrlTag + ",Disable,^;"
         //:szWriteBuffer = "                  strDisabled = ^disabled^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "                  strDisabled = ^disabled^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:IF vDialog.Control.WebCtrlType = "escapeHTML"
      if ( CompareAttributeToString( vDialog, "Control", "WebCtrlType", "escapeHTML" ) == 0 )
      { 
         //:szWriteBuffer = "               task.log().debug( ^" + szCtrlTag + " prior to unescape: ^ + strErrorMapValue );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               task.log().debug( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " prior to unescape: ^ + strErrorMapValue );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "               strErrorMapValue = StringEscapeUtils.unescapeHtml4( strErrorMapValue );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               strErrorMapValue = StringEscapeUtils.unescapeHtml4( strErrorMapValue );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "               task.log().debug( ^" + szCtrlTag + " after unescape: ^ + strErrorMapValue );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               task.log().debug( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " after unescape: ^ + strErrorMapValue );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:szWriteBuffer = "            }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            catch (Exception e)"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            catch (Exception e)", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               out.println(^There is an error on " + szCtrlTag + ": ^ + e.getMessage());"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               out.println(^There is an error on ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + e.getMessage());", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               task.log().error( ^*** Error on ctrl " + szCtrlTag + "^, e );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               task.log().error( ^*** Error on ctrl ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^, e );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:/***************** END OF try/catch **********************/

      //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
      //:szWriteBuffer = "            if ( strErrorMapValue == null )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( strErrorMapValue == null )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               strErrorMapValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               strErrorMapValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:szWriteBuffer = "            task.log( ).debug( ^" + vDialog.CtrlMapRelatedEntity.Name + "." +
      //:                vDialog.CtrlMapER_Attribute.Name + ": ^ + strErrorMapValue );"
      GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).debug( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + strErrorMapValue );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:// If there is a WebControlProperty for "Create Session Variable", then create a session variable
      //:// with the value of this edit box with the Control Tag as the name.
      //:IF szCreateSessionVariable = "Y" 
      if ( ZeidonStringCompare( szCreateSessionVariable, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:szWriteBuffer = "            session.setAttribute(^" + szCtrlTag + "^, strErrorMapValue); "     
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            session.setAttribute(^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^, strErrorMapValue); ", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            task.log( ).debug( ^Entity does not exist for " + szCtrlTag + ": ^ + ^" +
      //:                vDialog.CtrlMapView.Name +
      //:                "." + vDialog.CtrlMapRelatedEntity.Name + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).debug( ^Entity does not exist for ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_12, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );

      //:lStyleX = vDialog.Control.ExtendedStyle
      GetIntegerFromAttribute( &lStyleX, vDialog, "Control", "ExtendedStyle" );
      //:lTemp = IsFlagSequenceSet( lStyleX, zCONTROLX_PREFILL )
      lTemp = IsFlagSequenceSet( lStyleX, zCONTROLX_PREFILL );
      //:IF lTemp != 0
      if ( lTemp != 0 )
      { 
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

         //:szWriteBuffer = "      if ( strErrorMapValue == ^^ )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( strErrorMapValue == ^^ )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         strErrorMapValue = ^" + vDialog.Control.Text + "^;"
         GetVariableFromAttribute( szTempString_13, 0, 'S', 255, vDialog, "Control", "Text", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorMapValue = ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_13, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^;", 1, 0, 10001 );
      } 

      //:END
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:ELSE
   } 
   else
   { 
      //:bMapping = 0
      bMapping = 0;
   } 

   //:END

   //:// Get the format for the edit box if specified.
   //:lTemp = 600
   lTemp = 600;
   //:GetBlobFromAttribute( szBlob, lTemp, vDialog, "Control", "CtrlBOI" )
   GetBlobFromAttribute( szBlob, (zPULONG) &lTemp, vDialog, "Control", "CtrlBOI" );
   //:lTemp = lTemp - (3 * zSIZEOFLONG)
   lTemp = lTemp - ( 3 * zSIZEOFLONG );
   //:IF lTemp <= 0
   if ( lTemp <= 0 )
   { 
      //:szBlob = ""
      ZeidonStringCopy( szBlob, 1, 0, "", 1, 0, 601 );
      //:ELSE
   } 
   else
   { 
      //:lTemp = (3 * zSIZEOFLONG) + 1
      lTemp = ( 3 * zSIZEOFLONG ) + 1;
      //:ZeidonCopyWithinString( szBlob, 1, lTemp, 600 )
      ZeidonCopyWithinString( szBlob, 1, lTemp, 600 );
   } 

   //:END

   //://KJS 07/25/07
   //://See if there is a WebControlProperty for "Hidden"
   //://I don't think "hidden" is a webcontrolproperty for the editbox yet
   //://but since I don't think this hurts I'm going to keep it in.
   //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Hidden"
   RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Hidden", "" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szWCP_Hidden = "Y"
      ZeidonStringCopy( szWCP_Hidden, 1, 0, "Y", 1, 0, 2 );
      //:ELSE
   } 
   else
   { 
      //:szWCP_Hidden = "N"
      ZeidonStringCopy( szWCP_Hidden, 1, 0, "N", 1, 0, 2 );
   } 

   //:END

   //:IF vDialog.Control.WebCtrlType = "Hidden" OR szWCP_Hidden = "Y"
   if ( CompareAttributeToString( vDialog, "Control", "WebCtrlType", "Hidden" ) == 0 || ZeidonStringCompare( szWCP_Hidden, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:szType = " type=^hidden^ "
      ZeidonStringCopy( szType, 1, 0, " type=^hidden^ ", 1, 0, 301 );
      //:ELSE
   } 
   else
   { 
      //:IF szBrowseFile = "Y"
      if ( ZeidonStringCompare( szBrowseFile, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:szType = " type=^file^ "
         ZeidonStringCopy( szType, 1, 0, " type=^file^ ", 1, 0, 301 );
         //:ELSE
      } 
      else
      { 
         //://IF vDialog.CtrlMapER_Domain EXISTS AND vDialog.CtrlMapER_Domain.Name = "Password"
         //:IF vDialog.CtrlMapER_Domain EXISTS AND vDialog.Control.Subtype = 4
         lTempInteger_2 = CheckExistenceOfEntity( vDialog, "CtrlMapER_Domain" );
         if ( lTempInteger_2 == 0 && CompareAttributeToInteger( vDialog, "Control", "Subtype", 4 ) == 0 )
         { 
            //:szType = " type=^password^ "
            ZeidonStringCopy( szType, 1, 0, " type=^password^ ", 1, 0, 301 );
            //:ELSE
         } 
         else
         { 
            //:IF szNumber = "Y"
            if ( ZeidonStringCompare( szNumber, 1, 0, "Y", 1, 0, 2 ) == 0 )
            { 
               //:szType = " type=^number^"
               ZeidonStringCopy( szType, 1, 0, " type=^number^", 1, 0, 301 );
               //:ELSE
            } 
            else
            { 
               //:szType = " type=^text^ "
               ZeidonStringCopy( szType, 1, 0, " type=^text^ ", 1, 0, 301 );
               //:IF szBlob != ""
               if ( ZeidonStringCompare( szBlob, 1, 0, "", 1, 0, 601 ) != 0 )
               { 
                  //:szType = szType + "mask=^"
                  ZeidonStringConcat( szType, 1, 0, "mask=^", 1, 0, 301 );
                  //:szType = szType + szBlob
                  ZeidonStringConcat( szType, 1, 0, szBlob, 1, 0, 301 );
                  //:szType = szType + "^ onblur=^return onBlurMask(this);^ onfocus=^return onFocusMask(this);^ onkeydown=^return doMask(this);^ "
                  ZeidonStringConcat( szType, 1, 0, "^ onblur=^return onBlurMask(this);^ onfocus=^return onFocusMask(this);^ onkeydown=^return doMask(this);^ ", 1, 0, 301 );
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

   //:// If there is an Event for the EditBox, build code to insert in <input statement.
   //:szEditActionCode = ""
   ZeidonStringCopy( szEditActionCode, 1, 0, "", 1, 0, 601 );
   //:FOR EACH vDialog.Event
   RESULT = SetCursorFirstEntity( vDialog, "Event", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:IF vDialog.EventAct EXISTS
      lTempInteger_3 = CheckExistenceOfEntity( vDialog, "EventAct" );
      if ( lTempInteger_3 == 0 )
      { 
         //:szActionName = vDialog.EventAct.Tag
         GetVariableFromAttribute( szActionName, 0, 'S', 33, vDialog, "EventAct", "Tag", "", 0 );
         //:IF vDialog.Event.Type = 16
         if ( CompareAttributeToInteger( vDialog, "Event", "Type", 16 ) == 0 )
         { 
            //:szEditActionCode = szEditActionCode + " onfocus=^" + szActionName + "( )^ "
            ZeidonStringConcat( szEditActionCode, 1, 0, " onfocus=^", 1, 0, 601 );
            ZeidonStringConcat( szEditActionCode, 1, 0, szActionName, 1, 0, 601 );
            ZeidonStringConcat( szEditActionCode, 1, 0, "( )^ ", 1, 0, 601 );
            //:ELSE
         } 
         else
         { 
            //:IF vDialog.Event.Type = 17
            if ( CompareAttributeToInteger( vDialog, "Event", "Type", 17 ) == 0 )
            { 
               //:szEditActionCode = szEditActionCode + " onblur=^" + szActionName + "( )^ "
               ZeidonStringConcat( szEditActionCode, 1, 0, " onblur=^", 1, 0, 601 );
               ZeidonStringConcat( szEditActionCode, 1, 0, szActionName, 1, 0, 601 );
               ZeidonStringConcat( szEditActionCode, 1, 0, "( )^ ", 1, 0, 601 );
               //:ELSE
            } 
            else
            { 
               //:IF vDialog.Event.Type = 18
               if ( CompareAttributeToInteger( vDialog, "Event", "Type", 18 ) == 0 )
               { 
                  //:szEditActionCode = szEditActionCode + " onchange=^" + szActionName + "( )^ "
                  ZeidonStringConcat( szEditActionCode, 1, 0, " onchange=^", 1, 0, 601 );
                  ZeidonStringConcat( szEditActionCode, 1, 0, szActionName, 1, 0, 601 );
                  ZeidonStringConcat( szEditActionCode, 1, 0, "( )^ ", 1, 0, 601 );
                  //:ELSE
               } 
               else
               { 
                  //:IF vDialog.Event.Type = 21
                  if ( CompareAttributeToInteger( vDialog, "Event", "Type", 21 ) == 0 )
                  { 
                     //:szEditActionCode = szEditActionCode + " onkeydown=^" + szActionName + "( )^ "
                     ZeidonStringConcat( szEditActionCode, 1, 0, " onkeydown=^", 1, 0, 601 );
                     ZeidonStringConcat( szEditActionCode, 1, 0, szActionName, 1, 0, 601 );
                     ZeidonStringConcat( szEditActionCode, 1, 0, "( )^ ", 1, 0, 601 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:IF vDialog.Event.Type = 22
                     if ( CompareAttributeToInteger( vDialog, "Event", "Type", 22 ) == 0 )
                     { 
                        //:szEditActionCode = szEditActionCode + " onkeyup=^" + szActionName + "( )^ "
                        ZeidonStringConcat( szEditActionCode, 1, 0, " onkeyup=^", 1, 0, 601 );
                        ZeidonStringConcat( szEditActionCode, 1, 0, szActionName, 1, 0, 601 );
                        ZeidonStringConcat( szEditActionCode, 1, 0, "( )^ ", 1, 0, 601 );
                        //:ELSE
                     } 
                     else
                     { 
                        //:IF vDialog.Event.Type = 24
                        if ( CompareAttributeToInteger( vDialog, "Event", "Type", 24 ) == 0 )
                        { 
                           //:szEditActionCode = szEditActionCode + " onclick=^" + szActionName + "( )^ "
                           ZeidonStringConcat( szEditActionCode, 1, 0, " onclick=^", 1, 0, 601 );
                           ZeidonStringConcat( szEditActionCode, 1, 0, szActionName, 1, 0, 601 );
                           ZeidonStringConcat( szEditActionCode, 1, 0, "( )^ ", 1, 0, 601 );
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
      } 

      RESULT = SetCursorNextEntity( vDialog, "Event", "" );
      //:END
   } 

   //:END

   //:   // If there is a Default button for the window, we must add some characters
   //:   // to the end of the input statement.
   //:   CreateViewFromView( vDialogRoot, vDialog )
   CreateViewFromView( &vDialogRoot, vDialog );
   //:   nRC = ResetViewFromSubobject( vDialogRoot )
   nRC = ResetViewFromSubobject( vDialogRoot );
   //:   LOOP WHILE nRC = 0
   while ( nRC == 0 )
   { 
      //:   nRC = ResetViewFromSubobject( vDialogRoot )
      nRC = ResetViewFromSubobject( vDialogRoot );
   } 

   //:   END
   //:   IF vDialogRoot.Window.DfltButton != ""
   if ( CompareAttributeToString( vDialogRoot, "Window", "DfltButton", "" ) != 0 )
   { 
      //:szEditActionCode = szEditActionCode + " onkeypress=^return _OnEnter( event )^"
      ZeidonStringConcat( szEditActionCode, 1, 0, " onkeypress=^return _OnEnter( event )^", 1, 0, 601 );
   } 

   //:   END
   //:   DropView( vDialogRoot )
   DropView( vDialogRoot );

   //:IF szNoOutputMapping = "Y"
   if ( ZeidonStringCompare( szNoOutputMapping, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:   szValue = " value=^^"
      ZeidonStringCopy( szValue, 1, 0, " value=^^", 1, 0, 301 );
      //:ELSE 
   } 
   else
   { 
      //:lStyleX = vDialog.Control.ExtendedStyle
      GetIntegerFromAttribute( &lStyleX, vDialog, "Control", "ExtendedStyle" );
      //:lTemp = IsFlagSequenceSet( lStyleX, zCONTROLX_PREFILL )
      lTemp = IsFlagSequenceSet( lStyleX, zCONTROLX_PREFILL );
      //:IF lTemp != 0 AND bMapping = 0
      if ( lTemp != 0 && bMapping == 0 )
      { 
         //:szValue = " value=^" + vDialog.Control.Text + "^"
         GetVariableFromAttribute( szTempString_14, 0, 'S', 255, vDialog, "Control", "Text", "", 0 );
         ZeidonStringCopy( szValue, 1, 0, " value=^", 1, 0, 301 );
         ZeidonStringConcat( szValue, 1, 0, szTempString_14, 1, 0, 301 );
         ZeidonStringConcat( szValue, 1, 0, "^", 1, 0, 301 );
         //:ELSE
      } 
      else
      { 
         //:szValue = " value=^<%=strErrorMapValue%>^"
         ZeidonStringCopy( szValue, 1, 0, " value=^<%=strErrorMapValue%>^", 1, 0, 301 );
      } 

      //:END
   } 

   //:END

   //:IF szNoOutputMapping = "Y"
   if ( ZeidonStringCompare( szNoOutputMapping, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:   szValue = "value=^^"
      ZeidonStringCopy( szValue, 1, 0, "value=^^", 1, 0, 301 );
      //:ELSE 
   } 
   else
   { 
      //:lStyleX = vDialog.Control.ExtendedStyle
      GetIntegerFromAttribute( &lStyleX, vDialog, "Control", "ExtendedStyle" );
      //:lTemp = IsFlagSequenceSet( lStyleX, zCONTROLX_PREFILL )
      lTemp = IsFlagSequenceSet( lStyleX, zCONTROLX_PREFILL );
      //:IF lTemp != 0 AND bMapping = 0
      if ( lTemp != 0 && bMapping == 0 )
      { 
         //:szValue = " value=^" + vDialog.Control.Text + "^"
         GetVariableFromAttribute( szTempString_15, 0, 'S', 255, vDialog, "Control", "Text", "", 0 );
         ZeidonStringCopy( szValue, 1, 0, " value=^", 1, 0, 301 );
         ZeidonStringConcat( szValue, 1, 0, szTempString_15, 1, 0, 301 );
         ZeidonStringConcat( szValue, 1, 0, "^", 1, 0, 301 );
         //:ELSE
      } 
      else
      { 
         //:szValue = " value=^<%=strErrorMapValue%>^"
         ZeidonStringCopy( szValue, 1, 0, " value=^<%=strErrorMapValue%>^", 1, 0, 301 );
      } 

      //:END
   } 

   //:END

   //://KJS 10/19/07 - Added title.
   //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = " title=^" + szTitle + "^ "
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END
   //:// KJS 08/23/16 - Doug and I both added placeholder. I had added a new attribute to tzwdlgso.lod
   //:// for this (WebPlaceholder) and I've only put this on the input field.
   //:// Doug has his on CommonDetail. For now we will keep both...
   //:// dks 2016.04.08 - trying placeholder
   //://szTitle = vDialog.Control.Placeholder
   //://IF szTitle != ""
   //://   szTitleHTML = szTitleHTML + " placeholder=^" + szTitle + "^"
   //://END

   //:szPlaceholder = ""
   ZeidonStringCopy( szPlaceholder, 1, 0, "", 1, 0, 257 );
   //:IF vDialog.Control.WebPlaceholder != ""
   if ( CompareAttributeToString( vDialog, "Control", "WebPlaceholder", "" ) != 0 )
   { 
      //:szPlaceholder = " placeholder=^" + vDialog.Control.WebPlaceholder + "^ "
      GetVariableFromAttribute( szTempString_16, 0, 'S', 255, vDialog, "Control", "WebPlaceholder", "", 0 );
      ZeidonStringCopy( szPlaceholder, 1, 0, " placeholder=^", 1, 0, 257 );
      ZeidonStringConcat( szPlaceholder, 1, 0, szTempString_16, 1, 0, 257 );
      ZeidonStringConcat( szPlaceholder, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:szHTML5Attr = ""
   ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
   //:IF vDialog.Control.WebHTML5Attribute != ""
   if ( CompareAttributeToString( vDialog, "Control", "WebHTML5Attribute", "" ) != 0 )
   { 
      //:szHTML5Attr = " " + vDialog.Control.WebHTML5Attribute + " "
      GetVariableFromAttribute( szTempString_17, 0, 'S', 255, vDialog, "Control", "WebHTML5Attribute", "", 0 );
      ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_17, 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
   } 

   //:END

   //:szClass = ""
   ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
   //:IF vDialog.Control.CSS_Class != ""
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) != 0 )
   { 
      //:szClass = " class=^" + vDialog.Control.CSS_Class + "^ "
      GetVariableFromAttribute( szTempString_18, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_18, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:CreateDisabledString( vDialog, szDisabled )
   CreateDisabledString( vDialog, szDisabled );
   //:szWriteBuffer = "<input" + szClass + " name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> " + szDisabled +
   //:                szAutoCompleteOff + szTitleHTML + szHTML5Attr + szPlaceholder + szStyle + szType + szValue + szEditActionCode + " >"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<input", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " name=^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szAutoCompleteOff, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szPlaceholder, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szType, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szValue, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szEditActionCode, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " >", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteGroupTable( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                        VIEW vGroupParent BASED ON LOD TZWDLGSO,
//:                        VIEW vDialogRoot BASED ON LOD TZWDLGSO,
//:                        INTEGER          lFile,
//:                        STRING ( 10000 ) szWriteBuffer,
//:                        STRING ( 100 )   szIndent,
//:                        INTEGER          lTableRowCnt,
//:                        STRING ( 1 )     szNoPositioning,
//:                        STRING ( 100 )   szRepeatGrpKey )

//:   INTEGER        X_Size
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteGroupTable( zVIEW     vDialog,
                        zVIEW     vGroupParent,
                        zVIEW     vDialogRoot,
                        zLONG     lFile,
                        zPCHAR    szWriteBuffer,
                        zPCHAR    szIndent,
                        zLONG     lTableRowCnt,
                        zPCHAR    szNoPositioning,
                        zPCHAR    szRepeatGrpKey )
{
   zLONG     X_Size = 0; 
   //:INTEGER        Y_Size
   zLONG     Y_Size = 0; 
   //:INTEGER        X_Pos
   zLONG     X_Pos = 0; 
   //:INTEGER        Y_Pos
   zLONG     Y_Pos = 0; 
   //:INTEGER        LastY_Pos
   zLONG     LastY_Pos = 0; 
   //:INTEGER        iHighestColCnt
   zLONG     iHighestColCnt = 0; 
   //:INTEGER        iColCnt
   zLONG     iColCnt = 0; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 
   //:INTEGER        iColWidth
   zLONG     iColWidth = 0; 
   //:INTEGER        lMaxStringLth
   zLONG     lMaxStringLth = 0; 
   //:INTEGER        lTemp
   zLONG     lTemp = 0; 
   //:INTEGER        lStyleX
   zLONG     lStyleX = 0; 
   //:INTEGER        Size
   zLONG     Size = 0; 
   //:INTEGER        lOffsetX
   zLONG     lOffsetX = 0; 
   //:INTEGER        lOffsetY
   zLONG     lOffsetY = 0; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:STRING ( 10 )  szColCnt
   zCHAR     szColCnt[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szWidth
   zCHAR     szWidth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szHeight
   zCHAR     szHeight[ 11 ] = { 0 }; 
   //:STRING ( 50 )  szCtrlTag
   zCHAR     szCtrlTag[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szParentCtrlTag
   zCHAR     szParentCtrlTag[ 51 ] = { 0 }; 
   //:STRING ( 85 )  szHTMLCtrlID
   zCHAR     szHTMLCtrlID[ 86 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szHTML5Attr
   zCHAR     szHTML5Attr[ 257 ] = { 0 }; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 32 )  szActionName
   zCHAR     szActionName[ 33 ] = { 0 }; 
   //:STRING ( 50 )  szControlType
   zCHAR     szControlType[ 51 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 500 ) szText
   zCHAR     szText[ 501 ] = { 0 }; 
   //:STRING ( 500 ) szText2
   zCHAR     szText2[ 501 ] = { 0 }; 
   //:STRING ( 256 ) szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 5 )   szSrch
   zCHAR     szSrch[ 6 ] = { 0 }; 
   //:STRING ( 32 )  szContextName
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 10 )  szMaxStringLth
   zCHAR     szMaxStringLth[ 11 ] = { 0 }; 
   //:STRING ( 32 )  szSize
   zCHAR     szSize[ 33 ] = { 0 }; 
   //:STRING ( 300 ) szAbsoluteStyle
   zCHAR     szAbsoluteStyle[ 301 ] = { 0 }; 

   //:STRING ( 1 )   szNoHeightFlag
   zCHAR     szNoHeightFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szNoWidthFlag
   zCHAR     szNoWidthFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szSpanFlag
   zCHAR     szSpanFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szDivFlag
   zCHAR     szDivFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szScrollableFlag
   zCHAR     szScrollableFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szScrollingTable
   zCHAR     szScrollingTable[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szGroupIsTable
   zCHAR     szGroupIsTable[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szreCAPTCHAFlag
   zCHAR     szreCAPTCHAFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szTinyMCEFlag
   zCHAR     szTinyMCEFlag[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szWindowIsjMobile
   zCHAR     szWindowIsjMobile[ 2 ] = { 0 }; 
   //:STRING ( 1 )   szToggleFlag
   zCHAR     szToggleFlag[ 2 ] = { 0 }; 
   //:STRING ( 100 ) szIndentGroup
   zCHAR     szIndentGroup[ 101 ] = { 0 }; 
   //:STRING ( 50 )  szWebCtrlType
   zCHAR     szWebCtrlType[ 51 ] = { 0 }; 
   zCHAR     szTempString_0[ 255 ]; 
   zCHAR     szTempString_1[ 255 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_2[ 255 ]; 
   zLONG     lTempInteger_2; 
   zCHAR     szTempString_3[ 255 ]; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 


   //:LastY_Pos = 0
   LastY_Pos = 0;
   //:iColCnt = 0
   iColCnt = 0;
   //:iHighestColCnt = 0
   iHighestColCnt = 0;
   //:szIndent = szIndent + "   "
   ZeidonStringConcat( szIndent, 1, 0, "   ", 1, 0, 101 );

   //:X_Size = vGroupParent.Control.SZDLG_X
   GetIntegerFromAttribute( &X_Size, vGroupParent, "Control", "SZDLG_X" );
   //:Y_Size = vGroupParent.Control.SZDLG_Y
   GetIntegerFromAttribute( &Y_Size, vGroupParent, "Control", "SZDLG_Y" );
   //:szParentCtrlTag = vGroupParent.Control.Tag
   GetVariableFromAttribute( szParentCtrlTag, 0, 'S', 51, vGroupParent, "Control", "Tag", "", 0 );

   //:IF vGroupParent.Control.CSS_Class != ""
   if ( CompareAttributeToString( vGroupParent, "Control", "CSS_Class", "" ) != 0 )
   { 
      //:szClass = " class=^" + vGroupParent.Control.CSS_Class + "^"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vGroupParent, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szClass = ""
      ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
   } 

   //:END


   //:IF vDialogRoot.WndStyle.Tag = "jMobile Window"
   if ( CompareAttributeToString( vDialogRoot, "WndStyle", "Tag", "jMobile Window" ) == 0 )
   { 
      //:szWindowIsjMobile = "Y"
      ZeidonStringCopy( szWindowIsjMobile, 1, 0, "Y", 1, 0, 2 );
      //:ELSE
   } 
   else
   { 
      //:szWindowIsjMobile = ""
      ZeidonStringCopy( szWindowIsjMobile, 1, 0, "", 1, 0, 2 );
   } 

   //:END

   //:   
   //:// KJS 02/23/16 - We have added a field for HTML5 attributes (this could be for jQuery Mobile or whatever extra attributes we want).
   //:// It is simply a string and we will add it to different controls like <div> or <input> etc.
   //:szHTML5Attr = ""
   ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
   //:IF vGroupParent.Control.WebHTML5Attribute != ""
   if ( CompareAttributeToString( vGroupParent, "Control", "WebHTML5Attribute", "" ) != 0 )
   { 
      //:szHTML5Attr = " " + vGroupParent.Control.WebHTML5Attribute + " "
      GetVariableFromAttribute( szTempString_1, 0, 'S', 255, vGroupParent, "Control", "WebHTML5Attribute", "", 0 );
      ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_1, 1, 0, 257 );
      ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
   } 

   //:END

   //:IF vDialog.Control EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDialog, "Control" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SET CURSOR  FIRST vDialog.Control
      RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
      //:LastY_Pos = vDialog.Control.PSDLG_Y
      GetIntegerFromAttribute( &LastY_Pos, vDialog, "Control", "PSDLG_Y" );
   } 

   //:END

   //:// Loop through each Control within this group.  Determine what row has the largest
   //:// number of controls.  This will determine how many columns our table will have.
   //:FOR EACH vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:IF LastY_Pos < vDialog.Control.PSDLG_Y
      if ( CompareAttributeToInteger( vDialog, "Control", "PSDLG_Y", LastY_Pos ) > 0 )
      { 

         //:IF  iColCnt > iHighestColCnt
         if ( iColCnt > iHighestColCnt )
         { 
            //:iHighestColCnt = iColCnt
            iHighestColCnt = iColCnt;
         } 

         //:END
         //:iColCnt = 0
         iColCnt = 0;
      } 

      //:END

      //:iColCnt = iColCnt + 1
      iColCnt = iColCnt + 1;

      //:LastY_Pos = vDialog.Control.PSDLG_Y
      GetIntegerFromAttribute( &LastY_Pos, vDialog, "Control", "PSDLG_Y" );
      RESULT = SetCursorNextEntity( vDialog, "Control", "" );
   } 

   //:END

   //://TraceLineI("**** Highest Col Count *** ", iHighestColCnt )
   //:zIntegerToString( szColCnt, 10, iHighestColCnt )
   zIntegerToString( szColCnt, 10, iHighestColCnt );
   //:PIX_PER_DU( vDialog, dDLUnits )
   PIX_PER_DU( vDialog, &dDLUnits );
   //:zIntegerToString( szWidth, 10, X_Size * dDLUnits )
   zIntegerToString( szWidth, 10, X_Size * dDLUnits );
   //:zIntegerToString( szHeight, 10, Y_Size * dDLUnits )
   zIntegerToString( szHeight, 10, Y_Size * dDLUnits );

   //:// 05/27/09 - Currently, we never put in a height.  Would we want to do this?????!!!.
   //:IF szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 

      //:szNoWidthFlag = ""
      ZeidonStringCopy( szNoWidthFlag, 1, 0, "", 1, 0, 2 );
      //:SET CURSOR FIRST vGroupParent.WebControlProperty WHERE vGroupParent.WebControlProperty.Name = "No Width"
      RESULT = SetCursorFirstEntityByString( vGroupParent, "WebControlProperty", "Name", "No Width", "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:szNoWidthFlag = "Y"
         ZeidonStringCopy( szNoWidthFlag, 1, 0, "Y", 1, 0, 2 );
      } 

      //:END
   } 

   //:END

   //:// Create a div around the whole table.  This should probably be float.
   //://szWriteBuffer = "<div style=^float:left;width:" + szWidth + "px;height:" + szHeight + "px;^" + szClass + ">"
   //:// KJS 09/11/09 - Neither the table or the div had an id or name.  I am not sure whether I should put the
   //:// id on the <table we create or on the <div> but I will put it on the div.
   //:IF szNoPositioning = "S"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
   { 
      //:szWriteBuffer = "<div id=^" + szParentCtrlTag + "^ name=^" + szParentCtrlTag + "^ "
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szParentCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szParentCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "<div id=^" + szParentCtrlTag + "^ name=^" + szParentCtrlTag + "^ style=^float:left;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szParentCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szParentCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ style=^float:left;", 1, 0, 10001 );
      //:IF szNoWidthFlag = ""
      if ( ZeidonStringCompare( szNoWidthFlag, 1, 0, "", 1, 0, 2 ) == 0 )
      { 
         //:szWriteBuffer = szWriteBuffer + "width:" + szWidth + "px;^"
         ZeidonStringConcat( szWriteBuffer, 1, 0, "width:", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szWidth, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "px;^", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = szWriteBuffer + "^"
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^", 1, 0, 10001 );
      } 

      //:END
   } 

   //:END
   //:// KJS 02/23/16 - We have added a field for HTML5 attributes, but I am not sure whether this should be
   //:// here on the <div> (which surrounds the table) or whether this should be on the <table>. For now since the
   //:// class is here I will also put the HTLM5 attribute here.
   //:szWriteBuffer = szWriteBuffer + szHTML5Attr + " " + szClass + ">"
   ZeidonStringConcat( szWriteBuffer, 1, 0, szHTML5Attr, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:// Now create the table with the proper number of columns.
   //://szWriteBuffer = "<table cols=" szColCount + " name=^" + szCtrlTag + "^ id=^" + szCtrlTag + "^>"
   //:IF szWindowIsjMobile = "" 
   if ( ZeidonStringCompare( szWindowIsjMobile, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:szWriteBuffer = "<table cols=" + szColCnt
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<table cols=", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szColCnt, 1, 0, 10001 );
      //:IF szNoWidthFlag = "" AND szNoPositioning != "S"
      if ( ZeidonStringCompare( szNoWidthFlag, 1, 0, "", 1, 0, 2 ) == 0 && ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) != 0 )
      { 
         //:szWriteBuffer = szWriteBuffer + " style=^width:" + szWidth + "px;^ "
         ZeidonStringConcat( szWriteBuffer, 1, 0, " style=^width:", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szWidth, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "px;^ ", 1, 0, 10001 );
      } 

      //:END
      //:szWriteBuffer = szWriteBuffer + " class=^grouptable^>"
      ZeidonStringConcat( szWriteBuffer, 1, 0, " class=^grouptable^>", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:// page is being generated for jmobile, so we don't want the grouptable or columns.
      //:szWriteBuffer = "<table data-role=^table^> "
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<table data-role=^table^> ", 1, 0, 10001 );
      //:szWriteBuffer = "<table data-role=^listview^> " // some reason "table" was causing page to hang.
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<table data-role=^listview^> ", 1, 0, 10001 );
      //:szWriteBuffer = "<table> " // now try nothing
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<table> ", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:szWriteBuffer = "<thead></thead>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<thead></thead>", 1, 0, 10001 );
   } 

   //:END
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:szWriteBuffer = "<tr>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<tr>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:LastY_Pos = 0
   LastY_Pos = 0;
   //:IF  vDialog.Control EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( vDialog, "Control" );
   if ( lTempInteger_1 == 0 )
   { 
      //:SET CURSOR  FIRST vDialog.Control
      RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
      //:LastY_Pos = vDialog.Control.PSDLG_Y
      GetIntegerFromAttribute( &LastY_Pos, vDialog, "Control", "PSDLG_Y" );
   } 

   //:END


   //:// Loop through the controls, placing each control in a different column.
   //:// When the Y_Pos has changed, we need to create a new row in the table.
   //:FOR EACH vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// We are onto a new row
      //:IF  LastY_Pos < vDialog.Control.PSDLG_Y
      if ( CompareAttributeToInteger( vDialog, "Control", "PSDLG_Y", LastY_Pos ) > 0 )
      { 

         //:// If LastY_Pos is 0 then we are on the first control.
         //:// If this wasn't the very first row, then
         //:// create the end tag for the last row.
         //://IF  LastY_Pos != 0
         //:   // If the previous row didn't have as many controls as there are
         //:   // table columns, then we need to create a blank column
         //:   LOOP  WHILE iColCnt < iHighestColCnt
         while ( iColCnt < iHighestColCnt )
         { 
            //:   szWriteBuffer = "<td>&nbsp</td>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<td>&nbsp</td>", 1, 0, 10001 );
            //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:   iColCnt = iColCnt + 1
            iColCnt = iColCnt + 1;
         } 

         //:   END
         //:   szWriteBuffer = "</tr>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "</tr>", 1, 0, 10001 );
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //://END

         //:szWriteBuffer = "<tr>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<tr>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:// Since we are on a new row, reset the column count.
         //:iColCnt = 0
         iColCnt = 0;
      } 

      //:END

      //:szCtrlTag = vDialog.Control.Tag
      GetVariableFromAttribute( szCtrlTag, 0, 'S', 51, vDialog, "Control", "Tag", "", 0 );
      //:szHTMLCtrlID = " id=^" + szCtrlTag + "^ name=^" + szCtrlTag + "^ "
      ZeidonStringCopy( szHTMLCtrlID, 1, 0, " id=^", 1, 0, 86 );
      ZeidonStringConcat( szHTMLCtrlID, 1, 0, szCtrlTag, 1, 0, 86 );
      ZeidonStringConcat( szHTMLCtrlID, 1, 0, "^ name=^", 1, 0, 86 );
      ZeidonStringConcat( szHTMLCtrlID, 1, 0, szCtrlTag, 1, 0, 86 );
      ZeidonStringConcat( szHTMLCtrlID, 1, 0, "^ ", 1, 0, 86 );
      //:szControlType = vDialog.ControlDef.Tag
      GetVariableFromAttribute( szControlType, 0, 'S', 51, vDialog, "ControlDef", "Tag", "", 0 );
      //:CreateNoPosStyleString( vDialog, szStyle, "" )
      CreateNoPosStyleString( vDialog, szStyle, "" );


      //:// KJS 02/23/16 - We have added a field for HTML5 attributes (this could be for jQuery Mobile or whatever extra attributes we want).
      //:// It is simply a string and we will add it to different controls like <div> or <input> etc.
      //:szHTML5Attr = ""
      ZeidonStringCopy( szHTML5Attr, 1, 0, "", 1, 0, 257 );
      //:IF vDialog.Control.WebHTML5Attribute != ""
      if ( CompareAttributeToString( vDialog, "Control", "WebHTML5Attribute", "" ) != 0 )
      { 
         //:szHTML5Attr = " " + vDialog.Control.WebHTML5Attribute + " "
         GetVariableFromAttribute( szTempString_2, 0, 'S', 255, vDialog, "Control", "WebHTML5Attribute", "", 0 );
         ZeidonStringCopy( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
         ZeidonStringConcat( szHTML5Attr, 1, 0, szTempString_2, 1, 0, 257 );
         ZeidonStringConcat( szHTML5Attr, 1, 0, " ", 1, 0, 257 );
      } 

      //:END


      //:X_Pos = vDialog.Control.PSDLG_X
      GetIntegerFromAttribute( &X_Pos, vDialog, "Control", "PSDLG_X" );
      //://TraceLineI("*** Control Start position *** ", vDialog.Control.PSDLG_X )
      //:LastY_Pos = vDialog.Control.PSDLG_Y
      GetIntegerFromAttribute( &LastY_Pos, vDialog, "Control", "PSDLG_Y" );

      //:// We want to find out how wide this column should bed
      //:nRC = SetCursorNextEntity( vDialog, "Control", "" )
      nRC = SetCursorNextEntity( vDialog, "Control", "" );
      //:iColWidth = 0
      iColWidth = 0;
      //:IF  nRC >= zCURSOR_SET AND LastY_Pos = vDialog.Control.PSDLG_Y
      if ( nRC >= zCURSOR_SET && CompareAttributeToInteger( vDialog, "Control", "PSDLG_Y", LastY_Pos ) == 0 )
      { 
         //://TraceLineI("*** Next Control Start position *** ", vDialog.Control.PSDLG_X )
         //:iColWidth = vDialog.Control.PSDLG_X - X_Pos
         GetIntegerFromAttribute( &lTempInteger_2, vDialog, "Control", "PSDLG_X" );
         iColWidth = lTempInteger_2 - X_Pos;
      } 

      //:END

      //:IF  nRC >= zCURSOR_SET
      if ( nRC >= zCURSOR_SET )
      { 
         //:nRC = SetCursorPrevEntity( vDialog, "Control", "" )
         nRC = SetCursorPrevEntity( vDialog, "Control", "" );
      } 

      //:END

      //:IF  iColWidth = 0
      if ( iColWidth == 0 )
      { 
         //://TraceLineI("*** Width is width of control *** ", vDialog.Control.SZDLG_X )
         //:iColWidth = vDialog.Control.SZDLG_X
         GetIntegerFromAttribute( &iColWidth, vDialog, "Control", "SZDLG_X" );
      } 

      //:END

      //:szClass = vDialog.Control.CSS_Class
      GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
      //:IF vDialog.Control.CSS_Class != ""
      if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) != 0 )
      { 
         //:szClass = " class=^" + vDialog.Control.CSS_Class + "^ "
         GetVariableFromAttribute( szTempString_3, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
         ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, szTempString_3, 1, 0, 257 );
         ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szClass = ""
         ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
      } 

      //:END

      //://KELLY0210
      //:zIntegerToString( szWidth, 10, iColWidth * dDLUnits )
      zIntegerToString( szWidth, 10, iColWidth * dDLUnits );
      //:// KJS 02/23/17 - Both of the following <td had valign=^top^. This is no longer supported in HTML5 and several browsers
      //:// so I think it might be useless. Going to try and take it out...
      //:// I see it says to use <td style="vertical-align:bottom"> instead but I will keep it out for the moment since I don't think it worked.
      //:IF szNoPositioning = "S"  // No style information
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szWriteBuffer = "<td " + szClass + ">"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<td ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         //:ELSE 
      } 
      else
      { 
         //:szWriteBuffer = "<td " + szClass + "style=^width:" + szWidth + "px;^>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<td ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "style=^width:", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szWidth, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "px;^>", 1, 0, 10001 );
      } 

      //:END
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //://****************************************************************************

      //:IF szControlType = "GroupBox"
      if ( ZeidonStringCompare( szControlType, 1, 0, "GroupBox", 1, 0, 51 ) == 0 )
      { 

         //:// Do I need to check if this is a groupbox and we are not using absolute positioning, then I want to
         //:// check if there are other groupboxes at this same PSDLG_Y.  If so, then these groups would need to
         //:// have a float:left applied to them.
         //:// Would I only want to do this once for the row?
         //:// Or, do I want to put a float on every div created and then create a <div style="clear:both;"</div> after
         //:// every row?
         //://IF  szNoPositioning = "Y" AND szCheckedRowForGroups         //END


         //:// InitializeRepeatingGroup for GroupBox

         //:// Check to see if this GroupBox has a WebControlProperty of "Table".  If so, then we will create this
         //:// as a table and try and position what is in this group box as a table.
         //:szGroupIsTable = ""
         ZeidonStringCopy( szGroupIsTable, 1, 0, "", 1, 0, 2 );
         //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Table"
         RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Table", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szGroupIsTable = "Y"
            ZeidonStringCopy( szGroupIsTable, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END

         //:szNoHeightFlag = ""
         ZeidonStringCopy( szNoHeightFlag, 1, 0, "", 1, 0, 2 );
         //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "No Height"
         RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "No Height", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szNoHeightFlag = "Y"
            ZeidonStringCopy( szNoHeightFlag, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END

         //:szNoWidthFlag = ""
         ZeidonStringCopy( szNoWidthFlag, 1, 0, "", 1, 0, 2 );
         //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "No Width"
         RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "No Width", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szNoWidthFlag = "Y"
            ZeidonStringCopy( szNoWidthFlag, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END

         //:szreCAPTCHAFlag = ""
         ZeidonStringCopy( szreCAPTCHAFlag, 1, 0, "", 1, 0, 2 );
         //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "reCAPTCHA"
         RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "reCAPTCHA", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szreCAPTCHAFlag = "Y"
            ZeidonStringCopy( szreCAPTCHAFlag, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END

         //:szToggleFlag = "" 
         ZeidonStringCopy( szToggleFlag, 1, 0, "", 1, 0, 2 );
         //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Show/Hide Toggle"
         RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Show/Hide Toggle", "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:szToggleFlag = "Y"
            ZeidonStringCopy( szToggleFlag, 1, 0, "Y", 1, 0, 2 );
         } 

         //:END
         //:IF szWebCtrlType = "Span"
         if ( ZeidonStringCompare( szWebCtrlType, 1, 0, "Span", 1, 0, 51 ) == 0 )
         { 
            //:szSpanFlag = "Y"
            ZeidonStringCopy( szSpanFlag, 1, 0, "Y", 1, 0, 2 );
            //:ELSE
         } 
         else
         { 
            //:szSpanFlag = ""
            ZeidonStringCopy( szSpanFlag, 1, 0, "", 1, 0, 2 );
            //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Span"
            RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Span", "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:szSpanFlag = "Y"
               ZeidonStringCopy( szSpanFlag, 1, 0, "Y", 1, 0, 2 );
            } 

            //:END
         } 

         //:END
         //:IF szWebCtrlType = "Div"
         if ( ZeidonStringCompare( szWebCtrlType, 1, 0, "Div", 1, 0, 51 ) == 0 )
         { 
            //:szDivFlag = "Y"
            ZeidonStringCopy( szDivFlag, 1, 0, "Y", 1, 0, 2 );
            //:ELSE
         } 
         else
         { 
            //:szDivFlag = ""
            ZeidonStringCopy( szDivFlag, 1, 0, "", 1, 0, 2 );
            //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Div"
            RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Div", "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:szDivFlag = "Y"
               ZeidonStringCopy( szDivFlag, 1, 0, "Y", 1, 0, 2 );
            } 

            //:END
         } 

         //:END
      } 


      //:END

      //:// Initialize the szAbsoluteStyle variable with X and Y values.
      //:IF  szNoPositioning = "Y"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:IF szControlType = "GroupBox" AND szNoHeightFlag = "Y" AND szNoWidthFlag = "Y"
         if ( ZeidonStringCompare( szControlType, 1, 0, "GroupBox", 1, 0, 51 ) == 0 && ZeidonStringCompare( szNoHeightFlag, 1, 0, "Y", 1, 0, 2 ) == 0 && ZeidonStringCompare( szNoWidthFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
         { 
            //:szAbsoluteStyle = ""
            ZeidonStringCopy( szAbsoluteStyle, 1, 0, "", 1, 0, 301 );
            //:ELSE
         } 
         else
         { 
            //:IF szControlType = "GroupBox" AND szNoHeightFlag = "Y"
            if ( ZeidonStringCompare( szControlType, 1, 0, "GroupBox", 1, 0, 51 ) == 0 && ZeidonStringCompare( szNoHeightFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
            { 
               //:// We might not want the groupbox to have a height (so it grows)
               //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
               GetIntegerFromAttribute( &lTempInteger_3, vDialog, "Control", "SZDLG_X" );
               zIntegerToString( szWidth, 10, lTempInteger_3 * dDLUnits );
               //:szAbsoluteStyle = "style=^width:" + szWidth + ";^"
               ZeidonStringCopy( szAbsoluteStyle, 1, 0, "style=^width:", 1, 0, 301 );
               ZeidonStringConcat( szAbsoluteStyle, 1, 0, szWidth, 1, 0, 301 );
               ZeidonStringConcat( szAbsoluteStyle, 1, 0, ";^", 1, 0, 301 );
               //:ELSE
            } 
            else
            { 
               //:IF szControlType = "GroupBox" AND szNoWidthFlag = "Y"
               if ( ZeidonStringCompare( szControlType, 1, 0, "GroupBox", 1, 0, 51 ) == 0 && ZeidonStringCompare( szNoWidthFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 
                  //:// We might not want the groupbox to have a width (so it grows)
                  //:zIntegerToString( szHeight, 10, vDialog.Control.SZDLG_Y * dDLUnits )
                  GetIntegerFromAttribute( &lTempInteger_4, vDialog, "Control", "SZDLG_Y" );
                  zIntegerToString( szHeight, 10, lTempInteger_4 * dDLUnits );
                  //:szAbsoluteStyle = "style=^height:" + szHeight + ";^"
                  ZeidonStringCopy( szAbsoluteStyle, 1, 0, "style=^height:", 1, 0, 301 );
                  ZeidonStringConcat( szAbsoluteStyle, 1, 0, szHeight, 1, 0, 301 );
                  ZeidonStringConcat( szAbsoluteStyle, 1, 0, ";^", 1, 0, 301 );
                  //:ELSE
               } 
               else
               { 
                  //:CreateNoPosStyleString( vDialog, szAbsoluteStyle, "" )
                  CreateNoPosStyleString( vDialog, szAbsoluteStyle, "" );
               } 

               //:END
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:IF szNoPositioning = "S"
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
         { 
            //:szAbsoluteStyle = "" // No style information.
            ZeidonStringCopy( szAbsoluteStyle, 1, 0, "", 1, 0, 301 );
            //:ELSE 
         } 
         else
         { 
            //:CreateAbsolStyleString( vDialog, szAbsoluteStyle, lOffsetX, lOffsetY, "" )
            CreateAbsolStyleString( vDialog, szAbsoluteStyle, lOffsetX, lOffsetY, "" );
         } 

         //:END
      } 

      //:END

      //:szWriteBuffer = "<% /* " + szCtrlTag + ":" + szControlType + " */ %>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<% /* ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ":", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szControlType, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " */ %>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:// Check to see if this GroupBox has a WebControlProperty of "Table".  If so, then we will create this
      //:// as a table and try and position what is in this group box as a table.
      //:IF szControlType = "GroupBox" AND szGroupIsTable = "Y"
      if ( ZeidonStringCompare( szControlType, 1, 0, "GroupBox", 1, 0, 51 ) == 0 && ZeidonStringCompare( szGroupIsTable, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 

         //:X_Size = vDialog.Control.SZDLG_X
         GetIntegerFromAttribute( &X_Size, vDialog, "Control", "SZDLG_X" );
         //:Y_Size = vDialog.Control.SZDLG_Y
         GetIntegerFromAttribute( &Y_Size, vDialog, "Control", "SZDLG_Y" );

         //:OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
         OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" );
         //:CreateViewFromView( vGroupParent, vDialog )
         CreateViewFromView( &vGroupParent, vDialog );

         //:// Create view for Group as a parent. This will be used in processing subcontrols to check for WebControlProperty.
         //:SetViewToSubobject( vDialog, "CtrlCtrl" )
         SetViewToSubobject( vDialog, "CtrlCtrl" );
         //:szIndentGroup = szIndent
         ZeidonStringCopy( szIndentGroup, 1, 0, szIndent, 1, 0, 101 );
         //:GenJSPJ_CrteGroupTable( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndent, lTableRowCnt, szNoPositioning, szRepeatGrpKey )
         GenJSPJ_CrteGroupTable( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndent, lTableRowCnt, szNoPositioning, szRepeatGrpKey );
         //:ResetViewFromSubobject( vDialog )
         ResetViewFromSubobject( vDialog );
         //:DropView( vGroupParent )
         DropView( vGroupParent );
         //:vGroupParent = 0
         vGroupParent = 0;
      } 

      //:END

      //:IF szControlType = "GroupBox" AND szGroupIsTable = ""
      if ( ZeidonStringCompare( szControlType, 1, 0, "GroupBox", 1, 0, 51 ) == 0 && ZeidonStringCompare( szGroupIsTable, 1, 0, "", 1, 0, 2 ) == 0 )
      { 

         //:// Set Group flags either from old szWebCtrlType or new WebControlProperty.
         //:IF szWebCtrlType = "Scrollable"
         if ( ZeidonStringCompare( szWebCtrlType, 1, 0, "Scrollable", 1, 0, 51 ) == 0 )
         { 
            //:szScrollableFlag = "Y"
            ZeidonStringCopy( szScrollableFlag, 1, 0, "Y", 1, 0, 2 );
            //:ELSE
         } 
         else
         { 
            //:szScrollableFlag = ""
            ZeidonStringCopy( szScrollableFlag, 1, 0, "", 1, 0, 2 );
            //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Scrollable"
            RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Scrollable", "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:szScrollableFlag = "Y"
               ZeidonStringCopy( szScrollableFlag, 1, 0, "Y", 1, 0, 2 );
            } 

            //:END
         } 

         //:END
         //:IF szWebCtrlType = "Span"
         if ( ZeidonStringCompare( szWebCtrlType, 1, 0, "Span", 1, 0, 51 ) == 0 )
         { 
            //:szSpanFlag = "Y"
            ZeidonStringCopy( szSpanFlag, 1, 0, "Y", 1, 0, 2 );
            //:ELSE
         } 
         else
         { 
            //:szSpanFlag = ""
            ZeidonStringCopy( szSpanFlag, 1, 0, "", 1, 0, 2 );
            //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Span"
            RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Span", "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:szSpanFlag = "Y"
               ZeidonStringCopy( szSpanFlag, 1, 0, "Y", 1, 0, 2 );
            } 

            //:END
         } 

         //:END
         //:IF szWebCtrlType = "Div"
         if ( ZeidonStringCompare( szWebCtrlType, 1, 0, "Div", 1, 0, 51 ) == 0 )
         { 
            //:szDivFlag = "Y"
            ZeidonStringCopy( szDivFlag, 1, 0, "Y", 1, 0, 2 );
            //:ELSE
         } 
         else
         { 
            //:szDivFlag = ""
            ZeidonStringCopy( szDivFlag, 1, 0, "", 1, 0, 2 );
            //:SET CURSOR FIRST vDialog.WebControlProperty WHERE vDialog.WebControlProperty.Name = "Div"
            RESULT = SetCursorFirstEntityByString( vDialog, "WebControlProperty", "Name", "Div", "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:szDivFlag = "Y"
               ZeidonStringCopy( szDivFlag, 1, 0, "Y", 1, 0, 2 );
            } 

            //:END
         } 

         //:END


         //:// Is this clear even working???!!!
         //:IF szWebCtrlType = "Clear"
         if ( ZeidonStringCompare( szWebCtrlType, 1, 0, "Clear", 1, 0, 51 ) == 0 )
         { 
            //:szWriteBuffer = "<div id='clear'></div>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<div id='clear'></div>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
            //:X_Size = vDialog.Control.PSDLG_X * dDLUnits + lOffsetX  // borrow these for a second
            GetIntegerFromAttribute( &lTempInteger_5, vDialog, "Control", "PSDLG_X" );
            X_Size = lTempInteger_5 * dDLUnits + lOffsetX;
            //:Y_Size = vDialog.Control.PSDLG_Y * dDLUnits + lOffsetY
            GetIntegerFromAttribute( &lTempInteger_6, vDialog, "Control", "PSDLG_Y" );
            Y_Size = lTempInteger_6 * dDLUnits + lOffsetY;
            //:OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
            OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" );
            //:// Create view for Group as a parent. This will be used in processing subcontrols to check for WebControlProperty.
            //:CreateViewFromView( vGroupParent, vDialog )
            CreateViewFromView( &vGroupParent, vDialog );
            //:SetViewToSubobject( vDialog, "CtrlCtrl" )
            SetViewToSubobject( vDialog, "CtrlCtrl" );
            //:szIndentGroup = szIndent
            ZeidonStringCopy( szIndentGroup, 1, 0, szIndent, 1, 0, 101 );
            //:GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup,
            //:                         lTableRowCnt, szNoPositioning, X_Size, Y_Size, szRepeatGrpKey )
            GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup, lTableRowCnt, szNoPositioning, X_Size, Y_Size, szRepeatGrpKey );
            //:ResetViewFromSubobject( vDialog )
            ResetViewFromSubobject( vDialog );
            //:DropView( vGroupParent )
            DropView( vGroupParent );
            //:vGroupParent = 0
            vGroupParent = 0;
            //:szWriteBuffer = ""
            ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:ELSE
         } 
         else
         { 
            //:IF szSpanFlag = "Y"
            if ( ZeidonStringCompare( szSpanFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
            { 

               //:// Div on a Group Control requests that all the controls inside the group line up one under the other.
               //:szClass = vDialog.Control.CSS_Class
               GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
               //:IF szClass = ""
               if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
               { 
                  //:szWriteBuffer = "<span" + szHTMLCtrlID + szAbsoluteStyle + ">"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "<span", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
                  //:ELSE
               } 
               else
               { 
                  //:szWriteBuffer = "<span" + szHTMLCtrlID + "class=^" + szClass + "^ " + szAbsoluteStyle + ">"
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "<span", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "class=^", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
               } 

               //:END
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

               //:OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
               OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" );
               //:// Create view for Group as a parent. This will be used in processing subcontrols to check for WebControlProperty.
               //:CreateViewFromView( vGroupParent, vDialog )
               CreateViewFromView( &vGroupParent, vDialog );
               //:SetViewToSubobject( vDialog, "CtrlCtrl" )
               SetViewToSubobject( vDialog, "CtrlCtrl" );
               //:szIndentGroup = szIndent
               ZeidonStringCopy( szIndentGroup, 1, 0, szIndent, 1, 0, 101 );
               //:GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup,
               //:                      lTableRowCnt, szNoPositioning, lOffsetX, lOffsetY, szRepeatGrpKey )
               GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup, lTableRowCnt, szNoPositioning, lOffsetX, lOffsetY, szRepeatGrpKey );
               //:ResetViewFromSubobject( vDialog )
               ResetViewFromSubobject( vDialog );
               //:DropView( vGroupParent )
               DropView( vGroupParent );
               //:vGroupParent = 0
               vGroupParent = 0;
               //:szWriteBuffer = "</span>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "</span>", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:ELSE
            } 
            else
            { 
               //:IF szDivFlag = "Y"
               if ( ZeidonStringCompare( szDivFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
               { 

                  //:// Add Scrollable if requested.
                  //:IF szScrollableFlag = "Y"
                  if ( ZeidonStringCompare( szScrollableFlag, 1, 0, "Y", 1, 0, 2 ) == 0 )
                  { 
                     //:zAppendQuotedString( szAbsoluteStyle, "overflow:auto;", "style=", "^" )
                     zAppendQuotedString( szAbsoluteStyle, "overflow:auto;", "style=", "^" );
                  } 

                  //:END

                  //:// If we are not using absolute positioning, then make all the divs
                  //:// float left.
                  //:IF szNoPositioning = "Y"
                  if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
                  { 
                     //:zAppendQuotedString( szAbsoluteStyle, "float:left;", "style=", "^" )
                     zAppendQuotedString( szAbsoluteStyle, "float:left;", "style=", "^" );
                     //:ELSE
                  } 
                  else
                  { 
                     //:IF szNoPositioning = "S"
                     if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
                     { 
                        //:szAbsoluteStyle = "" // No style information.             
                        ZeidonStringCopy( szAbsoluteStyle, 1, 0, "", 1, 0, 301 );
                     } 

                     //:END
                  } 

                  //:END

                  //:// Div on a Group Control requests that all the controls inside the group line up one under the other.
                  //:szClass = vDialog.Control.CSS_Class
                  GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
                  //:IF szClass = ""
                  if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
                  { 
                     //:szWriteBuffer = "<div" + szHTMLCtrlID + szAbsoluteStyle + ">"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "<div", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:szWriteBuffer = "<div" + szHTMLCtrlID + "class=^" + szClass + "^ " + szAbsoluteStyle + ">"
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "<div", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, "class=^", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
                  } 

                  //:END
                  //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
                  WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

                  //:OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
                  OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" );
                  //:// Create view for Group as a parent. This will be used in processing subcontrols to check for WebControlProperty.
                  //:CreateViewFromView( vGroupParent, vDialog )
                  CreateViewFromView( &vGroupParent, vDialog );
                  //:SetViewToSubobject( vDialog, "CtrlCtrl" )
                  SetViewToSubobject( vDialog, "CtrlCtrl" );
                  //:szIndentGroup = szIndent
                  ZeidonStringCopy( szIndentGroup, 1, 0, szIndent, 1, 0, 101 );
                  //:GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup,
                  //:                   lTableRowCnt, szNoPositioning, lOffsetX, lOffsetY, szRepeatGrpKey )
                  GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup, lTableRowCnt, szNoPositioning, lOffsetX, lOffsetY, szRepeatGrpKey );
                  //:ResetViewFromSubobject( vDialog )
                  ResetViewFromSubobject( vDialog );
                  //:DropView( vGroupParent )
                  DropView( vGroupParent );
                  //:vGroupParent = 0
                  vGroupParent = 0;
                  //:szWriteBuffer = "</div>  <!-- " + szCtrlTag + " --> "
                  ZeidonStringCopy( szWriteBuffer, 1, 0, "</div>  <!-- ", 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
                  ZeidonStringConcat( szWriteBuffer, 1, 0, " --> ", 1, 0, 10001 );
                  //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
                  WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
                  //:ELSE
               } 
               else
               { 
                  //:IF szWebCtrlType = "DivScroll"
                  if ( ZeidonStringCompare( szWebCtrlType, 1, 0, "DivScroll", 1, 0, 51 ) == 0 )
                  { 
                     //:// Div on a Group Control requests that all the controls inside the group line up one under the other.
                     //:zAppendQuotedString( szAbsoluteStyle, "overflow:auto;", "style=", "^" )
                     zAppendQuotedString( szAbsoluteStyle, "overflow:auto;", "style=", "^" );

                     //:// If we are not using absolute positioning, then make all the divs
                     //:// float left.
                     //:IF szNoPositioning = "Y"
                     if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
                     { 
                        //:zAppendQuotedString( szAbsoluteStyle, "float:left;", "style=", "^" )
                        zAppendQuotedString( szAbsoluteStyle, "float:left;", "style=", "^" );
                     } 

                     //:END
                     //:IF szNoPositioning = "S"
                     if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
                     { 
                        //:szAbsoluteStyle = "" // No style information.             
                        ZeidonStringCopy( szAbsoluteStyle, 1, 0, "", 1, 0, 301 );
                     } 

                     //:END
                     //:szClass = vDialog.Control.CSS_Class
                     GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
                     //:IF szClass = ""
                     if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
                     { 
                        //:szWriteBuffer = "<div" + szHTMLCtrlID + szAbsoluteStyle + ">"
                        ZeidonStringCopy( szWriteBuffer, 1, 0, "<div", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
                        //:ELSE
                     } 
                     else
                     { 
                        //:szWriteBuffer = "<div" + szHTMLCtrlID + "class=^" + szClass + "^ " + szAbsoluteStyle + ">"
                        ZeidonStringCopy( szWriteBuffer, 1, 0, "<div", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "class=^", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
                     } 

                     //:END
                     //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
                     WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

                     //:OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
                     OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" );
                     //:// Create view for Group as a parent. This will be used in processing subcontrols to check for WebControlProperty.
                     //:CreateViewFromView( vGroupParent, vDialog )
                     CreateViewFromView( &vGroupParent, vDialog );
                     //:SetViewToSubobject( vDialog, "CtrlCtrl" )
                     SetViewToSubobject( vDialog, "CtrlCtrl" );
                     //:szIndentGroup = szIndent
                     ZeidonStringCopy( szIndentGroup, 1, 0, szIndent, 1, 0, 101 );
                     //:GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup,
                     //:                lTableRowCnt, szNoPositioning, lOffsetX, lOffsetY, szRepeatGrpKey )
                     GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup, lTableRowCnt, szNoPositioning, lOffsetX, lOffsetY, szRepeatGrpKey );
                     //:ResetViewFromSubobject( vDialog )
                     ResetViewFromSubobject( vDialog );
                     //:DropView( vGroupParent )
                     DropView( vGroupParent );
                     //:vGroupParent = 0
                     vGroupParent = 0;
                     //:szWriteBuffer = "</div>  <!-- " + szCtrlTag + " --> "
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "</div>  <!-- ", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, " --> ", 1, 0, 10001 );
                     //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
                     //:ELSE
                  } 
                  else
                  { 
                     //:// Default Group is div for now

                     //:// If we are not using absolute positioning, then make all the divs float left.
                     //:IF szNoPositioning = "Y"
                     if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
                     { 
                        //:zAppendQuotedString( szAbsoluteStyle, "float:left;", "style=", "^" )
                        zAppendQuotedString( szAbsoluteStyle, "float:left;", "style=", "^" );
                        //:ELSE
                     } 
                     else
                     { 
                        //:IF szNoPositioning = "S"
                        if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
                        { 
                           //:szAbsoluteStyle = "" // No style information.             
                           ZeidonStringCopy( szAbsoluteStyle, 1, 0, "", 1, 0, 301 );
                        } 

                        //:END
                     } 

                     //:END

                     //:szClass = vDialog.Control.CSS_Class
                     GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
                     //:IF szClass = ""
                     if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
                     { 
                        //:szWriteBuffer = "<div" + szHTMLCtrlID + szAbsoluteStyle + ">"
                        ZeidonStringCopy( szWriteBuffer, 1, 0, "<div", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
                        //:ELSE
                     } 
                     else
                     { 
                        //:szWriteBuffer = "<div" + szHTMLCtrlID + "class=^" + szClass + "^ " + szAbsoluteStyle + ">"
                        ZeidonStringCopy( szWriteBuffer, 1, 0, "<div", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szHTMLCtrlID, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "class=^", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szAbsoluteStyle, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
                     } 

                     //:END
                     //:/*IF szClass = ""
                     //:szWriteBuffer = "<fieldset " + szAbsoluteStyle + ">"
                     //:ELSE
                     //:szWriteBuffer = "<fieldset class=^" + szClass + "^ " + szAbsoluteStyle + ">"
                     //:END*/
                     //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
                     //:IF vDialog.Control.Text != ""
                     if ( CompareAttributeToString( vDialog, "Control", "Text", "" ) != 0 )
                     { 

                        //:// KJS 09/30/16 - Language Conversion.
                        //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
                        //:// So for now, I am always using, if the flag is set.
                        //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
                        if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
                        { 
                           //:// Seems like we need to check the text, and if the text has " then we shouldn't use the getLanguageText. Because one example we have
                           //:// is where the text is <img src="./images/admi..." border="0".
                           //:szText2 = vDialog.Control.Text
                           GetVariableFromAttribute( szText2, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
                           //:szTextID = vDialog.Control.LangID
                           GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
                           //:zSearchAndReplace( szText2, 1000, "\", "\\" )
                           zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
                           //:szSrch = "\" + QUOTES 
                           ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
                           ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
                           //:zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
                           zSearchAndReplace( szText2, 1000, QUOTES, szSrch );
                           //:szSrch = ""
                           ZeidonStringCopy( szSrch, 1, 0, "", 1, 0, 6 );
                           //:IF szTextID = ""
                           if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
                           { 
                              //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
                              ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
                              ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
                              ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
                              //:ELSE 
                           } 
                           else
                           { 
                              //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
                              ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
                              ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
                              ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 501 );
                              ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 501 );
                              ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
                           } 

                           //:END
                           //://szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
                           //:ELSE
                        } 
                        else
                        { 
                           //:szText = vDialog.Control.Text
                           GetVariableFromAttribute( szText, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
                        } 

                        //:END

                        //:szWriteBuffer = "<div>" + szText + "</div>"
                        ZeidonStringCopy( szWriteBuffer, 1, 0, "<div>", 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
                        ZeidonStringConcat( szWriteBuffer, 1, 0, "</div>", 1, 0, 10001 );
                        //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
                        WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
                     } 

                     //:END

                     //:OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
                     OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" );

                     //:// Create view for Group as a parent. This will be used in processing subcontrols to check for WebControlProperty.
                     //:CreateViewFromView( vGroupParent, vDialog )
                     CreateViewFromView( &vGroupParent, vDialog );
                     //:SetViewToSubobject( vDialog, "CtrlCtrl" )
                     SetViewToSubobject( vDialog, "CtrlCtrl" );
                     //:szIndentGroup = szIndent + "   "
                     ZeidonStringCopy( szIndentGroup, 1, 0, szIndent, 1, 0, 101 );
                     ZeidonStringConcat( szIndentGroup, 1, 0, "   ", 1, 0, 101 );
                     //:GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup,
                     //:                lTableRowCnt, szNoPositioning, lOffsetX, lOffsetY, szRepeatGrpKey )
                     GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup, lTableRowCnt, szNoPositioning, lOffsetX, lOffsetY, szRepeatGrpKey );
                     //:ResetViewFromSubobject( vDialog )
                     ResetViewFromSubobject( vDialog );
                     //:DropView( vGroupParent )
                     DropView( vGroupParent );
                     //:vGroupParent = 0
                     vGroupParent = 0;

                     //:szWriteBuffer = "</div>  <!-- " + szCtrlTag + " --> "
                     ZeidonStringCopy( szWriteBuffer, 1, 0, "</div>  <!-- ", 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
                     ZeidonStringConcat( szWriteBuffer, 1, 0, " --> ", 1, 0, 10001 );
                     //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
                     WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 

         //:END
      } 


      //:END  // Control is groupbox

      //:IF szControlType = "Text"  // TEXT
      if ( ZeidonStringCompare( szControlType, 1, 0, "Text", 1, 0, 51 ) == 0 )
      { 
         //:GenJSPJ_CrteText( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndent, szCtrlTag,
         //:                  "", szNoPositioning, 0, 0, szRepeatGrpKey )
         GenJSPJ_CrteText( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndent, szCtrlTag, "", szNoPositioning, 0, 0, szRepeatGrpKey );
      } 

      //:END

      //:IF szControlType = "EditBox"  // EDITBOX
      if ( ZeidonStringCompare( szControlType, 1, 0, "EditBox", 1, 0, 51 ) == 0 )
      { 
         //:GenJSPJ_CrteEditBox( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag,
         //:                     "", "", szNoPositioning, 0, 0, szRepeatGrpKey )
         GenJSPJ_CrteEditBox( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag, "", "", szNoPositioning, 0, 0, szRepeatGrpKey );
      } 

      //:END

      //:IF szControlType = "Calendar"  // CALENDAR
      if ( ZeidonStringCompare( szControlType, 1, 0, "Calendar", 1, 0, 51 ) == 0 )
      { 
         //:GenJSPJ_CrteCalendar( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag,
         //:                      "", "", szNoPositioning, 0, 0, szRepeatGrpKey )
         GenJSPJ_CrteCalendar( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag, "", "", szNoPositioning, 0, 0, szRepeatGrpKey );
      } 

      //:END

      //:IF szControlType = "RadioGrp" // RADIO BUTTON GROUP
      if ( ZeidonStringCompare( szControlType, 1, 0, "RadioGrp", 1, 0, 51 ) == 0 )
      { 
         //:/*
         //:szWriteBuffer = "<% strErrorMapValue = ^^;"
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:IF vDialog.CtrlMapView EXISTS
         //:   szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
         //:                   "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "      task.log( ).debug( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "   else"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "   {"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
         //:                   vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "      if ( nRC >= 0 )"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "         strRadioGroupValue = " +
         //:                   vDialog.CtrlMapView.Name + ".cursor( ^" +
         //:                   vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
         //:                   vDialog.CtrlMapER_Attribute.Name + "^ ).getString( );"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "   }"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:END
         //:szWriteBuffer = "%>"
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         //://KJS 11/14/07 - I don't think there should always be a box around radio buttons.
         //://I'm going to do this like the way group box is (or was).  If there is text
         //://to go on the box then we will create <fieldset.  If there is no text then
         //://we will create a div.
         //://KJS 10/07/09 - I added the id to this <div but then realized that the radio buttons also have this
         //://id, so I am adding "gb" in front of the control tag.  Not sure how much we care that this div has
         //://an id and name but think it should.
         //:IF vDialog.Control.Text = ""
         //:   //szWriteBuffer = "<div " + szStyle + ">"
         //:   szWriteBuffer = "<div id=^gb" + szCtrlTag + "^ name=^gb" + szCtrlTag + "^ " + szStyle + ">"
         //:ELSE
         //:   szWriteBuffer = "<fieldset " + szStyle + ">"
         //:END
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:IF vDialog.Control.Text != ""
         //:   // KJS 09/30/16 - Language Conversion.
         //:   // I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
         //:   // So for now, I am always using, if the flag is set.
         //:   IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
         //:      // Seems like we need to check the text, and if the text has " then we shouldn't use the getLanguageText. Because one example we have
         //:      // is where the text is <img src="./images/admi..." border="0".
         //:      szText2 = vDialog.Control.Text
         //:      szTextID = vDialog.Control.LangID
         //:      zSearchAndReplace( szText2, 1000, "\", "\\" )
         //:      szSrch = "\" + QUOTES 
         //:      zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
         //:      szSrch = ""
         //:      IF szTextID = ""
         //:         szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
         //:      ELSE 
         //:         szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
         //:      END
         //:      //szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
         //:   ELSE
         //:      szText = vDialog.Control.Text
         //:   END
         //:   szWriteBuffer = "<div>" + szText + "</div>"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:END
         //:OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
         //:SetViewToSubobject( vDialog, "CtrlCtrl" )
         //:szIndent = szIndent + "   "
         //:*/
         //:GenJSPJ_CrteCtrlsRadio( vDialog, lFile, szWriteBuffer, szIndent,
         //:                        szCtrlTag, szNoPositioning, 0, 0, szRepeatGrpKey )
         GenJSPJ_CrteCtrlsRadio( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag, szNoPositioning, 0, 0, szRepeatGrpKey );
      } 

      //:   /*
      //:   ResetViewFromSubobject( vDialog )
      //:   IF vDialog.Control.Text = ""
      //:      szWriteBuffer = "</div>"
      //:   ELSE
      //:      szWriteBuffer = "</fieldset>"
      //:   END
      //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      //:   */

      //:END

      //:IF szControlType = "CheckBox"  // CHECKBOX
      if ( ZeidonStringCompare( szControlType, 1, 0, "CheckBox", 1, 0, 51 ) == 0 )
      { 
         //:GenJSPJ_CrteCheckBox( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag,
         //:                      "", "", szNoPositioning, 0, 0, szRepeatGrpKey )
         GenJSPJ_CrteCheckBox( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag, "", "", szNoPositioning, 0, 0, szRepeatGrpKey );
      } 

      //:END

      //:IF szControlType = "PushBtn"  // PUSH BUTTON
      if ( ZeidonStringCompare( szControlType, 1, 0, "PushBtn", 1, 0, 51 ) == 0 )
      { 

         //:// KJS 05/26/17 - I'm not sure why we wouldn't have been calling GenJSPJ_CrtePushButton, 
         //:// going to use this instead and comment out other code.
         //:GenJSPJ_CrtePushButton( vDialog, vGroupParent, lFile, szWriteBuffer, szIndent, szCtrlTag,
         //:                        "", "", szNoPositioning, 0, 0, szRepeatGrpKey )
         GenJSPJ_CrtePushButton( vDialog, vGroupParent, lFile, szWriteBuffer, szIndent, szCtrlTag, "", "", szNoPositioning, 0, 0, szRepeatGrpKey );
      } 

      //:END  //End of PushBtn

      //:IF szControlType = "ComboBox"  // COMBOBOX
      if ( ZeidonStringCompare( szControlType, 1, 0, "ComboBox", 1, 0, 51 ) == 0 )
      { 
         //:GenJSPJ_CrteComboBox( vDialog, lFile, szWriteBuffer, szIndent,
         //:                      szCtrlTag, "", szNoPositioning, 0, 0, szRepeatGrpKey )
         GenJSPJ_CrteComboBox( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag, "", szNoPositioning, 0, 0, szRepeatGrpKey );
      } 

      //:END

      //:IF szControlType = "Bitmap" OR szControlType = "BitmapBtn"  // BITMAP / JPG
      if ( ZeidonStringCompare( szControlType, 1, 0, "Bitmap", 1, 0, 51 ) == 0 || ZeidonStringCompare( szControlType, 1, 0, "BitmapBtn", 1, 0, 51 ) == 0 )
      { 
         //:GenJSPJ_CrteJPG( vDialog, lFile, szWriteBuffer, szIndent,
         //:                 szCtrlTag, szNoPositioning, 0, 0 )
         GenJSPJ_CrteJPG( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag, szNoPositioning, 0, 0 );
      } 

      //:END

      //:IF szControlType = "ListBox"  // LISTBOX
      if ( ZeidonStringCompare( szControlType, 1, 0, "ListBox", 1, 0, 51 ) == 0 )
      { 
         //://IF jquery
         //://ELSE
         //:   GenJSPJ_CrteListBox( vDialog, lFile, szWriteBuffer, szIndent,
         //:                        szCtrlTag, szNoPositioning, 0, 0 )
         GenJSPJ_CrteListBox( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag, szNoPositioning, 0, 0 );
      } 

      //:   //END                              
      //:END  // IF szControlType = "ListBox"

      //:IF szControlType = "MLEdit"  // MLEDIT
      if ( ZeidonStringCompare( szControlType, 1, 0, "MLEdit", 1, 0, 51 ) == 0 )
      { 

         //:GenJSPJ_CrteMLEdit( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag,
         //:                    "", "", szNoPositioning, 0, 0, szRepeatGrpKey )
         GenJSPJ_CrteMLEdit( vDialog, lFile, szWriteBuffer, szIndent, szCtrlTag, "", "", szNoPositioning, 0, 0, szRepeatGrpKey );
      } 

      //:END   //IF szControlType = "MLEdit"

      //:// KJS 07/13/18 - Right now, if I put a "Grid" inside a groupbox marked as "Table", the grid doesn't
      //:// get created (if I put the grid inside a groupbox, then it works). I need to perform something like the following
      //:// but I would need to put all of the grid code from within CrteCtrlsRecurs into it's own function and call that
      //:// function from here. I should do that but not right now...
      //:/*
      //:IF szControlType = "Grid"  // Grid

      //:   GenJSPJ_CrteCtrlsRecurs( vDialog, vGroupParent, vDialogRoot, lFile, szWriteBuffer, szIndentGroup,
      //:                            lTableRowCnt, szNoPositioning, X_Size, Y_Size, szRepeatGrpKey )
      //:END
      //:*/

      //://****************************************************************************


      //:szWriteBuffer = "</td>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</td>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:iColCnt = iColCnt + 1
      iColCnt = iColCnt + 1;
      RESULT = SetCursorNextEntity( vDialog, "Control", "" );
   } 

   //:END

   //:// If the previous row didn't have as many controls as there are
   //:// table columns, then we need to create a blank column
   //:LOOP  WHILE iColCnt < iHighestColCnt
   while ( iColCnt < iHighestColCnt )
   { 
      //:szWriteBuffer = "<td>&nbsp</td>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<td>&nbsp</td>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:iColCnt = iColCnt + 1
      iColCnt = iColCnt + 1;
   } 

   //:END

   //:szWriteBuffer = "</tr>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</tr>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:szWriteBuffer = "</table>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</table>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   //:szWriteBuffer = "</div>  <!-- " + szParentCtrlTag + " --> "
   ZeidonStringCopy( szWriteBuffer, 1, 0, "</div>  <!-- ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szParentCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " --> ", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteCtrlsRadio( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                       INTEGER          lFile,
//:                       STRING ( 10000 ) szWriteBuffer,
//:                       STRING ( 50 )    szIndent,
//:                       STRING ( 32 )    szRadioGroupName,
//:                       STRING ( 1 )     szNoPositioning,
//:                       INTEGER          lOffsetX,
//:                       INTEGER          lOffsetY,
//:                       STRING ( 100 )   szRepeatGrpKey )

//:   VIEW vDialogRoot BASED ON LOD TZWDLGSO
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteCtrlsRadio( zVIEW     vDialog,
                        zLONG     lFile,
                        zPCHAR    szWriteBuffer,
                        zPCHAR    szIndent,
                        zPCHAR    szRadioGroupName,
                        zPCHAR    szNoPositioning,
                        zLONG     lOffsetX,
                        zLONG     lOffsetY,
                        zPCHAR    szRepeatGrpKey )
{
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szCtrlTag
   zCHAR     szCtrlTag[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 500 ) szText
   zCHAR     szText[ 501 ] = { 0 }; 
   //:STRING ( 500 ) szText2
   zCHAR     szText2[ 501 ] = { 0 }; 
   //:STRING ( 256 ) szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 300 ) szAbsoluteStyle
   zCHAR     szAbsoluteStyle[ 301 ] = { 0 }; 
   //:STRING ( 5 )   szSrch
   zCHAR     szSrch[ 6 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szPosition
   zCHAR     szPosition[ 101 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 32 )  szActionName
   zCHAR     szActionName[ 33 ] = { 0 }; 
   //:STRING ( 1 )   szFirstTimeThru
   zCHAR     szFirstTimeThru[ 2 ] = { 0 }; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 
   //:INTEGER        LastY_Pos
   zLONG     LastY_Pos = 0; 
   zCHAR     szTempString_0[ 255 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_10[ 51 ]; 
   zCHAR     szTempString_11[ 51 ]; 
   zCHAR     szTempString_12[ 51 ]; 


   //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
   GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );

   //:   IF vDialog.Control.CSS_Class != "" 
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) != 0 )
   { 
      //:   szClass = " class=^" + vDialog.Control.CSS_Class + "^ "
      GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, " class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^ ", 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:   szClass = ""
      ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
   } 

   //:   END

   //:      // Initialize the szAbsoluteStyle variable with X and Y values.
   //:      //CreateAbsolStyleString( vDialog, szAbsoluteStyle, lOffsetX, lOffsetY, "" )

   //:   IF szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:   CreateNoPosStyleString( vDialog, szStyle, "")
      CreateNoPosStyleString( vDialog, szStyle, "" );
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" // No style information.
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
         //:ELSE
      } 
      else
      { 
         //:CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
         CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" );
      } 

      //:END
   } 

   //:   END

   //:         //szRadioGroupName = szCtrlTag
   //:         szWriteBuffer = "<% strErrorMapValue = ^^;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strErrorMapValue = ^^;", 1, 0, 10001 );
   //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:         IF vDialog.CtrlMapView EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDialog, "CtrlMapView" );
   if ( lTempInteger_0 == 0 )
   { 
      //:         szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
      //:                         "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:         szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:         szWriteBuffer = "      task.log( ).info( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.log( ).info( ^Invalid View: ^ + ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:         szWriteBuffer = "   else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:         szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:                      
      //:         // KJS 03/13/18 - If the view is read only, then we want to set the combo box to disabled.
      //:         // We only do this if the zeidon.ini has UseZeidonControlActionsView
      //:         IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y" 
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesControlActionsView", "Y" ) == 0 )
      { 
         //:         szWriteBuffer = "      strDisabled = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strDisabled = ^^;", 1, 0, 10001 );
         //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:         szWriteBuffer = "      if ( " + vDialog.CtrlMapView.Name + ".isReadOnly( ) )"
         GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".isReadOnly( ) )", 1, 0, 10001 );
         //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:         szWriteBuffer = "         strDisabled = ^disabled^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strDisabled = ^disabled^;", 1, 0, 10001 );
         //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:         END
      //:         
      //:         szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                         vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
      GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:         szWriteBuffer = "      if ( nRC >= 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:         szWriteBuffer = "         strRadioGroupValue = " +
      //:                         vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                         vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
      //:                         vDialog.CtrlMapER_Attribute.Name + "^ ).getString( );"
      GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strRadioGroupValue = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( );", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:         szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   } 

   //:         END

   //:         szWriteBuffer = "%>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
   //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:         //KJS 11/14/07 - I don't think there should always be a box around radio buttons.
   //:         //I'm going to do this like the way group box is (or was).  If there is text
   //:         //to go on the box then we will create <fieldset.  If there is no text then
   //:         //we will create a div.
   //:         //KJS 10/07/09 - I added the id to this <div but then realized that the radio buttons also have this
   //:         //id, so I am adding "gb" in front of the control tag.  Not sure how much we care that this div has
   //:         //an id and name but think it should.
   //:         IF vDialog.Control.Text = ""
   if ( CompareAttributeToString( vDialog, "Control", "Text", "" ) == 0 )
   { 
      //:         //szWriteBuffer = "<div id=^gb" + szRadioGroupName + szRepeatGrpKey + "^ name=^gb" + szRadioGroupName + szRepeatGrpKey + "^ " + szClass + szStyle + ">"
      //:         // Going to set the "id" of this outer div to the radiogrp tag, since each individual button has an id of the button tag and the NAME of the radio group tag.
      //:         // This makes hiding/disabling easier.
      //:         szWriteBuffer = "<div id=^" + szRadioGroupName + szRepeatGrpKey + "^ name=^gb" + szRadioGroupName + szRepeatGrpKey + "^ " + szClass + szStyle + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^gb", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:      ELSE
   } 
   else
   { 
      //:         //szWriteBuffer = "<fieldset id=^gb" + szRadioGroupName + szRepeatGrpKey + "^ name=^gb" + szRadioGroupName + szRepeatGrpKey + "^ " + szClass + szStyle + ">"
      //:         // Going to set the "id" of this outer div to the radiogrp tag, since each individual button has an id of the button tag and the NAME of the radio group tag.
      //:         // This makes hiding/disabling easier.
      //:         szWriteBuffer = "<fieldset id=^" + szRadioGroupName + szRepeatGrpKey + "^ name=^gb" + szRadioGroupName + szRepeatGrpKey + "^ " + szClass + szStyle + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<fieldset id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^gb", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
   } 

   //:         END
   //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:         
   //:      IF vDialog.Control.Text != ""
   if ( CompareAttributeToString( vDialog, "Control", "Text", "" ) != 0 )
   { 
      //:      // KJS 09/30/16 - Language Conversion.
      //:      // I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
      //:      // So for now, I am always using, if the flag is set.
      //:      IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:      // Seems like we need to check the text, and if the text has " then we shouldn't use the getLanguageText. Because one example we have
         //:      // is where the text is <img src="./images/admi..." border="0".
         //:      szText2 = vDialog.Control.Text
         GetVariableFromAttribute( szText2, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
         //:      szTextID = vDialog.Control.LangID
         GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
         //:      zSearchAndReplace( szText2, 1000, "\", "\\" )
         zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
         //:      szSrch = "\" + QUOTES 
         ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
         ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
         //:      zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
         zSearchAndReplace( szText2, 1000, QUOTES, szSrch );
         //:      szSrch = ""
         ZeidonStringCopy( szSrch, 1, 0, "", 1, 0, 6 );
         //:      IF szTextID = ""
         if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:      szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
            //:   ELSE 
         } 
         else
         { 
            //:      szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
         } 

         //:      END
         //:      //szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
         //:   ELSE
      } 
      else
      { 
         //:      szText = vDialog.Control.Text
         GetVariableFromAttribute( szText, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
      } 

      //:      END
      //:      szWriteBuffer = "<div>" + szText + "</div>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<div>", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "</div>", 1, 0, 10001 );
      //:      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   } 

   //:      END

   //:         OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" )
   OrderEntityForView( vDialog, "CtrlCtrl", "PSDLG_Y A PSDLG_X A" );
   //:         SetViewToSubobject( vDialog, "CtrlCtrl" )
   SetViewToSubobject( vDialog, "CtrlCtrl" );
   //:         szIndent = szIndent + "   "
   ZeidonStringConcat( szIndent, 1, 0, "   ", 1, 0, 51 );




   //:// KJS 09/02/13 - Adding table around radio buttons. This was already done for C jsps...
   //:LastY_Pos = 0
   LastY_Pos = 0;
   //:szFirstTimeThru = "Y"
   ZeidonStringCopy( szFirstTimeThru, 1, 0, "Y", 1, 0, 2 );
   //:IF vDialog.Control EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( vDialog, "Control" );
   if ( lTempInteger_1 == 0 )
   { 
      //:LastY_Pos = vDialog.Control.PSDLG_Y
      GetIntegerFromAttribute( &LastY_Pos, vDialog, "Control", "PSDLG_Y" );
   } 

   //:END
   //:szWriteBuffer = "   <table style=^margin:0px;padding:0px;^  name=^tbl" + szRadioGroupName + szRepeatGrpKey + "^ id=^tbl" + szRadioGroupName + szRepeatGrpKey + "^>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   <table style=^margin:0px;padding:0px;^  name=^tbl", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^tbl", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   <tr style=^border:0px^>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   <tr style=^border:0px^>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:// Process the individual Radio Buttons for a Radio Button Group.
   //:FOR EACH vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:szActionName = ""
      ZeidonStringCopy( szActionName, 1, 0, "", 1, 0, 33 );
      //:IF vDialog.EventAct EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( vDialog, "EventAct" );
      if ( lTempInteger_2 == 0 )
      { 
         //:szActionName = vDialog.EventAct.Tag
         GetVariableFromAttribute( szActionName, 0, 'S', 33, vDialog, "EventAct", "Tag", "", 0 );
         //:nRC = zstrcmpi( szActionName, "alt-f4" )
         nRC = zstrcmpi( szActionName, "alt-f4" );
         //:IF nRC = 0
         if ( nRC == 0 )
         { 
            //:szActionName = "AltF4"
            ZeidonStringCopy( szActionName, 1, 0, "AltF4", 1, 0, 33 );
         } 

         //:END
      } 

      //:END

      //:szCtrlTag = vDialog.Control.Tag
      GetVariableFromAttribute( szCtrlTag, 0, 'S', 257, vDialog, "Control", "Tag", "", 0 );

      //:szWriteBuffer = "<% strErrorMapValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strErrorMapValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( StringUtils.equals( strRadioGroupValue, ^" +
      //:                         vDialog.Control.RadioOrCheckboxValue + "^ ) )"
      GetVariableFromAttribute( szTempString_10, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( StringUtils.equals( strRadioGroupValue, ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ) )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      strErrorMapValue = ^checked=\^checked\^^;  %>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      strErrorMapValue = ^checked=\\^checked\\^^;  %>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:CreateTabIndexString( vDialog, szTabIndex )
      CreateTabIndexString( vDialog, szTabIndex );

      //:// If this window is being generated with no positioning then
      //:// do not create the position in the style.
      //:IF szNoPositioning = ""
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
      { 
         //:CreateAbsolPosString( vDialog, szPosition, lOffsetX, lOffsetY )
         CreateAbsolPosString( vDialog, szPosition, lOffsetX, lOffsetY );
      } 

      //:END
      //://CreateAbsolPosString( vDialog, szPosition, 0, 0 )
      //:CreateDisabledString( vDialog, szDisabled )
      CreateDisabledString( vDialog, szDisabled );
      //:FixStyleString( szStyle, szPosition, "", "" )
      FixStyleString( szStyle, szPosition, "", "" );

      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" // No style information
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
      } 

      //:END

      //:// If LastY_Pos = -100 then this is the first time thru...
      //:IF ( ( LastY_Pos < vDialog.Control.PSDLG_Y ) )
      if ( ( CompareAttributeToInteger( vDialog, "Control", "PSDLG_Y", LastY_Pos ) > 0 ) )
      { 
         //:szWriteBuffer = "   </tr>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   </tr>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   <tr style=^border:0px^>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <tr style=^border:0px^>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:ELSE
      } 
      else
      { 
         //:// KJS 07/22/10 - Currently for relative positioning, the radio buttons are being created one right after the other
         //:// (on the same line), no matter how I paint them on the screen).  Really to look at that but since we hardly ever
         //:// use them, I am for a fast fix going to append "&nbsp&nbsp&nbsp" after each button, to give some spacing.
         //:IF szNoPositioning = "Y" AND szFirstTimeThru = "N"
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 && ZeidonStringCompare( szFirstTimeThru, 1, 0, "N", 1, 0, 2 ) == 0 )
         { 
            //:szWriteBuffer = "      <td style=^border:0px;white-space:nowrap^>&nbsp&nbsp&nbsp</td>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^border:0px;white-space:nowrap^>&nbsp&nbsp&nbsp</td>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         } 

         //:END
      } 

      //:END

      //:szStyle = szStyle + szTabIndex
      ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );
      //:// szStyle = " style=^position:absolute;LEFT:" + szX + ";TOP:" + szY + ";^ "
      //:IF  szActionName != ""
      if ( ZeidonStringCompare( szActionName, 1, 0, "", 1, 0, 33 ) != 0 )
      { 
         //:szWriteBuffer = "      <td style=^width:15px;border:0px;^><input type=^radio^ name=^" + szRadioGroupName + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey +
         //:                "^ <%=strDisabled%> value=^" + vDialog.Control.RadioOrCheckboxValue +
         //:                "^ <%=strErrorMapValue%> " + szStyle + szDisabled + " onclick=^" + szActionName + "( )^  ></td>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^width:15px;border:0px;^><input type=^radio^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> value=^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_11, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strErrorMapValue%> ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "( )^  ></td>", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "      <td style=^width:15px;border:0px;^><input type=^radio^ name=^" + szRadioGroupName + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey +
         //:                "^ <%=strDisabled%> value=^" + vDialog.Control.RadioOrCheckboxValue +
         //:                "^ <%=strErrorMapValue%> " + szStyle + szDisabled + " ></td>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^width:15px;border:0px;^><input type=^radio^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRadioGroupName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> value=^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_12, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strErrorMapValue%> ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " ></td>", 1, 0, 10001 );
      } 

      //:END

      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:IF szNoPositioning = "Y"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
      { 
         //:CreateNoPosStyleString( vDialog, szStyle, "")
         CreateNoPosStyleString( vDialog, szStyle, "" );
         //:ELSE
      } 
      else
      { 
         //:IF szNoPositioning = "S"
         if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
         { 
            //:szStyle = "" // No style information.
            ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
            //:ELSE
         } 
         else
         { 
            //:CreateAbsolStyleString( vDialog, szStyle, 30 + lOffsetX, lOffsetY, "" )
            CreateAbsolStyleString( vDialog, szStyle, 30 + lOffsetX, lOffsetY, "" );
         } 

         //:END
      } 

      //:END

      //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
      //:szTitleHTML = ""
      ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
      //:szTitle = vDialog.Control.DIL_Text
      GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
      //:IF szTitle != ""
      if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
      { 
         //:szTitleHTML = " title=^" + szTitle + "^ "
         ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
         ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
         ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
      } 

      //:END

      //:// KJS 09/30/16 - Language Conversion.
      //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
      //:// So for now, I am always using, if the flag is set.
      //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
      { 
         //:szText2 = vDialog.Control.Text
         GetVariableFromAttribute( szText2, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
         //:szTextID = vDialog.Control.LangID
         GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
         //:zSearchAndReplace( szText2, 1000, "\", "\\" )
         zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
         //:szSrch = "\" + QUOTES 
         ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
         ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
         //:zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
         zSearchAndReplace( szText2, 1000, QUOTES, szSrch );
         //:szSrch = ""
         ZeidonStringCopy( szSrch, 1, 0, "", 1, 0, 6 );
         //:IF szTextID = ""
         if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
         { 
            //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
            //:ELSE 
         } 
         else
         { 
            //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
            ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 501 );
            ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
         } 

         //:END
         //://szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
         //:ELSE
      } 
      else
      { 
         //:szText = vDialog.Control.Text
         GetVariableFromAttribute( szText, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
      } 

      //:END

      //:szClass = vDialog.Control.CSS_Class
      GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );
      //:IF szClass = ""
      if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
      { 
         //:szWriteBuffer = "      <td style=^border:0px;white-space:nowrap^><span " + szTitleHTML + szStyle + ">" + szText + "</span></td>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^border:0px;white-space:nowrap^><span ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</span></td>", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "      <td style=^border:0px;white-space:nowrap^><span class=^" + szClass + "^ " + szTitleHTML + szStyle + ">" + szText + "</span></td>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      <td style=^border:0px;white-space:nowrap^><span class=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "</span></td>", 1, 0, 10001 );
      } 

      //:END

      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:LastY_Pos = vDialog.Control.PSDLG_Y
      GetIntegerFromAttribute( &LastY_Pos, vDialog, "Control", "PSDLG_Y" );
      //:szFirstTimeThru = "N"
      ZeidonStringCopy( szFirstTimeThru, 1, 0, "N", 1, 0, 2 );
      RESULT = SetCursorNextEntity( vDialog, "Control", "" );
   } 

   //:END
   //:szWriteBuffer = "   </tr>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   </tr>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   </table>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   </table>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );



   //:         ResetViewFromSubobject( vDialog )
   ResetViewFromSubobject( vDialog );
   //:         IF vDialog.Control.Text = ""
   if ( CompareAttributeToString( vDialog, "Control", "Text", "" ) == 0 )
   { 
      //:         szWriteBuffer = "</div>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</div>", 1, 0, 10001 );
      //:      ELSE
   } 
   else
   { 
      //:         szWriteBuffer = "</fieldset>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</fieldset>", 1, 0, 10001 );
   } 

   //:         END
   //:         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = ""
   ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteComboBox( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                     INTEGER          lFile,
//:                     STRING ( 10000 ) szWriteBuffer,
//:                     STRING ( 50 )    szIndent,
//:                     STRING ( 32 )    szCtrlTag,
//:                     STRING ( 1 )     szTableRowFlag,
//:                     STRING ( 1 )     szNoPositioning,
//:                     INTEGER          lOffsetX,
//:                     INTEGER          lOffsetY,
//:                     STRING ( 100 )   szRepeatGrpKey )

//:   // Combo boxes on OUTPUT

//:   // There are two kinds of combo boxes on output.
//:   // 1. Domain combo boxes, which need to get the individual table values
//:   //    from core calls.
//:   // 2. Select combo boxes (e.g.  Automatic Include, Set Foreign Key, etc.)
//:   //    which read the data values from an object, the same as a list box.
//:   // These two conditions are handled quite separately below, even though
//:   // the resulting HTML is basically the same.

//:   VIEW vList   BASED ON LOD TZWDLGSO  // identifies attribute mapping for list entries
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteComboBox( zVIEW     vDialog,
                      zLONG     lFile,
                      zPCHAR    szWriteBuffer,
                      zPCHAR    szIndent,
                      zPCHAR    szCtrlTag,
                      zPCHAR    szTableRowFlag,
                      zPCHAR    szNoPositioning,
                      zLONG     lOffsetX,
                      zLONG     lOffsetY,
                      zPCHAR    szRepeatGrpKey )
{
   zVIEW     vList = 0; 
   //:VIEW vEntity BASED ON LOD TZWDLGSO  // identifies entity mapping for list entries
   zVIEW     vEntity = 0; 
   //:VIEW vScope  BASED ON LOD TZWDLGSO  // identifies scoping entity for list entries
   zVIEW     vScope = 0; 
   //:VIEW vDialogRoot BASED ON LOD TZWDLGSO
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 32 )  szContextName
   zCHAR     szContextName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szScopingEntityName
   zCHAR     szScopingEntityName[ 33 ] = { 0 }; 
   //:STRING ( 32 )  szListEntityName
   zCHAR     szListEntityName[ 33 ] = { 0 }; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 100 ) szCtrlTagRG
   zCHAR     szCtrlTagRG[ 101 ] = { 0 }; 
   //:STRING ( 100 ) szInListCtrlTag
   zCHAR     szInListCtrlTag[ 101 ] = { 0 }; 
   //:STRING ( 50 )  szKeyPressCode
   zCHAR     szKeyPressCode[ 51 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 10 )  szWidth
   zCHAR     szWidth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szSelectAction
   zCHAR     szSelectAction[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szSelectFunction
   zCHAR     szSelectFunction[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szHeight
   zCHAR     szHeight[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szX_Pos
   zCHAR     szX_Pos[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szY_Pos
   zCHAR     szY_Pos[ 11 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:INTEGER        lSubtype
   zLONG     lSubtype = 0; 
   //:SHORT          nRC
   zSHORT    nRC = 0; 
   zCHAR     szTempString_0[ 255 ]; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zSHORT    lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zSHORT    lTempInteger_6; 
   zSHORT    lTempInteger_7; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 33 ]; 
   zCHAR     szTempString_13[ 33 ]; 
   zCHAR     szTempString_14[ 33 ]; 
   zCHAR     szTempString_15[ 33 ]; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_8; 
   zSHORT    lTempInteger_9; 
   zCHAR     szTempString_16[ 33 ]; 
   zCHAR     szTempString_17[ 33 ]; 
   zCHAR     szTempString_18[ 33 ]; 
   zSHORT    lTempInteger_10; 
   zCHAR     szTempString_19[ 33 ]; 
   zCHAR     szTempString_20[ 33 ]; 
   zCHAR     szTempString_21[ 33 ]; 
   zCHAR     szTempString_22[ 33 ]; 
   zCHAR     szTempString_23[ 33 ]; 
   zSHORT    lTempInteger_11; 
   zCHAR     szTempString_24[ 33 ]; 
   zCHAR     szTempString_25[ 33 ]; 
   zCHAR     szTempString_26[ 33 ]; 
   zSHORT    lTempInteger_12; 
   zSHORT    lTempInteger_13; 
   zCHAR     szTempString_27[ 33 ]; 
   zCHAR     szTempString_28[ 33 ]; 
   zSHORT    lTempInteger_14; 
   zSHORT    lTempInteger_15; 
   zCHAR     szTempString_29[ 33 ]; 
   zCHAR     szTempString_30[ 33 ]; 


   //:szStyle = "width:" + szWidth + "px; height:" + szHeight + "px;"
   ZeidonStringCopy( szStyle, 1, 0, "width:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px; height:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szHeight, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );
   //:szClass = ""
   ZeidonStringCopy( szClass, 1, 0, "", 1, 0, 257 );
   //:IF vDialog.Control.CSS_Class != ""
   if ( CompareAttributeToString( vDialog, "Control", "CSS_Class", "" ) != 0 )
   { 
      //:szClass = "class=^" + vDialog.Control.CSS_Class + "^"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 255, vDialog, "Control", "CSS_Class", "", 0 );
      ZeidonStringCopy( szClass, 1, 0, "class=^", 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, szTempString_0, 1, 0, 257 );
      ZeidonStringConcat( szClass, 1, 0, "^", 1, 0, 257 );
   } 

   //:END

   //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle != ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) != 0 )
   { 
      //:szTitleHTML = " title=^" + szTitle + "^ "
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:CreateTabIndexString( vDialog, szTabIndex )
   CreateTabIndexString( vDialog, szTabIndex );
   //:// We will set the height to the constant 20 px.
   //:PIX_PER_DU( vDialog, dDLUnits )
   PIX_PER_DU( vDialog, &dDLUnits );
   //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
   GetIntegerFromAttribute( &lTempInteger_0, vDialog, "Control", "SZDLG_X" );
   zIntegerToString( szWidth, 10, lTempInteger_0 * dDLUnits );
   //:szHeight = "20"
   ZeidonStringCopy( szHeight, 1, 0, "20", 1, 0, 11 );
   //:// The height used to be hard coded but we took that out!!!
   //://szStyle = "style=^width:" + szWidth + "px;height:" + szHeight + "px;"
   //:szStyle = "style=^width:" + szWidth + "px;"
   ZeidonStringCopy( szStyle, 1, 0, "style=^width:", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );

   //:// If szNoPositioning is null then we are using absolute positioning.
   //:IF szNoPositioning = ""
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
   { 

      //:zIntegerToString( szX_Pos, 10, vDialog.Control.PSDLG_X * dDLUnits )
      GetIntegerFromAttribute( &lTempInteger_1, vDialog, "Control", "PSDLG_X" );
      zIntegerToString( szX_Pos, 10, lTempInteger_1 * dDLUnits );
      //:zIntegerToString( szY_Pos, 10, vDialog.Control.PSDLG_Y * dDLUnits )
      GetIntegerFromAttribute( &lTempInteger_2, vDialog, "Control", "PSDLG_Y" );
      zIntegerToString( szY_Pos, 10, lTempInteger_2 * dDLUnits );

      //:szStyle = szStyle + "position:absolute;" + "left:" + szX_Pos + "px;" + "top:" + szY_Pos + "px;^ "
      ZeidonStringConcat( szStyle, 1, 0, "position:absolute;", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "left:", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, szX_Pos, 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "top:", 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, szY_Pos, 1, 0, 301 );
      ZeidonStringConcat( szStyle, 1, 0, "px;^ ", 1, 0, 301 );
      //:// CreateAbsolStyleString( vDialog, szStyle, lOffsetX, lOffsetY, "" )
      //:ELSE
   } 
   else
   { 
      //:szStyle = szStyle + "^ "
      ZeidonStringConcat( szStyle, 1, 0, "^ ", 1, 0, 301 );
   } 

   //:END

   //:IF szNoPositioning = "S"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
   { 
      //:szStyle = "" // No style information
      ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
   } 

   //:END
   //:szStyle = szStyle + szTabIndex
   ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );

   //:szWriteBuffer = "<% strErrorMapValue = ^^; "
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<% strErrorMapValue = ^^; ", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0)
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
   GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );
   //:   
   //:// KJS 03/13/18 - If the view is read only, then we want to set the combo box to disabled.
   //:// We only do this if the zeidon.ini has UseZeidonControlActionsView
   //:IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y" AND vDialog.CtrlMapView EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( vDialog, "CtrlMapView" );
   if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesControlActionsView", "Y" ) == 0 && lTempInteger_3 == 0 )
   { 

      //:szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
      //:             "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) )"
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "       strDisabled = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "       strDisabled = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "       if ( " + vDialog.CtrlMapView.Name + ".isReadOnly( ) )"
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "       if ( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".isReadOnly( ) )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //://szWriteBuffer = "                  strCtrlActionsView = strCtrlActionsView + ^" + szCtrlTag + ",Disable,^;"
      //://szWriteBuffer = "                  strCtrlActions = strCtrlActions + ^" + szCtrlTag + ",Disable,^;"
      //:szWriteBuffer = "          strDisabled = ^disabled^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "          strDisabled = ^disabled^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   } 


   //:END
   //:szWriteBuffer = "%>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );


   //:lSubtype = vDialog.Control.Subtype
   GetIntegerFromAttribute( &lSubtype, vDialog, "Control", "Subtype" );
   //:nRC = AreBitsSetInFlag( lSubtype, 61440 ) // 0x0000F000
   nRC = AreBitsSetInFlag( lSubtype, 61440 );
   //:IF nRC = 0   // IF vDialog.Control.Subtype < 12000  ==> Domain Combobox
   if ( nRC == 0 )
   { 

      //:// Domain type combo box

      //://szWriteBuffer = "<input name=^h" + szCtrlTag + szRepeatGrpKey + "^ id=^h" + szCtrlTag + szRepeatGrpKey + "^ type=^hidden^ value=^0^ >"
      //://WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )

      //:// The Select Action is either onfocusout or onchange depending on the following.
      //:// 1. If no Action, use onchange because this allows using keystrokes in the control.
      //:// 2. If Action is SelectChange, use onchange.
      //:// 3. If Action is SelectEnd, use onblur
      //:IF vDialog.Event EXISTS
      lTempInteger_4 = CheckExistenceOfEntity( vDialog, "Event" );
      if ( lTempInteger_4 == 0 )
      { 
         //:IF vDialog.Event.Type = 1
         if ( CompareAttributeToInteger( vDialog, "Event", "Type", 1 ) == 0 )
         { 
            //:szSelectAction   = "onchange"
            ZeidonStringCopy( szSelectAction, 1, 0, "onchange", 1, 0, 11 );
            //:szSelectFunction = "OnChange"
            ZeidonStringCopy( szSelectFunction, 1, 0, "OnChange", 1, 0, 11 );
            //:ELSE
         } 
         else
         { 
            //:szSelectAction   = "onblur"
            ZeidonStringCopy( szSelectAction, 1, 0, "onblur", 1, 0, 11 );
            //:szSelectFunction = "OnBlur"
            ZeidonStringCopy( szSelectFunction, 1, 0, "OnBlur", 1, 0, 11 );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:szSelectAction   = "onchange"
         ZeidonStringCopy( szSelectAction, 1, 0, "onchange", 1, 0, 11 );
         //:szSelectFunction = "OnChange"
         ZeidonStringCopy( szSelectFunction, 1, 0, "OnChange", 1, 0, 11 );
      } 

      //:END

      //:// If there is a Default button for the window, we must add some characters
      //:// to the end of the input statement.
      //:szKeyPressCode = ""
      ZeidonStringCopy( szKeyPressCode, 1, 0, "", 1, 0, 51 );
      //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
      GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );
      //://CreateViewFromView( vDialogRoot, vDialog )
      //://nRC = ResetViewFromSubobject( vDialogRoot )
      //://LOOP WHILE nRC = 0
      //://   nRC = ResetViewFromSubobject( vDialogRoot )
      //://END
      //:IF vDialogRoot.Window.DfltButton != ""
      if ( CompareAttributeToString( vDialogRoot, "Window", "DfltButton", "" ) != 0 )
      { 
         //:szKeyPressCode = szKeyPressCode + " onkeypress=^return _OnEnter( event )^"
         ZeidonStringConcat( szKeyPressCode, 1, 0, " onkeypress=^return _OnEnter( event )^", 1, 0, 51 );
      } 

      //:END            

      //:CreateDisabledString( vDialog, szDisabled )
      CreateDisabledString( vDialog, szDisabled );
      //:IF szRepeatGrpKey = ""
      if ( ZeidonStringCompare( szRepeatGrpKey, 1, 0, "", 1, 0, 101 ) == 0 )
      { 
         //:szWriteBuffer = "<select " + szClass + " name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> size=^1^ " +
         //:                szTitleHTML + szStyle + szDisabled + szSelectAction + "=^" + szCtrlTag + szSelectFunction + "( )^ " + szKeyPressCode + ">"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<select ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> size=^1^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSelectAction, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSelectFunction, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "( )^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szKeyPressCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szCtrlTagRG = szRepeatGrpKey
         ZeidonStringCopy( szCtrlTagRG, 1, 0, szRepeatGrpKey, 1, 0, 101 );
         //://TraceLineS("***** szRepeatGrpKey -----> ", szRepeatGrpKey )
         //:zSearchAndReplace( szCtrlTagRG, 100, "::", "" )
         zSearchAndReplace( szCtrlTagRG, 100, "::", "" );
         //://TraceLineS("***** szCtrlTagRG    -----> ", szCtrlTagRG )
         //:szWriteBuffer = "<select " + szClass + " name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ size=^1^ " +
         //:                szTitleHTML + szStyle + szDisabled + szSelectAction + "=^" + szCtrlTag + szSelectFunction + "( 'h" + szCtrlTag  + szRepeatGrpKey + "', this, '" + 
         //:                szCtrlTagRG + "' )^ " + szKeyPressCode + ">"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<select ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ size=^1^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSelectAction, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szSelectFunction, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "( 'h", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "', this, '", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTagRG, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "' )^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szKeyPressCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      } 

      //:                   //szTitleHTML + szStyle + szDisabled + szSelectAction + "=^" + szCtrlTag + szSelectFunction + "( 'h" + szCtrlTag  + szRepeatGrpKey + ', this, <%=strEntityKeyRG%> )^" + ">"
      //:END

      //:/*
      //:IF szClass = ""
      //:   szWriteBuffer = "<select name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ size=^1^ " + szTitleHTML + szStyle + szDisabled + szSelectAction + "=^" +
      //:                   szCtrlTag + szSelectFunction + "( )^" + ">"
      //:ELSE
      //:   szWriteBuffer = "<select class=^" + szClass + "^ name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ size=^1^ " +
      //:                   szTitleHTML + szStyle + szDisabled + szSelectAction + "=^" + szCtrlTag + szSelectFunction + "( )^" + ">"
      //:END
      //:*/      
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:IF vDialog.CtrlMapView EXISTS
      lTempInteger_5 = CheckExistenceOfEntity( vDialog, "CtrlMapView" );
      if ( lTempInteger_5 == 0 )
      { 
         //:// KJS 10/22/14 - If the value from the database is not part of the domain anymore, we will put that option in but
         //:// we will set it as disabled.
         //:szInListCtrlTag = "inList" + szCtrlTag
         ZeidonStringCopy( szInListCtrlTag, 1, 0, "inList", 1, 0, 101 );
         ZeidonStringConcat( szInListCtrlTag, 1, 0, szCtrlTag, 1, 0, 101 );

         //:szWriteBuffer = "   boolean " + szInListCtrlTag + " = false;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   boolean ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szInListCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " = false;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

         //:szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
         //:             "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
         GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) )"
         GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:// If the view is readOnly, then we want to disable this control.
         //:/*
         //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
         //:IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y"
         //:   szWriteBuffer = "               if ( " + vDialog.CtrlMapView.Name + ".isReadOnly( ) )"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   //szWriteBuffer = "                  strCtrlActionsView = strCtrlActionsView + ^" + szCtrlTag + ",Disable,^;"
         //:   szWriteBuffer = "                  strCtrlActions = strCtrlActions + ^" + szCtrlTag + ",Disable,^;"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:END
         //:*/


         //:// We get each table value from a core call.
         //:IF vDialog.CtrlMapLOD_Attribute EXISTS
         lTempInteger_6 = CheckExistenceOfEntity( vDialog, "CtrlMapLOD_Attribute" );
         if ( lTempInteger_6 == 0 )
         { 

            //:IF vDialog.CtrlMapContext EXISTS
            lTempInteger_7 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
            if ( lTempInteger_7 == 0 )
            { 
               //:szContextName = vDialog.CtrlMapContext.Name
               GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
               //:ELSE
            } 
            else
            { 
               //:szContextName = ""
               ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
            } 

            //:END

            //:szWriteBuffer = "      List<TableEntry> list = JspWebUtils.getTableDomainValues( " + vDialog.CtrlMapView.Name + " , ^" +
            //:                vDialog.CtrlMapRelatedEntity.Name + "^, ^" + vDialog.CtrlMapER_Attribute.Name + "^, ^" + szContextName + "^ );"
            GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      List<TableEntry> list = JspWebUtils.getTableDomainValues( ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " , ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^, ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

            //:szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
            //:         vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
            GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_12, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      if ( nRC >= 0 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:// Get the current value of the combo box.
            //:szWriteBuffer = "         strComboCurrentValue = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
            //:         vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
            //:         vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^^ );"
            GetVariableFromAttribute( szTempString_13, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strComboCurrentValue = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_13, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_14, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_14, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_15, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_15, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^^ );", 1, 0, 10001 );
            //:         // KJS 10/22/14 - Right now we are not adding the context name, we only want the default, which
            //:         // I am currently assuming has ALL the values (old and new) of this domain. When we get DGs domain fix
            //:         // I think we need to put this back!!!!!!!
            //:         //vDialog.CtrlMapER_Attribute.Name + "^, ^" + szContextName + "^ );"
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
            //:szWriteBuffer = "         if ( strComboCurrentValue == null )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( strComboCurrentValue == null )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            strComboCurrentValue = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            strComboCurrentValue = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      else"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         strComboCurrentValue = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strComboCurrentValue = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

            //:IF vDialog.CtrlMapER_Attribute.NotNull != "Y"
            if ( CompareAttributeToString( vDialog, "CtrlMapER_Attribute", "NotNull", "Y" ) != 0 )
            { 
               //:// Generate code when attribute is NOT required.
               //:szWriteBuffer = "      // Code for NOT required attribute, which makes sure a blank entry exists."
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      // Code for NOT required attribute, which makes sure a blank entry exists.", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      if ( strComboCurrentValue == ^^ )"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( strComboCurrentValue == ^^ )", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      {"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

               //:szWriteBuffer = "         " + szInListCtrlTag + " = true;"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         ", 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, szInListCtrlTag, 1, 0, 10001 );
               ZeidonStringConcat( szWriteBuffer, 1, 0, " = true;", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

               //:szWriteBuffer = "%>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         <option selected=^selected^ value=^^></option>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         <option selected=^selected^ value=^^></option>", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "<%"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      }"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      else"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      {"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "%>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "         <option value=^^></option>"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "         <option value=^^></option>", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "<%"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
               //:szWriteBuffer = "      }"
               ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
               //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
               WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            } 

            //:END

            //:szWriteBuffer = "      for ( TableEntry entry : list )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      for ( TableEntry entry : list )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         String internalValue = entry.getInternalValue( );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         String internalValue = entry.getInternalValue( );", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         String externalValue = entry.getExternalValue( );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         String externalValue = entry.getExternalValue( );", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         // Perhaps getInternalValue and getExternalValue should return an empty string, "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // Perhaps getInternalValue and getExternalValue should return an empty string, ", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         // but currently it returns null.  Set to empty string. "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         // but currently it returns null.  Set to empty string. ", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         if ( externalValue == null )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( externalValue == null )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            internalValue = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            internalValue = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            externalValue = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            externalValue = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

            //:// The following if statement eliminates a null value, which would be the second null in the combo box.
            //:szWriteBuffer = "         if ( !StringUtils.isBlank( externalValue ) )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( !StringUtils.isBlank( externalValue ) )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            if ( StringUtils.equals( strComboCurrentValue, externalValue ) )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( StringUtils.equals( strComboCurrentValue, externalValue ) )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:szWriteBuffer = "               " + szInListCtrlTag + " = true;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "               ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szInListCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " = true;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:szWriteBuffer = "%>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:// KJS 03/08/18 - The old way was setting value to externalValue but we are now setting to internalValue.
            //:// This is better for language conversion and javascript. Not to mention that if the external name changes... 
            //:szWriteBuffer = "               <option selected=^selected^ value=^<%=internalValue%>^><%=externalValue%></option>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "               <option selected=^selected^ value=^<%=internalValue%>^><%=externalValue%></option>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "<%"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            else"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            else", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "%>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:// KJS 03/08/18 - The old way was setting value to externalValue but we are now setting to internalValue.
            //:// This is better for language conversion and javascript. Not to mention that if the external name changes... 
            //:szWriteBuffer = "               <option value=^<%=internalValue%>^><%=externalValue%></option>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "               <option value=^<%=internalValue%>^><%=externalValue%></option>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "<%"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }  // for ( TableEntry entry"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }  // for ( TableEntry entry", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:// KJS 10/22/14 - Add this value as disabled.
            //:szWriteBuffer = "      // The value from the database isn't in the domain, add it to the list as disabled."
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      // The value from the database isn't in the domain, add it to the list as disabled.", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      if ( !" + szInListCtrlTag + " )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( !", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szInListCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      { "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      { ", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "%>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:szWriteBuffer = "         <option disabled selected=^selected^ value=^<%=strComboCurrentValue%>^><%=strComboCurrentValue%></option>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         <option disabled selected=^selected^ value=^<%=strComboCurrentValue%>^><%=strComboCurrentValue%></option>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "<%"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:szWriteBuffer = "      }  "
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }  ", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         } 

         //:   
         //:   
         //:END
      } 

      //:END

      //:szWriteBuffer = "   }  // if view != null"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }  // if view != null", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "</select>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</select>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:szWriteBuffer = "<input name=^h" + szCtrlTag + szRepeatGrpKey + "^ id=^h" + szCtrlTag + szRepeatGrpKey + "^ type=^hidden^ value=^<%=strComboCurrentValue%>^ >"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<input name=^h", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^h", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ type=^hidden^ value=^<%=strComboCurrentValue%>^ >", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:ELSE  
   } 
   else
   { 
      //:// Select type combo box
      //:CreateViewFromViewForTask( vList, vDialog, 0 )
      CreateViewFromViewForTask( &vList, vDialog, 0 );
      //:SET CURSOR NEXT vList.CtrlMap
      RESULT = SetCursorNextEntity( vList, "CtrlMap", "" );
      //:CreateViewFromViewForTask( vEntity, vList, 0 )
      CreateViewFromViewForTask( &vEntity, vList, 0 );
      //:SET CURSOR NEXT vEntity.CtrlMap
      RESULT = SetCursorNextEntity( vEntity, "CtrlMap", "" );
      //:CreateViewFromViewForTask( vScope, vEntity, 0 )
      CreateViewFromViewForTask( &vScope, vEntity, 0 );
      //:SET CURSOR NEXT vScope.CtrlMap
      RESULT = SetCursorNextEntity( vScope, "CtrlMap", "" );

      //:// The Select Action is either onblur or onchange depending on the following.
      //:// 1. If no Action, use onchange because this allows using keystrokes in the control.
      //:// 2. If Action is SelectChange, use onchange.
      //:// 3. If Action is SelectEnd, use onfocusout.
      //:IF vDialog.Event EXISTS
      lTempInteger_8 = CheckExistenceOfEntity( vDialog, "Event" );
      if ( lTempInteger_8 == 0 )
      { 
         //:IF vDialog.Event.Type = 1
         if ( CompareAttributeToInteger( vDialog, "Event", "Type", 1 ) == 0 )
         { 
            //:szSelectAction   = "onchange"
            ZeidonStringCopy( szSelectAction, 1, 0, "onchange", 1, 0, 11 );
            //:szSelectFunction = "OnChange"
            ZeidonStringCopy( szSelectFunction, 1, 0, "OnChange", 1, 0, 11 );
            //:ELSE
         } 
         else
         { 
            //:szSelectAction   = "onblur"
            ZeidonStringCopy( szSelectAction, 1, 0, "onblur", 1, 0, 11 );
            //:szSelectFunction = "OnBlur"
            ZeidonStringCopy( szSelectFunction, 1, 0, "OnBlur", 1, 0, 11 );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:szSelectAction = "onchange"
         ZeidonStringCopy( szSelectAction, 1, 0, "onchange", 1, 0, 11 );
         //:szSelectFunction = "OnChange"
         ZeidonStringCopy( szSelectFunction, 1, 0, "OnChange", 1, 0, 11 );
      } 

      //:END

      //:szWriteBuffer = "<select " + szClass + " name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> size=^1^" +
      //:                szTitleHTML + szStyle + szDisabled + szSelectAction + "=^" + szCtrlTag + szSelectFunction + "( )^" + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<select ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> size=^1^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szSelectAction, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szSelectFunction, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "( )^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:szWriteBuffer = "<%"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:IF vDialog.CtrlMapView EXISTS
      lTempInteger_9 = CheckExistenceOfEntity( vDialog, "CtrlMapView" );
      if ( lTempInteger_9 == 0 )
      { 
         //:szWriteBuffer = "   " + vList.CtrlMapView.Name + " = " +
         //:                "task.getViewByName( ^" + vList.CtrlMapView.Name + "^ );"
         GetVariableFromAttribute( szTempString_16, 0, 'S', 33, vList, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_16, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_17, 0, 'S', 33, vList, "CtrlMapView", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_17, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   if ( VmlOperation.isValid( " + vList.CtrlMapView.Name + " ) )"
         GetVariableFromAttribute( szTempString_18, 0, 'S', 33, vList, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_18, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "   {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         strComboCurrentValue = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strComboCurrentValue = ^^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      View v" + szCtrlTag + ";"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      View v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ";", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:// If mapping is specified, set up the value to be used in selecting
         //:// the correct entry.
         //:IF vDialog.CtrlMapLOD_Attribute EXISTS
         lTempInteger_10 = CheckExistenceOfEntity( vDialog, "CtrlMapLOD_Attribute" );
         if ( lTempInteger_10 == 0 )
         { 
            //:szWriteBuffer = "      " + vDialog.CtrlMapView.Name + " = " +
            //:                "task.getViewByName( ^" + vDialog.CtrlMapView.Name +
            //:                "^ );"
            GetVariableFromAttribute( szTempString_19, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_19, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_20, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_20, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) )"
            GetVariableFromAttribute( szTempString_21, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( VmlOperation.isValid( ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_21, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " ) )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
            //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
            GetVariableFromAttribute( szTempString_22, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_22, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_23, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_23, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         if ( nRC >= 0 )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:IF vDialog.CtrlMapContext EXISTS
            lTempInteger_11 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
            if ( lTempInteger_11 == 0 )
            { 
               //:szContextName = vDialog.CtrlMapContext.Name
               GetVariableFromAttribute( szContextName, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
               //:ELSE
            } 
            else
            { 
               //:szContextName = ""
               ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
            } 

            //:END
            //:szWriteBuffer = "            strComboCurrentValue = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
            //:         vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
            //:         vDialog.CtrlMapER_Attribute.Name + "^ ).getString( ^" + szContextName + "^ );"
            GetVariableFromAttribute( szTempString_24, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            strComboCurrentValue = ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_24, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_25, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_25, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_26, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_26, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
            //:szWriteBuffer = "            if ( strComboCurrentValue == null )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( strComboCurrentValue == null )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "               strComboCurrentValue = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "               strComboCurrentValue = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 
            //:szWriteBuffer = "      strComboCurrentValue = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      strComboCurrentValue = ^^;", 1, 0, 10001 );
         } 

         //:END
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:// See if a list entity has been specified, and if so, use it.
         //:IF vEntity.CtrlMapLOD_Entity EXISTS
         lTempInteger_12 = CheckExistenceOfEntity( vEntity, "CtrlMapLOD_Entity" );
         if ( lTempInteger_12 == 0 )
         { 
            //:szListEntityName = vEntity.CtrlMapLOD_Entity.Name
            GetVariableFromAttribute( szListEntityName, 0, 'S', 33, vEntity, "CtrlMapLOD_Entity", "Name", "", 0 );
            //:ELSE
         } 
         else
         { 
            //:szListEntityName = vList.CtrlMapRelatedEntity.Name
            GetVariableFromAttribute( szListEntityName, 0, 'S', 33, vList, "CtrlMapRelatedEntity", "Name", "", 0 );
         } 

         //:END

         //:// See if a scoping entity has been specified, and if so, use it.
         //:lSubtype = vDialog.Control.Subtype
         GetIntegerFromAttribute( &lSubtype, vDialog, "Control", "Subtype" );
         //:lSubtype = IsFlagSequenceSet( lSubtype, zCOMBOBOX_SCOPE_OI )
         lSubtype = IsFlagSequenceSet( lSubtype, zCOMBOBOX_SCOPE_OI );
         //:IF lSubtype = 0
         if ( lSubtype == 0 )
         { 
            //:IF vScope.CtrlMapLOD_Entity EXISTS
            lTempInteger_13 = CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" );
            if ( lTempInteger_13 == 0 )
            { 
               //:szScopingEntityName = "^" + vScope.CtrlMapLOD_Entity.Name + "^"
               GetVariableFromAttribute( szTempString_27, 0, 'S', 33, vScope, "CtrlMapLOD_Entity", "Name", "", 0 );
               ZeidonStringCopy( szScopingEntityName, 1, 0, "^", 1, 0, 33 );
               ZeidonStringConcat( szScopingEntityName, 1, 0, szTempString_27, 1, 0, 33 );
               ZeidonStringConcat( szScopingEntityName, 1, 0, "^", 1, 0, 33 );
               //:ELSE
            } 
            else
            { 
               //:szScopingEntityName = ""
               ZeidonStringCopy( szScopingEntityName, 1, 0, "", 1, 0, 33 );
            } 

            //:END
         } 

         //:END

         //:szWriteBuffer = "      v" + szCtrlTag + " = " + vList.CtrlMapView.Name + ".newView( );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_28, 0, 'S', 33, vList, "CtrlMapView", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_28, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".newView( );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      ComboCount = 0;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      ComboCount = 0;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      strComboSelectedValue = ^0^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strComboSelectedValue = ^0^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

         //:// Auto Include
         //:IF vDialog.Control.Subtype >= 20480 AND vDialog.Control.Subtype <= 21252   // IF Auto Include
         if ( CompareAttributeToInteger( vDialog, "Control", "Subtype", 20480 ) >= 0 && CompareAttributeToInteger( vDialog, "Control", "Subtype", 21252 ) <= 0 )
         { 
            //:szWriteBuffer = ""
            ZeidonStringCopy( szWriteBuffer, 1, 0, "", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:// For Auto Include, we are automatically putting a blank entry as the first entry in the
            //:// combobox.  With this in mind, there is code in DoInputMapping that assumes the first
            //:// entry in a combobox is null (or blank).  What if the user doesn't want a blank entry?
            //:// and wants to use the "No null in list" checkbox in the painter.  I would need to
            //:// change this code but I'm not sure how because I can't tell from DoInputMapping what
            //:// the value of combobox.selectedindex[0] is, I only know they selected the index 0.
            //:szWriteBuffer = "      // For Auto Include, always add a null entry to the combo box."
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      // For Auto Include, always add a null entry to the combo box.", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      ComboCount++;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      ComboCount++;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      if ( StringUtils.isBlank( strComboCurrentValue ) )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( StringUtils.isBlank( strComboCurrentValue ) )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "%>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         <option selected=^selected^></option>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         <option selected=^selected^></option>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "<%"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      else"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      {"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "%>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "         <option></option>"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         <option></option>", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "<%"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "      }"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
         } 

         //:END

         //:IF lSubtype = 0
         if ( lSubtype == 0 )
         { 
            //:szWriteBuffer = "      csrRC = v" + szCtrlTag + ".cursor( ^" +
            //:                             szListEntityName + "^ ).setFirst( " + szScopingEntityName + " );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      csrRC = v", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirst( ", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szScopingEntityName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, " );", 1, 0, 10001 );
            //:ELSE
         } 
         else
         { 
            //:szWriteBuffer = "      csrRC = v" + szCtrlTag + ".cursor( ^" + szListEntityName + "^ ).setFirstWithinOi( );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "      csrRC = v", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setFirstWithinOi( );", 1, 0, 10001 );
         } 

         //:END

         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      while ( csrRC.isSet() )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      while ( csrRC.isSet() )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:IF vList.CtrlMapLOD_Attribute EXISTS
         lTempInteger_14 = CheckExistenceOfEntity( vList, "CtrlMapLOD_Attribute" );
         if ( lTempInteger_14 == 0 )
         { 
            //:IF vList.CtrlMapContext EXISTS
            lTempInteger_15 = CheckExistenceOfEntity( vList, "CtrlMapContext" );
            if ( lTempInteger_15 == 0 )
            { 
               //:szContextName = vList.CtrlMapContext.Name
               GetVariableFromAttribute( szContextName, 0, 'S', 33, vList, "CtrlMapContext", "Name", "", 0 );
               //:ELSE
            } 
            else
            { 
               //:szContextName = ""
               ZeidonStringCopy( szContextName, 1, 0, "", 1, 0, 33 );
            } 

            //:END

            //:szWriteBuffer = "         strErrorMapValue = v" + szCtrlTag + ".cursor( ^" +
            //:                vList.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
            //:                vList.CtrlMapER_Attribute.Name + "^ ).getString( ^" +
            //:                szContextName + "^ );"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorMapValue = v", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_29, 0, 'S', 33, vList, "CtrlMapRelatedEntity", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_29, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
            GetVariableFromAttribute( szTempString_30, 0, 'S', 33, vList, "CtrlMapER_Attribute", "Name", "", 0 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_30, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ^", 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
            ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

            //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
            //:szWriteBuffer = "         if ( strErrorMapValue == null )"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( strErrorMapValue == null )", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
            //:szWriteBuffer = "            strErrorMapValue = ^^;"
            ZeidonStringCopy( szWriteBuffer, 1, 0, "            strErrorMapValue = ^^;", 1, 0, 10001 );
            //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
            WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
         } 


         //:END

         //:// KJS 05/20/14 - There were some issues for auto include when the "list view" already had a blank entry, then
         //:// two blank entries were in the list and this caused errors. For auto include check if one of these entries is
         //:// blank, don't add it if it is.
         //:/*
         //:IF vDialog.Control.Subtype >= 20480 AND vDialog.Control.Subtype <= 21252   // IF Auto Include
         //:   szWriteBuffer = "      // For Auto Include, we have already created a blank entry for the combobox, check to make"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "         // we don't add another blank entry. That will cause errors. "
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "         if ( !StringUtils.isBlank( strErrorMapValue ) )"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:   szWriteBuffer = "         {"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         //:END
         //: */
         //:szWriteBuffer = "         if ( StringUtils.equals( strComboCurrentValue, strErrorMapValue ) )"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( StringUtils.equals( strComboCurrentValue, strErrorMapValue ) )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "%>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "            <option selected=^selected^><%=strErrorMapValue%></option>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            <option selected=^selected^><%=strErrorMapValue%></option>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "<%"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "            strComboSelectedValue = Integer.toString( ComboCount );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            strComboSelectedValue = Integer.toString( ComboCount );", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         else"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         {"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "%>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "            <option><%=strErrorMapValue%></option>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "            <option><%=strErrorMapValue%></option>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "<%"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "         }"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

         //:/*
         //:IF vDialog.Control.Subtype >= 20480 AND vDialog.Control.Subtype <= 21252   // IF Auto Include
         //:   szWriteBuffer = "         } // if ( !StringUtils.isBlank( strErrorMapValue ) )"
         //:   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
         //:END
         //:*/
         //:szWriteBuffer = "         ComboCount++;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         ComboCount++;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:// IF lSubtype = 0  now always do setNextContinue here for performance reasons dks/dgc 2011.05.03
         //:   szWriteBuffer = "         csrRC =  v" + szCtrlTag +
         //:                   ".cursor( ^" + szListEntityName +
         //:                   "^ ).setNextContinue( );"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         csrRC =  v", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szListEntityName, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).setNextContinue( );", 1, 0, 10001 );
         //:                   //"^ ).setNext( " + szScopingEntityName + " );"
         //:// ELSE
         //://    szWriteBuffer = "         nRC =  v" + szCtrlTag +
         //://                    ".cursor( ^" + szListEntityName + "^ ).setNextWithinOi( );"
         //:// END

         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:DropView( vList )
         DropView( vList );
         //:DropView( vEntity )
         DropView( vEntity );
         //:DropView( vScope )
         DropView( vScope );
      } 

      //:END

      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
      //:szWriteBuffer = "      v" + szCtrlTag + ".drop( );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      v", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".drop( );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );


      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "</select>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</select>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "<input name=^h" + szCtrlTag + szRepeatGrpKey + "^ id=^h" + szCtrlTag + szRepeatGrpKey + "^ type=^hidden^ value=^<%=strComboSelectedValue%>^ >"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<input name=^h", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^h", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ type=^hidden^ value=^<%=strComboSelectedValue%>^ >", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   } 

   //:END
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteCheckBox( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                     INTEGER          lFile,
//:                     STRING ( 10000 ) szWriteBuffer,
//:                     STRING ( 50 )    szIndent,
//:                     STRING ( 32 )    szCtrlTag,
//:                     STRING ( 1 )     szTableRowFlag,
//:                     STRING ( 20 )    szJustify,
//:                     STRING ( 1 )     szNoPositioning,
//:                     INTEGER          lOffsetX,
//:                     INTEGER          lOffsetY,
//:                     STRING ( 100 )   szRepeatGrpKey )

//:   // Format an CHECKBOX control.
//:   VIEW vDialogRoot BASED ON LOD TZWDLGSO
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteCheckBox( zVIEW     vDialog,
                      zLONG     lFile,
                      zPCHAR    szWriteBuffer,
                      zPCHAR    szIndent,
                      zPCHAR    szCtrlTag,
                      zPCHAR    szTableRowFlag,
                      zPCHAR    szJustify,
                      zPCHAR    szNoPositioning,
                      zLONG     lOffsetX,
                      zLONG     lOffsetY,
                      zPCHAR    szRepeatGrpKey )
{
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 300 ) szStyle
   zCHAR     szStyle[ 301 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitle
   zCHAR     szTitle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szTitleHTML
   zCHAR     szTitleHTML[ 257 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:STRING ( 300 ) szPosition
   zCHAR     szPosition[ 301 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 20 )  szSpaces
   zCHAR     szSpaces[ 21 ] = { 0 }; 
   //:STRING ( 256 ) szBlob
   zCHAR     szBlob[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szUnChecked
   zCHAR     szUnChecked[ 257 ] = { 0 }; 
   //:STRING ( 500 ) szText
   zCHAR     szText[ 501 ] = { 0 }; 
   //:STRING ( 500 ) szText2
   zCHAR     szText2[ 501 ] = { 0 }; 
   //:STRING ( 256 ) szTextID
   zCHAR     szTextID[ 257 ] = { 0 }; 
   //:STRING ( 5 )   szSrch
   zCHAR     szSrch[ 6 ] = { 0 }; 
   //:INTEGER lTemp
   zLONG     lTemp = 0; 
   //:INTEGER lLen
   zLONG     lLen = 0; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 51 ]; 
   zSHORT    lTempInteger_1; 
   zCHAR     szTempString_10[ 51 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 51 ]; 


   //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
   GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );

   //:// See if the checkbox has an unchecked value. The checked value is in attrib RadioOrCheckboxValue but we 
   //:// don't have an attrib for the unchecked value.
   //:/*
   //:lTemp = 256
   //:GetBlobFromAttribute( szBlob, lTemp, vDialog, "Control", "CtrlBOI" )
   //:lLen = zstrlen( szBlob )
   //:szUnChecked = ""
   //:IF lTemp > ( lLen + 2 )
   //:   ZeidonCopyWithinString( szBlob, 1, lLen + 2, 256 )
   //:   szUnChecked = szBlob
   //:END
   //:*/

   //:szWriteBuffer = "<%"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   strErrorMapValue = ^^;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   strErrorMapValue = ^^;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:IF vDialog.CtrlMapView EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDialog, "CtrlMapView" );
   if ( lTempInteger_0 == 0 )
   { 
      //:szWriteBuffer = "   " + vDialog.CtrlMapView.Name + " = " +
      //:                "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_0, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      task.log( ).debug( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      task.log( ).debug( ^Invalid View: ^ + ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y"
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesControlActionsView", "Y" ) == 0 )
      { 
         //:szWriteBuffer = "      strDisabled = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      strDisabled = ^^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "      if ( " + vDialog.CtrlMapView.Name + ".isReadOnly( ) )"
         GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".isReadOnly( ) )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //://szWriteBuffer = "                  strCtrlActionsView = strCtrlActionsView + ^" + szCtrlTag + ",Disabled,^;"
         //://szWriteBuffer = "                  strCtrlActions = strCtrlActions + ^" + szCtrlTag + ",Disable,^;"
         //:szWriteBuffer = "         strDisabled = ^disabled^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "         strDisabled = ^disabled^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:szWriteBuffer = "      nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      nRC = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( nRC >= 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( nRC >= 0 )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         strRadioGroupValue = " +
      //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
      //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( );"
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         strRadioGroupValue = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_6, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   } 

   //:END

   //:szWriteBuffer = "   if ( StringUtils.equals( strRadioGroupValue, ^" +
   //:                vDialog.Control.RadioOrCheckboxValue + "^ ) )"
   GetVariableFromAttribute( szTempString_9, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( StringUtils.equals( strRadioGroupValue, ^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ) )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "      strErrorMapValue = ^checked=\^checked\^^;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      strErrorMapValue = ^checked=\\^checked\\^^;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "%>"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

   //:CreateTabIndexString( vDialog, szTabIndex )
   CreateTabIndexString( vDialog, szTabIndex );

   //:// If this window is being generated with no positioning then
   //:// do not create the position in the style.
   //:IF szNoPositioning = ""
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
   { 
      //:CreateAbsolPosString( vDialog, szPosition, lOffsetX, lOffsetY )
      CreateAbsolPosString( vDialog, szPosition, lOffsetX, lOffsetY );
   } 

   //:END

   //:IF vDialog.Control.VisibleBorder = "Y"
   if ( CompareAttributeToString( vDialog, "Control", "VisibleBorder", "Y" ) == 0 )
   { 
      //:szPosition = szPosition + "border:solid;border-width:2px;border-style:groove;"
      ZeidonStringConcat( szPosition, 1, 0, "border:solid;border-width:2px;border-style:groove;", 1, 0, 301 );
   } 

   //:END

   //:szStyle = "style=^" + szPosition + "^" + szTabIndex
   ZeidonStringCopy( szStyle, 1, 0, "style=^", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szPosition, 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, "^", 1, 0, 301 );
   ZeidonStringConcat( szStyle, 1, 0, szTabIndex, 1, 0, 301 );

   //:IF szNoPositioning = "S"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
   { 
      //:szStyle = "" // No style
      ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
   } 

   //:END

   //:CreateDisabledString( vDialog, szDisabled )
   CreateDisabledString( vDialog, szDisabled );

   //://If we put a title on the control, when the mouse is hovered over the control, this text will display.
   //:szTitleHTML = ""
   ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
   //:szTitle = vDialog.Control.DIL_Text
   GetVariableFromAttribute( szTitle, 0, 'S', 257, vDialog, "Control", "DIL_Text", "", 0 );
   //:IF szTitle = ""
   if ( ZeidonStringCompare( szTitle, 1, 0, "", 1, 0, 257 ) == 0 )
   { 
      //:szTitleHTML = ""
      ZeidonStringCopy( szTitleHTML, 1, 0, "", 1, 0, 257 );
      //:ELSE
   } 
   else
   { 
      //:szTitleHTML = " title=^" + szTitle + "^ "
      ZeidonStringCopy( szTitleHTML, 1, 0, " title=^", 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, szTitle, 1, 0, 257 );
      ZeidonStringConcat( szTitleHTML, 1, 0, "^ ", 1, 0, 257 );
   } 

   //:END

   //:IF vDialog.EventAct EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( vDialog, "EventAct" );
   if ( lTempInteger_1 == 0 )
   { 
      //:szWriteBuffer = "<input type=^checkbox^ name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> " + szDisabled +
      //:                " value=^" + vDialog.Control.RadioOrCheckboxValue +
      //:                "^ <%=strErrorMapValue%> " + szTitleHTML + szStyle + " onclick=^" + vDialog.EventAct.Tag + "( )^>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<input type=^checkbox^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " value=^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_10, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strErrorMapValue%> ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " onclick=^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialog, "EventAct", "Tag", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "( )^>", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "<input type=^checkbox^ name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> " + szDisabled +
      //:                " value=^" + vDialog.Control.RadioOrCheckboxValue +
      //:                "^ <%=strErrorMapValue%> " + szTitleHTML + szStyle + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<input type=^checkbox^ name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " value=^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_12, 0, 'S', 51, vDialog, "Control", "RadioOrCheckboxValue", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strErrorMapValue%> ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
   } 

   //:END
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "<input type=^hidden^ name=^zhidden" + szCtrlTag + szRepeatGrpKey + "^ id=^zhidden" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> " + szDisabled + " value=^^ >"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<input type=^hidden^ name=^zhidden", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^zhidden", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> ", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, " value=^^ >", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

   //:// If this window is being generated with no positioning then
   //:// do not create the position in the style.
   //:IF szNoPositioning = "Y"
   if ( ZeidonStringCompare( szNoPositioning, 1, 0, "Y", 1, 0, 2 ) == 0 )
   { 
      //:CreateNoPosStyleString( vDialog, szStyle, "" )
      CreateNoPosStyleString( vDialog, szStyle, "" );
      //:// If we aren't putting in absolute positioning, make sure that there is a little space between
      //:// the checkbox and the text.
      //:szSpaces = "&nbsp "
      ZeidonStringCopy( szSpaces, 1, 0, "&nbsp ", 1, 0, 21 );
      //:ELSE
   } 
   else
   { 
      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = ""  // No style information.
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 301 );
         //:szSpaces = ""
         ZeidonStringCopy( szSpaces, 1, 0, "", 1, 0, 21 );
         //:ELSE
      } 
      else
      { 
         //:CreateAbsolStyleString( vDialog, szStyle, 30 + lOffsetX, lOffsetY, "" )
         CreateAbsolStyleString( vDialog, szStyle, 30 + lOffsetX, lOffsetY, "" );
         //:szSpaces = ""
         ZeidonStringCopy( szSpaces, 1, 0, "", 1, 0, 21 );
      } 

      //:END
   } 

   //:END
   //:szClass = vDialog.Control.CSS_Class
   GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );


   //:// KJS 09/30/16 - Language Conversion.
   //:// I'm not sure that I want to use language conversion on mapped fields, but then again, I'm thinking maybe we should.
   //:// So for now, I am always using, if the flag is set.
   //:IF vDialogRoot.Dialog.wWebUsesLanguageConversion = "Y"
   if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesLanguageConversion", "Y" ) == 0 )
   { 
      //:szText2 = vDialog.Control.Text
      GetVariableFromAttribute( szText2, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
      //:szTextID = vDialog.Control.LangID
      GetVariableFromAttribute( szTextID, 0, 'S', 257, vDialog, "Control", "LangID", "", 0 );
      //:zSearchAndReplace( szText2, 1000, "\", "\\" )
      zSearchAndReplace( szText2, 1000, "\\", "\\\\" );
      //:szSrch = "\" + QUOTES 
      ZeidonStringCopy( szSrch, 1, 0, "\\", 1, 0, 6 );
      ZeidonStringConcat( szSrch, 1, 0, QUOTES, 1, 0, 6 );
      //:zSearchAndReplace( szText2, 1000, QUOTES, szSrch )
      zSearchAndReplace( szText2, 1000, QUOTES, szSrch );
      //:szSrch = ""
      ZeidonStringCopy( szSrch, 1, 0, "", 1, 0, 6 );
      //:IF szTextID = ""
      if ( ZeidonStringCompare( szTextID, 1, 0, "", 1, 0, 257 ) == 0 )
      { 
         //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
         ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
         ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
         ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
         //:ELSE 
      } 
      else
      { 
         //:szText = "<%=LangConv.getLanguageText(^" + szText2 + "^, ^" + szTextID + "^)%>" 
         ZeidonStringCopy( szText, 1, 0, "<%=LangConv.getLanguageText(^", 1, 0, 501 );
         ZeidonStringConcat( szText, 1, 0, szText2, 1, 0, 501 );
         ZeidonStringConcat( szText, 1, 0, "^, ^", 1, 0, 501 );
         ZeidonStringConcat( szText, 1, 0, szTextID, 1, 0, 501 );
         ZeidonStringConcat( szText, 1, 0, "^)%>", 1, 0, 501 );
      } 

      //:END
      //://szText = "<%=LangConv.getLanguageText(^" + szText2 + "^)%>" 
      //:ELSE
   } 
   else
   { 
      //:szText = vDialog.Control.Text
      GetVariableFromAttribute( szText, 0, 'S', 501, vDialog, "Control", "Text", "", 0 );
   } 

   //:END   

   //:IF szClass = ""
   if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
   { 
      //:szWriteBuffer = "<span name=^span" + szCtrlTag + szRepeatGrpKey + "^ id=^span" + szCtrlTag + szRepeatGrpKey + "^ " + szTitleHTML + szStyle + ">" + szSpaces + szText + "</span>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<span name=^span", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^span", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szSpaces, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "</span>", 1, 0, 10001 );
      //:ELSE
   } 
   else
   { 
      //:szWriteBuffer = "<span name=^span" + szCtrlTag + szRepeatGrpKey + "^ id=^span" + szCtrlTag + szRepeatGrpKey + "^ class=^" + szClass + "^ " + szTitleHTML + szStyle + ">" + szSpaces + szText + "</span>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<span name=^span", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^span", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ class=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTitleHTML, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szSpaces, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szText, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "</span>", 1, 0, 10001 );
   } 

   //:END
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   return;
// END
} 


//:GLOBAL OPERATION
//:GenJSPJ_CrteCalendar( VIEW vDialog BASED ON LOD TZWDLGSO,
//:                     INTEGER          lFile,
//:                     STRING ( 10000 ) szWriteBuffer,
//:                     STRING ( 50 )    szIndent,
//:                     STRING ( 32 )    szCtrlTag,
//:                     STRING ( 1 )     szTableRowFlag,
//:                     STRING ( 20 )    szJustify,
//:                     STRING ( 1 )     szNoPositioning,
//:                     INTEGER          lOffsetX,
//:                     INTEGER          lOffsetY,
//:                     STRING ( 100 )   szRepeatGrpKey )

//:   // Format a CALENDAR control.

//:   VIEW vDialogRoot BASED ON LOD TZWDLGSO
zOPER_EXPORT zVOID OPERATION
GenJSPJ_CrteCalendar( zVIEW     vDialog,
                      zLONG     lFile,
                      zPCHAR    szWriteBuffer,
                      zPCHAR    szIndent,
                      zPCHAR    szCtrlTag,
                      zPCHAR    szTableRowFlag,
                      zPCHAR    szJustify,
                      zPCHAR    szNoPositioning,
                      zLONG     lOffsetX,
                      zLONG     lOffsetY,
                      zPCHAR    szRepeatGrpKey )
{
   zVIEW     vDialogRoot = 0; 
   //:STRING ( 256 ) szStyle
   zCHAR     szStyle[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szStyleCalendar
   zCHAR     szStyleCalendar[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szClass
   zCHAR     szClass[ 257 ] = { 0 }; 
   //:STRING ( 256 ) szContextName
   zCHAR     szContextName[ 257 ] = { 0 }; 
   //:STRING ( 100 ) szDisabled
   zCHAR     szDisabled[ 101 ] = { 0 }; 
   //:STRING ( 10 )  szWidth
   zCHAR     szWidth[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szHeight
   zCHAR     szHeight[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szX_Pos
   zCHAR     szX_Pos[ 11 ] = { 0 }; 
   //:STRING ( 10 )  szY_Pos
   zCHAR     szY_Pos[ 11 ] = { 0 }; 
   //:STRING ( 16 )  szTabIndex
   zCHAR     szTabIndex[ 17 ] = { 0 }; 
   //:STRING ( 50 )  szActionName
   zCHAR     szActionName[ 51 ] = { 0 }; 
   //:STRING ( 50 )  szKeyPressCode
   zCHAR     szKeyPressCode[ 51 ] = { 0 }; 
   //:STRING ( 500 ) szActionCode
   zCHAR     szActionCode[ 501 ] = { 0 }; 
   //:STRING ( 500 ) szCloseUpCode
   zCHAR     szCloseUpCode[ 501 ] = { 0 }; 
   //:DECIMAL        dDLUnits
   ZDecimal  dDLUnits = 0.0; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 
   zCHAR     szTempString_12[ 33 ]; 
   zCHAR     szTempString_13[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zSHORT    lTempInteger_5; 
   zCHAR     szTempString_14[ 33 ]; 


   //:szWriteBuffer = "<%"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "<%", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   strErrorMapValue = VmlOperation.CheckError( ^" + szCtrlTag + "^, strError );"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   strErrorMapValue = VmlOperation.CheckError( ^", 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
   ZeidonStringConcat( szWriteBuffer, 1, 0, "^, strError );", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   if ( !StringUtils.isBlank( strErrorMapValue ) )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   if ( !StringUtils.isBlank( strErrorMapValue ) )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "      if ( StringUtils.equals( strErrorFlag, ^Y^ ) )"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( StringUtils.equals( strErrorFlag, ^Y^ ) )", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "         strErrorColor = ^color:red;^;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "         strErrorColor = ^color:red;^;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   }"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   else"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   else", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "   {"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "   {", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
   //:szWriteBuffer = "      strErrorColor = ^^;"
   ZeidonStringCopy( szWriteBuffer, 1, 0, "      strErrorColor = ^^;", 1, 0, 10001 );
   //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
   WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );


   //:SET CURSOR FIRST vDialog.CtrlMapLOD_Attribute WITHIN vDialog.Control
   RESULT = SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute", "Control" );
   //:IF RESULT >= zCURSOR_SET
   if ( RESULT >= zCURSOR_SET )
   { 
      //:szWriteBuffer = "      " + vDialog.CtrlMapView.Name + " = " +
      //:                "task.getViewByName( ^" + vDialog.CtrlMapView.Name + "^ );"
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_0, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "task.getViewByName( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_1, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      if ( VmlOperation.isValid( " + vDialog.CtrlMapView.Name + " ) == false )"
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      if ( VmlOperation.isValid( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_2, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " ) == false )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         task.log( ).debug( ^Invalid View: ^ + ^" + szCtrlTag + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         task.log( ).debug( ^Invalid View: ^ + ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         nRC = " + vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).checkExistenceOfEntity( ).toInt();"
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         nRC = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_3, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_4, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).checkExistenceOfEntity( ).toInt();", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         if ( nRC >= 0 )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         if ( nRC >= 0 )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         {"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         {", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:   
      //:GetViewByName( vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK )   
      GetViewByName( &vDialogRoot, "DialogRoot", vDialog, zLEVEL_TASK );
      //:   
      //:// KJS 03/13/18 - If the view is read only, then we want to set the combo box to disabled.
      //:// We only do this if the zeidon.ini has UseZeidonControlActionsView
      //:IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y" 
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesControlActionsView", "Y" ) == 0 )
      { 

         //:szWriteBuffer = "               strDisabled = ^^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               strDisabled = ^^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //:szWriteBuffer = "               if ( " + vDialog.CtrlMapView.Name + ".isReadOnly( ) )"
         GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
         ZeidonStringCopy( szWriteBuffer, 1, 0, "               if ( ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_5, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, ".isReadOnly( ) )", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
         //://szWriteBuffer = "                  strCtrlActionsView = strCtrlActionsView + ^" + szCtrlTag + ",Disable,^;"
         //://szWriteBuffer = "                  strCtrlActions = strCtrlActions + ^" + szCtrlTag + ",Disable,^;"
         //:szWriteBuffer = "                  strDisabled = ^disabled^;"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "                  strDisabled = ^disabled^;", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 


      //:END

      //:IF vDialog.CtrlMapContext EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
      if ( lTempInteger_0 == 0 )
      { 
         //:szContextName = "^" + vDialog.CtrlMapContext.Name + "^"
         GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
         ZeidonStringCopy( szContextName, 1, 0, "^", 1, 0, 257 );
         ZeidonStringConcat( szContextName, 1, 0, szTempString_6, 1, 0, 257 );
         ZeidonStringConcat( szContextName, 1, 0, "^", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szContextName = "strDateFormat"
         ZeidonStringCopy( szContextName, 1, 0, "strDateFormat", 1, 0, 257 );
      } 

      //:END      
      //:szWriteBuffer = "            strErrorMapValue = " +
      //:                vDialog.CtrlMapView.Name + ".cursor( ^" +
      //:                vDialog.CtrlMapRelatedEntity.Name + "^ ).getAttribute( ^" +
      //:                vDialog.CtrlMapER_Attribute.Name + "^ ).getString( " + szContextName + " );"
      GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            strErrorMapValue = ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_7, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".cursor( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_8, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_8, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getAttribute( ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_9, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ ).getString( ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szContextName, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, " );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:// KJS 02/15/11 - Thinking I need to add code to change values returned as null to "".
      //:szWriteBuffer = "            if ( strErrorMapValue == null )"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            if ( strErrorMapValue == null )", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "               strErrorMapValue = ^^;"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "               strErrorMapValue = ^^;", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //:szWriteBuffer = "            task.log( ).debug( ^" + vDialog.CtrlMapRelatedEntity.Name + "." +
      //:                vDialog.CtrlMapER_Attribute.Name + ": ^ + strErrorMapValue );"
      GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).debug( ^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_10, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vDialog, "CtrlMapER_Attribute", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_11, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + strErrorMapValue );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "         else"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "         else", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "            task.log( ).debug( ^Entity does not exist for " + szCtrlTag + ": ^ + ^" +
      //:                vDialog.CtrlMapView.Name +
      //:                "." + vDialog.CtrlMapRelatedEntity.Name + "^ );"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "            task.log( ).debug( ^Entity does not exist for ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ": ^ + ^", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_12, 0, 'S', 33, vDialog, "CtrlMapView", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_12, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ".", 1, 0, 10001 );
      GetVariableFromAttribute( szTempString_13, 0, 'S', 33, vDialog, "CtrlMapRelatedEntity", "Name", "", 0 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_13, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ );", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "      }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "      }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "   }"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   }", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:szWriteBuffer = "%>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "%>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );

      //://**************** Get control event actions *********/

      //:// KJS 11/07/16 - Not sure when this was put in, but I don't think it's working and I'm not exactly sure where
      //:// we get the Event.Type numbers because they are what we have in cheetah, but not what we have in zencas (or anywhere
      //:// else). Do we have any calendar actions in windows, because then I can't just change the event.type...
      //:// I'm going to try using the name for right now instead of number?????
      //:// Also... For some reason if I have the CloseUp action AND a context on the date (which then calls the scwShowWFormat action
      //:// as opposed to the scwShow action) then clicking on the calendar image doesn't work. :(
      //:szActionCode = ""
      ZeidonStringCopy( szActionCode, 1, 0, "", 1, 0, 501 );
      //:szCloseUpCode = ""
      ZeidonStringCopy( szCloseUpCode, 1, 0, "", 1, 0, 501 );
      //:FOR EACH vDialog.Event
      RESULT = SetCursorFirstEntity( vDialog, "Event", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:IF vDialog.EventAct EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( vDialog, "EventAct" );
         if ( lTempInteger_1 == 0 )
         { 
            //:szActionName = vDialog.EventAct.Tag
            GetVariableFromAttribute( szActionName, 0, 'S', 51, vDialog, "EventAct", "Tag", "", 0 );
            //:IF vDialog.Event.Type = 80 // Event was GetFocus but I have deleted that because it wasn't working correctly.
            if ( CompareAttributeToInteger( vDialog, "Event", "Type", 80 ) == 0 )
            { 
               //:szActionCode = szActionCode + " onfocus=^" + szActionName + "( )^ "
               ZeidonStringConcat( szActionCode, 1, 0, " onfocus=^", 1, 0, 501 );
               ZeidonStringConcat( szActionCode, 1, 0, szActionName, 1, 0, 501 );
               ZeidonStringConcat( szActionCode, 1, 0, "( )^ ", 1, 0, 501 );
               //:ELSE
            } 
            else
            { 
               //:IF vDialog.Event.Type = 48  // Event is LoseFocus 
               if ( CompareAttributeToInteger( vDialog, "Event", "Type", 48 ) == 0 )
               { 
                  //:szActionCode = szActionCode + " onblur=^" + szActionName + "( )^ "
                  ZeidonStringConcat( szActionCode, 1, 0, " onblur=^", 1, 0, 501 );
                  ZeidonStringConcat( szActionCode, 1, 0, szActionName, 1, 0, 501 );
                  ZeidonStringConcat( szActionCode, 1, 0, "( )^ ", 1, 0, 501 );
                  //:ELSE
               } 
               else
               { 
                  //:IF vDialog.Event.Type = 96 OR vDialog.Event.Type = 64 // Event is CloseUp
                  if ( CompareAttributeToInteger( vDialog, "Event", "Type", 96 ) == 0 || CompareAttributeToInteger( vDialog, "Event", "Type", 64 ) == 0 )
                  { 
                     //:szCloseUpCode = szCloseUpCode + "scwNextAction=" + szActionName + ".runsAfterSCW(this);"
                     ZeidonStringConcat( szCloseUpCode, 1, 0, "scwNextAction=", 1, 0, 501 );
                     ZeidonStringConcat( szCloseUpCode, 1, 0, szActionName, 1, 0, 501 );
                     ZeidonStringConcat( szCloseUpCode, 1, 0, ".runsAfterSCW(this);", 1, 0, 501 );
                  } 

                  //:END
               } 

               //:END
            } 

            //:END
         } 

         RESULT = SetCursorNextEntity( vDialog, "Event", "" );
         //:END
      } 

      //:END
      //://***************************************************

      //:// We will set the height to the constant 32 px.
      //:PIX_PER_DU( vDialog, dDLUnits )
      PIX_PER_DU( vDialog, &dDLUnits );
      //:zIntegerToString( szWidth, 10, vDialog.Control.SZDLG_X * dDLUnits )
      GetIntegerFromAttribute( &lTempInteger_2, vDialog, "Control", "SZDLG_X" );
      zIntegerToString( szWidth, 10, lTempInteger_2 * dDLUnits );
      //:szHeight = "32"
      ZeidonStringCopy( szHeight, 1, 0, "32", 1, 0, 11 );
      //:szStyle = "style=^width:" + szWidth + "px;height:" + szHeight + "px;"
      ZeidonStringCopy( szStyle, 1, 0, "style=^width:", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, szWidth, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, "px;height:", 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, szHeight, 1, 0, 257 );
      ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 257 );

      //:// If szNoPositioning is null then we are using absolute positioning when creating controls.
      //:IF szNoPositioning = ""
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "", 1, 0, 2 ) == 0 )
      { 

         //:zIntegerToString( szX_Pos, 10, vDialog.Control.PSDLG_X * dDLUnits )
         GetIntegerFromAttribute( &lTempInteger_3, vDialog, "Control", "PSDLG_X" );
         zIntegerToString( szX_Pos, 10, lTempInteger_3 * dDLUnits );
         //:zIntegerToString( szY_Pos, 10, vDialog.Control.PSDLG_Y * dDLUnits )
         GetIntegerFromAttribute( &lTempInteger_4, vDialog, "Control", "PSDLG_Y" );
         zIntegerToString( szY_Pos, 10, lTempInteger_4 * dDLUnits );
         //:szStyle = szStyle + "position:absolute;left:" + szX_Pos + "px;" + "top:" + szY_Pos + "px;^ "
         ZeidonStringConcat( szStyle, 1, 0, "position:absolute;left:", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szX_Pos, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, "px;", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, "top:", 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, szY_Pos, 1, 0, 257 );
         ZeidonStringConcat( szStyle, 1, 0, "px;^ ", 1, 0, 257 );
         //:szStyleCalendar = " style=^width:80px^"
         ZeidonStringCopy( szStyleCalendar, 1, 0, " style=^width:80px^", 1, 0, 257 );
         //:ELSE
      } 
      else
      { 
         //:szStyle = szStyle + "^ "
         ZeidonStringConcat( szStyle, 1, 0, "^ ", 1, 0, 257 );
         //:szStyleCalendar = " style=^width:80px^"
         ZeidonStringCopy( szStyleCalendar, 1, 0, " style=^width:80px^", 1, 0, 257 );
      } 

      //:END

      //:IF szNoPositioning = "S"
      if ( ZeidonStringCompare( szNoPositioning, 1, 0, "S", 1, 0, 2 ) == 0 )
      { 
         //:szStyle = "" // No style information
         ZeidonStringCopy( szStyle, 1, 0, "", 1, 0, 257 );
         //:szStyleCalendar = ""
         ZeidonStringCopy( szStyleCalendar, 1, 0, "", 1, 0, 257 );
      } 

      //:END

      //:CreateTabIndexString( vDialog, szTabIndex )
      CreateTabIndexString( vDialog, szTabIndex );

      //:CreateDisabledString( vDialog, szDisabled )
      CreateDisabledString( vDialog, szDisabled );

      //:// If there is a Default button for the window, we must add some characters
      //:// to the end of the input statement.
      //:szKeyPressCode = ""
      ZeidonStringCopy( szKeyPressCode, 1, 0, "", 1, 0, 51 );
      //:CreateViewFromView( vDialogRoot, vDialog )
      CreateViewFromView( &vDialogRoot, vDialog );
      //:nRC = ResetViewFromSubobject( vDialogRoot )
      nRC = ResetViewFromSubobject( vDialogRoot );
      //:LOOP WHILE nRC = 0
      while ( nRC == 0 )
      { 
         //:nRC = ResetViewFromSubobject( vDialogRoot )
         nRC = ResetViewFromSubobject( vDialogRoot );
      } 

      //:END
      //:IF vDialogRoot.Window.DfltButton != ""
      if ( CompareAttributeToString( vDialogRoot, "Window", "DfltButton", "" ) != 0 )
      { 
         //:szKeyPressCode = szKeyPressCode + " onkeypress=^return _OnEnter( event )^"
         ZeidonStringConcat( szKeyPressCode, 1, 0, " onkeypress=^return _OnEnter( event )^", 1, 0, 51 );
      } 

      //:END      


      //:// Multiple class names must be separated by white space characters.
      //:szClass = vDialog.Control.CSS_Class
      GetVariableFromAttribute( szClass, 0, 'S', 257, vDialog, "Control", "CSS_Class", "", 0 );

      //:szWriteBuffer = "<span " + szStyle + ">"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "<span ", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szStyle, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, ">", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      //:IF szClass = ""
      if ( ZeidonStringCompare( szClass, 1, 0, "", 1, 0, 257 ) == 0 )
      { 
         //:szWriteBuffer = "   <input name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> " + szDisabled +
         //:                    szActionCode + szStyleCalendar + " type=^text^ value=^<%=strErrorMapValue%>^ " + szTabIndex + szKeyPressCode + " />"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <input name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyleCalendar, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " type=^text^ value=^<%=strErrorMapValue%>^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTabIndex, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szKeyPressCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " />", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "   <input class=^" + szClass + "^ name=^" + szCtrlTag + szRepeatGrpKey + "^ id=^" + szCtrlTag + szRepeatGrpKey + "^ <%=strDisabled%> " + szDisabled +
         //:                    szActionCode + szStyleCalendar + " type=^text^ value=^<%=strErrorMapValue%>^ " + szTabIndex + szKeyPressCode + " />"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <input class=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szClass, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ name=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szDisabled, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szActionCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szStyleCalendar, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " type=^text^ value=^<%=strErrorMapValue%>^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTabIndex, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szKeyPressCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " />", 1, 0, 10001 );
      } 

      //:END
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:// KJS 03/13/18 - If the view is read only, then we want to set the combo box to disabled.
      //:// We only do this if the zeidon.ini has UseZeidonControlActionsView
      //:IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y" 
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesControlActionsView", "Y" ) == 0 )
      { 
         //:szWriteBuffer = "   <% if (strDisabled == ^^) { %>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <% if (strDisabled == ^^) { %>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END
      //:szWriteBuffer = "   <img src=^images/scw.gif^  name=^" + szCtrlTag + "Img" + szRepeatGrpKey + "^ id=^" + szCtrlTag + "Img" + szRepeatGrpKey + "^ <%=strDisabled%> title=^Select Date^ alt=^Select Date^"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "   <img src=^images/scw.gif^  name=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "Img", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ id=^", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "Img", 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
      ZeidonStringConcat( szWriteBuffer, 1, 0, "^ <%=strDisabled%> title=^Select Date^ alt=^Select Date^", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:// KJS 03/16/16 - Added scwShowWFormat to scw.js file so that if we have a format on the calendar, then we use that. If no context, I am just calling scwShow, even
      //:// though I could call scwShowWFormat with zDateFormat... which would do that same thing. This is compatible for any jsp pages that are older and don't use a context.
      //:IF vDialog.CtrlMapContext EXISTS
      lTempInteger_5 = CheckExistenceOfEntity( vDialog, "CtrlMapContext" );
      if ( lTempInteger_5 == 0 )
      { 
         //:szWriteBuffer = "        onclick=^" + szCloseUpCode + "scwShowWFormat( document.getElementById( '" + szCtrlTag + szRepeatGrpKey + "' ), this, '" + vDialog.CtrlMapContext.Name + "' );^ " + szTabIndex + " />"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "        onclick=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCloseUpCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "scwShowWFormat( document.getElementById( '", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "' ), this, '", 1, 0, 10001 );
         GetVariableFromAttribute( szTempString_14, 0, 'S', 33, vDialog, "CtrlMapContext", "Name", "", 0 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTempString_14, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "' );^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTabIndex, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " />", 1, 0, 10001 );
         //:ELSE
      } 
      else
      { 
         //:szWriteBuffer = "        onclick=^" + szCloseUpCode + "scwShow( document.getElementById( '" + szCtrlTag + szRepeatGrpKey + "' ), this );^ " + szTabIndex + " />"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "        onclick=^", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCloseUpCode, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "scwShow( document.getElementById( '", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szCtrlTag, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szRepeatGrpKey, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, "' ), this );^ ", 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, szTabIndex, 1, 0, 10001 );
         ZeidonStringConcat( szWriteBuffer, 1, 0, " />", 1, 0, 10001 );
      } 

      //:END
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );

      //:IF vDialogRoot.Dialog.wWebUsesControlActionsView = "Y" 
      if ( CompareAttributeToString( vDialogRoot, "Dialog", "wWebUsesControlActionsView", "Y" ) == 0 )
      { 
         //:szWriteBuffer = "   <% } %>"
         ZeidonStringCopy( szWriteBuffer, 1, 0, "   <% } %>", 1, 0, 10001 );
         //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 )
         WL_QC( vDialog, lFile, szWriteBuffer, "^", 0 );
      } 

      //:END

      //:szWriteBuffer = "</span>"
      ZeidonStringCopy( szWriteBuffer, 1, 0, "</span>", 1, 0, 10001 );
      //:WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 )
      WL_QC( vDialog, lFile, szWriteBuffer, "^", 1 );
   } 


   //:END
   return;
// END
} 


 
#ifdef __cplusplus
}
#endif
