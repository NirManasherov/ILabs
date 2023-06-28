#include <stdio.h>	/*	puts		*/
#include <stdlib.h>	/*	malloc free	*/
#include <stddef.h>	/*	size_t		*/
#include <string.h>	/*	strncmp		*/

#include "recursion.h"

#define FIB_RANGE 15
#define STACK_CAPACITY 10

static int fib_nums[FIB_RANGE] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 
									377, 610};

typedef enum {TEST_SUCCESS = 0, TEST_FAIL = 1}exit_success;  

static exit_success IterFibTest(void); 
static exit_success RecFibTest(void);
static exit_success RecFlipListTest(void);
static exit_success RecSortStackTest(void);
static exit_success StrlenTest(void); 
static exit_success StrcmpTest(void); 
static exit_success StrcpyTest(void);
static exit_success StrcatTest(void);
static exit_success StrStrTest(void);

int main(void)
{
	if(TEST_FAIL == IterFibTest())
	{
		puts("IterFibTest failed!"); 
		
		return (int)TEST_FAIL; 
	}
	
	if(TEST_FAIL == RecFibTest())
	{
		puts("RecFibTest failed!"); 
		return (int)TEST_FAIL; 
	}
	
	if(TEST_FAIL == RecFlipListTest())
	{
		puts("RecFlipSlist failed!"); 
		return (int)TEST_FAIL; 
	}
	
	if(TEST_FAIL == RecSortStackTest())
	{
		puts("SortStack failed!"); 
		return (int)TEST_FAIL; 
	}
	
	if(TEST_FAIL == StrlenTest())
	{
		puts("strlen failed!"); 
		return (int)TEST_FAIL; 
	}
	
	if(TEST_FAIL == StrcmpTest())
	{
		puts("strcmp failed!"); 
		return (int)TEST_FAIL; 
	}
	
	if(TEST_FAIL == StrcpyTest())
	{
		puts("strcpy failed!"); 
		return (int)TEST_FAIL; 
	}
	
	if(TEST_FAIL == StrcatTest())
	{
		puts("strcat failed!"); 
		return (int)TEST_FAIL; 
	}
	
	if(TEST_FAIL == StrStrTest())
	{
		puts("strStr failed!"); 
		return (int)TEST_FAIL; 
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	puts("all tests passed!"); 
	return (int)TEST_SUCCESS; 
}

static exit_success IterFibTest(void)
{
	int i = 0;
	
	for(i = 0 ; i < FIB_RANGE ; ++i)
	{
		if(IterativelyFibonacci(i + 1) != fib_nums[i]) 
		{
			puts("couldn't find correct fibonacci number in iterative way!");
			return TEST_FAIL; 
		}
	}
	
	return TEST_SUCCESS; 
}

static exit_success RecFibTest(void)
{
	int i = 0;
	
	for(i = 0 ; i < FIB_RANGE ; ++i)
	{
		if(RecursiveFibonacci(i + 1) != fib_nums[i]) 
		{
			puts("couldn't find correct fibonacci number in recursive way!");
			return TEST_FAIL; 
		}
	}
	
	return TEST_SUCCESS; 
}

static exit_success RecFlipListTest(void)
{
	int data1 = 1; 
	int data2 = 2; 
	int data3 = 3; 
	int data4 = 4;	
	
	node_ty node4; 
	node_ty node3; 
	node_ty node2; 
	node_ty node1;
	node_ty *head;

	node4.data = &data4;
	node4.next = NULL;
	
	node3.data = &data3;
	node3.next = &node4;
	
	node2.data = &data2;
	node2.next = &node3;
	
	node1.data = &data1;
	node1.next = &node2;
	
	head = &node1;
	
	head = RecursiveFlipList(head); 
	
	if(4 != *(int *)head->data)
	{
		printf("%d\n", *(int *)head->data); 
		puts("couldn't flip node no. 4!\n"); 
		return TEST_FAIL; 
	}
	
	head = head->next; 
	
	if(3 != *(int *)head->data)
	{
		puts("couldn't flip node no. 3!\n"); 
		return TEST_FAIL; 
	}
	
	head = head->next; 
	
	if(2 != *(int *)head->data)
	{
		puts("couldn't flip node no. 2!\n"); 
		return TEST_FAIL; 
	}
	
	head = head->next; 
	
	if(1 != *(int *)head->data)
	{
		puts("couldn't flip node no. 1!\n"); 
		return TEST_FAIL; 
	}
	
	head = head->next; 
	
	return TEST_SUCCESS; 
	
}

static exit_success RecSortStackTest(void)
{
	stack_ty *stack; 
	size_t size_of_element = sizeof(void *); 
	int i = 0; 
	int data_top = 0; 
	int data_bot = 0;
	int data_arr[STACK_CAPACITY] = {0};  
	
	data_arr[0] = 1;
	
	/* initializing data arr not sorted*/
	for(i = 1 ; i < STACK_CAPACITY ; ++i)
	{
		data_arr[i] = (-2)*(data_arr[i-1]); 
	}
	
	/*initializing stack*/
	stack = StackCreate(STACK_CAPACITY, size_of_element); 
	
	for (i = 0 ; i < STACK_CAPACITY ; ++i)
	{
		StackPush(stack, (const void *)&data_arr[i]);
	}
	
	SortStack(stack); 
	
	for(i = 1 ; i < STACK_CAPACITY ; ++i)
	{
		data_top = *(int *)StackPeek(stack);
		StackPop(stack);
		data_bot = *(int *)StackPeek(stack);
		
		if(data_top > data_bot)
		{
			puts("stack is not sorted!\n");
			return TEST_FAIL; 
		}
	}
	
	StackDestroy(stack); 
	
	return TEST_SUCCESS;
}

static exit_success StrlenTest(void)
{
	size_t res_arr[] = {6, 4, 9, 12, 5, 15, 14, 5, 35}; 

	size_t i = 0; 
	
	char *test_array_basic[] = 
	{
	"aBBa65" ,
	"ABBA" ,
	"nirmanash" ,
	"748201$#@#27" , 
	"nxusw\0loppookm" ,
	"hhhUBHuoB873#@#", 
	"bbbbbbbaaaaaaa", 
	"ba$ab" , 
	"YgdbaisIOCB;DSAO7053270%@#T$#ncsdio"
	};
	
	size_t arr_size = (sizeof(test_array_basic)/sizeof(test_array_basic[0]));
	
	for(i = 0 ; i < arr_size ; ++i) 
	{
		if(res_arr[i] != RecursiveStrlen(test_array_basic[i]))
		{
			printf("strlen failed on test no. : %ld\n", i); 
			return TEST_FAIL; 
		}
	}
	
	return TEST_SUCCESS; 
}

static exit_success StrcmpTest(void)
{
	int i = 0; 
	
	char *string_arr1[] = 
	{
	"aBBa65" ,
	"ABBA" ,
	"nirmanash" ,
	"748201$#@#27" , 
	"nxusw\0loppookm" ,
	"hhHUBHuoB873#@#", 
	"bbbbbbcaaaaaaa", 
	};
	
	char *string_arr2[] = 
	{
	"aBBa67" ,
	"ABAA" ,
	"nirmanash" ,
	"748a01$#@#27" , 
	"nxusw\0AAAApookm" ,
	"hhhUBHuoB873#@#", 
	"bbbbbbbaaaaaaa", 
	};
	
	int res[] = {-1, 1, 0, -1, 0, -1, 1}; 
	
	for(i = 0 ; i < 7 ; ++i)
	{
		if(res[i] != RecursiveStrcmp(string_arr1[i], string_arr2[i]))
		{
			printf("strcmp failed on test no. : %d\n", i); 
			return TEST_FAIL; 
		}
	}
	
	return TEST_SUCCESS; 
}

static exit_success StrcpyTest(void)
{
	char *src = "giokf467#!#^&@#256MFO;DW";
	char dest[24] = {0};
	
	if(0 != RecursiveStrcmp(src, RecursiveStrcpy(dest, src)))
	{
		printf("strcpy failed!"); 
			return TEST_FAIL; 
	} 
	
	return TEST_SUCCESS; 
}

static exit_success StrcatTest(void)
{
	char dest[54] = "Lital was killed by a racoon,";
	char *src = " but she was reserected!"; 
	char *res = "Lital was killed by a racoon, but she was reserected!";
	
	if(0 != RecursiveStrcmp(res, RecursiveStrcat(dest, src)))
	{
		puts("strcat failed!\n");
		return TEST_FAIL; 
	}
	
	return TEST_SUCCESS; 
}

static exit_success StrStrTest(void)
{
	char *haystack = "*^%$GFdfNirIsAwsome__74320y0"; 
	char *needle = "NirIsAwsome"; 
	
	if(0 != strncmp(needle, RecursiveStrstr(haystack, needle), 
													   RecursiveStrlen(needle)))
	{
		puts("strcat failed!\n");
		return TEST_FAIL; 
	}
	
	return TEST_SUCCESS; 
}
