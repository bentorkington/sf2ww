/*
 *  projectiles.c
 *  GLUTBasics
 *
 *  Created by Ben on 16/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "sf2.h"

#include "structs.h"
#include "player.h"
#include "lib.h"

#include "particle.h"
#include "collision.h"

#include "projectiles.h"

#include "projdata.h"

extern Game g;

static void sub_23508(Object *obj, short d7);
static void projsm_fireball(Object *obj, short d7);
static void proc_yogafire(Object *obj, short d7); /* 0x23426 */
static void action_207f0(Object *obj, short d7);

static void destroy_projectile(Object *obj) {		// 235f8
	NEXT(obj->mode0);
	if(obj->Energy == -1) {
		NEXT(obj->mode0);
		obj->Owner->Projectile = 0;
	}
}

void process_projectiles(void) {			/* 22aca */
	short d7, i;
	
	debughook(8);
	d7=7;
	for(i=0; i<8; i++) {
		if(g.Objects1[i].exists == 0) {continue;}
		switch (g.Objects1[i].Sel) {
			case SF2_PROJ_HADOUKEN:  projsm_fireball(&g.Objects1[i], d7); break; 
			case SF2_PROJ_YOGAFIRE:  proc_yogafire(&g.Objects1[i], d7);   break;
			case SF2_PROJ_YOGAFLAME: sub_23508(&g.Objects1[i],d7);        break;
			case SF2_PROJ_SONICBOOM: projsm_fireball(&g.Objects1[i], d7); break;
			case SF2_PROJ_TIGER:	 projsm_fireball(&g.Objects1[i], d7); break;
			case SF2_PROJ_X05:		 action_207f0(&g.Objects1[i], d7);    break;		
			FATALDEFAULT;
		}
		--d7;
	}
}

// 207f0 barrels
void action_207f0(Object *obj, short d7) {
	int i;
	Object *obj2;
	
	if (g.PreRoundAnim) {
		return;
	}
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			g.x8ab8 = 20;
			g.x8ab9 = 32;
			obj->LocalTimer = 1;
			g.x8ab6 = obj;
			for(i=0; i<=7; i++) {
				if ((obj2=AllocActor())) {
					obj2->exists = 2;
					obj2->Sel = SF2ACT_0X09;
					obj2->SubSel = 7-i;
					obj2->x002e = 0;
					// XXX ud->x00b0[i]=obj;
				}
			}
			break;
		case 2:
			if (--obj->LocalTimer == 0) {
				// XXX sub_20858(obj);
			}
			break;
		case 4:
		case 6:
			FreeProjectile(obj);
		default:
			break;
	}
	/* XXX */
}



