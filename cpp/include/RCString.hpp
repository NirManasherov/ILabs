#ifndef __ILRD_HRD28_RCSTRING_HPP__
#define __ILRD_HRD28_RCSTRING_HPP__

#include <ostream>

namespace ilrd
{

class String
{
    
//comparison and ostream operators after the class definition 
    
public:
    String(const char *str=""); //implicit for implicit cast of string literal
    ~String();
    
    String(const String& o_);
    String& operator=(const String& o_);
    
    size_t Length() const;
    const char *Cstr() const;
private:
    typedef struct string_handle string_handle_ty; 
    
    string_handle_ty *m_str_handler;
    
    static string_handle_ty *AllocStringIMP(const char *src);
    
    void IncreaseCount(); 
    
    void DecreaseCount(); 
    
    static void DeleteIfNooneWatch(string_handle_ty *str); 
};

struct string_handle
{
    size_t m_ref_no; 
    char m_str_begining[1]; //the size of the string will be considered in creation 
}; 

bool operator>(const String& str1_,const String& str2_);

bool operator<(const String& str1_,const String& str2_);

bool operator==(const String& str1_,const String& str2_);

std::ostream& operator<<(std::ostream& os_, const String& str_);

}// namespace ilrd



#endif // __ILRD_HRD28_RCSTRING_HPP__

//using std::ostream

