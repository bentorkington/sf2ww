/* gstate.c */

#include "sf2.h"

#include "gstate.h"
#include "structs.h"
#include "player.h"
#include "particle.h"

#include "lib.h"
#include "gfxlib.h"
#include "scroll_maint.h"
#include "scroll_data.h"
#include "parallax.h"
#include "gemu.h"

extern CPSGFXEMU gemu;
extern Game g;
extern int scroll_cursor;

ScrollState gstate_Scroll1;		/* b52 - bd1 */
ScrollState gstate_Scroll2;		/* bd2 - c51 */
ScrollState gstate_Scroll3;		/* c52 - cd1 */
RowScrollState gstate_RowScroll;	/* cd2 - d51 */

static void draw_n_rows(unsigned short *gfx_p, const u16 *tile_p, short count);
static void _GSDrawScroll2C(ScrollState *gs, unsigned short *gfx_p, const u16 *tile_p, CP cp);

static void gstate_update_scroll2(ScrollState *gs);
static void gstate_update_scroll3(ScrollState *gs);

static void _GSDrawScroll3A(ScrollState *gs, u16 *gfx_p, const u16 *tilep, CP cp);

#pragma mark ---- Layer Initialisers ----

void GSInitUpdateMethods(void) {
	static const char data_83900[16][6] = {	// Scroll update methods
		{ SCR1X_SKY,         0x4,               SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_0_75,  },  /* RYU */
		{ SCR1X_PERSPECTIVE, SCR1Y_FOLLOW_SCR2, SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_SKY2,        0x4,               SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_PERSPECTIVE, 0x4,               SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_NONE,        0x6,               SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y_MINUS_OFFSET,  },
		{ SCR1X_NONE,        0x6,               SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_PERSPECTIVE, SCR1Y_FOLLOW_SCR2, SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_PERSPECTIVE, 0xe,               SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_PERSPECTIVE, SCR1Y_FOLLOW_SCR2, SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_PERSPECTIVE, 0x4,               SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_1_25,  },
		{ SCR1X_PERSPECTIVE, 0x4,               SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_PERSPECTIVE, SCR1Y_FOLLOW_SCR2, SCR2X_FIGHT, SCR2Y_FIGHT, SCR3X_PERSP, SCR3Y_2Y,  },
		{ SCR1X_NONE,        SCR1Y_NONE,        SCR2X_NONE,  SCR2Y_NONE,  SCR3X_NONE,  SCR3Y_2Y,  },
		{ SCR1X_NONE,        SCR1Y_FOLLOW_SCR2, SCR2X_NONE,  SCR2Y_FIGHT, SCR3X_NONE,  SCR3Y_2Y,  },
		{ SCR1X_SKY2,        SCR1Y_FOLLOW_SCR2, SCR2X_NONE,  SCR2Y_FIGHT, SCR3X_NONE,  SCR3Y_2Y,  },
		{ SCR1X_NONE,        SCR1Y_FOLLOW_SCR2, SCR2X_NONE,  SCR2Y_FIGHT, SCR3X_NONE,  SCR3Y_2Y,  },
	};
	
    gstate_Scroll1.XUpdateMethod = data_83900[g.CurrentStage][0];
    gstate_Scroll1.YUpdateMethod = data_83900[g.CurrentStage][1];
    gstate_Scroll2.XUpdateMethod = data_83900[g.CurrentStage][2];
    gstate_Scroll2.YUpdateMethod = data_83900[g.CurrentStage][3];
    gstate_Scroll3.XUpdateMethod = data_83900[g.CurrentStage][4];
    gstate_Scroll3.YUpdateMethod = data_83900[g.CurrentStage][5];
}

