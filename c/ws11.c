#define INT_MAXIMUM_LENGTH 10
#define ASCII_0 (int)'0'
#define ASCII_9 (int)'9'
#define ASCII_A (int)'a'
#define ASCII_Z (int)'z'
#define IS_LITTLE_ENDIAN (1 == (*(short *) &"\1"))
#define BASE 10
#define ASCII_SIZE 256
#include "ws11.h"
#include <stdio.h>


static int FindNumberOfCharInNum(int num, int base); 

/*this function transforn int to string in desimal base*/
char *IToA10(int num)
{
	int sign = 1; 
	int number_of_chars_in_num = 0; 
	static char res[INT_MAXIMUM_LENGTH + 2] = {'\0'}; /*+2 for null terminator +sign */
	int i = 0; 
	
	if (0 > num) 
	{
		sign = -1; 
		num = -num; 
	}
	
	num = (unsigned int)num;
	number_of_chars_in_num = FindNumberOfCharInNum(num, BASE); 
	
	if (-1 == sign)
	{
		i = number_of_chars_in_num; /*extra place for "-" char*/
	}
	
	if (1 == sign)
	{
		i = number_of_chars_in_num - 1;
	}
	
	for (i = 0 ; i > 0 ; --i) /*copies all of the chars by reverce order*/
	{
		res[i] = ((char)((num%BASE) + ASCII_0));
		num /= BASE; 
	}
	
	--i; 
		
	if (-1 == sign)
	{
		res[i] = '-';
	}
	 return res; 	
}

/*this function finds the number of characters in a given int*/
static int FindNumberOfCharInNum(int num,int base)
{
	int number_of_chars_in_num = 0; 
	 
	while (0 != num) /*finding how much characters are in num*/
	{
		num /= base ;
		++number_of_chars_in_num; 
	}
	
	return number_of_chars_in_num; 
}

/*this function transforn str (with 0-9 characters) to int in desimal base
if an error occure it returns the resault up to that point or 0 (if the first one
is not correct*/
int AToI10(char* str)
{
	int res = 0; 
	int sign = 1; 
	
	if ((*str) == '-') /*only at first place*/
	{
		sign = -1;
		++str;
	}
	
	while ('\0' != *str) 
	{
		if (((int)(*str) <= ASCII_0) || ((int)(*str) >= ASCII_9))
		{
			return (sign * res); 
		}
		
		res *= BASE;
		res += ((int)(*str) - ASCII_0);
		++str; 
	}
	
	return (sign * res); 
}

/*this function transforn int to string in in any base up to 36 base*/
char *IToAAnyBaseUntil36(int num, int base)
{
	int sign = 1; 
	int number_of_chars_in_num = 0; 
	static char res[INT_MAXIMUM_LENGTH + 2] = {'\0'}; /*+2 for null terminator +sign */
	int i = 0; 
	
	if (0 > num) 
	{
		sign = -1; 
		num = -num; 
	}
	
	num = (unsigned int)num;
	number_of_chars_in_num = FindNumberOfCharInNum(num, base); 
	
	if (-1 == sign)
	{
		i = number_of_chars_in_num; /*extra place for "-" char*/
	}
	
	if (1 == sign)
	{
		i = number_of_chars_in_num - 1;
	}
	
	
	for (i = 0 ; i > 0 ; --i)/*copies all of the chars by reverce order*/
	{
		
		if (((num%base) > ASCII_0) && ((num%base) < ASCII_9))
		{
			res[i] = ((char)((num%base) + ASCII_0));
			num /= base; 
		}
		
		if (((num%base) > ASCII_A) && ((num%base) < ASCII_Z))
		{
			res[i] = ((char)((num%base) + ASCII_A));
			num /= base; 
		}
	}
	
	--i; 
		
	if (-1 == sign)
	{
		res[i] = '-';
	}
	 return res; 	
}

/*this function transforn str (with 0-9 a-z characters) to int in any base up to 
36 base. if an error occure it returns the resault up to that point or 0 (if the 
first one is not correct undefined behavior for base < 2 or base > 36*/
int AToIBaseUntil36(char *str, int base)
{
	int res = 0; 
	int sign = 1; 
	if ((*str) == '-') 
	{
		sign = -1; 
		++str;
	}
	
	while ('\0' != *str) 
	{	
		/*not valid*/
		if ((!((((int)(*str) >= ASCII_0) && ((int)(*str) <= ASCII_9))))&&(!((((int)(*str) >= ASCII_A) && ((int)(*str) <= ASCII_Z)))))
		{
			return (sign * res); 
		}
		
		else 
		{
			
			if (((int)(*str) >= ASCII_0) && ((int)(*str) <= ASCII_9))
			{
				res *= base; 
				res += ((int)(*str) - ASCII_0);
				++str;
			}
		
			if (((int)(*str) >= ASCII_A) && ((int)(*str) <= ASCII_Z))
			{
				res *= base; 
				res += ((int)(*str) - ASCII_A + 10);
				++str;
			}	
		}
	}
	
	return (sign * res); 
}

/*this function gets three arrays of chars and it returns the characters that
appear in the first two (str1 + str2) but not in the third (str3) the function
returns it as a string*/
char *InFirstTwoExcludeThird(char *arr1, int size_1, char *arr2, int size_2, char *arr3, int size_3) 
{
	static char res[ASCII_SIZE]; 
	enum status {untouched = 0, in_3 = 1, in_2_not_in_3 = 2, 
										in_1_and_2_not_in_3 = 3};
	int i = 0;
	int i_ascii = 0; 
	char ascii_indicator[ASCII_SIZE] = {untouched};  
	
	i = 0;
	while (i < size_3) /*marking arr3 chars as forbidden*/
	{
		ascii_indicator[(int)arr3[i]] = in_3; 
		++i; 
	}
	
	i = 0; 
	while (i < size_2)
	{
		if (untouched == ascii_indicator[(int)arr2[i]])
		{
			ascii_indicator[(int)arr2[i]] = in_2_not_in_3;
		}
		
		++i;
	}
	
	i = 0; 
	while (i < size_1)
	{
		if (in_2_not_in_3 == ascii_indicator[(int)arr1[i]])
		{
			ascii_indicator[(int)arr1[i]] = in_1_and_2_not_in_3;
		}
		
		++i;
	}
	
	i = 0; 
	while (i < ASCII_SIZE)
	{
		if (ascii_indicator[i] == in_1_and_2_not_in_3)
		{
			res[i_ascii] = (char)i;
			++i_ascii;
		}
		++i; 
	}
	res[i_ascii] = '\0';
	return res; 
}

/*this function checks if the system is little endian if true return 1 else 0*/
int InLittleEndian(void)
{
	int i = 1;
	char c = *(char *) &i;
	
	return ((int)c == 1 ? 1 : 0);
}
