/**********************************************************************/
/**************************libraries***********************************/

#include <assert.h> /*assert*/
#include <stddef.h> /*size_t*/
#include <stdio.h> /*puts*/
#include <string.h> /*memcpy*/
#include "ws9.h"

/**********************************************************************/
/******************************macros**********************************/

#define TEST_SIZE 25

/**********************************************************************/
/*********************functions*declerations***************************/

static void Test(void);
static void InitArrMemcpy(char *arr);
static void InitArrMemmove(char *arr);
static void InitArrMemset(char *arr);
static void CmpArr(char *arr1, char *arr2);
static void DoTestMemcpy(char *arr1, char *arr2, char *src, size_t n); 
static void DoTestMemmove(char *arr1, char *arr2, int i, int j, size_t n);
static void DoTestMemset(char *arr1, char *arr2, int c, size_t n);

/**********************************************************************/
/**************************functions***********************************/

int main(void)
{
	Test();
	
	return 0;
}

/**********************************************************************/

static void Test(void)
{
	static char arr1[TEST_SIZE] = {0};
	static char arr2[TEST_SIZE] = {0};
	static char src[] = "cRaZy!?$# I was crazy once - they put me in a white room full of rats!\
	RaTs?! I hate rAtS! they make me crazy!!!";
	
	/*memcpy*/
	DoTestMemcpy(arr1, arr2, src, 24); /* aligned */
	DoTestMemcpy(arr1, arr2, src, 16); /* aligned */
	DoTestMemcpy(arr1+1, arr2+1, src+1, 16); /* not aligned */
	DoTestMemcpy(arr1, arr2, src, 6); /* n < 8 */
	/*end memcpy*/
	/*memmove*/
	DoTestMemmove(arr1, arr2, 0, 8, 15); /* Memcpy call */
	DoTestMemmove(arr1, arr2, 8, 0, 16); /* aligned, n % 8 == 0 */
	DoTestMemmove(arr1, arr2, 8, 0, 14); /* aligned, n % 8 != 0 */
	DoTestMemmove(arr1, arr2, 8, 0, 6); /* n < 8 */	
	/*end memmove*/
	/*memset*/
	DoTestMemset(arr1, arr2, '*', 16); /* aligned, n % 8 == 0 */
	DoTestMemset(arr1, arr2, '*', 14); /* aligned, n % 8 != 0 */
	DoTestMemset(arr1 + 1, arr2 + 1, '*', 16); /* not aligned */
	DoTestMemset(arr1, arr2, '*', 6); /* n < 8 */
	/*end memset*/
}

/**********************************************************************/

static void DoTestMemcpy(char *arr1, char *arr2, char *src, size_t n)
{
	InitArrMemcpy(arr1);
	InitArrMemcpy(arr2);
	Memcpy(arr1, src, n);
	memcpy(arr2, src, n);
	CmpArr(arr1, arr2);
}

/**********************************************************************/

static void DoTestMemmove(char *arr1, char *arr2, int i, int j, size_t n)
{
	InitArrMemmove(arr1);
	InitArrMemmove(arr2);
	Memmove(arr1 + i, arr1 + j, n);
	memmove(arr2 + i, arr2 + j, n);
	CmpArr(arr1, arr2);
}

/**********************************************************************/

static void DoTestMemset(char *arr1, char *arr2, int c, size_t n)
{
	InitArrMemset(arr1);
	InitArrMemset(arr2);
	Memset(arr1, c, n);
	memset(arr2, c, n);
	CmpArr(arr1, arr2);
}

/**********************************************************************/

static void CmpArr(char *arr1, char *arr2)
{
	int i = 0;
	
	for (; i < TEST_SIZE; i++)
	{
		assert(arr1[i] == arr2[i]);		
	}
}

/**********************************************************************/

static void InitArrMemcpy(char *arr)
{
	int i = 0;
	
	for (i = 0 ; i < TEST_SIZE ; i++)
	{
		arr[i] = 0;
	}
}

/**********************************************************************/

static void InitArrMemmove(char *arr)
{
	int i = 0;
	
	for (; i < TEST_SIZE; i++)
	{
		arr[i] = 0;
	}
	strncpy(arr, "this5432543Is65*&^3Inpu543^%$52s#%$#For^%Memmove", TEST_SIZE);
	arr[TEST_SIZE - 1] = '\0';
}

/**********************************************************************/

static void InitArrMemset(char *arr)
{
	int i = 0;
	
	for (; i < TEST_SIZE; i++)
	{
		arr[i] = 0;
	}
	
	strcpy(arr, "STringForMemsett$%#%!1321");
}

/**********************************************************************/
