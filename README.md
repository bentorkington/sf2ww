# MustardTiger

C99 rewrite of Street Fighter II World Warror (MAME romset sf2ua)

## Fistblue

The actual game code from the M68k ROMs

todo: gfx_glut.\[c|h\] are not part of the M68k code and should be moved to RedHammer

## RedHammer

OpenGL + pthreads wrapper for running the game on POSIX systems

## Issues right now

* The project is a bit of a mess and needs organising
* No instructions on how to run it, which ROMs are required, etc. out of the box
* No unit tests. Most game functions are tightly coupled to the game state and have side effects, making unit testing almost impossible

