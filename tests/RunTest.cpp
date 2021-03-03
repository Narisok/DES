#include <iostream>
#include "keys/KeysVerify.h"

int main()
{
    std::cout << "Keys verify test run: ";
    try {
        if(keysVerifiTests()) {
            std::cout << " PASSED " << std::endl;
        } else {
            std::cout << " FAILED " << std::endl;
        }
    } catch(...) {
        std::cout << " FAILED an exception has been catched!" << std::endl;
    }
}