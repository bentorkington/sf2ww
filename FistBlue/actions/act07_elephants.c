/*
 *  act07_elephants.c
 *  MT2
 *
 *  Created by Ben on 5/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#include "sf2.h"

#include "particle.h"
#include "sound.h"
#include "structs.h"
#include "lib.h"

#include "gfxlib.h"

#include "act07_elephants.h"
typedef struct UserData_Act07 UD07;
extern Game g;
extern ScrollState gstate_Scroll1;


void action_07(Object *obj) {		//e6cc 
	UD07 *ud = (UD07 *)&obj->UserData;
	
	static const char data_e816[] = {SCROLL_2, SCROLL_2, SCROLL_3, SCROLL_3};
	static const short data_e81a[][2] = {{456, 184}, {728, 184}, {368, 176}, {752, 176}};
	
	
	short elephant, i;
	Object *child;
	
	if (obj->SubSel == 0) {		// Controller
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				for (i=0; i<4; i++) {
					if ((child = AllocActor())) {
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
						obj->LocalTimer = (short []){54, 120, 180, 60, 120, 60, 180, 120}[RAND8];
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
                            RHSetScrollActionList(obj, RHCODE(0xe8ca), obj->SubSel - 1);
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
                        RHSetScrollActionList(obj, RHCODE(0xe982), obj->SubSel - 1);
						if (obj->SubSel == 1) {
							queuesound(SOUND_ELEPHANT);
						}
						break;
					case 6:
						actiontickdraw(obj);
						if (obj->SubSel == 1) {
							if (obj->AnimFlags & 1) {
								gstate_Scroll1.position.y.part.integer = 248;
							} else {
								gstate_Scroll1.position.y.part.integer = 0;
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

