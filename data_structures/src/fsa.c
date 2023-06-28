#include <stddef.h>	/*	size_t	*/
#include <assert.h>	/*	assert 	*/

#include "fsa.h"

#define WORD_SIZE (sizeof(size_t)) 

struct fsa
{
	void *next_free; 
}; 	

size_t FSASuggestSize(size_t num_of_blocks, size_t block_size)
{
	assert(0 < num_of_blocks);	/*irrelevent value*/
	assert(0 < block_size);		/*irrelevent value*/
	assert(0 == block_size%WORD_SIZE); 	/*alignment*/
	
	if(0 != block_size%WORD_SIZE)
	{
		block_size += (WORD_SIZE - block_size%WORD_SIZE); 
	} 
	
	return sizeof(FSA_pool_ty) + num_of_blocks*block_size; 
}

FSA_pool_ty *FSAInit(void *mem_pool, size_t FSA_pool_size, size_t block_size)
{
	size_t num_of_blocks = 0; 
	size_t suggest_size = 0; 
	char *curr_runner_ptr = mem_pool; 
	char *next_runner_ptr = (char *)mem_pool + sizeof(FSA_pool_ty); 
	char *end_ptr = (char *)mem_pool + FSA_pool_size; 
	
	assert(0 == (size_t)mem_pool%WORD_SIZE); /*alignment*/
	assert(NULL != mem_pool); 
	assert(0 != FSA_pool_size); 
	assert(0 != block_size); 
	num_of_blocks = (FSA_pool_size - sizeof(FSA_pool_ty))/block_size; 
	suggest_size = FSASuggestSize(num_of_blocks, block_size); 
	assert(suggest_size == FSA_pool_size); 
	
	/*initializing handler*/
	((FSA_pool_ty *)curr_runner_ptr)->next_free = (void *)next_runner_ptr; 
	
	/*moving pointers*/
	curr_runner_ptr = next_runner_ptr; 
	next_runner_ptr = next_runner_ptr + block_size;	/*next block*/
	
	/*while the adress of next_runner_ptr is lower than the adress of
	end ptr*/
	while((size_t)next_runner_ptr < (size_t)end_ptr)
	{
		/*putting the adress of the next block*/
		*((size_t *)curr_runner_ptr) = (size_t)next_runner_ptr; 
		/*moving pointers*/
		curr_runner_ptr = next_runner_ptr;
		next_runner_ptr = next_runner_ptr + block_size; 
	}
	
	/*initializing last block to point to NULL */
	*((size_t **)curr_runner_ptr) = NULL;	/*end block*/
	
	return (FSA_pool_ty *)((FSA_pool_ty *)mem_pool)->next_free; 
}

void *FSAAlloc(FSA_pool_ty *FSA_pool) 
{
	char **curr_runner_ptr = NULL; 
	FSA_pool_ty *handler_ptr = NULL;
	
	assert(NULL != FSA_pool);
	
	handler_ptr = (FSA_pool_ty *)FSA_pool - 1; /*moving one hadler size back*/
	
	curr_runner_ptr = handler_ptr->next_free; /*moving to the free space*/
	
	if(NULL != curr_runner_ptr)
	{
		handler_ptr->next_free = *curr_runner_ptr;
		*curr_runner_ptr = NULL; 
	}
	
	return (void *)curr_runner_ptr;
}

void FSAFree(FSA_pool_ty *FSA_pool, void *block_to_free)
{
	void **curr_runner_ptr = NULL; 
	FSA_pool_ty *handler_ptr = NULL;
	
	assert(NULL != FSA_pool);
	assert(NULL != block_to_free);
	
	curr_runner_ptr = (void **)block_to_free; 

	handler_ptr = (FSA_pool_ty *)FSA_pool - 1; /*moving one hadler size back*/
	
	/*reinitializing the deleted block*/
	*curr_runner_ptr = handler_ptr->next_free; 
	
	/*setting the deleted block to next*/
	handler_ptr->next_free = (void *)curr_runner_ptr; 
}

size_t FSACountFree(FSA_pool_ty *FSA_pool)
{
	size_t counter = 0; 
	FSA_pool_ty *handler_ptr = NULL;
	void **curr_runner_ptr = NULL; 
	
	assert(NULL != FSA_pool); 
	
	handler_ptr = (FSA_pool_ty *)FSA_pool - 1; 
	curr_runner_ptr = handler_ptr->next_free; 
	
	while(NULL != curr_runner_ptr)
	{
		++counter; 
		curr_runner_ptr = (void **)*curr_runner_ptr; 
	}
	
	return counter; 
}
