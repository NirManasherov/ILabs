/*******************************************************************************
 * Project: SharedPointer
 * Author:  HRD28 (Nir Manasherov)
 * Version: 1.0 - 21/05/2023
*******************************************************************************/

#ifndef ILRD_HRD28_SHARED_POINTER_HPP
#define ILRD_HRD28_SHARED_POINTER_HPP

#include <cstddef>      //size_t
#include <cassert>

#include "utils.hpp"

namespace ilrd
{

enum {NO_REFS = 0, DEFAULT_REF = 1};

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

    T& operator*()const; 
    
    T* operator->()const;

    bool operator!()const;

    bool operator==(const SharedPointer& o_) const;
    bool operator!=(const SharedPointer& o_) const;


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
    }
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer& ptr_)
{//fit to assign to self
    m_ptr = ptr_.m_ptr; 
    ++(*(ptr_.m_counter)); 
    m_counter = ptr_.m_counter;     
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer& ptr_)
{
    size_t *temp_count = ptr_.m_counter; 
    
    m_ptr = ptr_.m_ptr; 
    
    --(*m_counter); 
    
    if(*m_counter == NO_REFS)
    {
        delete m_counter; 
    }
    
    m_counter = temp_count;
    
    ++(*m_counter);  
    
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

}//ilrd

#endif // ILRD_HRD28_SHARED_POINTER_HPP
