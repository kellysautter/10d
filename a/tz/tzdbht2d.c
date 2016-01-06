#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "ZDRVROPR.H"
#include "stdio.h"

#include "tz__oprs.h"
#include "tzlodopr.h"

/*
CHANGE LOG

2002.08.29  DGC
   Added an argument to ParseSource().

2000.01.13  BL
   remove unreferenced local variable

No change log because this file is used only by DG.

*/

char szlArgument[]         = "Argument";
char szlExecutionResult[]  = "ExecutionResult";
char szlTransformation[]   = "Transformation";

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZEIDONOP.H"

zSHORT
fnShowResult( zVIEW  vSubtask,
              zVIEW  vT,
              zSHORT nRC,
              zPCHAR pszObjectName,
              zPCHAR pszOperName,
              zPCHAR pszMsg )
{
   zVIEW vParentSubtask;
   zVIEW vTranWindow;

   CreateEntity( vT, szlExecutionResult, zPOS_LAST );
   SetAttributeFromInteger( vT, szlExecutionResult, "ReturnCode", (zLONG) nRC );
   SetAttributeFromString( vT, szlExecutionResult, "Operation", "Call Tran" );
   SetAttributeFromString( vT, szlExecutionResult, "ViewName", pszOperName );
   SetAttributeFromString( vT, szlExecutionResult, "LOD", pszObjectName );
   SetAttributeFromString( vT, szlExecutionResult, "FileName", pszMsg );

   SetCursorLastEntity( vT, szlExecutionResult, 0 );

   GetViewByName( &vTranWindow, "ShowTransformation", vSubtask, zLEVEL_TASK );
   GetSubtaskForWindowName( vTranWindow, &vParentSubtask, "TZDBHTSD" );
   RefreshWindow( vParentSubtask );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
ShowTrans_ExecuteTrans( zVIEW vDBH_Subtask )
{
   zSHORT     nRC;
   zLONG      lRC;
   zSHORT     iViewCount;
   zPCHAR     pszOperName;
   zPCHAR     pszObjectName;
   zPCHAR     psz;
   zVIEW      vT;
   zVIEW      vSubtask;
   zVIEW      vViewList[ 20 ];
   WORD       wArgValues[ 30 ];
   LPWORD     pwArg;
   zCHAR      szArgControl[ 20 ];
   LPVIEWOD   lpViewOD;

   GetViewByName( &vT, "TZDBHTSO", vDBH_Subtask, zLEVEL_TASK );

   GetAddrForAttribute( &psz, vT, "Application", "Name" );
   SfCreateSubtask( &vSubtask, vDBH_Subtask, psz );

   GetAddrForAttribute( &pszObjectName, vT, "LOD", "Name" );
   lpViewOD = ActivateViewObject( vSubtask, pszObjectName, FALSE );
   if ( lpViewOD == 0 )
      return( fnShowResult( vDBH_Subtask, vT, zCALL_ERROR, pszObjectName, "",
                            "Can't find obj" ) );

   GetAddrForAttribute( &pszOperName, vT, szlTransformation, "OperName" );
   if ( *pszOperName == 0 )
      return( fnShowResult( vDBH_Subtask, vT, zCALL_ERROR, pszObjectName, "",
                            "No Oper" ) );

   zmemset( wArgValues, 0, sizeof( wArgValues ) );
   pwArg = wArgValues;
   *szArgControl = 0;
   iViewCount = -1;
   for ( nRC = SetCursorFirstEntity( vT, szlArgument, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vT, szlArgument, 0 ) )
   {
      zPCHAR pszDataType;
      zPCHAR pchReturnedValue;

      GetAddrForAttribute( &pchReturnedValue, vT, szlArgument, "ReturnedValue" );
      if ( *pchReturnedValue == 'Y' )
         strcat_s( szArgControl, sizeof( szArgControl ), "p" );

      GetAddrForAttribute( &pszDataType, vT, szlArgument, "DataType" );
      strcat_s( szArgControl, sizeof( szArgControl ), pszDataType );
      switch ( *pszDataType )
      {
         case 'N':
         case 'Y':
         {
            zSHORT n;
            zLONG  l;

            GetIntegerFromAttribute( &l, vT, szlArgument, "Value" );
            n = (zSHORT) l;
            *pwArg++ = (WORD) n;

            break;
         }

         case 'L':
         {
            zLONG  l;

            GetIntegerFromAttribute( &l, vT, szlArgument, "Value" );
            * (zPLONG) pwArg = l;
            pwArg += sizeof( zLONG ) / sizeof( WORD );

            break;
         }

         case zTYPE_DECIMAL:
         {
            zDECIMAL d;

            GetDecimalFromAttribute( &d, vT, szlArgument, "Value" );
            * (zPDECIMAL) pwArg = d;
            pwArg += sizeof( zDECIMAL ) / sizeof( WORD );

            break;
         }

         case 'S':
         {
            GetAddrForAttribute( &psz, vT, szlArgument, "Value" );
            * (zPPCHAR) pwArg = psz;
            pwArg += sizeof( zPCHAR ) / sizeof( WORD );

            break;
         }

         case 'V':
            iViewCount++;

            GetAddrForAttribute( &psz, vT, szlArgument, "SubtaskView" );
            if ( *psz == 'Y' )
            {
               GetAddrForAttribute( &psz, vT, szlArgument, "ApplicationName" );
               if ( *psz == 0 )
               {
                  GetAddrForAttribute( &psz, vT, szlArgument, "Desc" );
                  TraceLineS( "(tzdbhtsd) Arg error - no Appl name for subtask "
                              "view. Desc = ", psz );
                  return( fnShowResult( vDBH_Subtask, vT, zCALL_ERROR,
                                        pszObjectName, pszOperName,
                                        "Arg error" ) );
               }

               SfCreateSubtask( &vViewList[ iViewCount ], vDBH_Subtask, psz );
               if ( vViewList[ iViewCount ] == 0 )
               {
                  GetAddrForAttribute( &psz, vT, szlArgument, "Desc" );
                  TraceLineS( "(tzdbhtsd) Arg error - Bad appl name. "
                              "Desc = ", psz );
                  return( fnShowResult( vDBH_Subtask, vT, zCALL_ERROR,
                                        pszObjectName, pszOperName,
                                        "Arg error" ) );
               }
            }
            else
            {
               // Try getting the view indicated by "ViewName"
               GetAddrForAttribute( &psz, vT, szlArgument, "ViewName" );
               if ( *pchReturnedValue != 'Y' && ( psz == 0 || *psz == 0 ) )
               {
                  GetAddrForAttribute( &psz, vT, szlArgument, "Desc" );
                  TraceLineS( "(tzdbhtsd) Arg error - no view name for view. "
                              "Desc = ", psz );
                  return( fnShowResult( vDBH_Subtask, vT, zCALL_ERROR,
                                        pszObjectName, pszOperName,
                                        "Arg error" ) );
               }

               GetViewByName( &vViewList[ iViewCount ], psz, vDBH_Subtask,
                              zLEVEL_TASK );
               if ( *pchReturnedValue != 'Y' && ( vViewList[ iViewCount ] == 0 ) )
               {
                  GetAddrForAttribute( &psz, vT, szlArgument, "Desc" );
                  TraceLineS( "(tzdbhtsd) Arg error - view name not found. "
                              "Desc = ", psz );
                  return( fnShowResult( vDBH_Subtask, vT, zCALL_ERROR,
                                        pszObjectName, pszOperName,
                                        "Arg error" ) );
               }
            }

            // If ReturnedValue is 'Y', then NetCallOperation is expecting
            // a pointer to a view.
            if ( *pchReturnedValue == 'Y' )
               *(zPVIEW *) pwArg = &vViewList[ iViewCount ];
            else
               *(zPVIEW) pwArg = vViewList[ iViewCount ];

            pwArg += sizeof( zVIEW ) / sizeof( WORD );

            break; // Case 'V'...

      } // switch ( *pszDataType )...

   } // for ( nRC = SetCursorFirstEntity( vT, szlArgument, 0 )...

   if ( CompareAttributeToString( vT, szlTransformation, "NetCall",
                                  "Y" ) == 0 )
   {
#if 0
      nRC = NetCallOperation( "zWinSock", 0,
                              vSubtask,
                              pszObjectName,
                              pszOperName,
                              &lRC,
                              szArgControl,
                              wArgValues[  0 ], wArgValues[  1 ],
                              wArgValues[  2 ], wArgValues[  3 ],
                              wArgValues[  4 ], wArgValues[  5 ],
                              wArgValues[  6 ], wArgValues[  7 ],
                              wArgValues[  8 ], wArgValues[  9 ],
                              wArgValues[ 10 ], wArgValues[ 11 ],
                              wArgValues[ 12 ], wArgValues[ 13 ],
                              wArgValues[ 14 ], wArgValues[ 15 ],
                              wArgValues[ 16 ], wArgValues[ 17 ],
                              wArgValues[ 18 ], wArgValues[ 19 ],
                              wArgValues[ 20 ], wArgValues[ 21 ],
                              wArgValues[ 22 ], wArgValues[ 23 ],
                              wArgValues[ 24 ], wArgValues[ 25 ],
                              wArgValues[ 26 ], wArgValues[ 27 ],
                              wArgValues[ 28 ], wArgValues[ 29 ] );
#endif
   }
   else
   {
      typedef zSHORT (POPERATION local_PFN_OPER)( WORD, WORD, WORD, WORD, WORD,
                                                  WORD, WORD, WORD, WORD, WORD,
                                                  WORD, WORD, WORD, WORD, WORD,
                                                  WORD, WORD, WORD, WORD, WORD,
                                                  WORD, WORD, WORD, WORD, WORD,
                                                  WORD, WORD, WORD, WORD, WORD );

      LPLIBRARY  hLibrary;
      LPAPP      lpApp;
      zCHAR      szFileName[ zMAX_FILENAME_LTH + 1 ];
      local_PFN_OPER pfnOper;

      SfGetApplicationForSubtask( &lpApp, vSubtask );

      strcpy_s( szFileName, sizeof( szFileName ), lpApp->szLibraryDir );
      SysAppendcDirSep( szFileName );
      strcat_s( szFileName, sizeof( szFileName ), lpViewOD->szOperLibname );
      TraceLineS( "DBHTest: Trying to load library ", szFileName );
      hLibrary = SysLoadLibrary( vSubtask, szFileName );
      if ( hLibrary == 0 )
         return( fnShowResult( vDBH_Subtask, vT, zCALL_ERROR, pszObjectName,
                               pszOperName, "Can't load lib" ) );

      TraceLineS( "DBHTest: Trying to load operation ", pszOperName );
      pfnOper = (local_PFN_OPER) SysGetProc( hLibrary, pszOperName );
      if ( pfnOper == 0 )
         return( fnShowResult( vDBH_Subtask, vT, zCALL_ERROR, pszObjectName,
                               pszOperName, "Can't load proc" ) );

      nRC = 0;
      lRC = (*pfnOper)( wArgValues[ 29 ], wArgValues[ 28 ],
                        wArgValues[ 27 ], wArgValues[ 26 ],
                        wArgValues[ 25 ], wArgValues[ 24 ],
                        wArgValues[ 23 ], wArgValues[ 22 ],
                        wArgValues[ 21 ], wArgValues[ 20 ],
                        wArgValues[ 19 ], wArgValues[ 18 ],
                        wArgValues[ 17 ], wArgValues[ 16 ],
                        wArgValues[ 15 ], wArgValues[ 14 ],
                        wArgValues[ 13 ], wArgValues[ 12 ],
                        wArgValues[ 11 ], wArgValues[ 10 ],
                        wArgValues[  9 ], wArgValues[  8 ],
                        wArgValues[  7 ], wArgValues[  6 ],
                        wArgValues[  5 ], wArgValues[  4 ],
                        wArgValues[  3 ], wArgValues[  2 ],
                        wArgValues[  1 ], wArgValues[  0 ] );

      SysFreeLibrary( vDBH_Subtask, hLibrary );
   }

   if ( nRC < 0 )
   {
      strcpy_s( szArgControl, sizeof( szArgControl ), "CallTran error" );
      goto EndOfFunction;
   }

   // Check for any returned views and set their view names.
   iViewCount = -1;
   for ( nRC = SetCursorFirstEntity( vT, szlArgument, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vT, szlArgument, 0 ) )
   {
      zPCHAR pszDataType;
      zPCHAR pchReturnedValue;

      GetAddrForAttribute( &pszDataType, vT, szlArgument, "DataType" );
      switch ( *pszDataType )
      {
         case 'V':
            iViewCount++;

            GetAddrForAttribute( &pchReturnedValue, vT, szlArgument,
                                 "ReturnedValue" );
            if ( *pchReturnedValue != 'Y' )
               continue;

            GetAddrForAttribute( &psz, vT, szlArgument, "SubtaskView" );
            if ( *psz == 'Y' )
               continue;

            GetAddrForAttribute( &psz, vT, szlArgument, "ViewName" );
            if ( psz == 0 || *psz == 0 )
               continue;

            if ( SetCursorFirstEntityByString( vT, "View", "Name",
                                               psz, 0 ) >= zCURSOR_SET )
            {
               zCHAR szName[ 34 ];

               if ( CompareAttributeToString( vT, "Root", "AutoDrop",
                                              "Y" ) == 0 )
               {
                  zVIEW vTemp;

                  GetViewByName( &vTemp, psz, vSubtask, zLEVEL_TASK );
                  if ( vTemp != 0 )
                  {
                     DropView( vTemp );
                  }
               }

               MiGetObjectNameForView( szName, vViewList[ iViewCount ] );
               SetAttributeFromString( vT, "View", "CurrentLOD", szName );
            }

            SetNameForView( vViewList[ iViewCount ], psz, vDBH_Subtask,
                            zLEVEL_TASK );

            break; // Case 'V'...

      } // switch ( *pszDataType )...

   } // for ( nRC = SetCursorFirstEntity( vT, szlArgument, 0 )...

   nRC = 0;
   sprintf_s( szArgControl, sizeof( szArgControl ), "OK/RC = %ld", lRC );

EndOfFunction:
   return( fnShowResult( vDBH_Subtask, vT, nRC, pszObjectName,
                         pszOperName, szArgControl ) );
}

zOPER_EXPORT zLONG OPERATION
fnGetTickCount( zSHORT dummy )
{
   return( SysGetTickCount( ) );
}

zSHORT LOCALOPER
fnInclude_ChangeEntityAttributes( zVIEW        vT,
                                  zVIEW        vTemp,
                                  LPVIEWENTITY lpViewEntity,
                                  zBOOL        bResetChildren )

{
   zSHORT       nRC;
   LPVIEWATTRIB lpViewAttrib;

   if ( lpViewEntity == 0 )
   {
      zPCHAR pszEntityName;

      GetAddrForAttribute( &pszEntityName, vT, "Entity", "Name" );
      if ( CheckExistenceOfEntity( vTemp, pszEntityName ) < zCURSOR_SET )
         return( 0 );

      lpViewEntity = MiGetViewEntityForView( vTemp, pszEntityName );
   }
   else
   if ( CheckExistenceOfEntity( vTemp, lpViewEntity->szName ) < zCURSOR_SET )
      return( 0 );


   for ( nRC = SetCursorFirstEntity( vT, "Attribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vT, "Attribute", 0 ) )
   {
      DeleteEntity( vT, "Attribute", zREPOS_NONE );
   }

   for ( lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib ) )
   {
      if ( lpViewAttrib->bHidden )
         continue;

      CreateEntity( vT, "Attribute", zPOS_LAST );
      SetAttributeFromString( vT, "Attribute", "Name", lpViewAttrib->szName );
      SetAttributeFromAttribute( vT, "Attribute", "Value",
                                 vTemp, lpViewEntity->szName,
                                 lpViewAttrib->szName );
   }

   if ( bResetChildren )
   {
      zSHORT nLevel = lpViewEntity->nLevel;
      zLONG  l;
      zVIEW  vT2;

      CreateViewFromViewForTask( &vT2, vT, 0 );

      l = CountEntitiesForView( vTemp, lpViewEntity->szName );
      SetAttributeFromInteger( vT2, "Entity", "TotalEntities", l );
      l = GetRelativeEntityNumber( vTemp, lpViewEntity->szName, 0, 0 );
      SetAttributeFromInteger( vT2, "Entity", "CurrentEntity", l + 1 );

      for ( lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );
            lpViewEntity && lpViewEntity->nLevel > nLevel;
            lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier ) )
      {
         SetCursorFirstEntityByString( vT2, "Entity", "Name",
                                       lpViewEntity->szName, 0 );

         if ( CheckExistenceOfEntity( vTemp,
                                      lpViewEntity->szName ) < zCURSOR_SET )
         {
            SetAttributeFromString( vT2, "Entity", "TotalEntities", "" );
            SetAttributeFromString( vT2, "Entity", "CurrentEntity", "" );

            for ( nRC = SetCursorFirstEntity( vT2, "Attribute", 0 );
                  nRC >= zCURSOR_SET;
                  nRC = SetCursorNextEntity( vT2, "Attribute", 0 ) )
            {
               DeleteEntity( vT2, "Attribute", zREPOS_NONE );
            }

            continue;
         }

         fnInclude_ChangeEntityAttributes( vT2, vTemp, lpViewEntity, FALSE );

         l = CountEntitiesForView( vTemp, lpViewEntity->szName );
         SetAttributeFromInteger( vT2, "Entity", "TotalEntities", l );
         l = GetRelativeEntityNumber( vTemp, lpViewEntity->szName, 0, 0 );
         SetAttributeFromInteger( vT2, "Entity", "CurrentEntity", l + 1 );
      }

      DropView( vT2 );
   }

   return( 0 );
}

