-- Indexes for Entity - AccessAuthority 
DROP INDEX ACCESSAUTHORITY_ID_ACCESSAUTHORIT ON ACCESSAUTHORITY ;
-- Index for Relationship - 'z_USER(has [0:m] ) ACCESSAUTHORITY' 
DROP INDEX ACCESSAUTHORITY_FK_ZKEY01 ON ACCESSAUTHORITY ;
-- Index for Relationship - 'CPLR(has [0:M] ) ACCESSAUTHORITY' 
DROP INDEX ACCESSAUTHORITY_FK_ZKEY0202 ON ACCESSAUTHORITY ;

-- Indexes for Entity - AuditTrail 
DROP INDEX AUDITTRAIL_ID ON AUDITTRAIL ;
-- Index for Relationship - 'REVEVENT(for [0:m] ) AUDITTRAIL' 
DROP INDEX AUDITTRAIL_FK_TYPE01 ON AUDITTRAIL ;
-- Index for Relationship - 'REPOSITORYCLIENT(may have [0:m] ) AUDITTRAIL' 
DROP INDEX AUDITTRAIL_FK_ZKEY02 ON AUDITTRAIL ;
-- Index for Relationship - 'REVSUBEVENT(for [0:m] ) AUDITTRAIL' 
DROP INDEX AUDITTRAIL_FK_SUBTYPE03 ON AUDITTRAIL ;
-- Index for Relationship - 'REVSUBEVENT(for [0:m] ) AUDITTRAIL' 
DROP INDEX AUDITTRAIL_FK_TYPE0204 ON AUDITTRAIL ;
-- Index for Relationship - 'CPLR(has [0:m] ) AUDITTRAIL' 
DROP INDEX AUDITTRAIL_FK_ZKEY0205 ON AUDITTRAIL ;

-- Indexes for Entity - AuditTrailMeta 
DROP INDEX AUDITTRAILMETA_ID ON AUDITTRAILMETA ;
-- Index for Relationship - 'Z_METADEF(has [0:m] ) AUDITTRAILMETA' 
DROP INDEX AUDITTRAILMETA_FK_ZKEY01 ON AUDITTRAILMETA ;
-- Index for Relationship - 'AUDITTRAIL(has [0:m] ) AUDITTRAILMETA' 
DROP INDEX AUDITTRAILMETA_FK_TIMESTAMP02 ON AUDITTRAILMETA ;
-- Index for Relationship - 'AUDITTRAIL(has [0:m] ) AUDITTRAILMETA' 
DROP INDEX AUDITTRAILMETA_FK_ZKEY0203 ON AUDITTRAILMETA ;

-- Indexes for Entity - Buffer 
DROP INDEX BUFFER_ID_BUFFER ON BUFFER ;
-- Index for Relationship - 'z_FILE(has [0:m] ) BUFFER' 
DROP INDEX BUFFER_FK_ZKEY01 ON BUFFER ;

-- Indexes for Entity - CPL 
DROP INDEX CPL_ID_CPL ON CPL ;
-- Index for Relationship - 'z_USER(may administer [0:m] ) CPL' 
DROP INDEX CPL_FK_ZKEY01 ON CPL ;
-- Index for Relationship - 'INSTALLATION(contains [0:m] ) CPL' 
DROP INDEX CPL_FK_ZKEY0202 ON CPL ;

-- Indexes for Entity - CPL_ReuseableItem 
DROP INDEX CPL_REUSEABLEITEMIDCPLREUSEABLEIT ON CPL_REUSEABLEITEM ;

-- Indexes for Entity - CPLR 
DROP INDEX CPLR_ID_CPLR ON CPLR ;
-- Index for Relationship - 'CPL(has [1:M] ) CPLR' 
DROP INDEX CPLR_FK_ZKEY01 ON CPLR ;

-- Indexes for Entity - CPLR_Enqueue 
DROP INDEX CPLR_ENQUEUE_CPLR_MAYHAVEA ON CPLR_ENQUEUE ;
-- Index for Relationship - 'CPLR(may have a [0:1] ) CPLR_ENQUEUE' 
DROP INDEX CPLR_ENQUEUE_FK_ZKEY01 ON CPLR_ENQUEUE ;
-- Index for Relationship - 'REPOSITORYCLIENT(has enqueue for [0:1] ) CPLR_ENQUEUE' 
DROP INDEX CPLR_ENQUEUE_FK_ZKEY0202 ON CPLR_ENQUEUE ;

-- Indexes for Entity - CtrlProperty 
DROP INDEX CTRLPROPERTY_ZKEY ON CTRLPROPERTY ;

