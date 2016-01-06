/* Copyright (c) Gupta Corporation 1984-97. All Rights Reserved. */
#define   FETEEOF   1       /* End of Fetch                */
#define   FETEUPD   2       /* Row has been updated              */
#define   FETENF    3       /* Row has not been found            */
#define   FETECOC   4       /* INTERNAL USE ONLY -- Row was updated by CURRENT OF cursor */
#define   SQLETNF   101     /* Translation error not found          */
#define   SQLESTS   102     /* Select buffer <number> is too small        */
#define   SQLEDNC   103     /* OS/2 Operation did not cancel           */
#define   SQLEDNN   104     /* Attempt to fetch non-numeric data into numeric column <number> */
#define   SQLENOF   105     /* Attempt to fetch into numeric column <number> that is too small */
#define   SQLEDTN   106     /* Application Programming Error: Data type not supported */
#define   SQLELNE   107     /* Application Programming Error: Long operation not ended */
#define   SQLEAFE   108     /* Row already fetched before enabling fetch backward */
#define   SQLENOP   109     /* MAIN database not open to the public       */
#define   SQLELNS   110     /* Long columns cannot be accessed from this database */
#define   SQLEAPR   111     /* SQL Application Programming Interface (API) recursively entered */
#define   SQLELBS   112     /* Application Programming Error: Long bind already exists */
#define   SQLETMB   113     /* Application Programming Error: Too many binds */
#define   SQLEIVC   114     /* Application Programming Error: Invalid bind name or bind number */
#define   SQLEPDT   115     /* Invalid program data type <pdt> for select buffer #<number> */
#define   SQLEMLB   116     /* Programming Error: Missing long bind before write long */
#define   SQLEIPB   117     /* Invalid (null) parameter buffer pointer       */
#define   SQLEMSC   118     /* Must follow a successful compile of a SELECT statement */
#define   SQLEDND   119     /* Fetched data for column <number> is not in DATE-like or TIME-like format */
#define   SQLEIEV   120     /* Found incorrect version of ERROR.SQL file     */
#define   SQLEIVD   121     /* Attempt to insert or update with an invalid date */
#define   SQLEMVC   122     /* Number of SELECT columns & number of INSERT columns do not match */
#define   SQLECEX   123     /* CURRENT OF cursor not allowed with sqlcex API function call */
#define   SQLECBR   124     /* Out of memory at the client workstation (SQL CBR) */
#define   SQLENTL   125     /* Name parameter is too long           */
#define   SQLENCC   126     /* Named cursor does not exist or is not open    */
#define   SQLENCR   127     /* Cursor identified in the SQL command is not positioned on a row */
#define   SQLECAE   128     /* Cursor name already exists or is already open */
#define   SQLECAI   129     /* Out of memory at the client workstation (SQL CAI) */
#define   SQLEDNT   130     /* Database/user/password is too large        */
#define   SQLEOMS   131     /* Output message size is too small        */
#define   SQLEIMS   132     /* Input message size is too small         */
#define   SQLEIVE   133     /* Invalid error return code         */
#define   SQLECNO   134     /* Could not open file 'error.sql'         */
#define   SQLEICN   135     /* Invalid cursor number             */
#define   SQLECCS   136     /* Out of memory at the client workstation (SQL CCS) */
#define   SQLENFC   137     /* No row fetched for current of cursor       */
#define   SQLEIPI   138     /* Application Programming Error: Invalid picture specified */
#define   SQLEOSP   139     /* Application Programming Error: Output buffer too small */
#define   SQLEDNI   140     /* Database system not installed           */
#define   SQLECSS   141     /* Out of memory at the client workstation (SQL CSS) */
#define   SQLECBS   142     /* Out of memory at the client workstation (SQL CBS) */
#define   SQLEIMT   143     /* Invalid error message type specified       */
#define   SQLEIRN   144     /* Invalid row number             */
#define   SQLEOOM   145     /* Out of memory at the client workstation (SQL OOM) */
#define   SQLEEMS   146     /* Input message buffer set too small      */
#define   SQLEELE   147     /* Got error looking up error message      */
#define   SQLEDNO   148     /* Specified database not on specified database server */
#define   SQLEBNE   149     /* Online backup not properly ended        */
#define   SQLERNE   150     /* Restore not properly ended           */
#define   SQLEMTS   151     /* Cannot set input message buffer smaller than current message length */
#define   SQLEDAE   152     /* Database already exists              */
#define   SQLERAA   153     /* Result set processing already active       */
#define   SQLERSM   154     /* Result set processing must already be active  */
#define   SQLECIA   155     /* Named cursor must be performing a SELECT      */
#define   SQLECTC   156     /* Command does not support CURRENT OF or ADJUSTING CURSOR */
#define   SQLENRM   157     /* Named cursor is not in result set mode        */
#define   SQLENBS   158     /* Application Programming Error: Database names buffer too small */
#define   SQLEFNS   159     /* Fetch backwards not allowed with result sets  */
#define   SQLECSB   160     /* Out of memory at the client workstation (SQL CSB) */
#define   SQLEACL   161     /* Out of memory at the client workstation (SQL ACL) */
#define   SQLEIDN   162     /* Invalid database name             */
#define   SQLERNA   163     /* Result sets not active            */
#define   SQLENRS   164     /* No result set               */
#define   SQLE065   165     /* NOT USED                 */
#define   SQLECRS   166     /* Cannot create a front-end result set       */
#define   SQLENET   167     /* Network check error (front end)         */
#define   SQLEFMG   168     /* Error in reading from temporary Front-End Result Set file */
#define   SQLESAV   169     /* Error in writing to temporary Front-End Result Set file */
#define   SQLEDSB   170     /* Requested operation is disabled for foreign DBMS */
#define   SQLE071   171     /* NOT USED                 */
#define   SQLENSP   172     /* API CANcel (sqlcan) is only supported under the OS/2 environment */
#define   SQLE073   173     /* NOT USED                 */
#define   SQLE074   174     /* NOT USED                 */
#define   SQLEOLC   175     /* Cannot open local client workstation file     */
#define   SQLEWRI   176     /* Cannot write to local client workstation file */
#define   SQLEREA   177     /* Cannot read from local client workstation file */
#define   SQLESFW   178     /* Cannot write to remote SQLBase Server file    */
#define   SQLEISI   179     /* NOT USED                 */
#define   SQLEISH   180     /* Invalid database server handle          */
#define   SQLEBFE   181     /* Backup destination file already exists        */
#define   SQLECCB   182     /* Cannot create backup file         */
#define   SQLENED   183     /* Not enough disk space to complete Backup or Restore operation */
#define   SQLEISN   184     /* Invalid server name            */
#define   SQLECNB   185     /* Cannot open backup file on local (client) computer */
#define   SQLECAB   186     /* Cannot allocate Backup or Restore memory buffer */
#define   SQLECNC   187     /* Cannot connect to server          */
#define   SQLENV4   188     /* Specified operation is not valid for SQLBase 4.0 or later */
#define   SQLEEAS   189     /* Database already exists on another database server */
#define   SQLEFUP   190     /* CURRENT OF cursor requires SELECT FOR UPDATE  */
#define   SQLEFIP   191     /* CURRENT OF cursor processing cannot be used; fetch in-progress */
#define   SQLELAE   192     /* Log files already exists          */
#define   SQLEFRT   193     /* Local result set directory name string too big */
#define   SQLEMTT   194     /* More than two slashes in connect        */
#define   SQLEISP   195     /* Invalid SET parameter             */
#define   SQLEIGP   196     /* Invalid GET parameter             */
#define   SQLEGNE   197     /* Global cursor does not exists           */
#define   SQLEIPL   198     /* Invalid SET parameter length         */
#define   SQLENBR   199     /* Backend result sets not supported       */
#define   SQLENTT   200     /* No error translation table found        */
#define   DBOENCC   201     /* No compiled command            */
#define   DBOENSE   202     /* Not a SELECT command              */
#define   DBOENEX   203     /* Command has not been executed           */
#define   DBOENST   204     /* Fetch backward not allowed with this query    */
#define   DBOEORD   205     /* ORDER BY with fetch backward         */
#define   DBOERTL   206     /* Row too long for fetch            */
#define   DBOECNF   207     /* Command not found for retrieval         */
#define   DBOECIN   208     /* Stored command has been invalidated        */
#define   DBOESTO   209     /* Command has been stored              */
#define   DBOEIFN   210     /* Invalid long field number         */
#define   DBOENBK   211     /* On-line backup not allowed in read only isolation level */
#define   DBOESDD   212     /* Cannot store a DDL command           */
#define   DBOENTL   213     /* Stored command name <name> is too long        */
#define   DBOENLB   214     /* Not a long bind variable          */
#define   DBOEGBY   215     /* GROUP BY with fetch backward         */
#define   DBOESFN   216     /* Set functions with fetch backward       */
#define   DBOERNA   217     /* Recovery not active            */
#define   DBOESTD   218     /* Stored command <name> does not exist       */
#define   DBOEFBV   219     /* Cannot fetch backward on this view      */
#define   DBOELSK   220     /* Long seek past end of long data         */
#define   DBOECNL   221     /* Current command type is not Load/Unload       */
#define   DBOECSD   224     /* Cannot open start database           */
#define   DBOEIRF   225     /* Initialize read failure              */
#define   DBOEIWF   226     /* Initialize write failure          */
#define   DBOEIIL   227     /* Invalid isolation level <invalid setting>     */
#define   DBOEIDP   228     /* INTERNAL ERROR ONLY - Invalid database parameter type */
#define   DBOESNL   229     /* SELECT must be the last cmd in chained command */
#define   DBOEIFD   230     /* Invalid or non-existent directory name        */
#define   DBOEIVR   231     /* Invalid version                */
#define   DBOEDBA   232     /* Must be a DBA for this operation        */
#define   DBOEILS   233     /* Input message length too small          */
#define   DBOECAI   234     /* Out of memory on the database computer (DBO CAI) */
#define   DBOENBI   235     /* No on behave user information           */
#define   DBOEISO   236     /* Invalid security options          */
#define   DBOEMBR   239     /* Must be in result set mode to turn on restriction mode */
#define   DBOERMA   240     /* Restriction mode already on          */
#define   DBOERAO   241     /* Restriction mode already off         */
#define   DBOERSN   242     /* Result set processing not in effect        */
#define   DBOECIB   243     /* Out of memory on the database computer (DBO CIB) */
#define   DBOEDIU   244     /* Deinstall with other users           */
#define   DBOESDC   245     /* SELECT disallowed in chained commands      */
#define   DBOECDC   246     /* CURRENT OF CURSOR in chained commands      */
#define   DBOEDAR   247     /* DBA authority required for this operation     */
#define   DBOECOF   248     /* Cannot open the remote file <filename> on the SQLBase Server */
#define   DBOESAR   249     /* SYSADM authority required         */
#define   DBOECDF   250     /* Cannot delete the remote file <filename> on the SQLBase Server */
#define   DBOEREA   251     /* Read failure on remote file at the SQLBase Server */
#define   DBOEWRI   252     /* Write failure to a remote file at the SQLBase Server */
#define   DBOECSK   253     /* Seek failure on remote file at the SQLBase Server */
#define   DBOESYS   254     /* System command cannot be retrieved/executed/erased */
#define   DBOEINC   255     /* Invalid cursor number             */
#define   DBOEOOM   256     /* Out of memory at the database computer (DBO OOM) */
#define   DBOECAL   257     /* Out of memory at the database computer (DBO CAL) */
#define   DBOERDO   258     /* Read-only mode is disabled for this database  */
#define   DBOENV4   259     /* Specified operation is not valid for SQLBase 4.0 or later */
#define   DBOEDNS   260     /* Datetime not specified with ROLLFORWARD to specific datetime */
#define   DBOEIDS   261     /* Invalid datetime <value> specified with ROLLFORWARD */
#define   DBOEICM   262     /* INTERNAL USE ONLY - Invalid connect mode      */
#define   DBOELFS   263     /* Transaction Log file size <value> is too small */
#define   DBOEIRM   264     /* Illegal rollforward mode <value>        */
#define   DBOECTI   265     /* Checkpoint time interval of <value> minutes is too long */
#define   DBOERFS   266     /* Get Next Log is only valid after Rollforward has been initiated */
#define   DBOENSD   267     /* Not connect to the same database        */
#define   DBOEHSS   268     /* History file size must be a positive value    */
#define   DBOENSC   269     /* Stored command cannot be found.         */
#define   DBOERFI   270     /* Rollforward interrupted - must restore database backup again */
#define   DBOELNE   271     /* Logging not enabled            */
#define   DBOECAF   272     /* Out of memory on the database computer (DBO CAF) */
#define   DBOECOP   273     /* Database open copy file failure         */
#define   DBOECCR   274     /* Database create copy file failure       */
#define   DBOENPW   275     /* No password can be retrieved for connecting to database. */
#define   DBOEFCS   276     /* Cannot generate connection string       */
#define   DBOELBD   277     /* Log backup disabled            */
#define   DBOELCU   278     /* Long varchar column was updated or deleted    */
#define   DBOESLS   279     /* Transaction span limit too small        */
#define   DBOEITO   280     /* Invalid timeout value             */
#define   DBOEIVS   281     /* Invalid version string            */
#define   DBOERNS   282     /* INTERNAL USE ONLY - Request not supported     */
#define   DBOELFL   283     /* Transaction Log file size <value> is too large */
#define   DBOELBM   284     /* LOGBACKUP must be ON for BACKUP DATABASE; try BACKUP SNAPSHOT */
#define   DBOENSR   285     /* Cannot store SELECT commands or procedures in restriction mode */
#define   DBOECBL   286     /* Next Log# <value> less than the Next Log# <value> that should be backed up */
#define   DBOECBG   287     /* Next Log# <value> greater than current active log # <value> */
#define   DBOEIFE   288     /* Database file size extension too small        */
#define   DBOERSR   289     /* Cannot retrieve SELECT stored command/procedure <user>.<name> in restriction mode */
#define   DBOENDB   290     /* Invalid NEWDB value            */
#define   DBOEBME   291     /* LOGBACKUP mode already enabled          */
#define   DBOEBMD   292     /* LOGBACKUP mode already disabled         */
#define   DBOEFEV   293     /* (Incompatible) client cannot invoke attempted function */
#define   DBOENOL   294     /* RECOVERY is OFF, there is no log        */
#define   DBOENSS   295     /* Not a server session              */
#define   DBOEIVC   296     /* Backend code page is not same, check SQL.INI file and country.sql */
#define   DBOECSC   297     /* Cannot store the command, <command name>      */
#define   DBOECRM   298     /* Cannot restore/rollforward MAIN while partitioned db enabled */
#define   DBOEIOL   299     /* Invalid optimizer level <value>         */
#define   DBOEITV   300     /* Invalid command timeout value           */
#define   EXEENCH   301     /* Can only modify CHAR data type          */
#define   EXEENAV   302     /* ALTER privilege not allowed on views       */
#define   EXEECRI   303     /* Cannot create a VIEW using ROWID column       */
#define   EXEECDT   304     /* Cannot drop another's table          */
#define   EXEECDI   305     /* Cannot drop indexed column <column name>      */
#define   EXEECDS   306     /* Cannot drop system information <column, index, or table name> */
#define   EXEESTO   307     /* Cannot execute after store           */
#define   EXEECIL   308     /* Cannot index long data            */
#define   EXEEMSC   309     /* Cannot modify system column <column name>     */
#define   EXEERSC   310     /* Cannot rename system column <name>      */
#define   EXEERST   311     /* Cannot rename system table <table name>       */
#define   EXEERCD   312     /* Cannot revoke connect from a DBA <user name>  */
#define   EXEERND   313     /* Cannot revoke DBA from a non-DBA <user name>  */
#define   EXEERRD   314     /* Cannot revoke resource from a DBA <user name> */
#define   EXEECAE   315     /* Column <column name> already exists        */
#define   EXEECST   316     /* Column <name> specified twice           */
#define   EXEEDND   317     /* Data is not a date             */
#define   EXEEDNN   318     /* Data is not numeric            */
#define   EXEEIAE   319     /* Index <index name> already exists       */
#define   EXEEIDE   320     /* Index <index name> does not exist       */
#define   EXEEVTL   321     /* Insert/update value is too large        */
#define   EXEEIDV   322     /* Invalid data for this view           */
#define   EXEEIVN   323     /* Invalid number                 */
#define   EXEEIBV   324     /* Invalid program bind variable           */
#define   EXEESSF   325     /* INTERNAL USE ONLY - expected next row but did not find one */
#define   EXEENGA   326     /* No grant authority             */
#define   EXEERSP   327     /* Result set not allowed with procedure      */
#define   EXEENRE   328     /* No resource authority             */
#define   EXEENRA   329     /* No revoke authority            */
#define   EXEENCN   330     /* Not a column                */
#define   EXEENEV   331     /* Not enough non-null values           */
#define   EXEEPEU   332     /* Privileges exist for user <name>        */
#define   EXEESNI   333     /* Size of CHAR not increased           */
#define   EXEERUN   334     /* Command not allowed in SQLBase RUNTIME database engine */
#define   EXEESYT   335     /* Table, view, or synonym, <name> does not exist */
#define   EXEENEP   336     /* No execute privilege on <object> <name> for user <username>. */
#define   EXEETEU   337     /* Table(s) exist for specified user <name>      */
#define   EXEETVS   338     /* Table, view, or synonym <name> already exists */
#define   EXEETMC   339     /* Too many columns               */
#define   EXEEUNE   340     /* User <name> does not exist           */
#define   EXEEVIN   341     /* INSERT/UPDATE value is NULL & target column cannot contain nulls */
#define   EXEEWPS   342     /* INTERNAL USE ONLY - Wait/post           */
#define   EXEEICR   343     /* Invalid ROWID               */
#define   EXEECAS   344     /* Out of memory at the database computer (EXE CAS) */
#define   EXEEIOP   345     /* Invalid old password              */
#define   EXEENVL   346     /* NULL value encountered in column <name>       */
#define   EXEEEFS   347     /* External function <name> already exists as a stored command or stored procedure */
#define   EXEECNA   348     /* Current of cursor not allowed with this query */
#define   EXEECDD   349     /* Current cursor is on a different database     */
#define   EXEEDBA   350     /* DBA authority required for this operation     */
#define   EXEECDA   351     /* Cannot drop all columns              */
#define   EXEEKTL   352     /* Index key size too large          */
#define   EXEENSY   353     /* Object <name> specified in DROP SYNONYM is not a synonym */
#define   EXEENVW   354     /* Object <name> specified in DROP VIEW is not a view */
#define   EXEENTB   355     /* The object <name> specified in DROP TABLE is a view name. */
#define   EXEEMBC   356     /* Function parameters must be constants for index <index name> */
#define   EXEEPTL   357     /* Function parameter too large for column <name> */
#define   EXEENIR   358     /* Numeric date value not in range         */
#define   EXEEOCT   359     /* Table referenced by other cursor        */
#define   EXEERSD   360     /* Cannot revoke DBA from SYSADM           */
#define   EXEEACI   361     /* ADJUSTING CURSOR not allowed with embedded SELECT */
#define   EXEEDHR   362     /* User <user name> does not have RESOURCE authority */
#define   EXEETVC   363     /* Too many view columns defined           */
#define   EXEETSE   364     /* Too many view select expressions        */
#define   EXEEPNO   365     /* Privileges not owned for this user <name>     */
#define   EXEEIIF   366     /* @function not supported as an index        */
#define   EXEESNR   367     /* Small integer value not in range        */
#define   EXEEINR   368     /* Integer value not in range           */
#define   EXEECCD   369     /* Current cursor is on a different table        */
#define   EXEEOOC   370     /* Table can have only one clustered hashed index */
#define   EXEETNE   371     /* Clustered index can only be created on an empty table */
#define   EXEEIPD   372     /* A clustered index was previously defined on this table */
#define   EXEENIV   373     /* INDEX privilege not allowed on views       */
#define   EXEENNE   374     /* NOT NULL column <name> must be added only to an empty table */
#define   EXEEEFE   375     /* External function <name> already exists       */
#define   EXEEBEF   376     /* External function <name> does not exist       */
#define   EXEECUI   377     /* Cannot update statistics of a CLUSTERED HASHED INDEX */
#define   EXEERCN   378     /* ROWCOUNT not supported for read-only transactions */
#define   EXEEBRL   379     /* Invalid ROWID               */
#define   EXEECEF   380     /* CHECK EXISTS failure              */
#define   EXEECAN   381     /* NOT USED                 */
#define   EXEEUFV   382     /* Unmatched foreign key values         */
#define   EXEECDR   383     /* Cannot delete row until all the dependent rows are deleted */
#define   EXEEAMO   384     /* Self-referencing table only allows one row from INSERT... SELECT */
#define   EXEEUPO   385     /* UPDATE involving primary key cannot update more than one row */
#define   EXEECUR   386     /* Cannot update row until all the dependent rows are deleted */
#define   EXEENUI   387     /* A suitable unique index must exist before adding primary key */
#define   EXEEAFU   388     /* Adding foreign key will cause unmatched foreign key values */
#define   EXEEIND   389     /* NOT USED                 */
#define   EXEECCL   390     /* Cannot compare long data          */
#define   EXEEFUP   391     /* Select for update requires RR or CS isolation */
#define   EXEENDR   392     /* No DDL statement in readonly database      */
#define   EXEEUEE   393     /* User error already exist for the specified error class */
#define   EXEEUEN   394     /* User error not defined for the specified error class */
#define   EXEECDK   395     /* Cannot drop primary or foreign key column <column name> */
#define   EXEENUV   396     /* UPDATE privilege not allowed on views      */
#define   EXEECIR   397     /* Cannot index ROWID             */
#define   EXEEISH   398     /* Invalid statistics column for a hashed index  */
#define   EXEEISI   399     /* Invalid statistics columns for a B-Tree index */
#define   EXEERNO   400     /* Cannot revoke non-granted execute privilege from user <name> */
#define   DBAECOD   401     /* Cannot open database              */
#define   DBAEWDB   402     /* Wait for DBA structure            */
#define   DBAEOFF   403     /* Database found; but the file can't be opened  */
#define   DBAEIPW   404     /* Invalid password               */
#define   DBAEIUN   405     /* Invalid user name              */
#define   DBAEDNP   406     /* Escape sequence connect not supported.        */
#define   DBAECAD   407     /* Out of memory at the database computer (DBA CAD) */
#define   DBAEDON   408     /* Database already opened with no recovery      */
#define   DBAEDOR   409     /* Database already opened with recovery      */
#define   DBAEIDF   410     /* Invalid database file             */
#define   DBAEIDB   411     /* Invalid database name             */
#define   DBAEDNA   412     /* Unused - Error DBA COD is used for consistency */
#define   DBAENLS   413     /* NOT USED                 */
#define   DBAERIP   414     /* Connects not allowed until restore is complete */
#define   DBAEREC   415     /* Recovery operation in-progress          */
#define   DBAEOUC   416     /* Operation not allowed while other users are connected */
#define   DBAECCD   417     /* Cannot create database file          */
#define   DBAESDN   418     /* Shutdown in progress              */
#define   DBAEICM   419     /* Incompatible connect mode         */
#define   DBAECRD   420     /* Cannot convert Read-Only database to current version. */
#define   DBAEISC   421     /* Invalid server connect            */
#define   DBAEISL   422     /* Invalid servername length         */
#define   DBAENRS   423     /* Not remote server              */
#define   DBAEUCE   424     /* Unexpected SQLBase condition         */
#define   DBAEROE   425     /* Read-Only mode already enabled          */
#define   DBAEROD   426     /* Read-only mode already disabled         */
#define   DBAECCH   427     /* Could not create history file on database computer */
#define   DBAEIRS   428     /* Invalid parameter setting for READONLY transaction value */
#define   DBAEDNB   429     /* Database name is too big          */
#define   DBAENFS   430     /* No free cursor slot while trying to close database */
#define   DBAECLF   431     /* Clean up log files failed during restore from snapshot */
#define   DBAEISI   432     /* Sort information in database does not match COUNTRY.SQL file */
#define   DBAERDE   433     /* Read-only database already enabled      */
#define   DBAERDD   434     /* Read-only database already disabled        */
#define   DBAETDM   435     /* TEMPDIR must be specified to access read-only databases */
#define   DBAECTF   436     /* Unable to create temporary file         */
#define   DBAEICP   437     /* Code page in database does not match COUNTRY.SQL file */
#define   DBAECCT   438     /* Cannot create temporary file         */
#define   DBAEILW   439     /* Lowercase table in database does not match COUNTRY.SQL file */
#define   DBAEIUP   440     /* Uppercase table in database does not match COUNTRY.SQL file */
#define   DBAENDB   441     /* SQLBase internal database connect failed      */
#define   DBAECOF   442     /* Cannot open database; file handles exceeded   */
#define   DBAECAC   443     /* Out of memory at the database computer (DBA CAC) */
#define   DBAEDUW   444     /* Attempt to disconnect with uncommitted work (DBA DUW) */
#define   DBAEDTP   445     /* Attempt to turn off recovery with pending distributed transactions. */
#define   DBAESDS   446     /* Server shutdown in progress          */
#define   DBAEXDL   447     /* Cannot open database; exclusively locked by another user. */
#define   DBAEIVD   448     /* DBS version (<version>), incompatible with software (<version>) */
#define   DBAEDBN   449     /* Cannot connect to database; number limit exceeded */
#define   DBAEANC   450     /* Attributes not compatible         */
#define   DBAEFRO   451     /* Database file is readonly         */
#define   DBAEUPG   452     /* Cannot convert this database - Database has been shut down */
#define   DBAERAS   453     /* Operation not supported while running as a service */
#define   WSPEISP   501     /* NOT USED                 */
#define   WSPECNA   502     /* Out of memory at the database computer (WSP CNA) */
#define   WSPETMC   503     /* NOT USED                 */
#define   WSPECRP   504     /* Cannot relocate pointer              */
#define   WSPEWLE   505     /* Work space limit exceeded         */
#define   DICEITN   601     /* Table <table name> has not been created       */
#define   DICETAE   602     /* NOT USED                 */
#define   DICEICN   603     /* Invalid column number             */
#define   DICEIIN   604     /* Invalid index number              */
#define   DICESYN   605     /* Synonym <synonym name> does not exist      */
#define   DICEIVN   606     /* Event does not exist              */
#define   DICESTN   607     /* A system table definition in the dictionary could not be found. */
#define   DICESTC   608     /* Security initialization in the external dictionary has failed. */
#define   DICENSO   609     /* <name1> is not a synonym for <name2>       */
#define   DICECVT   610     /* Conversion of internal trigger dictionary failed - Entry not found */
#define   VIOEOCP   701     /* Out of cache pages             */
#define   VIOEPNO   702     /* Fatal SQLBase System Failure (VIO PNO)        */
#define   VIOEIPN   703     /* Fatal SQLBase System Failure (VIO IPN)        */
#define   VIOEDUN   704     /* Crashed with recovery off, database unrecoverable */
#define   VIOENFT   705     /* NOT USED                 */
#define   VIOECPO   706     /* Out of memory at the database computer (VIO CPO) */
#define   VIOETBO   707     /* Out of memory at the database computer (VIO TBO) */
#define   VIOENUR   708     /* No updates with read only isolation        */
#define   VIOEBVC   709     /* Fatal SQLBase System Failure (VIO BVC)        */
#define   VIOEILP   710     /* Fatal SQLBase System Failure (VIO ILP)        */
#define   VIOEMEM   711     /* NOT USED                 */
#define   VIOECAT   712     /* Out of memory at the database computer (VIO CAT) */
#define   VIOECAF   713     /* Out of memory at the database computer (VIO CAF) */
#define   VIOEILT   714     /* Fatal SQLBase System Failure (VIO ILT)        */
#define   VIOERFI   715     /* Rollforward interrupted - must restore database backup again */
#define   VIOEMOR   716     /* NOT USED                 */
#define   VIOETHR   717     /* NOT USED                 */
#define   VIOEBKF   718     /* Restored backup file requires media recovery: use ROLLFORWARD command */
#define   VIOERFS   719     /* Cannot connect until rollforward completed, use ROLLFORWARD END command */
#define   VIOEUCO   720     /* Fatal SQLBase System Failure (VIO UCO)        */
#define   VIOEDMO   721     /* Too many pages allocated for demonstration program */
#define   VIOEMIP   722     /* Fatal SQLBase System Failure (VIO MIP)        */
#define   VIOENRD   723     /* No updates against read only database      */
#define   VIOEPMC   724     /* Fatal SQLBase System Failure (VIO PMC)        */
#define   VIOEITP   725     /* Fatal SQLBase System Failure (VIO ITP)        */
#define   VIOEDNL   726     /* Cannot have distributed transaction with logging disabled. */
#define   VIOECAL   727     /* Out of memory at the database computer (VIO CAT) */
#define   VIOEDTN   728     /* Distributed transaction not supported      */
#define   VIOECPG   729     /* A corrupted page is found (VIO CPG)        */
#define   VIOEBPN   730     /* A bad page number is found (VIO BPN)       */
#define   VIOECNS   731     /* Can not start any more transaction      */
#define   VIOEMZU   732     /* Page being modified has a zero use count (VIO MZU) */
#define   VIOERZU   733     /* Page being released has a zero use count (VIO RZU) */
#define   ROWEEND   801     /* INTERNAL USE ONLY - End of fetch        */
#define   ROWEISA   802     /* Insufficient data page space available to update column(s) */
#define   ROWEUEP   803     /* Fatal SQLBase System Failure (ROW UEP)        */
#define   ROWEKTL   804     /* Key value is too large            */
#define   ROWENUD   805     /* Insert/update of unique constrained columns with duplicate data */
#define   ROWEIRI   806     /* Invalid ROWID               */
#define   ROWEIIC   807     /* Index is corrupt               */
#define   ROWEUFA   808     /* Fatal SQLBase System Failure (ROW UFA)        */
#define   ROWEBPT   809     /* CHECK Failure (ROW BPT): <data page corrupted> */
#define   ROWEBSN   810     /* CHECK Failure (ROW BSN): <data page corrupted> */
#define   ROWEFSB   811     /* CHECK Failure (ROW FSB): <data page corrupted> */
#define   ROWEBSO   812     /* CHECK Failure (ROW BSO): <data page corrupted> */
#define   ROWEBSL   813     /* CHECK Failure (ROW BSL): <data page corrupted> */
#define   ROWEBRH   814     /* CHECK Failure (ROW BRH): <data page corrupted> */
#define   ROWEMRH   815     /* CHECK Failure (ROW MRH): <data page corrupted> */
#define   ROWEURH   816     /* CHECK Failure (ROW URH): <data page corrupted> */
#define   ROWEBIT   817     /* CHECK Failure (ROW BIT): <data page corrupted> */
#define   ROWEBIL   818     /* CHECK Failure (ROW BIL): <data page corrupted> */
#define   ROWEBEM   819     /* CHECK Failure (ROW BEM): <data page corrupted> */
#define   ROWETMI   820     /* CHECK Failure (ROW TMI): <data page corrupted> */
#define   ROWETFI   821     /* CHECK Failure (ROW TFI): <data page corrupted> */
#define   ROWEIOS   822     /* CHECK Failure (ROW IOS): <data page corrupted> */
#define   ROWESDT   823     /* CHECK Failure (ROW SDT): <data page corrupted> */
#define   ROWEFDT   824     /* CHECK Failure (ROW FDT): <data page corrupted> */
#define   ROWEBPL   825     /* CHECK Failure (ROW BPL): <data page corrupted> */
#define   ROWEBEN   826     /* CHECK Failure (ROW BEN): <data page corrupted> */
#define   ROWEBEB   827     /* CHECK Failure (ROW BEB): <data page corrupted> */
#define   ROWEBRC   828     /* CHECK Failure (ROW BRC): <data page corrupted> */
#define   ROWEBRN   829     /* CHECK Failure (ROW BEM): <index page corrupted> */
#define   ROWEUSL   830     /* CHECK Failure (ROW USL): <index page corrupted> */
#define   ROWEBTP   831     /* CHECK Failure (ROW BTP): <data page corrupted> */
#define   ROWEBRP   832     /* CHECK Failure (ROW BRP): <data page corrupted> */
#define   ROWERIF   833     /* Fatal SQLBase System Failure (ROW RIF)        */
#define   ROWECKU   834     /* Cannot UPDATE the key of a CLUSTERED HASHED index */
#define   ROWEMHV   835     /* CHECK Failure (ROW MHV): <data page corrupted> */
#define   ROWETNN   836     /* CLUSTERED HASHED index must be created on a freshly created table */
#define   ROWEIVF   837     /* INTERNAL USE ONLY - Invalid function       */
#define   ROWERTL   838     /* Initial row size is too large           */
#define   ROWECNL   839     /* Fatal SQLBase System Failure (ROW CNL)        */
#define   ROWECND   840     /* Fatal SQLBase System Failure (ROW CND)        */
#define   ROWECNU   841     /* Fatal SQLBase System Failure (ROW CNU)        */
#define   ROWEPPN   842     /* Fatal SQLBase System Failure (ROW PPN)        */
#define   ROWEPEB   843     /* CHECK Failure (ROW PEB): <data page corrupted> */
#define   ROWEBFP   844     /* CHECK Failure (ROW BFP): <data page corrupted> */
#define   ROWEBLP   845     /* CHECK Failure (ROW BLP): <long data page corrupted> */
#define   ROWEBLS   846     /* CHECK Failure (ROW BLS): <long data page corrupted> */
#define   ROWEIRS   847     /* Row size is too big for a temporary table     */
#define   ROWEPRM   848     /* Row being processed for a DELETE/UPDATE was modified by triggered actions. */
#define   PRSECNE   901     /* Command not properly ended           */
#define   PRSEMFC   902     /* Missing FROM clause            */
#define   PRSEICC   903     /* Invalid CREATE command            */
#define   PRSEMLP   904     /* Missing left parenthesis          */
#define   PRSEMRP   905     /* Missing right parenthesis         */
#define   PRSEITN   906     /* Invalid table name             */
#define   PRSEICN   907     /* Invalid column name            */
#define   PRSEICS   908     /* Invalid CHAR or VARCHAR size         */
#define   PRSEINC   909     /* Invalid character              */
#define   PRSEITL   910     /* Identifier too long            */
#define   PRSEMIK   911     /* Missing INTO keyword              */
#define   PRSEMVK   912     /* Missing VALUES keyword            */
#define   PRSENEV   913     /* Number of INSERT values not equal to number of target columns */
#define   PRSEIBV   914     /* Invalid bind variable             */
#define   PRSEIVC   915     /* Invalid constant               */
#define   PRSEQNE   916     /* Quoted string not ended properly        */
#define   PRSEISC   917     /* Invalid SQL statement             */
#define   PRSETMV   918     /* Number of INSERT values exceeds number of target columns */
#define   PRSEQTL   919     /* Quoted string too long            */
#define   PRSEMES   920     /* Missing equal sign             */
#define   PRSEIVO   921     /* Invalid operator               */
#define   PRSEMCP   922     /* Missing comma or parenthesis         */
#define   PRSEMSE   923     /* Missing SET keyword            */
#define   PRSECST   924     /* Column <name> specified more than once        */
#define   PRSEMFK   925     /* Missing FROM keyword              */
#define   PRSEMNK   926     /* Missing NULL keyword              */
#define   PRSEMOK   927     /* Missing ON keyword             */
#define   PRSEMBK   928     /* Missing BY keyword             */
#define   PRSEIDC   929     /* Invalid DROP command              */
#define   PRSEIIN   930     /* Invalid index name             */
#define   PRSEIVD   931     /* Invalid date and/or time          */
#define   PRSETMC   932     /* Too many columns defined          */
#define   PRSEICL   933     /* Invalid CLIENT LIMIT value.          */
#define   PRSEIVN   934     /* Invalid number                 */
#define   PRSECLX   935     /* CLIENT LIMIT exceeds the number of clients allowed by this server */
#define   PRSEIAO   936     /* Invalid ALTER option              */
#define   PRSESFN   937     /* Set function not allowed here           */
#define   PRSEICD   938     /* Invalid CLIENT DBA option         */
#define   PRSEIAS   939     /* Invalid asterisk               */
#define   PRSEITA   940     /* Invalid tablename.*            */
#define   PRSEMAK   941     /* Missing AND keyword            */
#define   PRSEAWC   942     /* Asterisk allowed only with COUNT        */
#define   PRSEMTK   943     /* Missing TO keyword             */
#define   PRSEUST   944     /* Duplicate user name <name> in list      */
#define   PRSEIPW   945     /* Invalid password               */
#define   PRSEIWI   946     /* Invalid WITHOUT INDEXES clause          */
#define   PRSEIUN   947     /* Invalid user name              */
#define   PRSEIPR   948     /* Invalid privilege specified          */
#define   PRSEMID   949     /* Missing IDENTIFIED keyword           */
#define   PRSEMIS   950     /* Missing IS keyword             */
#define   PRSECOM   951     /* Invalid COMMENT command              */
#define   PRSEMDK   952     /* Missing data type keyword         */
#define   PRSEMTN   953     /* Missing table name             */
#define   PRSEGBE   954     /* GROUP BY under EXISTS             */
#define   PRSEMSC   955     /* SELECT clause of a subquery specifies multiple columns */
#define   PRSEISO   956     /* Invalid SYSTEM ONLY clause           */
#define   PRSEIUI   957     /* Invalid UNION               */
#define   PRSESSR   958     /* Subselect required             */
#define   PRSEIWC   959     /* Invalid wait code              */
#define   PRSEMFO   960     /* Missing FOR keyword            */
#define   PRSEISN   961     /* Invalid SYNONYM name              */
#define   PRSEMSL   962     /* Missing SELECT keyword            */
#define   PRSEBNV   963     /* Bind variable not allowed in view       */
#define   PRSEAUD   964     /* AUDIT keyword missing or misspelled        */
#define   PRSETMP   965     /* Number of passwords exceed number of usernames */
#define   PRSEDIN   966     /* Delimited identifier not ended          */
#define   PRSEDIL   967     /* Delimited identifier is too long        */
#define   PRSETMO   968     /* Too many operands              */
#define   PRSENEO   969     /* Not enough operands            */
#define   PRSECCN   970     /* CURRENT OF cursor not allowed           */
#define   PRSEMOF   971     /* Missing OF keyword             */
#define   PRSEICU   972     /* Invalid CURRENT OF cursor name          */
#define   PRSEOCU   973     /* ORDER BY on column not allowed with union     */
#define   PRSEMBR   974     /* Missing bar                 */
#define   PRSEIPE   975     /* Invalid decimal precision         */
#define   PRSEISA   976     /* Invalid decimal scale             */
#define   PRSEMKK   977     /* Missing CHECK keyword             */
#define   PRSEMSK   978     /* Missing AS keyword             */
#define   PRSEPSR   979     /* Plus sign required for outer join       */
#define   PRSEIOJ   980     /* Illegal outer join specification        */
#define   PRSEASC   981     /* Invalid audit string constant           */
#define   PRSECUM   982     /* User names for index and table must be the same */
#define   PRSEFNA   983     /* Function not allowed in create index       */
#define   PRSEMBF   984     /* Must be function on column           */
#define   PRSEMPK   985     /* Missing PRECISION keyword         */
#define   PRSEIFN   986     /* Invalid FLOAT number              */
#define   PRSEIVW   987     /* Invalid view name              */
#define   PRSEPMB   988     /* PCTFREE must be an integer between 0 and 99   */
#define   PRSEIAN   989     /* Invalid audit identifier: <name>        */
#define   PRSEILC   990     /* Invalid LABEL ON stateement          */
#define   PRSEILP   991     /* Invalid LIKE predicate            */
#define   PRSEIRE   992     /* INTERNAL USE ONLY - Invalid repair type       */
#define   PRSESIM   993     /* Missing SAVEPOINT identifier         */
#define   PRSEICQ   994     /* Invalid qualifier(s) for object         */
#define   PRSEBSZ   995     /* Bad SIZE specification            */
#define   PRSESZD   996     /* SIZE allowed only for HASHED indexes       */
#define   PRSESZN   997     /* SIZE clause required for CLUSTERED HASHED indexes */
#define   PRSEONA   998     /* ASC/DESC not allowed in creation of HASHED CLUSTERED index */
#define   PRSENCH   999     /* A HASHED index must be CLUSTERED        */
#define   PRSEMOP   1000    /* Missing OPTION keyword            */
#define   EDTENSG   1001    /* Nested set function with no GROUP BY       */
#define   EDTENGC   1002    /* Not a GROUP BY column             */
#define   EDTENSS   1003    /* Not a single value set function         */
#define   EDTEICN   1004    /* Integer in ORDER BY/GROUP BY does not identify a valid column */
#define   EDTEVCU   1005    /* Non-updatable view column <column name>       */
#define   EDTEMNE   1006    /* Operands of a UNION do not have the same number of columns */
#define   EDTEMTE   1007    /* Mismatch in types of SELECT expressions       */
#define   EDTELNA   1008    /* Long not allowed here             */
#define   EDTEDKL   1009    /* DISTINCT key is too long          */
#define   EDTEMTF   1010    /* Object table/view <name> of INSERT/UPDATE/DELETE also in FROM clause */
#define   EDTETMS   1011    /* Too many items are being selected       */
#define   SECEUAE   1101    /* User already exists            */
#define   SECESVI   1102    /* Security violation attempting to access <object name> */
#define   SECENGA   1103    /* No grant authority             */
#define   SECEUNE   1104    /* User <name> does not exist           */
#define   SECEUAD   1105    /* User <name> already has DBA authority      */
#define   SECEUAR   1106    /* User <name> already has resource authority    */
#define   SECEDCU   1107    /* DBA authority required to create table/view for another user */
#define   SECEIOP   1108    /* Invalid old password              */
#define   SECETGR   1109    /* Trigger security violation           */
#define   SECEVNT   1110    /* Event security violation          */
#define   SECEMSO   1111    /* Stored object <creator>.<name> cannot be found */
#define   SECESTO   1112    /* System command or procedure <creator>.<name> security violation */
#define   SECEXDL   1113    /* Lock Database security violation        */
#define   SECEEXF   1114    /* External Function security violation       */
#define   APIEICS   1201    /* Invalid column <number> specified       */
#define   APIEMOO   1202    /* Fatal SQLBase System Failure (API MOO) - Message Output Overflow */
#define   APIECSM   1203    /* Out of memory at the client (API CSM)      */
#define   APIECEX   1204    /* Bind variables not allowed with sqlcex API function call */
#define   APIEITL   1205    /* Bind Variable Name <name> is too long      */
#define   APIEIRW   1206    /* NOT USED                 */
#define   APIEIID   1207    /* Invalid or missing identifier           */
#define   APIEWFO   1208    /* WaitForSingleObject failed.          */
#define   APIEBDR   1209    /* Backup directory must be non-null rooted pathname */
#define   APIEDBN   1210    /* Database name must be specified         */
#define   APIEBIF   1211    /* INTERNAL USE ONLY - Bulk insert buffer full   */
#define   APIEIDV   1212    /* Invalid database version          */
#define   APIEBAC   1213    /* Bulk execute mode and autocommit mode cannot be both ON */
#define   APIECMF   1214    /* CreateMutex failed.            */
#define   APIETIO   1215    /* Timeout waiting for connection handle.        */
#define   APIERMF   1216    /* ReleaseMutex failed.              */
#define   MFEEIMO   1301    /* Client input message buffer overflow (MFE IMO) */
#define   MFEEIVC   1302    /* Invalid cursor number             */
#define   MFEENCO   1303    /* Not connected               */
#define   MFEESCR   1304    /* Server connection required           */
#define   MFEEDCR   1305    /* Database connection required         */
#define   MFEECAM   1306    /* Out of memory at the client (MFE CAM)      */
#define   MFEEIFC   1307    /* Invalid function code             */
#define   MFEEOLS   1308    /* Fatal SQLBase System Failure (MFE OLS)        */
#define   MFEECAO   1309    /* Out of memory at the client (MFE CAO)      */
#define   MFEERWF   1310    /* INTERNAL USE ONLY - Row won't fit in message buffer */
#define   MFEECAN   1311    /* Transaction cancelled             */
#define   MFEETRM   1312    /* Transaction terminated            */
#define   MFEEOOM   1313    /* Out of memory at the client (MFE OOM)      */
#define   MFEEBLK   1314    /* Bulk insert error              */
#define   MFEEDBD   1315    /* Database has been shut down (please disconnect) */
#define   MFEENET   1316    /* Network check error (back end)          */
#define   DLUEICN   1401    /* Invalid column name <column name>       */
#define   DLUEACR   1402    /* Ambiguous column <column name> reference      */
#define   DLUEITN   1403    /* Invalid table name <table name>         */
#define   DLUEMNV   1404    /* Mismatch in number of values         */
#define   DLUERWV   1405    /* NOT USED                 */
#define   DLUEVNA   1406    /* View <view name> not allowed here       */
#define   DLUECUV   1407    /* Cannot update view <view name>          */
#define   DLUERNA   1408    /* NOT USED                 */
#define   DLUEARV   1409    /* Ambiguous use of ROWID with a view      */
#define   DLUEOOJ   1410    /* Only one outer join table allowed       */
#define   DLUEICF   1411    /* Insert columns must contain all the foreign key columns */
#define   DLUEICP   1412    /* Insert columns must contain all the primary key columns */
#define   DLUEPTI   1413    /* Parent table <username>.<tablename> in incomplete state */
#define   DLUEDCS   1414    /* Invalid DELETE with WHERE CURRENT OF       */
#define   DLUEDMR   1415    /* Dependent table in subquery must have RESTRICT delete rule */
#define   DLUEUCP   1416    /* Invalid UPDATE with WHERE CURRENT OF       */
#define   DLUETIC   1417    /* Table <tablename> in incomplete state      */
#define   DLUETAP   1418    /* Table <tablename> already has a primary key   */
#define   DLUEPCN   1419    /* Primary key column must be NOT NULL or NOT NULL WITH DEFAULT */
#define   DLUETCK   1420    /* INTERNAL USE ONLY - Table in check pending state */
#define   DLUEFND   1421    /* Foreign key not found in dictionary        */
#define   DLUEPND   1422    /* Primary key not defined yet          */
#define   DLUECPS   1423    /* Cannot add primary key to system table <tablename> */
#define   DLUECFS   1424    /* Cannot add foreign key to system table <tablename> */
#define   DLUEMSC   1425    /* Must be single column subselect         */
#define   DLUENUS   1426    /* No user error for system table <tablename>    */
#define   DLUERIV   1427    /* ROWID not allowed on this view <username>.<viewname> */
#define   DLUEDIC   1428    /* dependent table <tablename> in incomplete state */
#define   DLUENUC   1429    /* Update column does not exist in subject table */
#define   DLUENPC   1430    /* Column specified as parameter does not exist in subject table */
#define   DLUETMU   1431    /* Update column does not exist in subject table */
#define   DLUEDTC   1432    /* dependent table in incomplete state        */
#define   DLUEIDE   1433    /* Index does not exist              */
#define   DLUECMM   1434    /* Stored-command does not exist.          */
#define   DLUEUNT   1435    /* Cannot unload non-existent table        */
#define   DLUESEC   1436    /* Cannot unload inaccessible table        */
#define   DLUENUP   1437    /* Column <view name>.<column name> is not updatable */
#define   DLUEFNF   1438    /* Function <name> has not been created.      */
#define   DLUESTN   1439    /* Cannot define trigger on system table.        */
#define   DLUEEFE   1440    /* External function or synonym <name> already exists */
#define   DLUESPE   1441    /* Procedure or procedure synonym <name> already exists */
#define   SRTECCR   1501    /* Cannot create sort file              */
#define   SRTEQST   1502    /* Fatal SQLBase System Failure (SRT QST)        */
#define   SRTEWTS   1503    /* Out of memory at the database computer (SRT WTS) */
#define   SRTEEOF   1504    /* Fatal SQLBase System Failure (SRT EOF)        */
#define   SRTERWF   1505    /* Disk Write Failure (SRT RWF)         */
#define   SRTECSS   1506    /* Out of memory at the database computer (SRT CSS) */
#define   TYPEIDT   1601    /* Invalid data type              */
#define   TYPEMBB   1602    /* Long must be set to bind variable       */
#define   TYPEOLO   1603    /* Cannot order long data            */
#define   TYPECUR   1604    /* Cannot update or insert a ROWID column        */
#define   TYPECMS   1605    /* Cannot modify system data in system table <name> */
#define   TYPEIDA   1606    /* Invalid date arithmetic              */
#define   TYPEIET   1607    /* Invalid external data type           */
#define   TYPEIIS   1608    /* Date/time interval only expressions not allowed in select list */
#define   TYPEINC   1609    /* Date/time interval comparisons are not allowed */
#define   TYPECCL   1610    /* Cannot compare Long VARCHAR data        */
#define   TYPEMBC   1611    /* Must be a column to perform this function     */
#define   TYPEDNC   1612    /* Must be character data type to perform this function */
#define   TYPENVL   1613    /* @NULLVALUE column must be replaced by same data type value */
#define   TYPETFP   1614    /* Too many procedure function parameters        */
#define   TYPEPNN   1615    /* Number of parameters does not match        */
#define   TYPEMVC   1616    /* Must be a variable or a constant        */
#define   TYPETPO   1617    /* Output parameter in procedure cannot be supported */
#define   TYPETPT   1618    /* Invalid parameter type for procedure/external function */
#define   TYPETPC   1619    /* Too few/many parameters for procedure      */
#define   TYPEMPR   1620    /* Specified procedure does not exist      */
#define   TYPEVSC   1621    /* Stored command not allowed           */
#define   TYPEUFC   1622    /* Undefined procedure function call       */
#define   TYPEBOO   1623    /* Invalid data type (boolean expected)       */
#define   TYPENUM   1624    /* Invalid data type (number expected)        */
#define   TYPESTR   1625    /* Invalid data type (string expected)        */
#define   TYPEDTM   1626    /* Invalid data type (date/time expected)        */
#define   TYPELON   1627    /* Invalid data type (long expected)       */
#define   TYPEBBO   1628    /* Invalid boolean type detected           */
#define   TYPEBNU   1629    /* Invalid number type detected         */
#define   TYPEBST   1630    /* Invalid string type detected         */
#define   TYPEBDT   1631    /* Invalid date/time type detected         */
#define   TYPEBLO   1632    /* Invalid long type detected           */
#define   TYPEMBS   1633    /* Triggerred stored procedure <procedure name> must be static. */
#define   TYPEDNA   1634    /* Delete triggers can not use receive parameters. */
#define   TYPEATR   1635    /* AFTER triggers can not use receive parameters. */
#define   TYPEMBV   1636    /* Only variables can be passed as receive parameters. */
#define   TYPEFNS   1637    /* External function support not available in this SqlBase version. */
#define   TYPEPMC   1638    /* Parameter must be a column type.        */
#define   TYPEONA   1639    /* Old column values cannot be modified by a trigger. */
#define   GPIEINF   1701    /* Invalid function call             */
#define   GPIEDNI   1702    /* NOT USED                 */
#define   GPIEIPL   1703    /* Incorrect parameter length           */
#define   GPIEIDT   1704    /* Invalid dispatch table            */
#define   GPIEIPM   1705    /* Invalid parameter              */
#define   GPIEOUD   1706    /* Out of memory at the client (GPI OUD)      */
#define   GPIERMD   1707    /* Real mode database             */
#define   GPIEOOM   1708    /* Out of memory at the client (GPI OOM)      */
#define   LKMEDLK   1801    /* Application deadlock              */
#define   LKMEILE   1802    /* Insufficient lock entries         */
#define   LKMEITC   1803    /* Fatal SQLBase System Failure (LKM ITC)        */
#define   LKMEILC   1804    /* Fatal SQLBase System Failure (LKM ILC)        */
#define   LKMETMO   1805    /* Time out                 */
#define   LKMEOLE   1806    /* Fatal SQLBase System Failure (LKM OLE)        */
#define   LKMETMP   1807    /* Fatal SQLBase System Failure (LKM TMP)        */
#define   LKMECAL   1808    /* Out of memory at the database computer (LKM CAL) */
#define   LKMELPB   1809    /* Out of memory at the database computer (LKM LPB) */
#define   LKMEWST   1810    /* NOT USED                 */
#define   LKMEFDU   1811    /* Fatal SQLBase System Failure (LKM FDU)        */
#define   LKMEIWS   1812    /* Out of memory at the database computer (LKM IWS) */
#define   LKMEUND   1813    /* Lock held by undecided transaction.        */
#define   LKMEROD   1814    /* NOT USED                 */
#define   LKMENXL   1815    /* The database is not exclusively locked.       */
#define   LKMEIXL   1816    /* The database is already exclusively locked.   */
#define   LKMEISL   1817    /* Database could not be locked because other connections exist. */
#define   LKMEFDX   1818    /* Fatal SQLBase System Failure (LKM FDX)        */
#define   SYSEICV   1901    /* Fatal SQLBase System Failure (SYS ICV)        */
#define   SYSESNA   1902    /* Fatal SQLBase System Failure (SYS SNA)        */
#define   SYSEEXC   1903    /* Fatal SQLBase System Failure (SYS EXC)        */
#define   SYSECGD   1904    /* Fatal SQLBase System Failure (SYS CGD)        */
#define   SYSECSD   1905    /* Fatal SQLBase System Failure (SYS CSD)        */
#define   SYSEMBP   1906    /* Fatal SQLBase System Failure (SYS MBP)        */
#define   SYSEMLM   1907    /* Fatal SQLBase System Failure (SYS MBP)        */
#define   SYSENMF   1908    /* No more files               */
#define   SYSEPNF   1909    /* Path not found or name exceed maximum range   */
#define   SYSEOCN   1910    /* Unable to obtain local computer name from the registry */
#define   SYSESCN   1911    /* Unable to set local computer name into the registry */
#define   SYSEACC   1912    /* Access violation on opening file        */
#define   SYSEBOM   1913    /* INTERNAL USE ONLY - System bombed!      */
#define   SYSEURF   1914    /* INTERNAL USE ONLY - Unused symbols reference check */
#define   SYSEOOM   1915    /* Out of memory when encrypting/decrypting data */
#define   SYSEICF   1916    /* 003 - Fatal SQLBase System Failure (SYS ICF)  */
#define   SYSECAS   1917    /* 008 - Out of memory at the database computer (SYS CAS) */
#define   SYSEFAE   1918    /* Failure already exists            */
#define   SYSEFNE   1919    /* Failure does not exist            */
#define   SYSENDB   1920    /* No DB associated with this cursor       */
#define   SYSEOCO   1921    /* Only 1 crash-count failure can be setup.      */
#define   SYSE000   1922    /* (Unused, original SYSECAD moved to new home dmnerr.h) */
#define   SYSEINP   1923    /* Internal error; Invalid parameters to the routine. */
#define   SYSENWF   1924    /* Local computer name too large for internal buffer */
#define   SYSEAHS   1925    /* Fatal SQLBase System Failure (SYS AHS)        */
#define   SYSEALS   1926    /* Fatal SQLBase System Failure (SYS ALS)        */
#define   SYSEDNL   1927    /* Fatal SQLBase System Failure (SYS DNL)        */
#define   LODEIWS   2001    /* Out of memory at the database computer (LOD IWS) */
#define   LODENAI   2002    /* Asynchronous I/O is not available       */
#define   LODEATL   2003    /* Only single characters allowed in this section */
#define   LODECFC   2004    /* Cannot find COUNTRY.SQL file         */
#define   LODEIVC   2005    /* Cannot find specified country in COUNTRY.SQL file */
#define   LODEICD   2006    /* Invalid directive found in COUNTRY.SQL        */
#define   LODEIHX   2007    /* Invalid hexidecimal number in COUNTRY.SQL     */
#define   LODECTB   2008    /* String in COUNTRY.SQL is too large      */
#define   LODENTT   2009    /* Missing second translate string under TRANSLATE section of COUNTRY.SQL */
#define   LODESMM   2010    /* Many to many translations not allowed in COUNTRY.SQL */
#define   LODEUL1   2011    /* Strings in UPPER to LOWER section of COUNTRY.SQL must be one character */
#define   LODEUL2   2012    /* Strings must be in pairs in the UPPER or LOWER section of COUNTRY.SQL */
#define   FILECLO   2101    /* Cannot close file              */
#define   FILEDEL   2102    /* Cannot delete file             */
#define   FILELSK   2103    /* Cannot perform lseek              */
#define   FILERWF   2104    /* Read or write failure             */
#define   FILECWT   2105    /* Cannot set write-through          */
#define   FILEOOM   2106    /* Out of memory at the database computer (FIL OOM) */
#define   FILEAID   2107    /* Asynchronous I/O did not start          */
#define   FILEAIF   2108    /* Asynchronous I/O failure          */
#define   FILEAPF   2109    /* Asynchronous I/O position failure       */
#define   FILECSF   2110    /* Cannot get file descriptor status       */
#define   FILEPSE   2111    /* Partition size error              */
#define   FILEOOS   2112    /* Out of disk space              */
#define   FILESAR   2113    /* Asynchronous short read error           */
#define   FILESAW   2114    /* Asynchronous short write error          */
#define   FILEARE   2115    /* Asynchronous read error              */
#define   FILEAWE   2116    /* Asynchronous write error          */
#define   FILEIFO   2117    /* FIFO file types are not allowed in this context */
#define   FILEBLK   2118    /* Block special files are not allowed in this context */
#define   FILEDIR   2119    /* Directories are not allowed in this context   */
#define   EVAESRN   2201    /* NOT USED                 */
#define   EVAESRM   2202    /* Subselect resulted in multiple rows        */
#define   EVAEDAO   2203    /* Date arithmetic over/under-flow         */
#define   EVAEIVH   2204    /* Invalid hour value             */
#define   EVAEIVM   2205    /* Invalid minute value              */
#define   EVAEIVS   2206    /* Invalid second value              */
#define   EVAEIVD   2207    /* Invalid day value              */
#define   EVAEIVY   2208    /* Invalid year value             */
#define   EVAEIVO   2209    /* Invalid month value            */
#define   EVAECOR   2210    /* @CHAR value out of range          */
#define   EVAEIVL   2211    /* Invalid length                 */
#define   EVAENOF   2212    /* Numeric overflow               */
#define   EVAEISN   2213    /* Invalid @CHOOSE selector number         */
#define   EVAESTL   2214    /* String too long                */
#define   EVAEOBS   2215    /* Function <name> is now obsolete         */
#define   EVAEVNI   2216    /* Value is not an integer              */
#define   EVAENUF   2217    /* Numeric underflow              */
#define   EVAESNG   2218    /* Attempt to find square-root of negative number */
#define   EVAEVIN   2219    /* Value must not be negative           */
#define   EVAENZO   2220    /* Value is not zero or one          */
#define   EVAEENR   2221    /* Trying to evaluate a non-referenced column    */
#define   RSTECOR   2401    /* Cannot open result set            */
#define   RSTECOT   2402    /* NOT USED                 */
#define   RSTECCF   2403    /* NOT USED                 */
#define   RSTEFNX   2404    /* Filter file does not exist           */
#define   RSTERSA   2405    /* Result set already active         */
#define   RSTERSN   2406    /* Result set not active             */
#define   RSTERSI   2407    /* Result set identifier too large         */
#define   RSTESRC   2408    /* NOT USED                 */
#define   RSTEPER   2409    /* NOT USED                 */
#define   RSTEPNA   2410    /* Previous filter not available           */
#define   RSTECRS   2411    /* Out of memory at the database computer (RST CRS) */
#define   RSTETMT   2412    /* Too many tables in result set           */
#define   RSTEQNS   2413    /* Result set restriction mode is not supported for this query */
#define   RSTEOBY   2414    /* Can't use ORDER BY when using the result-set of previous query */
#define   RSTEIRS   2415    /* Result row size is too big for a temporary table */
#define   RSTEPRO   2416    /* Restriction mode is not supported for procedures */
#define   SRVENPA   2501    /* No process available              */
#define   SRVEINE   2502    /* INTERNAL USE ONLY - Internal error (SRV INE)  */
#define   SRVEOOS   2503    /* Server is out of sessions         */
#define   SRVEMXD   2504    /* Maximum number of clients <max number> exceeded */
#define   SRVENAN   2505    /* Installation of duplicate DBNAME        */
#define   SRVEINN   2506    /* Invalid database attempting to be installed   */
#define   SRVECIS   2507    /* Cannot install server name           */
#define   SRVETMO   2508    /* INTERNAL USE ONLY - Timeout          */
#define   SRVEOOM   2509    /* Out of memory at the database computer (SRV OOM) */
#define   SRVEISN   2510    /* Invalid servername             */
#define   SRVEIPW   2511    /* NOT USED                 */
#define   SRVEACD   2512    /* Access denied to remote server configured as local-only */
#define   SRVEIDP   2513    /* Invalid database process number         */
#define   SRVEPDE   2514    /* Process does not exist            */
#define   SRVECCL   2515    /* Cannot create logfile, <filename>       */
#define   SRVEUCA   2516    /* Server cancelled by operator with active connections */
#define   SRVEDNB   2517    /* Database name is too big          */
#define   SRVENLF   2518    /* NOT USED                 */
#define   SRVEIDN   2519    /* Invalid database name             */
#define   SRVEMXU   2520    /* Maximum number of users <max number> exceeded */
#define   SRVENNO   2521    /* NOT USED                 */
#define   SRVEMXC   2522    /* Maximum number of clients <maximum number> exceeded */
#define   SRVEPLV   2523    /* Activity log print level must be between 0 and 4 inclusive */
#define   SRVEDBA   2524    /* This operation requires DBA authority      */
#define   SRVEALG   2525    /* Activity Log File Name is Too Long      */
#define   SRVECID   2526    /* The database name may not be the same as server name. */
#define   SRVEMDN   2527    /* MAIN database name found on network        */
#define   SRVEISP   2528    /* Invalid self database process number       */
#define   SRVESYS   2529    /* Protected system process number         */
#define   SRVECPT   2530    /* Server does not not Connect-Pass-Thru mode    */
#define   SRVEPIA   2531    /* Database process is Aborting or doing Rollback */
#define   SRVECTS   2532    /* Server is running as a Service          */
#define   SRVEXER   2550    /* Backend returned an error; more information available */
#define   SRVEX51   2551    /* SQLBase returned an error; more information available */
#define   SRVEX52   2552    /* DB2 specific error reported; more information available */
#define   SRVEX53   2553    /* IBM OS/2 Database Manager specific error reported; more */
#define   SRVEX54   2554    /* Oracle specific error reported; more information available */
#define   SRVEX55   2555    /* Informix specific error reported; more information available */
#define   SRVEX56   2556    /* Netware SQL specific error reported; more information */
#define   SRVEX57   2557    /* IBM AS/400 SQL/400 specific error reported; more information */
#define   SRVEX58   2558    /* Sybase SQL Server specific error reported; more information */
#define   SRVEX59   2559    /* HP Allbase Server specific error reported; more information */
#define   SRVEX60   2560    /* Teradata specific error reported; more information available */
#define   SRVEX61   2561    /* Rdb specific error reported; more information available */
#define   SRVEX62   2562    /* Tandem specific error reported; more information available */
#define   SRVEX63   2563    /* IBM SQL/DS specific error reported; more information available */
#define   SRVEX64   2564    /* SNI SESAM specific error reported; more information available */
#define   SRVEX65   2565    /* Ingres specific error reported; more information available */
#define   SRVEX66   2566    /* ODBC specific error reported; more information available */
#define   SRVEX67   2567    /* Dbase specific error reported; more information available */
#define   SRVEX68   2568    /* SNI DDB4 specific error reported; more information available */
#define   SRVEX69   2569    /* Fujitsu specific error reported; more information available */
#define   SRVEX70   2570    /* Cincom SUPRA specific error reported; more information */
#define   SRVEX71   2571    /* CCA Model 204 specific error reported; more information */
#define   SRVEX72   2572    /* Apple DAL interface specific error reported; more information available */
#define   SRVEX73   2573    /* Teradata ShareBase specific error reported; more information available */
#define   SRVEX74   2574    /* Informix OnLine specific error reported; more information available */
#define   SRVEX75   2575    /* EDA/SQL specific error reported; more information available */
#define   SRVEX76   2576    /* SNI UDS specific error reported; more information available */
#define   SRVEX77   2577    /* Mimer specific error reported; more information available */
#define   SRVEX79   2579    /* Ingres OpenSQL specific error reported; more information available */
#define   SRVEX80   2580    /* Ingres OpenSQL specific error reported; more information available */
#define   SRVEX81   2581    /* Entire Access specific error reported; more information available */
#define   SRVEX99   2599    /* SQLHost App Services specific error reported; more information available */
#define   RSIENAS   2601    /* no available sessions             */
#define   RSIECAM   2602    /* Out of memory at the database computer (RSI CAM) */
#define   SCDEIST   2701    /* NOT USED                 */
#define   SCDENSA   2702    /* NOT USED                 */
#define   SCDENES   2703    /* Out of memory at the database computer (SCD NES) */
#define   SCDENEM   2704    /* NOT USED                 */
#define   SCDENSD   2705    /* Fatal SQLBase System Failure (SCD NSD)        */
#define   SCDESNI   2706    /* INTERNAL USE ONLY - Scheduler not initialized */
#define   SCDEAIN   2707    /* INTERNAL USE ONLY - Scheduler already initialized */
#define   SCDEOOM   2708    /* Out of memory at the database computer (SCD OOM) */
#define   SCDECFP   2709    /* INTERNAL USE ONLY - Cannot find process       */
#define   SCDEPIU   2710    /* INTERNAL USE ONLY - Process number already in use */
#define   SCDEPIB   2711    /* INTERNAL USE ONLY - Process is blocked        */
#define   SCDENCP   2712    /* INTERNAL USE ONLY - No current process        */
#define   SCDECWQ   2713    /* NOT USED                 */
#define   SCDECRQ   2714    /* NOT USED                 */
#define   SCDECCQ   2715    /* NOT USED                 */
#define   SCDECCT   2716    /* Out of memory at the database computer (SCD CCT) */
#define   SCDECCS   2717    /* NOT USED                 */
#define   SCDEISM   2718    /* NOT USED                 */
#define   SCDECSS   2719    /* NOT USED                 */
#define   SCDESWF   2720    /* NOT USED                 */
#define   SCDESRF   2721    /* NOT USED                 */
#define   SCDESTC   2722    /* INTERNAL USE ONLY - Semaphore type conflict   */
#define   SCDETMO   2723    /* INTERNAL USE ONLY - Timeout occurred       */
#define   SCDEIDV   2724    /* NOT USED                 */
#define   SCDECCM   2725    /* NOT USED                 */
#define   SCDECCP   2726    /* Out of memory at the database computer (SCD CCP) */
#define   SCDECST   2727    /* Cannot create Operating System thread      */
#define   SCDETMD   2728    /* Cannot create thread because thread manager was diabled */
#define   SCDECAB   2729    /* Cannot allocate buffer for asynchronized I/O  */
#define   SCDECAS   2730    /* Cannot allocate semaphore for asynchronized I/O */
#define   SCDECSP   2731    /* Cannot spwan process for asynchronized I/O    */
#define   SCDETMP   2732    /* Too many threads already          */
#define   SCDECAN   2733    /* Out of memory at the database computer (SCD CAN) */
#define   SCDECCN   2734    /* Cannot create native thread. OS Error = <OsError> */
#define   SCDECCH   2735    /* Cannot do 'CloseHandle'. OS Error = <OsError> */
#define   SCDECSE   2736    /* Cannot do 'SetEvent'.  OS Error = <OsError>   */
#define   SCDECRE   2737    /* Cannot do 'ResetEvent'.   OS Error = <OsError> */
#define   SCDEEWF   2738    /* 'WaitForSingleObject' failed.  OS Error = <OsError> */
#define   SCDENSP   2739    /* Operation not supported for this platform.    */
#define   RSWESAC   2801    /* Session already created              */
#define   RSWENEM   2802    /* Out of memory at the database computer (RSW NEM) */
#define   RSWEIEN   2803    /* Invalid entry               */
#define   RSWETNI   2804    /* Task not initialized              */
#define   RSWETDE   2805    /* NOT USED                 */
#define   RSWETAI   2806    /* Task already initialized          */
#define   RSWENAS   2807    /* No available slots             */
#define   RSWESNE   2808    /* Session does not exist            */
#define   RSWEICB   2809    /* Invalid cursor buffer pointer           */
#define   RSWESCT   2810    /* Session closed/terminated         */
#define   RSWECFS   2811    /* Cannot find session            */
#define   RSWEINE   2812    /* Fatal SQLBase System Failure (RSW INE)        */
#define   RSWEOOS   2813    /* Out of sessions                */
#define   RSWECAN   2814    /* Cannot access network             */
#define   PYBECNI   2901    /* Cannot initialize program         */
#define   PYBENEM   2902    /* Not enough memory              */
#define   PYBECOF   2903    /* Cannot open network message file        */
#define   PYBEIVF   2904    /* Invalid network message file         */
#define   PYBEMFC   2905    /* Network message file corrupted          */
#define   PYBEEOF   2906    /* End of file                 */
#define   PYBEPEF   2907    /* Premature end of file             */
#define   PYBENNN   2908    /* No network message file in use          */
#define   PYBECIS   2909    /* Cannot invoke command shell          */
#define   PYBEINE   2910    /* Internal error                 */
#define   PYBENES   2911    /* Switch to non-existent session          */
#define   PYBECOL   2912    /* Cannot open log file              */
#define   PYBELNO   2913    /* Log file not open              */
#define   CFMENLD   3001    /* NOT USED                 */
#define   CFMENRD   3002    /* NOT USED                 */
#define   CFMEICM   3003    /* NOT USED                 */
#define   CFMEOOM   3004    /* Out of memory at the database computer (CFM OOM) */
#define   CFMERBS   3005    /* NOT USED                 */
#define   CFMEIDN   3006    /* Inconsistent database name           */
#define   CFMECOS   3007    /* Cannot open startup database         */
#define   CFMECCD   3008    /* Cannot create database            */
#define   CFMEIRF   3009    /* Initialize read failure              */
#define   CFMEIWF   3010    /* Initialize write failure          */
#define   CFMEDBE   3011    /* Database already exists              */
#define   CFMEDBN   3012    /* Database does not exist              */
#define   CFMECRD   3013    /* Cannot create directory list         */
#define   CFMEOOH   3014    /* Out of handles                 */
#define   CFMEIVH   3015    /* Invalid handle                 */
#define   VTSEINV   3101    /* INTERNAL USE ONLY - Invalid screen id      */
#define   VTSEOOR   3102    /* INTERNAL USE ONLY - Out of range        */
#define   VTSEICT   3103    /* INTERNAL USE ONLY - Invalid cursor type       */
#define   VTSECIN   3104    /* INTERNAL USE ONLY - Cannot initialize      */
#define   VTSECSR   3105    /* INTERNAL USE ONLY - Cannot save or restore screen */
#define   BKPEBFE   3201    /* Backup file already exists           */
#define   BKPECRB   3202    /* File Creation Failure - Cannot create backup file */
#define   BKPEBIP   3203    /* Backup already in progress           */
#define   BKPEUAB   3204    /* Out of memory at the database computer (BKP UAB) */
#define   BKPESFA   3205    /* File seek failure              */
#define   BKPEUSG   3206    /* Unknown backup file segment          */
#define   BKPECAC   3207    /* Out of memory at the database computer (BKP CAC) */
#define   BKPENCF   3208    /* Need control file to backup database > 2GB  (BKP NCF) */
#define   RESECCN   3301    /* Cannot create new partitioned database file   */
#define   RESECOB   3302    /* Cannot open backup file on database server computer */
#define   RESECAC   3303    /* Out of memory at the database computer (RES CAC) */
#define   RESESFA   3304    /* Backup File Seek Failure          */
#define   RESEUAB   3305    /* Out of memory at the database computer (RES UAB) */
#define   RESESIZ   3306    /* Restore over maximum file size (RES SIZ)      */
#define   SFLEBFE   3401    /* CHECK Failure (SFL BFE): <system free list corrupt> */
#define   SFLEOVL   3402    /* CHECK Failure (SFL OVL): <system free list corrupt> */
#define   SFLEENM   3403    /* CHECK Failure (SFL ENM): <system free list corrupt> */
#define   SFLEFPC   3404    /* CHECK Failure (SFL FPC): <system free list corrupt> */
#define   SFLEBSI   3405    /* CHECK Failure (SFL BSI): <system free list corrupt> */
#define   EMPEULP   3501    /* Database or object in the database is corrupt (EMP ULP) */
#define   EMPEBSI   3502    /* CHECK Failure (EMP BSI): <extent page corrupted> */
#define   EMPEBME   3503    /* CHECK Failure (EMP BME): <extent page corrupted> */
#define   EMPEBES   3504    /* CHECK Failure (EMP BES): <extent page corrupted> */
#define   EMPEENM   3505    /* CHECK Failure (EMP ENM): <extent page corrupted> */
#define   EMPEPIM   3506    /* CHECK Failure (EMP PIM): <extent page corrupted> */
#define   EMPEMOB   3507    /* NOT USED                 */
#define   EMPEIEM   3508    /* CHECK Failure (EMP IEM): <extent page corrupted> */
#define   EMPEPNA   3509    /* CHECK Failure (EMP PNA): <extent page corrupted> */
#define   EMPERNA   3510    /* CHECK Failure (EMP RNA): <extent page corrupted> */
#define   DBWENIN   3601    /* DBWINDOWS is not installed           */
#define   DBWECAM   3602    /* Out of memory at the database computer (DBW CAM) */
#define   CFFENCF   3701    /* No configuration file             */
#define   CFFECOC   3702    /* Cannot open configuration file          */
#define   CFFEMCE   3703    /* Missing configuration file entry        */
#define   CFFEFCL   3704    /* File closed                 */
#define   CFFELSK   3705    /* SQL.INI configuration disk file seek error    */
#define   CFFECRD   3706    /* Read failure of SQL.INI configuration file    */
#define   CFFECWT   3707    /* Write failure to SQL.INI configuration file   */
#define   CFFEEOF   3708    /* INTERNAL USE ONLY - End of file         */
#define   CFFENDL   3709    /* No delete line available in the SQL.INI configuration file */
#define   CFFEOOM   3710    /* Out of memory               */
#define   CFFEIAV   3711    /* Invalid APPEND value              */
#define   CFFEICK   3712    /* Invalid SQL.INI configuration parameter       */
#define   CFFEMPV   3713    /* Missing configuration parameter value      */
#define   CFFECFE   3714    /* Error in configuration file.         */
#define   CFFEANM   3715    /* Missing AUDIT name configuration parameter    */
#define   CFFEINI   3716    /* Invalid integer parameter         */
#define   CFFEINB   3717    /* Invalid boolean parameter         */
#define   CFFEINK   3718    /* Invalid parameter keyword         */
#define   CFFEMES   3719    /* Missing equal sign on parameter         */
#define   CFFENOP   3720    /* MAIN db not open to public           */
#define   CFFEIPL   3721    /* Inappropriate server prefix length      */
#define   CFFEICN   3722    /* Invalid CLIENTNAME parameter         */
#define   CFFEEPD   3723    /* Invalid EPDATECHK parameter          */
#define   CFFEIMA   3724    /* Invalid MACHINE parameter         */
#define   CFFEDDB   3725    /* Invalid DEFAULTDATABASE              */
#define   CFFEITF   3726    /* Cannot open error translation file      */
#define   CFFEOME   3727    /* Out of memory for error translation table     */
#define   CFFEAPC   3728    /* Invalid APPCDLC parameter         */
#define   CFFEPEX   3729    /* Cannot open SQL.INI configuration file for read or write */
#define   CFFEPSW   3730    /* Invalid DEFAULTPASSWORD              */
#define   CFFEPWD   3731    /* Invalid PASSWORD               */
#define   CFFETMZ   3732    /* Invalid TIMEZONE               */
#define   CFFEUSR   3733    /* Invalid DEFAULTUSER            */
#define   CFFEOOF   3734    /* Only accept 0 or 1 as parameter         */
#define   CFFEOCS   3735    /* Cannot open file for character sets        */
#define   CFFECST   3736    /* Out of memory for character sets tables       */
#define   CFFEIVC   3737    /* Invalid character in character sets        */
#define   CFFEIRO   3738    /* Invalid recorder option              */
#define   CFFEROS   3739    /* Recorder option not contained in system       */
#define   CFFELTL   3740    /* Line too long in SQL.INI          */
#define   CFFEINP   3741    /* Invalid NETPREFIX or NETPREFIX too long       */
#define   CFFETTL   3742    /* Invalid audit name identifier           */
#define   CFFEAUT   3743    /* Invalid audit type specified         */
#define   CFFEAWN   3744    /* SQL.INI AUDIT parameter without audit name identifier */
#define   CFFEANF   3745    /* SQL.INI AUDIT parameter could not be found    */
#define   CFFEAAE   3746    /* Specified AUDIT already exists in SQL.INI file */
#define   CFFEBSV   3747    /* Invalid SIZE value specified in AUDIT setting */
#define   CFFEIKV   3748    /* Invalid KEEP value             */
#define   CFFEOVR   3749    /* Invalid AUDIT OVERWRITE keyword         */
#define   CFFEIAI   3750    /* Invalid ADAPTER_ID parameter         */
#define   CFFEIMI   3751    /* Invalid MAIL_ID parameter         */
#define   CFFEIUI   3752    /* Invalid APP_ID parameter          */
#define   CFFEINW   3753    /* Invalid NETWORK_ID parameter         */
#define   CFFEIAD   3754    /* Invalid NWAdvertiseMode parameter       */
#define   CFFENLS   3755    /* NDSLoginId not specified          */
#define   RSOEOOS   3801    /* Out of sessions                */
#define   RSOENEM   3802    /* Out of memory at the client          */
#define   RSOEINE   3803    /* Fatal SQLBase System Failure (RSE INE)        */
#define   RSOECFS   3804    /* Cannot find session            */
#define   RSOESNE   3805    /* Session does not exist            */
#define   RSOESCT   3806    /* Session closed/terminated         */
#define   LOGECCL   3901    /* Cannot create log file            */
#define   LOGEILF   3902    /* Incompatible Log file Format         */
#define   LOGECAL   3903    /* Out of memory at the database computer (LOG CAL) */
#define   LOGECAB   3904    /* Out of memory at the database computer (LOG CAB) */
#define   LOGECOL   3905    /* NOT USED                 */
#define   LOGECRL   3906    /* NOT USED                 */
#define   LOGELNE   3907    /* Fatal SQLBase System Failure (LOG LNE)        */
#define   LOGELRO   3908    /* Fatal SQLBase System Failure (LOG LRO)        */
#define   LOGEBMP   3909    /* Fatal SQLBase System Failure (LOG BMP)        */
#define   LOGEMIP   3910    /* Fatal SQLBase System Failure (LOG MIP)        */
#define   LOGEPNP   3911    /* Fatal SQLBase System Failure (LOG PNP)        */
#define   LOGEETS   3912    /* Exceed transaction span limit           */
#define   LOGEOOM   3913    /* Out of memory at the database computer (LOG OOM) */
#define   LOGEBOX   3914    /* INTERNAL USE ONLY - Boxcar event        */
#define   LOGECLF   3915    /* Fatal SQLBase System Failure (LOG CLF)        */
#define   LOGEBLF   3916    /* Bad log file                */
#define   LOGEIOP   3917    /* Cannot open the log file for consistency check. */
#define   LOGEINB   3918    /* Cannot read expected number of bytes from the log file. */
#define   LOGEIOF   3919    /* The log file offset has become inconsistent.  */
#define   LOGEIFB   3920    /* The log file does not match the log buffer.   */
#define   LOGEIFS   3921    /* The log file does not match the buffer saved. */
#define   LOGEIBS   3922    /* The saved log buffer does not match the original. */
#define   LOGERFL   3923    /* A bad zero-filled log file length has been detected. */
#define   LOGERCS   3924    /* A bad log flush record checksum has been detected. */
#define   LOGERNB   3925    /* The number of bytes checksumed is wrong.      */
#define   LOGELSC   3926    /* Log disk space critically short         */
#define   RECECAR   4001    /* Out of memory at the database computer (REC CAR) */
#define   RECEBLC   4002    /* Bad log record chain              */
#define   RECELRF   4003    /* Log read failed                */
#define   RECEMBT   4004    /* Missing begin transaction log record       */
#define   RECECOL   4005    /* Cannot open log file              */
#define   RECEBLR   4006    /* Bad log record                 */
#define   RECEPUF   4007    /* Physical undo of a deallocation chain failed  */
#define   RECESNF   4008    /* Savepoint not found            */
#define   RECECRL   4009    /* Out of memory at the database computer (REC CRL) */
#define   RECEFNB   4010    /* First log record not begin transaction        */
#define   RECECP1   4011    /* Missing first checkpoint log record        */
#define   RECECP2   4012    /* Missing second checkpoint log record       */
#define   RECETLO   4013    /* Fatal SQLBase System Failure (REC TLO)        */
#define   RECECRC   4014    /* Out of memory at the database computer (REC CRC) */
#define   RECEDBT   4015    /* Duplicate begin transaction record      */
#define   RECETMB   4016    /* Rollforward time must be after online backup end */
#define   RECEMOB   4017    /* Missing online backup begin log record        */
#define   RECEMOE   4018    /* Missing online backup end log record       */
#define   RECEBTV   4019    /* Fatal SQLBase System Failure (REC BTV)        */
#define   RECERFN   4020    /* A Rollforward is not currently in progress    */
#define   RECERFA   4021    /* Rollforward already started          */
#define   RECERFI   4022    /* Rollforward interrupted - must restore database backup again */
#define   RECECRF   4023    /* Restore database first before rollforward     */
#define   RECERCO   4024    /* Cannot rollforward thru region where recovery was turned off */
#define   RECEITI   4025    /* Fatal SQLBase System Failure (REC ITI)        */
#define   RECECLR   4026    /* Fatal SQLBase System Failure (REC CLR)        */
#define   RECELCM   4027    /* Fatal SQLBase System Failure (REC LCM)        */
#define   BMOEOOM   4101    /* Out of menory at the database computer (BMO OOM) */
#define   BMOEEDA   4102    /* Compressed load file corrupt         */
#define   BMOEKIL   4103    /* Message has been killed              */
#define   BMOEOPE   4104    /* Cannot open the file              */
#define   BMOEFAE   4105    /* Unload file already exists.          */
#define   CPREOOM   4201    /* Out of menory at the database computer (CPR OOM) */
#define   CPREOVR   4202    /* INTERNAL USE ONLY - Internal error (CPR OVR)  */
#define   CPREINE   4203    /* INTERNAL USE ONLY - Internal error (CPR INE)  */
#define   CPRECRP   4204    /* Cannot recognize compressed data in file      */
#define   LDPECAL   4301    /* Out of menory at the database computer (LDP CAL) */
#define   LDPETMB   4302    /* Too much bind data             */
#define   LDPEIQS   4303    /* Invalid quoted data field         */
#define   LDPEIDT   4304    /* Invalid data type              */
#define   LDPEIDA   4305    /* Invalid data for specified data type       */
#define   LDPEUEF   4306    /* Unexpected end of file            */
#define   LDPENAP   4307    /* Data not specified for all columns in the last row */
#define   LDPEMCM   4308    /* Missing comma separator              */
#define   LDPEINF   4309    /* Load long with file name not supported        */
#define   LDPEILL   4310    /* Input too long for load buffer          */
#define   LDPEDTL   4311    /* Diff table name too long          */
#define   LDPETNM   4312    /* Table name missing from load file       */
#define   LDPEVCM   4313    /* Vector count missing on load file       */
#define   LDPEICO   4314    /* Invalid column number             */
#define   LDPEIDI   4315    /* Invalid load data item found         */
#define   LDPESDI   4316    /* Special data item expected in load      */
#define   LDPEINV   4317    /* Invalid version string            */
#define   LDPEINE   4318    /* Load file end before insert command bind data */
#define   LDPEINB   4319    /* Load file insert command does not bind data   */
#define   LDPESET   4320    /* Invalid set command in load file        */
#define   LDPETNE   4321    /* Table does not exist              */
#define   LDPEMEQ   4322    /* Missing end quote              */
#define   LDPEGON   4323    /* Load Process already gone         */
#define   LDPENSV   4324    /* Single user system try to execute a server load command */
#define   LDPENEX   4325    /* Load command could not compile and execute twice */
#define   LDPENVS   4326    /* Load command hit end befor the start line number */
#define   LDPEOOO   4327    /* Missing ON or OFF keyword         */
#define   LDPEMQN   4328    /* Missing query name             */
#define   LDPECTL   4329    /* Creator name is too large         */
#define   LDPESTL   4330    /* Stored command name is too large        */
#define   LDPETPC   4331    /* Tuple count missing in DIF load file       */
#define   LDPEICR   4332    /* Invalid COMMIT or ROLLBACK command in the load file. */
#define   LDPECLF   4333    /* Cannot create Message Log file          */
#define   LDPEILC   4334    /* Invalid LOAD or UNLOAD command in the load file. */
#define   UNLEPNR   4401    /* Non-printable character found           */
#define   UNLECLF   4402    /* Cannot open the log file          */
#define   UNLETNE   4403    /* Fatal unload error             */
#define   UNLETNP   4404    /* Specified Table does not exist in the database */
#define   UNLEIFV   4405    /* Fatal unload error             */
#define   UNLECUL   4406    /* Can't unload long data            */
#define   UNLECUB   4407    /* Out of memory at the server (UNL CUB)      */
#define   UNLECSC   4408    /* Can not store the command if ON SERVER option is not specified */
#define   UNLECUS   4409    /* Unload of system table not allowed      */
#define   UNLECRM   4410    /* Cannot unload while in result set restriction mode */
#define   UNLENSA   4411    /* Must be SYSADM to unload database or schema   */
#define   CSVEMNI   4501    /* 003 - Fatal SQLBase System Failure (CSV MNI)  */
#define   CSVEMNE   4502    /* MAIN database not exist              */
#define   CSVEPNF   4503    /* 003 - Fatal SQLBase System Failure (CSV PNF)  */
#define   CSVEITI   4504    /* 003 - Fatal SQLBase System Failure (CSV ITI)  */
#define   CSVETIU   4505    /* 003 - Fatal SQLBase System Failure (CSV TIU)  */
#define   CSVEBPT   4506    /* 003 - Fatal SQLBase System Failure (CSV BPT)  */
#define   CSVEOOM   4507    /* 008 - Out of memory at the database computer  */
#define   CSVEIST   4508    /* 003 - Fatal SQLBase System Failure (CSV IST)  */
#define   CSVEICF   4509    /* 003 - Fatal SQLBase System Failure (CSV ICF)  */
#define   CSVETMP   4510    /* 003 - Fatal SQLBase System Failure (CSV TMP)  */
#define   CSVEIPS   4511    /* 003 - Fatal SQLBase System Failure (CSV IPS)  */
#define   CSVEISS   4512    /* 003 - Fatal SQLBase System Failure (CSV ISS)  */
#define   CSVETCL   4513    /* Too many commit-server log records for transaction. */
#define   CSVECSN   4514    /* Commit-server feature not enabled.      */
#define   CSVERDS   4515    /* Commit Server Recovery Daemon has already been Started (CSV RDS) */
#define   CSVECCD   4516    /* Commit Server cannot connect to participant database (CSV CCD) */
#define   CSVEISC   4517    /* Invalid command for single-user server.       */
#define   DTMEIST   4601    /* 003 - Fatal SQLBase System Failure (DTM IST)  */
#define   DTMEICF   4602    /* 003 - Fatal SQLBase System Failure (DTM ICF)  */
#define   DTMECAC   4603    /* Out of memory at the database computer (DTM CAC) */
#define   DTMETLU   4604    /* Distributed transaction left unresolved.      */
#define   DTMEUTS   4605    /* Unresolved distributed transactions found.    */
#define   DTMEITI   4606    /* 003 - Fatal SQLBase System Failure (DTM ITI)  */
#define   DTMECRT   4607    /* Cannot resolve distributed transaction (DTM CRT) */
#define   DTMEVNM   4608    /* Database not set up properly  (DTM VNM)       */
#define   DTMEUST   4609    /* Unresolved single user distributed transaction found. */
#define   CDNECAC   4701    /* 002 - Out of memory at the client       */
#define   CDNECCS   4702    /* 002 - Out of memory at the client       */
#define   CDNECAP   4703    /* 002 - Out of memory at the client       */
#define   CDNEMNI   4704    /* 003 - Fatal SQLBase System Failure (CDN MNI)  */
#define   CDNENCS   4705    /* No commit-server available           */
#define   CDNERBV   4706    /* Rollback vote from participant.         */
#define   CDNEI2P   4707    /* Invalid 2-phase commit protocol specified.    */
#define   CDNEIFC   4708    /* Invalid function code.            */
#define   SLKECAS   4801    /* 008 - Out of memory at the database computer  */
#define   SLKETST   4802    /* 003 - Fatal SQLBase System Failure (SLK TST)  */
#define   SLKERDM   4803    /* 003 - Fatal SQLBase System Failure (SLK RDM)  */
#define   SLKETMO   4804    /* Semaphore-lock timeout            */
#define   SLKEILM   4805    /* 003 - Fatal SQLBase System Failure (SLK RDM)  */
#define   SLKETDO   4806    /* 003 - Fatal SQLBase System Failure (SLK TDO)  */
#define   SLKEICF   4807    /* 003 - Fatal SQLBase System Failure (SLK ICF)  */
#define   SLKEMNI   4808    /* 003 - Fatal SQLBase System Failure (SLK MNI)  */
#define   SLKELOV   4809    /* 003 - Fatal SQLBase System Failure (SLK LOV)  */
#define   SLKECAB   4810    /* 008 - Out of memory at the database computer  */
#define   HSTETTB   4901    /* 003 - Fatal SQLBase System Failure (HST TTB)  */
#define   HSTECAT   4902    /* 008 - Out of memory at the database computer  */
#define   HSTENUK   4903    /* 003 - Fatal SQLBase System Failure (HST NUK)  */
#define   HSTECAE   4904    /* 008 - Out of memory at the database computer  */
#define   HSTEEDE   4905    /* 003 - Fatal SQLBase System Failure (HST EDE)  */
#define   HSTEINS   4906    /* 003 - Fatal SQLBase System Failure (HST INS)  */
#define   HSTEICF   4907    /* 003 - Fatal SQLBase System Failure (HST ICF)  */
#define   HSTEINK   4908    /* 003 - Fatal SQLBase System Failure (HST INK)  */
#define   TLKEINR   5001    /* Invalid number of rows            */
#define   TLKEQNS   5002    /* Result set restriction mode is not supported for this query */
#define   TLKECOL   5003    /* Can't open long data file -- skipped       */
#define   TLKECOC   5004    /* Can't open the cursor file           */
#define   TLKECNA   5005    /* Cursor is not active              */
#define   TLKEDNA   5006    /* Data not allowed with this command      */
#define   TLKECTB   5007    /* Command too big -- ignored           */
#define   TLKEICN   5008    /* Invalid cursor number             */
#define   TLKEMFR   5009    /* Missing FROM keyword              */
#define   TLKETMC   5010    /* Too many columns               */
#define   TLKECUL   5011    /* Can't unload long data            */
#define   TLKEMBC   5012    /* Missing or misspelled CONNECTION keyword      */
#define   TLKEOPE   5013    /* Open file error                */
#define   TLKECFE   5014    /* Close file error               */
#define   TLKEITL   5015    /* Identifier too long            */
#define   TLKEIVA   5016    /* Invalid argument               */
#define   TLKEIBC   5017    /* Invalid connection name              */
#define   TLKEMSL   5018    /* Missing slash (e.g.user/password)       */
#define   TLKEISC   5019    /* Invalid SET command            */
#define   TLKECNE   5020    /* Command not properly ended           */
#define   TLKEICS   5021    /* Invalid cursor size            */
#define   TLKEIVP   5022    /* Invalid password               */
#define   TLKEMRP   5023    /* Missing right parentheses         */
#define   TLKEMIC   5024    /* Missing or invalid cursor number        */
#define   TLKECPS   5025    /* Invalid cache page size              */
#define   TLKERFN   5026    /* Invalid run file name             */
#define   TLKEEDB   5027    /* Expected database/username/password authentication string */
#define   TLKECOR   5028    /* Can't open the run file              */
#define   TLKEOOO   5029    /* Missing ON or OFF keyword         */
#define   TLKEQNE   5030    /* Quoted string not ended properly        */
#define   TLKEQTL   5031    /* Quoted string is too long         */
#define   TLKEMQS   5032    /* Missing quoted string             */
#define   TLKENTL   5033    /* Null string is too large          */
#define   TLKEISH   5034    /* Invalid show command              */
#define   TLKENSF   5035    /* No active spool file to close           */
#define   TLKESFE   5036    /* Spool file is already active         */
#define   TLKECOS   5037    /* Can't open the spool file         */
#define   TLKEIFN   5038    /* Invalid file name              */
#define   TLKEMLP   5039    /* NOT USED                 */
#define   TLKETMA   5040    /* Too many arguments             */
#define   TLKENCF   5041    /* No command found to process          */
#define   TLKEMBS   5042    /* Previous command must be a select       */
#define   TLKEMQN   5043    /* Missing query name             */
#define   TLKECNS   5044    /* Command NOT stored             */
#define   TLKEIFT   5045    /* Invalid format type            */
#define   TLKEOSF   5046    /* Only SQL format allow multi-tables      */
#define   TLKEULA   5047    /* Unload aborted                 */
#define   TLKECUS   5048    /* Unload of system table not allowed      */
#define   TLKETNE   5049    /* Table does not exist              */
#define   TLKEUEF   5050    /* Unexpected end of file            */
#define   TLKETNM   5051    /* Table name missing from load file       */
#define   TLKEVCM   5052    /* Vector count missing on load file       */
#define   TLKEUDT   5053    /* NOT USED                 */
#define   TLKETTL   5054    /* NOT USED                 */
#define   TLKEIDI   5055    /* Invalid load data item found         */
#define   TLKESDI   5056    /* Special data item expected in load      */
#define   TLKEISN   5057    /* Invalid source file name          */
#define   TLKEITP   5058    /* Invalid title parameter              */
#define   TLKENTS   5059    /* No tables specified            */
#define   TLKETEM   5060    /* Title exceeds max number of lines       */
#define   TLKEDTN   5061    /* Destination table not used with SQL format    */
#define   TLKEMDT   5062    /* Missing destination table name          */
#define   TLKEICI   5063    /* Invalid column identifier         */
#define   TLKEHTL   5064    /* Header too long                */
#define   TLKEHEM   5065    /* Header exceeds max lines allowed        */
#define   TLKEICW   5066    /* Invalid column width              */
#define   TLKEICP   5067    /* Invalid column parameter          */
#define   TLKETIL   5068    /* Title too large                */
#define   TLKEFCE   5069    /* FATAL CONNECT ERROR - EXIT NOW          */
#define   TLKECPT   5070    /* Cache page size too large         */
#define   TLKEUEV   5071    /* Invalid value               */
#define   TLKECRU   5072    /* Can't remove unload file          */
#define   TLKEDNE   5073    /* Database does not exist              */
#define   TLKEMEK   5074    /* Missing ERRORS keyword            */
#define   TLKEIWS   5075    /* Out of memory at the client (TLK IWS)      */
#define   TLKEPTL   5076    /* Picture too long               */
#define   TLKEIPI   5077    /* NOT USED                 */
#define   TLKEIAP   5078    /* Invalid adjust parameter          */
#define   TLKESAV   5079    /* Invalid SAVE command parameter          */
#define   TLKEIBP   5080    /* Invalid BREAK command parameter         */
#define   TLKENBS   5081    /* No breaks specified            */
#define   TLKECMP   5082    /* Invalid COMPUTE command parameter       */
#define   TLKEMOF   5083    /* Missing OF keyword             */
#define   TLKEMON   5084    /* NOT USED                 */
#define   TLKENBC   5085    /* No break exists for this column         */
#define   TLKEINC   5086    /* Invalid number of columns         */
#define   TLKEAAE   5087    /* Alias already exists              */
#define   TLKENPF   5088    /* No previous file found for $DATA        */
#define   TLKENDF   5089    /* No data found in previous file          */
#define   TLKECOP   5090    /* Can not open printer for report         */
#define   TLKESTS   5091    /* Size smaller than column width          */
#define   TLKECCD   5092    /* Cursor number conflicts with database name    */
#define   TLKECIF   5093    /* Cannot open input file            */
#define   TLKEMST   5094    /* Missing source or destination table name      */
#define   TLKEISI   5095    /* NOT USED                 */
#define   TLKEMTO   5096    /* Missing TO keyword             */
#define   TLKENPC   5097    /* No previous command found         */
#define   TLKELNF   5098    /* Line number not found             */
#define   TLKECDC   5099    /* NOT USED                 */
#define   TLKECEF   5100    /* Cannot open edit file             */
#define   TLKEEDI   5101    /* Editor not available              */
#define   TLKEDEE   5102    /* DOS execution error            */
#define   TLKEMJK   5103    /* NOT USED                 */
#define   TLKEJFN   5104    /* NOT USED                 */
#define   TLKEFIL   5105    /* Scroll mode already active           */
#define   TLKESSS   5106    /* NOT USED                 */
#define   TLKECSM   5107    /* Cannot set cache pages with multi-user database */
#define   TLKESP1   5108    /* Cannot set cache pages if connected more than once */
#define   TLKEDCC   5109    /* Cannot disconnect current cursor        */
#define   TLKEBND   5110    /* Bind variables with no bind data        */
#define   TLKEMBD   5111    /* NOT USED                 */
#define   TLKECTS   5112    /* Command size too small            */
#define   TLKECAC   5113    /* Out of memory at the client (TLK CAC)      */
#define   TLKEILL   5114    /* Input too long for load buffer          */
#define   TLKECAL   5115    /* Out of memory at the client (TLK CAL)      */
#define   TLKEDTL   5116    /* Diff table name too long          */
#define   TLKECUB   5117    /* Out of memory at the client (TLK CUB)      */
#define   TLKEMCK   5118    /* Missing CURSOR or FILE keyword          */
#define   TLKEIDN   5119    /* Invalid database name             */
#define   TLKEITN   5120    /* Invalid table name             */
#define   TLKENSC   5121    /* Out of memory at the client (TLK NSC)      */
#define   TLKECPO   5122    /* Can not turn print off for subtitle        */
#define   TLKETND   5123    /* Table not found in dictionary           */
#define   TLKEMDB   5124    /* Missing DATABASE keyword          */
#define   TLKESRV   5125    /* Missing SERVER keyword            */
#define   TLKEISV   5126    /* NOT USED                 */
#define   TLKEMDS   5127    /* Missing DATABASES keyword         */
#define   TLKENSA   5128    /* Must be SYSADM to unload database       */
#define   TLKENCD   5129    /* NOT USED                 */
#define   TLKE001   5130    /* NOT USED                 */
#define   TLKESCN   5131    /* Select list element is not a simple column name */
#define   TLKEMFC   5132    /* Missing FROM <directory> clause         */
#define   TLKEMTC   5133    /* Missing TO <database> clause         */
#define   TLKESSC   5134    /* SET SERVER command must precede this operation */
#define   TLKE002   5135    /* NOT USED                 */
#define   TLKE003   5136    /* NOT USED                 */
#define   TLKEBDT   5137    /* Bad datetime, expected: MM/DD/YY HH:MM:SS     */
#define   TLKEBTE   5138    /* BACKUP, TIME, or END expected           */
#define   TLKEIVS   5139    /* Invalid server name            */
#define   TLKERLE   5140    /* RELEASE LOG expected              */
#define   TLKEDOV   5141    /* NOT USED                 */
#define   TLKEOOS   5142    /* Only ON SERVER or ON CLIENT allowed        */
#define   TLKEMPE   5143    /* NOT USED                 */
#define   TLKERSM   5144    /* Result set mode must be in effect to set restriction */
#define   TLKETMB   5145    /* Too much bind data             */
#define   TLKEIDT   5146    /* Invalid data type              */
#define   TLKEIDA   5147    /* Invalid data for specified data type       */
#define   TLKEIUN   5148    /* Invalid user name              */
#define   TLKTNOF   5149    /* NOT USED                 */
#define   TLKEPWN   5150    /* NOT USED                 */
#define   TLKEIQS   5151    /* Invalid quoted data field         */
#define   TLKECNL   5152    /* Set cursor is no longer supported       */
#define   TLKEICO   5153    /* Invalid column number             */
#define   TLKESTL   5154    /* Stored command name is too large        */
#define   TLKECTL   5155    /* Creator name is too large         */
#define   TLKEJOR   5156    /* NOT USED                 */
#define   TLKEENM   5157    /* Expecting a number             */
#define   TLKEINV   5158    /* Invalid version string            */
#define   TLKEDLS   5159    /* DATABASE, LOGS, or SNAPSHOT expected       */
#define   TLKENPR   5160    /* Non-printable character found           */
#define   TLKEOOD   5161    /* Missing ON, OFF or DEFAULT keyword      */
#define   TLKEMSN   5162    /* Missing server name            */
#define   TLKENCN   5163    /* No CONNECTs done yet: this command requires a database connection */
#define   TLKECAB   5164    /* Command aborted due to user's instruction     */
#define   TLKEPEP   5165    /* Prepare expected before fetch           */
#define   TLKENOP   5166    /* NOT USED                 */
#define   TLKEUWF   5167    /* NOT USED                 */
#define   TLKEFWF   5168    /* File write failure             */
#define   TLKEICH   5169    /* Invalid character              */
#define   TLKENIW   5170    /* This feature is not supported in Windows      */
#define   TLKEUDS   5171    /* UNLOAD DATABASE only supported for SQLBase    */
#define   TLKEMEQ   5172    /* Missing end quote              */
#define   TLKEMCM   5173    /* Missing comma separator              */
#define   TLKEIVC   5174    /* Code page is not same, check your SQL.INI file and country.sql */
#define   TLKEMSF   5175    /* Database session not exist, must have spool file name */
#define   TLKERCV   5176    /* Database is active with RECOVERY ON        */
#define   TLKEICL   5177    /* Invalid client name            */
#define   TLKEIPN   5178    /* Invalid process number            */
#define   TLKEISE   5179    /* Invalid sqlset command            */
#define   TLKEIGE   5180    /* Invalid sqlget command            */
#define   TLKEIGS   5181    /* Invalid sqlgsi flag            */
#define   TLKELIM   5182    /* Transaction outcome is unknown.         */
#define   TLKEIPT   5183    /* Invalid participant name          */
#define   TLKEDTS   5184    /* Distributed transaction not started.       */
#define   TLKEMSC   5185    /* Stored command cannot be found.         */
#define   TLKEPER   5186    /* Perform must come after a prepare with planonly on */
#define   TLKEBCI   5187    /* Syntax for BEGIN CONNECTION is invalid.       */
#define   TLKEBCE   5188    /* Connection Handle Exists.         */
#define   TLKELTL   5189    /* Identifier too long            */
#define   DB2E001   6001    /* Invalid network message or incorrect message version */
#define   DB2E002   6002    /* Too many DB2 cursors              */
#define   DB2E003   6003    /* This operation is invalid after a commit or rollback */
#define   DB2E004   6004    /* Cursor not found for disconnect         */
#define   DB2E005   6005    /* Unable to locate host variable for execute or read long */
#define   DB2E006   6006    /* Cursor not open for fetch         */
#define   DB2E007   6007    /* Statement contains an illegal character       */
#define   DB2E010   6010    /* The string constant is not terminated      */
#define   DB2E084   6084    /* Unacceptable SQL statement           */
#define   DB2E100   6100    /* Row not found, or the result of a query is an empty table */
#define   DB2E101   6101    /* The statement is too long or complex       */
#define   DB2E102   6102    /* String constant length > 254 characters or 124 graphic characters */
#define   DB2E103   6103    /* Invalid numeric literal              */
#define   DB2E104   6104    /* Statement contains an invalid character or token */
#define   DB2E105   6105    /* Invalid string constants          */
#define   DB2E107   6107    /* Name used that is too long           */
#define   DB2E109   6109    /* Attempt to use a clause that is not permitted */
#define   DB2E110   6110    /* Invalid hexadecimal literal          */
#define   DB2E111   6111    /* A SQL function does not include a column name */
#define   DB2E112   6112    /* Function operand is another function or expression after DISTINCT */
#define   DB2E113   6113    /* Invalid character found in name         */
#define   DB2E115   6115    /* See Error SQLCODE number -115 in IBM DB2 Messages and Codes */
#define   DB2E117   6117    /* Number of insert values not equal to number of object columns */
#define   DB2E118   6118    /* Object table/view of INSERT,DELETE, or UPDATE also in FROM clause */
#define   DB2E119   6119    /* A Column in HAVING clause is not included in the GROUP BY clause */
#define   DB2E120   6120    /* WHERE or SET has function or refers to column in GROUP BY of view */
#define   DB2E121   6121    /* Column identified more than once in INSERT or UPDATE statement */
#define   DB2E122   6122    /* SELECT without GROUP BY has col & function or col not in GROUP BY */
#define   DB2E125   6125    /* Integer in the ORDER BY does not identify a column of the result */
#define   DB2E126   6126    /* The SELECT contains both an UPDATE clause and an ORDER BY clause */
#define   DB2E127   6127    /* DISTINCT is specified more than once in a subselect */
#define   DB2E128   6128    /* Invalid use of NULL in predicate        */
#define   DB2E129   6129    /* The statement contains too many table names   */
#define   DB2E131   6131    /* Statement with LIKE predicate has incompatible data types */
#define   DB2E132   6132    /* Bad LIKE predicate: operand 1 not column or operand 2 not string */
#define   DB2E133   6133    /* Bad function in HAVING:arithmetic operation applied to correlated reference */
#define   DB2E134   6134    /* Improper use of long string col or host variable max length > 254 */
#define   DB2E136   6136    /* Sort cannot be executed because the sort key length > 4000 bytes */
#define   DB2E150   6150    /* INSERT, DELETE, or UPDATE on view where desired operation is not allowed */
#define   DB2E151   6151    /* See Error SQLCODE number -151 in IBM DB2 Messages and Codes */
#define   DB2E153   6153    /* The create view statement does not include a required column list */
#define   DB2E154   6154    /* Create view failed because the view definition contains a view */
#define   DB2E155   6155    /* Bad FROM clause: one of the tables is a view that has a GROUP BY */
#define   DB2E156   6156    /* ALTER TABLE, DROP TABLE, LOCK TABLE, or CREATE INDEX uses a view */
#define   DB2E158   6158    /* Number of columns in view not same as number of columns in SELECT */
#define   DB2E159   6159    /* The name specified on DROP VIEW is a table name */
#define   DB2E160   6160    /* The WITH CHECK OPTION cannot be used for the specified view */
#define   DB2E161   6161    /* INSERT/UPDATE not allowed cause result row does not satisfy view */
#define   DB2E164   6164    /* User does not have the privilege to create a view */
#define   DB2E170   6170    /* The number of arguments specified for a function is invalid */
#define   DB2E171   6171    /* The data type/length/value of an argument to a function is invalid */
#define   DB2E172   6172    /* An invalid function name is specified      */
#define   DB2E180   6180    /* The string representation of a datetime value has invalid syntax */
#define   DB2E181   6181    /* The string representation of a datetime value is not a valid value */
#define   DB2E182   6182    /* An arithmetic expression with a datetime value is invalid */
#define   DB2E183   6183    /* An arithmetic operation on a date/timestamp has an invalid result */
#define   DB2E184   6184    /* Arithmetic expression with a datetime value contains a host variable */
#define   DB2E185   6185    /* See Error SQLCODE number -185 in IBM DB2 Messages and Codes */
#define   DB2E186   6186    /* See Error SQLCODE number -186 in IBM DB2 Messages and Codes */
#define   DB2E198   6198    /* The operand of the PREPARE or EXECUTE IMMEDIATE is blank or empty */
#define   DB2E199   6199    /* Illegal use of a keyword, a token was expected */
#define   DB2E203   6203    /* A reference to a column is ambiguous       */
#define   DB2E204   6204    /* A name is an undefined name          */
#define   DB2E205   6205    /* Column used that is not a column of the table used */
#define   DB2E206   6206    /* Column not in inserted table, updated table, or any table in FROM */
#define   DB2E207   6207    /* Bad ORDER BY: includes column that applies to result of a UNION */
#define   DB2E208   6208    /* Invalid ORDER BY: an ORDER BY column is not part of result table */
#define   DB2E219   6219    /* The required explanation table does not exist */
#define   DB2E220   6220    /* A column in explanation table is not defined properly */
#define   DB2E301   6301    /* Host variable can't be used as specified because of its data type */
#define   DB2E302   6302    /* The value of an input variable is too large for the target column */
#define   DB2E303   6303    /* Value not assigned to host variable: data types not comparable */
#define   DB2E304   6304    /* Value not assigned to host variable: value not in data type range */
#define   DB2E305   6305    /* NULL Value not assigned to host variable: no indicator variable */
#define   DB2E309   6309    /* Invalid predicate: referenced host variable has the NULL length */
#define   DB2E311   6311    /* The indicated length of a host variable is negative */
#define   DB2E312   6312    /* Undefined or unusable host variable        */
#define   DB2E313   6313    /* Number of host variables not equal to number of parameter markers */
#define   DB2E401   6401    /* Operands of arithmetic or comparison operation are not comparable */
#define   DB2E402   6402    /* An arithmetic function or operator is applied to character data */
#define   DB2E404   6404    /* The UPDATE or INSERT specifies a string that is too long */
#define   DB2E405   6405    /* Numeric literal can't be used as specified cause its out of range */
#define   DB2E406   6406    /* A derived numeric value is not within range of its object column */
#define   DB2E407   6407    /* Value is NULL, but object column can not contain NULL values */
#define   DB2E408   6408    /* Value is not comparable with the data type of its object column */
#define   DB2E409   6409    /* Invalid operand of a COUNT function        */
#define   DB2E410   6410    /* The floating point literal contains more than 30 characters */
#define   DB2E411   6411    /* The keyword USER cannot be used as specified  */
#define   DB2E412   6412    /* The select clause of a subquery specifies multiple columns */
#define   DB2E413   6413    /* Overflow occurred during data type conversion */
#define   DB2E414   6414    /* The numeric column is specified in a LIKE predicate */
#define   DB2E415   6415    /* Corresponding columns in UNION don't have identical descriptions */
#define   DB2E416   6416    /* An operand of a UNION contains a long string column */
#define   DB2E417   6417    /* STATEMENT includes parameter markers as operands of same operator */
#define   DB2E418   6418    /* STATEMENT string to be prepared has parameter markers in a SELECT */
#define   DB2E419   6419    /* The result of decimal division has produced a negative scale */
#define   DB2E421   6421    /* The operands of a UNION do not have the same number of columns */
#define   DB2E501   6501    /* Cursor identified in a FETCH or DISCONNECT statement is not open */
#define   DB2E502   6502    /* The cursor identified in a CONNECT statement is already open */
#define   DB2E503   6503    /* Column not updated: not identified in UPDATE clause of SELECT */
#define   DB2E504   6504    /* The specified cursor name is not defined      */
#define   DB2E507   6507    /* The cursor identified in the UPDATE or DELETE is not open */
#define   DB2E508   6508    /* Cursor identified in UPDATE or DELETE is not positioned on a row */
#define   DB2E509   6509    /* Table in UPDATE or DELETE not the same table designated by cursor */
#define   DB2E510   6510    /* Table designated by cursor of UPDATE or DELETE cannot be modified */
#define   DB2E511   6511    /* FOR UPDATE not allowed:table designated by cursor can't be modified */
#define   DB2E514   6514    /* The cursor is not in a prepared state      */
#define   DB2E516   6516    /* The DESCRIBE statement does not identify a prepared statement */
#define   DB2E517   6517    /* Cursor not used: its name doesn't identify a prepared SELECT */
#define   DB2E518   6518    /* EXECUTE statement does not identify a valid prepared statement */
#define   DB2E519   6519    /* PREPARE statement identifies the SELECT of the opened cursor */
#define   DB2E551   6551    /* User lacks privilege to perform operation on specified object */
#define   DB2E552   6552    /* User does not have privilege to perform the specified operation */
#define   DB2E554   6554    /* An authorization ID cannot grant a privilege to itself */
#define   DB2E555   6555    /* An authorization ID cannot revoke a privilege from itself */
#define   DB2E556   6556    /* A privilege cannot be revoked because it is not possessed by user */
#define   DB2E557   6557    /* Inconsistent GRANT/REVOKE keyword       */
#define   DB2E558   6558    /* The WITH GRANT option is ignored because GRANT is to PUBLIC */
#define   DB2E559   6559    /* All authorization functions have been disabled */
#define   DB2E560   6560    /* The WITH GRANT option is ignored for UPDATE   */
#define   DB2E601   6601    /* Name of the object to be created is identical to an existing name */
#define   DB2E602   6602    /* Too many columns specified in a create index  */
#define   DB2E603   6603    /* Can't create unique index:non-unique data exist for col specified */
#define   DB2E604   6604    /* Column definition specifies invalid length, precision, or scale */
#define   DB2E607   6607    /* Operation is not defined for system tables    */
#define   DB2E612   6612    /* Column name is a duplicate column name        */
#define   DB2E614   6614    /* Can't create index: sum of internal key lengths > allowable max */
#define   DB2E616   6616    /* Specified object cannot be dropped because it is still referenced */
#define   DB2E618   6618    /* Operation is not allowed on system data bases */
#define   DB2E619   6619    /* Operation disallowed because work file data base is not stopped */
#define   DB2E620   6620    /* Keyword in STATEMENT not allowed for table space in work file database */
#define   DB2E623   6623    /* A clustering index already exists on the specified table */
#define   DB2E636   6636    /* Partitioning keys not specified in ascending or descending order */
#define   DB2E637   6637    /* Duplicate keywords             */
#define   DB2E644   6644    /* Invalid value specified for keyword in specified statement */
#define   DB2E646   6646    /* Table not created:partition/default table space already contains table */
#define   DB2E647   6647    /* Bufferpool cannot be specified because it has not been activated */
#define   DB2E652   6652    /* Violation of installation defined edit or validation procedure */
#define   DB2E653   6653    /* Table in partition table space not available: no partitioned index */
#define   DB2E660   6660    /* Index not created on partitioned table space:key limit not specified */
#define   DB2E661   6661    /* Index not created on partitioned table space: parts not = partitions */
#define   DB2E662   6662    /* Partitioned index can't be created on non-partitioned tablespace */
#define   DB2E663   6663    /* Number of key limit values is zero, or > columns in key of index */
#define   DB2E664   6664    /* Internal length of limit-key flds for partitioned index too big */
#define   DB2E665   6665    /* The PART clause of an ALTER statement is omitted or invalid */
#define   DB2E666   6666    /* The statement cannot be executed because a utility is in progress */
#define   DB2E667   6667    /* Cluster index for partition tablspace can't be explicitly dropped */
#define   DB2E668   6668    /* Can't add column to table because the table has an edit procedure */
#define   DB2E669   6669    /* A table in a partitioned tablespace cannot be explicitly dropped */
#define   DB2E670   6670    /* The record length of the table exceeds the page size limit */
#define   DB2E671   6671    /* Can't alter bufferpool attribute: it changes pagesize of table space */
#define   DB2E676   6676    /* A 32K page bufferpool may not be used for an index */
#define   DB2E677   6677    /* Insufficient virtual storage for bufferpool expansion */
#define   DB2E678   6678    /* Literal for index limit key must conform to data type of column */
#define   DB2E679   6679    /* Object specified can't be created cause DROP is pending on object */
#define   DB2E680   6680    /* Too many columns specified for a table        */
#define   DB2E681   6681    /* A column is in violation of installation defined field procedure */
#define   DB2E682   6682    /* Field procedure could not be loaded        */
#define   DB2E683   6683    /* Invalid column type for FIELDPROC option      */
#define   DB2E684   6684    /* Length of literal list is too long      */
#define   DB2E685   6685    /* Invalid field type             */
#define   DB2E686   6686    /* Column with w/fld proc cant compare w/column w/different fld proc */
#define   DB2E687   6687    /* Field types incomparable          */
#define   DB2E688   6688    /* Incorrect data returned from field procedure  */
#define   DB2E802   6802    /* Arithmetic overflow or division by zero has occurred */
#define   DB2E803   6803    /* Inserting/updating unique constrained columns with duplicate data */
#define   DB2E804   6804    /* Error in application program input parameters for SQL statement */
#define   DB2E805   6805    /* Specified program name not found in specified plan */
#define   DB2E811   6811    /* See Error SQLCODE number -811 in IBM DB2 Messages and Codes */
#define   DB2E815   6815    /* See Error SQLCODE number -815 in IBM DB2 Messages and Codes */
#define   DB2E817   6817    /* See Error SQLCODE number -817 in IBM DB2 Messages and Codes */
#define   DB2E818   6818    /* See Error SQLCODE number -818 in IBM DB2 Messages and Codes */
#define   DB2E819   6819    /* Can't recreate View cause it refers to a uncreatable view */
#define   DB2E820   6820    /* See Error SQLCODE number -820 in IBM DB2 Messages and Codes */
#define   DB2E822   6822    /* SQLDA contains invalid data address or indicator variable address */
#define   DB2E840   6840    /* Too many items returned in a SELECT or INSERT list */
#define   DB2E901   6901    /* See Error SQLCODE number -901 in IBM DB2 Messages and Codes */
#define   DB2E902   6902    /* See Error SQLCODE number -902 in IBM DB2 Messages and Codes */
#define   DB2E904   6904    /* Unsuccessful execution caused by unavailable resource */
#define   DB2E906   6906    /* SQL statement not executed: function disabled due to prior error */
#define   DB2E909   6909    /* The object has been deleted          */
#define   DB2E910   6910    /* SQL statement can't access object on which drop/alter is pending */
#define   DB2E911   6911    /* Current unit of work rolled back due to deadlock or timeout */
#define   DB2E913   6913    /* Unsuccessful execution caused by deadlock or timeout */
#define   DB2E922   6922    /* Connection authorization failure        */
#define   DB2E923   6923    /* Connection not established           */
#define   DB2E924   6924    /* DB2 connection internal error           */
#define   DB2E925   6925    /* COMMIT not valid in IMS/VS or CICS/OS/VS environment */
#define   DB2E926   6926    /* ROLLBACK not valid in IMS/VS or CIC/OS/VS environment */
#define   DB2E927   6927    /* Language Interface called: connecting environment not established */
#define   DB2E992   6992    /* Unable to reconnect to DB2; try again later   */
#define   DB2E993   6993    /* Set autocommit is invalid with multiple cursors */
#define   DB2E994   6994    /* Connecting with multiple cursors is invalid with autocommit */
#define   DB2E995   6995    /* Row too long for fetch            */
#define   DB2E996   6996    /* Long seek past end of long data         */
#define   DB2E997   6997    /* "WHERE CURRENT OF" cursor name not found      */
#define   DB2E998   6998    /* Invalid long field number         */
#define   DB2E999   6999    /* Current of Cursor processing cannot be used; fetch in-progress */
#define   NETE001   7001    /* Illegal buffer length             */
#define   NETE003   7003    /* Invalid command                */
#define   NETETMO   7005    /* Command timed-out              */
#define   NETE006   7006    /* Message incomplete             */
#define   NETEISN   7008    /* Illegal local session number         */
#define   NETE009   7009    /* No resource available             */
#define   NETECLO   7010    /* Session closed                 */
#define   NETE011   7011    /* Command canceled               */
#define   NETEDPL   7013    /* Duplicate name in local name table      */
#define   NETE014   7014    /* Name table is full             */
#define   NETE015   7015    /* Cmd completed, name has active sessions and is now de-registered */
#define   NETESTF   7017    /* Local session table full          */
#define   NETESOR   7018    /* Session open rejected             */
#define   NETE019   7019    /* Illegal name number            */
#define   NETECFN   7020    /* Cannot find name called or no answer       */
#define   NETE021   7021    /* Name not found, cannot specify *, or 00H      */
#define   NETE022   7022    /* Name in use on remote adapter           */
#define   NETE023   7023    /* Name was deleted               */
#define   NETESEA   7024    /* Session ended abnormally          */
#define   NETE025   7025    /* Name conflict detected            */
#define   NETE026   7026    /* Incompatible remote device           */
#define   NETE033   7033    /* Interface busy                 */
#define   NETE034   7034    /* Too many commands outstanding           */
#define   NETE035   7035    /* Invalid adapter number            */
#define   NETECDC   7036    /* Command completed while cancel occurring      */
#define   NETE037   7037    /* Reserved name specified              */
#define   NETE038   7038    /* Command not valid to cancel          */
#define   NETENPR   7255    /* In progress                 */
#define   NETENNF   7257    /* Network not functioning              */
#define   NETEINP   7258    /* Invalid network pointer              */
#define   NETE500   7500    /* DBGATEWY local session table full. Call systems programmer. */
#define   NETE501   7501    /* APPC/PC allocation failure, no retry possible */
#define   NETE502   7502    /* APPC/PC allocation failure, try again later   */
#define   NETE503   7503    /* APPC/PC allocation failure, bad userid/password */
#define   NETE505   7505    /* APPC/PC attach PU failure         */
#define   NETE506   7506    /* APPC/PC attach LU failure         */
#define   NETE507   7507    /* APPC/PC receive data failure         */
#define   NETE508   7508    /* APPC/PC send data failure         */
#define   NETE509   7509    /* APPC/PC attach DLC failure           */
#define   NETE510   7510    /* APPC/PC transaction program ending failure    */
#define   NETE511   7511    /* APPC/PC transaction program initiating failure */
#define   NETE512   7512    /* APPC/PC change number of sessions failure     */
#define   NETE513   7513    /* APPC/PC detach LU failure         */
#define   NETE514   7514    /* APPC/PC detach PU failure         */
#define   NETE515   7515    /* APPC/PC deallocate failure           */
#define   NETE516   7516    /* APPC/PC unknown failure; turn on verbose tracing */
#define   NETE517   7517    /* APPC/PC not dialed to a host         */
#define   NETE518   7518    /* APPC/PC no more host sessions available       */
#define   NETE519   7519    /* APPC/PC Unrecoverable error encountered with the host */
#define   NETECAA   7600    /* Cannot allocate adapter status structure      */
#define   NETECAN   7601    /* Cannot allocate network control block      */
#define   NETENBS   7602    /* Network buffer is too small          */
#define   NETECRM   7603    /* Cannot allocate real memory          */
#define   FIOECCF   8001    /* Cannot commit file             */
#define   FIOECAD   8002    /* NOT USED                 */
#define   FIOEFME   8003    /* INTERNAL USE ONLY - Max file map entries exceeded */
#define   FIOEOOM   8004    /* Out of memory at the database computer        */
#define   FIOECGD   8005    /* NOT USED                 */
#define   FIOECFN   8006    /* Cannot find file               */
#define   FIOEIFN   8007    /* Invalid file name              */
#define   FIOECFD   8008    /* Cannot find directory             */
#define   FIOECOF   8009    /* Cannot open file               */
#define   FIOEICN   8010    /* Invalid cluster number            */
#define   FIOENMS   8011    /* Not in map space               */
#define   FIOECRD   8012    /* Cannot retrieve disk buffer          */
#define   FIOECGI   8013    /* Cannot get information            */
#define   FIOEITG   8014    /* Integrity error                */
#define   FIOEDME   8015    /* Disk mapping error             */
#define   FIOECID   8016    /* NOT USED                 */
#define   FIOEDIW   8017    /* Unable to use DIRECTIO            */
#define   FIOECTF   8018    /* Cannot create DIRECTIO test file        */
#define   FIOEWTF   8019    /* Cannot write DIRECTIO test file         */
#define   FIOERTF   8020    /* NOT USED                 */
#define   FIOELTF   8021    /* Cannot close DIRECTIO test file         */
#define   FIOESTF   8022    /* Cannot seek in DIRECTIO test file       */
#define   MEMEOUT   8101    /* Out of memory for allocation         */
#define   MEMEBIG   8102    /* Requested memory size too big for allocation  */
#define   PCGENPC   8201    /* No procedure command found           */
#define   PCGECFF   8202    /* Cannot find function              */
#define   PCGEDOC   8203    /* Duplicate ON clause            */
#define   PCGENLD   8204    /* No LOOP defined                */
#define   PCGENPW   8205    /* No password can be retrieved for connecting to database. */
#define   PCGENCS   8206    /* Only constant-string commands are allowed in STATIC procedure */
#define   PCGENDD   8207    /* No DDL statement allowed in the static stored procedure */
#define   SPEEINT   8301    /* Internal error occurred in Stored-Procedure execution */
#define   SPEECAL   8302    /* Cannot allocate cursor array         */
#define   SPEECOR   8303    /* Cursor (number) out of range (connection probably never made) */
#define   SPEECIS   8304    /* Cursor in invalid state              */
#define   SPEECNN   8305    /* Cursor has no name and cannot be closed       */
#define   SPEECNU   8306    /* Cursor not in use              */
#define   SPEECNM   8307    /* Cursor name is missing            */
#define   SPEECIL   8308    /* Invalid cursor name length is detected        */
#define   SPEECOT   8309    /* Cursor opened twice            */
#define   SPEECNE   8310    /* Cursor name already exists           */
#define   SPEECIQ   8311    /* Cursor in invalid query state           */
#define   SPEEMNN   8331    /* Static embedded command has no name        */
#define   SPEEOIO   8341    /* Addition/subtraction involving datetime failed */
#define   SPEEOIU   8342    /* Invalid operand update detected         */
#define   SPEEOIZ   8343    /* Insufficient operand size detected      */
#define   SPEEOMD   8344    /* Missing operand detected          */
#define   SPEEOZL   8345    /* Zero length operand/result buffer detected    */
#define   SPEEDND   8350    /* Procedure data does not meet the DATE/TIME format */
#define   SPEENIR   8351    /* DATE/TIME parameter data not in allowed range */
#define   SPEEWBZ   8361    /* Invalid buffer size detected         */
#define   SPEEWDT   8362    /* Invalid database parameter type detected in SqlSetParameter */
#define   SPEEWDP   8363    /* Invalid database parameter           */
#define   SPEEWFN   8364    /* Number has a fractional part         */
#define   SPEEWIE   8365    /* Invalid SqlExecute detected          */
#define   SPEEWII   8366    /* Invalid INTO (variables) in command        */
#define   SPEEWIL   8367    /* Invalid string length detected          */
#define   SPEEWIO   8368    /* Invalid operand detected          */
#define   SPEEWIP   8369    /* Invalid numeric precision detected      */
#define   SPEEWIQ   8370    /* Invalid command type detected           */
#define   SPEEWIS   8371    /* Invalid numeric scale detected          */
#define   SPEEWIT   8372    /* Invalid data type detected           */
#define   SPEEWNG   8373    /* Number cannot be negative         */
#define   SPEEWNH   8374    /* Operand is not a sql handle          */
#define   SPEEWMI   8375    /* Missing item after ':'            */
#define   SPEEWNI   8376    /* There is no immediate cursor         */
#define   SPEEWNN   8377    /* Cursor has no name             */
#define   SPEEWNP   8378    /* Cannot open a cursor without a prepared query */
#define   SPEEWNS   8379    /* Cannot open a cursor on a non-select query    */
#define   SPEEWNV   8380    /* Named variable cannot be found          */
#define   SPEEWPZ   8381    /* SQL function (number) parameter size is too big */
#define   SPEEWTM   8382    /* Type mismatch               */
#define   SPEEWMV   8383    /* INTO variables are expected for executing given stored procedure */
#define   SPEEWOB   8384    /* No or insufficient BIND variables       */
#define   SPEEWTB   8385    /* Too many BIND variables              */
#define   SPEEWOI   8386    /* No or insufficient INTO variables       */
#define   SPEEWTI   8387    /* Too many INTO variables              */
#define   SPEEXIV   8388    /* Invalid parameter value detected        */
#define   SPEEXNS   8389    /* No space available for allocation       */
#define   SPEEXID   8390    /* Invalid date/time value              */
#define   SPEEWDD   8391    /* DDL construct not allowed in STATIC procedure */
#define   SPEEWWB   8392    /* Boolean value does not match parameter usage  */
#define   SPEEEOT   8393    /* Procedure terminated by system or application rollback */
#define   SPEESIL   8394    /* Cannot change isolation level from within stored procedure */
#define   SPEEFPN   8395    /* NOT USED                 */
#define   SPEESTL   8396    /* String too large               */
#define   SPEEDNN   8397    /* Procedure parameter data is not numeric       */
#define   SPEEINR   8398    /* Procedure numeric parameter data not in range */
#define   SPEEMPV   8399    /* Data not fully supplied for all procedure parameter(s) */
#define   SPEECNA   8400    /* LOAD/UNLOAD on CLIENT not allowed.      */
#define   EX2ETGX   8401    /* Trigger already exists            */
#define   EX2ETGD   8402    /* Trigger does not exist            */
#define   EX2EVNX   8403    /* Event already exists              */
#define   EX2EVND   8404    /* Event does not exist              */
#define   EX2EMUT   8405    /* Too many triggers for event/time/frequency    */
#define   EX2ETGA   8406    /* Identical trigger already exists        */
#define   EX2ETGC   8407    /* Message deleted - Multiple trigger support    */
#define   EX2ETGE   8408    /* Message deleted - Multiple trigger support    */
#define   EX2EVN2   8409    /* Event already exists              */
#define   EX2EVNM   8410    /* Event does not exist              */
#define   EX2ETGK   8411    /* Trigger defined on column         */
#define   EX2ETGL   8412    /* Delete Cascade constraint already defined on table. */
#define   EX2ETGM   8413    /* Delete Null constraint already defined on table. */
#define   EX2ETGN   8414    /* Delete trigger already defined on table.      */
#define   EX2ETGO   8415    /* Update trigger already defined on table.      */
#define   EX2ETGP   8416    /* Commit/Rollback/Savepoint not allowed from within triggerred */
#define   EX2ETGQ   8417    /* Maximum nesting level exceeded.         */
#define   EX2EVGZ   8418    /* The value being set should be greater than zero. */
#define   EX2EVPK   8419    /* The columns inside parenthesis do not form a valid prefix key. */
#define   EX2ENSC   8420    /* This is a non modifiable statistics column.   */
#define   EX2EVEZ   8421    /* The value being set should be greater than or equal to zero. */
#define   EX2EOHM   8422    /* Out of heap memory during execution of the command. */
#define   EX2EODB   8423    /* You are not an owner, or a creator, or a DBA. */
#define   EX2ETRT   8424    /* The table cannot be renamed since a trigger references it. */
#define   EX2ETRC   8425    /* Column cannot be renamed since a trigger exists on the table. */
#define   EX2ETMC   8426    /* Column cannot be modified since a trigger exists on the table. */
#define   EX2ETDC   8427    /* Column cannot be dropped since a trigger exists on the table. */
#define   EX2ETFF   8428    /* Trigger fetch failed.             */
#define   EX2EDOP   8429    /* Cannot delete function, dependent object <name> present. */
#define   EX2ESAC   8430    /* Cannot modify statistics column, active statistics present. */
#define   EVMEUIN   8501    /* INTERNAL USE ONLY - Event manager not initialized */
#define   EVMEAIN   8502    /* INTERNAL USE ONLY - Event manager already initialized */
#define   EVMEUEV   8503    /* INTERNAL USE ONLY - Unknown event       */
#define   EVMENEV   8504    /* INTERNAL USE ONLY - Event not allowed      */
#define   GCIEIBL   9001    /* Illegal buffer length             */
#define   GCIEIVC   9003    /* Invalid command                */
#define   GCIETMO   9005    /* Command timed-out              */
#define   GCIEMIN   9006    /* Message incomplete             */
#define   GCIEISN   9008    /* Illegal local session number         */
#define   GCIENRA   9009    /* No resource available             */
#define   GCIECLO   9010    /* Session closed                 */
#define   GCIECMC   9011    /* Command canceled               */
#define   GCIEDPL   9013    /* Duplicate name in local name table      */
#define   GCIENTF   9014    /* Name table is full             */
#define   GCIENDR   9015    /* Cmd completed, name has active sessions and is now de-registered */
#define   GCIESTF   9017    /* Local session table full          */
#define   GCIESOR   9018    /* Session open rejected             */
#define   GCIEINN   9019    /* Illegal name number            */
#define   GCIECFN   9020    /* Cannot find name called or no answer       */
#define   GCIENMF   9021    /* Name not found, cannot specify *, or 00H      */
#define   GCIENUR   9022    /* Name in use on remote adapter           */
#define   GCIENWD   9023    /* Name was deleted               */
#define   GCIESEA   9024    /* Session ended abnormally          */
#define   GCIENMC   9025    /* Name conflict detected            */
#define   GCIEIRD   9026    /* Incompatible remote device           */
#define   GCIESEN   9028    /* session ended normally            */
#define   GCIEIFB   9033    /* Interface busy                 */
#define   GCIETMC   9034    /* Too many commands outstanding           */
#define   GCIEIAN   9035    /* Invalid adapter number            */
#define   GCIECDC   9036    /* Command completed during cancel         */
#define   GCIERNS   9037    /* Reserved name specified              */
#define   GCIENVC   9038    /* Command not valid to cancel          */
#define   GCIEPND   9255    /* Pending completion             */
#define   GCIENNF   9256    /* Network not functioning              */
#define   GCIEINP   9257    /* Invalid network pointer              */
#define   GCIECGA   9258    /* Cannot get address             */
#define   GCIECRH   9259    /* Cannot resolve hostname              */
#define   GCIEDNC   9260    /* Database name not configured         */
#define   GCIEINI   9261    /* Invalid network interface         */
#define   GCIEINR   9262    /* Network interface not running           */
#define   GCIEOOM   9263    /* Out of memory               */
#define   GCIEINH   9264    /* Invalid handle                 */
#define   GCIESSE   9265    /* Session error               */
#define   GCIENCF   9266    /* No configuration file             */
#define   GCIEIDL   9267    /* Invalid COM DLL                */
#define   GCIECCN   9268    /* Cannot connect                 */
#define   GCIETMA   9269    /* Too many arguments             */
#define   GCIEIAL   9270    /* Invalid argument list             */
#define   GCIEAIN   9271    /* Already initialized            */
#define   GCIENTI   9272    /* Not initialized                */
#define   GCIETIP   9273    /* Termination in progress              */
#define   GCIEMCE   9274    /* Missing configuration file entry        */
#define   GCIENCD   9275    /* No COMDLL configuration parameter specified   */
#define   GCIESTO   9276    /* Semaphore timed out            */
#define   GCIECNL   9277    /* Cancel command error              */
#define   GCIEOOS   9278    /* Out of sessions                */
#define   GCIEEOR   9279    /* Cannot find specified protocol entry       */
#define   GCIECOC   9281    /* Cannot open configuration file          */
#define   GCIEGEN   9282    /* General error               */
#define   GCIEIVL   9283    /* Incorrect version of library         */
#define   GCIEICK   9284    /* Invalid GCI configuration keyword or parameter */
#define   GCIECGF   9285    /* Cannot get flags               */
#define   GCIEINU   9286    /* Network interface not up          */
#define   GCIENAC   9287    /* Network address conflict          */
#define   GCIECFP   9288    /* Cannot find protocol              */
#define   GCIECCS   9289    /* Cannot create socket              */
#define   GCIECBS   9290    /* Cannot bind socket             */
#define   GCIECLS   9291    /* Cannot listen on socket              */
#define   GCIECAC   9292    /* Cannot accept connection          */
#define   GCIENDL   9293    /* Cannot load any communication DLLs, network may not be installed */
#define   GCIECAS   9294    /* Cannot allocate semaphore         */
#define   GCIE295   9295    /* NOT USED                 */
#define   GCIECIN   9296    /* Cancel invalid for this COM DLL         */
#define   GCIEDAE   9297    /* DEALLOCATE verb failure              */
#define   GCIETEE   9298    /* TPENDED verb failure              */
#define   GCIERWE   9299    /* RECEIVE_AND_WAIT verb failure           */
#define   GCIESDA   9300    /* SEND_DATA verb failure            */
#define   GCIEDNF   9301    /* Input dbname not found in APPC tables      */
#define   GCIENLA   9302    /* No LUs available to start a session        */
#define   GCIEAEE   9303    /* ASCII-to-EBCDIC conversion error        */
#define   GCIETSE   9304    /* TPSTARTED verb failure            */
#define   GCIEALE   9305    /* ALLOCATE verb failure             */
#define   GCIEUAB   9306    /* Unable to allocate input/output buffers       */
#define   GCIECMN   9307    /* Communication Manager not running       */
#define   GCIEFLE   9308    /* FLUSH verb failure             */
#define   GCIESNV   9309    /* Userid/password security not valid      */
#define   GCIELNN   9310    /* Listen name not previously registered      */
#define   GCIELNR   9311    /* Listen name not ready to listen         */
#define   GCIERAE   9312    /* RECEIVE_ALLOCATE verb failure           */
#define   GCIERAC   9313    /* Listen cancelled               */
#define   GCIETLM   9314    /* Tranid, local LU, or modename missing      */
#define   GCIEHNM   9315    /* Serverpath specified, but no hostname      */
#define   GCIESNF   9316    /* Servername not found in server table       */
#define   GCIENSP   9317    /* Function not fully supported         */
#define   GCIETNA   9318    /* Host transaction program not available        */
#define   GCIETNR   9319    /* Host transaction name not recognized       */
#define   GCIEDAP   9320    /* Host transaction abended          */
#define   GCIECSA   9321    /* Communication Manager abended           */
#define   GCIESUS   9322    /* Only one task connection allowed for DBWINDOW */
#define   GCIETPU   9323    /* Transport provider unavailable          */
#define   GCIETAU   9324    /* Transport address unavailable           */
#define   GCIETLF   9325    /* Transport listen failed              */
#define   GCIETAF   9326    /* Transport accept failed              */
#define   GCIEOSE   9327    /* Operating system error            */
#define   GCIEALU   9328    /* ATTACH_LU verb failure            */
#define   GCIEADC   9329    /* ACTIVATE_DLC verb failure         */
#define   GCIECNS   9330    /* CNOS verb failure              */
#define   GCIESNI   9331    /* SPX Not Installed              */
#define   GCIECOS   9332    /* Cannot open socket             */
#define   GCIEILP   9333    /* IPXListenForPacket Failed         */
#define   GCIEISP   9334    /* IPXSendPacket Failed              */
#define   GCIESEC   9335    /* SPXEstablishConnection Failed           */
#define   GCIECNX   9336    /* NDS Context error              */
#define   GCIEOBJ   9337    /* NDS Object error               */
#define   GCIEAXS   9338    /* NDS Access error               */
#define   GCIESCH   9339    /* NDS Class error                */
#define   GRPEBSI   9401    /* CHECK Failure (GRP BSI): <group page corrupted> */
#define   GRPEBFL   9402    /* CHECK Failure (GRP BFL): <group page corrupted> */
#define   GRPEBAP   9403    /* CHECK Failure (GRP BAP): <group page corrupted> */
#define   GRPEBGP   9404    /* CHECK Failure (GRP BGP): <group page corrupted> */
#define   GRPEBPP   9405    /* CHECK Failure (GRP BPP): <group page corrupted> */
#define   GRPEPNA   9406    /* CHECK Failure (GRP PNA): <group page corrupted> */
#define   GRPEBPT   9407    /* bad page type in allocation page        */
#define   GRPECFP   9408    /* cycling free page list            */
#define   SFSEBSI   9501    /* CHECK Failure (SFS BSI): <system free list corrupt> */
#define   SFSEBNP   9502    /* CHECK Failure (SFS BNP): <system free list corrupt> */
#define   SFSEBPT   9503    /* CHECK Failure (SFS BPT): <system free list corrupt> */
#define   IDXEBPT   9601    /* CHECK Failure (IDX BPT): <index page corrupted> */
#define   IDXEBFV   9602    /* CHECK Failure (IDX BFU): <index page corrupted> */
#define   IDXEBLV   9603    /* CHECK Failure (IDX BLV): <index page corrupted> */
#define   IDXEBMK   9604    /* CHECK Failure (IDX BMK): <index page corrupted> */
#define   IDXEBKL   9605    /* CHECK Failure (IDX BKL): <index page corrupted> */
#define   IDXEBEO   9606    /* CHECK Failure (IDX BEO): <index page corrupted> */
#define   IDXEDKU   9607    /* CHECK Failure (IDX DKU): <index page corrupted> */
#define   IDXESDT   9608    /* CHECK Failure (IDX SDT): <index page corrupted> */
#define   IDXEKIF   9609    /* CHECK Failure (IDX KIF): <index page corrupted> */
#define   IDXEWNE   9610    /* CHECK Failure (IDX WNE): <index page corrupted> */
#define   IDXELNS   9611    /* CHECK Failure (IDX LNS): <index page corrupted> */
#define   IDXEBOA   9612    /* CHECK Failure (IDX BOA): <index page corrupted> */
#define   IDXENLC   9613    /* CHECK Failure (IDX NLC): <index page corrupted> */
#define   IDXEIRF   9614    /* Database or object in the database is corrupt (IDX IRF) */
#define   IDXEIDF   9615    /* Database or object in the database is corrupt (IDX IDF) */
#define   IDXERTY   9616    /* Please retry                */
#define   IDXEBKR   9617    /* CHECK Failure (IDX BKR): <index page corrupted> */
#define   CHKEBAS   9701    /* CHECK Failure (CHK BAS): <page has bad allocation status> */
#define   CHKELOP   9702    /* CHECK Failure (CHK LOP): <page has been lost> */
#define   PRSETMT   9801    /* Too many tables to join              */
#define   PRSEIST   9802    /* Invalid statistics type              */
#define   PRSEICO   9803    /* Invalid object to CHECK              */
#define   PRSEMEK   9804    /* Missing EXISTS keyword            */
#define   PRSECNH   9805    /* Only CLUSTERED HASHED indexes supported       */
#define   PRSEMUK   9806    /* Missing DEFAULT keyword              */
#define   PRSEMDB   9807    /* Missing database name             */
#define   PRSEMTS   9808    /* Missing tablespace name              */
#define   PRSEFUU   9809    /* FOR UPDATE OF not allowed with UNION       */
#define   PRSEFUO   9810    /* FOR UPDATE OF not allowed with ORDER BY       */
#define   PRSEFUP   9811    /* Invalid FOR UPDATE OF clause         */
#define   PRSESFU   9812    /* SELECT FOR UPDATE must reference a single table */
#define   PRSEUNA   9813    /* UNION ALL required for all SELECT statements  */
#define   PRSERCT   9814    /* Result column name can only be specified with top select statement */
#define   PRSERCI   9815    /* Result column name must be identifier      */
#define   PRSEETC   9816    /* Expression is too big             */
#define   PRSEMKY   9817    /* Missing KEY keyword            */
#define   PRSEIPC   9818    /* Invalid primary key column name         */
#define   PRSECPK   9819    /* Column already defined for primary key        */
#define   PRSECNN   9820    /* Primary key column must be NOT NULL or NOT NULL WITH DEFAULT */
#define   PRSEPCN   9821    /* Primary key column not defined          */
#define   PRSEIFK   9822    /* Invalid foreign key name          */
#define   PRSEIFC   9823    /* Invalid foreign key column name         */
#define   PRSEFKA   9824    /* Foreign key <name> already defined      */
#define   PRSECFK   9825    /* Column already defined for foreign key        */
#define   PRSEFCN   9826    /* Foreign key column not defined          */
#define   PRSEMRK   9827    /* Missing REFERENCE keyword         */
#define   PRSEIRN   9828    /* Invalid reference table name         */
#define   PRSEMFR   9829    /* Missing FOREIGN keyword              */
#define   PRSEACN   9830    /* At least one column in the foreign key needs to allow NULL value */
#define   PRSESRC   9831    /* Self-reference foreign key must have CASCADE delete rule */
#define   PRSEFDN   9832    /* Foreign key column data type not matching primary key column */
#define   PRSEFNC   9833    /* Foreign key number of columns not matching primary key */
#define   PRSESDN   9834    /* Distinct paths must have same delete rules and cannot be SET NULL */
#define   PRSEIDR   9835    /* Invalid delete rule            */
#define   PRSETKF   9836    /* Too many concatenated key fields        */
#define   PRSEMDL   9837    /* Missing DELETE keyword            */
#define   PRSEPND   9838    /* Parent table not defined yet         */
#define   PRSEPTI   9839    /* Parent table in incomplete state        */
#define   PRSEPKA   9840    /* Primary key already specified           */
#define   PRSEPKN   9841    /* Primary key not defined in parent table       */
#define   PRSEEMP   9842    /* Exceed maximum primary key length       */
#define   PRSEATS   9843    /* Use ALTER TABLE to create self-referencing foreign key */
#define   PRSETDS   9844    /* Add foreign key causes table delete-connected to itself */
#define   PRSESFD   9845    /* Same column list in different foreign keys cannot have same parent */
#define   PRSEFRS   9846    /* Foreign key cannot reference system table     */
#define   PRSEICR   9847    /* Invalid cursor name            */
#define   PRSEOOL   9848    /* Number of operands over system limitation     */
#define   PRSEISV   9849    /* Invalid server command            */
#define   PRSEIDN   9850    /* Invalid database name             */
#define   PRSEIAR   9851    /* Invalid dbarea name <name>           */
#define   PRSEIAF   9852    /* Invalid dbarea file name          */
#define   PRSEMZK   9853    /* Missing SIZE keyword              */
#define   PRSEISG   9854    /* Invalid stogroup name <name>         */
#define   PRSEUSK   9855    /* Missing USING keyword             */
#define   PRSEISK   9856    /* Invalid SET type.              */
#define   PRSESGK   9857    /* Missing STOGROUP Keyword          */
#define   PRSEASO   9858    /* Invalid ALTER STOGROUP option           */
#define   PRSEADO   9859    /* Invalid ALTER DATABASE option           */
#define   PRSEIDE   9860    /* Invalid DELETE command            */
#define   PRSEABO   9861    /* System exit from abort command          */
#define   PRSENDQ   9862    /* DBAREA filename should not be specified in double quotes */
#define   PRSEASL   9863    /* DBAREA size too large             */
#define   PRSEUMI   9864    /* User error number must be greater than 0 and less than 32767 */
#define   PRSEIUC   9865    /* Invalid user error class          */
#define   PRSEMFN   9866    /* Missing foreign key name          */
#define   PRSEMPF   9867    /* Missing FOREIGN or PRIMARY keyword      */
#define   PRSEEFN   9868    /* Foreign key name cannot exceed 18 characters  */
#define   PRSECOU   9869    /* No check option allowed with UNION      */
#define   PRSEVCR   9870    /* View column names required with UNION      */
#define   PRSECNS   9871    /* Command no longer suppered           */
#define   PRSETMB   9872    /* Too many booleans in query           */
#define   PRSEMEX   9873    /* Missing Expression before [NOT] IN predicate  */
#define   PRSEITI   9874    /* Invalid global transaction id           */
#define   PRSEMPT   9875    /* Missing PARTICIPANT keyword          */
#define   PRSEMEN   9876    /* Missing END keyword            */
#define   PRSEDBK   9877    /* Missing DATABASE keyword          */
#define   PRSEMCO   9878    /* Missing COMMA keyword             */
#define   PRSEPTS   9879    /* Missing PARTICIPANTS keyword         */
#define   PRSEBPT   9880    /* Bad protocol name              */
#define   PRSESLK   9881    /* Missing SLASH keyword             */
#define   PRSEIPN   9882    /* Invalid number of participants          */
#define   PRSETMU   9883    /* Too many participants specified         */
#define   PRSENEU   9884    /* Not enough participants specified       */
#define   PRSEOOM   9885    /* Out of memory               */
#define   PRSEBOS   9886    /* Bad option specified              */
#define   PRSEFMB   9887    /* Failure ID must be a positive number non-zero number. */
#define   PRSEEWT   9888    /* Expecting keyword WITH            */
#define   PRSEIET   9889    /* Invalid failure error type           */
#define   PRSECDN   9890    /* Event code  must be a positive non-zero number */
#define   PRSEMIN   9891    /* Missing IN keyword             */
#define   PRSEBES   9892    /* Bad error scope specified.           */
#define   PRSEBEV   9893    /* Bad error event specified.           */
#define   PRSEMCT   9894    /* Missing COUNT keyword             */
#define   PRSEECP   9895    /* EVERY <n-times> must be a positive non-zero number */
#define   PRSEMCS   9896    /* Missing CSV keyword            */
#define   PRSEIDS   9897    /* Invalid DB specification          */
#define   PRSEMBA   9898    /* Missing ON, BEFORE or AFTER keyword.       */
#define   PRSEMAL   9899    /* Missing ALL keyword            */
#define   PRSEMTR   9900    /* Missing TRANSACTIONS keyword         */
#define   PRSEMFE   9901    /* Missing FORCE keyword             */
#define   PRSEISS   9902    /* Invalid SET command            */
#define   PRSEMOT   9903    /* More than one source table specified in the unload/load command */
#define   PRSENTB   9904    /* No table should be specified, if the load format is SQL */
#define   PRSEIFT   9905    /* Invalid load/unload format           */
#define   PRSENTU   9906    /* No table name(s) specified in unload       */
#define   PRSEICP   9907    /* Invalid compress option.          */
#define   PRSEMIA   9908    /* Missing AT keyword             */
#define   PRSEMLN   9909    /* Missing LINE number            */
#define   PRSEMDT   9910    /* Missing destination table name          */
#define   PRSEDVN   9911    /* Duplicate variable name              */
#define   PRSEIVE   9912    /* INTO variable already exists         */
#define   PRSEMLD   9913    /* Missing line delimiter            */
#define   PRSEDPS   9914    /* Duplicate PARAMETERS section         */
#define   PRSENPS   9915    /* PARAMETERS section not allowed here        */
#define   PRSEDLS   9916    /* Duplicate LOCAL section              */
#define   PRSENLS   9917    /* LOCAL section not allowed here          */
#define   PRSEIPI   9918    /* Invalid indentation in PARAMETERS section     */
#define   PRSEDAS   9919    /* Duplicate ACTIONS section         */
#define   PRSEILI   9920    /* Invalid indentation in LOCAL section       */
#define   PRSEIAI   9921    /* Invalid indentation in ACTIONS section        */
#define   PRSEUPC   9922    /* Unknown procedure command         */
#define   PRSEBMF   9923    /* BEGIN must be the first one          */
#define   PRSETMG   9924    /* Too many BEGINs                */
#define   PRSETME   9925    /* Too many ENDs               */
#define   PRSENAG   9926    /* No aggregate function             */
#define   PRSEILN   9927    /* Invalid LOOP name              */
#define   PRSEBVN   9928    /* Bad variable name              */
#define   PRSEMCL   9929    /* Must be a variable             */
#define   PRSEMHP   9930    /* Must have PROCEDURE keyword          */
#define   PRSEUPS   9931    /* Unknown procedure state              */
#define   PRSECFF   9932    /* Cannot find function handler         */
#define   PRSEMVR   9933    /* Missing VARIABLES keyword         */
#define   PRSEBPN   9934    /* Invalid procedure name            */
#define   PRSEMNM   9935    /* Missing NAME keyword              */
#define   PRSEUVN   9936    /* Undefined variable name              */
#define   PRSEBMP   9937    /* BEGIN keyword must be used before END      */
#define   PRSEIPS   9938    /* Procedure section (name) expected       */
#define   PRSEMIF   9939    /* Missing IF keyword             */
#define   PRSEIOU   9940    /* Invalid ON usage               */
#define   PRSEDLN   9941    /* Duplicate loop name            */
#define   PRSEULN   9942    /* Unknown loop name              */
#define   PRSEBBC   9943    /* Bad boolean constant              */
#define   PRSEICI   9944    /* Invalid command indentation          */
#define   PRSEOBS   9945    /* Function <name> is now obsolete         */
#define   PRSEVTT   9946    /* STATIC procedure must be stored first      */
#define   PRSEMSQ   9947    /* Missing SQLERROR keyword          */
#define   PRSEHIH   9948    /* Declare error handler indide an error handler */
#define   PRSENRK   9949    /* No RECEIVE keyword here              */
#define   PRSENSH   9950    /* No SQL HANDLE or FILE HANDLE in PARAMETERS section */
#define   PRSEMDS   9951    /* Missing keyword DYNAMIC or STATIC after IS    */
#define   PRSEDVC   9952    /* Duplicate values correlation name detected    */
#define   PRSEIVT   9953    /* Event name expected            */
#define   PRSEITG   9954    /* Trigger name expected             */
#define   PRSEIVP   9955    /* Procedure name expected              */
#define   PRSEMXK   9956    /* Missing EXECUTE keyword              */
#define   PRSEMIV   9957    /* Number of input values does not match number required by */
#define   PRSEVPP   9958    /* Invalid identifier provided as procedure parameter or part of */
#define   PRSEMRA   9959    /* Missing RAISE keyword             */
#define   PRSEMVY   9960    /* Missing EVERY keyword             */
#define   PRSENBA   9961    /* Need BEFORE or AFTER keyword         */
#define   PRSEMVA   9962    /* Missing event action              */
#define   PRSEIVV   9963    /* Invalid values correlation name         */
#define   PRSEMNO   9964    /* Missing OLD or NEW keyword           */
#define   PRSERED   9965    /* Redundant OLD or NEW keyword         */
#define   PRSEMEA   9966    /* Missing EACH keyword              */
#define   PRSEMRS   9967    /* Missing ROW or STATEMENT keyword        */
#define   PRSEIRF   9968    /* Invalid REFERENCING keyword          */
#define   PRSEBXN   9969    /* Prefixing table name does not match the subject table. */
#define   PRSEBCN   9970    /* Table name does not match OLD or NEW values correlation name */
#define   PRSEIVI   9971    /* Invalid identifier             */
#define   PRSEIAG   9972    /* Invalid aggregate              */
#define   PRSEIFU   9973    /* Invalid function               */
#define   PRSEMPB   9974    /* Missing PROCEDURE keyword         */
#define   PRSEICV   9975    /* Column value not allowed as procedure parameter */
#define   PRSENNE   9976    /* Not enough END keyword            */
#define   PRSEMTP   9977    /* Missing terminating right parenthesis after inline procedure */
#define   PRSEVVN   9978    /* Invalid interval specification          */
#define   PRSEVPM   9979    /* Invalid identifier for procedure parameter    */
#define   PRSEVRI   9980    /* Invalid row id for procedure parameter        */
#define   PRSEMTX   9981    /* Missing TRANSACTION keyword          */
#define   PRSEIEP   9982    /* Invalid grant execute privilege options       */
#define   PRSEMPS   9983    /* Missing PRIVILEGES keyword           */
#define   PRSEITC   9984    /* Invalid audit category            */
#define   PRSESSC   9985    /* Administrative server connection required     */
#define   PRSESAS   9986    /* Invalid START AUDIT SIZE value          */
#define   PRSEIAP   9987    /* Invalid APPEND file number           */
#define   PRSEIKV   9988    /* Invalid KEEP value             */
#define   PRSEDFN   9989    /* Invalid AUDIT directory name         */
#define   PRSEMAI   9990    /* Missing AUDIT name identifier           */
#define   PRSEMSG   9991    /* MESSAGE keyword missing or misspelled      */
#define   PRSEDMM   9992    /* Datatype mismatch.             */
#define   PRSERNA   9993    /* REFERENCING clause not allowed in FOR EACH STATEMENT trigger. */
#define   PRSEARC   9994    /* Keyword AUTORECOMPILE is expected       */
#define   PRSEOPT   9995    /* Keyword ON or OFF is expected           */
#define   PRSEMBS   9996    /* Keyword STATIC is expected           */
#define   PRSEVOT   9997    /* Future time expected for a one-time event     */
#define   PRSEVVE   9998    /* Time element cannot be an expression       */
#define   PRSEVCN   9999    /* Invalid command name              */
#define   PRSESET   10000   /* Keyword SET is expected              */
#define   MTHE001   10001   /* MATH LIB: argument domain error (DOMAIN)      */
#define   MTHE002   10002   /* MATH LIB: argument singularity error (SING)   */
#define   MTHE003   10003   /* MATH LIB: overflow range error (OVERFLOW)     */
#define   MTHE004   10004   /* MATH LIB: underflow range error (UNDERFLOW)   */
#define   MTHE005   10005   /* MATH LIB: total loss of precision (TLOSS)     */
#define   MTHE006   10006   /* MATH LIB: partial loss of precision (PLOSS)   */
#define   DSMEIMT   10101   /* INTERNAL USE ONLY - Invalid message type      */
#define   DSMEMNF   10102   /* INTERNAL USE ONLY - Message text not found    */
#define   DSMEOOM   10103   /* Out of memory at the database computer (DSM OOM) */
#define   KBDEKOR   10201   /* INTERNAL USE ONLY              */
#define   MCIEIES   10301   /* INTERNAL USE ONLY - invalid export size       */
#define   MCIEIIS   10302   /* INTERNAL USE ONLY - invalid import size       */
#define   MCIEIAT   10303   /* Invalid architecture type         */
#define   SQLENLD   10401   /* No local databases             */
#define   SQLEINM   10402   /* Invalid cursor name            */
#define   SQLECAO   10403   /* Out of memory at the client (SQL CAO)      */
#define   SQLEMSE   10404   /* Describe must follow a successful Select Execute */
#define   SQLENDE   10405   /* No Describe information is available for this cursor */
#define   SQLEBCS   10407   /* Cannot execute a chained cmd with SELECT in bulk execute mode */
#define   SQLEFCH   10408   /* Fetch backwards not allowed with chained commands */
#define   SQLEIBF   10409   /* Invalid backup file            */
#define   SQLENEU   10410   /* Not a recently executed UPDATE          */
#define   SQLENFQ   10411   /* No fully qualified name              */
#define   SQLERSV   10412   /* NOT USED                 */
#define   SQLELIP   10413   /* 003 - Fatal SQLBase System Failure (SQL LIP)  */
#define   SQLECAQ   10414   /* Out of memory at the client (SQL CAQ)      */
#define   SQLEANT   10415   /* API nested too deep.              */
#define   SQLESTO   10416   /* Out of stack space - command rolled back.     */
#define   SQLEACN   10417   /* Already connected in non-distributed transaction mode. */
#define   SQLEACD   10418   /* Already connected in distributed transaction mode. */
#define   SQLECRD   10419   /* Cannot explicitly turn off distributed transaction mode */
#define   SQLECSD   10420   /* Cannot start distributed transaction.      */
#define   SQLECOF   10421   /* Cannot open Load or Unload file         */
#define   SQLEFAE   10422   /* Load or unload file already already exists    */
#define   SQLENNB   10423   /* No name buffer specified          */
#define   SQLEIVN   10424   /* Invalid number of INTO variables        */
#define   SQLECAX   10425   /* 008 - Out of memory at the database computer (SQL CAX) */
#define   SQLEFNP   10426   /* Feature not allowed on a procedure cursor     */
#define   SQLEITH   10427   /* Invalid task handle            */
#define   SQLESNE   10428   /* SQLTrace not enabled              */
#define   SQLECNL   10429   /* Invalid Client name            */
#define   SQLEMID   10430   /* Invalid Mail Id                */
#define   SQLENID   10431   /* Invalid Network Id             */
#define   SQLEAID   10432   /* Invalid Adapter Id             */
#define   SQLEUID   10433   /* Invalid Application Id            */
#define   SQLEIPN   10434   /* Invalid process number            */
#define   SQLEBEV   10435   /* (Incompatible) server does not support attempted function */
#define   SQLENPR   10436   /* No permission to access remote files       */
#define   SQLETNS   10437   /* SQLTrace not supported on this platform       */
#define   SQLENDT   10438   /* Distributed transactions are not supported by this back-end */
#define   SQLEDTR   10439   /* Connection Handles are not supported in Distrubuted Transaction mode. */
#define   SQLESDT   10440   /* Distributed Transaction cannot be set when Connection Hanldes   exist. */
#define   SQLESIZ   10441   /* Restore is over maximum file size (SQL SIZ)   */
#define   NUMEBNP   10501   /* Pointer to number is bad          */
#define   NUMENLS   10502   /* Length of the number is too small (less than or equal to zero) */
#define   NUMENLL   10503   /* Length of the number is too large (greater than 12) */
#define   NUMEETS   10504   /* Exponent of the number is too small        */
#define   NUMEETL   10505   /* Exponent of the number is too large        */
#define   NUMEMNM   10506   /* Number is missing negative marker       */
#define   NUMEBNM   10507   /* Number has bad negative marker          */
#define   NUMEDTS   10508   /* Digit of the number is too small        */
#define   NUMEDTL   10509   /* Digit of the number is too large        */
#define   MDBENSC   10601   /* INTERNAL USE ONLY - No saved context for restore */
#define   MDBECOI   10602   /* Cannot open MAIN.INI              */
#define   MDBEXXX   10603   /* Unused                   */
#define   MDBEUME   10604   /* User cannot name databases MAIN, please rename this database */
#define   MDBEMMV   10605   /* SQLBase and MAIN.INI have mismatching versions */
#define   MDBEICT   10606   /* 003 - Fatal SQLBase System Failure (MDB ICT)  */
#define   MDBEIDT   10607   /* 003 - Fatal SQLBase System Failure (MDB IDT)  */
#define   PFSENEF   10701   /* NOT USED                 */
#define   PFSEIFH   10702   /* INTERNAL USE ONLY - Invalid file handle       */
#define   PFSEIEN   10703   /* Invalid extent number in MAIN database        */
#define   PFSEIAI   10704   /* NOT USED                 */
#define   PFSEIPE   10705   /* INTERNAL USE ONLY - I/O attempted past end-of-file */
#define   PFSECOV   10706   /* Out of memory at the database computer (PFS COV) */
#define   PFSECOD   10707   /* Out of memory at the database computer (PFS COD) */
#define   PFSECFV   10708   /* Out of memory at the database computer (PFS CFV) */
#define   PFSECFD   10709   /* Out of memory at the database computer (PFS CFD) */
#define   PFSECAV   10710   /* Out of memory at the database computer (PFS CAV) */
#define   PFSECAD   10711   /* Out of memory at the database computer (PFS CAD) */
#define   PFSECOA   10712   /* Cannot open area file for MAIN database       */
#define   PFSECEM   10713   /* Out of memory at the database computer        */
#define   PFSE002   10714   /* NOT USED                 */
#define   PFSECEF   10715   /* Cannot extend partitioned file          */
#define   PFSECEE   10716   /* Out of memory at the database computer (PFS CEE) */
#define   PFSENPD   10717   /* INTERNAL USE ONLY - Not partitioned database  */
#define   PFSE001   10718   /* NOT USED                 */
#define   PFSEFDE   10719   /* File does not exist for MAIN database      */
#define   PFSECAI   10720   /* Cannot allocate initial extent          */
#define   PFSECDO   10721   /* INTERNAL USE ONLY - Cannot delete open file   */
#define   PFSECEV   10722   /* Out of memory at the database computer (PFS CEV) */
#define   PFSECOR   10723   /* Main database tables are corrupt        */
#define   PFSEMDC   10724   /* Incorrect extent table entries in the main database. */
#define   SVCECAT   10801   /* Out of memory at the database computer        */
#define   SVCEVNM   10802   /* Main db set up improperly (stored cmd variables not matched) */
#define   SVCESNM   10803   /* Main db set up improperly (select col not matched w/stored cmd) */
#define   SVCEDAN   10804   /* DBAREA <name> already exists or DBAREA not found */
#define   SVCEDSG   10805   /* Stogroup name <name> already exists        */
#define   SVCEDSA   10806   /* DBAREA <name> already in stogroup       */
#define   SVCENPD   10807   /* Not partitioned database, command not allowed */
#define   SVCESCD   10808   /* Specified DBAREA is associated with stogroup  */
#define   SVCEECD   10809   /* Specified DBAREA is in use           */
#define   SVCEDCS   10810   /* Specified stogroup has been assigned to the database */
#define   SVCEFCS   10811   /* Specified stogroup is the default stogroup    */
#define   SVCEANF   10812   /* DBAREA <name> not found              */
#define   SVCEDNF   10813   /* Database not found             */
#define   SVCEDAE   10814   /* Database <name> already exists          */
#define   SVCEIRS   10815   /* Insufficient raw device space           */
#define   SVCEIFS   10816   /* Insufficient file space              */
#define   SVCEFAE   10817   /* File already exists            */
#define   SVCEFCE   10818   /* File <filename> Creation Failure        */
#define   SVCEIOE   10819   /* Disk Write Failure             */
#define   SVCECCD   10820   /* INTERNAL USE ONLY              */
#define   SVCEAAE   10821   /* Auditname <name> already exists         */
#define   SVCEFNF   10822   /* File associated with DBAREA not found      */
#define   SVCEMNE   10823   /* MAIN database does not exist         */
#define   SVCECCM   10824   /* Cannot create Main database          */
#define   SVCESGN   10825   /* Stogroup name <name> not found          */
#define   SVCEDSO   10826   /* Data stored outside specified area      */
#define   SVCEANS   10827   /* DBAREA <name> is not in stogroup <name>       */
#define   SVCEDFE   10828   /* Delete file error              */
#define   SVCEIRF   10829   /* Initialization reading failure          */
#define   SVCEIWF   10830   /* Initialization writing failure          */
#define   SVCEDNA   10831   /* Specified DBAREA is not allocable (internal error) */
#define   SVCEMTF   10832   /* Cannot start another audit, maximum audit files exceeded */
#define   SVCEMZK   10833   /* Must have SIZE keyword            */
#define   SVCENOP   10834   /* MAIN database not open to public        */
#define   SVCETNF   10835   /* Audit does not exist              */
#define   SVCEUSC   10836   /* Unknown server command            */
#define   SVCEDNE   10837   /* Database <name> does not exist          */
#define   SVCEPDE   10838   /* Partitioned database is already enabled       */
#define   SVCEPDD   10839   /* Partitioned database is already disabled      */
#define   SVCEPIU   10840   /* Partitioned database in use          */
#define   SVCEIGP   10841   /* Illegal server get parameter         */
#define   SVCECSA   10842   /* 001 - Undocumented             */
#define   SVCECSD   10843   /* Commit service is already disabled      */
#define   SVCECSE   10844   /* Commit service is already enabled       */
#define   SVCEATE   10845   /* AUDIT file <filename> does not exist       */
#define   SVCECOF   10846   /* Cannot open AUDIT file <filename>       */
#define   SVCEAAA   10847   /* AUDIT <auditname> already active        */
#define   SVCEAFE   10848   /* Audit file <filename> already exists       */
#define   SVCECCT   10849   /* Unable to create temporary file (failing MAIN database creation) */
#define   SVCEUCM   10850   /* Unable to connect to the MAIN database        */
#define   SVCEISC   10851   /* Invalid SQL statement detected (failing MAIN database creation) */
#define   MESECLM   10901   /* Cannot locate message file           */
#define   MESECOM   10902   /* Cannot open message file          */
#define   MESEIXC   10903   /* Bad message file: invalid X-coordinate        */
#define   MESEIYC   10904   /* Bad message file: invalid Y-coordinate        */
#define   MESEIMP   10905   /* Bad message file: invalid message parameter   */
#define   MESEISV   10906   /* INTERNAL USE ONLY - Bad message file: invalid state value */
#define   MESEIMI   10907   /* Bad message file: invalid message id       */
#define   MESEMMC   10908   /* Bad message file: missing message comment     */
#define   MESECAM   10909   /* Cannot allocate message table           */
#define   MESEEOF   10910   /* Bad message file: unexpected EOF        */
#define   MESEMEQ   10911   /* Bad message file: missing end quote        */
#define   MESEEEC   10912   /* Bad message file: expected end of comment     */
#define   MESEIES   10913   /* Bad message file: invalid escape sequence     */
#define   MESEMMS   10914   /* Bad message file: missing message string      */
#define   MESEMMV   10915   /* SQLBase and MESSAGE.SQL file have mismatching versions */
#define   MESEAIN   10916   /* Message table already initialised       */
#define   HSHECSS   11001   /* Out of memory at the database computer.       */
#define   APAECNA   11101   /* Out of memory at the database computer        */
#define   APAEOOM   11102   /* Out of memory at the database computer        */
#define   APAETMB   11103   /* Too many booleans in query           */
#define   APAECNC   11104   /* Cannot create PLAN_TABLE due to error code: <database sql error code> */
#define   APAECNU   11105   /* Cannot update PLAN_TABLE due to error code: <database sql error code> */
#define   GFSEICF   11201   /* Fatal SQLBase System Failure (GFS ICF)        */
#define   GFSEIVF   11202   /* Fatal SQLBase System Failure (GFS IVF)        */
#define   GFSECAV   11203   /* Out of memory at the database computer        */
#define   GFSECAD   11204   /* Out of memory at the database computer        */
#define   GFSEMAX   11205   /* Attempt to extend file beyond 2 gigabytes     */
#define   GFSELCM   11206   /* Log Checksum Mismatch (GFS LCM)         */
#define   CACECNE   11301   /* Command not properly ended           */
#define   CACEICC   11302   /* Invalid catalog command              */
#define   CACEMTB   11303   /* Missing TBNAME keyword            */
#define   CACEUIO   11304   /* Unknown input option              */
#define   CACEATY   11305   /* AUTHORIRY keyword has been specified       */
#define   CACECTR   11306   /* CREATOR keyword has been specified      */
#define   CACEGEE   11307   /* GRANTEE keyword has been specified      */
#define   CACEQNS   11308   /* QUALIFIER not supported              */
#define   CACETYP   11309   /* TYPE keyword has been specified         */
#define   CACEUTT   11310   /* Unknown table type             */
#define   CACEUIT   11311   /* Unknown index type             */
#define   CACEUOO   11312   /* Unknown output option             */
#define   CACEOOM   11313   /* Out of memory at the database computer (CAC OOM) */
#define   CACEUAO   11314   /* Unknown authority option          */
#define   CACEUOC   11315   /* Unknown ORDER BY column              */
#define   CACESEL   11316   /* Output option is specified more than once     */
#define   CACECNS   11317   /* Command not supported             */
#define   CACEMMI   11318   /* Missing mandatory input parameter       */
#define   CACEUCT   11319   /* Unknown column type            */
#define   CACEITB   11320   /* Invalid character in TBNAME          */
#define   CACEMCK   11321   /* Missing comma               */
#define   CACEMAO   11322   /* Missing authority option          */
#define   CACENSC   11323   /* No select column               */
#define   CACENOB   11324   /* No ORDER BY column             */
#define   CACEWNA   11325   /* Wild card characters not allowed here      */
#define   CACETBN   11326   /* TBNAME keyword has been specified       */
#define   CACEIXN   11327   /* IXNAME keyword has been specified       */
#define   CACESYN   11328   /* SYNNAME keyword has been specified      */
#define   CACESCR   11329   /* SYNCREATOR keyword has been specified      */
#define   CACEOCS   11330   /* ORDER BY column not in select list      */
#define   CACEMEL   11331   /* Missing equality sign or LIKE keyword      */
#define   DFSEEOF   11401   /* Cannot find End of File for Direct File System file */
#define   DFSEVMI   11402   /* Cannot get volume mapping info for Direct File System file */
#define   DFSEVMF   11403   /* Volume is full for Direct File System file    */
#define   DFSEEXP   11404   /* Cannot expand Direct File System file      */
#define   DFSEOSP   11405   /* Out of space for Direct File System file      */
#define   DFSEOOM   11406   /* Out of memory at the database computer        */
#define   TMPEOOM   11501   /* Out of memory in temporary file manipulation  */
#define   TMPEOVF   11502   /* Temporary-file use count overflow       */
#define   TMPEUND   11503   /* Temporary-file use count underflow      */
#define   DBOECTL   11601   /* Exceeded command time limit.         */
#define   DBOEICN   11602   /* Invalid client name            */
#define   DBOEIAC   11603   /* Invalid admin command             */
#define   DBOEDRO   11604   /* Distributed read-only transaction not currently supported */
#define   DBOESCT   11605   /* Stored command text not available       */
#define   DBOEAWS   11606   /* Invalid OS Averaging window size.       */
#define   DBOECDR   11607   /* Selected cursor is not active.          */
#define   DBOEIMI   11608   /* Invalid mail id parameter         */
#define   DBOEINI   11609   /* Invalid network id parameter         */
#define   DBOEIAI   11610   /* Invalid adapter id parameter         */
#define   DBOEIUI   11611   /* Invalid application id parameter        */
#define   DBOEOSR   11612   /* Invalid OS sample rate.              */
#define   DBOEUSD   11613   /* Unable to shutdown database.         */
#define   DBOETPD   11614   /* Too many processes on the 6.0 database server. */
#define   DBOECVS   11615   /* Command or operation is valid for multi-user servers only */
#define   DBOECNS   11616   /* NOT USED                 */
#define   DBOESPE   11617   /* Stored Procedure already exists         */
#define   DBOEEFE   11618   /* External function <name> already exists.      */
#define   PRSENKC   11701   /* This is not a known update statistics column. */
#define   PRSEQNA   11702   /* Qualified names are not permitted in update statistics. */
#define   PRSEIOV   11703   /* Invalid OVERWRITE option          */
#define   PRSEICF   11704   /* Invalid CONTROL option            */
#define   PRSEIUO   11704   /* Invalid UNLOAD option             */
#define   PRSEINT   11705   /* Invalid INTO clause            */
#define   PRSEXDL   11706   /* Invalid object to LOCK or UNLOCK        */
#define   PRSEISF   11707   /* Invalid SqlWindows function          */
#define   PRSENFN   11708   /* No load/unload filename has been specified.   */
#define   PRSENOV   11709   /* No output variables for FETCH section      */
#define   PRSERVM   11710   /* Return value missing              */
#define   PRSESFL   11711   /* Unload/load file name and Message Log file name are same. */
#define   PRSENLF   11712   /* No Load/Unload Message Log filename has been specified. */
#define   PRSESND   11713   /* START AT clause is not supported for DIF format. */
#define   PRSEMLR   11714   /* Missing the keyword REFERENCING or left parenthesis */
#define   PRSENSP   11715   /* Database Events are not supported in this version. */
#define   PRSENUL   11716   /* Empty message received by the parser.      */
#define   PRSEIEV   11717   /* Event name expected            */
#define   PRSEEFK   11718   /* Expected FUNCTION keyword         */
#define   PRSEIRT   11719   /* Invalid return data type          */
#define   PRSEEDT   11720   /* Invalid or missing external data type      */
#define   PRSEUDT   11721   /* NOT USED                 */
#define   PRSEBFN   11722   /* Invalid or missing function name        */
#define   PRSEMMC   11723   /* Missing expected colon (:)           */
#define   PRSEBLN   11724   /* Missing or invalid library name         */
#define   PRSEMLC   11725   /* Missing library name clause          */
#define   PRSEMMN   11726   /* Missing or misspelled NAME keyword      */
#define   PRSEBFI   11727   /* Invalid function name             */
#define   PRSEMMO   11728   /* Missing or misspelled ORDINAL keyword      */
#define   PRSEION   11729   /* Missing or invalid ordinal number       */
#define   PRSEMPW   11730   /* Missing or misspelled PROCESS keyword      */
#define   PRSEMTW   11731   /* Missing or misspelled THREAD keyword       */
#define   PRSEESS   11732   /* Expected SEPARATE or SAME keyword       */
#define   PRSEEXF   11733   /* External functions are not supported in this environment */
#define   PRSEIEF   11734   /* Disallowed name <name> used as external function name */
#define   PRSEIQC   11735   /* Invalid Sequence Column format          */
#define   PRSENAS   11736   /* No Attributes Specified              */
#define   PRSEIDA   11737   /* Invalid Database Attribute specified.      */
#define   PRSEIDV   11738   /* Invalid Database Attribute Value.       */
#define   PRSEIAT   11739   /* Invalid Alter Trigger option         */
#define   PRSEEFT   11740   /* Expecting the keywords EXTERNAL FUNCTION or TABLE. */
#define   PRSEICT   11741   /* Invalid CALLSTYLE option.         */
#define   PRSENOA   11742   /* Nothing altered                */
#define   PRSETOR   11743   /* Create Trigger ORDER value invalid      */
#define   PRSELTL   11744   /* Identifier too long            */
#define   PRSESPN   11745   /* Separate process execution not allowed in the 16 bit servers. */
#define   PRSEPCT   11746   /* Primary key column's datatype cannot be a LONG VARCHAR */
#define   PRSEMST   11747   /* Expecting STRING keyword.         */
#define   PRSEIDT   11748   /* Invalid or missing internal data type      */
#define   PSTE001   11901   /* Not enough available stack space for psort    */
#define   PSTE002   11902   /* Stack overflow in sort            */
#define   PSTE003   11903   /* Segment length too large          */
#define   PSTE004   11904   /* Attempt to send record to sort already terminated */
#define   PSTE005   11905   /* Insufficient memory available           */
#define   PSTE006   11906   /* Allocation of psort data stack failed      */
#define   PSTE007   11907   /* Allocation of sort work area failed        */
#define   PSTE008   11908   /* Space exhausted while writing work file       */
#define   PSTE010   11910   /* Error in file read             */
#define   PSTE011   11911   /* Temporary file name too long         */
#define   PSTE012   11912   /* Stack overflow in calling psort         */
#define   PSTE015   11915   /* Programming error #2 in pstsio.c        */
#define   PSTE017   11917   /* Cannot allocate memory for sorting      */
#define   PSTE019   11919   /* Couldn't get memory for workfile input buffer */
#define   PSTE020   11920   /* Couldn't get memory for workfile output buffer */
#define   PSTE022   11922   /* Couldn't open working file for writing        */
#define   PSTE023   11923   /* Couldn't open working file for reading        */
#define   CFOEMSK   12001   /* Missing SIZE keyword              */
#define   CFOEMSZ   12002   /* Missing/Inappropriate SIZE value        */
#define   CFOEMDK   12003   /* Missing DIR keyword            */
#define   CFOEMDR   12004   /* Missing DIR value              */
#define   CFOEFAE   12005   /* Segment File already exists.         */
#define   CFOEOPE   12006   /* Error Opening load/unload or backup/restore segment file. */
#define   CFOELTL   12007   /* Line Too Long               */
#define   CFOEMPK   12008   /* Missing FILEPREFIX keyword           */
#define   CFOEMPX   12009   /* Missing FILEPREFIX value          */
#define   CFOENFS   12010   /* Not enough file segments          */
#define   CFOECAM   12011   /* Cannot allocate memory            */
#define   CFOEDBS   12012   /* Cannot get database size for testing       */
#define   GBSE001   12101   /* Couldn't get memory for file structure        */
#define   GBSE002   12102   /* Couldn't get memory for file buffer        */
#define   GBSE003   12103   /* Seek from end of file not supported        */
#define   TRCECCF   12201   /* Cannot create file, <filename>          */
#define   DMNECAD   12301   /* 008 - Out of memory at the database computer (DMN CAD) */
#define   DMNEBIP   12302   /* Internal error: Invalid interval parameter passed in to Daemon */
#define   TLEEREG   12401   /* Could Not Provide Registration Handle      */
#define   TLEELNK   12402   /* Register Could Not Allocate Link Structure    */
#define   TLEETSK   12403   /* Register Could Not Allocate Task Structure    */
#define   TLEETLK   12404   /* Register Could Not Allocate Talk Structure    */
#define   TLEECUV   12405   /* Register Could Not Allocate Cursor Vector     */
#define   TLEECMD   12406   /* Register Could Not Allocate Command Buffer    */
#define   TLEECIS   12407   /* Could Not Allocate Connect Info Sturcture     */
#define   TLEESIS   12408   /* Could Not Allocate Session Info Structure     */
#define   TLEEWKB   12409   /* Register Could Not Allocate Work Buffer       */
#define   TLEE010   12410   /*                                                                            /*   MsgTxt:
 */
