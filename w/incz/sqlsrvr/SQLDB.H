/* SQLDB.H - DB-Library header file for the Microsoft SQL Server.
**      The SYBASE SQL Server database for PC networks.
**
** Copyright (c) 1989, 1990 by Microsoft Corp.  All rights reserved.
** Copyright (c) 1989, 1990 by Sybase, Inc.  All rights reserved.
**
*/

#if defined( __OS2__ ) || defined( __WIN32__ )
#define  DBPROCESS  void       /* dbprocess structure type */
#define  LOGINREC   void       /* login record type */
#define  far
#ifdef __OS2__
#define  CALLBACK            EXPENTRY
typedef  void (CALLBACK*     FARPROC)(void);
#endif
#else
#define  DBPROCESS  void _near /* dbprocess structure type */
#define  LOGINREC   void _near   /* login record type */
#endif

/* Macros for setting the loginrec */
#define DBSETLHOST(a,b)    dbsetlname((a), (b), DBSETHOST)
#define DBSETLUSER(a,b)         dbsetlname((a), (b), DBSETUSER)
#define DBSETLPWD(a,b)          dbsetlname((a), (b), DBSETPWD)
#define DBSETLHID(a,b)          dbsetlname((a), (b), DBSETHID)
#define DBSETLAPP(a,b)          dbsetlname((a), (b), DBSETAPP)
#define BCP_SETL(a,b)      bcp_setl((a), (b))

/* Macros for referencing the dbproc */
#define DBDEAD(a)    dbdead(a)
#define DBRBUF(a)               ((DBINT)dbdataready(a))
#define dbrbuf(a)               ((DBINT)dbdataready(a))
#define DBCOUNT(a)      dbcount(a)
#define DBCURCMD(a)     dbcurcmd(a)
#define DBCURROW(a)     dbcurrow(a)
#define DBFIRSTROW(a)      dbfirstrow(a)
#define DBLASTROW(a)    dblastrow(a)
#define DBNUMORDERS(a)     dbnumorders(a)
#define DBROWS(a)    dbrows(a)
#define DBMORECMDS(a)      dbmorecmds(a)
#define DBROWTYPE(a)    dbrowtype(a)
#define DBCMDROW(a)     dbcmdrow(a)
#define DBISAVAIL(a)    dbisavail(a)
#define DBGETTIME()     dbgettime()

#ifndef DBMSWIN

/* Macros for locking and unlocking Windows data segment (for compatibility) */
#define DBLOCKLIB
#define DBUNLOCKLIB

#else /* #ifndef DBMSWIN; Windows compilation */

/* Macros for locking and unlocking the DBLIB data segment */
#define DBLOCKLIB()  dblocklib()
#define DBUNLOCKLIB()   dbunlocklib()

#endif   /* #ifndef DBMSWIN */

typedef long (far pascal *LGFARPROC)();      // os/2/windows loadable driver fp

/* Bulk Copy Definitions (bcp) */

#define DB_IN  1         /* transfer from client to server */
#define DB_OUT 2         /* transfer from server to client */

#define BCPMAXERRS  1          /* bcp_control parameter */
#define BCPFIRST    2          /* bcp_control parameter */
#define BCPLAST     3          /* bcp_control parameter */
#define BCPBATCH    4          /* bcp_control parameter */

#ifdef NOTDEF

/* Global Variables. */
extern CHAR     *errarray[];  /* array of error message strings   */
extern USHORT      DbTimeOut;    /* time out period in seconds     */
extern USHORT      DbLoginTime;  /* login time out period in seconds */
extern USHORT      DbMaxProcs;      /* max number of allowed dbprocs per process */
extern DBPROCESS    **DbProcArray;  /* array of valid dbprocs.     */
extern OPTIONDICT   OptionDict[14]; /* options dictionary          */
extern CHAR     *Offsets[];      /* offset modifier list        */
extern CHAR     *dbon;     /* " on " for SetOptionsParams()    */
extern CHAR     *dboff;    /* " off " for SetOptionsParams()   */
extern BYTE     *MonthAbr[];  /* month abbreviations array      */
extern BYTE ConvertArray[MAXTYPES][MAXTYPES]; /* will convert truth table   */
extern BYTE     DblibName[];  /* DBLIB name for LOGINREC     */
extern BYTE     DblibVer[];   /* DBLIB version for LOGINREC     */
extern BYTE     TdsVer[];     /* TDS version for LOGINREC       */
extern CHAR     *prtypes[];      /* dbprtype() string array */

