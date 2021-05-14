//----------------------------------------------------------------------
//
// .Name:         TZADWEXP.C
//
// .Version:      1.0
//
// .Last change:  11-06-2000
//
// .Description:  Zeidon operations
//
//----------------------------------------------------------------------
// .Change notes
//
//  1.0  = New
//        (11-06-2000) Zeidon
//
//  2000.06.11  BL  Z10
//
//        Added ADW_Exp_ReplaceHex, ReplaceSpecialCharacter for ADW
//        Export only
//
//----------------------------------------------------------------------

#include <windows.h>

#define KZSYSSVC_INCL
#include <KZOENGAA.H>
#include <ZDRVROPR.H>


#ifdef __cplusplus
extern "C"
{
#endif

#include "ZEIDONOP.H"

#include <stdlib.h>
#include <string.h>
#include <io.h>


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: ADW_Exp_ReplaceHex
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*GLOBAL */  OPERATION
ADW_Exp_ReplaceHex( zPCHAR  pszTarget,
                    zULONG  ulTargetLen,
                    zPCHAR  pszSearch,
                    zPCHAR  pszReplace )
{

   zPCHAR  pszPos;
   zULONG  ulOffs;
   zULONG  ulOrigLen;
   zULONG  ulSearchLen;
   zULONG  ulReplaceLen;
   zLONG   lLenDiff;
   zUSHORT usCountReplaces;
   zCHAR Suche[ 3 ] = { 0x0d, 0x0a, 0x00 };
   zCHAR Ersetze[ 69 ] = { ' ', 0x00 };

   pszPos = pszTarget;
   usCountReplaces = 0;

   pszReplace = Ersetze;
   pszSearch = Suche;

   ulOrigLen    = strlen( pszTarget );
   ulSearchLen  = strlen( pszSearch );
   ulReplaceLen = strlen( pszReplace );

   lLenDiff     = ulReplaceLen - ulSearchLen;

   do
   {
      pszPos = strstr( pszPos, pszSearch );
      if ( pszPos == NULL )   // Searchstring nicht vorhanden
      {
         break;
      }

      ulOffs = (zULONG) (pszPos - pszTarget);
      // bei jedem Durchlauf Neuberechnung, da sich bei jedem Replace
      // die Laenge aendert

      if ( ulOrigLen + lLenDiff < ulTargetLen )
      {
         // Normalfall Target ist gross genug fuer replace
         memmove( pszPos + ulReplaceLen, pszPos + ulSearchLen,
                  ulOrigLen - ulOffs - ulSearchLen + 1 );
                 // +1 wegen dem abschliessenden 0
         memcpy( pszPos, pszReplace, ulReplaceLen );
         ulOrigLen += lLenDiff;
      }
      else
      {
         // Target ist nicht gross genug, ein Teil wird hinten abgeschnitten
         if ( ulOffs + ulReplaceLen < ulTargetLen )
         {
            // Der Replace String passt noch komplett in Target
            memmove( pszPos + ulReplaceLen, pszPos + ulSearchLen,
                     ulTargetLen - ulOffs - ulReplaceLen - 1 );
            memcpy( pszPos, pszReplace, ulReplaceLen );
         }
         else
         {
            // Der Replace String passt nicht komplett in Target
            memcpy( pszPos, pszReplace, ulTargetLen - ulOffs - 1 );
         }

         ulOrigLen = ulTargetLen - 1;
         pszTarget[ulTargetLen] = 0;
      }

      usCountReplaces++;
      pszPos += ulReplaceLen; //Weitersuchen ab dem Ende des letzten Replace

      if (pszPos >= (pszTarget + ulTargetLen - 1))
         break;

   } while( TRUE );

   return( usCountReplaces );

} // ADW_Exp_ReplaceHex

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: ReplaceSpecialCharacter
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /* GLOBAL */  OPERATION
ReplaceSpecialCharacter( zPCHAR pszTarget,
                         zULONG ulTargetLen,
                         zPCHAR pszSearch,
                         zPCHAR pszReplace )

{
  zPCHAR  pszPos;
  zULONG  ulOffs;
  zULONG  ulOrigLen;
  zULONG  ulSearchLen;
  zULONG  ulReplaceLen;
  zLONG   lLenDiff;
  zUSHORT usCountReplaces;

  pszPos = pszTarget;
  usCountReplaces = 0;

  ulOrigLen    = strlen( pszTarget );
  ulSearchLen  = strlen( pszSearch );
  ulReplaceLen = strlen( pszReplace );

  lLenDiff     = ulReplaceLen - ulSearchLen;

  do
  {

    pszPos = strstr( pszPos, pszSearch );
    if ( pszPos == NULL )   // Searchstring nicht vorhanden
    {
      break;
    }

    ulOffs = (zULONG) (pszPos - pszTarget);

    // bei jedem Durchlauf Neuberechnung, da sich bei jedem Replace
    // die Laenge aendert

    if ( ulOrigLen + lLenDiff < ulTargetLen )
    {
      // Normalfall Target ist gross genug fuer replace
      memmove( pszPos + ulReplaceLen, pszPos + ulSearchLen,
               ulOrigLen - ulOffs - ulSearchLen + 1 );
              // +1 wegen dem abschliessenden 0
      memcpy( pszPos, pszReplace, ulReplaceLen );
      ulOrigLen += lLenDiff;
    }
    else
    {
      // Target ist nicht gross genug, ein Teil wird hinten abgeschnitten
      if ( ulOffs + ulReplaceLen < ulTargetLen )
      {
        // Der Replace String passt noch komplett in Target
        memmove( pszPos + ulReplaceLen, pszPos + ulSearchLen,
                 ulTargetLen - ulOffs - ulReplaceLen - 1 );
        memcpy( pszPos, pszReplace, ulReplaceLen);
      }
      else
      {
        // Der Replace String passt nicht komplett in Target
        memcpy( pszPos, pszReplace, ulTargetLen - ulOffs - 1 );
      }
      ulOrigLen = ulTargetLen - 1;
      pszTarget[ ulTargetLen ] = 0;
    }
    usCountReplaces++;

    pszPos += ulReplaceLen; //Weitersuchen ab dem Ende des letzten Replace

    if ( pszPos >= (pszTarget + ulTargetLen - 1) )
      break;

  } while( TRUE );

  return( 0 );

} // ReplaceSpecialCharacter



/*************************************************************************************************
**
**    OPERATION: GetBorderLineWidth
**
*************************************************************************************************/
zOPER_EXPORT zLONG /*GLOBAL */  OPERATION
GetBorderLineWidth( zLONG lBorderStyle,
                    zPCHAR szWidth )
{

  TraceLineS( "*** In GetBorderLineWidth *****", "" );


   lBorderStyle >>= 24;
   zltoa( lBorderStyle, szWidth, zsizeof( szWidth ) );
  TraceLineS( "*** szWidth *****", szWidth );

   return( 0 );
} // GetBorderLineWidth




/*************************************************************************************************
**
**    OPERATION: SetRptShapeFromBlob
**
*************************************************************************************************/
zOPER_EXPORT zLONG /*GLOBAL */  OPERATION
SetRptShapeFromBlob( zVIEW vReportDef )
{

   zCHAR   szBlob[ 8 + (6 * sizeof( zLONG )) ];
// zVIEW   vReport;
// zLONG   lSubtype;
   zULONG  ulPenColor = 0xFF000000;
   zULONG  ulShapeColor = 0xFF000000;
   zULONG  ulLth = zsizeof( szBlob );

   zmemset( szBlob, 0, ulLth );
   GetBlobFromAttribute( szBlob, &ulLth, vReportDef, "Control", "CtrlBOI" );
// TraceLineI( "Shape control blob lth = ", ulLth );
   if ( ulLth == zsizeof( szBlob ) ||
        ulLth == zsizeof( szBlob ) - (1 * sizeof( zLONG )) || // remove this soon
        ulLth == zsizeof( szBlob ) - (2 * sizeof( zLONG )) )
   {
      zLONG   lPenWidth;
      zLONG   lPenType;
      zLONG   lRoundValue;
      zLONG   lFlags = 0;

      if ( szBlob[ 4 ] == TRUE )
      {
         TraceLineI( "****   Horizontal Line    **** ", szBlob[ 4 ] );
         SetAttributeFromInteger( vReportDef, "Control", "BorderBottom", szBlob[ 4 ] );
      }

/*
      SetCtrlState( vSubtask, "Rectangle", zCONTROL_STATUS_CHECKED,
                    szBlob[ 0 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "RoundRect", zCONTROL_STATUS_CHECKED,
                    szBlob[ 1 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Ellipse", zCONTROL_STATUS_CHECKED,
                    szBlob[ 2 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Diamond", zCONTROL_STATUS_CHECKED,
                    szBlob[ 3 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "HorizontalLine", zCONTROL_STATUS_CHECKED,
                    szBlob[ 4 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "VerticalLine", zCONTROL_STATUS_CHECKED,
                    szBlob[ 5 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "DiagonalDescend", zCONTROL_STATUS_CHECKED,
                    szBlob[ 6 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "DiagonalAscend", zCONTROL_STATUS_CHECKED,
                    szBlob[ 7 ] ? TRUE : FALSE );
*/

      lPenWidth = *((zPLONG) (szBlob + 8));
      lPenType = *((zPLONG) (szBlob + 8 + sizeof( zLONG )));
      ulPenColor = *((zPLONG) (szBlob + 8 + (2 * sizeof( zLONG ))));
      lRoundValue = *((zPLONG) (szBlob + 8 + (3 * sizeof( zLONG ))));
      if ( ulLth == zsizeof( szBlob ) )
      {
         ulShapeColor = *((zPULONG) (szBlob + 8 + (4 * sizeof( zLONG ))));
         lFlags = *((zPLONG) (szBlob + 8 + (5 * sizeof( zLONG ))));
      }

      zltoa( ulPenColor, szBlob, zsizeof( szBlob ) );
      SetAttributeFromString( vReportDef, "Control", "wLineColorText", szBlob );
      SetAttributeFromInteger( vReportDef, "Control", "wLineColorInt", ulPenColor );
      SetAttributeFromInteger( vReportDef, "Control", "BorderWidth", lPenWidth );
      switch ( lPenType )
      {
         case 1:
            SetAttributeFromInteger( vReportDef, "Control", "BorderStyle", 1 );
            break;

         case 2:
            SetAttributeFromInteger( vReportDef, "Control", "BorderStyle", 2 );
            break;

         case 3:
            SetAttributeFromInteger( vReportDef, "Control", "BorderStyle", 3 );
            break;

         case 4:
            SetAttributeFromInteger( vReportDef, "Control", "BorderStyle", 4 );
            break;

         default:
            SetAttributeFromInteger( vReportDef, "Control", "BorderStyle", 0 );
            break;
      }


      zltoa( lRoundValue, szBlob, zsizeof( szBlob ) );
      //SetCtrlText( vSubtask, "RoundValue", szBlob );
   }
   else
   {
      //SetCtrlState( vSubtask, "Rectangle", zCONTROL_STATUS_CHECKED, TRUE );
      //SetCtrlText( vSubtask, "PenWidth", "1" );
   }

   /*
   SetCtrlProperty( vSubtask, "__ColorLine", zCONTROL_PROPERTY_INTEGER_DATA, ulPenColor, 0 );
   SetCtrlProperty( vSubtask, "__ColorShape", zCONTROL_PROPERTY_INTEGER_DATA, ulShapeColor, 0 );
   */

   return( 0 );
} // SetRptShapeFromBlob


#ifdef __cplusplus
}
#endif


