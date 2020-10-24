//
//  scroll.h
//  MT2
//
//  Created by Ben Torkington on 24/10/20.
//

#ifndef scroll_h
#define scroll_h

#include "sf2.h"

#define GSTATE_PIXEL    (1 << 16)             // fixed precision 1.0

typedef struct Point16 CP;

typedef struct {
    u16 exists;
    u8  mode0;
    u8  mode1;
    /* unused? */

    FIXED16_16  X;      /* Fixed precision world->display offsets */
    FIXED16_16  Y;      /* 0xa-d */
    u16 SpecialStage;          /* either 0 or 2, 2 for bonus stage? */
    u16 gs_0010;
    /// host address pointer to tilemaps
    u32 TileMaps;
    u16 Offset;       /* offset  */
    u16 OffMask;      /* bitmask */
    u16 gs_001a;
    u16 gs_001c;
    u8  x001e;
    u8  x001f;
    u16 Index;      /* 0x0020 current offset */
    u16 InitialIndex;
    u8  x0024;        /* used with x0025 as U16 */
    u8  x0025;
    u16 XCoarse;    /* x0026 number of columns skipped in draw routine */
    u16 YCoarse;    /* x0028 */
    u8  XUpdateMethod, YUpdateMethod;
    u16 XOff, YOff;
} ScrollState;

typedef struct {
    u16 exists;            // ff8cd2
    u8  mode0;
    u8  mode1;
    /* unused? */
    
    FIXED16_16  X;      /* Fixed precision world->display offsets */
    FIXED16_16  Y;      /* 0xa-d */
    u16 CenterX;          /* the zero-parallax x coordinate */
    u16 ss_0010;        /* amount of parallax skew per pixel off-center */
    u16 GroundRow;
    FIXED16_16 Scroll1Parallax; // 0x14
    FIXED16_16 Scroll3Parallax;    // 0x18
    short ss_001c[4];    // ff8cee

    u16  x0024;        /* used with x0025 as U16 */
    u16 XCoarse;    /* x0026 number of columns skipped in draw routine */
    u16 YCoarse;    /* x0028 */
    u8  XUpdateMethod, YUpdateMethod;
    u16 XOff, YOff;    /* 0x30, 0x32 */
#ifdef REDHAMMER
    int nPlanes;
    RHTilePlane *planes;
    int RowOffset;
#endif
} RowScrollState;

void TMGotoCenter(void);
void TMUpdateScrollsAttractSequence(void);
void TMSetupScroll2(ScrollState *gs);
void TMSetupScroll3(ScrollState *gs);
void TMUpdate(void);
void TMInitForStage(void);

#endif /* scroll_h */
