/* actions.c */

#include <stdio.h>


#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"


#include "gstate.h"
#include "structs.h"
#include "player.h"

#include "particle.h"
#include "lib.h"
#include "gfxlib.h"
#include "sound.h"
#include "actions.h"
#include "gemu.h"
#include "sf2io.h"
#include "text.h"

#include "actiondata.h"


extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;
extern CPSGFXEMU gemu;

/*
static void action_NN(Object *obj) {
	UDXX *ud = (UDXX *)obj->UserData;
	switch (obj->mode0) {
		case 0:
			
			break;
		case 2:
			
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}
*/


static void _draw_portrait_scr2(const SimpleImage *a1, short d1, u16 **gfx_p);
static POINT16 Act23RandomSmallOffset(void);
static Player *sub_1e7ae(Object *obj);
static void sub_1e59a(Object *obj);
static void sub_1e84c(Object *obj);
static void Act23SMBlood(Object *obj);

static void action_0(Object *obj);
static void action_1(Object *obj);	/* cb2a */
extern void action_2(Object *obj);
static void action_3(Object *obj);
static void action_4(Object *obj);
static void action_05(Object *obj);
static void action_06(Object *obj);
extern void action_07(Object *obj);
static void action_08(Object *obj);
static void action_09(Object *obj);
static void action_0a(Object *obj);
static void action_0b(Object *obj);
static void action_0c(Object *obj);
static void action_0d(Object *obj);
static void action_0e(Object *obj);
static void action_0f(Object *obj);
static void action_10(Object *obj);
static void action_11(Object *obj);
static void action_12(Object *obj);
static void action_13(Object *obj);
static void action_14(Object *obj);
static void action_15(Object *obj);
static void action_16(Object *obj);
static void action_17(Object *obj);
static void action_18(Object *obj);
static void action_19(Object *obj);
static void action_1a(Object *obj);
static void action_1b(Object *obj);
static void action_1c(Object *obj);
static void action_1d(Object *obj);
extern void action_1e(Object *obj);
static void action_1f(Object *obj);
static void action_20(Object *obj);
static void action_21(Object *obj);
static void action_22(Object *obj);
void action_1e420(Object *obj);			// action_23
static void action_24(Object *obj);

extern void action_29(Object *obj);

extern void action_2c(Object *obj);

extern void action_2e(Object *obj);

static void action_2f(Object *obj);

static void action_30(Object *obj);

static void action_33(Object *obj);

static void action_35(Object *obj);
static void action_36(Object *obj);	/* screenwobble */
static void action_37(Object *obj);
static void action_38(Object *obj);
static void action_39(Object *obj);
static void action_3a(Object *obj);
static void action_3b(Object *obj);

extern void action_3e(Object *obj);
extern void action_3f(Object *obj);

static void action_44(Object *obj);
static void action_48(Object *obj);
short g_d7;


static void _draw_frame_corners(u16 **gfx_p, u32 cp );	/* 15df6 */


typedef struct UserData_Act09 UD09;
typedef struct UserData_Act0B UD0B;
typedef struct UserData_Act0E UD0E;
typedef struct UserData_Act11 UD11;
typedef struct UserData_Act12 UD12;
typedef struct UserData_Act1d UD1D;
typedef struct UserData_Act2f UD2F;

typedef struct UserData_Act35 UD35;
typedef struct UserData_Act36 UD36;
typedef struct UserData_Act3b UD3B;
typedef struct UserData_Act44 UD44;
typedef struct UserData_Act48 UD48;


/* 
 MAIN ENTRY
 proc_actions, loop over all ations, check if active, and call back */

void proc_actions(void) {			/* c7da */
	short i;
	debughook(2);
	g_d7=0x3b;
	for(i=0; i<=0x3b; i++) {
		if(g.Objects3[i].exists == 0) {continue;}
		Object *obj = &g.Objects3[i];

		// replace the switch with an array LU
		// printf("action %02d sel %02x subsel %02x\n", i, obj->Sel, obj->SubSel);
		switch (g.Objects3[i].Sel) {
				ACT117C(   0, action_0)
				ACT117C(   1, action_1)
				ACT117C(   2, action_2)
				ACT117C(   3, action_3)
				ACT117C(   4, action_4)
				ACT117C(   5, action_05)
				ACT117C(   6, action_06)
				ACT117C(   7, action_07)	
				ACT117C(   8, action_08)
				ACT117C(   9, action_09)
				ACT117C(0x0a, action_0a)
				ACT117C(0x0b, action_0b)
				ACT117C(0x0c, action_0c)
				ACT117C(0x0d, action_0d)
				ACT117C(0x0e, action_0e)
				ACT117C(0x0f, action_0f)
				ACT117C(0x10, action_10)
				ACT117C(0x11, action_11)
				ACT117C(0x12, action_12)
				ACT117C(0x13, action_13)
				ACT117C(0x14, action_14)
				ACT117C(0x15, action_15)
				ACT117C(0x16, action_16)
				//ACT117C(0x17, action_17)
				ACT117C(0x18, action_18)				
				ACT117C(0x19, action_19)
				ACT117C(0x1a, action_1a)
				ACT117C(0x1b, action_1b)
				ACT117C(0x1c, action_1c)
				ACT117C(0x1d, action_1d)
				ACT117C(0x1e, action_1e)
				ACT117C(0x1f, action_1f)
				ACT117C(0x20, action_20)
				ACT117C(0x21, action_21)
				ACT117C(0x22, action_22)
				ACT117C(0x23, action_1e420)	
				//ACT117C(0x24, action_24)
				
				ACT117C(0x29, action_29)
				ACT117C(0x2c, action_2c)
				
				ACT117C(0x2e, action_2e)
				ACT117C(0x2f, action_2f)
				ACT117C(0x30, action_30)
				
				ACT117C(0x33, action_33)
				
				ACT117C(0x35, action_35)
				ACT117C(0x36, action_36)
				ACT117C(0x37, action_37)
				ACT117C(0x38, action_38)
				ACT117C(0x39, action_39)
				ACT117C(0x3a, action_3a)
				ACT117C(0x3b, action_3b)
				
				ACT117C(0x3e, action_3e)
				ACT117C(0x3f, action_3f)
				
				ACT117C(0x44, action_44)
				ACT117C(0x48, action_48)
			default:
				printf("action 258a id %02x not implemented\n", g.Objects3[i].Sel);
				FreeActor(obj);
				break;
		}
		g_d7--;
	}
}





static void action_0(Object *obj) {    /* c934 */
    switch(obj->mode0) {
    case 0:
        NEXT(obj->mode0);
        setactiondraw(obj, data_c95c, obj->SubSel);	/* animgroup 4 */
        break;
    case 2:
        actiontickdraw(obj);
        break;
    case 4:
    case 6:
        FreeActor(obj);
        break;
    }
}
static void action_1(Object *obj) {	/* cb2a */
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool     = data_cb60[obj->SubSel];
			obj->OnGround = data_cb6c[obj->SubSel];
			setaction_direct(obj, actlist_cb78[obj->SubSel]);
			break;
		case 2:
			switch (obj->SubSel) {
				case 0:
					/* cbb6 */
					switch (obj->mode1) {
						case 0:
							if(g.FightOver) {
								NEXT(obj->mode1);
								obj->Timer = 10;
								obj->x002e = 0;
								obj->Draw1 = TRUE;
								obj->Draw2.part.integer = 0x1f;
							}
							break;
						case 2:
							if(obj->Timer-- == 0) {
								obj->Timer = 10;
								obj->Draw2.part.integer = data_cc0e[obj->x002e];	/* u16 x 8 */
								obj->x002e++;
								obj->x002e &= 0xe;
							}		
							break;
							
							FATALDEFAULT;
					}
					break;
				case 1:
					/* cc1e */
					if(obj->mode1==0) {
						NEXT(obj->mode1);
						obj->OnGround = TRUE;	/* u8 */
						obj->VelY.full = obj->YPI;
					}
					obj->YPI = obj->VelY.full;
					obj->YPI += (gstate_Scroll2.YPI >> 1);
					check_rect_queue_draw(obj);
					break;
				case 2 ... 11:
					check_rect_queue_draw(obj);
					break;
					FATALDEFAULT;
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
			FATALDEFAULT;
	}	
}


#pragma mark ACT02 Bycycle riders on ChunLi Stage



#pragma mark ACT03 Das Boat

static void action_3(Object *obj) {    /* d81e Das Boat */
    switch(obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			gstate_Scroll3.YOff = 0;	/* Scroll3 + 0x32 u16 */
			setaction_direct(obj, actlist_d878);
			break;
		case 2:
			if (0x74f6 & ( 1 << RAND16)) {
				gstate_Scroll3.YOff = obj->AnimFlags & 0xf;
				enqueue_and_layer(obj);
			} else {
				actiontick(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
    }
}

#pragma mark Action_04

static void sub_dac2(Player *ply) {
	ply->x01ad = ply->x01ae ? TRUE : FALSE;		/* dade inlined */
	if (ply->x0142) {
		ply->OnPlatform = TRUE;
		ply->x0142      = FALSE;
	} else {
		ply->OnPlatform = FALSE;
	}
}
static void action_4(Object *obj) {     /* da72 */
    switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			if (obj->SubSel) {
				sub_41c2(obj, actlist_dc52);		
			} else {
				sub_41c2(obj, actlist_dc6e);
			}
			break;
		case 2:
			if (obj->SubSel) {
				check_ply_x_bounds((Player *)obj);
				sub_dac2(PLAYER1);
				sub_dac2(PLAYER2);
			}
			if (g.FightOver) {
				actiontick(obj);	/* tick twice */
			}
			actiontick(obj);
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}

#pragma mark Act05

static void action_05(Object *obj) {	//daf6
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			setactiondraw(obj, simpleact_db1a, obj->SubSel);
			break;
		case 2:
			if (obj->mode1 == 0 && g.FightOver != 0) {
				NEXT(obj->mode1);
				setactiondraw(obj, simpleact_db4a, obj->SubSel);
			} else {
				actiontickdraw(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		default:
			break;
	}
}

#pragma mark Act06
static void action_06(Object *obj) {		//db5a
	Object *obj2;
	int i;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			setactiondraw(obj, simpleact_db7e, obj->SubSel);
			break;
		case 2:
			if (obj->mode1 == 0) {
				if (g.FightOver == 0) {
					actiontickdraw(obj);
				} else {
					NEXT(obj->mode1);
					if (obj->SubSel == 1) {
						obj->XPI = 0x1d0;
						obj->YPI = 0x090;
					} else if (obj->SubSel == 5) {
						obj->XPI = 0x310;
					}
					setactiondraw(obj, simpleact_dbdc, obj->SubSel);
				}
			} else {
				if (obj->SubSel == 4) {
					switch (obj->mode2) {
						case 0:
							if (obj->AnimFlags != 0) {
								NEXT(obj->mode2);
								for (i=0; i<=9; i++) {
									if((obj2 = AllocActor())) {
										obj2->exists = TRUE;
										obj2->Sel = SF2ACT_0X10;
										obj2->Scroll = 4;
										obj2->XPI = obj->XPI + 0x10;
										obj2->YPI = obj->YPI + 0x04;
									}
								}
							}
							break;
						case 2:
							if (obj->AnimFlags == 0) {
								obj->mode2 = 0;
							}
							break;
						FATALDEFAULT;
					}
				} else {
					actiontickdraw(obj);
				}
			}
			break;
		default:
			break;
	}
}

#pragma mark Act07 Dhalsim Elephants
#pragma mark Act08
static void action_08(Object *obj) {		//f13a
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			setactiondraw(obj, simpleact_f15e, obj->SubSel);
			break;
		case 2:
			if (obj->mode1 || g.FightOver == FALSE) {
				actiontickdraw(obj);
			} else {
				NEXT(obj->mode1);
				setactiondraw(obj, simpleact_f190, obj->SubSel);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}

#pragma mark Act09	 EHonda Bathwater
static void sub_f664(Object *obj) {
	UD09 *ud = (UD09 *)&obj->UserData;
	int i;
	Object *nobj;
	if (ud->h0083c) {
		if ((obj->AnimFlags & 0xf) != 6) {
			ud->h0083c = 0;
		}
	} else {
		if ((obj->AnimFlags & 0xf) != 6) {
			ud->h0083c = 1;
			for (i=9; i>=0; --i) {
				if (0xaaaa & (1 << RAND16)) {
					if ((nobj = AllocActor()) == NULL) {
						return;
					}
					nobj->exists = TRUE;
					nobj->Sel	= SF2ACT_0X0E;
					nobj->XPI	= obj->XPI;
					nobj->YPI	= obj->YPI;
					nobj->ZDepth = obj->ZDepth;
					nobj->XPI += (short []) {
						0x0020, 0x0043, 0x0052, 0x005d, 0x007b, 0x009f, 0x00d4, 0x00e1, 0x00ee, 0x01ff,
					} [i];
					nobj->YPI += (short []) {
						0xffec, 0xfff0, 0xffed, 0xffec, 0xfff0, 0xffec, 0xffef, 0xffec, 0xffeb, 0xffeb,
					} [i];
				}
			}
			
		}
	}
}	

static void action_09(Object *obj) {	// f568
	UD09 *ud = (UD09 *)&obj->UserData;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			ud->h0082c = 0;
			ud->h0083c = 0;
			ud->h0080s = 0;
			setaction_direct(obj, actlist_f708);
			// Tamper protection removed
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					// f620
					NEXT(obj->mode1);
					obj->LocalTimer = (char []){60,0x78,60,0x78,0x78,60,0xb4,0x78}[RAND8];
					break;
				case 2:
					// f63c
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
						obj->LocalTimer = 0x32;
					}
					break;
				case 4:
					// f64e
					if (--obj->LocalTimer == 0) {
						obj->mode1 = 0;
						setaction_direct(obj, actlist_f708);
					} else {
						actiontick(obj);
					}

					break;
				default:
					break;
			}
			sub_f664(obj);
			if ((obj->AnimFlags & 0xff) == ud->h0080s) {
				if (ud->h0082c) {
					ud->h0082c = 0;
					// XXX drawsimple_scroll2noattr_check(obj, data_f86c[ud->h0080s], 21, 1);
				}
			} else {
				ud->h0080s = obj->AnimFlags & 0xff;
				ud->h0082c = TRUE;
			}
			enqueue_and_layer(obj);

		FATALDEFAULT;
																  
	}
}

