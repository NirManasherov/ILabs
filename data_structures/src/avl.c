/*******************************************************************************
author: Nir Manasherov
project name: AVL
Date: 08.02.2023
version: 1.2
reviewer: Nazmiya farese
*******************************************************************************/
#include <stddef.h>	/*	size_t		*/
#include <stdlib.h>	/*	malloc free	*/
#include <assert.h> /*	assert		*/

#include "avl.h"
/*******************************macros*****************************************/
#define MAX(x,y) ((x > y) ? x : y)
/********************************enums*****************************************/
typedef enum {MIN_BF = -2, LOW_BF = -1, BALANCED = 0, HIGH_BF = 1, 
							MAX_BF = 2}balance_factor_ty; 
							
typedef enum child{LEFT = 0, RIGHT = 1, NUM_OF_CHILDREN = 2}child_ty;

typedef enum {SUCCESS = 0, FAIL = 1}exit_success_ty; 

typedef enum {ROTATE_LEFT = 0, ROTATE_RIGHT = 1}rot_dir_ty; 

typedef struct avl_node avl_node_ty;
/********************************structs***************************************/
struct avl_node
{
    size_t height;
    void *data;
    avl_node_ty *children[NUM_OF_CHILDREN];
};

struct avl
{
    avl_node_ty *root;
    cmp_func_ty cmp;
};
/***************************function declerations******************************/
static void RecAVLDestroyImp(avl_node_ty *node); 

static size_t ReAVLSizeImp(avl_node_ty *node);

static avl_node_ty *RecAVLInsertImp(avl_node_ty *node, avl_node_ty *new_node,
												  void *data, cmp_func_ty);

static avl_node_ty *RecAVLRemoveImp(avl_node_ty *node, void *data_to_remove,
											      	   	cmp_func_ty cmp_func); 

static avl_node_ty *FindMin(avl_node_ty *node); 

static avl_node_ty *DetMin(avl_node_ty *node); 

static int RecAVLFindImp(avl_node_ty *node, void *data_to_match);

static int RecAVLForEachImp(avl_node_ty *node, order_ty order,
                                        action_func_ty action, void *params); 

static int TraversInOrder(avl_node_ty *node, action_func_ty action, 
																void *params); 

static int TraversPreOrder(avl_node_ty *node, action_func_ty action, 
																void *params);

static int TraversPostOrder(avl_node_ty *node, action_func_ty action, 
																void *params); 

static void CorrectHeight(avl_node_ty *node); 

static int GetBalanceFactor(avl_node_ty *node); 

static avl_node_ty *Rotate(avl_node_ty *node, rot_dir_ty rotation_direction); 

