/////////////////////////////////////////////////////////////////////////////
//
// KZHSQLGA.C - Generates SQL calls for dbhandlers.
//
// Programming Notes:
//
//    o  All function return codes use the following convention:
//       - RC = 0 means that the function worked as intended.
//       - RC > 0 means that the function worked but with warnings.
//       - RC < 0 means that an error occurred.
//
///////////////////////////// DEFINES ///////////////////////////////////////

/*
CHANGE LOG

2005.11.03  DKS  Z10
   Added (disabled) code to <> operand to say IS NULL OR ... <> ... .

2002.10.20  DGC  Z10
   Added functionality to select entities from a qual list.

2002.04.26  HH
   fix bug in fnCanonicalOrder.

2002.03.08  HH
   support the UPPER.. syntax for qualification operators.

2002.02.14  HH
   added Canonical Ordering (for x:m joins, TonBeller DBHandler).
   bugfix generating ORDER BY.

2002.01.30  HH
   made local functions static (not fnTraceSql).

2001.11.21  HH
   Improved SqlEntityValidForJoin.
   Improved JOIN with recursive structure.

2001.09.26  DGC  Z10
   Made change requested by Phil: SqlBuildSelect( ) now sets two bit flags in
   pchEntityJoinTable to indicate whether a table was joined with its child.

2001.08.09  DGC
   Made a fix for 54161.  SQL generation no longer crashes.

2001.07.31  DGC
   Fixed Deuka bug reported by HH.  Wasn't joining entities that had recursive
   structure but were not logically recursive.

2001.06.20  DGC
   More JOIN stuff using qualification.

2001.06.07  HH
   Returncode from SqlBuildSelect, respect regular join.

2001.06.07  DGC
   No LEFT JOIN if QualEntity->bOptionsNoJoins

2001.06.04  HH
   external scope for SqlCheckOpenSQL.

2001.06.04  DGC
   Fixed bug reported by HH, which is a continuance of the bug fixed on
   2001.05.21.

   Short term solution for bug when child qualification is used.  We don't
   allow joins of child entities if there is qualification on a child entity.

2001.06.01  HH
2001.05.29  HH
   Improve handling of timestamps in qual objects.

2001.05.29  HH
   Fix for DB_Oper+JOIN problem.

2001.05.21  DGC/HH
   Fixed bug reported by Dueka.  When using left joins, if a user qualified
   an entity using a child entity and "IS NULL", the results were wrong
   the generated SQL loaded all entities where the child didn't exist.

2001.05.17  DGC/HH
   fix for the DEUKA ROOTONLYMULTIPLE bug.

2001.05.12  DGC/HH
   fix for the AND AND problem.

2001.05.02  HH
   improve returncode handling (JOINS) in SqlBuildSelect.

2001.05.11  DGC  Z10
   More bug fixes for JOINS.

2001.04.19  DGC  Z10
   Added code for "Use OrderBy" logic.

2001.03.15  DGC  Z10
   First start on JOIN processing.

2000.12.13  DGC  Z10
   Changed signature of SqlInitBoundAttrList( ) for use with holding stmt handles.

2000.11.21  DGC  Z10
   Fixed a loop hole.  User shouldn't be qualifying on a derived entity/attr.

2000.08.14  DGC  Z10
   Made fixe requested by Hartmut.  For LIKE operations we don't pad out fixed
   lth attributes.

2000.08.01  DGC  Z10
   Fixed bug reported by Phil/Jeff.  We sometimes where creating a join for
   and entity even though the entity was the root for a SELECT statement.

2000.06.28  DGC  Z10
   Changed qualification as per request by Helmut.  Previously a qualification
   involving a x-to-many child required an EXISTS clause in the qualification
   object.  This is now assumed.

   Fixed, hopefully for the last time, the many-to-many join problem.

2000.06.13  DGC  Z10
   Fixed NULL crash.

2000.06.07  DGC  Z10
   First attempt at fixing bugs reported by Frank H.:
      - Check for NULL attributes during qualification.
      - Regard entities with Hanging FK's as 0-to-1 relationships during
        JOIN processing.

2000.05.30  HH  Z10
   returncode of SqlBuildSelect indicate Order By generated.

2000.05.25  HH  Z10
   modified returncode definition of SqlBuildSelect to indicate that
   joins are generated.
   not use joins on many-to-many relationships, see 2000.04.21  DGC  Z10

2000.05.10  DGC  Z10
   Fixed MM join bug reported by HH.

2000.05.04  DGC  Z10
   Change signature of LoadEntity( ).

2000.04.21  DGC  Z10
   Fixed bug reported by Jeff.  We can't use joins if it involves many-to-many
   relationships.

2000.04.21  DGC  Z10
   Made changes to set more reasonable max lths for SQL buffers.

2000.04.07  HH
   Corrected the comma setting in FROM clause with LEFT JOIN.

2000.01.24  DGC  Z10
   We now allow the 'Value' attribute in the qualification object to be null
   if the comparison operator is '=' or '!='.

1999.11.24  DGC  Z10
   Finished JOINS.

   Added logic when qualifying using an 'EXISTS' or 'NOT EXISTS' clause on
   a child entity that has a x-to-1 relationship with it's parent.  In this
   case we don't need a subselect but can instead do a
      WHERE parent.fk IS [ NOT ] NULL
   The only problem with what I did is that I hard-code the 'NULL' string.  In
   DBMS's that need a different string for 'NULL' (e.g. SqlServer DBLIB) this
   will be a problem.  If this is to be fixed we have to worry about the side
   problem that the oper "IS" has a different meaning if it is part of the
   Zeidon qualification.  This will have to be sorted out.

1999.10.14  DGC  Z10
   Started coding for joins.

1999.09.29  DGC
   A little cleaning.

1999.06.29  DGC
   Added logic to update/create only attributes that were changed.

1999.02.26  DGC
   Fixed a problem with LB Wes10 activate.  For some reason we where not adding
   table names to the FROM table when adding foreign keys.  I couldn't figure
   out why we were skipping some tables so I took out the check.  Look for a
   comment matching 2/26/99 for more.

1999.01.12  LW
   Fixed Compile-Error under MVS.

*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

#define DELETEHACK 1
#define DBG_DISPLAY  FALSE

#define  KZSYSSVC_INCL
#define  KZMETA_INCL
#include "kzoengaa.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "kzhsqlgp.h"

// Calls the DBH callback function.  Returns TRUE if the callback function
// handled the command.  Expects the following variables to be
// defined in the function calling EXEC_CALLBACK:
//    nRC, lpBoundList
#define EXEC_CALLBACK( callback_cmd, lpView, lpDataField,                 \
                       pInfo, pchTarget )                                 \
           lpBoundList && lpBoundList->pfnCallback &&                     \
           (lpBoundList->lFuncs & callback_cmd) != 0 &&                   \
           (nRC = (*lpBoundList->pfnCallback)( callback_cmd,              \
                                               lpView, lpBoundList,       \
                                               lpDataField,               \
                                               (zPVOID) pInfo,            \
                                               pchTarget )) != 0

// Just checks to see if a callback function will be called.
#define CHECK_CALLBACK( callback_cmd )                                    \
           lpBoundList && lpBoundList->pfnCallback &&                     \
           (lpBoundList->lFuncs & callback_cmd) != 0

// Checks to see if a flag is set in the callback settings.
#define CHECK_CALLBACK_FLAG( callback_flag )                              \
           (lpBoundList && (lpBoundList->lFuncs & callback_flag) != 0)

#define zSELECT_CMD             0x0001
#define zSUBSELECT_CMD          0x0002
#define zINSERT_CMD             0x0004
#define zDELETE_CMD             0x0008
#define zUPDATE_CMD             0x0010
#define zCORR_CMD               0x1000

#define zFULLY_QUALIFIED        0x0001
#define zKEYS_ONLY              0x0002
#define zNO_HIDDEN              0x0004
#define zUPDATED_ONLY           0x0008
#define zFOREIGN_KEY            0x0010

#define zSUB_ENTITY             1
#define zROOT_ENTITY            2
#define zERROR                  0
#define zWARNING                1

// Flags for fnAddTableToFrom
#define AT_ALWAYS_ADD           0x00000001

#define MAXLEN_KEY_ARRAY        256

// Following determines if 'c' is a valid char for Zeidon entity/attr names.
#define IS_VALID_ID_CHAR( c )   (zisalnum( c ) || (c) == '_')

#define mAdvanceToNull( a )     while ( *(a) ) (a)++

static zCHAR szlEntitySpec[]   = "EntitySpec";
static zCHAR szlQualAttrib[]   = "QualAttrib";
static zCHAR szlSubQualAttrib[] = "SubQualAttrib";

typedef struct TableNameStruct TableNameInfoRecord;
struct TableNameStruct
{
   zPCHAR       pchTableName;       // Pointer to table name in FROM clause.
   zCHAR        szAliasName[ 10 ];  // Alias name of table (if it has one).
   LPVIEWENTITY lpViewEntity;       // Entity referencing the table name.
};

typedef struct SqlStatementStruct SqlStatementRecord;
typedef SqlStatementRecord *LPSQLSTATEMENT;
struct SqlStatementStruct
{
   zCHAR  szText[ 10 ];       // Used to help debugging.
   zSHORT nCommandType;       // Id of command type (Select, Insert, etc. )
   zPCHAR pchSqlCmd;          // Ptr to buffer that can hold entire SQL cmd.
   zPCHAR pchSqlCmdEnd;       // Ptr to null terminator of pchSqlCmd.
   zPCHAR pchColList2;        // Buffer containing 2nd list of columns.
   zPCHAR pchColList2End;     // Ptr to null terminator of pchColList2.
   zPCHAR pchFrom;            // Ptr to buffer to temporarily hold FROM clause.
   zPCHAR pchFromEnd;         // Ptr to null terminator of pchFrom.
   zPCHAR pchNextJoinParen;   // Ptr to next available space for opening paren.
   zPCHAR pchWhere;           // Ptr to buffer to temporarily hold WHERE clause.
   zPCHAR pchWhereEnd;        // Ptr to null terminator of pchWhere.
   zPCHAR pchSuffix;          // Suffix information ('ORDER BY', etc.)
   zPCHAR pchSuffixEnd;       // Ptr to null terminator of pchSuffix.
   zSHORT nComponentCount;    // Current # of components in WHERE clause.
   zLONG  hSqlCmd;            // Memory handle for pchSqlCmd.
   zSHORT nSubLevel;          // Levels of sub-commands -- usually 0.
   zVIEW  lpView;             // View we are activating/committing.

   // Following used during BuildSelect processing.
   LPVIEWENTITY lpLoadViewEntity;   // This is the entity that is being loaded.
   LPQUALENTITY lpFirstQualEntity;  // List of qualification object.

   unsigned int
      bContainsSubselect   : 1, // TRUE if SELECT stmt contains a subselect.
      bQualUsesChildEntity : 1, // Qual for EntitySpec uses a child
      bUseLeftJoin         : 1, // DBH can use left joins for loading ents.
      bExample             : 1; // Dummy example bit.

   zSHORT nFromTableCount;    // # of tables in FROM clause.
   TableNameInfoRecord TableNameInfo[ MAX_TABLES ];

   LPSQLSTATEMENT lpParentSql; // If the current Sql statement is a sub-select
                               // then lpParentSql points to the parent select.

   LPBOUNDLIST    lpBoundList;
};

static zSHORT
fnSqlAddQualToWhere( zVIEW          lpView,
                     LPQUALENTITY   lpQualEntity,
                     LPQUALATTRIB   lpFirstQualAttrib,
                     LPSQLSTATEMENT lpSql );
static zSHORT
fnGenerateOrderBy( LPSQLSTATEMENT lpSql,
                   LPVIEWENTITY   lpViewEntity,
                   LPQUALENTITY   lpFirstQualEntity,
                   LPBOUNDLIST    lpBoundList );

static zSHORT
fnCanonicalOrder( LPSQLSTATEMENT lpSql,
                  LPVIEWENTITY   lpViewEntity,
                  LPQUALENTITY   lpFirstQualEntity,
                  LPBOUNDLIST    lpBoundList );

// Remove whitespaces at the beginning and end of a string.
void
zstrtrim( zPCHAR pchSource )
{
   zPCHAR pch;
   zLONG  lLth = zstrlen( pchSource ) + 1;

   // Find first non-space.
   for ( pch = pchSource; zisspace( *pch ); pch++ )
      ; // Nothing needs to be done here.

   if ( pch != pchSource )
      strcpy_s( pchSource, lLth, pch );

   for ( pch = pchSource + zstrlen( pchSource ); zisspace( *pch ); pch-- )
      ; // nothing needs to be done here

   *pch = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// SqlEntityIsQualified
//
/////////////////////////////////////////////////////////////////////////////
LPQUALENTITY OPERATION
SqlEntityIsQualified( LPQUALENTITY lpFirstQualEntity,
                      LPVIEWENTITY lpViewEntity )
{
   LPQUALENTITY lpQualEntity;

   // Check to see if table is qualified by looking in the qualification
   // table for a matching View entity.
   lpQualEntity = lpFirstQualEntity;
   while ( lpQualEntity && lpQualEntity->lpViewEntity != lpViewEntity )
      lpQualEntity = lpQualEntity->lpNextQualEntity;

   return( lpQualEntity );
}

/////////////////////////////////////////////////////////////////////////////
//
// fnCompare
//
// Compares a binary value with a string using pchOperStr as the
// comparison operator.
//
// Valid values for pchOperStr are: "=", "!=", "<", and ">"
// Valid binary types are zTYPE_INTEGER, zTYPE_DATETIME, zTYPE_DATE,
// zTYPE_TIME, and zTYPE_STRING.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnCompare( zVIEW  lpView,
           zPVOID lpA,
           zPCHAR pchOperStr,
           zPCHAR pchBStr,
           zCHAR  chType )
{
   zPCHAR   szOper;
   zBOOL    b;
   zBOOL    bNot;
   zBOOL    bReturn;
   zPVOID   lpB;
   zLONG    lTemp;
   zDECIMAL dTemp;
   DateTimeRecord dtTemp;

   switch ( chType )
   {
      case zTYPE_INTEGER:
         lTemp = zatol( pchBStr );
         lpB = (zPVOID) &lTemp;
         break;

      case zTYPE_DECIMAL:
         SysConvertStringToDecimal( pchBStr, &dTemp );
         lpB = (zPVOID) &dTemp;
         break;

      case zTYPE_DATETIME:
      case zTYPE_TIME:
      case zTYPE_DATE:
         UfStringToDateTime( pchBStr, &dtTemp );
         lpB = (zPVOID) &dtTemp;
         break;
   }

   bReturn = FALSE;
   bNot    = FALSE;
   szOper  = pchOperStr;
   while ( szOper[ 0 ] )
   {
      // Skip whitespaces.
      while ( szOper[ 0 ] == ' ' )
         szOper++;

      // Using szOper, compare the two values.  The variable b is the result
      // of the compareson.
      switch ( szOper[ 0 ] )
      {
         case '>':
            switch ( chType )
            {
               case zTYPE_INTEGER:
                  b = *((zPLONG) lpA) > *((zPLONG) lpB);
                  break;

               case zTYPE_DECIMAL:
                  b = SysCompareDecimalToDecimal( (zPDECIMAL) lpA,
                                                  (zPDECIMAL) lpB ) > 0;
                  break;

               case zTYPE_STRING:
                  b = zstrcmp( (zPCHAR) lpA, (zPCHAR) pchBStr ) > 0;
                  break;

               case zTYPE_DATETIME:
               case zTYPE_TIME:
               case zTYPE_DATE:
                  b = UfCompareDateTimeToDateTime( (LPDATETIME) lpA,
                                                   (LPDATETIME) lpB ) > 0;
                  break;
            }

            break;

         case '<':
            switch ( chType )
            {
               case zTYPE_INTEGER:
                  b = *((zPLONG) lpA) < *((zPLONG) lpB);
                  break;

               case zTYPE_DECIMAL:
                  b = SysCompareDecimalToDecimal( (zPDECIMAL) lpA,
                                                  (zPDECIMAL) lpB ) < 0;
                  break;

               case zTYPE_STRING:
                  b = zstrcmp( (zPCHAR) lpA, pchBStr ) < 0;
                  break;

               case zTYPE_DATETIME:
               case zTYPE_TIME:
               case zTYPE_DATE:
                  b = UfCompareDateTimeToDateTime( (LPDATETIME) lpA,
                                                   (LPDATETIME) lpB ) < 0;
                  break;
            }

            break;

         case '=':
            switch ( chType )
            {
               case zTYPE_INTEGER:
                  b = *((zPLONG) lpA) == *((zPLONG) lpB);
                  break;

               case zTYPE_DECIMAL:
                  b = SysCompareDecimalToDecimal( (zPDECIMAL) lpA,
                                                  (zPDECIMAL) lpB ) == 0;
                  break;

               case zTYPE_STRING:
                  b = zstrcmp( (zPCHAR) lpA, pchBStr ) == 0;
                  break;

               case zTYPE_DATETIME:
               case zTYPE_TIME:
               case zTYPE_DATE:
                  b = UfCompareDateTimeToDateTime( (LPDATETIME) lpA,
                                                   (LPDATETIME) lpB ) == 0;
                  break;
            }

            break;

         case '!':
            bNot = !bNot;
            b    = FALSE;
            break;
      }

      bReturn = bReturn || b;

      szOper++;
   } // while ( szOper[ 0 ] )...

   if ( bNot )
      return( !bReturn );
   else
      return( bReturn );

}  /* fnCompare */

#define fnSqlAddStringToSql( a, s, b )   (a)->pchSqlCmdEnd = zstrcatp( (a)->pchSqlCmdEnd, s, (b) )
#define fnSqlAddStringToWhere( a, s, b ) (a)->pchWhereEnd = zstrcatp( (a)->pchWhereEnd, s, (b) )
#define fnSqlAddStringToSuffix( a, s, b ) (a)->pchSuffixEnd = zstrcatp( (a)->pchSuffixEnd, s, (b) )
#define fnSqlWhereIsEmpty( a )            (!*(a)->pchWhere)
#define fnSqlWhereIsNotEmpty( a )         (*(a)->pchWhere)
#define fnSqlFromIsEmpty( a )             (!*(a)->pchFrom)
#define fnSqlIncrementComponentCount( a ) (a)->nComponentCount++

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlInitSqlStatement
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlInitSqlStatement( LPSQLSTATEMENT lpSql,
                       zSHORT         nCommandType,
                       zPCHAR         pchSqlCmd,
                       zVIEW          lpView,
                       LPBOUNDLIST    lpBoundList )
{
   zmemset( (zPVOID) lpSql, 0, sizeof( SqlStatementRecord ) );
   if ( pchSqlCmd == 0 )
   {
      lpSql->hSqlCmd = SysAllocMemory( &pchSqlCmd, MAX_SQLCMD_LENGTH,
                                       0, zCOREMEM_ALLOC, 0 );
      if ( lpSql->hSqlCmd == 0 )
         return( zCALL_ERROR );
   }

#ifdef DEBUG
   zmemset( pchSqlCmd, 0, MAX_SQLCMD_LENGTH - 1 );
#endif

   *pchSqlCmd = 0;   // Make sure SQL initialized with null.
   lpSql->pchSqlCmd    = pchSqlCmd;
   lpSql->pchSqlCmdEnd = pchSqlCmd;

   // Set aside room for the column list.
   pchSqlCmd += MAX_COLUMN_LENGTH;

   // Set up space for 2nd column list.  This is used when some columns must
   // appear at the end of the SELECT list (e.g. ODBC long columns).
   *pchSqlCmd            = 0;            // Init to empty string.
   lpSql->pchColList2    = pchSqlCmd;
   lpSql->pchColList2[ 0 ] = 0;
   lpSql->pchColList2End = lpSql->pchColList2;

   // Set aside room for the 2nd column list.
   pchSqlCmd += MAX_COLUMN2_LENGTH;

   *pchSqlCmd         = 0;            // Init to empty string.
   lpSql->pchWhere    = pchSqlCmd;
   lpSql->pchWhereEnd = pchSqlCmd;

   // Set aside room for the WHERE clause.
   pchSqlCmd += MAX_WHERE_LENGTH;

   *pchSqlCmd        = 0;            // Init to empty string.
   lpSql->pchFrom    = pchSqlCmd;
   lpSql->pchFromEnd = pchSqlCmd;

   // Set aside room for the FROM clause.
   pchSqlCmd += MAX_FROM_LENGTH;

   *pchSqlCmd          = 0;            // Init to empty string.
   lpSql->pchSuffix    = pchSqlCmd;
   lpSql->pchSuffixEnd = pchSqlCmd;

   lpSql->nCommandType = nCommandType;
   lpSql->lpView       = lpView;
   lpSql->lpBoundList  = lpBoundList;

   return( 0 );

}  /* fnSqlInitSqlStatement */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlFreeSqlStatement
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlFreeSqlStatement( LPSQLSTATEMENT lpSql )
{
   if ( lpSql->hSqlCmd )
      SysFreeMemory( lpSql->hSqlCmd );

   return( 0 );
}  /* fnSqlFreeSqlStatement */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlAssembleSqlStatement
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlAssembleSqlStatement( LPSQLSTATEMENT lpSql )
{
   if ( lpSql->pchColList2[ 0 ] )
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpSql->pchColList2 );

   if ( lpSql->pchFrom[ 0 ] )
   {
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " FROM " );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpSql->pchFrom );
   }

   if ( lpSql->pchWhere[ 0 ] )
   {
      if ( lpSql->nComponentCount > 1 )
      {
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " WHERE ( " );
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpSql->pchWhere );
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " )" );
      }
      else
      {
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " WHERE " );
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpSql->pchWhere );
      }
   }

   if ( lpSql->pchSuffix[ 0 ] )
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpSql->pchSuffix );

   fnSqlFreeSqlStatement( lpSql );

   return( 0 );
}  /* fnSqlAssembleSqlStatement */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlFindTableName
//
// Finds the table/entity name in the list of tables for lpSql.  If
// bCheckParent is TRUE and if the table/entity name is not found in
// lpSql then we recursively call fnSqlFindTableName to find the table/entity
// name in the parent sql.
//
// We must find a match on table and entity name because we could have the
// same table name for different entities.
//
// RETURNS: #  - Index of pchTableName if it is contained in the From table
//               list.
//          -1 - if it isn't.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlFindTableName( zPCHAR         pchTableName,
                    LPVIEWENTITY   lpViewEntity,
                    LPSQLSTATEMENT lpSql,
                    zBOOL          bCheckParent )
{
   zSHORT nTableCount;

   for ( nTableCount = 0;
         nTableCount < lpSql->nFromTableCount;
         nTableCount++ )
   {
      if ( lpSql->TableNameInfo[ nTableCount ].lpViewEntity == lpViewEntity &&
           zstrcmp( lpSql->TableNameInfo[ nTableCount ].pchTableName,
                    pchTableName ) == 0 )
      {
         return( nTableCount );
      }
   }

   // We didn't find the table name so if the caller wants us to check the
   // parent (and if there is a parent) then make recursive call using the
   // parent.
   if ( bCheckParent && lpSql->lpParentSql )
      return( fnSqlFindTableName( pchTableName, lpViewEntity,
                                  lpSql->lpParentSql, bCheckParent ) );

   // We got here?  Then there is no table name.
   return( -1 );

}  /* fnSqlFindTableName */

/////////////////////////////////////////////////////////////////////////////
//
// fnTableName
//
// Returns a pointer to the name of the table.  If the table name has an
// alias, a pointer to the alias is returned.
//
/////////////////////////////////////////////////////////////////////////////
zPCHAR LOCALOPER
fnTableName( LPSQLSTATEMENT lpSql,
             zPCHAR         pchTableName,
             LPVIEWENTITY   lpViewEntity )
{
   zSHORT nTableNum = fnSqlFindTableName( pchTableName, lpViewEntity,
                                          lpSql, FALSE );

   // If nTableNum >= 0 then a matching table/entity has been found.  If the
   // table has an alias name then return the alias name as the name of the
   // table.  If not just return the table name.
   if ( nTableNum >= 0 )
   {
      if (  lpSql->TableNameInfo[ nTableNum ].szAliasName[ 0 ] != 0 )
         return( lpSql->TableNameInfo[ nTableNum ].szAliasName );
      else
         return( pchTableName );
   }

   // No matching table/entity was found.  If lpSql has a parent sql (this
   // means that lpSql is part of a sub-select) then check for the table
   // name in the parent sql statement.
   if ( lpSql->lpParentSql )
      return( fnTableName( lpSql->lpParentSql, pchTableName, lpViewEntity ) );

   // No matching table/entity and no parent sql statement.  This happens for
   // simple stuff like INSERT, etc. where there are no alias names.  Just
   // return the table name.
   return( pchTableName );

} // fnTableName

