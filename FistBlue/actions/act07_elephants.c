/*
 *  act07_elephants.c
 *  MT2
 *
 *  Created by Ben on 5/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"

#include "particle.h"
#include "sound.h"
#include "structs.h"
#include "lib.h"

#include "act07_elephants.h"
typedef struct UserData_Act07 UD07;
extern Game g;
extern GState gstate_Scroll1;


SimpleImage image_ec78 = {
    7, 7, 1,
    { 0x09e3, 0x0028, 0x0a4b, 0x0032, 0x0a53, 0x0023, 0x0a5b, 0x0023, 0x0aa3, 0x0023, 0x0aab, 0x0029, 0x0ab3, 0x0029, 0x0a42, 0x0021, 0x0a4a, 0x002d, 0x0a52, 0x0023, 0x0a5a, 0x002d, 0x0aa2, 0x002d, 0x0aaa, 0x0029, 0x0ab2, 0x0029, 0x0a41, 0x0021, 0x0a49, 0x002c, 0x0a51, 0x0023, 0x0a59, 0x002d, 0x0aa1, 0x002d, 0x0aa9, 0x0031, 0x0ab1, 0x0025, 0x0a40, 0x0021, 0x0a48, 0x002e, 0x0a50, 0x002f, 0x0a58,  }
};
SimpleImage image_ed42 = {
    7, 7, 1,
    { 0x09e3, 0x0028, 0x1543, 0x0032, 0x154b, 0x0023, 0x1553, 0x0023, 0x0aa3, 0x0023, 0x0aab, 0x0029, 0x0ab3, 0x0029, 0x0a42, 0x0021, 0x1542, 0x002d, 0x154a, 0x0023, 0x1552, 0x002d, 0x155a, 0x002d, 0x0aaa, 0x0029, 0x0ab2, 0x0029, 0x0a41, 0x0021, 0x1541, 0x002c, 0x1549, 0x0023, 0x1551, 0x002d, 0x1559, 0x002d, 0x1527, 0x0031, 0x152f, 0x0025, 0x0a40, 0x0021, 0x1540, 0x002e, 0x1548, 0x002f, 0x1550,  }
};
SimpleImage image_ee0c = {
    7, 7, 1,
    { 0x09e3, 0x0028, 0x1525, 0x0032, 0x152d, 0x0023, 0x1535, 0x0023, 0x0aa3, 0x0023, 0x0aab, 0x0029, 0x0ab3, 0x0029, 0x0a42, 0x0021, 0x1524, 0x002d, 0x152c, 0x0023, 0x1534, 0x002d, 0x153c, 0x002d, 0x0aaa, 0x0029, 0x0ab2, 0x0029, 0x0a41, 0x0021, 0x1523, 0x002c, 0x152b, 0x0023, 0x1533, 0x002d, 0x153b, 0x002d, 0x1527, 0x0031, 0x152f, 0x0025, 0x0a40, 0x0021, 0x1522, 0x002e, 0x152a, 0x002f, 0x1532,  }
};
CASimpleFrame actlist_e8fe[6] = {
	{45, 0x00, 0x0, &image_ec78,},
	{12, 0x00, 0x0, &image_ed42,},
	{40, 0x00, 0x0, &image_ee0c,},
	{12, 0x00, 0x0, &image_ed42,},
	{45, 0x80, 0x0, &image_ec78,}, /* LOOP BEGIN */ 
	{0,0,0,(struct image2 *)&actlist_e8fe[4],}
};
SimpleImage image_ea1a = {
    7, 7, 1,
    { 0x0a35, 0x0002, 0x0a2d, 0x0002, 0x0a35, 0x0002, 0x0a3d, 0x0002, 0x0a85, 0x0002, 0x0a8d, 0x0002, 0x0a97, 0x0005, 0x0a36, 0x0002, 0x0a2e, 0x0002, 0x0a36, 0x0002, 0x0a3e, 0x0002, 0x0a86, 0x0002, 0x0a8e, 0x0002, 0x0a97, 0x0005, 0x0a37, 0x0001, 0x0a2f, 0x0001, 0x0a37, 0x0001, 0x0a3f, 0x0007, 0x0a87, 0x0007, 0x0a8f, 0x0007, 0x0a97, 0x0005, 0x0a40, 0x0001, 0x0a48, 0x000e, 0x0a50, 0x000f, 0x0a58,  }
};
SimpleImage image_eae4 = {
    7, 7, 1,
    { 0x0a35, 0x0002, 0x0a2d, 0x0002, 0x0a35, 0x0002, 0x0a3d, 0x0002, 0x1538, 0x001d, 0x155b, 0x001d, 0x0a97, 0x0005, 0x0a36, 0x0002, 0x0a2e, 0x0002, 0x0a36, 0x0002, 0x0a3e, 0x0002, 0x153d, 0x001f, 0x0a8e, 0x0002, 0x0a97, 0x0005, 0x0a37, 0x0001, 0x0a2f, 0x0001, 0x0a37, 0x0001, 0x1537, 0x0007, 0x153f, 0x001f, 0x0a8f, 0x0007, 0x0a97, 0x0005, 0x0a40, 0x0001, 0x1540, 0x000e, 0x1548, 0x000f, 0x1550,  }
};
SimpleImage image_ebae = {
    7, 7, 1,
    { 0x1536, 0x001d, 0x153e, 0x001d, 0x0a35, 0x0002, 0x0a3d, 0x0002, 0x0a85, 0x0002, 0x0a8d, 0x0002, 0x0a97, 0x0005, 0x0a36, 0x0002, 0x1520, 0x001f, 0x1528, 0x001f, 0x1530, 0x001f, 0x0a86, 0x0002, 0x0a8e, 0x0002, 0x0a97, 0x0005, 0x0a37, 0x0001, 0x1521, 0x001e, 0x1529, 0x001e, 0x1531, 0x001e, 0x1539, 0x0007, 0x0a8f, 0x0007, 0x0a97, 0x0005, 0x0a40, 0x0001, 0x1522, 0x000e, 0x152a, 0x000f, 0x1532,  }
};
CASimpleFrame actlist_e8d2[6] = {
	{45, 0x00, 0x0, &image_ea1a,},
	{12, 0x00, 0x0, &image_eae4,},
	{40, 0x00, 0x0, &image_ebae,},
	{12, 0x00, 0x0, &image_eae4,},
	{45, 0x80, 0x0, &image_ea1a,}, /* LOOP BEGIN */ 
	{0,0,0,(struct image2 *)&actlist_e8d2[4],}
};
SimpleImage image_f008 = {
    6, 4, 1,
    { 0x0193, 0x0020, 0x0197, 0x0020, 0x019b, 0x0020, 0x019f, 0x0021, 0x0192, 0x0020, 0x0196, 0x0020, 0x019a, 0x0020, 0x019e, 0x0021, 0x0191, 0x0020, 0x0195, 0x0020, 0x0199, 0x0020, 0x019d, 0x0021,  }
};
SimpleImage image_f06e = {
    6, 4, 1,
    { 0x01c2, 0x0020, 0x01c6, 0x0020, 0x019b, 0x0020, 0x019f, 0x0021, 0x01c1, 0x0020, 0x01c5, 0x0020, 0x01bf, 0x0020, 0x019e, 0x0021, 0x01c0, 0x0020, 0x01c4, 0x0020, 0x018f, 0x0020, 0x018e, 0x0021,  }
};
SimpleImage image_f0d4 = {
    6, 4, 1,
    { 0x01b9, 0x0020, 0x01bd, 0x0020, 0x019b, 0x0020, 0x019f, 0x0021, 0x01b8, 0x0020, 0x01bc, 0x0020, 0x01bf, 0x0020, 0x019e, 0x0021, 0x01b3, 0x0020, 0x01b7, 0x0020, 0x0000, 0x0020, 0x018e, 0x0021,  }
};
CASimpleFrame actlist_e956[6] = {
	{45, 0x00, 0x0, &image_f008,},
	{12, 0x00, 0x0, &image_f06e,},
	{40, 0x00, 0x0, &image_f0d4,},
	{12, 0x00, 0x0, &image_f06e,},
	{45, 0x80, 0x0, &image_f008,}, /* LOOP BEGIN */ 
	{0,0,0,(struct image2 *)&actlist_e956[4],}
};
SimpleImage image_eed6 = {
    6, 4, 1,
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0194, 0x0000, 0x0198, 0x0000, 0x019c, 0x0001,  }
};
SimpleImage image_ef3c = {
    6, 4, 1,
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0071, 0x0000, 0x0075, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0072, 0x0000, 0x0076, 0x0000, 0x01c3, 0x0000, 0x01c7, 0x0000, 0x0190, 0x0000, 0x0000, 0x0001,  }
};
SimpleImage image_efa2 = {
    6, 4, 1,
    { 0x01be, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01b1, 0x0000, 0x01b5, 0x0000, 0x01ba, 0x0000, 0x0000, 0x0000, 0x01b2, 0x0000, 0x01b6, 0x0000, 0x01bb, 0x0000, 0x0000, 0x0001,  }
};
CASimpleFrame actlist_e92a[6] = {
	{45, 0x00, 0x0, &image_eed6,},
	{12, 0x00, 0x0, &image_ef3c,},
	{40, 0x00, 0x0, &image_efa2,},
	{12, 0x00, 0x0, &image_ef3c,},
	{45, 0x80, 0x0, &image_eed6,}, /* LOOP BEGIN */ 
	{0,0,0,(struct image2 *)&actlist_e92a[4],}
};
CASimpleFrame actlist_e9ae[5] = {
	{45, 0x00, 0x0, &image_ec78,}, /* LOOP BEGIN */ 
	{12, 0x00, 0x0, &image_ed42,},
	{40, 0x00, 0x1, &image_ee0c,},
	{12, 0x80, 0x0, &image_ed42,},
	{0,0,0,(struct image2 *)&actlist_e9ae[0],}
};
CASimpleFrame actlist_e98a[5] = {
	{45, 0x00, 0x0, &image_ea1a,}, /* LOOP BEGIN */ 
	{12, 0x00, 0x0, &image_eae4,},
	{40, 0x00, 0x0, &image_ebae,},
	{12, 0x80, 0x0, &image_eae4,},
	{0,0,0,(struct image2 *)&actlist_e98a[0],}
};
CASimpleFrame actlist_e9f6[5] = {
	{45, 0x00, 0x0, &image_f008,}, /* LOOP BEGIN */ 
	{12, 0x00, 0x0, &image_f06e,},
	{40, 0x00, 0x0, &image_f0d4,},
	{12, 0x80, 0x0, &image_f06e,},
	{0,0,0,(struct image2 *)&actlist_e9f6[0],}
};
CASimpleFrame actlist_e9d2[5] = {
	{45, 0x00, 0x0, &image_eed6,}, /* LOOP BEGIN */ 
	{12, 0x00, 0x0, &image_ef3c,},
	{40, 0x00, 0x0, &image_efa2,},
	{12, 0x80, 0x0, &image_ef3c,},
	{0,0,0,(struct image2 *)&actlist_e9d2[0],}
};

