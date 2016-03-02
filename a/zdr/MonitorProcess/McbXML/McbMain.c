// McbMain.c : Defines the entry point for the application.
//

/*
 ****************************************************************************
 * Include all necessary include files
 ****************************************************************************
 */
#define WIN32_LEAN_AND_MEAN   /* Exclude rarely-used stuff from headers */

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>    /* Windows Header Files */
#include <stdlib.h>        /* C RunTime Header Files */
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commctrl.h>
#include <commdlg.h>
#include <assert.h>
#include <richedit.h>
#include <shellapi.h>      /* for DragAcceptFiles */
#include "resource.h"
#include "McbXML.h"

/*
 ****************************************************************************
 * Structure used for options
 ****************************************************************************
 */
typedef struct tagMcbOptions
{
   TCHAR szLastFile[_MAX_PATH+1];
   int      bPanelsVertical;
   int      nFilterIndex;

} McbOptions;

/*
 ****************************************************************************
 * Global Variables
 ****************************************************************************
 */
#define     MAX_LOADSTRING 100
HINSTANCE   g_hInst;                   /* Current instance */
TCHAR    g_szTitle[MAX_LOADSTRING];       /* The title bar text */
TCHAR    g_szWindowClass[MAX_LOADSTRING]; /* Windows class name */
McbOptions  g_options;
TCHAR    g_szOptFile[_MAX_PATH+1];

/**
 ****************************************************************************
 * <P> Obtain the options filename.  </P>
 *
 * @methodName  McbGetOptionsFileName
 *
 * @param       lpszFile
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbGetOptionsFileName(LPTSTR lpszFile)
{
   TCHAR szFile[_MAX_PATH+1];
   TCHAR szDrive[_MAX_DRIVE+1];
   TCHAR szDir[_MAX_DIR+1];

   GetModuleFileName(NULL, szFile, _MAX_PATH+1);

   _tsplitpath(szFile, szDrive, szDir, NULL, NULL);

   _tcscpy(lpszFile, szDrive);
   lpszFile += _tcslen(szDrive);
   _tcscpy(lpszFile, szDir);
   lpszFile += _tcslen(szDir);

#define McbOPTIONSFILE  _T("Options.xml")
   _tcscpy(lpszFile, McbOPTIONSFILE);

}/* McbGetOptionsFileName */

/**
 ****************************************************************************
 * <P> Helper function to load a file into allocated memory.  </P>
 *
 * @methodName  McbLoadFile
 *
 * @param       lpszFile
 *
 * @return      LPTSTR
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
LPTSTR McbLoadFile(LPCTSTR lpszFile)
{
   LPTSTR lpszResult = NULL;
   HANDLE hFile;
   DWORD cbFile;
   DWORD cbRead;

   hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile != INVALID_HANDLE_VALUE)
   {
      cbFile = GetFileSize(hFile, NULL);

      if (cbFile)
      {
         lpszResult = malloc((cbFile+1) * zsizeof(TCHAR));
         lpszResult[cbFile] = 0;

         if (!ReadFile(hFile, lpszResult, cbFile, &cbRead, NULL) ||
            cbFile > cbRead)
         {
            free(lpszResult);
            lpszResult = NULL;
         }
      }

      CloseHandle(hFile);
   }

   return lpszResult;

}/* McbLoadFile */

/**
 ****************************************************************************
 * <P> Helper function to save to a file.  </P>
 *
 * @methodName  McbLoadFile
 *
 * @param       LPCTSTR lpszFile
 * @param       LPCTSTR lpszData
 *
 * @return      LPTSTR
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
int McbSaveFile(LPCTSTR lpszFile, LPCTSTR lpszData)
{
   int nResult = FALSE;
   HANDLE hFile;
   DWORD cbData;
   DWORD cbWrite;

   assert(lpszData && *lpszData);

   hFile = CreateFile(lpszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile != INVALID_HANDLE_VALUE)
   {
      cbData = _tcslen(lpszData);

      if (WriteFile(hFile, lpszData, cbData, &cbWrite, NULL) &&
         cbData <= cbWrite)
      {
         nResult = TRUE;
      }

      CloseHandle(hFile);
   }

   return nResult;

}/* McbSaveFile */

/**
 ****************************************************************************
 * <P> Add quotes round a string so it can be used as an attribute value.
 * </P>
 *
 * @methodName  McbAddQuotes
 *
 * @param       lpszData
 *
 * @return      LPTSTR
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
LPTSTR McbAddQuotes(LPCTSTR lpszData)
{
   LPTSTR lpszResult;
   int cbData;

   if (!lpszData || !(*lpszData))
   {
      lpszResult = malloc(3 * zsizeof(TCHAR));
      _tcscpy(lpszResult, _T("\"\""));
   }
   else
   {
      cbData = _tcslen(lpszData);
      lpszResult = malloc((cbData + 3)* zsizeof(TCHAR));

      *lpszResult = _T('\"');
      _tcscpy(lpszResult+1, lpszData);
      lpszResult[cbData+1] = _T('\"');
      lpszResult[cbData+2] = 0;
   }

   return lpszResult;

}/* McbAddQuotes */

/**
 ****************************************************************************
 * <P> Remove quotes from an attribute value.  </P>
 *
 * @methodName  McbRemoveQuotes
 *
 * @param       lpszData
 *
 * @return      LPTSTR
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
LPTSTR McbRemoveQuotes(LPCTSTR lpszData)
{
   LPTSTR lpszResult = NULL;
   int cbData, cbTemp;
   LPCTSTR lpszMarker;

   if (lpszData && *lpszData)
   {
      cbData = _tcslen(lpszData);
      cbTemp = cbData;
      lpszMarker = lpszData;

      if (lpszData[0] == _T('\"'))
      {
         lpszMarker++;
         cbTemp--;
      }

      if (cbTemp && (lpszData[cbData-1] == _T('\"')))
      {
         cbTemp--;
      }

      if (cbTemp)
      {
         lpszResult = malloc((cbTemp + 1) *zsizeof(TCHAR));
         _tcsncpy(lpszResult, lpszMarker, cbTemp);
         lpszResult[cbTemp] = 0;
      }
   }

   return lpszResult;

}/* McbRemoveQuotes */

