/* gfxlib.c functions in here are loose emulations of CPS graphics */

#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"


#include "structs.h"
#include "player.h"

#include "particle.h"

#include "gemu.h"

#include "gfxlib.h"
#include "lib.h"


extern Game g;

extern CPSGFXEMU gemu;

extern u8 *data_8d2ac[];

extern u16 data_86676[32][16];
extern u16 data_8a8ac[32][16];
extern u16 data_8aaac[20][16][16];
extern u16 data_c0000[20][32][16];
extern u16 data_c5000[20][32][16];
extern u16 data_ca000[20][32][16];

static void drawsimple_scroll2noattr(Object *obj, const u16 *tiles, int width, int height);


#define IMAGE_END 0
#define IMAGE_NEWLINE 0xffff


#pragma mark ---- Palette Setters ----

void palette_base_scroll1(void) {
    int u, v;
    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
			gemu.PalScroll1[u][v] = data_86676[u][v];
        }
    }
}
void set_shadow_pen(void) {	/* 1ae2 Shadow opaque colour */
	const static u16 data_1af6[15]={
		0x0700, 0x0007, 0x0650, 0x0555, 0x0700, 0x0870, 0x0566, 0x0060,
		0x0555, 0x0666, 0x0000, 0x0700, 0x0700, 0x0666, 0x0666              
	};
	gemu.PalObject[10][0] = data_1af6[g.CurrentStage];
}
void setpalette_objtop(short palette) {		// 16ae
    short u, v;
    for(u=16; u<32; u++) {
        for(v=0;v<16; v++) {
			gemu.PalObject[u][v] = data_8aaac[palette][u-16][v];			
        }
    }
}
void palette_from_game(void) {			// 160c
	printf("palettes_from_game %d\n", g.Palette1);

    setpalette_objtop (g.Palette1);
    setpalette_scroll1(g.Palette1);
    setpalette_scroll2(g.Palette1);
    setpalette_scroll3(g.Palette1);
}
void palettes_nextlevel(void) {			// 1698

    setpalette_objtop (g.CurrentStage);
    setpalette_scroll1(g.CurrentStage);
    setpalette_scroll2(g.CurrentStage);
    setpalette_scroll3(g.CurrentStage);
}
void palette_macro(int palette) {			
    setpalette_objtop (palette);
    setpalette_scroll1(palette);
    setpalette_scroll2(palette);
    setpalette_scroll3(palette);
}


void setpalette_scroll1_CS(void) {
    setpalette_scroll1(g.CurrentStage);
}
void setpalette_scroll1(short palette) {		// emulation of 16ca
    int u,v;

    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
			gemu.PalScroll1[u][v] = data_c0000[palette][u][v];
        }
    }
}

inline void palette_scr1_16(void) {		/* 168c  */
	setpalette_scroll1(16);
}
inline void palette_scr1_19(void) {		// 1692
	setpalette_scroll1(19);
}

void setpalette_scroll2(short palette) {		// emulation of 16ea
    int u,v;

    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
			gemu.PalScroll2[u][v] = data_c5000[palette][u][v];
		}
    }
}
void setpalette_scroll3(short palette) {		// emulation of 1706
    int u,v;

    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
			gemu.PalScroll3[u][v] = data_ca000[palette][u][v];
        }
    }
}


void sub_1742(int palette) {
	short u, v;

    for(u=16; u<32; u++) {
        for(v=0;v<16; v++) {
			gemu.PalObject[u][v] = data_8aaac[palette][u-16][v] | 0xf000; 
        }
    }
}


void gfxrepeat(u16 *gfxram, int count, u16 character, u16 attribute) {		// 56fe
    unsigned int i;
    for(i=0; i<count; i++) {
        gfxram[i*2+0] = character;
        gfxram[i*2+1] = attribute;
    }
}

void clear_scrolls_123(Task *task) {           /* 0x4c3c */
    gfxrepeat((u16 *)BMAP_SCROLL1, 0xfff, GFXROM_SCROLL1 + 0x20, 0);  /* a whitespace */
    gfxrepeat((u16 *)BMAP_SCROLL2, 0xfff, GFXROM_SCROLL2       , 0);  /* first tile is blank */
    gfxrepeat((u16 *)BMAP_SCROLL3, 0xfff, GFXROM_SCROLL3       , 0);    
    DIEFREE;
}



#pragma mark ---- Glyph Printers ----

