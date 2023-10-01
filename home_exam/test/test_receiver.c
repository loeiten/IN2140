#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...
#include <string.h>  // for strcmp

#include "../routing_server/include/receiver.h"  // for checkAllNodesReceived

void testCheckAllNodesReceived(void) {
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
  int neighborAddress1[2] = {3, 5};
  int neighborWeights1[2] = {1, 3};
  int neighborAddress3[3] = {1, 5, 10};
  int neighborWeights3[3] = {1, 1, 2};
  int neighborAddress5[3] = {3, 0, 17};
  int neighborWeights5[3] = {2, 3, 1};
  int neighborAddress10[1] = {3};
  int neighborWeights10[1] = {1};
  int neighborAddress17[1] = {3};
  int neighborWeights17[1] = {1};
  struct ReceivedNode receivedNodeArray[N] = {
      {.address = 1,
       .nNeighbors = 2,
       .neighborAddresses = neighborAddress1,
       .neighborWeights = neighborWeights1},
      {.address = 3,
       .nNeighbors = 3,
       .neighborAddresses = neighborAddress3,
       .neighborWeights = neighborWeights3},
      {.address = 5,
       .nNeighbors = 3,
       .neighborAddresses = neighborAddress5,
       .neighborWeights = neighborWeights5},
      {.address = 10,
       .nNeighbors = 1,
       .neighborAddresses = neighborAddress10,
       .neighborWeights = neighborWeights10},
      {.address = 17,
       .nNeighbors = 1,
       .neighborAddresses = neighborAddress17,
       .neighborWeights = neighborWeights17},
  };
  struct Edge edgeArray[MAX_EDGES];
  // Initialize the edgeArray
  for (int i = 0; i < MAX_EDGES; ++i) {
    edgeArray[i].lowNodeAddress = -1;
    edgeArray[i].highNodeAddress = -1;
  }
  struct EdgeArray invalidEdgesArray = {
      .array = edgeArray, .firstAvailablePosition = 0, .maxEdges = MAX_EDGES};

  struct EdgeArray* invalidEdgesArrayPtr = &invalidEdgesArray;
  int success =
      checkAllNodesReceived(receivedNodeArray, invalidEdgesArrayPtr, N);
  // Check that the test went successful
  assert(success == EXIT_SUCCESS);
  // Check that all the nodes are valid
  for (int i = 0; i < MAX_EDGES; ++i) {
    assert(edgeArray[i].lowNodeAddress == -1);
    assert(edgeArray[i].highNodeAddress == -1);
  }
#undef MAX_EDGES
#undef N
}

void testCreateAdjacencyMatrix(void) {
#define N (1)
  struct ReceivedNode* receivedNodeArray = NULL;
  struct EdgeArray* invalidEdgesArray = NULL;
  int** adjacencyMatrix = NULL;
  int success = createAdjacencyMatrix(receivedNodeArray, invalidEdgesArray,
                                      &adjacencyMatrix, N);
  assert(success == EXIT_SUCCESS);
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

  if (strcmp(argv[1], "checkAllNodesReceived") == 0) {
    testCheckAllNodesReceived();
  } else if (strcmp(argv[1], "createAdjacencyMatrix") == 0) {
    testCreateAdjacencyMatrix();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }

  return EXIT_SUCCESS;
}
