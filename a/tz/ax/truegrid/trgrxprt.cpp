/********************************************************************************************/
//
// File:            trgrxprt.cpp
// *******************************************************************
// *  Copyright © 2001 - 2016 QuinSoft, Inc. and TONBELLER AG.       *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:           TMV
// Datum:           10. Oktober 2001
// describtion:     this file contains all the functions that are exported so they can be
//                  invoked as global operation in a Zeidon envirionment
//
/* Change log most recent first order:

  2001.10.10   TMV   2000   ActiveX
     add inline documentation

  2001.08.08  TMV
     fix a bug with TG_SetBackColor

  2001.06.18  BL    ActiveX
     Change Parameter in Operation TG_LockColumn

  2001.06.17  BL    ActiveX
     added new Grid functions (ITrueDBGridCtrl_GetMouseDownParam,
     TG_GetMouseClick and TG_GetMouseClickWithKey)

  2001.04.03  BL    2000   ActiveX
      modified ITrueDBGridCtrl_SetActiveCell for set focus to Grid Control

  2001.03.13   TMV   ActiveX
      added functions for call from VML

   2001.3.13   TMV   2000   ActiveX
      adding ITrueDBGridCtrl_EnableAutosort to enable/disable autosort

   2001.31.01   TMV   2000   ActiveX
      adding MFC macros for debug purpose

   2000.9.18   TMV   2000   ActiveX
      add additional exported operation "ITrueDBGridCtrl_SetCheckBoxTranslation"
      to support checkboxes in a grid

   2000.09.06   TMV
      Created


*/
/*********************************************************************************************/
#include "zstdafx.h"
#include "tdbg6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef __cplusplus
extern "C"
{
#endif

static void TraceExceptionError(CException *e, CString strExtra )
{
   CString strTitle = "<Truegrid Exception> " + strExtra + " ";

   CString strErrorMessage;
   LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
   e->GetErrorMessage( lpErrorMessage, 1024 );
   strErrorMessage.ReleaseBuffer();
   strErrorMessage = "   " + strErrorMessage;

   TraceLineS( strTitle, "" );
   TraceLineS( strErrorMessage, "");
}

//****************************************************************************************************
//
// Special exports
//
//
//****************************************************************************************************

/////////////////////////////////////////////////////////////////////////////////////////
// Operations for retrieving event informations

//./ ADD NAME=TG_GetMouseDownParam
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_GetMouseDownParam
//                                            10/9/2001 Modified
//  PURPOSE:     To retrieve the parameters of a mouseclick event
//
//  PARAMETERS:  zVIEW   vSubtask - View to the window
//               zPSHORT psButton - pointer to a short getting information of
//                                  which button has been pressed possible values are
//                                     1 - left
//                                     2 - right
//                                     4 - middle
//
//               zPSHORT psShift  - pointer to a short getting informaation of
//                                  which key was press during MouseDown event.
//                                  Possible values are
//                                     1 - shift
//                                     2 - ctrl
//                                     4 - alt
//                                     5 - alt + shift
//                                     6 - alt + ctrl
//                                     7 - alt + ctrl + shift
//
//               zPLONG  plXPos   - pointer to a long getting information about the
//                                  x - position where the MouseDown event occured
//
//               zPLONG  plYPos   - pointer to a long getting information about the
//                                  y - position where the MouseDown event occured
//
//  RETURNS   :  0 if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 6
zOPER_EXPORT zSHORT OPERATION
TG_GetMouseDownParam( zVIEW  vSubtask,
                      zPSHORT psButton,
                      zPSHORT psShift,
                      zPLONG plXPos,
                      zPLONG  plYPos )
{
   zCTRL_EVENT *pCE = NULL;
   zSHORT      nRC = zCALL_ERROR;
   const DISPID MOUSE_DOWN_ID = 0xfffffda3;
   zSHORT      sButton = 0;
   zSHORT      sShift = 0;
   zLONG X = 0;
   zLONG Y = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_GetMouseDownParam","");
#endif

   // get the parameter of current Action
   pCE = (zCTRL_EVENT *) GetActionParameters( vSubtask );

   // check whether event is type of mouse_down
   if ( pCE->m_pEvent->m_dispid != MOUSE_DOWN_ID )
      nRC = zCALL_ERROR;
   else
   {
      // if we got here this even has 4 parameters
      if( pCE->m_pEvent->m_pDispParams->cArgs == 4 )
      {
         Y         = (zCHAR) pCE->m_pEvent->m_pDispParams->rgvarg[ 0 ].iVal;
         X         = (zCHAR) pCE->m_pEvent->m_pDispParams->rgvarg[ 1 ].iVal;
         sShift    = (zCHAR) pCE->m_pEvent->m_pDispParams->rgvarg[ 2 ].lVal;
         sButton   = (zCHAR) pCE->m_pEvent->m_pDispParams->rgvarg[ 3 ].lVal;
         *plYPos   = Y      ;
         *plXPos   = X      ;
         *psShift  = sShift ;
         *psButton = sButton;
         nRC = 0;
      }
      else
         nRC = zCALL_ERROR;
   }

   return nRC;
}


//./ ADD NAME=TG_GetMouseClick
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_GetMouseClick
//                                            10/9/2001 Modified
//  PURPOSE:     Determines whether a MouseDown event occured using a specified
//               mouse button. Returns 1 if the event occured using the specified
//               mouse button
//
//  PARAMETERS:  zVIEW  vSubtask - View to the window
//               zSHORT nButton  - specifies the MouseButton to be checked
//                                 possible values are
//                                     1 - left
//                                     2 - right
//                                     4 - middle
//
//  RETURNS   :  1 - if the MouseDown Event occured using the specified button
//               0 - if the MouseDown Event occured using a button other than specified
//               otherwise zCALL_ERROR
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
TG_GetMouseClick( zVIEW vSubtask, zSHORT nButton )
{
   zLONG  lXPos;
   zLONG  lYPos;
   zSHORT nShift;
   zSHORT nMouseButton;
   zSHORT nRC;

#ifdef DEBUG_ALL
   TraceLineS("TG_GetMouseClick","");
#endif

   nRC = TG_GetMouseDownParam( vSubtask, &nMouseButton, &nShift,
                                            &lXPos, &lYPos );
   if ( nRC >= 0 )
   {
      if ( nMouseButton == nButton )
         nRC = 1;
      else
         nRC = 0;
   }

   return nRC;
}



//./ ADD NAME=TG_GetMouseClickWithKey
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_GetMouseClickWithKey
//                                            10/9/2001 Modified
//  PURPOSE:     Determines whether a MouseDown event occured using a specified
//               mouse button and key. Returns 1 if the event occured using the specified
//               mouse button and key
//
//  PARAMETERS:  zVIEW  vSubtask - View to the window
//
//               zSHORT nButton  - specifies the MouseButton to be checked
//                                 possible values are
//                                     1 - left
//                                     2 - right
//                                     4 - middle
//
//               zPSHORT psShift  - pointer to a short getting informaation of
//                                  which key was press during MouseDown event.
//                                  Possible values are
//                                     1 - shift
//                                     2 - ctrl
//                                     4 - alt
//                                     5 - alt + shift
//                                     6 - alt + ctrl
//                                     7 - alt + ctrl + shift
//
//  RETURNS   :  1 - if the MouseDown Event occured using the
//                   specified button and key
//               0 - if the MouseDown Event occured using a
//                   button/key other than specified
//               otherwise zCALL_ERROR
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
TG_GetMouseClickWithKey( zVIEW vSubtask, zSHORT nButton, zSHORT nKey )
{
   zLONG  lXPos = 0;
   zLONG  lYPos = 0;
   zSHORT nShift= 0;
   zSHORT nMouseButton = 0;
   zSHORT nRC = 0;

#ifdef DEBUG_ALL
   TraceLineS("TG_GetMouseClickWithKey","");
#endif


   nRC = TG_GetMouseDownParam( vSubtask, &nMouseButton, &nShift,
                                            &lXPos, &lYPos );
   if ( nRC >= 0 )
   {
      if ( nMouseButton == nButton && nShift == nKey )
         nRC = 1;
      else
         nRC = 0;
   }

   return nRC;
}




//./ ADD NAME=ITrueDBGridCtrl_SetDataMode
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       ITrueDBGridCtrl_SetDataMode
//                                            10/9/2001 Modified
//  PURPOSE:     Sets the data access mode ( for internal [Quinsoft/TONBELLER] use only )
//                                         ==============================================
//  PARAMETERS:  zVIEW vSubtask     - view to the window
//               zCPCHAR cpcCtrlTag - controls tag
//               zLONG nDataMode    - datamode
//                                      0 - Bound (default)
//                                      1 - Unbound
//                                      2 - Unbound Extended
//                                      3 - Application
//                                      4 - Storage ( the only one currently implemented )
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
ITrueDBGridCtrl_SetDataMode( zVIEW vSubtask, zCPCHAR cpcCtrlTag, zLONG nDataMode )
{
   zSHORT nRC = 0;

#ifdef DEBUG_ALL
   TraceLineS("ITrueDBGridCtrl_SetDataMode","");
#endif

   if ( IsRemoteServer() )
   {
      return( zCALL_ERROR ); // this is an operation that shuld not be called in web
   }
   else
   {
      // validate parameters
      ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
      if ( !pInstance )
         return( zCALL_ERROR );

      TRY
      {
         pInstance->SetDataMode( nDataMode );
      }
      CATCH_ALL ( e )
      {
         TraceExceptionError( e, "ITrueDBGridCtrl_SetDataMode" );
         nRC = zCALL_ERROR;
      }
      END_CATCH_ALL
   }

   return nRC;
}



//./ ADD NAME=TG_LockColumn
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_LockColumn
//                                            10/9/2001 Modified
//  PURPOSE:     Locks or unlocks a column, to permit altering a cells value.
//               Before a column is unlocked the mapping is checked whether it
//               allows updating a cells value
//
//  PARAMETERS:  zVIEW   vSubtask   - View to the window
//               zCPCHAR cpcCtrlTag - Controls Tag
//               zSHORT  nIndex     - a zero based index specifing the column
//                                    to be locked/unlocked
//               zBOOL   bLock      - TRUE if the column has to be locked
//                                    otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise a value < 0
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
TG_LockColumn( zVIEW vSubtask, zCPCHAR cpcCtrlTag, zSHORT nIndex, zBOOL bLock )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_LockColumn","");
#endif

#ifdef zREMOTE_SERVER
   if ( IsRemoteServer() )
   {
      ZWEBGRID_LockColumn( vSubtask, cpcCtrlTag, nIndex, bLock );
   }
   else
#endif
   {
      LPDISPATCH lpDispatchColumns = NULL;

      // validate parameters
      ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
      if ( !pInstance || nIndex  < 0 )
         nRC = -1;

      if ( nRC >= 0 )
         nRC = pInstance->LockColumn( nIndex, bLock );
      else
         nRC = -1;
   }

   return nRC;
}



