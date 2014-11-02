/*
 *  task.c
 *  GLUTBasics
 *
 *  Created by Ben Torkington on 21/04/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

#ifdef __LINUX__
//#define SF2_UCONTEXT 1
#endif

#include <stdio.h>

#ifdef SF2_UCONTEXT
#include <stdlib.h>
#include <ucontext.h>
#endif

#include "sf2.h"

#include "structs.h"
#include "player.h"

#include "task.h"
#include "lib.h"

#ifndef CPS
#include "pthreads.h"
#include <pthread.h>
#endif

extern Game g;
struct executive_t Exec;

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

#ifdef SF2_UCONTEXT
static ucontext_t uctx_main;
#endif

static void despatch_tasks(void);

void task_timer(void){
    sf2_interrupt();
	despatch_tasks();
	if (g.x8a30) {				// naaasty
		despatch_tasks();		// to do speedup
		despatch_tasks();		// to do speedup
		despatch_tasks();		// to do speedup
	}
}

void justdie() {			// XXX doesn't work
	Task *task = &Exec.Tasks[Exec.CurrentTask];
	memclear(&task->params, sizeof(TaskParam));
	task->status = TASK_EMPTY;
	printf("just_die %d\n", Exec.CurrentTask);
	pthread_exit(NULL);
}	
	

void diefree() {
	Task *task = &Exec.Tasks[Exec.CurrentTask];
	memclear(&task->params, sizeof(TaskParam));
	task->status = TASK_EMPTY;

	--Exec.NextFreeTask;
	Exec.NextFreeTask[0] = task;
	++Exec.FreeTasks;
#ifdef REDHAMMER
	RHKill(task);
#endif
#ifdef CPS
	/* would swapstacks() here on 68k */
#endif
}

	
void wrap_trap7(void *code, u16 p1, u16 p2) {

	Task *task;
	if (Exec.FreeTasks) {
		task = Exec.NextFreeTask[0];
		Exec.NextFreeTask++;
		Exec.FreeTasks--;
		task->status = TASK_NEW;
		task->code = code;
		task->params.Param0 = p1;
		task->params.Param1 = p2 >> 8;
		task->params.Param2 = p2 & 0xff;
#ifdef REDHAMMER
		task->name = "Effect";
		task->signal = "-----";
		RHCreateThread(task->RHThreadID);
#endif
	} else {
#ifdef REDHAMMER
		printf("trap7 Failed!\n");
        print_task_table();
#endif
	}
}


#ifdef REDHAMMER
void print_task_table(void) {
	for (int i=0; i<MAX_TASKS; ++i) {
		if (Exec.Tasks[i].status != TASK_EMPTY) {
			printf("%2d %20s ", i, Exec.Tasks[i].name);
			switch (Exec.Tasks[i].status) {
				case TASK_SLEEP:
					puts("TASK_SLEEP ");
					break;
				case TASK_READY:
					puts("TASK_READY ");
					break;
				default:
					puts("?????????? ");
					break;
			}
			printf("%20s\n", Exec.Tasks[i].signal);
		}
	}
}
#endif

void sf2sleep (int duration) {		// see asm wrap_n_trap3
    if(duration) {
        Exec.Tasks[Exec.CurrentTask].timer = duration;
        Exec.Tasks[Exec.CurrentTask].status = TASK_SLEEP; 
    } else {
        Exec.Tasks[Exec.CurrentTask].status = TASK_SLEEP;
    }
#ifdef SF2_UCONTEXT
#ifdef DEBUG
    printf("currentTask %d sleeping %d SP~=%08x\n", CurrentTask, duration, (unsigned int) &c);
#endif
    if(swapcontext(&Tasks[CurrentTask].UC, &uctx_main)==-1) {
        handle_error("swapcontext");
    }
#ifdef DEBUG
    printf("exited sleep() task %d\n", Exec.CurrentTask);
#endif
#endif
#ifdef CPS
	volatile __asm__ {
		movem	xxx,-(%sp)
		trap 	#3
		movem	(%sp)+,xxx
	}
#endif
#ifdef REDHAMMER
	RHWait(&Exec.Tasks[Exec.CurrentTask]);
#endif
}

#ifdef CPS
__asm__ volatile {
	move	0x2000, %sr
}
#endif


void exit_ready (void) {
    Exec.Tasks[Exec.CurrentTask].status = TASK_READY;
#ifdef SF2_UCONTEXT
    if(swapcontext(&Exec.Tasks[Exec.CurrentTask].UC, &uctx_main)==-1) {
        handle_error("swapcontext");
    }
#endif
#ifdef REDHAMMER
	RHWait(&Exec.Tasks[Exec.CurrentTask]);
#endif
#ifdef CPS
    // todo
#endif
}