#endif

#ifdef DBMSDOS

/* error handler */
extern INT ( API * DbErrHandler)(DBPROCESS *,INT,INT,INT,CHAR *,CHAR *);

/* message handler */
extern INT ( API * DbMsgHandler)(DBPROCESS *,DBINT,INT,INT,CHAR *);

/* Dblib function prototypes */
extern   LOGINREC *  API dblogin(void );
extern   DBPROCESS * API dbopen(LOGINREC *,CHAR *);

extern  INT (API *dberrhandle( INT (API *)( DBPROCESS *,INT,INT,INT,CHAR *,CHAR *)))();
extern   INT (API *dbmsghandle( INT (API *)( DBPROCESS *,DBINT,DBSMALLINT,DBSMALLINT,CHAR *)))();

extern   BYTE *   API dbinit(void);
extern  RETCODE API dbsetlname(LOGINREC *,CHAR *,INT);
extern   void  API dbfreelogin(LOGINREC *);
extern  RETCODE API dbresults(DBPROCESS *);
extern   STATUS   API dbnextrow(DBPROCESS *);
extern   void  API dbclose(DBPROCESS *);
extern   RETCODE API dbcmd(DBPROCESS *,CHAR *);
extern   RETCODE API dbfcmd(DBPROCESS *, CHAR *, ...);
extern   INT   API dbstrlen(DBPROCESS *);
extern   RETCODE API dbstrcpy(DBPROCESS *,INT,INT,CHAR *);
extern   CHAR *   API dbgetchar(DBPROCESS *, INT);
extern  RETCODE API dbsqlexec(DBPROCESS *);
extern  RETCODE API dbsqlok(DBPROCESS *);
extern  RETCODE API dbsqlsend(DBPROCESS *);
extern  RETCODE API dbcanquery(DBPROCESS *);
extern   STATUS   API dbgetrow(DBPROCESS *,DBINT);
extern   void  API dbclrbuf(DBPROCESS *,DBINT);
extern   void  API dbfreebuf(DBPROCESS *);
extern  RETCODE API dbsettime(INT);
extern  RETCODE API dbsetlogintime(INT);
extern   BOOL  API dbdataready(DBPROCESS *);
extern  RETCODE API dbcancel(DBPROCESS *);
extern   void  API dbexit(void);
extern  RETCODE API dbsetopt(DBPROCESS *,INT,CHAR *);
extern   BOOL  API dbisopt(DBPROCESS *, INT, CHAR *);
extern  RETCODE API dbclropt(DBPROCESS *,INT,CHAR *);
extern   INT   API dbordercol(DBPROCESS *,INT);
extern   INT   API dbconvert(DBPROCESS *,INT,BYTE *,DBINT,INT,BYTE *,DBINT);
extern   DBINT API dbcollen(DBPROCESS *,INT);
extern  BYTE *  API dbcolname(DBPROCESS *,INT);
extern   INT   API dbcoltype(DBPROCESS *,INT);
extern  BYTE *  API dbdata(DBPROCESS *,INT);
extern   DBINT API dbdatlen(DBPROCESS *,INT);
extern   INT   API dbnumcols(DBPROCESS *);
extern   BOOL  API dbcolbrowse(DBPROCESS *,INT);
extern  CHAR *  API dbtabname(DBPROCESS *,INT);
extern  CHAR *  API dbcolsource(DBPROCESS *,INT);
extern   BOOL  API dbtabbrowse(DBPROCESS *,INT);
extern  CHAR *  API dbtabsource(DBPROCESS *, INT, INT *);
extern   INT   API dbtabcount(DBPROCESS *);
extern   INT   API dbtsnewlen(DBPROCESS *);
extern  RETCODE API dbtsput(DBPROCESS *,DBBINARY *,INT,INT,CHAR *);
extern  CHAR * API dbqual(DBPROCESS *,INT,CHAR *);
extern   void  API dbfreequal(CHAR *);
extern  void API dbprhead(DBPROCESS *);
extern  RETCODE API dbprrow(DBPROCESS *);
extern  RETCODE API dbtxtsput(DBPROCESS *,DBBINARY *,INT);
extern   RETCODE API dbwritetext(DBPROCESS *,CHAR *,DBBINARY *,DBTINYINT, DBBINARY *,DBBOOL,DBINT,BYTE *);
extern   RETCODE API dbmoretext(DBPROCESS *,DBINT,BYTE *);
extern  RETCODE API dbuse(DBPROCESS *,CHAR *);
extern  RETCODE API dbbind(DBPROCESS *,INT,INT,DBINT,BYTE *);
extern   BOOL  API dbwillconvert(INT,INT);
extern  RETCODE API dbsetnull(DBPROCESS *, INT, INT, BYTE *);
extern  RETCODE API dbaltbind(DBPROCESS *,INT,INT,INT,DBINT,BYTE *);
extern   DBINT API dbaltlen(DBPROCESS *,INT,INT);
extern   INT   API dbalttype(DBPROCESS *,INT,INT);
extern  BYTE *  API dbadata(DBPROCESS *,INT,INT);
extern   DBINT API dbadlen(DBPROCESS *,INT,INT);
extern   INT   API dbnumalts(DBPROCESS *,INT);
extern   INT   API dbaltcolid(DBPROCESS *,INT,INT);
extern   BYTE *   API dbbylist(DBPROCESS *,INT,INT *);
extern   INT   API dbaltop(DBPROCESS *,INT,INT);
extern   INT   API dbnumcompute(DBPROCESS *);
extern   CHAR *   API dbprtype(INT);
extern   CHAR *   API dbchange(DBPROCESS *);
extern   CHAR *   API dbname(DBPROCESS *);
extern   INT   API dbgetoff(DBPROCESS *,DBUSMALLINT,INT);
extern   DBINT API dbreadpage(DBPROCESS *,CHAR far *,DBINT,BYTE *);
extern   RETCODE API dbwritepage(DBPROCESS *,CHAR *,DBINT,DBINT,BYTE *);

