#include <libgen.h>  // for basename
#include <stdio.h>   // for printf, snprintf, FILE
#include <stdlib.h>  // for EXIT_SUCCESS, malloc, EXIT_FAILURE
#include <string.h>  // for strlen

#include "../../utils/include/file_operations.h"
#include "../../utils/include/person.h"

int readRemoveEntries(const char* path, char** removePersonArray,
                      const int removePersonLen) {
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    printf("%s not found", path);
    return EXIT_FAILURE;
  }

  char* line = NULL;
  size_t len = 0;

  // NOTE: getline is from POSIX.1-2008, not the C-standard, see
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getline.html
  // for specification, and
  // https://github.com/NetBSD/src/blob/trunk/tools/compat/getline.c
  // for possible implementation
  // Read the entries
  for (int i = 0; i < removePersonLen; ++i) {
    ssize_t nBytes;  // ssize_t is when the output can be negative
    nBytes = getline(&line, &len, fp);
    if (nBytes == -1) {
      printf("Failed to read line %d of %s", i, path);
      return EXIT_FAILURE;
    }
    snprintf(removePersonArray[i], MAX_NAME_LEN, "%s", line);
  }

  free(line);
  fclose(fp);
  return EXIT_SUCCESS;
}

// NOTE: We change the personArray, hence we must pass a pointer to the array
void removeFromFile(const char* path, struct Person** personArray,
                    const int personArrayLen) {
  // Get number of lines
  int removePersonLen;
  int exitFailure = getNumberOfLines(path, &removePersonLen);
  if (exitFailure != 0) {
    printf("Failed to obtain the number of lines of %s", path);
    free(personArray);
    exit(EXIT_FAILURE);
  }

  // Allocate array to remove names
  char** removePersonArray = (char**)malloc(removePersonLen * sizeof(char*));
  for (int i = 0; i < removePersonLen; ++i) {
    removePersonArray[i] = malloc((MAX_NAME_LEN + 1) * sizeof(char));
  }

  // Fill the removePersonArray
  exitFailure = readRemoveEntries(path, removePersonArray, removePersonLen);
  if (exitFailure != 0) {
    printf("Failed to read remove entries");
    for (int i = 0; i < removePersonLen; ++i) {
      free(removePersonArray[i]);
    }
    free(removePersonArray);
    free(personArray);
    exit(EXIT_FAILURE);
  }

  // Remove the entries (rather set them to "null" and -1)
  // NOTE: In an ideal world we would search the personArray as a hash-map to
  //       have constant time look-up
  //       Instead we will have O(n) time look-up for each person we're removing
  for (int removeIdx = 0; removeIdx < removePersonLen; ++removeIdx) {
    for (int personIdx = 0; personIdx < personArrayLen; ++personIdx) {
      char name[MAX_NAME_LEN];
      exitFailure = getName(personArray[personIdx], name);
      if (exitFailure != 0) {
        printf("Failed to get person number %d", personIdx);
        for (int i = 0; i < removePersonLen; ++i) {
          free(removePersonArray[i]);
        }
        free(removePersonArray);
        free(personArray);
        exit(EXIT_FAILURE);
      }
      if (strcmp(removePersonArray[removeIdx], name) == 0) {
        exitFailure = setName(personArray[personIdx], "null");
        if (exitFailure) {
          printf("Failed to set name for person number %d", personIdx);
          for (int i = 0; i < removePersonLen; ++i) {
            free(removePersonArray[i]);
          }
          free(removePersonArray);
          free(personArray);
          exit(EXIT_FAILURE);
        }
        exitFailure = setAge(personArray[personIdx], -1);
        if (exitFailure) {
          printf("Failed to set age for person number %d", personIdx);
          for (int i = 0; i < removePersonLen; ++i) {
            free(removePersonArray[i]);
          }
          free(personArray);
          free(removePersonArray);
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  // Free memory
  for (int i = 0; i < removePersonLen; ++i) {
    free(removePersonArray[i]);
  }
  free(removePersonArray);
}

void addFromFile(const char* path, struct Person** personArray,
                 int* personArrayLen, struct Person** extendedPersonArray,
                 int* extendedPersonArrayLen) {
  // Get number of lines
  int lines;
  int exitFailure = getNumberOfLines(path, &lines);
  if (exitFailure) {
    printf("Failed to obtain the number of lines of %s", path);
    exit(EXIT_FAILURE);
  }
  *extendedPersonArrayLen = (*personArrayLen) + lines;

  // Re-allocate memory to take the additions into account
  (*extendedPersonArray) =
      (struct Person*)realloc(personArray, *personArrayLen);

  // Extend with the file
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    printf("Failed to open %s", path);
    exit(EXIT_FAILURE);
  }

  char* line = NULL;
  size_t len = 0;

  // Read the entries
  for (int i = 0; i < lines; ++i) {
    ssize_t nBytes;  // ssize_t is when the output can be negative
    nBytes = getline(&line, &len, fp);
    if (nBytes == -1) {
      printf("Failed to entry %d of %s", i, path);
      exit(EXIT_FAILURE);
    }
    char* curName;
    int curAge;
    // NOTE: We are changing curName, so we call using the address of curName
    exitFailure = strToNameAge(line, &curName, &curAge);
    if (exitFailure) {
      printf("Failed to get entry %d of %s", i, path);
      exit(EXIT_FAILURE);
    }
    exitFailure = createPerson(curName, curAge,
                               extendedPersonArray[i + (*personArrayLen - 1)]);
    free(curName);
    if (exitFailure) {
      exit(EXIT_FAILURE);
    }
  }

  free(line);

  fclose(fp);
  return;
}

void storeToDirtyFile(char* originalPath, char* dirtyPath,
                      struct Person* personArray, const int personArrayLen) {
  // Obtain the dirty name

  // We concatenate the string in order to write everything at once
  // We do this as we have to convert the age to a string anyways, see
  // https://stackoverflow.com/a/32819876/2786884
  const char* format = "%s_dirty.csv";
  int length = snprintf(NULL, 0, format, basename(originalPath));
  dirtyPath = (char*)malloc((length + 1) * sizeof(char));
  snprintf(dirtyPath, 0, format, basename(originalPath));

  int exitFailure = storeRegister(dirtyPath, personArray, personArrayLen);
  if (exitFailure != 0) {
    printf("Failed to write to %s", dirtyPath);
    free(personArray);
    free(dirtyPath);
    exit(EXIT_FAILURE);
  }
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

  // Read the original register
  struct Person* personArray = NULL;
  int personArrayLen;
  readRegister(argv[1], &personArray, &personArrayLen);

  // Read whom to remove
  if (argc >= 3) {
    // Remove from a file
    removeFromFile(argv[2], &personArray, personArrayLen);
  }

  // Read whom to add
  int extendedPersonArrayLen;
  struct Person* extendedPersonArray = NULL;
  if (argc >= 4) {
    // Add to the register
    addFromFile(argv[3], &personArray, &personArrayLen, &extendedPersonArray,
                &extendedPersonArrayLen);
  } else {
    extendedPersonArrayLen = personArrayLen;
    extendedPersonArray = personArray;
  }

  // Store the register to a "dirty" file
  char* dirtyPath = NULL;
  storeToDirtyFile(argv[1], dirtyPath, extendedPersonArray,
                   extendedPersonArrayLen);

  // Read the register from the "dirty" file
  free(extendedPersonArray);
  struct Person* newPersonArray = NULL;
  int newPersonArrayLen;
  readRegister(dirtyPath, &newPersonArray, &newPersonArrayLen);

  // Print the register
  for (int i = 0; i < newPersonArrayLen; ++i) {
    printPerson(&newPersonArray[i]);
  }

  // Free memory
  free(newPersonArray);
  free(dirtyPath);

  return EXIT_SUCCESS;
}
