#include <iostream>

#include "bit_array.hpp"
#include "utils.hpp"

using namespace ilrd;

int main()
{
    BitArray<128> b1;

    BitArray<128> b2;

    std::cout << "b1[0] = " << b1.At(0) << std::endl;

    std::cout << "b1[64] = " << b1.At(64) << std::endl;

    std::cout << "b1[1] = " << b1.At(1) << std::endl;

    std::cout << "b1[127] = " << b1.At(127) << std::endl;

    std::cout << "b1 == b2 ->" << (b1 == b2) << std::endl;

    std::cout << "b1 != b2 ->" << (b1 != b2) << std::endl;

    std::cout<< "before change b1[3] = " << b1[3] << std::endl;

    if (b1[3])
    {
        std::cerr << "shouldnt be here" << std::endl;
    }

    b1.SetBit(3,true);

    std::string s_b1 = b1.ToString();

    std::cout<< "b1 = " << s_b1 << std::endl;

    size_t one_bit_on = b1.CountSetBits();

    std::cout << "countsetbits for one_bit_on is = " << one_bit_on << std::endl;

    b2[5] = true;

    std::cout << "before &= b2[5] = " << b2[5] << std::endl;

    b2 &= b1;

    std::cout << "after &= the answer for b2[5] = " << b2[5] << std::endl;

    if (b1[3])
    {
        std::cout << "we are in if (b1[3]) with val = " << b1[3] << std::endl;
    }

    std::cout<< "after change b1[3] = " << b1[3] << std::endl;

    std::cout<< "before change b2[4] = " << b2[4] << std::endl;

    b2[4] = true;

    std::cout<< "after change b2[4] = " << b2[4] << std::endl;

    b2[4] = false;

    std::cout<< "after more change b2[4] = " << b2[4] << std::endl;

    std::cout << "b1 == b2 ->" << (b1 == b2) << std::endl;

    std::cout << "b1 != b2 ->" << (b1 != b2) << std::endl;

    BitArray<7> b3;

    //error: no match for ‘operator==’ - GOOD!
    //b3 == b1;
     
    std::cout << "b1[4] = " << b1[3] << std::endl;

    BitArray<7> b4;

    b4[0] = b1[3];

    std::cout << "before |= b4[0] = " << b4[0] << std::endl;

    std::cout << "before |= b3[0] = " << b3[0] << std::endl;

    b3 |= b4;

    std::cout << "b3 |= b4 result in b3[0] = " << b3[0] << std::endl;

    b4[0] = true;

    b3 ^= b4;

    std::cout << "b3 ^= b4 result (true ^ true) in b3[0] = " << b3[0] << std::endl;

    if (!b3[0]) //b3[0] == false
    {
        std::cout << "! on CharProxy works!" << std::endl;
    }

    BitArray<64> all_flipped;

    all_flipped.FlipAll();

    if (all_flipped[63] && all_flipped[0] && all_flipped[33])
    {
        std::cout << "flipped succeed!" << std::endl;
    }

    BitArray<64> all_zero;

    std::cout << "all_flipped == all_zero is " << (all_flipped == all_zero) << std::endl;

    size_t ret_all_off = all_zero.CountSetBits();

    std::cout << "countsetbits for all_zero is = " << ret_all_off << std::endl;    
    
    all_zero.FlipBit(63);

    if (false == !all_zero[63])
    {
        std::cout << "Flip Bit workd!" << std::endl;
    }

    BitArray<64> all_set_all;

    all_set_all.SetAll(true);

    std::cout << "all_flipped == all_set_all is " << (all_flipped == all_set_all) << std::endl;

    size_t ret_all_on = all_set_all.CountSetBits();

    std::cout << "countsetbits for all_set_all is = " << ret_all_on << std::endl;
    
    BitArray<128> b5;

    b5.FlipAll();

    size_t two_words_on = b5.CountSetBits();

    std::cout << "countsetbits for b5 is = " << two_words_on << std::endl;

    std::string s_all_on = b5.ToString();

    std::cout<< "b5 = " << s_all_on << std::endl;

    return 0;
}
