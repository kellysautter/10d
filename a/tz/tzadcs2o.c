#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "TZ__OPRS.H" 
#include "tzlodopr.h" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

static zSHORT
oTZADCSDO_DS_EntityRelink( zVIEW     vSubtask,
                           zVIEW     vDSR,
                           zVIEW     vLOD );


static zSHORT
oTZADCSDO_DialogSpecEntityCopy( zVIEW     vSubtask,
                                zVIEW     SourceLPLR,
                                zVIEW     OldDS,
                                zVIEW     OldDS_C,
                                zVIEW     NewDS,
                                zVIEW     NewDS_C,
                                zVIEW     vPE,
                                zVIEW     MapLOD );


static zSHORT
oTZADCSDO_DialogSpecGetVOR_LOD( zVIEW     vSubtask,
                                zVIEW     OldDS,
                                zPVIEW    VOR,
                                zPVIEW    vLOD,
                                zPCHAR    sLOD_Name,
                                zPCHAR    sViewName,
                                zPCHAR    sOrigViewEntityName );


//:LOCAL OPERATION
//:DS_EntityRelink( VIEW vSubtask,
//:                 VIEW vDSR BASED ON LOD TZADCSDO,  // Recursive DS
//:                 VIEW vLOD BASED ON LOD TZZOLODO )