/**
 ****************************************************************************
 * <P> Store or load options to an XML file.  </P>
 *
 * @methodName  McbArchiveOptions
 *
 * @param       lpszFile
 * @param       * pOptions
 * @param       nStore
 *
 * @return      int
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
int McbArchiveOptions(LPCTSTR lpszFile, McbOptions * pOptions, int nStore)
{
   int nResult = 0;
   McbXMLElement * pRoot;
   McbXMLElement * pChild;
   McbXMLAttribute * pAttr;
   LPTSTR lpszName;
   LPTSTR lpszValue;
   LPTSTR lpszXML;
   int cbXML;

   assert(pOptions);

   /*
    *************************************************************************
    * If we are storing to file.
    *************************************************************************
    */
   if (nStore)
   {
       /*
        *********************************************************************
        * Create the XML structure
        *********************************************************************
        */
      pRoot = McbCreateRoot();

      if (pRoot)
      {
           /*
            *****************************************************************
            * Add the head element
            *****************************************************************
            */
         pChild = McbCreateElements(pRoot, _T("XMLOptions"));

         if (pChild)
         {
               /*
                *************************************************************
                * Add vertical or horizontal panel
                *************************************************************
                */
            if (pOptions->bPanelsVertical)
            {
               lpszName = McbStrdup(_T("PanelsVertical"), 0);
            }
            else
            {
               lpszName = McbStrdup(_T("PanelsHorizontal"), 0);
            }

            McbAddAttribute(pChild, lpszName, NULL, 5);

               /*
                *************************************************************
                * Add the file options element
                *************************************************************
                */
            pChild = McbCreateElements(pChild, _T("FileOptions"));

            if (pChild)
            {
                   /*
                    *********************************************************
                    * Add the filename attribute
                    *********************************************************
                    */
               lpszName = McbStrdup(_T("Filename"), 0);
               lpszValue = McbAddQuotes(pOptions->szLastFile);

               McbAddAttribute(pChild, lpszName, lpszValue, 5);

                   /*
                    *********************************************************
                    * Add the filter index attribute
                    *********************************************************
                    */
               lpszName = McbStrdup(_T("FilterIndex"), 0);
               lpszValue = malloc(10 * zsizeof(TCHAR));
               _ltot(pOptions->nFilterIndex, lpszValue, 10);

               McbAddAttribute(pChild, lpszName, lpszValue, 5);

            }
         }

           /*
            *****************************************************************
            * Create string from the XML structure.
            *****************************************************************
            */
         lpszXML = McbCreateXMLString(pRoot, 1, &cbXML);

         if (lpszXML)
         {
               /*
                *************************************************************
                * Attempt to save the XML to file.
                *************************************************************
                */
            if (McbSaveFile(lpszFile, lpszXML))
            {
               nResult = TRUE;
            }

            free(lpszXML);
         }

         McbDeleteElement(pRoot);
      }
   }
   /*
    *************************************************************************
    * If we are reading from file
    *************************************************************************
    */
    else
    {
       /*
        *********************************************************************
        * Initialise the options structure.
        *********************************************************************
        */
      memset(pOptions, 0, zsizeof(McbOptions));

       /*
        *********************************************************************
        * Load the XML file.
        *********************************************************************
        */
      lpszXML = McbLoadFile(lpszFile);

      if (lpszXML)
      {
           /*
            *****************************************************************
            * Parse the XML into the tree structure
            *****************************************************************
            */
            pRoot = McbParseXML(lpszXML, NULL);

            if (pRoot)
            {
               /*
                *************************************************************
                * Find the head element
                *************************************************************
                */
            pChild = McbFindElement(pRoot, _T("XMLOptions"));

            if (pChild)
            {
               nResult = TRUE;

                   /*
                    *********************************************************
                    * Search for the vertical panel attribute, if this isnt
               * found then the assume horizontal panel
                    *********************************************************
                    */
               pAttr = McbFindAttribute(pChild, _T("PanelsVertical"));

               if (pAttr)
               {
                  pOptions->bPanelsVertical = TRUE;
               }

                   /*
                    *********************************************************
                    * Find the file options element
                    *********************************************************
                    */
               pChild = McbFindElement(pChild, _T("FileOptions"));

               if (pChild)
               {
                       /*
                        *****************************************************
                        * Obtain the filename attribute
                        *****************************************************
                        */
                  pAttr = McbFindAttribute(pChild, _T("Filename"));

                  if (pAttr && pAttr->lpszValue)
                  {
                     lpszValue = McbRemoveQuotes(pAttr->lpszValue);

                     if (lpszValue)
                     {
                        _tcscpy(pOptions->szLastFile, lpszValue);
                        free(lpszValue);
                     }
                  }

                       /*
                        *****************************************************
                        * Obtain the filter index attribute
                        *****************************************************
                        */
                  pAttr = McbFindAttribute(pChild, _T("FilterIndex"));

                  if (pAttr && pAttr->lpszValue)
                  {
                     pOptions->nFilterIndex = _ttol(pAttr->lpszValue);
                  }
               }
            }

            McbDeleteRoot(pRoot);
         }

         free(lpszXML);
      }
    }

   return nResult;

}/* McbArchiveOptions */

/*
 ****************************************************************************
 * Constants used withthe image list.
 ****************************************************************************
 */
#define McbATTRIBUTEBMP    0
#define McbCDATABMP        1
#define McbCLEARBMP        2
#define McbDECLARATIONBMP  3
#define McbELEMENTBMP      4
#define McbTEXTBMP         5

/*
 ****************************************************************************
 * Helper macro to set the status bar text.
 ****************************************************************************
 */
#define McbSETSTATUS(hWnd, lpszText, nPanel)                         \
   SendMessage((hWnd), SB_SETTEXT, (nPanel), (LPARAM)(lpszText))

/**
 ****************************************************************************
 * <P> Message handler for about box.  </P>
 *
 * @methodName  CALLBACK McbAbout
 *
 * @param       hDlg
 * @param       message
 * @param       wParam
 * @param       lParam
 *
 * @return      LRESULT
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 3rd February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
LRESULT CALLBACK McbAbout(HWND hDlg, UINT message, WPARAM wParam,
                   LPARAM lParam)
{
   switch (message)
   {
      case WM_INITDIALOG:
         return TRUE;

      case WM_COMMAND:
         if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
         {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
         }
         break;
   }

    return FALSE;

}/* CALLBACK McbAbout */

/**
 ****************************************************************************
 * <P> Saves instance handle and creates main window.
 *
 * In this function, we save the instance handle in a global variable and
 * create and display the main program window.  </P>
 *
 * @methodName  McbInitInstance
 *
 * @param       hInstance
 * @param       nCmdShow
 *
 * @return      BOOL
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 3rd February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
BOOL McbInitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

  /*
   **************************************************************************
   * Store instance handle in our global variable
   **************************************************************************
   */
   g_hInst = hInstance;

   hWnd = CreateWindow(g_szWindowClass, g_szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;

}/* McbInitInstance */

