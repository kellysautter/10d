#include "zstdafx.h"
#include "zcmddef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ZVMLCommand::ZVMLCommand( )
{
   m_strText = "Unknown";
   m_strInsertText = "Unknown";
   m_strInsertTextFull = "Unknown";
   m_strCC_Code = "Unknown";
   m_bCC_FirstChar = TRUE;   //command must be at the beginning of the line
}

ZVMLCommand::~ZVMLCommand()
{
}

ZVMLCommand::ZVMLCommand( zCPCHAR strText,
                          zCPCHAR strType,
                          zCPCHAR strInsertText,
                          zCPCHAR strInsertTextFull,
                          zCPCHAR strCC_Code,
                          zBOOL   bCC_FirstChar )
{
   m_strText            = strText;
   m_strType            = strType;
   m_strInsertText      = strInsertText;
   m_strInsertTextFull  = strInsertTextFull;
   m_strCC_Code         = strCC_Code;
   m_bCC_FirstChar      = bCC_FirstChar;
}
