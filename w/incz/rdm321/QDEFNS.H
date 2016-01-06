/*-------------------------------------------------------------------------
   qdefns.h - Header file for db_QUERY declarations

   Copyrights (c) Raima Corporation 1986 - 1992, All Rights Reserved.
-------------------------------------------------------------------------*/
/* *************************** INCLUDE FILES *************************** */

#ifndef _TM_DEFINED
#include <time.h>
#endif

#define _QDEFNS_                       /* indicates that this file has
                                        * been included */

/* systems that do not provide a stricmp can define NEED_STRICMP to
 * use a local version called _qstricmp()
 */


/* db_VISTA header files like MSC, TURBO, or ZOR defined as follows */
#ifdef MSC                             /* Microsoft 'C' */
#define ANSI
#endif

#ifdef TURBO                           /* Borland 'C' */
#define ANSI
#ifndef __TIME_H
#include <time.h>                      /* so BCP won't complain about
                                        * undefined tm struct */
#endif
#endif

#ifdef ZOR                             /* Zortech 'C' */
#define ANSI
#endif

#define WINDOWS
#define ANSI



#ifdef NO_CONST
#define CONST     /**/
#else
#define CONST     const
#endif

#define MULTI_TASK
#define WFSTATIC  static      /*  Used to make auto variables DS based */


#ifndef QFAR_C
#define NO_WINDOWS_H
#endif

#define _FWINDOWS    /*  Far pointer version */

#define stdin   (FILE *)0
#define stdout  (FILE *)1
#define stderr  (FILE *)2
#define stdaux  (FILE *)3
#define stdprn  (FILE *)4

/**************************** DLL Only stuff follows: **********************/

#define WFAR      _far

#define QSTATIC   /* */       /* The segmentor can not have static fcns. */

#ifdef NO_WINDOWS_H           /* windows.h already included */
/* Prototypes taken from Microsoft's WINDOWS.H and WINDOWSX.H.
   We should not do this, but those headers are by far too big.
*/
#define PM_REMOVE   0x0001
typedef struct tagMSG
{
   unsigned int   hwnd;
   unsigned int   message;
   unsigned int   wParam;
   long           lParam;
   unsigned long  time;
   struct {short x, y;} pt;
} MSG;

unsigned int far pascal PeekMessage(MSG far *, unsigned, unsigned, unsigned,
                                    unsigned);
unsigned int far pascal TranslateMessage(MSG far *);
long         far pascal DispatchMessage(MSG far *);

int _far _cdecl wsprintf(char _far *, char _far *,...);
int _far _pascal MessageBox(unsigned int, char _far *, char _far *,
                           unsigned int);
unsigned int _far _pascal GetFocus(void);
int _far _pascal wvsprintf(char _far *, char _far *, char _far *);
void _far _pascal Yield(void);

unsigned int _far _pascal GlobalAlloc(unsigned int, unsigned long);
unsigned int _far _pascal GlobalFree(unsigned int);
unsigned long _far _pascal GlobalHandle(unsigned int);
void _far * _far _pascal GlobalLock(unsigned int);
int _far _pascal GlobalUnlock(unsigned int);

#ifndef OF_READ
#define OF_READ               0x0000
#endif
#ifndef OF_WRITE
#define OF_WRITE              0x0001
#endif
#ifndef OF_READWRITE
#define OF_READWRITE          0x0002
#endif
#ifndef OF_SHARE_EXCLUSIVE
#define OF_SHARE_EXCLUSIVE    0x0010
#endif
#ifndef OF_SHARE_DENY_WRITE
#define OF_SHARE_DENY_WRITE   0x0020
#endif
#ifndef OF_SHARE_DENY_READ
#define OF_SHARE_DENY_READ    0x0030
#endif
#ifndef OF_SHARE_DENY_NONE
#define OF_SHARE_DENY_NONE    0x0040
#endif
#ifndef OF_CREATE
#define OF_CREATE             0x1000
#endif

#define GMEM_MOVEABLE         0x0002
#define GMEM_ZEROINIT         0x0040

#define LOWORD(l)           ((unsigned short)(unsigned long)(l))
#define HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xFFFF))
#define SELECTOROF(lp)      HIWORD(lp)

#define     GlobalPtrHandle(lp)        ((unsigned int)LOWORD(GlobalHandle(SELECTOROF(lp))))
#define     GlobalUnlockPtr(lp)        GlobalUnlock(GlobalPtrHandle(lp))
#define     GlobalAllocPtr(flags, cb)  (GlobalLock(GlobalAlloc((flags), (cb))))
#define     GlobalFreePtr(lp)          (GlobalUnlockPtr(lp), (int)GlobalFree(GlobalPtrHandle(lp)))

#endif /* N_WINDOWS_H */

#define _qmemcmp  _fmemcmp
#define _qmemcpy  _fmemcpy
#define _qmemset  _fmemset
#define _qstrcat  _fstrcat
#define _qstrchr  _fstrchr
#define _qstrcmp  _fstrcmp
#define _qstricmp _fstricmp
#define _qstrcpy  _fstrcpy
#define _qstrlen  _fstrlen
#define _qstrncmp _fstrncmp
#define _qstrncpy _fstrncpy
#define _qstrncat _fstrncat
#define _qstrupr  _fstrupr
#define _qunlink  v_unlink

/* only works as long as there is no floating point format strings */
#define sprintf   wsprintf

#define _qfree(ptr)           GlobalFreePtr(ptr)
#define _qmalloc(size)        GlobalAllocPtr(GMEM_MOVEABLE, size)
#define _qcalloc(num, size)   GlobalAllocPtr((GMEM_MOVEABLE|GMEM_ZEROINIT), (num * size))



#ifdef NEED_STRICMP
#undef _qstricmp
#ifdef ANSI
int _qstricmp (char *, char *);
#else
int _qstricmp ();
#endif
#endif

#define NO_DT_COVER                    /*  Don't substitute names */
#define NO_QT_COVER                    /*  Don't substitute names */
#define DTCOVER_H                      /*  Don't substitute names */

#include "vista.h"
#define NO_STD_H                       /* dbtype.h conflict resolved */
#include "dbtype.h"
#include "query.h"

/*
   TO THE USER, THE MULTI_TASK (qt_) AND SINGLE_TASK (q_)
   FUNCTIONS ARE SUPPOSED TO BE MUTUALLY EXCLUSIVE.
   HOWEVER, WE NEED THEM INTERNALLY.
*/
#ifdef ANSI
   int QEXTERNAL_FIXED d_query(char DB_FAR *stmt, int DB_FAR *epos,
                               char DB_FAR *DB_FAR *emsg);
   int QEXTERNAL_FIXED q_batch(char DB_FAR *batfile);
   int QEXTERNAL_FIXED q_close(void);
   int QEXTERNAL_FIXED q_hdrline(int hline, char DB_FAR *text);
   int QEXTERNAL_FIXED q_open(char DB_FAR *db_list, char DB_FAR *type);
   int QEXTERNAL_FIXED q_lockcount(int);
   int QEXTERNAL_FIXED q_nextrow(char DB_FAR *);
   int QEXTERNAL_FIXED q_nextval(int DB_FAR *, VALUE DB_FAR *);
   int QEXTERNAL_FIXED q_sqlinit(char DB_FAR *str, int DB_FAR *ep,
                                 char DB_FAR *DB_FAR *em);
   int QEXTERNAL_FIXED q_set_ctod(QCTODPROC func);
   int QEXTERNAL_FIXED q_set_displayfunc(QDISPLAYPROC func);
   int QEXTERNAL_FIXED q_set_printerfunc(QPRINTERPROC func);
   int QEXTERNAL_FIXED q_set_function(QFUNCTIONPROC func);
   int QEXTERNAL_FIXED q_set_translate(QTRANSLATEPROC func);
   int QEXTERNAL_FIXED q_get_global(int id, DB_VOID DB_FAR * fpDestIn,
                                    unsigned index);
   int QEXTERNAL_FIXED q_set_global(int id, DB_VOID DB_FAR * fpSrcIn);
   int QEXTERNAL_FIXED q_qrfpath(CONST char DB_FAR * path);
   int QEXTERNAL_FIXED q_qdfpath(CONST char DB_FAR * path);
   int QEXTERNAL_FIXED q_qdbpath(CONST char DB_FAR * path);
   int QEXTERNAL_FIXED q_on_opt(unsigned short);
   int QEXTERNAL_FIXED q_off_opt(unsigned short);
#else         /* ANSI */
   int QEXTERNAL_FIXED d_query();
   int QEXTERNAL_FIXED q_batch();
   int QEXTERNAL_FIXED q_close();
   int QEXTERNAL_FIXED q_hdrline();
   int QEXTERNAL_FIXED q_open();
   int QEXTERNAL_FIXED q_lockcount();
   int QEXTERNAL_FIXED q_nextrow();
   int QEXTERNAL_FIXED q_nextval();
   int QEXTERNAL_FIXED q_sqlinit();
   int QEXTERNAL_FIXED q_set_ctod();
   int QEXTERNAL_FIXED q_set_displayfunc();
   int QEXTERNAL_FIXED q_set_function();
   int QEXTERNAL_FIXED q_set_printerfunc();
   int QEXTERNAL_FIXED q_set_translate();
   int QEXTERNAL_FIXED q_get_global();
   int QEXTERNAL_FIXED q_set_global();
   int QEXTERNAL_FIXED q_qrfpath();
   int QEXTERNAL_FIXED q_qdfpath();
   int QEXTERNAL_FIXED q_qdbpath();
   int QEXTERNAL_FIXED q_on_opt();
   int QEXTERNAL_FIXED q_off_opt();
#endif         /* ANSI */

#define QINTERNAL_FIXED _pascal
#define QSTATICPROC     _near QINTERNAL_FIXED

#define QACT            QSTATICPROC

/* *********************** CODE MACRO DEFINITIONS ********************** */
#ifdef  NO_FMT
#define NO_OUTPUT
#endif

#ifdef  NO_OUTPUT
#define NO_SHOW
#endif

/* database # of QUERYDBNM */
#define QDBNUM (no_of_dbs-1)

