/////////////////////////////////////////////////////////////////////////////

   KZHSQLGA.C - Generates SQL calls for dbhandlers.

   Programming Notes:

      o  All function return codes use the following convention:
         - rc = 0 means that the function worked as intended.
         - rc > 0 means that the function worked but with warnings.
         - rc < 0 means that an error occured.

///////////////////////////// DEFINES ///////////////////////////////////////
#define Debug 1

#define  KZSYSSVC_INCL
#define  KZMETA_INCL
#include <kzoengaa.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "KZHSQLGP.H"


#define MAX_WHERE_LENGTH        4000
#define MAX_FROM_LENGTH         300

#define zSELECT_CMD             0x0001
#define zSUBSELECT_CMD          0x0002
#define zINSERT_CMD             0x0004
#define zDELETE_CMD             0x0008
#define zUPDATE_CMD             0x0010
#define zCORR_CMD               0x1000

#define zFULLY_QUALIFIED        0x0001
#define zKEYS_ONLY              0x0002

#define zSUB_ENTITY             1
#define zROOT_ENTITY            2
#define zERROR                  0
#define zWARNING                1

#define mAdvanceToNull( a )     while ( *(a) ) (a)++

zCHAR szlEntitySpec[]    = "EntitySpec";
zCHAR szlQualAttrib[]    = "QualAttrib";
zCHAR szlSubQualAttrib[] = "SubQualAttrib";

typedef struct SqlStatementSruct SqlStatementRecord;
typedef SqlStatementRecord *LPSQLSTATEMENT;
struct SqlStatementSruct
{
   zCHAR  szText[ 10 ];       // Used to help debugging.
   zSHORT nCommandType;       // Id of command type (Select, Insert, etc. )
   zPCHAR szSqlCmd;           // Ptr to buffer that can hold entire SQL cmd.
   zPCHAR szSqlCmdPtr;        // Ptr to null terminator of szSqlCmd.
   zPCHAR szFrom;             // Ptr to buffer to temporarily hold FROM clause.
   zPCHAR szFromPtr;          // Ptr to null terminator of szFrom.
   zPCHAR szWhere;            // Ptr to buffer to temporarily hold WHERE clause.
   zPCHAR szWherePtr;         // Ptr to null terminator of szWhere.
   zSHORT nComponentCount;    // Current # of components in WHERE clause.
   zSHORT nFromTableCount;    // # of tables in FROM clause.
   zPCHAR lpFromTable[ 10 ];  // List of pointers to table names in FROM clause.
   zLONG  hWhereMem;          // Memory handle for szWhere.
   zLONG  hFromMem;           // Memory handle for szFrom.
};

zSHORT
SqlAddQualToWhere( zCHAR          cDBType,
                   zVIEW          lpView,
                   LPQUALENTITY   lpQualEntity,
                   LPQUALATTRIB   lpFirstQualAttrib,
                   LPSQLSTATEMENT lpSql );

#define fnSqlAddStringToSql( a, b )   \
                        (a)->szSqlCmdPtr = zstrcatp( (a)->szSqlCmdPtr, (b) )
#define fnSqlAddStringToWhere( a, b ) \
                        (a)->szWherePtr = zstrcatp( (a)->szWherePtr, (b) )
#define fnSqlAddTableToFrom( a, b )   \
                        (a)->lpFromTable[ (a)->nFromTableCount++ ] = (b)
#define fnSqlWhereIsEmpty( a )            (!*(a)->szWhere)
#define fnSqlWhereIsNotEmpty( a )         (*(a)->szWhere)
#define fnSqlFromIsEmpty( a )             (!*(a)->szFrom)
#define fnSqlFromIsNotEmpty( a )          (*(a)->szFrom)
#define fnSqlIncrementComponentCount( a ) (a)->nComponentCount++

/////////////////////////////////////////////////////////////////////////////

      fnSqlInitSqlStatement

/////////////////////////////////////////////////////////////////////////////
zSHORT
fnSqlInitSqlStatement( LPSQLSTATEMENT lpSql,
                       zSHORT         nCommandType,
                       zPCHAR         pchSqlCmd )
{
   zLONG  hMem;
   zPCHAR lpPtr;

   zmemset( (zPVOID) lpSql, sizeof( SqlStatementRecord ), 0 );
   if ( pchSqlCmd )
   {
      pchSqlCmd[ 0 ] = 0;             // ensure SQL initialized with null
      lpSql->szSqlCmd    = pchSqlCmd;
      lpSql->szSqlCmdPtr = pchSqlCmd;
   }

   // Set up some debugging stuff.
   strcpy_s( lpSql->szText, "-DGC- dbg" );

   // Allocate memory for WHERE clause.
   hMem = SysAllocMemory( &lpPtr, MAX_WHERE_LENGTH, 0, zCOREMEM_ALLOC, 0 );
   if ( hMem == 0 )
      return( zCALL_ERROR );

   lpPtr[ 0 ] = 0;         // ensure WHERE clause initialized with null
   lpSql->szWhere    = lpPtr;
   lpSql->szWherePtr = lpPtr;
   lpSql->hWhereMem  = hMem;

   // Allocate memory for FROM clause.
   hMem = SysAllocMemory( &lpPtr, MAX_FROM_LENGTH, 0, zCOREMEM_ALLOC, 0 );
   if ( hMem == 0 )
      return( zCALL_ERROR );

   lpPtr[ 0 ] = 0;         // ensure FROM clause initialized with null
   lpSql->szFrom    = lpPtr;
   lpSql->szFromPtr = lpPtr;
   lpSql->hFromMem  = hMem;

   lpSql->nCommandType = nCommandType;

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      fnSqlFreeSqlStatement

/////////////////////////////////////////////////////////////////////////////
zSHORT
fnSqlFreeSqlStatement( LPSQLSTATEMENT lpSql )
{
   SysFreeMemory( lpSql->hWhereMem );
   SysFreeMemory( lpSql->hFromMem );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      fnSqlAssembleSqlStatement

/////////////////////////////////////////////////////////////////////////////
zSHORT
fnSqlAssembleSqlStatement( LPSQLSTATEMENT lpSql )
{
   if ( lpSql->szFrom[ 0 ] )
   {
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " FROM " );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpSql->szFrom );
   }

   if ( lpSql->szWhere[ 0 ] )
   {
      if ( lpSql->nComponentCount > 1 )
      {
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " WHERE ( " );
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpSql->szWhere );
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " )" );
      }
      else
      {
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " WHERE " );
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpSql->szWhere );
      }
   }

   fnSqlFreeSqlStatement( lpSql );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      fnSqlAddStringToFrom

/////////////////////////////////////////////////////////////////////////////
void
fnSqlAddStringToFrom( LPSQLSTATEMENT lpSql,
                      zPCHAR         pchStr )
{
   // If FROM clause is not empty, then add comma.
   if ( fnSqlFromIsNotEmpty( lpSql ) )
      lpSql->szFromPtr = zstrcatp( lpSql->szFromPtr, ", " );

   lpSql->szFromPtr = zstrcatp( lpSql->szFromPtr, pchStr );
   fnSqlAddTableToFrom( lpSql, pchStr );
}

