#include <cstring>  //strlen, strcmp.. 
#include <cassert>  //assert
#include <iostream> //io operations
#include <cstddef>  //offsetof

#include "RCString.hpp"

namespace ilrd
{

String::String(const char *str): m_str_handler(AllocStringIMP(str)){}

String::String(const String& o_): m_str_handler(o_.m_str_handler){}

const char *String::Cstr() const
{
    return m_str_handler->m_str_begining;
}

size_t String::Length() const
{
    return strlen(m_str_handler->m_str_begining);
}

String::~String()
{
    DecreaseCount(); 
    
    DeleteIfNooneWatch(m_str_handler);
}

String& String::operator=(const String& o_)
{//handles assignment to self    
    string_handle_ty *start_str = m_str_handler; 
  
    DecreaseCount();//in the left-side string
    
    m_str_handler = o_.m_str_handler; 
    
    IncreaseCount();//in the right-side string
    
    DeleteIfNooneWatch(start_str);
    
    return *this;
}

//static method
String::string_handle_ty *String::AllocStringIMP(const char *src)
{
    assert(NULL !=src);

    size_t len = strlen(src);
    
    string_handle_ty *ret_str_handler = reinterpret_cast<string_handle_ty *>
                  (operator new(offsetof(string_handle, m_str_begining) + len));
    
    ret_str_handler->m_ref_no = 1;
    
    std::copy(src, src + len, ret_str_handler->m_str_begining);
    
    return ret_str_handler;
}

void String::IncreaseCount()
{
    ++(m_str_handler->m_ref_no); 
}

void String::DecreaseCount()
{
    --(m_str_handler->m_ref_no); 
}

//static member function
void String::DeleteIfNooneWatch(string_handle_ty *str)
{
    if(0 == str->m_ref_no)
    {
        delete str;
    }    
}

bool operator<(const String& str2_, const String &str1_)
{
    return !!(strcmp(str1_.Cstr(), str2_.Cstr()) < 0);
}

bool operator>(const String& str2_,const String& str1_)
{
    return !!(strcmp(str1_.Cstr(), str2_.Cstr()) > 0);
}

bool operator==(const String& str2_,const String& str1_)
{
    return !!(strcmp(str1_.Cstr(), str2_.Cstr()) == 0);
}

std::ostream& operator<<(std::ostream& os_, const String& str_)
{
    return os_ << str_.Cstr();
}

}//namespace ilrd

using namespace std; 
using namespace ilrd; 

int main()
{
    String str("hi");

    cout << "1.the string of str is: " << str.Cstr() << endl;

    String str2(str);

    cout << "2.the string of str2 is: " << str2.Cstr() << endl;

    str = "hello";

    cout << "3.the string of str is: " << str.Cstr() << endl;

    str2 = str2;

    cout << "4.the string of str2 is: " << str2.Cstr() << endl;

    str = str2;

    return 0;
}

