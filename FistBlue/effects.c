/* effects.c Ported Low level CPS routines */

#include <stdio.h>

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
#include "effects.h"
#include "sf2io.h"

#include "text.h"

#ifndef CPS
#include "gfx_glut.h"
#endif

extern struct executive_t Exec;


extern CPSGFXEMU gemu;
extern Game g;

struct effectstate es;


#define CPS_VIDEO_SCROLL1 (u16 *)&gemu.Tilemap_Scroll1
#define CPS_VIDEO_SCROLL2 (u16 *)&gemu.Tilemap_Scroll2
#define CPS_VIDEO_SCROLL3 (u16 *)&gemu.Tilemap_Scroll3
#define CPS_PALBASE_OBJECT (u16 *)&gemu.PalObject
#define CPS_PALBASE_SCROLL1 (u16 *)&gemu.PalScroll1
#define CPS_PALBASE_SCROLL2 (u16 *)&gemu.PalScroll2
#define CPS_PALBASE_SCROLL3 (u16 *)&gemu.PalScroll3

static void syslib_00 (void);
static void syslib_04(void);
static void syslib_08 (void);
static void syslib_0c (void);
static void syslib_10 (void);
static void syslib_18 (void);
static void syslib_1c (void);
static void syslib_20 (void);
static void syslib_26 (void);


void *data_155c[] = {
	syslib_00,
	syslib_04,	// 597a	copyright notices etc.
	syslib_08,	// 4f3a insert coin blinker
	syslib_0c,	// 4ade
	syslib_10,	//4f9e,
	NULL,		//5a3e,
	syslib_18,	//5b22	usa/etc winning chants,
	syslib_1c,	//5c12,
	syslib_20,	//5410, add to player score
	NULL,		//5ce2 jap winning chants,
	NULL,		//4a76 die and go to test menu?
	syslib_26,		//5e14
};

void clear_object(void) {      /* clear_object() actually 256 * 2 * longwords = 2048 */
	int i;
	for(i=0; i<256; i++) {
		gemu.Tilemap_Object[i][0]=0;
		gemu.Tilemap_Object[i][1]=0;
		gemu.Tilemap_Object[i][2]=0;
		gemu.Tilemap_Object[i][3]=0;
	}
#ifndef CPS
	gemu_set_cache_clear();
#endif
}

static void adder(u16 **palbase, int *a1, short arg, char d7) {		//11ea
	int i;
	short d0 = **palbase;
	if (arg >= 0) {
		d0 += arg;
	}
	if (d0 & 0xf000) {
		for (i=0; i<16; ++i) {
			*(*palbase+i) += arg;
		}	
	} else {
		// 120a
		if (arg < 0) {
			*a1 |= (1 << d7);
		} else {
			*a1 &= ~(1 << d7);
		}
	}
	*palbase += 16;
}
static void massadder(GPAL *palbase, int *a1, short arg) {
	for (short d7=0x1f; d7 >= 0; --d7) {
		adder(&palbase, a1, arg, d7);
	}
}
static void blackadder(GPAL *palbase, int *fadebase, int count, short arg) {		// 1142
    int i;
    for(i=count; i >= 0; --i) {
        adder(&palbase, fadebase, arg, i);
    }
    es.FadeCounter += *fadebase;
}
static void whiteadder(GPAL *palbase, int *fadebase, int count, short arg) {			//11c2
	int d0;
	
	for(int i=count; i >= 0; --i) {
        adder(&palbase, fadebase, arg, i);
    }

	d0 = *fadebase;
	if (arg < 0) {
		d0 |= ( 1 << 31);
	} else {
		d0 &= ~(1 << 31);
	}
	es.FadeCounter += d0;
}
	

void _clear_scr23_wait_die(Task *task) {		// 4bd6
	gfxrepeat((u16 *)CPS_VIDEO_SCROLL2, 0x1000, GFXROM_SCROLL2      , 0);  /* first tile is blank */
    gfxrepeat((u16 *)CPS_VIDEO_SCROLL3, 0x1000, GFXROM_SCROLL3      , 0);        
    g.x02b8 = 0;
    g.x02ba = 0;
    clear_object();
	// todo second buffer
    sf2sleep(1);
    clear_rowscroll();
    sf2sleep(4 + task->params.Param2);
    es.FadeBusy = FALSE;
    DIEFREE;
}

void sub_1030(int arg) {        /* fade all layers */
    es.FadeCounter = 0;
    blackadder(gemu.PalObject[0],  &es.FadeObject,  0x1f, arg);
    blackadder(gemu.PalScroll1[0], &es.FadeScroll1, 0x1f, arg);
    blackadder(gemu.PalScroll2[0], &es.FadeScroll2, 0x1f, arg);
    blackadder(gemu.PalScroll3[0], &es.FadeScroll3, 0x1f, arg);
    blackadder(gemu.PalUnk1[0]	 , &es.x5d4e,       0x7,  arg);       /* probably the "PRESS START" */
    blackadder(gemu.PalUnk2[0]   , &es.x5d52,       0x7,  arg);       /* blinkers                   */
}
void sub_1078 (Task *task) {        /* enable layers specified in task->params.Param1 */
    const u16 LAYERBITS[4]= { 0x0000, 0x0008, 0x0010, 0x0002 };
    g.CPS.DispEna |= LAYERBITS[ ((task->params.Param1 & 0xe0) >> 5) ];
}
void sub_140e (int arg) {
	es.FadeCounter = 0;
    blackadder(gemu.PalScroll3[0], &es.FadeScroll3, 0x1f, arg);
    blackadder(gemu.PalUnk1[0]   , &es.x5d4e,       0x7,  arg);
    blackadder(gemu.PalUnk2[0]   , &es.x5d52,       0x7,  arg);          
}
static void sub_10e0(short data) {
	es.FadeCounter = 0;
	blackadder(gemu.PalObject[0],  &es.FadeObject,  0x1f, data);
	blackadder(gemu.PalScroll1[0], &es.FadeScroll1, 0x1f, data);
	blackadder(gemu.PalScroll2[0], &es.FadeScroll2, 0x1f, data);
	blackadder(gemu.PalScroll3[0], &es.FadeScroll3, 0x1f, data);
	blackadder(gemu.PalUnk1[0],    &es.x5d4e,          7, data);
	blackadder(gemu.PalUnk2[0],    &es.x5d52,          7, data);
}
static void sub_1152(short data) {	// same as 10e0, but doesn't do Object0
	es.FadeCounter = 0;
	whiteadder(gemu.PalObject[1],  &es.FadeObject,  0x1e, data);
	blackadder(gemu.PalScroll1[0], &es.FadeScroll1, 0x1f, data);
	blackadder(gemu.PalScroll2[0], &es.FadeScroll2, 0x1f, data);
	blackadder(gemu.PalScroll3[0], &es.FadeScroll3, 0x1f, data);
	blackadder(gemu.PalUnk1[0], &es.x5d4e, 7, data);
	blackadder(gemu.PalUnk2[0], &es.x5d52, 7, data);
}

