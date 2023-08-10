/*
 * Assignment:
 *
 * A
 * The following program
 *
 *    #include <stdio.h>
 *
 *    int main() {
 *    short a, b, sum;
 *
 *    a = 20000; b = 20000; sum = a+b;
 *    printf("%d + %d = %d\n", a, b, sum);
 *    }
 *
 * gives the following result when its run
 *
 *    20000 + 20000 = -25536
 *
 * why does this happen?
 *
 * B
 * Extend the program from A such that it detects the problem and gives an error
 * message when `sum` is wrong.
 * When you've solve the assignment for `short`, try to solve it for `long` and
 * find appropriate values to trigger the error.
 * Does your solution still work?
 */

#include <limits.h>  // for LONG_MAX
#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_SUCCESS

int main(void) {
  short a = 20000;
  short b = 20000;
  short sum = a + b;

  if ((sum < a) || (sum < b)) {  // cppcheck-suppress knownConditionTrueFalse
    printf("WARNING: Overflow detected when using short\n");
  }

  printf("%d + %d = %d\n", a, b, sum);

  /*
   * long overflow is undefined behaviour, see
   * https://stackoverflow.com/questions/18195715/why-is-unsigned-integer-overflow-defined-behavior-but-signed-integer-overflow-is
   * long lA = LONG_MAX;
   * long lB = 1;
   * long lSum = lA + lB;

   * if ((lSum < lA) || (lSum < lB)) {
   *   printf("WARNING: Overflow detected when using long\n");
   * }

   * printf("%ld + %ld = %ld\n", lA, lB, lSum);
  */

  long lA2 = LONG_MAX - 1;
  long lB2 = 1;
  long lSum2 = lA2 + lB2;

  if ((lSum2 < lA2) ||
      (lSum2 < lB2)) {  // cppcheck-suppress knownConditionTrueFalse
    printf("WARNING: Overflow detected when using long\n");
  }

  printf("%ld + %ld = %ld\n", lA2, lB2, lSum2);

  return EXIT_SUCCESS;
}
