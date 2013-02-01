/* sf2 lib.c */

#include "sf2types.h"
#include "sf2macros.h"
#include "sf2const.h"

#include "gstate.h"
#include "structs.h"
#include "player.h"

#include "particle.h"

#include "sprite.h"

#include "lib.h"
#include "task.h"
#include "rules.h"
#include "sound.h"
#include "sf2io.h"

#include "actions.h"

#include "gemu.h"
#include "gfxlib.h"

#include "actions_198a.h"
#include "projectiles.h"
#include "collision.h"
#include "effects.h"
#include "sm.h"


#ifdef __APPLE__
#include <execinfo.h>
#endif





#ifndef CPS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#endif

#include "libdata.h"


extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;
extern ScrollState gstate_RowScroll;

extern struct executive_t Exec;


extern CPSGFXEMU gemu;

extern struct inputs gInputs;
extern void *data_155c[];
extern u8 data_995de[12][32];

static void sub_2af2(void);
static void sub_2c38(void);
static void _LBResetState(void);
static void clear_playerselect(void);
static void sub_297a(void);
static void clear_gstates(void);


#ifdef APPLICATION_TESTS
int main(void) {
	printf("lib.c v0.01 tests\n");
}
#endif

static void decode_jumpers(void);

static void sub_2c38(void) {		// 2c38
	int d2=g.CurrentStage;
	if(g.OnBonusStage==FALSE && g.ActiveHumans != 3) {
		if (g.Player1.Human || g.Player1.x02ae) {
			if (g.Player1.FighterID == d2) {
				d2 = g.Player1.FighterSelect;
				if (d2 == g.CurrentStage) {
					d2=g.Player2.FighterID;
				}
					
			}
			g.Player2.FighterID = d2;
		} else {
			if (g.Player2.FighterID == d2) {
				d2 = g.Player2.FighterSelect;
				if (d2 == g.CurrentStage) {
					d2 = g.Player1.FighterID;
				}
			}
			g.Player1.FighterID = d2;			
		}
	}
}
			

void sleep2(void) {	
	if(g.Debug && (g.JPCost & JP_DBGSLEEP)) {
		sf2sleep((g.JPDifficulty & JP_DIFFMASK) + 2);
	} else {
		sf2sleep(1);
	}
}
		

void init_fight_vars(void) {			/* 0x2b0c */
	g.Player1.RoundsWon  = 0;
	g.Player2.RoundsWon  = 0;
	g.Player1.Continuing = FALSE;
	g.Player2.Continuing = FALSE;
	g.ActiveHumans = (g.Player1.Human | g.Player1.x02ae) + ((g.Player2.Human | g.Player2.x02ae) * 2);
	if(!g.OnBonusStage) {
		sub_2c38();
	}
	g.Player2.OpponentID = g.Player1.FighterID;
	g.Player1.OpponentID = g.Player2.FighterID;
	
	memclear(&g.x0a4a, (void *)&g.x0a6a - (void *)&g.x0a4a);
}
	
void newgame(void) {
	sub_2af2();		/* zero the scores and some game vars */
	BumpDiff_NewGame();
}
	
static void sub_2af2(void) {
	/* 0x180 bytes in CPS */
	memclear(&g.CurrentStage, (void *)&g.x0b4a - (void *)&g.CurrentStage);
	g.Player1.Score = 0;
	g.Player2.Score = 0;
}

void debughook(int data) {
	/* unimp */
}

void panic(int data) {
	printf("PANIC()\n");

#ifndef CPS
	void *callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = 0; i < frames; ++i) {
		printf("%s\n", strs[i]);
	}
	free(strs);

	exit(1);
#endif
	
	while (TRUE) {
		
	}
	/* do panic stuff here */
}

#pragma mark ---- Jumper Decoding ---

void decode_difficulty(void) {
	g.Difficulty = g.JPDifficulty & JP_DIFFMASK;
	if(g.Version == VERSION_JAP) {
		g.JapanJumper = 1 ^ ((g.JPDifficulty & JP_JAPANJUMP) / JP_JAPANJUMP);
	}
}
static void decode_jumpers(void) {
	decode_coincosts();
	decode_difficulty();
	decode_params();
}

void read_jumpers() {
    g.JPCost       = GET_JUMPER(0);
    g.JPDifficulty = GET_JUMPER(1);
    g.JPParam      = GET_JUMPER(2);
}
static void _get_live_jumpers(void) {		//1e32
    g.DemoSound     = g.JPParam & JP_DEMOSOUND ? TRUE : FALSE;
    g.FreezeMachine = g.JPParam & JP_FREEZE    ? TRUE : FALSE;
    g.FlipDisplay   = g.JPParam & JP_FLIP      ? TRUE : FALSE;
    g.Debug2        = g.JPParam & JP_DEBUG2    ? TRUE : FALSE;
	
    /* to enable g.Debug:
	 start machine, holding down P2 jab + fierce, until international copyright starts printing,
	 then put jumper 0Debug2 on
     */
	
    if(g.Debug2) { g.Debug = g.Debug2; }
}
void decode_params(void) {
    g.FreePlay      = g.JPParam &= JP_FREEPLAY  ? TRUE : FALSE;
    g.AllowContinue = g.JPParam &= JP_ALLOWCONT ? TRUE : FALSE;
	_get_live_jumpers();
}
static void intmaths(void) {
#ifndef CPS
	gemu.Scroll1X = g.CPS.Scroll1XDash;
	g.CPS.Scroll1XDash = g.CPS.Scroll1X - 0x40;
	gemu.Scroll2X = g.CPS.Scroll2XDash;
	g.CPS.Scroll2XDash = g.CPS.Scroll2X - 0x40;
	gemu.Scroll3X = g.CPS.Scroll3XDash;
	g.CPS.Scroll3XDash = g.CPS.Scroll3X - 0x40;
	/* TODO star1x star2x */
	gemu.Scroll1Y = g.CPS.Scroll1YDash;
	g.CPS.Scroll1YDash = 0x100 - g.CPS.Scroll1Y;
	gemu.Scroll2Y = g.CPS.Scroll2YDash;
	g.CPS.Scroll2YDash = 0x300 - g.CPS.Scroll2Y;
	gemu.Scroll3Y = g.CPS.Scroll3YDash;
	g.CPS.Scroll3YDash = 0x700 - g.CPS.Scroll3Y;
	/* TODO star1x star1Y*/
#endif
	
}

// divide 
static int sub_252a(u32 d2, u32 d3) {
	return d3 / d2;
	
//	int d0 = 0;
//	int i;
//	for (i=31; i >= 0; --i) {
//		if (coord & 0x80000000) 
//			++d0;
//		coord <<= 1;
//		d0    <<= 1;
//		if (d0 >= greatercoord) {
//			d0 -= greatercoord;
//			++coord;
//		}
//	}
//	return coord;
}

// calculate the angle from the object to a given point.
// returns angle out of 0xff
u8 calc_flightpath(Object *obj, int x, int y) {		// 24d2 flightpath
	u8 direction = 0;
	int d3;
	
	if (y - obj->YPI == 0) {
		if (x - obj->XPI < 0) {
			return 0xc0;
		} else {
			return 0x40;
		}
	} else if (y - obj->YPI < 0) {
		direction = 0x80;
	} else {			
		direction = 0x00;
	}			
	if (x - obj->XPI == 0) {
		return direction;
	}
	direction >>= 1;
	if (x - obj->XPI < 0) {
		direction |= 0x80;
	}
	if (ABS(y-obj->YPI) == ABS(x-obj->XPI)) {		// multiple of 45 degrees
		return	(u8 []){0x20, 0x60, 0xe0, 0xa0}[direction >> 6];	
	} else if (ABS(y-obj->YPI) > ABS(x-obj->XPI)) {
		direction  >>= 1;
		d3 = (sub_252a(ABS(y-obj->YPI),ABS(x-obj->XPI) << 8) >> 3) & 0x1f;
	} else {
		direction  >>= 1;
		direction |= 0x80;
		d3 = sub_252a(ABS(x-obj->XPI), ABS(y-obj->YPI) << 8) >> 3;
	}
					//  0		          180         90    270
	direction = (u8 []){0x00, 0x81, 0x01, 0x80, 0x41, 0x40, 0xc0, 0xc1}[(direction >> 5)];
	if (direction & 0x01) {
		return (direction & 0xfe)-d3;
	} else {
		return d3+direction;
	}	
}
	


void sub_22a8(void) {		/* copy the controls to the player struct */
	g.Player1.JoyDecode.full = g.ContrP1.full;
	/* some wierdness at 22ba  todo , easter egg?    */
	g.Player2.JoyDecode.full = g.ContrP2.full;
}

void LBDecodeInputs(void) {		// 2320
	int a,b;
	int c,d;
	
	a = g.Player1.JoyDecode.full;
	b = g.Player1.JoyDecodeDash.full;
	c=a;d=b;
	if (g.Player1.Direction) {		// todo: check this
		a &= 0x77c; b &= 0x77c;
		if (c & 2) {
			a |= 1;
			b |= 1;
		}
		if (c & 1) {
			a |= 2;
			b |= 2;
		}
	}
	g.Player1.JoyCorrect  = (a & 0xff00) >> 8;
	g.Player1.JoyCorrect2 = (a & 0xff); 
	g.Player1.JoyCorrectDash  = (b & 0xff00) >> 8;
	g.Player1.JoyCorrectDash2  = (b & 0xff); 
}

