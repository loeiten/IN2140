#include <arpa/inet.h>
#include <errno.h>   // for errno
#include <libgen.h>  // for basename
#include <netinet/in.h>
#include <stdio.h>   // for fprintf, stderr, NULL
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_S...
#include <string.h>
#include <sys/socket.h>

#include "../../utils/include/common.h"          // for IndexToAddress
#include "../../utils/include/dynamic_memory.h"  // for allocateRoutingServer
#include "../include/dijkstra.h"                 // for dijkstra
#include "../include/receiver.h"                 // for checkAllNodesReceived
#include "../include/route.h"                    // for createRoutingTables

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr,
            "Usage: ./%s <P> <N>\n"
            "<P> the TCP port on which the server listens\n"
            "<N> The number of nodes",
            basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Set variables from the arguments
  int listenPort = atoi(argv[1]);
  int n = atoi(argv[2]);
  int maxEdges = (n * (n - 1) / 2);

  // Allocate memory
  struct ReceivedNode* receivedNodeArray = NULL;
  struct EdgeArray invalidEdgesArray;
  struct IndexToAddress indexToAddress;
  int** adjacencyMatrix = NULL;
  int* distanceArray = NULL;
  struct Route* routeArray = NULL;
  struct RoutingTable* routingTable = NULL;
  int success = allocateRoutingServer(
      &receivedNodeArray, &invalidEdgesArray, &indexToAddress, &adjacencyMatrix,
      &distanceArray, &routeArray, &routingTable, n, maxEdges);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory failed, exiting\n");
    return EXIT_FAILURE;
  }

  // FIXME: You are here
  // Don't know what order the message is coming in
  // Cannot do parallel, only one socket at the time

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
  int serverFd = socket(AF_LOCAL,      // We are communicating locally
                        SOCK_STREAM,   // Sequenced, reliable, two-way,
                                       // connection-based byte streams
                        IPPROTO_TCP);  // Use the TCP protocol
  if (serverFd == -1) {
    // NOTE: fprintf(stderr, "%s\n", strerror(errno)) is similar to perror(NULL)
    printf("Failed to create socket for the server.\nError %d: %s\n", errno,
           strerror(errno));
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
  success = bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if (success != 0) {
    printf("Binding server socket to address failed.\nError %d: %s\n", errno,
           strerror(errno));
    return EXIT_FAILURE;
  }

  // Activate listening on the socket
  // It puts the server socket in a passive mode, where it waits for the client
  // to approach the server to make a connection
  success = listen(serverFd,
                   SOMAXCONN);  // Max length for queue pending connection to
                                // the request socket
  if (success != 0) {
    printf("Listening to the server socket failed.\nError %d: %s\n", errno,
           strerror(errno));
    return EXIT_FAILURE;
  }
  // FIXME: The above can be refactored to getListenSocket

  // Accept a connection
  // Extract first connection request from the queue and return a new file
  // descriptor referring to that socket
  // FIXME: Can do a for-loop and Optionally: fork (do not share memory) or make
  // a posix-thread (does share memory)
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen;
  int newSocketFd = accept(
      serverFd,
      (struct sockaddr*)&clientAddr,  // Will be filled with the address of the
                                      // peer socket
      &clientAddrLen);  // Will be filled with the length of the peer address
  if (newSocketFd == -1) {
    printf("Accepting request failed.\nError %d: %s\n", errno, strerror(errno));
    return EXIT_FAILURE;
  }
  // FIXME: read - can you do this several times, read is a blocking call
  //        you can thus read
  //        1. address
  //        2. size of array
  //        3. neighboring array
  //        4. weight array
  // recv()/send() are specific to socket descriptors, whereas read()/write()
  // are universal functions working on all descriptors

  // FIXME: YOU ARE HERE

  // Await connection from N nodes (receives data about)
  // - Address
  // - Length of data
  // - Neighboring nodes
  // - Their weights

  // Fill the index to address struct
  for (int i = 0; i < n; ++i) {
    indexToAddress.map[i] = receivedNodeArray[i].address;
  }

  // Test if edge has been reported twice
  success = checkAllNodesReceived(receivedNodeArray, &invalidEdgesArray, n);
  if (success != EXIT_SUCCESS) {
    // FIXME: Close ports
    freeRoutingServer(&receivedNodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTable, n);
    fprintf(stderr, "The checking of all nodes failed, exiting\n");
    return EXIT_FAILURE;
  }

  // Create the adjacency matrix
  success = createAdjacencyMatrix(receivedNodeArray, &indexToAddress,
                                  &invalidEdgesArray, &adjacencyMatrix, n);
  if (success != EXIT_SUCCESS) {
    // FIXME: Clean-up: Close ports
    fprintf(stderr, "Could not create the adjacency matrix, exiting\n");
    return EXIT_FAILURE;
  }

  // Compute shortest path using Dijkstra
  // We overcome the problem with discarding qualifiers with a cast
  // We can do this as we are sure that the function is not modifying the data
  // If that was the case we would've ended up with undefined behaviour
  success = dijkstra(0, (const int* const*)adjacencyMatrix, distanceArray,
                     &routeArray, n);
  if (success != EXIT_SUCCESS) {
    // FIXME: Close and free stuff
    fprintf(
        stderr,
        "Could not find shortest path using Dijkstra's algorithm, exiting\n");
    return EXIT_FAILURE;
  }

  // Print the edges
  printEdges(distanceArray, routeArray, &indexToAddress, n);

  // Create the routing tables
  success = createRoutingTables(routeArray, &routingTable, n);
  if (success != EXIT_SUCCESS) {
    // FIXME: Close and free stuff
    fprintf(stderr, "Could not create the routing tables, exiting\n");
    return EXIT_FAILURE;
  }

  // Send the routing tables

  // Close ports

  // FIXME:
  // Free memory
  return EXIT_SUCCESS;
}
