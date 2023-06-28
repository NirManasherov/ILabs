#include <assert.h>	/*assert*/
#include <stdlib.h>	/*srand*/
#include <stdio.h>	/*printf*/

#include "knight_tour.h"

#define NUM_OF_ROWS 8
#define NUM_OF_COLS 8
#define BOARD_SIZE ((NUM_OF_COLS) * (NUM_OF_ROWS))


/*print the chess board based on route dest*/
static void PrintBoard(pos_ty *route_dest); 

int main(void)
{
	static pos_ty route_dest[BOARD_SIZE] = {0};
	time_t time_cap = 0;

	assert(BOARD_SIZE <= 64);

	
	srand(time(NULL));

	time_cap = time(NULL) + 300;
	
	 
	
	if (SUCCESS == KnightsTour(rand() % DIM, route_dest, time_cap))
	{
		PrintBoard(route_dest);
	}
	else
	{
		puts("Knight Tour function did not find any valid solution!");
	}


	return SUCCESS;
}

/******************************************************************************/

/*print the chess board based on route dest*/
static void PrintBoard(pos_ty *route_dest)
{
	size_t idx = 0;

	for (idx = 0; idx < BOARD_SIZE; idx++)
	{
		if (0 == (idx % NUM_OF_COLS))
		{
			putchar('\n');
			putchar('\n');
		}
		printf("%d\t", route_dest[idx]);
	}
	putchar('\n');
}

/******************************************************************************/
