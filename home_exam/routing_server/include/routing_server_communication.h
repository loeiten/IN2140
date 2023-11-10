#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTING_SERVER_COMMUNICATION_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTING_SERVER_COMMUNICATION_H_

// FIXME: These functions are untested

struct Node;
struct RoutingTableArray;

/**
 * @brief Get a TCP socket which is listening to a specific port
 *
 * @param listenSocket The socket descriptor which will listen
 * @param basePort The port to bind the socket to
 * @return 0 on success, 1 on error
 */
int getTCPServerSocket(int* const listenSocket, const int basePort);

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

/**
 * @brief Send the routing tables to the nodes
 *
 * @param nodeArray The array of nodes
 * @param routingTableArray  The array of routing tables
 * @return 0 on success, 1 on error
 */
int sendRoutingTables(const struct Node* const nodeArray,
                      const struct RoutingTableArray* const routingTableArray);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTING_SERVER_COMMUNICATION_H_
