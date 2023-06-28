#include <stdio.h>	/*	puts				*/
#include <stdlib.h>	/*	rand	qsort		*/
#include <time.h>	/*	clock	clock_ty	*/


#include "comparisonlinearsort.h"

#define TEST_SIZE 5000

enum {FALSE = 0, TRUE = 1}; 
enum {SUCCESS = 0, FAIL = 1}; 

static int TestSort(void); 
static int IsSorted(int *arr, size_t size);
static int CmpMem(const void *data1, const void *data2);

void printMe(int *arr, size_t size);

int main(void)
{
	if(FAIL == TestSort())
	{
		puts("some tests did not passed!");
		return FAIL;  
	}
	
	puts("all tests passed!");
	return SUCCESS; 
}

static int TestSort(void)
{
	int test_arr[TEST_SIZE] = {0}; 
	int test_arr_qsort[TEST_SIZE] = {0}; 
	size_t index = 0; 
	time_t before_call; 
	time_t after_call; 
	time_t before_qsort_call; 
	time_t after_qsort_call;
	int res = 0; 
	
	/*bubble sort*/
	index = 0; 
	while(index < TEST_SIZE) 
	{
		test_arr[index] = rand(); 
		test_arr_qsort[index] = test_arr[index];
		++index; 
	}
	
	before_qsort_call = clock(); 
	qsort(test_arr_qsort, TEST_SIZE, sizeof(int), CmpMem);
	after_qsort_call = clock();
	
	before_call = clock();
	BubbleSort(test_arr ,TEST_SIZE); 
	after_call = clock();
	
	printf("bubble_time/qsort_time = %f\n", 
											((double)after_call - before_call)/
								((double)after_qsort_call - before_qsort_call));
	
	if(FALSE == IsSorted(test_arr, TEST_SIZE))
	{
		puts("bubble sort wasn't successfull");
		return FAIL;
	}
	
	/*insertion*/
	index = 0; 
	while(index < TEST_SIZE) 
	{
		test_arr[index] = rand(); 
		test_arr_qsort[index] = test_arr[index];
		++index; 
	}
	
	before_qsort_call = clock(); 
	qsort(test_arr_qsort, TEST_SIZE, sizeof(int), CmpMem);
	after_qsort_call = clock();
	
	before_call = clock();
	InsertionSort(test_arr ,TEST_SIZE); 
	after_call = clock();
	
	printf("insertion_time/qsort_time = %f\n", 
											((double)after_call - before_call)/
								((double)after_qsort_call - before_qsort_call));
	
	if(FALSE == IsSorted(test_arr, TEST_SIZE))
	{
		puts("insertion sort wasn't successfull");
		return FAIL;
	}
	
	/*selection*/
	index = 0; 
	while(index < TEST_SIZE) 
	{
		test_arr[index] = rand(); 
		test_arr_qsort[index] = test_arr[index];
		++index; 
	}
	
	before_qsort_call = clock(); 
	qsort(test_arr_qsort, TEST_SIZE, sizeof(int), CmpMem);
	after_qsort_call = clock();
	
	before_call = clock();
	SelectionSort(test_arr ,TEST_SIZE); 
	after_call = clock();
	
	printf("selection_time/qsort_time = %f\n", 
											((double)after_call - before_call)/
								((double)after_qsort_call - before_qsort_call));
	
	if(FALSE == IsSorted(test_arr, TEST_SIZE))
	{
		puts("selection sort wasn't successfull");
		return FAIL;
	}
	
	/*counting*/
	index = 0; 
	while(index < TEST_SIZE) 
	{
		test_arr[index] = (rand() % 100) + 1; 
		test_arr_qsort[index] = test_arr[index];
		++index; 
	}
	
	before_qsort_call = clock(); 
	qsort(test_arr_qsort, TEST_SIZE, sizeof(int), CmpMem);
	after_qsort_call = clock();
	
	before_call = clock();
	res = CountingSort(test_arr ,TEST_SIZE); 
	after_call = clock();

	
	if (FAIL == res)
	{
		puts("counting sort failiure");	
		return FAIL; 
	} 
	
	
	printf("counting_time/qsort_time = %f\n", 
											((double)after_call - before_call)/
								((double)after_qsort_call - before_qsort_call));
	
	if(FALSE == IsSorted(test_arr, TEST_SIZE))
	{
		puts("counting sort wasn't successfull");
		return FAIL;
	}
	
	/*radix*/
	index = 0; 
	while(index < TEST_SIZE) 
	{
		test_arr[index] = rand()%100000000; 
		test_arr_qsort[index] = test_arr[index];
		++index; 
	}
	
	before_qsort_call = clock(); 
	qsort(test_arr_qsort, TEST_SIZE, sizeof(int), CmpMem);
	after_qsort_call = clock();
	
	before_call = clock();
	RadixSort(test_arr ,TEST_SIZE); 
	after_call = clock();
	
	printf("radix_time/qsort_time = %f\n", 
											((double)after_call - before_call)/
								((double)after_qsort_call - before_qsort_call));
	if(FALSE == IsSorted(test_arr, TEST_SIZE))
	{
		puts("radix sort wasn't successfull");
		return FAIL;
	}
	
	return SUCCESS; 
}

static int IsSorted(int *arr, size_t size)
{
	size_t index = 1; 
	
	while(index < size)
	{
		if((arr[index] < arr[index - 1]))
		{
			printf("arr : %d. arr-1: %d. index: %lu\n", arr[index], 
			arr[index - 1], index);
			return FALSE; 
		}
		
		++index; 
	}
	
	return TRUE; 
}	

void printMe(int *arr, size_t size)
{
	size_t index = 0; 
	
	while(index < size)
	{
		printf("%d\n", arr[index]); 
		
		++index; 
	}
	
}	


/*
	"data1" and "data2" are the compared elements
    returns:    > 0 if data1 > data2
                < 0 if data1 < data2
                = 0 if data1 == data2 
    assuming data1, data2 are int pointers                                        
*/
static int CmpMem(const void *data1, const void *data2)
{
	return (*((int *)data1) - *((int *)data2));
}
