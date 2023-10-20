#include "../include/receiver.h"

#include <arpa/inet.h>   // for htons
#include <errno.h>       // for errno
#include <netinet/in.h>  // for sockaddr_in, INADDR_ANY
#include <stdio.h>       // for fprintf, stderr, fflush
#include <stdlib.h>      // for EXIT_FAILURE, EXIT_S...
#include <string.h>      // for strerror
#include <strings.h>     // for bzero
#include <sys/socket.h>  // for recv, MSG_WAITALL
#include <unistd.h>      // for close, NULL

#include "../../utils/include/common.h"          // for getIndexFromAddress
#include "../../utils/include/dynamic_memory.h"  // for freeIntArray, freeEd...

int checkAllNodesReceived(struct CommunicatedNode* communicatedNodeArray,
                          struct EdgeArray* invalidEdgesArray, int n) {
  // NOTE: In a undirected graph there can be at most n*(n-1)/2 edges
  int maxEdges = (n * (n - 1) / 2);
  // We start by allocating the memory needed for the EdgeCounterArray
  struct EdgeCounter* edgeCounterArrayArray = NULL;
  int success = allocateEdgeCounterArray(&edgeCounterArrayArray, maxEdges,
                                         "edgeCounterArray");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  struct EdgeCounterArray edgeCounterArray = {.maxEdges = maxEdges,
                                              .firstAvailablePosition = 0,
                                              .array = edgeCounterArrayArray};

  for (int addressIdx = 0; addressIdx < n; ++addressIdx) {
    int addressOfFirstIndex = communicatedNodeArray[addressIdx].address;
    for (int neighborIdx = 0;
         neighborIdx < communicatedNodeArray[addressIdx].nNeighbors;
         ++neighborIdx) {
      int neighborAddress =
          communicatedNodeArray[addressIdx].neighborAddresses[neighborIdx];
      int edgeWeight =
          communicatedNodeArray[addressIdx].edgeWeights[neighborIdx];
      int searchLowAddress = addressOfFirstIndex < neighborAddress
                                 ? addressOfFirstIndex
                                 : neighborAddress;
      int searchHighAddress = addressOfFirstIndex > neighborAddress
                                  ? addressOfFirstIndex
                                  : neighborAddress;

      // Check if there are any matches in the EdgeCounterArray
      int foundMatch = 0;
      for (int counterIdx = 0;
           counterIdx < edgeCounterArray.firstAvailablePosition; ++counterIdx) {
        if (counterIdx == (maxEdges - 1)) {
          fprintf(stderr,
                  "When investigating addressIdx %d with neighborIdx %d, the "
                  "counterIdx became %d, which is out of bounds as maxEdges is "
                  "%d\n",
                  addressIdx, neighborIdx, counterIdx, maxEdges);
          freeEdgeCounterArray(&(edgeCounterArray.array));
          return EXIT_FAILURE;
        }

        // See if there are any matches
        struct EdgeCounter* curEdgeCounter =
            &(edgeCounterArray.array[counterIdx]);
        if (curEdgeCounter->edge.lowNodeAddress == searchLowAddress) {
          // Here we have a match, we need to check if the high address is also
          // matching
          if (curEdgeCounter->edge.highNodeAddress == searchHighAddress) {
            // Ok, we are now matching both on the low address and the high
            // address. This means that this edge has been observed before.
            ++(curEdgeCounter->encounters);
            foundMatch = 1;

            success = checkIfEdgeIsValid(searchLowAddress, searchHighAddress,
                                         addressOfFirstIndex, edgeWeight,
                                         curEdgeCounter, invalidEdgesArray);
            if (success != EXIT_SUCCESS) {
              freeEdgeCounterArray(&(edgeCounterArray.array));
              return EXIT_FAILURE;
            }
            break;
          }
        }
      }

      if (foundMatch == 0) {
        success = addEdgeToEdgeCounterArray(searchLowAddress, searchHighAddress,
                                            addressOfFirstIndex, edgeWeight,
                                            &edgeCounterArray);
        if (success != EXIT_SUCCESS) {
          freeEdgeCounterArray(&(edgeCounterArray.array));
          return EXIT_FAILURE;
        }
      }
    }
  }

  success = checkDualReport(&edgeCounterArray, invalidEdgesArray);
  if (success != EXIT_SUCCESS) {
    freeEdgeCounterArray(&(edgeCounterArray.array));
    return EXIT_FAILURE;
  }

  freeEdgeCounterArray(&(edgeCounterArray.array));
  return EXIT_SUCCESS;
}

