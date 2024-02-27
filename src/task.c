/******************************************************************************
* Project: task                                                               *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Arial                                                            *
*                                                                             *
******************************************************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* free, malloc */

#include "uid.h"
#include "task.h"

#define FREE(ptr) {free(ptr); ptr = NULL;}

struct task
{
    uid_ty uid;
    time_t interval;
    time_t time_to_run;
    task_func_ty function;
    void *param;
};

/*******************************************************************************
********************************** Task Create ********************************/

task_ty *TaskCreate(time_t interval, task_func_ty function, void *param)
{
    task_ty *new_task = NULL;

    assert(function);

    task_ty *new_task = (task_ty *)malloc(sizeof(task_ty));

    if(NULL == new_task)
    {
        return  NULL;
    }

    new_task->uid = UidCreate();
    new_task->interval = interval;
    new_task->time_to_run = time(NULL) + interval;
    new_task->function = function;
    new_task->param = param;

    return (new_task);
}

/*******************************************************************************
********************************* Task Destroy ********************************/

void TaskDestroy(task_ty *task)
{
    assert(NULL != task);

    FREE(task);
}

/*******************************************************************************
********************************* Task Get UID ********************************/

uid_ty TaskGetUID(const task_ty *task)
{
    assert(NULL != task);

    return (task->uid);
}

/*******************************************************************************
***************************** Task Get Time To Run ****************************/

time_t TaskGetTimeToRun(const task_ty *task)
{
    assert(NULL != task);

    return (task->time_to_run); 
}

/*******************************************************************************
**************************** Task Update Time To Run **************************/

void TaskUpdateTimeToRun(task_ty *task)
{
    assert(NULL != task);

    task->time_to_run = time(NULL) + task->interval;
}

/*******************************************************************************
*********************************** Task Run **********************************/

int TaskRun(const task_ty *task)
{
    assert(NULL != task);

    return (task->function(task->param));
}
