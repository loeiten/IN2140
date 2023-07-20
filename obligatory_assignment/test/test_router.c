
#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stddef.h>  // for NULL, size_t
#include <stdio.h>   // for fprintf, perror, stderr, snprintf
#include <stdlib.h>  // for free, atoi, malloc, EXIT_SUCCESS, EXI...
#include <string.h>  // for strcmp, strlen

#include "../include/router.h"  // for findRouterId, printNeighbors, Router

void testPrintNeighbors(const char *const neighborStr) {
  // Initialize test
  // Initialize the neighbors
  int neighbors[MAX_NEIGHBORS];
  for (int i = 0; i < MAX_NEIGHBORS; ++i) {
    neighbors[i] = -1;
  }
  // Fill the neighbors (if any)
  if (neighborStr == NULL) {
    printNeighbors(neighbors);
    return;
  }
  size_t strLen = strlen(neighborStr);
  size_t neighbourCounter = 0;
  size_t start = 0;
  size_t end = 0;
  char *subStr = NULL;
  for (int i = 0; i < strLen; ++i) {
    if (neighborStr[i] == ',') {
      end = i;
      size_t subStrLen = end - start;
      subStr = (char *)malloc((subStrLen + 1) * sizeof(char));
      if (subStr == NULL) {
        perror("Could not allocate memory to subStr: ");
        return;
      }
      int charWritten =
          snprintf(subStr, subStrLen + 1, "%s", &neighborStr[start]);
      if ((charWritten < 0) || (charWritten < subStrLen)) {
        fprintf(stderr, "Could not copy to subStr");
        free(subStr);
        subStr = NULL;
        return;
      }
      neighbors[neighbourCounter] = atoi(subStr);
      free(subStr);
      subStr = NULL;
      start = end + 1;
      ++neighbourCounter;
    }
    ++end;
  }

  // Add the last neighbor if any
  if (end != start) {
    size_t subStrLen = end - start;
    subStr = (char *)malloc((subStrLen + 1) * sizeof(char));
    if (subStr == NULL) {
      perror("Could not allocate memory to subStr: ");
      return;
    }
    int charWritten =
        snprintf(subStr, subStrLen + 1, "%s", &neighborStr[start]);
    if ((charWritten < 0) || (charWritten < subStrLen)) {
      fprintf(stderr, "Could not copy to subStr");
      free(subStr);
      subStr = NULL;
      return;
    }
    neighbors[neighbourCounter] = atoi(subStr);
    free(subStr);
    subStr = NULL;
  }
  // Call printNeighbors
  printNeighbors(neighbors);
  return;
}

void testFindRouterId() {
  // Create testRouters
  unsigned int N = 3;
  struct Router router0 = {.routerId = 42};
  struct Router router1 = {.routerId = 7};
  struct Router router2 = {.routerId = 88};
  struct Router routerArray[3] = {router0, router1, router2};

  int hitIdx;

  // Test positive
  int success = findRouterId(routerArray, N, 42, &hitIdx);
  assert(success == EXIT_SUCCESS);
  assert(hitIdx == 0);

  success = findRouterId(routerArray, N, 88, &hitIdx);
  assert(success == EXIT_SUCCESS);
  assert(hitIdx == 2);

  success = findRouterId(routerArray, N, 7, &hitIdx);
  assert(success == EXIT_SUCCESS);
  assert(hitIdx == 1);

  success = findRouterId(routerArray, N, 100, &hitIdx);
  assert(success == EXIT_FAILURE);
  assert(hitIdx == -1);

  return;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "printNeighbors") == 0) {
    testPrintNeighbors(argv[2]);
  } else if (strcmp(argv[1], "findRouterId") == 0) {
    testFindRouterId();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }
  return EXIT_SUCCESS;
}
