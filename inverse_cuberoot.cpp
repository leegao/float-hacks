#include <iostream>
#include "floathacks/pow.h"
#include "floathacks/consts/pow.h"

using namespace floathacks;

int main() {
    // More precise (2 newton iterations)
    std::cout << pow<FLOAT(-0.5f)>(3.1415926f) << std::endl;
    std::cout << consts::pow(3.1415926f, -0.5f) << std::endl;
    // Raw guesses
    std::cout << fpow<FLOAT(-0.5f)>::estimate(3.1415926f) << std::endl;
    std::cout << consts::fpow(3.1415926f, -0.5f) << std::endl;
    return 0;
}