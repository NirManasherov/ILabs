/*******************************************************************************
 * Project: BitArray
 * Author:  HRD28
 * Version: 1.0 - 16/05/2023
            1.1 - 16/05/2023: - NOEXCEPT with Dtor only
                              - We want to delete operator& on &bitset[i]
                               (BitProxy)
            1.2 - 17/05/2023: - fixed BitProxy to be template <size_t SIZE>
                              - removed "vector" header
                              - added comments why we added cstddef, string
                                and boost/core/noncopyable.hpp
            1.3 - 17/05/2023: - added static variable for saving the size of
                                array of size_t, instead of MACROS
            1.4 - 17/05/2023: - fixed the size of array to static const
                              - Dtor of BitArray,Dtor and CCtor of BitProxy are
                                generated
                              - changed name of GetBit to At
                              - added comments on each method in BitProxy
            1.5 -17/05/2023:  - changed where_ to idx_ for consistency
                              - the comment on the functions operator[]
                                is updated
                              - the comment on the function operator bool()
                                is updated
            1.6 -18/05/2023:  - removed private boost::noncopyable, and instead
                                we can use the generated Cctor and assignemt =
*******************************************************************************/
#ifndef ILRD_HRD28_BIT_ARRAY_HPP
#define ILRD_HRD28_BIT_ARRAY_HPP

#include <cstddef>      //size_t
#include <string>       //std::string
#include <climits>      //CHAR_BIT
#include <numeric>      //accumilate
#include <algorithm>    //iterator
#include <iterator>		//class iterator
#include <cassert>		//assert


#include "utils.hpp" //NOEXCEPT

#define WORD_BITS (sizeof(size_t) * CHAR_BIT)

namespace ilrd
{

enum {MASK_ONE = 1}; 

template <size_t SIZE>
class BitArray
{
private:
    class BitProxy;

public:
    explicit BitArray();

    //~BitArray() NOEXCEPT=default;
    //BitArray(const BitArray& o_) = default;
    //BitArray& operator=(const BitArray& o_) = default;

    bool operator==(const BitArray& o_) const;
    bool operator!=(const BitArray& o_) const;

    BitArray& operator|=(const BitArray& o_);
    BitArray& operator&=(const BitArray& o_);
    BitArray& operator^=(const BitArray& o_);

    //idx starts from 0 at LSB, undefined behaviour if idx is out of range
    bool operator[](size_t idx_) const NOEXCEPT;
    BitProxy operator[](size_t idx_) NOEXCEPT;

    bool At(size_t idx_) const; //throws std::out_of_range

    BitArray& SetAll(bool val_ = 0);
    BitArray& SetBit(size_t idx_, bool val_);

    BitArray& FlipAll();
    BitArray& FlipBit(size_t idx_);

    size_t CountSetBits() const;

    std::string ToString() const; //throws std::bad_alloc

private:
	static const size_t ARR_SIZE = ((SIZE - (SIZE % WORD_BITS)) / WORD_BITS) +
                                                        !!(SIZE % WORD_BITS);

    size_t m_array[ARR_SIZE];

	static char MakeChar(const bool &b_); 
    
    void CleanForbidenBits();
    
    bool GetBit(size_t idx_) const;
    
    //unary biotwise operators
    
    static size_t Or(const size_t &num1_, const size_t &num2_)
    {
    	return (num1_)|(num2_); 
    }
    
    static size_t And(const size_t &num1_, const size_t &num2_)
    {
    	return (num1_)&(num2_); 
    }
    
    static size_t Xor(const size_t &num1_, const size_t &num2_)
    {
    	return (num1_)^(num2_); 
    }
    
