/*
 *  aidata.h
 *  GLUTBasics
 *
 *  Created by Ben on 16/11/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "aibyte_ryu.h"
#include "aibyte_ehonda.h"
#include "aibyte_blanka.h"
#include "aibyte_guile.h"
#include "aibyte_ken.h"
#include "aibyte_chunli.h"
#include "aibyte_zangeif.h"
#include "aibyte_dhalsim.h"
#include "aibyte_mbison.h"
#include "aibyte_sagat.h"
#include "aibyte_balrog.h"
#include "aibyte_vega.h"


const struct defense_strategy **dataAIDefensive[]={		//9960e
	data_a23ce, data_a3336, data_a427a, data_a51ea, 
	data_a607e, data_a6e40, data_a7d3c, data_a8c28, 
	data_a9aa4, data_aa95e, data_ab7d6, data_ac66e, 
};

const struct dualptr *dataAIAggressive[]={				// 9963e
	&data_9966e, &data_9a1be, &data_9adc0, &data_9be68,
	&data_9c96a, &data_9d2fe, &data_9dce0, &data_9e740, 
	&data_9f374, &data_9fff6, &data_a0b22, &data_a16ce, 
};




char data_2b094[6]={0x8, 0x10, 0x20, 0x28, 0x3f, 0x50};


char data_2b09a[16][32] = {	// XXX guessed length: Yes, you totally did.
    {  16,  24,  16,   8,  24,  16,   8,  24,  24,   8,  16,  24,   8,  24,  16,   8,   8,  16,  24,   8,  16,  16,  24,  16,  16,  16,   8,  16,  24,   8,   8,  24,  },
    {  32,  48,  40,  48,  40,  32,  40,  32,  48,  40,  32,  40,  40,  48,  48,  40,  32,  48,  40,  48,  32,  40,  48,  32,  40,  32,  40,  32,  48,  48,  32,  40,  },
    {  64,  72,  56,  64,  72,  56,  64,  56,  56,  64,  64,  72,  56,  64,  72,  64,  64,  72,  64,  72,  64,  56,  64,  72,  56,  56,  56,  64,  72,  72,  56,  72,  },
    {  16,   8,  16,   8,  24,   8,  24,  48,  16,  48,  24,   8,  48,  16,   8,  24,  24,   8,  16,  48,  16,  24,  48,  16,   8,  48,  24,  48,  24,   8,  16,  48,  },
    {   8,  64,  16,  24,  16,  64,   8,  16,  16,  24,  24,   8,  64,  24,  24,  64,   8,  64,  24,  16,  64,  16,  64,  24,  16,   8,   8,  64,  24,   8,  16,   8,  },
    {   8,  72,  48,  40,  32,  48,  40,  48,  32,  40,  32,  48,  72,  40,  32,  72,  40,   8,  48,   8,  48,   8,  72,  40,  32,  48,  40,  32,  72,  32,  40,   8,  },
    {   8,  64,   8,  64,  56,  72,  56,   8,  56,   8,  72,  72,  64,   8,  72,  64,  64,  56,  64,   8,  64,  72,  72,  56,  56,   8,  72,  56,  64,  56,   8,   0,  },
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  },
    {  16,  46,   2,   2,  50,  59,   0,   6,  78,  -5,  16,   2,   0,   4,   0, 118,  84,  46,   2,   2,  66,  46,   2,  18,  66,  46,   2,  19, 112,   0,  16,  46,  },
    {   2,   6, 103,  12,  74,  46,   2,   5, 106,   2,  68,  64,  96,   0,   0,  72,  16,  46,   2,   5,   8,   0,   0,   6, 103,  38,  18,   0,  20,   0,   2,  65,  },
    {   0,  63, -21,  73,  65,  -6,   0, -44,  65, -16,  16,   0,  78, -72,  13, -44,   2,  64,   0,  31,  16,  48,   0,   0,  74,   2, 106,   4,   0,   0,   0, -128,  },
    {  20,   0,   2,   0,   0, 127, -48,  64,  65,  -6,   0, -112,  48,  48,   0,   0,  74,   2, 106,   2,  68,  64,  61,  64,   2,  38,  97,   0,   0,  70,  29,  64,  },
    {   2,  17,  97,   0,   0,  62,  29,  64,   2,  17,  97,   0,  26, -112, 102,  46,  74, 109, -117,  18, 103,  10,  16,  46,   1,  96, -80,  46,   2,  17, 102,  30,  },
    {  16,  46,   0,  72, 102,   6,  16,  46,   0,  91, 103,   8,  83,   0, -80,  46,   2,  17, 103,  10,  97,   0,  13,  36, 102,   0,  13,  60,  78, 117,  96,   0,  },
    {   4, 108,  74,  45,  10, -32, 103,   4,  78, -72,  47, -118,  48, 110,   2, -98,  20,  40,   1, -66,  48,  40,   1, -62,  50,  46,   2,  38,  74,   2, 102,   4,  },
    {  68,  64,  68,  65, -48, 104,   0,   6, -46,  64,  61,  65,   2,  34, 112,   0, -78, 110,   0,   6, 101,   2, 112,   1,  78, 117,   0,  16,   0,  32,   0,  48,  },
};
/* next address 0002b29a */


short data_2b294[16]={			/* bzzt */
    0x0010, 0x0020, 0x0030, 0x0040, 0x0050, 0x0060, 0x0070, 0x0080, 
    0x0090, 0x00a0, 0x00b0, 0x00c0, 0x00d0, 0x00e0, 0x00f0, 0x0100, 
};
/* next address 0002b2b4 */



