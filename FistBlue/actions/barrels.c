//
//  barrels.c
//  MT2
//
//  Created by bjt on 2/01/16.
//
//

#include "sf2.h"
#include "structs.h"
#include "barrels.h"
#include "lib.h"

extern struct game g;

// barrel distance calc
struct UserData_Barrels {
    Object *barrel[8];		/* 0xb0 */
};
struct UserData_Barrel {
    short playerdists[2];		//x0098
};

typedef struct UserData_Barrels UD_Bars;
typedef struct UserData_Barrel UD_Bar;

short sub_208c8(short d2) {		// 208c8
    
    // sf2ua follows a NULL pointer from x8ab6 here, and obviously gets lucky
    if (!g.x8ab6) {
        return 0x100;
    }
    
    UD_Bars *ud = (UD_Bars *)&g.x8ab6->UserData;
    UD_Bar *bar;
    
    short d0 = 0x100;
    Object *a2;
    short i;
    
    for (i=0;i<8;i++) {
        a2 = ud->barrel[i];
        bar = (UD_Bar *)&a2->UserData;
        if (bar->playerdists[d2]<d0) {
            d0 = bar->playerdists[d2];
        }
    }
    return d0;
}

static void sub_20858(Object *obj)
{
    UD_Bars *ud = (UD_Bars *)&obj->UserData;

    int i;
    for (i=0; i<8; ++i) {
        if (ud->barrel[i]->x002e == 0) {
            g.x8ab8--;
            if (g.x8ab8 < 0) {
                NEXT(obj->mode0);
            } else {
                obj->Timer = RHByteOffset(0x20892, g.x8ab8);
            }
            break;
        }
    }
    obj->LocalTimer = 1;
}

// 207f0 barrels
void action_207f0(Object *obj, short d7) {
    int i;
    Object *obj2;
    
    UD_Bars *ud = (UD_Bars *)&obj->UserData;
    
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
                if ((obj2=pop_1174())) {
                    obj2->exists = 2;
                    obj2->Sel = 0x9;
                    obj2->SubSel = 7-i;
                    obj2->x002e = 0;
                    ud->barrel[i]=obj2;
                }
            }
            break;
        case 2:
            if (--obj->LocalTimer == 0) {
                sub_20858(obj);
            }
            break;
        case 4:
        case 6:
            FreeProjectile(obj);
        default:
            break;
    }
}