//./ ADD NAME=TG_SetCellData
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetCellData
//                                            10/9/2001 Modified
//  PURPOSE:    To set a cells value using cell coordinates ( row/column )
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zCPCHAR pchData    - the string value to be set
//              zLONG   lRowNbr    - zero based index specifying the row
//              zLONG   lColNbr    - zero based index specifying the column
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 6
zOPER_EXPORT zSHORT OPERATION
TG_SetCellData( zVIEW vSubtask,
               zCPCHAR  cpcCtrlTag,
               zCPCHAR  pchData,
               zLONG lRowNbr,
               zLONG lColNbr )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetCellData","");
#endif

#ifdef zREMOTE_SERVER
   if ( IsRemoteServer() )
   {
      nRC = ZWEBGRID_SetCellData( vSubtask, cpcCtrlTag, pchData, lRowNbr, lColNbr );
   }
   else
#endif
   {
      ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
      if ( !pInstance )
         nRC = zCALL_ERROR;
      else
      {
         nRC = pInstance->SetCellData( pchData, lRowNbr, lColNbr );
         nRC = ( nRC == 0 ) ? zCALL_ERROR : 0;
      }
   }

   return nRC;
}



//./ ADD NAME=TG_GetCellData
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_GetCellData
//                                            10/9/2001 Modified
//  PURPOSE:    To get a cells value using cell coordinates ( row/column )
//
//  PARAMETERS: zVIEW   vSubtask   - View to the window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zPCHAR  pchData    - a string buffer getting the data
//              zLONG   lRowNbr    - zero based index specifying the row
//              zLONG   lColNbr    - zero based index specifying the column
//
//  RETURNS   :  the number of bytes copied into the string buffer
//               otherwise zCALL_ERROR
/**************************************************************************/
//./END + 6
zOPER_EXPORT zSHORT OPERATION
TG_GetCellData( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zPCHAR  pchData,
                zLONG   lMaxLth,
                zLONG   lRowNbr,
                zLONG   lColNbr )
{
   zSHORT nRC = zCALL_ERROR;

#ifdef DEBUG_ALL
   TraceLineS("TG_GetCellData","");
#endif

   if( !pchData )
       return nRC;
   else
      *pchData = '\0';

#ifdef zREMOTE_SERVER
   if ( IsRemoteServer() )
   {
      nRC = ZWEBGRID_GetCellData( vSubtask, cpcCtrlTag, pchData, lMaxLth, lRowNbr, lColNbr );
   }
   else
#endif
   {
      LPDISPATCH lpDispatchColumns = NULL;

      // validate parameters
      ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
      if ( !pInstance )
         nRC = zCALL_ERROR;
      else
         nRC = pInstance->GetCellData( pchData, lRowNbr, lColNbr );
   }

   return nRC;
}



