#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/

#include "dlist.h"

typedef struct dlist_node dlist_node_ty;

struct dlist
{
	dlist_node_ty *front; 
	dlist_node_ty *end; 	
};

struct dlist_node 
{
	void *data; 
	dlist_node_ty *next;
	dlist_node_ty *prev;
}; 

/*get the node of a corresponding iter*/
static dlist_node_ty *IterToNode(dlist_iter_ty iterator); 
/*get the iter of a corresponding node*/
static dlist_iter_ty NodeToIter(dlist_node_ty *node);
/*this function removes node of corresponding iter from the list*/
static void Remove(dlist_iter_ty iter_to_remove); 
/*this function insert node with data "data" one place before "where"
returns boolian value of if it did it correctly*/
static int Insert(dlist_iter_ty where, const void *data); 
/*this function returns the end dummy of list of iter*/
static dlist_iter_ty GetEnd(dlist_iter_ty where);

/*get the node of a corresponding iter*/
static dlist_iter_ty NodeToIter(dlist_node_ty *node_ptr) 
{
	return node_ptr; 
} 

/*get the iter of a corresponding node*/
static dlist_node_ty *IterToNode(dlist_iter_ty iterator) 
{
	return (dlist_node_ty *)iterator; 
} 


dlist_ty *DlistCreate(void)
{
	dlist_node_ty *start_dummy = NULL; 
	dlist_node_ty *end_dummy = NULL;
	dlist_ty *my_dlist = NULL;
	
	start_dummy = (dlist_node_ty *)malloc(sizeof(dlist_node_ty));
	
	if(NULL == start_dummy) 
	{
		return NULL;
	}
	
	end_dummy = (dlist_node_ty *)malloc(sizeof(dlist_node_ty));
	
	if(NULL == end_dummy) 
	{
		free(start_dummy);
		return NULL;
	}
	
	my_dlist = (dlist_ty *)malloc(sizeof(dlist_ty));
	
	if(NULL == my_dlist) 
	{
		free(start_dummy);
		free(end_dummy);
		return NULL;
	}
	
	/*initialization dummies*/
	start_dummy->prev = NULL; 
	start_dummy->next = end_dummy;
	start_dummy->data = my_dlist;
	
	end_dummy->prev = start_dummy; 
	end_dummy->next = NULL; 
	end_dummy->data = my_dlist; 
	
	/*dlist*/
	(my_dlist->front) = start_dummy; 
	(my_dlist->end) = end_dummy; 
	
	return my_dlist; 
}

void DlistDestroy(dlist_ty *dlist) 
{
	dlist_node_ty *my_node = NULL;
	dlist_node_ty *my_next_node = NULL;
	
	assert(NULL != dlist); 
		
	my_node = dlist->front; 
	
	do
	{
		my_next_node = my_node->next; 
		free(my_node);
		my_node = NULL; 
		my_node = my_next_node; 
	}while(my_node->next != NULL);
	
	free(my_node);
	dlist->front = NULL;  
	dlist->end = NULL; 
	free(dlist); 
	dlist = NULL; 
}

size_t DlistSize(const dlist_ty *dlist)
{
	dlist_node_ty *curr_node = NULL;
	size_t counter = 0; 
	
	assert(NULL != dlist); 
	curr_node = dlist->front->next;	/*(->next) for front dummy*/ 
	
	while(curr_node->next != NULL)
	{
		++counter; 
		curr_node = curr_node->next; 
	}
	
	return counter;
}

int DlistIsEmpty(const dlist_ty *dlist)
{
	assert(NULL != dlist); 
	
	if(dlist->front->next == dlist->end)
	{
		return 1;	/*true*/
	}
	
	return 0; 	/*false*/
}

dlist_iter_ty DlistIterBegin(const dlist_ty *dlist)
{
	assert(NULL != dlist); 
	
	return NodeToIter(dlist->front->next);
}

