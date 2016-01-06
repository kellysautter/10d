//
// MODULE NAME:  KZOETSAA  -  Object services compile timestamp.
// DESCRIPTION:  This is the source file which prints the compile time of
//               Core.
// OPERATIONS:   fnTraceCompileTime( ).
//
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  D.G. Christensen
//  DATE:    10/06/98
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

97.10.06    DGC
   Started coding.

*/

#include "kzoephdr.h"

zVOID LOCALOPER
fnTraceCompileTime( void )
{
   TraceLineS( "Compile Date/Time for OE is ", __DATE__ " " __TIME__ );
}
