#include <iostream>

void Foo(const double&); 
void Bar(double&); 

int main(void)
{
    int i = 4; 
    //const int ci = 4; 
    
    Foo(i); 
    Bar(reinterpret_cast<double &>(i)); 
    //Bar(ci); 
    
    std::cout << "Main int address: " << &i << std::endl; 
    
    return 0; 
}

void Foo(const double& d)
{
    std::cout << "Foo double: " << d << std::endl; 
    std::cout << "Foo double address: " << &d << std::endl; 
}

void Bar(double& d)
{
    std::cout << "Bar double: " << d << std::endl; 
    std::cout << "Bar double address: " << &d << std::endl; 
}
