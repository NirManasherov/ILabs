/*******************************************************************************
* Author: Nir Manasherov
* Project: 
* Reviewer: 
* Date: 
* Version: 1.0
*******************************************************************************/
/*******************************************************************************
                                  libraries
*******************************************************************************/
#include <stdio.h>		/*  print 		*/
#include <time.h>		/*  rand 		*/
#include <assert.h>		/*  assertion 	*/

#include "knight_tour.h"
#include "bit_arr.h"	

/*******************************************************************************
                                  macros
*******************************************************************************/      

#define TRUE 1
#define FALSE 0
#define NUM_OF_ROWS 8
#define NUM_OF_COLS 8
#define BOARD_SIZE ((NUM_OF_COLS) * (NUM_OF_ROWS))
#define MAX_KNIGHT_MOVES 8
#define END 65

#define GET_IDX(row, col) ((row) * NUM_OF_COLS + (col))
#define GET_ROW(x) ((x) / NUM_OF_COLS)
#define GET_COL(x) ((x) % NUM_OF_COLS)

/*******************************************************************************
                                  enums
*******************************************************************************/

enum {ROW = 0, COL = 1, NUM_OF_idx};

/*******************************************************************************
                             Static vartiables
*******************************************************************************/

/*declare global lut for knight moves*/
int moves_lut[BOARD_SIZE][MAX_KNIGHT_MOVES+1] = {0};

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

/*recursively solve the knight problem*/
static status_ty KnightsTourImp(pos_ty start_pos, pos_ty route_dest[DIM * DIM],
                        time_t time_cap, size_t step_cnt, size_t board_state);
                        
/* returns a boolian value for if index is valid chess - board index */
static int isPosValid(int row, int col); 

/* initialize the lookup table for possible moves */
static void InitMovesLut(); 

/* returns the number of unvisited valid moves */
static int CountMoves(int board_idx, bit_arr_ty failed_route);

/* returns the next index to visit using heuristic algorithm (warnsdorff) */
static int GetNextMove(pos_ty board_idx, bit_arr_ty failed_route); 

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/

status_ty KnightsTour(pos_ty start_pos, pos_ty route_dest[DIM * DIM],
                                                            time_t time_cap)
{
	/*set board_state variable*/
	size_t board_state = 0; 
	
	assert(DIM*DIM > start_pos);
	assert(NULL != route_dest);
	assert(time(NULL) < time_cap);
	
	/*init the LUT*/
	InitMovesLut();
	
	/*call the recursive implimentation func in an appropriate manner*/
	return KnightsTourImp(start_pos, route_dest,
                        time_cap, 0, board_state);
}

/******************************************************************************/

/*recursively solve the knight problem*/
static status_ty KnightsTourImp(pos_ty start_pos, pos_ty route_dest[DIM * DIM],
                        time_t time_cap, size_t step_cnt, size_t board_state)
{
	/* bit array represents the already visited indexes and moves failed */
	bit_arr_ty failed_route = 0;	
	pos_ty next_pos = 0;
	route_dest[start_pos] = step_cnt;
	
	/*if we got bored size - 1 iterations - we have filled the board*/
	if (step_cnt == (BOARD_SIZE - 1))
	{	/* we finish successfuly */
		return SUCCESS;
	}
	
	/*check time overhead*/
	if (time(NULL) > time_cap)
	{	/* time out */
		return FAILURE;
	}
	
	/*mark visited place in bit array "board" and trial of moves*/
	board_state = BitArrSetOn(board_state, start_pos);
	failed_route = board_state;

	/*recursivly solv the board*/
	/*if get next position gives the end of solution indicator stop*/
	while (END != (next_pos = GetNextMove(start_pos, failed_route)))
	{                        
		/*as long as there recuirsively callings are successful return 
		the solution*/
		if (SUCCESS == KnightsTourImp(next_pos, route_dest,
                        time_cap, step_cnt + 1, board_state))
		{
			return SUCCESS;
		}

		/*if the route is lead to deadend (no solution) mark it in the 
		appropriate variable */
		failed_route = BitArrSetOn(failed_route, next_pos);
	}
	
	
	return FAILURE;
}

/******************************************************************************/

/* returns a boolian value for if index is valid chess - board index */
static int isPosValid(int row, int col)
{
	return (row < NUM_OF_ROWS && row >= 0 && col < NUM_OF_COLS && col >= 0);
}

/******************************************************************************/

/* initialize the lookup table for possible moves */
static void InitMovesLut()
{
	/*hold in array the traverse distance of (x,y) manner inside an array (2D)*/
	const int possible_knight_moves[MAX_KNIGHT_MOVES][NUM_OF_idx] = 
		{{-2, -1}, {-2, 1}, {2, -1}, {2, 1},
		{-1, -2}, {-1, 2}, {1, -2}, {1, 2}};

	int row_idx = 0;
	int col_idx = 0;
	int idx = 0;
	int moves_idx = 0;
	int moves_counter = 0;
	
	/*traverse on the entire board*/
	for (idx = 0; idx < BOARD_SIZE; idx++)
	{
		row_idx = GET_ROW(idx);
		col_idx = GET_COL(idx);
		
		/*check which moves are legal knight moves and store the right ones*/
		for (moves_idx = 0, moves_counter = 0;
		moves_idx < MAX_KNIGHT_MOVES; moves_idx++)
		{
			if (isPosValid(row_idx + possible_knight_moves[moves_idx][ROW],
			col_idx + possible_knight_moves[moves_idx][COL]))
			{	
				/* stores the valid moves in the table */
				moves_lut[idx][moves_counter] =
					GET_IDX(row_idx + possible_knight_moves[moves_idx][ROW],
					col_idx + possible_knight_moves[moves_idx][COL]);
				moves_counter++;
			}
		}
		/*make an end stub with unique value for determine filled board*/
		moves_lut[idx][moves_counter] = END;
	}
}

/******************************************************************************/

/* returns the number of unvisited valid moves */
static int CountMoves(int board_idx, bit_arr_ty failed_route)
{
	int next_idx = 0;
	int moves_idx = 0;
	int counter = 0;
	
	while (END != (next_idx = moves_lut[board_idx][moves_idx]))
	{
		if (0 == BitArrGetVal(failed_route, next_idx))
		{
			++counter;
		}
		moves_idx++;
	}

	return counter;
}

/******************************************************************************/

/* returns the next index to visit using heuristic algorithm (warnsdorff) */
static int GetNextMove(pos_ty board_idx, bit_arr_ty failed_route)
{
	pos_ty next_idx = 0;
	pos_ty moves_idx = 0;
	pos_ty min_idx = END;
	pos_ty min_moves = MAX_KNIGHT_MOVES + 1;

	while (END != (next_idx = moves_lut[board_idx][moves_idx]))
	{
		if (0 == BitArrGetVal(failed_route, next_idx))
		{
			int current_moves = CountMoves(next_idx, failed_route);
			if (current_moves < min_moves)
			{
				min_idx = next_idx;
				min_moves = current_moves;
			}
		}
		moves_idx++;
	}

	return min_idx;
}

/******************************************************************************/
