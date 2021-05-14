#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZPESRCO_CreateWindowStyle( zVIEW     vSubtask,
                             zPVIEW    NewPPE,
                             zVIEW     OldPPE );


static zSHORT
oTZPESRCO_CreateControlDef( zVIEW     vSubtask,
                            zPVIEW    NewPPE,
                            zVIEW     OldPPE );


static zSHORT
oTZPESRCO_MergeWindowStyleSubob( zVIEW     vSubtask,
                                 zPVIEW    NewPPE,
                                 zVIEW     OldPPE );


static zSHORT
oTZPESRCO_MergeControlDefSubob( zVIEW     vSubtask,
                                zPVIEW    NewPPE,
                                zVIEW     OldPPE );


static zSHORT
oTZPESRCO_CheckPPE_Format( zPVIEW    OldPPE,
                           zVIEW     SourceLPLR );


static zSHORT
oTZPESRCO_CreateControlDefSubob( zVIEW     vSubtask,
                                 zPVIEW    NewPPE,
                                 zVIEW     OldPPE );


static zSHORT
oTZPESRCO_MergeCtrlDefChilds( zPVIEW    NewPPE,
                              zVIEW     OldPPE );


static zSHORT
oTZPESRCO_MergeFontsAndColors( zVIEW     vSubtask,
                               zPVIEW    NewPPE,
                               zVIEW     OldPPE );


static zSHORT
oTZPESRCO_SetCtrlDefVisible( zPVIEW    NewPPE,
                             zVIEW     OldPPE );


//:TRANSFORMATION OPERATION
//:MigratePPE( VIEW NewPPE      BASED ON LOD TZPESRCO,
//:            VIEW SourceLPLR  BASED ON LOD TZCMLPLO,
//:            VIEW vSubtask )

