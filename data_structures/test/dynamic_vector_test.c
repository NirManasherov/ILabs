/**************************libraries***********************************/
#include <stdio.h> /*for puts, size_t*/
#include "dynamic_vector.h" 

/**********************************************************************/
/**************************functions***********************************/

int main(void) 
{
	size_t elem_size = 4; 
	size_t capacity = 6; 
	int x = 5; 
	int y = 7;
	size_t i = 0; 
	int *res = NULL; 
	d_vector_ty *dv_ptr = NULL; 
	
	dv_ptr = VectorCreate(capacity, elem_size); 
	VectorPushBack(dv_ptr, &x);/*1 mem*/
	res = VectorGetAccessToElement(dv_ptr, 0);
	
	if(x != *res)
	{
		puts("Error1!\n");
	}
	
	if(capacity != VectorGetCapacity(dv_ptr))
	{
		puts("Error2!\n");
	}
	
	for(i = 0 ; i < capacity +2 ; ++i) /*9 mem*/
	{
		VectorPushBack(dv_ptr, &(y));
	}
	
	res = VectorGetAccessToElement(dv_ptr, 7);
	if(y != *(int *)res)
	{
		puts("Error3!\n");
	}
	
	capacity *= 2; 
	
	if(capacity != VectorGetCapacity(dv_ptr))
	{
		puts("Error4!\n");
	}
	
	for(i = 0 ; i < 5 ; i++) 
	{
		VectorPopBack(dv_ptr);/*4 mem*/
	}
	
	if(4 != VectorGetSize(dv_ptr))
	{
		puts("Error5!\n");
	}
	VectorReserve(dv_ptr, 100);
	capacity = 100; 
	if(capacity != VectorGetCapacity(dv_ptr))
	{
		puts("Error6!\n");
	}
	VectorShrinkToFit(dv_ptr);
	capacity = 4; 
	if(capacity != VectorGetCapacity(dv_ptr))
	{
		puts("Error7!\n");
	}
	VectorDestroy(dv_ptr); 
	return 0; 
}

/**********************************************************************/