static void proc_yogafire(Object *obj, short d7) {  /* 0x23426 */
	Object *nobj;
	
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->flag1		= TRUE;
			obj->Step		= obj->Flip;
			obj->Step		^= 1;
			obj->x0024		= TRUE;				/* is a threat */
			obj->Energy		= 0x100;
			obj->HitBoxes	= &data_24932;
			obj->MoreBoxes	= data_24976;
			obj->XPI		+= obj->Flip ? 0x38 : -0x38;
			obj->YPI		+= 40;
			obj->Path = data_2349a[obj->SubSel/2];
            RHSetActionList(obj, RHCODE(0x23612), obj->SubSel / 2);
			break;
		case 2:
			CDCheckProjectile(obj, d7);
			if (obj->Energy < 0 || obj->flag1 == 0) {
				if (obj->Energy == -1) {
					if ((nobj=AllocActor())) {
						nobj->exists	= TRUE;
						nobj->Sel		= SF2ACT_HITSTUN;
						nobj->XPI		= obj->XPI;
						nobj->YPI		= obj->YPI;
						nobj->Flip		= obj->Flip;
						nobj->SubSel	= 5;		// fire splash
						
						obj->Owner->Projectile = NULL;
					}
				}
				NEXT(obj->mode0);
			} else {
				update_motion(obj);
				actiontick(obj);
				check_rect_queue_draw(obj);
			}
			break;
		case 4:
			NEXT(obj->mode0);
			obj->Owner->Projectile = 0;
			break;
		case 6:
			FreeProjectile(obj);
			break;
		FATALDEFAULT;
	}
}
static void projsm_fireball(Object *obj, short d7) {
	Object *splash;
		
	switch (obj->mode0) {
		case 0:
			obj->mode0	+= 2;
			obj->flag1	= TRUE;
			obj->Step	= obj->Flip ^ 1;
			obj->Energy = 0x100;
			if(obj->Flip) {
				obj->XPI += data_22bf6[obj->Sel];
			} else {
			    obj->XPI -= data_22bf6[obj->Sel];
			}
			obj->YPI += data_22c00[obj->Sel];
			obj->HitBoxes  = data_22c0a[obj->Sel].hb;
			obj->MoreBoxes = data_22c0a[obj->Sel].mb;
			
			if(obj->Sel == 4) {							  /* Tiger? */
				obj->Path = data_22c3e[obj->SubSel/2];    /* short[] *s */
			} else {
				obj->Path = data_22c32[obj->SubSel/2];
			}

            /* data_22c4a: Hadouken, unused, unused, Sonic Boom, Tiger */
            RHSetActionList(obj, RHCODE(((const u32[]){0x22cd2, 0x22cd2, 0x22cd2, 0x22cd2, 0x22e88, 0x23134}[obj->Sel])), 0);

            if(obj->Sel == 0 && sf2rand() == 0) {
				obj->Draw1				= TRUE;
				obj->Draw2.part.integer = PALETTE_OBJ_GOLDFIREBALL;	
			}
			if(obj->Sel == 4) {	/* Tiger */
				obj->YPI += (int[]){0, -40}[obj->UserByte];   /* 22c5e ducking tiger offsets */
			}
			break;
		case 2:
			/* 0x22c62 */
			CDCheckProjectile(obj, d7);
			if(obj->Energy < 0 || obj->flag1 == 0) {
				/* 22c80 */
				if(obj->Energy == -1) {
					obj->mode0 += 2;
					if((splash=AllocActor())) {
						splash->exists  = TRUE;
						splash->Sel     = SF2ACT_HITSTUN;
						splash->XPI		= obj->XPI;
						splash->YPI		= obj->YPI;
						splash->Flip	= obj->Flip;
						splash->SubSel  = (int []){4 ,0, 0, 4, 5, 0, 0, 0}[obj->Sel];   /* data_22cca subselectors for hitstun; these are the 'splashes' */
						obj->Owner->Projectile = 0;
					}
				}
				obj->mode0 +=2;
			} else {
                update_motion(obj);
                actiontick(obj);
                check_rect_queue_draw(obj);
            }
			break;
		case 4:
			obj->mode0 += 2;
			obj->Owner->Projectile = NULL;
			break;
		case 6:
			FreeProjectile(obj);
			break;
		FATALDEFAULT;
	}
}
static void sub_23508(Object *obj, short d7) {	/* Yoga Flame */	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0) 
			obj->flag1		= TRUE;
			obj->Step		= obj->Flip;
			obj->Step	   ^= 1;
			obj->x0024		= TRUE;				/* Is a threat */
			obj->Energy		= 0x100;
			obj->HitBoxes	= &data_24996;
			obj->MoreBoxes	= data_249da;
			obj->XPI	   += obj->Flip ? -0x48 : 0x48 ;
			obj->YPI	   += 0x1e;
            RHSetActionList(obj, RHCODE(0x23b6e), obj->SubSel / 2);
			break;
		case 2:
			CDCheckProjectile(obj, d7);
			obj->XPI = obj->Owner->XPI;
			if(obj->Flip) { 
				obj->XPI -= data_23572[obj->Owner->CA_ANIMFLAG][0];
			} else {
				obj->XPI += data_23572[obj->Owner->CA_ANIMFLAG][1];
			}
			obj->YPI = obj->Owner->YPI + data_23572[obj->Owner->CA_ANIMFLAG][1];
			if(obj->Owner->Energy < 0 ||
			   obj->Owner->mode0 == 4 ||
			   obj->Owner->mode1 == PLSTAT_REEL ||
									obj->Owner->ThrowStat) {
				destroy_projectile(obj);
				return;
			}
			if(obj->Energy < 0) {
                RHSetActionList(obj, RHCODE(0x23b6e), 3);
				obj->Energy = 0;
			}
			if (obj->CA_ENDFLAG) {
				destroy_projectile(obj);
				return;
			}
			actiontick(obj);
			check_rect_queue_draw(obj);
			break;
		case 4:
			NEXT(obj->mode0);
			obj->Owner->Projectile = NULL;
			break;
		case 6:
			FreeProjectile(obj);
			break;
		FATALDEFAULT;
	}
	
}