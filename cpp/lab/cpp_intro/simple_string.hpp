#ifndef __SIMPLE_STRING__
#define __SIMPLE_STRING__

namespace irld
{

class String
{
public: 
    explicit String(const char *s_); 
    String(const String& o_);
    String& operator=(const String& o_);
    
    size_t Length() const; 
    const char *Cstr() const; 
    
private: 
    chatr *m_str; 
}

}//end namespace

#endif //__SIMPLE_STRING__