#pragma mark SYSLIB_00

void syslib_00 (void) {					// e12
    Task *task = &Exec.Tasks[Exec.CurrentTask];
    switch (task->params.Param0) {
			
		case LC0_DARK_ALL_DISABLE:                     /* Fade out and disable all layers */
			if(Exec.FadeOutComplete) {
				if((gemu.PalScroll3[0][0] & 0xf000) == 0x0000) {
					DIEFREE;
					return;
				}
				Exec.FadeOutComplete = FALSE;
			}
			es.FadeInEffect = TRUE;
			
			while (TRUE) {
				do {
					sub_10e0(-0x1000);
					TASKSLEEP;
				} while  (es.FadeCounter != 0x1fa);
				
				es.FadeInEffect        = FALSE;
				Exec.FadeOutComplete   = TRUE;
				g.CPS.DispEna       &= 0xffc0;	
				DIEFREE;
			}
			break;
		case LC0_LIGHT_ALL_ENABLE:                    /* Enable and fade up all layers */
			g.CPS.DispEna |= 0x1a;           
			if(Exec.FadeOutComplete == FALSE) {
				if(((gemu.PalScroll3[0][0]+0x1000) & 0xf000) == 0x0000) {
					DIEFREE;
					return;
				} 
				Exec.FadeOutComplete = TRUE;
			}
			es.FadeInEffect = TRUE;
			do {
				sub_10e0(0x1000);
				TASKSLEEP;
			} while (es.FadeCounter);
			es.FadeInEffect    = FALSE;
			Exec.FadeOutComplete = FALSE;
			//DIEFREE;
			break;
		case 4:
			/* dunno */  
			break;  
		case 6:
			sub_1078(task);         /*set layer enable according to task->params.Param1*/
			/* XXX whoa */
			printf("XXX unimplemented SL00_06");
			break;
			
			/***************************
			 * Individual layer FADE OUT 
			 ***************************/
			
		case 0xa:                       /* OBJECT */
			if (es.FadeObject == -1) {
				DIEFREE;
			} else {
				massadder(CPS_PALBASE_OBJECT, &es.FadeObject, -0x1000);
				TASKSLEEP;
			}
			break;  
		case 0xc:                       /* SCROLL1 */
			if (es.FadeScroll1 == -1) {
				DIEFREE;
			} else {
				massadder(CPS_PALBASE_SCROLL1, &es.FadeScroll1, -0x1000);
				TASKSLEEP;
			}
			break;
		case 0xe:                       /* SCROLL2 */
			if (es.FadeScroll2 == -1) {
				DIEFREE;
			} else {
				massadder(CPS_PALBASE_SCROLL2, &es.FadeScroll2, -0x1000);
				TASKSLEEP;
			}
			break;
		case 0x10:                      /* SCROLL3 */
			if (es.FadeScroll3 == -1) {
				DIEFREE;
			} else {
				massadder(CPS_PALBASE_SCROLL3, &es.FadeScroll3, -0x1000);
				TASKSLEEP;
			}
			break;
			
			
			/* FADE IN */
			
		case 0x12:                       /* OBJECT */
			if (es.FadeObject == -1) {
				DIEFREE;
			} else {
				massadder(CPS_PALBASE_OBJECT, &es.FadeObject, -0x1000);
				TASKSLEEP;
			}
			break;  
		case 0x14:                       /* SCROLL1 */
			if (es.FadeScroll1 == -1) {
				DIEFREE;
			} else {
				massadder(CPS_PALBASE_SCROLL1, &es.FadeScroll1, -0x1000);
				TASKSLEEP;
			}
			break;
		case 0x16:                       /* SCROLL2 */
			if (es.FadeScroll2 == -1) {
				DIEFREE;
			} else {
				massadder(CPS_PALBASE_SCROLL2, &es.FadeScroll2, -0x1000);
				TASKSLEEP;
			}
			break;
		case 0x18:                      /* SCROLL3 */
			while (es.FadeScroll3 != -1) {
				massadder(CPS_PALBASE_SCROLL3, &es.FadeScroll3, -0x1000);
				TASKSLEEP;
			}
			DIEFREE;
			break;
		case 0x1a:			// ef4
			if (Exec.FadeOutComplete) {
				if (gemu.PalScroll3[0][0] & 0xf000) {
					Exec.FadeOutComplete = FALSE;
				} else {
					DIEFREE;
				}
			}
			es.FadeInEffect = TRUE;
			do {
				sub_1152(-0x1000);
				TASKSLEEP;
			} while (es.FadeCounter != 0x1fa);
			es.FadeInEffect = FALSE;
			Exec.FadeOutComplete = TRUE;
			g.CPS.DispEna &= 0xffc0;
			DIEFREE;
			break;
            /* XXX some missing */
			
		case 0x1e:
			while(es.FadeScroll3 != 0xffffffff)  {
				massadder(CPS_PALBASE_SCROLL1, &es.FadeScroll1, -0x1000);
				massadder(CPS_PALBASE_SCROLL2, &es.FadeScroll2, -0x1000);
				massadder(CPS_PALBASE_SCROLL3, &es.FadeScroll3, -0x1000);
				TASKSLEEP;
			}
			DIEFREE;
			break;
		case 0x20:
			while(TRUE) {
				if(es.FadeScroll3 == 0xffffffff) { DIEFREE; }
				massadder(CPS_PALBASE_SCROLL1, &es.FadeScroll1, 0x1000);
				massadder(CPS_PALBASE_SCROLL2, &es.FadeScroll2, 0x1000);
				massadder(CPS_PALBASE_SCROLL3, &es.FadeScroll3, 0x1000);
				TASKSLEEP;
			}
			break; 
		case 0x22:
			do {
				sub_140e(-0x1000);
				TASKSLEEP;
			} while (task->params.Param1--);
			DIEFREE;
			break;
		case 0x24:
			while(TRUE) {
				if(es.FadeObject == 0) { DIEFREE; }
				massadder(CPS_PALBASE_OBJECT, &es.FadeObject, 0x1000);
				TASKSLEEP;
			}
			break;
		case 0x26:
			while(TRUE) {
				if(es.FadeObject == 0) { DIEFREE; }
				massadder(CPS_PALBASE_SCROLL2, &es.FadeScroll2, 0x1000);
				TASKSLEEP;
			}
			break;
		case 0x28:
			while(TRUE) {
				if(es.FadeObject == 0) { DIEFREE; }
				massadder(CPS_PALBASE_SCROLL3, &es.FadeScroll3, 0x1000);
				TASKSLEEP;
			}
			break;
		FATALDEFAULT;
    }
	DIEFREE;	
}        

