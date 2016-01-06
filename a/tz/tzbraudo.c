#define KZSYSSVC_INCL
#include "KZOENGAA.H" 
#include "ZDRVROPR.H" 
 
#ifdef __cplusplus
extern "C"
{
#endif
 
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
oTZBRAUDO_DeleteText( zVIEW     ViewtoInstance,
                      LPVIEWENTITY InternalEntityStructure,
                      LPVIEWATTRIB InternalAttribStructure,
                      zSHORT    GetOrSetFlag );


//:DERIVED ATTRIBUTE OPERATION
//:DeleteText( VIEW ViewtoInstance BASED ON LOD TZBRAUDO,
//:            STRING ( 32 ) InternalEntityStructure,
//:            STRING ( 32 ) InternalAttribStructure,
//:            SHORT GetOrSetFlag )

//:   STRING(60)  szText
zOPER_EXPORT zSHORT OPERATION
oTZBRAUDO_DeleteText( zVIEW     ViewtoInstance,
                      LPVIEWENTITY InternalEntityStructure,
                      LPVIEWATTRIB InternalAttribStructure,
                      zSHORT    GetOrSetFlag )
{
   zCHAR     szText[ 61 ] = { 0 }; 


   //:CASE GetOrSetFlag
   switch( GetOrSetFlag )
   { 
      //:OF   zDERIVED_GET:
      case zDERIVED_GET :
         //: IF ViewtoInstance.AuditTrailMeta.bDeleted = "Y"
         if ( CompareAttributeToString( ViewtoInstance, "AuditTrailMeta", "bDeleted", "Y" ) == 0 )
         { 
            //: szText = "Deleted"
            ZeidonStringCopy( szText, 1, 0, "Deleted", 1, 0, 61 );
            //:ELSE
         } 
         else
         { 
            //: IF ViewtoInstance.AuditTrailMeta.bDeleted = "N"
            if ( CompareAttributeToString( ViewtoInstance, "AuditTrailMeta", "bDeleted", "N" ) == 0 )
            { 
               //: szText = "Updated"
               ZeidonStringCopy( szText, 1, 0, "Updated", 1, 0, 61 );
               //:ELSE
            } 
            else
            { 
               //: szText = ""
               ZeidonStringCopy( szText, 1, 0, "", 1, 0, 61 );
            } 

            //: END
         } 

         //: END
         //:StoreValueInRecord( ViewtoInstance,
         //:                   InternalEntityStructure,
         //:                   InternalAttribStructure,
         //:                    szText, 0 )
         StoreValueInRecord( ViewtoInstance, InternalEntityStructure, InternalAttribStructure, szText, 0 );
         break ;

      //:  /* end zDERIVED_GET */
      //:OF   zDERIVED_SET:
      case zDERIVED_SET :
         break ;
   } 


   //:     /* end zDERIVED_SET */
   //:END  /* case */
   return( 0 );
// END
} 


 
#ifdef __cplusplus
}
#endif
