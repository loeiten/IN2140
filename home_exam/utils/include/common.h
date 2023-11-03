#ifndef HOME_EXAM_UTILS_INCLUDE_COMMON_H_
#define HOME_EXAM_UTILS_INCLUDE_COMMON_H_

#define MAX_MSG_LENGTH (1024)

/**
 * @brief Struct representing the communicated node
 */
struct CommunicatedNode {
  int address;            /**< The address of this node */
  int nNeighbors;         /**< The number of neighbors */
  int* neighborAddresses; /**< Addresses to the other neighbors */
  int* edgeWeights;       /**< Weight to the neighbors */
};

/**
 * @brief Elements in the routing table
 */
struct RoutingTableRows {
  int destination; /**< Destination of package */
  int nextHop;     /**< The next hop needed in order to reach the destination */
};

/**
 * @brief The routing table
 */
struct RoutingTable {
  int nRows;                                 /**< Number of elements */
  struct RoutingTableRows* routingTableRows; /**< Array of routingTables */
};

/**
 * @brief Map from index to node id
 */
struct IndexToAddress {
  int n;    /**< Number of elements */
  int* map; /**< The index to id map */
};

/**
 * @brief Struct representing an edge
 */
struct Edge {
  int lowNodeAddress;  /**< One of the nodes in the edge */
  int highNodeAddress; /**< The other node in the edge */
};

/**
 * @brief Struct representing an array of edges
 */
struct EdgeArray {
  struct Edge* array;          /**< The array */
  int firstAvailablePosition;  //**< Position of first non-used counter */
  int maxEdges;                /**< Number of edges */
};

/**
 * @brief Get the index from an address
 *
 * @param address The address to get the index from
 * @param indexToAddress The map of index to address
 * @param index The index corresponding to the address
 * @return 0 in success, 1 on error
 */
int getIndexFromAddress(const int address,
                        const struct IndexToAddress* const indexToAddress,
                        int* index);

#endif  // HOME_EXAM_UTILS_INCLUDE_COMMON_H_
