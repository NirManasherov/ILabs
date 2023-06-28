#define _POSIX_C_SOURCE 200112L             /*  sigaction   */
#include <signal.h>     /*  kill, sigaction                 */
#include <sys/types.h>  /*  pid_t                           */
#include <stdlib.h>    /*   setenv                          */

#include "scheduler.h"
#include "watchdog.h"
#include "WD_internal.c"
#include "utils.h"

int g_app_counter = 0;

struct sigaction g_sigusr1; 

struct sigaction g_sigusr2; 

enum {ENV_ERR = -1, ENV_CANT_CHANGE = 0, ENV_CAN_CHANGE = 1}; 

enum {BUFF_SIZE = 11}; 

/*argv = [client argv, watchdog_params *watchdog_params]*/

static int SetEnvVar(); 

static void SigUsr1Handler(int signum);

static void SigUsr2Handler(int signum); 

int main(int argc, char *argv[]) 
{
    wd_params_ty wd_params; 
    
    /*init fields of g_g_sigusr1*/
    g_sigusr1.sa_handler = SigUsr1Handler;
	g_sigusr1.sa_flags = 0;
    
    /*sigusr1(sigaction, &SIGUSR1_g, NULL)*/
    RETURN_IF_BAD(SUCCESS == sigemptyset(&g_sigusr1.sa_mask), 
                                     "sigemptyset fail", FAIL);
    
    /*init fields of g_g_sigusr2*/
    g_sigusr2.sa_handler = SigUsr2Handler;
	g_sigusr2.sa_flags = 0;
    
    /*sigusr2(sigaction, &SIGUSR2_g, NULL)*/
    RETURN_IF_BAD(SUCCESS == sigemptyset(&g_sigusr2.sa_mask), 
                                     "sigemptyset fail", FAIL);
    
    /*SetEnvVar*/
    RETURN_IF_BAD(SUCCESS == SetEnvVar(), "SetEnvVar fail!", FAIL); 

    /*Watch()*/
    Watch(&wd_params); 
    
    /*return SUCCESS*/
    return SUCCESS;                               
}

static int SetEnvVar(void)
{
    /*pid_t wd_pid*/
    pid_t wd_pid; 
    char *pid_str[BUFF_SIZE] = {'\0'};
    
    wd_pid = getpid(); 
    
    RETURN_IF_BAD(0 < fprint(pid_str, "%d", wd_pid), "fprint fail!", FAIL); 
    
    /*check env var WD_PID */
    /*update env var using setenv check if fail*/
    RETURN_IF_BAD(ENV_ERR == setenv("WD_PID", pid_str, ENV_CANT_CHANGE), 
                                                           "setenv fail", FAIL);
    
    /*return success*/
    return SUCCESS; 
}

static void SigUsr1Handler(int signum)
{
    /*UNUSED(params);*/
    UNUSED(params);
    
    /*g_app_counter = 0*/
    __atomic_store(&g_app_counter ,&counter_val , __ATOMIC_SEQ_CST);
    
    /*return NULL*/
    return NULL; 
}

static void SigUsr2Handler(int signum)
{
    /*UNUSED(params);*/
    UNUSED(params);
     
    /*set env var to 0, NULL and so on...*/
    RETURN_IF_BAD(SUCCESS == unsetenv("WD_PID"), "unsetenv fail!", FAIL); 
    
    /*exit()*/
    exit();
    
    /*return NULL*/
    return NULL; 
}