void printchar2 (u16 *cursor, int x, int y ,u8 *printzeroes, u16 tile, u16 attr) {	/* 0x50f6 */
	if(*printzeroes) {
		OBJECT_DRAW_SINGLE(cursor,x,y, tile, attr);
		//OBJECT_DRAW_SINGLE(cursor+0x4000, x, y, tile, attr);
		OBJ_CURSOR_BUMP(cursor);
		return;
	} else {
		/* 0x5128 */
		*printzeroes = TRUE;
		if(tile & 0xf) {
			OBJECT_DRAW_SINGLE(cursor,x, y, tile, attr);
			//OBJECT_DRAW_SINGLE(cursor+0x4000, x, y, tile, attr);
			OBJ_CURSOR_BUMP(cursor);
			return;
		}
		*printzeroes = FALSE;
		OBJ_CURSOR_BUMP(cursor);
	}
}

void sub_50e2(u16 *cursor, int x, int y, u8 byte, u8 *printzeroes, u16 attr) { /* 0x50e2 */
	/* x and y are actually passed as a coordpair in %d5 */
	printchar2(cursor, x, y, printzeroes, byte >> 4 , attr);
	printchar2(cursor, x, y, printzeroes, byte & 0xf, attr);
}
void printwordhex(u16 *cursor, int x, int y, u16 word, u8 *printzeroes, u16 attr) { /* 0x50d8 */
	/* x and y are actually passed as a coordpair in %d5 */
	sub_50e2(cursor, x, y,  word >>  8 ,printzeroes,  attr);
	sub_50e2(cursor, x, y, word & 0xff,printzeroes, attr);
}
void printbytehex(u16 *cursor, int x, int y, u8 byte, u8 *printzeroes, u16 attr) { /* 0x50ec */
	u32 cp = MakePointObj(x, y);
	printchar2(cursor, CP_X, CP_Y, printzeroes, byte >> 4 , attr);
	printchar2(cursor, CP_X, CP_Y, printzeroes, byte & 0xf, attr);
}
void printbyte (u16 **cursor, u32 *gfxcursor, u16 attr, u8 arg, u8 *printzeroes) {
    printnibble(cursor, gfxcursor, attr, arg   , printzeroes);
    printnibble(cursor, gfxcursor, attr, arg>>4, printzeroes);
}
void printnibble (u16 **cursor, u32 *gfxcursor, u16 attr, u8 arg, u8 *printzeroes) {   /* 0x51ba */
	u16 x, y;
    if(*printzeroes == 0 && (arg & 0xf) == 0) {
        INC_GFX_CURSOR(gfxcursor, 16, 0);
        return;
    }
    *printzeroes=TRUE;
	x = *gfxcursor >> 16;
	y = *gfxcursor & 0xffff;
    OBJECT_DRAW(*cursor, x, y, (arg & 0xf) + SF2_TILE_OBJ_HEXCHARS, attr);
    INC_GFX_CURSOR(gfxcursor, 16, 0);
    OBJ_CURSOR_BUMP(*cursor);
}



void DrawTileLine(u16 *gfx_p, const u16 *source, int x, int y) {	//5de2
	// regs: %a0, %a2, %d5, %d6
	u16 attr;
	u32 cp = MakePointObj(x, y);
    
    int i=0;
    attr = source[0];
    source++;
    
    while(source[i]) {
        OBJECT_DRAW(gfx_p, CP_X , CP_Y, source[i], attr);
		OBJ_CURSOR_BUMP(gfx_p);
		INC_GFX_CURSOR(&cp, 16, 0);
        i++;
    }
}           

static void _putchar(u16 **cursor, u32 *gfxcursor, u16 arg, u16 attr) { /* 521a a kind of putchar */
	u16 x,y;
	x = *gfxcursor >> 16;
	y = *gfxcursor && 0xffff;
	
    OBJECT_DRAW(*cursor, x, y , arg & 0xff + SF2_TILE_OBJ_ASCII, attr);  /* main charset */ 
    INC_GFX_CURSOR(gfxcursor, 12,0);
    OBJ_CURSOR_BUMP(cursor);
}
static void _putword(u16 **cursor, u32 *gfxcursor, u16 arg, u16 attr){		//5208
    _putchar(cursor,gfxcursor,arg >> 8  , attr);
    _putchar(cursor,gfxcursor,arg & 0xff, attr);
}
void _putlong(u16 **cursor, short x, short y, int arg, short attr) {	//51fe
    u32 gfxcursor = MakePointObj(x,y);
    _putword(cursor, &gfxcursor, arg >> 16   , attr);
    _putword(cursor, &gfxcursor, arg & 0xffff, attr);
}
void sub_516a(u16 **gfx_p, u32 *cp_p, u8 d0, short *leading_zero, u16 d3 ) {		// 516a
	u32 cp;
	if (*leading_zero == 0) {
		if (d0 & 0xf) {
			*leading_zero = 1;
		} else {
			*leading_zero = 0;
			INC_GFX_CURSOR(cp_p, 12, 0);
			return;
		}
	}
	
	cp = *cp_p;
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, 0x80b0 + (d0 & 0xf),d3);
	OBJ_CURSOR_BUMP(*gfx_p);
	/* and other buffer */
	INC_GFX_CURSOR(cp_p, 12, 0);	
}

