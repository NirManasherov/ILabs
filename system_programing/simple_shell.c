/*******************************************************************************
* Author: Nir Manasherov
* Project: Simple Shell
* Reviewer: 
* Date: 23.02.2023
* Version: 1.0
*******************************************************************************/
/*******************************************************************************
                                  libraries
*******************************************************************************/

#include <assert.h> 		/*	assert			*/
#include <string.h>			/*	strcmp, strlen	*/
#include <sys/types.h>		/*	pid_t			*/
#include <stdio.h>			/*	fgets, stdin	*/
#include <stdlib.h>			/*	exit			*/
#include <unistd.h>			/*	fork			*/
#include <sys/wait.h>		/*	wait			*/

/*******************************************************************************
                                  macros
*******************************************************************************/   
 
#define MAX_COMMAND_LENGTH 20
#define WAITING_ANY_CHILD 0
#define EXECL_FAIL -1
#define DID_NOT_RECIEVE_CHILD_SIG -1

/*******************************************************************************
                                  enums
*******************************************************************************/

enum {FALSE = 0, TRUE = 1};

enum {SUCCESS = 0, FGETS_FAILURE = 1, FORK_FAILURE = 2, EXE_FAILURE = 3}; 

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

/*return TRUE if command is exit FALSE otherwise*/
static int IsExit(char *command); 

/*put input in command buffer*/
static char *GetInput(char *command);

/*run the fork command and handle it*/
static int RunFork(char *command); 

/*run in the system mode*/
static int RunSys(char *command); 

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/

int main(int argc, char *argv[]) 
{
	/*make a buffer for commands from the user*/
	char command[MAX_COMMAND_LENGTH] = {0};
	
	assert(0 != argc);
	assert(NULL != argv); 
	
	/*cherck if the user wants to use the fork option*/
	if(0 == strcmp(argv[1], "fork"))
	{
		while(TRUE)
		{
			/*get a command from user*/
			{
				if(NULL == GetInput(command))
				{				
					return FGETS_FAILURE; 
				}
				
				/*if exit entered*/
				if(IsExit(command))
				{
					exit(SUCCESS);
				}
				
				if (SUCCESS != RunFork(command))
				{
					return FORK_FAILURE; 
				} 
			}
		}
	}
	
	/*user chose system option*/
	else
	{
		while(TRUE)
		{
			if(SUCCESS != RunSys(command))
			{
				return FGETS_FAILURE; 
			} 
		}
	}
	
	return SUCCESS;
}

/******************************************************************************/

/*return TRUE if command is exit FALSE otherwise*/
static int IsExit(char *command)
{
	return (0 == strncmp("exit", command, strlen("exit"))); 
}

/******************************************************************************/

/*put input in command buffer*/
static char *GetInput(char *command)
{
	return fgets(command, MAX_COMMAND_LENGTH, stdin);
}

/******************************************************************************/

/*run the fork command and handle it*/
static int RunFork(char *command)
{
	int status = 0;
	pid_t child_pid = 0;
	child_pid = fork();
				
	/*for child process*/
	if(0 == child_pid)
	{
		execvp("/bin/sh", &command);
		
		exit(EXE_FAILURE);
	}
				
	/*parent should wait for child death*/
	else if(0 < child_pid)
	{
		waitpid(child_pid, &status, WAITING_ANY_CHILD); 
		
		if(DID_NOT_RECIEVE_CHILD_SIG == status)
		{
			exit(DID_NOT_RECIEVE_CHILD_SIG);
		}
		
		exit(SUCCESS); 
	}
				
	else
	{
		return FORK_FAILURE;
	}
		
	return SUCCESS; 
}

/******************************************************************************/

/*run in the system mode*/
static int RunSys(char *command)
{	
	
		if(NULL == fgets(command, MAX_COMMAND_LENGTH, stdin))
		{
			return FGETS_FAILURE; 
		}
			
		/*if exit entered*/
		if(IsExit(command))
		{
			exit(SUCCESS);
		}
			
		/*execute user command*/
		system(command);
		
		return SUCCESS;
}

/******************************************************************************/
