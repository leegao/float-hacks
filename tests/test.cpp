#include <assert.h>
#include <cmath>
#include <iostream>
#include "../floathacks/hacks.h"

using floathacks::Ratio;

void assertWithinRange(float expected, float actual, float threshold = 1e-1) {
    assert(fabs(expected - actual) < threshold);
}

template <class R>
void test_pow() {
    std::cout << "Testing pow(n, " << R::value() << ");" << std::endl;
    for (float n = 0.1; n < 10; n += 0.01) {
        assertWithinRange(floathacks::fpow<R>::estimate(n), n);
    }
    std::cout << "Success!" << std::endl;
}

int main() {
    test_pow<Ratio<1, 1>>();
}