static void sub_5152(u16 **cursor, u32 *gfxcursor, u16 arg, u16 attr) {
	short lz = 0;
	sub_516a(cursor, gfxcursor, arg >> 4, &lz, attr);
	sub_516a(cursor, gfxcursor, arg     , &lz, attr);
}

static void sub_5148(u16 **cursor, u32 *gfxcursor, u16 arg, u16 attr) {
	sub_5152(cursor, gfxcursor, arg >> 8, attr);
	sub_5152(cursor, gfxcursor, arg     , attr);	
}	

void printlonghex2(u16 **cursor, short x, short y, int arg, short attr) {
	u32 gfxcursor = MakePointObj(x, y);
	sub_5148(cursor, &gfxcursor, (arg >> 4) & 0xfff, attr);
	sub_5148(cursor, &gfxcursor, arg,                attr);
}
void sub_5162(u16 **gfx_p, u32 *cp, u8 d0, short *d2, u16 d3) {
	sub_516a(gfx_p, cp, d0 >> 4, d2, d3);
	sub_516a(gfx_p, cp, d0,      d2, d3);
}


void print_bonusremaining(void) {		// 5248
	u32 coords;
	u16 *cur;
	u8 printzeroes;
	
	printzeroes = TRUE;
	
	coords = MakePointObj(176, 208);
	OBJ_CURSOR_SET(cur, 28);			/* 0x910070 */
	
	printnibble(&cur, &coords, PALETTE_0D, g.x8ab9 >> 4, &printzeroes);	/* number of barrels remaining */
	printnibble(&cur, &coords, PALETTE_0D, g.x8ab9     , &printzeroes);
}
void print_timeremaining(void) {		//5260
	u32 coords;
	u16 *cur;
	u8 printzeroes;
	
	printzeroes = TRUE;
	
	coords = MakePointObj(176, 208);
	OBJ_CURSOR_SET(cur, 28);			/* 0x910070 */
	printnibble(&cur, &coords, PALETTE_0D, g.TimeRemainBCD >> 4, &printzeroes);
	printnibble(&cur, &coords, PALETTE_0D, g.TimeRemainBCD     , &printzeroes);
}
	

/* libtextgfx */