// Add an alias name to the FROM clause for the specified table.
static void
fnAddAliasName( LPSQLSTATEMENT lpSql, int nTableNbr )
{
   zCHAR  szAlias[ 10 ];

   // If we are using outer joins then we need to add "AS" to the join statement.
   if ( lpSql->bUseLeftJoin )
      lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 4, " AS" );

   // Generate aliases for the table names.  The alias name is generated
   // as An where:
   //
   //     A = A letter representation of nSubLevel (i.e.  0='A', 1='B', etc.)
   //     n = Number representing the sequence number of the table.
   //
   //  For example, the following FROM statement would be generated for a
   //  subselect 1 level deep (e.g. nSubLevel = 1) with 2 tables:
   //
   //  FROM Table1 B1, Table2 B2...

   szAlias[ 0 ] = ' ';
   szAlias[ 1 ] = 'A' + (zCHAR) lpSql->nSubLevel;
   zltoa( (zLONG) nTableNbr, szAlias + 2, sizeof( szAlias ) - 2 );

   lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, sizeof( szAlias ), szAlias );

   // Copy alias name to table info (skipping the leading space ).
   strcpy_s( lpSql->TableNameInfo[ nTableNbr ].szAliasName,
             sizeof( lpSql->TableNameInfo[ nTableNbr ].szAliasName ), szAlias + 1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// fnAddTableToFrom
//
// Adds the table name pchTableName to the FROM buffer in lpSql.  This
// routine is smart enough to figure out if an alias name is to be
// created.
//
/////////////////////////////////////////////////////////////////////////////
static void
fnAddTableToFrom( LPSQLSTATEMENT lpSql,
                  zPCHAR         pchTableName,
                  LPVIEWENTITY   lpViewEntity,
                  zLONG          lControl )
{
   LPBOUNDLIST  lpBoundList    = lpSql->lpBoundList;
   LPDATARECORD lpDataRecord   = zGETPTR( lpViewEntity->hFirstDataRecord );
   LPRELRECORD  lpRelRecord    = zGETPTR( lpDataRecord->hFirstRelRecord );
   zBOOL        bAlwaysAdd     = (lControl & AT_ALWAYS_ADD);
   zBOOL        bCreateAlias   = FALSE;
   zSHORT       nTableNbr;
   zSHORT       nRelTableNbr;
   zSHORT       k;

   // Default the table name to match the table name for the entity.
   if ( pchTableName == 0 )
      pchTableName = lpDataRecord->szRecordName;

   // If the table name has already been added then return.
   if ( bAlwaysAdd == FALSE &&
        fnSqlFindTableName( pchTableName, lpViewEntity,
                            lpSql, TRUE ) >= 0 )
   {
      return;
   }

   if ( lpSql->nFromTableCount >= MAX_TABLES )
   {
      SysMessageBox( 0, "Internal Core Error", "Too many tables in SQL cmd", 1 );
      return;
   }

   // Check to see if we need to create an alias name for the table.
   // There are five conditions that cause alias names to be used:
   //    o  User wants us to always use an alias.
   //    o  If we are building sub-selects (see below).
   //    o  If we are referencing a table name that is already being used for
   //       another entity.
   //    o  If the current select statement contains a sub-select.
   //    o  If the there is a Qualification on a child.

   // Check to see if we pchTableName already exists in the table list.
   for ( k = 0; k < lpSql->nFromTableCount; k++ )
   {
      if ( zstrcmp( pchTableName,
                    lpSql->TableNameInfo[ k ].pchTableName ) == 0 )
      {
         break;
      }
   }

   // If the sub level of the current Sql statement is greater than 0 then
   // we are building a sub-select--which means that we need to use aliases
   // for the table names.
   if ( CHECK_CALLBACK_FLAG( zSQLCALLBACK_ALWAYSALIAS ) ||
        k < lpSql->nFromTableCount                      ||
        lpSql->nSubLevel > 0                            ||
        lpSql->bContainsSubselect )
   {
      bCreateAlias = TRUE;
   }

   // See if this table has a many-to-many relationship with it's parent.
   // If it does, we want to add the relationship table.  If the name of
   // the table matches the table name in the lpRelRecord, then we don't
   // need to add this because it will be added shortly.
   if ( lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY &&
        zstrcmp( lpRelRecord->szRecordName, pchTableName ) != 0 )
   {
      // Do we already have tables in the FROM clause?  If we do then we need
      // to add a "," or a "LEFT JOIN".
      if ( lpSql->nFromTableCount > 0 )
      {
         if ( lpSql->bUseLeftJoin )
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 12, " LEFT JOIN " );
         else
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 3, ", " );
      }

      // Add the rel table to the table list.
      nRelTableNbr = lpSql->nFromTableCount++;

      lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, sizeof( lpRelRecord->szRecordName ),
                                    lpRelRecord->szRecordName );
      lpSql->TableNameInfo[ nRelTableNbr ].pchTableName =
                                             lpRelRecord->szRecordName;
      //lpSql->TableNameInfo[ nRelTableNbr ].pchTableName = pchTableName;
      lpSql->TableNameInfo[ nRelTableNbr ].lpViewEntity = lpViewEntity;

      if ( bCreateAlias )
         fnAddAliasName( lpSql, nRelTableNbr );

      // If the entity that contains the M-M table is not the root entity
      // of the sub-object we are loading, we need to add qualification.
      if ( lpSql->bUseLeftJoin &&
           lpViewEntity != lpSql->lpLoadViewEntity )
      {
         LPRELFIELD lpRelField;
         zBOOL      bFirstTime = TRUE;

         lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 5, " ON " );

         for ( lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
               lpRelField;
               lpRelField = zGETPTR( lpRelField->hNextRelField ) )
         {
            LPDATAFIELD  lpSrcDataField;
            LPDATARECORD lpSrcDataRecord;
            LPVIEWATTRIB lpSrcViewAttrib;
            LPVIEWENTITY lpSrcViewEntity;

            lpSrcDataField  = zGETPTR( lpRelField->hSrcDataField );
            lpSrcViewAttrib = zGETPTR( lpSrcDataField->hViewAttrib );
            lpSrcViewEntity = zGETPTR( lpSrcViewAttrib->hViewEntity );
            lpSrcDataRecord = zGETPTR( lpSrcViewEntity->hFirstDataRecord );

            // We only want to add the rel fields that are linked with the
            // parent.
            if ( lpSrcViewEntity == lpViewEntity )
               continue;

            if ( bFirstTime )
               bFirstTime = FALSE;
            else
               lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 6, " AND " );

            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 256,  // ???
                                          fnTableName( lpSql,
                                                       lpRelRecord->szRecordName,
                                                       lpViewEntity ) );
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 2, "." );
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, sizeof( lpRelField->szFldName ),
                                          lpRelField->szFldName );

            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 4, " = " );

            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 256,  // ???
                                          fnTableName( lpSql,
                                                       lpSrcDataRecord->szRecordName,
                                                       lpSrcViewEntity ) );
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 2, "." );
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, sizeof( lpSrcDataField->szFldName ),
                                          lpSrcDataField->szFldName );

         } // for ( lpRelField... )

         lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 4, " ) " );

         // We've added a paren to the end of the FROM clause, so we need to
         // add one to the beginning.
         *lpSql->pchNextJoinParen++ = '(';
         lpSql->pchNextJoinParen++;

      } // if ( lpSql->bUseLeftJoin ... )

   } // if ( lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )...

   // Do we already have tables in the FROM clause?  If we do then we need
   // to add a "," or a "LEFT JOIN".
   if ( lpSql->nFromTableCount > 0 )
   {
      if ( lpSql->bUseLeftJoin )
         lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 12, " LEFT JOIN " );
      else
         lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 3, ", " );
   }

   nTableNbr = lpSql->nFromTableCount++;
   lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 256, pchTableName );  // ???
   lpSql->TableNameInfo[ nTableNbr ].pchTableName = pchTableName;
   lpSql->TableNameInfo[ nTableNbr ].lpViewEntity = lpViewEntity;

   if ( bCreateAlias )
      fnAddAliasName( lpSql, nTableNbr );

   // If we are using joins then add the foreign keys.
   if ( lpSql->bUseLeftJoin &&
        (lpViewEntity != lpSql->lpLoadViewEntity ||
         (lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY)) )
   {
      LPRELFIELD   lpRelField;
      zBOOL        bFirst = TRUE;

      lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 5, " ON " );

      // Go through all the RelFields and add the qualification.
      for ( lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
            lpRelField;
            lpRelField = zGETPTR( lpRelField->hNextRelField ) )
      {
         LPDATAFIELD  lpDataField;
         LPVIEWATTRIB lpViewAttrib;
         LPVIEWENTITY lpTempEntity;

         // Get Source relfield table name.
         lpDataField  = zGETPTR( lpRelField->hSrcDataField );
         lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
         lpTempEntity = zGETPTR( lpViewAttrib->hViewEntity );
         lpDataRecord = zGETPTR( lpTempEntity->hFirstDataRecord );

         // If we are adding the qualification for a m-m relationship we only
         // need to worry about some of the rel fields because the others
         // were added when we added the relationship table above.
         // parent.
         if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
         {
            if ( lpTempEntity != lpViewEntity )
               continue;
         }

         if ( bFirst )
            bFirst = FALSE;
         else
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 6, " AND " );

         lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 256,  // ???
                                       fnTableName( lpSql,
                                                    lpDataRecord->szRecordName,
                                                    lpTempEntity ) );
         lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 2, "." );
         lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, sizeof( lpDataField->szFldName ),
                                       lpDataField->szFldName );

         lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 4, " = " );

         // Get Target relfield table name.
         if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
         {
            // Target for this relationship is in the relationship table.
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 256,  // ???
                                          fnTableName( lpSql,
                                                       lpRelRecord->szRecordName,
                                                       lpTempEntity ) );
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 2, "." );
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, sizeof( lpRelField->szFldName ),
                                          lpRelField->szFldName );
         }
         else
         {
            lpDataField  = zGETPTR( lpRelField->hRelDataField );
            lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
            lpTempEntity = zGETPTR( lpViewAttrib->hViewEntity );
            lpDataRecord = zGETPTR( lpTempEntity->hFirstDataRecord );

            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 256,  // ???
                                          fnTableName( lpSql,
                                                       lpDataRecord->szRecordName,
                                                       lpTempEntity ) );
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 2, "." );
            lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, sizeof( lpDataField->szFldName ),
                                          lpDataField->szFldName );
         }
      }

      // Close the qualification and the join statement.
      lpSql->pchFromEnd = zstrcatp( lpSql->pchFromEnd, 4, " ) " );

      // We've added a paren to the end of the FROM clause, so we need to add one to the beginning.
      *lpSql->pchNextJoinParen++ = '(';
      lpSql->pchNextJoinParen++;

   } // if ( lpSql->bUseLeftJoin ... )

   if ( lpSql->lpBoundList->pchEntityJoinTable )
   {
      LPVIEWENTITY lpParent;
      zPCHAR       pchEntityJoinTable = lpSql->lpBoundList->pchEntityJoinTable;

      // Now set the JOINEDWITHCHILD flag for the parent.  In addition, if
      // the child entity has a x-to-m relationship with its parent,
      // everything joined with the parent must have JOINEDWITHMULTCHILD
      // flag set.
      for ( lpParent = zGETPTR( lpViewEntity->hParent );
            lpParent;
            lpParent = zGETPTR( lpParent->hParent ) )
      {
         pchEntityJoinTable[ lpParent->nHierNbr ] |= zSELECT_JOINEDWITHCHILD;

         // If child entity has card max of 1, we're done.
         if ( lpViewEntity->uCardMax <= 1 )
            break;

         // Set multichild flag.
         pchEntityJoinTable[ lpParent->nHierNbr ] |= zSELECT_JOINEDWITHMULTCHILD;

         // If the parent is the entity we are loading, then we don't need to
         // go any higher.
         if ( lpParent == lpSql->lpLoadViewEntity )
            break;
      }
   }

   return;

}  /* fnAddTableToFrom */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlUnformatDateTimeString
//
// Converts different DateTime string formats to YYYYMMDDHHMMSSTTT.
// For example:
//
//    1:32 AM     -> 00010101013200000
//
// Returns: 1 - Everything ok
//          zCALL_ERROR - Oops.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlUnformatDateTimeString( zPCHAR pchTarget, zPCHAR pchSource )
{
   zPCHAR pchDefault;
   zCHAR  szStr[ 100 ];
   zLONG  k, j;
   zLONG  lLth;
   zSHORT nRC;

   // Copy source to temp string, elimating some spaces.  We can't eliminate
   // spaces that are between 2 digits.
   for ( k = j = 0; pchSource[ k ]; k++ )
   {
      if ( pchSource[ k ] == ' ' )
      {
         // Current char is a space--make sure we can eliminate it.

         // If j = 0, then no chars have been copied yet to the target string,
         // so pchSource has leading spaces--its OK to get rid of them.
         if ( j == 0 )
            continue;   // Continue loop without copying the space.

         // The only time we can't eliminate a space is if it's the only char
         // between two digits.  Make sure that the previous char in the
         // target string and the next char in the source string aren't #'s.
         if ( isdigit( szStr[ j - 1 ] ) && isdigit( pchSource[ k + 1 ] ) )
         {
            ; // Can't skip space.
         }
         else
            continue;   // Continue loop without copying the space.
      }

      // Copy the char from source to target.
      szStr[ j++ ] = pchSource[ k ];
   }

   szStr[ j ] = 0;

   nRC = zCALL_ERROR;

   // If the date string is 8 chars or more and of valid length
   //  and consists only of digits, then we assume that it is already
   //  in the right format.
   //  A valid length is
   //    8  - YYYYMMDD
   //    10 - YYYYMMDDHH
   //    12 - YYYYMMDDHHMM
   //    14 - YYYYMMDDHHMMSS
   //    17 - YYYYMMDDHHMMSSTTT
   //  Then, we just fill it up with '0' chars.
   j = zstrlen( szStr );
   if ( j == 8 || j == 10 || j == 12 || j == 14 || j == 17 )
   {
      for ( k = 0; k < j; k++ )
      {
         if ( !isdigit( szStr[ k ] ) )
            break;
      }

      if ( k == j )
      {
        // nothing but digits
        strcpy_s( pchTarget, 18, szStr );
        for ( k = j; k < 17; k++ )
          pchTarget[ k ] = '0';

        pchTarget[ 17 ] = 0;
        return( 1 );
      }
   }

   // Now find a valid format that matches szStr and unformat it.  Note that
   // the order of the UfEditFormatDateTime calls is important--the more
   // complex ones MUST be done FIRST.  UfEditFormatDateTime returns 0 only
   // if the data in szStr matches the format given.
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "YYYYMMDD" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "YYYY-M-D H:MI:SSAM" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "D-Mmm-YYYY H:MI:SSAM" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "D-Mmm-YYYY" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "YYYY-M-D H:MI:SS" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "YYYY-M-D H:MIAM" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "YYYY-M-D H:MI" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "YYYY-M-D" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "H:MI:SSAM" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "H:MI:SS" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "H:MIAM" ) == 0 )
      nRC = 1;
   else
   if ( UfEditFormatDateTime( szStr, sizeof( szStr ), "H:MI" ) == 0 )
      nRC = 1;

   if ( nRC == 1 )
   {
      // We formatted the date-time string.  If the date-time string is
      // incomplete, then we need to fill in default data.  For example if
      // the date-time string is "1996-04-02" (doesn't have time data), then
      // the string szStr will look like "19960402XXXXXXXXX" -- X's for the
      // unspecified data.  We need to set the X's to default values.

      // Changes any capitol X's to reflect the default.
      pchDefault = "19000101000000000";       // i.e. Jan 1, 1900, 00:00:00.000
      lLth = zstrlen( szStr );
      for ( k = 0; k < lLth; k++ )
      {
         if ( szStr[ k ] == 'X' )
            szStr[ k ] = pchDefault[ k ];
      }

      strcpy_s( pchTarget, 18, szStr );
   }

   return( nRC );

}  /* fnSqlUnformatDateTimeString */

/////////////////////////////////////////////////////////////////////////////
//
// SqlAttributeIsNull
//
// Returns TRUE or FALSE depending on whether current attribute defined
// by lpDataField is NULL.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlAttributeIsNull( zVIEW          lpView,
                    LPDATAFIELD    lpDataField,
                    LPBOUNDLIST    lpBoundList )
{
   zSHORT       nRC;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;

   if ( EXEC_CALLBACK( zSQLCALLBACK_CHECKNULL, lpView, lpDataField, 0, 0 ) )
   {
      // Callback routine returns following:
      //    1 - Attribute is null.
      //    0 - Callback didn't determine -- use default processing.
      //   -1 - Attribute is not null.
      if ( nRC == 1 )
         return( TRUE );
      else
      if ( nRC == -1 )
         return( FALSE );
   }

   lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
   lpViewEntity = zGETPTR( lpViewAttrib->hViewEntity );

   switch ( lpViewAttrib->cType )
   {
      case zTYPE_STRING:
      {
         zULONG  uLth;
         zPCHAR  szStr;
         zCHAR   chType;

         // In the special case where the string is stored as a fixed-length
         // string the attribute is never considered null.
         if ( lpDataField->cFldType == zTYPE_FIXEDCHAR )
            return( FALSE );

         GetValueFromRecord( lpView, lpViewEntity, lpViewAttrib, &szStr,
                             &chType, &uLth );
         if ( szStr == 0 || szStr[ 0 ] == 0 )
            return( TRUE );
         else
            return( FALSE );
      }

      case zTYPE_BLOB:
      {
         zULONG  uLth;
         zPCHAR  szStr;
         zCHAR   chType;
         GetValueFromRecord( lpView, lpViewEntity, lpViewAttrib, &szStr,
                             &chType, &uLth );
         if ( szStr == 0 || uLth == 0 )
            return( TRUE );
         else
            return( FALSE );
      }

      default:
      {
         zCHAR sz[ 25 ];

         // Get string from record with a max length of 25. We're just
         // checking to see if the attribute is null, so we don't care
         // about the return string.
         if ( GetStringFromRecord( lpView, lpViewEntity, lpViewAttrib,
                                   sz, 20 ) == -1 )
         {
            return( TRUE );
         }
         else
            return( FALSE );
      }
   }

} // SqlAttributeIsNull

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlGetNullString
//
// Returns in pchValue the value that represents NULL for a particular
// database.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlGetNullString( zPCHAR         pchValue,
                    LPDATAFIELD    lpDataField,
                    LPBOUNDLIST    lpBoundList )
{
   zSHORT nRC;

   // If pfnCallback is specified try to get the null string from the dbhandler.
   if ( EXEC_CALLBACK( zSQLCALLBACK_GETNULLSTRING, 0, lpDataField, 0, pchValue ) )
   {
      // Callback handled it.
      return( 0 );
   }

   strcpy_s( pchValue, 5, "null" );
   return( 0 );

} // SqlGetNullString

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlGetUpperString
//
// Returns in pchValue the value that represents UPPER for a particular
// database.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlGetUpperString( zPCHAR       pchValue,
                     LPBOUNDLIST  lpBoundList )
{
   zSHORT nRC;

   // If pfnCallback is specified try to get the UPPER string from the dbhandler.
   if ( EXEC_CALLBACK( zSQLCALLBACK_GETUPPERSTRING, 0, 0, 0, pchValue ) )
   {
      // Callback handled it.
      return( 0 );
   }

   strcpy_s( pchValue, 6, "UPPER" );

   return( 0 );

} // SqlGetNullString

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlConvertStringToSqlString
//
// Converts a string value to a string value that the SQL database
// understands.  For example:
//
//    abcd        -> 'abcd'
//    abc'd       -> 'abc''d'
//    1234        -> 1234
//    19930516    -> '1993-05-16'
//
// Returns: 0 - Everything ok
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlConvertStringToSqlString( LPDATAFIELD  lpDataField,
                               zPCHAR       pchSrc,
                               zPCHAR       pchTgt,
                               LPBOUNDLIST  lpBoundList )
{
   zSHORT       nRC;
   zPCHAR       sz;
   zCHAR        szDateTime1[ 30 ], szDateTime2[ 30 ];
   LPVIEWATTRIB lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

   // If source is null, then return value for null string.
   if ( pchSrc == 0 || *pchSrc == 0 )
   {
      fnSqlGetNullString( pchTgt, lpDataField, lpBoundList );
      return( 0 );
   }

   if ( EXEC_CALLBACK( zSQLCALLBACK_CONVERTSTRING, 0,
                       lpDataField, pchSrc, pchTgt ) )
   {
      // Callback handled it.
      return( 0 );
   }

   switch ( lpViewAttrib->cType )
   {
      case zTYPE_BLOB:
         // At least for now we better never get a blob.
         SysMessageBox( 0, "Zeidon Core",
                        "Blobs not supported as string attributes", 1 );
         return( zCALL_ERROR );

      case zTYPE_STRING:
      {
         // Go through the string and check for single quotes and double them
         // up (ie "don't" becomes "don''t").
         *pchTgt++ = '\'';

         // If there are any imbedded single quotes then double them up.
         while ( *pchSrc )
         {
            if ( *pchSrc == '\'' )
            {
               *pchTgt++ = '\'';
               *pchTgt++ = '\'';
               *pchSrc++;
            }
            else
               *pchTgt++ = *pchSrc++;
         }

         // In the special case where the string is stored as a fixed-length
         // string make sure the length is set correctly.
         if ( lpDataField->cFldType == zTYPE_FIXEDCHAR )
         {
            zLONG k;

            for ( k = zstrlen( pchSrc );
                  k < lpDataField->uFldLth;
                  k++ )
            {
               *pchTgt++ = ' ';
            }
         }

         *pchTgt++ = '\'';
         *pchTgt   = 0;
         break;
      }

      case zTYPE_DATETIME:
         // Get date-time from pchSrc as an unformatted string --
         // yyyymmddhhmmsss.
         strcpy_s( szDateTime1, sizeof( szDateTime1 ), pchSrc );

         // Make sure that the DateTime doesn't contain any spaces.
         sz = szDateTime1;
         while ( sz[ 0 ] )
         {
            if ( sz[ 0 ] == ' ' )
               sz[ 0 ] = '0';

            sz++;
         }

         // Now format for date/time/datetime.
         switch ( lpDataField->cFldType )
         {
            case zTYPE_TIME:
               UfFormatDateTime( szDateTime2, sizeof( szDateTime2 ), szDateTime1, "HH.MI.SS.999" );
               break;

            case zTYPE_DATE:
               UfFormatDateTime( szDateTime2, sizeof( szDateTime2 ), szDateTime1, "YYYY-mm-DD" );
               break;

            default:
               UfFormatDateTime( szDateTime2, sizeof( szDateTime2 ), szDateTime1, "YYYY-mm-DD-HH.MI.SS.999" );
         }

         // Date string must be inside a quote.
         sprintf_s( pchTgt, 30, "'%s'", szDateTime2 );
         break;

      default:
         // For all other datatypes, just copy it to pchTgt.
         strcpy_s( pchTgt, 30, pchSrc );
         break;

   } // switch ( lpDataField->cFldType )...

   return( 0 );

}  /* fnSqlConvertStringToSqlString */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlGetAttributeValue
//
// Retrieve the string value of an attribute and concats it to the end
// of the:
//
//       INSERT if current SQL cmd is an INSERT
//       UPDATE if current SQL cmd is an UPDATE and attrib is not a key.
//          --or--
//       WHERE clause if not.
//
// Returns: 0 - Everything ok
//          1 - Everything ok, but attribute value is null.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlGetAttributeValue( zVIEW          lpView,
                        LPDATAFIELD    lpDataField,
                        LPSQLSTATEMENT lpSql )
{
   zSHORT  nRC;
   zCHAR   chType;
   zPCHAR  szStr;
   zPCHAR  pchValue;
   zULONG  uLth;
   zBOOL   bSqlCmd;
   zBOOL   bBound;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;

   nRC = 0;    // Assume that attribute is not null.
   lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
   lpViewEntity = zGETPTR( lpViewAttrib->hViewEntity );
   bBound = FALSE;

   // If the command is an INSERT or UPDATing a non-key then add value to
   // main SQL command, otherwise add value to the WHERE part of command.
   if ( (lpSql->nCommandType & zINSERT_CMD) ||
        (lpSql->nCommandType & zUPDATE_CMD && lpViewAttrib->bKey == FALSE) )
   {
      pchValue = lpSql->pchSqlCmdEnd;
      bSqlCmd = TRUE;
   }
   else
   {
      pchValue = lpSql->pchWhereEnd;
      bSqlCmd = FALSE;
   }

   // If callback function is supplied see if it will get the value.
   if ( EXEC_CALLBACK( zSQLCALLBACK_GETATTRVALUE, lpView,
                       lpDataField, 0, pchValue ) )
   {
      // The DBHandler call back routine retrieved the attribute value.
      // Check for null value.
      if ( SqlAttributeIsNull( lpView, lpDataField, lpBoundList ) )
         nRC = 1;
      else
         nRC = 0;
   }
   else
   {
      // If attribute is null then insert language specific null.
      if ( SqlAttributeIsNull( lpView, lpDataField, lpBoundList ) )
      {
         fnSqlGetNullString( pchValue, lpDataField, lpBoundList );
         nRC = 1;    // Indicate that attribute is null.
      }
      else
      switch ( lpViewAttrib->cType )
      {
         zCHAR sz[ 300 ];

         case zTYPE_STRING:
            GetValueFromRecord( lpView, lpViewEntity, lpViewAttrib, &szStr,
                                &chType, &uLth );
            fnSqlConvertStringToSqlString( lpDataField,
                                           szStr, pchValue, lpBoundList );
            break;

         case zTYPE_DATETIME:
            // Get date-time from record as an unformatted string --
            // yyyymmddhhmmsss.
            GetStringFromRecord( lpView, lpViewEntity, lpViewAttrib, sz, 20 );
            fnSqlConvertStringToSqlString( lpDataField, sz, pchValue,
                                           lpBoundList );
            break;

         case zTYPE_DECIMAL:
            GetStringFromRecord( lpView, lpViewEntity, lpViewAttrib, sz, 256 );
            fnSqlConvertStringToSqlString( lpDataField, sz, pchValue,
                                           lpBoundList );
            break;

         default:
            // For all other datatypes, get the value of the attribute as a
            // string and put it in pchValue.

            // Get string from record with a max length of 256.
            GetStringFromRecord( lpView, lpViewEntity, lpViewAttrib, sz, 256 );
            fnSqlConvertStringToSqlString( lpDataField, sz, pchValue,
                                           lpBoundList );
            break;
      }
   }

   if ( bSqlCmd )
      mAdvanceToNull( lpSql->pchSqlCmdEnd );
   else
      mAdvanceToNull( lpSql->pchWhereEnd );

   return( nRC );
}  /* fnSqlGetAttributeValue */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlBuildColumnList
//
// Build a list of the columns i.e. "Col1, Col2, Col3...".
// Uses fnSqlAddStringToSql(...) to add string to SQL statement.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlBuildColumnList( LPVIEWENTITY   lpViewEntity,
                      LPSQLSTATEMENT lpSql,
                      zSHORT         nControl )
{
   LPVIEWATTRIB lpViewAttrib;
   LPDATAFIELD  lpDataField;
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;
   zCHAR        szColumnName[ 256 ];
   zBOOL        bFirstTime;
   zSHORT       nRC;

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

   bFirstTime = TRUE;
   for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
         lpDataField;
         lpDataField = zGETPTR( lpDataField->hNextDataField ) )
   {
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

      // Hartmut thinks we shouldn't look for foreign keys.
      if ( (nControl & zKEYS_ONLY) &&
           (lpViewAttrib->bKey == FALSE) )
        // (!lpViewAttrib->bKey && !lpViewAttrib->bForeignKey) )
      {
         continue;
      }

      // If nControl indicates that we don't want hidden attributes, then
      // don't use hidden attributes UNLESS THEY ARE KEYS.  Keys, even if
      // they are hidden, should be included.  Same thing with auto sequencing
      // attributes.
      if ( (nControl & zNO_HIDDEN) &&
           (lpViewAttrib->bHidden && lpViewAttrib->bKey == FALSE &&
                                     lpViewAttrib->bForeignKey == FALSE &&
                                     lpViewAttrib->bAutoSeq == FALSE) )
      {
         continue;
      }

      // If the attribute is an Auto Seq attribute and the relationship
      // is many-to-many then the attribute is stored in the corrsponding
      // table.  If the command type is also INSERT then the attribute is
      // not to be included in this list.
      if ( lpViewAttrib->bAutoSeq &&
           lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY &&
           lpSql->nCommandType & zINSERT_CMD )
      {
         continue;
      }

      // Check to see if we only want updated attributes (for INSERT).
      if ( nControl & zUPDATED_ONLY )
      {
         LPATTRIBFLAGS lpAttribFlags;

         GetAttributeFlags( &lpAttribFlags, lpSql->lpView,
                            lpViewEntity, lpViewAttrib );

         // If the attribute hasn't been updated then forget it.
         if ( lpAttribFlags->u.bFlags.bUpdated == FALSE )
            continue;
      }

      // If we are creating a SELECT command and the entity has an attribute
      // that has a db operation, then check to see if the current attribute
      // has a db operation.  If not, then skip the attribute.
      if ( lpSql->nCommandType & zSELECT_CMD &&
           lpViewEntity->bHasDB_Oper && lpViewAttrib->cDB_Oper == 0 )
      {
         continue;
      }

      szColumnName[ 0 ] = 0; // init szColumnName to null string

      if ( lpSql->nCommandType & zSELECT_CMD )
      {
         switch ( lpViewAttrib->cDB_Oper )
         {
            case 'A':
               strcat_s( szColumnName, sizeof( szColumnName ), "AVG( " );
               break;

            case 'C':
               strcat_s( szColumnName, sizeof( szColumnName ), "COUNT( " );
               break;

            case 'N':
               strcat_s( szColumnName, sizeof( szColumnName ), "MIN( " );
               break;

            case 'S':
               strcat_s( szColumnName, sizeof( szColumnName ), "SUM( " );
               break;

            case 'X':
               strcat_s( szColumnName, sizeof( szColumnName ), "MAX( " );
               break;
         }
      }

      // Add table names if columns need to be fully qualified.
      // We don't want the table name if a COUNT Sql Function is generated.
      if ( (nControl & zFULLY_QUALIFIED) && lpViewAttrib->cDB_Oper != 'C' )
      {
         if ( lpViewAttrib->bAutoSeq &&
              lpRelRecord &&
              lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
         {
            // Qualify column using corresponding table.
            strcat_s( szColumnName, sizeof( szColumnName ),
                      fnTableName( lpSql,
                                   lpRelRecord->szRecordName,
                                   lpViewEntity ) );
         }
         else
            strcat_s( szColumnName, sizeof( szColumnName ),
                      fnTableName( lpSql,
                                   lpDataRecord->szRecordName,
                                   lpViewEntity ) );

         strcat_s( szColumnName, sizeof( szColumnName ), "." );
      }

      if ( EXEC_CALLBACK( zSQLCALLBACK_GETCOLUMNNAME, 0, lpDataField,
                          (lpSql->nCommandType & zINSERT_CMD),
                          szColumnName ) )
      {
         // Callback handled it.
      }
      else
      {
         // Default processing is to just copy the column name.
         strcat_s( szColumnName, sizeof( szColumnName ), lpDataField->szFldName );
      }

      if ( lpSql->nCommandType & zSELECT_CMD && lpViewAttrib->cDB_Oper )
         strcat_s( szColumnName, sizeof( szColumnName ), " )" );

      // Some columns need to go at the end of the list.  Check with the DBH
      // if this column should do so but only if we are dealing with a
      // SELECT command.
      if ( lpSql->nCommandType & zSELECT_CMD &&
           EXEC_CALLBACK( zSQLCALLBACK_GETCOLLISTPOS, 0, lpDataField,
                          (lpSql->nCommandType & zINSERT_CMD), 0 ) )
      {
         // Callback indicated that column goes in 2nd column list.
         lpSql->pchColList2End = zstrcatp( lpSql->pchColList2End, 3, ", " );
         lpSql->pchColList2End = zstrcatp( lpSql->pchColList2End, sizeof( szColumnName ), szColumnName );
      }
      else
      {
         // Default processing is to copy the column name to the first list.
         if ( bFirstTime )
            bFirstTime = FALSE;
         else
            fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );

         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, szColumnName );
      }

   } // for (; lpDataField; ... )...

   return( 0 );

}  /* fnSqlBuildColumnList */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlBuildIntoList
//
// Build an 'INTO' list.  For example:
//
//    SELECT ...
//    INTO :Var1, :Var2, :Var3...
//    WHERE...
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlBuildIntoList( LPVIEWENTITY   lpViewEntity,
                    LPSQLSTATEMENT lpSql )
{
   LPVIEWATTRIB lpViewAttrib;
   LPDATAFIELD  lpDataField;
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;
   zCHAR        szColumnName[ 200 ];
   zBOOL        bFirstTime;
   zSHORT       nRC;

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " INTO " );

   bFirstTime = TRUE;
   for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
         lpDataField;
         lpDataField = zGETPTR( lpDataField->hNextDataField ) )
   {
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

      // We don't want hidden attributes UNLESS THEY ARE KEYS.  Keys, even if
      // they are hidden, should be included.  Same thing with auto sequencing
      // attributes.
      if ( lpViewAttrib->bHidden && lpViewAttrib->bKey == FALSE &&
                                    lpViewAttrib->bForeignKey == FALSE &&
                                    lpViewAttrib->bAutoSeq == FALSE )
      {
         continue;
      }

      // If the attribute is an Auto Seq attribute and the relationship
      // is many-to-many then the attribute is stored in the corrsponding
      // table.  If the command type is also INSERT then the attribute is
      // not to be included in this list.
      if ( lpViewAttrib->bAutoSeq &&
           lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY &&
           lpSql->nCommandType & zINSERT_CMD )
      {
         continue;
      }

      // If we are creating a SELECT command and the entity has an attribute
      // that has a db operation, then check to see if the current attribute
      // has a db operation.  If not, then skip the attribute.
      if ( lpSql->nCommandType & zSELECT_CMD &&
           lpViewEntity->bHasDB_Oper && lpViewAttrib->cDB_Oper == 0 )
         continue;

      if ( bFirstTime )
         bFirstTime = FALSE;
      else
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );

      if ( EXEC_CALLBACK( zSQLCALLBACK_GETINTONAME, 0, lpDataField, 0,
                          szColumnName ) )
      {
         // Callback handled it.
      }
      else
      {
         // Default processing is to just copy the attribute name.
         strcpy_s( szColumnName, sizeof( szColumnName ), ":" );
         strcat_s( szColumnName, sizeof( szColumnName ), lpViewAttrib->szName );
      }

      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, szColumnName );

   } // for (; lpDataField; ... )...

   return( 0 );

}  /* fnSqlBuildIntoList */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlBuildColumnValueList
//
// Builds a list values taken from the entity instance
//    (e.g.  "'Larry Bird' , '10 Celtic Ave' , 02134 , ...").
// Used only for insert.
//
// Returns: 0 - Everything OK
//          1 - Everything OK, but some attributes need to be bound.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlBuildColumnValueList( LPVIEWENTITY   lpViewEntity,
                           LPSQLSTATEMENT lpSql,
                           zSHORT         nControl )
{
   zBOOL        bFirstTime;
   zSHORT       nLongCharCount;
   LPVIEWATTRIB lpViewAttrib;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPRELRECORD  lpRelRecord;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;

   lpDataRecord  = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord   = zGETPTR( lpDataRecord->hFirstRelRecord );

   nLongCharCount = 0;
   bFirstTime     = TRUE;
   for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
         lpDataField;
         lpDataField = zGETPTR( lpDataField->hNextDataField ) )
   {
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

      // If the attribute is not persistent then try again with next field.
      if ( lpViewAttrib->bPersist == FALSE )
         continue;

      // If the attribute is an auto seq attribute and the parent relationship
      // is many-to-many then the attribute is stored in the corresponding
      // table and not include in the column value list.
      if ( lpViewAttrib->bAutoSeq &&
           lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         continue;
      }

      // Check to see if we only want updated attributes (for INSERT).
      if ( nControl & zUPDATED_ONLY )
      {
         LPATTRIBFLAGS lpAttribFlags;

         GetAttributeFlags( &lpAttribFlags, lpSql->lpView,
                            lpViewEntity, lpViewAttrib );

         // If the attribute hasn't been updated then forget it.
         if ( lpAttribFlags->u.bFlags.bUpdated == FALSE )
            continue;
      }

      // If column is not the first then append comma.
      if ( bFirstTime )
         bFirstTime = FALSE;
      else
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " , " );

      // Insert the text value of the attribute.  It is possible that instead
      // of the value we will insert a place-holder for bound attributes.
      fnSqlGetAttributeValue( lpSql->lpView, lpDataField, lpSql );

   } // for (; lpDataField )...

   if ( lpBoundList->nBoundCount > 0 )
      return( 1 );
   else
      return( 0 );

}  /* fnSqlBuildColumnValueList */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlBuildWhere
//
// Build "WHERE col-name = attrib-value [ AND col-name2 = attrib-value2...]
// for each of the keys in lpViewEntity.
//
// Used by Update and Delete.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlBuildWhere( zVIEW          lpView,
                 LPVIEWOD       lpViewOD,
                 LPVIEWENTITY   lpViewEntity,
                 LPSQLSTATEMENT lpSql,
                 zSHORT         nQual )
{
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPVIEWATTRIB lpViewAttrib;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;
   zSHORT       bKeyFound = FALSE;

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );

   // Start dks ... 2006.01.25
   if ( nQual & zFOREIGN_KEY )
   {
      LPRELRECORD  lpRelRecord = zGETPTR( lpDataRecord->hFirstRelRecord );
      LPRELFIELD   lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
      LPDATAFIELD  lpSrcDataField = zGETPTR( lpRelField->hSrcDataField );
      LPDATAFIELD  lpRelDataField = zGETPTR( lpRelField->hRelDataField );

      TraceLineS( "fnSqlBuildWhere found ForeignKey: ", lpRelDataField->szFldName );
      TraceLineS( "fnSqlBuildWhere found Attribute: ", lpSrcDataField->szFldName );

      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " ( " );
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpRelDataField->szFldName );
      if ( SqlAttributeIsNull( lpView, lpSrcDataField, lpBoundList ) )
      {
         // If the attribute is null, then write 'null'-value into the
         // statement 29.10.1997, RG
         zCHAR pchValue[ 25 ];

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " IS " );
         fnSqlGetNullString( pchValue, lpSrcDataField, lpBoundList );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, pchValue );
      }
      else
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );
         fnSqlGetAttributeValue( lpView, lpSrcDataField, lpSql );
      }

      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " )" );
      return( 0 );
   }
   // End dks ... 2006.01.25

   for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
         lpDataField;
         lpDataField = zGETPTR( lpDataField->hNextDataField ) )
   {
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

      if ( lpViewAttrib->bKey == FALSE )
         continue;

      bKeyFound = TRUE;
      if ( fnSqlWhereIsEmpty( lpSql ) )
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " ( " );
      else
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );

      if ( nQual == zFULLY_QUALIFIED )
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, fnTableName( lpSql,
                                                    lpDataRecord->szRecordName,
                                                    lpViewEntity ) );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
      }

      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );
      if ( SqlAttributeIsNull( lpView, lpDataField, lpBoundList ) )
      {
         // If the attribute is null, then write 'null'-value into the
         // statement 29.10.1997, RG
         zCHAR pchValue[ 25 ];

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " IS " );
         fnSqlGetNullString( pchValue, lpDataField, lpBoundList );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, pchValue );
      }
      else
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );
         fnSqlGetAttributeValue( lpView, lpDataField, lpSql );
      }
   } // for ( ... )...

   if ( bKeyFound == FALSE )
   {
      zCHAR szMsg[ 400 ];

      strcpy_s( szMsg, sizeof( szMsg ), "SQL Error creating WHERE clause -- no key attribute "
               "found in object definition.\nObject name = " );
      strcat_s( szMsg, sizeof( szMsg ), lpViewOD->szName );
      strcat_s( szMsg, sizeof( szMsg ), "\nEntity name = " );
      strcat_s( szMsg, sizeof( szMsg ), lpViewEntity->szName );
      IssueError( lpView, 16, 16, szMsg );

      return( zCALL_ERROR );
   }

   fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " )" );
   return( 0 );

}  /* fnSqlBuildWhere */

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildInsert
//
// Returns: 0           - Everything OK.
//          zCALL_ERROR - Processing error!
//
//    For SqlBase:
//          1 - Everything worked OK, but some long varchars must be bound.
//
//    For SqlServer:
//          1 - Everything OK, but blob and/or text cols need to be written.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildInsert( zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zPCHAR       pchSqlCmd,
                LPBOUNDLIST  lpBoundList )
{
   LPDATARECORD   lpDataRecord;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zSHORT         nFlag;
   zSHORT         nRC;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zINSERT_CMD, pchSqlCmd, lpView, lpBoundList );
   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "INSERT INTO " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataRecord->szRecordName );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " ( " );

   // Check to see if we should only inserted updated attributes.
   if ( CHECK_CALLBACK_FLAG( zSQLCALLBACK_UPDATEDONLY ) )
      nFlag = zUPDATED_ONLY;
   else
      nFlag = 0;

   nRC = fnSqlBuildColumnList( lpViewEntity, lpSql, nFlag );
   if ( nRC < 0 )
   {
      fnSqlFreeSqlStatement ( lpSql );
      return( zCALL_ERROR );
   }

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " ) VALUES ( " );
   nRC = fnSqlBuildColumnValueList( lpViewEntity, lpSql, nFlag );
   if ( nRC < 0 )
   {
      fnSqlFreeSqlStatement ( lpSql );
      return( zCALL_ERROR );
   }

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " )" );

   fnSqlFreeSqlStatement( lpSql );
   return( nRC );
}  /* SqlBuildInsert */

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildInsertCorrTable
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildInsertCorrTable( zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd,
                         LPBOUNDLIST  lpBoundList )
{
   LPVIEWATTRIB   lpViewAttrib;
   LPDATARECORD   lpDataRecord;
   LPDATAFIELD    lpDataField;
   LPRELRECORD    lpRelRecord;
   LPRELFIELD     lpRelField;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zINSERT_CMD | zCORR_CMD, pchSqlCmd,
                          lpView, lpBoundList );
   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "INSERT INTO " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpRelRecord->szRecordName );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " ( " );

   // Add list of corr table column names to sql cmd.
   lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
   while ( lpRelField )
   {
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpRelField->szFldName );

      lpRelField = zGETPTR( lpRelField->hNextRelField );
      if ( lpRelField )
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );
   }

   // If the entity has a seq attribute, that also must be inserted.
   if ( lpViewEntity->bAutoSeq )
   {
      lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
      while ( lpDataField )
      {
         lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
         if ( lpViewAttrib->bAutoSeq )
         {
            fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );
            fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );
            break;  // Only 1 seq per entity, so break out.
         }

         lpDataField = zGETPTR( lpDataField->hNextDataField );
      } // while ( lpDataField )...
   } // if ( lpViewEntity->bAutoSeq )...
   else
      lpDataField = 0;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " ) VALUES ( " );

   // Add list of corr table column values to sql cmd.
   lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
   while ( lpRelField )
   {
      fnSqlGetAttributeValue( lpView, zGETPTR( lpRelField->hSrcDataField ),
                              lpSql );

      lpRelField = zGETPTR( lpRelField->hNextRelField );
      if ( lpRelField )
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );
   }

   // If a seq attribute was found earlier then add it to insert.
   if ( lpDataField )
   {
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );
      fnSqlGetAttributeValue( lpView, lpDataField, lpSql );
   }

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " )" );

   fnSqlAssembleSqlStatement( lpSql );
   return( 0 );
}  /* SqlBuildInsertCorrTable */

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildUpdate
//
// RETURNS: 0 - OK.
//          1 - Everything OK but some columns need to be bound.
//         -1 - No attributes in this table need to be updated.
//              (returned only if DBH updates changed attributes).
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildUpdate( zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zSHORT       bAutoSeqOnly,
                zPCHAR       pchSqlCmd,
                LPBOUNDLIST  lpBoundList )
{
   zBOOL          bFirstTime;
   zBOOL          bAttribUpdated;
   zBOOL          bUpdatedOnly;
   LPDATARECORD   lpDataRecord;
   LPDATAFIELD    lpDataField;
   LPVIEWATTRIB   lpViewAttrib;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zUPDATE_CMD, pchSqlCmd, lpView, lpBoundList );
   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "UPDATE " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataRecord->szRecordName );

   // Check to see if we should only updating changed attributes.
   if ( CHECK_CALLBACK_FLAG( zSQLCALLBACK_UPDATEDONLY ) )
      bUpdatedOnly = TRUE;
   else
      bUpdatedOnly = FALSE;

   // First create "Set col = val [, ...]" clause.
   bFirstTime     = TRUE;
   bAttribUpdated = FALSE;
   for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
         lpDataField;
         lpDataField = zGETPTR( lpDataField->hNextDataField ) )
   {
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

      // If this update is for autosequence fields only and the attribute is
      // not an autoseq attribute then do nothing for this attribute.
      if ( bAutoSeqOnly && lpViewAttrib->bAutoSeq == FALSE )
         continue;

      // Update only persistent attributes.
      if ( lpViewAttrib->bPersist == FALSE )
         continue;

      // No updating keys!
      if ( lpViewAttrib->bKey )
         continue;

      // Check to see if we only want updated attributes (for INSERT).
      if ( bUpdatedOnly )
      {
         LPATTRIBFLAGS lpAttribFlags;

         GetAttributeFlags( &lpAttribFlags, lpSql->lpView,
                            lpViewEntity, lpViewAttrib );

         // If the attribute hasn't been updated then forget it.
         if ( lpAttribFlags->u.bFlags.bUpdated == FALSE )
            continue;

         // If we get here then an attribute will be updated.
         bAttribUpdated = TRUE;
      }
      else
      {
         // If the attribute is hidden then skip it UNLESS the attribute is a:
         //    o Foreign key.
         //    o Sequence attribute.
         if ( lpViewAttrib->bHidden &&
              !( lpViewAttrib->bForeignKey || lpViewAttrib->bAutoSeq ) )
            continue;
      }

      if ( bFirstTime )
      {
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " SET " );
         bFirstTime = FALSE;
      }
      else
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " , " );

      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " = " );

      // Insert the text value of the attribute.  It is possible that instead
      // of the value we will insert a place-holder for bound attributes.
      fnSqlGetAttributeValue( lpView, lpDataField, lpSql );

   } // for ( lpDataField )...

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " " );
   if ( fnSqlBuildWhere( lpView, lpViewOD, lpViewEntity,
                         lpSql, 0 ) == zCALL_ERROR )
      return( zCALL_ERROR );

   fnSqlAssembleSqlStatement( lpSql );

   // If no attributes were updated then return -1.
   if ( bUpdatedOnly && bAttribUpdated == FALSE )
      return( -1 );

   // Return 1 if columns need to be bound, 0 if not.
   if ( lpBoundList->nBoundCount > 0 )
      return( 1 );

   return( 0 );

}  /* SqlBuildUpdate */

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildUpdateCorrTable
//
// Updates the auto seq attribute in the corr table.
//
// RETURNS:
//          0 - OK.
//         -1 - Nothing to update.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildUpdateCorrTable( zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd,
                         LPBOUNDLIST  lpBoundList )
{
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPRELRECORD  lpRelRecord;
   LPRELFIELD   lpRelField;
   LPVIEWATTRIB lpViewAttrib;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zBOOL        bUpdatedOnly;

   // Since we can only update a corr table if it has auto seq attributes,
   // exit if there are no seq attributes for the entity.
   if ( lpViewEntity->bAutoSeq == FALSE )
      return( zCALL_ERROR );

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );
   if ( lpRelRecord == 0 )
      return( zCALL_ERROR );

   if ( lpRelRecord->cOwnerMember != zDBH_MANY_TO_MANY )
      return( -1 );

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zUPDATE_CMD | zCORR_CMD, pchSqlCmd,
                          lpView, lpBoundList );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "UPDATE " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpRelRecord->szRecordName );

   // Check to see if we should only updating changed attributes.
   if ( CHECK_CALLBACK_FLAG( zSQLCALLBACK_UPDATEDONLY ) )
      bUpdatedOnly = TRUE;
   else
      bUpdatedOnly = FALSE;

   // First create "SET col = val" clause for AutoSeq attribute.
   // I know we already check for bAutoSeq, but I'm leaving the IF statment
   // here in case we need it in the future.
   if ( lpViewEntity->bAutoSeq )
   {
      // Find the Autoseq attribute.
      lpDataField  = zGETPTR( lpDataRecord->hFirstDataField );
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
      while ( lpViewAttrib->bAutoSeq == FALSE )
      {
         lpDataField  = zGETPTR( lpDataField->hNextDataField );
         lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
      }

      // Check to see if we only want updated attributes (for INSERT).
      if ( bUpdatedOnly )
      {
         LPATTRIBFLAGS lpAttribFlags;

         GetAttributeFlags( &lpAttribFlags, lpSql->lpView,
                            lpViewEntity, lpViewAttrib );

         // If the attribute hasn't been updated then forget it.
         if ( lpAttribFlags->u.bFlags.bUpdated == FALSE )
            return( -1 );
      }

      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " SET " );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " = " );
      fnSqlGetAttributeValue( lpView, lpDataField, lpSql );

   } // if ( lpViewEntity->bAutoSeq )...

   // Build WHERE clause.
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " WHERE ( " );
   lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
   while ( lpRelField )
   {
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpRelField->szFldName );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " = " );

      fnSqlGetAttributeValue( lpView, zGETPTR( lpRelField->hSrcDataField ),
                              lpSql );

      lpRelField = zGETPTR( lpRelField->hNextRelField );
      if ( lpRelRecord )
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " AND " );
   }

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " )" );

   fnSqlAssembleSqlStatement( lpSql );
   return( 0 );
}  /* SqlBuildUpdateCorrTable */

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildDelete
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildDelete( zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zPCHAR       pchSqlCmd,
                LPBOUNDLIST  lpBoundList )
{
   LPDATARECORD lpDataRecord;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zSHORT       nRC;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zDELETE_CMD, pchSqlCmd, lpView, lpBoundList );

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "DELETE FROM " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataRecord->szRecordName );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " " );

   nRC = fnSqlBuildWhere( lpView, lpViewOD, lpViewEntity, lpSql, 0 );

   fnSqlAssembleSqlStatement( lpSql );
   return( nRC );
}  /* SqlBuildDelete */

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildDeleteAll
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildDeleteAll( zVIEW        lpView,
                   LPVIEWOD     lpViewOD,
                   LPVIEWENTITY lpViewEntity,
                   zPCHAR       pchSqlCmd,
                   LPBOUNDLIST  lpBoundList )
{
#if 1

   LPDATARECORD lpDataRecord;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zSHORT       nRC;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zDELETE_CMD, pchSqlCmd, lpView, lpBoundList );

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "DELETE FROM " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataRecord->szRecordName );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " " );

   nRC = fnSqlBuildWhere( lpView, lpViewOD, lpViewEntity, lpSql, zFOREIGN_KEY );

   fnSqlAssembleSqlStatement( lpSql );
   return( nRC );