/**
 ****************************************************************************
 * <P> Replace non-printable characters with a space.  </P>
 *
 * @methodName  McbReplaceNonPrintableChars
 *
 * @param       lpszText
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 26th August       2001   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbReplaceNonPrintableChars(LPTSTR lpszText)
{
   LPTSTR lpszMarker = lpszText;

   if (lpszMarker)
   {
      for (; *lpszMarker; lpszMarker++)
      {
         switch(*lpszMarker)
         {
         case _T('\r'):
         case _T('\n'):
         case _T('\t'):
            *lpszMarker = _T(' ');
         }
      }
   }

}/* McbReplaceNonPrintableChars */

/**
 ****************************************************************************
 * <P> Recursively build the tree control.  </P>
 *
 * @methodName  McbBuildTree
 *
 * @param       HWND hWndTree
 * @param       McbXMLNode *pNode
 * @param       HTREEITEM hParent
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 20th August       2001   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbBuildTree(HWND hWndTree, McbXMLNode *pNode, HTREEITEM hParent)
{
   int cbElement;
   TV_INSERTSTRUCT insert;
   int nImage;
   HTREEITEM hElement;
   int nIndex;
   int cb;
   int cbText;
   int cbName;
   int cbValue;
   McbXMLNode *pChild;
   McbXMLClear *pClear;
   McbXMLElement *pElement;
   McbXMLAttribute *pAttrib;
   LPTSTR lpszText;
   LPTSTR lpszMarker;

   assert(pNode->type == eNodeElement);
   assert(hWndTree);

   pElement = pNode->node.pElement;
   assert(pElement);

   cbElement = pElement->lpszName ? _tcslen(pElement->lpszName) : 0;

   insert.hParent = hParent;
   insert.hInsertAfter = TVI_LAST;

   insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE |
      TVIF_STATE | TVIF_PARAM;

   nImage = pElement->nIsDeclaration ? McbDECLARATIONBMP : McbELEMENTBMP;

   insert.item.hItem;
   insert.item.state = TVIS_BOLD;
   insert.item.stateMask = TVIS_BOLD;
   insert.item.pszText = pElement->lpszName;
   insert.item.cchTextMax = cbElement;
   insert.item.iImage = nImage;
   insert.item.iSelectedImage = nImage;
   insert.item.cChildren = 0;

   /*
    *************************************************************************
    * The lParam for each node will contain the character offset at where the
   * node starts (calculated when the xml was parsed) and the significant
   * size of the node.  This can then be used to update the rich edit window
   * when someone selects a node in the tree.
    *************************************************************************
    */
   insert.item.lParam = (LPARAM)MAKELONG((WORD)pNode->nStringOffset,
      (WORD)cbElement);

   /*
    *************************************************************************
    * Insert the element into the tree
    *************************************************************************
    */
   if (cbElement)
   {
      hElement = (HTREEITEM)SendMessage(hWndTree, TVM_INSERTITEM, 0,
         (LPARAM)(LPTV_INSERTSTRUCT)&insert);

      assert(hElement);
   }
   else
   {
      hElement = NULL;
   }

   insert.hParent = hElement;

   /*
    *************************************************************************
    * Enumerate nodes in the list
    *************************************************************************
    */
   nIndex = 0;
   while(pChild = McbEnumNodes(pElement, &nIndex))
   {
      switch(pChild->type)
      {
       /*
        *********************************************************************
        * If we have an attribute node
        *********************************************************************
        */
      case eNodeAttribute:

         insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE |
            TVIF_PARAM;
         insert.item.iImage = McbATTRIBUTEBMP;
         insert.item.iSelectedImage = McbATTRIBUTEBMP;

         pAttrib = pChild->node.pAttrib;

           /*
            *****************************************************************
            * Construct attribute name : value
            *****************************************************************
            */
            cbName = pAttrib->lpszName ? _tcslen(pAttrib->lpszName) : 0;
            cbValue = pAttrib->lpszValue ? _tcslen(pAttrib->lpszValue) : 0;

         insert.item.lParam = (LPARAM)MAKELONG((WORD)
            pChild->nStringOffset, (WORD)cbName);

            cbText = cbName + cbValue;

            assert(cbText);

            if (cbValue) cbText += 3;

            lpszText = malloc((cbText+1) * zsizeof(TCHAR));
            lpszMarker = lpszText;

            if (cbName)
            {
                _tcscpy(lpszMarker, pAttrib->lpszName);
                lpszMarker += cbName;
            }

            if (cbValue)
            {
                _tcscpy(lpszMarker, _T(" : "));
                lpszMarker += 3;

                _tcscpy(lpszMarker, pAttrib->lpszValue);
            }

            insert.item.cchTextMax = cbText;
            insert.item.pszText = lpszText;

            McbReplaceNonPrintableChars(insert.item.pszText);

           /*
            *****************************************************************
            * Insert each attribute into the tree.
            *****************************************************************
            */
            SendMessage(hWndTree, TVM_INSERTITEM, 0, (LPARAM)
            (LPTV_INSERTSTRUCT)&insert);

         free(lpszText);
         break;

       /*
        *********************************************************************
        * If we have an element node.
        *********************************************************************
        */
      case eNodeElement:
         McbBuildTree(hWndTree, pChild, hElement);
         break;

       /*
        *********************************************************************
        * If we have a text node.
        *********************************************************************
        */
      case eNodeText:

         insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE |
            TVIF_PARAM;
         insert.item.iImage = McbTEXTBMP;
         insert.item.iSelectedImage = McbTEXTBMP;

         cbText = _tcslen(pChild->node.pText->lpszValue);
         lpszText = malloc((cbText+1) * zsizeof(TCHAR));

         insert.item.lParam = (LPARAM)MAKELONG((WORD)
            pChild->nStringOffset, (WORD)cbText);

         _tcscpy(lpszText, pChild->node.pText->lpszValue);

         insert.item.pszText = lpszText;
         insert.item.cchTextMax = cbText;

         McbReplaceNonPrintableChars(insert.item.pszText);

           /*
            *****************************************************************
            * Insert text node into the tree
            *****************************************************************
            */
            SendMessage(hWndTree, TVM_INSERTITEM, 0,
            (LPARAM)(LPTV_INSERTSTRUCT)&insert);

         free(lpszText);

         break;

       /*
        *********************************************************************
        * If we have a Clear node.
        *********************************************************************
        */
      case eNodeClear:

         insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE |
            TVIF_PARAM;
         insert.item.iImage = McbCLEARBMP;
         insert.item.iSelectedImage = McbCLEARBMP;

         pClear = pChild->node.pClear;

         cbName = pClear->lpszOpenTag ? _tcslen(pClear->lpszOpenTag) : 0;
         cbValue = pClear->lpszValue ? _tcslen(pClear->lpszValue) : 0;
         cb = pClear->lpszCloseTag ? _tcslen(pClear->lpszCloseTag) : 0;

         insert.item.lParam = (LPARAM)MAKELONG((WORD)
            pChild->nStringOffset, (WORD)cbName);

         cbText = cbName + cbValue + cb;

         assert(cbText);

         lpszText = malloc((cbText+1) * zsizeof(TCHAR));
         lpszMarker = lpszText;

         if (cbName)
         {
            _tcscpy(lpszMarker, pClear->lpszOpenTag);
            lpszMarker += cbName;
         }

         if (cbValue)
         {
            _tcscpy(lpszMarker, pClear->lpszValue);
            lpszMarker += cbValue;
         }

         if (cb)
         {
            _tcscpy(lpszMarker, pClear->lpszCloseTag);
         }

         insert.item.pszText = lpszText;
         insert.item.cchTextMax = cbText;

         McbReplaceNonPrintableChars(insert.item.pszText);

           /*
            *****************************************************************
            * Insert node into the tree
            *****************************************************************
            */
            SendMessage(hWndTree, TVM_INSERTITEM, 0,
            (LPARAM)(LPTV_INSERTSTRUCT)&insert);

         free(lpszText);
      }
   }

}/* McbBuildTree */

