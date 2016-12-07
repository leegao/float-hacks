#ifndef FAST_INVERSE_CUBE_ROOT_CONST_POW_H
#define FAST_INVERSE_CUBE_ROOT_CONST_POW_H

#include "../common.h"

namespace floathacks {
    inline constexpr float l2f(unsigned long x);
    inline constexpr unsigned long f2l(float f);
    constexpr std::tuple<int, int> break_float(float f);
}

namespace floathacks { namespace consts {
    inline constexpr float const_pow_(float b, int num);

    inline constexpr float const_pow_(float b, int num) {
        return (!num) ?
               1 :
               ((num < 0) ? 1/b : b) * const_pow_(b, (num < 0) ? num + 1 : num - 1);
    }

    inline constexpr float fpow(float base, float c) {
        return c - static_cast<int>(c) == 0
               ? const_pow_(base, static_cast<int>(c))
               : l2f(static_cast<unsigned long>(
                           static_cast<long long>(f2l(base) * c) + static_cast<long long>((1.0f - c) * (0x3f800000))));
    }

    inline constexpr float refine(float base, float estimate, int num, int den, unsigned iterations);

    inline constexpr float pow(float base, float c, unsigned iterations = 2) {
        return refine(
                base,
                fpow(base, 1.f/std::get<1>(break_float(c))),
                std::get<0>(break_float(c)),
                std::get<1>(break_float(c)),
                iterations);
    }

    inline constexpr float refine(float base, float estimate, int num, int den, unsigned iterations) {
        return iterations ?
                refine(
                        base,
                        estimate - (estimate / den - base / (den * const_pow_(estimate, den - 1))),
                        num, den,
                        iterations - 1):
                const_pow_(estimate, num);
    }
}}

#endif //FAST_INVERSE_CUBE_ROOT_CONST_POW_H
