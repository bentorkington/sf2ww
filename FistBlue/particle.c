/* sf2 particle.c */

#include "sf2.h"

#include "gstate.h"
#include "structs.h"
#include "player.h"

#include "particle.h"
#include "lib.h"

extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;


#ifdef REDHAMMER_EXTROM
void RHSetActionList(Object *obj, const void *listaddr, short sel) {
#if 0
    print_rom_offset("setactionList", listaddr);
#endif
    RHSetAction(obj, (FBAction *)RHOffsetLookup16(listaddr, sel));
}
void RHSetAction(Object *obj, const FBAction *act) {
    obj->ActionScript       = act;
    obj->Timer              = RHSwapWord(act->Delay);
    obj->AnimFlags          = RHSwapWord(act->Flags);

#if 0
    const struct image *image = (const struct image *)RHCODE(RHSwapLong(obj->ActionScript->Image));
    
    if ((void *)image < (void *)act || ((void *)image - (void *)act) > 0x8000) {
        print_rom_offset("act", act);
        print_rom_offset("img", image);
        printf("suspect image %p\n", image);
    }
#endif
}
void RHActionTick(Object *obj) {
    if(--obj->Timer == 0) {
        if(RHSwapWord(obj->ActionScript->Flags) & 0x8000) {
            
            u32 *next = (void *)obj->ActionScript + sizeof(FBAction);
            obj->ActionScript = (FBAction *)RHCODE(RHSwapLong(*next));
        } else {
            obj->ActionScript++;
        }
        RHSetAction(obj, obj->ActionScript);
    }
}
void setaction_list(Object *obj, const Action **list, short sel) {
    panic(99);
}
void setaction_direct(Object *obj, const Action *act) {
    panic(99);
}
void actiontick(Object *obj) {
    RHActionTick(obj);
}
#else
void setaction_list(Object *obj, const Action **list, short sel ) {		/* 0x23ce */
    
    const Action *act = list[sel];
    /* Action *act = (Action *)list + (u16 *)*(list + (sel * sizeof (short))); */
    setaction_direct(obj, act);
}

void setaction_direct(Object *obj, const Action *act) {			/* 23da */
    obj->ActionScript       = act;
    obj->Timer              = act->Delay;
    obj->AnimFlags          = act->Loop << 8 | act->Next;
}

void actiontick(Object *obj) {		/* 23ec */
    if(--obj->Timer == 0) {
        if(obj->ActionScript->Loop & 0x80) {
            obj->ActionScript = (Action *)obj->ActionScript[1].Image;
        } else {
            obj->ActionScript++;
        }
        setaction_direct(obj, obj->ActionScript);
    }
}
#endif

void sub_25f8(Object *obj) {
	GState *gs;
	POINT16 point;
	if (obj->exists) {
		gs = get_graphics_context(obj);	
		point.x = obj->XPI - gs->XPI;
		point.y = obj->YPI - gs->YPI;
		if ((point.x > -64 && point.x < 544) && (point.y > -96 && point.y < 512)) {
			obj->flag1 = TRUE;
			enqueue_and_layer(obj);
		} else {
			obj->flag1 = FALSE;
		}
	}
}
void check_rect_queue_draw(Object *obj) {   /* 0x2540 */
    GState *gc;       
	POINT16 point;

    if(obj->exists) {
        obj->flag1 = FALSE;
        gc = get_graphics_context(obj);
        point.x = obj->XPI - gc->XPI;  /*world to camera */
        if (point.x < -VISIBLE_MARGIN || point.x > (SCREEN_WIDTH + VISIBLE_MARGIN)) { return; }
        point.y = obj->YPI - gc->YPI;
        point.y += VISIBLE_MARGIN;
        if (point.y < -VISIBLE_MARGIN || point.y > (SCREEN_HEIGHT + VISIBLE_MARGIN)) { return; }
        obj->flag1=TRUE;
        enqueue_and_layer(obj);
    }
}

void check_onscreen_queue(Object *obj) {		// 0x2578
	GState *gs;
	int x,y;
	
	if (obj->exists != FALSE) { 
		obj->flag1 = FALSE;
		gs = get_graphics_context(obj);
		x = obj->XPI - gs->XPI + VISIBLE_MARGIN;
		if (x < 0 || x >= 576) { return; }	
		y = obj->YPI - gs->YPI + VISIBLE_MARGIN;
		if (y < 0 || y >= 448) { return; }
		obj->flag1=TRUE;
		enqueue_and_layer(obj);
	}
}



