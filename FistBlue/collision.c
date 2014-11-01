/*
 *  collision.c
 *  GLUTBasics
 *
 *  Created by Ben on 20/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

#include <stdio.h>

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

/* globals */
short hitresult[5];
#define HITRES_D2	0
#define HITRES_D3	1
#define HITRES_D4	2
#define HITRES_D5	3
#define HITRES_D6	4


//HitBoxAct *g_hitbox;		/* %a3 */
//HitBoxAct *g_hitbox4;

short g_d5; // XXX

static FBBOOL sub_7e094(int d0, Object *a6, Player *vict, const HitBox *a3, const HitBox *a4);
static FBBOOL slib_check_overlap(Object *obj, Player *vict, const HitBox *a3, const HitBox *a4);

static void sub_7d74e(Player *ply, Player *opp, const HitBoxAct *a3);

static void set_blockstun_react(Player *vict);

static void _CDCheckPlayer(Player *ply, Player *vict);								/* 7cf38 */
static void _CDSpecialReactMode(Player *ply, Player *vict, const HitBoxAct *a3);	/* 7d02e */
static void _CDTurnToFaceMe(Player *ply, Player *vict);								/* 7d0a8 */


static const HitBox *_CDGetHitBoxBody(Object *obj);
static const HitBox *_CDGetHitBoxFoot(Object *obj);
static const HitBox *_CDGetHitBoxWeak(Object *obj);

static void mac_stun2005(Player *ply, Player *opp);
static void sub_7e314(void);
static void set_reel_react(Player *vict);
static void start_timewarp_action (Player *vict);

/* main entry to check collisions between players */
void CDCheckPlayers(void) {       /* 7cee0, called from fighttick. */
    debughook(6);
    if(g.PlayersThrowing) { return; }
    if(g.Player1.CompImmune) {g.Player1.CompImmune--;}
    if(g.Player2.CompImmune) {g.Player2.CompImmune--;}
	
    if(g.Player1.Human || g.DebugNoCollide==0) {
        _CDCheckPlayer(PLAYER1, PLAYER2);
    }
	
    if(g.Player2.Human || g.DebugNoCollide==0) {
        _CDCheckPlayer(PLAYER2, PLAYER1);
    }
}

void set_hitstun_effect_for_projectile(Object *projectile, Object *obj2) {		// 7d7fc
    switch(projectile->Sel) {		
		case SF2_PROJ_YOGAFIRE:
		case SF2_PROJ_YOGAFLAME:
		case SF2_PROJ_TIGER:
			obj2->SubSel = 5;
			break;
		default:
			obj2->SubSel = 4;
			break;
    }
}        

/* d6 = sub_7d378 (Player *a6, Player *a2)  calculate damage of hit to victim 
 and score reward                    */

