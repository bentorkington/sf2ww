/*
 *  coll_projectile.c
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

#include "coll_projectile.h"

extern Game g;



static void _CDEffectProjColl(Object *proj) {           /* 7d7d4 */
    Object *obj;
    if(obj = AllocActor()) {
        center_collision_coords();
        obj->exists = TRUE;
        obj->Sel    = SF2ACT_HITSTUN;     /* hitstuns */
        obj->XPI    = g.GPCollX;
        obj->YPI    = g.GPCollY;
        obj->Flip   = proj->Flip;   
        set_hitstun_effect_for_projectile(proj, obj);
    }
}
static void _CDStartAction38Vict(Player *vict) {      
	/* 0x7d720  Slow down gameplay for effect */
    Object *obj;
    if(obj=AllocActor()) {
        g.FlagTimeWarp = TRUE;
        obj->exists = TRUE;
        obj->Sel    = SF2ACT_TIMEWARP;      
        obj->Owner = vict;
        obj->UserByte  = 0;
        if(vict->Energy < 0) {
            obj->SubSel = 1;
        }
    }
}


void CDCheckProjectile(Object *obj, int d7) {   /* 0x7d0c0 check for projectile collision */
	/* d7 as far as I can tell is a leftover loop counter, hmm */
	/* d5 is also significant, but not sure why yet */
    Player *ply, *opp;
	const HitBoxAct *a3;
    short damage;
    
    if(obj->exists != 1) { return; }
	
	/* XXX */
	//    if((g.libsplatter + d7) & 1) {
	//        projectile_to_projectile((Player *)obj);     /* check for collision with opponents projectile */
	//        return;
	//    }
    ply = obj->Owner;
    opp = ply->Opponent;
    
    if(opp->exists != 1) { return; }
    if(g.PlayersThrowing || opp->CompImmune || opp->TCollDis || opp->Invincible) {
        return;
    }
    if(ply->Human == 0 && g.DebugNoCollide)  { return; }
	a3 = get_active_hitbox(obj);
    if(a3==0)         { return; }       /* sets hitbox in a3 */
    if(check_main_hitboxes(obj, (Object *)opp, a3)==0) { return; } /* return if no collision */
    
    /* projectile has collided */
    
    opp->NextReactMode2 = a3->ReactMode2;
    ply->Timer2 = opp->Timer2 = 14;             
    opp->Direction = obj->Flip;
    
	
    damage=lookup_damage_and_score(ply, opp, a3);
    if(sub_7d4fa(opp, a3) == 0) {
        damage=damage_multiplier(opp, damage);
        damage=randomize_damage(opp,damage, a3);   
        damage=diminishing_damage(damage, opp);
        
        if(g.GPHitBlocked) {
            damage /= 4;
            if (damage == 0) { damage = 1; }
        }
        
        opp->Energy -= damage;
        if(opp->Energy < 0) {
            opp->Energy      = -1;
            g.GPHitBlocked   = 0;
            opp->BlockStun   = 0; 
        }
    }
    opp->ProjectilePushBack = TRUE;
    
    set_newreact(opp, ply, a3);       
    make_collision_sound(opp, a3);
    
    if(opp->Energy < 0 || g.GPHitBlocked == 0) {
        _CDStartAction38Vict(opp);
    }
    obj->Energy = -2;
    _CDEffectProjColl(obj);
    if(opp->BlockStun || g.OnBonusStage) { return; }
    QueueEffect(g.GPPointsReward, ply->Side);
}

/* check for collision between projectile and enemy projectile,  supports only one projectile per player
 bound to be completely different in hacks with mutli fireballs */

void projectile_to_projectile(Player *obj) {				// 7d1c0
    Object *enemyProjectile;
    const HitBox *a3,*a4;
	
    if(!(a3=CDGetPushBox((Object *)obj)))		{ return; }
    if(obj->Owner->Opponent->Projectile==0)		{ return; }
    enemyProjectile = (Object *)obj->Owner->Opponent->Projectile;
	if(!(a4=CDGetPushBox(enemyProjectile)))	{ return; }
    if(check_hitbox_overlap((Object *)obj, (Object *)obj->Opponent,(HitBoxAct *)a3,a4)) {
        obj->Energy = -1;
        enemyProjectile->Energy = -1;
    }
}
