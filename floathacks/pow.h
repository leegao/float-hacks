#ifndef FAST_INVERSE_CUBE_ROOT_STATIC_POW_H
#define FAST_INVERSE_CUBE_ROOT_STATIC_POW_H

#include "common.h"

namespace floathacks {
    template <int num>
    inline float static_pow_(float b) {
        float base = b;
        if (num < 0) base = 1 / base;
        unsigned int n = static_cast<unsigned int>(num >= 0 ? num : -num);
        float result = 1;
        while (n) {
            if (n % 2) result *= base;
            base *= base;
            n /= 2;
        }
        return result;
    }

    template <int n, int d>
    inline float static_pow_(float base) {
        union lens lens = { base };
        constexpr float c = static_cast<float>(n)/static_cast<float>(d);
        constexpr long long M = static_cast<long long>((1.0f - c) * (0x3f800000 - 0x5c416));
        lens.int_view = static_cast<unsigned long>(static_cast<long long>(lens.int_view * c) + M);
        return lens.float_view;
    }

    template <class F>
    struct fpow {
        static_assert(F::value, "Must pass in a floathacks::Ratio");
        static_assert(F::n, "Must pass in a floathacks::Ratio");
        static_assert(F::d, "Must pass in a floathacks::Ratio");
        inline static float estimate(float base) {
            return F::d ? static_pow_<F::n, F::d>(base) : static_pow_<F::n>(base);
        }
    };

    template <class F, int iterations = 2>
    inline float pow(float base) {
        double estimate = fpow<Ratio<1, F::d>>::estimate(base);
        for (int i = 0; i < iterations; i++) {
            estimate -= estimate/F::d - base / (F::d * static_pow_<F::d - 1>((float) estimate));
        }
        return static_pow_<F::n>(static_cast<float>(estimate));
    }
}

#endif //FAST_INVERSE_CUBE_ROOT_STATIC_POW_H
