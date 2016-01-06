#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zSHORT zwTZDMUPDD_RemoveOperFromGroup( zVIEW, zVIEW, zPCHAR );
zSHORT zwTZDMUPDD_CreateContext( zVIEW, zVIEW );
zSHORT zwTZDMUPDD_CopyOperationWorkLOD( zVIEW, zVIEW, zSHORT );
zSHORT zwfnTZDMUPDD_DeleteCode( zVIEW, zVIEW );
zSHORT zwfnTZDMUPDD_DeleteFromXLP( zVIEW, zULONG );
zSHORT zwfnTZDMUPDD_PromptForDelete( zVIEW, zVIEW, zSHORT, zSHORT );
zSHORT zwTZDMUPDD_SaveAsCheckValues( zVIEW, zPCHAR, zPCHAR );
zSHORT zwTZDMUPDD_CreateDeleteWorkLod( zVIEW, zVIEW, zPCHAR );
zSHORT zwTZDMUPDD_CreateOperation( zVIEW, zVIEW, zVIEW, zSHORT );
zSHORT zwTZDMUPDD_DeleteOperFromFile( zVIEW, zVIEW, zSHORT );
zSHORT zwTZDMUPDD_MoveDomainOperToFile( zVIEW*, zSHORT );
zSHORT zwTZDMUPDD_MoveCheckGroupType( zVIEW*, zVIEW, zSHORT );
zSHORT zwTZDMUPDD_CheckGroupType( zVIEW, zVIEW );
zSHORT zwTZDMUPDD_CheckGroupName( zVIEW, zVIEW, zVIEW );
zSHORT zwTZDMUPDD_LoadPopup( zVIEW, zPCHAR, zPCHAR, zPCHAR, zPCHAR );
zSHORT zwTZDMUPDD_CheckDomainGroupTyp( zVIEW, zVIEW );
zSHORT zwTZDMUPDD_SetNullEntry( zVIEW );
zSHORT zwTZDMUPDD_AcceptSubobject( zVIEW, zPCHAR, zPCHAR, zPCHAR );
zSHORT zwTZDMUPDD_SaveAsNewDomain( zVIEW*, zPCHAR, zPCHAR, zSHORT, zSHORT );
zSHORT zwfnTZDMUPDD_DeleteDomain( zVIEW*, zSHORT, zSHORT );
zSHORT zwTZDMUPDD_CreateDomainParameter( zVIEW );
zSHORT zwTZDMUPDD_TransferToEditor( zVIEW*, zSHORT );
zSHORT zwTZDMUPDD_CreateContextAndOper( zVIEW, zVIEW, zVIEW, zSHORT );

