#include <stddef.h>	/*	size_t		*/
#include <assert.h>	/*	assert		*/
#include <stdlib.h>	/*	free malloc	*/
#include <string.h>	/*	memcpy		*/

#include "comparisonlinearsort.h"

#define BASE 10
#define UNUSED(x) (void)(x)

enum {SUCCESS = 0, FAIL = 1}; 

typedef int (*read_func_ty)(int *num, void *param);

static int CmpMem(void *int1, void *int2);
static void SwapMembers(void *data1, void *data2); 
static void *MinMember(void *arr, size_t size);
static void SortLastMemberInArr(void *arr, size_t size);
static void FindMinMax(int *arr, size_t size, int *out_min, int *out_max, 
										  read_func_ty read_func ,void* param);
static void SetCountArr(int *arr, size_t size, int *count_arr, 	
			size_t count_size, int offset, read_func_ty read_func ,void* param);
static int FindNumOfDigits(int num); 
static int FindMaximumNumOfDigitsInArr(int *arr, size_t size);
static int CountingSortByAspect(int *arr, size_t size, read_func_ty read_func
																,void* param);
static int ReadInt(int *num_ptr, void *param); 
static int CountingSortByAspect(int *arr, size_t size, read_func_ty read_func
																,void* param); 
static int ReadIntDigit(int *num_ptr, void *param); 

void BubbleSort(int *arr, size_t size)
{
	size_t swaps_counter = 1; 
	size_t iter_count = 0; 
	
	assert(0 != size);
	assert(NULL != arr); 
	
	while(0 != swaps_counter)	/*wile there are swaps*/
	{
		swaps_counter = 0; /*for each iter reset swap_counter*/		
		
		/*scan the array and swap if members are not in order*/
		for(iter_count = 1 ; iter_count < size ; ++iter_count)
		{
			/*arr[iter_count] < arr[iter_count - 1]*/
			if(0 > CmpMem((void *)(arr + iter_count), 
										   (void *)(arr + iter_count - 1)))
			{
				/*set in order*/
				SwapMembers((void *)(arr + iter_count), 
										   (void *)(arr + iter_count - 1)); 
				++swaps_counter;
			}
		}
	}
}

void SelectionSort(int *arr, size_t size)
{
	size_t iter_counter = 0;
	
	assert(NULL != arr); 
	assert(0 != size);
	
	for(iter_counter = 0 ; iter_counter < size - 1 ; ++iter_counter)
	{
		SwapMembers((void *)(arr + iter_counter), 
				  MinMember((void *)(arr + iter_counter), size - iter_counter));
		
	}
}

void InsertionSort(int *arr, size_t size)
{
	size_t index = 1;
	
	assert(NULL != arr); 
	assert(0 != size); 
	
	while(index < size) 
	{
		SortLastMemberInArr((void *)arr, index + 1); 
		++index; 
	}
}

int CountingSort(int *arr, size_t size)
{
	assert(NULL != arr); 
	assert(0 != size);
	
	return CountingSortByAspect(arr, size, ReadInt, NULL);
}

int RadixSort(int *arr, size_t size)
{
	int max_num_digits = 0;
	int index = 0;
	int count_res = 0; 
	
	assert(NULL != arr); 
	assert(0 != size);
	
	max_num_digits = FindMaximumNumOfDigitsInArr(arr, size); 
	
	for(index = 1 ; index <= max_num_digits ; ++index)
	{
		
		count_res = CountingSortByAspect(arr, size, ReadIntDigit, 
														(void *)&(index));
	}
	
	return count_res;
}

/*
	"data1" and "data2" are the compared elements
    returns:    > 0 if data1 > data2
                < 0 if data1 < data2
                = 0 if data1 == data2 
    assuming data1, data2 are int pointers                                        
*/
static int CmpMem(void *data1, void *data2)
{
	return (*((int *)data1) - *((int *)data2));
}

/*
	swap two members that pointed to by data1 and data2 assuming data1 data2 
	these are int pointers
*/
static void SwapMembers(void *data1, void *data2)
{
	if(data1 != data2)
	{
		*((int *)data1) = (*((int *)data1)) ^ (*((int *)data2)); 
		*((int *)data2) = (*((int *)data1)) ^ (*((int *)data2));
		*((int *)data1) = (*((int *)data1)) ^ (*((int *)data2));  
	}
}

/*
	finds the minimum valued member in arr assuming it's int array using CmpMem
	function
*/
static void *MinMember(void *arr, size_t size)
{
	size_t iter_counter = 0;
	void *local_min = arr;  
	
	for(iter_counter = 0 ; iter_counter < size ; ++iter_counter)
	{
		if(*((int *)local_min) > *(((int *)arr) + iter_counter))
		{
			local_min = (void *)(((int *)arr) + iter_counter); 
		}
	}
	
	return local_min; 
}

