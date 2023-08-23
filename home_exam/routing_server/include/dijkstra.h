#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_

/**
 * @brief Computes the distance from the source to all other nodes.
 *
 * @param src The id of the source
 * @param graph The graph to traverse given as an adjacency matrix
 * @param distance The distance from src to all other nodes
 */
void dijkstra(const int src, const int *const *const graph,
              int *const *const distance);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_DIJKSTRA_H_
