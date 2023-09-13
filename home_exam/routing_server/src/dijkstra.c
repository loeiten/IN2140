#include "../include/dijkstra.h"

#include <limits.h>  // for INT_MAX
#include <stdio.h>   // for perror, NULL, fprintf, stderr
#include <stdlib.h>  // for malloc, free, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for memcpy

#include "../include/route.h"  // for Route

int dijkstra(const int src, const int *const *const graph,
             int *const distanceArray, struct Route **const routeArray,
             const int n) {
  // Solve Dijkstra's algorithm using shortest path three
  // Note that this solution has time complexity O(V^2) where V is the number of
  // vertices
  // By using a heap this can be reduced to O(E*log(V)) where E are the number
  // of edges

  // Allocate memory to the array of visited nodes
  int *visitedArray = (int *)malloc(n * sizeof(int));
  if (visitedArray == NULL) {
    perror("Could not allocate memory to visitedArray: ");
    return EXIT_FAILURE;
  }

  // Allocate memory to the routeArray
  struct Route *routeArrayTmp =
      (struct Route *)malloc(n * sizeof(struct Route));
  if (routeArrayTmp == NULL) {
    perror("Could not allocate memory to routeArray: ");
    freeIntArray(&visitedArray);
    return EXIT_FAILURE;
  }

  // Initialize the data
  for (int i = 0; i < n; ++i) {
    // Mark all nodes as unvisited
    visitedArray[i] = 0;
    // Set the distance to every node to zero
    distanceArray[i] = INT_MAX;
    // Init the RouteArray
    routeArrayTmp[i].route = (int *)malloc(n * sizeof(int));
    if (routeArrayTmp[i].route == NULL) {
      perror("Could not allocate memory to routeArrayTmp[i].route: ");
      // NOTE: The freeing of routeArrayTmp will happen in the place where it
      // was
      //       allocated
      freeIntArray(&visitedArray);
      // NOTE: Need to free the temporary as it's local, and not yet assigned to
      //       routeArray
      freeRouteArray(&routeArrayTmp, n);
      return EXIT_FAILURE;
    }
    routeArrayTmp[i].nHops = -1;
  }

  // The distance to itself is 0
  distanceArray[src] = 0;

  // Initialize the helper variable for registerRoute
  int *visitedAndNeighbourArray = (int *)malloc(n * sizeof(int));
  if (visitedAndNeighbourArray == NULL) {
    perror("Could not allocate memory to visitedAndNeighbourArray: ");
    // NOTE: The freeing of routeArray will happen in the place where it was
    //       allocated
    freeIntArray(&visitedArray);
    freeRouteArray(&routeArrayTmp, n);
    return EXIT_FAILURE;
  }

  // We need to iterate through all nodes to get their paths
  for (int iteration = 0; iteration < n; ++iteration) {
    // Pick the minumum distance from the set of vertices not yet processed
    int minIdx = getMinDistanceIdx(distanceArray, visitedArray, n);
    if (minIdx == -1) {
      fprintf(
          stderr,
          "Unexpected error: Could not find a minIdx for the %d iteration\n",
          iteration);
      freeIntArray(&visitedArray);
      freeRouteArray(&routeArrayTmp, n);
      return EXIT_FAILURE;
    }

    // Mark this node as visited
    visitedArray[minIdx] = 1;
    int success =
        registerRoute(src, minIdx, n, graph, visitedArray, distanceArray,
                      visitedAndNeighbourArray, routeArrayTmp);
    if (success != EXIT_SUCCESS) {
      freeIntArray(&visitedArray);
      freeIntArray(&visitedAndNeighbourArray);
      freeRouteArray(&routeArrayTmp, n);
      fprintf(stderr, "Could not register route in the routeArray\n");
      return EXIT_FAILURE;
    }

    // As we will find the shortest path to all nodes, and since we visit a new
    // node in each iteration, we need to iterate through n-1 nodes as we have
    // already found the shortest path to the node itself
    if (iteration < (n - 1)) {
      for (int i = 0; i < n; ++i) {
        if (
            // If this node has not been visited
            (visitedArray[i] == 0) &&
            // There must be an edge between minIdx and i
            (graph[minIdx][i] != 0) &&
            // A path from the source must have been registered
            (distanceArray[minIdx] != INT_MAX) &&
            // The new path must be smaller than any previously recorded
            (distanceArray[minIdx] + graph[minIdx][i] < distanceArray[i])) {
          // Update the shortest distance to the current node
          distanceArray[i] = distanceArray[minIdx] + graph[minIdx][i];
        }
      }
    }
  }

  // Finally assign the local temporary to the output value
  *routeArray = routeArrayTmp;

  freeIntArray(&visitedArray);
  freeIntArray(&visitedAndNeighbourArray);
  return EXIT_SUCCESS;
}

