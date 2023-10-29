#include "../include/node_communication.h"

#include <arpa/inet.h>   // for htons
#include <errno.h>       // for errno
#include <netinet/in.h>  // for sockaddr_in, INADDR_LOOPBACK
#include <stdio.h>       // for fprintf, stderr, ssize_t
#include <stdlib.h>      // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>      // for strerror
#include <strings.h>     // for bzero
#include <sys/socket.h>  // for send, socket, AF_LOCAL, bind

#include "../../utils/include/common.h"          // for CommunicatedNode
#include "../../utils/include/dynamic_memory.h"  // for CommunicatedNode

int getUDPSocket(int* const clientSocket, const int clientPort) {
  // A great introduction to socket programming can be found at:
  // https://users.cs.jmu.edu/bernstdh/web/common/lectures/summary_unix_udp.php
  (*clientSocket) = socket(AF_LOCAL,    // We are communicating locally
                           SOCK_DGRAM,  // Datagrams (connectionless, unreliable
                                        // messages of a fixed maximum length)
                           IPPROTO_UDP);  // Use the UDP protocol
                                          // (redundant because of SOCK_DGRAM)
  if ((*clientSocket) == -1) {
    // NOTE: fprintf(stderr, "%s\n", strerror(errno)) is similar to perror(NULL)
    fprintf(stderr, "Failed to create UPD socket.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  }

  // Create an address, which we can bind to the socket fd
  struct sockaddr_in addr;
  // For safety measure: Erase data by writing 0's to the memory location
  // https://stackoverflow.com/q/36086642/2786884
  bzero((void*)&addr, sizeof(addr));

  // Bind the socket to an address
  // NOTE: Only the server needs to bind to a port because a client needs a
  //       fixed port to send to.
  //       A client needn't bind() at all:
  //       an automatic bind() will take place on the first
  //       send()/sendto()/recv()/recvfrom() using a system-assigned local port
  //       number.
  //       https://stackoverflow.com/a/6194518/2786884
  // In this assignment the socket will be used to receive from and sending to
  // other nodes
  addr.sin_family = AF_LOCAL;  // We are still communicating locally
  addr.sin_addr.s_addr =
      INADDR_LOOPBACK;  // Only local addresses are accepted (INADDR_ANY would
                        // accept connection to any addresses)
  addr.sin_port = htons(clientPort);  // The port in network byte order

  // Bind assigns the address specified by sockaddr_in to a socket
  // Traditionally, this operation is called "assigning a name to a socket".
  int success = bind((*clientSocket), (struct sockaddr*)&addr, sizeof(addr));
  if (success != 0) {
    fprintf(stderr, "Binding client socket to address failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  }

  // NOTE: We are not listening as UDP is connectionless
  return EXIT_SUCCESS;
}

int getTCPClientSocket(int* const clientSocket, const int serverPort) {
  // https://man7.org/linux/man-pages/man2/socket.2.html
  // Create the socket file descriptor for the server
  // This is an endpoint for the communication
  (*clientSocket) = socket(AF_LOCAL,      // We are communicating locally
                           SOCK_STREAM,   // Sequenced, reliable, two-way,
                                          // connection-based byte streams (TCP)
                           IPPROTO_TCP);  // Use the TCP protocol
                                          // (redundant because of SOCK_STREAM)
  if ((*clientSocket) == -1) {
    // NOTE: fprintf(stderr, "%s\n", strerror(errno)) is similar to perror(NULL)
    fprintf(stderr, "Failed to create socket for the client.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  }

  // Create an address, which we can bind to the socket fd
  struct sockaddr_in serverAddr;
  // For safety measure: Erase data by writing 0's to the memory location
  // https://stackoverflow.com/q/36086642/2786884
  bzero((void*)&serverAddr, sizeof(serverAddr));

  // Bind the socket to an address
  serverAddr.sin_family = AF_LOCAL;  // We are still communicating locally
  serverAddr.sin_addr.s_addr =
      INADDR_LOOPBACK;  // Equivalent to inet_addr("127.0.0.1")
  serverAddr.sin_port = htons(serverPort);  // The port in network byte order

  // Bind assigns the address specified by sockaddr_in to a socket
  // Traditionally, this operation is called "assigning a name to a socket".
  int success = connect((*clientSocket), (struct sockaddr*)&serverAddr,
                        sizeof(serverAddr));
  if (success != 0) {
    fprintf(stderr, "Connection to server failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int sendEdgeInformation(const int tcpRoutingServerSocketFd,
                        struct CommunicatedNode* const communicatedNode) {
  // Send own address
  ssize_t nBytes = sizeof(int);
  ssize_t bytesSent =
      send(tcpRoutingServerSocketFd, &(communicatedNode->address), nBytes, 0);
  if (bytesSent == -1) {
    fprintf(stderr, "Sending communicatedNode->address failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(stderr,
            "Sent less bytes than expected for communicatedNode->address\n");
    return EXIT_FAILURE;
  }

  // Send the size of the array
  bytesSent = send(tcpRoutingServerSocketFd, &(communicatedNode->nNeighbors),
                   nBytes, 0);
  if (bytesSent == -1) {
    fprintf(stderr,
            "Sending communicatedNode->nNeighbors failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(stderr,
            "Sent less bytes than expected for communicatedNode->nNeighbors\n");
    return EXIT_FAILURE;
  }

  // Send the neighbor address array
  nBytes *= communicatedNode->nNeighbors;
  bytesSent = send(tcpRoutingServerSocketFd,
                   &(communicatedNode->neighborAddresses), nBytes, 0);
  if (bytesSent == -1) {
    fprintf(
        stderr,
        "Sending communicatedNode->neighborAddresses failed.\nError %d: %s\n",
        errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(stderr,
            "Sent less bytes than expected for "
            "communicatedNode->neighborAddresses\n");
    return EXIT_FAILURE;
  }

  // Send the neighbor weight array
  bytesSent = send(tcpRoutingServerSocketFd, &(communicatedNode->edgeWeights),
                   nBytes, 0);
  if (bytesSent == -1) {
    fprintf(stderr,
            "Sending communicatedNode->edgeWeights failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(
        stderr,
        "Sent less bytes than expected for communicatedNode->edgeWeights\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int receiveRoutingTable(const int tcpRoutingServerSocketFd,
                        struct RoutingTable* routingTable,
                        int* const tableRows) {
  // NOTE: recv()/send() are specific to socket descriptors, whereas
  //       read()/write() are universal functions working on all descriptors
  //       Both of them are blocking by default
  // Receive the number of rows
  ssize_t nBytes = sizeof(int);
  ssize_t bytesReceived =
      recv(tcpRoutingServerSocketFd, &(routingTable->n), nBytes, MSG_WAITALL);
  if (bytesReceived == -1) {
    fprintf(stderr, "Receiving routingTable->n failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesReceived != nBytes) {
    fprintf(stderr, "Received less bytes than expected for routingTable->n\n");
    return EXIT_FAILURE;
  }
  (*tableRows) = routingTable->n;

  // Allocate memory to the routing table
  int success =
      allocateRoutingTable(&routingTable, *tableRows, "node routing table");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Receive the table
  nBytes = (*tableRows) * sizeof(struct DestinationNextPair);
  bytesReceived = recv(tcpRoutingServerSocketFd, &(routingTable->table), nBytes,
                       MSG_WAITALL);
  if (bytesReceived == -1) {
    fprintf(stderr, "Receiving routingTable->table failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesReceived != nBytes) {
    fprintf(stderr,
            "Received less bytes than expected for routingTable->table\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
