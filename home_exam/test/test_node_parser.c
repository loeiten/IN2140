#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strcmp

#include "../node/include/node_parser.h"      // for parseNodes
#include "../utils/include/common.h"          // for Node
#include "../utils/include/dynamic_memory.h"  // for allocateNodeNeighborAnd...

void testParseNodes(void) {
#define N (4)
  const char* nodes[N] = {"12:9", "17:1", "123:113", "88:99"};
  struct Node node;
  allocateNodeNeighborAndWeights(&node, N, "node");
  assert(node.nNeighbors == N);
  int success = parseNodes((const char* const* const)nodes, N, &node);
  assert(success == EXIT_SUCCESS);
  assert(node.neighborAddresses[0] == 12);
  assert(node.neighborAddresses[1] == 17);
  assert(node.neighborAddresses[2] == 123);
  assert(node.neighborAddresses[3] == 88);
  assert(node.edgeWeights[0] == 9);
  assert(node.edgeWeights[1] == 1);
  assert(node.edgeWeights[2] == 113);
  assert(node.edgeWeights[3] == 99);
  freeNeighborAddressesAndEdgeWeights(&node);
#undef N
  return;
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

  if (strcmp(argv[1], "parseNodes") == 0) {
    testParseNodes();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
