#ifndef HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_
#define HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_

struct EdgeArray;
struct EdgeCounter;
struct IndexToAddress;
struct Node;
struct Route;
struct RoutingTable;
struct RoutingTableArray;

/**
 * @brief Allocate an int array
 *
 * @param intArray The pointer to allocate the array to
 * @param n The size of the array
 * @param name Name of the array
 * @returns 0 on success, 1 on error
 */
int allocateIntArray(int **intArray, const int n, const char *name);

/**
 * @brief Free the memory from a malloced array
 *
 * @param intArray The array to free.
 *                 We're passing the pointer to the pointer as we're
 *                 modifying the original pointer
 */
void freeIntArray(int **intArray);

/**
 * @brief Allocate an int matrix
 *
 * @param intMatrix The pointer to allocate the matrix to
 * @param n The size of the nxn matrix
 * @param name Name of the matrix
 * @returns 0 on success, 1 on error
 */
int allocateIntMatrix(int ***intMatrix, const int n, const char *name);

/**
 * @brief Free the memory from a malloced array
 *
 * @param intMatrix The matrix to free.
 *                  We're passing a triple pointer as we're
 *                  modifying the original pointer
 * @param n The number of rows in the matrix
 */
void freeIntMatrix(int ***intMatrix, int n);

/**
 * @brief Allocate memory to a route array
 *
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 *                   We're passing the pointer to the pointer as we're modifying
 *                   the original pointer
 * @param n The number of nodes in the graph
 * @param name Name of the route array
 * @returns 0 on success, 1 on error
 */
int allocateRouteArray(struct Route **routeArray, const int n,
                       const char *name);

/**
 * @brief Free the malloced memory form the route array
 *
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 *                   We're passing the pointer to the pointer as we're modifying
 *                   the original pointer
 * @param n The number of nodes in the graph
 */
void freeRouteArray(struct Route **routeArray, const int n);

/**
 * @brief Allocate memory to a routing table
 *
 * @param routingTable The routing table, one for each node
 * @param nRows The number of nodes in the graph
 * @param name Name of the routing table
 * @returns 0 on success, 1 on error
 */
int allocateRoutingTable(struct RoutingTable *routingTable, const int nRows,
                         const char *name);

/**
 * @brief Free the routing table and its contents
 *
 * @param routingTableArray The routing table, one for each node
 */
void freeRoutingTable(struct RoutingTable *routingTable);

/**
 * @brief Allocate memory to an array of routing tables
 *
 * @param routingTableArray The routing table array, one for each node
 * @param n The number of nodes in the graph
 * @param name Name of the routing table
 * @returns 0 on success, 1 on error
 */
int allocateRoutingTableArray(struct RoutingTableArray *routingTableArray,
                              const int n, const char *name);

/**
 * @brief Free the routing table array and its contents
 *
 * @param routingTableArray The routing table array
 */
void freeRoutingTableArray(struct RoutingTableArray *routingTableArray);

/**
 * @brief Allocate memory and initialize the values to an edge array
 *
 * @param edgeArray The array of edges
 * @param maxEdges The maximum number of edges in the graph
 * @param name Name of the edge counter
 * @returns 0 on success, 1 on error
 */
int allocateEdgeArray(struct EdgeArray *edgeArray, int maxEdges,
                      const char *name);

/**
 * @brief Free the memory of the edge counter array
 *
 * @param edgeArray The array of edges
 */
void freeEdgeArray(struct EdgeArray *edgeArray);

/**
 * @brief Allocate memory to an edge counter array
 *
 * @param edgeCounterArray The array of edge counters
 * @param maxEdges The maximum number of edges in the graph
 * @param name Name of the edge counter
 * @returns 0 on success, 1 on error
 */
int allocateEdgeCounterArray(struct EdgeCounter **edgeCounterArray,
                             int maxEdges, const char *name);

/**
 * @brief Free the memory of the edge counter array
 *
 * @param edgeCounterArray The array of edge counters
 */
void freeEdgeCounterArray(struct EdgeCounter **edgeCounterArray);

/**
 * @brief Allocate memory and initialize the values to a node array
 *
 * @param nodeArray The array of nodes
 * @param n The number of nodes
 * @param name Name of the node array
 * @returns 0 on success, 1 on error
 */
int allocateNodeArray(struct Node **nodeArray, int n, const char *name);

/**
 * @brief Allocate memory to a neighbor addresses and corresponding weights
 *
 * @param node The node
 * @param nNeighbors The number of neighbors
 * @param name Name of the neighbor
 * @returns 0 on success, 1 on error
 */
int allocateNodeNeighborAndWeights(struct Node *node, int nNeighbors,
                                   const char *name);

/**
 * @brief Free the memory of the node array
 *
 * @param nodeArray The array of nodes
 * @param n The number of nodes
 */
void freeNodeArray(struct Node **nodeArray, int n);

/**
 * @brief Allocate memory to an index to address struct
 *
 * @param indexToAddress The index to address struct
 * @param n The number of nodes
 * @param name Name of the index to address struct
 * @returns 0 on success, 1 on error
 */
int allocateIndexToAddress(struct IndexToAddress *indexToAddress, int n,
                           const char *name);

/**
 * @brief Allocate memory to the neighbor addresses and edge weights
 *
 * @param node The node to allocate memory to
 * @param nNeighbors The number of neighbors
 * @param name The name of the communication node
 * @return 0 on success, 1 on error
 */
int allocateNeighborAddressesAndEdgeWeights(struct Node *node,
                                            const int nNeighbors,
                                            const char *name);

/**
 * @brief Free memory of the neighbor addresses and edge weights
 *
 * @param node The node to free memory from
 */
void freeNeighborAddressesAndEdgeWeights(struct Node *node);

/**
 * @brief Free the memory of the edge counter array
 *
 * @param indexToAddress The index to address struct
 */
void freeIndexToAddress(struct IndexToAddress *indexToAddress);

/**
 * @brief Allocate all memory for the routing server
 *
 * @param nodeArray The array of nodes
 * @param invalidEdgesArray The invalid edges array
 * @param indexToAddress The index to array map
 * @param adjacencyMatrix The adjacency matrix
 * @param distanceArray The distance array
 * @param routeArray The route array
 * @param routingTableArray The routing table
 * @param n The number of nodes
 * @param maxEdges The maximum number of edges
 * @return 0 on success, 1 on error
 */
int allocateRoutingServer(struct Node **nodeArray,
                          struct EdgeArray *invalidEdgesArray,
                          struct IndexToAddress *indexToAddress,
                          int ***adjacencyMatrix, int **distanceArray,
                          struct Route **routeArray,
                          struct RoutingTableArray *routingTableArray, int n,
                          int maxEdges);

/**
 * @brief Free memory allocated to the routing server
 *
 * @param nodeArray The array of nodes
 * @param invalidEdgesArray The invalid edges array
 * @param indexToAddress The index to array map
 * @param adjacencyMatrix The adjacency matrix
 * @param distanceArray The distance array
 * @param routeArray The route array
 * @param routingTableArray The routing table
 * @param n The number of nodes
 */
void freeRoutingServer(struct Node **nodeArray,
                       struct EdgeArray *invalidEdgesArray,
                       struct IndexToAddress *indexToAddress,
                       int ***adjacencyMatrix, int **distanceArray,
                       struct Route **routeArray,
                       struct RoutingTableArray *routingTableArray, int n);

#endif  // HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_