void intproc(void) {        /* 0x1baa */
#ifdef CPS
	g.x008d = g.FlipDisplay ^ g.x02df;
    (void *)CPS_VIDCNTL = (g.x008d >> 4) | g.x004c; /* might as well */
    (void *)CPS_DISPENA = g.DispEna;     /* need to emulate this */
    (void *)CPS_COINCTL = g.CoinCtrl;
    (void *)CPS_PRIO0   = g.x0054;       
    (void *)CPS_PRIO1   = g.x0056;
    (void *)CPS_PRIO2   = g.x0058;
    (void *)CPS_PRIO3   = g.x005a;
#endif
	// these were bitshifts on a longword
	g.RawButtons0Dash3 = g.RawButtons0Dash2;
	g.RawButtons0Dash2 = g.RawButtons0Dash;
    g.RawButtons0Dash = g.RawButtons0;
    g.RawButtons0 = GET_INPUT_1;     /* start, coin, and service buttons */
    g.ContrP1DB.full = g.ContrP1.full;
    g.ContrP2DB.full = g.ContrP2.full;
	
	
    g.ContrP1.part.p1 = GET_INPUT_P1_1;
    g.ContrP2.part.p1 = GET_INPUT_P2_1;
    g.ContrP1.part.p0 = GET_INPUT_P1_0;
    g.ContrP2.part.p0 = GET_INPUT_P2_0;
	
    intmaths();
    //read_jumpers();
	_get_live_jumpers();
}

static void _controlscript_P1(void) {	// 225c fetch controls from script instead of user
	const u16 *data;
	if(--g.DemoJoyP1Timer != 0) {
		data = g.DemoJoyP1;
		if (data[1]==0) {
			data += 2;
		}
		g.Player1.JoyDecode.full = data[0];
		g.DemoJoyP1Timer = data[1];
		g.DemoJoyP1 = data + 2;
	}
}
static void _controlscript_P2(void) {	// 2282 fetch controls from script instead of user
	const u16 *data;
	if(--g.DemoJoyP2Timer != 0) {
		data = g.DemoJoyP2;
		if (data[1]==0) {
			data += 2;
		}
		g.Player2.JoyDecode.full = data[0];
		g.DemoJoyP2Timer = data[1];
		g.DemoJoyP2 = data + 2;
	}
}

void LBGetInputs(void) {		//2224
	g.Player1.JoyDecodeDash.full = g.Player1.JoyDecode.full;
	g.Player2.JoyDecodeDash.full = g.Player2.JoyDecode.full;
	if (g.InDemo) {
		if ((g.JPCost & 0x80) && (g.JPCost & 0x40)) {
			sub_22a8();
		} else {
			_controlscript_P1();
			_controlscript_P2();
		}
	} else {
		g.Player1.JoyDecode.full = g.ContrP1.full;
		g.Player2.JoyDecode.full = g.ContrP2.full;
	}
}


static void _any_buttons(short d0, short d1) {		//23ae
	if ((d0 | d1) & BUTTON_MASK ) {
		g.WaitMode = TRUE;
	}
}

void startup_impatience(void) {		
	_any_buttons(g.ContrP1.full, g.ContrP2DB.full);
}
	

/* Allow a player to speed up countdowns and intermediate screens by pressing
 any of their buttons */
void set_waitmode(void) {		/* 2388 */
	short player1 = 0; 
	short player2 = 0;
	
	if(g.InDemo == FALSE) { 
		g.WaitMode = FALSE;
		
		if(g.PlayersOnline & ONLY_P1) {
			player1 = g.Player1.JoyDecode.full;
		}
		if(g.PlayersOnline & ONLY_P2) {
			player2 = g.Player2.JoyDecode.full;
		}
 		_any_buttons(player1,player2);
	}
}

#pragma mark ---- Timer Callbacks ----

void fightstuff (void) {
    if(g.TimeWarpTimer) {
        if(--g.TimeWarpTimer == 0) { g.FlagTimeWarp = FALSE; } 
        if(--g.TimeWarpSlowdown == 0) { 
            g.TimeWarpSlowdown = g.TimeWarpSlowdown2; 
            fighttick();
        } else {
            g.libsplatter--;
        }
    } else {
        fighttick();
    }
}
inline void fighttick(void) {
    proc_all_actions();
    CDCheckPlayers();
    DSDrawAllMain(); 
}
inline void proc_all_actions (void) {    /* 0x7f9a */
    proc_player_actions();    
    DSDrawShadows();    
    process_projectiles();
    actions_198a();
    proc_actions();
    actions_530a();
    GSMain();      
}
void all_actions_sprites (void) {
	/* Collisions are ignored, otherwise same as fighttick */
    proc_all_actions();
    DSDrawAllMain(); 
}
int check_if_new_player (void) { /* 7e60 */
    if(g.NewPlayers) {
        g.mode2  = 0xc; /* new challenger */
        g.mode3  = 0;
        g.mode4  = 0;
        g.mode5  = 0;
        g.mode6  = 0;
        g.WaitMode = FALSE;
        return 1;
    }
	return 0;
}

static void sub_b06(void) {
	/* write g.x005c to 80014a */
	/* write g.CPS.PalBase to 80010a */
}
	
static void soundhook(void) {
	/* todo: empty a word from soundqueue to sound system */
}



static void _refresh_jumpers(void) {		//1d72
	_get_live_jumpers();		
	if(g.Debug && (g.JPParam & JP_DBGNOCOLLIDE)) {
		g.DebugNoCollide = TRUE;
	} else {
		g.DebugNoCollide = FALSE;
	}

}


/****************************************************************/
#pragma mark           INTERRUPT HANDLER                        
/****************************************************************/


#ifdef CPS
	// todo: GCC interrupt routine specifier
#endif CPS
void sf2_interrupt (void) {
    static int i;    
	
    /* arch-specific stuff should be done already */
    intproc();   /* Set video regs, get player inputs */
    sub_b06();
    debughook(0);	/* not correct value */
    soundhook();
    sub_1ed0();   /* coin accounting */
    int_cb_coinage();   /* play coin sounds, update credit display */
    _refresh_jumpers();   /* reread some jumpers, some debug stuff */
	
    g.tick++;
#ifdef CPS
    //Exec.x820e = -1;
#endif
    g.NoInterrupt = FALSE;
    
    for(i = 0; i<MAX_TASKS; ++i) {
        if(Exec.Tasks[i].status == TASK_SLEEP) {
			if (--Exec.Tasks[i].timer == 0) {
				Exec.Tasks[i].status = TASK_READY;
			}
        }
    }
    /* return to arch-specific return */
    /* or start despatching tasks on non-CPS, event-based machines.*/
}


void reset_layer_prios(void) {		//18e4 
	g.CPS.Prio[0]=0;
	g.CPS.Prio[1]=0;
	g.CPS.Prio[2]=0;
	g.CPS.Prio[3]=0;
}
	
void sub_18c8(void) {
	/* does same as ... */
	reset_layer_prios();
}

/****************************************************************/
#pragma mark                STARTUP     
/****************************************************************/

void startup (void) {
    int i;
	
	static const struct HiScore data_da4[] = {
		{0x50000, {'N','I','N',0 }},
		{0x45000, {'S','I','N',0 }},
		{0x40000, {'H','A','C',0 }},
		{0x35000, {'M','T','I',0 }},
		{0x30000, {'E','S','H',0 }},
		{0x25000, {'C','B','X',0 }},
	};
	
#ifdef CPS
    /* todo: asm specific stuff, supervisor stack pointer, register vars */
#endif /* CPS */
	
    g.effectNext    = 0;
    g.effectCurrent = 0;
    g.soundNext     = 0;
    g.soundCurrent  = 0;
    
    for (i=0; i<0x80; i++) {g.effectQueue[i] = 0xffff;}
#ifdef SOUND
    for (i=0; i<0x80; i++) {g.soundQueue[i] =  0;}
#endif
    
    g.Version = VERSION_USA;
	
    palette_base_1k();		// checkme
    palette_macro(0x10);
    read_jumpers();
    decode_jumpers();
	
    g.Debug2 = FALSE;
#ifdef CPS
    if ( 0x50 == !*CPS_INPUT_0 ) {g.Debug2 = 1;}	// todo: Want this out of CPS
	
    for (i=0; i<MAX_TASKS; i++) {
        Tasks[i].SP = init_task_SPs[i];
    }
#endif 

	for (i=0; i<6; i++) {  g.HiScoreTable[i] = data_da4[i];  }
	for (i=7; i>=0; i--) { Exec.FreeTaskStack[i]=&Exec.Tasks[15-i];}
	
    Exec.FreeTasks    = 8;
	Exec.NextFreeTask = &Exec.FreeTaskStack[0];
    g.NotUsed         = FALSE;
	
	TASK_CREATE(task_initmachine, 0, 0, 0, 0);		/* init vars, copyright notices etc */
    TASK_CREATE(task_scheduler  , 6, 0, 0, 0);      /* the CQ processor */
    
    /* startup done */
#ifdef CPS
    game_despatcher();      /* never returns */
#endif
	// otherwise we return to event-based system we run in, and it despatches
	// the tasks in its timer loop.
}


