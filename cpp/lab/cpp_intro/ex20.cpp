#include <iostream>

struct X
{
    //initialization function without parameters is named 
    //default constructor (often abberviated as: default Ctor)
    explicit X();
    explicit X(int a_, int b_ = 8); //regular (non default) Ctor
    ~X(); //deinitialization is called a destructor (Dtor)
    X(const X& other_); // copy initalization function is called copy costructor or CCtor
    X& operator = (const X& other_); //assignment operator

    int m_a;
    const int m_b;
};

struct Y
{
    explicit Y();
    explicit Y(X x_, int b_);
    
    X m_x; 
    int m_i; 
};

//impl of the default Ctor. 
//Why X::X? the code after the colon is called an initialization list
X::X():m_a(3), m_b(4)
{
    m_a = 3;
    //m_b = 4;
    std::cout <<"this: " << this << "X default Ctor. m_a: " << 
                           m_a << " m_b: " << m_b << std::endl;
}

Y::Y():m_x(), m_i(4)
{
    m_x = X(); 
    m_i = 3;
    //m_b = 4;
    std::cout <<"this: " << this  << " m_i: " << m_i << std::endl;
}

//impl of the second Ctor
X::X(int a_, int b_): m_a(a_), m_b(b_)
{
    //m_a = a_;
    //m_b = b_;
    std::cout << "this: " << this << 
    " X int int Ctor. m_a: " << m_a << " m_b: " << m_b << std::endl;
}

Y::Y(X x_, int b_): m_x(x_), m_i(b_)
{
    m_x = x_; 
    m_i = b_; 
    
    std::cout << "this: " << this << 
    " m_i: " << m_i << std::endl;
}

//impl of copy Ctor
X::X(const X& other_): m_a(other_.m_a), m_b(other_.m_b)
{
    m_a = other_.m_a;
    //m_b = other_.m_b;
    std::cout << "this: " << this << " X copy Ctor. m_a: "
    << m_a << " m_b: " << m_b << std::endl;
}

X& X:: operator=(const X& other_)
{
    m_a = other_.m_a;
    std::cout << "this: " << this << " X assignment operator. m_a: "  
    << " does not change m_b: " << m_b << std::endl;

    return *this;
}

X::~X()
{
    std::cout << "this " << this << " X Dtor. m_a: " << m_a << " m_b: "
    << m_b << std::endl;
}

int main()
{
    double* dp = new double[10]; 
    delete[] dp; 
    
    X* xp = new X[10]; 
    delete[] xp; 
    
    X x1;
    X x2(7);
    X *px = new X(x2);
    std::cout << "px adress " << px << std::endl;
    X x3(9,10);
    X x4(x1);
    
    std::cout << "start Y " << std::endl;
    Y y1; 
    y1.m_x.m_a = 250; 
    //y1.m_x.m_b = 750;
    Y y2(y1); 
    Y y3; 
    
    y3=y1; 
    std::cout << "end Y " << std::endl;

    //x1 = x3;

    std::cout << x1.m_a << " " << x2.m_b << std::endl;

    std::cout << "size of struct x is " << sizeof(X) << std::endl;
    
    int i = 7; 
    std::cout << "stack var " << &i << std::endl;

    delete px; px = 0;

    return 0;
}

