/*child process*/
/*should compile this file second and add to the command line the other process 
PID*/
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

static void Handler(int signum);

int main(int argc, char *argv[])
{
	int sigact_res = -1; 
	struct sigaction child; 
	pid_t pid = 0;
	int kill_res = -1; 
	
	/*initialization of structs*/
	child.sa_handler = Handler;
	child.sa_flags = 0; 
	sigemptyset(&child.sa_mask); 
	
	/*init sigusr1*/
	sigact_res = sigaction(SIGUSR1, &child, NULL);
	
	if(SIG_ACT_ERROR == sigact_res)
	{
		return SIG_ACT_ERROR; 
	} 
	
	kill_res = kill(atoi(argv[1]), SIGUSR2); 
	
	/*child process*/
	while(TRUE)
	{
		pause();
		
		kill_res = kill(atoi(argv[1]), SIGUSR2); 
			
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
	
	puts("pong"); 
	sleep(1); 
}
