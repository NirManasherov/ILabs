#include <string.h>		/* for str functions : strlen*/
#include <strings.h>		/*for str functions strcasecmp*/
#include <ctype.h>		/*for caps letters manipulations : tolower, toupper */
#include <stdlib.h>		/*for EXIT_SUCCESS */
#include <assert.h>		/*for assert */
#include <stdio.h>		/* for fun printf */
#include "string.h"		/* for str functions */

/*********************************declerations******************************************/

void count (int booli, int tot_test_num);
void ResetCounter (void);

/*******************************global variables****************************************/

int error_counter = 0;
int suc_counter = 0;
int test_num =0;

/***************************************************************************************/

int main()
{
	int i = 0;
	
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
	
	char *test_array_similar[] = 
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
	
	char test_array_copy[9][100] = {0}; 
	
	int truth_palindrom_table[] = {0, 1, 0, 0, 0, 0, 0, 1, 0 };
	
	int chars[] = { 97, 104, 114, 35, 108, 117 , 97, 98, 64 }; /*ascii for chars*/
	
	
	int n[] = {1, 2, 3, 4, 3, 7, 5, 2, 10 };
	
	int res = -1;
			 
	int size_test =(int) (sizeof(test_array_basic))/(sizeof(test_array_basic[0]));
	
	/*for strlen*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strlen(test_array_basic[i]) == strlen(test_array_basic[i]) );
		count (res, size_test); 

	}
	
		ResetCounter();
	
	/*for strcmp*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strcmp(test_array_basic[i], test_array_similar[i]) ) == (strcmp(test_array_basic[i], test_array_similar[i]) ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
	
	/*for strcpy*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strcpy(test_array_copy[i], test_array_similar[i]) ) == (strcpy(test_array_copy[i], test_array_similar[i]) ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
	
	/*for strncpy*/ 

	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strncpy(test_array_copy[i], test_array_similar[i],n[i] ) ) == (strncpy(test_array_copy[i], test_array_similar[i], n[i] ) ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
	
	/*for Strcasecmp*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strcasecmp(test_array_basic[i], test_array_similar[i]) ) == (strcasecmp(test_array_basic[i], test_array_similar[i]) ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
		
	/*for Strchr*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strchr(test_array_basic[i], chars[i]) ) == (Strchr(test_array_basic[i],chars[i]) ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
		
	/*for Strdup*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = Strcmp(test_array_basic[i], Strdup(test_array_basic[i]) ) ;
		if (0 == res)
		{
			res = 1;
		}
		else 
		{
			res =0; 
		}
		count (res, size_test); 

	}
	
		ResetCounter();
		
	/*for Strcat*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strcat(test_array_copy[i], test_array_similar[i]) ) == (strcat(test_array_copy[i],test_array_similar[i]) ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
	
	/*for Strncat*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strncat(test_array_copy[i], test_array_similar[i], n[i]) ) == (strncat(test_array_copy[i] ,test_array_similar[i], n[i]) ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
	
	/*for Strstr*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( Strstr(test_array_basic[i], test_array_similar[i]) ) == (strstr(test_array_basic[i],test_array_similar[i]) ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
		
	/*for IsPalindrome*/ 
	for (i = 0 ; i < size_test ; ++i)
	{
		res = ( IsPalindrome(test_array_basic[i]) ) == ( truth_palindrom_table[i] ) ;
		count (res, size_test); 

	}
	
		ResetCounter();
	
	return 0;
} 

/***************************************************************************************/

/*a function that counts the amount of tests and their resault
booli - a boolian variable that says if the statement is true or false. 0 = false 1=true*/
void count (int booli, int tot_test_num)
{
	
	++test_num; 
		
	if (1 == booli) 
	{
		++suc_counter; 
	}
	if (0 == booli) 
	{ 
		++error_counter;
		printf ("test number %d has failed! \n", test_num);
	}
	
	if (test_num == tot_test_num)
	{
		printf("total successes %d/%d \n", suc_counter, tot_test_num); 
	}
	
	
} 

void ResetCounter (void) 
{
	error_counter = 0;
	suc_counter = 0;
	test_num =0;
}