extern   DBBINARY * API dbtsnewval(DBPROCESS *);
extern   DBBINARY * API dbtxptr(DBPROCESS *,INT);
extern   DBBINARY * API dbtxtimestamp(DBPROCESS *,INT);
extern   DBBINARY * API dbtxtsnewval(DBPROCESS *);

/* remote procedure call functions */
extern   DBBOOL   API dbhasretstat(DBPROCESS *);
extern   INT   API dbnumrets(DBPROCESS *);
extern   BYTE *   API dbretdata(DBPROCESS *, INT);
extern   DBINT API dbretlen(DBPROCESS *, INT);
extern   CHAR *   API dbretname(DBPROCESS *, INT);
extern   DBINT API dbretstatus(DBPROCESS *);
extern   INT   API dbrettype(DBPROCESS *, INT);
extern   RETCODE API dbrpcinit(DBPROCESS *, CHAR *, DBSMALLINT);
extern   RETCODE API dbrpcparam(DBPROCESS *,CHAR *,BYTE,INT,DBINT,DBINT,BYTE *);
extern   RETCODE API dbrpcsend(DBPROCESS *);
extern   void  API dbrpwclr(LOGINREC *);
extern   RETCODE API dbrpwset(LOGINREC *, CHAR *, CHAR *, INT);

/* two-phase commit functions */
extern DBPROCESS * API open_commit(LOGINREC *,CHAR *);
extern void    API close_commit(DBPROCESS *);
extern void    API build_xact_string(CHAR *,CHAR *,DBINT,CHAR *);
extern DBINT      API start_xact(DBPROCESS *,CHAR *,CHAR *,INT);
extern RETCODE    API commit_xact(DBPROCESS *,DBINT);
extern RETCODE    API abort_xact(DBPROCESS *,DBINT);
extern RETCODE    API remove_xact(DBPROCESS *,DBINT,INT);
extern RETCODE    API scan_xact(DBPROCESS *,DBINT);
extern INT     API stat_xact(DBPROCESS *,DBINT);

