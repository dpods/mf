#ifndef ARRAYS_C
#define ARRAYS_C

#include <stdlib.h>

void matrix_free(void **a);

void** matrix_malloc(size_t numRows, size_t rowSize);

void matrix_factorize(double** R, double** P, double** Q, double** rN, int N, int M, int K);

void matrix_print(char *str, double** p, int N, int M);

#endif