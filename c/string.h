#include <stddef.h>		/* for fun printf */

/* returns the length of string */
size_t Strlen(const char *s);

/* checks if strings are identical */
int Strcmp(const char *s1, const char *s2);

/* copy a string from source to destination including the terminating null */
char *Strcpy(char *dest, const char *src);

/* copy first n byts of source into destination string */
char *Strncpy(char *dest, const char *src, size_t n);

/* check if strings are identical ignoring cases */
int Strcasecmp(const char *s1, const char *s2);

/* return pointer to the first occurance of a char in a string */
char *Strchr(const char *s, int c);

/* duplicates a string into allocated memory */
char *Strdup(const char *s);

/* concatenate a source string to the end of destination string, overwriting its terminating null */
char *Strcat(char *dest, const char *src);

/* concatenate first n bytes from source to the end of destination string, adding terminating null */
char *Strncat(char *dest, const char *src, size_t n);

/* returns a pointer to the first occurance of a substring */
char *Strstr(const char *haystack, const char *needle);

/*checks if a str is a palindrome*/
int IsPalindrome(const char *str);
