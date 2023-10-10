#ifndef HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_
#define HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_

struct Route;
struct RoutingTable;
struct EdgeCounter;
struct ReceivedNode;

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
 * @brief Allocate memory to the route array
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
 * @brief Allocate space to the routing table
 *
 * @param routingTable The routing table, one for each node
 * @param n The number of nodes in the graph
 * @param name Name of the routing table
 * @returns 0 on success, 1 on error
 */
int allocateRoutingTable(struct RoutingTable **routingTable, int n,
                         const char *name);

/**
 * @brief Free the routing table and its contents
 *
 * @param routingTable The routing table, one for each node
 * @param n The number of nodes in the graph
 */
void freeRoutingTable(struct RoutingTable **routingTable, int n);

int allocateEdgeCounterArray(struct EdgeCounter **edgeCounterArray, int n,
                             const char *name);

/**
 * @brief Allocate memory to the edge counter array
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
 * @brief Allocate memory to the received node array
 *
 * @param receivedNodeArray The array of received nodes
 * @param n The number of received nodes
 * @param name Name of the received node array
 * @returns 0 on success, 1 on error
 */
int allocateReceivedNodeArray(struct ReceivedNode **receivedNodeArray, int n,
                              const char *name);

/**
 * @brief Allocate memory to the neighbor addresses and corresponding weights
 *
 * @param receivedNode The received node
 * @param nNeighbors The number of neighbors
 * @param name Name of the neighbor
 * @returns 0 on success, 1 on error
 */
int allocateReceivedNodeNeighborAndWeights(struct ReceivedNode *receivedNode,
                                           int nNeighbors, const char *name);

/**
 * @brief Free the memory of the received node array
 *
 * @param receivedNodeArray The array of received nodes
 * @param n The number of received nodes
 */
void freeReceivedNodeArray(struct ReceivedNode **receivedNodeArray, int n);

#endif  // HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_
