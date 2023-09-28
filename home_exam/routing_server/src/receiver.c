#include "../include/receiver.h"

#include <stdlib.h>

int checkAllNodesReceived(struct ReceivedNode* receivedNodeArray,
                          struct EdgeArray* nonValidEdgesArray, int n) {
  // NOTE: In a undirected graph there can be at most n*(n-1)/2 edges
  (void)receivedNodeArray;
  (void)nonValidEdgesArray;
  (void)n;
  // FIXME:
  return EXIT_FAILURE;
}

int createAdjacencyMatrix(struct ReceivedNode* receivedNodeArray,
                          struct EdgeArray* nonValidEdgesArray,
                          int*** adjacencyMatrix, int n) {
  (void)receivedNodeArray;
  (void)nonValidEdgesArray;
  (void)adjacencyMatrix;
  (void)n;
  // FIXME:
  return EXIT_FAILURE;
}