/*
	arr is pointing to the last member of the array 
	sort this member in the rest of the sorted array
*/
static void SortLastMemberInArr(void *arr, size_t size)
{
	size_t index = size - 1;
	
							/*arr[index - 1] > arr[index]*/
	while((0 < index) && (0 < CmpMem((void *)(((int *)arr) + index - 1), 
											(void *)(((int *)arr) + index))))
	{
		SwapMembers((void *)(((int *)arr) + index - 1), 
												(void *)(((int *)arr) + index));
		--index;  
	}
	
}

/*
	return pointers min and max elements in arr in the appropriate arguments 
*/
static void FindMinMax(int *arr, size_t size, int *out_min, int *out_max, 
										  read_func_ty read_func ,void* param)
{
	size_t index = 0; 
	
	*out_min = read_func(arr, param); 
	*out_max = read_func(arr, param);
	
	for(index = 0 ; size > index ; ++index)
	{
		if(*out_min > read_func((arr + index),param))
		{
			*out_min = read_func((arr + index),param);
		}
		
		/*out_max < arr[index]*/
		else if(*out_max < read_func((arr + index),param))
		{
			*out_max = read_func((arr + index),param);
		}
	}	
}

/*
	set a count array in count_arr (size of count_size) based on arr elements
	(size of size)
	the offset is the offset of counting_arr index
	the user should provide an count arr
*/
static void SetCountArr(int *arr, size_t size, int *count_arr, 	
			size_t count_size, int offset, read_func_ty read_func ,void* param)
{
	size_t index = 0; 
	
	/*initializing count_arr*/
	for(index = 0 ; count_size > index ; ++index)
	{
		count_arr[index] = 0; 
	}
	
	for(index = 0 ; size > index ; ++index)
	{
		++count_arr[read_func(arr + index, param) - offset];
	}
	
	/*accumiativly*/
	for(index = 1 ; count_size > index ; ++index)
	{
		count_arr[index] += count_arr[index - 1];
	}
}

/*
	return the number of digits in num by base "BASE"
*/
static int FindNumOfDigits(int num)
{
	int digits_counter = 0; 
	
	if(0 == num)
	{
		return 1; 
	}
	
	while(0 != num)
	{
		num = num/BASE; 
		++digits_counter; 
	}
	
	return digits_counter;
}

/*
	find the maximum number of digits of array of ints
*/
static int FindMaximumNumOfDigitsInArr(int *arr, size_t size)
{
	size_t index = 0;
	int max_num_digits = 0;
	int curr_num_digits = 0;  
	
	for(index = 0 ; size > index ; ++index)
	{
		curr_num_digits = FindNumOfDigits(arr[index]);
		
		if(max_num_digits < curr_num_digits)
		{
			max_num_digits = curr_num_digits; 
		} 
	}
	
	return max_num_digits; 
}

/*
	sort an array of ints by read_func using selection sort method
*/
static int CountingSortByAspect(int *arr, size_t size, read_func_ty read_func
																,void* param)
{
	int min = 0; 
	int max = 0; 
	int min_max_range = 0; 
	int *count_arr; 
	int index = size;
	int count_arr_index = 0; 
	int correct_place = 0; 
	int *cpy_arr; 
	
	FindMinMax(arr, size, &min, &max, read_func, param);
	min_max_range = max - min + 1; 
	count_arr = (int *)malloc(min_max_range*sizeof(int)); 
	
	if(NULL == count_arr)
	{
		return FAIL; 
	}
	
	cpy_arr = (int *)malloc(size*sizeof(int)); 
	
	if(NULL == cpy_arr)
	{
		free(count_arr); 
		count_arr = NULL; 
		return FAIL; 
	}
	
	/*make an counting array*/
	SetCountArr(arr, size, count_arr, min_max_range, min, read_func, param); 
	
	/*duplicate array*/
	memcpy(cpy_arr, arr, (size * sizeof(int)));
	
	for(index = size - 1 ; 0 <= index ; --index)
	{
		/*find cpy_arr index by using its offset*/
		count_arr_index = read_func(cpy_arr + index, param) - min; 
		/*find sorted place of member by using counting array*/
		correct_place = count_arr[count_arr_index] - 1;
		/*put the last member of that type in correct place*/
		arr[correct_place] = cpy_arr[index];
		/*update the count array*/
		--count_arr[count_arr_index]; 
	}
	
	free(count_arr); 
	count_arr = NULL; 
	free(cpy_arr); 
	cpy_arr = NULL; 
	return SUCCESS; 
}

/*
	read a number by it's arithmetical value
*/
static int ReadInt(int *num_ptr, void *param)
{
	UNUSED(param); 
	
	return *num_ptr; 
}

/*
	param is a pointer to the number of digit starting from the least 
	significant one (starting to count from 1); 
	return the digit in param place
*/
static int ReadIntDigit(int *num_ptr, void *param)
{
	int digit_place = *(int *)param; 
	int index = 0; 
	int represent_digit = 1;
	int num = *num_ptr;
	
	for(index = 0 ; digit_place - 1 > index ; ++index)
	{
		represent_digit *=BASE;
	}
	
	return ((num%(represent_digit*BASE))/represent_digit);
}









