#include "../include/dynamic_memory.h"

#include <errno.h>   // for errno
#include <stdio.h>   // for fprintf, NULL
#include <stdlib.h>  // for free, EXIT_FAILURE
#include <string.h>  // for strerror

#include "../../routing_server/include/route.h"       // for RoutingTableArray
#include "../../routing_server/include/validation.h"  // for EdgeCounter
#include "../include/common.h"                        // for Node

int allocateIntArray(int **intArray, const int n, const char *name) {
  *intArray = (int *)malloc(n * sizeof(int));
  if ((*intArray) == NULL) {
    fprintf(stderr, "Could not allocate memory to %s: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void freeIntArray(int **intArray) {
  if ((*intArray) != NULL) {
    free(*intArray);
    *intArray = NULL;
  }
}

int allocateIntMatrix(int ***intMatrix, const int n, const char *name) {
  // Allocate memory for the array of pointers
  (*intMatrix) = (int **)calloc(n, sizeof(int *));
  if ((*intMatrix) == NULL) {
    fprintf(stderr, "Could not allocate memory to *%s: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }
  // Allocate memory for each row individually
  for (int i = 0; i < n; i++) {
    (*intMatrix)[i] = (int *)calloc(n, sizeof(int));
    if ((*intMatrix)[i] == NULL) {
      fprintf(stderr, "Memory allocation for %s failed for row %d: %s\n", name,
              i, strerror(errno));
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void freeIntMatrix(int ***intMatrix, int n) {
  if ((*intMatrix) != NULL) {
    for (int i = 0; i < n; ++i) {
      if ((*intMatrix)[i] != NULL) {
        free((*intMatrix)[i]);
        (*intMatrix)[i] = NULL;
      }
    }
    free(*intMatrix);
    *intMatrix = NULL;
  }
}

int allocateRouteArray(struct Route **routeArray, const int n,
                       const char *name) {
  // NOTE: Difference between malloc and calloc is that calloc is zero
  //       initializing the array
  //       If we don't do this and we want to check if
  //       routeArray[i].route == NULL,
  //       then routeArray[i].route pointer has an indeterminate value
  *routeArray = (struct Route *)calloc(n, sizeof(struct Route));
  if ((*routeArray) == NULL) {
    fprintf(stderr, "Memory allocation for %s failed: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }

  // Allocate the RouteArray
  for (int i = 0; i < n; ++i) {
    (*routeArray)[i].route = (int *)malloc(n * sizeof(int));
    if ((*routeArray)[i].route == NULL) {
      fprintf(
          stderr,
          "Memory allocation for %s failed for routeArrayTmp[%d].route: %s\n",
          name, i, strerror(errno));
      // NOTE: The freeing of routeArray will happen in the place where it
      // was allocated
      freeRouteArray(routeArray, n);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void freeRouteArray(struct Route **routeArray, const int n) {
  if ((*routeArray) != NULL) {
    for (int i = 0; i < n; ++i) {
      if ((*routeArray)[i].route != NULL) {
        free((*routeArray)[i].route);
        (*routeArray)[i].route = NULL;
      }
    }
    free(*routeArray);
    (*routeArray) = NULL;
  }
}

int allocateRoutingTable(struct RoutingTable *routingTable, const int nRows,
                         const char *name) {
  // Zero allocate the routing table
  routingTable->nRows = nRows;
  routingTable->routingTableRows =
      (struct RoutingTableRows *)calloc(nRows, sizeof(struct RoutingTableRows));
  if (routingTable->routingTableRows == NULL) {
    routingTable->nRows = -1;
    fprintf(stderr, "Memory allocation for %s failed: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void freeRoutingTable(struct RoutingTable *routingTable) {
  routingTable->nRows = -1;
  if (routingTable->routingTableRows == NULL) {
    return;
  }

  free(routingTable->routingTableRows);
  routingTable->routingTableRows = NULL;
}

int allocateRoutingTableArray(struct RoutingTableArray *routingTableArray,
                              int n, const char *name) {
  routingTableArray->n = n;
  // Zero allocate the routing table array
  routingTableArray->routingTables =
      (struct RoutingTable *)calloc(n, sizeof(struct RoutingTable));
  if (routingTableArray->routingTables == NULL) {
    routingTableArray->n = -1;
    fprintf(stderr, "Memory allocation for %s failed: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }

  // Allocate the RoutingTableArray
  for (int i = 0; i < n; ++i) {
    // There will be at max one table for each node
    routingTableArray->routingTables[i].routingTableRows =
        (struct RoutingTableRows *)malloc(n * sizeof(struct RoutingTableRows));
    routingTableArray->routingTables[i].nRows = 0;
    if (routingTableArray->routingTables[i].routingTableRows == NULL) {
      fprintf(
          stderr,
          "Memory allocation for %s failed for routeArrayTmp[%d].route: %s\n",
          name, i, strerror(errno));
      // NOTE: The freeing of routeArray will happen in the place where it
      // was allocated
      freeRoutingTableArray(routingTableArray);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void freeRoutingTableArray(struct RoutingTableArray *routingTableArray) {
  if (routingTableArray->routingTables == NULL) {
    routingTableArray->n = -1;
    return;
  }

  for (int i = 0; i < routingTableArray->n; ++i) {
    freeRoutingTable(&(routingTableArray->routingTables[i]));
  }
  free(routingTableArray->routingTables);
  routingTableArray->routingTables = NULL;
  routingTableArray->n = -1;
}

int allocateEdgeArray(struct EdgeArray *edgeArray, int maxEdges,
                      const char *name) {
  edgeArray->maxEdges = -1;
  edgeArray->firstAvailablePosition = -1;
  edgeArray->array = (struct Edge *)malloc(maxEdges * sizeof(struct Edge));
  if (edgeArray->array == NULL) {
    fprintf(stderr, "Could not allocate memory to %s: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }
  for (int i = 0; i < maxEdges; ++i) {
    edgeArray->array[i].lowNodeAddress = -1;
    edgeArray->array[i].highNodeAddress = -1;
  }
  edgeArray->maxEdges = maxEdges;
  edgeArray->firstAvailablePosition = 0;
  return EXIT_SUCCESS;
}

void freeEdgeArray(struct EdgeArray *edgeArray) {
  if (edgeArray->array != NULL) {
    free(edgeArray->array);
    edgeArray->array = NULL;
  }
  edgeArray->maxEdges = -1;
  edgeArray->firstAvailablePosition = -1;
}

int allocateEdgeCounterArray(struct EdgeCounter **edgeCounterArray,
                             int maxEdges, const char *name) {
  *edgeCounterArray =
      (struct EdgeCounter *)malloc(maxEdges * sizeof(struct EdgeCounter));
  if ((*edgeCounterArray) == NULL) {
    fprintf(stderr, "Could not allocate memory to %s: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void freeEdgeCounterArray(struct EdgeCounter **edgeCounterArray) {
  if ((*edgeCounterArray) != NULL) {
    free((*edgeCounterArray));
    (*edgeCounterArray) = NULL;
  }
}

int allocateNodeArray(struct Node **nodeArray, int n, const char *name) {
  (*nodeArray) = (struct Node *)calloc(n, sizeof(struct Node));
  if ((*nodeArray) == NULL) {
    fprintf(stderr, "Could not allocate memory to %s: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }
  for (int i = 0; i < n; ++i) {
    (*nodeArray)[i].nNeighbors = -1;
    (*nodeArray)[i].tcpSocket = -1;
  }
  return EXIT_SUCCESS;
}

int allocateNodeNeighborAndWeights(struct Node *node, int nNeighbors,
                                   const char *name) {
  node->nNeighbors = nNeighbors;
  int success = allocateIntArray(&(node->neighborAddresses), nNeighbors, name);
  if (success != EXIT_SUCCESS) {
    node->nNeighbors = -1;
    fprintf(stderr, "Could not allocate memory to the neighbors of %s: %s\n",
            name, strerror(errno));
    return EXIT_FAILURE;
  }
  success = allocateIntArray(&(node->edgeWeights), nNeighbors, name);
  if (success != EXIT_SUCCESS) {
    node->nNeighbors = -1;
    fprintf(stderr, "Could not allocate memory to the edgeWeights of %s: %s\n",
            name, strerror(errno));
    freeIntArray(&(node->neighborAddresses));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void freeNodeArray(struct Node **nodeArray, int n) {
  if ((*nodeArray) == NULL) {
    return;
  }
  for (int i = 0; i < n; ++i) {
    freeNeighborAddressesAndEdgeWeights(&((*nodeArray)[i]));
  }
  (*nodeArray) = NULL;
}

int allocateIndexToAddress(struct IndexToAddress *indexToAddress, int n,
                           const char *name) {
  indexToAddress->n = -1;
  indexToAddress->map = (int *)malloc(n * sizeof(int));
  if (indexToAddress->map == NULL) {
    fprintf(stderr, "Could not allocate memory to %s: %s\n", name,
            strerror(errno));
    return EXIT_FAILURE;
  }
  indexToAddress->n = n;
  return EXIT_SUCCESS;
}

void freeIndexToAddress(struct IndexToAddress *indexToAddress) {
  if (indexToAddress->map != NULL) {
    free(indexToAddress->map);
    indexToAddress->map = NULL;
  }
  indexToAddress->n = -1;
}

int allocateNeighborAddressesAndEdgeWeights(struct Node *node,
                                            const int nNeighbors,
                                            const char *name) {
  int success = allocateIntArray(&(node->neighborAddresses), nNeighbors, name);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "The failure happened when allocating to the neighborAddresses.");
    return EXIT_FAILURE;
  }
  success = allocateIntArray(&(node->edgeWeights), nNeighbors, name);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "The failure happened when allocating to the edgeWeights.");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void freeNeighborAddressesAndEdgeWeights(struct Node *node) {
  if (node->neighborAddresses != NULL) {
    free(node->neighborAddresses);
    node->neighborAddresses = NULL;
  }
  if (node->edgeWeights != NULL) {
    free(node->edgeWeights);
    node->edgeWeights = NULL;
  }
}

int allocateRoutingServer(struct Node **nodeArray,
                          struct EdgeArray *invalidEdgesArray,
                          struct IndexToAddress *indexToAddress,
                          int ***adjacencyMatrix, int **distanceArray,
                          struct Route **routeArray,
                          struct RoutingTableArray *routingTableArray, int n,
                          int maxEdges) {
  // nodeArray
  int success = allocateNodeArray(nodeArray, n, "nodeArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory to nodeArray failed, exiting\n");
    return EXIT_FAILURE;
  }
  // invalidEdgesArray
  success = allocateEdgeArray(invalidEdgesArray, maxEdges, "invalidEdgesArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Allocation of memory to invalidEdgesArray failed, exiting\n");
    freeRoutingServer(nodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray,
                      routingTableArray, n);
    return EXIT_FAILURE;
  }
  // indexToAddress
  success = allocateIndexToAddress(indexToAddress, n, "indexToAddress");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory to indexToAddress failed, exiting\n");
    freeRoutingServer(nodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray,
                      routingTableArray, n);
    return EXIT_FAILURE;
  }
  // adjacencyMatrix
  success = allocateIntMatrix(adjacencyMatrix, n, "adjacencyMatrix");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Allocation of memory to adjacencyMatrix failed, exiting\n");
    freeRoutingServer(nodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray,
                      routingTableArray, n);
    return EXIT_FAILURE;
  }
  // distanceArray
  success = allocateIntArray(distanceArray, n, "distanceArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory to distanceArray failed, exiting\n");
    freeRoutingServer(nodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray,
                      routingTableArray, n);
    return EXIT_FAILURE;
  }
  // routeArray
  success = allocateRouteArray(routeArray, n, "routeArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory to routeArray failed, exiting\n");
    freeRoutingServer(nodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray,
                      routingTableArray, n);
    return EXIT_FAILURE;
  }
  // routingTableArray
  success =
      allocateRoutingTableArray(routingTableArray, n, "routingTableArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Allocation of memory to routingTableArray failed, exiting\n");
    freeRoutingServer(nodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray,
                      routingTableArray, n);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void freeRoutingServer(struct Node **nodeArray,
                       struct EdgeArray *invalidEdgesArray,
                       struct IndexToAddress *indexToAddress,
                       int ***adjacencyMatrix, int **distanceArray,
                       struct Route **routeArray,
                       struct RoutingTableArray *routingTableArray, int n) {
  freeNodeArray(nodeArray, n);
  freeEdgeArray(invalidEdgesArray);
  freeIndexToAddress(indexToAddress);
  freeIntMatrix(adjacencyMatrix, n);
  freeIntArray(distanceArray);
  freeRouteArray(routeArray, n);
  freeRoutingTableArray(routingTableArray);
}
