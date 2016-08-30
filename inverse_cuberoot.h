#ifndef FAST_INVERSE_CUBE_ROOT_CUBEROOT_H
#define FAST_INVERSE_CUBE_ROOT_CUBEROOT_H

inline float inverse_cuberoot(float x) {
    union {float float_view; unsigned int int_view;} lens = { x };
    // The magic constant balances the bias after a single newton step
    // For more, visit http://bullshitmath.lol/
    lens.int_view = 0x54a2fa8c - lens.int_view / 3;
    float y = lens.float_view;
    // Two steps of newton's to get the relative error down to 1e-6
    y = (2.0f / 3) * y  + 1 / (3.0f * y * y * x);
    y = (2.0f / 3) * y  + 1 / (3.0f * y * y * x);
    return y;
}

#endif //FAST_INVERSE_CUBE_ROOT_CUBEROOT_H
