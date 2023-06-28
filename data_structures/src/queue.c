#include <stdio.h>	/*for size_t*/
#include <assert.h> /*for assert*/
#include "slist.c"
#include "queue.h"

struct queue
{
	slist_ty *slist; 
};

queue_ty *QueueCreate(void)
{
	queue_ty *queue_handler = (queue_ty *)malloc(sizeof(struct queue));
	
	if(NULL == queue_handler)
	{ 
		return NULL; 
	}
	
	queue_handler->slist = SlistCreate(); 
	
	if(NULL == (queue_handler->slist))
	{
		free(queue_handler); 
		queue_handler = NULL; 
		return NULL;
	}
	
	return queue_handler;
}

void QueueDestroy(queue_ty *queue)
{
	assert(queue);
	
	SlistDestroy(queue->slist);
	free(queue); 
	queue = NULL; 
}

void *QueuePeek(const queue_ty *queue)
{
	assert(queue);
	
	return SlistIterGetData(SlistIterBegin((slist_ty *)(queue->slist)));
}

int QueueEnqueue(queue_ty *queue, const void *data)
{
	assert(queue);
	
	if(data == SlistIterGetData(SlistInsert(SlistIterEnd(queue->slist), data)))
	{
		return 0; /*success*/
	}
	
	return 1; /*fail*/
}

void QueueDequeue(queue_ty *queue)
{
	assert(queue);
	
	SlistRemove(SlistIterBegin(queue->slist));
}

int QueueIsEmpty(queue_ty *queue)
{
	assert(queue);
	
	if(SlistIterIsEqual(SlistIterBegin(queue->slist), SlistIterEnd(queue->slist)))
	{
		return 1; /*true*/
	}
	
	return 0; /*false*/
}

size_t QueueSize(queue_ty *queue)
{
	assert(queue);
	
	return SlistCount(queue->slist);
}

void QueueAppend(queue_ty *dest_queue, queue_ty *src_queue)
{
	assert(dest_queue);
	assert(src_queue);
	
	SlistAppend(dest_queue->slist, src_queue->slist);
	free(src_queue); 
	src_queue = NULL; 
}
