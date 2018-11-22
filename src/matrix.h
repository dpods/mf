//
// Created by David Glassanos on 11/21/18.
//

#ifndef MF_MATRIX_H
#define MF_MATRIX_H

#include <stddef.h>

typedef struct {
	int rows;
	int cols;
	double** data;
} Matrix;

void freeMatrix(Matrix* matrix);
void printMatrix(Matrix* matrix);
void initMatrix(Matrix* matrix, int rows, int cols);

#endif //MF_MATRIX_H
