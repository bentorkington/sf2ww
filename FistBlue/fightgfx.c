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
#include    "gfxlib.h"
#include    "fightgfx.h"

#include    "lib.h"


extern CPSGFXEMU gemu;
extern Game g;

#define VICTORY_Y         224
#define VICTORY_P1_R1_X   0
#define VICTORY_P1_R2_X   16
#define VICTORY_P2_R1_X   368
#define VICTORY_P2_R2_X   352

// Sprite object slots

#define OBJS_HIGH_SCORE_NAME    16
#define OBJS_VICTORY_P1_R1      33
#define OBJS_VICTORY_P1_R2      34

#define OBJS_VICTORY_P2_R1      42
#define OBJS_VICTORY_P2_R2      43
#define OBJS_ENERGY_GAUGE_P1    44
#define OBJS_ENERGY_GAUGE_P2    54

void draw_victorysigns(void) {                /* 0x9316 */
    if (g.Player1.RoundsWon > 0) {
        OBJECT_DRAW_COORDS(OBJS_VICTORY_P1_R1, VICTORY_P1_R1_X, VICTORY_Y, SF2_TILE_OBJ_VICTORY, ATTR_NO_FLIP | PALETTE_0D);
    }
    if (g.Player1.RoundsWon > 1) {
        OBJECT_DRAW_COORDS(OBJS_VICTORY_P1_R2, VICTORY_P1_R2_X, VICTORY_Y, SF2_TILE_OBJ_VICTORY, ATTR_NO_FLIP | PALETTE_0D);
    }
    if (g.Player2.RoundsWon > 0) {
        OBJECT_DRAW_COORDS(OBJS_VICTORY_P2_R1, VICTORY_P2_R1_X, VICTORY_Y, SF2_TILE_OBJ_VICTORY, ATTR_NO_FLIP | PALETTE_0D);
    }
    if (g.Player2.RoundsWon > 1) {
        OBJECT_DRAW_COORDS(OBJS_VICTORY_P2_R2, VICTORY_P2_R2_X, VICTORY_Y, SF2_TILE_OBJ_VICTORY, ATTR_NO_FLIP | PALETTE_0D);
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
		draw_victorysigns();
		draw_fight_dsk_init();
        fight_player_names();
        action_start_21();				// "KO" sign
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
			OBJECT_DRAW_ID((x * 16) + y, cp >> 16, cp & 0xffff, SF2_TILE_OBJ_ASCII_12X12 + (((x * 16) + y) & 0xf), 0);
		}
	}
}

#include "gemu.h"

static void sub_92de(u16 *gfx_p, u32 cp, const char *a1) {
    int i;
    for (i=2; i>=0; --i) {
        gemuObjectDraw(gfx_p,          CP_X, CP_Y, *a1 + SF2_TILE_OBJ_ASCII_12X12, 13);
#ifdef CPS
        gemuObjectDraw(gfx_p + 0x4000, CP_X, CP_Y, *a1 + SF2_TILE_OBJ_ASCII_12X12, 13);
#endif
        gfx_p += 4;
        ++a1;
        COORDS_OFFSET(&cp, 12, 0);
    }
}

/*!
 @brief Show the name of the high score owner.
 'HI' is displayed if on of the current players has the high score.
 @see sf2ua/0x929c
 */
static void _print_high_score_name(void) {
    u32 cp;
    u16 *gfx_p;
    static const char hi[]=" HI\xff";
    if (g.Player1.Score > g.TopScore || g.Player2.Score > g.TopScore ) {
        OBJ_CURSOR_SET(gfx_p, OBJS_HIGH_SCORE_NAME)
        cp = MakePointObj(120, 240);
        sub_92de(gfx_p, cp, hi);
    } else {
        OBJ_CURSOR_SET(gfx_p, OBJS_HIGH_SCORE_NAME)
        cp = MakePointObj(124, 240);
        sub_92de(gfx_p, cp, g.HiScoreTable[0].name);
    }
}

/**
 @see: sf2ua/0x9418
 */
