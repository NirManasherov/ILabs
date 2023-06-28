/******************************************************
Name: Nir Manasherov
Date of Publish: 6/11/2022
Last Change: 8/11/20022
Reviewer: Almog Ifergan
Version: 3
*******************************************************/
#include <stdio.h> /*for printf func*/
#include <math.h> /*for pow function*/

/* past " "Hello world" " using hex ASCII*/
int helloWorld ();
/*calculate the resault of 10^n*/
double TenPow (int n);
/* flip a given number */ 
int NumFlip(int num);
/* swap two int pointers */ 
void PointerSwap(int *a, int *b);

/*tests*/
void TestTenPow ();
void TestNumFlip();
void TestPointerSwap()

int main ()
{ 
	helloWorld();
	TestTenPow();
	TestNumFlip();
	TestPointerSwap();
	return 0; 
}

/* exercise #3 */
/* past " "Hello world" " using hex ASCII*/
int helloWorld ()
{
	char output[] = "\x22\x48\x65\x6C\x6C\x6F\x20\x57\x6F\x72\x6C\x64\x21\x22\n" ; /* ASCII of " "Hello world!" " */ 
	printf("%s", output); 
	return 0;
}


/* exercise #4 */
/*calculate the resault of 10^n*/
double TenPow (int n)
{
	double res = 1;
	const int base = 10; 
	
	if (0 == n)
	{
		return 1;
	}
	
	if (0 < n)
	{
		res = 1; 
		int i = 0;
		
		for(i = 0; i < n; i++)
		{
			res *= base;
		}
		
		return res;
	}
		
	else
	{
		res = 1;
		int i = 0;
		  
		for (0 = i; i < n; i++)
		{
			res *= 1 / base;
		}
		
		return res;	
	}
	
}
/* test */
void TestTenPow ()
{
	double test_nums[] = {-1,0,22,7,-5,9,-10};
	size_t arr_size = ( sizeof (test_nums) ) / (sizeof test_nums[0]);
	int i=0; 
	const int accuracy = 0.000000001; /* to avoide number value error in saving */
	int counter = 0;
	
	for (i = 0; arr_size > i ; i++)
	{
		int curnum = test_nums[i];
		if (( pow(10, curnum)-TenPow(curnum)) > accuracy ) /* test failed */
		{
			printf("test failed for test number %d \n", test_num[i]);
		}
		
		else /* test passed */
		{
			counter++;
		}
		
		if (counter == arr_size)
		{
			printf("Success! all tests passed\n");
		}
		
		else /*not all tests passed*/ 
		{
			int failes = arr_size - counter;
			printf("%d tests have failed \n", failes);
		}
	}
}

/* exercise #5 */
/* flip a given number */ 
int NumFlip(int num)
{
	int res = 0;
		
	
	while (0 != num) 
	{
		res = res * 10 + (num % 10);
		num = (num - (num % 10)) / 10;
	}
	
	return res;
}

/* test */ 
void TestNumFlip()
{
	int num = -2142100;
	int flipped = NumFlip (num);
	printf("the flipped number of %d is: %d \n",num ,flipped); 
}

/* exercise #6 */
/* swap two int pointers */ 
void PointerSwap(int *a, int *b)
{
	int tmp = *a; 
	*a = *b; 
	*b = tmp; 
}

/* test */ 
void TestPointerSwap()
{
	int a = 7;
	int b = 200;
	 
	printf("before swap; a value is %d and b value is %d\n", a,b); 
	PointerSwap (&a,&b); 
	printf("after swap; a value is %d and b value is %d\n", a,b); 
}