void bin2bcd(short dec){
#ifndef CPS
	/* Artificial for non-embedded systems */
	int result = 0, i;
	
	for(i = 0; dec;) {
		result = (dec % 10) * (int) pow(10,i); /* Get and convert lowest-order number. */
		dec = (int) dec / 10; /* moved down here for clarity */
	}		 
					 
	g.x8a42 = result;
#endif
#ifdef CPS
	/* use 68000 asm code */
	volatile __asm__ {
		//68000 asm here
	}
	panic(1);
#endif
}



	
short sf2rand(void) {
    int x = (g.randSeed1 << 8) + g.randSeed2;
	
#ifdef DEBUG 
	static int tally[256];
#endif
	
    x *= 3;
    x = x >> 8;
    g.randSeed2 += x;
    g.randSeed1  = x;

#ifdef DEBUG
	tally[g.randSeed2]++;
#endif
    return g.randSeed2;
}

void set_ply_directions(Player *ply) {	/* 2f8a */
	short temp;
	if (g.Player1.XPI > g.Player2.XPI) {
		temp = 1;
	} else {
		temp = 0;
	}
	if ((g.Player2.XPI - g.Player1.XPI <=  24 ||		// checkme
		g.Player2.XPI - g.Player1.XPI >= -24) && 
		g.GPCollDetect) {
		/* 2fc0 */
		printf("odd case in set_ply_directions\n");
		g.Player1.EnemyDirection = 1 ^ g.Player1.PushDirection;
		g.Player2.EnemyDirection = 0 ^ g.Player1.PushDirection;
	} else {
		g.Player1.EnemyDirection = 1 ^ temp;
		g.Player2.EnemyDirection = 0 ^ temp;
	}
}
		
/* rarely returns 1 for free blades etc + spritelib */
short LBRareChance(void) {					// 3052
	if(data_98d82[RAND64] & (1 << RAND8)) {
		return TRUE;
	} else {
		return FALSE;
	}
}


#pragma mark AI Updates

static void _set_AI_urgency(Player *ply) {		// 30da
	const static char data_95660[0x92]={
		0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
		0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
		0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
		0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
		0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
		0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
		0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
		0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
		0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 
		0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 
		0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
		0x07, 0x07, 
	};
	
	short d1 = 0;
	short d2, d0;
	if (g.FastEndingFight) {
		ply->RoughTimeRemain = 7;
	} else {
		if (ply->Energy >= 0) {
			if (ply->Energy >= 0x91) {
				d0 = 0x90;
			} else {
				d0 = ply->Energy;
			}
			d1 = data_95660[d0];
		}
		
		if        (g.TimeRemainBCD < 0x10) { d2 = 0;
		} else if (g.TimeRemainBCD < 0x30) { d2 = 1;
		} else if (g.TimeRemainBCD < 0x40) { d2 = 2;
		} else if (g.TimeRemainBCD < 0x50) { d2 = 3;
		} else if (g.TimeRemainBCD < 0x60) { d2 = 4;
		} else if (g.TimeRemainBCD < 0x70) { d2 = 5;
		} else if (g.TimeRemainBCD < 0x80) { d2 = 6;
		} else {							 d2 = 7;
		}
		
		if (d2 < d1) {
			d1 = d2;
		}
		ply->RoughTimeRemain = d1;
	}
}
static void _set_AI_timers(Player *ply) {		// 307e
	int i;
	for (i=0; i<8; i++) {
		ply->AITimers[i] = data_956f2[ply->FighterID][i][ply->Difficulty];
	}
	for (i=0; i<8; i++) {
		ply->AITimers[i]+=data_956f2[ply->FighterID][i+8][RAND32];
		if (ply->AITimers[i]<=0) {
			ply->AITimers[i]=1;
		}
	}
}
void LBUpdateAITimers(Player *ply) {	/* 3074 Updates needed by AI */
	_set_AI_urgency(ply);		
	_set_AI_timers(ply);		
}
static void _init_energy(void) {			/* 2e6e */
	g.Player1.Energy		= 
	g.Player1.EnergyDash	=
	g.Player2.Energy		= 
	g.Player2.EnergyDash	=
	g.Player1.EnergyCursor	=
	g.Player2.EnergyCursor	=	ENERGY_START;
}
static void _init_difficulty(void) { 
	Player *ply;

	const static char data_2e4e[32]={
		0x00, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x03, 
		0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 
		0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 
		0x05, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x08, 
	};
	
	if (g.InDemo) {
		g.Player1.Difficulty = g.Player2.Difficulty = 15;
	} else if (g.ActiveHumans == 3) {
		g.Player1.Difficulty = 16;
		g.Player2.Difficulty = 16;

		if (g.Player1.RoundsWon != g.Player2.RoundsWon) {
			if (g.Player1.RoundsWon) {
				ply=PLAYER2;
			} else {
				ply=PLAYER1;
			}
			ply->Difficulty +=	data_2e4e[RAND32];	/* u8[32] */
			if (ply->Difficulty >= 32) {
				ply->Difficulty = 31;
			}
		}
	} else {
		if (g.Player1.Human) {
			g.Player2.Difficulty = 16;
		} else {
			g.Player1.Difficulty = 16;
		}
	}
}

void LBInitPlayers(void) {		// 2c8a
	_init_difficulty();	
	_init_energy();		
	
	memclear((void *) &g.TimeRemainBCD, ((void *)&g.x0b4a - (void *)&g.TimeRemainBCD));
	
	g.TimeWarpTimer     = 0;
	g.TimeWarpSlowdown  = 0;
	g.TimeWarpSlowdown2 = 0;
	bumpdifficulty();
	GSInitOffsets();
}
    
void ClearEffectQueue(void) {		/* 21c2 was resetcq */
    int i;
    g.effectNext = g.effectCurrent = 0;
    for (i = 0; i<EFFECT_QUEUE_LENGTH; i++) {
        g.effectQueue[i] = 0xffff;
    }
}

short start_effect(short d0, short d1) {		/* 158c was libcall()*/
	if (Exec.FreeTasks == 0) {
		sf2sleep(1);
	} else {
		if (g.mode0 == 2 && g.mode1 == 4 && g.mode2 == 10 && (d0 & 0xff00) == 0) {
			return g.libsplatter;
		} else {
			wrap_trap7(data_155c[d0 >> 10], d0 & 0xff, d1);
		}
	}
	return 0;
}

int QueueEffect(u16 arg1, u16 arg2) {		/* 21e2 was cqsave */
    if (g.mode0 == 2 && g.mode1 == 4 && g.mode2 == 0xa && (arg2 & 0xff00)) {
        return g.libsplatter;
    } else {
        g.effectQueue[g.effectNext+0] = arg1;
        g.effectQueue[g.effectNext+1] = arg2;
        g.effectNext += 2;
        return g.effectNext;
    }
}


void player_check_dizzy(Player *ply) {	/* 377c test player dizzying */
	if (ply->DizzyFall == FALSE) {
		ply->DizzyCount += 10 + (char []) {			// data chars @ 0x37ba
			1, -1,  2,  0,  1,  0,  0, -2,  
			0,  1, -1, -2,  0,  0,  2,  0,
			3, -3, -1,  0,  0,  1,  0,  1, 
		   -1,  0, -2,  2, -2,  2,  0, -1,
		}[RAND32];
		ply->DizzyClearCnt += 0x64;
		if(ply->DizzyCount <= 0x1e) { return; }
	}
	ply->DizzyFall = TRUE;
	ply->DizzyCount = 0;
	ply->DizzyClearCnt = 0;
}


u16 *objcoords_scroll1(Object *obj) {
	return coords_scroll1(obj->XPI, obj->YPI);
}
u16 *objcoords_scroll2(Object *obj) {
	return coords_scroll2(obj->XPI, obj->YPI);
}
u16 *objcoords_scroll3(Object *obj) {
	return coords_scroll3(obj->XPI, obj->YPI);
}

/* coords routines, return gfx_p in a0 */

u16  *coords_scroll1(short x, short y){	/* 40bc: defines SCR1_CURSOR_SET */
	// u16       yxxx xxxy yyyy[2]		2y x 64y x 32y x 2words x u16  tiles 8x8
	// void 00yx xxxx xyyy yy00			rowmask 0x1f80
	y = ~y;
	return gemu.Tilemap_Scroll1[ ((y & 0x100)<<3) | ((y & 0xf8)>>3) | ((x & 0x1f8)<<2) ];
}
u16 *coords_scroll2(short x, short y){	/* 40e8 */
	// u16       yyxx xxxx yyyy[2]    4y * 64x * 16y * 2words * u16  tiles 16x16
	// void 00yy xxxx xxyy yy00		  rowmask 0x0fc0
	y = ~y;
	return gemu.Tilemap_Scroll2[((y & 0x300 )<< 2) + (x & 0x3f0) + ((y & 0xf0) >> 4)];
}
u16 *coords_scroll3(short x, short y){	/* 4114 */
	// u16       yyyx xxxx xyyy[2]    8y * 64x * 8y * 2words * u16  tiles 32x32
	// void 00yy yxxx xxxy yy00       rowmask 0x03e0
	y = ~y;
	return gemu.Tilemap_Scroll3[  ((y & 0x700)<<1) | ((y & 0xe0)>>5) | ((x & 0x7e0)>>2) ];
}


u32 MakePointObj (int x, int y) {		/* 50ac */
    x += 0x40;
    y ^= 0xff;
    y++;
    return (x << 16) + y;
}

