#include <assert.h>  // for assert
#include <errno.h>   // for errno
#include <libgen.h>  // for basename
#include <stddef.h>  // for NULL
#include <stdio.h>   // for fprintf, fopen, stderr, FILE
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>  // for strcmp, strerror

#include "../include/binary_file.h"  // for readNewline, readRouter
#include "../include/router.h"       // for printRouter, Router

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

int main(int argc, char** argv) {
  if (argc != 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "readNewline") == 0) {
    testReadNewline();
  } else if (strcmp(argv[1], "readRouter") == 0) {
    testReadRouter();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }
  return EXIT_SUCCESS;
}
