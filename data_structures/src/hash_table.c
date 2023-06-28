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

#include <stddef.h>         /*		size_t      			*/
#include <assert.h>			/*		assert		 			*/
#include <stdlib.h>			/*		malloc, calloc, free	*/
#include <stdio.h>

#include "hash_table.h"
#include "dlist.h"          /*   dlist_ty    */

/*******************************************************************************
                                  macros
*******************************************************************************/      

#define UNUSED(x) (void)(x)

/*******************************************************************************
                                  structs
*******************************************************************************/              

struct hash_table
{
	size_t ht_capacity;
	hash_func_ty hash_func;
	hash_cmp_func_ty cmp_func;
	get_key_func_ty get_key; 
	dlist_ty **hash_table; 
}; 

typedef struct data
{
	void *key; 
	void *value; 
}data_ty; 

typedef struct find_structure
{
	hash_cmp_func_ty cmp_func;
	void *key; 
}find_ty; 

/*******************************************************************************
                                  enums
*******************************************************************************/

enum {SUCCESS = 0, DLIST_CREATE_FAIL = 1, DLIST_INSERT_FAIL = 2, 
														CANT_FIND_DATA = 3}; 

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

/*initialize hash table manager struct*/
static void InitHashManager(hash_table_ty *hash_table, size_t capacity, 		
	 hash_func_ty hash_func, hash_cmp_func_ty cmp_func, get_key_func_ty get_key, 
														  dlist_ty **dlist_arr);

/*traverse the dlist table and destroy it's content, capacity is the table size*/
static void DestroyDlistTable( dlist_ty **dlist_table, size_t capacity);

/*create a dlist in spesified place if it has not been set yet*/
static int CreateDlistInTable(dlist_ty **wanted_dlist);
															  
/*find a fit for key in hash table and return the dlist iter to it*/
static dlist_iter_ty HTFindDIter(hash_table_ty *hash_table, const void *key, 
										   	 dlist_iter_ty *wanted_end_iter);

/*uses cmp_func. return 1 if match is found, 0 otherwise*/
static int MatchFunc(const void *ds_data, void *params); 

/*free data struct in dlist node*/
static int ActionFree(void *hash_data, void *params);

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/

hash_table_ty *HTCreate(size_t capacity, hash_func_ty hash_func, 
                            hash_cmp_func_ty cmp_func, get_key_func_ty get_key)
{
	hash_table_ty *new_hash_table = NULL; 
	dlist_ty **hash_table = NULL; 
	
	/*assert undefined behaviour sources*/
	assert(NULL != hash_func); 
	assert(NULL != cmp_func); 
	assert(NULL != get_key); 
	assert(0 != capacity); 
	
	/*do sorces allocations of manager struct of hash table*/
	new_hash_table = (hash_table_ty *)malloc(sizeof(hash_table_ty));
	
	/*is allocation failed return "fail"*/
	if(NULL == new_hash_table) 
	{
		return NULL; 
	}
	
	/*allocate sorces for the d-list table*/
	hash_table = (dlist_ty **)calloc(capacity, sizeof(dlist_ty *)); 
	
	/*if allocation failed free manager struvt and return "fail"*/
	if(NULL == hash_table) 
	{
		free(new_hash_table); 
		new_hash_table = NULL; 
		return NULL; 
	}
	
	/*initialize hash table manager struct*/
	InitHashManager(new_hash_table, capacity, hash_func, cmp_func, get_key, 		
																	hash_table);
	
	/*return the manager struct*/
	return new_hash_table; 
}

/******************************************************************************/

/*initialize hash table manager struct*/
static void InitHashManager(hash_table_ty *hash_table, size_t capacity, 		
	 hash_func_ty hash_func, hash_cmp_func_ty cmp_func, get_key_func_ty get_key, 
														   dlist_ty **dlist_arr)
{
	hash_table->ht_capacity = capacity; 
	hash_table->hash_func = hash_func; 
	hash_table->cmp_func = cmp_func; 
	hash_table->get_key = get_key; 
	hash_table->hash_table = dlist_arr; 
}

/******************************************************************************/

void HTDestroy(hash_table_ty *hash_table)
{
	size_t capacity = 0; 
	dlist_ty **dlist_table = NULL; 	
	
	/*assert hash table*/
	assert(NULL != hash_table); 
	
	/*get dlist table*/
	dlist_table = hash_table->hash_table; 
	
	/*get dlist table size*/
	capacity = hash_table->ht_capacity; 
	
	/*destroy data structs in nodes*/
	HTForEach(hash_table, ActionFree, NULL);
			
	/*traverse the dlist table and destroy it's content*/
	DestroyDlistTable(dlist_table, capacity); 
	
	/*the stroy the dlist table*/
	free(dlist_table); 
	hash_table->hash_table = NULL; 
	
	/*free hash table manager*/
	free(hash_table); 
	hash_table = NULL;
}

