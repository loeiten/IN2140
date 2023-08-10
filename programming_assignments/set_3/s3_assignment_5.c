/*
 * Assignment:
 *
 * Write a program with an infinite loop which asks for a file name.
 * Print every second line of the file
 * Exit the program if the given filename is "QUIT"
 */

#include <stdio.h>   // for printf, snprintf
#include <stdlib.h>  // for free, malloc, EXIT_SUCCESS
#include <string.h>  // for strlen

#include "../../utils/include/file_operations.h"

#define MAX_LEN (100)

int printEverySecondLine(FILE* fp, const int lines) {
  char* line = NULL;
  size_t len = 0;

  // NOTE: getline is from POSIX.1-2008, not the C-standard, see
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getline.html
  // for specification, and
  // https://github.com/NetBSD/src/blob/trunk/tools/compat/getline.c
  // for possible implementation
  // Read the entries
  for (int i = 0; i < lines; ++i) {
    ssize_t nBytes;  // ssize_t is when the output can be negative
    nBytes = getline(&line, &len, fp);
    if (i % 2 != 0) {
      continue;
    }
    if (nBytes == -1) {
      free(line);
      printf("Failed to read line %d\n", i);
      return EXIT_FAILURE;
    }
    printf("%s", line);
  }

  free(line);
  return EXIT_SUCCESS;
}

int main(void) {
  while (1) {
    // Capture input from stdin
    char path[MAX_LEN];

    printf("Enter a path: ");
    fgets(path, MAX_LEN, stdin);
    // Strip the newline
    // https://stackoverflow.com/a/28462221/2786884
    path[strcspn(path, "\r\n")] = '\0';

    if (strcmp(path, "QUIT") == 0) {
      break;
    }

    // Get the number of lines
    int lines;
    int exitFailure = getNumberOfLines(path, &lines);
    if (exitFailure) {
      continue;
    }

    // Open file
    FILE* fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
      printf("Could not open %s", path);
      continue;
    }

    printEverySecondLine(fp, lines);

    // Close the file
    fclose(fp);
  }

  return EXIT_SUCCESS;
}
