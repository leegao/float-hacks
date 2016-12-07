# Floating Point Hacks

### Completely useless, but fun nevertheless.

<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/690b9db878741ab50045c64c303ad998.svg" valign=0.0px width=256.66560000000004pt height=42.87816pt/></p>

<sub>*Equations for a "fast" inverse cube-root method.*</sub>

------------------------------------

This repository contains a set of procedures to compute numerical methods in the vein of the
[fast inverse root method](https://en.wikipedia.org/wiki/Fast_inverse_square_root). In particular,
we will generate code that

1. Computes rational powers (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/d49c11cb1a1f5ee6a6e609ab67b4f6bb.svg" valign=0.0px width=19.306439999999995pt height=15.172848000000002pt/>) to an arbitrary precision.
2. Computes irrational powers (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1bed8656510c12b49e4a47c9a81e78ac.svg" valign=0.0px width=16.595459999999996pt height=11.892186pt/>) to within 10% relative error.
3. Computes <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/559b96359a4653a6c35dbf27c11f68d2.svg" valign=-4.466555999999997px width=51.50357999999999pt height=17.865756pt/> to within 10% relative error.
4. Computes <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/38f816ed8d9782e71ecfd164e77c5150.svg" valign=-4.466555999999997px width=47.26944pt height=17.865756pt/> to within 10% relative error.

Additionally, we will do so using mostly just integer arithmetic.

## Justification

### Prelude

The key ingredient of these types of methods is the pair of transformations
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/b46a9543f58a8f4ef51bf513d0195b76.svg" valign=-4.466555999999997px width=154.1583pt height=17.865756pt/> and 
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/8b1eb6ddb44c5ea9346105ed36f25424.svg" valign=-4.466555999999997px width=153.90269999999998pt height=17.865756pt/>.

* <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/014a505b8ec0a2e3dd98b6c8577cbdea.svg" valign=-4.466555999999997px width=43.533539999999995pt height=17.865756pt/> takes a `IEEE 754` single precision floating point number and outputs its "machine" representation.
In essence, it acts like


        unsigned long f2l(float x) {
          union {float fl; unsigned long lg;} lens = { x };
          return lens.lg;
        }
* <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/2484e1c18e94550c31824679fd30291d.svg" valign=-4.466555999999997px width=42.41988pt height=17.865756pt/> takes an unsigned long representing a float and returns a `IEEE 754` single precision floating point number.
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
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1956bacf4be82337ef506b5f97ad6590.svg" valign=0.0px width=202.7466pt height=42.87816pt/></p>

In a similar vein, a fast inverse cube-root method is presented at the start of this page.
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/690b9db878741ab50045c64c303ad998.svg" valign=0.0px width=256.66560000000004pt height=42.87816pt/></p>

We will justify this in the next section.

### Arbitrary Powers

We can approximate any <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1bed8656510c12b49e4a47c9a81e78ac.svg" valign=0.0px width=16.595459999999996pt height=11.892186pt/> using just integer arithmetic on the machine representation of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/332cc365a4987aacce0ead01b8bdcc0b.svg" valign=0.0px width=10.210679999999998pt height=7.692192pt/>. To do
so, compute
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/85325d7348ff0db8a0a89d53e9cec59f.svg" valign=0.0px width=155.97162pt height=17.865756pt/></p>
where <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/763af86a1c2d443760aeb707325ee397.svg" valign=-4.466555999999997px width=222.85475999999997pt height=17.865756pt/>. In general, any value of `bias`, as long
as it is reasonably small, will work. At `bias = 0`, the method computes a value whose error is completely positive.
Therefore, by increasing the bias, we can shift some of the error down into the negative plane and
halve the error. 

As seen in the fast-inverse-root method, a bias of `-0x5c416` tend to work well for pretty much every case that I've
tried, as long as we tack on at least one Newton refinement stage at the end. It works well without refinement as well,
but an even bias of `-0x5c000` works even better.

Why does this work? See [these slides](http://www.bullshitmath.lol/FastRoot.slides.html) for the derivation. In
particular, the fast inverse square-root is a subclass of this method.

### Exp

We can approximate <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/559b96359a4653a6c35dbf27c11f68d2.svg" valign=-4.466555999999997px width=51.50357999999999pt height=17.865756pt/> up to <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/b921a412dbb057d50cc70ebb4f18df62.svg" valign=0.0px width=51.9345pt height=11.513484pt/> using a similar set of bit tricks. I'll first give its equation, and then
give its derivations. As far as I am aware, these are original. However, since there are no refinement methods
for the computation of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/559b96359a4653a6c35dbf27c11f68d2.svg" valign=-4.466555999999997px width=51.50357999999999pt height=17.865756pt/>, there is practically no reason to ever resort to this approximation unless you're okay
with 10% error.

<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/dd860a34346bda9f243b20a8f2525b7f.svg" valign=0.0px width=342.387pt height=21.662640000000003pt/></p>

Here, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/7ccca27b5ccc533a2dd72dc6fa28ed84.svg" valign=0.0px width=7.251641999999999pt height=7.692192pt/> is the [machine epsilon](https://en.wikipedia.org/wiki/Machine_epsilon) for single precision, and it
is computed by <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/e0032bb43ddccaa3ea0566c22746ee48.svg" valign=-4.466555999999997px width=137.17548000000002pt height=17.865756pt/>.

To give a derivation of this equation, we'll need to borrow a few mathematical tools from analysis. In particular, while
`l2f` and `f2l` have many discontinuities (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/4f3b183ca15faf05b91c6ad7e3b6d7a5.svg" valign=0.0px width=16.05438pt height=14.567093999999999pt/> of them to be exact), it is mostly smooth. This
carries over to its "rate-of-change" as well, so we will just pretend that it has mostly smooth derivatives
everywhere.

#### Differentiating the `l2f` and `f2l` functions.
Consider the function
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/12d8e344454774536a8da176b6cc638d.svg" valign=0.0px width=227.3454pt height=37.831860000000006pt/></p>
where the equality is a consequence of the chain-rule, assuming that `f2l` is differentiable at the particular value of
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/210d22201f1dd53994dc748e91210664.svg" valign=-4.466555999999997px width=33.66972pt height=17.865756pt/>. Now, this raises an interesting question: What does it mean to take a derivative of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/014a505b8ec0a2e3dd98b6c8577cbdea.svg" valign=-4.466555999999997px width=43.533539999999995pt height=17.865756pt/>?

Well, it's not all that mysterious. The derivative of `f2l` is just the rate at which a number's IEEE 754 machine
representation changes as we make small perturbations to a number. Unfortunately, while it might be easy to compute
this derivative as a numerical approximation, we still don't have an approximate form for algebraic manipulation.

While <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/c380902839f6237c1c6760127ad4ccfa.svg" valign=-4.466519999999992px width=48.56472pt height=19.43424pt/> might be difficult to construct, we can fair much better with its sibling, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/ed9711acdecd664aa11a66b17111740a.svg" valign=-4.466519999999992px width=48.56472pt height=19.43424pt/>.
Now, the derivative <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/ed9711acdecd664aa11a66b17111740a.svg" valign=-4.466519999999992px width=48.56472pt height=19.43424pt/> is the rate that a float will change given that we make small perturbations
to its machine representation. However, since its machine representations are all bit-vectors, it doesn't make sense
to take a derivative here since we can't make these perturbations arbitrarily small. The smallest change we can make
is to either add or subtract one. However, if we just accept our fate, then we can define the "derivative" as the finite
difference
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/51b0758e839d40e093628d5fdad75d91.svg" valign=0.0px width=214.62480000000002pt height=37.831860000000006pt/></p>
where
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/594af30420fe403e3ecad1c72cd67c0e.svg" valign=0.0px width=286.5528pt height=48.149460000000005pt/></p>

Here, equality holds when <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/71207e357beb2b5cf4f092c9ebcedc2a.svg" valign=-4.466555999999997px width=43.533539999999995pt height=17.865756pt/> is a perfect power of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/76c5792347bb90ef71cfbace628572cf.svg" valign=0.0px width=8.932787999999997pt height=11.513484pt/> (including fractions of the form <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/99f338fcaa9681622b3f135c089bf43a.svg" valign=0.0px width=27.99594pt height=15.192396pt/>).

Therefore,
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/202b89eea44d638e1fee08bb018a0f17.svg" valign=0.0px width=145.07388pt height=19.43424pt/></p>

From here, we also have
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/95f95e0e442c68d11bba22f18d4752b1.svg" valign=0.0px width=492.5556pt height=235.81799999999998pt/></p>

#### A Tale of Two Functions

Given <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/41bd2b9df7093e1c0caa2c5cade45afc.svg" valign=-6.183899999999999px width=97.26407999999999pt height=24.61158pt/>, antidifferentiating both sides gives
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/ec4b015914608948961ad0b8aa7aca12.svg" valign=0.0px width=180.06660000000002pt height=19.93014pt/></p>

Similarly, since <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/4517ee70225fd1fbc2a56462aad1fb33.svg" valign=-4.466519999999992px width=123.2082pt height=19.43424pt/> satisfies <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/fe24365f266dbd2a3c942fa304305bde.svg" valign=-3.4740179999999987px width=54.94374pt height=16.929378pt/>, we have
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/3742db4f3e50f5a3a2b9b24080b3b10e.svg" valign=0.0px width=136.8675pt height=17.865756pt/></p>

This makes sense, since we'd like these two functions to be inverses of each other.

#### Exp, redux.

Consider
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1f804b1d46ed8b17c6969731592141ac.svg" valign=0.0px width=262.6524pt height=19.93014pt/></p>
which suggests that <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/02956f2e4eb289405ddb74c773002d97.svg" valign=-6.198840000000002px width=186.81876pt height=21.43908pt/>.

Since we would like <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/fc627948191f20de9f76da56255cf0ff.svg" valign=-4.466555999999997px width=83.01671999999999pt height=17.865756pt/>, we can impose the boundary condition
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/96ca7d2813c9f413253ffba263c6fee6.svg" valign=0.0px width=302.9562pt height=21.662640000000003pt/></p>
which gives <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/168268e1451a8dfc43a23b8773d2ab42.svg" valign=-4.466555999999997px width=80.16534pt height=17.865756pt/>. However, while this method gives bounded relative error, in its unbiased form
this is pretty off the mark for general purposes (it approximates some other <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/80c5c65e2fa2a2d733d1ac16fa5b3acc.svg" valign=0.0px width=15.83298pt height=11.892186pt/>). Instead, we can add in an unbiased form:
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1768bdc96604f36fc58deea3f106c4f1.svg" valign=0.0px width=258.45120000000003pt height=21.662640000000003pt/></p>
where, empirically, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/fd57f6d91f6e31899590811fc9f2128b.svg" valign=0.0px width=103.48488pt height=12.40677pt/> gives a good approximation. Notice that the <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/4bdc8d9bcfb35e1c9bfb51fc69687dfc.svg" valign=0.0px width=7.667261999999999pt height=12.40677pt/> we've chosen is close to
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/23f78e0b2a6bbbadec2d25ce8d5a81a0.svg" valign=-4.466519999999992px width=48.367799999999995pt height=19.033559999999998pt/>, which is what we need to transform <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/80c5c65e2fa2a2d733d1ac16fa5b3acc.svg" valign=0.0px width=15.83298pt height=11.892186pt/> to <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/b6b70db98c2a5c2031dea120886f8211.svg" valign=0.0px width=16.42032pt height=11.892186pt/>.
In particular, for all <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/55a049b8f161ae7cfeb0197d75aff967.svg" valign=0.0px width=10.723499999999996pt height=7.692192pt/>, the <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/596205e74b63d939fb386f5456910cf6.svg" valign=0.0px width=19.27998pt height=14.567093999999999pt/>, <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/e8831293b846e3a3799cd6a02e4a0cd9.svg" valign=0.0px width=19.27998pt height=14.567093999999999pt/>, and <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/72d8c986bb268cc5845e6aa7b3d3ce0f.svg" valign=0.0px width=26.401499999999995pt height=12.208248pt/> relative error is always below 10%.

<p align="center"><img src="http://i.imgur.com/Kr7dcSz.png"/></p>

### Log

In a similar spirit, we can use the approximation
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/ec4b015914608948961ad0b8aa7aca12.svg" valign=0.0px width=180.06660000000002pt height=19.93014pt/></p>
to derive
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/a59a6cf3ffdb2becd3ffa45eacf5df41.svg" valign=0.0px width=193.617pt height=17.865756pt/></p>
Imposing a boundary condition at <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/8614628c35cbd72f9732b246c2e4d7b8.svg" valign=0.0px width=43.001639999999995pt height=11.513484pt/> gives <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/168268e1451a8dfc43a23b8773d2ab42.svg" valign=-4.466555999999997px width=80.16534pt height=17.865756pt/>, so we should expect
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/7774c7653eb46407ac10b0ebf9f0ab66.svg" valign=0.0px width=221.8212pt height=17.865756pt/></p>

However, this actually computes some other logarithm <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/c6774276dd6e07aabea5c2d3725f52b4.svg" valign=-4.466555999999997px width=54.57474pt height=17.865756pt/>, and we'll have to, again, unbias this term
<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/7bacde2da21534aaaf06d6f354884c15.svg" valign=0.0px width=328.869pt height=17.865756pt/></p>

where the <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/2bf3e29d19ad9f0c0758e54dcc9fdf9b.svg" valign=-4.466555999999997px width=55.31435999999999pt height=17.865756pt/> term came from the fact that the base computation approximates the 2-logarithm. Empirically, I've
found that a bias of <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/a6bb6f6852e5def4a2ac06b15cdee7a8.svg" valign=0.0px width=94.55202pt height=12.40677pt/> works well. In particular, for all <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/55a049b8f161ae7cfeb0197d75aff967.svg" valign=0.0px width=10.723499999999996pt height=7.692192pt/>, the <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/596205e74b63d939fb386f5456910cf6.svg" valign=0.0px width=19.27998pt height=14.567093999999999pt/>,
<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/e8831293b846e3a3799cd6a02e4a0cd9.svg" valign=0.0px width=19.27998pt height=14.567093999999999pt/>, and <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/72d8c986bb268cc5845e6aa7b3d3ce0f.svg" valign=0.0px width=26.401499999999995pt height=12.208248pt/> relative error is always below 10%.

<p align="center"><img src="http://i.imgur.com/TsjGPwc.png"/></p>

-------------------------------------

For more information on how the constant (`0x54a2fa8c`) is derived for
the cube-root, visit http://www.bullshitmath.lol/.

Equations rendered with [readme2tex](https://github.com/leegao/readme2tex).