/*
exercise 37: Exeption safety - Dtors
*/

#include <iostream>
#include <stdexcept>
#include <typeinfo>

using namespace std;
/*
class X
{
public: 
    ~X(); 
    
    int Foo(int) throw(bad_alloc);  
    void Fishi();
    void Bar() throw(bad_cast);

private:
    int m_a;
};*/

class X
{
public:
    int Foo(int) throw(bad_alloc); 
    explicit X(){}
    ~X();
private:
    int m_a;
};


void Bar() throw(bad_cast){}

X::~X()
{
    cerr << Foo(m_a) << endl; 
}

void Fishi()
{
    X x1; 
    //...
    Bar();
    //... 
}

//my additions 
//section b
int X::Foo(int) throw(bad_alloc)
{
    throw(bad_alloc()); 
}

int main(void)
{
    X x; 
    x.Foo(1);
    
    return 0;
}

