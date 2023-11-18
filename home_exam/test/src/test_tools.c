#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...

#include "../../utils/include/common.h"          // for Node, RoutingTable
#include "../../utils/include/dynamic_memory.h"  // for allocateNodeNeighbor...

int createInvertedAGraphNodeArray(struct Node** nodeArray) {
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

  // Malloc the nodeArray
  success = allocateNodeArray(nodeArray, n, "nodeArray of invertedAGraph");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Fill the nodeArray
  // Node 1
  success =
      allocateNodeNeighborAndWeights(&((*nodeArray)[0]), 2, "nodeArray[0]");
  if (success != EXIT_SUCCESS) {
    freeNodeArray(nodeArray, n);
    return EXIT_FAILURE;
  }
  (*nodeArray)[0].address = 1;
  (*nodeArray)[0].neighborAddresses[0] = 3;
  (*nodeArray)[0].neighborAddresses[1] = 5;
  (*nodeArray)[0].edgeWeights[0] = 1;
  (*nodeArray)[0].edgeWeights[1] = 3;
  // Node 3
  success =
      allocateNodeNeighborAndWeights(&((*nodeArray)[1]), 3, "nodeArray[1]");
  if (success != EXIT_SUCCESS) {
    freeNodeArray(nodeArray, n);
    return EXIT_FAILURE;
  }
  (*nodeArray)[1].address = 3;
  (*nodeArray)[1].neighborAddresses[0] = 1;
  (*nodeArray)[1].neighborAddresses[1] = 5;
  (*nodeArray)[1].neighborAddresses[2] = 10;
  (*nodeArray)[1].edgeWeights[0] = 1;
  (*nodeArray)[1].edgeWeights[1] = 2;
  (*nodeArray)[1].edgeWeights[2] = 1;
  // Node 5
  success =
      allocateNodeNeighborAndWeights(&((*nodeArray)[2]), 3, "nodeArray[2]");
  if (success != EXIT_SUCCESS) {
    freeNodeArray(nodeArray, n);
    return EXIT_FAILURE;
  }
  (*nodeArray)[2].address = 5;
  (*nodeArray)[2].neighborAddresses[0] = 3;
  (*nodeArray)[2].neighborAddresses[1] = 1;
  (*nodeArray)[2].neighborAddresses[2] = 17;
  (*nodeArray)[2].edgeWeights[0] = 2;
  (*nodeArray)[2].edgeWeights[1] = 3;
  (*nodeArray)[2].edgeWeights[2] = 1;
  // Node 10
  success =
      allocateNodeNeighborAndWeights(&((*nodeArray)[3]), 1, "nodeArray[3]");
  if (success != EXIT_SUCCESS) {
    freeNodeArray(nodeArray, n);
    return EXIT_FAILURE;
  }
  (*nodeArray)[3].address = 10;
  (*nodeArray)[3].neighborAddresses[0] = 3;
  (*nodeArray)[3].edgeWeights[0] = 1;
  // Node 17
  success =
      allocateNodeNeighborAndWeights(&((*nodeArray)[4]), 1, "nodeArray[4]");
  if (success != EXIT_SUCCESS) {
    freeNodeArray(nodeArray, n);
    return EXIT_FAILURE;
  }
  (*nodeArray)[4].address = 17;
  (*nodeArray)[4].neighborAddresses[0] = 5;
  (*nodeArray)[4].edgeWeights[0] = 1;

  return EXIT_SUCCESS;
}

int createIGraphRoutingTableArray(struct RoutingTableArray* routingTableArray,
                                  const int* const addressMap,
                                  const char* name) {
  int success = allocateRoutingTableArray(routingTableArray, 3, name);
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Address 101
  routingTableArray->routingTables[addressMap[0]].nRows = 2;
  routingTableArray->routingTables[addressMap[0]]
      .routingTableRows[0]
      .destination = addressMap[1];
  routingTableArray->routingTables[addressMap[0]].routingTableRows[0].nextHop =
      addressMap[1];
  routingTableArray->routingTables[addressMap[0]]
      .routingTableRows[1]
      .destination = addressMap[2];
  routingTableArray->routingTables[addressMap[0]].routingTableRows[1].nextHop =
      addressMap[1];

  // Address 15
  routingTableArray->routingTables[addressMap[1]].nRows = 2;
  routingTableArray->routingTables[addressMap[1]]
      .routingTableRows[0]
      .destination = addressMap[0];
  routingTableArray->routingTables[addressMap[1]].routingTableRows[0].nextHop =
      addressMap[0];
  routingTableArray->routingTables[addressMap[1]]
      .routingTableRows[1]
      .destination = addressMap[2];
  routingTableArray->routingTables[addressMap[1]].routingTableRows[1].nextHop =
      addressMap[2];

  // Address 42
  routingTableArray->routingTables[addressMap[2]].nRows = 2;
  routingTableArray->routingTables[addressMap[2]]
      .routingTableRows[0]
      .destination = addressMap[0];
  routingTableArray->routingTables[addressMap[2]].routingTableRows[0].nextHop =
      addressMap[1];
  routingTableArray->routingTables[addressMap[2]]
      .routingTableRows[1]
      .destination = addressMap[1];
  routingTableArray->routingTables[addressMap[2]].routingTableRows[1].nextHop =
      addressMap[1];

  return EXIT_SUCCESS;
}
