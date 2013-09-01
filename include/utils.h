#define UTILS_H
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
#define DEBUG_GL do { if (check_gl_errors()) { fprintf(stderr, "Error on line %d in file %s \n", __LINE__, __FILE__); exit(1); } } while (0); 
#else
#define DEBUG_GL
#endif
int check_gl_errors(void);
char *getfile(char *filename); // char * is NULL or entire file contents.