static void sub_4cb2() {
	u16 *gfx_p;
	if (g.Player1.Alive) {
		OBJ_CURSOR_CPS(gfx_p, 0x910000);
		gfxrepeat(gfx_p, 48, 0, 0);
	}
	if (g.Player2.Alive) {
		OBJ_CURSOR_CPS(gfx_p, 0x910038);

		gfxrepeat(gfx_p, 48, 0, 0);
	}
	OBJ_CURSOR_CPS(gfx_p, 0x910070);
	gfxrepeat(gfx_p, 1928, 0, 0);
}

void syslib_0c (void) {
    Task *task = &Exec.Tasks[Exec.CurrentTask];

    switch (task->params.Param0) {
		case 0x1c:			/* fade out, clear all, wait */
			QueueEffect(LC0_DARK_DUNNO, task->params.Param2);
			SETSLEEP(1);
			SIG_WAIT(!Exec.FadeOutComplete);
			gfxrepeat(CPS_VIDEO_SCROLL1, 0x1000, GFXROM_SCROLL1 + ' ', 0);  /* a whitespace */
			gfxrepeat(CPS_VIDEO_SCROLL2, 0x1000, GFXROM_SCROLL2      , 0);  /* first tile is blank */
			gfxrepeat(CPS_VIDEO_SCROLL3, 0x1000, GFXROM_SCROLL3      , 0);
			g.x02b8 = 0;
			g.x02ba = 0;
			sub_4cb2();	/* also do other buffer */
			SETSLEEP(1);
			clear_rowscroll();
			SETSLEEP(4 + task->params.Param2);
			es.FadeBusy = FALSE;
			DIEFREE;
			break;  
		case 0:
			QueueEffect(LC0_DARK_ALL_DISABLE, task->params.Param2);
			SETSLEEP(1);
			SIG_WAIT(!Exec.FadeOutComplete);
			gfxrepeat(CPS_VIDEO_SCROLL1, 0x1000, GFXROM_SCROLL1 + ' ', 0);  /* a whitespace */
			_clear_scr23_wait_die(task);     /* dies */ 
			break;          
		case 2:
			g.x02b8 = 0;
			g.x02ba = 0;                        
			clear_object();
			/* clear other buffer */
			DIEFREE;
			break; 
		case 4:
			gfxrepeat(CPS_VIDEO_SCROLL1, 0x1000, GFXROM_SCROLL1 + ' ', 0);  /* a whitespace */
			DIEFREE;        
			break;
		case 6:
			gfxrepeat(CPS_VIDEO_SCROLL2, 0x1000, GFXROM_SCROLL2      , 0);  
			sf2sleep(1);
			clear_rowscroll();
			diefree();        
			break;
		case 8:
			gfxrepeat(CPS_VIDEO_SCROLL3, 0x1000, GFXROM_SCROLL3      , 0); 
			diefree();        
			break;  
		case 0xa:
			g.CPS.DispEna ^= 0x20;
			DIEFREE;
			break;
		case 0xc:
			g.CPS.DispEna ^= 0x04;
			DIEFREE;
			break;
		case 0xe:
			g.CPS.DispEna ^= 0x24;
			DIEFREE;
			break;
		case 0x10:
			QueueEffect(LC0_LIGHT_ALL_ENABLE, task->params.Param2);
			do {sf2sleep(1);} while (Exec.FadeOutComplete);
			sf2sleep(task->params.Param2);
			es.FadeBusy = FALSE;
			DIEBREAK;
		case 0x12:
			QueueEffect(LC0_DARK_OBJECT, task->params.Param2);    
			do {sf2sleep(1);} while (es.FadeObject != 0xffffffff);
			g.x02b8 = 0;
			g.x02ba = 0;
			clear_object();
			/* clear other buffer */
			DIEBREAK;
		case 0x14:
			QueueEffect(LC0_DARK_SCROLL1, task->params.Param2);    
			do {sf2sleep(1);} while (es.FadeScroll1 != 0xffffffff);    
			gfxrepeat(CPS_VIDEO_SCROLL1, 0x1000, GFXROM_SCROLL1 + ' ', 0);
			es.FadeBusy = FALSE;
			DIEBREAK;
		case 0x16:
			QueueEffect(LC0_DARK_123, task->params.Param2);    
			do {sf2sleep(1);} while (es.FadeScroll2 != 0xffffffff);    
			gfxrepeat(CPS_VIDEO_SCROLL2, 0x1000, GFXROM_SCROLL2 , 0);
			es.FadeBusy = FALSE;
			DIEBREAK; 
		case 0x18:
			QueueEffect(LC0_DARK_SCROLL3, task->params.Param2);    
			do {sf2sleep(1);} while (es.FadeScroll3 != 0xffffffff);    
			gfxrepeat(CPS_VIDEO_SCROLL3, 0x1000, GFXROM_SCROLL3 , 0);
			es.FadeBusy = FALSE;
			DIEBREAK;
		case 0x1a:
			QueueEffect(LC0_DARK_ALL_DISABLE, task->params.Param2);
			do {sf2sleep(1);} while (Exec.FadeOutComplete == 0);
			_clear_scr23_wait_die(task); /* dies */
			break;
		case 0x1e:
			QueueEffect(LC0_DARK_123, task->params.Param2);
			do {sf2sleep(1);} while (es.FadeScroll3 != 0xffffffff);  
			clear_scrolls_123(task);      /* dies */
			break;
		case 0x20:
			es.FadeObject = 0;
			es.FadeScroll2 = 0;
			es.FadeScroll3 = 0;
			
			start_effect(LC0_DARK_OBJECT,  task->params.Param2);
			start_effect(LC0_DARK_SCROLL2, task->params.Param2);
			start_effect(LC0_DARK_SCROLL3, task->params.Param2);
			
			do { sf2sleep(1); } while (es.FadeScroll3 != 0xffffffff);
			_clear_scr23_wait_die(task);
    }
}

