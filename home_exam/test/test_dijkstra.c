#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_S...
#include <string.h>  // for strcmp

#include "../routing_server/include/dijkstra.h"  // for getMinDistanceIdx, di...

void testGetMinDistanceIdx(void) {
#define N (2)
  // Easy test
  const int distance1[N] = {15, 7};
  const int visited1[N] = {0, 0};
  int minIdx = getMinDistanceIdx(distance1, visited1, N);
  assert(minIdx == 1);
#undef N

// More elaborate test
#define N (5)
  const int distance2[N] = {15, 7, 8, 1, 7};
  const int visited2[N] = {0, 0, 0, 1, 0};
  minIdx = getMinDistanceIdx(distance2, visited2, N);
  assert(minIdx == 4);
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
  int distance[N];
  int src = 0;

  // We must convert graphA to a pointer pointer
  // We cannot do this directly as we cannot directly decay to pointer pointer
  // https://en.cppreference.com/w/cpp/language/array#:~:text=Multidimensional%20arrays,-When%20the%20element&text=Note%20that%20when%20array%2Dto,decay%20is%20applied%20only%20once.
  const int *graphPtrA[N];
  for (int i = 0; i < N; ++i) {
    // Recall that i gives the pointer to the i'th row
    graphPtrA[i] = graphA[i];
  }

  int success = dijkstra(src, graphPtrA, distance, N);
  assert(success == EXIT_SUCCESS);
  assert(distance[0] == 0);
  assert(distance[1] == 4);
  assert(distance[2] == 12);
  assert(distance[3] == 19);
  assert(distance[4] == 21);
  assert(distance[5] == 11);
  assert(distance[6] == 9);
  assert(distance[7] == 8);
  assert(distance[8] == 14);

  src = 8;
  success = dijkstra(src, graphPtrA, distance, N);
  assert(success == EXIT_SUCCESS);
  assert(distance[0] == 14);
  assert(distance[1] == 10);
  assert(distance[2] == 2);
  assert(distance[3] == 9);
  assert(distance[4] == 16);
  assert(distance[5] == 6);
  assert(distance[6] == 6);
  assert(distance[7] == 7);
  assert(distance[8] == 0);
#undef N

#define N (6)
  // Graph from https://www.programiz.com/dsa/dijkstra-algorithm
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
  success = dijkstra(src, graphPtrB, distance, N);
  assert(success == EXIT_SUCCESS);
  assert(distance[0] == 0);
  assert(distance[1] == 4);
  assert(distance[2] == 7);
  assert(distance[3] == 8);
  assert(distance[4] == 5);
  assert(distance[5] == 4);
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
  } else if (strcmp(argv[1], "dijkstra") == 0) {
    testDijkstra();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }

  return EXIT_SUCCESS;
}
