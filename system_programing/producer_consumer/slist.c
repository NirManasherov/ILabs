#include <stddef.h>     /*  size_t          */
#include <stdlib.h>     /*  malloc, free    */
#include <assert.h>     /*  assert          */

#include "slist.h"

typedef struct node
{
    void *data;
    struct node *next;
} node_ty;

struct slist
{
    node_ty *head;  /*  ptr to first valid element, or "tail" if empty  */
    node_ty *tail;  /*  ptr to the dummy end node                       */
};

/*  returns 1 if iter points to slist.tail                                    */
static int IsDummyEnd(node_ty *node_ptr);

/*  create an iterator to a NULL node                                         */
static iter_ty GetNullIter(void);

/*  find the dummy node of a list that contains "iter"                        */
static iter_ty GoToDummyEnd(iter_ty iter);

/*  get node from iterator                                                    */
static node_ty *GetNode(iter_ty iter);

/*  get iterator from node                                                    */
static iter_ty GetIter(node_ty *node_ptr);

/*  copy "src" into "dest"                                                    */
static void CopyNode(node_ty *dest, node_ty *src);

/*******************************************************************************
*******************************************************************************/

slist_ty *SlistCreate(void)
{
    slist_ty *slist = (slist_ty *)malloc(sizeof(slist_ty));
    node_ty *end_dummy = NULL;
    
    if (NULL == slist)
    {
        return NULL;
    }
    
    end_dummy = (node_ty *)malloc(sizeof(node_ty));
    
    if (NULL == end_dummy)
    {
        free(slist);
        slist = NULL;
        
        return NULL;
    }
    
    slist->tail = end_dummy;
    slist->head = end_dummy;
    
    /*  init "end_dummy"  */
    slist->tail->data = slist;
    slist->tail->next = NULL;
        
    return slist;
}

/*******************************************************************************
*******************************************************************************/

void SlistDestroy(slist_ty *slist)
{
    /*  traverse through list:
        save next address, free current address 
        finally, free slist                                                   */
    iter_ty curr_iter = GetNullIter();
    node_ty *curr_node = NULL;
    
    assert(NULL != slist);
    
    curr_iter = SlistIterBegin(slist);
        
    while (!IsDummyEnd(curr_iter))
    {
        curr_node = GetNode(curr_iter);
        
        curr_iter = SlistIterNext(curr_iter);
        
        free(curr_node);
        curr_node = NULL;
    }
    
    curr_node = GetNode(curr_iter);
    free(curr_node);
    curr_node = NULL;
    
    slist->head = NULL;
    slist->tail = NULL;
    
    free(slist);
    slist = NULL;
}

/*******************************************************************************
*******************************************************************************/

iter_ty SlistInsert(iter_ty where, const void *data)
{
    /*  malloc a new node_ty*, if failed return SlistIterEnd()                */
    node_ty *new_node = NULL;
    node_ty *curr_node = NULL;
    int flag_is_dummy_end = IsDummyEnd(where);
    
    curr_node = GetNode(where);
    assert(NULL != curr_node);
    
    new_node = (node_ty *)malloc(sizeof(node_ty));
    if (NULL == new_node)
    {
        return GoToDummyEnd(where);
    }
        
    /*  copy curr_node to the new node,
        set curr_node's data to "data", and its next to the new node          */
    CopyNode(new_node, curr_node);
    
    curr_node->data = (void *)data;
    curr_node->next = new_node;
    
    if(flag_is_dummy_end)
    {
        ((slist_ty *)new_node->data)->tail = new_node;
    }
    
    /*  "curr_node" is still pointed to by "where"  */
    return where;
}

/*******************************************************************************
*******************************************************************************/

iter_ty SlistRemove(iter_ty iter_to_remove)
{
    /*  store next node's data in "iter_to_remove", and free next node.
        return "iter_to_remove"                                               */
    node_ty *curr_node = NULL;
    node_ty *next_node = NULL;
    
    assert(!IsDummyEnd(iter_to_remove));
    
    curr_node = GetNode(iter_to_remove);
    next_node = GetNode(SlistIterNext(iter_to_remove));
        
    CopyNode(curr_node, next_node);
    free(next_node);
    next_node = NULL;
    
    if(IsDummyEnd(curr_node))
    {
        ((slist_ty *)curr_node->data)->tail = curr_node;
    }
    
    /*  curr_node has changed to the next node  */
    return iter_to_remove;
}

/*******************************************************************************
*******************************************************************************/

