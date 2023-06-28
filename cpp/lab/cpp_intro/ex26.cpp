#include <cstdio>

struct X
{
public:
    explicit X(); 
    explicit X(int a_);
    void Foo();
    int GetMem() const; 
    //void SetMem(int a_); 

private:
    int m_a;
};

X::X(int a_) : m_a(a_)
{}

X::X(): m_a(0); 
{}

void X::Foo()
{
    printf("%d\n", m_a);
}

int X::GetMem() const
{
    return m_a; 
}

/*void X::SetMem(int a_)
{
    m_a = a_; 
}*/

void Foo(const X &x_)
{
    printf("%d\n", x_.GetMem());
}

int main()
{
    X x1;

    x1.Foo();
    printf("%d\n", x1.m_a);
    Foo(x1);

    return 0;
}
