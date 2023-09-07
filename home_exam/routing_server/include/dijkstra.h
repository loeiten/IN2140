#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_

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
 * @param distance The distance from src to all other nodes
 * @param n The number of nodes in the graph
 * @return int 0 on success, 1 on failure
 */
int dijkstra(const int src, const int *const *const graph, int *const distance,
             const int n);

/**
 * @brief Helper which finds the index with the lowest distance from src
 *
 * @param distance The distance from src to all other nodes
 * @param visited The array marking if a node is visited.
 *                Note that in Dijkstra's algorithm a node is only marked
 *                visited when it has the lowest total distance from the src
 * @param n The number of nodes in the graph
 * @return int The index of the node with the lowest distance to the src
 */
int getMinDistanceIdx(const int *const distance, const int *const visited,
                      const int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_
