#include <assert.h> /*assert()*/
#include <stddef.h> /*size_t*/
#define ALL_BITS_ON	(~(size_t)0)
#define ALL_BITS_OFF (size_t)0
#define MASK_1 (size_t)1
#define SIZE_T_SIZE 64
#define ASCII_0 (int)'0'
#define UNUSED(x) (void)(x)

/*1010101010101010101010101010101010101010101010101010101010101010*/
#define MASK_BIT 0xAAAAAAAAAAAAAAAA 

/*1100110011001100110011001100110011001100110011001100110011001100*/
#define MASK_TUPLE 0xCCCCCCCCCCCCCCCC

/*1111000011110000111100001111000011110000111100001111000011110000*/
#define MASK_NIBBLE 0xF0F0F0F0F0F0F0F0

/*1111111100000000111111110000000011111111000000001111111100000000*/
#define MASK_BYTE 0xFF00FF00FF00FF00

/*1111111111111111000000000000000011111111111111110000000000000000*/
#define MASK_TWO_BYTE 0xFFFF0000FFFF0000

/*1111111111111111111111111111111100000000000000000000000000000000*/
#define MASK_FOUR_BYTE  0xFFFFFFFF00000000
#include "bit_arr.h"

bit_arr_ty BitArrSetAll(bit_arr_ty bit_array)
{
	UNUSED(bit_array);
	return ALL_BITS_ON; 
}

bit_arr_ty BitArrResetAll(bit_arr_ty bit_array)
{
	UNUSED(bit_array);
	return ALL_BITS_OFF; 
}

bit_arr_ty BitArrSetOn(bit_arr_ty bit_array, size_t index)
{
	return (bit_array | (MASK_1<<index)); 
}

bit_arr_ty BitArrSetOff(bit_arr_ty bit_array, size_t index)
{
	return (bit_array & (~(MASK_1<<index)));
}

bit_arr_ty BitArrSetBit(bit_arr_ty bit_array, size_t index, unsigned int bool_val)
{
	return (((size_t)bool_val<<index) | (bit_array & (~(MASK_1<<index))));
}

unsigned int BitArrGetVal(bit_arr_ty bit_array, size_t index)
{
	return (((MASK_1<<index) & bit_array)>>index);
}

bit_arr_ty BitArrFlip(bit_arr_ty bit_array, size_t index)
{
	return (bit_array ^ (MASK_1<<index));
}

bit_arr_ty BitArrMirror(bit_arr_ty bit_array)
{
	bit_array = ((bit_array & MASK_FOUR_BYTE) >> 32) | ((bit_array & (~MASK_FOUR_BYTE)) << 32);
	bit_array = ((bit_array & MASK_TWO_BYTE) >> 16) | ((bit_array & (~MASK_TWO_BYTE)) << 16);
	bit_array = ((bit_array & MASK_BYTE) >> 8) | ((bit_array & (~MASK_BYTE)) << 8);
	bit_array = ((bit_array & MASK_NIBBLE) >> 4) | ((bit_array & (~MASK_NIBBLE)) << 4); 
	bit_array = ((bit_array & MASK_TUPLE) >> 2) | ((bit_array & (~MASK_TUPLE)) << 2); 
	bit_array = ((bit_array & MASK_BIT) >> 1) | ((bit_array & (~MASK_BIT)) << 1); 
	return bit_array;
}

bit_arr_ty BitArrRotateRight(bit_arr_ty bit_array, size_t shifts)
{
	size_t real_shift = shifts %  SIZE_T_SIZE;
	return ((bit_array>>real_shift) | (bit_array<<(SIZE_T_SIZE-real_shift)));
}

bit_arr_ty BitArrRotateLeft(bit_arr_ty bit_array, size_t shifts)
{
	size_t real_shift = shifts %  SIZE_T_SIZE;
	return ((bit_array<<real_shift) | (bit_array>>(SIZE_T_SIZE-real_shift)));
}

size_t BitArrCountOn(bit_arr_ty bit_array)
{
	bit_array = (bit_array & (~MASK_BIT)) + ((bit_array >> 1) & (~MASK_BIT)); 
	bit_array = (bit_array & (~MASK_TUPLE)) + ((bit_array >> 2) & (~MASK_TUPLE)); 
	bit_array = (bit_array & (~MASK_NIBBLE)) + ((bit_array >> 4) & (~MASK_NIBBLE));
	bit_array = (bit_array & (~MASK_BYTE)) + ((bit_array >> 8) & (~MASK_BYTE)); 
	bit_array = (bit_array & (~MASK_TWO_BYTE)) + ((bit_array >> 16) & (~MASK_TWO_BYTE));
	bit_array = (bit_array & (~MASK_FOUR_BYTE)) + ((bit_array >> 32) & (~MASK_FOUR_BYTE));  
	return bit_array;
}

size_t BitArrCountOff(bit_arr_ty bit_array)
{
	return (SIZE_T_SIZE - BitArrCountOn(bit_array));
}

char *BitArrToString(bit_arr_ty bit_array, char *dest_buffer)
{
	int i = 0;
	
	assert(NULL != dest_buffer);  
	dest_buffer[SIZE_T_SIZE] = '\0'; 
	
	for(i = SIZE_T_SIZE - 1 ; i > -1 ; --i)
	{
		dest_buffer[i] = (char)((bit_array & MASK_1) + ASCII_0);
	}
	
	return dest_buffer; 
}