void print_libtextgfx(u8 sel) {	/* sub_5816 */
	static const short data_8df6c[]={0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 137, 154, 
		165, 176, 183, 190, 197, 203, 220, 249, 278, 291, 304, 
		311, 318, 334, 346, 362, 374, 
	};
	
	static const u16 tiles_8df6c[]={
		0, /* 0008dfa8 */
		8, 240, 0x0000, 0x8120, 0x8121, 0x8122, 0x8123, 0x8124, 0x8125, 0x8126, IMAGE_END, 
		
		56, /* 0008dfc0 */
		256, 240, 0x0000, 0x8120, 0x8121, 0x8122, 0x8123, 0x8124, 0x8125, 0x8126, IMAGE_END, 
		
		0, /* 0008dfd8 */
		8, 240, 0x0000, 0x8130, 0x8131, 0x8132, 0x8133, 0x8134, 0x8135, 0x8136, IMAGE_END, 
		
		56, /* 0008dff0 */
		256, 240, 0x0000, 0x8130, 0x8131, 0x8132, 0x8133, 0x8134, 0x8135, 0x8136, IMAGE_END, 
		
		0, /* 0008e008 */
		8, 240, 0x0000, 0x8138, 0x8139, 0x813a, 0x813b, 0x813c, 0x813d, 0x813e, IMAGE_END, 
		
		56, /* 0008e020 */
		256, 240, 0x0000, 0x8138, 0x8139, 0x813a, 0x813b, 0x813c, 0x813d, 0x813e, IMAGE_END, 
		
		0, /* 0008e038 */
		8, 240, 0x0000, 0x8110, 0x8111, 0x8112, 0x8113, 0x8114, 0x8115, 0x8116, IMAGE_END, 
		
		56, /* 0008e050 */
		256, 240, 0x0000, 0x8110, 0x8111, 0x8112, 0x8113, 0x8114, 0x8115, 0x8116, IMAGE_END, 
		
		0, /* 0008e068 */
		8, 240, 0x0000, 0x8118, 0x8119, 0x811a, 0x811b, 0x811c, 0x811d, 0x811e, IMAGE_END, 
		
		56, /* 0008e080 */
		256, 240, 0x0000, 0x8118, 0x8119, 0x811a, 0x811b, 0x811c, 0x811d, 0x811e, IMAGE_END, 
		
		512, /* 0008e098 */
		160, 160, 0x000d, 0x8169, 0x816a, 0x816b, 0x816c, IMAGE_NEWLINE,
		160, 144, 0x000d, 0x8179, 0x817a, 0x817b, 0x817c, IMAGE_END, 
		
		512, /* 0008e0ba */
		160, 160, 0x000c, 0x8160, 0x8161, 0x8162, 0x8163, IMAGE_NEWLINE,
		160, 144, 0x000c, 0x8170, 0x8171, 0x8172, 0x8173, IMAGE_END, 
		
		0, /* 0008e0dc */
		8, 240, 0x0000, 0x8128, 0x8129, 0x812a, 0x812b, 0x812c, 0x812d, IMAGE_END, 
		
		56, /* 0008e0f2 */
		256, 240, 0x0000, 0x8128, 0x8129, 0x812a, 0x812b, 0x812c, 0x812d, IMAGE_END, 
		
		512, /* 0008e108 */
		128, 192, 0x000d, 0x818a, 0x818b, IMAGE_END, 
		
		528, /* 0008e116 */
		128, 176, 0x000d, 0x819a, 0x819b, IMAGE_END, 
		
		544, /* 0008e124 */
		128, 152, 0x000d, 0x818c, 0x818d, IMAGE_END, 
		
		512, /* 0008e132 */
		160, 144, 0x000d, 0x811f, IMAGE_END, 
		
		512, /* 0008e13e */
		160, 144, 0x000d, 0x81ba, 0x81bb, 0x81bc, 0x81bd, IMAGE_NEWLINE,
		160, 160, 0x000d, 0x81aa, 0x81ab, 0x81ac, 0x81ad, IMAGE_END, 
		
		384, /* 0008e160 */
		0, 80, 0x0016, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, IMAGE_END, 
		
		384, /* 0008e19a */
		0, 64, 0x0016, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, IMAGE_END, 
		
		512, /* 0008e1d4 */
		136, 128, 0x0000, 0x8118, 0x8119, 0x811a, 0x811b, 0x811c, 0x811d, 0x811e, IMAGE_END, 
		
		352, /* 0008e1ee */
		128, 236, 0x000d, 0x7e18, 0x7e19, 0x7e1a, 0x7e1b, 0x7e1c, 0x7e1d, 0x7e1e, 0x7e1f, IMAGE_END, 
		
		416, /* 0008e208 */
		32, 144, 0x0012, 0x7e08, 0x7e09, IMAGE_END, 
		
		432, /* 0008e216 */
		320, 144, 0x0012, 0x7e0a, 0x7e0b, IMAGE_END, 
		
		0, /* 0008e224 */
		112, 176, 0x0006, 0x2590, 0x2591, 0x25e0, 0x25e1, 0x25e2, IMAGE_NEWLINE,
		128, 160, 0x0006, 0x26e0, 0x26e1, IMAGE_END, 
		
		512, /* 0008e244 */
		160, 160, 0x0006, 0x27a6, 0x27a7, 0x27b7, 0x27e8, 0x27e9, 0x27ea, 0x037c, IMAGE_END, 
		
		0, /* 0008e25c */
		112, 176, 0x000e, 0x2590, 0x2591, 0x25e0, 0x25e1, 0x25e2, IMAGE_NEWLINE,
		128, 160, 0x000e, 0x26e0, 0x26e1, IMAGE_END, 
		
		512, /* 0008e27c */
		160, 160, 0x000e, 0x27a6, 0x27a7, 0x27b7, 0x27e8, 0x27e9, 0x27ea, 0x037c, IMAGE_END, 
		
		512, /* 0008e294 */
		136, 176, 0x000d, 0x80d0, 0x80c5, 0x80d2, 0x80c6, 0x80c5, 0x80c3, 0x80d4, IMAGE_END, 
		
	};
	
	
	u16 *cur;
	const u16 *gp=&tiles_8df6c[data_8df6c[sel & 0x7f]];

	if(sel & 0x80) {		/* Erase it */
		OBJ_CURSOR_CPS (cur, 0x910000 + *gp++);
		u16 tile;
		
		gp += 3;
		
		
		while (tile = *gp++) {
			if (tile == 0xffff) {
				gp += 3;
			} else {
				OBJECT_DRAW(cur, 0, 0, 0, 0);				
			}
			cur += 4;
		}
	} else {			/* Draw it */
		OBJ_CURSOR_CPS (cur, 0x910000 + *gp++);
		u16 x,y,tile;
		
		x = *gp++ + 0x40;
		y = (*gp++ ^ 0xff)+1;         
		u16 attrs = *gp++;
		
		while (tile = *gp++) {
			if (tile == 0xffff) {
				x = *gp++ + 0x40;
				y = (*gp++ ^ 0xff) + 1;
				attrs = *gp++;
			} else {
				OBJECT_DRAW(cur, x, y, tile, attrs);
				cur += 4;
				x += 16;
			}
		}
	}
}

#pragma mark ---- Display Clearers ----