//./ ADD NAME=TG_GetActiveCell
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_GetActiveCell
//                                            10/9/2001 Modified
//  PURPOSE:    to determine which cell has got the input focus
//
//  PARAMETERS: zVIEW   vSubtask   - View to the window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zPLONG  lRowNbr    - pointer to a long buffer getting the
//                                   row index ( zero based )
//                                   if the active cell is a cell of a row that
//                                   is in AddNew mode the returned row number is -1
//              zPLONG  lColNbr    - pointer to a long buffer getting the
//                                   column index ( zero based )
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_GetActiveCell( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  zPLONG  lRowNbr,
                  zPLONG  lColNbr )
{
   zSHORT nRC = zCALL_ERROR;
#ifdef DEBUG_ALL
   TraceLineS("TG_GetActiveCell","");
#endif

#ifdef zREMOTE_SERVER
   if ( IsRemoteServer() )
   {
      nRC = ZWEBGRID_GetActiveCell( vSubtask, cpcCtrlTag, lRowNbr, lColNbr );
   }
   else
#endif
   {
      LPDISPATCH lpDispatchColumns = NULL;

      // validate parameters
      ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
      if ( !pInstance )
         nRC = zCALL_ERROR;
      else
      {
         nRC = pInstance->GetActiveCell( lRowNbr, lColNbr );
         nRC = ( nRC == 0 ) ? zCALL_ERROR : 0;
      }
   }
   return nRC;
}



//./ ADD NAME=TG_SetActiveCell
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:      TG_SetActiveCell
//                                            10/9/2001 Modified
//  PURPOSE:    to set the input focus to a cell specified by row and column index
//
//  PARAMETERS: zVIEW   vSubtask   - View to the window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zLONG   lRowNbr    - row index ( zero based ) specify -1 to set
//                                   cursor to a different column of the current row
//              zLONG   lColNbr    - column index ( zero based ) specify -1 to set
//                                   cursor to the same column of the specified row
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
TG_SetActiveCell( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zLONG lRowNbr, zLONG lColNbr )
{
   zSHORT nRC = zCALL_ERROR;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetActiveCell","");
#endif

#ifdef zREMOTE_SERVER
   if ( IsRemoteServer() )
   {
      ZWEBGRID_SetActiveCell( vSubtask, cpcCtrlTag, lRowNbr, lColNbr );
   }
   else
#endif
   {
      LPDISPATCH lpDispatchColumns = NULL;
      // validate parameters
      ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
      if ( !pInstance )
         nRC = zCALL_ERROR;
      else
      {
         nRC = pInstance->SetCurrentCell( lRowNbr, lColNbr, TRUE );
         nRC = (nRC == 1) ? 0 : zCALL_ERROR;
      }

   }

   if ( nRC != zCALL_ERROR )
      SetFocusToCtrl( vSubtask, cpcCtrlTag );

   return nRC;
}


