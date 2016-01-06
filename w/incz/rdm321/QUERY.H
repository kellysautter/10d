/*----------------------------------------------------------------------
   query.h  -- db_QUERY status code and external variables header file
               For use with db_VISTA V3.21

   Copyrights (c) Raima Corporation 1986 - 1992, All Rights Reserved.
----------------------------------------------------------------------*/

#ifndef _QUERY_
#define _QUERY_

#ifndef DB_ADDR
#include "vista.h"
#endif                                 /* DB_ADDR */


#ifdef NO_PASCAL
#define _pascal /**/
#endif

#define DB_LOADDS _loadds

/* keep compiler from squawking about unreferenced formal paramaters */
#ifndef REF_FORMAL
#define REF_FORMAL(x) if(x)
#endif

/* Current release version number */
#define db_QVERS "2.21A"


/* q_nextval value container */
#ifndef OM_EXPORT
#define OM_EXPORT    /* needed by OMAN */
#endif

typedef struct OM_EXPORT
{
   union OM_EXPORT
   {                       /* used tag-field based on val_type */
      int iv;              /* integer result value */
      long lv;             /* long result value */
      double dv;           /* long result value */
      DB_ADDR av;          /* database address result value */
      char DB_FAR *cv;     /* character result value */
   }  result;
   char type;              /* 'i' = int, 'l' = long, 'F' = double,
                              'd' = database address,
                              'c' = string (dynamically allocated),
                              'z' = div by zero, 'n' = null,
                              'u' = unsigned int, 'U' = unsigned long,
                              '\0' = none                               */
}  VALUE;

/* db_QUERY status code constants */
#define Q_OKAY          0

#define Q_END           -1
#define Q_SET           -2
#define Q_DEFINE        -3
#define Q_FIELD         -4
#define Q_RELATION      -5
#define Q_VIEW          -5
#define Q_BATCH         -6
#define Q_HELP          -7
#define Q_SHOW          -8
#define Q_NOTFOUND      -9
#define Q_UNAVAIL       -10
#define Q_BREAK         -11

/* db_QUERY error code constants */
/* The ones commented out are obsolete, but not deleted for maintenance. */
#define Q_INVSTMT       1
#define Q_INVFLD        2
#define Q_SYSERR        3
#define Q_FLDEXPR       4
/* #define Q_RELEXPR       5
   #define Q_RELOP         6
   #define Q_INVNUM        7
   #define Q_RANGE         8
*/
#define Q_FLDREL        9
#define Q_SORTED        10
#define Q_PRINTER       11
#define Q_FILERR        12
#define Q_INTOERR       13
#define Q_NOMEMORY      14
#define Q_LINKTO        15
#define Q_FLDSPEC       16
#define Q_DECIMALS      17
/* #define Q_FLDDESC       18
*/
#define Q_GROUPED       19
#define Q_LENGTH        20
#define Q_WIDTH         21
#define Q_TITLE         22
#define Q_DATE          23
#define Q_BADREL        24
/* #define Q_BADPATH       25
*/
#define Q_INVREC        26
#define Q_INVTHRU       27
#define Q_SETREL        28
#define Q_LINKREC       29
#define Q_NOFIELD       30
#define Q_NOLINK        31
#define Q_INVREL        32
/* #define Q_NOTSELECTED   33
*/
#define Q_MAXFIELDS     34
#define Q_TOOBIG        35
#define Q_NOBATCH       36
#define Q_SORTKEY       37
#define Q_NOFORM        38
#define Q_DDLFIELD      39
#define Q_DATEFMT       40
#define Q_TOOLONG       41
#define Q_OUTPUT        42
/* #define Q_NOTAKEY       43
   #define Q_INVPATHDIR    44
   #define Q_NOSELECT      45
   #define Q_FLDREF        46
   #define Q_NOEXPR        47
*/
#define Q_INVEXPR       48
/* #define Q_INVTERM       49
   #define Q_NOTERM        50
   #define Q_INVPRIM       51
   #define Q_MATHOPR       52
   #define Q_HUGE          53
*/
#define Q_INVVAL        54
#define Q_INVTYPE       55
#define Q_INVCALC       56
#define Q_REPVAL        57
#define Q_NOOCCUR       58
/* #define Q_INVGROUP      59
*/
#define Q_DISTINCT      60
#define Q_SUBSCRIPT     61
#define Q_NULLOUT       62
/* #define Q_ONEDB         63
*/
#define Q_MAXDIMS       64
#define Q_SUBRANGE      65
#define Q_INVDB         66
/* #define Q_KEYWORD       67
   #define Q_GROUPSEL      68
*/
#define Q_MAXSORT       69
#define Q_INVFMT        70
#define Q_NOMOREFLDS    71
#define Q_NOSUBS        72
#define Q_MAXCOLS       73
#define Q_MAXARGS       74
#define Q_MAXGROUP      75
#define Q_COLREF        76
#define Q_ORDPATH       77
#define Q_RECREL        78
#define Q_DIVBYZERO     79
#define Q_IFEXPR        80
#define Q_DISTARG       81
#define Q_SMALLCACHE    82
#define Q_BIGFORM       83
#define Q_DBOPEN        84
#define Q_NUMARGS       85
#define Q_INVFUNC       86
#define Q_INVARG        87
#define Q_BASEYR        88
#define Q_FLDWIDTH      89
#define Q_INVNOFMT      90
#define Q_FORKEY        91
#define Q_DISPLAY       92
#define Q_INVTASK       93
#define Q_INVPARM       94