void clear_scrolls(void) {		/* 0x5f10 */
	short i;
	/* clear Scroll1, Scroll2, Scroll3 & Object */
	for (i=0; i<0x1000; i++) {	
		gemu.Tilemap_Scroll1[i][0] = GFXROM_SCROLL1 + ' ';	/* whitespace */
		gemu.Tilemap_Scroll1[i][1] = 0;
		gemu.Tilemap_Scroll2[i][0] = GFXROM_SCROLL2;
		gemu.Tilemap_Scroll2[i][1] = 0;
		gemu.Tilemap_Scroll3[i][0] = GFXROM_SCROLL3;
		gemu.Tilemap_Scroll3[i][1] = 0;
	}
	clear_rowscroll();	/* 6080 */
	g.x02b8 = 0;
	g.x02ba = 0;	
	
	GEMU_CLEAR_OBJECT_D;
#ifndef CPS
	gemu_set_cache_clear();
#endif
}

void clear_rowscroll(void) {	/* 6080 clear rowscroll*/
	/* XXX rowscroll todo */
	
}

static void sub_911c(u16 arg) {		// set attributes for 2 object tiles
	u16 *cur;
	OBJ_CURSOR_SET(cur, 28);
	cur[3] = arg;
	cur[7] = arg;
	/* also other buffer */
}
void sub_90c8(void) {	/* low time remaining, flash the time display */
	switch (g.x0ae8) {
		case 0:
			NEXT(g.x0ae8);
			g.x0ae9 = 4;
			sub_911c(0xd);
			break;
		case 2:
			if(--g.x0ae9) { return; }
			g.x0ae8 += 2;
			g.x0ae9 = 2;
			sub_911c(0xe);
			break;
		case 4:
			if(--g.x0ae9) { return; }
			g.x0ae8 = 2;
			g.x0ae9 = 4;
			sub_911c(0xd);
			break;
		FATALDEFAULT;
	}
}
int test_offset_scroll1(Player *ply) {
    if(ply->XPI - get_scr1x() + 0x40 > 0x200) { return 1;}
/*    if(ply->YPI - g.gstate_scroll1.YPI + 0x80 > 0x200) { return 1;}  */
    if(ply->YPI - g.x8b5c + 0x80 > 0x200) { return 1;}  /* as per the source */

    return 0;
}
int test_offset_scroll2(Player *ply) {
    if(ply->XPI - get_scr2x() + 0x80 > 0x280) { return 1;}
    if(ply->YPI - get_scr2y() + 0x80 > 0x200) { return 1;}
    return 0;
}
int test_offset_scroll3(Player *ply) {
    if(ply->XPI - get_scr3x() + 0x80 > 0x280) { return 1;}
    if(ply->YPI - get_scr3y() + 0x80 > 0x200) { return 1;}
    return 0;
}

void sub_41c2(Object *obj, const struct simpleaction *act) {		//41c2
	obj->ActionScript = (Action *)act; 
    obj->Timer      = obj->ActionScript->Delay;
    obj->AnimFlags  = obj->ActionScript->Loop << 8 | obj->ActionScript->Next; 
	
    draw_simple(obj);
}

void setactiondraw(Object *obj, const CASimpleFrame **act, int step) {	//41b6
	/* 41c2 inlined for efficiency */
    obj->ActionScript = (Action *)act[step]; 
	
    obj->Timer      = obj->ActionScript->Delay;
    obj->AnimFlags  = obj->ActionScript->Loop << 8 | obj->ActionScript->Next; 
  
    draw_simple(obj);
}

static void _fill_with_tile1() {		// 604c
	gfxrepeat(BMAP_SCROLL2, 0x3ff, GFXROM_SCROLL2 + 1, 0);
}

void sub_6032(void) {
	gemu.PalScroll2[0][1] = 0xf765;
	_fill_with_tile1();
}

void sub_6040(void) {
	gemu.PalScroll2[0][1] = 0x0765;
	_fill_with_tile1();
}

void sub_6060(void) {		// 6060 dot cloth scroll2
	gemu.PalScroll2[0][1] = 0xf765;
	gfxrepeat(BMAP_SCROLL2, 0x3ff, GFXROM_SCROLL2 + 4, 0);
}

void DrawFighterNameAt(u16 *gfx_p, u32 cp, Player *ply, const u16 **a3) {			// 968c
	DrawTileLine(gfx_p, a3[ply->FighterID], CP_X, CP_Y);
}

