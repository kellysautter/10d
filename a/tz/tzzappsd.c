#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
TZZAPPSD_MainInit( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
TZZAPPSD_SelectApp( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
TZZAPPSD_MainExit( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
TZZAPPSD_SaveZeidonApp( zVIEW     vSubtask );


//:DIALOG OPERATION
//:TZZAPPSD_MainInit( VIEW vSubtask )

//:   STRING(128) szZeidon
zOPER_EXPORT zSHORT OPERATION
TZZAPPSD_MainInit( zVIEW     vSubtask )
{
   zCHAR     szZeidon[ 129 ] = { 0 }; 
   //:STRING(128) szZeidonApp
   zCHAR     szZeidonApp[ 129 ] = { 0 }; 
   //:VIEW        vZApp  BASED ON LOD KZAPPLOO
   zVIEW     vZApp = 0; 
   //:INTEGER     nRC
   zLONG     nRC = 0; 


   //:// get the path for ZEIDON.APP
   //:nRC = SysGetEnvVar( szZeidon /* sReturn Env */,
   //:                    "ZEIDON" /* szEnvVarName */,
   //:                    128 /* MaxLength */ )
   nRC = SysGetEnvVar( szZeidon, "ZEIDON", 128 );
   //:IF nRC != 0
   if ( nRC != 0 )
   { 
      //:MessageSend( vSubtask,
      //:             "AM00001", "Application Maintenance",
      //:             "Environment Variable ZEIDON is not set!",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 1)
      MessageSend( vSubtask, "AM00001", "Application Maintenance", "Environment Variable ZEIDON is not set!", zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
      //:SetWindowActionBehavior( vSubtask,
      //:                         zWAB_ExitDialogTask,
      //:                         0, 0)
      SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// add the filename to the path
   //:SysAppendcDirSep( szZeidon )
   SysAppendcDirSep( szZeidon );
   //:szZeidonApp = szZeidon + "ZEIDON.APP"
   ZeidonStringCopy( szZeidonApp, 1, 0, szZeidon, 1, 0, 129 );
   ZeidonStringConcat( szZeidonApp, 1, 0, "ZEIDON.APP", 1, 0, 129 );

   //:// open it
   //:nRC = ActivateOI_FromFile( vZApp,
   //:                           "KZAPPLOO",
   //:                           vSubtask,
   //:                           szZeidonApp,
   //:                           zMULTIPLE )
   nRC = ActivateOI_FromFile( &vZApp, "KZAPPLOO", vSubtask, szZeidonApp, zMULTIPLE );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( vSubtask,
      //:             "AM00002", "Application Maintenance",
      //:             "Could not activate ZEIDON.APP !",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 1)
      MessageSend( vSubtask, "AM00002", "Application Maintenance", "Could not activate ZEIDON.APP !", zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
      //:SetWindowActionBehavior( vSubtask,
      //:                         zWAB_ExitDialogTask,
      //:                         0, 0)
      SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:// name it
   //:NAME VIEW vZApp "KZAPPLOO"
   SetNameForView( vZApp, "KZAPPLOO", 0, zLEVEL_TASK );

   //:SetWindowCaptionTitle( vSubtask,
   //:                       "Zeidon Admin",
   //:                       szZeidonApp )
   SetWindowCaptionTitle( vSubtask, "Zeidon Admin", szZeidonApp );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:TZZAPPSD_SelectApp( VIEW vSubtask )

//:   VIEW vZApp REGISTERED AS KZAPPLOO
zOPER_EXPORT zSHORT OPERATION
TZZAPPSD_SelectApp( zVIEW     vSubtask )
{
   zVIEW     vZApp = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &vZApp, "KZAPPLOO", vSubtask, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:TZZAPPSD_MainExit( VIEW vSubtask )

//:   VIEW        vZApp REGISTERED AS KZAPPLOO
zOPER_EXPORT zSHORT OPERATION
TZZAPPSD_MainExit( zVIEW     vSubtask )
{
   zVIEW     vZApp = 0; 
   zSHORT    RESULT; 
   //:STRING(80)  szMsg
   zCHAR     szMsg[ 81 ] = { 0 }; 
   //:INTEGER     nRC
   zLONG     nRC = 0; 

   RESULT = GetViewByName( &vZApp, "KZAPPLOO", vSubtask, zLEVEL_TASK );

   //:nRC = ObjectInstanceUpdated( vZApp )
   nRC = ObjectInstanceUpdated( vZApp );
   //:IF nRC = 1
   if ( nRC == 1 )
   { 
      //:szMsg = "ZEIDON.APP has changed" + NEW_LINE +
      //:       "Do you want to save it?"
      ZeidonStringCopy( szMsg, 1, 0, "ZEIDON.APP has changed", 1, 0, 81 );
      ZeidonStringConcat( szMsg, 1, 0, NEW_LINE, 1, 0, 81 );
      ZeidonStringConcat( szMsg, 1, 0, "Do you want to save it?", 1, 0, 81 );
      //:nRC = MessagePrompt( vSubtask,
      //:              "AM00004", "Appl Admin",
      //:              szMsg,
      //:              0,
      //:              zBUTTONS_YESNOCANCEL,
      //:              zRESPONSE_YES,
      //:              0 )
      nRC = MessagePrompt( vSubtask, "AM00004", "Appl Admin", szMsg, 0, zBUTTONS_YESNOCANCEL, zRESPONSE_YES, 0 );
      //:IF nRC = zRESPONSE_YES
      if ( nRC == zRESPONSE_YES )
      { 
         //:TZZAPPSD_SaveZeidonApp( vSubtask )
         TZZAPPSD_SaveZeidonApp( vSubtask );
      } 

      //:END
      //:IF nRC = zRESPONSE_CANCEL
      if ( nRC == zRESPONSE_CANCEL )
      { 
         //:SetWindowActionBehavior( vSubtask,
         //:                         zWAB_StayOnWindow,
         //:                         0, 0)
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:TZZAPPSD_SaveZeidonApp( VIEW vSubtask )

//:   STRING(128) szZeidon
zOPER_EXPORT zSHORT OPERATION
TZZAPPSD_SaveZeidonApp( zVIEW     vSubtask )
{
   zCHAR     szZeidon[ 129 ] = { 0 }; 
   //:STRING(128) szZeidonApp
   zCHAR     szZeidonApp[ 129 ] = { 0 }; 
   //:VIEW        vZApp REGISTERED AS KZAPPLOO
   zVIEW     vZApp = 0; 
   zSHORT    RESULT; 
   //:INTEGER     nRC
   zLONG     nRC = 0; 

   RESULT = GetViewByName( &vZApp, "KZAPPLOO", vSubtask, zLEVEL_TASK );

   //:// get the path for ZEIDON.APP
   //:nRC = SysGetEnvVar( szZeidon /* sReturn Env */,
   //:                    "ZEIDON" /* szEnvVarName */,
   //:                    128 /* MaxLength */ )
   nRC = SysGetEnvVar( szZeidon, "ZEIDON", 128 );

   //:// add the filename to the path
   //:SysAppendcDirSep( szZeidon )
   SysAppendcDirSep( szZeidon );
   //:szZeidonApp = szZeidon + "ZEIDON.APP"
   ZeidonStringCopy( szZeidonApp, 1, 0, szZeidon, 1, 0, 129 );
   ZeidonStringConcat( szZeidonApp, 1, 0, "ZEIDON.APP", 1, 0, 129 );

   //:// save it
   //:nRC = CommitOI_ToFile( vZApp,
   //:                       szZeidonApp,
   //:                       zASCII )
   nRC = CommitOI_ToFile( vZApp, szZeidonApp, zASCII );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:MessageSend( vSubtask,
      //:             "AM00003", "Application Maintenance",
      //:             "Could not save ZEIDON.APP !",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 1)
      MessageSend( vSubtask, "AM00003", "Application Maintenance", "Could not save ZEIDON.APP !", zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
      //:RETURN -1
      return( -1 );
   } 

   //:END
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