#else

   LPENTITYINSTANCE  lpEntityInstance;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITY      lpVE_Parent;
   LPENTITYINSTANCE  lpEI_Parent;

   lpViewCsr = zGETPTR( lpView->hViewCsr );

   // Validate that the entity exists.
   if ( fnValidViewEntity( &lpViewEntityCsr, lpView,
                           lpViewEntity->szName, 0 ) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( lpViewEntityCsr )
   {
      SqlStatementRecord SqlStatement;
      LPSQLSTATEMENT lpSql;
      LPDATARECORD lpDataRecord;
      LPDATAFIELD  lpDataField;
      LPVIEWATTRIB lpViewAttrib;
      zSHORT       bKeyFound;
   // zSHORT       nRC;

      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
      lpEI_Parent = zGETPTR( lpEntityInstance->hParent );
      lpVE_Parent = zGETPTR( lpViewEntity->hParent );
      lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
      TraceLineS( "DeleteAll Found lpEntityInstance: ", lpViewEntity->szName );
   // bDeleteRequired = FALSE;

      lpSql = &SqlStatement;
      fnSqlInitSqlStatement( lpSql, zDELETE_CMD, pchSqlCmd,
                             lpView, lpBoundList );

      lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );

      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "DELETE FROM " );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataRecord->szRecordName );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " " );

  // nRC = fnSqlBuildWhere( lpView, lpViewOD, lpViewEntity, lpSql, 0 );

      bKeyFound = FALSE;
      lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
      for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
            lpDataField;
            lpDataField = zGETPTR( lpDataField->hNextDataField ) )
      {
         lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

         if ( lpViewAttrib->bKey == TRUE )
         {
            bKeyFound = TRUE;
            TraceLine( "SqlBuildDeleteAll Key: %s  ERTok: 0x%08x  XVTok: %d",
                       lpViewAttrib->szName, lpViewAttrib->lERAttTok, lpViewAttrib->lXVAAttTok );
         }

         if ( lpViewAttrib->bForeignKey == TRUE )
            TraceLine( "SqlBuildDeleteAll ForeignKey: %s  ERTok: 0x%08x  XVTok: %d",
                       lpViewAttrib->szName, lpViewAttrib->lERAttTok, lpViewAttrib->lXVAAttTok );
      }

      bKeyFound = FALSE;
      lpDataRecord = zGETPTR( lpVE_Parent->hFirstDataRecord );
      for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
            lpDataField;
            lpDataField = zGETPTR( lpDataField->hNextDataField ) )
      {
         lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

         if ( lpViewAttrib->bKey == TRUE )
         {
            bKeyFound = TRUE;
            TraceLine( "SqlBuildDeleteAll Parent Key: %s  ERTok: 0x%08x  XVTok: %d",
                       lpViewAttrib->szName, lpViewAttrib->lERAttTok, lpViewAttrib->lXVAAttTok );
         }

         if ( lpViewAttrib->bForeignKey == TRUE )
         {
            TraceLine( "SqlBuildDeleteAll Parent ForeignKey: %s  ERTok: 0x%08x  XVTok: %d",
                       lpViewAttrib->szName, lpViewAttrib->lERAttTok, lpViewAttrib->lXVAAttTok );
         }
      }

      return( 1 );

#if 0
      if ( fnSqlWhereIsEmpty( lpSql ) )
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " ( " );
      else
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );

      if ( nQual == zFULLY_QUALIFIED )
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, fnTableName( lpSql,
                                                    lpDataRecord->szRecordName,
                                                    lpViewEntity ) );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
      }

      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );
      if ( SqlAttributeIsNull( lpView, lpDataField, lpBoundList ) )
      {
         // if the attribute is null, then write 'null'-value into the
         // statement 29.10.1997, RG
         zCHAR pchValue[ 25 ];

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " IS " );
         fnSqlGetNullString( pchValue, lpDataField, lpBoundList );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, pchValue );
      }
      else
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );
         fnSqlGetAttributeValue( lpView, lpDataField, lpSql );
      }

      if ( bKeyFound == FALSE )
      {
         zCHAR szMsg[ 400 ];

         strcpy_s( szMsg, sizeof( szMsg ), "SQL Error creating WHERE clause -- no key attribute "
                  "found in object definition.\nObject name = " );
         strcat_s( szMsg, sizeof( szMsg ), lpViewOD->szName );
         strcat_s( szMsg, sizeof( szMsg ), "\nEntity name = " );
         strcat_s( szMsg, sizeof( szMsg ), lpViewEntity->szName );
         IssueError( lpView, 16, 16, szMsg );

         return( zCALL_ERROR );
      }

      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " )" );
      return( 0 );

      fnSqlAssembleSqlStatement( lpSql );

   return( nRC );

#endif

   }

   return( FALSE );