    static size_t Not(const size_t &num1_)
    {
    	return !(num1_); 
    }
};

//nothing to see here
template <size_t SIZE> 
class BitArray<SIZE>::BitProxy : public std::iterator
                                         <std::bidirectional_iterator_tag, bool>
{
public:
    explicit BitProxy(BitArray& arr_, size_t idx_);

    //BitProxy(const BitProxy& o_)=default;
    //~BitProxy() NOEXCEPT=default;

    BitProxy& operator=(const BitProxy& o_); //b[1]=b[2]
        
    BitProxy& operator=(bool val_); //b[1] = true
       
    operator bool() const; //implicit casting of BitProxy to bool
        
    //iterator operations
    BitProxy& operator++();
   
    BitProxy& operator--();
    
    BitProxy& operator++(int add_);
        
    bool operator==(BitProxy o_) const; 
        
    bool operator !=(BitProxy o_) const;
       
    bool operator*() const;    
    
private:
    BitProxy* operator& (); //=delete
    BitArray& m_bitarr;
    size_t m_index;
};

//bitarray implimentation
template <size_t SIZE>
BitArray<SIZE>::BitArray(): m_array() {}

template <size_t SIZE>
bool BitArray<SIZE>::operator==(const BitArray& o_) const
{
	return (std::equal(m_array, m_array + ARR_SIZE, o_.m_array)); 
}

template <size_t SIZE>
bool BitArray<SIZE>::operator!=(const BitArray& o_) const
{
	return !(this->operator==(o_)); 
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator|=(const BitArray& o_) 
{
	std::transform(o_.m_array, o_.m_array + ARR_SIZE, m_array, m_array, Or); 
	
	return *this; 
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator&=(const BitArray& o_) 
{
	std::transform(o_.m_array, o_.m_array + ARR_SIZE, m_array, m_array, And); 
	
	return *this; 
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator^=(const BitArray& o_) 
{
	std::transform(o_.m_array, o_.m_array + ARR_SIZE, m_array, 
	                                                m_array + ARR_SIZE, Xor); 
	
	return *this; 
}

template <size_t SIZE>
bool BitArray<SIZE>::operator[](size_t idx_) const NOEXCEPT
{
	return ((const_cast<BitArray<SIZE> *>(this))->operator[](idx_));
}

template <size_t SIZE>
class BitArray<SIZE>::BitProxy BitArray<SIZE>::operator[](size_t idx_) NOEXCEPT
{ 
	assert(SIZE > idx_); 
	
	BitProxy bp(*this , idx_); 	
	
	return bp; 	
}

template <size_t SIZE>
bool BitArray<SIZE>::At(size_t idx_) const
{//throws out of range
		if(SIZE < idx_)
		{
			throw std::out_of_range("index is out of range!");			
		}
		
		return (*this)[idx_];
}

template <size_t SIZE>
void BitArray<SIZE>::CleanForbidenBits()
{
	if(0 != SIZE%WORD_BITS)
	{
		size_t bit_num = WORD_BITS - SIZE; 
		
		//defined behaviour because unsigned intiger
		m_array[ARR_SIZE -1] = m_array[ARR_SIZE -1]>>bit_num; 
		
		m_array[ARR_SIZE -1] = m_array[ARR_SIZE -1]<<bit_num;  
	}
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::SetAll(bool val_)
{
	std::fill(m_array, m_array + ARR_SIZE, val_); 
	
	CleanForbidenBits(); 
	
	return *this; 
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::SetBit(size_t idx_, bool val_)
{
	assert(idx_ < SIZE);
	
	size_t arr_place =  (idx_ - (idx_%WORD_BITS))/WORD_BITS;
	
	size_t mask = (1 << (idx_ % WORD_BITS)); 
	
	if(true == val_)
	{
		m_array[arr_place] |= mask; 
	}
	else
	{
		m_array[arr_place] &= ~mask;
	}
	
	return *this; 
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::FlipAll()
{
	std::transform(m_array, m_array + ARR_SIZE, m_array, Not); 
	
	CleanForbidenBits(); 
	
	return *this; 
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::FlipBit(size_t idx_)
{
	SetBit(idx_, !((*this)[idx_])); 
	
	CleanForbidenBits(); 
	
	return *this; 
}

template <size_t SIZE>
size_t BitArray<SIZE>::CountSetBits() const
{
	return std::accumulate<BitProxy, int>(BitProxy(const_cast<BitArray&>(*this)
	                     , 0), BitProxy(const_cast<BitArray&>(*this), SIZE), 0); 
}

template <size_t SIZE>
char BitArray<SIZE>::MakeChar(const bool &b_)
{
    if(b_ == true)
    {
        return '1'; 
    }
    
    else
    {
        return '0'; 
    }
}

template <size_t SIZE>
std::string BitArray<SIZE>::ToString() const
{
    char res[SIZE + 1] = {'\0'};
    
    std::transform<BitProxy, char*, char(*)(const bool &b_)>
                                     (BitProxy(const_cast<BitArray&>(*this), 0),
                  BitProxy(const_cast<BitArray&>(*this) , SIZE), res, MakeChar); 
    
    return res; 
}

//bit proxy implimentation

template <size_t SIZE>
BitArray<SIZE>::BitProxy::BitProxy(BitArray& arr_, size_t idx_): 
                                                   m_bitarr(arr_), m_index(idx_) 
{}

template <size_t SIZE>
typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator=(
                        const typename BitArray<SIZE>::BitProxy& o_)
{
    m_bitarr.SetBit(m_index, o_.m_bitarr[m_index]);
        
    return *this; 
}

template <size_t SIZE>
typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator=
                                                                     (bool val_)
{
    m_bitarr.SetBit(m_index, val_); 
        
    return *this; 
}

template <size_t SIZE> 
bool BitArray<SIZE>::GetBit(size_t idx_) const
{
	size_t bit_place_in_arr_place = idx_%WORD_BITS; 

    size_t arr_place = ((idx_ - bit_place_in_arr_place)/WORD_BITS);
		
		
	return (((m_array[arr_place]) >> (WORD_BITS - bit_place_in_arr_place)) & MASK_ONE ); 
} 

template <size_t SIZE>
BitArray<SIZE>::BitProxy::operator bool() const //implicit casting of BitProxy to bool
{
    return (const_cast<BitArray&>(m_bitarr)).GetBit(m_index); 
}
    
template <size_t SIZE>
typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator++()
{
    ++m_index; 
        
    return *this; 
}     

template <size_t SIZE>
typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator--()
{
    --m_index; 
        
    return *this; 
}    
    
template <size_t SIZE>
typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator++(int add_)
{
    m_index += add_;
        
    return *this;  
}     
    
template <size_t SIZE>
bool BitArray<SIZE>::BitProxy::operator==(BitArray<SIZE>::BitProxy o_) const 
{
    return ((bool)(*this)) == ((bool)(o_)); 
}

template <size_t SIZE>    
bool BitArray<SIZE>::BitProxy::operator !=(BitArray<SIZE>::BitProxy o_) const
{
    return !(operator==(o_)); 
}

template <size_t SIZE>
bool BitArray<SIZE>::BitProxy::operator*() const
{
    return (bool)(*this); 
} 

}//ilrd

#endif /* ILRD_HRD28_BIT_ARRAY_HPP */