#pragma mark Act0A
static void action_0a(Object *obj) {		// f91c
	Object2 *newobj;
	switch (obj->mode0) {
		case 0:
			if (g.FightOver) {
				NEXT(obj->mode0);
				if ((newobj = pop_5d0c())) {
					newobj->exists = TRUE;
					newobj->Sel = 0;
					newobj->SubSel = 14;
				}
				if ((newobj = pop_5d0c())) {
					newobj->exists = TRUE;
					newobj->Sel = 0;
					newobj->SubSel = 15;
				}
				// XXX setactiondraw(obj, data_f976, 0);	// XXX step is undef!
			}
			break;
		case 2:
			actiontickdraw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}

#pragma mark Act0B
static void action_0b(Object *obj) {	// fa12 Ryu Stage Bird
	UD0B *ud = (UD0B *)&obj->UserData;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			ud->h0080s = obj->YPI - obj->Step;
			if (obj->YPI - obj->Step >= g.GroundPlaneY) {
				obj->Pool = 6;
				ud->h0082s = obj->YPI;
			} else {
				obj->Pool = 0;
			}
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->YPI = ud->h0082s;
					obj->ZDepth = obj->YPI;
					setaction_list(obj, actlist_fb2e , obj->SubSel);
					break;
				case 2:
					if ((obj->AnimFlags & 0xff) == 1) {
						NEXT(obj->mode1);
						obj->LocalTimer = (short []){60, 0x78, 0xb4, 0xf0, 0xf0, 0xb4, 0x78, 60}[RAND8];
					} else {
						actiontick(obj);
					}
					break;
				case 4:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
						obj->VelY.full = 0;
					}
					break;
				case 6:
					if ((obj->AnimFlags & 0xff) == 2) {
						NEXT(obj->mode1);
					} else {
						actiontick(obj);
					}
					break;
				case 8:
					obj->VelY.full -= 24;
					if (obj->VelY.full < -0x400 ) {
						obj->VelY.full = -0x400;
					}
					obj->Y.full += (obj->VelY.full * 256);
					if (obj->YPI <= ud->h0080s) {
						setaction_list(obj, actlist_fb32, obj->SubSel);
					}
					break;
				case 10:
					if (obj->AnimFlags & 0xff) {
						obj->mode1 = 0;
					}
					actiontick(obj);
					break;
				FATALDEFAULT
			}
			check_rect_queue_draw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}

#pragma mark Act0C
static void action_0c(Object *obj) {		// fd1a
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			// XXX setactiondraw(obj, data_fd64, obj->SubSel);
			break;
		case 2:
			if (obj->mode1 == 0 && g.FightOver) {
				NEXT(obj->mode1);
				if (obj->SubSel < 8) {
					// XXX setactiondraw(obj, data_fd86, obj->SubSel);
				} else {
					actiontickdraw(obj);
				}
			} else {
				actiontickdraw(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
		FATALDEFAULT
	}
}

#pragma mark Act0D
static void action_0d(Object *obj) {			//1045e
	switch(obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			if (obj->SubSel == 3 && obj->Step) {
				g.x8a66[0] = 0;
				g.x8a66[1] = 0;
			}
			setactiondraw(obj, actlist_10492, obj->SubSel);
			break;
		case 2:
			if (obj->SubSel == 3) {
				// 104d0
				if (obj->mode1) {
					actiontickdraw(obj);
				} else {
					if (g.x8a66[obj->Step]) {
						NEXT(obj->mode1);
						setactiondraw(obj, actlist_10628, obj->Step);
						// ?? any Step other than 0 is invalid access
					}
				}
			} else {
				if (obj->mode1 == 0 && g.FightOver) {
					NEXT(obj->mode1);
					setactiondraw(obj, actlist_104c6, obj->Step);
				} else {
					actiontickdraw(obj);
				} 
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
		FATALDEFAULT
	}
}

#pragma mark Act0E
static void action_0e(Object *obj) {		// 10756
	UD0E *ud = (UD0E *)&obj->UserData;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			setaction_direct(obj, actlist_10822);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					actiontick(obj);
					if (obj->AnimFlags) {
						NEXT(obj->mode1);
						obj->VelY.full = -0x80;
						obj->LocalTimer = RAND8 + 1;
					}
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
						ud->h0080s = 63 - (sf2rand() & 3);
					}
					break;
				case 4:
					obj->VelY.full -= 0x20;
					obj->YPI += (obj->VelY.full * 256);
					if (obj->YPI <= ud->h0080s) {
						NEXT(obj->mode1);
						obj->LocalTimer = 15;
						obj->YPI = ud->h0080s;
						setaction_direct(obj, actlist_10852);
					}
					break;
				case 6:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
						obj->mode1 = 0; 
					}
					actiontick(obj);
				FATALDEFAULT
			}
			check_rect_queue_draw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}

#pragma mark Act0F
static void action_0f(Object *obj) {		// 10b44 Another Bird
	switch (obj->mode0) {
		case 0:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->Step = (char []){1,0,1,2,1,1,0,1,2,1,0,2,0,2,1,0}[RAND16];
					obj->LocalTimer = (char[]){
						5, 30, 25, 10, 15, 20, 20, 20, 10, 15, 20, 10, 5, 30, 15, 35
					}[RAND16];
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
						obj->mode1 = 0;
						setaction_list(obj, actlist_10bc2, obj->Step);
					}
				default:
					break;
			}
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->VelY.full = (short []){
						0x100, 0x110, 0xa0, 0xc0, 0x130, 0x100, 0x200, 0x100
					}[RAND8WD];
					obj->VelX.full = (short[]){0xffd0, 0x0,   0x40}[obj->Step];
					obj->AclX.full = (short[]){   0x8, 0x0, 0xfff8}[obj->Step];
					break;
				case 2:
					obj->VelY.full += 6;
					if (obj->VelY.full >= 0x300) {
						obj->VelY.full = 0x400;
					}
					obj->Y.full += (obj->VelY.full * 256);
					obj->VelX.full -= obj->AclX.full;
					obj->X.full += (obj->VelX.full * 256);
					break;
				FATALDEFAULT;
			}
			if (((g.libsplatter + g_d7) & 0xf) == 0 ) {
				die_if_offscreen(obj);
			}
			actiontick(obj);
			check_rect_queue_draw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		FATALDEFAULT;
	}
}

#pragma mark Act10 10c66

static void action_10(Object *obj) {
	char data_10c9e[]={
		10, 2, 8, 5, 1, 1, 3, 4, 4, 3, 1, 4, 5, 2, 1, 3
	};

	switch (obj->mode0) {
		case 0:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->Timer = data_10c9e[RAND16];
					break;
				case 2:
					NEXT(obj->mode0);
					obj->mode1=0;
					if (0x5555 & (1 << RAND32)) {
						setaction_direct(obj, act_10f90);
					} else {
						setaction_direct(obj, act_10f5c);
					}
				FATALDEFAULT
			}
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->LocalTimer = (char []) {0x3c, 0x28, 0x5a, 0x28, 0x28, 0x3c, 0x14, 0x46,} [RAND8];
					obj->VelX.full = (short []) {0xff00, 0xfee0, 0xffb0, 0xfe80, 0xff60, 0xff40, 0xfe00, 0xfde0,} [RAND8WD];
					obj->VelY.full = (short []) {
						0x0040, 0x0063, 0x0028, 0x0080, 0x0060, 0x0023, 0x0088, 0x0058,
					} [RAND8WD];
					obj->AclX.full = (short []){
						0xfffe, 0xfffc, 0x0008, 0x0004, 0xfffc, 0x0002, 0xfffc, 0xfff8,
					}[RAND8WD];
					obj->AclY.full = (short[]) {
						0x0005, 0x0006, 0x0007, 0x0008, 0x000a, 0x0010, 0x0011, 0x0012,
					}[RAND8WD];
					break;
				case 2:
					CATrajectory(obj);
					break;
				FATALDEFAULT;
			}
			if (--obj->LocalTimer == 0) {
				NEXT(obj->mode0);
				obj->mode1 = 0;
			}
			actiontick(obj);
			check_rect_queue_draw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}

#pragma mark ACT11  10ff4 Barely noticable bat 

static void action_11(Object *obj) {
	UD11 *ud = (UD11 *)&obj->UserData;
	int d0;
	Object *nobj;
	
	const static u16 data_1104e[] = {0x02a0, 0x02a0, 0x02b0, 0x02d0, 0x02f0, 0x0310, 0x0320, 0x0340};
	const static u16 data_1105e[] = {0xa8, 0xd0, 0xc0, 0xb8, 0xd0, 0xa0, 0xd8, 0xb0};
	
	switch (obj->mode0) {
		case 0:
			if (obj->SubSel) {
				NEXT(obj->mode0);
				obj->LocalTimer = 0x20;
				ud->h0080l = 0x0000;
				ud->h0084l = 0x4000;
				obj->OnGround = TRUE;
				d0 = (sf2rand() & 0xe);		// SF2UA bug does & 0x1e
				obj->XPI = data_1104e[d0/2];
				obj->YPI = data_1105e[d0/2];
				setaction_list(obj, actlist_111e6, 0);
			} else {
				NEXT(obj->mode0);
				obj->LocalTimer = 0x1e;
				g.x8a5c = 0;
			}
			break;
		case 2:
			if (--obj->LocalTimer == 0) {
				obj->LocalTimer = 0x1e;
				if (g.x8a5c < 3) {
					if ((sf2rand() & 3) == 0) {
						if ((nobj = AllocActor())) {
							nobj->exists = TRUE;
							nobj->Sel = 0x11;
							nobj->SubSel = 0x1;
							nobj->ZDepth = obj->ZDepth;
							nobj->Scroll = SCROLL_2;
							nobj->Pool = 6;
							++g.x8a5c;
						}
					}
				}
			}
		FATALDEFAULT;
	}
}

#pragma mark Act12

static void sub_11516(Object *obj) {
	UD12 *ud = (UD12 *)&obj->UserData;
	
	if (ud->h0080c) {
		if ((obj->YPI & 0xff) != 0x98) {
			--obj->YPI;
		}
	}
}

