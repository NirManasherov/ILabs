/*compile with flag -pthread*/
#include <pthread.h>		/*	pthread_create, pthread_destroy */
#include <stdio.h>			/*	printf							*/
#include <unistd.h>			/*	sleep							*/

#define ARR_SIZE 100000
#define SLEEP_TIME 10

enum {SUCCESS = 0, FAIL = 1}; 

int int_arr[ARR_SIZE] = {0}; 

static void *UpdateIndex(void *index);

int main(void)
{
	size_t i = 0; 
	pthread_t id[ARR_SIZE] = {0}; 
	
	for(i = 0 ; i < ARR_SIZE ; ++i)
	{
		printf("%ld\n", i);
		
		if(SUCCESS != pthread_create(&(id[i]), NULL, UpdateIndex, (void *)i))
		{
			while(SUCCESS != 
						pthread_create(&(id[i]), NULL, UpdateIndex, (void *)i));
		}
	}
	
	sleep(SLEEP_TIME);
	
	
	for(i = 0 ; i < ARR_SIZE ; ++i)
	{
		if(int_arr[i] != (int)i)
		{
			printf("fail on index: %ld\n", i); 
			return FAIL; 
		}
	}
		
	return SUCCESS;
}	

static void *UpdateIndex(void *index)
{	
	size_t idx = (size_t)index; 
	
	int_arr[idx] = idx; 
	
	return NULL;
}