static void sub_507a(u16 **gfx_p, u8 d0, short *d2, u16 d3) {		//507a
	if (*d2 == 0) {
		if (d0 & 0xf) {
			*d2 = 1;
		} else {
			SCR1_DRAW_TILE(*gfx_p, GFXROM_SCROLL1 + 0x20, d3);
			SCR1_CURSOR_BUMP(*gfx_p, 1, 0);
			return;
		}
	}
	
	SCR1_DRAW_TILE(*gfx_p, GFXROM_SCROLL1 + (d0 & 0xf),d3);
	SCR1_CURSOR_BUMP(*gfx_p, 0, 1);
}
static void sub_5072(u16 **gfx_p, short d0, short d2, u16 d3) {		//5072
	sub_507a(gfx_p, d0 >> 4, &d2, d3);
	sub_507a(gfx_p, d0,      &d2, d3);
}



static void syslib_04(void) {		// SL04	 597a version string
	Task *task = &Exec.Tasks[Exec.CurrentTask];
	task->params.x0014 = 0;
	sub_5982(task);
}


static void syslib_10(void) {		// 4f9e
	u16 *cur; int i;
	Task *task = &Exec.Tasks[Exec.CurrentTask];
	switch (task->params.Param0) {
		case 0:
			task->params.x0014 = 1;		//byte
			task->params.Param1 = 0;
			task->params.Param2 = 0;
			task->params.Param0 &= 0xff00;
			task->params.Param0 |= g.TwoCreditsToStart ? 8 : 7;
			sub_5982(task);
			SCR1_CURSOR_CPS(cur, 0x90d670);
			sub_5072(&cur, g.NumberCredits, 0, 0);
			DIEFREE;
			break;
		case 2:
			QueueEffect(0x180f, 0);
			for (i=4; i>=0; --i) {
				printlonghex2(&cur, 0x80, 0xc0 - (i * 32), g.HiScoreTable[i].score, 0);
			}
			for (i=4; i>=0; --i) {
				_putlong(&cur, 0x100, 0xc0 - (i * 32), g.HiScoreTable[i].name, 0);
			}
			DIEFREE;
			break;
		FATALDEFAULT;
	}
}


static void syslib_18(void) {		//5b22
	Task *task = &Exec.Tasks[Exec.CurrentTask];
	u16			*gfx_p;
	const u8	*data;
	u32			cp;
	short		palette;
	extern const u8 *data_8dbc4[];
	
	if (task->params.Param0 & 0x80) {
		//5bba todo erase
		data = data_8dbc4[task->params.Param0 & 0xff];
		task->params.x0014 = 0;
		OBJ_CURSOR_CPS(gfx_p, 0x910000 + (data[0] << 8) + data[1]);
		data += 7;		
SYSLIB18ERASE:
		if (data[0] == 0x2f) {
			data += 5;
		} else if (data[0] != 0x20) {
			OBJECT_DRAW(gfx_p, 0, 0, 0, 0);
		}
		data++;
		if (task->params.Param2) {
			SETSLEEP(task->params.Param2);
		} else {
			goto SYSLIB18ERASE;
		}
	} else {
		data = data_8dbc4[task->params.Param0 & 0xff];
		task->params.x0014 = 0;
		OBJ_CURSOR_CPS(gfx_p, 0x910000 + (data[0] << 8) + data[1]);
		
		cp = MakePointObj((data[2] << 8)+ data[3] , (data[4] << 8)+ data[5] );
		
		palette = data[6] & 0x1f;
		data += 7;
SYSLIB18LOOP:
		if (data[0] == 0) {
			if (task->params.Param1) {
				g.TextEffectBusy = FALSE;
			}
			DIEFREE;		//XXX
		} else if (data[0] == 0x2f) {
			cp = MakePointObj((data[1] << 8)+ data[2] , (data[3] << 8)+ data[4] );
			palette = data[5] & 0x1f;
			data += 6;
		} else {
			if (*(data) != 0x20) {
				OBJECT_DRAW((gfx_p), (cp >> 16), (cp & 0xffff), (data[0] + 0x8080), palette); 	
				OBJ_CURSOR_BUMP(gfx_p);
			}
			cp += 0xc0000;
		}
		data++;
		if (task->params.Param2) {
			SETSLEEP(task->params.Param2);
		} 
		goto SYSLIB18LOOP;
		
	}
}