//./ ADD NAME=TG_SetCheckBoxTranslation
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetCheckBoxTranslation
//                                            10/10/2001 Modified
//  PURPOSE:    Defines Translation strings for a checked and unchecked
//              Checkbox in a grid
//
//  PARAMETERS: zVIEW   vSubtask    - View to the Dialog window
//              zCPCHAR cpcCtrlTag  - the controls tag
//              zSHORT  nColumn     - ColumnIndex of the column that
//                                    that displays a checkbox
//              zCPCHAR zcpcTRUE    - String that is assigned to the entityattribute
//                                    if the checkbox is checked
//              zCPCHAR zcpcFALSE   - String that is assigned to the entityattribute
//                                    if the checkbox is unchecked
// RETURNS   :  0 - if succesful otherwise < 0
//             -1 - if the control could not be located
//             -2 - if column index is out of range
//             -3 - if Checked/unchecked values are eqaul
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
TG_SetCheckBoxTranslation( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zSHORT nColumn, zCPCHAR zcpcTRUE, zCPCHAR zcpcFALSE )
{
   zSHORT nRC = zCALL_ERROR;

#ifdef DEBUG_ALL
   TraceLineS("TG_SetCheckBoxTranslation","");
#endif

   // validate parameters
#ifdef zREMOTE_SERVER
   if ( IsRemoteServer() )
   {
      nRC = ZWEBGRID_SetCheckBoxTranslation( vSubtask, cpcCtrlTag, nColumn, zcpcTRUE, zcpcFALSE );
   }
   else
#endif
   {
      ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
      if ( !pInstance )
         nRC = -1;
      else
         nRC = pInstance->SetCheckBoxTranslation( nColumn, zcpcTRUE, zcpcFALSE );
   }

   return nRC;

}


//./ ADD NAME=OPERATION    TG_EnableAutosort
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       OPERATION    TG_EnableAutosort
//                                            3/13/2001 Modified
//  PURPOSE:     Enable or disable autosort functionality
//
//  PARAMETERS:  zVIEW vSubtask      - View to the Dialog window
//               zCPCHAR  cpcCtrlTag - the controls tag
//               zBOOL propVal       - TRUE to enable autosort otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 1
zOPER_EXPORT zSHORT OPERATION
TG_EnableAutosort( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zBOOL propVal )
{
   zSHORT nRC = zCALL_ERROR;
#ifdef DEBUG_ALL
   TraceLineS("TG_EnableAutosort","");
#endif
#ifdef zREMOTE_SERVER
   if ( IsRemoteServer() )
   {
      return ZWEBGRID_EnableAutosort( vSubtask, cpcCtrlTag, propVal );
   }
   else
#endif
   {
      ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
      if ( pInstance )
      {
         pInstance->EnableAutosort( propVal );
         nRC = 0;
      }
   }
   return nRC;
}


//./ ADD NAME=TG_AllowAddNew
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:      TG_AllowAddNew
//                                            10/9/2001 Modified
//  PURPOSE:    to enable or disable the ability to add new rows in the grid
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog window
//              zCPCHAR cpcCtrlTag - the controls tag
//              zBOOL   bAllow     - TRUE to allow adding rows using the grid
//                                   otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
TG_AllowAddNew( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zBOOL bAllow )
{
   zSHORT nRC = 0;

#ifdef DEBUG_ALL
   TraceLineS("TG_AllowAddNew","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   pInstance->SetAllowAddNew( bAllow );

   return nRC;
}


//./ ADD NAME=TG_AllowDelete
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_AllowDelete
//                                            10/9/2001 Modified
//  PURPOSE:    to enable or disable the ability to delete rows from the grid
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog window
//              zCPCHAR cpcCtrlTag - the controls tag
//              zBOOL   bAllow     - TRUE to allow adding rows using the grid
//                                   otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
TG_AllowDelete( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zBOOL bAllow )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_AllowDelete","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   pInstance->SetAllowDelete( bAllow );

   return nRC;
}


