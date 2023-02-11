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

  for (int i = 0; i < strlen(s1); ++i) {
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

int main() {
  test("Abc", "Ab");
  test("Abc", "Abc");
  test("Abc", "Abcd");
  test("Abd", "Abc");
  test("Abc", "Abd");
  test("abc", "Abc");
  test("Abc", "abc");

  return EXIT_SUCCESS;
}