int lookup_damage_and_score(Player *ply, Player *vict, const HitBoxAct *a3) { /* 7d378 */
	
	static char data_99324[16][32] = {
		{   4,   5,   5,   5,   5,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,   8,   8,   9,   9,   9,   9,   9,  10,  10,  10,  12,  },
		{   5,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,   8,   8,   9,   9,   9,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  14,  },
		{   5,   6,   6,   7,   7,   7,   8,   8,   8,   8,   9,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  18,  },
		{   7,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  11,  11,  12,  12,  12,  12,  13,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  20,  },
		{   8,   9,  10,  10,  11,  11,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,  15,  15,  16,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  23,  },
		{  10,  11,  12,  12,  13,  13,  14,  14,  14,  15,  15,  15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  24,  },
		{  10,  13,  14,  14,  15,  15,  16,  16,  16,  17,  17,  17,  17,  18,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  26,  },
		{  12,  15,  16,  16,  17,  17,  18,  18,  18,  19,  19,  19,  19,  20,  20,  20,  20,  21,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,  28,  },
		{  12,  13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  22,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,  27,  28,  29,  30,  31,  32,  34,  },
		{  15,  16,  17,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  24,  25,  25,  26,  26,  27,  27,  28,  28,  29,  30,  31,  32,  33,  34,  36,  },
		{  15,  16,  17,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  24,  25,  25,  26,  26,  27,  27,  28,  28,  29,  30,  31,  32,  33,  34,  36,  },
		{  17,  18,  19,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,  26,  27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  33,  34,  35,  36,  38,  },
		{  18,  19,  20,  21,  22,  23,  23,  24,  24,  25,  25,  26,  26,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  32,  32,  33,  34,  35,  36,  37,  38,  40,  },
		{  22,  23,  24,  25,  26,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  32,  32,  33,  33,  34,  34,  35,  35,  36,  36,  37,  38,  39,  40,  41,  42,  44,  },
		{  26,  27,  28,  29,  30,  31,  31,  32,  32,  33,  33,  34,  34,  35,  35,  36,  36,  37,  37,  38,  38,  39,  39,  40,  40,  41,  42,  43,  44,  45,  46,  48,  },
		{  30,  31,  32,  33,  34,  35,  35,  36,  36,  37,  37,  38,  38,  39,  39,  40,  40,  41,  41,  42,  42,  43,  43,  44,  44,  45,  46,  47,  48,  49,  50,  52,  },
	};
	
	
	static const u16 data_99544[16]={
		0x2002, 0x2002, 0x2002, 0x2004, 0x2004, 0x201c, 0x201e, 0x201e, 
		0x2006, 0x2006, 0x2006, 0x2006, 0x2006, 0x2008, 0x2008, 0x2020,  
	};
	static const u16 data_99566[4][8] = {
		{ 0x0001, 0x0014, 0x000a, 0x003c, 0x001e, 0x0000, 0x0000, 0x0000,  },
		{ 0x0001, 0x001a, 0x000d, 0x0050, 0x0028, 0x0000, 0x0000, 0x0000,  },
		{ 0x0001, 0x001e, 0x000f, 0x0064, 0x0032, 0x0000, 0x0000, 0x0000,  },
		{ 0x0002, 0x0028, 0x001e, 0x0064, 0x004b, 0x0000, 0x0000, 0x0000,  },
	};
	/* next address 000995a6 */
	
	static const u16 data_995a6[4]={0x200a, 0x200a, 0x2022, 0x2022,  };
	
	
    g_d5=vict->MoreBoxes[vict->Difficulty];
    if((a3->Damage & 0x7f) < 0x20) {
        g.GPPointsReward = data_99544[ a3->Damage ];
        return data_99324[ a3->Damage ][ ply->Difficulty ];
    } 
	g.GPPointsReward = data_995a6[ a3->Damage - 0x20 ];
	
	if(ply->Energy < data_99566[ a3->Damage - 0x20 ][ 1 ]) {
		return data_99566[ a3->Damage - 0x20 ][2];
	}
	if(ply->Energy >= data_99566[ a3->Damage - 0x20 ][ 3 ]) {
		return data_99566[ a3->Damage - 0x20 ][4];
	}
	return (vict->Energy - (vict->Energy >> data_99566[ a3->Damage - 0x20 ][0]));
}
int damage_multiplier(Player *vict, int d6) {   /* 7d40e */
    int xx;
    if(g.GPHitBox4Hit) {
        g.Diff_WeakBoxCnt++;
        xx=vict->ActionScript->YOffset & 0xff; /* XXX variable sharing nastiness workaround */
    } else {
        xx=vict->HitBoxUnused[g.GPHitBoxHit / 2];		// unused, always 0
    }
    switch (xx) {
		case 2:
			d6 += d6/2;
			break;
		case 4:
			d6 *= 2;
			break;
		case 6:
			d6 *= 3;
			break;
		case 8:
			d6 += d6 / 4;
			break;
	}
    return d6;
}
short randomize_damage(Player *ply,  int damage, const HitBoxAct *a3) {     /* 7d476 */
    int d2;
	static const signed char data_93300[9][32] = {		// XXX redo as signed char
		{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  },
		{   1,   2,   3,   1,   2,   1,   2,   1,   1,   1,   1,   2,   1,   1,   1,   1,   2,   1,   3,   1,   3,   1,   2,   3,   1,   1,   2,   1,   1,   2,   1,   1,  },
		{   2,   1,   1,   1,   1,   1,   1,   1,   1,   3,   1,   4,   2,   2,   4,   2,   2,   1,   2,   1,   6,   1,   5,   1,   1,   1,   1,   3,   1,   1,   1,   1,  },
		{   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  15,   0,   0,   0,  },
		{   0, 246,   0,   0,   0,   0,   0,   0,   0,   0,  10,   0,   0,   0,  10,   0,   0,   0,   0,   0, 241,   0,   0,   0,   0,  15,   0,   0,   0,   0,   0, 246,  },
		{   0,  10, 241, 246,  10,   0, 241,  15,  15,  15,   0, 241,  15, 241,  10,  15, 241,   0, 241,  10, 246,  15,  15, 246, 246,  15, 241,  15, 241,  15, 241, 241,  },
		{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  },
		{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  },
		{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  },
	};
	/* next address 00093420 */
	
	
    if (ply->Energy >= 0x60) {
        d2=a3->Random1;
    } else {
        d2=a3->Random2;
    }
    
    damage += data_93300[d2][RAND32];
    if(damage <= 0) { damage = 1; }
    
    g_d5 *= damage;        /* MULU instruction! */
    g_d5 = g_d5 >> 5;
    damage -= g_d5;
    if(damage == 0) {damage = 1;}
    return damage;
}
short diminishing_damage(int damage, Player *vict) {  /* 7d4b6 */
	static const char data_93420[31]={
		24,  24,  24,  24,  24,  24,  20,  20,  20,  20,  20,  16,  16,  16,  16,  16, 
		12,  12,  12,  12,  12,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4, 
	};
	
    if(vict->Energy >= 0x1f) { return damage; }
    damage -= (damage * data_93420[vict->Energy]) >> 5;
    if (damage==0) {damage = 1; }
    return damage;
}

static int sub_7d594(short d0, Player *vict) {		// 7d594
	vict->BlockStun = TRUE;
	if(d0 >= 0) {
		bump_difficulty_4816();
		return TRUE;
	}
	g.GPHitBlocked = TRUE;
	return FALSE;
}