static void action_12(Object *obj) {		// "Street Fighter" logo
	UD12 *ud = (UD12 *)&obj->UserData;
	Object *nobj;
	
	switch (obj->SubSel) {
		case 0:				// static image
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->Scroll = SCROLL_NONE;
					obj->Pool	= 6;
					obj->XPI	= 192;
					obj->YPI	= 152;
					setaction_list(obj, actlist_11602, 1);
					/* FALLTHRU */
				case 2:
					enqueue_and_layer(obj);
					break;
				case 4:
				case 6:
					FreeActor(obj);
					break;
				FATALDEFAULT;
			}
			break;
		case 1:
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->Pool	= 6;
					ud->h0080c  = 0;
					setaction_list(obj, actlist_11602, 0);
					break;
				case 2:
					switch (obj->mode1) {
						case 0:
							if (g.Pause_9e1 < 0) {
								NEXT(obj->mode1);
								ud->h0080c      = 1;
								obj->Step       = 0;
								obj->LocalTimer = 16;
								obj->Draw2.full = 0;
								obj->Draw1      = -1;		// tilespin effect
							}
							break;
						case 2:
							obj->Step = (obj->Step + 2) & 0x1f;
							if (obj->Step == 0) {
								NEXT(obj->mode1);
								if ((nobj=AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel    = 0x29;
								}
								setaction_list(obj, actlist_11602, 1);
							}
							break;
						case 4:
							obj->Step = (obj->Step + 2) & 0x1f;
							if (obj->Step == 0) {
								NEXT(obj->mode1);
							}
							break;
						case 6:
							/* does nothing */
							break;
						FATALDEFAULT;
					}
					sub_11516(obj);
					check_rect_queue_draw(obj);
					break;
				case 4:
				case 6:
					FreeActor(obj);
					break;
				FATALDEFAULT;
			}
			break;
		case 2:
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->Pool = 6;
					ud->h0080c = 0;
					setaction_list(obj, actlist_11602, 0);
					break;
				case 2:
					switch (obj->mode1) {
						case 0:
							if (g.Pause_9e1 < 0) {
								NEXT(obj->mode1);
								ud->h0080c = 1;
								obj->Step = 0;
								obj->LocalTimer = 0x10;
								obj->Draw1 = -1;
								obj->Draw2.full = 0;
							}
							break;
						case 2:
							--obj->Draw2.full;
							if (--obj->LocalTimer == 0) {
								NEXT(obj->mode1);
								if ((nobj=AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x29;
								}
								setaction_list(obj, actlist_11602, 1);
							}
							break;
						case 4:
							++obj->Draw2.full;
							if (obj->Draw2.full >= 0) {
								NEXT(obj->mode1);
							}
							break;
						case 6:
							/* does nothing */
							break;
							FATALDEFAULT;
					}
					sub_11516(obj);
					check_rect_queue_draw(obj);
					break;
				case 4:
				case 6:
					FreeActor(obj);
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}

#pragma mark mark

static void sub_15dae(u16 **gfx_p, u32 cp){
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, TILE_FRAMECORNER2, 0x1f | ATTR_X_FLIP);
	INC_GFX_CURSOR(&cp, 0x50, 0);
	OBJ_CURSOR_BUMP(*gfx_p);
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, TILE_FRAMECORNER2, 0x1f | ATTR_NO_FLIP);
	INC_GFX_CURSOR(&cp, 0x50, 0);
}
static void PlayerNameFirst8(u16 **gfx_p, short fighterid, int x, int y) {		// 15e3e
	static const u16 data_15e50[8][8] = {
		{ PALETTE_0D, 0x7e28, 0x7e29, 0x7e2a, 0x8020, 0x8020, 0x8020, 0x0000,  },	// RYU
		{ PALETTE_0D, 0x7e38, 0x7e39, 0x7e3a, 0x7e3b, 0x7e3c, 0x7e3d, 0x0000,  },
		{ PALETTE_0D, 0x7e48, 0x7e49, 0x7e4a, 0x7e4b, 0x7e4c, 0x7e4d, 0x0000,  },
		{ PALETTE_0D, 0x7e2b, 0x7e2c, 0x7e2d, 0x7e2e, 0x7e2f, 0x8020, 0x0000,  },
		{ PALETTE_0D, 0x7e3e, 0x7e3f, 0x7e4e, 0x7e4f, 0x8020, 0x8020, 0x0000,  },
		{ PALETTE_0D, 0x7e5a, 0x7e5b, 0x7e5c, 0x7e5d, 0x7e5e, 0x7e5f, 0x0000,  },
		{ PALETTE_0D, 0x7e6a, 0x7e6b, 0x7e6c, 0x7e6d, 0x7e6e, 0x7e6f, 0x0000,  },
		{ PALETTE_0D, 0x7e7a, 0x7e7b, 0x7e7c, 0x7e7d, 0x7e7e, 0x7e7f, 0x0000,  },
	};
	DrawTileLine(*gfx_p, data_15e50[fighterid], x, y);
}
static void DrawFighterPortrait(u16 **gfx_p, short fighterid, short flip) {	//15f7a
	
	static const SimpleImage image_1640a = {
		7, 7, 1,
		{	0x0000, 0x0000, 0x1148, 0x0000, 0x1150, 0x0000, 0x1158, 0x0000, 0x1160, 0x0000, 0x1168, 0x0000, 0x0000, 0x0000,
			0x1141, 0x0000, 0x1149, 0x0000, 0x1151, 0x0000, 0x1159, 0x0000, 0x1161, 0x0000, 0x1169, 0x0000, 0x0000, 0x0000, 
			0x1142, 0x0000, 0x114a, 0x0000, 0x1152, 0x0000, 0x115a, 0x0000, 0x1162, 0x0000, 0x116a, 0x0000, 0x0000, 0x0000, 
			0x1143, 0x0000, 0x114b, 0x0000, 0x1153, 0x0000, 0x115b, 0x0000, 0x1163, 0x0000, 0x116b, 0x0000, 0x0000, 0x0000, 
			0x1144, 0x0000, 0x114c, 0x0000, 0x1154, 0x0000, 0x115c, 0x0000, 0x1164, 0x0000, 0x116c, 0x0000, 0x0000, 0x0000, 
			0x1145, 0x0000, 0x114d, 0x0000, 0x1155, 0x0000, 0x115d, 0x0000, 0x1165, 0x0000, 0x116d, 0x0000, 0x0000, 0x0000, 
			0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000,  }
	};
	static const SimpleImage image_164d4 = {
		7, 7, 1,
		{ 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x119c, 0x0001, 0x11ac, 0x0001, 0x11b4, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x118d, 0x0001, 0x1195, 0x0001, 0x119d, 0x0001, 0x11ad, 0x0001, 0x11b5, 0x0001, 0x0000, 0x0001, 0x1186, 0x0001, 0x118e, 0x0001, 0x1196, 0x0001, 0x119e, 0x0001, 0x11ae, 0x0001, 0x11b6, 0x0001, 0x0000, 0x0001, 0x1187, 0x0001, 0x118f, 0x0001, 0x1197, 0x0001, 0x119f, 0x0001, 0x11af, 0x0001, 0x11b7, 0x0001, 0x0000, 0x0001, 0x11a0, 0x0001, 0x11a8, 0x0001, 0x11b0, 0x0001, 0x11b8, 0x0001, 0x11c8, 0x0001, 0x11d0, 0x0001, 0x0000, 0x0001, 0x11a1, 0x0001, 0x11a9, 0x0001, 0x11b1, 0x0001, 0x11b9, 0x0001, 0x11c9, 0x0001, 0x11d1, 0x0001, 0x0000, 0x0001, 0xffff, 0x0001, 0xffff, 0x0001, 0xffff, 0x0001, 0xffff, 0x0001, 0xffff, 0x0001, 0xffff, 0x0001, 0xffff, 0x0001,  }
	};
	static const SimpleImage image_1659e = {
		7, 7, 1,
		{ 0x0000, 0x0002, 0x1248, 0x0002, 0x1222, 0x0002, 0x122a, 0x0002, 0x1232, 0x0002, 0x123a, 0x0002, 0x0000, 0x0002, 0x1241, 0x0002, 0x1249, 0x0002, 0x1223, 0x0002, 0x122b, 0x0002, 0x1233, 0x0002, 0x123b, 0x0002, 0x0000, 0x0002, 0x1242, 0x0002, 0x124a, 0x0002, 0x1224, 0x0002, 0x122c, 0x0002, 0x1234, 0x0002, 0x123c, 0x0002, 0x0000, 0x0002, 0x1243, 0x0002, 0x124b, 0x0002, 0x1225, 0x0002, 0x122d, 0x0002, 0x1235, 0x0002, 0x123d, 0x0002, 0x0000, 0x0002, 0x1244, 0x0002, 0x124c, 0x0002, 0x1226, 0x0002, 0x122e, 0x0002, 0x1236, 0x0002, 0x123e, 0x0002, 0x0000, 0x0002, 0x0000, 0x0002, 0x1240, 0x0002, 0x1227, 0x0002, 0x122f, 0x0002, 0x1237, 0x0002, 0x123f, 0x0002, 0x0000, 0x0002, 0xffff, 0x0002, 0xffff, 0x0002, 0xffff, 0x0002, 0xffff, 0x0002, 0xffff, 0x0002, 0xffff, 0x0002, 0xffff, 0x0002,  }
	};
	static const SimpleImage image_16668 = {
		7, 7, 1,
		{ 0x1265, 0x0003, 0x0000, 0x0003, 0x0000, 0x0003, 0x127d, 0x0003, 0x1285, 0x0003, 0x128d, 0x0003, 0x0000, 0x0003, 0x1266, 0x0003, 0x126e, 0x0003, 0x1276, 0x0003, 0x127e, 0x0003, 0x1286, 0x0003, 0x128e, 0x0003, 0x0000, 0x0003, 0x1267, 0x0003, 0x126f, 0x0003, 0x1277, 0x0003, 0x127f, 0x0003, 0x1287, 0x0003, 0x128f, 0x0003, 0x0000, 0x0003, 0x1280, 0x0003, 0x1288, 0x0003, 0x1290, 0x0003, 0x1298, 0x0003, 0x12a0, 0x0003, 0x12a8, 0x0003, 0x0000, 0x0003, 0x1281, 0x0003, 0x1289, 0x0003, 0x1291, 0x0003, 0x1299, 0x0003, 0x12a1, 0x0003, 0x12a9, 0x0003, 0x0000, 0x0003, 0x1282, 0x0003, 0x128a, 0x0003, 0x1292, 0x0003, 0x129a, 0x0003, 0x12a2, 0x0003, 0x12aa, 0x0003, 0x0000, 0x0003, 0xffff, 0x0003, 0xffff, 0x0003, 0xffff, 0x0003, 0xffff, 0x0003, 0xffff, 0x0003, 0xffff, 0x0003, 0xffff, 0x0003,  }
	};
	static const SimpleImage image_16732 = {
		7, 7, 1,
		{ 0x0000, 0x0004, 0x12c8, 0x0004, 0x12d0, 0x0004, 0x1158, 0x0004, 0x1160, 0x0004, 0x1168, 0x0004, 0x0000, 0x0004, 0x12c1, 0x0004, 0x12c9, 0x0004, 0x12d1, 0x0004, 0x12d9, 0x0004, 0x12c6, 0x0004, 0x1169, 0x0004, 0x0000, 0x0004, 0x12c2, 0x0004, 0x12ca, 0x0004, 0x12d2, 0x0004, 0x12da, 0x0004, 0x12c7, 0x0004, 0x116a, 0x0004, 0x0000, 0x0004, 0x12c3, 0x0004, 0x12cb, 0x0004, 0x12d3, 0x0004, 0x12db, 0x0004, 0x12e0, 0x0004, 0x116b, 0x0004, 0x0000, 0x0004, 0x12c4, 0x0004, 0x12cc, 0x0004, 0x12d4, 0x0004, 0x12dc, 0x0004, 0x12e1, 0x0004, 0x116c, 0x0004, 0x0000, 0x0004, 0x12c5, 0x0004, 0x12cd, 0x0004, 0x12d5, 0x0004, 0x12dd, 0x0004, 0x12e2, 0x0004, 0x116d, 0x0004, 0x0000, 0x0004, 0xffff, 0x0004, 0xffff, 0x0004, 0xffff, 0x0004, 0xffff, 0x0004, 0xffff, 0x0004, 0xffff, 0x0004, 0xffff, 0x0004,  }
	};
	static const SimpleImage image_167fc = {
		7, 7, 1,
		{ 0x0000, 0x0005, 0x1334, 0x0005, 0x133c, 0x0005, 0x1304, 0x0005, 0x130c, 0x0005, 0x1314, 0x0005, 0x0000, 0x0005, 0x0000, 0x0005, 0x1335, 0x0005, 0x133d, 0x0005, 0x1305, 0x0005, 0x130d, 0x0005, 0x1315, 0x0005, 0x0000, 0x0005, 0x132e, 0x0005, 0x1336, 0x0005, 0x133e, 0x0005, 0x1306, 0x0005, 0x130e, 0x0005, 0x1316, 0x0005, 0x0000, 0x0005, 0x132f, 0x0005, 0x1337, 0x0005, 0x133f, 0x0005, 0x1307, 0x0005, 0x130f, 0x0005, 0x1317, 0x0005, 0x0000, 0x0005, 0x1348, 0x0005, 0x1350, 0x0005, 0x1358, 0x0005, 0x1320, 0x0005, 0x1328, 0x0005, 0x1330, 0x0005, 0x0000, 0x0005, 0x1349, 0x0005, 0x1351, 0x0005, 0x1359, 0x0005, 0x1321, 0x0005, 0x1329, 0x0005, 0x1331, 0x0005, 0x0000, 0x0005, 0xffff, 0x0005, 0xffff, 0x0005, 0xffff, 0x0005, 0xffff, 0x0005, 0xffff, 0x0005, 0xffff, 0x0005, 0xffff, 0x0005,  }
	};
	static const SimpleImage image_168c6 = {
		7, 7, 1,
		{ 0x0000, 0x0006, 0x0000, 0x0006, 0x0000, 0x0006, 0x1368, 0x0006, 0x1370, 0x0006, 0x1378, 0x0006, 0x0000, 0x0006, 0x1390, 0x0006, 0x1398, 0x0006, 0x1361, 0x0006, 0x1369, 0x0006, 0x1371, 0x0006, 0x1379, 0x0006, 0x0000, 0x0006, 0x1391, 0x0006, 0x1399, 0x0006, 0x1362, 0x0006, 0x136a, 0x0006, 0x1372, 0x0006, 0x137a, 0x0006, 0x0000, 0x0006, 0x1392, 0x0006, 0x139a, 0x0006, 0x1363, 0x0006, 0x136b, 0x0006, 0x1373, 0x0006, 0x137b, 0x0006, 0x0000, 0x0006, 0x1393, 0x0006, 0x139b, 0x0006, 0x1364, 0x0006, 0x136c, 0x0006, 0x1374, 0x0006, 0x137c, 0x0006, 0x0000, 0x0006, 0x1394, 0x0006, 0x139c, 0x0006, 0x1365, 0x0006, 0x136d, 0x0006, 0x1375, 0x0006, 0x137d, 0x0006, 0x0000, 0x0006, 0xffff, 0x0006, 0xffff, 0x0006, 0xffff, 0x0006, 0xffff, 0x0006, 0xffff, 0x0006, 0xffff, 0x0006, 0xffff, 0x0006,  }
	};
	static const SimpleImage image_16990 = {
		7, 7, 1,
		{ 0x0000, 0x000c, 0x13ac, 0x000c, 0x13b4, 0x000c, 0x13bc, 0x000c, 0x13c3, 0x000c, 0x13cb, 0x000c, 0x0000, 0x000c, 0x0000, 0x000c, 0x13ad, 0x000c, 0x13b5, 0x000c, 0x13bd, 0x000c, 0x13c4, 0x000c, 0x13cc, 0x000c, 0x0000, 0x000c, 0x0000, 0x000c, 0x13ae, 0x000c, 0x13b6, 0x000c, 0x13be, 0x000c, 0x13c5, 0x000c, 0x13cd, 0x000c, 0x0000, 0x000c, 0x0000, 0x000c, 0x13af, 0x000c, 0x13b7, 0x000c, 0x13bf, 0x000c, 0x13c6, 0x000c, 0x13ce, 0x000c, 0x0000, 0x000c, 0x0000, 0x0007, 0x13c8, 0x0007, 0x13d0, 0x0007, 0x13d8, 0x0007, 0x13c7, 0x0007, 0x13cf, 0x0007, 0x0000, 0x0007, 0x13c1, 0x0007, 0x13c9, 0x0007, 0x13d1, 0x0007, 0x13d9, 0x0007, 0x13e0, 0x0007, 0x13e8, 0x0007, 0x0000, 0x0007, 0xffff, 0x0007, 0xffff, 0x0007, 0xffff, 0x0007, 0xffff, 0x0007, 0xffff, 0x0007, 0xffff, 0x0007, 0xffff, 0x0007,  }
	};
	
	
	static const SimpleImage *data_15f8e[8]={
		&image_1640a, &image_164d4, &image_1659e, &image_16668, 
		&image_16732, &image_167fc, &image_168c6, &image_16990, 
	};
	
	_draw_portrait_scr2(data_15f8e[fighterid], flip, gfx_p);
}

