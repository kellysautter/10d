/*----------------------------------------------------------------------------
   dbtype.h: Standard Raima Data Manager header file containing:

      - DDL dictionary type definitions
      - Machine dependent constant definitions
      - Miscellaneous constants used by runtime functions
      - Virtual memory page table type definitions

   (An #include "vista.h" must precede this include)

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef DBTYPE_H
#define DBTYPE_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  101 21-Jun-88 RSC Added rn_type and rn_dba to database table
  344 06-Jul-88 RSC LOGFILELEN also defined in dblock.h - add ifndef
  115 18-Jul-88 RSC Integrate VAX VMS changes
      21-Jul-88 RSC Fix bugaboo in VAX integration
  368 28-Jul-88 RSC Integrate BSD changes into code
  310 10-Aug-88 RSC Cleanup function prototype
      19-Aug-88 RSC Moved rn_type,rn_dba to separate table for compatibility
                    with db_QUERY and db_REVISE
  423 10-Sep-88 RSC Add dbpush names for netunix.c
  423 22-Sep-88 RSC Modify/simplify DB_ENTER macro for Lattice compiler
  368 23-Sep-88 RSC Change type of pid from INT to int and
                    corrected defn of MESSAGE
  423 26-Sep-88 RSC Moved INIT_PAGE_P defn to initial.c
      11-Oct-88 RSC Fix for ONE_DB compile
  441 06-Dec-88 RSC Move defn of MESSAGE from netgen.h to here
  532 06-Jan-89 RSC Fixed defn of DB_ID for ONE_DB
      21-Mar-90 WLW Generalized offsets to account for longer INT's
      26-Mar-90 KGM Fixed DBN_PARAM for ZORTECH
      17-Dec-90 DPG Added db_exit() for OS2_MT
 1306 25-Jul-91 DJS Bracket unused DB_ENTRY_P and RN_ENTRY_P when ONE_DB
 1430 30-Oct-91 DMB Full support of context switching using multiple caches
*/

/* Compiler dependencies are handled with ifdef macros:
NOTE: One of these macros should be defined on the compile command,
for example
        cc -c -DUNIX fillnew.c
Failure to do so will produce compilation errors.

   UNIX    = Unix C compiler
   LAT     = Lattice
   MSC     = Microsoft
   VMS     = Vax/VMS
   TURBO   = Turbo C
*/
#ifdef MSC
#else
#ifdef TURBO
#else
#ifdef ZOR
#else
You need a 'compiler' definition in the compile command!!

#endif
#endif
#endif



#ifndef OMAN         /* OMAN does not like these */

#define PGHOLD 1
#define NOPGHOLD 0
#define PGFREE 1
#define NOPGFREE 0

#define KEYFIND 0
#define KEYNEXT 1
#define KEYPREV 2
#define KEYFRST 3
#define KEYLAST 4

#endif /* OMAN */

/* dictionary attribute flags */
#define SORTFLD     0x0001             /* field is a set sort field */
#define STRUCTFLD   0x0002             /* field is sub-field of struct */
#define UNSIGNEDFLD 0x0004             /* field is unsigned */
#define STATIC      0x0002             /* file/record is static */
#define LOCAL       0x0004             /* file/record is local */
#define COMKEYED    0x0010             /* record/field contains/included
                                        * in compound key */
#define TIMESTAMPED 0x0001             /* record/set is timestamped */
#define NEEDS_COMMIT 0x0001            /* file has been written during
                                        * transaction */

#define TRUE 1
#define FALSE 0
#define YES 1
#define NO 0

#ifndef OMAN

#define DBUSER_ALIVE_BYTE   1
#define DBUSER_COMMIT_BYTE  2

#define BITS_PER_BYTE 8

/* Number of bits in a "type" */
#define BITS( type )  (BITS_PER_BYTE * sizeof( type ))

/* Number of elements in a vector */
#define arraysize(v) (sizeof(v)/sizeof(*(v)))

