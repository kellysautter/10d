#ifndef __DECARI_H__
#define __DECARI_H__

/*
** decari.h
**
**  header file for special decimal arithmetic
**
*/

/*
CHANGE LOG most recent first order

1999.07.09  HH
   increased precision, 19 bytes mantissa now.

1999.06.09  LW All Aktiviere Siron
   eliminated too long comment line.
   New Format+Sequence of Log-Entries.

old CHANGE LOG

08.05.1998-30.05.1998  HH
   created new

24.08.1998  CS
new functions decari_to_packed, packed_to_decari, hzadec

02.03.1999  HH
   added new function decari_int.

27.07.1999  CF
   functions decari_to_packed, packed_to_decari need parameters
   with equal lentghs.

*/

/* 20 Bytes = 19 bytes mantissa = maximum 38 digits precision
 */
#define DECARI_N 20


/* DECARI_M = length of mantissa
 */
#define DECARI_M 19

/* PACKED_N = length of packed
 */
#define PACKED_N DECARI_N

/* ReturnCodes */

/* Decari <-> Packed */
#define RC_NKS                  11
#define RC_NO_PACKED            12
#define RC_NO_DECARI            13
#define RC_PACKED_OVERFLOW      14

/* hzadec */
#define RC_OPCO                 21

/* OperationCodes */

/* Konvertierung */
#define OC_PACKED_TO_DECARI      1
#define OC_DECARI_TO_PACKED      2
#define OC_DECARI_FROM_STRING    3
#define OC_DECARI_TO_STRING      4

/* Pruefen */
#define OC_DECARI_COMPARE       11
#define OC_DECARI_ISNAN         12

/* Rechnen */
#define OC_DECARI_ADD           21
#define OC_DECARI_SUB           22
#define OC_DECARI_MULT          23
#define OC_DECARI_DIV           24
#define OC_DECARI_ROUND         25




typedef struct sDecari
{
   unsigned char expo;
   unsigned char mant[ DECARI_M ] ;
} tDec ;

typedef struct
{
  tDec             *pd1;        /* 1. decari-Operant
                                   oder Input/Output bei Packed <->decari */
  tDec             *pd2;        /* 2. decari-Operant */
  tDec             *pd3;        /* 3. decari-Operant */
  char             *szDec;      /* Ergebnis bei Konvertierung in String */
  unsigned char    *Packed;     /* gepackte Zahl */
  short             nPrecision; /* Nachkommastellen */
  unsigned short    nScale;     /* Nachkommastellen bei Rundung
                                   Decari_round */
  char              cEF;        /* bei decari_to_string
                                   Darstellung EXP-Format oder nicht */
  char              filler00;   /* nicht genutzt */
  short             retco;      /* ReturnCode */
} tAll;

#define packed_to_decari   DECARI00
#define decari_to_packed   DECARI01
#define decari_add         DECARI02
#define decari_sub         DECARI03
#define decari_mult        DECARI04
#define decari_div         DECARI05
#define decari_compare     DECARI06
#define decari_from_string DECARI07
#define decari_to_string   DECARI08
#define decari_isnan       DECARI09
#define decari_round       DECARI10

/* Entry for HOST */
void hzadec (short *p_opco, tAll *pAll);

short packed_to_decari (tDec *pDec, const unsigned char *Packed,
                        short nks);
short decari_to_packed (const tDec *pDec, unsigned char *Packed,
                        short nks);
int decari_add( const tDec *pd1, const tDec *pd2, tDec *pd3 );
int decari_sub( const tDec *pd1, const tDec *pd2, tDec *pd3 );
int decari_mult( const tDec *pd1, const tDec *pd2, tDec *pd3 );
int decari_div( const tDec *pd1, const tDec *pd2, tDec *pd3 ) ;
int decari_compare( const tDec *pd1, const tDec *pd2);
int decari_from_string( tDec *pd1, const char *szDec) ;
void decari_to_string( char *szReturn, const tDec *pd1, short nPrecision, char cEF );
int decari_isnan( const tDec *pd );
void decari_round( tDec *pd, unsigned short nScale );
void decari_int( tDec *pd );

/* =========================================================== */
/* the following is needed by siron arith */

extern const tDec decari_1;
extern const tDec decari_0;
extern const tDec decari_10;
extern const tDec decari_m1;
extern const tDec decari_inf;
extern const tDec decari_minf;
extern const tDec decari_999;  /* 0.99999999999999999999999 */


#define decari_negate(p)   ((p)->expo ^= (unsigned char)0x80)

void decari_abs(tDec *pd);
int decari_sign(const tDec *pd);
int decari_ipow10(tDec *pd, int exp);

#define decari_equal(a,b) (decari_compare((a), (b)) == 0)

#endif

