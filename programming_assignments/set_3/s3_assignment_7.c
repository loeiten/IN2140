/*
 * Assignment:
 *
 * `printf()` is often used to
 * - Write a message to the user
 * - Write debug information
 * - Write error messages
 *
 * The problem with this is that it's often hard to separate the three.
 * A standard way to solve the problem is to have a system for telling something
 * about *how important* the message is in addition to the message itself.
 *
 * Such system could look like
 *
 *     log_msg(DEBUG, "The value of a is 10.");
 *     log_msg(ERROR, "Something is wrong here!");
 *     log_msg(INFO, "Hello world :)");
 *
 * A
 * Implement a system similar to the one shown above
 *
 * B
 * Add support for selecting the level to be printed through command line
 * arguments.
 * You could use `getopt()`, but it's not required
 */

#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strcmp

#include "../../utils/include/logger.h"  // for logMsg, level, logLevel

int main(int argc, char** argv) {
  if (argc > 1) {
    if (strcmp(argv[1], "DEBUG") == 0) {
      logLevel = DEBUG;
    } else if (strcmp(argv[1], "INFO") == 0) {
      logLevel = INFO;
    } else if (strcmp(argv[1], "ERROR") == 0) {
      logLevel = ERROR;
    } else {
      // NOTE: Base is from POSIX.1-2008, not the C-standard, see
      // https://www.unix.com/man-page/posix/3p/basename/
      // for specification and
      // https://github.com/coreutils/coreutils/blob/master/src/basename.c
      // for possible implementation
      printf("Usage: ./%s [DEBUG|INFO|ERROR]\n", basename(argv[0]));
      return EXIT_FAILURE;
    }
  }

  // Do the logging
  logMsg(DEBUG, "The value of a is 10");
  logMsg(INFO, "Hello world :)");
  logMsg(ERROR, "Something is wrong here");

  printf("%s\n", "Done");

  return EXIT_SUCCESS;
}
