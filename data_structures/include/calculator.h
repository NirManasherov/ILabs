/*******************************************************************************
 * Project:     Calculator
 * Author:      HRD28
 * Version:     1.0 - 01/02/2023
                1.1 - 01/02/2023 : added OperationDummy
                1.2 - 01/02/2023 : unary operators clarification
*******************************************************************************/
#ifndef __CALC_H__
#define __CALC_H__

#include <stddef.h> /*  size_t  */

typedef struct calc calc_ty;

typedef enum status
{
    SUCCESS = 0,
    MATH_ERR = 1,
    SYNTAX_ERR = 2
} status_ty;

/*******************************************************************************
 *  initiates an instance of the calculator and returns a ptr to it, or NULL if
 *  initiation fails
 *  "max_len" is the max length of a string that can be evaluated
 *  note:   undefined behaviour if "max_len" is 0
*******************************************************************************/
calc_ty *CalcInit(size_t max_len);

/*******************************************************************************
 *  evaluates "expression" by using "calc" and stores the result in "res"
 *  "expression" can contain only numbers (will be interpreted as doubles), 
 *  '+', '-', '*', '/', '(', ')', and single spaces
 *  returns exit status
 *  note: - undefined behaviour if "expression", "calc", or "res" are NULL,
 *          or if the "expression" is longer than "max_len" which was given to 
 *          CalcInit()
 *        - unary operators cannot be followed by spaces
*******************************************************************************/
status_ty Calculate(const char *expression, calc_ty *calc, double *res);

/*******************************************************************************
 *  frees all resources used by "calc"
 *  note:   undefined behaviour if "calc" is NULL
*******************************************************************************/
void CalcDestory(calc_ty *calc);

#endif  /*  __CALC_H__  */
