#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL, stderr, size_t
#include <stdlib.h>  // for free, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strcmp, strrchr

#include "../include/binary_file.h"     // for readBinaryFile
#include "../include/command.h"         // for getCommand
#include "../include/dynamic_memory.h"  // for freeRouterArray

// argv is allocated by the OS, see:
// https://stackoverflow.com/questions/2115301/where-command-line-arguments-are-stored
// https://stackoverflow.com/questions/39626689/how-much-memory-is-allocated-for-argv?noredirect=1&lq=1
int main(int argc, char** argv) {
  if (argc != 3) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s binFile COMMAND|COMMAND_FILE.txt\n",
            basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Capture the binFile
  const char* binFile = argv[1];
  // Capture the command
  const char* commandArg = argv[2];

  // Initialize malloced memory
  struct Router* routerArray = NULL;
  char* command = NULL;
  char** args = NULL;

  // Initialize and declare helper variables
  int success = EXIT_FAILURE;
  unsigned int N;
  size_t nArgs;

  // Load binary file
  // Initialize the routers
  success = readBinaryFile(binFile, &routerArray, &N);
  if (success != EXIT_SUCCESS) {
    freeRouterArray(&routerArray, N);
    fprintf(stderr, "Failed to read binary file\n");
    return EXIT_FAILURE;
  }

  // Check if we are reading one command or a command file
  char* dot = strrchr(commandArg, '.');
  if ((dot != NULL) && (strcmp(dot, ".txt") == 0)) {
    // Open the file
    // getline
    // for
    // runCommand(command)
    success = getCommand(commandArg, &command, &args, &nArgs);
    if (success != EXIT_SUCCESS) {
      freeRouterArray(&routerArray, N);
      freeCommand(&command, &args, nArgs);
      fprintf(stderr, "Failed to obtain command\n");
      return EXIT_FAILURE;
    }
  } else {
    success = getCommand(commandArg, &command, &args, &nArgs);
    if (success != EXIT_SUCCESS) {
      freeRouterArray(&routerArray, N);
      freeCommand(&command, &args, nArgs);
      fprintf(stderr, "Failed to obtain command\n");
      return EXIT_FAILURE;
    }
    // runCommand(commandArg);
  }

  // Free the router
  freeRouterArray(&routerArray, N);
  freeCommand(&command, &args, nArgs);

  return EXIT_SUCCESS;
}
