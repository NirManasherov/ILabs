#include <time.h>		/*	time_t, size_t				*/
#include <stdlib.h>		/*	malloc, free				*/
#include <assert.h>		/*	assert						*/

#include "uid.h"        /*  ilrd_uid_ty                 */
#include "utilities.h"  /* oper_func_ty, clean_func_ty 	*/
#include "task.h"

#define UNUSED(x) (void)(x)

static size_t TaskGetInterval(task_ty *task);
static clean_func_ty TaskGetCleanFunc(task_ty *task); 
static oper_func_ty TaskGetOperFunc(task_ty *task); 
static void *TaskGetOperFuncParam(task_ty *task);

struct task
{
    ilrd_uid_ty     uid;
    oper_func_ty    operation;
    void            *operation_param;
    clean_func_ty   clean_func;
    time_t          time_to_run;
    size_t          interval;
};

task_ty *TaskCreate(oper_func_ty operation, size_t interval, 
               clean_func_ty clean_func, void *param)
{
	task_ty *new_task; 
	ilrd_uid_ty new_uid; 
	time_t curr_time; 
	
	assert(NULL != clean_func); 
	assert(0 != interval);
	assert(NULL !=operation);
	
		
	new_task = (task_ty *)malloc(sizeof(task_ty));
	
	if(NULL == new_task)		/*malloc can fail*/
	{
		return NULL; 
	} 
	
	new_uid = UIDCreate(); 
	
	if(UIDIsSame(UIDBadID ,new_uid))		/*createUID can fail*/
	{
		free(new_task); 
		new_task = NULL; 
		return NULL; 
	} 
	
	new_task->uid = new_uid;
	new_task->operation = operation;
	new_task->operation_param = param;
	new_task->clean_func = clean_func;
	new_task->interval = interval;
	
	curr_time = time(NULL); 
	
	if((time_t)-1 == curr_time)		/*time can fail*/
	{
		free(new_task); 
		new_task = NULL; 
		return NULL; 
	}
	
	new_task->time_to_run = curr_time + TaskGetInterval(new_task);
	
	return new_task;
}

void TaskDestroy(task_ty *task)
{
	assert(NULL != task); 
	
	if(NULL != TaskGetCleanFunc(task))	/*if we don't get a valid cleanFunc*/
	{
		TaskGetCleanFunc(task)(TaskGetUID(task), TaskGetOperFuncParam(task)); 
	}
	
	free(task); 
	task = NULL;  
}

int TaskRun(task_ty *task)
{
	assert(NULL != task); 
	
	return TaskGetOperFunc(task)(TaskGetOperFuncParam(task)); 
}

time_t TaskGetTimeToRun(const task_ty *task)
{
	task_ty *task_tasktype = (task_ty *)task; 
	
	assert(NULL != task); 
	
	return task_tasktype->time_to_run; 
}

ilrd_uid_ty TaskGetUID(const task_ty *task)
{
	task_ty *task_tasktype = (task_ty *)task; 
	
	assert(NULL != task); 
	
	return task_tasktype->uid; 
}

void TaskUpdateTimeToRun(task_ty *task)
{
	time_t curr_time = time(NULL); 
	
	assert(NULL != task);
	
	if((time_t) -1 == curr_time)
	{
		curr_time = TaskGetTimeToRun(task);
	}
		
	task->time_to_run = (curr_time + TaskGetInterval(task)); 
}

int TaskIsMatchUID(const task_ty *task, ilrd_uid_ty uid)
{
	task_ty *task_tasktype = (task_ty *)task;
	
	assert(NULL != task); 
	
	return UIDIsSame(TaskGetUID(task_tasktype), uid); 
}

int TaskIsBefore(const task_ty *task1, const task_ty *task2, void *param)
{
    assert(task1);
    assert(task2);
    
    UNUSED(param);
    
    return (TaskGetTimeToRun(task1) < TaskGetTimeToRun(task2));
}


static size_t TaskGetInterval(task_ty *task)
{
	return task->interval; 
}

static clean_func_ty TaskGetCleanFunc(task_ty *task)
{
	return task->clean_func; 
}

static oper_func_ty TaskGetOperFunc(task_ty *task)
{
	return task->operation; 
}

static void *TaskGetOperFuncParam(task_ty *task)
{
	return task->operation_param; 
}
