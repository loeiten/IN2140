#include "../include/route.h"

#include <stdio.h>   // for perror
#include <stdlib.h>  // for free, NULL, calloc, EXIT_FA...

// NOTE: We are not specifying the full path here
//       As a consequence we have to do the following
//       1. Use -I in the compilation to expand the include path of the compiler
//       2. If you use compile_commands.json for code completion,
//          compile errors, go-to definition etc. you need to create compile
//          command
//          You can do this with for example
//          bear --output ../build/compile_commands.json --append -- make -B
#include "print_lib/include/print_lib.h"  // for print_weighted_edge

void printEdges(const int *const distanceArray,
                const struct Route *const routeArray,
                const struct IndexToId *const indexToIdMap, const int n) {
  for (int fromNode = 0; fromNode < n; ++fromNode) {
    for (int toNode = 0; toNode < n; ++toNode) {
      // Check if fromNode is on the shortest path from the source to the toNode
      const int *const shortestPath = routeArray[toNode].route;
      const int nHops = routeArray[toNode].nHops;
      int pathLen = -1;
      // NOTE: We include the nHop as a 0 hop is the same as the source node
      for (int i = 0; i <= nHops; ++i) {
        if (shortestPath[i] == fromNode) {
          // Get the distance from the source to fromNode
          pathLen = distanceArray[fromNode];
          break;
        }
      }

      // Print the value
      print_weighted_edge(indexToIdMap->map[fromNode],
                          indexToIdMap->map[toNode], pathLen);
    }
  }
}

int createRoutingTables(struct Route *routeArray,
                        struct RoutingTable **const routingTable, int n) {
  // Allocate the visited matrix
  int **visited = NULL;
  // Allocate memory for the array of pointers
  visited = (int **)calloc(n, sizeof(int *));
  if (visited == NULL) {
    perror("Could not allocate memory to the *visited array: ");
    return EXIT_FAILURE;
  }
  // Allocate memory for each row individually
  for (int i = 0; i < n; i++) {
    visited[i] = (int *)malloc(n * sizeof(int));
    if (visited[i] == NULL) {
      fprintf(stderr, "Memory allocation failed for row %d.\n", i);
      return 1;
    }
  }
  // Initialize and use the matrix
  for (int row = 0; row < n; ++row) {
    for (int col = 0; col < n; ++col) {
      visited[row][col] = 0;
    }
  }

  // FIXME:
  (void)visited;
  (void)routeArray;
  (void)n;

  // Zero allocate the routing table
  struct RoutingTable *routingTableTmp =
      (struct RoutingTable *)calloc(n, sizeof(struct RoutingTable));
  if (routingTableTmp == NULL) {
    perror("Could not allocate memory to the routingTable: ");
    return EXIT_FAILURE;
  }

  // Pseudo code:
  // 1. Create a n x n matrix called visited, the rows will be the source
  //    indices and the rows will be the destination indices
  // 2. Loop through the routeArray and
  //    i.   Set the destination pointer to the last element
  //    ii.  Set the source pointer to the second to last element
  //    iii. Loop through the elements of the route and
  //         a. Check if visited[*sourcePtr][*destinationPtr] is true, if yes
  //            go to step 2.iii.f
  //         b. Allocate a DestinationNextPair to
  //            routingTable[*sourcePtr].table[routingTable[*sourcePtr].n]
  //         c. Set
  //            routingTable[*sourcePtr].table[routingTable[*sourcePtr].n].destination
  //            = *destinationPtr
  //         c. Set
  //            routingTable[*sourcePtr].table[routingTable[*sourcePtr].n].nextHop
  //            = *(sourcePtr + 1)
  //         d. ++(routingTable[*sourcePtr].n)
  //         e. Mark visited[*sourcePtr][*destinationPtr] as true
  //         f. Decrement destination pointer, if *destinationPtr == *sourcePtr,
  //            go to the next element of routingArray, else go to step
  //            2.iii.a

  // Finally assign the local temporary to the output value
  *routingTable = routingTableTmp;
  return EXIT_SUCCESS;
}
