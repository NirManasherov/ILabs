
#include <iostream>

struct X
{
    X(int a_, int b_);

    void Inc();
    void Dec();

    int m_a;
    int m_b;
};

X::X(int a_, int b_): m_a(a_), m_b(b_)
{}

void X::Inc()
{
    std::cout << "this: " << this << std::endl;
    std::cout << "memFunc Inc() is here " << std::endl;
    ++m_a;
    ++m_b;
}

void X::Dec()
{
    std::cout << "this: " << this << std::endl;
    std::cout << "memFunc Dec() is here " << std::endl;
    --m_a;
    --m_b;
}

void Inc()
{
    std::cout << "NO!!!! not a memFunc Inc() is here Impasta ~*~" << std::endl;
}

int main(void)
{
	X x1(7,-55);
    X x2(x1);
    
    std::cout << "this is the adress of x1 " << &x1 << std::endl;
    std::cout << "this is the adress of x2 " << &x2 << std::endl;

    Inc();
    x1.Inc();
    x1.Inc();
    x2.Dec();
    
    //std::cout << "this: " << this << std::endl;
    
    return 0;
}

