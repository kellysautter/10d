#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
Prebuild_Init( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
SystemClose_Init( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetLibFile_Delete( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetLibFile_New( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
GetCurrentPath( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
GetCurrentPathPostBuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
BuildCompilerEntity( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetPrebuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetObjFile_AddNew( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetLibFile_AddNew( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetObjFile_New( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetObjFile_Delete( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetRebuildTargetList( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetCreateList( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CopyCompilerSpec( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
RetrieveCurrentCompileSpec( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
RebuildTargetList( zVIEW     vSubtask );


static zSHORT
o_MoveEntity( zVIEW     vView,
              zPCHAR    szEntityName,
              zSHORT    nMovement );


zOPER_EXPORT zSHORT OPERATION
ExternalTargetRefresh( zVIEW     vSubtask );


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
Prebuild_Init( zVIEW     vSubtask )
{

   //:Prebuild_Init( VIEW vSubtask )

   //:InitializeLPLR( vSubtask, "" )
   InitializeLPLR( vSubtask, "" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
SystemClose_Init( zVIEW     vSubtask )
{

   //:SystemClose_Init( VIEW vSubtask )

   //:TerminateLPLR( vSubtask )
   TerminateLPLR( vSubtask );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ExternalTargetLibFile_Delete( VIEW vSubtask )

//:   VIEW  vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
ExternalTargetLibFile_Delete( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:IF vTaskLPLR.ExternalLibFile EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vTaskLPLR, "ExternalLibFile" );
   if ( lTempInteger_0 == 0 )
   { 

      //:SetSelectStateOfEntity( vTaskLPLR, "ExternalLibFile", TRUE )
      SetSelectStateOfEntity( vTaskLPLR, "ExternalLibFile", TRUE );
      //:RefreshCtrl( vSubtask, "tgLibFiles" )
      RefreshCtrl( vSubtask, "tgLibFiles" );

      //:nRC = MessagePrompt( vSubtask, "ED1004", "Zeidon Compiler",
      //:                     "OK to delete selected External Lib Files?",
      //:                     0, zBUTTONS_YESNO, zRESPONSE_NO, 0 )
      nRC = MessagePrompt( vSubtask, "ED1004", "Zeidon Compiler", "OK to delete selected External Lib Files?", 0, zBUTTONS_YESNO, zRESPONSE_NO, 0 );

      //:IF nRC = zRESPONSE_YES
      if ( nRC == zRESPONSE_YES )
      { 

         //:nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "ExternalLibFile", "" )
         nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "ExternalLibFile", "" );

         //:LOOP WHILE nRC >= zCURSOR_SET
         while ( nRC >= zCURSOR_SET )
         { 

            //:  DeleteEntity( vTaskLPLR, "ExternalLibFile", zREPOS_NONE )
            DeleteEntity( vTaskLPLR, "ExternalLibFile", zREPOS_NONE );

            //:  nRC = SetCursorNextSelectedEntity( vTaskLPLR, "ExternalLibFile", "" )
            nRC = SetCursorNextSelectedEntity( vTaskLPLR, "ExternalLibFile", "" );
         } 

         //:END
      } 


      //:END

      //:TG_SetActiveCell( vSubtask, "tgLibFiles", -1, 0 )
      TG_SetActiveCell( vSubtask, "tgLibFiles", -1, 0 );
   } 


   //:END

   //:IF vTaskLPLR.ExternalObjFile DOES not EXIST AND vTaskLPLR.ExternalLibFile DOES not EXIST
   lTempInteger_1 = CheckExistenceOfEntity( vTaskLPLR, "ExternalObjFile" );
   lTempInteger_2 = CheckExistenceOfEntity( vTaskLPLR, "ExternalLibFile" );
   if ( lTempInteger_1 != 0 && lTempInteger_2 != 0 )
   { 
      //:vTaskLPLR.ExternalTarget.ExternalExists = "N"
      SetAttributeFromString( vTaskLPLR, "ExternalTarget", "ExternalExists", "N" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ExternalTargetLibFile_New( VIEW vSubtask )


//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
ExternalTargetLibFile_New( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:IF vTaskLPLR.ExternalLibFile EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vTaskLPLR, "ExternalLibFile" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetAllSelectStatesForEntity( vTaskLPLR, "ExternalLibFile", FALSE, "" )
      SetAllSelectStatesForEntity( vTaskLPLR, "ExternalLibFile", FALSE, "" );
   } 

   //:END

   //:CREATE ENTITY vTaskLPLR.ExternalLibFile LAST
   RESULT = CreateEntity( vTaskLPLR, "ExternalLibFile", zPOS_LAST );

   //:vTaskLPLR.ExternalTarget.ExternalExists = "Y"
   SetAttributeFromString( vTaskLPLR, "ExternalTarget", "ExternalExists", "Y" );

   //:RefreshWindowExceptForCtrl( vSubtask, "tgObjectFiles" )
   RefreshWindowExceptForCtrl( vSubtask, "tgObjectFiles" );
   //:TG_SetActiveCell( vSubtask, "tgLibFiles", -1, 0 )
   TG_SetActiveCell( vSubtask, "tgLibFiles", -1, 0 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GetCurrentPath( VIEW vSubtask )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
GetCurrentPath( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   //:STRING ( 1000 ) szPath
   zCHAR     szPath[ 1001 ] = { 0 }; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:SysGetEnvVar( szPath, "PATH", 1000 )
   SysGetEnvVar( szPath, "PATH", 1000 );
   //:vTaskLPLR.Compiler.Path = szPath
   SetAttributeFromString( vTaskLPLR, "Compiler", "Path", szPath );

   //:RefreshCtrl( vSubtask, "edPath" )
   RefreshCtrl( vSubtask, "edPath" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GetCurrentPathPostBuild( VIEW vSubtask )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
GetCurrentPathPostBuild( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //: IF vTaskLPLR.Compiler.Path = ""
   if ( CompareAttributeToString( vTaskLPLR, "Compiler", "Path", "" ) == 0 )
   { 
      //:  GetCurrentPath( vSubtask )
      GetCurrentPath( vSubtask );
   } 

   //: END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:BuildCompilerEntity( VIEW vSubtask )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
BuildCompilerEntity( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:IF vTaskLPLR.Compiler DOES NOT EXIST
   lTempInteger_0 = CheckExistenceOfEntity( vTaskLPLR, "Compiler" );
   if ( lTempInteger_0 != 0 )
   { 
      //:CREATE ENTITY vTaskLPLR.Compiler
      RESULT = CreateEntity( vTaskLPLR, "Compiler", zPOS_AFTER );
      //:vTaskLPLR.Compiler.Name = "AutoGen"
      SetAttributeFromString( vTaskLPLR, "Compiler", "Name", "AutoGen" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
ExternalTargetPrebuild( zVIEW     vSubtask )
{

   //:ExternalTargetPrebuild( VIEW vSubtask )

   //:// create the external target list
   //:ExternalTargetCreateList( vSubtask )
   ExternalTargetCreateList( vSubtask );

   //:ExternalTargetRefresh( vSubtask )
   ExternalTargetRefresh( vSubtask );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ExternalTargetObjFile_AddNew( VIEW vSubtask )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
ExternalTargetObjFile_AddNew( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:CREATE ENTITY vTaskLPLR.ExternalObjFile LAST
   RESULT = CreateEntity( vTaskLPLR, "ExternalObjFile", zPOS_LAST );

   //:vTaskLPLR.ExternalTarget.ExternalExists = "Y"
   SetAttributeFromString( vTaskLPLR, "ExternalTarget", "ExternalExists", "Y" );

   //:RefreshWindowExceptForCtrl( vSubtask, "tgObjectFiles" )
   RefreshWindowExceptForCtrl( vSubtask, "tgObjectFiles" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ExternalTargetLibFile_AddNew( VIEW vSubtask )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
ExternalTargetLibFile_AddNew( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:CREATE ENTITY vTaskLPLR.ExternalLibFile LAST
   RESULT = CreateEntity( vTaskLPLR, "ExternalLibFile", zPOS_LAST );

   //:vTaskLPLR.ExternalTarget.ExternalExists = "Y"
   SetAttributeFromString( vTaskLPLR, "ExternalTarget", "ExternalExists", "Y" );

   //:RefreshWindowExceptForCtrl( vSubtask, "tgLibFiles" )
   RefreshWindowExceptForCtrl( vSubtask, "tgLibFiles" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ExternalTargetObjFile_New( VIEW vSubtask )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
ExternalTargetObjFile_New( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:IF vTaskLPLR.ExternalObjFile EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vTaskLPLR, "ExternalObjFile" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SetAllSelectStatesForEntity( vTaskLPLR, "ExternalObjFile", FALSE, "" )
      SetAllSelectStatesForEntity( vTaskLPLR, "ExternalObjFile", FALSE, "" );
   } 

   //:END

   //:CREATE ENTITY vTaskLPLR.ExternalObjFile LAST
   RESULT = CreateEntity( vTaskLPLR, "ExternalObjFile", zPOS_LAST );

   //:vTaskLPLR.ExternalTarget.ExternalExists = "Y"
   SetAttributeFromString( vTaskLPLR, "ExternalTarget", "ExternalExists", "Y" );

   //:RefreshWindowExceptForCtrl( vSubtask, "tgLibFiles" )
   RefreshWindowExceptForCtrl( vSubtask, "tgLibFiles" );
   //:TG_SetActiveCell( vSubtask, "tgObjectFiles", -1, 0 )
   TG_SetActiveCell( vSubtask, "tgObjectFiles", -1, 0 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ExternalTargetObjFile_Delete( VIEW vSubtask )

//:   VIEW  vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
ExternalTargetObjFile_Delete( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:IF vTaskLPLR.ExternalObjFile EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vTaskLPLR, "ExternalObjFile" );
   if ( lTempInteger_0 == 0 )
   { 

      //:SetSelectStateOfEntity( vTaskLPLR, "ExternalObjFile", TRUE )
      SetSelectStateOfEntity( vTaskLPLR, "ExternalObjFile", TRUE );
      //:RefreshCtrl( vSubtask, "tgObjectFiles" )
      RefreshCtrl( vSubtask, "tgObjectFiles" );

      //:nRC = MessagePrompt( vSubtask, "ED1004", "Zeidon Compiler",
      //:                     "OK to delete selected External Object Files?",
      //:                     0, zBUTTONS_YESNO, zRESPONSE_NO, 0 )
      nRC = MessagePrompt( vSubtask, "ED1004", "Zeidon Compiler", "OK to delete selected External Object Files?", 0, zBUTTONS_YESNO, zRESPONSE_NO, 0 );

      //:IF nRC = zRESPONSE_YES
      if ( nRC == zRESPONSE_YES )
      { 

         //:nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "ExternalObjFile", "" )
         nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "ExternalObjFile", "" );

         //:LOOP WHILE nRC >= zCURSOR_SET
         while ( nRC >= zCURSOR_SET )
         { 

            //:  DeleteEntity( vTaskLPLR, "ExternalObjFile", zREPOS_NONE )
            DeleteEntity( vTaskLPLR, "ExternalObjFile", zREPOS_NONE );

            //:  nRC = SetCursorNextSelectedEntity( vTaskLPLR, "ExternalObjFile", "" )
            nRC = SetCursorNextSelectedEntity( vTaskLPLR, "ExternalObjFile", "" );
         } 

         //:END
      } 


      //:END

      //:TG_SetActiveCell( vSubtask, "tgObjectFiles", -1, 0 )
      TG_SetActiveCell( vSubtask, "tgObjectFiles", -1, 0 );
   } 


   //:END

   //:IF vTaskLPLR.ExternalObjFile DOES not EXIST AND vTaskLPLR.ExternalLibFile DOES not EXIST
   lTempInteger_1 = CheckExistenceOfEntity( vTaskLPLR, "ExternalObjFile" );
   lTempInteger_2 = CheckExistenceOfEntity( vTaskLPLR, "ExternalLibFile" );
   if ( lTempInteger_1 != 0 && lTempInteger_2 != 0 )
   { 
      //:vTaskLPLR.ExternalTarget.ExternalExists = "N"
      SetAttributeFromString( vTaskLPLR, "ExternalTarget", "ExternalExists", "N" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:ExternalTargetRebuildTargetList( VIEW vSubtask )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
ExternalTargetRebuildTargetList( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:// Rebuild Executable Target list
   //:RebuildTargetList( vSubtask )
   RebuildTargetList( vSubtask );

   //:// Create External Target list from Executable Target list
   //:vTaskLPLR.Compiler.CreateExternalList = 0
   SetAttributeFromInteger( vTaskLPLR, "Compiler", "CreateExternalList", 0 );
   //:ExternalTargetCreateList( vSubtask )
   ExternalTargetCreateList( vSubtask );

   //:ExternalTargetRefresh( vSubtask )
   ExternalTargetRefresh( vSubtask );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:ExternalTargetCreateList( VIEW vSubtask )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
ExternalTargetCreateList( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:IF vTaskLPLR.Compiler.CreateExternalList = 1
   if ( CompareAttributeToInteger( vTaskLPLR, "Compiler", "CreateExternalList", 1 ) == 0 )
   { 
      //:OrderEntityForView( vTaskLPLR, "ExternalTarget", "Name" )
      OrderEntityForView( vTaskLPLR, "ExternalTarget", "Name" );
      //:SetSelectStateOfEntity( vTaskLPLR, "ExternalTarget", TRUE )
      SetSelectStateOfEntity( vTaskLPLR, "ExternalTarget", TRUE );
      //:RETURN
      return( 0 );
   } 

   //:END

   //:// The external target list is a list of all execution targets
   //:// plus the entry "<ALL>", which means that these externals
   //:// belong to all execution targets.
   //:// The LPLR contains normally only these external targets, for
   //:// which are external libs or objs defined. The others are only
   //:// temporal entries for the external target dialog.

   //:// Delete old Targets, which no more exist
   //:FOR EACH vTaskLPLR.ExternalTarget
   RESULT = SetCursorFirstEntity( vTaskLPLR, "ExternalTarget", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //: // Check, if ExternalTarget still exists as TargetExecutable
      //: SET CURSOR FIRST vTaskLPLR.TargetExecutable
      //:            WHERE vTaskLPLR.ExternalTarget.Name = vTaskLPLR.TargetExecutable.Name
      RESULT = SetCursorFirstEntity( vTaskLPLR, "TargetExecutable", "" );
      if ( RESULT > zCURSOR_UNCHANGED )
      { 
         while ( RESULT > zCURSOR_UNCHANGED && ( CompareAttributeToAttribute( vTaskLPLR, "ExternalTarget", "Name", vTaskLPLR, "TargetExecutable", "Name" ) != 0 ) )
         { 
            RESULT = SetCursorNextEntity( vTaskLPLR, "TargetExecutable", "" );
         } 

      } 

      //: // if not, delete it from ExternalTarget, too
      //: IF RESULT != zCURSOR_SET AND vTaskLPLR.ExternalTarget.Name != "<ALL>"
      if ( RESULT != zCURSOR_SET && CompareAttributeToString( vTaskLPLR, "ExternalTarget", "Name", "<ALL>" ) != 0 )
      { 
         //: DELETE ENTITY vTaskLPLR.ExternalTarget NONE
         RESULT = DeleteEntity( vTaskLPLR, "ExternalTarget", zREPOS_NONE );
      } 

      RESULT = SetCursorNextEntity( vTaskLPLR, "ExternalTarget", "" );
      //: END
   } 

   //:END

   //:// Create the target "<ALL>"
   //:// This is for externals which belong to all targets
   //:SET CURSOR FIRST vTaskLPLR.ExternalTarget
   //:           WHERE vTaskLPLR.ExternalTarget.Name = "<ALL>"
   RESULT = SetCursorFirstEntityByString( vTaskLPLR, "ExternalTarget", "Name", "<ALL>", "" );
   //:IF RESULT != zCURSOR_SET
   if ( RESULT != zCURSOR_SET )
   { 
      //:CREATE ENTITY vTaskLPLR.ExternalTarget
      RESULT = CreateEntity( vTaskLPLR, "ExternalTarget", zPOS_AFTER );
      //:vTaskLPLR.ExternalTarget.Name = "<ALL>"
      SetAttributeFromString( vTaskLPLR, "ExternalTarget", "Name", "<ALL>" );
   } 

   //:END

   //:// Copy the target lists after the "<ALL>" target
   //:FOR EACH vTaskLPLR.TargetExecutable
   RESULT = SetCursorFirstEntity( vTaskLPLR, "TargetExecutable", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //: // Copy only, if not existing
      //: SET CURSOR FIRST vTaskLPLR.ExternalTarget
      //:            WHERE vTaskLPLR.ExternalTarget.Name = vTaskLPLR.TargetExecutable.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTaskLPLR, "TargetExecutable", "Name" );
      RESULT = SetCursorFirstEntityByString( vTaskLPLR, "ExternalTarget", "Name", szTempString_0, "" );
      //: IF RESULT != zCURSOR_SET
      if ( RESULT != zCURSOR_SET )
      { 
         //: CREATE ENTITY vTaskLPLR.ExternalTarget
         RESULT = CreateEntity( vTaskLPLR, "ExternalTarget", zPOS_AFTER );
         //: vTaskLPLR.ExternalTarget.Name = vTaskLPLR.TargetExecutable.Name
         SetAttributeFromAttribute( vTaskLPLR, "ExternalTarget", "Name", vTaskLPLR, "TargetExecutable", "Name" );
         //:ELSE
      } 
      else
      { 
         //: IF vTaskLPLR.ExternalObjFile EXISTS OR vTaskLPLR.ExternalLibFile EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( vTaskLPLR, "ExternalObjFile" );
         lTempInteger_1 = CheckExistenceOfEntity( vTaskLPLR, "ExternalLibFile" );
         if ( lTempInteger_0 == 0 || lTempInteger_1 == 0 )
         { 
            //: vTaskLPLR.ExternalTarget.ExternalExists = "Y"
            SetAttributeFromString( vTaskLPLR, "ExternalTarget", "ExternalExists", "Y" );
         } 

         //: END
      } 

      RESULT = SetCursorNextEntity( vTaskLPLR, "TargetExecutable", "" );
      //: END
   } 

   //:END

   //:// Sort the list
   //:SetAllSelectStatesForEntity( vTaskLPLR, "ExternalTarget", FALSE, "Compiler" )
   SetAllSelectStatesForEntity( vTaskLPLR, "ExternalTarget", FALSE, "Compiler" );
   //:OrderEntityForView( vTaskLPLR, "ExternalTarget", "Name" )
   OrderEntityForView( vTaskLPLR, "ExternalTarget", "Name" );
   //:SET CURSOR FIRST vTaskLPLR.ExternalTarget
   RESULT = SetCursorFirstEntity( vTaskLPLR, "ExternalTarget", "" );
   //:SetSelectStateOfEntity( vTaskLPLR, "ExternalTarget", TRUE )
   SetSelectStateOfEntity( vTaskLPLR, "ExternalTarget", TRUE );
   //:vTaskLPLR.Compiler.CreateExternalList = 1
   SetAttributeFromInteger( vTaskLPLR, "Compiler", "CreateExternalList", 1 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CopyCompilerSpec( VIEW vSubtask )

//:   STRING ( 32 )  szName
zOPER_EXPORT zSHORT OPERATION
CopyCompilerSpec( zVIEW     vSubtask )
{
   zCHAR     szName[ 33 ] = { 0 }; 
   //:STRING ( 10 )  szNumber
   zCHAR     szNumber[ 11 ] = { 0 }; 
   //:INTEGER        nNumber
   zLONG     nNumber = 0; 

   //:VIEW vTaskLPLR REGISTERED AS TaskLPLR
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW vTemp     BASED ON LOD  TZCMLPLO
   zVIEW     vTemp = 0; 
   //:VIEW vTempName BASED ON LOD  TZCMLPLO
   zVIEW     vTempName = 0; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:CreateViewFromViewForTask( vTemp, vTaskLPLR, vSubtask )
   CreateViewFromViewForTask( &vTemp, vTaskLPLR, vSubtask );
   //:CreateViewFromViewForTask( vTempName, vTaskLPLR, vSubtask )
   CreateViewFromViewForTask( &vTempName, vTaskLPLR, vSubtask );

   //:CREATE ENTITY vTaskLPLR.Compiler LAST
   RESULT = CreateEntity( vTaskLPLR, "Compiler", zPOS_LAST );
   //:SetMatchingAttributesByName( vTaskLPLR, "Compiler",
   //:                             vTemp, "Compiler", zSET_ALL )
   SetMatchingAttributesByName( vTaskLPLR, "Compiler", vTemp, "Compiler", zSET_ALL );
   //:// set unique Name
   //:nNumber = 0
   nNumber = 0;
   //:szName  = "New"
   ZeidonStringCopy( szName, 1, 0, "New", 1, 0, 33 );
   //:SET CURSOR FIRST vTempName.Compiler
   //:           WHERE vTempName.Compiler.Name = szName
   RESULT = SetCursorFirstEntityByString( vTempName, "Compiler", "Name", szName, "" );

   //:LOOP WHILE RESULT >= zCURSOR_SET
   while ( RESULT >= zCURSOR_SET )
   { 
      //:  nNumber = nNumber + 1
      nNumber = nNumber + 1;
      //:  zltoa( nNumber, szNumber )
      zltoa( nNumber, szNumber, sizeof( szNumber ) );
      //:  szName = "New" + szNumber
      ZeidonStringCopy( szName, 1, 0, "New", 1, 0, 33 );
      ZeidonStringConcat( szName, 1, 0, szNumber, 1, 0, 33 );
      //:  SET CURSOR FIRST vTempName.Compiler
      //:             WHERE vTempName.Compiler.Name = szName
      RESULT = SetCursorFirstEntityByString( vTempName, "Compiler", "Name", szName, "" );
   } 

   //:END
   //:DropView( vTempName )
   DropView( vTempName );

   //:vTaskLPLR.Compiler.Name = szName
   SetAttributeFromString( vTaskLPLR, "Compiler", "Name", szName );

   //:FOR EACH vTemp.Include
   RESULT = SetCursorFirstEntity( vTemp, "Include", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY vTaskLPLR.Include
      RESULT = CreateEntity( vTaskLPLR, "Include", zPOS_AFTER );
      //:SetMatchingAttributesByName( vTaskLPLR, "Include",
      //:                             vTemp, "Include", zSET_ALL )
      SetMatchingAttributesByName( vTaskLPLR, "Include", vTemp, "Include", zSET_ALL );
      RESULT = SetCursorNextEntity( vTemp, "Include", "" );
   } 

   //:END

   //:FOR EACH vTemp.Lib
   RESULT = SetCursorFirstEntity( vTemp, "Lib", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY vTaskLPLR.Lib
      RESULT = CreateEntity( vTaskLPLR, "Lib", zPOS_AFTER );
      //:SetMatchingAttributesByName( vTaskLPLR, "Lib",
      //:                             vTemp, "Lib", zSET_ALL )
      SetMatchingAttributesByName( vTaskLPLR, "Lib", vTemp, "Lib", zSET_ALL );
      RESULT = SetCursorNextEntity( vTemp, "Lib", "" );
   } 

   //:END

   //:FOR EACH vTemp.ExternalTarget
   RESULT = SetCursorFirstEntity( vTemp, "ExternalTarget", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CREATE ENTITY vTaskLPLR.ExternalTarget
      RESULT = CreateEntity( vTaskLPLR, "ExternalTarget", zPOS_AFTER );
      //:SetMatchingAttributesByName( vTaskLPLR, "ExternalTarget",
      //:                             vTemp, "ExternalTarget", zSET_ALL )
      SetMatchingAttributesByName( vTaskLPLR, "ExternalTarget", vTemp, "ExternalTarget", zSET_ALL );

      //:FOR EACH vTemp.ExternalObjFile
      RESULT = SetCursorFirstEntity( vTemp, "ExternalObjFile", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CREATE ENTITY vTaskLPLR.ExternalObjFile
         RESULT = CreateEntity( vTaskLPLR, "ExternalObjFile", zPOS_AFTER );
         //:SetMatchingAttributesByName( vTaskLPLR, "ExternalObjFile",
         //:                            vTemp, "ExternalObjFile", zSET_ALL )
         SetMatchingAttributesByName( vTaskLPLR, "ExternalObjFile", vTemp, "ExternalObjFile", zSET_ALL );
         RESULT = SetCursorNextEntity( vTemp, "ExternalObjFile", "" );
      } 

      //:END

      //:FOR EACH vTemp.ExternalLibFile
      RESULT = SetCursorFirstEntity( vTemp, "ExternalLibFile", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:CREATE ENTITY vTaskLPLR.ExternalLibFile
         RESULT = CreateEntity( vTaskLPLR, "ExternalLibFile", zPOS_AFTER );
         //:SetMatchingAttributesByName( vTaskLPLR, "ExternalLibFile",
         //:                            vTemp, "ExternalLibFile", zSET_ALL )
         SetMatchingAttributesByName( vTaskLPLR, "ExternalLibFile", vTemp, "ExternalLibFile", zSET_ALL );
         RESULT = SetCursorNextEntity( vTemp, "ExternalLibFile", "" );
      } 

      RESULT = SetCursorNextEntity( vTemp, "ExternalTarget", "" );
      //:END
   } 

   //:END

   //:DropView( vTemp )
   DropView( vTemp );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:RetrieveCurrentCompileSpec( VIEW vSubtask )

//:   STRING ( 50 )  szControlText
zOPER_EXPORT zSHORT OPERATION
RetrieveCurrentCompileSpec( zVIEW     vSubtask )
{
   zCHAR     szControlText[ 51 ] = { 0 }; 
   //:STRING ( 32 )  szName
   zCHAR     szName[ 33 ] = { 0 }; 

   //:VIEW vTaskLPLR     REGISTERED AS TaskLPLR
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW vCompilerSpec BASED ON LOD  TZCMLPLO
   zVIEW     vCompilerSpec = 0; 
   zCHAR     szTempString_0[ 33 ]; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:CreateViewFromViewForTask( vCompilerSpec, vTaskLPLR, vSubtask )
   CreateViewFromViewForTask( &vCompilerSpec, vTaskLPLR, vSubtask );
   //:NAME VIEW vCompilerSpec "CompilerSpec"
   SetNameForView( vCompilerSpec, "CompilerSpec", 0, zLEVEL_TASK );

   //:SET CURSOR FIRST vTaskLPLR.Compiler
   //:    WHERE vTaskLPLR.Compiler.Name = vTaskLPLR.LPLR.CurrentCompilerSpec
   GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTaskLPLR, "LPLR", "CurrentCompilerSpec" );
   RESULT = SetCursorFirstEntityByString( vTaskLPLR, "Compiler", "Name", szTempString_0, "" );

   //:szName = vTaskLPLR.LPLR.Name
   GetVariableFromAttribute( szName, 0, 'S', 33, vTaskLPLR, "LPLR", "Name", "", 0 );
   //:szControlText = " LPLR: " + szName
   ZeidonStringCopy( szControlText, 1, 0, " LPLR: ", 1, 0, 51 );
   ZeidonStringConcat( szControlText, 1, 0, szName, 1, 0, 51 );
   //:SetCtrlText( vSubtask, "txtLPLR", szControlText )
   SetCtrlText( vSubtask, "txtLPLR", szControlText );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:RebuildTargetList( VIEW vSubtask )
//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
RebuildTargetList( zVIEW     vSubtask )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW vMetaList BASED ON LOD TZCMLPLO
   zVIEW     vMetaList = 0; 
   //:VIEW vGOG_Meta BASED ON LOD TZOGSRCO
   zVIEW     vGOG_Meta = 0; 
   //:VIEW vDLG_Meta BASED ON LOD TZWDLGSO
   zVIEW     vDLG_Meta = 0; 
   //:VIEW vLOD_Meta BASED ON LOD TZZOLODO
   zVIEW     vLOD_Meta = 0; 
   //:VIEW vDomainMeta BASED ON LOD TZDGSRCO
   zVIEW     vDomainMeta = 0; 
   //:STRING ( 513 ) szName                    // zMAX_FILESPEC_LTH+1
   zCHAR     szName[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szDLL_Name                // zMAX_FILESPEC_LTH+1
   zCHAR     szDLL_Name[ 514 ] = { 0 }; 
   //:STRING ( 513 ) szMsg                     // zMAX_MESSAGE_LTH+1
   zCHAR     szMsg[ 514 ] = { 0 }; 
   //:INTEGER nRC
   zLONG     nRC = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   //:// Delete all the TargetExecutable entities.
   //:FOR EACH vTaskLPLR.TargetExecutable
   RESULT = SetCursorFirstEntity( vTaskLPLR, "TargetExecutable", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: DELETE ENTITY vTaskLPLR.TargetExecutable NONE
      RESULT = DeleteEntity( vTaskLPLR, "TargetExecutable", zREPOS_NONE );
      RESULT = SetCursorNextEntity( vTaskLPLR, "TargetExecutable", "" );
   } 

   //:END

   //:// Loop through each of the metas that have operations: Operation
   //:// groups, dialogs, objects, and domains and retrieve the name of
   //:// the target executable from each one.

   //:// Retrieve every Global Operation Group meta and copy every source file
   //:// reference to TargetExecutable.
   //:RetrieveViewForMetaList( vSubtask, vMetaList, zREFER_GOPGRP_META )
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_GOPGRP_META );
   //:FOR EACH vMetaList.W_MetaDef
   RESULT = SetCursorFirstEntity( vMetaList, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: szMsg = "Searching Operation Group " + vMetaList.W_MetaDef.Name
      GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vMetaList, "W_MetaDef", "Name", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "Searching Operation Group ", 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 514 );
      //: MB_SetMessage( vSubtask, 0, szMsg )
      MB_SetMessage( vSubtask, 0, szMsg );

      //: szName = vTaskLPLR.LPLR.MetaSrcDir + "\" + vMetaList.W_MetaDef.Name + ".POG"
      GetStringFromAttribute( szName, sizeof( szName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szName, 1, 0, "\\", 1, 0, 514 );
      GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vMetaList, "W_MetaDef", "Name", "", 0 );
      ZeidonStringConcat( szName, 1, 0, szTempString_1, 1, 0, 514 );
      ZeidonStringConcat( szName, 1, 0, ".POG", 1, 0, 514 );
      //: nRC = ActivateOI_FromFile( vGOG_Meta, "TZOGSRCO", vSubtask,
      //:                            szName, zACTIVATE_ROOTONLY + zIGNORE_ATTRIB_ERRORS )
      nRC = ActivateOI_FromFile( &vGOG_Meta, "TZOGSRCO", vSubtask, szName, zACTIVATE_ROOTONLY + zIGNORE_ATTRIB_ERRORS );

      //: IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //: szDLL_Name = vGOG_Meta.GlobalOperationGroup.DomainAndGlobalOpGroupName
         GetVariableFromAttribute( szDLL_Name, 0, 'S', 514, vGOG_Meta, "GlobalOperationGroup", "DomainAndGlobalOpGroupName", "", 0 );
         //: // KJS 02/25/15 - In the java world, we don't put values in the DLL_Name field, so 
         //: // use the object name in the list.
         //: IF szDLL_Name = ""
         if ( ZeidonStringCompare( szDLL_Name, 1, 0, "", 1, 0, 514 ) == 0 )
         { 
            //: szDLL_Name = vGOG_Meta.GlobalOperationGroup.Name   
            GetVariableFromAttribute( szDLL_Name, 0, 'S', 514, vGOG_Meta, "GlobalOperationGroup", "Name", "", 0 );
         } 

         //: END

         //: IF szDLL_Name != ""
         if ( ZeidonStringCompare( szDLL_Name, 1, 0, "", 1, 0, 514 ) != 0 )
         { 
            //: // Check to see if the DLL name of the GOPGRP is specified as a
            //: // TargetExecutable.  If not, create it.
            //: // NOTE: Assume that the DLL name for the group is the group name!!!
            //: SET CURSOR FIRST vTaskLPLR.TargetExecutable
            //:            WHERE vTaskLPLR.TargetExecutable.Name = szDLL_Name
            RESULT = SetCursorFirstEntityByString( vTaskLPLR, "TargetExecutable", "Name", szDLL_Name, "" );
            //: IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //: CREATE ENTITY vTaskLPLR.TargetExecutable LAST
               RESULT = CreateEntity( vTaskLPLR, "TargetExecutable", zPOS_LAST );
               //: vTaskLPLR.TargetExecutable.Name = szDLL_Name
               SetAttributeFromString( vTaskLPLR, "TargetExecutable", "Name", szDLL_Name );
            } 

            //: END

            //: // Add the GOP meta to the Target's list.
            //: CREATE ENTITY vTaskLPLR.Meta
            RESULT = CreateEntity( vTaskLPLR, "Meta", zPOS_AFTER );
            //: vTaskLPLR.Meta.Name = vGOG_Meta.GlobalOperationGroup.Name
            SetAttributeFromAttribute( vTaskLPLR, "Meta", "Name", vGOG_Meta, "GlobalOperationGroup", "Name" );
            //: vTaskLPLR.Meta.ZKey = vGOG_Meta.GlobalOperationGroup.ZKey
            SetAttributeFromAttribute( vTaskLPLR, "Meta", "ZKey", vGOG_Meta, "GlobalOperationGroup", "ZKey" );
            //: SetAttributeFromInteger( vTaskLPLR, "Meta", "MetaType", zREFER_GOPGRP_META )
            SetAttributeFromInteger( vTaskLPLR, "Meta", "MetaType", zREFER_GOPGRP_META );
         } 

         //: END

         //: DropObjectInstance( vGOG_Meta )
         DropObjectInstance( vGOG_Meta );
      } 

      RESULT = SetCursorNextEntity( vMetaList, "W_MetaDef", "" );
      //: END
   } 

   //:END

   //:DropView( vMetaList )
   DropView( vMetaList );

   //:// Retrieve every Dialog meta and copy every source file reference
   //:// to TargetExecutable.
   //:RetrieveViewForMetaList( vSubtask, vMetaList, zREFER_DIALOG_META )
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_DIALOG_META );
   //:FOR EACH vMetaList.W_MetaDef
   RESULT = SetCursorFirstEntity( vMetaList, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: szMsg = "Searching Dialog " + vMetaList.W_MetaDef.Name
      GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vMetaList, "W_MetaDef", "Name", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "Searching Dialog ", 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_2, 1, 0, 514 );
      //: MB_SetMessage( vSubtask, 0, szMsg )
      MB_SetMessage( vSubtask, 0, szMsg );

      //: szName = vTaskLPLR.LPLR.MetaSrcDir + "\" + vMetaList.W_MetaDef.Name + ".PWD"
      GetStringFromAttribute( szName, sizeof( szName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szName, 1, 0, "\\", 1, 0, 514 );
      GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vMetaList, "W_MetaDef", "Name", "", 0 );
      ZeidonStringConcat( szName, 1, 0, szTempString_3, 1, 0, 514 );
      ZeidonStringConcat( szName, 1, 0, ".PWD", 1, 0, 514 );
      //: nRC = ActivateOI_FromFile( vDLG_Meta, "TZWDLGSO", vSubtask,
      //:                            szName, zSINGLE + zIGNORE_ATTRIB_ERRORS )
      nRC = ActivateOI_FromFile( &vDLG_Meta, "TZWDLGSO", vSubtask, szName, zSINGLE + zIGNORE_ATTRIB_ERRORS );

      //: IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //: szDLL_Name = vDLG_Meta.Dialog.DLL_Name
         GetVariableFromAttribute( szDLL_Name, 0, 'S', 514, vDLG_Meta, "Dialog", "DLL_Name", "", 0 );
         //: // KJS 02/25/15 - In the java world, we don't put values in the DLL_Name field, so 
         //: // use the object name in the list.
         //: IF szDLL_Name = ""
         if ( ZeidonStringCompare( szDLL_Name, 1, 0, "", 1, 0, 514 ) == 0 )
         { 
            //: szDLL_Name = vDLG_Meta.Dialog.Tag  
            GetVariableFromAttribute( szDLL_Name, 0, 'S', 514, vDLG_Meta, "Dialog", "Tag", "", 0 );
         } 

         //: END

         //: // Show only dialogs with source in target list
         //: // these dialogs have an DLL name
         //: IF szDLL_Name != ""
         if ( ZeidonStringCompare( szDLL_Name, 1, 0, "", 1, 0, 514 ) != 0 )
         { 

            //:  SET CURSOR FIRST vTaskLPLR.TargetExecutable
            //:             WHERE vTaskLPLR.TargetExecutable.Name = szDLL_Name
            RESULT = SetCursorFirstEntityByString( vTaskLPLR, "TargetExecutable", "Name", szDLL_Name, "" );
            //:  IF RESULT < zCURSOR_SET
            if ( RESULT < zCURSOR_SET )
            { 
               //:  CREATE ENTITY vTaskLPLR.TargetExecutable LAST
               RESULT = CreateEntity( vTaskLPLR, "TargetExecutable", zPOS_LAST );
               //:  vTaskLPLR.TargetExecutable.Name = szDLL_Name
               SetAttributeFromString( vTaskLPLR, "TargetExecutable", "Name", szDLL_Name );
            } 

            //:  END

            //:  CREATE ENTITY vTaskLPLR.Meta
            RESULT = CreateEntity( vTaskLPLR, "Meta", zPOS_AFTER );
            //:  vTaskLPLR.Meta.Name = vDLG_Meta.Dialog.Tag
            SetAttributeFromAttribute( vTaskLPLR, "Meta", "Name", vDLG_Meta, "Dialog", "Tag" );
            //:  vTaskLPLR.Meta.ZKey = vDLG_Meta.Dialog.ZKey
            SetAttributeFromAttribute( vTaskLPLR, "Meta", "ZKey", vDLG_Meta, "Dialog", "ZKey" );
            //:  SetAttributeFromInteger( vTaskLPLR, "Meta", "MetaType", zREFER_DIALOG_META )
            SetAttributeFromInteger( vTaskLPLR, "Meta", "MetaType", zREFER_DIALOG_META );
         } 

         //: END

         //: DropObjectInstance( vDLG_Meta )
         DropObjectInstance( vDLG_Meta );
      } 

      RESULT = SetCursorNextEntity( vMetaList, "W_MetaDef", "" );
      //:END
   } 

   //:END

   //:DropView( vMetaList )
   DropView( vMetaList );

   //:// Retrieve every LOD meta and copy every source file reference
   //:// to TargetExecutable.
   //:RetrieveViewForMetaList( vSubtask, vMetaList, zREFER_LOD_META )
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_LOD_META );
   //:FOR EACH vMetaList.W_MetaDef
   RESULT = SetCursorFirstEntity( vMetaList, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: IF vMetaList.W_MetaDef.Name != "KZMSGQOO"
      if ( CompareAttributeToString( vMetaList, "W_MetaDef", "Name", "KZMSGQOO" ) != 0 )
      { 
         //: szMsg = "Searching Object Definition " + vMetaList.W_MetaDef.Name
         GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vMetaList, "W_MetaDef", "Name", "", 0 );
         ZeidonStringCopy( szMsg, 1, 0, "Searching Object Definition ", 1, 0, 514 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_4, 1, 0, 514 );
         //: MB_SetMessage( vSubtask, 0, szMsg )
         MB_SetMessage( vSubtask, 0, szMsg );

         //: szName = vTaskLPLR.LPLR.MetaSrcDir + "\" + vMetaList.W_MetaDef.Name + ".LOD"
         GetStringFromAttribute( szName, sizeof( szName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
         ZeidonStringConcat( szName, 1, 0, "\\", 1, 0, 514 );
         GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vMetaList, "W_MetaDef", "Name", "", 0 );
         ZeidonStringConcat( szName, 1, 0, szTempString_5, 1, 0, 514 );
         ZeidonStringConcat( szName, 1, 0, ".LOD", 1, 0, 514 );
         //: nRC = ActivateOI_FromFile( vLOD_Meta, "TZZOLODO", vSubtask,
         //:                            szName, zSINGLE + zIGNORE_ATTRIB_ERRORS )
         nRC = ActivateOI_FromFile( &vLOD_Meta, "TZZOLODO", vSubtask, szName, zSINGLE + zIGNORE_ATTRIB_ERRORS );

         //: // If DLL_Name isn't set, then assume that there is no target
         //: // executable for the LOD.
         //: IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //: szDLL_Name = vLOD_Meta.LOD.DLL_Name
            GetVariableFromAttribute( szDLL_Name, 0, 'S', 514, vLOD_Meta, "LOD", "DLL_Name", "", 0 );
            //:// KJS 02/25/15 - In the java world, we don't put values in the DLL_Name field, so 
            //:// use the object name in the list.
            //:IF szDLL_Name = ""
            if ( ZeidonStringCompare( szDLL_Name, 1, 0, "", 1, 0, 514 ) == 0 )
            { 
               //:szDLL_Name = vLOD_Meta.LOD.Name   
               GetVariableFromAttribute( szDLL_Name, 0, 'S', 514, vLOD_Meta, "LOD", "Name", "", 0 );
            } 

            //:END

            //: // Show only LODs with source in target list
            //: // these LODs have an DLL name
            //: IF szDLL_Name != ""
            if ( ZeidonStringCompare( szDLL_Name, 1, 0, "", 1, 0, 514 ) != 0 )
            { 
               //: SET CURSOR FIRST vLOD_Meta.Operation WITHIN vLOD_Meta.LOD
               RESULT = SetCursorFirstEntity( vLOD_Meta, "Operation", "LOD" );
               //: IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //: // Check to see if the DLL name of the LOD is specified as a
                  //: // TargetExecutable.  If not, create it.
                  //: SET CURSOR FIRST vTaskLPLR.TargetExecutable
                  //:            WHERE vTaskLPLR.TargetExecutable.Name = szDLL_Name
                  RESULT = SetCursorFirstEntityByString( vTaskLPLR, "TargetExecutable", "Name", szDLL_Name, "" );
                  //: IF RESULT < zCURSOR_SET
                  if ( RESULT < zCURSOR_SET )
                  { 
                     //: CREATE ENTITY vTaskLPLR.TargetExecutable LAST
                     RESULT = CreateEntity( vTaskLPLR, "TargetExecutable", zPOS_LAST );
                     //: vTaskLPLR.TargetExecutable.Name = szDLL_Name
                     SetAttributeFromString( vTaskLPLR, "TargetExecutable", "Name", szDLL_Name );
                  } 

                  //: END

                  //: CREATE ENTITY vTaskLPLR.Meta
                  RESULT = CreateEntity( vTaskLPLR, "Meta", zPOS_AFTER );
                  //: vTaskLPLR.Meta.Name = vLOD_Meta.LOD.Name
                  SetAttributeFromAttribute( vTaskLPLR, "Meta", "Name", vLOD_Meta, "LOD", "Name" );
                  //: vTaskLPLR.Meta.ZKey = vLOD_Meta.LOD.ZKey
                  SetAttributeFromAttribute( vTaskLPLR, "Meta", "ZKey", vLOD_Meta, "LOD", "ZKey" );
                  //: SetAttributeFromInteger( vTaskLPLR, "Meta", "MetaType", zREFER_LOD_META )
                  SetAttributeFromInteger( vTaskLPLR, "Meta", "MetaType", zREFER_LOD_META );
               } 

               //: END
            } 

            //: END

            //: DropObjectInstance( vLOD_Meta )
            DropObjectInstance( vLOD_Meta );
         } 

         //: END
      } 

      RESULT = SetCursorNextEntity( vMetaList, "W_MetaDef", "" );
      //: END
   } 

   //:END

   //:DropView( vMetaList )
   DropView( vMetaList );

   //:// Retrieve every Domain Group meta and copy every source file
   //:// reference to TargetExecutable.
   //:RetrieveViewForMetaList( vSubtask, vMetaList, zREFER_DOMAINGRP_META )
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_DOMAINGRP_META );
   //:FOR EACH vMetaList.W_MetaDef
   RESULT = SetCursorFirstEntity( vMetaList, "W_MetaDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: szMsg = "Searching Domain Group " + vMetaList.W_MetaDef.Name
      GetVariableFromAttribute( szTempString_6, 0, 'S', 33, vMetaList, "W_MetaDef", "Name", "", 0 );
      ZeidonStringCopy( szMsg, 1, 0, "Searching Domain Group ", 1, 0, 514 );
      ZeidonStringConcat( szMsg, 1, 0, szTempString_6, 1, 0, 514 );
      //: MB_SetMessage( vSubtask, 0, szMsg )
      MB_SetMessage( vSubtask, 0, szMsg );

      //: szName = vTaskLPLR.LPLR.MetaSrcDir + "\" + vMetaList.W_MetaDef.Name + ".PDG"
      GetStringFromAttribute( szName, sizeof( szName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szName, 1, 0, "\\", 1, 0, 514 );
      GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vMetaList, "W_MetaDef", "Name", "", 0 );
      ZeidonStringConcat( szName, 1, 0, szTempString_7, 1, 0, 514 );
      ZeidonStringConcat( szName, 1, 0, ".PDG", 1, 0, 514 );
      //: nRC = ActivateOI_FromFile( vDomainMeta, "TZDGSRCO", vSubtask,
      //:                            szName, zSINGLE + zIGNORE_ATTRIB_ERRORS )
      nRC = ActivateOI_FromFile( &vDomainMeta, "TZDGSRCO", vSubtask, szName, zSINGLE + zIGNORE_ATTRIB_ERRORS );

      //: IF nRC >= 0 AND vDomainMeta.Operation EXISTS
      lTempInteger_0 = CheckExistenceOfEntity( vDomainMeta, "Operation" );
      if ( nRC >= 0 && lTempInteger_0 == 0 )
      { 
         //: szDLL_Name = vDomainMeta.DomainGroup.DomainAndGlobalOpGroupName
         GetVariableFromAttribute( szDLL_Name, 0, 'S', 514, vDomainMeta, "DomainGroup", "DomainAndGlobalOpGroupName", "", 0 );

         //: IF szDLL_Name != ""
         if ( ZeidonStringCompare( szDLL_Name, 1, 0, "", 1, 0, 514 ) != 0 )
         { 
            //: IF szDLL_Name != "TZAPDMAA"
            if ( ZeidonStringCompare( szDLL_Name, 1, 0, "TZAPDMAA", 1, 0, 514 ) != 0 )
            { 
               //: // Check to see if the DLL name of the Domain is specified as a
               //: // TargetExecutable.  If not, create it.
               //: SET CURSOR FIRST vTaskLPLR.TargetExecutable
               //:            WHERE vTaskLPLR.TargetExecutable.Name = szDLL_Name
               RESULT = SetCursorFirstEntityByString( vTaskLPLR, "TargetExecutable", "Name", szDLL_Name, "" );
               //: IF RESULT < zCURSOR_SET
               if ( RESULT < zCURSOR_SET )
               { 
                  //: CREATE ENTITY vTaskLPLR.TargetExecutable LAST
                  RESULT = CreateEntity( vTaskLPLR, "TargetExecutable", zPOS_LAST );
                  //: vTaskLPLR.TargetExecutable.Name = szDLL_Name
                  SetAttributeFromString( vTaskLPLR, "TargetExecutable", "Name", szDLL_Name );
               } 

               //: END

               //: CREATE ENTITY vTaskLPLR.Meta
               RESULT = CreateEntity( vTaskLPLR, "Meta", zPOS_AFTER );
               //: vTaskLPLR.Meta.Name = vDomainMeta.DomainGroup.Name
               SetAttributeFromAttribute( vTaskLPLR, "Meta", "Name", vDomainMeta, "DomainGroup", "Name" );
               //: vTaskLPLR.Meta.ZKey = vDomainMeta.DomainGroup.ZKey
               SetAttributeFromAttribute( vTaskLPLR, "Meta", "ZKey", vDomainMeta, "DomainGroup", "ZKey" );
               //: SetAttributeFromInteger( vTaskLPLR, "Meta", "MetaType", zREFER_DOMAINGRP_META )
               SetAttributeFromInteger( vTaskLPLR, "Meta", "MetaType", zREFER_DOMAINGRP_META );
            } 

            //: END
         } 

         //: END
         //: DropObjectInstance( vDomainMeta )
         DropObjectInstance( vDomainMeta );
      } 

      RESULT = SetCursorNextEntity( vMetaList, "W_MetaDef", "" );
      //: END
   } 

   //:END

   //:DropView( vMetaList )
   DropView( vMetaList );

   //:OrderEntityForView( vTaskLPLR, "TargetExecutable", "Name A" )
   OrderEntityForView( vTaskLPLR, "TargetExecutable", "Name A" );
   //:MB_SetMessage( vSubtask, 0, "Done Building List" )
   MB_SetMessage( vSubtask, 0, "Done Building List" );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:MoveEntity( VIEW vView,
//:            STRING ( 32 ) szEntityName,
//:            SHORT nMovement )

//:   VIEW  vTemp
static zSHORT
o_MoveEntity( zVIEW     vView,
              zPCHAR    szEntityName,
              zSHORT    nMovement )
{
   zVIEW     vTemp = 0; 
   //:SHORT nRC
   zSHORT    nRC = 0; 
   zSHORT    lTempInteger_0; 


   //:// If the cursor is not set for the entity name,
   //:// then don't do anything.
   //:IF CheckExistenceOfEntity( vView, szEntityName ) != zCURSOR_SET
   lTempInteger_0 = CheckExistenceOfEntity( vView, szEntityName );
   if ( lTempInteger_0 != zCURSOR_SET )
   { 
      //:RETURN  0
      return( 0 );
   } 

   //:END

   //:CreateViewFromViewForTask( vTemp, vView, vView )
   CreateViewFromViewForTask( &vTemp, vView, vView );
   //:IF nMovement = 0
   if ( nMovement == 0 )
   { 
      //:// Move entity up.
      //:nRC = SetCursorPrevEntity( vView, szEntityName, 0 )
      nRC = SetCursorPrevEntity( vView, szEntityName, 0 );
      //:IF nRC = zCURSOR_UNCHANGED
      if ( nRC == zCURSOR_UNCHANGED )
      { 
         //:RETURN 0
         return( 0 );
      } 

      //:END

      //:CreateEntity( vView, szEntityName, zPOS_BEFORE )
      CreateEntity( vView, szEntityName, zPOS_BEFORE );
      //:SetMatchingAttributesByName( vView, szEntityName,
      //:                             vTemp, szEntityName, zSET_ALL )
      SetMatchingAttributesByName( vView, szEntityName, vTemp, szEntityName, zSET_ALL );

      //:DeleteEntity( vTemp, szEntityName, zREPOS_NONE )
      DeleteEntity( vTemp, szEntityName, zREPOS_NONE );
      //:DropView( vTemp )
      DropView( vTemp );
      //:SetSelectStateOfEntity( vView, szEntityName, 1 )
      SetSelectStateOfEntity( vView, szEntityName, 1 );
      //:ELSE
   } 
   else
   { 
      //:// Move entity down.
      //:nRC = SetCursorNextEntity( vView, szEntityName, 0 )
      nRC = SetCursorNextEntity( vView, szEntityName, 0 );
      //:IF nRC = zCURSOR_UNCHANGED
      if ( nRC == zCURSOR_UNCHANGED )
      { 
         //:RETURN 0
         return( 0 );
      } 

      //:END

      //:CreateEntity( vView, szEntityName, zPOS_AFTER )
      CreateEntity( vView, szEntityName, zPOS_AFTER );
      //:SetMatchingAttributesByName( vView, szEntityName,
      //:                             vTemp, szEntityName, zSET_ALL )
      SetMatchingAttributesByName( vView, szEntityName, vTemp, szEntityName, zSET_ALL );

      //:DeleteEntity( vTemp, szEntityName, zREPOS_NONE )
      DeleteEntity( vTemp, szEntityName, zREPOS_NONE );
      //:DropView( vTemp )
      DropView( vTemp );
      //:SetSelectStateOfEntity( vView, szEntityName, 1 )
      SetSelectStateOfEntity( vView, szEntityName, 1 );
   } 

   //:END

   //:RETURN 0
   return( 0 );
// END
} 


//:DIALOG OPERATION
zOPER_EXPORT zSHORT OPERATION
ExternalTargetRefresh( zVIEW     vSubtask )
{

   //:ExternalTargetRefresh( VIEW vSubtask )

   //:RefreshWindowExceptForCtrl( vSubtask, "tgTargetList" )
   RefreshWindowExceptForCtrl( vSubtask, "tgTargetList" );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
