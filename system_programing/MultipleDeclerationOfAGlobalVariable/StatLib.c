#include <stdio.h>
#include "StatLib.h"

int x = 14; 

void PrintGlobalVariableAdress(void)
{
	printf("%p" ,(void *)&x);
}

