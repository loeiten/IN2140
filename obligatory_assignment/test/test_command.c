#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, printf, size_t, stderr
#include <stdlib.h>  // for atoi, EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>  // for strcmp

#include "../include/command.h"  // for getCommand

void testGetCommand(const char* const commandStr,
                    const char* const expectedCommand,
                    const char* const expectedNArgs) {
  char* command;
  char** args;
  size_t nArgs;

  int success = getCommand(commandStr, &command, &args, &nArgs);
  assert(success == EXIT_SUCCESS);
  assert(strcmp(command, expectedCommand) == 0);
  assert(nArgs == (size_t)atoi(expectedNArgs));
  for (int i = 0; i < nArgs; ++i) {
    printf("%s\n", args[i]);
  }
  return;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "getCommand") == 0) {
    testGetCommand(argv[2], argv[3], argv[4]);
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }
  return EXIT_SUCCESS;
}
