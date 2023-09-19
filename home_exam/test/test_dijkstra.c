#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <limits.h>  // for INT_MAX
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...
#include <string.h>  // for strcmp

#include "../routing_server/include/dijkstra.h"  // for dijkstra, freeRouteA...
#include "../routing_server/include/route.h"     // for Route

void testGetMinDistanceIdx(void) {
#define N (2)
  // Easy test
  const int distanceArray1[N] = {15, 7};
  const int visited1[N] = {0, 0};
  int minIdx = getMinDistanceIdx(distanceArray1, visited1, N);
  assert(minIdx == 1);
#undef N

// More elaborate test
#define N (5)
  const int distanceArray2[N] = {15, 7, 8, 1, 7};
  const int visited2[N] = {0, 0, 0, 1, 0};
  minIdx = getMinDistanceIdx(distanceArray2, visited2, N);
  assert(minIdx == 4);
#undef N
}

void testRegisterRoute(void) {
// This test only checks that the function works with a simple test
// For a more elaborate test, see testDijkstra
#define N (2)
  const int graph[N][N] = {{0, 1}, {1, 0}};
  const int visitedArray[N] = {1, 0};
  const int distanceArray[N] = {0, INT_MAX};
  int visitedAndNeighbourArray[N] = {0, 0};
  int route1[N] = {0, INT_MAX};
  int route2[N] = {INT_MAX, INT_MAX};
  struct Route routeArray[N] = {{.nHops = 0, .route = route1},
                                {.nHops = -1, .route = route2}};

  // We must convert graph to a pointer pointer
  // We cannot do this directly as we cannot directly decay to pointer pointer
  // https://en.cppreference.com/w/cpp/language/array#:~:text=Multidimensional%20arrays,-When%20the%20element&text=Note%20that%20when%20array%2Dto,decay%20is%20applied%20only%20once.
  const int *graphPtr[N];
  for (int i = 0; i < N; ++i) {
    // Recall that i gives the pointer to the i'th row
    graphPtr[i] = graph[i];
  }

  int *visitedAndNeighbourArrayPtr = visitedAndNeighbourArray;
  struct Route *routeArrayPtr = routeArray;

  int success = registerRoute(0, 1, N, graphPtr, visitedArray, distanceArray,
                              visitedAndNeighbourArrayPtr, routeArrayPtr);
  assert(success == EXIT_SUCCESS);
  assert(routeArray[0].nHops == 0);
  assert(routeArray[0].route[0] == 0);
  assert(routeArray[1].nHops == 1);
  assert(routeArray[1].route[0] == 0);
  assert(routeArray[1].route[1] == 1);
#undef N
}

