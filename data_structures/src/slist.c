#include <stdlib.h>	/*for malloc, free*/ 
#include <assert.h> /*for assert*/
#include <stdio.h>	/*for size_t*/
#include "slist.h"

static int InsertAfter(iter_ty where, const void *data);
static void SwapNodesMembers(iter_ty where);
static void SwapNodesDataMembers(iter_ty where);
static iter_ty FindEnd(iter_ty where); 

typedef struct node node_ty;

struct node
{
    void *data; 
    node_ty *next; 
};

struct slist
{
    node_ty *head;
    node_ty *tail;
};

slist_ty *SlistCreate(void)
{
	struct node *dummy_ptr = NULL; 
	struct slist *my_slist_ptr = NULL; 
	
	my_slist_ptr = (slist_ty *)malloc(sizeof(struct slist));
	
	if(NULL == my_slist_ptr)	/*malloc can fail*/
	{
		return NULL; 
	}
	
	dummy_ptr = (node_ty *)malloc(sizeof(struct node)); 
	
	if(NULL == dummy_ptr)	/*malloc can fail*/
	{
		free(my_slist_ptr);
		my_slist_ptr = NULL; 
		return NULL; 
	}
	
	/*initializing dummu*/
	dummy_ptr->data = my_slist_ptr; 
	dummy_ptr->next = NULL; 
	/*initializing slist*/
	my_slist_ptr->head = dummy_ptr; 
	my_slist_ptr->tail = dummy_ptr; 
	
	return my_slist_ptr; 
}

void SlistDestroy(slist_ty *slist)
{

	node_ty *temp_curr_node_ptr = NULL;
	node_ty *temp_next_node_ptr = NULL;

	assert(NULL != slist);
	temp_curr_node_ptr = slist->head;
	
	do /*free all the nodes*/
	{
		/*save next node ptr*/
		temp_next_node_ptr = temp_curr_node_ptr->next;	
		/*free current node*/
		free(temp_curr_node_ptr); 
		temp_curr_node_ptr = NULL; 
		/*move to the next node in order to free it next iteration*/
		temp_curr_node_ptr = temp_next_node_ptr; 
	}while(NULL != temp_next_node_ptr);	/*stop after freeing dummy*/
	
	/*free slist*/
	slist->head = NULL;
	slist->tail = NULL; 
	free(slist);
	slist = NULL;
}

iter_ty SlistInsert(iter_ty where, const void *data)
{
	int is_tail_need_update = 0; /*false*/ 
	
	assert(NULL != data);
	assert(NULL != where);
	
	if(NULL == where->next)	/*the list is empty and head is dummy*/ 
	{
		is_tail_need_update = 1; 
	}
	
	if(1 == InsertAfter(where, data)) /*fail*/
	{
		return FindEnd(where); 
	} 
	
	SwapNodesDataMembers(where);	
	
	if(1 == is_tail_need_update)
	{
		/*tail is after where and need update*/
		/*dummy data is slist*/
		((slist_ty *)((where->next)->data))->tail = where->next; 
	}
	
	return where;	/*this is the added data after the swap*/
}

static iter_ty FindEnd(iter_ty where)
{
	iter_ty curr_node = where; 
	
	assert(NULL != where);
	
	while(NULL != curr_node->next)/*get to dummy*/
	{
		curr_node = curr_node->next;
	} 
	
	return curr_node;
}

/*this function add node next to "where" node*/
static int InsertAfter(iter_ty where, const void *data)
{
	iter_ty wanted_next_node = NULL; 
	iter_ty next_of_where_node = NULL; 
	
	assert(NULL != where);
	assert(NULL != data);
	
	next_of_where_node = where->next;
	/*make next node*/
	wanted_next_node = (iter_ty)malloc(sizeof(struct node)); 
	
	if(NULL == wanted_next_node)	/*malloc can fail*/
	{
		return 1; /*fail*/
	}
	
	/*assigning wanted next node members*/
	wanted_next_node->data = (void *)data; 
	wanted_next_node->next = next_of_where_node; 
	
	/*assigning next node member of current node (where)*/
	where->next = wanted_next_node; 
	return 0; 
}