-- Indexes for Entity - CtrlPropertyAttribute 
DROP INDEX CTRLPROPERTYATTRIBUTE_ZKEY ON CTRLPROPERTYATTRIBUTE ;
-- Index for Relationship - 'CTRLPROPERTY(has [0:m] ) CTRLPROPERTYATTRIBUTE' 
DROP INDEX CTRLPROPERTYATTRIBUTE_FK_ZKEY01 ON CTRLPROPERTYATTRIBUTE ;

-- Indexes for Entity - File 
DROP INDEX z_FILE_ID_FILE ON z_FILE ;
-- Index for Relationship - 'CPLR(has [0:m] ) z_FILE' 
DROP INDEX z_FILE_FK_ZKEY01 ON z_FILE ;

-- Indexes for Entity - Installation 
DROP INDEX INSTALLATION_ID_INSTALLATION ON INSTALLATION ;

-- Indexes for Entity - LPLR 
DROP INDEX LPLR_ID_LPLR ON LPLR ;
-- Index for Relationship - 'CPLR(has corresponding [0:m] ) LPLR' 
DROP INDEX LPLR_FK_ZKEY01 ON LPLR ;
-- Index for Relationship - 'REPOSITORYCLIENT(contains [0:M] ) LPLR' 
DROP INDEX LPLR_FK_ZKEY0202 ON LPLR ;

-- Indexes for Entity - Panel 
DROP INDEX PANEL_ID_PANEL ON PANEL ;
-- Index for Relationship - 'XSLT(has [1:m] ) PANEL' 
DROP INDEX PANEL_FK_ZKEY01 ON PANEL ;

-- Indexes for Entity - RepositoryClient 
DROP INDEX REPOSITORYCLIENT_IDREPOSITORYCLIE ON REPOSITORYCLIENT ;
-- Index for Relationship - 'INSTALLATION(has satelite [0:m] ) REPOSITORYCLIENT' 
DROP INDEX REPOSITORYCLIENT_FK_ZKEY01 ON REPOSITORYCLIENT ;

-- Indexes for Entity - RevEvent 
DROP INDEX REVEVENT_ID ON REVEVENT ;

-- Indexes for Entity - RevSubEvent 
DROP INDEX REVSUBEVENT_REVEVENT_HAS ON REVSUBEVENT ;
-- Index for Relationship - 'REVEVENT(has [0:m] ) REVSUBEVENT' 
DROP INDEX REVSUBEVENT_FK_TYPE01 ON REVSUBEVENT ;

-- Indexes for Entity - SysEnqueue 
DROP INDEX SYSENQUEUE_USERNAME ON SYSENQUEUE ;

-- Indexes for Entity - User 
DROP INDEX z_USER_ID_USER ON z_USER ;

-- Indexes for Entity - UserMetaStatus 
DROP INDEX USERMETASTATUS_ID_USERMETASTATUS ON USERMETASTATUS ;
-- Index for Relationship - 'LPLR(has checked out metas from [0:m] ) USERMETASTATUS' 
DROP INDEX USERMETASTATUS_FK_ZKEY01 ON USERMETASTATUS ;
-- Index for Relationship - 'REPOSITORYCLIENT(contains [0:m] ) USERMETASTATUS' 
DROP INDEX USERMETASTATUS_FK_ZKEY0202 ON USERMETASTATUS ;
-- Index for Relationship - 'z_USER(has [0:M] ) USERMETASTATUS' 
DROP INDEX USERMETASTATUS_FK_ZKEY0303 ON USERMETASTATUS ;
-- Index for Relationship - 'Z_METADEF(has [0:M] ) USERMETASTATUS' 
DROP INDEX USERMETASTATUS_FK_ZKEY0404 ON USERMETASTATUS ;

-- Indexes for Entity - XSLT 
DROP INDEX XSLT_ID_XSLT ON XSLT ;
-- Index for Relationship - 'Z_METADEF(is a [0:m] ) XSLT' 
DROP INDEX XSLT_FK_ZKEY01 ON XSLT ;

-- Indexes for Entity - Z_MetaDef 
DROP INDEX Z_METADEF_ID_Z_METADEF ON Z_METADEF ;
-- Index for Relationship - 'CPLR(contains [0:M] ) Z_METADEF' 
DROP INDEX Z_METADEF_FK_ZKEY01 ON Z_METADEF ;