#endif /* OMAN */

/* Number of bytes in a disk block */
#define D_BLKSZ 512

#ifndef OMAN

#define DBD_COMPAT_LEN 6
#define INT_SIZE     sizeof(int)
#define SHORT_SIZE   sizeof(short)
#define LONG_SIZE    sizeof(long)
#define FLOAT_SIZE   sizeof(float)
#define DOUBLE_SIZE  sizeof(double)
#define CHAR_SIZE    sizeof(char)
#define DB_ADDR_SIZE sizeof(long)
#define PGHDRSIZE 4
#define CTBNAME "vista.ctb"

#endif /* OMAN */

#define RECHDRSIZE (sizeof(DB_SHORT) + DB_ADDR_SIZE)
#define NONE -1L
#define FILEMASK 0xFFL
#define ADDRMASK 0x00FFFFFFL
#define FILESHIFT 24
#define OPTKEYNDX 0x003F
#define OPTKEY_LIMIT 63

#if DB_SHORT_LEN == 4
#define OPTKEYSHIFT 26
#define OPTKEYMASK 0xFC000000
#define RLBMASK 0x40000000
#elif DB_SHORT_LEN == 2
#define OPTKEYSHIFT 10
#define OPTKEYMASK 0xFC00
#define RLBMASK 0x4000
#endif

/* creation timestamp in rec hdr */
#define RECCRTIME (sizeof(DB_SHORT) + DB_ADDR_SIZE)

/* update timestamp in rec hdr */
#define RECUPTIME (RECCRTIME + sizeof(long))

#ifndef OMAN

/* max size of set pointer */
/*                member count   prior + next     timestamp (opt) */
#define SETPSIZE (sizeof(long) + 2*DB_ADDR_SIZE + sizeof(DB_ULONG))

/* Offset to total members in set ptr */
#define SP_MEMBERS 0

/* Offset to first member ptr in set ptr */
#define SP_FIRST   sizeof(long)

/* Offset to last member ptr in set ptr */
#define SP_LAST    (SP_FIRST+DB_ADDR_SIZE)

/* Offset to timestamp in set ptr */
#define SP_UTIME   (SP_LAST+DB_ADDR_SIZE)

/* Size of member pointer = 3*DB_ADDR_SIZE */
#define MEMPSIZE   (3*DB_ADDR_SIZE)

/* Offset to owner ptr in member ptr */
#define MP_OWNER   0

/* Offset to previous member ptr in member ptr */
#define MP_PREV    (MP_OWNER+DB_ADDR_SIZE)

/* Offset to next member ptr in member ptr */
#define MP_NEXT    (MP_PREV+DB_ADDR_SIZE)

#endif /* OMAN */

/* maximum length of a database file name */
#ifndef FILENMLEN
#define FILENMLEN  48
#endif
#ifndef LOGFILELEN
#define LOGFILELEN 48
#endif


/* maximum length of a database name */
#define DBNMLEN 9

/* maximum length of a database path */
#define DB_PATHLEN FILENMLEN-11

#ifndef OMAN

/*------------ transaction logging data ------------*/
#define DEFIXPAGES 4             /* default number of index cache pages */
#define MINIXPAGES 2             /* minimum number of index cache pages */
/*------------ end of transaction logging data ------------*/

#define DEFDBPAGES 16            /* default number of database cache pages */
#define MINDBPAGES 8             /* minimum number of database cache pages */
#define LENVDBID 48
#define MAXKEYSIZE 256

#endif /* OMAN */

#define MAXDIMS 3

#define OPEN 'o'
#define CLOSED 'c'
#define DATA 'd'
#define KEY 'k'
#define DB_OVERFLOW 'o'

#ifndef OMAN

