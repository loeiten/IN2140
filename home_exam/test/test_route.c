#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <limits.h>  // for INT_MAX
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strcmp

#include "../routing_server/include/route.h"  // for printEdges, IndexToId

// NOTE: We are not specifying the full path here
//       As a consequence we have to do the following
//       1. Use -I in the compilation to expand the include path of the compiler
//       2. If you use compile_commands.json for code completion,
//          compile errors, go-to definition etc. you need to create compile
//          command
//          You can do this with for example
//          bear --output ../build/compile_commands.json --append -- make -B
#include "print_lib/include/print_lib.h"  // for print_weighted_edge

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
  const struct IndexToId indexToIdMap = {.n = N, .map = map};

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
  printEdges(distanceArray, routeArray, &indexToIdMap, N);
#undef N
}

void testCreateRoutingTables(void) {
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
  struct RoutingTable *routingTable = NULL;
  int success = createRoutingTables(routeArrayA, &routingTable, N);
  assert(success == EXIT_SUCCESS);
  // FIXME: Make a choice whether one should use label or indices on the labels
  //        Easy could be to use a 1 to 1 map and use labels, or use a label
  //        like (i*10 + i/2) for all nodes except node 1
  //        NOTE: In general it's dangerous to rely on the index as there is no
  //        guarantee what order the responses come in
  // Check for 0th index node
  assert(routingTable[0].n == 8);
  assert(routingTable[0].table[0].destination == 1);
  assert(routingTable[0].table[0].nextHop == 1);
  assert(routingTable[0].table[1].destination == 2);
  assert(routingTable[0].table[1].nextHop == 1);
  assert(routingTable[0].table[2].destination == 3);
  assert(routingTable[0].table[2].nextHop == 1);
  assert(routingTable[0].table[3].destination == 4);
  assert(routingTable[0].table[3].nextHop == 7);
  assert(routingTable[0].table[4].destination == 5);
  assert(routingTable[0].table[4].nextHop == 7);
  assert(routingTable[0].table[5].destination == 6);
  assert(routingTable[0].table[5].nextHop == 7);
  assert(routingTable[0].table[6].destination == 7);
  assert(routingTable[0].table[6].nextHop == 7);
  assert(routingTable[0].table[7].destination == 8);
  assert(routingTable[0].table[7].nextHop == 1);
  // Check for 1st index node
  assert(routingTable[1].n == 3);
  assert(routingTable[1].table[0].destination == 2);
  assert(routingTable[1].table[0].nextHop == 2);
  assert(routingTable[1].table[1].destination == 3);
  assert(routingTable[1].table[1].nextHop == 2);
  assert(routingTable[1].table[2].destination == 8);
  assert(routingTable[1].table[2].nextHop == 2);
  // Check for 2nd index node
  assert(routingTable[2].n == 2);
  assert(routingTable[2].table[0].destination == 3);
  assert(routingTable[2].table[0].nextHop == 3);
  assert(routingTable[2].table[1].destination == 8);
  assert(routingTable[2].table[1].nextHop == 8);
  // Check for 3rd index node
  assert(routingTable[3].n == 0);
  // Check for 4th index node
  assert(routingTable[4].n == 0);
  // Check for 5th index node
  assert(routingTable[5].n == 1);
  assert(routingTable[5].table[0].destination == 4);
  assert(routingTable[5].table[0].nextHop == 4);
  // Check for 6th index node
  assert(routingTable[6].n == 2);
  assert(routingTable[6].table[0].destination == 4);
  assert(routingTable[6].table[0].nextHop == 4);
  assert(routingTable[6].table[1].destination == 5);
  assert(routingTable[6].table[1].nextHop == 4);
  // Check for 7th index node
  assert(routingTable[7].n == 3);
  assert(routingTable[7].table[0].destination == 4);
  assert(routingTable[7].table[0].nextHop == 6);
  assert(routingTable[7].table[1].destination == 5);
  assert(routingTable[7].table[1].nextHop == 6);
  assert(routingTable[7].table[2].destination == 6);
  assert(routingTable[7].table[2].nextHop == 6);
  // Check for 8th index node
  assert(routingTable[8].n == 0);
  freeRoutingTable(&routingTable, N);

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
  success = createRoutingTables(routeArrayB, &routingTable, N);
  assert(success == EXIT_SUCCESS);
  // Check for 0th index node
  assert(routingTable[0].n == 0);
  // Check for 1st index node
  assert(routingTable[1].n == 1);
  assert(routingTable[1].table[0].destination == 0);
  assert(routingTable[1].table[0].nextHop == 0);
  // Check for 2nd index node
  assert(routingTable[2].n == 4);
  assert(routingTable[2].table[0].destination == 0);
  assert(routingTable[2].table[0].nextHop == 1);
  assert(routingTable[2].table[1].destination == 1);
  assert(routingTable[2].table[1].nextHop == 1);
  assert(routingTable[2].table[2].destination == 3);
  assert(routingTable[2].table[2].nextHop == 3);
  assert(routingTable[2].table[3].destination == 5);
  assert(routingTable[2].table[3].nextHop == 5);
  // Check for 3rd index node
  assert(routingTable[3].n == 0);
  // Check for 4th index node
  assert(routingTable[4].n == 0);
  // Check for 5th index node
  assert(routingTable[5].n == 1);
  assert(routingTable[5].table[0].destination == 4);
  assert(routingTable[5].table[0].nextHop == 4);
  // Check for 6th index node
  assert(routingTable[6].n == 1);
  assert(routingTable[6].table[0].destination == 7);
  assert(routingTable[6].table[0].nextHop == 7);
  // Check for 7th index node
  assert(routingTable[7].n == 0);
  // Check for 8th index node
  assert(routingTable[8].n == 8);
  assert(routingTable[8].table[0].destination == 0);
  assert(routingTable[8].table[0].nextHop == 2);
  assert(routingTable[8].table[1].destination == 1);
  assert(routingTable[8].table[1].nextHop == 2);
  assert(routingTable[8].table[2].destination == 2);
  assert(routingTable[8].table[2].nextHop == 2);
  assert(routingTable[8].table[3].destination == 3);
  assert(routingTable[8].table[3].nextHop == 2);
  assert(routingTable[8].table[4].destination == 4);
  assert(routingTable[8].table[4].nextHop == 2);
  assert(routingTable[8].table[5].destination == 5);
  assert(routingTable[8].table[5].nextHop == 2);
  assert(routingTable[8].table[6].destination == 6);
  assert(routingTable[8].table[6].nextHop == 6);
  assert(routingTable[8].table[7].destination == 7);
  assert(routingTable[8].table[7].nextHop == 6);
  freeRoutingTable(&routingTable, N);
#undef N

#define N (6)
  // Graph from https://www.programiz.com/dsa/dijkstra-algorithm
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
  success = createRoutingTables(routeArrayC, &routingTable, N);
  assert(success == EXIT_SUCCESS);

  assert(routingTable[0].n == 5);
  assert(routingTable[0].table[0].destination == 1);
  assert(routingTable[0].table[0].nextHop == 1);
  assert(routingTable[0].table[1].destination == 2);
  assert(routingTable[0].table[1].nextHop == 1);
  assert(routingTable[0].table[2].destination == 3);
  assert(routingTable[0].table[2].nextHop == 1);
  assert(routingTable[0].table[3].destination == 4);
  assert(routingTable[0].table[3].nextHop == 1);
  assert(routingTable[0].table[4].destination == 5);
  assert(routingTable[0].table[4].nextHop == 5);
  // Check for 1st index node
  assert(routingTable[1].n == 4);
  assert(routingTable[1].table[0].destination == 2);
  assert(routingTable[1].table[0].nextHop == 2);
  assert(routingTable[1].table[1].destination == 3);
  assert(routingTable[1].table[1].nextHop == 4);
  assert(routingTable[1].table[2].destination == 4);
  assert(routingTable[1].table[2].nextHop == 4);
  assert(routingTable[1].table[3].destination == 5);
  assert(routingTable[1].table[3].nextHop == 5);
  // Check for 2nd index node
  assert(routingTable[2].n == 0);
  // Check for 3rd index node
  assert(routingTable[3].n == 0);
  // Check for 4th index node
  assert(routingTable[4].n == 1);
  assert(routingTable[4].table[0].destination == 3);
  assert(routingTable[4].table[0].nextHop == 3);
  // Check for 5th index node
  assert(routingTable[5].n == 0);
  freeRoutingTable(&routingTable, N);
#undef N
}

int main(int argc, char **argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n1", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "printEdges") == 0) {
    testPrintEdges();
  } else if (strcmp(argv[1], "createRoutingTables") == 0) {
    testCreateRoutingTables();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }

  return EXIT_SUCCESS;
}