DROP INDEX MMZUSRBLNGTNSTLLTNDMMZSRBLNGTNSTL ON MM_Z_USER_BELONGTO_INSTALLATION ;
-- Index for Relationship - 'INSTALLATION(has [0:m] ) MM_Z_USER_BELONGTO_INSTALLATION' 
DROP INDEX MM_Z_USERBELONGTOINSTALLATIONFKZK01 ON MM_Z_USER_BELONGTO_INSTALLATION ;
-- Index for Relationship - 'z_USER(belong to [0:m] ) MM_Z_USER_BELONGTO_INSTALLATION' 
DROP INDEX MMZUSERBELONGTOINSTALLATIONFKZKEY02 ON MM_Z_USER_BELONGTO_INSTALLATION ;

DROP INDEX MMREPSTRYCLNTHSZSRDMMRPSTRYCLNTHS ON MM_REPOSITORYCLIENT_HAS_Z_USER ;
-- Index for Relationship - 'z_USER(works on [0:m] ) MM_REPOSITORYCLIENT_HAS_Z_USER' 
DROP INDEX MM_REPOSITORYCLIENT_HAS_ZUSERFKZK01 ON MM_REPOSITORYCLIENT_HAS_Z_USER ;
-- Index for Relationship - 'REPOSITORYCLIENT(has [0:m] ) MM_REPOSITORYCLIENT_HAS_Z_USER' 
DROP INDEX MM_REPOSITORYCLIENTHASZUSERFKZKEY02 ON MM_REPOSITORYCLIENT_HAS_Z_USER ;

DROP INDEX MMCPLRISRESDBYCPLRDMMCPLRSRSDBYCP ON MM_CPLR_ISREUSEDBY_CPLR ;
-- Index for Relationship - 'CPLR(reuses [0:m] ) MM_CPLR_ISREUSEDBY_CPLR' 
DROP INDEX MM_CPLR_ISREUSEDBY_CPLR_FK_ZKEY01 ON MM_CPLR_ISREUSEDBY_CPLR ;
-- Index for Relationship - 'CPLR(is reused by [0:m] ) MM_CPLR_ISREUSEDBY_CPLR' 
DROP INDEX MM_CPLR_ISREUSEDBY_CPLR_FK_ZKEY0202 ON MM_CPLR_ISREUSEDBY_CPLR ;

DROP TABLE ACCESSAUTHORITY ;
DROP TABLE AUDITTRAIL ;
DROP TABLE AUDITTRAILMETA ;
DROP TABLE BUFFER ;
DROP TABLE CPL ;
DROP TABLE CPL_REUSEABLEITEM ;
DROP TABLE CPLR ;
DROP TABLE CPLR_ENQUEUE ;
DROP TABLE CTRLPROPERTY ;
DROP TABLE CTRLPROPERTYATTRIBUTE ;
DROP TABLE z_FILE ;
DROP TABLE INSTALLATION ;
DROP TABLE LPLR ;
DROP TABLE PANEL ;
DROP TABLE REPOSITORYCLIENT ;
DROP TABLE REVEVENT ;
DROP TABLE REVSUBEVENT ;
DROP TABLE SYSENQUEUE ;
DROP TABLE z_USER ;
DROP TABLE USERMETASTATUS ;
DROP TABLE XSLT ;
DROP TABLE Z_METADEF ;
DROP TABLE MM_Z_USER_BELONGTO_INSTALLATION ;
DROP TABLE MM_REPOSITORYCLIENT_HAS_Z_USER ;
DROP TABLE MM_CPLR_ISREUSEDBY_CPLR ;

-- Entity - AccessAuthority 
CREATE TABLE ACCESSAUTHORITY ( 
           ZKEY                               INTEGER            NOT NULL, 
           AUTHORITY                          INTEGER            NOT NULL, 
           FK_ZKEY                            INTEGER            NOT NULL, 
           FK_ZKEY02                          INTEGER            NOT NULL ) ;
 
-- Entity - AuditTrail 
CREATE TABLE AUDITTRAIL ( 
           USERNAME                           TEXT( 32 )                 , 
           USERDESC                           MEMO                       , 
           WKS_ID                             INTEGER                    , 
           z_TIMESTAMP                        TEXT( 25 )         NOT NULL, 
           SHORTDESC                          TEXT( 254 )                , 
           z_DESC                             MEMO                       , 
           FK_TYPE                            INTEGER                    , 
           FK_ZKEY                            INTEGER                    , 
           FK_SUBTYPE                         INTEGER                    , 
           FK_TYPE02                          INTEGER                    , 
           FK_ZKEY02                          INTEGER            NOT NULL ) ;
 
-- Entity - AuditTrailMeta 
CREATE TABLE AUDITTRAILMETA ( 
           BDELETED                           TEXT( 1 )                  , 
           METADEFNAME                        TEXT( 32 )         NOT NULL, 
           METADEFTYPE                        INTEGER            NOT NULL, 
           FK_ZKEY                            INTEGER                    , 
           FK_TIMESTAMP                       TEXT( 25 )         NOT NULL, 
           FK_ZKEY02                          INTEGER            NOT NULL ) ;
 
