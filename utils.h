#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

#undef DEBUG
#define DEBUG 1

#ifdef DEBUG
#define DEBUG_GL if (check_gl_errors()) { fprintf(stderr, "Error on line %d in file %s \n", __LINE__, __FILE__); exit(1); }
#else
#define DEBUG_GL
#endif

int check_gl_errors();
void print_matrix(float *);
