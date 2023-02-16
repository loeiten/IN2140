#include <libgen.h>  // for basename
#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for EXIT_SUCCESS, malloc, EXIT_FAILURE
#include <string.h>  // for strlen

#include "../../utils/include/person.h"

int getNumberOfLines() {
  // Based on https://stackoverflow.com/a/70708991/2786884
  return EXIT_SUCCESS;
}

int readRegister(const char* registerPath, struct Person* personArray,
                 int* personArrayLen) {
  // Get number of lines

  // Get number of entries (subtract 2 due to header)
  // Read entries and store it to the personArray
  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
  // NOTE: We want to have the whole register in memory, if not, we should've
  //       traversed the file line by line
  // Read register
  // Read whom to remove
  // Remove remove from the register
  // Read whom to add
  // Add to the register
  // Store the register to a "dirty file"
  // Read the register from the "dirty file"
  // Print the register

  if (argc < 2) {
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s registerPath [removeFilePath] [addFilePath]\n",
           basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Obtain the registerPath
  int strBufferLen = strlen(argv[1]) + 1;
  char* registerPath = (char*)malloc(strBufferLen * sizeof(char));
  snprintf(registerPath, strBufferLen, "%s", argv[1]);

  // Read register
  struct Person* personArray = NULL;
  int personArrayLen;
  int exitFailure = readRegister(registerPath, personArray, &personArrayLen);
  if (exitFailure) {
    printf("Failed to read the register");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
