/*
 * Assignment:
 *
 * Write a macro
 *
 *     #define STREQ(s1,s2)
 *
 * which checks whether the two strings are equal.
 * The purpose is to use this in if-tests like
 *
 *     if(STREQ(s, "Abc"))
 */

#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strlen

#define STREQ(s1, s2) (strcmp(s1, s2) == 0)

/*
 * The following uses GNU statement expression extension
 *
 * #define STREQ(s1, s2)                        \
 *   ({                                         \
 *     int result = 1;                          \
 *     if (strlen(s1) != strlen(s2)) {          \
 *       result = 0;                            \
 *     } else {                                 \
 *       for (unsigned long i = 0; i < strlen(s1); ++i) { \
 *         if ((s1[i] != s2[i])) {              \
 *           result = 0;                        \
 *           break;                             \
 *         }                                    \
 *       }                                      \
 *     }                                        \
 *     result;                                  \
 *   })
 */

int main(int argc, char** argv) {
  if (argc != 3) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
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
