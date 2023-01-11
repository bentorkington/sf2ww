/* actions.c */

#include <stdio.h>


#include "sf2.h"


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

#include "redhammer.h"

extern Game g;
extern ScrollState gstate_Scroll1;
extern ScrollState gstate_Scroll2;
extern ScrollState gstate_Scroll3;
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

static void action_32(Object *obj);
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
static void action_40(Object *obj);
static void action_41(Object *obj);

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
				
                ACT117C(0x32, action_32)
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
                ACT117C(0x40, action_40)
                ACT117C(0x41, action_41)
				
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





static void action_0(Object *obj) {    /* c934 Guile stage actions */
    switch(obj->mode0) {
    case 0:
        NEXT(obj->mode0);
            RHSetScrollActionList(obj, RHCODE(0xc95c), obj->SubSel);
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
            RHSetActionList(obj, RHCODE(0xcb78), obj->SubSel);
			break;
		case 2:
			switch (obj->SubSel) {
				case 0:
					/* cbb6 */
					switch (obj->mode1) {
						case 0:
							if (g.FightOver) {
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
					if (obj->mode1==0) {
						NEXT(obj->mode1);
						obj->OnGround = TRUE;	/* u8 */
						obj->VelY.full = obj->YPI;
					}
					obj->YPI = obj->VelY.full;
					obj->YPI += (gstate_Scroll2.position.y.part.integer >> 1);
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

#pragma mark ACT03 Das Boat

static void action_3(Object *obj) {    /* d81e Das Boat */
    switch(obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			gstate_Scroll3.YOff = 0;	/* Scroll3 + 0x32 u16 */
            RHSetAction(obj, RHCODE(0xd878));
			break;
		case 2:
			if (0x74f6 & (1 << RAND16)) {
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
                RHSetScrollAction(obj, RHCODE(0xdc52));
			} else {
                RHSetScrollAction(obj, RHCODE(0xdc6e));
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
            RHSetScrollActionList(obj, RHCODE(0xdb1a), obj->SubSel);
			break;
		case 2:
			if (obj->mode1 == 0 && g.FightOver != 0) {
				NEXT(obj->mode1);
                RHSetScrollActionList(obj, RHCODE(0xdb4a), obj->SubSel);
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
            RHSetScrollActionList(obj, RHCODE(0xdb7e), obj->SubSel);
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
                    RHSetScrollActionList(obj, RHCODE(0xdbdc), obj->SubSel);
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
            RHSetScrollActionList(obj, RHCODE(0xf15e), obj->SubSel);
			break;
		case 2:
			if (obj->mode1 || g.FightOver == FALSE) {
				actiontickdraw(obj);
			} else {
				NEXT(obj->mode1);
                RHSetScrollActionList(obj, RHCODE(0xf190), obj->SubSel);
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
			for (i = 9; i >= 0; --i) {
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
            RHSetAction(obj, RHCODE(0xf708));
			// Tamper protection removed
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					// f620
					NEXT(obj->mode1);
					obj->LocalTimer = (char []){ 60,0x78,60,0x78,0x78,60,0xb4,0x78 }[RAND8];
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
                        RHSetAction(obj, RHCODE(0xf708));
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
					drawsimple_scroll2noattr_check(obj, RHOffsetLookup16(RHCODE(0xf86c), ud->h0080s), 21, 1);
				}
			} else {
				ud->h0080s = obj->AnimFlags & 0xff;
				ud->h0082c = TRUE;
			}
			enqueue_and_layer(obj);
            break;
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
                RHSetScrollActionList(obj, RHCODE(0xf976), 0);      // XXX step (%d0) is undef here!
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
                    RHSetActionList(obj, RHCODE(0xfb2e), obj->SubSel);
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
                        RHSetActionList(obj, RHCODE(0xfb32), obj->SubSel);
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
            RHSetScrollActionList(obj, RHCODE(0xfd64), obj->SubSel);
			break;
		case 2:
			if (obj->mode1 == 0 && g.FightOver) {
				NEXT(obj->mode1);
				if (obj->SubSel < 8) {
                    RHSetScrollActionList(obj, RHCODE(0xfd86), obj->SubSel);
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
            RHSetScrollActionList(obj, RHCODE(0x10492), obj->SubSel);
			break;
		case 2:
			if (obj->SubSel == 3) {
				// 104d0
				if (obj->mode1) {
					actiontickdraw(obj);
				} else {
					if (g.x8a66[obj->Step]) {
						NEXT(obj->mode1);
                        RHSetScrollActionList(obj, RHCODE(0x10628), obj->Step);
						// ?? any Step other than 0 is invalid access
					}
				}
			} else {
				if (obj->mode1 == 0 && g.FightOver) {
					NEXT(obj->mode1);
                    RHSetScrollActionList(obj, RHCODE(0x104c6), obj->Step);
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
            RHSetAction(obj, RHCODE(0x10822));
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
                        RHSetAction(obj, RHCODE(0x10852));
					}
					break;
				case 6:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
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
                        RHSetActionList(obj, RHCODE(0x10bc2), obj->Step);
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
                        RHSetAction(obj, RHCODE(0x10f90));
					} else {
                        RHSetAction(obj, RHCODE(0x10f5c));
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
                RHSetActionList(obj, RHCODE(0x111e6), 0);
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
            break;
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
					obj->XPI	= (SCREEN_WIDTH / 2);
					obj->YPI	= 152;
                    RHSetActionList(obj, RHCODE(0x11602), 1);
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
                    RHSetActionList(obj, RHCODE(0x11602), 0);
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
                                RHSetActionList(obj, RHCODE(0x11602), 1);
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
                    RHSetActionList(obj, RHCODE(0x11602), 0);
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
                                RHSetActionList(obj, RHCODE(0x11602), 1);
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
	COORDS_OFFSET(&cp, 0x50, 0);
	OBJ_CURSOR_BUMP(*gfx_p);
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, TILE_FRAMECORNER2, 0x1f | ATTR_NO_FLIP);
	COORDS_OFFSET(&cp, 0x50, 0);
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
    _draw_portrait_scr2(RHOffsetLookup16(RHCODE(0x15f8e), fighterid), flip, gfx_p);
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
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->UserData[0] = 0;
			obj->ZDepth = 0;
			obj->Scroll = SCROLL_2;
			obj->Pool = 0;
            RHSetActionList(obj, RHCODE(0x11aca), obj->SubSel);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
                    obj->VelX.full = RH2DWord(0x11a80, 4, 0, obj->SubSel);
					obj->VelY.full = RH2DWord(0x11a80, 4, 1, obj->SubSel);
					obj->XPI      += RH2DWord(0x11a80, 4, 2, obj->SubSel);
					obj->YPI      += RH2DWord(0x11a80, 4, 3, obj->SubSel);
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
            RHSetAction(obj, RHCODE(0x1237c));
		}
	} else {
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
                RHSetScrollAction(obj, RHCODE(0x126e8));
				break;
			case 2:
				actiontickdraw(obj);
				break;
			default:
				break;
		}
	}

}

#pragma mark Act15 Chun Li stage anims
static void action_15(Object *obj) {
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = (u8 []){0,0,0,0,0,0,0,2}[obj->SubSel];
            RHSetScrollActionList(obj, RHCODE(0x11bc0), obj->SubSel);
			break;
		case 2:
			if (obj->SubSel >= 4 || obj->mode1 || g.FightOver == FALSE) {
				actiontickdraw(obj);
			} else {
				NEXT(obj->mode1);
                RHSetScrollActionList(obj, RHCODE(0x11c00), obj->SubSel);
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
		if (gstate_Scroll1.position.x.part.integer < 448) {
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		if (gstate_Scroll1.position.x.part.integer < 416) {
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
					for (int i = 14; i >= 0; --i) {
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
                        drawsimple_scroll1attr(obj, RHCODE(0x1300c), 0x11, 8);
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
            RHSetActionList(obj, RHCODE(0x1325e), obj->SubSel);
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

#pragma mark Car Collision
#pragma mark Act1A

static Object *sub_13bf8(void)
{
    Object *obj = AllocActor();
    if (obj) {
        obj->exists = TRUE;
        obj->Sel = 0x1A;
        obj->XPI = g.GPCollX;
        obj->YPI = g.GPCollY;
    }
    return obj;
}

static void sub_13a0e(Object_G2 *obj_a2, int count_d4, int d2, int d5, int d3)
{
    UDcar *car = (UDcar *)&obj_a2->UserByte;
    while (count_d4 >= 0) {
        Object *glass = sub_13bf8();
        if (glass) {
            glass->UserData[0] = d2;
            glass->UserData[1] = d5;
            glass->UserData[2] = d3;
            if (car->h00a0w) {
                glass->XPI = car->h00a0w;
                glass->YPI = car->h00a2w;
            }
        }
        --count_d4;
    }
}

static void action_139ea(Object_G2 *obj_a2)
{
    UDcar *car = (UDcar *)&obj_a2->UserByte;
    car->h00a0w = 0;
    sub_13a0e(obj_a2, 3, 0, 0, 0);
}

static void action_139fc(Object_G2 *obj_a2)
{
    UDcar *car = (UDcar *)&obj_a2->UserByte;
    car->h00a0w = 0;
    sub_13a0e(obj_a2, 3, 1, 0, 0);
}

void action_132fe(Object_G2 *obj_a2, int arg_d6)
{
    UDcar *car = (UDcar *)&obj_a2->UserByte;

    if (car->h0093c <= 0xf && car->h0093c > 0) {
        switch (car->h0093c - 1) {
            case 0:
            case 1:
            case 2:         // sf2ua:0x13336
                if (arg_d6) {
                    action_139ea(obj_a2);
                }
                else {
                    if (g.GPHitBoxHit) {
                        car->h00a0w = 0xe2;
                    }
                    else {
                        car->h00a0w = 0x8b;
                    }
                    car->h00a2w = 0x74;
                    sub_13a0e(obj_a2, 10, -1, 0, 1);
                }
                break;
            case 4:         // sf2ua:0x13368
                if (!arg_d6) {
                    car->h00a0w = 0x9b;
                    car->h00a2w = 0x71;
                    sub_13a0e(obj_a2, 12, -1, 2, 2);
                    
                    car->h00a0w = 0xb6;
                    car->h00a2w = 0x66;
                    sub_13a0e(obj_a2, 12, -1, 1, 1);
                    
                    car->h00a0w = 0xe3;
                    car->h00a2w = 0x58;
                    sub_13a0e(obj_a2, 10, 1, 2, 2);
                    
                    Object *obj;
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 0;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 8;
                        obj->Step   = 0;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 8;
                        obj->Step   = 1;
                    }
                }
                break;
            case 5:         // sf2ua:0x13416
                if (!arg_d6) {
                    car->h00a0w = 0xa8;
                    car->h00a2w = 0x6b;
                    sub_13a0e(obj_a2, 7, 0, 0, 1);
                }
                break;
            case 6:         // sf2ua:0x13436
                if (!arg_d6) {
                    car->h00a0w = 0x65;
                    car->h00a2w = 0x54;
                    sub_13a0e(obj_a2, 5, 1, 2, 2);
                    
                    car->h00a0w = 0x79;
                    car->h00a2w = 0x6a;
                    sub_13a0e(obj_a2, 3, 0, 2, 2);
                    
                    car->h00a0w = 0x95;
                    car->h00a2w = 0x6d;
                    sub_13a0e(obj_a2, 4, -1, 1, 1);
                    
                    car->h00a0w = 0xac;
                    car->h00a2w = 0x69;
                    sub_13a0e(obj_a2, 4, -1, 1, 1);
                }
                break;
            case 7:         // sf2ua:134a4
                if (arg_d6) {
                    action_139fc(obj_a2);
                }
                else {
                    car->h00a0w = 0xed;
                    car->h00a2w = 0x64;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xbd;
                    car->h00a2w = 0x60;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                    
                    car->h00a0w = 0xd9;
                    car->h00a2w = 0x55;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                }
                break;
            case 8:         // 134f8
                if (arg_d6) {
                    action_139fc(obj_a2);
                }
                else {
                    car->h00a0w = 0x101;
                    car->h00a2w = 0x5e;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0x93;
                    car->h00a2w = 0x67;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xb8;
                    car->h00a2w = 0x5a;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                }
                break;
            case 9:         // 1354c
                if (arg_d6) {
                    action_139fc(obj_a2);
                }
                else {
                    car->h00a0w = 0xa2;
                    car->h00a2w = 0x62;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xc0;
                    car->h00a2w = 0x60;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                    
                    car->h00a0w = 0xf9;
                    car->h00a2w = 0x55;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                }
                break;
            case 10:           // 135a0
                if (arg_d6) {
                    action_139fc(obj_a2);
                }
                else {
                    car->h00a0w = 0x94;
                    car->h00a2w = 0x61;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xb2;
                    car->h00a2w = 0x5d;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xd5;
                    car->h00a2w = 0x5d;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);

                    car->h00a0w = 0x101;
                    car->h00a2w = 0x5d;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                    
                    Object *obj;
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 1;
                        obj->Step   = obj_a2->Direction;
                    }
                }
                break;
            case 11:        //13630
                if (arg_d6) {
                    action_139fc(obj_a2);
                }
                else {
                    car->h00a0w = 0xcb;
                    car->h00a2w = 0x50;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xaf;
                    car->h00a2w = 0x64;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xa1;
                    car->h00a2w = 0x59;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                    
                    car->h00a0w = 0x101;
                    car->h00a2w = 0x5d;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                    
                    Object *obj;
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 3;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 2;
                        obj->Step   = 0;
                        obj->Flip   = obj->Step;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 2;
                        obj->Step   = 1;
                        obj->Flip   = obj->Step;
                    }
                }
                break;
            case 12:        // 13706
                if (arg_d6) {
                    action_139fc(obj_a2);
                }
                else {
                    car->h00a0w = 0xcb;
                    car->h00a2w = 0x50;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xb2;
                    car->h00a2w = 0x5d;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xd5;
                    car->h00a2w = 0x5d;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                    
                    car->h00a0w = 0x101;
                    car->h00a2w = 0x5d;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                }
                break;
            case 13:        // 13774
                if (arg_d6) {
                    action_139fc(obj_a2);
                }
                else {
                    car->h00a0w = 0x80;
                    car->h00a2w = 0x57;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xa1;
                    car->h00a2w = 0x47;
                    sub_13a0e(obj_a2, 5, 1, 1, 1);
                    
                    car->h00a0w = 0xda;
                    car->h00a2w = 0x3d;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                    
                    car->h00a0w = 0x101;
                    car->h00a2w = 0x4a;
                    sub_13a0e(obj_a2, 4, 1, 1, 1);
                    
                    Object *obj;
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 4;
                        obj->Step   = obj_a2->Direction ^ 1;
                    }
                }
                break;
            case 14:        // 1380a
                if (arg_d6) {
                    action_139fc(obj_a2);
                }
                else {
                    Object *obj;
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 7;
                        obj->Step   = 0;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 7;
                        obj->Step   = 1;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 6;
                        obj->Step   = 0;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 6;
                        obj->Step   = 1;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 6;
                        obj->Step   = 2;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 5;
                        obj->Step   = 0;
                        obj->LocalTimer = 3;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 5;
                        obj->Step   = 1;
                        obj->LocalTimer = 3;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 5;
                        obj->Step   = 2;
                        obj->LocalTimer = 3;
                    }
                    if ((obj = AllocActor())) {
                        obj->exists = TRUE;
                        obj->Sel    = 0x41;
                        obj->SubSel = 5;
                        obj->Step   = 2;
                        obj->LocalTimer = 3;
                    }
                    
                    car->h00a0w = 0x67;
                    car->h00a2w = 0x56;
                    sub_13a0e(obj_a2, 5, 1, 2, 2);
                    
                    car->h00a0w = 0x9a;
                    car->h00a2w = 0x6b;
                    sub_13a0e(obj_a2, 2, 0, 0, 1);
                    
                    car->h00a0w = 0x86;
                    car->h00a2w = 0x51;
                    sub_13a0e(obj_a2, 5, 1, 2, 2);
                    
                    car->h00a0w = 0xaf;
                    car->h00a2w = 0x58;
                    sub_13a0e(obj_a2, 5, 1, 2, 2);
                    
                    car->h00a0w = 0xdb;
                    car->h00a2w = 0x58;
                    sub_13a0e(obj_a2, 3, 1, 1, 2);
                    
                    car->h00a0w = 0x103;
                    car->h00a2w = 0x63;
                    sub_13a0e(obj_a2, 2, 1, 2, 2);
                }
                break;
            FATALDEFAULT;
        }
    }
}


static void sub_13c1a(Object *obj) {
	if (obj->UserData[0] & 0x80) {
		if (0xaaaa & (1 << (RAND16))) {
            RHSetActionList(obj, RHCODE(0x13c5a), obj->UserData[1]);
		} else {
            RHSetActionList(obj, RHCODE(0x13c54), obj->UserData[1]);
		}
	} else {
		if (obj->UserData[0]) {
            RHSetActionList(obj, RHCODE(0x13c5a), obj->UserData[1]);
		} else {
            RHSetActionList(obj, RHCODE(0x13c54), obj->UserData[1]);
		}
		
	}
}

static void action_1a(Object *obj) {		// 13a3a
	int temp;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
            obj->LocalTimer = RHByteOffset(0x13ac6, sf2rand() & 0x3e);
			obj->SubTimer   = RHByteOffset(0x13ac6, (sf2rand() & 0x3e) + 1);
            obj-> XPI      += RHWordOffset(0x13ae6, RAND16WD);
			obj-> YPI      += RHWordOffset(0x13ae6, RAND16WD);
			
			temp = RAND8;
            obj->VelX.full = RH3DWord(0x13b06, 8, 4, obj->UserData[2], temp, 0);
			obj->AclX.full = RH3DWord(0x13b06, 8, 4, obj->UserData[2], temp, 1);
			obj->VelY.full = RH3DWord(0x13b06, 8, 4, obj->UserData[2], temp, 2);
			obj->AclY.full = RH3DWord(0x13b06, 8, 4, obj->UserData[2], temp, 3);
			
			sub_13c1a(obj);
			break;
		case 2:
			CATrajectory(obj);
			if (--obj->LocalTimer == 0) {
				NEXT(obj->mode0);
			}
			actiontick(obj);
			if (obj->SubTimer) {
				--obj->SubTimer;
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
			obj->LocalTimer = 127;
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
            ply = obj->Owner;
			if (ply->SubSel & (1 << g.ContinueBits)) {
				if (obj->LocalTimer != ply->ContinueSecs) {
					obj->LocalTimer = ply->ContinueSecs;
					queuesound(sounds[obj->LocalTimer]);
                    RHSetActionList(obj, RHCODE(0x13fba), obj->LocalTimer);
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

#pragma mark Act1C wooden crate fragments
static void action_1c(Object *obj) {
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
            RHSetActionList(obj, RHCODE(0x158fe), obj->SubSel);
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
                        obj->VelY.full = RHSwapWord(*RHCODE16_ARRAY(0x1585a, 1, RAND8WD));
                        obj->VelX.full = RHSwapWord(*RHCODE16_ARRAY(0x1586a, 1, RAND8WD));
                        obj->XPI += RHSwapWord(*RHCODE16_ARRAY(0x1587a, 1, RAND8WD));
                        obj->YPI += RHSwapWord(*RHCODE16_ARRAY(0x1588a, 1, RAND8WD));
					}
                    obj->UserData[2] = RHSwapWord(*RHCODE16_ARRAY(0x15891, 1, RAND8WD)) + 0x28;
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
                        obj->VelY.full = RHSwapWord(*RHCODE16_ARRAY(0x158ea, 1, RAND8WD));
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
            break;
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
    FBSetPalette(17 + g.BattleLoser, RHCODE16_ARRAY(0x160ae, 16, g.BattleLoser));
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
	Player *ply = obj->SubSel ? PLAYER2 : PLAYER1;
	switch (obj->mode0) {
		case 0:
			if (ply->Human) {
				NEXT(obj->mode0);
                RHSetActionList(obj, RHCODE(0x1a10c), obj->SubSel);
			}
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					actiontick(obj);
					if (ply->SelectComplete || ply->SelectDelayTmr) {
						NEXT(obj->mode1);
                        RHSetActionList(obj, RHCODE(0x1a10c), obj->SubSel);
					}
					break;
				case 2:
					/* nothing */
					break;
				FATALDEFAULT;
			}
            obj->XPI = RH2DWord(0x18eb8, 2, ply->FighterID, 0);
            obj->YPI = RH2DWord(0x18eb8, 2, ply->FighterID, 1);
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
                RHSetActionList(obj, RHCODE(0x1dec6), obj->SubSel);
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
            RHSetAction(obj, RHCODE(0x117aa));      /* Flashing KO */
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
                        RHSetAction(obj, RHCODE(0x117aa));
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
	Object *obj;
	if ((obj=AllocActor())) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_PREROUNDANIM;
	}
	if ((g.CurrentStage == STAGE_THAILAND_BISON) && (g.RoundCnt != 0)) {
        FBSetPalette(11, RHCODE16(0x1abf6));
	}
}


static void sub_1acaa(Object *obj) {
	if (g.OnBonusStage) {
		switch (obj->mode1) {
			case 0:
				NEXT(obj->mode1);
				obj->Draw2.full	= 0x40;
				obj->LocalTimer = 120;
                RHSetAction(obj, RHCODE(0x1af46));      /* BONUS STAGE */
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
                RHSetAction(obj, RHCODE(0x1af7e));      /* START */
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
				NEXT(obj->mode1);
				obj->Draw2.full = 0xfff0;
                RHSetActionList(obj, RHCODE(0x1ae1a), g.RoundCnt);
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
				g.PreFightWait = FALSE;		// Queue the Bison Cape remove anim
                RHSetAction(obj, RHCODE(0x1af62));          /* FIGHT! */
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
			obj->Step	= 0;
			if (g.OnBonusStage) {
				return;
			}
			showtextbank1(0x14);	/* BATTLE */
			
            int d0 = g.VictoryCnt;
            if (d0 > 0x63) {
                d0 = 0x62;
            }
            bin2bcd(d0);
            u8 printZeroes = 0;
            u16 *gfx_p = CPS_OBJ(0x910230);
            printbytehex(gfx_p, 0xd8, 0x78, g.bin2bcd_result, &printZeroes, 0xd);

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

static void _draw_portrait_beaten(u16 **scr_p, char d2, char d3) {		//15cf2
   	/* anti-copying wierdness, fuck it */
	d3 *= 10;
	d3 += ((sf2rand() & 0xe) + (g.libsplatter & 6)) % 10;
	
    _draw_portrait_scr2(RHOffsetLookup16(RHCODE(0x15d36), d3/2), d2, scr_p);
}



void action_1286e(Object *obj, short d0) {	
	g.x8a62[d0]=60;
	add_bcd_8(1, &g.x8a60[d0]);
}

static void _draw_frame_corners(u16 **gfx_p, u32 cp ){	/* 15df6 */
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, TILE_FRAMECORNER, 0x1f | ATTR_X_FLIP);
	COORDS_OFFSET(&cp, 112, 0);
	OBJECT_DRAW(*gfx_p, CP_X, CP_Y, TILE_FRAMECORNER, 0x1f | ATTR_NO_FLIP);
}

static void _draw_portrait_scr2(const SimpleImage *a1, short flip, u16 **scr_p) {			// 1601c
	short width, height;
	const u16 *img = (const u16 *)a1;
	width  = RHSwapWord(*img++);
	height = RHSwapWord(*img++);
	img++;
	sub_4386(flip, width, height, img, scr_p);	/*backside of drawsimple_scroll2attr_nocheck */
}

static void _draw_portrait_prefight(u16 **scr_p, short flip, short fid_d3) {		// 15c02
    _draw_portrait_scr2(RHOffsetLookup16(RHCODE(0x15c16), fid_d3), flip, scr_p);
}
static void sub_15ff0(u16 **scr_p, short flip, short fid_d3) {
    _draw_portrait_scr2(RHOffsetLookup16(RHCODE(0x16004), fid_d3), flip, scr_p);
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
            RHSetActionList(obj, RHCODE(0x1d4f0), 0);
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
                        RHSetActionList(obj, RHCODE(0x1d4f0), 2);
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
                        RHSetActionList(obj, RHCODE(0x1d4f0), 3);
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
            RHSetActionList(obj, RHCODE(0x1d4f0), 1);   // black guy
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
                        RHSetActionList(obj, RHCODE(0x1d4f0), 4);   //black guy reeling backwards
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
            RHSetActionList(obj, RHCODE(0x1d4f0), 5);   // punched head
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
            RHSetActionList(obj, RHCODE(0x1d4f0), 6);   // bang
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
            RHSetActionList(obj, RHCODE(0x1d4f0), 7);   // Teeth
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
			obj->XPI += gstate_Scroll3.position.x.part.integer;
			obj->YPI += gstate_Scroll3.position.y.part.integer;
			obj->Pool = 6;
            RHSetAction(obj, RHCODE(0x1da8c));
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

#pragma mark Act32 car shake

static void action_1f406(Object *obj)
{
    struct UserData_Act32 *ud = (struct UserData_Act32 *)obj->UserData;

    g.CarOffX += RHSwapWord(ud->x0080[0]);
    g.CarOffY += RHSwapWord(ud->x0080[1]);
    if (RHSwapWord(ud->x0080[2] < 0)) {
        obj->mode0 = 4;
    }
    else {
        obj->LocalTimer = RHSwapWord(ud->x0080[2]);
        ud->x0080 += 3;
    }
}

static void action_32(Object *obj)       // 1f3c6
{
    struct UserData_Act32 *ud = (struct UserData_Act32 *)obj->UserData;
    
    switch (obj->mode0) {
        case 0:
            NEXT(obj->mode0);
            if (g.x8abe) {
                FreeActor(obj);
            }
            else {
                ud->x0080 = RHOffsetLookup16(RHCODE(0x1f432), obj->SubSel);
                action_1f406(obj);
            }
            break;
        case 2:
            if (--obj->LocalTimer == 0) {
                action_1f406(obj);
            }
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
            RHSetActionList(obj, RHCODE(0x1f502), obj->SubSel);
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
                RHSetActionList(obj, RHCODE(0x1fb52), d0);      // water splashes
				check_rect_queue_draw(obj);
			} else {
                RHSetActionList(obj, RHCODE(0x1fb46), d0);      // dust clouds
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
	gstate_Scroll1.position.y.part.integer = ud->Save_Scroll1Y;
	gstate_Scroll2.position.y.part.integer = ud->Save_Scroll2Y;
	gstate_Scroll3.position.y.part.integer = ud->Save_Scroll3Y;
}
static void sub_1fe4e(Object *obj, short adjust) {	/* per stage wobbles */
	UD36 *ud = (UD36 *)obj->UserData;

	switch (g.CurrentStage) {
		case STAGE_JAPAN_RYU:			// 1fea4
			if (gstate_Scroll1.mode1) {
				ud->Save_Scroll1Y = 0x100;
			}
			gstate_Scroll2.position.y.part.integer += adjust;
			gstate_Scroll3.position.y.part.integer += adjust;
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
			gstate_Scroll1.position.y.part.integer += adjust;
			gstate_Scroll2.position.y.part.integer += adjust;
			gstate_Scroll3.position.y.part.integer += adjust;
			break;
		case STAGE_BRAZIL_BLANKA:
		case STAGE_USA_BALROG:
		case STAGE_BONUS_BARRELS:
		case STAGE_BONUS_DRUMS:
			gstate_Scroll2.position.y.part.integer += adjust;
			gstate_Scroll3.position.y.part.integer += adjust;
			break;
		case STAGE_USSR_ZANGIEF:
			gstate_Scroll1.position.y.part.integer += adjust;
			gstate_Scroll2.position.y.part.integer += adjust;
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
			obj->SubTimer = 7;
			ud->Save_Scroll1Y = gstate_Scroll1.position.y.part.integer;
			ud->Save_Scroll2Y = gstate_Scroll2.position.y.part.integer;
			ud->Save_Scroll3Y = gstate_Scroll3.position.y.part.integer;
			break;
		case 2:
			if (--obj->SubTimer < 0) {
				if(--obj->LocalTimer == 0) {
					NEXT(obj->mode0);
					sub_1fe36(obj);
					return;
				} else {
					obj->SubTimer = 7;
				}
			}
			if (obj->SubTimer & 1) {
                g.ScreenWobbleMagnitude = 3;
                ud->Save_Scroll1Y = gstate_Scroll1.position.y.part.integer;
                ud->Save_Scroll2Y = gstate_Scroll2.position.y.part.integer;
                ud->Save_Scroll3Y = gstate_Scroll3.position.y.part.integer;
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
            RHSetActionList(obj, RHCODE(0x1fcfa), 0);
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
		
	switch (obj->mode0) {
		case 0:
			if (g.Version != VERSION_JAP) {
                obj->XPI += RHByteOffset(0x1ff38, (ply->FighterID * 2) + ply->Side);
			} else {
                obj->XPI += RHByteOffset(0x1ff20, (ply->FighterID * 2) + ply->Side);
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

#define ACT3B_TIME_BONUS_BUSY  1
#define ACT3B_VITAL_BONUS_BUSY 2

/*!
 @abstract draw BCD encoded score to tile RAM
 @param bid-endian BCD encoded score (%d0)
 @param pointer to tile RAM (%a1)
 @discussion sf2ua:0x205f6
 */
static void _act3b_print_counter(u32 d0, short d2, u16 *a1) {
	d2 -= 3;
	while (d2 >= 0) {
        --a1;
		*a1 = (d0 & 0xf) + SF2_TILE_LARGE_HEX;
		d2--;
		d0 >>= 4;
	}
}
static void _init_counter_image(Object *obj) {		//20610
	const u16 *source;
	u16 *dest;
	int i;

	/* all same anyway  sf2ua: 20640  original ROM uses three the same   */
	const static u16 blank_counter_image[] = {0x1, 0xd, 0x2d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, SF2_TILE_LARGE_HEX_ZERO};
    
	switch (obj->SubSel) {
		case 0:
			source = blank_counter_image; dest = g.TimeBonusSprite; break;
		case 2:
			source = blank_counter_image; dest = g.VitalBonusSprite; break;
		case 4: 
			source = blank_counter_image; dest = g.TotalBonusSprite; break;
		FATALDEFAULT;
	}
	for (i=0; i<10; i++) {
		dest[i] = source[i];
	}
}
/*!
sfu2a: 205d8 add points to total bonus
 */
static void _add_to_total_bonus(Object *obj) {
	UD3B *ud = (UD3B *)&obj->UserData;
	add_bcd_32_16(ud->x0082, &g.TotalBonusCount);
    start_effect(0x2002, g.RoundWinnerSide);        // 100 points
}

static void action_3b(Object *obj) {	//203ba
	UD3B *ud = (UD3B *)&obj->UserData;
	
	Player *ply;
	short d2;
	int d0;
	
	//coordinates of score counters sf2ua: 20412
	static const short bonus_coords[][2] = {
		{216, 176},
		{216, 160},
		{216, 136}, 
	};
	
	/* perfect energy score per player sf2ua 204d8 */
	static const short perfect_bonus_per_fighter[12] = {
		0x300, 0x300, 0x300, 0x300,
		0x300, 0x300, 0x300, 0x300, 
		0x800, 0x500, 0x500, 0x500,		// Bison, three other bosses
	};
	
	static const Action action_time_bonus  =      // sf2ua: 2067c
        {8, 0, 0, (Image *)&g.TimeBonusSprite, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static const Action action_vital_bonus =      // sf2ua: 20694
        {8, 0, 0, (Image *)&g.VitalBonusSprite, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static const Action action_total_bonus =      // sf2ua: 206ac
        {8, 0, 0, (Image *)&g.TotalBonusSprite, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	switch (obj->mode0) {
		case 0:							// init the animation
			NEXT(obj->mode0);
			obj->Scroll    = SCROLL_NONE;
			obj->Pool      = 2;
			ud->x0080      = 0;
			
			_init_counter_image(obj);
			
			obj->XPI = bonus_coords[obj->SubSel/2][0];
			obj->YPI = bonus_coords[obj->SubSel/2][1];
			switch (obj->SubSel) {
				case 0:												// TIME
					ud->x0080 = (g.TimeRemainBCD << 8);
					if (g.TimeRemainBCD != 0) {
						d2 = (g.TimeRemainBCD & 0xf0) ? 4 : 3;		// number of tiles
						g.TimeBonusSprite[0] = d2;
						g.TimeBonusSprite[8] = SF2_TILE_LARGE_HEX_ZERO;
						_act3b_print_counter(g.TimeRemainBCD, d2, &g.TimeBonusSprite[8]);
					}
					g.ScoreCountFlags |= ACT3B_TIME_BONUS_BUSY;
                    RHSetAction(obj, RHCODE(0x2067c));
					break;
				case 2:												// VITAL
					ply = g.RoundWinnerSide ? PLAYER2 : PLAYER1;
					if (ply->Energy) {
						if (ply->Energy < 0x90) {
							// 20494
							bin2bcd(ply->Energy);
							d0 = g.bin2bcd_result;
						} else {
							queuesound(SOUND_PERFECT);
							d0 = perfect_bonus_per_fighter[ply->Opponent->FighterID];
						}
						ud->x0080 = d0;
						if (d0 < 0x10) {
							d2 = 3;
						} else if (d0 < 0x100) {
							d2 = 4;
						} else {
							d2 = 5;
						}
						g.VitalBonusSprite[0] = d2;
						g.VitalBonusSprite[8] = SF2_TILE_LARGE_HEX_ZERO;
						_act3b_print_counter(d0, d2, &g.VitalBonusSprite[8]);
					}
					g.ScoreCountFlags |= ACT3B_VITAL_BONUS_BUSY;
                    RHSetAction(obj, RHCODE(0x20694));
					break;
				case 4:											// TOTAL
					ud->x0080 = g.TotalBonusCount;
                    RHSetAction(obj, RHCODE(0x206ac));
					break;
				FATALDEFAULT;
			}
			break;
		case 2:							// 204fc animate and decrement score
			switch (obj->SubSel) {
				case 0:
					if (g.CanSpeedUpScoreCount) {
						if (ud->x0080 & 0xff00) {
							sub_bcd_32_8shift(1, &ud->x0080);
							ud->x0082 = 0x0001;
							_add_to_total_bonus(obj);
						} else {
							g.ScoreCountFlags &= ~ACT3B_TIME_BONUS_BUSY;
						}
					}
					break;
				case 2:
					if (g.CanSpeedUpScoreCount) {
						if (ud->x0080) {
                            obj->SubTimer = 1;      // hacky, used for BCD operand
                                                    // in CPS. side effects?
							sub_bcd_32_16(1, &ud->x0080);	// XXX
                            ud->x0082 = 0x0001;
							_add_to_total_bonus(obj);
						} else {
							g.ScoreCountFlags &= ~ACT3B_VITAL_BONUS_BUSY;
						}						
					}
					break;
				case 4:
					g.CanSpeedUpScoreCount = TRUE;
					if (g.TotalBonusCount) {
						if (g.TotalBonusCount < 0x10) {
							d2 = 3;
						} else if (g.TotalBonusCount < 0x100) {
							d2 = 4;
						} else {
							d2 = 5;
						}
                        g.TotalBonusSprite[0] = d2;
                        g.TotalBonusSprite[8] = SF2_TILE_LARGE_HEX_ZERO;

						_act3b_print_counter(g.TotalBonusCount, d2, &g.TotalBonusSprite[8]);
						if ((g.libsplatter & 3) == 0 && g.TotalBonusCount != ud->x0080) {
							queuesound(SOUND_SCORE_DING);
						}
					}
                    RHSetAction(obj, RHCODE(0x206ac));
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
#pragma mark Act40 20de8
static void sub_20f10(Object *obj, Player *ply) {
    sub_bcd_32(0x400, &ply->x015c);
    if (ply->x015c > 0) {
        start_effect(0x2002, ply->Side);
    } else {
        obj->UserData[ply->Side] = 1;
        return;
    }
    sub_bcd_32(0x400, &ply->x015c);
    if (ply->x015c > 0) {
        start_effect(0x2002, ply->Side);
    } else {
        obj->UserData[ply->Side] = 1;
        return;
    }
}
static void action_40(Object *obj)      // 20de8
{
    switch (obj->SubSel) {
        case 0:
            switch (obj->mode0) {
                case 0:
                    NEXT(obj->mode0);
                    obj->LocalTimer = 10;
                    if (g.CurrentStage == STAGE_BONUS_BARRELS) {
                        g.Pause_9e1 = 1;
                        NEXT(obj->mode0);
                    }
                    break;
                case 2:
                    if (--obj->LocalTimer == 0) {
                        obj->LocalTimer = 10;
                        if (g.TimeRemainBCD == 0) {
                            NEXT(obj->mode0);
                            g.Pause_9e1 = 1;
                        } else {
                            // Award 2000 points to each player for each second remaining
                            
                            sub_bcd_8(1, &g.TimeRemainBCD);
                            sub_529c();                 // update time remain display
                            if (g.Player1.exists) {
                                LBAddPoints(8, 0);
                            }
                            if (g.Player2.exists) {
                                LBAddPoints(8, 1);
                            }
                            queuesound(SOUND_SCORE_DING);
                        }
                    }
                    break;
                case 4:
                case 6:
                    FreeActor(obj);
                    break;
                FATALDEFAULT
            }
            break;
        case 1:
            switch (obj->mode0) {
                case 0:
                    NEXT(obj->mode0);
                    obj->UserData[0] = g.Player1.exists ^ 1;
                    obj->UserData[1] = g.Player2.exists ^ 1;
                    g.Player1.x015c = g.Player1.BonusScore;
                    g.Player2.x015c = g.Player2.BonusScore;
                    break;
                case 2:
                    if (!obj->UserData[0] && g.Player1.exists) {
                        sub_20f10(obj, &g.Player1);
                    }
                    if (!obj->UserData[1] && g.Player2.exists) {
                        sub_20f10(obj, &g.Player2);
                    }
                    if ((g.libsplatter & 3) == 0) {
                        queuesound(SOUND_SCORE_DING);
                    }
                    if (obj->UserData[0] & obj->UserData[1]) {
                        NEXT(obj->mode0);
                        g.Pause_9e1 = 1;
                    }
                    break;
                case 4:
                case 6:
                    FreeActor(obj);
                    break;
                FATALDEFAULT
            }
            break;

        FATALDEFAULT
    }
    
}

#pragma mark Act41
static void action_41(Object *obj)
{
    switch (obj->SubSel) {
        case 0:
        case 3:
        case 4:         // 2109e
            switch (obj->mode0) {
                case 0:
                    NEXT(obj->mode0);
                    obj->Pool = 0;
                    obj->UserData[0] = 0;
                    int temp = obj->SubSel;
                    if (temp) {
                        temp -= 2;
                    }
                    obj->XPI       = RH2DWord(0x21102, 3, 0, temp);
                    obj->YPI       = RH2DWord(0x21102, 3, 1, temp);
                    obj->VelX.full = RH2DWord(0x21102, 3, 2, temp);
                    obj->VelY.full = RH2DWord(0x21102, 3, 3, temp);
                    obj->AclX.full = 0;
                    obj->AclY.full = 0x40;
                    
                    obj->LocalTimer = 0x14;
                    RHSetActionList(obj, RHCODE(0x2155c), obj->SubSel);
                    break;
                case 2:
                    switch (obj->mode1) {
                        case 0:
                            if (--obj->LocalTimer == 0) {
                                NEXT(obj->mode1);
                                obj->UserData[0] = 0x14;
                            }
                            break;
                        case 2:
                            if (--obj->UserData[0] == 0) {
                                NEXT(obj->mode0);
                                obj->mode1 = 0;
                            }
                        FATALDEFAULT
                    }
                    CATrajectory(obj);
                    if ((obj->UserData[0] & 1) == 0) {
                        check_rect_queue_draw(obj);
                    }
                    break;
                case 4:
                case 6:
                    FreeActor(obj);
                    break;
                FATALDEFAULT
            }
            break;
        case 1:
            // 2115e
            switch (obj->mode0) {
                case 0:
                    NEXT(obj->mode0);
                    obj->Pool = 0;
                    obj->UserData[0] = 0;
                    obj->XPI       = 0x00cf;
                    obj->YPI       = 0x005a;
                    obj->VelY.full = 0x0600;
                    if (obj->Step)
                        obj->VelX.full = -0x300;
                    else
                        obj->VelX.full =  0x300;

                    obj->AclX.full = 0;
                    obj->AclY.full = 0x40;
                    
                    obj->LocalTimer = 0x14;
                    RHSetActionList(obj, RHCODE(0x2155c), obj->SubSel);
                    break;
                case 2:
                    switch (obj->mode1) {
                        case 0:
                            if (--obj->LocalTimer == 0) {
                                NEXT(obj->mode1);
                                obj->UserData[0] = 0x14;
                            }
                            break;
                        case 2:
                            if (--obj->UserData[0] == 0) {
                                NEXT(obj->mode0);
                                obj->mode1 = 0;
                            }
                        FATALDEFAULT
                    }
                    CATrajectory(obj);
                    if ((obj->UserData[0] & 1) == 0) {
                        check_rect_queue_draw(obj);
                    }
                    break;
                case 4:
                case 6:
                    FreeActor(obj);
                FATALDEFAULT
            }
            break;
        case 2:
        case 8:
            // 21202
            switch (obj->mode0) {
                case 0:
                    NEXT(obj->mode0);
                    obj->Pool = 0;
                    obj->UserData[0] = 0;
                    if (obj->SubSel == 2) {
                        obj->XPI       = RH2DWord(0x21298, 2, 0, obj->Step);
                        obj->YPI       = RH2DWord(0x21298, 2, 1, obj->Step);
                        obj->VelX.full = RH2DWord(0x21298, 2, 2, obj->Step);
                        obj->VelY.full = RH2DWord(0x21298, 2, 3, obj->Step);
                    }
                    else {
                        obj->XPI       = RH2DWord(0x212a8, 2, 0, obj->Step);
                        obj->YPI       = RH2DWord(0x212a8, 2, 1, obj->Step);
                        obj->VelX.full = RH2DWord(0x212a8, 2, 2, obj->Step);
                        obj->VelY.full = RH2DWord(0x212a8, 2, 3, obj->Step);
                    }
                    obj->AclX.full = 0;
                    obj->AclY.full = 0x40;
                    
                    obj->LocalTimer = 0x14;
                    
                    if (obj->SubSel == 2) {
                        RHSetActionList(obj, RHCODE(0x2155c), 2);
                    } else {
                        if (obj->Step) {
                            RHSetActionList(obj, RHCODE(0x2155c), 9);
                        } else {
                            RHSetActionList(obj, RHCODE(0x2155c), 8);
                        }
                    }
                    break;
                case 2:
                    switch (obj->mode1) {
                        case 0:
                            if (--obj->LocalTimer == 0) {
                                NEXT(obj->mode1);
                                obj->UserData[0] = 0x14;
                            }
                            break;
                        case 2:
                            if (--obj->UserData[0] == 0) {
                                NEXT(obj->mode0);
                                obj->mode1 = 0;
                            }
                            FATALDEFAULT
                    }
                    CATrajectory(obj);
                    if ((obj->UserData[0] & 1) == 0) {
                        check_rect_queue_draw(obj);
                    }
                    break;
                case 4:
                case 6:
                    FreeActor(obj);
                    break;
                FATALDEFAULT
            }
            break;
        case 5:
            //212fc
            switch (obj->mode0) {
                case 0:
                    NEXT(obj->mode0);
                    obj->Pool = 0;
                    obj->UserData[1] = 0;
                    obj->UserData[2] = 0;

                    obj->XPI       = RH2DWord(0x21358, 4, 0, obj->Step);
                    obj->YPI       = RH2DWord(0x21358, 4, 1, obj->Step);
                    obj->VelX.full = RH2DWord(0x21358, 4, 2, obj->Step);
                    obj->VelY.full = RH2DWord(0x21358, 4, 3, obj->Step);

                    obj->AclX.full = 0;
                    obj->AclY.full = 0x40;
                    RHSetActionList(obj, RHCODE(0x2155c), obj->SubSel);
                    break;
                case 2:
                    switch (obj->mode1) {
                        case 0:
                            CATrajectory(obj);
                            if (obj->VelY.full < 0) {
                                NEXT(obj->mode1)
                                obj->UserData[1] = 0x1e;
                                obj->UserData[2] = 1;
                            }
                            break;
                        case 2:
                            if (--obj->UserData[1] < 0) {
                                obj->mode0 -= 2;
                                obj->mode1 = 0;
                                if (--obj->LocalTimer == 0) {
                                    obj->mode0 += 4;
                                }
                            }
                            break;
                        FATALDEFAULT
                    }
                    RHActionTick(obj);
                    if (g.libsplatter & 1 && obj->UserData[2]) {
                        check_rect_queue_draw(obj);
                    }
                    break;
                case 4:
                case 6:
                    FreeActor(obj);
                    break;
                FATALDEFAULT
            }
            break;
        case 6:
            //213d8
            switch (obj->mode0) {
                case 0:
                    NEXT(obj->mode0);
                    obj->Pool = 0;
                    obj->UserData[0] = 0;
                    
                    obj->XPI       = RH2DWord(0x21436, 3, 0, obj->Step);
                    obj->YPI       = RH2DWord(0x21436, 3, 1, obj->Step);
                    obj->VelX.full = RH2DWord(0x21436, 3, 2, obj->Step);
                    obj->VelY.full = RH2DWord(0x21436, 3, 3, obj->Step);
                    
                    obj->AclX.full = 0;
                    obj->AclY.full = 0x40;
                    obj->LocalTimer = 0x14;
                    RHSetActionList(obj, RHCODE(0x2155c), obj->SubSel);
                    break;
                case 2:
                    switch (obj->mode1) {
                        case 0:
                            if (--obj->LocalTimer == 0) {
                                NEXT(obj->mode1);
                                obj->UserData[0] = 0x14;
                            }
                            break;
                        case 2:
                            if (--obj->UserData[0] == 0) {
                                NEXT(obj->mode0);
                                obj->mode1 = 0;
                            }
                        FATALDEFAULT
                    }
                    CATrajectory(obj);
                    if ((obj->UserData[0] & 1) == 0) {
                        check_rect_queue_draw(obj);
                    }
                    break;
                case 4:
                case 6:
                    FreeActor(obj);
                    break;
                FATALDEFAULT
            }
            break;
        case 7:
            // 21492
            switch (obj->mode0) {
                case 0:
                    NEXT(obj->mode0);
                    obj->Pool = 0;
                    obj->XPI = RH2DWord(0x214e0, 2, 0, obj->Step);
                    obj->YPI = RH2DWord(0x214e0, 2, 1, obj->Step);
                    if (obj->Step) {
                        obj->Draw1      = 1;
                        obj->Draw2.full = 0xf;
                    }
                    RHSetActionList(obj, RHCODE(0x2155c), obj->SubSel);
                    break;
                case 2:
                    switch (obj->mode1) {
                        case 0:
                            if (obj->AnimFlags < 0) {
                                NEXT(obj->mode1)
                                RHSetAction(obj, RHCODE(0x216f8));
                                obj->Timer = RHWordOffset(0x2152e, RAND8WD);
                            }
                            RHActionTick(obj);
                            break;
                        case 2:
                            obj->mode1 -= 2;
                            RHSetActionList(obj, RHCODE(0x2155c), obj->SubSel);
                            RHActionTick(obj);
                            break;
                        FATALDEFAULT
                    }
                    if (g.libsplatter & 1) {
                        check_rect_queue_draw(obj);
                    }
                    break;
                case 4:
                case 6:
                    FreeActor(obj);
                    break;
                FATALDEFAULT
            }
            break;
        FATALDEFAULT
    }
}

#pragma mark Projectile 207f0
// see barrels.c


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
                RHSetActionList(obj, RHCODE(0x21f26), 0);
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
                    RHSetActionList(obj, RHCODE(0x21bea), 1);
					break;
				case 2:
					if (g.mode2 == 0xc) {
						queuesound(SOUND_GAME_OVER);
						DrawTileText(TILETEXT_GAME_OVER);
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
                    RHSetActionList(obj, RHCODE(0x21bea), 0);
					//setaction_list(obj, data_21bea, 0); TODO
					break;
				case 2:
					if (g.mode2 == 0xc) {
						queuesound(SOUND_GAME_OVER);
						DrawTileText(TILETEXT_GAME_OVER);
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
static void sub_2224c(Object *obj_a4, Object_G2 *obj_a2) {
	obj_a4->exists = TRUE;
	obj_a4->Sel    = 0x44;
	obj_a4->UserByte = 0;
	obj_a4->XPI = g.GPCollX;
	obj_a4->YPI = g.GPCollY;
	obj_a4->Flip = obj_a2->Direction ^ 1;
}
void sub_221ea(Object_G2 *obj_a2) {
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
void sub_221bc(Object_G2 *obj_a2) {
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
                RHSetActionList(obj, RHCODE(0x2246a), obj->UserByte + 1);
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
                RHSetActionList(obj, RHCODE(0x2246a), obj->SubSel);
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
            RHSetActionList(obj, RHCODE(0x2278c), obj->SubSel);
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

