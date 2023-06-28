/*********************************************************************
made by: Nir Manasherov
Date: 12.12.2022
Reviewer: Almog Ifargan 
Version: 3
API version: 1.1
***********************************************************************/

/**************************libraries***********************************/

#include <stdlib.h> /*for malloc*/
#include <assert.h> /*for assert*/
#include <string.h> /*for memmove*/

#include "stack.h" 

/**********************************************************************/
/************************manager struct********************************/

struct stack
{
	size_t capacity; 
	size_t elem_num; 
	size_t elem_size;
	void *data_array; 
}; 

/**********************************************************************/
/**************************functions***********************************/

stack_ty *StackCreate(size_t capacity, size_t size_of_element)
{
	void *data_arr = NULL;
	stack_ty *stack_ptr = NULL; 
	
	stack_ptr =(stack_ty *)malloc(sizeof(stack_ty)); 
		
	if(NULL == stack_ptr)
	{
		return NULL; 
	}
	
	data_arr = (void *)malloc(capacity * size_of_element);
	
	if(NULL == data_arr)
	{
		free(stack_ptr); 
		return NULL; 
	}
		
	stack_ptr -> capacity = capacity; 
	stack_ptr -> elem_num = 0; 
	stack_ptr -> elem_size = size_of_element; 
	stack_ptr -> data_array = data_arr; 
	
	return stack_ptr; 	
}

/**********************************************************************/

void StackDestroy(stack_ty *stack_ptr)
{
	assert(NULL != stack_ptr); 
	free(stack_ptr -> data_array); 
	stack_ptr -> data_array = NULL; 
	free(stack_ptr); 
	stack_ptr = NULL; 
}

/**********************************************************************/

void StackPush(stack_ty *stack_ptr, const void *element)
{
	void *array = NULL;
	
	assert(NULL != stack_ptr); 
	array = (void *)((char *)(stack_ptr ->  data_array) + 
	((stack_ptr -> elem_num)*(stack_ptr -> elem_size))); 
	memmove(array, element, stack_ptr -> elem_size);
	++(stack_ptr -> elem_num); 
}

/**********************************************************************/

void  StackPop(stack_ty *stack_ptr)
{
	assert(NULL != stack_ptr);
	--(stack_ptr -> elem_num); 
}

/**********************************************************************/

void *StackPeek(const stack_ty *stack_ptr)
{
	void *res = NULL; 
	assert(NULL != stack_ptr);
	res = (char *)(stack_ptr -> data_array) + 
	((stack_ptr -> elem_size) * (stack_ptr -> elem_num - 1)); 
	return (void *)res; 
}

/**********************************************************************/

size_t StackGetSize(const stack_ty *stack_ptr)
{
	size_t res = 0; 
	assert(NULL != stack_ptr);
	res = stack_ptr -> elem_num; 
	return res; 
}

/**********************************************************************/

int StackIsEmpty(const stack_ty *stack_ptr)
{
	size_t res = 0; 
	assert(NULL != stack_ptr);
	res = stack_ptr -> elem_num; 
	if(res == 0) 
	{
		return 1; /*=true*/
	}
	else 
	{
		return 0; /*false*/
	} 
}

/**********************************************************************/

size_t StackGetCapacity(const stack_ty *stack_ptr)
{
	size_t res = 0; 
	assert(NULL != stack_ptr);
	res = stack_ptr -> capacity; 
	return res;
}

/**********************************************************************/