/******************************************************************************/

/*traverse the dlist table and destroy it's content, capacity is the table size*/
static void DestroyDlistTable( dlist_ty **dlist_table, size_t capacity)
{
	size_t idx = 0; 
	
	for(idx = 0 ; idx < capacity ; ++idx)
	{
		/*if dlist is set destroy it*/
		if(0 != (size_t)((void *)dlist_table[idx]))
		{
			DlistDestroy(dlist_table[idx]); 
		}
	}
}

/******************************************************************************/

void HTRemove(hash_table_ty *hash_table, const void *key)
{ 
	hash_func_ty hash_func;
	size_t dlist_idx = 0; 
	dlist_ty *wanted_dlist = NULL;
	dlist_iter_ty wanted_data_iter; 
	dlist_iter_ty wanted_end_iter;
	
	/*assert hash_table is valid*/
	assert(NULL != hash_table); 
	
	hash_func = hash_table->hash_func;
	/*get index in dlist table using hash_func*/
	dlist_idx = hash_func(key);
	
	/*assert index is valid*/
	assert(dlist_idx < hash_table->ht_capacity); 
	/*get correct dlist*/
	wanted_dlist = (hash_table->hash_table)[dlist_idx];
	
	/*if dlist doesn't exist do nothing*/
	if(0 != (size_t)((void *)wanted_dlist))
	{
		wanted_data_iter = HTFindDIter(hash_table, key, &wanted_end_iter);
		
		free(DlistIterGetData(wanted_data_iter));
		/*remove the wanted data from dlist*/
		DlistRemove(wanted_data_iter); 
	}
}

/******************************************************************************/

int HTInsert(hash_table_ty *hash_table, const void *value)
{
	void *key = NULL; 
	get_key_func_ty get_key; 
	hash_func_ty hash_func;
	size_t dlist_idx = 0; 
	dlist_ty **wanted_dlist = NULL; 
	data_ty *data; 
	
	/*assert the hash_table*/
	assert(NULL != hash_table); 
	
	get_key = hash_table->get_key;
	/*get key from value*/
	key = get_key(value);
	
	hash_func = hash_table->hash_func; 
	/*get index in dlist table using hash_func*/
	dlist_idx = hash_func(key);
	
	/*assert index is valid*/
	assert(dlist_idx < hash_table->ht_capacity); 
	/*get correct dlist*/
	wanted_dlist = &(hash_table->hash_table)[dlist_idx];
	
	/*if dlist isnt set in the wanted place - create it*/
	if(DLIST_CREATE_FAIL == CreateDlistInTable(wanted_dlist))
	{
		return DLIST_CREATE_FAIL; 
	}
	
	data = (data_ty *)malloc(sizeof(data_ty));
	/*make data struct that include the key*/
	data->key = key;
	data->value = (void *)value;
	
	/*insert data in dlist*/
	if(DlistIterIsEqual(DlistIterEnd(*wanted_dlist),
									 DlistPushBack(*wanted_dlist, (void *)data)))
	{
		/*if dlistInsert fail return fail*/
		return DLIST_INSERT_FAIL; 
	}
	
	/*return success*/
	return SUCCESS; 	 
}

/******************************************************************************/

/*create a dlist in spesified place if it has not been set yet*/
static int CreateDlistInTable(dlist_ty **wanted_dlist)
{
	/*if dlist isn't set in the wanted place*/
	if(0 == (size_t)((void *)*wanted_dlist)) 
	{
		/*create new dlist in appropriate place*/
		*wanted_dlist = DlistCreate(); 
		
		/*if dlistCreate fail return fail*/
		if(NULL == *wanted_dlist) 
		{
			return DLIST_CREATE_FAIL; 
		}
	}
	
	return SUCCESS;
}

/******************************************************************************/

size_t HTSize(hash_table_ty *hash_table)
{
	size_t data_counter = 0; 
	size_t dlist_table_idx = 0; 
	size_t capacity = 0; 
	dlist_ty *curr_dlist = NULL;
		
	/*assert hash table is valid*/
	assert(NULL != hash_table);
	
	capacity = hash_table->ht_capacity; 
	
	/*traverse on dlist table*/
	for(dlist_table_idx = 0 ; dlist_table_idx < capacity ; ++dlist_table_idx)
	{
		/*get current dlist*/
		curr_dlist = hash_table->hash_table[dlist_table_idx]; 
		
		/*if a dlist has formed*/
		if(0 != (size_t)((void *)curr_dlist)) 
		{
			data_counter += DlistSize(curr_dlist);
		}
	}	
	
	return data_counter; 
}