void testDijkstra(void) {
// graphA from
// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
#define N (9)
  // clang-format off
  const int graphA[N][N] = {
      {0, 4, 0, 0, 0, 0, 0, 8, 0},
      {4, 0, 8, 0, 0, 0, 0, 11, 0},
      {0, 8, 0, 7, 0, 4, 0, 0, 2},
      {0, 0, 7, 0, 9, 14, 0, 0, 0},
      {0, 0, 0, 9, 0, 10, 0, 0, 0},
      {0, 0, 4, 14, 10, 0, 2, 0, 0},
      {0, 0, 0, 0, 0, 2, 0, 1, 6},
      {8, 11, 0, 0, 0, 0, 1, 0, 7},
      {0, 0, 2, 0, 0, 0, 6, 7, 0}};
  // clang-format on
  int distanceArray[N];
  int src = 0;

  // We must convert graphA to a pointer pointer
  // We cannot do this directly as we cannot directly decay to pointer pointer
  // https://en.cppreference.com/w/cpp/language/array#:~:text=Multidimensional%20arrays,-When%20the%20element&text=Note%20that%20when%20array%2Dto,decay%20is%20applied%20only%20once.
  const int *graphPtrA[N];
  for (int i = 0; i < N; ++i) {
    // Recall that i gives the pointer to the i'th row
    graphPtrA[i] = graphA[i];
  }

  struct Route *routeArray = NULL;
  int success = dijkstra(src, graphPtrA, distanceArray, &routeArray, N);
  assert(success == EXIT_SUCCESS);
  // Check that the distances are correct
  assert(distanceArray[0] == 0);
  assert(distanceArray[1] == 4);
  assert(distanceArray[2] == 12);
  assert(distanceArray[3] == 19);
  assert(distanceArray[4] == 21);
  assert(distanceArray[5] == 11);
  assert(distanceArray[6] == 9);
  assert(distanceArray[7] == 8);
  assert(distanceArray[8] == 14);
  // Check that the routes are correct
  assert(routeArray[0].nHops == 0);
  assert(routeArray[0].route[0] == 0);
  assert(routeArray[1].nHops == 1);
  assert(routeArray[1].route[0] == 0);
  assert(routeArray[1].route[1] == 1);
  assert(routeArray[2].nHops == 2);
  assert(routeArray[2].route[0] == 0);
  assert(routeArray[2].route[1] == 1);
  assert(routeArray[2].route[2] == 2);
  assert(routeArray[3].nHops == 3);
  assert(routeArray[3].route[0] == 0);
  assert(routeArray[3].route[1] == 1);
  assert(routeArray[3].route[2] == 2);
  assert(routeArray[3].route[3] == 3);
  assert(routeArray[4].nHops == 4);
  assert(routeArray[4].route[0] == 0);
  assert(routeArray[4].route[1] == 7);
  assert(routeArray[4].route[2] == 6);
  assert(routeArray[4].route[3] == 5);
  assert(routeArray[4].route[4] == 4);
  assert(routeArray[5].nHops == 3);
  assert(routeArray[5].route[0] == 0);
  assert(routeArray[5].route[1] == 7);
  assert(routeArray[5].route[2] == 6);
  assert(routeArray[5].route[3] == 5);
  assert(routeArray[6].nHops == 2);
  assert(routeArray[6].route[0] == 0);
  assert(routeArray[6].route[1] == 7);
  assert(routeArray[6].route[2] == 6);
  assert(routeArray[7].nHops == 1);
  assert(routeArray[7].route[0] == 0);
  assert(routeArray[7].route[1] == 7);
  assert(routeArray[8].nHops == 3);
  assert(routeArray[8].route[0] == 0);
  assert(routeArray[8].route[1] == 1);
  assert(routeArray[8].route[2] == 2);
  assert(routeArray[8].route[3] == 8);
  freeRouteArray(&routeArray, N);

  src = 8;
  success = dijkstra(src, graphPtrA, distanceArray, &routeArray, N);
  // Check that the distances are correct
  assert(success == EXIT_SUCCESS);
  assert(distanceArray[0] == 14);
  assert(distanceArray[1] == 10);
  assert(distanceArray[2] == 2);
  assert(distanceArray[3] == 9);
  assert(distanceArray[4] == 16);
  assert(distanceArray[5] == 6);
  assert(distanceArray[6] == 6);
  assert(distanceArray[7] == 7);
  assert(distanceArray[8] == 0);
  // Check that the routes are correct
  assert(routeArray[0].nHops == 3);
  assert(routeArray[0].route[0] == 8);
  assert(routeArray[0].route[1] == 2);
  assert(routeArray[0].route[2] == 1);
  assert(routeArray[0].route[3] == 0);
  assert(routeArray[1].nHops == 2);
  assert(routeArray[1].route[0] == 8);
  assert(routeArray[1].route[1] == 2);
  assert(routeArray[1].route[2] == 1);
  assert(routeArray[2].nHops == 1);
  assert(routeArray[2].route[0] == 8);
  assert(routeArray[2].route[1] == 2);
  assert(routeArray[3].nHops == 2);
  assert(routeArray[3].route[0] == 8);
  assert(routeArray[3].route[1] == 2);
  assert(routeArray[3].route[2] == 3);
  assert(routeArray[4].nHops == 3);
  assert(routeArray[4].route[0] == 8);
  assert(routeArray[4].route[1] == 2);
  assert(routeArray[4].route[2] == 5);
  assert(routeArray[4].route[3] == 4);
  assert(routeArray[5].nHops == 2);
  assert(routeArray[5].route[0] == 8);
  assert(routeArray[5].route[1] == 2);
  assert(routeArray[5].route[2] == 5);
  assert(routeArray[6].nHops == 1);
  assert(routeArray[6].route[0] == 8);
  assert(routeArray[6].route[1] == 6);
  // NOTE: There are two routes with equal shortest path which leads to node 7
  //       Either 8 -> 7
  //       Or     8 -> 6 -> 7
  //       The latter will be chosen as we have a strict < when setting new
  //       minDistNeighborIdx, and because the index 6 comes before 7
  assert(routeArray[7].nHops == 2);
  assert(routeArray[7].route[0] == 8);
  assert(routeArray[7].route[1] == 6);
  assert(routeArray[7].route[2] == 7);
  assert(routeArray[8].nHops == 0);
  assert(routeArray[8].route[0] == 8);
  freeRouteArray(&routeArray, N);
#undef N

#define N (6)
  // graphB from https://www.programiz.com/dsa/dijkstra-algorithm
  // clang-format off
  const int graphB[N][N] = {
      {0, 4, 0, 0, 0, 4},
      {4, 0, 3, 6, 1, 2},
      {0, 3, 0, 2, 0, 0},
      {0, 6, 2, 0, 3, 0},
      {0, 1, 0, 3, 0, 0},
      {4, 0, 0, 0, 2, 0}};
  // clang-format on

  const int *graphPtrB[N];
  for (int i = 0; i < N; ++i) {
    // Recall that i gives the pointer to the i'th row
    graphPtrB[i] = graphB[i];
  }

  src = 0;
  success = dijkstra(src, graphPtrB, distanceArray, &routeArray, N);
  // Check that the distances are correct
  assert(success == EXIT_SUCCESS);
  assert(distanceArray[0] == 0);
  assert(distanceArray[1] == 4);
  assert(distanceArray[2] == 7);
  assert(distanceArray[3] == 8);
  assert(distanceArray[4] == 5);
  assert(distanceArray[5] == 4);
  // Check that the routes are correct
  assert(routeArray[0].nHops == 0);
  assert(routeArray[0].route[0] == 0);
  assert(routeArray[1].nHops == 1);
  assert(routeArray[1].route[0] == 0);
  assert(routeArray[1].route[1] == 1);
  assert(routeArray[2].nHops == 2);
  assert(routeArray[2].route[0] == 0);
  assert(routeArray[2].route[1] == 1);
  assert(routeArray[2].route[2] == 2);
  assert(routeArray[3].nHops == 3);
  assert(routeArray[3].route[0] == 0);
  assert(routeArray[3].route[1] == 1);
  assert(routeArray[3].route[2] == 4);
  assert(routeArray[3].route[3] == 3);
  assert(routeArray[4].nHops == 2);
  assert(routeArray[4].route[0] == 0);
  assert(routeArray[4].route[1] == 1);
  assert(routeArray[4].route[2] == 4);
  assert(routeArray[5].nHops == 1);
  assert(routeArray[5].route[0] == 0);
  assert(routeArray[5].route[1] == 5);
  freeRouteArray(&routeArray, N);
#undef N
}

int main(int argc, char **argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n1", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "getMinDistanceIdx") == 0) {
    testGetMinDistanceIdx();
  } else if (strcmp(argv[1], "registerRoute") == 0) {
    testRegisterRoute();
  } else if (strcmp(argv[1], "dijkstra") == 0) {
    testDijkstra();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }

  return EXIT_SUCCESS;
}
