/*******************************************************************************
*	author: 	Nir Manasherov
*	reviewer: 	Lital Barkai
*	version: 	1.0 - 05.02.2023
*******************************************************************************/
#include <stdlib.h>     /*  malloc, free   */
#include <assert.h>		/*  assert		   */

#include "calculator.h"
#include "stack.h"

#define EVENT_NUM	256
#define LUT_SIZE	256
#define STACK_SIZE	50

#define GET_ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define OP_PRIORITY(op) ((signed int) op_lut[(int)(op)]->priority)
#define OP_ASSO(op) ((signed int) op_lut[(int)(op)]->associativity)
#define UNUSED(x) (void)(x)

/*******************************************************************************
							typedefs and enums
*******************************************************************************/

typedef enum 	{	
				WAIT_FOR_NUM = 0, 
				WAIT_FOR_OP = 1, 
				STATE_RANGE, 
				FINISH = 3
				} state_ty;
typedef enum {LTR = 0, RTL = 1} asso_ty;

typedef enum {
				STUB = -1,
				PAR = 0, 
				ADD = 1, 
				SUB = ADD, 
				MULT = 2, 
				DIV = MULT
				}priority_ty;

typedef struct state_entry state_entry_ty;
typedef struct operation operation_ty;

typedef status_ty (*handler_ty)(calc_ty *calc);
typedef double (*calc_func_ty)(double num1, double num2);

/*******************************************************************************
							structs and handles
*******************************************************************************/

struct calc
{
	char *runner;		/* point to position in expression */
	stack_ty *num_stack;
	stack_ty *op_stack;
};

struct state_entry 
{
    char event;
    handler_ty handler_func;
    state_ty next_state;
};

struct operation
{
    char op;
    priority_ty priority;
    asso_ty associativity;
    calc_func_ty calc_func;
};

/*******************************************************************************
							static functions
*******************************************************************************/
static double Add(double num1, double num2);

static double Substract(double num1, double num2);

static double Multiply(double num1, double num2);

static double Divide(double num1, double num2); 

/* handles a valid num when machine is in state WAIT_FOR_NUM */
static status_ty NumHandler(calc_ty *calc);

/* handles a '(' when machine is in state WAIT_FOR_NUM */
static status_ty OpenParHandler(calc_ty *calc);

/* handles a ')' when machine is in state WAIT_FOR_OP */
static status_ty CloseParHandler(calc_ty *calc);

/* handles a valid operator when machine in state WAIT_FOR_OP */
static status_ty OpHandler(calc_ty *calc);

/* calculate one operator and push res to num_stack */
static void CalcOneOp(calc_ty *calc);

/* calculate all the operators and push res to num_stack */
static status_ty FoldNumStack(calc_ty *calc);

static status_ty BadNumHandler(calc_ty *calc);

static status_ty BadOpHandler(calc_ty *calc);

static status_ty DoNothing(calc_ty *calc);

static void LUTsInit(void); 

/*******************************************************************************
									functions
*******************************************************************************/
static operation_ty operation[] =	{
										{-1, STUB, 0, NULL},
                                		{'+', ADD, LTR, Add},
										{'-', SUB, LTR, Substract},
										{'*', MULT, LTR, Multiply},
										{'/', DIV, LTR, Divide},
										{'(', PAR, 0, NULL}
									};

static operation_ty *op_lut[LUT_SIZE] = {0};

static state_entry_ty get_num_state[] =	{
											{-1, BadNumHandler, FINISH},
											{'(', OpenParHandler, WAIT_FOR_NUM},
                                            {'+', NumHandler, WAIT_FOR_OP},
                                            {'-', NumHandler, WAIT_FOR_OP},
                                            {'0', NumHandler, WAIT_FOR_OP},
                                            {'1', NumHandler, WAIT_FOR_OP},
                                            {'2', NumHandler, WAIT_FOR_OP},
                                            {'3', NumHandler, WAIT_FOR_OP},
                                            {'4', NumHandler, WAIT_FOR_OP},
                                            {'5', NumHandler, WAIT_FOR_OP},
                                            {'6', NumHandler, WAIT_FOR_OP},
                                            {'7', NumHandler, WAIT_FOR_OP},
                                            {'8', NumHandler, WAIT_FOR_OP},
                                            {'9', NumHandler, WAIT_FOR_OP},
											{' ', DoNothing, WAIT_FOR_NUM},
										};