/* bulk copy prototypes */
extern DBINT   API bcp_batch(DBPROCESS *);
extern RETCODE API bcp_bind(DBPROCESS *, BYTE *, INT, DBINT, BYTE *, INT, INT, INT);
extern RETCODE API bcp_colfmt(DBPROCESS *, INT, BYTE, INT, DBINT, BYTE *, INT, INT);
extern RETCODE API bcp_collen(DBPROCESS *, DBINT, INT);
extern RETCODE API bcp_colptr(DBPROCESS *, BYTE *, INT);
extern RETCODE API bcp_columns(DBPROCESS *, INT);
extern RETCODE API bcp_control(DBPROCESS *, INT, DBINT);
extern DBINT   API bcp_done(DBPROCESS *);
extern RETCODE API bcp_exec(DBPROCESS *, DBINT *);
extern RETCODE API bcp_init(DBPROCESS *, CHAR *, CHAR *, CHAR *, INT);
extern RETCODE API bcp_moretext(DBPROCESS *, DBINT, BYTE *);
extern RETCODE API bcp_sendrow(DBPROCESS *);
extern RETCODE API bcp_setl(LOGINREC *, DBBOOL);

/* Functions for referencing the dbproc */
RETCODE API dbcmdrow(DBPROCESS *);
DBINT API dbcount(DBPROCESS *);
INT   API dbcurcmd(DBPROCESS *);
DBINT API dbcurrow(DBPROCESS *);
BOOL  API dbdead(DBPROCESS *);
DBINT API dbfirstrow(DBPROCESS *);
BOOL  API dbisavail(DBPROCESS *);
DBINT API dblastrow(DBPROCESS *);
RETCODE API dbmorecmds(DBPROCESS *);
INT   API dbnumorders(DBPROCESS *);
RETCODE API dbrows(DBPROCESS *);
STATUS   API dbrowtype(DBPROCESS *);

#endif

#ifdef DBMSOS2
/* error handler */
extern INT ( API * DbErrHandler)(DBPROCESS far *,INT,INT,INT,CHAR far *,CHAR far *);

/* message handler */
extern INT ( API * DbMsgHandler)(DBPROCESS far *,DBINT,INT,INT,CHAR far *);

/* extern USHORT DbTimeOut;   /* time out period in seconds    */

/* Dblib function prototypes */
extern LOGINREC far *  API dblogin(void );
extern DBPROCESS far * API dbopen(LOGINREC far *,CHAR far *);

extern INT (API *dberrhandle( INT (API *)( DBPROCESS far *,INT,INT,INT,CHAR far *,CHAR far *)))();
extern INT (API *dbmsghandle( INT (API *)( DBPROCESS far *,DBINT,DBSMALLINT,DBSMALLINT,CHAR far *)))();

