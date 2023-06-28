#include <iostream>

void foo();
void foo(int i);
void foo(char c);

int main(void)
{
    int i = 7; 
    foo(i); 
    
    char c = 'N'; 
    foo(c); 
    
    //long l = 9; 
    //foo(l);
    
    bool b = 1; 
    foo(b); 
    
    //double d = 3.14; 
    //foo(d); 
    
    //float f = 2.4; 
    //foo(f);
    
    //void *ptr = &i; 
    //foo(ptr); 
    
    unsigned char uc = 'c'; 
    foo(uc); 
    
    short sh = 2; 
    foo(sh); 
    
    //foo(NULL); 
}

void foo()
{
    std::cout << "this is the regular foo" << std::endl; 
}

void foo(int i)
{
    std::cout << "this is the inty foo and i've got the int :" << i << std::endl; 
}

void foo(char c)
{
    std::cout << "this is the chary foo and i've got the char :" << c << std::endl;  
}


