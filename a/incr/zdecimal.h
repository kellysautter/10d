/*

   Flags for Zeidon Decimal support.

CHANGE LOG
2002.01.28  HH
   added zDEC_CMD_ROUND.
1998.05.14    DGC
   Added zDEC_CMD_INIT.

1999.04.19  RS
   Commentlines required for Rose.

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> //required for Rose

#ifdef __cplusplus
extern "C"
{
#endif

// Prototype for decimal operation.
typedef short (POPERATION zDECOPER) ( void * , void *, void *, long );

zSHORT OPERATION
zDecimalOperation( void  *pvReturn,
                   void  *pvDecimal,
                   void  *pvInfo,
                   long  lControl );

#define zDEC_CMD                    0x000000FFL

// Misc. commands.
#define zDEC_CMD_ASSIGNDEC          0x00000001L
#define zDEC_CMD_COMPAREDEC         0x00000002L
#define zDEC_CMD_ASSIGNNULL         0x00000003L
#define zDEC_CMD_COMPARENULL        0x00000004L
#define zDEC_CMD_COMPAREDOUBLE      0x00000005L
#define zDEC_CMD_INIT               0x00000006L

// Arithmetic commands
#define zDEC_CMD_ADDDEC             0x00000010L
#define zDEC_CMD_SUBDEC             0x00000011L
#define zDEC_CMD_MULTDEC            0x00000012L
#define zDEC_CMD_DIVDEC             0x00000013L
#define zDEC_CMD_ROUND              0x00000014L

// Conversion commands.
#define zDEC_CMD_DECTOSTRING        0x00000030L
#define zDEC_CMD_STRINGTODEC        0x00000031L
#define zDEC_CMD_DECTOLONG          0x00000032L
#define zDEC_CMD_LONGTODEC          0x00000033L
#define zDEC_CMD_DECTODOUBLE        0x00000034L
#define zDEC_CMD_DOUBLETODEC        0x00000035L

// Floating-point operations
#define zDEC_CMD_SQRT               0x00000040L
#define zDEC_CMD_EXPONENT           0x00000041L

#ifdef __cplusplus
}
#endif
