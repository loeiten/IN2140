/*
 * Assignment:
 *
 * A
 * Write the function `int ishex(unsigned char c)` which checks whether `c` is
 * a hex-number (0'-'9', 'a'-'f' or 'A'-'F')
 *
 * B
 * Write the function `int hexval(unsigned char c)` which converts a hex-number
 * to its decimal value 0-15.
 * Use the function from A to check that `c` has a valid value.
 * Deal with potential exceptions in an appropriate way.
 */

#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS

int isHex(const unsigned char c) {
  if (((c >= '0') && (c <= '9')) ||  // Number
      ((c >= 'a') && (c <= 'f')) ||  // Lower case
      ((c >= 'A') && (c <= 'F'))     // Upper case
  ) {
    return 1;
  }
  return 0;
}

int hexVal(const unsigned char c) {
  if (isHex(c)) {
    if ((c >= '0') && (c <= '9')) {
      return c - '0';
    } else if ((c >= 'a') && (c <= 'f')) {
      return 10 + c - 'a';
    } else {
      return 10 + c - 'A';
    }
  }
  printf("ERROR: '%c' is not a valid hex-value\n", c);
  return -1;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    printf("Usage: ./%s CHAR\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  unsigned char c = argv[1][0];

  printf("%d", hexVal(c));

  return EXIT_SUCCESS;
}
