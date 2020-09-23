/* gfxlib.c functions in here are loose emulations of CPS graphics */

#include "sf2.h"

#include "structs.h"
#include "player.h"

#include "particle.h"

#include "gemu.h"

#include "gfxlib.h"
#include "lib.h"

#ifndef CPS
#include "gfx_glut.h"
#include <stdio.h>
#endif


extern Game g;

extern CPSGFXEMU gemu;

static void drawsimple_scroll2noattr(Object *obj, const u16 *tiles, int width, int height);


#define IMAGE_END 0
#define IMAGE_NEWLINE 0xffff


#pragma mark ---- Palette Setters ----

void palette_base_scroll1(void) {
    int u, v;
    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
            gemu.PalScroll1[u][v] = RH2DWord(0x86676, 16, u, v);
        }
    }
}
void set_shadow_pen(void) {	/* 1ae2 Shadow opaque colour */
    gemu.PalObject[10][0] = RHWordOffset(0x1af6, g.CurrentStage);
}
void setpalette_objtop(short palette) {		// 16ae
    short u, v;
    for(u=16; u<32; u++) {
        for(v=0;v<16; v++) {
            gemu.PalObject[u][v] = RH3DWord(0x8aaac, 16, 16, palette, u-16, v);
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
            gemu.PalScroll1[u][v] = RH3DWord(0xc0000, 32, 16, palette, u, v);
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
            gemu.PalScroll2[u][v] = RH3DWord(0xc5000, 32, 16, palette, u, v);
		}
    }
}
void setpalette_scroll3(short palette) {		// emulation of 1706
    int u,v;

    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
            gemu.PalScroll3[u][v] = RH3DWord(0xca000, 32, 16, palette, u, v);
        }
    }
}

/**
 * @brief 
 * 
 * @param palette 
 * @see sf2ua/0x1742
 */
void sub_1742(int palette) {
	short u, v;

    for(u=16; u<32; u++) {
        for(v=0;v<16; v++) {
			gemu.PalObject[u][v] = RH3DWord(0x8aaac, 16, 16, palette, u-16, v) | 0xf000;
        }
    }
}


void gfxrepeat(u16 *gfxram, int count, u16 character, u16 attribute) {		// 56fe
    unsigned int i;
    for(i=0; i<count; i++) {
        gfxram[(i*2)+0] = character;
        gfxram[(i*2)+1] = attribute;
    }
}

void clear_scrolls_123(Task *task) {           /* 0x4c3c */
    gfxrepeat((u16 *)BMAP_SCROLL1, 0x1000, GFXROM_SCROLL1 + 0x20, 0);  /* a whitespace */
    gfxrepeat((u16 *)BMAP_SCROLL2, 0x1000, GFXROM_SCROLL2       , 0);  /* first tile is blank */
    gfxrepeat((u16 *)BMAP_SCROLL3, 0x1000, GFXROM_SCROLL3       , 0);    
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
void printnibble (u16 **gfx_p, u32 *gfxcursor, u16 attr, u8 arg, u8 *printzeroes) {   /* 0x51ba */
	u16 x, y;
    if(*printzeroes == 0 && (arg & 0xf) == 0) {
        COORDS_OFFSET(gfxcursor, 16, 0);
        return;
    }
    *printzeroes=TRUE;
	x = *gfxcursor >> 16;
	y = *gfxcursor & 0xffff;
    OBJECT_DRAW(*gfx_p, x, y, (arg & 0xf) + SF2_TILE_OBJ_HEXCHARS, attr);
    COORDS_OFFSET(gfxcursor, 16, 0);
    OBJ_CURSOR_BUMP(*gfx_p);
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
        COORDS_OFFSET(&cp, 16, 0);
        i++;
    }
}           
void FBDrawTileLine(u16 *gfx_p, const u16 *source, int x, int y) {	//5de2
    // regs: %a0, %a2, %d5, %d6
    u16 attr;
    u32 cp = MakePointObj(x, y);
    
    int i=0;
    attr = RHSwapWord(source[0]);
    source++;
    
    while(RHSwapWord(source[i])) {
        OBJECT_DRAW(gfx_p, CP_X , CP_Y, RHSwapWord(source[i]), attr);
        OBJ_CURSOR_BUMP(gfx_p);
        COORDS_OFFSET(&cp, 16, 0);
        i++;
    }
}           