#ifdef __cplusplus
extern "C"
{
#endif

#define QEXTERNAL_FIXED DB_FAR _pascal

/**************** db_QUERY User Function Prototypes **********************/

#ifdef ANSI
   typedef void (DB_FAR PASCAL DB_LOADDS * QCTODPROC) (char DB_FAR *,
                  double DB_FAR *);
   typedef void (DB_FAR PASCAL DB_LOADDS * QDISPLAYPROC) (CONST char DB_FAR *,
                  int, int, int);
   typedef int  (DB_FAR PASCAL DB_LOADDS * QFUNCTIONPROC) (char DB_FAR *,
                  int, VALUE DB_FAR *, VALUE DB_FAR *);
   typedef void (DB_FAR PASCAL DB_LOADDS * QPRINTERPROC) (CONST char DB_FAR *,
                  int, int, int, char DB_FAR *);
   typedef void (DB_FAR PASCAL DB_LOADDS * QTRANSLATEPROC) (char DB_FAR *,
                  long, char DB_FAR *, char DB_FAR *);
#else
   typedef void (DB_FAR PASCAL DB_LOADDS * QCTODPROC) ();
   typedef void (DB_FAR PASCAL DB_LOADDS * QDISPLAYPROC) ();
   typedef int  (DB_FAR PASCAL DB_LOADDS * QFUNCTIONPROC) ();
   typedef void (DB_FAR PASCAL DB_LOADDS * QPRINTERPROC) ();
   typedef void (DB_FAR PASCAL DB_LOADDS * QTRANSLATEPROC) ();
#endif

#ifdef ANSI
   int QEXTERNAL_FIXED dt_query(char DB_FAR *, int DB_FAR *,
                                char DB_FAR * DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_closetask(DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_open(char DB_FAR *, char DB_FAR *,
                               DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_opentask(DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_batch(char DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_close(DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_get_global(int, void DB_FAR *, unsigned,
                                     DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_hdrline(int, char DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_lockcount(int, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_nextrow(char DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_nextval(int DB_FAR *, VALUE DB_FAR *,
                                  DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_qdbpath(CONST char DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_qrfpath(CONST char DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_qdfpath(CONST char DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_set_ctod(QCTODPROC, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_set_displayfunc(QDISPLAYPROC, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_set_function(QFUNCTIONPROC, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_set_global(int, void DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_set_printerfunc(QPRINTERPROC, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_set_translate(QTRANSLATEPROC, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_sqlinit(char DB_FAR *, int DB_FAR *,
                                  char DB_FAR * DB_FAR *, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_on_opt(unsigned short, DB_TASK DB_FAR *);
   int QEXTERNAL_FIXED qt_off_opt(unsigned short, DB_TASK DB_FAR *);
#else    /* ANSI */
   int QEXTERNAL_FIXED dt_query();
   int QEXTERNAL_FIXED qt_closetask();
   int QEXTERNAL_FIXED qt_open();
   int QEXTERNAL_FIXED qt_opentask();
   int QEXTERNAL_FIXED qt_batch();
   int QEXTERNAL_FIXED qt_close();
   int QEXTERNAL_FIXED qt_get_global();
   int QEXTERNAL_FIXED qt_hdrline();
   int QEXTERNAL_FIXED qt_lockcount();
   int QEXTERNAL_FIXED qt_nextrow();
   int QEXTERNAL_FIXED qt_nextval();
   int QEXTERNAL_FIXED qt_qdbpath();
   int QEXTERNAL_FIXED qt_qrfpath();
   int QEXTERNAL_FIXED qt_qdfpath();
   int QEXTERNAL_FIXED qt_set_ctod();
   int QEXTERNAL_FIXED qt_set_displayfunc();
   int QEXTERNAL_FIXED qt_set_function();
   int QEXTERNAL_FIXED qt_set_global();
   int QEXTERNAL_FIXED qt_set_printerfunc();
   int QEXTERNAL_FIXED qt_set_translate();
   int QEXTERNAL_FIXED qt_sqlinit();
   int QEXTERNAL_FIXED qt_on_opt();
   int QEXTERNAL_FIXED qt_off_opt();
#endif      /* ANSI */

#ifndef   NO_QT_COVER
#define  d_query(a, b, c)       dt_query(a, b, c, &Currtask)
#define  q_open(a, b)           qt_open(a, b, &Currtask)
#define  q_batch(a)             qt_batch(a, &Currtask)
#define  q_close()              qt_close(&Currtask)
#define  q_get_global(a, b, c)  qt_get_global(a, b, c, &Currtask)
#define  q_hdrline(a, b)        qt_hdrline(a, b, &Currtask)
#define  q_lockcount(a)         qt_lockcount(a, &Currtask)
#define  q_nextrow(a)           qt_nextrow(a, &Currtask)
#define  q_nextval(a, b)        qt_nextval(a, b, &Currtask)
#define  q_qdbpath(a)           qt_qdbpath(a, &Currtask)
#define  q_qrfpath(a)           qt_qrfpath(a, &Currtask)
#define  q_qdfpath(a)           qt_qdfpath(a, &Currtask)
#define  q_set_ctod(a)          qt_set_ctod(a, &Currtask)
#define  q_set_displayfunc(a)   qt_set_displayfunc(a, &Currtask)
#define  q_set_function(a)      qt_set_function(a, &Currtask)
#define  q_set_global(a, b)     qt_set_global(a, b, &Currtask)
#define  q_set_printerfunc(a)   qt_set_printerfunc(a, &Currtask)
#define  q_set_translate(a)     qt_set_translate(a, &Currtask)
#define  q_sqlinit(a, b, c)     qt_sqlinit(a, b, c, &Currtask)
#define  q_on_opt(a)            qt_on_opt(a, &Currtask);
#define  q_off_opt(a)           qt_off_opt(a, &Currtask);
#endif         /* NOT NO_QT_COVER */


#ifdef __cplusplus
}
#endif
/**************************************************************************/

#define MAXSTRING     512  /* Max len of any quoted string in a statement */
                           /*    and max length of output string.         */
#define MAXTRANS      128  /* Max len of translated coded-value string */
#define MAXERRTXT      64  /* Max len of qerrinfo string */
#define MAXERRMSGTXT   80  /* Max len of error message */
#define MAXSTMT      4096  /* Max len of any SQL statement */
#define PRLEN          40  /* max len of qprompt  string */
#define LPLEN          23  /* max len of qprinter string */

/* q_get_global and q_set_global global ids */

#define QG_PUBLICBASE     0                    /* NOT MODIFIABLE */

#define QG_QBREAK          QG_PUBLICBASE + 1
#define QG_QECHO           QG_PUBLICBASE + 2
#define QG_QERRINFO        QG_PUBLICBASE + 3
#define QG_QFORMFEED       QG_PUBLICBASE + 4
#define QG_QMAXALIASES     QG_PUBLICBASE + 5
#define QG_QTABSTOP        QG_PUBLICBASE + 6
#define QG_QUSEOPTKEY      QG_PUBLICBASE + 7
#define QG_QREPSIZE        QG_PUBLICBASE + 8
#define QG_QTRACE          QG_PUBLICBASE + 9
#define QG_QERRPOS         QG_PUBLICBASE + 10
#define QG_QERRMSG         QG_PUBLICBASE + 11
#define QG_QPROMPT         QG_PUBLICBASE + 12
#define QG_QLOCKCOUNT      QG_PUBLICBASE + 13
#define QG_QWILDONE        QG_PUBLICBASE + 14
#define QG_QWILDALL        QG_PUBLICBASE + 15
#define QG_QCOLSEP         QG_PUBLICBASE + 16
#define QG_QROWSEP         QG_PUBLICBASE + 17
#define QG_QWIDTH          QG_PUBLICBASE + 18
#define QG_QLENGTH         QG_PUBLICBASE + 19
#define QG_QYIELDCOUNT     QG_PUBLICBASE + 20
#define QG_QHASHSIZE       QG_PUBLICBASE + 21

#define QO_VIEW2CACHE      0x0001   /* check worst case view vs cache size */
#define QO_TITLE2UPPER     0x0002   /* force default column titles to CAPS */

/* display and printer function line output types */
#define    Q_HEADERLINE     0
#define    Q_DETAILLINE     1
#define    Q_BREAKLINE      2
#define    Q_FOOTERLINE     3
#define    Q_SHOWLINE       4
#define    Q_ERRORLINE      5
#define    Q_FORMFEEDLINE   6

/* Handle open address hash length, used by qhdl_xxx hash functions in qfar.c           */
#define  HDLHASHSIZE  701              /* Linear hash range will be 0 to
                                        * HDLHASHSIZE-1. If the hash *
                                        * load factor exceeds .8 then the
                                        * hash will be increased in * size
                                        * by HDLHASHSIZE.                                      */

/*
   Simple open address hash function, used by qhdl_xxx
   hash functions in qfar.c
*/
#define  HDLHASHFUNC(h)    ((h) % (HDLHASHSIZE * qphdl_hash.ld.size))

/*
   Load factor macro. Macro computes the load factor of hash table used by
   qhdl_xxxx hash functions.  The Hash table will be increased in size when
   load factor exceeds 0.8.
*/
#define  HDLLOADFACTOR     ((double)qhdl_count / (HDLHASHSIZE * qphdl_hash.ld.size))

#define  HDLSLOTEMPTY    0             /* marks hash slot as empty */
#define  HDLSLOTDELETED  -1            /* marks hash slot as deleted */

typedef struct
{
   HANDLE handle;
   short lock_cnt;
   int size;
}  qlock_desc;

#define QLOCK_DESC  qlock_desc  ld;

typedef struct
{
   char _far *ptr;
      QLOCK_DESC
}  QCHAR_P;
typedef struct
{
   QCHAR_P _far *ptr;
      QLOCK_DESC
}  QCHAR_PP;
typedef struct
{
   DB_ADDR _far *ptr;
      QLOCK_DESC
}  QDB_ADDR_P;
typedef struct
{
   int _far *ptr;
      QLOCK_DESC
}  QINT_P;

#endif                                 /* _QUERY_ */

/* vpp @win.vpp QUERY.H (Wed Jun 10 16:29:37 1992)
 *
 *     -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS
 *     -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC
 *     -nARCHIVING -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES
 *     -dMULTI_TASK -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP
 *     -nSINGLE_USER -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4
 *     -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY -nTRUENAME -nLAT
 */

