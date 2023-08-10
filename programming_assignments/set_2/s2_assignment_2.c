/*
 * Assignment:
 *
 * Rewrite this for-loop so that it uses pointers instead of normal array
 * indices (such as `arr[i]`)
 *
 *     int i;
 *     short arr[] = {2, 1, -1, 10, 22, 21};
 *     int arrlen = 6;
 *     for(i = 0; i &lt; arrlen; i++)
 *         printf("[%d]: %d\n", i, arr[i]);
 */

#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_SUCCESS

int main(void) {
  short arr[] = {2, 1, -1, 10, 22, 21};
  int arrLen = 6;

  for (int i = 0; i < arrLen; ++i) {
    printf("[%d]: %d\n", i, *(arr + i));
  }

  return EXIT_SUCCESS;
}
