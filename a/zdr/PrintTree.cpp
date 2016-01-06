// TreeCtrlPrint.cpp: implementation of the TreeCtrlPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeCtrlPrint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TreeCtrlPrint::TreeCtrlPrint()
{

}

TreeCtrlPrint::~TreeCtrlPrint()
{

}

HTREEITEM TreeCtrlPrint::GetNextTreeItem(const CTreeCtrl& treeCtrl, HTREEITEM hItem)
{
   // we return the next HTEEITEM for a tree such as:
   // Root (1)
   //    Child1 (2)
   //       xxxx (3)
   //       yyyy (4)
   //    Chiled2 (5)
   // Item (6)

   // has this item got any children
   if (treeCtrl.ItemHasChildren(hItem))
   {
      return treeCtrl.GetNextItem(hItem, TVGN_CHILD);
   }
   else if (treeCtrl.GetNextItem(hItem, TVGN_NEXT) != NULL)
   {
      // the next item at this level
      return treeCtrl.GetNextItem(hItem, TVGN_NEXT);
   }
   else
   {
      // return the next item after our parent
      hItem = treeCtrl.GetParentItem(hItem);
      if (hItem == NULL)
      {
         // no parent
         return NULL;
      }
      while (treeCtrl.GetNextItem(hItem, TVGN_NEXT) == NULL)
      {
         hItem = treeCtrl.GetParentItem(hItem);
      }
      // next item that follows our parent
      return treeCtrl.GetNextItem(hItem, TVGN_NEXT);
   }
}

tc_DrawTypes TreeCtrlPrint::GetIndentLevelStyle(const CTreeCtrl& treeCtrl, HTREEITEM hPrintItem, HTREEITEM hIndentItem)
{
   tc_DrawTypes   drawType = tc_None;
   bool bHasButtons = ((::GetWindowLong(treeCtrl.m_hWnd, GWL_STYLE) & TVS_HASBUTTONS) != 0);
   bool bHasLines = ((::GetWindowLong(treeCtrl.m_hWnd, GWL_STYLE) & TVS_HASLINES) != 0);
   bool bHasChildren = (treeCtrl.ItemHasChildren(hPrintItem) != 0);
   bool bIsSiblingItem = (treeCtrl.GetNextItem(hPrintItem, TVGN_NEXT) != NULL);
   bool bIsExpanded = (treeCtrl.GetItemState(hPrintItem, TVIS_EXPANDED) != 0);

   if (bHasButtons)
   {
      if (bHasLines)
      {
         if (hPrintItem == hIndentItem)
         {
            // were drawing this at the actual item level
            if (bHasChildren)
            {
               if (bIsSiblingItem)
               {
                  if (bIsExpanded)
                  {
                     drawType = tc_MinusDrop;
                  }
                  else
                  {
                     drawType = tc_PlusDrop;
                  }
               }
               else
               {
                  if (bIsExpanded)
                  {
                     drawType = tc_Minus;
                  }
                  else
                  {
                     drawType = tc_Plus;
                  }
               }
            }
            else
            {
               if (bIsSiblingItem)
               {
                  drawType = tc_LinkDrop;
               }
               else
               {
                  drawType = tc_Link;
               }
            }
         }
         else
         {
            // this is a level above the current one
            bool bIsSiblingItem = (treeCtrl.GetNextItem(hIndentItem, TVGN_NEXT) != NULL);
            if (bIsSiblingItem)
            {
               drawType = tc_Drop;
            }
         }
      }
      else
      {
         // buttons, but no lines
         if (hPrintItem == hIndentItem)
         {
            // were drawing this at the actual item level
            if (bHasChildren)
            {
               if (bIsExpanded)
               {
                  drawType = tc_Minus;
               }
               else
               {
                  drawType = tc_Plus;
               }
            }
         }
      }
   }
   else
   {
      if (bHasLines)
      {
         // we are not drawing any [+] or [-] buttons
         if (hPrintItem == hIndentItem)
         {
            // were drawing this at the actual item level
            if (bIsSiblingItem)
            {
               drawType = tc_LinkDrop;
            }
            else
            {
               drawType = tc_Link;
            }
         }
         else
         {
            // this is a level above the current one
            bool bIsSiblingItem = (treeCtrl.GetNextItem(hIndentItem, TVGN_NEXT) != NULL);
            if (bIsSiblingItem)
            {
               drawType = tc_Drop;
            }
         }
      }
   }
   return drawType;
}

