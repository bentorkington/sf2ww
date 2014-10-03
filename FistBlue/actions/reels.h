/*
 *  reels.h
 *  MT2
 *
 *  Created by Ben on 14/12/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

struct UserData_Act23 {
	short	ObjX;		//84
	short	ObjY;		//86
	short	PlyX;		//88
	short	PlyY;		//8a
	short	x008c;		//8c
	void	*x008e;		// ptr to exit routine
	
	char	x0092;
	char	x0093;		// side of player
};


const Image image_1ec12 = {
    2, 0x0006, 0x2a, 0, 0,
    { 0x089b, 0x089c,  }
};
const Image image_1ec20 = {
    2, 0x0006, 0x2a, 0, 0,
    { 0x0880, 0x0881,  }
};
const CAFrame actlist_1e956[3] = {
	{ 5, 0x00, 0x0, &image_1ec12, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
	{ 5, 0x80, 0x0, &image_1ec20, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1e956[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1ed16 = {
    1, 0x0001, 0x31, 0, 0,
    { 0x0000, 0x0c57,  }
};
const Image image_1ed28 = {
    1, 0x0001, 0x31, 0, 0,
    { 0x0000, 0x0ca5,  }
};
const Image image_1ed3a = {
    4, 0x0001, 0x31, 0, 0,
    { 0x0d30, 0x0d40, 0x0dc6, 0x0dd6,  }
};
const Image image_1ed4c = {
    4, 0x0001, 0x31, 0, 0,
    { 0x0d16, 0x0d82, 0x0d17, 0x0d82,  }
};
const Image image_1ed5e = {
    4, 0x0001, 0x31, 0, 0,
    { 0x0df1, 0x0dfe, 0x0def, 0x0dff,  }
};
const CAFrame actlist_1eb42[7] = {
	{ 5, 0x00, 0x0, &image_1ed16, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
	{ 5, 0x00, 0x0, &image_1ed28, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 5, 0x00, 0x0, &image_1ed3a, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 5, 0x00, 0x0, &image_1ed4c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 5, 0x00, 0x0, &image_1ed5e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 5, 0x80, 0x1, &image_1ed5e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1eb42[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1ec2e = {
    1, 0x0003, 0x78, 0, 0,
    { 0x0b36,  }
};
const Image image_1ec3a = {
    1, 0x0003, 0x78, 0, 0,
    { 0x0b45,  }
};
const Image image_1ec46 = {
    2, 0x0003, 0x78, 0, 0,
    { 0x0b36, 0x0b36,  }
};
const Image image_1ec54 = {
    2, 0x0003, 0x78, 0, 0,
    { 0x0b45, 0x0b45,  }
};
const Image image_1ec62 = {
    3, 0x0003, 0x78, 0, 0,
    { 0x0b36, 0x0b36, 0x0b36,  }
};
const Image image_1ec72 = {
    3, 0x0003, 0x78, 0, 0,
    { 0x0b45, 0x0b45, 0x0b45,  }
};
const Image image_1ec82 = {
    3, 0x0003, 0x78, 0, 0,
    { 0x0be3, 0x0be3, 0x0be3,  }
};
const Image image_1ec92 = {
    3, 0x0003, 0x78, 0, 0,
    { 0x0bf6, 0x0bf6, 0x0bf6,  }
};
const CAFrame actlist_1e98a[10] = {
	{ 2, 0x00, 0x0, &image_1ec2e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
	{ 2, 0x00, 0x0, &image_1ec3a, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ec46, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ec54, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ec62, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ec72, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ec82, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ec92, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x80, 0x1, &image_1ec92, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1e98a[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1eca2 = {
    1, 0x000b, 0x78, 0, 0,
    { 0x0b36,  }
};
const Image image_1ecae = {
    1, 0x000b, 0x78, 0, 0,
    { 0x0b45,  }
};
const Image image_1ecba = {
    2, 0x000b, 0x78, 0, 0,
    { 0x0b36, 0x0b36,  }
};
const Image image_1ecc8 = {
    2, 0x000b, 0x78, 0, 0,
    { 0x0b45, 0x0b45,  }
};
const Image image_1ecd6 = {
    3, 0x000b, 0x78, 0, 0,
    { 0x0b36, 0x0b36, 0x0b36,  }
};
const Image image_1ece6 = {
    3, 0x000b, 0x78, 0, 0,
    { 0x0b45, 0x0b45, 0x0b45,  }
};
const Image image_1ecf6 = {
    3, 0x000b, 0x78, 0, 0,
    { 0x0be3, 0x0be3, 0x0be3,  }
};
const Image image_1ed06 = {
    3, 0x000b, 0x78, 0, 0,
    { 0x0bf6, 0x0bf6, 0x0bf6,  }
};
const CAFrame actlist_1ea66[10] = {
	{ 2, 0x00, 0x0, &image_1eca2, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ecae, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ecba, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ecc8, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ecd6, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ece6, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ecf6, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0x00, 0x0, &image_1ed06, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	/* 1e98a */ 	{ 2, 0x80, 0x1, &image_1ed06, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1ea66[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1f2c0 = {
    1, 0x000e, 0x29, 0, 0,
    { 0x0903,  }
};
const Image image_1f2cc = {
    1, 0x000e, 0x29, 0, 0,
    { 0x0920,  }
};
const CAFrame actlist_1efda[3] = {
	{48, 0x00, 0x0, &image_1f2c0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
	{48, 0x80, 0x0, &image_1f2cc, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1efda[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1f2d8 = {
    1, 0x000e, 0x29, 0, 0,
    { 0x0957,  }
};
const Image image_1f2e4 = {
    1, 0x000e, 0x29, 0, 0,
    { 0x08c4,  }
};
const CAFrame actlist_1f00e[3] = {
	{48, 0x00, 0x0, &image_1f2d8, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
	{48, 0x80, 0x0, &image_1f2e4, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1f00e[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const CAFrame actlist_1eff2[2] = {
	/* 1efda */ 	{48, 0x80, 0x0, &image_1f2cc, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1eff2[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const CAFrame actlist_1f026[2] = {
	/* 1f00e */ 	{48, 0x80, 0x0, &image_1f2e4, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1f026[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

const Action *actlist_1e8d6[]={
	actlist_1e956, actlist_1eb42, actlist_1e98a, actlist_1ea66, actlist_1efda, actlist_1f00e, actlist_1eff2, actlist_1f026, actlist_1eb42, 
};

const static POINT8 data_1e804[12][3] = {
	{ { 35, 84, },	{  1, 66,  },	{  0, 87,  },},
	{ { 46, 84, },	{-17, 57,  },	{  3, 88,  },},
	{ { 54, 92, },	{-13, 51,  },	{  6, 96,  },},
    { { 42, 89, },	{ -9, 64,  },	{  0, 88,  },},
    { { 35, 84, },	{  1, 66,  },	{  0, 87,  },},
    { { 34, 86, },	{  9, 58,  },	{ 10, 82,  },},
    { { 40, 104,},	{-24, 76,  },	{ 16, 96,  },},
    { { 33, 89, },	{ -8, 68,  },	{  0, 88,  },},
    { { 43, 95, },	{ -8, 73,  },	{  0, 89,  },},
    { { 37, 115,},	{-19, 76,  },	{  7, 117, },},
    { { 30, 108,},	{-15, 75,  },	{ -3, 104, },},
    { { 37, 94, },	{ 11, 74,  },   {  0, 83,  },},
};





