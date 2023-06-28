#include <pthread.h>	/*	pthread_create, pthread_join	*/
#include <unistd.h> 	/*	sleep							*/
#include <string.h>		/*	strlen							*/
#include <stdio.h>		/*	putchar							*/
#include <semaphore.h>	/*	sem_open, sem_close sem_getvalue*/
#include <fcntl.h>		/*	mode_t 							*/
#include <sys/stat.h>	/*	mode_t vars						*/

#include "slist.h"

#define SLEEP_TIME 2
#define NUM_OF_CON_PROD 4
#define UNUSED(x) (void)(x)

enum  {SUCCESS = 0, 
	  THREAD_CREATE_FAIL = 1,
	  THREAD_JOIN_FAIL = 2, 
	  SLIST_CREATE_FAIL = 3, 
	  MUTEX_INIT_FAIL = 4, 
	  MUTEX_DESTROY_FAIL = 5, 
	  SLIST_INSERT_FAIL = 6, 
	  SEM_CLOSE_FAIL = 7, 
	  SEM_OPEN_FAILED = 8
	  }; 
	  
enum {FALSE = 0, TRUE = 1}; 

static void *Consumer(void *params); 

static void *Producer(void *params); 

char *message = "This is a message to be sent"; 

pthread_mutex_t slist_mutex;

sem_t *sem = NULL; 

mode_t mode = S_IRGRP + S_IROTH + S_IRUSR + S_IWUSR;

slist_ty *slist;

int main(void)
{
	pthread_t consumer; 
	pthread_t producer; 
	size_t i = 0; 
	size_t serial_num_arr[NUM_OF_CON_PROD]; 
	
	for(i = 0 ; i < NUM_OF_CON_PROD ; ++i)
	{
		serial_num_arr[i] = i + 1; 
	}
	
	slist = SlistCreate();
	
	if(NULL == slist)
	{
		return SLIST_CREATE_FAIL; 
	}
	
	if(SUCCESS != pthread_mutex_init(&slist_mutex, NULL))
	{
		return MUTEX_INIT_FAIL;
	}
	
	sem = sem_open("sem", O_CREAT, mode, 1);
	
	if(SEM_FAILED == sem)
	{
		return SEM_OPEN_FAILED; 
	}
	
	for(i = 0; i < NUM_OF_CON_PROD ; ++i)
	{
		if(SUCCESS != pthread_create(&consumer, NULL, Consumer, 
												   (void *)(serial_num_arr +i)))
		{
			return THREAD_CREATE_FAIL; 
		}
	}
	
	sleep(SLEEP_TIME);
	
	for(i = 0; i < NUM_OF_CON_PROD ; ++i)
	{
		if(SUCCESS != pthread_create(&producer, NULL, Producer, 
												  (void *)(serial_num_arr + i)))
		{
			return THREAD_CREATE_FAIL; 
		}
	}
	
	
	sleep(SLEEP_TIME); 
	
	for(i = 0; i < NUM_OF_CON_PROD ; ++i)
	{
		if(SUCCESS != pthread_join(consumer, NULL))
		{
			return THREAD_JOIN_FAIL; 
		}
		
		if(SUCCESS != pthread_join(producer, NULL))
		{
			return THREAD_CREATE_FAIL; 
		}
	}
	
	if(SUCCESS != pthread_mutex_destroy(&slist_mutex))
	{
		return MUTEX_DESTROY_FAIL;
	}
	
	if(SUCCESS != sem_close(sem))
	{
		return SEM_CLOSE_FAIL; 
	}
	
	SlistDestroy(slist);
	
	return SUCCESS; 
}

static void *Producer(void *params)
{
	size_t i = 0;
	char *message_runner = message;
	size_t serial_num = *((size_t *)params); 
	
	UNUSED(params);
	
	while(TRUE)
	{		
		sem_wait(sem);
		
		while(SUCCESS != pthread_mutex_trylock(&slist_mutex))
		{			
			if(0 == SlistCount(slist))
			{
				pthread_mutex_lock(&slist_mutex); 
				
				i = 0;
				
				while(i <= strlen(message))
				{
					if(FALSE == SlistIterIsEqual(SlistIterEnd(slist),
					 							SlistInsert(SlistIterEnd(slist),
					 							(const void *)message_runner)))
					{
						++message_runner;
						++i;
					}
					
				}
			}
				
			printf(" printed by producer %ld\n", serial_num);
			pthread_mutex_unlock(&slist_mutex); 
			sem_post(sem);
		}
		
		sleep(SLEEP_TIME); 
		
		message_runner = message;
	}
	
	return SUCCESS; 
}

static void *Consumer(void *params)
{
	iter_ty curr_iter;
	size_t serial_num = *((size_t *)params); 
	
	UNUSED(params);		
	
	while(TRUE)
	{	
		sem_wait(sem);
				
		while(SUCCESS != pthread_mutex_trylock(&slist_mutex))
		{	
			if(0 != SlistCount(slist))
			{
			 	while(0 != SlistCount(slist))
				{
					curr_iter = SlistIterBegin(slist);
						
					putchar(*((char *)SlistIterGetData(curr_iter)));
					
					SlistRemove(curr_iter);
				}				
			}
			
			printf(" printed by consumer %ld\n", serial_num);
			pthread_mutex_unlock(&slist_mutex); 
			sem_post(sem);
			
		}
		
		sleep(SLEEP_TIME); 
	}
	return SUCCESS; 
}
