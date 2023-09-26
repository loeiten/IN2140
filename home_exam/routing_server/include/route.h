#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_

/**
 * @brief Representation of a route to an endpoint using indices
 */
struct Route {
  int nHops;  /**< Number of hops from the starting point */
  int *route; /**< The route taken */
};

/**
 * @brief Map from index to node id
 */
struct IndexToId {
  int n;    /**< Number of elements */
  int *map; /**< The index to id map */
};

/**
 * @brief Elements in the routing table
 */
struct DestinationNextPair {
  int destination; /**< Destination of package */
  int nextHop;     /**< The next hop needed in order to reach the destination */
};

/**
 * @brief The routing table
 */
struct RoutingTable {
  int n;                             /**< Number of elements */
  struct DestinationNextPair *table; /**< Array of dest-next pairs */
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

/**
 * @brief Create the routing tables
 *
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 * @param routingTable The routing table, one for each node
 * @param n The number of nodes in the graph
 * @returns 0 on success, 1 on error
 */
int createRoutingTables(struct Route *routeArray,
                        struct RoutingTable **const routingTable, int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_
