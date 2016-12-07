# Floating Point Hacks

### Completely useless, but fun nevertheless.

<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/690b9db878741ab50045c64c303ad998.svg" valign=0px width=285.184pt height=47.6424pt/></p>

<sub>*Equations for a "fast" inverse cube-root method.*</sub>

------------------------------------

This repository contains a set of procedures to compute numerical methods in the vein of the
[fast inverse root method](https://en.wikipedia.org/wiki/Fast_inverse_square_root). In particular,
we will generate code that

1. Computes rational powers (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/d49c11cb1a1f5ee6a6e609ab67b4f6bb.svg" valign=0px width=21.451599999999996pt height=16.85872pt/>) to an arbitrary precision.
2. Computes irrational powers (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1bed8656510c12b49e4a47c9a81e78ac.svg" valign=0px width=18.439399999999996pt height=13.21354pt/>) to within 10% relative error.
3. Computes <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/559b96359a4653a6c35dbf27c11f68d2.svg" valign=-4.962839999999996px width=57.22619999999999pt height=19.85084pt/> to within 10% relative error.
4. Computes <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/38f816ed8d9782e71ecfd164e77c5150.svg" valign=-4.962839999999996px width=52.5216pt height=19.85084pt/> to within 10% relative error.

Additionally, we will do so using mostly just integer arithmetic.

## Justification

### Prelude

The key ingredient of these types of methods is the pair of transformations
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/b46a9543f58a8f4ef51bf513d0195b76.svg" valign=-4.962839999999996px width=171.287pt height=19.85084pt/> and 
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/8b1eb6ddb44c5ea9346105ed36f25424.svg" valign=-4.962839999999996px width=171.003pt height=19.85084pt/>.

* <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/014a505b8ec0a2e3dd98b6c8577cbdea.svg" valign=-4.962839999999996px width=48.370599999999996pt height=19.85084pt/> takes a `IEEE 754` single precision floating point number and outputs its "machine" representation.
In essence, it acts like


        unsigned long f2l(float x) {
          union {float fl; unsigned long lg;} lens = { x };
          return lens.lg;
        }
* <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/2484e1c18e94550c31824679fd30291d.svg" valign=-4.962839999999996px width=47.133199999999995pt height=19.85084pt/> takes an unsigned long representing a float and returns a `IEEE 754` single precision floating point number.
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
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1956bacf4be82337ef506b5f97ad6590.svg" valign=0px width=225.274pt height=47.6424pt/></p>

In a similar vein, a fast inverse cube-root method is presented at the start of this page.
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/690b9db878741ab50045c64c303ad998.svg" valign=0px width=285.184pt height=47.6424pt/></p>

We will justify this in the next section.

### Arbitrary Powers

We can approximate any <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1bed8656510c12b49e4a47c9a81e78ac.svg" valign=0px width=18.439399999999996pt height=13.21354pt/> using just integer arithmetic on the machine representation of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/332cc365a4987aacce0ead01b8bdcc0b.svg" valign=0px width=11.345199999999998pt height=8.54688pt/>. To do
so, compute
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/85325d7348ff0db8a0a89d53e9cec59f.svg" valign=0px width=173.3018pt height=19.85084pt/></p>
where <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/763af86a1c2d443760aeb707325ee397.svg" valign=-4.962839999999996px width=247.61639999999997pt height=19.85084pt/>. In general, any value of `bias`, as long
as it is reasonably small, will work. At `bias = 0`, the method computes a value whose error is completely positive.
Therefore, by increasing the bias, we can shift some of the error down into the negative plane and
halve the error. 

As seen in the fast-inverse-root method, a bias of `-0x5c416` tend to work well for pretty much every case that I've
tried, as long as we tack on at least one Newton refinement stage at the end. It works well without refinement as well,
but an even bias of `-0x5c000` works even better.

Why does this work? See [these slides](http://www.bullshitmath.lol/FastRoot.slides.html) for the derivation. In
particular, the fast inverse square-root is a subclass of this method.

### Exp

We can approximate <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/559b96359a4653a6c35dbf27c11f68d2.svg" valign=-4.962839999999996px width=57.22619999999999pt height=19.85084pt/> up to <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/b921a412dbb057d50cc70ebb4f18df62.svg" valign=0px width=57.705pt height=12.79276pt/> using a similar set of bit tricks. I'll first give its equation, and then
give its derivations. As far as I am aware, these are original. However, since there are no refinement methods
for the computation of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/559b96359a4653a6c35dbf27c11f68d2.svg" valign=-4.962839999999996px width=57.22619999999999pt height=19.85084pt/>, there is practically no reason to ever resort to this approximation unless you're okay
with 10% error.

<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/dd860a34346bda9f243b20a8f2525b7f.svg" valign=0px width=380.43pt height=24.0696pt/></p>

Here, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/7ccca27b5ccc533a2dd72dc6fa28ed84.svg" valign=0px width=8.057379999999998pt height=8.54688pt/> is the [machine epsilon](https://en.wikipedia.org/wiki/Machine_epsilon) for single precision, and it
is computed by <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/64885526322d9df034356737808e2ea7.svg" valign=-4.962839999999996px width=46.950799999999994pt height=19.85084pt/>.

To give a derivation of this equation, we'll need to borrow a few mathematical tools from analysis. In particular, while
`l2f` and `f2l` have many discontinuities (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/4f3b183ca15faf05b91c6ad7e3b6d7a5.svg" valign=0px width=17.838199999999997pt height=16.18566pt/> of them to be exact), it is mostly smooth. This
carries over to its "rate-of-change" as well, so we will just pretend that it has mostly smooth derivatives
everywhere.

Consider the function
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/12d8e344454774536a8da176b6cc638d.svg" valign=0px width=252.606pt height=42.0354pt/></p>
where the equality is a consequence of the chain-rule, assuming that `f2l` is differentiable at the particular value of
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/210d22201f1dd53994dc748e91210664.svg" valign=-4.962839999999996px width=37.410799999999995pt height=19.85084pt/>. Now, this raises an interesting question: What does it mean to take a derivative of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/014a505b8ec0a2e3dd98b6c8577cbdea.svg" valign=-4.962839999999996px width=48.370599999999996pt height=19.85084pt/>?

Well, it's not all that mysterious. The derivative of `f2l` is just the rate at which a number's IEEE 754 machine
representation changes as we make small perturbations to a number. Unfortunately, while it might be easy to compute
this derivative as a numerical approximation, we still don't have an approximate form for algebraic manipulation.

While <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/c380902839f6237c1c6760127ad4ccfa.svg" valign=-4.962799999999991px width=53.9608pt height=21.5936pt/> might be difficult to construct, we can fair much better with its sibling, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/ed9711acdecd664aa11a66b17111740a.svg" valign=-4.962799999999991px width=53.9608pt height=21.5936pt/>.
Now, the derivative <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/ed9711acdecd664aa11a66b17111740a.svg" valign=-4.962799999999991px width=53.9608pt height=21.5936pt/> is the rate that a float will change given that we make small perturbations
to its machine representation. However, since its machine representations are all bit-vectors, it doesn't make sense
to take a derivative here since we can't make these perturbations arbitrarily small. The smallest change we can make
is to either add or subtract one. However, if we just accept our fate, then we can define the "derivative" as the finite
difference
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/51b0758e839d40e093628d5fdad75d91.svg" valign=0px width=238.472pt height=42.0354pt/></p>
where
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/594af30420fe403e3ecad1c72cd67c0e.svg" valign=0px width=318.392pt height=53.4994pt/></p>

Here, equality holds when <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/71207e357beb2b5cf4f092c9ebcedc2a.svg" valign=-4.962839999999996px width=48.370599999999996pt height=19.85084pt/> is a perfect power of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/76c5792347bb90ef71cfbace628572cf.svg" valign=0px width=9.925319999999996pt height=12.79276pt/> (including fractions of the form <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/99f338fcaa9681622b3f135c089bf43a.svg" valign=0px width=31.1066pt height=16.88044pt/>).

Therefore,
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/202b89eea44d638e1fee08bb018a0f17.svg" valign=0px width=161.1932pt height=21.5936pt/></p>

From here, we also have
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/c1a2f7231dfbed1936ce911695bc7903.svg" valign=0px width=468.128pt height=466.066pt/></p>

-------------------------------------

For more information on how the constant (`0x54a2fa8c`) is derived for
the cube-root, visit http://www.bullshitmath.lol/.

Equations rendered with [readme2tex](https://github.com/leegao/readme2tex).