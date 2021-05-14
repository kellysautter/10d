#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

//:GLOBAL OPERATION
//:ComputeQueryOperationValue( VIEW vReportDef BASED ON LOD TZRPSRCO,
//:                            VIEW vQueryObject,
//:                            STRING ( 20 ) ReturnedString,
//:                            STRING ( 32 ) OperationName,
//:                            STRING ( 32 ) szEntityName,
//:                            STRING ( 32 ) szAttributeName )

//:   // Perform the group operation of:
//:   //   SUM
//:   //   AVG
//:   //   COUNT
//:   //   MAX
//:   //   MIN
//:   // on the Entity/Attribute passed and return the value as a decimal.

//:   DECIMAL Sum
zOPER_EXPORT zSHORT OPERATION
ComputeQueryOperationValue( zVIEW     vReportDef,
                            zVIEW     vQueryObject,
                            zPCHAR    ReturnedString,
                            zPCHAR    OperationName,
                            zPCHAR    szEntityName,
                            zPCHAR    szAttributeName )
{
   ZDecimal  Sum = 0.0; 
   //:DECIMAL Max
   ZDecimal  Max = 0.0; 
   //:DECIMAL Min
   ZDecimal  Min = 0.0; 
   //:DECIMAL Average
   ZDecimal  Average = 0.0; 
   //:DECIMAL AttributeValue
   ZDecimal  AttributeValue = 0.0; 
   //:DECIMAL ReturnedValue
   ZDecimal  ReturnedValue = 0.0; 
   //:INTEGER Count
   zLONG     Count = 0; 
   //:SHORT   nRC
   zSHORT    nRC = 0; 
   //:STRING ( 20 ) MaxString
   zCHAR     MaxString[ 21 ] = { 0 }; 
   //:STRING ( 20 ) MinString
   zCHAR     MinString[ 21 ] = { 0 }; 
   //:STRING ( 20 ) AttributeValueString
   zCHAR     AttributeValueString[ 21 ] = { 0 }; 
   //:STRING ( 32 ) szScopingEntity
   zCHAR     szScopingEntity[ 33 ] = { 0 }; 


   //:// The ScopingEntity is the Entity for the Group Set on which this Control exists.
   //:IF vReportDef.GroupSet.Type = "E"
   if ( CompareAttributeToString( vReportDef, "GroupSet", "Type", "E" ) == 0 )
   { 
      //:szScopingEntity = vReportDef.GroupSet.Tag
      GetVariableFromAttribute( szScopingEntity, 0, 'S', 33, vReportDef, "GroupSet", "Tag", "", 0 );
      //:ELSE
   } 
   else
   { 
      //:szScopingEntity = ""
      ZeidonStringCopy( szScopingEntity, 1, 0, "", 1, 0, 33 );
   } 

   //:END

   //:IF OperationName = "MAX"
   if ( ZeidonStringCompare( OperationName, 1, 0, "MAX", 1, 0, 33 ) == 0 )
   { 
      //:IF vReportDef.CtrlMapER_Domain.DataType = "L" OR
      //:   vReportDef.CtrlMapER_Domain.DataType = "M"
      if ( CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "L" ) == 0 || CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "M" ) == 0 )
      { 

         //:nRC = SetCursorFirstEntity( vQueryObject,
         //:                            szEntityName,
         //:                            szScopingEntity )
         nRC = SetCursorFirstEntity( vQueryObject, szEntityName, szScopingEntity );
         //:IF nRC >= zCURSOR_SET
         if ( nRC >= zCURSOR_SET )
         { 
            //:GetDecimalFromAttribute( Max,
            //:                         vQueryObject,
            //:                         szEntityName,
            //:                         szAttributeName )
            GetDecimalFromAttribute( &Max, vQueryObject, szEntityName, szAttributeName );
            //:LOOP WHILE nRC >= zCURSOR_SET
            while ( nRC >= zCURSOR_SET )
            { 
               //:nRC = SetCursorNextEntity( vQueryObject,
               //:                           szEntityName,
               //:                           szScopingEntity )
               nRC = SetCursorNextEntity( vQueryObject, szEntityName, szScopingEntity );
               //:IF nRC >= zCURSOR_SET
               if ( nRC >= zCURSOR_SET )
               { 
                  //:GetDecimalFromAttribute( AttributeValue,
                  //:                         vQueryObject,
                  //:                         szEntityName,
                  //:                         szAttributeName )
                  GetDecimalFromAttribute( &AttributeValue, vQueryObject, szEntityName, szAttributeName );

                  //:IF AttributeValue > Max
                  if ( AttributeValue > Max )
                  { 
                     //:Max = AttributeValue
                     Max = AttributeValue;
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
            //:Max = 0
            Max = 0;
         } 

         //:END
         //:ReturnedValue = Max
         ReturnedValue = Max;
         //:ELSE
      } 
      else
      { 
         //:nRC = SetCursorFirstEntity( vQueryObject,
         //:                            szEntityName,
         //:                            szScopingEntity )
         nRC = SetCursorFirstEntity( vQueryObject, szEntityName, szScopingEntity );
         //:IF nRC >= zCURSOR_SET
         if ( nRC >= zCURSOR_SET )
         { 
            //:GetStringFromAttribute( MaxString,
            //:                         vQueryObject,
            //:                         szEntityName,
            //:                         szAttributeName )
            GetStringFromAttribute( MaxString, zsizeof( MaxString ), vQueryObject, szEntityName, szAttributeName );
            //:LOOP WHILE nRC >= zCURSOR_SET
            while ( nRC >= zCURSOR_SET )
            { 
               //:nRC = SetCursorNextEntity( vQueryObject,
               //:                           szEntityName,
               //:                           szScopingEntity )
               nRC = SetCursorNextEntity( vQueryObject, szEntityName, szScopingEntity );
               //:IF nRC >= zCURSOR_SET
               if ( nRC >= zCURSOR_SET )
               { 
                  //:GetStringFromAttribute( AttributeValueString,
                  //:                         vQueryObject,
                  //:                         szEntityName,
                  //:                         szAttributeName )
                  GetStringFromAttribute( AttributeValueString, zsizeof( AttributeValueString ), vQueryObject, szEntityName, szAttributeName );
                  //:IF AttributeValueString > MaxString
                  if ( ZeidonStringCompare( AttributeValueString, 1, 0, MaxString, 1, 0, 21 ) < 0 )
                  { 
                     //:MaxString = AttributeValueString
                     ZeidonStringCopy( MaxString, 1, 0, AttributeValueString, 1, 0, 21 );
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
            //:MaxString = ""
            ZeidonStringCopy( MaxString, 1, 0, "", 1, 0, 21 );
         } 

         //:END

         //:IF vReportDef.CtrlMapER_Domain.DataType = "D"
         if ( CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "D" ) == 0 )
         { 
            //:vReportDef.Report.wFormatDate = MaxString
            SetAttributeFromString( vReportDef, "Report", "wFormatDate", MaxString );
            //:GetStrFromAttrByContext( ReturnedString,
            //:                         20,
            //:                         vReportDef, "Report", "wFormatDate", "" )
            GetStrFromAttrByContext( ReturnedString, 20, vReportDef, "Report", "wFormatDate", "" );
            //:ELSE
         } 
         else
         { 
            //:IF vReportDef.CtrlMapER_Domain.DataType = "T"
            if ( CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "T" ) == 0 )
            { 
               //:vReportDef.Report.wFormatDateTime = MaxString
               SetAttributeFromString( vReportDef, "Report", "wFormatDateTime", MaxString );
               //:GetStrFromAttrByContext( ReturnedString,
               //:                         20,
               //:                         vReportDef, "Report", "wFormatDateTime", "" )
               GetStrFromAttrByContext( ReturnedString, 20, vReportDef, "Report", "wFormatDateTime", "" );
               //:ELSE
            } 
            else
            { 
               //:ReturnedString = MaxString
               ZeidonStringCopy( ReturnedString, 1, 0, MaxString, 1, 0, 21 );
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
      //:IF OperationName = "MIN"
      if ( ZeidonStringCompare( OperationName, 1, 0, "MIN", 1, 0, 33 ) == 0 )
      { 
         //:IF vReportDef.CtrlMapER_Domain.DataType = "L" OR
         //:vReportDef.CtrlMapER_Domain.DataType = "M"
         if ( CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "L" ) == 0 || CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "M" ) == 0 )
         { 

            //:nRC = SetCursorFirstEntity( vQueryObject,
            //:                         szEntityName,
            //:                         szScopingEntity )
            nRC = SetCursorFirstEntity( vQueryObject, szEntityName, szScopingEntity );
            //:IF nRC >= zCURSOR_SET
            if ( nRC >= zCURSOR_SET )
            { 
               //:GetDecimalFromAttribute( Min,
               //:                      vQueryObject,
               //:                      szEntityName,
               //:                      szAttributeName )
               GetDecimalFromAttribute( &Min, vQueryObject, szEntityName, szAttributeName );
               //:LOOP WHILE nRC >= zCURSOR_SET
               while ( nRC >= zCURSOR_SET )
               { 
                  //:nRC = SetCursorNextEntity( vQueryObject,
                  //:                        szEntityName,
                  //:                        szScopingEntity )
                  nRC = SetCursorNextEntity( vQueryObject, szEntityName, szScopingEntity );
                  //:IF nRC >= zCURSOR_SET
                  if ( nRC >= zCURSOR_SET )
                  { 
                     //:GetDecimalFromAttribute( AttributeValue,
                     //:                      vQueryObject,
                     //:                      szEntityName,
                     //:                      szAttributeName )
                     GetDecimalFromAttribute( &AttributeValue, vQueryObject, szEntityName, szAttributeName );

                     //:IF AttributeValue < Min
                     if ( AttributeValue < Min )
                     { 
                        //:Min = AttributeValue
                        Min = AttributeValue;
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
               //:Min = 0
               Min = 0;
            } 

            //:END
            //:ReturnedValue = Min
            ReturnedValue = Min;
            //:ELSE
         } 
         else
         { 
            //:nRC = SetCursorFirstEntity( vQueryObject,
            //:                         szEntityName,
            //:                         szScopingEntity )
            nRC = SetCursorFirstEntity( vQueryObject, szEntityName, szScopingEntity );
            //:IF nRC >= zCURSOR_SET
            if ( nRC >= zCURSOR_SET )
            { 
               //:GetStringFromAttribute( MinString,
               //:                     vQueryObject,
               //:                     szEntityName,
               //:                     szAttributeName )
               GetStringFromAttribute( MinString, zsizeof( MinString ), vQueryObject, szEntityName, szAttributeName );
               //:LOOP WHILE nRC >= zCURSOR_SET
               while ( nRC >= zCURSOR_SET )
               { 
                  //:nRC = SetCursorNextEntity( vQueryObject,
                  //:                        szEntityName,
                  //:                        szScopingEntity )
                  nRC = SetCursorNextEntity( vQueryObject, szEntityName, szScopingEntity );
                  //:IF nRC >= zCURSOR_SET
                  if ( nRC >= zCURSOR_SET )
                  { 
                     //:GetStringFromAttribute( AttributeValueString,
                     //:                     vQueryObject,
                     //:                     szEntityName,
                     //:                     szAttributeName )
                     GetStringFromAttribute( AttributeValueString, zsizeof( AttributeValueString ), vQueryObject, szEntityName, szAttributeName );
                     //:IF AttributeValueString < MinString
                     if ( ZeidonStringCompare( AttributeValueString, 1, 0, MinString, 1, 0, 21 ) > 0 )
                     { 
                        //:MinString = AttributeValueString
                        ZeidonStringCopy( MinString, 1, 0, AttributeValueString, 1, 0, 21 );
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
               //:MinString = ""
               ZeidonStringCopy( MinString, 1, 0, "", 1, 0, 21 );
            } 

            //:END
            //:ReturnedString = MinString
            ZeidonStringCopy( ReturnedString, 1, 0, MinString, 1, 0, 21 );
            //:IF vReportDef.CtrlMapER_Domain.DataType = "D"
            if ( CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "D" ) == 0 )
            { 
               //:vReportDef.Report.wFormatDate = MinString
               SetAttributeFromString( vReportDef, "Report", "wFormatDate", MinString );
               //:GetStrFromAttrByContext( ReturnedString,
               //:                      20,
               //:                      vReportDef, "Report", "wFormatDate", "" )
               GetStrFromAttrByContext( ReturnedString, 20, vReportDef, "Report", "wFormatDate", "" );
               //:ELSE
            } 
            else
            { 
               //:IF vReportDef.CtrlMapER_Domain.DataType = "T"
               if ( CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "T" ) == 0 )
               { 
                  //:vReportDef.Report.wFormatDateTime = MinString
                  SetAttributeFromString( vReportDef, "Report", "wFormatDateTime", MinString );
                  //:GetStrFromAttrByContext( ReturnedString,
                  //:                      20,
                  //:                      vReportDef, "Report", "wFormatDateTime", "" )
                  GetStrFromAttrByContext( ReturnedString, 20, vReportDef, "Report", "wFormatDateTime", "" );
                  //:ELSE
               } 
               else
               { 
                  //:ReturnedString = MinString
                  ZeidonStringCopy( ReturnedString, 1, 0, MinString, 1, 0, 21 );
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
         //:IF OperationName = "COUNT"
         if ( ZeidonStringCompare( OperationName, 1, 0, "COUNT", 1, 0, 33 ) == 0 )
         { 
            //:Count = 0
            Count = 0;
            //:nRC = SetCursorFirstEntity( vQueryObject,
            //:                      szEntityName,
            //:                      szScopingEntity )
            nRC = SetCursorFirstEntity( vQueryObject, szEntityName, szScopingEntity );
            //:LOOP WHILE nRC >= zCURSOR_SET
            while ( nRC >= zCURSOR_SET )
            { 
               //:Count = Count + 1
               Count = Count + 1;
               //:nRC = SetCursorNextEntity( vQueryObject,
               //:                     szEntityName,
               //:                     szScopingEntity )
               nRC = SetCursorNextEntity( vQueryObject, szEntityName, szScopingEntity );
            } 

            //:END
            //:ReturnedValue = Count
            ReturnedValue = Count;
            //:ELSE
         } 
         else
         { 
            //:IF OperationName = "SUM" OR OperationName = "AVG"
            if ( ZeidonStringCompare( OperationName, 1, 0, "SUM", 1, 0, 33 ) == 0 || ZeidonStringCompare( OperationName, 1, 0, "AVG", 1, 0, 33 ) == 0 )
            { 
               //:Count = 0
               Count = 0;
               //:Sum   = 0
               Sum = 0;
               //:nRC = SetCursorFirstEntity( vQueryObject,
               //:                   szEntityName,
               //:                   szScopingEntity )
               nRC = SetCursorFirstEntity( vQueryObject, szEntityName, szScopingEntity );
               //:LOOP WHILE nRC >= zCURSOR_SET
               while ( nRC >= zCURSOR_SET )
               { 
                  //:GetDecimalFromAttribute( AttributeValue,
                  //:                vQueryObject,
                  //:                szEntityName,
                  //:                szAttributeName )
                  GetDecimalFromAttribute( &AttributeValue, vQueryObject, szEntityName, szAttributeName );

                  //:Count = Count + 1
                  Count = Count + 1;
                  //:Sum = Sum + AttributeValue
                  Sum = Sum + AttributeValue;
                  //:nRC = SetCursorNextEntity( vQueryObject,
                  //:                  szEntityName,
                  //:                  szScopingEntity )
                  nRC = SetCursorNextEntity( vQueryObject, szEntityName, szScopingEntity );
               } 

               //:END
               //:IF OperationName = "AVG"
               if ( ZeidonStringCompare( OperationName, 1, 0, "AVG", 1, 0, 33 ) == 0 )
               { 
                  //:IF Count = 0
                  if ( Count == 0 )
                  { 
                     //:ReturnedValue = 0
                     ReturnedValue = 0;
                     //:ELSE
                  } 
                  else
                  { 
                     //:ReturnedValue = Sum / Count
                     ReturnedValue = Sum / Count;
                  } 

                  //:END
                  //:ELSE
               } 
               else
               { 
                  //:ReturnedValue = Sum
                  ReturnedValue = Sum;
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

   //:// Set values.
   //:IF vReportDef.CtrlMapER_Domain.DataType = "L" OR
   //:   vReportDef.CtrlMapER_Domain.DataType = "M"
   if ( CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "L" ) == 0 || CompareAttributeToString( vReportDef, "CtrlMapER_Domain", "DataType", "M" ) == 0 )
   { 

      //:vReportDef.Report.wFormatQueryDecimal = ReturnedValue
      SetAttributeFromDecimal( vReportDef, "Report", "wFormatQueryDecimal", ReturnedValue );
      //:GetStringFromAttributeByContext( ReturnedString,
      //:                                 vReportDef,
      //:                                 "Report",
      //:                                 "wFormatQueryDecimal",
      //:                                 "", 20 )
      GetStringFromAttributeByContext( ReturnedString, vReportDef, "Report", "wFormatQueryDecimal", "", 20 );
   } 

   //:END
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
