#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <limits.h>  // for INT_MAX
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>  // for strcmp

#include "../routing_server/include/route.h"  // for createRoutingTableArray
#include "../utils/include/common.h"          // for RoutingTable, RoutingTa...
#include "../utils/include/dynamic_memory.h"  // for freeRoutingTableArray
// NOTE: We are not specifying the full path here
//       As a consequence we have to do the following
//       1. Use -I in the compilation to expand the include path of the compiler
//       2. If you use compile_commands.json for code completion,
//          compile errors, go-to definition etc. you need to create compile
//          command
//          You can do this with for example
//          bear --output ../build/compile_commands.json --append -- make -B
#include "print_lib/include/print_lib.h"  // for print_clear_logfile

void testPrintEdges(void) {
#define N (5)
  // Graph
  // The weight are written on the edges
  // The ids are written on the vertices
  // The indices are written in square brackets
  //   10 [2]   17 [4]
  //   1 |        | 1
  //   3 [1]   5 [3]
  //   1  \     /  3
  //       1 [0]
  int map[N] = {1, 3, 10, 5, 17};
  const struct IndexToAddress indexToAddressMap = {.n = N, .map = map};

  const int distanceArray[N] = {0, 1, 2, 3, 4};

  int route0[N] = {0, INT_MAX, INT_MAX, INT_MAX, INT_MAX};
  int route1[N] = {0, 1, INT_MAX, INT_MAX, INT_MAX};
  int route2[N] = {0, 1, 2, INT_MAX, INT_MAX};
  int route3[N] = {0, 3, INT_MAX, INT_MAX, INT_MAX};
  int route4[N] = {0, 3, 4, INT_MAX, INT_MAX};
  const struct Route routeArray[N] = {{.nHops = 0, .route = route0},
                                      {.nHops = 1, .route = route1},
                                      {.nHops = 2, .route = route2},
                                      {.nHops = 1, .route = route3},
                                      {.nHops = 2, .route = route4}};

  print_clear_logfile();
  printEdges(distanceArray, routeArray, &indexToAddressMap, N);
#undef N
}

