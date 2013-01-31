/*
 *  collision.h
 *  GLUTBasics
 *
 *  Created by Ben on 20/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


void slib_ply_overlap();
void CDCheckPlayers(void);
void sub_7d222(Player *vict, Object *obj);
void CDCheckProjectile(Object *obj, int d7);
void projectile_to_projectile(Player *obj);
short diminishing_damage(int damage, Player *vict);
void center_collision_coords(void);
void sub_7d74e(Player *ply, Player *opp, const HitBoxAct *a3);

void sub_7d99a(Object *a6);

short sub_7e094(int d0, Object *a6, Player *vict, const HitBox *a3, const HitBox *a4);
short check_hitbox_overlap(Object *obj, Object *vict, const HitBoxAct *a3, const HitBox *a4);
short slib_check_overlap(Object *obj, Player *vict, const HitBox *a3, const HitBox *a4);
void CDCheckPushBoxes(void);			/* check pushboxes, take action */


void sub_7e16e(int d2);
int check_main_hitboxes(Object *obj, Object *vict, const HitBoxAct *a3);
void set_newreact(Player *vict, Player *ply, const HitBoxAct *a3);
void make_collision_sound(Player *ply, const HitBoxAct *a3);

void CDCheckDecor (Object *a6);		/* 7e340 check if collision with player */

const HitBoxAct *get_active_hitbox(Object *obj);
const HitBox *CDGetHitBoxHead(Object *obj);		
const HitBox *CDGetPushBox(Object *obj);
short CDPushOverlap(Player *a2, Object *a6);								/* 7e460 */

void set_hitstun_effect_for_projectile(Object *projectile, Object *obj2);

#pragma mark damage functions 
int sub_7d4fa(Player *vict, const HitBoxAct *a3);
int lookup_damage_and_score(Player *ply, Player *vict, const HitBoxAct *a3);
short randomize_damage(Player *ply,  int damage, const HitBoxAct *a3);
int damage_multiplier(Player *vict, int d6);

void mac_stun_from76(Player *ply, Player *opp);
struct hitboxresult {
    short id;       /* which hitbox was found */
    void *hitbox;   /*the data */
};