-- Entity - Buffer 
CREATE TABLE BUFFER ( 
           z_VALUE                            LONGBINARY                 , 
           SEQNO                              INTEGER            NOT NULL, 
           FK_ZKEY                            INTEGER            NOT NULL ) ;
 
-- Entity - CPL 
CREATE TABLE CPL ( 
           NAME                               TEXT( 32 )         NOT NULL, 
           ZKEY                               INTEGER            NOT NULL, 
           z_DESC                             MEMO                       , 
           FK_ZKEY                            INTEGER                    , 
           FK_ZKEY02                          INTEGER            NOT NULL ) ;
 
-- Entity - CPL_ReuseableItem 
CREATE TABLE CPL_REUSEABLEITEM ( 
           ZKEY                               INTEGER            NOT NULL ) ;
 
-- Entity - CPLR 
CREATE TABLE CPLR ( 
           NAME                               TEXT( 32 )         NOT NULL, 
           LASTCHECKINDATE                    TEXT( 25 )                 , 
           ZKEY                               INTEGER            NOT NULL, 
           z_DESC                             MEMO                       , 
           METASRCDIR                         TEXT( 254 )                , 
           PGMSRCDIR                          TEXT( 254 )                , 
           EXECDIR                            TEXT( 254 )                , 
           DB_MODE                            INTEGER                    , 
           FK_ZKEY                            INTEGER                     ) ;
 
-- Entity - CPLR_Enqueue 
CREATE TABLE CPLR_ENQUEUE ( 
           z_TIMESTAMP                        TEXT( 25 )         NOT NULL, 
           ENQUEUETYPE                        INTEGER                    , 
           USERNAME                           TEXT( 32 )                 , 
           USERDESC                           MEMO                       , 
           FK_ZKEY                            INTEGER            NOT NULL, 
           FK_ZKEY02                          INTEGER                     ) ;
 
-- Entity - CtrlProperty 
CREATE TABLE CTRLPROPERTY ( 
           NAME                               TEXT( 254 )                , 
           z_VALUE                            TEXT( 254 )                , 
           TYPE                               TEXT( 1 )                  , 
           ZKEY                               INTEGER            NOT NULL ) ;
 
-- Entity - CtrlPropertyAttribute 
CREATE TABLE CTRLPROPERTYATTRIBUTE ( 
           NAME                               TEXT( 32 )                 , 
           z_VALUE                            TEXT( 32 )                 , 
           TYPE                               TEXT( 1 )                  , 
           ZKEY                               INTEGER            NOT NULL, 
           FK_ZKEY                            INTEGER            NOT NULL ) ;
 
-- Entity - File 
CREATE TABLE z_FILE ( 
           ZKEY                               INTEGER            NOT NULL, 
           NAME                               TEXT( 32 )                 , 
           EXTENSION                          TEXT( 3 )                  , 
           FK_ZKEY                            INTEGER            NOT NULL ) ;
 
-- Entity - Installation 
CREATE TABLE INSTALLATION ( 
           NAME                               TEXT( 32 )         NOT NULL, 
           ZKEY                               INTEGER            NOT NULL, 
           SYSADM                             TEXT( 32 )         NOT NULL, 
           z_PASSWORD                         TEXT( 8 )                  , 
           NEXTWKS_ID                         INTEGER                    , 
           NEXTINSTALLZKEY                    INTEGER                    , 
           z_DESC                             MEMO                        ) ;
 
-- Entity - LPLR 
CREATE TABLE LPLR ( 
           NAME                               TEXT( 32 )         NOT NULL, 
           z_DESC                             MEMO                       , 
           ZKEY                               INTEGER            NOT NULL, 
           PGMSRCDIR                          TEXT( 254 )                , 
           EXECDIR                            TEXT( 254 )                , 
           METASRCDIR                         TEXT( 254 )                , 
           TASKUSECOUNT                       INTEGER                    , 
           LASTREFRESHDATE                    TEXT( 25 )                 , 
           MAXZKEY                            INTEGER                    , 
           FK_ZKEY                            INTEGER                    , 
           FK_ZKEY02                          INTEGER                     ) ;
 
-- Entity - Panel 
CREATE TABLE PANEL ( 
           TAG                                TEXT( 32 )                 , 
           CAPTION                            TEXT( 254 )                , 
           PSDLG_X                            INTEGER                    , 
           PSDLG_Y                            INTEGER                    , 
           SZDLG_X                            INTEGER                    , 
           SZDLG_Y                            INTEGER                    , 
           ZKEY                               INTEGER            NOT NULL, 
           CSS_CLASS                          TEXT( 50 )                 , 
           AUTOSEQ                            INTEGER                    , 
           FK_ZKEY                            INTEGER            NOT NULL ) ;
 
