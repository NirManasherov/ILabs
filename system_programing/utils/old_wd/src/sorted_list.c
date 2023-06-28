#include <assert.h>	/*assert*/
#include <stdlib.h>

#include "sorted_list.h"
#include "dlist.h"

struct sort_list
{
	dlist_ty *dlist; 
	cmp_func_ty cmp_func; 
};

static sort_list_iter_ty GetIter(dlist_iter_ty dlist_iter);
static dlist_iter_ty GetDlistIter(sort_list_iter_ty sort_iter);

sort_list_ty *SortedListCreate(cmp_func_ty cmp_func)
{
	sort_list_ty *sort_list_handler = (sort_list_ty *)malloc(sizeof(sort_list_ty));
	
	if(NULL == sort_list_handler)
	{
		return NULL;
	}
	
	assert(NULL != cmp_func); 
	
	sort_list_handler->dlist = DlistCreate(); 
	
	if(NULL == sort_list_handler->dlist) 
	{
		free(sort_list_handler);
		sort_list_handler = NULL;  
	}	
	
	sort_list_handler->cmp_func = cmp_func;
	return sort_list_handler;
}

void SortedListDestroy(sort_list_ty *sort_list)
{
	assert(NULL != sort_list); 
	
	DlistDestroy(sort_list->dlist); 
	sort_list->dlist = NULL;
	free(sort_list); 
	sort_list = NULL; 
}


size_t SortedListSize(const sort_list_ty *sort_list)
{
	assert(NULL != sort_list);
	
	return DlistSize((const dlist_ty *)sort_list->dlist);
}

int SortedListIsEmpty(const sort_list_ty *sort_list)
{
	assert(NULL != sort_list);	
	
	return DlistIsEmpty((const dlist_ty *)sort_list->dlist);
}

sort_list_iter_ty SortedListBegin(const sort_list_ty *sort_list)
{
	assert(NULL != sort_list);
	
	return GetIter(DlistIterBegin((const dlist_ty *)((sort_list_ty *)sort_list->dlist)));
}	

sort_list_iter_ty SortedListEnd(const sort_list_ty *sort_list)
{
	assert(NULL != sort_list);
	
	return GetIter(DlistIterEnd((const dlist_ty *)((sort_list_ty *)sort_list->dlist)));
}

sort_list_iter_ty SortedListNext(sort_list_iter_ty iter)
{
	return GetIter(DlistIterNext(GetDlistIter(iter)));
}

sort_list_iter_ty SortedListPrev(sort_list_iter_ty iter)
{
	return GetIter(DlistIterPrev(GetDlistIter(iter)));
}

int SortedListIterIsEqual(sort_list_iter_ty iter1, sort_list_iter_ty iter2)
{
	return DlistIterIsEqual(GetDlistIter(iter1), GetDlistIter(iter2));
}

void *SortedListGetData(sort_list_iter_ty iter)
{
	return DlistIterGetData(GetDlistIter(iter)); 
}

sort_list_iter_ty SortedListInsert(sort_list_ty *sort_list, const void *data)
{
	sort_list_iter_ty curr_iter;
	void *curr_data = NULL; 
	
	assert(NULL != sort_list);
	
	/*initialization*/
	curr_iter = SortedListBegin(sort_list);
	curr_data = SortedListGetData(curr_iter);
	
	/*while data>curr_data*/
	while(!SortedListIterIsEqual(curr_iter, SortedListEnd(sort_list)) &&
				(0 < (sort_list->cmp_func)((void *)data, curr_data)))/*data > curr_data*/
	{
		curr_iter = SortedListNext(curr_iter); 
		curr_data = SortedListGetData(curr_iter);
	}
	
	return GetIter(DlistInsert(GetDlistIter(curr_iter), data));
}

sort_list_iter_ty SortedListRemove(sort_list_iter_ty iter_to_remove)
{
	return GetIter(DlistRemove(GetDlistIter(iter_to_remove)));
}

void *SortedListPopFront(sort_list_ty *sort_list)
{
	assert(NULL != sort_list);
	
	return DlistPopFront(sort_list->dlist);
}

void *SortedListPopBack(sort_list_ty *sort_list)
{
	assert(NULL != sort_list);
	
	return DlistPopBack(sort_list->dlist);
}

int SortedListForeach(sort_list_iter_ty from, sort_list_iter_ty to, 
                                            action_func_ty action, void *param)
{
	assert(NULL != action);
	
	return DlistForEach(GetDlistIter(from), GetDlistIter(to), action, param);
}

void SortedListMerge(sort_list_ty *dest, sort_list_ty *src)
{
	sort_list_iter_ty to;
	sort_list_iter_ty from; 
	sort_list_iter_ty where;
	
	assert(NULL != src);
	assert(NULL != dest);
	
	where = SortedListBegin(dest);
	from = SortedListBegin(src); 
	to = SortedListBegin(src); 
	
	while(1 != SortedListIterIsEqual(where, SortedListEnd(dest)))
	{
		/*while(to<where)*/
		while(0 < (dest->cmp_func(SortedListGetData(where), SortedListGetData(to))))
		{
			to = SortedListNext(to);
		}
		
		/*we need to copy*/
		if(0 < (dest->cmp_func(SortedListGetData(where), SortedListGetData(from))))
		{
			DlistSplice(GetDlistIter(where), GetDlistIter(from), GetDlistIter(to));
		}
		
		where = SortedListNext(where); 
		from = SortedListBegin(src); 
		to = SortedListBegin(src); 
	}
	
	if (1 != SortedListIsEmpty(src))	/*need to copy the end to dest*/
	{
		to = SortedListEnd(src);
		DlistSplice(GetDlistIter(where), GetDlistIter(from), GetDlistIter(to));
	}
	
	SortedListDestroy(src);
}

sort_list_iter_ty SortedListFind(sort_list_ty *list, sort_list_iter_ty from, 
                                 sort_list_iter_ty to, const void *param)
{
	
	sort_list_iter_ty curr_iter;
	
	assert(NULL != list);
	
	curr_iter = from;
	
	while(!(SortedListIterIsEqual(curr_iter, SortedListEnd(list))) &&
				   (0 > list->cmp_func(SortedListGetData(curr_iter), (void *)param)))
	{
		curr_iter = SortedListNext(curr_iter);
	}
	
	if(0 != list->cmp_func(SortedListGetData(curr_iter), (void *)param))
	{
		return to;
	}
	
	return curr_iter; 
}

sort_list_iter_ty SortedListFindIf(sort_list_iter_ty from, sort_list_iter_ty to, 
                                is_match_func_ty match_func, const void *param)
{
	assert(NULL != match_func);
	
	return GetIter(DlistFind(GetDlistIter(from), GetDlistIter(to), match_func, 
																(void *)param));
}

static sort_list_iter_ty GetIter(dlist_iter_ty dlist_iter)
{
    sort_list_iter_ty sort_iter;

    sort_iter.dlist_iter = dlist_iter;

    return sort_iter;
}

static dlist_iter_ty GetDlistIter(sort_list_iter_ty sort_iter)
{
    dlist_iter_ty dlist_iter = sort_iter.dlist_iter;

    return dlist_iter;
}
