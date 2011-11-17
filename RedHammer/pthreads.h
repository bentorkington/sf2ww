/*
 *  pthreads.h
 *  MT2
 *
 *  Created by Ben on 27/08/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include "task.h"

void RHWait(Task *task);
void RHKill(Task *task);
void RHCreateThread(int worker);
void RHExit(Task *task);
void RHResume(Task *task);

void RHInitThreads(void);