//
//  cps_tile.h
//  MT2
//
//  Created by Ben Torkington on 24/11/15.
//
//

#ifndef cps_tile_h
#define cps_tile_h


#define TILE_PIXELS_SCR1    8
#define TILE_PIXELS_SCR2    16
#define TILE_PIXELS_SCR3    32
#define TILE_PIXELS_OBJ     16


/* Tile ROM locations and bytesizes */
#define TILE_OFFSET_OBJECT      0
#define TILE_OFFSET_SCROLLS     0x400000
#define TILE_BYTES_8x8          0x40        // not a bug, the SCR1 tiles are packed ineffieintly
#define TILE_BYTES_16x16        0x80
#define TILE_BYTES_32x32        0x200

/* Object masks */
#define TILE_MASK_BLOCK_X   0x0f00
#define TILE_MASK_BLOCK_Y   0xf000
#define TILE_MASK_BLOCK     (TILE_MASK_BLOCK_X | TILE_MASK_BLOCK_Y)
#define TILE_MASK_OFFSET    0x01ff

/* Tile attribute masks */
#define TILE_MASK_FLIP          0x0060
#define TILE_MASK_PALETTE       0x001f

/* Blank tile IDs */
#define TILE_BLANK_SCR1     0x4000
#define TILE_BLANK_SCR2     0x2800
#define TILE_BLANK_SCR3     0x400

#define TILE_OBJECT_END_TAG     0xff00

#define PALETTE_MASK_BRIGHTNESS 0xf000
#define PALETTE_MASK_COLOR_R    0x0f00
#define PALETTE_MASK_COLOR_G    0x00f0
#define PALETTE_MASK_COLOR_B    0x000f

#endif /* cps_tile_h */
