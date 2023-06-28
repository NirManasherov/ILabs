#include <sys/types.h>	/*	ssize_t	*/
#include <assert.h> 	/*	assert	*/
#include <stdlib.h>		/*	malloc	*/
#include <string.h>		/*	memmove	*/
#include <stdio.h>

#include "search_and_sorts.h"

#define NUM_NOT_FOUND -1
#define SEARCH_FAIL -1
#define UNUSED(x) (void)(x)

enum {SUCCESS = 0 ,FAIL =1}; 

/*finds num using binary search recursivly*/
static ssize_t BinarySearchRecursiveImp(int num, int *arr, size_t size, 
																int *arr_start); 

/*preform Merge in order between left arr and right array seperated by 
seperator*/
static int Merge(int *num_arr, int *start, int *end, int *seperator);
																
/*swap the data od pointers data1 and data2*/
static void SwapData(int *data1, int *data2); 

ssize_t BinarySearchIterative(int num, int *arr, size_t size)
{
	/*make indexes for the middle, start and end of array*/
	ssize_t arr_idx = 0;
	ssize_t middle_idx = 0; 
	ssize_t start_idx = 0; 
	ssize_t end_idx = size; 
	
	/*asseert relevant arguments*/
	assert(NULL != arr); 
	assert(0 < size); 
	
	/*itirate for the entire array*/
	for(arr_idx = 0 ; (size_t)arr_idx < size ; ++arr_idx)
	{
		/*find the middle of the array by using the array's edges indexes*/
		middle_idx = (start_idx + end_idx)/2; 
		
		/*if the wanted num is equal to the middle return found idx*/
		if(num == arr[middle_idx])
		{
			return middle_idx; 
		}
		
		/*if the number is not found return -1*/
		if(start_idx == end_idx) 
		{
			return NUM_NOT_FOUND; 
		}
		
		/*if the wanted num is greater than the middle go to the upper side*/
		if(num > arr[middle_idx])
		{
			start_idx =  middle_idx;
		}
		
		/*if the wanted num is smaller than the middle go to the lower side*/
		if(num < arr[middle_idx])
		{
			 end_idx = middle_idx; 
		}
	}
	
	return SEARCH_FAIL; 
}

ssize_t BinarySearchRecursive(int num, int *arr, size_t size)
{
	return BinarySearchRecursiveImp(num, arr, size, arr);
}

/*finds num using binary search recursivly*/
static ssize_t BinarySearchRecursiveImp(int num, int *arr, size_t size, 
																int *arr_start)
{
	/*find the middle of the array to size/2*/
	ssize_t middle_idx = 0; 
	int *middle = NULL; 
	
	middle_idx = size/2;
	middle = arr + middle_idx;
	
	/*if the num is found in the middle return it's distance (int-wise) from 
	the start of the array*/
	if(num == *middle)
	{
		return (middle - arr_start);  
	}
	
	/*if the number is not found (size = 1 and the only num is not a match 
	return -1*/
	if(1 == size)
	{
		return NUM_NOT_FOUND;		
	}
	
	/*if the wanted num is greater than the middle go to the upper side by 
	calling the function onto the upper sub-array*/
	if((num > *middle) && (middle != (arr + size - 1)))
	{
		return BinarySearchRecursiveImp(num, middle + 1, 
									  ((arr + size) - (middle + 1)), arr_start); 
	}
	
	/*if the wanted num is lower than the middle go to the lower side by 
	calling the function onto the lower sub-array*/
	if(num < *middle)
	{
		return BinarySearchRecursiveImp(num, arr , middle - arr, arr_start); 
	}
	
	return SEARCH_FAIL;
	
}

int MergeSort(int *arr_to_sort, size_t num_elements)
{
	int *right_sub_arr = NULL; 
	int *left_sub_arr = NULL; 
	
	left_sub_arr = arr_to_sort; 
	right_sub_arr = left_sub_arr + num_elements/2; 
	
	/*if there is no lonely member*/
	if(1 < num_elements)
	{
		/*sort left sub array*/
		if(FAIL == MergeSort(left_sub_arr, num_elements/2)) /*sending with middle*/
		{
			return FAIL; 
		}
				
		/*sort right array*/
		if(FAIL == MergeSort(right_sub_arr, num_elements - num_elements/2)) /*sending without middle*/
		{
			return FAIL; 
		}
		
		return Merge(arr_to_sort , arr_to_sort, arr_to_sort + num_elements - 1, 
																 right_sub_arr); 
	}
	
	return SUCCESS; 
}

