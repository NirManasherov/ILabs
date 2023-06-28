/*********************************************************************
made by: Nir Manasherov
Date: 14.11.2022
Reviewer: unknown
Version: 1
***********************************************************************/
/**************************libraries***********************************/
#include <stdio.h> 
#include <stdlib.h>
#define ESC 27
#define A 'A'
#define T 'T'
#include "ws4.h"
/*********************************************************************/
/***************************static variables**************************/
static void (*lut[256]) (void); 
/*********************************************************************/
/****************************declerations*****************************/
/*prints out "A" and "T" keys when pressed using if implitation*/ 
static void If (void);

/*prints out "A" and "T" keys when pressed using switch implitation*/ 
static void Switch (void);

/*initializing LUT to the desired values*/
static void InitLut (void);

/*prints out "A" and "T" keys when pressed using LUT implitation*/ 
static void LookUpTable(void) ;

/*prints out "A"*/ 
static void APressed (void);

/*prints out "T"*/ 
static void TPressed (void);

/*Do nothing*/
static void Nothing (void);

/*exit the program*/
static void EscPressed (void); 


/*********************************************************************/
/****************************functions********************************/
/*this function select implitation of the program
running with; 1 - uses if/ese 2 - uses switch 3 - uses LUT*/
void print (char **argv)
{
	system("stty -icanon -echo"); 
	switch (*argv[1]) 
	{
		case '1': 
		{
			If();
			break;  
		}	
		case '2': 
		{
			Switch(); 
			break; 
		}
		
		case '3': 
		{
			InitLut(); 
			LookUpTable(); 
			break;
		}
	}
}
/*********************************************************************/
/*prints out "A" and "T" keys when pressed using if implitation*/ 
static void If (void) 
{
	char pressed = '\0';  
	while (1) /*always true*/ 
	{
		pressed = getchar(); 
		if ( 'A' == pressed )
		{
			APressed(); 
		}
		
		if ( 'T' == pressed )
		{
			TPressed(); 
		}
		
		if ( ESC == pressed )
		{
			EscPressed(); 
		}
	}
}
/*********************************************************************/
/*prints out "A" and "T" keys when pressed using switch implitation*/ 
static void Switch (void) 
{
	char pressed = 0;
	while (1) /*always true*/
	{
		pressed = getchar(); 
		switch (pressed) 
		{
			case 'A' :
			{
				APressed(); 
				break; 
			}
			
			case 'T' :
			{
				TPressed(); 
				break; 
			}
			
			case ESC :
			{
				EscPressed(); 
				break; 
			}
		}
	} 
}
/*********************************************************************/
/*initializing LUT to the desired values*/
static void InitLut (void) 
{
	typedef void (*pointer_to_function) (void);
	pointer_to_function esc_pressed =  &EscPressed; 
	pointer_to_function a_pressed = &APressed; 
	pointer_to_function t_pressed = &TPressed; 
	pointer_to_function nothing = &Nothing;
	
	int i = 0; 
	for (i=0 ; i<256 ; i++) 
	{
		lut[i] = nothing; 
	}
	
	lut[(int)ESC] = esc_pressed; 
	lut[(int)A] = a_pressed; 
	lut[(int)T] = t_pressed; 
}
/*********************************************************************/
/*prints out "A" and "T" keys when pressed using LUT implitation*/ 
static void LookUpTable(void) 
{
	while (1) /*always true*/ 
	{
		lut[getchar()](); 
	}
}
/*********************************************************************/
/*prints out "A"*/ 
static void APressed (void)
{
	puts("A pressed"); 
}
/*********************************************************************/
/*prints out "T"*/ 
static void TPressed (void)
{
	puts("T pressed");
}
/*********************************************************************/
/*Do nothing*/
static void Nothing (void) 
{
	return; 
}
/*********************************************************************/
/*exit the program*/
static void EscPressed (void)
{
	system("stty icanon echo"); 
	exit(EXIT_SUCCESS);
}
/*********************************************************************/
