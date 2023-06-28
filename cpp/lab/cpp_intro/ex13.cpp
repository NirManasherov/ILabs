namespace useless
{
unsigned int g_wastful[400]; 
extern int g_notthere;
void foo() {}

namespace wasteoftyme
{
void Foo() {}
}//namespace wasteoftyme 
}//namespace useless

namespace stupid
{
void Foo() {}
void Bar() {}
void DoNothing(unsigned int) {}
}//namespace stupid 

namespace useless 
{
void DoNothing(int) {}
}//namespace useless
void DoNothing(int); 

void Foo(); 

void DoStuff()
{
    stupid::Bar(); 
    Foo(); 
    using stupid::Foo; 
    Foo(); 
    DoNothing(useless::g_wastful[3] + 1); 
}

void Foo() {}

using namespace stupid; 

namespace comeon = useless::wasteoftyme; 

void DoMoreStuff()
{
    comeon::Foo(); 
    //Foo(); //try uncommenting this line, solve the error
    Bar(); //why doesn'r this line create an error? 
    DoNothing(useless::g_wastful[3] + 1); 
}

namespace useless 
{
void DoUselessStuff()
{
    DoNothing(useless::g_wastful[3] + 1); 
}
}//namespace useless