static void SwapNodesDataMembers(iter_ty where)
{
	void *curr_data = NULL; 

	assert(NULL != where);
	/*save current node members*/
	curr_data = where->data; 
	/*copying next node members to current node*/
	where->data = (where->next)->data; 
	/*copying current node members to next node from temps varibles*/
	(where->next)->data = curr_data; 
}

iter_ty SlistRemove(iter_ty iter_to_remove)
{
	iter_ty curr_next_node = NULL;
	
	assert(NULL != iter_to_remove);
	/*copies next node pointer because after node swap we would lose its 
	pointer in current node*/
	curr_next_node = iter_to_remove->next; 
	SwapNodesMembers(iter_to_remove);
	
	if(NULL == curr_next_node->next)
	{
		((slist_ty *)(curr_next_node->data))->tail = iter_to_remove; 
	}
	
	/*free next node*/
	curr_next_node->next = NULL; 
	free(curr_next_node); 
	curr_next_node = NULL;
	/*this is the node that containes the next element after the swap of 
	members*/
	
	return iter_to_remove;
}

/*this function swaps the "where" node and the next one of it*/
static void SwapNodesMembers(iter_ty where)
{
	iter_ty curr_next_node = NULL;
	void *curr_data = NULL; 
	
	assert(NULL != where);
	/*save current node members*/
	curr_next_node = where->next; 
	curr_data = where->data; 
	/*copying next node members to current node*/
	where->data = curr_next_node->data; 
	where->next = curr_next_node->next; 
	/*copying current node members to next node from temps varibles*/
	curr_next_node->data = curr_data; 
	curr_next_node->next = curr_next_node;/*the "next node points to itself*/
}

size_t SlistCount(const slist_ty *slist)
{
	node_ty *temp_next_node_ptr = NULL; 
	size_t count = 0; 
	
	assert(NULL != slist);
	temp_next_node_ptr = slist->head; 
	
	while(NULL != temp_next_node_ptr->next)
	{
		++count; 
		temp_next_node_ptr = temp_next_node_ptr->next; 
	}
	
	return count; 
}

iter_ty SlistFind(iter_ty from, iter_ty to, is_match_func_ty match_func, 
																void *param)
{
	iter_ty curr_node = NULL; 
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != match_func);
	curr_node = from; 
	while((to != curr_node)&&(!(match_func(curr_node->data, param))))
	{
		curr_node = curr_node->next; 
	}	
	
	return curr_node; 
}

int SlistForEach(iter_ty from, iter_ty to, action_func_ty action, void *param)
{
	iter_ty curr_node = NULL; 
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action);
	
	curr_node = from; 
	
	while((to != curr_node)&&(0 == (action(curr_node->data, param))))
	{
		curr_node = curr_node->next; 
	}	
	
	if(to == curr_node)
	{
		return 0; /*success!*/ 
	}
	
	return 1; /*fail!*/
}

iter_ty SlistIterBegin(slist_ty *slist)
{
	assert(NULL != slist);
	return (slist->head); 
}

iter_ty SlistIterEnd(slist_ty *slist)
{
	assert(NULL != slist);
	return (slist->tail); 
}

iter_ty SlistIterNext(iter_ty iter)
{
	assert(NULL != iter);
	return (iter->next); 
}

void *SlistIterGetData(iter_ty iter)
{
	assert(NULL != iter);
	return (iter->data);
}

void SlistIterSetData(iter_ty iter, const void *new_data)
{
	assert(NULL != iter);
	assert(NULL != new_data);
	iter->data = (void *)new_data; 
}

int SlistIterIsEqual(iter_ty iterator1, iter_ty iterator2)
{
	assert(NULL != iterator1);
	assert(NULL != iterator2);
	
	if(iterator1 == iterator2) 
	{
		return 1; 
	}
	
	return 0; 
}

void SlistAppend(slist_ty *dest, slist_ty *src)
{
	assert(dest); 
	assert(src);
	
	/*make dest dummy for first node of src*/
	((dest->tail)->data) = ((src->head)->data); 
	((dest->tail)->next) = ((src->head)->next);
	
	dest->tail = src->tail; 
	src->head->next = NULL;
	free(src->head); 
	
	(dest->tail)->data = dest;
	
	src->head = NULL; 
	src->tail = NULL; 
	free(src); 
}
