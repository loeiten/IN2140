#include <libgen.h>  // for basename
#include <stddef.h>
#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for EXIT_SUCCESS, malloc, EXIT_FAILURE
#include <string.h>  // for strlen

#include "../../utils/include/person.h"

#define BUFFER_SIZE 65536  // 2^16

int getNumberOfLines(const char* path, int* lines) {
  // Based on https://stackoverflow.com/a/70708991/2786884
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    printf("%s not found", path);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE];
  *lines = 0;

  int nRead = 1;  // Set to 1 to trigger the while loop
  while (nRead > 0) {
    nRead = fread(buffer, sizeof(char), BUFFER_SIZE, fp);
    for (int i = 0; i < nRead; ++i) {
      if (buffer[i] == '\n') {
        ++(*lines);
      }
    }
  }
  if (ferror(fp)) {
    fclose(fp);
    return EXIT_FAILURE;
  }

  fclose(fp);
  return EXIT_SUCCESS;
}

// NOTE: We are changing the pointer so we take a pointer to the pointer as an
//       input
int getNameAge(char* line, char** name, int* age) {
  // Find the length of the string and the length until comma
  int lineLen = strlen(line);
  int i = 0;
  for (; i < lineLen; ++i) {
    if (line[i] == ',') {
      break;
    }
  }

  if ((i == 0) || (i == lineLen)) {
    printf("Failed to split %s into 'name' and 'age", line);
    return EXIT_FAILURE;
  }

  // Increment i once to go from index to counter
  ++i;

  // Capture the name
  // +1 for the /0 char
  *name = (char*)malloc((i + 1) * sizeof(char));
  snprintf(*name, i, "%s", line);

  // Capture the age
  char* ageStr = &(line[i]);
  *age = atoi(ageStr);

  return EXIT_SUCCESS;
}

int readEntries(const char* path, struct Person* personArray,
                int personArrayLen) {
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    printf("%s not found", path);
    return EXIT_FAILURE;
  }

  // NOTE: getline is POSIX, not C-standard
  // https://stackoverflow.com/a/3501681/2786884
  char* line = NULL;
  size_t len = 0;
  ssize_t nBytes;  // ssize_t is when the output can be negative

  // NOTE: getline is from POSIX.1-2008, not the C-standard, see
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getline.html
  // for specification, and
  // https://github.com/NetBSD/src/blob/trunk/tools/compat/getline.c
  // for possible implementation
  // Read the header
  for (int i = 0; i < 2; ++i) {
    nBytes = getline(&line, &len, fp);
    if (nBytes == -1) {
      printf("Failed to read line %d of %s", i, path);
      return EXIT_FAILURE;
    }
  }

  // Read the entries
  for (int i = 0; i < personArrayLen; ++i) {
    nBytes = getline(&line, &len, fp);
    if (nBytes == -1) {
      printf("Failed to read line %d of %s", i + 2, path);
      return EXIT_FAILURE;
    }
    char* curName;
    int curAge;
    // NOTE: We are changing curName, so we call using the address of curName
    int exitFailure = getNameAge(line, &curName, &curAge);
    if (exitFailure) {
      printf("Failed to get entry %d of %s", i, path);
      return EXIT_FAILURE;
    }
    exitFailure = createPerson(curName, curAge, &personArray[i]);
    free(curName);
    if (exitFailure) {
      return EXIT_FAILURE;
    }
  }

  free(line);

  fclose(fp);
  return EXIT_SUCCESS;
}

// NOTE: We change the personArray, hence we must pass a pointer to the array
int readRegister(const char* registerPath, struct Person** personArray,
                 int* personArrayLen) {
  // Get number of lines
  int lines;
  int exitFailure = getNumberOfLines(registerPath, &lines);
  if (exitFailure) {
    printf("Failed to obtain the number of lines of %s", registerPath);
    return EXIT_FAILURE;
  }

  // Get number of entries (subtract 2 due to header)
  (*personArrayLen) = lines - 2;

  // Allocate array to the personArray
  *personArray =
      (struct Person*)malloc((*personArrayLen) * sizeof(struct Person));

  // Read entries and store it to the personArray
  exitFailure = readEntries(registerPath, *personArray, *personArrayLen);
  if (exitFailure) {
    printf("Failed to obtain the entries from %s", registerPath);
    return EXIT_FAILURE;
  }

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
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
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
  int exitFailure = readRegister(registerPath, &personArray, &personArrayLen);
  if (exitFailure) {
    printf("Failed to read the register");
    return EXIT_FAILURE;
  }

  // Print the register
  for (int i = 0; i < personArrayLen; ++i) {
    printPerson(&personArray[i]);
  }

  // Free memory
  free(personArray);

  return EXIT_SUCCESS;
}
