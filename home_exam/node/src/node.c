#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS

#include "../../utils/include/common.h"          // for getIndexFromAddress
#include "../../utils/include/dynamic_memory.h"  // for freeIntArray, freeEd...
#include "../include/node_parser.h"              // for getIndexFromAddress

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr,
            "Usage: %s <Port> <OwnAddress> <NeighborAddress>:<weight> ...\n"
            "<Port> Value between 1024 and 65535 - number of nodes\n"
            "<OwnAddress> Node's address written as decimal number (1-1023) "
            "<NeighborAddress>:<weight>\n"
            "       <NeighborAddress> - Neighbor's address\n"
            "       <weight> - Link's weight to corresponding neighbor (an "
            "integer number)\n",
            basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Capture all the neighbors
  // One argument for the file name, one for the port and one for own address
  int nNeighbors = argc - 3;
  struct CommunicatedNode communicatedNode = {.nNeighbors = nNeighbors};
  int success = allocateNeighborAddressesAndEdgeWeights(
      &(communicatedNode), nNeighbors, "communicatedNode");
  if (success != EXIT_SUCCESS) {
    freeNeighborAddressesAndEdgeWeights(&communicatedNode);
    return EXIT_FAILURE;
  }
  success = parseNodes(argc, (const char *const *const)argv, &communicatedNode);
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Check that UDP socket P+A is valid

  // Bind to socket, exit with -1 if unsuccessful

  // Open a TCP connection to the routing_server

  // Receive the routing table

  // Await packet from the other nodes

  return EXIT_SUCCESS;
}