/* general memory allocation macro */
#define QMALLOC(p,s) if(((p) = qNewObject(sizeof(s))) == NULL) return(Q_NOMEMORY);

/* keep compiler from squawking about unreferenced formal paramaters */
#ifndef REF_FORMAL
#define REF_FORMAL(x) if(x)
#endif

/* ********************** GENERAL SIZING CONSTANTS ********************* */
#define NAMELEN   33       /* maximum identifier length + null */
#define MAXARGS   8        /* maximum no. of query procedure arguments */

/********************************************************************
   if (page_size == 1K)
   {
      MAXKEYDATA = ((1k - key_page_ovrhd) / 4 - key_slot_ovrhd) ==> 243);

      Since it should be an word aligned (4 byte word), ==> 240
   }
   else
   {
      (page_size > 1K)
      MAXKEYDATA = ((MAXKEYSIZE - key_slot_ovrhd) => 246);

      Since it should be an word aligned (4 byte word), ==> 244
   }

   The code to compute this is not worth the 4 extra bytes.
*********************************************************************/
#define MAXKEYDATA   240

#define DEFLINES  25                /* Default number of lines on screen */

#define MACNESTMAX 8       /* maximum macro nesting level */
#define MAXOR 20           /* max. number of or'd record types in a path */

/* *********************** CONSTANT DECLARATIONS *********************** */

/* where expr evaluation returns TRUE, FALSE, and INDETERMINATE */
#define INDETERMINATE -1

/* additional data types for query */
#define MONEY 'M'
#define FIXED 'X'
#define DATE  'D'
#define TIME  'T'

/* report output device type */
#define NULL_OUT  0
#define TERM_OUT  1
#define PRT_OUT   2
#define FILE_OUT  3
#define ASCII_OUT 4
#define DIF_OUT   5

/* numeric format type */
#define E_PACKED 'p'
#define E_COMMA  'c'
#define E_EXP    'e'

/* expression operator types */
#define E_PLUS          '+'
#define E_MINUS         '-'
#define E_MULT          '*'
#define E_DIVIDE        '/'

/* calc functions */
#define E_AVG           'a'
#define E_COUNT         'C'
#define E_MAX           'M'
#define E_MIN           'm'
#define E_SUM           's'
#define E_NOCALC        '\0'

/* string functions */
#define E_LOWER         'l'
#define E_UPPER         'u'
#define E_STRLEN        'L'
#define E_SUBSTR        'S'
#define E_TRIM          't'

/* date functions */
#define E_DAY           'A'
#define E_MONTH         'o'
#define E_WEEKDAY       'w'
#define E_YEAR          'y'

/* expr_leaf types */
#define E_CALC          'c'
#define E_STRING        'z'
#define E_FIELD         'f'
#define E_VALUE         'v'
#define E_IF            'i'
#define E_DBA           'd'
#define E_DATE          'D'
#define E_CONCAT        '^'
#define E_USER          'U'

/* value types:  CHANGE THESE AND YOU MUST CHANGE THE NEXT GROUP */
#define E_NOVAL         0
#define E_SHORT         1
#define E_INT           2
#define E_LONG          3
#define E_DOUBLE        4
#define E_CHAR          5
#define E_DBADDR        6
#define E_FLOAT         7
#define E_ZERODIV       8
#define E_NULL          9
#define E_USHORT       10
#define E_UINT         11
#define E_ULONG        12

/* binary arithmetic operation operand type codes (L * 10 + R) */
#define E_NC            04             /* NULL + string   */
#define E_II            22             /* int + int       */
#define E_IL            23             /* int + long      */
#define E_ID            24             /* int + double    */
#define E_LI            32             /* long + int      */
#define E_LL            33             /* long + long     */
#define E_LD            34             /* long + double   */
#define E_DI            42             /* double + int    */
#define E_DL            43             /* double + long   */
#define E_DD            44             /* double + double */
#define E_CC            55             /* string + string */
#define E_CN            50             /* string + NULL   */
#define E_AA            66             /* dba + dba       */

/* string type codes */
#define E_NOSTR         '\0'
#define E_STATIC        's'
#define E_DYNAMIC       'd'
#define E_SLIT          'l'
#define E_FLIT          'f'

/* translation types */
#define E_USING         'T'      /* FIELD stmt TRANSLATE USING clause used */
#define E_VALUES        'V'      /* FIELD stmt VALUES ARE clause used */

/* key check optimization operators */
#define K_LIKE_NP 0x01
#define K_LT      0x02
#define K_LE      0x03
#define K_GT      0x08
#define K_GE      0x0c
#define K_GTLT    0x0b
#define K_GTLE    0x0d
#define K_GELT    0x0e
#define K_RANGE   0x0f
#define K_LIKE_WP 0x10
#define K_IN      0x20
#define K_EQ      0x30
#define K_NOP     0x50

/* output control codes */
#define O_ROW    'r'                /* output select resultant row */
#define O_BREAK  'b'                /* output column break skips or pages */
#define O_HSEP   'h'                /* output row of horizontal separators */
#define O_PAGE   'p'                /* format & output page summary */
#define O_REPORT 'r'                /* format & output report summary */


#define INIT_FIRST_TIME  0
#define INIT_AFTER_CLOSE 1
#define INIT_DONE        2
#define INIT_ALL         3
#define INIT_SOME        4


/* ************************* TYPE DECLARATIONS ************************* */

/* lock request packet info per open database */
typedef struct
{
   int lrp_size;                       /* no. of record types to be locked */
   LOCK_REQUEST WFAR *lrp;             /* lock request packet pointer */
}  QDB_LOCKS;


/* data value container */
typedef union
{                          /* tag-field based on val_type */
   short    sv;            /* short integer value */
   int      iv;            /* integer result value */
   long     lv;            /* long result value */
   float    fv;            /* float result value */
   double   dv;            /* long result value */
   DB_ADDR  av;            /* database address result value */
   struct
   {
      size_t      bl;      /* buffer length >= string length */
      char WFAR * sv;      /* string result value */
      char        st;      /* string type: '\0' = undetermined,
                              's' = static, 'd' = dynamic,
                              'l' = select literal, 'f' = field literal */
   }  cv;
}  VAL_TYPE;

typedef struct val_entry
{
   struct val_entry WFAR *next;        /* sometimes used as linked list */
   VAL_TYPE vt;                        /* result value */
   char val_type;                      /* E_INT, E_UINT, E_LONG, E_ULONG,
                                          E_DOUBLE, E_DBADDR, E_CHAR */
}  VAL_ENTRY;

/* potential key check field list */
typedef struct key_check
{
   struct key_check WFAR *next;        /* usual linked list */
   int key;                            /* field_table index of key field */
   int fld;                /* field_table index of check field */
   int opr;                /* encoded relational operation */
   VAL_ENTRY WFAR *lo;     /* low val in range (single value if not range) */
   VAL_ENTRY WFAR *hi;     /* high value in range */
}  KEY_CHECK;

/* relation path node descriptor */
typedef struct path_entry
{
   struct path_entry WFAR *next;                /* next path node */
   struct path_entry WFAR *prev;                /* previous path node */
   struct path_entry WFAR *WFAR * setups;       /* null terminated list of setup nodes */
   char WFAR *occur_name;                       /* occurrence name */
   DB_ADDR dba;                  /* db address of last record accessed */
   int chpos;                    /* char position in stmt of this path_seg */
   int dbn;                      /* number of database containing record */
   int to_rec;                   /* record_table index of record to access */
   int thru;                     /* set or field relating the records */
   int link;                     /* link to field */
   char lstat;       /* lock status: 'l' = locked by query, 'f' = free,
                        'L' = locked outside query, 'd' = duplicate record */
   char path_op;     /* path operation:   'm' = thru member, 'o' = thru owner,
                        'f' = thru file, 's' = thru scan,  'l' = thru link,
                        'd' = thru db_addr */
   union
   {
      KEY_CHECK WFAR *optimize;  /* key scan optimization info */
      int WFAR *members;         /* "or'd" member record types, link = count */
      char DB_FAR *keyvalue;     /* source key value for keyed 'link to' */
      DB_ADDR DB_FAR *sdba;      /* db_addr value for direct 'link to' */
   }  op;
}  PATH_ENTRY;

/* code/replacement pairs */
typedef struct val_pair
{
   struct val_pair WFAR *next;         /* next value pair in list */
   VAL_ENTRY WFAR *code_val;           /* value of coded value in field */
   char WFAR *repl_text;               /* replacement text for code_val */
}  VAL_PAIR;

/* type of coded value substitution */
typedef struct trans_type
{
   char tr_type;                       /* E_USING or E_VALUES */
   union
   {
      VAL_ENTRY WFAR *tr_using;  /* if tr_type==E_USING (arg to q_translate) */
      VAL_PAIR WFAR *val_list;   /* if tr_type==E_VALUES */
   }  tt;
}  TRANS_TYPE;

/* display format */
typedef struct
{
   char f_type;               /* display type */
   char f_pos;                /* positioning within field */
   char f_num;                /* numeric format: E_EXP, E_PACKED, E_COMMA */
   int f_dec;                 /* number of decimal places */
   int f_width;               /* display width */
   TRANS_TYPE WFAR *f_trans;  /* NULL if not TRANSLATE USING or VALUES ARE */
}  FORMAT;

/* data field occurrence descriptor */
typedef struct occur_ptr
{
   struct occur_ptr WFAR *next;  /* linked list of occurrence names */
   char WFAR *occur_name;        /* occurrence name or NULL if unused entry */
   char DB_FAR *data;            /* pointer to data in db cache */
   int dbn;                      /* coded_value() needs a dbn for qfldref() */
}  OCCUR_PTR;

/* expression pointer descriptor */
typedef struct expr_ptr
{
   char type;                    /* type of pointer: 'n' = node, 'l' = leaf */
   union
   {
      struct expr_node WFAR *node;
      struct expr_leaf WFAR *leaf;
   }  np;
}  EXPR_PTR;

/* field info descriptor */
typedef struct field_info
{
   int link;                           /* 'link_to' field_table index */
   char WFAR *desc;                    /* field description */
   char WFAR *occur;                   /* 'field alias = occur.fldname;' */
   FORMAT fmt;                         /* display format */
   EXPR_PTR expr;                      /* aliased expression */
}  FIELD_INFO;

