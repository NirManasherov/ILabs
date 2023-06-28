#include <stdio.h>
#include "dlist.h" 

#define UNUSED(x) (void)(x) 

static int Test(void);
static int IsMultipleOf4(const void *list_data, void *param);
static int PrintMe(void *list_data, void *param); 

int main(void) 
{
	if (0 == Test())	/*success!*/ 
	{
		puts("Success! all tests passed!");
	}
	else
	{
		puts("Error! some tests didnt pass!");
	}
	
	return 0; 
}

static int Test(void)
{
	dlist_ty *my_dlist = NULL; 
	dlist_ty *my_dlist_copy_find = NULL;
	dlist_iter_ty where;
	dlist_iter_ty where_dynamic;
	dlist_iter_ty where_dynamic_copy_find;
	
	
	/***************************************************************************
	start dummy -> end dummy
	***************************************************************************/
	my_dlist = DlistCreate();
	
	if(0 != DlistSize((const dlist_ty *)my_dlist))
	{
		puts("Error! didnt recognize empty list by Size!");
		return 1;	/*fail*/
	}
	
	if(1 != DlistIsEmpty((const dlist_ty *)my_dlist))
	{
		puts("Error! didnt recognize empty list by IsEmpty!");
		return 1;	/*fail*/
	}
	
	if(1 != DlistIterIsEqual(DlistIterBegin((const dlist_ty *)my_dlist) ,
								DlistIterEnd((const dlist_ty *)my_dlist)))
	{
		puts("Error! dummies configuration is wrong for empty list!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 30 -> end dummy
	***************************************************************************/
	where = DlistPushBack(my_dlist, (const void *)30);
	
	if(30 != (int)DlistIterGetData(where))
	{
		puts("Error! adding with pushBack 30!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 30 -> 40 -> end dummy
	***************************************************************************/
	where_dynamic = DlistPushBack(my_dlist, (const void *)40);
	
	if(40 != (int)DlistIterGetData(where_dynamic))
	{
		puts("Error! adding with pushBack 40!");
		return 1;	/*fail*/
	}
	
	if(30 != (int)DlistIterGetData(DlistIterPrev(where_dynamic)))
	{
		puts("Error! using prev!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 30 -> 40 -> 50 -> end dummy
	***************************************************************************/
	where_dynamic = DlistPushBack(my_dlist, (const void *)50);
	
	if(50 != (int)DlistIterGetData(where_dynamic))
	{
		puts("Error! adding with pushBack 50!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 10 -> 30 -> 40 -> 50 -> end dummy
	***************************************************************************/
	where_dynamic = DlistPushFront(my_dlist, (const void *)10);
	
	if(10 != (int)DlistIterGetData(where_dynamic))
	{
		puts("Error! adding with pushFront 10!");
		return 1;	/*fail*/
	}
	
	if(4 != DlistSize((const dlist_ty *)my_dlist))
	{
		puts("Error! didnt recognize non-empty list by Size!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 0 -> 10 -> 30 -> 40 -> 50 -> end dummy
	***************************************************************************/
	where_dynamic = DlistPushFront(my_dlist, (const void *)0);
	
	if(0 != (int)DlistIterGetData(where_dynamic))
	{
		puts("Error! adding with pushFront 0!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 0 -> 10 -> 20 -> 30 -> 40 -> 50 -> end dummy
	***************************************************************************/
	where_dynamic = DlistInsert(where, (const void *)20);
	
	if(20 != (int)DlistIterGetData(where_dynamic))
	{
		puts("Error! adding with insert 20!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 0 -> 10 -> 101 -> 20 -> 30 -> 40 -> 50 -> end dummy
	***************************************************************************/
	
	where_dynamic = DlistInsert(where_dynamic, (const void *)101);
	
	if(101 != (int)DlistIterGetData(where_dynamic))
	{
		puts("Error! adding with insert 100!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 0 -> 10 -> 20 -> 30 -> 40 -> 50 -> end dummy
	***************************************************************************/
	where_dynamic = DlistRemove(where_dynamic); 
	
	if(20 != (int)DlistIterGetData(where_dynamic))
	{
		puts("Error! adding with remove 100!");
		return 1;	/*fail*/
	}	
	
	if(30 != (int)DlistIterGetData(DlistIterNext(where_dynamic)))
	{
		puts("Error! using next!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 0 -> 10 -> 20 -> 30 -> 40 -> end dummy
	***************************************************************************/
	if(50 != (int)DlistPopBack(my_dlist))
	{
		puts("Error! using PopBack!");
		return 1;	/*fail*/
	}
	
	/***************************************************************************
	start dummy -> 10 -> 20 -> 30 -> 40 -> end dummy
	***************************************************************************/
	if(0 != (int)DlistPopFront(my_dlist))
	{
		puts("Error! using PopFront!");
		return 1;	/*fail*/
	}
	
	where_dynamic = DlistIterBegin((const dlist_ty *)my_dlist);
	where = DlistIterEnd((const dlist_ty *)my_dlist);
	
	if(20 != (int)DlistIterGetData(DlistFind(where_dynamic, where, IsMultipleOf4
																		, NULL)))
	{
		puts("Error! using Find!");
		return 1;	/*fail*/
	}
	
	my_dlist_copy_find = DlistCreate();
	
	/***************************************************************************
	original:  start dummy -> 10 -> 20 -> 30 -> 40 -> end dummy
	copy find: start dummy -> 20 -> 40 -> end dummy
	***************************************************************************/
	if(0 != DlistMultiFind(where_dynamic, where, IsMultipleOf4, NULL, 
															my_dlist_copy_find))
	{
		puts("Error! using MultiFind!");
		return 1;	/*fail*/
	}
	
	where_dynamic_copy_find = DlistIterBegin(my_dlist_copy_find);
	
	if(20 != (int)DlistIterGetData(where_dynamic_copy_find))
	{
		puts("Error! making new list with multiFind 20!");
		return 1;	/*fail*/
	}
	
	where_dynamic_copy_find = DlistIterPrev(DlistIterEnd(my_dlist_copy_find)); 
	
	if(40 != (int)DlistIterGetData(where_dynamic_copy_find))
	{
		puts("Error! making new list with multiFind 40!");
		return 1;	/*fail*/
	}
	
	DlistDestroy(my_dlist);
	DlistDestroy(my_dlist_copy_find);
	
	/***************************************************************************
	start dummy -> end dummy
	***************************************************************************/
	my_dlist = DlistCreate();
	my_dlist_copy_find = DlistCreate();
	
	/***************************************************************************
	start dummy -> 1 -> end dummy
	***************************************************************************/
	where = DlistPushBack(my_dlist, (const void *)1);
	/***************************************************************************
	start dummy -> 1 -> 2 -> end dummy
	***************************************************************************/
	DlistPushBack(my_dlist, (const void *)2);
	/***************************************************************************
	start dummy -> 1 -> 2 -> 3 -> end dummy
	***************************************************************************/
	where_dynamic = DlistPushBack(my_dlist, (const void *)3);
	/***************************************************************************
	start dummy -> 1 -> 2 -> 3 -> end dummy
	***************************************************************************/
	DlistForEach(where, DlistIterEnd((const dlist_ty *)my_dlist), PrintMe, NULL);
	
	/***************************************************************************
	start dummy -> 1 -> 2 -> 3 -> end dummy
	start dummy -> 100 -> end dummy
	***************************************************************************/
	where_dynamic_copy_find = DlistPushBack(my_dlist_copy_find, (const void *)100);
	/***************************************************************************
	start dummy -> 1 -> 2 -> 3 -> end dummy
	start dummy -> 100 -> 200 -> end dummy
	***************************************************************************/
	DlistPushBack(my_dlist_copy_find, (const void *)200);
	/***************************************************************************
	start dummy -> 1 -> 2 -> 3 -> end dummy
	start dummy -> 100 -> 200 -> 300 -> end dummy
	***************************************************************************/
	DlistPushBack(my_dlist_copy_find, (const void *)300);
	/***************************************************************************
	start dummy -> 3 -> end dummy
	start dummy -> 1 -> 2 -> 100 -> 200 -> 300 -> end dummy
	***************************************************************************/
	DlistSplice(where_dynamic_copy_find, where, where_dynamic);
	/***************************************************************************
	start dummy -> end dummy
	start dummy -> 1 -> 2 -> 100 -> 200 -> 300 -> end dummy
	***************************************************************************/
	if(3 != (int)DlistPopFront(my_dlist))
	{
		puts("Error! using splice src list!");
		return 1;	/*fail*/
	}
	/***************************************************************************
	start dummy -> end dummy
	start dummy -> 2 -> 100 -> 200 -> 300 -> end dummy
	***************************************************************************/
	if(1 != (int)DlistPopFront(my_dlist_copy_find))
	{
		puts("Error! using splice src list 1!");
		return 1;	/*fail*/
	}
	/***************************************************************************
	start dummy -> end dummy
	start dummy -> 100 -> 200 -> 300 -> end dummy
	***************************************************************************/
	if(2 != (int)DlistPopFront(my_dlist_copy_find))
	{
		puts("Error! using splice dest list 2!");
		return 1;	/*fail*/
	}
	/***************************************************************************
	start dummy -> end dummy
	start dummy -> 200 -> 300 -> end dummy
	***************************************************************************/
	if(100 != (int)DlistPopFront(my_dlist_copy_find))
	{
		puts("Error! using splice dest list 2!");
		return 1;	/*fail*/
	}
	/***************************************************************************
	start dummy -> end dummy
	start dummy -> 300 -> end dummy
	***************************************************************************/
	if(200 != (int)DlistPopFront(my_dlist_copy_find))
	{
		puts("Error! using splice dest list 2!");
		return 1;	/*fail*/
	}
	/***************************************************************************
	start dummy -> end dummy
	start dummy -> end dummy
	***************************************************************************/
	if(300 != (int)DlistPopFront(my_dlist_copy_find))
	{
		puts("Error! using splice dest list 2!");
		return 1;	/*fail*/
	}
	
	DlistDestroy(my_dlist);
	DlistDestroy(my_dlist_copy_find);
	return 0;	/*Success*/
}

static int IsMultipleOf4(const void *list_data, void *param)
{
	int data = 0; 
	
	UNUSED(param);
	
	data = (int)list_data; 
	
	if(0 == data%4) 
	{
		return 1; /*true*/
	}
	return 0; /*false*/
}

static int PrintMe(void *list_data, void *param)
{
	UNUSED(param);
	
	printf("PrintMe prints: %d\n", ((int)list_data));
	return 0; /*success*/
}
