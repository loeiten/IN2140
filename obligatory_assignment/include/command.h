#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_

#include <stddef.h>  // for size_t

struct Router;

// A note on constness:
// We should not change the commandStr in any way
// We should be allowed to change the content of command, but not the pointer
// itself
// In args we need to allocate the array and change it contents, however the
// pointer should not be allowed to change
// In similar fashion, we shouldn't be allowed to change the pointer of nArgs
int getCommand(const char* const commandStr, char** const command,
               char*** const args, size_t* const nArgs);
// When we run the command nothing is allowed to change
int runCommand(const char* const command, const char* const* const args,
               struct Router** routerArray, unsigned int* N);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_
