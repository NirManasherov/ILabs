#include <assert.h>	/*	assert				*/
#include <stdlib.h>	/*	malloc				*/
#include <time.h>	/*	time, time_t size_t	*/
#include <unistd.h>	/*	sleep				*/
#include <assert.h>	/*	assert				*/

#include "p_queue.h"
#include "scheduler.h"
#include "task.h" 
#include "uid.h"
#include "utilities.h" 

static p_queue_ty *SchedulerGetPQ(scheduler_ty *scheduler);
static int SchedulerPriorityFunc(void *task1, void *task2); 
static int IsUIDTheSame(const void *list_data, void *param); 
static int GetStopFlag(scheduler_ty *scheduler); 
static void MakeRun(scheduler_ty *scheduler); 

enum {RUN = 0, STOP = 1};
enum {SUCCESS = 0 , FAILIURE = 1};

struct scheduler
{
    p_queue_ty *pq_handler;
    int flag_stop;			/*  0 = RUN, 1 = STOP */
};

scheduler_ty *SchedulerCreate(void)
{
	scheduler_ty *my_scheduler; 
	
	my_scheduler = (scheduler_ty *)malloc(sizeof(scheduler_ty));
	
	if(NULL ==  my_scheduler)	/*malloc can fail*/
	{
		return NULL; 
	}
	
	my_scheduler->pq_handler = PQueueCreate(SchedulerPriorityFunc);
	
	if(NULL == SchedulerGetPQ(my_scheduler))	/*PQueueCreate can fail*/
	{
		free(my_scheduler); 
		my_scheduler = NULL; 
		return NULL; 
	}
	
	MakeRun(my_scheduler);
	
	return my_scheduler; 
}

void SchedulerDestroy(scheduler_ty *scheduler)
{
	assert(NULL != scheduler); 
	
	SchedulerClear(scheduler);
	
	PQueueDestroy(SchedulerGetPQ(scheduler)); 
	scheduler->pq_handler = NULL; 
	
	free(scheduler); 
	scheduler = NULL; 
}

ilrd_uid_ty SchedulerAddTask(scheduler_ty *scheduler, size_t interval, 
                            oper_func_ty operation, void *param,
                            clean_func_ty clean_func)
{
	task_ty *task_ptr; 
	
	assert(NULL != scheduler);
    assert(0 != interval);
    assert(NULL != operation);
    assert(NULL != clean_func);
	
	task_ptr = TaskCreate(operation, interval, clean_func, param);
	
	if(NULL == task_ptr)	/*TaskCreate can fail*/
	{
		return UIDBadID; 
	}
	
	if(FAILIURE == PQueueEnqueue(SchedulerGetPQ(scheduler), (const void *)task_ptr))
	{
		TaskDestroy(task_ptr);
        task_ptr = NULL;
        
        return UIDBadID;
	} 
	
	return  TaskGetUID((const task_ty *)task_ptr); 
}

int SchedulerRemoveTask(scheduler_ty *scheduler, ilrd_uid_ty uid)
{
	task_ty *task_to_remove = NULL;
	ilrd_uid_ty *uid_to_remove_ptr= &uid;
	
	assert(NULL != scheduler); 
	assert(!UIDIsSame(uid, UIDBadID));
    assert(!SchedulerIsEmpty(scheduler));
	
	task_to_remove = PQueueErase(SchedulerGetPQ(scheduler), IsUIDTheSame,
													(void *)uid_to_remove_ptr);
	
	if(NULL == task_to_remove)
	{
		return FAILIURE; /*fail*/
	}
	
	if(!TaskIsMatchUID((const task_ty *)task_to_remove, uid))
	{
		TaskDestroy(task_to_remove);
		 
		return FAILIURE; 
	}
	
	TaskDestroy(task_to_remove); 
	
	return SUCCESS; 
}

