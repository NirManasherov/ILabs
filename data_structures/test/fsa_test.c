#include <stddef.h>	/*	size_t	*/
#include <stdio.h> 	/*	puts	*/
#include <stdlib.h>	/*	malloc	*/

#include "fsa.h"

#define UNUSED(x) (void)(x)

enum {SUCCESS = 0 , FAIL = 1}; 

static int FSATest(void);

int main(void)
{
	if (1 == FSATest())
	{
		puts("some errors occured!"); 
		return FAIL; 
	}	 
	
	puts("all tests passed!"); 
	return SUCCESS; 
}

static int FSATest(void) 
{
	size_t sugest_size = 0; 
	FSA_pool_ty *pool = NULL; 
	size_t block_size = 16; 
	size_t num_of_blocks = 10; 
	FSA_pool_ty *fsa = NULL; 
	void *alloc_1 = NULL; 
	void *alloc_2 = NULL; 
	void *save_alloc = NULL; 
	
	sugest_size = FSASuggestSize(num_of_blocks, block_size);
	pool = (void *)malloc(sugest_size); 
	
	if(NULL == pool)
	{
		puts("couldn't allocate pool!"); 
		return FAIL; 
	}
	
	fsa = FSAInit(pool, sugest_size, block_size);
	
	if(10 != FSACountFree(fsa))
	{
		puts("count free failed 10!"); 
		return FAIL; 
	}
	
	alloc_1 = FSAAlloc(fsa);
	save_alloc = alloc_1; 
	
	if(9 != FSACountFree(fsa))
	{
		puts("count free failed 9!"); 
		return FAIL; 
	}
	
	alloc_2 = FSAAlloc(fsa); 
	
	if(8 != FSACountFree(fsa))
	{
		puts("count free failed 8!"); 
		return FAIL; 
	}
	
	FSAFree(fsa, alloc_1);
	
	if(9 != FSACountFree(fsa))
	{
		puts("count free failed 9 after free!"); 
		return FAIL; 
	}
	
	alloc_1 = FSAAlloc(fsa);
	
	if(alloc_1 != save_alloc)
	{
		puts("free failed by switching free vacancy to next free!");
		return FAIL; 
	}

	UNUSED(alloc_2); 
	
	free(pool); 
		
	return SUCCESS; 
}
