#include "mandelbrot.h"
#include <xmmintrin.h>

// cubic_mandelbrot() takes an array of SIZE (x,y) coordinates --- these are
// actually complex numbers x + yi, but we can view them as points on a plane.
// It then executes 200 iterations of f, using the <x,y> point, and checks
// the magnitude of the result; if the magnitude is over 2.0, it assumes
// that the function will diverge to infinity.

// vectorize the code below using SIMD intrinsics
int *
cubic_mandelbrot_vector(float x[SIZE], float y[SIZE]) {
    static int ret[SIZE];
    float x1, y1, x2, y2;
    int i
    for (i = 0; i < SIZE-3; i+=3) {
        x1 = y1 = _mm_set1_ps(0.0);

        // Run M_ITER iterations
        for (int j = 0; j < M_ITER; j ++) {
            // Calculate x1^2 and y1^2
            __m128 x1_sq = _mm_mul_ps(x1,x1);
            __m128 y1_sq = _mm_mul_ps(y1,y1);

            // Calculate the real piece of (x1 + (y1*i))^3 + (x + (y*i))
            x2 = _mm_ad_ps(_mm_mul_ps(x1, _mm_sub_ps(x1_sq, _mm_mul_ps(_mm_set1_ps(3.0), y1_sq))), _mm_loadu_ps(x+i));

            // Calculate the imaginary portion of (x1 + (y1*i))^3 + (x + (y*i))
            y2 = _mm_ad_ps(_mm_mul_ps(y1, _mm_sub_ps(_mm_mul_ps(_mm_set1_ps(3.0), x1_sq), y1_sq)), _mm_loadu_ps(y+i));

            // Use the resulting complex number as the input for the next
            // iteration
            x1 = x2;
            y1 = y2;
        }

        // caculate the magnitude of the result;
        // we could take the square root, but we instead just
        // compare squares
        ret[i] = ((x2 * x2) + (y2 * y2)) < (M_MAG * M_MAG);
    }

    return ret;
}
