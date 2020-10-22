/*
 *  io.h
 *  MT2
 *
 *  Created by Ben on 3/09/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include <unistd.h>

struct cps_a_regs {
    // base address registers /256
    // e.g. object_base might be 0x9020, meaning sprites begin at 0x902000
    
    u_int16_t object_base;
    u_int16_t scroll1_base;
    u_int16_t scroll2_base;
    u_int16_t scroll3_base;
    u_int16_t rowscroll_base;
    u_int16_t palette_base;

    // scroll displacements
    
    u_int16_t scroll1x, scroll1y;   // offset 0xc
    u_int16_t scroll2x, scroll2y;
    u_int16_t scroll3x, scroll3y;
    u_int16_t star1x, star1y;
    u_int16_t star2x, star2y;

    u_int16_t rowscroll_offset;
    u_int16_t video_control;
};

