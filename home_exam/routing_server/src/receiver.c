#include "../include/receiver.h"

#include <stdio.h>   // for fprintf, fflush, printf
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...

#include "../../utils/include/common.h"          // for getIndexFromAddress
#include "../../utils/include/dynamic_memory.h"  // for freeEdgeCounterArray

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
    for (int neighborIdx = 0;
         neighborIdx < receivedNodeArray[addressIdx].nNeighbors;
         ++neighborIdx) {
      int neighborAddress =
          receivedNodeArray[addressIdx].neighborAddresses[neighborIdx];
      int edgeWeight = receivedNodeArray[addressIdx].edgeWeights[neighborIdx];
      int searchLowAddress = addressOfFirstIndex < neighborAddress
                                 ? addressOfFirstIndex
                                 : neighborAddress;
      int searchHighAddress = addressOfFirstIndex > neighborAddress
                                  ? addressOfFirstIndex
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
        struct EdgeCounter* curEdgeCounter =
            &(edgeCounterArray.array[counterIdx]);
        if (curEdgeCounter->edge.lowNodeAddress == searchLowAddress) {
          // Here we have a match, we need to check if the high address is also
          // matching
          if (curEdgeCounter->edge.highNodeAddress == searchHighAddress) {
            // Ok, we are now matching both on the low address and the high
            // address. This means that this edge has been observed before.
            ++(curEdgeCounter->encounters);
            foundMatch = 1;

            success = checkIfEdgeIsValid(searchLowAddress, searchHighAddress,
                                         addressOfFirstIndex, edgeWeight,
                                         curEdgeCounter, invalidEdgesArray);
            if (success != EXIT_SUCCESS) {
              freeEdgeCounterArray(&(edgeCounterArray.array));
              return EXIT_FAILURE;
            }
            break;
          }
        }
      }

      if (foundMatch == 0) {
        success = addEdgeToEdgeCounterArray(searchLowAddress, searchHighAddress,
                                            addressOfFirstIndex, edgeWeight,
                                            &edgeCounterArray);
        if (success != EXIT_SUCCESS) {
          freeEdgeCounterArray(&(edgeCounterArray.array));
          return EXIT_FAILURE;
        }
      }
    }
  }

  success = checkDualReport(&edgeCounterArray, invalidEdgesArray);
  if (success != EXIT_SUCCESS) {
    freeEdgeCounterArray(&(edgeCounterArray.array));
    return EXIT_FAILURE;
  }

  freeEdgeCounterArray(&(edgeCounterArray.array));
  return EXIT_SUCCESS;
}

int checkIfEdgeIsValid(const int lowAddress, const int highAddress,
                       const int addressOfFirstIndex, const int edgeWeight,
                       const struct EdgeCounter* const edgeCounter,
                       struct EdgeArray* invalidEdgesArray) {
  // To validate that the match is valid we need to check that
  // - It has not been reported more than twice
  // - The addressOfFirstIndex is different
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
  // We need to check that this edge has not been reported as
  // invalid before
  if (isEdgePresent(lowAddress, highAddress, invalidEdgesArray) == 1) {
    return EXIT_SUCCESS;
  }
  // Furthermore we check first that we are not out of bounds
  if (invalidEdgesArray->firstAvailablePosition ==
      invalidEdgesArray->maxEdges) {
    fprintf(stderr,
            "Wanted to add that edge %d <-> %d was out of bounds as it has "
            "been %s, but the first available position in invalidEdgesArray "
            "equals the maxEdges which is %d and therefore out of bounds\n",
            lowAddress, highAddress, reason, invalidEdgesArray->maxEdges);
    return EXIT_FAILURE;
  }
  // Finally we print the warning
  printf(
      "WARNING: The edge %d <-> %d has been %s. This may be due to data "
      "corruption. Will therefore mark the edge invalid\n",
      lowAddress, highAddress, reason);
  // Force a flush in order to get the order correct when piping to file
  fflush(stdout);
  invalidEdgesArray->array[invalidEdgesArray->firstAvailablePosition]
      .lowNodeAddress = lowAddress;
  invalidEdgesArray->array[invalidEdgesArray->firstAvailablePosition]
      .highNodeAddress = highAddress;
  ++(invalidEdgesArray->firstAvailablePosition);
  return EXIT_SUCCESS;
}

