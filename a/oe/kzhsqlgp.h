/*
CHANGE LOG
2002.03.08  HH
   added zSQLCALLBACK_GETUPPERSTRING.

2002.02.12  HH
   added zSQLCALLBACK_CANONICALORDER.

2001.09.04  DGC  Z10
   Made change requested by Phil: SqlBuildSelect() now set's two bit flags in
   pchEntityJoinTable to indicate whether a table was joined with it's child.

2001.06.04  HH
   added SqlCheckOpenSQL.

2001.03.15  DGC  Z10
   First start on JOIN processing.

2000.12.13  DGC  Z10
   Increased the number of attributes allowed in OpenSQL to 500.

2000.12.13  DGC  Z10
   Changed signature of SqlInitBoundList() for use with holding stmt handles.

2000.04.21  DGC  Z10
   Made changes to set more reasonable max lths for SQL buffers.

1999.10.14  DGC  Z10
   Started coding for joins.

1999.04.19  RS
        Commentlines required for Rose.

1999.04.16  RS
        Includes required for Rose


Callback Documentation.

   The SQL generator is trying to generate an attribute value and has called
   this routine to help.  What this function does and what it returns depends
   on nCommand.  This function returns FALSE if it didn't do anything -- this
   means default processing will be used by the SQL Generator.  The Following
   arguments are (almost) always set:

      nCommand    - See below for description.
      lpBoundList - Pointer to BoundListRecord.
      lpDataField - Attribute/Data field of attribute.

   Valid nCommand values are:

   zSQLCALLBACK_INITSELECT - Initialize SELECT statement.
      Other arguments:
         lpView      - View pointer.
         pInfo       - lpQualEntity pointer.
         pszTarget   - String buffer to contain SELECT string.

      lpDataField points to the first datafield for the entity being loaded.

      Returns:
         TRUE  - SELECT statement initialized.
         FALSE - Nothing done -- use default processing.

   zSQLCALLBACK_CHECKNULL     - Check to see if attribute value is NULL.
      Other arguments:
         lpView      - View pointer.

      Returns:
         1 - Attribute is NULL.
         0 - Not determined -- use default processing.
        -1 - Attribute is not NULL.

   zSQLCALLBACK_GETNULLSTRING - Return SQL value for NULL.
      Other arguments:
         pszTarget   - String buffer to contain string for NULL.

      Returns:
         TRUE  - Null value set.
         FALSE - Nothing done -- use default processing.

   zSQLCALLBACK_CONVERTSTRING - Convert string to SQL string.
      Other arguments:
         pInfo       - String to be converted.
         pszTarget   - String buffer to contain converted string.

      Returns:
         TRUE  - String converted.
         FALSE - Nothing done -- use default processing.

   zSQLCALLBACK_GETATTRVALUE  - Get value of attribute.
      Other arguments:
         lpView      - View pointer.
         pszTarget   - String buffer to contain attribute value.

      Returns:
         TRUE  - Attribute value retrieved.
         FALSE - Nothing done -- use default processing.

   zSQLCALLBACK_GETCOLUMNNAME - Get column name for SELECT or INSERT statment.
      Other arguments:
         pInfo       - If non-zero then column is for INSERT statement.
         pszTarget   - String buffer to contain column name.  Note that this
                       string already contains the table name in some cases
                       so the column name should be concatenated.

      Returns:
         TRUE  - Attribute value retrieved.
         FALSE - Nothing done -- use default processing.

   zSQLCALLBACK_USEINTO - Determine whether the 'INTO' clause should be gen'd.
         Currently used when generating static SQL.

      Returns:
         TRUE  - Generate 'INTO' clause.
         FALSE - Don't generate.

   zSQLCALLBACK_GETINTONAME - When generating the 'INTO' clause this function
         is called every time a new variable for the 'INTO' clause is gen'd.
         Currently used when generating static SQL.

      Other arguments:
         pszTarget   - String buffer to contain native variable.

      Returns:
         TRUE  - Native variable generated.
         FALSE - Use default processing.

   zSQLCALLBACK_GETCOLLISTPOS - Determines if a column name should go at the
         end of the column list for SELECTS.

      Returns:
         TRUE  - Column name goes at the end of the list.
         FALSE - Use default processing.


The following are flags that are passed into the generator via the callback
function flags but don't actually cause the callback function to be called.

   zSQLCALLBACK_UPDATEDONLY - Used when creating INSERT and UPDATE statements.
         If this flag is set then only attributes that have been changed will
         inserted/updated.

   zSQLCALLBACK_USEJOIN - If this flag is set then the generator will join
         1-1 tables when doing an activate.

   zSQLCALLBACK_FORCEORDERBY - Forces the 'ORDER BY' clause to be generated
         if the entity as attribute ordering.

   zSQLCALLBACK_ALWAYSALIAS - When generating table names, this flag causes
         an alias name (e.g. "table-name AS alias-name") to be used.

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoemeaa.h> //required for Rose

#ifdef __cplusplus
extern "C" {
#endif

#define zMAX_OPENSQL_ATTRS      500

#define MAX_TABLES              100       // Max # of tables in a SELECT cmd.
#define MAX_COLUMN_LENGTH       50000
#define MAX_COLUMN2_LENGTH      10000
#define MAX_WHERE_LENGTH        20000
#define MAX_FROM_LENGTH         20000
#define MAX_SUFFIX_LENGTH       5000      // "ORDER BY..." etc.
#define MAX_SQLCMD_LENGTH       ( MAX_COLUMN_LENGTH  + \
                                  MAX_COLUMN2_LENGTH + \
                                  MAX_WHERE_LENGTH   + \
                                  MAX_FROM_LENGTH    + \
                                  MAX_SUFFIX_LENGTH )

#define zSELECT_JOINEDWITHPARENT     0x01
#define zSELECT_JOINEDWITHCHILD      0x02
#define zSELECT_JOINEDWITHMULTCHILD  0x04

#define zSQLCALLBACK_CHECKNULL      0x00000001
#define zSQLCALLBACK_GETNULLSTRING  0x00000002
#define zSQLCALLBACK_CONVERTSTRING  0x00000004
#define zSQLCALLBACK_GETATTRVALUE   0x00000008
#define zSQLCALLBACK_INITSELECT     0x00000010
#define zSQLCALLBACK_GETCOLUMNNAME  0x00000020
#define zSQLCALLBACK_USEINTO        0x00000040
#define zSQLCALLBACK_GETINTONAME    0x00000080
#define zSQLCALLBACK_GETCOLLISTPOS  0x00000100
#define zSQLCALLBACK_GETUPPERSTRING 0x00000200

// Following are only flags and don't actually cause the callback to be called.
#define zSQLCALLBACK_UPDATEDONLY      0x01000000
#define zSQLCALLBACK_USEJOIN          0x02000000
#define zSQLCALLBACK_CANONICALORDER   0x04000000
#define zSQLCALLBACK_FORCEORDERBY     0x08000000
#define zSQLCALLBACK_ALWAYSALIAS      0x10000000

#define zSQLCALLBACK_ALLFUNCTIONS  zSQLCALLBACK_CHECKNULL      | \
                                   zSQLCALLBACK_GETNULLSTRING  | \
                                   zSQLCALLBACK_CONVERTSTRING  | \
                                   zSQLCALLBACK_GETATTRVALUE   | \
                                   zSQLCALLBACK_INITSELECT     | \
                                   zSQLCALLBACK_GETCOLUMNNAME  | \
                                   zSQLCALLBACK_USEINTO        | \
                                   zSQLCALLBACK_GETINTONAME    | \
                                   zSQLCALLBACK_UPDATEDONLY    | \
                                   zSQLCALLBACK_GETCOLLISTPOS  | \
                                   zSQLCALLBACK_USEJOIN        | \
                                   zSQLCALLBACK_FORCEORDERBY

typedef struct QualAttribStruct QualAttribRecord;
typedef QualAttribRecord *LPQUALATTRIB;
struct QualAttribStruct
{
   zLONG        hMem;                  // Mem handle for structure.
   LPQUALATTRIB lpNextQualAttrib;      // Next QualAttrib under EntitySpec.
   LPQUALATTRIB lpFirstSubQualAttrib;  // SubQual chain for QualAttrib.
   LPVIEWENTITY lpViewEntity;          // ViewEnt for QualAttrib.
   LPDATARECORD lpDataRecord;          // DataRecord for ViewEnt.
   LPVIEWATTRIB lpViewAttrib;          //
   LPDATAFIELD  lpDataField;           //
   zLONG        hOperMem;              // Mem handle for Oper string.
   zPCHAR       szOper;                //
   zLONG        hValueMem;             // Mem handle for Value string.
   zPCHAR       szValue;               //
   zPVOID       lpKeyList;             // Pointer to list of keys.
   zLONG        lKeyCount;
   LPVIEWENTITY lpSourceViewEntity;    //
   LPDATAFIELD  lpSourceDataField;     //
   zVIEW        lpSourceView;          //
   unsigned int bExists          : 1,
                bDoesNotExist    : 1,
                bLike            : 1,
                bIs              : 1;
};

typedef struct QualEntityStruct QualEntityRecord;
typedef QualEntityRecord *LPQUALENTITY;
struct QualEntityStruct
{
   zLONG        hMem;
   LPQUALENTITY lpNextQualEntity;
   LPVIEWENTITY lpViewEntity;
   LPQUALATTRIB lpFirstQualAttrib;
   zPCHAR       pszOpenSQL;               // OpenSQL SELECT statement.
   LPDATAFIELD *lpDataField;              // Attr list for OpenSQL SELECT stmt.
   zLONG        hAttrListMem;             // Mem handle for attr list.
   unsigned int bQualUsesChildEntity  : 1, // Qual for EntitySpec uses a child
                bContainsSubselect    : 1, // Qualification uses sub-select.
                bContainsIS           : 1, // Qualification uses 'IS' oper.

                // Options specified through Qualification Object:
                bOptionsNoJoins       : 1,   // NOJOIN

                // Qual uses only keys (and all the keys) of the qual entity.
                // This means we are performing a qualification that finds a
                // unique entity.
                bUniqueQualification : 1,

                // TRUE if one of the QualAttribs uses a KeyList.
                bContainsKeyList     : 1;
};

typedef struct BoundAttrList BoundListRecord;
typedef BoundListRecord *LPBOUNDLIST;

// Bound Attribute nodes for linked list.
typedef struct BoundAttrNode BoundAttr;
typedef BoundAttr *LPBOUNDATTR;
struct BoundAttrNode
{
   LPDATAFIELD             lpDataField;       // Bound DataField
   zVIEW                   lpView;            // Bound View
   LPBOUNDATTR             lpNextBoundAttr;
   zLONG                   lHMemHandle;
   zPVOID                  pszValue;              // Bound Value (from qual. Obj.)
   zLONG                   lHMemValue;
};

typedef zSHORT (POPERATION zDBH_CALLBACK)( zULONG      nCommand,
                                           zVIEW       lpView,
                                           LPBOUNDLIST lpBoundList,
                                           LPDATAFIELD lpDataField,
                                           zPVOID      pInfo,
                                           zPCHAR      pszTarget );

struct BoundAttrList
{
   zLONG         hMemory;              // Mem handle for BoundAttrList struct.
   zSHORT        nBoundCount;          // # of bound attributes.
   LPBOUNDATTR   lpFirstBoundAttr;     // First bound attribute.
   zDBH_CALLBACK pfnCallback;          // DBH format callback function.
   zULONG        lFuncs;               // Bit-flags for functions.
   zPCHAR        pchEntityJoinTable;   // Supplied by DBHandler.
   zPVOID        pvPtr;                // Callback info pointer.
} BoundAttrListRecord;

zSHORT OPERATION
SqlFreeBoundAttrList( LPBOUNDLIST  lpBoundList );

zBOOL OPERATION
SqlAddBoundAttr( LPBOUNDLIST  lpBoundList,
                 LPDATAFIELD  lpDataField,
                 zVIEW        lpView,
                 zPVOID       pszValue,
                 zLONG        lHMemValue );
zBOOL OPERATION
SqlInitBoundAttrList( LPBOUNDLIST   *lpBoundList,
                      zPCHAR        pchEntityJoinTable,
                      zDBH_CALLBACK pfnCallback,
                      zULONG        uFuncs,
                      zPVOID        pvPtr );
zSHORT OPERATION
SqlBuildInsert( zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zPCHAR       pchSqlCmd,
                LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlBuildInsertCorrTable( zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd,
                         LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlBuildUpdate( zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zSHORT       bAutoSeqOnly,
                zPCHAR       pchSqlCmd,
                LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlBuildUpdateCorrTable( zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd,
                         LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlBuildDelete( zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zPCHAR       pchSqlCmd,
                LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlBuildDeleteAll( zVIEW        lpView,
                   LPVIEWOD     lpViewOD,
                   LPVIEWENTITY lpViewEntity,
                   zPCHAR       pchSqlCmd,
                   LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlBuildDeleteCorrTable( zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd,
                         LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlBuildSelect( zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                LPQUALENTITY lpFirstQualEntity,
                zPCHAR       pchSqlCmd,
                LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlBuildSelectForEntityInstance( zVIEW        lpView,
                                 LPVIEWOD     lpViewOD,
                                 LPVIEWENTITY lpViewEntity,
                                 zPCHAR       pchColumnList,
                                 zPCHAR       pchSqlCmd,
                                 LPBOUNDLIST  lpBoundList );
zSHORT OPERATION
SqlAutoLoadFromParent( zVIEW        lpView,
                       LPVIEWOD     lpViewOD,
                       LPVIEWENTITY lpViewEntity,
                       LPQUALENTITY lpFirstQualEntity,
                       zPCHAR       pchSqlCmd,
                       LPBOUNDLIST  lpBoundList );
LPQUALENTITY OPERATION
SqlEntityIsQualified( LPQUALENTITY lpFirstQualEntity,
                      LPVIEWENTITY lpViewEntity );
void OPERATION
SqlFreeQualEntity( LPQUALENTITY *lpFirstQualEntity );

zSHORT OPERATION
SqlRetrieveQualObject( zVIEW        lpView,
                       zVIEW        lpQualView,
                       LPVIEWOD     lpViewOD,
                       LPQUALENTITY *lpFirstQualEntity,
                       LPBOUNDLIST  lpBoundList );
void OPERATION
SqlDisplayCommandEx( zPCHAR pchSqlCmd,
                     zPCHAR pchErrorChar,
                     zPVOID pfnWrite,
                     zPVOID pInfo );
void OPERATION
SqlDisplayCommand( zPCHAR pchSqlCmd,
                   zPCHAR pchErrorChar );
void OPERATION
SqlDisplayQualEntity( LPQUALENTITY lpQualEntity,
                      zBOOL        bDisplayAll );

zSHORT OPERATION
SqlAttributeIsNull( zVIEW          lpView,
                    LPDATAFIELD    lpDataField,
                    LPBOUNDLIST    lpBoundList );
zSHORT OPERATION
SqlFindNextJoinableChild( LPVIEWENTITY lpParent,
                          LPVIEWENTITY *plpChildEntity,
                          LPVIEWENTITY lpLoadEntity,
                          LPQUALENTITY lpFirstQualEntity,
                          LPBOUNDLIST  lpBoundList );
zBOOL OPERATION
SqlEntityValidForJoin( LPVIEWENTITY lpViewEntity,
                       LPVIEWENTITY lpLoadEntity,
                       LPQUALENTITY lpFirstQualEntity );
zSHORT OPERATION
SqlCheckOpenSQL( LPVIEWENTITY lpViewEntity,
                 LPQUALENTITY lpFirstQualEntity );

#ifdef __cplusplus
}
#endif