/* virtual page table entry */
typedef struct page_entry
{
   DB_TASK task;                 /* task pointer */
   FILE_NO file;                 /* file table entry number (0..size_ft-1) */
   F_ADDR pageno;                /* database page number */
   BOOLEAN recently_used;        /* LRU flag */
   BOOLEAN modified;             /* TRUE if page has been modified */
   DB_SHORT holdcnt;             /* "hold-in-cache" counter */
   short lu_slot;                /* lookup table slot pointing to this entry */
   F_ADDR ovfl_addr;             /* overflow file address of page */
   CHAR_P Buff;                  /* page buffer pointer */
   short buff_size;
} PAGE_ENTRY;

#define buff Buff.ptr

/* page table slot pointing to this entry */
typedef PAGE_ENTRY DB_FAR * LOOKUP_ENTRY_S;
typedef PAGE_ENTRY DB_FAR * LOOKUP_ENTRY;

#endif /* OMAN */

typedef struct FILE_ENTRY_S
{
   char ft_name[FILENMLEN];            /* name of file */
   DB_SHORT ft_desc;                   /* file descriptor */
   char ft_status;                     /* 'o'=opened, 'c'=closed */
   char ft_type;                       /* 'd'=data,   'k'=key, 'o'=overflow */
   DB_SHORT ft_slots;                  /* record slots per page */
   DB_SHORT ft_slsize;                 /* size of record slots in bytes */
   DB_SHORT ft_pgsize;                 /* size of page */
   DB_SHORT ft_flags;                  /* 0x0002 is set if file is static */
} FILE_ENTRY;

typedef struct REOPEN_ENTRY_S
{
   OFSTRUCT ro_info;                   /* file reopening info for Windows */
#define OPENED 'o'
#define NEVER_OPENED 'n'
   char ro_status;                     /* 'o'=opened, 'n'=never opened */
} REOPEN_ENTRY;


typedef struct RECORD_ENTRY_S
{
   DB_SHORT rt_file;                   /* file table entry of file
                                        * containing record */
   DB_SHORT rt_len;                    /* total length of record */
   DB_SHORT rt_data;                   /* offset to start of data in record */
   DB_SHORT rt_fields;                 /* first field def in field_table */
   DB_SHORT rt_fdtot;                  /* total number of fields in record */
   DB_SHORT rt_flags;                  /* 0x0001 is set if record is
                                        * timestamped 0x0002 is set if
                                        * record is static */
} RECORD_ENTRY;

#ifndef OMAN

#define FIRST      'f'
#define LAST       'l'
#define NEXT       'n'

#endif /* OMAN */

#define ASCENDING  'a'
#define DESCENDING 'd'
#define NOORDER    'n'

typedef struct SET_ENTRY_S
{
   DB_SHORT st_order;            /* 'f'=first, 'l'=last,
                                  * 'a'=ascending, 'd'=descending,
                                  * 'n'=no order */
   DB_SHORT st_own_rt;           /* record table entry of owner */
   DB_SHORT st_own_ptr;          /* offset to set pointers in record */
   DB_SHORT st_members;          /* index of first member record in
                                  * member table */
   DB_SHORT st_memtot;           /* total number of members of set */
   DB_SHORT st_flags;            /* 0x0001 is set if record is
                                  * timestamped */
} SET_ENTRY;


typedef struct MEMBER_ENTRY_S
{
   DB_SHORT mt_record;           /* record table entry for this member */
   DB_SHORT mt_mem_ptr;          /* offset to member ptrs in record */
   DB_SHORT mt_sort_fld;         /* sort table entry of first sort field */
   DB_SHORT mt_totsf;            /* total number of sort fields */
} MEMBER_ENTRY;


typedef struct SORT_ENTRY_S
{
   DB_SHORT se_fld;              /* field table entry of sort field */
   DB_SHORT se_set;              /* set table entry of sorted set */
}  SORT_ENTRY;


#define NOKEY      'n'
#define DUPLICATES 'd'
#define UNIQUE     'u'
#define CHARACTER  'c'
#define SHORTINT   's'
#define REGINT     'i'
#define LONGINT    'l'
#define FLOAT      'f'
#define DOUBLE     'F'
#define DBADDR     'd'
#define GROUPED    'g'
#define COMKEY     'k'

