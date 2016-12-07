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

In a similar vein, a fast inverse cube-root method is presented at the start of this page.
$$
\textrm{l2f}\left(\text{0x54a2fa8c} - \frac{\textrm{f2l}(x)}{3}\right) \approx \frac{1}{\sqrt[3]{x}}
$$

We will justify this in the next section.

### Arbitrary Powers

We can approximate any $x^c$ using just integer arithmetic on the machine representation of $x$. To do
so, compute
$$
\textrm{l2f}\left(M_c + c \times \textrm{f2l}(x)\right)
$$
where $M_c = (1 - c) \cdot (\textrm{f2l}(1) + \textrm{bias})$. In general, any value of `bias`, as long
as it is reasonably small, will work. At `bias = 0`, the method computes a value whose error is completely positive.
Therefore, by increasing the bias, we can shift some of the error down into the negative plane and
halve the error. 

As seen in the fast-inverse-root method, a bias of `-0x5c416` tend to work well for pretty much every case that I've
tried, as long as we tack on at least one Newton refinement stage at the end. It works well without refinement as well,
but an even bias of `-0x5c000` works even better.

Why does this work? See [these slides](http://www.bullshitmath.lol/FastRoot.slides.html) for the derivation. In
particular, the fast inverse square-root is a subclass of this method.

### Exp

We can approximate $\exp(x)$ up to $x = 90$ using a similar set of bit tricks. I'll first give its equation, and then
give its derivations. As far as I am aware, these are original. However, since there are no refinement methods
for the computation of $\exp(x)$, there is practically no reason to ever resort to this approximation unless you're okay
with 10% error.

$$
\exp(x) \approx \textrm{l2f}\left(x \times (\epsilon^{-1} + \textrm{0x2df854}) + \textrm{f2l}(1)\right)
$$

Here, $\epsilon$ is the [machine epsilon](https://en.wikipedia.org/wiki/Machine_epsilon) for single precision, and it
is computed by $\textrm{l2f}(1)$.

-------------------------------------

For more information on how the constant (`0x54a2fa8c`) is derived for
the cube-root, visit http://www.bullshitmath.lol/.

Equations rendered with [readme2tex](https://github.com/leegao/readme2tex).