int SchedulerRun(scheduler_ty *scheduler)
{
	time_t curr_time; 
	task_ty *curr_task; 
	
	assert(NULL != scheduler); 
	
	while((STOP != GetStopFlag(scheduler)) && (!SchedulerIsEmpty(scheduler)))
	{		
		curr_task = (task_ty *)PQueuePeek(SchedulerGetPQ(scheduler)); 
		PQueueDequeue(SchedulerGetPQ(scheduler));
		
		curr_time = time(NULL); 
			
		if((time_t)-1 == curr_time)	/*time can fail*/
		{
			return FAILIURE; /*fail*/
		}
			
		if(curr_time < TaskGetTimeToRun(curr_task))
		{
			sleep(TaskGetTimeToRun(curr_task)-curr_time); 
		}
		
		if(0 == TaskRun(curr_task))	/*task should repeate*/
		{
			TaskUpdateTimeToRun(curr_task); 
			
			/*enqueue can fail*/
			if(SUCCESS != PQueueEnqueue(SchedulerGetPQ(scheduler), curr_task)) 
			{
				TaskDestroy(curr_task);
            	curr_task = NULL;
			
				return FAILIURE; /*fail*/
			}
		}
		
		else
		{
			TaskDestroy(curr_task);
            curr_task = NULL;
		}
	}
	
	MakeRun(scheduler); 
	
	return SUCCESS; /*success*/
}

void SchedulerStop(scheduler_ty *scheduler)
{
	assert(NULL != scheduler);
	
	scheduler->flag_stop = STOP; 	/*sflag_stop = 1 => stop schedual*/
}

size_t SchedulerSize(scheduler_ty *scheduler)
{
	assert(NULL != scheduler); 
	
	return PQueueSize(SchedulerGetPQ(scheduler));
}

int SchedulerIsEmpty(scheduler_ty *scheduler)
{
	assert(NULL != scheduler); 
	
	return PQueueIsEmpty(SchedulerGetPQ(scheduler));/*not using SchedulerSize because
														    of time complexity*/
}

void SchedulerClear(scheduler_ty *scheduler)
{
	task_ty *task_to_remove; 
	
	assert(NULL != scheduler);
	
	while(!SchedulerIsEmpty(scheduler))
	{
		/*not using scheduler remove because of time complexity*/
		task_to_remove = (task_ty *)PQueuePeek((const p_queue_ty *)
													SchedulerGetPQ(scheduler));
		TaskDestroy(task_to_remove); 
		PQueueDequeue(SchedulerGetPQ(scheduler));
	}
}

static int SchedulerPriorityFunc(void *task1, void *task2)
{
	task_ty *task_tasktype1; 
	task_ty *task_tasktype2;
	
	assert(NULL != task1); 
	assert(NULL != task2);
	
	task_tasktype1 = (task_ty *)task1;
	task_tasktype2 = (task_ty *)task2;
	
	if(TaskIsBefore((const task_ty *)task_tasktype1, 
										 (const task_ty *)task_tasktype2, NULL))
	{
		return +1;	/*P(task1) > P(task 2)*/
	}
	
	if(TaskGetTimeToRun(task1) == TaskGetTimeToRun(task2))
	{
		return 0; 	/*P(task1) == P(task 2)*/
	}
	
	return -1;	/*P(task1) < P(task 2)*/
}

static p_queue_ty *SchedulerGetPQ(scheduler_ty *scheduler)
{
	assert(NULL != scheduler); 
	
	return scheduler->pq_handler; 
}

static int IsUIDTheSame(const void *list_data, void *param)
{
	const task_ty *my_task = (const task_ty *)list_data;
	ilrd_uid_ty uid_to_cmp = *((ilrd_uid_ty *)param);
	
	return TaskIsMatchUID(my_task, uid_to_cmp);
}

static int GetStopFlag(scheduler_ty *scheduler)
{
	assert(NULL != scheduler);
	
	return scheduler->flag_stop; 
}

static void MakeRun(scheduler_ty *scheduler)
{
	assert(NULL != scheduler); 
	
	scheduler->flag_stop = RUN; 
}
