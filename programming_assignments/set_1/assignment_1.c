#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// argv is allocated by the OS, see:
// https://stackoverflow.com/questions/2115301/where-command-line-arguments-are-stored
// https://stackoverflow.com/questions/39626689/how-much-memory-is-allocated-for-argv?noredirect=1&lq=1
int main(int argc, char** argv) {
  char* msg;

  if (argc < 2) {
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s STR\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Allocate to the heap instead of stack to protect against buffer overflow
  // +1 to account for the \0 terminator
  msg = (char*)malloc((strlen(argv[1]) + 1) * sizeof(char));

  if (msg == NULL) {
    free(msg);
    printf("Memory not allocated\n");
    return EXIT_FAILURE;
  }

  // Almost always, snprintf is better than strcpy according to cpplint
  // Also see:
  // https://stackoverflow.com/questions/2606539/snprintf-vs-strcpy-etc-in-c
  snprintf(msg, sizeof(msg), "%s", argv[1]);
  printf("Input: %s\n", msg);

  free(msg);

  return EXIT_SUCCESS;
}
