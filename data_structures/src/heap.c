/*******************************************************************************
* Author: Nir Manasherov
* Project: hash table
* Reviewer: 
* Date: 9/02/2023
* Version: 1.0
*******************************************************************************/
/*******************************************************************************
                                  libraries
*******************************************************************************/

#include <stddef.h> 	/* 	size_t 			*/
#include <assert.h>		/*	assert			*/
#include <stdlib.h>		/*	malloc free		*/
#include <sys/types.h>	/*	ssize_t			*/

#include "heap.h"
#include "dynamic_vector.h"

/*******************************************************************************
                                  macros
*******************************************************************************/  

#define CAPACITY 32
#define DATA_WASNT_FOUND -1 

#define MAX(x,y) ((x > y) ? x : y) 

/*******************************************************************************
                                  macros
*******************************************************************************/      

enum {SUCCESS = 0, FAIL = 1};

enum {FALSE = 0, TRUE = 1}; 

enum {LEFT = 1, RIGHT =2}; 

/*******************************************************************************
                                  structs
*******************************************************************************/   

struct heap
{
    d_vector_ty *d_vector;
    heap_cmp_func_ty cmp_func;
};

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

/*init the heap handler members to sent members*/
static void InitHeapHandler(heap_ty *heap, heap_cmp_func_ty cmp_func, 
														  d_vector_ty *d_vector); 


static int CmpAndReturnIdx(const heap_ty *heap, int idx1, int idx2);
static void UpHeap(const heap_ty *heap, int idx);
											
/*get heap dynamic vector*/
static d_vector_ty *GetDV(const heap_ty *heap);

/*get heap cmpare function*/
static heap_cmp_func_ty GetCmpFunc(heap_ty *heap);

/*return the parent's index of a spesified member index*/
static ssize_t GetParent(size_t index); 

/*reutrn the child index spesify child direction*/
static size_t GetChild(size_t index, int direction); 

/*make a up heap algorithm to member in spesified index*/
/*static void UpHeap(heap_ty *heap, size_t index); */

/*make a up heap algorithm to member in spesified index*/
static void DownHeap(heap_ty *heap, size_t index); 

/*swap two data of void pointers*/
static void SwapData(void **data1_ptr, void **data2_ptr); 

/*find an element in heap by data*/
static ssize_t FindElem(heap_ty *heap, heap_is_match_func_ty match_func, 
															 const void *data);

/*swap two data of void pointers*/
static void SwapData(void **data1_ptr, void **data2_ptr); 

/*return true id child index is valid by heap size, false otherwise*/
static int IsChildValid(size_t heap_size, size_t child_index); 

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/

heap_ty *HeapCreate(heap_cmp_func_ty cmp_func)
{
	heap_ty *heap = NULL; 
	d_vector_ty *d_vec = NULL; 
	
	assert(NULL != cmp_func); 
	
	/*dv create*/
	d_vec = VectorCreate(CAPACITY, sizeof(void *));
	
	/*if failed return FAIL*/
	if(NULL == d_vec)
	{
		return NULL;
	}
	
	/*allocate resources for heap handle*/ 
	heap = (heap_ty *)malloc(sizeof(heap_ty)); 
	
	/*if fail DV destroy*/
	if(NULL == heap)
	{
		return NULL;
	}
	
 	/*init heap handle members*/
 	InitHeapHandler(heap, cmp_func, d_vec); 
	 GetDV(heap);

	return heap; 
}

/******************************************************************************/

/*init the heap handler members to sent members*/
static void InitHeapHandler(heap_ty *heap, heap_cmp_func_ty cmp_func, 
														  d_vector_ty *d_vector)
{
	heap->d_vector = d_vector; 
	heap->cmp_func = cmp_func; 
}

void HeapDestroy(heap_ty *heap)
{
	assert(NULL != heap); 
	
	/*dv destroy*/
	VectorDestroy(GetDV(heap)); 
	
	/*deallocate heap handle*/
	heap->cmp_func = NULL; 
	heap->d_vector = NULL; 
	
	free(heap); 
	heap = NULL; 
}

/******************************************************************************/

int HeapPush(heap_ty *heap, const void*data)
{
	assert(NULL != heap); 
	assert(NULL != data);
	
	/*VectorPushBack (add the member to the last place at the DV )*/
	if(SUCCESS != VectorPushBack(GetDV(heap), data))
	{
		return FAIL;
	}
	
	/*up-heap*/
	UpHeap(heap, HeapSize(heap) - 1); 
	
	return SUCCESS; 
}

/******************************************************************************/

void HeapPop(heap_ty *heap)
{
	assert(NULL != heap); 
	assert(!HeapIsEmpty(heap)); 
	
	/*swap data of first and last of heap*/
	SwapData(VectorGetAccessToElement(GetDV(heap),HeapSize(heap) - 1), 
									VectorGetAccessToElement(GetDV(heap), 0));
	
	/*VectorPopBack*/
	VectorPopBack(GetDV(heap));
	
	/*down heap first idx*/
	DownHeap(heap, 0); 
}

/******************************************************************************/

