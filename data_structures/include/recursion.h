/*******************************************************************************
 *  Project:    Recursion
 *  Author:     HRD28
 *  Version:    1.0 - 05/02/2023
*******************************************************************************/
#ifndef __RECURSION_H__
#define __RECURSION_H__

#include <stddef.h> /* size_t       */

#include "stack.h"  /* stack_ty     */

typedef struct node node_ty;

struct node
{
	void *data; 
	node_ty *next; 
};

/*******************************************************************************
 *  returns the "element_index"th element in a Fibonacci series in which the 1st
 *  element is 0
 *  undefined behaviour if element_index is negative
*******************************************************************************/
int IterativelyFibonacci(int element_index);

int RecursiveFibonacci(int element_index);

/*******************************************************************************
 *  flips a singly linked list and returns the new head
 *  undefined behaviour if node is NULL, or if the singly linked list is empty
*******************************************************************************/
node_ty *RecursiveFlipList(node_ty *node);

/*******************************************************************************
 *  sorts a stack of int values
 *  undefined behaviour if the stack is empty
*******************************************************************************/
stack_ty *SortStack(stack_ty *stack);

/*******************************************************************************
 *  returns the length of "str", not including the '\0'
 *  undefined behaviour if s is NULL
*******************************************************************************/
size_t RecursiveStrlen(const char *s);

/*******************************************************************************
 *  compares the two strings s1 and s2
 *  returns an integer indicating the result of the
    comparison, as follows:
        -   0, if the s1 and s2 are equal
        -   <0, if s1 is less than s2
        -   >0, if s1 is greater than s2
 *  undefined behaviour if dest or src are NULL
*******************************************************************************/
int RecursiveStrcmp(const char *s1, const char *s2);

/*******************************************************************************
 *  copies the string pointed to by "src", including the '\0', to the buffer
 *  pointed to by "dest"
 *  the strings may not overlap, and the destination string "dest" must be 
 *  large enough to receive the copy
  *  undefined behaviour if dest or src are NULL
*******************************************************************************/
char *RecursiveStrcpy(char *dest, const char *src);

/*******************************************************************************
 *  appends "src" to "dest", overwriting the '\0' at the end of "dest"
 *  the strings may not overlap, and "dest" must have enough space for the
 *  result
 *  undefined behaviour if dest or src are NULL
*******************************************************************************/
char *RecursiveStrcat(char *dest, const char *src);

/*******************************************************************************
 *  returns the first occurrence of the sub-string "needle" in "haystack", or
 *  NULL if not found
 *  undefined behaviour if haystack or needle are NULL
*******************************************************************************/
char *RecursiveStrstr(const char *haystack, const char *needle);

#endif /* __RECURSION_H__ */
