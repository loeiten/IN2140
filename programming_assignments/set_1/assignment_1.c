#include <stdio.h>

// argv is allocated by the OS, see:
// https://stackoverflow.com/questions/2115301/where-command-line-arguments-are-stored
// https://stackoverflow.com/questions/39626689/how-much-memory-is-allocated-for-argv?noredirect=1&lq=1
int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: ./%s STR", argv[0]);
    return 1;
  }
  printf("Input: %s\n", argv[1]);
  return 0;
}
