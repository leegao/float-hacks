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
is computed by $\textrm{l2f}(\textrm{f2l}(1) + 1) - 1$.

To give a derivation of this equation, we'll need to borrow a few mathematical tools from analysis. In particular, while
`l2f` and `f2l` have many discontinuities ($2^8$ of them to be exact), it is mostly smooth. This
carries over to its "rate-of-change" as well, so we will just pretend that it has mostly smooth derivatives
everywhere.

#### Differentiating the `l2f` and `f2l` functions.
Consider the function
$$
\frac{\partial \textrm{f2l}(f(z))}{\partial z} = \textrm{f2l}'(f(z)) \cdot f'(z)
$$
where the equality is a consequence of the chain-rule, assuming that `f2l` is differentiable at the particular value of
$f(z)$. Now, this raises an interesting question: What does it mean to take a derivative of $\textrm{f2l}(x)$?

Well, it's not all that mysterious. The derivative of `f2l` is just the rate at which a number's IEEE 754 machine
representation changes as we make small perturbations to a number. Unfortunately, while it might be easy to compute
this derivative as a numerical approximation, we still don't have an approximate form for algebraic manipulation.

While $\textrm{f2l}'(x)$ might be difficult to construct, we can fair much better with its sibling, $\textrm{l2f}'(x)$.
Now, the derivative $\textrm{l2f}'(x)$ is the rate that a float will change given that we make small perturbations
to its machine representation. However, since its machine representations are all bit-vectors, it doesn't make sense
to take a derivative here since we can't make these perturbations arbitrarily small. The smallest change we can make
is to either add or subtract one. However, if we just accept our fate, then we can define the "derivative" as the finite
difference
$$
\textrm{l2f}'(x) \approx \frac{\textrm{l2f}(x + 1) - \textrm{l2f}(x)}{1}
$$
where
\begin{align*}
\textrm{l2f}(x + 1) &= \textrm{l2f}(x) + \epsilon \times 2^{\lfloor\log_2(\textrm{l2f}(x))\rfloor} \\
&\approx \textrm{l2f}(x) + \epsilon \times \textrm{l2f}(x)
\end{align*}

Here, equality holds when $\textrm{l2f}(x)$ is a perfect power of $2$ (including fractions of the form $2^{-k}$).

Therefore,
$$
\textrm{l2f}'(x) \approx \epsilon \times \textrm{l2f}(x)
$$

From here, we also have
\begin{align*}
\textrm{f2l}(\textrm{l2f}(z)) &= z \\
\frac{\partial \textrm{f2l}(\textrm{l2f}(z))}{\partial z} &= \frac{dz}{dz} \\
\textrm{f2l}'(\textrm{l2f}(z)) \textrm{l2f}'(z) &= 1 & \text{chain rule}\\
\textrm{f2l}'(\textrm{l2f}(z)) &= \frac{1}{\textrm{l2f}'(z)} \\
&\approx \frac{1}{\epsilon \times \textrm{l2f}(z)} & \text{since }\textrm{l2f}'(z) = \epsilon \textrm{l2f}(z)\\
\textrm{f2l}'(x) &\approx \frac{\epsilon^{-1}}{x} & \text{by substituting } x = \textrm{l2f}(z)
\end{align*}

#### A Tale of Two Functions

Given $\textrm{f2l}'(x) \approx \frac{\epsilon^{-1}}{x}$, antidifferentiating both sides gives
$$
\textrm{f2l}(x) \approx \epsilon^{-1} \log(x) + C
$$

Similarly, since $\textrm{l2f}'(x) \approx \epsilon \textrm{l2f}(x)$ satisfies $y' \approx \epsilon y$, we have
$$
\textrm{l2f}(x) \approx c \exp(\epsilon x)
$$

This makes sense, since we'd like these two functions to be inverses of each other.

#### Exp, redux.

Consider
$$
\textrm{f2l}(\exp(x)) \approx \epsilon^{-1} \log(\exp(x)) + C
$$
which suggests that $\exp(x) \approx \textrm{l2f}\left(\epsilon^{-1} x + C\right)$.

Since we would like $\exp(0) = 1$, we can impose the boundary condition
$$
\textrm{l2f}(\textrm{f2l}(1)) = \textrm{l2f}\left(\epsilon^{-1} \times 0 + C\right) = \textrm{l2f}(C)
$$
which gives $C = \textrm{f2l}(1)$. However, while this method gives bounded relative error, in its unbiased form
this is pretty off the mark for general purposes (it approximates some other $c^x$). Instead, we can add in an unbiased form:
$$
\exp(x) \approx \textrm{l2f}\left((\epsilon^{-1} + b)x + \textrm{f2l}(1)\right)
$$
where, empirically, $b = \textrm{0x38aa22}$ gives a good approximation. Notice that the $b$ we've chosen is close to
$(2\epsilon)^{-1}$, which is what we need to transform $c^x$ to $e^x$.
In particular, for all $n$, the $L^1$, $L^2$, and $L^{\infty}$ relative error is always below 10%.

<p align="center">![exp](http://i.imgur.com/Kr7dcSz.png)</p>

### Log

In a similar spirit, we can use the approximation
$$
\textrm{f2l}(x) \approx \epsilon^{-1} \log(x) + C
$$
to derive
$$
\log(x) \approx \epsilon \times \left(\textrm{f2l}(x) - C\right)
$$
Imposing a boundary condition at $x = 1$ gives $C = \textrm{f2l}(1)$, so we should expect
$$
\log(x) \approx \epsilon \times \left(\textrm{f2l}(x) - \textrm{f2l}(1)\right)
$$

However, this actually computes some other logarithm $\log_c(x)$, and we'll have to, again, unbias this term
$$
\log(x) \approx \epsilon \times \left(\textrm{f2l}(x) - \textrm{f2l}(1) + b\right) \times \log_2(x)
$$

where the $\log_2(x)$ term came from the fact that the base computation approximates the 2-logarithm. Empirically, I've
found that a bias of $b = \textrm{0x66774}$ works well. In particular, for all $n$, the $L^1$,
$L^2$, and $L^{\infty}$ relative error is always below 10%.

<p align="center"> ![log](http://i.imgur.com/TsjGPwc.png) </p>

-------------------------------------

For more information on how the constant (`0x54a2fa8c`) is derived for
the cube-root, visit http://www.bullshitmath.lol/.

Equations rendered with [readme2tex](https://github.com/leegao/readme2tex).