size_t SlistCount(const slist_ty *slist)
{
    size_t counter = 0;
    iter_ty curr_iter = NULL;
    
    assert(NULL != slist);
    
    curr_iter = SlistIterBegin((slist_ty *)slist);
        
    while (!IsDummyEnd(curr_iter))
    {
        ++counter;
        
        curr_iter = SlistIterNext(curr_iter);
    }
    
    return counter;
}

/*******************************************************************************
*******************************************************************************/

iter_ty SlistFind(iter_ty from, iter_ty to, is_match_func_ty match_func, void *param)
{
    assert(!IsDummyEnd(from));
    assert(NULL != match_func); 
        
    /*  while !SlistIterIsEqual(from, to) && !match_func(from's data, param)  
        "++from"    */
    while (!SlistIterIsEqual(from, to) && 
            !match_func(GetNode(from)->data, param))
    {
        from = SlistIterNext(from);
    }
    
    return from;
}

/*******************************************************************************
*******************************************************************************/

int SlistForEach(iter_ty from, iter_ty to, action_func_ty action, void *param)
{
    int res = 0;
    
    assert(!IsDummyEnd(from));
    assert(NULL != action);
            
    /*  while !SlistIterIsEqual(from, to) && res = !action(from's data, param)  
        "++from"    */
    while (!SlistIterIsEqual(from, to) && 
            (res = !action(GetNode(from)->data, param)))
    {
        from = SlistIterNext(from);     
    }
    
    return !res;
}

/*******************************************************************************
*******************************************************************************/

iter_ty SlistIterBegin(slist_ty *slist)
{
    assert(NULL != slist);
    
    return GetIter(slist->head);
}

/*******************************************************************************
*******************************************************************************/

iter_ty SlistIterEnd(slist_ty *slist)
{
    assert(NULL != slist);
    
    return GetIter(slist->tail);
}

/*******************************************************************************
*******************************************************************************/

iter_ty SlistIterNext(iter_ty iter)
{
    assert(!IsDummyEnd(iter));
    
    return GetIter(GetNode(iter)->next);
}

/*******************************************************************************
*******************************************************************************/

void *SlistIterGetData(iter_ty iter)
{
    assert(!IsDummyEnd(iter));
    
    return GetNode(iter)->data;
}

/*******************************************************************************
*******************************************************************************/

void SlistIterSetData(iter_ty iter, const void *new_data)
{
    assert(!IsDummyEnd(iter));
    
    GetNode(iter)->data = (void *)new_data;
}

/*******************************************************************************
*******************************************************************************/

int SlistIterIsEqual(iter_ty iterator1, iter_ty iterator2)
{    
    return (GetNode(iterator1) == GetNode(iterator2));
}

/*******************************************************************************
*******************************************************************************/
void SlistAppend(slist_ty *dest_slist, slist_ty *src_slist)
{
/*  change dest tail->next to point to src head
    from dest remove the middle dummy
    change dest tail to src tail
    destroy src                                                               */
    iter_ty dest_og_end = GetNullIter();
    iter_ty src_begin = GetNullIter();
    iter_ty src_end = GetNullIter();
    
    assert(NULL != dest_slist);
    assert(NULL != src_slist);
    
    dest_og_end = SlistIterEnd(dest_slist);
    src_begin = SlistIterBegin(src_slist);
    src_end = SlistIterEnd(src_slist);

    GetNode(dest_og_end)->next = GetNode(src_begin);
    dest_slist->tail = GetNode(src_end);
    GetNode(SlistIterEnd(dest_slist))->data = (void *)dest_slist;
    
    SlistRemove(dest_og_end);
    
    src_slist->head = NULL;
    src_slist->tail = NULL;
    free(src_slist);
    src_slist = NULL;
}

/*******************************************************************************
*******************************************************************************/

static int IsDummyEnd(iter_ty iter)
{
    return (NULL == GetNode(iter)->next);
}

/******************************************************************************/

static iter_ty GetNullIter(void)
{
    return GetIter(NULL);
}

/******************************************************************************/

static iter_ty GoToDummyEnd(iter_ty iter)
{
    while (!IsDummyEnd(iter))
    {
        iter = SlistIterNext(iter);
    }
    
    return iter;
}        

/******************************************************************************/

static node_ty *GetNode(iter_ty iter)
{
    /*  iter_ty is currently node_ty*   */
    return (node_ty *)iter;
}

/******************************************************************************/

static iter_ty GetIter(node_ty *node_ptr)
{
    /*  iter_ty is currently node_ty*   */
    return (iter_ty)node_ptr;
}

/******************************************************************************/

static void CopyNode(node_ty *dest, node_ty *src)
{
    *dest = *src;
}