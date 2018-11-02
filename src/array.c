//
// Created by David Glassanos on 11/1/18.
//

#include <stdio.h>
#include <stdlib.h>

#include "array.h"

double* double_array_alloc(double length) {
	return (double*)malloc(length * sizeof(double));
}

double dot_product_array(double *a, double *b, size_t n) {
	double sum = 0.0;
	size_t i;

	for (i = 0; i < n; i++) {
		sum += a[i] * b[i];
	}

	return sum;
}

void print_array(char *str, double* p, int N) {
	int i;
	for(i = 0 ; i < N ; i++) {
		printf("%f ", p[i]);
	}

	printf("\n\n");
}