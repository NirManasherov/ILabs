#include <stdio.h>

enum E {AA, BB, CC, DD}; 

enum E2 {TT, KK};

enum E e=2; 

enum E2 e2 = AA; 

/*E2 e22 = KK;*/

/*++e;*/

/*int i = e;*/

int main(void)
{
    int j = e;
    printf("AA: %d\n", AA); 
    printf("BB: %d\n", BB); 
    printf("CC: %d\n", CC); 
    printf("DD: %d\n", DD); 
    printf("TT: %d\n", TT); 
    printf("KK: %d\n", KK);  
    printf("e: %d\n", e);
    printf("e2: %d\n", e2);   
    printf("j: %d\n", j); 
}