static state_entry_ty get_op_state[] =	{
											{-1, BadOpHandler, FINISH},
											{')', CloseParHandler, WAIT_FOR_OP},
											{'+', OpHandler, WAIT_FOR_NUM},
											{'-', OpHandler, WAIT_FOR_NUM},
											{'*', OpHandler, WAIT_FOR_NUM},
											{'/', OpHandler, WAIT_FOR_NUM},
											{'\0', FoldNumStack, FINISH}
										};

static state_entry_ty *machine_lut[STATE_RANGE][EVENT_NUM] = {0};

calc_ty *CalcInit(size_t max_len)
{
	calc_ty *calc = NULL;

	assert(0 != max_len);
	
	calc = (calc_ty *)malloc(1 * sizeof(calc_ty));
	if (!calc)
	{
		return NULL;
	}
	
    if (!op_lut['+'])
    {	/* we get here only once when we need to init the LUTs */
        LUTsInit();
    }

    calc->num_stack = StackCreate(max_len, sizeof(double));
    if (!calc->num_stack)
	{
		free(calc);
		return NULL;
	}

	calc->op_stack = StackCreate(max_len, sizeof(char));
	if (!calc->op_stack)
	{
		StackDestroy(calc->num_stack);	
		calc->num_stack = NULL;
		free(calc);
		calc = NULL; 
		
		return NULL;
	}

	return calc;
}

void CalcDestory(calc_ty *calc)
{
	assert(NULL != calc); 
	
	StackDestroy(calc->num_stack);
	calc->num_stack = NULL; 
    StackDestroy(calc->op_stack);
    calc->op_stack = NULL; 
	free(calc);
	calc = NULL; 
}

status_ty Calculate(const char *expression, calc_ty *calculator, double *res)
{
    state_ty curr_state = WAIT_FOR_NUM;	/* initial state */
    
	calc_ty *calc = NULL;
		
	status_ty status_value = SUCCESS; 
	
	status_ty *status = &status_value;
	
	char event = '\0'; 
	
	state_entry_ty *next_event = NULL; 
	
	assert(calculator);
	
	calc = calculator;
	calc->runner = (char *)expression;

	StackPush(calc->op_stack, "1");		/* stub for base of op_stack */


    while (FINISH != curr_state)
    {
        event = *calc->runner;
        
		next_event = machine_lut[curr_state][(int)event];
		
        *status = next_event->handler_func(calc);
        
        curr_state = next_event->next_state;
        if (SUCCESS != *status)
        {	/* if handler_func failed we break */
            curr_state = FINISH;
        }
    }

	if (SUCCESS == *status)
	{
    	*res = *((double *)StackPeek(calc->num_stack));
	}

	while(!StackIsEmpty(calc->op_stack))
	{
		StackPop(calc->op_stack);
	}
	
	while(!StackIsEmpty(calc->num_stack))
	{
		StackPop(calc->num_stack);
	}

    return *status;
}

static double Add(double num1, double num2)
{
    return (num1 + num2);
}

static double Substract(double num1, double num2)
{
    return (num1 - num2);
}

static double Multiply(double num1, double num2)
{
    return (num1 * num2);
}

static double Divide(double num1, double num2)
{
    if (0 == num2)
    {
    	return MATH_ERR;
    }
	return (num1 / num2);
}

/* handles a valid num when machine is in state WAIT_FOR_NUM */
static status_ty NumHandler(calc_ty *calc)
{
    double num = 0;
    char *befor_strtod = NULL; 
    
	assert(calc);
	
	befor_strtod = (char *)calc->runner; 
	
    num = strtod(calc->runner, &calc->runner);  /* set runner to next char in string */
    if (calc->runner == befor_strtod)
    {
        return SYNTAX_ERR;
    }

    StackPush(calc->num_stack, &num);

    return SUCCESS;
}

