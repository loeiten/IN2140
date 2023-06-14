#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_BINARY_FILE_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_BINARY_FILE_H_

#include <stdio.h>  // for FILE
struct Router;

int readBinaryFile(const char* binFile, struct Router** routerArray, size_t* N);
int readRouter(FILE* fp, struct Router* router);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_BINARY_FILE_H_