-- Entity - RepositoryClient 
CREATE TABLE REPOSITORYCLIENT ( 
           ZKEY                               INTEGER            NOT NULL, 
           AUTHENTIC                          TEXT( 32 )                 , 
           WKS_ID                             INTEGER                    , 
           MAXZKEY                            INTEGER                    , 
           TE_MAXZKEY                         INTEGER                    , 
           LASTSYNCZKEY                       INTEGER                    , 
           z_DESC                             MEMO                       , 
           DEFAULTLPLR_ZKEY                   INTEGER                    , 
           FK_ZKEY                            INTEGER                     ) ;
 
-- Entity - RevEvent 
CREATE TABLE REVEVENT ( 
           TYPE                               INTEGER            NOT NULL, 
           z_DESC                             TEXT( 32 )         NOT NULL ) ;
 
-- Entity - RevSubEvent 
CREATE TABLE REVSUBEVENT ( 
           z_DESC                             TEXT( 254 )                , 
           SUBTYPE                            INTEGER            NOT NULL, 
           NECESSARY                          TEXT( 1 )                  , 
           FK_TYPE                            INTEGER            NOT NULL ) ;
 
-- Entity - SysEnqueue 
CREATE TABLE SYSENQUEUE ( 
           z_TIMESTAMP                        TEXT( 25 )         NOT NULL, 
           USERNAME                           TEXT( 32 )         NOT NULL, 
           USERDESC                           MEMO                        ) ;
 
-- Entity - User 
CREATE TABLE z_USER ( 
           NAME                               TEXT( 32 )         NOT NULL, 
           ZKEY                               INTEGER            NOT NULL, 
           z_PASSWORD                         TEXT( 8 )                  , 
           z_DESC                             MEMO                       , 
           GENERATIONSTARTZKEY                INTEGER                     ) ;
 
-- Entity - UserMetaStatus 
CREATE TABLE USERMETASTATUS ( 
           ZKEY                               INTEGER            NOT NULL, 
           STATUS                             INTEGER            NOT NULL, 
           CHECKOUTDATE                       TEXT( 25 )         NOT NULL, 
           COMMENTS                           TEXT( 254 )                , 
           FK_ZKEY                            INTEGER            NOT NULL, 
           FK_ZKEY02                          INTEGER                    , 
           FK_ZKEY03                          INTEGER            NOT NULL, 
           FK_ZKEY04                          INTEGER            NOT NULL ) ;
 
-- Entity - XSLT 
CREATE TABLE XSLT ( 
           TAG                                TEXT( 32 )                 , 
           ZKEY                               INTEGER            NOT NULL, 
           z_DESC                             MEMO                       , 
           REPORTTEXTSIZE                     INTEGER                    , 
           UNIT                               INTEGER                    , 
           MARGINTOP                          INTEGER                    , 
           MARGINLEFT                         INTEGER                    , 
           MARGINBOTTOM                       INTEGER                    , 
           MARGINRIGHT                        INTEGER                    , 
           FK_ZKEY                            INTEGER                     ) ;
 
-- Entity - Z_MetaDef 
CREATE TABLE Z_METADEF ( 
           NAME                               TEXT( 32 )         NOT NULL, 
           ZKEY                               INTEGER            NOT NULL, 
           TYPE                               INTEGER            NOT NULL, 
           LASTUPDATEDATE                     TEXT( 25 )                 , 
           LASTCHECKINDATE                    TEXT( 25 )                 , 
           z_DESC                             MEMO                       , 
           AUTOSEQ                            INTEGER                    , 
           FK_ZKEY                            INTEGER            NOT NULL ) ;
 
CREATE TABLE MM_Z_USER_BELONGTO_INSTALLATION ( 
           FK_ZKEY                            INTEGER            NOT NULL, 
           FK_ZKEY02                          INTEGER            NOT NULL ) ;
 
CREATE TABLE MM_REPOSITORYCLIENT_HAS_Z_USER ( 
           FK_ZKEY                            INTEGER            NOT NULL, 
           FK_ZKEY02                          INTEGER            NOT NULL ) ;
 
CREATE TABLE MM_CPLR_ISREUSEDBY_CPLR ( 
           FK_ZKEY                            INTEGER            NOT NULL, 
           FK_ZKEY02                          INTEGER            NOT NULL ) ;
 
