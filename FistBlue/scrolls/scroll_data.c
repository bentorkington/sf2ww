//
//  scroll_data.c
//  MT2
//
//  Created by Ben Torkington on 24/10/20.
//

#include "scroll_data.h"
#include "scroll.h"
#include "gemu.h"
#include "stagegfx.h"
#include "sf2.h"
#include "structs.h"

#ifdef REDHAMMER
#include "redhammer.h"
#endif

extern CPSGFXEMU gemu;
extern ScrollState gstate_Scroll1;
extern ScrollState gstate_Scroll2;
extern ScrollState gstate_Scroll3;
extern Game g;

const u16 *skyscraper_realign(ScrollState *gs, u16 **gfx_p) {            // 84384
    u32 d0;
    u32 d1;
    int offset;
    
#ifdef CPS
#warning Scroll cursor arithmetic not optimal for CPS
#endif
    *gfx_p -= 0x20;
    offset = (*gfx_p - BMAP_SCROLL2) * sizeof(u16);
    d0 = offset & 0xfffff000;
    d1 = (offset + 0x40) & 0xfff;
    d0 |= d1;
    *gfx_p = (u16 *)BMAP_SCROLL2 + (d0 / sizeof(u16));
    
    gs->InitialIndex = ((gs->InitialIndex + 2) & gs->gs_001a) | (gs->InitialIndex & gs->gs_001c);
    return &data_e0000[ RHWordOffset(gs->TileMaps, gs->InitialIndex / 2) ][gs->YCoarse/2];
}

//void decode_scr3_coord(int offset) {
//    int x = (offset & 0x3e0) >> 5;
//    int y = ((offset & 0x1c00) >> 8) + ((offset & 0x1c) >> 2);
//    printf("X:%d Y:%d ",x, y);
//}

const u16 *realign_scr3a(ScrollState *gs, u16 **gfx_p) {
    // 84178 for realigning scroll3 cursor
    u32 d0;
    u32 d1;
    int offset;
#ifdef CPS
#warning Scroll cursor arithmetic not optimal for CPS
#endif
    *gfx_p -= 2;
    offset = (*(gfx_p) - BMAP_SCROLL3) * sizeof(u16);
    d0 = offset & 0xffffc7e3;
    d1 = (offset + 0x800) & 0x3800;
    d0 |= d1;
    *gfx_p = (u16 *)BMAP_SCROLL3 + (d0 / sizeof(u16));
    
    gs->Index = (gs->Index + gs->Offset) & gs->OffMask;
    return &data_d8000[ RHWordOffset(gs->TileMaps, gs->Index / 2) ][ gs->XCoarse/2 ];
}


static const u16 *realign_scr3b(ScrollState *gs, u16 **gfx_p) {        // 8442a for scroll3
    u32 d0;
    u32 d1;
    int offset;
#ifdef CPS
#warning Scroll cursor arithmetic not optimal for CPS
#endif
    *gfx_p -= 16;
    offset = (*gfx_p - BMAP_SCROLL3) * sizeof(u16);
    d0 = offset & 0xfffff800;
    d1 = (offset + 0x20) & 0x7ff;
    d0 |= d1;
    *gfx_p = (u16 *)BMAP_SCROLL3+(d0 / sizeof(u16));
    
    gs->Index = (gs->Index + gs->Offset) & gs->OffMask;
    
    return &data_d8000[ RHWordOffset(gs->TileMaps, gs->Index / 2) ][ gs->XCoarse/2 ];
}

// u16       yxxx xxxy yyyy[2]        2y x 64y x 32y x 2words x u16  tiles 8x8
// void 00yx xxxx xyyy yy00            rowmask 0x1f80
static const u16 *_GSRealignScr1a(ScrollState *gs, u16 **gfx_p) {    // 84052 checked
    int element1, element2;
    
    *gfx_p -= 8;
    
    element2 = element1 = (*gfx_p - gemu.Tilemap_Scroll1[0]) * sizeof(u16);
    //element2 = *gfx_p;
    
    element1 &= 0xcf80;    // top of this column
    element2 += 0x2000;
    element2 &= 0x3000;
    
    *gfx_p = gemu.Tilemap_Scroll1[0] + ((element1 | element2) / sizeof(u16));
    
    gs->Index = (gs->Index + gs->Offset) & gs->OffMask;
    return &data_d6800[ RHWordOffset(gs->TileMaps, gs->Index / 2) ][ gs->XCoarse/2 ];
}

