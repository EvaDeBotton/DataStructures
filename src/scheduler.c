/******************************************************************************
* Project: Scheduler                                                          *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Arial                                                            *
*                                                                             *
******************************************************************************/

#include <time.h>   /* time_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <unistd.h> /* sleep  */

#include "functions.h"
#include "scheduler.h"
#include "pqueue.h"
#include "task.h"
#include "utils.h"

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) {(type *)malloc(sizeof(type) * nelements)};

#define CONTINUE  0
#define FAILUER   1
#define SUCCESS   0 

typedef enum return_value
{
	COMPLETE = 0,
	NOT_COMPLETE = 1,
	ERROR = 2

} return_value_ty;

struct scheduler
{
    pqueue_ty *pq;
    int stop;
};

static int cmp_run_time   (const void *data1, const void *data2);
static int MatchFun(const void* task_to_remove, const void* uid);

/*******************************************************************************
******************************** Scheduler Create *****************************/

scheduler_ty *SchedulerCreate(void)
{
	scheduler_ty *scheduler = MALLOC(1, scheduler_ty)

	if(NULL == scheduler)
	{
		return NULL; 
	}

	scheduler->pq = PQCreate(cmp_run_time);
	scheduler->stop = CONTINUE;

	if(NULL == scheduler->pq)
	{
		FREE(scheduler)
		return NULL; 
	}

	return scheduler; 
}

/*******************************************************************************
******************************** Scheduler Destroy ****************************/

void SchedulerDestroy(scheduler_ty *scheduler)
{
	if(NULL != scheduler)
	{
		SchedulerClear(scheduler);
	
		PQDestroy(scheduler->pq);
		scheduler->pq = NULL;

		FREE(scheduler)
	}
}

/*******************************************************************************
********************************** Scheduler Add ******************************/

uid_ty SchedulerAdd(scheduler_ty *scheduler, time_t interval, 
	                                        scheduler_func_ty func, void *param)
{
	task_ty *new_tesk = TaskCreate(interval, (task_func_ty)func, param);

	assert(NULL != scheduler);
	assert(NULL != scheduler->pq);
	assert(NULL != func);

	if(NULL == new_tesk)
    {
        return UidGetBadUid();
    }

    /* PQEnqueue :return value: 0 for success, 1 for failure */
    if(FAILUER == PQEnqueue(scheduler->pq, (void *)new_tesk))
    {
    	TaskDestroy(new_tesk);

    	return UidGetBadUid();
    }

	return (TaskGetUID(new_tesk));
}

/*******************************************************************************
******************************** Scheduler Remove *****************************/

int SchedulerRemove(scheduler_ty *scheduler, uid_ty id)
{
	task_ty* task_to_remove = NULL;
	
	assert(NULL != scheduler);
	assert(NULL != scheduler->pq);
	
	task_to_remove = (task_ty *)PQErease(scheduler->pq, MatchFunc, &id);
	
	if(NULL == task_to_remove)
	{
		return FAILUER;
	}
	
	TaskDestroy(task_to_remove);
	
	return SUCCESS;
}

/*******************************************************************************
******************************** Scheduler Run ********************************/

status_of_execution_ty SchedulerRun(scheduler_ty *scheduler)
{
	task_ty *current_task = NULL;
	int current_task_status = COMPLETE;
	time_t time_to_run = 0;
	time_t current_time = 0;

	scheduler->stop = CONTINUE;

	while(EMPTY != SchedulerIsEmpty(scheduler))
	{
		current_time = time(NULL);

		time_to_run = TaskGetTimeToRun((task_ty *)PQPeek(scheduler->pq)); 

		if(time_to_run > current_time)
		{
			/* put sleep in a while! */
			while(0 != sleep((time_to_run - current_time)));
		}

		/* Dequeue and execute the task */  
		current_task = (task_ty *)PQDequeue(scheduler->pq);
		current_task_status = TaskRun(current_task);

		/* check task status*/  
		if(NOT_COMPLETE == current_task_status)
		{
			TaskUpdateTimeToRun(current_task);
			if(FAILUER == PQEnqueue(scheduler->pq, (void *)current_task))
    		{
    			TaskDestroy(current_task);
				return SYS_ERROR;
    		}
		}
		else if(ERROR == current_task_status) 
		{
			TaskDestroy(current_task);
			return TASK_ERROR;
		}
		else (COMPLETE == current_task_status)
		{	
			TaskDestroy(current_task);
		}
		else if(STOPPED != scheduler->stop)
		{	
			return STOPPED;
		}	
	}

	return SUCCESS;
}

/*******************************************************************************
******************************** Scheduler Stop *******************************/

void SchedulerStop(scheduler_ty *scheduler)
{
	assert(scheduler);
	
	scheduler->stop = STOPPED;
}

/*******************************************************************************
******************************** Scheduler Size *******************************/

size_t SchedulerSize(const scheduler_ty *scheduler)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->pq);

	return (PQSize(scheduler->pq)); 
}

/*******************************************************************************
****************************** Scheduler Is Empty *****************************/

int SchedulerIsEmpty(const scheduler_ty *scheduler)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->pq);

	return (PQIsEmpty(scheduler->pq)); 
}

/*******************************************************************************
****************************** Scheduler Clear ********************************/

void SchedulerClear(scheduler_ty *scheduler)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->pq);

	while(NOT_EMPTY == PQIsEmpty(scheduler->pq))
	{
		TaskDestroy((task_ty *)PQDequeue(scheduler->pq));
	}
}

/*************************** static cmp_run_time ******************************/
/*
  	 function that compares the time to run of task1 and task2, and return:							
 			1) positive number if (time to run of task1 > task2)									
 			2) zero if the time to run is equal												
 			3) negative number if (time to run of task1 < task2)
 */
static int cmp_run_time(const void *task1, const void *task2)
{
	return (TaskGetTimeToRun((task_ty *)task2) - TaskGetTimeToRun((task_ty *)task1)); 
}

static int MatchFunc(const void* task_to_remove, const void* uid)
{
	task_ty* task = (task_ty *)task_to_remove;
	return (UidIsSame(*(uid_ty*)uid, TaskGetUID(task)));
}