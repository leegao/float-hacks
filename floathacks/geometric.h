#ifndef FAST_INVERSE_CUBE_ROOT_GEOMETRIC_H
#define FAST_INVERSE_CUBE_ROOT_GEOMETRIC_H

#include <algorithm>
#include "common.h"

namespace floathacks {

    template<int n>
    inline constexpr float fgmean(std::array<float, n> input) {
        return l2f(std::accumulate(
                input.begin(),
                input.end(),
                0ul,
                [](unsigned long x, float y) -> unsigned long {
                    return x + f2l(y);
                }) / n);
    }

    inline float fgmean(std::vector<float> input) {
        return l2f(std::accumulate(
                input.begin(),
                input.end(),
                0ul,
                [](unsigned long x, float y) -> unsigned long {
                    return x + f2l(y);
                }) / input.size());
    }

}

#endif //FAST_INVERSE_CUBE_ROOT_GEOMETRIC_H
