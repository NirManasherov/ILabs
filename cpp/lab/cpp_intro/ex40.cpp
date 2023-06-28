#include <iostream> //cout

class X 
{
public:
    virtual ~X(){std::cout << "this is X Dtor" << std::endl;};   
private:
    double m_a; 
};

class Y: public X
{
private: 
    int m_b; 
}; 

int main()
{
    X *xp = new Y[5]; 
    
    delete[] xp; 
    
    return 0; 
}