//:   VIEW         vVOR      BASED ON LOD TZWDVORO
static zSHORT
oTZADCSDO_DS_EntityRelink( zVIEW     vSubtask,
                           zVIEW     vDSR,
                           zVIEW     vLOD )
{
   zVIEW     vVOR = 0; 
   //:VIEW         vLOD_List BASED ON LOD TZZOLODO
   zVIEW     vLOD_List = 0; 
   //:STRING (128) szMsg
   zCHAR     szMsg[ 129 ] = { 0 }; 
   //:INTEGER      nRemapFlag
   zLONG     nRemapFlag = 0; 
   //:SHORT  nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zLONG     lTempInteger_2; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zSHORT    lTempInteger_3; 
   zLONG     lTempInteger_4; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zSHORT    lTempInteger_5; 
   zLONG     lTempInteger_6; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zLONG     lTempInteger_7; 
   zLONG     lTempInteger_8; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 


   //:// This is the recursive routine to process each UIS_Entity for relink and
   //:// delete of LOD_Entity, M_LOD_Attribute, L_LOD_Attribute, I_LOD_Attribute,
   //:// ListViewObjRef and IncludeViewObjRef entities.

   //:FOR EACH vDSR.UIS_ChildEntity
   RESULT = SetCursorFirstEntity( vDSR, "UIS_ChildEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( vDSR, "UIS_ChildEntity" )
      SetViewToSubobject( vDSR, "UIS_ChildEntity" );
      //:DS_EntityRelink( vDSR, vLOD, vSubtask )
      oTZADCSDO_DS_EntityRelink( vDSR, vLOD, vSubtask );
      //:ResetViewFromSubobject( vDSR )
      ResetViewFromSubobject( vDSR );
      RESULT = SetCursorNextEntity( vDSR, "UIS_ChildEntity", "" );
   } 

   //:END

   //:SET CURSOR FIRST vLOD.LOD_Entity WHERE
   //:    vLOD.LOD_Entity.ZKey = vDSR.LOD_Entity.ZKey
   GetIntegerFromAttribute( &lTempInteger_0, vDSR, "LOD_Entity", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vLOD, "LOD_Entity", "ZKey", lTempInteger_0, "" );
   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 
      //:RelinkAllSubobjectsForOI ( vDSR, "LOD_Entity", vLOD, "LOD_Entity" )
      RelinkAllSubobjectsForOI( vDSR, "LOD_Entity", vLOD, "LOD_Entity" );
      //:nRemapFlag = 0
      nRemapFlag = 0;
      //:ELSE
   } 
   else
   { 
      //:SET CURSOR FIRST vLOD.LOD_Entity WHERE
      //:    vLOD.LOD_Entity.Name = vDSR.LOD_Entity.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vDSR, "LOD_Entity", "Name" );
      RESULT = SetCursorFirstEntityByString( vLOD, "LOD_Entity", "Name", szTempString_0, "" );
      //:IF RESULT >= 0
      if ( RESULT >= 0 )
      { 
         //:nRemapFlag = 1
         nRemapFlag = 1;
         //:EXCLUDE vDSR.LOD_Entity NONE
         RESULT = ExcludeEntity( vDSR, "LOD_Entity", zREPOS_NONE );
         //:INCLUDE vDSR.LOD_Entity FROM vLOD.LOD_Entity
         RESULT = IncludeSubobjectFromSubobject( vDSR, "LOD_Entity", vLOD, "LOD_Entity", zPOS_AFTER );
         //:ELSE
      } 
      else
      { 
         //:nRemapFlag = -1
         nRemapFlag = -1;
         //:szMsg = "Deleting UIS Entity: " + vDSR.LOD_Entity.Name +
         //:        "|Not found in LOD: " + vLOD.LOD.Name
         GetVariableFromAttribute( szTempString_0, 0, 'S', 33, vDSR, "LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( szMsg, 1, 0, "Deleting UIS Entity: ", 1, 0, 129 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_0, 1, 0, 129 );
         ZeidonStringConcat( szMsg, 1, 0, "Not found in LOD: ", 1, 0, 129 );
         GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vLOD, "LOD", "Name", "", 0 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 129 );
         //:MessageSend( vSubtask, "AD11010", "Dialog Spec Relink",
         //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "AD11010", "Dialog Spec Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:DELETE ENTITY vDSR.UIS_Entity NONE
         RESULT = DeleteEntity( vDSR, "UIS_Entity", zREPOS_NONE );
      } 

      //:END
   } 

   //:END

   //:IF nRemapFlag >= 0   // There was a match on either ZKey or Name
   if ( nRemapFlag >= 0 )
   { 
      //:FOR EACH vDSR.M_LOD_Attribute
      RESULT = SetCursorFirstEntity( vDSR, "M_LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:SET CURSOR FIRST vLOD.LOD_Attribute WITHIN vLOD.LOD WHERE
         //:    vLOD.LOD_Attribute.ZKey = vDSR.M_LOD_Attribute.ZKey
         GetIntegerFromAttribute( &lTempInteger_1, vDSR, "M_LOD_Attribute", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vLOD, "LOD_Attribute", "ZKey", lTempInteger_1, "LOD" );
         //:IF RESULT < 0
         if ( RESULT < 0 )
         { 
            //:// Since there was no match on ZKey, try to find a match on LOD Entity
            //:// name and ER Attribute name, in case the Attribute had been deleted
            //:// and recreated.  In this case, re-include the Attribute.
            //:SET CURSOR FIRST vLOD.LOD_Entity WHERE
            //:                 vLOD.LOD_Entity.Name = vDSR.M_LOD_Entity.Name
            GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), vDSR, "M_LOD_Entity", "Name" );
            RESULT = SetCursorFirstEntityByString( vLOD, "LOD_Entity", "Name", szTempString_2, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:SET CURSOR FIRST vLOD.ER_Attribute WITHIN vLOD.LOD_Entity WHERE
               //:                 vLOD.ER_Attribute.Name = vDSR.M_ER_Attribute.Name
               GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), vDSR, "M_ER_Attribute", "Name" );
               RESULT = SetCursorFirstEntityByString( vLOD, "ER_Attribute", "Name", szTempString_2, "LOD_Entity" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:nRemapFlag = 0
                  nRemapFlag = 0;
                  //:ELSE
               } 
               else
               { 
                  //:nRemapFlag = -1
                  nRemapFlag = -1;
               } 

               //:END
               //:ELSE
            } 
            else
            { 
               //:nRemapFlag = -1
               nRemapFlag = -1;
            } 

            //:END
            //:IF nRemapFlag = 0  // We were able to remap to an attribute by same name.
            if ( nRemapFlag == 0 )
            { 
               //:EXCLUDE vDSR.M_LOD_Attribute NONE
               RESULT = ExcludeEntity( vDSR, "M_LOD_Attribute", zREPOS_NONE );
               //:INCLUDE vDSR.M_LOD_Attribute FROM vLOD.LOD_Attribute
               RESULT = IncludeSubobjectFromSubobject( vDSR, "M_LOD_Attribute", vLOD, "LOD_Attribute", zPOS_AFTER );
               //:ELSE
            } 
            else
            { 
               //:szMsg = "Deleting Main Attribute: " + vDSR.M_ER_Attribute.Name +
               //:        "|Not found in LOD: " + vLOD.LOD.Name
               GetVariableFromAttribute( szTempString_2, 0, 'S', 33, vDSR, "M_ER_Attribute", "Name", "", 0 );
               ZeidonStringCopy( szMsg, 1, 0, "Deleting Main Attribute: ", 1, 0, 129 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_2, 1, 0, 129 );
               ZeidonStringConcat( szMsg, 1, 0, "Not found in LOD: ", 1, 0, 129 );
               GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vLOD, "LOD", "Name", "", 0 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_3, 1, 0, 129 );
               //:MessageSend( vSubtask, "AD11011", "Dialog Spec Relink",
               //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               MessageSend( vSubtask, "AD11011", "Dialog Spec Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               //:EXCLUDE vDSR.M_LOD_Attribute NONE
               RESULT = ExcludeEntity( vDSR, "M_LOD_Attribute", zREPOS_NONE );
            } 

            //:END
         } 

         RESULT = SetCursorNextEntity( vDSR, "M_LOD_Attribute", "" );
         //:END
      } 

      //:END

      //:FOR EACH vDSR.L_LOD_Attribute
      RESULT = SetCursorFirstEntity( vDSR, "L_LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      { 
         //:nRemapFlag = 0
         nRemapFlag = 0;
         //:SET CURSOR FIRST vLOD.LOD_Attribute WITHIN vLOD.LOD WHERE
         //:    vLOD.LOD_Attribute.ZKey = vDSR.L_LOD_Attribute.ZKey
         GetIntegerFromAttribute( &lTempInteger_2, vDSR, "L_LOD_Attribute", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vLOD, "LOD_Attribute", "ZKey", lTempInteger_2, "LOD" );
         //:IF RESULT < 0
         if ( RESULT < 0 )
         { 
            //:// Since there was no match on ZKey, try to find a match on LOD Entity
            //:// name and ER Attribute name, in case the Attribute had been deleted
            //:// and recreated.  In this case, re-include the Attribute.
            //:SET CURSOR FIRST vLOD.LOD_Entity WHERE
            //:                 vLOD.LOD_Entity.Name = vDSR.L_LOD_Entity.Name
            GetStringFromAttribute( szTempString_4, sizeof( szTempString_4 ), vDSR, "L_LOD_Entity", "Name" );
            RESULT = SetCursorFirstEntityByString( vLOD, "LOD_Entity", "Name", szTempString_4, "" );
            //:IF RESULT >= zCURSOR_SET
            if ( RESULT >= zCURSOR_SET )
            { 
               //:SET CURSOR FIRST vLOD.ER_Attribute WITHIN vLOD.LOD_Entity WHERE
               //:                 vLOD.ER_Attribute.Name = vDSR.L_ER_Attribute.Name
               GetStringFromAttribute( szTempString_4, sizeof( szTempString_4 ), vDSR, "L_ER_Attribute", "Name" );
               RESULT = SetCursorFirstEntityByString( vLOD, "ER_Attribute", "Name", szTempString_4, "LOD_Entity" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:nRemapFlag = 0
                  nRemapFlag = 0;
                  //:ELSE
               } 
               else
               { 
                  //:nRemapFlag = -1
                  nRemapFlag = -1;
               } 

               //:END
               //:ELSE
            } 
            else
            { 
               //:nRemapFlag = -1
               nRemapFlag = -1;
            } 

            //:END
         } 

         //:END
         //:IF nRemapFlag = 0  // We were able to remap to an attribute by same name.
         if ( nRemapFlag == 0 )
         { 
            //:EXCLUDE vDSR.L_LOD_Attribute NONE
            RESULT = ExcludeEntity( vDSR, "L_LOD_Attribute", zREPOS_NONE );
            //:INCLUDE vDSR.L_LOD_Attribute FROM vLOD.LOD_Attribute
            RESULT = IncludeSubobjectFromSubobject( vDSR, "L_LOD_Attribute", vLOD, "LOD_Attribute", zPOS_AFTER );
            //:ELSE
         } 
         else
         { 
            //:szMsg = "Deleting List Attribute: " + vDSR.L_ER_Attribute.Name +
            //:        "|Not found in LOD: " + vLOD.LOD.Name
            GetVariableFromAttribute( szTempString_4, 0, 'S', 33, vDSR, "L_ER_Attribute", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "Deleting List Attribute: ", 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_4, 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, "Not found in LOD: ", 1, 0, 129 );
            GetVariableFromAttribute( szTempString_5, 0, 'S', 33, vLOD, "LOD", "Name", "", 0 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_5, 1, 0, 129 );
            //:MessageSend( vSubtask, "AD11012", "Dialog Spec Relink",
            //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "AD11012", "Dialog Spec Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:EXCLUDE vDSR.L_LOD_Attribute NONE
            RESULT = ExcludeEntity( vDSR, "L_LOD_Attribute", zREPOS_NONE );
         } 

         RESULT = SetCursorNextEntity( vDSR, "L_LOD_Attribute", "" );
         //:END
      } 

      //:END
   } 

   //:END

   //:IF vDSR.ListViewObjRef EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( vDSR, "ListViewObjRef" );
   if ( lTempInteger_3 == 0 )
   { 
      //:nRC = ActivateMetaOI_ByZKey( vSubtask, vVOR, 0, zREFER_VOR_META,
      //:                             zSINGLE, vDSR.ListViewObjRef.ZKey  , 0 )
      GetIntegerFromAttribute( &lTempInteger_4, vDSR, "ListViewObjRef", "ZKey" );
      nRC = ActivateMetaOI_ByZKey( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, lTempInteger_4, 0 );
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:RelinkAllSubobjectsForOI ( vDSR, "ListViewObjRef",
         //:                           vVOR, "ViewObjRef" )
         RelinkAllSubobjectsForOI( vDSR, "ListViewObjRef", vVOR, "ViewObjRef" );
         //:DropMetaOI( vSubtask, vVOR )
         DropMetaOI( vSubtask, vVOR );
         //:ELSE
      } 
      else
      { 
         //:nRC = ActivateMetaOI_ByName( vSubtask, vVOR, 0, zREFER_VOR_META,
         //:                             zSINGLE, vDSR.ListViewObjRef.Name  , 0 )
         GetStringFromAttribute( szTempString_6, sizeof( szTempString_6 ), vDSR, "ListViewObjRef", "Name" );
         nRC = ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, szTempString_6, 0 );
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:EXCLUDE vDSR.ListViewObjRef NONE
            RESULT = ExcludeEntity( vDSR, "ListViewObjRef", zREPOS_NONE );
            //:INCLUDE vDSR.ListViewObjRef FROM vVOR.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( vDSR, "ListViewObjRef", vVOR, "ViewObjRef", zPOS_AFTER );
            //:DropMetaOI( vSubtask, vVOR )
            DropMetaOI( vSubtask, vVOR );
            //:ELSE
         } 
         else
         { 
            //:szMsg = "Deleting UIS Include: " + vDSR.ListViewObjRef.Name +
            //:        "|Not found in LPLR. "
            GetVariableFromAttribute( szTempString_7, 0, 'S', 33, vDSR, "ListViewObjRef", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "Deleting UIS Include: ", 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_7, 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, "Not found in LPLR. ", 1, 0, 129 );
            //:MessageSend( vSubtask, "AD11013", "Dialog Spec Relink",
            //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "AD11013", "Dialog Spec Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:DELETE ENTITY vDSR.UIS_Include
            RESULT = DeleteEntity( vDSR, "UIS_Include", zPOS_NEXT );
         } 

         //:END
      } 

      //:END
   } 

   //:END


   //:IF vDSR.IncludeViewObjRef EXISTS
   lTempInteger_5 = CheckExistenceOfEntity( vDSR, "IncludeViewObjRef" );
   if ( lTempInteger_5 == 0 )
   { 
      //:nRC = ActivateMetaOI_ByZKey( vSubtask, vVOR, 0, zREFER_VOR_META,
      //:                             zSINGLE, vDSR.IncludeViewObjRef.ZKey  , 0 )
      GetIntegerFromAttribute( &lTempInteger_6, vDSR, "IncludeViewObjRef", "ZKey" );
      nRC = ActivateMetaOI_ByZKey( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, lTempInteger_6, 0 );
      //:IF nRC >= 0
      if ( nRC >= 0 )
      { 
         //:nRemapFlag = 0
         nRemapFlag = 0;
         //:RelinkAllSubobjectsForOI ( vDSR, "IncludeViewObjRef",
         //:                           vVOR, "ViewObjRef" )
         RelinkAllSubobjectsForOI( vDSR, "IncludeViewObjRef", vVOR, "ViewObjRef" );
         //:DropMetaOI( vSubtask, vVOR )
         DropMetaOI( vSubtask, vVOR );
         //:ELSE
      } 
      else
      { 
         //:nRC = ActivateMetaOI_ByName( vSubtask, vVOR, 0, zREFER_VOR_META,
         //:                             zSINGLE, vDSR.IncludeViewObjRef.Name  , 0 )
         GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), vDSR, "IncludeViewObjRef", "Name" );
         nRC = ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, szTempString_8, 0 );
         //:IF nRC >= 0
         if ( nRC >= 0 )
         { 
            //:EXCLUDE vDSR.IncludeViewObjRef NONE
            RESULT = ExcludeEntity( vDSR, "IncludeViewObjRef", zREPOS_NONE );
            //:INCLUDE vDSR.IncludeViewObjRef FROM vVOR.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( vDSR, "IncludeViewObjRef", vVOR, "ViewObjRef", zPOS_AFTER );
            //:nRemapFlag = 1
            nRemapFlag = 1;
            //:DropMetaOI( vSubtask, vVOR )
            DropMetaOI( vSubtask, vVOR );
            //:ELSE
         } 
         else
         { 
            //:szMsg = "Deleting UIS Include View: " + vDSR.IncludeViewObjRef.Name +
            //:        "|Not found in LPLR. "
            GetVariableFromAttribute( szTempString_9, 0, 'S', 33, vDSR, "IncludeViewObjRef", "Name", "", 0 );
            ZeidonStringCopy( szMsg, 1, 0, "Deleting UIS Include View: ", 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, szTempString_9, 1, 0, 129 );
            ZeidonStringConcat( szMsg, 1, 0, "Not found in LPLR. ", 1, 0, 129 );
            //:MessageSend( vSubtask, "AD11014", "Dialog Spec Relink",
            //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "AD11014", "Dialog Spec Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:DELETE ENTITY vDSR.UIS_Include
            RESULT = DeleteEntity( vDSR, "UIS_Include", zPOS_NEXT );
            //:nRemapFlag = -1
            nRemapFlag = -1;
         } 

         //:END
      } 

      //:END

      //:IF nRemapFlag >= 0
      if ( nRemapFlag >= 0 )
      { 
         //:// Now relink the related I_LOD_Attribute entities.
         //:ActivateMetaOI_ByZKey( vSubtask, vLOD_List, 0, zREFER_LOD_META,
         //:                       zSINGLE, vDSR.ListSubObjLOD.ZKey  , 0 )
         GetIntegerFromAttribute( &lTempInteger_7, vDSR, "ListSubObjLOD", "ZKey" );
         ActivateMetaOI_ByZKey( vSubtask, &vLOD_List, 0, zREFER_LOD_META, zSINGLE, lTempInteger_7, 0 );
         //:FOR EACH vDSR.I_LOD_Attribute
         RESULT = SetCursorFirstEntity( vDSR, "I_LOD_Attribute", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         { 
            //:nRemapFlag = 0
            nRemapFlag = 0;
            //:SET CURSOR FIRST vLOD_List.LOD_Attribute
            //:    WITHIN vLOD_List.LOD WHERE
            //:    vLOD_List.LOD_Attribute.ZKey = vDSR.I_LOD_Attribute.ZKey
            GetIntegerFromAttribute( &lTempInteger_8, vDSR, "I_LOD_Attribute", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vLOD_List, "LOD_Attribute", "ZKey", lTempInteger_8, "LOD" );
            //:IF RESULT < 0
            if ( RESULT < 0 )
            { 
               //:// Since there was no match on ZKey, try to find a match on LOD Entity
               //:// name and ER Attribute name, in case the Attribute had been deleted
               //:// and recreated.  In this case, re-include the Attribute.
               //:SET CURSOR FIRST vLOD_List.LOD_Entity WHERE
               //:                 vLOD_List.LOD_Entity.Name = vDSR.I_LOD_Entity.Name
               GetStringFromAttribute( szTempString_10, sizeof( szTempString_10 ), vDSR, "I_LOD_Entity", "Name" );
               RESULT = SetCursorFirstEntityByString( vLOD_List, "LOD_Entity", "Name", szTempString_10, "" );
               //:IF RESULT >= zCURSOR_SET
               if ( RESULT >= zCURSOR_SET )
               { 
                  //:SET CURSOR FIRST vLOD_List.ER_Attribute WITHIN vLOD_List.LOD_Entity WHERE
                  //:                 vLOD_List.ER_Attribute.Name = vDSR.I_ER_Attribute.Name
                  GetStringFromAttribute( szTempString_10, sizeof( szTempString_10 ), vDSR, "I_ER_Attribute", "Name" );
                  RESULT = SetCursorFirstEntityByString( vLOD_List, "ER_Attribute", "Name", szTempString_10, "LOD_Entity" );
                  //:IF RESULT >= zCURSOR_SET
                  if ( RESULT >= zCURSOR_SET )
                  { 
                     //:nRemapFlag = 0
                     nRemapFlag = 0;
                     //:ELSE
                  } 
                  else
                  { 
                     //:nRemapFlag = -1
                     nRemapFlag = -1;
                  } 

                  //:END
                  //:ELSE
               } 
               else
               { 
                  //:nRemapFlag = -1
                  nRemapFlag = -1;
               } 

               //:END
            } 

            //:END
            //:IF nRemapFlag = 0  // We were able to remap to an attribute by same name.
            if ( nRemapFlag == 0 )
            { 
               //:EXCLUDE vDSR.I_LOD_Attribute NONE
               RESULT = ExcludeEntity( vDSR, "I_LOD_Attribute", zREPOS_NONE );
               //:INCLUDE vDSR.I_LOD_Attribute FROM vLOD_List.LOD_Attribute
               RESULT = IncludeSubobjectFromSubobject( vDSR, "I_LOD_Attribute", vLOD_List, "LOD_Attribute", zPOS_AFTER );
               //:ELSE
            } 
            else
            { 
               //:szMsg = "Deleting Include List Attribute: |  " + vDSR.I_ER_Attribute.Name +
               //:        "|Not found in LOD: " + vLOD_List.LOD.Name
               GetVariableFromAttribute( szTempString_10, 0, 'S', 33, vDSR, "I_ER_Attribute", "Name", "", 0 );
               ZeidonStringCopy( szMsg, 1, 0, "Deleting Include List Attribute: \n ", 1, 0, 129 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_10, 1, 0, 129 );
               ZeidonStringConcat( szMsg, 1, 0, "Not found in LOD: ", 1, 0, 129 );
               GetVariableFromAttribute( szTempString_11, 0, 'S', 33, vLOD_List, "LOD", "Name", "", 0 );
               ZeidonStringConcat( szMsg, 1, 0, szTempString_11, 1, 0, 129 );
               //:MessageSend( vSubtask, "AD11015", "Dialog Spec Relink",
               //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
               MessageSend( vSubtask, "AD11015", "Dialog Spec Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
               //:EXCLUDE vDSR.I_LOD_Attribute NONE
               RESULT = ExcludeEntity( vDSR, "I_LOD_Attribute", zREPOS_NONE );
            } 

            RESULT = SetCursorNextEntity( vDSR, "I_LOD_Attribute", "" );
            //:END
         } 

         //:END
         //:DropView ( vLOD_List )
         DropView( vLOD_List );
      } 


      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DS_RelinkDelete( VIEW vDialogSpec BASED ON LOD TZADCSDO, VIEW vSubtask )

//:   VIEW        vLOD         BASED ON LOD TZZOLODO
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_DS_RelinkDelete( zVIEW     vDialogSpec,
                           zVIEW     vSubtask )
{
   zVIEW     vLOD = 0; 
   //:VIEW        vVOR         BASED ON LOD TZWDVORO
   zVIEW     vVOR = 0; 
   //:INTEGER     nRemapFlag
   zLONG     nRemapFlag = 0; 
   //:STRING(128) szMsg
   zCHAR     szMsg[ 129 ] = { 0 }; 
   //:SHORT       nRC
   zSHORT    nRC = 0; 
   zLONG     lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zLONG     lTempInteger_1; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zSHORT    lTempInteger_2; 
   zLONG     lTempInteger_3; 
   zCHAR     szTempString_4[ 33 ]; 
   zSHORT    RESULT; 


   //:// Get VOR and LOD for this Dialog Spec and relink.
   //:// Try to activate first by ZKey and then by Name.  The latter would
   //:// occur if the VOR or LOD had been deleted and readded under a new ZKey.
   //:nRC = ActivateMetaOI_ByZKey( vSubtask, vVOR, 0, zREFER_VOR_META,
   //:                             zSINGLE, vDialogSpec.UIS_ViewObjRef.ZKey , 0 )
   GetIntegerFromAttribute( &lTempInteger_0, vDialogSpec, "UIS_ViewObjRef", "ZKey" );
   nRC = ActivateMetaOI_ByZKey( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, lTempInteger_0, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:nRC = ActivateMetaOI_ByName( vSubtask, vVOR, 0, zREFER_VOR_META,
      //:                             zSINGLE, vDialogSpec.UIS_ViewObjRef.Name , 0 )
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), vDialogSpec, "UIS_ViewObjRef", "Name" );
      nRC = ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, szTempString_0, 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:szMsg = "Skipping Relink for missing View" + vDialogSpec.UIS_ViewObjRef.Name
         GetVariableFromAttribute( szTempString_1, 0, 'S', 33, vDialogSpec, "UIS_ViewObjRef", "Name", "", 0 );
         ZeidonStringCopy( szMsg, 1, 0, "Skipping Relink for missing View", 1, 0, 129 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_1, 1, 0, 129 );
         //:MessageSend( vSubtask, "AD11016", "Dialog Spec Relink",
         //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "AD11016", "Dialog Spec Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:RETURN ( -1 )
         return( ( -1 ) );
         //:ELSE
      } 
      else
      { 
         //:nRemapFlag = 1
         nRemapFlag = 1;
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:nRemapFlag = 0
      nRemapFlag = 0;
   } 

   //:END

   //:nRC = ActivateMetaOI_ByZKey( vSubtask, vLOD, 0, zREFER_LOD_META,
   //:                             zSINGLE, vDialogSpec.UIS_LOD.ZKey  , 0 )
   GetIntegerFromAttribute( &lTempInteger_1, vDialogSpec, "UIS_LOD", "ZKey" );
   nRC = ActivateMetaOI_ByZKey( vSubtask, &vLOD, 0, zREFER_LOD_META, zSINGLE, lTempInteger_1, 0 );
   //:IF nRC < 0
   if ( nRC < 0 )
   { 
      //:nRC = ActivateMetaOI_ByName( vSubtask, vLOD, 0, zREFER_LOD_META,
      //:                             zSINGLE, vDialogSpec.UIS_LOD.Name  , 0 )
      GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), vDialogSpec, "UIS_LOD", "Name" );
      nRC = ActivateMetaOI_ByName( vSubtask, &vLOD, 0, zREFER_LOD_META, zSINGLE, szTempString_2, 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:szMsg = "Skipping Relink for missing LOD" + vDialogSpec.UIS_LOD.Name
         GetVariableFromAttribute( szTempString_3, 0, 'S', 33, vDialogSpec, "UIS_LOD", "Name", "", 0 );
         ZeidonStringCopy( szMsg, 1, 0, "Skipping Relink for missing LOD", 1, 0, 129 );
         ZeidonStringConcat( szMsg, 1, 0, szTempString_3, 1, 0, 129 );
         //:MessageSend( vSubtask, "AD11017", "Dialog Spec Relink",
         //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "AD11017", "Dialog Spec Relink", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:RETURN ( -1 )
         return( ( -1 ) );
         //:ELSE
      } 
      else
      { 
         //:nRemapFlag = 1
         nRemapFlag = 1;
      } 

      //:END
      //:ELSE
   } 
   else
   { 
      //:nRemapFlag = 0
      nRemapFlag = 0;
   } 

   //:END

   //:// Only Relink if both VOR and LOD were activated by ZKey.
   //:IF nRemapFlag = 0
   if ( nRemapFlag == 0 )
   { 
      //:RelinkAllSubobjectsForOI ( vDialogSpec, "UIS_ViewObjRef",
      //:                           vVOR, "ViewObjRef" )
      RelinkAllSubobjectsForOI( vDialogSpec, "UIS_ViewObjRef", vVOR, "ViewObjRef" );
   } 

   //:END

   //:// Relink ListVOR, if it exists.
   //:IF vDialogSpec.ListVOR EXISTS
   lTempInteger_2 = CheckExistenceOfEntity( vDialogSpec, "ListVOR" );
   if ( lTempInteger_2 == 0 )
   { 
      //:nRC = ActivateMetaOI_ByZKey( vSubtask, vVOR, 0, zREFER_VOR_META,
      //:                             zSINGLE, vDialogSpec.ListVOR.ZKey , 0 )
      GetIntegerFromAttribute( &lTempInteger_3, vDialogSpec, "ListVOR", "ZKey" );
      nRC = ActivateMetaOI_ByZKey( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, lTempInteger_3, 0 );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:nRC = ActivateMetaOI_ByName( vSubtask, vVOR, 0, zREFER_VOR_META,
         //:                             zSINGLE, vDialogSpec.ListVOR.Name , 0 )
         GetStringFromAttribute( szTempString_4, sizeof( szTempString_4 ), vDialogSpec, "ListVOR", "Name" );
         nRC = ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zREFER_VOR_META, zSINGLE, szTempString_4, 0 );
         //:IF nRC < 0
         if ( nRC < 0 )
         { 
            //:EXCLUDE vDialogSpec.ListVOR NONE
            RESULT = ExcludeEntity( vDialogSpec, "ListVOR", zREPOS_NONE );
            //:ELSE
         } 
         else
         { 
            //:EXCLUDE vDialogSpec.ListVOR NONE
            RESULT = ExcludeEntity( vDialogSpec, "ListVOR", zREPOS_NONE );
            //:INCLUDE vDialogSpec.ListVOR FROM vVOR.ViewObjRef
            RESULT = IncludeSubobjectFromSubobject( vDialogSpec, "ListVOR", vVOR, "ViewObjRef", zPOS_AFTER );
         } 

         //:END
         //:ELSE
      } 
      else
      { 
         //:RelinkAllSubobjectsForOI( vDialogSpec, "ListVOR",
         //:                          vVOR, "ViewObjRef" )
         RelinkAllSubobjectsForOI( vDialogSpec, "ListVOR", vVOR, "ViewObjRef" );
      } 

      //:END
   } 

   //:END

   //:// Process each UIS_Entity recursively
   //:DS_EntityRelink( vDialogSpec, vLOD, vSubtask )
   oTZADCSDO_DS_EntityRelink( vDialogSpec, vLOD, vSubtask );

   //:// Relink the LOD_Attributes.  When RelinkInstanceToInstance has a version that
   //:// allows resetting the include flags, this code would be replaced by
   //:// RelinkInstanceToInstance calls in EntityRelinkDelete.
   //:RelinkAllSubobjectsForOI( vDialogSpec, "M_LOD_Attribute", vLOD, "LOD_Attribute" )
   RelinkAllSubobjectsForOI( vDialogSpec, "M_LOD_Attribute", vLOD, "LOD_Attribute" );
   //:RelinkAllSubobjectsForOI( vDialogSpec, "L_LOD_Attribute", vLOD, "LOD_Attribute" )
   RelinkAllSubobjectsForOI( vDialogSpec, "L_LOD_Attribute", vLOD, "LOD_Attribute" );
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:DialogSpecEntityCopy( VIEW vSubtask,
//:                      VIEW SourceLPLR BASED ON LOD TZCMLPLO,
//:                      VIEW OldDS      BASED ON LOD TZADCSDO,
//:                      VIEW OldDS_C    BASED ON LOD TZADCSDO,
//:                      VIEW NewDS      BASED ON LOD TZADCSDO,
//:                      VIEW NewDS_C    BASED ON LOD TZADCSDO,
//:                      VIEW vPE        BASED ON LOD TZPESRCO,
//:                      VIEW MapLOD     BASED ON LOD TZZOLODO )

//:   VIEW        VOR BASED ON LOD TZWDVORO
static zSHORT
oTZADCSDO_DialogSpecEntityCopy( zVIEW     vSubtask,
                                zVIEW     SourceLPLR,
                                zVIEW     OldDS,
                                zVIEW     OldDS_C,
                                zVIEW     NewDS,
                                zVIEW     NewDS_C,
                                zVIEW     vPE,
                                zVIEW     MapLOD )
{
   zVIEW     VOR = 0; 
   //:STRING (64) MG_ErrorMessage
   zCHAR     MG_ErrorMessage[ 65 ] = { 0 }; 
   //:SHORT       nRC
   zSHORT    nRC = 0; 
   zSHORT    RESULT; 
   zSHORT    lTempInteger_0; 
   zCHAR     szTempString_0[ 33 ]; 
   zSHORT    lTempInteger_1; 
   zSHORT    lTempInteger_2; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 
   zCHAR     szTempString_5[ 33 ]; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_6[ 33 ]; 
   zCHAR     szTempString_7[ 33 ]; 
   zCHAR     szTempString_8[ 33 ]; 
   zCHAR     szTempString_9[ 33 ]; 
   zCHAR     szTempString_10[ 33 ]; 
   zCHAR     szTempString_11[ 33 ]; 


   //:CreateMetaEntity( vSubtask, NewDS_C, "UIS_Entity", zPOS_AFTER )
   CreateMetaEntity( vSubtask, NewDS_C, "UIS_Entity", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewDS_C, "UIS_Entity", OldDS_C,
   //:                             "UIS_Entity", zSET_NULL )
   SetMatchingAttributesByName( NewDS_C, "UIS_Entity", OldDS_C, "UIS_Entity", zSET_NULL );

   //:// Process any UIS_ChildEntity entities recursively.
   //:FOR EACH OldDS_C.UIS_ChildEntity
   RESULT = SetCursorFirstEntity( OldDS_C, "UIS_ChildEntity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SetViewToSubobject( OldDS_C, "UIS_ChildEntity" )
      SetViewToSubobject( OldDS_C, "UIS_ChildEntity" );
      //:SetViewToSubobject( NewDS_C, "UIS_ChildEntity" )
      SetViewToSubobject( NewDS_C, "UIS_ChildEntity" );

      //:nRC = DialogSpecEntityCopy( vSubtask, SourceLPLR,
      //:                            OldDS, OldDS_C,
      //:                            NewDS, NewDS_C,
      //:                            vPE, MapLOD )
      nRC = oTZADCSDO_DialogSpecEntityCopy( vSubtask, SourceLPLR, OldDS, OldDS_C, NewDS, NewDS_C, vPE, MapLOD );

      //:IF nRC != 0
      if ( nRC != 0 )
      { 
         //:RETURN -1
         return( -1 );
      } 

      //:END

      //:ResetViewFromSubobject( OldDS_C )
      ResetViewFromSubobject( OldDS_C );
      //:ResetViewFromSubobject( NewDS_C )
      ResetViewFromSubobject( NewDS_C );
      RESULT = SetCursorNextEntity( OldDS_C, "UIS_ChildEntity", "" );
   } 

   //:END

   //:IF OldDS_C.LOD_Entity EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( OldDS_C, "LOD_Entity" );
   if ( lTempInteger_0 == 0 )
   { 
      //:SET CURSOR FIRST MapLOD.LOD_Entity WHERE
      //:    MapLOD.LOD_Entity.Name = OldDS_C.LOD_Entity.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldDS_C, "LOD_Entity", "Name" );
      RESULT = SetCursorFirstEntityByString( MapLOD, "LOD_Entity", "Name", szTempString_0, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MG_ErrorMessage = "LOD Entity ( " + OldDS_C.LOD_Entity.Name + ") not found."
         GetVariableFromAttribute( szTempString_0, 0, 'S', 33, OldDS_C, "LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( MG_ErrorMessage, 1, 0, "LOD Entity ( ", 1, 0, 65 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_0, 1, 0, 65 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") not found.", 1, 0, 65 );
         //:MessageSend( vSubtask, "AD11003", "Component Migration",
         //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "AD11003", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:ELSE
      } 
      else
      { 
         //:INCLUDE NewDS_C.LOD_Entity FROM MapLOD.LOD_Entity
         RESULT = IncludeSubobjectFromSubobject( NewDS_C, "LOD_Entity", MapLOD, "LOD_Entity", zPOS_AFTER );
      } 

      //:END
   } 

   //:END

   //:IF OldDS_C.WndDesign EXISTS
   lTempInteger_1 = CheckExistenceOfEntity( OldDS_C, "WndDesign" );
   if ( lTempInteger_1 == 0 )
   { 
      //:CreateMetaEntity( vSubtask, NewDS_C, "WndDesign", zPOS_AFTER )
      CreateMetaEntity( vSubtask, NewDS_C, "WndDesign", zPOS_AFTER );
      //:SetMatchingAttributesByName ( NewDS_C, "WndDesign", OldDS_C,
      //:                              "WndDesign", zSET_NULL )
      SetMatchingAttributesByName( NewDS_C, "WndDesign", OldDS_C, "WndDesign", zSET_NULL );
      //:IF OldDS_C.WndStyle EXISTS
      lTempInteger_2 = CheckExistenceOfEntity( OldDS_C, "WndStyle" );
      if ( lTempInteger_2 == 0 )
      { 
         //:SET CURSOR FIRST vPE.WindowStyle WHERE
         //:    vPE.WindowStyle.Tag = OldDS_C.WndStyle.Tag
         GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), OldDS_C, "WndStyle", "Tag" );
         RESULT = SetCursorFirstEntityByString( vPE, "WindowStyle", "Tag", szTempString_1, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:INCLUDE NewDS_C.WndStyle FROM vPE.WindowStyle
            RESULT = IncludeSubobjectFromSubobject( NewDS_C, "WndStyle", vPE, "WindowStyle", zPOS_AFTER );
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage ="WindowStyle doesn't exist: " + OldDS.WndStyle.Tag
            GetVariableFromAttribute( szTempString_1, 0, 'S', 33, OldDS, "WndStyle", "Tag", "", 0 );
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "WindowStyle doesn't exist: ", 1, 0, 65 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_1, 1, 0, 65 );
            //:MessageSend( vSubtask, "AD11004", "Component Migration",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "AD11004", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:RETURN -1
            return( -1 );
         } 

         //:END
      } 

      //:END
   } 

   //:END

   //:FOR EACH OldDS_C.M_LOD_Attribute
   RESULT = SetCursorFirstEntity( OldDS_C, "M_LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST MapLOD.LOD_Entity WHERE
      //:    MapLOD.LOD_Entity.Name = OldDS_C.M_LOD_Entity.Name
      GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), OldDS_C, "M_LOD_Entity", "Name" );
      RESULT = SetCursorFirstEntityByString( MapLOD, "LOD_Entity", "Name", szTempString_2, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MG_ErrorMessage = "LOD Entity ( " + OldDS_C.M_LOD_Entity.Name + ") not found."
         GetVariableFromAttribute( szTempString_2, 0, 'S', 33, OldDS_C, "M_LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( MG_ErrorMessage, 1, 0, "LOD Entity ( ", 1, 0, 65 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_2, 1, 0, 65 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") not found.", 1, 0, 65 );
         //:MessageSend( vSubtask, "AD11005", "Component Migration",
         //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "AD11005", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:ELSE
      } 
      else
      { 
         //:SET CURSOR FIRST MapLOD.ER_Attribute WITHIN MapLOD.LOD_Entity WHERE
         //:    MapLOD.ER_Attribute.Name = OldDS_C.M_ER_Attribute.Name
         GetStringFromAttribute( szTempString_3, sizeof( szTempString_3 ), OldDS_C, "M_ER_Attribute", "Name" );
         RESULT = SetCursorFirstEntityByString( MapLOD, "ER_Attribute", "Name", szTempString_3, "LOD_Entity" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:MG_ErrorMessage = "LOD Attribute ( " + OldDS_C.M_ER_Attribute.Name + ") not found."
            GetVariableFromAttribute( szTempString_3, 0, 'S', 33, OldDS_C, "M_ER_Attribute", "Name", "", 0 );
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "LOD Attribute ( ", 1, 0, 65 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_3, 1, 0, 65 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") not found.", 1, 0, 65 );
            //:MessageSend( vSubtask, "AD11006", "Component Migration",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "AD11006", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:ELSE
         } 
         else
         { 
            //:INCLUDE NewDS_C.M_LOD_Attribute FROM MapLOD.LOD_Attribute
            RESULT = IncludeSubobjectFromSubobject( NewDS_C, "M_LOD_Attribute", MapLOD, "LOD_Attribute", zPOS_AFTER );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( OldDS_C, "M_LOD_Attribute", "" );
      //:END
   } 

   //:END

   //:FOR EACH OldDS_C.L_LOD_Attribute
   RESULT = SetCursorFirstEntity( OldDS_C, "L_LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST MapLOD.LOD_Entity WHERE
      //:    MapLOD.LOD_Entity.Name = OldDS_C.L_LOD_Entity.Name
      GetStringFromAttribute( szTempString_4, sizeof( szTempString_4 ), OldDS_C, "L_LOD_Entity", "Name" );
      RESULT = SetCursorFirstEntityByString( MapLOD, "LOD_Entity", "Name", szTempString_4, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MG_ErrorMessage = "LOD Entity ( " + OldDS_C.L_LOD_Entity.Name + ") not found."
         GetVariableFromAttribute( szTempString_4, 0, 'S', 33, OldDS_C, "L_LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( MG_ErrorMessage, 1, 0, "LOD Entity ( ", 1, 0, 65 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_4, 1, 0, 65 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") not found.", 1, 0, 65 );
         //:MessageSend( vSubtask, "AD11007", "Component Migration",
         //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "AD11007", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:ELSE
      } 
      else
      { 
         //:SET CURSOR FIRST MapLOD.ER_Attribute WITHIN MapLOD.LOD_Entity WHERE
         //:    MapLOD.ER_Attribute.Name = OldDS_C.L_ER_Attribute.Name
         GetStringFromAttribute( szTempString_5, sizeof( szTempString_5 ), OldDS_C, "L_ER_Attribute", "Name" );
         RESULT = SetCursorFirstEntityByString( MapLOD, "ER_Attribute", "Name", szTempString_5, "LOD_Entity" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:MG_ErrorMessage = "LOD Attribute ( " + OldDS_C.L_ER_Attribute.Name + ") not found."
            GetVariableFromAttribute( szTempString_5, 0, 'S', 33, OldDS_C, "L_ER_Attribute", "Name", "", 0 );
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "LOD Attribute ( ", 1, 0, 65 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_5, 1, 0, 65 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") not found.", 1, 0, 65 );
            //:MessageSend( vSubtask, "AD11008", "Component Migration",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "AD11008", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:ELSE
         } 
         else
         { 
            //:INCLUDE NewDS_C.L_LOD_Attribute FROM MapLOD.LOD_Attribute
            RESULT = IncludeSubobjectFromSubobject( NewDS_C, "L_LOD_Attribute", MapLOD, "LOD_Attribute", zPOS_AFTER );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( OldDS_C, "L_LOD_Attribute", "" );
      //:END
   } 

   //:END

   //:IF OldDS_C.UIS_Include EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( OldDS_C, "UIS_Include" );
   if ( lTempInteger_3 == 0 )
   { 
      //:CreateMetaEntity( vSubtask, NewDS_C, "UIS_Include", zPOS_AFTER )
      CreateMetaEntity( vSubtask, NewDS_C, "UIS_Include", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewDS_C, "UIS_Include", OldDS_C,
      //:                             "UIS_Include", zSET_NULL )
      SetMatchingAttributesByName( NewDS_C, "UIS_Include", OldDS_C, "UIS_Include", zSET_NULL );

      //:nRC = DialogSpecGetVOR_LOD( vSubtask, OldDS,
      //:                            VOR,
      //:                            MapLOD,
      //:                            OldDS_C.ListSubObjLOD.Name,
      //:                            OldDS_C.ListViewObjRef.Name,
      //:                            "ListViewObjRef" )
      GetStringFromAttribute( szTempString_6, sizeof( szTempString_6 ), OldDS_C, "ListSubObjLOD", "Name" );
      GetStringFromAttribute( szTempString_7, sizeof( szTempString_7 ), OldDS_C, "ListViewObjRef", "Name" );
      nRC = oTZADCSDO_DialogSpecGetVOR_LOD( vSubtask, OldDS, &VOR, &MapLOD, szTempString_6, szTempString_7, "ListViewObjRef" );

      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:INCLUDE NewDS_C.ListViewObjRef FROM VOR.ViewObjRef
         RESULT = IncludeSubobjectFromSubobject( NewDS_C, "ListViewObjRef", VOR, "ViewObjRef", zPOS_AFTER );
         //:DropMetaOI( vSubtask, VOR )
         DropMetaOI( vSubtask, VOR );
         //:ELSE
      } 
      else
      { 
         //:RETURN -1
         return( -1 );
      } 

      //:END

      //:nRC = DialogSpecGetVOR_LOD( vSubtask, OldDS,
      //:                            VOR,
      //:                            MapLOD,
      //:                            OldDS_C.IncludeSubObjLOD.Name,
      //:                            OldDS_C.IncludeViewObjRef.Name,
      //:                            "IncludeViewObjRef" )
      GetStringFromAttribute( szTempString_8, sizeof( szTempString_8 ), OldDS_C, "IncludeSubObjLOD", "Name" );
      GetStringFromAttribute( szTempString_9, sizeof( szTempString_9 ), OldDS_C, "IncludeViewObjRef", "Name" );
      nRC = oTZADCSDO_DialogSpecGetVOR_LOD( vSubtask, OldDS, &VOR, &MapLOD, szTempString_8, szTempString_9, "IncludeViewObjRef" );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:INCLUDE NewDS_C.IncludeViewObjRef FROM VOR.ViewObjRef
         RESULT = IncludeSubobjectFromSubobject( NewDS_C, "IncludeViewObjRef", VOR, "ViewObjRef", zPOS_AFTER );
         //:DropMetaOI( vSubtask, VOR )
         DropMetaOI( vSubtask, VOR );
         //:ELSE
      } 
      else
      { 
         //:RETURN -1
         return( -1 );
      } 

      //:END
   } 


   //:END

   //:FOR EACH OldDS_C.I_LOD_Attribute
   RESULT = SetCursorFirstEntity( OldDS_C, "I_LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:SET CURSOR FIRST MapLOD.LOD_Entity WHERE
      //:    MapLOD.LOD_Entity.Name = OldDS_C.I_LOD_Entity.Name
      GetStringFromAttribute( szTempString_10, sizeof( szTempString_10 ), OldDS_C, "I_LOD_Entity", "Name" );
      RESULT = SetCursorFirstEntityByString( MapLOD, "LOD_Entity", "Name", szTempString_10, "" );
      //:IF RESULT < zCURSOR_SET
      if ( RESULT < zCURSOR_SET )
      { 
         //:MG_ErrorMessage = "LOD Entity ( " + OldDS_C.I_LOD_Entity.Name + ") not found."
         GetVariableFromAttribute( szTempString_10, 0, 'S', 33, OldDS_C, "I_LOD_Entity", "Name", "", 0 );
         ZeidonStringCopy( MG_ErrorMessage, 1, 0, "LOD Entity ( ", 1, 0, 65 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_10, 1, 0, 65 );
         ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") not found.", 1, 0, 65 );
         //:MessageSend( vSubtask, "AD11007", "Component Migration",
         //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "AD11007", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:ELSE
      } 
      else
      { 
         //:SET CURSOR FIRST MapLOD.ER_Attribute WITHIN MapLOD.LOD_Entity WHERE
         //:    MapLOD.ER_Attribute.Name = OldDS_C.I_ER_Attribute.Name
         GetStringFromAttribute( szTempString_11, sizeof( szTempString_11 ), OldDS_C, "I_ER_Attribute", "Name" );
         RESULT = SetCursorFirstEntityByString( MapLOD, "ER_Attribute", "Name", szTempString_11, "LOD_Entity" );
         //:IF RESULT < zCURSOR_SET
         if ( RESULT < zCURSOR_SET )
         { 
            //:MG_ErrorMessage = "LOD Attribute ( " + OldDS_C.I_ER_Attribute.Name + ") not found."
            GetVariableFromAttribute( szTempString_11, 0, 'S', 33, OldDS_C, "I_ER_Attribute", "Name", "", 0 );
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "LOD Attribute ( ", 1, 0, 65 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_11, 1, 0, 65 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") not found.", 1, 0, 65 );
            //:MessageSend( vSubtask, "AD11008", "Component Migration",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "AD11008", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:ELSE
         } 
         else
         { 
            //:INCLUDE NewDS_C.I_LOD_Attribute FROM MapLOD.LOD_Attribute
            RESULT = IncludeSubobjectFromSubobject( NewDS_C, "I_LOD_Attribute", MapLOD, "LOD_Attribute", zPOS_AFTER );
         } 

         //:END
      } 

      RESULT = SetCursorNextEntity( OldDS_C, "I_LOD_Attribute", "" );
      //:END
   } 

   //:END
   return( 0 );
