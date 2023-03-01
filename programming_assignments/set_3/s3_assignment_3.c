/*
 * Assignment:
 *
 * When using `malloc()` we need to free the memory when we're done with it.
 * Write a program which uses `malloc()` to allocate an area in the memory
 * before you fill it with text and write it out.
 * Finally the program should free the memory before terminating.
 * To test whether you have succeed use `valgrind` or `leaks`.
 * If you are successful you should get the following message
 *
 *    All heap blocks were freed -- no leaks are possible
 *
 * or
 *
 *    0 leaks for 0 total leaked bytes
 */

#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for free, malloc, EXIT_SUCCESS
#include <string.h>  // for strlen

int main() {
  char* strLiteral =
      "I'm a string literal copied to the heap before being printed";
  int bufLen = strlen(strLiteral) + 1;
  char* msg = (char*)malloc(bufLen * sizeof(char));
  snprintf(msg, bufLen, "%s", strLiteral);
  printf("%s\n", msg);
  free(msg);
  return EXIT_SUCCESS;
}
