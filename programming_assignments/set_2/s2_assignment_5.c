#include <libgen.h>  // for basename
#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for EXIT_FAILURE, free
#include <string.h>  // for strlen

#include "../../utils/include/person.h"          // for printPerson, setAge
#include "../../utils/include/string_helpers.h"  // for getInt

int main(int argc, char** argv) {
  if (argc < 3) {
    // NOTE: Base does only work for UNIX systems
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
      return EXIT_FAILURE;
    }

    int age;
    int exitFailure = getInt(argv[i], &age);
    if (exitFailure) {
      printf("Failed to capture the age from '%s'", argv[i]);
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