void PrintPlayerPic(Player *ply, short side, short fighterid) {	
	// fighterid %d3
	const static u16 data_15f14[8]={
		310, 288, 288, 295, 308, 285, 288, 288, 
	};
	const static u16 data_15f6a[8]={
		23, 0, 0, 9, 20, 0, 0, 0, 
	};

	u16 *gfx_p;
	
	if (side) {
		gfx_p = CPS_OBJ(0x910080);
		sub_15dae(&gfx_p, MakePointObj(288, 32));
		gfx_p = CPS_OBJ(0x910240);
		PlayerNameFirst8(&gfx_p, fighterid, data_15f14[fighterid] , 128);
		gfx_p = coords_scroll2(296, 104);
		DrawFighterPortrait(&gfx_p, fighterid, FALSE);
	} else {
		gfx_p = CPS_OBJ(0x910070);
		sub_15dae(&gfx_p, MakePointObj(0, 32));
		gfx_p = CPS_OBJ(0x9101c0);
		PlayerNameFirst8(&gfx_p, fighterid, data_15f6a[fighterid] ,128);
		gfx_p = coords_scroll2(0, 0x68);
		gfx_p -= 0x20;			// XXX non-CPS!
		DrawFighterPortrait(&gfx_p, fighterid, TRUE);
	}
}
	
void action_start_35(Player *ply) {			// 1f9c6
	Object *obj;
	if ((obj=AllocActor())) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_GROUNDDUST;
		obj->SubSel = 1;
		obj->Owner = ply;
		obj->Flip = ply->Flip;
		obj->LocalTimer = (char []){16, 21, 24, 0}[(unsigned char)ply->OnPlatform];
	}
}

void action_print_chant() {			/* 15a2a */
	
	static const short data_15a74[12][8] = {
		{ 0x0004, 0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x0048, 0x0004,  },
		{ 0x0005, 0x004f, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055,  },
		{ 0x0006, 0x0056, 0x0057, 0x004d, 0x0006, 0x0056, 0x0057, 0x004d,  },
		{ 0x0007, 0x0058, 0x0059, 0x005a, 0x0007, 0x0058, 0x0059, 0x005a,  },
		{ 0x0008, 0x005b, 0x005c, 0x0072, 0x0008, 0x005b, 0x005c, 0x0072,  },
		{ 0x0009, 0x005d, 0x005e, 0x005f, 0x0009, 0x005d, 0x005e, 0x005f,  },
		{ 0x000a, 0x0060, 0x0061, 0x0070, 0x000a, 0x0060, 0x0061, 0x0070,  },
		{ 0x000b, 0x0062, 0x0063, 0x0064, 0x000b, 0x0062, 0x0063, 0x0064,  },
		{ 0x000c, 0x006e, 0x006f, 0x004e, 0x0071, 0x000c, 0x006e, 0x004e,  },
		{ 0x000d, 0x006a, 0x006b, 0x006c, 0x006d, 0x006d, 0x006c, 0x006a,  },
		{ 0x000e, 0x0065, 0x0066, 0x0073, 0x0073, 0x000e, 0x0065, 0x0066,  },
		{ 0x000f, 0x0067, 0x0068, 0x0069, 0x000f, 0x0067, 0x0068, 0x0069,  },
	};
	
	static const short data_15b34[12][8] = {
		{ 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,  },
		{ 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011,  },
		{ 0x0012, 0x0012, 0x0012, 0x0012, 0x0012, 0x0012, 0x0012, 0x0012,  },
		{ 0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0013,  },
		{ 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014,  },
		{ 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015,  },
		{ 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016,  },
		{ 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017,  },
		{ 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,  },
		{ 0x0019, 0x0019, 0x0019, 0x0019, 0x0019, 0x0019, 0x0019, 0x0019,  },
		{ 0x001a, 0x001a, 0x001a, 0x001a, 0x001a, 0x001a, 0x001a, 0x001a,  },
		{ 0x001b, 0x001b, 0x001b, 0x001b, 0x001b, 0x001b, 0x001b, 0x001b,  },
	};
	
	static const short *data_15a24[] = {		//15a24
		data_15a74[0],		/* JAPAN */
		data_15b34[0],
		data_15b34[0],
	};
	
	g.TextEffectBusy = TRUE;
	const short *data = data_15a24[g.Version];
	
	if (g.Version == VERSION_JAP) {
		start_effect(0x2400 + *(data + 8*g.BattleWinner + (RAND8WD)), 0x0102);
	} else {	
		start_effect(0x1800 + *(data + 8*g.BattleWinner + (RAND8WD)), 0x0102);
	}
}

#pragma mark Act13 Ryu Sign fragments

static void action_13(Object *obj) {  // 119ee
	const static short data_11a80[4][4]={
		{-0xc0, -0x80, 0x100, 0xa0},
		{0x500, 0x400, 0x500, 0x400},
		{ -0x8,     0,   0x8,   0x8},
		{ 0x46,  0x36,  0x36,  0x26}
	};
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->UserData[0] = 0;
			obj->ZDepth = 0;
			obj->Scroll = SCROLL_2;
			obj->Pool = 0;
			setaction_list(obj, actlist_11aca, obj->SubSel);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->VelX.full = data_11a80[0][obj->SubSel];
					obj->VelY.full = data_11a80[1][obj->SubSel];
					obj->XPI += data_11a80[2][obj->SubSel];
					obj->YPI += data_11a80[3][obj->SubSel];
					obj->AclY.full = 0x40;
					obj->AclX.full = 0;
					obj->Pool = 6;
					obj->LocalTimer = 0x19;
					break;
				case 2:
					if(--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
						obj->UserData[0] = 0x1e;
					}
					CATrajectory(obj);
					break;
				case 4:
					if (--obj->UserData[0] == 0) {
						NEXT(obj->mode0);
						obj->mode1 = 0;
					}
					CATrajectory(obj);
					break;
				FATALDEFAULT;
			}
			if ((obj->UserData[0] & 1) == 0) {
				check_rect_queue_draw(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}
#pragma mark Act14
static void action_14(Object *obj) {
	// doesn't call FreeActor(), needs to be wiped out.
	
	if (obj->SubSel) {
		if (obj->mode0) {
			actiontick(obj);
			check_rect_queue_draw(obj);
		} else {
			setaction_direct(obj, action_1237c);
		}
	} else {
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				sub_41c2(obj, simpleaction_126e8);
				break;
			case 2:
				actiontickdraw(obj);
				break;
			default:
				break;
		}
	}

}

#pragma mark Act15
static void action_15(Object *obj) {
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = (u8 []){0,0,0,0,0,0,0,2}[obj->SubSel];
			setactiondraw(obj, actlist_11bc0, obj->SubSel);
			break;
		case 2:
			if (obj->SubSel >= 4 || obj->mode1 || g.FightOver == FALSE) {
				actiontickdraw(obj);
			} else {
				NEXT(obj->mode1);
				setactiondraw(obj, actlist_11c00, obj->SubSel);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}
#pragma mark Act16
#pragma mark Act17
#pragma mark Act18
static int sub_12fe6(Object *obj) {
	if (obj->SubSel) {
		if (gstate_Scroll1.XPI < 448) {
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		if (gstate_Scroll1.XPI < 416) {
			return TRUE;
		} else {
			return FALSE;
		}		
	}
}

static void action_18(Object *obj) {
	Object *nobj;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Scroll = SCROLL_1;
			obj->XPI = (short []){0x144, 0x344}[obj->SubSel];
			obj->YPI = 0xe4;
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					for (int i=14; i>=0; --i) {
						// not quite same, sf2ua bails on
						// first fail of AllocActor
						if ((nobj = AllocActor())) {
							nobj->exists = TRUE;
							nobj->Sel = 0x17;
							nobj->Scroll = SCROLL_1;
							nobj->XPI = obj->XPI + 10;
							nobj->YPI = obj->YPI - 64;
							nobj->SubSel =
								(char []){0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0}[RAND16];
						}
					
					}
					g.x8a66[nobj->SubSel] = 1;
					break;
				case 2:
					if (sub_12fe6(obj) == FALSE) {
						NEXT(obj->mode1);
						drawsimple_scroll1attr(obj, tiles_1300c, 0x11, 8);
					}
					break;
				case 4:
					if (sub_12fe6(obj)) {
						obj->mode1 = 2;
					}
					break;
				FATALDEFAULT;
			}
		case 4:
		case 6:
			FreeActor(obj);
		FATALDEFAULT;
	}
}


#pragma mark Act19 Guile Plane Tail
static void action_19(Object *obj) {     // 1322c
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			setaction_list(obj, actlist_1325e, obj->SubSel);
			break;
		case 2:
			check_rect_queue_draw(obj);
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}

#pragma mark Act1A
static int action_13bf8(void) {
	Object *nobj;
	if ((nobj = AllocActor())) {
		nobj->exists = TRUE;
		nobj->Sel = 0x1a;
		nobj->XPI = g.GPCollX;
		nobj->YPI = g.GPCollY;
		return TRUE;
	}
	return FALSE;
}

static void sub_13c1a(Object *obj) {
	if (obj->UserData[0] & 0x80) {
		if (0xaaaa & (1 << (RAND16))) {
			setaction_list(obj, actlist_13c5a, obj->UserData[1]);
		} else {
			setaction_list(obj, actlist_13c54, obj->UserData[1]);
		}
	} else {
		if (obj->UserData[0]) {
			setaction_list(obj, actlist_13c5a,obj->UserData[1]);
		} else {
			setaction_list(obj, actlist_13c54,obj->UserData[1]);
		}
		
	}
}