#endif
}

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildDeleteCorrTable
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildDeleteCorrTable( zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd,
                         LPBOUNDLIST  lpBoundList )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   LPRELFIELD   lpRelField;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zBOOL        bFirstTime;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zDELETE_CMD | zCORR_CMD, pchSqlCmd,
                          lpView, lpBoundList );

   bFirstTime = TRUE;

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "DELETE FROM " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpRelRecord->szRecordName );
   fnSqlIncrementComponentCount( lpSql );

   lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
   while ( lpRelField )
   {
      if ( bFirstTime == FALSE )
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );
         fnSqlIncrementComponentCount( lpSql );
      }
      else
         bFirstTime = FALSE;

      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpRelField->szFldName );
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );

      fnSqlGetAttributeValue( lpView, zGETPTR( lpRelField->hSrcDataField ),
                              lpSql );

      lpRelField = zGETPTR( lpRelField->hNextRelField );
   }

   fnSqlAssembleSqlStatement( lpSql );

   return( 0 );
}  /* SqlBuildDeleteCorrTable */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlBuildForeignKeys
//
// Adds "col-name = attrib-value..." for any foreign key that is part of
// the lpViewEntity's parent path.
//
// Returns: 0 - Everything ok
//          1 - Everything ok, but one of the foriegn key's value is null.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlBuildForeignKeys( zVIEW          lpView,
                       LPVIEWENTITY   lpViewEntity,
                       LPSQLSTATEMENT lpSql,
                       zSHORT         nControl )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   LPRELFIELD   lpRelField;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;
   zBOOL        bAddAnd;
   zSHORT       nNull;
   zSHORT       nRC;

   // Check to see if the current entity was joined with the entity we are
   // loading.  If it was, then the FK qualification was already added.
   if ( lpSql->lpLoadViewEntity != lpViewEntity &&
        lpSql->bUseLeftJoin &&
        SqlEntityValidForJoin( lpViewEntity, lpSql->lpLoadViewEntity,
                               lpSql->lpFirstQualEntity ) )
   {
      return( 0 );
   }

   nRC          = 0;
   nNull        = 0;
   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

   fnSqlIncrementComponentCount( lpSql );

   // If the table name for lpViewEntity is not already part of the SELECT
   // statement then add it to the FROM clause.
   fnAddTableToFrom( lpSql, 0, lpViewEntity, 0 );

   // If we are using an outer join in this statement and if we are
   // adding the FK's for an entity that is not the root of the join then
   // qualification has already been added as part of the "LEFT JOIN"
   // statment.
   if ( lpSql->bUseLeftJoin &&
        lpSql->lpLoadViewEntity != lpViewEntity )
      return( 0 );

   if ( lpRelRecord->hFirstRelField )
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " ( " );

   // Add the rel fields (for the view entity) to the WHERE clause.
   bAddAnd = FALSE;
   for ( lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
         lpRelField;
         lpRelField = zGETPTR( lpRelField->hNextRelField ) )
   {
      if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         LPDATAFIELD  lpSrcDataField;
         LPVIEWATTRIB lpSrcViewAttrib;
         LPVIEWENTITY lpSrcViewEntity;

         lpSrcDataField  = zGETPTR( lpRelField->hSrcDataField );
         lpSrcViewAttrib = zGETPTR( lpSrcDataField->hViewAttrib );
         lpSrcViewEntity = zGETPTR( lpSrcViewAttrib->hViewEntity );

         // If we are using an outer join in this statement then some of
         // the qualification has already been added.
         if ( lpSql->bUseLeftJoin &&
              lpSrcViewEntity == lpViewEntity )
         {
            // Already added.  Continue with the next lpRelField.
            continue;
         }

         if ( bAddAnd )
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );
         else
            // We want to add an " AND " next time through the loop.
            bAddAnd = TRUE;

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, fnTableName( lpSql,
                                                    lpRelRecord->szRecordName,
                                                    lpViewEntity ) );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpRelField->szFldName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );

         // In m-m relationships, we need to qualify using the correspondence
         // table.  If the current rel field is stored in the correspondence
         // then we compare the column from above (from the c-table) with the
         // the column in the child (non-correspondence) table.
         if ( lpSrcViewEntity == lpViewEntity )
         {
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                   fnTableName( lpSql,
                                                lpDataRecord->szRecordName,
                                                lpViewEntity ) );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                   lpSrcDataField->szFldName );
         }
         else
         if ( nControl == zROOT_ENTITY )
         {
            // If we get here then we are building the foreign keys for the
            // entity that we are loading.  In this case the key values from
            // the parent entities have already been loaded so we only need
            // to copy the attribute values from the parent entities.

            nRC = fnSqlGetAttributeValue( lpView,
                                          zGETPTR( lpRelField->hSrcDataField ),
                                          lpSql );

            // If nRC == 1 then the attribute value is null, so set nNull
            // to 1.
            if ( nRC == 1 )
               nNull = 1;
         }
         else
         {
            LPDATARECORD lpSrcDataRecord =
                              zGETPTR( lpSrcViewEntity->hFirstDataRecord );

            // If we get here then we are building the foreign keys for an
            // entity/table that is being used to qualify the entity/table
            // that we are loading.  This means that the foreign key values
            // must come from a parent table that is part of the current
            // select.

            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                   fnTableName( lpSql,
                                                lpSrcDataRecord->szRecordName,
                                                lpSrcViewEntity ) );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpSrcDataField->szFldName );
         }
      } // if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      else
      if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_SOURCE )
      {
         LPDATAFIELD  lpRelDataField;
         LPDATAFIELD  lpSrcDataField;
         LPVIEWATTRIB lpRelViewAttrib;
         LPVIEWENTITY lpRelViewEntity;
         LPDATARECORD lpRelDataRecord;

         if ( bAddAnd )
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );
         else
            // We want to add an " AND " next time through the loop.
            bAddAnd = TRUE;

         lpSrcDataField  = zGETPTR( lpRelField->hSrcDataField );
         lpRelDataField  = zGETPTR( lpRelField->hRelDataField );
         lpRelViewAttrib = zGETPTR( lpRelDataField->hViewAttrib );
         lpRelViewEntity = zGETPTR( lpRelViewAttrib->hViewEntity );
         lpRelDataRecord = zGETPTR( lpRelViewEntity->hFirstDataRecord );

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                fnTableName( lpSql,
                                             lpRelDataRecord->szRecordName,
                                             lpRelViewEntity ) );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpRelDataField->szFldName );

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );

         if ( nControl == zSUB_ENTITY )
         {
            LPVIEWENTITY lpForeignEntity;
            LPVIEWATTRIB lpTempViewAttrib;
            LPDATARECORD lpForeignDataRecord;

            // If we get here then we are building the foreign keys for an
            // entity/table that is being used to qualify the entity/table
            // that we are loading.  This means that the foreign key values
            // must come from a parent table that is part of the current
            // select.

            lpTempViewAttrib    = zGETPTR( lpSrcDataField->hViewAttrib );
            lpForeignEntity     = zGETPTR( lpTempViewAttrib->hViewEntity );
            lpForeignDataRecord = zGETPTR( lpForeignEntity->hFirstDataRecord );

            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                   fnTableName( lpSql,
                                                lpForeignDataRecord->szRecordName,
                                                lpForeignEntity ) );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpSrcDataField->szFldName );

         }
         else
         {
            // If we get here then we are building the foreign keys for the
            // entity that we are loading.  In this case the key values from
            // the parent entities have already been loaded so we only need
            // to copy the attribute values from the parent entities.

            nRC = fnSqlGetAttributeValue( lpView, lpSrcDataField, lpSql );

            // If nRC == 1 then the attribute value is null, so set nNull
            // to 1.
            if ( nRC == 1 )
               nNull = 1;
         }
      } // if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_SOURCE )...
      else
      if ( lpRelRecord->cOwnerMember == zDBH_CHILD_IS_SOURCE )
      {
         zBOOL        bUseLiteralValue;
         LPVIEWENTITY lpSrcViewEntity;
         LPDATARECORD lpSrcDataRecord;
         LPDATAFIELD  lpRelDataField;
         LPDATAFIELD  lpSrcDataField;
         LPVIEWATTRIB lpSrcViewAttrib;
         LPVIEWATTRIB lpRelViewAttrib;
         LPVIEWENTITY lpRelViewEntity;
         LPDATARECORD lpRelDataRecord;

         if ( bAddAnd )
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );
         else
            // We want to add an " AND " next time through the loop.
            bAddAnd = TRUE;

         lpSrcDataField  = zGETPTR( lpRelField->hSrcDataField );
         lpRelDataField  = zGETPTR( lpRelField->hRelDataField );
         lpSrcViewAttrib = zGETPTR( lpSrcDataField->hViewAttrib );
         lpRelViewAttrib = zGETPTR( lpRelDataField->hViewAttrib );
         lpRelViewEntity = zGETPTR( lpRelViewAttrib->hViewEntity );
         lpRelDataRecord = zGETPTR( lpRelViewEntity->hFirstDataRecord );
         lpSrcViewEntity = zGETPTR( lpSrcViewAttrib->hViewEntity );
         lpSrcDataRecord = zGETPTR( lpSrcViewEntity->hFirstDataRecord );

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                fnTableName( lpSql,
                                             lpSrcDataRecord->szRecordName,
                                             lpSrcViewEntity ) );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpSrcDataField->szFldName );

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );

         if ( nControl == zROOT_ENTITY )
            bUseLiteralValue = TRUE;
         else
         {
            LPRELRECORD lpRelRecord;

            // If we get here then we are building the foreign keys for an
            // entity/table that is being used to qualify the entity/table
            // that we are loading.  This means that the foreign key values
            // must come from a parent table that is part of the current
            // select.

            // Note that lpRelViewEntity points to a parent of lpViewEntity.
            bUseLiteralValue = FALSE;

            lpRelRecord = zGETPTR( lpRelDataRecord->hFirstRelRecord );

            while ( lpRelViewAttrib->bKey && lpRelRecord &&
                    lpRelRecord->cOwnerMember == zDBH_CHILD_IS_SOURCE )
            {
               LPRELFIELD   lpTempRelField;

               // Find the rel field for the qualifying attribute.
               for ( lpTempRelField = zGETPTR( lpRelRecord->hFirstRelField );
                     lpTempRelField;
                     lpTempRelField = zGETPTR( lpTempRelField->hNextRelField ) )
               {
                  LPDATAFIELD lpTempDataField = zGETPTR( lpTempRelField->hSrcDataField );

                  if ( lpTempDataField->hViewAttrib ==
                                                lpRelDataField->hViewAttrib )
                     break;
               }

               if ( lpTempRelField )
               {
                  // Change the column we are qualifying on.
                  lpRelDataField  = zGETPTR( lpTempRelField->hRelDataField );
                  lpRelViewAttrib = zGETPTR( lpRelDataField->hViewAttrib );
                  lpRelViewEntity = zGETPTR( lpRelViewAttrib->hViewEntity );
                  lpRelDataRecord = zGETPTR( lpRelViewEntity->hFirstDataRecord );
                  lpRelRecord     = zGETPTR( lpRelDataRecord->hFirstRelRecord );

                  // If the new RelViewEntity is the parent of the entity we
                  // are loading then it has already been loaded so we can
                  // now use a literal value instead of a column value.
                  if ( lpRelViewEntity ==
                                 zGETPTR( lpSql->lpLoadViewEntity->hParent ) )
                     bUseLiteralValue = TRUE;
               }
               else
               {
                  LPVIEWOD lpViewOD;

                  lpViewOD = (LPVIEWOD) MiGetViewEntityForView( lpView, 0 );
                  TraceLineS( "(#11) View OD Name = ", lpViewOD->szName );
                  TraceLineS( "(#11) Building foreign keys for entity", lpViewEntity->szName );
                  TraceLineS( "(#11) RelViewAttrib = ", lpRelViewAttrib->szName );
                  SysMessageBox( lpView, "Zeidon SQL Generator Error",
                                 "Internal error #11--can't find RelRecord", 1 );
                  break;
               }
            } // while...
         }

         if ( bUseLiteralValue )
         {
            // If we get here then we are building the foreign keys for the
            // entity that we are loading.  In this case the key values from
            // the parent entities have already been loaded so we only need
            // to copy the attribute values from the parent entities.

            nRC = fnSqlGetAttributeValue( lpView, lpRelDataField, lpSql );

            // If nRC == 1 then the attribute value is null, so set nNull
            // to 1.
            if ( nRC == 1 )
               nNull = 1;
         }
         else
         {
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                   fnTableName( lpSql,
                                                lpRelDataRecord->szRecordName,
                                                lpRelViewEntity ) );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpRelDataField->szFldName );
         }
      } // if ( lpRelRecord->cOwnerMember == zDBH_CHILD_IS_SOURCE )...

   } // for ( lpRelField )...

   fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " )" );

   return( nNull );

}  /* fnSqlBuildForeignKeys */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlAddTableToSelect
//
// A join is being used in a SELECT statement.  Add the table name of
// lpViewEntity to the SQL command and add any foreign keys to the
// WHERE clause.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlAddTableToSelect( zVIEW          lpView,
                       LPVIEWENTITY   lpViewEntity,
                       zBOOL          bParentsOnly,
                       LPSQLSTATEMENT lpSql )
{
   LPDATARECORD lpDataRecord;
   LPVIEWENTITY lpParent;
   LPVIEWENTITY lpViewEntityList[ MAX_TABLES ];
   LPRELRECORD  lpRelRecord;
   zSHORT       nViewCount = 0;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;

   lpViewEntityList[ nViewCount++ ] = lpViewEntity;

   // Make sure that all the parent tables have been added to the select.
   // If we are using LEFT joins, we'll call ourselves recursively.  If not,
   // we'll loop through the parents and add them.  We have to do this because
   // if we are not using left joins we need to keep track of the tables
   // that we add.
   if ( lpSql->bUseLeftJoin )
   {
      lpParent = zGETPTR( lpViewEntity->hParent );
      lpDataRecord = zGETPTR( lpParent->hFirstDataRecord );
      lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

      if ( fnSqlFindTableName( lpDataRecord->szRecordName,
                               lpParent, lpSql, FALSE ) == -1 )
      {
         // Table not in select statement.  Add it.
         fnSqlAddTableToSelect ( lpView, lpParent, FALSE, lpSql );
      }
   }
   else
   {
      // We'll keep track of all the tables that haven't been added to the select
      // so that we can add foreign keys later.
      for ( lpParent = zGETPTR( lpViewEntity->hParent );
            lpParent;
            lpParent = zGETPTR( lpParent->hParent ) )
      {
         lpDataRecord = zGETPTR( lpParent->hFirstDataRecord );
         lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

         // If the table exists in the current select statement, then
         // stop searching.
         if ( fnSqlFindTableName( lpDataRecord->szRecordName,
                                  lpParent, lpSql, FALSE ) != -1 )
            break;

         fnAddTableToFrom( lpSql, 0, lpParent, 0 );

         // Save the entity for later.
         lpViewEntityList[ nViewCount++ ] = lpParent;
      }
   }

   if ( bParentsOnly == FALSE )
      fnAddTableToFrom( lpSql, 0, lpViewEntity, 0 );

   // If we are using left joins, then the qualification for the new tables
   // as already been added in the FROM clause so we're done.
   if ( lpSql->bUseLeftJoin )
      return( 0 );

   // Now that we've added all the table names to the table list, add the
   // qualification for the foreign keys.
   for ( nViewCount--; nViewCount >= 0; nViewCount-- )
   {
      lpDataRecord = zGETPTR( lpViewEntityList[ nViewCount ]->hFirstDataRecord );
      lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

      // If the ViewEntity doesn't have a rel record then don't try adding the
      // foreign keys (there aren't any!).  This is probably only valid if the
      // entity is the root.
      if ( lpRelRecord == 0 )
         continue;

      if ( !fnSqlWhereIsEmpty( lpSql ) )
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );

      fnSqlBuildForeignKeys( lpView, lpViewEntityList[ nViewCount ],
                             lpSql, zSUB_ENTITY );
   }

   return( 0 );

}  /* fnSqlAddTableToSelect */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlBuildSubselect
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlBuildSubselect( zVIEW          lpView,
                     LPQUALENTITY   lpQualEntity,
                     LPQUALATTRIB   lpQualAttrib,
                     LPSQLSTATEMENT lpParentSql )
{
   LPVIEWENTITY       lpViewEntity;
   LPDATARECORD       lpDataRecord;
   LPBOUNDLIST        lpBoundList = lpParentSql->lpBoundList;
   SqlStatementRecord SubselectSql;
   LPSQLSTATEMENT     lpSql;

   lpViewEntity = lpQualAttrib->lpViewEntity;
   lpDataRecord = lpQualAttrib->lpDataRecord;

   lpSql = &SubselectSql;
   fnSqlInitSqlStatement( lpSql, zSUBSELECT_CMD, 0, lpView, lpBoundList );

   // Since we're dealing with a sub-select, the set the sub level as 1 more
   // than the parent's sub level.
   lpSql->lpParentSql = lpParentSql;
   lpSql->nSubLevel   = lpParentSql->nSubLevel + 1;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpQualAttrib->szOper );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, " ( SELECT " );

   // Add the QualEntity table to pchFromTable so that fnSqlBuildColumnList
   // will use any generated alias names.  Last argument is TRUE which means
   // add the table no matter what.
   fnAddTableToFrom( lpSql, 0, lpViewEntity, AT_ALWAYS_ADD );

   fnSqlBuildColumnList( lpViewEntity, lpSql, zKEYS_ONLY | zFULLY_QUALIFIED );

   // Add table representing lpViewEntity to select statement.
   fnSqlAddTableToSelect( lpView, lpViewEntity, TRUE, lpSql );

   // If Subselect is further qualified, add qualification to WHERE clause.
   if ( lpQualAttrib->lpFirstSubQualAttrib )
   {
      fnSqlAddQualToWhere ( lpView, lpQualEntity,
                            lpQualAttrib->lpFirstSubQualAttrib, lpSql );
   }

   if ( fnSqlWhereIsNotEmpty( lpSql ) )
   {
      fnSqlAddStringToWhere( lpParentSql, MAX_SQLCMD_LENGTH, lpSql->pchSqlCmd );
      fnSqlAddStringToWhere( lpParentSql, MAX_SQLCMD_LENGTH, " FROM " );
      fnSqlAddStringToWhere( lpParentSql, MAX_SQLCMD_LENGTH, lpSql->pchFrom );
      fnSqlAddStringToWhere( lpParentSql, MAX_SQLCMD_LENGTH, " WHERE " );
      if ( lpSql->nComponentCount > 1 )
         fnSqlAddStringToWhere( lpParentSql, MAX_SQLCMD_LENGTH, " ( " );
      fnSqlAddStringToWhere( lpParentSql, MAX_SQLCMD_LENGTH, lpSql->pchWhere );
      if ( lpSql->nComponentCount > 1 )
         fnSqlAddStringToWhere( lpParentSql, MAX_SQLCMD_LENGTH, " ) " );
      fnSqlAddStringToWhere( lpParentSql, MAX_SQLCMD_LENGTH, " )" );
   }

   fnSqlFreeSqlStatement( lpSql );
   return( 0 );

}  /* fnSqlBuildSubselect */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlBuildIS_Clause
//
// IS clause is short hand for comparing the keys of one entity to the
// keys of another.  Go through all the data fields (looking for keys)
// and compare them in the where clause.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlBuildIS_Clause( LPQUALENTITY   lpQualEntity,
                     LPQUALATTRIB   lpQualAttrib,
                     LPSQLSTATEMENT lpSql )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;
   zVIEW        lpView;
   zSHORT       nKeyCnt;

   nKeyCnt      = 0;
   lpView       = lpQualAttrib->lpSourceView;
   lpViewEntity = lpQualAttrib->lpSourceViewEntity;
   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );

   for ( lpDataField  = zGETPTR( lpDataRecord->hFirstDataField );
         lpDataField;
         lpDataField = zGETPTR( lpDataField->hNextDataField ) )
   {
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
      if ( lpViewAttrib->bKey == FALSE )
         continue;

      if ( nKeyCnt > 0 )
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );

      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                             fnTableName( lpSql,
                                          lpDataRecord->szRecordName,
                                          lpViewEntity ) );
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );
      fnSqlGetAttributeValue( lpView, lpDataField, lpSql );

      nKeyCnt++;
   }

   return( 0 );

}  /* fnSqlBuildIS_Clause */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlAddQualToWhere
//
// Note that lpFirstQualAttrib is not necessarily the same as
// lpQualEntity->lpFirstQualAttrib.  It could be the first qual attrib
// for its parent qual attrib.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlAddQualToWhere( zVIEW          lpView,
                     LPQUALENTITY   lpQualEntity,
                     LPQUALATTRIB   lpFirstQualAttrib,
                     LPSQLSTATEMENT lpSql )
{
   LPQUALATTRIB lpQualAttrib;
   LPBOUNDLIST  lpBoundList = lpSql->lpBoundList;
   zCHAR        szStr[ 500 ];
   zSHORT       nRC;

   // Qualification stuff are components, so increment count.
   fnSqlIncrementComponentCount( lpSql );

   // Go through each of the QualAttrib's looking for tables that are not
   // already part of the select.
   for ( lpQualAttrib = lpFirstQualAttrib;
         lpQualAttrib;
         lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
   {
      // Add the table to the SELECT statement only if the QualAttrib has
      // a lpViewEntity and a lpViewAttrib.
      if ( lpQualAttrib->lpViewEntity == 0 || lpQualAttrib->lpViewAttrib == 0 )
         continue;

      // If lpSearchRecord->lpRecordName can't be found in the FromTable
      // then it needs to be added to the SELECT statement.
      if ( fnSqlFindTableName( lpQualAttrib->lpDataRecord->szRecordName,
                               lpQualAttrib->lpViewEntity,
                               lpSql, TRUE ) == -1 )
      {
         // Foreign keys are a component, so increment count.
         fnSqlIncrementComponentCount( lpSql );

         fnSqlAddTableToSelect( lpView, lpQualAttrib->lpViewEntity,
                                FALSE, lpSql );
      }
   } // for ( lpQualAttrib... )...

   if ( fnSqlWhereIsNotEmpty( lpSql ) )
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND ( " );
    else
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "( " );

   //===
   //===  At this point, all tables that are needed in the select have
   //===  been included in the SELECT.  All that remains to do is to
   //===  add the QualAttrib expressions to the SELECT statement.
   //===

   for ( lpQualAttrib = lpFirstQualAttrib;
         lpQualAttrib;
         lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
   {
      if ( lpQualAttrib->lpViewEntity == 0 )
      {
         // If there is no entity name for QualAttrib then the QualAttrib
         // has just an Oper.  Tack it on to the end of the WHERE clause.

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " " );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpQualAttrib->szOper );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " " );

         continue;
      }

      // QualAttrib has an entity name, so this is an expression.  Add
      // the expression to the WHERE clause.

      // Check to see if szOper matches certain commands.  If it does
      // then the oper preceeds a subselect.
      if ( lpQualAttrib->bExists || lpQualAttrib->bDoesNotExist )
      {
         nRC = fnSqlBuildSubselect( lpView, lpQualEntity,
                                    lpQualAttrib, lpSql );
         if ( nRC < 0 )
            return( zCALL_ERROR );
      }
      else
      if ( lpQualAttrib->bIs )
      {
         nRC = fnSqlBuildIS_Clause( lpQualEntity, lpQualAttrib, lpSql );
         if ( nRC < 0 )
            return( zCALL_ERROR );
      }
      else
      if ( lpQualAttrib->lpKeyList )
      {
         zCHAR szCmd[ 300 ];
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                fnTableName( lpSql,
                                    lpQualAttrib->lpDataRecord->szRecordName,
                                    lpQualAttrib->lpViewEntity ) );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpQualAttrib->lpDataField->szFldName );


         sprintf_s( szCmd, sizeof( szCmd ), " IN ( SELECT INTVALUE FROM ZEIDONKEYLIST WHERE TASKID = %ld ) ",
                    SysGetTaskFromView( lpView ) );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, szCmd );
      }
      else
      {
      // zSHORT bNotEqual;
      // zSHORT nLoop;
         zBOOL  bNull;
         zBOOL  bUpper;
         zPCHAR pchQualOper = lpQualAttrib->szOper;

         // Oper is a simple compare.  Add statement to WHERE clause. Check for Qual Operator UPPER
         if ( zstrncmpi( lpQualAttrib->szOper, "UPPER", 5 ) == 0 )
         {
            zCHAR szUpper[ 32 ];

            bUpper = TRUE;
            fnSqlGetUpperString( szUpper, lpBoundList );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, szUpper );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "( " );
         }
         else
         {
            bUpper = FALSE;
         }

#if 0
         if ( bUpper )
            pchQualOper += 5; // after "UPPER"

         if ( pchQualOper[ 2 ] == 0 &&
              ((pchQualOper[ 0 ] == '<' && pchQualOper[ 1 ] == '>') ||
               (pchQualOper[ 0 ] == '!' && pchQualOper[ 1 ] == '=')) )
         {
            nLoop = 2;
            bNotEqual = TRUE;
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "( " );
         }
         else
         {
            nLoop = 1;
            bNotEqual = FALSE;
         }

         while ( nLoop > 0 )  // loop added to put out IS NULL when oper is <>
#endif
         {
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH,
                                   fnTableName( lpSql,
                                       lpQualAttrib->lpDataRecord->szRecordName,
                                       lpQualAttrib->lpViewEntity ) );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpQualAttrib->lpDataField->szFldName );
            if ( bUpper )
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " )" );

            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " " );

            // Check first to see if the compare value is null.
            bNull = FALSE;
            if ( lpQualAttrib->szValue )
            {
               if ( lpQualAttrib->szValue == 0 ) // this could never happen??? dks 2005.11.03
                  bNull = TRUE;
            }
            else
            if ( lpQualAttrib->lpSourceDataField &&
                 SqlAttributeIsNull( lpQualAttrib->lpSourceView,
                                     lpQualAttrib->lpSourceDataField,
                                     lpBoundList ) )
            {
               bNull = TRUE;
            }
#if 0
            if ( bNotEqual && nLoop == 2 )
            {
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " IS NULL OR " );

               nLoop--;
               continue;
            }
#else

            if ( bUpper )
               pchQualOper += 5; // after "UPPER"

#endif
            // Add operation.  Note that some SQL implementations use different
            // forms of the "Not Equal" operator.
            // In German language + EBCDIC environment there is a problem:
            //  sometimes the '!' is a german Umlaut 'UE', and so the '!'
            //  may be converted to another character.
            //  I consider every character preceding '=', which is not a ' ',
            //  '<' , '>' and not a '=', logically to be a to be a '!'
            // better use <> instead of !=
            if ( pchQualOper[ 1 ] == '=' &&
                 pchQualOper[ 0 ] != ' ' &&
                 pchQualOper[ 0 ] != '=' &&
                 pchQualOper[ 0 ] != '<' &&
                 pchQualOper[ 0 ] != '>' )
            {
               // This is SQL standard, which is understood by every DB.
               if ( bNull )
                  fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " IS NOT " );
               else
                  fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "<>" );
            }
            else
            {
               if ( bNull )
                  fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " IS " );
               else
                  fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, pchQualOper );
            }

            if ( bNull )
            {
               fnSqlGetNullString( szStr, lpQualAttrib->lpDataField, lpBoundList );
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, szStr );
            }
            else
            if ( lpQualAttrib->szValue )
            {
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " " );
               fnSqlConvertStringToSqlString( lpQualAttrib->lpDataField,
                                              lpQualAttrib->szValue, szStr,
                                              lpBoundList );

               // If the oper is LIKE then we don't want to pad with spaces for
               // fixed char types.
               if ( lpQualAttrib->lpDataField->cFldType == zTYPE_FIXEDCHAR &&
                    lpQualAttrib->bLike )
               {
                  zLONG lLth = zstrlen( szStr ) - 1;

                  while ( lLth >= 0 && szStr[ lLth ] == ' ' )
                     szStr[ lLth-- ] = 0;
               }

               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, szStr );
            }
            else
            if ( lpQualAttrib->lpSourceDataField )
            {
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " " );
               fnSqlGetAttributeValue( lpQualAttrib->lpSourceView,
                                       lpQualAttrib->lpSourceDataField, lpSql );
            }
#if 0
            if ( bNotEqual && nLoop == 1 )
            {
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " ) " );
            }

            nLoop--;
#endif
         }
      }

   } // for ( lpQualAttrib... )...

   fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " )" );

   return( 0 );

}  /* fnSqlAddQualToWhere */

// >>>SqlCheckOpenSQL
//
// Test whether an entity has a Open SQL qualification
// Return
//        1 ==> if YES
//        0 ==> if NOT
zSHORT OPERATION
SqlCheckOpenSQL( LPVIEWENTITY lpViewEntity, LPQUALENTITY lpFirstQualEntity )
{
   LPQUALENTITY lpQualEntity;

   // check, whether we find a qual entity with open SQL
   lpQualEntity = lpFirstQualEntity;
   while ( lpQualEntity )
   {
      if ( lpQualEntity->lpViewEntity == lpViewEntity &&
           lpQualEntity->pszOpenSQL && lpQualEntity->pszOpenSQL[ 0 ] )
      {
         return( 1 );
      }

      lpQualEntity = lpQualEntity->lpNextQualEntity;
   }

   return( 0 );
}

// Determines if lpViewEntity is a valid entity for joining with lpLoadEntity.
// Note that lpLoadEntity is not necessarily it's immediate parent.
zBOOL OPERATION
SqlEntityValidForJoin( LPVIEWENTITY lpViewEntity,
                       LPVIEWENTITY lpLoadEntity,
                       LPQUALENTITY lpFirstQualEntity )
{
   LPDATARECORD lpDataRecord;
   LPQUALENTITY lpQualEntity;
   LPVIEWENTITY lpParent;
   LPVIEWENTITY lpTemp;
   zBOOL        bQualOnChild = FALSE;

   // Root entities aren't joinable--it has no parent.
   if ( lpViewEntity->hParent == 0 )
      return( FALSE );

   // Only entities with physical information can be joined.
   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   if ( lpDataRecord == 0 )
      return( FALSE );

   // Only entities flagged with 'Y' can be joined.
   if ( lpDataRecord->cJoin != 'Y' )
      return( FALSE );

   // If we can load the entities attrs from the parent then it's faster than
   // doing a join so...
   if ( lpViewEntity->bAutoLoadFromParent )
      return( FALSE );

   // If entity is part of a duplicate relationship then we'll copy the EI
   // from a different part of the OI so don't load it.
   if ( lpViewEntity->bDupRelPath )
      return( FALSE );

   // Don't try joining recursive entities.
   if ( lpViewEntity->bRecursive )
      return( FALSE );

   // This probably isn't necessary but we'll check for it anyway ...
   // Don't join derived entities.
   if ( lpViewEntity->bDerivedPath )
      return( FALSE );

   // we cannot JOIN, if the parent has Open SQL
   lpParent = (LPVIEWENTITY) zGETPTR( lpViewEntity->hParent );
   if ( lpParent && SqlCheckOpenSQL( lpParent, lpFirstQualEntity ) )
      return( FALSE );

   // we cannot JOIN, if the parent has a DB Oper (MAX, SUM  ... )
   if ( lpParent && lpParent->bHasDB_Oper )
      return( FALSE );

   // Go through the qual entity list.  The entity is not valid for a join if:
   // o  Any of the QualEntities have the bOptionsNoJoins set.
   // o  The current entity has qualification.
   // o  The current is x:many related to the entity beeing loaded AND
   //    there is a qualification on the current entity or on one of
   //    its childs.
   for ( lpQualEntity = lpFirstQualEntity;
         lpQualEntity;
         lpQualEntity = lpQualEntity->lpNextQualEntity )
   {
      if ( lpQualEntity->bOptionsNoJoins )
         return( FALSE ); // User specified NOJOINS in the qual object.

      if ( lpQualEntity->lpViewEntity == lpViewEntity )
         return( FALSE ); // This entity has a RESTRICTING clause...can't join it.


      if ( lpQualEntity->lpViewEntity == lpLoadEntity )
      {
         LPQUALATTRIB lpQualAttrib;

         // We found the qualification for the entity we are loading.  Check
         // to see if any of the QualAttribs reference lpViewEntity or
         // a child of lpViewEntity.
         // If it does, then we can't join lpViewEntity if it is x:many
         // connected to lpLoadEntity. That is checked later.
         for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
               lpQualAttrib && bQualOnChild == FALSE;
               lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
         {
            // If the lpQualAttrib doesn't have a lpViewEntity then it's just
            // an oper (e.g. AND, OR) so we can ignore it.
            if ( lpQualAttrib->lpViewEntity == 0 )
               continue;

            for ( lpTemp = lpQualAttrib->lpViewEntity;
                  lpTemp;
                  lpTemp = zGETPTR( lpTemp->hParent ) )
            {
               // If lpTemp = lpLoadEntity, then lpQualAttrib has nothing
               //   to do with lpViewEntity.
               if ( lpTemp == lpLoadEntity )
                  break;

               // If lpTemp = lpViewEntity, then lpQualAttrib references
               //  lpViewEntity or a child of lpViewEntity.
               if ( lpTemp == lpViewEntity )
               {
                  bQualOnChild = TRUE;
                  break;
               }
            } // for lpTemp
         } // for lpQualAttrib
      } // if ( lpQualEntity->lpViewEntity == lpLoadEntity )
   }  // for lpQualEntity

   // If there is no qualification on child, we can join.
   // Otherwise:
   // If the entity to be checked (lpViewEntity) is connected to the
   //  entity beeing loaded (lpLoadEntity) via x:many relationships,
   //  then it is NOT VALID to do the join (1.Example).

   //  1.Example:
   //  A -0:m- B -1:1- C
   //  ACTIVATE ABC WHERE C.attr = 123
   //  lpLoadEntity=A, lpViewEntity=B
   //  We cannot join B, otherwise:
   //  SELECT A.attrs, B.attrs FROM
   //     ( A LEFT JOIN B ON ... ) LEFT JOIN C ON ... WHERE C.qual = ...
   //  If the result set for a particular A might be:
   //  A1 B1 C1
   //  Now, if there is a B2 below A1, this is NOT read, ERROR

   //  2.Example:
   //  A -1:1- B -0:m- C
   //  ACTIVATE ABC WHERE C.attr = 123
   //  We CAN join B ( not C ).
   //  SELECT A.attrs, B.attrs FROM
   //     ( A LEFT JOIN B ON ... ) LEFT JOIN C ON ... WHERE C.qual = ...
   //  If the result set for a particular A has multiple C's:
   //  A1 B1 C1
   //  A1 B1 Cm
   //  No problem, there is only one B, and it is read.

   if ( bQualOnChild == FALSE )
      return( TRUE );

   for ( lpTemp = lpViewEntity;
         lpTemp;
         lpTemp = zGETPTR( lpTemp->hParent ) )
   {
      // If reach lpLoadEntity, we're done
      if ( lpTemp == lpLoadEntity )
         break;

      // If lpTemp = lpViewEntity, then lpQualAttrib references
      //  lpViewEntity or a child of lpViewEntity.
      if ( lpTemp->uCardMax > 1 )
      {
         // Qual on Child AND x:many
         //  this is the JOIN KILLER.
         return( FALSE );
      }
   } // for lpTemp

   // If we get here then everything must check out so return TRUE.
   return( TRUE );
}

// Find the next child that has the Join flag set to 'Y'.
//
// This is used to determine which entities can be joined with lpParent when
// performing a select.
zSHORT OPERATION
SqlFindNextJoinableChild( LPVIEWENTITY   lpParent,
                          LPVIEWENTITY   *plpChildEntity,
                          LPVIEWENTITY   lpLoadEntity,
                          LPQUALENTITY   lpFirstQualEntity,
                          LPBOUNDLIST    lpBoundList )
{
   if ( *plpChildEntity == 0 )
      *plpChildEntity = lpParent;

   // DBHandler has said there might be more children so look for them.
   *plpChildEntity = zGETPTR( (*plpChildEntity)->hNextHier );
   while ( *plpChildEntity )
   {
      if ( *plpChildEntity == 0 ||
           (*plpChildEntity)->nLevel <= lpParent->nLevel )
      {
         return( FALSE );
      }

      // If the relationship between the child and parent is not joinable
      // skip it.
      if ( !SqlEntityValidForJoin( *plpChildEntity, lpLoadEntity,
                                   lpFirstQualEntity ) )
      {
         zSHORT nLevel;

         // This entity is not joinable so skip it.  We also want to skip all
         // it's children.
         if ( (*plpChildEntity)->hNextSibling )
         {
            (*plpChildEntity) = zGETPTR( (*plpChildEntity)->hNextSibling );
            continue;
         }

         // (*plpChildEntity) has no siblings.  Find the next entity by trolling
         // through the children hierachically.
         nLevel = (*plpChildEntity)->nLevel;
         while ( (*plpChildEntity) && (*plpChildEntity)->nLevel >= nLevel )
         {
            while ( (*plpChildEntity)->hNextSibling )
               (*plpChildEntity) = zGETPTR( (*plpChildEntity)->hNextSibling );

            (*plpChildEntity) = zGETPTR( (*plpChildEntity)->hNextHier );
         }

         continue;
      }

      // If we get here then we've found what we're looking for.
      return( TRUE );
   }

   // If we get here then *plpChildEntity must be 0 so return FALSE.
   return( FALSE );
}