void fight_player_names() {			// 961a
	const static u16 data_96fe[]={PALETTE_0D, 0x1008, 0x1009, 0x0000,  };			// RYU...
	const static u16 data_9706[]={PALETTE_0D, 0x7e0c, 0x7e0d, 0x7e0e, 0x7e0f, 0x0000,  };
	const static u16 data_9712[]={PALETTE_0D, 0x100a, 0x100b, 0x100c, 0x100d, 0x0000,  };
	const static u16 data_971e[]={PALETTE_0D, 0x7fc0, 0x7fc1, 0x7fc2, 0x0000,  };
	const static u16 data_9728[]={PALETTE_0D, 0x100e, 0x100f, 0x0000,  };
	const static u16 data_9730[]={PALETTE_0D, 0x1018, 0x1019, 0x101a, 0x101b, 0x0000,  };
	const static u16 data_973c[]={PALETTE_0D, 0x101c, 0x101d, 0x101e, 0x101f, 0x0000,  };
	const static u16 data_9748[]={PALETTE_0D, 0x103b, 0x103c, 0x103d, 0x103e, 0x103f, 0x0000,  };
	const static u16 data_9756[]={PALETTE_0D, 0x1038, 0x1039, 0x103a, 0x0000,  };
	const static u16 data_9760[]={PALETTE_0D, 0x7fd0, 0x7fd1, 0x7fd2, 0x0000,  };
	const static u16 data_976a[]={PALETTE_0D, 0x1028, 0x1029, 0x102a, 0x102b, 0x0000,  };
	const static u16 data_9776[]={PALETTE_0D, 0x102c, 0x102d, 0x102e, 0x102f, 0x0000,  };
	
	
	const static u16 data_969e[12]={
		0x0144, 0x0120, 0x0126, 0x0130, 0x0141, 0x0122, 0x0122, 0x011f, 0x0138, 0x0130, 0x0122, 0x0128,  };				
	const static u16 data_96ce[12]={
		0x0144, 0x0120, 0x0126, 0x0130, 0x0141, 0x0122, 0x0122, 0x011f, 0x0122, 0x0130, 0x0128, 0x0138,  };
	
	const static u16 *player_names_japan[12]={			// 96b6
		data_96fe, data_9706, data_9712, data_971e, 
		data_9728, data_9730, data_973c, data_9748, 
		data_9756, data_9760, data_976a, data_9776, 
	};													
	const static u16 *player_names_other[12]={			// 96e6
		data_96fe, data_9706, data_9712, data_971e, 
		data_9728, data_9730, data_973c, data_9748, 
		data_976a, data_9760, data_9776, data_9756, 
	};
	
	u16 *gfx_p;
	
	if (g.OnBonusStage == FALSE || g.Player1.Human) {
		OBJ_CURSOR_CPS(gfx_p, 0x9100e0);
		if (g.Version == VERSION_JAP) {
			DrawFighterNameAt(gfx_p, MakePointObj(32, 208), PLAYER1, player_names_japan);
		} else {
			DrawFighterNameAt(gfx_p, MakePointObj(32, 208), PLAYER1, player_names_other);
		}
	}
	if (g.OnBonusStage == FALSE || g.Player2.Human) {
		OBJ_CURSOR_CPS(gfx_p, 0x910128);
		if (g.Version == VERSION_JAP) {
			DrawFighterNameAt(gfx_p, MakePointObj(data_969e[g.Player2.FighterID], 208), PLAYER2, player_names_japan);
		} else {
			DrawFighterNameAt(gfx_p, MakePointObj(data_96ce[g.Player2.FighterID], 208), PLAYER2, player_names_other);
		}
	}
}


void sub_4386(short d1, short width_d3, short width_d4, const u16 *a1, u16 **gfx_p) {		//4386
	short d6,d2;
	u16 *a2;
	
	
	if(d1 == 0) {
		/* 42fa dirty half of drawsimple_Scroll2_attr inlined */
		width_d3--;
		width_d4--;
		for(;width_d3>=0;width_d3--) {			/* postdecrements */
			a2 = *gfx_p;
			for(d2 = width_d4;d2>=0;d2--) {
				if((*a1) & 0x8000) {
					/* 431e */
					a1+=2;
					a2+=2;
				} else {
					a2[0] = GFXROM_SCROLL2 + a1[0];
					a2[1] = a1[1];
					a1 +=2;
					a2 +=2;
				}
			}
			*gfx_p += 0x20;
		}
	} else {
		//438c
		// seek to the end of the image
		a1 += (width_d4  * width_d3) * 2;
		// fix fencepost
		--width_d3;
		--width_d4;
		
		*gfx_p += (width_d4 * 2) + 2;
		
		for(; width_d3 >= 0; --width_d3){
			a2 = *gfx_p;
			for(d6 = width_d4; d6 >= 0; --d6) {
				if((a1[-2] & 0x8000) == 0) {
					a2[-2] = GFXROM_SCROLL2 + a1[-2];
					//a2[-2] = GFXROM_SCROLL2 + 0x4;		// XXX
					a2[-1] = ATTR_X_FLIP ^ a1[-1];
				}
				a1 -= 2;
				a2 -= 2;				
			}
			*gfx_p+=0x20;
		}
	}
}
			