/////////////////////////////////////////////////////////////////////////////

      SqlFindTableName

      RETURNS: TRUE  - if lpTableName is contained in the From table list;
               FALSE - if it isn't.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlFindTableName( zPCHAR         pchTableName,
                  LPSQLSTATEMENT lpSql )
{
   zSHORT nTableCount;

   nTableCount = lpSql->nFromTableCount;
   while ( nTableCount-- > 0 )
   {
      if ( zstrcmp( lpSql->lpFromTable[ nTableCount ], pchTableName ) == 0 )
         return( TRUE );
   }

   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////

      SqlUnformatDateTimeString

      Fills in any blanks with zeros so that value is in the form:
         YYYYMMDDHHMMSSTTT.

      Returns: 0 - Everything ok

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlUnformatDateTimeString( zPCHAR pchTgt, zPCHAR pchSrc )
{
   zSHORT k, j;

   // Copy source to target, converting spaces to zeros.
   for ( k = j = 0; pchSrc[ k ] != 0; k++ )
   {
      if ( pchSrc[ k ] == ' ' )
         pchTgt[ j++ ] = '0';
      else
         pchTgt[ j++ ] = pchSrc[ k ];
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

 ???? SqlConvertStringToSqlString

      Converts a string value to a string value that the SQL database understands.  For example:

         abcd        -> 'abcd'
         abc'd       -> 'abc''d'
         1234        -> 1234
         19930516    -> '1993-05-16'  // Is this how dates are handled????

      Returns: 0 - Everything ok

      NOTE: THIS FUNCTION ASSUMES THAT THE SOURCE STRING WILL NOT(!) BE NULL!

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlConvertStringToSqlString( zCHAR        cDBType,
                             LPDATAFIELD  lpDataField,
                             zPCHAR       pchSrc,
                             zPCHAR       pchTgt )
{
   zPCHAR sz;
   zCHAR  szDateTime1[ 30 ];
   zCHAR  szDateTime2[ 30 ];

   switch ( lpDataField->cFldType )
   {
      case zTYPE_STRING:
         // Go through the string and check for single quotes and double them up (ie "don't" becomes "don''t").
         *pchTgt++ = '\'';

         // If there are any imbedded single quotes, then double them up.
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

         *pchTgt++ = '\'';
         *pchTgt   = 0;
         break;

      case zTYPE_DATETIME:
         // Get date-time from pchSrc as an unformatted string -- yyyymmddhhmmsss.
         strcpy_s( szDateTime1, sizeof( szDateTime1 ), pchSrc );

         // Make sure that the DateTime doesn't contain any spaces.
         sz = szDateTime1;
         while ( sz[ 0 ] )
         {
            if ( sz[ 0 ] == ' ' )
               sz[ 0 ] = '0';

            sz++;
         }

         // Convert DateTime string to a formatted string that proprietary SQL understands.
         switch ( cDBType )
         {
            case zSQL_SQLBASE:
                  UfFormatDateTime( szDateTime2, sizeof( szDateTime2 ), szDateTime1, "YYYY-mm-DD HH:MI:SS:999" );
               break;

            case zSQL_SQLSERVER:
                  UfFormatDateTime( szDateTime2, sizeof( szDateTime2 ), szDateTime1, "Mmm dd YYYY HH:MI:SS:999" );
               break;
         } // switch ( cDBType )...

         // Date string must start with a quote.
         *pchTgt++ = '\'';

         // Copy formatted date-time string to pchTgt and terminate
         // with a quote.
         strcpy_s( pchTgt, szDateTime2 );
         strcat_s( pchTgt, "'" );
         break;

      default:
         // For all other datatypes, put it in pchTgt.
         strcpy_s( pchTgt, pchSrc );
         break;

   } // switch ( lpDataField->cFldType )...

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      SqlAttributeIsNull

      Returns TRUE or FALSE depending on whether current attribute defined by lpDataField is NULL.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlAttributeIsNull( zCHAR          cDBType,
                    zVIEW          lpView,
                    LPDATAFIELD    lpDataField )
{
   zPCHAR  pchStr;
   zCHAR   chType;
   zUSHORT uLth;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;

   lpViewAttrib = lpDataField->lpViewAttrib;
   lpViewEntity = lpViewAttrib->lpViewEntity;

   switch ( lpDataField->cFldType )
   {
      zCHAR sz[ 300 ];

      case zTYPE_STRING:
         // For SqlServer, a string is never considered null.
         if ( cDBType == zSQL_SQLSERVER )
            return( FALSE );

         GetValueFromRecord( lpView, lpViewEntity, lpViewAttrib, &pchStr, &chType, &uLth );
         if ( pchStr == 0 || *pchStr == 0 )
            return( TRUE );
         else
            return( FALSE );
         break;

      default:
         // Get string from record with a max length of 256.
         if ( GetStringFromRecord( lpView, lpViewEntity, lpViewAttrib, sz, 20 ) == -1 )
            return( TRUE );
         else
            return( FALSE );
         break;
   }
}

/////////////////////////////////////////////////////////////////////////////

      SqlGetNullString

      Returns in szValue the value that represents NULL for a particular
      database.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlGetNullString( zCHAR          cDBType,
                  zPCHAR         pchValue,
                  LPDATAFIELD    lpDataField )
{
   switch ( cDBType )
   {
      case zSQL_SQLBASE:
         strcpy_s( pchValue, "null" );
         break;

      case zSQL_SQLSERVER:
         if ( lpDataField->cFldType == zTYPE_STRING )
            strcpy_s( pchValue, "''" );      // two single quotes.
         else
            strcpy_s( pchValue, "NULL" );
         break;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      SqlGetAttributeValue

      Retrieve the string value of an attribute and concats it to the end
      of the :

            INSERT if current SQL cmd is an INSERT
            UPDATE if current SQL cmd is an UPDATE and attrib is not a key.
               --or--
            WHERE clause if not.

      Returns: 0 - Everything ok
               1 - Everything ok, but attribute value is null.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlGetAttributeValue( zCHAR          cDBType,
                      zVIEW          lpView,
                      LPDATAFIELD    lpDataField,
                      LPSQLSTATEMENT lpSql )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   zBOOL   bAdvanceSql;
   zPCHAR  pchValue;
   zPCHAR  pchStr;
   zCHAR   chType;
   zUSHORT uLth;
   zSHORT  nRC;

   nRC = 0;    // Assume that attribute is not null.
   lpViewAttrib = lpDataField->lpViewAttrib;
   lpViewEntity = lpViewAttrib->lpViewEntity;

   // If the command is an INSERT or UPDATing a non-key, then add value to
   // main SQL command, otherwise add value to the WHERE part of command.
   if ( (lpSql->nCommandType & zINSERT_CMD) || (lpSql->nCommandType & zUPDATE_CMD && lpViewAttrib->bKey == FALSE) )
   {
      pchValue = lpSql->szSqlCmdPtr;
      bAdvanceSql = TRUE;
   }
   else
   {
      pchValue = lpSql->szWherePtr;
      bAdvanceSql = FALSE;
   }

   // If attribute is null, then insert language specific null.
   if ( SqlAttributeIsNull( cDBType, lpView, lpDataField ) )
   {
      SqlGetNullString( cDBType, pchValue, lpDataField );
      nRC = 1;    // Indicate that attribute is null.
   }
   else
   switch ( lpDataField->cFldType )
   {
      zCHAR sz[ 300 ];

      case zTYPE_STRING:
         GetValueFromRecord( lpView, lpViewEntity, lpViewAttrib, &pchStr, &chType, &uLth );
  ????   SqlConvertStringToSqlString( cDBType, lpDataField, pchStr, pchValue );
         break;

      case zTYPE_DATETIME:
         // Get date-time from record as an unformatted string --
         // yyyymmddhhmmsss.
         GetStringFromRecord( lpView, lpViewEntity, lpViewAttrib, sz, 20 );
  ????   SqlConvertStringToSqlString( cDBType, lpDataField, sz, pchValue );
         break;

      default:
         // For all other datatypes, get the value of the attribute as a
         // string and put it in pchValue.

         // Get string from record with a max length of 256.
         GetStringFromRecord( lpView, lpViewEntity, lpViewAttrib, sz, 256 );
  ????   SqlConvertStringToSqlString( cDBType, lpDataField, sz, pchValue );
         break;
   }

   if ( bAdvanceSql )
      mAdvanceToNull( lpSql->szSqlCmdPtr );
   else
      mAdvanceToNull( lpSql->szWherePtr );

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildColumnList

      Build a list of the columns i.e. "Col1, Col2, Col3...".
      Uses fnSqlAddStringToSql(...) to add string to SQL statement.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlBuildColumnList( zCHAR          cDBType,
                    LPVIEWENTITY   lpViewEntity,
                    LPSQLSTATEMENT lpSql,
                    zSHORT         nControl )
{
   LPVIEWATTRIB lpViewAttrib;
   LPDATAFIELD  lpDataField;
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   zCHAR        szColumnName[ 200 ];
   zBOOL        bFirstTime;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;

   bFirstTime = TRUE;
   lpDataField = lpDataRecord->lpFirstDataField;
   for ( ; lpDataField; lpDataField = lpDataField->lpNextDataField )
   {
      lpViewAttrib = lpDataField->lpViewAttrib;

      if ( lpViewAttrib->bKey == FALSE && (nControl & zKEYS_ONLY) )
         continue;

      // If the attribute is an Auto Seq attribute and the relationship is many-to-many,
      // then the attribute is stored in the corrsponding table.  If the command type is
      // also INSERT, then the attribute is not to be included in this list.
      if ( lpViewAttrib->bAutoSeq &&
           lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY &&
           lpSql->nCommandType & zINSERT_CMD )
      {
         continue;
      }

      // Use only persistent attributes.
      if ( lpViewAttrib->bPersist == FALSE )
         continue;

      if ( bFirstTime )
         bFirstTime = FALSE;
      else
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH ", " );

      // Init szColumnName to null string.
      szColumnName[ 0 ] = 0;

      // Add table names if columns need to be fully qualified.
      if ( nControl & zFULLY_QUALIFIED )
      {
         if ( lpViewAttrib->bAutoSeq &&
              lpRelRecord &&
              lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
         {
            // Qualify column using corresponding table.
            strcpy_s( szColumnName, sizeof( szColumnName ), lpRelRecord->lpRecordName );
         }
         else
            strcpy_s( szColumnName, sizeof( szColumnName ), lpDataRecord->lpRecordName );

         strcat_s( szColumnName, sizeof( szColumnName ), "." );
      }

      // Now add column name to list.  For date/time columns, sometimes
      // certain stuff needs to be added.
      if ( lpDataField->cFldType == zTYPE_DATETIME )
      {
         switch ( cDBType )
         {
            // For SqlBase, just add column name to list.
            case zSQL_SQLBASE:
               strcat_s( szColumnName, sizeof( szColumnName ), lpDataField->lpFldName );
               break;

            // For SqlServer, on Insert just add column name to list. On
            // Select, use CONVERT function to get date in a certain format.
            case zSQL_SQLSERVER:
               if ( lpSql->nCommandType & zINSERT_CMD )
                  strcat_s( szColumnName, sizeof( szColumnName ), lpDataField->lpFldName );
               else
               {
                  zCHAR sz[ 300 ];

                  // Set szColumnName to "CONVERT( VARCHAR, [tbl.]column, 109 )"
                  strcpy_s( sz, sizeof( sz ), "CONVERT( VARCHAR, " );
                  if ( szColumnName[ 0 ] )
                     strcat_s( sz, sizeof( sz ), szColumnName );

                  strcat_s( sz, sizeof( sz ), lpDataField->lpFldName );
                  strcat_s( sz, sizeof( sz ), ", 109 )" );
                  strcpy_s( szColumnName, sizeof( szColumnName ), sz );
               }

               break;
         }
      }
      else
         // For other field types, just add column name to list.
         strcat_s( szColumnName, sizeof( szColumnName ), lpDataField->lpFldName );

      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH szColumnName );
   } // for (; lpDataField; ... )...

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildColumnValueList

      Builds a list values taken from the entity instance
         (e.g.  "'Larry Bird', '10 Celtic Ave', 02134 , ...").

      Used mostly for insert.

      Returns: 0 - Everything OK

         For SqlBase:
               1 - Everything OK, but long varchar variables need to be bound.

         For SqlServer:
               1 - Everything OK, but blob and/or text cols need to be written.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlBuildColumnValueList( zCHAR          cDBType,
                         zVIEW          lpView,
                         LPVIEWENTITY   lpViewEntity,
                         LPSQLSTATEMENT lpSql )
{
   LPVIEWATTRIB lpViewAttrib;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPRELRECORD  lpRelRecord;
   zBOOL        bFirstTime;
   zSHORT       nLongCharCount;
   zSHORT       nRC;

   nRC = 0;  // Default of return code is 0.

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;

   nLongCharCount = 0;
   bFirstTime     = TRUE;
   lpDataField = lpDataRecord->lpFirstDataField;
   for (; lpDataField; lpDataField = lpDataField->lpNextDataField )
   {
      lpViewAttrib = lpDataField->lpViewAttrib;

      // If the attribute is not persistent, then try again with next field.
      if ( lpViewAttrib->bPersist == FALSE )
         continue;

      // If the attribute is an auto seq attribute and the parent relationship
      // is many-to-many, then the attribute is stored in the corresponding
      // table and not include in the column value list.
      if ( lpViewAttrib->bAutoSeq &&
           lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         continue;
      }

      // If column is not the first, then append comma.
      if ( bFirstTime )
         bFirstTime = FALSE;
      else
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " , " );

      switch ( cDBType )
      {
         case zSQL_SQLBASE:
            if ( lpDataField->cFldType == zTYPE_BLOB ||
                 lpDataField->uFldLth > 254 )
            {
               zCHAR sz[ 20 ];

               nRC = 1;   // Indicate that blobs/long text fields are used.

               // For LONG VARCHAR variables, create a bind variable.
               nLongCharCount++;
               fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH ":" );
               zltoa( nLongCharCount, sz );
               fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH sz );
            }
            else
               SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );

            break;

         case zSQL_SQLSERVER:
            if ( lpDataField->cFldType == zTYPE_BLOB ||
                 lpDataField->uFldLth > 255 )
            {
               nRC = 1;   // Indicate that blobs/long text fields are used.

               // A null string must be inserted for variable length columns so
               // that they can be updated later.
               fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "''" );
            }
            else
               SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );
            break;

         default:
            SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );
      } // switch ( cDBType )...

   } // for (; lpDataField )...

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildWhere

      Build "WHERE col-name = attrib-value [ AND col-name2 = attrib-value2...]
      for each of the keys in lpViewEntity.

      Used by Update and Delete.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlBuildWhere( zCHAR          cDBType,
               zVIEW          lpView,
               LPVIEWOD       lpViewOD,
               LPVIEWENTITY   lpViewEntity,
               LPSQLSTATEMENT lpSql,
               zSHORT         nQual )
{
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPVIEWATTRIB lpViewAttrib;
   zSHORT       bKeyFound;

   bKeyFound = FALSE;
   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpDataField  = lpDataRecord->lpFirstDataField;
   while ( lpDataField )
   {
      lpViewAttrib = lpDataField->lpViewAttrib;

      if ( lpViewAttrib->bKey )
      {
         bKeyFound = TRUE;
         if ( fnSqlWhereIsEmpty( lpSql ) )
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " ( " );
         else
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " AND " );

         if ( nQual == zFULLY_QUALIFIED )
         {
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpDataRecord->lpRecordName );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
         }

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpDataField->lpFldName );
         if ( SqlAttributeIsNull( cDBType, lpView, lpDataField ) )
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " IS " );
         else
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " = " );
         SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );
      }

      lpDataField = lpDataField->lpNextDataField;
   }

   if ( bKeyFound )
   {
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " )" );
      return( 0 );
   }
   else
   {
      zCHAR szMsg[ 400 ];

      strcpy_s( szMsg, sizeof( szMsg ), "SQL Error creating WHERE clause -- no key attribute "
               "found in object definition.\nObject name = " );
      strcat_s( szMsg, sizeof( szMsg ), lpViewOD->lpName );
      strcat_s( szMsg, sizeof( szMsg ), "\nEntity name = " );
      strcat_s( szMsg, sizeof( szMsg ), lpViewEntity->lpName );
      IssueError( lpView, 16, 16, szMsg );

      return( zCALL_ERROR );
   }
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildInsert

      Returns: 0           - Everything OK.
               zCALL_ERROR - Processing error!

         For SqlBase:
               1 - Everything worked OK, but some long varchars must be bound.

         For SqlServer:
               1 - Everything OK, but blob and/or text cols need to be written.

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildInsert( zCHAR        cDBType,
                zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zPCHAR       pchSqlCmd )
{
   LPDATARECORD   lpDataRecord;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zSHORT         nRC;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zINSERT_CMD, pchSqlCmd );
   lpDataRecord = lpViewEntity->lpFirstDataRecord;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "INSERT INTO " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpDataRecord->lpRecordName );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " ( " );

   nRC = SqlBuildColumnList( cDBType, lpViewEntity, lpSql, 0 );
   if ( nRC < 0 )
   {
      fnSqlFreeSqlStatement ( lpSql );
      return( zCALL_ERROR );
   }

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " ) VALUES ( " );
   nRC = SqlBuildColumnValueList( cDBType, lpView, lpViewEntity, lpSql );
   if ( nRC < 0 )
   {
      fnSqlFreeSqlStatement( lpSql );
      return( zCALL_ERROR );
   }

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " )" );

   fnSqlFreeSqlStatement( lpSql );
   //fnSqlAssembleSqlStatement( lpSql );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildInsertCorrTable

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildInsertCorrTable( zCHAR        cDBType,
                         zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd )
{
   LPVIEWATTRIB   lpViewAttrib;
   LPDATARECORD   lpDataRecord;
   LPDATAFIELD    lpDataField;
   LPRELRECORD    lpRelRecord;
   LPRELFIELD     lpRelField;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zINSERT_CMD | zCORR_CMD, pchSqlCmd );
   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "INSERT INTO " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpRelRecord->lpRecordName );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " ( " );

   // Add list of corr table column names to sql cmd.
   lpRelField = lpRelRecord->lpFirstRelField;
   while ( lpRelField )
   {
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpFldName );

      lpRelField = lpRelField->lpNextRelField;
      if ( lpRelField )
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH ", " );
   }

   // If the entity has a seq attribute, that also must be inserted.
   if ( lpViewEntity->bAutoSeq )
   {
      lpDataField = lpDataRecord->lpFirstDataField;
      while ( lpDataField )
      {
         lpViewAttrib = lpDataField->lpViewAttrib;
         if ( lpViewAttrib->bAutoSeq )
         {
            fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH ", " );
            fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpDataField->lpFldName );
            break;  // Only 1 seq per entity, so break out.
         }

         lpDataField = lpDataField->lpNextDataField;
      } // while ( lpDataField )...
   } // if ( lpViewEntity->bAutoSeq )...
   else
      lpDataField = 0;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " ) VALUES ( " );

   // Add list of corr table column values to sql cmd.
   lpRelField = lpRelRecord->lpFirstRelField;
   while ( lpRelField )
   {
      SqlGetAttributeValue( cDBType, lpView, lpRelField->lpSrcDataField, lpSql );

      lpRelField = lpRelField->lpNextRelField;
      if ( lpRelField )
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH ", " );
   }

   // If a seq attribute was found earlier, then add it to insert.
   if ( lpDataField )
   {
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH ", " );
      SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );
   }

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " )" );

   fnSqlAssembleSqlStatement( lpSql );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildUpdate

      RETURNS:
               0 - OK.

         For SqlBase:
               1 - Everything worked OK, but some long varchars must be bound.

         For SqlServer:
               1 - Everything OK, but blob and/or text cols need to be written.

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildUpdate( zCHAR        cDBType,
                zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zSHORT       bAutoSeqOnly,
                zPCHAR       pchSqlCmd )
{
   LPDATARECORD   lpDataRecord;
   LPDATAFIELD    lpDataField;
   LPVIEWATTRIB   lpViewAttrib;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zBOOL          bFirstTime;
   zSHORT         nLongCount;
   zSHORT         nRC;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zUPDATE_CMD, pchSqlCmd );
   lpDataRecord = lpViewEntity->lpFirstDataRecord;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "UPDATE " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpDataRecord->lpRecordName );

   // First create "Set col = val [, ...]" clause.
   bFirstTime = TRUE;
   nLongCount = 0;
   lpDataField = lpDataRecord->lpFirstDataField;
   while ( lpDataField )
   {
      lpViewAttrib = lpDataField->lpViewAttrib;

      // If this update is for autosequence fields only and the attribute is
      // not an autoseq attribute, then do nothing for this attribute.
      if ( bAutoSeqOnly && lpViewAttrib->bAutoSeq == FALSE )
      {
         ;   // do nothing.
      }
      else
      // Update only persisten non-keys.
      if ( lpViewAttrib->bPersist && lpViewAttrib->bKey == FALSE )
      {
         if ( bFirstTime )
         {
            fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " SET " );
            bFirstTime = FALSE;
         }
         else
            fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " , " );

         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpDataField->lpFldName );
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " = " );

         // If the attribute is a blob or has a max length of 254, then the
         // the attribute is to be stored in a dynamic length column and
         // special processing must take place later on.  For this reason
         // we won't insert the attribute value but a place holder.
         if ( lpViewAttrib->cType == zTYPE_BLOB || lpViewAttrib->ulLth > 254 )
         {
            // Count the number of dynamic variables needed.
            nLongCount++;

            switch ( cDBType )
            {
               zCHAR sz[ 20 ];

               case zSQL_SQLBASE:
                  fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH ":" );
                  zltoa( nLongCount, sz );
                  fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH sz );
                  break;

               case zSQL_SQLSERVER:
                  fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "NULL" );
                  break;

            } // switch ( cDBType )...
         }
         else
            // Add attribute value to SQL command.
            SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );
      }

      lpDataField = lpDataField->lpNextDataField;
   } // while ( lpDataField )...

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " " );
   nRC = SqlBuildWhere( cDBType, lpView, lpViewOD, lpViewEntity, lpSql, zFULLY_QUALIFIED );
   if ( nRC == zCALL_ERROR )
      return( zCALL_ERROR );

   fnSqlAssembleSqlStatement( lpSql );

   // Return 1 if dynamic columns need to be bound, 0 if no dynamic columns.
   if ( nLongCount > 0 )
      return( 1 );
   else
      return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildUpdateCorrTable

      Updates the auto seq attribute in the corr table.

      RETURNS:
               0 - OK.

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildUpdateCorrTable( zCHAR        cDBType,
                         zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd )
{
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPRELRECORD  lpRelRecord;
   LPRELFIELD   lpRelField;
   LPVIEWATTRIB lpViewAttrib;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zBOOL        bFirstTime;

   // Since we can only update a corr table if it has auto seq attributes, exit if there
   // are no seq attributes for the entity.
   if ( lpViewEntity->bAutoSeq == FALSE )
      return( zCALL_ERROR );

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;
   if ( lpRelRecord == 0 )
      return( zCALL_ERROR );

   if ( lpRelRecord->cOwnerMember != zDBH_MANY_TO_MANY )
      return( 0 );

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zUPDATE_CMD | zCORR_CMD, pchSqlCmd );

   bFirstTime = TRUE;
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "UPDATE " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpRelRecord->lpRecordName );

   // First create "SET col = val" clause for AutoSeq attribute.  I know we already check for bAutoSeq,
   // but I'm leaving the IF statement here in case we need it in the future.
   if ( lpViewEntity->bAutoSeq )
   {
      // Find the Autoseq attribute.
      lpDataField = lpDataRecord->lpFirstDataField;
      lpViewAttrib = lpDataField->lpViewAttrib;
      while ( lpViewAttrib->bAutoSeq == FALSE )
      {
         lpDataField = lpDataField->lpNextDataField;
         lpViewAttrib = lpDataField->lpViewAttrib;
      }

      if ( bFirstTime )
      {
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " SET " );
         bFirstTime = FALSE;
      }
      else
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " , " );

      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpDataField->lpFldName );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " = " );
      SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );
   } // if ( lpViewEntity->bAutoSeq )...

   // Build WHERE clause.
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " WHERE ( " );
   lpRelField = lpRelRecord->lpFirstRelField;
   while ( lpRelField )
   {
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpFldName );
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " = " );

      SqlGetAttributeValue( cDBType, lpView, lpRelField->lpSrcDataField, lpSql );

      lpRelField = lpRelField->lpNextRelField;
      if ( lpRelRecord )
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " AND " );
   }

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " )" );

   fnSqlAssembleSqlStatement( lpSql );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildDelete

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildDelete( zCHAR        cDBType,
                zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                zPCHAR       pchSqlCmd )
{
   LPDATARECORD lpDataRecord;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zSHORT       nRC;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zDELETE_CMD, pchSqlCmd );

   lpDataRecord = lpViewEntity->lpFirstDataRecord;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "DELETE FROM " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpDataRecord->lpRecordName );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " " );

   nRC = SqlBuildWhere( cDBType, lpView, lpViewOD, lpViewEntity, lpSql, 0 );

   fnSqlAssembleSqlStatement( lpSql );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////

      SqlBuildDeleteCorrTable

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildDeleteCorrTable( zCHAR        cDBType,
                         zVIEW        lpView,
                         LPVIEWOD     lpViewOD,
                         LPVIEWENTITY lpViewEntity,
                         zPCHAR       pchSqlCmd )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   LPRELFIELD   lpRelField;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zBOOL        bFirstTime;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zDELETE_CMD | zCORR_CMD, pchSqlCmd );

   bFirstTime = TRUE;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "DELETE FROM " );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpRelRecord->lpRecordName );
   fnSqlIncrementComponentCount( lpSql );

   lpRelField = lpRelRecord->lpFirstRelField;
   while ( lpRelField )
   {
      if ( bFirstTime == FALSE )
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " AND " );
         fnSqlIncrementComponentCount( lpSql );
      }
      else
         bFirstTime = FALSE;

      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpFldName );
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " = " );

      SqlGetAttributeValue( cDBType, lpView, lpRelField->lpSrcDataField, lpSql );

      lpRelField = lpRelField->lpNextRelField;
   }

   fnSqlAssembleSqlStatement( lpSql );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

   SqlDataFieldIsInParentPath

 ??This routine does not appear to be used.

   RETURNS: TRUE  - if lpDataField is part of an entity that is in the parent path of lpViewEntity.
            FALSE - if lpDataField is not.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlDataFieldIsInParentPath( LPVIEWENTITY lpViewEntity,
                            LPDATAFIELD  lpDataField )
{
   LPVIEWENTITY lpDataFieldViewEntity;

   lpDataFieldViewEntity = lpDataField->lpViewAttrib->lpViewEntity;
   while ( lpViewEntity )
   {
      if ( lpViewEntity == lpDataFieldViewEntity )
         return( TRUE );

      lpViewEntity = lpViewEntity->lpParent;
   }

   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////

   SqlBuildForeignKeys

   Adds "col-name = attrib-value..." for any foreign key that is part of
   the lpViewEntity's parent path.

   Returns: 0 - Everything ok
            1 - Everything ok, but one of the foriegn key's value is null.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlBuildForeignKeys( zCHAR          cDBType,
                     zVIEW          lpView,
                     LPVIEWENTITY   lpViewEntity,
                     LPSQLSTATEMENT lpSql,
                     zSHORT         nControl )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   LPRELFIELD   lpRelField;
   zBOOL  bFirstTime;
   zSHORT nNull;
   zSHORT nRC;

   bFirstTime   = TRUE;
   nRC          = 0;
   nNull        = 0;
   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;
   lpRelField   = lpRelRecord->lpFirstRelField;

   fnSqlIncrementComponentCount( lpSql );

   // If the table name for lpViewEntity is not already part of the SELECT
   // statement, then add it to the FROM clause.
   if ( !SqlFindTableName( lpDataRecord->lpRecordName, lpSql ) )
      fnSqlAddStringToFrom( lpSql, lpDataRecord->lpRecordName );

   // If the relationship is many-to-many, then the correspondence table
   // needs to be added to the SELECT statement.
   if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      fnSqlAddStringToFrom( lpSql, lpRelRecord->lpRecordName );

   // Add the rel fields (for the view entity) to the WHERE clause.
   while ( lpRelField )
   {
      if ( bFirstTime )
      {
         bFirstTime = FALSE;
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " ( " );
      }
      else
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " AND " );

      if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      // There is a real question as to whether or not Siron will use
      // many-to-many relationships or will require an Associative entity
      // to create support this situation.
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelRecord->lpRecordName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpFldName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " = " );

         // ???
         // In m-m relationships, we need to qualify using the correspondence
         // table.  If the current rel field is stored in the correspondence,
         // then we compare the column from above (from the c-table) with the
         // the column in the child (non-correspondence) table.
         if ( lpRelField->lpSrcDataField->lpViewAttrib->lpViewEntity == lpViewEntity )
         {
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpDataRecord->lpRecordName );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpSrcDataField->lpFldName );
         }
         else
         if ( nControl == zROOT_ENTITY )
         {
            // ???
            // If we get here, then we are loading the root entity (the root
            // entity in this case is the entity that we are loading) but we
            // are doing qualification on a sub-entity that involves a m-m.
            nRC = SqlGetAttributeValue( cDBType, lpView, lpRelField->lpSrcDataField, lpSql );

            // If nRC == 1, then the attribute value is null, so set nNull to 1.
            if ( nRC == 1 )
               nNull = 1;
         }
         else
         {
            LPDATARECORD lpTempRecord;

            // ???
            // If we get here, then there is a sub-qualification on the entity that we are loading.

            lpTempRecord = lpRelField->lpSrcDataField->lpViewAttrib->lpViewEntity->lpFirstDataRecord;

            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpTempRecord->lpRecordName );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpSrcDataField->lpFldName );
         }
      } // if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      else
      if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_SOURCE )
      {
         LPVIEWENTITY lpRelViewEntity;
         LPDATARECORD lpRelDataRecord;

         lpRelViewEntity = lpRelField->lpRelDataField->lpViewAttrib->
                              lpViewEntity;
         lpRelDataRecord = lpRelViewEntity->lpFirstDataRecord;

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelDataRecord->lpRecordName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpRelDataField->lpFldName );

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " = " );

         if ( nControl == zSUB_ENTITY )  // What is subentity????
         {
            LPDATARECORD lpForeignDataRecord;

            // If we get here, then there is a sub-qualification on the entity that we are loading.
            lpForeignDataRecord = lpRelField->lpSrcDataField->lpViewAttrib->lpViewEntity->lpFirstDataRecord;
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpForeignDataRecord->lpRecordName );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpSrcDataField->lpFldName );

         }
         else
         {
            nRC = SqlGetAttributeValue( cDBType, lpView, lpRelField->lpSrcDataField, lpSql );

            // If nRC == 1, then the attribute value is null, so set nNull
            // to 1.
            if ( nRC == 1 )
               nNull = 1;
         }
      } // if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_SOURCE )...
      else
      if ( lpRelRecord->cOwnerMember == zDBH_CHILD_IS_SOURCE )
      {
         LPVIEWENTITY lpSrcViewEntity;
         LPDATARECORD lpSrcDataRecord;

         lpSrcViewEntity = lpRelField->lpSrcDataField->lpViewAttrib->lpViewEntity;
         lpSrcDataRecord = lpSrcViewEntity->lpFirstDataRecord;

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpSrcDataRecord->lpRecordName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpSrcDataField->lpFldName );

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " = " );

         if ( nControl == zSUB_ENTITY )
         {
            LPDATARECORD lpForeignDataRecord;

            lpForeignDataRecord = lpRelField->lpRelDataField->lpViewAttrib->lpViewEntity->lpFirstDataRecord;
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpForeignDataRecord->lpRecordName );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpRelField->lpRelDataField->lpFldName );
         }
         else
         {
            nRC = SqlGetAttributeValue( cDBType, lpView, lpRelField->lpRelDataField, lpSql );

            // If nRC == 1, then the attribute value is null, so set nNull
            // to 1.
            if ( nRC == 1 )
               nNull = 1;
         }
      } // if ( lpRelRecord->cOwnerMember == zDBH_CHILD_IS_SOURCE )...

      lpRelField = lpRelField->lpNextRelField;
   } // while ( lpRelField )...

   fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " )" );

   return( nNull );
}

