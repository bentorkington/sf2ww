/*
 *  cpstests.c
 *  MT2
 *
 *  Created by Ben on 25/08/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include "cpstests.h"
#include "sf2const.h"
#include "sf2types.h"
#include "sf2macros.h"
#include "gemu.h"
#include "sf2io.h"
#include "structs.h"

#include "testlib.h"
#include	"lib.h"
#include	"gfxlib.h"
extern CPSGFXEMU gemu;

extern Game g;

const char onlyitem[] =  "\0\0\0\0Can't read here";		//todo

const char tm_text_test_menu[] =
	"\x08\x14\x04" "TEST MENU"     "\x2f"
	"\x07\x20\x05" "1. COLOR"      "\x2f"
	"\x07\x2c\x05" "2. DIP SWITCH" "\x2f";

const char *data_86b00[]={
	onlyitem,
};

const char data_865e0[2][4]={"OFF\x20", "ON\x20\x20"};


void draw_testmenu(int);

void sub_86114(u16 **scrp, u16 **scrp_min, u16 **scrp_max) {
	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + ' ', 0);
	SCR1_CURSOR_BUMP(scrp, 0, 3);
	if (*scrp > *scrp_max) {
		*scrp = *scrp_min;
	}
	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + 'A', 0);
	g.x8b16 += 2;
	if (g.x8b16 > 8) {
		g.x8b16 = 0;
	}
}
void sub_86142(u16 **scrp, u16 **scrp_min, u16 **scrp_max) {
	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + ' ', 0);
	SCR1_CURSOR_BUMP(scrp, 0, -3);
	if (*scrp < *scrp_min) {
		*scrp = *scrp_max;
	}
	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + 'A', 0);
	g.x8b16 -= 2;
	if (g.x8b16 < 0) {
		g.x8b16 = 8;
	}
}

void sub_8616e(u16 **scrp, u16 **scrp_min, u16 **scrp_max, int *mode, int mode_max) {
	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + ' ', 0);
	SCR1_CURSOR_BUMP(scrp, 0, 3);
	if (*scrp > *scrp_max) {
		*scrp = *scrp_min;
	}
	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + 'A', 0);
	*mode += 2;
	if (*mode > mode_max) {
		g.x8b16 = 0;
	}
}
void sub_86192(u16 **scrp, u16 **scrp_min, u16 **scrp_max, int *mode, int mode_max) {
	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + ' ', 0);
	SCR1_CURSOR_BUMP(scrp, 0, -3);
	if (*scrp < *scrp_min) {
		*scrp = *scrp_max;
	}
	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + 'A', 0);
	*mode -= 2;
	if (*mode < 0) {
		*mode = mode_max;
	}
}
void sub_85514(void) { // 85514 dot cloth
	int i, j;
	u16 *scrp, *scrp_a1;
	switch (g.mode2) {
		case 0:
			test_dimscreenout();
			break;
		case 2:
			SCR2_CURSOR_SET(scrp, 0, 1);
			for (i=23; i >= 0; --i) {
				scrp_a1 = scrp;
				for (j=13; j >= 0; --j) {
					SCR2_DRAW_TILE(scrp_a1, GFXROM_SCROLL2 + 4, 0);
					SCR2_CURSOR_BUMP(scrp_a1, 0, 1);
				}
				SCR2_CURSOR_BUMP(scrp, 1, 0);
			}
			NEXT(g.mode2);
			draw_testmenu(6);
			test_fadescreenin();
			break;
		case 4:		// 85566
			//sub_86674(); Empty subroutine
			if((g.ContrP1.part.p1 & !g.ContrP1DB.part.p1 ) & 0x20) {
				g.mode0 = 0;
			}
			break;
		FATALDEFAULT;
	}
}

void _test_stage_update(u16 *scrp, int arg) {	// 85702
	SCR1_DRAW_TILE(scrp, GFXROM_SCROLL1 + (arg >> 4), 0);
	SCR1_CURSOR_BUMP(scrp, 1, 0);
	SCR1_DRAW_TILE(scrp, GFXROM_SCROLL1 + (arg & 0xf), 0);
	// may return new stage in %d4
}



void sub_856e6(int arg) {
	u16 *scrp;
	SCR1_CURSOR_CPS(scrp, 0x90c514);
	_test_stage_update(scrp, ++arg);
}

void sub_856f6(int arg) {
	u16 *scrp;
	SCR1_CURSOR_CPS(scrp, 0x90ca10);
	_test_stage_update(scrp, ++arg);
}



void sub_858d8(void) {
	u16 *scrp;
	int d0;
	
	switch (g.mode6) {
		case 0:
			test_dimscreenout();
			draw_testmenu(9);
			sub_6040();
			sub_856e6(g.x8b24);
			sub_1742(g.x8b24);
			sub_862a8(   );
			sub_8630e(   );
			sub_86570(   );
			SCR1_CURSOR_CPS(scrp, 0x90cf0c);
			test_putbool(&scrp, (g.x8b2e & ATTR_Y_FLIP) / ATTR_Y_FLIP);
			SCR1_CURSOR_CPS(scrp, 0x90d48c);
			test_putbool(&scrp, (g.x8b2e & ATTR_X_FLIP) / ATTR_X_FLIP);
			sub_864ea(   );
			NEXT(g.mode6);
			test_fadescreenin();
			break;
		case 2:
			d0 = (g.RawButtons0Dash & (!g.RawButtons0));
			
			break;
		default:
			break;
	}
	
}


void test_char_object() {		// 85636
	switch (g.mode4) {
		case 0:
			NEXT(g.mode4);
			g.mode5 = 0;
			g.mode6 = 0;
			test_dimscreenout();
			g.x8b2c = 0;
			g.x8b2e = 0;
			break;
		case 2:
			switch (g.mode5) {
				case 0:
					sub_85674();
					break;
				case 2:
					sub_858d8();
					FATALDEFAULT;
			}
			FATALDEFAULT;
	}
}


void sub_855b0(u16 **scrp, u16 **scrp_min, u16 **scrp_max, int *mode, int *mode_max) {
	int d0;
	switch (g.mode2) {
		case 0:
			test_dimscreenout();
			g.CurrentStage = 0;
			palettes_nextlevel();
			setpalette_scroll1(16);
			draw_testmenu(7);
			g.mode3 = 0;
			NEXT(g.mode2);
			SCR1_CURSOR_SET(*scrp, 0, 0);
			*scrp_min = *scrp;
			SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + 'A', 0);
			SCR1_CURSOR_SET(*scrp_max, 0, 0);
			*mode     = 0;
			*mode_max = 6;
			break;
		case 2:
			d0 = g.ContrP1.part.p1 & !g.ContrP1DB.part.p1;
			if (d0 & 0x10) {
				NEXT(g.mode2);
				g.mode3 = *mode;
				g.mode4 = 0;
				g.x8b24 = g.x8b26 = g.x8b28 = g.x8b2a = 0;
			} else if (d0 & 0x04) {
				sub_8616e(scrp, scrp_min, scrp_max, mode, *mode_max);
			} else if (d0 & 0x08) {
				sub_86192(scrp, scrp_min, scrp_max, mode, *mode_max);
			} else if (d0 & 0x20) {
				g.mode0 = 0;
			}
			break;
		case 4:
			// in submenu, 85622();
			switch (g.mode3) {
				case 0: test_char_object();  break;
				case 2: test_char_scroll1(); break;
				case 4: test_char_scroll2(); break;
				case 6: test_char_scroll3(); break;
				FATALDEFAULT;
			}
		default:
			break;
	}
}	




void sub_84d10(u16 **scrp) {
	test_dimscreenout();
	draw_testmenu(0);
	NEXT(g.mode0);
	g.mode1 = g.mode2 = g.mode3 = g.mode4 = 0;

	SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + 'A', 0);
	test_fadescreenin();
}
	

void test_menu_select_input(u16 **scrp, u16 **scrp_min, u16 **scrp_max) {	// 84d48
	int d0 = g.ContrP1.part.p1 & !g.ContrP1DB.part.p1;
	if (d0 & 0x10) {
		g.ts_scr1 = *scrp_min;
		g.ts_scr2 = *scrp_max;
		g.ts_scr3 = *scrp;
		g.mode1 = g.x8b16;
		NEXT(g.mode0);
	} else if (d0 & JOY_DOWN) {	
		sub_86114(scrp, scrp_min, scrp_max);
	} else if (d0 & JOY_UP) {
		sub_86142(scrp, scrp_min, scrp_max);
	}
}

void draw_testmenu(int sel) {		// 86a76
	const char *string;
	u16 *scrp;
	u16 attr;
	
	if (sel & 0x80) {
		// erase 86abe
		string = data_86b00[sel];
		
DRAW_TESTMENU_REPOS_ERASE:
		SCR1_CURSOR_SET(scrp, string[0], string[1]);
		attr = string[2];
		string += 3;
		
		while (TRUE) {				// gotos and returns, punk.
			if (string[1] == 0x2f) {
				string += 1;
				goto DRAW_TESTMENU_REPOS_ERASE;
			} else if (string[1] == 0) {
				return;
			} else {
				SCR1_DRAW_TILE(scrp, GFXROM_SCROLL1 + 0x20, attr);	// draw a space
				string += 1;
				SCR1_CURSOR_BUMP(scrp, 1, 0);
			}
		}
	} else {
		// draw 86a7a
		string = data_86b00[sel];
		
DRAW_TESTMENU_REPOS:
		SCR1_CURSOR_SET(scrp, string[0], string[1]);
		attr = string[2];
		string += 3;
		
		while (TRUE) {				// gotos and returns, punk.
			if (string[1] == 0x2f) {
				string += 1;
				goto DRAW_TESTMENU_REPOS;
			} else if (string[1] == 0) {
				return;
			} else {
				SCR1_DRAW_TILE(scrp, string[1] + GFXROM_SCROLL1, attr);
				string += 1;
				SCR1_CURSOR_BUMP(scrp, 1, 0);
			}
		}
	}
}

void test_puts(u16 **scrp, char *string, int attr, int length) { //86610
	for (; length >= 0; --length) {
		SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + *string, attr);
		++string;
		SCR1_CURSOR_BUMP(*scrp, 1, 0);
	}
}

void test_putblank(u16 **scrp_a, u16 **scrp_b, int length) {		// 86628
	for (; length >= 0; --length) {
		SCR1_DRAW_TILE_NOATTR(*scrp_a, GFXROM_SCROLL1);
		SCR1_DRAW_TILE_NOATTR(*scrp_b, GFXROM_SCROLL1);
		SCR1_CURSOR_BUMP(*scrp_a, 1, 0);
		SCR1_CURSOR_BUMP(*scrp_b, 1, 0);
	}
}

void test_putbool(u16 **scrp, int value) {
	int i;
	for (i=0; i<4; ++i) {
		SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + data_865e0[value][i], 0);
		SCR1_CURSOR_BUMP(*scrp, 1, 0);
	}
}

void test_putboolvert(u16 **scrp, int value) {
	int i;
	for (i=0; i<4; ++i) {
		SCR1_DRAW_TILE(*scrp, GFXROM_SCROLL1 + data_865e0[value][i], 0);
		SCR1_CURSOR_BUMP(*scrp, 0, 1);
	}
}


int test_is_flipped_y(void) {		// 861fe
	return (g.x8b2e & ATTR_Y_FLIP) / ATTR_Y_FLIP;
}

int test_is_flipped_x(void) {		// 8620a
	return (g.x8b2e & ATTR_X_FLIP) / ATTR_X_FLIP;
}



void sub_86216(u16 *scrp, int d5) {
	int index = g.x8b2c;
	int d1 = g.mode3;
	int token, i;
	u16 *scrp_a1;
	
	for (; d5 >= 0; --d5) {
		token = index;
		scrp_a1 = scrp;
		for (i=0; i<4; ++i) {
			SCR1_DRAW_TILE(scrp_a1, GFXROM_SCROLL1 + (token & 0xf), 0);
			SCR1_CURSOR_BUMP(scrp_a1, -1, 0);
			token >>= 4;
		}
		SCR1_CURSOR_BUMP(scrp, 0, 4);
		index = (index + 0x10) & (const u16[]){0xffff, 0x0fff, 0x1fff, 0x7000}[d1];
	}
}

void sub_86266(u16 *scrp, int d5) {
}


