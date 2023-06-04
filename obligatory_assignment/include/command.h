#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_

#include <stddef.h>

int getCommand(const char* commandStr, char* command, char** args,
               size_t* nArgs);
int runCommand(const char* command, ...);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_
