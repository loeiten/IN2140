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
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s CHAR\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  unsigned char c = argv[1][0];

  printf("%d", hexVal(c));

  return EXIT_SUCCESS;
}