extern BYTE far * API dbinit(void);
extern RETCODE   API dbsetlname(LOGINREC far *,CHAR far *,INT);
extern void   API dbfreelogin(LOGINREC far *);
extern RETCODE   API dbresults(DBPROCESS far *);
extern STATUS    API dbnextrow(DBPROCESS far *);
extern void   API dbclose(DBPROCESS far *);
extern RETCODE   API dbcmd(DBPROCESS far *,CHAR far *);
extern RETCODE   API dbfcmd(DBPROCESS far *, CHAR far *, ...);
extern INT    API dbstrlen(DBPROCESS far *);
extern RETCODE   API dbstrcpy(DBPROCESS far *,INT,INT,CHAR far *);
extern CHAR far * API dbgetchar(DBPROCESS far *, INT);
extern RETCODE   API dbsqlexec(DBPROCESS far *);
extern RETCODE   API dbsqlok(DBPROCESS far *);
extern RETCODE   API dbsqlsend(DBPROCESS far *);
extern RETCODE   API dbcanquery(DBPROCESS far *);
extern STATUS    API dbgetrow(DBPROCESS far *,DBINT);
extern void   API dbclrbuf(DBPROCESS far *,DBINT);
extern void   API dbfreebuf(DBPROCESS far *);
extern RETCODE   API dbsettime(INT);
extern RETCODE   API dbsetlogintime(INT);
extern BOOL   API dbdataready(DBPROCESS far *);
extern RETCODE   API dbcancel(DBPROCESS far *);
extern void   API dbexit(void);
extern RETCODE   API dbsetopt(DBPROCESS far *,INT,CHAR far *);
extern BOOL   API dbisopt(DBPROCESS far *, INT, CHAR far *);
extern RETCODE   API dbclropt(DBPROCESS far *,INT,CHAR far *);
extern INT    API dbordercol(DBPROCESS far *,INT);
extern INT    API dbconvert(DBPROCESS far *,INT,BYTE far *,DBINT,INT,BYTE far *,DBINT);
extern DBINT     API dbcollen(DBPROCESS far *,INT);
extern BYTE far * API dbcolname(DBPROCESS far *,INT);
extern INT    API dbcoltype(DBPROCESS far *,INT);
extern BYTE far * API dbdata(DBPROCESS far *,INT);
extern DBINT     API dbdatlen(DBPROCESS far *,INT);
extern INT    API dbnumcols(DBPROCESS far *);
extern BOOL   API dbcolbrowse(DBPROCESS far *,INT);
extern CHAR far * API dbtabname(DBPROCESS far *,INT);
extern CHAR far * API dbcolsource(DBPROCESS far *,INT);
extern BOOL   API dbtabbrowse(DBPROCESS far *,INT);
extern CHAR far * API dbtabsource(DBPROCESS far *, INT, INT far *);
extern INT    API dbtabcount(DBPROCESS far *);
extern INT    API dbtsnewlen(DBPROCESS far *);
extern RETCODE   API dbtsput(DBPROCESS far *,DBBINARY far *,INT,INT,CHAR far *);
extern CHAR far * API dbqual(DBPROCESS far *,INT,CHAR far *);
extern void   API dbfreequal(CHAR far *);
extern void   API dbprhead(DBPROCESS far *);
extern RETCODE   API dbprrow(DBPROCESS far *);
extern RETCODE   API dbtxtsput(DBPROCESS far *,DBBINARY far *,INT);
extern RETCODE   API dbwritetext(DBPROCESS far *,CHAR far *,DBBINARY far *,DBTINYINT, DBBINARY far *,DBBOOL,DBINT,BYTE far *);
extern RETCODE   API dbmoretext(DBPROCESS far *,DBINT,BYTE far *);
extern RETCODE   API dbuse(DBPROCESS far *,CHAR far *);
extern RETCODE   API dbbind(DBPROCESS far *,INT,INT,DBINT,BYTE far *);
extern BOOL   API dbwillconvert(INT,INT);
extern RETCODE   API dbsetnull(DBPROCESS far *, INT, INT, BYTE far *);
extern RETCODE   API dbaltbind(DBPROCESS far *,INT,INT,INT,DBINT,BYTE far *);
extern DBINT     API dbaltlen(DBPROCESS far *,INT,INT);
extern INT    API dbalttype(DBPROCESS far *,INT,INT);
extern BYTE far * API dbadata(DBPROCESS far *,INT,INT);
extern DBINT     API dbadlen(DBPROCESS far *,INT,INT);
extern INT    API dbnumalts(DBPROCESS far *,INT);
extern INT    API dbaltcolid(DBPROCESS far *,INT,INT);
extern BYTE far * API dbbylist(DBPROCESS far *,INT,INT far *);
extern INT    API dbaltop(DBPROCESS far *,INT,INT);
extern INT    API dbnumcompute(DBPROCESS far *);
extern CHAR far * API dbprtype(INT);
extern CHAR far * API dbchange(DBPROCESS far *);
extern CHAR far * API dbname(DBPROCESS far *);
extern INT    API dbgetoff(DBPROCESS far *,DBUSMALLINT,INT);
extern DBINT     API dbreadpage(DBPROCESS far *,CHAR far *,DBINT,BYTE far *);
extern RETCODE   API dbwritepage(DBPROCESS far *,CHAR far *,DBINT,DBINT,BYTE far *);
extern DBBOOL    API dbhasretstat(DBPROCESS far *);
extern INT    API dbnumrets(DBPROCESS far *);
extern BYTE far * API dbretdata(DBPROCESS far *, INT);
extern DBINT     API dbretlen(DBPROCESS far *, INT);
extern CHAR far * API dbretname(DBPROCESS far *, INT);
extern DBINT     API dbretstatus(DBPROCESS far *);
extern INT    API dbrettype(DBPROCESS far *, INT);
extern RETCODE   API dbrpcinit(DBPROCESS far *, CHAR far *, DBSMALLINT);
extern RETCODE   API dbrpcparam(DBPROCESS far *,CHAR far *,BYTE,INT,DBINT,DBINT,BYTE far*);
extern RETCODE   API dbrpcsend(DBPROCESS far *);
extern void   API dbrpwclr(LOGINREC far *);
extern RETCODE   API dbrpwset(LOGINREC far *, CHAR far *, CHAR far *, INT);

