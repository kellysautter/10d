//----------------------------------------------------------------------
//
// .Name:         TZPNCW2D.c
//
// .Version:      1.0
//
// .Last change:  16-09-2000
//
// .Description:  Zeidon operations
//
//----------------------------------------------------------------------
// CHANGE LOG
//
// 2000.09.16  BL
//   Create this File for call a Dialog Operation from an other Dialog
//

#define KZSYSSVC_INCL
#include <KZOENGAA.H>
#include <ZDRVROPR.H>
#ifdef __cplusplus
extern "C"
{
#endif

#include "tz__oprs.h"
#include "ZEIDONOP.H"


zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_DropOperationFromSource( zVIEW  vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_CopyOperationCode( zVIEW vSubtask,
                            zSHORT nRenameOperation );

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPNCW2D_CopyOperationToFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPNCW2D_CopyOperationToFile( zVIEW  vSubtask,
                                zSHORT nCopyOrDeleteFlag )
{
   zVIEW  vProfileXFER;
   zVIEW  vTaskLPLR;
   zVIEW  vEdWrk;
   zVIEW  vDialog;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEdWrk, "TZEDWRKO", vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   SetNameForView( vDialog, "METAOPERATIONS", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vProfileXFER, "ED", "InvokingTool", "GO" );

   if ( vEdWrk == 0 )
   {
      ActivateEmptyObjectInstance( &vEdWrk, "TZEDWRKO", vTaskLPLR, zSINGLE );

      // Create empty root.
      CreateEntity( vEdWrk, "Editor", zPOS_FIRST );
      SetNameForView( vEdWrk, "TZEDWRKO", vSubtask, zLEVEL_SUBTASK );
   }

   SetAttributeFromString( vProfileXFER, "ED", "InvokeEditorCommand", "S" );

   if ( nCopyOrDeleteFlag == zDELETE_OPERATION )
   {
      // Delete Operation from File
      TZEDFRMD_DropOperationFromSource( vSubtask );
   }
   else
   if ( nCopyOrDeleteFlag == zCOPY_OPERATION )
   {
      // Copy Operation from File to File
      TZEDFRMD_CopyOperationCode( vSubtask, zNORENAME_OPERATION );
   }

   return( 0 );
} // zwTZPNCW2D_CopyOperationToFile

#ifdef __cplusplus
}
#endif