zSHORT LOCALOPER
fnInclude_SelectView( zVIEW  vSubtask,
                      zPCHAR pszViewName,
                      zVIEW  vT )
{
   zSHORT       nRC;
   zPCHAR       pszObjName;
   zPCHAR       pszName;
   zVIEW        vTemp;
   LPVIEWOD     lpViewOD;
   LPVIEWENTITY lpViewEntity;

   // First delete all the entities under View.
   for ( nRC = SetCursorFirstEntity( vT, "Entity", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vT, "Entity", 0 ) )
   {
      DeleteEntity( vT, "Entity", zREPOS_NONE );
   }

   GetAddrForAttribute( &pszObjName, vT, "View", "CurrentLOD" );
   if ( zstrcmp( pszObjName, "?" ) == 0 )
      return( 0 );

   GetViewByName( &vTemp, pszViewName, vSubtask, zLEVEL_TASK );
   if ( vTemp )
      DropView( vTemp );

   GetAddrForAttribute( &pszName, vT, "View", "Name" );
   GetViewByName( &vTemp, pszName, vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTemp, vTemp, 0 );
   SetNameForView( vTemp, pszViewName, vSubtask, zLEVEL_TASK );

   lpViewOD = (LPVIEWOD) MiGetViewEntityForView( vTemp, 0 );
   if ( lpViewOD == 0 )
      return( 0 );

   for ( lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );
         lpViewEntity;
         lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier ) )
   {
      zCHAR  szEntityName[ 100 ];
      zLONG  l;

      CreateEntity( vT, "Entity", zPOS_LAST );
      SetAttributeFromString( vT, "Entity", "Name", lpViewEntity->szName );
      for ( nRC = 0; nRC < (3 * (lpViewEntity->nLevel - 1)); nRC++ )
      {
         szEntityName[ nRC ] = ' ';
      }
      szEntityName[ nRC ] = 0;
      strcat_s( szEntityName, sizeof( szEntityName ), lpViewEntity->szName );
      SetAttributeFromString( vT, "Entity", "IndentedName", szEntityName );

      if ( CheckExistenceOfEntity( vTemp, lpViewEntity->szName ) < zCURSOR_SET )
         continue;

      l = CountEntitiesForView( vTemp, lpViewEntity->szName );
      SetAttributeFromInteger( vT, "Entity", "TotalEntities", l );
      l = GetRelativeEntityNumber( vTemp, lpViewEntity->szName, 0, 0 );
      SetAttributeFromInteger( vT, "Entity", "CurrentEntity", l + 1 );

      fnInclude_ChangeEntityAttributes( vT, vTemp, lpViewEntity, FALSE );
   }

   SetCursorFirstEntity( vT, "Entity", 0 );
   SetCursorFirstEntity( vT, "Attribute", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Include_SelectView1( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTZDBHTSO;

   GetViewByName( &vTZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );
   nRC = fnInclude_SelectView( vSubtask, "_Include1", vTZDBHTSO );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
Include_SelectEntity1( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "AttributeList1" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Include_SelectView2( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTZDBHTSO2;

   GetViewByName( &vTZDBHTSO2, "TZDBHTSO2", vSubtask, zLEVEL_TASK );
   nRC = fnInclude_SelectView( vSubtask, "_Include2", vTZDBHTSO2 );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
Include_SelectEntity2( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "AttributeList2" );
   return( 0 );
}

zSHORT LOCALOPER
fnInclude_ChangeEntity( zVIEW  vSubtask,
                        zVIEW  vT,
                        zPCHAR pszViewName,
                        zSHORT nCursor )
{
   zSHORT nRC = FALSE;
   zPCHAR pszObjName;
   zPCHAR pszEntityName;
   zVIEW  vTemp;

   GetAddrForAttribute( &pszObjName, vT, "View", "CurrentLOD" );
   if ( zstrcmp( pszObjName, "?" ) == 0 )
      return( FALSE );

   GetViewByName( &vTemp, pszViewName, vSubtask, zLEVEL_TASK );
   if ( vTemp == 0 )
      return( FALSE );

   GetAddrForAttribute( &pszEntityName, vT, "Entity", "Name" );
   if ( nCursor == 0 )
   {
      if ( SetCursorNextEntity( vTemp, pszEntityName, 0 ) >= zCURSOR_SET )
         nRC = TRUE;
   }
   else
   {
      if ( SetCursorPrevEntity( vTemp, pszEntityName, 0 ) >= zCURSOR_SET )
         nRC = TRUE;
   }

   if ( nRC )
      fnInclude_ChangeEntityAttributes( vT, vTemp, 0, TRUE );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
Include_NextEntity1( zVIEW vSubtask )
{
   zVIEW  vTZDBHTSO;

   GetViewByName( &vTZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );
   if ( fnInclude_ChangeEntity( vSubtask, vTZDBHTSO, "_Include1", 0 ) )
   {
      RefreshCtrl( vSubtask, "EntityList1" );
      RefreshCtrl( vSubtask, "AttributeList1" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Include_PrevEntity1( zVIEW vSubtask )
{
   zVIEW  vTZDBHTSO;

   GetViewByName( &vTZDBHTSO, "TZDBHTSO", vSubtask, zLEVEL_TASK );
   if ( fnInclude_ChangeEntity( vSubtask, vTZDBHTSO, "_Include1", 1 ) )
   {
      RefreshCtrl( vSubtask, "EntityList1" );
      RefreshCtrl( vSubtask, "AttributeList1" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Include_NextEntity2( zVIEW vSubtask )
{
   zVIEW  vTZDBHTSO2;

   GetViewByName( &vTZDBHTSO2, "TZDBHTSO2", vSubtask, zLEVEL_TASK );
   if ( fnInclude_ChangeEntity( vSubtask, vTZDBHTSO2, "_Include2", 0 ) )
   {
      RefreshCtrl( vSubtask, "EntityList2" );
      RefreshCtrl( vSubtask, "AttributeList2" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Include_PrevEntity2( zVIEW vSubtask )
{
   zVIEW  vTZDBHTSO2;

   GetViewByName( &vTZDBHTSO2, "TZDBHTSO2", vSubtask, zLEVEL_TASK );
   if ( fnInclude_ChangeEntity( vSubtask, vTZDBHTSO2, "_Include2", 1 ) )
   {
      RefreshCtrl( vSubtask, "EntityList2" );
      RefreshCtrl( vSubtask, "AttributeList2" );
   }

   return( 0 );
}

char g_szReadBuffer[ 2000 ];

zSHORT OPERATION
fnRead( zVIEW   lpTaskView,
        zPPCHAR ppchReturnBuffer,
        zPULONG pulReturnLth,
        zPVOID  lpData )
{
   zLONG  hFile = (zLONG) lpData;

   *ppchReturnBuffer = g_szReadBuffer;
   *pulReturnLth = SysReadFile( lpTaskView, hFile, g_szReadBuffer,
                                sizeof( g_szReadBuffer ) );
   if ( *pulReturnLth < sizeof( g_szReadBuffer ) )
      return( TRUE );   // Nothing more to read.
   else
      return( FALSE );  // Still more info in file.
}

zOPER_EXPORT zSHORT OPERATION
DBH_ActivateOI_FromXML( zPVIEW pvOI,
                        zPCHAR pszOD_Name,
                        zVIEW  vSubtask,            /* Subtask qual */
                        zPCHAR pszFileName,
                        zLONG  lFlags )             /* Control Value */
{
   zSHORT nRC;

   nRC = ActivateOI_FromXML_File( pvOI, pszOD_Name, vSubtask, pszFileName, lFlags );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
DBH_WriteOI_ToXML( zVIEW  vOI,
                   zPCHAR pszFileName,
                   zLONG  lFlags )
{
   zSHORT nRC;

   nRC = CommitOI_ToXML_File( vOI, pszFileName, lFlags );

   return( nRC );
}

void TestDec( zVIEW v, zPCHAR psz1, zPCHAR psz2 )
{
   zCHAR  szMsg[ 500 ];
   zSHORT nRC;

   SetAttributeFromString( v, "Root", "Int1", psz1 );
   SetAttributeFromString( v, "Root", "Dec2", psz2 );

   nRC = CompareAttributeToAttribute( v, "Root", "Int1", v, "Root", "Dec2" );

   sprintf_s( szMsg, sizeof( szMsg ), "Result of comparing Int1 = '%s' and Dec2 = '%s' = %d", psz1, psz2, nRC );
   TraceLineS( "DGC ", szMsg );

}

void TestInt( zVIEW v, zPCHAR psz1, zPCHAR psz2 )
{
   zCHAR  szMsg[ 500 ];
   zSHORT nRC;

   SetAttributeFromString( v, "Root", "Dec1", psz1 );
   SetAttributeFromString( v, "Root", "Int2", psz2 );

   nRC = CompareAttributeToAttribute( v, "Root", "Dec1", v, "Root", "Int2" );

   sprintf_s( szMsg, sizeof( szMsg ), "Result of comparing Dec1 = '%s' and Int2 = '%s' = %d", psz1, psz2, nRC );
   TraceLineS( "DGC ", szMsg );

}

void TestListener( zVIEW v, zCPCHAR pszEntityName, zPVOID pv )
{
   TraceLineS( "Cursor Listener -- ", pszEntityName );
}

zOPER_EXPORT zSHORT OPERATION
ParseAndExecute( zVIEW vSubtask )
{
   zVIEW  vLOD;
   zVIEW  vXPG;
   zVIEW  vAppSubtask;
   zVIEW  vStackObject;
// zVIEW  CM_List;
// zVIEW  vTaskLPLR;
   zLONG  lRC;
   zCHAR  szFileName[] = "dgtest";
   zCHAR  szOperName[] = "mInvLotO_temp_function";
   zLONG  lReturn;


   InitializeLPLR( vSubtask, "ZDM" );
   ActivateMetaOI_ByName( vSubtask, &vLOD, 0, zREFER_LOD_META, zSINGLE,
                          "mInvLotO", zCURRENT_OI );

   // Create a temp SourceFile in the LOD
   CreateEntity( vLOD, "SourceFile", zPOS_LAST );
   SetAttributeFromString( vLOD, "SourceFile", "Name", szFileName );

   lRC = ParseSource( vSubtask, vLOD, zPARSE_ACTION_FULL, zPARSE_LIMIT_SOURCE,
                      zPARSE_SOURCE_FILE, szFileName, "",
                      zREFER_LOD_META, zPARSE_DONOTWRITEXPG, "C" );
   if ( lRC < 0 )
   {
      SysMessageBox( vSubtask, "DGC", "Parse failed", 0 );
      TerminateLPLR( vSubtask );
      return( 0 );
   }

   // Set up interpreter.
   // Create the appl subtask.
   SfCreateSubtask( &vAppSubtask, vSubtask, "ZDM" );

   // name the ZeidonVML subtask at the task level.
   SetNameForView( vSubtask, "ZeidonVML", vSubtask, zLEVEL_TASK );

   GetViewByName( &vXPG, "XPG", vSubtask, zLEVEL_TASK );
   SetNameForView( vXPG, "XPG", vSubtask, zLEVEL_SUBTASK );

   // Initialize the stack object with arguments for the operation.
   ActivateEmptyObjectInstance( &vStackObject, "TZVSRTOO", vSubtask, zSINGLE );
   CreateEntity( vStackObject, "StackObject", zPOS_AFTER );
   CreateEntity( vStackObject, "OperationCall", zPOS_AFTER );
   SetAttributeFromString( vStackObject, "OperationCall", "Name", szOperName );
   CreateEntity( vStackObject, "Variable", zPOS_AFTER );
   SetAttributeFromInteger( vStackObject, "Variable", "ID", 1 );
   SetAttributeFromString( vStackObject, "Variable", "Name", "Subtask" );

   CreateEntity( vStackObject, "Variable", zPOS_AFTER );
   SetAttributeFromInteger( vStackObject, "Variable", "ID", 2 );
   SetAttributeFromString( vStackObject, "Variable", "Name",
                           "InternalEntityStructure" );
   SetAttributeFromBlob( vStackObject, "Variable", "Value", "EntityName", 11 );

   CreateEntity( vStackObject, "Variable", zPOS_AFTER );
   SetAttributeFromInteger( vStackObject, "Variable", "ID", 3 );
   SetAttributeFromString( vStackObject, "Variable", "Name",
                           "InternalAttribStructure" );
   SetAttributeFromBlob( vStackObject, "Variable", "Value", "AttribName", 11 );

   // Name the Stack Object
   SetNameForView( vStackObject, "StackObject", vSubtask, zLEVEL_SUBTASK );

#if 0
   TerminateLPLR( vSubtask );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   DropView( vTaskLPLR );
#endif

   SysMessageBox( vSubtask, "DGC", "about to invoke interp", 1 );
   InvokeInterp( vSubtask, vAppSubtask, szFileName, szOperName, &lReturn );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Tester( zVIEW vSubtask )
{
   zVIEW v;
// zVIEW vQual;
   zCHAR sz[ 5000 ];

   GetViewByName( &v, "Source", vSubtask, zLEVEL_TASK );

#if 0
   //GetStringFromAttribute( sz, sizeof( sz ), v, "SalesOrder", "DonTest" );
   //TraceLineS( "DonTest =====> ", sz );
   GetStringFromAttribute( sz, sizeof( sz ), v, "SalesOrder", "DGTest" );
   TraceLineS( "DGTest  =====> ", sz );
#endif

   CommitSubobjectToXML_File( v, "c:\\temp\\temp1.por", "Employee", 0 );
   TraceLineS( "DGTest ==> ", sz );

   return( 0 );
}

#ifdef __cplusplus
}
#endif
