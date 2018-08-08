// This file is a test for the library.
// On Linux, compile using: `g++ -shared -o sum.so -fPIC sum.cpp`


#include <iostream>
#include "../clusterize/clusterize.h"


CLUSTERIZE_MAIN(identifier, argumentorinos) {
    for (const auto& x: argumentorinos)
        std::cout << x;
}