/* field name table */
typedef struct field_name
{
   char WFAR *name;                    /* name of field (alias) */
   int fld;                            /* field_table index for field */
   FIELD_INFO WFAR *info;              /* field statement info */
}  FIELD_NAME;

/* data field value descriptor */
typedef struct field_value
{
   FIELD_NAME WFAR *field;             /* data field whose value this is */
   OCCUR_PTR WFAR *occur;              /* points to qfldval entry */
   short length;                       /* length of selected array element */
   short offset;                       /* byte offset to array element */
}  FIELD_VALUE;

/* conditional expression descriptor */
typedef struct cond_expr
{
   struct cond_expr WFAR *left;     /* pointer to left branch of expr tree */
   struct cond_expr WFAR *right;    /* pointer to right branch of expr tree */
   struct rel_expr WFAR *expr;      /* if leaf, pointer to relational expr */
   int opr;                         /* T_AND, T_OR or T_XOR */
}  COND_EXPR;

/* relational expression descriptor */
typedef struct rel_expr
{
   int opr;                   /* T_IN=list, T_BETWEEN=range, T_CONTAINS=text */
                              /* or T_EQ, T_NE, T_LT, T_LE, T_GT, T_GE) */
   BOOLEAN not;               /* TRUE if rel_expr is "NOT cond_expr" */
   EXPR_PTR lexp;             /* left hand expression */
   EXPR_PTR rexp;             /* right hand expression */
                              /* if .opr !(T_IN||T_BETWEEN||T_CONTAINS), */
   VAL_ENTRY WFAR *val;       /* text/range-start/item-list-start, (range- */
}  REL_EXPR;                  /* end or next-in-list in val.next) */

/* expression tree node */
typedef struct expr_node
{
   int opr;                   /* operator: E_PLUS, E_MINUS, E_MULT, E_DIV */
   EXPR_PTR left;             /* left operand */
   EXPR_PTR right;            /* right operand */
}  EXPR_NODE;

/* SQL calc function descriptor */
typedef struct calc_expr
{
   char func;                 /* E_AVG, E_SUM, E_MIN, E_MAX, E_COUNT */
   BOOLEAN distinct;          /* TRUE if DISTINCT in function, else FALSE */
   long count;                /* # of values contributing to val */
   VAL_ENTRY val;             /* current value of calc function  */
   VAL_ENTRY xval;            /* result of expression */
   EXPR_PTR expr;             /* expression on which calc fcn operates */
}  CALC_EXPR;

/* calc function list descriptor */
typedef struct calc_list
{
   struct calc_list WFAR *next;  /* pointer to next calc_list entry */
   CALC_EXPR WFAR *calc;         /* pointer to calc_expr used in select stmt */
}  CALC_LIST;

/* date function descriptor */
typedef struct dat_func
{
   char func;                          /* E_DAY, E_MONTH, E_YEAR, E_WEEKDAY */
   EXPR_PTR expr;                      /* must evaluate to a (julian) date */
}  DAT_FUNC;

/* string function descriptor */
typedef struct str_func
{
   char func;               /* E_LOWER, E_UPPER, E_STRLEN, E_SUBSTR, E_TRIM */
   EXPR_PTR expr;           /* string expression */
   int start;               /* sub-string start position */
   int length;              /* length of sub-string */
}  STR_FUNC;

/* string concatenation list */
typedef struct cat_list
{
   struct cat_list WFAR *next;         /* next cat_list entry */
   EXPR_PTR prim;                      /* expr_ptr to primary expr_leaf */
}  CAT_LIST;

/* IF function descriptor */
typedef struct if_expr
{
   COND_EXPR WFAR *cond;       /* if condition */
   EXPR_PTR t_exp;             /* expression to execute when cond is true */
   EXPR_PTR f_exp;             /* expression to execute when cond is false */
}  IF_EXPR;

/* expression list */
typedef struct expr_list
{
   struct expr_list WFAR *next;        /* next expr_list entry */
   EXPR_PTR expr;                      /* expression pointer */
   VAL_ENTRY val;                      /* expression result */
}  EXPR_LIST;

/* user function descriptor */
typedef struct user_fcn
{
   char WFAR *name;                    /* user function name */
   EXPR_LIST WFAR *xlist;              /* expression argument list */
   int count;                          /* number of expression arguments */
   char type;                          /* data type */
}  USER_FCN;

/* database address function descriptor */
typedef struct dba_fcn
{
   char WFAR *recname;                 /* record name */
   int rectype;                        /* record type */
   PATH_ENTRY WFAR *node;              /* pointer to path node */
} DBA_FCN;

/* db_addr function reference descriptor */
typedef struct dba_list
{
   struct dba_list WFAR *next;      /* next db_addr reference */
   DBA_FCN WFAR *dba;               /* pointer to related E_DBA expr_leaf */
}  DBA_LIST;

/* expression tree leaf */
typedef struct expr_leaf
{
   char type;                       /* E_FIELD, E_VALUE, E_CALC, E_IF,
                                       E_STRFCN, E_CONCAT, E_USER, E_DBA */
   char sign;                       /* E_PLUS or E_MINUS */
   union
   {
      FIELD_VALUE fld_value;           /* E_FIELD */
      VAL_ENTRY lit_value;             /* E_VALUE */
      CALC_EXPR WFAR *fcn_value;       /* E_CALC */
      IF_EXPR if_value;                /* E_IF */
      STR_FUNC str_value;              /* E_STRFCN */
      CAT_LIST cat_value;              /* E_CONCAT */
      USER_FCN usr_value;              /* E_USER */
      DAT_FUNC dat_value;              /* E_DATE */
      DBA_FCN dba_value;               /* E_DBA */
   }  x;
}  EXPR_LEAF;

/* select column descriptor */
typedef struct
{
   char WFAR *heading;           /* column heading */
   EXPR_PTR expr;                /* column spec. */
   VAL_ENTRY val;                /* expr result */
   FORMAT fmt;                   /* column's display format */
   VAL_ENTRY WFAR *distcpy;      /* copy of last value for column distinct */
}  COLUMN_DESCR;

/* list of column selections for break on statement */
typedef struct col_list
{
   struct col_list WFAR *next;      /* usual linked list */
   long count;                      /* # of values contributing to result */
   int col;                         /* qcol_list entry */
   VAL_ENTRY val;                   /* current calc or break value */
}  COL_LIST;

/* break on statement descriptor */
typedef struct break_descr
{
   struct break_descr WFAR *next;   /* usual linked list */
   char func;                       /* calc fcn code - e.g. E_SUM */
   char WFAR *label;                /* alternate row label */
   COL_LIST WFAR *calc_cols;        /* list of cols on which calcs are to be made */
   COL_LIST WFAR *brk_cols;         /* list of break cols (null => break on report) */
   short skips;                     /* number of lines to skip after break */
   short pages;                     /* number of pages to skip after break */
   int break_no;                    /* number of break calc in calculation */
}  BREAK_DESCR;

/* relation descriptor */
typedef struct relation
{
   struct relation WFAR *next;      /* usual linked list */
   char WFAR *name;                 /* relation name */
   char WFAR *desc;                 /* relation description */
   PATH_ENTRY WFAR *path;           /* relation path */
   char type;                       /* 'r' = record, 's' = set, 'p' = path */
}  RELATION;

/* macro descriptor */
typedef struct macro
{
   struct macro WFAR *next;            /* usual linked list */
   char WFAR *term;                    /* name of macro */
   char WFAR *defn;                    /* macro definition */
   char WFAR *desc;                    /* macro description */
}  MACRO;

/* help text descriptor */
typedef struct help_text
{
   char term_type;                  /* field, record, set, or relation */
   char WFAR *desc;                 /* help text */
   struct help_text WFAR *alt;      /* alternate help for duplicate names */
}  HELP_TEXT;

/* *********************** LOCAL DECLARATIONS batch.c ************************ */
struct qsqlstack
{
   struct qsqlstack WFAR *prev;
   FILE *sqlfp;
};

/* *********************** LOCAL DECLARATIONS lexanal.c ************************ */
extern struct qtext
{
   char DB_FAR *qstr;                  /* query text string */
   int qpos;                           /* current input position in qstr */
};

/* ******************** LOCAL VARIABLES memory.c ******************** */
/* allocated entry information */
typedef struct alloc_entry
{
   size_t size;                     /* size of allocated area */
   char WFAR *ptr;                  /* pointer to allocated area */
   char WFAR *freed;                /* pointer to copy of allocated area */
}  ALLOCATION;

typedef struct bucket
{
   struct bucket DB_FAR *next;      /* pointer to next bucket when filled */
   int entries;                     /* number of used entries */
   ALLOCATION DB_FAR *alloc;        /* allocated entry array */
}  BUCKET;

#define NUL_BUCKET ((BUCKET DB_FAR *)0)

/* ************************* LOCAL VARIABLES output.c ************************* */
/* column reference info packet used by non-native (regular) library */
typedef struct col_ref
{
   struct col_ref WFAR *next;    /* next column reference */
   int col;                      /* qcol_list index of refd column */
   char WFAR *pos;               /* character position of start of field */
}  COL_REF;

/* report line packet */
typedef struct rep_line
{
   char WFAR *text;              /* line of report form text */
   COL_REF WFAR *cols;           /* ptr to first column reference */
}  REP_LINE;

/* break section packet */
typedef struct brk_sectn
{
   struct brk_sectn WFAR *next;  /* link list in order of spec. */
   REP_LINE WFAR *formsum;       /* 1st report line in summary sections */
   int lines;                    /* no. of lines in section */
}  BRK_SECTN;


/********************** string type token for YACC parser ****************/
typedef struct
{
   int pos;                            /* input char position */
   char id[NAMELEN];                   /* identifier string */
   union
   {
      char WFAR *str;
      FIELD_NAME WFAR *field;
      PATH_ENTRY WFAR *path;
      COND_EXPR WFAR *where;
      REL_EXPR WFAR *rel;
      VAL_ENTRY WFAR *val;
      EXPR_LIST WFAR *xlist;
      EXPR_PTR expr;
   }  pt;
}  STRTOK;