/**
 ****************************************************************************
 * <P> Helper function to obtain windows text in allocated memory.  </P>
 *
 * @methodName  McbGetWindowText
 *
 * @param       hWnd
 *
 * @return      LPTSTR
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
LPTSTR McbGetWindowText(HWND hWnd)
{
   LPTSTR lpszResult = NULL;
   LONG cbText;

   /*
    *************************************************************************
    * Obtain text from the window
    *************************************************************************
    */
    cbText = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);

   if (cbText)
   {
      lpszResult = malloc((cbText+1) * zsizeof(TCHAR));

      cbText = SendMessage(hWnd, WM_GETTEXT, (WPARAM)cbText+1,
         (LPARAM)lpszResult);

      if (!cbText)
      {
         free(lpszResult);
         lpszResult = NULL;
      }
   }

   return lpszResult;

}/* McbGetWindowText */

/**
 ****************************************************************************
 * <P> Count characters in an edit box to find number of character offset for
 * a specified line.  </P>
 *
 * @methodName  McbCountCharsUpdateLine
 *
 * @param       hWndRich
 * @param       nLines
 *
 * @return      int
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
int McbCountCharsUpdateLine(HWND hWndRich, int nLines)
{
   int nResult = 0;
   int nLine;
   LPTSTR lpszText;
   int cbText;
   int nChar;

   lpszText = McbGetWindowText(hWndRich);

   if (lpszText)
   {
      cbText = _tcslen(lpszText);

      nLine = 0;

      for (nChar = 0; nLine < nLines && nChar < cbText; nChar++)
      {
         nResult++;

         if (lpszText[nChar] == _T('\n'))
         {
            nLine++;
         }
      }

      free(lpszText);
   }

   return nResult;

}/* McbCountCharsUpdateLine */

/**
 ****************************************************************************
 * <P> Create formatted XML with tabs and carriage returns.  </P>
 *
 * @methodName  McbFormatXML
 *
 * @param       lpszXML
 *
 * @return      LPTSTR
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 5th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
LPTSTR McbFormatXML(LPCTSTR lpszXML)
{
   LPTSTR lpszResult = NULL;

   McbXMLElement * pRoot;
   McbXMLResults results;

   /*
    *************************************************************************
    * Parse the XML into the tree structure
    *************************************************************************
    */
   pRoot = McbParseXML(lpszXML, &results);

   if (pRoot)
   {
      lpszResult = McbCreateXMLString(pRoot, 1, 0);

       /*
        *********************************************************************
        * Cleanup
        *********************************************************************
        */
      McbDeleteRoot(pRoot);
   }

   return lpszResult;
}/* McbFormatXML */

/**
 ****************************************************************************
 * <P> Does exactly what it says on the tin.  </P>
 *
 * @methodName  McbParseXMLIntoTree
 *
 * @param       hWndStatus
 * @param       hWndTree
 * @param       lpszXML
 *
 * @return      int
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
int McbParseXMLIntoTree(HWND hWndRich, HWND hWndStatus, HWND hWndTree,
                  LPCTSTR lpszXML)
{
   int nResult = FALSE;
   int nCharsOffset;
   CHARRANGE range;

   McbXMLNode node;
   McbXMLElement * pRoot;
   McbXMLResults results;

   /*
    *************************************************************************
    * Clear the tree control.
    *************************************************************************
    */
   TreeView_DeleteAllItems(hWndTree);

   /*
    *************************************************************************
    * Parse the XML into the tree structure
    *************************************************************************
    */
   pRoot = McbParseXML(lpszXML, &results);

   if (pRoot)
   {
       /*
        *********************************************************************
        * Build the tree control
        *********************************************************************
        */
      node.node.pElement = pRoot;
      node.type = eNodeElement;
      node.nStringOffset = 0;
      McbBuildTree(hWndTree, &node, NULL);

      if (pRoot->nSize == 0)
      {
         McbSETSTATUS(hWndStatus, _T("Document contains no elements"), 0);
      }
      else
      {
         McbSETSTATUS(hWndStatus, _T("No errors"), 0);
      }

       /*
        *********************************************************************
        * Cleanup
        *********************************************************************
        */
      McbDeleteRoot(pRoot);

      nResult = TRUE;
   }
   /*
    *************************************************************************
    * If we failed to parse the XML
    *************************************************************************
    */
    else
    {
       /*
        *********************************************************************
        * Update the status
        *********************************************************************
        */
      TCHAR szBuffer[200];

      _stprintf(szBuffer, _T("Error at line: %d, Column: %d - %s"),
         results.nLine, results.nColumn-1, McbGetError(results.error));

      McbSETSTATUS(hWndStatus, szBuffer, 0);

       /*
        *********************************************************************
        * Calculate the offset in the edit box, set the range to the error
      * line and scroll to the position
        *********************************************************************
        */
      nCharsOffset = McbCountCharsUpdateLine(hWndRich, results.nLine - 1);

      nCharsOffset += results.nColumn-1;

      range.cpMin = nCharsOffset > 0 ? nCharsOffset - 1 : nCharsOffset;
      range.cpMax = range.cpMin + 1;

      SendMessage(hWndRich, EM_EXSETSEL, 0, (LPARAM)&range);
      SendMessage(hWndRich, EM_SCROLLCARET, 0, 0);

      SetFocus(hWndRich);
    }

   return nResult;

}/* McbParseXMLIntoTree */

