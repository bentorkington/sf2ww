/*
 *  coll_bonus.c
 *  MT2
 *
 *  Created by Ben on 30/11/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */
#include "sf2types.h"
#include "sf2macros.h"
#include "sf2const.h"

#include "structs.h"
#include "player.h"

#include "lib.h"
#include "rules.h"
#include "particle.h"
#include "actions_198a.h"
#include "gfxlib.h"
#include "sound.h"


#include "playerstate.h"
#include "reactmode.h"
#include "actions.h"
#include "collision.h"
#include "projectiles.h"
extern struct game g;

#include "coll_bonus.h"

static void _CDBonus3(Object_G2 *a6) {			// 7d20a
	
	
}

static void _CDDecorSoundPts(Object *a6){		// 7e3b0
	soundsting((u16 []){
		0, 
		SOUND_CAR_BREAK,
		SOUND_BARRELS_BREAK,
		SOUND_BARRELS_BREAK,
		SOUND_BARRELS_BREAK,
		0, 0, 0,
		SOUND_BARRELS_BREAK,
		0, 0}[a6->Sel]);
	QueueEffect(0x8200, 0);		// Both players get points
	QueueEffect(0x8200, 1);		// XXX check the id!
}

static void _CDKillDecor(Player *a2, Object *a6) {	/* 7e39e */
	/* Kill the object, make a sound, reward points */
	a6->Energy     = -1;
	a6->Timer2     = 6;
	a2->ReactTimer = 6;
	_CDDecorSoundPts(a6);		// make smash sound, reward points.
}


static void mac_stun_oppopp(Player *ply, Player *opp) {		//7d93c checked
    Object *obj;
    if(obj = AllocActor()) {
        center_collision_coords();
        obj->exists = TRUE;
        obj->Sel    = SF2ACT_HITSTUN;     /* hitstuns */
        obj->XPI    = g.GPCollX;
        obj->YPI    = g.GPCollY;
        obj->Owner = opp->Opponent;  
        obj->Flip   = ply->Flip;       
        obj->SubSel = 2;
    }
}

static void _CDKillDecor2(Object *a6, Player *a2) {		// 7e424 checked
	mac_stun_oppopp((Player *)a6, a2);
	a6->exists=2;
	a6->Energy = -1;
	a6->Timer2 = 6;
	action_1286e((Object *)a6, a2->Side ^ 1);
	LBAddPoints(8,a2->Side ^ 1);
	_CDDecorSoundPts(a6);		/* Make sound, reward points */
}

static void mac_stunhim_from76(Object *obj1, Player *vict) {		//7d908
	/* 7d884 obj1 %a6 vict %a2 */
    Object *obj2;
    if(obj2 = AllocActor()) {
        center_collision_coords();
        obj2->exists = TRUE;
        obj2->Sel    = SF2ACT_HITSTUN;		
        obj2->XPI    = g.GPCollX;
        obj2->YPI    = g.GPCollY;
        obj2->Owner = vict;
        obj2->Flip   = obj1->Flip;      
        obj2->SubSel = vict->NextReactMode2;
		if (obj2->SubSel > 5) {
			panic(0);
		}
		
    }
}

static void sub_7d314(Object *a6, Player *a2) {
	const HitBoxAct *a3;
	/* 0x7d314  Object %a6, Player %a2, HitBoxAct %a3 */
	if (g.PlayersThrowing || g.DebugNoCollide) {
		return;
	}
	a3 = get_active_hitbox(a6);
	if (a3 == NULL) {
		return;
	}
	if (a2->TimerInvincible != 0) {
		return;
	}
	if (check_main_hitboxes(a6, (Object *)a2, a3)==0) {
		return;
	}
	a2->TimerInvincible = 120;
	a2->DidCollide = TRUE;
	a2->NextReactMode2 = a3->ReactMode2;
	a6->Timer2 = 14;
	a2->Timer2 = 14;
	a2->Direction = a6->Flip;
	--a2->Energy;
	a2->ProjectilePushBack = FALSE;
	set_newreact(a2, (Player *)a6, a3);
	soundsting(SOUND_IMPACT5);
	mac_stunhim_from76(a6, a2);
}



