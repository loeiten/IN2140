#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL, stderr, size_t
#include <stdlib.h>  // for free, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strcmp, strrchr

#include "../include/binary_file.h"  // for readBinaryFile
#include "../include/router.h"       // for Router

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

  int success = 0;

  // Load binary file
  // Initialize the routers
  struct Router* routerArray = NULL;
  unsigned int N;
  success = readBinaryFile(binFile, &routerArray, &N);
  if (success != EXIT_SUCCESS) {
    // Free the router according to https://stackoverflow.com/a/33170941/2786884
    free(routerArray);
    fprintf(stderr, "Failed to read binary file");
    return EXIT_FAILURE;
  }

  // Check if we are reading one command or a command file
  char* dot = strrchr(commandArg, '.');
  if ((dot != NULL) && (strcmp(dot, ".txt") == 0)) {
    // Open the file
    // getline
    // for
    // runCommand(command)
  } else {
    // runCommand(commandArg);
  }

  // Clean-up
  // Free the router according to https://stackoverflow.com/a/33170941/2786884
  for (size_t i = 0; i < N; ++i) {
    free((void*)routerArray[i].producerModel);
  }
  free(routerArray);
  return EXIT_SUCCESS;
}