// Add all child entities that are flagged with the JOIN=Y flag.
static zSHORT
fnSqlAddChildEntities( LPSQLSTATEMENT lpSql,
                       LPVIEWENTITY   lpViewEntity )
{
   zBOOL        bFirst;
   LPVIEWENTITY lpChildEntity = zGETPTR( lpViewEntity->hNextHier );
   LPDATARECORD lpDataRecord;
   zSHORT       nRC = 0;
   zPCHAR       pchEntityJoinTable = lpSql->lpBoundList->pchEntityJoinTable;

   // Loop through all the children of lpViewEntity.
   lpChildEntity = 0;
   bFirst = TRUE;
   while ( SqlFindNextJoinableChild( lpViewEntity, &lpChildEntity,
                                     lpSql->lpLoadViewEntity,
                                     lpSql->lpFirstQualEntity, lpSql->lpBoundList ) )
   {
      // Ok, we now have to add this entity to the select statement as a join.
      // First, add the table name.
      lpDataRecord = zGETPTR( lpChildEntity->hFirstDataRecord );
      fnAddTableToFrom( lpSql, 0, lpChildEntity, 0 );

      // Set a flag to tell the dbhandler that this entity will be loaded via
      // a join.
      if ( pchEntityJoinTable )
         pchEntityJoinTable[ lpChildEntity->nHierNbr ] |= zSELECT_JOINEDWITHPARENT;

      // Now add the column names.  Since we know that we've already added the
      // column names of the parent (lpViewEntity) then add a ',' to the table
      // name list.
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );
      fnSqlBuildColumnList( lpChildEntity, lpSql,
                            zFULLY_QUALIFIED | zNO_HIDDEN );

      nRC = 1; // indicate that Joins were generated
   }

   if ( bFirst == FALSE && lpViewEntity->hParent )
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );

   return( nRC );
}

/*
// Copies and formats user OpenSQL statement to pchSqlCmd.  Mostly we copy
// the string, but if we find a '@' then we must copy an attribute value
// from lpView.
//
// For example, if the OpenSQL statment is:
//    SELECT TEMP_DATA FROM TEMP_TABLE WHERE ZKEY = @ParentEnt.Attr
//
// then we will replace '@ParentEnt.Attr' with the value of the attribute
// Attr in the current entity ParentEnt.
*/
static zSHORT
fnSqlBuildOpenSQL( zVIEW         lpView,
                   LPVIEWOD      lpViewOD,
                   LPVIEWENTITY  lpViewEntity,
                   LPQUALENTITY  lpQualEntity,
                   zPCHAR        pchSqlCmd,
                   LPBOUNDLIST   lpBoundList )
{
   zPCHAR       pchSrc;    // Points to current char in OpenSQL statement.
   zPCHAR       pchTgt;    // Points to current char in pchSqlCmd.
   zCHAR        szTempStr[ 500 ];
   zCHAR        szEntityName[ 33 ];
   zCHAR        szAttributeName[ 33 ];
   LPDATAFIELD  lpDataField;
   LPVIEWENTITY lpSrchEntity;
   LPDATARECORD lpDataRecord;
   zSHORT       k;
   zSHORT       nRC;

   pchSrc = lpQualEntity->pszOpenSQL;
   pchTgt = pchSqlCmd;

   while ( *pchSrc )
   {
      // If the current char isn't a '@', then just copy the char and continue.
      if ( *pchSrc != '@' )
      {
         *pchTgt++ = *pchSrc++;
         continue;
      }

      // If the next char is also a '@', then copy '@@' as '@' and continue.
      if ( pchSrc[ 1 ] == '@' )
      {
         *pchTgt++ = *pchSrc++;     // Copy '@'.
         pchSrc++;                  // Skip second '@'.
         continue;
      }

      // We should now be pointing to a string with the form @EntName.Attr.
      // Get the Entity name.

      pchSrc++;   // Skip the '@'.
      k = 0;
      while ( *pchSrc && IS_VALID_ID_CHAR( *pchSrc ) )
         szEntityName[ k++ ] = *pchSrc++;

      szEntityName[ k ] = 0;

      if ( *pchSrc )
         pchSrc++;  // Skip the '.'.

      k = 0;
      while ( *pchSrc && IS_VALID_ID_CHAR( *pchSrc ) )
         szAttributeName[ k++ ] = *pchSrc++;

      szAttributeName[ k ] = 0;

      // Try getting the attribute value as a string.  If it failed, then
      // return with an error.  This call also verifies that the entity name
      // and attribute name are valid.
      nRC = GetStringFromAttribute( szTempStr, sizeof( szTempStr ), lpView, szEntityName, szAttributeName );
      if ( nRC < -2 )
         return( zCALL_ERROR );
      else
      if ( nRC == -2 )
      {
         sprintf_s( szTempStr, sizeof( szTempStr ), "Cursor is NULL for attribute '@%s.%s'",
                    szEntityName, szAttributeName );
         MessageSend( lpView, "KZH0202", "Qualification Object Error",
                      szTempStr, zMSGQ_SYSTEM_ERROR, 0 );
         return( zCALL_ERROR );
      }

      // Find the lpViewEntity that matches szEntityName.
      for ( lpSrchEntity = zGETPTR( lpViewOD->hFirstOD_Entity );
            zstrcmp( lpSrchEntity->szName, szEntityName ) != 0;
            lpSrchEntity = zGETPTR( lpSrchEntity->hNextHier ) )
      {
         ; // Nothing needs to be done here.
      }

      // Now find the data field for the attribute.
      lpDataRecord = zGETPTR( lpSrchEntity->hFirstDataRecord );
      for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
            lpDataField;
            lpDataField = zGETPTR( lpDataField->hNextDataField ) )
      {
         LPVIEWATTRIB lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

         if ( zstrcmp( szAttributeName, lpViewAttrib->szName ) == 0 )
            break;
      }

      // Convert the string value to a string the DB understands.
      fnSqlConvertStringToSqlString( lpDataField, szTempStr, pchTgt, lpBoundList );
      mAdvanceToNull( pchTgt );

   } // while ( *pchSrc )...

   *pchTgt = 0;

   return( 0 );

} // fnSqlBuildOpenSQL

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildSelect
//
// Builds a SELECT statment to locate all rows for lpViewEntity that
// are children of the current parent entity instance.
//
// Returns:  0 - everything ok
//           1 - everything ok, but there is at least 1 foreign key that
//               has a null value.
//           2 - a join was generated
//           4 - Order By Was generated
//
//           The return values are used bitwise
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildSelect( zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                LPQUALENTITY lpFirstQualEntity,
                zPCHAR       pchSqlCmd,
                LPBOUNDLIST  lpBoundList )
{
   LPDATARECORD       lpDataRecord;
   LPDATAFIELD        lpDataField;
   LPRELRECORD        lpRelRecord;
   LPQUALENTITY       lpQualEntity;
   LPSQLSTATEMENT     lpSql;
   SqlStatementRecord SqlStatement;
   zCHAR              sz[ 100 ];
   zSHORT             nOrderBy = 0;
   zSHORT             nNull;
   zSHORT             nRC;

   // Check to see if table is qualified by looking in the qualification
   // table for a matching View entity.
   lpQualEntity = SqlEntityIsQualified( lpFirstQualEntity, lpViewEntity );

   if ( lpQualEntity && lpQualEntity->pszOpenSQL )
   {
      // User has specified their own SQL, so copy SQL and return.
      return( fnSqlBuildOpenSQL( lpView, lpViewOD, lpViewEntity,
                                 lpQualEntity, pchSqlCmd, lpBoundList ) );
   }

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );
   lpDataField  = zGETPTR( lpDataRecord->hFirstDataField );

   // Exec callback to intialize SELECT string.
   if ( EXEC_CALLBACK( zSQLCALLBACK_INITSELECT, lpView,
                       lpDataField, (zPCHAR) lpQualEntity, sz ) )
   {
      // Init handled by call back.
   }
   else
      strcpy_s( sz, sizeof( sz ), "SELECT " );

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zSELECT_CMD, pchSqlCmd, lpView, lpBoundList );
   lpSql->lpLoadViewEntity  = lpViewEntity;
   lpSql->lpFirstQualEntity = lpFirstQualEntity;
   nNull = 0;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, sz );

   if ( lpQualEntity )
   {
      lpSql->bContainsSubselect   = lpQualEntity->bContainsSubselect;
      lpSql->bQualUsesChildEntity = lpQualEntity->bQualUsesChildEntity;
   }

   // We have to add an extra paren for each child that will be joined with the root.
   if ( CHECK_CALLBACK_FLAG( zSQLCALLBACK_USEJOIN ) &&
        (lpQualEntity == 0 || lpQualEntity->bOptionsNoJoins == FALSE) )
   {
      LPVIEWENTITY lpChildEntity;

      lpSql->bUseLeftJoin = TRUE;

      // Set aside some spaces for opening parens.  We'll be safe and set aside
      // two spaces for each child entity (and an extra one for good measure).
      lpSql->pchNextJoinParen = lpSql->pchFromEnd;
      for ( lpChildEntity = zGETPTR( lpViewEntity->hNextHier );
            lpChildEntity && lpChildEntity->nLevel > lpViewEntity->nLevel;
            lpChildEntity = zGETPTR( lpChildEntity->hNextHier ) )
      {
         LPDATARECORD lpChildDataRecord;
         LPRELRECORD  lpChildRelRecord;

         // If the child entity doesn't have a data record then it can't
         // be part of a SELECT so skip it.
         if ( lpChildEntity->hFirstDataRecord == 0 )
            continue;

         *lpSql->pchFromEnd++ = ' ';
         *lpSql->pchFromEnd++ = ' ';

         lpChildDataRecord = zGETPTR( lpChildEntity->hFirstDataRecord );
         lpChildRelRecord  = zGETPTR( lpChildDataRecord->hFirstRelRecord );

         // Take into account M-M tables.
         if ( lpChildRelRecord &&
              lpChildRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
         {
            *lpSql->pchFromEnd++ = ' ';
            *lpSql->pchFromEnd++ = ' ';
         }
      }

      *lpSql->pchFromEnd++ = ' ';
      *lpSql->pchFromEnd = 0;

   } // if ( CHECK_CALLBACK_FLAG( zSQLCALLBACK_USEJOIN ) )...

   // Add the main table name to the FROM list so that if any alias names are
   // generated they can be used in fnSqlBuildColumnList.
   fnAddTableToFrom( lpSql, 0, lpViewEntity, 0 );

   nRC = fnSqlBuildColumnList( lpViewEntity, lpSql, zFULLY_QUALIFIED | zNO_HIDDEN );

   // Check to see if we should use a join to select all child tables that
   // are flagged as joinable.
   if ( lpSql->bUseLeftJoin )
      fnSqlAddChildEntities( lpSql, lpViewEntity );

   // Some generated SQL needs an 'INTO' clause.  This is used for static SQL.
   if ( EXEC_CALLBACK( zSQLCALLBACK_USEINTO, lpView, 0, 0, 0 ) )
   {
      // DBHandler wants us to generate 'INTO' clause.
      fnSqlBuildIntoList( lpViewEntity, lpSql );
   }

   //===
   //=== Add parent foreign keys to WHERE clause.
   //===

   // If the table we are about to select has a parent table then we need to
   // add to the WHERE clause any foreign keys that logically connect the
   // table with it's parent.
   if ( lpViewEntity->hParent )
   {
      // If the table is qualified then we're going to add more stuff later
      // so let's add an opening paren.
      if ( lpQualEntity )
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, "( " );

      // Add foreign keys to WHERE.  We use zROOT_ENTITY to indicate that the
      // entity specified by lpViewEntity is the entity that we are loading
      // (i.e. it is the 'root' of the current load).
      nNull = fnSqlBuildForeignKeys( lpView, lpViewEntity, lpSql, zROOT_ENTITY );
   }

   // If entity is qualified, then add qualification.
   if ( lpQualEntity )
   {
      //===
      //===  Table is qualified.  Add qualification to WHERE clause.
      //===
      fnSqlAddQualToWhere( lpView, lpQualEntity,
                           lpQualEntity->lpFirstQualAttrib, lpSql );

      // If the parent is non-null then we added foreign keys above.  Add
      // the closing paren.
      if ( lpViewEntity->hParent )
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " )" );

   } // if ( lpQualEntity )...

   // If the entity has default ordering attributes, set up a 'ORDER BY'
   // clause.  We only do this if there is an activate limit.  If there is no
   // limit then we will load all the entities and sort them in memory.  If
   // there is an activate limit we need the 'ORDER BY' to ensure that we
   // load the *first* 'n' entities.
   // Special handling is  needed (TonBeller DBHandler), if left Joins
   //  with a x:m cardinality are used.
   // This is indicated by zSQLCALLBACK_CANONICALORDER

   if ( CHECK_CALLBACK_FLAG( zSQLCALLBACK_CANONICALORDER ) )
   {
      nOrderBy = fnCanonicalOrder( lpSql, lpViewEntity,
                                   lpFirstQualEntity, lpBoundList );
   }
   else
   if ( lpViewOD->bUseOrderBy ||
        CHECK_CALLBACK_FLAG( zSQLCALLBACK_FORCEORDERBY ) ||
        lpViewEntity->lActivateLimit )
   {
      // Changed the *OrderBy check* to old logic.
      nOrderBy = fnGenerateOrderBy( lpSql, lpViewEntity,
                                    lpFirstQualEntity, lpBoundList );
   }

   // Assemble the SQL statement from all the parts.
   fnSqlAssembleSqlStatement( lpSql );

   if ( nNull >= 0 )
   {
      if ( lpSql->nFromTableCount > 1 )
        nNull += 2;  // indicate that joins were generated

      if ( nOrderBy > 0 )
        nNull += 4;  // indicate that Order By was generated
   }

   return( nNull );
}  /* SqlBuildSelect */

/////////////////////////////////////////////////////////////////////////////
//
// SqlBuildSelectForEntityInstance
//
// Builds a SELECT statment to locate the source table for a particular
// entity.  DOES NOT set up foriegn keys.
//
// Will only select the columns that are part of pchColumnList.  If
// pchColumnList is 0 or *pchColumnList is 0 then all columns are
// selected.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildSelectForEntityInstance( zVIEW        lpView,
                                 LPVIEWOD     lpViewOD,
                                 LPVIEWENTITY lpViewEntity,
                                 zPCHAR       pchColumnList,
                                 zPCHAR       pchSqlCmd,
                                 LPBOUNDLIST  lpBoundList )
{
   zBOOL        bFirstKey;
   zBOOL        bFirstColumn;
   zBOOL        bPartialColumnList;
   zPCHAR       szToken;
   zLONG        lLth;
   LPVIEWATTRIB lpViewAttrib;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zSELECT_CMD, pchSqlCmd, lpView, lpBoundList );

   lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpDataField  = zGETPTR( lpDataRecord->hFirstDataField );
   bPartialColumnList = (pchColumnList && *pchColumnList);

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "SELECT " );
   fnAddTableToFrom( lpSql, 0, lpViewEntity, 0 );

   // Go through all the datafields.  If a key is found, add it to the WHERE
   // clause.  Also conditionally add the column name to the SELECT clause.
   bFirstColumn = TRUE;
   bFirstKey    = TRUE;
   for (; lpDataField; lpDataField = zGETPTR( lpDataField->hNextDataField ) )
   {
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

      // If attribute is a key then add it to the WHERE clause.
      if ( lpViewAttrib->bKey )
      {
         if ( bFirstKey == FALSE )
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );
         else
            bFirstKey = FALSE;

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );
         fnSqlGetAttributeValue( lpView, lpDataField, lpSql );
      }

      // If only a partial list is wanted then check to make sure that the
      // current data field should belong to the SELECT statement.
      if ( bPartialColumnList )
      {
         lLth = zstrlen( lpDataField->szFldName );
         szToken = pchColumnList;
         while ( szToken[ 0 ] &&
                 zstrncmpi( szToken,
                            lpDataField->szFldName, lLth ) != 0 )
         {
            // Set szToken to point to end of current token.
            while ( szToken[ 0 ] && szToken[ 0 ] != ',' && szToken[ 0 ] != ' ' )
               szToken++;

            // Edit out delimiters.
            while ( szToken[ 0 ] && (szToken[ 0 ] == ',' || szToken[ 0 ] == ' ') )
               szToken++;
         }

         // If szToken[ 0 ] is 0 then the column was not found and shouldn't be
         // included in the SELECT clause, so continue "for" loop.
         if ( szToken[ 0 ] == 0 )
            continue;
      }

      if ( bFirstColumn == FALSE )
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );
      else
         bFirstColumn = FALSE;

      // Add DataField to column list.
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );
   } // for (; lpDataField; ... )...

   fnSqlAssembleSqlStatement( lpSql );

   return( 0 );

}  /* SqlBuildSelectForEntityInstance */

/////////////////////////////////////////////////////////////////////////////
//
// SqlAutoLoadFromParent
//
// The information in the lpViewEntity can be retrieved from the parent
// entity by using the RelFields for the relationship between
// lpViewEntity and it's parent.
//
// RETURNS: 0 - entity was not loaded--normal processing needed.
//          1 - entity loaded OK--nothing else needed to load entity.
//          2 - a SELECT command has been created and needs to be executed.
//              Note that the SELECT command selects only the KEYS that
//              are part of the entity.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlAutoLoadFromParent( zVIEW        lpView,
                       LPVIEWOD     lpViewOD,
                       LPVIEWENTITY lpViewEntity,
                       LPQUALENTITY lpFirstQualEntity,
                       zPCHAR       pchSqlCmd,
                       LPBOUNDLIST  lpBoundList )
{
   zBOOL        bFirstTime;
   LPVIEWENTITY lpParent;
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   zULONG       uLth;
   zPVOID       lpValue;
   zCHAR        chType;
   zCHAR        szStr[ 500 ];
   LPVIEWATTRIB lpSrcViewAttrib;
   LPVIEWATTRIB lpRelViewAttrib;
   zBOOL        bEntityCreated;
   LPQUALENTITY lpQualEntity;
   LPQUALATTRIB lpQualAttrib;
   SqlStatementRecord SqlStatement;
   LPSQLSTATEMENT lpSql;
   zSHORT       nRC;

   // Check to see if the entity we are about to load is qualified.
   lpQualEntity = SqlEntityIsQualified (lpFirstQualEntity, lpViewEntity);
   if ( lpQualEntity )
   {
      // If there is a lpQualEntity then the entity we are loading is
      // qualified.  Loop through the attributes we are qualifing on; if any
      // are NOT keys or the attribute belongs to a different entity
      // then we can't perform the load here--return 0 to
      // indicate that regular SELECT processing must be performed.
      for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
            lpQualAttrib;
            lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
      {
         if ( lpQualAttrib->lpViewEntity != lpViewEntity ||
              (lpQualAttrib->lpViewAttrib &&
               lpQualAttrib->lpViewAttrib->bKey == FALSE) )
         {
            return( 0 );
         }
      }
   }

   lpParent       = zGETPTR( lpViewEntity->hParent );
   lpDataRecord   = zGETPTR( lpViewEntity->hFirstDataRecord );
   lpRelRecord    = zGETPTR( lpDataRecord->hFirstRelRecord );
   bEntityCreated = FALSE;
   bFirstTime     = TRUE;

   if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_ONE )
   {
      LPRELFIELD   lpRelField;

      // For many-to-one relationships, the keys for the child entity can be
      // retrieved from the parent entity.
      for ( lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
            lpRelField;
            lpRelField = zGETPTR( lpRelField->hNextRelField ) )
      {
         LPDATAFIELD lpSrcDataField = zGETPTR( lpRelField->hSrcDataField );
         LPDATAFIELD lpRelDataField = zGETPTR( lpRelField->hRelDataField );
         zSHORT      nNull;

         lpSrcViewAttrib = zGETPTR( lpSrcDataField->hViewAttrib );
         lpRelViewAttrib = zGETPTR( lpRelDataField->hViewAttrib );

         // Get attribute value from the parent entity.  We use GetString...
         // because it will tell us if the attribute is null.
         nNull = GetStringFromRecord( lpView,
                                      zGETPTR( lpRelViewAttrib->hViewEntity ),
                                      lpRelViewAttrib, szStr, 500 );

         // If entity is qualified, check to make sure that the attribute
         // value just retrieved matches the qualification.
         if ( lpQualEntity )
         {
            for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
                  lpQualAttrib;
                  lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
            {
               if ( lpQualAttrib->lpViewAttrib == lpSrcViewAttrib )
               {
                  GetValueFromRecord( lpView, zGETPTR( lpRelViewAttrib->hViewEntity ),
                                    lpRelViewAttrib, (zPVOID) &lpValue,
                                    &chType, &uLth );

                  // If attribute value does not compare with qualification
                  // value then return without creating entity.
                  if ( !fnCompare( lpView,
                                   (zPVOID) lpValue, lpQualAttrib->szOper,
                                   (zPVOID) lpQualAttrib->szValue, chType ) )
                  {
                     if ( bEntityCreated )
                     {
                        LPVIEWENTITY lpTempEntity =
                                    zGETPTR( lpSrcViewAttrib->hViewEntity );

                        DropEntity( lpView, lpTempEntity->szName, zREPOS_NONE );
                     }
                     return( 1 );
                  }
               }

            } // for ( lpQualAttrib... )...

         } // if ( lpQuallEntity )...

         // If a value was retrieved then store it in the entity.
         if ( nNull != -1 )
         {
            if ( bEntityCreated == FALSE )
            {
               bEntityCreated = TRUE;
               LoadEntity( lpView, lpViewEntity->szName, zPOS_AFTER, 0 );
            }

            StoreStringInRecord( lpView, zGETPTR( lpSrcViewAttrib->hViewEntity ),
                                 lpSrcViewAttrib, szStr );
         }
      } // for ( lpRelField;...)...

      return( 1 ); // Information for entity loaded.
   }
   else
   if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      LPRELFIELD   lpRelField;
      LPDATAFIELD  lpDataField;

      // For many-to-many relationships, the keys for the child entity can
      // be retrieved using only the correspondence table--the main table
      // for the child entity does not need to be used.
      lpSql = &SqlStatement;
      fnSqlInitSqlStatement( lpSql, zSELECT_CMD, pchSqlCmd,
                             lpView, lpBoundList );

      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, "SELECT " );

      // Build column list for key attributes.  We loop thru the datafields
      // looking for keys.  When we find one, we must then find the
      // lpRelField that matches the data field.  When it is found then
      // we add the rel fields FieldName to the select statement.  We must
      // do this round-about code because the rel fields must be listed in the
      // same order as the datafields.
      for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
            lpDataField;
            lpDataField = zGETPTR( lpDataField->hNextDataField ) )
      {
         LPVIEWATTRIB lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

         // We only want keys.
         if ( lpViewAttrib->bKey == FALSE )
            continue;

         // Find the rel field that matches lpDataField.
         lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
         while( lpRelField &&
                zGETPTR( lpRelField->hSrcDataField ) != lpDataField )
            lpRelField = zGETPTR( lpRelField->hNextRelField );

         if ( lpRelField == 0 )
         {
            IssueError( lpView, 16, 16, "(kzhsqlga) Internal error #2" );
            fnSqlFreeSqlStatement( lpSql );
            return( zCALL_ERROR );
         }

         if ( bFirstTime )
            bFirstTime = FALSE;
         else
            fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, ", " );

         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH, lpRelField->szFldName );
      } // for ( ; lpDataField; ... )...

      fnAddTableToFrom( lpSql, lpRelRecord->szRecordName, lpViewEntity, 0 );

      if ( lpQualEntity )
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " ( " );

      // Add the foreign keys.
      bFirstTime = TRUE;
      for ( lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
            lpRelField;
            lpRelField = zGETPTR( lpRelField->hNextRelField ) )
      {
         LPDATAFIELD lpSrcDataField = zGETPTR( lpRelField->hSrcDataField );

         lpSrcViewAttrib = zGETPTR( lpSrcDataField->hViewAttrib );

         // We only want the key values of the attributes that are part of
         // the parent entity.
         if ( zGETPTR( lpSrcViewAttrib->hViewEntity ) != lpParent )
            continue;

         if ( bFirstTime )
         {
            bFirstTime = FALSE;
            fnSqlIncrementComponentCount( lpSql );
         }
         else
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpRelField->szFldName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " = " );
         nRC = fnSqlGetAttributeValue( lpView, lpSrcDataField, lpSql );

         // If nRC == 1 then the attribute value is null, therefore there
         // can be no child entities.  Return with value that indicates that
         // nothing needs to be done.
         if ( nRC == 1 )
         {
            fnSqlFreeSqlStatement( lpSql );
            return( 1 );
         }
      } // for ( ; lpRelField; ... )...

      // Add Qualification to SELECT command.
      if ( lpQualEntity )
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " ) AND ( " );
         bFirstTime = TRUE;
         for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
               lpQualAttrib;
               lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
         {
            if ( bFirstTime )
            {
               bFirstTime = FALSE;
               fnSqlIncrementComponentCount( lpSql );
            }
            else
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " AND " );

            // Find the rel field that matches lpQualAttrib->lpDataField.
            // We need it to find the column name in the correspondence table
            // that matches the qualification attribute.
            lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
            while( lpRelField &&
                   zGETPTR( lpRelField->hSrcDataField ) !=
                                                 lpQualAttrib->lpDataField )
               lpRelField = zGETPTR( lpRelField->hNextRelField );

            if ( lpRelField == 0 )
            {
               IssueError( lpView, 16, 16, "(kzhsqlga) Internal error #3" );
               fnSqlFreeSqlStatement( lpSql );
               return( zCALL_ERROR );
            }

            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpRelField->szFldName );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " " );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, lpQualAttrib->szOper );

            if ( lpQualAttrib->szValue )
            {
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " " );
               fnSqlConvertStringToSqlString( lpQualAttrib->lpDataField,
                                              lpQualAttrib->szValue, szStr,
                                              lpBoundList );
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, szStr );
            }
            else
            if ( lpQualAttrib->lpSourceDataField )
            {
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " " );
               fnSqlGetAttributeValue( lpView,
                                       lpQualAttrib->lpSourceDataField, lpSql );
            }
         }

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH, " )" );
      } // if ( lpQualEntity )...

      fnSqlAssembleSqlStatement( lpSql );

      return( 2 );         // SELECT command created and must be executed.
   }
   else
   {
      // Nothing special can be done for one-to-many relationships, so
      // return 0.
      return( 0 );
   }

}  /* SqlAutoLoadFromParent */