/* number type token for parser */
typedef struct
{
   int pos;                            /* input char position */
   int num;
   long lnum;
   double dnum;
}  NUMTOK;

/* null pointers */
#define NUL_BREAK    ((BREAK_DESCR WFAR *)0)
#define NUL_CALC     ((CALC_LIST WFAR *)0)
#define NUL_CAT      ((CAT_LIST WFAR *)0)
#define NUL_CHAR     ((char WFAR *)0)
#define NUL_CLIST    ((COL_LIST WFAR *)0)
#define NUL_COND     ((COND_EXPR WFAR *)0)
#define NUL_DBALIST  ((DBA_LIST WFAR *)0)
#define NUL_EXPR     ((EXPR_PTR WFAR *)0)
#define NUL_INFO     ((FIELD_INFO WFAR *)0)
#define NUL_FORMAT   ((FORMAT WFAR *)0)
#define NUL_HELP     ((HELP_TEXT WFAR *)0)
#define NUL_KEYCK    ((KEY_CHECK WFAR *)0)
#define NUL_LEAF     ((EXPR_LEAF WFAR *)0)
#define NUL_MACRO    ((MACRO WFAR *)0)
#define NUL_FIELD    ((FIELD_NAME WFAR *)0)
#define NUL_NODE     ((EXPR_NODE WFAR *)0)
#define NUL_OCCUR    ((OCCUR_PTR WFAR *)0)
#define NUL_PAIR     ((VAL_PAIR WFAR *)0)
#define NUL_PATH     ((PATH_ENTRY WFAR *)0)
#define NUL_REL      ((REL_EXPR WFAR *)0)
#define NUL_RELATION ((RELATION WFAR *)0)
#define NUL_TRANS    ((TRANS_TYPE WFAR *)0)
#define NUL_VAL      ((VAL_ENTRY WFAR *)0)
#define NUL_XLIST    ((EXPR_LIST WFAR *)0)

#ifndef _QTASK_C_

#ifdef ANSI

/* functions in ACTIONS.c */
void QACT qactinit(void);
void QACT qact000(void);
int  QACT qact010(char WFAR * relname);
int  QACT qact020(struct expr_ptr WFAR * expr, int coldist, char WFAR * hdg);
int  QACT qact030(struct expr_ptr WFAR * expr);
void QACT qact031(struct cond_expr WFAR * cond);
int  QACT qact034(struct expr_ptr WFAR * left, struct expr_ptr WFAR * right,
                  struct expr_ptr WFAR * new);
int  QACT qact037(struct expr_ptr WFAR * prim);
int  QACT qact040(int opr, struct expr_ptr WFAR * left,
                  struct expr_ptr WFAR * right, struct expr_ptr WFAR * new);
int  QACT qact050(struct val_entry WFAR * val, struct expr_ptr WFAR * new);
int  QACT qact055(struct expr_leaf WFAR * leaf, char add_op);
int  QACT qact060(char WFAR * qual, char WFAR * name,
                  struct expr_ptr WFAR * new);
void QACT qact070(void);
int  QACT qact080(int n);
int  QACT qact090(void);
int  QACT qact100(char WFAR * string);
int  QACT qact105(void);
int  QACT qact110(char WFAR * func, int distinct,
                  struct expr_ptr WFAR * expr, struct expr_ptr WFAR * new);
int  QACT qact120(struct cond_expr WFAR * cond, struct expr_ptr WFAR * expr1,
                  struct expr_ptr WFAR * expr2, struct expr_ptr WFAR * new);
int  QACT qact130(struct expr_ptr WFAR * expr, int start, int length,
                  struct expr_ptr WFAR * new);
int  QACT qact140(char WFAR * recname, struct expr_ptr WFAR * new);
int  QACT qact143(struct expr_ptr WFAR * expr, struct expr_list WFAR * xlist,
                  struct expr_ptr WFAR * new);
void QACT qact145(void);
int  QACT qact146(int type, char WFAR * id, struct expr_ptr WFAR * new);
int  QACT qact148(struct expr_ptr WFAR * expr, struct expr_list WFAR * xold,
                  struct expr_list WFAR * WFAR * xnew);
int  QACT qact150(int opr, struct cond_expr WFAR * left,
                  struct cond_expr WFAR * right,
                  struct cond_expr WFAR * WFAR * where);
int  QACT qact160(struct rel_expr WFAR * rel,
                  struct cond_expr WFAR * WFAR * where);
int  QACT qact170(int not, int opr, struct expr_ptr WFAR * expr1,
                  struct expr_ptr WFAR * expr2,
                  struct rel_expr WFAR * WFAR * rel);
int  QACT qact180(struct expr_ptr WFAR * expr, int not,
                  struct val_entry WFAR * val,
                  struct rel_expr WFAR * WFAR * rel);
int  QACT qact190(struct expr_ptr WFAR * expr, int not,
                  struct val_entry WFAR * val,
                  struct rel_expr WFAR * WFAR * rel);
int  QACT qact200(int not, struct expr_ptr WFAR * expr, char WFAR * string,
                  struct rel_expr WFAR * WFAR * rel);
void QACT qact210(struct val_entry WFAR * valin,
                  struct val_entry WFAR * WFAR * valout);
int  QACT qact220(struct val_entry WFAR * valin,
                  struct val_entry WFAR * WFAR * valout);
int  QACT qact230(int col, int type);
int  QACT qact240(char WFAR * colname, int type);
int  QACT qact250(int col);
int  QACT qact260(char WFAR * colname);
int  QACT qact270(char WFAR * func, char WFAR * label);
void QACT qact275(void);
void QACT qact280(void);
int  QACT qact290(int col);
int  QACT qact300(char WFAR * colname, int WFAR * num);
void QACT qact310(int act, int num);
int  QACT qact320(int pos);
int  QACT qact330(char WFAR * name, int pos, int type);
void QACT qact340(void);
int  QACT qact350(int num);
void QACT qact360(char WFAR * title);
int  QACT qact370(struct path_entry WFAR * path);
int  QACT qact375(struct path_entry WFAR * leg, struct path_entry WFAR * seg,
                  struct path_entry WFAR * fork);
int  QACT qact380(struct path_entry WFAR * left,
                  struct path_entry WFAR * right);
int  QACT qact390(struct path_entry WFAR * opath, int op, char WFAR * id,
                  int pos, char WFAR * qual,
                  struct path_entry WFAR * WFAR * new);
int  QACT qact400(char WFAR * id, struct path_entry WFAR * WFAR * new);
int  QACT qact410(char WFAR * id);
int  QACT qact420(char WFAR * id);
int  QACT qact430(char WFAR * name);
int  QACT qact440(char WFAR * prompt);
int  QACT qact450(int base, int num, char WFAR * string, int pos);
int  QACT qact460(int pos);
int  QACT qact470(int type, char WFAR * name, int pos);
void QACT qact480(void);
int  QACT qact490(struct field_name WFAR * field, int link, char WFAR * desc);
int  QACT qact500(char WFAR * qual, char WFAR * id,
                  struct expr_ptr WFAR * expr,
                  struct field_name WFAR * WFAR * field);
void QACT qact510(void);
int  QACT qact520(struct val_entry WFAR * val);
int  QACT qact530(void);
int  QACT qact540(struct val_entry WFAR * val);
int  QACT qact550(struct val_entry WFAR * val, char WFAR * string);
int  QACT qact560(void);
void QACT qact570(char fpos);
void QACT qact580(char fpos, char ftype);
void QACT qact590(int nform, int wid);
int  QACT qact600(char WFAR * form, int wid, int pos);
int  QACT qact610(int wid, int dec);
int  QACT qact615(int wid, char WFAR * fstr);
int  QACT qact620(char WFAR * qual, char WFAR * id, int pos, int WFAR * fld);
int  QACT qact630(char WFAR * name, int pos);
int  QACT qact640(char WFAR * arg);

/* functions in DATECNV.c */
int  QINTERNAL_FIXED qdatefmt(char WFAR * fmtstr);
int  QINTERNAL_FIXED qsetdate(int yr, int fmtno, char WFAR * fmtstr);
long QINTERNAL_FIXED qcnvdate(int mm, int dd, int yy);
struct tm WFAR * QINTERNAL_FIXED jtod(long jul);
int  QINTERNAL_FIXED qidate(char WFAR * s, long WFAR * n);
void QINTERNAL_FIXED qcdate(long jul, int WFAR * mon, int WFAR * day,
                            int WFAR * yr, int WFAR * dow);
void QINTERNAL_FIXED qodate(long jul, char WFAR * str, int fmt, int width);
void QINTERNAL_FIXED qshowdate(void);

/* functions in EXECUTE.c */
void QINTERNAL_FIXED qfree_locks(int last_db);
void QINTERNAL_FIXED qplace_locks(void);
void QINTERNAL_FIXED qchklocks(void);
int  QINTERNAL_FIXED qfetch(void);
void QINTERNAL_FIXED qscaninit(int fld, int order);
int  QINTERNAL_FIXED qkeychk(int fld);
int  QINTERNAL_FIXED qgetfields(char DB_FAR *recbuf, char WFAR * occur);
int  QINTERNAL_FIXED qreset(void);
int  QINTERNAL_FIXED qdi_keyfrst(int fld);
int  QINTERNAL_FIXED qdi_keynext(int fld);

/* functions in EXPREVAL.c */
void QINTERNAL_FIXED promote( VAL_ENTRY WFAR *xval, VAL_TYPE WFAR *xt,
                              int WFAR *xvt);
int  QINTERNAL_FIXED qevaluate(void);
int  QINTERNAL_FIXED qinitbrk(void);
int  QINTERNAL_FIXED qcheckbrk(void);
int  QINTERNAL_FIXED qevalbrk(void);
int  QINTERNAL_FIXED qlastbrk(void);
int  QINTERNAL_FIXED qresetbrk(void);
int  QINTERNAL_FIXED qcondeval(struct cond_expr WFAR * wp);
int  QINTERNAL_FIXED valcmp(struct val_entry WFAR * lval,
                            struct val_entry WFAR * rval);
int  QINTERNAL_FIXED qvstrcpy(struct val_entry WFAR * val, char DB_FAR *str);
int  QINTERNAL_FIXED valcpy(struct val_entry WFAR * v1,
                            struct val_entry WFAR * v2);

