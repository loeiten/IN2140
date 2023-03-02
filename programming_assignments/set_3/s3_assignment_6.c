/*
 * Assignment:
 *
 * Write the library `mymath` with functions to sum, multiply and divide.
 * It's up to you whether you choose to support only one or several datatypes.
 *
 * The library should consist of the files `mymath.h` and `mymath.c` such that
 * `mymath.c` can be compiled against programs which uses the library and
 * `mymath.h` can be included against programs which uses the library.
 *
 * Make a test program and a Makefile
 */

#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_SUCCESS

#include "../../utils/include/my_math.h"  // for divide, multiply, sum

int main() {
  int s = sum(1, 9);
  int m = multiply(s, 10);
  int d = divide(m, 5);
  printf("%d\n", d);
  return EXIT_SUCCESS;
}