/////////////////////////////////////////////////////////////////////////////

   SqlAddTableToSelect

   A join is being used in a SELECT statement.  Add the table name of
   lpViewEntity to the SQL command and add any foreign keys to the WHERE clause.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlAddTableToSelect( zCHAR          cDBType,
                     zVIEW          lpView,
                     LPVIEWENTITY   lpViewEntity,
                     LPSQLSTATEMENT lpSql )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;

   SqlBuildForeignKeys( cDBType, lpView, lpViewEntity, lpSql, zSUB_ENTITY );

   // Now make sure that the parent of lpViewEntity is included in the in the select statement.
   if ( lpViewEntity->lpParent )
   {
      lpViewEntity = lpViewEntity->lpParent;
      lpDataRecord = lpViewEntity->lpFirstDataRecord;
      if ( !SqlFindTableName( lpDataRecord->lpRecordName, lpSql ) )
      {
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " AND " );
         SqlAddTableToSelect( cDBType, lpView, lpViewEntity, lpSql );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

   SqlBuildSubselect

   What is a subselect????

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlBuildSubselect( zCHAR          cDBType,
                   zVIEW          lpView,
                   LPQUALENTITY   lpQualEntity,
                   LPQUALATTRIB   lpQualAttrib,
                   LPSQLSTATEMENT lpParentSql )
{
   zCHAR        szSelect[ 100 ];
   LPVIEWENTITY lpViewEntity;
   LPDATARECORD lpDataRecord;
   SqlStatementRecord SubselectSql;
   LPSQLSTATEMENT     lpSql;

   lpViewEntity = lpQualAttrib->lpViewEntity;
   lpDataRecord = lpQualAttrib->lpDataRecord;

   lpSql = &SubselectSql;
   fnSqlInitSqlStatement( lpSql, zSUBSELECT_CMD, szSelect );

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpQualAttrib->szOper );
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH " ( SELECT " );
   SqlBuildColumnList( cDBType, lpViewEntity, lpSql, zKEYS_ONLY );

   // Add the QualEntity table to szFromTable so that SqlAddTableToSelect doesn't try to add it later.
   fnSqlAddTableToFrom( lpSql, lpQualEntity->lpViewEntity->lpFirstDataRecord->lpRecordName );

   // Add table representing lpViewEntity to select statement.
   SqlAddTableToSelect( cDBType, lpView, lpViewEntity, lpSql );

   // If Subselect is further qualified, add qualification to WHERE clause.
   if ( lpQualAttrib->lpFirstSubQualAttrib )
   {
      SqlAddQualToWhere( cDBType, lpView, lpQualEntity, lpQualAttrib->lpFirstSubQualAttrib, lpSql );
   }

   if ( fnSqlWhereIsNotEmpty( lpSql ) )
   {
      fnSqlAddStringToWhere( lpParentSql, sizeof( lpParentSql ), lpSql->szSqlCmd );
      fnSqlAddStringToWhere( lpParentSql, sizeof( lpParentSql ), " FROM " );
      fnSqlAddStringToWhere( lpParentSql, sizeof( lpParentSql ), lpSql->szFrom );
      fnSqlAddStringToWhere( lpParentSql, sizeof( lpParentSql ), " WHERE " );
      if ( lpSql->nComponentCount > 1 )
         fnSqlAddStringToWhere( lpParentSql, sizeof( lpParentSql ), " ( " );

      fnSqlAddStringToWhere( lpParentSql, sizeof( lpParentSql ), lpSql->szWhere );
      if ( lpSql->nComponentCount > 1 )
         fnSqlAddStringToWhere( lpParentSql, sizeof( lpParentSql ), " ) " );

      fnSqlAddStringToWhere( lpParentSql, sizeof( lpParentSql ), " )" );
   }

   fnSqlFreeSqlStatement( lpSql );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

   SqlBuildIS_Clause

   IS clause is short hand for comparing the keys of one entity to the keys
   of another.  Go through all the data fields (looking for keys) and compare
   them in the where clause.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlBuildIS_Clause( zCHAR          cDBType,
                   LPQUALENTITY   lpQualEntity,
                   LPQUALATTRIB   lpQualAttrib,
                   LPSQLSTATEMENT lpSql )
{
   zVIEW        lpView;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   zSHORT       nKeyCnt;

   nKeyCnt      = 0;
   lpView       = lpQualAttrib->lpSourceView;
   lpViewEntity = lpQualAttrib->lpSourceViewEntity;
   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpDataField  = lpDataRecord->lpFirstDataField;

   while ( lpDataField )
   {
      lpViewAttrib = lpDataField->lpViewAttrib;
      if ( lpViewAttrib->bKey )
      {
         if ( nKeyCnt > 0 )
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " AND " );

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpDataRecord->lpRecordName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpDataField->lpFldName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " = " );
         SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );

         nKeyCnt++;
      }

      lpDataField = lpDataField->lpNextDataField;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

   SqlAddQualToWhere

   Note that lpFirstQualAttrib is not necessarily the same as lpQualEntity->lpFirstQualAttrib.
   It could be the first qual attrib for it's parent qual attrib.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlAddQualToWhere( zCHAR          cDBType,
                   zVIEW          lpView,
                   LPQUALENTITY   lpQualEntity,
                   LPQUALATTRIB   lpFirstQualAttrib,
                   LPSQLSTATEMENT lpSql )
{
   LPQUALATTRIB lpQualAttrib;
   zCHAR        szStr[ 500 ];
   zSHORT       nRC;

   // Qualification stuff are components, so increment count.
   fnSqlIncrementComponentCount( lpSql );

   // Go through each of the QualAttrib's looking for tables that are not already part of the select.
   lpQualAttrib = lpFirstQualAttrib;
   while ( lpQualAttrib )
   {
      // Add the table to the SELECT statement only if the QualAttrib has a lpViewEntity and a lpViewAttrib.
      if ( lpQualAttrib->lpViewEntity && lpQualAttrib->lpViewAttrib )
      {
         // If lpSearchRecord->lpRecordName can't be found in the FromTable,
         // then it needs to be added to the SELECT statement.
         if ( SqlFindTableName( lpQualAttrib->lpDataRecord->lpRecordName, lpSql ) == FALSE )
         {
            if ( fnSqlWhereIsNotEmpty( lpSql ) )
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " AND " );

            // Foreign keys are a component, so increment count.
            fnSqlIncrementComponentCount( lpSql );

            SqlAddTableToSelect( cDBType, lpView, lpQualAttrib->lpViewEntity, lpSql );
         }
      } // if ( lpQualAttrib->lpViewEntity )...

      lpQualAttrib = lpQualAttrib->lpNextQualAttrib;
   } // while ( lpQualAttrib )...

   if ( fnSqlWhereIsNotEmpty( lpSql ) )
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " AND ( " );
   else
      fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "( " );

   //===
   //===  At this point, all tables that are needed in the select have
   //===  been included in the SELECT.  All that remains to do is to
   //===  add the QualAttrib expressions to the SELECT statement.
   //===

   lpQualAttrib = lpFirstQualAttrib;
   while ( lpQualAttrib )
   {
      if ( lpQualAttrib->lpViewEntity == 0 )
      {
         // If there is no entity name for QualAttrib, then the QualAttrib
         // has just an Oper.  Tack it on to the end of the WHERE clause.

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " " );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpQualAttrib->szOper );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " " );
      } // if ( lpQualAttrib->szEntityName[ 0 ] == 0 )...
      else
      {
         // QualAttrib has an entity name, so this is an expression.  Add the expression to the WHERE clause.

         // Check to see if szOper matches certain commands.  If it does, then the oper preceeds a subselect.
         if ( zstrcmpi( lpQualAttrib->szOper, "EXISTS" ) == 0 ||
              zstrcmpi( lpQualAttrib->szOper, "NOT EXISTS" ) == 0 )
         {
            nRC = SqlBuildSubselect( cDBType, lpView, lpQualEntity, lpQualAttrib, lpSql );
            fnSqlAddTableToFrom( lpSql, lpQualAttrib->lpDataRecord->lpRecordName );
            if ( nRC < 0 )
               return( zCALL_ERROR );
         }
         else
         if ( zstrcmpi( lpQualAttrib->szOper, "IS" ) == 0 )
         {
            nRC = SqlBuildIS_Clause( cDBType, lpQualEntity, lpQualAttrib, lpSql );
            if ( nRC < 0 )
               return( zCALL_ERROR );
         }
         else
         {
            // Oper is a simple compare.  Add statement to WHERE clause.
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpQualAttrib->lpDataRecord->lpRecordName );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH "." );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpQualAttrib->lpDataField->lpFldName );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " " );
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpQualAttrib->szOper );

            if ( lpQualAttrib->szValue )
            {
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " " );
     ????      SqlConvertStringToSqlString( cDBType, lpQualAttrib->lpDataField, lpQualAttrib->szValue, szStr );
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH szStr );
            }
            else
            if ( lpQualAttrib->lpSourceDataField )
            {
               fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " " );
               SqlGetAttributeValue( cDBType, lpView, lpQualAttrib->lpSourceDataField, lpSql );
            }
         }
      } // if ( lpQualAttrib->lpViewEntity )...else...

      lpQualAttrib = lpQualAttrib->lpNextQualAttrib;
   } // while ( lpQualAttrib )...

   fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " )" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

   SqlBuildSelect

   Builds a SELECT statment to locate all rows for lpViewEntity that
   are children of the parent entity instance.  This is done in the
   following steps.

   1.  The columns to be accessed are added to the SELECT.

   2.  The WHERE components for foreign keys are built, if needed.

   3.  The WHERE components for Activate qualification are built, if
       needed.

   Returns:  0 - everything ok
             1 - everything ok, but there is at least foriegn key that
                 has a null value.

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
SqlBuildSelect( zCHAR        cDBType,
                zVIEW        lpView,
                LPVIEWOD     lpViewOD,
                LPVIEWENTITY lpViewEntity,
                LPQUALENTITY lpFirstQualEntity,
                zPCHAR       pchSqlCmd )
{
   LPDATARECORD   lpDataRecord;
   LPVIEWATTRIB   lpViewAttrib;
   LPDATAFIELD    lpDataField;
   LPRELRECORD    lpRelRecord;
   LPQUALENTITY   lpQualEntity;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zSHORT         nNull;
   zSHORT         nRC;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zSELECT_CMD, pchSqlCmd );
   nNull = 0;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpDataField  = lpDataRecord->lpFirstDataField;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;

   // Build the list of all columns that are to be accessed.  Does Siron return the whole
   // record for a userview, or do we access individual fields???
   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "SELECT " );
   nRC = SqlBuildColumnList( cDBType, lpViewEntity, lpSql, zFULLY_QUALIFIED );

   fnSqlAddStringToFrom( lpSql, lpDataRecord->lpRecordName );

   //===
   //=== Add parent foreign keys to WHERE clause.
   //===

   // If the table we are about to select has a parent table, then we need toadd to the WHERE
   //  clause any foreign keys that logically connect the table with its parent.
   if ( lpViewEntity->lpParent )
   {
      // Add foreign keys to WHERE.
      nNull = SqlBuildForeignKeys( cDBType, lpView, lpViewEntity, lpSql, zROOT_ENTITY );
   } // if ( lpViewEntity->lpParent )...

   // Check to see if table is qualified by looking in the qualification
   // table for a matching View entity.
   lpQualEntity = lpFirstQualEntity;
   while ( lpQualEntity && lpQualEntity->lpViewEntity != lpViewEntity )
      lpQualEntity = lpQualEntity->lpNextQualEntity;
   if ( lpQualEntity )
   {
      //===
      //===  Table is qualified.  Add qualification to WHERE clause.
      //===

      SqlAddQualToWhere( cDBType, lpView, lpQualEntity, lpQualEntity->lpFirstQualAttrib, lpSql );
   } // if ( lpQualEntity )...

   // Assemble the SQL statement from all the parts.
   fnSqlAssembleSqlStatement( lpSql );

   return( nNull );
}

