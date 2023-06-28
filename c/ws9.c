/*********************************************************************
made by: Nir Manasherov
Date: 04.12.2022
Reviewer: Nazmeya faris-shannan
Version: 1
***********************************************************************/
/**************************libraries***********************************/

#include <stdio.h>
#include "ws9.h"

/**********************************************************************/
/***********************macros and typedef*****************************/

#define SIZE_OF_WORD sizeof(size_t)
#define MASK_BYTE_ALL_ON 255 /*0B11111111*/

typedef unsigned char byte_ty;
typedef size_t word_ty;

/**********************************************************************/
/**************************functions***********************************/

/*copy n characters from src memory areato dest memory area
the function return a pointer to dest
in case of overlapping between src and dest the behaviour is undetermained*/
void *Memcpy(void *dest, const void *src, size_t n)
{
	word_ty *word_copy_dest = NULL;
	word_ty *word_copy_src = NULL;
	byte_ty *byte_copy_dest = NULL;
	byte_ty *byte_copy_src = NULL;
	unsigned int until_aligned = 0;
	size_t tail = 0;
	
	byte_copy_dest = (byte_ty *) dest; /* jumps of 1 byte */
	byte_copy_src = (byte_ty *) src;
	
	until_aligned = *(int *) &byte_copy_dest & (SIZE_OF_WORD - 1); 
	
	while ((0 < n) && (0 != until_aligned)) /* setting the start of s until aligned */
	{
		*byte_copy_dest = *byte_copy_src;
		byte_copy_src++;
		byte_copy_dest++;
		
		--until_aligned;
		--n;
	}
	
	tail = n & (SIZE_OF_WORD - 1); /* saving the carry after filling word patterns */
	
	n /= SIZE_OF_WORD;	;/* dividing by word size*/
	
	word_copy_dest = (word_ty *) byte_copy_dest; /* setting a copy of 1 word */
	word_copy_src = (word_ty *) byte_copy_src;
	
	while (0 != n)	/* filling word patterns */
	{
		*word_copy_dest = *word_copy_src;
		word_copy_src++;
		word_copy_dest++;
		n--;
	}
	
	byte_copy_dest = (byte_ty *) word_copy_dest; /* setting back a copy of 1 byte */
	byte_copy_src = (byte_ty *) word_copy_src;
	
	while (0 != tail) /* setting the tail */
	{
		*byte_copy_dest = *byte_copy_src;
		byte_copy_src++;
		byte_copy_dest++;
		tail--;
	}
	
	return dest;
}

/**********************************************************************/

/*copy n characters from src memory areato dest memory area
the function return a pointer to dest
in case of overlapping between src and dest the the content will be 
copied safely*/
void *Memmove(void *dest, const void *src, size_t n)
{
	word_ty *word_copy_dest = NULL;
	word_ty *word_copy_src = NULL;
	byte_ty *byte_copy_dest = NULL;
	byte_ty *byte_copy_src = NULL;
	size_t until_aligned = 0;
	
	int is_overlap = (((size_t) ((word_ty *) dest - (word_ty *) src)) < n); /*checks if there is an overlap*/
	
	if ((dest > src) && is_overlap)	/* in case of overlap we copy backwords */
	{
		byte_copy_dest = ((byte_ty *) dest) + n; /* setting a copy of 1 byte from the last byte */
		byte_copy_src = ((byte_ty *) src) + n;
	
		until_aligned = n & (SIZE_OF_WORD-1);
		
		while (0 != until_aligned) /* moving until alignment */
		{
			--byte_copy_dest;
			--byte_copy_src;
			*byte_copy_dest = *byte_copy_src;
		
			--until_aligned;
		}
		
		word_copy_dest = (word_ty *) byte_copy_dest;	/* setting the copy to word */
		word_copy_src = (word_ty *) byte_copy_src;
		
		n /= SIZE_OF_WORD; /*we move in words*/
		
		while (0 != n)	/* filling word patterns */
		{
			--word_copy_dest;
			--word_copy_src;
			*word_copy_dest = *word_copy_src;
			
			--n;
		}
	}
	
	else
	{
		Memcpy(dest, src, n); /*the two functions are interchangable in case of no overlap*/
	}

	return dest;
}

/**********************************************************************/

/*copies character c to the first n characters of string s*/
void *Memset(void *s, int c, size_t n)
{
	word_ty word_char_selected = 0;
	word_ty *word_copy = NULL;
	byte_ty byte_char_selected = 0;
	byte_ty *byte_copy = NULL;
	unsigned int until_aligned = 0;
	size_t tail = 0;
	size_t i = 0;
	
	byte_char_selected = c & MASK_BYTE_ALL_ON;
	byte_copy = (byte_ty *) s; /* setting a copy of 1 byte */
	
	until_aligned = *(int *) &byte_copy & (SIZE_OF_WORD - 1);
	
	while ((0 != until_aligned) && (0 < n))	/* set until aligned */
	{
		*byte_copy = byte_char_selected;
		++byte_copy;
		
		--until_aligned;
		--n;
	}
	
	tail = n & (SIZE_OF_WORD - 1);	
	
	word_char_selected = byte_char_selected; /* creating the word pattern */
	
	for (i = 8; i < 64; i <<= 1)
	{
		word_char_selected |= word_char_selected << i;
	}
	
	word_copy = (word_ty *) byte_copy; /* setting a copy of 1 word */
	n /= SIZE_OF_WORD;  /*we move in words*/
	
	while (0 != n)	/* filling word patterns */
	{
		*word_copy = word_char_selected;
		word_copy++;
		n--;
	}
	
	byte_copy = (byte_ty *) word_copy; /* setting back a copy of 1 byte */
	
	while (0 != tail) /* setting the tail */
	{
		*byte_copy = byte_char_selected;
		byte_copy++;
		tail--;
	}
	
	return s;
}

/**********************************************************************/
