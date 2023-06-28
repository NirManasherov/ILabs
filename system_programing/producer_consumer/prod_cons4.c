#include <pthread.h>	/*	pthread_create, pthread_join	*/
#include <unistd.h> 	/*	sleep							*/
#include <string.h>		/*	strlen							*/
#include <stdio.h>		/*	putchar							*/
#include <semaphore.h>	/*	sem_open, sem_close sem_getvalue*/
#include <fcntl.h>		/*	mode_t 							*/
#include <sys/stat.h>	/*	mode_t vars						*/

#include "cbuff.h"

#define SLEEP_TIME 2
#define NUM_OF_CON_PROD 4
#define UNUSED(x) (void)(x)
#define BUFF_CAPACITY 8
#define BIT_BYTE 8

enum  {SUCCESS = 0, 
	  THREAD_CREATE_FAIL = 1,
	  THREAD_JOIN_FAIL = 2, 
	  CBUFF_CREATE_FAIL = 3, 
	  MUTEX_INIT_FAIL = 4, 
	  MUTEX_DESTROY_FAIL = 5, 
	  CBUFF_INSERT_FAIL = 6, 
	  SEM_CLOSE_FAIL = 7, 
	  SEM_OPEN_FAILED = 8
	  }; 
	  
enum {FALSE = 0, TRUE = 1}; 

static void *Consumer(void *params); 

static void *Producer(void *params); 

char *message = "This is a message to be sent"; 

pthread_mutex_t cbuff_mutex;

sem_t sem_read; 
sem_t sem_write; 

mode_t mode = S_IRGRP + S_IROTH + S_IRUSR + S_IWUSR;

c_buff_ty *cbuff = NULL;

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
	
	cbuff = CBuffCreate(BUFF_CAPACITY*BIT_BYTE);
	
	if(NULL == cbuff)
	{
		return CBUFF_CREATE_FAIL; 
	}
	
	if(SUCCESS != pthread_mutex_init(&cbuff_mutex, NULL))
	{
		return MUTEX_INIT_FAIL;
	}
		
	if(SUCCESS != sem_init(&sem_read, 0, 0))
	{
		return SEM_OPEN_FAILED; 
	}
	
	if(SUCCESS != sem_init(&sem_write, 0, BUFF_CAPACITY))
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
	
	if(SUCCESS != pthread_mutex_destroy(&cbuff_mutex))
	{
		return MUTEX_DESTROY_FAIL;
	}
	
	if(SUCCESS != sem_destroy(&sem_read))
	{
		return SEM_CLOSE_FAIL; 
	}
	
	if(SUCCESS != sem_destroy(&sem_write))
	{
		return SEM_CLOSE_FAIL; 
	}
	
	CBuffDestroy(cbuff);
	
	return SUCCESS; 
}

static void *Producer(void *params)
{
	size_t serial_num = *((size_t *)params);
	
	UNUSED(params);
	puts("1"); 
	while(TRUE)
	{		
		puts("2"); 
		sem_wait(&sem_write);
		puts("3"); 
		pthread_mutex_lock(&cbuff_mutex);
		puts("4"); 	
		if(BUFF_CAPACITY != CBuffSize(cbuff))
		{
			/*pthread_mutex_lock(&cbuff_mutex); */
								
			CBuffWrite(cbuff, message, sizeof(void *));
		}
			
		printf(" printed by producer %ld\n", serial_num);
		pthread_mutex_unlock(&cbuff_mutex); 
		sem_post(&sem_read);
				
		sleep(SLEEP_TIME); 
	}
	
	return SUCCESS; 
}

static void *Consumer(void *params)
{
	size_t serial_num = *((size_t *)params); 
	char message_init = '\0'; 
	char *message_ptr = &message_init; 
	
	UNUSED(params);		
	
	while(TRUE)
	{	
		sem_wait(&sem_read);
				
		pthread_mutex_lock(&cbuff_mutex);
			
		if(TRUE != CBuffIsEmpty(cbuff))
		{
		 	CBuffRead(cbuff, message_ptr, sizeof(void *));
			 	
		 	puts(message_ptr); 				
		}
			
		printf(" printed by consumer %ld\n", serial_num);
		pthread_mutex_unlock(&cbuff_mutex); 
		sem_post(&sem_write);
		
		sleep(SLEEP_TIME); 
	}
	return SUCCESS; 
}
