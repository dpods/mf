//
// Created by David Glassanos on 10/23/18.
//
// Matrix Factorization
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"

double randomDouble()
{
	return (double) rand() / (double) RAND_MAX;
}

void fill_matrix(double** arr, int N, int M) {
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			arr[i][j] = randomDouble();
		}
	}
}

int main() {
	// seed random number generator
	srand(time(NULL));

	double **R;
	double **P;
	double **Q;

	size_t N = 7;
	size_t M = 4;
	size_t K = 2;

	R = (double **) matrix_malloc(N, M * sizeof(double));

	R[0][0] = 5;
	R[0][1] = 3;
	R[0][2] = 0;
	R[0][3] = 1;

	R[1][0] = 4;
	R[1][1] = 0;
	R[1][2] = 0;
	R[1][3] = 1;

	R[2][0] = 1;
	R[2][1] = 1;
	R[2][2] = 0;
	R[2][3] = 5;

	R[3][0] = 1;
	R[3][1] = 0;
	R[3][2] = 0;
	R[3][3] = 4;

	R[4][0] = 0;
	R[4][1] = 1;
	R[4][2] = 5;
	R[4][3] = 4;

	R[5][0] = 4;
	R[5][1] = 0;
	R[5][2] = 0;
	R[5][3] = 2;

	R[6][0] = 1;
	R[6][1] = 0;
	R[6][2] = 0;
	R[6][3] = 4;

//	matrix_print("R", R, N, M);
//
//	printf("N: %d\n", N);
//	printf("M: %d\n", M);
//	printf("K: %d\n", K);
//	printf("\n");

	P = (double **) matrix_malloc(N, K * sizeof(double));
	fill_matrix(P, N, K);
//	matrix_print("P", P, N, K);

	Q = (double **) matrix_malloc(M, K * sizeof(double));
	fill_matrix(Q, M, K);
//	matrix_print("Q", Q, M, K);

	matrix_factorize(R, P, Q, N, M, K);

	matrix_free((void**)R);
	matrix_free((void**)P);
	matrix_free((void**)Q);

	return 0;
}

