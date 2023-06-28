/*********************************************************************
made by: Nir Manasherov
Date: 29.11.2022
Reviewer: Almog Ifergan
Version: 2
***********************************************************************/
/**************************libraries***********************************/

#include <stdio.h> /*for printf, itoa*/ 
#include <string.h> /*for strcat*/
#include <assert.h> /*for assert*/
#include <stdlib.h> /*for realloc, malloc and free*/
#include "ws8.h" 

/**********************************************************************/
/*****************************macros***********************************/

#define UNUSED(x) (void)(x)
#define MAX_INT_NUMBER_OF_DIGITS 11
#define ARRAY_SIZE 3
#define STR_SIZE 5

/**********************************************************************/
/*********************functions*declerations***************************/

static void PrintInt (void *p);
static void PrintFloat (void *p);
static void PrintStr (void *p);
static void AddInt (void *p, int num);
static void AddFloat (void *p, int num);
static void AddStr (void *p, int num); 
static void CleanInt (void *p);
static void CleanFloat (void *p);
static void CleanStr (void *p);

/**********************************************************************/
/**************************functions***********************************/

int data_int = 4; 
float data_float = 2.2;
char *data_str = NULL;

/**********************************************************************/

/*this function create structs array, initializing it*/
arr_element_ty *InitArr ()
{
	char *ptr_str = "nir1"; 
	arr_element_ty *array = (arr_element_ty *)malloc(ARRAY_SIZE*sizeof(arr_element_ty));
	data_str = (char *)malloc(STR_SIZE*sizeof(char) + 1);
	
	if (NULL == data_str) /*malloc can fail and return NULL pointer*/
	{
		perror("location error");
		return NULL;
	} 
	
	array[0].data = (void *)&data_int;
	array[0].add = AddInt;
	array[0].print = PrintInt;
	array[0].clean = CleanInt;
	
	array[1].data = (void *)&data_float;
	array[1].add = AddFloat;
	array[1].print = PrintFloat;
	array[1].clean = CleanFloat;
	
	array[2].data = (void *)&data_str;
	array[2].add = AddStr;
	array[2].print = PrintStr;
	array[2].clean = CleanStr;
	
	strcpy(data_str, ptr_str); 
	return array;
}

/**********************************************************************/

/*this function prints every struct in the struct array*/
void PrintAll(arr_element_ty array[])
{
	int i = 0; 
	
	for (i = 0 ; i < ARRAY_SIZE ; ++i)
	{
		array[i].print(array[i].data);
	} 
}

/**********************************************************************/

/*this function add an intiger for every struct in the struct array
for int and float the int is added arithmeticly and for string the 
int is added to the ednd of it*/
void AddAll(arr_element_ty *array, int n)
{
	int i = 0; 
	
	for (i = 0 ; i < ARRAY_SIZE ; ++i)
	{
		array[i].add(array[i].data, n);
	} 
}

/**********************************************************************/

/*this function cleans all the structure in the struct array*/
void CleanAll(arr_element_ty *array)
{
	int i = 0; 
	
	for (i = 0 ; i < ARRAY_SIZE ; ++i)
	{
		array[i].clean(array[i].data);
	} 
	free(array);

}

/**********************************************************************/

/*this function prints the struct data of type "int" struct*/
static void PrintInt (void *p) 
{
	int *int_p = (int *)p;
	
	assert(NULL != p);  
	printf ("%d\n", *int_p); 
}

/**********************************************************************/

/*this function prints the struct data of type "float" struct*/
static void PrintFloat (void *p) 
{
	float *float_p = (float *)p;
	
	assert(NULL != p);  
	printf ("%f\n", *float_p); 
}

/**********************************************************************/

/*this function prints the struct data of type "string"(char *) struct*/
static void PrintStr (void *p) 
{
	char *str_p;
	
	assert(NULL != p); 
	str_p = *(char **)p; 
	printf ("%s\n", str_p); 
}

/**********************************************************************/

/*this function add int to the struct's data of type "int" struct*/
static void AddInt (void *p, int num)
{
	int *int_p = (int *)p; 
	
	assert(NULL != p); 
	*int_p = *int_p + num; 
}

/**********************************************************************/

/*this function add int to the struct's data of type "float" struct*/
static void AddFloat (void *p, int num)
{
	float *float_p = (float *)p; 
	
	assert(NULL != p); 
	*float_p = *float_p + (float)num; 
}

/**********************************************************************/

/*this function "cat" int to the struct's data of type "str" struct*/
static void AddStr (void *p, int num)
{
	char *str_p = *(char **)p; 
	char num_str[MAX_INT_NUMBER_OF_DIGITS]; 
	int num_len = 0;
	
	assert(NULL != p); 
	num_len = sprintf(num_str, "%d", num); 
	
	if (0 > num_len) /*sprintf can fail and return negative value*/
	{
		perror("sprintf fail error");
		return; 
	}
	
	str_p = (char *)realloc(str_p, sizeof(char) * (num_len + strlen(str_p) + 1));

	if (NULL == str_p) /*realloc can fail and return NULL pointer*/
	{
		perror("location error");
		return;
	}
	
	strcat (str_p, num_str); 
}

/**********************************************************************/

/*this function clean the struct data of type "int" struct (there is no
need to do nothing)*/
static void CleanInt (void *p)
{
	UNUSED(p); 
}

/**********************************************************************/

/*this function clean the struct data of type "float" struct (there is no
need to do nothing)*/
static void CleanFloat (void *p)
{
	UNUSED(p); 
}

/**********************************************************************/

/*this function clean the struct data of type "str" struct*/
static void CleanStr (void *p)
{
	char *str = *(char **)p; 
	
	free(str);
	p = NULL; 
}

/**********************************************************************/
