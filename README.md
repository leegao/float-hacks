# Fast Inverse Cube Root

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