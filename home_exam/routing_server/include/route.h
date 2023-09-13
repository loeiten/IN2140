#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_

/**
 *  @brief Representation of a route to an endpoint
 */
struct Route {
  int nHops;  /**< Number of hops from the starting point */
  int *route; /**< The route taken */
};

/**
 * @brief Print the edges from the source route to all other nodes
 *
 * @param src The id of the source
 * @param distanceArray The distance from src to all other nodes
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 * @param n The number of nodes in the graph
 */
void printEdges(const int *const distanceArray,
                const struct Route *const routeArray, const int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_
