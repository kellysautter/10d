/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzzolodr.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"



zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_ReportOI( zVIEW  vOI,
                    zPCHAR szReportFileName )
{
   zCHAR    Line[ 511 ];
   zCHAR    szData[ 511 ];
   zLONG    lRptFile;
   zLONG    lTempInteger_0;
   zSHORT   nZRetCode;
   zSHORT   RESULT;

//  Source Line # 14:    IF szReportFileName = ""
   lTempInteger_0 = zstrcmp( szReportFileName, "" );
   if ( lTempInteger_0 == 0 )
   {
//  Source Line # 15:       RETURN -16
      return( -16 );
//  Source Line # 16:    ELSE
   }
   else
   {
//  Source Line # 17:       lRptFile = SysOpenFile( szReportFileName, COREFILE_WRITE )
      lRptFile = SysOpenFile( vOI, szReportFileName, COREFILE_WRITE );
//  Source Line # 18:       IF lRptFile < 0
      if ( lRptFile < 0 )
      {
//  Source Line # 19:          RETURN -1
         return( -1 );
      }
//  Source Line # 20:       END
   }
//  Source Line # 28:         + "ReLin  "
   ZeidonStringCopy( Line, 1, 0, "hLOD                                    ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Description                       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ReLin  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI, lRptFile, Line );
//  Source Line # 51:         + "Description                       "
   ZeidonStringCopy( Line, 1, 0, "h  LOD_EntityParent                     ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Indent Level  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "CREATE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DELETE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Updat  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "INCLUDE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Source of Include  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "EXCLUDE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "HIDDEN  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "RECURSIVE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "AUTOCREATE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Deriv  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Sequence Type  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Work   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Paren  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Description                       ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI, lRptFile, Line );
//  Source Line # 74:         + "Description                       "
   ZeidonStringCopy( Line, 1, 0, "h     LOD_EntityChild                   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Indent Level  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "CREATE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DELETE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Updat  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "INCLUDE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Source of Include  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "EXCLUDE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "HIDDEN  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "RECURSIVE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "AUTOCREATE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Deriv  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Sequence Type  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Work   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Paren  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Description                       ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI, lRptFile, Line );
//  Source Line # 88:         + "Sequence Order  "
   ZeidonStringCopy( Line, 1, 0, "h     LOD_AttributeRec                  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Work   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Updat  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Deriv. Lib  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Deriv. Function                  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "INIT                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "HIDDEN  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Sequence Priority  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Sequence Order  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI, lRptFile, Line );
//  Source Line # 117:       /* Attribute "ER_AttributeRec.t6B_HelpKey" suppressed due to Line length. */
   ZeidonStringCopy( Line, 1, 0, "h        ER_AttributeRec                ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Length       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Key    ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Required  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Description                       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Prompt Paint Literal              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Pompt Paint Length  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "List Paint Literal                ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "List Paint Length  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_Position      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_OI_Toke  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_AI_Enti  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_AI_DT_T  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_OI_DT_T  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_Decimal  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_T  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_Domain                        ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_Column          ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_Order    ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_S  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_DilKey                        ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_ErrKey                        ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI, lRptFile, Line );
//  Source Line # 132:         + "DecimalForm  "
   ZeidonStringCopy ( Line, 1, 0, "h           DomainRec                   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Desc                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Domai  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DataT  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "MaxStringLt  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "C  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DLL_Name                          ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "OperName                          ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DecimalForm  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 144:         + "ADW_OI_Toke  "
   ZeidonStringCopy ( Line, 1, 0, "h     ER_EntityRec                      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Purpose       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Description                       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "OwningArea                        ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_Position      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_OI_Toke  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 153:         + "Max Cardinality  "
   ZeidonStringCopy( Line, 1, 0, "h     ER_RelLinkRec                     ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Min Cardinality  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Max Cardinality  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 162:         + "ADW_AI_Toke  "
   ZeidonStringCopy( Line, 1, 0, "h        ER_RelTypeRec                  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Fact                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_Position      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_AI_Toke  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 185:         + "Description                       "
   ZeidonStringCopy( Line, 1, 0, "h  LOD_Entity                           ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Indent Level  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "CREATE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DELETE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Updat  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "INCLUDE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Source of Include  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "EXCLUDE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "HIDDEN  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "RECURSIVE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "AUTOCREATE  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Deriv  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Sequence Type  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Work   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Paren  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Description                       ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 199:         + "Sequence Order  "
   ZeidonStringCopy( Line, 1, 0, "h     LOD_Attribute                     ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Work   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Updat  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Deriv. Lib  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Deriv. Function                  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "INIT                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "HIDDEN  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Sequence Priority  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Sequence Order  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 228:       /* Attribute "ER_Attribute.t6B_HelpKey" suppressed due to Line length. */
   ZeidonStringCopy( Line, 1, 0, "h        ER_Attribute                   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Length       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Key    ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Required  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Description                       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Prompt Paint Literal              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Pompt Paint Length  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "List Paint Literal                ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "List Paint Length  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_Position      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_OI_Toke  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_AI_Enti  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_AI_DT_T  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_OI_DT_T  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_Decimal  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_T  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_Domain                        ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_Column          ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_Order    ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_S  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_DilKey                        ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "t6B_ErrKey                        ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 243:         + "DecimalForm  "
   ZeidonStringCopy( Line, 1, 0, "h           Domain                      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Desc                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Domai  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DataT  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "MaxStringLt  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "C  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DLL_Name                          ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "OperName                          ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "DecimalForm  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 255:         + "ADW_OI_Toke  "
   ZeidonStringCopy( Line, 1, 0, "h     ER_Entity                         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Purpose       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Description                       ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "OwningArea                        ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_Position      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_OI_Toke  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 264:         + "Max Cardinality  "
   ZeidonStringCopy( Line, 1, 0, "h     ER_RelLink                        ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Min Cardinality  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Max Cardinality  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 273:         + "ADW_AI_Toke  "
   ZeidonStringCopy( Line, 1, 0, "h        ER_RelType                     ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Fact                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_Position      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ADW_AI_Toke  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 288:         + "W_VML_Fi  "
   ZeidonStringCopy( Line, 1, 0, "h  Operation                            ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Desc                              ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Type   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ReturnDesc                        ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Retur  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ExecName  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "HeaderFi  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "OrdinalValu  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "W_VML_Fi  ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 298:         + "ZKey         "
   ZeidonStringCopy( Line, 1, 0, "h     Parameter                         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ShortDesc                         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Type   ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "PtrFl  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Unsig  ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 306:         + "Desc                              "
   ZeidonStringCopy( Line, 1, 0, "h     VML_Source_Ref                    ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Desc                              ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 314:         + "Desc                              "
   ZeidonStringCopy( Line, 1, 0, "h  VML_Source                           ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey         ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Name      ", 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Desc                              ", 1, 0, 511 );
   nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 321:    FOR EACH vOI.LOD
   RESULT = SetCursorFirstEntity( vOI, "LOD", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
//  Source Line # 322:       Line = "dLOD                                    "
      ZeidonStringCopy( Line, 1, 0, "dLOD                                    ", 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD", "ZKey" );
//  Source Line # 323:       GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD", "ZKey" )
      zstrblank ( szData, 14, 1 );
//  Source Line # 325:       Line = Line + szData
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD", "Name" );
//  Source Line # 326:       GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD", "Name" )
      zstrblank ( szData, 35, 1 );
//  Source Line # 328:       Line = Line + szData
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD", "Desc" );
//  Source Line # 329:       GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD", "Desc" )
      zstrblank ( szData, 35, 1 );
//  Source Line # 331:       Line = Line + szData
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD", "ReLinked" );
//  Source Line # 332:       GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD", "ReLinked" )
      zstrblank ( szData, 8, 1 );
//  Source Line # 334:       Line = Line + szData
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 336:       FOR EACH vOI.LOD_EntityParent
      RESULT = SetCursorFirstEntity( vOI, "LOD_EntityParent", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
//  Source Line # 337:          Line = "d  LOD_EntityParent                     "
         ZeidonStringCopy( Line, 1, 0, "d  LOD_EntityParent                     ", 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Name" );
//  Source Line # 338:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Name" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 340:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "IndentName" );
//  Source Line # 341:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "IndentName" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 343:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "IndentLvl" );
//  Source Line # 344:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "IndentLvl" )
         zstrblank ( szData, 15, 1 );
//  Source Line # 346:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "ZKey" );
//  Source Line # 347:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "ZKey" )
         zstrblank ( szData, 14, 1 );
//  Source Line # 349:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Create" );
//  Source Line # 350:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Create" )
         zstrblank ( szData, 9, 1 );
//  Source Line # 352:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Delete" );
//  Source Line # 353:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Delete" )
         zstrblank ( szData, 9, 1 );
//  Source Line # 355:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Update" );
//  Source Line # 356:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Update" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 358:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Include" );
//  Source Line # 359:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Include" )
         zstrblank ( szData, 10, 1 );
//  Source Line # 361:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "InclSrc" );
//  Source Line # 362:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "InclSrc" )
         zstrblank ( szData, 20, 1 );
//  Source Line # 364:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Exclude" );
//  Source Line # 365:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Exclude" )
         zstrblank ( szData, 10, 1 );
//  Source Line # 367:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Hidden" );
//  Source Line # 368:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Hidden" )
         zstrblank ( szData, 9, 1 );
//  Source Line # 370:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Recursive" );
//  Source Line # 371:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Recursive" )
         zstrblank ( szData, 12, 1 );
//  Source Line # 373:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "AutoCreate" );
//  Source Line # 374:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "AutoCreate" )
         zstrblank ( szData, 13, 1 );
//  Source Line # 376:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Derived" );
//  Source Line # 377:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Derived" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 379:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "SeqType" );
//  Source Line # 380:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "SeqType" )
         zstrblank ( szData, 16, 1 );
//  Source Line # 382:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Work" );
//  Source Line # 383:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Work" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 385:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "ParentDeleteBehave" );
//  Source Line # 386:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "ParentDeleteBehave" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 388:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Desc" );
//  Source Line # 389:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityParent", "Desc" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 391:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 393:          FOR EACH vOI.LOD_EntityChild
         RESULT = SetCursorFirstEntity( vOI, "LOD_EntityChild", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 394:             Line = "d     LOD_EntityChild                   "
            ZeidonStringCopy( Line, 1, 0, "d     LOD_EntityChild                   ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Name" );
//  Source Line # 395:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Name" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 397:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "IndentName" );
//  Source Line # 398:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "IndentName" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 400:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "IndentLvl" );
//  Source Line # 401:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "IndentLvl" )
            zstrblank ( szData, 15, 1 );
//  Source Line # 403:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "ZKey" );
//  Source Line # 404:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 406:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Create" );
//  Source Line # 407:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Create" )
            zstrblank ( szData, 9, 1 );
//  Source Line # 409:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Delete" );
//  Source Line # 410:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Delete" )
            zstrblank ( szData, 9, 1 );
//  Source Line # 412:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Update" );
//  Source Line # 413:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Update" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 415:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Include" );
//  Source Line # 416:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Include" )
            zstrblank ( szData, 10, 1 );
//  Source Line # 418:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "InclSrc" );
//  Source Line # 419:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "InclSrc" )
            zstrblank ( szData, 20, 1 );
//  Source Line # 421:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Exclude" );
//  Source Line # 422:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Exclude" )
            zstrblank ( szData, 10, 1 );
//  Source Line # 424:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Hidden" );
//  Source Line # 425:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Hidden" )
            zstrblank ( szData, 9, 1 );
//  Source Line # 427:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Recursive" );
//  Source Line # 428:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Recursive" )
            zstrblank ( szData, 12, 1 );
//  Source Line # 430:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "AutoCreate" );
//  Source Line # 431:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "AutoCreate" )
            zstrblank ( szData, 13, 1 );
//  Source Line # 433:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Derived" );
//  Source Line # 434:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Derived" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 436:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "SeqType" );
//  Source Line # 437:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "SeqType" )
            zstrblank ( szData, 16, 1 );
//  Source Line # 439:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Work" );
//  Source Line # 440:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Work" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 442:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "ParentDeleteBehave" );
//  Source Line # 443:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "ParentDeleteBehave" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 445:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Desc" );
//  Source Line # 446:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_EntityChild", "Desc" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 448:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 393:
            RESULT = SetCursorNextEntity ( vOI, "LOD_EntityChild", "" );
//  Source Line # 449:             SysWriteLine( lRptFile, Line )
         }
//  Source Line # 451:          FOR EACH vOI.LOD_AttributeRec
         RESULT = SetCursorFirstEntity( vOI, "LOD_AttributeRec", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 452:             Line = "d     LOD_AttributeRec                  "
            ZeidonStringCopy( Line, 1, 0, "d     LOD_AttributeRec                  ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "ZKey" );
//  Source Line # 453:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 455:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "Work" );
//  Source Line # 456:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "Work" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 458:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "Update" );
//  Source Line # 459:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "Update" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 461:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "DerivedE" );
//  Source Line # 462:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "DerivedE" )
            zstrblank ( szData, 13, 1 );
//  Source Line # 464:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "DerivedF" );
//  Source Line # 465:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "DerivedF" )
            zstrblank ( szData, 34, 1 );
//  Source Line # 467:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "Init" );
//  Source Line # 468:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "Init" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 470:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "Hidden" );
//  Source Line # 471:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "Hidden" )
            zstrblank ( szData, 9, 1 );
//  Source Line # 473:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "SeqPriority" );
//  Source Line # 474:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "SeqPriority" )
            zstrblank ( szData, 20, 1 );
//  Source Line # 476:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "SeqOrder" );
//  Source Line # 477:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_AttributeRec", "SeqOrder" )
            zstrblank ( szData, 17, 1 );
//  Source Line # 479:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 481:             FOR EACH vOI.ER_AttributeRec
            RESULT = SetCursorFirstEntity( vOI, "ER_AttributeRec", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
//  Source Line # 482:                Line = "d        ER_AttributeRec                "
               ZeidonStringCopy( Line, 1, 0, "d        ER_AttributeRec                ", 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "Name" );
//  Source Line # 483:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "Name" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 485:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ZKey" );
//  Source Line # 486:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ZKey" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 488:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "Lth" );
//  Source Line # 489:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "Lth" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 491:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "Key" );
//  Source Line # 492:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "Key" )
               zstrblank ( szData, 8, 1 );
//  Source Line # 494:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "NotNull" );
//  Source Line # 495:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "NotNull" )
               zstrblank ( szData, 11, 1 );
//  Source Line # 497:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "Desc" );
//  Source Line # 498:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "Desc" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 500:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "PromptPaintLit" );
//  Source Line # 501:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "PromptPaintLit" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 503:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "PromptPaintLth" );
//  Source Line # 504:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "PromptPaintLth" )
               zstrblank ( szData, 21, 1 );
//  Source Line # 506:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ListPaintLit" );
//  Source Line # 507:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ListPaintLit" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 509:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ListPaintLth" );
//  Source Line # 510:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ListPaintLth" )
               zstrblank ( szData, 20, 1 );
//  Source Line # 512:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_Position" );
//  Source Line # 513:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_Position" )
               zstrblank ( szData, 19, 1 );
//  Source Line # 515:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_OI_Token" );
//  Source Line # 516:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_OI_Token" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 518:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_AI_EntityToken" );
//  Source Line # 519:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_AI_EntityToken" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 521:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_AI_DT_Token" );
//  Source Line # 522:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_AI_DT_Token" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 524:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_OI_DT_Token" );
//  Source Line # 525:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "ADW_OI_DT_Token" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 527:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Decimal" );
//  Source Line # 528:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Decimal" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 530:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Type" );
//  Source Line # 531:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Type" )
               zstrblank ( szData, 8, 1 );
//  Source Line # 533:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Domain" );
//  Source Line # 534:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Domain" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 536:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Column" );
//  Source Line # 537:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Column" )
               zstrblank ( szData, 21, 1 );
//  Source Line # 539:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Order" );
//  Source Line # 540:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_Order" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 542:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_SysType" );
//  Source Line # 543:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_SysType" )
               zstrblank ( szData, 8, 1 );
//  Source Line # 545:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_DilKey" );
//  Source Line # 546:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_DilKey" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 548:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_ErrKey" );
//  Source Line # 549:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_AttributeRec", "t6B_ErrKey" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 552:                   /* Attribute "ER_AttributeRec.t6B_HelpKey" suppressed due to Line length. */
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 554:                FOR EACH vOI.DomainRec
               RESULT = SetCursorFirstEntity( vOI, "DomainRec", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               {
//  Source Line # 555:                   Line = "d           DomainRec                   "
                  ZeidonStringCopy( Line, 1, 0, "d           DomainRec                   ", 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "ZKey" );
//  Source Line # 556:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "ZKey" )
                  zstrblank ( szData, 14, 1 );
//  Source Line # 558:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "Name" );
//  Source Line # 559:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "Name" )
                  zstrblank ( szData, 35, 1 );
//  Source Line # 561:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "Desc" );
//  Source Line # 562:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "Desc" )
                  zstrblank ( szData, 33, 1 );
//  Source Line # 564:                   Line = Line + szData + "  "
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  ZeidonStringConcat( Line, 1, 0, "  ", 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "DomainType" );
//  Source Line # 565:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "DomainType" )
                  zstrblank ( szData, 8, 1 );
//  Source Line # 567:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "DataType" );
//  Source Line # 568:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "DataType" )
                  zstrblank ( szData, 8, 1 );
//  Source Line # 570:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "MaxStringLth" );
//  Source Line # 571:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "MaxStringLth" )
                  zstrblank ( szData, 14, 1 );
//  Source Line # 573:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "ContextRestriction" );
//  Source Line # 574:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "ContextRestriction" )
                  zstrblank ( szData, 4, 1 );
//  Source Line # 576:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "DLL_Name" );
//  Source Line # 577:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "DLL_Name" )
                  zstrblank ( szData, 35, 1 );
//  Source Line # 579:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "OperName" );
//  Source Line # 580:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "OperName" )
                  zstrblank ( szData, 35, 1 );
//  Source Line # 582:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "DecimalFormat" );
//  Source Line # 583:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "DomainRec", "DecimalFormat" )
                  zstrblank ( szData, 14, 1 );
//  Source Line # 585:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 554:
                  RESULT = SetCursorNextEntity ( vOI, "DomainRec", "" );
//  Source Line # 586:                   SysWriteLine( lRptFile, Line )
               }
//  Source Line # 481:
               RESULT = SetCursorNextEntity ( vOI, "ER_AttributeRec", "" );
//  Source Line # 587:                END   /* of Data print for entity 'DomainRec' */
            }
//  Source Line # 451:
            RESULT = SetCursorNextEntity ( vOI, "LOD_AttributeRec", "" );
//  Source Line # 588:             END   /* of Data print for entity 'ER_AttributeRec' */
         }
//  Source Line # 590:          FOR EACH vOI.ER_EntityRec
         RESULT = SetCursorFirstEntity( vOI, "ER_EntityRec", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 591:             Line = "d     ER_EntityRec                      "
            ZeidonStringCopy( Line, 1, 0, "d     ER_EntityRec                      ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "Name" );
//  Source Line # 592:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "Name" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 594:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "ZKey" );
//  Source Line # 595:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 597:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "Purpose" );
//  Source Line # 598:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "Purpose" )
            zstrblank ( szData, 15, 1 );
//  Source Line # 600:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "Desc" );
//  Source Line # 601:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "Desc" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 603:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "OwningArea" );
//  Source Line # 604:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "OwningArea" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 606:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "ADW_Position" );
//  Source Line # 607:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "ADW_Position" )
            zstrblank ( szData, 19, 1 );
//  Source Line # 609:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "ADW_OI_Token" );
//  Source Line # 610:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_EntityRec", "ADW_OI_Token" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 612:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 590:
            RESULT = SetCursorNextEntity ( vOI, "ER_EntityRec", "" );
//  Source Line # 613:             SysWriteLine( lRptFile, Line )
         }
//  Source Line # 615:          FOR EACH vOI.ER_RelLinkRec
         RESULT = SetCursorFirstEntity( vOI, "ER_RelLinkRec", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 616:             Line = "d     ER_RelLinkRec                     "
            ZeidonStringCopy( Line, 1, 0, "d     ER_RelLinkRec                     ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLinkRec", "Name" );
//  Source Line # 617:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLinkRec", "Name" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 619:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLinkRec", "ZKey" );
//  Source Line # 620:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLinkRec", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 622:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLinkRec", "CardMin" );
//  Source Line # 623:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLinkRec", "CardMin" )
            zstrblank ( szData, 18, 1 );
//  Source Line # 625:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLinkRec", "CardMax" );
//  Source Line # 626:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLinkRec", "CardMax" )
            zstrblank ( szData, 18, 1 );
//  Source Line # 628:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 630:             FOR EACH vOI.ER_RelTypeRec
            RESULT = SetCursorFirstEntity( vOI, "ER_RelTypeRec", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
//  Source Line # 631:                Line = "d        ER_RelTypeRec                  "
               ZeidonStringCopy( Line, 1, 0, "d        ER_RelTypeRec                  ", 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelTypeRec", "ZKey" );
//  Source Line # 632:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelTypeRec", "ZKey" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 634:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelTypeRec", "Fact" );
//  Source Line # 635:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelTypeRec", "Fact" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 637:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelTypeRec", "ADW_Position" );
//  Source Line # 638:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelTypeRec", "ADW_Position" )
               zstrblank ( szData, 19, 1 );
//  Source Line # 640:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelTypeRec", "ADW_AI_Token" );
//  Source Line # 641:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelTypeRec", "ADW_AI_Token" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 643:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 630:
               RESULT = SetCursorNextEntity ( vOI, "ER_RelTypeRec", "" );
//  Source Line # 644:                SysWriteLine( lRptFile, Line )
            }
//  Source Line # 615:
            RESULT = SetCursorNextEntity ( vOI, "ER_RelLinkRec", "" );
//  Source Line # 645:             END   /* of Data print for entity 'ER_RelTypeRec' */
         }
//  Source Line # 336:
         RESULT = SetCursorNextEntity ( vOI, "LOD_EntityParent", "" );
//  Source Line # 646:          END   /* of Data print for entity 'ER_RelLinkRec' */
      }
//  Source Line # 648:       FOR EACH vOI.LOD_Entity
      RESULT = SetCursorFirstEntity( vOI, "LOD_Entity", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
//  Source Line # 649:          Line = "d  LOD_Entity                           "
         ZeidonStringCopy( Line, 1, 0, "d  LOD_Entity                           ", 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Name" );
//  Source Line # 650:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Name" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 652:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "IndentName" );
//  Source Line # 653:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "IndentName" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 655:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "IndentLvl" );
//  Source Line # 656:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "IndentLvl" )
         zstrblank ( szData, 15, 1 );
//  Source Line # 658:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "ZKey" );
//  Source Line # 659:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "ZKey" )
         zstrblank ( szData, 14, 1 );
//  Source Line # 661:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Create" );
//  Source Line # 662:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Create" )
         zstrblank ( szData, 9, 1 );
//  Source Line # 664:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Delete" );
//  Source Line # 665:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Delete" )
         zstrblank ( szData, 9, 1 );
//  Source Line # 667:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Update" );
//  Source Line # 668:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Update" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 670:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Include" );
//  Source Line # 671:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Include" )
         zstrblank ( szData, 10, 1 );
//  Source Line # 673:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "InclSrc" );
//  Source Line # 674:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "InclSrc" )
         zstrblank ( szData, 20, 1 );
//  Source Line # 676:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Exclude" );
//  Source Line # 677:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Exclude" )
         zstrblank ( szData, 10, 1 );
//  Source Line # 679:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Hidden" );
//  Source Line # 680:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Hidden" )
         zstrblank ( szData, 9, 1 );
//  Source Line # 682:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Recursive" );
//  Source Line # 683:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Recursive" )
         zstrblank ( szData, 12, 1 );
//  Source Line # 685:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "AutoCreate" );
//  Source Line # 686:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "AutoCreate" )
         zstrblank ( szData, 13, 1 );
//  Source Line # 688:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Derived" );
//  Source Line # 689:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Derived" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 691:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "SeqType" );
//  Source Line # 692:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "SeqType" )
         zstrblank ( szData, 16, 1 );
//  Source Line # 694:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Work" );
//  Source Line # 695:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Work" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 697:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "ParentDeleteBehave" );
//  Source Line # 698:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "ParentDeleteBehave" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 700:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Desc" );
//  Source Line # 701:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Entity", "Desc" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 703:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 705:          FOR EACH vOI.LOD_Attribute
         RESULT = SetCursorFirstEntity( vOI, "LOD_Attribute", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 706:             Line = "d     LOD_Attribute                     "
            ZeidonStringCopy( Line, 1, 0, "d     LOD_Attribute                     ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "ZKey" );
//  Source Line # 707:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 709:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "Work" );
//  Source Line # 710:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "Work" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 712:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "Update" );
//  Source Line # 713:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "Update" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 715:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "DerivedE" );
//  Source Line # 716:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "DerivedE" )
            zstrblank ( szData, 13, 1 );
//  Source Line # 718:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "DerivedF" );
//  Source Line # 719:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "DerivedF" )
            zstrblank ( szData, 34, 1 );
//  Source Line # 721:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "Init" );
//  Source Line # 722:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "Init" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 724:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "Hidden" );
//  Source Line # 725:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "Hidden" )
            zstrblank ( szData, 9, 1 );
//  Source Line # 727:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "SeqPriority" );
//  Source Line # 728:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "SeqPriority" )
            zstrblank ( szData, 20, 1 );
//  Source Line # 730:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "SeqOrder" );
//  Source Line # 731:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "LOD_Attribute", "SeqOrder" )
            zstrblank ( szData, 17, 1 );
//  Source Line # 733:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 735:             FOR EACH vOI.ER_Attribute
            RESULT = SetCursorFirstEntity( vOI, "ER_Attribute", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
//  Source Line # 736:                Line = "d        ER_Attribute                   "
               ZeidonStringCopy( Line, 1, 0, "d        ER_Attribute                   ", 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "Name" );
//  Source Line # 737:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "Name" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 739:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ZKey" );
//  Source Line # 740:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ZKey" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 742:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "Lth" );
//  Source Line # 743:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "Lth" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 745:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "Key" );
//  Source Line # 746:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "Key" )
               zstrblank ( szData, 8, 1 );
//  Source Line # 748:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "NotNull" );
//  Source Line # 749:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "NotNull" )
               zstrblank ( szData, 11, 1 );
//  Source Line # 751:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "Desc" );
//  Source Line # 752:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "Desc" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 754:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "PromptPaintLit" );
//  Source Line # 755:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "PromptPaintLit" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 757:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "PromptPaintLth" );
//  Source Line # 758:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "PromptPaintLth" )
               zstrblank ( szData, 21, 1 );
//  Source Line # 760:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ListPaintLit" );
//  Source Line # 761:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ListPaintLit" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 763:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ListPaintLth" );
//  Source Line # 764:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ListPaintLth" )
               zstrblank ( szData, 20, 1 );
//  Source Line # 766:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_Position" );
//  Source Line # 767:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_Position" )
               zstrblank ( szData, 19, 1 );
//  Source Line # 769:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_OI_Token" );
//  Source Line # 770:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_OI_Token" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 772:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_AI_EntityToken" );
//  Source Line # 773:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_AI_EntityToken" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 775:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_AI_DT_Token" );
//  Source Line # 776:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_AI_DT_Token" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 778:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_OI_DT_Token" );
//  Source Line # 779:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "ADW_OI_DT_Token" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 781:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Decimal" );
//  Source Line # 782:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Decimal" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 784:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Type" );
//  Source Line # 785:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Type" )
               zstrblank ( szData, 8, 1 );
//  Source Line # 787:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Domain" );
//  Source Line # 788:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Domain" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 790:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Column" );
//  Source Line # 791:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Column" )
               zstrblank ( szData, 21, 1 );
//  Source Line # 793:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Order" );
//  Source Line # 794:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_Order" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 796:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_SysType" );
//  Source Line # 797:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_SysType" )
               zstrblank ( szData, 8, 1 );
//  Source Line # 799:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_DilKey" );
//  Source Line # 800:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_DilKey" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 802:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_ErrKey" );
//  Source Line # 803:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Attribute", "t6B_ErrKey" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 806:                   /* Attribute "ER_Attribute.t6B_HelpKey" suppressed due to Line length. */
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 808:                FOR EACH vOI.Domain
               RESULT = SetCursorFirstEntity( vOI, "Domain", "" );
               while ( RESULT > zCURSOR_UNCHANGED )
               {
//  Source Line # 809:                   Line = "d           Domain                      "
                  ZeidonStringCopy( Line, 1, 0, "d           Domain                      ", 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "ZKey" );
//  Source Line # 810:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "ZKey" )
                  zstrblank ( szData, 14, 1 );
//  Source Line # 812:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "Name" );
//  Source Line # 813:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "Name" )
                  zstrblank ( szData, 35, 1 );
//  Source Line # 815:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "Desc" );
//  Source Line # 816:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "Desc" )
                  zstrblank ( szData, 35, 1 );
//  Source Line # 818:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "DomainType" );
//  Source Line # 819:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "DomainType" )
                  zstrblank ( szData, 8, 1 );
//  Source Line # 821:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "DataType" );
//  Source Line # 822:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "DataType" )
                  zstrblank ( szData, 8, 1 );
//  Source Line # 824:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "MaxStringLth" );
//  Source Line # 825:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "MaxStringLth" )
                  zstrblank ( szData, 14, 1 );
//  Source Line # 827:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "ContextRestriction" );
//  Source Line # 828:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "ContextRestriction" )
                  zstrblank ( szData, 4, 1 );
//  Source Line # 830:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "DLL_Name" );
//  Source Line # 831:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "DLL_Name" )
                  zstrblank ( szData, 35, 1 );
//  Source Line # 833:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "OperName" );
//  Source Line # 834:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "OperName" )
                  zstrblank ( szData, 35, 1 );
//  Source Line # 836:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "DecimalFormat" );
//  Source Line # 837:                   GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Domain", "DecimalFormat" )
                  zstrblank ( szData, 14, 1 );
//  Source Line # 839:                   Line = Line + szData
                  ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
                  nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 808:
                  RESULT = SetCursorNextEntity ( vOI, "Domain", "" );
//  Source Line # 840:                   SysWriteLine( lRptFile, Line )
               }
//  Source Line # 735:
               RESULT = SetCursorNextEntity ( vOI, "ER_Attribute", "" );
//  Source Line # 841:                END   /* of Data print for entity 'Domain' */
            }
//  Source Line # 705:
            RESULT = SetCursorNextEntity ( vOI, "LOD_Attribute", "" );
//  Source Line # 842:             END   /* of Data print for entity 'ER_Attribute' */
         }
//  Source Line # 844:          FOR EACH vOI.ER_Entity
         RESULT = SetCursorFirstEntity( vOI, "ER_Entity", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 845:             Line = "d     ER_Entity                         "
            ZeidonStringCopy( Line, 1, 0, "d     ER_Entity                         ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "Name" );
//  Source Line # 846:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "Name" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 848:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "ZKey" );
//  Source Line # 849:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 851:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "Purpose" );
//  Source Line # 852:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "Purpose" )
            zstrblank ( szData, 15, 1 );
//  Source Line # 854:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "Desc" );
//  Source Line # 855:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "Desc" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 857:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "OwningArea" );
//  Source Line # 858:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "OwningArea" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 860:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "ADW_Position" );
//  Source Line # 861:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "ADW_Position" )
            zstrblank ( szData, 19, 1 );
//  Source Line # 863:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "ADW_OI_Token" );
//  Source Line # 864:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_Entity", "ADW_OI_Token" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 866:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 844:
            RESULT = SetCursorNextEntity ( vOI, "ER_Entity", "" );
//  Source Line # 867:             SysWriteLine( lRptFile, Line )
         }
//  Source Line # 869:          FOR EACH vOI.ER_RelLink
         RESULT = SetCursorFirstEntity( vOI, "ER_RelLink", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 870:             Line = "d     ER_RelLink                        "
            ZeidonStringCopy( Line, 1, 0, "d     ER_RelLink                        ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLink", "Name" );
//  Source Line # 871:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLink", "Name" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 873:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLink", "ZKey" );
//  Source Line # 874:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLink", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 876:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLink", "CardMin" );
//  Source Line # 877:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLink", "CardMin" )
            zstrblank ( szData, 18, 1 );
//  Source Line # 879:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLink", "CardMax" );
//  Source Line # 880:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelLink", "CardMax" )
            zstrblank ( szData, 18, 1 );
//  Source Line # 882:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 884:             FOR EACH vOI.ER_RelType
            RESULT = SetCursorFirstEntity( vOI, "ER_RelType", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
//  Source Line # 885:                Line = "d        ER_RelType                     "
               ZeidonStringCopy( Line, 1, 0, "d        ER_RelType                     ", 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelType", "ZKey" );
//  Source Line # 886:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelType", "ZKey" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 888:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelType", "Fact" );
//  Source Line # 889:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelType", "Fact" )
               zstrblank ( szData, 35, 1 );
//  Source Line # 891:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelType", "ADW_Position" );
//  Source Line # 892:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelType", "ADW_Position" )
               zstrblank ( szData, 19, 1 );
//  Source Line # 894:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelType", "ADW_AI_Token" );
//  Source Line # 895:                GetStringFromAttribute( szData, zsizeof( szData ), vOI, "ER_RelType", "ADW_AI_Token" )
               zstrblank ( szData, 14, 1 );
//  Source Line # 897:                Line = Line + szData
               ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
               nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 884:
               RESULT = SetCursorNextEntity ( vOI, "ER_RelType", "" );
//  Source Line # 898:                SysWriteLine( lRptFile, Line )
            }
//  Source Line # 869:
            RESULT = SetCursorNextEntity ( vOI, "ER_RelLink", "" );
//  Source Line # 899:             END   /* of Data print for entity 'ER_RelType' */
         }
//  Source Line # 648:
         RESULT = SetCursorNextEntity ( vOI, "LOD_Entity", "" );
//  Source Line # 900:          END   /* of Data print for entity 'ER_RelLink' */
      }
//  Source Line # 902:       FOR EACH vOI.Operation
      RESULT = SetCursorFirstEntity( vOI, "Operation", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
//  Source Line # 903:          Line = "d  Operation                            "
         ZeidonStringCopy( Line, 1, 0, "d  Operation                            ", 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "Name" );
//  Source Line # 904:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "Name" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 906:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "ZKey" );
//  Source Line # 907:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "ZKey" )
         zstrblank ( szData, 14, 1 );
//  Source Line # 909:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "Desc" );
//  Source Line # 910:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "Desc" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 912:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "Type" );
//  Source Line # 913:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "Type" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 915:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "ReturnDesc" );
//  Source Line # 916:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "ReturnDesc" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 918:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "ReturnType" );
//  Source Line # 919:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "ReturnType" )
         zstrblank ( szData, 8, 1 );
//  Source Line # 921:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "ExecName" );
//  Source Line # 922:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "ExecName" )
         zstrblank ( szData, 11, 1 );
//  Source Line # 924:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "HeaderFile" );
//  Source Line # 925:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "HeaderFile" )
         zstrblank ( szData, 11, 1 );
//  Source Line # 927:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "OrdinalValue" );
//  Source Line # 928:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "OrdinalValue" )
         zstrblank ( szData, 14, 1 );
//  Source Line # 930:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "W_VML_FileName" );
//  Source Line # 931:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Operation", "W_VML_FileName" )
         zstrblank ( szData, 11, 1 );
//  Source Line # 933:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 935:          FOR EACH vOI.Parameter
         RESULT = SetCursorFirstEntity( vOI, "Parameter", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 936:             Line = "d     Parameter                         "
            ZeidonStringCopy( Line, 1, 0, "d     Parameter                         ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "ShortDesc" );
//  Source Line # 937:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "ShortDesc" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 939:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "Type" );
//  Source Line # 940:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "Type" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 942:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "PtrFlag" );
//  Source Line # 943:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "PtrFlag" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 945:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "UnsignedFlag" );
//  Source Line # 946:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "UnsignedFlag" )
            zstrblank ( szData, 8, 1 );
//  Source Line # 948:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "ZKey" );
//  Source Line # 949:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "Parameter", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 951:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 935:
            RESULT = SetCursorNextEntity ( vOI, "Parameter", "" );
//  Source Line # 952:             SysWriteLine( lRptFile, Line )
         }
//  Source Line # 954:          FOR EACH vOI.VML_Source_Ref
         RESULT = SetCursorFirstEntity( vOI, "VML_Source_Ref", "" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
//  Source Line # 955:             Line = "d     VML_Source_Ref                    "
            ZeidonStringCopy( Line, 1, 0, "d     VML_Source_Ref                    ", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source_Ref", "ZKey" );
//  Source Line # 956:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source_Ref", "ZKey" )
            zstrblank ( szData, 14, 1 );
//  Source Line # 958:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source_Ref", "Name" );
//  Source Line # 959:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source_Ref", "Name" )
            zstrblank ( szData, 11, 1 );
//  Source Line # 961:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source_Ref", "Desc" );
//  Source Line # 962:             GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source_Ref", "Desc" )
            zstrblank ( szData, 35, 1 );
//  Source Line # 964:             Line = Line + szData
            ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
            nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 954:
            RESULT = SetCursorNextEntity ( vOI, "VML_Source_Ref", "" );
//  Source Line # 965:             SysWriteLine( lRptFile, Line )
         }
//  Source Line # 902:
         RESULT = SetCursorNextEntity ( vOI, "Operation", "" );
//  Source Line # 966:          END   /* of Data print for entity 'VML_Source_Ref' */
      }
//  Source Line # 968:       FOR EACH vOI.VML_Source
      RESULT = SetCursorFirstEntity( vOI, "VML_Source", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
//  Source Line # 969:          Line = "d  VML_Source                           "
         ZeidonStringCopy( Line, 1, 0, "d  VML_Source                           ", 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source", "ZKey" );
//  Source Line # 970:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source", "ZKey" )
         zstrblank ( szData, 14, 1 );
//  Source Line # 972:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source", "Name" );
//  Source Line # 973:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source", "Name" )
         zstrblank ( szData, 11, 1 );
//  Source Line # 975:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source", "Desc" );
//  Source Line # 976:          GetStringFromAttribute( szData, zsizeof( szData ), vOI, "VML_Source", "Desc" )
         zstrblank ( szData, 35, 1 );
//  Source Line # 978:          Line = Line + szData
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = SysWriteLine( vOI,  lRptFile, Line );
//  Source Line # 968:
         RESULT = SetCursorNextEntity ( vOI, "VML_Source", "" );
//  Source Line # 979:          SysWriteLine( lRptFile, Line )
      }
//  Source Line # 321:
      RESULT = SetCursorNextEntity ( vOI, "LOD", "" );
//  Source Line # 980:       END   /* of Data print for entity 'VML_Source' */
   }
//  Source Line # 981:    END   /* of Data print for entity 'LOD' */
   nZRetCode = SysCloseFile( vOI, lRptFile, 0 );
//  Source Line # 985:
   return( 0 );
}