int isEdgePresent(const int lowAddress, const int highAddress,
                  const struct EdgeArray* const edgeArray) {
  for (int edgeIdx = 0; edgeIdx < edgeArray->firstAvailablePosition;
       ++edgeIdx) {
    if ((edgeArray->array[edgeIdx].lowNodeAddress == lowAddress) &&
        (edgeArray->array[edgeIdx].highNodeAddress == highAddress)) {
      return 1;
    }
  }
  return 0;
}

int addEdgeToEdgeCounterArray(const int lowAddress, const int highAddress,
                              const int addressOfFirstIndex,
                              const int edgeWeight,
                              struct EdgeCounterArray* const edgeCounterArray) {
  // Check that we are not out of bounds
  if (edgeCounterArray->firstAvailablePosition == edgeCounterArray->maxEdges) {
    fprintf(stderr,
            "When adding edge %d <-> %d to the edge counter array the "
            "firstAvailablePosition %d was out of bounds as maxEdges was "
            "%d.\n",
            lowAddress, highAddress, edgeCounterArray->firstAvailablePosition,
            edgeCounterArray->maxEdges);
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

int checkDualReport(const struct EdgeCounterArray* const edgeCounterArray,
                    struct EdgeArray* invalidEdgesArray) {
  for (int edgeIdx = 0; edgeIdx < edgeCounterArray->firstAvailablePosition;
       ++edgeIdx) {
    if (edgeCounterArray->array[edgeIdx].encounters < 2) {
      int success =
          addInvalidEdge(edgeCounterArray->array[edgeIdx].edge.lowNodeAddress,
                         edgeCounterArray->array[edgeIdx].edge.highNodeAddress,
                         invalidEdgesArray, "reported less than twice");
      if (success != EXIT_SUCCESS) {
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}

int createAdjacencyMatrix(const struct ReceivedNode* const receivedNodeArray,
                          const struct IndexToAddress* const indexToAddress,
                          const struct EdgeArray* const invalidEdgesArray,
                          int*** adjacencyMatrix, const int n) {
  // Allocate memory to the adjacency matrix
  // Note that this allocation is zero initalized
  int success = allocateIntMatrix(adjacencyMatrix, n, "adjacencyMatrix");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Loop through the edge array and fill the adjacencyMatrix
  // Note that this is inefficient as we are looping through all edges twice
  // (given that they are valid)
  for (int nodeIdx = 0; nodeIdx < n; ++nodeIdx) {
    // Loop through the neighbors
    int nNeighbors = receivedNodeArray[nodeIdx].nNeighbors;
    int firstIndex;
    success = getIndexFromAddress(receivedNodeArray[nodeIdx].address,
                                  indexToAddress, &firstIndex);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    for (int neighborIdx = 0; neighborIdx < nNeighbors; ++neighborIdx) {
      int secondIndex;
      success = getIndexFromAddress(
          receivedNodeArray[nodeIdx].neighborAddresses[neighborIdx],
          indexToAddress, &secondIndex);
      if (success != EXIT_SUCCESS) {
        return EXIT_FAILURE;
      }
      // Set the weight
      (*adjacencyMatrix)[firstIndex][secondIndex] =
          receivedNodeArray[nodeIdx].edgeWeights[neighborIdx];
    }
  }

  // Remove all edges with invalid edges
  for (int edgeIdx = 0; edgeIdx < invalidEdgesArray->firstAvailablePosition;
       ++edgeIdx) {
    // Get the first and the second index
    int firstIndex;
    success =
        getIndexFromAddress(invalidEdgesArray->array[edgeIdx].lowNodeAddress,
                            indexToAddress, &firstIndex);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    int secondIndex;
    success =
        getIndexFromAddress(invalidEdgesArray->array[edgeIdx].highNodeAddress,
                            indexToAddress, &secondIndex);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    // Nullify the weight for one part of the edge
    (*adjacencyMatrix)[firstIndex][secondIndex] = 0;
    // Nullify the weight for the other part of the edge
    (*adjacencyMatrix)[secondIndex][firstIndex] = 0;
  }

  return EXIT_SUCCESS;
}
