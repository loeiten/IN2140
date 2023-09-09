#include "../include/dijkstra.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>  // for perror
#include <stdlib.h>

// FIXME: Should store the shortest path as well
//        The path would be updated when it's marked as visited
//          At this time one can loop through all the vertices of the hit
//          The one which is visited and has the shortest path is the prev
//          One can then copy the path until that point (from the vertex) and
//          add the current node to it
//        Can be a struct with array and N so that we don't have to loop every
//        time to find the next available slot
//        We can then check if the from_node is in the path by checking if it's
//        in one of the elements of shortest path from src to to_node
int dijkstra(const int src, const int *const *const graph,
             int *const distanceArray, struct Route *routeArray, const int n) {
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
  routeArray = (struct Route *)malloc(n * sizeof(struct Route));
  if (routeArray == NULL) {
    perror("Could not allocate memory to routeArray: ");
    freeVisitedArray(&visitedArray);
    return EXIT_FAILURE;
  }

  // Initialize the data
  for (int i = 0; i < n; ++i) {
    // Mark all nodes as unvisited
    visitedArray[i] = 0;
    // Set the distance to every node to zero
    distanceArray[i] = INT_MAX;
    // Init the RouteArray
    routeArray[i].route = (int *)malloc(n * sizeof(int));
    if (routeArray[i].route == NULL) {
      perror("Could not allocate memory to routeArray[i].route: ");
      // NOTE: The freeing of routeArray will happen in the place where it was
      //       allocated
      freeVisitedArray(&visitedArray);
      return EXIT_FAILURE;
    }
    routeArray[i].nHops = -1;
  }

  // The distance to itself is 0
  distanceArray[src] = 0;

  // As we will find the shortest path to all nodes, and since we visit a new
  // node in each iteration, we need to iterate through n-1 nodes as we have
  // already found the shortest path to the node itself
  for (int iteration = 0; iteration < (n - 1); ++iteration) {
    // Pick the minumum distance from the set of vertices not yet processed
    int minIdx = getMinDistanceIdx(distanceArray, visitedArray, n);
    if (minIdx == -1) {
      fprintf(
          stderr,
          "Unexpected error: Could not find a minIdx for the %d iteration\n",
          iteration);
      freeVisitedArray(&visitedArray);
      return EXIT_FAILURE;
    }

    // Mark this node as visited
    visitedArray[minIdx] = 1;
    int success = registerRoute(graph, minIdx, distanceArray, routeArray);
    if (success != EXIT_SUCCESS) {
      freeVisitedArray(&visitedArray);
      fprintf(stderr, "Could not register route in the routeArray\n");
      return EXIT_FAILURE;
    }

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

  freeVisitedArray(&visitedArray);
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

int registerRoute(const int *const *const graph, const int visitIdx,
                  const int *const distanceArray,
                  struct Route *const routeArray) {
  // FIXME: Needs implementation
  (void)graph;
  (void)visitIdx;
  (void)distanceArray;
  (void)routeArray;
  return EXIT_SUCCESS;
}

void freeVisitedArray(int **visitedArray) {
  if ((*visitedArray) != NULL) {
    free(*visitedArray);
    *visitedArray = NULL;
  }
}

void freeRouteArray(struct Route **routeArray, const int n) {
  if ((*routeArray) != NULL) {
    for (int i = 0; i < n; ++i) {
      if (routeArray[i]->route != NULL) {
        free(routeArray[i]->route);
        routeArray[i]->route = NULL;
      }
    }
    free(*routeArray);
    (*routeArray) = NULL;
  }
}
