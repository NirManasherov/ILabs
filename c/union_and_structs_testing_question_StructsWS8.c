#include <stdio.h>

union members 
	{
		int i; 
		float f; 
		double d;
	}my_member = {1}; 

struct X
{
	int i;
	char c;
	double d; 
	short s;
} my_x; 

struct card 
{
	unsigned int suit:2; 
	unsigned int face_value:4; 
} my_card; 

int main()
{
	printf("%ld\n" , sizeof(my_member) );
	printf("%ld\n" , sizeof(my_x) );
	printf("%ld\n" , sizeof(my_card) );
	return 0;
}
