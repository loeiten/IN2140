#include <limits.h>  // for INT_MAX
#include <stdio.h>   // for perror, NULL, fprintf, stderr
#include <stdlib.h>  // for malloc, free, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for memcpy

#include "../../routing_server/include/route.h"  // for Route

void freeIntArray(int **intArray) {
  if ((*intArray) != NULL) {
    free(*intArray);
    *intArray = NULL;
  }
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

void freeRoutingTable(struct RoutingTable **routingTable, int n) {
  if ((*routingTable) != NULL) {
    for (int i = 0; i < n; ++i) {
      if ((*routingTable[i]).table != NULL) {
        free((*routingTable[i]).table);
        (*routingTable[i]).table = NULL;
      }
    }
    free(*routingTable);
    (*routingTable) = NULL;
  }
}