static avl_node_ty *Balance(avl_node_ty *node); 
/*************************function implimentations*****************************/
avl_ty *AVLCreate(cmp_func_ty cmp_func)
{	
	avl_ty *avl = (avl_ty *)malloc(sizeof(avl_ty)); 
	if(NULL == avl) 
	{
		return NULL; 
	}	
	
	assert(NULL != cmp_func);
	
	avl->root = NULL; 
	avl->cmp = cmp_func; 
	
	return avl; 
}
/******************************************************************************/
void AVLDestroy(avl_ty *avl)
{
	assert(NULL != avl); 
	
	RecAVLDestroyImp(avl->root); 
	
	free(avl);
	avl = NULL; 
}
/******************************************************************************/
static void RecAVLDestroyImp(avl_node_ty *node)
{
	if(NULL == node)
	{
		return; 
	} 
	
	RecAVLDestroyImp(node->children[LEFT]); 
	RecAVLDestroyImp(node->children[RIGHT]);
	free(node);  
	node = NULL; 
}
/******************************************************************************/
size_t AVLSize(const avl_ty *avl)
{
	assert(NULL != avl); 
	
	return ReAVLSizeImp(avl->root); 
}
/******************************************************************************/
static size_t ReAVLSizeImp(avl_node_ty *node)
{
	if(NULL == node)
	{
		return 0; 
	}
	
	return (ReAVLSizeImp(node->children[LEFT]) + 
									   ReAVLSizeImp(node->children[RIGHT]) + 1); 
}
/******************************************************************************/
int AVLIsEmpty(const avl_ty *avl)
{
	assert(NULL != avl); 
	
	return (NULL == avl->root);
}
/******************************************************************************/
ssize_t AVLHeight(const avl_ty *avl)
{
	assert(NULL != avl); 
	
	if(NULL == avl->root)
	{
		return -1; 
	}
	
	return (avl->root)->height; 
}
/******************************************************************************/
int AVLInsert(avl_ty *avl, const void *data)
{	
	avl_node_ty *new_node = (avl_node_ty *)malloc(sizeof(avl_node_ty)); 
	if(NULL == new_node)
	{
		return FAIL; 
	} 
	
	assert(NULL != avl); 
	
	new_node->data = (void *)data; 
	new_node->height = 0; 
	new_node->children[LEFT] = NULL; 
	new_node->children[RIGHT] = NULL; 
	
	avl->root = RecAVLInsertImp(avl->root, new_node, (void *)data, avl->cmp); 
	
	return SUCCESS; 
}
/******************************************************************************/
static avl_node_ty *RecAVLInsertImp(avl_node_ty *node, avl_node_ty *new_node,
												  void *data, cmp_func_ty cmp)
{
	int cmp_res = 0; 
	
	if(NULL == node)
	{
		return new_node; 
	}
	
	cmp_res = cmp(node->data, (void *)data);
	
	assert(0 != cmp_res);
	
	/*data > node data*/
	if(0 > cmp_res) 
	{
		node->children[RIGHT] = RecAVLInsertImp(node->children[RIGHT], new_node,
																	data, cmp); 
	}
	
	/*data < node data*/
	else if(0 < cmp_res) 
	{
		node->children[LEFT] = RecAVLInsertImp(node->children[LEFT], new_node,
																	data, cmp);  
	}
	
	CorrectHeight(node);
	return  Balance(node);
}
/******************************************************************************/
void AVLRemove(avl_ty *avl, const void *data_to_remove)
{
	assert(NULL != avl);
	
	avl->root = RecAVLRemoveImp(avl->root, (void *)data_to_remove, avl->cmp); 
	
	return; 
}
/******************************************************************************/
static avl_node_ty *RecAVLRemoveImp(avl_node_ty *node, void *data_to_remove,
											      	   	cmp_func_ty cmp_func)
{
	int cmp_res = 0; 
	avl_node_ty *right_node; 
	avl_node_ty *left_node; 
	avl_node_ty *next_node; 
	
	assert(NULL != cmp_func); 
	
	if(NULL == node)
	{
		return node; 
	}
	
	cmp_res = cmp_func(node->data, data_to_remove);
	
	/*data < node data*/
	if(0 > cmp_res) 
	{
		node->children[RIGHT] = RecAVLRemoveImp(node->children[RIGHT],
								 					data_to_remove, cmp_func); 
		CorrectHeight(node);
		return Balance(node);  
	}
	
	/*data > node data*/
	if(0 < cmp_res) 
	{
		node->children[LEFT] = RecAVLRemoveImp(node->children[LEFT],
								 					data_to_remove, cmp_func);  
		CorrectHeight(node);
		return Balance(node); 
	}
	
	/*here data = node data*/
	left_node = node->children[LEFT]; 
	right_node = node->children[RIGHT]; 
	free(node); 
	node = NULL;
	
	if(NULL == right_node)
	{
		return left_node; 
	}
	
	next_node = FindMin(right_node); 
	
	next_node->children[RIGHT] = DetMin(right_node); 
	
	next_node->children[LEFT] = left_node; 
	
	CorrectHeight(node);
	return Balance(next_node); 
}
/******************************************************************************/
/*detach left sub-tree*/
static avl_node_ty *DetMin(avl_node_ty *node)
{	
	if(NULL == node->children[LEFT])
	{
		return node->children[RIGHT]; 
	}
	
	node->children[LEFT] = DetMin(node->children[LEFT]); 
	
	CorrectHeight(node);
	return Balance(node); 
}
/******************************************************************************/
static avl_node_ty *FindMin(avl_node_ty *node)
{
	assert(NULL != node); 
	
	return (node->children[LEFT] ? FindMin(node->children[LEFT]) : node);
}
/******************************************************************************/
int AVLFind(const avl_ty *avl, const void *data_to_match)
{
	return RecAVLFindImp(avl->root, (void *)data_to_match); 
}
/******************************************************************************/
static int RecAVLFindImp(avl_node_ty *node, void *data_to_match)
{
	int cmp_res = 0; 
	
	if(NULL == node)
	{
		return FAIL; 
	}
	
	/*data > node data*/
	if(0 > cmp_res) 
	{
		return RecAVLFindImp(node->children[RIGHT], data_to_match); 
	}
	
	/*data < node data*/
	if(0 < cmp_res) 
	{
		return RecAVLFindImp(node->children[LEFT], data_to_match); 
	}
	
	return SUCCESS; 
}
/******************************************************************************/
int AVLForEach(avl_ty *avl, order_ty order,
                                        action_func_ty action, void *params)
{
	assert(NULL != avl);
	assert(order >= 0);
	assert(order <= 2);
	assert(NULL != action);   
	
	return RecAVLForEachImp((avl->root), order, action, params); 
}
/******************************************************************************/
static int RecAVLForEachImp(avl_node_ty *node, order_ty order,
                                        action_func_ty action, void *params)
{
	switch (order)
	{
		case PRE: 
			return TraversPreOrder(node, action, params);
		case IN: 
			return TraversInOrder(node, action, params);
		
		case POST:
			return TraversPostOrder(node, action, params);
	}
	
	return FAIL; 
}
/******************************************************************************/
static int TraversInOrder(avl_node_ty *node, action_func_ty action, 
																void *params)
{
	if(NULL == node)
	{
		return SUCCESS; 
	}
	
	TraversInOrder(node->children[LEFT], action, params);
	
	if(SUCCESS != action(node->data, params)) 
	{
		return FAIL; 
	}
	
	TraversInOrder(node->children[RIGHT], action, params);
	
	return SUCCESS;
}
/******************************************************************************/
static int TraversPreOrder(avl_node_ty *node, action_func_ty action, 
																void *params)
{
	if(NULL == node)
	{
		return SUCCESS; 
	}
	
	if(SUCCESS != action(node->data, params)) 
	{
		return FAIL; 
	}
	
	TraversPreOrder(node->children[LEFT], action, params);
	
	TraversPreOrder(node->children[RIGHT], action, params);
	
	return SUCCESS;
}
/******************************************************************************/
static int TraversPostOrder(avl_node_ty *node, action_func_ty action, 
																void *params)
{
	int res = 0; 
	
	if(NULL == node)
	{
		return SUCCESS; 
	}
	
	res += TraversPostOrder(node->children[LEFT], action, params);
	
	res += TraversPostOrder(node->children[RIGHT], action, params);
	
	res += action(node->data, params); 
	
	return !!res; 
}
/******************************************************************************/
static void CorrectHeight(avl_node_ty *node)
{
		avl_node_ty *left_child = NULL; 
		avl_node_ty *right_child = NULL; 
		
		if(NULL == node)
		{
			return; 
		}
		
		left_child = node->children[LEFT];
		right_child = node->children[RIGHT];
		
		if((NULL != left_child) && (NULL != right_child))
		{
			node->height = MAX(left_child->height, right_child->height) + 1; 
			return; 
		}
		
		if((NULL != left_child))
		{
			node->height = left_child->height + 1; 
			return; 
		}
		
		if((NULL != right_child))
		{
			node->height = right_child->height + 1; 
			return; 
		}
		
		node->height = 0; 		
}
/******************************************************************************/
/*calculate the balance factor of node : BF = LC - RC*/
static int GetBalanceFactor(avl_node_ty *node)
{
	ssize_t right_hight = 0; 
	ssize_t left_hight = 0; 
	
	if(NULL == node)
	{
		return BALANCED; 
	}
	
	if(NULL == node->children[LEFT]) 
	{
		left_hight = -1; 
	}
	else
	{
		left_hight = (node->children[LEFT])->height;
	}
	
	if(NULL == node->children[RIGHT])
	{
		right_hight = -1; 
	}
	else
	{
		right_hight = (node->children[RIGHT])->height;
	}
	
	
	
	return ((int)left_hight - (int)(right_hight));
}
/******************************************************************************/
/*rotate according to rotation direction*/
static avl_node_ty *Rotate(avl_node_ty *node, rot_dir_ty rotation_direction)
{
	/*rotate left => right child , rotate right => left child*/
	avl_node_ty *pivot = node->children[!rotation_direction];
	
	node->children[!rotation_direction] = pivot->children[rotation_direction]; 
	
	pivot->children[rotation_direction] = node; 
	
	CorrectHeight(node);
	CorrectHeight(pivot);
	
	return pivot; 	 
}
/******************************************************************************/
static avl_node_ty *Balance(avl_node_ty *node)
{
	int balance_factor = 0; 
	
	if(NULL == node)
	{
		return NULL; 
	}
	
	CorrectHeight(node);
	
	balance_factor = GetBalanceFactor(node); 
	
	/*left is heavy*/
	if(MAX_BF == balance_factor)
	{
		/*if left subtree is unbalanced balance it first*/
		if(LOW_BF == GetBalanceFactor(node->children[LEFT]))
		{
			/*ruin the balance in the sub tree to the "wrong" direction*/
			node->children[LEFT] = Rotate(node->children[LEFT],ROTATE_LEFT); 
		}
		
		/*rotate the parent to the "correct" direction*/
		return Rotate(node,ROTATE_RIGHT); 
	}
	
	/*right is heavy*/
	if(MIN_BF == balance_factor)
	{
		/*if right subtree is unbalanced balance it first*/
		if(HIGH_BF == GetBalanceFactor(node->children[RIGHT]))
		{
			/*ruin the balance in the sub tree to the "wrong" direction*/
			node->children[RIGHT] = Rotate(node->children[RIGHT],ROTATE_RIGHT); 
		}
		
		/*rotate the parent to the "correct" direction*/
		return Rotate(node,ROTATE_LEFT); 
	}
	
	return node; 
}
/******************************************************************************/
