#include <stdio.h>	/*	malloc				*/
#include <time.h>	/*	clock	clock_ty	*/
#include <stdlib.h>	/*	qsort				*/

#include "search_and_sorts.h"

#define ARR_SIZE 5

enum {NOT_FOUND = -1, SUCCESS = 0 ,FAIL =1}; 

static int BinarySearchTest(void); 

static int MergeSortTest(void); 

static int QuickSortTest(void); 

static int IntCmp(const void *data1, const void *data2); 

int main(void)
{
	int fail_count = 0; 
	
	fail_count += BinarySearchTest();
	
	if(SUCCESS != fail_count)
	{
		puts("binary test failed!\n"); 
		return FAIL; 
	}
	
	fail_count += MergeSortTest();
	
	if(SUCCESS != fail_count)
	{
		puts("merge sort test failed!\n"); 
		return FAIL; 
	}
	
	fail_count += QuickSortTest();
	
	if(SUCCESS != fail_count)
	{
		puts("quick sort test failed!\n"); 
		return FAIL; 
	}
	
	if(SUCCESS == fail_count)
	{
		puts("all tests passed!\n"); 
	}
	
	return SUCCESS;  	
}

static int BinarySearchTest(void)
{
	int arr[ARR_SIZE] = {0}; 
	ssize_t idx = 0;  
	
	/*create sorted array*/
	for(idx = 0 ; idx < ARR_SIZE ; ++idx)
	{
		arr[idx] = idx; 
	}
	
	for(idx = 0 ; idx < ARR_SIZE ; ++idx)
	{
		if(idx != BinarySearchIterative(idx, arr, ARR_SIZE))
		{
			printf("iterative binary search failed! on iteration %lu\n", idx); 
			return FAIL; 
		} 	
	}
	
	if(NOT_FOUND != BinarySearchIterative(-10, arr, ARR_SIZE))
		{
			printf("iterative binary search failed! left exceeding\n"); 
			return FAIL; 
		} 	
		
	if(NOT_FOUND != BinarySearchIterative(ARR_SIZE + 100, arr, ARR_SIZE))
		{
			printf("iterative binary search failed! left exceeding\n"); 
			return FAIL; 
		} 	
	
		
	for(idx = 0 ; idx < ARR_SIZE ; ++idx)
	{
		if(idx != BinarySearchRecursive(idx, arr, ARR_SIZE))
		{
			printf("recursive binary search failed! on iteration %lu\n", idx); 
			return FAIL; 
		} 	
	}
	
	if(NOT_FOUND != BinarySearchRecursive(-10, arr, ARR_SIZE))
		{
			printf("recursive binary search failed! left exceeding\n"); 
			return FAIL; 
		} 	
		
	if(NOT_FOUND != BinarySearchRecursive(ARR_SIZE + 100, arr, ARR_SIZE))
		{
			printf("recursive binary search failed! left exceeding\n"); 
			return FAIL;
		} 	
	
	return SUCCESS;
}


static int MergeSortTest(void)
{
	int arr[10] = {1, -2, 4, -8, 16, -32, 64, -128, 256, -512}; 
	ssize_t idx = 0; 
		
	MergeSort(arr, 10); 
	
	
	for(idx = 1 ; idx < 10 ; ++idx)
	{
		if(arr[idx] < arr[idx-1])
		{
			printf("merge sort failed! on arr index %lu\n", idx); 
			return FAIL; 
		} 	
	}
	
	return SUCCESS; 
}

static int QuickSortTest(void)
{
	int arr[ARR_SIZE] = {0}; 
	ssize_t idx = 0; 
	time_t before_call; 
	time_t after_call; 
	time_t before_qsort_call; 
	time_t after_qsort_call; 
	
	arr[0] = 1; 
	
	/*create sorted array*/
	for(idx = 1 ; idx < ARR_SIZE ; ++idx)
	{
		arr[idx] = (-2) * arr[idx-1]; 
	}
	
	
	before_call = clock(); 
	QuickSort(arr, ARR_SIZE, sizeof(int), IntCmp); 
	after_call = clock(); 
	
	for(idx = 1 ; idx < ARR_SIZE ; ++idx)
	{
		if(arr[idx] < arr[idx-1])
		{ 
			return FAIL; 
		} 	
	}
	
	/*library q-sort*/
	/*create sorted array*/
	arr[0] = 1;
	
	for(idx = 0 ; idx < ARR_SIZE ; ++idx)
	{
		arr[idx] = (-2) * arr[idx-1]; 
	}
	
	before_qsort_call = clock(); 
	qsort(arr, ARR_SIZE, sizeof(int), IntCmp);
	after_qsort_call = clock(); 
	
	printf("our qsort_time/lib qsort_time = %f\n", 
											((double)after_call - before_call)/
								((double)after_qsort_call - before_qsort_call));
	
	return SUCCESS; 
}

static int IntCmp(const void *data1, const void *data2)
{
	return (*((int *)data1) - *((int *)data2)); 
}

