#include <stdio.h>
#include <math.h>
#include "inverse_cuberoot.h"

int main() {
    float i;
    for (i = 1; i <= 8; i += 1.0/1024.0f) {
        printf(
                "For i = %f, cuberoot(i) = %f while we expect it to be %f\n",
                i,
                inverse_cuberoot(i),
                pow(i, -1.0 / 3.0));
    }
    return 0;
}