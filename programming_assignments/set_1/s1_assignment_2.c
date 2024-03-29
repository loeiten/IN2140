/*
 * Assignment:
 *
 * Write a function char `strgetc(char s[], int pos)` which returns the
 * character at position `pos` in the text `s`.
 * This should be equivalent to `String.charAt()` in Java.
 * Also write a small test program.
 */

#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_S...
#include <string.h>  // for strlen

#include "../../utils/include/string_helpers.h"  // for getInt

char strgetc(const char* c, int pos) {
  // Error checking
  // +1 as pos is zero indexed
  if (((pos) < 0) || ((pos + 1) > (long)strlen(c))) {
    // 0 is reserved for the null charater
    return 0;
  }

  return c[pos];
}

int main(int argc, char** argv) {
  if (argc != 3) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    printf("Usage: ./%s STR INT\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  int pos;
  int errorNo = getInt(argv[2], &pos);

  if (errorNo != 0) {
    printf("Could not capture the position from %s\n", argv[2]);
    return EXIT_FAILURE;
  }

  char c = strgetc(argv[1], pos);
  if (c == 0) {
    printf("Error when finding position %s of '%s' with length %lu\n", argv[2],
           argv[1], (unsigned long)strlen(argv[1]));
    return EXIT_FAILURE;
  }

  printf("Position %s in '%s' is %c", argv[2], argv[1], c);

  return EXIT_SUCCESS;
}
