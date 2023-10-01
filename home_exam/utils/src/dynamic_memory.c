#include "../include/dynamic_memory.h"

#include <stdio.h>   // for NULL
#include <stdlib.h>  // for free

#include "../../routing_server/include/receiver.h"  // for EdgeCounterArray
#include "../../routing_server/include/route.h"     // for Route, RoutingTable

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
    (*intMatrix)[i] = (int *)malloc(n * sizeof(int));
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
