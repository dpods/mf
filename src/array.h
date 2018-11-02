//
// Created by David Glassanos on 11/1/18.
//

#ifndef MF_ARRAY_H
#define MF_ARRAY_H

double* double_array_alloc(double length);

double dot_product_array(double *a, double *b, size_t n);

void print_array(char *str, double* p, int N);

#endif //MF_ARRAY_H
