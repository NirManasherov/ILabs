#include <stdio.h>	/*for puts*/
#include "p_queue.h"

/*if test passes return 0 else return 1*/
static int Test(void); 
/*returns 1 if data is prime 0 otherwise*/
static int IsPrimeNum(void *data);
/*return 1 if data is even*/
static int IsEvenNum(void *data);
/*gives priority for prime numbers, after that noneven numbers that are not
prime, and lastly the even numbers. 
i.e.: priority(prime)>priority(odd)>priority(even)*/
static int PriorityFunc(void *data1, void *data2);
/*return 1 if data1 and data2 are equal 0 otherwize*/
static int AreNumsEqual(const void *list_data, void *param);

int main(void) 
{
	if(0 != Test())
	{
		puts("Error! some tests did not passed!");
		return 1; 
	}
	
	puts("Success! all tests passed!");
	return 0;
}

/*if test passes return 0 else return 1*/
static int Test(void)
{
	p_queue_ty *my_p_queue;
	
	/***************************************************************************
	StartD->EndD
	***************************************************************************/
	my_p_queue = PQueueCreate(PriorityFunc);
	
	if(1 != PQueueIsEmpty(my_p_queue))
	{
		puts("Error! can't identify empty p_queue"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->6->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)6);
	if(6 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 6!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->6->9->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)9);
	if(9 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 9!"); 
		return 1; 
	}
	
	if(0 != PQueueIsEmpty(my_p_queue))
	{
		puts("Error! can't identify non-empty p_queue"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->6->9->5->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)5);
	if(5 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 5!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->9->5->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)60);
	if(5 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 60!"); 
		return 1; 
	}
	
	if(4 != PQueueSize(my_p_queue))
	{
		puts("Error! couldnt find size 7!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->15->9->5->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)15);
	if(5 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 15!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->21->15->9->5->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)21);
	if(5 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 21!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->21->15->9->97->5->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)97);/*97 is prime*/
	if(5 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 97!"); 
		return 1; 
	}
	
	if(7 != PQueueSize(my_p_queue))
	{
		puts("Error! couldnt find size 7!"); 
		return 1; 
	}
	
	if(NULL != PQueueErase(my_p_queue, AreNumsEqual, (void *)100)) 
	{
		puts("Erase could not handle a number that is not in the list!");
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->21->15->9->97->EndD
	***************************************************************************/
	PQueueDequeue(my_p_queue);
	if(97 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't dequeue 97!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->21->15->9->EndD
	***************************************************************************/
	PQueueDequeue(my_p_queue);
	if(9 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't dequeue 9!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->21->15->EndD
	***************************************************************************/
	PQueueDequeue(my_p_queue);
	if(15 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't dequeue 15!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->21->EndD
	***************************************************************************/
	PQueueDequeue(my_p_queue);
	if(21 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't dequeue 27!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->6->EndD
	***************************************************************************/
	PQueueDequeue(my_p_queue);
	if(6 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't dequeue 6!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->60->EndD
	***************************************************************************/
	PQueueDequeue(my_p_queue);
	if(60 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't dequeue 6!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->EndD
	***************************************************************************/
	PQueueClear(my_p_queue);
	
	if(1 != PQueueIsEmpty(my_p_queue))
	{
		puts("Error! can't identify empty p_queue after clear"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->23->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)23);
	if(23 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 23!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->16->23->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)16);
	if(23 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 16!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->16->333->23->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)333);
	if(23 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 333!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->16->333->53->23->EndD
	***************************************************************************/
	PQueueEnqueue(my_p_queue, (void *)53); /*53 is prime*/
	if(23 != (int)PQueuePeek(my_p_queue))
	{
		puts("Error! couldn't enqueue 333!"); 
		return 1; 
	}
	
	/***************************************************************************
	StartD->16->333->23->EndD
	***************************************************************************/
	if(53 != (int)PQueueErase(my_p_queue, AreNumsEqual, (void *)53)) 
	{
		puts("Erase could not handle a number that is not in the list!");
		return 1; 
	}
	
	/***************************************************************************
	StartD->EndD
	***************************************************************************/
	PQueueClear(my_p_queue);
	
	if(1 != PQueueIsEmpty(my_p_queue))
	{
		puts("Error! can't identify empty p_queue after clear multiple values"); 
		return 1; 
	}
	
	PQueueDestroy(my_p_queue);
	
	return 0; 
}

/*gives priority for prime numbers, after that noneven numbers that are not
prime, and lastly the even numbers. i.e.: 
priority(prime)>priority(odd)>priority(even)*/
static int PriorityFunc(void *data1, void *data2)
{
	if(IsPrimeNum(data1)&&(!IsPrimeNum(data2)))
	{
		return 1; /*data1>data2*/
	}
	
	if(IsPrimeNum(data2)&&(!IsPrimeNum(data1)))
	{
		return -1; /*data1<data2*/
	}
	
	else /*no prime*/
	{
		if(IsEvenNum(data1)&&(!IsEvenNum(data2))) /*even num has the lowest priority*/
		{
			return -1; /*data1<data2*/
		}
		
		if(IsEvenNum(data2)&&(!IsEvenNum(data1))) /*even num has the lowest priority*/
		{
			return 1; /*data1>data2*/
		}
	}
	
	return 0; /*data1 = data2*/
}

/*returns 1 if data is prime 0 otherwise*/
static int IsPrimeNum(void *data)
{
	int i = 2; 
	int int_data = (int)data; 
	
	for(i = 2 ; i*i <= int_data ; ++i)
	{
		if(0 == int_data%i)
		{
			return 0; /*false*/
		}
	}
	return 1; /*true*/
}

/*return 1 if data is even*/
static int IsEvenNum(void *data)
{
	int int_data = (int)data; 
	
	if(0 == int_data%2)
	{
		return 1; /*true*/
	}
	
	return 0; 
}

/*return 1 if data1 and data2 are equal 0 otherwize*/
static int AreNumsEqual(const void *list_data, void *param)
{
	if((int)param == (int)list_data)
	{
		return 1; 
	}
	
	return 0;
}
