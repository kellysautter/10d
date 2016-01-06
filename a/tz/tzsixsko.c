// New build by Rolf Kaemmerer 31.07.1995
///////////////////////////////////////////////////////////////////////
// Update
//        Compare TE-Attribute <-> XOD Attribute by name, not
//        by ZKey. Neccesary because ERD-attributes for Foreign
//        Keys were deleted during synchronisation.
//        (17.01.1996) HH
//
///////////////////////////////////////////////////////////////////////

/* Change LOG

2000.10.25  SR  Z2000 Size of path variables
   Modified size of szFileName in function zoTZSIXSKO_SaveXSK
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.Adjust the size of messagetext
   variables.

1999.06.15  RG  Z9J  Savin XSK-Files
   While saving the XSK-file work attributes may not be saved.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


extern zPCHAR   szlATTRIB; // = "ATTRIB";
extern zPCHAR   szlENTITY; // = "ENTITY";

zOPER_EXPORT zSHORT OPERATION
zofnTZSIXSKO_BldXSK( zVIEW vKZSIXSKO_Root,          /* Work View     */
                     zVIEW vKZSIXSKO,               /* Original View */
                     zVIEW vTZZOXOD1,               /* Work XOD      */
                     zVIEW vTZTENVRO_REF );         /* Original TE   */
