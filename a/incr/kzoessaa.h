//KZOESSAA.H - Structures for internal system calls
/*
CHANGE LOG
1999.05.20  RG   Z9J
   again missing linefeed and carriage return characters on the last line

1999.05.19  TMV
   missing linefeed and carriage return characters on the last line of the file causes
   Resource Compiler Fatal Error RC1004 "unexpected end of file found"

1999.04.19  RS
   Commentlines required for Rose.

1999.02.08  DGC
   Added lFlags to LibraryStruct and added defines for flags.

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> //required for Rose

#define zLOADLIB_WITHERR         0x00000001
#define zLOADLIB_RESOURCES       0x00000002

// Internal Object Services structures which are made available
// when system services are used by an Object Services client

//this should be the last line refer change log 1999.05.19  TMV
