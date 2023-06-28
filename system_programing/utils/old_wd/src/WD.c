/*******************************************************************************
* Author: Nir Manasherov
* Project: WatchDog
* Reviewer: 
* Date: 15.3.23
* Version: 1.0
*******************************************************************************/
/*******************************************************************************
                                  libraries
*******************************************************************************/
#define _POSIX_C_SOURCE 200112L             /*  sigaction   */
#include <pthread.h>	/*	pthread_create, pthread_join	*/  
#include <sys/types.h>  /*  pid_t                           */
#include <unistd.h>     /*  fork, execvp                    */ 
#include <sys/wait.h>   /*  pause, waitpid                  */
#include <semaphore.h>  /*  sem_init, sem_wait, sem_post    */
#include <signal.h>     /*  kill, sigaction                 */
#include <stddef.h>     /*  size_t                          */
#include <assert.h>     /*  assert                          */
#include <stdlib.h>     /*  malloc, free, getenv            */

#include "scheduler.h"
#include "watchdog.h" 
#include "WD_internal.c"
#include "utils.h"

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

/*clean all resorces used by WD*/
static int CleanUp(wd_task_ty *task_struct);

/*init scheduler and check for failes*/
static int InitScheduler(wd_task_ty *task_params); 

/*make a new client process*/
static int Resuscitate(pid_t target); 

static void SigUsr1Handler(int signum); 

static void SigUsr2Handler(int signum); 

static int SignOfLife(void *params); 

static char **InitialArgsPackeging(char **new_argv, int argc, char *argv[], 
                                             size_t interval, size_t max_misses);
                                             

/*a scheduler task that pause a process untill signal is recieved happend once only*/
static int PauseFunc(void *params); 

/*if a process did not send back signal more than max_misses it will be killed
and resesutate*/
static int IsAlive(void *params); 

static void InitWaitTime(struct timespec *sem_wait_time);

/*******************************************************************************
                                typedefs
*******************************************************************************/

typedef void *(*routine_func)(void *);

/*******************************************************************************
                                global variables
*******************************************************************************/

int g_num_of_struct_mems = 4; 

int g_counter = 0;

int g_should_stop = FALSE;  

sem_t sem_is_wd_exist; 

struct sigaction g_sigusr1; 

struct sigaction g_sigusr2; 

pthread_attr_t attr; 

extern int g_app_counter; 

/*******************************************************************************
                                  enums
*******************************************************************************/

enum {SEM_INIT_VAL = 0, MAKE_SEM_FOR_THREADS = 0};

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/

void *ThreadRoutine(wd_params_ty *watchdog_params)
{
	/*init fields of g_sigusr1*/
	g_sigusr1.sa_handler = SigUsr1Handler;
	g_sigusr1.sa_flags = 0;
    
    /*sigaction(SIGUSR1, &g_sigusr1, NULL)*/
    RETURN_IF_BAD(SUCCESS == sigemptyset(&g_sigusr1.sa_mask), 
                                     "sigemptyset fail", (void *)((size_t)FAIL)); 

    /*Watch();*/
    RETURN_IF_BAD(SUCCESS == Watch(watchdog_params), "WatchDog func fail!", 
                                                        (void *)((size_t)FAIL));
    
    /*return NULL*/
    return (void *)((size_t)SUCCESS);
}

/******************************************************************************/

