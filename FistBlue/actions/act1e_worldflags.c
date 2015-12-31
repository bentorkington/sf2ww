/*
 *  act1e_selcursor.c
 *  MT2
 *
 *  Created by Ben on 5/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */
#include "sf2.h"

#include "particle.h"
#include "sound.h"
#include "structs.h"
#include "lib.h"

#include "act1e_worldflags.h"

extern Game g;
typedef struct UserData_Act1e UD1E;

static void sub_18d9a(Object *obj, Player *ply) {
	UD1E *ud = (UD1E *)&obj->UserData;
	
	if (ply->Human && ply->FighterID == obj->UserByte) {
		if (ply->SelectComplete) {
			ud->x0080 = ply->Side ? 0x1916a : 0x1915a;
		} else {
			ud->x0080 = 0x19142;
		}
	}
}
static void sub_18e52(Object *obj) {
    RHSetActionList(obj, RHCODE(0x19142), obj->UserByte);
}

void action_1e(Object *obj) {		//18c1c
	UD1E *ud = (UD1E *)&obj->UserData;
    
	switch (obj->SubSel) {
		case 3:
			if (obj->mode0 == 0) {
                RHSetActionList(obj, RHCODE(0x1912a), obj->UserByte);
			}
			enqueue_and_layer(obj);
			break;
		case 0 ... 2:
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->LocalTimer = 1 * TICKS_PER_SECOND;
					ud->x0084 = 0;
					g.PlaneLandedInCity[obj->UserByte] = FALSE;
                    ud->x0080 = 0x1912a;                //actlist_1912a;
                    RHSetActionList(obj, RHCODE(0x1912a), obj->UserByte);
					if (obj->UserByte >= STAGE_THAILAND_BISON) {
						if (g.UpToBosses == FALSE) {
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
					} else if (g.CurrentStage != STAGE_USA_BALROG) {
						return;
					} else if (obj->SubSel == 1) {
						if (g.x0a02) {
							obj->mode1 = 16;
							sub_18e52(obj);
						} else {
							obj->mode1 = 10;
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
                                ud->x0080 = 0x1917a;
							} else {
                                ud->x0080 = 0x1912a;
							}
							sub_18d9a(obj, PLAYER1);
							sub_18d9a(obj, PLAYER2);
                            
                            RHSetActionList(obj, RHCODE(ud->x0080), obj->UserByte);
							if (g.PlaneLandedInCity[obj->UserByte]) {
								NEXT(obj->mode1);
							}
							break;
						case 2:				
							actiontick(obj);
							break;
						case 4:
							NEXT(obj->mode1);
							obj->LocalTimer = 3 * TICKS_PER_SECOND;
                            RHSetActionList(obj, RHCODE(0x1a200), obj->UserByte);
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
                            obj->LocalTimer    = RH2DByte(0x18e2a, 2, obj->UserByte - 9, 0);
							obj->SubTimer      = RH2DByte(0x18e2a, 2, obj->UserByte - 9, 1);
                            RHSetActionList(obj, RHCODE(0x1a200), obj->UserByte);
							break;
						case 12:
							if (--obj->LocalTimer == 0) {
								NEXT(obj->mode1);
								ud->x0084 = 0;
								queuesound(0x23);		/*dingdong!*/
							}
							break;
						case 14:
							if (--obj->SubTimer == 0) {
								NEXT(obj->mode1);
								sub_18e52(obj);
							}
							break;
						case 16:
							obj->mode1 = 0;
							if (obj->UserByte == STAGE_THAILAND_SAGAT) {
								g.Pause_9e1 = 1;
								g.x0a02     = TRUE;
							}
							goto FLAGAGAIN;			// suck it
							break;
							FATALDEFAULT;
					}
					if (ud->x0084 == 0) {
						enqueue_and_layer(obj);
					}
					break;
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
void action_2c (Object *obj) {
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
            RHSetActionList(obj, RHCODE(0x1a200), obj->UserByte);
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