/////////////////////////////////////////////////////////////////////////////

   SqlBuildSelectForEntityInstance

 ??This appears to only be used for sqlserver.  Can it be deleted?

   Builds a SELECT statment to locate the source table for a particular
   entity.  DOES NOT set up foriegn keys.

   Will only select the columns that are part of szColumnList.  If
   szColumnList is 0 or szColumnList[ 0 ] is 0, then all columns are
   selected.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlBuildSelectForEntityInstance( zCHAR        cDBType,
                                 zVIEW        lpView,
                                 LPVIEWOD     lpViewOD,
                                 LPVIEWENTITY lpViewEntity,
                                 zPCHAR       pchColumnList,
                                 zPCHAR       pchSqlCmd )
{
   LPVIEWATTRIB lpViewAttrib;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPSQLSTATEMENT lpSql;
   SqlStatementRecord SqlStatement;
   zBOOL        bFirstKey;
   zBOOL        bFirstColumn;
   zBOOL        bPartialColumnList;
   zPCHAR       pchToken;
   zUSHORT      uLth;

   lpSql = &SqlStatement;
   fnSqlInitSqlStatement( lpSql, zSELECT_CMD, pchSqlCmd );

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpDataField  = lpDataRecord->lpFirstDataField;
   bPartialColumnList = (pchColumnList && *pchColumnList);

   fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH "SELECT " );
   fnSqlAddStringToFrom( lpSql, lpDataRecord->lpRecordName );

   // Go through all the datafields.  If a key is found, add it to the WHERE
   // clause.  Also conditionally add the column name to the SELECT clause.
   bFirstColumn = TRUE;
   bFirstKey    = TRUE;
   for (; lpDataField; lpDataField = lpDataField->lpNextDataField )
   {
      lpViewAttrib = lpDataField->lpViewAttrib;

      // If attribute is a key, then add it to the WHERE clause.
      if ( lpViewAttrib->bKey )
      {
         if ( bFirstKey == FALSE )
            fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " AND " );
         else
            bFirstKey = FALSE;

         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH lpDataField->lpFldName );
         fnSqlAddStringToWhere( lpSql, MAX_SQLCMD_LENGTH " = " );
         SqlGetAttributeValue( cDBType, lpView, lpDataField, lpSql );
      }

      // If only a partial list is wanted, then check to make sure that the
      // current data field should belong to the SELECT statement.
      if ( bPartialColumnList )
      {
         uLth    = zstrlen( lpDataField->lpFldName );
         pchToken = pchColumnList;
         while ( *pchToken &&
                 zstrncmpi( pchToken, lpDataField->lpFldName, uLth ) != 0 )
         {
            // Set pchToken to point to end of current token.
            while ( *pchToken && *pchToken != ',' && *pchToken != ' ' )
               pchToken++;

            // Edit out delimiters.
            while ( *pchToken && ( *pchToken == ',' || *pchToken == ' ' ) )
               pchToken++;
         }

         // If *pchToken is 0, then the column was not found and shouldn't be
         // included in the SELECT clause, so continue "for" loop.
         if ( pchToken[ 0 ] == 0 )
            continue;
      }

      if ( bFirstColumn == FALSE )
         fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH ", " );
      else
         bFirstColumn = FALSE;

      // Add DataField to column list.
      fnSqlAddStringToSql( lpSql, MAX_SQLCMD_LENGTH lpDataField->lpFldName );
   } // for (; lpDataField; ... )...

   fnSqlAssembleSqlStatement( lpSql );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

   SqlDisplayQualAttrib

   Used for debugging purposes.

