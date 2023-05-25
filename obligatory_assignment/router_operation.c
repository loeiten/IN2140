#include <libgen.h>  // for basename
#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for free, malloc, EXIT_S...
#include <string.h>  // for strlen

struct Router {
  unsigned char routerId;
  unsigned char flag;
  char* producerModel;
  struct Router* neighbors[10];  // Assignment specifies that a router can have
                                 // maximum 10 connections
};

int readBinaryFile(const char* binFile, struct Router** routers);

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
    printf("Usage: ./%s binFile COMMAND|COMMAND_FILE.txt\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Capture the binFile
  const char* binFile = argv[1];

  // Capture the command
  const char* commandArg = argv[2];

  int success = 0;

  // Load binary file
  // Initialize the routers
  struct Router* routers = NULL;
  success = readBinaryFile(binFile, &routers);
  if (success != EXIT_SUCCESS) {
    printf("Failed to read binary file");
    return EXIT_FAILURE;
  }

  // Check if we are reading one command or a command file
}

int readBinaryFile(const char* binFile, struct Router** routers) {
  // Open the file
  FILE* fp;
  fp = fopen(binFile, "rb");
  if (fp == NULL) {
    printf("%s not found", binFile);
    return EXIT_FAILURE;
  }

  // Read the number of records
  unsigned int N;
  size_t nBytes = 4;  // Given in the assignment
  size_t nItems = 1;
  int readBytes = fread(&N, nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fclose(fp);
    printf("Failed to read from '%s'\n", binFile);
    return EXIT_FAILURE;
  }

  printf("Found %d records\n", N);

  return EXIT_SUCCESS;
}
