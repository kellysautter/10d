/*

Include file for QuinSoft's DBH Run-time (kzhsqlxa.c) and Generator (kzhsqlxb.c)
modules.

*/

/*
CHANGE LOG

1999.11.18  DGC  10a
   Added ODBC_RecordDataStruct for supporting joins.

1999.04.19  RS
   Commentlines required for Rose.

1999/04/16  RS
   includes required for Rose

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> //required for Rose

// ODBC specific information to be stored in the XOD at the OBJECT level.
struct ODBC_ObjectDataStruct
{
   zLONG    nTimestampPrecision;
   zBYTE    bStoreTimestampAsString;
   zBYTE    byReserved[ 99 ];
};
typedef struct ODBC_ObjectDataStruct ODBC_ObjectDataRecord;
typedef ODBC_ObjectDataRecord *LPODBC_OBJECTDATA;

// ODBC specific information to be stored in the XOD at the DATARECORD level.
struct ODBC_RecordDataStruct
{
   zBYTE    bNoJoinWithParent;
   zBYTE    byReserved[ 10 ];
};
typedef struct ODBC_RecordDataStruct ODBC_RecordDataRecord;
typedef ODBC_RecordDataRecord *LPODBC_RECORDDATA;

struct DB2_ObjectDataStruct
{
   zBOOL    bStoreDB2_Data;  // Used when building an XOD.
   zBOOL    bUseStaticSQL;
   zCHAR    szStaticSQL_DLL[ 100 ];
};
typedef struct DB2_ObjectDataStruct DB2_ObjectDataRecord;
typedef DB2_ObjectDataRecord *LPDB2_OBJECTDATA;

