/*compile with flag -pthread*/
#include <pthread.h>		/*	pthread_create, pthread_destroy */
#include <stdio.h>			/*	printf							*/
#include <unistd.h>			/*	sleep							*/
#include <time.h>			/*	time, time_t					*/

#define ARR_SIZE 100000
#define SLEEP_TIME 10

enum {SUCCESS = 0, FAIL = 1}; 

int int_arr[ARR_SIZE] = {0}; 

static void *UpdateIndex(void *index);

int main(void)
{
	size_t i = 0; 
	pthread_t id[ARR_SIZE] = {0}; 
	time_t start_time = 0; 
	time_t end_time = 0; 
	
	start_time = time(NULL); 
	
	for(i = 0 ; i < ARR_SIZE ; ++i)
	{
		pthread_create(&(id[i]), NULL, UpdateIndex, (void *)i);
	}
	
	end_time = time(NULL); 
	
	sleep(SLEEP_TIME);
	
	printf("time of performance: %ld [sec]\n", end_time - start_time); 
	
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