static void syslib_1c(void) {		//5c12
	static const u16 data_8dfa8[12]={
		0x0000, 0x0008, 0x00f0, 0x0000, 0x8120, 0x8121, 0x8122, 0x8123, 
		0x8124, 0x8125, 0x8126, 0x0000,  };
	static const u16 data_8dfc0[12]={
		0x0038, 0x0100, 0x00f0, 0x0000, 0x8120, 0x8121, 0x8122, 0x8123, 
		0x8124, 0x8125, 0x8126, 0x0000,  };
	static const u16 data_8dfd8[12]={
		0x0000, 0x0008, 0x00f0, 0x0000, 0x8130, 0x8131, 0x8132, 0x8133, 
		0x8134, 0x8135, 0x8136, 0x0000,  };
	static const u16 data_8dff0[12]={
		0x0038, 0x0100, 0x00f0, 0x0000, 0x8130, 0x8131, 0x8132, 0x8133, 
		0x8134, 0x8135, 0x8136, 0x0000,  };
	static const u16 data_8e008[12]={
		0x0000, 0x0008, 0x00f0, 0x0000, 0x8138, 0x8139, 0x813a, 0x813b, 
		0x813c, 0x813d, 0x813e, 0x0000,  };
	static const u16 data_8e020[12]={
		0x0038, 0x0100, 0x00f0, 0x0000, 0x8138, 0x8139, 0x813a, 0x813b, 
		0x813c, 0x813d, 0x813e, 0x0000,  };
	static const u16 data_8e038[12]={
		0x0000, 0x0008, 0x00f0, 0x0000, 0x8110, 0x8111, 0x8112, 0x8113, 
		0x8114, 0x8115, 0x8116, 0x0000,  };
	static const u16 data_8e050[12]={
		0x0038, 0x0100, 0x00f0, 0x0000, 0x8110, 0x8111, 0x8112, 0x8113, 
		0x8114, 0x8115, 0x8116, 0x0000,  };
	static const u16 data_8e068[12]={
		0x0000, 0x0008, 0x00f0, 0x0000, 0x8118, 0x8119, 0x811a, 0x811b, 
		0x811c, 0x811d, 0x811e, 0x0000,  };
	static const u16 data_8e080[12]={
		0x0038, 0x0100, 0x00f0, 0x0000, 0x8118, 0x8119, 0x811a, 0x811b, 
		0x811c, 0x811d, 0x811e, 0x0000,  };
	static const u16 data_8e098[17]={
		0x0200, 0x00a0, 0x00a0, 0x000d, 0x8169, 0x816a, 0x816b, 0x816c, 
		0xffff, 0x00a0, 0x0090, 0x000d, 0x8179, 0x817a, 0x817b, 0x817c, 
		0x0000,  };
	static const u16 data_8e0ba[17]={
		0x0200, 0x00a0, 0x00a0, 0x000c, 0x8160, 0x8161, 0x8162, 0x8163, 
		0xffff, 0x00a0, 0x0090, 0x000c, 0x8170, 0x8171, 0x8172, 0x8173, 
		0x0000,  };
	static const u16 data_8e0dc[12]={
		0x0000, 0x0008, 0x00f0, 0x0000, 0x8128, 0x8129, 0x812a, 0x812b, 
		0x812c, 0x812d, 0x0000,  };
	static const u16 data_8e0f2[12]={
		0x0038, 0x0100, 0x00f0, 0x0000, 0x8128, 0x8129, 0x812a, 0x812b, 
		0x812c, 0x812d, 0x0000, };
	static const u16 data_8e108[12]={0x0200, 0x0080, 0x00c0, 0x000d, 0x818a, 0x818b, 0x0000, };
	static const u16 data_8e116[12]={0x0210, 0x0080, 0x00b0, 0x000d, 0x819a, 0x819b, 0x0000, };
	static const u16 data_8e124[12]={0x0220, 0x0080, 0x0098, 0x000d, 0x818c, 0x818d, 0x0000, };
	static const u16 data_8e132[12]={0x0200, 0x00a0, 0x0090, 0x000d, 0x811f, 0x0000, 0x0200  };
	static const u16 data_8e13e[17]={
		0x0200, 0x00a0, 0x0090, 0x000d, 0x81ba, 0x81bb, 0x81bc, 0x81bd, 
		0xffff, 0x00a0, 0x00a0, 0x000d, 0x81aa, 0x81ab, 0x81ac, 0x81ad, 
		0x0000,  };
	static const u16 data_8e160[32]={
		0x0180, 0x0000, 0x0050, 0x0016, 0x8090, 0x8090, 0x8090, 0x8090, 
		0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 
		0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 
		0x8090, 0x8090, 0x8090, 0x8090, 0x0000,};
	static const u16 data_8e19a[32]={
		0x0180, 0x0000, 0x0040, 0x0016, 0x8090, 0x8090, 0x8090, 0x8090, 
		0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 
		0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 0x8090, 
		0x8090, 0x8090, 0x8090, 0x8090, 0x0000, };
	static const u16 data_8e1d4[12]={
		0x0200, 0x0088, 0x0080, 0x0000, 0x8118, 0x8119, 0x811a, 0x811b, 
		0x811c, 0x811d, 0x811e, 0x0000,  };
	static const u16 data_8e1ee[]={
		0x0160, 0x0080, 0x00ec, 0x000d, 0x7e18, 0x7e19, 0x7e1a, 0x7e1b, 
		0x7e1c, 0x7e1d, 0x7e1e, 0x7e1f, 0x0000,  };
	static const u16 data_8e208[12]={
		0x01a0, 0x0020, 0x0090, 0x0012, 0x7e08, 0x7e09, 0x0000, };
	static const u16 data_8e216[12]={
		0x01b0, 0x0140, 0x0090, 0x0012, 0x7e0a, 0x7e0b, 0x0000, };
	static const u16 data_8e224[22]={
		0x0000, 0x0070, 0x00b0, 0x0006, 0x2590, 0x2591, 0x25e0, 0x25e1, 
		0x25e2, 0xffff, 0x0080, 0x00a0, 0x0006, 0x26e0, 0x26e1, 0x0000, };
	static const u16 data_8e244[12]={
		0x0200, 0x00a0, 0x00a0, 0x0006, 0x27a6, 0x27a7, 0x27b7, 0x27e8, 
		0x27e9, 0x27ea, 0x037c, 0x0000,  };
	static const u16 data_8e25c[17]={
		0x0000, 0x0070, 0x00b0, 0x000e, 0x2590, 0x2591, 0x25e0, 0x25e1, 
		0x25e2, 0xffff, 0x0080, 0x00a0, 0x000e, 0x26e0, 0x26e1, 0x0000,  };
	static const u16 data_8e27c[12]={
		0x0200, 0x00a0, 0x00a0, 0x000e, 0x27a6, 0x27a7, 0x27b7, 0x27e8, 
		0x27e9, 0x27ea, 0x037c, 0x0000,  };
	static const u16 data_8e294[12]={
		0x0200, 0x0088, 0x00b0, 0x000d, 0x80d0, 0x80c5, 0x80d2, 0x80c6, 
		0x80c5, 0x80c3, 0x80d4, 0x0000,  };
	
	static const u16 *data_8df6c[30]={
		data_8dfa8, data_8dfc0, data_8dfd8, data_8dff0, 
		data_8e008, data_8e020, data_8e038, data_8e050, 
		data_8e068, data_8e080, data_8e098, data_8e0ba, 
		data_8e0dc, data_8e0f2, data_8e108, data_8e116, 
		data_8e124, data_8e132, data_8e13e, data_8e160, 
		data_8e19a, data_8e1d4, data_8e1ee, data_8e208, 
		data_8e216, data_8e224, data_8e244, data_8e25c, 
		data_8e27c, data_8e294, 
	};
	
	Task *task = &Exec.Tasks[Exec.CurrentTask];
	const u16	*data;
	u16			*gfx_p;
	u16			tile;
	short		X, Y;
	short		palette;
	
	task->params.x0014 = 0;
	if (task->params.Param0 & 0x80) {
		//5c92
		data = data_8df6c[task->params.Param0 & 0x7f];
		gfx_p = CPS_OBJ(0x910000 + data[0]);
		data += 4;
		
		while (TRUE) {
			tile = data[0];
			if (tile == 0) {
				if (task->params.Param1) {
					g.TextEffectBusy = FALSE;
				}
				if (task->params.x0014 == 0) {
					DIEFREE;
					return;
				}
				return;
			} else if (tile == 0xffff) {
				data += 4;
			} else {
				OBJECT_DRAW(gfx_p, 0, 0, 0, 0);
				OBJ_CURSOR_BUMP(gfx_p);
				++data;
			}
			if (task->params.Param2) {
				SETSLEEP(task->params.Param2);
			}
		}
	} else {
		data = data_8df6c[task->params.Param0 & 0x7f];
		gfx_p = CPS_OBJ(0x910000 + data[0]);
		X = (data[1] + 0x40);
		Y = -data[2];
		palette = data[3];
		data += 4;
		
		while (TRUE) {
			tile = data[0];
			if (tile == 0) {
				if (task->params.Param1) {
					g.TextEffectBusy = FALSE;
				}
				if (task->params.x0014 == 0) {
					DIEFREE;
					return;
				}
				return;
			} else if (tile == 0xffff) {
				X = (data[1] + 64);
				Y = -data[2];
				palette = data[3];
				data += 4;
			} else {
				OBJECT_DRAW(gfx_p, X, Y, tile, palette);
				OBJ_CURSOR_BUMP(gfx_p);
				X += 0x10;
				++data;
			}
			if (task->params.Param2) {
				SETSLEEP(task->params.Param2);
				return;
			}
		}
	}
}	
static void syslib_20(void) {		//5410 increase player score
	u32 data;
	u16 *gfx_p;
	Player *ply;
	static const u32 data_5500[] = {
		0x100,		0x200,		0x500,		0x1000,
		0x2000,		0x5000,		0x6000,		0x8000,
		0x10000,	0x20000,	0x30000,	0x100000,
		0x1000000L,	0x300,		0x400,		0x1500,
		0x3000,		0x429100,	0x4241700,	0x50000,
	};
		
	Task *task = CURRENT_TASK;

	if (g.InDemo == 0) {
		data = data_5500[task->params.Param0/2];
		ply = task->params.Param2 ? PLAYER2 : PLAYER1;
				
		add_bcd_32(data, &ply->Score);
		printf("inc_score param0: %04x %08x %08x\n", task->params.Param0, data, ply->Score);
		if (ply->Score > 0x9999999) {
			ply->Score = 0x9999999;
		}
		//if (task->params.Param1 == 0) {
			if (ply->Side == 0) {
				OBJ_CURSOR_CPS(gfx_p, 0x910000);
				sub_54bc(&gfx_p, 32, 240, (u8 *)&ply->Score);
			} else {
				OBJ_CURSOR_CPS(gfx_p, 0x910038);
				sub_54bc(&gfx_p, 296, 240, (u8 *)&ply->Score);
			}
		//}
		if (g.TopScore < ply->Score) {
			g.TopScore = ply->Score;
			OBJ_CURSOR_CPS(gfx_p, 0x910098);
			sub_54bc(&gfx_p, 164, 240, (u8 *)&g.TopScore);
		}
	}
	DIEFREE;
}