/* CDCheckDecor		@7e340
 Check for collision from a player, but only if falling or being thrown.
 Stage decorations such as Ryu's signs, Ken's drums call this detect when to break */

void CDCheckDecor (Object *a6) {		/* 7e340 check if collision with player */
	if(a6->exists & 0x01) {
		switch (a6->Sel) {
			case 1:
			case 2:
			case 3:
			case 7:
				/* 7e372 see if player fell onto barrels  / signs, etc. make sound */
				if(g.Player1.exists && g.Player1.Airborne < 0) {
					if(CDPushOverlap(PLAYER1, a6)) {		/* check for a collision */
						_CDKillDecor(PLAYER1, a6);		/* kill the object */
					}
				}
				if(g.Player2.exists && g.Player2.Airborne < 0) {
					if(CDPushOverlap(PLAYER2, a6)) {
						_CDKillDecor(PLAYER2, a6);
					}
				}
				break;
			case 9:
				/* 7e3ea */
				if (g.Player1.exists) {
					sub_7d314(a6, PLAYER1);
					if(g.Player1.Airborne == AIR_REEL2) {
						if(CDPushOverlap(PLAYER1, a6)) {
							_CDKillDecor2(a6, PLAYER1);
							return;
						}
					}
				}
				/* 7e406 */
				if (g.Player2.exists) {
					sub_7d314(a6, PLAYER2);
					if(g.Player1.Airborne == AIR_REEL2) {
						if(CDPushOverlap(PLAYER2, a6)) {
							_CDKillDecor2(a6, PLAYER2);
							return;
						}
					}
				}
				break;
			FATALDEFAULT;
		}
	}
}


#pragma mark BONUS0 - The car

static void _CDSplashBonus0(Object *obja2) {		// 7db9c
	Object *nobj;
	if (nobj = AllocActor()) {
		nobj->exists = TRUE;
		nobj->Sel = 0x32;
		if (obja2->Direction) {
			nobj->SubSel = 0;
		} else {
			nobj->SubSel = 1;
		}
	}
}
static void _CDSoundBonus0(Object *obja2) {						// 7dafc
	if (obja2->BlockStun) {
		queuesound(0x72);
	} else {
		queuesound(0x33);
	}
}

static void sub_7db12 (Object *obja6, Object_G2 *obja2) {
	const static short data_7db7e[] = {
		6, 6, 6, 8, 8, 8, 8, 8, 8, 10, 10, 34, 34, 34, 34
	};
	
	if (obja2->BlockStun == FALSE) {
		if (g.x8aec > 0) {
			if (g.GPWasProjectile) {
				LBAddPoints(2, ((Player *)obja6)->Side);
			} else {
				LBAddPoints(2, obja6->Owner->Side);
			}
			//todo sub_132fe(1);
		} else {
			if(++obja2->UD.UDcar.h0093c == 15) {
				obja2->UD.UDcar.h0093c = 14;
			}
			if (g.GPWasProjectile) {
				LBAddPoints(data_7db7e[obja2->UD.UDcar.h0093c], ((Player *)obja6)->Side);
			} else {
				LBAddPoints(data_7db7e[obja2->UD.UDcar.h0093c], obja6->Owner->Side);
			}
			//todo sub_132fe(0);
		}
		
	}
}
static void mac_stunme2(Player *ply, Player *opp) {		//7d824
	//todo: ply is actually a projectile
    Object *obj;
    if(obj = AllocActor()) {
        center_collision_coords();
        obj->exists = TRUE;
        obj->Sel    = SF2ACT_HITSTUN;     /* hitstuns */
        obj->XPI    = g.GPCollX;
        obj->YPI    = g.GPCollY;
        obj->Flip   = ply->Flip;       /*hmm */
        if(g.GPWasProjectile) {		/* projectile? */
            obj->Owner = ply->Owner;
            obj->SubSel = 3;
            if(opp->BlockStun) {
                set_hitstun_effect_for_projectile(ply, obj);
            }    
        } else {
            obj->Owner = ply;
            obj->SubSel = opp->NextReactMode2;
            if(opp->BlockStun) {
                obj->SubSel = 3;
            }    
			
        }
		if (obj->SubSel > 5) {
			panic(0);
		}
		
    }
}

