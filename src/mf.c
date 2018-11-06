//
// Created by David Glassanos on 10/23/18.
//
// Matrix Factorization
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <stdbool.h>

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

size_t get_line_width(FILE *fp) {
	fseek(fp, 0, SEEK_SET);

	char *line = NULL;
	size_t len = 0;

	getline(&line, &len, fp);

	size_t i;
	for (i=0; line[i]; line[i]==',' ? i++ : *line++);

	return i + 1;
}

size_t get_line_count(FILE *fp) {
	fseek(fp, 0, SEEK_SET);

	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	size_t lines = 0;

	while ((read = getline(&line, &len, fp)) != -1) {
		lines++;
	}

	return lines;
}

void populate_matrix_from_file(double **matrix, size_t cols, FILE *fp) {
	fseek(fp, 0, SEEK_SET);

	char *line = NULL;
	size_t len = 0;
	ssize_t read;

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

	if (line) {
		free(line);
	}
}

int main(int argc, char **argv) {
	int n;
	bool verbose = false;
	char *option, *value;
	FILE *fp_in, *fp_out;

	fp_in = stdin;
	fp_out = stdout;

	for( n = 1; n < argc; n = n + 2 )
	{
		option = argv[n];
		value = argv[n + 1];

		switch ( (int) option[1] )
		{
			case 'i':
			case 'I':
				if ( ( fp_in = fopen(value, "r")) == NULL )
				{
					puts( "Can't open input file.\n" );
					exit( 0 );
				}

				break;

			case 'o':
			case 'O':
				if ( ( fp_out = fopen( value, "w" )) == NULL )
				{
					puts( "Can't open output file.\n" );
					exit( 0 );
				}
				break;

			case 'v':
			case 'V':
				verbose = true;
				break;

			default:
				printf( "Unknown option %s\n", option );
				exit( 0 );
		}
	}

	size_t cols = get_line_width(fp_in);
	size_t rows = get_line_count(fp_in);

	// seed random number generator
	srand(time(NULL));

	double **R;
	double **P;
	double **Q;
	double **rN;

	size_t N = rows;
	size_t M = cols;
	size_t K = 2;

	R = (double **) matrix_malloc(N, M * sizeof(double));

	populate_matrix_from_file(R, cols, fp_in);

	if ( verbose )
	{
		printf("rows: %zu\n", N);
		printf("cols: %zu\n", M);
		printf("K: %zu\n", K);
		printf("\n");
	}

	if ( verbose )
	{
		matrix_print("input", R, N, M);
	}

	P = (double **) matrix_malloc(N, K * sizeof(double));
	fill_matrix(P, N, K);

	if ( verbose )
	{
		matrix_print("P", P, N, K);
	}

	Q = (double **) matrix_malloc(M, K * sizeof(double));
	fill_matrix(Q, M, K);

	if ( verbose )
	{
		matrix_print("Q", Q, M, K);
	}

	rN = (double **) matrix_malloc(N, M * sizeof(double));

	matrix_factorize(R, P, Q, rN, N, M, K);

	int i;
	int j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (j == 0)
			{
				fprintf(fp_out, "%f", rN[i][j]);
			}
			else
			{
				fprintf(fp_out, ",%f", rN[i][j]);
			}
		}

		fprintf(fp_out, "\n");
	}

	/* All done, close up shop. */
	matrix_free((void**)R);
	matrix_free((void**)P);
	matrix_free((void**)Q);
	matrix_free((void**)rN);

	fclose( fp_in );
	fclose( fp_out );

	return 0;
}