void die_if_offscreen(Object *obj) {	// 248c
	if (obj->XPI - gstate_Scroll2.XPI <= -96   ||
		obj->XPI - gstate_Scroll2.XPI > 480 ||
		obj->YPI - gstate_Scroll2.YPI <= -96 ||
		obj->YPI - gstate_Scroll2.YPI > 352)
	{	
		obj->mode0 = 6;		// actor dies
	}
}

GState *get_graphics_context(Object *obj) {		/* 2628 */
	static GState *scrolls[3] = { &gstate_Scroll2, &gstate_Scroll1, &gstate_Scroll3 };
	
    if (obj->Scroll) {
		obj->x0044 = obj->XPI;
        return scrolls[(unsigned char)obj->Scroll / 2];		/* cheeky */
    }
	if (obj->ZDepth) {
//		obj->x0044 = obj->XPI - (g.x02be[0x800 - ((obj->ZDepth +1))]-0xc0);     XXX rowscroll not working
		obj->x0044 = obj->XPI;
		//obj->x0044 = obj->XPI;
	} else {
		obj->x0044 = obj->XPI;
	}
    return scrolls[0];
}


static Object *action_null (void) {
    return NULL;
}

Object *(* const actiontypes[7])() = {
    &action_null,
    &action_null,
    &AllocProjectile,
    (Object *(*const)())&pop_1174,
    &AllocActor,
    &action_null,
    (Object *(*const)())&pop_5d0c,
};

Object *alloc_action_by_type(short type) {
	return (Object *)actiontypes[type>>1]();
}


void update_motion(Object *obj) {		/* 2416 */
    /* fixed precision arithmetic */	
    obj->X.full += (obj->Path[obj->Step].x.full << 8);
    obj->Y.full += (obj->Path[obj->Step].y.full << 8);
}


void enqueue_and_layer (Object *obj) {		/* 25de */
    switch(obj->Layer) {
        case GFX_LAYER1: 
            switch(obj->Pool) {
                case 0: g.x8e16[g.Layer1Grp1Cnt++] = obj; break;
                case 2: g.x8e26[g.Layer1Grp2Cnt++] = obj; break;
            }
            break;
        case GFX_LAYER2: 
            switch(obj->Pool) {
                case 0: g.x8e46[g.Layer2Grp1Cnt++] = obj; break;
                case 2: g.x8e66[g.Layer2Grp2Cnt++] = obj; break;
                case 4: g.x8e86[g.Layer2Grp3Cnt++] = obj; break;
            }
            break;
        case GFX_LAYER3: 
            switch(obj->Pool) {
                case 0: g.x8efe[g.Layer3Grp1Cnt++] = obj; break;
                case 2: g.x8f76[g.Layer3Grp2Cnt++] = obj; break;
                case 4: g.x8fee[g.Layer3Grp3Cnt++] = obj; break;
                case 6: g.x9066[g.Layer3Grp4Cnt++] = obj; break;
                case 8: g.x90de[g.Layer3Grp5Cnt++] = obj; break;
                case 0xa: g.x9156[g.Layer3Grp6Cnt++] = obj; break;
            }
            break;
		FATALDEFAULT;
    }
}

#pragma mark ---- Animation Functions ----

int check_ground_collision(Object *ply) {		//3152
    if(ply->OnPlatform) {
        ply->OnPlatform2 = TRUE;
        return 1;
    }
    if (ply->YPI < g.GroundPlaneY) {
        ply->YPI = g.GroundPlaneY;
        return TRUE;
    } else {
        return FALSE;
    }
}

void update_obj_path(Object *ply) {		/* 31b0 */
    if (ply->Flip != FACING_LEFT) { 
		ply->X.full -= ply->Path[ply->Step ^ ply->Flip].x.full << 8;
	} 
	else { 
		ply->X.full += ply->Path[ply->Step ^ ply->Flip].x.full << 8;
	}
	ply->Y.full -= ply->Path[ply->Step ^ ply->Flip].y.full << 8;
}


void CAApplyVelocity(Object *obj) {		// 2472
	obj->X.full += (obj->VelX.full << 8);
    obj->Y.full += (obj->VelY.full << 8);
}

void CATrajectory(Object *obj) {         /* 245a */
    obj->VelX.full -= obj->AclX.full;
    obj->VelY.full -= obj->AclY.full;
	
	CAApplyVelocity(obj);
}
    
    
   
        
