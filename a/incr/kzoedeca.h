// CHANGE LOG

// 1999.08.25  TMV    Z10
//   fix function CommitMultipleOIs to make Z10 compileable
//   therefore we have to define a 2nd friend CommitMultipleOIs
//   with LPVIEWCLUSTER as first parameter (see KZOENGAA.H)
//
// 1999.08.24  BL    Z10
//   Remove function in if 0
//   function CommitMultipleOIs: insert zOPER_EXPORT and OPERATION
//
// 1999.08.20  BL    ALL
//    Move function code from h- into cpp-file
//
// from here old version
//
// 1998.05.29 - 1998.07.19   RG!!
//    Several changes from Quinsoft without change log
//
// 1998.07.23  TMV
//    Make changes so that a 16-Bit Runtime can be build from MFC 5.0 Sources
//
// 1998.07.29  RG
//    changed: WIN32 --> __WIN32__, because WIN32 is not set in the makefiles
//    for the object engine and the zeidon tools
//
// 08.09.1998  HH
//    move function bodies to kzoedeca.cpp.
//
// 1198.12.22  DGC
//    Added overload of CommitMultipleOIs so we can use it from VML.
//

#define zDecimal ZDecimal

#ifdef ZDECIMAL_CLASS
#else
   #define ZDECIMAL_CLASS
#endif

//#ifdef __WIN32__
class ZDECIMAL_CLASS ZDecimal : public DecimalStruct
{
public:
   char     szValue[ 100 ];   // Used when converting ZDecimal -> string.

   // ====================================================================
   // Constructors
   // ====================================================================

   ZDecimal( );
   ZDecimal( zDECIMAL DEC  );
   ZDecimal( zPDECIMAL pDEC );
   ZDecimal( zPCHAR  pszStr );
   ZDecimal( zLONG   l );
   ZDecimal( zULONG  l );
   ZDecimal( zSHORT  n );
   ZDecimal( zUSHORT n );
   ZDecimal( double  d );
   ZDecimal( int i );
   ZDecimal( unsigned int i );
   ZDecimal( zCPCHAR cpcStr );

   ~ZDecimal( );

   // ====================================================================
   // Assignment to ZDecimal from variable.
   // ====================================================================

   ZDecimal operator = ( ZDecimal Dec );
   ZDecimal operator = ( zLONG l );
   ZDecimal operator = ( zULONG l );
   ZDecimal operator = ( zSHORT n);
   ZDecimal operator = ( zUSHORT n );
   ZDecimal operator = ( double d );
   ZDecimal operator = ( const double d ) const;
   ZDecimal operator = ( zPCHAR pszStr );
   ZDecimal operator = ( zCPCHAR cpcStr );
   ZDecimal operator = ( zPVOID pv );
   ZDecimal operator = ( zDECIMAL DEC );
   ZDecimal operator = ( zPDECIMAL pDEC );
   ZDecimal operator = ( int i );
   ZDecimal operator = ( unsigned int i );

   // ====================================================================
   // Assignment to variable from ZDecimal
   // ====================================================================

   operator double( );
   operator zLONG( );
   operator zULONG( );
   operator zSHORT( );
   operator zUSHORT( );
   operator zPCHAR( );

   zBOOL IsNull( );

   // ====================================================================
   // Decimal only comparators
   // ====================================================================

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT char OPERATION
   operator ! ( ZDecimal zDec );

