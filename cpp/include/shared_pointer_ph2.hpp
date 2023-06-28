/*******************************************************************************
 * Project: SharedPointer
 * Author:  HRD28
 * Version: 1.0 - 21/05/2023
 	    1.1 - implicit conversion for up cast
 	        - add Get() function
*******************************************************************************/

#ifndef ILRD_HRD28_SHARED_POINTER_HPP
#define ILRD_HRD28_SHARED_POINTER_HPP

#include <cstddef>      //size_t

#include "utils.hpp"

enum {NO_REFS = 0, DEFAULT_REF = 1};

namespace ilrd
{

//undefined behavior if the typename is array,
//    memory in stack and undefined variables
template <typename T>
class SharedPointer
{
public:
    explicit SharedPointer(T *val_ptr_= NULL); 
    ~SharedPointer()NOEXCEPT;
    
    SharedPointer(const SharedPointer& ptr_);

    SharedPointer& operator=(const SharedPointer& ptr_);
    
    template<class C> friend class SharedPointer;
	    
    template<class C>
    SharedPointer(const SharedPointer<C>& o_);

    template<class C>
    SharedPointer& operator=(const SharedPointer<C>& o_);
    

    T& operator*()const; 
    
    T* operator->()const;

    bool operator!()const;

    bool operator==(const SharedPointer& o_) const;
    bool operator!=(const SharedPointer& o_) const;
    
    T* Get()const;

private:
    T *m_ptr;
    size_t *m_counter;

};

template <typename T>
SharedPointer<T>::SharedPointer(T *val_ptr_):m_ptr(val_ptr_), 
                                              m_counter(new size_t(DEFAULT_REF))
{}

template <typename T>
SharedPointer<T>::~SharedPointer()NOEXCEPT
{
    --(*m_counter); 
    
    if(*m_counter == 0)
    {
        delete m_counter;
        delete m_ptr; 
    }
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer& ptr_)
{
    m_ptr = ptr_.m_ptr; 
    ++(*(ptr_.m_counter)); 
    m_counter = ptr_.m_counter;     
}

template <typename T>
template<class C>
SharedPointer<T>::SharedPointer(const SharedPointer<C>& o_):
        m_ptr(static_cast<T>(o_.m_ptr)), m_counter(&(++(*o_.m_counter)))
{}

template <typename T>
template<class C>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<C>& o_)
{    
   return (*this = SharedPointer<T>(o_));
}

template <typename T>
SharedPointer& SharedPointer<T>::operator=(const SharedPointer& ptr_)
{//assignment to self

    T *temp_m_ptr = m_ptr;
    size_t *temp_count = m_counter; 
    
    m_ptr = ptr_.m_ptr;
    
    --(*m_counter);

    ++(*(ptr_.m_counter));  
        
    if(*m_counter == NO_REFS)
    {
        delete temp_m_ptr; 
        delete temp_count; 
    }

    m_counter = ptr_.m_count;

    return *this;
}


template <typename T>
T& SharedPointer<T>::operator*()const
{
    assert(NULL != m_ptr); 
    
    return *m_ptr; 
}

template <typename T>
T* SharedPointer<T>::operator->()const
{
    return m_ptr;
}

template <typename T>
bool SharedPointer<T>::operator!()const
{
    if(NULL == m_ptr)
    {
        return true;
    }
    
    return false;
    
    //return !m_ptr; 
}

template <typename T>
bool SharedPointer<T>::operator==(const SharedPointer& o_) const
{
    return (m_ptr == o_.m_ptr); 
}

template <typename T>
bool SharedPointer<T>::operator!=(const SharedPointer& o_) const
{
    return !(this->operator==(o_)); 
}

template <typename T>
T* SharedPointer<T>::Get()const
{
    return m_ptr; 
}

}//ilrd

#endif // ILRD_HRD28_SHARED_POINTER_HPP