typedef struct FIELD_ENTRY_S
{
   char fd_key;                        /* 'n'=none, 'd'=duplicates,
                                        * 'u'=unique */
   unsigned char fd_type;              /* 'c'=char, 's'=short, 'i'=int,
                                        * 'l'=long, 'd'=DB_ADDR, 'f' =
                                        * float, 'D' = double, 'g'=grouped
                                        * field, 'k' = compound key */
   DB_SHORT fd_len;                    /* length of field in bytes */
   DB_SHORT fd_dim[MAXDIMS];           /* size of each array dimension */
   DB_SHORT fd_keyfile;                /* file_table entry for key file */
   DB_SHORT fd_keyno;                  /* key prefix number */
   DB_SHORT fd_ptr;                    /* offset to field in record or 1st
                                        * compound key field in key_table */
   DB_SHORT fd_rec;                    /* record table entry of record
                                        * containing field */
   DB_SHORT fd_flags;                  /* 0x0001 is set if field in "by"
                                        * clause of set spec 0x0002 is set
                                        * if field member of struct field
                                        * 0x0004 is set if field is
                                        * unsigned data type 0x0008 is set
                                        * if field is an optional key */
} FIELD_ENTRY;

/* compound key table entry declaration */
typedef struct KEY_ENTRY_S
{
   DB_SHORT kt_key;              /* compound key field number */
   DB_SHORT kt_field;            /* field number of included field */
   DB_SHORT kt_ptr;              /* offset to start of field data in key */
   DB_SHORT kt_sort;             /* 'a' = ascending, 'd' = descending */
} KEY_ENTRY;


/* database table entry declaration */
typedef struct DB_ENTRY_S
{
   /* NOTE: db_name, db_path and ft_offset defined in TASK if ONE_DB */
   char db_name[DBNMLEN];              /* name of this database */
   char db_path[DB_PATHLEN];           /* name of path to this database */
   DB_ADDR sysdba;                     /* database address of system record */
   DB_ADDR curr_dbt_rec;               /* this db's current record */
   DB_SHORT Page_size;                 /* size of this db's page */
   DB_SHORT Size_ft;                   /* size of this db's file_table */
   DB_SHORT ft_offset;                 /* offset to start of this db's
                                        * file_table entries */
   DB_SHORT Size_rt;                   /* size of this db's record_table */
   DB_SHORT rt_offset;                 /* offset to start of this db's
                                        * record_table entries */
   DB_SHORT Size_fd;                   /* size of this db's field_table */
   DB_SHORT fd_offset;                 /* offset to start of this db's
                                        * field_table entries */
   DB_SHORT Size_st;                   /* size of this db's set_table */
   DB_SHORT st_offset;                 /* offset to start of this db's
                                        * set_table entries */
   DB_SHORT Size_mt;                   /* size of this db's member_table */
   DB_SHORT mt_offset;                 /* offset to start of this db's
                                        * member_table entries */
   DB_SHORT Size_srt;                  /* size of this db's sort_table */
   DB_SHORT srt_offset;                /* offset to start of this db's
                                        * sort_table entries */
   DB_SHORT Size_kt;                   /* size of this db's key_table */
   DB_SHORT kt_offset;                 /* offset to start of this db's key
                                        * table entries */
   DB_SHORT key_offset;                /* key prefix offset for this db */
} DB_ENTRY;

/* Structure containing current record type & address for recfrst/set/next..*/
/* NOTE: rn_type and rd_dba defined in TASK when ONE_DB */
typedef struct RN_ENTRY_S
{
   DB_SHORT rn_type;                   /* Last record type supplied to
                                        * recfrst/recset */
   DB_ADDR rn_dba;                     /* Last db addr computed by
                                        * recfrst/recset/recnext */
} RN_ENTRY;

