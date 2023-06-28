#include <pthread.h>	/*	pthread_create, pthread_join			*/
#include <unistd.h> 	/*	sleep									*/
#include <string.h>		/*	strlen									*/
#include <stdio.h>		/*	putchar									*/
#include <stdlib.h>		/*	malloc, free							*/
#include <stdatomic.h> 	/*	atomic_int, atomic_load, atomic_store	*/

#define SLEEP_TIME 2
#define UNUSED(x) (void)(x)

enum  {SUCCESS = 0, 
	  THREAD_CREATE_FAIL = 1,
	  THREAD_JOIN_FAIL = 2, 
	  MALLOC_FAIL = 3
	  }; 
	  
enum {FALSE = 0, TRUE = 1}; 

static void *Consumer(void *params); 

static void *Producer(void *params); 

static char *Produce(void);

static void Consume(char *message_);

char *g_message = "This is a message to be sent"; 

char *g_allo_message_ptr = NULL;

volatile atomic_int g_may_cons = FALSE;
volatile atomic_int g_may_prod = TRUE; 

int main(void)
{
	pthread_t consumer; 
	pthread_t producer;
	
	if(SUCCESS != pthread_create(&producer, NULL, Producer, NULL))
	{
		return THREAD_CREATE_FAIL; 
	} 
	
	if(SUCCESS != pthread_create(&consumer, NULL, Consumer, NULL))
	{
		return THREAD_CREATE_FAIL; 
	}

	
	if(SUCCESS != pthread_join(consumer, NULL))
	{
		return THREAD_JOIN_FAIL; 
	}
	
	if(SUCCESS != pthread_join(producer, NULL))
	{
		return THREAD_CREATE_FAIL; 
	}
	
	return SUCCESS; 
}

static void *Producer(void *params_)
{	
	char *message = NULL;
	
	UNUSED(params_);
	
	while(TRUE)
	{
		message = Produce(); 
		
		if(NULL == message)
		{
			return (void *)((size_t)MALLOC_FAIL); 
		}
	
		while(FALSE == atomic_load(&g_may_prod))
		{}
		
		g_allo_message_ptr = message;
			
		atomic_store(&g_may_prod ,FALSE); 
				
		atomic_store(&g_may_cons ,TRUE); 
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
	char *message = NULL; 
	
	UNUSED(params_);
	
	while(TRUE)
	{		
		while(FALSE == atomic_load(&g_may_cons))
		{}
		
		message = g_allo_message_ptr; 
		g_allo_message_ptr = NULL;
		
		atomic_store(&g_may_prod ,TRUE);
		
		atomic_store(&g_may_cons ,FALSE); 
		
		Consume(message);
		
		message = NULL;
	}
	return SUCCESS; 
}

static void Consume(char *message_)
{		
	puts(message_); 
	
	free(message_); 
	message_ = NULL;
	
	puts("");
	
}
