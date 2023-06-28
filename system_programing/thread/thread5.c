/*compile with flag -pthread*/
#include <pthread.h>		/*	pthread_create, pthread_destroy */
#include <stdio.h>			/*	printf							*/
#include <unistd.h>			/*	sleep							*/
#include <ulimit.h>			/*	ulimit							*/
#include <time.h>			/*	time, time_t					*/
#include <stdlib.h>			/*	malloc, free					*/

#define ARR_SIZE 32752
#define SLEEP_TIME 10
#define DIV_NUM 1234567890
#define NUM_OF_THREADS_CHECKED 14

enum {SUCCESS = 0, FAIL = 1}; 

int int_arr[ARR_SIZE] = {0}; 

static void *UpdateIndex(void *index);

size_t num_of_threads[NUM_OF_THREADS_CHECKED] = {0}; 

size_t j = 0; 

int main(void)
{
	size_t i = 0;  
	pthread_t *id = NULL; 
	void *join_return_val = NULL; 
	size_t sum_of_divisors = 0; 
	time_t start_time = 0; 
	time_t end_time = 0; 
	
	num_of_threads[0] = 1; 
	
	for(i = 1 ; i < NUM_OF_THREADS_CHECKED ; ++i)
	{
		num_of_threads[i] = i + 1; 
	}

	for(j = 0 ; j < NUM_OF_THREADS_CHECKED ; ++j)
	{
		id = (pthread_t *)calloc(num_of_threads[j], sizeof(pthread_t)); 
		
		if(1 != num_of_threads[j])
		{
			start_time = time(NULL);
			
			for(i = 0 ; i < num_of_threads[j] ; ++i)
			{
				if(SUCCESS != pthread_create(&(id[i]), NULL, UpdateIndex, (void *)i))
				{
					printf("pthread_create failed on index %ld\n", i);
				}
			}
			
			end_time = time(NULL); 
			
			sleep(SLEEP_TIME);

			printf("time of performance with %ld threads: %ld [sec]\n", 
				  					num_of_threads[j] + 1, (end_time - start_time));
			
			for(i = 0 ; i < num_of_threads[j] ; ++i)
			{
				if(int_arr[i] != (int)i)
				{
					printf("fail on index: %ld\n", i); 
					return FAIL; 
				}
			}
			
			for(i = 0 ; i < num_of_threads[j] ; ++i)
			{
				if(SUCCESS != pthread_join((id[i]), &join_return_val))
				{
					printf("pthread_join failed on index %ld\n", i);
				}
				
				sum_of_divisors += (size_t)join_return_val; 
			}
		}
		
		else
		{
			start_time = time(NULL);
			
			UpdateIndex(0); 
			
			end_time = time(NULL);
			
			printf("time of performance with 1 threads: %ld [sec]\n", 
								     				   (end_time - start_time));
		}	
		
		free(id); 
	}	
	
	return SUCCESS;
}	

static void *UpdateIndex(void *index)
{	
	size_t idx = (size_t)index; 
	size_t sum_of_divisors = 0;
	size_t i = 0; 
	
	int_arr[idx] = idx; 
	
	for(i = idx*(DIV_NUM / num_of_threads[j]) + 1 ; 
							   i < (idx +1)*(DIV_NUM / num_of_threads[j]) ; ++i)
	{
		if(0 == DIV_NUM % i)
		{
			sum_of_divisors += i;
		} 
	}
	
	return (void *)sum_of_divisors;
}