zOPER_EXPORT zSHORT OPERATION
NLS_DIL_DeleteText( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
GrayGroupControls( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NewDomainGroup( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NLS_CreateNLS_View( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NLS_DIL_NewText( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NLS_DIL_Refresh( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NLS_DIL_SaveText( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NLS_ExtValueDeleteText( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NLS_ExtValueNewText( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NLS_ExtValueRefresh( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
NLS_ExtValueSaveText( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
LoadProperties( zVIEW     vSubtask );


zOPER_EXPORT zSHORT OPERATION
CHANGE_JavaClassName( zVIEW     ViewToWindow );


//:DIALOG OPERATION
//:NLS_DIL_DeleteText( VIEW vSubtask )

//:   VIEW vDG BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
NLS_DIL_DeleteText( zVIEW     vSubtask )
{
   zVIEW     vDG = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:GET VIEW vDG NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );

   //:IF vDG.DIL_NLS_Text EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDG, "DIL_NLS_Text" );
   if ( lTempInteger_0 == 0 )
   { 
      //:DELETE ENTITY vDG.DIL_NLS_Text
      RESULT = DeleteEntity( vDG, "DIL_NLS_Text", zPOS_NEXT );
      //:NLS_DIL_Refresh( vSubtask )
      NLS_DIL_Refresh( vSubtask );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:GrayGroupControls( VIEW vSubtask )

//:   VIEW vProfileXFER      BASED ON LOD TZ__PRFO
zOPER_EXPORT zSHORT OPERATION
GrayGroupControls( zVIEW     vSubtask )
{
   zVIEW     vProfileXFER = 0; 
   //:VIEW vCM_ListGroup     BASED ON LOD TZCMLPLO
   zVIEW     vCM_ListGroup = 0; 
   //:VIEW vTZDGSRCO_DETAIL  BASED ON LOD TZDGSRCO
   zVIEW     vTZDGSRCO_DETAIL = 0; 

   //:SHORT  nEnable
   zSHORT    nEnable = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 


   //:GET VIEW vProfileXFER     NAMED "ProfileXFER"
   RESULT = GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   //:GET VIEW vCM_ListGroup    NAMED "CM_ListGroup"
   RESULT = GetViewByName( &vCM_ListGroup, "CM_ListGroup", vSubtask, zLEVEL_TASK );
   //:GET VIEW vTZDGSRCO_DETAIL NAMED "TZDGSRCO_DETAIL"
   RESULT = GetViewByName( &vTZDGSRCO_DETAIL, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

   //:// Gray the DomainGroup Name control, if this is not a new Domain Group
   //:// function.
   //:IF vProfileXFER.DM.NewDomainGroupFlag = "N" // new Domain
   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag", "N" ) == 0 )
   { 
      //:SetCtrlState( vSubtask, "DomainGroupName", zCONTROL_STATUS_ENABLED, TRUE )
      SetCtrlState( vSubtask, "DomainGroupName", zCONTROL_STATUS_ENABLED, TRUE );
      //:SetCtrlText( vSubtask, "OK", "Create" )
      SetCtrlText( vSubtask, "OK", "Create" );
      //:ELSE
   } 
   else
   { 
      //:IF vProfileXFER.DM.NewDomainGroupFlag = "X" // new Domain in new Group
      if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag", "X" ) == 0 )
      { 
         //:SetCtrlState( vSubtask, "DomainGroupName", zCONTROL_STATUS_ENABLED, TRUE )
         SetCtrlState( vSubtask, "DomainGroupName", zCONTROL_STATUS_ENABLED, TRUE );
         //:SetCtrlText( vSubtask, "OK", "Move" )
         SetCtrlText( vSubtask, "OK", "Move" );
         //:ELSE
      } 
      else
      { 
         //:IF vProfileXFER.DM.NewDomainGroupFlag = "S" // save as Domain in new Group
         if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag", "S" ) == 0 )
         { 
            //:SetCtrlState( vSubtask, "DomainGroupName", zCONTROL_STATUS_ENABLED, TRUE )
            SetCtrlState( vSubtask, "DomainGroupName", zCONTROL_STATUS_ENABLED, TRUE );
            //:SetCtrlText( vSubtask, "OK", "Save as" )
            SetCtrlText( vSubtask, "OK", "Save as" );
            //:ELSE
         } 
         else
         { 
            //:SetCtrlState( vSubtask, "DomainGroupName", zCONTROL_STATUS_ENABLED, FALSE )
            SetCtrlState( vSubtask, "DomainGroupName", zCONTROL_STATUS_ENABLED, FALSE );

            //:// set check out state in Title
            //:nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup )
            nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_ListGroup );

            //:IF vProfileXFER.DM.NewDomainGroupFlag = "P"  // load Group Properies for current Domain
            if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag", "P" ) == 0 )
            { 
               //:SET CURSOR FIRST vCM_ListGroup.W_MetaDef
               //:           WHERE vCM_ListGroup.W_MetaDef.Name = vTZDGSRCO_DETAIL.DomainGroup.Name
               GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vTZDGSRCO_DETAIL, "DomainGroup", "Name" );
               RESULT = SetCursorFirstEntityByString( vCM_ListGroup, "W_MetaDef", "Name", szTempString_0, "" );
               //:IF RESULT < zCURSOR_SET // new Domain Group
               if ( RESULT < zCURSOR_SET )
               { 
                  //:nEnable = 1
                  nEnable = 1;
                  //:SetWindowCaptionTitle( vSubtask, "", "Domain Group Properties" )
                  SetWindowCaptionTitle( vSubtask, "", "Domain Group Properties" );
               } 

               //:END
            } 

            //:END

            //:SetCtrlState( vSubtask, "edDescription", zCONTROL_STATUS_ENABLED, nEnable )
            SetCtrlState( vSubtask, "edDescription", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );
            //:SetCtrlState( vSubtask, "edExecutable", zCONTROL_STATUS_ENABLED, nEnable )
            SetCtrlState( vSubtask, "edExecutable", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );
            //:SetCtrlState( vSubtask, "rbC", zCONTROL_STATUS_ENABLED, nEnable )
            SetCtrlState( vSubtask, "rbC", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );
            //:SetCtrlState( vSubtask, "rbNone", zCONTROL_STATUS_ENABLED, nEnable )
            SetCtrlState( vSubtask, "rbNone", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );
            //:IF nEnable = 1
            if ( nEnable == 1 )
            { 
               //:SetFocusToCtrl( vSubtask, "edDescription" )
               SetFocusToCtrl( vSubtask, "edDescription" );
            } 

            //:END
         } 

         //:END
      } 

      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NewDomainGroup( VIEW vSubtask )

//:   VIEW vTZDGSRCO_DETAIL BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
NewDomainGroup( zVIEW     vSubtask )
{
   zVIEW     vTZDGSRCO_DETAIL = 0; 
   //:VIEW vProfileXFER     BASED ON LOD TZ__PRFO
   zVIEW     vProfileXFER = 0; 
   //:VIEW vCM_List         BASED ON LOD TZCMLPLO
   zVIEW     vCM_List = 0; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 


   //:GET VIEW vProfileXFER NAMED "ProfileXFER"
   RESULT = GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   //:GET VIEW vCM_List NAMED "CM_List"
   RESULT = GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );

   //:// Check that a domain name has been entered.
   //:IF vProfileXFER.DM.DomainName = ""
   if ( CompareAttributeToString( vProfileXFER, "DM", "DomainName", "" ) == 0 )
   { 
      //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      //:// Note same message as in TZDMUPDD.C
      //:MessageSend( vSubtask, "DM00103", "Domain Maintenance",
      //:             "Domain name required.",
      //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
      MessageSend( vSubtask, "DM00103", "Domain Maintenance", "Domain name required.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      //:SetFocusToCtrl( vSubtask, "edDomainName" )
      SetFocusToCtrl( vSubtask, "edDomainName" );
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //://BL, 1999.10.29 no check by move Domain in new Domain Group
   //:IF vProfileXFER.DM.NewDomainGroupFlag != "M"
   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag", "M" ) != 0 )
   { 
      //:// Check that the domain name is unique.
      //:SET CURSOR FIRST vCM_List.W_MetaDef
      //:           WHERE vCM_List.W_MetaDef.Name = vProfileXFER.DM.DomainName
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vProfileXFER, "DM", "DomainName" );
      RESULT = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name", szTempString_0, "" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 )
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         //:// Note same message as in TZDMUPDD.C
         //:MessageSend( vSubtask, "DM00104", "Domain Maintenance",
         //:             "A Domain already exists by that name.",
         //:             zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 )
         MessageSend( vSubtask, "DM00104", "Domain Maintenance", "A Domain already exists by that name.", zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         //:SetFocusToCtrl( vSubtask, "edDomainName" )
         SetFocusToCtrl( vSubtask, "edDomainName" );
         //:RETURN -1
         return( -1 );
      } 

      //:END
   } 

   //:END

   //:ActivateEmptyMetaOI( vSubtask, vTZDGSRCO_DETAIL, zSOURCE_DOMAINGRP_META, zSINGLE )
   ActivateEmptyMetaOI( vSubtask, &vTZDGSRCO_DETAIL, zSOURCE_DOMAINGRP_META, zSINGLE );
   //:CreateMetaEntity( vSubtask, vTZDGSRCO_DETAIL, "DomainGroup", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vTZDGSRCO_DETAIL, "DomainGroup", zPOS_AFTER );
   //:CreateMetaEntity( vSubtask, vTZDGSRCO_DETAIL, "Domain", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vTZDGSRCO_DETAIL, "Domain", zPOS_AFTER );

   //:NAME VIEW vTZDGSRCO_DETAIL "TZDGSRCO_DETAIL"
   SetNameForView( vTZDGSRCO_DETAIL, "TZDGSRCO_DETAIL", 0, zLEVEL_TASK );

   //:// if move Domain
   //:IF vProfileXFER.DM.NewDomainGroupFlag = "M"
   if ( CompareAttributeToString( vProfileXFER, "DM", "NewDomainGroupFlag", "M" ) == 0 )
   { 
      //:// Set flag so Domain Group Detail window will know this is a new
      //:// Domain Group function (move Domain in new Group).
      //:vProfileXFER.DM.NewDomainGroupFlag = "X"
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "X" );
      //:ELSE
   } 
   else
   { 
      //:vProfileXFER.DM.NewDomainGroupFlag = "N" // create new Domain
      SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "N" );
      //:vTZDGSRCO_DETAIL.Domain.Name = vProfileXFER.DM.DomainName
      SetAttributeFromAttribute( vTZDGSRCO_DETAIL, "Domain", "Name", vProfileXFER, "DM", "DomainName" );
      //:vTZDGSRCO_DETAIL.Domain.Desc = vProfileXFER.DM.DomainDesc
      SetAttributeFromAttribute( vTZDGSRCO_DETAIL, "Domain", "Desc", vProfileXFER, "DM", "DomainDesc" );
      //:CreateMetaEntity( vSubtask, vTZDGSRCO_DETAIL, "Context", zPOS_AFTER )
      CreateMetaEntity( vSubtask, vTZDGSRCO_DETAIL, "Context", zPOS_AFTER );
      //:vTZDGSRCO_DETAIL.Context.Name = vProfileXFER.DM.DomainName
      SetAttributeFromAttribute( vTZDGSRCO_DETAIL, "Context", "Name", vProfileXFER, "DM", "DomainName" );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NLS_CreateNLS_View( VIEW vSubtask )

//:   VIEW vDG       BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
NLS_CreateNLS_View( zVIEW     vSubtask )
{
   zVIEW     vDG = 0; 
   //:VIEW vTZDGSRCO BASED ON LOD TZDGSRCO
   zVIEW     vTZDGSRCO = 0; 
   zSHORT    RESULT; 


   //:GET VIEW vDG NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );
   //:IF RESULT < 0
   if ( RESULT < 0 )
   { 
      //:GET VIEW vTZDGSRCO NAMED "TZDGSRCO"
      RESULT = GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );
      //:CreateViewFromViewForTask( vDG, vTZDGSRCO, vSubtask )
      CreateViewFromViewForTask( &vDG, vTZDGSRCO, vSubtask );
      //:NAME VIEW vDG "TZDGSRCO_NLS"
      SetNameForView( vDG, "TZDGSRCO_NLS", 0, zLEVEL_TASK );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NLS_DIL_NewText( VIEW vSubtask )

//:   VIEW vDG     BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
NLS_DIL_NewText( zVIEW     vSubtask )
{
   zVIEW     vDG = 0; 
   //:VIEW Profile BASED ON LOD TZ__PRFO
   zVIEW     Profile = 0; 
   zSHORT    RESULT; 


   //:GET VIEW vDG     NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );
   //:GET VIEW Profile NAMED "ProfileXFER"
   RESULT = GetViewByName( &Profile, "ProfileXFER", vSubtask, zLEVEL_TASK );

   //:// Create the new entity with default values.  We will make sure that
   //:// the profile has a default LanguageIndex and the text is initialized with
   //:// original text.
   //:IF Profile.TZ.NLS_LanguageIndex = 0
   if ( CompareAttributeToInteger( Profile, "TZ", "NLS_LanguageIndex", 0 ) == 0 )
   { 
      //:Profile.TZ.NLS_LanguageIndex = 1
      SetAttributeFromInteger( Profile, "TZ", "NLS_LanguageIndex", 1 );
   } 

   //:END
   //:Profile.TZ.NLS_LastText = vDG.Context.DIL_Text
   SetAttributeFromAttribute( Profile, "TZ", "NLS_LastText", vDG, "Context", "DIL_Text" );

   //:CreateMetaEntity( vSubtask, vDG, "DIL_NLS_Text", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vDG, "DIL_NLS_Text", zPOS_AFTER );
   //:vDG.DIL_NLS_Text.Text          = Profile.TZ.NLS_LastText
   SetAttributeFromAttribute( vDG, "DIL_NLS_Text", "Text", Profile, "TZ", "NLS_LastText" );
   //:vDG.DIL_NLS_Text.LanguageIndex = Profile.TZ.NLS_LanguageIndex
   SetAttributeFromAttribute( vDG, "DIL_NLS_Text", "LanguageIndex", Profile, "TZ", "NLS_LanguageIndex" );

   //:// Allow the NLS edit boxes to be active and refresh necessary list boxes.
   //:SetCtrlState( vSubtask, "NLS_Text", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( vSubtask, "NLS_Text", zCONTROL_STATUS_ENABLED, TRUE );
   //:SetCtrlState( vSubtask, "NLS_Code", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( vSubtask, "NLS_Code", zCONTROL_STATUS_ENABLED, TRUE );
   //:SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, TRUE );
   //:RefreshCtrl( vSubtask, "NLS_Text" )
   RefreshCtrl( vSubtask, "NLS_Text" );
   //:RefreshCtrl( vSubtask, "NLS_ListBox" )
   RefreshCtrl( vSubtask, "NLS_ListBox" );
   //:RefreshCtrl( vSubtask, "OK" )
   RefreshCtrl( vSubtask, "OK" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NLS_DIL_Refresh( VIEW vSubtask )

//:   VIEW vDG     BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
NLS_DIL_Refresh( zVIEW     vSubtask )
{
   zVIEW     vDG = 0; 
   //:VIEW Profile BASED ON LOD TZ__PRFO
   zVIEW     Profile = 0; 
   //:SHORT        nEnable
   zSHORT    nEnable = 0; 
   //:SHORT        nIsCheckedOut
   zSHORT    nIsCheckedOut = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:GET VIEW vDG     NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );
   //:GET VIEW Profile NAMED "ProfileXFER"
   RESULT = GetViewByName( &Profile, "ProfileXFER", vSubtask, zLEVEL_TASK );

   //:// Before refreshing the DIL_Text list box, set up the Profile XFER object with
   //:// the NLS attributes for the NLS edit boxes.  Also enable the edit boxes.
   //:IF vDG.DIL_NLS_Text  EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDG, "DIL_NLS_Text" );
   if ( lTempInteger_0 == 0 )
   { 
      //:Profile.TZ.NLS_LanguageIndex = vDG.DIL_NLS_Text.LanguageIndex
      SetAttributeFromAttribute( Profile, "TZ", "NLS_LanguageIndex", vDG, "DIL_NLS_Text", "LanguageIndex" );
      //:Profile.TZ.NLS_LastText      = vDG.DIL_NLS_Text.Text
      SetAttributeFromAttribute( Profile, "TZ", "NLS_LastText", vDG, "DIL_NLS_Text", "Text" );
      //:nEnable = 1
      nEnable = 1;
      //:ELSE
   } 
   else
   { 
      //:// If there is no NLS entity, disable the edit boxes and set text to spaces.
      //:Profile.TZ.NLS_LastText      = ""
      SetAttributeFromString( Profile, "TZ", "NLS_LastText", "" );
      //:nEnable = 0
      nEnable = 0;
   } 

   //:END

   //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDG, zSOURCE_DOMAINGRP_META )
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDG, zSOURCE_DOMAINGRP_META );
   //:IF nIsCheckedOut != 1
   if ( nIsCheckedOut != 1 )
   { 
      //:nEnable = 0
      nEnable = 0;
   } 

   //:END

   //:SetCtrlState( vSubtask, "NLS_Text", zCONTROL_STATUS_ENABLED, nEnable )
   SetCtrlState( vSubtask, "NLS_Text", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );
   //:SetCtrlState( vSubtask, "NLS_Code", zCONTROL_STATUS_ENABLED, nEnable )
   SetCtrlState( vSubtask, "NLS_Code", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );
   //:SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, nEnable )
   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );

   //:// Now refresh all three controls
   //:RefreshCtrl( vSubtask, "NLS_ListBox" )
   RefreshCtrl( vSubtask, "NLS_ListBox" );
   //:RefreshCtrl( vSubtask, "NLS_Text" )
   RefreshCtrl( vSubtask, "NLS_Text" );
   //:RefreshCtrl( vSubtask, "NLS_Code" )
   RefreshCtrl( vSubtask, "NLS_Code" );
   //:RefreshCtrl( vSubtask, "OK" )
   RefreshCtrl( vSubtask, "OK" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NLS_DIL_SaveText( VIEW vSubtask )

//:   // On save, copy the Profile attributes that were entered in the window over
//:   // to the DIL_NLS_Text entity.
//:   SHORT        nIsCheckedOut
zOPER_EXPORT zSHORT OPERATION
NLS_DIL_SaveText( zVIEW     vSubtask )
{
   zSHORT    nIsCheckedOut = 0; 
   //:VIEW vDG     BASED ON LOD TZDGSRCO
   zVIEW     vDG = 0; 
   //:VIEW Profile BASED ON LOD TZ__PRFO
   zVIEW     Profile = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:GET VIEW vDG     NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );
   //:GET VIEW Profile NAMED "ProfileXFER"
   RESULT = GetViewByName( &Profile, "ProfileXFER", vSubtask, zLEVEL_TASK );

   //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDG, zSOURCE_DOMAINGRP_META )
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDG, zSOURCE_DOMAINGRP_META );

   //:IF vDG.DIL_NLS_Text EXISTS AND nIsCheckedOut = 1
   lTempInteger_0 = CheckExistenceOfEntity( vDG, "DIL_NLS_Text" );
   if ( lTempInteger_0 == 0 && nIsCheckedOut == 1 )
   { 
      //:vDG.DIL_NLS_Text.LanguageIndex = Profile.TZ.NLS_LanguageIndex
      SetAttributeFromAttribute( vDG, "DIL_NLS_Text", "LanguageIndex", Profile, "TZ", "NLS_LanguageIndex" );
      //:vDG.DIL_NLS_Text.Text          = Profile.TZ.NLS_LastText
      SetAttributeFromAttribute( vDG, "DIL_NLS_Text", "Text", Profile, "TZ", "NLS_LastText" );
   } 

   //:END

   //:RefreshCtrl( vSubtask, "NLS_ListBox" )
   RefreshCtrl( vSubtask, "NLS_ListBox" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NLS_ExtValueDeleteText( VIEW vSubtask )

//:   VIEW vDG BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
NLS_ExtValueDeleteText( zVIEW     vSubtask )
{
   zVIEW     vDG = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:GET VIEW vDG NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );

   //:IF vDG.ExtValueNLS_Text EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDG, "ExtValueNLS_Text" );
   if ( lTempInteger_0 == 0 )
   { 
      //:DELETE ENTITY vDG.ExtValueNLS_Text
      RESULT = DeleteEntity( vDG, "ExtValueNLS_Text", zPOS_NEXT );
      //:NLS_ExtValueRefresh( vSubtask )
      NLS_ExtValueRefresh( vSubtask );
   } 

   //:END
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NLS_ExtValueNewText( VIEW vSubtask )

//:   VIEW vDG     BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
NLS_ExtValueNewText( zVIEW     vSubtask )
{
   zVIEW     vDG = 0; 
   //:VIEW Profile BASED ON LOD TZ__PRFO
   zVIEW     Profile = 0; 
   zSHORT    RESULT; 


   //:GET VIEW vDG     NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );
   //:GET VIEW Profile NAMED "ProfileXFER"
   RESULT = GetViewByName( &Profile, "ProfileXFER", vSubtask, zLEVEL_TASK );

   //:// Create the new entity with default values.  We will make sure that
   //:// the profile has a default LanguageIndex and the text is initialized with
   //:// original text.
   //:IF Profile.TZ.NLS_LanguageIndex = 0
   if ( CompareAttributeToInteger( Profile, "TZ", "NLS_LanguageIndex", 0 ) == 0 )
   { 
      //:Profile.TZ.NLS_LanguageIndex = 1
      SetAttributeFromInteger( Profile, "TZ", "NLS_LanguageIndex", 1 );
   } 

   //:END
   //:Profile.TZ.NLS_LastText = vDG.TableEntry.ExtValueNLS_Text
   SetAttributeFromAttribute( Profile, "TZ", "NLS_LastText", vDG, "TableEntry", "ExtValueNLS_Text" );
   //:CreateMetaEntity( vSubtask, vDG, "ExtValueNLS_Text", zPOS_AFTER )
   CreateMetaEntity( vSubtask, vDG, "ExtValueNLS_Text", zPOS_AFTER );
   //:vDG.ExtValueNLS_Text.Text          = Profile.TZ.NLS_LastText
   SetAttributeFromAttribute( vDG, "ExtValueNLS_Text", "Text", Profile, "TZ", "NLS_LastText" );
   //:vDG.ExtValueNLS_Text.LanguageIndex = Profile.TZ.NLS_LanguageIndex
   SetAttributeFromAttribute( vDG, "ExtValueNLS_Text", "LanguageIndex", Profile, "TZ", "NLS_LanguageIndex" );

   //:// Allow the NLS edit boxes to be active and refresh necessary list boxes.
   //:SetCtrlState( vSubtask, "NLS_Text", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( vSubtask, "NLS_Text", zCONTROL_STATUS_ENABLED, TRUE );
   //:SetCtrlState( vSubtask, "NLS_Code", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( vSubtask, "NLS_Code", zCONTROL_STATUS_ENABLED, TRUE );
   //:SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, TRUE )
   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, TRUE );
   //:RefreshCtrl( vSubtask, "NLS_Text" )
   RefreshCtrl( vSubtask, "NLS_Text" );
   //:RefreshCtrl( vSubtask, "NLS_ListBox" )
   RefreshCtrl( vSubtask, "NLS_ListBox" );
   //:RefreshCtrl( vSubtask, "OK" )
   RefreshCtrl( vSubtask, "OK" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NLS_ExtValueRefresh( VIEW vSubtask )

//:   VIEW vDG     BASED ON LOD TZDGSRCO
zOPER_EXPORT zSHORT OPERATION
NLS_ExtValueRefresh( zVIEW     vSubtask )
{
   zVIEW     vDG = 0; 
   //:VIEW Profile BASED ON LOD TZ__PRFO
   zVIEW     Profile = 0; 
   //:SHORT        nEnable
   zSHORT    nEnable = 0; 
   //:SHORT        nIsCheckedOut
   zSHORT    nIsCheckedOut = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:GET VIEW vDG     NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );
   //:GET VIEW Profile NAMED "ProfileXFER"
   RESULT = GetViewByName( &Profile, "ProfileXFER", vSubtask, zLEVEL_TASK );

   //:// Before refreshing the ExternalValue list box, set up the Profile XFER object with
   //:// the NLS attributes for the NLS edit boxes.  Also enable the edit boxes.
   //:IF vDG.ExtValueNLS_Text  EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( vDG, "ExtValueNLS_Text" );
   if ( lTempInteger_0 == 0 )
   { 
      //:Profile.TZ.NLS_LanguageIndex = vDG.ExtValueNLS_Text.LanguageIndex
      SetAttributeFromAttribute( Profile, "TZ", "NLS_LanguageIndex", vDG, "ExtValueNLS_Text", "LanguageIndex" );
      //:Profile.TZ.NLS_LastText      = vDG.ExtValueNLS_Text.Text
      SetAttributeFromAttribute( Profile, "TZ", "NLS_LastText", vDG, "ExtValueNLS_Text", "Text" );
      //:nEnable = 1
      nEnable = 1;
      //:ELSE
   } 
   else
   { 
      //:// If there is no NLS entity, disable the edit boxes and set text to spaces.
      //:Profile.TZ.NLS_LastText      = ""
      SetAttributeFromString( Profile, "TZ", "NLS_LastText", "" );
      //:nEnable = 0
      nEnable = 0;
   } 

   //:END

   //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDG, zSOURCE_DOMAINGRP_META )
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDG, zSOURCE_DOMAINGRP_META );
   //:IF nIsCheckedOut != 1
   if ( nIsCheckedOut != 1 )
   { 
      //:nEnable = 0
      nEnable = 0;
   } 

   //:END

   //:SetCtrlState( vSubtask, "NLS_Text", zCONTROL_STATUS_ENABLED, nEnable )
   SetCtrlState( vSubtask, "NLS_Text", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );
   //:SetCtrlState( vSubtask, "NLS_Code", zCONTROL_STATUS_ENABLED, nEnable )
   SetCtrlState( vSubtask, "NLS_Code", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );
   //:SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, nEnable )
   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, (zLONG) nEnable );

   //:// Now refresh all three controls
   //:RefreshCtrl( vSubtask, "NLS_ListBox" )
   RefreshCtrl( vSubtask, "NLS_ListBox" );
   //:RefreshCtrl( vSubtask, "NLS_Text" )
   RefreshCtrl( vSubtask, "NLS_Text" );
   //:RefreshCtrl( vSubtask, "NLS_Code" )
   RefreshCtrl( vSubtask, "NLS_Code" );
   //:RefreshCtrl( vSubtask, "OK" )
   RefreshCtrl( vSubtask, "OK" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:NLS_ExtValueSaveText( VIEW vSubtask )

//:   // On save, copy the Profile attributes that were entered in the window over
//:   // to the ExtValueNLS_Text entity.
//:   SHORT        nIsCheckedOut
zOPER_EXPORT zSHORT OPERATION
NLS_ExtValueSaveText( zVIEW     vSubtask )
{
   zSHORT    nIsCheckedOut = 0; 
   //:VIEW vDG     BASED ON LOD TZDGSRCO
   zVIEW     vDG = 0; 
   //:VIEW Profile BASED ON LOD TZ__PRFO
   zVIEW     Profile = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 


   //:GET VIEW vDG     NAMED "TZDGSRCO_NLS"
   RESULT = GetViewByName( &vDG, "TZDGSRCO_NLS", vSubtask, zLEVEL_TASK );
   //:GET VIEW Profile NAMED "ProfileXFER"
   RESULT = GetViewByName( &Profile, "ProfileXFER", vSubtask, zLEVEL_TASK );

   //:nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDG, zSOURCE_DOMAINGRP_META )
   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDG, zSOURCE_DOMAINGRP_META );

   //:IF vDG.ExtValueNLS_Text EXISTS AND nIsCheckedOut = 1
   lTempInteger_0 = CheckExistenceOfEntity( vDG, "ExtValueNLS_Text" );
   if ( lTempInteger_0 == 0 && nIsCheckedOut == 1 )
   { 
      //:vDG.ExtValueNLS_Text.LanguageIndex = Profile.TZ.NLS_LanguageIndex
      SetAttributeFromAttribute( vDG, "ExtValueNLS_Text", "LanguageIndex", Profile, "TZ", "NLS_LanguageIndex" );
      //:vDG.ExtValueNLS_Text.Text          = Profile.TZ.NLS_LastText
      SetAttributeFromAttribute( vDG, "ExtValueNLS_Text", "Text", Profile, "TZ", "NLS_LastText" );
   } 

   //:END

   //:RefreshCtrl( vSubtask, "NLS_ListBox" )
   RefreshCtrl( vSubtask, "NLS_ListBox" );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:LoadProperties( VIEW vSubtask )

//:   SHORT nRC
zOPER_EXPORT zSHORT OPERATION
LoadProperties( zVIEW     vSubtask )
{
   zSHORT    nRC = 0; 
   //:VIEW  vTZDGSRCO_DETAIL   BASED ON LOD TZDGSRCO
   zVIEW     vTZDGSRCO_DETAIL = 0; 
   //:VIEW  vTZDGSRCO          BASED ON LOD TZDGSRCO
   zVIEW     vTZDGSRCO = 0; 
   //:VIEW  vProfileXFER       BASED ON LOD TZ__PRFO
   zVIEW     vProfileXFER = 0; 
   zSHORT    RESULT; 


   //:GET VIEW vProfileXFER  NAMED "ProfileXFER"
   RESULT = GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   //:GET VIEW vTZDGSRCO     NAMED "TZDGSRCO"
   RESULT = GetViewByName( &vTZDGSRCO, "TZDGSRCO", vSubtask, zLEVEL_TASK );

   //:vProfileXFER.DM.NewDomainGroupFlag = "P"
   SetAttributeFromString( vProfileXFER, "DM", "NewDomainGroupFlag", "P" );

   //:nRC = GetViewByName( vTZDGSRCO_DETAIL, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK )
   nRC = GetViewByName( &vTZDGSRCO_DETAIL, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );

   //:IF nRC >= 0
   if ( nRC >= 0 )
   { 
      //:DropView( vTZDGSRCO_DETAIL )
      DropView( vTZDGSRCO_DETAIL );
   } 

   //:END

   //:CreateViewFromViewForTask( vTZDGSRCO_DETAIL, vTZDGSRCO, vSubtask )
   CreateViewFromViewForTask( &vTZDGSRCO_DETAIL, vTZDGSRCO, vSubtask );
   //:SetNameForView( vTZDGSRCO_DETAIL, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK )
   SetNameForView( vTZDGSRCO_DETAIL, "TZDGSRCO_DETAIL", vSubtask, zLEVEL_TASK );
   return( 0 );
// END
} 


//:DIALOG OPERATION
//:CHANGE_JavaClassName( VIEW ViewToWindow )

//:   VIEW vTaskLPLR REGISTERED AS TaskLPLR
zOPER_EXPORT zSHORT OPERATION
CHANGE_JavaClassName( zVIEW     ViewToWindow )
{
   zVIEW     vTaskLPLR = 0; 
   zSHORT    RESULT; 
   //:VIEW vTZDGSRCO REGISTERED AS TZDGSRCO
   zVIEW     vTZDGSRCO = 0; 

   RESULT = GetViewByName( &vTaskLPLR, "TaskLPLR", ViewToWindow, zLEVEL_TASK );
   RESULT = GetViewByName( &vTZDGSRCO, "TZDGSRCO", ViewToWindow, zLEVEL_TASK );

   //:vTZDGSRCO.Domain.JavaClass = vTaskLPLR.DomainJavaClass.JavaClassName 
   SetAttributeFromAttribute( vTZDGSRCO, "Domain", "JavaClass", vTaskLPLR, "DomainJavaClass", "JavaClassName" );
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