/**
 ****************************************************************************
 * <P> Obtain a filename from the punter and attempt to load it as XML.  </P>
 *
 * @methodName  McbGetFileNameAndLoadXML
 *
 * @param       hWnd
 * @param       hWndRich
 * @param       hWndStatus
 * @param       hWndTree
 * @param       *pOptions
 *
 * @return      int
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
int McbGetFileNameAndLoadXML(HWND hWnd, HWND hWndRich, HWND hWndStatus,
                     HWND hWndTree, McbOptions *pOptions)
{
   int nResult = FALSE;

   LPTSTR lpszXML;

   TCHAR szFile[_MAX_PATH+1];
   TCHAR szFileTitle[_MAX_PATH+1];
   TCHAR szStartDir[_MAX_PATH+1];
   TCHAR szDrive[_MAX_DRIVE+1];
   TCHAR szDir[_MAX_DIR+1];
   TCHAR szExt[_MAX_EXT+1];
   TCHAR szFname[_MAX_FNAME+1];

   OPENFILENAME openName;

   openName.lStructSize = zsizeof(OPENFILENAME);
   openName.hwndOwner = hWnd;
   openName.hInstance = g_hInst;

   openName.lpstrFilter = _T("XML Files (*.xml)\0*.xml\0Text Files (*.txt)")
      _T("\0*.txt\0All Files (*.*)\0*.*\0\0");

   openName.lpstrCustomFilter = NULL;
   openName.nMaxCustFilter = 0;
   openName.nFilterIndex = pOptions->nFilterIndex;
   openName.lpstrFile = szFile;
   openName.nMaxFile = _MAX_PATH;
   openName.lpstrFileTitle = szFileTitle;
   openName.nMaxFileTitle = _MAX_PATH;
   openName.lpstrInitialDir = NULL;
   openName.lpstrTitle = _T("Load XML file");
   openName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
      OFN_HIDEREADONLY;
   openName.nFileOffset = 0;
   openName.nFileExtension = 0;
   openName.lpstrDefExt = NULL;
   openName.lCustData = 0;
   openName.lpfnHook = NULL;
   openName.lpTemplateName = NULL;

    if (*pOptions->szLastFile)
    {
        _tsplitpath(pOptions->szLastFile, szDrive, szDir, szFname, szExt);

        _tcscpy(szStartDir, szDrive);
        _tcscpy(szStartDir + _tcslen(szDrive), szDir);

        openName.lpstrInitialDir = szStartDir;

        _tcscpy(szFile, szFname);
        _tcscpy(szFile + _tcslen(szFname), szExt);
    }
    else
    {
        szFile[0] = 0;
    }

   /*
    *************************************************************************
    * Obtain the file name and attempt to load it
    *************************************************************************
    */
   if (GetOpenFileName(&openName))
   {
      lpszXML = McbLoadFile(szFile);

      if (lpszXML)
      {
           /*
            *****************************************************************
            * Update the rich text window and attempt to parse the XML and
         * build the tree.
            *****************************************************************
            */
         SetWindowText(hWndRich, lpszXML);
         McbParseXMLIntoTree(hWndRich, hWndStatus, hWndTree, lpszXML);

         pOptions->nFilterIndex = openName.nFilterIndex;
         _tcscpy(pOptions->szLastFile, szFile);

         nResult = TRUE;

         free(lpszXML);
      }
       /*
        *********************************************************************
        * If we failed to load the file.
        *********************************************************************
        */
        else
      {
         TreeView_DeleteAllItems(hWndTree);

         McbSETSTATUS(hWnd, _T("Failed to read the file"), 0);
      }
   }

   return nResult;

}/* McbGetFileNameAndLoadXML */

/**
 ****************************************************************************
 * <P> Obtain a filename from the punter and attempt to save XML into it.
 * </P>
 *
 * @methodName  McbGetFileNameAndSaveXML
 *
 * @param       lpszXML
 * @param       hWnd
 * @param       hWndRich
 * @param       hWndStatus
 * @param       *pOptions
 *
 * @return      int
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 4th February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
int McbGetFileNameAndSaveXML(LPTSTR lpszXML, HWND hWnd, HWND hWndRich,
                     HWND hWndStatus, McbOptions *pOptions)
{
   int nResult = FALSE;

   TCHAR szFile[_MAX_PATH+1];
   TCHAR szFileTitle[_MAX_PATH+1];
   TCHAR szStartDir[_MAX_PATH+1];
   TCHAR szDrive[_MAX_DRIVE+1];
   TCHAR szDir[_MAX_DIR+1];
   TCHAR szExt[_MAX_EXT+1];
   TCHAR szFname[_MAX_FNAME+1];

   OPENFILENAME openName;

   if (lpszXML)
   {
        openName.lStructSize = zsizeof(OPENFILENAME);
        openName.hwndOwner = hWnd;
        openName.hInstance = g_hInst;

        openName.lpstrFilter = _T("XML Files (*.xml)\0*.xml\0Text Files (*.txt)")
            _T("\0*.txt\0All Files (*.*)\0*.*\0\0");

        openName.lpstrCustomFilter = NULL;
        openName.nMaxCustFilter = 0;
        openName.nFilterIndex = pOptions->nFilterIndex;
        openName.lpstrFile = szFile;
        openName.nMaxFile = _MAX_PATH;
        openName.lpstrFileTitle = szFileTitle;
        openName.nMaxFileTitle = _MAX_PATH;
        openName.lpstrInitialDir = NULL;
        openName.lpstrTitle = _T("Save XML file");
        openName.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
            OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
        openName.nFileOffset = 0;
        openName.nFileExtension = 0;
        openName.lpstrDefExt = NULL;
        openName.lCustData = 0;
        openName.lpfnHook = NULL;
        openName.lpTemplateName = NULL;

        if (*pOptions->szLastFile)
        {
            _tsplitpath(pOptions->szLastFile, szDrive, szDir, szFname, szExt);

            _tcscpy(szStartDir, szDrive);
            _tcscpy(szStartDir + _tcslen(szDrive), szDir);

            openName.lpstrInitialDir = szStartDir;

            _tcscpy(szFile, szFname);
            _tcscpy(szFile + _tcslen(szFname), szExt);
        }
        else
        {
            szFile[0] = 0;
        }

       /*
        *********************************************************************
        * Obtain the file name and attempt to save it
        *********************************************************************
        */
        if (GetSaveFileName(&openName))
        {
            if (McbSaveFile(szFile, lpszXML))
            {
               /*
                *************************************************************
                * Cache the options.
                *************************************************************
                */
            pOptions->nFilterIndex = openName.nFilterIndex;
            _tcscpy(pOptions->szLastFile, szFile);

                McbSETSTATUS(hWnd, _T("Saved"), 0);
            }
            else
            {
                McbSETSTATUS(hWnd, _T("Failed to write to file"), 0);
            }
        }
   }

   return nResult;

}/* McbGetFileNameAndSaveXML */

