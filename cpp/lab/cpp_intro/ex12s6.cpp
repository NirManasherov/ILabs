#include <dlfcn.h>
#include <cstddef>
#include <iostream>

typedef void (*foo_handler)(int); 

int main(void)
{
    void *foo_lib = dlopen("/home/student/nir/git/cpp/lab/cpp_intro/libex12.so", 
                                                                     RTLD_LAZY);
    
    if(NULL == foo_lib)
    {
        std::cout << "dlopen failed!" << std::endl;
        
        return 1; 
    }
    
    void *foo_handle = dlsym(foo_lib, "_Z3fooi"); 
    
    if(NULL == foo_handle)
    {
        std::cout << "dlsym failed!" << std::endl;
        
        return 1; 
    }
    
    char i = 10; 
    ((foo_handler)foo_handle)(i); 
    
    int ret_close = dlclose(foo_lib);
    
    if(0 != ret_close)
    {
        std::cout << "dlclose failed!" << std::endl;
        
        return 1; 
    } 
    
    return 0;
}