/* functions in EXPRFMT.c */
int  QINTERNAL_FIXED qexpfmt(struct expr_ptr WFAR * expr, FORMAT WFAR * fmt);
int  QINTERNAL_FIXED qexptype(struct expr_ptr WFAR * expr, char WFAR * type,
                              int WFAR * len);
int  QINTERNAL_FIXED qnumfmt(char WFAR * fstr, char WFAR * f_num,
                             int WFAR * f_dec);
void QINTERNAL_FIXED qfldfmt(struct FIELD_ENTRY_S DB_FAR *fp,
                             FORMAT WFAR * fmt);
void QINTERNAL_FIXED qvalfmt(struct val_entry WFAR * val, FORMAT WFAR * fmt);

/* functions in FORMAT.c */
char WFAR *QINTERNAL_FIXED nstrchr(CONST char WFAR * s, int i);
int  QINTERNAL_FIXED qfmthdr(void);
void QINTERNAL_FIXED qfmtrow(void);
void QINTERNAL_FIXED qfmtbrk(void);
void QINTERNAL_FIXED qfmtval(char WFAR * txt, FORMAT WFAR * fmt,
                             struct val_entry WFAR * val);

/* functions in FREEMEM.c */
int  QINTERNAL_FIXED qfree(void);
int  QINTERNAL_FIXED qpathfree(struct path_entry WFAR * path);
int  QINTERNAL_FIXED qselfree(void);
int  QINTERNAL_FIXED qxprfree(struct expr_ptr WFAR * expr);
int  QINTERNAL_FIXED qvstrfree(struct val_entry WFAR * val);
void QINTERNAL_FIXED qFreeTrans(TRANS_TYPE WFAR * trans);

/* functions in LEXANAL.c */
int  QINTERNAL_FIXED qparse(char DB_FAR *str, int DB_FAR *ep,
                            char DB_FAR *DB_FAR *em);
char WFAR *QINTERNAL_FIXED qgetexpr(void);
int  QINTERNAL_FIXED qqlex(void);
void QINTERNAL_FIXED qqerror(char WFAR * s);

/* functions in LOOKUP.c */
int  QINTERNAL_FIXED qlookup(char WFAR * id);
char QINTERNAL_FIXED qfindfcn(char WFAR * fname, char WFAR * code);

/* functions in MEMORY.c */
char WFAR *QINTERNAL_FIXED q_alloc(size_t b);
char WFAR *QINTERNAL_FIXED q_calloc(size_t i, size_t s);
void QINTERNAL_FIXED q_free(void WFAR * p);
void QINTERNAL_FIXED q_chkfree(void);
DB_VOID WFAR *QINTERNAL_FIXED qNewObject(size_t size);

/* functions in OBJNAMES.c */
int  QINTERNAL_FIXED q_objnames(void);

/* functions in OUTPUT.c */
void QINTERNAL_FIXED qoutinit(void);
void QINTERNAL_FIXED qoutfini(void);
void QINTERNAL_FIXED qformfree(void);
int  QINTERNAL_FIXED qoutput(char ctl);
void QINTERNAL_FIXED qoutclose(void);
int  QINTERNAL_FIXED qforminit(char WFAR * fn, char WFAR * ext);
int  QINTERNAL_FIXED qformdet(void);
int  QINTERNAL_FIXED qformsum(void);
void QINTERNAL_FIXED qStringOut(char WFAR * s, unsigned nl);

/* functions in PARSER.c */
int  QINTERNAL_FIXED qqparse( zVIEW vSubtask );

/* functions in QSETDB.c */
int  QINTERNAL_FIXED qsetdb(int dbn);

/* functions in QSTRING.c */
int  _qprintf(CONST char WFAR * fmt,...);

int  qfprintf(CONST char WFAR * fmt,...);

/* functions in Q_CTOD.c */
void DB_FAR PASCAL DB_LOADDS q_ctod(char DB_FAR *cp, double DB_FAR *dp);

/* functions in Q_FUNCTN.c */
int  DB_FAR PASCAL DB_LOADDS q_function(char  WFAR *name,int count, VALUE WFAR *vals,
                      VALUE WFAR *res);

/* functions in Q_TRANS.c */
void  DB_FAR PASCAL DB_LOADDS q_translate(char  WFAR *tr_type,long  field,char  WFAR *code,
                        char  WFAR *trans);

/* functions in REFCNV.c */
int  QINTERNAL_FIXED qrecref(int rti);
int  QINTERNAL_FIXED qrecndx(int rno);
int  QINTERNAL_FIXED qsetref(int sti);
int  QINTERNAL_FIXED qsetndx(int sno);
long QINTERNAL_FIXED qfldref(int ndx);
int  QINTERNAL_FIXED qfldndx(long fno);

/* functions in SORT.c */
int  QINTERNAL_FIXED qinitgrp(void);
int  QINTERNAL_FIXED qinitsrt(void);
int  QINTERNAL_FIXED qentergrp(void);
int  QINTERNAL_FIXED qnextgrp(char WFAR * key);
int  QINTERNAL_FIXED qentersrt(void);
int  QINTERNAL_FIXED qnextsrt(void);

/* functions in TABLEMGT.c */
struct macro WFAR *QINTERNAL_FIXED qfindmac(char WFAR * name);

int  QINTERNAL_FIXED q_defmacro(char WFAR * term, char WFAR * defn,
                                char WFAR * desc);
int  QINTERNAL_FIXED q_getmacro(char WFAR * term, char WFAR * WFAR * defn,
                                char WFAR * WFAR * desc);

int  QINTERNAL_FIXED qmacdef(char WFAR * term, char WFAR * defn,
                             char WFAR * desc);
int  QINTERNAL_FIXED qfindfld(char WFAR * name, char WFAR * qual,
                              struct field_name WFAR * WFAR * fnp,
                              struct occur_ptr WFAR * WFAR * fop);
int  QINTERNAL_FIXED qaddfld(char WFAR * name, char WFAR * qual,
                             struct field_name WFAR * WFAR * fnp);
struct relation WFAR *QINTERNAL_FIXED qfindrel(char WFAR * name);
int  QINTERNAL_FIXED qaddrel(char WFAR * name, char WFAR * desc,
                             struct path_entry WFAR * path);
int  QINTERNAL_FIXED qrelchk(char WFAR * relname);
int  QINTERNAL_FIXED qdbachk(void);
int  QINTERNAL_FIXED qallrel(char WFAR * relname);

/* functions in TXTFCNS.c */
int  q_patmatch(char WFAR * str, char WFAR * pat);
char WFAR *QINTERNAL_FIXED padtext(char WFAR * string, char ch, int len);
char WFAR *QINTERNAL_FIXED ctrtext(char WFAR * dest, char WFAR * source,
                                   int length);
char WFAR *QINTERNAL_FIXED rjtext(char WFAR * dest, char WFAR * source,
                                  int length);
char WFAR *QINTERNAL_FIXED ljtext(char WFAR * dest, char WFAR * source,
                                  int length);
char WFAR *QINTERNAL_FIXED mrgtext(char WFAR * dest, char WFAR * source);
char WFAR *QINTERNAL_FIXED upshift(char WFAR * str);
char WFAR *QINTERNAL_FIXED downshift(char WFAR * str);
char WFAR *QINTERNAL_FIXED exptabs(char WFAR * s);


/* functions in USERINFO.c */
void QINTERNAL_FIXED qshowpar(int what);
void QINTERNAL_FIXED qshowfld(char WFAR * fldname, char WFAR * qual);
void QINTERNAL_FIXED pr_field(struct field_name WFAR * fnp);
void QINTERNAL_FIXED qshowrel(char WFAR * relname);
void QINTERNAL_FIXED qshowmac(char WFAR * macname);
struct help_text WFAR *QINTERNAL_FIXED qhelp(char WFAR * term);
void QINTERNAL_FIXED qlisthelp(char WFAR * term);

int  QINTERNAL_FIXED v_fclose(FILE * stream);
char _far *QINTERNAL_FIXED v_fgets(char _far * s, int len, FILE * fd);
FILE *QINTERNAL_FIXED v_fopen(char _far * path, char _far * type);
int  QINTERNAL_FIXED v_fprintf(FILE * fp, char _far * fmt);
int QINTERNAL_FIXED v_unlink(char _far *);
long QINTERNAL_FIXED v_fseek(FILE *stream, long offset, int origin);
long QINTERNAL_FIXED v_ftell(FILE *stream);


#else                                  /* ANSI */

/* functions in ACTIONS.c */
void QACT qactinit();
void QACT qact000();
int  QACT qact010();
int  QACT qact020();
int  QACT qact030();
void QACT qact031();
int  QACT qact034();
int  QACT qact037();
int  QACT qact040();
int  QACT qact050();
int  QACT qact055();
int  QACT qact060();
void QACT qact070();
int  QACT qact080();
int  QACT qact090();
int  QACT qact100();
int  QACT qact105();
int  QACT qact110();
int  QACT qact120();
int  QACT qact130();
int  QACT qact140();
int  QACT qact143();
void QACT qact145();
int  QACT qact146();
int  QACT qact148();
int  QACT qact150();
int  QACT qact160();
int  QACT qact170();
int  QACT qact180();
int  QACT qact190();
int  QACT qact200();
void QACT qact210();
int  QACT qact220();
int  QACT qact230();
int  QACT qact240();
int  QACT qact250();
int  QACT qact260();
int  QACT qact270();
void QACT qact275();
void QACT qact280();
int  QACT qact290();
int  QACT qact300();
void QACT qact310();
int  QACT qact320();
int  QACT qact330();
void QACT qact340();
int  QACT qact350();
void QACT qact360();
int  QACT qact370();
int  QACT qact375();
int  QACT qact380();
int  QACT qact390();
int  QACT qact400();
int  QACT qact410();
int  QACT qact420();
int  QACT qact430();
int  QACT qact440();
int  QACT qact450();
int  QACT qact460();
int  QACT qact470();
void QACT qact480();
int  QACT qact490();
int  QACT qact500();
void QACT qact510();
int  QACT qact520();
int  QACT qact530();
int  QACT qact540();
int  QACT qact550();
int  QACT qact560();
void QACT qact570();
void QACT qact580();
void QACT qact590();
int  QACT qact600();
int  QACT qact610();
int  QACT qact615();
int  QACT qact620();
int  QACT qact630();
int  QACT qact640();