static const u16 *_GSRealignScroll2A(ScrollState *gs, u16 **gfx_p) {
    // 840e4 scroll2
    u32 d0;
    u32 d1;
    int offset;
    
#ifdef CPS
#warning Scroll cursor arithmetic not optimal for CPS
#endif
    *gfx_p -= 2;
    offset = (*gfx_p - BMAP_SCROLL2) * sizeof(u16);
    d0 = offset & 0xffffcfc3;
    d1 = (offset + 0x1000) & 0x3000;
    d0 |= d1;
    *gfx_p = (u16 *)BMAP_SCROLL2 + (d0 / sizeof(u16));
    
    gs->Index = (gs->Index + gs->Offset) & gs->OffMask;
    return &data_e0000[ RHWordOffset(gs->TileMaps, gs->Index / 2) ][gs->XCoarse/2];
}

#pragma mark ---- Tilemap Lookup ----

static short _GSCalcBlockIndex(ScrollState *gs, CP cp) {
    /* 0x83ee6 x and y div by 256 was cacl_gstate_2022 */
    short index;
    if (gs->SpecialStage) {
        index = ((cp.y & 0x700) >> 4) + ((cp.x & 0x700) >> 7);    // 0YYY XXX0
    } else {
        index = ((cp.y & 0x100) >> 5) + ((cp.x & 0x300) >> 7);    // YXX0
    }
    gs->Index        = index;
    gs->InitialIndex = index;
    return RHWordOffset(gs->TileMaps, index / 2);// gs->TileMaps[index/2];
}

const u16 *_GSLookupScroll1(ScrollState *gs, CP cp) {    /* 83e5c */
    /* No ATTRs */
    const u16 *temp2;
        
    temp2 = data_d6800[ _GSCalcBlockIndex(gs, cp) ];

    /* xxx yyy0 64 vctors  */
    gs->YCoarse = (cp.y & 0xe0) >> 4;
    gs->XCoarse = (cp.x & 0xe0) >> 1;

    temp2 += (gs->YCoarse + gs->XCoarse)/2;
    return temp2;
}

const u16 *_GSLookupScroll2(ScrollState *gs, CP cp) {    /* 83e8a */
    const u16 *temp2;
    
    temp2 = data_e0000[_GSCalcBlockIndex(gs, cp)];
    
    /* xx xxyy yy00  256 tiles + attr in u16 */
    gs->YCoarse = (cp.y & 0xf0) >> 2;
    gs->XCoarse = (cp.x & 0xf0) << 2;
    
    
    temp2 += (gs->YCoarse + gs->XCoarse)/2;
    return temp2;
}

const u16 *_GSLookupScroll3(ScrollState *gs, CP cp) {    /* 83eba */
    const u16 *temp2;
    
    temp2 = data_d8000[_GSCalcBlockIndex(gs, cp)];
    gs->YCoarse = (cp.y & 0xe0) >> 3;
    gs->XCoarse = (cp.x & 0xe0);        /* xxxy yy00 */
    temp2 += (gs->YCoarse + gs->XCoarse)/2;
    return temp2;
}


void GSStageScroll1 (short d0) {    /* 83730 */
    int bonus;                    /* not actually bonus, but VS Screen, etc */
    //bonus = data_8375c[d0];
    bonus = d0 >= 0x10 ? TRUE : FALSE;        /* more compact than a U16 LUT */
    
    gstate_Scroll1.SpecialStage = bonus * 2;
    gstate_Scroll1.Offset  = RH2DWord(0x83834, 4, bonus / 2, 0); /* 0x8 or 0x10 */
    gstate_Scroll1.OffMask = RH2DWord(0x83834, 4, bonus / 2, 1); /* 0xe or 0x7e */
    gstate_Scroll1.gs_001a = RH2DWord(0x83834, 4, bonus / 2, 2); /* 0x6 or 0xe */
    gstate_Scroll1.gs_001c = RH2DWord(0x83834, 4, bonus / 2, 3); /* 0x8 or 0x70 */

    gstate_Scroll1.TileMaps = RH1DLong(0x89ebc, d0);
}
void GSStageScroll2(short d0) {
    short bonus;        /* not actually bonus but special stages */
    //bonus = data_837b4[g.CurrentStage];     /* same as 0x8375c */
    bonus = d0 >= 0x10 ? 2: 0;

    gstate_Scroll2.SpecialStage = bonus;
    gstate_Scroll2.Offset  = RH2DWord(0x83834, 4, bonus / 2, 0); /* 0x8 or 0x10 */
    gstate_Scroll2.OffMask = RH2DWord(0x83834, 4, bonus / 2, 1); /* 0xe or 0x7e */
    gstate_Scroll2.gs_001a = RH2DWord(0x83834, 4, bonus / 2, 2); /* 0x6 or 0xe */
    gstate_Scroll2.gs_001c = RH2DWord(0x83834, 4, bonus / 2, 3); /* 0x8 or 0x70 */

    
    gstate_Scroll2.TileMaps = RH1DLong(0x89f0c, d0);
}