static void _putchar(u16 **cursor, u32 *gfxcursor, u16 arg, u16 attr) { /* 521a a kind of putchar */
	u16 x,y;
	x = *gfxcursor >> 16;
	y = *gfxcursor & 0xffff;
	
    OBJECT_DRAW(*cursor, x, y , arg & 0xff + SF2_TILE_OBJ_ASCII_12X12, attr);  /* main charset */ 
    COORDS_OFFSET(gfxcursor, 12,0);
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
/*!
 Print number in 12-pixel wide digits
 */
void sub_516a(u16 **gfx_p, u32 *cp_p, u8 d0, short *leading_zero, u16 d3 ) {		// 516a
	u32 cp;
	if (*leading_zero == 0) {
		if (d0 & 0xf) {
			*leading_zero = 1;
		} else {
			*leading_zero = 0;
			COORDS_OFFSET(cp_p, 12, 0);
			return;
		}
	}
	
	cp = *cp_p;
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, SF2_TILE_OBJ_ASCII_12X12 + '0' + (d0 & 0xf),d3);
	OBJ_CURSOR_BUMP(*gfx_p);
	/* todo: and other buffer */
	COORDS_OFFSET(cp_p, 12, 0);	
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
	u16 *gfx_p;
	u8 printzeroes;
	
	printzeroes = TRUE;
	
	coords = MakePointObj(176, 208);
	OBJ_CURSOR_SET(gfx_p, 14);			/* 0x910070 */
	
	printnibble(&gfx_p, &coords, PALETTE_0D, g.x8ab9 >> 4, &printzeroes);	/* number of barrels remaining */
	printnibble(&gfx_p, &coords, PALETTE_0D, g.x8ab9     , &printzeroes);
}
void print_timeremaining(void) {		//5260
	u32 coords;
	u16 *gfx_p;
	u8 printzeroes;
	
	printzeroes = TRUE;
	
	coords = MakePointObj(176, 208);
	OBJ_CURSOR_SET(gfx_p, 14);			/* 0x910070 */
	printnibble(&gfx_p, &coords, PALETTE_0D, g.TimeRemainBCD >> 4, &printzeroes);
	printnibble(&gfx_p, &coords, PALETTE_0D, g.TimeRemainBCD     , &printzeroes);
}
	

/* libtextgfx */