/* functions in DATECNV.c */
int  QINTERNAL_FIXED qdatefmt();
int  QINTERNAL_FIXED qsetdate();
long QINTERNAL_FIXED qcnvdate();
struct tm WFAR * QINTERNAL_FIXED jtod();
int  QINTERNAL_FIXED qidate();
void QINTERNAL_FIXED qcdate();
void QINTERNAL_FIXED qodate();
void QINTERNAL_FIXED qshowdate();

/* functions in EXECUTE.c */
void QINTERNAL_FIXED qfree_locks();
void QINTERNAL_FIXED qplace_locks();
void QINTERNAL_FIXED qchklocks();
int  QINTERNAL_FIXED qfetch();
void QINTERNAL_FIXED qscaninit();
int  QINTERNAL_FIXED qkeychk();
int  QINTERNAL_FIXED qgetfields();
int  QINTERNAL_FIXED qreset();
int  QINTERNAL_FIXED qdi_keyfrst();
int  QINTERNAL_FIXED qdi_keynext();

/* functions in EXPREVAL.c */
void QINTERNAL_FIXED promote();
int  QINTERNAL_FIXED qevaluate();
int  QINTERNAL_FIXED qinitbrk();
int  QINTERNAL_FIXED qcheckbrk();
int  QINTERNAL_FIXED qevalbrk();
int  QINTERNAL_FIXED qlastbrk();
int  QINTERNAL_FIXED qresetbrk();
int  QINTERNAL_FIXED qcondeval();
int  QINTERNAL_FIXED valcmp();
int  QINTERNAL_FIXED qvstrcpy();
int  QINTERNAL_FIXED valcpy();

/* functions in EXPRFMT.c */
int  QINTERNAL_FIXED qexpfmt();
int  QINTERNAL_FIXED qexptype();
int  QINTERNAL_FIXED qnumfmt();
void QINTERNAL_FIXED qfldfmt();
void QINTERNAL_FIXED qvalfmt();

/* functions in FORMAT.c */
char WFAR *QINTERNAL_FIXED nstrchr();
int  QINTERNAL_FIXED qfmthdr();
void QINTERNAL_FIXED qfmtrow();
void QINTERNAL_FIXED qfmtbrk();
void QINTERNAL_FIXED qfmtval();

/* functions in FREEMEM.c */
int  QINTERNAL_FIXED qfree();
int  QINTERNAL_FIXED qpathfree();
int  QINTERNAL_FIXED qselfree();
int  QINTERNAL_FIXED qxprfree();
int  QINTERNAL_FIXED qvstrfree();
void QINTERNAL_FIXED qFreeTrans();

/* functions in LEXANAL.c */
int  QINTERNAL_FIXED qparse();
char WFAR *QINTERNAL_FIXED qgetexpr();
int  QINTERNAL_FIXED qqlex();
void QINTERNAL_FIXED qqerror();

/* functions in LOOKUP.c */
int  QINTERNAL_FIXED qlookup();
char QINTERNAL_FIXED qfindfcn();

/* functions in MEMORY.c */
char WFAR *QINTERNAL_FIXED q_alloc();
char WFAR *QINTERNAL_FIXED q_calloc();
void QINTERNAL_FIXED q_free();
void QINTERNAL_FIXED q_chkfree();
DB_VOID WFAR *QINTERNAL_FIXED qNewObject();

/* functions in OBJNAMES.c */
int  QINTERNAL_FIXED q_objnames();

/* functions in OUTPUT.c */
void QINTERNAL_FIXED qoutinit();
void QINTERNAL_FIXED qoutfini();
void QINTERNAL_FIXED qformfree();
int  QINTERNAL_FIXED qoutput();
void QINTERNAL_FIXED qoutclose();
int  QINTERNAL_FIXED qforminit();
int  QINTERNAL_FIXED qformdet();
int  QINTERNAL_FIXED qformsum();
void QINTERNAL_FIXED qStringOut();

/* functions in PARSER.c */
int  QINTERNAL_FIXED qqparse( zVIEW vSubtask );

/* functions in QSETDB.c */
int  QINTERNAL_FIXED qsetdb();

/* functions in QSTRING.c */

int  qfprintf();

/* functions in Q_CTOD.c */
void DB_FAR PASCAL DB_LOADDS q_ctod();

/* functions in Q_FUNCTN.c */
int  DB_FAR PASCAL DB_LOADDS q_function();

/* functions in Q_TRANS.c */
void  DB_FAR PASCAL DB_LOADDS q_translate();

/* functions in REFCNV.c */
int  QINTERNAL_FIXED qrecref();
int  QINTERNAL_FIXED qrecndx();
int  QINTERNAL_FIXED qsetref();
int  QINTERNAL_FIXED qsetndx();
long QINTERNAL_FIXED qfldref();
int  QINTERNAL_FIXED qfldndx();

/* functions in SORT.c */
int  QINTERNAL_FIXED qinitgrp();
int  QINTERNAL_FIXED qinitsrt();
int  QINTERNAL_FIXED qentergrp();
int  QINTERNAL_FIXED qnextgrp();
int  QINTERNAL_FIXED qentersrt();
int  QINTERNAL_FIXED qnextsrt();

/* functions in TABLEMGT.c */
struct macro WFAR *QINTERNAL_FIXED qfindmac();

int  QINTERNAL_FIXED q_defmacro();
int  QINTERNAL_FIXED q_getmacro();

int  QINTERNAL_FIXED qmacdef();
int  QINTERNAL_FIXED qfindfld();
int  QINTERNAL_FIXED qaddfld();
struct relation WFAR *QINTERNAL_FIXED qfindrel();
int  QINTERNAL_FIXED qaddrel();
int  QINTERNAL_FIXED qrelchk();
int  QINTERNAL_FIXED qdbachk();
int  QINTERNAL_FIXED qallrel();

/* functions in TXTFCNS.c */
int  q_patmatch();
char WFAR *QINTERNAL_FIXED padtext();
char WFAR *QINTERNAL_FIXED ctrtext();
char WFAR *QINTERNAL_FIXED rjtext();
char WFAR *QINTERNAL_FIXED ljtext();
char WFAR *QINTERNAL_FIXED mrgtext();
char WFAR *QINTERNAL_FIXED upshift();
char WFAR *QINTERNAL_FIXED downshift();
char WFAR *QINTERNAL_FIXED exptabs();


/* functions in USERINFO.c */
void QINTERNAL_FIXED qshowpar();
void QINTERNAL_FIXED qshowfld();
void QINTERNAL_FIXED pr_field();
void QINTERNAL_FIXED qshowrel();
void QINTERNAL_FIXED qshowmac();
struct help_text WFAR *QINTERNAL_FIXED qhelp();
void QINTERNAL_FIXED qlisthelp();

#endif                                 /* ANSI */

#define qbStrDup     qStrDup
#define qGetFarAddr(x) (char WFAR *)(x)

char WFAR *QINTERNAL_FIXED qbStrDupFree(P1(char WFAR * cp));

char WFAR *QINTERNAL_FIXED qStrDup(P1( CONST char WFAR * cp ));

#else                   /* _QTASK_C_ */


#endif                  /* _QTASK_C_ */

/* ************************* GLOBAL VARIABLES ************************** */

