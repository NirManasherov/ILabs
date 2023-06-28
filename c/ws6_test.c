#include <stdio.h>	/*for printf*/
#include <math.h>	/*for pow*/
#include "ws6.h"
/**************************decleration*********************************/
void TestPow2();
void TestIsPowTwo();
void TestIsPowTwoLoop();
void TestAddOne();
void TestOneCount();
void TestByteMirrorLoop();
void TestByteMirror(); 
void TestIsSixTwoOneLeastOn();
void TestIsSixAndTwoOn();
void TestSwapFiveThree();
void TestSixteenDevidedClosest();
void TestSwap();
void TestCountOnBitLoop();
void TestCountOnBit();

/**********************************************************************/

/**************************functions***********************************/
int main(void)
{
	TestPow2(); 
	TestIsPowTwo();
	TestIsPowTwoLoop();
	TestAddOne();
	TestOneCount();
	TestByteMirrorLoop();
	TestByteMirror(); 
	TestIsSixTwoOneLeastOn();
	TestIsSixAndTwoOn();
	TestSwapFiveThree();
	TestSixteenDevidedClosest();
	TestSwap();
	TestCountOnBitLoop(); 
	TestCountOnBit();
	PrintFloat (2.72);
	return 0; 
}
/**********************************************************************/

/*1 = true ; 0 = false*/ 
/**********************************************************************/

void IsSuccessful(int booli, int num_of_test, int num_iteration,const char *test_name) 
{
	if (0 == booli) 
	{
		printf ("error! test number %d failed! in test of %s \n" , num_iteration , test_name); 
	}
	
	if ((num_of_test - 1 == num_iteration) && (1 == booli)) 
	{
		printf ("Success! all test for %s test passed!\n" , test_name); 	
	}
}
/**********************************************************************/

void TestPow2()
{
	int x[] = {0,1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int y[] = {0,1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int i = 0; 
	long real_result = 0;
	int size_arr = (sizeof (x) ) / ( sizeof (x[0]) ) ; 
	 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		real_result = x[i] * (pow(2, y[i]));
		IsSuccessful ((long)pow2(x[i], y[i]) == real_result , size_arr , i , "pow 2") ; 
	}
}
/**********************************************************************/

void TestIsPowTwo()
{
	int num_array[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 9, 7, 71, 1111};
	int is_pow2[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}; 
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (IsPowTwo(num_array[i]) == is_pow2[i] , size_arr , i , "is pow 2") ; 
	}
}
/**********************************************************************/

void TestIsPowTwoLoop()
{
	int num_array[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 9, 7, 71, 1111};
	int is_pow2[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}; 
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (IsPowTwoLoop(num_array[i]) == is_pow2[i] , size_arr , i , "is pow 2 loop") ; 
	}
}
/**********************************************************************/

void TestAddOne()
{
	int num_array[] = {0,1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int num_plus_one[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11};
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (AddOne(num_array[i]) == num_plus_one[i] , size_arr , i , "add one") ; 
	}
}
/**********************************************************************/

void TestOneCount()
{
	int num_array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int how_many_ones[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2};
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (OneCount(num_array[i]) == how_many_ones[i] , size_arr , i , "add one") ; 
	}
}
/**********************************************************************/

void TestByteMirrorLoop()
{
	unsigned char num_array[] = {101, 68, 126, 70, 71, 53, 36, 4, 0};
	unsigned char mirror[] =    {-90, 34, 126, 98, -30, -84, 36, 32, 0};
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (ByteMirrorLoop(num_array[i]) == mirror[i] , size_arr , i , "byte mirror loop ") ; 
	}
}
/**********************************************************************/

void TestByteMirror()
{
	unsigned char num_array[] = {101, 68, 126, 70, 71, 53, 36, 4, 0};
	unsigned char mirror[] =    {-90, 34, 126, 98, -30, -84, 36, 32, 0};
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (ByteMirror(num_array[i]) == mirror[i] , size_arr , i , "byte mirror loop ") ; 
	}
}
/**********************************************************************/

void TestIsSixTwoOneLeastOn()
{
	unsigned char num_array[] = {126, 3, 17, 47, 58, 0};
	unsigned char SixTwoOnTruth[] =    {1, 1, 0, 1, 1, 0 };
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (IsSixTwoOneLeastOn(num_array[i]) == SixTwoOnTruth[i] , size_arr , i , "six two at least one on ") ; 
	}
}
/**********************************************************************/

void TestIsSixAndTwoOn()
{
	unsigned char num_array[] = {126, 3, 17, 47, 58, 0};
	unsigned char SixTwoOnTruth[] =    {1, 0, 0, 1, 1, 0 };
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (IsSixAndTwoOn(num_array[i]) == SixTwoOnTruth[i] , size_arr , i , "six two are on ") ; 
	}
}
/**********************************************************************/

void TestSwapFiveThree()
{
	unsigned char num_array[] = {126, 3 , 17, 47, 58, 0};
	unsigned char SixTwoOnSwapped[] = {126, 3, 5, 59, 46, 0 };
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (SwapFiveThree(num_array[i]) == SixTwoOnSwapped[i] , size_arr , i , "swap five three ") ; 
	}
}
/**********************************************************************/

void TestSixteenDevidedClosest()
{
	unsigned char num_array[] = {17, 36, 45, 53, 69};
	unsigned char Sixteen_closest[] = {16, 32, 32, 48, 64};
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (SixteenDevidedClosest(num_array[i]) == Sixteen_closest[i] , size_arr , i , "closest 16 multiplication") ; 
	}
}
/**********************************************************************/

void TestSwap()
{
	int num1_array[] = {17, 36, 45, 53, 69};
	int num2_array[] = {123, 765, 0, 2, 954};
	int before_swap1[] = {17, 36, 45, 53, 69};
	int before_swap2[] = {123, 765, 0, 2, 954};
	int i = 0; 	
	int size_arr = (sizeof (num1_array) ) / ( sizeof (num1_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		Swap(&num1_array[i], &num2_array[i]);
		IsSuccessful ( (num1_array[i]== before_swap2[i]) && (num2_array[i]== before_swap1[i]) , size_arr , i , "swap ints") ; 
	}
}
/**********************************************************************/

void TestCountOnBitLoop()
{
	unsigned char num_array[] = {126, 3 , 17, 47, 58, 0};
	unsigned char OnCount[] = {6, 2, 2, 5, 4, 0 };
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (CountOnBitLoop(num_array[i]) == OnCount[i] , size_arr , i , "count bit loop ") ; 
	}
}
/**********************************************************************/

void TestCountOnBit()
{
	unsigned char num_array[] = {126, 3 , 17, 47, 58, 0};
	unsigned char OnCount[] = {6, 2, 2, 5, 4, 0 };
	int i = 0; 	
	int size_arr = (sizeof (num_array) ) / ( sizeof (num_array[0])) ; 
	
	for (i = 0 ; i < size_arr ; ++i)
	{
		IsSuccessful (CountOnBit(num_array[i]) == OnCount[i] , size_arr , i , "count bit loop ") ; 
	}
}
/**********************************************************************/