void *HeapPeek(const heap_ty *heap)
{
	assert(NULL != heap);
	
	return VectorGetAccessToElement(GetDV(heap), 0); 	
}

/******************************************************************************/

void *HeapRemove(heap_ty *heap, heap_is_match_func_ty match_func, const void *data)
{
	/*find index of removed data*/
	ssize_t wanted_data_idx = 0;
	void *data_to_remove = NULL; 
	
	assert(NULL != heap);
	assert(NULL != match_func); 
	assert(NULL != data); 
	
	wanted_data_idx = FindElem(heap, match_func,  data);
	data_to_remove = *(void **)VectorGetAccessToElement(GetDV(heap), wanted_data_idx); 
	
	if(DATA_WASNT_FOUND == wanted_data_idx)
	{
		return NULL;
	}
	 
	SwapData(VectorGetAccessToElement(GetDV(heap), (HeapSize(heap) - 1)), 
					   VectorGetAccessToElement(GetDV(heap), wanted_data_idx));
	
	/*VectorPopBack*/
	VectorPopBack(GetDV(heap));
	
	/*find correct place for swapped data using down heap*/
	if(!HeapIsEmpty(heap) && (HeapSize(heap) != (size_t)wanted_data_idx))
	{
		DownHeap(heap, wanted_data_idx); 
	}
	
	return data_to_remove; 
}

/******************************************************************************/

int HeapIsEmpty(const heap_ty *heap)
{
	assert(NULL != heap); 
	
	return !HeapSize(heap);
}

/******************************************************************************/

size_t HeapSize(const heap_ty *heap)
{
	assert(NULL != heap); 
	
	return (VectorGetSize(GetDV(heap))); 
}

/*******************************************************************************
                        Static functions implimentations
*******************************************************************************/

/*get heap dynamic vector*/
static d_vector_ty *GetDV(const heap_ty *heap)
{
	return heap->d_vector; 
}

/******************************************************************************/

/*get heap cmpare function*/
static heap_cmp_func_ty GetCmpFunc(heap_ty *heap)
{
	return heap->cmp_func; 
}

/******************************************************************************/

/*return the parent's index of a spesified member index*/
static ssize_t GetParent(size_t index)
{	
	return (index - (2 - index%2)) / 2; 
}

/******************************************************************************/

/*reutrn the child index spesify child direction*/
static size_t GetChild(size_t index, int direction)
{
	return (2*index + direction); 
}

/******************************************************************************/

/*make a up heap algorithm to member in spesified index*/
static void UpHeap2(heap_ty *heap, size_t index)
{
	void *parent_data = NULL; 
	void *child_data = NULL; 
	size_t child_idx = 0; 
	ssize_t parent_idx = 0; 
	
	/*initiaqlize starting data and index*/
	child_idx = index;
	parent_idx = GetParent(child_idx); 
	
	if(DATA_WASNT_FOUND == parent_idx)
	{
		return;
	}
	
	parent_data = (void *)VectorGetAccessToElement(GetDV(heap), parent_idx);
	child_data = (void *)VectorGetAccessToElement(GetDV(heap), child_idx); 
	
	/*while VectorGetAccessToElement(get parent) is smaller than curr_data*/
	while(0 > GetCmpFunc(heap)(parent_data, child_data))
	{
		/*swap(parent_ptr, child_ptr)*/
		SwapData(VectorGetAccessToElement(GetDV(heap), child_idx), 
						    VectorGetAccessToElement(GetDV(heap), parent_idx));
		
		/*move up the tree*/
		child_idx = GetParent(child_idx); 
		parent_idx = GetParent(child_idx);
		
		if(DATA_WASNT_FOUND == parent_idx)
		{
			return;
		}
		
		/*update data for comparison*/
		parent_data = VectorGetAccessToElement(GetDV(heap), parent_idx);
		child_data = VectorGetAccessToElement(GetDV(heap), child_idx); 
	}
}
/*
static void HeapDown(const heap_ty *heap, int idx)
{
    int left_idx = 0, right_idx = 0;
    int child = 0;
    int need_swap = 0;
    d_vector_ty *vector = NULL;
    void **parent_data = NULL;
    void **child_data = NULL;

    assert(heap);
    
    vector = heap->d_vector;
    

    while(1)
    {
        right_idx = GetRightChildIdx(idx);
        left_idx = GetLeftChildIdx(idx);
        
        if (!IsValidIdx(heap, left_idx))
        {            
            break;
        }
        
        if (IsValidIdx(heap, right_idx))
        {

            child = CmpAndReturnIdx(heap, right_idx, left_idx); 
        }
        else
        {
            child = left_idx;
        }
        
        need_swap = CmpAndReturnIdx(heap, idx, child);
        
        if (need_swap == child)
        {
            parent_data = VectorGetAccessToElement(vector, idx);
            child_data = VectorGetAccessToElement(vector, child);
            
            Swap(parent_data, child_data);
            idx = child;
        }
        else
        {
            break;
        }
    }
}*/