static void action_1a(Object *obj) {		// 13a3a
	int temp;
	
	static const short data_13ae6[] = {
		0xFFFC, 0x0002, 0xFFFD, 0xFFFE, 0x0003, 0x0000, 0x0002, 0x0001, 
		0xFFFE, 0x0002, 0x0004, 0x0002, 0x0002, 0xFFFE, 0xFFFE, 0xFFFF,
	};
	static const char data_13ac6 []={
		0x28, 0x03, 0x14, 0x05, 0x11, 0x02, 0x45, 0x08, 0x16, 0x07, 0x19, 0x02, 0x0A, 0x03, 0x32, 0x05,
		0x2B, 0x06, 0x0D, 0x0D, 0x28, 0x06, 0x0B, 0x07, 0x0D, 0x03, 0x23, 0x06, 0x17, 0x04, 0x24, 0x11
	};
	
	static const u16 data_13b06 [3][8][4]={{
		{0x0020, 0x0000, 0x0230, 0x0058, },
		{0xFFCE, 0x0000, 0x0180, 0x0048, },
		{0x0040, 0x0000, 0x0180, 0x0060, },
		{0xFFCF, 0x0000, 0x0190, 0x0010, },
		{0x0025, 0x0000, 0x0190, 0x0058, },
		{0xFFF0, 0x0000, 0x0200, 0x0048, },
		{0x0022, 0x0000, 0x0210, 0x0050, },
		{0xFFEE, 0x0000, 0x02C0, 0x0050, },
	}, {
		{0x0100, 0x0000, 0x0400, 0x0062, },
		{0x0110, 0x0000, 0x0540, 0x0053, },
		{0xFEA0, 0x0000, 0x0330, 0x0042, },
		{0xFEE0, 0x0000, 0x0130, 0x0053, },
		{0x0120, 0x0000, 0x0400, 0x0045, },
		{0x0120, 0x0000, 0x0530, 0x0063, },
		{0xFEEB, 0x0000, 0x0340, 0x0053, },
		{0xFED0, 0x0000, 0x0530, 0x0043, },
	}, {
		{0x0223, 0x0000, 0x0600, 0x0052, },
		{0xFEA0, 0x0000, 0x0480, 0x0040, },
		{0x0183, 0x0000, 0x0480, 0x0040, },
		{0xFE40, 0x0000, 0x0600, 0x0045, },
		{0x0223, 0x0000, 0x0600, 0x0054, },
		{0xFCD0, 0x0000, 0x0480, 0x0050, },
		{0x0143, 0x0000, 0x0480, 0x0040, },
		{0xFDD0, 0x0000, 0x0600, 0x0064, },
	}};
		
		
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->LocalTimer = data_13ac6[(sf2rand() & 0x3e)  ];
			obj->x001f      = data_13ac6[(sf2rand() & 0x3e)+1];
			obj-> XPI      += data_13ae6[RAND16WD];
			obj-> YPI      += data_13ae6[RAND16WD];
			
			temp = RAND8;
			obj->VelX.full = data_13b06[obj->UserData[2]][temp][0];
			obj->AclX.full = data_13b06[obj->UserData[2]][temp][1];
			obj->VelY.full = data_13b06[obj->UserData[2]][temp][2];
			obj->AclY.full = data_13b06[obj->UserData[2]][temp][3];
			
			sub_13c1a(obj);
			break;
		case 2:
			CATrajectory(obj);
			if (--obj->LocalTimer == 0) {
				NEXT(obj->mode0);
			}
			actiontick(obj);
			if (obj->x001f) {
				--obj->x001f;
				check_rect_queue_draw(obj);
			} else if ((obj->Timer & 1) == 0) {
				check_rect_queue_draw(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
		FATALDEFAULT;
	}
	
}



#pragma mark Act1B 

//13f18
static void action_1b(Object *obj) {
	static const short sounds[]={
		SOUND_ONE,		SOUND_TWO,		SOUND_THREE,	SOUND_FOUR,	
		SOUND_FIVE,		SOUND_SIX,		SOUND_SEVEN,	SOUND_EIGHT,
		SOUND_NINE
	};	// there is no zero
	
	Player *ply;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->XPI = 0x110;
			obj->YPI = 0x50;
			obj->LocalTimer = 0x7f;
			if (g.PlyLostToComp) {
				ply = &g.Player2;
				ply->SubSel = 1;
			} else {
				ply = &g.Player1;
				ply->SubSel = 0;
			}
			obj->Owner = ply;
			obj->LocalTimer = 0;		// ??
			/* FALLTHRU */
		case 2:
			if (obj->Owner->SubSel & (1 << g.ContinueBits)) {
				if (obj->LocalTimer != ply->ContinueSecs) {
					obj->LocalTimer = ply->ContinueSecs;
					queuesound(sounds[obj->LocalTimer]);
					setaction_list(obj, actlist_13fba, obj->LocalTimer);
				}
			}
			actiontick(obj);
			enqueue_and_layer(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		FATALDEFAULT;
	}
}

#pragma mark Act1C
static void action_1c(Object *obj) {
	static const u16 data_1585a[]={0x0200, 0xFEC0, 0x0180, 0xFEA0, 0x0160, 0xFE80, 0x0140, 0xFE00, };
	static const u16 data_1586a[]={0x04E0, 0x03C0, 0x04B0, 0x0390, 0x05A0, 0x03F0, 0x0510, 0x0480, };
	static const u16 data_1587a[]={0xFFD0, 0x0015, 0x0000, 0x0020, 0xFFEB, 0xFFE0, 0x0010, 0xFFF0, };
	static const u16 data_1588a[]={0x0020, 0x0000, 0x0008, 0x0005, 0x0015, 0x0013, 0x0010, 0x0017, };
	static const u16 data_1589a[]={0x0000, 0xFFFC, 0x0004, 0x0002, 0xFFFE, 0x0001, 0xFFFF, 0xFFFD, };
	static const u16 data_158ea[]={0x0280, 0x02A0, 0x0240, 0x0260, 0x0250, 0x02B0, 0x0290, 0x0270, };
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->UserData[0] = 0;
			obj->UserData[1] = 0;
			if (obj->SubSel >= 5) {
				obj->UserData[1] = 1;
				if (obj->UserByte) {
					obj->Pool = 6;
				}
			} else {
				if (0x9249 & (1<<RAND16)) {
					obj->Pool = 6;
				}
			}
			setaction_list(obj, actlist_158fe, obj->SubSel);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					if (obj->UserData[1]) {
						if (obj->UserByte) {
							if (obj->SubSel == 5) {
								obj->VelX.full = 0x400;
								obj->VelY.full = 0x680;
							} else {
								obj->VelX.full = 0x300;
								obj->VelY.full = 0x500;
							}							
						} else {
							if (obj->SubSel == 5) {
								obj->VelX.full = 0x300;
								obj->VelY.full = 0x580;
							} else {
								obj->VelX.full = 0x200;
								obj->VelY.full = 0x400;
							}
						}
					} else {
						obj->VelX.full = data_1585a[RAND8WD];
						obj->VelY.full = data_1586a[RAND8WD];
						obj->XPI += data_1587a[RAND8WD];
						obj->YPI += data_1588a[RAND8WD];
					}
					obj->UserData[2] = data_1589a[RAND8WD] + 0x28;
					if (obj->Step == 0) {
						obj->VelX.full = -obj->VelX.full;
						obj->Flip ^= 1;
					}
					obj->AclX.full = 0;
					obj->AclY.full = 0x0040;
					obj->LocalTimer = 0x50;
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
						obj->mode1 = 0;
					}
					CATrajectory(obj);
					if (obj->VelY.full < 0 && obj->YPI <= obj->UserData[2]) {
						obj->VelY.full = data_158ea[RAND8W];
					}
					break;
				FATALDEFAULT;
			}
			if (((g_d7 + g.libsplatter) & 1) == 0) {
				check_rect_queue_draw(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
		FATALDEFAULT;
	}
}

#pragma mark Act1D
static void sub_15cf2(u16 *gfx_p_a3, int d2, int d3) {
	// todo
}

static void action_1d(Object *obj) {		//16232
	UD1D *ud = (UD1D *)&obj->UserData;
	if (obj->SubSel) {
		// 1628e
		if (obj->mode0 == 0) {
			NEXT(obj->mode2);
			obj->LocalTimer = 60;
			u16 *gfx_p = coords_scroll2(0xa0, 0xb0);
			sub_15cf2(gfx_p, 0, ud->h0080c);
		}
	} else {
		if (g.x09fe == 1) {
			// 1627c
			if (ud->h0084c == 0) {
				++ud->h0084c;
//TODO				sub_7ce12();
			}
		} else if (g.x09fe == 2) {
			// 161ea
			if((g.libsplatter & 7) == 0) {
				//161f4 todo
			}
		} else {
			switch (ud->h0080c) {
//				case 0:	sub_7c90a(obj); break;
//				case 1:	sub_7c97e(obj); break;
//				case 2:	sub_7c9f8(obj); break;
//				case 3:	sub_7caa0(obj); break;
//				case 4:	sub_7cb38(obj); break;
//				case 5:	sub_7cbac(obj); break;
//				case 6:	sub_7cc54(obj); break;
//				case 7:	sub_7ccc8(obj); break;
				FATALDEFAULT;
			}
		}
	}
}


#pragma mark MARK

void action_1ab8a() {		/* 1ab8a */
	int i;
	for (i=0; i<8; i++) {
		OBJECT_DRAW_ID(64 + i, 0, 0, 0, 0);
	}
}

void action_b8aa() { 
	/* XXX meh japanese powermove tips
	 maybe doesn't belong in here */ 
}


static void sub_160ca(void) {
	gemu_setpalette(17+g.BattleLoser, data_160ea[g.BattleLoser]);
	/* sets palette from 900220 + (battleloser * 32) using copy32bytes */
}
void action_1606c() {
	Object *obj;
	
	sub_160ca();			/* set loser palette */
	u16 *gfx_p = CPS_OBJ(0x910070);
	u32 cp = MakePointObj(48, 96);
	_draw_frame_corners(&gfx_p, cp);
	
	if ((obj=AllocActor())) {
		struct UserData_Act1d *ud = (struct UserData_Act1d *)&obj->UserData;

		obj->exists = TRUE;
		obj->Sel = SF2ACT_0X1D;
		ud->h0080c = g.BattleLoser;
		obj->XPI = 0x10;
		obj->YPI = 0xb0;
		obj->SubSel = 0;
		ud->h0084c = 0;
		ud->x0088 = g.PlyLostToComp ? PLAYER1 : PLAYER2;
	}
}



#pragma mark Act1f Player Select Cursor
static void action_1f(Object *obj) {		//18e7e
	static const short data_18eb8[8][2] = {
		{   144,    64,  },
		{   176,    64,  },
		{   208,    64,  },
		{   240,    64,  },
		{   144,    32,  },
		{   176,    32,  },
		{   208,    32,  },
		{   240,    32,  },
	};
	
	Player *ply = obj->SubSel ? PLAYER2 : PLAYER1;
	switch (obj->mode0) {
		case 0:
			if (ply->Human) {
				NEXT(obj->mode0);
				setaction_list(obj, actlist_1a10c, obj->SubSel);
			}
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					actiontick(obj);
					if (ply->SelectComplete || ply->SelectDelayTmr) {
						NEXT(obj->mode1);
						setaction_list(obj, actlist_1a10c, obj->SubSel);
					}
					break;
				case 2:
					/* nothing */
					break;
				FATALDEFAULT;
			}
			obj->XPI = data_18eb8[ply->FighterID][0];
			obj->YPI = data_18eb8[ply->FighterID][1];
			enqueue_and_layer(obj);
			break;
			// no FreeActor(), I get killed.
		FATALDEFAULT;
	}
}

#pragma mark ACTION 0x20 hitstun splashes
static void action_20(Object *obj) {
	// 0-2 = blue splashes, 3 = big red splash, 4-5=fireballsplash
	static const POINT16 data_1de94[4] = {{ -2, 0}, {2, -2}, {0, 0}, {-2, 2}};
	short temp;
	if (obj->mode3 == 5) {
		if (obj->LocalTimer) {
			--obj->LocalTimer;
		} else {
			g.libsplatter = 1;
		}
	} else {
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				obj->Draw1 = FALSE;
				if (obj->SubSel < 3) {
					obj->Draw1 = TRUE;
					obj->Draw2.part.integer = obj->Owner->Side ? 14 : 11;
				} else if (obj->SubSel == 5) {
					obj->Draw1 = TRUE;
					obj->Draw2.part.integer = 15;
				}
				temp = (sf2rand() & 0xc ) >> 2;
				obj->XPI += data_1de94[temp].x;
				obj->YPI += data_1de94[temp].y;
				setaction_list(obj, actlist_1dec6, obj->SubSel);
				break;
			case 2:
				actiontick(obj);
				if (obj->AnimFlags & 0x8000) {
					NEXT(obj->mode0)
				} else { 
					if (obj->SubSel > 3) {
						obj->Draw1 = obj->AnimFlags & 0xff;
					}
					check_rect_queue_draw(obj);
				}
				break;
			case 4:
			case 6:
				FreeActor(obj);
				break;
			default:
				break;
		}
	}
}

#pragma mark Act21

void action_start_21(void) {		//1152a
	Object *obj;
	if ((obj = AllocActor())) {
		obj->exists = 1;
		obj->Sel    = SF2ACT_0X21;
	}
}

