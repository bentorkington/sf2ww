//
//  scroll_maint.c
//  MT2
//
//  Created by Ben Torkington on 24/10/20.
//

#include "scroll_maint.h"
#include "sf2.h"
#include "structs.h"
#include "gstate.h"
#include "lib.h"
#include "scroll_data.h"
#include "gstate.h"
#include "gemu.h"

extern ScrollState gstate_Scroll1;
extern ScrollState gstate_Scroll2;
extern ScrollState gstate_Scroll3;
extern RowScrollState gstate_RowScroll;    /* cd2 - d51 */
extern CPSGFXEMU gemu;

#pragma mark ---- Scroll Maint ----

extern Game g;

static void _gstate_against_right(ScrollState *gstate, int d1) {         // sf2ua:832f2
    int d3;
    /* missing GPCollDetect redundant */
    if (d1 > 6) {
        d1 = 6;
    }
    d3 = gstate->XPI;
    gstate->XPI += d1;
    if (gstate->XPI > g.StageMaxX ) {
        gstate->XPI = g.StageMaxX;
        gstate->XOff = g.StageMaxX - d3;
    } else {
        gstate->XOff = d1;
    }
    gstate->x0024 = 4;
}

static void _gstate_against_left(ScrollState *gstate, int d0) {        // sf2ua:83334   d0 is negative
    int d3;
    /* missing GPCollDetect redundant */
    if (d0 < -6) {
        d0 = -6;
    }
    d3 = gstate->XPI;
    gstate->XPI += d0;
    if (gstate->XPI < g.StageMinX ) {
        gstate->XPI = g.StageMinX;
        gstate->XOff = g.StageMinX - d3;
    } else {
        gstate->XOff = d0;
    }
    gstate->x0024 = 0xc;
}

static void _GSMaintScroll3X(ScrollState *gs) {        // 83658
    gs->x0024 = gstate_Scroll2.x0024;
    switch (gs->XUpdateMethod) {
        case SCR3X_PERSP:        // scroll 2 + Zdepth
            gs->XPI = gstate_Scroll2.XPI + (gstate_RowScroll.Scroll3Parallax.part.integer - (SCREEN_WIDTH / 2));
            break;
        case SCR3X_NONE:
            // do nothing
            break;
        FATALDEFAULT;
    }
}

static void _GSMaintScroll3Y(ScrollState *gs) {        // 8368c
    if (g.ScreenWobbleMagnitude) {
        return;
    }
    gs->x0025 = gstate_Scroll2.x0025;
    switch (gs->YUpdateMethod) {
        case 0x2:
            gs->YPI = gstate_Scroll2.YPI;
            break;
        case 0x0:
            break;
        case 0x4:
            gs->Y.full += gstate_Scroll2.YOff * 0xc000 ; /* 0.75 */
            break;
        case 6:
            gs->Y.full += gstate_Scroll2.YOff * 0x8000;  /* 0.5 */
            break;
        case 8:
            gs->Y.full += gstate_Scroll2.YOff * 0xe000;  /* 0.875 */
            break;
        case 10:
            gs->Y.full += gstate_Scroll2.YOff * 0x14000; /* 1.25 */
            break;
        case 12:
            gs->YPI += gstate_Scroll2.YPI - gs->YOff;
            gs->YOff = 0;
            break;
    }
}

static void update_scroll2_X (ScrollState *gstate) {     /* 0x83270 */
    short leftmost_x, rightmost_x;
    short d0;
    Player *left, *right;
    switch (gstate->XUpdateMethod) {
        case SCR2X_FIGHT:
            if (g.Player1.XPI <= g.Player2.XPI) {
                leftmost_x    = g.Player1.XPI;
                rightmost_x   = g.Player2.XPI;
                left  = PLAYER1;
                right = PLAYER2;
            } else {
                leftmost_x    = g.Player2.XPI;
                rightmost_x   = g.Player1.XPI;
                left  = PLAYER2;
                right = PLAYER1;
            }
            leftmost_x  -= left ->Size;
            rightmost_x += right->Size;

            if ((rightmost_x - leftmost_x) < 256) {
                if (rightmost_x - gstate->XPI  >= (SCREEN_WIDTH - 64)) {
                    _gstate_against_right(gstate,rightmost_x - gstate->XPI - (SCREEN_WIDTH - 64));
                } else if (leftmost_x - gstate->XPI < 64) {
                    _gstate_against_left(gstate,leftmost_x - gstate->XPI - 64);
                }
            } else if ((rightmost_x - leftmost_x) < SCREEN_WIDTH) {             // XXX checkme
                d0=(left->XPI - left->Size + right->XPI + right->Size)/2;
                d0-=gstate->XPI;
                d0-= SCREEN_WIDTH / 2;
                if (d0<0) {
                    _gstate_against_left(gstate, d0);
                } else {
                    _gstate_against_right(gstate, d0);
                }
            }
            break;
        case SCR2X_NONE:
            /* empty method, nothing happens */
            break;
        FATALDEFAULT;
    }
}
static void update_scroll2_Y (ScrollState *gstate) { /* 0x3=83376 */
    switch (gstate->YUpdateMethod) {
        case 0:
            if (g.x0ade || g.ScreenWobbleMagnitude) { return; }
            
            /* XXX not done */
            
            break;
        case 2:
            // do nothing
            break;
        FATALDEFAULT;
    }
}