const CASimpleFrame *simpleact_e8ca[]={
	actlist_e8fe, actlist_e8d2, actlist_e956, actlist_e92a, 
};

const CASimpleFrame *simpleact_e982[]={
	actlist_e9ae, actlist_e98a, actlist_e9f6, actlist_e9d2, 
};




void action_07(Object *obj) {		//e6cc 
	UD07 *ud = (UD07 *)&obj->UserData;
	
	static const char data_e816[] = {SCROLL_2, SCROLL_2, SCROLL_3, SCROLL_3};
	static const short data_e81a[][2] = {{0x1c8, 0xb8}, {0x2d8, 0xb8}, {0x170, 0xb0}, {0x2f0, 0xb0}};
	
	
	short elephant, i;
	Object *child;
	
	if (obj->SubSel == 0) {		// Controller
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				for (i=0; i<4; i++) {
					if (child = AllocActor()) {
						obj->exists = TRUE;
						obj->Sel = SF2ACT_INDIA_ELEPHANTS;
						obj->SubSel = i+1;
						ud->x0080[i]=child;
						obj->Pool = 2;
					}
				}
				break;
			case 2:
				switch (obj->mode1) {
					case 0:
						NEXT(obj->mode1);
						obj->LocalTimer = (short []){0x36, 0x78, 0xb4, 0x3c, 0x78, 0x3c, 0xb4, 0x78}[RAND8];
						break;
					case 2:
						if (--obj->LocalTimer == 0) {
							obj->mode1 = 0;
							elephant = RAND4;
							if (ud->x0080[elephant]->UserData[0] == 0) {
								ud->x0080[elephant]->UserData[1] = TRUE;
							}
						}
						break;
						FATALDEFAULT;
				}
				if (g.FightOver) {
					NEXT(obj->mode0);
					obj->mode1 = 0 ;
				}
				break;
			case 4:			//e78a
				switch (obj->mode1) {
					case 0:
						for (i=0; i<4; i++) {
							if (ud->x0080[i]->UserData[0]) {
								return;
							}
							/* all elephants have finished speaking */
							NEXT(obj->mode1);
						}
						break;
					case 2:
						NEXT(obj->mode0);	// die and kill the elephants
						obj->mode1 = 0;
						for (i=0; i<4; i++) {
							ud->x0080[i]->mode1 = 4;
						}
						break;
						FATALDEFAULT;
				}
				break;
			case 6:
				FreeActor(obj);
				break;
				FATALDEFAULT;
		}
	} else {
		/* e7da child elephant */
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				obj->UserData[0] = 0;
				obj->UserData[1] = 0;
				obj->Scroll = data_e816[obj->SubSel];
				obj->XPI = data_e81a[obj->SubSel][0];
				obj->YPI = data_e81a[obj->SubSel][1];
				break;
			case 2:
				switch (obj->mode1) {
					case 0:
						if (obj->UserData[1] != 0) {
							NEXT(obj->mode1);
							obj->UserData[1] = 0;
							obj->LocalTimer = 0x70;
							obj->UserData[0]=TRUE;
							setactiondraw(obj, simpleact_e8ca,obj->SubSel - 1);
							queuesound(SOUND_ELEPHANT);
						}
						break;
					case 2:
						if (--obj->LocalTimer == 0) {
							obj->mode3 = 0;
							obj->UserData[0] = FALSE;
						}
						actiontickdraw(obj);
						break;
					case 4:
						NEXT(obj->mode1);
						setactiondraw(obj, simpleact_e982, obj->SubSel - 1);
						if (obj->SubSel == 1) {
							queuesound(SOUND_ELEPHANT);
						}
						break;
					case 6:
						actiontickdraw(obj);
						if (obj->SubSel == 1) {
							if (obj->AnimFlags & 1) {
								gstate_Scroll1.YPI = 0xf8;
							} else {
								gstate_Scroll1.YPI = 0;
							}
						}
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}
