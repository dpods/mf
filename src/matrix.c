#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "array.h"

/* frees a 2d array created by matrix_malloc */
void matrix_free(void **a) {
	void **row;

	/* first free rows */
	for(row = a; *row != 0; row++) {
		free(*row);
	}

	/* then free array of rows */
	free(a);
}

/* returns a two-dimensional array with numRows rows and
 * rowSize bytes per row, or 0 on allocation failure.
 * The caller is responsible for freeing the result with free2d. */
void** matrix_malloc(size_t numRows, size_t rowSize)
{
	void **a;
	size_t i;

	/* a is an array of void * pointers that point to the rows */
	/* The last element is 0, so free2d can detect the last row */
	a = malloc(sizeof(void *) * (numRows + 1));        /* one extra for sentinel */
	if(a == 0) {
		/* malloc failed */
		return 0;
	}

	/* now allocate the actual rows */
	for(i = 0; i < numRows; i++) {
		a[i] = malloc(rowSize);
		if(a[i] == 0) {
			/* note that 0 in a[i] will stop freed2d after it frees previous rows */
			matrix_free(a);
			return 0;
		}
	}

	/* initialize the sentinel value */
	a[numRows] = 0;

	return a;
}

void matrix_transpose(double **dest, double **src, int src_h, int src_w)
{
	int i, j;
	for (i = 0; i < src_h; i++) {
		for( j = 0 ; j < src_w ; j++ ) {
			dest[j][i] = src[i][j];
		}
	}
}

void cross_product_row(double* dest, double** src, int row, int src_w)
{
	int i;
	for (i = 0; i < src_w; i++) {
		dest[i] = src[row][i];
	}
}

void cross_product_col(double* dest, double** src, int col, int src_h)
{
	int j;
	for (j = 0; j < src_h; j++) {
		dest[j] = src[j][col];
	}
}

void matrix_dot_product(double** dest, double** a, double** b, size_t k, size_t rowsize, size_t colsize)
{
	double row[rowsize];
	double col[colsize];
	int i, j;

	for (i = 0; i < rowsize; i++) {
		cross_product_row(row, a, i, k);

		for (j = 0; j < colsize; j++) {
			cross_product_col(col, b, j, k);

			dest[i][j] = dot_product_array(row, col, k);
		}
	}
}

void matrix_print(char *str, double** p, int N, int M) {
	printf("%s\n", str);
	printf("------------------\n");
	int i, j;
	for(i = 0 ; i < N ; i++) {
		for(j = 0 ; j < M ; j++) {
			printf("%f ", p[i][j]);
		}
		printf("\n");
	}

	printf("\n");
}

/**
 *
 * @param R the matrix to be factorized, dimension N x M
 * @param P an initial matrix of dimension N x K
 * @param Q an initial matrix of dimension M x K
 * @param N
 * @param M
 * @param K
 */
void matrix_factorize(double** R, double** P, double** Q, double** rN, int N, int M, int K) {
	int steps = 5000;
	double alpha = 0.0002;
	double beta = 0.02;

	double** QT = (double **) matrix_malloc(K, M * sizeof(double));
	matrix_transpose(QT, Q, M, K);

	int step, i, j, h;
	for (step = 0; step < steps; step++) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < M; j++) {
				if (R[i][j] > 0) {
					double row[K];
					double col[K];
					cross_product_row(row, P, i, K);
					cross_product_col(col, QT, j, K);

					// eij = R[i][j] - numpy.dot(P[i,:],Q[:,j])
					double eij = R[i][j] - dot_product_array(row, col, K);

					for (h = 0; h < K; h++) {
						// P[i][h] = P[i][h] + alpha * (2 * eij * Q[h][j] - beta * P[i][h])
						P[i][h] = P[i][h] + alpha * (2 * eij * QT[h][j] - beta * P[i][h]);

						// Q[h][j] = Q[h][j] + alpha * (2 * eij * P[i][h] - beta * Q[h][j])
						QT[h][j] = QT[h][j] + alpha * (2 * eij * P[i][h] - beta * QT[h][j]);
					}
				}
			}
		}

		double e = 0.0;

		for (i = 0; i < N; i++) {
			for (j = 0; j < M; j++) {
				if (R[i][j] > 0) {
					double row[K];
					double col[K];
					cross_product_row(row, P, i, K);
					cross_product_col(col, QT, j, K);
					double eij = R[i][j] - dot_product_array(row, col, K);

					e = e + pow(eij, 2);

					for (h = 0; h < K; h++) {
						e = e + (beta/2) * (pow(P[i][h], 2) + pow(QT[h][j], 2));
					}
				}
			}
		}

		if (e < 0.001) {
			break;
		}
	}

	matrix_dot_product(rN, P, QT, K, N, M);

	matrix_free((void**)QT);
}