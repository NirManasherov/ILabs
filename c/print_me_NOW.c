/***************************************************************************************
made by: Nir Manasherov
Date: 14.11.2022
Reviewer: unknown
Version: 1
**************************************************************************************/
#include <stdio.h>
/*************************************************************************************/
/*******************************struct definition*************************************/
typedef struct print_me
{
    int num;
    void (*print_ptr)(int);
} print_me;
/*************************************************************************************/
/*******************************declerations*********************************************/
static void InitPrintMe (print_me array[]);
static void IntroduceYourself (print_me array[]);
static void Print(int num);
/*************************************************************************************/
/*******************************functions*********************************************/
int main(void)
{
    print_me arr_of_primt_me[10];

    InitPrintMe (arr_of_primt_me); 
	IntroduceYourself (arr_of_primt_me);
	
    return 0;
}
/*************************************************************************************/
/*initialize 10 print_me in an array*/
static void InitPrintMe (print_me array[])
{
	int i = 0;
	for (i = 0; i < 10; ++i)
    {
        array[i].num = i;
        array[i].print_ptr = &Print;
    }
}
/*************************************************************************************/
/*this function print the number value of all print_me items in an array*/ 
static void IntroduceYourself (print_me array[])
{
	int i = 0;
	for (i = 0; i < 10; ++i)
    {
        array[i].print_ptr(array[i].num);
    }
}
/*************************************************************************************/
/*inside function of print_me*/
static void Print(int num)
{
    printf("%d\n", num);
}
/*************************************************************************************/
