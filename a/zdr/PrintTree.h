// TreeCtrlPrint.h: interface for the TreeCtrlPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREECTRLPRINT_H__62B67F58_CE61_4E35_A061_437242E5B768__INCLUDED_)
#define AFX_TREECTRLPRINT_H__62B67F58_CE61_4E35_A061_437242E5B768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// this enum is used when printing the content of a tree control
// ir returns the style of lines / buttons at a given level
enum tc_DrawTypes
{
   tc_None = 0,            // no lines/buttons
   tc_Plus,             // [+]-

   tc_PlusDrop,            // [+]-
                        //  |

   tc_Minus,               // [-]-

   tc_MinusDrop,           // [-]-
                        //  |

   tc_Link,             // |
                        // +-

   tc_LinkDrop,            // |
                        // +-
                        // |

   tc_Drop                 // |
};

class TreeCtrlPrint
{
public:
               TreeCtrlPrint();
   virtual        ~TreeCtrlPrint();

   HTREEITEM      GetNextTreeItem(const CTreeCtrl& treeCtrl, HTREEITEM hItem);
   void        DrawTreeItem(const CTreeCtrl& treeCtrl, HTREEITEM hPrintItem, CDC *pDC, CRect rect, CWnd *pWnd);
   void        ExpandAll(CTreeCtrl& treeCtrl);
private:
   tc_DrawTypes   GetIndentLevelStyle(const CTreeCtrl& treeCtrl, HTREEITEM hPrintItem, HTREEITEM hIndentItem);
   void        DrawTreeStyle(const CTreeCtrl& treeCtrl, CDC *pDC, CRect box, tc_DrawTypes type);
   HANDLE         ImageToDIB(CImageList* pImageList, int iImageNumber, CWnd* pWnd);
   HANDLE         DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal);

};

#endif // !defined(AFX_TREECTRLPRINT_H__62B67F58_CE61_4E35_A061_437242E5B768__INCLUDED_)
