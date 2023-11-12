#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL
#include <stdlib.h>  // for EXIT_SUCCESS
#include <string.h>  // for strcmp

void testGetTCPServerSocket(void) { assert(1 == 0); }

void testPopulateNodeArray(void) { assert(1 == 0); }

void testSendRoutingTables(void) { assert(1 == 0); }

void testTranslateTableFromIdxToAddress(void) { assert(1 == 0); }

int main(int argc, char** argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n1", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "getTCPServerSocket") == 0) {
    testGetTCPServerSocket();
  } else if (strcmp(argv[1], "populateNodeArray") == 0) {
    testPopulateNodeArray();
  } else if (strcmp(argv[1], "sendRoutingTables") == 0) {
    testSendRoutingTables();
  } else if (strcmp(argv[1], "translateTableFromIdxToAddress") == 0) {
    testTranslateTableFromIdxToAddress();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
