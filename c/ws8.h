/***********************************************************************/
/**************************declerations*********************************/

#ifndef __WS8__H_ 
#define __WS8__H_

typedef void (* AddFunc)(void *, int num);
typedef void (* PrintFunc)(void *);
typedef void (* CleanFunc)(void *);

typedef struct arr_var 
{
	void * data; 
	AddFunc add; 
	PrintFunc print; 
	CleanFunc clean; 
} arr_element_ty;

/***********************************************************************/
/*********************functions*declerations***************************/

/*this function create structs array, initializing it*/
arr_element_ty *InitArr ();

/*this function prints every struct in the struct array*/
void PrintAll(arr_element_ty array[]);

/*this function add an intiger for every struct in the struct array
for int and float the int is added arithmeticly and for string the 
int is added to the ednd of it*/
void AddAll(arr_element_ty array[], int n);

/*this function cleans all the structure in the struct array*/
void CleanAll(arr_element_ty array[]); 

/***********************************************************************/

#endif /* __WS8__H_ */
