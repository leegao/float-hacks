#include <cmath>
#include <iostream>
#include "../floathacks/hacks.h"

using floathacks::Ratio;

inline bool assertWithinRange(float expected, float actual, float threshold = 1e-1) {
    return fabs(expected - actual)/fabs(actual) < threshold;
}

template <class R>
void test_pow(float threshold = 1e-1) {
    std::cout << "Testing pow(n, " << R::value() << ");" << std::endl;
    for (float n = 0.1; n < 10; n += 0.01) {
        if (!assertWithinRange(floathacks::fpow<R>::estimate(n), powf(n, R::value()), threshold)) {
            std::cout << "Test failed for pow(" << n << ", " << R::value() << ");"
                      << " expected " << powf(n, R::value())
                      << ", but got " << floathacks::fpow<R>::estimate(n) << " instead" << std::endl;
            return;
        }
    }
    std::cout << "Success!" << std::endl;
}

void test_exp(float threshold = 1e-1) {
    std::cout << "Testing exp(n);" << std::endl;
    for (float n = 0.1; n < 10; n += 0.01) {
        if (!assertWithinRange(floathacks::fexp(n), expf(n), threshold)) {
            std::cout << "Test failed for exp(" << n << ");"
                      << " expected " << exp(n)
                      << ", but got " << floathacks::fexp(n) << " instead" << std::endl;
            return;
        }
    }
    std::cout << "Success!" << std::endl;
}

void test_log(float threshold = 1e-1) {
    std::cout << "Testing log(n);" << std::endl;
    for (float n = 1.1; n < 10; n += 0.01) {
        if (!assertWithinRange(floathacks::flog(n), logf(n), threshold)) {
            std::cout << "Test failed for log(" << n << ");"
                      << " expected " << logf(n)
                      << ", but got " << floathacks::flog(n) << " instead" << std::endl;
            return;
        }
    }
    std::cout << "Success!" << std::endl;
}

int main() {
    test_pow<Ratio<1, 1>>();
    test_pow<FLOAT(-0.5f)>();
    test_pow<FLOAT(2)>();
    test_pow<FLOAT(5)>(2e-1); // relative error is a bit higher
    test_pow<FLOAT(0.2)>();
    test_exp();
    test_log();
}