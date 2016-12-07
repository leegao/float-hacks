#ifndef FAST_INVERSE_CUBE_ROOT_COMMON_H
#define FAST_INVERSE_CUBE_ROOT_COMMON_H

#include <tuple>
#include <limits>
#include "consts/pow.h"

namespace floathacks {

    union lens { float float_view; unsigned long int_view; };
    union blens { unsigned long int_view; float float_view; };

    constexpr std::tuple<int, int> break_float(float f, int pow = 0, bool negate = false) {
        return f < 0 ? break_float(-f, pow, true) :
               (f - static_cast<int>(f) > 0 ?
                break_float(f * 10, pow + 1, negate) :
                std::make_tuple(static_cast<int>(negate ? -f : f),
                                static_cast<int>(consts::const_pow_(10.0f, pow))));
    }

    inline constexpr unsigned long f2l(float f) {
        return (lens { f }).int_view;
    }

    inline constexpr float l2f(unsigned long x) {
        return (blens { x }).float_view;
    }

    template<int num, int den>
    struct Ratio {
        constexpr static float value() { return static_cast<float>(num) / static_cast<float>(den); }
        constexpr static int n = num;
        constexpr static int d = den;
    };


#define FLOAT(f) floathacks::Ratio<std::get<0>(floathacks::break_float(f)), std::get<1>(floathacks::break_float(f))>
}

#endif //FAST_INVERSE_CUBE_ROOT_COMMON_H
