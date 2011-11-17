/* sf2 fightgfx.c */

#include	"sf2types.h"
#include    "sf2const.h"
#include    "sf2macros.h"

#include    "structs.h"
#include	"player.h"

#include	"particle.h"
#include    "sprite.h"

#include	"actions.h"

#include	"gemu.h"

#include "fightgfx.h"

extern CPSGFXEMU gemu;
extern Game g;

static void fight_player_names(void);

void draw_peacesigns(void) {                /* 0x9316 */
    if (g.Player1.RoundsWon > 0) {
        OBJECT_DRAW_COORDS(66, 0x0, 0xe0, SF2_TILE_OBJ_PEACE, ATTR_NO_FLIP | PALETTE_0D);
    }
    if (g.Player1.RoundsWon > 1) {
        OBJECT_DRAW_COORDS(68, 0x10, 0xe0, SF2_TILE_OBJ_PEACE, ATTR_NO_FLIP | PALETTE_0D);
    }
    if (g.Player2.RoundsWon > 0) {
        OBJECT_DRAW_COORDS(84, 0x170, 0xe0, SF2_TILE_OBJ_PEACE, ATTR_NO_FLIP | PALETTE_0D);
    }
    if (g.Player2.RoundsWon > 1) {
        OBJECT_DRAW_COORDS(86, 0x160, 0xe0, SF2_TILE_OBJ_PEACE, ATTR_NO_FLIP | PALETTE_0D);
    }
}

void check_newtopscore(void) {
    return;
}                                 /* todo */

void init_fightgfx(void) {                /* 0x9254 */
    if (g.OnBonusStage) {
        redraw_fight_dsk();
        fight_player_names();
        check_newtopscore();
    } else {
		draw_peacesigns();
		draw_fight_dsk_init();
        fight_player_names();
        action_start_21();
        check_newtopscore();
    }
    /* tamper protection not included */
}



static void update_energy_cursor(Player *ply) {			// 939a
	// Update the energy cursor by 1 each frame, for smooth movement
	if (ply->EnergyCursor != ply->Energy) {
		--ply->EnergyCursor;
	}
}

void object_dot_cloth(void) {
	u32 cp;
	u16 x,y;
	for (x=0; x<24; x++) {
		for (y=0; y<16; y++) {
			cp = MakePointObj(x * 16, y * 16);
			OBJECT_DRAW_ID((x * 16) + y, cp >> 16, cp & 0xffff, 0x8080 + (((x * 16) + y)&0xf), 0);
		}
	}
	
}

void redraw_fight_dsk(void) {			//9418
	u32 cp;
	// Tile IDs for the Energy guage.
	static const u16 data_95fa[16]={		// 95fa
		0x81ee, 0x81fe, 0x81fd, 0x81fc, 0x81fb, 0x81fa, 0x81f9, 0x81f8,
		0x81f7, 0x81f6, 0x81f5, 0x81f4, 0x81f3, 0x81f2, 0x81f1, 0x81f0,
	};
	

	
    short temp = 8;
    int i=0;
	//    sub_929c();  /* see if we've beaten hiscore */
    if(g.OnBonusStage) {
		//        sub_97e8(); /* update bonus score */
    } else {
        if(g.TimeOut) { return; }    
		
        update_energy_cursor(PLAYER1);
		
		cp = MakePointObj(160, 220);
        if(g.Player1.EnergyCursor >= 0) {     
            if(g.Player1.EnergyCursor / 16) {
                for(i=0; i<(g.Player1.EnergyCursor / 16); i++) {        
                    OBJECT_DRAW_ID(90 + i, CP_X, CP_Y, TILE_ENERGY_FULL, ATTR_NO_FLIP | PALETTE_0C);
					INC_GFX_CURSOR(&cp, -16, 0);
                    temp--;
                }
            }
            if(temp >= 0) {
                OBJECT_DRAW_ID(90 + i, CP_X, CP_Y, 
                            data_95fa[g.Player1.EnergyCursor & 0xf],
                            ATTR_X_FLIP | PALETTE_0C
							);
				INC_GFX_CURSOR(&cp, -16, 0);
                i++;
                temp--;
            }
		}
		while(temp >= 0) {
			OBJECT_DRAW_ID(90 + i, CP_X, CP_Y, TILE_ENERGY_EMPTY, ATTR_NO_FLIP | PALETTE_0C);
			INC_GFX_CURSOR(&cp, -16, 0);
			i++; temp--;
		}
        
		
		temp = 8;
        update_energy_cursor(PLAYER2);
        if(g.Player2.EnergyCursor >= 0) {     
            if(g.Player2.EnergyCursor / 16) {
				cp = MakePointObj(208, 220);
                for(i=0; i<g.Player2.EnergyCursor >> 4; i++) {  
					OBJECT_DRAW_ID(110 + i, CP_X, CP_Y, TILE_ENERGY_FULL, ATTR_NO_FLIP | PALETTE_0C);
					INC_GFX_CURSOR(&cp, 16, 0);
                    temp--;
                }
            }
            if(temp >= 0) {
                OBJECT_DRAW_ID(110 + i, CP_X, CP_Y, 
							   data_95fa[g.Player2.EnergyCursor & 0xf],
							   ATTR_NO_FLIP | PALETTE_0C
							   );
				INC_GFX_CURSOR(&cp, 16, 0);
                i++;
                temp--;
            }
		}
		while(temp >= 0) {
			OBJECT_DRAW_ID(110 + i, CP_X, CP_Y, TILE_ENERGY_EMPTY, ATTR_NO_FLIP | PALETTE_0C);
			INC_GFX_CURSOR(&cp, 16, 0);
			i++; temp--;
		}
		
    }
}


void bonus_init_player(Player *ply) {
    ply->BonusScore     = 0;
    ply->BonusScoreDash = 0;
    // todo: setup_bonus_display(ply);
}

void draw_fight_dsk_init (void) {
	u32 cp;
    if(g.OnBonusStage) {
        bonus_init_player(PLAYER1);
        bonus_init_player(PLAYER2);
     } else {
		 // round corners at edge of energy guage
		 cp = MakePointObj(16, 220);
         OBJECT_DRAW_ID( 88, CP_X, CP_Y, 0x81ff, ATTR_X_FLIP | PALETTE_0C);
		 cp = MakePointObj(352, 220);
		 OBJECT_DRAW_ID(108, CP_X, CP_Y, 0x81ff, ATTR_NO_FLIP | PALETTE_0C);
         redraw_fight_dsk();
    }
}

