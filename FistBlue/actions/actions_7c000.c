/*
 *  actions_7c000.c
 *  GLUTBasics
 *
 *  Created by Ben on 31/12/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"


#include "structs.h"
#include "player.h"

#include "particle.h"
#include "lib.h"
#include "gfxlib.h"
#include "actions.h"
#include "actions_7c000.h"

extern struct game g;

static void sub_7cdf8(void);


static void sub_7cd3c(Object *obj) {
	switch (obj->mode2) {
		case 0:
			NEXT(obj->mode2);
			obj->LocalTimer = 8;
			obj->x001f = 2;
			sub_7cd84();
			sub_58c0(obj->UserData[0]);		/* byte */
			break;
		case 2:
			if (--obj->LocalTimer == 0) {
				obj->LocalTimer = 8;
				if (--obj->x001f < 0) {
					NEXT(obj->mode0);
				} else {
					sub_7cd84();
				}
			}
			break;
		FATALDEFAULT;
	}
}

static void sub_7c90a(Object *obj) {
	Player *ply;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			ud->x0082 = -1;			/* word */
			action_162c6();
			action_16340();
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					if(--obj->LocalTimer == 0) {
						action_162c6();
						action_16340();
					} else {
						ply=ud->x0088;
						if (ply->x02a5) {
							NEXT(obj->mode1);
							sub_7cdf8();
							action_1631e();
						}
					}
					break;
				case 2:
					if (g.Pause_9e1) {
						NEXT(obj->mode1);
					}
					break;
				case 4:
					sub_7cd3c(obj);
					break;
				default:
					break;
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


static void sub_7d2ae(Object *obj, Player *ply) {
	HitBoxAct *hb;
	if (g.PlayersThrowing || g.DebugNoCollide) {
		return;
	}
	if(hb=get_active_hitbox(obj)==0) { return; }
	if (ply->TimerInvincible) {
		return;
	}
	if (check_main_hitboxes(obj, ply, hb)==0) {
		return;
	}
	ply->TimerInvincible = 120;
	ply->DidCollide = TRUE;
	ply->NextReactMode2 = hb->ReactMode2;
	obj->Timer2 = 14;
	ply->Timer2 = 14;
	ply->Direction = obj->Flip;
	ply->Energy--;
	ply->x0071 = 0;
	ply->NextReactMode = RM_HITINAIR;
	queuesound(45);
	mac_stunhim_from76(obj, ply);
}


static void sub_7d284(Object *obj) {
	if (g.Player1.UserData[4] == 0 || g.Player2.UserData[4] == 0) {
		return;
	}
	if (g.Player1.exists ) {
		sub_7d2ae(obj, PLAYER1);
	}
	if (g.Player2.exists) {
		sub_7d2ae(obj, PLAYER2);
	}
}

static void sub_7dd38(Object *a6, Object *a2) {
	/* XXX access userdata in A2 */
	
}

static void sub_7dd0c(Object *obj) {
	sub_7d284(obj);
	g.x8aea = obj;
	/* XXX to be cont... */
}


static void sub_7cdf8(void) {
	Object *obj;
	if (obj=AllocActor()) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_0X31;
		obj->SubSel = 0;
	}
}

static void sub_7ce12(void) {
	Object *obj;
	if (obj =AllocActor()) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_0X31;
		obj->SubSel = 1;
	}
}
