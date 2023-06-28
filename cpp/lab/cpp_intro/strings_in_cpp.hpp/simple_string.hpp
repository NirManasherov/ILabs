class String
{
public: 
    explicit String(char *str_); 
    String(const String& other_);
    String& operator=(const String& other_);
    
    size_t Length(); 
    char *Cstr(); 
    
private: 
    chatr *m_str; 
    size_t size; 
}