static void sub_117f8() {
	if (g.Debug && (g.JPCost & 0x4)) {
		
		/* Debugging stuff todo later */
	}
}
static void sub_11820(void) {
	if (g.Debug && (g.JPCost & JP_DBGTACTICS)) {
		/* debugging todo */
	}
}
static void action_21(Object *obj) {    // 1153e
	switch (obj->mode0) {
		case 0:
			g.x8a5e = FALSE;
			NEXT(obj->mode0);
			obj->Scroll = SCROLL_NONE;
			obj->Pool	= 2;
			obj->XPI	= 192;
			obj->YPI	= 207;
			setaction_direct(obj, actlist_117aa);	/* Flashing KO */
			/* FALLTHRU */
		case 2:
			switch (obj->mode1) {
				case 0:
					if (g.Player1.Energy < 30 || g.Player2.Energy < 30) {
						NEXT(obj->mode1);
						
						// Speed up the stage music
						queuesound((const short []){
							0x0079, 0x007a, 0x007b, 0x007c,             
							0x007d, 0x007e, 0x007f, 0x0080,                       
							0x0084, 0x0083, 0x0081, 0x0082
						}[g.CurrentStage]);
					}
					break;
				case 2:
					actiontick(obj);
					if (g.Player1.Energy < 0 || g.Player2.Energy < 0) {
						NEXT(obj->mode1);
						setaction_direct(obj, actlist_117aa);
					}
					break;
				case 4:
					/* does nothing */
					break;
					FATALDEFAULT;
			}
			sub_117f8();		/* debugging stuff */
			sub_11820();		/* more debugging stuff */
			if (g.x8a5e == 0) {
				enqueue_and_layer(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
			FATALDEFAULT;
	}
}

#pragma mark Act22 RoundXFight!

void action_start_22(void) { //  1abc6  round X .. fight!
	const static u16 pals_1abf6[]={
		0xf000, 0xffff, 0xfccb, 0xf887, 0xf8ff, 0xf4cc, 0xf1aa, 0xfcef,                             
		0xfaef, 0xf8cf, 0xf69e, 0xf46e, 0xf04d, 0xf02c, 0xf00b, 0xf00a,
	};
	
	Object *obj;
	if ((obj=AllocActor())) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_PREROUNDANIM;
	}
	if ((g.CurrentStage == STAGE_THAILAND_BISON) && (g.RoundCnt != 0)) {
		gemu_setpalette(11, pals_1abf6);
	}
}


static void sub_1acaa(Object *obj) {
	if (g.OnBonusStage) {
		switch (obj->mode1) {
			case 0:
				NEXT(obj->mode1);
				obj->Draw2.full	= 0x40;
				obj->LocalTimer = 120;
				setaction_direct(obj, actlist_1af46);	/* BONUS STAGE */
				break;
			case 2:		//1adcc
				if (obj->Draw2.full) {
					--obj->Draw2.full;
				}
				if(--obj->LocalTimer == 0){
					NEXT(obj->mode1);
				}
				break;
			case 4:		//1ade2
				--obj->Draw2.full;
				if(obj->Draw2.full < 0 && obj->Draw2.full > -16) {return;}
				NEXT(obj->mode1);
				setaction_direct(obj, actlist_1af7e);	/* START */
				break;
			case 6:		//1ae00
				if (obj->Draw2.full) {
					++obj->Draw2.full;
				}
				if (--obj->LocalTimer == 0) {
					NEXT(obj->mode0);
					obj->mode1 = 0;
				}
				break;
				FATALDEFAULT;
		}
	} else {
		switch (obj->mode1) {
			case 0:
				NEXT(obj->mode0);
				obj->Draw2.full = 0xfff0;	
				setaction_list(obj, actlist_1ae1a, g.RoundCnt);
				break;
			case 2:
				if (obj->Draw2.full) {
					obj->Draw2.full += 1;
				} else {
					NEXT(obj->mode1);
					obj->LocalTimer = 40;
					if (g.RoundCnt == 9) {
						queuesound(SOUND_FINAL);
					} else {
						queuesound(SOUND_ROUND);
					}
				}
				break;
			case 4:
				if (--obj->LocalTimer == 0) {
					NEXT(obj->mode1);
					obj->LocalTimer = 30;
					if (g.RoundCnt == 9) {
						queuesound(SOUND_ROUND);
					} else {
						queuesound(g.RoundCnt + SOUND_ONE);
					}
				}
				break;
			case 6:
				if (--obj->LocalTimer) {
					NEXT(obj->mode1);
				}
				break;
			case 8:
				--obj->Draw2.full;
				if(obj->Draw2.full < 0 && obj->Draw2.full > -16) {return;}
				NEXT(obj->mode1);
				g.x0305 = FALSE;		// Queue the Bison Cape remove anim
				setaction_direct(obj, actlist_1af62);		/* FIGHT! */
				break;
			case 10:
				if (obj->Draw2.full) {
					obj->Draw2.full += 2;
				} else {
					NEXT(obj->mode1);
					obj->LocalTimer = 0x32;
					queuesound(SOUND_FIGHT);
				}
				break;
			case 12:
				if (--obj->LocalTimer == 0) {
					obj->mode1 = 0;
					NEXT(obj->mode0);
				}
				break;
			default:
				break;
		}
	}
}
static void action_22(Object *obj) {		// 1ac16
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool   = 2;
			obj->Scroll = SCROLL_NONE;
			obj->XPI	= 0xc0;
			obj->YPI	= 0x90;
			obj->Draw1  = -1;
			obj->Draw2.full  = 0;
			obj->Step	= 0x11;
			if (g.OnBonusStage) {
				return;
			}
			showtextbank1(0x14);	/* BATTLE */
			
			/* XXX  todo */
			
			break;
		case 2:
			sub_1acaa(obj);
			if (g.NewPlayers == 0) {
				enqueue_and_layer(obj);
			}
			break;
		case 4:
			NEXT(obj->mode0);
			action_1ab8a();
			break;
		case 6:
			g.PreRoundAnim = FALSE;
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}
	

#pragma mark ACTION 0x23 diziness, pukes
/* Vomit, birds, stars etc */
// gone to reels.c
#pragma mark ----

#pragma mark Act24
/*
static void sub_1b14e(Object *obj) {
	obj->mode1 = 0;
	ud->h0088w = g.CPS.Scroll3Y;
	ud->h0080l = 0xffff6000;
	ud->h0084l = 0x00001000;	
}

static void action_NN(Object *obj) {
	UDXX *ud = (UDXX *)obj->UserData;
	switch (obj->SubSel) {
		case 0:
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					sub_1b14e(obj);
					break;
				case 2:
					if (obj->mode1) {
						if (--obj->LocalTimer == 0) {
							sub_1b14e(obj);
						}						
					} else {
						gstate_Scroll3.Y.full = ud->h0080l;
						ud->h0080l += ud->h0084l;
						g.CPS.Scroll3Y = gstate_Scroll3.YPI;
//						if (g.CPS.Scroll3Y <<>> ud->h0088w) {
//							//todo
//						}
					}
					break;
				case 4:
				case 6:
					FreeActor(obj);
					break;
				FATALDEFAULT;
			}
			break;
		case 1:
			break;
		case 2:
		case 3:
		case 4:
			break;
	}
}
*/


void action_1cd3c(Player *ply) {
	/* XXX vegas claw falls off */
}

#pragma mark MARK

static void _draw_portrait_beaten(u16 **scr_p, char d2, char d3) {		//15cfr2
	static const u16 *data_15d36[60]={
		data_17522, data_17522, data_17608, data_17608, 
		data_17522, data_176ee, data_177d4, data_178ba, 
		data_179a0, data_17a86, data_17b6c, data_17c52, 
		data_17d38, data_17b6c, data_17b6c, data_17e1e, 
		data_17f04, data_17f04, data_17e1e, data_17e1e, 
		data_17fea, data_17fea, data_180d0, data_180d0, 
		data_17fea, data_181b6, data_181b6, data_1829c, 
		data_1829c, data_181b6, data_18382, data_18382, 
		data_18468, data_18468, data_18382, data_1854e, 
		data_18634, data_1871a, data_1854e, data_18634, 
		data_18800, data_18800, data_18800, data_18800, 
		data_18800, data_188e6, data_188e6, data_188e6, 
		data_188e6, data_188e6, data_189cc, data_189cc, 
		data_189cc, data_189cc, data_189cc, data_18ab2, 
		data_18ab2, data_18ab2, data_18ab2, data_18ab2, 
	};
	/* next address 00015dae */
	
	
	/* anti-copying wierdness, fuck it */
	d3 *= 11;
	d3 += ((sf2rand() & 0xe) + (g.libsplatter & 6)) % 10;
	
	_draw_portrait_scr2((const SimpleImage *)data_15d36[d3/2], d2, scr_p);
}



void action_1286e(Object *obj, short d0) {	
	g.x8a62[d0]=60;
	add_bcd_8(1, &g.x8a60[d0]);
}

static void _draw_frame_corners(u16 **gfx_p, u32 cp ){	/* 15df6 */
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, TILE_FRAMECORNER, 0x1f | ATTR_X_FLIP);
	INC_GFX_CURSOR(&cp, 112, 0);
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, TILE_FRAMECORNER, 0x1f | ATTR_NO_FLIP);
}

static void _draw_portrait_scr2(const SimpleImage *a1, short flip, u16 **scr_p) {			// 1601c
	short width, height;
	const u16 *img = (const u16 *)a1;
	width  = *img++;
	height = *img++;
	img++;
	sub_4386(flip, width, height, img, scr_p);	/*backside of drawsimple_scroll2attr_nocheck */
}

static void _draw_portrait_prefight(u16 **scr_p, short flip, short fid_d3) {		// 15c02
	static const u16 *data_15c16[12]={
		data_16a5a, data_16b40, data_16c26, data_16d0c, 
		data_16df2, data_16ed8, data_16fbe, data_170a4, 
		data_1718a, data_17270, data_17356, data_1743c, 
	};
	_draw_portrait_scr2((const SimpleImage *)data_15c16[fid_d3], flip, scr_p);		/* offsets to simpleimages x 18 */
}
static void sub_15ff0(u16 **scr_p, short flip, short fid_d3) {
	static const u16 *data_16004[12]={
		data_16a5a, data_16b40, data_16c26, data_16d0c, 
		data_16df2, data_16ed8, data_16fbe, data_170a4, 
		data_1718a, data_17270, data_17356, data_1743c, 
	};
	
	_draw_portrait_scr2((const SimpleImage *)data_16004[fid_d3], flip, scr_p);			/* 12 * offset simpleimage */
}

/* portrait drawing, not scheduled */
void draw_portraits_prefight(void) {		/* 15f9e */
	u16 *gfx_p;
	u16 *scr_p;
	
	gfx_p = CPS_OBJ(0x910080);
	_draw_frame_corners(&gfx_p,  MakePointObj(240, 112));		// draw corners
	scr_p = coords_scroll2(240, 192);
	sub_15ff0(&scr_p, FALSE, g.Player2.FighterID);		// was 0

	gfx_p = CPS_OBJ(0x910070);
	_draw_frame_corners(&gfx_p, MakePointObj(16, 112));
	scr_p = coords_scroll2(16, 192);
	sub_15ff0(&scr_p, TRUE, g.Player1.FighterID);		// was 1
}

void draw_portraits_postfight(void) {	/* 15c2e after a fight, one beaten up */
	u16 *gfx_p;
	u16 *scr_p;
	
	if(g.BattleWinSide) {
		gfx_p=CPS_OBJ(0x910070);
		_draw_frame_corners(&gfx_p, MakePointObj(16, 112));			
		
		scr_p = coords_scroll2(16, 192);
		_draw_portrait_beaten(&scr_p, TRUE, g.BattleLoser);
		
		gfx_p=CPS_OBJ(0x910080);
		_draw_frame_corners(&gfx_p, MakePointObj(240, 112));
		
		scr_p = coords_scroll2(240, 192);
		_draw_portrait_prefight(&scr_p, FALSE, g.BattleWinner);		
	} else {
		gfx_p=CPS_OBJ(0x910070);
		_draw_frame_corners(&gfx_p, MakePointObj(16, 112));
		
		scr_p = coords_scroll2(16, 192);
		_draw_portrait_prefight(&scr_p, TRUE, g.BattleWinner);
		
		gfx_p=CPS_OBJ(0x910080);
		_draw_frame_corners(&gfx_p, MakePointObj(240, 112));
		
		scr_p=coords_scroll2(240, 192);
		_draw_portrait_beaten(&scr_p, FALSE, g.BattleLoser);
	}
}


#pragma mark Act2C Playerselect something

#pragma mark Act2E Plane
//act2e_plane.c

#pragma mark Act2f	Attract sequence fighters
static void skyskraperanim_00(Object *obj) {		// 1d176
	UD2F *ud = (UD2F *)obj->UserData;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			g.x8a74   = 0;
			g.x8a75   = 0xf0;
			obj->Pool = 4;
			ud->OldY  = obj->YPI;
			setaction_list(obj, actlist_1d4f0, 0);
			sub_25f8(obj);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					if (--g.x8a75 == 0) {
						NEXT(obj->mode1);
						g.x8a74  =  1;
						g.x8a75  = 16;
						obj->XPI = 0xc8;
						setaction_list(obj, actlist_1d4f0, 2);
					} else {
						obj->YPI = ud->OldY;
						obj->YPI |= ((g.x8a75 & 0x10)>>3);
					}
					sub_25f8(obj);
					break;
				case 2:
					if (--g.x8a75 == 0) {
						NEXT(obj->mode1);
						g.x8a74   = -1;
						g.x8a75   = 1 * TICKS_PER_SECOND;
						obj->XPI  = 0xb0;
						obj->YPI  = 0x610;
						obj->Pool = 4;
						setaction_list(obj, actlist_1d4f0, 3);
					}
					sub_25f8(obj);
					break;
				case 4:
					if (--g.x8a75 == 0) {
						g.Pause_9e1 = 1;
					}
					sub_25f8(obj);
					break;
				FATALDEFAULT;
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}
static void skyskraperanim_02(Object *obj) {		// 1d250
	UD2F *ud = (UD2F *)obj->UserData;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 4;
			ud->OldY = obj->YPI;
			setaction_list(obj, actlist_1d4f0, 1);		// black guy
			sub_25f8(obj);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					if (g.x8a74 == 0) {
						obj->YPI = ud->OldY;
						obj->YPI |= ((g.x8a75 & 0x20)>>4);
						sub_25f8(obj);
					} else {
						NEXT(obj->mode1);
					}
					break;
				case 2:
					if (g.x8a74 < 0) {
						NEXT(obj->mode1);
						obj->XPI = 0x58;
						obj->YPI = 0x610;
						obj->Pool = 4;
						obj->LocalTimer = 6;
						setaction_list(obj, actlist_1d4f0, 4);	// black guy reeling backwards
						sub_25f8(obj);
					}
					break;
				case 4:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
					}
					break;
				case 6:
					obj->XPI -= 2;
					obj->YPI -= 1;
					sub_25f8(obj);
					break;
				FATALDEFAULT;
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}
static void skyskraperanim_04(Object *obj) {		// 1d308
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 4;
			setaction_list(obj, actlist_1d4f0, 5);		// punched head
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					if (g.x8a74) {
						NEXT(obj->mode1);
					}
					break;
				case 2:
					if (g.x8a74 < 0) {
						NEXT(obj->mode1);
						obj->XPI = 0x38;
						obj->YPI = 0x640;
						obj->Pool = 6;
						obj->LocalTimer = 6;
						sub_25f8(obj);
					}
					break;
				case 4:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
					}
					break;
				case 6:
					obj->XPI -= 2;
					obj->YPI -= 1;
					sub_25f8(obj);
					break;
				FATALDEFAULT;
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}
static void skyskraperanim_06(Object *obj) {		// 1d394
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			setaction_list(obj, actlist_1d4f0, 6);		// bang
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					if (g.x8a74) {
						NEXT(obj->mode1);
					}
					break;
				case 2:
					if (g.x8a74 < 0) {
						NEXT(obj->mode1);
						soundsting(SOUND_IMPACT3);
						obj->XPI = 0x40;
						obj->YPI = 0x650;
						obj->Pool = 6;
						obj->LocalTimer = 6;
						sub_25f8(obj);
					}
					break;
				case 4:
					if (obj->LocalTimer) {
						--obj->LocalTimer;
						sub_25f8(obj);
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}
static void skyskraperanim_08(Object *obj) {		// 1d41a teeth
	UD2F *ud = (UD2F *)obj->UserData;

	static const u32 data_1d4ac[] = {0x0002c000, 0x00030000, 0x00034000, 0x00028000};
	static const u32 data_1d4bc[] = {0x00020000, 0x00010000, 0x00030000, 0x00000000};
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 4;
			ud->VelY.full    = 0x00000000;
			ud->Gravity.full = 0xffffc000;
			setaction_list(obj, actlist_1d4f0, 7);		// Teeth
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					if (g.x8a74) {
						NEXT(obj->mode1);
					}
					break;
				case 2:
					if (g.x8a74 < 0) {
						NEXT(obj->mode1);
						soundsting(SOUND_IMPACT3);
						obj->XPI = 0x30;
						obj->YPI = 0x640;
						obj->Pool = 4;
						obj->LocalTimer = 10;
						ud->VelX.full = data_1d4ac[obj->Step];
						ud->VelY.full = data_1d4bc[obj->Step];
						sub_25f8(obj);
					}
					break;
				case 4:
					obj->X.full -= ud->VelX.full;
					obj->Y.full += ud->VelY.full;
					ud->VelY.full += ud->Gravity.full;
					actiontick(obj);
					sub_25f8(obj);
					break;
				FATALDEFAULT;
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}

void action_2f(Object *obj) {		// 1d160
	switch (obj->SubSel) {
		case 0:			skyskraperanim_00(obj);		break;
		case 2:			skyskraperanim_02(obj);		break;
		case 4:			skyskraperanim_04(obj);		break;
		case 6:			skyskraperanim_06(obj);		break;
		case 8:			skyskraperanim_08(obj);		break;	// teeth
			break;
		FATALDEFAULT;
	}
}

#pragma mark Act30 cheering people on skycraper fight

static void action_30(Object *obj) {		// 1da4a
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->XPI += gstate_Scroll3.XPI;
			obj->YPI += gstate_Scroll3.YPI;
			obj->Pool = 6;
			setaction_direct(obj, actlist_1da8c);
			/* FALLTHRU */
		case 2:
			actiontick(obj);
			check_rect_queue_draw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
			FATALDEFAULT;
	}
}
#pragma mark Act33 random stage decor
static void action_33(Object *obj) {
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Flip = obj->Step;
			obj->Pool = obj->UserByte;
			setaction_list(obj, actlist_1f502, obj->SubSel);
			break;
		case 2:
			check_rect_queue_draw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		FATALDEFAULT;
	}
}


