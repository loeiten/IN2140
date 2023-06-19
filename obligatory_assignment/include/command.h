#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_

#include <stddef.h>  // for size_t
struct Router;

int getCommand(const char* commandStr, char** command, char*** args,
               size_t* nArgs);
int runCommand(const char* command, const char* const* args,
               const struct Router* routerArray, unsigned int N);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_
