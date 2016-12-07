#include <iostream>
#include "floathacks/pow.h"
#include "floathacks/consts/pow.h"

using namespace floathacks;

int main() {
    std::cout << std::get<0>(break_float(3.14)) << "/" << std::get<1>(break_float(3.14)) << std::endl;
    std::cout << std::get<1>(break_float(0.5)) << std::endl;
    std::cout << pow<FLOAT(0.5)>(2) << std::endl;
    std::cout << consts::pow(2, 0.5) << std::endl;
    std::cout << fpow<Ratio<1, 2>>::estimate(2) << std::endl;
//    std::cout << consts::fpow(20, -0.5) << std::endl;
//    std::cout << l2f(f2l(-3.256)) << std::endl;
    return 0;
}