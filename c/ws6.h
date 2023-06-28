/**************************libraries***********************************/
#ifndef __WS6__H_ 
#define __WS6__H_

/**************************functions***********************************/
/*this function gets two number and return it's x*2^y*/ 
long pow2(unsigned int x, unsigned int y);

/*this function checks if a number is a power of 2 with using loops*/ 
int IsPowTwoLoop(unsigned int n); 

/*this function checks if a number is a power of 2 without using loops*/ 
int IsPowTwo(unsigned int n);

/*this function gets a number and return it's value + 1*/ 
int AddOne(int n) ;

/*this function count the number of ones in a binary representation of a number*/
int OneCount(int n);

/*this function mirror a byte (100 turns to 001) using a loop*/
unsigned char ByteMirrorLoop(unsigned char n) ;

/*this function mirror a byte (100 turns to 001) whithout using a loop*/
unsigned char ByteMirror(unsigned char n); 

/*this function returntrue if at least one of the 5th and third bits are both on
else it returns false*/
int IsSixAndTwoOn(unsigned char n);

/*this function return true if at least one of the 5th and third bits are on
else it returns false*/
int IsSixTwoOneLeastOn(unsigned char n);

/*this function swaps the 5th and third bits in a given unsigned int*/
int SwapFiveThree(unsigned char n);

/*this function finds the smaller closest number that is devisable by 16 
to a given unsigned int*/
unsigned int SixteenDevidedClosest(unsigned int n);

/*this function swaps two intigers without using temp variable*/
void Swap(int *x, int *y);

/*this function calculate how many bits are on in an intiger with using loops*/
int CountOnBitLoop(int n); 

/*this function calculate how many bits are on in an intiger without using loops
this method is based on hemming weight algorithm*/
int CountOnBit(int n);

/*this function recieves a float and print it bit by bit*/
void PrintFloat(float n); 
/**********************************************************************/

#endif /* __WS6__H_ */
/**********************************************************************/
