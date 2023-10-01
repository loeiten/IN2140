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
      int neighborWeight =
          receivedNodeArray[addressIdx].neighborWeights[neighborIdx];
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

            // To validate that the match is valid we need to check that
            // - It has not been reported more than twice
            // - The addressOfFirstIndex is different (if not this means that
            //   the same edge is reported twice which might indicate data
            //   corruption)
            // - That the reported weight is the same (as the graph we are
            //   working with are undirected)
            // FIXME: We also need to check that this edge has not been reported
            // as invalid
            if (curEdgeCounter.encounters > 2) {
              // FIXME: Check invalid
              // If this is an invalid edge we don't want to report it twice,
              // hence we check that it's not reported before
              // FIXME: Extract: addInvalidEdge function
              printf(
                  "WARNING: The edge %d <-> %d has been reported more than "
                  "twice. This may be due to data corruption. Will therefore "
                  "mark the edge invalid",
                  searchLowAddress, searchHighAddress);
              invalidEdgesArray
                  ->array[invalidEdgesArray->firstAvailablePosition]
                  .lowNodeAddress = searchLowAddress;
              invalidEdgesArray
                  ->array[invalidEdgesArray->firstAvailablePosition]
                  .highNodeAddress = searchHighAddress;
              // FIXME: Check that we are not out of bounds
              ++(invalidEdgesArray->firstAvailablePosition);
              // No need to do more processing of this edge
              continue;
            }
            if (curEdgeCounter.addressOfFirstIndex == addressOfFirstIndex) {
              printf(
                  "WARNING: The edge %d <-> %d has been reported in the exact "
                  "same way before. This may be due to data corruption. Will "
                  "therefore mark the edge invalid",
                  searchLowAddress, searchHighAddress);
              invalidEdgesArray
                  ->array[invalidEdgesArray->firstAvailablePosition]
                  .lowNodeAddress = searchLowAddress;
              invalidEdgesArray
                  ->array[invalidEdgesArray->firstAvailablePosition]
                  .highNodeAddress = searchHighAddress;
              ++(invalidEdgesArray->firstAvailablePosition);
              // No need to do more processing of this edge
              continue;
            }
            if (curEdgeCounter.reportedWeight != neighborWeight) {
              printf(
                  "WARNING: The edge %d <-> %d has been reported with "
                  "different weights (%d and %d). This may be due to data "
                  "corruption. Will therefore mark the edge invalid",
                  searchLowAddress, searchHighAddress,
                  curEdgeCounter.reportedWeight, neighborWeight);
              invalidEdgesArray
                  ->array[invalidEdgesArray->firstAvailablePosition]
                  .lowNodeAddress = searchLowAddress;
              invalidEdgesArray
                  ->array[invalidEdgesArray->firstAvailablePosition]
                  .highNodeAddress = searchHighAddress;
              ++(invalidEdgesArray->firstAvailablePosition);
              // No need to do more processing of this edge
              continue;
            }
          }
        }
      }

      if (foundMatch == 0) {
        // Check that we are not out of bounds
        if (edgeCounterArray.firstAvailablePosition ==
            edgeCounterArray.maxEdges) {
          fprintf(stderr,
                  "When adding edge %d <-> %d to the edge counter array the "
                  "firstAvailablePosition %d was out of bounds as maxEdges was "
                  "%d.\n",
                  searchLowAddress, searchHighAddress,
                  edgeCounterArray.firstAvailablePosition, maxEdges);
          freeEdgeCounterArray(&(edgeCounterArray.array));
          return EXIT_FAILURE;
        }

        // There are no matches in the edge counter array, which means that this
        // is the first time we are observing this edge
        // Hence we add it to the edge counter array
        int idx = edgeCounterArray.firstAvailablePosition;
        edgeCounterArray.array[idx].addressOfFirstIndex = addressOfFirstIndex;
        edgeCounterArray.array[idx].edge.lowNodeAddress = searchLowAddress;
        edgeCounterArray.array[idx].edge.highNodeAddress = searchHighAddress;
        edgeCounterArray.array[idx].encounters = 1;
        edgeCounterArray.array[idx].reportedWeight = neighborWeight;

        // Increment the first available position
        ++(edgeCounterArray.firstAvailablePosition);
      }
    }
  }

  // FIXME: Check that all the nodes has been reported more than once
  //        If not raise the error

  freeEdgeCounterArray(&(edgeCounterArray.array));
  return EXIT_FAILURE;
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
