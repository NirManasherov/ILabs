#include "StateMachine.h" 

#define ZERO_ASCII (int)'0'

typedef enum states {FIRST_CHAR = 0 , WAITING_FOR = 1, ACCEPTED = 2, ERROR = 3, 
NUM_OF_STATES} state_ty; 

typedef enum imput {ZERO = 0, ONE =1, NUM_OF_IMPUT} imput_ty; 

static int StateLUT[NUM_OF_STATES][NUM_OF_IMPUT] =
{	{WAITING_FOR, ERROR		}, 
	{ACCEPTED, WAITING_FOR	}, 
	{ACCEPTED, WAITING_FOR	}, 
	{ERROR, ERROR			}
};  

/*translate num char to a number value*/
static int MakeNum(char num); 

/*
return true or false if str starts with '0' and end with '0' and have at 
least two chars
*/ 
int IsWordAccepted(const char *str)
{
		state_ty curr_state = FIRST_CHAR; 
		
		while('\0' != *str)
		{
			while((MakeNum(*str) == ZERO) || (MakeNum(*str) == ONE))
			{
				curr_state = StateLUT[curr_state][MakeNum(*str)];
				++str;
			}
			
			if('\0' != *str)
			{
				++str; 
				curr_state = FIRST_CHAR;
			}		
		}
		
		return (curr_state == ACCEPTED); 
}

/*translate num char to a number value*/
static int MakeNum(char num)
{
	return ((int)num - ZERO_ASCII); 
}


