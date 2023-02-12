#include <limits.h>  // for LONG_MAX
#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_SUCCESS

int main() {
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
