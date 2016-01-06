/*******************************************************************************
;
;   MODULE  :     IMGLIB.H
;
;   USE :         Callable Library function prototypes and #defines
;
;   DATE:         06/15/1992
;
;
;  DESCRIPTION:   Used in an application program for defining current
;                 Accusoft library (dll) function prototypes.
;
;
;        Copyright 1992, Accusoft Corporation.
;
***************************************************************************/

            /* dll exported high level routines */
int FAR PASCAL IMG_decompress_bitmap(LPSTR);
int FAR PASCAL IMG_decompress_bitmap_fd(int,unsigned long,int);
int FAR PASCAL IMG_decompress_bitmap_mem(char far *,int);
int FAR PASCAL IMG_save_bitmap(int,LPSTR,int);
int FAR PASCAL IMG_save_bitmap_fd(int,int,int);
int FAR PASCAL IMG_save_bitmap_mem(int,LPSTR,int);

int FAR PASCAL IMG_display_bitmap(int,HDC,int,int,int,int);
int FAR PASCAL IMG_print_bitmap(int,HDC,int,int,int,int);
int FAR PASCAL IMG_print_bitmap_fast(int,HDC,int,int,int,int);
int FAR PASCAL IMG_delete_bitmap(int);
HANDLE FAR PASCAL IMG_bitmap_info (int,int far *,int far *,int far *);
int FAR PASCAL IMG_save_clipboard (LPSTR,HWND);
int FAR PASCAL IMG_get_croprect (int,int far *,int far *,int far *,int far *);
int FAR PASCAL IMG_set_croprect (int,int,int,int,int);
int FAR PASCAL IMG_bitmap_palette (int,HDC);
int FAR PASCAL IMG_create_handle(LPBITMAPINFOHEADER);
int FAR PASCAL IMG_create_handle_ddb(HBITMAP,HPALETTE);


int FAR PASCAL IMGLOW_decompress_bitmap(int,unsigned long,int,void ( *)(char far *,void far *,int,int),
   void far *,int ( *)(LPBITMAPINFOHEADER,void far *));
int FAR PASCAL IMGLOW_decompress_bitmap_mem(char far *,int,void ( *)(char far *,void far *,int,int),
   void far *,int ( *)(LPBITMAPINFOHEADER,void far *));
int FAR PASCAL IMGLOW_save_bitmap(int,LPBITMAPINFOHEADER,int,
      void ( *)(char far *,char far *,int,int),void far *);
int FAR PASCAL IMGLOW_save_bitmap_mem(LPSTR,LPBITMAPINFOHEADER,int,
      void ( *)(char far *,char far *,int,int),void far *);


int FAR PASCAL IMGLOW_get_filetype(LPSTR);
int FAR PASCAL IMGLOW_get_bitmap_name(LPSTR,LPSTR);
int FAR PASCAL IMGLOW_set_bitmap_name(LPSTR,LPSTR);
int FAR PASCAL IMG_dib_to_ddb(int,int,int);
int FAR PASCAL IMG_display_ddb(int,HDC,int,int);
int FAR PASCAL IMGLOW_get_bitmap_header (int,char far *);
int FAR PASCAL IMGLOW_set_comp_quality(int);


            /* dll image processing exported routines */
int FAR PASCAL IMG_rotate_bitmap(int,int);
int FAR PASCAL IMG_flip_bitmapx(int);
int FAR PASCAL IMG_flip_bitmapy(int);
int FAR PASCAL IMG_invert_bitmap(int);
int FAR PASCAL IMG_resize_bitmap(int,int,int);
int FAR PASCAL IMG_sharpen_bitmap(int,int);
int FAR PASCAL IMG_set_lut(int,int,int);


         /* COLOR REDUCTION FUNCTIONS */
int FAR PASCAL IMG_bayer_mono(int);
int FAR PASCAL IMG_halftone_mono(int);
int FAR PASCAL IMG_diffusion_mono(int);
int FAR PASCAL IMG_bayer_color(int);
int FAR PASCAL IMG_diffusion_color(int);
int FAR PASCAL IMG_popularity_color(int);
int FAR PASCAL IMG_color_gray(int);
int FAR PASCAL IMG_mediancut_color(int);




      /* define different compression formats */

#define        TIFF_UNCOMPRESSED 0
#define        BMP_UNCOMPRESSED  1
#define        PCX               2
#define        TARGA             3
#define        GIF               4
#define        WPG               5
#define        WMF               6
#define        TIFF_HUFFMAN      7
#define        TIFF_G3_FAX       8
#define        TIFF_LZW          9
#define        TIFF_G4_FAX       10
#define        DCX               11
#define        BMP_COMPRESSED    12
#define        JPEG              13
#define        EPS               14
#define        PICT              15