void LBAddPoints(int d0, short d1) {		// 55c2
	Player *ply;

	ply = d1==0 ? PLAYER1 : PLAYER2;
	if (ply->Human) {
		add_bcd_32(data_530c[d0/2], &ply->BonusScore);
	}
	if (ply->Score > 0x09999999L) {
		ply->Score = 0x09999999L;
	}
}

#ifndef CPS_COMPAT
// XXX If we're not on CPS, why don't we use stdlib/memclear?
void memclear(void *c, int len) {
	int i;
	char *cur = (char *)c;
	for (i=0; i<len; i++) {
		*cur++=0;
	}
}
#endif

#pragma mark ---- Resetter Functions ----

void clear_players(void) {
    memclear((char *)PLAYER1, (void *)(&PLAYER1->Alive) - (void *)(&PLAYER1->exists));	
    memclear((char *)PLAYER2, (void *)(&PLAYER2->Alive) - (void *)(&PLAYER2->exists));
}
static void clear_gstates(void) {			/* 0x2944 */
    memclear(&gstate_Scroll1, sizeof(GState));
    memclear(&gstate_Scroll2, sizeof(GState));
    memclear(&gstate_Scroll3, sizeof(GState));
}
void clear_gsrowscroll(void) {
    memclear (&gstate_RowScroll, sizeof(ScrollState));
}
static void clear_scrolls23() {			/* 0x29b0 */
	memclear(&gstate_Scroll2, sizeof(GState));
	memclear(&gstate_Scroll3, sizeof(GState));
}

void resetstate_B(void) {
    clear_scrolls23();     /* clear gstate bd2 */
    _LBResetState();
}
               
void LBResetState(void) {		// 2794
    clear_gstates();  
    _LBResetState();
}

static void _LBResetState(void) {
    int i;
    
    clear_players();
    clear_gstates();
    sub_297a();
    clear_playerselect();
    
    memclear(&g.x5dfe, sizeof(Object));	    
    g.FreeShadows = 2;
    memclear (&g.Ply1Shadow, sizeof(Object));
    memclear (&g.Ply2Shadow, sizeof(Object));
    g.Ply1Shadow.SubSel = 0;
    g.Ply2Shadow.SubSel = 1;
    
    g.FreeLayer1 = COUNT_LAYER1;    
    g.FreeLayer2 = COUNT_LAYER2;
    g.FreeLayer3 = COUNT_LAYER3;
    
    for(i=0; i<COUNT_LAYER1; i++) {
        memclear(&g.Objects1[i], sizeof(Object));
        g.Objects1[i].Layer = GFX_LAYER1;
        g.FreeStack_Layer1[ i ] = &g.Objects1[i];
    }	
    for(i=0; i<COUNT_LAYER2; i++) {
        memclear(&g.Objects2[i], sizeof(Object));
        g.Objects2[i].Layer = GFX_LAYER2;
        g.FreeStack_Layer2[ i ] = &g.Objects2[i];
    }
    for(i=0; i<COUNT_LAYER3; i++) {
        memclear(&g.Objects3[i], sizeof(Object));
        g.Objects3[i].Layer = GFX_LAYER3;
        g.FreeStack_Layer3[ i ] = &g.Objects3[i];
    }
	
    g.Layer1Grp1Cnt = g.Layer1Grp2Cnt = 0;
    g.Layer1Grp1Cnt = g.Layer1Grp2Cnt = 0;
    
    
    g.Layer2Grp1Cnt = 
    g.Layer2Grp2Cnt = 
    g.Layer2Grp3Cnt = 0;
    
    g.Layer3Grp1Cnt = 
    g.Layer3Grp2Cnt = 
    g.Layer3Grp3Cnt = 
    g.Layer3Grp4Cnt = 
    g.Layer3Grp5Cnt = 
    g.Layer3Grp6Cnt = 0; 
}


static void clear_playerselect(void) {			//2968
	memclear(&g.PLSL,sizeof(struct state_playerselect));
}

static void sub_297a(void) {
	/* XXX reset the 5d0c stack */
}

#pragma mark ---- Decoding ----

void decode_buttons(Player *ply, short d0) {		/* 3296 */
/* only valid if a button is actually pressed, otherwise will always
 result in Big Kick */
	
	ply->PunchKick = PLY_PUNCHING;
	if (d0 & BUTTON_A) {
		ply->ButtonStrength = STRENGTH_LOW;
		return;
	} else if (d0 & BUTTON_B) {
		ply->ButtonStrength = STRENGTH_MED;
		return;
	} else if (d0 & BUTTON_C) {
		ply->ButtonStrength = STRENGTH_HIGH;
		return;
	}
	ply->PunchKick = PLY_KICKING;
	if (d0 & BUTTON_D) {
		ply->ButtonStrength = STRENGTH_LOW;
		return;
	} else if (d0 & BUTTON_E) {
		ply->ButtonStrength = STRENGTH_MED;
		return;
	} else {
		ply->ButtonStrength = STRENGTH_HIGH;
		return;
	}
}
short buttonspressed(Player *ply, u16 d1) {		/* 32c8 */
	return ply->JoyDecode.full & ~ply->JoyDecodeDash.full & d1;
}
short buttonsreleased(Player *ply, u16 d1) {	/* 32d6 */
	return ply->JoyDecodeDash.full & ~ply->JoyDecode.full & d1;
}
void set_towardsaway(Player *ply) {       /* 318a */
    if(ply->JoyDecode.full & JOY_RIGHT) {
        ply->Step = STEP_RIGHT; 
    } else if (ply->JoyDecode.full & JOY_LEFT) {     
        ply->Step = STEP_LEFT;
    } else {
        ply->Step = STEP_STILL;
    }
}

#pragma mark ---- Object Alloc/Dealloc ----

Object *pop_1174(void) {				/* 0x29c2 */
    if(g.FreeLayer2 == 0) { return NULL; }
    return g.FreeStack_Layer2[--g.FreeLayer2];
}
void clearpush_1174(Object *obj) {
	obj->exists = obj->flag1 = obj->mode0 = obj->mode1 = obj->mode2 = obj->mode3 = 0;
	obj->ZDepth = 0;
	obj->Pool = obj->Step = obj->Flip = obj->Draw1 = 0;
	obj->Sel = obj->SubSel = 0;
	obj->x0024 = obj->Scroll = SCROLL_2;
	obj->OnGround = 0;
	
    g.FreeStack_Layer2[g.FreeLayer2++] = obj;    
}
Object *AllocProjectile(void) {				/* 0x2a0c */
    if(g.FreeLayer1 == 0) { return NULL; }
    return g.FreeStack_Layer1[--g.FreeLayer1];
}
void FreeProjectile(Object *obj) {
	obj->exists = obj->flag1 = obj->mode0 = obj->mode1 = obj->mode2 = obj->mode3 = 0;
	obj->ZDepth = 0;
	obj->Pool = obj->Step = obj->Flip = obj->Draw1 = 0;
	obj->Sel = obj->SubSel = 0;
	obj->x0024 = 0;
	obj->Scroll = SCROLL_2;
	obj->OnGround = 0;
	
    g.FreeStack_Layer1[g.FreeLayer1++] = obj;    
}
Object *AllocActor(void) {		// was pop-117c
	if(g.FreeLayer3 == 0) { return NULL; }
    return g.FreeStack_Layer3[--g.FreeLayer3];
}
void FreeActor(Object *obj){
	obj->exists = obj->flag1 = obj->mode0 = obj->mode1 = obj->mode2 = obj->mode3 = 0;
	obj->ZDepth = 0;
	obj->Pool = obj->Step = obj->Flip = obj->Draw1 = 0;
	obj->Sel = obj->SubSel = 0;
	obj->x0024 = 0;
	obj->Scroll = SCROLL_2;
	obj->OnGround = 0;
	
	g.FreeStack_Layer3[g.FreeLayer3++] = obj;
}
Object2 *pop_5d0c(void) {
	if(g.FreeCount_530a == 0) { return NULL; }
    return g.FreeStack_530a[--g.FreeCount_530a];
}
void clearpush_5d0c(Object2 *obj){
	obj->exists = obj->flag1 = obj->mode0 = obj->mode1 = obj->mode2 = obj->mode3 = 0;
	obj->ZDepth = 0;
	obj->Pool = obj->Step = obj->Flip = obj->Draw1 = 0;
	obj->Sel = obj->SubSel = 0;
	obj->x0024 = 0;
	obj->Scroll = SCROLL_2;
	obj->OnGround = 0;
	
	g.FreeStack_530a[g.FreeCount_530a++] = obj;
}

#pragma mark ---- graphics ----

