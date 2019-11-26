/*
//----------------------------------------------------------------------
//
// .Name:         TZRBLDMT.c
//
// .Version:      1.0
//
// .Last change:  25-01-2019
//
// .Description:  Zeidon operations
//
//----------------------------------------------------------------------
// .Change notes
//
//  1.0  = New
//        (25-01-2019) Zeidon
//
//----------------------------------------------------------------------
*/

#define zGLOBAL_DATA
#include "..\..\a\oe\kzoephdr.h"   // added to get "..\oe\kzoengaa.hg" to work
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "ZEIDONOP.H"



zOPER_EXPORT zSHORT OPERATION
zwTZRBLDMT_RebuildMetaLists( zVIEW vSubtask )
{
   zSHORT nRC ;

   if ( MessagePrompt( vSubtask, "CM00609",
                       "Configuration Management",
                       "Do you want to rebuild your Project?",
                       zBEEP,
                       zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      return( 0 );
   }
   
   if ( RebuildMetaLists( vSubtask ) == 0 )
   {
      MessageSend( vSubtask, "CM00608", "Configuration Management",
                   "Your project has been rebuilt. ",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }

   return( 0 );
}

