#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_

int getCommand(const char* commandStr, const char* command, const char** args);
int runCommand(const char* command, ...);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_COMMAND_H_