#pragma mark SysLib08 Text Routines 



static void aTextRoutine(Task *task) {		// 4f78
	static void (*textRoutines[])(u8 param) = {		// 4f8a
		showtextbank0,			// 5602
		showtextbank1,			// 568c
		showtextbank2,			// 574a		winners
		print_libtextgfx,		// 5816
		showtextbank4,			// 58c0
	};
	
    textRoutines[task->params.x0015] (task->params.Param0 & 0xff);
}
static void syslib_08 (void) {	// 4f3a Text Blinker, insert coin etc.
	Task *task = &Exec.Tasks[Exec.CurrentTask];

	task->params.x0014 = TRUE;					//byte
	task->params.x0016 = task->params.Param1;	//byte
	task->params.x0015 = task->params.Param2;
	task->params.Param0 ^= 0x80;			
	
	while (TRUE) {
		task->params.Param0 ^= 0x80;
		aTextRoutine(task);
		sf2sleep(24);
	}
}


static void syslib_26(void) {
	Task *task = CURRENT_TASK;
	sf2sleep(10);
	DIEFREE;
}
	


#pragma mark STATIC TASKS 

void task_scheduler(void) {		//14f2
	unsigned char i;
	
	while (TRUE) {
		g.x8a30 = FALSE;
		if (g.WaitMode || (g.Debug && (g.JPParam & JP_DBGWAITMODE))) {
			g.x8a30 = TRUE;
			for (i=0; i<16; i++) {
				if (Exec.Tasks[i].params.x001f == 0 && Exec.Tasks[i].status == TASK_SLEEP) {
					if (--Exec.Tasks[i].timer == 0) {
						Exec.Tasks[i].status = TASK_READY;
					}
				}
			}
		}
		exit_ready();
		if (Exec.x820e == 0 && Exec.FreeTasks != 0) {
			if (g.effectQueue[g.effectCurrent / 2] != 0xffff) {
				wrap_trap7(data_155c[g.effectQueue[g.effectCurrent / 2] >> 10],
						   g.effectQueue[g.effectCurrent / 2 ] & 0xff,
						   g.effectQueue[(g.effectCurrent / 2 ) + 1]
						   );
				g.effectCurrent += 4;
				g.effectCurrent &= 0xfc;
			}
		}
	}
}