/////////////////////////////////////////////////////////////////////////////
void
SqlDisplayQualAttrib( LPQUALATTRIB lpQualAttrib,
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
         strcat_s( szStr, "Entity name: " );
         TraceLineS( szStr, lpQualAttrib->lpViewEntity->lpName );
         szStr[ n ] = 0;
      }

      if ( lpQualAttrib->lpViewAttrib )
      {
         strcat_s( szStr, "Attrib name: " );
         TraceLineS( szStr, lpQualAttrib->lpViewAttrib->lpName );
         szStr[ n ] = 0;
      }

      strcat_s( szStr, "Oper      : " );
      TraceLineS( szStr, lpQualAttrib->szOper );
      szStr[ n ] = 0;
      if ( lpQualAttrib->szValue )
      {
         strcat_s( szStr, "Value     : " );
         TraceLineS( szStr, lpQualAttrib->szValue );
         szStr[ n ] = 0;
      }

      if ( lpQualAttrib->lpSourceViewEntity )
      {
         strcat_s( szStr, "Source Entity name: " );
         TraceLineS( szStr, lpQualAttrib->lpSourceViewEntity->lpName );
         szStr[ n ] = 0;
      }

      if ( lpQualAttrib->lpSourceDataField )
      {
         strcat_s( szStr, "Source Attrib name: " );
         TraceLineS( szStr, lpQualAttrib->lpSourceDataField->lpViewAttrib->lpName );
         szStr[ n ] = 0;
      }

      if ( lpQualAttrib->lpSourceView )
      {
         TraceLineS( szStr, "SourceView is defined" );
         szStr[ n ] = 0;
      }

      SqlDisplayQualAttrib( lpQualAttrib->lpFirstSubQualAttrib, nIndent );
      lpQualAttrib = lpQualAttrib->lpNextQualAttrib;
   }
}

/////////////////////////////////////////////////////////////////////////////

   SqlDisplayQualEntity

   Used for debugging purposes.

