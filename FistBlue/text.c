/*
 *  text.c
 *  MT2
 *
 *  Created by Ben on 13/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */
#include "sf2types.h"
#include "sf2macros.h"
#include "sf2const.h"

#include "structs.h"

#include "lib.h"
#include "task.h"
#include "rules.h"


#include "gfxlib.h"
#include "gemu.h"
#include "sprite.h"
#include "sm.h"
#include "sound.h"
#include "text.h"
#include "effects.h"
#include "sf2io.h"



#include "textdata.c"

extern Game g;
extern CPSGFXEMU gemu;

// print a player's score
void sub_54bc(u16 **gfx_p, short x, short y, u8 *string) {		// 54bc
	u32 cp = MakePointObj(x, y);
	short leadingzero = FALSE;
	if (*((u32 *)string) == 0) {
		INC_GFX_CURSOR(&cp, 0x48, 0);
		leadingzero = TRUE;
		sub_516a(gfx_p, &cp, 0, &leadingzero, 13);	/* XXX really not sure where d3 (13) comes from */
	} else {
#ifdef SF2_ENDIAN_LITTLE
	 	sub_516a(gfx_p, &cp, string[3], &leadingzero, 13);
		sub_5162(gfx_p, &cp, string[2], &leadingzero, 13);
		sub_5162(gfx_p, &cp, string[1], &leadingzero, 13);
		sub_5162(gfx_p, &cp, string[0], &leadingzero, 13);
#else
	 	sub_516a(gfx_p, &cp, string[0], &leadingzero, 13);
		sub_5162(gfx_p, &cp, string[1], &leadingzero, 13);
		sub_5162(gfx_p, &cp, string[2], &leadingzero, 13);
		sub_5162(gfx_p, &cp, string[3], &leadingzero, 13);
#endif		
	}
}

void showtextbank4(u8 d0)  {		//58c0 showtextbank4
	u16			*gfx_p;
	const u16	*data;
	const u8	*data2;
	u32			cp;
	short		palette;
	
	short x2,y2;
	u16 ch, format;
	const short *offsets;
	short count;
	
	if(d0 & 0x80) {
		//593a 
		data = data_8e2ac[d0 & 0x7f];
		
		
		OBJ_CURSOR_CPS(gfx_p, 0x910000 + *data++);
		data ++;
		count = *data;
		for(--count; count >= 0; count--) {
			OBJECT_DRAW(gfx_p, 0, 0, 0, 0);
		}
	} else {
		data = data_8e2ac[d0];
		OBJ_CURSOR_CPS(gfx_p, 0x910000 + data[0]);
		cp = MakePointObj(data[1], data[2]);
		count = data[3];
		--count;
		palette = data[4];
		format = data[5];
		offsets = sub_7f224(format);
		data2 += 6;
		for (;count >= 0; count--) {
			ch = data2[0];
			if (ch == 0) {
				offsets += 2;
			} else {
				x2 = *offsets++ + (cp >> 16);
				y2 = *offsets++ + (cp & 0xffff);
				OBJECT_DRAW(gfx_p, x2, y2, ch, palette);
			}
		}
		
	}
	
}

void sub_5982(Task *task) {		// 5982 in scroll1
	u8			ch;	
	u16			*gfx_p;
	const u8	*data;
	short		palette;
	
	if (task->params.Param0 & 0x80) {
		data = data_8d2ac[(task->params.Param0 & 0x7f)];
		data += 2;		// skip the object offset
		
		SCR1_CURSOR_SET(gfx_p, data[0], data[1]);
		palette = data[2];
		data += 3;
		while (TRUE) {		/* we return out */
			ch = data[0];
			if (ch == 0) {		// 59de
				if (task->params.Param1) {
					g.TextEffectBusy = FALSE;
				}
				if (task->params.x0014 == 0) {
					DIEFREE;
					return;
				}
				return;
			} else if (ch == 0x2f) {
				++data;
				SCR1_CURSOR_SET(gfx_p, data[0], data[1]);
				palette = data[2];
				data += 3;
			} else {
				SCR1_DRAW_TILE(gfx_p, GFXROM_SCROLL1 + 0x20, palette);
				SCR1_CURSOR_BUMP(gfx_p, 1, 0);
				data++;
				
				if (task->params.Param2) {
					SETSLEEP(task->params.Param2);
				}
			}
		}
	} else {
		data = data_8d2ac[(task->params.Param0 & 0xff)];
		data += 2;
		
		SCR1_CURSOR_SET(gfx_p, data[0], data[1]);
		palette = data[2];
		data += 3;
		while (TRUE) {		/* we return out */
			ch = data[0];
			if (ch == 0) {		// 59de
				if (task->params.Param1) {
					g.TextEffectBusy = FALSE;
				}
				if (task->params.x0014 == 0) {
					DIEFREE;
				}
				return;
			} else if (ch == 0x2f) {
				++data;
				SCR1_CURSOR_SET(gfx_p, data[0], data[1]);
				palette = data[2];
				data += 3;
			} else {
				SCR1_DRAW_TILE(gfx_p, GFXROM_SCROLL1 + ch, palette);
				SCR1_CURSOR_BUMP(gfx_p, 1, 0);
				
				data++;
				if (task->params.Param2) {
					SETSLEEP(task->params.Param2);
				}
			}
		}
	}
}