void DrawTileText(enum libtextgfx_sel sel) {	/* sub_5816 */
	static const short data_8df6c[]={
		0x003C, 0x0054, 0x006C, 0x0084, 0x009C, 0x00B4, 0x00CC, 0x00E4, 
		0x00FC, 0x0114, 0x012C, 0x014E, 0x0170, 0x0186, 0x019C, 0x01AA, 
		0x01B8, 0x01C6, 0x01D2, 0x01F4, 0x022E, 0x0268, 0x0282, 0x029C, 
		0x02AA, 0x02B8, 0x02D8, 0x02F0, 0x0310, 0x0328, 
	};
	static const u16 tiles_8df6c[]={
		0x0000, 0x0008, 0x00F0, 0x0000, 0x8120, 0x8121, 0x8122, 0x8123, 0x8124, 0x8125, 0x8126, 0x0000, 
		0x0038, 0x0100, 0x00F0, 0x0000, 0x8120, 0x8121, 0x8122, 0x8123, 0x8124, 0x8125, 0x8126, 0x0000, 
		0x0000, 0x0008, 0x00F0, 0x0000, 0x8130, 0x8131, 0x8132, 0x8133, 0x8134, 0x8135, 0x8136, 0x0000, 
		0x0038, 0x0100, 0x00F0, 0x0000, 0x8130, 0x8131, 0x8132, 0x8133, 0x8134, 0x8135, 0x8136, 0x0000, 
		0x0000, 0x0008, 0x00F0, 0x0000, 0x8138, 0x8139, 0x813A, 0x813B, 0x813C, 0x813D, 0x813E, 0x0000, 
		0x0038, 0x0100, 0x00F0, 0x0000, 0x8138, 0x8139, 0x813A, 0x813B, 0x813C, 0x813D, 0x813E, 0x0000, 
		0x0000, 0x0008, 0x00F0, 0x0000, 0x8110, 0x8111, 0x8112, 0x8113, 0x8114, 0x8115, 0x8116, 0x0000, 
		0x0038, 0x0100, 0x00F0, 0x0000, 0x8110, 0x8111, 0x8112, 0x8113, 0x8114, 0x8115, 0x8116, 0x0000, 
		0x0000, 0x0008, 0x00F0, 0x0000, 0x8118, 0x8119, 0x811A, 0x811B, 0x811C, 0x811D, 0x811E, 0x0000, 
		0x0038, 0x0100, 0x00F0, 0x0000, 0x8118, 0x8119, 0x811A, 0x811B, 0x811C, 0x811D, 0x811E, 0x0000, 
		0x0200, 0x00A0, 0x00A0, 0x000D, 0x8169, 0x816A, 0x816B, 0x816C, 0xFFFF, 
				0x00A0, 0x0090, 0x000D, 0x8179, 0x817A, 0x817B, 0x817C, 0x0000, 
		0x0200, 0x00A0, 0x00A0, 0x000C, 0x8160, 0x8161, 0x8162, 0x8163, 0xFFFF, 
				0x00A0, 0x0090, 0x000C, 0x8170, 0x8171, 0x8172, 0x8173, 0x0000, 
		0x0000, 0x0008, 0x00F0, 0x0000, 0x8128, 0x8129, 0x812A, 0x812B, 0x812C, 0x812D, 0x0000, 
		0x0038, 0x0100, 0x00F0, 0x0000, 0x8128, 0x8129, 0x812A, 0x812B, 0x812C, 0x812D, 0x0000,
        0x0200, 0x0080, 0x00C0, 0x000D, 0x818A, 0x818B, 0x0000,
		0x0210, 0x0080, 0x00B0, 0x000D, 0x819A, 0x819B, 0x0000, 
		0x0220, 0x0080, 0x0098, 0x000D, 0x818C, 0x818D, 0x0000,
        0x0200, 0x00A0, 0x0090, 0x000D, 0x811F, 0x0000,
		0x0200, 0x00A0, 0x0090, 0x000D, 0x81BA, 0x81BB, 0x81BC, 0x81BD, 0xFFFF, 
				0x00A0, 0x00A0, 0x000D, 0x81AA, 0x81AB, 0x81AC, 0x81AD, 0x0000, 
		0x0180, 0x0000, 0x0050, 0x0016, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x0000, 
		0x0180, 0x0000, 0x0040, 0x0016, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x0000, 
		0x0200, 0x0088, 0x0080, 0x0000, 0x8118, 0x8119, 0x811A, 0x811B, 0x811C, 0x811D, 0x811E, 0x0000, 
		0x0000, 
		0x0160, 0x0080, 0x00EC, 0x000D, 0x7E18, 0x7E19, 0x7E1A, 0x7E1B, 0x7E1C, 0x7E1D, 0x7E1E, 0x7E1F, 0x0000, 
		0x01A0, 0x0020, 0x0090, 0x0012, 0x7E08, 0x7E09, 0x0000, 
		0x01B0, 0x0140, 0x0090, 0x0012, 0x7E0A, 0x7E0B, 0x0000, 
		0x0000, 0x0070, 0x00B0, 0x0006, 0x2590, 0x2591, 0x25E0, 0x25E1, 0x25E2, 0xFFFF, 
				0x0080, 0x00A0, 0x0006, 0x26E0, 0x26E1, 0x0000, 
		0x0200, 0x00A0, 0x00A0, 0x0006, 0x27A6, 0x27A7, 0x27B7, 0x27E8, 0x27E9, 0x27EA, 0x037C, 0x0000, 
		0x0000, 0x0070, 0x00B0, 0x000E, 0x2590, 0x2591, 0x25E0, 0x25E1, 0x25E2, 0xFFFF, 
				0x0080, 0x00A0, 0x000E, 0x26E0, 0x26E1, 0x0000, 
		0x0200, 0x00A0, 0x00A0, 0x000E, 0x27A6, 0x27A7, 0x27B7, 0x27E8, 0x27E9, 0x27EA, 0x037C, 0x0000, 
		0x0200, 0x0088, 0x00B0, 0x000D, 0x80D0, 0x80C5, 0x80D2, 0x80C6, 0x80C5, 0x80C3, 0x80D4, 0x0000, 
	};
	
	
	u16 *cur;
	const u16 *gp= &tiles_8df6c[(data_8df6c[sel & (LIBTEXT_ERASE-1)] - 0x3c)/2];    /* XXX magic: 0x3c is sizeof data_8df6c when it is u16[] */
    
	if(sel & LIBTEXT_ERASE) {		/* Erase it */
		OBJ_CURSOR_CPS (cur, 0x910000 + *gp++);
		u16 tile;
		
		gp += 3;
		
		while ((tile = *gp++)) {
			if (tile == 0xffff) {
				gp += 3;
			} else {
				OBJECT_DRAW(cur, 0, 0, 0, 0);				
			}
			OBJ_CURSOR_BUMP(cur);
		}
	} else {			/* Draw it */
		OBJ_CURSOR_CPS (cur, 0x910000 + *gp++);
		u16 x,y,tile;
		
		x = gp[0] + 64;
		y = (gp[1] ^ 0xff)+1;         
		u16 attrs = gp[2];
		gp += 3;
		while ((tile = *gp++)) {
			if (tile == IMAGE_NEWLINE) {
				x = gp[0] + 64;
				y = (gp[1] ^ 0xff) + 1;
				attrs = gp[2];
				gp += 3;

			} else {
				OBJECT_DRAW(cur, x, y, tile, attrs);
				OBJ_CURSOR_BUMP(cur);
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
	
    gemu_clear_object();
    
#ifndef CPS
	gemu_set_cache_clear();
#endif
}

void clear_rowscroll(void) {	/* 6080 clear rowscroll*/
	// kludge
	for (int i=0; i<2048; ++i) {
		gemu.RowScroll2[i] = 0;
	}
	/* XXX rowscroll todo */
	
}

static void sub_911c(u16 arg) {		// set attributes for 2 object tiles
	u16 *cur;
	OBJ_CURSOR_SET(cur, 14);
	cur[3] = arg;
	cur[7] = arg;
	/* also other buffer */
}
void sub_90c8(void) {	/* low time remaining, flash the time display */
	static char state, timer;		// were in Game struct at 0x0ae8-9
	switch (state) {
		case 0:
			NEXT(state);
			timer = 4;
			sub_911c(0xd);
			break;
		case 2:
			if(--timer) { return; }
			NEXT(state);
			timer = 2;
			sub_911c(0xe);
			break;
		case 4:
			if(--timer) { return; }
			state = 0;
			timer = 4;
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

void sub_41c2(Object *obj, const FBSimpleAction *act) {		//41c2
    obj->ActionScript = (FBAction *)act;
    obj->Timer = obj->ActionScript->Delay;
    obj->AnimFlags = obj->ActionScript->Flags;
    draw_simple(obj);
}

void RHSetScrollAction(Object *obj, const FBSimpleAction *act) {
    obj->ActionScript = (FBAction *)act;
    obj->Timer        = RHSwapWord(obj->ActionScript->Delay);
    obj->AnimFlags    = RHSwapWord(obj->ActionScript->Flags);
}
void RHSetScrollActionList(Object *obj, void *act, int step) {
    RHSetScrollAction(obj, (const FBSimpleAction *)RHOffsetLookup16(act, step));
}

void setactiondraw(Object *obj, const FBSimpleAction **act, int step) {	//41b6
	/* 41c2 inlined for efficiency */
    obj->ActionScript = (FBAction *)act[step];
    obj->Timer = obj->ActionScript->Delay;
    obj->AnimFlags = obj->ActionScript->Flags;
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

void DrawFighterNameAt(u16 *gfx_p, int x_d5, int y_d6, Player *ply, const u16 *a3) {			// 968c
	FBDrawTileLine(gfx_p, RHOffsetLookup16(a3, ply->FighterID), x_d5, y_d6);
}

void fight_player_names() {			// 961a
	u16 *gfx_p;
    
	if (g.OnBonusStage == FALSE || g.Player1.Human) {
		OBJ_CURSOR_SET(gfx_p, 28);
		if (g.Version == VERSION_JAP) {
			DrawFighterNameAt(gfx_p, 32, 208, PLAYER1, RHCODE16(0x96b6));
		} else {
			DrawFighterNameAt(gfx_p, 32, 208, PLAYER1, RHCODE16(0x96e6));
		}
	}
	if (g.OnBonusStage == FALSE || g.Player2.Human) {
		OBJ_CURSOR_SET(gfx_p, 37);
		if (g.Version == VERSION_JAP) {
            DrawFighterNameAt(gfx_p, RHWordOffset(0x969e, g.Player2.FighterID), 208, PLAYER2, RHCODE16(0x96e6));
		} else {
            DrawFighterNameAt(gfx_p, RHWordOffset(0x96ce, g.Player2.FighterID), 208, PLAYER2, RHCODE16(0x96e6));
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
				if(RHSwapWord(*a1) & 0x8000) {
					/* 431e */
					a1+=2;
					a2+=2;
				} else {
					a2[0] = GFXROM_SCROLL2 + RHSwapWord(a1[0]);
					a2[1] = RHSwapWord(a1[1]);
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
				if((RHSwapWord(a1[-2]) & 0x8000) == 0) {
					a2[-2] = GFXROM_SCROLL2 + RHSwapWord(a1[-2]);
					a2[-1] = ATTR_X_FLIP ^ RHSwapWord(a1[-1]);
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
            SCR1_DRAW_TILE_NOATTR(coord2, RHSwapWord(tiles[0]) + GFXROM_SCROLL1);
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
            SCR1_DRAW_TILE(coord2, RHSwapWord(tiles[0]) + GFXROM_SCROLL1, RHSwapWord(tiles[1]));
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
void drawsimple_scroll2noattr_check(Object *obj, const u16 *tiles, int width, int height) {	/* 0x42c2 */
    if(test_offset_scroll2((Player *)obj)) { return; }
    drawsimple_scroll2noattr(obj, tiles, width, height);
}
static void drawsimple_scroll2noattr(Object *obj, const u16 *tiles, int width, int height) {	/* 0x42c8 */
	int x,y;
    CPSCOORD coord=objcoords_scroll2(obj);
    CPSCOORD coord2;
    for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
			/* tile, attr*/
            SCR2_DRAW_TILE_NOATTR(coord2, RHSwapWord(tiles[0]) + GFXROM_SCROLL2);
			tiles++;
			SCR2_CURSOR_BUMP(coord2, 0, 1);
		}
        SCR2_CURSOR_BUMP(coord, 1, 0);
    }
    /* original did return zero, should we? */
}
static void drawsimple_scroll2attr(Object *obj, const u16 *tiles, int width, int height) { /* 0x42f6 */
	int x,y;
    CPSCOORD coord=objcoords_scroll2(obj);
    CPSCOORD coord2;
	for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
			if ((RHSwapWord(tiles[0]) & 0x8000) == 0) {
				SCR2_DRAW_TILE(coord2, RHSwapWord(tiles[0]) + GFXROM_SCROLL2, RHSwapWord(tiles[1]));
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
    CPSCOORD coord=objcoords_scroll3(obj);
    CPSCOORD coord2;
    for(x=0; x<width; x++) {
        coord2=coord;
        for(y=0; y<height; y++) {
			/* tile, attr*/
            SCR3_DRAW_TILE_NOATTR(coord2, RHSwapWord(tiles[0]) + GFXROM_SCROLL3);
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
			if ((tiles[0] & 0x8000) == 0) {
				SCR3_DRAW_TILE(coord2, RHSwapWord(tiles[0]) + GFXROM_SCROLL3, RHSwapWord(tiles[1]));
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

    if (RHSwapWord(obj->ActionScript->Flags) & 0x8000) {
        u32 *next = (void *)obj->ActionScript + sizeof(FBSimpleAction);
        obj->ActionScript = (const FBAction *)RHCODE(RHSwapLong(*next));
    } else {
        const void *next = (void *)obj->ActionScript + sizeof(FBSimpleAction);
        obj->ActionScript = (const FBAction *)next;
    }
    obj->Timer       = RHSwapWord(obj->ActionScript->Delay);
    obj->AnimFlags   = RHSwapWord(obj->ActionScript->Flags);
    draw_simple(obj);
}        

void draw_simple(Object *obj) {             /* 0x4200 */
    unsigned int width, height, palette;
	const u16 *tiles;
    
    struct image2 *im = (struct image2 *)RHCODE(RHSwapLong(obj->ActionScript->Image));
    
    width   = RHSwapWord(im->Width);
    height  = RHSwapWord(im->Height);
    palette = RHSwapWord(im->Palette);  // not actually a palette: a flag indicating tile, attr pairs

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
    FBSetPalette(16, RHCODE16_ARRAY(0x1a42, 16, 0));
    FBSetPalette(17, RHCODE16_ARRAY(0x1a42, 16, 1));
    FBSetPalette(18, RHCODE16_ARRAY(0x1a42, 16, 2));
    FBSetPalette(19, RHCODE16_ARRAY(0x1a42, 16, 3));
    FBSetPalette(20, RHCODE16_ARRAY(0x1a42, 16, 4));
    
	setpalette_scroll1(0x13);
	setpalette_scroll2(g.Palette1);
	setpalette_scroll3(g.Palette1);
}

#pragma mark Letterbox drawers for ending seqs

static void sub_6196(u16 **gfxp, u16 tile, u16 attr, int count) {
	int i;
	for (i=0; i<=count; ++i) {
		SCR1_DRAW_TILE(*gfxp, tile, attr);
		SCR1_CURSOR_BUMP(*gfxp, 0, 1);
	}
}
	
static void sub_613c(u16 *gfxp) {
	int i;
	u16 *gfxp2;
	for (i=0; i<=9; ++i) {
		gfxp2 = gfxp;
		sub_6196(&gfxp2, 0x4011, 0x1f, 31);
		SCR1_CURSOR_BUMP(gfxp, 1, 0);
	}
}

static void sub_615c(u16 *gfxp) {
	int i;
	u16 *gfxp2;
	for (i=0; i<=0x1b; ++i) {
		gfxp2 = gfxp;
		sub_6196(&gfxp2, 0x4011, 0x9f, 7);
		sub_6196(&gfxp2, 0x4020, 0x00, 0xd);
		sub_6196(&gfxp2, 0x4011, 0x1f, 9);
		SCR1_CURSOR_BUMP(gfxp, 1, 0);
	}
}
static void sub_61b8(u16 *gfxp) {
	int i;
	u16 *gfxp2;
	for (i=0; i<=0x1b; ++i) {
		gfxp2 = gfxp;
		sub_6196(&gfxp2, 0x4011, 0x1f, 7);
		sub_6196(&gfxp2, 0x4020, 0x00, 0xd);
		sub_6196(&gfxp2, 0x4011, 0x1f, 9);
		SCR1_CURSOR_BUMP(gfxp, 1, 0);
	}
}

void sub_6126(void) {
	u16 *gfxp;
	SCR1_CURSOR_CPS(gfxp, 0x90c000);
	sub_613c(gfxp);
	SCR1_CURSOR_CPS(gfxp, 0x90c500);
	sub_615c(gfxp);
	SCR1_CURSOR_CPS(gfxp, 0x90d300);
	sub_613c(gfxp);
}
void sub_61a0(void) {
	u16 *gfxp;
	SCR1_CURSOR_CPS(gfxp, 0x90c000);
	sub_613c(gfxp);
	SCR1_CURSOR_CPS(gfxp, 0x90c500);
	sub_61b8(gfxp);
	SCR1_CURSOR_CPS(gfxp, 0x90d300);
	sub_613c(gfxp);
	
}