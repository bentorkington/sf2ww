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

#include "actiondata.h"


extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;


static void _portrait_scroll2(const SimpleImage *a1, short d1, u16 **gfx_p);
static POINT16 Act23RandomSmallOffset(void);
static Player *sub_1e7ae(Object *obj);
static void sub_1e59a(Object *obj);
static void sub_1e84c(Object *obj);
static void Act23SMBlood(Object *obj);

static void action_0(Object *obj);
static void action_1(Object *obj);	/* cb2a */
static void action_2(Object *obj);
static void action_3(Object *obj);
static void action_4(Object *obj);
static void action_05(Object *obj);
static void action_06(Object *obj);
static void action_07(Object *obj);
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

static void action_19(Object *obj);

static void action_1e(Object *obj);
static void action_1f(Object *obj);
static void action_20(Object *obj);
static void action_21(Object *obj);
static void action_22(Object *obj);
static void action_1e420(Object *obj);

static void action_2c(Object *obj);

static void action_2e(Object *obj);
static void action_2f(Object *obj);
static void action_30(Object *obj);

static void action_35(Object *obj);
static void action_36(Object *obj);	/* screenwobble */
static void action_37(Object *obj);
static void action_38(Object *obj);
static void action_39(Object *obj);
static void action_3a(Object *obj);
static void action_3b(Object *obj);

static void action_48(Object *obj);
short g_d7;


static void _draw_frame_corners(u16 **gfx_p, u32 cp );	/* 15df6 */


typedef struct UserData_Act07 UD07;
typedef struct UserData_Act09 UD09;
typedef struct UserData_Act0B UD0B;
typedef struct UserData_Act0E UD0E;
typedef struct UserData_Act11 UD11;
typedef struct UserData_Act12 UD12;
typedef struct UserData_Act1e UD1E;
typedef struct UserData_Act23 UD23;
typedef struct UserData_Act2e UD2E;
typedef struct UserData_Act2f UD2F;

typedef struct UserData_Act35 UD35;
typedef struct UserData_Act36 UD36;
typedef struct UserData_Act3b UD3B;
typedef struct UserData_Act48 UD48;


/* 
 MAIN ENTRY
 proc_actions, loop over all ations, check if active, and call back */

void proc_actions(void) {			/* c7da */
	short i;
	debughook(2);
	g_d7=0x3b;
	for(i=0; i<0x3b; i++) {
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
				
				
				ACT117C(0x19, action_19)
				
				ACT117C(0x1e, action_1e)
				ACT117C(0x1f, action_1f)
				ACT117C(0x20, action_20)
				ACT117C(0x21, action_21)
				ACT117C(0x22, action_22)
				ACT117C(0x23, action_1e420)
				
				ACT117C(0x2c, action_2c)
				
				ACT117C(0x2e, action_2e)
				ACT117C(0x2f, action_2f)
				ACT117C(0x30, action_30)
				
				ACT117C(0x35, action_35)
				ACT117C(0x36, action_36)
				ACT117C(0x37, action_37)
				ACT117C(0x38, action_38)
				ACT117C(0x39, action_39)
				ACT117C(0x3a, action_3a)
				ACT117C(0x3b, action_3b)
				
				ACT117C(0x48, action_48)
			default:
				printf("action 258a id %02x not implemented\n", g.Objects3[i].Sel);
				FreeActor(obj);
				break;
		}
		g_d7--;
	}
}


