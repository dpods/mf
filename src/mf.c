//
// Created by David Glassanos on 10/23/18.
//
// Matrix Factorization
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#include "matrix.h"
#include "str.h"

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

size_t get_line_width() {
	FILE *fp;
	char *line = NULL;
	size_t len = 0;

	fp = fopen("./matrix.csv", "r");
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}

	getline(&line, &len, fp);

	size_t i;
	for (i=0; line[i]; line[i]==',' ? i++ : *line++);
	return i + 1;
}

size_t get_line_count() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	size_t lines = 0;

	fp = fopen("./matrix.csv", "r");
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		lines++;
	}

	fclose(fp);

	if (line) {
		free(line);
	}

	return lines;
}

void populate_matrix_from_file(double **matrix, size_t cols) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("./matrix.csv", "r");
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}

	int col;
	int row = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		// Remove newline char at end of string
		trim(line, '\n');

		// Split line by the delimeter
		char **dest = malloc(sizeof(char *) * cols);
		split(dest, line, ",");

		// Add values to matrix and cast to double
		for (col = 0; col < cols; col++) {
			matrix[row][col] = atof(dest[col]);
		}

		row++;
	}

	fclose(fp);

	if (line) {
		free(line);
	}
}

int main(int argc, char **argv) {
	size_t cols = get_line_width();
	size_t rows = get_line_count();

	// seed random number generator
	srand(time(NULL));

	double **R;
	double **P;
	double **Q;

	size_t N = rows;
	size_t M = cols;
	size_t K = 2;

	R = (double **) matrix_malloc(N, M * sizeof(double));

	populate_matrix_from_file(R, cols);

	matrix_print("R", R, N, M);
	return 0;

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

