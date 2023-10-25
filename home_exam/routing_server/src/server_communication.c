#include "../include/server_communication.h"

#include <arpa/inet.h>   // for htons
#include <errno.h>       // for errno
#include <netinet/in.h>  // for sockaddr_in, INADDR_ANY
#include <stdio.h>       // for fprintf, stderr, ssi...
#include <stdlib.h>      // for EXIT_FAILURE, EXIT_S...
#include <string.h>      // for strerror
#include <strings.h>     // for bzero
#include <sys/socket.h>  // for recv, MSG_WAITALL
#include <unistd.h>      // for close

#include "../../utils/include/common.h"          // for CommunicatedNode
#include "../../utils/include/dynamic_memory.h"  // for freeNeighborAddresse...

int getListenSocket(const int listenPort, int* const listenSocket) {
  // Abbreviations:
  // AF - Address family
  //      Address families are categorization of network addresses and protocols
  //      Examples: AF_INET (AddressFamily - InterNET) for IPv4
  //      Examples: AF_INET6 (AddressFamily - InterNET) for IPv6
  //      Examples: AF_LOCAL (AddressFamily - Local) for local communication
  //      See https://man7.org/linux/man-pages/man7/address_families.7.html
  // IN - Internet
  // SIN - Shorthand for sockaddr_in

  // https://man7.org/linux/man-pages/man2/socket.2.html
  // Create the socket file descriptor for the server
  // This is an endpoint for the communication
  // Note that this is not the socket that will
  (*listenSocket) = socket(AF_LOCAL,      // We are communicating locally
                           SOCK_STREAM,   // Sequenced, reliable, two-way,
                                          // connection-based byte streams
                           IPPROTO_TCP);  // Use the TCP protocol
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
  serverAddr.sin_addr.s_addr = INADDR_ANY;  // All addresses are accepted
  serverAddr.sin_port = htons(listenPort);  // The port in network byte order

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

int populateCommunicatedNodeArray(
    const int listenSocket, struct CommunicatedNode* communicatedNodeArray,
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

    // NOTE: recv()/send() are specific to socket descriptors, whereas
    //       read()/write() are universal functions working on all descriptors
    //       Both of them are blocking by default
    // Receive the address
    ssize_t nBytes = 1;
    ssize_t bytesReceived = recv(
        newSocketFd, &(communicatedNodeArray[i].address), nBytes, MSG_WAITALL);
    if (bytesReceived == -1) {
      fprintf(
          stderr,
          "Receiving communicatedNodeArray[%d].address failed.\nError %d: %s\n",
          i, errno, strerror(errno));
      close(newSocketFd);
      return EXIT_FAILURE;
    } else if (bytesReceived != nBytes) {
      fprintf(stderr,
              "Received less bytes than expected for "
              "communicatedNodeArray[%d].address\n",
              i);
      return EXIT_FAILURE;
    }

    // Receive the size of the array
    bytesReceived = recv(newSocketFd, &(communicatedNodeArray[i].nNeighbors),
                         nBytes, MSG_WAITALL);
    if (bytesReceived == -1) {
      fprintf(stderr,
              "Receiving communicatedNodeArray[%d].nNeighbors failed.\nError "
              "%d: %s\n",
              i, errno, strerror(errno));
      close(newSocketFd);
      return EXIT_FAILURE;
    } else if (bytesReceived != nBytes) {
      fprintf(stderr,
              "Received less bytes than expected for "
              "communicatedNodeArray[%d].nNeighbors\n",
              i);
      close(newSocketFd);
      return EXIT_FAILURE;
    }

    // Allocate memory to the addresses and weights
    int success = allocateNeighborAddressesAndEdgeWeights(
        &(communicatedNodeArray[i]), communicatedNodeArray[i].nNeighbors,
        "communicatedNodeArray[i]");
    if (success != EXIT_SUCCESS) {
      freeNeighborAddressesAndEdgeWeights(&(communicatedNodeArray[i]));
      close(newSocketFd);
      return EXIT_FAILURE;
    }

    // Receive the neighbor array
    nBytes *= communicatedNodeArray[i].nNeighbors;
    bytesReceived =
        recv(newSocketFd, &(communicatedNodeArray[i].neighborAddresses), nBytes,
             MSG_WAITALL);
    if (bytesReceived == -1) {
      fprintf(stderr,
              "Receiving communicatedNodeArray[%d].neighborAddresses "
              "failed.\nError "
              "%d: %s\n",
              i, errno, strerror(errno));
      freeNeighborAddressesAndEdgeWeights(&(communicatedNodeArray[i]));
      close(newSocketFd);
      return EXIT_FAILURE;
    } else if (bytesReceived != nBytes) {
      fprintf(stderr,
              "Received less bytes than expected for "
              "communicatedNodeArray[%d].neighborAddresses\n",
              i);
      freeNeighborAddressesAndEdgeWeights(&(communicatedNodeArray[i]));
      close(newSocketFd);
      return EXIT_FAILURE;
    }

    // Receive the weight array
    bytesReceived = recv(newSocketFd, &(communicatedNodeArray[i].edgeWeights),
                         nBytes, MSG_WAITALL);
    if (bytesReceived == -1) {
      fprintf(stderr,
              "Receiving communicatedNodeArray[%d].edgeWeights failed.\nError "
              "%d: %s\n",
              i, errno, strerror(errno));
      freeNeighborAddressesAndEdgeWeights(&(communicatedNodeArray[i]));
      close(newSocketFd);
      return EXIT_FAILURE;
    } else if (bytesReceived != nBytes) {
      fprintf(stderr,
              "Received less bytes than expected for "
              "communicatedNodeArray[%d].edgeWeights\n",
              i);
      freeNeighborAddressesAndEdgeWeights(&(communicatedNodeArray[i]));
      close(newSocketFd);
      return EXIT_FAILURE;
    }
    close(newSocketFd);
  }
  return EXIT_SUCCESS;
}
