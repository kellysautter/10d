#define KZSYSSVC_INCL
#include "kzoengaa.h"
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzctlopr.h"
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "Zeidonop.H"

zOPER_EXPORT zSHORT OPERATION
PrebuildLogin( zVIEW  vSubtask )
{
   zVIEW  vAccount;
   zSHORT nRC;

   nRC = ActivateOI_FromFile( &vAccount, "ACCOUNT", vSubtask,
                              "CARLSON.PSV", zMULTIPLE );
   SetNameForView( vAccount, "ACCOUNTLST", vSubtask, zLEVEL_TASK );
   nRC = ActivateOI_FromFile( &vAccount, "ACCOUNT", vSubtask,
                              "CARLSON.POR", zMULTIPLE );
   SetNameForView( vAccount, "ACCOUNT", vSubtask, zLEVEL_TASK );
   return( 0 );
}

#ifdef __cplusplus
}
#endif
