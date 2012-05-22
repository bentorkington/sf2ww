/*
 *  blanka.c
 *  GLUTBasics
 *
 *  Created by Ben on 7/02/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */


#include "sf2types.h"
#include "sf2macros.h"
#include "sf2const.h"
#include "structs.h"
#include "player.h"
#include "particle.h"
#include "rules.h"
#include "actions.h"
#include "playerstate.h"
#include "computer.h"
#include "sound.h"
#include "sf2io.h"

#include "lib.h"
#include "gfxlib.h"


#include "blanka.h"
#include "blankadata.h"

extern Game g;

typedef struct UserData_Blanka UD;

void pl_cb_setstatus2_blanka(Player *ply, short status, int argd0) {		//43b80
	setaction_list((Object *)ply, data_43b90[status / 2], argd0);
}
void pl_cb_setstatus3_blanka(Player *ply, short status) {
	pl_cb_setstatus2_blanka(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_blanka(Player *ply, short status) {
	pl_cb_setstatus2_blanka(ply, status, 0);
}

void sub_33ada(Player *ply, short sel ) {
	setaction_direct((Object *)ply, data_46df4[sel]);
}

void _BlankaCatchLU(Player *ply) {		//2e886
	UD *ud=(UD *)&ply->UserData;
	static const char data_939a0[]={
		0xcf, 0x00, 0x20, 0x04, 0xcf, 0x00, 0x20, 0x04, 0xcf, 0x00, 0x20, 0x04, 0xcf, 0x00, 0x20, 0x05, 
		0xcf, 0x00, 0x20, 0x04, };
		
	static const char data_939b4[] = {
			                    0xbd, 0x00, 0x20, 0x00, 0xca, 0xf6, 0x20, 0x00, 0xc8, 0xf8, 0x20, 0x00, 
		0xe9, 0x03, 0x20, 0x01, 0xe9, 0x03, 0x20, 0x02, };
		
	static const char data_939c8[] = { 0x00, 0x00, 0x00, 0x00, };	/* Blanka can't catch himself */
		
	static const char data_939cc[] = {
																				0xc0, 0x00, 0x20, 0x00, 
		0xc0, 0x00, 0x20, 0x00, 0xc0, 0x00, 0x20, 0x00, 0xc0, 0x00, 0x20, 0x05, 0xc0, 0x00, 0x20, 0x04, };
	static const char data_939e0[]={
		0xc5, 0x00, 0x20, 0x00, 0xc5, 0x00, 0x20, 0x00, 0xc5, 0x00, 0x20, 0x00, 0xd4, 0x03, 0x20, 0x05, 
		0xd4, 0x03, 0x20, 0x04, };
	static const char data_939f4[]={
								0xba, 0x00, 0x20, 0x04, 0xba, 0x00, 0x20, 0x04, 0xba, 0x00, 0x20, 0x04, 
		0xb8, 0xf1, 0x20, 0x06, 0xb8, 0xee, 0x20, 0x05, };
		
	static const char data_93a08[]={
														0xc5, 0x00, 0x20, 0x00, 0xbf, 0xf5, 0x21, 0x00, 
		0xbd, 0xf9, 0x20, 0x00, 0xcd, 0xef, 0x20, 0x00, 0xcd, 0xef, 0x20, 0x01,};
	static const char data_93a1c[]={
																				0xbd, 0x00, 0x20, 0x00, 
		0xbc, 0xf6, 0x20, 0x00, 0xb6, 0xf9, 0x20, 0x05, 0xc8, 0xf5, 0x20, 0x06, 0xc8, 0xf5, 0x20, 0x05, };
	static const char data_93a30[]={
		0xc7, 0x00, 0x20, 0x00, 0xca, 0xdf, 0x20, 0x00, 0xca, 0xe0, 0x20, 0x00, 0xdc, 0xe3, 0x20, 0x01, 
		0xdc, 0xe3, 0x20, 0x02, };
	static const char data_93a44[]={
								0xc9, 0x00, 0x20, 0x00, 0xc2, 0xe7, 0x21, 0x00, 0xc1, 0xed, 0x20, 0x00, 
		0xd9, 0xe5, 0x20, 0x01, 0xd9, 0xe5, 0x20, 0x02, };
	static const char data_93a58[]={
														0xc8, 0x00, 0x20, 0x03, 0xbf, 0xf3, 0x21, 0x03, 
		0xbf, 0xf3, 0x20, 0x03, 0xd8, 0xf4, 0x20, 0x04, 0xd5, 0xf4, 0x20, 0x03, 0xd4, 0x00, 0x20, 0x00, 
	/* probably ends here */
		0xf0, 0x00, 0x20, 0x02, 0xe0, 0x19, 0x20, 0x05, 0x16, 0x25, 0x00, 0x0f,  };
	/* next address 00093a7c */
	
	const static char *data_93488[12]={
		data_939a0, data_939b4, data_939c8, data_939cc, 
		data_939a0, data_939e0, data_939f4, data_93a08, 
		data_93a1c, data_93a30, data_93a44, data_93a58, 
	};
	/* next address 000934a0 */
	ud->x0096 = data_93488[ply->Opponent->FighterID];
}
void _BlankaCatchSet(Player *ply) {		//2e8a6
	UD *ud=(UD *)&ply->UserData;
	short d1;
	const char *bc= &ud->x0096[ply->Opponent->ActionScript->Catch * 4];
	d1 = ply->Flip ? -bc[0] : bc[0];
	d1 -= ply->Opponent->XPI;
	ply->XPI = -d1;
	d1 = bc[1] - ply->Opponent->YPI;
	ply->YPI = -d1;
}
void _BlankaSetAnim(Player *ply, u16 d0, u16 d1) {		// 2ef68
	setaction_direct((Object *)ply, data_46df4[d0] + (d1 / 18));
}		
void PSCBVictoryBlanka(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	short d0;
	Object *obj;
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			if (ply->RoundsWon != 2 && g.OnFinalStage==0 &&	g.NotUsed==0) {
				NEXT(ply->mode3);
			}
			d0 = (RAND16) + 0x20;
			if (g.OnBonusStage) {
				d0 -= 0x10;
				if (ply->RoundsWon == 1) {
					d0 -= 0x10;
				}
			}
			if ((char []){1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
				1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,}[d0]) 
			{
				sub_33ada(ply, 0x78);
			} else {
				sub_33ada(ply, 0x76);
			}
			g.FightOver = TRUE;
			if (ply->Opponent->XPI > ply->XPI) {
				ply->Flip = FACING_RIGHT;
			} else {
				ply->Flip = FACING_LEFT;
			}
			break;
		case 2:
			ply->PSFinishedParticipating = TRUE;
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
			break;
		case 4:
			ply->PSFinishedParticipating = TRUE;
			if (g.Pause_9e1 == 2) {
				if (obj=AllocActor()) {
					INITOBJ(obj, 0x25, 0xa);
					obj->Pool = 6;
					obj->Owner = ply;
					ud->x00a2 = obj;
					NEXT(ply->mode3);
				}				
			}
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
			break;	
		case 6:
			if (AF1) {
				if (ud->x00a2->XPI - ply->XPI + 0x50 < 0xa0 &&  ud->x00a2->XPI - ply->XPI + 0x50 > 0) {
					NEXT(ply->mode3);
					if (ply->Flip == ud->x00a2->Flip) {
						sub_33ada(ply, 0x4c);
					} else {
						ply->Flip ^= 1;
						sub_33ada(ply, 0x4c);
					}
				} else {
					PLAYERTICK;
				}
			} else {
				PLAYERTICK;
			}
			break;
		case 8:
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
		default:
			break;
	}
}


#pragma mark Computer CBs
