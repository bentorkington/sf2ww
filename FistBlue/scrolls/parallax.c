//
//  parallax.c
//  MT2
//
//  Created by Ben Torkington on 24/10/20.
//

#include "parallax.h"
#include "sf2.h"
#include "structs.h"
#include "lib.h"
#include "gemu.h"
#include "scroll.h"

extern Game g;
extern CPSGFXEMU gemu;
extern ScrollState gstate_Scroll2;

#ifdef REDHAMMER
#include <stdio.h>
#include <stdlib.h>
#endif


static void _GSUpdateRowScroll(RowScrollState *gs, short *a0, short *a1);

#define BUMP_2BE(increase)                                            \
    {                                                                \
        int offset = (g.x02be - gemu.RowScroll2) * sizeof(short);    \
        offset += (increase);                                        \
        offset &= 0xffff3000;                                        \
        g.x02be = gemu.RowScroll2 + (offset / sizeof(short));        \
    }                                                                \

#pragma mark Linescroll

#define LINESCROLL_SET_DEC(count)                \
for (i=0; i<(count); ++i) {                        \
*cursor++ = AccumOffset.part.integer;            \
AccumOffset.full -= Offset;                    \
}                                                \

#define LINESCROLL_SET_INC_BACK(count)            \
for (i=0; i<(count); ++i) {                        \
AccumOffset.full += Offset;                    \
*--cursor = AccumOffset.part.integer;            \
}                                                \

#define LINESCROLL_SET_DEC_BACK(count)            \
for (i=0; i<(count); ++i) {                        \
AccumOffset.full -= Offset;                    \
*--cursor = AccumOffset.part.integer;            \
}                                                \

#define LINESCROLL_SET(count)                    \
for (i=0; i<(count); ++i) {                        \
*cursor++ = AccumOffset.part.integer;            \
}                                                \

#define LINESCROLL_SET_BACK(count)                \
for (i=0; i<(count); ++i) {                        \
*--cursor = AccumOffset.part.integer;            \
}                                                \

#define LINESCROLL_SET_BACK_CONST(count,const)    \
for (i=0; i<(count); ++i) {                        \
*--cursor = const;                                \
}                                                \

#define LINESCROLL_INC(count);                    \
for    (i=0; i<(count); ++i) {                        \
AccumOffset.full += Offset;                    \
}                                                \



void TMMaintRowScroll(RowScrollState *ss) {    /* 84480 */
    short *a1;

    switch (ss->mode0) {
        case 0:
            /* 84496 */
            NEXT(ss->mode0);
            ss->CenterX    = 448;    // not actually zdepth, but Scr2X where parallax is zero
            // in other words, center of the stage
            ss->ss_0010         = 630;          // ~FP(0.0096) pixels per pixel per row
            ss->GroundRow       = 984 * 2;
            g.CPS.RowScrollBase = 0x9200;
#ifdef CPS
            g.x02be = (short *)0x921000;
#endif
#ifdef REDHAMMER
            g.x02be = gemu.RowScroll2;
            if (ss->planes) {
                free(ss->planes);
                ss->planes = NULL;
            }
            
            switch (g.CurrentStage) {
                case STAGE_INDIA_DHALSIM:
                    ss->planes = calloc(sizeof(RHTilePlane), 2);
                    if (!ss->planes) {
                        FBPanic(1);
                    }
                    ss->nPlanes = 2;
                    ss->planes[0] = (RHTilePlane){  0,  63, -24, 40 };
                    ss->planes[1] = (RHTilePlane){ 64, 256,  40, 40 };
                    break;
                default:
                    
                    /* XXX remove me */
                    ss->planes = calloc(sizeof(RHTilePlane), 2);
                    ss->planes[0] = (RHTilePlane){  0,  63, -24, 40 };
                    ss->planes[1] = (RHTilePlane){ 64, 256,  40, 40 };
                    break;
            }
#endif
            ss->x0024 = 0;
            /* returns BTST #0, %d1 why???  */
            break;
        case 2:
            /* 84546 */
            g.CPS.RowScrollBase = (g.CPS.RowScrollBase + 0x10) & 0xfff30;

            // XXX double buffer rowscroll
            //BUMP_2BE(0x1000);
            
            a1 = &ss->ss_001c[ss->x0024 / 4];
            // xxx lookup 84564 - 2e
            ss->x0024 = (ss->x0024 + 4) & 0xc;
            ss->position.y.part.integer = gstate_Scroll2.position.y.part.integer;
            g.CPS.RowScrollOffset = ss->position.y.part.integer;
            if (a1[0] != gstate_Scroll2.position.x.part.integer) {
                a1[0] = gstate_Scroll2.position.x.part.integer;
                _GSUpdateRowScroll(ss, g.x02be, a1);
            }
            break;
        FATALDEFAULT;
    }
}

