#include <iostream>

int g_i = 1; 
int& g_ip = g_i; 

struct foo_struct 
{
    int i; 
    float f; 
};

int foo(int i, float f);

int main(void)
{
    int i = 8; 
    int& ip = i; 
    
    const int ci = 8; 
    const int& cip = ci; 
    
    short sh = 9; 
    short& shp = sh; 
    
    void *vp = NULL; 
    void *& vpp = vp; 
    
    long l = 14; 
    long& lp = l; 
    
    bool b = true; 
    bool& bp = b; 
    
    float f = 14.3; 
    float& fp = f;
    
    double d = 6.5; 
    double& dp = d; 
        
    shp = 13; 
    ip = 9; 
    vpp = &i; 
    lp = 17; 
    bp = false; 
    fp = 22.34; 
    dp = 3.14;  
    g_ip = 10;    
    
    std::cout << "int " << i << std::endl; 
    std::cout << "short " << sh << std::endl; 
    std::cout << "pointer " << vp << std::endl;
    std::cout << "long " << l << std::endl; 
    std::cout << "boolean " << b << std::endl;
    std::cout << "float " << f << std::endl;
    std::cout << "double " << d << std::endl;
    std::cout << "const int " << cip << std::endl;
    std::cout << "global int " << g_i << std::endl; 
    
    std::cout << "int adress " << &i << " int reference adress " << &ip <<
                               "are equal?: " << (bool)(&i == &ip) << std::endl; 
    
    
    std::cout << "int " << foo(ip, f) << std::endl; 
    
    std::cout << "int after foo " << i << std::endl; 
    std::cout << "float after foo " << f << std::endl;
    
    std::cout << "sizeof reference var: " << sizeof(bp) << std::endl;
                                                        
    return ip; 
}

int foo(int i, float f)
{
    i = 1; 
    f = 7.14; 
    
    struct foo_struct st = {0 , 0}; 
    st.i = i; 
    st.f = f; 
    
    std::cout << "float in struct " << st.f << std::endl;
    
    return (st.i); 
}
