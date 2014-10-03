//
//  strings.c
//  MT2
//
//  Created by Ben Torkington on 28/09/14.
//
//

#include "strings.h"

#include    "structs.h"
extern Game g;

const char *redhammer_avatars[]={
    "Ryu",      "E.Honda",  "Blanka",   "Guile",
    "Ken",      "Chun-Li",  "Zangeif",  "Dhalsim",
    "Dictator", "Sagat",    "Boxer",    "Dancer"
};

const char *redhammer_stages[]={
    "Ryu",      "E.Honda",  "Blanka",   "Guile",
    "Ken",      "Chun-Li",  "Zangeif",  "Dhalsim",
    "Dictator", "Sagat",    "Boxer",    "Dancer",
    "Car",      "Barrels",  "Drums",    "Unknown0f",
    "Highscore","Unknown11","Unknown12","Unknown13"
};

const char *redhammer_standsquat[]={
    "Stand",    "Crouch",   "Jump",     "Throw"
};

const char *redhammer_plstat[]={                /* PLSTAT_* */
    "Normal",       "Crouch",       "Jumping",      "Turnaround",
    "Standblock",   "Attacking",    "InPowerMove",  "Reel",
    "Victory",      "Loss",         "Tumble",
};

const char *redhammer_reactmode[]={
    "Vomit1",       "Vomit2",       "0x04",         "0x06",
    "0x08",         "0x0a",         "0x0c",         "0x0e",
    "Footswept",    "HitInAir",     "FireBallDown", "0x14",
    "MultiHit",     "YogaFireHit",  "Electocuted",  "Tumble1",
    "Tumble2",      "Hirrucane",    "FireballHit",  "0x24",
    "0x26",
};

const char *redhammer_status[]={
    "Walking",      "Normal",       "Crouch",       "Stand up",
    "Jump Start",   "Landing",      "Turn Around",  "Crouch Turn",
    "Falling",      "Stand Block",  "Stand Block 2","Crouch Block",
    "Crouch Block2","Stun1",        "Stun2",        "StandBlckFrz",
    "CrouchBlkFrz", "Crouch Stun",  "Footswept",    "Knockdown",
    "Back up",      "Dizzy",        "Hit Air",      "Electrocuted",
    "Tumble 30",    "Tumble 32",    "Tumble 34",    "Tumble 36",
    "Pissed Off",   "GettingThrown","Punch",        "Kick",
    "Crouch Punch", "Crouch Kick",  "Jump Punch",   "Jump Kick"
};

void redhammer_print_level_table() {
    int i;
    for (i=0; i<16; ++i) {
        printf("Level %02d is %s\n", i, redhammer_stages[g.LevelScript[i]]);
    }
}

void redhammer_print_level(int level) {
    printf("Level %02d: %s\n", level, redhammer_stages[level]);
}
