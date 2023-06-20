#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_DYNAMIC_MEMORY_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_DYNAMIC_MEMORY_H_

#include <stddef.h>  // for size_t
struct Router;

// Clean-up according to https://stackoverflow.com/a/33170941/2786884
// and https://stackoverflow.com/a/722993/2786884
// A note on constness:
// When freeing memory we are freeing everything, so nothing is allowed to be
// const
void freeRouterArray(struct Router** routerArray, size_t N);
void freeCommand(char** command, char*** args, size_t nArgs);
void freeRoutersAndCommand(struct Router** routerArray, size_t N,
                           char** command, char*** args, size_t nArgs);

void freeCommandStrCpy(char** commandStrCpy, const char* const errorMsg);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_DYNAMIC_MEMORY_H_
