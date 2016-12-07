# Floating Point Hacks

### Completely useless, but fun nevertheless.

<p align="center"><img src="https://rawgit.com/leegao/fast-inverse-cube-root/master/svgs/8cb2c10c30636b2b08c60e5e7e17c1ed.svg" valign=0px width=283.938pt height=47.6424pt/></p>

Like the [Fast Inverse Square Root](https://en.wikipedia.org/wiki/Fast_inverse_square_root),
but cubier.

```
  #include "inverse_cuberoot.h"
  
  void example() {
    printf("The inverse of the cuberoot of 42 is %f.\n", inverse_cuberoot(42));
  }
```

For more information on how the constant (`0x54a2fa8c`) is derived for
the cuberoot, visit http://www.bullshitmath.lol/.