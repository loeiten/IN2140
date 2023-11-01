#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr, NULL
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_S...
#include <unistd.h>  // for close

#include "../../utils/include/common.h"          // for CommunicatedNode
#include "../../utils/include/dynamic_memory.h"  // for freeRoutingServer
#include "../include/adjacency_matrix.h"         // for createAdjacencyMatrix
#include "../include/dijkstra.h"                 // for dijkstra
#include "../include/route.h"                    // for createRoutingTables
#include "../include/server_communication.h"     // for getTCPServerSocket
#include "../include/validation.h"               // for checkAllNodesReceived

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
  struct CommunicatedNode* communicatedNodeArray = NULL;
  struct EdgeArray invalidEdgesArray;
  struct IndexToAddress indexToAddress;
  int** adjacencyMatrix = NULL;
  int* distanceArray = NULL;
  struct Route* routeArray = NULL;
  struct RoutingTable* routingTableArray = NULL;
  int success =
      allocateRoutingServer(&communicatedNodeArray, &invalidEdgesArray,
                            &indexToAddress, &adjacencyMatrix, &distanceArray,
                            &routeArray, &routingTableArray, n, maxEdges);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory failed, exiting\n");
    freeRoutingServer(&communicatedNodeArray, &invalidEdgesArray,
                      &indexToAddress, &adjacencyMatrix, &distanceArray,
                      &routeArray, &routingTableArray, n);
    return EXIT_FAILURE;
  }

  int listenSocket = -1;
  success = getTCPServerSocket(&listenSocket, listenPort);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Obtaining the listen socket failed, exiting\n");
    freeRoutingServer(&communicatedNodeArray, &invalidEdgesArray,
                      &indexToAddress, &adjacencyMatrix, &distanceArray,
                      &routeArray, &routingTableArray, n);
    if (listenSocket != -1) {
      close(listenSocket);
      listenSocket = -1;
    }
    return EXIT_FAILURE;
  }

  success =
      populateCommunicatedNodeArray(listenSocket, communicatedNodeArray, n);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Populating the communicated node array failed, exiting\n");
    freeRoutingServer(&communicatedNodeArray, &invalidEdgesArray,
                      &indexToAddress, &adjacencyMatrix, &distanceArray,
                      &routeArray, &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Fill the index to address struct
  for (int i = 0; i < n; ++i) {
    indexToAddress.map[i] = communicatedNodeArray[i].address;
  }

  // Test if edge has been reported twice
  success = checkAllNodesReceived(communicatedNodeArray, &invalidEdgesArray, n);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "The checking of all nodes failed, exiting\n");
    freeRoutingServer(&communicatedNodeArray, &invalidEdgesArray,
                      &indexToAddress, &adjacencyMatrix, &distanceArray,
                      &routeArray, &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Create the adjacency matrix
  success = createAdjacencyMatrix(communicatedNodeArray, &indexToAddress,
                                  &invalidEdgesArray, &adjacencyMatrix, n);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not create the adjacency matrix, exiting\n");
    freeRoutingServer(&communicatedNodeArray, &invalidEdgesArray,
                      &indexToAddress, &adjacencyMatrix, &distanceArray,
                      &routeArray, &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Compute shortest path using Dijkstra
  // We overcome the problem with discarding qualifiers with a cast
  // We can do this as we are sure that the function is not modifying the data
  // If that was the case we would've ended up with undefined behaviour
  success = dijkstra(0, (const int* const*)adjacencyMatrix, distanceArray,
                     &routeArray, n);
  if (success != EXIT_SUCCESS) {
    fprintf(
        stderr,
        "Could not find shortest path using Dijkstra's algorithm, exiting\n");
    freeRoutingServer(&communicatedNodeArray, &invalidEdgesArray,
                      &indexToAddress, &adjacencyMatrix, &distanceArray,
                      &routeArray, &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Print the edges
  printEdges(distanceArray, routeArray, &indexToAddress, n);

  // Create the routing tables
  success = createRoutingTableArray(routeArray, &routingTableArray, n);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not create the routing tables, exiting\n");
    freeRoutingServer(&communicatedNodeArray, &invalidEdgesArray,
                      &indexToAddress, &adjacencyMatrix, &distanceArray,
                      &routeArray, &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Send the routing tables

  // FIXME:
  // Close ports
  close(listenSocket);

  // Free memory
  freeRoutingServer(&communicatedNodeArray, &invalidEdgesArray, &indexToAddress,
                    &adjacencyMatrix, &distanceArray, &routeArray,
                    &routingTableArray, n);
  return EXIT_SUCCESS;
}
