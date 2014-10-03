/*
 *  gemu.c
 *  GLUTBasics
 *
 *  Created by Ben on 2/01/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include <stdio.h>

#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"

#include "gstate.h"
#include "structs.h"

#include "gemu.h"

CPSGFXEMU gemu;
extern Game g;

int gemu_scroll_enable[4];

struct inputs gInputs;

extern PALETTEWORD data_8a8ac[32][16];
extern PALETTEWORD data_c5000[20][32][16];
extern PALETTEWORD data_ca000[20][32][16];
extern PALETTEWORD data_c0000[20][32][16];

#ifndef CPS
void gemu_clear_object(void) {			// 5fbc CPS actually takes u16* param
	printf("*** Objects cleared ***");
	int i;
	for(i=0; i<256; i++) {
		OBJECT_DRAW_ID(i, 0, 0, 0, 0);
	}
}
void gemu_clear_object_first72(void) {		// 5f84
	int i;
	g.x8a5e = 1;		/* u8 */
	
	for(i=0; i<72; i++) {
		OBJECT_DRAW_ID(i, 0, 0, 0, 0);
		// second buffer
	}
}
#endif /* ifdef CPS */


void gemu_setpalette(short major, const u16 *palette) {
	short i;
	for (i = 0; i < 16; i++) {
		gemu.PalObject[major][i] = palette[i];
	}
}

void palette_base_1k(void) {		// move me back to gfxlib
    int u, v;
    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
            gemu.PalObject[u][v] = data_8a8ac[u][v] ; 
        }
    }
}

void scroll1_base_1k(short stage) {
    int u, v;
    printf ("Setting palette stage %d\n",stage);
    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
			gemu.PalScroll1[u][v] = data_c0000[stage][u][v];
        }
    }
}
void scroll2_base_1k(short stage) {
    int u, v;
    printf ("Setting palette stage %d\n",stage);
    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
            /* convert 4-bit RGBM to 8-bit RGBM */
            gemu.PalScroll2[u][v] = data_c5000[stage][u][v];
        }
    }
}
void scroll3_base_1k(short stage) {
    int u, v;
    printf ("Setting palette stage %d\n",stage);
    for(u=0; u<32; u++) {
        for(v=0; v<16; v++) {
            /* convert 4-bit RGBM to 8-bit RGBM */
            gemu.PalScroll3[u][v] = data_ca000[stage][u][v];
        }
    }
}

inline void gemuObjectDraw(u16 *slot, u16 x, u16 y, u16 tile, u16 attr) {
	*(slot+0)=x;                   
	*(slot+1)=y;                   
	*(slot+2)=tile;                
	*(slot+3)=attr;		
}
inline void gemuObjectDrawNoAttr(u16 *slot, u16 x, u16 y, u16 tile) {
	*(slot+0)=x;                   
	*(slot+1)=y;                   
	*(slot+2)=tile;                
}

inline void gemuObjectDrawPoint(u16 *slot, u32 point, u16 tile, u16 attr) {
	*(slot+0)=point & 0xffff;                   
	*(slot+1)=point >> 16;                   
	*(slot+2)=tile;                
	*(slot+3)=attr;		
}

inline void gemuObjectDrawID(short id, u16 x, u16 y, u16 tile, u16 attr) {
	gemu.Tilemap_Object[id][0]=x;
	gemu.Tilemap_Object[id][1]=y;
	gemu.Tilemap_Object[id][2]=tile;
	gemu.Tilemap_Object[id][3]=attr;
}
inline void gemuObjectDrawIDCoords(short id, u16 x, u16 y, u16 tile, u16 attr) {
	x += 0x40;
	y ^= 0xff;
	++y;
	gemu.Tilemap_Object[id][0]=x;
	gemu.Tilemap_Object[id][1]=y;
	gemu.Tilemap_Object[id][2]=tile;
	gemu.Tilemap_Object[id][3]=attr;
	
}
inline void gemuIncGfxCursor(u32 *gfxcursor, short x, short y) {
	*gfxcursor += (x << 16) + y;
}
inline void gemuWriteEndTag(u16 *slot) {
	*(slot+0)=0x0000;
	*(slot+1)=0x0100;
	*(slot+2)=0x0000;
	*(slot+3)=0xff00;
}

inline u16 *gemuObjCursorSet(short id){
	return gemu.Tilemap_Object[id];
}
