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

/*******************************************************************************
                                typedefs
*******************************************************************************/

typedef void *(*routine_func)(void *);

/*******************************************************************************
                                  enums
*******************************************************************************/

enum {SEM_INIT_VAL = 0, MAKE_SEM_FOR_THREADS = 0, BUFF_SIZE = 11};

/*******************************************************************************
                                global variables
*******************************************************************************/

sem_t g_sem_is_wd_exist; 

char *g_wd_filename = "./WD_app.c"

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/

int MakeMeImmortal(int argc_, char *argv_[], size_t interval_, 
                                                             size_t max_misses_)
{
    int status = 0; 
    
    /*wd_params_ty *wd_struct = NULL */
    wd_params_ty *wd_struct = NULL;
    
    /*make new_argv pointer*/
    char **new_argv = NULL; 
    
    /*asserts*/
    assert(NULL != argv_); 
    assert(0 < argc_); 
    assert(0 < interval_);
    assert(1 < max_misses_);
    
    /*make new_argv for WD_APP*/
    MakeArgv(argc_, argv_[], interval_, max_misses_, new_argv); 
    
    /*MakeWDStruct(argc_, argv_[], interval_, max_misses_, wd_struct)*/
    status = MakeWDStruct(argc_, new_argv, interval_, max_misses_, wd_struct); 
    
    /*check for fail*/
    RETURN_IF_BAD(SUCCESS != status, "MakeWDStruct failed!", FAIL);
    
    /*MakeSemaphore*/
    status = MakeSemaphore(); 
    
    /*check for fail*/
    RETURN_IF_BAD(SUCCESS != status, "MakeSemaphore failed!", FAIL);
    
    /*MakeThread()*/
    status = MakeThread();
    
    /*check for fail*/
    RETURN_IF_BAD(SUCCESS != status, "MakeThread failed!", FAIL); 

    /*return SUCCESS*/
    return SUCCESS; 
}

static int MakeWDStruct(int argc_, char **new_argv, size_t interval_, 
                                   size_t max_misses_, wd_params_ty *wd_struct_)
{
    int status = 0; 
    
    /*make dynamic allocation for wd_struct_ return if failed*/
    wd_struct_ = (wd_params_ty *)malloc(sizeof(wd_params_ty)); 
    
    /*check for fails*/
    RETURN_IF_BAD(NULL != wd_struct_, "wd_struct malloc failed!", FAIL);
    
    /*init struct*/
    wd_struct_->argc = argc;
    wd_struct_->argv = new_argv;
    wd_struct_->interval = interval_;
    wd_struct_->max_misses = max_misses_; 
    wd_struct_->is_child = FALSE; 
    
    /*return SUCCESS*/
    return SUCCESS;
}

static int MakeSemaphore(void)
{
    int status = 0; 
    
    /*struct timespec *sem_wait_time*/
    struct timespec *sem_wait_time; 
    
    /*sem_init - check for fails*/
    status = sem_init(&sem_is_wd_exist, MAKE_SEM_FOR_THREADS, SEM_INIT_VAL);

    RETURN_IF_BAD_CLEAN(SUCCESS == status ,"sem init fail", FAIL, 
                                                               free(wd_params));
    /*return SUCCESS*/
    return SUCCESS; 
}

static int MakeThread(void)
{
    /*pthread_attr_t attr*/
    
    /*pthread_t new_thread*/
    
    /*MakeAttr(attr)*/
    
    /*pthread_create with casting to routine func the last argument is wd_params*/
	/*check for fail*/
	
	/*set wait time for the semaphore with InitWaitTime*/
	
	/*sem_wait with SemTimedWait*/
	
	/*pthread_attr_destroy check for fails*/
	
	/*return SUCCESS*/
}

static int MakeAttr(pthread_attr_t *attr)
{
    /*init attr check for fails*/
    
    /*set attr detached check for fails*/
    
    /*return SUCCESS*/
}

static void InitWaitTime(struct timespec *sem_wait_time)
{
    /*set tv_sec to 5 sec*/
    
    /*set tv_nsec to 0 nanoseconds*/
}

static int SemTimedWait(struct timespec *sem_wait_time)
{
    /*while(status != SUCCESS)*/
        /*status = sem_timedwait(g_sem_is_wd_exist)*/
        
    /*return SUCCESS*/   
}

static int MakeArgv(int argc_, char *argv_[], size_t interval_, 
                                            size_t max_misses_, char **new_argv)
{
    int status = 0; 
    
    char str_interval[BUFF_SIZE] = {'\0'}; 
    char str_max_misses[BUFF_SIZE] = {'\0'}; 
    
    /*allocate resorces for new_argv of size argc+3 * sizeof (char *)*/
    new_argv = (char **)malloc((argc_ + 3) * sizeof(char *)); 
     
    /*check for fails*/
    RETURN_IF_BAD(NULL != new_argv, "new_argv malloc failed!", FAIL);
    
    /*make strins of interval_ and max_misses_*/
    status = sprintf(str_interval, "%lu", interval_);
    
    RETURN_IF_BAD_CLEAN(status > 0, "sprintf for interval failed", FAIL);
    
    status = sprintf(str_max_misses, "%lu", max_misses_); 
    
    RETURN_IF_BAD_CLEAN(status > 0, "sprintf for max_misses failed", FAIL);
    
    /*set zeroth place in new_argv to wd_app filename*/
    new_argv[0] = g_wd_filename; 
    
    /*set the first member of argv to interval*/
    new_argv[1] =  str_interval; 
    
    /*set the sacond member of argv to max misses*/
    new_argv[2] =  str_max_misses; 
    
    /*memcpy argv to new argv from the third member untill the end*/
    memcpy(new_argv[3], argv_, argc*sizeof(char *));
    
    /*return SUCCESS*/
    return SUCCESS; 
}
