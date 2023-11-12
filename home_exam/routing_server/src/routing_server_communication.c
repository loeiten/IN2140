#include "../include/routing_server_communication.h"

#include <arpa/inet.h>   // for htons
#include <errno.h>       // for errno
#include <netinet/in.h>  // for sockaddr_in, INADDR_...
#include <stdio.h>       // for fprintf, stderr, NULL
#include <stdlib.h>      // for EXIT_FAILURE, free
#include <string.h>      // for strerror
#include <strings.h>     // for bzero
#include <sys/socket.h>  // for recv, send, MSG_WAITALL

#include "../../utils/include/common.h"          // for RoutingTable, Node
#include "../../utils/include/dynamic_memory.h"  // for freeNeighborAddresse...
#include "../include/route.h"                    // for RoutingTableArray

int getTCPServerSocket(int* const listenSocket, const int basePort) {
  // Abbreviations:
  // ARPA - Address and Routing Parameter Area
  // AF - Address family
  //      Address families are categorization of network addresses and protocols
  //      Examples: AF_INET (AddressFamily - InterNET) for IPv4
  //      Examples: AF_INET6 (AddressFamily - InterNET) for IPv6
  //      Examples: AF_LOCAL (AddressFamily - Local) for local communication
  //      See https://man7.org/linux/man-pages/man7/address_families.7.html
  // IN - Internet
  // SIN - Shorthand for sockaddr_in
  // hton - Host to network
  // ntoh - Network to host

  // https://man7.org/linux/man-pages/man2/socket.2.html
  // Create the socket file descriptor for the server
  // This is an endpoint for the communication
  // Note that this is not the socket that will send and receive messages
  (*listenSocket) = socket(AF_LOCAL,      // We are communicating locally
                           SOCK_STREAM,   // Sequenced, reliable, two-way,
                                          // connection-based byte streams (TCP)
                           IPPROTO_TCP);  // Use the TCP protocol
                                          // (redundant because of SOCK_STREAM)
  if ((*listenSocket) == -1) {
    // NOTE: fprintf(stderr, "%s\n", strerror(errno)) is similar to perror(NULL)
    fprintf(stderr, "Failed to create socket for the server.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  }

  // Create an address, which we can bind to the socket fd
  struct sockaddr_in serverAddr;
  // For safety measure: Erase data by writing 0's to the memory location
  // https://stackoverflow.com/q/36086642/2786884
  bzero((void*)&serverAddr, sizeof(serverAddr));

  // Bind the socket to an address
  // When a process wants to receive new incoming packets or connections, it
  // should bind a socket to a local interface address using bind.
  // In this case, only one IP socket may be bound to any given local (address,
  // port) pair. When INADDR_ANY is specified in the bind call, the socket will
  // be bound to all local interfaces.
  serverAddr.sin_family = AF_LOCAL;  // We are still communicating locally
  serverAddr.sin_addr.s_addr =
      INADDR_LOOPBACK;  // Only local addresses are accepted (INADDR_ANY would
                        // accept connection to any addresses)
  serverAddr.sin_port = htons(basePort);  // The port in network byte order

  // Bind assigns the address specified by sockaddr_in to a socket
  // Traditionally, this operation is called "assigning a name to a socket".
  int success =
      bind((*listenSocket), (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if (success != 0) {
    fprintf(stderr, "Binding server socket to address failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  }

  // Activate listening on the socket
  // It puts the server socket in a passive mode, where it waits for the client
  // to approach the server to make a connection
  success = listen((*listenSocket),
                   SOMAXCONN);  // Max length for queue pending connection to
                                // the request socket
  if (success != 0) {
    fprintf(stderr, "Listening to the server socket failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int populateNodeArray(const int listenSocket, struct Node* nodeArray,
                      const int n) {
  // NOTE: For large nodes, we could consider to fork (create a new process
  //       which does not share memory) or make a posix-thread (which does share
  //       memory)
  for (int i = 0; i < n; ++i) {
    // Accept a connection
    // Extract first connection request from the queue and return a new file
    // descriptor referring to that socket
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    // Accept is blocking
    int newSocketFd = accept(
        listenSocket,
        (struct sockaddr*)&clientAddr,  // Will be filled with the address of
                                        // the peer socket
        &clientAddrLen);  // Will be filled with the length of the peer address
    if (newSocketFd == -1) {
      fprintf(stderr, "Accepting request failed.\nError %d: %s\n", errno,
              strerror(errno));
      return EXIT_FAILURE;
    }

    nodeArray[i].tcpSocket = newSocketFd;

    // NOTE: recv()/send() are specific to socket descriptors, whereas
    //       read()/write() are universal functions working on all descriptors
    //       Both of them are blocking by default
    // Receive the address
    ssize_t nBytes = sizeof(int);
    ssize_t bytesReceived =
        recv(newSocketFd, &(nodeArray[i].address), nBytes, MSG_WAITALL);
    if (bytesReceived == -1) {
      fprintf(stderr, "Receiving nodeArray[%d].address failed.\nError %d: %s\n",
              i, errno, strerror(errno));
      return EXIT_FAILURE;
    } else if (bytesReceived != nBytes) {
      fprintf(stderr,
              "Received less bytes than expected for "
              "nodeArray[%d].address\n",
              i);
      return EXIT_FAILURE;
    }

    // Receive the size of the array
    bytesReceived =
        recv(newSocketFd, &(nodeArray[i].nNeighbors), nBytes, MSG_WAITALL);
    if (bytesReceived == -1) {
      fprintf(stderr,
              "Receiving nodeArray[%d].nNeighbors failed.\nError "
              "%d: %s\n",
              i, errno, strerror(errno));
      return EXIT_FAILURE;
    } else if (bytesReceived != nBytes) {
      fprintf(stderr,
              "Received less bytes than expected for "
              "nodeArray[%d].nNeighbors\n",
              i);
      return EXIT_FAILURE;
    }

    // Allocate memory to the addresses and weights
    int success = allocateNeighborAddressesAndEdgeWeights(
        &(nodeArray[i]), nodeArray[i].nNeighbors, "nodeArray[i]");
    if (success != EXIT_SUCCESS) {
      freeNeighborAddressesAndEdgeWeights(&(nodeArray[i]));
      return EXIT_FAILURE;
    }

    // Receive the neighbor array
    nBytes *= nodeArray[i].nNeighbors;
    bytesReceived = recv(newSocketFd, &(nodeArray[i].neighborAddresses), nBytes,
                         MSG_WAITALL);
    if (bytesReceived == -1) {
      fprintf(stderr,
              "Receiving nodeArray[%d].neighborAddresses "
              "failed.\nError "
              "%d: %s\n",
              i, errno, strerror(errno));
      freeNeighborAddressesAndEdgeWeights(&(nodeArray[i]));
      return EXIT_FAILURE;
    } else if (bytesReceived != nBytes) {
      fprintf(stderr,
              "Received less bytes than expected for "
              "nodeArray[%d].neighborAddresses\n",
              i);
      freeNeighborAddressesAndEdgeWeights(&(nodeArray[i]));
      return EXIT_FAILURE;
    }

    // Receive the weight array
    bytesReceived =
        recv(newSocketFd, &(nodeArray[i].edgeWeights), nBytes, MSG_WAITALL);
    if (bytesReceived == -1) {
      fprintf(stderr,
              "Receiving nodeArray[%d].edgeWeights failed.\nError "
              "%d: %s\n",
              i, errno, strerror(errno));
      freeNeighborAddressesAndEdgeWeights(&(nodeArray[i]));
      return EXIT_FAILURE;
    } else if (bytesReceived != nBytes) {
      fprintf(stderr,
              "Received less bytes than expected for "
              "nodeArray[%d].edgeWeights\n",
              i);
      freeNeighborAddressesAndEdgeWeights(&(nodeArray[i]));
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int sendRoutingTables(const struct Node* const nodeArray,
                      const struct RoutingTableArray* const routingTableArray,
                      const struct IndexToAddress* const indexToAddress) {
  struct RoutingTable addressRoutingTable;
  addressRoutingTable.routingTableRows = NULL;

  // NOTE: About what the index i actually means
  //       nodeArray is created by populateNodeArray
  //           We do not know what order the nodes will come in
  //       nodeArray is used by createAdjacencyMatrix, which creates
  //       adjacencyMatrix
  //           createAdjacencyMatrix uses indexToArray, as the adjacencyMatrix
  //           operates on indices
  //       routeArray is created by dijkstra which uses adjacencyMatrix
  //           routeArray also operates on indices
  //       routingTableArray is created by createRoutingTableArray using
  //       routeArray
  //           The routingTableArray also operates on indices
  //       Hence both the nodeArray and routingTableArray operates on indices
  //       This means that we need to translate the nextHop and destination to
  //       addresses before sending them

  for (int i = 0; routingTableArray->n; ++i) {
    int success =
        translateTableFromIdxToAddress(&(routingTableArray->routingTables[i]),
                                       indexToAddress, &addressRoutingTable);
    if (success != EXIT_SUCCESS) {
      fprintf(stderr,
              "Failed to translate routing table from idx to address for index "
              "%d \n",
              i);
      if (addressRoutingTable.routingTableRows != NULL) {
        free(addressRoutingTable.routingTableRows);
        addressRoutingTable.routingTableRows = NULL;
      }
    }

    ssize_t nBytes = sizeof(int);
    ssize_t bytesSent =
        send(nodeArray[i].tcpSocket, &(addressRoutingTable.nRows), nBytes, 0);
    if (bytesSent == -1) {
      fprintf(stderr,
              "Sending addressRoutingTable.nRows failed.\nError %d: %s\n",
              errno, strerror(errno));
      free(addressRoutingTable.routingTableRows);
      addressRoutingTable.routingTableRows = NULL;
      return EXIT_FAILURE;
    } else if (bytesSent != nBytes) {
      fprintf(stderr,
              "Sent less bytes than expected for addressRoutingTable.nRows\n");
      free(addressRoutingTable.routingTableRows);
      addressRoutingTable.routingTableRows = NULL;
      return EXIT_FAILURE;
    }

    nBytes = addressRoutingTable.nRows * sizeof(int);
    bytesSent = send(nodeArray[i].tcpSocket,
                     &(addressRoutingTable.routingTableRows), nBytes, 0);
    if (bytesSent == -1) {
      fprintf(stderr,
              "Sending addressRoutingTable.routingTableRows failed.\nError %d: "
              "%s\n",
              errno, strerror(errno));
      free(addressRoutingTable.routingTableRows);
      addressRoutingTable.routingTableRows = NULL;
      return EXIT_FAILURE;
    } else if (bytesSent != nBytes) {
      fprintf(stderr,
              "Sent less bytes than expected for "
              "addressRoutingTable.routingTableRows\n");
      free(addressRoutingTable.routingTableRows);
      addressRoutingTable.routingTableRows = NULL;
      return EXIT_FAILURE;
    }
  }
  // Free the temporary addressRouting
  free(addressRoutingTable.routingTableRows);
  addressRoutingTable.routingTableRows = NULL;
  return EXIT_SUCCESS;
}

int translateTableFromIdxToAddress(
    const struct RoutingTable* const idxRoutingTable,
    const struct IndexToAddress* const indexToAddress,
    struct RoutingTable* addressRoutingTable) {
  addressRoutingTable->nRows = idxRoutingTable->nRows;
  addressRoutingTable->routingTableRows =
      realloc(addressRoutingTable->routingTableRows,
              addressRoutingTable->nRows * sizeof(struct RoutingTableRows));
  if (addressRoutingTable->routingTableRows == NULL) {
    perror("Failed to realloc addressRoutingTable.routingTableRows: ");
    return EXIT_FAILURE;
  }

  for (int row = 0; row < addressRoutingTable->nRows; ++row) {
    addressRoutingTable->routingTableRows[row].destination =
        indexToAddress->map[idxRoutingTable->routingTableRows[row].destination];
    addressRoutingTable->routingTableRows[row].nextHop =
        indexToAddress->map[idxRoutingTable->routingTableRows[row].nextHop];
  }

  return EXIT_SUCCESS;
}
