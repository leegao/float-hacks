#include <cmath>
#include <iostream>
#include <chrono>
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

template <class R>
void test_time_pow(float s) {
    std::cout << "Time test for pow(" << R::value() << ")" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    float sum = 0;
    for (float n = s; n < 100000; n += 0.01) {
        sum += floathacks::fpow<R>::estimate(n);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << sum << std::endl;
    std::cout << "floathacks::fpow Took " << duration << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    for (float n = s; n < 100000; n += 0.01) {
        sum += powf(n, R::value());
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << sum << std::endl;
    std::cout << "std::pow Took " << duration << " ms" << std::endl;
}

void test_time_exp(float s) {
    std::cout << "Time test for exp(n)" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    float sum = 0;
    for (float n = s; n < 100000; n += 0.01) {
        sum += floathacks::fexp(n);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << sum << std::endl;
    std::cout << "floathacks::fexp Took " << duration << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    for (float n = s; n < 100000; n += 0.01) {
        sum += expf(n);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << sum << std::endl;
    std::cout << "std::exp Took " << duration << " ms" << std::endl;
}

void test_time_log(float s) {
    std::cout << "Time test for log(n)" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    float sum = 0;
    for (float n = s; n < 100000; n += 0.01) {
        sum += floathacks::flog(n);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << sum << std::endl;
    std::cout << "floathacks::flog Took " << duration << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    for (float n = s; n < 100000; n += 0.01) {
        sum += expf(n);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << sum << std::endl;
    std::cout << "std::log Took " << duration << " ms" << std::endl;
}

int main() {
    test_pow<Ratio<1, 1>>();
    test_pow<FLOAT(-0.5f)>();
    test_pow<FLOAT(2)>();
    test_pow<FLOAT(5)>(2e-1); // relative error is a bit higher
    test_pow<FLOAT(0.2)>();
    test_exp();
    test_log();
    float start = (float)(rand() % 10) / 10.0f;
    test_time_pow<FLOAT(2)>(start);
    test_time_pow<FLOAT(-0.5f)>(start);
    test_time_pow<FLOAT(5)>(start);
    test_time_exp(start);
    test_time_log(start);
}