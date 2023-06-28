/*********************************************************************
made by: Nir Manasherov
Date: 22.11.2022
Reviewer: Lital
Version: 3
***********************************************************************/

/**************************libraries***********************************/
#include <stdio.h>	/*for printf*/
#include <math.h>
#include "ws6.h"
/**********************************************************************/
/**************************functions***********************************/

/*this function gets two number and return it's x*2^y*/ 
long pow2(unsigned int x, unsigned int y)
{ 
	return x << y ; 
}
/**********************************************************************/

/*this function checks if a number is a power of 2 with using loops*/ 
int IsPowTwoLoop(unsigned int n) 
{
	int i = 0; 
	int bit_num = sizeof(int) * 8;	/*number of bits in the number*/
	int count = 0;	/*number of 1's in the bit representation*/
	
	if (0 == n) 
	{
		return 0;
	}
	
	for (i = 0 , count = 0; (i < bit_num) && (count < 2) ; ++i) 
	{
		if (1 == (n & 1)) /*true is the most right bit is 1*/
		{
			++count; 
		}  
		
		n >>= 1 ; /*push n 1 time to the right bitwise*/
	}
	return (1 == count);
} 
/**********************************************************************/

/*this function checks if a number is a power of 2 without using loops*/ 
int IsPowTwo(unsigned int n)
{
	if ((0 == (n & (n - 1))) && (0 != n)) 
	{
		return 1; 
	}
	
	return 0; 
} 	
/**********************************************************************/

/*this function gets a number and return it's value + 1*/ 
int AddOne(int n) 
{
	return (-(~n)); 
}
/**********************************************************************/

/*this function count the number of ones in a binary representation of a number*/
int OneCount(int n) 
{
	int count = 0; 
	int mask = 1; 
	
	while (mask != 0) /*scanning the whole number bit by bit*/ 
	{
		if (mask == (n & mask)) /*true in case of the mask one and the number one 
		aline in the same place*/ 
		++count; 
		mask <<= 1; 
	}
	return count; 
}
/**********************************************************************/

/*this function mirror a byte (100 turns to 001) using a loop*/
unsigned char ByteMirrorLoop(unsigned char n) 
{
	int mask = 1; /*=00000001*/
	unsigned char res = 0; 
	int bit_num = 8; /*8 bits in a byte*/
	int i = 0; 
	
	for (i = 0 ; i < bit_num ; ++i)
	{
		res = ((res << 1) | (n & mask)) ; 
		n >>= 1; 
	}
	
	return res;
}
/**********************************************************************/

/*this function mirror a byte (100 turns to 001) whithout using a loop*/
unsigned char ByteMirror(unsigned char n)
{	
	n = ((n & 240) >> 4) | ((n & 15) << 4); /*240 = 11110000 and 15 = 00001111*/ 
	n = ((n & 204) >> 2) | ((n & 51) << 2); /*204 = 11001100 and 51 = 00110011*/
	n = ((n & 170) >> 1) | ((n & 85) << 1); /*170 = 10101010 and 85 = 01010101*/ 
	
	return n; 
}	
/**********************************************************************/

/*this function returntrue if at least one of the 5th and third bits are both on
else it returns false*/
int IsSixAndTwoOn(unsigned char n)
{
	return (((n & 32) >> 5) & ((n & 2) >> 1));  /*32 = 00100000 , 2 = 00000010*/
}
/**********************************************************************/

/*this function returntrue if at least one of the 5th and third bits are on
else it returns false*/
int IsSixTwoOneLeastOn(unsigned char n) 
{
	return (((n & 32) >> 5) | ((n & 2) >> 1));  /*32 = 00100000 , 2 = 00000010*/
}
/**********************************************************************/

/*this function swaps the 5th and third bits in a given unsigned int*/
int SwapFiveThree(unsigned char n)
{
	return (int)((n & (~20)) | (( n & 4 ) << 2) | ((n & 16) >> 2)); /*4 = 00000100 ,
	 16=00010000*/ 
}
/**********************************************************************/

/*this function finds the smaller closest number that is devisable by 16 
to a given unsigned int*/
unsigned int SixteenDevidedClosest(unsigned int n)
{
	return (n & (~15));
}
/**********************************************************************/

/*this function swaps two intigers without using temp variable*/
void Swap(int *x, int *y) 
{
	*x = *x ^ *y; 
	*y = *x ^ *y;
	*x = *x ^ *y;
}
/**********************************************************************/

/*this function calculate how many bits are on in an intiger with using loops*/
int CountOnBitLoop(int n)
{
	int mask = 1;
	int i = 0; 
	int bit_num = sizeof(int) * 8;	/*number of bits in the number*/
	int count = 0;	/*number of 1's in the bit representation*/
	
	for (i = 0 ; i < bit_num ; ++i) 
	{
		if (mask == (n & mask)) /*true is the most right bit is 1*/
		{
			++count; 
		} 
		 
		n >>= 1 ; /*push n 1 time to the right bitwise*/
	}
	
	return count;
}
/**********************************************************************/

/*this function calculate how many bits are on in an intiger without using loops
this method is based on hemming weight algorithm*/
int CountOnBit(int n)
{
	int mask_bit = 1431655765; /*=01010101010101010101010101010101*/
	int mask_tuple = 858993459; /*=00110011001100110011001100110011*/
	int mask_nibble = 252645135; /*=00001111000011110000111100001111*/
	int mask_byte = 16711935; /*=00000000111111110000000011111111*/
	int mask_two_bytes = 65535; /*=00000000000000001111111111111111*/
	int count = 0;
	 
	count = n; 
	count = (count & mask_bit) + ((count >> 1) & mask_bit); 
	count = (count & mask_tuple) + ((count >> 2) & mask_tuple); 
	count = (count & mask_nibble) + ((count >> 4) & mask_nibble);
	count = (count & mask_byte) + ((count >> 8) & mask_byte); 
	count = (count & mask_two_bytes) + ((count >> 16) & mask_two_bytes); 
	return count; 
}
/**********************************************************************/

void PrintFloat (float f)
{
	float *fp = &f; 
	unsigned int *ip = (unsigned int *)fp; 
	unsigned int mask = (unsigned int)pow(2, 31); 
	int i = 0; 
   
	while (0 != mask) 
	{ 
		printf("%d", (*ip & mask) >> (31 - i) ); 
		mask >>= 1; 
		++i; 
	} 
	printf("\n"); 
} 
/**********************************************************************/


