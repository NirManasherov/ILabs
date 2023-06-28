#include <semaphore.h>	/*	sem_open, sem_close sem_getvalue	*/
#include <fcntl.h>		/*	mode_t 								*/
#include <sys/stat.h>	/*	mode_t vars							*/
#include <stddef.h>		/*	NULL								*/
#include <stdio.h>		/*	puts, printf						*/

#define BUFFER_SIZE 100; 

enum {FALSE = 0, TRUE = 1};

enum {
	SUCCESS = 0,
	FGET_FAIL = 1, 
	SEM_INIT_FAIL = 2, 
	SEM_CLOSE_FAIL = 3,
	SEM_GET_VAL_FAIL = 3
	};  

int main(void) 
{
	sem_t *sem = NULL; 
	mode_t mode = 0;
	char buffer[BUFFER_SIZE] = {0}; 
	int val = 0; 
	int init_val = 0; 
	char *buff_runner = NULL; 
	int sem_val = 0; 
	
	mode = S_IRGRP + S_IROTH + S_IRUSR + S_IWUSR; 
	
	/*use sem_open to make new semophore*/
	sem = sem_open("sem", O_CREAT, mode, 1);
	
	if(SEM_FAILED == sem)
	{
		return SEM_FAILED; 
	}
	
	init_value = GetVal(semaphore);
	
	while(TRUE)
	{
		if(NULL == fgets(bufferr, BUFFER_SIZE, stdin))
		{
			return FGET_FAIL; 
		}
		
		switch(buffer[0])
		{
			case 'X': 
				
				puts("exit"); 
				
				if(SUCCESS != sem_init(sem, 1)
				{
					/*exit with error*/
					exit(SEM_INIT_FAIL); 
				}
				
				if(SUCCESS != sem_close(sem))
				{
					return SEM_CLOSE_FAIL; 
				}
				
				return SUCCESS; 
				
				break; 
			
			case 'V':
			
				if(SUCCESS != sem_getvalue(sem, &val)
				{
					return SEM_GET_VAL_FAIL; 
				}
				
				printf("value of semaphore is: %d\n", val); 
				
				break; 
				
			 case 'D': 
			 	
			 	val = atoi(buffer); 
			 	val = (-1)*val;
			 	
			 	if(SUCCESS != sem_getvalue(sem, &sem_val))
			 	{
			 		return SEM_GET_VAL_FAIL;
			 	}
			 	
			 	InitVal(sem, init_value);
			 	
			 	buff_runner = buffer; 
			 	
			 	while('\0' != *(buff_runner + 2))
			 	{
			 		++buff_runner; 
			 	}
			 	
			 	if('U' != *buff_runner)
			 	{
			 		puts("not UNDO"); 
			 		
			 		init_val += val; 
			 	}
			 	
			 	val += sem_val;
			 	
			 	if(SUCCESS != sem_init(sem, 1, val))
			 	{
			 		return SEM_INIT_FAIL;
			 	}
			 	
			 case 'I':
			 	
			 	val = atoi(buffer);
			 	
			 	if(SUCCESS != sem_getvalue(sem, &sem_val))
			 	{
			 		return SEM_GET_VAL_FAIL;
			 	}
			 	
			 	InitVal(sem, init_value);
			 	
			 	buff_runner = buffer; 
			 	
			 	while('\0' != *(buff_runner + 2))
			 	{
			 		++buff_runner; 
			 	}
			 	
			 	if('U' != *buff_runner)
			 	{
			 		puts("not UNDO"); 
			 		
			 		init_val += val; 
			 	}
			 	
			 	val += sem_val;
			 	
			 	if(SUCCESS != sem_init(sem, 1, val))
			 	{
			 		return SEM_INIT_FAIL;
			 	}
			 	
		}
	}
}	