static void UpHeap(const heap_ty *heap, int idx)
{
    int need_swap = 0;
    int parent_idx = 0;
    void **parent_data = NULL;
    void **child_data = NULL;
    d_vector_ty *vector = NULL;

    assert(heap);
    
    vector = heap->d_vector;

    /* save the pointer to the element using VectorGetAccessToElement()*/
    while(1)
    {
        /* cmp between idx and parent */
        parent_idx = GetParent(idx);

        if (parent_idx < 0)
        {
            return;
        }
        
        need_swap = CmpAndReturnIdx(heap, idx, parent_idx);
        
        /* if the parent is bigger swap them */
        if (need_swap == idx)
        {
            parent_data = VectorGetAccessToElement(vector, parent_idx);
            child_data = VectorGetAccessToElement(vector, idx);
            
            SwapData(parent_data, child_data);
            
            idx = parent_idx;
        }
        else
        {
            return;
        }
    }
}

static int CmpAndReturnIdx(const heap_ty *heap, int idx1, int idx2)
{
    d_vector_ty *vector = NULL;
    void *idx1_data = NULL;
    void *idx2_data = NULL;
    int cmp_res = 0;
    
    assert(heap);
    
    vector = heap->d_vector;
    
    idx1_data = (void *)VectorGetAccessToElement(vector, idx1);
    idx2_data = (void *)VectorGetAccessToElement(vector, idx2);
    
    cmp_res = heap->cmp_func(idx1_data, idx2_data);
    
    if (0 < cmp_res)
    {
        return idx1;
    }
    else if (0 > cmp_res)
    {
        return idx2;
    }
    
    return -1;
}


/******************************************************************************/

/*make a up heap algorithm to member in spesified index*/
static void DownHeap(heap_ty *heap, size_t index)
{	
	int is_continue = TRUE;
	size_t idx = index; 
	size_t right_idx = 0; 
	size_t left_idx = 0; 
	size_t heap_size = 0; 
	ssize_t child = -1; 
	
	void *right_child_val = NULL;
	void *left_child_val = NULL; 
	void *child_val = NULL; 
	void *curr_val = NULL; 
	
	heap_size = HeapSize(heap); 
	right_idx = GetChild(idx, RIGHT);
	left_idx = GetChild(idx, LEFT);
	
	right_child_val = VectorGetAccessToElement(GetDV(heap), right_idx);
	left_child_val = VectorGetAccessToElement(GetDV(heap), left_idx);
	
	while(is_continue)
	{
		{
			/*if left child doesn't exists - break*/
			if(!IsChildValid(heap_size, GetChild(idx, LEFT)))
			{
				is_continue = FALSE; 
			}
			/*if left child is valid*/
			else
			{
				/*and right child is valid*/
				if(IsChildValid(heap_size, GetChild(idx, RIGHT)))
				{
					/*take the maximum valued child*/
					/*right > left*/
					if(0 < GetCmpFunc(heap)(right_child_val, left_child_val))
					{
						child = right_idx; 
					}
					/*left > right*/
					else
					{
						child = left_idx; 
					}
				}
				/*only left child is valid*/
				else 
				{
					child = left_idx;
				}
				
				child_val = VectorGetAccessToElement(GetDV(heap), child);
				curr_val = VectorGetAccessToElement(GetDV(heap), idx);
				
				/*if need to swap maximum child => curr_val < child*/
				if(0 < GetCmpFunc(heap)(child_val, curr_val))
				{
					/*swap child with parent*/
					SwapData(VectorGetAccessToElement(GetDV(heap), child), 
									VectorGetAccessToElement(GetDV(heap), idx)); 
					
					/*update indexes for next iteration*/
					idx = child; 
					right_idx = GetChild(idx, RIGHT);
					left_idx = GetChild(idx, LEFT);
					
					right_child_val = VectorGetAccessToElement(GetDV(heap), 
																	right_idx);
					left_child_val = VectorGetAccessToElement(GetDV(heap), 	
																	left_idx);
				}
				/*parent is in place => break*/
				else
				{
					is_continue = FALSE; 
				}
			}	
		}
	}	
}

/******************************************************************************/

static int IsChildValid(size_t heap_size, size_t child_index)
{
	return (child_index < heap_size); 
}

/******************************************************************************/

/*swap two data of void pointers*/
static void SwapData(void **data1_ptr, void **data2_ptr)
{
	if (data1_ptr != data2_ptr)
	{
		*data1_ptr = (void *)(((size_t)(*data1_ptr))^((size_t)(*data2_ptr))); 
		*data2_ptr = (void *)(((size_t)(*data1_ptr))^((size_t)(*data2_ptr)));  
		*data1_ptr = (void *)(((size_t)(*data1_ptr))^((size_t)(*data2_ptr))); 
	}
}

/******************************************************************************/

/*find an element in heap by data*/
static ssize_t FindElem(heap_ty *heap, heap_is_match_func_ty match_func, 
															 const void *data)
{
	size_t idx = 0; 
	size_t heap_size = 0; 
	
	heap_size = HeapSize(heap);
	
	for(idx = 0 ; idx < heap_size ; ++idx)
	{
		if(1 == match_func(VectorGetAccessToElement(GetDV(heap), idx), 
																  (void *)data))
		{
			return idx; 
		}
	}
	
	return DATA_WASNT_FOUND;
}

/******************************************************************************/