#pragma mark Act35 Ground Dust

void ActStartGroundDust(Player *ply) {			// 1f980
	Object *obj;
	if ((obj=AllocActor())) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_GROUNDDUST;
		if (ply->ProjHit) {
			obj->Draw1 = TRUE;
			obj->Draw2.part.integer = (ply->ProjHit == 2) ? 0x11 : 0xf ; 
		}
		obj->XPI = ply->XPI;
		obj->YPI = ply->YPI;
		obj->Flip = ply->Flip;
	}
}
static void action_35(Object *obj) {			//1f9fa
	short d0;
	const char *data;
	UD35 *ud = (UD35 *)obj->UserData;

	switch (obj->mode0) {
		case 0:
			if (obj->SubSel) {
				if (--obj->LocalTimer == 0) {
					NEXT(obj->mode0);
					ud->x0084 = (char *)&data_1fb1a;
				}
			} else {
				NEXT(obj->mode0);
				ud->x0080 = obj->XPI;
				ud->x0082 = obj->YPI;
				ud->x0084 = (char *)&data_1fab6;
			}
			break;
		case 2:
			if (obj->SubSel) {
				ud->x0080 = obj->Owner->XPI;
				ud->x0082 = obj->Owner->YPI;
			}
			data = ud->x0084;
			if (obj->Step) {
				d0 = *data++;		// char
			} else {
				d0 = -(*data++);
			}
			obj->XPI = ud->x0080 + d0;
			d0 = *data++;
			obj->YPI = ud->x0082 + d0;
			ud->x0084 += 3;
			d0 = *data;
			if (d0 == 0) {
				obj->flag1 = 0;
			} else if (d0 < 0) {
				NEXT(obj->mode0);
			} else if (g.CurrentStage == STAGE_JAPAN_EHONDA) {
				setaction_list(obj, actlist_1fb52, d0);			// water splashes
				check_rect_queue_draw(obj);
			} else {
				setaction_list(obj, actlist_1fb46, d0);			// dust clouds
				check_rect_queue_draw(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		default:
			break;
	}
}


#pragma mark Act36 ScreenWobble

void ActStartScreenWobble(void){			// 1fd9e
	Object *obj;
	if (g.FastEndingFight == FALSE && g.ScreenWobbleStarted == FALSE) {
		if ((obj=AllocActor())) {
			obj->exists = TRUE;
			obj->Sel = SF2ACT_SCREENWOBBLE;
			g.ScreenWobbleStarted = TRUE;
		}
	}
}

static void sub_1fe36(Object *obj) {
	UD36 *ud = (UD36 *)obj->UserData;

	g.ScreenWobbleMagnitude = 0;
	gstate_Scroll1.YPI = ud->Save_Scroll1Y;
	gstate_Scroll2.YPI = ud->Save_Scroll2Y;
	gstate_Scroll3.YPI = ud->Save_Scroll3Y;
}
static void sub_1fe4e(Object *obj, short adjust) {	/* per stage wobbles */
	UD36 *ud = (UD36 *)obj->UserData;

	switch (g.CurrentStage) {
		case STAGE_JAPAN_RYU:			// 1fea4
			if (gstate_Scroll1.mode1) {
				ud->Save_Scroll1Y = 0x100;
			}
			gstate_Scroll2.YPI += adjust;
			gstate_Scroll3.YPI += adjust;
			break;
		case STAGE_JAPAN_EHONDA:
		case STAGE_USA_GUILE:
	    case STAGE_USA_KEN:
		case STAGE_CHINA_CHUNLI:
		case STAGE_INDIA_DHALSIM:
		case STAGE_THAILAND_BISON:
		case STAGE_THAILAND_SAGAT:
		case STAGE_SPAIN_VEGA:
		case STAGE_BONUS_CAR:
		case STAGE_BONUS_UNKNOWN:			// 1fe96
			gstate_Scroll1.YPI += adjust;
			gstate_Scroll2.YPI += adjust;
			gstate_Scroll3.YPI += adjust;
			break;
		case STAGE_BRAZIL_BLANKA:
		case STAGE_USA_BALROG:
		case STAGE_BONUS_BARRELS:
		case STAGE_BONUS_DRUMS:
			gstate_Scroll2.YPI += adjust;
			gstate_Scroll3.YPI += adjust;
			break;
		case STAGE_USSR_ZANGIEF:
			gstate_Scroll1.YPI += adjust;
			gstate_Scroll2.YPI += adjust;
			break;
		default:
			break;
	}
}

void action_36(Object *obj) {		/* 1fdc4 */
	UD36 *ud = (UD36 *)obj->UserData;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->LocalTimer = 8;
			obj->x001f = 7;
			ud->Save_Scroll1Y = gstate_Scroll1.YPI;
			ud->Save_Scroll2Y = gstate_Scroll2.YPI;
			ud->Save_Scroll3Y = gstate_Scroll3.YPI;
			break;
		case 2:
			if (--obj->x001f < 0) {
				if(--obj->LocalTimer == 0) {
					NEXT(obj->mode0);
					sub_1fe36(obj);
					return;
				} else {
					obj->x001f = 7;
				}
			}
			if (obj->x001f & 1) {
                g.ScreenWobbleMagnitude = 3;
                ud->Save_Scroll1Y = gstate_Scroll1.YPI;
                ud->Save_Scroll2Y = gstate_Scroll2.YPI;
                ud->Save_Scroll3Y = gstate_Scroll3.YPI;
                sub_1fe4e(obj, 3);
            } else {
                sub_1fe36(obj);
			}
			break;
		case 4:
		case 6:
			g.ScreenWobbleStarted = FALSE;
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
	
}

#pragma mark Act37

static void action_37(Object *obj) {	//1fc84
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			obj->UserData[0] = 0;
			obj->Flip = obj->Step;
			setaction_list(obj,actlist_1fcfa , 0);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->LocalTimer = (char []){8,11,11,3,6,11,2,4}[RAND8];
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						obj->mode1 = 0;
						obj->UserData[0] ^= 1;
					}
					break;
				FATALDEFAULT;
			}
			check_rect_queue_draw(obj);
			break;
		FATALDEFAULT;
	}
}


#pragma mark Act38 TimeWarp

static void action_38(Object *obj) {		// 1fd50
	if (g.OnBonusStage) {
		g.FlagTimeWarp = FALSE;
		FreeActor(obj);
	} else {
		if (obj->UserByte || obj->Owner->Timer2 == 0) {
			if (obj->SubSel) {
				g.TimeWarpTimer     = 180;		/* duration of Time Warp */
				g.TimeWarpSlowdown  = 3;		/* slowdown factor */
				g.TimeWarpSlowdown2 = 3;		/* reloaded from here */
			} else {
				g.TimeWarpTimer     = 1 * TICKS_PER_SECOND;
				g.TimeWarpSlowdown  = 2;
				g.TimeWarpSlowdown2 = 2;
			}
			FreeActor(obj);
		} 
	}
}

#pragma mark Act39 Nametiles

