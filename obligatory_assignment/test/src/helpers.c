
#include "../include/helpers.h"

#include <stddef.h>  // for NULL, size_t
#include <stdio.h>   // for fprintf, perror, snprintf, stderr
#include <stdlib.h>  // for free, atoi, malloc, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strlen

int strToIntArray(const char *const arrayStr, int *const intArray) {
  size_t strLen = strlen(arrayStr);
  size_t intCounter = 0;
  size_t start = 0;
  size_t end = 0;
  char *subStr = NULL;
  for (int i = 0; i < strLen; ++i) {
    if (arrayStr[i] == ',') {
      end = i;
      size_t subStrLen = end - start;
      subStr = (char *)malloc((subStrLen + 1) * sizeof(char));
      if (subStr == NULL) {
        perror("Could not allocate memory to subStr: ");
        return EXIT_FAILURE;
      }
      int charWritten = snprintf(subStr, subStrLen + 1, "%s", &arrayStr[start]);
      if ((charWritten < 0) || (charWritten < subStrLen)) {
        fprintf(stderr, "Could not copy to subStr");
        free(subStr);
        subStr = NULL;
        return EXIT_FAILURE;
      }
      intArray[intCounter] = atoi(subStr);
      free(subStr);
      subStr = NULL;
      start = end + 1;
      ++intCounter;
    }
    ++end;
  }

  // Add the last element (if any)
  if (end != start) {
    size_t subStrLen = end - start;
    subStr = (char *)malloc((subStrLen + 1) * sizeof(char));
    if (subStr == NULL) {
      perror("Could not allocate memory to subStr: ");
      return EXIT_FAILURE;
    }
    int charWritten = snprintf(subStr, subStrLen + 1, "%s", &arrayStr[start]);
    if ((charWritten < 0) || (charWritten < subStrLen)) {
      fprintf(stderr, "Could not copy to subStr");
      free(subStr);
      subStr = NULL;
      return EXIT_FAILURE;
    }
    intArray[intCounter] = atoi(subStr);
    free(subStr);
    subStr = NULL;
  }

  return EXIT_SUCCESS;
}
