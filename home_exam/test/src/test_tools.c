#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...

#include "../../utils/include/common.h"          // for CommunicatedNode
#include "../../utils/include/dynamic_memory.h"  // for allocateCommunicated...

int createInvertedAGraphCommunicatedNodeArray(
    struct CommunicatedNode** communicatedNodeArray) {
  // Graph
  // The weight are written on the edges
  // The ids are written on the vertices
  // The indices are written in square brackets
  //   10 [2]   17 [4]
  //   1 |   2    | 1
  //   3 [1] - 5 [3]
  //   1  \     /  3
  //       1 [0]
  int n = 5;
  int success;

  // Malloc the communicatedNodeArray
  success = allocateCommunicatedNodeArray(
      communicatedNodeArray, n, "communicatedNodeArray of invertedAGraph");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Fill the communicatedNodeArray
  // Node 1
  success = allocateCommunicatedNodeNeighborAndWeights(
      &((*communicatedNodeArray)[0]), 2, "communicatedNodeArray[0]");
  if (success != EXIT_SUCCESS) {
    freeCommunicatedNodeArray(communicatedNodeArray, n);
    return EXIT_FAILURE;
  }
  (*communicatedNodeArray)[0].address = 1;
  (*communicatedNodeArray)[0].neighborAddresses[0] = 3;
  (*communicatedNodeArray)[0].neighborAddresses[1] = 5;
  (*communicatedNodeArray)[0].edgeWeights[0] = 1;
  (*communicatedNodeArray)[0].edgeWeights[1] = 3;
  // Node 3
  success = allocateCommunicatedNodeNeighborAndWeights(
      &((*communicatedNodeArray)[1]), 3, "communicatedNodeArray[1]");
  if (success != EXIT_SUCCESS) {
    freeCommunicatedNodeArray(communicatedNodeArray, n);
    return EXIT_FAILURE;
  }
  (*communicatedNodeArray)[1].address = 3;
  (*communicatedNodeArray)[1].neighborAddresses[0] = 1;
  (*communicatedNodeArray)[1].neighborAddresses[1] = 5;
  (*communicatedNodeArray)[1].neighborAddresses[2] = 10;
  (*communicatedNodeArray)[1].edgeWeights[0] = 1;
  (*communicatedNodeArray)[1].edgeWeights[1] = 2;
  (*communicatedNodeArray)[1].edgeWeights[2] = 1;
  // Node 5
  success = allocateCommunicatedNodeNeighborAndWeights(
      &((*communicatedNodeArray)[2]), 3, "communicatedNodeArray[2]");
  if (success != EXIT_SUCCESS) {
    freeCommunicatedNodeArray(communicatedNodeArray, n);
    return EXIT_FAILURE;
  }
  (*communicatedNodeArray)[2].address = 5;
  (*communicatedNodeArray)[2].neighborAddresses[0] = 3;
  (*communicatedNodeArray)[2].neighborAddresses[1] = 1;
  (*communicatedNodeArray)[2].neighborAddresses[2] = 17;
  (*communicatedNodeArray)[2].edgeWeights[0] = 2;
  (*communicatedNodeArray)[2].edgeWeights[1] = 3;
  (*communicatedNodeArray)[2].edgeWeights[2] = 1;
  // Node 10
  success = allocateCommunicatedNodeNeighborAndWeights(
      &((*communicatedNodeArray)[3]), 1, "communicatedNodeArray[3]");
  if (success != EXIT_SUCCESS) {
    freeCommunicatedNodeArray(communicatedNodeArray, n);
    return EXIT_FAILURE;
  }
  (*communicatedNodeArray)[3].address = 10;
  (*communicatedNodeArray)[3].neighborAddresses[0] = 3;
  (*communicatedNodeArray)[3].edgeWeights[0] = 1;
  // Node 17
  success = allocateCommunicatedNodeNeighborAndWeights(
      &((*communicatedNodeArray)[4]), 1, "communicatedNodeArray[4]");
  if (success != EXIT_SUCCESS) {
    freeCommunicatedNodeArray(communicatedNodeArray, n);
    return EXIT_FAILURE;
  }
  (*communicatedNodeArray)[4].address = 17;
  (*communicatedNodeArray)[4].neighborAddresses[0] = 5;
  (*communicatedNodeArray)[4].edgeWeights[0] = 1;

  return EXIT_SUCCESS;
}
