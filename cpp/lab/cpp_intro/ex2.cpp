#include <iostream>

enum E {AA , BB , CC, DD}; 

enum E2 {TT, KK};

/*enum E e=2;*/ 

/*enum E2 e2 = AA;*/ 

//E2 e22 = KK;

/*++e;*/

/*int i = e;*/

int main(void)
{
    std::cout << "AA: " << AA << "\n"; 
    std::cout << "BB: " << BB << "\n"; 
    std::cout << "CC: " << CC << "\n";  
    std::cout << "DD: " << DD << "\n";  
    std::cout << "TT: " << TT << "\n"; 
    std::cout << "KK: " << KK << "\n";   
}
