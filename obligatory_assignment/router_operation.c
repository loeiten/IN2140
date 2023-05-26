#include <libgen.h>  // for basename
#include <stddef.h>
#include <stdio.h>   // for printf, fclose, ferror, fopen, fread, size_t, FILE
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS, NULL
#include <string.h>

struct Router {
  unsigned char routerId;
  unsigned char flag;
  char* producerModel;
  struct Router* neighbors[10];  // Assignment specifies that a router can have
                                 // maximum 10 connections
};

int readBinaryFile(const char* binFile, struct Router** routerArray,
                   unsigned int* N);
int readRouter(FILE* fp, struct Router* router);

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
  struct Router* routerArray = NULL;
  unsigned int N;
  success = readBinaryFile(binFile, &routerArray, &N);
  if (success != EXIT_SUCCESS) {
    // Free the router according to https://stackoverflow.com/a/33170941/2786884
    for (size_t i = 0; i < N; ++i) {
      free(&routerArray[i]);
    }
    printf("Failed to read binary file");
    return EXIT_FAILURE;
  }

  // Check if we are reading one command or a command file

  // Clean-up
  // Free the router according to https://stackoverflow.com/a/33170941/2786884
  for (size_t i = 0; i < N; ++i) {
    free(&routerArray[i]);
  }
  return EXIT_SUCCESS;
}

int readBinaryFile(const char* binFile, struct Router** routerArray,
                   unsigned int* N) {
  // Initialize success
  int success;

  // Open the file
  FILE* fp;
  fp = fopen(binFile, "rb");
  if (fp == NULL) {
    printf("%s not found", binFile);
    return EXIT_FAILURE;
  }

  // Read the number of records
  size_t nBytes = 4;  // Given in the assignment
  size_t nItems = 1;
  int readBytes = fread(N, nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fclose(fp);
    printf("Failed to read from '%s'\n", binFile);
    return EXIT_FAILURE;
  }

  // Malloc the routerArray
  *routerArray = (struct Router*)malloc(*N * sizeof(struct Router));

  for (size_t i = 0; i < *N; ++i) {
    success = readRouter(fp, *(&(routerArray[i])));
    if (success != EXIT_SUCCESS) {
      // Free only the malloced strings
      // If readRouter failed on reading the producerModel the malloced string
      // will already have been freed
      for (size_t j = 0; j < i; ++j) {
        free(routerArray[i]->producerModel);
      }
      fclose(fp);
      printf("Failed to read router %zu\n", i);
      return EXIT_FAILURE;
    }
  }

  fclose(fp);

  printf("Found %u records\n", *N);

  return EXIT_SUCCESS;
}

int readRouter(FILE* fp, struct Router* router) {
  size_t nBytes = 1;  // Given in the assignment
  size_t nItems = 1;

  // Read router id
  int readBytes = fread(&(router->routerId), nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fclose(fp);
    printf("Failed to read routerId\n");
    return EXIT_FAILURE;
  }

  // Read flag
  readBytes = fread(&(router->flag), nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fclose(fp);
    printf("Failed to read flag\n");
    return EXIT_FAILURE;
  }

  // Read the length of the char
  unsigned char charLen;
  readBytes = fread(&charLen, nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fclose(fp);
    printf("Failed to read charLen\n");
    return EXIT_FAILURE;
  }

  // Read the producerModel
  router->producerModel = (char*)malloc((charLen + 1) * sizeof(char));
  readBytes = fread(router->producerModel, charLen, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    // Free the producerModel here we do not indicate whether or not is has been
    // allocated outside this function
    free(router->producerModel);
    fclose(fp);
    printf("Failed to read producerModel\n");
    return EXIT_FAILURE;
  }

  // Null terminate
  router->producerModel[readBytes] = '\0';
  return EXIT_SUCCESS;
}
