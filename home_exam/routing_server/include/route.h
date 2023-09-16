#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_

/**
 *  @brief Representation of a route to an endpoint using indices
 */
struct Route {
  int nHops;  /**< Number of hops from the starting point */
  int *route; /**< The route taken */
};

/**
 *  @brief Map from index to node id
 */
struct IndexToId {
  int n;    /**< Number of elements */
  int *map; /**< The index to id map */
};

/**
 * @brief Print the edges from the source route to all other nodes
 *
 * @param distanceArray The distance from src to all other nodes
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 * @param indexToIdMap A map from index in to vertex id
 * @param n The number of nodes in the graph
 */
void printEdges(const int *const distanceArray,
                const struct Route *const routeArray,
                const struct IndexToId *const indexToIdMap, const int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_
