#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_

struct IndexToAddress;
struct RoutingTableArray;

/**
 * @brief Representation of a route to an endpoint using indices
 */
struct Route {
  int nHops;  /**< Number of hops from the starting point */
  int *route; /**< The route taken */
};

/**
 * @brief Print the edges from the source route to all other nodes
 *
 * @param distanceArray The distance from src to all other nodes
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 * @param indexToAddressMap A map from index in to vertex id
 * @param n The number of nodes in the graph
 */
void printEdges(const int *const distanceArray,
                const struct Route *const routeArray,
                const struct IndexToAddress *const indexToAddressMap,
                const int n);

/**
 * @brief Create the array of routing tables
 *
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 * @param routingTableArray The routing table, one for each node
 * @param n The number of nodes in the graph
 * @returns 0 on success, 1 on error
 */
int createRoutingTableArray(struct Route *routeArray,
                            struct RoutingTableArray **const routingTableArray,
                            int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTE_H_