// END
} 


//:LOCAL OPERATION
//:DialogSpecGetVOR_LOD( VIEW          vSubtask,
//:                      VIEW          OldDS      BASED ON LOD TZADCSDO,
//:                      VIEW          VOR        BASED ON LOD TZWDVORO,
//:                      VIEW          vLOD       BASED ON LOD TZZOLODO,
//:                      STRING ( 32 ) sLOD_Name,
//:                      STRING ( 32 ) sViewName,
//:                      STRING ( 32 ) sOrigViewEntityName )

//:   VIEW VOR_List   BASED ON LOD TZCMLPLO
static zSHORT
oTZADCSDO_DialogSpecGetVOR_LOD( zVIEW     vSubtask,
                                zVIEW     OldDS,
                                zPVIEW    VOR,
                                zPVIEW    vLOD,
                                zPCHAR    sLOD_Name,
                                zPCHAR    sViewName,
                                zPCHAR    sOrigViewEntityName )
{
   zVIEW     VOR_List = 0; 
   //:VIEW LOD_List   BASED ON LOD TZCMLPLO
   zVIEW     LOD_List = 0; 

   //:STRING (64) szMsg
   zCHAR     szMsg[ 65 ] = { 0 }; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 


   //:/*  Try to use existing VOR  */
   //:RetrieveViewForMetaList( vSubtask, VOR_List, zREFER_VOR_META )
   RetrieveViewForMetaList( vSubtask, &VOR_List, zREFER_VOR_META );
   //:SET CURSOR FIRST VOR_List.W_MetaDef
   //:   WHERE  VOR_List.W_MetaDef.Name = sViewName
   RESULT = SetCursorFirstEntityByString( VOR_List, "W_MetaDef", "Name", sViewName, "" );

   //:IF RESULT >= 0
   if ( RESULT >= 0 )
   { 

      //:ActivateMetaOI( vSubtask, VOR, VOR_List, zREFER_VOR_META, zSINGLE )
      ActivateMetaOI( vSubtask, VOR, VOR_List, zREFER_VOR_META, zSINGLE );

      //:RetrieveViewForMetaList( vSubtask, LOD_List, zREFER_LOD_META )
      RetrieveViewForMetaList( vSubtask, &LOD_List, zREFER_LOD_META );
      //:SET CURSOR FIRST LOD_List.W_MetaDef WHERE
      //:                 LOD_List.W_MetaDef.Name = VOR.LOD.Name
      GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), *VOR, "LOD", "Name" );
      RESULT = SetCursorFirstEntityByString( LOD_List, "W_MetaDef", "Name", szTempString_0, "" );
      //:ActivateMetaOI( vSubtask, vLOD, LOD_List, zREFER_LOD_META, zSINGLE )
      ActivateMetaOI( vSubtask, vLOD, LOD_List, zREFER_LOD_META, zSINGLE );
      //:ELSE
   } 
   else
   { 
      //:// Create new VOR since it doesn't already exist.  This is required for
      //:// migrating from one LPLR to another, not for cloning within the same LPLR.
      //:RetrieveViewForMetaList( vSubtask, LOD_List, zREFER_LOD_META )
      RetrieveViewForMetaList( vSubtask, &LOD_List, zREFER_LOD_META );

      //:SET CURSOR FIRST LOD_List.W_MetaDef WHERE
      //:                 LOD_List.W_MetaDef.Name = sLOD_Name
      RESULT = SetCursorFirstEntityByString( LOD_List, "W_MetaDef", "Name", sLOD_Name, "" );
      //:IF RESULT >= 0
      if ( RESULT >= 0 )
      { 
         //:ActivateEmptyMetaOI( vSubtask, VOR, zSOURCE_VOR_META, zSINGLE )
         ActivateEmptyMetaOI( vSubtask, VOR, zSOURCE_VOR_META, zSINGLE );
         //:CreateMetaEntity( vSubtask, VOR, "ViewObjRef", zPOS_AFTER )
         CreateMetaEntity( vSubtask, *VOR, "ViewObjRef", zPOS_AFTER );
         //:// The original attribute values could be in one of 3 entities
         //:SetMatchingAttributesByName( VOR, "ViewObjRef",
         //:                             OldDS, sOrigViewEntityName, zSET_NULL )
         SetMatchingAttributesByName( *VOR, "ViewObjRef", OldDS, sOrigViewEntityName, zSET_NULL );
         //:ActivateMetaOI( vSubtask, vLOD, LOD_List, zREFER_LOD_META, zSINGLE )
         ActivateMetaOI( vSubtask, vLOD, LOD_List, zREFER_LOD_META, zSINGLE );
         //:IncludeSubobjectFromSubobject( VOR, "LOD",
         //:                               vLOD, "LOD", zPOS_AFTER )
         IncludeSubobjectFromSubobject( *VOR, "LOD", *vLOD, "LOD", zPOS_AFTER );
         //:CommitMetaOI( vSubtask, VOR, zSOURCE_VOR_META )
         CommitMetaOI( vSubtask, *VOR, zSOURCE_VOR_META );

         //:ELSE
      } 
      else
      { 
         //:szMsg = "Non-existent LOD: " + sLOD_Name
         ZeidonStringCopy( szMsg, 1, 0, "Non-existent LOD: ", 1, 0, 65 );
         ZeidonStringConcat( szMsg, 1, 0, sLOD_Name, 1, 0, 65 );
         //:MessageSend( vSubtask, "AD11009", "Component Migration",
         //:             szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
         MessageSend( vSubtask, "AD11009", "Component Migration", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         //:DropView( VOR_List )
         DropView( VOR_List );
         //:DropView( LOD_List )
         DropView( LOD_List );
         //:RETURN -1
         return( -1 );
      } 

      //:END
   } 


   //:END
   //:DropView( VOR_List )
   DropView( VOR_List );
   //:DropView( LOD_List )
   DropView( LOD_List );

   //:RETURN  0
   return( 0 );
// END
} 


