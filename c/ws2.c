/********************************************
this is the answer of exercises that are in Pointers-part 1; 
Date: 9.11.22
version: 1
Name of coder: Nir Manasherov
********************************************/
#include <stdio.h> /*for printf()*/ 
#include <stdlib.h> /*for malloc()*/

/*function of exercises*/

/* this function gets 2 ints pointers and swap them*/ 
void IntSwap (int *a, int *b); 
/* this function copy an array */
void CoppyArray (int *source_array , int size_of_array, int *out_array);
/* this function prints the adresses of variables within */
void PrintAdresses();
/* this function swap two size_t variables */
void SizeTSwap (size_t *a, size_t *b);
/* this function swap two size_t pointers variables */
void SizeTPointerSwap (size_t **a, size_t **b); 

/*test functions declerations*/ 

/*test for exercise 1*/ 
void TestIntSwap ();
/* test for CoppyArray */
void TestCoppyArray ();
/*test for SizeTSwap*/ 
void TestSizeTSwap ();
/*test for SizeTSwap*/ 
void TestSizePointerTSwap ();

int main ()
{
	TestIntSwap(); 
	TestCoppyArray();
	PrintAdresses();
	TestSizeTSwap();
	TestSizePointerTSwap();
	return 0; 
}

/* exercise 1*/ 
/* this function gets 2 ints pointers and swap them*/ 
void IntSwap (int *a, int *b)
{
	int temp = *a; 
	*a = *b; 
	*b = temp; 
}

/*test for exercise 1*/ 
void TestIntSwap ()
{
	int num_array1[] = {-1, 1, 10, 15, -20, -100, -200, 5};
	int num_array2[] = {-7, -10, 11, 22, 1453, 15, 400, 6};
	int num_ref2[] = {-1, 1, 10, 15, -20, -100, -200, 5};
	int num_ref1[] = {-7, -10, 11, 22, 1453, 15, 400, 6};
	int  num_of_cells =  ((int)sizeof(num_array1) / ( (int)sizeof(num_array1[0]) ));
	int i = 0; 
	int counter = 0; 
	
	while ( i < num_of_cells )
	{
		IntSwap (&(num_array1[i]), &(num_array2[i])); 
		
		if (num_ref1[i] == num_array1[i] && num_ref2[i] == num_array2[i])
		{
			counter++; 
		}
		
		else
		{
			printf ("error! a = %d and b = %d did not swap correctly! \n",num_array1[i], num_array2[i]); 
		}
		
		if (counter == num_of_cells ) 
		{
			printf ("Sucsess! \n"); 
		}
		i++;
	}
}

/* exercise 2*/ 
/* this function copy an array */
void CoppyArray (int *source_array , int size_of_array, int *out_array) 
{
	int i = 0; 
	for (i = 0 ; i < size_of_array ; i++ ) 
	{
		*out_array = *source_array;
		if ( i != size_of_array -1)  /*not to get out of bounds*/ 
		{
		source_array ++;
		out_array ++; 
		}
		
	}
} 

/* test for CoppyArray */
void TestCoppyArray () 
{
	int i = 0; 
	int source_array[] = {-1, 1, 10, 15, -20, -100, -200, 5};
	int out_array[] = {0, 0, 0, 0, 0, 0, 0, 0};
	int size_of_array = ( (int)sizeof(source_array) / ( (int)sizeof(source_array[0]) ) );
	CoppyArray (source_array , size_of_array, out_array); 
	
	
	for( i=0 ; i<size_of_array ; i++)
	{
		if (source_array[i] != out_array[i])
		{
			printf("the cells in place %d didnt swap correctly! \n", i); 
		}
		if (i == size_of_array -1)
		{
			printf("Sucsess! \n");
		}
	}
}

/* exercise 3*/ 
/* this function prints the adresses of variables within */
void PrintAdresses()
{
	static int s_i = 7;
	int i = 7; 
	int *ptr = &i;
	int *ptr2 = (int *) malloc(sizeof(int));
	
	if (ptr) /*if ptr is not null*/
	{
		int **ptr3 = &ptr; 
		printf( "the adress of ptr3 is %p \n", (void *)&(ptr3));
	}
	
	
	printf( "the adress of s_i is %p \n", (void *)&(s_i)); 
	printf( "the adress of i is %p \n", (void *)&(i)); 
	printf( "the adress of ptr is %p \n", (void *)&(ptr)); 
	printf( "the adress of ptr2 is %p \n", (void *)&(ptr2)); 

	
	free(ptr2); 
	
}

/* exercise 5a*/ 
/* this function swap two size_t variables */
void SizeTSwap (size_t *a, size_t *b)
{
	size_t temp = *a; 
	*a = *b; 
	*b = temp; 
}

/*test for SizeTSwap*/ 
void TestSizeTSwap ()
{
	size_t num_array1[] = {-1, 1, 10, 15, -20, -100, -200, 5};
	size_t num_array2[] = {-7, -10, 11, 22, 1453, 15, 400, 6};
	size_t num_ref2[] = {-1, 1, 10, 15, -20, -100, -200, 5};
	size_t num_ref1[] = {-7, -10, 11, 22, 1453, 15, 400, 6};
	int  num_of_cells =  ((int)sizeof(num_array1) / ( (int)sizeof(num_array1[0]) ));
	int i = 0; 
	int counter = 0; 
	
	while ( i < num_of_cells )
	{
		SizeTSwap (&(num_array1[i]), &(num_array2[i])); 
		
		if (num_ref1[i] == num_array1[i] && num_ref2[i] == num_array2[i])
		{
			counter++; 
		}
		
		else
		{
			printf ("error! a = %d and b = %d did not swap correctly! \n",(int)num_array1[i], (int)num_array2[i]); 
		}
		
		if (counter == num_of_cells ) 
		{
			printf ("Sucsess! \n"); 
		}
		i++;
	}
}


/* exercise 5b*/ 
/* this function swap two size_t pointers variables */
void SizeTPointerSwap (size_t **a, size_t **b)
{
	SizeTSwap((size_t *)a,(size_t *)b);
}

/*test for SizeTSwap*/ 
void TestSizePointerTSwap ()
{
	size_t a = 3; 
	size_t b = 7; 
	size_t *p_a = &a; 
	size_t *p_b = &b;
	size_t *p_a_ref = &b; 
	size_t *p_b_ref = &a;
	
	
	SizeTPointerSwap (&p_a, &p_b); 
		
	if (p_a_ref == p_a && p_b_ref == p_b)
	{
		printf ("Sucsess! \n"); 
	}
		
	else
	{
		printf ("error! a = %p and b = %p did not swap correctly! \n",(void *)p_a, (void *)p_b); 
	}
		
	
}