void GSStageScroll3(short d0) {    /* 837e0 */
    int bonus;
    bonus = d0 >= 0x10 ? 2 : 0 ;

    gstate_Scroll3.SpecialStage = bonus;        // not actually bonus but scrolldisable
    gstate_Scroll3.Offset  = RH2DWord(0x83834, 4, bonus / 2, 0); /* 0x8 or 0x10 */
    gstate_Scroll3.OffMask = RH2DWord(0x83834, 4, bonus / 2, 1); /* 0xe or 0x7e */
    gstate_Scroll3.gs_001a = RH2DWord(0x83834, 4, bonus / 2, 2); /* 0x6 or 0xe */
    gstate_Scroll3.gs_001c = RH2DWord(0x83834, 4, bonus / 2, 3); /* 0x8 or 0x70 */
    
    gstate_Scroll3.TileMaps = RH1DLong(0x89f5c, d0);
}

/*
static void gstate_nextlevel_scroll1 (void) {   // 0x8372c
    _GSStageScroll1(g.CurrentStage);
}
static void gstate_nextlevel_scroll2 (void) {    // 0x83784
    _GSStageScroll2(g.CurrentStage);
}
static void gstate_nextlevel_scroll3 (void) {
    _GSStageScroll3(g.CurrentStage);
}
 */


#pragma mark ---- Tilemap Fillers ----
void GSFillScroll2(ScrollState *gs) {  /* 0x83ae0 fill scroll2 from tilemap */
    int i,j;
    CPSCOORD gfx_p;
    
    g.CPS.Scroll2X = gs->position.x.part.integer - (SCREEN_WIDTH / 2);
    g.CPS.Scroll2Y = gs->position.y.part.integer;
    SCR2_CURSOR_SET(gfx_p, 0, 32);         /* starting at tile 0x800:
                              0x906000 = 0x904000 + (0x800 * 4) */
    
    for (i=0; i<8; i++) {
        for (j=0; j<256; j++) {
            SCR2_DRAW_TILE(
                gfx_p,
                data_e0000[ RHWordOffset(gs->TileMaps, i) ][j*2],
                data_e0000[ RHWordOffset(gs->TileMaps, i) ][(j*2)+1]
            );
            SCR2_CURSOR_BUMP(gfx_p, 0, 1);
        }
    }
}

void GSFillScroll3(ScrollState *gs) {        /* 0x83b2a fill scroll3 from tilemap */
    int i,j;
    CPSCOORD gfx_p;
    
    g.CPS.Scroll3X = gs->position.x.part.integer;
    g.CPS.Scroll3Y = gs->position.y.part.integer;
    
    SCR3_CURSOR_SET(gfx_p, 0, 24);
    
    for (i=0; i<4; i++) {
        for (j=0; j<64; j++) {
            SCR3_DRAW_TILE(gfx_p, data_d8000[ RHWordOffset(gs->TileMaps, i) ][ j*2 ],data_d8000[ RHWordOffset(gs->TileMaps, i) ][ (j*2) +1]);
            SCR3_CURSOR_BUMP(gfx_p, 0, 1);
        }
    }
        
    SCR3_CURSOR_SET(gfx_p, 0, 56);         /* starting at tile y=56 */

    for(i=4;i<8; i++) {
        for(j=0; j<64; j++) {
            SCR3_DRAW_TILE(gfx_p, data_d8000[ RHWordOffset(gs->TileMaps, i) ][ j*2 ], data_d8000[ RHWordOffset(gs->TileMaps, i) ][ (j*2)+1 ]);
            SCR3_CURSOR_BUMP(gfx_p, 0, 1);
        }
    }
}

