/*******************************************************************************
* Author: Nir Manasherov
* Project: hash table
* Reviewer: 
* Date: 9/02/2023
* Version: 1.0
*******************************************************************************/
/*******************************************************************************
                                  libraries
*******************************************************************************/

#include <assert.h>	/*assert*/
#include <stdlib.h> /*malloc size_t*/

#include "heap.h"
#include "p_queue.h"
#include "utilities.h"


/*******************************************************************************
                                  structs
*******************************************************************************/ 

struct p_queue
{
	heap_ty *heap;
};

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/

p_queue_ty *PQueueCreate(cmp_func_ty cmp_priority)
{
	heap_ty *my_heap;
	p_queue_ty *my_p_queue;
	
	assert(NULL != cmp_priority);
		
	my_p_queue = (p_queue_ty *)malloc(sizeof(p_queue_ty));
	
	if(NULL == my_p_queue)
	{
		return NULL;
	}
	
	my_heap = HeapCreate(cmp_priority);
	
	if(NULL == my_heap)
	{
		free(my_p_queue);
		my_p_queue = NULL;
		
		return NULL;
	}
	
	my_p_queue->heap = my_heap;
	
	return my_p_queue;
}

/******************************************************************************/

void PQueueDestroy(p_queue_ty *p_queue)
{
	assert(NULL != p_queue); 
	
	HeapDestroy(p_queue->heap);
	p_queue->heap = NULL; 
	
	free(p_queue); 
	p_queue = NULL;
}

/******************************************************************************/

void *PQueuePeek(const p_queue_ty *p_queue)
{
	assert(NULL != p_queue); 
	assert(!PQueueIsEmpty((p_queue_ty *)p_queue));
	
	return HeapPeek(p_queue->heap); 
}

/******************************************************************************/

int PQueueEnqueue(p_queue_ty *p_queue, const void *data)
{
	  assert(NULL != p_queue);
    
    return HeapPush(p_queue->heap, data);
}

/******************************************************************************/

void PQueueDequeue(p_queue_ty *p_queue)
{
	assert(NULL != p_queue);
	assert(!PQueueIsEmpty(p_queue));
	
	HeapPop(p_queue->heap);
}

/******************************************************************************/

int PQueueIsEmpty(p_queue_ty *p_queue)
{
	assert(NULL != p_queue);
	
	return HeapIsEmpty(p_queue->heap); 
}

/******************************************************************************/

size_t PQueueSize(p_queue_ty *p_queue)
{
	assert(NULL != p_queue);
	
	return HeapSize(p_queue->heap);
}

/******************************************************************************/

void PQueueClear(p_queue_ty *p_queue)
{
	assert(NULL != p_queue);
	
	while(!PQueueIsEmpty(p_queue))
	{
		PQueueDequeue(p_queue); 
	}
}

/******************************************************************************/

void *PQueueErase(p_queue_ty *p_queue, is_match_func_ty match_func, void *param)
{
	assert(NULL != p_queue);
	assert(NULL != match_func);
	
	return HeapRemove(p_queue->heap, match_func, (const void *)param);;
}

/******************************************************************************/
