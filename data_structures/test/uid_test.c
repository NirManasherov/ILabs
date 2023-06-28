#include <sys/types.h>		/*	pid_t			*/
#include <time.h>			/*	time_t, size_t	*/
#include <stdio.h>			/*	printf, puts	*/

#include "uid.h"

#define TEST_SIZE 1000

static int Test(void);

int main(void) 
{
	if(0 != Test())
	{
		puts("Error! some tests did not passed!");
		return 1; 
	}
	
	puts("Success! all tests passed!");
	return 0;
}

/*if test passes return 0 else return 1*/
static int Test(void)
{
	ilrd_uid_ty arr[TEST_SIZE] = {0};
	size_t i = 0; 
	size_t j = 0;
	
	/*create pids*/
	for(i = 0 ; i < TEST_SIZE ; ++i) 
	{
		arr[i] = UIDCreate(); 
		
		if(1 == UIDIsSame(UIDBadID ,arr[i]))	/*create failed*/
		{
			printf("Error! create failed for member %ld \n", i); 
			return 1; 
		}
	}
	
	for(j = 0 ; j < TEST_SIZE ; ++j) 
	{
		for(i = 0 ; i < TEST_SIZE ; ++i)
		{
			if(i == j)	/*same member*/
			{
				if(1 != UIDIsSame(arr[i] ,arr[j]))
				{
					printf("Error! couldnt identify same UID in place %ld\n!", i);
					return 1; 
				}
			}
			
			else	/*different member*/
			{
				if(0 != UIDIsSame(arr[i] ,arr[j]))
				{
					printf("Error! couldnt identify different UID in place %ld\n!", i);
					return 1; 
				}
			}
		}
	}
	
	return 0; 
}
