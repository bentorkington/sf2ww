#ifndef INC_GFXLIB
#define INC_GFXLIB

#include	"task.h"
/* gfxlib.c */
void palette_base_scroll1(void);
void gfxrepeat(unsigned short *gfxram, int count, unsigned short character, unsigned short attribute);
void setpalette_objtop(short palette);
void palette_from_game(void);
void palettes_nextlevel(void);
void palette_macro_10(void);
void palette_macro_11(void);
void palette_macro_12(void);
void palette_macro_13(void);

void palette_scr1_16(void);
void palette_scr1_19(void);

void setpalette_scroll1_CS(void);
void setpalette_scroll1(short palette);
void setpalette_scroll2(short palette);
void setpalette_scroll3(short palette);
void printbyte(u16 **cursor, u32 *gfxcursor, unsigned short attr, u8 arg, u8 *printzeroes);
void printnibble(u16 **cursor, u32 *gfxcursor, unsigned short attr, u8 arg, u8 *printzeroes);
void clear_scrolls(void);
void clear_rowscroll(void);	/* actually rowscroll */

void clear_scrolls_123(Task *task);
void print_bonusremaining(void);
// test macros
void sub_6032(void);
void sub_6040(void);
void sub_6060(void);
void sub_1742(int palette);

void print_libtextgfx(u8 sel);	/* sub_5816 */

void DrawTileLine(u16 *gfx_p, const u16 *source, int x, int y);
int test_offset_scroll1(Player *ply);
int test_offset_scroll2(Player *ply);
int test_offset_scroll3(Player *ply);
void setactiondraw(Object *obj, struct simpleaction **act, int step);
void sub_41c2(Object *obj, struct simpleaction *act);		//41c2
void actiontickdraw(Object *obj);
void draw_simple(Object *obj);
void print_timeremaining(void);
void print_libtextgfx(u8 sel);
void sub_90c8(void);	/* start the timeremain flashing */
void sub_4386(short d1, short d3, short d4, const 
			  u16 *a1, u16 **gfx_p);

void set_shadow_pen(void);

void showtextbank1(char sel);
void _putlong(u16 **cursor, short x, short y, int arg, short attr);	//51fe
void printlonghex2(u16 **cursor, short x, short y, int arg, short attr);

#endif /* INC_GFXLIB */
