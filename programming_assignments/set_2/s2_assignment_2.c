#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_SUCCESS

int main(int argc, char** argv) {
  short arr[] = {2, 1, -1, 10, 22, 21};
  int arrLen = 6;

  for (int i = 0; i < arrLen; ++i) {
    printf("[%d]: %d\n", i, *(arr + i));
  }

  return EXIT_SUCCESS;
}