struct qTask_s {
   QCTODPROC         Qctod_proc;
   QDISPLAYPROC      Qdisplay_proc;
   QFUNCTIONPROC     Qfunction_proc;
   QPRINTERPROC      Qprinter_proc;
   QTRANSLATEPROC    Qtranslate_proc;
   BOOLEAN           Qecho;
   BOOLEAN           Quseoptkey;
   int               Qtabstop;
   int               Qmaxaliases;
   int               Qrepsize;
   int               Qtrace;
   int               Hashsize;
   int               Qlock_limit;
   int               Qyield_limit;
   char              Qdbpath[DB_PATHLEN];
   char              Qdfpath[DB_PATHLEN];
   char              Qrfpath[DB_PATHLEN];
   char              Qformfeed;
   char              Qwildone;
   char              Qwildall;
   unsigned short    Qoptions;

/* elements following Qoptions will be initilized in q_close() */
   FILE *                     Qinto;
   FILE *                     Qinto_def;
   FILE *                     Sql;
   VAL_ENTRY WFAR *           Qcurval;
   DBA_LIST WFAR *            Qdbalist;
   VAL_ENTRY WFAR *           Qlast_vals;
   VALUE WFAR *               Quserval;
   FIELD_NAME WFAR * WFAR *   Qfld_names;
   COLUMN_DESCR WFAR *        Qcol_list;
   FIELD_VALUE WFAR *WFAR *   Qfld_list;
   CALC_LIST WFAR *           Qcalc_list;
   KEY_CHECK WFAR *           Qkey_check;
   COND_EXPR WFAR *           Qwhere;
   COND_EXPR WFAR *           Qhaving;
   BREAK_DESCR WFAR *         Qcol_calcs;
   BREAK_DESCR WFAR * WFAR *  Qbrklist;
   BREAK_DESCR WFAR * WFAR *  Qcurbrk;
   OCCUR_PTR WFAR *           Qfldval;
   RELATION WFAR *            Qreltab;
   RELATION WFAR *            Qdreltab;
   RELATION WFAR *            Qrelend;
   PATH_ENTRY WFAR *          Qcurrel;
   MACRO WFAR *               Qmactab;
   MACRO WFAR *               Qmaclast;
   HELP_TEXT WFAR *           Qhelp_list;
   VAL_PAIR WFAR *            Cur_pair;
   BREAK_DESCR WFAR *         Curbreak;
   TRANS_TYPE WFAR *          Trn_type;
   FORMAT                     Field_format;
   FORMAT WFAR *              Qfmt;
   VAL_PAIR WFAR *            Frst_pair;
   COL_LIST WFAR *            Frstcol;
   VAL_ENTRY WFAR *           Frstval;
   CAT_LIST WFAR *            Lastcat;
   COL_LIST WFAR *            Lastcol;
   VAL_ENTRY WFAR *           Lastval;
   FIELD_ENTRY WFAR *         Keyck_fld;
   PATH_ENTRY WFAR *          Curpath;
   VAL_ENTRY WFAR *           Next_in_list;
   BUCKET WFAR * WFAR *       Ptrhash;
   CALC_EXPR WFAR *           Distcalc;
   HELP_TEXT WFAR *           Curr_help;
   QDB_LOCKS WFAR *           Qdb_locks;
   PATH_ENTRY WFAR * WFAR *   QopSetups;
   REP_LINE WFAR *            Repform;
   REP_LINE WFAR *            Formhdr;
   REP_LINE WFAR *            Formdet;
   REP_LINE WFAR *            Formftr;
   BRK_SECTN WFAR *           Frstbrk;
   BRK_SECTN WFAR *           Lastbrk;
   struct qsqlstack WFAR *    Qsqlstk;
   int WFAR *                 Qsort_list;
   int WFAR *                 Qgroup_list;
   struct qtext               QtA[MACNESTMAX];
   unsigned long Tot_alloc;
   unsigned long Qmemory;
   long Qsortf00;
   long Qgrupf00;
   long Qsortkey;
   long Qgrupkey;
   long Curcodeval;
   long Base_julian;
   long Tuple_pos;
   long Tuple_count;
   BOOLEAN Qusing;
   BOOLEAN Qcalcdist;
   BOOLEAN Qdistinct;
   BOOLEAN Qnotnull;
   BOOLEAN Qbreak;
   BOOLEAN Qsorting;
   BOOLEAN Qmultirecs;
   BOOLEAN Select_stmt;
   BOOLEAN Sel_list;
   BOOLEAN Firstrec;
   BOOLEAN Firstkey;
   BOOLEAN New_group;
   BOOLEAN End_of_fetch;
   BOOLEAN End_of_vals;
   BOOLEAN Last_row;
   BOOLEAN Firstgrp;
   BOOLEAN Firstsrt;
   BOOLEAN Store_sort;
   BOOLEAN Qfiles_locked;
   BOOLEAN QtraceLocked;
   BOOLEAN Quser_readonly;
   int Q_status;
   int Qwidth;
   int Qlength;
   int Qlock_count;
   int Qdev;
   int Qdev_def;
   int Qwid_def;
   int Qkeyopt;
   int Qlen_def;
   int Qgchklen;
   int Qhdrlines;
   int Qerrpos;
   int Qbldexpr;
   int Qarg_count;
   int Qcurr_db;
   int Qsize_rt;
   int Qsize_st;
   int Qnextfld;
   int Qsize_fd;
   int Qcol_count;
   int Qcol_max;
   int Qfld_count;
   int Qfld_max;
   int Qsort_count;
   int Qgrouping;
   int Qgroup_count;
   int Qsortrec;
   int Qgruprec;
   int Qsrecno;
   int Qgrecno;
   int Qsf00fno;
   int Qgf00fno;
   int Qskeyfno;
   int Qgkeyfno;
   int Nextsub;
   int Rec_count;
   int Rec_list[MAXOR];
   int Subs[MAXDIMS];
   int Break_val;
   int Base_yr;
   int Def_format;
   int Havingcmp;
   int Skeylen;
   int Gkeylen;
   int Pfxlen;
   int QlI;
   int Xpos;
   int Pcnt;
   int Qlevel;
   int Line;
   int Qpage;
   int Formlen;
   int Hdrlines;
   int Detlines;
   int Sumlines;
   int Ftrlines;
   int Qnlinetype;
   int Qnprevbreak;
   int Qnprevtype;
   int Qnlinecount;

   int         QgrpSrtDbaDbn;
   DB_ADDR     QgrpSrtDbaddr;
   size_t      QsubStrBufSize;
   char WFAR * QsubStrBuf;

   int Qyield_count;
   /* from qfar.c */
   char WFAR * QpszOutEnd;
   char        QpszOut[1024];

   char WFAR *          Qgbuf;
   char WFAR * WFAR *   Qpagehdr;
   char WFAR *          Qpagehdrp;
   char WFAR *          Qproc_args[MAXARGS];
   char WFAR * WFAR *   Qrec_names;
   char WFAR * WFAR *   Qset_names;
   char WFAR *          Qsbuf;
   char WFAR *          Qrbuf;
   char WFAR *          Oldkey;
   char WFAR *          Newkey;
   char Qcolsep;
   char Qrowsep;
   char Qprompt[PRLEN + 1];
   char Qprinter[LPLEN + 1];
   char Qdate_hdr[30];                       /* magic number */
   char Qoutfile[FILENMLEN + 1];
   char Qsysdate[30];                        /* magic number */
   char Qtitle_hdr[MAXSTRING + 1];
   char Qdatesep;
   char Qbrktxt[MAXSTRING + 1];
   char Qhdrtxt[MAXSTRING + 1];
   char Qrowtxt[MAXSTRING + 1];
   char Qerrinfo[MAXERRTXT + 1];
   char Qcurncy[9];                          /* magic number */
   char Qdecimal;
   char Qnumsep;
   char Scanorder;
   char Scandir;
   char Chkey[MAXKEYSIZE];
   char Qpagtxt[MAXSTRING + 1];
   char Exprstr[MAXTRANS + 1];
   char Strbuf[MAXSTRING + 1];
   char Usermsg[MAXERRMSGTXT + 1];
};                                     /* end of struct qTask_s */

typedef struct qTask_s QTASK;
#define QTASKSIZE sizeof(QTASK)

extern QTASK qGlobals;                 /* declared in execute.c */


extern char WFAR *   Qerrmsg[];        /* declared in lexanal.c  */
extern unsigned      Qerrmsg_size;

#define  q_status       qGlobals.Q_status