static void drawsimple_scroll1noattr(Object *obj, const u16 *tiles, int width, int height) {  /* 4266 */
	int x,y;
    u16 *coord=objcoords_scroll1(obj);
    u16 *coord2;
    for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
            /* tile */
            SCR1_DRAW_TILE_NOATTR(coord2, tiles[0] + GFXROM_SCROLL1);
			tiles++;
            SCR1_CURSOR_BUMP(coord2, 0, 1);
        }
		SCR1_CURSOR_BUMP(coord, 1, 0);
    }
}
static void drawsimple_scroll1noattr_check(Object *obj, const u16 *tiles, int width, int height) {	/* 4260 */
    if(test_offset_scroll1((Player *)obj)) { return; }
    drawsimple_scroll1noattr(obj, tiles, width, height);
}
void drawsimple_scroll1attr(Object *obj,  const u16 *tiles, int width, int height) {	/* 0x4294 */
	int x,y;
    u16 *coord=objcoords_scroll1(obj);
    u16 *coord2;
    for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
			/* tile, attr*/
            SCR1_DRAW_TILE(coord2, tiles[0] + GFXROM_SCROLL1, tiles[1]);
			tiles++;
			SCR1_CURSOR_BUMP(coord2, 0, 1);
        }
		SCR1_CURSOR_BUMP(coord, 1, 0);
    }
}
static void drawsimple_scroll1attr_check(Object *obj, const u16 *tiles, int width, int height) {		/* 0x428e */
    if(test_offset_scroll1((Player *)obj)) { return; }
    drawsimple_scroll1attr(obj, tiles, width, height);
}
static void drawsimple_scroll2noattr_check(Object *obj, const u16 *tiles, int width, int height) {	/* 0x42c2 */
    if(test_offset_scroll2((Player *)obj)) { return; }
    drawsimple_scroll2noattr(obj, tiles, width, height);
}
static void drawsimple_scroll2noattr(Object *obj, const u16 *tiles, int width, int height) {	/* 0x42c8 */
	int x,y;
    COORD coord=objcoords_scroll2(obj);
    COORD coord2;
    for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
			/* tile, attr*/
            SCR2_DRAW_TILE_NOATTR(coord2, tiles[0] + GFXROM_SCROLL2);    
			tiles++;
			SCR2_CURSOR_BUMP(coord2, 0, 1);
		}
        SCR2_CURSOR_BUMP(coord, 1, 0);
    }
    /* original did return zero, should we? */
}
static void drawsimple_scroll2attr(Object *obj, const u16 *tiles, int width, int height) { /* 0x42f6 */
	int x,y;
    COORD coord=objcoords_scroll2(obj);
    COORD coord2;
	for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
			if (tiles[0] & 0x8000 == 0) {
				SCR2_DRAW_TILE(coord2, tiles[0] + GFXROM_SCROLL2, tiles[1]); 
			}
			tiles+=2;
			SCR2_CURSOR_BUMP(coord2, 0, 1);
        }
        SCR2_CURSOR_BUMP(coord, 1, 0);
    }
    /* original did return zero, should we? */
}
static void drawsimple_scroll2attr_check(Object *obj, const u16 *tiles, int width, int height) {
    if(test_offset_scroll2((Player *)obj)) { return; }
    drawsimple_scroll2attr(obj, tiles, width, height);
}
static void drawsimple_scroll3noattr(Object *obj, const u16 *tiles, int width, int height) {  /* 0x432a */
	int x,y;
    COORD coord=objcoords_scroll3(obj);
    COORD coord2;
    for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
			/* tile, attr*/
            SCR3_DRAW_TILE_NOATTR(coord2, tiles[0] + GFXROM_SCROLL3);    
			tiles++;
			SCR3_CURSOR_BUMP(coord2, 0, 1);
        }
        SCR3_CURSOR_BUMP(coord, 1, 0);
    }
    /* original did return zero, should we? */
}
static void drawsimple_scroll3noattr_check(Object *obj, const u16 *tiles, int width, int height) {	/* 4324 */
    if(test_offset_scroll3((Player *)obj)) { return; }
    drawsimple_scroll3noattr(obj, tiles, width, height);
}
static void drawsimple_scroll3attr(Object *obj, const u16 *tiles, int width, int height) {  /* 0x4358 */
	int x,y;
    u16* coord=objcoords_scroll3(obj);
    u16 *coord2;
    for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
			if (tiles[0] & 0x8000 == 0) {
				SCR3_DRAW_TILE(coord2, tiles[0] + GFXROM_SCROLL3, tiles[1]);    
			}
			tiles += 2;
            SCR3_CURSOR_BUMP(coord2, 0, 1);
        }
        SCR3_CURSOR_BUMP(coord, 1, 0);
    }
    /* original did return zero, should we? */
}
static void drawsimple_scroll3attr_check(Object *obj, const u16 *tiles, int width, int height) { /* 4352 */
    if(test_offset_scroll3((Player *)obj)) { return; }
    drawsimple_scroll3attr(obj, tiles, width, height);
}

