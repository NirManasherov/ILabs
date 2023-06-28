#include <string.h>		/*	strlen	*/
#include <stdio.h>		/*	puts	*/

#include "calculator.h"

typedef enum {SUCCESS_TEST = 0 , FAIL_TEST = 1} exit_success_ty; 

static exit_success_ty Test(void); 

int main(void)
{
	if(FAIL_TEST == Test())
	{
		puts("ERROR! some tests didn't pass! go check your code!\n"); 
		return  FAIL_TEST; 
	}
	
	puts("all test passed!\n"); 
	return SUCCESS_TEST; 
}

static exit_success_ty Test(void)
{
	char *expression = "31+(1+2)*3+(6/2+7)"; 
	calc_ty *calc = NULL; 
	status_ty cal_status; 
	double resault = 0; 
	double *res = &resault; 
	
	calc = CalcInit(strlen(expression)); 
	
	if(NULL == calc)
	{
		puts("error creating calculator!\n"); 
		return FAIL_TEST;
	}
	
	cal_status = Calculate((const char *)expression, calc, res);
	
	if(50 != *res)
	{
		puts("calculator wasn't successful!"); 
		return FAIL_TEST;
	}
	
	if(SYNTAX_ERR == cal_status)
	{
		puts("syntax error calculating!\n"); 
		return FAIL_TEST;
	}
	
	if(MATH_ERR == cal_status)
	{
		puts("math error calculating!\n"); 
		return FAIL_TEST;
	}
	
	CalcDestory(calc); 
	
	return SUCCESS_TEST; 
}