int sub_7d4fa(Player *vict_a2, const HitBoxAct *a3) {		// 7d4fa
	static const signed char data_7d5b0[3][16]={
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0, -1, -1,  0,  0 },
		{  1,  1,  1,  0,  1,  1,  1,  0,  0,  0,  0,  0, -1, -1, -1,  0 }
	};
	
	short temp;
    if (vict_a2->Human							&& 
		vict_a2->Airborne == AIR_ONGROUND		&&
        vict_a2->mode2 != PLSTAT_REEL			&&
        vict_a2->mode2 != PLSTAT_TUMBLE			&&
        vict_a2->BlockStun == FALSE				&&
        vict_a2->Energy >= 0					&&
        vict_a2->Energy == vict_a2->EnergyDash	&&
        vict_a2->DizzyStun == FALSE
		) {
        if(LBRareChance()) {                
            vict_a2->BlockStun  = TRUE;     /* 7d4da */
            g.GPHitBlocked		= FALSE;
            if(a3->Attack != 3) {
                return TRUE;
            }
            g.GPHitBlocked = TRUE;
            return FALSE;
        }
    } 
	/* 7d534 */
    g.GPHitBlocked = FALSE;
    temp = vict_a2->ActionScript->Block;
    
    if(a3->Attack != 3 || a3->ReactMode != RM_0x26) {
		/* 7d55c */
		
        if(a3->Attack == 4 && a3->ReactMode == RM_0x24) {
            temp=2;
        }
        temp = ((temp & 0x3) << 2) + (g.GPHitBoxHit / 2);
		
        temp =  data_7d5b0[vict_a2->ActionScript->Block / 2][temp];
        if (temp) {
            return sub_7d594(temp, vict_a2);
        } else {
            return FALSE;
        }
    } else {
        if(temp == 1) {
            return sub_7d594(-1, vict_a2);
        } else {
			return sub_7d594(temp, vict_a2);
		}
    }
}
void center_collision_coords(void) {		//7d974 checked
    g.GPCollX = g.VictimLeftEdge 
	    + ((g.GPLeftEdge - g.VictimLeftEdge) / 2);
    g.GPCollY = g.VictimTopEdge  
	    + ((g.GPTopEdge  - g.VictimTopEdge ) / 2);
}

static void sub_7d74e(Player *ply, Player *opp, const HitBoxAct *a3) {
    Object *obj;
    if((obj=AllocActor())) {
        center_collision_coords();
        obj->exists = TRUE;
        obj->Sel    = SF2ACT_HITSTUN;     /* hitstuns */
        obj->XPI    = g.GPCollX;
        obj->YPI    = g.GPCollY;
        obj->Owner  = ply;
        obj->Flip   = ply->Flip;
        obj->SubSel = opp->NextReactMode2;
        if(g.GPHitBlocked == 0 && opp->BlockStun) {
            obj->SubSel = 3;
            return;
        }
		/* patch for chun li problems */
		if (obj->SubSel > 5) {				
			// XXX
			obj->SubSel = 3;
			printf("Bad ChunLi NextReactMode2");
		}
    } else {
        return;
    }
	
    if(ply->FighterID == FID_VEGA && a3->Damage > 128) {
		if((obj=AllocActor())) {
			obj->exists = TRUE;				// some blood from Vega's claw
			obj->Sel    = SF2ACT_VOMIT; 
			obj->SubSel = 0x8;
			obj->XPI    = g.GPCollX;
			obj->YPI    = g.GPCollY;        
			obj->Owner = ply;
			obj->Flip   = ply->Flip;
		}
    }
}

void mac_stun_from76(Player *ply, Player *opp) {			//7d884
    Object *obj;
    if((obj = AllocActor())) {
        center_collision_coords();
        obj->exists = TRUE;
        obj->Sel    = SF2ACT_HITSTUN;     /* hitstuns */
        obj->XPI    = g.GPCollX;
        obj->YPI    = g.GPCollY;
        obj->Flip   = ply->Flip;       /*hmm */
        if(g.GPWasProjectile) {		/* projectile ? */
            obj->Owner = ply->Owner;
            obj->SubSel = 3;    
        } else {
            obj->Owner = ply;
            obj->SubSel = opp->NextReactMode2;
        }
		if (obj->SubSel > 5) {
			panic(0);
		}
		
    }
}
static void mac_stun2005(Player *ply, Player *opp) {		//7d8d4
    Object *obj;
    if((obj = AllocActor())) {
        center_collision_coords();
        obj->exists = TRUE;
        obj->Sel    = SF2ACT_HITSTUN;     /* hitstuns */
        obj->XPI    = g.GPCollX;
        obj->YPI    = g.GPCollY;
        obj->Owner = opp;
        obj->Flip   = ply->Flip;       /*hmm */
        obj->SubSel = 5;
    }
}


/*
 Function RectsOverlap:Int(x0, y0, w0, h0, x2, y2, w2, h2)
 If x0 > (x2 + w2) Or (x0 + w0) < x2 Then Return False
 If y0 > (y2 + h2) Or (y0 + h0) < y2 Then Return False
 Return True
 End Function
 */

#pragma mark ---- HitBox Overlap Checking ----