char data_2b2b4[8][32] = {
    { 0x1, 0x2, 0x1, 0x3, 0x2, 0x1, 0x1, 0x3, 0x3, 0x2, 0x2, 0x1, 0x2, 0x2, 0x3, 0x3, 0x2, 0x1, 0x2, 0x2, 0x3, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x1, 0x1, 0x3, 0x1, 0x3,  },
    { 0x6, 0x5, 0x5, 0x5, 0x5, 0x4, 0x5, 0x4, 0x4, 0x6, 0x4, 0x4, 0x6, 0x5, 0x6, 0x6, 0x5, 0x4, 0x5, 0x6, 0x4, 0x6, 0x5, 0x4, 0x6, 0x5, 0x4, 0x5, 0x6, 0x5, 0x4, 0x6,  },
    { 0x9, 0x7, 0x9, 0x7, 0x8, 0x7, 0x8, 0x9, 0x8, 0x8, 0x7, 0x8, 0x7, 0x8, 0x9, 0x8, 0x8, 0x7, 0x8, 0x7, 0x9, 0x7, 0x9, 0x8, 0x8, 0x9, 0x7, 0x9, 0x7, 0x9, 0x8, 0x9,  },
    { 0xa, 0xc, 0xc, 0xb, 0xb, 0xc, 0xc, 0xa, 0xb, 0xa, 0xa, 0xb, 0xb, 0xc, 0xa, 0xb, 0xb, 0xa, 0xc, 0xb, 0xb, 0xa, 0xa, 0xb, 0xa, 0xc, 0xc, 0xb, 0xb, 0xc, 0xc, 0xa,  },
    { 0xd, 0xf, 0xf, 0xf, 0xe, 0xd, 0x0, 0xd, 0xe, 0xf, 0xf, 0xd, 0xf, 0xe, 0xe, 0xe, 0xe, 0xd, 0xe, 0xe, 0xd, 0xe, 0xe, 0xd, 0xd, 0xe, 0xf, 0xd, 0xf, 0xf, 0xd, 0xf,  },
    { 0x4, 0x8, 0xc, 0x8, 0x4, 0x8, 0x8, 0x4, 0xc, 0xc, 0x4, 0x8, 0x8, 0x4, 0xc, 0xc, 0xc, 0x4, 0x8, 0x4, 0x8, 0xc, 0x8, 0xc, 0x8, 0xc, 0x4, 0x8, 0x4, 0x8, 0x4, 0xc,  },
    { 0x7, 0x6, 0x5, 0x6, 0x7, 0xb, 0xd, 0x5, 0xb, 0x7, 0xc, 0xb, 0xd, 0x7, 0xd, 0xd, 0x5, 0xc, 0xb, 0xd, 0x6, 0x5, 0xb, 0x6, 0x7, 0x6, 0xb, 0x5, 0xc, 0xc, 0xc, 0x7,  },
    { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  },
};
/* next address 0002b3b4, code continues */


u16 data_a238e[]={
    0x0064, 0x009a, 0x00a4, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
	0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
	0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
	0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
	0x0040, 0x00e4,
};
u16 data_a32f6[]={
    0x0078, 0x0080, 0x00a0, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082,
	0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082,
	0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028,
	0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
	0x0040, 0x00e2, 
};
u16 data_a423a[34]={
    0x0090, 0x009a, 0x00a4, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00d4,  };
/* next address 000a427e */

u16 data_a51aa[34]={
    0x0064, 0x0082, 0x00a0, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00f4,  };
/* next address 000a51ee */

u16 data_a603e[34]={
    0x0064, 0x009a, 0x00a4, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x005a, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00d4,  };
/* next address 000a6082 */

u16 data_a6e00[34]={
    0x0064, 0x0082, 0x00a0, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00d4,  };
/* next address 000a6e44 */

u16 data_a7cfc[34]={
    0x0078, 0x0087, 0x00a0, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00da,  };
/* next address 000a7d40 */

u16 data_a8be8[34]={
    0x008c, 0x00a0, 0x00b4, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00e4,  };
/* next address 000a8c2c */

u16 data_a9a64[34]={
    0x0064, 0x0082, 0x00a0, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00d2,  };
/* next address 000a9aa8 */

u16 data_aa91e[34]={
    0x0064, 0x0076, 0x00a0, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00d8,  };
/* next address 000aa962 */

u16 data_ab796[34]={
    0x0078, 0x0082, 0x00a0, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00d0,  };
/* next address 000ab7da */

u16 data_ac62e[34]={
    0x0064, 0x0082, 0x00a0, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x008c, 0x008c, 0x0082, 0x00a0, 0x00a0, 0x00a0, 0x0082, 0x0082, 
    0x0082, 0x0082, 0x00c8, 0x00b4, 0x0082, 0x00a0, 0x0082, 0x0028, 
    0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 0x0082, 
    0x0040, 0x00c0,  };
/* next address 000ac672 */

u16 *data_995de[12]={
    data_a238e,
	data_a32f6,
	data_a423a, data_a51aa, data_a603e, data_a6e00, data_a7cfc, 
	data_a8be8, data_a9a64, data_aa91e, data_ab796, data_ac62e,  
};

/* next address 0009960e */

/* next address 000a23ee */
