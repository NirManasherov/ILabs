/*parent process*/
/*compile pong (to pong.out) before compiling this file (ping)*/
#define _POSIX_SOURCE 	/* not using standart c */  
#include <signal.h>		/*	sigaction, signals	*/
#include <unistd.h>		/*	fork, getpid		*/
#include <sys/types.h>	/*	pid_t				*/
#include <stdio.h>		/*	puts				*/
#include <stdlib.h> 	/*	itoa				*/	

#define UNUSED(x) (void)(x)

enum {
	KILL_ERROR = -1, 
	SIG_ACT_ERROR = -1,
	EXEC_FAIL = -1, 
	FORK_ERROR = -1, 
	SUCCESS = 0, 
	CHILD_PID = 0
	};  
	
enum {FALSE = 0, TRUE = 1};  

static void Handler(int signum);

int main(void)
{
	int sigact_res = -1; 
	struct sigaction parent; 
	int kill_res = -1; 
	int  pid = getpid();
	char buffer[100];
	char *argv_for_child[3];
	sprintf(buffer, "%d", pid);
	
	argv_for_child[0] = "./pong.out";
	argv_for_child[1] =  buffer;
	argv_for_child[2] =  NULL;
	
	/*initialization of structs*/
	parent.sa_handler = Handler;
	parent.sa_flags = 0;
	sigemptyset(&parent.sa_mask);
	
	/*init sigusr2*/
	sigact_res = sigaction(SIGUSR2, &parent, NULL);
	
	pid = fork();
	
	if(FORK_ERROR == pid)
	{
		return FORK_ERROR; 
	}

	
	if(SIG_ACT_ERROR == sigact_res)
	{
		return SIG_ACT_ERROR; 
	}
	
	if (CHILD_PID == pid)
	{
		if (EXEC_FAIL == execvp(argv_for_child[0], argv_for_child))
		{
			return EXEC_FAIL; 
		}
	}
	
	
	while(TRUE)
	{
		pause(); 
		
		kill_res = kill(pid, SIGUSR1); 
		
		
		if(KILL_ERROR == kill_res)
		{
			return KILL_ERROR; 
		}
		

	
	}
	
	return SUCCESS; 	
}

static void Handler(int signum)
{
	UNUSED(signum); 
	
	puts("ping"); 
	sleep(1); 
}
