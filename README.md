# MustardTiger

C99 rewrite of Street Fighter II World Warrior (MAME romset sf2ua)

## Fistblue

The actual game code from the M68k ROMs, anything that's not a rewrite of the original game shouldn't be in here.

todo: gfx_glut.\[c|h\] are not part of the M68k code and should be moved to RedHammer.

## RedHammer

OpenGL + pthreads wrapper for running the game on POSIX systems

All original work, no CAPCOM code in here

## Issues right now

* The project is a bit of a mess and needs organising
* No instructions on how to run it, which ROMs are required, etc. out of the box
* No unit tests. Most game functions are tightly coupled to the game state and have side effects, making unit testing almost impossible
* The project should be split up into RedHammer, FistBlue, and one combining them, so that RedHammer can be 

## ROMs required to run

The engine expects the game ROMs to be saved into the Resources folder in the app bundle. These are

sf2gfx.bin - All the tile ROMs, interleaved and concatenated

Either all of the even/odd code ROMs, or one file allroms.bin, interleaved and concatenated. Use redhammer.c/REDHAMMER_USE_ALLROMS_BIN define to switch.
