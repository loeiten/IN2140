#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for atoi, EXIT_FAILURE, EXIT_SUCCESS

void sum(const int a, const int b, int* result) { *result = a + b; }

int main(int argc, char** argv) {
  if (argc != 3) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    printf("Usage: ./%s INT INT\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Capture the numbers
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  int result;

  sum(a, b, &result);

  // Print the result
  printf("%d\n", result);

  return EXIT_SUCCESS;
}
