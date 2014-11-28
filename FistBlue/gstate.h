/* gstate.h */
#ifndef INC_GSTATE
#define INC_GSTATE

#include "sf2types.h"

struct coordpair {
    short x;
    short y;
};

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
    const u16 *TileMaps;      /* ptr to tilemap */
    u16 Offset;       /* offset  */
    u16 OffMask;      /* bitmask */
    u16 gs_001a;
    u16 gs_001c;
    u8  x001e;
    u8  x001f;
    u16 Index;      /* 0x0020 current offset */
    u16 InitialIndex;
    u8  x0024;		/* used with x0025 as U16 */
    u8  x0025;
    u16 XCoarse;    /* x0026 number of columns skipped in draw routine */
    u16 YCoarse;    /* x0028 */
    u8  XUpdateMethod, YUpdateMethod;
    u16 XOff, YOff;
} GState;

typedef struct {
    u16 exists;			// ff8cd2
    u8  mode0;
    u8  mode1;
    /* unused? */
	
    FIXED16_16  X;      /* Fixed precision world->display offsets */
    FIXED16_16  Y;      /* 0xa-d */
    u16 CenterX;          /* the zero-parallax x coordinate */
    u16 ss_0010;        /* amount of parallax skew per pixel off-center */
    u16 GroundRow;
	FIXED16_16 ss_0014;
    FIXED16_16 ss_0018;	// plane Scroll3 follows
    short ss_001c[4];	// ff8cee

	u16  x0024;		/* used with x0025 as U16 */
    u16 XCoarse;    /* x0026 number of columns skipped in draw routine */
    u16 YCoarse;    /* x0028 */
    u8  XUpdateMethod, YUpdateMethod;
    u16 XOff, YOff;	/* 0x30, 0x32 */
} ScrollState;

void GSInitOffsets(void);
void draw_background(void);

void GSSetupScr2(GState *gs);
void GSSetupScr3(GState *gs);

void GSMain(void);

void GSInitForStage(void);			/* setup palettes from g.Pallete1 */

#endif	/* INC_GSTATE */

