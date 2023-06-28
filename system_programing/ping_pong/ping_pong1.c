#define _POSIX_SOURCE 	/* not using standart c */  
#include <signal.h>		/*	sigaction, signals	*/
#include <unistd.h>		/*	fork, getpid		*/
#include <sys/types.h>	/*	pid_t				*/
#include <stdio.h>		/*	puts				*/

#define UNUSED(x) (void)(x)

enum {
	KILL_ERROR = -1, 
	SIG_ACT_ERROR = -1, 
	FORK_ERROR = -1, 
	SUCCESS = 0, 
	CHILD_PID = 0
	};  
	
enum {FALSE = 0, TRUE = 1};  

static void ParentHandler(int signum);

static void ChildHandler(int signum);

int main(void)
{
	int sigact_res = -1; 
	struct sigaction child; 
	struct sigaction parent; 
	pid_t pid = 0;
	int kill_res = -1; 
	
	/*initialization of structs*/
	child.sa_handler = ChildHandler;
	parent.sa_handler = ParentHandler;
	child.sa_flags = 0; 
	parent.sa_flags = 0;
	sigemptyset(&child.sa_mask); 
	sigemptyset(&parent.sa_mask); 
	
	pid = fork(); 
	
	if(FORK_ERROR == pid)
	{
		return FORK_ERROR; 
	}
	
	/*init sigusr1*/
	sigact_res = sigaction(SIGUSR1, &child, NULL);
	
	if(SIG_ACT_ERROR == sigact_res)
	{
		return SIG_ACT_ERROR; 
	} 
	
	/*init sigusr2*/
	sigact_res = sigaction(SIGUSR2, &parent, NULL);
	
	if(SIG_ACT_ERROR == sigact_res)
	{
		return SIG_ACT_ERROR; 
	}
	
	/*parent process*/
	if(CHILD_PID != pid)
	{
		while(TRUE)
		{
			sleep(1); 
			
			kill_res = kill(pid, SIGUSR1); 
			
			if(KILL_ERROR == kill_res)
			{
				return KILL_ERROR; 
			}
			
			pause(); 
		}
	}
	
	/*child process*/
	else
	{
		while(TRUE)
		{
			pause();
			
			sleep(1); 
			
			kill_res = kill(getppid(), SIGUSR2); 
			
			if(KILL_ERROR == kill_res)
			{
				return KILL_ERROR; 
			}
			
		}
	}
	
	return SUCCESS; 
}

static void ParentHandler(int signum)
{
	UNUSED(signum); 
	
	puts("pong"); 
}

static void ChildHandler(int signum)
{
	UNUSED(signum); 
	
	puts("ping"); 
}
