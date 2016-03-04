//
// MODULE NAME:  KZOENETA  -  Object services Network Handling Operations
// DESCRIPTION:  This is the source file which contains Object services
//               operations for all network functions.  These functions make
//               calls to the OS and network specific Network Handler.
// OPERATIONS:   All OPERATIONS which do network handling (eg Net...)
// **********************************************************************
// * Copyright (c) 1993-2016 QuinSoft, Inc. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  D.G. Christensen
//  DATE:    3/22/96
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2002.01.28  HH
   added SysRoundDecimal.

2000.12.20  HH
   const char change.

1999.08.25  TMV    Z10
   fix function CommitMultipleOIs to make Z10 compileable

1999.08.24  BL    Z10
   Remove function in if 0
   function CommitMultipleOIs: insert zOPER_EXPORT and OPERATION

1999.08.20  BL    ALL
   Move function code from h- into cpp-file

1998.08.09  HH
  move function bodies over from kzoedeca.h.

1998.06.09    RG
   Based ZDecimal on ZDECIMAL.

1998.05.14    DGC
   Moved Sys... decimal operations here from kzoesyaa.c.

*/

#define KZOESYS_INCL
#define AFX_CLASS_EXPORT    __declspec(dllexport)
#define AFX_EXT_CLASS       AFX_CLASS_EXPORT
#define ZDECIMAL_CLASS      AFX_EXT_CLASS
#include "kzoephdr.h"
#include "zdecimal.h"

// ====================================================================
// Constructors
// ====================================================================

ZDecimal::ZDecimal( )
{
   SysAssignDecimalFromNull( this );
}

ZDecimal::ZDecimal( zDECIMAL zDec )
{
   zDECIMAL *pDEC = this;
   *pDEC = zDec;
// this->dDecValue = zDec;
}

ZDecimal::ZDecimal( zPDECIMAL zDec )
{
   zDECIMAL *pDEC = this;
   *pDEC = *zDec;
// this->dDecValue = *zDec;
}

ZDecimal::ZDecimal( zPCHAR pszStr )
{
   SysConvertStringToDecimal( pszStr, this );
}

ZDecimal::ZDecimal( zCPCHAR pszStr )
{
   SysConvertStringToDecimal( pszStr, this );
}

ZDecimal::ZDecimal( zLONG l )
{
   SysConvertLongToDecimal( l, this );
}

ZDecimal::ZDecimal( zULONG l )
{
   SysConvertLongToDecimal( (zULONG) l, this );
}

ZDecimal::ZDecimal( zSHORT n )
{
   SysConvertLongToDecimal( (zLONG) n, this );
}

ZDecimal::ZDecimal( zUSHORT n )
{
   SysConvertLongToDecimal( (zLONG) n, this );
}

ZDecimal::ZDecimal( double d )
{
   SysConvertDoubleToDecimal( d, this );
}

ZDecimal::ZDecimal( int k )
{
   SysConvertLongToDecimal( (zLONG) k, this );
}

ZDecimal::ZDecimal( unsigned int k )
{
   SysConvertLongToDecimal( (zLONG) k, this );
}

ZDecimal::~ZDecimal( )
{
}

// ====================================================================
// Assignment to ZDecimal from variable
// ====================================================================