void testCreateRoutingTableArray(void) {
#define N (9)
  // GraphA from
  // https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
  int routeA0[1] = {0};
  int routeA1[2] = {0, 1};
  int routeA2[3] = {0, 1, 2};
  int routeA3[4] = {0, 1, 2, 3};
  int routeA4[5] = {0, 7, 6, 5, 4};
  int routeA5[5] = {0, 7, 6, 5};
  int routeA6[3] = {0, 7, 6};
  int routeA7[2] = {0, 7};
  int routeA8[4] = {0, 1, 2, 8};
  struct Route routeArrayA[N] = {
      {.nHops = 0, .route = routeA0}, {.nHops = 1, .route = routeA1},
      {.nHops = 2, .route = routeA2}, {.nHops = 3, .route = routeA3},
      {.nHops = 4, .route = routeA4}, {.nHops = 3, .route = routeA5},
      {.nHops = 2, .route = routeA6}, {.nHops = 1, .route = routeA7},
      {.nHops = 3, .route = routeA8},
  };
  struct RoutingTableArray routingTableArray;
  int success = createRoutingTableArray(routeArrayA, &routingTableArray, N);
  assert(success == EXIT_SUCCESS);

  // Check for 0th index node
  assert(routingTableArray.routingTables[0].nRows == 8);
  assert(routingTableArray.routingTables[0].routingTableRows[0].destination ==
         1);
  assert(routingTableArray.routingTables[0].routingTableRows[0].nextHop == 1);
  assert(routingTableArray.routingTables[0].routingTableRows[1].destination ==
         2);
  assert(routingTableArray.routingTables[0].routingTableRows[1].nextHop == 1);
  assert(routingTableArray.routingTables[0].routingTableRows[2].destination ==
         3);
  assert(routingTableArray.routingTables[0].routingTableRows[2].nextHop == 1);
  assert(routingTableArray.routingTables[0].routingTableRows[3].destination ==
         4);
  assert(routingTableArray.routingTables[0].routingTableRows[3].nextHop == 7);
  assert(routingTableArray.routingTables[0].routingTableRows[4].destination ==
         5);
  assert(routingTableArray.routingTables[0].routingTableRows[4].nextHop == 7);
  assert(routingTableArray.routingTables[0].routingTableRows[5].destination ==
         6);
  assert(routingTableArray.routingTables[0].routingTableRows[5].nextHop == 7);
  assert(routingTableArray.routingTables[0].routingTableRows[6].destination ==
         7);
  assert(routingTableArray.routingTables[0].routingTableRows[6].nextHop == 7);
  assert(routingTableArray.routingTables[0].routingTableRows[7].destination ==
         8);
  assert(routingTableArray.routingTables[0].routingTableRows[7].nextHop == 1);
  // Check for 1st index node
  assert(routingTableArray.routingTables[1].nRows == 3);
  assert(routingTableArray.routingTables[1].routingTableRows[0].destination ==
         2);
  assert(routingTableArray.routingTables[1].routingTableRows[0].nextHop == 2);
  assert(routingTableArray.routingTables[1].routingTableRows[1].destination ==
         3);
  assert(routingTableArray.routingTables[1].routingTableRows[1].nextHop == 2);
  assert(routingTableArray.routingTables[1].routingTableRows[2].destination ==
         8);
  assert(routingTableArray.routingTables[1].routingTableRows[2].nextHop == 2);
  // Check for 2nd index node
  assert(routingTableArray.routingTables[2].nRows == 2);
  assert(routingTableArray.routingTables[2].routingTableRows[0].destination ==
         3);
  assert(routingTableArray.routingTables[2].routingTableRows[0].nextHop == 3);
  assert(routingTableArray.routingTables[2].routingTableRows[1].destination ==
         8);
  assert(routingTableArray.routingTables[2].routingTableRows[1].nextHop == 8);
  // Check for 3rd index node
  assert(routingTableArray.routingTables[3].nRows == 0);
  // Check for 4th index node
  assert(routingTableArray.routingTables[4].nRows == 0);
  // Check for 5th index node
  assert(routingTableArray.routingTables[5].nRows == 1);
  assert(routingTableArray.routingTables[5].routingTableRows[0].destination ==
         4);
  assert(routingTableArray.routingTables[5].routingTableRows[0].nextHop == 4);
  // Check for 6th index node
  assert(routingTableArray.routingTables[6].nRows == 2);
  assert(routingTableArray.routingTables[6].routingTableRows[0].destination ==
         4);
  assert(routingTableArray.routingTables[6].routingTableRows[0].nextHop == 5);
  assert(routingTableArray.routingTables[6].routingTableRows[1].destination ==
         5);
  assert(routingTableArray.routingTables[6].routingTableRows[1].nextHop == 5);
  // Check for 7th index node
  assert(routingTableArray.routingTables[7].nRows == 3);
  assert(routingTableArray.routingTables[7].routingTableRows[0].destination ==
         4);
  assert(routingTableArray.routingTables[7].routingTableRows[0].nextHop == 6);
  assert(routingTableArray.routingTables[7].routingTableRows[1].destination ==
         5);
  assert(routingTableArray.routingTables[7].routingTableRows[1].nextHop == 6);
  assert(routingTableArray.routingTables[7].routingTableRows[2].destination ==
         6);
  assert(routingTableArray.routingTables[7].routingTableRows[2].nextHop == 6);
  // Check for 8th index node
  assert(routingTableArray.routingTables[8].nRows == 0);
  freeRoutingTableArray(&routingTableArray);

  // Using 8 as the source
  int routeB0[4] = {8, 2, 1, 0};
  int routeB1[3] = {8, 2, 1};
  int routeB2[2] = {8, 2};
  int routeB3[3] = {8, 2, 3};
  int routeB4[4] = {8, 2, 5, 4};
  int routeB5[3] = {8, 2, 5};
  int routeB6[2] = {8, 6};
  int routeB7[3] = {8, 6, 7};
  int routeB8[1] = {8};
  struct Route routeArrayB[N] = {
      {.nHops = 3, .route = routeB0}, {.nHops = 2, .route = routeB1},
      {.nHops = 1, .route = routeB2}, {.nHops = 2, .route = routeB3},
      {.nHops = 3, .route = routeB4}, {.nHops = 2, .route = routeB5},
      {.nHops = 1, .route = routeB6}, {.nHops = 2, .route = routeB7},
      {.nHops = 0, .route = routeB8},
  };
  success = createRoutingTableArray(routeArrayB, &routingTableArray, N);
  assert(success == EXIT_SUCCESS);
  // Check for 0th index node
  assert(routingTableArray.routingTables[0].nRows == 0);
  // Check for 1st index node
  assert(routingTableArray.routingTables[1].nRows == 1);
  assert(routingTableArray.routingTables[1].routingTableRows[0].destination ==
         0);
  assert(routingTableArray.routingTables[1].routingTableRows[0].nextHop == 0);
  // Check for 2nd index node
  assert(routingTableArray.routingTables[2].nRows == 5);
  assert(routingTableArray.routingTables[2].routingTableRows[0].destination ==
         0);
  assert(routingTableArray.routingTables[2].routingTableRows[0].nextHop == 1);
  assert(routingTableArray.routingTables[2].routingTableRows[1].destination ==
         1);
  assert(routingTableArray.routingTables[2].routingTableRows[1].nextHop == 1);
  assert(routingTableArray.routingTables[2].routingTableRows[2].destination ==
         3);
  assert(routingTableArray.routingTables[2].routingTableRows[2].nextHop == 3);
  assert(routingTableArray.routingTables[2].routingTableRows[3].destination ==
         4);
  assert(routingTableArray.routingTables[2].routingTableRows[3].nextHop == 5);
  assert(routingTableArray.routingTables[2].routingTableRows[4].destination ==
         5);
  assert(routingTableArray.routingTables[2].routingTableRows[4].nextHop == 5);
  // Check for 3rd index node
  assert(routingTableArray.routingTables[3].nRows == 0);
  // Check for 4th index node
  assert(routingTableArray.routingTables[4].nRows == 0);
  // Check for 5th index node
  assert(routingTableArray.routingTables[5].nRows == 1);
  assert(routingTableArray.routingTables[5].routingTableRows[0].destination ==
         4);
  assert(routingTableArray.routingTables[5].routingTableRows[0].nextHop == 4);
  // Check for 6th index node
  assert(routingTableArray.routingTables[6].nRows == 1);
  assert(routingTableArray.routingTables[6].routingTableRows[0].destination ==
         7);
  assert(routingTableArray.routingTables[6].routingTableRows[0].nextHop == 7);
  // Check for 7th index node
  assert(routingTableArray.routingTables[7].nRows == 0);
  // Check for 8th index node
  assert(routingTableArray.routingTables[8].nRows == 8);
  assert(routingTableArray.routingTables[8].routingTableRows[0].destination ==
         0);
  assert(routingTableArray.routingTables[8].routingTableRows[0].nextHop == 2);
  assert(routingTableArray.routingTables[8].routingTableRows[1].destination ==
         1);
  assert(routingTableArray.routingTables[8].routingTableRows[1].nextHop == 2);
  assert(routingTableArray.routingTables[8].routingTableRows[2].destination ==
         2);
  assert(routingTableArray.routingTables[8].routingTableRows[2].nextHop == 2);
  assert(routingTableArray.routingTables[8].routingTableRows[3].destination ==
         3);
  assert(routingTableArray.routingTables[8].routingTableRows[3].nextHop == 2);
  assert(routingTableArray.routingTables[8].routingTableRows[4].destination ==
         4);
  assert(routingTableArray.routingTables[8].routingTableRows[4].nextHop == 2);
  assert(routingTableArray.routingTables[8].routingTableRows[5].destination ==
         5);
  assert(routingTableArray.routingTables[8].routingTableRows[5].nextHop == 2);
  assert(routingTableArray.routingTables[8].routingTableRows[6].destination ==
         6);
  assert(routingTableArray.routingTables[8].routingTableRows[6].nextHop == 6);
  assert(routingTableArray.routingTables[8].routingTableRows[7].destination ==
         7);
  assert(routingTableArray.routingTables[8].routingTableRows[7].nextHop == 6);
  freeRoutingTableArray(&routingTableArray);
#undef N

#define N (6)
  // GraphB from https://www.programiz.com/dsa/dijkstra-algorithm
  // Check for 0th index node
  int routeC0[1] = {0};
  int routeC1[2] = {0, 1};
  int routeC2[3] = {0, 1, 2};
  int routeC3[4] = {0, 1, 4, 3};
  int routeC4[3] = {0, 1, 4};
  int routeC5[2] = {0, 5};
  struct Route routeArrayC[N] = {
      {.nHops = 0, .route = routeC0}, {.nHops = 1, .route = routeC1},
      {.nHops = 2, .route = routeC2}, {.nHops = 3, .route = routeC3},
      {.nHops = 2, .route = routeC4}, {.nHops = 1, .route = routeC5},
  };
  success = createRoutingTableArray(routeArrayC, &routingTableArray, N);
  assert(success == EXIT_SUCCESS);

  // Check for 0th index node
  assert(routingTableArray.routingTables[0].nRows == 5);
  assert(routingTableArray.routingTables[0].routingTableRows[0].destination ==
         1);
  assert(routingTableArray.routingTables[0].routingTableRows[0].nextHop == 1);
  assert(routingTableArray.routingTables[0].routingTableRows[1].destination ==
         2);
  assert(routingTableArray.routingTables[0].routingTableRows[1].nextHop == 1);
  assert(routingTableArray.routingTables[0].routingTableRows[2].destination ==
         3);
  assert(routingTableArray.routingTables[0].routingTableRows[2].nextHop == 1);
  assert(routingTableArray.routingTables[0].routingTableRows[3].destination ==
         4);
  assert(routingTableArray.routingTables[0].routingTableRows[3].nextHop == 1);
  assert(routingTableArray.routingTables[0].routingTableRows[4].destination ==
         5);
  assert(routingTableArray.routingTables[0].routingTableRows[4].nextHop == 5);
  // Check for 1st index node
  assert(routingTableArray.routingTables[1].nRows == 3);
  assert(routingTableArray.routingTables[1].routingTableRows[0].destination ==
         2);
  assert(routingTableArray.routingTables[1].routingTableRows[0].nextHop == 2);
  assert(routingTableArray.routingTables[1].routingTableRows[1].destination ==
         3);
  assert(routingTableArray.routingTables[1].routingTableRows[1].nextHop == 4);
  assert(routingTableArray.routingTables[1].routingTableRows[2].destination ==
         4);
  assert(routingTableArray.routingTables[1].routingTableRows[2].nextHop == 4);
  // Check for 2nd index node
  assert(routingTableArray.routingTables[2].nRows == 0);
  // Check for 3rd index node
  assert(routingTableArray.routingTables[3].nRows == 0);
  // Check for 4th index node
  assert(routingTableArray.routingTables[4].nRows == 1);
  assert(routingTableArray.routingTables[4].routingTableRows[0].destination ==
         3);
  assert(routingTableArray.routingTables[4].routingTableRows[0].nextHop == 3);
  // Check for 5th index node
  assert(routingTableArray.routingTables[5].nRows == 0);
  freeRoutingTableArray(&routingTableArray);
#undef N
}

int main(int argc, char **argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "printEdges") == 0) {
    testPrintEdges();
  } else if (strcmp(argv[1], "createRoutingTableArray") == 0) {
    testCreateRoutingTableArray();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
