#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_BINARY_FILE_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_BINARY_FILE_H_

#include <stdio.h>  // for FILE
struct Router;

// A note on the constness:
// The content of binFile should not change, also we don't want to change the
// pointer to something else.
// Hence we will have that binFile is a const pointer to a char which is const
// In the routerArray we will change both the pointer to the array and the
// content of the array
// At last, we can change the content of N, but we don't want to change what it
// points to
// Hence, the pointer should be const but not the value
// Remember to read from right to left:
// N is a const pointer to an unsigned int

// Note that the top level const is irrelevant from the caller perspective as
// the variable gets its own local scope within the function
// One way of think about this is to think about what happens to an int when
// it's passed by value
// See also:
// https://abseil.io/tips/109#:~:text=Never%20use%20top%2Dlevel%20const,and%20it%20could%20mislead%20readers

int readBinaryFile(const char* const binFile, struct Router** routerArray,
                   unsigned int* const N);
int readNewline(FILE* fp);
// In read router we will change the file pointer as we read and the router
// itself
int readRouter(FILE* fp, struct Router* router);
int readAndSetNeighbors(FILE* fp, struct Router* const routerArray,
                        unsigned int const N, size_t* pairNumber);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_BINARY_FILE_H_
