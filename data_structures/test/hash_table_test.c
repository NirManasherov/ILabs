#include "tests.h"
#include "hash_table.h"

#define UNUSED(x) (void)(x)
#define CAPACITY 100
#define KEY_LIMIT 1000

enum {SUCCESS = 0 , FAIL = 1};

static void *GetKey(const void *data);

static size_t HashFunc(const void *key);

static int KeyCmp(const void *curr_key, void *key_to_match);



int main(void)
{
	hash_table_ty *hash_table = NULL; 
	void *key3 = NULL; 
	int data1 = 1; 
	int data2 = 2; 
	int data3 = 3;
	
	hash_table = HTCreate(CAPACITY, HashFunc, KeyCmp, GetKey);
	
	if(1 != HTIsEmpty(hash_table))
	{
		puts("is empty didn't work after create"); 
		return FAIL; 
	}
	
	if(0 != HTSize(hash_table))
	{
		puts("size didn't work after create"); 
		return FAIL;
	}
	
	if(0 != HTInsert(hash_table, (const void *)&data1))
	{
		puts("insert 1 didnt work"); 
		return FAIL; 
	}
	
	if(0 != HTInsert(hash_table, (const void *)&data2))
	{
		puts("insert 2 didnt work"); 
		return FAIL; 
	}
	
	if(0 != HTInsert(hash_table, (const void *)&data3))
	{
		puts("insert 3 didnt work"); 
		return FAIL; 
	}
	
	if(3 != HTSize(hash_table))
	{
		puts("size didn't work after last insert"); 
		return FAIL;
	}
	
	key3 = GetKey((const void *)&data3);
	
	if(NULL == HTFind(hash_table, key3))
	{
		puts("find 3 didnt work"); 
		return FAIL; 
	}
	
	HTRemove(hash_table, key3);
	
	if(NULL != HTFind(hash_table, key3))
	{
		puts("find 3 didnt work after remove"); 
		return FAIL; 
	}
	
	
	HTDestroy(hash_table);
	
	return SUCCESS; 
}	

static void *GetKey(const void *data)
{
	return (void *)data; 
}

static size_t HashFunc(const void *key)
{
	return ((size_t)key)%CAPACITY; 
}

static int KeyCmp(const void *curr_key, void *key_to_match)
{
	return ((size_t)curr_key == (size_t)key_to_match);
}