#define  DB_REF(item)            (db_global.Curr_db_table->item)
#define  RN_REF(item)            (curr_rn_table->item)
#define  NUM2INT(num, offset)    ((num) + db_global.Curr_db_table->offset)
#define  NUM2EXT(num, offset)    ((num) - db_global.Curr_db_table->offset)
#define  ORIGIN(offset)          (curr_db_table->offset)

#ifndef OMAN

#define  TABLE_SIZE(size)        DB_REF(size)

/* DBN_PARM... and TASK_PARM... are used for function interfaces */
#define  TASK_PARM               , task
#ifdef ZOR
#define  ATASK_PARM              , DB_TASK DB_FAR *task
#endif
#define  TASK_PARM_DBN           task ,
#define  TASK_ONLY               task
#define  TASK_DECL               DB_TASK DB_FAR *task;
#define  TASK_PTR_DECL           DB_TASK DB_FAR *task;
#define  TASK_DBN_ONLY           task DBN_PARM
#define  DBN_TASK_ONLY           DBN_ONLY , task

/* Country code table definition */
typedef struct CNTRY_TBL_S
{
   unsigned char out_chr;
   unsigned char sort_as1;
   unsigned char sort_as2;
   unsigned char sub_sort;
} CNTRY_TBL;


/* names lists structure declaration */
typedef struct OBJNAMES_S
{
   char **recs;
   char **flds;
   char **sets;
} OBJNAMES;


/* directory separator character:
   MS-DOS: '\'
   UNIX:   '/'
*/
#define DIRCHAR '\\'

/* The following define aid in the use of a set of routines which use RMS
   directly to do the i/o to database files. */

#define DB_TELL tell
#define DB_OPEN  open_b
#define DB_CLOSE _lclose
#define DB_WRITE _lwrite
#define DB_READ  _lread
#define DB_LSEEK _llseek


#pragma pack(1)
typedef struct
{
   FILE_NO DB_FAR *ptr;
   LOCK_DESC
} FILE_NO_P;

typedef struct
{
   int DB_FAR *ptr;
   LOCK_DESC
}  INT_P;

typedef struct
{
   struct NODE_PATH_S DB_FAR *ptr;
   LOCK_DESC
} NODE_PATH_P;

typedef struct
{
   struct LM_LOCK_S DB_FAR *ptr;
   LOCK_DESC
} LM_LOCK_P;

typedef struct
{
   struct LM_FREE_S DB_FAR *ptr;
   LOCK_DESC
} LM_FREE_P;

typedef struct
{
   struct LM_DBOPEN_S DB_FAR *ptr;
   LOCK_DESC
} LM_DBOPEN_P;

typedef struct
{
   struct LM_DBCLOSE_S DB_FAR *ptr;
   LOCK_DESC
} LM_DBCLOSE_P;

typedef struct
{
   struct LR_DBOPEN_S DB_FAR *ptr;
   LOCK_DESC
} LR_DBOPEN_P;

typedef struct
{
   struct KEY_INFO_S DB_FAR *ptr;
   LOCK_DESC
} KEY_INFO_P;

typedef struct
{
   struct ren_entry DB_FAR *ptr;
   LOCK_DESC
} REN_ENTRY_P;

typedef struct
{
   LOOKUP_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} LOOKUP_ENTRY_P;

typedef struct
{
   struct page_entry DB_FAR *ptr;
   LOCK_DESC
} PAGE_ENTRY_P;

typedef struct
{
   struct FILE_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} FILE_ENTRY_P;

typedef struct
{
   struct DB_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} DB_ENTRY_P;

typedef struct
{
   struct RN_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} RN_ENTRY_P;

typedef struct
{
   TASK_P DB_FAR *ptr;
   LOCK_DESC
}  TASK_TABLE_P;

typedef struct
{
   struct REOPEN_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} REOPEN_ENTRY_P;