/**
 ****************************************************************************
 * <P> Processes messages for the main window. </P>
 *
 * @methodName  CALLBACK McbWndProc
 *
 * @param       hWnd
 * @param       message
 * @param       wParam
 * @param       lParam
 *
 * @return      LRESULT
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 3rd February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
LRESULT CALLBACK McbWndProc(HWND hWnd, UINT msg, WPARAM wParam,
                     LPARAM lParam)
{
   static HWND       hWndRich = NULL;     /* rich edit */
   static HWND       hWndStatus = NULL;      /* status bar */
   static HWND       hWndTree = NULL;     /* tree control */
   static HFONT      hFontArial = NULL;      /* main font */
   static HINSTANCE  hInstRichEdit = NULL;   /* rich edit dll */
   static HIMAGELIST hImages = NULL;         /* Image list for the tree */
   static BOOL       bTreeUpdated = FALSE;

   int            wmId;
   int            wmEvent;
   PAINTSTRUCT    ps;
   HDC            hDC;
   RECT        rcWnd;
   RECT        rcTemp;
   int            nInit;
   DWORD       dwStyle;
   DWORD       dwExStyle;
   int            nWndHeight;
   int            nWndWidth;
   int            nX;
   TEXTMETRIC     txtMet;
   BOOL        bRedraw;
   LOGFONT        logFont;
   LPTSTR         lpszText;
   LPTSTR         lpszText2;
   LPTSTR         lpszFile;
   NMHDR       *pNmh;
   NM_TREEVIEW    *pNmtv;
   TV_ITEM        tvItem;
   CHARRANGE      range;
   HDROP       hDrop;
   UINT        cch;

   switch (msg)
   {
       /*
        *********************************************************************
        * The window procedure receives this message after the window is
      * created, but before the window becomes visible. The message is sent
      * before the CreateWindowEx or CreateWindow function returns.
      * Within here we are going to create all the required resources.
        *********************************************************************
        */
      case WM_CREATE:
         {
            nInit = FALSE;

               /*
                *************************************************************
                * Load options from file.
                *************************************************************
                */
            McbGetOptionsFileName(g_szOptFile);
            McbArchiveOptions(g_szOptFile, &g_options, FALSE);

               /*
                *************************************************************
                * Load the DLL which services common controls for the tree
            * control
                *************************************************************
                */
                InitCommonControls();

               /*
                *************************************************************
                * Create the tree view control
                *************************************************************
                */
            dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASBUTTONS
               | TVS_HASLINES | TVS_LINESATROOT;

            dwExStyle = 0;

                hWndTree = CreateWindowEx(dwExStyle, WC_TREEVIEW, NULL,
               dwStyle, 0, 0, 0, 0, hWnd, (HMENU)IDC_TREE, g_hInst,
               NULL);

            if (hWndTree)
            {
                   /*
                    *********************************************************
                    * Create the status bar
                    *********************************************************
                    */
               dwStyle = WS_CHILD | WS_VISIBLE;

                    hWndStatus = CreateStatusWindow(dwStyle, NULL, hWnd, 0);

               if (hWndStatus)
               {
                       /*
                        *****************************************************
                        * Load the dll for the rich edit
                        *****************************************************
                        */
#define McbRICHEDIT  _T("RICHED32.DLL")
                  hInstRichEdit = LoadLibrary(McbRICHEDIT);

                       /*
                        *****************************************************
                        * Create rich edit box
                        *****************************************************
                        */
                        dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER |
                     WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
                     ES_MULTILINE;

                        hWndRich = CreateWindowEx(dwExStyle, _T("RICHEDIT"),
                     NULL, dwStyle, 0, 0, 0, 0, hWnd,
                     (HMENU)IDC_RICHEDIT, g_hInst, NULL);

                  if (hWndRich)
                  {
                           /*
                            *************************************************
                            * Enable drag and drop notifications.
                            *************************************************
                            */
                     DragAcceptFiles(hWnd, TRUE);

                           /*
                            *************************************************
                            * Create font
                            *************************************************
                            */
                            hDC = GetDC(hWnd);
                            GetTextMetrics(hDC, &txtMet);
                            ReleaseDC(hWnd, hDC);

                     logFont.lfHeight =         txtMet.tmHeight;
                     logFont.lfWidth =
                        txtMet.tmAveCharWidth;
                     logFont.lfEscapement =     0;
                     logFont.lfOrientation =    0;
                     logFont.lfWeight =         FW_NORMAL;
                     logFont.lfItalic =         txtMet.tmItalic;
                     logFont.lfUnderline =      txtMet.tmUnderlined;
                     logFont.lfStrikeOut =      txtMet.tmStruckOut;
                     logFont.lfCharSet =        txtMet.tmCharSet;
                     logFont.lfOutPrecision =   OUT_DEFAULT_PRECIS;
                     logFont.lfClipPrecision =  CLIP_DEFAULT_PRECIS;
                     logFont.lfQuality =        DEFAULT_QUALITY;
                     logFont.lfPitchAndFamily =
                        txtMet.tmPitchAndFamily;

                     _tcscpy(logFont.lfFaceName, _T("Arial"));

                            hFontArial = CreateFontIndirect(&logFont);

                     if (hFontArial)
                     {
                               /*
                                *********************************************
                                * Associate the font with the edit window.
                                *********************************************
                                */
                                bRedraw = FALSE;

                                SendMessage(hWndRich, WM_SETFONT,
                           (WPARAM)hFontArial,
                           MAKELPARAM(bRedraw, 0));

                               /*
                                *********************************************
                                * Attempt to load the last file
                                *********************************************
                                */
                        lpszText = McbLoadFile(g_options.szLastFile);

                        if (lpszText)
                        {
                           SetWindowText(hWndRich, lpszText);

                           if (McbParseXMLIntoTree(hWndRich,
                              hWndStatus, hWndTree, lpszText))
                           {
                              bTreeUpdated = TRUE;
                           }

                           free(lpszText);
                        }

                               /*
                                *********************************************
                                * Create the image list from the bitmap
                                *********************************************
                                */
                        hImages = ImageList_LoadImage(g_hInst,
                           MAKEINTRESOURCE(IDB_BMPIMAGELIST),
                           12, 5, RGB(0, 0, 0), IMAGE_BITMAP,
                           LR_DEFAULTCOLOR);

                        if (hImages)
                        {
                                   /*
                                    *****************************************
                                    * Associate the image list with the
                                    * tree-view control.
                                    *****************************************
                                    */
                                    TreeView_SetImageList(hWndTree, hImages,
                              TVSIL_NORMAL);

                                    nInit = TRUE;
                        }
                     }
                  }
               }
            }

            if (nInit == FALSE)
            {
                   /*
                    *********************************************************
                    * Terminate by sending a destroy message back to self.
                    *********************************************************
                    */
                    PostMessage(hWnd, WM_DESTROY, 0, 0);
            }
         }
         break;

       /*
        *********************************************************************
        * Window resize message.  Scale the controls and graphics here.
        *********************************************************************
        */
      case WM_SIZE:
         {
               /*
                *************************************************************
                * Obtain the status window sizes
                *************************************************************
                */
            GetClientRect(hWndStatus, &rcTemp);

               /*
                *************************************************************
                * Obtain the size of the main window, allowing for the size
            * of the status window
                *************************************************************
                */
            GetClientRect(hWnd, &rcWnd);

            rcWnd.bottom -= rcTemp.bottom;

            nWndHeight = rcWnd.bottom - rcWnd.top;
            nWndWidth = rcWnd.right - rcWnd.left;

               /*
                *************************************************************
                * Resize the windows
                *************************************************************
                */
            MoveWindow(hWndStatus, 0, nWndHeight, nWndWidth, rcTemp.top,
               TRUE);

            if (g_options.bPanelsVertical)
            {
               nX = (int)nWndWidth / 2;

               MoveWindow(hWndRich, 0, 0, nX, nWndHeight, TRUE);
               MoveWindow(hWndTree, nX, 0, nWndWidth - nX, nWndHeight,
                  TRUE);
            }
            else
            {
               nX = (int)nWndHeight / 2;

               MoveWindow(hWndRich, 0, 0, nWndWidth, nX, TRUE);
               MoveWindow(hWndTree, 0, nX, nWndWidth, nWndHeight - nX,
                  TRUE);
            }

            return DefWindowProc(hWnd, msg, wParam, lParam);
         }

       /*
        *********************************************************************
        * Drag and drop messages
        *********************************************************************
        */
      case WM_DROPFILES:
         {
            hDrop = (HDROP)wParam;

               /*
                *************************************************************
                * Only accept one filename at most
                *************************************************************
                */
            if (DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0) > 1)
            {
               McbSETSTATUS(hWndStatus,
                  _T("Multiple files not supported"), 0);
            }
            else
            {
                   /*
                    *********************************************************
                    * Obtain the filename, the first call obtains the size.
                    *********************************************************
                    */
               cch = DragQueryFile(hDrop, 0, NULL, 0);

               if (cch)
               {
                  lpszFile = malloc((cch+1) + zsizeof(TCHAR));

                  cch = DragQueryFile(hDrop, 0, lpszFile, cch+1);

                  if (cch)
                  {
                           /*
                            *************************************************
                            * Attempt to load the file then parse it into the
                     * tree.
                            *************************************************
                            */
                     lpszText = McbLoadFile(lpszFile);

                     if (lpszText)
                     {
                        SetWindowText(hWndRich, lpszText);

                        if (McbParseXMLIntoTree(hWndRich, hWndStatus,
                           hWndTree, lpszText))
                        {
                                   /*
                                    *****************************************
                                    * Update the options
                                    *****************************************
                                    */
                           _tcscpy(g_options.szLastFile, lpszFile);

                           bTreeUpdated = TRUE;
                        }

                        free(lpszText);
                     }
                  }
                  else
                  {
                     McbSETSTATUS(hWndStatus,
                        _T("Failed to query filename"), 0);
                  }

                  free(lpszFile);
               }
            }

            DragFinish(hDrop);


         }
         return DefWindowProc(hWnd, msg, wParam, lParam);


       /*
        *********************************************************************
        * Notification messages
        *********************************************************************
        */
      case WM_NOTIFY:
         {
               /*
                *************************************************************
                * Obtain the notification header
                *************************************************************
                */
            pNmh = (NMHDR*)lParam;

            switch(pNmh->idFrom)
            {
               /*
                *************************************************************
                * Tree view
                *************************************************************
                */
            case IDC_TREE:
                   /*
                    *********************************************************
                    * If the selection has changed in the tree view.
                    *********************************************************
                    */
               if (pNmh->code == TVN_SELCHANGED)
               {
                  if (bTreeUpdated)
                  {
                           /*
                            *************************************************
                            * Obtain the item from the tree view.  When the
                     * tree view was built (in McbBuildTree()) the
                     * lParam parameter value was built up to contain
                     * the character offset at which the node starts
                     * at and the length of the node start.
                     * We'll use this to format the rich edit box so
                     * that when someone selects an entry in the tree
                     * control, the current selection changes in the
                     * rich edit box.
                            *************************************************
                            */
                     pNmtv = (NM_TREEVIEW*)lParam;

                     tvItem.mask = TVIF_PARAM | TVIF_HANDLE;
                     tvItem.hItem = pNmtv->itemNew.hItem;

                     if (TreeView_GetItem(hWndTree, &tvItem))
                     {
                               /*
                                *********************************************
                                * Set the selection in the rich edit box,
                        * scroll the window so it is visible and
                        * ensure the selection is visible.
                                *********************************************
                                */
                        range.cpMin = LOWORD(tvItem.lParam);
                        range.cpMax = range.cpMin +
                           HIWORD(tvItem.lParam);

                        SendMessage(hWndRich, EM_EXSETSEL, 0,
                           (LPARAM)&range);

                        SendMessage(hWndRich, EM_SCROLLCARET, 0, 0);

                        SendMessage(hWndRich, EM_HIDESELECTION,
                           (WPARAM)(BOOL)FALSE,
                           (LPARAM)(BOOL)FALSE);
                     }
                  }
               }
               break;
            }
         }
         break;

       /*
        *********************************************************************
        * Menu/control messages.
        *********************************************************************
        */
      case WM_COMMAND:
         wmId    = LOWORD(wParam);
         wmEvent = HIWORD(wParam);

         switch (wmId)
         {
         case IDM_ABOUT:
            DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd,
               (DLGPROC)McbAbout);
            break;

           /*
            *****************************************************************
            * Format XML command
            *****************************************************************
            */
         case IDM_FORMATXML:
            lpszText = McbGetWindowText(hWndRich);

            if (lpszText)
            {
                   /*
                    *********************************************************
                    * Create formatted version of the XML
                    *********************************************************
                    */
               lpszText2 = McbFormatXML(lpszText);

               if (lpszText2)
               {
                       /*
                        *****************************************************
                        * Update the rich edit control
                        *****************************************************
                        */
                  SetWindowText(hWndRich, lpszText2);

                  free(lpszText2);
               }

               free(lpszText);
            }
               /*
                *************************************************************
                * Drop into the Parse XML command
                *************************************************************
                */

           /*
            *****************************************************************
            * Parse XML command
            *****************************************************************
            */
         case IDM_PARSE:
            lpszText = McbGetWindowText(hWndRich);

            if (lpszText)
            {
               if (McbParseXMLIntoTree(hWndRich, hWndStatus, hWndTree,
                  lpszText))
               {
                  bTreeUpdated = TRUE;
               }

               free(lpszText);
            }
            break;

           /*
            *****************************************************************
            * Rich edit control commands
            *****************************************************************
            */
         case IDC_RICHEDIT:
            switch(wmEvent)
            {
            case EN_CHANGE:
               bTreeUpdated = FALSE;
            }
            break;

           /*
            *****************************************************************
            * Change view command
            *****************************************************************
            */
         case IDM_CHANGEVIEW:
            g_options.bPanelsVertical = !g_options.bPanelsVertical;
            PostMessage(hWnd, WM_SIZE, 0, 0);
            break;

         case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

           /*
            *****************************************************************
            * File open command
            *****************************************************************
            */
         case IDM_FILEOPEN:
            if (McbGetFileNameAndLoadXML(hWnd, hWndRich, hWndStatus,
               hWndTree, &g_options))
            {
               bTreeUpdated = TRUE;
            }
            break;

           /*
            *****************************************************************
            * File save command.
            *****************************************************************
            */
         case IDM_FILESAVE:
            lpszText = McbGetWindowText(hWndRich);

            if (lpszText)
            {
               McbGetFileNameAndSaveXML(lpszText, hWnd, hWndRich,
                  hWndStatus, &g_options);
            }
            else
            {
               McbSETSTATUS(hWndStatus, _T("No data to save!"), 0);
            }
            break;

         default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
         }
         break;

       /*
        *********************************************************************
        * Painting messages.
        *********************************************************************
        */
      case WM_PAINT:
         hDC = BeginPaint(hWnd, &ps);
         EndPaint(hWnd, &ps);
         break;

       /*
        *********************************************************************
        * Terminate message or cleanup.
        *********************************************************************
        */
      case WM_DESTROY:

           /*
            *****************************************************************
            * Save options to file.
            *****************************************************************
            */
         McbArchiveOptions(g_szOptFile, &g_options, TRUE);

         if (hWndRich) DestroyWindow(hWndRich);
         if (hWndStatus) DestroyWindow(hWndStatus);
         if (hImages) ImageList_Destroy(hImages);
         if (hWndTree) DestroyWindow(hWndTree);
         if (hFontArial) DeleteObject(hFontArial);
         if (hInstRichEdit) FreeLibrary(hInstRichEdit);

         PostQuitMessage(0);
         break;

      default:
         return DefWindowProc(hWnd, msg, wParam, lParam);
   }

   return 0;

}/* CALLBACK McbWndProc */