void GSInitDimensions(void) {
	static const u16 data_83960[16][4] = {		// Max dimensions of stages
		// MinX   MaxX     MinY   MaxY
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },	// 384 pixel difference, 2 screenwidths
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0100, 0x0280, 0x0000, 0x0010,  },
		{ 0x0000, 0x0000, 0x0000, 0x0010,  },
		{ 0x0000, 0x0000, 0x0000, 0x0010,  },
		{ 0x0000, 0x0000, 0x0000, 0x0010,  },
		{ 0x0000, 0x0000, 0x0000, 0x0010,  },
	};
	static const u16 data_839e0[16]={            /* ground planes stupid, same for all levels */
		0x0028, 0x0028, 0x0028, 0x0028, 0x0028, 0x0028, 0x0028, 0x0028,
		0x0028, 0x0028, 0x0028, 0x0028, 0x0028, 0x0028, 0x0028, 0x0028,  
	};
	
	g.StageMinX = data_83960[g.CurrentStage][0];
    g.StageMaxX = data_83960[g.CurrentStage][1];
    g.StageMinY = data_83960[g.CurrentStage][2];
    g.StageMaxY = data_83960[g.CurrentStage][3];

	g.GroundPlaneY = data_839e0[g.CurrentStage];
}
void GSInitDispEna(void) {
	static const u16 data_83ac0[20]={		// DISPENAs
		0x0b40, 0x06c0, 0x0e40, 0x0e40, 0x0e40, 0x0b40, 0x12c0, 0x0780, 0x0780, 0x3240,
		0x0e40, 0x0780, 0x06c0, 0x06c0, 0x0e40, 0x0e40, 0x302e, 0x0006, 0x0440, 0x00c0,
	};
	
    short temp = g.CPS.DispEna & 0x1b;
    g.CPS.DispEna = temp | data_83ac0[g.CurrentStage];
}


static CP _GSCoordOffsetScr2 (ScrollState *gs, short offset) {
	static const struct Point16 data_83db0[4] = {
		{ -144,   384 },
		{  512,   384 },
		{ -144,  -144 },
		{ -144,   384 },
	};
	
    CP cp;
    offset &= 0xf;
    cp.x = gs->position.x.part.integer + data_83db0[offset / 4].x;
    cp.y = ~(gs->position.y.part.integer + data_83db0[offset / 4].y);
    
    return cp;
}
static CP _GSCoordOffsetScr3 (ScrollState *gs, short offset) {
	static const struct Point16 data_83ddc[4] = {
		{ -160,   384 },
		{  512,   384 },
		{ -160,  -160 },
		{ -160,   384 },
	};
	
    CP cp;
    offset &= 0x000f;
    cp.x =   gs->position.x.part.integer + data_83ddc[offset / 4].x;
    cp.y = ~(gs->position.y.part.integer + data_83ddc[offset / 4].y);
    
    return cp;
}
    

static u16 *_GSCoordsScroll2(CP cp){
#ifdef CPS							
    return  (void *)(CPS_VIDEO_SCROLL2 + 
                ((cp.y & 0x300) << 4) +
                ((cp.y & 0xf0) >> 2)  +			
                ((cp.x << 2) & 0xfc0)
    );
#else
	return gemu.Tilemap_Scroll2[
				((cp.y & 0x300) << 2) |
				((cp.y & 0x0f0) >> 4) |
				(cp.x & 0x3f0)
								];
#endif
}    
static void *_GSCoordsScroll3(CP cp){
#ifdef CPS
    return  (void *)(CPS_VIDEO_SCROLL3 + 
                ((cp.y & 0x700) << 3) +
                ((cp.y & 0xe0) >> 3)  +
                (cp.x & 0x7e0)
    );
#else
	return gemu.Tilemap_Scroll3[ 
				((cp.y & 0x700) << 1) |
				((cp.y & 0x0e0) >> 5) |
				((cp.x >> 2) & 0x1f8) 
					];
#endif
}