void (*DRAW_NOATTR_CHECK[3])(Object *, const u16 *, int, int)   =                            
	{drawsimple_scroll2noattr_check, drawsimple_scroll1noattr_check, drawsimple_scroll3noattr_check};
void (*DRAW_NOATTR_NOCHECK[3])(Object *obj, const u16 *, int, int ) =                  
	{drawsimple_scroll2noattr, drawsimple_scroll1noattr, drawsimple_scroll3noattr}; void (*DRAW_ATTR_CHECK[3])(Object *, const u16 *, int, int)     =                     
	{drawsimple_scroll2attr_check,   drawsimple_scroll1attr_check,    drawsimple_scroll3attr_check};
void (*DRAW_ATTR_NOCHECK[3])(Object *, const u16 *, int, int)   =                     
	{
		drawsimple_scroll2attr, 
		drawsimple_scroll1attr,  
		drawsimple_scroll3attr
	};


void actiontickdraw(Object *obj) {		/* 0x41d4 */
    if(--obj->Timer) { return; }
	
	// XXX C messiness, not emulated
	CASimpleFrame *temp = (CASimpleFrame *)obj->ActionScript;
	temp++;
	
    if(obj->ActionScript->Loop == 0) {
		obj->ActionScript = (const Action *)temp;
	} else {
		obj->ActionScript = ((struct simpleaction *)obj->ActionScript)[1].Image;
    }
    obj->Timer      = obj->ActionScript->Delay;
    obj->AnimFlags  = obj->ActionScript->Loop << 8 | obj->ActionScript->Next; 
    draw_simple(obj);
}        

void draw_simple(Object *obj) {             /* 0x4200 */
    unsigned int width, height, palette;
	const u16 *tiles;
	struct image2 *im = (struct image2 *)obj->ActionScript->Image;
	
    width   = im->Width;
    height  = im->Height;
    palette = im->Palette;		// not actually a palette: a flag indicating tile, attr pairs
	
	tiles = im->Tiles;
    if (palette == 0) {
        if(obj->Pool == 0) {
            DRAW_NOATTR_CHECK[obj->Scroll/2](obj, tiles, width, height);
        } else {
            DRAW_NOATTR_NOCHECK[obj->Scroll/2](obj, tiles, width, height);
        }
    } else {
        if(obj->Pool == 0) {
            DRAW_ATTR_CHECK[obj->Scroll/2](obj, tiles, width, height);
        } else {
            DRAW_ATTR_NOCHECK[obj->Scroll/2](obj, tiles, width, height);
        }
    }
}

void sub_1a0c(void) {			//1a0c 
	const static u16 data_1a42[5][16] = {
		{ 0x0000, 0x055b, 0x089f, 0x0f76, 0x0666, 0x0530, 0x0651, 0x0862, 
		  0x0a73, 0x0c84, 0x0ea7, 0x0fc9, 0x0fec, 0x0fff, 0x0f00, 0x0000, },
		{ 0x0530, 0x0651, 0x0862, 0x0a73, 0x0c84, 0x0d96, 0x0eb7, 0x0fc9, 
		  0x0feb, 0x0ffc, 0x0fff, 0x0b34, 0x0d65, 0x0f87, 0x0666, 0x0000, },
		{ 0x0530, 0x0651, 0x0862, 0x0a73, 0x0c84, 0x0d96, 0x0eb7, 0x0fc9, 
		  0x0fda, 0x0feb, 0x0059, 0x0ffd, 0x006b, 0x0fff, 0x0666, 0x0000, },
		{ 0x0111, 0x0fdc, 0x0fd9, 0x0fb8, 0x0e97, 0x0c86, 0x0965, 0x0643, 
		  0x0600, 0x0999, 0x0600, 0x0900, 0x0c40, 0x0d73, 0x0fa7, 0x0000, },
		{ 0x0530, 0x0651, 0x0862, 0x0a73, 0x0c84, 0x0d96, 0x0eb7, 0x0fc9, 
		  0x0feb, 0x0ffc, 0x0fff, 0x0fff, 0x0ffc, 0x0feb, 0x0666, 0x0000, },
	};
	gemu_setpalette(16, data_1a42[0]);
	gemu_setpalette(17, data_1a42[1]);
	gemu_setpalette(18, data_1a42[2]);
	gemu_setpalette(19, data_1a42[3]);
	gemu_setpalette(20, data_1a42[4]);

	
	/* XXX starting at 920000, copy these */
	setpalette_scroll1(0x13);
	setpalette_scroll2(g.Palette1);
	setpalette_scroll3(g.Palette1);
}
	
