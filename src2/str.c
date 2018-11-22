//
// Created by David Glassanos on 11/3/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "str.h"

void trim(char *str, char delimeter) {
	size_t ln = strlen(str) - 1;
	if (*str && str[ln] == delimeter) {
		str[ln] = '\0';
	}
}

void split(char **dest, char *str, const char *delimeter) {
	char *token, *copy;
	int idx = 0;
	copy = strdup(str);
	while ((token = strsep(&copy, delimeter)) != NULL) {
		dest[idx] = token;
		idx++;
	}
}