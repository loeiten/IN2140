#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_SERVER_COMMUNICATION_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_SERVER_COMMUNICATION_H_

// FIXME: These functions are untested

struct CommunicatedNode;

/**
 * @brief Get the a listen socket
 *
 * @param listenPort The port to bind the socket to
 * @param listenSocket The socket descriptor
 * @return 0 on success, 1 on error
 */
int getListenSocket(const int listenPort, int* const listenSocket);

/**
 * @brief Populate the communicated node array
 *
 * @param listenSocket The socket descriptor which listens
 * @param communicatedNodeArray Array of the communicated nodes
 * @param n The number of nodes in the graph
 * @return 0 on success, 1 on error
 */
int populateCommunicatedNodeArray(
    const int listenSocket, struct CommunicatedNode* communicatedNodeArray,
    const int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_SERVER_COMMUNICATION_H_