#define   TLEETNR   12411   /* Task Not Registered To The Engine       */
#define   TLEETIH   12412   /* Task Invalid Handle            */
#define   TLEETRE   12413   /* Task Reentrancy Error             */
#define   TLEEHRE   12414   /* Handle Reentrancy Error              */
#define   TLEESPE   12415   /* Spool File Exists, Instructed Not To Create   */
#define   TLEEASE   12416   /* Allocated Space Error - Not Enough To Perform Operation */
#define   TLEEFTS   12417   /* Failed To Set               */
#define   TLEEBNL   12418   /* Block Not Large Enough to Hold Data        */
#define   TLEEBTL   12419   /* Block Too Large. Incorrect Size.        */
#define   TLEE020   12420   /*                                                                            /*   MsgTxt:
 */
#define   TLEECNA   12421   /* Connection Not Available          */
#define   TLEEOPF   12422   /* Error Openeing Output File           */
#define   TLEEAFB   12423   /* Error Allocation Fetch Buffer           */
#define   TLEEAWB   12424   /* Error Allocation Work Buffer         */
#define   TLEEUAB   12425   /* Unable To Allocate Buffer         */
#define   TLEEBNA   12426   /* Buffer Not Available              */
#define   TLEECNR   12427   /* Callback not Registered              */
#define   TLEESTS   12428   /* Size Too Small                 */
#define   SPEECLL   12501   /* Cannot load libarary <name>          */
#define   SPEEGPA   12502   /* Cannot get address for external function <name> */
#define   SPEERTL   12503   /* Returned string too long.         */
#define   SPEEEFE   12504   /* Error number <number> occurred in external function <name>. */
#define   SPEERSL   12505   /* Receive string too long.          */
#define   SEQEUSA   12601   /* Uninitialized Sequence Access.          */
#define   SEQEOOM   12602   /* Out Of Memory at the database computer.       */
#define   MTSETGV   12701   /* Failure on getting Thread Local Storage.      */
#define   MTSETSV   12702   /* Failure on setting Thread Local Storage.      */
#define   MTSECSM   12703   /* Out of memory at the client (MTS CSM)      */
#define   NDSENOE   12801   /* Cannot find user object in Directory Schema   */
#define   NDSENOA   12802   /* Cannot find attribute definition in Directory Schema. */
#define   NDSENOC   12803   /* Cannot find class definition in Directory Schema. */
#define   NDSEAEX   12804   /* Attribute definition already exists in Directory Schema. */
#define   NDSECEX   12805   /* Class definition already exists in Directory Schema. */
#define   NDSEOOM   12806   /* Ran out of memory to allocate buffer for Dir. Services operation. */
#define   NDSEINP   12807   /* Invalid password.              */
#define   NDSENOR   12808   /* No rights to define new atributes or classes in Schema. */
#define   NDSEERR   12809   /* Directory Services returned error.      */
#define   NDSEECC   12810   /* Directory Services Error Context Creation.    */
#define   EXFECLL   12901   /* Load Library of <name> failed with error <error>. */
#define   EXFEGPA   12902   /* Cannot get address for external function <name> */
#define   EXFERTL   12903   /* Returned/Receive string too long.       */
#define   EXFEEFE   12904   /* Error number <number> occurred in external function <name>. */
#define   EXFECAH   12905   /* Could not allocate HSTRING datatype.       */
#define   EXFEDND   12906   /* External function data does not meet the DATE/TIME format */
#define   EXFECFL   12907   /* Free Libarary of <name> failed with error <error>. */
#define   EXFECAO   12908   /* Cannot get address for external function ordinal <number> in DLL name <name> */
#define   EXFEUEE   12909   /* Unhandled exception "<code>" encountered in the function <name> */
#define   EXFECAM   12910   /* EFHost couldn't allocate memory for the function <name>. OS error code is <code> */
#define   EXFERME   12911   /* Receive memory exceeded the limit <limit> for the function <name> */
#define   EXFECSS   12912   /* INTERNAL USE ONLY -- Error in computing shared memory size */
#define   EXFECNS   12913   /* INTERNAL USE ONLY -- Could not allocate internal semaphore */
#define   EXFEECD   12914   /* EFDaemon came down while executing the function <name> */
#define   EXFECDS   12915   /* Couldn't deallocate memory just before an EF execution. OS error code is <code> */
#define   EXFECAS   12916   /* Couldn't allocate memory just before an EF execution. OS error code is <code> */
#define   EXFECFD   12917   /* Couldn't find the executable <name>        */
#define   EXFEFSD   12918   /* Failed to start <name>            */
#define   EXFEFSE   12919   /* Failed to start EFDaemon. OS error code is <code> */
#define   EXFECMR   12920   /* Couldn't map receive parameter shared memory. Error code is <code> */
#define   EXFEURP   12921   /* Unhandled exception "<code>" encountered in receive processing of  the  function <name> */
#define   EXFEIER   12922   /* Internal error <code> encountered in receive processing of  the  function <name> */
#define   EXFECGA   12923   /* Cannot get address for a routine in SWINDOW DLL */
#define   EXFECFH   12924   /* INTERNAL USE ONLY -- Could not find the handle to EFDaemon */
#define   EXFECLS   12925   /* Could not load SWINDOW DLL           */
#define   EXFEDOV   12926   /* A receive parameter overran other parameter(s) in function <name> */
#define   EXFEDNI   12927   /* A datatype <value> is not implemented in the function <name> */
#define   EXFEEDC   12928   /* External function daemon crashed while executing the function <name> */
#define   EXFEFED   12929   /* Fatal error <code> encountered while communicating with the external function daemon */
#define   EXFEHFB   12930   /* EFHost.EXE file format is bad; fuction name is <name> */
#define   EXFEHNF   12931   /* EFHost.EXE file could not be found; fuction name is <name> */
#define   EXFEEOE   12932   /* External function daemon reported OS error code <code>; function name is <name> */
#define   EXFEUCD   12933   /* Unhandled exception "<code>" encountered during execution of a function in Centura DLL */
#define   EXFENNA   12934   /* NULL value being passed for a numeric datatype in <name>. */
