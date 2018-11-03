//
// Created by David Glassanos on 11/3/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "str.h"

int main() {
	char *test;
	test[0] = '1';
	test[1] = '.';
	test[2] = '0';
	test[3] = ',';
	test[4] = '2';
	test[5] = '.';
	test[6] = '2';
	test[7] = ',';
	test[8] = '3';
	test[9] = ',';
	test[10] = '9';

	char **dest = malloc(sizeof(char *) * 4);

	split(dest, test, ",");

	for (int i = 0; i < 4; i++) {
		printf("%s\n", dest[i]);
	}

	return 0;
}