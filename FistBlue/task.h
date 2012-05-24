/*
 *  task.h
 *  GLUTBasics
 *
 *  Created by Ben Torkington on 21/04/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */
#ifndef INC_FBTASK
#define INC_FBTASK

#define MAX_TASKS       16


#include "sf2types.h"



void task_timer(void);
void sf2sleep(int duration);
void exit_ready(void);
void task_die(void);
void task_kill(unsigned short id);
void create_task(void *task, short taskid, unsigned short param, unsigned char param1, unsigned char param2);
void die_top8(void);
void wrap_trap7(void *code, u16 p1, u16 p2);
void justdie();
void printtasktable(void);
void diefree();

#ifndef SF2_UCONTEXT		// only for ucontext_t?
#define TASK_STACKSIZE  16384       
#endif 

#ifdef CPS
/* CPS uses a Motorola MC68000 */
#define M68K
#endif



struct task_params {
	u16				Param0;		// x0010
    u8				Param1;		// x0011
    u8				Param2;		// x0012
	unsigned char	x0014;
	unsigned char	x0015;
	unsigned char	x0016;
    u8				x001f;		// can be sped up by impatience 
};
typedef struct task_params TaskParam;


typedef enum task_stat_t {
	TASK_EMPTY	= 0x0,
	TASK_SLEEP	= 0x1,
	TASK_STOP	= 0x2,
	TASK_READY	= 0x4,
	TASK_RUN	= 0x8,
	TASK_NEW	= 0xc,
} TaskStat;


typedef struct {
    u8			status;		// TaskStat
    u8			timer;		// How long to sleep
	
#ifndef CPS 	
	void		(*code)();	// !!: used by pthreads, where nothing else here is
	const char	*name;
	const char	*signal;	
	char	RHThreadID;
#endif
	
#ifdef M68K					// register save vars for multitasking 
    u16			cpu_sr;
    u32			cpu_pc;
    u32			cpu_sp;
    u32			cpu_st;
#endif
	
#ifdef SF2_UCONTEXT			// XXX make it RH_UCONTEXT
    ucontext_t	UC;		
#endif
	
	TaskParam		params;
} Task;

struct executive_t {
	Task	Tasks[MAX_TASKS];
	int		CurrentTask;
	char	FadeOutComplete;
	
	Task	**NextFreeTask;		// 8208
	u16		FreeTasks;			/* 820c tasks free */
	u8		x820e;
	
	// Stacks for the tasks. pthreads takes care of this for us.
#ifdef SF2_UCONTEXT
	u8		TaskStacks[MAX_TASKS][TASK_STACKSIZE];		// 8210-8a09
#endif
#ifdef CPS
	u8		TaskStacks[MAX_TASKS][128];				// 0x8210 to 0x8a10 
#endif
	Task    *FreeTaskStack[8];		//8a10 
	
};

#define TASK_CREATE(subname, slot, par_a, par_b, par_c)					\
	create_task(&subname, slot, par_a, par_b, par_c);					\
	Exec.Tasks[slot].name = __STRING(subname);							\
	Exec.Tasks[slot].signal = "---";


#define SIG_WAIT(varname)													\
	Exec.Tasks[Exec.CurrentTask].signal = __STRING(varname);				\
	while(varname) { sf2sleep(1); }											\

#define CURRENT_TASK &Exec.Tasks[Exec.CurrentTask]



#define DIEFREE					\
	task->status = TASK_EMPTY;  \
	diefree();					\
	return;				

#define SETSLEEP(time)    sf2sleep(time)

#endif /* INC_FBTASK */