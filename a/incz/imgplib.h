/*******************************************************************************
;
;   MODULE   :     IMGPLIB.H
;
;   USE :         Callable Library function prototypes and #defines
;
;    DATE:         08/23/1992
;
;
;   DESCRIPTION:   Used in an application program for defining current
;                 Accusoft library image processing (dll) function prototypes.
;
;
;         Copyright 1992, Accusoft Corporation.
;
***************************************************************************/

            /* dll exported routines */
int FAR PASCAL IMG_rotate_dib(int,int);
int FAR PASCAL IMG_flip_dibx(int);
int FAR PASCAL IMG_flip_diby(int);
int FAR PASCAL IMG_invert_dib(int);
int FAR PASCAL IMG_resize_dib(int,int,int);
int FAR PASCAL IMG_sharpen_dib(int,int);
int FAR PASCAL IMG_set_lut(int,int,int);

int FAR PASCAL IMG_bayer_mono(int);
int FAR PASCAL IMG_halftone_mono(int);
int FAR PASCAL IMG_diffusion_mono(int);
int FAR PASCAL IMG_bayer_color(int);
int FAR PASCAL IMG_diffusion_color(int);
int FAR PASCAL IMG_popularity_color(int);
int FAR PASCAL IMG_color_gray(int);
int FAR PASCAL IMG_mediancut_color(int);

