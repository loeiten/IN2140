/*
 * Assignment:
 *
 * We will write object oriented in C.
 * We have a class `Person` with the methods `setName`, `setAge`, `getName` and
 * `getAge`.
 * Since we don't have classes in C we have to solve this a bit differently.
 * The assignment is to figure out how to do this. Here are some hints:
 * - Make a struct that can contain the information about a person
 * - Make the functions `getName`, `setAge` etc. such that the first parameter
 *   points to the person-struct
 *
 * A
 * Implement the simplest possible solution
 *
 * B
 * Think through the names of your function and struct.
 * Recall that all names are global in C.
 * Change the names as you see fit.
 *
 * C
 * Add two functions which checks whether the name and age of the person is set.
 * They should return 1 for true and 0 for false.
 *
 * D
 * Make a function which return a string with the information about a person.
 * The function should work in a similar manner as `toString()` in Java.
 *
 * E
 * Make a function which makes a person-struct with the same format as what you
 * made in assignment D
 *
 * F
 * Think through how errors can be handled in a way that can be used in all the
 * functions you've defined so far
 *
 * G
 * Add the restrictions:
 *    - The age can not surpass 200 years
 *    - The name can be at most 30 characters
 * Also implement error handling
 */

#include <libgen.h>  // for basename
#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for EXIT_FAILURE, free
#include <string.h>  // for strlen

#include "../../utils/include/person.h"          // for printPerson, setAge
#include "../../utils/include/string_helpers.h"  // for getInt

int main(int argc, char** argv) {
  if (argc < 3) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    printf("Usage: ./%s STR INT ...\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  int i = 1;
  while (i < argc) {
    // Always initialize the person to some non-valid values
    struct Person person = {"null", -1};

    // Capture name
    char* name;
    int bufferLen = strlen(argv[i]) + 1;
    name = (char*)malloc(bufferLen * sizeof(char));
    snprintf(name, bufferLen, "%s", argv[i]);

    // Capture age
    ++i;
    if (i == argc) {
      printf("Not enough arguments given\n");
      free(name);
      return EXIT_FAILURE;
    }

    int age;
    int exitFailure = getInt(argv[i], &age);
    if (exitFailure) {
      printf("Failed to capture the age from '%s'", argv[i]);
      free(name);
      return EXIT_FAILURE;
    }

    // Create person
    setName(&person, name);
    setAge(&person, age);

    // Print the person
    printPerson(&person);

    // Clean-up
    free(name);

    ++i;
  }

  return EXIT_SUCCESS;
}
