#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
zwTZCMREVO_DeleteReviewType( zVIEW     vSubtask );


static zSHORT
o_zwTZCMREVO_DeleteEmptyInstances( zVIEW     vView,
                                   zPCHAR    szEntityName,
                                   zPCHAR    szAttributeName,
                                   zPCHAR    szErrorText,
                                   zPLONG    nColumn );


zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_CreateReviewType( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_PreBuild( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_CommitReviewType( zVIEW     vSubtask );


static zSHORT
o_zwTZCMREVO_CheckUniqueInteger( zVIEW     vView,
                                 zPCHAR    szEntityName,
                                 zPCHAR    szAttributeName,
                                 zPCHAR    szIntegerValue );


zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_DisableAutoSort( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_CreateSubEvent( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
zwTZCMREVO_DeleteSubEvent( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_AcceptSubEvent( zVIEW     vSubtask );


static zSHORT
o_zwTZCMREVD_IsSysadm( zVIEW     vSubtask );


//:DIALOG OPERATION
//:zwTZCMREVO_DeleteReviewType( VIEW vSubtask )

//:   STRING (150) szMsg
zOPER_EXPORT zSHORT OPERATION
zwTZCMREVO_DeleteReviewType( zVIEW     vSubtask )
{
   zCHAR     szMsg[ 151 ] = { 0 }; 
   //:STRING ( 32) szType
   zCHAR     szType[ 33 ] = { 0 }; 
   //:INTEGER      nRC
   zLONG     nRC = 0; 

   //:VIEW TZCMREVO REGISTERED AS TZCMREVO
   zVIEW     TZCMREVO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZCMREVO, "TZCMREVO", vSubtask, zLEVEL_TASK );

   //:SetSelectStateOfEntity( TZCMREVO, "RevEvent", TRUE )
   SetSelectStateOfEntity( TZCMREVO, "RevEvent", TRUE );
   //:RefreshCtrl( vSubtask, "tgReviewType" )
   RefreshCtrl( vSubtask, "tgReviewType" );

   //:nRC = MessagePrompt( vSubtask, "CM0001", "Configuration Management",
   //:                     "OK to delete selected Review Events?",
   //:                     0, zBUTTONS_YESNO, zRESPONSE_NO, 0 )
   nRC = MessagePrompt( vSubtask, "CM0001", "Configuration Management", "OK to delete selected Review Events?", 0, zBUTTONS_YESNO, zRESPONSE_NO, 0 );

   //:IF nRC = zRESPONSE_YES
   if ( nRC == zRESPONSE_YES )
   { 
      //:nRC = SetCursorFirstSelectedEntity( TZCMREVO, "RevEvent", "" )
      nRC = SetCursorFirstSelectedEntity( TZCMREVO, "RevEvent", "" );

      //:LOOP WHILE nRC >= zCURSOR_SET
      while ( nRC >= zCURSOR_SET )
      { 
         //:  IF TZCMREVO.AuditTrail EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TZCMREVO, "AuditTrail" );
         if ( lTempInteger_0 == 0 )
         { 
            //:  szType = TZCMREVO.RevEvent.Type
            GetVariableFromAttribute( szType, 0, 'S', 33, TZCMREVO, "RevEvent", "Type", "", 0 );
            //:  szMsg = "Review Event '" + szType  + "' cannot be deleted " + zNEW_LINE +
            //:          "if it is used by an AuditTrail."
            ZeidonStringCopy( szMsg, 1, 0, "Review Event '", 1, 0, 151 );
            ZeidonStringConcat( szMsg, 1, 0, szType, 1, 0, 151 );
            ZeidonStringConcat( szMsg, 1, 0, "' cannot be deleted ", 1, 0, 151 );
            ZeidonStringConcat( szMsg, 1, 0, zNEW_LINE, 1, 0, 151 );
            ZeidonStringConcat( szMsg, 1, 0, "if it is used by an AuditTrail.", 1, 0, 151 );
            //:  MessageSend( vSubtask, "CM0003", "Configuration Management",
            //:               szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 )
            MessageSend( vSubtask, "CM0003", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
            //:  SetSelectStateOfEntity( TZCMREVO, "RevEvent", FALSE )
            SetSelectStateOfEntity( TZCMREVO, "RevEvent", FALSE );
            //:ELSE
         } 
         else
         { 
            //:  DeleteEntity( TZCMREVO, "RevEvent", zREPOS_NONE )
            DeleteEntity( TZCMREVO, "RevEvent", zREPOS_NONE );
         } 

         //:  END
         //:  nRC = SetCursorNextSelectedEntity( TZCMREVO, "RevEvent", "" )
         nRC = SetCursorNextSelectedEntity( TZCMREVO, "RevEvent", "" );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:zwTZCMREVO_DeleteEmptyInstances( VIEW vView, STRING ( 32 ) szEntityName, STRING ( 32 ) szAttributeName,
//:                                 STRING ( 32 ) szErrorText, INTEGER nColumn )

//:    STRING (254)  szValue
static zSHORT
o_zwTZCMREVO_DeleteEmptyInstances( zVIEW     vView,
                                   zPCHAR    szEntityName,
                                   zPCHAR    szAttributeName,
                                   zPCHAR    szErrorText,
                                   zPLONG    nColumn )
{
   zCHAR     szValue[ 255 ] = { 0 }; 
   //: STRING (254)  szDesc
   zCHAR     szDesc[ 255 ] = { 0 }; 
   //: INTEGER       nRC
   zLONG     nRC = 0; 


   //: nRC = SetCursorFirstEntity( vView, szEntityName, "" )
   nRC = SetCursorFirstEntity( vView, szEntityName, "" );

   //: LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 
      //:   GetStringFromAttribute( szValue, vView, szEntityName, szAttributeName )
      GetStringFromAttribute( szValue, zsizeof( szValue ), vView, szEntityName, szAttributeName );
      //:   GetStringFromAttribute( szDesc, vView, szEntityName, "Desc" )
      GetStringFromAttribute( szDesc, zsizeof( szDesc ), vView, szEntityName, "Desc" );

      //:   IF szValue = "" AND szDesc = ""
      if ( ZeidonStringCompare( szValue, 1, 0, "", 1, 0, 255 ) == 0 && ZeidonStringCompare( szDesc, 1, 0, "", 1, 0, 255 ) == 0 )
      { 
         //:   DeleteEntity( vView, szEntityName, zREPOS_NONE )
         DeleteEntity( vView, szEntityName, zREPOS_NONE );
         //:ELSE
      } 
      else
      { 
         //:   IF szValue = "" AND szDesc != ""
         if ( ZeidonStringCompare( szValue, 1, 0, "", 1, 0, 255 ) == 0 && ZeidonStringCompare( szDesc, 1, 0, "", 1, 0, 255 ) != 0 )
         { 
            //:   szErrorText = "Type"
            ZeidonStringCopy( szErrorText, 1, 0, "Type", 1, 0, 33 );
            //:   nColumn = 0
            *nColumn = 0;
            //:   RETURN -1
            return( -1 );
            //:ELSE
         } 
         else
         { 
            //:   IF szValue != "" AND szDesc = ""
            if ( ZeidonStringCompare( szValue, 1, 0, "", 1, 0, 255 ) != 0 && ZeidonStringCompare( szDesc, 1, 0, "", 1, 0, 255 ) == 0 )
            { 
               //:   szErrorText = "Desc"
               ZeidonStringCopy( szErrorText, 1, 0, "Desc", 1, 0, 33 );
               //:   nColumn = 1
               *nColumn = 1;
               //:   RETURN -1
               return( -1 );
            } 

            //:   END
         } 

         //:   END
      } 

      //:   END

      //:  nRC = SetCursorNextEntity( vView, szEntityName, "" )
      nRC = SetCursorNextEntity( vView, szEntityName, "" );
   } 

   //: END

   //: RETURN 0
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:zwTZCMREVD_CreateReviewType( VIEW vSubtask )

//:   VIEW TZCMREVO REGISTERED AS TZCMREVO
zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_CreateReviewType( zVIEW     vSubtask )
{
   zVIEW     TZCMREVO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZCMREVO, "TZCMREVO", vSubtask, zLEVEL_TASK );

   //:CREATE ENTITY TZCMREVO.RevEvent LAST
   RESULT = CreateEntity( TZCMREVO, "RevEvent", zPOS_LAST );

   //:RefreshWindow( vSubtask )
   RefreshWindow( vSubtask );
   //:TG_SetActiveCell( vSubtask, "tgReviewType", -1, 0 )
   TG_SetActiveCell( vSubtask, "tgReviewType", -1, 0 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:zwTZCMREVD_PreBuild( VIEW vSubtask )

//:   INTEGER  lRepo_ViewCluster
zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_PreBuild( zVIEW     vSubtask )
{
   zLONG     lRepo_ViewCluster = 0; 

   //:VIEW TZCMREPO BASED ON LOD TZCMREPO
   zVIEW     TZCMREPO = 0; 
   //:VIEW TZCMREVO BASED ON LOD TZCMREVO
   zVIEW     TZCMREVO = 0; 
   zSHORT    RESULT; 


   //:GET VIEW TZCMREPO NAMED "TZCMREPO"
   RESULT = GetViewByName( &TZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   //:GET VIEW TZCMREVO NAMED "TZCMREVO"
   RESULT = GetViewByName( &TZCMREVO, "TZCMREVO", vSubtask, zLEVEL_TASK );

   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:ACTIVATE TZCMREVO MULTIPLE
      RESULT = ActivateObjectInstance( &TZCMREVO, "TZCMREVO", vSubtask, 0, zMULTIPLE );
      //:NAME VIEW TZCMREVO "TZCMREVO"
      SetNameForView( TZCMREVO, "TZCMREVO", 0, zLEVEL_TASK );

      //:// Get Repo View Cluster and add TZCMREVO
      //:lRepo_ViewCluster = TZCMREPO.Installation.Repo_ViewCluster
      GetIntegerFromAttribute( &lRepo_ViewCluster, TZCMREPO, "Installation", "Repo_ViewCluster" );
      //:AddToViewCluster( lRepo_ViewCluster, TZCMREVO, 0 )
      AddToViewCluster( lRepo_ViewCluster, TZCMREVO, 0 );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:zwTZCMREVD_CommitReviewType( VIEW vSubtask )

//:   STRING ( 32 )  szErrorText
zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_CommitReviewType( zVIEW     vSubtask )
{
   zCHAR     szErrorText[ 33 ] = { 0 }; 
   //:STRING ( 16 )  szIntegerValue
   zCHAR     szIntegerValue[ 17 ] = { 0 }; 
   //:STRING ( 60 )  szMsg
   zCHAR     szMsg[ 61 ] = { 0 }; 
   //:INTEGER        nColumn
   zLONG     nColumn = 0; 

   //:VIEW TZCMREPO BASED ON LOD TZCMREPO
   zVIEW     TZCMREPO = 0; 
   //:VIEW TZCMREVO REGISTERED AS TZCMREVO
   zVIEW     TZCMREVO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 

   RESULT = GetViewByName( &TZCMREVO, "TZCMREVO", vSubtask, zLEVEL_TASK );

   //:GET VIEW TZCMREPO NAMED "TZCMREPO"
   RESULT = GetViewByName( &TZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   //:nColumn = 0
   nColumn = 0;

   //:IF zwTZCMREVD_IsSysadm( vSubtask ) = 0
   lTempInteger_0 = o_zwTZCMREVD_IsSysadm( vSubtask );
   if ( lTempInteger_0 == 0 )
   { 
      //:RETURN 0
      return( 0 );
   } 

   //:END

   //:IF zwTZCMREVO_DeleteEmptyInstances( TZCMREVO, "RevEvent", "Type", szErrorText, nColumn ) < 0
   lTempInteger_1 = o_zwTZCMREVO_DeleteEmptyInstances( TZCMREVO, "RevEvent", "Type", szErrorText, &nColumn );
   if ( lTempInteger_1 < 0 )
   { 
      //:szMsg = "Event " + szErrorText + " is required."
      ZeidonStringCopy( szMsg, 1, 0, "Event ", 1, 0, 61 );
      ZeidonStringConcat( szMsg, 1, 0, szErrorText, 1, 0, 61 );
      ZeidonStringConcat( szMsg, 1, 0, " is required.", 1, 0, 61 );
      //:MessageSend( vSubtask, "CM0001", "Configuration Management", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 )
      MessageSend( vSubtask, "CM0001", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RefreshCtrl( vSubtask, "tgReviewType" )
      RefreshCtrl( vSubtask, "tgReviewType" );
      //:TG_SetActiveCell( vSubtask, "tgReviewType", -1, nColumn )
      TG_SetActiveCell( vSubtask, "tgReviewType", -1, nColumn );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:szIntegerValue = ""
   ZeidonStringCopy( szIntegerValue, 1, 0, "", 1, 0, 17 );
   //:IF zwTZCMREVO_CheckUniqueInteger( TZCMREVO, "RevEvent", "Type", szIntegerValue ) < 0
   lTempInteger_2 = o_zwTZCMREVO_CheckUniqueInteger( TZCMREVO, "RevEvent", "Type", szIntegerValue );
   if ( lTempInteger_2 < 0 )
   { 
      //:szMsg = "Event Type '" + szIntegerValue + "' is not unique."
      ZeidonStringCopy( szMsg, 1, 0, "Event Type '", 1, 0, 61 );
      ZeidonStringConcat( szMsg, 1, 0, szIntegerValue, 1, 0, 61 );
      ZeidonStringConcat( szMsg, 1, 0, "' is not unique.", 1, 0, 61 );
      //:MessageSend( vSubtask, "CM0002", "Configuration Management", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 )
      MessageSend( vSubtask, "CM0002", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RefreshCtrl( vSubtask, "tgReviewType" )
      RefreshCtrl( vSubtask, "tgReviewType" );
      //:TG_SetActiveCell( vSubtask, "tgReviewType", -1, 0 )
      TG_SetActiveCell( vSubtask, "tgReviewType", -1, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:IF ObjectInstanceUpdated( TZCMREVO ) = 1
   lTempInteger_3 = ObjectInstanceUpdated( TZCMREVO );
   if ( lTempInteger_3 == 1 )
   { 
      //:TZCMREPO.Installation.UpdateAuditTrail = 1
      SetAttributeFromInteger( TZCMREPO, "Installation", "UpdateAuditTrail", 1 );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:zwTZCMREVO_CheckUniqueInteger( VIEW vView, STRING ( 32 ) szEntityName, STRING ( 32 ) szAttributeName,
//:                               STRING ( 16 ) szIntegerValue )

//:    INTEGER nValue
static zSHORT
o_zwTZCMREVO_CheckUniqueInteger( zVIEW     vView,
                                 zPCHAR    szEntityName,
                                 zPCHAR    szAttributeName,
                                 zPCHAR    szIntegerValue )
{
   zLONG     nValue = 0; 
   //: INTEGER nRC
   zLONG     nRC = 0; 
   //: VIEW    vReviewView
   zVIEW     vReviewView = 0; 
   //: VIEW    vReviewView1
   zVIEW     vReviewView1 = 0; 


   //: CreateViewFromViewForTask( vReviewView, vView, vView )
   CreateViewFromViewForTask( &vReviewView, vView, vView );
   //: CreateViewFromViewForTask( vReviewView1, vView, vView )
   CreateViewFromViewForTask( &vReviewView1, vView, vView );

   //: SetAllSelectStatesForEntity( vView, szEntityName, FALSE, "" )
   SetAllSelectStatesForEntity( vView, szEntityName, FALSE, "" );

   //: nRC = SetCursorFirstEntity( vReviewView, szEntityName, "" )
   nRC = SetCursorFirstEntity( vReviewView, szEntityName, "" );

   //: LOOP WHILE nRC >= zCURSOR_SET
   while ( nRC >= zCURSOR_SET )
   { 
      //:   GetIntegerFromAttribute( nValue, vReviewView, szEntityName, szAttributeName )
      GetIntegerFromAttribute( &nValue, vReviewView, szEntityName, szAttributeName );

      //:   nRC = SetCursorFirstEntityByInteger( vReviewView1, szEntityName, szAttributeName, nValue, "" )
      nRC = SetCursorFirstEntityByInteger( vReviewView1, szEntityName, szAttributeName, nValue, "" );
      //:   nRC = SetCursorNextEntityByInteger( vReviewView1, szEntityName, szAttributeName, nValue, "" )
      nRC = SetCursorNextEntityByInteger( vReviewView1, szEntityName, szAttributeName, nValue, "" );

      //:   IF nRC >= zCURSOR_SET
      if ( nRC >= zCURSOR_SET )
      { 
         //:   SetCursorFirstEntityByInteger( vView, szEntityName, szAttributeName, nValue, "" )
         SetCursorFirstEntityByInteger( vView, szEntityName, szAttributeName, nValue, "" );
         //:   SetCursorNextEntityByInteger( vView, szEntityName, szAttributeName, nValue, "" )
         SetCursorNextEntityByInteger( vView, szEntityName, szAttributeName, nValue, "" );
         //:   szIntegerValue = nValue
         ZeidonStringConvertFromNumber( szIntegerValue, 1, 0, 16, nValue, (ZDecimal) 0.0, "I" );
         //:   DropView( vReviewView )
         DropView( vReviewView );
         //:   DropView( vReviewView1 )
         DropView( vReviewView1 );
         //:   RETURN -1
         return( -1 );
      } 

      //:   END

      //:   nRC = SetCursorNextEntity( vReviewView, szEntityName, "" )
      nRC = SetCursorNextEntity( vReviewView, szEntityName, "" );
   } 

   //: END

   //: DropView( vReviewView )
   DropView( vReviewView );
   //: DropView( vReviewView1 )
   DropView( vReviewView1 );

   //: RETURN 0
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:zwTZCMREVD_DisableAutoSort( VIEW vSubtask )

//:    INTEGER bSysadm
zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_DisableAutoSort( zVIEW     vSubtask )
{
   zLONG     bSysadm = 0; 
   //: SHORT bLockColumn
   zSHORT    bLockColumn = 0; 
   zSHORT    lTempInteger_0; 


   //: TG_EnableAutosort( vSubtask, "tgReviewType", FALSE )
   TG_EnableAutosort( vSubtask, "tgReviewType", FALSE );

   //: IF zwTZCMREVD_IsSysadm( vSubtask ) = 1
   lTempInteger_0 = o_zwTZCMREVD_IsSysadm( vSubtask );
   if ( lTempInteger_0 == 1 )
   { 
      //: bSysadm     = TRUE
      bSysadm = TRUE;
      //: bLockColumn = FALSE
      bLockColumn = FALSE;
      //:ELSE
   } 
   else
   { 
      //: bSysadm     = FALSE
      bSysadm = FALSE;
      //: bLockColumn = TRUE
      bLockColumn = TRUE;
   } 

   //: END

   //: SetCtrlState( vSubtask, "pbNew", zCONTROL_STATUS_ENABLED, bSysadm )
   SetCtrlState( vSubtask, "pbNew", zCONTROL_STATUS_ENABLED, bSysadm );
   //: SetCtrlState( vSubtask, "pbDelete", zCONTROL_STATUS_ENABLED, bSysadm )
   SetCtrlState( vSubtask, "pbDelete", zCONTROL_STATUS_ENABLED, bSysadm );

   //: TG_LockColumn( vSubtask, "tgReviewType", 0, bLockColumn )
   TG_LockColumn( vSubtask, "tgReviewType", 0, (zLONG) bLockColumn );
   //: TG_LockColumn( vSubtask, "tgReviewType", 1, bLockColumn )
   TG_LockColumn( vSubtask, "tgReviewType", 1, (zLONG) bLockColumn );
   //: TG_LockColumn( vSubtask, "tgReviewType", 2, bLockColumn )
   TG_LockColumn( vSubtask, "tgReviewType", 2, (zLONG) bLockColumn );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:zwTZCMREVD_CreateSubEvent( VIEW vSubtask )

//:   VIEW TZCMREVO REGISTERED AS TZCMREVO
zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_CreateSubEvent( zVIEW     vSubtask )
{
   zVIEW     TZCMREVO = 0; 
   zSHORT    RESULT; 

   RESULT = GetViewByName( &TZCMREVO, "TZCMREVO", vSubtask, zLEVEL_TASK );

   //:CREATE ENTITY TZCMREVO.RevSubEvent LAST
   RESULT = CreateEntity( TZCMREVO, "RevSubEvent", zPOS_LAST );

   //:RefreshWindow( vSubtask )
   RefreshWindow( vSubtask );
   //:TG_SetActiveCell( vSubtask, "tgReviewType", -1, 0 )
   TG_SetActiveCell( vSubtask, "tgReviewType", -1, 0 );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:zwTZCMREVO_DeleteSubEvent( VIEW vSubtask )

//:   STRING (150) szMsg
zOPER_EXPORT zSHORT OPERATION
zwTZCMREVO_DeleteSubEvent( zVIEW     vSubtask )
{
   zCHAR     szMsg[ 151 ] = { 0 }; 
   //:STRING ( 32) szSubType
   zCHAR     szSubType[ 33 ] = { 0 }; 
   //:INTEGER      nRC
   zLONG     nRC = 0; 

   //:VIEW TZCMREVO REGISTERED AS TZCMREVO
   zVIEW     TZCMREVO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 

   RESULT = GetViewByName( &TZCMREVO, "TZCMREVO", vSubtask, zLEVEL_TASK );

   //:SetSelectStateOfEntity( TZCMREVO, "RevSubEvent", TRUE )
   SetSelectStateOfEntity( TZCMREVO, "RevSubEvent", TRUE );
   //:RefreshCtrl( vSubtask, "tgReviewType" )
   RefreshCtrl( vSubtask, "tgReviewType" );

   //:nRC = MessagePrompt( vSubtask, "CM0001", "Configuration Management",
   //:                     "OK to delete selected SubEvents?",
   //:                     0, zBUTTONS_YESNO, zRESPONSE_NO, 0 )
   nRC = MessagePrompt( vSubtask, "CM0001", "Configuration Management", "OK to delete selected SubEvents?", 0, zBUTTONS_YESNO, zRESPONSE_NO, 0 );

   //:IF nRC = zRESPONSE_YES
   if ( nRC == zRESPONSE_YES )
   { 
      //:nRC = SetCursorFirstSelectedEntity( TZCMREVO, "RevSubEvent", "" )
      nRC = SetCursorFirstSelectedEntity( TZCMREVO, "RevSubEvent", "" );

      //:LOOP WHILE nRC >= zCURSOR_SET
      while ( nRC >= zCURSOR_SET )
      { 
         //:  IF TZCMREVO.SubAuditTrail EXISTS
         lTempInteger_0 = CheckExistenceOfEntity( TZCMREVO, "SubAuditTrail" );
         if ( lTempInteger_0 == 0 )
         { 
            //:  szSubType = TZCMREVO.RevSubEvent.Subtype
            GetVariableFromAttribute( szSubType, 0, 'S', 33, TZCMREVO, "RevSubEvent", "Subtype", "", 0 );
            //:  szMsg = "SubEvent '" + szSubType  + "' cannot be deleted " + zNEW_LINE +
            //:          "if it is used by an SubAuditTrail."
            ZeidonStringCopy( szMsg, 1, 0, "SubEvent '", 1, 0, 151 );
            ZeidonStringConcat( szMsg, 1, 0, szSubType, 1, 0, 151 );
            ZeidonStringConcat( szMsg, 1, 0, "' cannot be deleted ", 1, 0, 151 );
            ZeidonStringConcat( szMsg, 1, 0, zNEW_LINE, 1, 0, 151 );
            ZeidonStringConcat( szMsg, 1, 0, "if it is used by an SubAuditTrail.", 1, 0, 151 );
            //:  MessageSend( vSubtask, "CM0003", "Configuration Management",
            //:               szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 )
            MessageSend( vSubtask, "CM0003", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
            //:  SetSelectStateOfEntity( TZCMREVO, "RevSubEvent", FALSE )
            SetSelectStateOfEntity( TZCMREVO, "RevSubEvent", FALSE );
            //:ELSE
         } 
         else
         { 
            //:  DeleteEntity( TZCMREVO, "RevSubEvent", zREPOS_NONE )
            DeleteEntity( TZCMREVO, "RevSubEvent", zREPOS_NONE );
         } 

         //:  END
         //:  nRC = SetCursorNextSelectedEntity( TZCMREVO, "RevSubEvent", "" )
         nRC = SetCursorNextSelectedEntity( TZCMREVO, "RevSubEvent", "" );
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:zwTZCMREVD_AcceptSubEvent( VIEW vSubtask )

//:   STRING ( 32 )  szErrorText
zOPER_EXPORT zSHORT OPERATION
zwTZCMREVD_AcceptSubEvent( zVIEW     vSubtask )
{
   zCHAR     szErrorText[ 33 ] = { 0 }; 
   //:STRING ( 16 )  szIntegerValue
   zCHAR     szIntegerValue[ 17 ] = { 0 }; 
   //:STRING ( 60 )  szMsg
   zCHAR     szMsg[ 61 ] = { 0 }; 
   //:INTEGER        nColumn
   zLONG     nColumn = 0; 

   //:VIEW TZCMREVO REGISTERED AS TZCMREVO
   zVIEW     TZCMREVO = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zSHORT    lTempInteger_3; 

   RESULT = GetViewByName( &TZCMREVO, "TZCMREVO", vSubtask, zLEVEL_TASK );
   //:nColumn = 0
   nColumn = 0;

   //:IF zwTZCMREVD_IsSysadm( vSubtask ) = 0
   lTempInteger_0 = o_zwTZCMREVD_IsSysadm( vSubtask );
   if ( lTempInteger_0 == 0 )
   { 
      //:CancelSubobject( TZCMREVO, "RevEvent" )
      CancelSubobject( TZCMREVO, "RevEvent" );
      //:RETURN 0
      return( 0 );
   } 

   //:END

   //:IF zwTZCMREVO_DeleteEmptyInstances( TZCMREVO, "RevSubEvent", "Subtype", szErrorText, nColumn ) < 0
   lTempInteger_1 = o_zwTZCMREVO_DeleteEmptyInstances( TZCMREVO, "RevSubEvent", "Subtype", szErrorText, &nColumn );
   if ( lTempInteger_1 < 0 )
   { 
      //:szMsg = "SubEvent " + szErrorText + " is required."
      ZeidonStringCopy( szMsg, 1, 0, "SubEvent ", 1, 0, 61 );
      ZeidonStringConcat( szMsg, 1, 0, szErrorText, 1, 0, 61 );
      ZeidonStringConcat( szMsg, 1, 0, " is required.", 1, 0, 61 );
      //:MessageSend( vSubtask, "CM0001", "Configuration Management", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 )
      MessageSend( vSubtask, "CM0001", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RefreshCtrl( vSubtask, "tgReviewType" )
      RefreshCtrl( vSubtask, "tgReviewType" );
      //:TG_SetActiveCell( vSubtask, "tgReviewType", -1, nColumn )
      TG_SetActiveCell( vSubtask, "tgReviewType", -1, nColumn );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:szIntegerValue = ""
   ZeidonStringCopy( szIntegerValue, 1, 0, "", 1, 0, 17 );
   //:IF zwTZCMREVO_CheckUniqueInteger( TZCMREVO, "RevSubEvent", "Subtype", szIntegerValue ) < 0
   lTempInteger_2 = o_zwTZCMREVO_CheckUniqueInteger( TZCMREVO, "RevSubEvent", "Subtype", szIntegerValue );
   if ( lTempInteger_2 < 0 )
   { 
      //:szMsg = "Subtype '" + szIntegerValue + "' is not unique."
      ZeidonStringCopy( szMsg, 1, 0, "Subtype '", 1, 0, 61 );
      ZeidonStringConcat( szMsg, 1, 0, szIntegerValue, 1, 0, 61 );
      ZeidonStringConcat( szMsg, 1, 0, "' is not unique.", 1, 0, 61 );
      //:MessageSend( vSubtask, "CM0002", "Configuration Management", szMsg,
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 )
      MessageSend( vSubtask, "CM0002", "Configuration Management", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 1 );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:RefreshCtrl( vSubtask, "tgReviewType" )
      RefreshCtrl( vSubtask, "tgReviewType" );
      //:TG_SetActiveCell( vSubtask, "tgReviewType", -1, 0 )
      TG_SetActiveCell( vSubtask, "tgReviewType", -1, 0 );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:IF AcceptSubobject( TZCMREVO, "RevEvent" ) < 0
   lTempInteger_3 = AcceptSubobject( TZCMREVO, "RevEvent" );
   if ( lTempInteger_3 < 0 )
   { 
      //:RefreshCtrl( vSubtask, "tgReviewType" )
      RefreshCtrl( vSubtask, "tgReviewType" );
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:zwTZCMREVD_IsSysadm( VIEW vSubtask )

//:   VIEW vTZCMREPO BASED ON LOD TZCMREPO
static zSHORT
o_zwTZCMREVD_IsSysadm( zVIEW     vSubtask )
{
   zVIEW     vTZCMREPO = 0; 
   //:VIEW vTZCMWIPO BASED ON LOD TZCMWIPO
   zVIEW     vTZCMWIPO = 0; 

   //:SHORT nSysadm
   zSHORT    nSysadm = 0; 


   //:nSysadm = 0
   nSysadm = 0;

   //:GetViewByName( vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK )
   GetViewByName( &vTZCMREPO, "TZCMREPO", vSubtask, zLEVEL_TASK );
   //:GetViewByName( vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK )
   GetViewByName( &vTZCMWIPO, "TZCMWIPO", vSubtask, zLEVEL_TASK );

   //:IF vTZCMWIPO.ROOT.UserName = vTZCMREPO.Installation.Sysadm
   if ( CompareAttributeToAttribute( vTZCMWIPO, "ROOT", "UserName", vTZCMREPO, "Installation", "Sysadm" ) == 0 )
   { 
      //:nSysadm = 1
      nSysadm = 1;
   } 

   //:END

   //:RETURN nSysadm
   return( nSysadm );
// END
} 


 
#ifdef __cplusplus
}
#endif