/////////////////////////////////////////////////////////////////////////////
void
SqlDisplayQualEntity( LPQUALENTITY lpQualEntity )
{
   TraceLineS( "************************************", "" );
   TraceLineS( "Display QualEntity structure", "" );
   while ( lpQualEntity )
   {
      TraceLineS( "", "" );
      TraceLineS( "--- QualEntity ---", "" );
      TraceLineS( "  Entity Name:", lpQualEntity->lpViewEntity->lpName );
      if ( lpQualEntity->bQualUsesChildEntity )
         TraceLineS( "  bQualUsesChildEntity: TRUE", "" );
      else
         TraceLineS( "  bQualUsesChildEntity: FALSE", "" );

      SqlDisplayQualAttrib( lpQualEntity->lpFirstQualAttrib, 0 );

      lpQualEntity = lpQualEntity->lpNextQualEntity;
   }

   TraceLineS( "************************************", "" );
}

/////////////////////////////////////////////////////////////////////////////

   SqlFreeQualAttrib

/////////////////////////////////////////////////////////////////////////////
void
SqlFreeQualAttrib( LPQUALATTRIB lpFirstQualAttrib )
{
   LPQUALATTRIB lpQualAttrib;

   while ( lpFirstQualAttrib )
   {
      if ( lpFirstQualAttrib )
         SqlFreeQualAttrib( lpFirstQualAttrib->lpFirstSubQualAttrib );

      lpQualAttrib = lpFirstQualAttrib;
      lpFirstQualAttrib = lpFirstQualAttrib->lpNextQualAttrib;

      if ( lpQualAttrib->hOperMem )
         SysFreeMemory( lpQualAttrib->hOperMem );

      if ( lpQualAttrib->hValueMem )
         SysFreeMemory( lpQualAttrib->hValueMem );

      SysFreeMemory( lpQualAttrib->hMem );
   }
}

/////////////////////////////////////////////////////////////////////////////

   SqlFreeQualEntity

/////////////////////////////////////////////////////////////////////////////
void
SqlFreeQualEntity( LPQUALENTITY *lpFirstQualEntity )
{
   LPQUALENTITY lpQualEntity;

   while ( *lpFirstQualEntity )
   {
      lpQualEntity = *lpFirstQualEntity;
      *lpFirstQualEntity = (*lpFirstQualEntity)->lpNextQualEntity;

      SqlFreeQualAttrib( lpQualEntity->lpFirstQualAttrib );
      SysFreeMemory( lpQualEntity->hMem );
   }
}

/////////////////////////////////////////////////////////////////////////////

   SqlDisplayQualMsg

/////////////////////////////////////////////////////////////////////////////
void
SqlDisplayQualMsg( zVIEW  lpView,
                   zPCHAR pchMsg,
                   zSHORT nErrorOrWarning,
                   zPCHAR pchQualEntity )
{
   zPCHAR szTitle;
   zBOOL  bBeep;
   zSHORT nIcon;
   zCHAR  szText[ 500 ];

   if ( nErrorOrWarning == zWARNING )
   {
      szTitle = "Qualification Object Warning";
      bBeep   = 0;
      nIcon   = zICON_EXCLAMATION;
      strcpy_s( szText, sizeof( szText ), "Warning loading qualification object for Activate:\n" );
   }
   else
   {
      szTitle = "Qualification Object Error";
      bBeep   = TRUE;
      nIcon   = zICON_ERROR;
      strcpy_s( szText, sizeof( szText ), "Error loading qualification object for Activate:\n\n" );
   }

   zstrcatp( szText, pchMsg );
   OperatorPrompt( lpView, szTitle, szText, bBeep, zBUTTONS_OK, 0, nIcon );

   TraceLineS( szText, "" );

   if ( pchQualEntity && pchQualEntity[ 0 ] )
      DisplayEntityInstancePath( lpView, pchQualEntity );
}

/////////////////////////////////////////////////////////////////////////////

   fnSqlVerifyDataBase

   Verify that all the QualAttribs point to the same database.

   RETURNS: TRUE  - All QualAttribs point to the same database.
            FALSE - They don't.

