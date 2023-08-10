/*
 * Assignment:
 *
 * The following program will result in a segmentation fault:
 *
 *    #include <stdlib.h>
 *    #include <stdio.h>
 *
 *    int main() {
 *        char *s = "hello world";
 *        *s = 'H';
 *        printf("%s\n", s);
 *        return 0;
 *    }
 *
 * Use a debugger to figure out why
 */

#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for free, malloc
#include <string.h>  // for strlen

int main(void) {
  // This creates a pointer to a string literal
  // This literal will be part of the executable (which should be read-only)
  // https://stackoverflow.com/q/1011455/2786884
  const char* s = "hello world";
  // One shouldn't modify the string literal (which is part of the executable)
  // *s = 'H';
  printf("%s\n", s);

  // Instead one can copy the content
  // https://stackoverflow.com/a/1011545/2786884
  int strBufferLen = strlen(s) + 1;
  char* sCopy;
  sCopy = (char*)malloc(strBufferLen * sizeof(char));
  snprintf(sCopy, strBufferLen, "%s", s);
  *sCopy = 'H';
  printf("%s\n", sCopy);
  free(sCopy);

  // Alternatively, one could've defined it as an array
  // If one declare a local array, then space is made on the stack for each
  // element of that array, and the string literal (which is stored in the
  // executable) is copied to that space in the stack
  // https://stackoverflow.com/a/1011545/2786884
  char sArray[] = "hello world";
  *sArray = 'H';
  printf("%s\n", sArray);

  return 0;
}
