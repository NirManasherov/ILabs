#ifndef __WS_11__
#define __WS_11__

/*this function transforn int to string in desimal base*/
char *IToA10(int num);

/*this function transforn str (with 0-9 characters) to int in desimal base
if an error occure it returns the resault up to that point or 0 (if the first one
is not correct*/
int AToI10(char* str);

/*this function transforn int to string in in any base up to 36 base*/
char *IToAAnyBaseUntil36(int num, int base);

/*this function transforn str (with 0-9 a-z characters) to int in any base up to 
36 base. if an error occure it returns the resault up to that point or 0 (if the 
first one is not correct*/
int AToIBaseUntil36(char *str, int base);

/*this function gets three arrays of chars and it returns the characters that
appear in the first two (str1 + str2) but not in the third (str3) the function
returns it as a string*/
char *InFirstTwoExcludeThird(char *arr1, int size_1, char *arr2, int size_2, char *arr3, int size_3);

/*this function checks if the system is little endian if true return 1 else 0*/
int InLittleEndian(void);

#endif /*__WS_11__*/