#define  qinto          qGlobals.Qinto
#define  qinto_def      qGlobals.Qinto_def
#define  sql            qGlobals.Sql
#define  qcurval        qGlobals.Qcurval
#define  qdbalist       qGlobals.Qdbalist
#define  qlast_vals     qGlobals.Qlast_vals
#define  quserval       qGlobals.Quserval
#define  qfld_names     qGlobals.Qfld_names
#define  qcol_list      qGlobals.Qcol_list
#define  qfld_list      qGlobals.Qfld_list
#define  qcalc_list     qGlobals.Qcalc_list
#define  qkey_check     qGlobals.Qkey_check
#define  qwhere         qGlobals.Qwhere
#define  qhaving        qGlobals.Qhaving
#define  qcol_calcs     qGlobals.Qcol_calcs
#define  qbrklist       qGlobals.Qbrklist
#define  qcurbrk        qGlobals.Qcurbrk
#define  qfldval        qGlobals.Qfldval
#define  qreltab        qGlobals.Qreltab
#define  qdreltab       qGlobals.Qdreltab
#define  qrelend        qGlobals.Qrelend
#define  qcurrel        qGlobals.Qcurrel
#define  qmactab        qGlobals.Qmactab
#define  qmaclast       qGlobals.Qmaclast
#define  qhelp_list     qGlobals.Qhelp_list
#define  qctod_proc     qGlobals.Qctod_proc
#define  qdisplay_proc     qGlobals.Qdisplay_proc
#define  qfunction_proc    qGlobals.Qfunction_proc
#define  qprinter_proc     qGlobals.Qprinter_proc
#define  qtranslate_proc   qGlobals.Qtranslate_proc
#define  cur_pair       qGlobals.Cur_pair
#define  curbreak       qGlobals.Curbreak
#define  trn_type       qGlobals.Trn_type
#define  field_format   qGlobals.Field_format
#define  qfmt           qGlobals.Qfmt
#define  frst_pair      qGlobals.Frst_pair
#define  frstcol        qGlobals.Frstcol
#define  frstval        qGlobals.Frstval
#define  lastcat        qGlobals.Lastcat
#define  lastcol        qGlobals.Lastcol
#define  lastval        qGlobals.Lastval
#define  keyck_fld      qGlobals.Keyck_fld
#define  curpath        qGlobals.Curpath
#define  next_in_list   qGlobals.Next_in_list
#define  ptrhash        qGlobals.Ptrhash
#define  distcalc       qGlobals.Distcalc
#define  curr_help      qGlobals.Curr_help
#define  qdb_locks      qGlobals.Qdb_locks
#define  qtraceLocked   qGlobals.QtraceLocked
#define  quser_readonly    qGlobals.Quser_readonly
#define  lock_count     qGlobals.Qlock_count
#define  files_locked   qGlobals.Qfiles_locked
#define  qopSetups      qGlobals.QopSetups
#define  qsqlstk        qGlobals.Qsqlstk
#define  qtA            qGlobals.QtA
#define  qmemory        qGlobals.Qmemory
#define  qsortf00       qGlobals.Qsortf00
#define  qgrupf00       qGlobals.Qgrupf00
#define  curcodeval     qGlobals.Curcodeval
#define  base_julian    qGlobals.Base_julian
#define  tot_alloc      qGlobals.Tot_alloc
#define  qcalcdist      qGlobals.Qcalcdist
#define  qdistinct      qGlobals.Qdistinct
#define  qnotnull       qGlobals.Qnotnull
#define  quseoptkey     qGlobals.Quseoptkey
#define  qbreak         qGlobals.Qbreak
#define  qecho          qGlobals.Qecho
#define  qsorting       qGlobals.Qsorting
#define  qmultirecs     qGlobals.Qmultirecs
#define  select_stmt    qGlobals.Select_stmt
#define  sel_list       qGlobals.Sel_list
#define  firstrec       qGlobals.Firstrec
#define  firstkey       qGlobals.Firstkey
#define  new_group      qGlobals.New_group
#define  end_of_fetch   qGlobals.End_of_fetch
#define  end_of_vals    qGlobals.End_of_vals
#define  last_row       qGlobals.Last_row
#define  firstgrp       qGlobals.Firstgrp
#define  firstsrt       qGlobals.Firstsrt
#define  store_sort     qGlobals.Store_sort
#define  qdev           qGlobals.Qdev
#define  qdev_def       qGlobals.Qdev_def
#define  qwid_def       qGlobals.Qwid_def
#define  qwidth         qGlobals.Qwidth
#define  qkeyopt        qGlobals.Qkeyopt
#define  qlen_def       qGlobals.Qlen_def
#define  qlength        qGlobals.Qlength
#define  qgchklen       qGlobals.Qgchklen
#define  qlock_limit    qGlobals.Qlock_limit
#define  qhdrlines      qGlobals.Qhdrlines
#define  qerrpos        qGlobals.Qerrpos
#define  qbldexpr       qGlobals.Qbldexpr
#define  qarg_count     qGlobals.Qarg_count
#define  qtrace         qGlobals.Qtrace
#define  qcurr_db       qGlobals.Qcurr_db
#define  qsize_rt       qGlobals.Qsize_rt
#define  qsize_st       qGlobals.Qsize_st
#define  qmaxaliases    qGlobals.Qmaxaliases
#define  qnextfld       qGlobals.Qnextfld
#define  qsize_fd       qGlobals.Qsize_fd
#define  qcol_count     qGlobals.Qcol_count
#define  qcol_max       qGlobals.Qcol_max
#define  qfld_count     qGlobals.Qfld_count
#define  qfld_max       qGlobals.Qfld_max
#define  qsort_list     qGlobals.Qsort_list
#define  qsort_count    qGlobals.Qsort_count
#define  qgrouping      qGlobals.Qgrouping
#define  qgroup_list    qGlobals.Qgroup_list
#define  qgroup_count   qGlobals.Qgroup_count
#define  qsortkey       qGlobals.Qsortkey
#define  qgrupkey       qGlobals.Qgrupkey
#define  qsortrec       qGlobals.Qsortrec
#define  qgruprec       qGlobals.Qgruprec
#define  qsrecno        qGlobals.Qsrecno
#define  qgrecno        qGlobals.Qgrecno
#define  qsf00fno       qGlobals.Qsf00fno
#define  qgf00fno       qGlobals.Qgf00fno
#define  qskeyfno       qGlobals.Qskeyfno
#define  qgkeyfno       qGlobals.Qgkeyfno
#define  nextsub        qGlobals.Nextsub
#define  rec_count      qGlobals.Rec_count
#define  rec_list       qGlobals.Rec_list
#define  subs           qGlobals.Subs
#define  break_val      qGlobals.Break_val
#define  base_yr        qGlobals.Base_yr
#define  def_format     qGlobals.Def_format
#define  havingcmp      qGlobals.Havingcmp
#define  skeylen        qGlobals.Skeylen
#define  gkeylen        qGlobals.Gkeylen
#define  pfxlen         qGlobals.Pfxlen
#define  qlI            qGlobals.QlI
#define  xpos           qGlobals.Xpos
#define  pcnt           qGlobals.Pcnt
#define  hashsize       qGlobals.Hashsize
#define  qtabstop       qGlobals.Qtabstop
#define  qlevel         qGlobals.Qlevel
#define  qoptions       qGlobals.Qoptions
#define  qprinter       qGlobals.Qprinter
#define  qdate_hdr      qGlobals.Qdate_hdr
#define  qoutfile       qGlobals.Qoutfile
#define  qprompt        qGlobals.Qprompt
#define  qsysdate       qGlobals.Qsysdate
#define  qtitle_hdr     qGlobals.Qtitle_hdr
#define  qdatesep       qGlobals.Qdatesep
#define  qgbuf          qGlobals.Qgbuf
#define  qbrktxt        qGlobals.Qbrktxt
#define  qhdrtxt        qGlobals.Qhdrtxt
#define  qrowtxt        qGlobals.Qrowtxt
#define  qpagehdr       qGlobals.Qpagehdr
#define  qpagehdrp      qGlobals.Qpagehdrp
#define  qproc_args     qGlobals.Qproc_args
#define  qrec_names     qGlobals.Qrec_names
#define  qset_names     qGlobals.Qset_names
#define  qsbuf          qGlobals.Qsbuf
#define  qerrinfo       qGlobals.Qerrinfo
#define  qdbpath        qGlobals.Qdbpath
#define  qdfpath        qGlobals.Qdfpath
#define  qrfpath        qGlobals.Qrfpath
#define  qcolsep        qGlobals.Qcolsep
#define  qrowsep        qGlobals.Qrowsep
#define  qcurncy        qGlobals.Qcurncy
#define  qdecimal       qGlobals.Qdecimal
#define  qnumsep        qGlobals.Qnumsep
#define  qrbuf          qGlobals.Qrbuf
#define  scanorder      qGlobals.Scanorder
#define  scandir        qGlobals.Scandir
#define  oldkey         qGlobals.Oldkey
#define  newkey         qGlobals.Newkey
#define  chkey          qGlobals.Chkey
#define  qpagtxt        qGlobals.Qpagtxt
#define  exprstr        qGlobals.Exprstr
#define  strbuf         qGlobals.Strbuf
#define  qwildone       qGlobals.Qwildone
#define  qwildall       qGlobals.Qwildall

#define  repform        qGlobals.Repform
#define  formhdr        qGlobals.Formhdr
#define  formdet        qGlobals.Formdet
#define  formftr        qGlobals.Formftr
#define  frstbrk        qGlobals.Frstbrk
#define  lastbrk        qGlobals.Lastbrk
#define  tuple_pos      qGlobals.Tuple_pos
#define  tuple_count    qGlobals.Tuple_count
#define  qusing         qGlobals.Qusing
#define  qrepsize       qGlobals.Qrepsize
#define  line           qGlobals.Line
#define  qpage          qGlobals.Qpage
#define  formlen        qGlobals.Formlen
#define  hdrlines       qGlobals.Hdrlines
#define  detlines       qGlobals.Detlines
#define  sumlines       qGlobals.Sumlines
#define  ftrlines       qGlobals.Ftrlines
#define  qformfeed      qGlobals.Qformfeed
#define  usermsg        qGlobals.Usermsg
#define  qnlinetype     qGlobals.Qnlinetype
#define  qnprevbreak    qGlobals.Qnprevbreak
#define  qnprevtype     qGlobals.Qnprevtype
#define  qnlinecount    qGlobals.Qnlinecount

#define  qpszOut        qGlobals.QpszOut
#define  qpszOutEnd     qGlobals.QpszOutEnd
#define  qyield_count   qGlobals.Qyield_count
#define  qyield_limit   qGlobals.Qyield_limit

#define qGrpSrtDbaDbn   qGlobals.QgrpSrtDbaDbn
#define qGrpSrtDbaddr   qGlobals.QgrpSrtDbaddr
#define qSubStrBufSize  qGlobals.QsubStrBufSize
#define qSubStrBuf      qGlobals.QsubStrBuf

/**************************************************************************/

#define qdi_keyfind(fld,fldval) d_keyfind(qfldref(fld),fldval,curr_db)

#ifdef ANSI
int EXTERNAL_DBN     d_initfile(FILE_NO, int);
int EXTERNAL_FIXED   d_keyread(DB_VOID DB_FAR *);
int EXTERNAL_DBN     d_lock(int, LOCK_REQUEST DB_FAR *, int);
int EXTERNAL_DBN     d_makenew(int, int);
int EXTERNAL_FIXED   d_open(CONST char DB_FAR *, CONST char DB_FAR *);
int EXTERNAL_DBN     d_recfree(int, int);
int EXTERNAL_DBN     d_reclock(int, char DB_FAR *, int);
int EXTERNAL_DBN     d_reclstat(int, char DB_FAR *, int);
int EXTERNAL_FIXED   d_renfile(CONST char DB_FAR *, FILE_NO,
                               CONST char DB_FAR *);
int EXTERNAL_FIXED   d_renclean(void);
int EXTERNAL_DBN     d_setfree(int, int);
int EXTERNAL_DBN     d_setlstat(int, char DB_FAR *, int);
int EXTERNAL_DBN     d_recfrst(int, int);
int EXTERNAL_DBN     d_recnext(int);
int EXTERNAL_DBN     d_keyfind(long, CONST DB_VOID DB_FAR *, int);
int EXTERNAL_FIXED   d_close(void);
int EXTERNAL_FIXED   d_dbuserid(CONST char DB_FAR *);
int EXTERNAL_DBN     d_utscr(DB_ULONG DB_FAR *, int);
int EXTERNAL_DBN     d_utscs(int, DB_ULONG DB_FAR *, int);
int EXTERNAL_DBN     d_crtype(int DB_FAR *, int);
int EXTERNAL_FIXED   d_trabort(void);
#else
int EXTERNAL_DBN     d_initfile();
int EXTERNAL_FIXED   d_keyread();
int EXTERNAL_DBN     d_lock();
int EXTERNAL_DBN     d_makenew();
int EXTERNAL_FIXED   d_open();
int EXTERNAL_DBN     d_recfree();
int EXTERNAL_DBN     d_reclock();
int EXTERNAL_DBN     d_reclstat();
int EXTERNAL_FIXED   d_renfile();
int EXTERNAL_FIXED   d_renclean();
int EXTERNAL_DBN     d_setfree();
int EXTERNAL_DBN     d_setlstat();
int EXTERNAL_DBN     d_recfrst();
int EXTERNAL_DBN     d_recnext();
int EXTERNAL_DBN     d_keyfind();
int EXTERNAL_FIXED   d_close();
int EXTERNAL_FIXED   d_dbuserid();
int EXTERNAL_DBN     d_utscr();
int EXTERNAL_DBN     d_utscs();
int EXTERNAL_DBN     d_crtype();
int EXTERNAL_FIXED   d_trabort();
#endif

#ifdef ANSI
   int  QINTERNAL_FIXED qtask_switch(DB_TASK DB_FAR * );
   void QINTERNAL_FIXED qTaskInit(QTASK DB_FAR *, int);
#else
   int  QINTERNAL_FIXED qtask_switch();
   void QINTERNAL_FIXED qTaskInit();
#endif

/* vpp @win.vpp QDEFNS.H (Wed Jun 10 16:29:30 1992)
 *
 *     -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS
 *     -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC
 *     -nARCHIVING -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES
 *     -dMULTI_TASK -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP
 *     -nSINGLE_USER -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4
 *     -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY -nTRUENAME -nLAT
 */

