#include "../include/receiver.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../utils/include/dynamic_memory.h"

int checkAllNodesReceived(struct ReceivedNode* receivedNodeArray,
                          struct EdgeArray* invalidEdgesArray, int n) {
  // NOTE: In a undirected graph there can be at most n*(n-1)/2 edges
  int maxEdges = (n * (n - 1) / 2);
  // We start by allocating the memory needed for the EdgeCounterArray
  struct EdgeCounter* edgeCounterArrayArray = NULL;
  int success = allocateEdgeCounterArray(&edgeCounterArrayArray, maxEdges,
                                         "edgeCounterArray");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  struct EdgeCounterArray edgeCounterArray = {.maxEdges = maxEdges,
                                              .firstAvailablePosition = 0,
                                              .array = edgeCounterArrayArray};

  for (int addressIdx = 0; addressIdx < n; ++addressIdx) {
    int addressOfFirstIndex = receivedNodeArray[addressIdx].address;
    for (int neighborIdx = 0; neighborIdx < receivedNodeArray->nNeighbors;
         ++neighborIdx) {
      int neighborAddress =
          receivedNodeArray[addressIdx].neighborAddresses[neighborIdx];
      int edgeWeight = receivedNodeArray[addressIdx].edgeWeights[neighborIdx];
      int searchLowAddress = addressOfFirstIndex
                                 ? addressOfFirstIndex < neighborAddress
                                 : neighborAddress;
      int searchHighAddress = addressOfFirstIndex
                                  ? addressOfFirstIndex > neighborAddress
                                  : neighborAddress;

      // Check if there are any matches in the EdgeCounterArray
      int foundMatch = 0;
      for (int counterIdx = 0;
           counterIdx < edgeCounterArray.firstAvailablePosition; ++counterIdx) {
        if (counterIdx == (maxEdges - 1)) {
          fprintf(stderr,
                  "When investigating addressIdx %d with neighborIdx %d, the "
                  "counterIdx became %d, which is out of bounds as maxEdges is "
                  "%d\n",
                  addressIdx, neighborIdx, counterIdx, maxEdges);
          freeEdgeCounterArray(&(edgeCounterArray.array));
          return EXIT_FAILURE;
        }

        // See if there are any matches
        struct EdgeCounter curEdgeCounter = edgeCounterArray.array[counterIdx];
        if (curEdgeCounter.edge.lowNodeAddress == searchLowAddress) {
          // Here we have a match, we need to check if the high address is also
          // matching
          if (curEdgeCounter.edge.highNodeAddress == searchHighAddress) {
            // Ok, we are now matching both on the low address and the high
            // address. This means that this edge has been observed before.
            ++(curEdgeCounter.encounters);
            foundMatch = 1;

            success = checkIfEdgeIsValid(searchLowAddress, searchHighAddress,
                                         addressOfFirstIndex, edgeWeight,
                                         &curEdgeCounter, invalidEdgesArray);
            if (success != EXIT_SUCCESS) {
              return EXIT_FAILURE;
            }
          }
        }
      }

      if (foundMatch == 0) {
        success = addEdgeToEdgeCounterArray(searchLowAddress, searchHighAddress,
                                            addressOfFirstIndex, edgeWeight,
                                            &edgeCounterArray);
        if (success != EXIT_SUCCESS) {
          return EXIT_FAILURE;
        }
      }
    }
  }

  // FIXME: Check that all the nodes has been reported more than once
  //        If not raise the error

  freeEdgeCounterArray(&(edgeCounterArray.array));
  return EXIT_FAILURE;
}

int checkIfEdgeIsValid(const int lowAddress, const int highAddress,
                       const int addressOfFirstIndex, const int edgeWeight,
                       const struct EdgeCounter* const edgeCounter,
                       struct EdgeArray* invalidEdgesArray) {
  // To validate that the match is valid we need to check that
  // - It has not been reported more than twice
  // - The addressOfFirstIndex is different (if not this means that
  //   the same edge is reported twice which might indicate data
  //   corruption)
  // - That the reported weight is the same (as the graph we are
  //   working with are undirected)
  if (edgeCounter->encounters > 2) {
    // If this is an invalid edge we don't want to report it twice,
    // hence we check that it's not reported before
    int success = addInvalidEdge(lowAddress, highAddress, invalidEdgesArray,
                                 "reported more than twice");
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // No need to do more processing of this edge
    return EXIT_SUCCESS;
  }
  if (edgeCounter->addressOfFirstIndex == addressOfFirstIndex) {
    int success = addInvalidEdge(lowAddress, highAddress, invalidEdgesArray,
                                 "reported in the exact same way before");
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // No need to do more processing of this edge
    return EXIT_SUCCESS;
  }
  if (edgeCounter->reportedWeight != edgeWeight) {
    int success = addInvalidEdge(lowAddress, highAddress, invalidEdgesArray,
                                 "reported with different weights before");
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // No need to do more processing of this edge
    return EXIT_SUCCESS;
  }
  // Everything is fine
  return EXIT_SUCCESS;
}

