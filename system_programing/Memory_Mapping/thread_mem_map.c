#include <pthread.h>	/*	pthread_create, pthread_join			*/
#include <stdlib.h>     /*  rand                                    */

#include "utils.h" 

#define NUM_OF_THREADS 2

static void *ThreadRoutine(void *params); 

__thread int th_var = 0;

static pthread_key_t tls_key = {0}; 

enum {SUCCESS = 0, FAIL = 1};

int main(void)
{
    int status = SUCCESS;
	pthread_t threads[NUM_OF_THREADS] = {0};
	size_t i = 0; 
	
    status = pthread_key_create(&tls_key, NULL); 

	for(i = 0 ; i < NUM_OF_THREADS ; ++i)
	{
		status = pthread_create(&threads[i], NULL, ThreadRoutine, NULL);
	}
	
	return SUCCESS; 
	
}

static void *ThreadRoutine(void *params)
{
    int status = SUCCESS;
    int on_stack = rand(); 
    void *tls = malloc(10);
    
    
    status = pthread_setspecific(tls_key, tls);
    
    return NULL;
}