/*preform Merge in order between left arr and right array seperated by 
seperator*/
static int Merge(int *num_arr, int *start, int *end, int *seperator)
{
	int *sorted_num_arr = NULL;
	int *sorted_num_arr_og = NULL; 
	int *right_side_og = NULL; 
	int *right_arr = NULL; 
	int *left_arr = NULL; 
	size_t arr_size = 0; 
	
	arr_size = end - start + 1; 
	
	sorted_num_arr = (int *)malloc(arr_size*sizeof(int));
	sorted_num_arr_og = sorted_num_arr; 
		
	if(NULL == sorted_num_arr)
	{
		return FAIL; 
	}
	
	/*distinguish between right and left sub-array*/
	right_side_og = seperator;  
	right_arr = seperator; 
	left_arr = num_arr; 
	
	if (0 == arr_size)
	{
		return SUCCESS;
	}
	
	if(1 == arr_size)
	{
		*sorted_num_arr = *num_arr; 
	}
	
	else
	{
		/*while left and right do not exeed their sub-array*/
		while((right_arr <= end) && (left_arr != right_side_og))
		{
			if(*right_arr > *left_arr)	/*equal sign to maintain stability*/
			{
				/*copy it to the res array*/
				*sorted_num_arr = *left_arr; 
				++sorted_num_arr; 
				++left_arr; 
			}
			
			/*if right member is smaller than left member*/
			if(*right_arr <= *left_arr)
			{
				/*copy it to the res array*/
				*sorted_num_arr = *right_arr; 
				++sorted_num_arr; 
				++right_arr; 	
			}
			
		}
	
	
		while(sorted_num_arr < (sorted_num_arr_og + arr_size))
		{
			/*left was added fully => right hasn't been added fully*/
			if((left_arr == right_side_og) && (right_arr <= end))
			{
				*sorted_num_arr = *right_arr;
				++right_arr;		
			}	
			
			/*right was added fully => left hasn't been added fully*/
			if((right_arr > end) && (left_arr != right_side_og))
			{
				*sorted_num_arr = *left_arr;
				++left_arr; 		
			}	
			
			++sorted_num_arr; 
		}
	}
	
	memmove(num_arr, sorted_num_arr_og, arr_size*sizeof(int));
	
	free(sorted_num_arr_og);
	sorted_num_arr = NULL;  
	
	return SUCCESS; 
	
}

void QuickSort(void *base, size_t nmemb, size_t size, sort_cmp_func_ty cmp_func)
{
	int *pivot = NULL; 
	int *num_arr = NULL; 
	int *begin = NULL;
	int *begin_og = NULL;
	int *end = NULL; 
	size_t smaller_counter = 0;
	size_t i = 0; 
	
	/*if there is a lonely member it is sorted*/
	if(1 >= nmemb)
	{
		return; 
	}
	
	num_arr = (int *)base; 
	pivot = num_arr + nmemb - 1;
	
	/*set pointers for the begin, middle, and end of the array*/
	begin = num_arr; 
	begin_og = begin;
	end = pivot - 1; /*last member is pivot*/
	
	 
	while(begin < end)
	{
		/*begin > pivot and end < pivot*/
		if((0 < cmp_func(begin, pivot)) && (0 > cmp_func(end, pivot)))
		{
			SwapData(begin, end);
			++smaller_counter; 
			++begin; 
			--end; 
		}
		
		else
		{
			/*begin  < pivot*/
			if(0 > cmp_func(begin, pivot))
			{
				++smaller_counter;
				++begin; 
			}
			
			/*end > pivot*/
			if(0 < cmp_func(end, pivot))
			{
				--end; 
			}
		}
	}
	

	if(*end < *pivot)
	{
		++end;
	}	
	
	/*swap pivot to the correct place*/
	SwapData(pivot, end); 
	
	/*call recursively the right anf left sub-arrays*/
	/*right sub-array*/
	QuickSort(begin_og, end - begin_og , size, cmp_func); 
	
	if(end != begin_og + nmemb -1)
	{
		/*left sub-array*/
		QuickSort(end + 1, nmemb - (end - begin_og) - 1  , size, cmp_func);
	} 
}   

/*swap the data od pointers data1 and data2*/
static void SwapData(int *data1, int *data2)
{
	/*XOR swap can't swap the same data*/
	if(data1 != data2)
	{
		*data1 = (*data1)^(*data2); 
		*data2 = (*data1)^(*data2);
		*data1 = (*data1)^(*data2);
	}
}
                                 