/////////////////////////////////////////////////////////////////////////////
// fnGenerateOrderBy
//
// generate ORDER BY to SELECT statement,
//  depending on the LOD defined sequencing attributes.
//
// returns
// 1  == > ORDER BY was generated
// 0       otherwise
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnGenerateOrderBy( LPSQLSTATEMENT lpSql,
                   LPVIEWENTITY   lpViewEntity,
                   LPQUALENTITY   lpFirstQualEntity,
                   LPBOUNDLIST    lpBoundList )
{
   zCHAR        chCurrentAttrNbr;
   zCHAR        chMaxAttrNbr = 0;
   LPVIEWENTITY lpOrderViewEntity = lpViewEntity;
   LPVIEWENTITY lpChildEntity = 0;
   LPDATAFIELD  lpDataField;
   LPVIEWATTRIB lpViewAttrib;
   zSHORT       nOrderBy = 0;

   // We want to list the fields in the "ORDER BY" in 'most-significant'
   // order.  We therefore need to list the fields in the order defined by
   // lpViewAttrib->cSequencing.  In other words, the first field in the
   // "ORDER BY" is the field associated with the attribute with cSequencing
   // 1, the second field has cSequencing 2, etc. chCurrentAttrNbr indicates
   // the cSequencing number we are currently looking for to add to the list.
   //
   // It is possible that an attribute with cSequencing is a work attribute
   // which means it has no DataField.  This is valid and we must be able
   // to handle it.
   do
   {
      // If the entity has attribute ordering create the ORDER BY.
      if ( lpOrderViewEntity->bAttrOrder )
      {
         LPDATARECORD lpOrderDataRecord;

         chMaxAttrNbr = 0;

         lpOrderDataRecord = zGETPTR( lpOrderViewEntity->hFirstDataRecord );

         // First find the max cSequencing numbers for persistent attributes.
         for ( lpDataField = zGETPTR( lpOrderDataRecord->hFirstDataField );
               lpDataField;
               lpDataField = zGETPTR( lpDataField->hNextDataField ) )
         {
            lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
            if ( lpViewAttrib->cSequencing > chMaxAttrNbr )
               chMaxAttrNbr = lpViewAttrib->cSequencing;
         }

         if ( chMaxAttrNbr > 0 )
         {
            // If we haven't added the ORDER BY, do it now.  We'll
            // set the flag to TRUE a little later.
            if ( nOrderBy == 0 )
               fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, " ORDER BY " );

            for ( chCurrentAttrNbr = 1;
                  chCurrentAttrNbr <= chMaxAttrNbr;
                  chCurrentAttrNbr++ )
            {
               // Search through the data fields looking for the current
               // sequencing attribute.
               for ( lpDataField = zGETPTR( lpOrderDataRecord->hFirstDataField );
                     lpDataField;
                     lpDataField = zGETPTR( lpDataField->hNextDataField ) )
               {
                  lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
                  if ( lpViewAttrib->cSequencing == chCurrentAttrNbr )
                     break;
               }

               if ( lpDataField == 0 )
                  continue;

               // If the ORDER BY was just added, then we don't want to
               // add a comma.
               if ( nOrderBy == 0 )
                  nOrderBy = 1;
               else
                  fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, ", " );

               fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH,
                                       fnTableName( lpSql,
                                                    lpOrderDataRecord->szRecordName,
                                                    lpOrderViewEntity ) );
               fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, "." );
               fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, lpDataField->szFldName );

               // If the sequencing is descending, add DESC chars to statement
               if ( lpViewAttrib->bSequencingD )
                  fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, " DESC" );

            } // for chCurrentAttrNbr...

         } // if ( chMaxAttrNbr > 0 )...

      } // if ( lpOrderViewEntity->bAttrOrder )...

      // If we aren't using joins, then we're done (only one entity per
      // SELECT statement), other wise we need to add the order by
      // fields for joine entities.
      if ( lpSql->bUseLeftJoin == FALSE ) // bugfix HH 2002.02.14
         break;
      else
      {
         if ( !SqlFindNextJoinableChild( lpViewEntity, &lpChildEntity,
                                         lpViewEntity,
                                         lpFirstQualEntity, lpBoundList ) )
         {
            // No more joined entities.  Stop adding attributes.
            break;
         }

         // lpChildEntity is only used to find the next joined child
         // entity.  Copy it's value to lpOrderViewEntity.
         lpOrderViewEntity = lpChildEntity;
      }

   } while ( TRUE ); // We'll break inside the loop.

   return( nOrderBy );
}

/////////////////////////////////////////////////////////////////////////////
// fnCanonicalOrder
//
// generate ORDER BY to SELECT statement,
//  depending on the LOD entity key attributes.
// returns
// 1  == > ORDER BY (according to sequencing attributes) was generated
// 0       otherwise
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnCanonicalOrder( LPSQLSTATEMENT lpSql,
                  LPVIEWENTITY   lpViewEntity,
                  LPQUALENTITY   lpFirstQualEntity,
                  LPBOUNDLIST    lpBoundList )
{
   LPVIEWENTITY lpOrderViewEntity = lpViewEntity;
   LPDATAFIELD  lpDataField;
   LPVIEWATTRIB lpViewAttrib;
   zSHORT       nOrderBy = -1;
   zSHORT       bContinue = 0;
   zLONG        nKey;
   zLONG        nSequencing;
   zLONG        k, j;

   do
   {
      LPDATARECORD lpOrderDataRecord;
      LPDATAFIELD apDataField[ MAXLEN_KEY_ARRAY ];

      memset( apDataField, 0, sizeof( apDataField ) );
      nKey = 0;
      nSequencing = 0;

      lpOrderDataRecord = zGETPTR( lpOrderViewEntity->hFirstDataRecord );

      // find the key attributes, honoring the sequencing fild
      for ( lpDataField = zGETPTR( lpOrderDataRecord->hFirstDataField );
            lpDataField;
            lpDataField = zGETPTR( lpDataField->hNextDataField ) )
      {
         lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
         if ( lpViewAttrib->cSequencing ) {
            nSequencing++;
            // if a sequencing attr is NOT a key, then the ORDER BY to
            //  be generated won't be the right one
            if ( lpViewAttrib->bKey == FALSE )
               nOrderBy = 0;
         }

         if ( lpViewAttrib->bKey ) {
            if ( nKey >= MAXLEN_KEY_ARRAY )
                break; // should not occur
            apDataField[ nKey ] = lpDataField;
            nKey++;
         }
      }

      if ( nOrderBy != 0 && nSequencing > 0 && nKey >= nSequencing )
      {
         // Since sequencing fields are key fields,
         // we put them to the right order using bubble sort.
         for ( k = 0; k < nSequencing; k++ )
         {
            for ( j = k; j < nKey; j++ )
            {
               lpViewAttrib = zGETPTR( apDataField[ j ]->hViewAttrib );

               // lpViewAttrib->cSequencing is a 1-based index
               if ( lpViewAttrib->cSequencing == k + 1 )
               {
                  if ( j > k )
                  {
                     LPDATAFIELD pTemp = apDataField[ j ];
                     apDataField[ j ] = apDataField[ k ];
                     apDataField[ k ] = pTemp;
                  }

                  break;
               }
            }

            if ( j >= nKey )
            {
               // this is a problem, because we did' not find cSequencing=k
               // should not occur
               TraceLineS( "Invalid Sequencing defined for Entity ",
                           lpOrderViewEntity->szName );
               nSequencing = -1;
               break;
            }
         }

         if ( nOrderBy == -1 && nSequencing > 0 )
            nOrderBy = 1; // valid Order By according to LOD sequencing
      }

      if ( nSequencing < 0 || nKey < nSequencing )
      {
         // the ORDER BY is not the right one according to LOD ordering
         nOrderBy = 0;
      }

      for ( k = 0; k < nKey; k++ )
      {
         if ( bContinue == FALSE )
         {
            // start ORDER BY clause
            fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, " ORDER BY " );
            bContinue = TRUE;
         }
         else
         {
            // add a separator comma
            fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, ", " );
         }

         fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH,
                                 fnTableName( lpSql,
                                    lpOrderDataRecord->szRecordName,
                                    lpOrderViewEntity ) );
         fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, "." );
         fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, apDataField[ k ]->szFldName );

         lpViewAttrib = zGETPTR( apDataField[ k ]->hViewAttrib );
         if ( lpViewAttrib->bSequencingD )
            fnSqlAddStringToSuffix( lpSql, MAX_SQLCMD_LENGTH, " DESC" );

      }

      // add the key fields for the joined child entities.
      if ( lpSql->bUseLeftJoin == FALSE )
         break;

      if ( !SqlFindNextJoinableChild( lpViewEntity, &lpOrderViewEntity,
                                      lpViewEntity,
                                      lpFirstQualEntity, lpBoundList ) )
      {
         // No more joined entities.  Stop adding attributes.
         break;
      }

   } while ( TRUE ); // We'll break inside the loop.

   return( nOrderBy );
}

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlDisplayQualAttrib
//
// Used for debugging purposes.
//
/////////////////////////////////////////////////////////////////////////////
static void
fnSqlDisplayQualAttrib( LPQUALATTRIB lpQualAttrib,
                        zSHORT       nIndent )
{
   zCHAR  szStr[ 200 ];
   zSHORT n;

   if ( lpQualAttrib == 0 )
      return;

   nIndent += 5;
   for ( n = 0; n < nIndent; n++ )
      szStr[ n ] = ' ';
   szStr[ n ] = 0;

   while ( lpQualAttrib )
   {
      TraceLineS( "", "" );
      TraceLineS( szStr, "--- QualAttrib ---" );

      if ( lpQualAttrib->lpViewEntity )
      {
         strcat_s( szStr, sizeof( szStr ), "Entity name: " );
         TraceLineS( szStr, lpQualAttrib->lpViewEntity->szName );
         szStr[ n ] = 0;
      }

      if ( lpQualAttrib->lpViewAttrib )
      {
         strcat_s( szStr, sizeof( szStr ), "Attrib name: " );
         TraceLineS( szStr, lpQualAttrib->lpViewAttrib->szName );
         szStr[ n ] = 0;
      }

      strcat_s( szStr, sizeof( szStr ), "Oper      : " );
      TraceLineS( szStr, lpQualAttrib->szOper );
      szStr[ n ] = 0;

      if ( lpQualAttrib->szValue )
      {
         strcat_s( szStr, sizeof( szStr ), "Value     : " );
         TraceLineS( szStr, lpQualAttrib->szValue );
         szStr[ n ] = 0;
      }

      if ( lpQualAttrib->lpSourceViewEntity )
      {
         strcat_s( szStr, sizeof( szStr ), "Source Entity name: " );
         TraceLineS( szStr, lpQualAttrib->lpSourceViewEntity->szName );
         szStr[ n ] = 0;
      }

      if ( lpQualAttrib->lpSourceDataField )
      {
         LPVIEWATTRIB lpViewAttrib = zGETPTR( lpQualAttrib->lpSourceDataField->hViewAttrib );

         strcat_s( szStr, sizeof( szStr ), "Source Attrib name: " );
         TraceLineS( szStr, lpViewAttrib->szName );
         szStr[ n ] = 0;
      }

      if ( lpQualAttrib->lpSourceView )
      {
         TraceLineS( szStr, "SourceView is defined" );
         szStr[ n ] = 0;
      }

      fnSqlDisplayQualAttrib( lpQualAttrib->lpFirstSubQualAttrib, nIndent );
      lpQualAttrib = lpQualAttrib->lpNextQualAttrib;
   }

}  /* fnSqlDisplayQualAttrib */

/////////////////////////////////////////////////////////////////////////////
//
// SqlDisplayQualEntity
//
// Used for debugging purposes.
//
/////////////////////////////////////////////////////////////////////////////
void OPERATION
SqlDisplayQualEntity( LPQUALENTITY lpQualEntity,
                      zBOOL        bDisplayAll )
{
   TraceLineS( "************************************", "" );
   TraceLineS( "Display QualEntity structure", "" );
   for ( ;
         lpQualEntity;
         lpQualEntity = lpQualEntity->lpNextQualEntity )
   {
      TraceLineS( "", "" );
      TraceLineS( "--- QualEntity ---", "" );
      TraceLineS( "  Entity Name:", lpQualEntity->lpViewEntity->szName );

      if ( lpQualEntity->bQualUsesChildEntity )
         TraceLineS( "  bQualUsesChildEntity: TRUE", "" );
      else
         TraceLineS( "  bQualUsesChildEntity: FALSE", "" );

      if ( lpQualEntity->bContainsSubselect )
         TraceLineS( "  bContainsSubselect: TRUE", "" );
      else
         TraceLineS( "  bContainsSubselect: FALSE", "" );

      fnSqlDisplayQualAttrib( lpQualEntity->lpFirstQualAttrib, 0 );

      if ( bDisplayAll == FALSE )
         break;
   }

   TraceLineS( "************************************", "" );

}  /* SqlDisplayQualEntity */

/////////////////////////////////////////////////////////////////////////////
//
// SqlFreeQualAttrib
//
/////////////////////////////////////////////////////////////////////////////
static void
fnSqlFreeQualAttrib( LPQUALATTRIB lpFirstQualAttrib )
{
   LPQUALATTRIB lpQualAttrib;

   while ( lpFirstQualAttrib )
   {
      fnSqlFreeQualAttrib( lpFirstQualAttrib->lpFirstSubQualAttrib );

      lpQualAttrib = lpFirstQualAttrib;
      lpFirstQualAttrib = lpFirstQualAttrib->lpNextQualAttrib;

      if ( lpQualAttrib->hOperMem )
         SysFreeMemory( lpQualAttrib->hOperMem );

      if ( lpQualAttrib->hValueMem )
         SysFreeMemory( lpQualAttrib->hValueMem );

      if ( lpQualAttrib->lpKeyList )
         free( lpQualAttrib->lpKeyList );

      SysFreeMemory( lpQualAttrib->hMem );
   }
}  /* fnSqlFreeQualAttrib */