static void sub_52bc(u16 *gp, short x, short y, char c) {
	OBJECT_DRAW_NOATTR(gp, x, y, (c & 0xf) + SF2_TILE_OBJ_HEXCHARS);	
	/* print the object in both both buffers */
}
static void _LBPrintTicker(u32 *coords, u8 a) {		//52ac print time remaining
	u16 *gp;
	OBJ_CURSOR_SET(gp, 14);
	sub_52bc(gp, *coords >> 16, *coords & 0xffff,a >> 4);
	INC_GFX_CURSOR(coords, 16, 0);
	OBJ_CURSOR_SET(gp, 15);
	sub_52bc(gp, *coords >> 16, *coords & 0xffff,a );
}
void sub_528a() {		/* print number of barrels remaining */
	u32 coords=MakePointObj(176, 208);
	_LBPrintTicker(&coords, g.x8ab9);		
}
static void sub_529c() {
	u32 coords = MakePointObj(176, 208);
	_LBPrintTicker(&coords,g.TimeRemainBCD);
}
static void ply1_loses(void) {		/* 0x8f7a */
	print_timeremaining();
	g.RoundResult = 2;
	g.Player2.RoundsWon++;
	if(g.InDemo) { return; }
	g.WinningFighter  = g.Player2.FighterID;
	g.LosingFighter   = g.Player1.FighterID;
	g.HumanWinner     = g.Player2.x02ae | g.Player2.Human;
	if(g.HumanWinner == 0) { g.ComputerWon = TRUE; }
	g.HumanLoser      = g.Player1.x02ae | g.Player1.Human;
	g.RoundWinnerSide = g.Player2.Side;
	g.RoundLoserSide  = g.Player1.Side;
	g.NewChallengerWait = TRUE;
	if(g.JapanJumper == 0) {
		if(g.OnFinalStage && (g.Player1.x02ae || g.Player1.Human) && g.Player2.RoundsWon == 2) {
			g.SkipEnding = FALSE;
			g.BattleOver = TRUE;
		}
		return;
	}
	if(g.x031e == 0) { return; }
	if(g.Player1.x02ae | g.Player1.Human) {
		if(g.Player2.RoundsWon == 2) {
			g.SkipEnding = TRUE;
			g.BattleOver = TRUE;
		}
		
	}
}
static void ply2_loses(void) {		/* 0x8ed6 */
	print_timeremaining();
	g.RoundResult = 1;
	g.Player1.RoundsWon++;
	if(g.InDemo) { return; }
	g.WinningFighter = g.Player1.FighterID;
	g.LosingFighter  = g.Player2.FighterID;
	g.HumanWinner    = g.Player1.x02ae | g.Player1.Human;
	if(g.HumanWinner == 0) { g.ComputerWon = TRUE; }
	g.HumanLoser      = g.Player2.x02ae | g.Player2.Human;
	g.RoundWinnerSide = g.Player1.Side;
	g.RoundLoserSide  = g.Player2.Side;
	g.NewChallengerWait = TRUE;
	if(g.JapanJumper) {
		if(g.OnFinalStage && (g.Player2.x02ae || g.Player2.Human) && g.Player1.RoundsWon == 2) {
			g.SkipEnding = FALSE;
			g.BattleOver = TRUE;
		}
		return;
	}
	if(g.x031e == 0) { return; }
	if(g.Player2.x02ae | g.Player2.Human) {
		if(g.Player1.RoundsWon == 2) {
			g.SkipEnding = TRUE;
			g.BattleOver = TRUE;
		}
	
	}
}

void startgame(int players_online) {	/* 6d4e */
	g.PlayersOnline = players_online;
	/* XXX kill task 6; */
	ClearEffectQueue();
	die_top8();
	// todo sub_62ec();
	g.mode0	=	g.timer0 =
	g.mode1 =	g.timer1 = 
	g.mode2 =	g.timer2 = 
	g.mode3 =	g.timer3 = 
	g.mode4 =	g.timer4 =
	g.mode5 =	g.timer5 = 0;
	g.SkipEnding		= FALSE;
	g.x02eb				= FALSE;
	g.FastEndingFight	= FALSE;
	g.OnFinalStage		= FALSE;
	g.x02ec				= FALSE;
	g.InDemo			= FALSE;
	g.ActiveHumans		= BOTH_COMPUTER;
	g.NumberCreditsDash = FALSE;
	g.NewChallengerWait = TRUE;
	g.Player1.BlinkerMode0 = 0;
	g.Player2.BlinkerMode0 = 0;
	g.Player1.Alive = FALSE;
	g.Player2.Alive = FALSE;
	g.Player1.Human = FALSE;
	g.Player2.Human = FALSE;
	g.Player1.x02ae = 0;
	g.Player2.x02ae = 0;
	g.Player1.SelectComplete = FALSE;
	g.Player2.SelectComplete = FALSE;
	g.Player1.Side = 0;
	g.Player2.Side = 1;
	g.Player1.Opponent = PLAYER2;
	g.Player2.Opponent = PLAYER1;
	g.Player1.FighterID = 0;
	g.Player2.FighterID = 0;
	g.Player1.Continuing = FALSE;
	g.Player2.Continuing = FALSE;
	if (g.PlayersOnline & 0x2) {
		g.Player2.Alive = 2;
		g.Player2.BlinkerMode0 = 4;
		g.Player2.Human = TRUE;
	}
	g.Player1.Alive = 2;
	g.Player1.BlinkerMode0 = 4;
	g.Player1.Human = TRUE;
	
	fadenwait1();
	
	
	TASK_CREATE(task_blinkers, 3, 0, 0, 0);
	TASK_CREATE(task_game, 4, 0, 0, 0);
	task_die();
}
	
void wait_for_ply_PSFinishedParticipating(void) {  /* 0x9048 */
	if(g.Player1.PSFinishedParticipating && g.Player2.PSFinishedParticipating) {
		g.RoundComplete = TRUE;
	}
}
	
short sub_2fe6(Player *ply, Object *obj, short yoke) {	
	short d0, d1,d4;
	
	if(ply->Size + 16 > ABS(ply->XPI - obj->XPI)) { return 1; }
		
	if(obj->Layer==0) {
		d4 = ((Player *)obj)->Size;
	} else {
		d4 = 0;
	}
	if(obj->Flip) {d4 = -d4;}
	d4 += obj->XPI;
	
	d0 = ply->XPI - d4;
	if(d0 < 0) {
		d1 = 0;
		d0 = -d0;
	} else {
		d1 = 1;
	}
	if(data_995de[ply->FighterID][yoke] < d0) { return 0;}	// XXX checkme
	if(obj->Flip != d1) { return 0; }
	return 1;
}

#pragma mark ---- BCD emulation ----

#ifdef GUSTY_LOBSTER
void add_bcd_32(int op, u32 *bcd) {			/* not SF2 code */
	int t1, t2, t3, t4, t5, t6;
	t1 = op + 0x06666666;
	t2 = t1 + *bcd;
	t3 = t1 ^ *bcd;
	t4 = t2 ^ t3;
	t5 = ~t4 & 0x11111110;
	t6 = (t5 >> 2) | (t5 >> 3);
	*bcd = t2 - t6;
}
void sub_bcd_32(int op, u32 *bcd) {
	int t1,t2,t3,t4,t5,t6;
	t1 = 0xffffffff - op;
	t2 = - op;
	t3 = t1 ^ 0x00000001;
	t4 = t2 ^t3;
	t5 = ~t4 & 0x11111110;
	t6 = (t5 >> 2) | (t5 >> 3);
	add_bcd_32(t2 - t6, bcd);
}
void add_bcd_16(u16 op, u16 *bcd) {
	u32 arg = *bcd;
	add_bcd_32((u32) op, &arg);
	*bcd = arg;
}
void sub_bcd_16(u16 op, u16 *bcd) {
	u32 arg = *bcd;
	sub_bcd_32((u32) op, &arg);
	*bcd = arg;
}
void add_bcd_8(u8 op, u8 *bcd) {
	u32 arg = *bcd;
	add_bcd_32((u32) op, &arg);
	*bcd = arg;
}
void sub_bcd_8(u8 op, u8 *bcd) {
	u32 arg = *bcd;
	sub_bcd_32((u32) op, &arg);
	*bcd = arg;
}
#else
#error No CPS BCD glue yet!
#endif


// move to gfxlib.c
static void proc_round_timer(void) {	/* 905c process round timers */
	if(g.TimeRemainBCD == 0 && g.TimeRemainTicks == 0) { return; }
	if(g.DisableTimer || g.RoundResult) { return; }
	if(g.TimeRemainBCD <= (g.OnBonusStage ? 0x10 : 0x15)) { 
		sub_90c8();							/* start the timeremain counter flashing */
	}
	if(g.Debug && (g.JPCost & JP_DBGNOTICK)) { return; }
	if(--g.TimeRemainTicks) {return;}
	
	g.TimeRemainTicks = SF2_GAME_TICKS;		// not real seconds, two thirds of a second.
	if (g.TimeRemainBCD > 0) {				/* buggered slightly from original code */
		sub_bcd_8(1, &g.TimeRemainBCD);
		sub_529c();							/* update time remaining display */
	} else {
		g.TimeRemainBCD = 0;
		g.NewChallengerWait = TRUE;
		g.TimeOut = TRUE;
		print_timeremaining();
	}
}
static void decide_timeout_result(void) {   /* 0x901e */
	if(g.Player1.Energy < g.Player2.Energy) {
		ply1_loses();
	} else if (g.Player1.Energy == g.Player2.Energy) {
		/* 0x9030 */
		g.RoundResult = -1;
		g.TimeResult  = 1;
		if(g.Player1.Energy < 0) { g.TimeResult = -1; }  
	} else {
		ply2_loses();
	}
}
/* check if time or bonus stage expired */
void LBCheckRoundResult(void) {
	if(g.OnBonusStage) {
		/* 0x913a */
		if(g.Player1.x02ae || g.Player2.x02ae) {
			/* 0x9160 */
			g.TimeOut = TRUE;
			g.RoundResult = -1;
			return;
		} else {
			if(g.CurrentStage == STAGE_BONUS_BARRELS) { /* sub_91c8(); XXX */  return; }
			if(g.CurrentStage == STAGE_BONUS_CAR ||
			   g.CurrentStage == STAGE_BONUS_DRUMS) {
				/* 0x916e */
				if(g.RoundResult) {
					wait_for_ply_PSFinishedParticipating();	/* sets g.RoundComplete */
					return;
				}
				proc_round_timer();
				if(g.TimeOut == 0) {
					if(g.CarWasted == FALSE) { return; }
					queuesound(SOUND_PERFECT);
					print_libtextgfx(PERFECT);
				}
				if(g.Player1.BonusScore = g.Player2.BonusScore) {
					g.RoundResult = -1;
					print_timeremaining();
					return;
				}
				if(g.Player1.BonusScore < g.Player2.BonusScore) {
					g.RoundResult = 2;
				} else {
					g.RoundResult = 1;
				}
				print_timeremaining();
				return;
			}
			return;
		}
	} else {
		if (g.RoundResult != 0) {
			wait_for_ply_PSFinishedParticipating();
			return;
		}
		proc_round_timer();
		if(g.TimeOut) { decide_timeout_result(); return;}
		if(g.Player2.Energy >= 0) {
			if(g.Player1.Energy < 0) {
				ply1_loses();
			}
			return;
		}
		if(g.Player1.Energy >= 0) {
			ply2_loses();
			return;
		}
		g.RoundResult = -1;
		g.TimeResult = -1;
		g.NewChallengerWait = TRUE;
		print_timeremaining();
	}
}		