int addInvalidEdge(const int lowAddress, const int highAddress,
                   struct EdgeArray* invalidEdgesArray, const char* reason) {
  // We check first that we are not out of bounds
  if (invalidEdgesArray->firstAvailablePosition ==
      invalidEdgesArray->maxEdges) {
    fprintf(stderr,
            "Wanted to add that edge %d <-> %d was out of bounds as it has "
            "been %s, but the first available position in invalidEdgesArray "
            "equals the maxEdges which is %d and therefore out of bounds\n",
            lowAddress, highAddress, reason, invalidEdgesArray->maxEdges);
    return EXIT_FAILURE;
  }
  // Furthermore We need to check that this edge has not been reported as
  // invalid before
  if (isEdgePresent(lowAddress, highAddress, invalidEdgesArray) == 1) {
    return EXIT_SUCCESS;
  }
  // Finally we print the warning
  printf(
      "WARNING: The edge %d <-> %d has been %s. This may be due to data "
      "corruption. Will therefore mark the edge invalid\n",
      lowAddress, highAddress, reason);
  invalidEdgesArray->array[invalidEdgesArray->firstAvailablePosition]
      .lowNodeAddress = lowAddress;
  invalidEdgesArray->array[invalidEdgesArray->firstAvailablePosition]
      .highNodeAddress = highAddress;
  ++(invalidEdgesArray->firstAvailablePosition);
  return EXIT_SUCCESS;
}

int isEdgePresent(const int lowAddress, const int highAddress,
                  struct EdgeArray const* edgeArray) {
  for (int edgeIdx = 0; edgeIdx < edgeArray->firstAvailablePosition;
       ++edgeIdx) {
    if ((edgeArray->array[edgeIdx].lowNodeAddress == lowAddress) &&
        (edgeArray->array[edgeIdx].highNodeAddress == highAddress)) {
      return 1;
    }
  }
  return 0;
}

int addEdgeToEdgeCounterArray(int lowAddress, int highAddress,
                              int addressOfFirstIndex, int edgeWeight,
                              struct EdgeCounterArray* edgeCounterArray) {
  // Check that we are not out of bounds
  if (edgeCounterArray->firstAvailablePosition == edgeCounterArray->maxEdges) {
    fprintf(stderr,
            "When adding edge %d <-> %d to the edge counter array the "
            "firstAvailablePosition %d was out of bounds as maxEdges was "
            "%d.\n",
            lowAddress, highAddress, edgeCounterArray->firstAvailablePosition,
            edgeCounterArray->maxEdges);
    freeEdgeCounterArray(&(edgeCounterArray->array));
    return EXIT_FAILURE;
  }

  // There are no matches in the edge counter array, which means that this
  // is the first time we are observing this edge
  // Hence we add it to the edge counter array
  int idx = edgeCounterArray->firstAvailablePosition;
  edgeCounterArray->array[idx].addressOfFirstIndex = addressOfFirstIndex;
  edgeCounterArray->array[idx].edge.lowNodeAddress = lowAddress;
  edgeCounterArray->array[idx].edge.highNodeAddress = highAddress;
  edgeCounterArray->array[idx].encounters = 1;
  edgeCounterArray->array[idx].reportedWeight = edgeWeight;

  // Increment the first available position
  ++(edgeCounterArray->firstAvailablePosition);
  return EXIT_SUCCESS;
}

int createAdjacencyMatrix(struct ReceivedNode* receivedNodeArray,
                          struct EdgeArray* invalidEdgesArray,
                          int*** adjacencyMatrix, int n) {
  (void)receivedNodeArray;
  (void)invalidEdgesArray;
  (void)adjacencyMatrix;
  (void)n;
  // FIXME:
  return EXIT_FAILURE;
}