dlist_iter_ty DlistIterEnd(const dlist_ty *dlist)
{
	assert(NULL != dlist); 
	
	return NodeToIter(dlist->end);
}

dlist_iter_ty DlistIterNext(dlist_iter_ty iter)
{
	return NodeToIter(IterToNode(iter)->next);
}

dlist_iter_ty DlistIterPrev(dlist_iter_ty iter)
{
	return NodeToIter(IterToNode(iter)->prev);
}

int DlistIterIsEqual(dlist_iter_ty iter1, dlist_iter_ty iter2)
{
	assert((NULL != IterToNode(iter1)) && (NULL != IterToNode(iter2))); 
	
	if(IterToNode(iter1)==IterToNode(iter2))
	{
		return 1; /*true*/
	}
	
	return 0; /*false*/
}

void *DlistIterGetData(dlist_iter_ty iter)
{
	assert(NULL != IterToNode(iter));
	 
	return NodeToIter(IterToNode(iter)->data);
}

/*this function removes node of corresponding iter from the list*/
static void Remove(dlist_iter_ty iter_to_remove)
{
	dlist_node_ty *node_to_remove = NULL; 
	dlist_node_ty *prev_node = NULL; 
	dlist_node_ty *next_node = NULL; 
	
	assert(NULL != IterToNode(iter_to_remove)); 
		
	node_to_remove = IterToNode(iter_to_remove);
	prev_node = node_to_remove->prev; 
	next_node = node_to_remove->next; 
	
	prev_node->next = next_node; 
	next_node->prev = prev_node; 
	
	free(node_to_remove); 
	node_to_remove = NULL; 
}

/*this function insert node with data "data" one place before "where"
returns boolian value of if it did it correctly*/
static int Insert(dlist_iter_ty where, const void *data)
{
	dlist_node_ty *new_node = NULL;
	
	new_node = (dlist_node_ty *)malloc(sizeof(dlist_node_ty));
	
	assert(NULL != IterToNode(where));
	
	if(NULL == new_node) 
	{
		return 1;/*fail -> couldnt do the addition correctly*/
	}
	
	/*initializing new node*/
	new_node->data = (void *)data; 
	new_node->prev = where->prev;
	new_node->next = where;
	/*connecting new node to the list*/ 
	where->prev->next = new_node; 
	where->prev = new_node; 
	
	return 0; /*success -> could make the addition correctly*/
}

dlist_iter_ty DlistPushBack(dlist_ty *dlist, const void *data)
{
	dlist_node_ty *where;
	
	assert(NULL != dlist); 
	
	where =  dlist->end;
	
	if(1 == Insert(where, data))
	{
		return where; 
	}
	
	return NodeToIter(where->prev); 
}

void *DlistPopBack(dlist_ty *dlist)
{
	void *data = NULL;
	dlist_node_ty *node_to_remove; 
	
	assert(NULL != dlist); 
	
	node_to_remove = (dlist->end)->prev;
	data = node_to_remove->data; 
	Remove(NodeToIter(node_to_remove));
	return data;
}

dlist_iter_ty DlistPushFront(dlist_ty *dlist, const void *data) 
{
	dlist_node_ty *where;
	
	assert(NULL != dlist); 
	
	where =  dlist->front->next; /*insert add befor where*/
	
	if(1 == Insert(where, data))
	{
		return (dlist->end); 
	}
	return where->prev; 
}

void *DlistPopFront(dlist_ty *dlist)
{
	void *data = NULL;
	dlist_node_ty *node_to_remove; 
	
	assert(NULL != dlist); 
	
	node_to_remove = (dlist->front)->next;
	data = node_to_remove->data; 
	Remove(NodeToIter(node_to_remove));
	return data;
}

/*this function returns the end dummy of list of iter*/
static dlist_iter_ty GetEnd(dlist_iter_ty where)
{
	dlist_iter_ty res; 
	dlist_node_ty *where_node;
	
	assert(NULL != IterToNode(where));
	
	where_node = IterToNode(where);
	
	while(NULL != where_node->next)
	{
		where_node = where_node->next; 
	}
	
	res = NodeToIter(where_node);
	return res; 
}

