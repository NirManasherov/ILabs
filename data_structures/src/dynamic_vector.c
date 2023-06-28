/*********************************************************************
made by: Nir Manasherov
Date: 12.12.2022
Reviewer: Almog Ifargan 
Version: 3
API version: 1.1
***********************************************************************/

/**************************libraries***********************************/

#include <stdio.h> /*for puts*/
#include <stdlib.h> /*for malloc*/
#include <assert.h> /*for assert*/
#include <string.h> /*for memmove*/

#include "dynamic_vector.h" 

/****************************macros************************************/

#define MULT_FACTOR 2

/**********************************************************************/
/************************manager struct********************************/
struct d_vector 
{
	size_t capacity; 
	size_t elem_num; 
	size_t elem_size; 
	void *data_array; 
}; 

/**********************************************************************/
/**************************functions***********************************/

d_vector_ty *VectorCreate(size_t capacity, size_t element_size)
{
	void *data_arr = NULL;
	d_vector_ty *vector_ptr = NULL; 
	
	vector_ptr =(d_vector_ty *)malloc(sizeof(d_vector_ty)); 
		
	if(NULL == vector_ptr)
	{
		puts("allocation of memory fail\n"); 
		return NULL; 
	}
	
	data_arr = (void *)malloc(capacity * element_size);
	
	if(NULL == data_arr)
	{
		puts("allocation of memory fail\n"); 
		free(vector_ptr); 
		return NULL; 
	}
		
	vector_ptr -> capacity = capacity; 
	vector_ptr -> elem_num = 0; 
	vector_ptr -> elem_size = element_size; 
	vector_ptr -> data_array = data_arr; 
	
	return vector_ptr; 	
}

/**********************************************************************/

void VectorDestroy(d_vector_ty *vector_ptr)
{
	assert(NULL != vector_ptr); 
	free(vector_ptr -> data_array); 
	vector_ptr -> data_array = NULL; 
	free(vector_ptr); 
	vector_ptr = NULL; 
}

int VectorPushBack(d_vector_ty *vector_ptr, const void *element)
{
	void *array = NULL;
	assert(NULL != vector_ptr); 
	array = (vector_ptr -> data_array); 
	
	if((vector_ptr -> elem_num) == (vector_ptr -> capacity)) /*arr is full*/
	{
		if(NULL == VectorReserve(vector_ptr, MULT_FACTOR*
		(vector_ptr -> capacity)))
		{
			puts("allocation of memeory fail\n");
			return 1;
		} 
	}
	
	array = (void *)((char *)(vector_ptr ->  data_array) + 
	((vector_ptr -> elem_num)*(vector_ptr -> elem_size))); 
	memmove(array, element, vector_ptr -> elem_size);
	++(vector_ptr -> elem_num); 
	
	return 0; 
}

/**********************************************************************/

void VectorPopBack(d_vector_ty *vector_ptr) 
{
	assert(NULL != vector_ptr);
	--(vector_ptr -> elem_num); 
}

/**********************************************************************/

void *VectorGetAccessToElement(const d_vector_ty *vector_ptr, size_t index) 
{
	void *res = NULL; 
	assert(NULL != vector_ptr);
	res = (char *)(vector_ptr -> data_array) + 
	((vector_ptr -> elem_size) * index); 
	return (void *)res; 
}

/**********************************************************************/

size_t VectorGetSize(const d_vector_ty *vector_ptr)
{
	size_t res = 0; 
	assert(NULL != vector_ptr);
	res = vector_ptr -> elem_num; 
	return res; 
}

/**********************************************************************/

size_t VectorGetCapacity(const d_vector_ty *vector_ptr) 
{
	size_t res = 0; 
	assert(NULL != vector_ptr);
	res = vector_ptr -> capacity; 
	return res; 
}

/**********************************************************************/

d_vector_ty *VectorReserve(d_vector_ty *vector_ptr, size_t new_capacity) 
{
	void *res = NULL; 
	assert(NULL != vector_ptr);
	res = realloc(vector_ptr -> data_array, 
	(vector_ptr -> elem_size)*new_capacity);
	if(res == NULL) 
	{
		puts("allocation of memory fail\n"); 
		return NULL; 
	}
	
	vector_ptr -> data_array = res; 
	vector_ptr -> capacity = new_capacity; 
	return vector_ptr; 
}

/**********************************************************************/

d_vector_ty *VectorShrinkToFit(d_vector_ty *vector_ptr) 
{
	return VectorReserve(vector_ptr, vector_ptr -> elem_num); 
}

/**********************************************************************/
