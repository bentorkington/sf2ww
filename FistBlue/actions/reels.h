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





