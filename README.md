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
where <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/12598784143ca70bf1fed30af7c66556.svg" valign=-4.962839999999996px width=173.45499999999998pt height=19.85084pt/>

For more information on how the constant (`0x54a2fa8c`) is derived for
the cube-root, visit http://www.bullshitmath.lol/.