#define QUARTER_PIXEL 0x4000


static void _GSMaintScroll1X(ScrollState *gs) {    // 834d0
    short int temp;
    
    gs->x0024 = gstate_Scroll2.x0024;
    switch (gs->XUpdateMethod) {
        case SCR1X_PERSPECTIVE:         // follow parallax from linescroll
            temp = gs->X.part.integer;
            gs->XPI = gstate_Scroll2.XPI + gstate_RowScroll.Scroll1Parallax.part.integer - (SCREEN_WIDTH / 2);
            g.x8b14 = gs->XPI - temp;
            break;
        case SCR1X_SKY:
            gs->X.full += QUARTER_PIXEL;        /* plus 0.25 */
            if(gs->mode1 == 0 && gs->XPI >= 0x200) {
                gs->mode1 += 2;
                gs->YPI += 0x100;
            }
            gs->x0024 = 4;
            gs->XPI  -= g.x8c02;
            break;
        case SCR1X_SKY2:
            gs->X.full += QUARTER_PIXEL;
            gs->x0024   = 4;
            gs->XPI -= g.x8c02;
            break;
        case SCR1X_NONE:           /* does nothing */
            break;
        FATALDEFAULT;
    }
}


static void _GSMaintScroll1Y(ScrollState *gstate) {    /* 83558 */
    if(g.ScreenWobbleMagnitude != 0)  {return;}
    
    gstate->x0025 = gstate_Scroll2.x0025;
    switch (gstate->YUpdateMethod) {
        case SCR1Y_FOLLOW_SCR2:
            gstate->YPI = gstate_Scroll2.YPI;
            break;
        case SCR1Y_NONE:
            break;
        case 4:
            gstate->Y.full += gstate_Scroll2.YOff * 128;   /* XXX all need <<4 */
            break;
        case 6:
            gstate->Y.full += gstate_Scroll2.YOff * 320;
            break;
        case 8:
            gstate->Y.full += gstate_Scroll2.YOff * 288;
            break;
        case 10:
            gstate->Y.full += gstate_Scroll2.YOff * 224;
            break;
        case 12:
            gstate->Y.full += gstate_Scroll2.YOff * 160;
            break;
        case 14:
            gstate->YPI += gstate_Scroll2.YOff;
            break;
    }
}


void GSMaintScroll3(ScrollState *gs) {   /* 835ec */
    switch (gs->mode0) {
        case 0:
            switch (gs->mode1) {
                case 0:
                    NEXT(gs->mode1)
                    gs->x001e = gs->x001f = gs->x0024 = gs->x0025 = 0;
                    break;
                case 2:
                    NEXT(gs->mode1)
                    GSFillScroll3(gs);
                    break;
                case 4:
                    NEXT(gs->mode1)
                    break;
                case 6:
                    NEXT(gs->mode0)
                    gs->mode1 = 0;
                    break;
            }
            break;
        case 2:
            _GSMaintScroll3X(gs);
            _GSMaintScroll3Y(gs);
            g.CPS.Scroll3X = gs->XPI;
            g.CPS.Scroll3Y = gs->YPI;
            break;
    }
}

#define SCROLL_MAINT_INIT   0
#define SCROLL_MAINT_NORM   2

