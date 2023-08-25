#include "../include/dijkstra.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>  // for perror
#include <stdlib.h>

int getMinDistanceIdx(const int *const distance, const int *const visited,
                      const int n) {
  int minDist = INT_MAX;
  int minIdx = -1;

  for (int i = 0; i < n; ++i) {
    // We will just search through non-visited nodes
    // If the minIdx is to be updated, is must be less than any previously found
    if ((visited[i] == 0) && (distance[i] <= minDist)) {
      minDist = distance[i];
      minIdx = i;
    }
  }

  return minIdx;
}

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
             int *const *const distance, const int n) {
  // Solve Dijkstra's algorithm using shortest path three
  // Note that this solution has time complexity O(V^2) where V is the number of
  // vertices
  // By using a heap this can be reduced to O(E*log(V)) where E are the number
  // of edges

  // Allocate memory to the array of visited nodes
  int *visited = (int *)malloc((n) * sizeof(int));
  if (visited == NULL) {
    perror("Could not allocate memory to visited: ");
    return EXIT_FAILURE;
  }
  // Mark all nodes as unvisited
  for (int i = 0; i < n; ++i) {
    visited[i] = 0;
  }

  // First set the distance to every node to zero
  for (int i = 0; i < n; ++i) {
    (*distance)[n] = INT_MAX;
  }
  // The distance to itself is 0
  (*distance)[src] = 0;

  // As we will find the shortest path to all nodes, and since we visit a new
  // node in each iteration, we need to iterate through n-1 nodes as we have
  // already found the shortest path to the node itself
  for (int iteration = 0; iteration < (n - 1); ++iteration) {
    // Pick the minumum distance from the set of vertices not yet processed
    int minIdx = getMinDistanceIdx(*distance, visited, n);
    if (minIdx == -1) {
      free(visited);
      fprintf(stderr,
              "Unexpected error: Could not find a minIdx for the %d iteration",
              iteration);
      return EXIT_FAILURE;
    }

    // Mark this node as visited
    visited[minIdx] = 1;

    for (int i = 0; i < n; ++i) {
      if (
          // If this node has not been visited
          (visited[i] != 1) &&
          // There must be an edge between minIdx and i
          (graph[minIdx][i] != INT_MAX) &&
          // A path from the source must have been registered
          ((*distance)[minIdx] != INT_MAX) &&
          // The new path must be smaller than any previously recorded
          ((*distance)[minIdx] + graph[minIdx][i] < (*distance)[i])) {
        // Update the shortest distance to the current node
        (*distance)[i] = (*distance)[minIdx] + graph[minIdx][i];
      }
    }
  }

  free(visited);
  return EXIT_SUCCESS;
}
