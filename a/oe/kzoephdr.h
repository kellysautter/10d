/*
CHANGE LOG

1998.12.02    DGC
   Added some standard C headers.


   A "map" of include files:

   kzoephdr.h
      kzoengaa.h
         kzoeenaa.h
         kzoegdef.h

         #ifdef KZOESYS_INCL
            kzoeidaa.h
            kzoetdaa.h
         #endif

         #ifdef KZMETA_INCL
            kzoemeaa.h
         #endif

         #ifdef KZSYSSVC_INCL
            kzoessaa.h
            kzoep1aa.h
         #endif

         #ifdef KZOESYS_INCL
            kzoep0aa.h
               #ifndef zNO_KZOEINCO
                  kzoeinco.h
               #endif
            kzoemiaa.h
         #endif

      kzoengaa.hg
      kzoeltaa.hg

*/
#define KZOESYS_INCL
#include "kzoengaa.h"     // Object Services public prototypes

#include "kzoengaa.hg"    // EXE and DLL globals
#include "kzoeltaa.hg"    // DLL only literals
#include <stdio.h>
#include <string.h>
#include <ctype.h>
