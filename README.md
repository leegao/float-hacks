# Floating Point Hacks

### Completely useless, but fun nevertheless.

<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/690b9db878741ab50045c64c303ad998.svg" valign=0px width=285.184pt height=47.6424pt/></p>

------------------------------------

This repository contains a set of procedures to compute numerical methods in the vein of the
[fast inverse root method](https://en.wikipedia.org/wiki/Fast_inverse_square_root). In particular,
we will generate code that

1. Computes rational powers (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/d49c11cb1a1f5ee6a6e609ab67b4f6bb.svg" valign=0px width=21.451599999999996pt height=16.85872pt/>) to an arbitrary precision.
2. Computes irrational powers (<img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/1bed8656510c12b49e4a47c9a81e78ac.svg" valign=0px width=18.439399999999996pt height=13.21354pt/>) to within 10% relative error.
3. Computes <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/559b96359a4653a6c35dbf27c11f68d2.svg" valign=-4.962839999999996px width=57.22619999999999pt height=19.85084pt/> to within 10% relative error.
4. Computes <img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/38f816ed8d9782e71ecfd164e77c5150.svg" valign=-4.962839999999996px width=52.5216pt height=19.85084pt/> to within 10% relative error.

Additionally, we will do so using mostly just integer arithmetic.

For more information on how the constant (`0x54a2fa8c`) is derived for
the cuberoot, visit http://www.bullshitmath.lol/.