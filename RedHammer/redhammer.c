//
//  redhammer.c
//  MT2
//
//  Created by bjt on 23/12/15.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "redhammer.h"

#include "sf2types.h"

char *g_code_roms;
char *g_sound_roms;
char *g_gfx_roms;


/** use the combined 'allroms.bin' instead of separate hi/lo ROMs */
#define REDHAMMER_USE_ALLROMS_BIN

#ifndef REDHAMMER_USE_ALLROMS_BIN

/** the names of each of the games code ROMs, in even/odd pairs */
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
#endif

/** the size of one code ROM, in bytes */
#define CODE_ROM_SIZE 0x20000
/** the size of a pair of hi/lo ROMs, in bytes */
#define ROM_PAIR_SIZE (CODE_ROM_SIZE * 2)
/** the size of all the game's code ROMs, in bytes */
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
#ifdef REDHAMMER_USE_ALLROMS_BIN
        FILE *allroms = fopen("allroms.bin", "r");
        int bytesread = fread(g_code_roms, 1, ALL_CODE_SIZE, allroms);
        printf("allroms: read %d bytes\n", bytesread);
#else
        FILE *rom0;     // even ROM
        FILE *rom1;     // odd ROM
        
        printf("opening from %s", getcwd(__DARWIN_NULL, 0));
        
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
#endif
    }
    else {
        return 0;
    }
    
    for (int i=0x20640; i<0x20740; ++i) {
        printf("%02x ", g_code_roms[i] & 0xff);
        if (i % 16 == 15) {
            printf("\n");
        }
    }
    
    return 1;
    
}
void print_rom_offset(const char *message, const void *addr)
{
    printf("%s: %08lx\n", message, ((char *)addr) - g_code_roms);
}
const void *RHOffsetLookup16(const u16 *base, int index)
{    
    return base + (short)(RHSwapWord(*(base + index)) / 2);
}
const u16 RHWordOffset(u32 base, int index)
{
    return RHSwapWord(*(u16 *)(RHCODE(base + (2 * index))));
}
const u8 RHByteOffset(u32 base, int index)
{
    return *(u8 *)(RHCODE(base + index));
}
const u32 RH3DLong(u32 base, int dim2, int dim3, int i1, int i2, int i3)
{
    u32 *array = RHCODE(base);
    return RHSwapLong(*(array + (i1 * dim2 * dim3) + (i2 * dim3) + i3));
}
const u32 RH2DLong(u32 base, int dim2, int i1, int i2)
{
    u32 *array = RHCODE(base);
    return RHSwapLong(*(array + (i1 * dim2) + i2));
}
const u16 RH3DWord(u32 base, int dim2, int dim3, int i1, int i2, int i3)
{
    u16 *array = RHCODE(base);
    return RHSwapWord(*(array + (i1 * dim2 * dim3) + (i2 * dim3) + i3));
}
const u16 RH2DWord(u32 base, int dim2, int i1, int i2)
{
    u16 *array = RHCODE(base);
    return RHSwapWord(*(array + (i1 * dim2) + i2));
}
const u8 RH3DByte(u32 base, int dim2, int dim3, int i1, int i2, int i3)
{
    u8 *array = RHCODE(base);
    return *(array + (i1 * dim2 * dim3) + (i2 * dim3) + i3);
}
const u8 RH2DByte(u32 base, int dim2, int i1, int i2)
{
    u8 *array = RHCODE(base);
    return *(array + (i1 * dim2) + i2);
}


u32 RHSwapLong(const u32 num)
{
    return ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
}

u16 RHSwapWord(const u16 num)
{
    return ((num >> 8) & 0xff) | ((num << 8) & 0xff00);
}
u32 RHReadLong(int romaddr)
{
    void *addr = RHCODE(romaddr);
    return RHSwapLong(*(u32 *)addr);
}
u16 RHReadWord(int romaddr)
{
    void *addr = RHCODE(romaddr);
    return RHSwapWord(*(u16 *)addr);
}

void redhammer_run_tests(void) {
    
    static const u16 data_13b06 [3][8][4]={{
        {0x0020, 0x0000, 0x0230, 0x0058, },
        {0xFFCE, 0x0000, 0x0180, 0x0048, },
        {0x0040, 0x0000, 0x0180, 0x0060, },
        {0xFFCF, 0x0000, 0x0190, 0x0010, },
        {0x0025, 0x0000, 0x0190, 0x0058, },
        {0xFFF0, 0x0000, 0x0200, 0x0048, },
        {0x0022, 0x0000, 0x0210, 0x0050, },
        {0xFFEE, 0x0000, 0x02C0, 0x0050, },
    }, {
        {0x0100, 0x0000, 0x0400, 0x0062, },
        {0x0110, 0x0000, 0x0540, 0x0053, },
        {0xFEA0, 0x0000, 0x0330, 0x0042, },
        {0xFEE0, 0x0000, 0x0130, 0x0053, },
        {0x0120, 0x0000, 0x0400, 0x0045, },
        {0x0120, 0x0000, 0x0530, 0x0063, },
        {0xFEEB, 0x0000, 0x0340, 0x0053, },
        {0xFED0, 0x0000, 0x0530, 0x0043, },
    }, {
        {0x0223, 0x0000, 0x0600, 0x0052, },
        {0xFEA0, 0x0000, 0x0480, 0x0040, },
        {0x0183, 0x0000, 0x0480, 0x0040, },
        {0xFE40, 0x0000, 0x0600, 0x0045, },
        {0x0223, 0x0000, 0x0600, 0x0054, },
        {0xFCD0, 0x0000, 0x0480, 0x0050, },
        {0x0143, 0x0000, 0x0480, 0x0040, },
        {0xFDD0, 0x0000, 0x0600, 0x0064, },
    }};
    
    for (int i=0; i<3; ++i) {
        for (int j=0; j<8; ++j) {
            for (int k=0; k<4; ++k) {
                if (data_13b06[i][j][k] != RH3DWord(0x13b06, 8, 4, i, j, k)) {
                    printf("incorrect\n");
                }
            }
        }
    }
}
