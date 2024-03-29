#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_VALIDATION_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_VALIDATION_H_

#include "../../utils/include/common.h"  // for Edge

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
 * @param nodeArray Array of the nodes
 * @param invalidEdgesArray Array of the non-valid edges
 * @param n The number of nodes in the graph
 * @returns 0 on success, 1 on error
 */
int checkAllNodesReceived(struct Node* nodeArray,
                          struct EdgeArray* invalidEdgesArray, int n);

/**
 * @brief Check for invalid edges, add to invalidEdgesArray and print warning.
 *
 * @param lowAddress The low address of the edge
 * @param highAddress The high address of the edge
 * @param addressOfFirstIndex The address of the first recorded edge
 * @param edgeWeight The weight of the edge
 * @param invalidEdgesArray The edge array to fill
 * @param edgeCounter The edge counter to use for the checks
 * @returns 0 on success, 1 on error
 */
int checkIfEdgeIsValid(const int lowAddress, const int highAddress,
                       const int addressOfFirstIndex, const int edgeWeight,
                       const struct EdgeCounter* const edgeCounter,
                       struct EdgeArray* invalidEdgesArray);

/**
 * @brief Add an edge to the invalid edges array and print a warning
 *
 * @param lowAddress The low address of the edge
 * @param highAddress The high address of the edge
 * @param invalidEdgesArray The edge array to fill
 * @param reason The reason for why this is an invalid edge
 * @returns 0 on success, 1 on error
 */
int addInvalidEdge(const int lowAddress, const int highAddress,
                   struct EdgeArray* invalidEdgesArray, const char* reason);

/**
 * @brief Check if an edge is present in the edge array
 *
 * @param lowAddress The low address of the edge
 * @param highAddress The high address of the edge
 * @param edgesArray The edge array to check for the edge
 * @return 1 if the edge is present, 0 if the edge is absent
 */
int isEdgePresent(const int lowAddress, const int highAddress,
                  const struct EdgeArray* const edgeArray);

/**
 * @brief Adds an edge to an edge counter array
 *
 * @param lowAddress The low address of the edge
 * @param highAddress The high address of the edge
 * @param edgeCounterArray The edge counter array
 * @return 0 on success, 1 on error
 */
int addEdgeToEdgeCounterArray(const int lowAddress, const int highAddress,
                              const int addressOfFirstIndex,
                              const int edgeWeight,
                              struct EdgeCounterArray* edgeCounterArray);

/**
 * @brief Check that the edges has been reported from both sides.
 *
 * If this is not the case, issue a warning and add the edge to
 * invalidEdgesArray
 *
 * @param edgeCounterArray The edge counter array
 * @param invalidEdgesArray The invalid edges array
 * @returns 0 on success, 1 on error
 */
int checkDualReport(const struct EdgeCounterArray* const edgeCounterArray,
                    struct EdgeArray* invalidEdgesArray);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_VALIDATION_H_
