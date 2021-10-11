#!/bin/sh
# How to merge the sf2ua romset for use with the SF2 rewrite

# Basically, you need to merge the ROMs together the same was as they're mapped on the real
# hardware.

# First, get the interleave tool from https://www.romhacking.net/forum/index.php?topic=26264.0

interleave int1 1 sf2u.30a sf2u.37a
interleave int2 1 sf2u.31a sf2u.38a
interleave int3 1 sf2u.28a sf2u.35a
interleave int4 1 sf2_29a.bin sf2_36a.bin
cat int1 int2 int3 int4 > allroms.bin
echo "4256ec60bf9eec21f4d6bb34c38990a9401af82e - expected shasum"
shasum allroms.bin 

interleave gint1 2 sf2-5m.4a sf2-7m.6a sf2-1m.3a sf2-3m.5a
interleave gint2 2 sf2-6m.4c sf2-8m.6c sf2-2m.3c sf2-4m.5c
interleave gint3 2 sf2-13m.4d sf2-15m.6d sf2-9m.3d sf2-11m.5d

cat gint1 gint2 gint3 > sf2gfx.bin
echo "db52a6314b4c0cd4c48eb324720c83dd142c3bff - expected shasum"
shasum sf2gfx.bin  # should be db52a6314b4c0cd4c48eb324720c83dd142c3bff 

# Copy both these ROMs into the `Resources` directory in the MT2.app bundle, 
# or leave them in the current directory if running the app from the terminal