int MakeMeImmortal(int argc_, char *argv_[], size_t interval_, 
                                                             size_t max_misses_)
{
	pthread_t new_thread; 
	struct timespec *sem_wait_time; 
	
	/*wd_params_ty wd_params struct*/
	wd_params_ty *wd_params = NULL;  
	
	assert(0 != interval_); 
	assert(0 != max_misses_); 
	    
    /*init attr*/
    RETURN_IF_BAD(SUCCESS != pthread_attr_init(&attr), "attr_init fail", FAIL);
    
    /*set attr*/
    RETURN_IF_BAD(SUCCESS != pthread_attr_setdetachstate(&attr, 
                    PTHREAD_CREATE_DETACHED), "attr_setdetachstate fail", FAIL);
    
    /*allocate resorces for wd_params struct*/
    wd_params = (wd_params_ty *)malloc(sizeof(wd_params_ty)); 
    
    RETURN_IF_BAD(NULL != wd_params, "malloc fail!", FAIL);
    
	/*init parameters of the user and save them in struct (argc, argv, interval,
	max_misses)*/
    wd_params->argc = argc_; 
	wd_params->argv = argv_; 
	wd_params->interval = interval_; 
	wd_params->max_misses = max_misses_; 
	
	/*sem_init*/
	/*check for fail*/
	RETURN_IF_BAD_CLEAN(SUCCESS == sem_init(&sem_is_wd_exist, 
	MAKE_SEM_FOR_THREADS, SEM_INIT_VAL),"sem init fail", FAIL, free(wd_params));
	
	/*pthread_create with casting to routine func the last argument is wd_params*/
	/*check for fail*/
	RETURN_IF_BAD_CLEAN(SUCCESS == pthread_create(&new_thread, &attr, 
	(routine_func)ThreadRoutine, wd_params), "pthread_create fail", FAIL, 
	                        {free(wd_params) ; sem_destroy(&sem_is_wd_exist);});
	
	/*set wait time for the semaphore*/
	InitWaitTime(sem_wait_time); 
	
	/*sem_wait of sem_is_wd_exist*/
	RETURN_IF_BAD_CLEAN(SUCCESS == sem_timedwait(&sem_is_wd_exist, sem_wait_time), 
	 "sem_wait fail", FAIL, {free(wd_params) ; sem_destroy(&sem_is_wd_exist);});
	
	/*return SUCCESS*/
	return SUCCESS; 
}

/******************************************************************************/

static void InitWaitTime(struct timespec *sem_wait_time)
{
    sem_wait_time->tv_sec = 5; 
    sem_wait_time->tv_nsec = 0; 
}

/******************************************************************************/

int DoNotResuscitate(void)
{
	pid_t wd_pid;
	
	char *env_char = NULL; 
	 
	/*g_should_stop = TRUE*/
	g_should_stop = TRUE;
	
	env_char = 	getenv("WD_PID"); 
	
	RETURN_IF_BAD(NULL != env_char, "get_env fail!", FAIL); 
	
	wd_pid = (pid_t)atoi(env_char); 
	
	/*send WD sigusr2*/
	RETURN_IF_BAD(kill(wd_pid, SIGUSR2), "kill fail!", FAIL); 
	
	return SUCCESS; 
}

/******************************************************************************/

/*clean all resorces used by WD*/
static int CleanUp(wd_task_ty *task_struct)
{
	/*sem destroy check for fail*/
	/*  scheduler           */
	/*SchedulerDestroy check for fail*/
	
    /*use kill to send SIGUSR2 to WD_app (WD_PID)*/
	
	/*return SUCCESS*/
}

/******************************************************************************/

/*init scheduler and check for failes*/
static int InitScheduler(wd_task_ty *task_params)
{
    /*if is_child == FALSE*/
        /*SchedulerAddTask(PauseFunc) interval should be 0 so it happens first*/
    
	/*target process == other process pid in struct*/
	
	/*SchedulerAddTask SOL func for interval - check if failed*/
	
	/*SchedulerAddTask of IsAlive in interval*max_misses*/
			
	/*return SUCCESS*/
}

/******************************************************************************/

/*make a new client process*/
static int Resuscitate(pid_t target)
{
	/*kill the process of target*/
	
	/*if getenv(WD_PID) != target - client stopped*/
	    /*fork the client process from watchdog_params - check if succeeded*/
	
	/*else - WD stopped*/ 
	    /*fork the WD process from client - check if succeeded*/
	
	/*return SUCCESS*/
}

/******************************************************************************/

