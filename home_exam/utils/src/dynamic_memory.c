#include "../include/dynamic_memory.h"

#include <stdio.h>   // for fprintf, perror
#include <stdlib.h>  // for free, EXIT_FAILURE

#include "../../routing_server/include/receiver.h"  // for EdgeArray, Receiv...
#include "../../routing_server/include/route.h"     // for RoutingTable, Route
#include "../include/common.h"                      // for IndexToAddress

int allocateIntArray(int **intArray, const int n, const char *name) {
  *intArray = (int *)malloc(n * sizeof(int));
  if ((*intArray) == NULL) {
    fprintf(stderr, "Could not allocate memory to %s", name);
    perror(": ");
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
    fprintf(stderr, "Could not allocate memory to *%s", name);
    perror(": ");
    return EXIT_FAILURE;
  }
  // Allocate memory for each row individually
  for (int i = 0; i < n; i++) {
    (*intMatrix)[i] = (int *)calloc(n, sizeof(int));
    if ((*intMatrix)[i] == NULL) {
      fprintf(stderr, "Memory allocation for %s failed for row %d", name, i);
      perror(": ");
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
    fprintf(stderr, "Memory allocation for %s failed", name);
    perror(": ");
    return EXIT_FAILURE;
  }

  // Allocate the RouteArray
  for (int i = 0; i < n; ++i) {
    (*routeArray)[i].route = (int *)malloc(n * sizeof(int));
    if ((*routeArray)[i].route == NULL) {
      fprintf(stderr,
              "Memory allocation for %s failed for routeArrayTmp[%d].route",
              name, i);
      perror(": ");
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

int allocateRoutingTable(struct RoutingTable **routingTable, int n,
                         const char *name) {
  // Zero allocate the routing table
  *routingTable = (struct RoutingTable *)calloc(n, sizeof(struct RoutingTable));
  if ((*routingTable) == NULL) {
    fprintf(stderr, "Memory allocation for %s failed", name);
    perror(": ");
    return EXIT_FAILURE;
  }

  // Allocate the RoutingTable
  for (int i = 0; i < n; ++i) {
    // There will be at max one table for each node
    (*routingTable)[i].table = (struct DestinationNextPair *)malloc(
        n * sizeof(struct DestinationNextPair));
    (*routingTable)[i].n = 0;
    if ((*routingTable)[i].table == NULL) {
      fprintf(stderr,
              "Memory allocation for %s failed for routeArrayTmp[%d].route",
              name, i);
      perror(": ");
      // NOTE: The freeing of routeArray will happen in the place where it
      // was allocated
      freeRoutingTable(routingTable, n);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void freeRoutingTable(struct RoutingTable **routingTable, int n) {
  if ((*routingTable) != NULL) {
    for (int i = 0; i < n; ++i) {
      if ((*routingTable)[i].table != NULL) {
        free((*routingTable)[i].table);
        (*routingTable)[i].table = NULL;
      }
    }
    free(*routingTable);
    (*routingTable) = NULL;
  }
}

int allocateEdgeArray(struct EdgeArray *edgeArray, int maxEdges,
                      const char *name) {
  edgeArray->maxEdges = -1;
  edgeArray->firstAvailablePosition = -1;
  edgeArray->array = (struct Edge *)malloc(maxEdges * sizeof(struct Edge));
  if (edgeArray->array == NULL) {
    fprintf(stderr, "Could not allocate memory to %s", name);
    perror(": ");
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
    fprintf(stderr, "Could not allocate memory to %s", name);
    perror(": ");
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

int allocateReceivedNodeArray(struct ReceivedNode **receivedNodeArray, int n,
                              const char *name) {
  (*receivedNodeArray) =
      (struct ReceivedNode *)calloc(n, sizeof(struct ReceivedNode));
  if ((*receivedNodeArray) == NULL) {
    fprintf(stderr, "Could not allocate memory to %s", name);
    perror(": ");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < n; ++i) {
    (*receivedNodeArray)[i].nNeighbors = -1;
  }
  return EXIT_SUCCESS;
}

int allocateReceivedNodeNeighborAndWeights(struct ReceivedNode *receivedNode,
                                           int nNeighbors, const char *name) {
  receivedNode->nNeighbors = nNeighbors;
  int success =
      allocateIntArray(&(receivedNode->neighborAddresses), nNeighbors, name);
  if (success != EXIT_SUCCESS) {
    receivedNode->nNeighbors = -1;
    fprintf(stderr, "Could not allocate memory to the neighbors of %s", name);
    perror(": ");
    return EXIT_FAILURE;
  }
  success = allocateIntArray(&(receivedNode->edgeWeights), nNeighbors, name);
  if (success != EXIT_SUCCESS) {
    receivedNode->nNeighbors = -1;
    fprintf(stderr, "Could not allocate memory to the edgeWeights of %s", name);
    perror(": ");
    freeIntArray(&(receivedNode->neighborAddresses));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void freeReceivedNodeArray(struct ReceivedNode **receivedNodeArray, int n) {
  if ((*receivedNodeArray) == NULL) {
    return;
  }
  for (int i = 0; i < n; ++i) {
    freeIntArray(&((*receivedNodeArray)[i].neighborAddresses));
    freeIntArray(&((*receivedNodeArray)[i].edgeWeights));
  }
  (*receivedNodeArray) = NULL;
}

int allocateIndexToAddress(struct IndexToAddress *indexToAddress, int n,
                           const char *name) {
  indexToAddress->n = -1;
  indexToAddress->map = (int *)malloc(n * sizeof(int));
  if (indexToAddress->map == NULL) {
    fprintf(stderr, "Could not allocate memory to %s", name);
    perror(": ");
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

int allocateRoutingServer(struct ReceivedNode **receivedNodeArray,
                          struct EdgeArray *invalidEdgesArray,
                          struct IndexToAddress *indexToAddress,
                          int ***adjacencyMatrix, int **distanceArray,
                          struct Route **routeArray,
                          struct RoutingTable **routingTable, int n,
                          int maxEdges) {
  // receivedNodeArray
  int success =
      allocateReceivedNodeArray(receivedNodeArray, n, "receivedNodeArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Allocation of memory to receivedNodeArray failed, exiting\n");
    return EXIT_FAILURE;
  }
  // invalidEdgesArray
  success = allocateEdgeArray(invalidEdgesArray, maxEdges, "invalidEdgesArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Allocation of memory to invalidEdgesArray failed, exiting\n");
    freeRoutingServer(receivedNodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray, routingTable,
                      n);
    return EXIT_FAILURE;
  }
  // indexToAddress
  success = allocateIndexToAddress(indexToAddress, n, "indexToAddress");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory to indexToAddress failed, exiting\n");
    freeRoutingServer(receivedNodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray, routingTable,
                      n);
    return EXIT_FAILURE;
  }
  // adjacencyMatrix
  success = allocateIntMatrix(adjacencyMatrix, n, "adjacencyMatrix");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Allocation of memory to adjacencyMatrix failed, exiting\n");
    freeRoutingServer(receivedNodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray, routingTable,
                      n);
    return EXIT_FAILURE;
  }
  // distanceArray
  success = allocateIntArray(distanceArray, n, "distanceArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory to distanceArray failed, exiting\n");
    freeRoutingServer(receivedNodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray, routingTable,
                      n);
    return EXIT_FAILURE;
  }
  // routeArray
  success = allocateRouteArray(routeArray, n, "routeArray");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory to routeArray failed, exiting\n");
    freeRoutingServer(receivedNodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray, routingTable,
                      n);
    return EXIT_FAILURE;
  }
  // routingTable
  success = allocateRoutingTable(routingTable, n, "routingTable");
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory to routingTable failed, exiting\n");
    freeRoutingServer(receivedNodeArray, invalidEdgesArray, indexToAddress,
                      adjacencyMatrix, distanceArray, routeArray, routingTable,
                      n);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void freeRoutingServer(struct ReceivedNode **receivedNodeArray,
                       struct EdgeArray *invalidEdgesArray,
                       struct IndexToAddress *indexToAddress,
                       int ***adjacencyMatrix, int **distanceArray,
                       struct Route **routeArray,
                       struct RoutingTable **routingTable, int n) {
  freeReceivedNodeArray(receivedNodeArray, n);
  freeEdgeArray(invalidEdgesArray);
  freeIndexToAddress(indexToAddress);
  freeIntMatrix(adjacencyMatrix, n);
  freeIntArray(distanceArray);
  freeRouteArray(routeArray, n);
  freeRoutingTable(routingTable, n);
}
