#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for atoi, exit, EXIT_SUC...

#include "../../utils/include/common.h"          // for CommunicatedNode
#include "../../utils/include/dynamic_memory.h"  // for allocateNeighborAddr...
#include "../include/node_communication.h"       // for getTCPClientSocket
#include "../include/node_parser.h"              // for parseNodes

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
  success = parseNodes((const char *const *const)&(argv[3]), nNeighbors,
                       &communicatedNode);
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  int serverPort = atoi(argv[1]);

  // We now make a UDP socket we can both send to and receive from
  int udpSocketFd;
  // The UDP port should be port + ownAddress
  int udpPort = serverPort + atoi(argv[2]);
  success = getUDPSocket(&udpSocketFd, udpPort);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Failed to get UDP socket, exiting\n");
    exit(-1);
  }

  // Open a TCP connection to the routing_server
  int tcpRoutingServerSocketFd;
  success = getTCPClientSocket(&tcpRoutingServerSocketFd, serverPort);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Failed to connect to the server, exiting\n");
    exit(-2);
  }

  // Receive the routing table

  // NOTE: In TCP you can bind before connecting
  //       If not this is done automatically by the OS
  // https://idea.popcount.org/2014-04-03-bind-before-connect/

  // Await packet from the other nodes

  // NOTE: In UDP you can connect before sending
  //       When a UDP socket is created, its local and remote addresses are
  //       unspecified. Datagrams can be sent immediately using sendto(2) or
  //       sendmsg(2) with a valid destination address as an argument. When
  //       connect(2) is called on the socket, the default destination address
  //       is set and datagrams can now be sent using send(2) or write(2)
  //       without specifying a destination address.
  // https://linux.die.net/man/7/udp
  // However, we know all the ports in advance

  return EXIT_SUCCESS;
}