void setup_stage_actions (void) { /* 822be */
    int i;
	const static short counts[]={ 4, 10, 17, 7, 13, 10, 2, 2, 7, 4, 24, 16, 0, 8, 1, 11, };
	static const struct actionhdr data_82312[]={
		{0x08, 0x02, 0x01, 0x00, 0x04, 0x00, 0x000f, 0x0260, 0x0070},
		{0x08, 0x01, 0x01, 0x00, 0x00, 0x00, 0x000f, 0x02b8, 0x007c},
		{0x06, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0000, 0x0170, 0x0028},
		{0x06, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0000, 0x0390, 0x0028},
		{0x08, 0x00, 0x11, 0x00, 0x00, 0x00, 0x000f, 0x0000, 0x0000},
		
	};
	static const struct actionhdr data_82350[]={
		{0x08, 0x00, 0x0a, 0x00, 0x04, 0x00, 0x0000, 0x02f0, 0x00b0},
		{0x08, 0x03, 0x01, 0x00, 0x04, 0x00, 0x0000, 0x0170, 0x0050},
		{0x08, 0x00, 0x0b, 0xc0, 0x00, 0x00, 0x0000, 0x0273, 0x00e4},
		{0x08, 0x00, 0x0b, 0xc2, 0x00, 0x00, 0x0000, 0x0173, 0x00e4},
		{0x08, 0x00, 0x0b, 0xc4, 0x00, 0x00, 0x0000, 0x0333, 0x00e4},
		{0x08, 0x01, 0x0b, 0x60, 0x00, 0x00, 0x0000, 0x0283, 0x00d0},
		{0x08, 0x01, 0x0b, 0x62, 0x00, 0x00, 0x0000, 0x0243, 0x00d0},
		{0x08, 0x01, 0x0b, 0x64, 0x00, 0x00, 0x0000, 0x02e3, 0x00d0},
		{0x08, 0x04, 0x33, 0x00, 0x00, 0x06, 0x0041, 0x01c6, 0x0039},
		{0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0034, 0x03b0, 0x0110},
		{0x08, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0060, 0x0208, 0x0068},
		
	};
	static const struct actionhdr data_823d6[]={
		{0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0000, 0x03cc, 0x00d7},
		{0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0000, 0x0208, 0x0098},
		{0x08, 0x01, 0x08, 0x00, 0x00, 0x00, 0x0000, 0x0178, 0x0098},
		{0x08, 0x02, 0x08, 0x00, 0x04, 0x00, 0x0000, 0x0230, 0x0090},
		{0x08, 0x03, 0x08, 0x00, 0x04, 0x00, 0x0000, 0x02f0, 0x0070},
		{0x08, 0x04, 0x08, 0x00, 0x04, 0x00, 0x0000, 0x0330, 0x0070},
		{0x08, 0x05, 0x08, 0x00, 0x04, 0x00, 0x0000, 0x0350, 0x0070},
		{0x08, 0x06, 0x08, 0x00, 0x04, 0x00, 0x0000, 0x0370, 0x0050},
		{0x08, 0x07, 0x08, 0x00, 0x04, 0x00, 0x0000, 0x0370, 0x0070},
		{0x08, 0x08, 0x08, 0x00, 0x04, 0x00, 0x0000, 0x0390, 0x0070},
		{0x08, 0x06, 0x01, 0x00, 0x04, 0x00, 0x0000, 0x0220, 0x0040},
		{0x08, 0x07, 0x01, 0x00, 0x04, 0x00, 0x0000, 0x02d0, 0x0040},
		{0x08, 0x08, 0x01, 0x00, 0x04, 0x00, 0x0000, 0x0310, 0x0040},
		{0x08, 0x09, 0x01, 0x00, 0x04, 0x00, 0x0000, 0x0380, 0x0040},
		{0x08, 0x03, 0x33, 0x00, 0x00, 0x06, 0x003c, 0x0210, 0x003c},
		{0x08, 0x03, 0x33, 0x00, 0x00, 0x06, 0x003c, 0x02c8, 0x003c},
		{0x08, 0x03, 0x33, 0x00, 0x00, 0x02, 0x0014, 0x0180, 0x000c},
		{0x08, 0x03, 0x33, 0x00, 0x00, 0x02, 0x0014, 0x03a0, 0x000c},
		
	};
	static const struct actionhdr data_824b0[]={
		{0x08, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0000, 0x01d0, 0x0090},
		{0x08, 0x01, 0x00, 0x00, 0x04, 0x00, 0x0000, 0x0230, 0x0070},
		{0x08, 0x02, 0x00, 0x00, 0x04, 0x00, 0x0000, 0x02d0, 0x0090},
		{0x08, 0x03, 0x00, 0x00, 0x04, 0x00, 0x0000, 0x0270, 0x0070},
		{0x08, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00bf, 0x0248, 0x00c0},
		{0x08, 0x01, 0x19, 0x00, 0x00, 0x00, 0x00bf, 0x0358, 0x00c0},
		{0x06, 0x00, 0x07, 0x01, 0x00, 0x00, 0x0000, 0x03a0, 0x0028},
		{0x06, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0000, 0x0160, 0x0028},
		
	};
	static const struct actionhdr data_82512[]={
		{0x08, 0x0a, 0x01, 0x00, 0x00, 0x00, 0x004b, 0x02d8, 0x004b},
		{0x08, 0x0a, 0x01, 0x00, 0x00, 0x00, 0x004b, 0x0220, 0x004b},
		{0x08, 0x0b, 0x01, 0x00, 0x00, 0x00, 0x0020, 0x01e9, 0x001f},
		{0x08, 0x0b, 0x01, 0x00, 0x00, 0x00, 0x0020, 0x030f, 0x001f},
		{0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0000, 0x03b8, 0x0028},
		{0x08, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0000, 0x0338, 0x00e8},
		{0x08, 0x01, 0x05, 0x00, 0x04, 0x00, 0x0000, 0x01d0, 0x00f0},
		{0x08, 0x02, 0x05, 0x00, 0x04, 0x00, 0x0000, 0x0270, 0x0090},
		{0x08, 0x03, 0x05, 0x00, 0x04, 0x00, 0x0000, 0x01d0, 0x0090},
		{0x08, 0x04, 0x05, 0x00, 0x04, 0x00, 0x0000, 0x0210, 0x0090},
		{0x08, 0x05, 0x05, 0x00, 0x04, 0x00, 0x0000, 0x01b0, 0x00d0},
		{0x08, 0x06, 0x05, 0x00, 0x04, 0x00, 0x0000, 0x0230, 0x0070},
		{0x08, 0x07, 0x05, 0x00, 0x04, 0x00, 0x0000, 0x01f0, 0x0090},
		{0x08, 0x00, 0x03, 0x00, 0x04, 0x00, 0x0000, 0x0240, 0x0050},
		
	};
	static const struct actionhdr data_825bc[]={
		{0x08, 0x00, 0x15, 0x00, 0x00, 0x00, 0x0000, 0x0178, 0x0098},
		{0x08, 0x01, 0x15, 0x00, 0x00, 0x00, 0x0000, 0x01e8, 0x0098},
		{0x08, 0x02, 0x15, 0x00, 0x00, 0x00, 0x0000, 0x0258, 0x0078},
		{0x08, 0x03, 0x15, 0x00, 0x00, 0x00, 0x0000, 0x02c8, 0x0098},
		{0x08, 0x04, 0x15, 0x00, 0x00, 0x00, 0x0000, 0x0348, 0x0078},
		{0x08, 0x05, 0x15, 0x00, 0x00, 0x00, 0x0000, 0x0398, 0x00c8},
		{0x08, 0x06, 0x15, 0x00, 0x04, 0x00, 0x0000, 0x0250, 0x00f0},
		{0x08, 0x07, 0x15, 0x00, 0x04, 0x00, 0x0000, 0x0110, 0x00f0},
		{0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x05, 0x33, 0x00, 0x00, 0x02, 0x0017, 0x01c0, 0x0017},
		{0x08, 0x05, 0x33, 0x00, 0x00, 0x02, 0x0014, 0x037f, 0x0014},
		
	};
	static const struct actionhdr data_82642[]={
		{0x08, 0x05, 0x01, 0x00, 0x02, 0x00, 0x0000, 0x03b8, 0x00c0},
		{0x08, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0000, 0x0178, 0x00e8},
		{0x08, 0x00, 0x33, 0x00, 0x00, 0x06, 0x0045, 0x0291, 0x0045},
		
	};
	static const struct actionhdr data_82668[]={
		{0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x01, 0x33, 0x00, 0x00, 0x06, 0x002c, 0x014a, 0x002c},
		{0x08, 0x02, 0x33, 0x00, 0x00, 0x06, 0x002c, 0x03b0, 0x002c},
		
	};
	static const struct actionhdr data_8268e[]={
		{0x08, 0x00, 0x0d, 0x00, 0x04, 0x00, 0x0000, 0x01d0, 0x0070},
		{0x08, 0x01, 0x0d, 0x00, 0x04, 0x00, 0x0000, 0x01f0, 0x0070},
		{0x08, 0x02, 0x0d, 0x00, 0x04, 0x00, 0x0000, 0x02f0, 0x0090},
		{0x08, 0x03, 0x0d, 0x00, 0x04, 0x00, 0x0000, 0x0210, 0x0070},
		{0x08, 0x03, 0x0d, 0x01, 0x04, 0x00, 0x0000, 0x0350, 0x0070},
		{0x08, 0x04, 0x0d, 0x00, 0x04, 0x00, 0x0000, 0x01b0, 0x0070},
		{0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0030, 0x0160, 0x0030},
		{0x06, 0x01, 0x03, 0x00, 0x00, 0x00, 0x0030, 0x0360, 0x0030},
		
	};
	static const struct actionhdr data_826f0[]={
		{0x08, 0x00, 0x14, 0x00, 0x04, 0x00, 0x0000, 0x0310, 0x00b0},
		{0x08, 0x01, 0x14, 0x00, 0x04, 0x00, 0x0000, 0x0340, 0x00c0},
		{0x08, 0x05, 0x33, 0x00, 0x00, 0x06, 0x003f, 0x030d, 0x003f},
		{0x08, 0x05, 0x33, 0x00, 0x00, 0x02, 0x0015, 0x038f, 0x0015},
		{0x08, 0x05, 0x33, 0x00, 0x00, 0x02, 0x0015, 0x01cf, 0x0015},
		
	};
	static const struct actionhdr data_8272e[]={
		{0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x05, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x08, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x09, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x0b, 0x00, 0x00, 0x02, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x0c, 0x00, 0x00, 0x02, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x0d, 0x00, 0x00, 0x02, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x04, 0x01, 0x00, 0x04, 0x00, 0x0000, 0x02e0, 0x0040},
		{0x08, 0x04, 0x01, 0x01, 0x04, 0x00, 0x0000, 0x0300, 0x0040},
		{0x08, 0x00, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x01b0, 0x0090},
		{0x08, 0x01, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x01b0, 0x0070},
		{0x08, 0x02, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x01f0, 0x0090},
		{0x08, 0x03, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x0210, 0x0090},
		{0x08, 0x04, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x0230, 0x0090},
		{0x08, 0x05, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x0270, 0x0090},
		{0x08, 0x06, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x02b0, 0x00a0},
		{0x08, 0x07, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x0330, 0x0090},
		{0x08, 0x08, 0x06, 0x00, 0x04, 0x00, 0x0000, 0x02f0, 0x0090},
		
	};
	static const struct actionhdr data_8285c[]={
		{0x0c, 0x10, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x11, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x12, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0150, 0x00b0},
		{0x08, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0210, 0x00b0},
		{0x08, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0390, 0x00b0},
		{0x08, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x03b0, 0x00b0},
		{0x08, 0x01, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0190, 0x00b0},
		{0x08, 0x01, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0310, 0x00b0},
		{0x08, 0x02, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x01b0, 0x00b0},
		{0x08, 0x02, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0330, 0x00b0},
		{0x08, 0x03, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0230, 0x0090},
		{0x08, 0x03, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x02b0, 0x0090},
		{0x08, 0x04, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0270, 0x00d0},
		{0x08, 0x05, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0290, 0x00d0},
		{0x08, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x0000, 0x0158, 0x00a8},
		{0x08, 0x07, 0x0c, 0x00, 0x00, 0x00, 0x0000, 0x02f8, 0x00a8},
		
	};
	static const struct actionhdr data_8292a[]={
		{0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0000, 0x00c0, 0x0030},
		
	};
	static const struct actionhdr data_82938[]={
		{0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x00, 0x16, 0x00, 0x00, 0x00, 0x0000, 0x0008, 0x00d8},
		{0x08, 0x01, 0x16, 0x00, 0x00, 0x00, 0x0000, 0x00d8, 0x00d8},
		{0x08, 0x02, 0x16, 0x00, 0x00, 0x00, 0x0000, 0x00c8, 0x0090},
		{0x08, 0x03, 0x16, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x03, 0x16, 0x00, 0x00, 0x01, 0x0000, 0x0000, 0x0000},
		{0x08, 0x03, 0x16, 0x00, 0x00, 0x02, 0x0000, 0x0000, 0x0000},
		{0x08, 0x03, 0x16, 0x00, 0x00, 0x03, 0x0000, 0x0000, 0x0000},
		{0x08, 0x04, 0x16, 0x00, 0x00, 0x00, 0x0000, 0x0148, 0x00a0},
		
	};
	static const struct actionhdr data_829a6[]={
		{0x08, 0x01, 0x04, 0x00, 0x00, 0x00, 0x0000, 0x0008, 0x00e8},
		{0x08, 0x00, 0x42, 0x00, 0x00, 0x00, 0x0000, 0x0008, 0x00e8},
		
	};
	static const struct actionhdr data_829c0[]={
		{0x0c, 0x10, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x11, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x0c, 0x12, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x08, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0050, 0x00b0},
		{0x08, 0x09, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0010, 0x00b0},
		{0x08, 0x0a, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0070, 0x0090},
		{0x08, 0x0a, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x00f0, 0x0090},
		{0x08, 0x0b, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x00b0, 0x00d0},
		{0x08, 0x0c, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x00d0, 0x00d0},
		{0x08, 0x0d, 0x0c, 0x00, 0x04, 0x00, 0x0000, 0x0170, 0x00b0},
		{0x08, 0x0e, 0x0c, 0x00, 0x00, 0x00, 0x0000, 0x0008, 0x00a8},
		{0x08, 0x0f, 0x0c, 0x00, 0x00, 0x00, 0x0000, 0x0138, 0x00b0},
		
	};
	const struct actionhdr *data_stageactions[]={	// 8227e
		data_82312, 
		data_82350, 
		data_823d6, 
		data_824b0, 
		data_82512, 
		data_825bc, 
		data_82642, 
		data_82668, 
		data_8268e, 
		data_826f0, 
		data_8272e, 
		data_8285c, 
		data_8292a, 
		data_82938, 
		data_829a6, 
		data_829c0, 
	};
	
	Object *action;
	short count = counts[g.CurrentStage];
	if (count < 0) { return; }
	
	const struct actionhdr *data = data_stageactions[g.CurrentStage];
	for (i=0; i<count; ++i) {
		if (action = alloc_action_by_type(data[i].Type)) {
			action->exists   = TRUE;
            action->SubSel   = data[i].SubSel;
            action->Sel      = data[i].Sel;
            action->Step     = data[i].Step;
            action->Scroll   = data[i].Scroll;
            action->UserByte = data[i].UserByte;
            action->ZDepth   = data[i].ZDepth;
            action->XPI      = data[i].X;
            action->YPI      = data[i].Y;			
		}
	}
	
}
void actionlibrary(void) {
	
	const static short counts[]={ 10, 21, 2, 1, 1, 0, 1, 1, 1, 2, 0, 4, 0, -1, -1, -1, 2, 21, 1, -1, };
	static const struct actionhdr data_82d7c[]={
		{0x08, 0x08, 0x2f, 0x00, 0x04, 0x00, 0x0000, 0x0028, 0x0640},
		{0x08, 0x08, 0x2f, 0x01, 0x04, 0x00, 0x0000, 0x0028, 0x0640},
		{0x08, 0x08, 0x2f, 0x02, 0x04, 0x00, 0x0000, 0x0028, 0x0640},
		{0x08, 0x08, 0x2f, 0x03, 0x04, 0x00, 0x0000, 0x0028, 0x0640},
		{0x08, 0x06, 0x2f, 0x00, 0x04, 0x00, 0x0000, 0x0028, 0x0640},
		{0x08, 0x04, 0x2f, 0x00, 0x04, 0x00, 0x0000, 0x0028, 0x0640},
		{0x08, 0x02, 0x2f, 0x00, 0x04, 0x00, 0x0000, 0x0078, 0x060c},
		{0x08, 0x00, 0x2f, 0x00, 0x04, 0x00, 0x0000, 0x0110, 0x060c},
		{0x08, 0x00, 0x30, 0x00, 0x04, 0x00, 0x0000, 0x0040, 0x0010},
		{0x08, 0x00, 0x30, 0x00, 0x04, 0x00, 0x0000, 0x00c0, 0x0010},
		{0x08, 0x00, 0x30, 0x00, 0x04, 0x00, 0x0000, 0x0140, 0x0010},
		
	};
	static const struct actionhdr data_82e02[]={
		{0x08, 0x00, 0x1e, 0x01, 0x00, 0x00, 0x0000, 0x00c8, 0x008c},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x01, 0x0000, 0x00d8, 0x00b0},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x02, 0x0000, 0x0118, 0x0078},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x03, 0x0000, 0x0128, 0x009c},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x04, 0x0000, 0x0124, 0x00be},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x05, 0x0000, 0x00ac, 0x00b4},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x06, 0x0000, 0x0076, 0x00b4},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x07, 0x0000, 0x007d, 0x0090},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x08, 0x0000, 0x00a4, 0x007e},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x09, 0x0000, 0x00a4, 0x007e},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x0a, 0x0000, 0x0100, 0x00b4},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x0b, 0x0000, 0x003e, 0x00b9},
		{0x08, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x0000, 0x00b0, 0x0040},
		{0x08, 0x01, 0x1f, 0x00, 0x00, 0x00, 0x0000, 0x00d0, 0x0040},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x00, 0x0000, 0x0090, 0x0740},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x01, 0x0000, 0x00b0, 0x0740},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x02, 0x0000, 0x00d0, 0x0740},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x03, 0x0000, 0x00f0, 0x0740},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x04, 0x0000, 0x0090, 0x0720},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x05, 0x0000, 0x00b0, 0x0720},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x06, 0x0000, 0x00d0, 0x0720},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x07, 0x0000, 0x00f0, 0x0720},
		
	};
	static const struct actionhdr data_82f0c[]={
		{0x08, 0x00, 0x24, 0x00, 0x04, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x03, 0x25, 0x00, 0x04, 0x00, 0x0000, 0x00b9, 0x0770},
		{0x08, 0x04, 0x25, 0x00, 0x04, 0x00, 0x0000, 0x00bb, 0x0760},
		
	};
	static const struct actionhdr data_82f32[]={
		{0x08, 0x00, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		{0x08, 0x05, 0x24, 0x00, 0x00, 0x00, 0x0000, 0x04e0, 0x0750},
		
	};
	static const struct actionhdr data_82f4c[]={
		{0x08, 0x01, 0x24, 0x00, 0x00, 0x00, 0x0000, 0x0088, 0x0598},
		{0x08, 0x03, 0x24, 0x00, 0x00, 0x00, 0x0000, 0x00c0, 0x0550},
		
	};
	static const struct actionhdr data_82f66[]={
		{0x08, 0x02, 0x24, 0x00, 0x04, 0x00, 0x0000, 0x00b8, 0x0540},
		
	};
	static const struct actionhdr data_82f74[]={
		{0x08, 0x07, 0x24, 0x00, 0xff, 0x00, 0x0000, 0x00c0, 0x0088},
		{0x08, 0x04, 0x24, 0x00, 0x00, 0x00, 0x0000, 0x02a8, 0x0150},
		
	};
	static const struct actionhdr data_82f8e[]={
		{0x08, 0x07, 0x24, 0x00, 0xff, 0x00, 0x0000, 0x00c0, 0x0088},
		{0x08, 0x01, 0x25, 0x00, 0x04, 0x00, 0x0000, 0x0068, 0x0350},
		
	};
	static const struct actionhdr data_82fa8[]={
		{0x08, 0x07, 0x24, 0x00, 0xff, 0x00, 0x0000, 0x00c0, 0x0088},
		{0x08, 0x02, 0x25, 0x00, 0x04, 0x00, 0x0000, 0x02c0, 0x0370},
		
	};
	static const struct actionhdr data_82fc2[]={
		{0x08, 0x05, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x02c0, 0x0640},
		{0x08, 0x06, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x0300, 0x0650},
		{0x08, 0x06, 0x25, 0x01, 0x00, 0x00, 0x0000, 0x0280, 0x0650},
		
	};
	static const struct actionhdr data_82fe8[]={
		{0x08, 0x06, 0x24, 0x00, 0x00, 0x00, 0x0000, 0x0078, 0x0150},
		
	};
	static const struct actionhdr data_82ff6[]={
		{0x08, 0x0e, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x02e0, 0x0350},
		{0x08, 0x08, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x02a8, 0x0360},
		{0x08, 0x07, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x0300, 0x0380},
		{0x08, 0x07, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x02c0, 0x0380},
		{0x08, 0x07, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x0280, 0x0380},
		
	};
	static const struct actionhdr data_83034[]={
		{0x08, 0x0a, 0x25, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		
	};
	static const struct actionhdr data_83042[]={
		
	};
	static const struct actionhdr data_83044[]={
		
	};
	static const struct actionhdr data_83046[]={
		
	};
	static const struct actionhdr data_83048[]={
		{0x08, 0x08, 0x24, 0x00, 0x00, 0x00, 0x0000, 0x0088, 0x014e},
		{0x08, 0x08, 0x24, 0x00, 0x00, 0x01, 0x0000, 0x0130, 0x0146},
		{0x08, 0x09, 0x24, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000},
		
	};
	static const struct actionhdr data_8306e[]={
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x0000, 0x00c8, 0x008c},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x01, 0x0000, 0x00d8, 0x00b0},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x02, 0x0000, 0x0118, 0x0078},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x03, 0x0000, 0x0128, 0x009c},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x04, 0x0000, 0x0124, 0x00be},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x05, 0x0000, 0x00ac, 0x00b4},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x06, 0x0000, 0x0076, 0x00b4},
		{0x08, 0x00, 0x1e, 0x00, 0x00, 0x07, 0x0000, 0x007d, 0x0090},
		{0x08, 0x02, 0x1e, 0x00, 0x00, 0x08, 0x0000, 0x00a4, 0x007e},
		{0x08, 0x01, 0x1e, 0x00, 0x00, 0x09, 0x0000, 0x00a4, 0x007e},
		{0x08, 0x01, 0x1e, 0x00, 0x00, 0x0a, 0x0000, 0x0100, 0x00b4},
		{0x08, 0x01, 0x1e, 0x00, 0x00, 0x0b, 0x0000, 0x003e, 0x00b9},
		{0x08, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x0000, 0x00b0, 0x0040},
		{0x08, 0x01, 0x1f, 0x00, 0x00, 0x00, 0x0000, 0x00d0, 0x0040},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x00, 0x0000, 0x0090, 0x0740},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x01, 0x0000, 0x00b0, 0x0740},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x02, 0x0000, 0x00d0, 0x0740},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x03, 0x0000, 0x00f0, 0x0740},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x04, 0x0000, 0x0090, 0x0720},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x05, 0x0000, 0x00b0, 0x0720},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x06, 0x0000, 0x00d0, 0x0720},
		{0x08, 0x00, 0x2c, 0x00, 0x04, 0x07, 0x0000, 0x00f0, 0x0720},
		
	};
	static const struct actionhdr data_82d62[]={
		{0x08, 0x00, 0x39, 0x00, 0x00, 0x00, 0x0000, 0x00c0, 0x0048},
		{0x08, 0x01, 0x39, 0x00, 0x00, 0x00, 0x0000, 0x00c0, 0x0048},
		
	};
	static const struct actionhdr data_83178[]={
		
	};
	static const struct actionhdr *data_actions[]={ 
		data_82d7c, 
		data_82e02, 
		data_82f0c, 
		data_82f32, 
		data_82f4c,			//4
		data_82f66, 
		data_82f74, 
		data_82f8e, 
		data_82fa8,			//8
		data_82fc2, 
		data_82fe8, 
		data_82ff6, 
		data_83034,			//c
		data_83042, 
		data_83044, 
		data_83046, 
		data_83048,			//10
		data_8306e,			//11 world map
		data_82d62, 
		data_83178, 
	};
	
	
	int i;
	Object *action;
	short count = counts[g.ActionLibSel];
	if (count < 0) { return; }
	
	const struct actionhdr *data = data_actions[g.ActionLibSel];
	for (i=0; i< count; i++) {
		if (action = alloc_action_by_type(data[i].Type)) {
			action->exists = TRUE;
            action->SubSel = data[i].SubSel;
            action->Sel    = data[i].Sel;
            action->Step   = data[i].Step;
            action->Scroll = data[i].Scroll;
            action->UserByte  = data[i].UserByte;
            action->ZDepth  = data[i].ZDepth;
            action->XPI      = data[i].X;
            action->YPI      = data[i].Y;	
		}
	}
}

void _bumplevel(void) {		/* 2bf2 */
	while((g.CurrentStage = g.LevelScript[g.LevelCursor]) < 0) {
		/* 2b76 */
		++g.LevelCursor;
	}
	if(g.LevelScript[g.LevelCursor+1] == 0x10) {g.OnFinalStage = TRUE;}
	boss_level_check();
}

void boss_level_check (void) {			//2c1a
	if(g.LevelCursor >= 7) {
		g.UpToBosses = TRUE;
		if(g.LevelCursor == 8) {
			g.OnLevel8 = TRUE;		/* u8 */
		}
	}
}



#pragma mark Synthetics
// these are here mainly to keep things tidy and avoid modules needing to #include and extern globals
short get_scr1x() { return gstate_Scroll1.XPI; }
short get_scr1y() { return gstate_Scroll1.YPI; }
short get_scr2x() { return gstate_Scroll2.XPI; }
short get_scr2y() { return gstate_Scroll2.YPI; }
short get_scr3x() { return gstate_Scroll3.XPI; }
short get_scr3y() { return gstate_Scroll3.YPI; }



