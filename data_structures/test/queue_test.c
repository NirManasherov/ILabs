#include <stdio.h> /*for puts, size_t*/
#include "queue.h" 

static int TestQueue(void); 

int main(void) 
{
	if (0 == TestQueue()) 
	{
		puts("All Tests Passed!"); 
		return 0; /*passed*/ 
	} 
	return 1; /*failed*/ 
}

static int TestQueue(void) 
{
	queue_ty *my_queue_src = QueueCreate(); 
	queue_ty *my_queue_dest = QueueCreate();
	size_t i = 0;  
	/*
	dummy
	*/
	queue_ty *my_queue = QueueCreate(); 
	
	if(1 != QueueIsEmpty(my_queue))
	{
		puts("Error! isempty says the queue is not empty when it is!"); 
	}
	
	/*
	1->dummy
	*/
	QueueEnqueue(my_queue, (const void *)1); 
	
	if(1 != (int)QueuePeek(my_queue))
	{
		puts("Error! peeking unsuccessful!"); 
	}
	
	/*
	1->2->dummy
	*/
	QueueEnqueue(my_queue, (const void *)2);
	
	if(1 != (int)QueuePeek(my_queue))
	{
		puts("Error! peeking unsuccessful!"); 
	}
	
	/*
	1->2->3->dummy
	*/
	QueueEnqueue(my_queue, (const void *)3);
	
	if(1 != (int)QueuePeek(my_queue))
	{
		puts("Error! peeking unsuccessful!"); 
	}
	
		if(3 != QueueSize(my_queue))
	{
		puts("Error! size doesnt work!"); 
	}
	
	/*
	2->3->dummy
	*/
	QueueDequeue(my_queue);
	
	if(2 != (int)QueuePeek(my_queue))
	{
		puts("Error! peeking unsuccessful!"); 
	} 
	
	/*
	3->dummy
	*/
	QueueDequeue(my_queue);
	
	if(3 != (int)QueuePeek(my_queue))
	{
		puts("Error! peeking unsuccessful!"); 
	} 
	
	if(0 != QueueIsEmpty(my_queue))
	{
		puts("Error! isempty says the queue is empty when it is not!"); 
	}
	
	if(1 != QueueSize(my_queue))
	{
		puts("Error! size doesnt work!"); 
	}
	
	QueueDestroy(my_queue);
	
	QueueEnqueue(my_queue_dest, (const void *)1); 
	QueueEnqueue(my_queue_dest, (const void *)2); 
	QueueEnqueue(my_queue_dest, (const void *)3); 
	
	QueueEnqueue(my_queue_src, (const void *)4); 
	QueueEnqueue(my_queue_src, (const void *)5); 
	QueueEnqueue(my_queue_src, (const void *)6); 
	
	QueueAppend(my_queue_dest, my_queue_src);  
	
	for(i = 1 ; i <= 6 ; ++i)
	{
		if(i != (int)QueuePeek((const queue_ty *)my_queue_dest))
		{
			printf("%d \n", (int)QueuePeek((const queue_ty *)my_queue_dest));
			printf("Error! peek after append no. %ld is not true!\n", i); 
		}
		
		QueueDequeue(my_queue_dest);
	}
	
	QueueDestroy(my_queue_dest);
	
	return 0;/*success*/
}

