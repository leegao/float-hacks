# Floating Point Hacks

### Completely useless, but fun nevertheless.

$$
\textrm{l2f}\left(\text{0x54a2fa8c} - \frac{\textrm{f2l}(x)}{3}\right) \approx \frac{1}{\sqrt[3]{x}}
$$

<sub>*Equations for a "fast" inverse cube-root method.*</sub>

------------------------------------

This repository contains a set of procedures to compute numerical methods in the vein of the
[fast inverse root method](https://en.wikipedia.org/wiki/Fast_inverse_square_root). In particular,
we will generate code that

1. Computes rational powers ($x^{\frac{a}{b}}$) to an arbitrary precision.
2. Computes irrational powers ($x^c$) to within 10% relative error.
3. Computes $\exp(x)$ to within 10% relative error.
4. Computes $\log(x)$ to within 10% relative error.

Additionally, we will do so using mostly just integer arithmetic.

## Justification

### Prelude

The key ingredient of these types of methods is the pair of transformations
$\textrm{f2l}(x) : \textrm{float} \to \textrm{long}$ and 
$\textrm{l2f}(x) : \textrm{long} \to \textrm{float}$.

* $\textrm{f2l}(x)$ takes a `IEEE 754` single precision floating point number and outputs its "machine" representation.
In essence, it acts like

      unsigned long f2l(float x) {
        union {float fl; unsigned long lg;} lens = { x };
        return lens.lg;
      }
* $\textrm{l2f}(z)$ takes an unsigned long representing a float and returns a `IEEE 754` single precision floating point number.
It acts like

      float l2f(unsigned long z) {
        union {float fl; unsigned long lg;} lens = { z };
        return lens.fl;
      }

So for example, the fast inverse root method:

      union {float fl; unsigned long lg;} lens = { x };
      lens.lg = 0x5f3759df - lens.lg / 2;
      float y = lens.fl;

can be equivalently expressed as
$$
\textrm{l2f}\left(\textrm{0x5f3759df} - \frac{\textrm{f2l}(x)}{2}\right)
$$

### Arbitrary Powers



For more information on how the constant (`0x54a2fa8c`) is derived for
the cube-root, visit http://www.bullshitmath.lol/.