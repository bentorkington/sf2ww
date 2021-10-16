# MustardTiger

C99 rewrite of Street Fighter II World Warrior (MAME romset sf2ua)

## Read this first!

This is a work in progress, not a complete game. It currently only builds in Xcode, and even then the whole game is riddled with bugs.

If you're after a complete, breathtaking gaming experience, *this repo is not for you*.

This repo is for people who'd like to learn what a game like SF2 looks like inside, and those that can help complete it. If you don't understand why someone would do this, go back to Reddit please.

## Want to learn more?

Check out the [blog](https://sf2platinum.wordpress.com). I'd always intended to write more (don't we all) but I've been very short of time lately, which is why I decided to open this repo up. It'd be a waste if I did all this work and noone ever saw it.

## ROMs required to run

Use Xcode to build the project (sorry, Makefile is out of date, little help please?), set the build target to MT2

The engine expects the game ROMs to be saved into the Resources folder in the app bundle. These are

sf2gfx.bin - All the tile ROMs, interleaved and concatenated

1. Get a tool for interleaving both the code and graphics ROMs together, such as [this one](https://www.romhacking.net/forum/index.php?topic=26264.0)
2. Use the script in `bin/mt2-merge.sh` to merge your roms into `sf2gfx.bin` and `allroms.bin`
3. Copy these files to the Resources directory in the MT2.app bundle

## Components of the project

### Fistblue

The actual game code from the M68k ROMs, anything that's not a rewrite of the original game shouldn't be in here. In theory, this component should be able to be compiled back to m68k, burned to EPROMS and run on a real CPS.

In theory.

todo: gfx_glut.\[c|h\] are not part of the M68k code and should be moved to RedHammer.

### RedHammer

OpenGL + pthreads wrapper for running the game on POSIX systems

All original work, no CAPCOM code in here

### SwiftBeam

An abandoned windowing GUI toolkit. I thought it might be fun to try writing a basic window manager from scratch. It wasn't. I thought I'd use it to manage debug windows, etc. Might pull this out one day.

## Issues right now

* The project is a bit of a mess and needs organising
* No instructions on how to run it, which ROMs are required, etc. out of the box
* No unit tests. Most game functions are tightly coupled to the game state and have side effects, making unit testing almost impossible
* The project should be split up into RedHammer, FistBlue, and one combining them, so that RedHammer can be 

## About the code

### Endianness

CPS SF2 runs on a big-endian m68k CPU. When we're running natively on a little-endian CPU, this creates complications when trying to keep binary compatibility with in-memory structures. Endian swapping is mostly done at runtime except where it can be easily cached beforehand. The code has been tested on a big-endian PowerPC G4, but not for some time.