-- Main key for Entity - AccessAuthority 
CREATE UNIQUE INDEX UACCESSAUTHORITYIDACCESSAUTH24386 
      ON ACCESSAUTHORITY ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'z_USER(has [0:m] ) ACCESSAUTHORITY' 
CREATE INDEX ACCESSAUTHORITY_FK_ZKEY_2440280 
      ON ACCESSAUTHORITY ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'CPLR(has [0:M] ) ACCESSAUTHORITY' 
CREATE INDEX ACCESSAUTHORITY_FK_ZKEY02_2440290 
      ON ACCESSAUTHORITY ( 
           FK_ZKEY02 ) ;
 
 
-- Main key for Entity - AuditTrail 
CREATE UNIQUE INDEX UAUDITTRAIL_ID_2438690 
      ON AUDITTRAIL ( 
           z_TIMESTAMP, 
           FK_ZKEY02 ) ;
 
 
-- Index for Relationship - 'REVEVENT(for [0:m] ) AUDITTRAIL' 
CREATE INDEX AUDITTRAIL_FK_TYPE_2440180 
      ON AUDITTRAIL ( 
           FK_TYPE ) ;
 
 
-- Index for Relationship - 'REPOSITORYCLIENT(may have [0:m] ) AUDITTRAIL' 
CREATE INDEX AUDITTRAIL_FK_ZKEY_2440420 
      ON AUDITTRAIL ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'REVSUBEVENT(for [0:m] ) AUDITTRAIL' 
CREATE INDEX AUDITTRAIL_FK_SUBTYPE_2440440 
      ON AUDITTRAIL ( 
           FK_SUBTYPE ) ;
 
 
-- Index for Relationship - 'REVSUBEVENT(for [0:m] ) AUDITTRAIL' 
CREATE INDEX AUDITTRAIL_FK_TYPE02_2440450 
      ON AUDITTRAIL ( 
           FK_TYPE02 ) ;
 
 
-- Index for Relationship - 'CPLR(has [0:m] ) AUDITTRAIL' 
CREATE INDEX AUDITTRAIL_FK_ZKEY02_2440500 
      ON AUDITTRAIL ( 
           FK_ZKEY02 ) ;
 
 
-- Main key for Entity - AuditTrailMeta 
CREATE UNIQUE INDEX UAUDITTRAILMETA_ID_2440510 
      ON AUDITTRAILMETA ( 
           FK_TIMESTAMP, 
           FK_ZKEY02, 
           METADEFNAME, 
           METADEFTYPE ) ;
 
 
-- Index for Relationship - 'Z_METADEF(has [0:m] ) AUDITTRAILMETA' 
CREATE INDEX AUDITTRAILMETA_FK_ZKEY_2440170 
      ON AUDITTRAILMETA ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'AUDITTRAIL(has [0:m] ) AUDITTRAILMETA' 
CREATE INDEX AUDITTRAILMETA_FK_TIMESTAMP_24405 
      ON AUDITTRAILMETA ( 
           FK_TIMESTAMP ) ;
 
 
-- Index for Relationship - 'AUDITTRAIL(has [0:m] ) AUDITTRAILMETA' 
CREATE INDEX AUDITTRAILMETA_FK_ZKEY02_2440520 
      ON AUDITTRAILMETA ( 
           FK_ZKEY02 ) ;
 
 
-- Main key for Entity - Buffer 
CREATE UNIQUE INDEX UBUFFER_ID_BUFFER_2440150 
      ON BUFFER ( 
           FK_ZKEY, 
           SEQNO ) ;
 
 
-- Index for Relationship - 'z_FILE(has [0:m] ) BUFFER' 
CREATE INDEX BUFFER_FK_ZKEY_2440150 
      ON BUFFER ( 
           FK_ZKEY ) ;
 
 
-- Main key for Entity - CPL 
CREATE UNIQUE INDEX UCPL_ID_CPL_2438840 
      ON CPL ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'z_USER(may administer [0:m] ) CPL' 
CREATE INDEX CPL_FK_ZKEY_2440300 
      ON CPL ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'INSTALLATION(contains [0:m] ) CPL' 
CREATE INDEX CPL_FK_ZKEY02_2440370 
      ON CPL ( 
           FK_ZKEY02 ) ;
 
 
-- Main key for Entity - CPL_ReuseableItem 
CREATE UNIQUE INDEX UCPLREUSEABLEITEMIDCPLREUSEA24388 
      ON CPL_REUSEABLEITEM ( 
           ZKEY ) ;
 
 
-- Main key for Entity - CPLR 
CREATE UNIQUE INDEX UCPLR_ID_CPLR_2438930 
      ON CPLR ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'CPL(has [1:M] ) CPLR' 