/* hitbox overlap checker 
 *      int dunno      %d0
 *      HitBox *hit    %a3
 *      HitBox *target %a4
 *      Object *obj    %a6
 *      Object *vict   %a2
 *   R  res[]          %d3, %d4, %d5, %d6  XX may include %d2
 */

static FBBOOL sub_7e094(int shove_x, Object *a6, Player *vict, const HitBox *a3, const HitBox *a4) {
    short d1,d2,d3,d4,d5,d6;
    if(a6->Flip)
		shove_x = -shove_x;
	
    shove_x += a6->XPI;
    g.GPLeftEdge = shove_x;			/* x0 + w0 */
	
	// %d2:
	d2 = g.VictimLeftEdge = vict->XPI + (vict->Flip ? -a4->X : a4->X) ;
								/* d2 is x2 */
	
	d4 = a3->width+a4->width;		/* d4 = w1+w2  */
	d5 = shove_x - d2;				/* d5 = (x0+w0)-x2 */
	shove_x = d5 + d4;					/* d0 = (x0+w0)-x2+w0+w2 */
	d6 = d4;

	if (shove_x > (d4 * 2) || shove_x < 0) {
		hitresult[HITRES_D4]=FALSE;
		return FALSE;
	}
	if (d5>=0) {
		d6=-d6;
	}
	hitresult[HITRES_D2] = d5 + d6;
	// 7e0e0
	g.GPTopEdge = d1 = a3->Y + a6->YPI;
	d3 = a4->Y + vict->YPI;
	g.VictimTopEdge = d3;
	// 7e0fc
	d4 = a3->height+a4->height;
	d1 -= d3;
	d5 = d1; 
	d1 += d4;
	d6 = d1;
	if (d1 > (d4 * 2) || d1 < 0) {
		hitresult[HITRES_D4]=FALSE;
		return FALSE;
	}
	if (d5 < 0) {
		d6 = -d6;
	}
	hitresult[HITRES_D3] = d5 + d6;
	hitresult[HITRES_D4] = TRUE;
	return TRUE;
}

/*check_hitbox_overlap()        0x7e082 
 *      HitBox *hit    %a3
 *      HitBox *target %a4
 *      Object *obj    %a6
 *      Object *vict   %a2
 *   R  res[]          %d3, %d4, %d5, %d6
 */


FBBOOL check_hitbox_overlap (Object *obj, Object *vict, const HitBoxAct *active, const HitBox *a4) {       /* 0x7e082 */
	
    if (active->Shove >= 0) {
		return slib_check_overlap(obj,(Player *) vict, (const HitBox *)active, a4);
    } else {
        return sub_7e094(-active->Shove, obj,(Player *) vict, (const HitBox *)active, a4);
    }
}

/* slib_check_overlap()   0x7e08e
 *      HitBox *hit    %a3
 *      HitBox *target %a4
 *      Object *obj    %a6
 *      Object *vict   %a2
 *   R  res[]          %d3, %d4, %d5, %d6
 */

static FBBOOL slib_check_overlap(Object *obj, Player *vict, const HitBox *a3, const HitBox *a4) {		/* 7e08e */
	return sub_7e094(a3->X, obj, vict, (HitBox *)a3, a4);
}


#pragma mark HitBox Lookup
/* used to be lookup_hitbox_8,9,10 etc. */
/* lookup for victim in %a2 */
const HitBox *CDGetHitBoxHead (Object *obj) {			/* 7e01c */
    if (obj->ActionScript->HB_Head) {
		return obj->HitBoxes->head+obj->ActionScript->HB_Head;
    } else {
        return NULL;
    }
}
static const HitBox *_CDGetHitBoxBody (Object *obj) {
    if (obj->ActionScript->HB_Body) {
		return obj->HitBoxes->body+obj->ActionScript->HB_Body;
    } else {
        return NULL;
    }
}
static const HitBox *_CDGetHitBoxFoot (Object *obj) {
    if (obj->ActionScript->HB_Foot) {
		return obj->HitBoxes->foot+obj->ActionScript->HB_Foot;
    } else {
        return NULL;
    }
}
static const HitBox *_CDGetHitBoxWeak (Object *obj) {
    if (obj->ActionScript->HB_Weak) {
		return obj->HitBoxes->weak+obj->ActionScript->HB_Weak;
    } else {
        return NULL;
    }
}
const HitBox *CDGetPushBox (Object *obj) {
    if (obj->ActionScript->HB_Push) {
		return obj->HitBoxes->push+obj->ActionScript->HB_Push;
    } else {
        return NULL;
    }
}
const HitBoxAct *get_active_hitbox(Object *obj) {      /* 7dff2 */
	if (obj->ActionScript == NULL) { return NULL; } //added to prevet crashes, CPS
                                                    //dereferences NULL here!
	
    if(obj->ActionScript->HB_Active) {
        return (HitBoxAct *)obj->HitBoxes->active + obj->ActionScript->HB_Active;	
    } else {
        return NULL;
    }
}
	

static void sub_7e2a0(int d1, int d2, int d6) {
	if (d1==1) {
		d2 = -ABS(d2);
	} else {
		d2 = ABS(d2);
	}
	// this is sub_7e2f8(d2), inlined;
	g.Player2.XPI += d2;
	PLAYER_PUSH(d2 >= 0);
	// 7e31c 
	g.Player1.XPI -= d2;
	PLAYER_PUSH(d6 >= 0);
	
}


