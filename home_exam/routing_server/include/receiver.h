#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_RECEIVER_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_RECEIVER_H_

/**
 * @brief Struct representing the received node
 */
struct ReceivedNode {
  int address;            /**< The address of this node */
  int nNeighbors;         /**< The number of neighbors */
  int* neighborAddresses; /**< Addresses to the other neighbors */
  int* neighborWeights;   /**< Weight to the neighbors */
};

/**
 * @brief Struct representing an edge
 */
struct Edge {
  int nodeAddressA; /**< One of the nodes in the edge */
  int nodeAddressB; /**< The other node in the edge */
};

/**
 * @brief Struct representing an array of edges
 */
struct EdgeArray {
  struct Edge* array; /**< The array */
  int n;              /**< Number of edges */
};

/**
 * @brief Check that every edge between the two nodes has been reported twice.
 *
 * Will print a warning if this is not the case and remove the edge.
 *
 * @param receivedNodeArray Array of the received nodes
 * @param nonValidEdgesArray Array of the non-valid edges
 * @param n The number of nodes in the graph
 * @returns 0 on success, 1 on error
 */
int checkAllNodesReceived(struct ReceivedNode* receivedNodeArray,
                          struct EdgeArray* nonValidEdgesArray, int n);

/**
 * @brief Create the adjacency matrix
 *
 * @param receivedNodeArray Array of the received nodes
 * @param nonValidEdgesArray Array of the non-valid edges
 * @param adjacencyMatrix The adjacency matrix of the graph
 * @param n The number of nodes in the graph
 * @returns 0 on success, 1 on error
 */
int createAdjacencyMatrix(struct ReceivedNode* receivedNodeArray,
                          struct EdgeArray* nonValidEdgesArray,
                          int*** adjacencyMatrix, int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_RECEIVER_H_
