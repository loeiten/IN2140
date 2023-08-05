#include <assert.h>    // for assert
#include <errno.h>     // for errno
#include <libgen.h>    // for basename
#include <stddef.h>    // for NULL, size_t
#include <stdio.h>     // for fprintf, fopen, printf, stderr
#include <stdlib.h>    // for EXIT_SUCCESS, EXIT_FAILURE, free
#include <string.h>    // for strcmp, strerror
#include <sys/stat.h>  // for stat

#include "../include/binary_file.h"  // for readNewline, getDirectories, mak...
#include "../include/router.h"       // for printRouter, Router, printNeighbors
#include "include/helpers.h"         // for removeRecursively

#define N (3)

void testReadBinaryFile() {
  struct Router* routerArray = NULL;
  unsigned int n;
  int success = readBinaryFile("../assignment_files/5_routers_fully_connected",
                               &routerArray, &n);
  assert(success == EXIT_SUCCESS);
  printRouter(routerArray, n, 0);
  printRouter(routerArray, n, 1);
  printRouter(routerArray, n, 2);
  printRouter(routerArray, n, 3);
  printRouter(routerArray, n, 4);
}

void testReadNewline() {
  // Test success
  const char* const binFile = "data/newline";
  FILE* fp = fopen(binFile, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", binFile, strerror(errno));
  }
  int success = readNewline(fp);
  assert(success == EXIT_SUCCESS);

  // Test failure
  // The next char in the file should be 'm'
  success = readNewline(fp);
  assert(success == EXIT_FAILURE);
  // We will read the readNewline before the EOF
  success = readNewline(fp);
  assert(success == EXIT_SUCCESS);
  // We will read the EOF
  success = readNewline(fp);
  assert(success == EXIT_FAILURE);
}

void testReadRouter() {
  // Open file
  const char* const binFile = "data/testRouter";
  FILE* fp = fopen(binFile, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", binFile, strerror(errno));
  }
  // Create the router
  struct Router routerArray[1];

  // Call readRouter
  int success = readRouter(fp, &routerArray[0]);
  assert(success == EXIT_SUCCESS);

  // Print the result
  printRouter(routerArray,
              1,    // N
              87);  // routerId
}

void testReadAndSetNeighbors() {
  // Open file
  const char* const binFile = "data/testNeighbors";
  FILE* fp = fopen(binFile, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", binFile, strerror(errno));
  }

  // Create testRouters
  struct Router routerArray[N] = {
      {.routerId = 42}, {.routerId = 7}, {.routerId = 88}};

  // Initialize the routers
  for (int r = 0; r < N; ++r) {
    for (int i = 0; i < MAX_NEIGHBORS; ++i) {
      routerArray[r].neighbors[i] = -1;
    }
  }

  size_t pairNumber = 0;
  int success = readAndSetNeighbors(fp, routerArray, N, &pairNumber);
  assert(success == EXIT_SUCCESS);
  assert(pairNumber == 3);

  // Print the neighbors
  for (int r = 0; r < N; ++r) {
    printNeighbors(routerArray[r].neighbors);
  }

  return;
}

void testGetDirectories(const char* binFile) {
  char* directories = NULL;
  int success = getDirectories(binFile, &directories);
  assert(success == EXIT_SUCCESS);
  if (directories != NULL) {
    printf("%s\n", directories);
  } else {
    printf("\n");
  }
  if (directories != NULL) {
    free(directories);
    directories = NULL;
  }
}

void testMakeDirectories(const char* directories) {
  int success = makeDirectories(directories);
  assert(success == EXIT_SUCCESS);
  struct stat st = {0};
  assert(stat(directories, &st) == 0);
  // Clean-up
  success = removeRecursively(directories);
  assert(success == EXIT_SUCCESS);
  printf("Success\n");
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

  if (strcmp(argv[1], "readBinaryFile") == 0) {
    testReadBinaryFile();
  } else if (strcmp(argv[1], "readNewline") == 0) {
    testReadNewline();
  } else if (strcmp(argv[1], "readRouter") == 0) {
    testReadRouter();
  } else if (strcmp(argv[1], "readAndSetNeighbors") == 0) {
    testReadAndSetNeighbors();
  } else if (strcmp(argv[1], "getDirectories") == 0) {
    testGetDirectories(argv[2]);
  } else if (strcmp(argv[1], "makeDirectories") == 0) {
    testMakeDirectories(argv[2]);
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }
  return EXIT_SUCCESS;
}
