/*
 *  actions_198a_data.h
 *  MT2
 *
 *  Created by Ben on 28/06/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */


/* sythentic: */
const HitBox hitb_null[] = {{0, 0, 0, 0}};
const HitBoxAct hitbact_null[] = {{0,0,0,0,0,0,0,0,0,0,0}};
								  

const static HitBox hitb_24ef2[] = {EMPTY_HITBOX, {0, 48, 17, 50}};
	const static struct hitboxes hitboxes_24eda={
		hitb_null,
		hitb_null,
		hitb_null,
		hitb_null,
		hitbact_null,
		hitb_24ef2,
	};
	

const Image image_24d1a = {
    23, 0x001f, 0x7, 0, 128,
    { 0x8509, 0x8519, 0x8529, 0x8539, 0x8549, 0x850c, 0x851c, 0x852c, 0x850a, 0x851a, 0x852a, 0x853a, 0x854a, 0x850d, 0x851d, 0x852d, 0x0000, 0x851b, 0x852b, 0x853b, 0x854b, 0x850e, 0x851e,  }
};
const struct action actlist_24cfe[] = {
	{65535, 0x80, 0x0, &image_24d1a, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
};


char data_24c3c[] = { 0,4,2,3,0,1,3,2,1,4,3,2,0,1,4,2};


const static Image image_24d8c = {
    53, 0x001f, 0x8, 0, 0,
    { 0x8550, 0x8560, 0x8570, 0x8558, 0x8568, 0x0000, 0x8551, 0x8561, 0x8571, 0x8559, 0x8569, 0x0000, 0x8552, 0x8562, 0x8572, 0x855a, 0x856a, 0x853e, 0x8553, 0x8563, 0x8573, 0x855b, 0x856b, 0x853f, 0x8554, 0x8564, 0x8574, 0x855c, 0x856c, 0x854e, 0x8555, 0x8565, 0x8575, 0x855d, 0x856d, 0x854f, 0x8556, 0x8566, 0x8576, 0x855e, 0x856e, 0x850b, 0x0000, 0x8567, 0x8577, 0x855f, 0x856f, 0x852f, 0x0000, 0x8557, 0x8508, 0x8518, 0x853c,  }
};
const static struct action actlist_24d54[] = {
	{65535, 0x80, 0x0, &image_24d8c, 0, 0, 0, 0, 0, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
};


const static Image image_24e0e = {
    0, 0x001f, 0x29, 0, 0,
    {  }
};
const static CAFrame actlist_24d70[2] = {
	{65535, 0x80, 0x0, &image_24e0e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
	{0,0,0,(struct image *)&actlist_24d70[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};


const HitBox hitb_24ec6[]={{0,0,0,0}, {0,0,0,0}};
const HitBoxAct hitb_24ed2 = { 0,0,0,0,-5,0x32, 0x49, 0x2e};

const struct hitboxes data_24eba = {
	hitb_24ec6,
	hitb_24ec6,
	hitb_24ec6,
	hitb_24ec6,
	&hitb_24ed2,
	hitb_24ec6,
};

const Image image_24e52 = {
    19, 0x001e, 0x2f, -8, 10,
    { 0x8330, 0x8340, 0x8350, 0x8360, 0x8370, 0x8380,
		0x8390, 0x8331, 0x8341, 0x8351, 0x8361, 0x8371,
		0x8381, 0x8391, 0x8332, 0x8342, 0x8342, 0x8342, 0x8358,  }
};
const Action actlist_24e1a[] = {
	{65535, 0x80, 0x0, &image_24e52, 0, 0, 0, 0, 0, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
};


const Image image_24e86 = {
    12, 0x001e, 0x2f, -8, 10,
    { 0x0000, 0x0000, 0x8327, 0x8337, 0x8370, 0x8380, 
		0x8390, 0x0000, 0x0000, 0x8328, 0x8338, 0x8371,  
	}
};
const Action actlist_24e36[] = {
	{65535, 0x80, 0x0, &image_24e86, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
};


#pragma mark ActB04 Fires on top of the drums

const HitBoxAct hitb_28290[] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

const struct hitboxes hitboxes_28284 = {
	hitb_28290,
	hitb_28290,
	hitb_28290,
	hitb_28290,
	hitb_28290,
	hitb_28290,
};

const Image image_28278 = {
    8, 0x311e, 0x3d, 0, 0,
    { 0x8646,  }
};
const CAFrame actlist_27fb0[] = {
	{ 8, 0x80, 0x0, &image_28278, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_27fb0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_28254 = {
    8, 0x311e, 0x3d, 0, 0,
    { 0x8640,  }
};
const Image image_28248 = {
    0, 0x001e, 0x3d, 0, 0,
    {  }
};
const Image image_28260 = {
    8, 0x311e, 0x3d, 0, 0,
    { 0x8642,  }
};
const Image image_2826c = {
    8, 0x311e, 0x3d, 0, 0,
    { 0x8644,  }
};
/* image 28248 */

const CAFrame actlist_27fcc[] = {
	{ 1, 0x00, 0x1, &image_28254, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 3, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x1, &image_28260, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 3, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x2, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 3, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x3, &image_2826c, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x80, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_27fcc[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const CAFrame actlist_280a8[] = {
	{ 1, 0x00, 0x1, &image_28254, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x1, &image_28260, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x2, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x3, &image_2826c, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x80, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_280a8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const CAFrame actlist_28184[] = {
	{ 1, 0x00, 0x1, &image_28254, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x1, &image_28260, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x2, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x3, &image_2826c, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x80, 0x0, &image_28248, 0, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_28184[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const CAFrame **actlist_27fa6[]={
	actlist_27fb0, 	actlist_27fcc, 	actlist_280a8, 	actlist_28184, 
};




#pragma mark ActB05 Burning Drums

const HitBox hitb_25964[] = {{0,0,0,0}, {0, 37, 29, 32}};
const HitBoxAct hitb_2596c[] = {
	{0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{0, 37, 29, 32, 2, 1, 0, 1, 0, 0, 0, 0 },
};

const struct hitboxes hitboxes_25958 = {
	hitb_25964,
	hitb_25964,
	hitb_25964,
	hitb_25964,
	hitb_2596c,
	hitb_25964,
};

const Image image_25904 = {
    20, 0x431f, 0x19, 0, 0,
    { 0x8608,  }
};
/* image 25904 */
const Image image_25910 = {
    20, 0x431f, 0x19, 0, 0,
    { 0x860c,  }
};
const Image image_2591c = {
    20, 0x431f, 0x19, 0, 0,
    { 0x8658,  }
};
/* image 2591c */
const CAFrame actlist_25834[] = {
	{ 1, 0x00, 0x0, &image_25904, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x0, &image_25904, 1, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x1, &image_25910, 1, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x00, 0x2, &image_2591c, 1, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 1, 0x80, 0x3, &image_2591c, 1, 0, 0, 0, 1, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_25834[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};



#pragma mark ActB06 The Car

const HitBox hitb_25d86[] = {
	{ 0,  0, -56, 64}, {22, 25, 56, 65}, {22, 25, 0, 22}, {61, 22, 48, 46},};
const HitBoxAct hitb_25d78[] = {0,0,0,0,0,0,0,0,0,0,0,0};

const struct hitboxes hitboxes_25d6c = {
	hitb_25d86,
	hitb_25d86,
	hitb_25d78,
	hitb_25d78,
	hitb_25d78,
	hitb_25d78,
};	


#pragma mark Act07

const HitBox hitb_274dc[] = {{ 0, 0, 0, 0}, {-3, 36, 44, 35}};

const struct hitboxes hitboxes_274c4 = {
	hitb_null,
	hitb_null,
	hitb_null,
	hitb_null,
	hitbact_null,
	hitb_274dc,
};

const Image image_273be = {
    30, 0x001d, 0x5, 0, 0,
    { 0x83c3, 0x83d3, 0x83e3, 0x83f3, 0x83f9, 0x83c4, 0x83d4, 0x83e4, 
		0x83f4, 0x83fa, 0x83c5, 0x83d5, 0x83e5, 0x83f5, 0x83fb, 0x83c6, 
		0x83d6, 0x83e6, 0x83f6, 0x83fc, 0x83c7, 0x83d7, 0x83e7, 0x83f7, 
		0x83fd, 0x83c8, 0x83d8, 0x83e8, 0x83f8, 0x83fe,  }
};
const CAFrame actlist_273a2[] = {
	{65535, 0x80, 0x0, &image_273be, 0, 0, 0, 0, 0, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_273a2[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

const Image image_27438 = {
    30, 0x001d, 0x5, 0, 0,
    { 0x83c9, 0x83d9, 
		0x83e9, 0x83f3, 0x83f9, 0x83ca, 0x83da, 0x83ea, 0x83f4, 0x83fa, 
		0x83cb, 0x83db, 0x83eb, 0x83f5, 0x83fb, 0x83cc, 0x83db, 0x83ec, 
		0x83f6, 0x83fc, 0x83cd, 0x83dd, 0x83ed, 0x83f7, 0x83fd, 0x83ce, 
		0x83de, 0x83ee, 0x83f8, 0x83fe,  }
};
const Image image_2747e = {
    18, 0x001d, 0x5, 0, 0,
    { 0x0000, 0x0000, 0x8399, 0x838b, 
		0x839b, 0x0000, 0x0000, 0x839a, 0x838c, 0x839c, 0x0000, 0x0000, 
		0x837d, 0x838d, 0x839d, 0x0000, 0x0000, 0x837e, 0x838e, 0x839e, 
		0x0000, 0x0000, 0x837f, 0x838f, 0x839f, 0x0000, 0x0000, 0x83df, 
		0x83ef, 0x83ff,  }
};
const CAFrame actlist_27404[] = {
	{ 8, 0x00, 0x0, &image_27438, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x80, 0x0, &image_2747e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_27404[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

#pragma mark Act09 Barrels

const HitBox hitb_27e82[] = {EMPTY_HITBOX, {-7, 28, 34, 29}};
const HitBoxAct hitb_27e8a[] = {EMPTY_HITBOXACT, {-7, 28, 18, 15, 0, 1, 0, 1, 2, 0, 0, 0}};

const struct hitboxes hitboxes_27e6a = {
	hitb_27e82,
	hitb_null,
	hitb_null,
	hitb_null,
	hitb_27e8a,
	hitb_27e82,
};

const Image image_27df4 = {
    18, 0x001f, 0x1e, 0, 0,
    { 0x0000, 0x0b40, 0x0b50, 0x0b60, 0x0c99, 0x0b41, 0x0bf0, 0x0bf3, 
		0x0c9a, 0x0b90, 0x0bf1, 0x0bf4, 0x0c9b, 0x0b91, 0x0bf2, 0x0bf5, 
		0x0000, 0x0c8f, 0x0c9f, 0x0c62,  }
};
const Image image_27dc2 = {
    18, 0x001f, 0x1e, 0, 0,
    { 0x0000, 0x0b94, 0x0bee, 0x0bfe, 
		0x0af8, 0x0ba4, 0x0bef, 0x0bff, 0x0af9, 0x0ba5, 0x0bb5, 0x0b00, 
		0x0afa, 0x0ba6, 0x0bb6, 0x0bc6, 0x0000, 0x0a54, 0x0a64, 0x0a63, 
	}
};
const Image image_27d90 = {
    18, 0x001f, 0x1e, 0, 0,
    { 0x0000, 0x0c40, 0x0c50, 0x0c60, 0x0c99, 0x0cb4, 0x0cc4, 0x0c2d, 
		0x0c9a, 0x0c0e, 0x0c1e, 0x0c2e, 0x0c9b, 0x0c8e, 0x0c51, 0x0c61, 
		0x0000, 0x0c8f, 0x0c9f, 0x0c62,  }
};
const Image image_27d5e = {
    18, 0x001f, 0x1e, 0, 0,
    { 0x0000, 0x0a68, 0x0a78, 0x0a88, 
		0x0af8, 0x0a69, 0x0a79, 0x0a89, 0x0af9, 0x0a0e, 0x0a1e, 0x0a2e, 
		0x0afa, 0x0ad6, 0x0ae6, 0x0af6, 0x0000, 0x0a54, 0x0a64, 0x0a63, 
	}
};
const CAFrame actlist_27c5e[] = {
	{ 8, 0x00, 0x0, &image_27df4, 1, 0, 0, 0, 0, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x00, 0x0, &image_27dc2, 1, 0, 0, 0, 0, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x00, 0x0, &image_27d90, 1, 0, 0, 0, 0, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x80, 0x0, &image_27d5e, 1, 0, 0, 0, 0, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_27c5e[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_27e26 = {
    12, 0x001f, 0x47, 0, 0,
    { 0x0c00, 0x0c10, 0x0c20, 0x0c30, 0x0cb0, 0x0cc0, 0x0cd0, 0x0ce0, 
		0x0c38, 0x0c48, 0x0c39, 0x0c49,  }
};
const CAFrame actlist_27cc2[] = {
	{65535, 0x80, 0x0, &image_27e26, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_27cc2[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_27e48 = {
    12, 0x001f, 0x47, 0, 0,
    { 0x0f06, 0x0f0d, 0x0fa4, 0x0fae, 
		0x0f07, 0x0f0e, 0x0fa5, 0x0faf, 0x0f08, 0x0f0f, 0x0fa6, 0x0fbf, 
	}
};
const CAFrame actlist_27cde[] = {
	{65535, 0x80, 0x0, &image_27e48, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_27cde[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const CAFrame actlist_27cfa[] = {
	{ 8, 0x00, 0x0, &image_27df4, 1, 0, 0, 0, 1, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x00, 0x0, &image_27dc2, 1, 0, 0, 0, 1, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x00, 0x0, &image_27d90, 1, 0, 0, 0, 1, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x80, 0x0, &image_27d5e, 1, 0, 0, 0, 1, 1, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_27cfa[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

const CAFrame **actlist_27c54[]={
	actlist_27c5e, 	actlist_27cc2, 	actlist_27cde, 	actlist_27cfa, 
};



