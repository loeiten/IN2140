#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_

struct Route;

/**
 * @brief Computes the distance from the source to all other nodes.
 *
 * The algorithm is visualized in
 * https://www.freecodecamp.org/news/dijkstras-shortest-path-algorithm-visual-introduction/
 *
 * Comment: We use double pointer to represent matrices
 * Drawback: Indirection through pointers
 * Drawback: Less cache friendly
 * Pro: One can access the elements by using [row][col]
 *
 * @param src The index of the source
 * @param adjacencyMatrix The graph to traverse given as an adjacency matrix.
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
int dijkstra(const int src, const int *const *const adjacencyMatrix,
             int *const distanceArray, struct Route **const routeArray,
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
 * @param src The id of the source
 * @param curVisitIdx The index of the node that just has been marked as visited
 * @param adjacencyMatrix The adjacency matrix
 * @param n The number of nodes in the graph
 * @param visitedArray The array marking if a node is visited
 * @param distanceArray The distance from src to all other nodes
 * @param visitedAndNeighbourArray Array of nodes which are both visited and
 *                                 neighbors of curVisitIdx
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph
 * @return int 0 on success, 1 on failure
 */
int registerRoute(const int src, const int curVisitIdx, const int n,
                  const int *const *const adjacencyMatrix,
                  const int *const visitedArray, const int *const distanceArray,
                  int *const visitedAndNeighbourArray,
                  struct Route *const routeArray);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_