//./ ADD NAME=TG_LockGrid
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_LockGrid
//                                            10/9/2001 Modified
//  PURPOSE:    to lock or unlock a split in a grid
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog window
//              zCPCHAR cpcCtrlTag - the controls tag
//              zLONG   nSplit     - zero based index identifying the split
//                                   to be locked/unlocked
//                                   specify -1 to lock all splits in the grid
//              zBOOL   bLock      - TRUE to lock the split
//                                   otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 2
zOPER_EXPORT zSHORT OPERATION
TG_LockGrid( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zLONG nSplit, zBOOL bLock )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_LockGrid","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      LPDISPATCH pDispatch = pInstance->GetSplits();
      if( !pDispatch )
         return( zCALL_ERROR );

      Splits splits( pDispatch );

      zLONG nSplitCount = splits.GetCount();

      if(  nSplitCount < nSplit )// check against upper bounds
      {
         TraceLineS( "Index out of Range ", "TG_LockGrid" );
         return( zCALL_ERROR );
      }

      if( nSplit < 0 ) // do it for all splits?
      {
         for( long lIndex = 0; lIndex < nSplitCount; lIndex++ )
         {
            Split split( splits.GetItem( lIndex ) ) ;
            split.SetLocked( bLock );
         }
      }
      else
      {
         long lIndex = 0;
         Split split( splits.GetItem( lIndex ) ) ;
         split.SetLocked( bLock );
      }
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "TG_LockGrid" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetForegroundColor
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetForegroundColor
//                                            10/9/2001 Modified
//  PURPOSE:    to set the foreground color of a specific column
//
//  PARAMETERS: zVIEW vSubtask      - View to the Dialog Window
//              zCPCHAR  cpcCtrlTag - Controls Tag
//              zLONG nColumn       - zero based index to identify the column
//                                    specify -1 to affect all columns
//              zLONG lRGBValue     - the color
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetForegroundColor( zVIEW vSubtask,
                       zCPCHAR  cpcCtrlTag,
                       zLONG nColumn,
                       zLONG lRGBValue )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetForegroundColor","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set the color for all columns in the grid
      if( nColumn < 0 )
      {
         for ( zSHORT sCol = 0; sCol < nColumnCount; sCol++ )
         {
            Column col = cols.GetColumn( sCol );
            col.SetForeColor( lRGBValue );
         }
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         col.SetForeColor( lRGBValue );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetForegroundColor" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetBackgroundColor
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetBackgroundColor
//                                            10/9/2001 Modified
//  PURPOSE:    to set the background color of a specific column
//
//  PARAMETERS: zVIEW vSubtask      - View to the Dialog Window
//              zCPCHAR  cpcCtrlTag - Controls Tag
//              zLONG nColumn       - zero based index to identify the column
//                                    specify -1 to affect all columns
//              zLONG lRGBValue     - the color
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetBackgroundColor( zVIEW   vSubtask,
                       zCPCHAR cpcCtrlTag,
                       zLONG   nColumn,
                       zLONG   lRGBValue )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetBackgroundColor","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set the color for all columns in the grid
      if( nColumn < 0 )
      {
         for ( zSHORT sCol = 0; sCol < nColumnCount; sCol++ )
         {
            Column col = cols.GetColumn( sCol );
            col.SetBackColor( lRGBValue );
            ZGridColumn *pGC = pInstance->GetGridColumn( sCol );
            if( pGC )
               pGC->SetUnlockColor( lRGBValue );
         }
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         col.SetBackColor( lRGBValue );
         ZGridColumn *pGC = pInstance->GetGridColumn( (zSHORT)nColumn );
         if( pGC )
            pGC->SetUnlockColor( lRGBValue );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetBackgroundColor" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetTextColor
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetTextColor
//                                            10/9/2001 Modified
//  PURPOSE:    to set the foreground color of a specific column if the column
//              is in edit mode
//
//  PARAMETERS: zVIEW vSubtask      - View to the Dialog Window
//              zCPCHAR  cpcCtrlTag - Controls Tag
//              zLONG nColumn       - zero based index to identify the column
//                                    specify -1 to affect all columns
//              zLONG lRGBValue     - the color
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetTextColor( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zLONG   nColumn,
                 zLONG   lRGBValue )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetTextColor","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set the color for all columns in the grid
      if( nColumn < 0 )
      {
         for ( zSHORT sCol = 0; sCol < nColumnCount; sCol++ )
         {
            Column col = cols.GetColumn( sCol );
            col.SetEditForeColor( lRGBValue );
         }
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         col.SetEditForeColor( lRGBValue );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetTextColor" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetFont
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:      TG_SetFont
//                                            10/9/2001 Modified
//  PURPOSE:    to set the font of a specific column
//
//  PARAMETERS: zVIEW vSubtask      - View to the Dialog Window
//              zCPCHAR  cpcCtrlTag - Controls Tag
//              zLONG nColumn       - zero based index to identify the column
//                                    specify -1 to affect all columns
//              zCPCHAR pFontName   - name of the font to be set
//              zLONG   sFontSize   - Font size
//              zBOOL   bBold       - TRUE if font is bold otherwise FALSE
//              zBOOL   bUnderLine  - TRUE if text is underlined otherwise FALSE
//              zBOOL   bItalic     - TRUE if font is Italic otherwise FALSE
//              zBOOL   bStrikeThrough - TRUE if text is striked through otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 10
zOPER_EXPORT zSHORT OPERATION
TG_SetFont( zVIEW   vSubtask,
            zCPCHAR cpcCtrlTag,
            zLONG   nColumn,
            zCPCHAR pFontName,
            zLONG   sFontSize,
            zBOOL   bBold,
            zBOOL   bUnderLine,
            zBOOL   bItalic,
            zBOOL   bStrikeThrough )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetFont","");
#endif
   CY cy;
   cy.int64 = sFontSize;

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set the color for all columns in the grid
      if( nColumn < 0 )
      {
         for ( zSHORT sCol = 0; sCol < nColumnCount; sCol++ )
         {
            Column col = cols.GetColumn( sCol );
            COleFont f( col.GetFont() );
            f.SetName( pFontName );
            f.SetSize( cy );
            f.SetBold( bBold );
            f.SetUnderline( bUnderLine );
            f.SetItalic( bItalic );
            f.SetStrikethrough( bStrikeThrough );
            col.SetFont( f.m_lpDispatch );
         }
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         COleFont f( col.GetFont() );
         f.SetName( pFontName );
         cy = f.GetSize();
         cy.int64 = sFontSize;
         f.SetSize( cy );
         f.SetBold( bBold );
         f.SetUnderline( bUnderLine );
         f.SetItalic( bItalic );
         f.SetStrikethrough( bStrikeThrough );
         col.SetFont( f.m_lpDispatch );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetFont" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetCaption
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetCaption
//                                            10/9/2001 Modified
//  PURPOSE:    to set the caption for a specific column or the grid
//
//  PARAMETERS: zVIEW vSubtask      - View to the Dialog Window
//              zCPCHAR  cpcCtrlTag - Controls Tag
//              zLONG nColumn       - zero based index to identify the column
//                                    specify -1 to set the grids caption
//              zCPCHAR pszCaption  - the text to be used as Column/Grid Caption
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetCaption( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zLONG   nColumn,
               zCPCHAR pszCaption )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetCaption","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set the color for all columns in the grid
      if( nColumn < 0 )
      {
         pInstance->SetCaption( pszCaption );
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         col.SetCaption( pszCaption );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetCaption" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetAlignment
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetAlignment
//                                            10/9/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: zVIEW   vSubtask    - View to the Dialog Window
//              zCPCHAR cpcCtrlTag  - Controls Tag
//              zLONG   nColumn     - zero based index to identify the column
//                                    specify -1 to affect all columns
//              zLONG   nAlignment  - the column alignment possible values are
//                                      0 - Left
//                                      1 - Right
//                                      2 - Center
//
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetAlignment( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  zLONG   nColumn,
                  zLONG   nAlignment)
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetAlignment","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set the color for all columns in the grid
      if( nColumn < 0 )
      {
         for ( zSHORT sCol = 0; sCol < nColumnCount; sCol++ )
         {
            Column col = cols.GetColumn( sCol );
            col.SetAlignment( nAlignment );
         }
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         col.SetAlignment( nAlignment );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetAllignment" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetColumnWidth
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetColumnWidth
//                                            10/9/2001 Modified
//  PURPOSE:    to set a columns width
//
//  PARAMETERS: zVIEW   vSubtask    - View to the Dialog Window
//              zCPCHAR cpcCtrlTag  - Controls Tag
//              zLONG   nColumn     - zero based index to identify the column
//                                    specify -1 to affect all columns
//              zLONG   nWidth      - column width
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetColumnWidth( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zLONG   nColumn,
                   zLONG   nWidth)
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetColumnWidth","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set the color for all columns in the grid
      if( nColumn < 0 )
      {
         for ( zSHORT sCol = 0; sCol < nColumnCount; sCol++ )
         {
            Column col = cols.GetColumn( sCol );
            col.SetWidth( (float)nWidth );
         }
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         col.SetWidth( (float)nWidth );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetColumnWidth" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}




//./ ADD NAME=TG_SetMultiSelect
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetMultiSelect
//                                            10/9/2001 Modified
//  PURPOSE:    to set the select mode of the grid
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zLONG   lSet       - the select mode of the grid
//                                   0 - None
//                                   1 - Simple
//                                   2 - Extended
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 4
zOPER_EXPORT zSHORT OPERATION
TG_SetMultiSelect( zVIEW    vSubtask,
                    zCPCHAR cpcCtrlTag,
                    zLONG   lSet )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetMultiSelect","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      // there are 3 possible values for multi select
      // 0 - None         dbgMultiSelectNone
      // 1 - Simple     dbgMultiSelectSimple (default)
      // 2 - Extended    dbgMultiSelectExtended
      if( lSet < 0 || lSet > 2 )
      {
         TraceLineI( "Invalid setting for multiselect property", lSet );
         return( zCALL_ERROR );
      }
      pInstance->SetMultiSelect( lSet );
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetMultiSelect" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}



//./ ADD NAME=TG_SetAllowColSelect
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetAllowColSelect
//                                            10/9/2001 Modified
//  PURPOSE:    to permit column selection
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zBOOL   bSet       - TRUE if column selection is allowed
//                                   otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 4
zOPER_EXPORT zSHORT OPERATION
TG_SetAllowColSelect( zVIEW   vSubtask,
                      zCPCHAR cpcCtrlTag,
                      zBOOL   bSet )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetAllowColSelect","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      pInstance->SetAllowColSelect( bSet );
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetAllowColSelect" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetAllowRowSelect
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:      TG_SetAllowRowSelect
//                                            10/9/2001 Modified
//  PURPOSE:    to permit row selection
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zBOOL   bSet       - TRUE if row selection is allowed
//                                   otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 4
zOPER_EXPORT zSHORT OPERATION
TG_SetAllowRowSelect( zVIEW   vSubtask,
                      zCPCHAR cpcCtrlTag,
                      zBOOL   bSet )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetAllowRowSelect","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      pInstance->SetAllowRowSelect( bSet );
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetAllowRowSelect" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetAllowRowSizing
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:      TG_SetAllowRowSizing
//                                            10/9/2001 Modified
//  PURPOSE:    to permit row sizing selection
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zBOOL   bSet       - TRUE if row sizing is allowed
//                                   otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 4
zOPER_EXPORT zSHORT OPERATION
TG_SetAllowRowSizing( zVIEW   vSubtask,
                      zCPCHAR cpcCtrlTag,
                      zBOOL   bSet )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetAllowRowSizing","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      pInstance->SetAllowRowSizing( bSet );
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetAllowRowSelect" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetAllowFocus
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetAllowFocus
//                                            10/9/2001 Modified
//  PURPOSE:    to permit cursor setting to a specific column
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zLONG   nColumn    - zero based index to identify the column
//                                   specify -1 to affect all columns
//              zBOOL   bSet       - TRUE if row sizing is allowed
//                                   otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetAllowFocus( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  zLONG   nColumn,
                  zBOOL    bSet )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetAllowFocus","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set the color for all columns in the grid
      if( nColumn < 0 )
      {
         for ( zSHORT sCol = 0; sCol < nColumnCount; sCol++ )
         {
            Column col = cols.GetColumn( sCol );
            col.SetAllowFocus( bSet );
         }
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         col.SetAllowFocus( bSet );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetAllowFocus " );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetButtonAlways
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetButtonAlways
//                                            10/9/2001 Modified
//  PURPOSE:    to specify that a combobox will always show its dropdown button
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zLONG   nColumn    - zero based index to identify the column
//                                   specify -1 to affect all columns
//              zBOOL   bSet       - TRUE if row sizing is allowed
//                                   otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetButtonAlways( zVIEW   vSubtask,
                    zCPCHAR cpcCtrlTag,
                    zLONG   nColumn,
                    zLONG   bSet )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetButtonAllways","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn is negative set button for all columns in the grid
      if( nColumn < 0 )
      {
         for ( zSHORT sCol = 0; sCol < nColumnCount; sCol++ )
         {
            Column col = cols.GetColumn( sCol );
            col.SetButtonAlways( bSet );
         }
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT)nColumn );
         col.SetButtonAlways( bSet );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetButtonAlways" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}



//./ ADD NAME=TG_SetInactiveBackColor
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:      TG_SetInactiveBackColor
//                                            10/9/2001 Modified
//  PURPOSE:    to set the background color of a specific split if split
//              is inactive
//
//  PARAMETERS: zVIEW vSubtask      - View to the Dialog Window
//              zCPCHAR  cpcCtrlTag - Controls Tag
//              zLONG nSplit        - zero based index to identify the split
//                                    specify -1 to affect all splits
//              zLONG lRGBValue     - the color
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetInactiveBackColor( zVIEW   vSubtask,
                         zCPCHAR cpcCtrlTag,
                         zLONG   nSplit,
                         zLONG   lRGBValue )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetInactiveBackColor","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      LPDISPATCH pDispatch = pInstance->GetSplits();
      if( !pDispatch )
         return( zCALL_ERROR );

      Splits splits( pDispatch );

      zLONG nSplitCount = splits.GetCount();

      if(  nSplitCount < nSplit )// check against upper bounds
      {
         TraceLineS( "Index out of Range ", "TG_SetInactiveBackColor" );
         return( zCALL_ERROR );
      }

      if( nSplit < 0 ) // do it for all splits?
      {
         for( long lIndex = 0; lIndex < nSplitCount; lIndex++ )
         {
            Split split( splits.GetItem( lIndex ) );
            Columns cols( split.GetColumns() );
            for ( short i = 0; i < cols.GetCount(); i++ )
            {
               Column col = cols.GetColumn( i );
               short sColIndex = col.GetColIndex();
               ZGridColumn *pGC = pInstance->GetGridColumn(i);
               if( pGC )
                  pGC->SetLockColor( lRGBValue );
            }
            StyleDisp style( split.GetInactiveStyle() );
            style.SetBackColor( lRGBValue );
         }
      }
      else
      {
         Split split( splits.GetItem( nSplit ) ) ;
         Columns cols( split.GetColumns() );
         for ( short i = 0; i < cols.GetCount(); i++ )
         {
            Column col = cols.GetColumn( i );
            ZGridColumn *pGC = pInstance->GetGridColumn(i);
            if( pGC )
               pGC->SetLockColor( lRGBValue );
         }
         StyleDisp style( split.GetInactiveStyle() );
         style.SetBackColor( lRGBValue );
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetInactiveBackColor" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_SetInactiveForeColor
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_SetInactiveForeColor
//                                            10/9/2001 Modified
//  PURPOSE:    to set the foreground color of a specific split if split
//              is inactive
//
//  PARAMETERS: zVIEW vSubtask      - View to the Dialog Window
//              zCPCHAR  cpcCtrlTag - Controls Tag
//              zLONG nSplit        - zero based index to identify the split
//                                    specify -1 to affect all splits
//              zLONG lRGBValue     - the color
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetInactiveForeColor( zVIEW   vSubtask,
                         zCPCHAR cpcCtrlTag,
                         zLONG   nSplit,
                         zLONG   lRGBValue )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetInactiveForeColor","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      LPDISPATCH pDispatch = pInstance->GetSplits();
      if( !pDispatch )
         return( zCALL_ERROR );

      Splits splits( pDispatch );

      zLONG nSplitCount = splits.GetCount();

      if(  nSplitCount < nSplit )// check against upper bounds
      {
         TraceLineS( "Index out of Range ", "TG_SetInactiveForeColor" );
         return( zCALL_ERROR );
      }

      if( nSplit < 0 ) // do it for all splits?
      {
         for( long lIndex = 0; lIndex < nSplitCount; lIndex++ )
         {
            Split split( splits.GetItem( lIndex ) ) ;
            StyleDisp style( split.GetInactiveStyle() );
            style.SetForeColor( lRGBValue );
         }
      }
      else
      {
         Split split( splits.GetItem( nSplit ) ) ;
         StyleDisp style( split.GetInactiveStyle() );
         style.SetForeColor( lRGBValue );
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_SetInactiveForeColor" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}



//./ ADD NAME=TG_GetCaption
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_GetCaption
//                                            10/9/2001 Modified
//  PURPOSE:    to get the caption for a specific column or the grid
//
//  PARAMETERS: zVIEW vSubtask      - View to the Dialog Window
//              zCPCHAR  cpcCtrlTag - Controls Tag
//              zLONG nColumn       - zero based index to identify the column
//                                    specify -1 to set the grids caption
//              zPCHAR pszCaption   - the text buffer receiving the cation
//              zULONG ulLth        - size of the text buffer
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 6
zOPER_EXPORT zSHORT OPERATION
TG_GetCaption( zVIEW    vSubtask,
               zCPCHAR  cpcCtrlTag,
               zLONG    nColumn,
               zPCHAR   pchCaption,
               zULONG   ulLth )
{
   zSHORT   nRC = 0;
   CString  csCaption = "";
#ifdef DEBUG_ALL
   TraceLineS("TG_GetCaption","");
#endif

   pchCaption[0] = 0;

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   TRY
   {
      Columns cols ( pInstance->GetColumns() );
      zLONG nColumnCount = cols.GetCount();

      // if nColumn < 0 get grids caption
      if( nColumn < 0 )
      {
         csCaption = pInstance->GetCaption();
         zstrncat( pchCaption, csCaption, ulLth );
      }
      else
      if( nColumnCount > nColumn )
      {
         Column col = cols.GetColumn( (zSHORT) nColumn );
         csCaption = col.GetCaption();
         zstrncat( pchCaption, csCaption, ulLth );
      }
      else
      {
         nRC = zCALL_ERROR;
      }
   }
   CATCH_ALL ( e )
   {
      TraceExceptionError( e, "TG_GetCaption" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

  return nRC;
}



//./ ADD NAME=TG_ShowColumn
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:      TG_ShowColumn
//                                            10/9/2001 Modified
//  PURPOSE:    to specify that a column is hidden or shown
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zLONG   nColumn    - zero based index to identify the column
//                                   specify -1 to affect all columns
//              zBOOL   bSet       - TRUE to hide a column otherwise FALSE
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_ShowColumn( zVIEW vSubtask,
               zCPCHAR  cpcCtrlTag,
               zLONG nColumn,
               zBOOL bShow )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_ShowColumn","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   Columns cols ( pInstance->GetColumns() );
   TRY
   {
      zLONG nColumnCount = cols.GetCount();
      if (  nColumnCount < nColumn )
      {
         TraceLineS("Invalid Column index ", "TG_ShowColumn" );
         return nRC;
      }

      if ( nColumn >= 0 )
      {
         Column col = cols.GetColumn( (zSHORT) nColumn );
         col.SetVisible( bShow );
         nRC = 0;
      }
      else
      {
         for ( zSHORT i = 0; i < nColumnCount; i++ )
         {
            Column col = cols.GetColumn( i );
            col.SetVisible( bShow );
         }
      }
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "TG_ShowColumn" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}



//./ ADD NAME=TG_LimitTextLen
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:       TG_LimitTextLen
//                                            10/9/2001 Modified
//  PURPOSE:    to limit the inputlength for a specific column
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zLONG   nColumn    - zero based index to identify the column
//                                   specify -1 to affect all columns
//              zLONG   nLength    - maximum number of characters to be entered
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_LimitTextLen( zVIEW vSubtask,
                 zCPCHAR  cpcCtrlTag,
                 zLONG nColumn,
                 zLONG nLength )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_LimitTextLen","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   Columns cols ( pInstance->GetColumns() );
   TRY
   {
      zLONG nColumnCount = cols.GetCount();
      if (  nColumnCount < nColumn )
      {
         TraceLineS("Invalid Column index ", "TG_LimitTextLen" );
         return nRC;
      }

      if ( nColumn >= 0 )
      {
         Column col = cols.GetColumn( (zSHORT) nColumn );
         col.SetDataWidth( nLength );
         nRC = 0;
      }
      else
      {
         for ( zSHORT i = 0; i < nColumnCount; i++ )
         {
            Column col = cols.GetColumn( i );
            col.SetDataWidth( nLength );
         }
      }
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "TG_LimitTextLen" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}


//./ ADD NAME=TG_LimitTextLen
// Source Module=trgrxprt.cpp
//**************************************************************************/
//
//  ENTRY:      TG_SetDateCtrlForColumn
//                                            10/9/2001 Modified
//  PURPOSE:    specify a column that should be displayed as
//              date time picker
//
//  PARAMETERS: zVIEW   vSubtask   - View to the Dialog Window
//              zCPCHAR cpcCtrlTag - Controls Tag
//              zLONG   nColumn    - zero based index to identify the column
//              zBOOL   bSet       - on / off
//
//  RETURNS   :  0 - if succesful otherwise zCALL_ERROR
/**************************************************************************/
//./END + 5
zOPER_EXPORT zSHORT OPERATION
TG_SetDateCtrlForColumn( zVIEW vSubtask,
                 zCPCHAR  cpcCtrlTag,
                 zLONG nColumn,
                 zBOOL bSet )
{
   zSHORT nRC = 0;
#ifdef DEBUG_ALL
   TraceLineS("TG_SetDateCtrlForColumn","");
#endif

   ITrueDBGridCtrl * pInstance  = (ITrueDBGridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( zCALL_ERROR );

   if ( nColumn < 0 )
      return( zCALL_ERROR );

   TRY
   {
      nRC = pInstance->SetDateCtrlForColumn( nColumn, bSet );
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "TG_SetDateCtrlForColumn" );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return nRC;
}

#ifdef __cplusplus
}
#endif


