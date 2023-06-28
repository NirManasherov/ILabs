/*********************************************************************
made by: Nir Manasherov
Date: 21.11.2022
Reviewer: unknown
Version: 1
***********************************************************************/

/**************************libraries***********************************/
#include <stdio.h>		/* for i/o */
#include <stdlib.h>		/* for file funcs */
#include <string.h>		/* for strcmp */
#include "ws5.h" 
#define UNUSED(x) (void)(x)
#define SIZE 256
/**********************************************************************/

typedef enum exit{FAIL_WRITE_FILE=-6, FAIL_GET_TEXT=-5, FAIL_REMOVE_FILE=-4, 
FAIL_RENAME_FILE=-3, FAIL_CLOSE_FILE=-2, FAIL_OPEN_FILE=-1, SUCCESS=0, FAILURE=1} exit_status;

/***************************decleration********************************/
static int IsMatch(const char *cmm_match, const char *cmd);
static int IsPrepend(const char *cmm_match, const char *cmd);
static int IsAppend(const char *cmm_match, const char *cmd);
static int Append(const char *fname, const char *txt);
static int Remove(const char *fname, const char *txt);
static int Count(const char *fname, const char *txt);
static int Prepend(const char *fname, const char *txt);
static int Exit(const char *fname, const char *txt);
/**********************************************************************/

struct cmd
{
	char *cmm_match;
	int (*CmpFunc)(const char *, const char *);
	int (*OptFunc)(const char *, const char *);
};

struct cmd cmd_arr[5] = {
	{"-remove\n", IsMatch, Remove}, 
	{"-count\n", IsMatch, Count}, 
	{"-exit\n", IsMatch, Exit}, 
	{"<", IsPrepend, Prepend}, 
	{NULL, IsAppend, Append}
};
/**************************functions***********************************/

void logger (char *argv[])
{
	int i = 0;
	
	static char buffer[SIZE] = {0};
	
	while (1)
	{
		if (NULL != fgets(buffer, SIZE, stdin))		/* fgets might fail */
		{
			for (i = 0 ; ; ++i)
			{
				if (cmd_arr[i].CmpFunc(cmd_arr[i].cmm_match, buffer))
				{
					cmd_arr[i].OptFunc(argv[1], buffer);
					break;
				}
			}
		}
		else 
		{
			return FAIL_GET;
		}
	}
	
}
/**********************************************************************/

/*this function compared the struct string to input*/
static int IsMatch(const char *cmm_match, const char *cmd)
{
	return (0 == strcmp(cmm_match, cmd));
}
/**********************************************************************/
/*this function checks if the first char is the saved char <*/

static int IsPrepend(const char *cmm_match, const char *cmd)
{
	return (*cmm_match == *cmd);		
}
/**********************************************************************/
/*this function always make append start because its the default option*/

static int IsAppend(const char *cmm_match, const char *cmd)
{
	UNUSED(cmm_match);
	UNUSED(cmd);
	return (1);	/*always return true because this is the default opperation*/
}
/**********************************************************************/
/*this function append the input to the file*/

static int Append(const char *fname, const char *txt)
{
	FILE *file_ptr = fopen(fname, "a");
	if (NULL == file_ptr)
	{
		return FAIL_OPEN_FILE;
	}
	
	if (EOF == fputs(txt, file_ptr))
	{
		return FAIL_WRITE_TEXT; 
	}
	if (EOF == fclose(file_ptr))
	{
		return FAIL_CLOSE_FILE; 
	}
	
	return SUCCESS;
}
/**********************************************************************/
/*this function removes a file*/

static int Remove(const char *fname, const char *txt)
{
	int check = 0;
	UNUSED(txt);
	
	check = remove(fname);
	
	if (-1 == check)
	{
		return FAIL_REMOVE_FILE;
	}
	
	return SUCCESS;
}
/**********************************************************************/
/*this function counts the line number*/

static int Count(const char *fname, const char *txt)
{
	size_t count = 0;
	int curr_char = 0;
	
	FILE *file_ptr = fopen(fname, "r");
	if (NULL == file_ptr)
	{
		return FAIL_OPEN_FILE;
	}
	
	curr_char = fgetc(file_ptr); /*check every char for end of line symbol*/
	if (EOF == curr_char )
	{
		return FAIL_GET_TEXT;
	}
	
	while (EOF != curr_char)
	{
		if ('\n' == curr_char)
		{
			++count;
		}
		curr_char = fgetc(file_ptr);
	}
	
	printf("number of lines in the file: %lu\n", count);
	fclose(file_ptr);
	
	UNUSED(txt);
	return SUCCESS;
}
/**********************************************************************/
/*this function prepend a file*/

static int Prepend(const char *fname, const char *txt)
{
	FILE *tmp_file_ptr = NULL;
	FILE *file_ptr = NULL;
	int ch = 0;
	
	file_ptr = fopen(fname, "a+");
	if (NULL == file_ptr)
	{
		return FAIL_OPEN_FILE;
	}
	
	tmp_file_ptr = fopen("./tmp.txt", "a");	/* opening a tmp file */
	if (NULL == tmp_file_ptr)
	{
		return FAIL_OPEN_FILE;
	}
	
	if (EOF == fputs(txt + 1, tmp_file_ptr)) /* prepending the text */
	{
		return FAIL_WRITE_FILE;
	}
	
	ch = fgetc(file_ptr);
	if (EOF == ch) 
	{
		return FAIL_GET_TEXT;
	}
	
	while (EOF != ch) /* appending the sent file */
	{
		fputc(ch, tmp_file_ptr);
		ch = fgetc(file_ptr);
	}
	
	if (EOF == fclose(file_ptr))
	{
		return FAIL_CLOSE_FILE; 
	}
	
	if (EOF == fclose(tmp_file_ptr))
	{
		return FAIL_CLOSE_FILE; 
	}
	
	ch = rename("tmp.txt", fname);	/* rename overwrites the file */
	if (-1 == ch)
	{
		return FAIL_RENAME_FILE;
	}
	
	return SUCCESS;
}
/**********************************************************************/
/*this function exit the system*/

static int Exit(const char *fname, const char *txt)
{
	UNUSED(fname);
	UNUSED(txt);
	return SUCCESS;
}
/**********************************************************************/
