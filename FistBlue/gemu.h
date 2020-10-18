/*
 *  gemu.h
 *  GLUTBasics
 *
 *  Created by Ben on 2/01/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#ifndef INC_GEMU
#define INC_GEMU

#include "sf2types.h"

#define OBJECT_DRAW			gemuObjectDraw			
#define OBJECT_DRAW_NOATTR	gemuObjectDrawNoAttr
#define OBJECT_PDRAW		gemuObjectDrawPoint			
#define OBJECT_DRAW_ID		gemuObjectDrawID
#define OBJECT_DRAW_COORDS	gemuObjectDrawIDCoords 
#define COORDS_OFFSET		gemuCoordOffset
#define OBJECT_DRAW_SINGLE	gemuObjectDraw
#define WRITE_END_TAG		gemuWriteEndTag(DSObjCur_g);


#define SCR1_DRAW_TILE(gfx_p, tile, attr)\
(gfx_p)[0] = tile;					\
(gfx_p)[1] = attr;					

#define SCR1_DRAW_TILE_NOATTR(gfx_p, tile)\
(gfx_p)[0] = tile;						

#define SCR2_DRAW_TILE SCR1_DRAW_TILE	
#define SCR2_DRAW_TILE_NOATTR SCR1_DRAW_TILE_NOATTR
#define SCR3_DRAW_TILE SCR1_DRAW_TILE
#define SCR3_DRAW_TILE_NOATTR SCR1_DRAW_TILE_NOATTR

#define SCR1_CURSOR_CPS(gfx_p, abs)  \
(gfx_p) = gemu.Tilemap_Scroll1[(abs - 0x90c000)/4];

//			d	 6    7    0   
//    00 y000 0000 0yyy yyAA
//        xxx xxxx x000 00AA

#define SCR1_CURSOR_SET(gfx_p, x, y) \
(gfx_p) = gemu.Tilemap_Scroll1[ (x << 5) + ((y/4) & 0x1f) ];

#define SCR2_CURSOR_SET(gfx_p, x, y) \
gfx_p = gemu.Tilemap_Scroll2[((y & 0x30)<<6)+(x*16+(y & 0xf)) ];
#define SCR3_CURSOR_SET(gfx_p, x, y) \
gfx_p = gemu.Tilemap_Scroll3[ ((y & 0x38)<<6) + ((x & 0x3f) << 3) + (y & 7) ];

#define SCR1_CURSOR_BUMP(gfx_p, x, y) \
gfx_p += y * 2;						  \
gfx_p += x * 32 * 2;                  \

#define SCR2_CURSOR_BUMP(gfx_p, x, y)	\
gfx_p += y * 2;						\
gfx_p += x * 16 * 2;					

#define SCR3_CURSOR_BUMP(gfx_p, x, y) \
gfx_p += ((y) * 2);               \
gfx_p += x * 8 * 2;				  


#pragma mark Object
// OBJECT, sprites are in X, Y, tile, attr format, each u16


//#define OBJ_CURSOR_SET(gfx_p, id) \
//gfx_p = gemuObjCursorSet(id);


#ifdef CPS
#define OBJ_CURSOR_SET(gfx_p, slot)                    \
    gfx_p = (void *)(0x910000 + (slot) * 8);
#else
#define OBJ_CURSOR_SET(gfx_p, id)                       \
    gfx_p = gemuObjCursorSet(id);
#endif


#ifdef CPS
#define OBJ_CURSOR_CPS(gfx_p, cpsgfx_p) \
gfx_p = cpsgfx_p;
#else
#define OBJ_CURSOR_CPS(gfx_p, cpsgfx_p) \
gfx_p = gemu.Tilemap_Object[(cpsgfx_p - 0x910000) / 8];
#endif

#define OBJ_CURSOR_BUMP(gfx_p) \
gfx_p += 4;

#define CPS_OBJ(addr) \
gemuObjCursorSet((addr - 0x910000)/8);

#define BMAP_SCROLL1 (u16 *)gemu.Tilemap_Scroll1
#define BMAP_SCROLL2 (u16 *)gemu.Tilemap_Scroll2
#define BMAP_SCROLL3 (u16 *)gemu.Tilemap_Scroll3



/* CPS I/O Constants */
#ifdef CPS

#define OBJECT_BASE		0x910000
#define OBJECT2_BASE	0x918000
#define PALETTE_BASE	0x900000
#define SCROLL2_BASE	0x904000
#define SCROLL1_BASE	0x90c000
#define SCROLL3_BASE	0x908000  
#define ROWSCROLL_BASE  0x920000

#define CPS_VIDEO_SCROLL3   0x908000
#define CPS_VIDEO_SCROLL2   0x904000
#define CPS_VIDEO_SCROLL1   0x90c000

