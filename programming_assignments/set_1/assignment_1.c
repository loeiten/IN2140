#include <ctype.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// argv is allocated by the OS, see:
// https://stackoverflow.com/questions/2115301/where-command-line-arguments-are-stored
// https://stackoverflow.com/questions/39626689/how-much-memory-is-allocated-for-argv?noredirect=1&lq=1
int main(int argc, char** argv) {
  int i;
  int len;
  int number;
  int strBufferLen;
  char* msg;

  if (argc < 2) {
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s STR\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Check if we are passed an unsigned integer
  len = strlen(argv[1]);
  i = 0;
  number = 0;

  while ((isdigit(argv[1][i])) && (i < len)) {
    number *= 10;
    number += argv[1][i] - '0';
    ++i;
  }

  if (i == len) {
    printf("Number: %d", number);
    return EXIT_SUCCESS;
  }

  // Allocate to the heap instead of stack to protect against buffer overflow
  // +1 to account for the \0 terminator
  strBufferLen = len + 1;
  msg = (char*)malloc(strBufferLen * sizeof(char));

  // Almost always, snprintf is better than strcpy according to cpplint
  // Also see:
  // https://stackoverflow.com/questions/2606539/snprintf-vs-strcpy-etc-in-c
  snprintf(msg, strBufferLen, "%s", argv[1]);
  printf("Input: %s\n", msg);

  free(msg);

  return EXIT_SUCCESS;
}
