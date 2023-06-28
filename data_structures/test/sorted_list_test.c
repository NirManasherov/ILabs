#include <stdio.h>
#include "sorted_list.h"

#define UNUSED(x) (void)(x)

/*returns 1 if test failed 0 if succeeded*/
static int Test(void); 
/*the comperason will be by age*/
static int CompFun(void *data1, void *data2);
/*multiple by two bitwise. undefined if most signifficant bit is on(int overflow)*/
static int MultBy2(void *list_data, void *param);
/*checks if a number is a multiple of 3. return 1 = true, 0 = false*/
static int IsMultOf3 (const void *list_data, void *param);

int main(void)
{
	if (0 == Test())
	{
		puts("success! all tests passed!");
		return 0; 
	}
	return 1; 
}

/*returns 1 if test failed 0 if succeeded*/
static int Test(void)
{
	sort_list_ty *my_sorted_list; 
	sort_list_iter_ty my_iter; 
	sort_list_ty *my_sorted_list2; 
	sort_list_iter_ty my_iter2; 
	
	my_sorted_list = SortedListCreate(CompFun); 
	
	if(NULL == my_sorted_list)
	{
		puts("Error! fail to create list!");
	}
	
	if(1 != SortedListIsEmpty((const sort_list_ty *)my_sorted_list)) 
	{
		puts("Error! empty list not detected");
		return 1; 
	}
	
	/***************************************************************************
	startD->endD
		    /\
			|| 
		  my_iter
	***************************************************************************/
	my_iter = SortedListBegin((const sort_list_ty *)my_sorted_list);
	
	/***************************************************************************
	startD->5->endD
			/\
			|| 
		  my_iter
	***************************************************************************/
	my_iter = SortedListInsert(my_sorted_list , (const void *)5);
	
	if(5 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant insert correctly 5!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->1->5->endD
	       /\
		   || 
		  my_iter
	***************************************************************************/
	my_iter = SortedListInsert(my_sorted_list , (const void *)1);
	
	if(1 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant insert correctly 1!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->1->5->8->endD
	              /\
			      || 
		        my_iter
	***************************************************************************/
	my_iter = SortedListInsert(my_sorted_list , (const void *)8);
	
	if(8 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant insert correctly 8!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->1->4->5->8->endD
		      /\
			  || 
		    my_iter
	***************************************************************************/
	my_iter = SortedListInsert(my_sorted_list , (const void *)4);
	
	if(4 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant insert correctly 4!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->1->4->5->8->endD
		    /\
			|| 
		  my_iter
	***************************************************************************/
	my_iter = SortedListBegin((const sort_list_ty *)my_sorted_list);
	
	if(1 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant read correctly 1!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->1->4->5->8->endD
		       /\
               || 
		     my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(4 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant read correctly 4!"); 
		return 1; 
	}	
	
	/***************************************************************************
	startD->1->4->5->8->endD
		          /\
                  || 
		        my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(5 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant read correctly 5!"); 
		return 1; 
	}	
	
	/***************************************************************************
	startD->1->4->5->8->endD
		             /\
                     || 
		           my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(8 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant read correctly 8!"); 
		return 1; 
	}	
	
	/***************************************************************************
	startD->1->4->5->8->endD
		          /\
                  || 
		         my_iter
	***************************************************************************/
	my_iter = SortedListPrev(my_iter); 
	
	if(5 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant read correctly after backup 5!"); 
		return 1; 
	}	
	
	
	/***************************************************************************
	startD->1->4->5->8->endD
		                 /\
                         || 
		               my_iter
	***************************************************************************/
	my_iter = SortedListNext(SortedListNext(my_iter)); 
	
	if(1 != SortedListIterIsEqual(SortedListEnd(my_sorted_list), my_iter))
	{
		puts("Error! cant read correctly endDummy!"); 
		return 1; 
	}	
	
	if(4 != SortedListSize((const sort_list_ty *)my_sorted_list))
	{
		puts("Error! using size!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->1->4->5->8->endD
		             /\
                     || 
		           my_iter
	***************************************************************************/
	my_iter = SortedListPrev(my_iter); 
	
	if(8 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant read correctly after backup 8!"); 
		return 1; 
	}	

	/***************************************************************************
	startD->1->4->5->endD
		              /\
                      || 
		            my_iter
	***************************************************************************/
	my_iter = SortedListRemove(my_iter); 
	
	if(1 != SortedListIterIsEqual(SortedListEnd(my_sorted_list), my_iter))
	{
		puts("Error! cant remove correctly!"); 
		return 1; 
	}	
	
	/***************************************************************************
	startD->1->4->5->10->endD
		             /\
                     || 
		           my_iter
	***************************************************************************/
	my_iter = SortedListInsert(my_sorted_list , (const void *)10);
	if(10 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant insert correctly 10!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->4->5->10->endD
		          /\
                  || 
		        my_iter
	***************************************************************************/
	if(1 != (int)SortedListPopFront(my_sorted_list))
	{
		puts("Error! cant popFront correctly 1!"); 
		return 1;
	}
	
	/***************************************************************************
	startD->4->5->endD     "10"
		                    /\
                            || 
		                   my_iter
	***************************************************************************/	
	if(10 != (int)SortedListPopBack(my_sorted_list))
	{
		puts("Error! cant popBack correctly 10!"); 
		return 1;
	}
	
	/***************************************************************************
	startD->4->5->40->endD
		          /\
                  || 
		        my_iter
	***************************************************************************/	
	my_iter = SortedListInsert(my_sorted_list , (const void *)40);
	
	if(40 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant insert correctly 40!"); 
		return 1; 
	}
	
	if(0 != SortedListForeach(SortedListBegin((const sort_list_ty *)my_sorted_list), 
	my_iter, MultBy2, NULL))
	{
		puts("Error! forEach failed!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->4->5->40->endD
		          /\
                  || 
		        my_iter
		        
		        
	startD->endD
		     /\
             || 
		   my_iter
	***************************************************************************/	
	my_sorted_list2 = SortedListCreate(CompFun); 
	my_iter2 = SortedListBegin(my_sorted_list2);
	
	/***************************************************************************
	startD->4->5->40->endD
		          /\
                  || 
		        my_iter
		        
		        
	startD->100->endD
		     /\
             || 
		   my_iter2
	***************************************************************************/	
	my_iter2 = SortedListInsert(my_sorted_list2 , (const void *)100);
	
	if(100 != (int)SortedListGetData(my_iter2))
	{
		puts("Error! cant insert correctly 100!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->4->5->40->endD
		          /\
                  || 
		        my_iter
		        
		        
	startD->9->100->endD
		    /\
            || 
		  my_iter2
	***************************************************************************/	
	my_iter2 = SortedListInsert(my_sorted_list2 , (const void *)9);
	
	if(9 != (int)SortedListGetData(my_iter2))
	{
		puts("Error! cant insert correctly 9!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->4->5->40->endD
		          /\
                  || 
		        my_iter
		        
		        
	startD->2->9->100->endD
		    /\
            || 
		  my_iter2
	***************************************************************************/	
	my_iter2 = SortedListInsert(my_sorted_list2 , (const void *)2);
	
	if(2 != (int)SortedListGetData(my_iter2))
	{
		puts("Error! cant insert correctly 2!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
		                /\
                        || 
		              my_iter
	***************************************************************************/
	SortedListMerge(my_sorted_list, my_sorted_list2);
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	       /\
           || 
		 my_iter
	***************************************************************************/
	my_iter = SortedListBegin((const sort_list_ty *)my_sorted_list);
	
	if(2 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant identify correctly after 2!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	          /\
              || 
		    my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(4 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant identify correctly after 4!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	             /\
                 || 
		       my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(5 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant identify correctly after 5!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	                /\
                    || 
		          my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(9 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant identify correctly after 9!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	                    /\
                        || 
		             my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(40 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant identify correctly after 40!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	                         /\
                             || 
		                   my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(100 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant identify correctly after 100!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	                              /\
                                  || 
		                        my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(1 != SortedListIterIsEqual(my_iter, SortedListEnd(my_sorted_list)))
	{
		puts("Error! cant identify correctly after endDummy!"); 
		return 1; 
	}
	
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	                    /\
                        || 
		              my_iter
	***************************************************************************/
	my_iter = SortedListPrev(SortedListPrev(my_iter)); 
	
	if(40 != (int)SortedListGetData(my_iter))
	{
		puts("Error! cant identify correctly after preving to 40!"); 
		return 1; 
	}
	
	if(1 != SortedListIterIsEqual(my_iter,SortedListFind(my_sorted_list, 				 
	SortedListBegin(my_sorted_list), SortedListEnd(my_sorted_list), 
	(const void *)40)))
	{
		puts("Error! cant find 40!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	        /\
            || 
		  my_iter
	***************************************************************************/
	my_iter = SortedListBegin(my_sorted_list); 
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	          /\
              || 
		    my_iter
	***************************************************************************/
	my_iter = SortedListNext(my_iter); 
	
	if(1 != SortedListIterIsEqual(my_iter,SortedListFind(my_sorted_list, 				 
	SortedListBegin(my_sorted_list), SortedListEnd(my_sorted_list), 
	(const void *)4)))
	{
		puts("Error! cant find 4!"); 
		return 1; 
	}
	
	/***************************************************************************
	startD->2->4->5->9->40->100->endD
	          /\
              || 
		    my_iter
	***************************************************************************/
	my_iter = SortedListNext(SortedListNext(my_iter)); 
	
	if(1 != SortedListIterIsEqual(my_iter, 
	SortedListFindIf(SortedListBegin(my_sorted_list), 
	SortedListEnd(my_sorted_list), IsMultOf3, NULL)))
	{
		puts("Error! using findIf to find 9(first multiple of 3)!"); 
		return 1; 
	}
	
	SortedListDestroy(my_sorted_list);
	return 0; /*success*/
}

/*the comperason will be by age*/
static int CompFun(void *data1, void *data2)
{
	int int1 = 0; 
	int int2 = 0; 
	
	int1 = (int)data1; 
	int2 = (int)data2;
	
	if(int1 == int2) 
	{
		return 0; /*same age*/
	}
	
	if(int1 > int2) 
	{
		return 1; /*person1 is older then person2*/
	}

	return -1; /*person1 is younger then person2*/
}

/*multiple by two bitwise. undefined if most signifficant bit is on(int overflow)*/
static int MultBy2(void *list_data, void *param)
{
	UNUSED(param); 
	
	list_data = (void *)((int)list_data << 1);
	return 0;
}

/*checks if a number is a multiple of 3. return 1 = true, 0 = false*/
static int IsMultOf3 (const void *list_data, void *param)
{
	int data = (int)list_data; 
	
	UNUSED(param); 
	
	if(0 == data%3)
	{
		return 1; /*match is found!*/
	}
	
	return 0; 
}
