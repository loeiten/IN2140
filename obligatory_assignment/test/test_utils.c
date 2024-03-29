#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, printf, stderr
#include <stdlib.h>  // for atoi, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strcmp

#include "../include/utils.h"  // for getBinaryString

void testGetBinaryString(const char* const number, const char* const expected) {
  // Initialize test
  // 8 for the number of bits
  // +1 for the null terminator
  char binaryFlag[sizeof(unsigned char) * 8 + 1];

  const unsigned char c = (const unsigned char)atoi(number);
  getBinaryString(c, binaryFlag);
  assert(strcmp(binaryFlag, expected) == 0);
  printf("Success\n");
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

  if (strcmp(argv[1], "getBinaryString") == 0) {
    testGetBinaryString(argv[2], argv[3]);
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }
  return EXIT_SUCCESS;
}
