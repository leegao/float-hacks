# Floating Point Hacks

### Completely useless, but fun nevertheless.

$$
\textrm{l2f}\left(\text{0x54a2fa8c} - \frac{\textrm{f2l}(x)}{3}\right) \approx \frac{1}{\sqrt[3]{x}}
$$

------------------------------------

This repository contains a set of procedures to compute numerical methods in the vein of the
[fast inverse root method](https://en.wikipedia.org/wiki/Fast_inverse_square_root). In particular,
we will generate code that

1. Computes rational powers ($x^{\frac{a}{b}}$) to an arbitrary precision.
2. Computes irrational powers ($x^c$) to within 10% relative error.
3. Computes $\exp(x)$ to within 10% relative error.
4. Computes $\log(x)$ to within 10% relative error.

Additionally, we will do so using mostly just integer arithmetic.

For more information on how the constant (`0x54a2fa8c`) is derived for
the cuberoot, visit http://www.bullshitmath.lol/.