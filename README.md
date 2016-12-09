# Floating Point Hacks

### Completely useless, but fun nevertheless.

<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/690b9db878741ab50045c64c303ad998.svg?b87f369cdc&invert_in_darkmode" align=middle width=235.2768pt height=39.30498pt/></p>

<sub>*Equations for a "fast" <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/3f5ce0fda24ff090e168fcbc2b6288a9.svg?1c97784c0a&invert_in_darkmode" align=middle width=18.253455000000002pt height=27.719999999999995pt/> method.*</sub>

------------------------------------

This repository contains a set of procedures to compute numerical methods in the vein of the
[fast inverse root method](https://en.wikipedia.org/wiki/Fast_inverse_square_root). In particular,
we will generate code that

1. Computes rational powers (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/d49c11cb1a1f5ee6a6e609ab67b4f6bb.svg?851cea52d7&invert_in_darkmode" align=middle width=17.697569999999995pt height=27.816690000000015pt/>) to an arbitrary precision.
2. Computes irrational powers (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/1bed8656510c12b49e4a47c9a81e78ac.svg?8512207c7c&invert_in_darkmode" align=middle width=15.212504999999995pt height=21.80211000000001pt/>) to within 10% relative error.
3. Computes <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/559b96359a4653a6c35dbf27c11f68d2.svg?7710e5c488&invert_in_darkmode" align=middle width=47.21161499999999pt height=24.565200000000008pt/> to within 10% relative error.
4. Computes <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/38f816ed8d9782e71ecfd164e77c5150.svg?d34b8bdb8d&invert_in_darkmode" align=middle width=43.33032pt height=24.565200000000008pt/> to within 10% relative error.

Additionally, we will do so using mostly just integer arithmetic.

## Usage

You can use everything in `floathacks` by including `hacks.h`

    #include <floathacks/hacks.h>
    using namespace floathacks; // Comment this out if you don't want your top-level namespace to be polluted

#### Pow

To generate an estimation for <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/1bed8656510c12b49e4a47c9a81e78ac.svg?ddf1c5f92f&invert_in_darkmode" align=middle width=15.212504999999995pt height=21.80211000000001pt/>, where <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/3e18a4a28fdee1744e5e3f79d13b9ff6.svg?57562ab41f&invert_in_darkmode" align=middle width=7.087112999999998pt height=14.102220000000006pt/> is any floating point number, you can run

    float approximate_root = fpow<FLOAT(0.12345)>::estimate(x);

Since estimates of `pow` can be refined into better iterates (as long as `c` is "rational enough"), you can also
compute a more exact result via

    float root = pow<FLOAT(0.12345), n>(x);

where `n` is the number of newton iterations to perform. The code generated by this template will unroll itself, so it's
relatively efficient.

However, the optimized code does not let you use it as a `constexpr` or where the exponent is not constant. In those cases,
you can use `consts::fpow(x, c)` and `consts::pow(x, c, iterations = 2)` instead:

    float root = consts::pow(x, 0.12345, n);

Note that the compiler isn't able to deduce the optimal constants in these cases, so you'll incur additional penalties
computing the constants of the method.

#### Exp

You can also compute an approximation of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/559b96359a4653a6c35dbf27c11f68d2.svg?88a1e215bb&invert_in_darkmode" align=middle width=47.21161499999999pt height=24.565200000000008pt/> with

    float guess = fexp(x);

Unfortunately, since there are no refinement methods available for exponentials, we can't do much
with this result if it's too coarse for your needs. In addition, due to overflow, this method breaks down
when `x` approaches 90.

#### Log

Similarly, you can also compute an approximation of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/38f816ed8d9782e71ecfd164e77c5150.svg?ce95cce2d&invert_in_darkmode" align=middle width=43.33032pt height=24.565200000000008pt/> with

    float guess = flog(x);

Again, as is with the case of `fexp`, there are no refinement methods available for logarithms either.

All of the `f***` methods above have bounded relative errors of at most 10%. The refined `pow` method
can be made to give arbitrary precision by increasing the number of refinement iterations. Each refinement
iteration takes time proportional to the number of digits in the floating point representation of the exponent.
Note that since floats are finite, this is bounded above by 32 (and more tightly, 23).

## Justification

### Prelude

The key ingredient of these types of methods is the pair of transformations
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/b46a9543f58a8f4ef51bf513d0195b76.svg?96a6824976&invert_in_darkmode" align=middle width=141.311775pt height=24.565200000000008pt/> and 
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/8b1eb6ddb44c5ea9346105ed36f25424.svg?cdcd63c4d&invert_in_darkmode" align=middle width=141.077475pt height=24.565200000000008pt/>.

* <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/014a505b8ec0a2e3dd98b6c8577cbdea.svg?968a356235&invert_in_darkmode" align=middle width=39.905744999999996pt height=24.565200000000008pt/> takes a `IEEE 754` single precision floating point number and outputs its "machine" representation.
In essence, it acts like


        unsigned long f2l(float x) {
          union {float fl; unsigned long lg;} lens = { x };
          return lens.lg;
        }
* <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/2484e1c18e94550c31824679fd30291d.svg?56a4794a82&invert_in_darkmode" align=middle width=38.88488999999999pt height=24.565200000000008pt/> takes an unsigned long representing a float and returns a `IEEE 754` single precision floating point number.
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
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/1956bacf4be82337ef506b5f97ad6590.svg?8b0988f14&invert_in_darkmode" align=middle width=185.85105pt height=39.30498pt/></p>

In a similar vein, a fast inverse cube-root method is presented at the start of this page.
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/690b9db878741ab50045c64c303ad998.svg?6902cab8b5&invert_in_darkmode" align=middle width=235.2768pt height=39.30498pt/></p>

We will justify this in the next section.

### Arbitrary Powers

We can approximate any <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/1bed8656510c12b49e4a47c9a81e78ac.svg?18fffcf157&invert_in_darkmode" align=middle width=15.212504999999995pt height=21.80211000000001pt/> using just integer arithmetic on the machine representation of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/332cc365a4987aacce0ead01b8bdcc0b.svg?1588ccae81&invert_in_darkmode" align=middle width=9.359789999999998pt height=14.102220000000006pt/>. To do
so, compute
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/85325d7348ff0db8a0a89d53e9cec59f.svg?ad614b6f68&invert_in_darkmode" align=middle width=142.97398499999997pt height=16.376943pt/></p>
where <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/763af86a1c2d443760aeb707325ee397.svg?d3195b9eda&invert_in_darkmode" align=middle width=204.28352999999996pt height=24.565200000000008pt/>. In general, any value of `bias`, as long
as it is reasonably small, will work. At `bias = 0`, the method computes a value whose error is completely positive.
Therefore, by increasing the bias, we can shift some of the error down into the negative plane and
halve the error. 

As seen in the fast-inverse-root method, a bias of `-0x5c416` tend to work well for pretty much every case that I've
tried, as long as we tack on at least one Newton refinement stage at the end. It works well without refinement as well,
but an even bias of `-0x5c000` works even better.

Why does this work? See [these slides](http://www.bullshitmath.lol/FastRoot.slides.html) for the derivation. In
particular, the fast inverse square-root is a subclass of this method.

### Exp

We can approximate <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/559b96359a4653a6c35dbf27c11f68d2.svg?94972724f6&invert_in_darkmode" align=middle width=47.21161499999999pt height=24.565200000000008pt/> up to <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/b921a412dbb057d50cc70ebb4f18df62.svg?de7440edff&invert_in_darkmode" align=middle width=47.606624999999994pt height=21.107790000000012pt/> using a similar set of bit tricks. I'll first give its equation, and then
give its derivations. As far as I am aware, these are original. However, since there are no refinement methods
for the computation of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/559b96359a4653a6c35dbf27c11f68d2.svg?b748910ede&invert_in_darkmode" align=middle width=47.21161499999999pt height=24.565200000000008pt/>, there is practically no reason to ever resort to this approximation unless you're okay
with 10% error.

<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/dd860a34346bda9f243b20a8f2525b7f.svg?445910936b&invert_in_darkmode" align=middle width=313.85474999999997pt height=19.85742pt/></p>

Here, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/7ccca27b5ccc533a2dd72dc6fa28ed84.svg?8f548f9a5&invert_in_darkmode" align=middle width=6.647338499999998pt height=14.102220000000006pt/> is the [machine epsilon](https://en.wikipedia.org/wiki/Machine_epsilon) for single precision, and it
is computed by <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/e0032bb43ddccaa3ea0566c22746ee48.svg?8bdaba7010&invert_in_darkmode" align=middle width=125.74419pt height=24.565200000000008pt/>.

To give a derivation of this equation, we'll need to borrow a few mathematical tools from analysis. In particular, while
`l2f` and `f2l` have many discontinuities (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/4f3b183ca15faf05b91c6ad7e3b6d7a5.svg?dd592e77bb&invert_in_darkmode" align=middle width=14.716514999999998pt height=26.70624000000001pt/> of them to be exact), it is mostly smooth. This
carries over to its "rate-of-change" as well, so we will just pretend that it has mostly smooth derivatives
everywhere.

#### Differentiating the `l2f` and `f2l` functions.
Consider the function
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/12d8e344454774536a8da176b6cc638d.svg?f37318152&invert_in_darkmode" align=middle width=208.39995pt height=34.679205pt/></p>
where the equality is a consequence of the chain-rule, assuming that `f2l` is differentiable at the particular value of
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/210d22201f1dd53994dc748e91210664.svg?96ac9717ca&invert_in_darkmode" align=middle width=30.863909999999994pt height=24.565200000000008pt/>. Now, this raises an interesting question: What does it mean to take a derivative of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/014a505b8ec0a2e3dd98b6c8577cbdea.svg?9a95967936&invert_in_darkmode" align=middle width=39.905744999999996pt height=24.565200000000008pt/>?

Well, it's not all that mysterious. The derivative of `f2l` is just the rate at which a number's IEEE 754 machine
representation changes as we make small perturbations to a number. Unfortunately, while it might be easy to compute
this derivative as a numerical approximation, we still don't have an approximate form for algebraic manipulation.

While <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/c380902839f6237c1c6760127ad4ccfa.svg?a42024a5e6&invert_in_darkmode" align=middle width=44.51766pt height=27.440820000000013pt/> might be difficult to construct, we can fair much better with its sibling, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/ed9711acdecd664aa11a66b17111740a.svg?652e947dc&invert_in_darkmode" align=middle width=44.51766pt height=27.440820000000013pt/>.
Now, the derivative <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/ed9711acdecd664aa11a66b17111740a.svg?b19395974d&invert_in_darkmode" align=middle width=44.51766pt height=27.440820000000013pt/> is the rate that a float will change given that we make small perturbations
to its machine representation. However, since its machine representations are all bit-vectors, it doesn't make sense
to take a derivative here since we can't make these perturbations arbitrarily small. The smallest change we can make
is to either add or subtract one. However, if we just accept our fate, then we can define the "derivative" as the finite
difference
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/51b0758e839d40e093628d5fdad75d91.svg?9a7eddd6e4&invert_in_darkmode" align=middle width=196.7394pt height=34.679205pt/></p>
where
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/594af30420fe403e3ecad1c72cd67c0e.svg?aa0cb87dd9&invert_in_darkmode" align=middle width=262.67339999999996pt height=44.137005pt/></p>

Here, equality holds when <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/71207e357beb2b5cf4f092c9ebcedc2a.svg?2d6dc70f99&invert_in_darkmode" align=middle width=39.905744999999996pt height=24.565200000000008pt/> is a perfect power of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/76c5792347bb90ef71cfbace628572cf.svg?2b96a507b2&invert_in_darkmode" align=middle width=8.188388999999995pt height=21.107790000000012pt/> (including fractions of the form <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/99f338fcaa9681622b3f135c089bf43a.svg?e0f46660ce&invert_in_darkmode" align=middle width=25.662945pt height=27.852660000000014pt/>).

Therefore,
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/202b89eea44d638e1fee08bb018a0f17.svg?b7c25cf8d2&invert_in_darkmode" align=middle width=132.98439pt height=17.814719999999998pt/></p>

From here, we also have
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/95f95e0e442c68d11bba22f18d4752b1.svg?a73b17265a&invert_in_darkmode" align=middle width=451.5093pt height=216.16649999999998pt/></p>

#### A Tale of Two Functions

Given <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/41bd2b9df7093e1c0caa2c5cade45afc.svg?8ebbd24782&invert_in_darkmode" align=middle width=89.15873999999998pt height=33.78408pt/>, antidifferentiating both sides gives
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/ec4b015914608948961ad0b8aa7aca12.svg?c4fe429ac3&invert_in_darkmode" align=middle width=165.06105pt height=18.269295pt/></p>

Similarly, since <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/4517ee70225fd1fbc2a56462aad1fb33.svg?55424d76aa&invert_in_darkmode" align=middle width=112.94085pt height=27.440820000000013pt/> satisfies <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/fe24365f266dbd2a3c942fa304305bde.svg?316e28fbdb&invert_in_darkmode" align=middle width=50.36509499999999pt height=24.66816pt/>, we have
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/3742db4f3e50f5a3a2b9b24080b3b10e.svg?133b15ba6d&invert_in_darkmode" align=middle width=125.46187499999998pt height=16.376943pt/></p>

This makes sense, since we'd like these two functions to be inverses of each other.

#### Exp, redux.

Consider
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/1f804b1d46ed8b17c6969731592141ac.svg?526c600706&invert_in_darkmode" align=middle width=240.7647pt height=18.269295pt/></p>
which suggests that <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/02956f2e4eb289405ddb74c773002d97.svg?4c2a0ae16d&invert_in_darkmode" align=middle width=171.25053pt height=27.940439999999995pt/>.

Since we would like <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/fc627948191f20de9f76da56255cf0ff.svg?1f1e37db50&invert_in_darkmode" align=middle width=76.09866pt height=24.565200000000008pt/>, we can impose the boundary condition
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/96ca7d2813c9f413253ffba263c6fee6.svg?77db81187a&invert_in_darkmode" align=middle width=277.70984999999996pt height=19.85742pt/></p>
which gives <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/168268e1451a8dfc43a23b8773d2ab42.svg?922b0da6eb&invert_in_darkmode" align=middle width=73.484895pt height=24.565200000000008pt/>. However, while this method gives bounded relative error, in its unbiased form
this is pretty off the mark for general purposes (it approximates some other <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/80c5c65e2fa2a2d733d1ac16fa5b3acc.svg?cbd46af37&invert_in_darkmode" align=middle width=14.513564999999998pt height=21.80211000000001pt/>). Instead, we can add in an unbiased form:
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/1768bdc96604f36fc58deea3f106c4f1.svg?e02707cec6&invert_in_darkmode" align=middle width=236.9136pt height=19.85742pt/></p>
where, empirically, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/fd57f6d91f6e31899590811fc9f2128b.svg?aa6ac07603&invert_in_darkmode" align=middle width=94.86113999999999pt height=22.745580000000004pt/> gives a good approximation. Notice that the <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/4bdc8d9bcfb35e1c9bfb51fc69687dfc.svg?c6aef61eb4&invert_in_darkmode" align=middle width=7.028323499999998pt height=22.745580000000004pt/> we've chosen is close to
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/23f78e0b2a6bbbadec2d25ce8d5a81a0.svg?ae02ae25d1&invert_in_darkmode" align=middle width=44.337149999999994pt height=26.70624000000001pt/>, which is what we need to transform <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/80c5c65e2fa2a2d733d1ac16fa5b3acc.svg?8525b77cf3&invert_in_darkmode" align=middle width=14.513564999999998pt height=21.80211000000001pt/> to <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/b6b70db98c2a5c2031dea120886f8211.svg?cc718ece2a&invert_in_darkmode" align=middle width=15.051959999999998pt height=21.80211000000001pt/>.
In particular, for all <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/55a049b8f161ae7cfeb0197d75aff967.svg?a19f28a556&invert_in_darkmode" align=middle width=9.829874999999996pt height=14.102220000000006pt/>, the <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/596205e74b63d939fb386f5456910cf6.svg?5520b73200&invert_in_darkmode" align=middle width=17.673314999999995pt height=26.70624000000001pt/>, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/e8831293b846e3a3799cd6a02e4a0cd9.svg?9ca00c051a&invert_in_darkmode" align=middle width=17.673314999999995pt height=26.70624000000001pt/>, and <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/72d8c986bb268cc5845e6aa7b3d3ce0f.svg?29951103d0&invert_in_darkmode" align=middle width=24.201374999999995pt height=22.381589999999996pt/> relative error is always below 10%.

<p align="center"><img src="http://i.imgur.com/Kr7dcSz.png"/></p>

### Log

In a similar spirit, we can use the approximation
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/ec4b015914608948961ad0b8aa7aca12.svg?1be9e2b73f&invert_in_darkmode" align=middle width=165.06105pt height=18.269295pt/></p>
to derive
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/a59a6cf3ffdb2becd3ffa45eacf5df41.svg?5d122b79ff&invert_in_darkmode" align=middle width=177.48225pt height=16.376943pt/></p>
Imposing a boundary condition at <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/8614628c35cbd72f9732b246c2e4d7b8.svg?22053173e7&invert_in_darkmode" align=middle width=39.418169999999996pt height=21.107790000000012pt/> gives <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/168268e1451a8dfc43a23b8773d2ab42.svg?1e1b5b4236&invert_in_darkmode" align=middle width=73.484895pt height=24.565200000000008pt/>, so we should expect
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/7774c7653eb46407ac10b0ebf9f0ab66.svg?8e7d55478a&invert_in_darkmode" align=middle width=203.3361pt height=16.376943pt/></p>

However, this actually computes some other logarithm <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/c6774276dd6e07aabea5c2d3725f52b4.svg?20a5ed4a47&invert_in_darkmode" align=middle width=50.026844999999994pt height=24.565200000000008pt/>, and we'll have to, again, unbias this term
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/7bacde2da21534aaaf06d6f354884c15.svg?3a3fe185b5&invert_in_darkmode" align=middle width=301.46325pt height=16.376943pt/></p>

where the <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/2bf3e29d19ad9f0c0758e54dcc9fdf9b.svg?4b050156ba&invert_in_darkmode" align=middle width=50.704829999999994pt height=24.565200000000008pt/> term came from the fact that the base computation approximates the 2-logarithm. Empirically, I've
found that a bias of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/a6bb6f6852e5def4a2ac06b15cdee7a8.svg?4608e8c02c&invert_in_darkmode" align=middle width=86.672685pt height=22.745580000000004pt/> works well. In particular, for all <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/55a049b8f161ae7cfeb0197d75aff967.svg?a9bb2fa60d&invert_in_darkmode" align=middle width=9.829874999999996pt height=14.102220000000006pt/>, the <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/596205e74b63d939fb386f5456910cf6.svg?a8d2e0d3b1&invert_in_darkmode" align=middle width=17.673314999999995pt height=26.70624000000001pt/>,
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/e8831293b846e3a3799cd6a02e4a0cd9.svg?b8954398bf&invert_in_darkmode" align=middle width=17.673314999999995pt height=26.70624000000001pt/>, and <img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/72d8c986bb268cc5845e6aa7b3d3ce0f.svg?b2e2c1129&invert_in_darkmode" align=middle width=24.201374999999995pt height=22.381589999999996pt/> relative error is always below 10%.

<p align="center"><img src="http://i.imgur.com/TsjGPwc.png"/></p>

-------------------------------------

For more information on how the constant (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/svgs/svgs/020b87e24d0ffe9a99194ef0411648e7.svg?84f452881&invert_in_darkmode" align=middle width=78.24564pt height=22.745580000000004pt/>) is derived for
the cube-root, visit http://www.bullshitmath.lol/.

Equations rendered with [readme2tex](https://github.com/leegao/readme2tex).