void GSMaintScroll2(ScrollState *gstate){      /* 831ca was nextlevel_dosetups */
    switch (gstate->mode0) {
        case SCROLL_MAINT_INIT:
            switch (gstate->mode1) {
                case 0x0:
                    gstate->mode1 +=2 ;
                    GSStageScroll1(g.CurrentStage);
                    GSStageScroll2(g.CurrentStage);
                    GSStageScroll3(g.CurrentStage);
                    
                    GSInitDimensions();
                    GSInitUpdateMethods();
                    GSInitDispEna();
                    
                    gstate-> x001e = 0;
                    gstate-> x001f = 0;
                    gstate-> x0024 = 0;
                    gstate-> x0025 = 0;
                    g.x8a4b = 0;
                    /* CPS code ends with a BTST !? 0x83222 */
                    break;
                case 0x2:
                    gstate->mode1 += 2;
                    break;
                case 0x4:
                    gstate->mode1 += 2;
                    GSFillScroll2(gstate); /*  83ae0 fill scroll2 tilemap from ROM */
                    break;
                case 0x6:
                    gstate->mode0 += 2;
                    gstate->mode1  = 0;
                    g.GSInitComplete = TRUE;
                    break;
            }
            break;
        case SCROLL_MAINT_NORM:
            gstate->XOff = 0;
            gstate->YOff = 0;
            update_scroll2_X(gstate);   /*   sub_83270 player horiz scrolling */
            update_scroll2_Y(gstate);        /* sub_83376 same for vert */
//            if(g.OnBonusStage) {    // XXX
              if(1) {
                g.CPS.Scroll2X = gstate->XPI;
            } else {
                // compensate for Rowscroll
                g.CPS.Scroll2X = gstate->XPI - (SCREEN_WIDTH / 2);
            }
            g.CPS.Scroll2Y = gstate->YPI;
    }
}

#pragma mark ---- Coordinate Calculators ----

static u16 *_GSCoordsScroll1(CP cp){            //83dec checked
    return gemu.Tilemap_Scroll1[
            ((cp.y & 0x100) << 3) +            /*      yxxx x00y yy00   */
            ((cp.y & 0xe0) >> 3)  +
            ((cp.x << 2) & 0x0780)
    ];
}

static CP _GSCoordOffsetScr1 (ScrollState *gs, short offset) {    // 83d68 checked
    CP cp;
    static const short data_83d84[4][2] = {
        {   -64,   352,  },
        {   416,   352,  },
        {   -64,  -128,  },
        {   -64,   352,  },
    };
    
    offset &= 0xf;
    cp.x =   gs->XPI + data_83d84[offset/4][0];
    cp.y = ~(gs->YPI + data_83d84[offset/4][1]);
    
    return cp;
}

static void _GSInitScroll1(ScrollState *gstate) {    /* 83b78 checked init scroll1 */
    CP cp;
    short i;
    const u16 *a1;
    u16 *gfx_p;
    
    g.CPS.Scroll1X = gstate->XPI;
    g.CPS.Scroll1Y = gstate->YPI;
    
    cp.x = gstate->XPI - 0x40;
    cp.y = ~(gstate->YPI + 0x160);
    
    for (i=0; i<16; ++i) {
        gfx_p = _GSCoordsScroll1(cp);
        a1 = _GSLookupScroll1(gstate, cp);
        GSDrawScroll1B(gstate, gfx_p, a1, cp);
        cp.x += 0x20;
    }
}

static void gstate_update_scroll1 (ScrollState *gs) {            //83498
    CP cp;
        
    g.CPS.Scroll1X = gs->XPI;
    g.CPS.Scroll1Y = gs->YPI;
    
    if((gs->XPI & 0x20) ^ gs->x001e) {        // does it need refilling?
        gs->x001e ^= 0x20;
    
        cp = _GSCoordOffsetScr1(gs, gs->x0024);
        GSDrawScroll1B(gs, _GSCoordsScroll1(cp), _GSLookupScroll1(gs, cp), cp);
    }
}

void GSMaintScroll1(ScrollState *gs) {        // 8343a
    switch (gs->mode0) {
        case 0:
            switch (gs->mode1) {
                case 0:
                    NEXT(gs->mode1)
                    _GSInitScroll1(gs);
                    break;
                case 2:
                    NEXT(gs->mode1)
                    gs->x001e = gs->x001f = gs->x0024 = gs->x0025 = 0;
                    gstate_update_scroll1(gs);
                    break;
                case 4:
                    NEXT(gs->mode1)
                    break;
                case 6:
                    NEXT(gs->mode0)
                    gs->mode1 = 0;
                    break;
                FATALDEFAULT;
            }
            break;
        case 2:
            _GSMaintScroll1X(gs);
            _GSMaintScroll1Y(gs);
            gstate_update_scroll1(gs);
            break;
        FATALDEFAULT;
    }
}

