/* sf2macros.h */

#ifndef INC_SF2MACROS
#define INC_SF2MACROS

#include "workarounds.h"
#define GUSTY_LOBSTER 1			// On a UNIX host

#ifndef ABS
#define ABS(x) ((x)<0 ? -(x) : (x))
#endif

#ifdef CPS 
#define TASKSLEEP sleep(task->params.Param2)
#endif

#ifndef CPS
#define TASKSLEEP					\
	task->status = TASK_SLEEP;		\
	sf2sleep(task->params.Param2);
#endif

#define PLAYERX ply->XPI
#define PLAYERY ply->YPI


#define NEXT(A) A += 2;

// A random number between 0 and 63
#define RAND64 (sf2rand() & 0x3f)
/// A random number between 0 and 31
#define RAND32 (sf2rand() & 0x1f)
// An even random number between 0 and 30
#define RAND16W (sf2rand() & 0x1e)
// A random number between 0 and 15
#define RAND16 (sf2rand() & 0xf)
// A random number between 0 and 7
#define RAND8  (sf2rand() & 7)
// An even random number between 0 and 14
#define RAND8W (sf2rand() & 0xe)
// A random number between 0 and 3
#define RAND4  (sf2rand() & 3)

// These pedantic macros emulate word offset lookups more accurately
// A random numbert between 0 and 7
#define RAND16WD (RAND16W >> 1)
// A random number between 0 and 3
#define RAND8WD (RAND8W >> 1)


#define RESET_MODES g.mode0 = g.mode1 = g.mode2 = g.mode3 = g.mode4 = g.mode5 = 0

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/* use at the end of switch statements to trap incorrect cases in the default */
#define FATALDEFAULT \
default:			 \
FBPanic(99);			 \
break;				

/* DIEBREAK used at end of switch statements to save typing 'break' (just to quiet warnings anyway) */

#define DIEBREAK \
diefree();       \
break;

// deprecated:
#define XPI X.part.integer
#define YPI Y.part.integer

// new:
#define XFPI x.part.integer
#define FYPI y.part.integer

#define FLT2FP16(x) (0x10000 * (x))
#define FLT2FP8(x) (0x100 * (x))

#define MINUS_ONE -1

// returns -1 if `obj` is facing right, -1 if facing left
#define FLIP(obj) ((obj)->Flip ? 1 : -1)

#define CP_X ( cp >> 16    )
#define CP_Y ( cp & 0xffff )


#define PLAYER1 (&g.Player1)
#define PLAYER2 (&g.Player2)

#define INITOBJ(obj, sel, subsel)   \
obj->exists = TRUE;					\
obj->Sel = sel;						\
obj->SubSel = subsel;				\


#define INITOBJC(obj, sel, subsel,x,y)   \
	obj->exists = TRUE;					\
	obj->Sel = sel;						\
	obj->SubSel = subsel;				\
	obj->XPI = x;						\
	obj->YPI = y;						\


#endif /* INC_SF2MACROS */
