/*******************************************************************************
Project:    Comperison Linear Sorts
Author:     Nir Manasherov
Version:    1.0 - 23/01/2023
*******************************************************************************/
#ifndef __COMPARISON_LINEAR_SORT_H__
#define __COMPARISON_LINEAR_SORT_H__

#include <stddef.h>	/*	size_t	*/

/*******************************************************************************
	sort an array of ints by arithmetic order using bubble sort method
	note: undefined behaviour if arr=NULL or if size is bigger then the array's
	actual size
	time complexity: O(n^2)
	space complexity: O(1)
	stability: stable
*******************************************************************************/
void BubbleSort(int *arr, size_t size); 

/*******************************************************************************
	sort an array of ints by arithmetic order using Insert sort method
	note: undefined behaviour if arr=NULL or if size doesn't match the 
	array's	actual size
	time complexity: O(n^2)
	space complexity: O(1)
	stability: stable
*******************************************************************************/
void InsertionSort(int *arr, size_t size);

/*******************************************************************************
	sort an array of ints by arithmetic order using selection sort method
	note: undefined behaviour if arr=NULL or if size is bigger then the array's
	actual size
	time complexity: O(n^2)
	space complexity: O(1)
	stability: stable
*******************************************************************************/
void SelectionSort(int *arr, size_t size); 

/*******************************************************************************
	sort an array of ints by arithmetic order using conting sort method
	note: undefined behaviour if arr=NULL or if size is bigger then the array's
	actual size
	time complexity: O(n)
	space complexity: O(n)
	stability: stable
*******************************************************************************/
int CountingSort(int *arr, size_t size);

/*******************************************************************************
	sort an array of ints by arithmetic order using radix sort method
	note: undefined behaviour if arr=NULL or if size is bigger then the array's
	actual size
	time complexity: O(n)
	space complexity: O(n)
	stability: stable
*******************************************************************************/
int RadixSort(int *arr, size_t size); 

#endif /*__COMPARISON_LINEAR_SORT_H__*/
