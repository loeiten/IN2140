#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, realloc
#include <string.h>  // for strcmp

#include "../routing_server/include/validation.h"  // for EdgeCounter, check...
#include "../utils/include/common.h"               // for Edge, EdgeArray
#include "../utils/include/dynamic_memory.h"       // for allocateEdgeArray
#include "include/test_tools.h"                    // for createInvertedAGra...

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
  const int lowAddresses[N] = {1, 115, 17};
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

  const int lowAddresses[N] = {1, 115, 17};
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
  const int lowAddresses[N] = {1, 115, 17};
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
  // Here all N edges will be in error, but we've only allocated N-1 for
  // invalidEdgesArray
  edgeCounterArray.array[0].encounters = 1;
  edgeCounterArray.array[2].encounters = 1;
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
#define N (3)
  // Initialize the data
  const int lowAddresses[N] = {1, 115, 17};
  const int highAddresses[N] = {2, 298, 20};
  const int addressOfFirstIndex[N] = {1, 115, 20};
  const int weights[N] = {3, 2, 17};
  struct EdgeCounter edgeCounter = {
      .edge = {.lowNodeAddress = lowAddresses[0],
               .highNodeAddress = highAddresses[0]},
      .encounters = 2,
      .addressOfFirstIndex = highAddresses[0],
      .reportedWeight = weights[0]};
  struct Edge array[N];
  struct EdgeArray invalidEdgesArray = {
      .array = array, .firstAvailablePosition = 0, .maxEdges = N};

  // Check that the the edge is valid
  int success = checkIfEdgeIsValid(lowAddresses[0], highAddresses[0],
                                   addressOfFirstIndex[0], weights[0],
                                   &edgeCounter, &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  assert(invalidEdgesArray.firstAvailablePosition == 0);

  // Trigger warnings
  // Reported more than twice
  edgeCounter.encounters = 3;
  success = checkIfEdgeIsValid(lowAddresses[0], highAddresses[0],
                               addressOfFirstIndex[0], weights[0], &edgeCounter,
                               &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  assert(invalidEdgesArray.firstAvailablePosition == 1);
  assert(invalidEdgesArray.array[0].lowNodeAddress == lowAddresses[0]);
  assert(invalidEdgesArray.array[0].highNodeAddress == highAddresses[0]);
  // Reported in the same way
  edgeCounter.edge.lowNodeAddress = lowAddresses[1];
  edgeCounter.edge.highNodeAddress = highAddresses[1];
  edgeCounter.encounters = 2;
  edgeCounter.addressOfFirstIndex = addressOfFirstIndex[1];
  edgeCounter.reportedWeight = weights[1];
  success = checkIfEdgeIsValid(lowAddresses[1], highAddresses[1],
                               addressOfFirstIndex[1], weights[1], &edgeCounter,
                               &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  assert(invalidEdgesArray.firstAvailablePosition == 2);
  assert(invalidEdgesArray.array[0].lowNodeAddress == lowAddresses[0]);
  assert(invalidEdgesArray.array[0].highNodeAddress == highAddresses[0]);
  assert(invalidEdgesArray.array[1].lowNodeAddress == lowAddresses[1]);
  assert(invalidEdgesArray.array[1].highNodeAddress == highAddresses[1]);
  // Reported with different weights
  edgeCounter.edge.lowNodeAddress = lowAddresses[2];
  edgeCounter.edge.highNodeAddress = highAddresses[2];
  edgeCounter.addressOfFirstIndex = addressOfFirstIndex[2];
  edgeCounter.reportedWeight = weights[2] + 100;
  success = checkIfEdgeIsValid(lowAddresses[2], highAddresses[2],
                               addressOfFirstIndex[2], weights[2], &edgeCounter,
                               &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  assert(invalidEdgesArray.firstAvailablePosition == 3);
  assert(invalidEdgesArray.array[0].lowNodeAddress == lowAddresses[0]);
  assert(invalidEdgesArray.array[0].highNodeAddress == highAddresses[0]);
  assert(invalidEdgesArray.array[1].lowNodeAddress == lowAddresses[1]);
  assert(invalidEdgesArray.array[1].highNodeAddress == highAddresses[1]);
  assert(invalidEdgesArray.array[2].lowNodeAddress == lowAddresses[2]);
  assert(invalidEdgesArray.array[2].highNodeAddress == highAddresses[2]);

  // Trigger errors
  // Reported more than twice
  const int errorLowAddress = 128;
  const int errorHighAddress = 256;
  const int errorAddressOfFirstIndex = errorHighAddress;
  const int errorWeight = 88;
  edgeCounter.edge.lowNodeAddress = errorLowAddress;
  edgeCounter.edge.highNodeAddress = errorHighAddress;
  edgeCounter.encounters = 3;
  edgeCounter.addressOfFirstIndex = errorAddressOfFirstIndex;
  edgeCounter.reportedWeight = errorWeight;
  success =
      checkIfEdgeIsValid(errorLowAddress, errorHighAddress, errorLowAddress,
                         errorWeight, &edgeCounter, &invalidEdgesArray);
  assert(success == EXIT_FAILURE);
  assert(invalidEdgesArray.firstAvailablePosition == 3);
  // Reported in the same way
  edgeCounter.encounters = 2;
  edgeCounter.addressOfFirstIndex = errorLowAddress;
  success =
      checkIfEdgeIsValid(errorLowAddress, errorHighAddress, errorLowAddress,
                         errorWeight, &edgeCounter, &invalidEdgesArray);
  assert(success == EXIT_FAILURE);
  // Reported with different weights
  edgeCounter.addressOfFirstIndex = errorHighAddress;
  edgeCounter.reportedWeight = errorWeight + 512;
  success =
      checkIfEdgeIsValid(errorLowAddress, errorHighAddress, errorLowAddress,
                         errorWeight, &edgeCounter, &invalidEdgesArray);
  assert(success == EXIT_FAILURE);

  // Check that the same node will not be reported twice
  // Reported more than twice
  edgeCounter.edge.lowNodeAddress = lowAddresses[0];
  edgeCounter.edge.highNodeAddress = highAddresses[0];
  edgeCounter.encounters = 3;
  edgeCounter.addressOfFirstIndex = addressOfFirstIndex[0];
  edgeCounter.reportedWeight = weights[0];
  success = checkIfEdgeIsValid(lowAddresses[0], highAddresses[0],
                               addressOfFirstIndex[0], weights[0], &edgeCounter,
                               &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  // Reported in the same way
  edgeCounter.edge.lowNodeAddress = lowAddresses[1];
  edgeCounter.edge.highNodeAddress = highAddresses[1];
  edgeCounter.encounters = 2;
  edgeCounter.addressOfFirstIndex = addressOfFirstIndex[1];
  edgeCounter.reportedWeight = weights[1];
  success = checkIfEdgeIsValid(lowAddresses[1], highAddresses[1],
                               addressOfFirstIndex[1], weights[1], &edgeCounter,
                               &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  // Reported with different weights
  edgeCounter.edge.lowNodeAddress = lowAddresses[2];
  edgeCounter.edge.highNodeAddress = highAddresses[2];
  edgeCounter.addressOfFirstIndex = addressOfFirstIndex[2];
  edgeCounter.reportedWeight = weights[2] + 100;
  success = checkIfEdgeIsValid(lowAddresses[2], highAddresses[2],
                               addressOfFirstIndex[2], weights[2], &edgeCounter,
                               &invalidEdgesArray);
  assert(success == EXIT_SUCCESS);
  assert(invalidEdgesArray.firstAvailablePosition == 3);
#undef N
}

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
  // Allocate and initialize
  struct CommunicatedNode* communicatedNodeArray = NULL;
  int success =
      createInvertedAGraphCommunicatedNodeArray(&communicatedNodeArray);
  assert(success == EXIT_SUCCESS);
  struct EdgeArray invalidEdgesArray;
  struct EdgeArray* invalidEdgesArrayPtr = &invalidEdgesArray;
  success = allocateEdgeArray(invalidEdgesArrayPtr, MAX_EDGES, "edgeArray");
  assert(success == EXIT_SUCCESS);

  success =
      checkAllNodesReceived(communicatedNodeArray, invalidEdgesArrayPtr, N);
  // Check that the test went successful
  assert(success == EXIT_SUCCESS);
  // Check that all the nodes are valid
  assert(invalidEdgesArray.firstAvailablePosition == 0);
  for (int i = 0; i < MAX_EDGES; ++i) {
    assert(invalidEdgesArray.array[i].lowNodeAddress == -1);
    assert(invalidEdgesArray.array[i].highNodeAddress == -1);
  }

  // Modify for report more than twice
  // Let the node 1 appear twice in the secondly processed node
#define N_NEIGHBORS (4)
  communicatedNodeArray[1].nNeighbors = N_NEIGHBORS;
  communicatedNodeArray[1].neighborAddresses = (int*)realloc(
      communicatedNodeArray[1].neighborAddresses, N_NEIGHBORS * sizeof(int));
  assert(communicatedNodeArray[1].neighborAddresses != NULL);
  communicatedNodeArray[1].edgeWeights = (int*)realloc(
      communicatedNodeArray[1].edgeWeights, N_NEIGHBORS * sizeof(int));
  assert(communicatedNodeArray[1].neighborAddresses != NULL);
  const int neighborAddress3Mod[N_NEIGHBORS] = {1, 5, 10, 1};
  const int edgeWeights3Mod[N_NEIGHBORS] = {1, 2, 1, 1};
  for (int i = 0; i < N_NEIGHBORS; ++i) {
    communicatedNodeArray[1].neighborAddresses[i] = neighborAddress3Mod[i];
    communicatedNodeArray[1].edgeWeights[i] = edgeWeights3Mod[i];
  }
#undef N_NEIGHBORS
#define N_NEIGHBORS (3)
  // Modify to trigger addressOfFirstIndex is the same
  // Let the node 1 appear twice in the firstly processed node
  communicatedNodeArray[0].nNeighbors = N_NEIGHBORS;
  communicatedNodeArray[0].neighborAddresses = (int*)realloc(
      communicatedNodeArray[0].neighborAddresses, N_NEIGHBORS * sizeof(int));
  assert(communicatedNodeArray[0].neighborAddresses != NULL);
  communicatedNodeArray[0].edgeWeights = (int*)realloc(
      communicatedNodeArray[0].edgeWeights, N_NEIGHBORS * sizeof(int));
  assert(communicatedNodeArray[0].edgeWeights != NULL);
  const int neighborAddress1Mod[N_NEIGHBORS] = {3, 5, 5};
  const int edgeWeights1Mod[N_NEIGHBORS] = {1, 3, 3};
  for (int i = 0; i < N_NEIGHBORS; ++i) {
    communicatedNodeArray[0].neighborAddresses[i] = neighborAddress1Mod[i];
    communicatedNodeArray[0].edgeWeights[i] = edgeWeights1Mod[i];
  }
#undef N_NEIGHBORS
  // Modify for different weights
  // Modify the last processed node with a different weight
  const int edgeWeights10Mod[1] = {7};
  communicatedNodeArray[3].edgeWeights[0] = edgeWeights10Mod[0];
  // Modify for report less than twice
  // Remove the connection from a node
  communicatedNodeArray[4].nNeighbors = 0;

  success =
      checkAllNodesReceived(communicatedNodeArray, invalidEdgesArrayPtr, N);
  assert(success == EXIT_SUCCESS);
  assert(invalidEdgesArray.firstAvailablePosition == 4);
  // 1 <-> 5 will be processed first
  // This is the edge which is reported in the exact same way twice
  assert(invalidEdgesArray.array[0].lowNodeAddress == 1);
  assert(invalidEdgesArray.array[0].highNodeAddress == 5);
  // 1 <-> 3 will be processed secondly
  // This is the edge which is reported more than twice
  assert(invalidEdgesArray.array[1].lowNodeAddress == 1);
  assert(invalidEdgesArray.array[1].highNodeAddress == 3);
  // 3 <-> 10 will be processed thirdly
  // This is the edge which has different weights
  assert(invalidEdgesArray.array[2].lowNodeAddress == 3);
  assert(invalidEdgesArray.array[2].highNodeAddress == 10);
  // 5 <-> 17 will be processed in the end
  // This is the edge which is reported only once
  assert(invalidEdgesArray.array[3].lowNodeAddress == 5);
  assert(invalidEdgesArray.array[3].highNodeAddress == 17);

  freeEdgeArray(invalidEdgesArrayPtr);
  freeCommunicatedNodeArray(&communicatedNodeArray, N);
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
  } else if (strcmp(argv[1], "checkAllNodesReceived") == 0) {
    testCheckAllNodesReceived();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
