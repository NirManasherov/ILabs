#include <stdio.h>	/*puts*/

#include "StateMachine.h" 

typedef enum {SUCCCESS = 0, FAIL = 1} exit_success_ty; 
typedef enum {FALSE = 0, TRUE = 1} bool_ty; 

static exit_success_ty Test(void); 

int main(void)
{
	if(SUCCCESS != Test())
	{
		puts("some tests did not passed! check your code!\n"); 
		return FAIL; 
	}
	
	puts("all tests passed!\n"); 
	return SUCCCESS;
}

static exit_success_ty Test(void)
{
	const char *str1 = "010";			/*	accepted		*/
	const char *str2 = "110101001110";	/*	not accepted	*/
	const char *str3 = "010101cv01110";	/*	accepted		*/
	const char *str4 = "0101010V1110";	/*	not accepted	*/
	const char *str5 = "00";			/*	accepted		*/
	const char *str6 = "0";				/*	not accepted	*/
	const char *str7 = "10"; 			/*	not accepted	*/
	const char *str8 = ""; 				/*	not accepted	*/
	const char *str9 = "Nir is aWs0mE"; /*	not accepted	*/
	
	if(TRUE != IsWordAccepted(str1))
	{
		puts("str1 FAIL\n"); 
		return FAIL;
	}
	
	if(FALSE != IsWordAccepted(str2))
	{
		puts("str2 FAIL\n"); 
		return FAIL;
	}
	
	if(TRUE != IsWordAccepted(str3))
	{
		puts("str3 FAIL\n"); 
		return FAIL;
	}
	
	if(FALSE != IsWordAccepted(str4))
	{
		puts("str4 FAIL\n"); 
		return FAIL;
	}
	
	if(TRUE != IsWordAccepted(str5))
	{
		puts("str5 FAIL\n"); 
		return FAIL;
	}
	
	if(FALSE != IsWordAccepted(str6))
	{
		puts("str6 FAIL\n"); 
		return FAIL;
	}
	
	if(FALSE != IsWordAccepted(str7))
	{
		puts("str7 FAIL\n"); 
		return FAIL;
	}
	
	if(FALSE != IsWordAccepted(str8))
	{
		puts("str8 FAIL\n"); 
		return FAIL;
	}
	
	if(FALSE != IsWordAccepted(str9))
	{
		puts("str9 FAIL\n"); 
		return FAIL;
	}
	
	return SUCCCESS;
}