//:   SHORT        nRC
zOPER_EXPORT zSHORT OPERATION
oTZPESRCO_MigratePPE( zPVIEW    NewPPE,
                      zVIEW     SourceLPLR,
                      zVIEW     vSubtask )
{
   zSHORT    nRC = 0; 
   //:VIEW OldPPE  BASED ON LOD TZPESRCO
   zVIEW     OldPPE = 0; 
   zSHORT    RESULT; 


   //://Check the PPE from SourceLPLR
   //://If the PPE in new format, migrate it. Otherwise Return.
   //:nRC = CheckPPE_Format( OldPPE, SourceLPLR )
   nRC = oTZPESRCO_CheckPPE_Format( &OldPPE, SourceLPLR );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:RETURN nRC
      return( nRC );
   } 

   //:END

   //:NAME VIEW OldPPE "OldPPE"
   SetNameForView( OldPPE, "OldPPE", 0, zLEVEL_TASK );

   //:ActivateEmptyMetaOI( vSubtask, NewPPE, zSOURCE_PENV_META, zSINGLE )
   ActivateEmptyMetaOI( vSubtask, NewPPE, zSOURCE_PENV_META, zSINGLE );
   //:NAME VIEW NewPPE "NewPPE"
   SetNameForView( *NewPPE, "NewPPE", 0, zLEVEL_TASK );

   //:CreateMetaEntity( vSubtask, NewPPE, "PresEnvDef", zPOS_AFTER )
   CreateMetaEntity( vSubtask, *NewPPE, "PresEnvDef", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewPPE, "PresEnvDef",
   //:                             OldPPE, "PresEnvDef", zSET_NULL )
   SetMatchingAttributesByName( *NewPPE, "PresEnvDef", OldPPE, "PresEnvDef", zSET_NULL );

   //:// // Copy Font subobject
   //:// FOR EACH OldPPE.Font
   //://    CreateMetaEntity( vSubtask, NewPPE, "Font", zPOS_AFTER )
   //://    SetMatchingAttributesByName( NewPPE, "Font",
   //://                                 OldPPE, "Font", zSET_NULL )
   //:// END

   //:// // Copy Color subobject
   //:// FOR EACH OldPPE.Color
   //://    CreateMetaEntity( vSubtask, NewPPE, "Color", zPOS_AFTER )
   //://    SetMatchingAttributesByName( NewPPE, "Color",
   //://                                 OldPPE, "Color", zSET_NULL )
   //:// END

   //:// Copy WindowStyle subobject with Events, Colors and Fonts
   //:FOR EACH OldPPE.WindowStyle
   RESULT = SetCursorFirstEntity( OldPPE, "WindowStyle", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: CreateWindowStyle( vSubtask, NewPPE, OldPPE )
      oTZPESRCO_CreateWindowStyle( vSubtask, NewPPE, OldPPE );
      RESULT = SetCursorNextEntity( OldPPE, "WindowStyle", "" );
   } 

   //:END

   //:// Copy ControlDef subobject with Events, Colors, Fonts...
   //:FOR EACH OldPPE.ControlDef
   RESULT = SetCursorFirstEntity( OldPPE, "ControlDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: CreateControlDef( vSubtask, NewPPE, OldPPE )
      oTZPESRCO_CreateControlDef( vSubtask, NewPPE, OldPPE );
      RESULT = SetCursorNextEntity( OldPPE, "ControlDef", "" );
   } 

   //:END

   //://Include subobjects from ControlDef
   //:CreateControlDefSubob( vSubtask, NewPPE, OldPPE )
   oTZPESRCO_CreateControlDefSubob( vSubtask, NewPPE, OldPPE );

   //:ResetView( NewPPE )
   ResetView( *NewPPE );
   //:// OrderEntityForView( NewPPE, "Color", "Tag A" )
   //:OrderEntityForView( NewPPE, "ControlDef", "Tag A" )
   OrderEntityForView( *NewPPE, "ControlDef", "Tag A" );
   //:AssignZKeyToMetaOI( vSubtask, NewPPE, zSOURCE_PENV_META, 0 )
   AssignZKeyToMetaOI( vSubtask, *NewPPE, zSOURCE_PENV_META, 0 );
   //:CommitMetaOI( vSubtask, NewPPE, zSOURCE_PENV_META )
   CommitMetaOI( vSubtask, *NewPPE, zSOURCE_PENV_META );

   //:DropObjectInstance( OldPPE )
   DropObjectInstance( OldPPE );

   //:RETURN 0
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:MergePPE( VIEW NewPPE      BASED ON LOD TZPESRCO,
//:          VIEW SourceLPLR  BASED ON LOD TZCMLPLO,
//:          VIEW vSubtask )

//:   VIEW OldPPE  BASED ON LOD TZPESRCO
zOPER_EXPORT zSHORT OPERATION
oTZPESRCO_MergePPE( zVIEW     NewPPE,
                    zVIEW     SourceLPLR,
                    zVIEW     vSubtask )
{
   zVIEW     OldPPE = 0; 

   //:STRING (256) MessageText
   zCHAR     MessageText[ 257 ] = { 0 }; 
   //:SHORT        RESULT
   zSHORT    RESULT = 0; 
   //:SHORT        nRC
   zSHORT    nRC = 0; 
   //:SHORT        nDoNotMergeCtrlDef
   zSHORT    nDoNotMergeCtrlDef = 0; 
   //:INTEGER      lVisible
   zLONG     lVisible = 0; 
   zCHAR     szTempString_0[ 33 ]; 
   zLONG     lTempInteger_0; 
   zSHORT    lTempInteger_1; 


   //://Check the PPE from SourceLPLR
   //://If the PPE in new format, merge it. Otherwise Return.
   //:nRC = CheckPPE_Format( OldPPE, SourceLPLR )
   nRC = oTZPESRCO_CheckPPE_Format( &OldPPE, SourceLPLR );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:RETURN nRC
      return( nRC );
   } 

   //:END

   //:NAME VIEW OldPPE "OldPPE"
   SetNameForView( OldPPE, "OldPPE", 0, zLEVEL_TASK );
   //:NAME VIEW NewPPE "NewPPE"
   SetNameForView( NewPPE, "NewPPE", 0, zLEVEL_TASK );

   //:// Merge Root Entity
   //:IF OldPPE.PresEnvDef.Desc != "" AND NewPPE.PresEnvDef.Desc = ""
   if ( CompareAttributeToString( OldPPE, "PresEnvDef", "Desc", "" ) != 0 && CompareAttributeToString( NewPPE, "PresEnvDef", "Desc", "" ) == 0 )
   { 
      //:NewPPE.PresEnvDef.Desc = OldPPE.PresEnvDef.Desc
      SetAttributeFromAttribute( NewPPE, "PresEnvDef", "Desc", OldPPE, "PresEnvDef", "Desc" );
   } 

   //:END

   //:// Merge Fonts and Colors Subobjects
   //:// MergeFontsAndColors( vSubtask, NewPPE, OldPPE )

   //:// Merge WindowStyle subobject with Events, Colors and Fonts
   //:FOR EACH OldPPE.WindowStyle
   RESULT = SetCursorFirstEntity( OldPPE, "WindowStyle", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:RESULT = CheckExistenceOfEntity( NewPPE, "WindowStyle" )
      RESULT = CheckExistenceOfEntity( NewPPE, "WindowStyle" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:SET CURSOR FIRST NewPPE.WindowStyle WHERE
         //:    NewPPE.WindowStyle.Tag = OldPPE.WindowStyle.Tag
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OldPPE, "WindowStyle", "Tag" );
         RESULT = SetCursorFirstEntityByString( NewPPE, "WindowStyle", "Tag", szTempString_0, "" );
      } 

      //:END
      //://WindowStyle does not exists, create new Style with all Sobobjects
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:CreateWindowStyle( vSubtask, NewPPE, OldPPE )
         oTZPESRCO_CreateWindowStyle( vSubtask, &NewPPE, OldPPE );
         //:ELSE
      } 
      else
      { 
         //:MergeWindowStyleSubob( vSubtask, NewPPE, OldPPE )
         oTZPESRCO_MergeWindowStyleSubob( vSubtask, &NewPPE, OldPPE );
      } 

      RESULT = SetCursorNextEntity( OldPPE, "WindowStyle", "" );
      //:END
   } 

   //:END

   //:// Merge ControlDef subobject with Events, Colors, Fonts...
   //:FOR EACH OldPPE.ControlDef
   RESULT = SetCursorFirstEntity( OldPPE, "ControlDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:RESULT = CheckExistenceOfEntity( NewPPE, "ControlDef" )
      RESULT = CheckExistenceOfEntity( NewPPE, "ControlDef" );
      //:IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:SET CURSOR FIRST NewPPE.ControlDef WHERE
         //:    NewPPE.ControlDef.Key = OldPPE.ControlDef.Key
         GetIntegerFromAttribute( &lTempInteger_0, OldPPE, "ControlDef", "Key" );
         RESULT = SetCursorFirstEntityByInteger( NewPPE, "ControlDef", "Key", lTempInteger_0, "" );
      } 

      //:END
      //://ControlDef does not exists, create new ControlDef with all Subobjects
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:nDoNotMergeCtrlDef = 0
         nDoNotMergeCtrlDef = 0;
         //:IF NewPPE.ControlDef EXISTS
         lTempInteger_1 = CheckExistenceOfEntity( NewPPE, "ControlDef" );
         if ( lTempInteger_1 == 0 )
         { 
            //://Check Tag
            //:SET CURSOR FIRST NewPPE.ControlDef WHERE
            //:    NewPPE.ControlDef.Tag = OldPPE.ControlDef.Tag
            GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OldPPE, "ControlDef", "Tag" );
            RESULT = SetCursorFirstEntityByString( NewPPE, "ControlDef", "Tag", szTempString_0, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //://Error situation
               //:MessageText = NEW_LINE + "The control with Tag '" + NewPPE.ControlDef.Tag +
               //:              "' already exists with different key." +
               //:              NEW_LINE + NEW_LINE + "Do you want to continue?"
               ZeidonStringCopy( MessageText, 1, 0, NEW_LINE, 1, 0, 257 );
               ZeidonStringConcat( MessageText, 1, 0, "The control with Tag '", 1, 0, 257 );
               GetVariableFromAttribute( szTempString_0, 0, 'S', 33, NewPPE, "ControlDef", "Tag", "", 0 );
               ZeidonStringConcat( MessageText, 1, 0, szTempString_0, 1, 0, 257 );
               ZeidonStringConcat( MessageText, 1, 0, "' already exists with different key.", 1, 0, 257 );
               ZeidonStringConcat( MessageText, 1, 0, NEW_LINE, 1, 0, 257 );
               ZeidonStringConcat( MessageText, 1, 0, NEW_LINE, 1, 0, 257 );
               ZeidonStringConcat( MessageText, 1, 0, "Do you want to continue?", 1, 0, 257 );
               //:nRC = MessagePrompt( vSubtask, "CM01013", "Configuration Management",
               //:                     MessageText, 1, zBUTTONS_YESNO, zRESPONSE_YES, 0 )
               nRC = MessagePrompt( vSubtask, "CM01013", "Configuration Management", MessageText, 1, zBUTTONS_YESNO, zRESPONSE_YES, 0 );
               //:IF nRC = zRESPONSE_NO
               if ( nRC == zRESPONSE_NO )
               { 
                  //:DropObjectInstance( OldPPE )
                  DropObjectInstance( OldPPE );
                  //:RETURN -1
                  return( -1 );
                  //:ELSE
               } 
               else
               { 
                  //:nDoNotMergeCtrlDef = 1
                  nDoNotMergeCtrlDef = 1;
               } 

               //:END
            } 

            //:END
         } 

         //:END
         //://Create ControlDef witch Subobject
         //:IF nDoNotMergeCtrlDef = 0
         if ( nDoNotMergeCtrlDef == 0 )
         { 
            //:CreateControlDef( vSubtask, NewPPE, OldPPE )
            oTZPESRCO_CreateControlDef( vSubtask, &NewPPE, OldPPE );
            //:lVisible = NewPPE.ControlDef.Invisible
            GetIntegerFromAttribute( &lVisible, NewPPE, "ControlDef", "Invisible" );
            //:IF lVisible = 0 OR lVisible = -1
            if ( lVisible == 0 || lVisible == -1 )
            { 
               //:AddToAttributeFromInteger( NewPPE, "PresEnvDef", "LastCtrl", 1 )
               AddToAttributeFromInteger( NewPPE, "PresEnvDef", "LastCtrl", 1 );
            } 

            //:END
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:// Merge Subobjects
         //:SetCtrlDefVisible( NewPPE, OldPPE )
         oTZPESRCO_SetCtrlDefVisible( &NewPPE, OldPPE );
         //:MergeControlDefSubob( vSubtask, NewPPE, OldPPE )
         oTZPESRCO_MergeControlDefSubob( vSubtask, &NewPPE, OldPPE );
      } 

      RESULT = SetCursorNextEntity( OldPPE, "ControlDef", "" );
      //:END
   } 

   //:END

   //://Include subobjects from ControlDef
   //:MergeCtrlDefChilds( NewPPE, OldPPE )
   oTZPESRCO_MergeCtrlDefChilds( &NewPPE, OldPPE );

   //:ResetView( NewPPE )
   ResetView( NewPPE );
   //:OrderEntityForView( NewPPE, "Color", "Tag A" )
   OrderEntityForView( NewPPE, "Color", "Tag A" );
   //:OrderEntityForView( NewPPE, "ControlDef", "Tag A" )
   OrderEntityForView( NewPPE, "ControlDef", "Tag A" );
   //:AssignZKeyToMetaOI( vSubtask, NewPPE, zSOURCE_PENV_META, 0 )
   AssignZKeyToMetaOI( vSubtask, NewPPE, zSOURCE_PENV_META, 0 );
   //:CommitMetaOI( vSubtask, NewPPE, zSOURCE_PENV_META )
   CommitMetaOI( vSubtask, NewPPE, zSOURCE_PENV_META );

   //:DropObjectInstance( OldPPE )
   DropObjectInstance( OldPPE );

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
oTZPESRCO_CreateWindowStyle( zVIEW     vSubtask,
                             zPVIEW    NewPPE,
                             zVIEW     OldPPE )
{
   zSHORT    RESULT; 

   //:CreateWindowStyle( VIEW vSubtask,
   //:                VIEW NewPPE BASED ON LOD TZPESRCO,
   //:                VIEW OldPPE BASED ON LOD TZPESRCO )

   //:CreateMetaEntity( vSubtask, NewPPE, "WindowStyle", zPOS_AFTER )
   CreateMetaEntity( vSubtask, *NewPPE, "WindowStyle", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewPPE, "WindowStyle",
   //:                             OldPPE, "WindowStyle", zSET_NULL )
   SetMatchingAttributesByName( *NewPPE, "WindowStyle", OldPPE, "WindowStyle", zSET_NULL );

   //:FOR EACH OldPPE.EventDef
   RESULT = SetCursorFirstEntity( OldPPE, "EventDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( vSubtask, NewPPE, "EventDef", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewPPE, "EventDef", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewPPE, "EventDef",
      //:                             OldPPE, "EventDef", zSET_NULL )
      SetMatchingAttributesByName( *NewPPE, "EventDef", OldPPE, "EventDef", zSET_NULL );
      RESULT = SetCursorNextEntity( OldPPE, "EventDef", "" );
   } 

   //:END

   //:// FOR EACH OldPPE.WndColorDef
   //://    CreateMetaEntity( vSubtask, NewPPE, "WndColorDef", zPOS_AFTER )
   //://    SetMatchingAttributesByName( NewPPE, "WndColorDef",
   //://                                 OldPPE, "WndColorDef", zSET_NULL )
   //://    SET CURSOR FIRST NewPPE.Color WHERE
   //://        NewPPE.Color.Tag = OldPPE.WndColor.Tag
   //://    INCLUDE NewPPE.WndColor FROM NewPPE.Color
   //:// END

   //:// FOR EACH OldPPE.WndFontDef
   //://    CreateMetaEntity( vSubtask, NewPPE, "WndFontDef", zPOS_AFTER )
   //://    SetMatchingAttributesByName( NewPPE, "WndFontDef",
   //://                                 OldPPE, "WndFontDef", zSET_NULL )
   //://    SET CURSOR FIRST NewPPE.Font WHERE
   //://        NewPPE.Font.Tag = OldPPE.WndFont.Tag
   //://    INCLUDE NewPPE.WndFont FROM NewPPE.Font
   //:// END

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
static zSHORT
oTZPESRCO_CreateControlDef( zVIEW     vSubtask,
                            zPVIEW    NewPPE,
                            zVIEW     OldPPE )
{
   zSHORT    RESULT; 

   //:CreateControlDef( VIEW vSubtask,
   //:               VIEW NewPPE BASED ON LOD TZPESRCO,
   //:               VIEW OldPPE BASED ON LOD TZPESRCO )

   //:CreateMetaEntity( vSubtask, NewPPE, "ControlDef", zPOS_AFTER )
   CreateMetaEntity( vSubtask, *NewPPE, "ControlDef", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewPPE, "ControlDef",
   //:                             OldPPE, "ControlDef", zSET_NULL )
   SetMatchingAttributesByName( *NewPPE, "ControlDef", OldPPE, "ControlDef", zSET_NULL );

   //:FOR EACH OldPPE.CtrlEventDef
   RESULT = SetCursorFirstEntity( OldPPE, "CtrlEventDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( vSubtask, NewPPE, "CtrlEventDef", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewPPE, "CtrlEventDef", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewPPE, "CtrlEventDef",
      //:                             OldPPE, "CtrlEventDef", zSET_NULL )
      SetMatchingAttributesByName( *NewPPE, "CtrlEventDef", OldPPE, "CtrlEventDef", zSET_NULL );
      RESULT = SetCursorNextEntity( OldPPE, "CtrlEventDef", "" );
   } 

   //:END

   //:// FOR EACH OldPPE.CtrlColorDef
   //://    CreateMetaEntity( vSubtask, NewPPE, "CtrlColorDef", zPOS_AFTER )
   //://    SetMatchingAttributesByName( NewPPE, "CtrlColorDef",
   //://                                 OldPPE, "CtrlColorDef", zSET_NULL )
   //://    SET CURSOR FIRST NewPPE.Color WHERE
   //://        NewPPE.Color.Tag = OldPPE.CtrlColor.Tag
   //://    INCLUDE NewPPE.CtrlColor FROM NewPPE.Color
   //:// END

   //:// FOR EACH OldPPE.CtrlFontDef
   //://    CreateMetaEntity( vSubtask, NewPPE, "CtrlFontDef", zPOS_AFTER )
   //://    SetMatchingAttributesByName( NewPPE, "CtrlFontDef",
   //://                                 OldPPE, "CtrlFontDef", zSET_NULL )
   //://    SET CURSOR FIRST NewPPE.Font WHERE
   //://        NewPPE.Font.Tag = OldPPE.CtrlFont.Tag
   //://    INCLUDE NewPPE.CtrlFont FROM NewPPE.Font
   //:// END

   //:FOR EACH OldPPE.CtrlMapDef
   RESULT = SetCursorFirstEntity( OldPPE, "CtrlMapDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( vSubtask, NewPPE, "CtrlMapDef", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewPPE, "CtrlMapDef", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewPPE, "CtrlMapDef",
      //:                             OldPPE, "CtrlMapDef", zSET_NULL )
      SetMatchingAttributesByName( *NewPPE, "CtrlMapDef", OldPPE, "CtrlMapDef", zSET_NULL );
      RESULT = SetCursorNextEntity( OldPPE, "CtrlMapDef", "" );
   } 

   //:END

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:MergeWindowStyleSubob( VIEW vSubtask,
//:                       VIEW NewPPE BASED ON LOD TZPESRCO,
//:                       VIEW OldPPE BASED ON LOD TZPESRCO )

//:   SHORT  RESULT
static zSHORT
oTZPESRCO_MergeWindowStyleSubob( zVIEW     vSubtask,
                                 zPVIEW    NewPPE,
                                 zVIEW     OldPPE )
{
   zSHORT    RESULT = 0; 
   zCHAR     szTempString_0[ 33 ]; 


   //:// Merge Subobject EventDef.
   //:FOR EACH OldPPE.EventDef
   RESULT = SetCursorFirstEntity( OldPPE, "EventDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: RESULT = CheckExistenceOfEntity( NewPPE, "EventDef" )
      RESULT = CheckExistenceOfEntity( *NewPPE, "EventDef" );
      //: IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //:SET CURSOR FIRST NewPPE.EventDef WHERE
         //:    NewPPE.EventDef.Tag = OldPPE.EventDef.Tag
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OldPPE, "EventDef", "Tag" );
         RESULT = SetCursorFirstEntityByString( *NewPPE, "EventDef", "Tag", szTempString_0, "" );
      } 

      //: END
      //: //EventDef does not exists, create it
      //: IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //: CreateMetaEntity( vSubtask, NewPPE, "EventDef", zPOS_AFTER )
         CreateMetaEntity( vSubtask, *NewPPE, "EventDef", zPOS_AFTER );
         //: SetMatchingAttributesByName( NewPPE, "EventDef",
         //:                              OldPPE, "EventDef", zSET_NULL )
         SetMatchingAttributesByName( *NewPPE, "EventDef", OldPPE, "EventDef", zSET_NULL );
      } 

      RESULT = SetCursorNextEntity( OldPPE, "EventDef", "" );
      //: END
   } 

   //:END

   //:// // Merge Subobject WndColorDef.
   //:// FOR EACH OldPPE.WndColorDef
   //://     RESULT = CheckExistenceOfEntity( NewPPE, "WndColorDef" )
   //://     IF RESULT >= zCURSOR_SET
   //://        SET CURSOR FIRST NewPPE.WndColorDef WHERE
   //://            NewPPE.WndColorDef.Tag = OldPPE.WndColorDef.Tag
   //://     END
   //://
   //://     // WndColorDef does not exists, create it.
   //://     IF RESULT < zCURSOR_SET
   //://        CreateMetaEntity( vSubtask, NewPPE, "WndColorDef", zPOS_AFTER )
   //://        SetMatchingAttributesByName( NewPPE, "WndColorDef",
   //://                                     OldPPE, "WndColorDef", zSET_NULL )
   //://        SET CURSOR FIRST NewPPE.Color WHERE
   //://            NewPPE.Color.Tag = OldPPE.WndColor.Tag
   //://        INCLUDE NewPPE.WndColor FROM NewPPE.Color
   //://     END
   //:// END

   //:// // Merge Subobject WndFontDef.
   //:// FOR EACH OldPPE.WndFontDef
   //://     RESULT = CheckExistenceOfEntity( NewPPE, "WndFontDef" )
   //://     IF RESULT >= zCURSOR_SET
   //://        SET CURSOR FIRST NewPPE.WndFontDef WHERE
   //://            NewPPE.WndFontDef.Tag = OldPPE.WndFontDef.Tag
   //://     END
   //://
   //://     // WndFontDef does not exists, create it
   //://     IF RESULT < zCURSOR_SET
   //://        CreateMetaEntity( vSubtask, NewPPE, "WndFontDef", zPOS_AFTER )
   //://        SetMatchingAttributesByName( NewPPE, "WndFontDef",
   //://                                     OldPPE, "WndFontDef", zSET_NULL )
   //://        SET CURSOR FIRST NewPPE.Font WHERE
   //://            NewPPE.Font.Tag = OldPPE.WndFont.Tag
   //://        INCLUDE NewPPE.WndFont FROM NewPPE.Font
   //://     END
   //:// END

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:MergeControlDefSubob( VIEW vSubtask,
//:                      VIEW NewPPE BASED ON LOD TZPESRCO,
//:                      VIEW OldPPE BASED ON LOD TZPESRCO )

//:   SHORT  RESULT
static zSHORT
oTZPESRCO_MergeControlDefSubob( zVIEW     vSubtask,
                                zPVIEW    NewPPE,
                                zVIEW     OldPPE )
{
   zSHORT    RESULT = 0; 
   zCHAR     szTempString_0[ 33 ]; 


   //:// Merge Subobject CtrlEventDef
   //:FOR EACH OldPPE.CtrlEventDef
   RESULT = SetCursorFirstEntity( OldPPE, "CtrlEventDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: RESULT = CheckExistenceOfEntity( NewPPE, "CtrlEventDef" )
      RESULT = CheckExistenceOfEntity( *NewPPE, "CtrlEventDef" );
      //: IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //: SET CURSOR FIRST NewPPE.CtrlEventDef WHERE
         //:     NewPPE.CtrlEventDef.Tag = OldPPE.CtrlEventDef.Tag
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OldPPE, "CtrlEventDef", "Tag" );
         RESULT = SetCursorFirstEntityByString( *NewPPE, "CtrlEventDef", "Tag", szTempString_0, "" );
      } 

      //: END
      //: //CtrlEventDef does not exists, create it
      //: IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //: CreateMetaEntity( vSubtask, NewPPE, "CtrlEventDef", zPOS_AFTER )
         CreateMetaEntity( vSubtask, *NewPPE, "CtrlEventDef", zPOS_AFTER );
         //: SetMatchingAttributesByName( NewPPE, "CtrlEventDef",
         //:                              OldPPE, "CtrlEventDef", zSET_NULL )
         SetMatchingAttributesByName( *NewPPE, "CtrlEventDef", OldPPE, "CtrlEventDef", zSET_NULL );
      } 

      RESULT = SetCursorNextEntity( OldPPE, "CtrlEventDef", "" );
      //:END
   } 

   //:END

   //:// // Merge Subobject CtrlColorDef
   //:// FOR EACH OldPPE.CtrlColorDef
   //://     RESULT = CheckExistenceOfEntity( NewPPE, "CtrlColorDef" )
   //://     IF RESULT >= zCURSOR_SET
   //://        SET CURSOR FIRST NewPPE.CtrlColorDef WHERE
   //://            NewPPE.CtrlColorDef.Tag = OldPPE.CtrlColorDef.Tag
   //://     END
   //://
   //://     // CtrlColorDef does not exists, create it.
   //://     IF RESULT < zCURSOR_SET
   //://        CreateMetaEntity( vSubtask, NewPPE, "CtrlColorDef", zPOS_AFTER )
   //://        SetMatchingAttributesByName( NewPPE, "CtrlColorDef",
   //://                                     OldPPE, "CtrlColorDef", zSET_NULL )
   //://        SET CURSOR FIRST NewPPE.Color WHERE
   //://            NewPPE.Color.Tag = OldPPE.CtrlColor.Tag
   //://        INCLUDE NewPPE.CtrlColor FROM NewPPE.Color
   //://     END
   //:// END

   //:// // Merge Subobject CtrlFontDef
   //:// FOR EACH OldPPE.CtrlFontDef
   //://     RESULT = CheckExistenceOfEntity( NewPPE, "CtrlFontDef" )
   //://     IF RESULT >= zCURSOR_SET
   //://        SET CURSOR FIRST NewPPE.CtrlFontDef WHERE
   //://            NewPPE.CtrlFontDef.Tag = OldPPE.CtrlFontDef.Tag
   //://     END
   //://
   //://     // CtrlFontDef does not exists, create it
   //://     IF RESULT < zCURSOR_SET
   //://        CreateMetaEntity( vSubtask, NewPPE, "CtrlFontDef", zPOS_AFTER )
   //://        SetMatchingAttributesByName( NewPPE, "CtrlFontDef",
   //://                                     OldPPE, "CtrlFontDef", zSET_NULL )
   //://        SET CURSOR FIRST NewPPE.Font WHERE
   //://            NewPPE.Font.Tag = OldPPE.CtrlFont.Tag
   //://        INCLUDE NewPPE.CtrlFont FROM NewPPE.Font
   //://     END
   //:// END

   //:// Merge Subobject CtrlMapDef
   //:FOR EACH OldPPE.CtrlMapDef
   RESULT = SetCursorFirstEntity( OldPPE, "CtrlMapDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //: RESULT = CheckExistenceOfEntity( NewPPE, "CtrlMapDef" )
      RESULT = CheckExistenceOfEntity( *NewPPE, "CtrlMapDef" );
      //: IF RESULT >= zCURSOR_SET
      if ( RESULT >= zCURSOR_SET )
      { 
         //: SET CURSOR FIRST NewPPE.CtrlMapDef WHERE
         //:     NewPPE.CtrlMapDef.Tag = OldPPE.CtrlMapDef.Tag
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OldPPE, "CtrlMapDef", "Tag" );
         RESULT = SetCursorFirstEntityByString( *NewPPE, "CtrlMapDef", "Tag", szTempString_0, "" );
      } 

      //: END
      //: //CtrlMapDef does not exists, create it
      //: IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //: CreateMetaEntity( vSubtask, NewPPE, "CtrlMapDef", zPOS_AFTER )
         CreateMetaEntity( vSubtask, *NewPPE, "CtrlMapDef", zPOS_AFTER );
         //: SetMatchingAttributesByName( NewPPE, "CtrlMapDef",
         //:                              OldPPE, "CtrlMapDef", zSET_NULL )
         SetMatchingAttributesByName( *NewPPE, "CtrlMapDef", OldPPE, "CtrlMapDef", zSET_NULL );
      } 

      RESULT = SetCursorNextEntity( OldPPE, "CtrlMapDef", "" );
      //: END
   } 

   //:END

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:CheckPPE_Format( VIEW OldPPE     BASED ON LOD TZPESRCO,
//:                 VIEW SourceLPLR BASED ON LOD TZCMLPLO )

//:   STRING ( 513 ) SourceFileName  // zMAX_FILESPEC_LTH+1
static zSHORT
oTZPESRCO_CheckPPE_Format( zPVIEW    OldPPE,
                           zVIEW     SourceLPLR )
{
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 


   //:GetStringFromAttribute( SourceFileName, SourceLPLR, "LPLR", "MetaSrcDir" )
   GetStringFromAttribute( SourceFileName, zsizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   //:SourceFileName = SourceFileName + "\ZEIDON.PPE"
   ZeidonStringConcat( SourceFileName, 1, 0, "\\ZEIDON.PPE", 1, 0, 514 );
   //:nRC = ActivateOI_FromFile( OldPPE, "TZPESRCO", SourceLPLR, SourceFileName, 8192 )
   nRC = ActivateOI_FromFile( OldPPE, "TZPESRCO", SourceLPLR, SourceFileName, 8192 );
   //:IF nRC >= 0
   if ( nRC >= 0 )
   { 
      //:nRC = SetCursorFirstEntityByInteger( OldPPE, "ControlDef", "Key", 1000, 0 )
      nRC = SetCursorFirstEntityByInteger( *OldPPE, "ControlDef", "Key", 1000, 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:DropView( OldPPE )
         DropView( *OldPPE );
         //:RETURN -16
         return( -16 );
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:RETURN -1
      return( -1 );
   } 

   //:END

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:CreateControlDefSubob( VIEW vSubtask,
//:                       VIEW NewPPE BASED ON LOD TZPESRCO,
//:                       VIEW OldPPE BASED ON LOD TZPESRCO )

//:   VIEW NewPPE1 BASED ON LOD TZPESRCO
static zSHORT
oTZPESRCO_CreateControlDefSubob( zVIEW     vSubtask,
                                 zPVIEW    NewPPE,
                                 zVIEW     OldPPE )
{
   zVIEW     NewPPE1 = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 


   //:CreateViewFromViewForTask( NewPPE1, NewPPE, 0 )
   CreateViewFromViewForTask( &NewPPE1, *NewPPE, 0 );

   //:SET CURSOR FIRST NewPPE.ControlDef
   RESULT = SetCursorFirstEntity( *NewPPE, "ControlDef", "" );

   //:FOR EACH OldPPE.ControlDef
   RESULT = SetCursorFirstEntity( OldPPE, "ControlDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:FOR EACH OldPPE.ValidChild
      RESULT = SetCursorFirstEntity( OldPPE, "ValidChild", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:SET CURSOR FIRST NewPPE1.ControlDef WHERE
         //:    NewPPE1.ControlDef.Key = OldPPE.ValidChild.Key
         GetIntegerFromAttribute( &lTempInteger_0, OldPPE, "ValidChild", "Key" );
         RESULT = SetCursorFirstEntityByInteger( NewPPE1, "ControlDef", "Key", lTempInteger_0, "" );
         //:INCLUDE NewPPE.ValidChild FROM NewPPE1.ControlDef
         RESULT = IncludeSubobjectFromSubobject( *NewPPE, "ValidChild", NewPPE1, "ControlDef", zPOS_AFTER );
         RESULT = SetCursorNextEntity( OldPPE, "ValidChild", "" );
      } 

      //:END

      //:FOR EACH OldPPE.ValidParent
      RESULT = SetCursorFirstEntity( OldPPE, "ValidParent", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:SET CURSOR FIRST NewPPE1.ControlDef WHERE
         //:    NewPPE1.ControlDef.Key = OldPPE.ValidParent.Key
         GetIntegerFromAttribute( &lTempInteger_1, OldPPE, "ValidParent", "Key" );
         RESULT = SetCursorFirstEntityByInteger( NewPPE1, "ControlDef", "Key", lTempInteger_1, "" );
         //:INCLUDE NewPPE.ValidParent FROM NewPPE1.ControlDef
         RESULT = IncludeSubobjectFromSubobject( *NewPPE, "ValidParent", NewPPE1, "ControlDef", zPOS_AFTER );
         RESULT = SetCursorNextEntity( OldPPE, "ValidParent", "" );
      } 

      //:END

      //:FOR EACH OldPPE.ConvertsSelfToChild
      RESULT = SetCursorFirstEntity( OldPPE, "ConvertsSelfToChild", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:SET CURSOR FIRST NewPPE1.ControlDef WHERE
         //:    NewPPE1.ControlDef.Key = OldPPE.ConvertsSelfToChild.Key
         GetIntegerFromAttribute( &lTempInteger_2, OldPPE, "ConvertsSelfToChild", "Key" );
         RESULT = SetCursorFirstEntityByInteger( NewPPE1, "ControlDef", "Key", lTempInteger_2, "" );
         //:INCLUDE NewPPE.ConvertsSelfToChild FROM NewPPE1.ControlDef
         RESULT = IncludeSubobjectFromSubobject( *NewPPE, "ConvertsSelfToChild", NewPPE1, "ControlDef", zPOS_AFTER );
         RESULT = SetCursorNextEntity( OldPPE, "ConvertsSelfToChild", "" );
      } 

      //:END

      //:FOR EACH OldPPE.ConvertableFrom
      RESULT = SetCursorFirstEntity( OldPPE, "ConvertableFrom", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:SET CURSOR FIRST NewPPE1.ControlDef WHERE
         //:    NewPPE1.ControlDef.Key = OldPPE.ConvertableFrom.Key
         GetIntegerFromAttribute( &lTempInteger_3, OldPPE, "ConvertableFrom", "Key" );
         RESULT = SetCursorFirstEntityByInteger( NewPPE1, "ControlDef", "Key", lTempInteger_3, "" );
         //:INCLUDE NewPPE.ConvertableFrom FROM NewPPE1.ControlDef
         RESULT = IncludeSubobjectFromSubobject( *NewPPE, "ConvertableFrom", NewPPE1, "ControlDef", zPOS_AFTER );
         RESULT = SetCursorNextEntity( OldPPE, "ConvertableFrom", "" );
      } 

      //:END

      //:SET CURSOR NEXT NewPPE.ControlDef
      RESULT = SetCursorNextEntity( *NewPPE, "ControlDef", "" );
      RESULT = SetCursorNextEntity( OldPPE, "ControlDef", "" );
   } 


   //:END //END FOR EACH OldPPE.ControlDef

   //:DropView( NewPPE1)
   DropView( NewPPE1 );

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:MergeCtrlDefChilds( VIEW NewPPE BASED ON LOD TZPESRCO,
//:                    VIEW OldPPE BASED ON LOD TZPESRCO )

//:   VIEW   NewPPE1 BASED ON LOD TZPESRCO
static zSHORT
oTZPESRCO_MergeCtrlDefChilds( zPVIEW    NewPPE,
                              zVIEW     OldPPE )
{
   zVIEW     NewPPE1 = 0; 
   //:SHORT  RESULT
   zSHORT    RESULT = 0; 
   zLONG     lTempInteger_0; 
   zLONG     lTempInteger_1; 
   zLONG     lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zLONG     lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zLONG     lTempInteger_7; 


   //:CreateViewFromViewForTask( NewPPE1, NewPPE, 0 )
   CreateViewFromViewForTask( &NewPPE1, *NewPPE, 0 );

   //:SET CURSOR FIRST NewPPE.ControlDef
   RESULT = SetCursorFirstEntity( *NewPPE, "ControlDef", "" );
   //:FOR EACH OldPPE.ControlDef
   RESULT = SetCursorFirstEntity( OldPPE, "ControlDef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 

      //:// Merge Subobject ValidChild
      //:FOR EACH OldPPE.ValidChild
      RESULT = SetCursorFirstEntity( OldPPE, "ValidChild", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: RESULT = CheckExistenceOfEntity( NewPPE, "ValidChild" )
         RESULT = CheckExistenceOfEntity( *NewPPE, "ValidChild" );
         //: IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //: SET CURSOR FIRST NewPPE.ValidChild WHERE
            //:     NewPPE.ValidChild.Key = OldPPE.ValidChild.Key
            GetIntegerFromAttribute( &lTempInteger_0, OldPPE, "ValidChild", "Key" );
            RESULT = SetCursorFirstEntityByInteger( *NewPPE, "ValidChild", "Key", lTempInteger_0, "" );
         } 

         //: END
         //: //ValidChild does not exists, include it
         //: IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //: SET CURSOR FIRST NewPPE1.ControlDef WHERE
            //:     NewPPE1.ControlDef.Key = OldPPE.ValidChild.Key
            GetIntegerFromAttribute( &lTempInteger_1, OldPPE, "ValidChild", "Key" );
            RESULT = SetCursorFirstEntityByInteger( NewPPE1, "ControlDef", "Key", lTempInteger_1, "" );
            //: INCLUDE NewPPE.ValidChild FROM NewPPE1.ControlDef
            RESULT = IncludeSubobjectFromSubobject( *NewPPE, "ValidChild", NewPPE1, "ControlDef", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( OldPPE, "ValidChild", "" );
         //: END
      } 

      //:END

      //:// Merge Subobject ValidParent
      //:FOR EACH OldPPE.ValidParent
      RESULT = SetCursorFirstEntity( OldPPE, "ValidParent", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: RESULT = CheckExistenceOfEntity( NewPPE, "ValidParent" )
         RESULT = CheckExistenceOfEntity( *NewPPE, "ValidParent" );
         //: IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //: SET CURSOR FIRST NewPPE.ValidParent WHERE
            //:     NewPPE.ValidParent.Key = OldPPE.ValidParent.Key
            GetIntegerFromAttribute( &lTempInteger_2, OldPPE, "ValidParent", "Key" );
            RESULT = SetCursorFirstEntityByInteger( *NewPPE, "ValidParent", "Key", lTempInteger_2, "" );
         } 

         //: END
         //: //ValidParent does not exists, include it
         //: IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //: SET CURSOR FIRST NewPPE1.ControlDef WHERE
            //:     NewPPE1.ControlDef.Key = OldPPE.ValidParent.Key
            GetIntegerFromAttribute( &lTempInteger_3, OldPPE, "ValidParent", "Key" );
            RESULT = SetCursorFirstEntityByInteger( NewPPE1, "ControlDef", "Key", lTempInteger_3, "" );
            //: INCLUDE NewPPE.ValidParent FROM NewPPE1.ControlDef
            RESULT = IncludeSubobjectFromSubobject( *NewPPE, "ValidParent", NewPPE1, "ControlDef", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( OldPPE, "ValidParent", "" );
         //: END
      } 

      //:END

      //:// Merge Subobject ConvertsSelfToChild
      //:FOR EACH OldPPE.ConvertsSelfToChild
      RESULT = SetCursorFirstEntity( OldPPE, "ConvertsSelfToChild", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: RESULT = CheckExistenceOfEntity( NewPPE, "ConvertsSelfToChild" )
         RESULT = CheckExistenceOfEntity( *NewPPE, "ConvertsSelfToChild" );
         //: IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //: SET CURSOR FIRST NewPPE.ConvertsSelfToChild WHERE
            //:     NewPPE.ConvertsSelfToChild.Key = OldPPE.ConvertsSelfToChild.Key
            GetIntegerFromAttribute( &lTempInteger_4, OldPPE, "ConvertsSelfToChild", "Key" );
            RESULT = SetCursorFirstEntityByInteger( *NewPPE, "ConvertsSelfToChild", "Key", lTempInteger_4, "" );
         } 

         //: END
         //: //ConvertsSelfToChild does not exists, include it
         //: IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //: SET CURSOR FIRST NewPPE1.ControlDef WHERE
            //:     NewPPE1.ControlDef.Key = OldPPE.ConvertsSelfToChild.Key
            GetIntegerFromAttribute( &lTempInteger_5, OldPPE, "ConvertsSelfToChild", "Key" );
            RESULT = SetCursorFirstEntityByInteger( NewPPE1, "ControlDef", "Key", lTempInteger_5, "" );
            //: INCLUDE NewPPE.ConvertsSelfToChild FROM NewPPE1.ControlDef
            RESULT = IncludeSubobjectFromSubobject( *NewPPE, "ConvertsSelfToChild", NewPPE1, "ControlDef", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( OldPPE, "ConvertsSelfToChild", "" );
         //: END
      } 

      //:END

      //:// Merge Subobject ConvertableFrom
      //:FOR EACH OldPPE.ConvertableFrom
      RESULT = SetCursorFirstEntity( OldPPE, "ConvertableFrom", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //: RESULT = CheckExistenceOfEntity( NewPPE, "ConvertableFrom" )
         RESULT = CheckExistenceOfEntity( *NewPPE, "ConvertableFrom" );
         //: IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //: SET CURSOR FIRST NewPPE.ConvertableFrom WHERE
            //:     NewPPE.ConvertableFrom.Key = OldPPE.ConvertableFrom.Key
            GetIntegerFromAttribute( &lTempInteger_6, OldPPE, "ConvertableFrom", "Key" );
            RESULT = SetCursorFirstEntityByInteger( *NewPPE, "ConvertableFrom", "Key", lTempInteger_6, "" );
         } 

         //: END
         //: //ConvertableFrom does not exists, include it
         //: IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //: SET CURSOR FIRST NewPPE1.ControlDef WHERE
            //:     NewPPE1.ControlDef.Key = OldPPE.ConvertableFrom.Key
            GetIntegerFromAttribute( &lTempInteger_7, OldPPE, "ConvertableFrom", "Key" );
            RESULT = SetCursorFirstEntityByInteger( NewPPE1, "ControlDef", "Key", lTempInteger_7, "" );
            //: INCLUDE NewPPE.ConvertableFrom FROM NewPPE1.ControlDef
            RESULT = IncludeSubobjectFromSubobject( *NewPPE, "ConvertableFrom", NewPPE1, "ControlDef", zPOS_AFTER );
         } 

         RESULT = SetCursorNextEntity( OldPPE, "ConvertableFrom", "" );
         //: END
      } 

      //:END

      //:SET CURSOR NEXT NewPPE.ControlDef
      RESULT = SetCursorNextEntity( *NewPPE, "ControlDef", "" );
      RESULT = SetCursorNextEntity( OldPPE, "ControlDef", "" );
   } 

   //:END //END FOR EACH OldPPE.ControlDef

   //:DropView( NewPPE1)
   DropView( NewPPE1 );

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:MergeFontsAndColors( VIEW vSubtask,
//:                     VIEW NewPPE BASED ON LOD TZPESRCO,
//:                     VIEW OldPPE BASED ON LOD TZPESRCO )
static zSHORT
oTZPESRCO_MergeFontsAndColors( zVIEW     vSubtask,
                               zPVIEW    NewPPE,
                               zVIEW     OldPPE )
{


   //:// SHORT  RESULT
   //://
   //:// // Merge Font subobject
   //:// FOR EACH OldPPE.Font
   //://    RESULT = CheckExistenceOfEntity( NewPPE, "Font" )
   //://    IF RESULT >= zCURSOR_SET
   //://       SET CURSOR FIRST NewPPE.Font WHERE
   //://           NewPPE.Font.Tag = OldPPE.Font.Tag
   //://    END
   //://    //Font does not exists, create it
   //://    IF RESULT < zCURSOR_SET
   //://       CreateMetaEntity( vSubtask, NewPPE, "Font", zPOS_LAST )
   //://       SetMatchingAttributesByName( NewPPE, "Font",
   //://                                    OldPPE, "Font", zSET_NULL )
   //://       NewPPE.Font.Id = NewPPE.PresEnvDef.LastFont
   //://       AddToAttributeFromInteger( NewPPE, "PresEnvDef", "LastFont", 1 )
   //://    END
   //:// END

   //:// // Merge Color subobject
   //:// FOR EACH OldPPE.Color
   //://    RESULT = CheckExistenceOfEntity( NewPPE, "Color" )
   //://    IF RESULT >= zCURSOR_SET
   //://       SET CURSOR FIRST NewPPE.Color WHERE
   //://           NewPPE.Color.Tag = OldPPE.Color.Tag
   //://    END
   //://    //Color does not exists, create it
   //://    IF RESULT < zCURSOR_SET
   //://       CreateMetaEntity( vSubtask, NewPPE, "Color", zPOS_LAST )
   //://       SetMatchingAttributesByName( NewPPE, "Color",
   //://                                    OldPPE, "Color", zSET_NULL )
   //://    END
   //:// END

   //:RETURN 0
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:SetCtrlDefVisible( VIEW NewPPE BASED ON LOD TZPESRCO,
//:                   VIEW OldPPE BASED ON LOD TZPESRCO )
static zSHORT
oTZPESRCO_SetCtrlDefVisible( zPVIEW    NewPPE,
                             zVIEW     OldPPE )
{

   //:// Flags for Entiy "ControlDef", Attribute "Invisible"
   //://   -2 - control def is visible only to the report builder
   //://   -1 - control def is visible only to the painter
   //://    0 - control def is visible to both painter and report builder
   //://    1 - control def is invisible (to be deleted)

   //:IF NewPPE.ControlDef.Tag = "INVISIBLE" AND NewPPE.ControlDef.Invisible = 1
   if ( CompareAttributeToString( *NewPPE, "ControlDef", "Tag", "INVISIBLE" ) == 0 && CompareAttributeToInteger( *NewPPE, "ControlDef", "Invisible", 1 ) == 0 )
   { 
      //:NewPPE.ControlDef.Tag = OldPPE.ControlDef.Tag
      SetAttributeFromAttribute( *NewPPE, "ControlDef", "Tag", OldPPE, "ControlDef", "Tag" );
      //:NewPPE.ControlDef.Invisible = OldPPE.ControlDef.Invisible
      SetAttributeFromAttribute( *NewPPE, "ControlDef", "Invisible", OldPPE, "ControlDef", "Invisible" );
   } 

   //:END

   //:RETURN 0
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
