#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stddef.h>  // for NULL
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>  // for strcmp

#include "../include/router.h"  // for findRouterId, printNeighbors, Router
#include "include/helpers.h"    // for strToIntArray

#define N 3

void testPrintNeighbors(const char *const neighborStr) {
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
  int success = strToIntArray(neighborStr, neighbors);
  assert(success == EXIT_SUCCESS);

  // Call printNeighbors
  printNeighbors(neighbors);
  return;
}

void testFindRouterId() {
  // Create testRouters
  struct Router routerArray[N] = {
      {.routerId = 42}, {.routerId = 7}, {.routerId = 88}};

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

  // Test negative
  success = findRouterId(routerArray, N, 100, &hitIdx);
  assert(success == EXIT_FAILURE);
  assert(hitIdx == -1);

  return;
}

void testFindFreeNeighbor(const char *const arrayStr,
                          const char *const expected) {
  // Declare the variables
  struct Router router;
  int hitIdx;
  int success;
  // Initialize the neighbors
  for (int i = 0; i < MAX_NEIGHBORS; ++i) {
    router.neighbors[i] = -1;
  }

  // Test negative
  if (arrayStr[0] == 'x') {
    success = findFreeNeighbor(&router, &hitIdx);
    assert(success == EXIT_SUCCESS);
    assert(hitIdx == atoi(expected));
    printf("Success\n");
    return;
  }

  success = strToIntArray(arrayStr, router.neighbors);
  assert(success == EXIT_SUCCESS);

  if (router.neighbors[MAX_NEIGHBORS - 1] != -1) {
    success = findFreeNeighbor(&router, &hitIdx);
    assert(success == EXIT_FAILURE);
    assert(hitIdx == atoi(expected));
    printf("Success\n");
    return;
  }

  // Test positive
  success = findFreeNeighbor(&router, &hitIdx);
  assert(success == EXIT_SUCCESS);
  assert(hitIdx == atoi(expected));
  printf("Success\n");
  return;
}

void testSetNeighbor() {
  // Create testRouters
  struct Router routerArray[N] = {
      {.routerId = 42}, {.routerId = 7}, {.routerId = 88}};

  // Initialize the routers
  for (int r = 0; r < N; ++r) {
    for (int i = 0; i < MAX_NEIGHBORS; ++i) {
      routerArray[r].neighbors[i] = -1;
    }
  }

  // Test positive
  int success =
      setNeighbor((unsigned char)42, (unsigned char)7, routerArray, N);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[0].neighbors[0] == 7);

  success = setNeighbor((unsigned char)42, (unsigned char)88, routerArray, N);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[0].neighbors[1] == 88);

  success = setNeighbor((unsigned char)42, (unsigned char)42, routerArray, N);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[0].neighbors[2] == 42);

  success = setNeighbor((unsigned char)7, (unsigned char)88, routerArray, N);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[1].neighbors[0] == 88);

  // This is allowed, even though the router doesn't exist
  success = setNeighbor((unsigned char)42, (unsigned char)99, routerArray, N);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[0].neighbors[3] == 99);

  // Test negative
  success = setNeighbor((unsigned char)99, (unsigned char)88, routerArray, N);
  assert(success == EXIT_FAILURE);

  // Fill the neighbors of the first router
  for (int i = 0; i < MAX_NEIGHBORS; ++i) {
    routerArray[0].neighbors[i] = i;
  }
  success = setNeighbor((unsigned char)42, (unsigned char)7, routerArray, N);
  assert(success == EXIT_FAILURE);

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
  } else if (strcmp(argv[1], "findFreeNeighbor") == 0) {
    testFindFreeNeighbor(argv[2], argv[3]);
  } else if (strcmp(argv[1], "setNeighbor") == 0) {
    testSetNeighbor();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }
  return EXIT_SUCCESS;
}