extern DBBINARY far * API dbtsnewval(DBPROCESS far *);
extern DBBINARY far * API dbtxptr(DBPROCESS far *,INT);
extern DBBINARY far * API dbtxtimestamp(DBPROCESS far *,INT);
extern DBBINARY far * API dbtxtsnewval(DBPROCESS far *);

/* two-phase commit functions */
extern DBPROCESS far * API open_commit(LOGINREC far *,CHAR far *);
extern void        API close_commit(DBPROCESS far *);
extern void        API build_xact_string(CHAR far *,CHAR far *,DBINT,CHAR far *);
extern DBINT          API start_xact(DBPROCESS far *,CHAR far *,CHAR far *,INT);
extern RETCODE        API commit_xact(DBPROCESS far *,DBINT);
extern RETCODE        API abort_xact(DBPROCESS far *,DBINT);
extern RETCODE        API remove_xact(DBPROCESS far *,DBINT,INT);
extern RETCODE        API scan_xact(DBPROCESS far *,DBINT);
extern INT         API stat_xact(DBPROCESS far *,DBINT);

/* bulk copy prototypes */
extern DBINT   API bcp_batch(DBPROCESS far *);
extern RETCODE API bcp_bind(DBPROCESS far *, BYTE far *, INT, DBINT, BYTE far *, INT, INT, INT);
extern RETCODE API bcp_colfmt(DBPROCESS far *, INT, BYTE, INT, DBINT, BYTE far *, INT, INT);
extern RETCODE API bcp_collen(DBPROCESS far *, DBINT, INT);
extern RETCODE API bcp_colptr(DBPROCESS far *, BYTE far *, INT);
extern RETCODE API bcp_columns(DBPROCESS far *, INT);
extern RETCODE API bcp_control(DBPROCESS far *, INT, DBINT);
extern DBINT   API bcp_done(DBPROCESS far *);
extern RETCODE API bcp_exec(DBPROCESS far *, DBINT far *);
extern RETCODE API bcp_init(DBPROCESS far *, CHAR far *, CHAR far *, CHAR far *, INT);
extern RETCODE API bcp_moretext(DBPROCESS far *, DBINT, BYTE far *);
extern RETCODE API bcp_sendrow(DBPROCESS far *);
extern RETCODE API bcp_setl(LOGINREC far *, DBBOOL);

/* Functions for referencing the dbproc */
RETCODE API dbcmdrow(DBPROCESS far *);
DBINT API dbcount(DBPROCESS far *);
INT   API dbcurcmd(DBPROCESS far *);
DBINT API dbcurrow(DBPROCESS far *);
BOOL  API dbdead(DBPROCESS far *);
DBINT API dbfirstrow(DBPROCESS far *);
BOOL  API dbisavail(DBPROCESS far *);
DBINT API dblastrow(DBPROCESS far *);
RETCODE API dbmorecmds(DBPROCESS far *);
INT   API dbnumorders(DBPROCESS far *);
RETCODE API dbrows(DBPROCESS far *);
STATUS   API dbrowtype(DBPROCESS far *);
#endif

#ifdef DBMSWIN

/* Dblib function prototypes */
extern LOGINREC *  API dblogin(void);
extern DBPROCESS * API dbopen(LOGINREC *,CHAR far *);

FARPROC dberrhandle(FARPROC);
FARPROC dbmsghandle(FARPROC);

extern BYTE far * API dbinit(void);

