// Macros for system operation interface

#define mSysObjectConstraintCall(p1,p2,p3,p4)      (*p1) (p2,p3,p4)
#define mSysEntityConstraintCall(p1,p2,p3,p4,p5)   (*p1) (p2,p3,p4,p5)
#define mSysDerivedCall(p1,p2,p3,p4,p5)            (*p1) (p2,p3,p4,p5)
#define mSysDomainCall(p1,p2,p3,p4,p5,p6,p7,p8,p9) (*p1) (p2,p3,p4,p5,p6,p7,p8,p9)
#define mSysGetObjectConstraintProc(p1,p2,p3)      SysGetProc(p2,p3)
#define mSysGetEntityConstraintProc(p1,p2,p3)      SysGetProc(p2,p3)
#define mSysGetDerivedProc(p1,p2,p3)               SysGetProc(p2,p3)
#define mSysGetDomainProc(p1,p2,p3)                SysGetProc(p2,p3)
#define mSysDBHandlerCall(p1,p2,p3,p4,p5,p6,p7)    (*pOper) (p1,p2,p3,p4,p5,p6,p7)
#define mSysGKHandlerCall(p1,p2,p3,p4,p5)          (*pGKOper) (p1,p2,p3,p4,p5)
#define mSysGetDBHandlerProc        SysGetProc
#define mSysGetGKHandlerProc        SysGetProc
