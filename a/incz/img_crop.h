/*;
;        Copyright 1992, Accusoft Corporation.
;
*/
BOOL crop_bitmap_win(HANDLE,HANDLE,HANDLE);
void draw_crop_rect(HDC,HANDLE,int);
void move_crop_rect(int,POINT,HANDLE);
void set_new_crop_rect(HWND,HANDLE);
void set_crop_rect(HANDLE,LPRECT);
void get_crop_rect(HANDLE,LPRECT);
BOOL FAR PASCAL getcroprect(HWND,unsigned int,WORD,LONG);
