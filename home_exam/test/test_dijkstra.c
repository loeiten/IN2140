#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_S...
#include <string.h>  // for strcmp

#include "../routing_server/include/dijkstra.h"  // for getMinDistanceIdx

void testGetMinDistanceIdx(void) {
  const int n = 2;
  const int distance[2] = {15, 7};
  const int visited[2] = {0, 0};
  int minIdx = getMinDistanceIdx(distance, visited, n);
  assert(minIdx == 1);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "getMinDistanceIdx") == 0) {
    testGetMinDistanceIdx();
  }

  return EXIT_SUCCESS;
}
