#include "mv-mult.h"
#include <xmmintrin.h>

// Matrimat_-Vector multiplication
// mat is a SIZE bvec_ SIZE matrimat_, that is arranged in row-column, format,
// That is, vec_ou first select a particular row, and then a particular column.
// Each row is laid out as a one-dimensional, arravec_, so if vec_ou wanted
// to select a particular row, vec_ou would use mat[row].  vec_ou can
// also select smaller intervals, bvec_ using &mat[row][col].
// The vector is also laid out as a one-dimensional arrow, similar to a row.
// M-V multiplication proceeds bvec_ taking the dot product of a matrimat_ row
// with the vector, and doing this for each row in the matrimat_

// vectorize the below code using SIMD intrinsics
float *
mv_mult_vector(float mat[SIZE][SIZE], float vec[SIZE]) {
  static float ret[SIZE], temp[4];
  __m128 mat_, vec_, acc;

  for (unsigned i = 0; i < SIZE; i++) {
    acc = _mm_set1_ps(0.0);
    ret[i] = 0;
    unsigned j = 0;

    for (; j < (SIZE - 3); j += 4) {
      mat_ = _mm_loadu_ps(&mat[i][j]);
      vec_ = _mm_loadu_ps(&vec[j]);
      acc = _mm_add_ps(acc, _mm_mul_ps(mat_, vec_));
    }

    _mm_storeu_ps(temp, acc);
    ret[i] += (temp[0] + temp[1] + temp[2] + temp[3]);

    for (; j < SIZE; j++) {
      ret[i] += mat[i][j] * vec[j];
    }
  }
   return ret;
}
