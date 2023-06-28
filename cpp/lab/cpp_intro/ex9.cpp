#include <iostream>

void foo(int, int i = 3, float = 4);

//void bar(int i = 10, int j);  

int main(void)
{
    foo(3); 
    foo(6, 5); 
    foo(4, 7, 8.7); 
    //bar(1);
    //bar(1,2);  
    foo(1, 2.00001); 
    
    
    return 0;
}

void foo(int a, int b, float c)
{
    std::cout << "first arg: " << a << std::endl; 
    std::cout << "second arg: " << b << std::endl; 
    std::cout << "third arg: " << c << std::endl; 
}

void bar(int i, int j)
{
    std::cout << "first arg: " << i << std::endl; 
    std::cout << "second arg: " << j << std::endl; 
}
