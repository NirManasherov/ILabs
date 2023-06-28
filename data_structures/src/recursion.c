#include <assert.h>	/*	assert	*/
#include <string.h>	/*	strncmp	*/

#include "recursion.h"

#define ABS(x) ((x < 0) ? -x : x)

enum {FALSE = 0, TRUE = 1}; 

int IterativelyFibonacci(int element_index)
{
	int prev_num = 1; 	/*index 1*/
	int next_num = 1;	/*index 2*/
	int temp_num = 0; 
	int index = 3; 
	
	if((1 == element_index) || (2 == element_index))
	{
		return next_num; 	/*num = 1*/
	}
	
	for(index = 3 ; index <= element_index ; ++index)
	{
		temp_num = next_num; 
		next_num += prev_num; 
		prev_num = temp_num; 
	}
	
	return next_num; 	
}

int RecursiveFibonacci(int element_index)
{
	if((1 == element_index) || (2 == element_index))
	{
		return 1; 	/*num = 1*/
	}
	
	return (RecursiveFibonacci(element_index - 1) + 
								RecursiveFibonacci(element_index - 2)); 
}

node_ty *RecursiveFlipList(node_ty *head)
{
	node_ty *node = head; 
	
	assert (NULL != head);
	 
	if(NULL == head->next)
	{
		return head; 
	}
	
	head = RecursiveFlipList(head->next); 
	node->next->next = node; 
	node->next = NULL; 
	
	return head; 
}

static void StackInsertSort(stack_ty *stack, void *data)
{
    void *tmp = NULL;
    
    if ((TRUE == StackIsEmpty(stack)) || *(int *)StackPeek(stack) < *(int *)data)

    {
        StackPush(stack, data);
        
        return;
    }
    
    tmp = StackPeek(stack); 

    StackPop(stack);

 	/* find correct place in stack */
    StackInsertSort(stack, data);   
    
	/* push the data back in after the insertion */
    StackPush(stack, tmp);
}

stack_ty *SortStack(stack_ty *stack)
{
    void *data = NULL;
    
    if (TRUE == StackIsEmpty(stack))

    {    
    	/* start stack insertion sort on an empty stack */
        return stack;
    }
	/* save data in stack last member that got in the stack */
    data = StackPeek(stack);    

    StackPop(stack);

	/* sort the rest of the stack */
    SortStack(stack);    

	/* insert data in right place */
    StackInsertSort(stack, data);    

	return stack;
}

size_t RecursiveStrlen(const char *s)
{
	if('\0' == *s)
	{
		return 0;
	}
	
	++s; 
	
	return (RecursiveStrlen(s) + 1);
}

int RecursiveStrcmp(const char *s1, const char *s2)
{
	int res = 0; 
	
	if(*s1 == *s2)
	{
		if(*s1 == '\0')
		{
			return 0; 
		}
		
		++s1;
		++s2; 
		
		return RecursiveStrcmp(s1, s2);
	}
	
	res = ((int)*s1 - (int)*s2);
	return res/(ABS(res)); 
}

char *RecursiveStrcpy(char *dest, const char *src)
{
	*dest = *src; 
	
	if(*src == '\0')
	{
		return dest;
	}
	
	return RecursiveStrcpy(++dest, ++src) - 1;
}

char *RecursiveStrcat(char *dest, const char *src)
{
	RecursiveStrcpy((dest + RecursiveStrlen(dest)), src);
	
	return dest;
}


char *RecursiveStrstr(const char *haystack, const char *needle)
{
	if(0 == strncmp(haystack, needle, RecursiveStrlen(needle)))
	{
		return (char *)haystack;
	}
	
	if (0 < (RecursiveStrlen(haystack) - RecursiveStrlen(needle)))
	{
		++haystack;
		return RecursiveStrstr(haystack, needle);
	}
	
	return NULL;	 
}
