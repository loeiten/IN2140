#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_ADJACENCY_MATRIX_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_ADJACENCY_MATRIX_H_

struct EdgeArray;
struct CommunicatedNode;
struct IndexToAddress;

/**
 * @brief Create the adjacency matrix
 *
 * @param communicatedNodeArray Array of the communicated nodes
 * @param invalidEdgesArray Array of the non-valid edges
 * @param adjacencyMatrix The adjacency matrix of the graph
 * @param n The number of nodes in the graph
 * @returns 0 on success, 1 on error
 */
int createAdjacencyMatrix(
    const struct CommunicatedNode* const communicatedNodeArray,
    const struct IndexToAddress* const indexToAddress,
    const struct EdgeArray* const invalidEdgesArray, int*** adjacencyMatrix,
    const int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_ADJACENCY_MATRIX_H_
