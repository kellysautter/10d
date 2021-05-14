#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "ZeidonOp.H"

zOPER_EXPORT zSHORT OPERATION
eKeyDown( zVIEW vSubtask )
{
   TraceLineS( "(tzeditod) ============= Key down ===============", "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDITOD_Close( zVIEW vSubtask )
{
   zVIEW vEdWrk;

   GetViewByName( &vEdWrk, "TZEDITOO", vSubtask, zLEVEL_TASK );
   if ( vEdWrk )
      CommitOI_ToFile( vEdWrk, "c:\\temp\\tzeditoo.por", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZEDITOD_Prebuild( zVIEW vSubtask )
{
   zVIEW vEdWrk;

   ActivateOI_FromFile( &vEdWrk, "tzeditoo", vSubtask,
                        "c:\\temp\\tzeditoo.por", zSINGLE );
   if ( vEdWrk == 0 )
   {
      ActivateEmptyObjectInstance( &vEdWrk, "tzeditoo", vSubtask, 0 );
      CreateEntity( vEdWrk, "TZEDITOO", zPOS_FIRST );
   }

   SetNameForView( vEdWrk, "TZEDITOO", vSubtask, zLEVEL_TASK );
   return( 0 );
}

