
#include "../include/helpers.h"

#include <ftw.h>     // for nftw, FTW (ptr only), FTW_DEPTH, FTW_PHYS
#include <stdio.h>   // for fprintf, perror, NULL, size_t, remove, stderr
#include <stdlib.h>  // for free, atoi, malloc, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strlen
struct stat;

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

int removeFunction(const char *path, const struct stat *sb, int typeFlag,
                   struct FTW *ftwBuf) {
  int success = remove(path);
  if (success != 0) {
    perror("Failed to remove path: ");
  }
  return success;
}

int removeRecursively(const char *path) {
  // ntfw will walk the file tree
  int success = nftw(path,
                     removeFunction,  // Function to apply to the walk
                     64,  // Maximum number of file descriptors to be used
                     // Flags
                     FTW_DEPTH  // Shall report all files in a directory before
                                // reporting the directory itself
                         | FTW_PHYS);  // Shall perform a physical walk and
                                       // shall not follow symbolic links

  if (success != 0) {
    fprintf(stderr, "Removing %s failed\n", path);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
