/*parent process*/
/*compile pong (to pong.out) before compiling this file (ping)*/
#define _POSIX_SOURCE 	/* not using standart c */ 
#define _POSIX_C_SOURCE 199309L	/*siginfo_t*/ 

#include <signal.h>		/*	sigaction, signals	*/
#include <unistd.h>		/*	fork, getpid		*/
#include <sys/types.h>	/*	pid_t				*/
#include <stdio.h>		/*	puts				*/
#include <stdlib.h> 	/*	itoa				*/

#define UNUSED(x) (void)(x)
#define UNINITIALIZED_PID 0

enum {
	KILL_ERROR = -1, 
	SIG_ACT_ERROR = -1,
	EXEC_FAIL = -1, 
	SUCCESS = 0, 
	CHILD_PID = 0
	};  
	
enum {FALSE = 0, TRUE = 1};  

pid_t child_pid = UNINITIALIZED_PID;

static void Handler(int signum, siginfo_t *siginfo, void *params);

int main(void)
{
	int sigact_res = -1; 
	struct sigaction parent; 
	int kill_res = -1; 
	int  pid = getpid();
	char buffer[100];
	sprintf(buffer, "%d", pid);

	
	/*initialization of structs*/
	parent.sa_sigaction = Handler;
	parent.sa_flags = SA_SIGINFO;
	sigemptyset(&parent.sa_mask);
	
	/*init sigusr2*/
	sigact_res = sigaction(SIGUSR2, &parent, NULL);
	
	
	if(SIG_ACT_ERROR == sigact_res)
	{
		return SIG_ACT_ERROR; 
	}
	
	while(TRUE)
	{
		pause(); 
		
		kill_res = kill(child_pid, SIGUSR1); 
		
		
		if(KILL_ERROR == kill_res)
		{
			return KILL_ERROR; 
		}
		
	}
	
	return SUCCESS; 	
}

static void Handler(int signum, siginfo_t *siginfo, void *params)
{
	UNUSED(signum); 
	UNUSED(params); 
	
	if(UNINITIALIZED_PID == child_pid) 
	{
		child_pid = siginfo->si_pid;
	}
	
	puts("ping"); 
	sleep(1); 
}