CREATE INDEX CPLR_FK_ZKEY_2440380 
      ON CPLR ( 
           FK_ZKEY ) ;
 
 
-- Main key for Entity - CPLR_Enqueue 
CREATE UNIQUE INDEX UCPLR_ENQUEUE_CPLR_MAYHAVEA_24402 
      ON CPLR_ENQUEUE ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'CPLR(may have a [0:1] ) CPLR_ENQUEUE' 
CREATE INDEX CPLR_ENQUEUE_FK_ZKEY_2440210 
      ON CPLR_ENQUEUE ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'REPOSITORYCLIENT(has enqueue for [0:1] ) CPLR_ENQUEUE' 
CREATE INDEX CPLR_ENQUEUE_FK_ZKEY02_2440220 
      ON CPLR_ENQUEUE ( 
           FK_ZKEY02 ) ;
 
 
-- Main key for Entity - CtrlProperty 
CREATE UNIQUE INDEX UCTRLPROPERTY_ZKEY_2439100 
      ON CTRLPROPERTY ( 
           ZKEY ) ;
 
 
-- Main key for Entity - CtrlPropertyAttribute 
CREATE UNIQUE INDEX UCTRLPROPERTYATTRIBUTE_ZKEY_24391 
      ON CTRLPROPERTYATTRIBUTE ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'CTRLPROPERTY(has [0:m] ) CTRLPROPERTYATTRIBUTE' 
CREATE INDEX CTRLPROPERTYATTRIBUTE_FKZKEY24401 
      ON CTRLPROPERTYATTRIBUTE ( 
           FK_ZKEY ) ;
 
 
-- Main key for Entity - File 
CREATE UNIQUE INDEX Uz_FILE_ID_FILE_2439190 
      ON z_FILE ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'CPLR(has [0:m] ) z_FILE' 
CREATE INDEX z_FILE_FK_ZKEY_2440160 
      ON z_FILE ( 
           FK_ZKEY ) ;
 
 
-- Main key for Entity - Installation 
CREATE UNIQUE INDEX UINSTALLATION_IDINSTALLATION24392 
      ON INSTALLATION ( 
           ZKEY ) ;
 
 
-- Main key for Entity - LPLR 
CREATE UNIQUE INDEX ULPLR_ID_LPLR_2439350 
      ON LPLR ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'CPLR(has corresponding [0:m] ) LPLR' 
CREATE INDEX LPLR_FK_ZKEY_2440400 
      ON LPLR ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'REPOSITORYCLIENT(contains [0:M] ) LPLR' 
CREATE INDEX LPLR_FK_ZKEY02_2440410 
      ON LPLR ( 
           FK_ZKEY02 ) ;
 
 
-- Main key for Entity - Panel 
CREATE UNIQUE INDEX UPANEL_ID_PANEL_2439500 
      ON PANEL ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'XSLT(has [1:m] ) PANEL' 
CREATE INDEX PANEL_FK_ZKEY_2440130 
      ON PANEL ( 
           FK_ZKEY ) ;
 
 
-- Main key for Entity - RepositoryClient 
CREATE UNIQUE INDEX UREPOSITORYCLIENTIDREPOSITOR24395 
      ON REPOSITORYCLIENT ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'INSTALLATION(has satelite [0:m] ) REPOSITORYCLIENT' 
CREATE INDEX REPOSITORYCLIENT_FK_ZKEY_2440190 
      ON REPOSITORYCLIENT ( 
           FK_ZKEY ) ;
 
 
-- Main key for Entity - RevEvent 
CREATE UNIQUE INDEX UREVEVENT_ID_2439640 
      ON REVEVENT ( 
           TYPE ) ;
 
 
-- Main key for Entity - RevSubEvent 
CREATE UNIQUE INDEX UREVSUBEVENT_REVEVENT_HAS_2440430 
      ON REVSUBEVENT ( 
           FK_TYPE, 
           SUBTYPE ) ;
 
 
-- Index for Relationship - 'REVEVENT(has [0:m] ) REVSUBEVENT' 
CREATE INDEX REVSUBEVENT_FK_TYPE_2440430 
      ON REVSUBEVENT ( 
           FK_TYPE ) ;
 
 
-- Main key for Entity - SysEnqueue 
CREATE UNIQUE INDEX USYSENQUEUE_USERNAME_2439740 
      ON SYSENQUEUE ( 
           USERNAME ) ;
 
 
-- Main key for Entity - User 
CREATE UNIQUE INDEX Uz_USER_ID_USER_2439790 
      ON z_USER ( 
           ZKEY ) ;
 
 