static void _CDCheckObjBonus0(Player *plya6, Object_G2 *obja2, char *a1) {		// 7da44
	HitBoxAct *hba3;
	int d4, d0;
	
	if(hba3 = get_active_hitbox(plya6)) {
		if (hba3->Shove < 0) {
			d4 = 1;
		} else {
			d4 = hba3->Shove;
		}
		if (d4 == *a1) {
			return;
		}
		if (check_main_hitboxes(obja2,plya6 , hba3)) {
			*a1 = hba3->Shove;
			if (*a1 < 0) {
				*a1 = 1;
			}
			
			obja2->NextReactMode2 = hba3->ReactMode2;
			obja2->Timer2 = 14;
			plya6->Timer2 = 14;
			obja2->Direction = plya6->Flip;
			obja2->BlockStun = FALSE;  
			
			if (g.TimeRemainBCD != 1 && g.TimeOut == FALSE && obja2->UD.UDcar.boxes[g.GPHitBoxHit] < 0) {
				obja2->BlockStun = TRUE;
			} else {
				d0 = obja2->UD.UDcar.boxes[g.GPHitBoxHit];
				d0 -= (hba3->ReactMode2 + 1);
				if (d0 < 0) {
					d0 = -1;
				}
				obja2->UD.UDcar.boxes[g.GPHitBoxHit] = d0;
				g.x8aec = d0;
			}
			if (g.GPWasProjectile) {
				plya6->Energy = -2;
			}
			_CDSoundBonus0(obja2);
			mac_stunme2(plya6, obja2);
			_CDSplashBonus0(obja2);
			sub_7db12(plya6, obja2);
			return;
		}
	}
	*a1=0;
}
void _CDCheckBonus0(Player *plya6, Object_G2 *obja2) {		// 7d9f6
	char *a1;
	
	if (plya6->Side) {
		a1 = &obja2->UD.UDcar.h008cc;
	} else {
		a1 = &obja2->UD.UDcar.h008dc;
	}
	if (plya6->OnPlatform) {
		*a1 = 0;
	} else {
		g.GPWasProjectile = FALSE;
		_CDCheckObjBonus0(plya6, obja2, a1);
	}
	if (plya6->Side) {
		a1 = &obja2->UD.UDcar.h008ec;
	} else {
		a1 = &obja2->UD.UDcar.h008fc;
	}
	if (plya6->Projectile && plya6->Projectile->exists == 1) {
		g.GPWasProjectile = TRUE;
		_CDCheckObjBonus0(plya6, obja2, a1);
	} else {
		*a1 = 0;
	}
}


#pragma mark BONUS1
static void _CDSoundBonus1(Object *obja2, HitBoxAct *hba3) {		// 7dcba
	if (obja2->Energy < 0) {
		queuesound(0x32);
	} else {
		make_collision_sound((Player *)obja2, hba3);
	}
}
static void sub_7dccc(Player *ply, Object *a2) {	// 7dccc a6 a2 
	short x=(ply->Side);
	struct ud_s {
		u16 a;
		u16 b;
		u16 x0084;
	};
	struct ud_s *ud = (struct ud_s *)&a2->UserData;
	if(g.GPWasProjectile) {		// projectile? 
		x = (ply->Owner->Side);
	}
	if (ply->Energy < 0) {
		// 7dcf0
		if (ud->x0084 == 0) {
			ud->x0084 = 1;
			action_1286e((Object *)ply, x);
		}
		LBAddPoints(8, x);
	} else {
		LBAddPoints(2, x);
	}
	
}

