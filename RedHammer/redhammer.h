//
//  redhammer.h
//  MT2
//
//  Created by Ben Torkington on 29/11/14.
//
//

#ifndef MT2_redhammer_h
#define MT2_redhammer_h

#include "sf2types.h"

extern char *g_code_roms;

struct fistblue_tile_plane {
    int y1;
    int y2;
    float z1;
    float z2;
};
typedef struct fistblue_tile_plane RHTilePlane;

int load_cps_roms();

const u16 *RHOffsetLookup16(const u16 *base, int index);
const u16 RHWordOffset(u32 base, int index);

u32 RHSwapLong(const u32 num);
u16 RHSwapWord(const u16 num);

typedef u32 RHROMPtr;
typedef u16 RHShortPtr;

#define RHCODE(x) &g_code_roms[(x)]

#endif