-- Main key for Entity - UserMetaStatus 
CREATE UNIQUE INDEX UUSERMETASTATUSIDUSERMETASTA24398 
      ON USERMETASTATUS ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'LPLR(has checked out metas from [0:m] ) USERMETASTATUS' 
CREATE INDEX USERMETASTATUS_FK_ZKEY_2440200 
      ON USERMETASTATUS ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'REPOSITORYCLIENT(contains [0:m] ) USERMETASTATUS' 
CREATE INDEX USERMETASTATUS_FK_ZKEY02_2440230 
      ON USERMETASTATUS ( 
           FK_ZKEY02 ) ;
 
 
-- Index for Relationship - 'z_USER(has [0:M] ) USERMETASTATUS' 
CREATE INDEX USERMETASTATUS_FK_ZKEY03_2440310 
      ON USERMETASTATUS ( 
           FK_ZKEY03 ) ;
 
 
-- Index for Relationship - 'Z_METADEF(has [0:M] ) USERMETASTATUS' 
CREATE INDEX USERMETASTATUS_FK_ZKEY04_2440320 
      ON USERMETASTATUS ( 
           FK_ZKEY04 ) ;
 
 
-- Main key for Entity - XSLT 
CREATE UNIQUE INDEX UXSLT_ID_XSLT_2439920 
      ON XSLT ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'Z_METADEF(is a [0:m] ) XSLT' 
CREATE INDEX XSLT_FK_ZKEY_2440120 
      ON XSLT ( 
           FK_ZKEY ) ;
 
 
-- Main key for Entity - Z_MetaDef 
CREATE UNIQUE INDEX UZ_METADEF_ID_Z_METADEF_2440030 
      ON Z_METADEF ( 
           ZKEY ) ;
 
 
-- Index for Relationship - 'CPLR(contains [0:M] ) Z_METADEF' 
CREATE INDEX Z_METADEF_FK_ZKEY_2440390 
      ON Z_METADEF ( 
           FK_ZKEY ) ;
 
 
CREATE UNIQUE INDEX UMMZUSERBELONGTOINSTALLATION24402 
      ON MM_Z_USER_BELONGTO_INSTALLATION ( 
           FK_ZKEY, 
           FK_ZKEY02 ) ;
 
 
-- Index for Relationship - 'INSTALLATION(has [0:m] ) MM_Z_USER_BELONGTO_INSTALLATION' 
CREATE INDEX MMZUSERBELONGTOINSTALLATIONF24402 
      ON MM_Z_USER_BELONGTO_INSTALLATION ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'z_USER(belong to [0:m] ) MM_Z_USER_BELONGTO_INSTALLATION' 
CREATE INDEX MMZUSERBELONGTOINSTALLATIO0224402 
      ON MM_Z_USER_BELONGTO_INSTALLATION ( 
           FK_ZKEY02 ) ;
 
 
CREATE UNIQUE INDEX UMMREPOSITORYCLIENTHASZUSERI24403 
      ON MM_REPOSITORYCLIENT_HAS_Z_USER ( 
           FK_ZKEY, 
           FK_ZKEY02 ) ;
 
 
-- Index for Relationship - 'z_USER(works on [0:m] ) MM_REPOSITORYCLIENT_HAS_Z_USER' 
CREATE INDEX MMREPOSITORYCLIENTHASZUSERFK24403 
      ON MM_REPOSITORYCLIENT_HAS_Z_USER ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'REPOSITORYCLIENT(has [0:m] ) MM_REPOSITORYCLIENT_HAS_Z_USER' 
CREATE INDEX MMREPOSITORYCLIENTHASZUSER0224403 
      ON MM_REPOSITORYCLIENT_HAS_Z_USER ( 
           FK_ZKEY02 ) ;
 
 
CREATE UNIQUE INDEX UMMCPLRISREUSEDBYCPLRIDMMCPL24404 
      ON MM_CPLR_ISREUSEDBY_CPLR ( 
           FK_ZKEY, 
           FK_ZKEY02 ) ;
 
 
-- Index for Relationship - 'CPLR(reuses [0:m] ) MM_CPLR_ISREUSEDBY_CPLR' 
CREATE INDEX MM_CPLR_ISREUSEDBYCPLRFKZKEY24404 
      ON MM_CPLR_ISREUSEDBY_CPLR ( 
           FK_ZKEY ) ;
 
 
-- Index for Relationship - 'CPLR(is reused by [0:m] ) MM_CPLR_ISREUSEDBY_CPLR' 
CREATE INDEX MMCPLRISREUSEDBYCPLRFKZKEY0224404 
      ON MM_CPLR_ISREUSEDBY_CPLR ( 
           FK_ZKEY02 ) ;
 
 
 