void TreeCtrlPrint::DrawTreeStyle(const CTreeCtrl& treeCtrl, CDC *pDC, CRect box, tc_DrawTypes type)
{
   CPen  penDark(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
   CPen  penBlack(PS_SOLID, 3, RGB(0, 0, 0));
   CPen* pOldPen;
   int      quarterX = box.Width() / 4;
   int      quarterY = box.Height() / 4;
   bool  bHasLines = ((::GetWindowLong(treeCtrl.m_hWnd, GWL_STYLE) & TVS_HASLINES) != 0);

   switch (type)
   {
   case tc_None:
      // draw nothing for this
      break;
   case tc_Plus:
      // [+]-
      pOldPen = pDC->SelectObject(&penDark);
      // []
      pDC->MoveTo(box.left + quarterX, box.top + quarterY);
      pDC->LineTo(box.right - quarterX, box.top + quarterY);
      pDC->LineTo(box.right - quarterX, box.bottom - quarterY);
      pDC->LineTo(box.left + quarterX, box.bottom - quarterY);
      pDC->LineTo(box.left + quarterX, box.top + quarterY);
      if (bHasLines)
      {
         pDC->MoveTo(box.right - quarterX, box.top + box.Height() / 2);
         pDC->LineTo(box.right, box.top + box.Height() / 2);
      }
      pDC->SelectObject(&penBlack);
      pDC->MoveTo(box.left + box.Width() / 2, box.top + quarterY);
      pDC->LineTo(box.left + box.Width() / 2, box.bottom - quarterY);
      pDC->MoveTo(box.left + quarterX, box.top + box.Height() / 2);
      pDC->LineTo(box.right - quarterX, box.top + box.Height() / 2);
      pDC->SelectObject(pOldPen);
      break;
   case tc_PlusDrop:
      // [+]-
      //  |
      pOldPen = pDC->SelectObject(&penDark);
      pDC->MoveTo(box.left + quarterX, box.top + quarterY);
      pDC->LineTo(box.right - quarterX, box.top + quarterY);
      pDC->LineTo(box.right - quarterX, box.bottom - quarterY);
      pDC->LineTo(box.left + quarterX, box.bottom - quarterY);
      pDC->LineTo(box.left + quarterX, box.top + quarterY);
      if (bHasLines)
      {
         pDC->MoveTo(box.right - quarterX, box.top + box.Height() / 2);
         pDC->LineTo(box.right, box.top + box.Height() / 2);
         pDC->MoveTo(box.left + box.Width() / 2, box.top + quarterY);
         pDC->LineTo(box.left + box.Width() / 2, box.top);
      }
      pDC->SelectObject(&penBlack);
      pDC->MoveTo(box.left + box.Width() / 2, box.top + quarterY);
      pDC->LineTo(box.left + box.Width() / 2, box.bottom - quarterY);
      pDC->MoveTo(box.left + quarterX, box.top + box.Height() / 2);
      pDC->LineTo(box.right - quarterX, box.top + box.Height() / 2);
      pDC->SelectObject(pOldPen);
      break;
   case tc_Minus:
      // [-]-
      pOldPen = pDC->SelectObject(&penDark);
      // []
      pDC->MoveTo(box.left + quarterX, box.top + quarterY);
      pDC->LineTo(box.right - quarterX, box.top + quarterY);
      pDC->LineTo(box.right - quarterX, box.bottom - quarterY);
      pDC->LineTo(box.left + quarterX, box.bottom - quarterY);
      pDC->LineTo(box.left + quarterX, box.top + quarterY);
      if (bHasLines)
      {
         // -
         pDC->MoveTo(box.right - quarterX, box.top + box.Height() / 2);
         pDC->LineTo(box.right, box.top + box.Height() / 2);
         // | top
         pDC->MoveTo(box.left + box.Width() / 2, box.top + quarterY);
         pDC->LineTo(box.left + box.Width() / 2, box.top);
      }
      // - in box
      pDC->SelectObject(&penBlack);
      pDC->MoveTo(box.left + quarterX, box.top + box.Height() / 2);
      pDC->LineTo(box.right - quarterX, box.top + box.Height() / 2);
      pDC->SelectObject(pOldPen);
      break;
   case tc_MinusDrop:
      // [-]-
      //  |
      pOldPen = pDC->SelectObject(&penDark);
      // []
      pDC->MoveTo(box.left + quarterX, box.top + quarterY);
      pDC->LineTo(box.right - quarterX, box.top + quarterY);
      pDC->LineTo(box.right - quarterX, box.bottom - quarterY);
      pDC->LineTo(box.left + quarterX, box.bottom - quarterY);
      pDC->LineTo(box.left + quarterX, box.top + quarterY);
      if (bHasLines)
      {
         // -
         pDC->MoveTo(box.right, box.top + box.Height() / 2);
         pDC->LineTo(box.right - quarterX, box.top + box.Height() / 2);
         // | top
         pDC->MoveTo(box.left + box.Width() / 2, box.top);
         pDC->LineTo(box.left + box.Width() / 2, box.top + quarterY);
         // | bottom
         pDC->MoveTo(box.left + box.Width() / 2, box.bottom);
         pDC->LineTo(box.left + box.Width() / 2, box.bottom - quarterY);
      }
      // - in box
      pDC->SelectObject(&penBlack);
      pDC->MoveTo(box.left + quarterX, box.top + box.Height() / 2);
      pDC->LineTo(box.right - quarterX, box.top + box.Height() / 2);
      pDC->SelectObject(pOldPen);
      break;
   case tc_Link:
      // |
      // +-
      pOldPen = pDC->SelectObject(&penDark);
      pDC->MoveTo(box.left + box.Width() / 2, box.top);
      pDC->LineTo(box.left + box.Width() / 2, box.top + box.Height() / 2);
      pDC->LineTo(box.right, box.top + box.Height() / 2);
      pDC->SelectObject(pOldPen);
      break;
   case tc_LinkDrop:
      // |
      // +-
      // |
      pOldPen = pDC->SelectObject(&penDark);
      pDC->MoveTo(box.left + box.Width() / 2, box.top);
      pDC->LineTo(box.left + box.Width() / 2, box.bottom);
      pDC->MoveTo(box.left + box.Width() / 2, box.top + box.Height() / 2);
      pDC->LineTo(box.right, box.top + box.Height() / 2);
      pDC->SelectObject(pOldPen);
      break;
   case tc_Drop:
      // |
      pOldPen = pDC->SelectObject(&penDark);
      pDC->MoveTo(box.left + box.Width() / 2, box.top);
      pDC->LineTo(box.left + box.Width() / 2, box.bottom);
      pDC->SelectObject(pOldPen);
      break;
   }
}

void TreeCtrlPrint::DrawTreeItem(const CTreeCtrl& treeCtrl, HTREEITEM hPrintItem, CDC *pDC, CRect rect, CWnd *pWnd)
{
   // we may/will need to offet the text to get the correct position
   HTREEITEM   hRootItem = treeCtrl.GetRootItem();
   HTREEITEM   hIndent = hPrintItem;
   int         indent = 0;                   // number of indent levels
   TVITEM      itemInfo;
   CImageList  *pIL = treeCtrl.GetImageList(TVSIL_NORMAL);
   bool     bLinesAtRoot = ((::GetWindowLong(treeCtrl.m_hWnd, GWL_STYLE) & TVS_LINESATROOT) != 0);

   if (bLinesAtRoot)
   {
      // we have lines at root, need to move everything over to make room for them
      indent++;
   }

   // count the indent levels for this item
   HTREEITEM hParentItem = treeCtrl.GetParentItem(hPrintItem);
   while (hIndent != NULL && hParentItem != NULL)
   {
      hIndent = treeCtrl.GetParentItem(hIndent);
      hParentItem = treeCtrl.GetParentItem(hParentItem);
      indent++;
   }

   if (pIL != NULL)
   {
      // make room for any icon next to the item
      indent++;
   }

   // work out the position of the text
   rect.left += indent * rect.Height();            // offset our rectangle to avoid lines/buttons

   if (::GetWindowLong(treeCtrl.m_hWnd, GWL_STYLE) & (TVS_HASBUTTONS | TVS_HASLINES))
   {
      // the item needs to have lines or +/- buttons drawn for it
      // correct position immediately to left of icon
      CRect box(rect.left - rect.Height() * 2 , rect.top, rect.left -rect.Height() - 1, rect.bottom);
      hIndent = hPrintItem;
      // draw items until we get to the null item
      while (hIndent != NULL)
      {
         hParentItem = treeCtrl.GetParentItem(hIndent);
         if (hParentItem == NULL && !bLinesAtRoot)
         {
            // done drawing this item,
            break;
         }
         tc_DrawTypes style = GetIndentLevelStyle(treeCtrl, hPrintItem, hIndent);
         DrawTreeStyle(treeCtrl, pDC, box, style);
         box -= CPoint(rect.Height(), 0);
         // get the style for the item at this level
         hIndent = treeCtrl.GetParentItem(hIndent);
      }
   }

   CString text = treeCtrl.GetItemText(hPrintItem);
   pDC->DrawText(text, rect, DT_END_ELLIPSIS | DT_EXPANDTABS | DT_SINGLELINE | DT_VCENTER);
   if (pIL != NULL)
   {
      // draw the image next to the item
      itemInfo.mask = TVIF_IMAGE;
      itemInfo.hItem = hPrintItem;
      treeCtrl.GetItem(&itemInfo);
      if (itemInfo.iImage >= 0)
      {
         HANDLE hDib;
         hDib = ImageToDIB(pIL, itemInfo.iImage, pWnd);
         BITMAPINFOHEADER  *pBMI;
         pBMI = (BITMAPINFOHEADER*)GlobalLock(hDib);
         int nColors = 0;
         if (pBMI->biBitCount <= 8)
         {
            nColors = (1 << pBMI->biBitCount);
         }
         // print the correct image
         ::StretchDIBits(pDC->m_hDC,
            rect.left - rect.Height(),
            rect.top,
            rect.Height(),
            rect.Height(),
            0,
            0,
            pBMI->biWidth,
            pBMI->biHeight,
            (LPBYTE)pBMI + (pBMI->biSize + (nColors * sizeof(RGBQUAD))),
            (BITMAPINFO*)pBMI,
            DIB_RGB_COLORS,
            SRCCOPY);
         // free resources
         GlobalUnlock(hDib);
         GlobalFree(hDib);
      }
   }
}

HANDLE TreeCtrlPrint::ImageToDIB(CImageList* pImageList, int iImageNumber, CWnd* pWnd)
{
    // Local Variables
    CBitmap    bitmap;
    CWindowDC  dc(pWnd);

    CDC     memDC;
    CRect      rect;
    CPalette    pal;
    IMAGEINFO   imageInfo;

    if (!pImageList->GetImageInfo(iImageNumber, &imageInfo))
   {
        // Getting of the Imageinfos failed
        return NULL;
   }

    // Create compatible stuff and select Bitmap
    if (!memDC.CreateCompatibleDC(&dc))
   {
        // Create failed
        return NULL;
   }

    if (!bitmap.CreateCompatibleBitmap(&dc,
      imageInfo.rcImage.bottom-imageInfo.rcImage.top,
      imageInfo.rcImage.right-imageInfo.rcImage.left))
   {
        // Create failed
      memDC.DeleteDC() ;
        return NULL;
   }

    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
    if(NULL == pOldBitmap)
   {
        // Select failed
      memDC.DeleteDC() ;
        return NULL;
   }

    // Local Variables for Draw
    CPoint point(0, 0);
    UINT nStyle = ILD_NORMAL;

    // Draw Image to the compatible DC
    if(!pImageList->Draw(&memDC, iImageNumber, point, nStyle))
   {
        // Drawing of the Image failed
      memDC.SelectObject(pOldBitmap) ;
      VERIFY(bitmap.DeleteObject()) ;
      memDC.DeleteDC() ;
        return NULL;
   }

    // Create logical palette if device support a palette
    if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
   {
      UINT        nSize   = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
      LOGPALETTE* pLP     = (LOGPALETTE*)new BYTE[nSize];
      pLP->palVersion     = 0x300;
      pLP->palNumEntries = (unsigned short)GetSystemPaletteEntries(dc, 0, 255,
         pLP->palPalEntry);

      // Create the palette
      pal.CreatePalette(pLP);

      // Free memory
      delete[] pLP;
   }

    memDC.SelectObject(pOldBitmap);
   memDC.DeleteDC() ;

    // Convert the bitmap to a DIB
    HANDLE h = DDBToDIB(bitmap, BI_RGB, &pal);
   VERIFY(bitmap.DeleteObject()) ;
   return h ;
}

// DDBToDIB    - Creates a DIB from a DDB
// bitmap      - Device dependent bitmap
// dwCompression  - Type of compression - see BITMAPINFOHEADER
// pPal        - Logical palette
HANDLE TreeCtrlPrint::DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal)
{
   BITMAP         bm;
   BITMAPINFOHEADER  bi;
   LPBITMAPINFOHEADER   lpbi;
   DWORD       dwLen;
   HANDLE         hDIB;
   HANDLE         handle;
   HDC         hDC;
   HPALETTE    hPal;


   ASSERT(bitmap.GetSafeHandle());

   // The function has no arg for bitfields
   if(dwCompression == BI_BITFIELDS)
      return NULL;

   // If a palette has not been supplied use defaul palette
   hPal = (HPALETTE) pPal->GetSafeHandle();
   if (hPal==NULL)
      hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

   // Get bitmap information
   bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

   // Initialize the bitmapinfoheader
   bi.biSize      = sizeof(BITMAPINFOHEADER);
   bi.biWidth     = bm.bmWidth;
   bi.biHeight       = bm.bmHeight;
   bi.biPlanes       = 1;
   bi.biBitCount     = (unsigned short)(bm.bmPlanes * bm.bmBitsPixel) ;
   bi.biCompression  = dwCompression;
   bi.biSizeImage    = 0;
   bi.biXPelsPerMeter   = 0;
   bi.biYPelsPerMeter   = 0;
   bi.biClrUsed      = 0;
   bi.biClrImportant = 0;

   // Compute the size of the  infoheader and the color table
   int nColors = 0;
   if(bi.biBitCount <= 8)
   {
      nColors = (1 << bi.biBitCount);
   }
   dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

   // We need a device context to get the DIB from
   hDC = ::GetDC(NULL);
   hPal = SelectPalette(hDC,hPal,FALSE);
   RealizePalette(hDC);

   // Allocate enough memory to hold bitmapinfoheader and color table
   hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

   if (!hDIB){
      SelectPalette(hDC,hPal,FALSE);
      ::ReleaseDC(NULL,hDC);
      return NULL;
   }

   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

   *lpbi = bi;

   // Call GetDIBits with a NULL lpBits param, so the device driver
   // will calculate the biSizeImage field
   GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
      (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

   bi = *lpbi;

   // If the driver did not fill in the biSizeImage field, then compute it
   // Each scan line of the image is aligned on a DWORD (32bit) boundary
   if (bi.biSizeImage == 0){
      bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
         * bi.biHeight;

      // If a compression scheme is used the result may infact be larger
      // Increase the size to account for this.
      if (dwCompression != BI_RGB)
         bi.biSizeImage = (bi.biSizeImage * 3) / 2;
   }

   // Realloc the buffer so that it can hold all the bits
   dwLen += bi.biSizeImage;
   handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE) ;
   if (handle != NULL)
      hDIB = handle;
   else
   {
      GlobalFree(hDIB);

      // Reselect the original palette
      SelectPalette(hDC,hPal,FALSE);
      ::ReleaseDC(NULL,hDC);
      return NULL;
   }

   // Get the bitmap bits
   lpbi = (LPBITMAPINFOHEADER)hDIB;

   // FINALLY get the DIB
   BOOL bGotBits = GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(),
      0L,            // Start scan line
      (DWORD)bi.biHeight,     // # of scan lines
      (LPBYTE)lpbi         // address for bitmap bits
      + (bi.biSize + nColors * sizeof(RGBQUAD)),
      (LPBITMAPINFO)lpbi,     // address of bitmapinfo
      (DWORD)DIB_RGB_COLORS);    // Use RGB for color table

   if(!bGotBits)
   {
      GlobalFree(hDIB);

      SelectPalette(hDC,hPal,FALSE);
      ::ReleaseDC(NULL,hDC);
      return NULL;
   }

   SelectPalette(hDC,hPal,FALSE);
   ::ReleaseDC(NULL,hDC);
   return hDIB;
}

void TreeCtrlPrint::ExpandAll(CTreeCtrl& treeCtrl)
{
   // expands all items in a tree control
   HTREEITEM hRootItem = treeCtrl.GetRootItem();
   HTREEITEM hItem = hRootItem;

   while (hItem)
   {
      if (treeCtrl.ItemHasChildren(hItem))
      {
         treeCtrl.Expand(hItem, TVE_EXPAND);
      }
      hItem = GetNextTreeItem(treeCtrl, hItem);
   }
}