#endif	/* CPS */

/* Graphics tile offsets */
#define GFXROM_OBJECT   0x0
#define GFXROM_SCROLL1  0x4000
#define GFXROM_SCROLL2  0x2800
#define GFXROM_SCROLL3  0x0400

#define ATTR_NO_FLIP     0x0
#define ATTR_X_FLIP     0x20
#define ATTR_Y_FLIP     0x40
#define ATTR_XY_FLIP    (ATTR_X_FLIP | ATTR_Y_FLIP)


#define CPS1_SCROLL_SIZE    0x4000	/* scroll1, scroll2, scroll3 */
#define CPS1_OBJ_SIZE       0x0800
#define CPS1_OTHER_SIZE     0x1000
#define CPS1_PALETTE_ALIGN  0x0800		//???!


#ifndef CPS

#define GEMU_CLEAR_OBJECT_72	gemu_clear_object_first72

#define GEMU_OBJECT_DRAW		gemuObjectDraw
#define GEMU_OBJECT_DRAW_NOATTR	gemuObjectDrawNoAttr

#endif	/* ifdef CPS */

void gemuObjectDrawPoint(u16 *slot, u32 point, u16 tile, u16 attr);
void gemuObjectDraw(u16 *slot, u16 x, u16 y, u16 tile, u16 attr);
void gemuObjectDrawNoAttr(u16 *slot, u16 x, u16 y, u16 tile);
void gemuObjectDrawID(short slot, u16 x, u16 y, u16 tile, u16 attr);
void gemuObjectDrawIDCoords(short id, u16 x, u16 y, u16 tile, u16 attr);

void gemuIncGfxCursor(u32 *gfxcursor, short x, short y);
void gemuCoordOffset(u32 *gfxcursor, short x, short y);
void gemuWriteEndTag(u16 *slot);
u16 *gemuObjCursorSet(short id);

void FBSetPalette(short major, const u16 *palette);
//void gemu_setpalette(short major, const u16 *palette);
void palette_base_1k(void);
void scroll1_base_1k(short stage);
void scroll2_base_1k(short stage);
void scroll3_base_1k(short stage);

void gemu_clear_object(void);
void gemu_clear_object_first72(void);



void gemu_draw_scroll1(void);
void gemu_draw_scroll2(void);
void gemu_draw_scroll3(void);


void gemu_draw_object(void);

#define GEMU_CLEAR_OBJECT_D		gemu_clear_object()
#define GEMU_CLEAR_OBJECT		gemu_clear_object()


typedef u16 GPAL;

typedef struct {
    // Palette entries 
	 
	/*
	 CPS stores packed nibbles in U16s, we use arrays of chars, which we later convert 
	 up to an 8-bit value by multiplying by 17  
	 */
	
	GPAL	PalObject[32][16];							// CPS 0x900000 
	GPAL	PalScroll1[32][16];							// CPS 0x900400
	GPAL	PalScroll2[32][16];							// CPS 0x900800
	GPAL	PalScroll3[32][16];							// CPS 0x900c00
	GPAL	PalUnk1[32][16];							// CPS 0x901000
	GPAL	PalUnk2[32][16];							// CPS 0x901400
								// 901800-903fff unknown
	/* Scrollable tilemaps */
    u16     Tilemap_Scroll2[CPS1_OTHER_SIZE][2];				/* CPS 0x904000 - 0x907fff */
    u16     Tilemap_Scroll3[CPS1_OTHER_SIZE][2];				/* CPS 0x908000 - 0x90bfff */
    u16     Tilemap_Scroll1[CPS1_OTHER_SIZE][2];				/* CPS 0x90c000 - 0x90ffff */
	
    /* A double-buffered "Object" or sprite area */
																/* CPS 0x910000 - 0x91ffff */
    u16     Tilemap_Object[CPS1_OBJ_SIZE * 4][4];   /* TWO playfields.*/
	/* that's 0x1000 or 4096d entries per playfield */
	
	// RowScroll 0x92 0000, 0x92 1000, 
	short	RowScroll1[2048];
	short	RowScroll2[2048];
	
    /* here on is non-CPS, only memory we use for emulating the tile to bitmap conversion */
    /* the tile to texture memory, emulating the CPS tile lookup */
    /* arrays of RGBA data fed into glTex2d                      */
    unsigned char tile[32][32];     /* temporary buffer for converting tiles */
    char    FadeEnable;             /* whether or not to apply fading to palette lookups,
									 otherwise full-brightness                        */
    
    /* cache of RGBA values */
	
	u16 Scroll1X, Scroll2X, Scroll3X, Scroll1Y, Scroll2Y, Scroll3Y;
} CPSGFXEMU;

#endif
