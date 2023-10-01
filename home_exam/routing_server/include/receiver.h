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
 * @brief Struct used for counting the encounter of an edge
 */
struct EdgeCounter {
  struct Edge
      edge;       /**< The edge where lowNodeAddress is the lowest of the two */
  int encounters; /**< How many times this node has been encountered */
  int addressOfFirstIndex; /**< The address of first index of first encounter */
  int reportedWeight;      /**< The weight reported on first encounter */
};

/**
 * @brief Struct used for the encounter of several edges
 */
struct EdgeCounterArray {
  struct EdgeCounter* array;   //**< Array of edge counters */
  int firstAvailablePosition;  //**< Position of first non-used counter */
  int maxEdges;                //**< Total number of edges */
};

/**
 * @brief Check that every edge between the two nodes has been reported twice.
 *
 * This function will tests that:
 *
 * - Every edge between two nodes has been reported twice
 *   (once by each of the two nodes)
 * - Both nodes reports the same weight.
 *
 * For edges where this is not the case, a warning will be printed and the edge
 * will be added to the invalidEdgesArray.
 *
 * @param receivedNodeArray Array of the received nodes
 * @param invalidEdgesArray Array of the non-valid edges
 * @param n The number of nodes in the graph
 * @returns 0 on success, 1 on error
 */
int checkAllNodesReceived(struct ReceivedNode* receivedNodeArray,
                          struct EdgeArray* invalidEdgesArray, int n);

/**
 * @brief Create the adjacency matrix
 *
 * @param receivedNodeArray Array of the received nodes
 * @param invalidEdgesArray Array of the non-valid edges
 * @param adjacencyMatrix The adjacency matrix of the graph
 * @param n The number of nodes in the graph
 * @returns 0 on success, 1 on error
 */
int createAdjacencyMatrix(struct ReceivedNode* receivedNodeArray,
                          struct EdgeArray* invalidEdgesArray,
                          int*** adjacencyMatrix, int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_RECEIVER_H_