void _CDCheckObjBonus1(Player *plya6, Object_G2 *obja2, char *a1) {		// 7dc2e
	const HitBoxAct *hb;
	const HitBox *hb2;
	int d4;
	
	if ((hb = get_active_hitbox((Object *)plya6)) == 0) {
		*a1 = 0;
	} else {
		d4 = hb->Shove;
		if (d4 < 0) {
			d4 = 1;
		}
		if (d4 == *a1) {
			return;
		}
		if (hb2 = CDGetHitBoxHead(obja2)) {
			if (check_hitbox_overlap((Object *)plya6, (Object *)obja2, hb, hb2)) {
				if (hb->Shove < 0) {
					*a1 = 1;
				} else {
					*a1 = hb->Shove;
				}
				obja2->NextReactMode2 = hb->ReactMode2;
				obja2->Timer2 = 14;
				plya6->Timer2 = 14;
				obja2->Direction = plya6->Flip;
				obja2->BlockStun = FALSE;
				obja2->Energy -= (hb->ReactMode2 + 1);
				if (obja2->Energy < 0) {
					obja2->Energy = -1;
					obja2->exists = 2;
				}
				if (g.GPWasProjectile) {
					obja2->Energy = -2;
				}
				_CDSoundBonus1((Object *)obja2, hb);
				mac_stunme2(plya6, (Object *)obja2);
				sub_7dccc(plya6, (Object *)obja2);
				return;
			}
		}
		*a1 = 0;
	}
}
void _CDCheckBonus1(Player *plya6, Object_G2 *obja2) {		// 7dbea
	char *a1;
	if (plya6->Side) {
		a1 = &obja2->UD.UDbonus1.h008cc;
	} else {
		a1 = &obja2->UD.UDbonus1.h008dc;
	}
	g.GPWasProjectile = FALSE;
	_CDCheckObjBonus1(plya6, obja2, a1);
	if (plya6->Side) {
		a1 = &obja2->UD.UDbonus1.h008ec;
	} else {
		a1 = &obja2->UD.UDbonus1.h008fc;
	}
	if (plya6->Projectile && plya6->Projectile->exists == 1) {
		g.GPWasProjectile = TRUE;
		_CDCheckObjBonus1(plya6, obja2, a1);
	} else {
		*a1 = 0;
	}
}
void _CDBonus1(Object_G2 *a6) {		// 7dbc2
	if (g.Player1.exists) {
		_CDCheckBonus1(&g.Player1, a6);
	}
	if (g.Player2.exists) {
		_CDCheckBonus1(&g.Player2, a6);
	}
}


