#include <stdio.h>	/*	puts	*/

#include "heap.h"

enum {TEST_SUCCESS = 0 ,TEST_FAIL = 1}; 

int HeapTest(void);

static int CmpInt(void *heap_data, void *data_to_match);  

int main(void)
{
	if(TEST_FAIL == HeapTest())
	{
		puts("heap test filed!"); 
	}
	
	return TEST_SUCCESS; 
}

int HeapTest(void)
{
	heap_ty *heap = NULL; 
	
	heap = HeapCreate(CmpInt); 
	
	if(heap == NULL)
	{
		puts("create TEST_FAILed!"); 
		return TEST_FAIL; 
	}
	
	HeapDestroy(heap); 
	
	return TEST_SUCCESS; 
}

static int CmpInt(void *heap_data, void *data_to_match)
{
	int data1 = *((int *)heap_data); 
	int data2 = *((int *)data_to_match);
	
	return (data1-data2); 
}
