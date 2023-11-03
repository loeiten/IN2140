#include "../include/node.h"

#include <errno.h>
#include <libgen.h>      // for basename
#include <netinet/in.h>  // for sockaddr_in, INADDR_LOOPBACK
#include <stdio.h>       // for fprintf, stderr
#include <stdlib.h>      // for exit, EXIT_SUCCESS
#include <string.h>
#include <sys/socket.h>  // for send, socket, AF_LOCAL, bind
#include <unistd.h>      // for close

#include "../../utils/include/common.h"          // for CommunicatedNode
#include "../../utils/include/dynamic_memory.h"  // for freeNeighborAddresse...
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

  int serverPort = atoi(argv[1]);
  int ownAddress = atoi(argv[2]);

  // Initialize variables to be cleaned
  int tcpRoutingServerSocketFd = -1;
  int udpSocketFd = -1;
  struct RoutingTable *routingTablePtr = NULL;

  // Capture all the neighbors
  // One argument for the file name, one for the port and one for own address
  int nNeighbors = argc - 3;
  struct CommunicatedNode communicatedNode = {.address = ownAddress,
                                              .nNeighbors = nNeighbors};
  int success = allocateNeighborAddressesAndEdgeWeights(
      &(communicatedNode), nNeighbors, "communicatedNode");
  if (success != EXIT_SUCCESS) {
    cleanup(&communicatedNode, routingTablePtr, &tcpRoutingServerSocketFd,
            &udpSocketFd, "\0");
    return EXIT_FAILURE;
  }
  success = parseNodes((const char *const *const)&(argv[3]), nNeighbors,
                       &communicatedNode);
  if (success != EXIT_SUCCESS) {
    freeNeighborAddressesAndEdgeWeights(&communicatedNode);
    return EXIT_FAILURE;
  }

  // We now make a UDP socket we can both send to and receive from
  // The UDP port should be port + ownAddress
  int udpPort = serverPort + ownAddress;
  success = getUDPSocket(&udpSocketFd, udpPort);
  if (success != EXIT_SUCCESS) {
    cleanup(&communicatedNode, routingTablePtr, &tcpRoutingServerSocketFd,
            &udpSocketFd, "Failed to get UDP socket, exiting\n");
    exit(-1);
  }

  // Open a TCP connection to the routing_server
  success = getTCPClientSocket(&tcpRoutingServerSocketFd, serverPort);
  if (success != EXIT_SUCCESS) {
    cleanup(&communicatedNode, routingTablePtr, &tcpRoutingServerSocketFd,
            &udpSocketFd, "Failed to connect to the server, exiting\n");
    exit(-2);
  }

  // Send the edge information
  success = sendEdgeInformation(tcpRoutingServerSocketFd, &communicatedNode);
  if (success != EXIT_SUCCESS) {
    cleanup(&communicatedNode, routingTablePtr, &tcpRoutingServerSocketFd,
            &udpSocketFd, "Failed to send the edge information\n");
    return EXIT_FAILURE;
  }

  // Receive the routing table
  struct RoutingTable routingTable;
  routingTablePtr = &routingTable;
  success = receiveRoutingTable(tcpRoutingServerSocketFd, routingTablePtr);
  if (success != EXIT_SUCCESS) {
    cleanup(&communicatedNode, routingTablePtr, &tcpRoutingServerSocketFd,
            &udpSocketFd, "Failed to receive the edge information\n");
    return EXIT_FAILURE;
  }

  // Await packet from the other nodes
  const char *msg = "";
  while (strcmp(msg, "QUIT") != 0) {
    success = receiveAndForwardPackets(udpSocketFd, ownAddress, serverPort,
                                       routingTablePtr);
    if (success != EXIT_SUCCESS) {
      cleanup(&communicatedNode, routingTablePtr, &tcpRoutingServerSocketFd,
              &udpSocketFd, "Failed to receive and forward packets\n");
      return EXIT_FAILURE;
    }
  }

  // Free all memory, close all ports
  cleanup(&communicatedNode, routingTablePtr, &tcpRoutingServerSocketFd,
          &udpSocketFd, "\0");
  return EXIT_SUCCESS;
}

void cleanup(struct CommunicatedNode *communicatedNode,
             struct RoutingTable *routingTable,
             int *const tcpRoutingServerSocketFd, int *const udpSocketFd,
             const char *msg) {
  fprintf(stderr, "%s", msg);
  freeNeighborAddressesAndEdgeWeights(communicatedNode);
  freeRoutingTable(routingTable);
  if ((*udpSocketFd) != -1) {
    close(*udpSocketFd);
    *udpSocketFd = -1;
  }
  if ((*tcpRoutingServerSocketFd) != -1) {
    close(*tcpRoutingServerSocketFd);
    *tcpRoutingServerSocketFd = -1;
  }
}