static void _GSDrawScroll2C(ScrollState *gs, u16 *gfx_p, const u16 *tile_p, CP cp) {        // 84336
    short d2, d0;
    d2 = ((~cp.x) & 0xf0) >> 4;
    
    draw_n_rows(gfx_p, tile_p, d2);
    
    tile_p = skyscraper_realign(gs, &gfx_p);
    draw_n_rows(gfx_p, tile_p, 15);
    
    tile_p = skyscraper_realign(gs, &gfx_p);
    d2 += 17;
    d0 = 41 - d2;
    if (d0 < 15) {
        draw_n_rows(gfx_p, tile_p, d0);
    } else {
        draw_n_rows(gfx_p, tile_p, 15);
        tile_p = skyscraper_realign(gs, &gfx_p);
        draw_n_rows(gfx_p, tile_p, d0 - 16);
    }
}

static void sub_84170(int lines, u16 **gfx_p, const u16 **tilep) {
    for (; lines >= 0; --lines) {
        *(*gfx_p)++ = *(*tilep)++;
        *(*gfx_p)++ = *(*tilep)++;
    }
}

static void _GSDrawScroll3A(ScrollState *gs, u16 *gfx_p, const u16 *tilep, CP cp) {  /* 84138 for Scroll3 was funky2_draw*/
    short d0 = ((~cp.y) & 0xe0) >> 5;        // y / 32
    short d3 = d0;
    sub_84170(d0, &gfx_p, &tilep);

    tilep = realign_scr3a(gs, &gfx_p);
    sub_84170(7, &gfx_p, &tilep);

    tilep = realign_scr3a(gs, &gfx_p);
    d3 += 0x8;
    d0 = 0x10 - d3;
    if (d0 < 8) {
        sub_84170(d0, &gfx_p, &tilep);
    } else {
        sub_84170(7, &gfx_p, &tilep);
        tilep = realign_scr3a(gs, &gfx_p);
        sub_84170(1, &gfx_p, &tilep);
    }
}

inline static void draw_n_rows(u16 *gfx_p, const u16 *tile_p, short n_cols) {            // 84374
    int i;
    for(i = 0; i <= n_cols; i++) {
        SCR2_DRAW_TILE(gfx_p, *tile_p, *tile_p + 1);
        SCR2_CURSOR_BUMP(gfx_p,  1, 0);
        SCR2_CURSOR_BUMP(tile_p, 1, 0);
    }
}

static void gstate_update_scroll2 (ScrollState *gs) {
    short temp;
    CP cp;

    g.CPS.Scroll2X = gs->position.x.part.integer;
    g.CPS.Scroll2Y = gs->position.x.part.integer;
    	
    temp = gs->position.x.part.integer & 0x10;
    temp ^= gs->x001e;
    if (temp == 0) {
        gs->x001e ^= 0x10;
    }
    
    cp = _GSCoordOffsetScr2(gs, gs->x0024);
    GSDrawScroll2A(gs, _GSCoordsScroll2(cp), _GSCoordsScroll2(cp),cp);
    
    temp  = gs->position.y.part.integer & 0x10;
    temp ^= gs->x001f;
    if(temp == 0) {
        gs->x001f ^= 0x10;
    }
    
    cp = _GSCoordOffsetScr2(gs, gs->x0024);
    _GSDrawScroll2C(gs, _GSCoordsScroll2(cp), _GSCoordsScroll2(cp), cp);     /* seems to only be used on attract building */
}

static void gstate_update_scroll3 (ScrollState *gs) {		//83d06
    short temp;
    CP cp;

    g.CPS.Scroll3X = gs->position.x.part.integer;
    g.CPS.Scroll3Y = gs->position.y.part.integer;
    
    temp  = gs->position.x.part.integer & 0x20;
    temp ^= gs->x001e;
    if (temp == 0) {
        gs->x001e ^= 0x20;
		cp = _GSCoordOffsetScr3(gs, gs->x0024);
		_GSDrawScroll3A(gs, _GSCoordsScroll3(cp), _GSLookupScroll3(gs, cp), cp);
    }
    	
    
    temp  = gs->position.y.part.integer & 0x20;
    temp ^= gs->x001f;
    if (temp == 0) {
        gs->x001f ^= 0x20;
		cp = _GSCoordOffsetScr3(gs, gs->x0024);
		// XXX _GSDrawScroll3B(gs, _GSCoordsScroll3(cp), _GSLookupScroll3(gs, cp),cp);
    }
}
    