#pragma mark BONUS2
static void _CDCheckObjBonus2(Object *obj, Player *ply) {			// 7d2ae
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
	ply->ProjectilePushBack = FALSE;
	ply->NextReactMode = RM_HITINAIR;
	queuesound(45);
	mac_stunhim_from76(obj, ply);
}
static void sub_7d284(Object_G2 *obj) {
	// XXX userdata;
	if (g.Player1.UserData[4] == 0 || g.Player2.UserData[4] == 0) {
		return;
	}
	if (g.Player1.exists ) {
		_CDCheckObjBonus2(obj, PLAYER1);
	}
	if (g.Player2.exists) {
		_CDCheckObjBonus2(obj, PLAYER2);
	}
}
static void sub_7de06(Object_G2 *a2) {
	if (g.TimeOut == FALSE) {
		if (a2->Energy < 0 && (a2->AnimFlags & 0xff) == 0) {
			//sub_221ea();		//Action44 todo
		} else {
			//sub_221bc();
		}
	}
}
static void sub_7de26(Player *a6, Object_G2 *a2) {
	int side;
	if (g.TimeOut == FALSE) {
		if (g.GPWasProjectile) {
			side = a6->Owner->Side;
		} else {
			side = a6->Side;
		}
		if (a2->Energy < 0) {
			LBAddPoints(8, side);
		} else {
			LBAddPoints(2, side);
		}
	}
}
static void sub_7dd7c(Player *a6, Object_G2 *a2, char *a1) {
	const HitBoxAct *a3 = get_active_hitbox((Object *)a6);
	if (a3) {
		int d4 = a3->Shove;
		if (d4 < 0) {
			d4 = 1;
		}
		if (a1[0] == d4) {
			return;
		}
		HitBox *a4 = CDGetHitBoxHead(a2);
		if (check_hitbox_overlap(a6, a2, a3, a4)) {
			a1[0]=a3->Shove;
			if (a1[0]<0) {
				a1[0]=1;
			}
			a2->NextReactMode2 = a3->ReactMode2;
			a6->Timer2 = 14;
			a2->Timer2 = 14;
			a2->Direction = a6->Flip;
			a2->BlockStun = 0;
			int d0 = a2->Energy - (a3->ReactMode2 + 1);
			if (d0 >= 0) {
				a2->Energy = d0;
			} else {
				a2->Energy = -1;
			}
			if (g.GPWasProjectile) {
				a6->Energy = -2;
			}
			make_collision_sound(a6, a3);
			mac_stun_from76(a6, a2);
			sub_7de06(a2);
			sub_7de26(a6, a2);
		} else {
			a1[0]=0;
			return;
		}
	} else {
		a1[0]=0;
	}

}
static void _CDCheckBonus2(Player *a6, Object_G2 *a2) {		// 7dd38
	char *a1;
	/* XXX access userdata in A2 */
	if (a6->Side) {
		a1 = &a2->UD.UDbonus2.h008dc;
	} else {
		a1 = &a2->UD.UDbonus2.h008cc;
	}
	g.GPWasProjectile = FALSE;
	sub_7dd7c(a6, a2, a1);
	if (a6->Side) {
		a1 = &a2->UD.UDbonus2.h008fc;
	} else {
		a1 = &a2->UD.UDbonus2.h008ec;
	}
	if (a6->Projectile) {
		if (a6->Projectile->exists == 1) {
			g.GPWasProjectile = TRUE;
			//todo sub_7dd7c(a6, a2, a1);
			return;
		}
	}
	*a1 = 0;
}

static void _CDBonus2(Object_G2 *a6) {		// 7dd0c
	sub_7d284(a6);
	if (g.Player1.exists) {
		_CDCheckBonus2(&g.Player1, a6);
	}
	if (g.Player2.exists) {
		_CDCheckBonus2(&g.Player2, a6);
	}
}
void CDBonusCollisionCheck(Object *a6) {		// 7dd9a - entry for Bonus objects
	// disable collision detection for most objects
	char data_7d9ba[]={-1, -1, -1, -1, 6, 4, 0, -1, -1, 2, -1, -1, -1, -1, -1, -1};
	
	// Subsel    BonusX
	//  4			3
	//  5			2
	//	6			0
	//  9           1
	
	if (a6->exists == 1 && data_7d9ba[a6->Sel] >= 0) {
		switch (data_7d9ba[a6->Sel]) {
			case 0:		// collision bonus obj 0
				//g.x8aea = a6; only temp?
				if (g.Player1.exists) {
					_CDCheckBonus0(&g.Player1, a6);
				}
				if (g.Player2.exists) {
					_CDCheckBonus0(&g.Player2, a6);
				}
				break;
			case 2:		// collision bonus obj 1
				_CDBonus1(a6);
				break;
			case 4:
				_CDBonus2(a6);
				break;
			case 6:
				_CDBonus3(a6);
				break;
				FATALDEFAULT;
		}
	}
}