/////////////////////////////////////////////////////////////////////////////
//
// SqlFreeQualEntity
//
/////////////////////////////////////////////////////////////////////////////
void OPERATION
SqlFreeQualEntity( LPQUALENTITY *lpFirstQualEntity )
{
   LPQUALENTITY lpQualEntity;

   while ( *lpFirstQualEntity )
   {
      lpQualEntity = *lpFirstQualEntity;
      *lpFirstQualEntity = (*lpFirstQualEntity)->lpNextQualEntity;

      fnSqlFreeQualAttrib( lpQualEntity->lpFirstQualAttrib );
      if ( lpQualEntity->lpDataField )
         SysFreeMemory( lpQualEntity->hAttrListMem );

      SysFreeMemory( lpQualEntity->hMem );
   }
}  /* SqlFreeQualEntity */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlDisplayQualMsg
//
/////////////////////////////////////////////////////////////////////////////
static void
fnSqlDisplayQualMsg( zVIEW  lpView,
                     zVIEW  lpQualView,
                     zPCHAR pchMsg,
                     zSHORT nErrorOrWarning,
                     zPCHAR szQualEntity )
{
   LPVIEWOD lpViewOD;
   zBOOL    bWarning = (nErrorOrWarning == zWARNING);
   zCHAR    szText[ 1000 ];

   lpViewOD = (LPVIEWOD) MiGetViewEntityForView( lpView, 0 );
   sprintf_s( szText, sizeof( szText ), "%s loading qualification object while activating LOD "
              "%s: %s", bWarning ? "Warning" : "Error", lpViewOD->szName, pchMsg );

   MessageSend( lpView, "KZH0201", bWarning ? "Qualification Object Warning" : "Qualification Object Error",
                szText, zMSGQ_SYSTEM_ERROR, 0 );

   TraceLineS( szText, "" );

   if ( szQualEntity && szQualEntity[ 0 ] )
      DisplayEntityInstancePath( lpQualView, szQualEntity );

}  /* fnSqlDisplayQualMsg */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlVerifyDataBase
//
// Verify that all the QualAttribs point to the same database.
//
// RETURNS: TRUE  - All QualAttribs point to the same database.
//          FALSE - They don't.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlVerifyDataBase( LPQUALATTRIB lpFirstQualAttrib,
                     zPCHAR       pchDBName,
                     zBOOL        bDBNameIsDefault )
{
   LPVIEWENTITY lpViewEntity;
   LPQUALATTRIB lpQualAttrib;

   for ( lpQualAttrib = lpFirstQualAttrib;
         lpQualAttrib;
         lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
   {
      // If the current lpQualAttrib has a subQualAttrib then make sure
      // that all the sub-quals are OK.
      if ( lpQualAttrib->lpFirstSubQualAttrib &&
           !fnSqlVerifyDataBase( lpQualAttrib->lpFirstSubQualAttrib,
                                 pchDBName, bDBNameIsDefault ) )
      {
         return( FALSE );
      }

      lpViewEntity = lpQualAttrib->lpViewEntity;

      // If there is no lpViewEntity, don't bother doing check.
      if ( lpViewEntity == 0 )
         continue;

      // If there is no DBName for lpViewEntity then the database name for
      // the entity is the default.  In this case, check...
      if ( lpViewEntity->szDBName[ 0 ] == 0 )
      {
         // ...to make sure that we are using the default DBName.  If...
         if ( bDBNameIsDefault )
            continue;           // ...we are then continue, otherwise...
         else
            return( FALSE );    // ...return FALSE.
      }

      // If DBNames aren't the same then return FALSE.
      if ( zstrcmpi( pchDBName, lpViewEntity->szDBName ) != 0 )
         return( FALSE );

   } // for ( lpQualAttrib... )...

   // If we got this far then all the DBNames match up OK.
   return( TRUE );

} // fnSqlVerifyDataBase

static void
fnAllocOper( LPQUALATTRIB lpQualAttrib,
             zPCHAR       pchOper )
{

   zULONG uLth = zstrlen( pchOper ) + 1;

   if ( lpQualAttrib->hOperMem )
   {
      SysFreeMemory( lpQualAttrib->hOperMem );
      lpQualAttrib->hOperMem = 0;
      lpQualAttrib->szOper = 0;
   }

   if ( pchOper == 0 || *pchOper == 0 )
      return;

   lpQualAttrib->hOperMem = SysAllocMemory( (zPVOID) &lpQualAttrib->szOper,
                                            uLth, 0, zCOREMEM_ALLOC, 0 );
   if ( lpQualAttrib->szOper )
      strcpy_s( lpQualAttrib->szOper, sizeof( lpQualAttrib->szOper ), pchOper );
}

static zSHORT
fnCreateQualAttrib( zVIEW        lpView,
                    zVIEW        lpQualView,
                    LPQUALATTRIB *lpFirstQualAttrib,
                    LPQUALATTRIB *plpQualAttrib,
                    LPVIEWENTITY lpViewEntity,
                    LPDATAFIELD  lpDataField,
                    zPCHAR       pchOper,
                    zPCHAR       pchValue )
{
   zULONG       uLth;
   zLONG        hMem;
   LPQUALATTRIB lpQualAttrib;

   // Allocate space for new QualAttrib record.
   hMem = SysAllocMemory( (zPVOID) plpQualAttrib,
                          sizeof( QualAttribRecord ), 0,
                          zCOREMEM_ALLOC, 0 );
   if ( *plpQualAttrib == 0 )
   {
      fnSqlDisplayQualMsg( lpView, lpQualView,
                           "Cannot allocate memory.", zERROR, 0 );
      return( zCALL_ERROR );
   }

   lpQualAttrib = *plpQualAttrib;

   // Fill new QualAttrib with zeros.
   zmemset( (zPVOID) lpQualAttrib, 0, sizeof( QualAttribRecord ) );
   lpQualAttrib->hMem = hMem;

   // Add lpQualAttrib as first qual attrib in chain.
   lpQualAttrib->lpNextQualAttrib = *lpFirstQualAttrib;
   *lpFirstQualAttrib = lpQualAttrib;

   // Allocate space for the oper.
   fnAllocOper( lpQualAttrib, pchOper );

   // Allocate space for the value.
   if ( pchValue && *pchValue )
   {
      uLth = zstrlen( pchValue ) + 1;
      hMem = SysAllocMemory( (zPVOID) &lpQualAttrib->szValue,
                             uLth, 0, zCOREMEM_ALLOC, 0 );
      if ( lpQualAttrib->szValue == 0 )
      {
         fnSqlDisplayQualMsg( lpView, lpQualView,
                              "Cannot allocate memory.", zERROR, 0 );
         return( zCALL_ERROR );
      }

      lpQualAttrib->hValueMem = hMem;
      strcpy_s( lpQualAttrib->szValue, sizeof( lpQualAttrib->szValue ), pchValue );
   }

   if ( lpViewEntity )
   {
      lpQualAttrib->lpViewEntity = lpViewEntity;
      lpQualAttrib->lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
   }

   if ( lpDataField )
   {
      lpQualAttrib->lpDataField  = lpDataField;
      lpQualAttrib->lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
   }

   return( 0 );
}

/*
// This takes the following lpQualAttrib and pairs it with a qual that
// ensures that one of the keys is not null.
//
// So "EntityA.Attrib IS NULL" becomes
// "( EntityA.key IS NOT NULL AND EntityA.Attrib IS NULL )"
//
// Assumes: lpQualAttrib points to a "IS NULL" qualification.
*/
void
fnAddCheckForNullKey( zVIEW        lpView,
                      zVIEW        lpQualView,
                      LPQUALENTITY lpQualEntity,
                      LPQUALATTRIB lpQualAttrib,
                      LPQUALATTRIB *lpFirstQualAttrib )
{
   LPQUALATTRIB lpNewQualAttrib;
   LPVIEWATTRIB lpViewAttrib;
   LPDATAFIELD  lpDataField;

   // Copy the lpQualAttrib.  In a minute we're going to change lpQualAttrib
   // to be a closing paren.
   fnCreateQualAttrib( lpView, lpQualView, lpFirstQualAttrib,
                       &lpNewQualAttrib, lpQualAttrib->lpViewEntity,
                       lpQualAttrib->lpDataField, lpQualAttrib->szOper, 0 );

   // Find a key attrib for the entity. Note that any key attrib will
   // do (even if it's a multi-part key) because we only need to check if
   // the key is NULL.
   for ( lpDataField = zGETPTR( lpQualAttrib->lpDataRecord->hFirstDataField );
         lpDataField;
         lpDataField = zGETPTR( lpDataField->hNextDataField ) )
   {
      lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
      if ( lpViewAttrib->bKey )
         break;
   }

   // Change lpQualAttrib to a close paren.
   lpQualAttrib->lpViewEntity = 0;
   lpQualAttrib->lpDataRecord = 0;
   lpQualAttrib->lpViewAttrib = 0;
   lpQualAttrib->lpDataField  = 0;
   fnAllocOper( lpQualAttrib, ")" );

   // Add the AND
   fnCreateQualAttrib( lpView, lpQualView, lpFirstQualAttrib,
                       &lpNewQualAttrib, 0, 0, "AND", 0 );

   // Add the check for a null key.
   fnCreateQualAttrib( lpView, lpQualView, lpFirstQualAttrib,
                       &lpNewQualAttrib, zGETPTR( lpViewAttrib->hViewEntity ),
                       lpDataField, "IS NOT NULL", 0 );

   // Add the opening paren.
   fnCreateQualAttrib( lpView, lpQualView, lpFirstQualAttrib,
                       &lpNewQualAttrib, 0, 0, "(", 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlRetrieveQualAttrib
//
// Retrieve all the QualAttribs under the current EntitySpec entity in
// the qualification view.
//
// Note: We need to use lpFirstQualAttrib because this function will be
// called recursively if there are any SubQualAttribs.
//
// We don't have to worry about cleaning up allocated memory if there is
// an error because SqlRetrieveQualObject will clean it up for us.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlRetrieveQualAttrib( zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         zVIEW        lpQualView,
                         LPQUALENTITY lpQualEntity,
                         LPQUALATTRIB *lpFirstQualAttrib,
                         LPBOUNDLIST  lpBoundList )
 {
   zSHORT       nParenCount;
   zLONG        lLth;
   zLONG        hMem;
   zBOOL        bQualUsesChildEntity;
   zBOOL        bQualUsesOR_Oper = FALSE;
   zBOOL        bUseLeftJoin = FALSE;
   zPCHAR       pchAttributeName, pchAttEntityName;
   zPCHAR       pchOper, pchValue, pchSrcAttributeName;
   zPCHAR       pchSrcEntityName, pchPtr;
   zPCHAR       pchSrcViewName;
   zCHAR        szStr[ 300 ];
   zVIEW        lpSourceView;
   LPQUALATTRIB lpQualAttrib;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   LPVIEWENTITY lpSourceViewEntity;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPRELRECORD  lpRelRecord;
   zSHORT       nRC;

   // Go through each of the QualAttrib entities for EntitySpec and
   // copy attribute data to QualAttrib records.
   //
   // Note that we process the QualAttrib entities by going BACKWARDS,
   // starting with the last entity.  This is done so that adding the
   // lpQualAttribs to the chain is simple--we add each one to the beginning
   // of the chain.

   // =================================================================
   // ===
   // === Retrieve each of the attributes in QualAttrib and copy them to
   // === the record lpQualAttrib.
   // ===
   // =================================================================

   if ( CHECK_CALLBACK_FLAG( zSQLCALLBACK_USEJOIN ) )
      bUseLeftJoin = TRUE;

   nParenCount = 0;
   for ( nRC = SetCursorLastEntity( lpQualView, szlQualAttrib, "" );
         nRC == zCURSOR_SET;
         nRC = SetCursorPrevEntity( lpQualView, szlQualAttrib, "" ) )
   {
      bQualUsesChildEntity = FALSE;

      // Allocate space for new QualAttrib record.
      hMem = SysAllocMemory( (zPVOID) &lpQualAttrib,
                             sizeof( QualAttribRecord ), 0,
                             zCOREMEM_ALLOC, 0 );
      if ( lpQualAttrib == 0 )
      {
         fnSqlDisplayQualMsg( lpView, lpQualView,
                              "Cannot allocate memory",
                              zERROR, 0 );
         return( zCALL_ERROR );
      }

      // Fill new QualAttrib with zeros.
      zmemset( (zPVOID) lpQualAttrib, 0, sizeof( QualAttribRecord ) );
      lpQualAttrib->hMem = hMem;

      // Add lpQualAttrib as first qual attrib in chain.
      lpQualAttrib->lpNextQualAttrib = *lpFirstQualAttrib;
      *lpFirstQualAttrib = lpQualAttrib;

      // ===
      // ===  QualAttrib.Oper
      // ===

      // All Qual attribs must have an Oper.
      GetAddrForAttribute( &pchOper, lpQualView, szlQualAttrib, "Oper" );
      if ( pchOper[ 0 ] == 0 )
      {
         fnSqlDisplayQualMsg( lpView, lpQualView,
                              "QualAttrib does not have an operation defined. "
                              " Oper is a required attribute.",
                              zERROR, szlQualAttrib );
         return( zCALL_ERROR );
      }

      // Oper was found--go through and count parens.
      pchPtr = pchOper;
      while ( *pchPtr )
      {
         if ( *pchPtr == '(' )
            nParenCount++;
         else
         if ( *pchPtr == ')' )
            nParenCount--;

         pchPtr++;
      }

      fnAllocOper( lpQualAttrib, pchOper );
      zstrtrim( lpQualAttrib->szOper );

      if ( zstrcmpi( lpQualAttrib->szOper, "OR" ) == 0 )
      {
         bQualUsesOR_Oper = TRUE;
         lpQualEntity->bUniqueQualification = FALSE;
      }

      // ===
      // ===  QualAttrib.EntityName
      // ===

      GetAddrForAttribute( &pchAttEntityName, lpQualView, szlQualAttrib,
                           "EntityName" );
      if ( *pchAttEntityName )
      {
         LPVIEWENTITY lpSearchEntity;

         if ( zstrcmp( pchAttEntityName, "*root*" ) == 0 )
         {
            // User specified they want to use the root.  Just get the entity
            // name.
            lpViewEntity = (LPVIEWENTITY) zGETPTR( lpViewOD->hFirstOD_Entity );
         }
         else
            // Find the view entity that matches entity name.
            lpViewEntity = MiGetViewEntityForView( lpView, pchAttEntityName );

         // If no view entity was found to match entity name in qual entity
         // then issue an error and exit.
         if ( lpViewEntity == 0 )
         {
            sprintf_s( szStr, sizeof( szStr ), "Entity '%s' was specified in the Qualification "
                       "object but does not exist in LOD '%s'",
                       pchAttEntityName, lpViewOD->szName );
            fnSqlDisplayQualMsg( lpView, lpQualView, szStr, zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Make sure the entity is a valid one.
         if ( lpViewEntity->bDerived || lpViewEntity->bDerivedPath ||
              lpViewEntity->hFirstDataRecord == 0 )
         {
            zCHAR szMsg[ 500 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Entity '%s' in Object '%s' is a work or derived entity "
                       "and is not allowed for entity qualification during an "
                       "Activate", pchAttEntityName, lpViewOD->szName );
            fnSqlDisplayQualMsg( lpView, lpQualView, szMsg, zERROR, szlEntitySpec );
            return( zCALL_ERROR );
         }

         // This entity had better be a child of the entity we are qualifying.
         for ( lpSearchEntity = lpViewEntity;
               lpSearchEntity != lpQualEntity->lpViewEntity;
               lpSearchEntity = zGETPTR( lpSearchEntity->hParent ) )
         {
            if ( lpSearchEntity->hParent == 0 )
            {
               // Oops.  lpSearchEntity is the root entity of the LOD but
               // we haven't found a match with lpQualEntity->lpViewEntity. Illegal qual.
               zCHAR szMsg[ 500 ];

               sprintf_s( szMsg, sizeof( szMsg ), "You cannot qualify entity '%s' using entity "
                          "'%s' in Object Definition '%s'.  Only child entities "
                          "may be used for qualification.",
                          lpQualEntity->lpViewEntity->szName,
                          pchAttEntityName, lpViewOD->szName );
               fnSqlDisplayQualMsg( lpView, lpQualView, szMsg, zERROR, szlEntitySpec );
               return( zCALL_ERROR );
            }
         }

         // If the entity name for the attribute is not the same as the
         // entity name of the qual entity then the qualification uses child entities.
         if ( lpQualEntity->lpViewEntity &&
              lpQualEntity->lpViewEntity != lpViewEntity )
            bQualUsesChildEntity = TRUE;

         lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
         lpQualAttrib->lpViewEntity = lpViewEntity;
         lpQualAttrib->lpDataRecord = lpDataRecord;

      } // if ( *pchAttEntityName )...

      // ===
      // ===  QualAttrib.AttributeName
      // ===

      GetAddrForAttribute( &pchAttributeName, lpQualView, szlQualAttrib,
                           "AttributeName" );
      if ( *pchAttributeName )
      {
         // If QualAttrib entity has an AttributeName then QualAttrib
         // must also have a valid EntityName.
         if ( lpQualAttrib->lpViewEntity == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib entity has AttributeName but "
                                 "doesn't have EntityName.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Find the lpViewAttrib and DataField that matches AttribName.
         for ( lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
               lpDataField;
               lpDataField = zGETPTR( lpDataField->hNextDataField ) )
         {
            lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

            if ( zstrcmp( lpViewAttrib->szName, pchAttributeName ) == 0 )
               break;
         }

         // If no view entity was found to match entity name in qual
         // entity then issue an error and exit.
         if ( lpDataField == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "AttributeName in QualAttrib not found in "
                                 "in target object definition.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // In some cases, we might be qualifying an entity using an attribute
         // from a child entity.  If the child attribute is a key AND that key
         // is the source attribute for a many-to-one relationship then the
         // attribute's value is also stored in the parent entity (the entity
         // we are qualifying) as a foreign key.  It will be much quicker to
         // perform qualification on just the foreign key, so change the
         // qualification to reference the foreign key.
         lpRelRecord = zGETPTR( lpDataRecord->hFirstRelRecord );
         while ( lpViewAttrib->bKey &&
                 lpQualAttrib->lpViewEntity != lpQualEntity->lpViewEntity &&
                 lpRelRecord &&
                 lpRelRecord->cOwnerMember == zDBH_CHILD_IS_SOURCE )
         {
            LPRELFIELD   lpRelField;

            // Find the rel field for the qualifying attribute.
            for ( lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
                  lpRelField;
                  lpRelField = zGETPTR( lpRelField->hNextRelField ) )
            {
               LPDATAFIELD lpSrcDataField = zGETPTR( lpRelField->hSrcDataField );

               if ( lpSrcDataField->hViewAttrib == lpDataField->hViewAttrib )
                  break;
            }

            if ( lpRelField == 0 )
            {
               fnSqlDisplayQualMsg( lpView, lpQualView,
                                    "Internal error #10--can't find RelRecord",
                                    zERROR, szlQualAttrib );
               break;
            }

            // Change the column we are qualifying on.
            lpDataField  = zGETPTR( lpRelField->hRelDataField );
            lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
            lpViewEntity = zGETPTR( lpViewAttrib->hViewEntity );
            lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
            lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );

            lpQualAttrib->lpViewEntity = lpViewEntity;
            lpQualAttrib->lpDataRecord = lpDataRecord;

            // If the entity name for the attribute is not the same as the
            // entity name of the qual entity then the qualification uses
            // child entities.  If we're here then we know that the entity
            // for the QualAttrib is not the same as the entity for
            // QualEntity (which means that bQualUsesChildEntity = TRUE).
            // Check to see if we should turn it back off.
            if ( lpQualEntity->lpViewEntity == lpViewEntity )
               bQualUsesChildEntity = FALSE;

         } // while...

         lpQualAttrib->lpDataField  = lpDataField;
         lpQualAttrib->lpViewAttrib = lpViewAttrib;
         if ( lpViewAttrib->bKey == FALSE )
            lpQualEntity->bUniqueQualification = FALSE;

      } // if ( *pchAttributeName )...

      // ===
      // ===  QualAttrib.Value
      // ===

      GetAddrForAttribute( &pchValue, lpQualView, szlQualAttrib, "Value" );
      if ( *pchValue )
      {
         // If Value is defined then QualAttrib must also have AttribName.
         if ( lpQualAttrib->lpDataField == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib entity has Value but doesn't "
                                 "have AttributeName.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If Attribute is a date/time, convert pchValue to YYYYMMDDHHMMSSTTT.
         if ( lpQualAttrib->lpDataField->cFldType == zTYPE_DATETIME ||
              lpQualAttrib->lpDataField->cFldType == zTYPE_DATE     ||
              lpQualAttrib->lpDataField->cFldType == zTYPE_TIME )
         {
            if ( fnSqlUnformatDateTimeString( szStr, pchValue ) < 0 )
            {
               fnSqlDisplayQualMsg( lpView, lpQualView,
                                    "QualAttrib Value invalid for DateTime.",
                                    zERROR, szlQualAttrib );
               return( zCALL_ERROR );
            }

            // Set pchValue to point to unformatted string.
            pchValue = szStr;
         }

         lLth = zstrlen( pchValue ) + 1;
         hMem = SysAllocMemory( (zPVOID) &lpQualAttrib->szValue,
                                lLth, 0, zCOREMEM_ALLOC, 0 );
         if ( lpQualAttrib->szValue == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Cannot allocate memory.", zERROR, 0 );
            return( zCALL_ERROR );
         }

         lpQualAttrib->hValueMem = hMem;
         strcpy_s( lpQualAttrib->szValue, sizeof( lpQualAttrib->szValue ), pchValue );

      } // if ( *pchValue )...

      // ===
      // ===  Check for KeyList entities.
      // ===
      if ( CheckExistenceOfEntity( lpQualView, "KeyList" ) >= zCURSOR_SET )
      {
         zLONG  lKeyCount;
         zPLONG pl;
         zSHORT k;

         if ( *pchValue )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Cannot specify 'Value' and 'KeyList' on the same "
                                 "QualAttrib.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If KeyList is defined then QualAttrib must also have AttribName.
         if ( lpQualAttrib->lpDataField == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib entity has KeyList but doesn't "
                                 "have AttributeName.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         if ( lpQualAttrib->lpDataField->cFldType != zTYPE_INTEGER )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "The only datatypes supported with KeyList is "
                                 "Integer.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // We only support one keylist at the moment.
         if ( lpQualEntity->bContainsKeyList )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Only one QualAttrib is allowed to have a KeyList "
                                 "specified under each QualEntity.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Get the number of entities in the key list.
         lKeyCount = CountEntitiesForView( lpQualView, "KeyList" );

         // Alloc space to hold the keys.
         lpQualAttrib->lpKeyList = malloc( lKeyCount * sizeof( zLONG ) );
         lpQualAttrib->lKeyCount = lKeyCount;
         pl = (zPLONG) lpQualAttrib->lpKeyList;

         // Copy the keys to the table.
         k = 0;
         for ( nRC = SetCursorFirstEntity( lpQualView, "KeyList", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( lpQualView, "KeyList", 0 ) )
         {
            GetIntegerFromAttribute( &pl[ k++ ], lpQualView, "KeyList", "IntegerValue" );
         }

         lpQualEntity->bContainsKeyList = TRUE;
      }

      // ===
      // ===  QualAttrib.SourceViewName/SourceViewID
      // ===

      GetAddrForAttribute( &pchSrcViewName, lpQualView, szlQualAttrib, "SourceViewName" );
      if ( *pchSrcViewName )
      {
         GetViewByName( &lpSourceView, pchSrcViewName, lpQualView, zLEVEL_TASK );
         if ( lpSourceView )
         {
            lpQualAttrib->lpSourceView = lpSourceView;
            SetAttributeFromInteger( lpQualView, szlQualAttrib,
                                     "SourceViewID", (zLONG) lpSourceView );
         }
         else
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "SourceViewName not found at TASK level.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }
      }
      else
      {
         // SourceViewName was not specified--see if SourceViewID was.
         GetIntegerFromAttribute( (zPLONG) &lpSourceView, lpQualView,
                                  szlQualAttrib, "SourceViewID" );
         if ( lpSourceView )
            lpQualAttrib->lpSourceView = lpSourceView;
      }

      // ===
      // ===  QualAttrib.SourceEntityName
      // ===

      GetAddrForAttribute( &pchSrcEntityName, lpQualView,
                           szlQualAttrib, "SourceEntityName" );
      lpSourceViewEntity = 0;
      if ( *pchSrcEntityName )
      {
         // Find the lpViewEntity for source attrib entity.
         if ( lpQualAttrib->lpSourceView )
            lpSourceViewEntity = MiGetViewEntityForView( lpQualAttrib->lpSourceView,
                                                         pchSrcEntityName );
         else
            lpSourceViewEntity = MiGetViewEntityForView( lpView, pchSrcEntityName );

         // If no view entity was found to match the entity name
         // then issue an error and exit.
         if ( lpSourceViewEntity == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Attribute SourceEntityName not found in "
                                 "target object defintion.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If lpSourceView is defined then make sure that the
         // SourceViewEntity entity is defined.
         if ( lpQualAttrib->lpSourceView )
         {
            nRC = CheckExistenceOfEntity( lpQualAttrib->lpSourceView,
                                          lpSourceViewEntity->szName );
            if ( nRC != zCURSOR_SET )
            {
               fnSqlDisplayQualMsg( lpView, lpQualView,
                                    "Cursor is not set properly in the Source "
                                    "View specified in QualAttrib.",
                                    zERROR, szlQualAttrib );
               return( zCALL_ERROR );
            }
         }
         // lpSourceView is not defined--the Source View Entity
         // is the view we will activate later.
         // Make sure that lpSourceViewEntity comes before entity spec in
         // hierarchical order.
         else
         if ( lpSourceViewEntity->nHierNbr > lpQualEntity->lpViewEntity->nHierNbr )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "SourceEntityName does not have hierarchical "
                                 "precidence over EntityName in EntitySpec.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         lpQualAttrib->lpSourceViewEntity = lpSourceViewEntity;
      } // if ( *pchSrcEntityName )...

      // ===
      // ===  QualAttrib.SourceAttributeName
      // ===

      GetAddrForAttribute( &pchSrcAttributeName, lpQualView,
                           szlQualAttrib, "SourceAttributeName" );
      if ( *pchSrcAttributeName )
      {
         if ( lpQualAttrib->lpSourceViewEntity == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib has SourceAttributeName but "
                                 "doesn't have SourceEntityName.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Find the lpViewAttrib and DataField that matches
         // SourceAttributeName.
         lpDataRecord = zGETPTR( lpSourceViewEntity->hFirstDataRecord );
         for ( lpDataField  = zGETPTR( lpDataRecord->hFirstDataField );
               lpDataField;
               lpDataField = zGETPTR( lpDataField->hNextDataField ) )
         {
            lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

            if ( zstrcmp( lpViewAttrib->szName, pchSrcAttributeName ) == 0 )
               break;
         }

         // If no data field was found to match source attribute name in
         // target entity then issue an error and exit.
         if ( lpDataField == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "SourceAttributeName in QualAttrib not found "
                                 "in target object definition.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         lpQualAttrib->lpSourceDataField = lpDataField;
      } // if ( *pchSrcAttribteName )...

      // =================================================================
      // ===
      // ===  Validate Qualification attributes.
      // ===
      // =================================================================

      // ===
      // === Make sure that all attributes have been set that are needed for
      // === each command.
      // ===
      if ( zstrcmpi( lpQualAttrib->szOper, "EXISTS" ) == 0 ||
           zstrcmpi( lpQualAttrib->szOper, "NOT EXISTS" ) == 0 )
      {
         zBOOL bContainsSubselect;

         // Exists commands must have entity name specified.
         if ( lpQualAttrib->lpViewEntity == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib must specify EntityName for "
                                 "[NOT] EXISTS operation.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Check to make sure that the entity in QualEntity is a parent of
         // lpQualAttrib->lpViewEntity.
         for ( lpViewEntity = zGETPTR( lpQualAttrib->lpViewEntity->hParent );
               lpViewEntity && lpViewEntity != lpQualEntity->lpViewEntity;
               lpViewEntity = zGETPTR( lpViewEntity->hParent ) )
         {
            // Nothing needs to be done here.
         }

         if ( lpViewEntity == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib.EntityName must be a direct "
                                 "descendant of EntitySpec.EntityName for the "
                                 "[NOT] EXISTS operation.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If any other attributes are specified, warn user but continue.
         if ( lpQualAttrib->lpDataField         ||
              lpQualAttrib->szValue             ||
              lpQualAttrib->lpSourceViewEntity  ||
              lpQualAttrib->lpSourceDataField   ||
              lpQualAttrib->lpSourceView )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Extraneous attributes specified for [NOT] "
                                 "EXISTS operation.  Only Oper and EntityName "
                                 "are needed.  All others will be ignored.",
                                 zWARNING, szlQualAttrib );
         }

         // For now we think we will need a subselect.
         bContainsSubselect = TRUE;

         if ( zstrcmpi( lpQualAttrib->szOper, "EXISTS" ) == 0 )
            lpQualAttrib->bExists = TRUE;
         else
            lpQualAttrib->bDoesNotExist = TRUE;

         // Check to see if the relationship with the parent is x-to-1.  If
         // it is then we can replace the EXISTS clause with a qualification
         // that uses "parent.fk_key not NULL" instead.
         // Note that this doesn't work if there is sub-qualification.
         if ( lpQualAttrib->lpViewEntity->uCardMax == 1 &&
              CheckExistenceOfEntity( lpQualView,
                                      szlSubQualAttrib ) != zCURSOR_SET )
         {
            // Even though the max cardinality is 1 we still need to make
            // sure the FKs are stored in the parent.  The only way to do
            // this is to check the rel record.
            lpDataRecord = zGETPTR( lpQualAttrib->lpViewEntity->hFirstDataRecord );
            lpRelRecord  = zGETPTR( lpDataRecord->hFirstRelRecord );
            if ( lpRelRecord->cOwnerMember == zDBH_CHILD_IS_SOURCE )
            {
               zBOOL        bFirst;
               zBOOL        bExists = lpQualAttrib->bExists;
               LPRELFIELD   lpRelField;
               LPVIEWENTITY lpParent = zGETPTR( lpQualAttrib->lpViewEntity->hParent );

               // We can change the qualification from EXISTS to check FKs.

               // If the parent is the same as the entity we are qualifying
               // then we no longer use child qualification for this
               // QualAttrib.
               if ( lpQualEntity->lpViewEntity == lpParent )
                  bQualUsesChildEntity = FALSE;

               // Qualification no longer needs a subselect.
               bContainsSubselect = FALSE;

               // Keep track of the entity.
               lpViewEntity = lpQualAttrib->lpViewEntity;

               // Change current qual attrib to be an close paren.  We do this
               // because it's possible we have more than one key attribute
               // and we use a close paren because we are adding the
               // QualAttribs *backwards*.
               lpQualAttrib->lpViewEntity = 0;
               lpQualAttrib->bExists = lpQualAttrib->bDoesNotExist = 0;
               strcpy_s( lpQualAttrib->szOper, sizeof( lpQualAttrib->szOper ), ")" );

               // Now we have to create a new QualAttrib for each of the FKs.
               bFirst = TRUE;
               for ( lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
                     lpRelField;
                     lpRelField = zGETPTR( lpRelField->hNextRelField ) )
               {
                  if ( bFirst )
                     bFirst = FALSE;
                  else
                  {
                     // Allocate an 'AND' qual.
                     if ( fnCreateQualAttrib( lpView, lpQualView,
                                              lpFirstQualAttrib,
                                              &lpQualAttrib, 0, 0, "AND", 0 ) != 0 )
                     {
                        return( zCALL_ERROR );
                     }
                  }

                  lpDataField  = zGETPTR( lpRelField->hRelDataField );
                  lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );

                  if ( fnCreateQualAttrib( lpView, lpQualView,
                                           lpFirstQualAttrib,
                                           &lpQualAttrib, lpParent,
                                           lpDataField,
                                           bExists ? "IS NOT NULL" : "IS NULL", 0 ) != 0 )
                  {
                     return( zCALL_ERROR );
                  }

               } // for each lpRelField...

               // Add the opening paren.
               if ( fnCreateQualAttrib( lpView, lpQualView, lpFirstQualAttrib,
                                        &lpQualAttrib, 0, 0, "(", 0 ) != 0 )
               {
                  return( zCALL_ERROR );
               }

            } // if ( lpRelRecord->cOwnerMember == zDBH_CHILD_IS_SOURCE )...

         } // if ( lpQualAttrib->lpViewEntity->uCardMax == 1 )...

         if ( bContainsSubselect )
            lpQualEntity->bContainsSubselect = TRUE;

      } // if ( zstrcmpi( lpQualAttrib->szOper, "EXISTS" ) == 0 || ... )...
      else
      if ( zstrcmpi( lpQualAttrib->szOper, "IS" ) == 0 )
      {
         // IS command must have entity name specified.
         if ( lpQualAttrib->lpViewEntity == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib must have EntityName for IS "
                                 "operation.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // IS command must have source entity name specified.
         if ( lpQualAttrib->lpSourceViewEntity == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib must have SourceEntityName for "
                                 "IS operation.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // IS command must have source view specified.
         if ( lpQualAttrib->lpSourceView == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib must have SourceView for "
                                 "IS operation.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If any other attributes are specified, warn user but continue.
         if ( lpQualAttrib->lpDataField         ||
              lpQualAttrib->szValue             ||
              lpQualAttrib->lpSourceDataField )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Extraneous attributes specified for IS "
                                 "operation.  Only Oper, EntityName, "
                                 "SourceEntityName, and SourceView "
                                 "are needed.  All others will be ignored.",
                                 zWARNING, szlQualAttrib );
         }

         lpQualEntity->bContainsIS = TRUE;
         lpQualAttrib->bIs         = TRUE;

      } // if ( zstrcmpi( lpQualAttrib->szOper, "IS" ) == 0 )...
      else
      if ( zstrcmpi( lpQualAttrib->szOper, "LIKE" ) == 0 )
      {
         lpQualEntity->bUniqueQualification = FALSE;

         // IS command must have entity name specified.
         if ( lpQualAttrib->lpViewAttrib == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib must have Entity.AttributeName "
                                 "for LIKE operation.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Make sure attribute is a string.
         if ( lpQualAttrib->lpViewAttrib->cType != zTYPE_STRING )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Entity.AttributeName for QualAttrib must be "
                                 "a string for the LIKE operation.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         lpQualAttrib->bLike = TRUE;

      } // if ( zstrcmpi( lpQualAttrib->szOper, "LIKE" ) == 0 )...
      else
      {
         // Check to see if pchValue should be changed to "NULL".
         if ( lpQualAttrib->lpDataField &&
              lpQualAttrib->szValue == 0 &&
              lpQualAttrib->lpSourceDataField == 0 )
         {
            if ( zstrcmp( lpQualAttrib->szOper, "=" ) == 0 )
               fnAllocOper( lpQualAttrib, "IS NULL" );
            else
            if ( strstr( "<> !=", lpQualAttrib->szOper ) )
               fnAllocOper( lpQualAttrib, "IS NOT NULL" );
            else
            // QualAttrib has AttributeName specified but no Value or
            // SourceAttributeName.  If the operator is a comparison
            // operator, a Value is required.  Connection operators ( AND,
            // OR ) and precidence operators ( "(", ")" ) don't have values.
            if ( strstr( "<= =< >= =>", lpQualAttrib->szOper ) )
            {
               fnSqlDisplayQualMsg( lpView, lpQualView,
                                    "QualAttrib has AttributeName but does "
                                    "not have Value or SourceAttributeName, "
                                    "which are needed for the following comparison "
                                    "opererators: '<', '<=', '>', and'>='.",
                                    zERROR, szlQualAttrib );
               return( zCALL_ERROR );
            }

            // For databases that allow LEFT JOINs, qualification using a child
            // will be added via a join.  If the child qualification uses a
            // "IS NULL" on a non-key, the resulting join will create a match
            // for child entities that do not exist.  To fix this, we will also
            // make sure a key is non-null.
            if ( bUseLeftJoin &&
                 lpQualAttrib->lpViewEntity &&
                 lpQualAttrib->lpViewEntity != lpQualEntity->lpViewEntity &&
                 lpQualAttrib->lpViewEntity->uCardMin == 0 &&
                 lpQualAttrib->lpViewAttrib->bKey == FALSE )
            {
               if ( zstrstr( "IS NULL", lpQualAttrib->szOper ) )
               {
                  fnAddCheckForNullKey( lpView, lpQualView, lpQualEntity,
                                        lpQualAttrib, lpFirstQualAttrib );
               }
            }
         }

         if ( lpQualAttrib->lpDataField == 0 &&
              (lpQualAttrib->szValue || lpQualAttrib->lpSourceDataField) )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "QualAttrib has Value or SourceAttributeName"
                                 " but does not have AttributeName.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If we're still think we have a unique qualification check to make
         // sure that the oper is '='.
         if ( lpQualEntity->bUniqueQualification )
         {
            if ( zstrcmp( lpQualAttrib->szOper, "=" ) != 0 )
               lpQualEntity->bUniqueQualification = FALSE;
         }
      }

      // =================================================================
      // ===
      // === Check to see if there are sub-qualifications for the current
      // === QualAttrib.  Retrieve the sub-qualifications if they are there.
      // ===
      // =================================================================
      if ( CheckExistenceOfEntity( lpQualView,
                                   szlSubQualAttrib ) == zCURSOR_SET )
      {
         zVIEW lpSubQualView;

         CreateViewFromViewForTask( &lpSubQualView, lpQualView, 0 );
         nRC = SetViewToSubobject( lpSubQualView, szlSubQualAttrib );
         if ( nRC < 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Internal error setting view to Subobject.",
                                 zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Call fnSqlRetrieveQualAttrib recursivly to load sub-qual.
         nRC = fnSqlRetrieveQualAttrib( lpView, lpViewOD,
                                        lpSubQualView, lpQualEntity,
                                        &lpQualAttrib->lpFirstSubQualAttrib,
                                        lpBoundList );
         DropView( lpSubQualView ); // Don't need view anymore.
         if ( nRC < 0 )
            return( nRC );
      }

      // Turn on bQualUsesChildEntity.
      lpQualEntity->bQualUsesChildEntity =
                  lpQualEntity->bQualUsesChildEntity || bQualUsesChildEntity;

   } // for each lpQualView.QualAttrib

   // =================================================================
   // ===
   // === End of attribute loop.
   // ===
   // =================================================================

   // Go through all the qual attribs and check to see if any are seperated
   // by an "OR".  Say we have the following object:
   //
   //      A
   //      /\
   //     B  C
   //
   // (B and C have 0-to-many relationships with A).
   // and the user qualified A with "B.col = 1 OR C.col = 2".  This causes
   // the following to be generated:
   //
   //    SELECT A-columns
   //    FROM A, B, C
   //    WHERE ( (A.fk_b = B.key AND a.fk_c = C.key) AND
   //            (b.key = 1 OR C.key = 2) )
   //
   // This is a problem because if either B or C doesn't exist then the the
   // select doesn't find anything because the join fails to find any children
   // that match the foreign keys.
   //
   // NOTE: This is only a problem with databases that don't support LEFT
   // JOINs.
   //
   // To solve this we need to go through each of the qual attribs and see if:
   //    o  They involve qualification on multiple children.
   //    o  There is an "OR" operation.
   // Then convert each of the child quals to use an EXIST clause.
   if ( bUseLeftJoin == FALSE && bQualUsesChildEntity && bQualUsesOR_Oper )
   {
      zBOOL        bConvertNeeded = FALSE;
      LPQUALATTRIB lpPrevQualAttrib = 0;

      // We still don't know if there are multiple children.  Check for it.
      for ( lpQualAttrib = *lpFirstQualAttrib;
            lpQualAttrib;
            lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
      {
         // No entity for this qual attrib?  Skip it.
         if ( lpQualAttrib->lpViewEntity == 0 )
            continue;

         // Entity for qual attrib is entity we are loading (i.e. the root)
         // then skip it (we only care about child entities).
         if ( lpQualAttrib->lpViewEntity == lpQualEntity->lpViewEntity )
            continue;

         // If the operation already is an EXISTS operation don't bother.
         if ( lpQualAttrib->bExists || lpQualAttrib->bDoesNotExist )
            continue;

         // If lpPrevQualEntity is 0 then this is the first child we have
         // found so set it and go on to the next qual attrib.
         if ( lpPrevQualAttrib == 0 )
         {
            lpPrevQualAttrib = lpQualAttrib;
            continue;
         }

         // If we have multiple child entities involved in the qualification
         // then we need to convert the QualAttrib.
         if ( lpPrevQualAttrib->lpViewEntity != lpQualAttrib->lpViewEntity )
         {
            bConvertNeeded = TRUE;
            break;
         }

      } // for each lpQualAttrib...

      // Now that we know if we need to convert child qualification to use
      // EXISTS do so now.
      if ( bConvertNeeded )
      {
         for ( lpPrevQualAttrib = 0, lpQualAttrib = *lpFirstQualAttrib;
               lpQualAttrib;
               lpPrevQualAttrib = lpQualAttrib,
                 lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
         {
            LPQUALATTRIB lpNewQualAttrib = 0;

            // No entity for this qual attrib?  Skip it.
            if ( lpQualAttrib->lpViewEntity == 0 )
               continue;

            // Entity for qual attrib is entity we are loading (i.e. the root)
            // then skip it (we only care about child entities).
            if ( lpQualAttrib->lpViewEntity == lpQualEntity->lpViewEntity )
               continue;

            // If the operation already is and EXISTS operation don't bother.
            if ( lpQualAttrib->bExists || lpQualAttrib->bDoesNotExist )
               continue;

            // We don't care about entities that have a min cardinality of 1
            // because there will always be an occurence of the child entity.
            if ( lpQualAttrib->lpViewEntity->uCardMin == 1 )
               continue;

            // =========================================================
            // If we get here then lpQualAttrib fits all the criteria and
            // needs to be converted to use an EXISTS clause.
            // =========================================================

            // First create the new EXISTS qual attrib in front of the
            // current qual attrib.
            if ( fnCreateQualAttrib( lpView, lpQualView,
                                     lpPrevQualAttrib ?
                                       &lpPrevQualAttrib->lpNextQualAttrib :
                                       lpFirstQualAttrib,
                                     &lpNewQualAttrib,
                                     lpQualAttrib->lpViewEntity,
                                     0, "EXISTS", 0 ) != 0 )
               return( zCALL_ERROR );

            // Now move qual attrib so that it is a sub-qual attrib of the
            // exists qual attrib.
            lpNewQualAttrib->lpFirstSubQualAttrib = lpQualAttrib;
            lpNewQualAttrib->lpNextQualAttrib     = lpQualAttrib->lpNextQualAttrib;
            lpQualAttrib->lpNextQualAttrib        = 0;

            // Qualification now uses a subselect so set flag accordingly.
            lpQualEntity->bContainsSubselect = TRUE;

            // Last thing...reset lpQualAttrib to lpNewQualAttrib so that the
            // next iteration of the 'for' loop works correctly.
            lpQualAttrib = lpNewQualAttrib;

         } // For each lpQualAttrib...

         // We have to go through the qual attribs again and reset the
         // bContainsChildQual flag.
         lpQualEntity->bQualUsesChildEntity = FALSE;
         for ( lpQualAttrib = *lpFirstQualAttrib;
               lpQualAttrib;
               lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
         {
            // If there is no entity for the qual attrib skip it.
            if ( lpQualAttrib->lpViewEntity == 0 )
               continue;

            // If entity for qual attrib is same as entity for qual entity
            // then qual attrib is not a child so skip it.
            if ( lpQualAttrib->lpViewEntity == lpQualEntity->lpViewEntity )
               continue;

            // If there is no data field for the qual attrib then the OPER
            // must be EXISTS or similair so skip it.
            if ( lpQualAttrib->lpDataField == 0 )
               continue;

            lpQualEntity->bQualUsesChildEntity = TRUE;
            break;

         } // For each lpQualAttrib...

      } // if ( bConvertNeeded )...

   } // if ( bQualUsesChildEntity && bQualUsesOR_Oper )...

   // Now check to make sure that all the QualAttribs use the same database
   // as the EntitySpec.
   if ( *lpQualEntity->lpViewEntity->szDBName )
   {
      // fnSqlVerifyDataBase with the DBName in lpViewEntity.
      nRC = fnSqlVerifyDataBase( lpQualEntity->lpFirstQualAttrib,
                                 lpQualEntity->lpViewEntity->szDBName,
                                 FALSE );
   }
   else
      // fnSqlVerifyDataBase with the default DBName.
      nRC = fnSqlVerifyDataBase( lpQualEntity->lpFirstQualAttrib,
                                 lpViewOD->szDfltDBName, TRUE );

   if ( nRC == 0 )
   {
      fnSqlDisplayQualMsg( lpView, lpQualView,
                           "The QualAttrib used to qualify EntitySpec "
                           "causes two tables from different databases "
                           "to be joined.  This is invalid.",
                           zERROR, szlQualAttrib );
      return( zCALL_ERROR );
   }

   // After going through all QualAttribs, the ParenCount must be zero. If
   // it isn't then there are too many left parens (if nParenCount > 0 )
   // or too may right parens (if nParenCount < 0).
   if ( nParenCount != 0 )
   {
      fnSqlDisplayQualMsg( lpView, lpQualView,
                           "Parens for all QualAttrib.Oper under "
                           "EntitySpec do not match.", zERROR, 0 );
      DisplayObjectInstance( lpQualView, szlEntitySpec, 0 );
      return( zCALL_ERROR );
   }

   return( 0 );

}  /* fnSqlRetrieveQualAttrib */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlRetrieveQualObject
//
// Retrieves the information from the qualification view and creates the
// QualEntity chain.
//
// NOTE that if we return zCALL_ERROR, then SqlRetrieveQualObject will free
// up any allocated space for us.
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnSqlRetrieveQualObject( zVIEW        lpView,
                         zVIEW        lpQualView,
                         LPVIEWOD     lpViewOD,
                         LPQUALENTITY *lpFirstQualEntity,
                         LPBOUNDLIST  lpBoundList )
{
   LPQUALENTITY lpQualEntity;
   LPVIEWENTITY lpViewEntity;
   LPDATARECORD lpDataRecord;
   zPCHAR       pchOptions;
   zPCHAR       pchEntityName;
   zLONG        hMem;
   zSHORT       nRC;

   *lpFirstQualEntity = 0;

   // Copy each of the EntitySpec entities to the qualification chain.
   for ( nRC = SetCursorFirstEntity( lpQualView, szlEntitySpec, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( lpQualView, szlEntitySpec, 0 ) )
   {
      hMem = SysAllocMemory( (zPVOID) &lpQualEntity,
                             sizeof( QualEntityRecord ), 0, zCOREMEM_ALLOC, 0 );
      if ( lpQualEntity == 0 )
      {
         fnSqlDisplayQualMsg( lpView, lpQualView,
                              "Cannot allocate memory", zERROR, 0 );
         return( zCALL_ERROR );
      }

      // Add lpQualEntity to beginning of chain.
      zmemset( (zPVOID) lpQualEntity, 0, sizeof( QualEntityRecord ) );
      lpQualEntity->hMem = hMem;
      lpQualEntity->lpNextQualEntity = *lpFirstQualEntity;
      *lpFirstQualEntity = lpQualEntity;

      // Find the view entity that matches the qual entity.
      GetAddrForAttribute( &pchEntityName, lpQualView, szlEntitySpec,
                           "EntityName" );
      if ( zstrcmp( pchEntityName, "*root*" ) == 0 )
      {
         LPVIEWOD lpViewOD;

         // User specified they want to use the root.  Just get the entity
         // name.
         lpViewOD = (LPVIEWOD) MiGetViewEntityForView( lpView, 0 );
         lpViewEntity = (LPVIEWENTITY) zGETPTR( lpViewOD->hFirstOD_Entity );
      }
      else
         lpViewEntity = MiGetViewEntityForView( lpView, pchEntityName );

      // If no view entity was found to match entity name in qual entity
      // then issue an error and exit.
      if ( lpViewEntity == 0 )
      {
         zCHAR szMsg[ 500 ];

         sprintf_s( szMsg, sizeof( szMsg ), "Entity '%s' does not exist in Object '%s'",
                   pchEntityName, lpViewOD->szName );
         fnSqlDisplayQualMsg( lpView, lpQualView, szMsg,
                              zERROR, szlEntitySpec );
         return( zCALL_ERROR );
      }

      if ( lpViewEntity->bDerived || lpViewEntity->bDerivedPath ||
           lpViewEntity->hFirstDataRecord == 0 )
      {
         zCHAR szMsg[ 500 ];

         sprintf_s( szMsg, sizeof( szMsg ), "Entity '%s' in Object '%s' is a work or derived entity "
                   "and is not allowed for entity qualification during an "
                   "Activate", pchEntityName, lpViewOD->szName );
         fnSqlDisplayQualMsg( lpView, lpQualView, szMsg,
                              zERROR, szlEntitySpec );
         return( zCALL_ERROR );
      }

      GetAddrForAttribute( &lpQualEntity->pszOpenSQL,
                           lpQualView, szlEntitySpec, "OpenSQL" );
      if ( lpQualEntity->pszOpenSQL && *lpQualEntity->pszOpenSQL == 0 )
         lpQualEntity->pszOpenSQL = 0;

      // Check to see if there is OpenSQL statement.
      if ( lpQualEntity->pszOpenSQL )
      {
         zPCHAR       pchAttrList;
         zPCHAR       pch;
         zSHORT       nAttrListCount;
         LPDATAFIELD  lpFirstDataField;

         lpQualEntity->bUniqueQualification = FALSE;

         // We have an OpenSQL select statement.  Now make sure that the
         // attribute list is specified.
         GetAddrForAttribute( &pchAttrList, lpQualView,
                              szlEntitySpec, "OpenSQL_AttributeList" );
         if ( pchAttrList == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "OpenSQL is specified but not "
                                 "OpenSQL_AttributeList.",
                                 zERROR, szlEntitySpec );
            return( zCALL_ERROR );
         }

         // Make sure the entity has data records.
         lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
         if ( lpDataRecord == 0 ||
              lpDataRecord->hFirstDataField == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "OpenSQL is specified but entity does not "
                                 "have DB information for attributes.",
                                 zERROR, szlEntitySpec );
            return( zCALL_ERROR );
         }

         lpFirstDataField = zGETPTR( lpDataRecord->hFirstDataField );

         // Retrieve the attribute list and convert the list to a table of
         // data records.
         hMem = SysAllocMemory( (zPVOID) &lpQualEntity->lpDataField,
                                sizeof( LPDATAFIELD ) * zMAX_OPENSQL_ATTRS,
                                0, zCOREMEM_ALLOC, 0 );
         if ( lpQualEntity->lpDataField == 0 )
         {
            fnSqlDisplayQualMsg( lpView, lpQualView,
                                 "Cannot allocate memory",
                                 zERROR, 0 );
            return( zCALL_ERROR );
         }

         lpQualEntity->hAttrListMem = hMem;

         zmemset( (zPVOID) lpQualEntity->lpDataField, 0,
                  sizeof( LPDATAFIELD ) * zMAX_OPENSQL_ATTRS );

         // Parse through the attribute list.  For each attribute, find the
         // cooresponding DataRecord and add it to the data record list.
         nAttrListCount = 0;
         pch = pchAttrList;
         while ( *pch )
         {
            zCHAR        szAttrName[ 33 ];
            zSHORT       nAttrLth;
            LPDATAFIELD  lpDataField;

            // Skip non-alphanumeric chars.
            while ( *pch && !IS_VALID_ID_CHAR( *pch ) )
               pch++;

            if ( pch[ 0 ] == 0 )
               break;

            // Copy current attr name in list to variable.
            for ( nAttrLth = 0; IS_VALID_ID_CHAR( *pch ); )
               szAttrName[ nAttrLth++ ] = *pch++;

            szAttrName[ nAttrLth ] = 0;

            // Now look for the attribute that matches szAttrName.
            for ( lpDataField = lpFirstDataField;
                  lpDataField;
                  lpDataField = zGETPTR( lpDataField->hNextDataField ) )
            {
               LPVIEWATTRIB lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
               if ( zstrcmp( szAttrName, lpViewAttrib->szName ) == 0 )
               {
                  // We found the attribute we were looking for.  Copy the
                  // data field pointer to the attr list table and break.
                  lpQualEntity->lpDataField[ nAttrListCount++ ] = lpDataField;
                  break;
               }
            }

            // If lpDataField is 0, then we didn't find szAttrName in the
            // attribute list.
            if ( lpDataField == 0 )
            {
               zCHAR szMsg[ 300 ];

               sprintf_s( szMsg, sizeof( szMsg ), "Attribute name '%s' specified in "
                         "OpenSQL_AttributeList does not exist for entity '%s'",
                         szAttrName, lpViewEntity->szName );
               fnSqlDisplayQualMsg( lpView, lpQualView,
                                    szMsg, zERROR, szlEntitySpec );
               return( zCALL_ERROR );
            }

         } // for ( nAttrListCount... )

      } // if ( lpQualEntity->pszOpenSQL )...

      // Check for the different options.
      GetAddrForAttribute( &pchOptions, lpQualView, szlEntitySpec, "Options" );
      if ( *pchOptions )
      {
         if ( zstrstr( pchOptions, "NOJOIN" ) != 0 )
            lpQualEntity->bOptionsNoJoins = TRUE;
      }

      lpQualEntity->lpViewEntity         = lpViewEntity;
      lpQualEntity->bUniqueQualification = TRUE;
      lpQualEntity->bContainsSubselect   = FALSE;

      // Retrieve all QualAttribs under the current EntitySpec.
      nRC = fnSqlRetrieveQualAttrib( lpView, lpViewOD,
                                     lpQualView, lpQualEntity,
                                     &lpQualEntity->lpFirstQualAttrib,
                                     lpBoundList );
      if ( nRC < 0 )
         return( nRC );

      // If we are qualifying using a child entity or a subselect then we are
      // not qualifying using only the keys of the qual entity.
      if ( lpQualEntity->bQualUsesChildEntity ||
           lpQualEntity->bContainsSubselect )
      {
         lpQualEntity->bUniqueQualification = FALSE;
      }

      // If we still think we are qualifying using only keys then let's check
      // to see if we are selecting by all the keys.  If the qualfication
      // contains an 'IS' oper then we don't have to worry about checking
      // because it automatically uses all the keys.
      if ( lpQualEntity->bUniqueQualification &&
           lpQualEntity->bContainsIS == FALSE )
      {
         LPVIEWATTRIB lpViewAttrib;
         LPQUALATTRIB lpQualAttrib;

         // We already know that we're qualifying using only keys with 'AND'.
         // Now make sure that we have all the keys.
         for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
               lpViewAttrib;
               lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
         {
            // We only care about keys.
            if ( lpViewAttrib->bKey == FALSE )
               continue;

            // Now see if there is a qual attrib for the key.
            for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
                  lpQualAttrib;
                  lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
            {
               if ( lpQualAttrib->lpViewAttrib == lpViewAttrib )
                  break;
            }

            // If we didn't find qualification for the key then break.
            if ( lpQualAttrib == 0 )
            {
               lpQualEntity->bUniqueQualification = FALSE;
               break;
            }

         } // for ( lpViewAttrib )...
      }

   } // while ( nRC == zCURSOR_SET )...

#if 0  // Change 0 to 1 for debugging...
   // Display the QualEntity structure to trace.
   SqlDisplayQualEntity( *lpFirstQualEntity );
#endif

   return( 0 );
}  /* fnSqlRetrieveQualObject */

/////////////////////////////////////////////////////////////////////////////
//
// SqlRetrieveQualObject
//
// PURPOSE: To retrieve the information in the qualification object and
// store it in a linked list for later.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlRetrieveQualObject( zVIEW         lpView,
                       zVIEW         lpQualView,
                       LPVIEWOD      lpViewOD,
                       LPQUALENTITY  *lpFirstQualEntity,
                       LPBOUNDLIST   lpBoundList )
{
   zSHORT nRC;

   nRC = fnSqlRetrieveQualObject( lpView, lpQualView, lpViewOD,
                                  lpFirstQualEntity, lpBoundList );
   if ( nRC < 0 )
   {
      if ( *lpFirstQualEntity )
         SqlFreeQualEntity( lpFirstQualEntity );
   }

   return( nRC );
}  /* SqlRetrieveQualObject */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlPrintLine
//
// Prints the line stored in szBuffer to the trace file.  *ppchLine points
// to the byte after the last char in szBuffer and must be set to 0.
//
/////////////////////////////////////////////////////////////////////////////
static void
fnSqlPrintLine( zPCHAR  pchBuffer,
                zPCHAR  *ppchLine,
                zPSHORT pnIndentLth,
                zPSHORT pnIndentIncrement,
                zPSHORT pnErrorIdx,
                zPVOID  pfn,
                zPVOID  pInfo )
{
   zLONG  lLth;
   zSHORT n;
   zSHORT nParenIndent;
   zPCHAR pchPtr;
   zPCHAR pchIndentStr;
   void (POPERATION pfnWrite)( zPCHAR, zPVOID ) = (void (POPERATION )( zPCHAR, zPVOID )) pfn;

   // Terminate ppchLine string.
   **ppchLine = 0;

   lLth = zstrlen( pchBuffer ) + 1000;
   pchIndentStr = SysMalloc( lLth );

   // If the first character is a right paren ( ')' ) then un-indent two spaces for each right paren.
   pchPtr = pchBuffer;
   nParenIndent = 0;
   while ( *pchPtr )
   {
      if ( *pchPtr == ')' )
         nParenIndent += 2;
      else
      if ( *pchPtr != ' ' && nParenIndent == 0 )
         break;

      pchPtr++;
   }

   // Set up indent string.
   pchPtr = pchIndentStr;
   for ( n = 0; n < (*pnIndentLth - nParenIndent); n++ )
      *pchPtr++ = ' ';

   *pchPtr = 0;

   strcat_s( pchIndentStr, lLth, pchBuffer );
   (*pfnWrite)( pchIndentStr, pInfo );

   if ( *pnErrorIdx )
   {
      strcpy_s( pchIndentStr, lLth, "ERR> " );
      pchPtr = pchIndentStr;
      mAdvanceToNull( pchPtr );
      for ( n = 0; n < (*pnIndentLth + *pnErrorIdx); n++ )
         *pchPtr++ = ' ';

      *pchPtr = 0;
      TraceLineS( pchIndentStr, "^" );
      *pnErrorIdx = 0;
   }

   *ppchLine     = pchBuffer;
   *pnIndentLth = *pnIndentLth + *pnIndentIncrement;
   *pnIndentIncrement = 0;

   SysFree( pchIndentStr );

} /* fnSqlPrintLine */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlCopyToken
//
// Copy pchToken to the buffer pointed to by pchBuffer.  *ppchLine points
// to the position in pchBuffer where szToken is to be copied and will
// be updated to point to the first character after szToken in pchBuffer.
//
// If pchErrorChar points to a character in pchToken then there is an
// error caused by pchToken.  After pchToken is copied into pchBuffer,
// convert *pnErrorIdx to be the offset of the offending character in
// pchBuffer.
//
/////////////////////////////////////////////////////////////////////////////
static void
fnSqlCopyToken( zPCHAR  pchToken,
                zSHORT  nTokenLth,
                zPCHAR  *ppchLine,
                zPCHAR  pchErrorChar,
                zPCHAR  pchBuffer,
                zPSHORT ppnErrorIdx )
 {
   zSHORT n;

   for ( n = 0; n < nTokenLth; n++ )
   {
      // Copy the n'th character from pchToken to **ppchLine.
      **ppchLine = pchToken[ n ];

      // If the current character in pchToken is the ErrorChar then set
      // pnErrorIdx to be the offset of the character in pchBuffer.
      if ( pchToken + n == pchErrorChar )
         *ppnErrorIdx = *ppchLine - pchBuffer;

      (*ppchLine)++;
   }
} /* fnSqlCopyToken */

/////////////////////////////////////////////////////////////////////////////
//
// fnSqlDisplayCommand
//
// Formats and displays the SQL command pchSqlCmd.
//
// If pchErrorChar is non-zero then the line containing the character
// pointed to by pchErrorChar will be followed by a line with a '^' under
// the character.  This is used to point out errors.
//
/////////////////////////////////////////////////////////////////////////////
static void
fnSqlDisplayCommand( zPCHAR *pchSqlCmdStr,
                     zPCHAR pchErrorChar,
                     zSHORT pnIndentLth,
                     zPCHAR pchInitString,
                     zBOOL  bFirstCall,
                     zPVOID pfnWrite,
                     zPVOID pInfo )
{
   zPCHAR pchBuffer;
   zLONG  hMemBuffer;
   zSHORT nIndentIncrement;
   zSHORT nParenCount;
   zSHORT nTokenLth;
   zSHORT pnErrorIdx;
   zPCHAR pchToken;
   zPCHAR pchLine;
   zPCHAR pchSqlCmd;
   zBOOL  bFirstOnStmt = TRUE;

   hMemBuffer = SysAllocMemory( (zCOREMEM) &pchBuffer, MAX_SQLCMD_LENGTH, 0,
                                zCOREMEM_ALLOC, 0 );
   if ( pchBuffer == 0 )
      return;

   pchLine = pchBuffer;
   if ( pchInitString )
   {
      strcpy_s( pchBuffer, MAX_SQLCMD_LENGTH, pchInitString );
      mAdvanceToNull( pchLine );
      nIndentIncrement = (zSHORT) zstrlen( pchInitString ) + 1;
   }
   else
      nIndentIncrement = 0;

   nParenCount = 0;
   pnErrorIdx   = 0;

   // Loop through, copying each character from pchSqlCmd to pchBuffer for
   // printing.
   pchSqlCmd = *pchSqlCmdStr;
   while ( *pchSqlCmd )
   {
      // Skip spaces.
      while ( zisspace( *pchSqlCmd ) )
         pchSqlCmd++;

      // Get next token.
      pchToken = pchSqlCmd;
      while ( *pchSqlCmd )
      {
         if ( zisspace( *pchSqlCmd ) )
            break;

         // If the character is a single quote, skip everything until ending
         // single quote is found.
         if ( *pchSqlCmd++ == '\'' )
         {
            while ( *pchSqlCmd )
            {
               if ( *pchSqlCmd == '\'' && pchSqlCmd[ 1 ] != '\'' )
                  break;

               if ( *pchSqlCmd == '\'' )
                  pchSqlCmd++;

               pchSqlCmd++;
            }

            if ( *pchSqlCmd == '\'' )
               pchSqlCmd++;
         }
      }

      nTokenLth = (zSHORT) (pchSqlCmd - pchToken);

      if ( pchLine != pchBuffer )
         *pchLine++ = ' ';

#if DBG_DISPLAY
      {
         zCHAR s[ 100 ];

         strncpy_s( s, sizeof( s ), pchToken, nTokenLth );
         s[ nTokenLth ] = 0;
         TraceLineS( "DBG Token> ", s );
      }
#endif

      switch ( *pchToken )
      {
         case '(':
            nParenCount++;
            nIndentIncrement += 2;
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );
            break;

         case ')':
            nParenCount--;
            nIndentIncrement -= 2;
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );

            // If paren count is 0 and we are in a recursive call
            // then get out of recursive call.
            if ( nParenCount == 0 && bFirstCall == FALSE )
            {
              fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                              &nIndentIncrement, &pnErrorIdx,
                              pfnWrite, pInfo );
              *pchSqlCmdStr = pchSqlCmd;
              SysFreeMemory( hMemBuffer );
              return;
            }

            break;

         case ',':
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );
            fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                            &nIndentIncrement, &pnErrorIdx,
                            pfnWrite, pInfo );
            break;

         case 'A':
         case 'O':
            if ( zstrncmpi( pchToken, "ON ", 3 ) == 0 )
            {
               fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                               &nIndentIncrement, &pnErrorIdx,
                               pfnWrite, pInfo );

               // pnIndentLth will be reset during the "WHERE" clause.
               if ( bFirstOnStmt )
                  pnIndentLth += 5;

               bFirstOnStmt = FALSE;
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );

            if ( zstrncmpi( pchToken, "AND ", 4 ) == 0 ||
                 zstrncmpi( pchToken, "OR ", 3 ) == 0 )
            {
               fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                               &nIndentIncrement, &pnErrorIdx,
                               pfnWrite, pInfo );
            }

            break;

         case 'F':
            if ( zstrncmpi( pchToken, "FROM ", 5 ) == 0 )
            {
               fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                               &nIndentIncrement, &pnErrorIdx,
                               pfnWrite, pInfo );
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );
            break;

         case 'I':
            if ( zstrncmpi( pchToken, "INTO ", 5 ) == 0 )
            {
               fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                               &nIndentIncrement, &pnErrorIdx,
                               pfnWrite, pInfo );
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );

            if ( zstrncmpi( pchToken, "IN ", 3 ) == 0 )
            {
               fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                               &nIndentIncrement, &pnErrorIdx,
                               pfnWrite, pInfo );
               fnSqlDisplayCommand( &pchSqlCmd, pchErrorChar,
                                    pnIndentLth, 0, FALSE,
                                    pfnWrite, pInfo );
            }

            break;

         case 'S':
            if ( zstrncmpi( pchToken, "SELECT ", 7 ) == 0 )
            {
               if ( bFirstCall == FALSE )
                  fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                                  &nIndentIncrement, &pnErrorIdx,
                                  pfnWrite, pInfo );
            }
            else
            if ( zstrncmpi( pchToken, "SET ", 4 ) == 0 )
            {
               fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                               &nIndentIncrement, &pnErrorIdx,
                               pfnWrite, pInfo );
               nIndentIncrement += 4;
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );
            break;

         case 'V':
            if ( zstrncmpi( pchToken, "VALUES ", 7 ) == 0 )
            {
               fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                               &nIndentIncrement, &pnErrorIdx,
                               pfnWrite, pInfo );
               nIndentIncrement += 7;
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );
            break;

         case 'W':
            if ( zstrncmpi( pchToken, "WHERE ", 6 ) == 0 )
            {
               zCHAR szTemp[ 10 ];

               fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                               &nIndentIncrement, &pnErrorIdx,
                               pfnWrite, pInfo );

               if ( bFirstCall )
                  pnIndentLth = 0;

               // Because of OpenSQL, we want to use the generated
               // WHERE because it might be lower-case.
               strncpy_s( szTemp, sizeof( szTemp ), pchToken, 5 );
               szTemp[ 5 ] = 0;

               fnSqlDisplayCommand( &pchSqlCmd, pchErrorChar,
                                    pnIndentLth, szTemp, FALSE,
                                    pfnWrite, pInfo );
               break;
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );
            break;

         default:
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar,
                            pchBuffer, &pnErrorIdx );
            break;

      } // switch ( *pchToken )...

   } // while ( *pchSqlCmd )...

   if ( pchLine > pchBuffer )
      fnSqlPrintLine( pchBuffer, &pchLine, &pnIndentLth,
                      &nIndentIncrement, &pnErrorIdx, pfnWrite, pInfo );

   *pchSqlCmdStr = pchSqlCmd;
   SysFreeMemory( hMemBuffer );

}  /* fnSqlDisplayCommand */

