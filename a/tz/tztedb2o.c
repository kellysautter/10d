#define zGLOBAL_DATA
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "ZeidonOp.H"

zOPER_EXPORT zSHORT OPERATION
LoadDataTypes( zVIEW vSubtask )
{
   zVIEW     vTZTEDBLO;
   LPLIBRARY hLibrary;
   zSHORT    (POPERATION pfn) ( zVIEW );
   zCHAR     szFileName[ zMAX_FILENAME_LTH + 1 ];

   GetViewByName( &vTZTEDBLO, "TZTEDBLO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTZTEDBLO,
                           "TE_DBMS_Source", "GenerateExecutable" );
   hLibrary = SysLoadLibrary( vSubtask, szFileName );
   if ( hLibrary )
   {
      pfn = SysGetProc( hLibrary, "LoadDataTypes" );
      if ( pfn )
      {
         zSHORT nRC;

         // First delete the data types that are already there.
         for ( nRC = SetCursorFirstEntity( vTZTEDBLO, "DB_DataTypes", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vTZTEDBLO, "DB_DataTypes", 0 ) )
         {
            DeleteEntity( vTZTEDBLO, "DB_DataTypes", zREPOS_NONE );
         }

         (*pfn)( vTZTEDBLO );
      }
      else
         MessageSend( vSubtask, "TE00422", "Technical Environment",
                      "Couldn't find 'LoadDataTypes' in Generater Executable",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      SysFreeLibrary( vSubtask, hLibrary );
   }
   else
      MessageSend( vSubtask, "TE00421", "Technical Environment",
                   "Couldn't load Generater Executable",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   return( 0 );
}