int checkIfEdgeIsValid(const int lowAddress, const int highAddress,
                       const int addressOfFirstIndex, const int edgeWeight,
                       const struct EdgeCounter* const edgeCounter,
                       struct EdgeArray* invalidEdgesArray) {
  // To validate that the match is valid we need to check that
  // - It has not been reported more than twice
  // - The addressOfFirstIndex is different
  // - That the reported weight is the same (as the graph we are
  //   working with are undirected)
  if (edgeCounter->encounters > 2) {
    // If this is an invalid edge we don't want to report it twice,
    // hence we check that it's not reported before
    int success = addInvalidEdge(lowAddress, highAddress, invalidEdgesArray,
                                 "reported more than twice");
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // No need to do more processing of this edge
    return EXIT_SUCCESS;
  }
  if (edgeCounter->addressOfFirstIndex == addressOfFirstIndex) {
    int success = addInvalidEdge(lowAddress, highAddress, invalidEdgesArray,
                                 "reported in the exact same way before");
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // No need to do more processing of this edge
    return EXIT_SUCCESS;
  }
  if (edgeCounter->reportedWeight != edgeWeight) {
    int success = addInvalidEdge(lowAddress, highAddress, invalidEdgesArray,
                                 "reported with different weights before");
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // No need to do more processing of this edge
    return EXIT_SUCCESS;
  }
  // Everything is fine
  return EXIT_SUCCESS;
}

int addInvalidEdge(const int lowAddress, const int highAddress,
                   struct EdgeArray* invalidEdgesArray, const char* reason) {
  // We need to check that this edge has not been reported as
  // invalid before
  if (isEdgePresent(lowAddress, highAddress, invalidEdgesArray) == 1) {
    return EXIT_SUCCESS;
  }
  // Furthermore we check first that we are not out of bounds
  if (invalidEdgesArray->firstAvailablePosition ==
      invalidEdgesArray->maxEdges) {
    fprintf(stderr,
            "Wanted to add that edge %d <-> %d was out of bounds as it has "
            "been %s, but the first available position in invalidEdgesArray "
            "equals the maxEdges which is %d and therefore out of bounds\n",
            lowAddress, highAddress, reason, invalidEdgesArray->maxEdges);
    return EXIT_FAILURE;
  }
  // Finally we print the warning
  printf(
      "WARNING: The edge %d <-> %d has been %s. This may be due to data "
      "corruption. Will therefore mark the edge invalid\n",
      lowAddress, highAddress, reason);
  // Force a flush in order to get the order correct when piping to file
  fflush(stdout);
  invalidEdgesArray->array[invalidEdgesArray->firstAvailablePosition]
      .lowNodeAddress = lowAddress;
  invalidEdgesArray->array[invalidEdgesArray->firstAvailablePosition]
      .highNodeAddress = highAddress;
  ++(invalidEdgesArray->firstAvailablePosition);
  return EXIT_SUCCESS;
}

int isEdgePresent(const int lowAddress, const int highAddress,
                  const struct EdgeArray* const edgeArray) {
  for (int edgeIdx = 0; edgeIdx < edgeArray->firstAvailablePosition;
       ++edgeIdx) {
    if ((edgeArray->array[edgeIdx].lowNodeAddress == lowAddress) &&
        (edgeArray->array[edgeIdx].highNodeAddress == highAddress)) {
      return 1;
    }
  }
  return 0;
}

