#include <iostream>
#include <cmath>
#include "floathacks/hacks.h"

using namespace floathacks;

int main() {
    // More precise (2 newton iterations)
    std::cout << pow<FLOAT(-0.5f)>(3.1415926f) << ", expected " << std::pow(3.1415926f, -0.5f) << std::endl;
    std::cout << consts::pow(3.1415926f, -0.5f) << ", expected " << std::pow(3.1415926f, -0.5f) << std::endl;
    // Raw guesses
    std::cout << fpow<FLOAT(-0.5f)>::estimate(3.1415926f) << ", expected " << std::pow(3.1415926f, -0.5f) << std::endl;
    std::cout << consts::fpow(3.1415926f, -0.5f) << ", expected " << std::pow(3.1415926f, -0.5f) << std::endl;
    // Exp guess
    std::cout << fexp(10.0f) << ", expected " << std::exp(10.0f)  << std::endl;
    // Log guess
    std::cout << flog(10.0f) << ", expected " << std::log(10.0f)  << std::endl;
    return 0;
}