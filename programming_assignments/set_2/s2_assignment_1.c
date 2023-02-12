#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for atoi, EXIT_FAILURE, EXIT_SUCCESS

void sum(const int a, const int b, int* result) { *result = a + b; }

int main(int argc, char** argv) {
  if (argc != 3) {
    // NOTE: Base does only work for UNIX systems
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
