#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...
#include <string.h>  // for strcmp

#include "../routing_server/include/receiver.h"  // for checkAllNodesReceived

void testIsEdgePresent(void) {
#define N (3)
  // Create an edge array and check that what is present is present and what is
  // absent is absent
  struct Edge array[N] = {
      {.lowNodeAddress = 1, .highNodeAddress = 2},
      {.lowNodeAddress = 115, .highNodeAddress = 298},
      {.lowNodeAddress = 17, .highNodeAddress = 20},
  };
  struct EdgeArray edgeArray = {
      .array = array, .firstAvailablePosition = N, .maxEdges = N};
  int isPresent = isEdgePresent(1, 2, &edgeArray);
  assert(isPresent == 1);
  isPresent = isEdgePresent(115, 298, &edgeArray);
  assert(isPresent == 1);
  isPresent = isEdgePresent(17, 20, &edgeArray);
  assert(isPresent == 1);
  isPresent = isEdgePresent(7, 10, &edgeArray);
  assert(isPresent == 0);
#undef N
}

void testAddEdgeToEdgeCounterArray(void) {
  // Add edges until edgeCounterArray, then add one more to check that out of
  // bounds is working
#define N (3)
  struct EdgeCounter edgeCounter[N];
  struct EdgeCounterArray edgeCounterArray = {
      .array = edgeCounter, .firstAvailablePosition = 0, .maxEdges = N};
  const int lowAddresses[N] = {1, 115, 20};
  const int highAddresses[N] = {2, 298, 20};
  const int weights[N] = {3, 2, 17};
  int success;
  for (int i = 0; i < N; ++i) {
    success = addEdgeToEdgeCounterArray(lowAddresses[i], highAddresses[i],
                                        lowAddresses[i], weights[i],
                                        &edgeCounterArray);
    assert(success == EXIT_SUCCESS);
    assert(edgeCounterArray.firstAvailablePosition == (i + 1));
    assert(edgeCounterArray.array[i].addressOfFirstIndex == lowAddresses[i]);
    assert(edgeCounterArray.array[i].edge.lowNodeAddress == lowAddresses[i]);
    assert(edgeCounterArray.array[i].edge.highNodeAddress == highAddresses[i]);
    assert(edgeCounterArray.array[i].encounters == 1);
    assert(edgeCounterArray.array[i].reportedWeight == weights[i]);
  }

  success = addEdgeToEdgeCounterArray(42, 77, 42, 99, &edgeCounterArray);
  assert(success == EXIT_FAILURE);

#undef N
}

void testAddInvalidEdge(void) {
#define N (3)
  // Create an empty edge array, fill it until full and fill once more to check
  // that out of bounds is working as expected
  struct Edge array[N];
  struct EdgeArray invalidEdgesArray = {
      .array = array, .firstAvailablePosition = 0, .maxEdges = N};

  const int lowAddresses[N] = {1, 115, 20};
  const int highAddresses[N] = {2, 298, 20};
  int success;
  for (int i = 0; i < N; ++i) {
    int isPresent =
        isEdgePresent(lowAddresses[i], highAddresses[i], &invalidEdgesArray);
    assert(isPresent == 0);
    success = addInvalidEdge(lowAddresses[i], highAddresses[i],
                             &invalidEdgesArray, "added to invalidArray");
    assert(success == EXIT_SUCCESS);
    assert(invalidEdgesArray.firstAvailablePosition == (i + 1));
  }

  success = addInvalidEdge(77, 88, &invalidEdgesArray, "added to invalidArray");
  assert(success == EXIT_FAILURE);
#undef N
}

