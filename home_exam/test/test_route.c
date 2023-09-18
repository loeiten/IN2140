#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <limits.h>  // for INT_MAX
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...
#include <string.h>  // for strcmp

#include "../routing_server/include/route.h"  // for Route

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
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }

  return EXIT_SUCCESS;
}