static void LBPlayerHasEntered(Player *ply) {		// 6fd4
	g.NewPlayers    |= (1 << ply->Side);
	g.PlayersOnline |= (1 << ply->Side);
	ply->Continuing = 0;
	ply->Alive = 2;
	ply->BlinkerMode0 = 4;
	ply->BlinkerMode1 = 0;
	ply->BlinkerMode2 = 0;
	print_libtextgfx(0x80 + ply->Side);	
	give_100_points(ply->Opponent->Side);
	give_100_points(ply->Side);	// 53d6
}	

static void setplayerblinker(Player *ply, unsigned char selector){		//6e8e
	if (g.BattleOver) {
		print_libtextgfx(ply->Side + 0x80);		// Erase the blinker
	} else {
		print_libtextgfx(selector);
	}
}
static void sub_703a(Player *ply, unsigned char erase) {
	if (g.FreePlay) {
		setplayerblinker(ply, erase + FREE_PLAY_P1 + ply->Side);
	} else if (g.TwoCreditsToStart) {
		if (g.NumberCredits < 2) {
			setplayerblinker(ply, erase + ADD_COIN_P1 + ply->Side);
		} else {
			setplayerblinker(ply, erase + PUSH_START_P1 + ply->Side);
		}
	} else {
		setplayerblinker(ply, erase + PUSH_START_P1 + ply->Side);
	}
}
static void sub_7018(Player *ply) {
	++ply->BlinkerTimer;
	if ((ply->BlinkerTimer & 7) == 0) {
		if (ply->BlinkerTimer & 0x10 ) {
			sub_703a(ply, 0x80);
		} else {
			sub_703a(ply, 0x0);
		}
	}
}
static void sub_716a(Player *ply) {
	u32 cp;
	u16 index;
	if (g.BattleOver ) {
		print_libtextgfx(0x80 + ply->Side);	
	} else {
		print_libtextgfx(0xc + ply->Side);
		if (ply->Side) {
			index = 0x68;
			cp = MakePointObj(360, 240);
		} else {
			index = 0x30;
			cp = MakePointObj(104, 240);
		}
		OBJECT_DRAW_ID(index / 8, CP_X, CP_Y, TILE_BIGDIGIT + ply->ContinueSecs, 0x0);
		/* both buffers */
	}
}

#pragma mark ---- Fade Entries ----

void fadenwait1 (void) {			/* 0x2138 */
    es.FadeBusy = TRUE;
    QueueEffect(SL0C | SL0C_FO_CL, 0x3);
	SIG_WAIT(es.FadeBusy);
}
void fadenwait2 (void) {		/* 215a */
    es.FadeBusy = TRUE;
    QueueEffect(SL0C | SL0C_FI_W, 0x3);
	SIG_WAIT(es.FadeBusy);
}
void fadenwait3 (void) {		/* 217c */
    es.FadeBusy = TRUE;
    QueueEffect(SL0C | SL0C_FO_CLW, 0x3);
	SIG_WAIT(es.FadeBusy);
}
void fadenwait4 (short arg0) {		/* 219e */
    es.FadeBusy = TRUE;
    QueueEffect(SL0C | SL0C_FO_CL, arg0);
	SIG_WAIT(es.FadeBusy);
}
void fadenwait5(short arg0) {		/* 21b0 */
	es.FadeBusy = TRUE;
	QueueEffect(SL0C | SL0C_FI_W, arg0);
	SIG_WAIT(es.FadeBusy);
}


static void sub_70d2(Player *ply) {		// coin inserted, reset continue
										// counter to 9
	if (ply->ContinueCredits < g.NumberCredits) {
		g.ContinueCredits = g.NumberCredits;
		ply->ContinueSecs = 9;
		ply->ContinueTick = 0x50;
		sub_716a(ply);
	}	
}

static short LBPlayerStartDown(Player *ply) {			// 725e
	if (g.RawButtons0Dash &  (1 <<(ply->Side ? 5 : 4)) ) {
		return 1;
	}
	return 0;
}
static short LBCheckPaidStart(Player *ply) {		// 7252
	if ((g.FreePlay == 0) && (g.NumberCredits == 0)) {
		return 0;
	}
	return LBPlayerStartDown(ply);
}

static void LBCheckContinued(Player *ply) {	
	if(LBCheckPaidStart(ply)) {
		++g.ContinueCount;
		bumpdifficulty_01();		// 453c
		if (g.FreePlay == 0) {
			g.NumberCredits--;		/* was BCD */
		}
		give_100_points(ply->Side ^ 1);
		ply->BlinkerMode1 = 4;
		if (g.NewChallengerWait) {
			ply->BlinkerMode1=6;
		}
		ply->ContinueTick = 2;
		print_libtextgfx(LIBTEXT_ERASE + ply->Side);	
	}
}
static void LBContinueBump(Player *ply) {			// 7222
	u16 data;
	if (ply->ContinueSecs < 7) {
		if (ply->Side) {
			data = (g.ContrP2.full & ~g.ContrP2DB.full);
		} else {
			data = (g.ContrP1.full & ~g.ContrP1DB.full);
		}
		if (data & BUTTON_MASK) {
			ply->ContinueTick = 1;
		}
	}
}
static void LBPlayerHasLeft(Player *ply) {			// 7660
	g.PlayersOnline &= ~(1 << ply->Side);
	ply->BlinkerMode0 = 0;
	print_libtextgfx(LIBTEXT_ERASE + ply->Side);	
}
	