void testCheckDualReport(void) {
#define N (3)
  // Setup the test data
  struct EdgeCounter edgeCounter[N];
  const int lowAddresses[N] = {1, 115, 20};
  const int highAddresses[N] = {2, 298, 20};
  const int weights[N] = {3, 2, 17};
  for (int i = 0; i < N; ++i) {
    edgeCounter[i].edge.lowNodeAddress = lowAddresses[i];
    edgeCounter[i].edge.highNodeAddress = highAddresses[i];
    edgeCounter[i].encounters = 2;
    edgeCounter[i].addressOfFirstIndex = lowAddresses[i];
    edgeCounter[i].reportedWeight = weights[i];
  }
  struct EdgeCounterArray edgeCounterArray = {
      .array = edgeCounter, .firstAvailablePosition = N, .maxEdges = N};
  struct Edge array[N];
  // NOTE: We deliberately set maxEdges = N-1 so that it's easier to trigger the
  //       out of bounds error
  struct EdgeArray invalidEdgesArray = {
      .array = array, .firstAvailablePosition = 0, .maxEdges = N - 1};
  // Check that the dual report passes without any exceptions
  int success = checkDualReport(&edgeCounterArray, &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  assert(invalidEdgesArray.firstAvailablePosition == 0);

  // Add exceptions
  // NOTE: We add an exception to the middle of the array
  edgeCounterArray.array[1].encounters = 1;
  success = checkDualReport(&edgeCounterArray, &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  assert(invalidEdgesArray.firstAvailablePosition == 1);
  assert(invalidEdgesArray.array[0].lowNodeAddress == lowAddresses[1]);
  assert(invalidEdgesArray.array[0].highNodeAddress == highAddresses[1]);

  // Add exceptions out of bounds
  // This will go out of bounds as it will first add edgeCounterArray.array[0],
  // next it will try to add edgeCounterArray.array[1], however this was added
  // in the call above, and since the invalidEdgesArray.maxEdges = N-1 we will
  // be out of bounds
  edgeCounterArray.array[0].encounters = 1;
  success = checkDualReport(&edgeCounterArray, &invalidEdgesArray);
  assert(success == EXIT_FAILURE);
  assert(invalidEdgesArray.firstAvailablePosition == 2);
  assert(invalidEdgesArray.array[0].lowNodeAddress == lowAddresses[1]);
  assert(invalidEdgesArray.array[0].highNodeAddress == highAddresses[1]);
  assert(invalidEdgesArray.array[1].lowNodeAddress == lowAddresses[0]);
  assert(invalidEdgesArray.array[1].highNodeAddress == highAddresses[0]);
#undef N
}

void testCheckIfEdgeIsValid(void) {
  // FIXME: Implement
  assert(1 == 0);
}

void testCheckAllNodesReceived(void) {
  // FIXME: This tests the vanilla case, add harder cases
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
  int edgeWeights1[2] = {1, 3};
  int neighborAddress3[3] = {1, 5, 10};
  int edgeWeights3[3] = {1, 1, 2};
  int neighborAddress5[3] = {3, 0, 17};
  int edgeWeights5[3] = {2, 3, 1};
  int neighborAddress10[1] = {3};
  int edgeWeights10[1] = {1};
  int neighborAddress17[1] = {3};
  int edgeWeights17[1] = {1};
  struct ReceivedNode receivedNodeArray[N] = {
      {.address = 1,
       .nNeighbors = 2,
       .neighborAddresses = neighborAddress1,
       .edgeWeights = edgeWeights1},
      {.address = 3,
       .nNeighbors = 3,
       .neighborAddresses = neighborAddress3,
       .edgeWeights = edgeWeights3},
      {.address = 5,
       .nNeighbors = 3,
       .neighborAddresses = neighborAddress5,
       .edgeWeights = edgeWeights5},
      {.address = 10,
       .nNeighbors = 1,
       .neighborAddresses = neighborAddress10,
       .edgeWeights = edgeWeights10},
      {.address = 17,
       .nNeighbors = 1,
       .neighborAddresses = neighborAddress17,
       .edgeWeights = edgeWeights17},
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

  if (strcmp(argv[1], "isEdgePresent") == 0) {
    testIsEdgePresent();
  } else if (strcmp(argv[1], "addEdgeToEdgeCounterArray") == 0) {
    testAddEdgeToEdgeCounterArray();
  } else if (strcmp(argv[1], "addInvalidEdge") == 0) {
    testAddInvalidEdge();
  } else if (strcmp(argv[1], "checkDualReport") == 0) {
    testCheckDualReport();
  } else if (strcmp(argv[1], "checkIfEdgeIsValid") == 0) {
    testCheckIfEdgeIsValid();
  } else if (strcmp(argv[1], "createAdjacencyMatrix") == 0) {
    testCreateAdjacencyMatrix();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }

  return EXIT_SUCCESS;
}
