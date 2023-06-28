/***************************************************************************************
made by: Nir Manasherov
Date: 14.11.2022
Reviewer: unknown
Version: 1
**************************************************************************************/
#include <stdio.h>	/*for printf*/
#include <stdlib.h>	/*for malloc*/
#include <ctype.h>	/* for tolower */
#include <string.h>	/* for strlen */
#include <stddef.h>	/*for size_t*/
#include <assert.h>	/* for assert */
/*************************************************************************************/
/*this function calculate the sum of the rows in a given matrix*/
int *ArrayRowSum ( int *arr, int rows, int cols, int *res , int res_size)
{
	int i = 0; /* index for matrix long array */  
	int j = 0; /* index for rows */ 
	int k = 0 ; /* index for res */
	int sum = 0; 
	int *arr_ptr = arr; 
	
	if (res_size != rows) /* not valid imput */
	{
		assert ( (NULL != res) && (NULL != arr) );
		printf ("the length of resault must be equal to the number of rows! the res vector will be returned\n");
		return res;
	}
	
	for (i = 0 ; i < rows*cols ; i++)
	{
		for (j = 0 ; j < cols ; j++)
		{
			sum += *arr_ptr;
			++arr_ptr; 
		}
		res[k] = sum;
		++k;
		sum = 0;  
	}
	return res;
} 
/*************************************************************************************/
/*this function solves josefus soldier problem
num_sols is the number of soldiers in the experiment*/
size_t josephus (size_t num_sols)
{
	size_t i = 0; /*intex for place in array*/
	size_t kill_count = 0; /*killed soldiers counter*/
	size_t *sols = (size_t *)calloc( num_sols, sizeof(size_t) ); /*"array" representing soldiers*/
	size_t *sols_ptr = sols; /*moving pointer*/
	if (sols == NULL) 
	{
		printf("Error! memory couldn't have been alocated properly\n"); 
		return ++num_sols; 
	} 

	
	while (kill_count != num_sols - 1)
	{
		sols_ptr = NextSoldier ( &i, &sols_ptr , num_sols, sols );
		*sols_ptr = 1; /*kill the soldier*/
		++kill_count; 
		sols_ptr = NextSoldier ( &i, &sols_ptr , num_sols, sols );
	}
	
	free(sols);
	sols = NULL;
	sols_ptr = NULL;
	return i; 
}
/**************************************************************************************/
/*this function return the next soldier from a spesiffic location i in an array size num_of_soldiers*/
size_t *NextSoldier (size_t *i, size_t **sols_ptr , size_t num_sols, size_t *sols )
{

	do	
	{
		if (*i == (num_sols -1) )
		{
			*i = 0;
			*sols_ptr = sols;
			continue;
			/**i=-1; 
			*sols_ptr = --sols;*/ 
		}
		
		++*sols_ptr;
		++*i;
	}while (**sols_ptr != 0);
	return *sols_ptr; 
}
/*************************************************************************************/
/* free all allocated memory with two possible cases */
void CleanMalloc(char *arr[], int num)
{
	if (-1 == num)				/* free allocated memory at the end of the main function */
	{
		while (NULL != *arr)
		{
			++arr;
			free(*arr);
			*arr = NULL;
		}
	}
	else
	{
		while (num >= 0)		/* free allocated memory when one failed */
		{
			--num;
			free(*(arr - num));
			*arr = NULL;
		}
	}
}
/*************************************************************************************/
/* duplicates env array and convert it to lowercase */
char **DupEnv(const char **env)
{
	int num_of_strings = 0;
	int count = 0;
	char **env_dup = NULL;
	
	while (NULL != *(env + num_of_strings))
	{
		++num_of_strings;
	}
	
	env_dup = (char **) malloc((num_of_strings + 1) * sizeof(*env_dup));	/* array of ptrs */
	if (NULL == env_dup)
	{
		return NULL;
	}
	
	while (NULL != *env)		/* allocates memories for strings and convert them to lowercase */
	{
		*env_dup = (char *) malloc((strlen(*env) + 1) * sizeof(**env_dup));
		if (NULL == *env_dup)
		{
			CleanMalloc(env_dup, count);
			free(env_dup);
			return NULL;
		}
		
		strcpy(*env_dup, *env);
		ToLower(*env_dup);
		
		++env;
		++env_dup;
		++count;
	}
	*env_dup = NULL;	/* last pointer in array is to NULL */
	
	return (env_dup - count);
}
/*************************************************************************************/
/* prints strings inside the ptr's array */
void PrintEnv(const char **env)
{
	assert(NULL != env);	
	
	while (NULL != *env)
	{
		puts(*env);
		++env;
	}
}	
/*************************************************************************************/
/* convert a full string to lowercase */
void ToLower(char *str)
{
	while ('\0' != *str)
	{
		*str = tolower(*str);
		++str;
	}
}
/*************************************************************************************/
/*this function prints the byte-size of all the c types*/
void DataSize (void) 
{
	printf( "the size of type %s is %ld bytes \n" , "int" , sizeof(int) );
	printf( "the size of type %s is %ld bytes \n" , "char" , sizeof(char) );
	printf( "the size of type %s is %ld bytes \n" , "float" , sizeof(float) );
	printf( "the size of type %s is %ld bytes \n" , "double" , sizeof(double) );
	printf( "the size of type %s is %ld bytes \n" , "short int" , sizeof(short int) );
	printf( "the size of type %s is %ld bytes \n" , "unsigned int" , sizeof(unsigned int) );
	printf( "the size of type %s is %ld bytes \n" , "long int" , sizeof(long int) );
	printf( "the size of type %s is %ld bytes \n" , "unsigned long int" , sizeof(unsigned long int) );
	printf( "the size of type %s is %ld bytes \n" , "signed char" , sizeof(signed char) );
	printf( "the size of type %s is %ld bytes \n" , "unsigned char" , sizeof(unsigned char) );
	printf( "the size of type %s is %ld bytes \n" , "long double" , sizeof(long double) );
	printf( "the size of type %s is %ld bytes \n" , "sized_t" , sizeof(size_t) );
}
/*************************************************************************************/
