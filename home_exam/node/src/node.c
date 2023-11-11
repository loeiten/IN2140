#include "../include/node.h"

#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr, NULL
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...
#include <string.h>  // for strcmp
#include <unistd.h>  // for close

#include "../../utils/include/common.h"          // for Node
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

  int basePort = atoi(argv[1]);
  int ownAddress = atoi(argv[2]);

  // Initialize variables to be cleaned
  int tcpRoutingServerSocketFd = -1;
  int udpSocketFd = -1;
  struct RoutingTable *routingTablePtr = NULL;

  // Capture all the neighbors
  // One argument for the file name, one for the port and one for own address
  int nNeighbors = argc - 3;
  struct Node node = {
      .tcpSocket = -1, .address = ownAddress, .nNeighbors = nNeighbors};
  int success =
      allocateNeighborAddressesAndEdgeWeights(&(node), nNeighbors, "node");
  if (success != EXIT_SUCCESS) {
    cleanUpNode(&node, routingTablePtr, &tcpRoutingServerSocketFd, &udpSocketFd,
                "\0");
    return EXIT_FAILURE;
  }
  success = parseNodes((const char *const *const)&(argv[3]), nNeighbors, &node);
  if (success != EXIT_SUCCESS) {
    freeNeighborAddressesAndEdgeWeights(&node);
    return EXIT_FAILURE;
  }

  // We now make a UDP socket we can both send to and receive from
  // The UDP port should be port + ownAddress
  int udpPort = basePort + ownAddress;
  success = getUDPSocket(&udpSocketFd, udpPort);
  if (success != EXIT_SUCCESS) {
    cleanUpNode(&node, routingTablePtr, &tcpRoutingServerSocketFd, &udpSocketFd,
                "Failed to get UDP socket, exiting\n");
    exit(-1);
  }

  // Open a TCP connection to the routing_server
  success = getTCPClientSocket(&tcpRoutingServerSocketFd, basePort);
  if (success != EXIT_SUCCESS) {
    cleanUpNode(&node, routingTablePtr, &tcpRoutingServerSocketFd, &udpSocketFd,
                "Failed to connect to the server, exiting\n");
    exit(-2);
  }

  // Send the edge information
  node.tcpSocket = tcpRoutingServerSocketFd;
  success = sendEdgeInformation(&node);
  if (success != EXIT_SUCCESS) {
    cleanUpNode(&node, routingTablePtr, &tcpRoutingServerSocketFd, &udpSocketFd,
                "Failed to send the edge information\n");
    return EXIT_FAILURE;
  }

  // Receive the routing table
  struct RoutingTable routingTable;
  routingTablePtr = &routingTable;
  success = receiveRoutingTable(tcpRoutingServerSocketFd, routingTablePtr);
  if (success != EXIT_SUCCESS) {
    cleanUpNode(&node, routingTablePtr, &tcpRoutingServerSocketFd, &udpSocketFd,
                "Failed to receive the edge information\n");
    return EXIT_FAILURE;
  }

  if (ownAddress == 1) {
    // Send packages to the other nodes
    success = prepareAndSendPackets(udpSocketFd, ownAddress, basePort,
                                    routingTablePtr);
    if (success != EXIT_SUCCESS) {
      cleanUpNode(&node, routingTablePtr, &tcpRoutingServerSocketFd,
                  &udpSocketFd, "Failed to prepare and send packets\n");
    }
  } else {
    // Await packet from the other nodes
    const char *msg = "";
    while (strcmp(msg, "QUIT") != 0) {
      success = receiveAndForwardPackets(udpSocketFd, ownAddress, basePort,
                                         routingTablePtr);
      if (success != EXIT_SUCCESS) {
        cleanUpNode(&node, routingTablePtr, &tcpRoutingServerSocketFd,
                    &udpSocketFd, "Failed to receive and forward packets\n");
        return EXIT_FAILURE;
      }
    }
  }

  // Free all memory, close all ports
  cleanUpNode(&node, routingTablePtr, &tcpRoutingServerSocketFd, &udpSocketFd,
              "\0");
  return EXIT_SUCCESS;
}

void cleanUpNode(struct Node *node, struct RoutingTable *routingTable,
                 int *const tcpRoutingServerSocketFd, int *const udpSocketFd,
                 const char *msg) {
  fprintf(stderr, "%s", msg);
  freeNeighborAddressesAndEdgeWeights(node);
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