static void action_39(Object *obj) {	// 1fec2
	Player *ply = obj->SubSel ? PLAYER2 : PLAYER1;
	
	static signed char data_1ff20[] = {
		0x8c, 0x74, 0x8c, 0x74, 0x8c, 0x74, 0x8c, 0x74, 
		0x94, 0x78, 0x8c, 0x74, 0x8c, 0x72, 0x92, 0x6e,
		0x96, 0x74, 0x90, 0x72, 0x90, 0x7a, 0x90, 0x74,
	};
	static signed char data_1ff38[] = {
		0x8c, 0x74, 0x8c, 0x74, 0x8c, 0x74, 0x8c, 0x74, 
		0x94, 0x78, 0x8c, 0x74, 0x8c, 0x72, 0x92, 0x6e,
		0x90, 0x7a, 0x90, 0x72, 0x90, 0x74, 0x96, 0x74,
	};
	
	
	switch (obj->mode0) {
		case 0:
			if (g.Version != VERSION_JAP) {
				obj->XPI += data_1ff38[(ply->FighterID * 2)+ply->Side];
			} else {
				obj->XPI += data_1ff20[(ply->FighterID * 2)+ply->Side];
			}			
			break;
		case 2:
			check_rect_queue_draw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
	
	
}

#pragma mark Act3A Cape

static void action_3a(Object *obj) {		// 201a0
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 0;
			if (obj->SubSel) {
				obj->XPI += 0x1b;
				if (obj->Owner->Flip) {
					obj->XPI += 10;
				}
				obj->VelX.full = 0x0100;
			} else {
				obj->XPI -= 40;
				if (obj->Owner->Flip) {
					obj->XPI += 0x10;
				}
				obj->VelX.full = 0xff00;
			}
			obj->YPI = 0x75;
			obj->VelY.full = 0x100;
			obj->AclY.full = 0x0018;
			obj->AclX.full = 0;
			quirkysound(2);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					CATrajectory(obj);
					if (obj->YPI <= 40) {
						NEXT(obj->mode1);
						obj->YPI = 40;
					}
					if (obj->AnimFlags & 0xff) {
						actiontick(obj);
					}
					break;
				case 2:
					actiontick(obj);
					break;
				default:
					break;
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}


#pragma mark Act3B Score Counters


/*!
 @abstract draw BCD encoded score to tile RAM
 @param bid-endian BCD encoded score (%d0)
 @param pointer to tile RAM (%a1)
 @discussion sf2ua:0x205f6
 */

static void sub_205f6(u32 d0, short d2, u16 *a1) {
    printf("print counter %08x\n", d0);
	d2 -= 3;
	while (d2 >= 0) {
        --a1;
		*a1 = (d0 & 0xf) + 0x8100;
		d2--;
		d0 >>= 4;
	}
}
static void _init_counter_image(Object *obj) {		//20610
	const u16 *source;
	u16 *dest;
	int i;

	/* all same anyway               tiles pal   */
	const static u16 data_20640[] = {0x1, 0xd, 0x2d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8100};
	const static u16 data_20654[] = {0x1, 0xd, 0x2d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8100};
	const static u16 data_20668[] = {0x1, 0xe, 0x2d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8100}; // todo: put palette back to 0xd

	
	switch (obj->SubSel) {
		case 0:
			source = data_20640; dest = g.TimeBonusSprite; break;
		case 2:
			source = data_20654; dest = g.VitalBonusSprite; break;
		case 4: 
			source = data_20668; dest = g.TotalBonusSprite; break;
		FATALDEFAULT;
	}
	for (i=0; i<10; i++) {
		dest[i] = source[i];
	}
}

static void sub_205d8(Object *obj) {
	UD3B *ud = (UD3B *)&obj->UserData;
	add_bcd_32(ud->x0080, &g.x8ab0);
	start_effect(0x2002, g.RoundWinnerSide);
}

static void action_3b(Object *obj) {	//203ba
	UD3B *ud = (UD3B *)&obj->UserData;
	
	Player *ply;
	const Action *anim;
	short d2;
	int d0;
	
	//coordinates of score counters
	static const short data_20412[][2] = {
		{216, 176},
		{216, 160},
		{216, 136}, 
	};
	
	/* perfect energy score per player */
	static const short data_204d8[12] = {
		0x300, 0x300, 0x300, 0x300, 
		0x300, 0x300, 0x300, 0x300, 
		0x800, 0x500, 0x500, 0x500,		// Bison, three other bosses
	};
	
	static const Action action_2067c = 
        {8, 0, 0, (Image *)&g.TimeBonusSprite, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static const Action action_20694 = 
        {8, 0, 0, (Image *)&g.VitalBonusSprite, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static const Action action_206ac = 
        {8, 0, 0, (Image *)&g.TotalBonusSprite, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	
	switch (obj->mode0) {
		case 0:							// init the animation
			NEXT(obj->mode0);
			obj->Scroll    = SCROLL_NONE;
			obj->Pool      = 2;
			ud->x0080      = 0;
			
			_init_counter_image(obj);
			
			obj->XPI = data_20412[obj->SubSel/2][0];
			obj->YPI = data_20412[obj->SubSel/2][1];
			switch (obj->SubSel) {
				case 0:												// TIME
					ud->x0080 = (g.TimeRemainBCD << 12);
					if (g.TimeRemainBCD != 0) {
						d2 = (g.TimeRemainBCD & 0xf0) ? 4 : 3;		// number of tiles
						g.TimeBonusSprite[0] = d2;
						g.TimeBonusSprite[8] = 0x8100;
						sub_205f6(g.TimeRemainBCD, d2, &g.TimeBonusSprite[7]);
					}
					g.x8ab4 |= 1;
					anim = &action_2067c;  
					break;
				case 2:												// VITAL
					ply = g.RoundWinnerSide ? PLAYER2 : PLAYER1;
					if (ply->Energy) {
						if (ply->Energy < 0x90) {
							// 20494
							bin2bcd(ply->Energy);
							d0 = g.x8a42;
						} else {
							queuesound(SOUND_PERFECT);
							d0 = data_204d8[ply->Opponent->FighterID];
						}
						ud->x0080 = d0 << 16;
						if (d0 < 0x10) {
							d2 = 3;
						} else if (d0 < 0x100) {
							d2 = 4;
						} else {
							d2 = 5;
						}
						g.VitalBonusSprite[0] = d2;
						g.VitalBonusSprite[8] = 0x8100;
						sub_205f6(d0, d2, &g.VitalBonusSprite[7]);
					}
					g.x8ab4 |= 2;
					anim = &action_20694;
					break;
				case 4:											// TOTAL
					ud->x0080 = g.x8ab2 << 16;
					anim = &action_206ac;
					break;
				FATALDEFAULT;
			}
			setaction_direct(obj, anim);
			break;
		case 2:							// 204fc animate and decrement score
			switch (obj->SubSel) {
				case 0:
					if (g.CanSpeedUpScoreCount) {
						if (ud->x0080 & 0xff000000) {
							sub_bcd_32(1, &ud->x0080);
							ud->x0080 &= 0xff000000;
							ud->x0080 |= 0x00000001;
							sub_205d8(obj);
						} else {
							g.x8ab4 &= 0xfffffffe;
						}
					}
					break;
				case 2:
					if (g.CanSpeedUpScoreCount) {
						if (ud->x0080 & 0xffff0000) {
							sub_bcd_32(1, &ud->x0080);	// XXX
							ud->x0080 &= 0xff000000;
							ud->x0080 |= 0x00000001;
							sub_205d8(obj);
						} else {
							g.x8ab4 &= 0xfffffffd;
						}						
					}
					break;
				case 4:
					g.CanSpeedUpScoreCount = TRUE;
					if (g.x8ab2) {
						if (g.x8ab2 < 0x10) {
							d2 = 3;
						} else if (g.x8ab2 < 0x100) {
							d2 = 4;
						} else {
							d2 = 5;
						}
						sub_205f6(g.x8ab2, d2, &g.TotalBonusSprite[6]);
						if ((g.libsplatter & 3)==0 && g.x8ab2 != ud->x0080) {
							queuesound(SOUND_UNK_DING);	/* Ding! */
						}
					}
					setaction_direct(obj, &action_206ac);
					break;
				FATALDEFAULT;
			}
			if (g.x8a5e == 0) {
				enqueue_and_layer(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}

#pragma mark Projectile 207f0

#pragma mark Act43
static void action_43(Object *obj) {        //219ce
	if (obj->SubSel) {
		//21adc todo
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				obj->Pool       = 6;
				obj->LocalTimer = 40;
                
				//todo
//				memcpy(gfx_p, data_21b2a, 160);
//				setaction_list(obj, data_21f26, 0);
				break;
			case 2:
				if (obj->LocalTimer) {
					--obj->LocalTimer;
				} else {
					if (g.mode2 == 0xc) {
						FreeActor(obj);
					} else {
						actiontick(obj);
						enqueue_and_layer(obj);
					}
				}
				break;
			case 4:
			case 6:
				FreeActor(obj);
				break;
			FATALDEFAULT;
		}
	} else {
		if (g.Version != VERSION_JAP) {
			// 21a66
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->Pool  = 2;
					obj->Draw1 = -1;
					obj->Step  = 0;
					obj->Draw2.full = 0xfff0;
					obj->YPI += 0x50;
					obj->LocalTimer = 0x80;
					obj->x002e = 0x40;
//					setaction_list(obj, data_21bea, 1); TODO
					break;
				case 2:
					if (g.mode2 == 0xc) {
						queuesound(SOUND_GAME_OVER);
						print_libtextgfx(0x15);
						FreeActor(obj);
					} else {
						if (obj->mode1 == 0) {
							++obj->Draw2.full;
							if(--obj->x002e == 0) {
								obj->mode1 = 2;
							}
						}
						enqueue_and_layer(obj);
					}
					break;
				case 4:
				case 6:
					FreeActor(obj);
					break;
					FATALDEFAULT;
			}
		} else {
			// Japan only
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->Pool  = 2;
					obj->Draw1 = -1;
					obj->Step  = 0;
					obj->Draw2.full = 0xffbd;
					obj->YPI += 0xb7;
					obj->LocalTimer = 0x80;
					obj->x002e = 0x40;
					//setaction_list(obj, data_21bea, 0); TODO
					break;
				case 2:
					if (g.mode2 == 0xc) {
						queuesound(SOUND_GAME_OVER);
						print_libtextgfx(0x15);
						FreeActor(obj);
					} else {
						if (obj->mode1 == 0) {
							++obj->Step;
							obj->Step &= 0x1f;
							obj->YPI -= 2;
							++obj->Draw2.full;
							if(--obj->x002e == 0) {
								obj->mode1 = 2;
							}
						}
						enqueue_and_layer(obj);
					}
					break; 
				case 4:
				case 6:
					FreeActor(obj);
					break;
				FATALDEFAULT;
			}
		}

	}

}


#pragma mark Act44
static void sub_2224c(Object *obj_a4, Object *obj_a2) {
	obj_a4->exists = TRUE;
	obj_a4->Sel    = 0x44;
	obj_a4->UserByte = 0;
	obj_a4->XPI = g.GPCollX;
	obj_a4->YPI = g.GPCollY;
	obj_a4->Flip = obj_a2->Direction ^ 1;
}
void sub_221ea(Object *obj_a2) {
	Object *obj;		// %a4
	if ((obj = AllocActor())) {
		obj->exists   = TRUE;
		obj->Sel      = 0x44;
		obj->UserByte = 1;
		obj->XPI      = obj_a2->XPI;
		obj->YPI      = obj_a2->YPI;
		obj->Flip	  = obj_a2->Direction ^ 1;
	}
	if ((obj = AllocActor())) {
		obj->exists   = TRUE;
		obj->Sel      = 0x44;
		obj->UserByte = 2;
		obj->XPI      = obj_a2->XPI;
		obj->YPI      = obj_a2->YPI;
		obj->Flip	  = obj_a2->Direction ^ 1;
	}
}
void sub_221bc(Object *obj_a2) {
	Object *obj;		// %a2
	if ((obj = AllocActor())) {
		sub_2224c(obj, obj_a2);
	}
	if ((obj = AllocActor())) {
		sub_2224c(obj, obj_a2);
	}
	if ((obj = AllocActor())) {
		sub_2224c(obj, obj_a2);
	}
	if ((obj = AllocActor())) {
		sub_2224c(obj, obj_a2);
	}
}
static void action_44(Object *obj) {
	UD44 *ud = (UD44 *)obj->UserData;
	const short data_222cc[8]={
		2, -1,
		3, -2,
		0, -3,
		1, -2,
	};
	const short data_222dc[8]={
		3, -2, 
		4, -1,
		0, -3,
		1,  2,
	};
	const short data_2234c[8]={
		0x0200, 0x0020, 0x0080, 0x0100, 0x0040, 0x0180, 0x0140, 0x0160,
	};
	const short data_2235c[8]={
		0xc8, 0x98, 0xb4, 0x92, 0xd4, 0x9d, 0xc3, 0xb0,
	};
	if (obj->UserByte) {
		//22392
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				ud->x0080       = 0;
				obj->LocalTimer = 20;
				obj->Pool       = 0;
				obj->VelX.full = (short []){0x300, 0x280}[obj->UserByte - 1];
				if (obj->Flip) {
					obj->VelX.full = -obj->VelX.full;
				}
				obj->VelY.full= (short []){0x600, 0x380}[obj->UserByte - 1];
				obj->AclX.full = 0;
				obj->AclY.full = 0x40;
//				setaction_list(obj, data_2246a, obj->UserByte + 1); TODO
				break;
			case 2:
				switch (obj->mode1) {
					case 0:
						if (obj->UserByte == 1) {
							if (--obj->LocalTimer == 0) {
								NEXT(obj->mode1);
								ud->x0080 = 0x19;
							}
						} else {
							if (obj->VelY.full < 0) {
								if (check_ground_collision(obj)) {
									NEXT(obj->mode1);
									ud->x0080 = 0x19;
									obj->VelY.full = 0x300;
								}
							}
						}
						CATrajectory(obj);
						break;
					case 2:
						if (--ud->x0080 < 0) {
							NEXT(obj->mode0);
							obj->mode1 = 0;
						}
						CATrajectory(obj);
					default:
						break;
				}
				if ((ud->x0080 & 1) == 0) {
					check_rect_queue_draw(obj);
				}
				break;
			case 4:
			case 6:
				FreeActor(obj);
				break;
			FATALDEFAULT;
		}
	} else {
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				ud->x0080 = 0;
				obj->Pool = 0;
				int d0 = RAND8WD;
				obj->XPI += data_222cc[d0];
				obj->YPI += data_222dc[d0];
				obj->SubSel = sf2rand() & 1;
				//setaction_list(obj, data_2246a, obj->SubSel); TODO
				break;
			case 2:
				switch (obj->mode1) {
					case 0:
						NEXT(obj->mode1);
						obj->VelX.full = data_2234c[RAND8WD];
						obj->VelY.full = data_2235c[RAND8WD];
						obj->AclX.full  = 0;
						if (obj->Flip == 0) {
							obj->VelY.full = -obj->VelY.full;
						}
						obj->AclY.full = 0x20;
						obj->LocalTimer = 10;
						break;
					case 2:
						if (--obj->LocalTimer == 0) {
							NEXT(obj->mode1);
							ud->x0080 = 0xf;
						}
						CATrajectory(obj);
						break;
					case 4:
						if (--ud->x0080 < 0) {
							NEXT(obj->mode0);
							obj->mode1 = 0;
						}
						CATrajectory(obj);
						break;
					FATALDEFAULT;
				}
				actiontick(obj);
				if ((ud->x0080 & 1) == 0) {
					check_rect_queue_draw(obj);
				}
				break;
			case 4:
			case 6:
				FreeActor(obj);
				break;
			FATALDEFAULT;
		}
	}

}


#pragma mark Act48 Speak You Win/Lose + Graphic

static void sub_22746(Object *obj) {
	UD48 *ud = (UD48 *)obj->UserData;

	switch (ud->x0080) {
		case 0:
			NEXT(ud->x0080);
			ud->x0081 = 10;
			queuesound(SOUND_YOU);
			break;
		case 2:
			if(--ud->x0081 == 0){
				NEXT(ud->x0080);
				queuesound(obj->SubSel == 9 ? SOUND_LOSE : SOUND_WIN);
			}
			break;
		case 4:
			/* does nothing */
			break;
		FATALDEFAULT;
	}
}

static void action_48(Object *obj) {		//226b6
	UD48 *ud = (UD48 *)obj->UserData;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			ud->x0080 = 0;    // word
			sub_22746(obj);
			obj->LocalTimer = 4;
			obj->Pool		= 0;
			obj->Scroll		= SCROLL_NONE;
			obj->XPI		= 0xc0;
			obj->YPI		= 0xa0;
			obj->Draw1		= 0;
			obj->Draw2.part.integer = obj->SubSel == 9 ? 0xf : 0xe;
			setaction_list(obj, actlist_2278c, obj->SubSel);
			break;
		case 2:
			sub_22746(obj);
			if (--obj->LocalTimer == 0) {		
				obj->LocalTimer = 4;
				obj->Draw1 ^= 1;
				obj->LocalTimer -= obj->Draw1;
				obj->LocalTimer -= obj->Draw1;
				obj->LocalTimer -= obj->Draw1;
			}
			if (g.KillAct48) {
				FreeActor(obj);
			} else {
				check_rect_queue_draw(obj);
			}
			break;
		FATALDEFAULT;
	}
}