typedef struct
{
   struct RECORD_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} RECORD_ENTRY_P;

typedef struct
{
   struct SET_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} SET_ENTRY_P;

typedef struct
{
   struct MEMBER_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} MEMBER_ENTRY_P;

typedef struct
{
   struct SORT_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
}  SORT_ENTRY_P;

typedef struct
{
   struct FIELD_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} FIELD_ENTRY_P;

typedef struct
{
   struct KEY_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} KEY_ENTRY_P;

typedef struct
{
   struct PGZERO_S DB_FAR *ptr;
   LOCK_DESC
} PGZERO_P;

typedef struct
{
   struct RI_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} RI_ENTRY_P;

typedef struct
{
   DB_ULONG DB_FAR *ptr;
   LOCK_DESC
}  ULONG_P;

typedef struct
{
   long DB_FAR *ptr;
   LOCK_DESC
} LONG_P;

typedef struct
{
   struct CNTRY_TBL_S DB_FAR *ptr;
   LOCK_DESC
} CNTRY_TBL_P;

struct sk
{
   DB_SHORT sk_fld;
   CHAR_P sk_val;
};

typedef struct
{
   struct sk DB_FAR *ptr;
   LOCK_DESC
} SK_P;

#pragma pack()

/* used file entry declarations  */
typedef struct USED_FILE_ENTRY_S
{
   int fno;
   short recently_used;
}  USED_FILE_ENTRY;

typedef struct
{
   struct USED_FILE_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
}  USED_FILE_ENTRY_P;

/* node key search path stack entry: one per level per key field */
typedef struct NODE_PATH_S
{
   F_ADDR node;                        /* node (page) number  */
   DB_SHORT slot;                      /* slot number of key */
} NODE_PATH;

/* index key information: one entry per key field */
typedef struct KEY_INFO_S
{
   NODE_PATH_P Node_path;              /* stack of node #s in search path */
   DB_SHORT level;                     /* current level # in node path */
   DB_SHORT max_lvls;                  /* maximum possible levels for key */
   DB_SHORT lstat;                     /* last key function status */
   DB_SHORT fldno;                     /* field number of key */
   FILE_NO keyfile;                    /* key file containing this key */
   CHAR_P Keyval;                      /* ptr to last key value */
   DB_ADDR dba;                        /* db address of last key */
} KEY_INFO;

#define node_path Node_path.ptr
#define keyval Keyval.ptr

/* key slot structure */
typedef struct key_slot
{
   F_ADDR child;                       /* child node pointer */
   DB_SHORT keyno;                     /* key number */
   char data[1];                       /* start of key data */
} KEY_SLOT;

typedef union key_type
{
   char kd[256];                 /* This makes KEY_TYPE 256 bytes long. */
   struct key_slot ks;
} KEY_TYPE;

/* file rename table entry declarations */
typedef struct ren_entry
{
   CHAR_P Ren_db_name;
   CHAR_P File_name;
   FILE_NO file_no;
}  REN_ENTRY;

