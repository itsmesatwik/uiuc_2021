#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "transpose.h"

// will be useful
// remember that you shouldn't go over SIZE
using std::min;

// modify this function to add tiling
void
transpose_tiled(int **src, int **dest) {
    int Stride = 16;
    for (int i = 0; i < SIZE; i += Stride) {
        for (int j = 0; j < SIZE; j += Stride) {
            for (int k = j; k < std::min(SIZE, j+Stride); k++) {
                for (int l = i; l < std::min(SIZE, i+Stride); l++) {
                    dest[l][k] = src[k][l];
                }
            }
        }
    }
}
