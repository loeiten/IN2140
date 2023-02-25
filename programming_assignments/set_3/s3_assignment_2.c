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

#include <stdio.h>
#include <stdlib.h>

int main() {
  char *s = "hello world";
  *s = 'H';
  printf("%s\n", s);
  return 0;
}