#ifdef MSC                             /* FAR_ALLOC & MSC */
#define bytecpy(s1, s2, n)  _fmemcpy((DB_VOID DB_FAR *)(s1), (DB_VOID DB_FAR *)(s2), (unsigned)(n))
#define bytecmp(s1, s2, n)  _fmemcmp((DB_VOID DB_FAR *)(s1), (DB_VOID DB_FAR *)(s2), (unsigned)(n))
#define byteset(s, c, n)    _fmemset((DB_VOID DB_FAR *)(s), (int)(c), (unsigned)(n))
#define vstrcpy(a, b)       _fstrcpy((char DB_FAR *)(a), (char DB_FAR *)(b))
#define vstrcmp(a, b)       _fstrcmp((char DB_FAR *)(a), (char DB_FAR *)(b))
#define vstrcat(a, b)       _fstrcat((char DB_FAR *)(a), (char DB_FAR *)(b))
#define vstrncat(s1, s2, n) _fstrncat((char DB_FAR *)(s1), (char DB_FAR *)(s2), (unsigned)(n))
#define vstrlen(a)          _fstrlen((char DB_FAR *)(a))
#define vstrncmp(s1, s2, n) _fstrncmp((char DB_FAR *)(s1), (char DB_FAR *)(s2), (unsigned)(n))
#define vstrncpy(s1, s2, n) _fstrncpy((char DB_FAR *)(s1), (char DB_FAR *)(s2), (unsigned)(n))
#define vstrchr(s, c)       _fstrchr((char DB_FAR *)(s), (int)(c))
#define vstrrchr(s, c)      _fstrrchr((char DB_FAR *)(s), (int)(c))
#else                                  /* FAR_ALLOC && !MSC */
#define bytecpy(s1, s2, n)  lmemcpy((DB_VOID DB_FAR *)(s1), (DB_VOID DB_FAR *)(s2), (unsigned)(n))
#define bytecmp(s1, s2, n)  lmemcmp((DB_VOID DB_FAR *)(s1), (DB_VOID DB_FAR *)(s2), (unsigned)(n))
#define byteset(s, c, n)    lmemset((DB_VOID DB_FAR *)(s), (int)(c), (unsigned)(n))
#define vstrcpy(a, b)       lstrcpy((char DB_FAR *)(a), (char DB_FAR *)(b))
#define vstrcmp(a, b)       lstrcmp((char DB_FAR *)(a), (char DB_FAR *)(b))
#define vstrcat(a, b)       lstrcat((char DB_FAR *)(a), (char DB_FAR *)(b))
#define vstrncat(s1, s2, n) _fstrncat((char DB_FAR *)(s1), (char DB_FAR *)(s2), (unsigned)(n))
#define vstrlen(a)          lstrlen((char DB_FAR *)(a))
#define vstrncmp(s1, s2, n) lstrncmp((char DB_FAR *)(s1), (char DB_FAR *)(s2), (unsigned)(n))
#define vstrncpy(s1, s2, n) lstrncpy((char DB_FAR *)(s1), (char DB_FAR *)(s2), (unsigned)(n))
#define vstrchr(s, c)       lstrchr((char DB_FAR *)(s), (int)(c))
#define vstrrchr(s, c)      lstrrchr((char DB_FAR *)(s), (int)(c))
#endif                                 /* FAR_ALLOC */

#ifndef NO_DBN                         /* ! NO_DBN */

/* note: CURR_DB == -1 */

#ifdef COMMA                           /* ! NO_DBN & COMMA */
#define DB_ID , dbn
#define NO_DB_ID , VOID_DB
#define DBN_PARAM , dbn
#else                                  /* ! NO_DBN & ! COMMA */
#define DB_ID dbn
#define NO_DB_ID VOID_DB
#define DBN_PARAM dbn
#define COMMA
#endif                                 /* ! NO_DBN */
#define ALL_DBS_PARAM , ALL_DBS
#else                                  /* NO_DBN */
#define DBN_PARAM                      /**/
#define DB_ID                          /**/
#define DB_PARM                        /**/
#define NO_DB_ID                       /**/
#define ALL_DBS_PARAM                  /**/
#endif                                 /* */

#ifdef COMMA                           /* MULTI_TASK & COMMA */
#define TASK_ID , task
#define TASK_PTR_ID , task
#define NO_TASK_ID , NULL
#else                                  /* MULTI_TASK & ! COMMA */
#define TASK_ID task
#define TASK_PTR_ID task
#define NO_TASK_ID NULL
#endif                                 /* MULTI_TASK */
#ifdef COMMA                           /* MULTI_TASK & COMMA */
#define TASK_PARAM , c
#else                                  /* MULTI_TASK & ! COMMA */
#define TASK_PARAM c
#define COMMA
#endif                                 /* MULTI_TASK */