extern void    API dbwinexit(void);
extern RETCODE API dbsetlname(LOGINREC *,CHAR far *,INT);
extern void    API dbfreelogin(LOGINREC *);
extern RETCODE API dbresults(DBPROCESS *);
extern STATUS  API dbnextrow(DBPROCESS *);
extern void    API dbclose(DBPROCESS *);
extern RETCODE API dbcmd(DBPROCESS *,CHAR far *);
extern RETCODE API dbfcmd(DBPROCESS *, CHAR far *, ...);
extern INT   API dbstrlen(DBPROCESS *);
extern RETCODE API dbstrcpy(DBPROCESS *,INT,INT,CHAR far *);
extern RETCODE API dbsqlexec(DBPROCESS *);
extern RETCODE API dbsqlok(DBPROCESS *);
extern RETCODE API dbsqlsend(DBPROCESS *);
extern RETCODE API dbcanquery(DBPROCESS *);
extern STATUS  API dbgetrow(DBPROCESS *,DBINT);
extern void    API dbclrbuf(DBPROCESS *,DBINT);
extern void    API dbfreebuf(DBPROCESS *);
extern RETCODE API dbsettime(INT);
extern RETCODE API dbsetlogintime(INT);
extern BOOL    API dbdataready(DBPROCESS *);
extern RETCODE API dbcancel(DBPROCESS *);
extern void    API dbexit(void);
extern RETCODE API dbsetopt(DBPROCESS *,INT,CHAR far *);
extern BOOL    API dbisopt(DBPROCESS *, INT, CHAR far *);
extern RETCODE API dbclropt(DBPROCESS *,INT,CHAR far *);
extern INT   API dbordercol(DBPROCESS *,INT);
extern INT   API dbconvert(DBPROCESS *,INT,BYTE far *,DBINT,INT,BYTE far *,DBINT);
extern DBINT   API dbcollen(DBPROCESS *,INT);
extern INT   API dbcoltype(DBPROCESS *,INT);
extern DBINT   API dbdatlen(DBPROCESS *,INT);
extern INT   API dbnumcols(DBPROCESS *);
extern BOOL    API dbcolbrowse(DBPROCESS *,INT);
extern BOOL    API dbtabbrowse(DBPROCESS *,INT);
extern INT   API dbtabcount(DBPROCESS *);
extern INT   API dbtsnewlen(DBPROCESS *);
extern RETCODE API dbtsput(DBPROCESS *,DBBINARY far *,INT,INT,CHAR far *);
extern void    API dbfreequal(CHAR far *);
extern void    API dbprhead(DBPROCESS *);
extern RETCODE API dbprrow(DBPROCESS *);
extern RETCODE API dbtxtsput(DBPROCESS *,DBBINARY far *,INT);
extern RETCODE API dbwritetext(DBPROCESS *,CHAR far *,DBBINARY far *,DBTINYINT, DBBINARY far *,DBBOOL,DBINT,BYTE far *);
extern RETCODE API dbmoretext(DBPROCESS *,DBINT,BYTE far *);
extern RETCODE API dbuse(DBPROCESS *,CHAR far *);
extern RETCODE API dbbind(DBPROCESS *,INT,INT,DBINT,BYTE far *);
extern BOOL    API dbwillconvert(INT,INT);
extern RETCODE API dbsetnull(DBPROCESS *, INT, INT, BYTE far *);
extern RETCODE API dbaltbind(DBPROCESS *,INT,INT,INT,DBINT,BYTE far *);
extern DBINT   API dbaltlen(DBPROCESS *,INT,INT);
extern INT   API dbalttype(DBPROCESS *,INT,INT);
extern DBINT   API dbadlen(DBPROCESS *,INT,INT);
extern INT   API dbnumalts(DBPROCESS *,INT);
extern INT   API dbaltcolid(DBPROCESS *,INT,INT);
extern INT   API dbaltop(DBPROCESS *,INT,INT);
extern INT   API dbnumcompute(DBPROCESS *);
extern INT   API dbgetoff(DBPROCESS *,DBUSMALLINT,INT);
extern DBINT   API dbreadpage(DBPROCESS *,CHAR far *,DBINT,BYTE far *);
extern RETCODE API dbwritepage(DBPROCESS *,CHAR far *,DBINT,DBINT,BYTE far *);
extern DBBOOL  API dbhasretstat(DBPROCESS *);
extern INT   API dbnumrets(DBPROCESS *);
extern DBINT   API dbretlen(DBPROCESS *, INT);
extern DBINT   API dbretstatus(DBPROCESS *);
extern INT   API dbrettype(DBPROCESS *, INT);
extern RETCODE API dbrpcinit(DBPROCESS *, CHAR far *, DBSMALLINT);
extern RETCODE API dbrpcparam(DBPROCESS*,CHAR far *,BYTE,INT,DBINT,DBINT,BYTE far*);
extern RETCODE API dbrpcsend(DBPROCESS *);
extern void    API dbrpwclr(LOGINREC *);
extern RETCODE API dbrpwset(LOGINREC *, CHAR far *, CHAR far *, INT);

