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
  // FIXME:
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
  // Brute force algorithm
  // Let's say we want to find all destination from node i
  // 1. In the route array, find all routes which have i in it (where i is not
  //    the final destination)
  // 2. For each of these routes:
  //    i.   Look at the proceeding node after i and call it j
  //    ii.  Create an array, and put all the nodes proceeding j in this array.
  //         These nodes will have j as their nextHop
  //    iii. Sort this array...
  //    ii.
  // 3.

  // Finally assign the local temporary to the output value
  *routingTable = routingTableTmp;
  return EXIT_SUCCESS;
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