static void SMPlayerBlinker(Task *task, Player *ply) {		// 6ea4
	if (ply->Alive == FALSE) {
		switch (ply->BlinkerMode0) {
			case 0:
				if (g.JapanJumper == 0) {
					if (g.x02eb != 0) {
						LBPlayerHasLeft(ply);
					} else {
						switch (ply->BlinkerMode1) {
							case 0:
								if (g.FreePlay || g.NumberCredits) {
									NEXT(ply->BlinkerMode0);
									ply->BlinkerMode1 = 0;
								} else {
									NEXT(ply->BlinkerMode1);
									ply->BlinkerTimer=0;
									setplayerblinker(ply, 0x0 + ply->Side);
								}
								break;
							case 2:
								++ply->BlinkerTimer;
								if (g.FreePlay || g.NumberCredits) {
									NEXT(ply->BlinkerMode0);
									ply->BlinkerMode1 = 0;
								} else {
									if ((ply->BlinkerTimer & 7) == 0) {
										if (ply->BlinkerTimer & 0x20) {
											setplayerblinker(ply, LIBTEXT_ERASE + ply->Side);
										} else {
											setplayerblinker(ply, 0x0  + ply->Side);
										}
									}
								}
								break;
							case 4:
								if (g.NewChallengerWait == 0) {
									ply->BlinkerMode1 = 0;
									LBPlayerHasEntered(ply);
								}
							default:
								break;
						}
					}

				}
				break;
			case 2:		// 6f4a
				if (g.JapanJumper == 0) {
					if (g.x02eb != 0) {
						LBPlayerHasLeft(ply);
					} else {
						if (g.FreePlay || g.NumberCredits) {
							switch (ply->BlinkerMode1) {
								case 0:
									NEXT(ply->BlinkerMode1);
									ply->BlinkerTimer = 0;
									sub_703a(ply, 0);
									break;
								case 2:
									sub_7018(ply);
									if (g.BattleOver != 0) {
										return;;
									} else if (LBPlayerStartDown(ply)) {
										g.x02f0 += 1;
										bumpdifficulty_4576();
										if (g.FreePlay == 0) {
											if (g.NumberCredits < (1 + g.TwoCreditsToStart)) {
												return;
											} 
											g.NumberCredits -= (1 + g.TwoCreditsToStart);
											
										}
										if (g.NewChallengerWait) {
											ply->BlinkerMode0 = 0;
											ply->BlinkerMode1 = 4;
											/* 6e98 */
											print_libtextgfx(LIBTEXT_ERASE + ply->Side);	
										} else {
											LBPlayerHasEntered(ply);
										}
									}
									break;
								FATALDEFAULT;
							}
						}
					}
				}
				break;
			case 4:		// 7072
				if ((g.JapanJumper && g.x031e) ||
					g.x02eb							 ||
					g.BattleOver					 ||
					g.AllowContinue == 0
					) {
					NEXT(ply->BlinkerMode0);
					ply->BlinkerMode1 = 0;
				} else {
					switch (ply->BlinkerMode1) {
						case 0:
							NEXT(ply->BlinkerMode1);
							ply->ContinueCredits = g.NumberCredits;
							g.ContinueBits |= (1 << ply->Side);
							ply->ContinueSecs = 9;
							ply->ContinueTick = 0x50;
							sub_716a(ply);
							break;
						case 2:
							LBContinueBump(ply);
							sub_70d2(ply);
							LBCheckContinued(ply);
							if(--ply->ContinueTick == 0) {
								if (ply->ContinueSecs == 0) {
									g.ContinueBits &= ~(1<<ply->Side);
									ply->BlinkerMode0 = 6;
									ply->BlinkerMode1 = 0;
								} else {
									ply->ContinueTick = 0x50;
									ply->ContinueSecs -= 1;
									sub_716a(ply);
									queuesound(SOUND_CONTTICK);
								}
							}
							break;
						case 6:
							if (g.NewChallengerWait) {
								break;
							}
							/* FALLTHRU */
						case 4:
							g.TwoHumans = TRUE;
							g.ContinueBits &= ~(1<<ply->Side);
							g.NewPlayers |= (1<<ply->Side);
							g.PlayersOnline |= (1<<ply->Side);
							ply->Continuing = TRUE;
							ply->Alive = 2;
							ply->BlinkerMode1 = 0;
							ply->BlinkerMode2 = 0;
							print_libtextgfx(0x80 + ply->Side);	
							break;

						default:
							break;
					}
				}

				break;
			case 6:			// 7274
				/* XXX highscore */
				NEXT(ply->BlinkerMode0);
				break;
			case 8:			// 7626
				if (g.x02eb || g.BattleOver) {
					LBPlayerHasLeft(ply);
				} else {
					switch (ply->BlinkerMode1) {
						case 0:
							NEXT(ply->BlinkerMode1);
							ply->ContinueSecs = 0x80;
							setplayerblinker(ply, 0x8 + ply->Side);		
							break;
						case 2:
							if (--ply->ContinueSecs == 0) {
								LBPlayerHasLeft(ply);
							}
							break;
						default:
							break;
					}
				}
			default:
				break;
		}
	}
}



void task_blinkers(void) {		// 6e64
	Task *task = &Exec.Tasks[Exec.CurrentTask];
	
	task->params.x001f = TRUE;
	while (TRUE) {
		check_coin_lockout();
		if (g.FreezeMachine == FALSE) {
			SMPlayerBlinker(task, PLAYER1);
			SMPlayerBlinker(task, PLAYER2);
		}
		sleep2();
	}
}

void task_game(void) {			// 7672 Game Supertask
	fadenwait4(1);
	while (TRUE) {
		g.NoInterrupt = MINUS_ONE;
		if (g.FreezeMachine == FALSE) {
			g.libsplatter++;
			
			LBGetInputs();				// get_inputs();
			LBDecodeInputs();			// decode_inputs();
			SM_game();					// game state machine 
			
			debughook(1);
		}
		(void)CHECK_SERVICE_BUTTON;       // XXX this is not a sub
		if (g.Debug && (g.JPCost & JP_DBGSLEEP)) {
			sf2sleep((g.JPDifficulty & JP_DIFFMASK) + 2);
		} else {
			if (g.NoInterrupt) {sf2sleep(1); };
			/* don't sleep if the interrupt stacked */
		}
	}
}

	


