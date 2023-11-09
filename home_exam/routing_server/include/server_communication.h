#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_SERVER_COMMUNICATION_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_SERVER_COMMUNICATION_H_

// FIXME: These functions are untested

struct Node;

/**
 * @brief Get a TCP socket which is listening to a specific port
 *
 * @param listenSocket The socket descriptor which will listen
 * @param listenPort The port to bind the socket to
 * @return 0 on success, 1 on error
 */
int getTCPServerSocket(int* const listenSocket, const int listenPort);

/**
 * @brief Populate the node array
 *
 * @param listenSocket The socket descriptor which listens
 * @param nodeArray Array of the nodes
 * @param n The number of nodes in the graph
 * @return 0 on success, 1 on error
 */
int populateNodeArray(const int listenSocket, struct Node* nodeArray,
                      const int n);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_SERVER_COMMUNICATION_H_
