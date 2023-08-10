/*
 * Assignment:
 *
 *
 * The standard function
 *
 *     int strcmp(unsigned char s1[], unsigned char s2[])
 *
 * does a lexicographical comparisons of two strings and returns a value which
 * is
 * < 0 if s1 comes before s2
 * = 0 if s1 and s2 are equal
 * > 0 if s1 comes after s2
 *
 * For example "Bar" comes before "Foo", and after "Abc".
 * Note that "Foo" comes before "Foobar".
 *
 * Write `strcmpx` in C.
 * It should behave similar as `strcmp`
 * Here is a boilerplate
 *
 *     #include <stdio.h>
 *
 *     int strcmpx(unsigned char s1[], unsigned char s2[]) {
 *     // Write your code here
 *     }
 *
 *     void test(unsigned char s1[], unsigned char s2[]) {
 *     printf("strcmpx(\"%s\", \"%s\") gives %d\n",
 *     s1, s2, strcmpx(s1,s2));
 *     }
 *
 *     int main() {
 *     test("Abc", "Ab");
 *     test("Abc", "Abc");
 *     test("Abc", "Abcd");
 *     return 0;
 *     }
 *
 * Ensure that you understand the example.
 * If you are wondering about the syntax for `printf`, look up the man page for
 * `printf`
 */

#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_SUCCESS
#include <string.h>  // for strlen

int strcmpx(const char* s1, const char* s2) {
  if (strlen(s1) > strlen(s2)) {
    return -1;
  }
  if (strlen(s1) < strlen(s2)) {
    return 1;
  }

  for (unsigned long i = 0; i < strlen(s1); ++i) {
    if (s1[i] < s2[i]) {
      return -1;
    }
    if (s1[i] > s2[i]) {
      return 1;
    }
  }

  return 0;
}

void test(const char* s1, const char* s2) {
  printf("strcmpx(\"%s\", \"%s\") gives %d\n", s1, s2, strcmpx(s1, s2));
}

int main(void) {
  test("Abc", "Ab");
  test("Abc", "Abc");
  test("Abc", "Abcd");
  test("Abd", "Abc");
  test("Abc", "Abd");
  test("abc", "Abc");
  test("Abc", "abc");

  return EXIT_SUCCESS;
}