/////////////////////////////////////////////////////////////////////////////
zSHORT
fnSqlVerifyDataBase( zCHAR        cDBType,
                     LPQUALATTRIB lpFirstQualAttrib,
                     zPCHAR       pchDBName,
                     zBOOL        bDBNameIsDefault )
{
   LPVIEWENTITY lpViewEntity;
   LPQUALATTRIB lpQualAttrib;

   for ( lpQualAttrib = lpFirstQualAttrib;
         lpQualAttrib;
         lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
   {
      // If the current lpQualAttrib has a subQualAttrib, then make sure that all the sub-quals are OK.
      if ( lpQualAttrib->lpFirstSubQualAttrib &&
          !fnSqlVerifyDataBase( cDBType, lpQualAttrib->lpFirstSubQualAttrib, pchDBName, bDBNameIsDefault ) )
            return( FALSE );

      lpViewEntity = lpQualAttrib->lpViewEntity;

      // If there is no lpViewEntity, don't bother doing check.
      if ( lpViewEntity == 0 )
         continue;

      // If there is no DBName for lpViewEntity then the database name for the entity is the default. In this case, check...
      if ( lpViewEntity->lpDBName == 0 )
      {
         // ...to make sure that we are using the default DBName.  If...
         if ( bDBNameIsDefault )
            continue;           // ...we are, then continue, otherwise...
         else
            return( FALSE );    // ...return FALSE
      }

      // If DBNames aren't the same, then return FALSE.
      if ( zstrcmpi( pchDBName, lpViewEntity->lpDBName ) != 0 )
         return( FALSE );
   } // for ( lpQualAttrib... )...

   // If we got this far, then all the DBNames match up OK.
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////

   SqlRetrieveQualAttrib

 ??Why this and SqlVerifyDataBase?

   Retrieve all the QualAttribs under the current EntitySpec entity in the qualification view.

   Note: We need to use lpFirstQualAttrib because this function will be called recursively if there are any SubQualAttribs.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlRetrieveQualAttrib( zCHAR        cDBType,
                       zVIEW        lpView,
                       LPVIEWOD     lpViewOD,
                       zVIEW        lpQualView,
                       LPQUALENTITY lpQualEntity,
                       LPQUALATTRIB *lpFirstQualAttrib )
{
   zSHORT       nRC;
   zSHORT       nParenCount;
   zUSHORT      uLth;
   zLONG        hMem;
   zPCHAR       szAttributeName, szAttEntityName;
   zPCHAR       szOper, pchValue, pchSrcAttributeName;
   zPCHAR       pchSrcEntityName, szPtr;
   zPCHAR       pchSrcViewName;
   zCHAR        szStr[ 300 ];
   zVIEW        lpSourceView;
   LPQUALATTRIB lpQualAttrib;
   LPVIEWENTITY lpViewEntity;
   LPVIEWENTITY lpSourceViewEntity;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;

   // Go through each of the QualAttrib entities for EntitySpec and copy attribute data to QualAttrib records.
   //
   // Note that we process the QualAttrib entities by going BACKWARDS, starting with the last entity.  This is done
   // so that adding the lpQualAttribs to the chain is simple--we add each one to the beginning of the chain.

   // ===
   // === Retrieve each of the attributes in QualAttrib and copy them to the record lpQualAttrib.
   // ===
   nParenCount = 0;
   nRC = SetCursorLastEntity( lpQualView, szlQualAttrib, "" );
   while ( nRC == zCURSOR_SET )
   {
      // Allocate space for new QualAttrib record.
      hMem = SysAllocMemory( (zPVOID) &lpQualAttrib, sizeof( QualAttribRecord ), 0, zCOREMEM_ALLOC, 0 );
      if ( lpQualAttrib == 0 )
      {
         SqlDisplayQualMsg( lpQualView, "Cannot allocate memory",
                            zERROR, 0 );
         return( zCALL_ERROR );
      }

      // Fill new QualAttrib with zeros.
      zmemset( (zPVOID) lpQualAttrib, sizeof( QualAttribRecord ), 0 );
      lpQualAttrib->hMem = hMem;

      // Add lpQualAttrib as first qual attrib in chain.
      lpQualAttrib->lpNextQualAttrib = *lpFirstQualAttrib;
      *lpFirstQualAttrib = lpQualAttrib;

      // ===
      // ===  QualAttrib.Oper
      // ===

      // All Qual attribs must have an Oper.
      GetAddrForAttribute( &szOper, lpQualView, szlQualAttrib, "Oper" );
      if ( szOper[ 0 ] == 0 )
      {
         SqlDisplayQualMsg( lpQualView,
                            "QualAttrib does not have an operation defined. Oper is a required attribute.",
                            zERROR, szlQualAttrib );
         return( zCALL_ERROR );
      }

      // Oper was found--go through and count parens.
      szPtr = szOper;
      while ( szPtr[ 0 ] )
      {
         if ( szPtr[ 0 ] == '(' )
            nParenCount++;
         else
         if ( szPtr[ 0 ] == ')' )
            nParenCount--;

         szPtr++;
      }

      uLth = zstrlen( szOper ) + 1;
      hMem = SysAllocMemory( (zPVOID) &lpQualAttrib->szOper, uLth, 0, zCOREMEM_ALLOC, 0 );
      if ( lpQualAttrib->szOper == 0 )
      {
         SqlDisplayQualMsg( lpQualView, "Cannot allocate memory.", zERROR, 0 );
         return( zCALL_ERROR );
      }

      lpQualAttrib->hOperMem = hMem;
      strcpy_s( lpQualAttrib->szOper, szOper );

      // ===
      // ===  QualAttrib.EntityName
      // ===
      GetAddrForAttribute( &szAttEntityName, lpQualView, szlQualAttrib, "EntityName" );
      if ( szAttEntityName[ 0 ] )
      {
         // If the entity name for the attribute is not the same as the entity
         // name of the qual entity, then the qualification uses child entities.
         if ( zstrcmp( lpQualEntity->lpViewEntity->lpName, szAttEntityName ) != 0 )
            lpQualEntity->bQualUsesChildEntity = TRUE;

         // Find the view entity that matches entity name.
         lpViewEntity = MiGetViewEntityForView( lpView, szAttEntityName );

         // If no view entity was found to match entity name in qual entity, then issue an error and exit.
         if ( lpViewEntity == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "Attribute AttributeName for entity QualAttrib not found in target object definition.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         lpDataRecord = lpViewEntity->lpFirstDataRecord;
         lpQualAttrib->lpViewEntity = lpViewEntity;
         lpQualAttrib->lpDataRecord = lpDataRecord;
      } // if ( szAttEntityName[ 0 ] )...

      // ===
      // ===  QualAttrib.AttributeName
      // ===
      GetAddrForAttribute( &szAttributeName, lpQualView, szlQualAttrib, "AttributeName" );
      if ( szAttributeName[ 0 ] )
      {
         // If QualAttrib entity has an AttributeName, then QualAttrib must also have a valid EntityName.
         if ( lpQualAttrib->lpViewEntity == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib entity has AttributeName but doesn't have EntityName.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Find the lpViewAttrib and DataField that matches AttribName.
         lpDataField = lpQualAttrib->lpDataRecord->lpFirstDataField;
         while ( lpDataField &&
                 zstrcmp( lpDataField->lpViewAttrib->lpName,
                          szAttributeName ) != 0 )
            lpDataField = lpDataField->lpNextDataField;

         // If no view entity was found to match entity name in qual entity, then issue an error and exit.
         if ( lpDataField == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "AttributeName in QualAttrib not found in target object definition.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         lpQualAttrib->lpDataField  = lpDataField;
         lpQualAttrib->lpViewAttrib = lpDataField->lpViewAttrib;
         if ( pDataField->lpViewAttrib->bKey == FALSE )
            lpQualEntity->bKeysOnly = FALSE;

      } // if ( szAttributeName[ 0 ] )...

      // ===
      // ===  QualAttrib.Value
      // ===

      // The value, which was stored in the Qual Object is retrieved here.
      GetAddrForAttribute( &pchValue, lpQualView, szlQualAttrib, "Value" );
      if ( pchValue[ 0 ] )
      {
         // If Value is defined, then QualAttrib must also have AttribName.
         if ( lpQualAttrib->lpDataField == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib entity has Value but doesn't have AttributeName.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If Attribute is a date/time, the value coming in must be in
         // the format YYYYMMDDHHMMSSTTT.  Sometimes, however, when one
         // of the units (e.g. HH) is a single digit, it comes in as a
         // blank and a single digit, rather than a 0 and a single
         // digit.  The following routine is called to replace the
         // blanks with zeros.
         if ( lpQualAttrib->lpDataField->cFldType == zTYPE_DATETIME )
         {
            SqlUnformatDateTimeString( szStr, pchValue );

            // Set pchValue to point to unformatted string.
            pchValue = szStr;
         }

         uLth = zstrlen( pchValue ) + 1;
         hMem = SysAllocMemory( (zPVOID) &lpQualAttrib->szValue, uLth, 0, zCOREMEM_ALLOC, 0 );
         if ( lpQualAttrib->szValue == 0 )
         {
            SqlDisplayQualMsg( lpQualView, "Cannot allocate memory.", zERROR, 0 );
            return( zCALL_ERROR );
         }

         lpQualAttrib->hValueMem = hMem;
         strcpy_s( lpQualAttrib->szValue, pchValue );
      } // if ( *pchValue )...

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
            SetAttributeFromInteger( lpQualView, szlQualAttrib, "SourceViewID", (zLONG) lpSourceView );
         }
         else
         {
            SqlDisplayQualMsg( lpQualView,
                               "SourceViewName not found at TASK level.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }
      }
      else
      {
         // SourceViewName was not specified--see if SourceViewID was.
         GetIntegerFromAttribute( (zPLONG) &lpSourceView, lpQualView, szlQualAttrib, "SourceViewID" );
         if ( lpSourceView )
            lpQualAttrib->lpSourceView = lpSourceView;
      }

      // ===
      // ===  QualAttrib.SourceEntityName
      // ===

      GetAddrForAttribute( &pchSrcEntityName, lpQualView, szlQualAttrib, "SourceEntityName" );
      lpSourceViewEntity = 0;
      if ( *pchSrcEntityName )
      {
         // Find the lpViewEntity for source attrib entity.
         if ( lpQualAttrib->lpSourceView )
            lpSourceViewEntity = MiGetViewEntityForView( lpQualAttrib->lpSourceView, pchSrcEntityName );
         else
            lpSourceViewEntity = MiGetViewEntityForView( lpView, pchSrcEntityName );

         // If no view entity was found to match the entity name, then issue an error and exit.
         if ( lpSourceViewEntity == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "Attribute SourceEntityName not found in target object defintion.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If lpSourceView is defined, then make sure that the SourceViewEntity entity is defined.
         if ( lpQualAttrib->lpSourceView )
         {
            nRC = CheckExistenceOfEntity( lpQualAttrib->lpSourceView, lpSourceViewEntity->lpName );
            if ( nRC != zCURSOR_SET )
            {
               SqlDisplayQualMsg( lpQualView,
                                  "Cursor is not set properly in the Source View specified in QualAttrib.",
                                  zERROR, szlQualAttrib );
               return( zCALL_ERROR );
            }
         }

         // lpSourceView is not defined--the Source View Entity is the view we will activate later.
         // Make sure that lpSourceViewEntity comes before entity spec in hierarchical order.
         else
         if ( lpSourceViewEntity->nHierNbr >= lpQualEntity->lpViewEntity->nHierNbr )
         {
            SqlDisplayQualMsg( lpQualView,
                               "SourceEntityName does not have hierarchical precedence over EntityName in EntitySpec.",
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
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib has SourceAttributeName but "
                               "doesn't have SourceEntityName.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Find the lpViewAttrib and DataField that matches SourceAttributeName.
         lpDataRecord = lpSourceViewEntity->lpFirstDataRecord;
         lpDataField  = lpDataRecord->lpFirstDataField;
         while ( lpDataField &&
                 zstrcmp( lpDataField->lpViewAttrib->lpName,
                          pchSrcAttributeName ) != 0 )
            lpDataField = lpDataField->lpNextDataField;

         // If no data field was found to match source attribute name in target entity then issue an error and exit.
         if ( lpDataField == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "SourceAttributeName in QualAttrib not found in target object definition.",
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
      // === Make sure that all attributes have been set that are needed for each command.
      // ===
      if ( zstrcmpi( lpQualAttrib->szOper, "EXISTS" ) == 0 ||
           zstrcmpi( lpQualAttrib->szOper, "NOT EXISTS" ) == 0 )
      {
         // Exists commands must have entity name specified.
         if ( lpQualAttrib->lpViewEntity == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib must specify EntityName for "
                               "[NOT] EXISTS operation.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Check to make sure that the entity in QualEntity is a parent of lpQualAttrib->lpViewEntity.
         lpViewEntity = lpQualAttrib->lpViewEntity->lpParent;
         while ( lpViewEntity && lpViewEntity != lpQualEntity->lpViewEntity )
            lpViewEntity = lpViewEntity->lpParent;

         if ( lpViewEntity == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib.EntityName must be a direct descendant of EntitySpec.EntityName for "
                               "the [NOT] EXISTS operation.",
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
            SqlDisplayQualMsg( lpQualView,
                               "Extraneous attributes specified for [NOT] EXISTS operation.  Only Oper and EntityName "
                               "are needed.  All others will be ignored.",
                               zWARNING, szlQualAttrib );
         }
      } // if ( zstrcmpi( lpQualAttrib->szOper, "EXISTS" ) == 0 || ... )...
      else
      if ( zstrcmpi( lpQualAttrib->szOper, "IS" ) == 0 )
      {
         // IS command must have entity name specified.
         if ( lpQualAttrib->lpViewEntity == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib must have EntityName for IS operation.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // IS command must have source entity name specified.
         if ( lpQualAttrib->lpSourceViewEntity == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib must have SourceEntityName for IS operation.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // IS command must have source view specified.
         if ( lpQualAttrib->lpSourceView == 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib must have SourceView for IS operation.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // If any other attributes are specified, warn user but continue.
         if ( lpQualAttrib->lpDataField || lpQualAttrib->szValue || lpQualAttrib->lpSourceDataField )
         {
            SqlDisplayQualMsg( lpQualView,
                               "Extraneous attributes specified for IS operation.  Only Oper, EntityName, "
                               "SourceEntityName, and SourceView are needed.  All others will be ignored.",
                               zWARNING, szlQualAttrib );
         }
      } // if ( zstrcmpi( lpQualAttrib->szOper, "IS" ) == 0 )...
      else
      {
         // Validate "normal" operators ( =, <, >, etc ).
         if ( lpQualAttrib->lpDataField &&
              (lpQualAttrib->szValue == 0 &&
               lpQualAttrib->lpSourceDataField == 0) )
         {
            // QualAttrib has AttributeName specified but no Value or
            // SourceAttributeName.  If the operator is a comparison
            // operator, a Value is required.  Connection operators ( AND,
            // OR ) and precidence operators ( "(", ")" ) don't have values.
            if ( strstr( "<= =< >= => !=", lpQualAttrib->szOper ) )
            {
               SqlDisplayQualMsg( lpQualView,
                                  "QualAttrib has AttributeName but does not have Value or SourceAttributeName, "
                                  "which are needed for a comparison opererator ( '=', '<', '>=', etc.)",
                                  zERROR, szlQualAttrib );
               return( zCALL_ERROR );
            }
         }

         if ( lpQualAttrib->lpDataField == 0 && (lpQualAttrib->szValue || lpQualAttrib->lpSourceDataField) )
         {
            SqlDisplayQualMsg( lpQualView,
                               "QualAttrib has Value or SourceAttributeName but does not have AttributeName.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }
      }

      // =================================================================
      // ===
      // === Check to see if there are sub-qualifications for the current
      // === QualAttrib.  Retrieve the sub-qualifications if they are there.
      // ===
      // =================================================================
      if ( CheckExistenceOfEntity( lpQualView, szlSubQualAttrib ) == zCURSOR_SET )
      {
         zVIEW lpSubQualView;

         CreateViewFromViewForTask( &lpSubQualView, lpQualView, 0 );
         nRC = SetViewToSubobject( lpSubQualView, szlSubQualAttrib );
         if ( nRC < 0 )
         {
            SqlDisplayQualMsg( lpQualView,
                               "Internal error setting view to Subobject.",
                               zERROR, szlQualAttrib );
            return( zCALL_ERROR );
         }

         // Call SqlRetrieveQualAttrib recursivly to load sub-qual.
         nRC = SqlRetrieveQualAttrib( cDBType, lpView, lpViewOD,
                                      lpSubQualView, lpQualEntity,
                                      &lpQualAttrib->lpFirstSubQualAttrib );
         DropView( lpSubQualView ); // Don't need view anymore.
         if ( nRC < 0 )
            return( nRC );
      }

      // Whew!...Now do all of the above again for the PREVIOUS QualAttrib.
      nRC = SetCursorPrevEntity( lpQualView, szlQualAttrib, "" );
   } // while ( nRC == zCURSOR_SET )...

   // =================================================================
   // ===
   // === End of attribute loop.
   // ===
   // =================================================================

   // Now check to make sure that all the QualAttribs use the same database as the EntitySpec.
   if ( lpQualEntity->lpViewEntity->lpDBName )
      // fnSqlVerifyDataBase with the DBName in lpViewEntity.
      nRC = fnSqlVerifyDataBase( cDBType, lpQualEntity->lpFirstQualAttrib,
                                 lpQualEntity->lpViewEntity->lpDBName, FALSE );
   else
      // fnSqlVerifyDataBase with the default DBName.
      nRC = fnSqlVerifyDataBase( cDBType, lpQualEntity->lpFirstQualAttrib,
                                 lpViewOD->lpDftDBName, TRUE );

   if ( !nRC )
   {
      SqlDisplayQualMsg( lpQualView,
                         "The QualAttrib used to qualify EntitySpec causes two tables from different databases "
                         "to be joined.  This is invalid.",
                         zERROR, szlQualAttrib );
      return( zCALL_ERROR );
   }

   // After going through all QualAttribs, the ParenCount must be zero. If it isn't, then there
   // are too many left parens (if nParenCount > 0 ) or too may right parens (if nParenCount < 0).
   if ( nParenCount != 0 )
   {
      SqlDisplayQualMsg( lpQualView, "Parens for all QualAttrib.Oper under EntitySpec do not match.", zERROR, 0 );
      DisplayObjectInstance( lpQualView, szlEntitySpec, 0 );
      return( zCALL_ERROR );
   }

   return( 0 );
}  /* SqlRetrieveQualAttrib */

/////////////////////////////////////////////////////////////////////////////

   fnSqlRetrieveQualObject

   ??? Why not just use the object in memory?

   Retrieves the information from the qualification view and creates the
   QualEntity chain.

/////////////////////////////////////////////////////////////////////////////
zSHORT
fnSqlRetrieveQualObject( zCHAR        cDBType,
                         zVIEW        lpView,
                         zVIEW        lpQualView,
                         LPVIEWOD     lpViewOD,
                         LPQUALENTITY *lpFirstQualEntity )
{
   LPQUALENTITY lpQualEntity;
   LPVIEWENTITY lpViewEntity;
   zPCHAR       pchEntityName;
   zLONG        hMem;
   zSHORT       nRC;

   *lpFirstQualEntity = 0;

   nRC = SetCursorFirstEntity( lpQualView, szlEntitySpec, "" );
   if ( nRC != zCURSOR_SET )
      // There are no entities in the qualification object, so exit.
      return( 0 );

   // Copy each of the EntitySpec entities to the qualification chain.
   while ( nRC == zCURSOR_SET )
   {
      hMem = SysAllocMemory( (zPVOID) &lpQualEntity, sizeof( QualEntityRecord ), 0, zCOREMEM_ALLOC, 0 );
      if ( lpQualEntity == 0 )
      {
         SqlDisplayQualMsg( lpQualView, "Cannot allocate memory", zERROR, 0 );
         return( zCALL_ERROR );
      }

      // Add lpQualEntity to beginning of chain.
      zmemset( (zPVOID) lpQualEntity, sizeof( QualEntityRecord ), 0 );
      lpQualEntity->hMem = hMem;
      lpQualEntity->lpNextQualEntity = *lpFirstQualEntity;
      *lpFirstQualEntity = lpQualEntity;

      // Find the view entity that matches the qual entity.
      GetAddrForAttribute( &pchEntityName, lpQualView, szlEntitySpec, "EntityName" );
      lpViewEntity = MiGetViewEntityForView( lpView, pchEntityName );

      // If no view entity was found to match entity name in qual entity, then issue an error and exit.
      if ( lpViewEntity == 0 )
      {
         SqlDisplayQualMsg( lpQualView,
                            "Attribute EntityName in entity EntitySpec not found in target object definition.",
                            zERROR, szlEntitySpec );
         return( zCALL_ERROR );
      }

      lpQualEntity->lpViewEntity = lpViewEntity;
      lpQualEntity->bKeysOnly    = TRUE;

      // Retrieve all QualAttribs under the current EntitySpec.
      nRC = SqlRetrieveQualAttrib( cDBType, lpView, lpViewOD,
                                   lpQualView, lpQualEntity,
                                   &lpQualEntity->lpFirstQualAttrib );
      if ( nRC < 0 )
         return( nRC );

      nRC = SetCursorNextEntity( lpQualView, szlEntitySpec, "" );
   } // while ( nRC == zCURSOR_SET )...

#if 0
   // Display the QualEntity structure to trace.
   SqlDisplayQualEntity( *lpFirstQualEntity );
#endif

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////

   SqlRetrieveQualObject

   PURPOSE: To retrieve the information in the qualification object and
   store it in a linked list for later.

/////////////////////////////////////////////////////////////////////////////
zSHORT
SqlRetrieveQualObject( zCHAR        cDBType,
                       zVIEW        lpView,
                       zVIEW        lpQualView,
                       LPVIEWOD     lpViewOD,
                       LPQUALENTITY *lpFirstQualEntity )
{
   zSHORT nRC;

   nRC = fnSqlRetrieveQualObject( cDBType, lpView, lpQualView, lpViewOD, lpFirstQualEntity );
   if ( nRC < 0 )
   {
      if ( *lpFirstQualEntity )
         SqlFreeQualEntity( lpFirstQualEntity );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////

   SqlPrintLine

   Prints the line stored in szBuffer to the trace file.  szLine[ 0 ]
   points to the byte after the last char in szBuffer and must be set to 0.

/////////////////////////////////////////////////////////////////////////////
void
SqlPrintLine( zPCHAR  pchBuffer,
              zPCHAR *ppchLine,
              zPSHORT pnIndentLth,
              zPSHORT pnIndentIncrement,
              zPSHORT pnErrorIdx )
{
   zPCHAR pchPtr;
   zCHAR  szIndentStr[ 1000 ];
   zSHORT nParenIndent;
   zSHORT n;

   // Terminate ppchLine string.
   **ppchLine = 0;

   // If the first character is a right paren ( ')' ), then un-indent two spaces for each right paren.
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
   strcpy_s( szIndentStr, sizeof( szIndentStr ), "SQL> " );
   pchPtr = szIndentStr;
   mAdvanceToNull( pchPtr );
   for ( n = 0; n < (*pnIndentLth - nParenIndent); n++ )
      *pchPtr++ = ' ';

   *pchPtr = 0;

   TraceLineS( szIndentStr, pchBuffer );

   if ( *pnErrorIdx )
   {
      strcpy_s( szIndentStr, sizeof( szIndentStr ), "ERR> " );
      pchPtr = szIndentStr;
      mAdvanceToNull( pchPtr );
      for ( n = 0; n < (*pnIndentLth + *pnErrorIdx); n++ )
         *pchPtr++ = ' ';

      *pchPtr = 0;

      TraceLineS( szIndentStr, "^" );
      *pnErrorIdx = 0;
   }

   *ppchLine     = pchBuffer;
   *pnIndentLth = *pnIndentLth + *pnIndentIncrement;
   *pnIndentIncrement = 0;
}

/////////////////////////////////////////////////////////////////////////////

      fnSqlCopyToken

      Copy pchToken to the buffer pointed to by pchBuffer.  *ppchLine points
      to the position in pchBuffer where pchToken is to be copied and will be
      updated to point to the first character after pchToken in pchBuffer.

      If pchErrorChar points to a character in pchToken, then there is an
      error caused by pchToken.  After pchToken is copied into pchBuffer,
      convert *pnErrorIdx to be the offset of the offending character in
      pchBuffer.

/////////////////////////////////////////////////////////////////////////////
void
fnSqlCopyToken( zPCHAR  pchToken,
                zSHORT  nTokenLth,
                zPCHAR *ppchLine,
                zPCHAR  pchErrorChar,
                zPCHAR  pchBuffer,
                zPSHORT pnErrorIdx )
{
   zSHORT n;

   for ( n = 0; n < nTokenLth; n++ )
   {
      // Copy the n'th character from pchToken to **ppchLine.
      **ppchLine = pchToken[ n ];

      // If the current character in pchToken is the ErrorChar, then set
      // pnErrorIdx to be the offset of the character in pchBuffer.
      if ( pchToken + n == pchErrorChar )
         *pnErrorIdx = *ppchLine - pchBuffer;

      (*ppchLine)++;
   }
}

/////////////////////////////////////////////////////////////////////////////

   fnSqlDisplayCommand

   Formats and displays the SQL command pchSqlCmd.

   If pchErrorChar is non-zero, then the line containing the character
   pointed to by pchErrorChar will be followed by a line with a '^' under
   the character.  This is used to point out errors.

/////////////////////////////////////////////////////////////////////////////
void
fnSqlDisplayCommand( zPCHAR *ppchSqlCmdStr,
                     zPCHAR pchErrorChar,
                     zSHORT nIndentLth,
                     zPCHAR pchInitString,
                     zBOOL  bFirstCall )
{
   zCHAR  szBuffer[ 1000 ];
   zPCHAR pchToken;
   zPCHAR pchLine;
   zPCHAR pchSqlCmd;
   zSHORT nIndentIncrement;
   zSHORT nParenCnt;
   zSHORT nTokenLth;
   zSHORT nErrorIdx;

   pchLine = szBuffer;
   if ( pchInitString )
   {
      strcpy_s( szBuffer, sizeof( szBuffer ), pchInitString );
      mAdvanceToNull( pchLine );
      nIndentIncrement = zstrlen( pchInitString ) + 1;
   }
   else
      nIndentIncrement = 0;

   nParenCnt = 0;
   nErrorIdx = 0;

   // Loop through, copying each character from pchSqlCmd to szBuffer for printing.
   pchSqlCmd = *ppchSqlCmdStr;
   while ( *pchSqlCmd )
   {
      // Skip spaces.
      while ( *pchSqlCmd == ' ' )
         pchSqlCmd++;

      // Get next token.
      pchToken = pchSqlCmd;
      while ( *pchSqlCmd != ' ' && *pchSqlCmd != 0 )
      {
         // If the character is a single quote, skip everything until ending single quote is found.
         if ( *pchSqlCmd++ == '\'' )
         {
            while ( pchSqlCmd[ 0 ] &&
                    (pchSqlCmd[ 0 ] != '\'' && pchSqlCmd[ 1 ] == '\'') )
            {
               pchSqlCmd++;
            }

            if ( *pchSqlCmd == '\'' )
               pchSqlCmd++;
         }
      }

      nTokenLth = (zSHORT) (pchSqlCmd - pchToken);

      if ( pchLine != szBuffer )
         *pchLine++ = ' ';

#if 0
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
            nParenCnt++;
            nIndentIncrement += 2;
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );
            break;

         case ')':
            nParenCnt--;
            nIndentIncrement -= 2;
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );

            // If paren count is 0 and we are in a recursive call, then get out of recursive call.
            if ( nParenCnt == 0 && bFirstCall == FALSE )
            {
              SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );
              *ppchSqlCmdStr = pchSqlCmd;
              return;
            }

            break;

         case ',':
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );
            SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );
            break;

         case 'A':
         case 'O':
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );

            if ( zstrncmpi( pchToken, "AND ", 4 ) == 0 || zstrncmpi( pchToken, "OR ", 3 ) == 0 )
            {
               SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );
            }

            break;

         case 'F':
            if ( zstrncmp( pchToken, "FROM ", 5 ) == 0 )
               SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );
            break;

         case 'I':
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );

            if ( zstrncmp( pchToken, "IN ", 3 ) == 0 )
            {
               SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );
               fnSqlDisplayCommand( &pchSqlCmd, pchErrorChar, nIndentLth, 0, FALSE );
            }

            break;

         case 'S':
            if ( zstrncmp( pchToken, "SELECT ", 7 ) == 0 )
            {
               if ( bFirstCall == FALSE )
                  SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );
            }
            else
            if ( zstrncmp( pchToken, "SET ", 4 ) == 0 )
            {
               SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );
               nIndentIncrement += 4;
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );
            break;

         case 'V':
            if ( zstrncmp( pchToken, "VALUES ", 7 ) == 0 )
            {
               SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );
               nIndentIncrement += 7;
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );
            break;

         case 'W':
            if ( zstrncmp( pchToken, "WHERE ", 6 ) == 0 )
            {
               SqlPrintLine( szBuffer, &pchLine, &nIndentLth, &nIndentIncrement, &nErrorIdx );

               if ( bFirstCall )
                  nIndentLth = 0;

               fnSqlDisplayCommand( &pchSqlCmd, pchErrorChar, nIndentLth, "WHERE", FALSE );
               break;
            }

            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );
            break;

         default :
            fnSqlCopyToken( pchToken, nTokenLth, &pchLine, pchErrorChar, szBuffer, &nErrorIdx );
            break;

      } // switch ( *pchToken )...

   } // while ( *pchSqlCmd )...

   if ( pchLine > szBuffer )
   {
      SqlPrintLine( szBuffer, &pchLine, &nIndentLth,
                    &nIndentIncrement, &nErrorIdx );
   }

   *ppchSqlCmdStr = pchSqlCmd;
}  /* fnSqlDisplayCommand */

/////////////////////////////////////////////////////////////////////////////

      SqlDisplayCommand

/////////////////////////////////////////////////////////////////////////////
void
SqlDisplayCommand( zPCHAR pchSqlCmd, zPCHAR pchErrorChar )
{
#if 0
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
         zmemset( s, m, ' ' );
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

   // Call fnSqlDisplayCommand with an initial indent value of 0.
   fnSqlDisplayCommand( &pchSqlCmd, pchErrorChar, 0, 0, TRUE );
}
