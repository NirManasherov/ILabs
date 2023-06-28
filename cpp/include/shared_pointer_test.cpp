#include <iostream>

#include "shared_pointer.hpp"

using namespace ilrd;

struct test_struct
{
    int a;
    size_t b;
};

int main()
{
    int *x = new int(5);

    SharedPointer<int> sp1(x);

    std::cout << "*sp1 = " << *sp1 << std::endl;

    SharedPointer<int> sp2(sp1);

    std::cout << "*sp2 = " << *sp2 << std::endl;

    SharedPointer<void *> sp3(NULL);

    if(!sp1)
    {
        std::cerr << "! isnt working!" << std::endl;
    }

    if(!sp3)
    {
        std::cout << "! is working!" << std::endl;
    }

    std::cout << "sp1 == sp2 -> " << (sp1 == sp2) << std::endl; //true

    std::cout << "sp1 != sp2 -> " << (sp1 != sp2) << std::endl; //false

    int *more_x = new int(8);

    SharedPointer<int> sp4(more_x);

    std::cout << "*sp4 = " << *sp4 << std::endl; //8

    sp4 = sp2;

    std::cout << "*sp4 = " << *sp4 << std::endl; //5

    struct test_struct y = {5,4};

    SharedPointer<struct test_struct> sp5(&y);

    std::cout << "sp5->a = " << (sp5->a) << std::endl;

    delete x;

    delete more_x;

    return 0;
}