/**
 ****************************************************************************
 * <P> Registers the window class.
 *
 * This function and its usage is only necessary if you want this code to be
 * compatible with Win32 systems prior to the 'RegisterClassEx' function that
 * was added to Windows 95. It is important to call this function so that the
 * application will get 'well formed' small icons associated with it.</P>
 *
 * @methodName  McbRegisterClass
 *
 * @param       hInstance
 *
 * @return      ATOM
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 3rd February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
ATOM McbRegisterClass(HINSTANCE hInstance)
{
   WNDCLASSEX wcex;

   wcex.cbSize = zsizeof(WNDCLASSEX);

   wcex.style        = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc  = (WNDPROC)McbWndProc;
   wcex.cbClsExtra      = 0;
   wcex.cbWndExtra      = 0;
   wcex.hInstance    = hInstance;
   wcex.hIcon        = LoadIcon(hInstance, (LPCTSTR)IDI_MCBXML);
   wcex.hCursor      = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground   = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName = (LPCSTR)IDC_MCBXML;
   wcex.lpszClassName   = g_szWindowClass;
   wcex.hIconSm      = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

   return RegisterClassEx(&wcex);

}/* McbRegisterClass */

/**
 ****************************************************************************
 * <P> Application entry point.  </P>
 *
 * @methodName  APIENTRY WinMain
 *
 * @param       hInstance
 * @param       hPrevInstance
 * @param       lpCmdLine
 * @param       nCmdShow
 *
 * @return      int
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory
 * 3rd February   2002   -    (V1.0) Creation (MCB)
 ****************************************************************************
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                LPSTR lpCmdLine, int nCmdShow)
{
   MSG msg;
   HACCEL hAccelTable;

   /*
    *************************************************************************
    * Initialize global strings
    *************************************************************************
    */
   LoadString(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
   LoadString(hInstance, IDC_MCBXML, g_szWindowClass, MAX_LOADSTRING);
   McbRegisterClass(hInstance);

   /*
    *************************************************************************
    * Perform application initialization:
    *************************************************************************
    */
   if (!McbInitInstance (hInstance, nCmdShow))
   {
      return FALSE;
   }

   hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MCBXML);

   /*
    *************************************************************************
    * Main message loop
    *************************************************************************
    */
   while (GetMessage(&msg, NULL, 0, 0))
   {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   return msg.wParam;

}/* WinMain */

