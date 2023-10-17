#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, stderr, NULL
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_S...

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
  (void)listenPort;
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