int getMinDistanceIdx(const int *const distanceArray,
                      const int *const visitedArray, const int n) {
  int minDist = INT_MAX;
  int minIdx = -1;

  for (int i = 0; i < n; ++i) {
    // We will just search through non-visited nodes
    // If the minIdx is to be updated, is must be less than any previously found
    if ((visitedArray[i] == 0) && (distanceArray[i] <= minDist)) {
      minDist = distanceArray[i];
      minIdx = i;
    }
  }

  return minIdx;
}

int registerRoute(const int src, const int curVisitIdx, const int n,
                  const int *const *const graph, const int *const visitedArray,
                  const int *const distanceArray,
                  int *const visitedAndNeighbourArray,
                  struct Route *const routeArray) {
  // Our strategy:
  // 1. Check the union of neighbors and visitedArray
  // 2. For each node in the union, check the minimum distance
  // 3. Copy the route from the minimum to the current route
  // 4. Add the curVisitIdx

  // Special case for the first node
  if (src == curVisitIdx) {
    routeArray[curVisitIdx].route[0] = src;
    routeArray[curVisitIdx].nHops = 0;
    return EXIT_SUCCESS;
  }

  // Step 1.
  int nVisitedAndNeighbour = 0;
  for (int neighbor = 0; neighbor < n; ++neighbor) {
    // Check if the node has a neighbor
    if (graph[curVisitIdx][neighbor] != 0) {
      // Check if this neighbor is visited
      if (visitedArray[neighbor] == 1) {
        visitedAndNeighbourArray[nVisitedAndNeighbour] = neighbor;
        ++nVisitedAndNeighbour;
      }
    }
  }

  // Step 2.
  int minDistNeighbor = INT_MAX;
  int minDistNeighborIdx = INT_MAX;
  for (int i = 0; i < nVisitedAndNeighbour; ++i) {
    int neighborId = visitedAndNeighbourArray[i];
    if ((distanceArray[neighborId] + graph[curVisitIdx][neighborId]) <
        minDistNeighbor) {
      minDistNeighbor =
          distanceArray[neighborId] + graph[curVisitIdx][neighborId];
      minDistNeighborIdx = neighborId;
    }
  }
  if (minDistNeighbor == INT_MAX || minDistNeighborIdx == INT_MAX) {
    fprintf(stderr, "Failed to find the shortest path to %d\n", curVisitIdx);
    return EXIT_FAILURE;
  }

  // Step 3.
  memcpy(routeArray[curVisitIdx].route, routeArray[minDistNeighborIdx].route,
         (routeArray[minDistNeighborIdx].nHops + 1) * sizeof(int));
  routeArray[curVisitIdx].nHops = routeArray[minDistNeighborIdx].nHops;

  // Step 4.
  ++routeArray[curVisitIdx].nHops;
  routeArray[curVisitIdx].route[routeArray[curVisitIdx].nHops] = curVisitIdx;

  return EXIT_SUCCESS;
}

void freeIntArray(int **visitedArray) {
  if ((*visitedArray) != NULL) {
    free(*visitedArray);
    *visitedArray = NULL;
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
