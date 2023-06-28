#include <stdlib.h> /*atoi()*/
#include <stdio.h> /*puts*/
#include <string.h>
#include "ws11.h"

static void DoTestAtoi(void);
static int TestAtoi(char *str); 
static void test (); 

int main()
{
	DoTestAtoi();
	test();
	return 0;
}

/*this function compares atoi (library) and atoi from this project 
if the two resaults are the same return 1 else 0*/
static int TestAtoi(char *str) 
{
	int my_atoi = 0;
	int lib_atoi = 0; 
	
	my_atoi = AToI10(str); 
	lib_atoi = atoi(str); 
	
	if(my_atoi != lib_atoi)
	{
		puts("error! atoi not the same!"); 
		return 0; 
	}
	
	else 
	{
		return 1;
	}
}

static void DoTestAtoi(void)
{
	int suc_count = 0; 
	int i = 0;
	char test_array[5][6] = 
	{
	{'1', '2', '3', '4', '5', '\0'},
	{'z', '2', '3', '4', '5', '\0'},
	{'1', '^', '3', '4', '5', '\0'},
	{'1', '-', '3', '4', '5', '\0'},
	{'-', '2', '3', '4', '5', '\0'},
	};
	
	for(i = 0 ; i < 5 ; ++i) 
	{
		TestAtoi(test_array[i]); 
		++suc_count;
	}
	
	if (suc_count == 5) 
	{
		puts("success! all tests passed for atoi function!");
	}	
}

void test ()
{
	
	char arr1[4] = {'a', 'b', '%', 'D'}; 
	char arr2[3] = {'b', '%', 'd'}; 
	char arr3[1] = {'%'}; 
	char arr4[1] = {'b'};
	
	if (0 == strcmp("12345" , IToA10(12345)))
	{
		puts("fail! not all tests passed for itoa function!");
	}
	
	if (0 == strcmp("1587" , IToA10(1587)))
	{
		puts("fail! not all tests passed for itoa function!");
	}
	
	if (0 == strcmp("2222777" , IToA10(2222777)))
	{
		puts("fail! not all tests passed for itoa function!");
	}
	
	if (0 == strcmp(IToAAnyBaseUntil36(8, 2) , "100" ) )
	{
		puts("fail! not all tests passed for itoa any base function!");
	}
	
	if (0 == strcmp(IToAAnyBaseUntil36(27, 3) , "1000" ) )
	{
		puts("fail! not all tests passed for itoa any base function!");
	}
	
	if (0 == strcmp(IToAAnyBaseUntil36(15, 16) , "F" ) )
	{
		puts("fail! not all tests passed for itoa any base function!");
	}
	
	if (AToIBaseUntil36("100", 3) != 9)
	{
		puts("fail! not all tests passed for atoi any base function!");
	}
	
	if (AToIBaseUntil36("f", 16) != 15)
	{
		puts("fail! not all tests passed for atoi any base function!");
	}
	
	if (AToIBaseUntil36("a", 13) != 10)
	{
		puts("fail! not all tests passed for atoi any base function!");
	}
	
	if (*(InFirstTwoExcludeThird (arr1, 4, arr2, 3, arr3, 1)) != *arr4)
	{
		puts("fail! not all tests passed for InFirstTwoExcludeThird any basefunction!");
	}
}
