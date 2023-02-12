#include <libgen.h>  // for basename
#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for free, malloc, EXIT_S...
#include <string.h>  // for strlen

#include "../../utils/include/string_helpers.h"  // for getInt

// argv is allocated by the OS, see:
// https://stackoverflow.com/questions/2115301/where-command-line-arguments-are-stored
// https://stackoverflow.com/questions/39626689/how-much-memory-is-allocated-for-argv?noredirect=1&lq=1
int main(int argc, char** argv) {
  if (argc != 2) {
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s STR\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Capture an unsigned integer
  int number;
  getInt(argv[1], &number);
  if (number != -1) {
    printf("Number: %d", number);
    return EXIT_SUCCESS;
  }

  // Allocate to the heap instead of stack to protect against buffer overflow
  // +1 to account for the \0 terminator
  int strBufferLen = strlen(argv[1]) + 1;
  char* msg;
  msg = (char*)malloc(strBufferLen * sizeof(char));

  // Almost always, snprintf is better than strcpy according to cpplint
  // Also see:
  // https://stackoverflow.com/questions/2606539/snprintf-vs-strcpy-etc-in-c
  snprintf(msg, strBufferLen, "%s", argv[1]);
  printf("Input: %s\n", msg);

  free(msg);

  return EXIT_SUCCESS;
}
