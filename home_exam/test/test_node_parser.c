#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <limits.h>  // for INT_MAX
#include <stdio.h>   // for fprintf, stderr, NULL
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>  // for strcmp

#include "../node/include/node_parser.h"      // for RoutingTable, Destinati...
#include "../utils/include/common.h"          // for IndexToAddress
#include "../utils/include/dynamic_memory.h"  // for freeRoutingTable

void testParseNodes(void) {
#define N (4)
  const char* nodes[N] = {"12:9", "17:1", "123:113", "88:99"};
  struct CommunicatedNode communicatedNode;
  allocateCommunicatedNodeNeighborAndWeights(&communicatedNode, N,
                                             "communicatedNode");
  assert(communicatedNode.nNeighbors == N);
  int success =
      parseNodes((const char* const* const)nodes, N, &communicatedNode);
  assert(success == EXIT_SUCCESS);
  assert(communicatedNode.neighborAddresses[0] == 12);
  assert(communicatedNode.neighborAddresses[1] == 17);
  assert(communicatedNode.neighborAddresses[2] == 123);
  assert(communicatedNode.neighborAddresses[3] == 88);
  assert(communicatedNode.edgeWeights[0] == 9);
  assert(communicatedNode.edgeWeights[1] == 1);
  assert(communicatedNode.edgeWeights[2] == 113);
  assert(communicatedNode.edgeWeights[3] == 99);
  freeNeighborAddressesAndEdgeWeights(&communicatedNode);
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
