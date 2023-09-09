#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_

/**
 *  @brief Representation of a route to an endpoint
 */
struct Route {
  int nHops;  /**< Number of hops from the starting point */
  int *route; /**< The route taken */
};

/**
 * @brief Computes the distance from the source to all other nodes.
 *
 * Comment: We use double pointer to represent matrices
 * Drawback: Indirection through pointers
 * Pro: One can access the elements by using [row][col]
 *
 * @param src The id of the source
 * @param graph The graph to traverse given as an adjacency matrix.
 *              The adjacency matrix will have INT_MAX where if there are no
 *              connection between the nodes
 * @param distanceArray The distance from src to all other nodes
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 *                   The array will be allocated, so it needs to be freed
 *                   outside this function
 * @param n The number of nodes in the graph
 * @return int 0 on success, 1 on failure
 */
int dijkstra(const int src, const int *const *const graph,
             int *const distanceArray, struct Route *const routeArray,
             const int n);

/**
 * @brief Helper which finds the index with the lowest distance from src
 *
 * @param distanceArray The distance from src to all other nodes
 * @param visitedArray The array marking if a node is visited.
 *                Note that in Dijkstra's algorithm a node is only marked
 *                visited when it has the lowest total distance from the src
 * @param n The number of nodes in the graph
 * @return int The index of the node with the lowest distance to the src
 */
int getMinDistanceIdx(const int *const distanceArray,
                      const int *const visitedArray, const int n);

/**
 * @brief Register the route.
 *
 * @param graph The graph
 * @param visitIdx The index of the node that just has been marked as visited
 * @param distanceArray The distance from src to all other nodes
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph
 * @return int 0 on success, 1 on failure
 */
int registerRoute(const int *const *const graph, const int visitIdx,
                  const int *const distanceArray,
                  struct Route *const routeArray);

/**
 * @brief Free the memory from the visitedArray
 *
 * @param visitedArray The array marking if a node is visited.
 *                     We're passing the pointer to the pointer as we're
 *                     modifying the original pointer
 * @param n The number of nodes in the graph
 */
void freeVisitedArray(int **visitedArray);

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

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_