/*!
 %a0: pointer to linescroll array
 %a1: pointer to position_x
 */
static void _GSUpdateRowScroll(RowScrollState *ss, short *a0, short *a1) { /* 84592 */
    int Offset;                    // %d0
    int i;
    FIXED16_16 AccumOffset;        // %d1
    short *cursor;              // %a2
    short *a3;          // %a3
    int d3;             // %d3 type should be u32
    
    Offset = ss->ss_0010 * (ss->CenterX - a1[0]);
    cursor = a0 + ((u32)ss->GroundRow / 2);
    AccumOffset.full = (SCREEN_WIDTH / 2) * GSTATE_PIXEL;
#ifdef REDHAMMER
    ss->RowOffset = Offset;
#endif
    
    switch (g.CurrentStage) {
        case STAGE_JAPAN_RYU:
            LINESCROLL_SET_DEC(12);
            LINESCROLL_SET(12);

            a3 = cursor;
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = (SCREEN_WIDTH / 2) * GSTATE_PIXEL;

            LINESCROLL_SET_INC_BACK(24);
            LINESCROLL_INC(16);
            d3 = AccumOffset.full;
            for (i=0; i<16; ++i) {
                d3 += Offset;
            }
            ss->Scroll3Parallax.full = d3;
            for (i=0; i<8; ++i) {
                d3 += Offset;
            }
            *a3 = d3;                   // a3 is the lowermost row, 256
            for (i=0; i<8; ++i) {
                d3 += Offset;
            }
            ss->Scroll1Parallax.full = d3;
            LINESCROLL_SET_BACK(208);
            break;
        case STAGE_JAPAN_EHONDA:
            LINESCROLL_SET_DEC(24);
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = (SCREEN_WIDTH / 2) * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(24);
            ss->Scroll1Parallax.part.integer = AccumOffset.part.integer;
            LINESCROLL_SET_INC_BACK(16);
            LINESCROLL_SET_BACK(16);
            
            LINESCROLL_SET_BACK_CONST(5, ss->Scroll1Parallax.part.integer);     // front of bath
            
            ss->Scroll3Parallax.full = AccumOffset.full;
            
            AccumOffset.full = ss->Scroll1Parallax.full;
            LINESCROLL_SET_INC_BACK(15);                // top of bath
            AccumOffset.full = ss->Scroll3Parallax.full;
            LINESCROLL_SET_BACK(92);
            LINESCROLL_SET_INC_BACK(64);
            break;
        case STAGE_BRAZIL_BLANKA:
            LINESCROLL_SET_DEC(24);
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = (SCREEN_WIDTH / 2) * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(24);
            ss->Scroll3Parallax.full = AccumOffset.full;
            LINESCROLL_INC(16);
            ss->Scroll1Parallax.full = AccumOffset.full;
            LINESCROLL_SET_BACK(208);
            break;
        case STAGE_USA_GUILE:
            LINESCROLL_SET_DEC(24);
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = (SCREEN_WIDTH / 2) * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(8);
            ss->Scroll3Parallax.full = AccumOffset.full;
            LINESCROLL_SET_INC_BACK(18);
            d3 = AccumOffset.full;
            for (i=0; i<16; ++i) {
                d3 += Offset;
            }
            ss->Scroll1Parallax.full = d3;
            LINESCROLL_SET_BACK(126);
            LINESCROLL_SET_DEC_BACK(80);
            break;
        case STAGE_USA_KEN:
            LINESCROLL_SET_DEC(16);
            ss->Scroll1Parallax.full = AccumOffset.full;
            LINESCROLL_SET(8);
            
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = (SCREEN_WIDTH / 2) * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(40);
            ss->Scroll3Parallax.full = AccumOffset.full;
            LINESCROLL_INC(16);
            LINESCROLL_SET_BACK(192);
            break;
        case STAGE_CHINA_CHUNLI:
            LINESCROLL_SET_DEC(24);
            ss->Scroll1Parallax.full = AccumOffset.full;
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = (SCREEN_WIDTH / 2) * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(40);
            ss->Scroll3Parallax.full = AccumOffset.full;
            LINESCROLL_SET_BACK(192);
            break;
        case STAGE_USSR_ZANGIEF:
            LINESCROLL_SET_DEC(24);
            ss->Scroll1Parallax.full = AccumOffset.full;
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = 192 * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(34);
            d3 = AccumOffset.full;
            for (i=0; i<16; ++i) {
                d3 += Offset;
            }
            ss->Scroll3Parallax.full = d3;
            LINESCROLL_SET_BACK(192);
            break;
        case STAGE_INDIA_DHALSIM:
#ifdef CPS
            AccumOffset.full = 192 * GSTATE_PIXEL;
            LINESCROLL_SET_DEC(24);
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = 192 * GSTATE_PIXEL;
            ss->ss_0014.full = AccumOffset.full;
            LINESCROLL_SET_INC_BACK(24);
            ss->ss_0018.full = AccumOffset.full;
            LINESCROLL_SET_INC_BACK(16);
            LINESCROLL_SET_BACK(192);
#else
            ss->Scroll1Parallax.full = 192 * GSTATE_PIXEL;
            ss->Scroll3Parallax.full = 192 * GSTATE_PIXEL + (24 * Offset);
#endif
            break;
        case STAGE_THAILAND_BISON:
            LINESCROLL_SET_DEC(24);
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = 192 * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(8);
            ss->Scroll1Parallax.full = AccumOffset.full;
            LINESCROLL_SET_INC_BACK(16);
            ss->Scroll3Parallax.full = AccumOffset.full;
            LINESCROLL_SET_INC_BACK(16);
            d3 = AccumOffset.full;
            for (i=0; i<32; ++i) {
                d3 += Offset;
            }
            LINESCROLL_SET_BACK(128);
            AccumOffset.full = d3;
            LINESCROLL_SET_DEC_BACK(64);
            break;
        case STAGE_THAILAND_SAGAT:
            LINESCROLL_SET_DEC(24);
            ss->Scroll3Parallax.full = AccumOffset.full;
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = 192 * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(24);
            d3 = AccumOffset.full;
            for (i=0; i<16; ++i) {
                d3 += Offset;
            }
            ss->Scroll1Parallax.full = d3;
            LINESCROLL_SET_BACK(208);
            break;
        case STAGE_USA_BALROG:
            LINESCROLL_SET_DEC(24);
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = 192 * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(13);
            LINESCROLL_SET_BACK(11);            // Light bulbs at edge of floor
            LINESCROLL_INC(20);
            ss->Scroll3Parallax.full = AccumOffset.full;
            LINESCROLL_INC(20);
            d3 = AccumOffset.full;
            for (i=0; i<36; ++i) {
                d3 += Offset;
            }
            ss->Scroll1Parallax.full = d3;
            LINESCROLL_SET_BACK(192);
            break;
        case STAGE_SPAIN_VEGA:
            LINESCROLL_SET_DEC(24);
            cursor = a0 + ((u32)ss->GroundRow / 2);
            AccumOffset.full = 192 * GSTATE_PIXEL;
            LINESCROLL_SET_INC_BACK(4);
            ss->Scroll1Parallax.full = AccumOffset.full;
            LINESCROLL_SET_INC_BACK(12);
            LINESCROLL_SET_BACK(59);
            LINESCROLL_INC(16);
            ss->Scroll3Parallax.full = AccumOffset.full
            LINESCROLL_INC(16);
            LINESCROLL_SET_BACK(149);
            break;
        case 12:
        case 13:
        case 14:
        case 15:
            /* do nothing */
            break;
        FATALDEFAULT;
    }
//    printf("x %4d skew %4d floor %f wall %f\n",
//           *a1,
//           ss->ss_0010,
//           (float)ss->ss_0014.full / 0x10000,
//           (float)ss->ss_0018.full / 0x10000);
}