/******************************************************************************/

int HTIsEmpty(hash_table_ty *hash_table)
{
	/*assert hash table*/
	assert(hash_table); 
	
	/*return TRUE = 1 if HTSize return 0 and return FALSE = 0 otherwise*/
	return !HTSize(hash_table); 
}

/******************************************************************************/

void *HTFind(hash_table_ty *hash_table, const void *key)
{
	dlist_iter_ty wanted_data_iter;
	dlist_iter_ty wanted_end_iter;
	
	/*assert hash table and key to be valid*/
	assert(NULL != hash_table); 
	assert(NULL != key); 
	
	/*get iter to wanted data*/
	wanted_data_iter = HTFindDIter(hash_table, key, &wanted_end_iter); 
	
	/*if found*/
	if(!DlistIterIsEqual(wanted_data_iter,wanted_end_iter))
	{
		return ((data_ty *)(DlistIterGetData(wanted_data_iter)))->value;
	}
	
	return NULL;
}

/******************************************************************************/

size_t HTForEach(hash_table_ty *hash_table, hash_action_func_ty action, 
                                                                  void *params)
{
	size_t altered_data_counter = 0; 
	size_t dlist_arr_idx = 0; 
	size_t dlist_arr_size = 0; 
	dlist_ty *curr_dlist = NULL;
	
	/*assert hash table and action function to be valid*/
	assert(NULL != hash_table); 
	assert(NULL != action); 
	
	dlist_arr_size = hash_table->ht_capacity; 
	
	for(dlist_arr_idx = 0 ; dlist_arr_idx < dlist_arr_size ; ++dlist_arr_idx)
	{
		/*get current dlist*/
		curr_dlist = (hash_table->hash_table)[dlist_arr_idx]; 
		
		/*if dlist has been set*/
		if(0 != (size_t)((void *)curr_dlist)) 
		{
			/*if dlistForEach was successful*/
			if(0 == DlistForEach(DlistIterBegin(curr_dlist), 
								DlistIterEnd(curr_dlist), action, params))
			{
				/*update counter for altered data*/
				altered_data_counter += DlistSize(curr_dlist); 
			}
		}
	}
	
	return altered_data_counter; 
}

/******************************************************************************/

/*uses cmp_func. return 1 if match is found, 0 otherwise*/
static int MatchFunc(const void *ds_data, void *params)
{
	find_ty *find_struct = NULL;
	hash_cmp_func_ty cmp_func;
	void *key = NULL; 
	void *key_to_cmp = NULL;
	
	assert(NULL != params);
	
	find_struct = (find_ty *)params;
	cmp_func = find_struct->cmp_func;
	key = find_struct->key; 
	
	key_to_cmp = ((data_ty *)ds_data)->key; 
	
	return cmp_func(key_to_cmp, key); 
}

/******************************************************************************/

/*find a fit for key in hash table and return the dlist iter to it*/
static dlist_iter_ty HTFindDIter(hash_table_ty *hash_table, const void *key, 
												dlist_iter_ty *wanted_end_iter)
{
	size_t dlist_table_idx = 0; 
	size_t capacity = 0; 
	dlist_ty *curr_dlist = NULL;
	dlist_iter_ty wanted_data_iter; 
	find_ty find_struct; 
	
	/*assert hash table and key to be valid*/
	assert(NULL != hash_table); 
	assert(NULL != key); 
	
	/*get hash table capacity*/ 
	capacity = hash_table->ht_capacity;
	
	/*traverse the array of dlist and search the key in every dlist*/
	for(dlist_table_idx = 0 ; dlist_table_idx < capacity ; ++dlist_table_idx)
	{
		/*get current dlist*/
		curr_dlist = hash_table->hash_table[dlist_table_idx]; 
		
		/*if a dlist has formed*/
		if(0 != (size_t)((void *)curr_dlist)) 
		{
			*wanted_end_iter = DlistIterEnd(curr_dlist); 
			
			/*make find struct*/
			find_struct.cmp_func = hash_table->cmp_func; 
			find_struct.key = (void *)key; 
			
			wanted_data_iter = DlistFind(DlistIterBegin(curr_dlist), 
					DlistIterEnd(curr_dlist), MatchFunc,(void *)(&find_struct));
			
			/*if found*/								
			if(!DlistIterIsEqual(wanted_data_iter, DlistIterEnd(curr_dlist)))
			{
				return wanted_data_iter;
			}		
		}
	}
	
	/*not found*/
	return *wanted_end_iter; 
}

/******************************************************************************/

/*free data struct in dlist node*/
static int ActionFree(void *hash_data, void *params)
{
	UNUSED(params);
	
	free(hash_data);
	hash_data = NULL;
	
	return SUCCESS;
}

/******************************************************************************/
