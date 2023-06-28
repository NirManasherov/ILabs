#include <stdio.h>
#include "slist_excercise.h"

int TestFindIntersection(void);
int TestHasLoop(void);
int TestFlip(void); 

int main(void) 
{
	int fail_counter = 0;
	
	fail_counter += TestHasLoop();
	fail_counter += TestFlip();
	fail_counter += TestFindIntersection();
	if(fail_counter ==0)
	{
		puts("Success! all tests passed!");
	}
	return 0;
}

int TestHasLoop(void)
{
	node_t node1, node2, node3, node4; 
	node1.next = &node2;
	node1.data = (void *)1; 
	node2.next = &node3; 
	node2.data = (void *)2; 
	node3.next = &node4;
	node3.data = (void *)3;  
	node4.next = &node2;
	node4.data = (void *)4;  
	
	if(1 != HasLoop((const node_t *)&node1)) 
	{
		puts("Error HasLoop!");
		return 1;/*fail*/
	}
	return 0; /*success*/
}

int TestFlip(void)
{
	node_t node1, node2, node3, node4;
	node_t *new_head = NULL; 
	node1.next = &node2;
	node1.data = (void *)1; 
	node2.next = &node3; 
	node2.data = (void *)2; 
	node3.next = &node4;
	node3.data = (void *)3;  
	node4.next = NULL;
	node4.data = (void *)4;  
	
	new_head = Flip(&node1);
	
	
	if(&node4 != new_head)
	{
		puts("Error! fail flip 1");
		return 1; /*fail*/
	}
	
	new_head = new_head->next; 
	
	if(&node3 != new_head)
	{
		puts("Error! fail flip 2");
		return 1; /*fail*/
	}
	
	new_head = new_head->next;
	
	if(&node2 != new_head)
	{
		puts("Error! fail flip 3");
		return 1; /*fail*/
	}
	
	new_head = new_head->next;
	
	if(&node1 != new_head)
	{
		puts("Error! fail flip 4");
		return 1; /*fail*/
	}
	return 0; /*success*/
}

int TestFindIntersection(void)
{
	node_t node1, node2, node3, node4, node5, node6; 
	node_t *intersection = NULL;
	node1.next = &node2;
	node1.data = (void *)1; 
	node2.next = &node3; 
	node2.data = (void *)2; 
	node3.next = &node4;
	node3.data = (void *)3;  
	node4.next = &node5;
	node4.data = (void *)4;
	node5.next = NULL;
	node5.data = (void *)5; 
	node6.next = &node3; 
	node6.data = (void *)6; 
	
	intersection = FindIntersection(&node1, &node6); 
	
	
	if(3 != (int)intersection->data)
	{
		puts("Error! failed to find intersection!"); 
		return 1; /*fail*/
	}
	return 0; /*true*/
}