// sub_7e2cc is the same as 7e2a0, but with a different reg, %d0
static void sub_7e2b6(int d1, int d2, int d6) {
	if (d1==1) {
		d2 = ABS(d2);
	} else {
		d2 = -ABS(d2);
	}
	//sub_7e2f8(d2);
	g.Player2.XPI += d2;
	PLAYER_PUSH(d2 >= 0);
	// 7e31c 
	g.Player1.XPI -= d2;
	PLAYER_PUSH(d6 >= 0);
	
}
static void sub_7e2e2(int d0, int d2, int d6) {
	if (d0 != 1) {
		d2=(ABS(d2));
	}
	if (d2 > 0) {
		d2=-d2;
	}
	// 7e31c 
	g.Player1.XPI -= d2;
	PLAYER_PUSH(d6 >= 0);
}
static void sub_7e274(int d0, int d2, int d6) {
	int d1;
	d1 = g.Player1.BoundCheck;
	if (d2 == 0) {
		if (g.Player1.PlatformFallDir == 0) {
			sub_7e2e2(d0,d2,d6);
			return;
		}
		d1 = g.Player1.PlatformFallDir;
	}
	if (d0 != d1) {
		return;
	}
	
	// 7e296 inlined
	if(g.Player1.YPI >= g.Player2.YPI) {
		sub_7e2a0(d1, d2, d6);
	} else {
		sub_7e2b6(d1, d2, d6);
	}
}
static void sub_7e286(int d0, int d2, int d6) {
	int d1;
	d1 = g.Player2.BoundCheck;
	if (d2 == 0) {
		if (g.Player2.PlatformFallDir == 0) {
			sub_7e2b6(d0,d2, d6);
			return;
		}
		d1 = g.Player2.PlatformFallDir;
	}
	if (d0 != d1) {
		return;
	}
	
	if(g.Player1.YPI >= g.Player2.YPI) {
		sub_7e2a0(d1, d2, d6);
	} else {
		sub_7e2b6(d1, d2, d6);
	}
}
static void sub_7e23c(int d2) {
	g.Player2.XPI += d2/2;
	g.Player1.XPI -= d2/2;
	PLAYER_PUSH(d2>=0);
}
static void sub_7e19a(int d6) {		/* resolve a ground collision between player */
	/* checked */
	short d0,d2,d3,d4;
	/* 0x7e19a */
	
	d4 = d6;
	d6=ABS(d6);
	
	d0 = 0;
	if (d6 & 0x1) {
		d0=1;
	}
	d6 >>= 1;
	if (d4 < 0) {
		d0 = -d0;
		d6 = -d6;
	}
	
	d2 = ABS(g.Player1.DeltaX.part.integer);
	d3 = ABS(g.Player2.DeltaX.part.integer);
	if (d2 <= d3) { /* 7e1c0 */
		g.Player2.XPI += d6;
		g.Player2.XPI += d0;
		g.Player1.XPI -= d6;
	} else {
		g.Player1.XPI -= d6;
		g.Player1.XPI -= d0;
		g.Player2.XPI += d6;
	}
	PLAYER_PUSH(d6<0);
}
static void sub_7e264(int d2, int d6) {
	if (g.Player1.VelX.full == 0) {
		sub_7e23c(d2);
	} else if (g.Player2.VelX.full == 0) {
		sub_7e23c(d2);
	} else {
		sub_7e19a(d6);
	}
}
static void sub_7e218 (int d2, int d6) {
	/* 7e218 resolve an in-air collision between player */
	
	int d0;
	
	if (g.Player1.BoundCheck) {
		sub_7e274(g.Player1.BoundCheck,d2,d6);
	} else if (g.Player1.PlatformFallDir) {
		sub_7e274(g.Player1.PlatformFallDir,d2,d6);
	} else if (g.Player2.BoundCheck) {
		sub_7e286(g.Player2.BoundCheck,d2,d6);
	} else if (g.Player2.PlatformFallDir) {
		sub_7e286(g.Player2.PlatformFallDir,d2,d6);
	} else {
		d0 = g.Player1.Airborne & g.Player2.Airborne;
		if (d0) {
			sub_7e264(d2, d6);
		} else {
			sub_7e23c(d2);
		}
	}
}
int check_main_hitboxes(Object *obj, Object *vict, const HitBoxAct *a3) {         /* 0x7de5a */
    const HitBox *a4;
	
    g.GPReg1  = 0;
    g.GPHitBox4Hit = 0;
	
    if ((a4=CDGetHitBoxHead(vict))) {
        if(check_hitbox_overlap(obj, vict, a3, a4)) {
            g.GPReg1 |= 1;
			g.GPHitBoxCoords[0][0]=g.VictimLeftEdge;
			g.GPHitBoxCoords[0][1]=g.VictimTopEdge;
            g.GPHitBoxDamage[0] = ABS(hitresult[HITRES_D3]);
        }
    }
    if ((a4=_CDGetHitBoxBody(vict))) {
        if(check_hitbox_overlap(obj, vict, a3, a4)) {
            g.GPReg1 |= 2;
			g.GPHitBoxCoords[1][0]=g.VictimLeftEdge;
			g.GPHitBoxCoords[1][1]=g.VictimTopEdge;
            g.GPHitBoxDamage[1] = ABS(hitresult[HITRES_D3]);
        }
    }
    if ((a4=_CDGetHitBoxFoot(vict))) {
        if(check_hitbox_overlap(obj, vict, a3, a4)) {
            g.GPReg1 |= 4;
			g.GPHitBoxCoords[2][0]=g.VictimLeftEdge;
			g.GPHitBoxCoords[2][1]=g.VictimTopEdge;
            g.GPHitBoxDamage[2] = ABS(hitresult[HITRES_D3]);
        }
    }
    if (g.GPReg1 == 0) { return FALSE; }
    switch (g.GPReg1){
		case 0:
			/* can't happen, special-cased out */
			return 0;
			break;
		case 1:
			/* Hitbox 1 only */
			g.GPHitBoxHit = 0;
			g.VictimLeftEdge = g.GPHitBoxCoords[0][0];
			g.VictimTopEdge = g.GPHitBoxCoords[0][1];
			break;
		case 2:
			/* Hitbox 2 only */
			g.GPHitBoxHit = 2;
			g.VictimLeftEdge = g.GPHitBoxCoords[1][0];
			g.VictimTopEdge = g.GPHitBoxCoords[1][1];
			break;
		case 3:
			/* Hitbox 1 and 2 */
			if (g.GPHitBoxDamage[0] > g.GPHitBoxDamage[1]) {
				g.GPHitBoxHit = 0;
				g.VictimLeftEdge = g.GPHitBoxCoords[0][0];
				g.VictimTopEdge = g.GPHitBoxCoords[0][1];
			} else {
				g.GPHitBoxHit = 2;
				g.VictimLeftEdge = g.GPHitBoxCoords[1][0];
				g.VictimTopEdge = g.GPHitBoxCoords[1][1];
			}
			break;
		case 4:
			/* Hitbox 3 only */            
			g.GPHitBoxHit = 4;
			g.VictimLeftEdge = g.GPHitBoxCoords[2][0];
			g.VictimTopEdge = g.GPHitBoxCoords[2][1];
			break;
		case 5:
			/* Hitbox 3 and 1 */
			if (g.GPHitBoxDamage[0] > g.GPHitBoxDamage[2]) {
				g.GPHitBoxHit = 0;
				g.VictimLeftEdge = g.GPHitBoxCoords[0][0];
				g.VictimTopEdge = g.GPHitBoxCoords[0][1];
			} else {
				g.GPHitBoxHit = 4;
				g.VictimLeftEdge = g.GPHitBoxCoords[2][0];
				g.VictimTopEdge = g.GPHitBoxCoords[2][1];
			}
			break;
		case 6:
			/* Hitbox 3 and 2 */
			if (g.GPHitBoxDamage[1] > g.GPHitBoxDamage[2]) {
				g.GPHitBoxHit = 2;
				g.VictimLeftEdge = g.GPHitBoxCoords[1][0];
				g.VictimTopEdge = g.GPHitBoxCoords[1][1];
			} else {
				g.GPHitBoxHit = 4;
				g.VictimLeftEdge = g.GPHitBoxCoords[2][0];
				g.VictimTopEdge = g.GPHitBoxCoords[2][1];
			}
			break;
		case 7:
			/* All three */
			if (g.GPHitBoxDamage[0] > g.GPHitBoxDamage[1]) {
				if (g.GPHitBoxDamage[0] > g.GPHitBoxDamage[2]) {
					g.GPHitBoxHit = 0;
					g.VictimLeftEdge = g.GPHitBoxCoords[0][0];
					g.VictimTopEdge = g.GPHitBoxCoords[0][1];
				} else {
					g.GPHitBoxHit = 4;
					g.VictimLeftEdge = g.GPHitBoxCoords[2][0];
					g.VictimTopEdge = g.GPHitBoxCoords[2][1];
				}
			} else {
				if (g.GPHitBoxDamage[2] > g.GPHitBoxDamage[1]) {
					g.GPHitBoxHit = 4;
					g.VictimLeftEdge = g.GPHitBoxCoords[2][0];
					g.VictimTopEdge = g.GPHitBoxCoords[2][1];
				} else {
					g.GPHitBoxHit = 2;
					g.VictimLeftEdge = g.GPHitBoxCoords[1][0];
					g.VictimTopEdge = g.GPHitBoxCoords[1][1];
				}
			}
			break;
	}
	
	if((a4=_CDGetHitBoxWeak(vict))) {
		if(check_hitbox_overlap(obj, vict,a3,a4)) {
			g.GPHitBox4Hit = TRUE;
		}
	}
	return TRUE;
}


