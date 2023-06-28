#include <iostream>

bool b1 = true; 
bool b2; 
int i = b1; 

int main(void)
{
    std::cout << "uninit bool val " << b2 << "\n";
    
    i = 7; 
    b2 = i; 
    /*printf("%d", b2);*/

    std::cout << b2 << "\n"; 
    
    std::cout << sizeof(bool) << "\n";    
}




