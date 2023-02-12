#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strlen

#define STREQ(s1, s2)                        \
  ({                                         \
    int result = 1;                          \
    if (strlen(s1) != strlen(s2)) {          \
      result = 0;                            \
    } else {                                 \
      for (int i = 0; i < strlen(s1); ++i) { \
        if ((s1[i] != s2[i])) {              \
          result = 0;                        \
          break;                             \
        }                                    \
      }                                      \
    }                                        \
    result;                                  \
  })

int main(int argc, char** argv) {
  if (argc != 3) {
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s STR STR\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (STREQ(argv[1], argv[2])) {
    printf("Strings are the same\n");
  } else {
    printf("Strings are NOT the same\n");
  }

  return EXIT_SUCCESS;
}