void task_die(void) {
	Exec.Tasks[Exec.CurrentTask].status = TASK_EMPTY;
	
#ifdef SF2_UCONTEXT
	if(swapcontext(&Exec.Tasks[Exec.CurrentTask].UC, &uctx_main)==-1) {
		handle_error("swapcontext");
	}
#endif
#ifdef REDHAMMER
	RHExit(&Exec.Tasks[Exec.CurrentTask]);
#endif
}

void task_kill(unsigned short id) {
	printf("Task_kill %d\n", id);
	Exec.Tasks[id].status = TASK_EMPTY;
#ifdef REDHAMMER
	RHKill(&Exec.Tasks[id]);
#endif
}

void die_top8(void) {
	int i;
	printf("die_top8()\n");

	for (i=8; i<16; i++) {
		if (Exec.Tasks[i].status != TASK_EMPTY) {
			
#ifdef REDHAMMER
			RHKill(&Exec.Tasks[i]);
#endif
			
			memclear(&Exec.Tasks[i].params, sizeof(TaskParam));
			
			Exec.Tasks[i].status = TASK_EMPTY;
			Exec.Tasks[i].params.Param0 = 0;
			Exec.Tasks[i].params.Param1 = 0;
			Exec.Tasks[i].params.Param2 = 0;
			// put back on the freetasks stack
			Exec.NextFreeTask--;
			Exec.NextFreeTask[0] = &Exec.Tasks[i];
			
			Exec.FreeTasks++;
		}
	}
}

void create_task(void *task, short taskid, u16 param, u8 param1, u8 param2) {
#ifdef SF2_UCONTEXT
    if (getcontext(&Tasks[taskid].UC) == -1)
        handle_error("getcontext");
    Exec.Tasks[taskid].UC.uc_stack.ss_sp = g.TaskStacks[taskid];
    Exec.Tasks[taskid].UC.uc_stack.ss_size = TASK_STACKSIZE;
    Exec.Tasks[taskid].UC.uc_link = NULL;
    
    makecontext(&Tasks[taskid].UC, task, 0);
#endif    
	Exec.Tasks[taskid].code = task;
    Exec.Tasks[taskid].status = TASK_NEW;
    Exec.Tasks[taskid].params.Param0  = param;
    Exec.Tasks[taskid].params.Param1 = param1;
    Exec.Tasks[taskid].params.Param2 = param2;	
#ifdef REDHAMMER
	RHCreateThread(taskid);
#endif
}



static void despatch_tasks (void)
{
    int i;
#ifdef CPS
	while (1) {		// CPS busies in here
		Exec.x820e = 0;
DESPATCH_STARTAGAIN:
#endif	
    for (i=0; i<MAX_TASKS; ++i) {
#ifdef CPS
		__asm__ volatile {
			move	0x2600, %sr
		}
		if (Exec.x820e) {
			goto DESPATCH_STARTAGAIN;
		}
#endif
		//printf ("considering task %d\n", i);
        Exec.CurrentTask = i;
        if(Exec.Tasks[i].status >= TASK_READY) {
            if(Exec.Tasks[i].status == TASK_READY) {
                Exec.Tasks[i].status = TASK_RUN;
            }
			if (Exec.Tasks[i].code == NULL) {
				Exec.Tasks[i].status=0;
				printf("!!!: NULL task %d\n", i);
			} else {
#ifdef CPS
				switch (task.status) {
					case 0x4:
						task.status = TASK_RUN;
						/* FALLTHRU */
					case 0x8:
						__asm__ volatile {
							move	task.usp, %a1
							move	%a1, %usp
							rte
						}
						break;
					case 0xc:
						__asm__ volatile {
							move	task.code, %a1
							move	task.initusp, %a4
							move	%a4, %usp
							move	0x0, %sr
							jmp		(%a1)
						}
						break;
				}
#else
				RHResume(&Exec.Tasks[i]);
#endif
#ifdef CPS		// XXX double-free 
				if (Exec.Tasks[i].status == TASK_EMPTY) {
					Exec.FreeTasks++;
					Exec.NextFreeTask--;
					Exec.NextFreeTask[0] = &Exec.Tasks[i];
				}
#endif
			}
			
#ifdef SF2_UCONTEXT
            if (swapcontext(&uctx_main, &Exec.Tasks[i].UC) == -1)
				handle_error("swapcontext");
#endif
#ifdef CPS 
			__asm__ volatile { move 0x2000, %sr }
#endif
        }   
    }
#ifdef CPS
	}	// end the while(1) 
#endif
	//printf("Despatcher run complete\n");
	
}


