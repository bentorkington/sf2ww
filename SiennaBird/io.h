/*
 *  io.h
 *  MT2
 *
 *  Created by Ben on 3/09/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include <stdint.h>

struct cps_a_regs {
    // base address registers /256
    // e.g. object_base might be 0x9020, meaning sprites begin at 0x902000
    
    uint16_t object_base;
    uint16_t scroll1_base;
    uint16_t scroll2_base;
    uint16_t scroll3_base;
    uint16_t rowscroll_base;
    uint16_t palette_base;

    // scroll displacements
    
    uint16_t scroll1x, scroll1y;   // offset 0xc
    uint16_t scroll2x, scroll2y;
    uint16_t scroll3x, scroll3y;
    uint16_t star1x, star1y;
    uint16_t star2x, star2y;

    uint16_t rowscroll_offset;
    uint16_t video_control;
};