   // ====================================================================
   // Addition operators involving mixed types
   // ====================================================================

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, ZDecimal zDec2 );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( int n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, double d );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( double d, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( ZDecimal zDec, float d );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator + ( float d, ZDecimal zDec );

   // ====================================================================
   // Subtraction operators
   // ====================================================================

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, ZDecimal zDec2 );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( int n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, double d );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( double d, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( ZDecimal zDec, float d );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator - ( float d, ZDecimal zDec );

   // ====================================================================
   // Multiplication operators
   // ====================================================================

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, ZDecimal zDec2 );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( int n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, double d );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( double d, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( ZDecimal zDec, float d );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator * ( float d, ZDecimal zDec );

   // ====================================================================
   // Division operators
   // ====================================================================

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, ZDecimal zDec2 );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( int n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, double d );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( double d, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( ZDecimal zDec, float d );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator / ( float d, ZDecimal zDec );

   // ====================================================================
   // ">=" operators involving mixed types
   // ====================================================================

   friend zOPER_EXPORT char OPERATION
   operator >= ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT char OPERATION
   operator >= ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT char OPERATION
   operator >= ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator >= ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, int n );

   friend zOPER_EXPORT char OPERATION
   operator >= ( int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT char OPERATION
   operator >= ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator >= ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator >= ( double n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, double n );

   friend zOPER_EXPORT char OPERATION
   operator >= ( float n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator >= ( ZDecimal zDec, float n );

   // ====================================================================
   // ">" operators involving mixed types
   // ====================================================================

   friend zOPER_EXPORT char OPERATION
   operator > ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT char OPERATION
   operator > ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT char OPERATION
   operator > ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator > ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, int n );

   friend zOPER_EXPORT char OPERATION
   operator > ( int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT char OPERATION
   operator > ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator > ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator > ( double n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, double n );

   friend zOPER_EXPORT char OPERATION
   operator > ( float n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator > ( ZDecimal zDec, float n );

   // ====================================================================
   // "<=" operators involving mixed types
   // ====================================================================

   friend zOPER_EXPORT char OPERATION
   operator <= ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT char OPERATION
   operator <= ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT char OPERATION
   operator <= ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator <= ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, int n );

   friend zOPER_EXPORT char OPERATION
   operator <= ( int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT char OPERATION
   operator <= ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator <= ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator <= ( double n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, double n );

   friend zOPER_EXPORT char OPERATION
   operator <= ( float n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator <= ( ZDecimal zDec, float n );

   // ====================================================================
   // "<" operators involving mixed types
   // ====================================================================

   friend zOPER_EXPORT char OPERATION
   operator < ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT char OPERATION
   operator < ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT char OPERATION
   operator < ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator < ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, int n );

   friend zOPER_EXPORT char OPERATION
   operator < ( int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT char OPERATION
   operator < ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator < ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator < ( double n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, double n );

   friend zOPER_EXPORT char OPERATION
   operator < ( float n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator < ( ZDecimal zDec, float n );

   // ====================================================================
   // "==" operators involving mixed types
   // ====================================================================

   friend zOPER_EXPORT char OPERATION
   operator == ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT char OPERATION
   operator == ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT char OPERATION
   operator == ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator == ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, int n );

   friend zOPER_EXPORT char OPERATION
   operator == ( int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT char OPERATION
   operator == ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator == ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator == ( double n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, double n );

   friend zOPER_EXPORT char OPERATION
   operator == ( float n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator == ( ZDecimal zDec, float n );

   // ====================================================================
   // "!=" operators involving mixed types
   // ====================================================================

   friend zOPER_EXPORT char OPERATION
   operator != ( zLONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT char OPERATION
   operator != ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT char OPERATION
   operator != ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator != ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, int n );

   friend zOPER_EXPORT char OPERATION
   operator != ( int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT char OPERATION
   operator != ( unsigned int n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator != ( zCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( zUCHAR n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT char OPERATION
   operator != ( double n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, double n );

   friend zOPER_EXPORT char OPERATION
   operator != ( float n, ZDecimal zDec );

   friend zOPER_EXPORT char OPERATION
   operator != ( ZDecimal zDec, float n );

   // ====================================================================
   // "+=" operators
   // ====================================================================

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal& zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT zLONG    OPERATION
   operator += ( zLONG & l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT zULONG   OPERATION
   operator += ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT zSHORT   OPERATION
   operator += ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT zUSHORT  OPERATION
   operator += ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator += ( ZDecimal zDec, double d );

   // ====================================================================
   // "-=" operators
   // ====================================================================

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal& zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT zLONG    OPERATION
   operator -= ( zLONG & l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT zULONG   OPERATION
   operator -= ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT zSHORT   OPERATION
   operator -= ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT zUSHORT  OPERATION
   operator -= ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator -= ( ZDecimal zDec, double d );

   // ====================================================================
   // "*=" operators
   // ====================================================================

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal& zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT zLONG    OPERATION
   operator *= ( zLONG & l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT zULONG   OPERATION
   operator *= ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT zSHORT   OPERATION
   operator *= ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT zUSHORT  OPERATION
   operator *= ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator *= ( ZDecimal zDec, double d );

   // ====================================================================
   // "/=" operators
   // ====================================================================

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal& zDec1, ZDecimal zDec2 );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, zLONG l );

   friend zOPER_EXPORT zLONG    OPERATION
   operator /= ( zLONG & l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, zULONG l );

   friend zOPER_EXPORT zULONG   OPERATION
   operator /= ( zULONG l, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, zSHORT n );

   friend zOPER_EXPORT zSHORT   OPERATION
   operator /= ( zSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, zUSHORT n );

   friend zOPER_EXPORT zUSHORT  OPERATION
   operator /= ( zUSHORT n, ZDecimal zDec );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, unsigned int n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, zCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, zUCHAR n );

   friend zOPER_EXPORT ZDecimal OPERATION
   operator /= ( ZDecimal zDec, double d );
/*
   // declared here implemented in kzoedeca.cpp
   friend zOPER_EXPORT zSHORT OPERATION
   CommitMultipleOIs( zLONG   lViewCluster,
                      zSHORT  nViewCount,
                      zPSHORT pnViewErrorIdx,
                      zLONG   lControl );

   // as it is declared in KZOENGAA.H
   friend zOPER_EXPORT zSHORT OPERATION
   CommitMultipleOIs( LPVIEWCLUSTER lViewCluster,
                      zSHORT  nViewCount,
                      zPSHORT pnViewErrorIdx,
                      zLONG   lControl );
*/
   zVIEW m_lpView;

};  // ZDecimal class

//
// Note: A Special Thanks to Mr. Warren Ward for his Sept. 1998 CUJ article:
// "Stream Encryption" Copyright (c) 1998 by Warren Ward
/////////////////////////////////////////////////////////////////////////////
// ARACrypt window

class ARACrypt
{
// Construction
public:
   ARACrypt( );

// Attributes
public:

// Operations
public:
   virtual void TransformString( zPCHAR pchTarget, zCPCHAR cpcKey = 0 );
   virtual void SetKey( zCPCHAR cpcKey );
   virtual void GetKey( zPCHAR pchKey, zLONG lKeyLth );
   virtual void TransformChar( unsigned char& chTarget );

// Implementation
public:
   virtual ~ARACrypt( );

// Attributes
protected:

   char m_szKey[ 65 ];

   unsigned long           m_LFSR_A;
   unsigned long           m_LFSR_B;
   unsigned long           m_LFSR_C;
   const unsigned long     m_Mask_A;
   const unsigned long     m_Mask_B;
   const unsigned long     m_Mask_C;
   const unsigned long     m_Rot0_A;
   const unsigned long     m_Rot0_B;
   const unsigned long     m_Rot0_C;
   const unsigned long     m_Rot1_A;
   const unsigned long     m_Rot1_B;
   const unsigned long     m_Rot1_C;
};

/////////////////////////////////////////////////////////////////////////////

//#endif // #ifdef __WIN32__
