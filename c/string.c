/* implementation of functions from string.h library */

#include <ctype.h>		/*for caps letters manipulations : tolower, toupper */
#include <stdio.h>		/* for fun printf */
#include <stdlib.h>		/*for EXIT_SUCCESS */
#include <string.h>		/* for str functions : strlen*/
#include <assert.h>		/* for assert */
#include "string.h"		/* for str functions */


/* returns the length of string */
size_t Strlen(const char *s)
{
	const char *s_ptr = s;

	assert(s);
	
	while ('\0' != *s)		
	{
		++s;
	}
	
	return (s - s_ptr);
}

/* checks if strings are identical */
int Strcmp(const char *s1, const char *s2)
{
	assert(s1 && s2);
	
	while (*s1 == *s2)
	{
		if ('\0' == *s1)
		{
			return 0;
		}
		
		++s1;
		++s2;
	}
	
	return (*s1 - *s2);	
}

/* copy a string from source to destination including the terminating null */
char *Strcpy(char *dest, const char *src)
{	
	char *dest_ptr = dest;	/* stores ptr to start of dest */

	assert(dest && src);

	while ('\0' != *src)	/* checks if we reach the null terminator */
	{
		*dest = *src;
		++dest;
		++src;
	}
	
	*dest = '\0';
	
	return dest_ptr;
}

/* copy first n byts of source into destination string */
char *Strncpy(char *dest, const char *src, size_t n)
{
	char *dest_ptr = dest;
	
	assert(dest && src);
	
	while (('\0' != *src) && n)		/* check if we reach null terminator or n */
	{
		*dest = *src;
		++dest;
		++src;
		--n;
	}
	
	while (n)
	{
		*dest = '\0';
		++dest;
		--n;
	}
	
	return dest_ptr;
}

/* check if strings are identical ignoring cases */
int Strcasecmp(const char *s1, const char *s2)
{
	assert(s1 && s2);
	
	while (tolower(*s1) == tolower(*s2))
	{
		if ('\0' == *s1)
		{
			return 0;
		}
		
		++s1;
		++s2;
	}
	
	return (int)(tolower(*s1) - tolower(*s2));	
}

/* return pointer to the first occurance of a char in a string */
char *Strchr(const char *s, int c)
{
	assert(s);
	
	if ('\0' == c)
	{
		return '\0';
	}
	
	while (*s != c)
	{
		if ('\0' == *s)
		{
			return NULL;
		}
		++s;
	}
	
	return (char*)s;
}

/* duplicates a string into allocated memory */
char *Strdup(const char *s)
{
	size_t len_s = Strlen(s) + 1;
	char *dest = (char *)malloc(len_s * sizeof(*s));
	
	assert(s);
	
	if (NULL == dest)
	{
		return NULL;
	}
	
	return Strcpy(dest, s);
}

/* concatenate a source string to the end of destination string, overwriting its terminating null */
char *Strcat(char *dest, const char *src)
{
	size_t len_dest = Strlen(dest);
	
	assert(dest && src);
	
	return	(Strcpy((dest + len_dest), src) - len_dest);
}

/* concatenate first n bytes from source to the end of destination string, adding terminating null */
char *Strncat(char *dest, const char *src, size_t n)
{
	size_t len_dest = Strlen(dest);
	
	char *cat_result = Strncpy((dest + len_dest), src, n);
	
	assert(dest && src);
	
	*(cat_result + len_dest + n) = '\0';
	
	return (cat_result - len_dest);
}

/* returns a pointer to the first occurance of a substring */
char *Strstr(const char *haystack, const char *needle)
{
	const char *og_needle = needle;
	
	assert(haystack && needle);
	
	while ('\0' != *haystack)
	{
		while (*haystack == *needle)
		{
			if ('\0' == *(needle + 1))
			{
				return (char *)(haystack - (needle - og_needle));
			}
			
			++haystack;
			++needle;
		}
		
		haystack = haystack - (needle - og_needle) + 1;
		needle = og_needle;
	}
	return NULL;
}

int IsPalindrome(const char *str)
{
	size_t str_len = strlen(str);
	const char *end = str + str_len - 1;
	
	assert(str);
	
	while (end > str)
	{
		if (*end != *str)
		{
			return 0;
		}
		
		--end;
		++str;
	}
	
	return 1;
}

/* 	pseudo code
	Strlen-		1. the function recieves a pointer to the first char of the string.
				2. we set a counter to 0.
				3. while the next char in the string is not '\0' we add 1 to counter.
				4. we return the counter.

	Strcmp-		1. we compare byte-by-byte the two strings
				2. if one byte is different, we return the differnce between s1 and s2 in that byte
				3. if we meet the terminating null on both strings, we return 0.

	Strcpy-		1. we copy byte-by-byte from source to destination.
				2. if the byte we're looking at in source is null, we return pointer to the first byte of dest.
	
	Strncpy-	1. we copy byte-by-byte from source to dest
				2. if source has less than n bytes, we complete the rest to be NULL in dest.
	
	Strcasecmp-	1. we check byte-by-byte the two strings
				2. if one byte contains an uppercase, we convert it to lowercase
				3. if the bytes are different we return *s1 - *s2
				4. if we meet the terminating null on both strings, we return 0.
				
	Strchr-		1. we move byte-by-byte and return if we find c
				2. if we reach null, we return null

	Strdup-		1. we check the len of s
				2. we allocate an array sized len(s) + 1
				3. we Strcpy s into the allocated array
				4. we return the allocated array.
			
	Strcat-		1. we move our pointer dest to point on the terminating null of dest.
				2. we call Strcpy with (dest + len(dest)) and src
				3. we return original pointer to dest.

	Strncat-	1. we save the len of dest
				2. we call Strncpy with (dest + len(dest), src, n)
				3. we return the returned value from Strncpy
				
	Strstr-		1. our stopping condition is if we reach null terminator on haystack.
				2. we check if the needle pointer and haystack pointer points to the same char-
					if so- we advance both pointers by 1.
					if not- we return needle to point to its start and we advance original haystack by 1
				3. we return 1 if we found the substring or NULL if we didnt.

palindrom 
	1. we check the len of the string
	2. we set a pointer to the last char of the string
	3. we check if pointer to the start and end of the string points to same char
	4. we proceed towards the center of the string from both sides
	5. we return 1 for the first unmatch we find
	6. if the pointers overpass themselves, we return 0
	

*/
