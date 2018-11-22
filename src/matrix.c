//
// Created by David Glassanos on 11/21/18.
//

#include <stdlib.h>
#include <printf.h>
#include "matrix.h"

void freeMatrix(Matrix* matrix) {
	// free array of cols for each row
	for (int i = 0; i < matrix->rows; i++) {
		free(matrix->data[i]);
	}

	// then free array of rows
	free(matrix->data);

	// then free the matrix
	free(matrix);
}

void printMatrix(Matrix* matrix) {
	printf("rows %d\n", matrix->rows);
	printf("cols %d\n", matrix->cols);

	for (int i = 0; i < matrix->rows; i++) {
		for (int k = 0; k < matrix->cols; k++) {
			printf("%f\t", matrix->data[i][k]);
		}
		printf("\n");
	}

	printf("\n");
}

void initMatrix(Matrix* matrix, int rows, int cols) {
	matrix->rows = rows;
	matrix->cols = cols;

	// matrix->data is an array of double * pointers that point to the rows
	matrix->data = (double **) malloc(sizeof(double *) * matrix->rows);
	if(matrix->data == 0) {
		// malloc failed
		return;
	}

	// now allocate the actual rows
	for(int i = 0; i < matrix->rows; i++) {
		matrix->data[i] = calloc((size_t)matrix->cols, sizeof(double*));

		// if malloc failed, free memory
		if(matrix->data[i] == 0) {
			freeMatrix(matrix);
			return;
		}
	}
}