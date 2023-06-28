#include <stdio.h>

void foo1();
void foo2(int i);
void foo3(char c);

void foo1()
{
   printf("this is the regular foo"); 
}

void foo2(int i)
{
    printf("this is the inty foo and i've got the int : %d", i);  
}

void foo3(char c)
{
    printf("this is the chary foo and i've got the char : %c", c); 
}


