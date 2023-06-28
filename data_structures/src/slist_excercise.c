#include <assert.h> /*for assert*/
#include <stddef.h> /*for NULL size_t*/
#include "slist_excercise.h"

#define MAX(a,b) (a>b) ? (a) : (b)
#define MIN(a,b) (a>b) ? (b) : (a)

node_t *Flip(node_t *head)
{
	struct node *current = NULL; 
	struct node *previous = NULL; 
	struct node *next = NULL; 
	
	assert(head); 
	
	next = head;  
	
	while(NULL != next)
	{
		current = next; 
		next = current->next; 
		current->next = previous; 
		previous = current;  		
	} 
		
	return current;
}

int HasLoop(const node_t *head)
{
	node_t *fast = NULL;
	node_t *slow = NULL;
	
	assert(head); 
	fast = (node_t *)head->next; 
	slow = (node_t *)head; 
	
	while((fast != slow) && ((fast->next) != NULL))
	{
		fast = ((fast->next)->next);
		slow = slow->next;
	}
	
	if(fast == slow) 
	{
		return 1; /*true => has loop*/
	}
	else /*has loop*/
	{
		return 0; /*false => has no loop*/
	}
}

node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
	node_t *runner1 = NULL; 
	node_t *runner2 = NULL; 
	node_t *bigger = NULL; 
	node_t *smaller = NULL; 
	size_t counter1 = 0; 
	size_t counter2 = 0; 
	size_t length_diff = 0; 
	size_t i = 0;
	
	assert(head_1);
	assert(head_2); 
	runner1 = head_1; 
	runner2 = head_2; 
	
	/*counting list length*/ 
	do
	{
		++counter1;
		runner1 = runner1->next; 
	}while(NULL != runner1->next);
	
	do
	{
		++counter2;
		runner2 = runner2->next; 
	}while(NULL != runner2->next);
	
	runner1 = head_1; 
	runner2 = head_2; 
	
	length_diff = (MAX(counter1,counter2)) - (MIN(counter1,counter2));
		
	if(counter1 >= counter2)
	{
		bigger = runner1; 
		smaller = runner2; 
	}
	
	else
	{
		bigger = runner2; 
		smaller = runner1; 
	}
	
	for(i = 0 ; i < length_diff ; ++i)
	{
		bigger = bigger->next; 		
	}
	
	while(bigger != smaller) 
	{
		bigger = bigger->next; 
		smaller = smaller->next; 
	}	
	if(bigger == smaller) 
	{
		return bigger; 
	}	
	else 
	{
		return NULL;
	}
}
