#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr, NULL
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_S...

#include "../../utils/include/common.h"          // for IndexToAddress
#include "../../utils/include/dynamic_memory.h"  // for allocateRoutingServer

int main(int argc, char** argv) {
  if (argc < 3) {
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

  // Check that UDP socket P+A is valid

  // Bind to socket, exit with -1 if unsuccessful

  // Open a TCP connection to the routing_server

  // Receive the routing table

  // Await packet from the other nodes

  return EXIT_SUCCESS;
}