/* handles a '(' when machine is in state WAIT_FOR_NUM */
static status_ty OpenParHandler(calc_ty *calc)
{
	assert(calc);

	StackPush(calc->op_stack, calc->runner);
	
	calc->runner += 1;

    return SUCCESS;
}

/* handles a ')' when machine is in state WAIT_FOR_OP */
static status_ty CloseParHandler(calc_ty *calc)
{
	assert(calc);

	FoldNumStack(calc);

	StackPop(calc->op_stack);	/* pop the '(' */

	calc->runner += 1;

    return SUCCESS;
}


static status_ty BadNumHandler(calc_ty *calc)
{
	assert(calc); 
	
    UNUSED(calc);
	return SYNTAX_ERR;
}

static status_ty BadOpHandler(calc_ty *calc)
{
    assert(calc); 
	
	UNUSED(calc);
    return SYNTAX_ERR;
}

static status_ty DoNothing(calc_ty *calc)
{
	assert(calc);

	calc->runner += 1;

	return SUCCESS;
}

static void LUTsInit(void)
{
	size_t i = 0;
	size_t arr_size = 0;

	/* init op_lut with the default operator */
	for (i = 0; i < LUT_SIZE; i++)
	{
		op_lut[i] = &operation[0];
	}

	arr_size = GET_ARR_SIZE(operation);
	/* init op_lut with data in operation */
	for (i = 1; i < arr_size; i++)
	{
		op_lut[(int)operation[i].op] = &operation[i];
	}

	/* init machine_lut with the bad num/op handlers */
	for (i = 0; i < EVENT_NUM; i++)
	{
		machine_lut[WAIT_FOR_NUM][i] = &get_num_state[0];
		machine_lut[WAIT_FOR_OP][i] = &get_op_state[0];
	}

	arr_size = GET_ARR_SIZE(get_num_state);
	/* init machine_lut with valid events */
	for (i = 1; i < arr_size; i++)
	{
		machine_lut[WAIT_FOR_NUM][(int)get_num_state[i].event] =
		 &get_num_state[i];
	}

	arr_size = GET_ARR_SIZE(get_op_state);
	for (i = 1; i < arr_size; i++)
	{
		machine_lut[WAIT_FOR_OP][(int)get_op_state[i].event] = &get_op_state[i];
	}
}

/* handles a valid operator when machine in state WAIT_FOR_OP */
static status_ty OpHandler(calc_ty *calc)
{
    char op_top = '\0';	/* top operator in stack */
	char op = '\0';		/* operator we want to push */

	assert(calc);
	op_top = *(char *)StackPeek(calc->op_stack);
	op = *calc->runner;

	while (OP_PRIORITY(op_top) >= (OP_PRIORITY(op) + OP_ASSO(op)))
	{
		CalcOneOp(calc);
		op_top = *(char *)StackPeek(calc->op_stack);
	}

	StackPush(calc->op_stack, calc->runner);

	calc->runner += 1;

    return SUCCESS;
}

/* calculate one operator and push res to num_stack */
static void CalcOneOp(calc_ty *calc)
{
	double num1 = 0;
	double num2 = 0;
	char op = 0;

	assert(calc);
	
	/*numbers are in reverse order on the stack*/
	num2 = *(double *) StackPeek(calc->num_stack);
	StackPop(calc->num_stack);
	num1 = *(double *) StackPeek(calc->num_stack);
	StackPop(calc->num_stack);
	op = *(char *) StackPeek(calc->op_stack);
	StackPop(calc->op_stack);
	
	num1 = op_lut[(int)op]->calc_func(num1, num2);
	
	StackPush(calc->num_stack, &num1);
}

/* calculate all the operators */
static status_ty FoldNumStack(calc_ty *calc)
{
	assert(calc);
	
	while ((StackGetSize(calc->op_stack) > 1) &&
	('(' != *(char *) StackPeek(calc->op_stack)))
	{
		CalcOneOp(calc);
	}
	
	if (StackGetSize(calc->op_stack) > 1)
	{	/* we get here when we have '(' in stack */
		return SYNTAX_ERR;
	}

    return SUCCESS;
}
