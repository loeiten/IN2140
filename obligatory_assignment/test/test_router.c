#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stddef.h>  // for NULL
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>  // for strcmp

#include "../include/dynamic_memory.h"  // for freeRouterArray
#include "../include/router.h"  // for findRouterId, printNeighbors, Router
#include "include/helpers.h"    // for strToIntArray

#define N 3

void testPrintRouter(const char* const routerIdStr, const char* const flagStr,
                     const char* const producerModel,
                     const char* const neighborsStr) {
  unsigned char routerId = atoi(routerIdStr);
  struct Router routerArray[1] = {{.routerId = routerId,
                                   .flag = (unsigned char)atoi(flagStr),
                                   .producerModel = producerModel}};
  // Initialize the neighbors
  for (int i = 0; i < MAX_NEIGHBORS; ++i) {
    routerArray[0].neighbors[i] = -1;
  }
  if (neighborsStr != NULL) {
    int success = strToIntArray(neighborsStr, routerArray[0].neighbors);
    assert(success == EXIT_SUCCESS);
  }

  printRouter(routerArray, 1, routerId);
}

void testPrintNeighbors(const char* const neighborStr) {
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
  int success = addLink(routerArray, N, (unsigned char)42, (unsigned char)7);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[0].neighbors[0] == 7);

  success = addLink(routerArray, N, (unsigned char)42, (unsigned char)88);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[0].neighbors[1] == 88);

  success = addLink(routerArray, N, (unsigned char)42, (unsigned char)42);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[0].neighbors[2] == 42);

  success = addLink(routerArray, N, (unsigned char)7, (unsigned char)88);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[1].neighbors[0] == 88);

  // This is allowed, even though the router doesn't exist
  success = addLink(routerArray, N, (unsigned char)42, (unsigned char)99);
  assert(success == EXIT_SUCCESS);
  assert(routerArray[0].neighbors[3] == 99);

  // Test negative
  success = addLink(routerArray, N, (unsigned char)99, (unsigned char)88);
  assert(success == EXIT_FAILURE);

  // Fill the neighbors of the first router
  for (int i = 0; i < MAX_NEIGHBORS; ++i) {
    routerArray[0].neighbors[i] = i;
  }
  success = addLink(routerArray, N, (unsigned char)42, (unsigned char)7);
  assert(success == EXIT_FAILURE);

  return;
}

void testFindRouterId() {
  // Create testRouters
  const struct Router routerArray[N] = {
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

void testFindFreeNeighbor(const char* const arrayStr,
                          const char* const expected) {
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

void testSetFlag(const char* routerIdStr, const char* flagStr,
                 const char* valueStr, const char* expectedSuccessStr) {
  // Setup test
  int routerId = atoi(routerIdStr);
  int flag = atoi(flagStr);
  int value = atoi(valueStr);
  int expectedSuccess = atoi(expectedSuccessStr);

  // Create the test routers
  struct Router routerArray[2] = {
      {.producerModel = "Foo", .routerId = 0, .flag = 0},
      {.producerModel = "Bar", .routerId = 1, .flag = 255}};

  // Initialize the routers
  for (int r = 0; r < 2; ++r) {
    for (int i = 0; i < MAX_NEIGHBORS; ++i) {
      routerArray[r].neighbors[i] = -1;
    }
  }

  // Test
  int success = setFlag(routerArray, 2, routerId, flag, value);
  assert(success == expectedSuccess);
  printRouter(routerArray, 2, routerId);
}

void testDeleteRouter(const char* const routerIdStr) {
  // Setup test
  int routerId = atoi(routerIdStr);
  unsigned int n = 4;

  // Malloc the test routers
  struct Router* routerArray =
      (struct Router*)malloc(n * sizeof(struct Router));
  if (routerArray == NULL) {
    perror("Could not allocate memory to routerArray: ");
    return;
  }
  // Fill the routers
  const char* producerModels[4] = {"Foo", "Bar", "Baz", "FooBar"};
  for (int i = 0; i < n; ++i) {
    routerArray[i].routerId = i;
    // Dynamically allocate the producerModels
    int length = snprintf(NULL, 0, "%s", producerModels[i]);
    char* producerModel = NULL;
    producerModel = malloc(length + 1);
    if (producerModel == NULL) {
      perror("Could not allocate memory to producerModel in the testRouters: ");
      freeRouterArray(&routerArray, n);
      return;
    }
    snprintf(producerModel, length + 1, "%s", producerModels[i]);
    routerArray[i].producerModel = producerModel;
  }

  // Initialize the neighbors
  for (int r = 0; r < n; ++r) {
    for (int i = 0; i < MAX_NEIGHBORS; ++i) {
      routerArray[r].neighbors[i] = -1;
    }
  }

  // Connect
  routerArray[0].neighbors[0] = 2;
  routerArray[0].neighbors[1] = 1;
  routerArray[0].neighbors[2] = 3;

  routerArray[1].neighbors[0] = 87;
  routerArray[1].neighbors[1] = 1;
  routerArray[1].neighbors[2] = 0;
  routerArray[1].neighbors[3] = 4;
  routerArray[1].neighbors[4] = 5;
  routerArray[1].neighbors[5] = 6;
  routerArray[1].neighbors[6] = 7;
  routerArray[1].neighbors[7] = 8;
  routerArray[1].neighbors[8] = 9;
  routerArray[1].neighbors[9] = 3;

  routerArray[2].neighbors[0] = 0;

  routerArray[3].neighbors[0] = 0;
  routerArray[3].neighbors[1] = 1;
  routerArray[3].neighbors[2] = 2;

  // Delete the router
  int success = deleteRouter(&routerArray, &n, routerId);
  assert(success == EXIT_SUCCESS);
  assert(n == 3);
  printNeighbors(routerArray[0].neighbors);
  printNeighbors(routerArray[1].neighbors);
  printNeighbors(routerArray[2].neighbors);

  // Clean-up
  freeRouterArray(&routerArray, n);
}

void testSetName() {
  // Malloc the test routers
  struct Router* routerArray[1];

  // Fill the routers
  const char* producerModels[4] = {"Foo", "FooBar", "Baz", "Quux"};
  routerArray[0]->routerId = 0;
  routerArray[0]->producerModel = NULL;

  // Dynamically allocate the producerModels
  for (int i = 0; i < 4; ++i) {
    int success = setModel(routerArray, 1, 0, producerModels[i]);
    assert(success == EXIT_SUCCESS);
    assert(strcmp(routerArray[0]->producerModel, producerModels[i]) == 0);
  }

  printf("Success\n");
}

int main(int argc, char** argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test [arg1 arg2 ...]\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "printRouter") == 0) {
    testPrintRouter(argv[2], argv[3], argv[4], argv[5]);
  } else if (strcmp(argv[1], "printNeighbors") == 0) {
    testPrintNeighbors(argv[2]);
  } else if (strcmp(argv[1], "addLink") == 0) {
    testSetNeighbor();
  } else if (strcmp(argv[1], "findRouterId") == 0) {
    testFindRouterId();
  } else if (strcmp(argv[1], "findFreeNeighbor") == 0) {
    testFindFreeNeighbor(argv[2], argv[3]);
  } else if (strcmp(argv[1], "setFlag") == 0) {
    testSetFlag(argv[2], argv[3], argv[4], argv[5]);
  } else if (strcmp(argv[1], "deleteRouter") == 0) {
    testDeleteRouter(argv[2]);
  } else if (strcmp(argv[1], "setName") == 0) {
    testSetName();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }
  return EXIT_SUCCESS;
}
