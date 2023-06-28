/*******************************************************************************
Project:    AVL
Author:     HRD28
Version:    1.0 - 06/02/2023
*******************************************************************************/
#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h>     /*  size_t  */

typedef struct avl avl_ty;

typedef struct avl_node avl_node_ty;

typedef enum order {PRE = 0, IN = 1, POST = 2} order_ty;

/*  write a function with the following signature to perform whichever action
    you need on "avl_data"
    "params" is extra data
    return 0 if succeeded, otherwise - not 0                                  */
typedef int (*action_func_ty)(void *avl_data, void *params);

/*  write a function with the following signature to perform the comparison of
    the sorting criteria of your choice
    "avl_data" and "data_to_match" are the compared elements
    returns:    > 0 if avl_data > data_to_match
                < 0 if avl_data < data_to_match
                = 0 if avl_data == data_to_match                              */
typedef int (*cmp_func_ty)(void *avl_data, void *data_to_match, void *params);

/*******************************************************************************
 *  creates an empty AVL tree, "avl", sorted by the comparison
 *  criteria specified by "cmp_func"
 *  returns pointer to "avl" on success, or NULL on failure
 *  Time Complexity: O(1)
*******************************************************************************/
avl_ty *AVLCreate(cmp_func_ty cmp_func);

/*******************************************************************************
pseudo code: 
allocate memory for avl handle 
check if failed
set vaues for avl handle (root = NULL)
*******************************************************************************/





/*******************************************************************************
 *  frees all resources used by "avl"
 *  note: undefined behaviour if "avl" is NULL
 *  Time Complexity: O(n)
*******************************************************************************/
void AVLDestroy(avl_ty *avl);

/*******************************************************************************
pseudo code: 
if left child exist swap data with it and return correct height; 
 
if right child exist swap data with it and return correct height; 

destroy curr_node and return 

**correct height**
put height as the maximum height of both children +1
*******************************************************************************/








/******************************************************************************* 
 *  returns the number of elements in "avl"
 *  note: undefined behaviour if "avl" is NULL
 *  Time Complexity: O(n)
*******************************************************************************/
size_t AVLSize(const avl_ty *avl);

/*******************************************************************************
pseudo code: 
call avlSize for the sum of left child and right child (if exists) + 1
if get to null return 0


*******************************************************************************/









/******************************************************************************* 
 *  returns 1 if "avl" is empty, 0 otherwise
 *  note: undefined behaviour if "avl" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
int AVLIsEmpty(const avl_ty *avl);

/*******************************************************************************
pseudo code: 
return the resault of IsLeaf on dummy
*******************************************************************************/










/******************************************known type name ‘cmp_func_ty’
   24 |     cmp_func_ty cmp;
************************************* 
 *  returns the height of "avl"
 *  note: undefined behaviour if "avl" is NULL
 *  Time Complexity: O(n)
*******************************************************************************/
size_t AVLHeight(const avl_ty *avl);

/*******************************************************************************
pseudo code: 
return the maximum heigth of left child and right child + 1
if node = null -1 
*******************************************************************************/










/******************************************************************************* 
 *  inserts "data" to the correct position in the "avl"
 *  returns 0 if succeeded, 1 otherwise
 *  note: undefined behavior if inserting a duplicate element or "avl" is NULL 
 *  Time Complexity: O(logn)
*******************************************************************************/
int AVLInsert(avl_ty *avl, const void *data);

/*******************************************************************************
pseudo code:
make node and init it with data = data, height = 0 , and so on (if error 
occured return FAIL)  
if the value of node is greater from data correct height and return go right 
if the value of node is less from data correct height and return go left 
if there is no corresponding children add it there:
get resorces (if fail return fail)
if i got HERE sign from returned recursion (returned if node == NULL) add there
the node

 correct height and return SUCCESS
 
 **correct height**
put height as the maximum height of both children +1
*******************************************************************************/










/******************************************************************************* 
 *  removes "data_to_remove" from the "avl"
 *  note: undefined behaviour if "avl" is NULL
 *  Time Complexity: O(logn)
*******************************************************************************/
void AVLRemove(avl_ty *avl, const void *data_to_remove);

/*******************************************************************************
pseudo code: 
use find (side func that return a node) and 
for it's resault: remove the node, replace it with the right 
child if exist (left if right not exist or do nothing if leaf)
append the left child of the swapped node to its new left branch
send the insert func for the corresponding child  
*******************************************************************************/











/******************************************************************************* 
 *  uses "cmp_func" to search "avl" for "data_to_match" till success
 *  returns 0 if found, 1 otherwise
 *  note: undefined behaviour if "avl" is NULL
 *  Time Complexity: O(logn)
*******************************************************************************/
int AVLFind(const avl_ty *avl, const void *data_to_match);

/*******************************************************************************
pseudo code: use find side func 
if the resault isnt null return true else false






find side func(return node that is searched):  
if the value of node is greater from data go right 
if the value of node is less from data go left 
if the data is equal to node return the node pointer to the wanted node 
otherwise return NULL 
*******************************************************************************/















/******************************************************************************* 
 *  performs "action" on each element in the "avl", in "order", till success
 *  returns 0 if succeeded, 1 otherwise
 *  note: undefined behaviour if "avl" is NULL
 *  Time Complexity: O(n)
*******************************************************************************/
int AVLForEach(avl_ty *avl, order_ty order,
                                        action_func_ty action, void *params);


/*******************************************************************************
pseudo code: 
call traverse side func with root_node,  order, action function and param  








side func tranverse (node, order, action function, param)
make order arrays: 
preorder {action curr node, left child recursion call, right child recursion call}
inorder {left child recursion call, action curr node, right child recursion call}
postorder {left child recursion call, right child recursion call, action curr node}

choose order array in which order_ty we got 

in a while loop (that goes on all members of order array) 
go to switch case for the three cases: 
left child recursion call : recForEachImp(node->child[LEFT])
left child recursion call : recForEachImp(node->child[RIGHT])
action curr node : preform action function on current node data with param
*******************************************************************************/
















#endif  /*  __AVL_H__  */
