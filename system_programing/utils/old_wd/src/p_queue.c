/*********************************************************************
made by: Nir Manasherov
Date: 29.11.2022
Reviewer: Nadav Mansur
Version: 1
***********************************************************************/

#include <assert.h>	/*assert*/
#include <stdlib.h> /*malloc size_t*/

#include "sorted_list.h"
#include "p_queue.h"

struct p_queue
{
	sort_list_ty *my_sorted_list;
};

p_queue_ty *PQueueCreate(cmp_func_ty cmp_priority)
{
	sort_list_ty *my_sorted_list;
	p_queue_ty *my_p_queue;
	
	assert(NULL != cmp_priority);
		
	my_p_queue = (p_queue_ty *)malloc(sizeof(p_queue_ty));
	
	if(NULL == my_p_queue)
	{
		return NULL;
	}
	
	my_sorted_list = SortedListCreate(cmp_priority);
	
	if(NULL == my_sorted_list)
	{
		free(my_p_queue);
		my_p_queue = NULL;
		
		return NULL;
	}
	
	my_p_queue->my_sorted_list = my_sorted_list;
	
	return my_p_queue;
}

void PQueueDestroy(p_queue_ty *p_queue)
{
	assert(NULL != p_queue); 
	
	SortedListDestroy(p_queue->my_sorted_list);
	p_queue->my_sorted_list = NULL; 
	
	free(p_queue); 
	p_queue = NULL;
}

void *PQueuePeek(const p_queue_ty *p_queue)
{
	assert(NULL != p_queue); 
	assert(!PQueueIsEmpty((p_queue_ty *)p_queue));
	
	return SortedListGetData(SortedListPrev(SortedListEnd(p_queue->my_sorted_list))); 
}

int PQueueEnqueue(p_queue_ty *p_queue, const void *data)
{
	  assert(NULL != p_queue);
    
    return SortedListIterIsEqual(SortedListInsert((p_queue->my_sorted_list),data), 		
    								SortedListEnd(p_queue->my_sorted_list));
}

void PQueueDequeue(p_queue_ty *p_queue)
{
	assert(NULL != p_queue);
	assert(!PQueueIsEmpty(p_queue));
	
	SortedListPopBack(p_queue->my_sorted_list);
}

int PQueueIsEmpty(p_queue_ty *p_queue)
{
	assert(NULL != p_queue);
	
	return SortedListIsEmpty(p_queue->my_sorted_list); 
}

size_t PQueueSize(p_queue_ty *p_queue)
{
	assert(NULL != p_queue);
	
	return SortedListSize(p_queue->my_sorted_list);
}

void PQueueClear(p_queue_ty *p_queue)
{
	assert(NULL != p_queue);
	
	while(!PQueueIsEmpty(p_queue))
	{
		PQueueDequeue(p_queue); 
	}
}

void *PQueueErase(p_queue_ty *p_queue, is_match_func_ty match_func, void *param)
{
	sort_list_iter_ty begin_iter;
	sort_list_iter_ty end_iter;
	sort_list_iter_ty remove_iter;
	void *data_to_remove;
	
	assert(NULL != p_queue);
	assert(NULL != match_func);
	
	begin_iter = SortedListBegin(p_queue->my_sorted_list);
	end_iter = SortedListEnd(p_queue->my_sorted_list);
	remove_iter = SortedListFindIf(begin_iter,end_iter, match_func, param);
	data_to_remove = SortedListGetData(remove_iter);
		
	if (!SortedListIterIsEqual(remove_iter, end_iter))
	{		
		SortedListRemove(remove_iter);
		
		return data_to_remove;		
	}
	
	return NULL;
}
