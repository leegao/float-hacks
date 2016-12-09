
#ifndef FAST_INVERSE_CUBE_ROOT_EXP_H
#define FAST_INVERSE_CUBE_ROOT_EXP_H

#include "common.h"

namespace floathacks {

    inline constexpr float fexp(float x) {
        return l2f(static_cast<unsigned long>(x * (1/epsilon() + 0x38aa22)) + f2l(1));
    }

    inline constexpr float flog(float x) {
        return static_cast<float>(epsilon() * 0.6931471805599453 * (f2l(x) - f2l(1) + 0x66774));
    }
}

#endif //FAST_INVERSE_CUBE_ROOT_EXP_H