void redraw_fight_dsk(void) {
	u32 cp;

    static const u16 energy_cursor_tiles[16]={		// 95fa
		0x81ee, 0x81fe, 0x81fd, 0x81fc, 0x81fb, 0x81fa, 0x81f9, 0x81f8,
		0x81f7, 0x81f6, 0x81f5, 0x81f4, 0x81f3, 0x81f2, 0x81f1, 0x81f0,
	};
		
    int tiles = 8;
    int i=0;
    
	_print_high_score_name();  /* see if we've beaten hiscore */

    if(g.OnBonusStage) {
        // todo:   sub_97e8(); /* update bonus score */
    } else {
        if(g.TimeOut) { return; }    
		
        update_energy_cursor(PLAYER1);
		cp = MakePointObj(160, 220);
		
        if(g.Player1.EnergyCursor >= 0) {     
            if(g.Player1.EnergyCursor / 16) {
                for(i=0; i<(g.Player1.EnergyCursor / 16); ++i) {
                    OBJECT_DRAW_ID(OBJS_ENERGY_GAUGE_P1 + 1 + i, CP_X, CP_Y, TILE_ENERGY_FULL, ATTR_NO_FLIP | PALETTE_0C);
					COORDS_OFFSET(&cp, -16, 0);
                    --tiles;
                }
            }
			
            if(tiles >= 0) {
                OBJECT_DRAW_ID(OBJS_ENERGY_GAUGE_P1 + 1 + i, CP_X, CP_Y,
                            energy_cursor_tiles[g.Player1.EnergyCursor & 0xf],
                            ATTR_X_FLIP | PALETTE_0C
							);
				COORDS_OFFSET(&cp, -16, 0);
                ++i;
                --tiles;
            }
		}
		// draw the remaining empty tiles
		while(tiles >= 0) {
			OBJECT_DRAW_ID(OBJS_ENERGY_GAUGE_P1 + 1 + i, CP_X, CP_Y, TILE_ENERGY_EMPTY, ATTR_NO_FLIP | PALETTE_0C);
			COORDS_OFFSET(&cp, -16, 0);
			i++; tiles--;
		}
        
		// 92de: now for P2
        
        i=0;
		tiles = 8;
        update_energy_cursor(PLAYER2);
		cp = MakePointObj(208, 220);

        if(g.Player2.EnergyCursor >= 0) {
            if(g.Player2.EnergyCursor / 16) {
                for(i=0; i<g.Player2.EnergyCursor / 16; ++i) {  
					OBJECT_DRAW_ID(OBJS_ENERGY_GAUGE_P2 + 1 + i, CP_X, CP_Y, TILE_ENERGY_FULL, ATTR_NO_FLIP | PALETTE_0C);
					COORDS_OFFSET(&cp, 16, 0);
                    --tiles;
                }
            }
            if(tiles >= 0) {
                OBJECT_DRAW_ID(OBJS_ENERGY_GAUGE_P2 + 1 + i, CP_X, CP_Y,
							   energy_cursor_tiles[g.Player2.EnergyCursor & 0xf],
							   ATTR_NO_FLIP | PALETTE_0C
							   );
				COORDS_OFFSET(&cp, 16, 0);
                ++i;
                --tiles;
            }
		}
		while(tiles >= 0) {
			OBJECT_DRAW_ID(OBJS_ENERGY_GAUGE_P2 + 1 + i, CP_X, CP_Y, TILE_ENERGY_EMPTY, ATTR_NO_FLIP | PALETTE_0C);
			COORDS_OFFSET(&cp, 16, 0);
			i++; tiles--;
		}
		
    }
}


void bonus_init_player(Player *ply) {
    ply->BonusScore     = 0;
    ply->BonusScoreDash = 0;
    //todo: setup_bonus_display(ply);
}

void draw_fight_dsk_init (void) {
	u32 cp;
    if(g.OnBonusStage) {
        bonus_init_player(PLAYER1);
        bonus_init_player(PLAYER2);
     } else {
		 // round corners at edge of energy guage
		 cp = MakePointObj(16, 220);
         OBJECT_DRAW_ID(OBJS_ENERGY_GAUGE_P1, CP_X, CP_Y, 0x81ff, ATTR_X_FLIP | PALETTE_0C);
         
		 cp = MakePointObj(352, 220);
		 OBJECT_DRAW_ID(OBJS_ENERGY_GAUGE_P2, CP_X, CP_Y, 0x81ff, ATTR_NO_FLIP | PALETTE_0C);
         redraw_fight_dsk();
    }
}