#pragma mark ---- ReactMode Setting ----

static void set_special_reactmode(Player *vict, Player *ply, const HitBoxAct *hbact) {	//7d64a checked
	vict->NextReactMode = hbact->ReactMode;     /* 0x7d64a */
	if (hbact->Attack == ATTACK_PRIORITY) {		
		if(vict->NextReactMode == RM_0x24) {		// 7d6a6
													/* 7d6bc() */
			ply->UserData[0x10] = -1;				/* XXX userdata */
			if(vict->Airborne != AIR_ONGROUND) {
				ply->NextReactMode = RM_HITINAIR;
			} else if(vict->BlockStun) {
				set_blockstun_react(vict);
			} else {
				/* 7d6d0 */
				ply->UserData[0x10] = 1;
				vict->NextReactMode = RM_0x0c;
				if(vict->Energy < 0) {
					ply->UserData[0x10] = -1;
				}
				set_reel_react(vict);   
			}
		} else {
			if(vict->Airborne) {    
				ply->NextReactMode = RM_HITINAIR;
			} else if (vict->BlockStun) {
				set_blockstun_react(vict);
			}
		}
	} else if (vict->NextReactMode == RM_0x26) {
		if (vict->Airborne != AIR_ONGROUND) {		// 7d692
			vict->NextReactMode = RM_FIREBALLDOWN;
		} else if (vict->BlockStun == FALSE) {
			set_reel_react(vict);
		} else {
			vict->NextReactMode = RM_0x06;
		}
	} else if ((vict->NextReactMode == RM_HURRICANE || 
			    vict->NextReactMode == RM_0x14 ||
			    vict->NextReactMode == RM_MULTIHIT 
			   ) && vict->Airborne) {
		vict->NextReactMode = RM_FIREBALLDOWN;
	} else {
		vict->IsBlockingSuper = TRUE;            /* 7d684 */
		if(vict->BlockStun) {
			set_blockstun_react(vict);
		}
	}
}	
void set_newreact(Player *vict, Player *ply, const HitBoxAct *hbact) {     /* checked 0x7d5e6 public */   
    vict->IsBlockingSuper = FALSE;
    vict->NextReactMode   = g.GPHitBoxHit;
	
    if (hbact->Attack >= ATTACK_SPECIAL) {
		set_special_reactmode(vict, ply, hbact);
	} else if (vict->Airborne) {
        vict->NextReactMode = RM_HITINAIR;
    } else if (vict->BlockStun) {
        set_blockstun_react(vict);
	} else {
		switch (hbact->Attack) {
			case ATTACK_STAND:
				if(vict->ActionScript->Crouch) {
					vict->NextReactMode = RM_0x0a;
				}
				break;
			case ATTACK_SWEEP:
				vict->NextReactMode = RM_FOOTSWEPT;
				break;
			case ATTACK_JUMP:
				set_reel_react(vict);
				break;
			FATALDEFAULT;
		}
	}
}
static void set_reel_react(Player *vict) {    // 7d634 checked
    if(vict->ActionScript->Crouch) {   /* standing / crouching */
		vict->NextReactMode = RM_0x0a;
    } else {
		vict->NextReactMode = RM_0x0c;
    }
}
static void set_blockstun_react(Player *vict) {		/* 7d6ec  checked */
    if (vict->ActionScript->Crouch) {
        vict->NextReactMode = RM_0x08;
    } else {
        vict->NextReactMode = RM_0x06;
    }    
}
void make_collision_sound(Player *ply, const HitBoxAct *a3) {         /* 7d700 checked */
    if(ply->BlockStun || g.GPHitBlocked) {
		queuesound(SOUND_HIT_BLOCKED);  /* blocked hit thud */
    } else {
        queuesound(a3->Sound);
    }
}




