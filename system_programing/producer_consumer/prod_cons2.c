#include <pthread.h>	/*	pthread_create, pthread_join	*/
#include <unistd.h> 	/*	sleep							*/
#include <string.h>		/*	strlen							*/
#include <stdio.h>		/*	putchar							*/
#include <stdlib.h>		/*	malloc, free					*/


#include "slist.h"

#define SLEEP_TIME 2
#define NUM_OF_CON_PROD 4
#define UNUSED(x) (void)(x)
#define BUFF_SIZE 100

enum  {SUCCESS = 0, 
	  THREAD_CREATE_FAIL = 1,
	  THREAD_JOIN_FAIL = 2, 
	  SLIST_CREATE_FAIL = 3, 
	  MUTEX_INIT_FAIL = 4, 
	  MUTEX_DESTROY_FAIL = 5, 
	  SLIST_INSERT_FAIL = 6
	  }; 
	  
enum {FALSE = 0, TRUE = 1}; 

static void *Consumer(void *params); 

static void *Producer(void *params); 

static char *Produce(void); 

static void Consume(char *message_, iter_ty iter_);

char *g_message = "This is a message to be sent"; 

pthread_mutex_t g_slist_mutex;

slist_ty *g_slist;

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
	
	g_slist = SlistCreate();
	
	if(NULL == g_slist)
	{
		return SLIST_CREATE_FAIL; 
	}
	
	if(SUCCESS != pthread_mutex_init(&g_slist_mutex, NULL))
	{
		return MUTEX_INIT_FAIL;
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
	
	if(SUCCESS != pthread_mutex_destroy(&g_slist_mutex))
	{
		return MUTEX_DESTROY_FAIL;
	}
	
	SlistDestroy(g_slist);
	
	return SUCCESS; 
}

static void *Producer(void *params_)
{
	size_t serial_num = *((size_t *)params_); 
	char *message = NULL; 
	
	UNUSED(params_);
	
	while(TRUE)
	{
		
		message = Produce(); 
		
		pthread_mutex_lock(&g_slist_mutex);
		
		if(0 == SlistCount(g_slist))
		{								
			if(FALSE == SlistIterIsEqual(SlistIterEnd(g_slist),
				 							SlistInsert(SlistIterEnd(g_slist),
				 										(const void *)message)))
			{
				return (void *)((size_t)SLIST_INSERT_FAIL);
			}
			
		}
			
		printf(" printed by producer %ld\n", serial_num);
		pthread_mutex_unlock(&g_slist_mutex); 
	
		
		sleep(SLEEP_TIME); 	
	}
	
	return SUCCESS; 
}

static char *Produce(void)
{
	char *message = NULL;
	
	message = (char *)malloc(sizeof(char) * (strlen(g_message) + 1));
	
	memmove(message, g_message, (strlen(g_message) + 1)*sizeof(char));
	
	return message; 
}

static void *Consumer(void *params_)
{
	iter_ty curr_iter;
	size_t serial_num = *((size_t *)params_);
	char *data = NULL;  
	
	UNUSED(params_);
	
	while(TRUE)
	{
		while(SUCCESS != pthread_mutex_trylock(&g_slist_mutex))
		{
			if(0 != SlistCount(g_slist))
			{
				curr_iter = SlistIterBegin(g_slist);
					
				data = (char *)SlistIterGetData(curr_iter);
				
			}				
			
			pthread_mutex_unlock(&g_slist_mutex); 
			
			if((data != NULL) && (!SlistIterIsEqual(curr_iter, 		
														SlistIterEnd(g_slist))))
			{
				Consume(data, curr_iter); 
				data = NULL;
			}
			
			printf(" printed by consumer %ld\n", serial_num);
		}
		
		sleep(SLEEP_TIME); 
	}
	return SUCCESS; 
}

static void Consume(char *message_, iter_ty iter_)
{		
	printf("%s", message_); 
	
	free(message_); 
	message_ = NULL;
	
	
	SlistRemove(iter_);
	
}