int addEdgeToEdgeCounterArray(const int lowAddress, const int highAddress,
                              const int addressOfFirstIndex,
                              const int edgeWeight,
                              struct EdgeCounterArray* const edgeCounterArray) {
  // Check that we are not out of bounds
  if (edgeCounterArray->firstAvailablePosition == edgeCounterArray->maxEdges) {
    fprintf(stderr,
            "When adding edge %d <-> %d to the edge counter array the "
            "firstAvailablePosition %d was out of bounds as maxEdges was "
            "%d.\n",
            lowAddress, highAddress, edgeCounterArray->firstAvailablePosition,
            edgeCounterArray->maxEdges);
    return EXIT_FAILURE;
  }

  // There are no matches in the edge counter array, which means that this
  // is the first time we are observing this edge
  // Hence we add it to the edge counter array
  int idx = edgeCounterArray->firstAvailablePosition;
  edgeCounterArray->array[idx].addressOfFirstIndex = addressOfFirstIndex;
  edgeCounterArray->array[idx].edge.lowNodeAddress = lowAddress;
  edgeCounterArray->array[idx].edge.highNodeAddress = highAddress;
  edgeCounterArray->array[idx].encounters = 1;
  edgeCounterArray->array[idx].reportedWeight = edgeWeight;

  // Increment the first available position
  ++(edgeCounterArray->firstAvailablePosition);
  return EXIT_SUCCESS;
}

int checkDualReport(const struct EdgeCounterArray* const edgeCounterArray,
                    struct EdgeArray* invalidEdgesArray) {
  for (int edgeIdx = 0; edgeIdx < edgeCounterArray->firstAvailablePosition;
       ++edgeIdx) {
    if (edgeCounterArray->array[edgeIdx].encounters < 2) {
      int success =
          addInvalidEdge(edgeCounterArray->array[edgeIdx].edge.lowNodeAddress,
                         edgeCounterArray->array[edgeIdx].edge.highNodeAddress,
                         invalidEdgesArray, "reported less than twice");
      if (success != EXIT_SUCCESS) {
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}

int createAdjacencyMatrix(
    const struct CommunicatedNode* const communicatedNodeArray,
    const struct IndexToAddress* const indexToAddress,
    const struct EdgeArray* const invalidEdgesArray, int*** adjacencyMatrix,
    const int n) {
  // Allocate memory to the adjacency matrix
  // Note that this allocation is zero initalized
  int success = allocateIntMatrix(adjacencyMatrix, n, "adjacencyMatrix");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Loop through the edge array and fill the adjacencyMatrix
  // Note that this is inefficient as we are looping through all edges twice
  // (given that they are valid)
  for (int nodeIdx = 0; nodeIdx < n; ++nodeIdx) {
    // Loop through the neighbors
    int nNeighbors = communicatedNodeArray[nodeIdx].nNeighbors;
    int firstIndex;
    success = getIndexFromAddress(communicatedNodeArray[nodeIdx].address,
                                  indexToAddress, &firstIndex);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    for (int neighborIdx = 0; neighborIdx < nNeighbors; ++neighborIdx) {
      int secondIndex;
      success = getIndexFromAddress(
          communicatedNodeArray[nodeIdx].neighborAddresses[neighborIdx],
          indexToAddress, &secondIndex);
      if (success != EXIT_SUCCESS) {
        return EXIT_FAILURE;
      }
      // Set the weight
      (*adjacencyMatrix)[firstIndex][secondIndex] =
          communicatedNodeArray[nodeIdx].edgeWeights[neighborIdx];
    }
  }

  // Remove all edges with invalid edges
  for (int edgeIdx = 0; edgeIdx < invalidEdgesArray->firstAvailablePosition;
       ++edgeIdx) {
    // Get the first and the second index
    int firstIndex;
    success =
        getIndexFromAddress(invalidEdgesArray->array[edgeIdx].lowNodeAddress,
                            indexToAddress, &firstIndex);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    int secondIndex;
    success =
        getIndexFromAddress(invalidEdgesArray->array[edgeIdx].highNodeAddress,
                            indexToAddress, &secondIndex);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    // Nullify the weight for one part of the edge
    (*adjacencyMatrix)[firstIndex][secondIndex] = 0;
    // Nullify the weight for the other part of the edge
    (*adjacencyMatrix)[secondIndex][firstIndex] = 0;
  }

  return EXIT_SUCCESS;
}

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