zOPER_EXPORT zSHORT OPERATION
zofnTZSIXSKO_BldXSKChildEnt( zVIEW  vKZSIXSKO_Root,  /* Work View     */
                             zVIEW  vKZSIXSKO,       /* Original View */
                             zVIEW  vTZZOXOD1,       /* Work XOD      */
                             zVIEW  vTZTENVRO_REF,   /* Original TE   */
                             zSHORT nEntitySet );

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zoTZSIXSKO_SaveXSK
//
// PURPOSE:
//
// TRIGGLODED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_Save
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zoTZSIXSKO_SaveXSK( zVIEW vTZZOXODO, zVIEW vTZTENVRO )
{
   zVIEW  vKZSIXSKO;
   zVIEW  vKZSIXSKO_Root;
   zVIEW  vTZTENVRO_REF = 0;
   zVIEW  vTZTENVR1 = 0;
   zVIEW  vTaskLPLR;
   zSHORT nRC;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szMetaName[ 33 ];
   zLONG  lTE_ZKey = 0;
   zSHORT nAttrCount = 0;
   zSHORT nDS_Count = 0;

   /*=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?
   ?? Hier muss noch der oder die Siron Kennungen abgehandelt
   ?? werden
   **=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?*/

//  Check access to TE Object
   if ( !vTZTENVRO )
   {
      nRC = MessagePrompt( vTZZOXODO, "XOD_02", "Internal LOD Error",
                    "Unable to Load related TE.\n\nOpen or Create TE First.",
                    0, zBUTTONS_OK, 0, 0 );
      vKZSIXSKO = 0;  // initialize to something DKS 2000.03.09?
      DeleteEntity( vKZSIXSKO, "OBJECT", zREPOS_NONE );
      return( 0 );
   }

   nRC = ActivateEmptyObjectInstance( &vKZSIXSKO, "KZSIXSKO", vTZZOXODO,
                                      zMULTIPLE | zLEVEL_APPLICATION );
   if ( nRC < 0 )
   {
      nRC = MessagePrompt( vTZZOXODO, "XOD_01", "Internal LOD Error",
                           "Unable to Activate Empty XSK.\n\nSeek help.",
                            0, zBUTTONS_OK, 0, 0 );
      return( 0 );
   }

   nRC = SetNameForView( vKZSIXSKO, "KZSIXSKO", vTZZOXODO, zLEVEL_TASK );
   nRC = CreateEntity( vKZSIXSKO, "OBJECT", zPOS_AFTER );

   nRC = SetAttributeFromAttribute( vKZSIXSKO, "OBJECT", "Name",
                                    vTZZOXODO, "OBJECT", "NAME" );
   nRC = SetAttributeFromAttribute( vKZSIXSKO, "OBJECT", "ZKey",
                                    vTZZOXODO, "OBJECT", "ZKey" );

   CreateViewFromViewForTask( &vKZSIXSKO_Root, vKZSIXSKO, 0 );
   zofnTZSIXSKO_BldXSK( vKZSIXSKO_Root, vKZSIXSKO, vTZZOXODO, vTZTENVRO );

   // Commit the XSK to a file named the same as the XOD,
   // except with suffix ".XSK".
   GetViewByName( &vTaskLPLR, "TaskLPLR", vTZZOXODO, zLEVEL_TASK );
   GetStringFromAttribute( szFileName, sizeof( szFileName ), vTaskLPLR, "LPLR", "ExecDir" );
   strcat_s( szFileName, sizeof( szFileName ), "\\" );
   GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vTZZOXODO, "OBJECT", "NAME" );
   strcat_s( szFileName, sizeof( szFileName ), szMetaName );
   strcat_s( szFileName, sizeof( szFileName ), ".XSK" );
   CommitOI_ToFile( vKZSIXSKO, szFileName, zASCII );

   DropView( vKZSIXSKO_Root );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zofnTZSIXSKO_BldXSK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zofnTZSIXSKO_BldXSK ( zVIEW vKZSIXSKO_Root,
                      zVIEW vKZSIXSKO, zVIEW vTZZOXODO,
                      zVIEW vTZTENVRO_REF )
{
   zVIEW  vTZZOXOD1;
   zVIEW  vTZZOXOD_TestWorkAttrib;
   zVIEW  vTZTENVR1;
   zPCHAR szDBMS;
   zSHORT nEntImpls;
   zSHORT nRC;
   zLONG  lEntityZKey;
   zSHORT nAttrCount = 0;
   zSHORT nEntitySet = 0;
   zCHAR   szMsg[ zLONG_MESSAGE_LTH + 1 ];
   zPCHAR  szMsgName;

   nRC = CreateViewFromViewForTask( &vTZTENVR1, vTZTENVRO_REF, 0 );
   nRC = CreateViewFromViewForTask( &vTZZOXOD1, vTZZOXODO, 0 );

   nRC = SetCursorFirstEntity( vTZZOXOD1, "ENTITY", "" );

   /*=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?
   ?? Was ist hier mit Work Entities ?
   ?? Wie koennen diese umgangen werden
   ?? natuerlich auch fuer Kinder beruecksichtigen
   **=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?=?*/

   /* Check physik, if no, then do not process this entity */
   if ( (nRC = CheckExistenceOfEntity( vTZZOXOD1,
                                       "DATARECORD" )) >= zCURSOR_SET )
   {
     // Suche SironKatalog in TE
     nRC = GetIntegerFromAttribute( &lEntityZKey, vTZZOXOD1, "ENTITY",
                                    "ERENT_TOK" );
     nRC = SetCursorFirstEntityByInteger( vTZTENVR1, "ER_Entity",
                                          "ZKey", lEntityZKey,
                                          "TE_DBMS_Source" );
     if ( nRC < zCURSOR_SET )
     {
         GetAddrForAttribute( &szDBMS,
                              vTZTENVR1, "TE_DBMS_Source", "DBMS" );
           nEntImpls = 0;
           for ( nRC = SetCursorFirstEntityByInteger( vTZTENVR1, "ER_Entity",
                                                      "ZKey", lEntityZKey,
                                                      "TE_DB_Environ" );
               nRC > zCURSOR_UNCHANGED;
               nRC = SetCursorNextEntityByInteger( vTZTENVR1, "ER_Entity",
                                             "ZKey", lEntityZKey,
                                             "TE_DB_Environ" ) )
           {
              if ( CompareAttributeToString( vTZTENVR1, "TE_DBMS_Source",
                                             "DBMS", szDBMS ) == 0 )
              {
                 nEntImpls++;
              }
           }

           if ( nEntImpls == 1 )
           {
              nRC = zCURSOR_SET;
           }
           else
           if ( nEntImpls > 1 )
           {
              for ( nRC = SetCursorFirstEntityByInteger( vTZTENVR1, "ER_Entity",
                                                         "ZKey", lEntityZKey,
                                                         "TE_DB_Environ" );
                    nRC > zCURSOR_UNCHANGED;
                    nRC = SetCursorNextEntityByInteger( vTZTENVR1, "ER_Entity",
                                                        "ZKey", lEntityZKey,
                                                        "TE_DB_Environ" ) )
              {
                 if ( CompareAttributeToString( vTZTENVR1, "TE_DBMS_Source",
                                                "DBMS", szDBMS ) == 0 )
                 {
                    strcpy_s( szMsg, sizeof( szMsg ), "Do you want to use Data Source '" );
                    GetAddrForAttribute( &szMsgName, vTZTENVR1, "TE_DBMS_Source", "Name" );
                    strcat_s( szMsg, sizeof( szMsg ), szMsgName );
                    strcat_s( szMsg, sizeof( szMsg ), "' to implement ER Entity '" );
                    GetAddrForAttribute( &szMsgName, vTZTENVR1, "ER_Entity", "Name" );
                    strcat_s( szMsg, sizeof( szMsg ), szMsgName );
                    strcat_s( szMsg, sizeof( szMsg ), "'?" );
                    nRC = MessagePrompt( vTZTENVR1, "XOD_06", "TE Choice",
                                         szMsg, 0, zBUTTONS_YESNO, 0, 0 );
                    if ( nRC == zRESPONSE_YES )
                       break;   // found Entity implemented in same DBMS
                 }
              }
           }

           if ( nRC < zCURSOR_SET )
           {
              strcpy_s( szMsg, sizeof( szMsg ),
                       "Unable to Find Entity in TE Data Source with same DBMS.\n\nEntity Name = " );
              GetAddrForAttribute( &szMsgName, vTZZOXOD1, "ENTITY", "NAME" );
              strcat_s( szMsg, sizeof( szMsg ), szMsgName );
              nRC = MessagePrompt( vTZZOXOD1, "XOD_03",
                                   "Internal LOD Error", szMsg,
                                   0, zBUTTONS_OK, 0, 0 );
              return( 0 );
           }
     }

     // Now: First Entity found
     //      if SironKatalog Exits
     //         create KATALOG and KATCOM entities
     //         create first ENTITY and ENTITYCOM entities
     //         create dependent ATTRIB and ATTRICOM entities
     nRC = CheckExistenceOfEntity ( vTZTENVR1, "SironKatalog" );
     if (  nRC >= zCURSOR_SET )
     {
        // Create KATALOG entity
        nRC = CreateEntity ( vKZSIXSKO_Root, "KATALOG", zPOS_AFTER );
        SetMatchingAttributesByName(vKZSIXSKO_Root, "KATALOG",
                               vTZTENVR1, "SironKatalog", zSET_ALL );

        for ( nRC = SetCursorFirstEntity( vTZTENVR1,
                                   "KatalogComment", "SironKatalog");
              nRC > zCURSOR_UNCHANGED;
              nRC = SetCursorNextEntity( vTZTENVR1,
                                  "KatalogComment", "SironKatalog") )
        {
          CreateEntity ( vKZSIXSKO_Root, "KATCOM", zPOS_AFTER );
          SetMatchingAttributesByName( vKZSIXSKO_Root, "KATCOM",
                             vTZTENVR1, "KatalogComment", zSET_ALL );
        }

        // Create first ENTITY entity
        nRC = CreateEntity ( vKZSIXSKO_Root, "ENTITY", zPOS_AFTER );
        nEntitySet = 1;
        SetMatchingAttributesByName(vKZSIXSKO_Root, "ENTITY",
                                    vTZTENVR1, "OrigSironField", zSET_ALL );
        SetAttributeFromAttribute( vKZSIXSKO_Root, "ENTITY", "ERENT_TOK",
                                   vTZZOXOD1,      "ENTITY", "ERENT_TOK" );

        for ( nRC = SetCursorFirstEntity( vTZTENVR1,
                                          "FieldComment", "OrigSironField");
              nRC > zCURSOR_UNCHANGED;
              nRC = SetCursorNextEntity( vTZTENVR1,
                                        "FieldComment", "OrigSironField") )
        {
          CreateEntity ( vKZSIXSKO_Root, "ENTITYCOM", zPOS_AFTER );
          SetMatchingAttributesByName( vKZSIXSKO_Root, "ENTITYCOM",
                                       vTZTENVR1, "FieldComment",
                                       zSET_ALL );
        }

        // Create all dependent ATTRIB entities
        //                             according to XOD ATTRIB entities
        for ( nRC = SetCursorFirstEntity( vTZZOXOD1, "ATTRIB", "ENTITY");
              nRC > zCURSOR_UNCHANGED;
              nRC = SetCursorNextEntity( vTZZOXOD1, "ATTRIB", "ENTITY") )
        {
          // If the Attribute is a Workattribute there is no Datafield.
          // Do not save the Workattributes in the XSK-file.
          nRC = CreateViewFromViewForTask( &vTZZOXOD_TestWorkAttrib, vTZZOXOD1, 0 );
          nRC = SetCursorFirstEntityByAttr (vTZZOXOD_TestWorkAttrib, "DATAFIELD",
                                            "FLDNAME",
                                            vTZZOXOD1, "ATTRIB", "NAME", "");

          if (nRC == zCURSOR_SET)
          {
             nRC = SetCursorFirstEntityByAttr
                        ( vTZTENVR1, "TE_FieldDataRel", "Name",
                          vTZZOXOD1, "ATTRIB", "NAME", "TE_TablRec");


             CreateEntity ( vKZSIXSKO_Root, "ATTRIB", zPOS_AFTER );
             SetMatchingAttributesByName( vKZSIXSKO_Root, "ATTRIB",
                                       vTZTENVR1, "SironField", zSET_ALL );
             SetAttributeFromAttribute( vKZSIXSKO_Root, "ATTRIB", "XVAATT_TOK",
                                     vTZZOXOD1,      "ATTRIB", "XVAATT_TOK" );
             // Create all dependent ATTRIBCOM entities
             for ( nRC = SetCursorFirstEntity( vTZTENVR1,"SironFieldComment","SironField");
                nRC > zCURSOR_UNCHANGED;
                nRC = SetCursorNextEntity( vTZTENVR1,"SironFieldComment","SironField") )
             {
                CreateEntity ( vKZSIXSKO_Root, "ATTRIBCOM", zPOS_AFTER );
                SetMatchingAttributesByName( vKZSIXSKO_Root, "ATTRIBCOM",
                                         vTZTENVR1, "SironFieldComment", zSET_ALL );
             }
          }

          DropView( vTZZOXOD_TestWorkAttrib );
        }
     }
   }

   nRC = CheckExistenceOfEntity( vTZZOXOD1, "CHILDENTITY" );
   if ( nRC >= zCURSOR_SET )
        zofnTZSIXSKO_BldXSKChildEnt( vKZSIXSKO_Root, vKZSIXSKO, vTZZOXOD1,
                                     vTZTENVRO_REF, nEntitySet );

   nRC = DropView( vTZZOXOD1 );
   nRC = DropView( vTZTENVR1 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zofnTZSIXSKO_BldXSKChildEnt
//
// PURPOSE:
//
// TRIGGLODED BY:
//    WINDOW: TZZOLODD
//    ACTION: OPTION
//    NAME: File_Save
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zofnTZSIXSKO_BldXSKChildEnt ( zVIEW vKZSIXSKO_Root,
                              zVIEW vKZSIXSKO, zVIEW vTZZOXOD1,
                              zVIEW vTZTENVRO_REF,
                              zSHORT nEntityFlag )
{
   zVIEW  vTZTENVRO_Parent;
   zSHORT nRC;
   zSHORT nRC2;
   zPCHAR szDBMS;
   zSHORT nEntImpls;
   zLONG  lEntityZKey;
   zSHORT nEntitySet    = nEntityFlag;
   zSHORT nSetViewToSubXsk = 0;
   zCHAR  szMsg[ zLONG_MESSAGE_LTH + 1 ];
   zPCHAR szMsgName;

   nRC = CreateViewFromViewForTask( &vTZTENVRO_Parent, vTZTENVRO_REF, 0 );

   nRC = zCURSOR_SET;
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetViewToSubobject( vTZZOXOD1, "CHILDENTITY" );

      /* Check physik, if no, then do not process this entity */
      if ( (nRC = CheckExistenceOfEntity ( vTZZOXOD1, "DATARECORD" )) >= zCURSOR_SET )
      {

         nRC = GetIntegerFromAttribute( &lEntityZKey, vTZZOXOD1,
                                        "ENTITY", "ERENT_TOK" );
         nRC = SetCursorFirstEntityByInteger( vTZTENVRO_Parent, "ER_Entity",
                                              "ZKey", lEntityZKey,
                                              "TE_DBMS_Source" );
         if ( nRC < zCURSOR_SET )
         {
            GetAddrForAttribute( &szDBMS,
                                 vTZTENVRO_Parent, "TE_DBMS_Source", "DBMS" );
            nEntImpls = 0;
            for ( nRC = SetCursorFirstEntityByInteger( vTZTENVRO_Parent, "ER_Entity",
                                                "ZKey", lEntityZKey,
                                                "TE_DB_Environ" );
                  nRC > zCURSOR_UNCHANGED;
                  nRC = SetCursorNextEntityByInteger( vTZTENVRO_Parent, "ER_Entity",
                                                "ZKey", lEntityZKey,
                                                "TE_DB_Environ" ) )
            {
               if ( CompareAttributeToString( vTZTENVRO_Parent, "TE_DBMS_Source", "DBMS",
                               szDBMS ) == 0 )
               {
                  nEntImpls++;
               }
            }
            if ( nEntImpls == 1 )
            {
               nRC = zCURSOR_SET;
            }
            else
            if ( nEntImpls > 1 )
            {
              for ( nRC = SetCursorFirstEntityByInteger( vTZTENVRO_Parent, "ER_Entity",
                                                  "ZKey", lEntityZKey,
                                                  "TE_DB_Environ" );
                    nRC > zCURSOR_UNCHANGED;
                    nRC = SetCursorNextEntityByInteger( vTZTENVRO_Parent, "ER_Entity",
                                                  "ZKey", lEntityZKey,
                                                  "TE_DB_Environ" ) )
              {
                 if ( CompareAttributeToString( vTZTENVRO_Parent, "TE_DBMS_Source", "DBMS",
                                szDBMS ) == 0 )
                 {
                    strcpy_s( szMsg, sizeof( szMsg ), "Do you want to use Data Source '" );
                    GetAddrForAttribute( &szMsgName,
                                         vTZTENVRO_Parent, "TE_DBMS_Source", "Name" );
                    strcat_s( szMsg, sizeof( szMsg ), szMsgName );
                    strcat_s( szMsg, sizeof( szMsg ), "' to implement ER Entity '" );
                    GetAddrForAttribute( &szMsgName,
                                         vTZTENVRO_Parent, "ER_Entity", "Name" );
                    strcat_s( szMsg, sizeof( szMsg ), szMsgName );
                    strcat_s( szMsg, sizeof( szMsg ), "'?" );
                    nRC = MessagePrompt ( vTZTENVRO_Parent, "XOD_06", "TE Choice",
                                          szMsg, 0, zBUTTONS_YESNO, 0, 0 );
                    if ( nRC == zRESPONSE_YES )
                        break;   // found Entity implemented in same DBMS
                 }
              }
            }

            if ( nRC < zCURSOR_SET )
            {
               strcpy_s( szMsg, sizeof( szMsg ), "Unable to Find Entity in TE Data Source with same DBMS.\n\nEntity Name = " );
               GetAddrForAttribute( &szMsgName, vTZZOXOD1, "ER_EntityRec", "Name" );
               strcat_s( szMsg, sizeof( szMsg ), szMsgName );
               nRC = MessagePrompt( vTZZOXOD1, "XOD_06", "Internal LOD Error", szMsg, 0,
                                    zBUTTONS_OK, 0, 0 );
               nRC = DropView( vTZTENVRO_Parent );
               return( 0 );
            }
         }

         //  new Entity found
         //  if SironGroup exists
         //     if SironKatalog exists
         //        create new Katalog
         //     create next ENTITY and ENTITYCOM entities
         //     create dependent ATTRIB and ATTRICOM entities
         nRC = CheckExistenceOfEntity ( vTZTENVRO_Parent, "SironGroup" );
         if ( nRC >= zCURSOR_SET )
         {
          nRC = CheckExistenceOfEntity ( vTZTENVRO_Parent, "SironKatalog" );
          if ( nRC >= zCURSOR_SET )
          {
            // Go back to KATALOG entity
            if ( nEntitySet )
            {
              while ( ResetViewFromSubobject( vKZSIXSKO_Root ) == 0 );
            }

            // Create new  KATALOG entity
            nRC = SetCursorLastEntity ( vKZSIXSKO_Root, "KATALOG", "" );
            nRC = CreateEntity ( vKZSIXSKO_Root, "KATALOG", zPOS_AFTER );
            SetMatchingAttributesByName(vKZSIXSKO_Root, "KATALOG",
                                        vTZTENVRO_Parent, "SironKatalog", zSET_ALL );

            for ( nRC = SetCursorFirstEntity( vTZTENVRO_Parent,
                                            "KatalogComment", "SironKatalog");
                  nRC > zCURSOR_UNCHANGED;
                  nRC = SetCursorNextEntity( vTZTENVRO_Parent,
                                             "KatalogComment", "SironKatalog") )
            {
              CreateEntity ( vKZSIXSKO_Root, "KATCOM", zPOS_AFTER );
              SetMatchingAttributesByName( vKZSIXSKO_Root, "KATCOM",
                                           vTZTENVRO_Parent, "KatalogComment", zSET_ALL );
            }
          }

          // ViewToSubobject only, if caller created an ENTITY entity
          if ( nEntitySet )
          {
            if ( CheckExistenceOfEntity( vKZSIXSKO_Root, "ENTITY" )
                                                             >= zCURSOR_SET )
            {
               SetViewToSubobject( vKZSIXSKO_Root, "ENTITYCHILD" );
               nSetViewToSubXsk = 1;
            }
          }

          // Create next ENTITY entity
          nRC = SetCursorLastEntity ( vKZSIXSKO_Root, "ENTITY", "" );
          nRC = CreateEntity ( vKZSIXSKO_Root, "ENTITY", zPOS_AFTER );
          nEntitySet = 1;
          SetMatchingAttributesByName(vKZSIXSKO_Root, "ENTITY",
                                      vTZTENVRO_Parent, "SironGroup", zSET_ALL );
          SetAttributeFromInteger( vKZSIXSKO_Root, "ENTITY", "ERENT_TOK",
                                   lEntityZKey );

          for ( nRC = SetCursorFirstEntity( vTZTENVRO_Parent, "GroupComment", "SironGroup");
                nRC > zCURSOR_UNCHANGED;
                nRC = SetCursorNextEntity( vTZTENVRO_Parent, "GroupComment", "SironGroup") )
          {
            CreateEntity ( vKZSIXSKO_Root, "ENTITYCOM", zPOS_AFTER );
            SetMatchingAttributesByName( vKZSIXSKO_Root, "ENTITYCOM",
                                         vTZTENVRO_Parent, "GroupComment", zSET_ALL );
          }

          // Create all dependent ATTRIB entities according to XOD ATTRIB
          // entities
          for ( nRC  = SetCursorFirstEntity( vTZZOXOD1, "ATTRIB", "ENTITY");
                nRC  > zCURSOR_UNCHANGED;
                nRC  = SetCursorNextEntity( vTZZOXOD1, "ATTRIB", "ENTITY") )
          {
             nRC = SetCursorFirstEntityByAttr( vTZTENVRO_Parent,
                                               "TE_FieldDataRel","Name",
                                               vTZZOXOD1, "ATTRIB", "NAME",
                                               "TE_TablRec");

             nRC = CheckExistenceOfEntity( vTZTENVRO_Parent, "SironField" );
             if ( nRC < zCURSOR_SET )
             {
               strcpy_s( szMsg, sizeof( szMsg ), "Unable to Find SironField in TE Data Source .\n\nAttribute Name = " );
               GetAddrForAttribute( &szMsgName, vTZZOXOD1, "ATTRIB", "NAME" );
               strcat_s( szMsg, sizeof( szMsg ), szMsgName );
               nRC = MessagePrompt ( vTZZOXOD1, "XOD_06",
                                     "Internal XOD Error", szMsg,
                                     0, zBUTTONS_OK, 0, 0 );
               nRC = DropView( vTZTENVRO_Parent );
               return( 0 );
             }

             CreateEntity( vKZSIXSKO_Root, "ATTRIB", zPOS_AFTER );
             SetMatchingAttributesByName( vKZSIXSKO_Root, "ATTRIB",
                                          vTZTENVRO_Parent, "SironField", zSET_ALL );
             SetAttributeFromAttribute( vKZSIXSKO_Root, "ATTRIB", "XVAATT_TOK",
                                        vTZZOXOD1,      "ATTRIB", "XVAATT_TOK" );

             // Create all dependent ATTRIBCOM entities
             for ( nRC = SetCursorFirstEntity( vTZTENVRO_Parent,
                                               "SironFieldComment","SironField");
                   nRC > zCURSOR_UNCHANGED;
                   nRC = SetCursorNextEntity( vTZTENVRO_Parent,
                                              "SironFieldComment","SironField"))
             {
               CreateEntity ( vKZSIXSKO_Root, "ATTRIBCOM", zPOS_AFTER );
               SetMatchingAttributesByName( vKZSIXSKO_Root, "ATTRIBCOM",
                                            vTZTENVRO_Parent, "SironFieldComment",
                                            zSET_ALL );
             }
          }
        }
      }

      // Abstieg des Pfades
      nRC = CheckExistenceOfEntity( vTZZOXOD1, "CHILDENTITY" );
      if ( nRC >= zCURSOR_SET )
              zofnTZSIXSKO_BldXSKChildEnt( vKZSIXSKO_Root, vKZSIXSKO,
                   vTZZOXOD1, vTZTENVRO_Parent, nEntitySet );

      nRC2 = ResetViewFromSubobject( vTZZOXOD1 );
      if ( nSetViewToSubXsk )
      {
        nRC2 = ResetViewFromSubobject( vKZSIXSKO_Root );
        nSetViewToSubXsk = 0;
      }

      nRC  = SetCursorNextEntity( vTZZOXOD1, "CHILDENTITY", "" );
   }

   if ( vTZTENVRO_Parent )
      nRC = DropView( vTZTENVRO_Parent );

   return( 0 );
}

