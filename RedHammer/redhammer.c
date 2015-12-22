//
//  redhammer.c
//  MT2
//
//  Created by bjt on 23/12/15.
//
//

#include <stdio.h>
#include <stdlib.h>

char *g_code_roms;
char *g_sound_roms;
char *g_gfx_roms;

const char *code_rom_names[] = {
    "sf2u.30a",
    "sf2u.37a",
    "sf2u.31a",
    "sf2u.38a",
    "sf2u.28a",
    "sf2u.35a",
    "sf2_29a.bin",
    "sf2_36a.bin",
};

#define CODE_ROM_SIZE 0x20000
#define ROM_PAIR_SIZE (CODE_ROM_SIZE * 2)
#define ALL_CODE_SIZE (CODE_ROM_SIZE * 8)

const char *gfx_rom_names[] = {
    "sf2_06.bin",
    "sf2_08.bin",
    "sf2_05.bin",
    "sf2_07.bin",
    "sf2_15.bin",
    "sf2_17.bin",
    "sf2_14.bin",
    "sf2_16.bin",
    "sf2_25.bin",
    "sf2_27.bin",
    "sf2_24.bin",
    "sf2_26.bin",
};

const char *sound_rom_names[] = {
    "sf2_09.bin",
};

const char *sample_rom_names[] = {
    "sf2_18.bin",
    "sf2_19.bin",
};

int load_cps_roms()
{
    if ((g_code_roms = malloc(ALL_CODE_SIZE))) {
        FILE *rom0;     // even ROM
        FILE *rom1;     // odd ROM
        for (int i=0; i<4; ++i) {
            printf("opening %s and %s\n", code_rom_names[2*i + 0], code_rom_names[2*i + 1]);
            rom0 = fopen(code_rom_names[2*i + 0], "r");
            rom1 = fopen(code_rom_names[2*i + 1], "r");
            for (int j=0; j<CODE_ROM_SIZE; ++j) {
                g_code_roms[(i * ROM_PAIR_SIZE) + (2 * j) + 0] = getc(rom0);
                g_code_roms[(i * ROM_PAIR_SIZE) + (2 * j) + 1] = getc(rom1);
            }
            fclose(rom0);
            fclose(rom1);
        }
    }
    else {
        return 0;
    }
    
    return 1;
    
}