#pragma mark Publics

/**
 @brief Update the tilemaps
 @see sf2ua/0x8318a
 */
void TMUpdate(void) {
    debughook(4);
    if (g.OnBonusStage) {
		/* 831b2 */
        GSMaintScroll2(&gstate_Scroll2);
        GSMaintScroll1(&gstate_Scroll1);
        GSMaintScroll3(&gstate_Scroll3);
    } else {
		/* 83196 */
        GSMaintScroll2(&gstate_Scroll2);
        TMMaintRowScroll(&gstate_RowScroll);
        GSMaintScroll1(&gstate_Scroll1);
        GSMaintScroll3(&gstate_Scroll3);
    }
}

/**
 @brief Initialize the tilemaps for the current stage
 @see sf2ua/0x83716
 */
void TMInitForStage(void){			/* 83716 setup tilemaps & palettes from g.Pallete1 */
	GSStageScroll1(g.Stage);
	GSStageScroll2(g.Stage);
	GSStageScroll3(g.Stage);
}

/**
 @brief Move to the center of the stage arrangement
 @see sf2ua/0x83860
 */
void TMGotoCenter(void) {
    gstate_Scroll1.position.x.part.integer = RH2DWord(0x83a00, 6, g.CurrentStage, 0);
    gstate_Scroll1.position.y.part.integer = RH2DWord(0x83a00, 6, g.CurrentStage, 1);
    gstate_Scroll2.position.x.part.integer = RH2DWord(0x83a00, 6, g.CurrentStage, 2);
    gstate_Scroll2.position.y.part.integer = RH2DWord(0x83a00, 6, g.CurrentStage, 3);
    gstate_Scroll3.position.x.part.integer = RH2DWord(0x83a00, 6, g.CurrentStage, 4);
    gstate_Scroll3.position.y.part.integer = RH2DWord(0x83a00, 6, g.CurrentStage, 5);
}

/**
 @brief Parallax is handled by some other mechanism, just update scrolls 2 & 3
 */
void TMUpdateScrollsAttractSequence(void) {
    gstate_update_scroll2(&gstate_Scroll2);
    gstate_update_scroll3(&gstate_Scroll3);
}

void TMSetupScroll2(ScrollState *gs) {			// 83c3c
    CP			cp;
	int			i;
	
	g.CPS.Scroll2X = gs->position.x.part.integer;
	g.CPS.Scroll2Y = gs->position.y.part.integer;
	
    cp.x = gs->position.x.part.integer -  144;
    cp.y = ~(gs->position.y.part.integer + 384);
	
	for (i = 0x29; i >= 0; --i) {			// 768 pixels
		GSDrawScroll2A(gs, _GSCoordsScroll2(cp), _GSLookupScroll2(gs, cp), cp);
		cp.x += 16;
	}
}
void TMSetupScroll3(ScrollState *gs) {			// 83cd2 was setup_scroll3
    CP			cp;
	int			i;
	
	g.CPS.Scroll3X = gs->position.x.part.integer;
	g.CPS.Scroll3Y = gs->position.y.part.integer;
	
    cp.x = gs->position.x.part.integer -  160;
    cp.y = ~(gs->position.y.part.integer + 384);
	
	for (i = 0x15; i >= 0; --i) {			// 0x16 x 32 = 704 pixels
		_GSDrawScroll3A(gs, _GSCoordsScroll3(cp), _GSLookupScroll3(gs, cp), cp);
		cp.x += 32;
	}
}
