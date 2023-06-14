#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_DYNAMIC_MEMORY_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_DYNAMIC_MEMORY_H_

#include <stddef.h>  // for size_t
struct Router;

// Clean-up according to https://stackoverflow.com/a/33170941/2786884
// and https://stackoverflow.com/a/722993/2786884

void freeRouterArray(struct Router** routerArray, size_t N);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_DYNAMIC_MEMORY_H_
