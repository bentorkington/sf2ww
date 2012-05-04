/*
 *  aitests.c
 *  MT2
 *
 *  Created by Ben on 2/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#include "aitests.h"
#include "sf2const.h"
#include "sf2types.h"
#include "sf2macros.h"
#include "gemu.h"
#include "sf2io.h"
#include "structs.h"
#include "ai.h"
#include "computer.h"
#include "testlib.h"

#include	"lib.h"
#include	"gfxlib.h"

extern CPSGFXEMU gemu;
extern Game g;

const u8 *_test_ai_A2;
u8 _test_ai_D2;

extern struct dualptr *dataAIAggressive[];
extern const struct defense_strategy **dataAIDefensive[];

static void _test_ai_putmsg(const char *message);
static void _test_ai_checkall();
static void _test_ai_clearsave_check_strategy(Player *ply, int d0);
static int  _test_ai_clearsave_check_highai(Player *ply, int instr);
static int  _test_ai_check_highai(Player *ply, int instr);
static void _test_ai_check_strategy(Player *ply, int instr);


static void _test_ai_putbyte(int scr1c, int value) {			// 89e58
	u16 *scrp;
	SCR1_CURSOR_CPS(scrp, scr1c);
	SCR1_DRAW_TILE(scrp, (value >> 4), 0);
	SCR1_CURSOR_BUMP(scrp, 1, 0);
	SCR1_DRAW_TILE(scrp, (value & 0xf), 0);
}
static void _test_ai_dumpstatus(Player *ply) {					// 89d34
	_test_ai_putmsg("\x02\x10\x01ENEMY=\x00");
	_test_ai_putbyte(0x90c410, ply->FighterID);
	switch (g.mode1) {
		case 0:
			_test_ai_putmsg("\x02\x14\x00TAIKI\x00");
			_test_ai_putmsg("\x02\x18\x00TARGET=\x00");
			_test_ai_putbyte(0x90c498, ply->OpponentID);
			_test_ai_putmsg("\x02\x1c\x00LEVEL=\x00");
			_test_ai_putbyte(0x90c41c, ply->RoughTimeRemain);
			break;
		case 2:
			_test_ai_putmsg("\x02\x14\x00ATTACK\x00");
			_test_ai_putmsg("\x02\x18\x00TARGET=\x00");
			_test_ai_putbyte(0x90c498, ply->OpponentID);
			_test_ai_putmsg("\x02\x1c\x00LEVEL=\x00");
			_test_ai_putbyte(0x90c41c, ply->RoughTimeRemain);
			_test_ai_putmsg("\x02\x20\x00ATCK=\x00");
			_test_ai_putbyte(0x90c3a0, ply->AITypeAgg0);
		case 4:
			_test_ai_putmsg("\x02\x14\x00YOKE\x00");
			_test_ai_putmsg("\x02\x18\x00YOKE NO=\x00");
			_test_ai_putbyte(0x90c518, ply->x0203);
			_test_ai_putmsg("\x02\x1c\x00RND NO=\x00");
			_test_ai_putbyte(0x90c49c, ply->AITypeAgg1);
		default:
			break;
			
	}
	
}

static void _test_ai_setup_agg0(Player *ply) {			// 89a8a
	ply->AIStratAgg0 = dataAIAggressive[ply->FighterID][ply->OpponentID].a1;
	ply->AIStratIndexAgg0 = 0;
}
static void _test_ai_setup_agg1(Player *ply) {			// 89ad0
	ply->AIStratAgg1 = dataAIAggressive[ply->FighterID][ply->OpponentID].a2->x0134[
						dataAIAggressive[ply->FighterID][ply->OpponentID].a2->x023e[ply->RoughTimeRemain]
																				   ];
	ply->AIStratIndexAgg1 = 0;
}
static void _test_ai_setup_def(Player *ply) {			// 89b32
	ply->AIStratDef = dataAIDefensive[ply->FighterID][ply->x0203]->codes[ply->x020c & 0x3f];
	ply->AIStratIndexDef = 0;
}

static void _test_ai_sm(void) {				// 896f0
	Player *ply = &g.Player1;
	switch (g.mode0) {
		case 0:
			NEXT(g.mode0);
			g.CurrentStage = 0;
			// XXX CPS x0036 = 0;
			//     CPS x0038 = 0x100;
			g.CPS.DispEna = 0x06da;
			palettes_nextlevel();
			palette_scr1_16();
			LBResetState();
			test_fadescreenin();
			break;
		case 2:
			switch (g.mode1) {
				case 0:
					for (ply->FighterID = 0; ply->FighterID < 12; ply->FighterID++) {
						for (ply->OpponentID = 0; ply->OpponentID < 8; ply->OpponentID++) {
							for (ply->RoughTimeRemain = 0; ply->RoughTimeRemain < 8; ply->RoughTimeRemain++) {
								_test_ai_setup_agg0(ply);
								_test_ai_checkall(ply);						
							}
						}
					}
					NEXT(g.mode1);					
					break;
				case 2:
					for (ply->FighterID = 0; ply->FighterID < 12; ply->FighterID++) {
						for (ply->OpponentID = 0; ply->OpponentID < 8; ply->OpponentID++) {
							for (ply->RoughTimeRemain = 0; ply->RoughTimeRemain < 8; ply->RoughTimeRemain++) {
								for (ply->x020a=0; ply->x020a<0x20; ply->x020a++) {
									_test_ai_setup_agg1(ply);
									_test_ai_checkall(ply);
								}
							}
						}
					}
					NEXT(g.mode1);
					break;
				case 4:
					for (ply->FighterID = 0; ply->FighterID < 12; ply->FighterID++) {
						for (ply->x0203 = 0; ply->x0203<0x10; ply->x0203++) {
							for (ply->x020c = 0; ply->x020c<0x3f; ply->x020c++) {
								_test_ai_setup_def(ply);
								_test_ai_checkall(ply);
							}
						}
					}
					NEXT(g.mode0);
					g.mode1 = 0;
					break;
				FATALDEFAULT;
			}
			break;
		case 4:
			_test_ai_putmsg("\x02\x10\x00OK.\x00");
			NEXT(g.mode0);
		case 6:
			// does nothing
			break;
		FATALDEFAULT;
	}
}
static void _test_ai_checkexit(void) {			// 896d0
	g.libsplatter++;
	if (g.Debug) {
		// todo exit out.
	}
}
static void test_ai_main(void) {						// 896c2
	while (TRUE) {
		sf2sleep(1);
		_test_ai_sm();
		_test_ai_checkexit();
	}
}


static int _test_ai_getnextbyte(Player *ply) {					// 89c96
	// need to find d2
	if (ply->AISaveState) {
		switch (g.mode1) {
			case 0:
				return ply->AIStratAgg0[_test_ai_D2];
				_test_ai_D2++;
				break;
			case 2:
				return ply->AIStratAgg1[_test_ai_D2];
				_test_ai_D2++;
				break;
			case 4:
				return ply->AIStratDef[_test_ai_D2];
				_test_ai_D2++;
				break;
			FATALDEFAULT;
		}
	} else {
		switch (g.mode1) {
			case 0:
				return ply->AIStratAgg0[++ply->AIStratIndexAgg0];
				break;
			case 2:
				return ply->AIStratAgg1[++ply->AIStratIndexAgg1];
				break;
			case 4:
				return ply->AIStratDef[++ply->AIStratIndexDef];
				break;
			FATALDEFAULT;
		}
	}
	return 0;	// can't get here
}

// check return status
static void _test_ai_error_construct(Player *ply, int instr) {		// 89bf0
	switch (instr) {
		case 0x94:
			_test_ai_putmsg("\x02\x0c\x00NO YON !!\x00");
			break;
		case 0xa0:
			_test_ai_putmsg("\x02\x0c\x00NO KYON !!\x00");
			break;
		case 0xa4:
			_test_ai_putmsg("\x02\x0c\x00NO TAN !!\x00");
			break;
		case 0xa8:
			_test_ai_putmsg("\x02\x0c\x00NO GTAN !!\x00");
			break;
		case 0xb0:
			_test_ai_putmsg("\x02\x0c\x00NO TIGA  !!\x00");
			break;
		default:
			_test_ai_putmsg("\x02\x0c\x00NO END IF!!\x00");
			break;
	}
	_test_ai_dumpstatus(ply);
}
static void _test_ai_error_illegal(Player *ply) {				//8987e
	_test_ai_putmsg("\x02\x0c\x00ERROR ILLEGAL Inst\x00");
	_test_ai_dumpstatus(ply);
}
static void _test_ai_specialskip(Player *ply) {		// 898b4
	int instr = _test_ai_getnextbyte(ply);
	switch (instr) {
		case 0x81:
		case 0xc0:
			_test_ai_getnextbyte(ply);
			break;
		default:
			break;
	}
}
static void _test_ai_skipparams(Player *ply, int instr) {			// 8989c
	switch (instr) {
		case 0:
		case 2:
		case 6:
		case 0xe:
			_test_ai_getnextbyte(ply);
			break;
		case 4:
		case 0xc:
			_test_ai_getnextbyte(ply);
			_test_ai_getnextbyte(ply);
			break;
		case 8:
			_test_ai_getnextbyte(ply);
			_test_ai_getnextbyte(ply);
			_test_ai_getnextbyte(ply);
			_test_ai_getnextbyte(ply);
			break;
		case 0xa:
			_test_ai_getnextbyte(ply);
			_test_ai_getnextbyte(ply);
			_test_ai_getnextbyte(ply);
			break;			
		FATALDEFAULT;
	}
}



static void _test_ai_clearsave_check_strategy(Player *ply, int instr) {		// 89870
	ply->AISaveState = 0;
	_test_ai_check_strategy(ply, instr);
}
static void _test_ai_check_strategy(Player *ply, int instr) {				// 89874
	if (instr == 0) {
		_test_ai_specialskip(ply);
	} else if (instr < 0xf) {
		_test_ai_skipparams(ply, instr);
	} else {
		_test_ai_error_illegal(ply);		// illegal
	}
}
static void _test_ai_setupglobals(Player *ply, const u8 **a2, u8 *d2) {			// 89b7c
	switch (g.mode1) {
		case 0:
			*a2 = ply->AIStratAgg0;
			*d2 = ply->AIStratIndexAgg0;
			break;
		case 2:
			*a2 = ply->AIStratAgg1;
			*d2 = ply->AIStratIndexAgg1;
			break;
		case 4:
			*a2 = ply->AIStratDef;
			*d2 = ply->AIStratIndexDef;
			break;
		FATALDEFAULT;
	}
}
static void _test_ai_skiptoken(Player *ply, int d0) {				// 89bd0
	if (g.mode1 == 4 && d0 == AIB_RESTART) {
		_test_ai_error_construct(ply, AIB_RESTART);
	} else {
		ply->AISaveState = 1;
		if (d0 & 0x80) {
			_test_ai_check_highai(ply, d0);
		} else {
			_test_ai_check_strategy(ply, d0);
		}
	}
}
static void _test_ai_searchtoken(Player *ply, int d3) {				// 89bac
	int instr;
	const u8 *a2;
	u8 d2;
	if (ply->AISaveState == 0) {
		_test_ai_setupglobals(ply, &a2, &d2);
		do {
			ply->AISaveState = 1;
			instr = _test_ai_getnextbyte(ply);
			if (instr == d3) {
				break;
			}
			_test_ai_skiptoken(ply, instr);
		} while (TRUE);
		ply->AISaveState = 0;
	}
}
static int _test_ai_check_highai(Player *ply, int instr) {			// 898fc
	instr -= 0x80;
	if (instr > 0x34) {
		_test_ai_error_illegal(ply);	
		return 0;
	} else {
		switch (instr) {
			case 0:
				if (g.mode1 != 4 && ply->AISaveState != 0) {
					_test_ai_error_construct(ply, instr);
					return 0;
				} else {
					return -1;
				}
				break;
			case 2:		// AIB_EXIT3
				if (g.mode1 == 4) {
					_test_ai_error_illegal(ply);	
					return 0;
				} else if (ply->AISaveState) {
					_test_ai_error_construct(ply, instr);
					return 0;
				} else {
					return -1;
				}
				break;
			case 4:		// AIB_EXIT4
			case 6:		// AIB_EXITRAND
				if (g.mode1 == 4) {
					_test_ai_error_illegal(ply);
					return 0;
				} else if (ply->AISaveState) {
					_test_ai_error_construct(ply, instr);
					return 0;
				} else {
					return -1;
				}
				break;
			case 8:		// AIB_COLLDIS
				return 0;
				break;
			case 0xa:		//AIB_EXIT5_1
			case 0xc:		//AIB_EXIT5_0
			case 0xe:		//AIB_EXIT5_2
							// Illegal in Defence 
				if (g.mode1 == 4) {
					_test_ai_error_illegal(ply);
					return 0;
				} else {
					_test_ai_getnextbyte(ply);
					return 0;
				}
				break;
			case 0x10:		// AIB_WALLBOUNCE
			case 0x14:		// AIB_LABEL_94
			case 0x16:		// AIB_SET_0216
			case 0x1a:		// AIB_GO_AGG1
			case 0x1c:		// AIB_MAYBE_RESTART
			case 0x20:		// AIB_LABEL_A0
			case 0x24:		// AIB_LABEL_A4
			case 0x28:		// AIB_LABEL_A8
			case 0x2c:		// AIB_LABEL_AC
			case 0x30:		// AIB_BB2
			case 0x32:		// AIB_LABEL_B2
				
				return 0;
				break;
			case 0x12:		//AIB_B94_NODIZZY
				_test_ai_searchtoken(ply, AIB_LABEL_94);
				return 0;
				break;
			case 0x18:
				if (g.mode1 == 4) {
					_test_ai_error_illegal(ply);
					return 0;
				} else {
					return 0;
				}
				break;
			case 0x1e:
				_test_ai_getnextbyte(ply);
				_test_ai_searchtoken(ply, 0xa0);
				return 0;
				break;
			case 0x22:
				_test_ai_getnextbyte(ply);
				_test_ai_searchtoken(ply, 0xa4);
				return 0;
				break;
			case 0x26:
				_test_ai_getnextbyte(ply);
				_test_ai_searchtoken(ply, 0xa8);
				return 0;
				break;
			case 0x2a:
				_test_ai_getnextbyte(ply);
				_test_ai_searchtoken(ply, 0xac);
				return 0;
				break;
			case 0x2e:		// AIB_BB0_NOTWITHIN
				instr = _test_ai_getnextbyte(ply);
				if (instr >= 5) {
					_test_ai_putmsg("\x02\x0c\x00ILLEGAL MOSI !!\x00");
					_test_ai_dumpstatus(ply);
					return 0;
				} else {
					instr = _test_ai_getnextbyte(ply);
					_test_ai_searchtoken(ply, 0xb0);
					_test_ai_searchtoken(ply, 0xb2);
					return 0;
				}
				break;
			case 0x34:
				_test_ai_getnextbyte(ply);
				return 0;
				break;
			FATALDEFAULT;
		}
	}
	return 0;
}
static int _test_ai_clearsave_check_highai(Player *ply, int instr) {				// 898f8
	ply->AISaveState = 0;
	return _test_ai_check_highai(ply, instr);
}

static void _test_ai_checkall(Player *ply) {					// 89822
	int res;
	int instr = _test_ai_getnextbyte(ply);
	if (instr != 0 && instr != 2 && instr != 4) {
		_test_ai_putmsg("\x02\x0c\x00ERROR! NO INIT DATA\x00");
		_test_ai_dumpstatus(ply);
	}
nextinstr:
	instr = _test_ai_getnextbyte(ply);
	if (instr & 0x80) {
		res = _test_ai_clearsave_check_highai(ply, instr);
		if (res >= 0) {
			goto nextinstr;
		}		
	} else {
		_test_ai_clearsave_check_strategy(ply, instr);
		goto nextinstr;
	}	
}

static void _test_ai_putmsg(const char *message) {			// 89e76
	u16 *scrp;
newline:
	SCR1_CURSOR_SET(scrp, message[0], message[1]);
	int attr = message[2];
	message += 3;
	
	if (message[0] == SF2_TEXTLIB_EOL) {
		++message;
		goto newline;
	} else if (message[0]) {
		SCR1_DRAW_TILE(scrp, GFXROM_SCROLL1 + message[0], attr);
		SCR1_CURSOR_BUMP(scrp, 1, 0);
	} else {
		return;
	}
}
