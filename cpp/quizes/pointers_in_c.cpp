#include <iostream>

using namespace std;
int main(void)
{
    int a = 7; 
    int *ptr = &a; 

    std::cout << ptr << std::endl; 
    std::cout << *ptr << std::endl; 

    //char *c_ptr = reinterprate_cast<char *>&ptr; 
    //*c_ptr = 1; 
    *(char *)&ptr = 1;
     
    std::cout << ptr << std::endl; 
    std::cout << *ptr << std::endl; 
    
    return 0; 
}

