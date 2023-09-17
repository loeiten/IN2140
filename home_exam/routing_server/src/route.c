#include "../include/route.h"

#include <stdio.h>

// NOTE: We are not specifying the full path here
//       As a consequence we have to do the following
//       1. Use -I in the compilation to expand the include path of the compiler
//       2. If you use compile_commands.json for code completion,
//          compile errors, go-to definition etc. you need to create compile
//          command
//          You can do this with for example
//          bear --output ../build/compile_commands.json --append -- make -B
#include "print_lib/include/print_lib.h"

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
