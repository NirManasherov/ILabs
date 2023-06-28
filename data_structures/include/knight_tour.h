/*******************************************************************************
 * Project:     Knight's Tour
 * Author:      HRD28
 * Version:     1.0 - 20/02/2023 
 *              1.1 - 20/02/2023 - Add include <stddef.h>
*******************************************************************************/
#ifndef __KNIGHTS_TOUR_H__
#define __KNIGHTS_TOUR_H__

#include <stddef.h> /* size_t */

#define DIM 8

typedef unsigned char pos_ty;

typedef enum status {SUCCESS = 0, FAILURE = 1} status_ty;

/*******************************************************************************
 * Finds the path of a knight's tour on a chess board, starting from "start_pos"
 * so that each position is traveled only once, and stores it in "route_dest"
 * returns SUCCESS if a possible path was found, FAILURE otherwise.
*******************************************************************************/
status_ty KnightsTour(pos_ty start_pos, pos_ty route_dest[DIM * DIM],
                                                            time_t time_cap);

/************************** in source file ************************************/
#include "bit_arr.h"

/*status_ty KnightsTourImp(pos_ty start_pos, pos_ty route_dest[DIM * DIM],
                        time_t time_cap, size_t step_cnt, size_t board_state);*/
/******************************************************************************/

#endif  /*  __KNIGHTS_TOUR_H__  */
