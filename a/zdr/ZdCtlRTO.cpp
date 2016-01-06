/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlrto.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of C-interface operations to Zeidon
// Rich Edit Control.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlRte.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//./ ADD NAME=RT_GetLineCount
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetLineCount
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the number of lines in this Rich
//              Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    the number of lines in the control's text
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zLONG OPERATION
RT_GetLineCount( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
         return( pRE->GetLineCount( ) );

      TraceLineS( "drvr - Invalid control type for RT_GetLineCount ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetLine
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetLine
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the text at the specified line in
//              the Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lLineNbr    - Line number for which to retrieve text
//              pchBuffer   - Buffer into which to retrieve text
//              lMaxLth     - Maximum length of the return buffer ... this
//                            length must be at least 4-bytes long and
//                            must be long enough to include the null
//                            terminator.
//
//  REMARK:
//
//  RETURNS:    the number of bytes copied into the return buffer
//             -1 - Error locating control ... or fewer than 4-bytes
//                  specified for the maximum length ... or the line
//                  number specified was not greater than zero.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zLONG OPERATION
RT_GetLine( zVIEW   vSubtask,
            zCPCHAR cpcCtrlTag,
            zLONG   lLineNbr,
            zPCHAR  pchBuffer,
            zLONG   lMaxLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( lMaxLth > 4 && lLineNbr > 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         lMaxLth = pRE->GetLine( lLineNbr - 1, pchBuffer, lMaxLth - 1 );
         pchBuffer[ lMaxLth ] = 0;
      }

      TraceLineS( "drvr - Invalid control type for RT_GetLine ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetFirstVisibleLine
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetFirstVisibleLine
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the line number of the first e in
//              the Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    the number of bytes copied into the return buffer
//             -1 - Error locating control ... or fewer than 4-bytes
//                  specified for the maximum length.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zLONG OPERATION
RT_GetFirstVisibleLine( zVIEW   vSubtask,
                        zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
         return( pRE->GetFirstVisibleLine( ) + 1 );

      TraceLineS( "drvr - Invalid control type for RT_GetFirstVisibleLine ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_LineNumber
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_LineNumber
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the character position of the specified
//              line in the Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lLineNbr    - Contains the line number for the desired line
//                            in the text of the edit control (if 0, it
//                            it specifies the current line, i.e. the line
//                            that contains the caret).
//
//  REMARK: Call this function to retrieve the character position of a line
//          within this CRichEditCtrl object. The character position is the
//          number of characters from the beginning of the rich edit control
//          to the specified line.
//
//  RETURNS: The character position of the line specified in lLineNbr.
//            0 - the specified line number is greater then the number of
//                lines in the edit control.
//           -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zLONG OPERATION
RT_LineNumber( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zLONG   lLineNbr )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( lLineNbr > 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
         return( pRE->LineIndex( lLineNbr + 1 ) + 1 );

      TraceLineS( "drvr - Invalid control type for RT_LineNumber ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_LineFromChar
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_LineFromChar
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the line number containing the specified
//              character position in the Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lCharPos    - Contains the character position for the desired
//                            line in the text of the edit control (if 0, it
//                            it specifies the current line, i.e. the line
//                            that contains the caret).
//
//  REMARK: Call this function to retrieve the number of the line that
//          contains the character position within this CRichEditCtrl object.
//
//  RETURNS: The line number containing the specified character position.
//           -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zLONG OPERATION
RT_LineFromChar( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zLONG   lCharPos )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( lCharPos >= 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
         return( pRE->LineFromChar( lCharPos - 1 ) + 1 );

      TraceLineS( "drvr - Invalid control type for RT_LineFromChar ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_LineLength
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_LineLength
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the length of the specified line in
//              the Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lLineNbr    - Line number for which to get length (0 to get
//                            the length of the current line).
//
//  REMARK:
//
//  RETURNS:    the length of the line at the specified line number.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zLONG OPERATION
RT_LineLength( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zLONG   lLineNbr )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( lLineNbr >= 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
         return( pRE->LineLength( lLineNbr - 1 ) );

      TraceLineS( "drvr - Invalid control type for RT_LineLength ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_LineScroll
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_LineScroll
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation scrolls the Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lLines      - Number of lines to scroll vertically.
//              lCharacters - Number of characters to scroll horizontally.
//
//  REMARK: The control does not scroll vertically past the last line of
//          text in the control. If the current line plus the number of
//          lines specified by lLines exceeds the total number of lines
//          in the control, the value is adjusted so that the last line
//          of the control is scrolled to the top of the control.
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG OPERATION
RT_LineScroll( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zLONG   lLines,
               zLONG   lCharacters )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->LineScroll( lLines, lCharacters );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_LineScroll ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_CanUndo
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_CanUndo
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation determines if an undo is possible for this
//              Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    1 - undo is possible
//              0 - no undo
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
RT_CanUndo( zVIEW   vSubtask,
            zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->CanUndo( ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_CanUndo ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetModify
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetModify
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation determines if the Rich Edit control's data
//              has been modified.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    1 - the control's data has been modified.
//  RETURNS:    0 - the control's data has NOT been modified.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
RT_GetModify( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->GetModify( ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetModify ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetModify
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetModify
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the state of the modify flag in this
//              Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              bModified   - TRUE sets the control's data as modified
//                            FALSE sets the control's data as NOT modified
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
RT_SetModify( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              zBOOL   bModified )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->SetModify( bModified );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetModify ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetRect
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetRect
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation retrieves the formatting rectangle for
//              this Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              plTop       - pointer to Top of the formatting rectangle
//              plLeft      - pointer to Left of the formatting rectangle
//              plBottom    - pointer to Bottom of the formatting rectangle
//              plRight     - pointer to Right of the formatting rectangle
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT
RT_GetRect( zVIEW   vSubtask,
            zCPCHAR cpcCtrlTag,
            zPLONG  plTop,
            zPLONG  plLeft,
            zPLONG  plBottom,
            zPLONG  plRight )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      RECT rect;
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->GetRect( &rect );
         *plTop = rect.top;
         *plLeft = rect.left;
         *plRight = rect.right;
         *plBottom = rect.bottom;
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetRect ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetCharPos
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetCharPos
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the position (top-left corner) of a
//              specified character in a Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lCharPos    - Character for which to obtain position.
//              plX         - X position of the top-left corner of the
//                            character specified by lCharPos.
//              plY         - Y position of the top-left corner of the
//                            character specified by lCharPos.
//
//  REMARK: Call this function to get the position (top-left corner) of a
//          given character within this CRichEditCtrl object. The character
//          is specified by giving its one-based index value. If lChar is
//          greater than the position of the last character in this
//          CRichEditCtrl object, the return value specifies the coordinates
//          of the character position just past the last character in this
//          CRichEditCtrl object.
//
//  RETURNS:   0 - OK
//            -1 - Error locating control ... or lCharPos less than 1
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
RT_GetCharPos( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zLONG   lCharPos,
               zPLONG  plX,
               zPLONG  plY )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( lCharPos > 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         CPoint pt = pRE->GetCharPos( lCharPos - 1 );
         *plX = pt.x;
         *plY = pt.y;
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetCharPos ",
                  cpcCtrlTag );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//./ ADD NAME=RT_SetOptions
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetOptions
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the options for the Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              uOp         - Indicates the type of operation. One of the
//                            following values:
//                 ECOOP_SET  Set the options to those specified by dwFlags.
//                 ECOOP_OR   Combine the current options with those
//                            specified by dwFlags.
//                 ECOOP_AND  Retain only those current options that are also
//                            specified by dwFlags.
//                 ECOOP_XOR  Retain only those current options that are not
//                            specified by dwFlags.
//
//              ulFlags     - Rich edit options.  The flag values may be a
//                            combination of the following values:
//                 ECO_AUTOWORDSELECTION  Automatic word selection on
//                            double-click.
//                 ECO_AUTOVSCROLL  Automatically scrolls text to the right
//                            by 10 characters when the user types a
//                            character at the end of the line. When the user
//                            presses the ENTER key, the control scrolls all
//                            text back to position zero.
//                 ECO_AUTOHSCROLL  Automatically scrolls text up one page
//                            when the user presses the ENTER key on the last
//                            line.
//                 ECO_NOHIDESEL  Negates the default behavior for an edit
//                            control. The default behavior hides the
//                            selection when the control loses the input
//                            focus and shows the selection when the control
//                            receives the input focus.  If you specify
//                            ECO_NOHIDESEL, the selected text is inverted,
//                            even if the control does not have the focus.
//                 ECO_READONLY  Prevents the user from typing or editing
//                            text in the edit control.
//                 ECO_WANTRETURN  Specifies that a carriage return be
//                            inserted when the user presses the ENTER key
//                            while entering text into a multiple-line rich
//                            edit control in a dialog box.  If you do not
//                            specify this style, pressing the ENTER key
//                            sends a command to the rich edit control's
//                            parent window, which mimics clicking the parent
//                            window's default button (for example, the OK
//                            button in a dialog box).  This style has no
//                            effect on a single-line edit control.
//                 ECO_SAVESEL  Preserves the selection when the control
//                            loses the focus.  By default, the entire
//                            contents of the control are selected when it
//                            regains the focus.
//                 ECO_VERTICAL  Draws text and objects in a vertical
//                            direction.  Available for Asian languages only.
//
//  REMARK:
//
//  RETURNS:    the number of lines in the control's text
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
RT_SetOptions( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zUSHORT uOp,
               zULONG  ulFlags )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->SetOptions( uOp, ulFlags );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetOptions ",
                  cpcCtrlTag );
   }

   return( -1 );
}


//./ ADD NAME=RT_CanPaste
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_CanPaste
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation determines if the clipboard format can be
//              pasted into this Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//              nFormat     - The Clipboard data format to query. This
//                            parameter can be one of the predefined
//                            Clipboard formats or the value returned by
//                            RegisterClipboardFormat.
//
//  REMARK:
//
//  RETURNS:    1 - the format can be pasted
//              0 - the format can NOT be pasted
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_CanPaste( zVIEW   vSubtask,
             zCPCHAR cpcCtrlTag,
             UINT    nFormat )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->CanPaste( nFormat ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_CanPaste ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetSel
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetSel
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation retrieves the bounds of the current
//              selection in this Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              plStartChar - Position of the first selected character
//              plEndChar   - Position of the last selected character
//
//  REMARK: The selection includes everything if the beginning (plStartChar)
//          is 1 and the end (plEndChar) is 0.
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_GetSel( zVIEW   vSubtask,
           zCPCHAR cpcCtrlTag,
           zPLONG  plStartChar,
           zPLONG  plEndChar )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         long lStart;
         long lEnd;

         pRE->GetSel( lStart, lEnd );
         *plStartChar = lStart + 1;
         *plEndChar = lEnd + 1;
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetSel ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_LimitText
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_LimitText
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation limits the amount of text the user can enter
//              into this Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lChars      - Specifies the length (in bytes) of the text
//                            that the user can enter.  If this parameter
//                            is 0, the text length is set to UINT_MAX bytes.
//                            This is the default behavior.
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control ... or lChars is less than zero.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_LimitText( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              zLONG   lChars )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( lChars >= 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->LimitText( lChars );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_LimitText ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetSel
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetSel
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the number of lines in this Rich
//              Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lStartChar  - First character position for the selection.
//              lEndChar    - Last character position for the selection.
//
//  REMARK: To select all the text in this control, call SetSel with a start
//          index of 1 and an end index of 0.
//          This function does not scroll the contents of the control so that
//          the caret is visible.
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_SetSel( zVIEW   vSubtask,
           zCPCHAR cpcCtrlTag,
           zLONG   lStartChar,
           zLONG   lEndChar )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( lStartChar > 0 && lEndChar >= 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->SetSel( lStartChar + 1, lEndChar + 1 );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetSel ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetDefaultCharFormat
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetDefaultCharFormat
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the default character formatting
//              attributes of this Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cf          - Pointer to a CHARFORMAT structure to receive
//                            the default character formatting attributes.
//
//  REMARK:
//
//  RETURNS:   The dwMask data member of cf.  It specified the default
//                  character formatting attributes.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zULONG
RT_GetDefaultCharFormat( zVIEW   vSubtask,
                         zCPCHAR cpcCtrlTag,
                         CHARFORMAT *cf )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->GetDefaultCharFormat( *cf ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetDefaultCharFormat ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetSelectionCharFormat
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetSelectionCharFormat
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the number of lines in this Rich
//              Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:   The dwMask data member of cf.  It specifies the character
//                  formatting attributes that are consistent throughout the
//                  current selection.
//
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zULONG
RT_GetSelectionCharFormat( zVIEW   vSubtask,
                           zCPCHAR cpcCtrlTag,
                           CHARFORMAT *cf )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->GetSelectionCharFormat( *cf ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetSelectionCharFormat ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetEventMask
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetEventMask
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the event mask for this Rich Edit
//              control.  The event mask specifies which notification
//              messages the control sends to its parent window.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:   The event mask for this control.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zLONG
RT_GetEventMask( zVIEW    vSubtask,
                 zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->GetEventMask( ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetEventMask ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetLimitText
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetLimitText
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the current limit in bytes for this
//              Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:   The current text limit in bytes
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zLONG
RT_GetLimitText( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->GetLimitText( ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetLimitText ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetParaFormat
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetParaFormat
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the paragraph formatting attributes of
//              the current selection in this Rich Edit control's text.  If
//              more than one paragraph is selected, pf receives the
//              attributes of the first selected paragraph.  The return value
//              specifies which attributes are consistent throughout the
//              selection.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              pf          - Pointer to a PARAFORMAT structure to hold the
//                            paragraph formatting attributes of the current
//                            selection.
//  REMARK:
//
//  RETURNS:   The dwMask data member of pf.  It specifies the paragraph
//                  formatting attributes that are consistent throughout
//                  the current selection.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zULONG
RT_GetParaFormat( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  PARAFORMAT *pf )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->GetParaFormat( *pf ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetParaFormat ", cpcCtrlTag );
   }

   return( -1 );
}

// richedit EM_GETSELTEXT is ANSI
//./ ADD NAME=RT_GetSelText
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SRT_GetSelText
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation retrieves the text from the current selection
//              in this Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              pchBuffer   - Return buffer containing selected text.
//              lMaxLth     - Maximum length the return buffer can hold.
//
//  REMARK:
//
//  RETURNS:   The length returned in the buffer.
//             -1 - Error locating control ... or buffer size insufficient
//                  to hold all the data from the current selection.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG
RT_GetSelText( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zPCHAR  pchBuffer,
               zLONG   lMaxLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( lMaxLth > 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         CString zs = pRE->GetSelText( );
         if ( lMaxLth > zs.GetLength( ) )
         {
            strcpy_s( pchBuffer, lMaxLth, zs );
            return( zs.GetLength( ) );
         }
         else
         {
            strncpy_s( pchBuffer, lMaxLth, zs, lMaxLth - 1 );
            pchBuffer[ lMaxLth - 1 ] = 0;
            return( -1 );
         }
      }

      TraceLineS( "drvr - Invalid control type for RT_GetSelText ",  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetSelectionType
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetSelectionType
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation determines the selection type in this
//              Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:   Flags indicating the contents of the current selection.  A
//             combination of the following flags:
//               SEL_EMPTY   Indicates that there is no current selection.
//               SEL_TEXT    Indicates that the current selection contains
//                           text.
//               SEL_OBJECT  Indicates that the current selection contains
//                           at least one OLE item.
//               SEL_MULTICHAR Indicates that the current selection contains
//                           more than one character of text.
//               SEL_MULTIOBJECT Indicates that the current selection
//                           contains more than one OLE object.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zULONG
RT_GetSelectionType( zVIEW   vSubtask,
                     zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->GetSelectionType( ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetSelectionType ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetBackgroundColor
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetBackgroundColor
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets background color for this Rich
//              Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              bSysColor   - Indicates if the background color should be set
//                            to the system value.  If this value is TRUE,
//                            lRGB_Color is ignored.
//              lRGB_Color  - The requested background color.  Used only if
//                            bSysColor is FALSE.
//
//  REMARK:
//
//  RETURNS:   The previous background color of this control.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG
RT_SetBackgroundColor( zVIEW   vSubtask,
                       zCPCHAR cpcCtrlTag,
                       zBOOL   bSysColor,
                       zLONG   lRGB_Color )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( (COLORREF) pRE->SetBackgroundColor( bSysColor,
                                                     (COLORREF) lRGB_Color ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetBackgroundColor ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetDefaultCharFormat
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetDefaultCharFormat
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the character formatting attributes for
//              new text in this Rich Edit control.  Only the attributes
//              specified by the dwMask member of cf are changed by this
//              function.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cf          - CHARFORMAT structure containing the new default
//                            character formatting attributes.
//
//  REMARK:
//
//  RETURNS:    1 - character formatting attributes set successfully
//              0 - character formatting attributes NOT set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_SetDefaultCharFormat( zVIEW   vSubtask,
                         zCPCHAR cpcCtrlTag,
                         CHARFORMAT *cf )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->SetDefaultCharFormat( *cf ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetDefaultCharFormat ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetSelectionCharFormat
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetSelectionCharFormat
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the character formatting attributes for
//              selected text in this Rich Edit control.  Only the attributes
//              specified by the dwMask member of cf are changed by this
//              function.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cf          - CHARFORMAT structure containing the new
//                            character formatting attributes for the
//                            current selection.
//
//  REMARK:
//
//  RETURNS:    1 - character formatting attributes set successfully
//              0 - character formatting attributes NOT set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_SetSelectionCharFormat( zVIEW   vSubtask,
                           zCPCHAR cpcCtrlTag,
                           CHARFORMAT *cf )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->SetSelectionCharFormat( *cf ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetSelectionCharFormat ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetWordCharFormat
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetWordCharFormat
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the character formatting attributes for
//              the currently selected word in this Rich Edit control.  Only
//              the attributes specified by the dwMask member of cf are
//              changed by this function.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cf          - CHARFORMAT structure containing the new
//                            character formatting attributes for the
//                            currently selected word.
//
//  REMARK:
//
//  RETURNS:    1 - character formatting attributes set successfully
//              0 - character formatting attributes NOT set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_SetWordCharFormat( zVIEW   vSubtask,
                      zCPCHAR cpcCtrlTag,
                      CHARFORMAT *cf )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->SetWordCharFormat( *cf ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetWordCharFormat ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetEventMask
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetEventMask
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the event mask for this Rich Edit control.
//              The event mask specifies which notification messages the
//              Rich Edit control sends to its parent window.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              ulEventMask - The new event mask for this rich edit control.
//
//  REMARK:
//
//  RETURNS:   The previous event mask.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zULONG
RT_SetEventMask( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zULONG  ulEventMask )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->SetEventMask( ulEventMask ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetEventMask ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetParaFormat
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetParaFormat
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the paragraph formatting attributes for
//              the current selection in this Rich Edit control.  Only the
//              attributes specified by the dwMask member of pf are changed
//              by this function.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              pf          - PARAFORMAT structure containing the new default
//                            paragraph formatting attributes.
//
//  REMARK:
//
//  RETURNS:    1 - character formatting attributes set successfully
//              0 - character formatting attributes NOT set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_SetParaFormat( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  PARAFORMAT *pf )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->SetParaFormat( *pf ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetParaFormat ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetTargetDevice
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetTargetDevice
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the target device and line width used for
//              WYSIWYG (what you see is what you get) formatting in this
//              Rich Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              hDC         - Handle to the device context for the new target
//                            device.
//              lLineWidth  - Line width to use for formatting.
//
//  REMARK: If this function is successful, the rich edit control owns the
//          device context passed as a parameter. In that case, the calling
//          function should not destroy the device context.
//
//  RETURNS:    1 - target device successfully set.
//              0 - target device NOT successfully set.
//             -1 - Error locating control ... or hDC is null ...
//                  or lLineWidth is not positive.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zBOOL
RT_SetTargetDevice( zVIEW   vSubtask,
                    zCPCHAR cpcCtrlTag,
                    HDC     hDC,
                    zLONG   lLineWidth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( hDC && lLineWidth > 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->SetTargetDevice( hDC, lLineWidth ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetTargetDevice ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_GetTextLength
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_GetTextLength
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation gets the length of the text in this Rich
//              Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:   The length of the text in the rich edit control.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG
RT_GetTextLength( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->GetTextLength( ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_GetTextLength ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetReadOnly
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetReadOnly
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the read only attribute for this
//              Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              bReadOnly   - TRUE - the text should be read only (not
//                                   updateable).
//                            FALSE -  the text should be updateable.
//
//  REMARK:
//
//  RETURNS:    1 - the read only attribute was set successfully
//              0 - the read only attribute was NOT set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_SetReadOnly( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zBOOL   bReadOnly )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->SetReadOnly( bReadOnly ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetReadOnly ",
                  cpcCtrlTag );
   }

   return( -1 );
}

// Operations
//./ ADD NAME=RT_EmptyUndoBuffer
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_EmptyUndoBuffer
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation resets (clears) the undo flag of this Rich
//              Edit control.  The control will now be unable to undo the
//              last editing operation.  The undo flag is set whenever an
//              operation within the rich edit control can be undone.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK: The undo flag is automatically cleared whenever you call the
//          function SetWindowText.
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_EmptyUndoBuffer( zVIEW   vSubtask,
                    zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->EmptyUndoBuffer( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_EmptyUndoBuffer ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_ReplaceSel
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_ReplaceSel
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation replaces the current selection in this
//              Rich Edit control with the specified text.  To replace
//              all the text in this control, use RT_SetWindowText.
//              Edit control's text.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cpcNewText  - Pointer to a null-terminated string containing
//                            the replacement text.
//              bCanUndo    - To specify that this function can be undone,
//                            set the value of this parameter to TRUE.
//
//  REMARK: If there is no current selection, the replacement text is
//          inserted at the insertion point, that is, the current caret
//          location.
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_ReplaceSel( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zCPCHAR cpcNewText,
               zBOOL   bCanUndo )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->ReplaceSel( cpcNewText, bCanUndo );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_ReplaceSel ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_SetRect
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_SetRect
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation sets the formatting rectangle for this
//              Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lpRect      - Pointer to a RECT that indicates the new bounds
//                            for the formatting rectangle.
//
//  REMARK: The formatting rectangle is the limiting rectangle for the text.
//          The limiting rectangle is independent of the size of the Rich
//          Edit control window.  When this Rich Edit control is first
//          created, the formatting rectangle is the same size as the client
//          area of the window.  Use RT_SetRect to make the formatting
//          rectangle larger or smaller than the Rich Edit window.
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_SetRect( zVIEW   vSubtask,
            zCPCHAR cpcCtrlTag,
            LPCRECT lpRect )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->SetRect( lpRect );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_SetRect ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_DisplayBand
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_DisplayBand
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation displays a portion of the contents of the
//              Rich Edit control (text and OLE items), as previously
//              formatted by FormatRange.  The text and OLE items are clipped
//              to the area specified by the pointer pDisplayRect.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              pDisplayRect - Pointer to a RECT specifying the area of the
//                            device to display the text.
//
//  REMARK:
//
//  RETURNS:    1 - display of the formatted text successful.
//              0 - display of the formatted text NOT successful.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
RT_DisplayBand( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                LPRECT  pDisplayRect )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->DisplayBand( pDisplayRect ) ? 1 : 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_DisplayBand ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_FindText
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_FindText
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation locates text within the Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              dwFlags     - Flags for the match criteria. Can be zero or
//                            more of the following values:
//                FR_MATCHCASE  Indicates that the search is case sensitive.
//                FR_WHOLEWORD  Indicates that the word boundaries should be
//                              considered in the search.
//              pFindText   - Pointer to the FINDTEXTEX structure giving the
//                            parameters for the search and returning the
//                            range where the match was found.
//
//  REMARK: You can search either up or down by setting the proper range
//          parameters in the CHARRANGE structure within the FINDTEXTEX
//          structure.
//
//  RETURNS:   Character position of the next match
//              0   if there are no more matches.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG
RT_FindText( zVIEW   vSubtask,
             zCPCHAR cpcCtrlTag,
             zULONG  ulFlags,
             FINDTEXTEX *pFindText )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->FindText( ulFlags, pFindText ) + 1 );
      }

      TraceLineS( "drvr - Invalid control type for RT_FindText ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_FormatRange
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_FormatRange
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation formats a range of text in this Rich
//              Edit control for a specific device. Typically, this call
//              is followed by a call to DisplayBand.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              pfr         - Pointer to the FORMATRANGE structure which
//                            contains information about the output device.
//                            NULL indicates that cached information within
//                            the Rich Edit control can be freed.
//              bDisplay    - Indicates if the text should be rendered.  If
//                            FALSE, the text is just measured.
//
//  REMARK:
//
//  RETURNS:   The position of the last character that fits in the region.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG
RT_FormatRange( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                FORMATRANGE *pfr,
                zBOOL   bDisplay )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->FormatRange( pfr, bDisplay ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_FormatRange ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_HideSelection
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_HideSelection
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation changes the visiblilty of the selected text
//              in this Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              bHide       - Indicates if the selection should be shown or
//                            hidden, TRUE to hide the selection.
//              bPerm       - Indicates if this change in visibility for the
//                            selection should be permanent.
//
//  REMARK: When bPerm is TRUE, it changes the ECO_NOHIDESEL option for this
//          Rich Edit control.  For a brief description of this option, see
//          SetOptions.  You can use this function to set all the options for
//          this Rich Edit control.
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_HideSelection( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  zBOOL   bHide,
                  zBOOL   bPerm )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->HideSelection( bHide, bPerm );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_HideSelection ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_PasteSpecial
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_PasteSpecial
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation pastes data in a specific Clipboard format
//              into this Rich Edit control.  The new material is inserted
//              at the insertion point ...the location of the caret.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lClipFormat - Clipboard format to paste into this control.
//              dvAspect    - Device aspect for the data to be retrieved from
//                            the Clipboard.
//              hMF         - Handle to the metafile containing the iconic
//                            view of the object to be pasted.
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
RT_PasteSpecial( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zULONG  ulClipFormat,
                 zULONG  dvAspect,
                 HMETAFILE hMF )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->PasteSpecial( ulClipFormat, dvAspect, hMF );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_PasteSpecial ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_RequestResize
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_RequestResize
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation forces this Rich Edit control to send
//              WM_REQUESTRESIZE notification messages to its parent window.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
RT_RequestResize( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->RequestResize( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_RequestResize ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_StreamIn
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_StreamIn
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation replaces text in this Rich Edit control with
//              text from the specified input stream.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lFormat     - Flags specifying the input data formats.  The
//                            value of lFormat must be one of the following:
//                   SF_TEXT  Indicates reading text only.
//                   SF_RTF   Indicates reading text and formatting.
//              es          - EDITSTREAM structure specifying the input
//                            stream.
//
//  REMARK: Either of the lFormat values can be combined with SFF_SELECTION.
//          If SFF_SELECTION is specified, StreamIn replaces the current
//          selection with the contents of the input stream.  If it is not
//          specified, StreamIn replaces the entire contents of this Rich
//          Edit control.
//          In the EDITSTREAM parameter es, you specify a callback function
//          which fills a buffer with text.  This callback function is called
//          repeatedly, until the input stream is exhausted.
//
//  RETURNS:   Number of characters read from the input stream.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG
RT_StreamIn( zVIEW   vSubtask,
             zCPCHAR cpcCtrlTag,
             zLONG   lFormat,
             EDITSTREAM *es )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->StreamIn( lFormat, *es ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_StreamIn ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=RT_StreamOut
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_StreamOut
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation writes out the contents of this Rich
//              Edit control to the specified output stream.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lFormat     - Flags specifying the output data formats.  The
//                            value of lFormat must be one of the following:
//                    SF_TEXT  Indicates writing text only.
//                    SF_RTF   Indicates writing text and formatting.
//                    SF_RTFNOOBJS  Indicates writing text and formatting,
//                            replacing OLE items with spaces.
//                    SF_TEXTIZED  Indicates writing text and formatting,
//                            with textual representations of OLE items.
//              es          - EDITSTREAM structure specifying the output
//                            stream
//
//  REMARK: Any of these values can be combined with SFF_SELECTION.  If
//          SFF_SELECTION is specified, StreamOut writes out the current
//          selection into the output stream.  If it is not specified,
//          StreamOut writes out the the entire contents of this Rich Edit
//          control.
//          In the EDITSTREAM parameter es, you specify a callback function
//          which fills a buffer with text.  This callback function is called
//          repeatedly, until the input stream is exhausted.
//
//  RETURNS:   Number of characters written to the output stream.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG
RT_StreamOut( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              zLONG   lFormat,
              EDITSTREAM *es )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         return( pRE->StreamOut( lFormat, *es ) );
      }

      TraceLineS( "drvr - Invalid control type for RT_StreamOut ",
                  cpcCtrlTag );
   }

   return( -1 );
}

// Clipboard operations
//./ ADD NART=RT_Undo
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_Undo
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation reverses the last operation performed on
//              this Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    1 - The undo is successful.
//              0 - The undo is NOT successful.
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_Undo( zVIEW   vSubtask,
         zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->Undo( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_Undo ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NARR=RT_Clear
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_Clear
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation deletes the current selected text in this
//              Rich Edit control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_Clear( zVIEW   vSubtask,
          zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->Clear( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_Clear ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NART=RT_Copy
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_Copy
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation copies the currently selected text in
//              this Rich Edit control to the Clipboard.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_Copy( zVIEW   vSubtask,
         zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->Copy( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_Copy ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NRT_=RT_Cut
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_Cut
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation deletes the currently selected text in
//              this Rich Edit control and copies it to the Clipboard.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_Cut( zVIEW   vSubtask,
        zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->Cut( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_Cut ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NRTR=RT_Paste
// Source Module=zdctlrto.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RT_Paste
//                                              02/02/98 Modified: 02/02/98
//
//  PURPOSE:    This operation inserts the data from the Clipboard into the
//              Rich Edit control at the insertion point ... the location of
//              the caret.  Data is inserted only if the Clipboard contains
//              data in a recognized format.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  REMARK:
//
//  RETURNS:    0 - OK
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
RT_Paste( zVIEW   vSubtask,
          zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZRichEdit *pRE = DYNAMIC_DOWNCAST( ZRichEdit, pzma->m_pCtrl );
      if ( pRE )
      {
         pRE->Paste( );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for RT_Paste ",
                  cpcCtrlTag );
   }

   return( -1 );
}