static void sub_d2ba(Object *obj, Object *nobj1, Object *nobj2) {
	struct UserData_Act2 *ud = (struct UserData_Act2 *)&obj->UserData;
	struct UserData_Act2 *udn1 = (struct UserData_Act2 *)&nobj1->UserData;
	
	short temp;
	const static short data_d326[] = { 29,30,31,29,30,31,29,30 } ;
	
	ud->x0080++;
	nobj1->exists = TRUE;
	nobj1->Sel = SF2ACT_0X02;
	nobj1->SubSel = 1;
	nobj1->Owner = (Player *)obj;
	udn1->x0082 = nobj2;		/* u16 to pointer */
	
	nobj2->exists = TRUE;
	nobj2->Sel = SF2ACT_0X02;
	nobj2->SubSel = -1;
	nobj2->Owner = (Player *)obj;
	
	if (sf2rand() & 1) {
		nobj1->Step = 1;
		nobj2->Step = 1;
	}
	nobj1->Draw1 = TRUE;
	nobj2->Draw1 = TRUE;
	
	temp = data_d326[(sf2rand() & 0xe) >>1];	/* 8 x u16 */
	nobj1->Draw2.part.integer = temp;
	nobj2->Draw2.part.integer = temp;
}
static void sub_d3de(Object *obj) {
	struct UserData_Act2 *ud = (struct UserData_Act2 *)&obj->UserData;
	
	if (obj->SubSel >= 0) {
		ud->x0082->XPI = obj->XPI-8;
		if (obj->Step) {
			ud->x0082->XPI += 0x10;
		}
		ud->x0082->YPI = obj->YPI + 0x40;
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
			obj->Pool=data_cb60[obj->SubSel];
			obj->OnGround=data_cb6c[obj->SubSel];
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
static void action_2(Object *obj) {				//d240 Bicycle people
	Object *nobj1, *nobj2;
	struct UserData_Act2 *ud = (struct UserData_Act2 *)&obj->UserData;
	
	short data_d3ae[]={-0x200, 0, 0x200, 0};
	
	if(obj->SubSel) {
		/* d33a */
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				obj->Pool   = 6;
				obj->Scroll = 0;
				obj->ZDepth  = 64;	
				obj->Flip   = obj->Step;
				obj->XPI    = gstate_Scroll2.XPI;
				obj->XPI   += obj->Step ? -0x60 : 0x1d0;
				obj->YPI    = 0x40;
				obj->Path   = data_d3ae;			/* 4 x u16 */
				if (obj->SubSel & 0x80) {
					/* d39e */
					setaction_list(obj, actlist_d6e8, RAND8);
				} else {
					setaction_direct(obj, actlist_d438);
				}
				break;
			case 2:
				/* d3b6 */
				if(obj->SubSel & 0x80 == 0) {
					if((g.libsplatter + g_d7) & 7 == 0) {
						die_if_offscreen(obj);	/* check if still on screen */
					}
					update_motion(obj);
					sub_d3de(obj);
					actiontick(obj);
				}
				check_rect_queue_draw(obj);
				break;
			case 4:
			case 6:		//object went offscreen
				/* d412 */
				if (obj->SubSel >= 0) {
					struct UserData_Act2 *udowner = (struct UserData_Act2 *)&obj->Owner->UserData;
					
					udowner->x0080--;
					udowner->x0082->mode0 = 6;
				}
				FreeActor(obj);
				break;
				
				FATALDEFAULT;
		}		
	} else {
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				ud->x0080=0;
				break;
			case 2:
				/* d266 */
				if(obj->mode1) {
					/* d292 */
					if(obj->Timer-- == 0) {
						if (ud->x0080 < 4 && g.FreeLayer3 >= 2) {
							nobj1 = AllocActor();
							nobj2 = AllocActor();
							sub_d2ba(obj,nobj1, nobj2);
						}
						obj->mode1 = 0;
					}
				} else {
					NEXT(obj->mode1);
					obj->Timer = (u16 []){60,300,300,420,420,420,420,300}[(RAND32 & 0xe) >> 1];
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
		ply->x0142 = FALSE;
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
									if(obj2 = AllocActor()) {
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
static void action_07(Object *obj) {		//e6cc 
	UD07 *ud = (UD07 *)&obj->UserData;
	
	static const char data_e816[] = {0, 0, 4, 4};
	static const short data_e81a[][2] = {{0x1c8, 0xb8}, {0x2d8, 0xb8}, {0x170, 0xb0}, {0x2f0, 0xb0}};
	
	
	short elephant, i;
	Object *child;
	
	if (obj->SubSel == 0) {		// Controller
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				for (i=0; i<4; i++) {
					if (child = AllocActor()) {
						obj->exists = TRUE;
						obj->Sel = SF2ACT_INDIA_ELEPHANTS;
						obj->SubSel = i+1;
						ud->x0080[i]=child;
						obj->Pool = 2;
					}
				}
				break;
			case 2:
				switch (obj->mode1) {
					case 0:
						NEXT(obj->mode1);
						obj->LocalTimer = (short []){0x36, 0x78, 0xb4, 0x3c, 0x78, 0x3c, 0xb4, 0x78}[RAND8];
						break;
					case 2:
						if (--obj->LocalTimer == 0) {
							obj->mode1 = 0;
							elephant = RAND4;
							if (ud->x0080[elephant]->UserData[0] == 0) {
								ud->x0080[elephant]->UserData[1] = TRUE;
							}
						}
						break;
					FATALDEFAULT;
				}
				if (g.FightOver) {
					NEXT(obj->mode0);
					obj->mode1 = 0 ;
				}
				break;
			case 4:			//e78a
				switch (obj->mode1) {
					case 0:
						for (i=0; i<4; i++) {
							if (ud->x0080[i]->UserData[0]) {
								return;
							}
							/* all elephants have finished speaking */
							NEXT(obj->mode1);
						}
						break;
					case 2:
						NEXT(obj->mode0);	// die and kill the elephants
						obj->mode1 = 0;
						for (i=0; i<4; i++) {
							ud->x0080[i]->mode1 = 4;
						}
						break;
					FATALDEFAULT;
				}
				break;
			case 6:
				FreeActor(obj);
				break;
			FATALDEFAULT;
		}
	} else {
		/* e7da child elephant */
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				obj->UserData[0] = 0;
				obj->UserData[1] = 0;
				obj->Scroll = data_e816[obj->SubSel];
				obj->XPI = data_e81a[obj->SubSel][0];
				obj->YPI = data_e81a[obj->SubSel][1];
				break;
			case 2:
				switch (obj->mode1) {
					case 0:
						if (obj->UserData[1] != 0) {
							NEXT(obj->mode1);
							obj->UserData[1] = 0;
							obj->LocalTimer = 0x70;
							obj->UserData[0]=TRUE;
							setactiondraw(obj, simpleact_e8ca,obj->SubSel - 1);
							queuesound(SOUND_ELEPHANT);
						}
						break;
					case 2:
						if (--obj->LocalTimer == 0) {
							obj->mode3 = 0;
							obj->UserData[0] = FALSE;
						}
						actiontickdraw(obj);
						break;
					case 4:
						NEXT(obj->mode1);
						setactiondraw(obj, simpleact_e982, obj->SubSel - 1);
						if (obj->SubSel == 1) {
							queuesound(SOUND_ELEPHANT);
						}
						break;
					case 6:
						actiontickdraw(obj);
						if (obj->SubSel == 1) {
							if (obj->AnimFlags & 1) {
								gstate_Scroll1.YPI = 0xf8;
							} else {
								gstate_Scroll1.YPI = 0;
							}
						}
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}

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
				if (newobj = pop_5d0c()) {
					newobj->exists = TRUE;
					newobj->Sel = 0;
					newobj->SubSel = 14;
				}
				if (newobj = pop_5d0c()) {
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
					if (obj->AnimFlags & 0xff == 1) {
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
			// XXX setactiondraw(obj, actlist_10492, obj->SubSel);
			break;
		case 2:
			if (obj->SubSel == 3) {
				// 104d0
				if (obj->mode1) {
					actiontickdraw(obj);
				} else {
					if (g.x8a66[obj->Step]) {
						NEXT(obj->mode1);
						// XXX setactiondraw(obj, actlist_10628, obj->Step);
					}
				}
			} else {
				if (obj->mode1 == 0 && g.FightOver) {
					NEXT(obj->mode1);
					// XXX setactiondraw(obj, actlist_104c6, obj->Step);
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
			// XXX setaction_direct(obj, actlist_10822);
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
						// XXX setaction_direct(obj, action_10852);
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
					}[RAND8W/2];
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
					obj->LocalTimer = (char []) {
						0x3c, 0x28, 0x5a, 0x28, 0x28, 0x3c, 0x14, 0x46,
					} [RAND8];
					obj->VelX.full = (short []) {
						0xff00, 0xfee0, 0xffb0, 0xfe80, 0xff60, 0xff40, 0xfe00, 0xfde0,
					} [RAND8W];
					obj->VelY.full = (short []) {
						0x0040, 0x0063, 0x0028, 0x0080, 0x0060, 0x0023, 0x0088, 0x0058,
					} [RAND8W];
					obj->AclX.full = (short []){
						0xfffe, 0xfffc, 0x0008, 0x0004, 0xfffc, 0x0002, 0xfffc, 0xfff8,
					}[RAND8W];
					obj->AclY.full = (short[]) {
						0x0005, 0x0006, 0x0007, 0x0008, 0x000a, 0x0010, 0x0011, 0x0012,
					}[RAND8W];
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
					if (sf2rand() & 3 == 0) {
						if (nobj = AllocActor()) {
							nobj->exists = TRUE;
							nobj->Sel = 0x11;
							nobj->SubSel = 0x1;
							nobj->ZDepth = obj->ZDepth;
							nobj->Scroll = 0;
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
		case 0:
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->Scroll = -1;
					obj->Pool	= 6;
					obj->XPI	= 0xc0;
					obj->YPI	= 0x98;
					setaction_list(obj, actlist_11602, 1);
					/* FALLTHRU */
				case 2:
					enqueue_and_layer(obj);
				case 4:
				case 6:
					FreeActor(obj);
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
								ud->h0080c = 1;
								obj->Step = 1;
								obj->LocalTimer = 0x10;
								obj->Draw1 = -1;
							}
							break;
						case 2:
							obj->Step = (obj->Step + 2) & 0x1f;
							if (obj->Step == 0) {
								NEXT(obj->mode1);
								if (nobj=AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x29;
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
								if (nobj=AllocActor()) {
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
	
	_portrait_scroll2(data_15f8e[fighterid], flip, gfx_p);
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
	if (obj=AllocActor()) {
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
		start_effect(0x2400 + *(data + 8*g.BattleWinner + (RAND8W/2)), 0x0102);
	} else {	
		start_effect(0x1800 + *(data + 8*g.BattleWinner + (RAND8W/2)), 0x0102);
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
			obj->Scroll = 0;
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
		default:
			break;
	}
}


#pragma mark Act17

static void action_17(Object *obj) {
	//UD17 *ud = (UD17 *)&obj->UserData;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			//ud->h0082c = 0;
			if (0x9249 & (1 << RAND16) == 0) {
				obj->Pool = 6;
			}
			//setaction_list(obj, data_10a72, obj->SubSel);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					//obj->VelX = data_[RAND8W];
					//obj->VelY = data_[RAND8W];
					//obj->XPI += data_[RAND16W];
					//obj->YPI += data_[RAND8W];
					//ud->h0080w += 0x28;
					obj->AclY.full = 0x0040;
					obj->AclX.full = 0;
					obj->LocalTimer = 0x32;
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
						//ud->h0082c = 0x1e;
					}
					CATrajectory(obj);
					if (obj->VelY.full < 0) {
						/* XXX oh god! */
					}
				case 4:
					//todo
					
					
					break;
				FATALDEFAULT;
			}
			//if ((ud->h0085c & 1) == 0) {
			//	check_rect_queue_draw(obj);
			//}
			break;
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

#pragma mark MARK
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


#pragma mark MARK

void action_1ab8a() {		/* 1ab8a */
	int i;
	for (i=0; i<16; i++) {
		OBJECT_DRAW_ID(128 + i, 0, 0, 0, 0);
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
	
	if (obj=AllocActor()) {
		struct UserData_Act1d *ud = (struct UserData_Act1d *)&obj->UserData;

		obj->exists = TRUE;
		obj->Sel = SF2ACT_0X1D;
		ud->x0080 = g.BattleLoser;
		obj->XPI = 0x10;
		obj->YPI = 0xb0;
		obj->SubSel = 0;
		ud->x0084 = 0;
		ud->x0088 = g.PlyLostToComp ? PLAYER1 : PLAYER2;
	}
}


#pragma mark Act1E
static void sub_18d9a(Object *obj, Player *ply) {
	UD1E *ud = (UD1E *)&obj->UserData;

	if (ply->Human && ply->FighterID == obj->UserByte) {
		if (ply->SelectComplete) {
			ud->x0080 = ply->Side ? actlist_1916a : actlist_1915a;
		} else {
			ud->x0080 = actlist_19142;
		}
	}
}
static void sub_18e52(Object *obj) {
	setaction_list(obj, actlist_19142, obj->UserByte);
}



static void action_1e(Object *obj) {		//18c1c
	UD1E *ud = (UD1E *)&obj->UserData;
	static const char data_18e2a[][2]={{0x78, 0x78},{0x28,0xc8},{0x50,0xa0}};
	switch (obj->SubSel) {
		case 3:
			if (obj->mode0 == 0) {		
				setaction_list(obj, actlist_1912a, obj->UserByte);		// flags
			}
			enqueue_and_layer(obj);
			break;
		case 0 ... 2:
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->LocalTimer = 60;
					ud->x0084 = 0;			//byte
					g.x8a68[obj->UserByte] = 0;
					ud->x0080 = actlist_1912a;
					setaction_list(obj, actlist_1912a, obj->UserByte);
					if (obj->UserByte >= STAGE_THAILAND_BISON) {
						if (g.UpToBosses == 0) {
							obj->mode0 = 6;
							return;
						}
						if (obj->UserByte != STAGE_THAILAND_BISON) {
							if (obj->UserByte == STAGE_THAILAND_SAGAT && g.OnLevel8 && g.x0a03 == 0) {
								obj->mode0 = 6;
								return;
							}
						} else {
							if (g.OnLevel8 == 0) {
								obj->mode0 = 6;
								return;
							}
						}

					}
					//18cc0
					if (obj->SubSel == 2) {
						obj->mode1 = 4;
					} else if (g.CurrentStage == STAGE_USA_BALROG) {
						return;
					} else if (obj->SubSel == 1) {
						if (g.x0a02) {
							obj->mode3 = 0x10;
							sub_18e52(obj);
						} else {
							obj->mode3 = 0xa;
						}
					}
					
					break;
				case 2:
					if (g.CurrentStage != STAGE_USA_BALROG && obj->UserByte == 0 && obj->Step == 0 && obj->mode2 == 0) {
						if (--obj->LocalTimer == 0) {
							g.Pause_9e1 = 1;
							obj->mode2 += 2;
						}
					}
					switch (obj->mode1) {
						case 0:
						FLAGAGAIN:
							if (g.Defeated[obj->UserByte]) {
								ud->x0080 = actlist_1917a;
							} else {
								ud->x0080 = actlist_1912a;	
							}
							sub_18d9a(obj, PLAYER1);
							sub_18d9a(obj, PLAYER2);
							setaction_list(obj, ud->x0080, obj->UserByte);
							if (g.x8a68[obj->UserByte]) {
								NEXT(obj->mode1);
							}
							break;
						case 2:				
							actiontick(obj);
							break;
						case 4:
							NEXT(obj->mode1);
							obj->LocalTimer = 0xb4;
							setaction_list(obj, actlist_1a200, obj->UserByte);
							break;
						case 6:
							if (--obj->LocalTimer == 0) {
								NEXT(obj->mode1);
								g.x0a03 = 1;
								g.Pause_9e1 = -1;
							}
							break;
						case 8:		/* nothing */  break;
						case 10:
							NEXT(obj->mode1);
							ud->x0084 = 1;
							obj->LocalTimer = data_18e2a[obj->UserByte - 9][0];
							obj->x001f      = data_18e2a[obj->UserByte - 9][1];
							setaction_list(obj, actlist_1a200, obj->UserByte);
							break;
						case 12:
							if (--obj->LocalTimer == 0) {
								NEXT(obj->mode1);
								ud->x0084 = 0;
								queuesound(0x23);
							}
							break;
						case 14:
							if (--obj->x001f == 0) {
								NEXT(obj->mode1);
								sub_18e52(obj);
								}
							break;
						case 16:
							obj->mode1 = 0;
							if (obj->UserByte == STAGE_THAILAND_SAGAT) {
								g.Pause_9e1 = 1;
								g.x0a02 = 1;
							}
							goto FLAGAGAIN;			// suck it, bitches
							break;
						FATALDEFAULT;
					}
					if (ud->x0084 == 0) {
						enqueue_and_layer(obj);
					}
				case 4:
				case 6:
					FreeActor(obj);
				default:
					break;
			}
			break;
		default:
			break;
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
	
	Player *ply = obj->SubSel ? PLAYER1 : PLAYER2;
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
	static const short data_1de94[4][2] = {{ -2, 0}, {2, -2}, {0, 0}, {-2, 2}};
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
				obj->XPI += data_1de94[temp][0];
				obj->YPI += data_1de94[temp][1];
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
	if (obj = AllocActor()) {
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
			NEXT(obj->mode0);
			obj->Scroll = -1;
			obj->Pool	= 2;
			obj->XPI	= 0xc0;
			obj->YPI	= 0xcf;
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
	if (obj=AllocActor()) {
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
			obj->Scroll = -1;
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

void ActStartVomit(Player *ply) {			// 1e304
	UD23 *ud;
	
	Object *obj;
	if (obj = AllocActor()) {
		ud = (UD23 *)&obj->UserData;
		obj->SubSel = 0;
		obj->flag1  = TRUE;
		obj->Sel    = SF2ACT_VOMIT;
		obj->UserByte  = sf2rand() & 1;		// orange or grey vomit?
		obj->Owner  = ply;
		ud->x0093   = ply->Side;
		ud->x008e   = &FreeActor;	//??
	}
}
void action_1e336(Player *ply) {
	UD23 *ud;

	Object *obj;
	char temp;
	
	static const char data_1e39c[] = {
		0,0,0,1,1,2,2,2,0,0,0,1,1,2,2,2,
		0,0,0,1,1,1,2,2,0,0,0,1,1,1,2,2,
	};
	
	if (obj = AllocActor()) {
		ud =  (UD23 *)&obj->UserData;
		obj->SubSel = 2;
		temp = data_1e39c[RAND32];
		if (temp) {			// hmm, wierd, but checked.
			obj->UserByte = temp-1;
			obj->exists = TRUE;
			obj->Sel = SF2ACT_VOMIT;	
			obj->Owner = ply;
			ud->x0093 = ply->Side;
			ud->x008e = &FreeActor;
		}
	}
}
static void _CreateDizzyObject(Player *ply, short d2) {		// 1e3c8
	Object *obj;
	UD23 *ud;
	if (obj=AllocActor()) {
		ud = (UD23 *)&obj->UserData;
		ud->x008c	= d2;
		obj->exists = TRUE;
		obj->Sel	= SF2ACT_VOMIT;
		obj->SubSel = 4;		
		obj->UserByte = ply->DizzySpell < 0x8c ? 0 : 1 ;	// 0=stars,1=birds
		obj->Owner = ply;
		obj->X.full = obj->Y.full = 0;
	}
}
void StartDizzyAnim(Player *ply) {				// 1e3bc
	_CreateDizzyObject(ply, 0);
	_CreateDizzyObject(ply, 8);
	_CreateDizzyObject(ply, 16);
}
void ActBlankaBiteBlood(Player *ply) {			//1e402
	Object *obj;
	if (obj=AllocActor()) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_VOMIT;
		obj->SubSel = 6;
		obj->Owner = ply;
	}
}
static void Act23SMVomit(Object *obj) {					// 1e43a
	UD23 *ud = (UD23 *)obj->UserData;
	POINT16 op;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			setaction_list(obj, actlist_1e8d6, obj->SubSel + obj->UserByte);
			obj->Flip = obj->Owner->Flip;
			ud->PlyX = obj->Owner->XPI;
			ud->PlyY = obj->Owner->YPI;
			obj->LocalTimer = 0x1e;
			obj->Pool = 0;
			sub_1e7ae(obj);
			op = Act23RandomSmallOffset();
			obj->XPI += op.x;
			obj->YPI += op.y;
			break;
		case 2:
			if (ud->x0093) { obj->Pool = 10; } else { obj->Pool = 8; }

			// move the vomit with the player
			obj->XPI += (obj->Owner->XPI - ud->PlyX);
			ud->PlyX = obj->Owner->XPI;
			obj->YPI += (obj->Owner->YPI - ud->PlyY);
			ud->PlyY = obj->Owner->YPI;
			
			if ((obj->AnimFlags & 0xff) == 0) {
				if (--obj->LocalTimer == 0) {
					NEXT(obj->mode0);
				}
			} else {
				NEXT(obj->mode0);
			}
			actiontick(obj);
			if (obj->SubSel == 0) {
				if (--ud->x0092) {
					check_rect_queue_draw(obj);
				} else {
					ud->x0092 = 3;
				}
				
			} else {
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



static void action_1e420(Object *obj) {		/* obj in %a6 */
	switch (obj->SubSel) {
		case 0:							//	teeth, blood
		case 2:							//  orange and grey vomit
			Act23SMVomit(obj);
			break;
		case 4:
			sub_1e59a(obj);				// animated star, bird
			break;
		case 6:							// small star, bird
			Act23SMBlood(obj);
			break;
		case 8:							// blood again
			sub_1e84c(obj);			
			break;
		FATALDEFAULT;
	}
}





static Player *sub_1e7ae(Object *obj) {		// 1e7ae
	char x,y;
	Player *ply = obj->Owner;	

	x = data_1e804[ply->FighterID][obj->SubSel/2].x;
	y = data_1e804[ply->FighterID][obj->SubSel/2].y;
	
	if (ply->Flip) {obj->XPI = ply->XPI - x;} else {obj->XPI = ply->XPI + x;}
	obj->YPI = ply->YPI + y;

	return ply;
}

static void sub_1e59a(Object *obj) {		// birds and stars?
	UD23 *ud = (UD23 *) &obj->UserData;
	
	Player *ply;
	short *temp;
	short temp2;
	
	switch (obj->mode0) {
		case 0:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->LocalTimer = 1;
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
						setaction_list(obj, actlist_1e8d6, obj->SubSel + obj->UserByte);
						ply = sub_1e7ae(obj);
						obj->Flip = ply->Flip;
						ud->PlyX = ply->XPI;
						ud->PlyY = ply->YPI;
						ud->ObjX = obj->XPI;
						ud->ObjY = obj->YPI;
						temp = &data_1e622[ud->x008c / 2];
						obj->XPI      += temp[0];
						obj->VelX.full = temp[1];
						obj->YPI      += temp[2];
						obj->VelY.full = temp[3];
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 2:				// 1e63a
			ply = obj->Owner;
			if (ply->DizzyStun == 0 || (ply->mode1 != PLSTAT_REEL && ply->mode1 != PLSTAT_TUMBLE) ) {
				NEXT(obj->mode2);	/* no longer dizzy */
			}
			obj->VelX.full = obj->XPI < ud->ObjX ? 0x100 : -0x100;
			obj->VelY.full = obj->YPI < ud->ObjY ?  0x40 : -0x40;
			
			temp2 = ply->XPI - ud->PlyX;
			obj->XPI += temp2;
			ud->ObjX += temp2;
			ud->PlyX = ply->XPI;
			
			temp2 = ply->YPI - ud->PlyY;
			obj->YPI += temp2;
			ud->ObjY += temp2;
			ud->PlyY = ply->YPI;
			
			obj->Flip = ply->VelX.full < 0 ? 1 : 0;
			obj->Pool = obj->Flip * 4;
			
			
			setaction_list(obj, actlist_1e8d6, ((obj->Flip * 2) + 4 + obj->UserByte));
			CAApplyVelocity(obj);		/* bottom half of calc-trajectory */
			if (obj->UserByte) {
				obj->Draw1 = TRUE;
				obj->Draw2.part.integer = data_1e6f4[(g.tick & 0x6) / 2];
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
static POINT16 Act23RandomSmallOffset(void) {		//1e888
	static const char data_1e8a6[] = {-2, 2, 0, -2, 2, 0, -2, 2 };
	
	POINT16 op;
	short temp = sf2rand();
	op.x = data_1e8a6[temp & 7];
	op.y = data_1e8a6[(temp >> 3) & 7];
	return op;
}
static void sub_1e77e(Object *obj) {
	UD23 *ud = (UD23 *) &obj->UserData;

	if (obj->AnimFlags && 0xff) {
		NEXT(obj->mode0);
	}
	actiontick(obj);
	if(--ud->x0092) {
		check_rect_queue_draw(obj);
	} else {
		ud->x0092 = 3;
	}
}

static void Act23SMBlood(Object *obj) {		// 1e6fc
	Player *a0;
	short temp;
	POINT16 op;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			a0 = obj->Owner;
			obj->Flip = obj->Owner->Flip;
			obj->XPI  = obj->Owner->XPI;
			obj->YPI  = obj->Owner->YPI;
			if (obj->Owner->FighterID == FID_BLANKA) {
				obj->XPI += obj->Flip ? 32 : -32 ;		
				obj->YPI += 69;
			} else {
				obj->XPI += obj->Flip ? 37 : -37 ;
				obj->YPI += 82;
			}
			obj->LocalTimer = 30;
			obj->Pool		= 0;
			op = Act23RandomSmallOffset();
			obj->XPI += op.x;
			obj->YPI += op.y;
			setaction_list(obj, actlist_1e8d6, 8);		/* star */
			break;
		case 2:
			sub_1e77e(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}


static void sub_1e84c(Object *obj) {
	UD23 *ud = (UD23 *) &obj->UserData;

	POINT16 op;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Flip ^= 1;
			op = Act23RandomSmallOffset();
			obj->XPI += op.x;
			obj->YPI += op.y;
			ud->x0092 = 3;
			setaction_direct(obj, actlist_1eb42);
			/* FALL THRU */
		case 2:
			sub_1e77e(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
	
	
}

#pragma mark ----


void action_1cd3c(Player *ply) {
	/* XXX vegas claw falls off */
}






#pragma mark MARK

static void _animate_beaten_port(u16 **scr_p, char d2, char d3) {		//15cfr2
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
	
	d3 *= 11;
	d3 += ((sf2rand() & 0xe) + (g.libsplatter & 6)) % 10;
	
	_portrait_scroll2((const SimpleImage *)data_15d36[d3/2], d2, scr_p);
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

static void _portrait_scroll2(const SimpleImage *a1, short d1, u16 **scr_p) {			// 1601c
	short width, height;
	const u16 *img = (const u16 *)a1;
	width  = *img++;
	height = *img++;
	img++;
	sub_4386(d1, width, height, img, scr_p);	/*backside of drawsimple_scroll2attr_nocheck */
}

static void sub_15c02(u16 **scr_p, short d1, short fid_d3) {
	static const u16 *data_15c16[12]={
		data_16a5a, data_16b40, data_16c26, data_16d0c, 
		data_16df2, data_16ed8, data_16fbe, data_170a4, 
		data_1718a, data_17270, data_17356, data_1743c, 
	};
	_portrait_scroll2((const SimpleImage *)data_15c16[fid_d3], d1, scr_p);		/* offsets to simpleimages x 18 */
}
static void sub_15ff0(u16 **scr_p, short d1, short fid_d3) {
	static const u16 *data_16004[12]={
		data_16a5a, data_16b40, data_16c26, data_16d0c, 
		data_16df2, data_16ed8, data_16fbe, data_170a4, 
		data_1718a, data_17270, data_17356, data_1743c, 
	};
	
	_portrait_scroll2((const SimpleImage *)data_16004[fid_d3], d1, scr_p);			/* 12 * offset simpleimage */
}

/* portrait drawing, not scheduled */
void actionlib_draw_portraits(void) {		/* 15f9e */
	u16 *gfx_p;
	u16 *scr_p;
	
	gfx_p = CPS_OBJ(0x910080);
	_draw_frame_corners(&gfx_p,  MakePointObj(240, 112));		// draw corners
	scr_p = coords_scroll2(240, 192);
	sub_15ff0(&scr_p, 0, g.Player2.FighterID);		// was 0

	gfx_p = CPS_OBJ(0x910070);
	_draw_frame_corners(&gfx_p, MakePointObj(16, 112));
	scr_p = coords_scroll2(16, 192);
	sub_15ff0(&scr_p, 1, g.Player1.FighterID);		// was 1
}
void action_draw_ports(void) {	/* 15c2e after a fight, one beaten up */
	u16 *gfx_p;
	u16 *scr_p;
	
	if(g.BattleWinSide) {
		gfx_p=CPS_OBJ(0x910070);
		_draw_frame_corners(&gfx_p, MakePointObj(16, 112));			
		
		scr_p = coords_scroll2(16, 192);
		_animate_beaten_port(&scr_p, 1, g.BattleLoser);
		
		gfx_p=CPS_OBJ(0x910080);
		_draw_frame_corners(&gfx_p, MakePointObj(240, 112));
		
		scr_p = coords_scroll2(240, 192);
		sub_15c02(&scr_p, 0, g.BattleWinner);		
	} else {
		gfx_p=CPS_OBJ(0x910070);
		_draw_frame_corners(&gfx_p, MakePointObj(16, 112));
		
		scr_p = coords_scroll2(16, 192);
		sub_15c02(&scr_p, 1, g.BattleWinner);
		
		gfx_p=CPS_OBJ(0x910080);
		_draw_frame_corners(&gfx_p, MakePointObj(240, 112));
		
		scr_p=coords_scroll2(240, 192);
		_animate_beaten_port(&scr_p, 0, g.BattleLoser);
	}
}


#pragma mark Act2C Playerselect something
static void action_2c (Object *obj) {
	switch (obj->SubSel) {
		case 0:
		case 2:
			break;
		FATALDEFAULT;
	}
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			setaction_list(obj, actlist_1a200, obj->UserByte);
			break;
		case 2:
			if(g.Defeated[obj->UserByte]) {
				if (g.Player1.Human == FALSE || g.Player1.FighterID != obj->UserByte) {
					if (g.Player2.Human == FALSE || g.Player2.FighterID != obj->UserByte) {
						enqueue_and_layer(obj);
					}
				}
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}

#pragma mark Act2E Plane
void synth_plane_setup(Object *obj, int city_from, int city_to) {
	UD2E *ud = (UD2E *)obj->UserData;
	ud->city_from = city_from;
	ud->city_to   = city_to;
}

static void action_2e(Object *obj) {		// 18f92
	UD2E *ud = (UD2E *)obj->UserData;
	static const short data_cfe74[64][2] = {
		{ 0x0000, 0x02a0,  },
		{ 0x004b, 0x029c,  },
		{ 0x0095, 0x0292,  },
		{ 0x00de, 0x0282,  },
		{ 0x0125, 0x026c,  },
		{ 0x0169, 0x024f,  },
		{ 0x01aa, 0x022e,  },
		{ 0x01e6, 0x0207,  },
		{ 0x021e, 0x01da,  },
		{ 0x0251, 0x01a9,  },
		{ 0x027e, 0x0174,  },
		{ 0x02a4, 0x013c,  },
		{ 0x02c4, 0x0100,  },
		{ 0x02de, 0x00c2,  },
		{ 0x02f0, 0x0082,  },
		{ 0x02fc, 0x0041,  },
		{ 0x0300, 0x0000,  },
		{ 0x02fc, 0xffbf,  },
		{ 0x02f0, 0xff7e,  },
		{ 0x02de, 0xff3e,  },
		{ 0x02c4, 0xff00,  },
		{ 0x02a4, 0xfec4,  },
		{ 0x027e, 0xfe8c,  },
		{ 0x0251, 0xfe57,  },
		{ 0x021e, 0xfe26,  },
		{ 0x01e6, 0xfdf9,  },
		{ 0x01aa, 0xfdd2,  },
		{ 0x0169, 0xfdb1,  },
		{ 0x0125, 0xfd94,  },
		{ 0x00de, 0xfd7e,  },
		{ 0x0095, 0xfd6e,  },
		{ 0x004b, 0xfd64,  },
		{ 0x0000, 0xfd60,  },
		{ 0xffb5, 0xfd64,  },
		{ 0xff6b, 0xfd6e,  },
		{ 0xff22, 0xfd7e,  },
		{ 0xfedb, 0xfd94,  },
		{ 0xfe97, 0xfdb1,  },
		{ 0xfe56, 0xfdd2,  },
		{ 0xfe1a, 0xfdf9,  },
		{ 0xfde2, 0xfe26,  },
		{ 0xfdaf, 0xfe57,  },
		{ 0xfd82, 0xfe8c,  },
		{ 0xfd5c, 0xfec4,  },
		{ 0xfd3c, 0xff00,  },
		{ 0xfd22, 0xff3e,  },
		{ 0xfd10, 0xff7e,  },
		{ 0xfd04, 0xffbf,  },
		{ 0xfd00, 0x0000,  },
		{ 0xfd04, 0x0041,  },
		{ 0xfd10, 0x0082,  },
		{ 0xfd22, 0x00c2,  },
		{ 0xfd3c, 0x0100,  },
		{ 0xfd5c, 0x013c,  },
		{ 0xfd82, 0x0174,  },
		{ 0xfdaf, 0x01a9,  },
		{ 0xfde2, 0x01da,  },
		{ 0xfe1a, 0x0207,  },
		{ 0xfe56, 0x022e,  },
		{ 0xfe97, 0x024f,  },
		{ 0xfedb, 0x026c,  },
		{ 0xff22, 0x0282,  },
		{ 0xff6b, 0x0292,  },
		{ 0xffb5, 0x029c,  },
	};
	const static u16 data_19040[12][2] = {
		{ 0x00be, 0x00ab,  },
		{ 0x00c4, 0x00b3,  },
		{ 0x012c, 0x008c,  },
		{ 0x0117, 0x00ac,  },
		{ 0x0112, 0x00bb,  },
		{ 0x00ae, 0x00b6,  },
		{ 0x0088, 0x00bf,  },
		{ 0x0092, 0x00a3,  },
		{ 0x00a0, 0x00a0,  },
		{ 0x00a4, 0x00a4,  },
		{ 0x0103, 0x00b0,  },
		{ 0x004e, 0x00b6,  },
	};
	int d6;
	switch (obj->mode0) {
		case 0:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->LocalTimer = 0x32;
					obj->Pool = 2;
					obj->Path = data_cfe74;
					if (ud->city_from == ud->city_to) {
						// 1901e
						g.x8a68[ud->city_to] = TRUE;
						obj->mode0 = 6;		// die
						obj->mode1 = 0;
						g.Pause_9e1 = -1;
					} else {
						//18fd8
						obj->XPI = data_19040[ud->city_from][0];
						obj->YPI = data_19040[ud->city_from][1];
						ud->x_to = data_19040[ud->city_to][0];
						ud->y_to = data_19040[ud->city_to][1];
						d6 = calc_flightpath(obj, ud->x_to, ud->y_to);
						//todo cheeky bitshifts
						obj->Step = d6;
						setaction_list(obj, actlist_19fa2, (d6+1) >> 3);
					}
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
						obj->mode3 = 0;
						queuesound(SOUND_PLANE);
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 2:
			//190a0
			if (obj->mode1 == 0) {
				d6 = calc_flightpath(obj, ud->x_to, ud->y_to);
				obj->Step = (d6 + 2) >> 2;
				if ((ABS(obj->XPI - ud->x_to) > 3)   ||
					(ABS(obj->YPI - ud->y_to) > 3)) {
					update_motion(obj);
					enqueue_and_layer(obj);
				} else {
					NEXT(obj->mode1);	// flight over
					g.Pause_9e1=-1;
					g.x8a68[ud->city_to] = TRUE;
					ud->sound = (short []) {
						SOUND_JAPAN,	SOUND_JAPAN,	SOUND_BRAZIL,	SOUND_USA,
						SOUND_USA,		SOUND_CHINA,	SOUND_USSR,		SOUND_INDIA,
						SOUND_THAILAND,	SOUND_THAILAND, SOUND_USA,		SOUND_SPAIN,
					}[ud->city_to];
					obj->LocalTimer = 0x32;
					enqueue_and_layer(obj);
				}
			} else {
				if (--obj->LocalTimer == 0) {
					queuesound(ud->sound);
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
						g.x8a74 = -1;
						g.x8a75 = 60;
						obj->XPI = 0xb0;
						obj->YPI = 0x610;
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
			setaction_list(obj, actlist_1d4f0, 1);
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
						setaction_list(obj, actlist_1d4f0, 4);
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
			setaction_list(obj, actlist_1d4f0, 5);
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
			setaction_list(obj, actlist_1d4f0, 6);
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
						soundsting(0x2a);
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
static void skyskraperanim_08(Object *obj) {		// 1d41a
	UD2F *ud = (UD2F *)obj->UserData;

	static const u32 data_1d4ac[] = {0x0002c000, 0x00030000, 0x00034000, 0x00028000};
	static const u32 data_1d4bc[] = {0x00020000, 0x00010000, 0x00030000, 0x00000000};
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 4;
			ud->VelY.full    = 0x00000000;
			ud->Gravity.full = 0xffffc000;
			setaction_list(obj, actlist_1d4f0, 7);
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
						soundsting(0x2a);
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
		case 8:			skyskraperanim_08(obj);		break;
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


#pragma mark Act35 Ground Dust

void ActStartGroundDust(Player *ply) {			// 1f980
	Object *obj;
	if (obj=AllocActor()) {
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
				setaction_list(obj, actlist_1fb52, d0);	
				check_rect_queue_draw(obj);
			} else {
				setaction_list(obj, actlist_1fb46, d0);
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
		if (obj=AllocActor()) {
			obj->exists = TRUE;
			obj->Sel = SF2ACT_SCREENWOBBLE;
			g.ScreenWobbleStarted = TRUE;
		}
	}
}

static void sub_1fe36(Object *obj) {
	UD36 *ud = (UD36 *)obj->UserData;

	g.ScreenWobble = 0;
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
			if (--obj->x001f == 0) {
				if(--obj->LocalTimer == 0) {
					NEXT(obj->mode0);
					sub_1fe36(obj);
					return;
				} else {
					obj->x001f = 7;
				}
			}
			if ((obj->x001f & 1)==0) {
				sub_1fe36(obj);
			} else {
				g.ScreenWobble = 3;
				ud->Save_Scroll1Y = gstate_Scroll1.YPI;
				ud->Save_Scroll2Y = gstate_Scroll2.YPI;
				ud->Save_Scroll3Y = gstate_Scroll3.YPI;
				sub_1fe4e(obj, 3);
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
				g.TimeWarpTimer     = 60;
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
				obj->XPI -= 0x28;
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
					if (obj->YPI <= 0x28) {
						NEXT(obj->mode1);
						obj->YPI = 0x28;
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

static void sub_205f6(u32 d0, short d2, u16 *a1) {
	d2 -= 3;
	while (d2 >= 0) {
		a1[0] = (d0 & 0xf) + 0x8100;
		a1--;
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
	const static u16 data_20668[] = {0x1, 0xd, 0x2d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8100};

	
	switch (obj->SubSel) {
		case 0:
			source = data_20640; dest = g.x8a76; break;
		case 2:
			source = data_20654; dest = g.x8a8a; break;
		case 4: 
			source = data_20668; dest = g.x8a9e; break;
		FATALDEFAULT;
	}
	for (i=0; i<10; i++) {
		dest[i] = source[i];
	}
}

static void sub_205d8(Object *obj) {
	UD3B *ud = (UD3B *)&obj->UserData;
	add_bcd_32(ud->x0080.full, &g.x8aac);
	
	add_bcd_32(/* XXX */ 200  , &g.x8ab0);
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
		{0x00d8, 0x00b0},
		{0x00d8, 0x00a0},
		{0x00d8, 0x0088}, 
	};
	
	/* perfect energy score per player */
	static const short data_204d8[12] = {
		0x300, 0x300, 0x300, 0x300, 
		0x300, 0x300, 0x300, 0x300, 
		0x800, 0x500, 0x500, 0x500,		// Bison, three other bosses
	};
	
	static const Action action_2067c = 
	{8, 0, 0, (Image *)&g.x8a76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static const Action action_20694 = 
	{8, 0, 0, (Image *)&g.x8a8a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static const Action action_206ac = 
	{8, 0, 0, (Image *)&g.x8a9e, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	
	switch (obj->mode0) {
		case 0:							// init the animation
			NEXT(obj->mode0);
			obj->Scroll    = -1;
			obj->Pool      = 2;
			ud->x0080.full = 0;	
			
			_init_counter_image(obj);
			
			obj->XPI = data_20412[obj->SubSel][0];
			obj->YPI = data_20412[obj->SubSel][1];
			switch (obj->SubSel) {
				case 0:												// TIME
					ud->x0080.full = (g.TimeRemainBCD << 12);
					if (g.TimeRemainBCD != 0) {
						d2 = (g.TimeRemainBCD & 0xf0) ? 4 : 3;		// number of bcd digits
						g.x8a76[0] = d2;
						g.x8a76[8] = 0x8100;
						sub_205f6(g.TimeRemainBCD, d2, &g.x8a76[8]);
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
						ud->x0080.full = d0 << 16;
						if (d0 < 0x10) {
							d2 = 3;
						} else if (d0 < 0x100) {
							d2 = 4;
						} else {
							d2 = 5;
						}
						g.x8a8a[0] = d2;
						g.x8a8a[8] = 0x8100;
						sub_205f6(d0, d2, &g.x8a8a[8]);
					}
					g.x8ab4 |= 2;
					anim = &action_20694;
					break;
				case 4:											// TOTAL
					ud->x0080.full = g.x8ab2 << 16;
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
						if (ud->x0080.full & 0xff000000) {
							sub_bcd_8(1, &ud->x0080.full);
							ud->x0080.full &= 0xffff0000;
							ud->x0080.full |= 0x00000001;
							sub_205d8(obj);
						} else {
							g.x8ab4 &= 0xfffffffe;
						}
					}
					break;
				case 2:
					if (g.CanSpeedUpScoreCount) {
						if (ud->x0080.split.msb) {
							sub_bcd_16(1, &ud->x0080.full);	// XXX
							ud->x0080.full &= 0xffff0000;
							ud->x0080.full |= 0x00000001;
							
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
						sub_205f6(g.x8ab2, d2, &g.x8a9e[6]);
						if ((g.libsplatter & 3)==0 && g.x8ab2 != ud->x0080.full) {
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



#pragma mark Act48 Speak You Win/Lose

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
			obj->Scroll		= -1;
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

