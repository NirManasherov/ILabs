/**************************libraries***********************************/
#include <stdio.h> /*for puts, size_t*/
#include "stack.h" 

/**********************************************************************/
/**************************functions***********************************/

int main(void) 
{
	int x = 5; 
	int y = 7;
	size_t capacity = 6; 
	size_t size_of_element = 4;
	size_t i = 0; 
	stack_ty *stack_ptr = NULL;
	
	stack_ptr = StackCreate(capacity, size_of_element);
	
	if(1 != StackIsEmpty(stack_ptr))/*stack is empty*/
	{
		puts("Error1!\n");
	}
	
	StackPush(stack_ptr, &x);
	StackPush(stack_ptr, &x);
	StackPush(stack_ptr, &x);
	StackPush(stack_ptr, &x);
	StackPush(stack_ptr, &x);
	StackPush(stack_ptr, &y);
	
	
	if(0 != StackIsEmpty(stack_ptr))/*stack is not empty*/
	{
		puts("Error2!\n");
	}
	
	if(capacity != StackGetCapacity(stack_ptr))
	{
		puts("Error3!\n");
	}
	
	if(capacity != StackGetSize(stack_ptr))
	{
		puts("Error4!\n"); 
	}
	
	for(i = 0 ; i < capacity ; ++i)
	{		
		if(i == 0) 
		{
			if(*(int *)StackPeek(stack_ptr) != y)
			{
				puts("Error5!\n"); 
			}
		}
		else 
		{
			if(*(int *)StackPeek(stack_ptr) != x)
			{
				puts("Error6!\n"); 
			}
		}
		StackPop(stack_ptr);
	}
	StackDestroy(stack_ptr);
	return 0; 
}

/**********************************************************************/
