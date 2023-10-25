#include "../include/adjacency_matrix.h"

#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...

#include "../../utils/include/common.h"          // for getIndexFromAddress
#include "../../utils/include/dynamic_memory.h"  // for allocateIntMatrix

int createAdjacencyMatrix(
    const struct CommunicatedNode* const communicatedNodeArray,
    const struct IndexToAddress* const indexToAddress,
    const struct EdgeArray* const invalidEdgesArray, int*** adjacencyMatrix,
    const int n) {
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
    int nNeighbors = communicatedNodeArray[nodeIdx].nNeighbors;
    int firstIndex;
    success = getIndexFromAddress(communicatedNodeArray[nodeIdx].address,
                                  indexToAddress, &firstIndex);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    for (int neighborIdx = 0; neighborIdx < nNeighbors; ++neighborIdx) {
      int secondIndex;
      success = getIndexFromAddress(
          communicatedNodeArray[nodeIdx].neighborAddresses[neighborIdx],
          indexToAddress, &secondIndex);
      if (success != EXIT_SUCCESS) {
        return EXIT_FAILURE;
      }
      // Set the weight
      (*adjacencyMatrix)[firstIndex][secondIndex] =
          communicatedNodeArray[nodeIdx].edgeWeights[neighborIdx];
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