static void _GSDrawScroll1A(const u16 **tilep_a1, u16 **gfx_p_a0, int numrows_d0) {        /* 83f70 */
    int x,y,youter;
    u16 *target;
    
    const u8 *cursor;
    for(youter=0;youter<=numrows_d0; youter++) {
        cursor = data_d0000[ (*tilep_a1)[youter]];
        for (x=0; x<4; x++) {
            for(y=0;y<4; y++) {        /* unrolled */
                target = *gfx_p_a0 + (y * 2) + (x * 64);
                SCR1_DRAW_TILE(target, (cursor[0] << 8) + cursor[1], cursor[2] );
                cursor += 3;
            }
        }
        *gfx_p_a0 += 8;
    }
}

void GSDrawScroll1B(ScrollState *gs, u16 *gfx_p, const u16 *a1, CP cp) {  /* 83f40 */
    int d3;
    int d0;
    
    d0 = ((~cp.y) & 0xe0) >> 5;
    d3 = d0;
    _GSDrawScroll1A(&a1, &gfx_p, d0);
    
    a1 = _GSRealignScr1a(gs, &gfx_p);
    d0 = 14 - d3;
    if(d0 <= 7) {
        _GSDrawScroll1A(&a1, &gfx_p, d0);
    } else {
        d3 = d0;
        _GSDrawScroll1A(&a1, &gfx_p, 7);
        a1 = _GSRealignScr1a(gs, &gfx_p);
        _GSDrawScroll1A(&a1, &gfx_p, d3-8);
    }
}

void GSDrawScroll2A(ScrollState *gs, u16 *gfx_p, const u16 *tilep, CP cp) {  /* 840a4 */
    short d0 = (cp.y & 0xf0) >> 4;
    short d3 = d0;
    for (; d0>0; d0--) {            /* inlined 840dc */
        *gfx_p++ = *tilep++;
        *gfx_p++ = *tilep++;
    }
    tilep = _GSRealignScroll2A(gs, &gfx_p);
    for (d0=0xf; d0>0; d0--) {            /* inlined 840dc */
        *gfx_p++ = *tilep++;
        *gfx_p++ = *tilep++;
    }
    tilep = _GSRealignScroll2A(gs, &gfx_p);
    d3 += 0x10;
    d0 = 0x20 - d3;
    if (d0 > 0xf) {
        for (; d0>0; d0--) {            /* inlined 840dc */
            *gfx_p++ = *tilep++;
            *gfx_p++ = *tilep++;
        }
    } else {
        for (d0 = 0xf; d0 > 0; d0--) {            /* inlined 840dc */
            *gfx_p++ = *tilep++;
            *gfx_p++ = *tilep++;
        }
        tilep = _GSRealignScroll2A(gs, &gfx_p);
        *gfx_p++ = *tilep++;
        *gfx_p++ = *tilep++;
    }
}




static void _GSDrawScroll3B(ScrollState *gs, u16 *gfx_p, const u16 *tilep, CP cp) {  /* 843dc was funky3_draw*/
    short d0 = (cp.y & 0xe0) >> 5;    // blocks of 32
    short d2 = d0;
    for (; d0 >= 0; --d0) {
        gfx_p[0] = tilep[0];
        gfx_p[1] = tilep[1];
        gfx_p += 0x10;
        tilep += 0x10;
    }
    tilep = realign_scr3b(gs, &gfx_p);
    for (d0 = 0; d0 < 8; ++d0) {
        gfx_p[0] = tilep[0];
        gfx_p[1] = tilep[1];
        gfx_p += 0x10;
        tilep += 0x10;
    }
    tilep = realign_scr3b(gs, &gfx_p);

    d0 = 0x15 - d2 - 9;
    if (d0 < 8) {
        for (; d0>0; d0--) {
            gfx_p[0] = tilep[0];
            gfx_p[1] = tilep[1];
            gfx_p += 0x10;
            tilep += 0x10;
        }
    } else {
        d2 = d0;
        for (d0 = 0; d0 < 8; ++d0) {
            gfx_p[0] = tilep[0];
            gfx_p[1] = tilep[1];
            gfx_p += 0x10;
            tilep += 0x10;
        }
        
        tilep = realign_scr3b(gs, &gfx_p);
        for (d0 = d2 - 8; d0 >= 0; --d0) {
            gfx_p[0] = tilep[0];
            gfx_p[1] = tilep[1];
            gfx_p += 0x10;
            tilep += 0x10;
        }
    }
}