extern CHAR far * API dbretname(DBPROCESS *, INT);
extern BYTE far * API dbretdata(DBPROCESS *, INT);
extern CHAR far * API dbprtype(INT);
extern CHAR far * API dbchange(DBPROCESS *);
extern CHAR far * API dbname(DBPROCESS *);
extern BYTE far * API dbbylist(DBPROCESS *,INT,INT far *);
extern BYTE far * API dbadata(DBPROCESS *,INT,INT);
extern CHAR far * API dbqual(DBPROCESS *,INT,CHAR far *);
extern CHAR far * API dbtabsource(DBPROCESS *, INT, INT far *);
extern CHAR far * API dbtabname(DBPROCESS *,INT);
extern CHAR far * API dbcolsource(DBPROCESS *,INT);
extern BYTE far * API dbdata(DBPROCESS *,INT);
extern BYTE far * API dbcolname(DBPROCESS *,INT);
extern CHAR far * API dbgetchar(DBPROCESS *, INT);

extern DBBINARY far * API dbtsnewval(DBPROCESS *);
extern DBBINARY far * API dbtxptr(DBPROCESS *,INT);
extern DBBINARY far * API dbtxtimestamp(DBPROCESS *,INT);
extern DBBINARY far * API dbtxtsnewval(DBPROCESS *);

/* two-phase commit functions */
extern DBPROCESS * API open_commit(LOGINREC *,CHAR far *);
extern void    API close_commit(DBPROCESS *);
extern void    API build_xact_string(CHAR far *,CHAR far *,DBINT,CHAR far *);
extern DBINT      API start_xact(DBPROCESS *,CHAR far *,CHAR far *,INT);
extern RETCODE    API commit_xact(DBPROCESS *,DBINT);
extern RETCODE    API abort_xact(DBPROCESS *,DBINT);
extern RETCODE    API remove_xact(DBPROCESS *,DBINT,INT);
extern RETCODE    API scan_xact(DBPROCESS *,DBINT);
extern INT     API stat_xact(DBPROCESS *,DBINT);

/* bulk copy prototypes */
extern DBINT   API bcp_batch(DBPROCESS *);
extern RETCODE API bcp_bind(DBPROCESS *, BYTE far *, INT, DBINT, BYTE far *, INT, INT, INT);
extern RETCODE API bcp_colfmt(DBPROCESS *, INT, BYTE, INT, DBINT, BYTE far *, INT, INT);
extern RETCODE API bcp_collen(DBPROCESS *, DBINT, INT);
extern RETCODE API bcp_colptr(DBPROCESS *, BYTE far *, INT);
extern RETCODE API bcp_columns(DBPROCESS *, INT);
extern RETCODE API bcp_control(DBPROCESS *, INT, DBINT);
extern DBINT   API bcp_done(DBPROCESS *);
extern RETCODE API bcp_exec(DBPROCESS *, DBINT far *);
extern RETCODE API bcp_init(DBPROCESS *, CHAR far *, CHAR far *, CHAR far *, INT);
extern RETCODE API bcp_moretext(DBPROCESS *, DBINT, BYTE far *);
extern RETCODE API bcp_sendrow(DBPROCESS *);
extern RETCODE API bcp_setl(LOGINREC *, DBBOOL);

/* Functions for referencing the dbproc */
RETCODE API dbcmdrow(DBPROCESS *);
DBINT API dbcount(DBPROCESS *);
INT   API dbcurcmd(DBPROCESS *);
DBINT API dbcurrow(DBPROCESS *);
BOOL  API dbdead(DBPROCESS *);
DBINT API dbfirstrow(DBPROCESS *);
BOOL  API dbisavail(DBPROCESS *);
DBINT API dblastrow(DBPROCESS *);
void  API dblocklib( void );
RETCODE API dbmorecmds(DBPROCESS *);
INT   API dbnumorders(DBPROCESS *);
RETCODE API dbrows(DBPROCESS *);
STATUS   API dbrowtype(DBPROCESS *);
void  API dbunlocklib( void );

/* Functions for referencing global variables */
USHORT   API dbgettime();

/* Function to return the absolute address of the dbproc */
//DBPROCESS far * API dbabsolute(DBPROCESS *);

#endif   /* #ifdef DBMSWIN */

