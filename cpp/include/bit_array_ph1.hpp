/*******************************************************************************
 * Project: BitArray
 * Author:  HRD28
 * Version: 1.0 - 16/05/2023
            1.1 - 16/05/2023: - NOEXCEPT with Dtor only
                              - We want to delete operator& on &bitset[i]
                               (BitProxy)
            1.2 - 17/05/2023: - fixed BitProxy to be template <size_t BITS_NUM>
                              - removed "vector" header
                              - added comments why we added cstddef, string
                                and boost/core/noncopyable.hpp
            1.3 - 17/05/2023: - added static variable for saving the size of
                                array of size_t, instead of MACROS  
*******************************************************************************/
#ifndef ILRD_HRD28_BIT_ARRAY_HPP
#define ILRD_HRD28_BIT_ARRAY_HPP

#include <cstddef> //size_t
#include <string>  //std::string
#include <climits> //CHAR_BIT

#include <boost/core/noncopyable.hpp> //private boost::noncpyable

#include "utils.hpp" //NOEXCEPT

#define WORD_BITS (sizeof(void*) * CHAR_BIT)

namespace ilrd
{
template <size_t BITS_NUM>
class BitArray: private boost::noncopyable
{
private:
    class BitProxy;

public:
    inline explicit BitArray();
    inline ~BitArray() NOEXCEPT;

    inline bool operator[](size_t where_) const;
    inline BitProxy operator[](size_t where_);
    inline bool GetBit(size_t where_) const; //throws std::out_of_range

    inline bool operator==(const BitArray& o_) const;
    inline bool operator!=(const BitArray& o_) const;

    inline BitArray& operator|=(const BitArray& o_);
    inline BitArray& operator&=(const BitArray& o_);
    inline BitArray& operator^=(const BitArray& o_);

    inline BitArray& SetAll(bool val_);
    inline BitArray& SetBit(size_t where_, bool val_);

    inline BitArray& FlipAll();
    inline BitArray& FlipBit(size_t where_);

    inline size_t CountSetBits() const;

    inline std::string ToString() const; //throws std::bad_alloc

private:
    size_t m_array[(((BITS_NUM - (BITS_NUM % WORD_BITS))) / WORD_BITS) + 
                                                      !!(BITS_NUM % WORD_BITS)];
    static const size_t s_arr_size;
};

//nothing to see here

template <size_t BITS_NUM>
const size_t BitArray<BITS_NUM>::s_arr_size = (((BITS_NUM - 
               (BITS_NUM % WORD_BITS))) / WORD_BITS) + !!(BITS_NUM % WORD_BITS);

template <size_t BITS_NUM>
class BitProxy
{
public:
    explicit BitProxy(const BitArray<BITS_NUM>& arr_, size_t where_);
    BitProxy(const BitProxy& o_);
    ~BitProxy() NOEXCEPT;

    BitProxy& operator=(const BitProxy& o_);
    BitProxy& operator=(bool bit_);

    operator bool() const;

private:
	BitProxy* operator& (); //=delete
    BitArray<BITS_NUM>& m_bitarr;
    size_t m_index;
};

BitArray<BITS_NUM>::BitArray() : m_array() {}

BitArray<BITS_NUM>::~BitArray() {}

bool BitArray<BITS_NUM>::operator[](size_t where_)
{//rely on implicit casting between bool and BitProxy
    
    return (const_cast<BitArray *>(this)).operator[](where_); 
}

BitProxy BitArray<BITS_NUM>::operator[](size_t where_)
{
    return BitProxy(this, where_);
}

bool BitArray<BITS_NUM>::GetBit(size_t where_)
{
    return 
}


}// namespace ilrd

#endif /* ILRD_HRD28_BIT_ARRAY_HPP */
