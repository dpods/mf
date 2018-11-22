
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "matrix.h"
#include "str.h"

double randomDouble()
{
	return (double) rand() / (double) RAND_MAX;
}

void fillMatrix(Matrix* matrix) {
	int i, j;

	for (i = 0; i < matrix->rows; i++) {
		for (j = 0; j < matrix->cols; j++) {
			matrix->data[i][j] = randomDouble();
		}
	}
}

int get_line_width(FILE *fp) {
	fseek(fp, 0, SEEK_SET);

	char *line = NULL;
	size_t len = 0;

	getline(&line, &len, fp);

	int i;
	for (i=0; line[i]; line[i] == ',' ? i++ : *line++);

	return i + 1;
}

int get_line_count(FILE *fp) {
	fseek(fp, 0, SEEK_SET);

	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int lines = 0;

	while ((read = getline(&line, &len, fp)) != -1) {
		lines++;
	}

	return lines;
}

void populateMatrixFromFile(Matrix* matrix, FILE *fp) {
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
		char **dest = malloc(sizeof(char *) * matrix->cols);
		split(dest, line, ",");

		// Add values to matrix and cast to double
		for (col = 0; col < matrix->cols; col++) {
			matrix->data[row][col] = atof(dest[col]);
		}

		row++;
	}

	if (line) {
		free(line);
	}
}

void transposeMatrix(Matrix* srcMatrix, Matrix* destMatrix) {
	for (int i = 0; i < srcMatrix->rows; i++) {
		for (int j = 0; j < srcMatrix->cols; j++) {
			destMatrix->data[j][i] = srcMatrix->data[i][j];
		}
	}
}

void rowCrossProduct(Matrix* srcMatrix, double* destArray, int row) {
	for (int i = 0; i < srcMatrix->cols; i++) {
		destArray[i] = srcMatrix->data[row][i];
	}
}

void colCrossProduct(Matrix* srcMatrix, double* destArray, int col) {
	for (int j = 0; j < srcMatrix->rows; j++) {
		destArray[j] = srcMatrix->data[j][col];
	}
}

double arrayDotProduct(double *a, double *b, int n) {
	double sum = 0.0;

	for (int i = 0; i < n; i++) {
		sum += a[i] * b[i];
	}

	return sum;
}

