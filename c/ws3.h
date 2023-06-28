#ifndef _WS3_H_
#define _WS3_H_
#include <stddef.h>	/*for type size_t*/

/* prints strings inside the ptr's array */
void PrintEnv(const char **env);

/*this function calculate the sum of the rows in a given matrix*/
int *ArrayRowSum ( int *arr, int rows, int cols, int *res , int res_size);

/*this function prints the byte-size of all the c types*/
void DataSize (void);

/*this function solves josefus soldier problem
num_sols is the number of soldiers in the experiment*/
size_t josephus (size_t num_sols);

#endif /*_WS3_H_*/
