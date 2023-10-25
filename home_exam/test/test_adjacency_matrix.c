#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL
#include <stdlib.h>  // for EXIT_SUCCESS
#include <string.h>  // for strcmp

#include "../routing_server/include/adjacency_matrix.h"  // for createAdjace...
#include "../utils/include/common.h"                     // for Edge, EdgeArray
#include "../utils/include/dynamic_memory.h"             // for allocateEdge...
#include "include/test_tools.h"                          // for createInvert...

void testCreateAdjacencyMatrix(void) {
#define N (5)
// NOTE: In a undirected graph there can be at most n*(n-1)/2 edges
#define MAX_EDGES (N * (N - 1) / 2)
  // Graph
  // The weight are written on the edges
  // The ids are written on the vertices
  // The indices are written in square brackets
  //   10 [2]   17 [4]
  //   1 |   2    | 1
  //   3 [1] - 5 [3]
  //   1  \     /  3
  //       1 [0]
  // Allocate and initialize
  struct CommunicatedNode* communicatedNodeArray = NULL;
  int success =
      createInvertedAGraphCommunicatedNodeArray(&communicatedNodeArray);
  assert(success == EXIT_SUCCESS);
  struct EdgeArray invalidEdgesArray;
  struct EdgeArray* invalidEdgesArrayPtr = &invalidEdgesArray;
  success = allocateEdgeArray(invalidEdgesArrayPtr, MAX_EDGES, "edgeArray");
  assert(success == EXIT_SUCCESS);

  int map[N] = {1, 3, 10, 5, 17};
  const struct IndexToAddress indexToAddress = {.n = N, .map = map};
  int** adjacencyMatrix = NULL;
  success = createAdjacencyMatrix(communicatedNodeArray, &indexToAddress,
                                  invalidEdgesArrayPtr, &adjacencyMatrix, N);
  assert(success == EXIT_SUCCESS);

  // Row 0 (connections from index 0)
  assert(adjacencyMatrix[0][0] == 0);
  assert(adjacencyMatrix[0][1] == 1);
  assert(adjacencyMatrix[0][2] == 0);
  assert(adjacencyMatrix[0][3] == 3);
  assert(adjacencyMatrix[0][4] == 0);
  // Row 1 (connections from index 1)
  assert(adjacencyMatrix[1][0] == 1);
  assert(adjacencyMatrix[1][1] == 0);
  assert(adjacencyMatrix[1][2] == 1);
  assert(adjacencyMatrix[1][3] == 2);
  assert(adjacencyMatrix[1][4] == 0);
  // Row 2 (connections from index 2)
  assert(adjacencyMatrix[2][0] == 0);
  assert(adjacencyMatrix[2][1] == 1);
  assert(adjacencyMatrix[2][2] == 0);
  assert(adjacencyMatrix[2][3] == 0);
  assert(adjacencyMatrix[2][4] == 0);
  // Row 3 (connections from index 3)
  assert(adjacencyMatrix[3][0] == 3);
  assert(adjacencyMatrix[3][1] == 2);
  assert(adjacencyMatrix[3][2] == 0);
  assert(adjacencyMatrix[3][3] == 0);
  assert(adjacencyMatrix[3][4] == 1);
  // Row 4 (connections from index 4)
  assert(adjacencyMatrix[4][0] == 0);
  assert(adjacencyMatrix[4][1] == 0);
  assert(adjacencyMatrix[4][2] == 0);
  assert(adjacencyMatrix[4][3] == 1);
  assert(adjacencyMatrix[4][4] == 0);

  // Making edges invalid so that we end up with the following graph
  //   10 [2]   17 [4]
  //         2   | 1
  //   3 [1] - 5 [3]
  //   1  |
  //   1 [0]
  invalidEdgesArray.firstAvailablePosition = 2;
  invalidEdgesArray.array[0].lowNodeAddress = 1;
  invalidEdgesArray.array[0].highNodeAddress = 5;
  invalidEdgesArray.array[1].lowNodeAddress = 3;
  invalidEdgesArray.array[1].highNodeAddress = 10;
  success = createAdjacencyMatrix(communicatedNodeArray, &indexToAddress,
                                  invalidEdgesArrayPtr, &adjacencyMatrix, N);
  assert(success == EXIT_SUCCESS);

  // Row 0 (connections from index 0)
  assert(adjacencyMatrix[0][0] == 0);
  assert(adjacencyMatrix[0][1] == 1);
  assert(adjacencyMatrix[0][2] == 0);
  assert(adjacencyMatrix[0][3] == 0);
  assert(adjacencyMatrix[0][4] == 0);
  // Row 1 (connections from index 1)
  assert(adjacencyMatrix[1][0] == 1);
  assert(adjacencyMatrix[1][1] == 0);
  assert(adjacencyMatrix[1][2] == 0);
  assert(adjacencyMatrix[1][3] == 2);
  assert(adjacencyMatrix[1][4] == 0);
  // Row 2 (connections from index 2)
  assert(adjacencyMatrix[2][0] == 0);
  assert(adjacencyMatrix[2][1] == 0);
  assert(adjacencyMatrix[2][2] == 0);
  assert(adjacencyMatrix[2][3] == 0);
  assert(adjacencyMatrix[2][4] == 0);
  // Row 3 (connections from index 3)
  assert(adjacencyMatrix[3][0] == 0);
  assert(adjacencyMatrix[3][1] == 2);
  assert(adjacencyMatrix[3][2] == 0);
  assert(adjacencyMatrix[3][3] == 0);
  assert(adjacencyMatrix[3][4] == 1);
  // Row 4 (connections from index 4)
  assert(adjacencyMatrix[4][0] == 0);
  assert(adjacencyMatrix[4][1] == 0);
  assert(adjacencyMatrix[4][2] == 0);
  assert(adjacencyMatrix[4][3] == 1);
  assert(adjacencyMatrix[4][4] == 0);

  freeEdgeArray(invalidEdgesArrayPtr);
  freeCommunicatedNodeArray(&communicatedNodeArray, N);
  freeIntMatrix(&adjacencyMatrix, N);
#undef MAX_EDGES
#undef N
}

int main(int argc, char** argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n1", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "createAdjacencyMatrix") == 0) {
    testCreateAdjacencyMatrix();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