void showtextbank0(u8 sel) {		// 5602 Scroll1
	const u8 *data = data_8d2ac[sel & 0x7f];
	u16 *gfx_p;
	u8 cx,cy;
	data += 2;
	
	cx = *data++;
	cy = *data++;
	SCR1_CURSOR_SET(gfx_p, cx, cy);
	
	const u8 *string = data;
	char ch;
	short x,y,attr;
	
	if (sel & 0x80) {
		// 564e
		string += 3;
		while (1) {
			ch = *string++;
			if (ch == 0) {
				return;
			} else if (ch == SF2_TEXTLIB_EOL) {
				string += 3;
			} else if (ch != ' ') {
				SCR1_DRAW_TILE(gfx_p, 0, 0);
				SCR1_CURSOR_BUMP(gfx_p, 1, 0);
			}
		}
	} else {  // 5606
		attr = *string++;
		while(1) {	// 56c0
			ch = *string++;
			if (ch == 0) {
				return;
			} else if (ch == SF2_TEXTLIB_EOL) {
				SCR1_CURSOR_SET(gfx_p, x, y);
				attr = *string++;
			} else {
				if (ch != ' ') {	/* whitespace */
					SCR1_DRAW_TILE(gfx_p, ch + SF2_TILE_SC1_ASCII, attr);
					SCR1_CURSOR_BUMP(gfx_p, 1, 0);
				}
			}
		}
	}
}
void showtextbank1(u8 sel) {		// 568c draw text in OBJECT
	const u8 *data = data_8d2ac[sel & 0x7f];
	
	u16 *gfx_p;
	
	OBJ_CURSOR_SET(gfx_p, ((data[0] << 8) + data[1]) / 8);
	data += 2;
	u8 *string = (u8 *)data;
	char ch;
	short x,y,attr;
	
	if (sel & 0x80) {
		// 56fe
		string += 3;
		while (1) {
			ch = *string++;
			if (ch == 0) {
				return;
			} else if (ch == SF2_TEXTLIB_EOL) {
				string += 3;
			} else if (ch != ' ') {
				OBJECT_DRAW(gfx_p, 0, 0, 0, 0);
				OBJ_CURSOR_BUMP(gfx_p);
			}
		}
	} else {
		x = ((*string++) * 8) + 0x40;
		y = (*string++) * 2;
		attr = *string++;
		while(1) {	// 56c0
			ch = *string++;
			if (ch == 0) {
				return;
			} else if (ch == SF2_TEXTLIB_EOL) {
				x = ((*string++) * 8) + 0x40;
				y = (*string++) * 2;
				attr = *string++;
			} else {
				if (ch != ' ') {	/* whitespace */
					OBJECT_DRAW(gfx_p, x, y, ch + SF2_TILE_OBJ_2ASCII, attr);
					/* draw in two buffers */
					OBJ_CURSOR_BUMP(gfx_p);
				}
				x+=8;
			}
		}
	}
}

void showtextbank2(u8 sel) {		// 574a Winners chants
	const char *string;
	u16 *gfx_p;
	if (sel & 0x80) {
		// 57ca
		//string = data_8dbc4[sel & 0x7f];
	} else {
		//string = data_8dbc4[sel];
		OBJ_CURSOR_CPS(gfx_p, 0x910000 + string[0]);
		// todo...
		
	}
}


