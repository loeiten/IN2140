#include "../include/route.h"

#include <stdio.h>   // for NULL
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...

#include "../../utils/include/common.h"          // for RoutingTable, Destin...
#include "../../utils/include/dynamic_memory.h"  // for freeIntMatrix, alloc...
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
                const struct IndexToAddress *const indexToAddressMap,
                const int n) {
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
      print_weighted_edge(indexToAddressMap->map[fromNode],
                          indexToAddressMap->map[toNode], pathLen);
    }
  }
}

int createRoutingTableArray(struct Route *routeArray,
                            struct RoutingTable **const routingTableArray,
                            int n) {
  // Pseudo code:
  // 1. Create a n x n matrix called visited, the rows will be the source
  //    indices and the cols will be the destination indices
  // 2. Loop through the routeArray and
  //    i.   Set the destination pointer to the last element
  //    ii.  Set the source pointer to the second to last element
  //    iii. Loop through the elements of the route and
  //         a. Check if visited[sourceNode][destinationNode] is true, if yes
  //            go to step 2.iii.f
  //         b. Allocate a DestinationNextPair to
  //            routingTableArray[sourceNode].table[routingTableArray[sourceNode].n]
  //         c. Set
  //            routingTableArray[sourceNode].table[routingTableArray[sourceNode].n].destination
  //            = *destinationPtr
  //         c. Set
  //            routingTableArray[sourceNode].table[routingTableArray[sourceNode].n].nextHop
  //            = *(sourcePtr + 1)
  //         d. ++(routingTableArray[sourceNode].n)
  //         e. Mark visited[sourceNode][destinationNode] as true
  //         f. Decrement destination pointer, if destinationNode == sourceNode,
  //            go to the next element of routingArray, else go to step
  //            2.iii.a

  // Step 1
  // Allocate the visited matrix
  int **visited = NULL;
  int success = allocateIntMatrix(&visited, n, "visited");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  // Initialize and use the matrix
  for (int row = 0; row < n; ++row) {
    for (int col = 0; col < n; ++col) {
      visited[row][col] = 0;
    }
  }

  // Allocate the routing table
  struct RoutingTable *routingTableArrayTmp = NULL;
  success = allocateRoutingTableArray(&routingTableArrayTmp, n,
                                      "routingTableArrayTmp");
  if (success != EXIT_SUCCESS) {
    freeIntMatrix(&visited, n);
    return EXIT_FAILURE;
  }

  // Step 2
  // Loop through all the routes
  // NOTE: There will be one for each node unless there are any orphan nodes
  for (int routeIdx = 0; routeIdx < n; ++routeIdx) {
    // Start at the end of the route and go backwards
    int nextHopIdx = routeArray[routeIdx].nHops;
    int sourceIdx = nextHopIdx - 1;

    while (sourceIdx != -1) {
      // (Re)set the destinationIdx
      int destinationIdx = routeArray[routeIdx].nHops;

      // Fix the node values while looping over the destination nodes
      int sourceNode = routeArray[routeIdx].route[sourceIdx];
      int nextHopNode = routeArray[routeIdx].route[nextHopIdx];

      // Loop through the destinations
      while (destinationIdx != sourceIdx) {
        // Update the destination node
        int destinationNode = routeArray[routeIdx].route[destinationIdx];

        if (visited[sourceNode][destinationNode] == 1) {
          --destinationIdx;
          continue;
        }
        // Set the destination and next
        routingTableArrayTmp[sourceNode]
            .routingTableRow[routingTableArrayTmp[sourceNode].nRows]
            .destination = destinationNode;
        routingTableArrayTmp[sourceNode]
            .routingTableRow[routingTableArrayTmp[sourceNode].nRows]
            .nextHop = nextHopNode;
        // Mark visited
        visited[sourceNode][destinationNode] = 1;
        // Increment the number of DestinationNextPair in the table
        ++(routingTableArrayTmp[sourceNode].nRows);

        // Shift the destination left
        --destinationIdx;
      }

      // Shift the source and nextHop left
      --sourceIdx;
      --nextHopIdx;
    }
  }

  // Free allocated memory
  freeIntMatrix(&visited, n);

  // Finally assign the local temporary to the output value
  *routingTableArray = routingTableArrayTmp;
  return EXIT_SUCCESS;
}