ZDecimal
ZDecimal::operator = ( ZDecimal zDec )
{
   SysAssignDecimalFromDecimal( this, &zDec );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zDECIMAL zDEC )
{
   SysAssignDecimalFromDecimal( this, &zDEC );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zPDECIMAL zPDEC )
{
   SysAssignDecimalFromDecimal( this, zPDEC );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zPVOID pv )
{
   SysAssignDecimalFromDecimal( this, (zPDECIMAL) pv );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zLONG l )
{
   SysConvertLongToDecimal( l, this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zULONG l )
{
   SysConvertLongToDecimal( (zLONG) l, this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zSHORT n )
{
   SysConvertLongToDecimal( (zLONG) n, this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zUSHORT n )
{
   SysConvertLongToDecimal( (zLONG) n, this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( int i )
{
   SysConvertLongToDecimal( (zLONG) i, this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( unsigned int i )
{
   SysConvertLongToDecimal( (zLONG) i, this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( double d )
{
   SysConvertDoubleToDecimal( d, this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( const double d ) const
{
   SysConvertDoubleToDecimal( (double) d, (zPDECIMAL) this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zPCHAR pszStr )
{
   SysConvertStringToDecimal( pszStr, this );
   return( *this );
}

ZDecimal
ZDecimal::operator = ( zCPCHAR pszStr )
{
   SysConvertStringToDecimal( pszStr, this );
   return( *this );
}

// ====================================================================
// Assignment to variable from ZDecimal
// ====================================================================

ZDecimal::operator double( )
{
   double d;

   SysConvertDecimalToDouble( this, &d );
   return( d );
}

ZDecimal::operator zLONG( )
{
   zLONG l;

   SysConvertDecimalToLong( this, &l );
   return( l );
}

ZDecimal::operator zULONG( )
{
   zULONG l;

   SysConvertDecimalToLong( this, (zPLONG) &l );
   return( l );
}

ZDecimal::operator zSHORT( )
{
   zLONG l;

   SysConvertDecimalToLong( this, &l );
   return( (zSHORT) l );
}

ZDecimal::operator zUSHORT( )
{
   zLONG l;

   SysConvertDecimalToLong( this, &l );
   return( (zUSHORT) l );
}

ZDecimal::operator zPCHAR()
{
   SysConvertDecimalToString( this, this->szValue, 10 );
   return( this->szValue );
}

// ====================================================================
// Decimal only comparitors
// ====================================================================

zBOOL
ZDecimal::IsNull( )
{
   return( SysCompareDecimalToNull( this ) == 0 );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec1, ZDecimal zDec2 )
{
   return( SysCompareDecimalToDecimal( &zDec1, &zDec2 ) <= 0 );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec1, ZDecimal zDec2 )
{
   return( SysCompareDecimalToDecimal( &zDec1, &zDec2 ) < 0 );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec1, ZDecimal zDec2 )
{
   return( SysCompareDecimalToDecimal( &zDec1, &zDec2 ) >= 0 );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec1, ZDecimal zDec2 )
{
   return( SysCompareDecimalToDecimal( &zDec1, &zDec2 ) > 0 );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec1, ZDecimal zDec2 )
{
   return( SysCompareDecimalToDecimal( &zDec1, &zDec2 ) == 0 );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec1, ZDecimal zDec2 )
{
   return( SysCompareDecimalToDecimal( &zDec1, &zDec2 ) != 0 );
}

zOPER_EXPORT zCHAR OPERATION
operator ! ( ZDecimal zDec )
{
   ZDecimal zTemp = 0;

   return( SysCompareDecimalToDecimal( &zTemp, &zDec ) == 0 );
}

// ====================================================================
// Addition operators involving mixed types
// ====================================================================

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, ZDecimal zDec2 )
{
   ZDecimal zTemp;

   SysAddDecimalToDecimal( &zTemp, &zDec, &zDec2 );
   return( zTemp );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, double d )
{
   ZDecimal zDecD;
   zDecD = d;
   return( zDecD + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( double d, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = d;
   return( zDecD + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( ZDecimal zDec, float d )
{
   ZDecimal zDecD;
   zDecD = (double) d;
   return( zDecD + zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator + ( float d, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) d;
   return( zDecD + zDec );
}

// ====================================================================
// Subtraction operators
// ====================================================================

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, ZDecimal zDec2 )
{
   ZDecimal zTemp;

   SysSubtractDecimalFromDecimal( &zTemp, &zDec, &zDec2 );
   return( zTemp );
}

zOPER_EXPORT ZDecimal
OPERATION operator - ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec - zDecL );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec - zDecL );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec - zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec - zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec - zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec - zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec - zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec - zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, double d )
{
   ZDecimal zDecD;
   zDecD = d;
   return( zDec - zDecD );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( double d, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = d;
   return( zDecD - zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( ZDecimal zDec, float d )
{
   ZDecimal zDecD;
   zDecD = (double) d;
   return( zDec - zDecD );
}

zOPER_EXPORT ZDecimal OPERATION
operator - ( float d, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) d;
   return( zDecD - zDec );
}

// ====================================================================
// Multiplication operators
// ====================================================================

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, ZDecimal zDec2 )
{
   ZDecimal zTemp;

   SysMultiplyDecimalByDecimal( &zTemp, &zDec, &zDec2 );
   return( zTemp );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec * zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec * zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, double d )
{
   ZDecimal zDecD;
   zDecD = d;
   return( zDecD * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( double d, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = d;
   return( zDecD * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( ZDecimal zDec, float d )
{
   ZDecimal zDecD;
   zDecD = (double) d;
   return( zDecD * zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator * ( float d, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) d;
   return( zDecD * zDec );
}

// ====================================================================
// Division operators
// ====================================================================

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, ZDecimal zDec2 )
{
   ZDecimal zTemp;

   SysDivideDecimalByDecimal( &zTemp, &zDec, &zDec2 );

   return( zTemp );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec / zDecL );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec / zDecL );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec / zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec / zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec / zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec / zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec / zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec / zDecN );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, double d )
{
   ZDecimal zDecD;
   zDecD = d;
   return( zDec / zDecD );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( double d, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = d;
   return( zDecD / zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( ZDecimal zDec, float d )
{
   ZDecimal zDecD;
   zDecD = (double) d;
   return( zDec / zDecD );
}

zOPER_EXPORT ZDecimal OPERATION
operator / ( float d, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) d;
   return( zDecD / zDec );
}

// ====================================================================
// ">=" operators involving mixed types
// ====================================================================

zOPER_EXPORT zCHAR OPERATION
operator >= ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec >= zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec >= zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec >= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec >= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec >= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec >= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec >= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec >= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( double n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDecD >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, double n )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDec >= zDecD );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( float n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDecD >= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator >= ( ZDecimal zDec, float n )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDec >= zDecD );
}

// ====================================================================
// ">" operators involving mixed types
// ====================================================================

zOPER_EXPORT zCHAR OPERATION
operator > ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec > zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec > zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec > zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec > zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec > zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec > zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec > zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec > zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( double n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDecD > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, double n )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDec > zDecD );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( float n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDecD > zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator > ( ZDecimal zDec, float n )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDec > zDecD );
}

// ====================================================================
// "<=" operators involving mixed types
// ====================================================================

zOPER_EXPORT zCHAR OPERATION
operator <= ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec <= zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec <= zDecL );
}


zOPER_EXPORT zCHAR OPERATION operator <= ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec <= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec <= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec <= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec <= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec <= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec <= zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( double n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDecD <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, double n )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDec <= zDecD );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( float n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDecD <= zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator <= ( ZDecimal zDec, float n )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDec <= zDecD );
}

// ====================================================================
// "<" operators involving mixed types
// ====================================================================

zOPER_EXPORT zCHAR OPERATION
operator < ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec < zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec < zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec < zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec < zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec < zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec < zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec < zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec < zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( double n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDecD < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, double n )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDec < zDecD );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( float n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDecD < zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator < ( ZDecimal zDec, float n )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDec < zDecD );
}

// ====================================================================
// "==" operators involving mixed types
// ====================================================================

zOPER_EXPORT zCHAR OPERATION
operator == ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec == zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec == zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec == zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec == zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec == zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec == zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec == zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec == zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( double n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDecD == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, double n )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDec == zDecD );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( float n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDecD == zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator == ( ZDecimal zDec, float n )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDec == zDecD );
}

// ====================================================================
// "!=" operators involving mixed types
// ====================================================================

zOPER_EXPORT zCHAR OPERATION
operator != ( zLONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec != zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( zULONG l, ZDecimal zDec )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDecL != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   return( zDec != zDecL );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( zSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec != zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( zUSHORT n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDecN != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   return( zDec != zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec != zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDec != zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( unsigned int n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zLONG) n;
   return( zDecN != zDec );
}

zOPER_EXPORT zCHAR OPERATION operator != ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec != zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( zCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( zUCHAR n, ZDecimal zDec )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDecN != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = (zSHORT) n;
   return( zDec != zDecN );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( double n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDecD != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, double n )
{
   ZDecimal zDecD;
   zDecD = n;
   return( zDec != zDecD );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( float n, ZDecimal zDec )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDecD != zDec );
}

zOPER_EXPORT zCHAR OPERATION
operator != ( ZDecimal zDec, float n )
{
   ZDecimal zDecD;
   zDecD = (double) n;
   return( zDec != zDecD );
}

// ====================================================================
// "+=" operators
// ====================================================================

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal& zDec1, ZDecimal zDec2 )
{
   zDec1 = zDec1 + zDec2;
   return( zDec1 );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   zDec = zDec + zDecL;
   return( zDec );
}

zOPER_EXPORT zLONG OPERATION
operator += ( zLONG & l, ZDecimal zDec )
{
   l = l + (zLONG) zDec;
   return( l );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   zDec = zDec + zDecL;
   return( zDec );
}

zOPER_EXPORT zULONG OPERATION
operator += ( zULONG l, ZDecimal zDec )
{
   l = l + (zULONG) zDec;
   return( l );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec + zDecN;
   return( zDec );
}

zOPER_EXPORT zSHORT OPERATION
operator += ( zSHORT n, ZDecimal zDec )
{
   n = n + (zSHORT) zDec;
   return( n );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec + zDecN;
   return( zDec );
}

zOPER_EXPORT zUSHORT OPERATION
operator += ( zUSHORT n, ZDecimal zDec )
{
   n = n + (zUSHORT) zDec;
   return( n );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec + zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec + zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec + zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec + zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator += ( ZDecimal zDec, double d )
{
   ZDecimal zDecD;
   zDecD = d;
   zDec = zDec + zDecD;
   return( zDec );
}

// ====================================================================
// "-=" operators
// ====================================================================

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal& zDec1, ZDecimal zDec2 )
{
   zDec1 = zDec1 - zDec2;
   return( zDec1 );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   zDec = zDec - zDecL;
   return( zDec );
}

zOPER_EXPORT zLONG OPERATION
operator -= ( zLONG & l, ZDecimal zDec )
{
   l = l - (zLONG) zDec;
   return( l );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   zDec = zDec - zDecL;
   return( zDec );
}

zOPER_EXPORT zULONG OPERATION
operator -= ( zULONG l, ZDecimal zDec )
{
   l = l - (zULONG) zDec;
   return( l );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec - zDecN;
   return( zDec );
}

zOPER_EXPORT zSHORT OPERATION
operator -= ( zSHORT n, ZDecimal zDec )
{
   n = n - (zSHORT) zDec;
   return( n );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec - zDecN;
   return( zDec );
}

zOPER_EXPORT zUSHORT OPERATION
operator -= ( zUSHORT n, ZDecimal zDec )
{
   n = n - (zUSHORT) zDec;
   return( n );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec - zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec - zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec - zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec - zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator -= ( ZDecimal zDec, double d )
{
   ZDecimal zDecD;
   zDecD = d;
   zDec = zDec - zDecD;
   return( zDec );
}

// ====================================================================
// "*=" operators
// ====================================================================

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal& zDec1, ZDecimal zDec2 )
{
   zDec1 = zDec1 * zDec2;
   return( zDec1 );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   zDec = zDec - zDecL;
   return( zDec );
}

zOPER_EXPORT zLONG OPERATION
operator *= ( zLONG & l, ZDecimal zDec )
{
   l = l * (zLONG) zDec;
   return( l );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   zDec = zDec * zDecL;
   return( zDec );
}

zOPER_EXPORT zULONG OPERATION
operator *= ( zULONG l, ZDecimal zDec )
{
   l = l * (zULONG) zDec;
   return( l );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec * zDecN;
   return( zDec );
}

zOPER_EXPORT zSHORT OPERATION
operator *= ( zSHORT n, ZDecimal zDec )
{
   n = n * (zSHORT) zDec;
   return( n );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec * zDecN;
   return( zDec );
}

zOPER_EXPORT zUSHORT OPERATION
operator *= ( zUSHORT n, ZDecimal zDec )
{
   n = n * (zUSHORT) zDec;
   return( n );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec * zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec * zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec * zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec * zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator *= ( ZDecimal zDec, double d )
{
   ZDecimal zDecD;
   zDecD = d;
   zDec = zDec * zDecD;
   return( zDec );
}

// ====================================================================
// "/=" operators
// ====================================================================

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal& zDec1, ZDecimal zDec2 )
{
   zDec1 = zDec1 / zDec2;
   return( zDec1 );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, zLONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   zDec = zDec / zDecL;
   return( zDec );
}

zOPER_EXPORT zLONG OPERATION
operator /= ( zLONG & l, ZDecimal zDec )
{
   l = l / (zLONG) zDec;
   return( l );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, zULONG l )
{
   ZDecimal zDecL;
   zDecL = l;
   zDec = zDec / zDecL;
   return( zDec );
}

zOPER_EXPORT zULONG OPERATION
operator /= ( zULONG l, ZDecimal zDec )
{
   l = l / (zULONG) zDec;
   return( l );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, zSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec / zDecN;
   return( zDec );
}

zOPER_EXPORT zSHORT OPERATION
operator /= ( zSHORT n, ZDecimal zDec )
{
   n = n / (zSHORT) zDec;
   return( n );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, zUSHORT n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec / zDecN;
   return( zDec );
}

zOPER_EXPORT zUSHORT OPERATION
operator /= ( zUSHORT n, ZDecimal zDec )
{
   n = n / (zUSHORT) zDec;
   return( n );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, int n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec / zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, unsigned int n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec / zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, zCHAR n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec / zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, zUCHAR n )
{
   ZDecimal zDecN;
   zDecN = n;
   zDec = zDec / zDecN;
   return( zDec );
}

zOPER_EXPORT ZDecimal OPERATION
operator /= ( ZDecimal zDec, double d )
{
   ZDecimal zDecD;
   zDecD = d;
   zDec = zDec / zDecD;
   return( zDec );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// ARACrypt.cpp : implementation file
//
// Note: A Special Thanks to Mr. Warren Ward for his Sept. 1998 CUJ article:
// "Stream Encryption" Copyright (c) 1998 by Warren Ward

/////////////////////////////////////////////////////////////////////////////
// ARACrypt

ARACrypt::ARACrypt( ) :

   // Initialize the shift registers to non-zero
   // values. If the shift register contains all
   // 0's when the generator starts, it will not
   // produce a usable sequence of bits. The
   // numbers used here have no special features
   // except for being non-zero.

       // m_LFSR_A( 0x13579BDF ),
       // m_LFSR_B( 0x2468ACE0 ),
       // m_LFSR_C( 0xFDB97531 ),
          m_LFSR_A( 0x3B5169DF ),
          m_LFSR_B( 0xD2758CE3 ),
          m_LFSR_C( 0xF40B971A ),


   // Initialize the masks to magic numbers.
   // These values are primitive polynomials mod
   // 2, described in Applied Cryptography,
   // second edition, by Bruce Schneier (New York:
   // John Wiley and Sons, 1994). See Chapter 15:
   // Random Sequence Generators and Stream
   // Ciphers, particularly the discussion on
   // Linear Feedback Shift Registers.
   //
   // The primitive polynomials used here are:
   // Register A: ( 32, 7, 6, 2, 0 )
   // Register B: ( 31, 6, 0 )
   // Register C: ( 29, 2, 0 )
   //
   // The bits that must be set to "1" in the
   // XOR masks are:
   // Register A: ( 31, 6, 5, 1 )
   // Register B: ( 30, 5 )
   // Register C: ( 28, 1 )
   //
   // Developer's Note
   // DO NOT CHANGE THESE NUMBERS WITHOUT
   // REFERRING TO THE DISCUSSION IN SCHNEIER'S
   // BOOK. They are some of very few
   // near-32-bit values that will act as
   // maximal-length random generators.

          m_Mask_A( 0x80000062 ),
          m_Mask_B( 0x40000020 ),
          m_Mask_C( 0x10000002 ),


   // Set up LFSR "rotate" masks.
   // These masks limit the number of bits
   // used in the shift registers. Each one
   // provides the most-significant bit (MSB)
   // when performing a "rotate" operation. Here
   // are the shift register sizes and the byte
   // mask needed to place a "1" bit in the MSB
   // for Rotate-1, and a zero in the MSB for
   // Rotate-0. All the shift registers are stored
   // in an unsigned 32-bit integer, but these
   // masks effectively make the registers 32
   // bits (A), 31 bits (B), and 29 bits (C).
   //
   // Bit   |  3            2             1            0
   // Pos'n | 1098 7654  3210 9876  5432 1098  7654 3210
   // ===== | ==========================================
   // Value | 8421-8421  8421-8421  8421-8421  8421-8421
   // ===== | ==========================================
   //       |
   // A-Rot0 | 0111 1111  1111 1111  1111 1111  1111 1111
   // A-Rot1 | 1000 0000  0000 0000  0000 0000  0000 0000
   //        |
   // B-Rot0 | 0011 1111  1111 1111  1111 1111  1111 1111
   // B-Rot1 | 1100 0000  0000 0000  0000 0000  0000 0000
   //        |
   // C-Rot0 | 0000 1111  1111 1111  1111 1111  1111 1111
   // C-Rot1 | 1111 0000  0000 0000  0000 0000  0000 0000
   //
   //
   // Reg Size MSB Position   Rotate-0 Mask  Rotate-1 Mask
   // A  32    31       0x7FFFFFFF     0x80000000
   // B  31    30       0x3FFFFFFF     0xC0000000
   // C  29    28       0x0FFFFFFF     0xF0000000
   //

          m_Rot0_A( 0x7FFFFFFF ),
          m_Rot0_B( 0x3FFFFFFF ),
          m_Rot0_C( 0x0FFFFFFF ),
          m_Rot1_A( 0x80000000 ),
          m_Rot1_B( 0xC0000000 ),
          m_Rot1_C( 0xF0000000 )
{
   strcpy_s( m_szKey, zsizeof( m_szKey ), "Don2Cam9Pas1Gig3Dks5Dgc8Kat2Mik3Jab1Mat2" );
}

// Everything is on the frame.
ARACrypt::~ARACrypt( )
{
}

/////////////////////////////////////////////////////////////////////////////
// ARACrypt operations

void
ARACrypt::SetKey( zCPCHAR cpcKey )
{
   zCHAR szSeed[ zMAXEL + 1 ];

   strcpy_s( m_szKey, zsizeof( m_szKey ), cpcKey );

   if ( m_szKey[ 0 ] == 0 )
   {
      // Put some really outrageous characters in seed just in case.
      strcpy_s( szSeed, sizeif( szSeed ), "\x43\xC8\x21\xD3\xF4\xB3\x10\x27\x09\xAA\x18\x56" );

      //TO DO: Add Message to Event Log and/or window when there is one.
//    AfxMessageBox( "WARNING: Missing Pass Phrase. Default in effect!" );
   }
   else
   {
      strcpy_s( szSeed, zsizeof( szSeed ), m_szKey );
   }

   // Make sure seed is at least 12 bytes long.

   int nIdx = 0;
   int nLth = strlen( szSeed );

   for ( nIdx = 0; nLth < 12; nIdx++ )
   {
      szSeed[ nLth++ ] = szSeed[ nIdx ];
      szSeed[ nLth ] = 0;
   }

   // LFSR A, B, and C get the first, second, and
   // third four bytes of the seed, respectively.

   for ( nIdx = 0; nIdx < 4; nIdx++ )
   {
      m_LFSR_A = ((m_LFSR_A <<= 8) |
                  ((unsigned long int) szSeed[ nIdx + 0 ]));

      m_LFSR_B = ((m_LFSR_B <<= 8) |
                  ((unsigned long int ) szSeed[ nIdx + 4 ]));

      m_LFSR_C = ((m_LFSR_C <<= 8) |
                  ((unsigned long int) szSeed[ nIdx + 8 ]));
   }

   // If any LFSR contains 0x00000000, load a
   // non-zero default value instead. There is
   // no particular "good" value. The ones here
   // were selected to be distinctive during
   // testing and debugging.

   if ( 0x00000000 == m_LFSR_A )
      m_LFSR_A = 0x13579BDF;

   if ( 0x00000000 == m_LFSR_B )
      m_LFSR_B = 0x2468ACE0;

   if ( 0x00000000 == m_LFSR_C )
      m_LFSR_C = 0xFDB97531;
}

void
ARACrypt::GetKey( zPCHAR pchKey, zLONG lKeyLth )
{
   strcpy( pchKey, lKeyLth, m_szKey );
}

//***********************************************
// ARACrypt::TransformChar
//***********************************************
// Transform a single character. If it is
// plain text, it will be encrypted. If it is
// encrypted, and if the LFSRs are in the same
// state as when it was encrypted (that is, the
// same keys loaded into them and the same number
// of calls to TransformChar after the keys
// were loaded), the character will be decrypted
// to its original value.
//
// DEVELOPER'S NOTE
// This code contains corrections to the LFSR
// operations that supercede the code examples
// in Applied Cryptography (first edition, up to
// at least the 4th printing, and second edition,
// up to at least the 6th printing). More recent
// errata sheets may show the corrections.
//***********************************************

void
ARACrypt::TransformChar( unsigned char& cTarget )
{
   int               Counter  = 0;
   unsigned char     Crypto   = '\0';
   unsigned long int Out_B = (m_LFSR_B & 0x00000001);
   unsigned long int Out_C = (m_LFSR_C & 0x00000001);

   // Cycle the LFSRs eight times to get eight
   // pseudo-random bits. Assemble these into
   // a single random character (Crypto).

   for ( Counter = 0; Counter < 8; Counter++ )
   {
      if ( m_LFSR_A & 0x00000001 )
      {
         // The least-significant bit of LFSR
         // A is "1". XOR LFSR A with its
         // feedback mask.

         m_LFSR_A = (((m_LFSR_A ^ m_Mask_A ) >> 1) | m_Rot1_A);

         // Clock shift register B once.
         if ( m_LFSR_B & 0x00000001 )
         {
            // The LSB of LFSR B is "1". XOR
            // LFSR B with its feedback mask.

            m_LFSR_B = (((m_LFSR_B ^ m_Mask_B) >> 1) | m_Rot1_B);

            Out_B = 0x00000001;
         }
         else
         {
            // The LSB of LFSR B is "0". Rotate
            // the LFSR contents once.

            m_LFSR_B = ((m_LFSR_B >> 1) & m_Rot0_B);

            Out_B = 0x00000000;
         }
      }
      else
      {
         // The LSB of LFSR A is "0".
         // Rotate the LFSR contents once.

         m_LFSR_A = ((m_LFSR_A >> 1) & m_Rot0_A);

         // Clock shift register C once.

         if ( m_LFSR_C & 0x00000001 )
         {
            // The LSB of LFSR C is "1".
            // XOR LFSR C with its feedback mask.

            m_LFSR_C = (((m_LFSR_C ^ m_Mask_C) >> 1) | m_Rot1_C);
            Out_C = 0x00000001;
         }
         else
         {
            // The LSB of LFSR C is "0". Rotate
            // the LFSR contents once.

            m_LFSR_C = ((m_LFSR_C >> 1) & m_Rot0_C);

            Out_C = 0x00000000;
         }
      }

      // XOR the output from LFSRs B and C and
      // rotate it into the right bit of Crypto.

      // The following conversion warning is unecessary here as
      // 'loss of data' is a side effect and harmless.
      #pragma warning( disable : 4244 )

      Crypto = ((Crypto << 1) | (Out_B ^ Out_C));

      #pragma warning( default : 4244 )
   }

   // XOR the resulting character with the
   // input character to encrypt/decrypt it.

   // The following conversion warning not necessary here either.
   // The 'loss of data', so to speak is a side effect and harmless.
   #pragma warning( disable : 4244 )

   cTarget = (cTarget ^ Crypto);

   if ( cTarget == NULL )           // No nulls allowed here. There is
     cTarget = (cTarget ^ Crypto);  // no working std::string available.

   #pragma warning( default : 4244 )
}

//***********************************************
// ARACrypt::TransformString
//***********************************************
// Encrypt or decrypt a standard string in place.
// The string to transform is passed in as
// Target.
//***********************************************

void
ARACrypt::TransformString( zPCHAR pchTarget, zCPCHAR cpcKey )
{
   // Reset the shift registers.

   if ( cpcKey && *cpcKey )
      SetKey( cpcKey );

   // Transform each character in the string.
   //
   // DEVELOPER'S NOTE
   // ==========================================
   // DO NOT TREAT THE OUTPUT STRING AS A NULL-
   // TERMINATED STRING.
   // ==========================================
   // The transformation process can create null
   // characters in the output string. Always
   // use the length( ) method to retrieve the full
   // string when it has been transformed.

   // bek NOTE: The above note does not apply to this
   // implementation of Warren Ward's method.
   // ARACrypt knows about nulls and transforms them
   // into XOR nulls so the entire result can be
   // treated as a 'normal' null terminated string.

   int nLth = strlen( pchTarget );

   for ( int nPos = 0; nPos < nLth; nPos++ )
   {
      // The following conversion warning not necessary here either.
      // The 'loss of data', so to speak is a side effect and harmless.
      #pragma warning( disable : 4244 )

      unsigned char cBuff = pchTarget[ nPos ];
      TransformChar( (unsigned char&) cBuff );
      pchTarget[ nPos ] = cBuff;
   }
}

unsigned
zCHAR EncryptA[] = { 22, 31,  5, 13, 26, 16, 15, 28,  0,  6,
                     23,  2, 19, 12,  9, 10, 30,  1, 25,  4,
                     29, 17,  7, 18, 27, 11, 21, 14, 24, 20, 3, 8 };
unsigned
zCHAR EncryptB[] = { 10,  9, 17,  4, 31, 22,  1,  6, 29, 20,
                     13, 12, 19,  5, 24,  8, 26, 15,  7, 30,
                     23,  2, 11, 28, 21, 27, 14,  0, 25,  3, 16, 18 };

unsigned
zCHAR EncryptNonAlpha[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            '\"',    // ' '
                            '@',     // '\"'
                            '%',     // '!'
                            '4',     // '#'
                            '&',     // '$'
                            '$',     // '%'
                            '\'',    // '&'
                            '3',     // '\''
                            '[',     // '('
                            '(',     // ')'
                            ':',     // '*'
                            '5',     // '+'
                            '+',     // ','
                            '!',     // '-'
                            '#',     // '.'
                            '?',     // '/'
                            '|',     // '0'
                            '.',     // '1'
                            '/',     // '2'
                            '`',     // '3'
                            '6',     // '4'
                            ',',     // '5'
                            '7',     // '6'
                            '-',     // '7'
                            '9',     // '8'
                            '8',     // '9'
                            '*',     // ':'
                            '1',     // ';'
                            ';',     // '<'
                            '>',     // '='
                            '<',     // '>'
                            ')',     // '?'
                            ' ',     // '@'
                            'h', 'k', 'n', 'b', 'r', 't', 'v', 'e', 'y', 'g',
                            'i', 'l', 'a', 'p', 's', 'd', 'w', 'x', 'f', 'z',
                            'j', 'm', 'o', 'q', 'c', 'u',
                            '}',     // '['
                            '=',     // '\\'
                            '2',     // ']'
                            '{',     // '^'
                            '\\',    // '_'
                            ']',     // '`'
                            'K', 'E', 'O', 'Q', 'C', 'U', 'G', 'X', 'I', 'Z',
                            'L', 'M', 'B', 'R', 'T', 'F', 'W', 'H', 'Y', 'J',
                            'A', 'N', 'P', 'D', 'S', 'V',
                            '^',     // '{'
                            '_',     // '|'
                            '~',     // '}'
                            '0'  };  // '~'
// #include <time.h>

//./ ADD NAME=UfEncryptStringUL
// Source Module=kzoedeca.cpp
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UfEncryptStringUL
//
// PARAMETERS: pchOut - return string encrypted
//             pchIn  - String to encrypt
//             pchKey - Encryption key (may be null)
//             uLth   - maximum length of string to encrypt
//
// DESCRIPTION - This utility takes in a string and returns an
//               encrypted version of the string with a length
//               of the passed length value. The maximum length
//               which may be passed is zMAXEL ... so the maximum
//               password length is zMAXEL - 1.
//
// RETURNS:  0 - Success
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
UfEncryptStringUL( zPCHAR pchOut, zPCHAR pchIn, zPCHAR pchKey, zUSHORT uLth )
{
   ARACrypt crypt;
   zPCHAR   pch;
   zUCHAR   uCrypt;
   zUSHORT  uChar;
   zSHORT   nOrderIdx;
   zLONG    lInLth;
// zCHAR    cChar;
   int      k;
// zCHAR    szMsg[ 256 ];

// if ( uLth == 20 )
//    return( UfEncryptString( pchOut, pchIn, uLth ) );

// sprintf_s( szMsg, zsizeof( szMsg ), "UfEncryptStringUL Length: %d In: \"%s\"", uLth, pchIn );
// TraceLineS( szMsg, "" );

   // Set up the encryption key.
   if ( pchKey && *pchKey )
   {
      zCHAR  szKey[ 65 ];
      zCHAR  szNewKey[ 65 ];
      zLONG  lKeyLth;

      strncpy_s( szNewKey, pchKey, zsizeof( szNewKey ) - 1 );
      szNewKey[ zsizeof( szNewKey ) - 1 ] = 0; // force null terminator
      lInLth = zstrlen( szNewKey );

      crypt.GetKey( szKey, zsizeof( szKey ) );
      lKeyLth = zstrlen( szKey );
      if ( lKeyLth + lInLth > zsizeof( szKey ) - 1 )
         lKeyLth = zsizeof( szKey ) - 1 - lInLth;

      if ( lKeyLth > 0 )
      {
         strncpy_s( szNewKey + lInLth, pchKey, lKeyLth );
         szNewKey[ zsizeof( szKey ) - 1 ] = 0; // force null terminator
         crypt.SetKey( szNewKey );
      }
   }

   pchOut[ uLth ] = 0;
   lInLth = zstrlen( pchIn );
   if ( uLth > zMAXEL || lInLth >= uLth )
   {
      SysMessageBox( 0, "Zeidon Internal Error",
                     "Length Error Encrypting String", 0 );
      return( zCALL_ERROR );
   }

   pch = new zCHAR[ uLth + 1 ];
   strcpy_s( pch, uLth + 1, pchIn );

   // Seed the random-number generator with current time so that
   // the numbers will be different every time we run.
// srand( (unsigned) time( NULL ) );

   // Seed the random-number generator with the same data so that
   // the numbers will be same every time we run with the same data.
   if ( lInLth > zsizeof( unsigned ) )
      k = lInLth - zsizeof( unsigned );
   else
   {
      k = lInLth + 1;
      while ( k < zsizeof( unsigned ) )
         pch[ k++ ] = 0;

      k = 0;
   }

   srand( (unsigned) *((unsigned *) ((zPCHAR) pch + k)) ); // set a random starting point

   for ( k = lInLth; k < uLth; k++ )  // set "empty" cells to random values
   {
      do
      {
         pch[ k ] = (zCHAR) rand( );

      } while ( pch[ k ] == 0 );
   }

   pch[ uLth ] = 0;
   crypt.TransformString( pch );
   uChar = 0;
   nOrderIdx = 0;
   while ( EncryptB[ nOrderIdx ] > (uLth - 1) )
      nOrderIdx++;

   while ( (zUSHORT) lInLth > uLth || lInLth > zMAXEL )
      lInLth -= uLth;

   pchOut[ EncryptB[ nOrderIdx ] ] = EncryptA[ lInLth ] + 'a';
// sprintf_s( szMsg, zsizeof( szMsg ), "UfEncryptStringUL Idx: %d EncryptIdx: %d Value: %2x",
//           nOrderIdx, EncryptB[ nOrderIdx ], pchOut[ EncryptB[ nOrderIdx ] ] );
// TraceLineS( szMsg, "" );
   nOrderIdx++;
   while ( uChar < (uLth - 1) )
   {
      while ( EncryptB[ nOrderIdx ] > (uLth - 1) )
         nOrderIdx++;

      uCrypt = pch[ uChar ];
      pchOut[ EncryptB[ nOrderIdx ] ] = uCrypt;
   // sprintf_s( szMsg, zsizeof( szMsg ), "UfEncryptStringUL Idx: %d EncryptIdx: %d Value: %2x",
   //            nOrderIdx, EncryptB[ nOrderIdx ], pchOut[ EncryptB[ nOrderIdx ] ] );
   // TraceLineS( szMsg, "" );

      uChar++;
      nOrderIdx++;
   }

   if ( zstrlen( pchOut ) != uLth )
   {
      SysMessageBox( 0, "Zeidon Internal Error",
                     "Null Error Encrypting String", 0 );
      return( zCALL_ERROR );
   }

// sprintf_s( szMsg, zsizeof( szMsg ), "UfEncryptStringUL Out: \"%s\"", pchOut );
// TraceLineS( szMsg, "" );

// UfDecryptStringUL( pch, pchOut, uLth );
   delete [] pch;
   return( 0 );
}

//./ ADD NAME=UfDecryptStringUL
// Source Module=kzoedeca.cpp
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UfDecryptStringUL
//
// PARAMETERS: pchOut - return string Decrypted
//             pchIn  - Encrypted string
//             pchKey - Encryption key (may be null)
//             uLth   - maximum length of string to return
//
// DESCRIPTION - This utility takes in an Encrypted string and returns
//               the Decrypted version of the string.
//
// RETURNS:  0 - Success
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
UfDecryptStringUL( zPCHAR pchOut, zPCHAR pchIn, zPCHAR pchKey, zUSHORT uLth )
{
   ARACrypt crypt;
   zUCHAR   uCrypt;
   zUSHORT  uChar;
   zSHORT   nOrderIdx;
   zLONG    lInLth;
   zSHORT   nReturnLth;
   zCHAR    szMsg[ 256 ];

// if ( uLth == 20 )
//    return( UfDecryptString( pchOut, pchIn, uLth ) );

// sprintf_s( szMsg, zsizeof( szMsg ), "UfDecryptStringUL Length: %d In: \"%s\"", uLth, pchIn );
// TraceLineS( szMsg, "" );

   if ( *pchIn == 0 )
   {
      *pchOut = 0;
      return( 0 );
   }

   // Set up the encryption key.
   if ( pchKey && *pchKey )
   {
      zCHAR  szKey[ 65 ];
      zCHAR  szNewKey[ 65 ];
      zLONG  lKeyLth;

      strncpy_s( szNewKey, zsizeof( szNewKey ), pchKey, zsizeof( szNewKey ) - 1 );
      szNewKey[ zsizeof( szNewKey ) - 1 ] = 0; // force null terminator
      lInLth = zstrlen( szNewKey );

      crypt.GetKey( szKey, zsizeof( szKey ) );
      lKeyLth = zstrlen( szKey );
      if ( lKeyLth + lInLth > zsizeof( szKey ) - 1 )
         lKeyLth = zsizeof( szKey ) - 1 - lInLth;

      if ( lKeyLth > 0 )
      {
         strncpy_s( szNewKey + lInLth, pchKey, lKeyLth );
         szNewKey[ zsizeof( szKey ) - 1 ] = 0; // force null terminator
         crypt.SetKey( szNewKey );
      }
   }

   uChar = 0;
   nOrderIdx = 0;
   while ( EncryptB[ nOrderIdx ] > (uLth - 1) )
      nOrderIdx++;

   lInLth = (zSHORT) (((zUCHAR) pchIn[ EncryptB[ nOrderIdx ] ]) - 'a');
   if ( uLth > zMAXEL || lInLth >= uLth || lInLth < 0 )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), "Length Error Decrypting String: (%d:%d) \"%s\"", uLth, lInLth, pchIn );
      SysMessageBox( 0, "Zeidon Internal Error", szMsg, 0 );
      return( zCALL_ERROR );
   }

   nReturnLth = 0;
   while ( EncryptA[ nReturnLth ] != (zUCHAR) lInLth )
      nReturnLth++;

   lInLth = (zCHAR) nReturnLth;
   nOrderIdx++;
   while ( lInLth )
   {
      while ( EncryptB[ nOrderIdx ] > (uLth - 1) )
         nOrderIdx++;

      uCrypt = pchIn[ EncryptB[ nOrderIdx ] ];
      pchOut[ uChar ] = uCrypt;

      lInLth--;
      uChar++;
      nOrderIdx++;
   }

   pchOut[ uLth ] = 0;
   crypt.TransformString( pchOut );
   pchOut[ uChar ] = 0;
// sprintf_s( szMsg, zsizeof( szMsg ), "UfDecryptStringUL Out: \"%s\"", pchOut );
// TraceLineS( szMsg, "" );
   return( nReturnLth );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
CommitMultipleOIs( zLONG   lViewCluster,
                   zSHORT  nViewCount,
                   zPSHORT pnViewErrorIdx,
                   zLONG   lControl )
{
   return( ::CommitMultipleOIs( (LPVIEWCLUSTER) lViewCluster,
                                nViewCount, pnViewErrorIdx, lControl ) );
}

/* --------------------------------------------------------------------

   Decimal operations.  Even though these operations start with 'Sys...'
   they are the same for all OS's.

-------------------------------------------------------------------- */

extern "C"
{

// Default decimal operation in kzoesyaa.c
zSHORT OPERATION
fnDecimalOperation( void  *pvReturn,
                    void  *pvDecimal,
                    void  *pvInfo,
                    long  lControl );

// Decimal operation currently defined only in decari\zdecimal.c
zSHORT OPERATION
DecimalOperation( void  *pvReturn,
                  void  *pvDecimal,
                  void  *pvInfo,
                  long  lControl );

zSHORT LOCALOPER
fnCallDecOper( zPVOID    pvReturn,
               zPVOID    pvDecimal,
               zPVOID    pvInfo,
               zLONG     lControl )
{
   zSHORT    nRC;
   zDECOPER  pfn;

#if 1

// pfn = fnDecimalOperation;   // in kzoesyaa.c
   pfn = DecimalOperation;     // in zdecimal.c

   // Call function to initialize.
   nRC = (*pfn) ( 0, 0, 0, zDEC_CMD_INIT );
   if ( nRC == zCALL_ERROR )
   {
      TraceLineS( "(ZDecimal) Error calling INIT function in ",
                  "zDecimalOperation" );
   }

#else

   LPLIBRARY lpLibrary;

   if ( lpTask && lpTask->pfnDecOper )
      pfn = (zDECOPER) lpTask->pfnDecOper;
   else
   {
      zCHAR szValue[ zMAX_FILENAME_LTH + 1 ];

      lpLibrary = fnSysLoadLibraryWithErrFlag( lpTask, "zdecimal", 0 );
      if ( lpLibrary != 0 )
      {
         pfn = (zDECOPER) SysGetProc( lpLibrary, "zDecimalOperation" );
         if ( pfn == 0 )
         {
            fnSysFreeLibrary( lpTask, lpLibrary );
            lpLibrary = 0;
         }
      }

      // If lpLibrary is 0 then we couldn't find zDecimalOperation in the
      // decimal DLL so we'll use the default one.
      if ( lpLibrary == 0 )
         pfn = fnDecimalOperation;   // in kzoesyaa.c

      // Call function to initialize.
      nRC = (*pfn)( 0, 0, 0, zDEC_CMD_INIT );
      if ( nRC == zCALL_ERROR )
      {
         fnSysFreeLibrary( lpTask, lpLibrary );
         lpLibrary = 0;

         TraceLineS( "(ZDecimal) Error calling INIT function in ",
                     "zDecimalOperation" );
         pfn = fnDecimalOperation;   // in kzoesyaa.c
      }

      // It's a little late to pop up the message but this allows the INIT
      // command to take place so we can write any pertinent info to trace.
      if ( lpLibrary == 0 )
      {
         SysReadZeidonIni( -1, "[Zeidon]", "DefaultDecimalOK", szValue, zsizeof( szValue ) );
         zstrupper( szValue );

         if ( szValue[ 0 ] != 'Y' &&
              szValue[ 0 ] != '1' && szValue[ 0 ] != 'T' )
         {
            fnSysMessageBox( lpTask, szlOE_SystemError,
                             "Couldn't find zDecimalOperation().\n"
                             "Using default of 'double'.", 1 );
         }
      }

      if ( lpTask )
      {
         lpTask->lpDecLibrary = lpLibrary;
         lpTask->pfnDecOper   = (zPOPER) pfn;
      }
   }

#endif

   return( (*pfn)( pvReturn, pvDecimal, pvInfo, lControl ) );
}

zSHORT OPERATION
SysCallDecimalOperation( zPVOID    pvReturn,
                         zPVOID    pvDecimal,
                         zPVOID    pvInfo,
                         zLONG     lControl )
{
   return( fnCallDecOper( pvReturn, pvDecimal, pvInfo, lControl ) );
}

zSHORT OPERATION
SysAssignDecimalFromDecimal( zPDECIMAL pdTarget,
                             zPDECIMAL pdSource )
{
   return( fnCallDecOper( pdTarget, pdSource, 0, zDEC_CMD_ASSIGNDEC ) );
}

zSHORT OPERATION
SysAssignDecimalFromNull( zPDECIMAL pdTarget )
{
   return( fnCallDecOper( pdTarget, 0, 0, zDEC_CMD_ASSIGNNULL ) );
}

zSHORT OPERATION
SysCompareDecimalToDecimal( zPDECIMAL pdTarget,
                            zPDECIMAL pdSource )
{
   return( fnCallDecOper( 0, pdTarget, pdSource, zDEC_CMD_COMPAREDEC ) );
}

zSHORT OPERATION
SysCompareDecimalToDouble( zPDECIMAL pdTarget,
                           double    dSource )
{
   return( fnCallDecOper( 0, pdTarget, &dSource, zDEC_CMD_COMPAREDOUBLE ) );
}

zSHORT OPERATION
SysCompareDecimalToNull( zPDECIMAL pdSource )
{
   return( fnCallDecOper( 0, pdSource, 0, zDEC_CMD_COMPARENULL ) );
}

zSHORT OPERATION
SysConvertDecimalToString( zPDECIMAL pdDecimal,
                           zPCHAR    pszString,
                           zSHORT    nPrecision )
{
   return( fnCallDecOper( pszString, pdDecimal, &nPrecision,
                          zDEC_CMD_DECTOSTRING ) );
}

zSHORT OPERATION
SysConvertStringToDecimal( zCPCHAR pszString, zPDECIMAL pdDecimal )
{
   return( fnCallDecOper( pdDecimal, 0, (zPVOID) pszString,
                          zDEC_CMD_STRINGTODEC ) );
}

zSHORT OPERATION
SysConvertLongToDecimal( zLONG     lLong,
                         zPDECIMAL pdDecimal )
{
   return( fnCallDecOper( pdDecimal, 0, &lLong, zDEC_CMD_LONGTODEC ) );
}

zSHORT OPERATION
SysConvertDecimalToLong( zPDECIMAL pdDecimal,
                         zPLONG    plLong )
{
   return( fnCallDecOper( plLong, pdDecimal, 0, zDEC_CMD_DECTOLONG ) );
}

zSHORT OPERATION
SysConvertDecimalToDouble( zPDECIMAL pdDecimal,
                           double *  pdDouble )
{
   return( fnCallDecOper( pdDouble, pdDecimal, 0, zDEC_CMD_DECTODOUBLE ) );
}

zSHORT OPERATION
SysConvertDoubleToDecimal( double    dDouble,
                           zPDECIMAL pdDecimal )
{
   return( fnCallDecOper( pdDecimal, 0, &dDouble, zDEC_CMD_DOUBLETODEC ) );
}

zSHORT OPERATION
SysAddDecimalToDecimal( zPDECIMAL pdResult,
                        zPDECIMAL pdDec1,
                        zPDECIMAL pdDec2 )
{
   return( fnCallDecOper( pdResult, pdDec1, pdDec2, zDEC_CMD_ADDDEC ) );
}

zSHORT OPERATION
SysSubtractDecimalFromDecimal( zPDECIMAL pdResult,
                               zPDECIMAL pdDec1,
                               zPDECIMAL pdDec2 )
{
   return( fnCallDecOper( pdResult, pdDec1, pdDec2, zDEC_CMD_SUBDEC ) );
}

zSHORT OPERATION
SysMultiplyDecimalByDecimal( zPDECIMAL pdResult,
                             zPDECIMAL pdDec1,
                             zPDECIMAL pdDec2 )
{
   return( fnCallDecOper( pdResult, pdDec1, pdDec2, zDEC_CMD_MULTDEC ) );
}

zSHORT OPERATION
SysDivideDecimalByDecimal( zPDECIMAL pdResult,
                           zPDECIMAL pdDec1,
                           zPDECIMAL pdDec2 )
{
   return( fnCallDecOper( pdResult, pdDec1, pdDec2, zDEC_CMD_DIVDEC ) );
}

zSHORT OPERATION
SysRoundDecimal( zPDECIMAL pd,
                 zSHORT nPrecision )
{
   return( fnCallDecOper( pd, pd, &nPrecision, zDEC_CMD_ROUND ) );
}

} // extern "C"