void CDCheckPushBoxes () {			/* 7e136 check pushboxes, take action */
    if(g.BattleOver) {return;}
    g.GPCollDetect = FALSE;
	const HitBox *a3,*a4;
	
    if(g.Player1.exists == FALSE) { return; }
	
    if((a3=CDGetPushBox((Object *)PLAYER1))) {
		if (g.Player2.exists) {
			if((a4=CDGetPushBox((Object*)PLAYER2))) {
				if(slib_check_overlap((Object *)PLAYER1, PLAYER2,a3,a4)) {
					// 7e16e
					if (hitresult[HITRES_D2]) {
						g.GPCollDetect = TRUE;
					}
					int d6 = hitresult[HITRES_D2];
					if (hitresult[HITRES_D2] < -8) {
						hitresult[HITRES_D2] = -8;
					}
					
					if(g.Player1.Airborne || g.Player2.Airborne) {
						sub_7e218(hitresult[HITRES_D2],d6);
					} else {
						sub_7e19a(d6);
					}
				}
			}			
		}
    }
}
void slib_ply_overlap() {		// 7e12a
	// CPS todo
	/* wrapper function, in game saves A6 to g.8af2 */
	CDCheckPushBoxes();
}
	
	
static void _CDCheckPlayer(Player *ply, Player *vict) {     /* 7cf38 */
    int temp;
	short damage;
	
	const HitBoxAct *active;		// %a3
    
    if(!(active=get_active_hitbox((Object *)ply))) {
        vict->SufferHB5 = 0;
        return;
    }
    if (vict->CompImmune || vict->TCollDis || vict->Invincible) { return; }
    temp = active->Shove;
    if(temp < 0) { temp = 1; }
    if(temp == vict->SufferHB5) { return; }
    if((active->ReactMode == RM_MULTIHIT ||  active->ReactMode == RM_ELECTROCUTED) 
	   && vict->MultiHoldoff ) {
        return;
    }
    if(!check_main_hitboxes((Object *)ply, (Object *)vict, active))   { return; }
    if(active->ReactMode == RM_MULTIHIT	|| 
	   active->ReactMode == RM_ELECTROCUTED) {
        vict->MultiHoldoff = 18;     
    }
    if(vict->Human == 0) {
        g.x0ae4++;
    }
    vict->SufferHB5 = active->Shove;
    if(vict->SufferHB5 < 0) {
        vict->SufferHB5 = 1;
    }
    vict->NextReactMode2 = active->ReactMode2;
    ply->Timer2  = 14;
    vict->Timer2 = 14;
    _CDSpecialReactMode(ply, vict, active);
	
    damage = lookup_damage_and_score(ply, vict, active);
    if(sub_7d4fa(vict, active) == 0) {
        damage=damage_multiplier(vict, damage);
        damage=randomize_damage(vict, damage, active);
        damage=diminishing_damage(damage, vict);
        if(g.GPHitBlocked) {
            damage /= 4;
            if (damage == 0) { damage = 1; }
        }
        vict->Energy -= damage;
        if(vict->Energy < 0) {
            vict->Energy    = -1;
			start_timewarp_action(vict);
            g.GPHitBlocked    = FALSE;
            vict->BlockStun   = FALSE; 
        }  
    }
    vict->ProjectilePushBack = FALSE;						//7d000
    set_newreact(vict, ply, active);       
    make_collision_sound(vict, active);
    sub_7d74e(ply, vict, active);		     /* splash, see if vega loses claw */
    if(vict->BlockStun || g.OnBonusStage) { return; }
    QueueEffect(g.GPPointsReward, ply->Side);                    /* award points */
}
short CDPushOverlap(Player *a2, Object *a6) {	
	/* 7e460 test if player %a2 push hit object %a6 push */
	const HitBox *hb;
	const HitBox *plyhb;
	short d4;
	
	if (a2->ActionScript->HB_Push == 0) {return 0;} 
	hb = a2->HitBoxes->push + a2->ActionScript->HB_Push;  /* XXX use globals */
	if (a6->ActionScript->HB_Push == 0) {return 0;}
	plyhb = a6->HitBoxes->push + a2->ActionScript->HB_Push;
	if(a2->ZDepth) {
		/* 0x7e4b2 */
		g.x8af6 = a2->XPI;
		a2->XPI = a2->x0044;
		d4 = slib_check_overlap((Object *)a2, (Player *)a6, hb, plyhb);	
		a2->XPI = g.x8af6;
	} else {
		d4 = slib_check_overlap((Object *)a2, (Player *)a6, hb, plyhb);
	}
	if (d4) {
		a6->Step = a2->Direction;
		a6->Direction = a2->Direction;
	}
	return d4;
}	
static void _CDSpecialReactMode(Player *ply_a6, Player *vict_a2, const HitBoxAct *a3) {	/* 0x7d02e */
	vict_a2->Direction = ply_a6->Flip;
	switch (ply_a6->FighterID) {
		case FID_RYU:
		case FID_KEN:
			if(a3->ReactMode == RM_HURRICANE)		{ _CDTurnToFaceMe(ply_a6, vict_a2); }
			break;
		case FID_E_HONDA:
			if(a3->ReactMode == RM_MULTIHIT)		{ _CDTurnToFaceMe(ply_a6, vict_a2); }
			break;
		case FID_BLANKA:
			if(a3->ReactMode == RM_ELECTROCUTED)	{_CDTurnToFaceMe(ply_a6, vict_a2); }
			break;
		case FID_CHUN_LI:
			if(a3->ReactMode == RM_MULTIHIT ||
			   a3->ReactMode == RM_HURRICANE)		{ _CDTurnToFaceMe(ply_a6, vict_a2); }
			break;
		case FID_ZANGEIF:
			if(a3->ReactMode == RM_0x14)			{ _CDTurnToFaceMe(ply_a6, vict_a2); }
			break;
		case FID_M_BISON:
			if((ply_a6->AnimFlags & 0xff) == 0x17) { vict_a2->Direction ^= 1; }
			break;
	/* no special processing for these guys */
	/*	case FID_GUILE:
		case FID_DHALSIM:
		case FID_SAGAT:
		case FID_BALROG:
		case FID_VEGA:		*/
		default:
			break;
	}
}
static void _CDTurnToFaceMe(Player *ply_a6, Player *vict_a2) {	/* 7d0a8 */
	if(ply_a6->XPI >= vict_a2->XPI) {
		vict_a2->Direction = FACING_LEFT;
	} else {
		vict_a2->Direction = FACING_RIGHT;
	}
}


static void start_timewarp_action (Player *vict) {	/* 7d720 */
	Object *obj;
	if((obj=AllocActor())) {
		g.FlagTimeWarp = TRUE;
		obj->exists = TRUE;
		obj->Sel    = SF2ACT_TIMEWARP;
		obj->Owner	= vict;
		obj->UserByte  = 0;
		if(vict->Energy < 0) {
			obj->SubSel = 1;
		}
	}
}


static void sub_7e314(void) {		// 7e314
	g.Player2.PushDirection = 1;
}
