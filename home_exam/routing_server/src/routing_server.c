#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr
#include <stdlib.h>  // for EXIT_FAILURE
#include <unistd.h>  // for close

#include "../../utils/include/common.h"               // for getIndexFromAdd...
#include "../../utils/include/dynamic_memory.h"       // for freeRoutingServer
#include "../include/adjacency_matrix.h"              // for createAdjacency...
#include "../include/dijkstra.h"                      // for dijkstra
#include "../include/route.h"                         // for createRoutingTa...
#include "../include/routing_server_communication.h"  // for getTCPServerSocket
#include "../include/validation.h"                    // for checkAllNodesRe...

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
  int basePort = atoi(argv[1]);
  int n = atoi(argv[2]);
  int maxEdges = (n * (n - 1) / 2);

  // Allocate memory
  struct Node* nodeArray = NULL;
  struct EdgeArray invalidEdgesArray;
  struct IndexToAddress indexToAddress;
  int** adjacencyMatrix = NULL;
  int* distanceArray = NULL;
  struct Route* routeArray = NULL;
  struct RoutingTableArray routingTableArray;
  int success = allocateRoutingServer(
      &nodeArray, &invalidEdgesArray, &indexToAddress, &adjacencyMatrix,
      &distanceArray, &routeArray, &routingTableArray, n, maxEdges);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Allocation of memory failed, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    return EXIT_FAILURE;
  }

  int listenSocket = -1;
  success = getTCPServerSocket(&listenSocket, basePort);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Obtaining the listen socket failed, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    if (listenSocket != -1) {
      close(listenSocket);
      listenSocket = -1;
    }
    return EXIT_FAILURE;
  }

  success = populateNodeArray(listenSocket, nodeArray, n);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Populating the node array failed, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Fill the index to address struct
  for (int i = 0; i < n; ++i) {
    indexToAddress.map[i] = nodeArray[i].address;
  }

  // Test if edge has been reported twice
  success = checkAllNodesReceived(nodeArray, &invalidEdgesArray, n);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "The checking of all nodes failed, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Create the adjacency matrix
  success = createAdjacencyMatrix(nodeArray, &indexToAddress,
                                  &invalidEdgesArray, &adjacencyMatrix, n);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not create the adjacency matrix, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Compute shortest path using Dijkstra
  // We overcome the problem with discarding qualifiers with a cast
  // We can do this as we are sure that the function is not modifying the data
  // If that was the case we would've ended up with undefined behaviour
  int indexOfOwnAddress1;
  success = getIndexFromAddress(1, &indexToAddress, &indexOfOwnAddress1);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could find index of address 1, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }
  success = dijkstra(indexOfOwnAddress1, (const int* const*)adjacencyMatrix,
                     distanceArray, &routeArray, n);
  if (success != EXIT_SUCCESS) {
    fprintf(
        stderr,
        "Could not find shortest path using Dijkstra's algorithm, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Print the edges
  printEdges(distanceArray, routeArray, &indexToAddress, n);

  // Create the routing tables
  success = createRoutingTableArray(routeArray, &routingTableArray, n);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not create the routing tables, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Send the routing tables
  success = sendRoutingTables(nodeArray, &routingTableArray, &indexToAddress);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not send the routing tables, exiting\n");
    freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                      &adjacencyMatrix, &distanceArray, &routeArray,
                      &routingTableArray, n);
    close(listenSocket);
    return EXIT_FAILURE;
  }

  // Free memory
  freeRoutingServer(&nodeArray, &invalidEdgesArray, &indexToAddress,
                    &adjacencyMatrix, &distanceArray, &routeArray,
                    &routingTableArray, n);

  // Close listenSocket
  close(listenSocket);
  return EXIT_SUCCESS;
}