//:TRANSFORMATION OPERATION
//:DialogSpecMigrate( VIEW          NewDS         BASED ON LOD TZADCSDO,
//:                   STRING ( 32 ) DialogSpecName,
//:                   VIEW          SourceLPLR    BASED ON LOD TZCMLPLO,
//:                   VIEW          vSubtask )

//:// LAST MESSAGE ID: AD11017

//:   VIEW OldDS     BASED ON LOD TZADCSDO
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_DialogSpecMigrate( zPVIEW    NewDS,
                             zPCHAR    DialogSpecName,
                             zVIEW     SourceLPLR,
                             zVIEW     vSubtask )
{
   zVIEW     OldDS = 0; 
   //:VIEW OldDS_C   BASED ON LOD TZADCSDO
   zVIEW     OldDS_C = 0; 
   //:VIEW NewDS_C   BASED ON LOD TZADCSDO
   zVIEW     NewDS_C = 0; 
   //:VIEW MapVOR    BASED ON LOD TZWDVORO
   zVIEW     MapVOR = 0; 
   //:VIEW MapLOD    BASED ON LOD TZZOLODO
   zVIEW     MapLOD = 0; 
   //:VIEW vPE       BASED ON LOD TZPESRCO
   zVIEW     vPE = 0; 
   //:VIEW vMetaList BASED ON LOD TZCMLPLO
   zVIEW     vMetaList = 0; 

   //:STRING ( 513 ) SourceFileName         // size according to zMAX_FILESPEC_LTH+1
   zCHAR     SourceFileName[ 514 ] = { 0 }; 
   //:STRING ( 129 ) MG_ErrorMessage        // size is according to zSHORT_MESSAGE_LTH+1
   zCHAR     MG_ErrorMessage[ 130 ] = { 0 }; 
   //:INTEGER        nRC
   zLONG     nRC = 0; 
   zSHORT    lTempInteger_0; 
   zSHORT    lTempInteger_1; 
   zSHORT    RESULT; 
   zCHAR     szTempString_0[ 33 ]; 
   zCHAR     szTempString_1[ 33 ]; 
   zCHAR     szTempString_2[ 33 ]; 
   zLONG     lTempInteger_2; 
   zSHORT    lTempInteger_3; 
   zCHAR     szTempString_3[ 33 ]; 
   zCHAR     szTempString_4[ 33 ]; 


   //:// Activate existing source meta OldDS
   //:SourceFileName = SourceLPLR.LPLR.MetaSrcDir + "\" + DialogSpecName + ".PUI"
   GetStringFromAttribute( SourceFileName, sizeof( SourceFileName ), SourceLPLR, "LPLR", "MetaSrcDir" );
   ZeidonStringConcat( SourceFileName, 1, 0, "\\", 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, DialogSpecName, 1, 0, 514 );
   ZeidonStringConcat( SourceFileName, 1, 0, ".PUI", 1, 0, 514 );

   //:ActivateOI_FromFile( OldDS, "TZADCSDO", SourceLPLR, SourceFileName, 8192 )
   ActivateOI_FromFile( &OldDS, "TZADCSDO", SourceLPLR, SourceFileName, 8192 );
   //:// 8192 is zIGNORE_ATTRIB_ERRORS
   //:NAME VIEW OldDS "OldDS"
   SetNameForView( OldDS, "OldDS", 0, zLEVEL_TASK );

   //:ActivateEmptyMetaOI( vSubtask, NewDS, zSOURCE_UIS_META, zSINGLE )
   ActivateEmptyMetaOI( vSubtask, NewDS, zSOURCE_UIS_META, zSINGLE );
   //:NAME VIEW NewDS "NewDS"
   SetNameForView( *NewDS, "NewDS", 0, zLEVEL_TASK );

   //:nRC = GetViewByName( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK )
   nRC = GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   //:IF nRC = -1
   if ( nRC == -1 )
   { 
      //:/* Activate the presentation environment */
      //:/* RetrieveViewForMetaList( vSubtask, vMetaList, zREFER_PENV_META ) */
      //:RetrieveViewForMetaList( vSubtask, vMetaList, zREFER_PENV_META )
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );

      //:MG_ErrorMessage = "User Interface Spec (" + DialogSpecName + ") aborted."
      ZeidonStringCopy( MG_ErrorMessage, 1, 0, "User Interface Spec (", 1, 0, 130 );
      ZeidonStringConcat( MG_ErrorMessage, 1, 0, DialogSpecName, 1, 0, 130 );
      ZeidonStringConcat( MG_ErrorMessage, 1, 0, ") aborted.", 1, 0, 130 );
      //:nRC = LoadZeidonPPE( vSubtask, vPE, zREFER_PENV_META, vMetaList,
      //:                     "Configuration Management", MG_ErrorMessage )
      nRC = LoadZeidonPPE( vSubtask, &vPE, zREFER_PENV_META, vMetaList, "Configuration Management", MG_ErrorMessage );
      //:DropView( vMetaList )
      DropView( vMetaList );
      //:IF nRC < 0
      if ( nRC < 0 )
      { 
         //:RETURN -16
         return( -16 );
      } 

      //:END
      //:SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK )
      SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   } 

   //:END

   //:CreateMetaEntity( vSubtask, NewDS, "UI_Spec", zPOS_AFTER )
   CreateMetaEntity( vSubtask, *NewDS, "UI_Spec", zPOS_AFTER );
   //:SetMatchingAttributesByName( NewDS, "UI_Spec", OldDS,
   //:                             "UI_Spec", zSET_NULL )
   SetMatchingAttributesByName( *NewDS, "UI_Spec", OldDS, "UI_Spec", zSET_NULL );

   //:IF OldDS.DfltWndDesign EXISTS
   lTempInteger_0 = CheckExistenceOfEntity( OldDS, "DfltWndDesign" );
   if ( lTempInteger_0 == 0 )
   { 
      //:CreateMetaEntity( vSubtask, NewDS, "DfltWndDesign", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewDS, "DfltWndDesign", zPOS_AFTER );
      //: SetMatchingAttributesByName( NewDS, "DfltWndDesign", OldDS,
      //:                              "DfltWndDesign", zSET_NULL )
      SetMatchingAttributesByName( *NewDS, "DfltWndDesign", OldDS, "DfltWndDesign", zSET_NULL );
      //: IF OldDS.DfltWndStyle EXISTS
      lTempInteger_1 = CheckExistenceOfEntity( OldDS, "DfltWndStyle" );
      if ( lTempInteger_1 == 0 )
      { 
         //:SET CURSOR FIRST vPE.WindowStyle WHERE
         //:    vPE.WindowStyle.Tag = OldDS.DfltWndStyle.Tag
         GetStringFromAttribute( szTempString_0, sizeof( szTempString_0 ), OldDS, "DfltWndStyle", "Tag" );
         RESULT = SetCursorFirstEntityByString( vPE, "WindowStyle", "Tag", szTempString_0, "" );
         //:IF RESULT >= zCURSOR_SET
         if ( RESULT >= zCURSOR_SET )
         { 
            //:INCLUDE NewDS.DfltWndStyle FROM vPE.WindowStyle
            RESULT = IncludeSubobjectFromSubobject( *NewDS, "DfltWndStyle", vPE, "WindowStyle", zPOS_AFTER );
            //:ELSE
         } 
         else
         { 
            //:MG_ErrorMessage ="WindowStyle doesn't exist: " + OldDS.DfltWndStyle.Tag
            GetVariableFromAttribute( szTempString_0, 0, 'S', 33, OldDS, "DfltWndStyle", "Tag", "", 0 );
            ZeidonStringCopy( MG_ErrorMessage, 1, 0, "WindowStyle doesn't exist: ", 1, 0, 130 );
            ZeidonStringConcat( MG_ErrorMessage, 1, 0, szTempString_0, 1, 0, 130 );
            //:MessageSend( vSubtask,  "AD11002", "Component Migration",
            //:             MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 )
            MessageSend( vSubtask, "AD11002", "Component Migration", MG_ErrorMessage, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            //:RETURN -1
            return( -1 );
         } 

         //:END
      } 

      //: END
   } 

   //:END

   //:FOR EACH OldDS.UIS_QualList
   RESULT = SetCursorFirstEntity( OldDS, "UIS_QualList", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:CreateMetaEntity( vSubtask, NewDS, "UIS_QualList", zPOS_AFTER )
      CreateMetaEntity( vSubtask, *NewDS, "UIS_QualList", zPOS_AFTER );
      //:SetMatchingAttributesByName( NewDS, "UIS_QualList", OldDS,
      //:                             "UIS_QualList", zSET_NULL )
      SetMatchingAttributesByName( *NewDS, "UIS_QualList", OldDS, "UIS_QualList", zSET_NULL );
      RESULT = SetCursorNextEntity( OldDS, "UIS_QualList", "" );
   } 

   //:END

   //:FOR EACH OldDS.UIS_ViewObjRef
   RESULT = SetCursorFirstEntity( OldDS, "UIS_ViewObjRef", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   { 
      //:nRC = DialogSpecGetVOR_LOD( vSubtask, OldDS,
      //:                            MapVOR,
      //:                            MapLOD,
      //:                            OldDS.UIS_LOD.Name,
      //:                            OldDS.UIS_ViewObjRef.Name,
      //:                            "UIS_ViewObjRef" )
      GetStringFromAttribute( szTempString_1, sizeof( szTempString_1 ), OldDS, "UIS_LOD", "Name" );
      GetStringFromAttribute( szTempString_2, sizeof( szTempString_2 ), OldDS, "UIS_ViewObjRef", "Name" );
      nRC = oTZADCSDO_DialogSpecGetVOR_LOD( vSubtask, OldDS, &MapVOR, &MapLOD, szTempString_1, szTempString_2, "UIS_ViewObjRef" );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:INCLUDE NewDS.UIS_ViewObjRef FROM MapVOR.ViewObjRef
         RESULT = IncludeSubobjectFromSubobject( *NewDS, "UIS_ViewObjRef", MapVOR, "ViewObjRef", zPOS_AFTER );
         //:DropMetaOI( vSubtask, MapVOR )
         DropMetaOI( vSubtask, MapVOR );
         //:DropMetaOI( vSubtask, MapLOD )
         DropMetaOI( vSubtask, MapLOD );
         //:ELSE
      } 
      else
      { 
         //:RETURN -1
         return( -1 );
      } 

      RESULT = SetCursorNextEntity( OldDS, "UIS_ViewObjRef", "" );
      //:END
   } 

   //:END

   //:// Reposition on first of ViewObjRef entities and get the LOD again,
   //:// since we use the first LOD later on.
   //:SET CURSOR FIRST NewDS.UIS_ViewObjRef
   RESULT = SetCursorFirstEntity( *NewDS, "UIS_ViewObjRef", "" );
   //:ActivateMetaOI_ByZKey( vSubtask, MapLOD, 0, zREFER_LOD_META,
   //:                       zSINGLE, NewDS.UIS_LOD.ZKey, 0 )
   GetIntegerFromAttribute( &lTempInteger_2, *NewDS, "UIS_LOD", "ZKey" );
   ActivateMetaOI_ByZKey( vSubtask, &MapLOD, 0, zREFER_LOD_META, zSINGLE, lTempInteger_2, 0 );

   //:IF OldDS.ListVOR EXISTS
   lTempInteger_3 = CheckExistenceOfEntity( OldDS, "ListVOR" );
   if ( lTempInteger_3 == 0 )
   { 
      //:nRC = DialogSpecGetVOR_LOD( vSubtask, OldDS,
      //:                            MapVOR,
      //:                            MapLOD,
      //:                            OldDS.ListLOD.Name,
      //:                            OldDS.ListVOR.Name,
      //:                            "UIS_ViewObjRef" )
      GetStringFromAttribute( szTempString_3, sizeof( szTempString_3 ), OldDS, "ListLOD", "Name" );
      GetStringFromAttribute( szTempString_4, sizeof( szTempString_4 ), OldDS, "ListVOR", "Name" );
      nRC = oTZADCSDO_DialogSpecGetVOR_LOD( vSubtask, OldDS, &MapVOR, &MapLOD, szTempString_3, szTempString_4, "UIS_ViewObjRef" );
      //:IF nRC = 0
      if ( nRC == 0 )
      { 
         //:INCLUDE NewDS.ListVOR FROM MapVOR.ViewObjRef
         RESULT = IncludeSubobjectFromSubobject( *NewDS, "ListVOR", MapVOR, "ViewObjRef", zPOS_AFTER );
         //:DropMetaOI( vSubtask, MapVOR )
         DropMetaOI( vSubtask, MapVOR );
         //:ELSE
      } 
      else
      { 
         //:RETURN -1
         return( -1 );
      } 

      //:END
   } 

   //:END

   //:CreateViewFromViewForTask( NewDS_C, NewDS, 0 )
   CreateViewFromViewForTask( &NewDS_C, *NewDS, 0 );
   //:CreateViewFromViewForTask( OldDS_C, OldDS, 0 )
   CreateViewFromViewForTask( &OldDS_C, OldDS, 0 );
   //:NAME VIEW NewDS_C "NewDS_C"
   SetNameForView( NewDS_C, "NewDS_C", 0, zLEVEL_TASK );
   //:NAME VIEW OldDS_C "OldDS_C"
   SetNameForView( OldDS_C, "OldDS_C", 0, zLEVEL_TASK );

   //:// Create recursive leg of object.
   //:DialogSpecEntityCopy( vSubtask, SourceLPLR,
   //:                      OldDS, OldDS_C,
   //:                      NewDS, NewDS_C,
   //:                      vPE, MapLOD )
   oTZADCSDO_DialogSpecEntityCopy( vSubtask, SourceLPLR, OldDS, OldDS_C, *NewDS, NewDS_C, vPE, MapLOD );

   //:CommitMetaOI( vSubtask, NewDS, zSOURCE_UIS_META )
   CommitMetaOI( vSubtask, *NewDS, zSOURCE_UIS_META );
   //:DropView( NewDS_C )
   DropView( NewDS_C );
   //:DropObjectInstance ( OldDS )
   DropObjectInstance( OldDS );
   //:DropMetaOI( vSubtask, MapLOD )
   DropMetaOI( vSubtask, MapLOD );

   //:RETURN 0
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