#define LOCK_DECL int lock_id;
#ifdef COMMA                           /* MEMLOCK & COMMA */
#define LOCK_SET(a) , a
#else                                  /* MEMLOCK & ! COMMA */
#define LOCK_SET(a) a
#endif                                 /* MEMLOCK */
#ifdef COMMA                           /* MEMLOCK & COMMA */
#define LOCK_PARAM , d
#define LOCK_PARM , lock_id
#else                                  /* MEMLOCK & ! COMMA */
#define LOCK_PARAM d
#define LOCK_PARM lock_id
#define COMMA
#endif                                 /* MEMLOCK */

#define TDTL_PARAM DBN_PARAM TASK_PARAM LOCK_PARAM

#define ALLOC(l, b) d_alloc((CHAR_P DB_FAR *)(l), (unsigned)(b))
#define RETURN(x)      {if (!ProtectedMode) return( d_lockpop(x)); else return(x);}
#define MEM_LOCK(cp)    if (!ProtectedMode) d_memlock((CHAR_P DB_FAR *)(cp))
#define MEM_UNLOCK(cp)  if (!ProtectedMode) d_memunlock((CHAR_P DB_FAR *)(cp))
#define FREE(cp)        d_free((CHAR_P DB_FAR *)(cp))

typedef struct {
   struct CACHE_S DB_FAR *ptr;
   LOCK_DESC
} CACHE_P;

typedef struct {
   CACHE_P DB_FAR *ptr;
   LOCK_DESC
} CACHE_TABLE_P;

/* Lock sets are defined in dblfcns.c */
#define LOCK_NONE       -1
#define LOCK_ALL        0
#define LOCK_IO         1
#define LOCK_NOIO       2
#define RECORD_IO       3
#define RECORD_NOIO     4
#define SET_IO          5
#define SET_NOIO        6

#define TIMEOUT_DEF     10

/* record/set lock descriptor */
struct lock_descr
{
   FILE_NO_P fl_list;                  /* array of files used by
                                        * record/set */
   int fl_cnt;                         /* Number of elements in fl_list */
   BOOLEAN fl_kept;                    /* Is lock kept after transaction? */
   char fl_type;                       /* lock type: 'r' = read, 'w' =
                                        * write, 'x' = exclusive, 'f' =
                                        * free */
   char fl_prev;                       /* previous lock type */
};
typedef struct
{
   struct lock_descr DB_FAR *ptr;
   LOCK_DESC
} LOCK_DESCR_P;

/* Maximum number of transactions which can commit a time, 1 sector */
/* #define TAFLIMIT ((512-sizeof(DB_SHORT))/FILENMLEN) */
#define TAFLIMIT 9

/* structure to read TAF data into */
typedef struct TAFFILE_S
{
   short    lmc_type;
   short    user_count;
   char     lmc_lockmgrn[FILENMLEN];
   DB_SHORT cnt;
   char     files[TAFLIMIT][FILENMLEN];
}  TAFFILE;


#ifdef SEGMENTS
#define DB_STATIC /*  */
#else
#define DB_STATIC static
#endif

#define VISTATASK_ONLY &Vistatask
#define ONLY_VISTATASK_DBN &Vistatask DBN_PARM
#define VISTATASK_PARM , &Vistatask

/* MTYPE is .eq. to fcn and is used with the lockmgr */
typedef int  MTYPE;

#include "trxlog.h"
#include "dblock.h"
#include "ll.h"
#include "dbxtrn.h"

#include "proto.h"
#ifdef NO_DT_COVER
#include "dtcover.h"
#endif

#endif /* OMAN */
#endif /* DBTYPE_H */
/* vpp @win.vpp dbtype.h (Wed Jul 08 18:06:19 1992)
 *
 *     -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS
 *     -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC
 *     -nARCHIVING -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES
 *     -dMULTI_TASK -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP
 *     -nSINGLE_USER -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4
 *     -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY -nTRUENAME -nLAT
 */

