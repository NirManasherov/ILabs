/**********************************************************************/
/**************************libraries***********************************/

#include <stdio.h>

/**********************************************************************/
/*****************************macros***********************************/

#define MAX2(a,b) (a > b) ? a : b
#define MAX3(a,b,c) (a > b) ? (a > c ? a : c) : (b > c ? b : c) 
#define SIZEOF_VAR(x) (char *)(&x+1) - (char *)&x
#define SIZEOF_TYPE(x) (long)((x *)0 + 1)
#include "ws8.h" 

/**********************************************************************/
/*********************functions*declerations***************************/

void macros (void); 

/**********************************************************************/
/**************************functions***********************************/

int main ()
{
	arr_element_ty *arr_ptr = InitArr ();
	PrintAll(arr_ptr);
	AddAll(arr_ptr, 147);
	PrintAll(arr_ptr);
	CleanAll(arr_ptr);
	macros(); 
		 
	return 0;
}

/**********************************************************************/

void macros (void)
{
	int x = -23; 
	int y = 5; 
	int z = 1000;
	long w = 3.14; \
	void *w_p = (void *)&w; 
	int res = 0; 
	res = MAX2(x,y);
	printf("the maximum of %d and %d is :%d\n", x, y, res); 
	res = MAX3(x,y,z); 
	printf("the maximum of %d, %d and %d is :%d\n", x, y, z, res); 
	res = SIZEOF_VAR(x) ;
	printf("the size of %d as int variable is :%d\n", x, res);
	res = SIZEOF_VAR(w) ;
	printf("the size of %d as long variable is :%d\n", x, res);
	printf("the size of %p as pointer variable is :%d\n", w_p, res);\
	res = SIZEOF_TYPE(int); 
	printf("the size of %s variable is :%d\n", "int", res);
}

/**********************************************************************/