/////////////////////////////////////////////////////////////////////////////
//
// SqlDisplayCommandEx
//
/////////////////////////////////////////////////////////////////////////////
void OPERATION
SqlDisplayCommandEx( zPCHAR pchSqlCmd,
                     zPCHAR pchErrorChar,
                     zPVOID pfnWrite,
                     zPVOID pInfo )
{
   zLONG  hMemBuffer;
   zPCHAR pchBuffer;
   zLONG  lLth;

#if DBG_DISPLAY
   zSHORT n, m;
   zCHAR  c;
   zCHAR  s[ 201 ];
   zPCHAR p;

   // Print out the unformatted SQL command for debugging purposes.
   n = zstrlen( pchSqlCmd );
   p = pchSqlCmd;
   while ( n > 200 )
   {
      c = p[ 200 ];
      p[ 200 ] = 0;
      TraceLineS( "DBG> ", p );

      if ( pchErrorChar && pchErrorChar >= p && pchErrorChar < &p[ 200 ] )
      {
         m = (zSHORT) (pchErrorChar - p);
         zmemset( s, ' ', m );
         s[ m ] = '^';
         s[ m + 1 ] = 0;
         TraceLineS( "ERR> ", s );
      }

      p = &p[ 200 ];
      *p = c;
      n = n - 200;
   }

   TraceLineS( "DBG> ", p );
   TraceLineS( "---- ", "" );
#endif

   // Copy the command to a temporary buffer because we will change the SQL
   // string while we are printing it.
   lLth = zstrlen( pchSqlCmd ) + 1;
   hMemBuffer = SysAllocMemory( (zCOREMEM) &pchBuffer, lLth, 0, zCOREMEM_ALLOC, 0 );
   if ( pchBuffer == 0 )
      return;

   strcpy_s( pchBuffer, lLth, pchSqlCmd );

   if ( pchErrorChar )
      pchErrorChar = pchBuffer + (pchErrorChar - pchSqlCmd);

   // Call fnSqlDisplayCommand with an initial indent value of 0.
   fnSqlDisplayCommand( &pchBuffer, pchErrorChar, 0, 0, TRUE, pfnWrite, pInfo );

   SysFreeMemory( hMemBuffer );

}  // SqlDisplayCommand

// fnTraceSql cannot be static because its function pointer is
// explicitly cast to POPERATION.
void OPERATION
fnTraceSql( zPCHAR pch, zPVOID pDummy )
{
#if 0
   if ( strstr( pch, "SELECT ATTENDAN" ) )  // dks debug 2005.12.13
   {
      int k = 0;

      k /= k;
      TraceLine( "fnTraceSql Cmd: 0x%08x", pch );
   }
#endif
   TraceLineS( "SQL>  ", pch );
}

/////////////////////////////////////////////////////////////////////////////
//
// SqlDisplayCommand
//
/////////////////////////////////////////////////////////////////////////////
void OPERATION
SqlDisplayCommand( zPCHAR pchSqlCmd, zPCHAR pchErrorChar )
{
   SqlDisplayCommandEx( pchSqlCmd, pchErrorChar, (zPVOID) fnTraceSql, 0 );
}

/*
// Initialize the bound attribute list structure.
//
// Parameters:
//    lpBoundList  - Pointer to BoundList structure.  This is what we are
//                   initializing.
//    pfnCallback  - The dbh_callback function.  May be 0.
//    pvPtr        - The pointer is passed to the dbh_callback function to
//                   allow information from the dbhandler to be passed.
*/
zBOOL OPERATION
SqlInitBoundAttrList( LPBOUNDLIST   *plpBoundList,
                      zPCHAR        pchEntityJoinTable,
                      zDBH_CALLBACK pfnCallback,
                      zULONG        lFuncs,
                      zPVOID        pvPtr )
{
   zLONG       lHandle;
   LPBOUNDLIST lpBoundList;

   lHandle = SysAllocMemory( (zCOREMEM) plpBoundList,
                             sizeof( BoundAttrListRecord ), 0,
                             zCOREMEM_ALLOC, 0 );
// TraceLine( "SqlInitBoundAttrList allocating memory: 0x%08x  for %d bytes",
//            lHandle, sizeof( BoundAttrListRecord ) );

   lpBoundList = *plpBoundList;
   zmemset( lpBoundList, 0, sizeof( BoundAttrListRecord ) );

   lpBoundList->hMemory            = lHandle;
   lpBoundList->nBoundCount        = 0;
   lpBoundList->lpFirstBoundAttr   = 0;
   lpBoundList->pfnCallback        = pfnCallback;
   lpBoundList->lFuncs             = lFuncs;
   lpBoundList->pchEntityJoinTable = pchEntityJoinTable;
   lpBoundList->pvPtr              = pvPtr;

   return( TRUE );
}

//
// Adds the column/data-field to the END of the bound-attribute list.
//
// Returns:  TRUE  - Everything OK.
//           FALSE - Something wrong.
//
zBOOL OPERATION
SqlAddBoundAttr( LPBOUNDLIST  lpBoundList,
                 LPDATAFIELD  lpDataField,
                 zVIEW        lpView,
                 zPVOID       pchValue,
                 zLONG        lHMemValue )
{
   LPBOUNDATTR lpBoundAttr = 0;
   zLONG       lHMemHandle;

   lHMemHandle = SysAllocMemory( (zPVOID) &lpBoundAttr, sizeof( BoundAttr ),
                                 0, zCOREMEM_ALLOC, 0 );

   if ( lpBoundAttr == 0 )
      return( FALSE );

   lpBoundAttr->lpDataField     = lpDataField;
   lpBoundAttr->lpView          = lpView;
   lpBoundAttr->lHMemHandle     = lHMemHandle;
   lpBoundAttr->lHMemValue      = lHMemValue;
   lpBoundAttr->pszValue        = pchValue;
   lpBoundAttr->lpNextBoundAttr = 0;

   // If the list currently has no entries then just add the current one.
   if ( lpBoundList->lpFirstBoundAttr == 0 )
      lpBoundList->lpFirstBoundAttr = lpBoundAttr;
   else
   {
      LPBOUNDATTR lpPrevBound;

      // Find the end of the list and add the current lpBoundAttr to the end.

      for ( lpPrevBound = lpBoundList->lpFirstBoundAttr;
            lpPrevBound->lpNextBoundAttr;
            lpPrevBound = lpPrevBound->lpNextBoundAttr )
      {
         ; // Nothing needs to be done here.
      }

      lpPrevBound->lpNextBoundAttr = lpBoundAttr;
   }

   lpBoundList->nBoundCount++;

   return( TRUE );

} // SqlAddBoundAttr

#if 1

zSHORT OPERATION
SqlFreeBoundAttrList( LPBOUNDLIST  lpBoundList )
{
   LPBOUNDATTR lpNextBound;
   zLONG       lHMemHandle;

   if ( lpBoundList == 0 )
      return( 0 );

   if ( lpBoundList->lpFirstBoundAttr )
   {
      lpNextBound = lpBoundList->lpFirstBoundAttr;

      do
      {
         if ( lpNextBound->lHMemValue )
         {
            SysFreeMemory( lpNextBound->lHMemValue );
         // TraceLine( "SqlFreeBoundAttrList freeing lHMemValue: 0x%08x",
         //            lpNextBound->lHMemValue );
         }

         lHMemHandle = lpNextBound->lHMemHandle;
         lpNextBound = lpNextBound->lpNextBoundAttr;

      // TraceLine( "SqlFreeBoundAttrList freeing lHMemHandle: 0x%08x",
      //            lHMemHandle );
         SysFreeMemory( lHMemHandle );

      } while ( lpNextBound );
   }

// TraceLine( "SqlFreeBoundAttrList freeing lpBoundList hMemory: 0x%08x",
//            lpBoundList->hMemory );
   SysFreeMemory( lpBoundList->hMemory );

   return( 0 );

} // SqlFreeBoundAttrList

#else

zSHORT OPERATION
SqlFreeBoundAttrList( LPBOUNDLIST  lpBoundList )
{
   LPBOUNDATTR lpNextBound;

   if ( lpBoundList == 0 )
      return( 0 );

   if ( lpBoundList->lpFirstBoundAttr )
   {
      lpNextBound = lpBoundList->lpFirstBoundAttr;
      do
      {
         if ( lpNextBound->lHMemValue )
         {
            SysFreeMemory( lpNextBound->lHMemValue );
            lpNextBound->lHMemValue = 0;
         }

         SysFreeMemory( lpNextBound->lHMemHandle );
         lpNextBound->lHMemHandle = 0;
         lpNextBound = lpNextBound->lpNextBoundAttr;

      } while ( lpNextBound );
   }

// TraceLine( "SqlFreeBoundAttrList freeing memory: 0x%08x", lpBoundList->hMem );
   SysFreeMemory( lpBoundList->hMem );
   lpBoundList->hMem = 0;

   return( 0 );

} // SqlFreeBoundAttrList

#endif