void matrixDotProduct(Matrix* destMatrix, Matrix* matrixA, Matrix* matrixB, int features, int rows, int cols)
{
	double rowArray[rows];
	double colArray[cols];

	for (int i = 0; i < rows; i++) {
		rowCrossProduct(matrixA, rowArray, i);

		for (int j = 0; j < cols; j++) {
			colCrossProduct(matrixB, colArray, j);

			destMatrix->data[i][j] = arrayDotProduct(rowArray, colArray, features);
		}
	}
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
void factorizeMatrix(Matrix* inputMatrix, Matrix* pMatrix, Matrix* qMatrix, Matrix* outputMatrix, int rows, int cols, int features) {
	int steps = 5000;
	double alpha = 0.0002;
	double beta = 0.02;

	Matrix* qtMatrix = malloc(sizeof(Matrix));
	initMatrix(qtMatrix, qMatrix->cols, qMatrix->rows);
	transposeMatrix(qMatrix, qtMatrix);

	int step, i, j, h;
	for (step = 0; step < steps; step++) {
		for (i = 0; i < rows; i++) {
			for (j = 0; j < cols; j++) {
				if (inputMatrix->data[i][j] > 0) {
					double rowArray[features];
					double colArray[features];

					rowCrossProduct(pMatrix, rowArray, i);
					colCrossProduct(qtMatrix, colArray, j);

					// eij = R[i][j] - numpy.dot(P[i,:],Q[:,j])
					double eij = inputMatrix->data[i][j] - arrayDotProduct(rowArray, colArray, features);

					for (h = 0; h < features; h++) {
						// P[i][h] = P[i][h] + alpha * (2 * eij * Q[h][j] - beta * P[i][h])
						pMatrix->data[i][h] = pMatrix->data[i][h] + alpha * (2 * eij * qtMatrix->data[h][j] - beta * pMatrix->data[i][h]);

						// Q[h][j] = Q[h][j] + alpha * (2 * eij * P[i][h] - beta * Q[h][j])
						qtMatrix->data[h][j] = qtMatrix->data[h][j] + alpha * (2 * eij * pMatrix->data[i][h] - beta * qtMatrix->data[h][j]);
					}
				}
			}
		}

		double e = 0.0;

		for (i = 0; i < rows; i++) {
			for (j = 0; j < cols; j++) {
				if (inputMatrix->data[i][j] > 0) {
					double rowArray[features];
					double colArray[features];

					rowCrossProduct(pMatrix, rowArray, i);
					colCrossProduct(qtMatrix, colArray, j);
					double eij = inputMatrix->data[i][j] - arrayDotProduct(rowArray, colArray, features);

					e = e + pow(eij, 2);

					for (h = 0; h < features; h++) {
						e = e + (beta/2) * (pow(pMatrix->data[i][h], 2) + pow(qtMatrix->data[h][j], 2));
					}
				}
			}
		}

		if (e < 0.001) {
			break;
		}
	}

	matrixDotProduct(outputMatrix, pMatrix, qtMatrix, features, rows, cols);

	freeMatrix(qtMatrix);
}

int main(int argc, char **argv) {
	bool verbose = false;
	char *option, *value;

	FILE *fp_in = NULL;
	FILE *fp_out = stdout;

	int n;
	for( n = 1; n < argc; n = n + 2 )
	{
		option = argv[n];
		value = argv[n + 1];

		switch ( (int) option[1] )
		{
			case 'i':
				if ( ( fp_in = fopen(value, "r")) == NULL )
				{
					puts( "Can't open input file.\n" );
					exit( 0 );
				}

				break;

			case 'o':
				if ( ( fp_out = fopen( value, "w" )) == NULL )
				{
					puts( "Can't open output file.\n" );
					exit( 0 );
				}
				break;

			case 'v':
				verbose = true;
				break;

			default:
				printf( "Unknown option %s\n", option );
				exit( 0 );
		}
	}

	if ( fp_in == NULL )
	{
		puts( "No input file specified." );
		exit( 0 );
	}

	int cols = get_line_width(fp_in);
	int rows = get_line_count(fp_in);
	int features = 2;

	Matrix* inputMatrix = malloc(sizeof(Matrix));
	initMatrix(inputMatrix, rows, cols);
	populateMatrixFromFile(inputMatrix, fp_in);

	if ( verbose )
	{
		printf("rows: %d\n", rows);
		printf("cols: %d\n", cols);
		printf("features: %d\n", features);
		printf("\n");

		printf("input\n");
		printMatrix(inputMatrix);
	}

	// seed random number generator
	srand(time(NULL));

	Matrix* pMatrix = malloc(sizeof(Matrix));
	initMatrix(pMatrix, rows, features);
	fillMatrix(pMatrix);

	if ( verbose )
	{
		printf("P\n");
		printMatrix(pMatrix);
	}

	Matrix* qMatrix = malloc(sizeof(Matrix));
	initMatrix(qMatrix, cols, features);
	fillMatrix(qMatrix);

	if ( verbose )
	{
		printf("Q\n");
		printMatrix(qMatrix);
	}

	Matrix* outputMatrix = malloc(sizeof(Matrix));
	initMatrix(outputMatrix, rows, cols);

	factorizeMatrix(inputMatrix, pMatrix, qMatrix, outputMatrix, rows, cols, features);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (j == 0) {
				fprintf(fp_out, "%f", outputMatrix->data[i][j]);
			} else {
				fprintf(fp_out, ",%f", outputMatrix->data[i][j]);
			}
		}

		fprintf(fp_out, "\n");
	}

	// All done, close up shop.
	freeMatrix(inputMatrix);
	freeMatrix(pMatrix);
	freeMatrix(qMatrix);
	freeMatrix(outputMatrix);

	fclose( fp_in );
	fclose( fp_out );

	return 0;
}