int Watch(wd_params_ty *watchdog_params)
{
	/*pid child_pid = 0;*/
	/*wd_task_ty task_struct;*/
	/*char *new_argv[g_num_of_struct_mems];*/
	
	/*g_num_of_struct_mems += watchdog_params->argc ; (extra place for NULL in the end)*/
	
	/*copy all arguments to new_argv including max misses and*/

	/* InitialArgsPackeging of new_argv*/
    
    /*if fail (return NULL) - there is no WD
	    child_pid = fork - check for fail*/
        
        /*init wd_params_ty struct of partner_pid: */
        /*if child_pid = 0 => we are child*/
            /*set is_child TRUE*/
            /*set partner_pid to getppid()*/
        /*else => parent*/
            /*set is_child FALSE*/
            /*set partner_pid to child_pid*/
        
	
	/*get enviorment variables using getenv and check for fails*/
	
	/*if fail (return NULL) - there is no WD cvurrently and is_child = TRUE - we are 
	child*/		
        /*make the memory image of WD_app:*/
	    /*execvp using argv to make with watchdog app with watchdog_params and 
	    file name is in ./WD_app.c*/
    
	/*SchedulerCreate and check for fail*/
	
	/*init task struct: wd_params, sched*/
	
	/*InitScheduler(task_struct)*/	
	/*check if fail*/
	
	/*while(!g_should_stop)*/
	    /*SchedulerRun() - check if failed*/
            
}

/******************************************************************************/

static void SigUsr1Handler(int signum)
{
    int sem_val = -1; 
    int counter_val = 0; 
    
    /*UNUSED(signum);*/
    UNUSED(signum);
    
    RETURN_IF_BAD_NO_RETURN(SUCCESS == sem_getvalue(&sem_is_wd_exist, &sem_val), 
                                                           "sem_getvalue fail"); 
    
    /*if sem_is_wd_exist value is 0*/ 
    if(0 == sem_val)
    {
        /*sem_post*/ 
        RETURN_IF_BAD_NO_RETURN(SUCCESS == sem_post(&sem_is_wd_exist), 
                                                               "sem_post fail");
    }        
    
    /*g_counter = 0 atomicly*/
    __atomic_store(&g_counter ,&counter_val , __ATOMIC_SEQ_CST);
}

/******************************************************************************/

static void SigUsr2Handler(int signum)
{
    /*UNUSED(signum);*/
    UNUSED(signum);
}

/******************************************************************************/

static int SignOfLife(void *params)
{
    /*pid_t *target*/
    pid_t *target;
    
    /*target = (pid *)params*/
    target = (pid_t *)params;
    
    /*if target == getenv => im sending to  WD => im client*/
    if(*target == (pid_t)atoi(getenv("WD_PID")))
    {
        /*atomicly add g_counter*/
        __atomic_add_fetch(&g_counter, 1, __ATOMIC_SEQ_CST);
    }   
	
	/*else im sending to  Client => im WD*/
	else
	{
        /*atomicly add g_app_counter*/
	    __atomic_add_fetch(&g_app_counter, 1, __ATOMIC_SEQ_CST);
	}   	    
    
    /*kill(SIGUSR1) to target*/   
    RETURN_IF_BAD(kill(*target, SIGUSR1), "kill fail!", FAIL); 
    
    /*return SUCCESS*/
    return SUCCESS; 
}

/******************************************************************************/

/*if a process did not send back signal more than max_misses it will be killed
and resesutate*/
static int IsAlive(void *params)
{
    /*wd_task_ty *task_struct = (watchdog_params_ty *)params*/
   
     /*if(g_should_stop) */
        /*SchedulerStop(task_struct->SCH)*/
        
    /*if max_misses < g_counter || max_misses < g_app_counter*//*proc not 
        `                                                       responsive*/
        /*SchedulerStop(task_struct->SCH)*/
        /*CleanUp*/
	    /*Resuscitate(partner_pid)*/
 	    
	/*return success*/
}

/******************************************************************************/

/*a scheduler task that pause a process untill signal is recieved happend once only*/
static int PauseFunc(void *params)
{
    /*UNUSED(params);*/
    UNUSED(params);
    
    /*wait for signal by pause();*/
    pause();
    
    /*return FAIL*/
    return FAIL; /*so the scheduler operate it once*/
}

/******************************************************************************/

static char **InitialArgsPackeging(char **new_argv, int argc, char *argv[], 
                                             size_t interval, size_t max_misses)
{
    /*char **/
    
    /*new_argv[0] = argv*/
    /*new_argv[] = sprintf(argc)*/
    /*new_argv[] = sprintf(interval)*/
    /*new_argv[] = sprintf(max_misses)*/
    
}

/******************************************************************************/