dlist_iter_ty DlistInsert(dlist_iter_ty where, const void *data)
{	 	
	 	assert(NULL != IterToNode(where)); 
	 	
	 	if(1 == Insert(where, data))
		{
			return GetEnd(where); 
		}
		return where->prev; 
}

dlist_iter_ty DlistRemove(dlist_iter_ty iter_to_remove)
{
	dlist_node_ty *next_node; 
	
	assert(NULL != IterToNode(iter_to_remove)); 
	
	next_node = IterToNode(iter_to_remove)->next; 
	Remove(iter_to_remove); 
	
	return next_node;
}

dlist_iter_ty DlistFind(dlist_iter_ty from, dlist_iter_ty to, 
                        is_match_func_ty match_func, void *param)
{
	dlist_node_ty *curr_node ; 
	
	assert(NULL != IterToNode(from));
	assert(NULL != IterToNode(to));
	assert(NULL != match_func);
	
	curr_node = IterToNode(from); 
	while((to != curr_node)&&(!(match_func(curr_node->data, param))))
	{
		curr_node = curr_node->next; 
	}	
	
	return NodeToIter(curr_node); 
}

int DlistMultiFind(dlist_iter_ty from, dlist_iter_ty to, 
                    is_match_func_ty match_func, void *param, 
                    dlist_ty *dest_dlist)
{
	dlist_node_ty *from_node; 
	dlist_node_ty *to_node; 
	
	assert(NULL != IterToNode(from));
	assert(NULL != IterToNode(to));
	assert(NULL != match_func);
	
	from_node = IterToNode(from); 
	to_node = IterToNode(to); 
		
	while(from_node != to_node)
	{
		from_node = IterToNode(DlistFind(NodeToIter(from_node), to, match_func, 
																		param));
		if(to_node != from_node)/*find success*/
		{
			if(dest_dlist->end == DlistPushBack((dest_dlist), 
							DlistIterGetData(from_node)))/*pushback fail*/
			{
				return 1; /*fail*/
			}
		}
			
		from_node = from_node->next;
	}
		
	return 0;
}

int DlistForEach(dlist_iter_ty from, dlist_iter_ty to, action_func_ty action, 
																	void *param)
{
	dlist_node_ty *curr_node = NULL; 
	dlist_node_ty *to_node = NULL;
	
	assert(NULL != IterToNode(from));
	assert(NULL != IterToNode(to));
	assert(NULL != action);
	
	curr_node = IterToNode(from); 
	to_node = IterToNode(to); 
	
	while((to_node != curr_node)&&(0 == (action(curr_node->data, param))))
	{
		curr_node = curr_node->next; 
	}	
	
	if(to_node == curr_node)
	{
		return 0; /*success!*/ 
	}
	
	return 1; /*fail!*/
}

dlist_iter_ty DlistSplice(dlist_iter_ty where, dlist_iter_ty from, dlist_iter_ty to)
{
	dlist_node_ty *where_node = NULL;
	dlist_node_ty *prev_where_node = NULL; 
	dlist_node_ty *to_node = NULL;
	dlist_node_ty *to_prev_node = NULL;
	dlist_node_ty *from_node = NULL;
	
	assert(NULL != IterToNode(from));
	assert(NULL != IterToNode(to));
	assert(NULL != IterToNode(where));
	
	where_node = IterToNode(where);
	to_node = IterToNode(to); 
	from_node = IterToNode(from); 
	
	to_prev_node = to_node->prev; 
	
	prev_where_node = where_node->prev; 
	
	from_node->prev->next = to_node; 
	
	to_node->prev = from_node->prev;
	
	from_node->prev = prev_where_node;
	
	prev_where_node->next = from_node; 
	
	to_prev_node->next = where_node;
	
	where_node->prev = to_prev_node;
	
	return NodeToIter(to_node); 
}
