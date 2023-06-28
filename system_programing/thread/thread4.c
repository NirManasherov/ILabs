/*compile with flag -pthread*/
#include <pthread.h>		/*	pthread_create, pthread_destroy */
#include <stdio.h>			/*	printf							*/
#include <unistd.h>			/*	sleep							*/
#include <ulimit.h>			/*	ulimit							*/
#include <time.h>			/*	time, time_t					*/

#define ARR_SIZE 32752
#define SLEEP_TIME 10
#define DIV_NUM 123456789012

enum {SUCCESS = 0, FAIL = 1}; 

int int_arr[ARR_SIZE] = {0}; 

static void *UpdateIndex(void *index);

int main(void)
{
	size_t i = 0; 
	pthread_t id[ARR_SIZE] = {0}; 
	void *join_return_val = NULL; 
	size_t sum_of_divisors = 0; 
	time_t start_time = 0; 
	time_t end_time = 0; 
	
	start_time = time(NULL);
	
	for(i = 0 ; i < ARR_SIZE ; ++i)
	{
		if(SUCCESS != pthread_create(&(id[i]), NULL, UpdateIndex, (void *)i))
		{
			printf("pthread_create failed on index %ld\n", i);
		}
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
	
	for(i = 0 ; i < ARR_SIZE ; ++i)
	{
		if(SUCCESS != pthread_join((id[i]), &join_return_val))
		{
			printf("pthread_join failed on index %ld\n", i);
		}
		
		sum_of_divisors += (size_t)join_return_val; 
	}
	
	
	return SUCCESS;
}	

static void *UpdateIndex(void *index)
{	
	size_t idx = (size_t)index; 
	size_t sum_of_divisors = 0;
	size_t i = 0; 
	
	int_arr[idx] = idx; 
	
	for(i = idx*100 + 1 ; i < (idx+1)*100 ; ++i)
	{
		if(0 == DIV_NUM % i)
		{
			sum_of_divisors += i;
		} 
	}
	
	return (void *)sum_of_divisors;
}
