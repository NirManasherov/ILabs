#include <stdio.h>
#include <unistd.h>

#include "scheduler.h"
#include "task.h"
#include "uid.h"

#define UNUSED(x) (void)(x); 

/*this function test task module
returns 1 on fail, 0 on success*/
static int TestTask(void);
/*this function multiply the number in two untill is greater than 20*/
static int OperFunc(void *params);
/*this function does nothing*/
static void CleanFunc(ilrd_uid_ty uid, void *params);
/*this function test scheduler module
returns 1 on fail, 0 on success*/
static int TestScheduler(void); 
/*operates stop func of schedule 
add it in scheduler in order to make it stop
does all operations one time*/
static int StopFunc(void *params);

int main(void)
{
	if(1 == TestTask())
	{
		puts("Error! task test didn't sucseeded!");
		return 1;  
	}
	
	if(1 == TestScheduler())
	{
		puts("Error! Scheduler test didn't sucseeded!");
		return 1; 
	}
	
	puts("Success! Everything passed!"); 
	return 0;	/*success*/
}

/*this function test task module
returns 1 on fail, 0 on success*/
static int TestTask(void)
{
	int *data1_ptr = NULL; 
	int *data2_ptr = NULL; 
	int data1 = 4;
	int data2 = 5;
	task_ty *task1; 
	task_ty *task2; 
	ilrd_uid_ty uid1;
	ilrd_uid_ty uid2; 
	
	data1_ptr = &data1; 
	data2_ptr = &data2; 
	
	/***************************************************************************
	value	4	
	time	1
	task	t1
	***************************************************************************/
	task1 = TaskCreate(OperFunc, 1, CleanFunc, (void *)data1_ptr);
	
	uid1 = TaskGetUID(task1); 
	
	/***************************************************************************
	value	4	5
	time	1	2
	task	t1	t2
	***************************************************************************/
	task2 = TaskCreate(OperFunc, 2, CleanFunc, (void *)data2_ptr);
	
	if(1 != TaskGetTimeToRun(task2) - TaskGetTimeToRun(task1))
	{
		puts("couldnt initialize time to run proparly!");
		return 1;	/*fail*/
	}
	
	uid2 = TaskGetUID(task2);
	
	if(1 == TaskIsMatchUID(task1, uid2))
	{
		puts("uid is not unique!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	value	8	5
	time	1	2
	task	t1	t2
	***************************************************************************/
	if(0 != TaskRun(task1))
	{
		puts("fail to continue running!"); 
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	value	16	5
	time	1	2
	task	t1	t2
	***************************************************************************/
	if(0 != TaskRun(task1))
	{
		puts("fail to continue running2!"); 
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	value	32	5
	time	1	2
	task	t1	t2
	***************************************************************************/
	if(1 != TaskRun(task1))
	{
		puts("fail to stop running!"); 
		return 1;	/*fail*/
	}
	
	if(0 == TaskIsMatchUID(task1, uid1))
	{
		puts("uid is not unique!");
		return 1;	/*fail*/
	}
	
	TaskDestroy(task1); 
	TaskDestroy(task2); 
	
	return 0;	/*Success*/ 
}

/*this function multiply the number in two untill is greater than 20*/
static int OperFunc(void *params)
{
	int *data = (int *)params; 
	
	*data *= 2; 
	
	if(*data>20)
	{
		return 1;	/*shouldn't repeate*/
	}
	
	return 0;	/*should repeate*/
}

/*this function does nothing*/
static void CleanFunc(ilrd_uid_ty uid, void *params)
{
	UNUSED(params); 
	UNUSED(uid); 
}

/*this function test scheduler module
returns 1 on fail, 0 on success*/
static int TestScheduler(void)
{
	int x = 1;
	int y = 2;
	int z = 3;
	int w = 4; 
	ilrd_uid_ty w_uid; 
	scheduler_ty *scheduler = NULL; 
	scheduler = SchedulerCreate(); 
	
	if(NULL == scheduler)
	{
		puts("couldn't create scheduler!"); 
		return 1;	/*fail*/
	}
	
	if(1 != SchedulerIsEmpty(scheduler))
	{
		puts("Error! scheduler is empty FAIL"); 
		return 1;	/*fail*/
	}
	/***************************************************************************
		x
	***************************************************************************/
	if(1 == UIDIsSame(UIDBadID , SchedulerAddTask(scheduler, 1, OperFunc, &x,
																    CleanFunc)))
	{
		puts("couldn't add task"); 
		return 1;	/*fail*/
	}
	
	/***************************************************************************
		y->x
	***************************************************************************/
	if(1 == UIDIsSame(UIDBadID , SchedulerAddTask(scheduler, 2, OperFunc, &y,
																    CleanFunc)))
	{
		puts("couldn't add task"); 
		return 1;	/*fail*/
	}
	
	/***************************************************************************
		y->z->x
	***************************************************************************/
	if(1 == UIDIsSame(UIDBadID , SchedulerAddTask(scheduler, 1, OperFunc, &z,
																    CleanFunc)))
	{
		puts("couldn't add task"); 
		return 1;	/*fail*/
	}
	
	/***************************************************************************
		y->STOP->z->x
	***************************************************************************/
	if(1 == UIDIsSame(UIDBadID , SchedulerAddTask(scheduler, 1, StopFunc, 
												(void *)scheduler, CleanFunc)))
	{
		puts("couldn't add stop task"); 
		return 1;	/*fail*/
	}
	
	/***************************************************************************
		y->z->x
	***************************************************************************/
	SchedulerRun(scheduler);
	
	if(2 != x) 
	{
		puts("fail in operating on x during run");
		return 1;	/*fail*/
	}
	
	if(2 != y) 
	{
		puts("fail in operating on y during run");
		return 1;	/*fail*/
	}
	
	if(6 != z) 
	{
		puts("fail in operating on z during run");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
		STOP->y->z->x
	***************************************************************************/
	if(1 == UIDIsSame(UIDBadID , SchedulerAddTask(scheduler, 2, StopFunc, 
												(void *)scheduler, CleanFunc)))
	{
		puts("couldn't add stop task"); 
		return 1;	/*fail*/
	}
	
	/***************************************************************************
		y->z->x
	***************************************************************************/
	SchedulerRun(scheduler);
	
	if(3 != SchedulerSize(scheduler))
	{
		puts("fail recognizing size!");
		return 1; 
	}
	
	if(4 != x) 
	{
		puts("fail in operating on x during second run");
		return 1;	/*fail*/
	}
	
	if(4 != y) 
	{
		puts("fail in operating on y during second run");
		return 1;	/*fail*/
	}
	
	if(12 != z) 
	{
		puts("fail in operating on z during second run");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
		y->z->x
	***************************************************************************/
	SchedulerRun(scheduler);
	
	if(32 != x) 
	{
		puts("fail in operating on x during final run");
		return 1;	/*fail*/
	}
	
	if(32 != y) 
	{
		puts("fail in operating on y during final run");
		return 1;	/*fail*/
	}
	
	if(24 != z) 
	{
		puts("fail in operating on z during final run");
		return 1;	/*fail*/
	}
	
	z = 3; /*reinitializing z*/
	
	/***************************************************************************
		w->y->z->x
	***************************************************************************/
	w_uid = SchedulerAddTask(scheduler, 10, OperFunc, &w, CleanFunc);
	
	/***************************************************************************
		y->z->x
	***************************************************************************/
	SchedulerRemoveTask(scheduler, w_uid);
	
	/***************************************************************************
		y->z->x
	***************************************************************************/
	SchedulerRun(scheduler);
	
	if(8 == w) 
	{
		puts("fail to remove w"); 
		return 1; 
	}
	
	SchedulerClear(scheduler); 
	
	if(1 != SchedulerIsEmpty(scheduler))
	{
		puts("can't clear scheduler!"); 
		return 1; 
	}
	
	SchedulerDestroy(scheduler); 
	
	return 0;	/*Success*/
}

/*operates stop func of schedule 
add it in scheduler in order to make it stop
does all operations one time*/
static int StopFunc(void *params)
{
	scheduler_ty *scheduler = (scheduler_ty *)params;
	
	SchedulerStop(scheduler);
	
	return 1;
}
