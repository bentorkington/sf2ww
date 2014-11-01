#ifndef INC_GFXLIB
#define INC_GFXLIB

#include	"task.h"
/* gfxlib.c */

#define LIBTEXT_ERASE		0x80
enum libtextgfx_sel {
    INSERT_COIN_P1,
    INSERT_COIN_P2,
    FREE_PLAY_P1,
    FREE_PLAY_P2,
    PUSH_START_P1,		//4
    PUSH_START_P2,
    ADD_COIN_P1,
    ADD_COIN_P2,
    GAME_OVER_P1,		//8
    GAME_OVER_P2,
    DRAW_GAME,			/* 0x0a */
    DOUBLE_KO,
    CONTINUE_P1,		//c
    CONTINUE_P2,
    TIME,				//e
    VITAL,
    BONUS,				//0x10
    ZEROZERO,
    TIME_OVER,
    BLANK_BAR_P1,
    BLANK_BAR_P2,		// 0x014
    GAME_OVER,
    PLAYER_SELECT,
    PLAYER_1,
    PLAYER_2,			//0x018
    HERE_COMES,
    CHALLENGER,
    HERE_COMES_2,
    CHALLENGER_2,		// 0x1c
    PERFECT,			/* 29 */
};

void palette_base_scroll1(void);
void gfxrepeat(unsigned short *gfxram, int count, unsigned short character, unsigned short attribute);
void setpalette_objtop(short palette);
void palette_from_game(void);
void palettes_nextlevel(void);

void palette_macro(int palette);

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

void print_libtextgfx(enum libtextgfx_sel sel);	/* sub_5816 */

void DrawTileLine(u16 *gfx_p, const u16 *source, int x, int y);
int test_offset_scroll1(Player *ply);
int test_offset_scroll2(Player *ply);
int test_offset_scroll3(Player *ply);
void setactiondraw(Object *obj, const CASimpleFrame **act, int step);
void sub_41c2(Object *obj, const struct simpleaction *act);		//41c2
void actiontickdraw(Object *obj);
void draw_simple(Object *obj);
void print_timeremaining(void);
void sub_90c8(void);	/* start the timeremain flashing */
void sub_4386(short d1, short d3, short d4, const 
			  u16 *a1, u16 **gfx_p);

void drawsimple_scroll1attr(Object *obj,  const u16 *tiles, int width, int height);


void set_shadow_pen(void);

void _putlong(u16 **cursor, short x, short y, int arg, short attr);	//51fe
void printlonghex2(u16 **cursor, short x, short y, int arg, short attr);

void sub_6126(void);
void sub_61a0(void);
void sub_516a(u16 **gfx_p, u32 *cp_p, u8 d0, short *leading_zero, u16 d3 );
void sub_5162(u16 **gfx_p, u32 *cp, u8 d0, short *d2, u16 d3);

void fight_player_names();


#endif /* INC_GFXLIB */
