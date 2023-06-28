#include <unistd.h>     /*  sleep               */
#include "uid.h"
#include "scheduler.h"
#include "tests.h"

static void TestScheduler(void);
static void TestSchedulerRun(void);

/******************************************************************************/

static scheduler_ty *scheduler = NULL;
static ilrd_uid_ty task_add1;
static ilrd_uid_ty task_stop;
static ilrd_uid_ty task_stop_cnt;
static size_t tasks = 0;
static int test_add1 = 0;
static int count_repeats = 0;


static int OperAdd1(void *params)
{
    static size_t counter = 10;
    int *test = (int *)params;
    
    --counter;
    *test += 1;
    
    TestSchedulerRun();
    
    return !counter;
}

static int OperStopAfterCount(void *param)
{
    (void)param;
    
    ++count_repeats;
    
    return ((count_repeats == 4) ? 1 : 0);
}

static int OperStop(void *params)
{    
    SchedulerStop((scheduler_ty *)params);
    
    return 1;
}

static void NonCleanFunc(ilrd_uid_ty uid, void *params)
{
    (void)uid;
    (void)params;
    
    puts("cleaned");
}

/******************************************************************************/

int main(void)
{
    TestScheduler();
    PASS;
    
    return 0;
}

/******************************************************************************/

static void TestScheduler(void)
{
    scheduler = SchedulerCreate();
    
    TEST("SchedulerIsEmpty", SchedulerIsEmpty(scheduler), !tasks);
    TEST("SchedulerSize", SchedulerSize(scheduler), tasks);
    
    task_add1 = SchedulerAddTask(scheduler, 2, OperAdd1, &test_add1, NonCleanFunc);
    ++tasks;
    TEST("SchedulerIsEmpty", SchedulerIsEmpty(scheduler), !tasks);
    TEST("SchedulerSize", SchedulerSize(scheduler), tasks);
    
    task_stop = SchedulerAddTask(scheduler, 7, OperStop, scheduler, NonCleanFunc);
    ++tasks;
    TEST("SchedulerIsEmpty", SchedulerIsEmpty(scheduler), !tasks);
    TEST("SchedulerSize", SchedulerSize(scheduler), tasks);
    
    task_stop_cnt = SchedulerAddTask(scheduler, 3, OperStopAfterCount, NULL, NonCleanFunc);
    ++tasks;
    TEST("SchedulerIsEmpty", SchedulerIsEmpty(scheduler), !tasks);
    TEST("SchedulerSize", SchedulerSize(scheduler), tasks);
    
    TEST("SchedulerRemoveTask", SchedulerRemoveTask(scheduler, task_stop_cnt), 0);
    --tasks;
    TEST("SchedulerIsEmpty", SchedulerIsEmpty(scheduler), !tasks);
    TEST("SchedulerSize", SchedulerSize(scheduler), tasks);
    
    SchedulerRun(scheduler);
    
    SchedulerClear(scheduler);
    
    TEST("SchedulerIsEmpty", SchedulerIsEmpty(scheduler), 1);
    TEST("SchedulerSize", SchedulerSize(scheduler), 0);
    
    task_stop_cnt = SchedulerAddTask(scheduler, 5, OperStopAfterCount, NULL, NonCleanFunc);
    
    SchedulerRun(scheduler);
    
    TEST("SchedulerRun", count_repeats, 4);
    TEST("SchedulerIsEmpty", SchedulerIsEmpty(scheduler), 1);
    TEST("SchedulerSize", SchedulerSize(scheduler), 0);
    
    SchedulerDestroy(scheduler);
}

/******************************************************************************/

static void TestSchedulerRun(void)
{
    static int tasks_count = 0;
    
    TEST("SchedulerRun", test_add1, ++tasks_count);
    printf("task_add1_rep: %d/4\n", tasks_count);
}