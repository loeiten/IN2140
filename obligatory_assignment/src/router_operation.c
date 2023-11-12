#include <errno.h>   // for errno
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL, stderr, feof
#include <stdlib.h>  // for free, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strerror, strcmp, strrchr

#include "../include/binary_file.h"     // for readBinaryFile, writeBinaryFile
#include "../include/command.h"         // for getCommand, runCommand
#include "../include/dynamic_memory.h"  // for freeRoutersAndCommand, freeRo...

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
  const char* dot = strrchr(commandArg, '.');
  if ((dot != NULL) && (strcmp(dot, ".txt") == 0)) {
    // Open the file
    FILE* fp;
    fp = fopen(commandArg, "r");
    if (fp == NULL) {
      fprintf(stderr, "Cannot open %s: %s\n", commandArg, strerror(errno));
      return EXIT_FAILURE;
    }

    char* line = NULL;
    size_t len = 0;
    int i = 1;
    while ((!feof(fp)) && (!ferror(fp))) {
      // Read the line
      // NOTE: getline is from POSIX.1-2008, not the C-standard, see
      // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getline.html
      // for specification, and
      // https://github.com/NetBSD/src/blob/trunk/tools/compat/getline.c
      // for possible implementation
      // ssize_t is when the output can be negative
      ssize_t nBytes = getline(&line, &len, fp);
      // We first get a end of stream *after* a failed read
      if (feof(fp)) {
        break;
      }
      if ((nBytes == -1) || ferror(fp)) {
        fprintf(stderr, "Failed to read line %d of %s: %s\n", i, commandArg,
                strerror(errno));
        free(line);
        freeRoutersAndCommand(&routerArray, N, &command, &args, nArgs);
        return EXIT_FAILURE;
      }
      ++i;

      // Obtain the command
      success = getCommand(line, &command, &args, &nArgs);
      if (success != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to obtain command\n");
        freeRoutersAndCommand(&routerArray, N, &command, &args, nArgs);
        return EXIT_FAILURE;
      }

      // Run the command
      success =
          runCommand(command, (const char* const* const)args, &routerArray, &N);
      if (success != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to run %s\n", commandArg);
        freeRoutersAndCommand(&routerArray, N, &command, &args, nArgs);
        return EXIT_FAILURE;
      }
    }
    free(line);
  } else {
    success = getCommand(commandArg, &command, &args, &nArgs);
    if (success != EXIT_SUCCESS) {
      freeRoutersAndCommand(&routerArray, N, &command, &args, nArgs);
      fprintf(stderr, "Failed to obtain command\n");
      return EXIT_FAILURE;
    }
    success =
        runCommand(command, (const char* const* const)args, &routerArray, &N);
    if (success != EXIT_SUCCESS) {
      fprintf(stderr, "Failed to run %s\n", commandArg);
      freeRoutersAndCommand(&routerArray, N, &command, &args, nArgs);
      return EXIT_FAILURE;
    }
  }

  // Store the router
  // Copy the commandStr as strtok_r will alter the string
  char* writeBinFile = NULL;
  int strLen =
      snprintf(NULL, 0, "%s/%s", "storedBinFile", basename((char*)binFile));
  writeBinFile = (char*)malloc((strLen + 1) * sizeof(char));
  if (writeBinFile == NULL) {
    perror("Could not allocate memory to writeBinFile: ");
    return EXIT_FAILURE;
  }
  int charWritten = snprintf(writeBinFile, (strLen + 1), "%s/%s",
                             "storedBinFile", basename((char*)binFile));
  if ((charWritten < 0) || (charWritten > strLen)) {
    freeRoutersAndCommand(&routerArray, N, &command, &args, nArgs);
    free(writeBinFile);
    return EXIT_FAILURE;
  }
  success = writeBinaryFile(writeBinFile, routerArray, N);
  if (success != EXIT_SUCCESS) {
    printf("Failed to write to %s\n", writeBinFile);
    // Free the router
    freeRoutersAndCommand(&routerArray, N, &command, &args, nArgs);
    free(writeBinFile);
    return EXIT_FAILURE;
  }

  // Free the router
  freeRoutersAndCommand(&routerArray, N, &command, &args, nArgs);
  free(writeBinFile);

  return EXIT_SUCCESS;
}
