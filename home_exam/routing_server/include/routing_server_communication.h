#ifndef HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTING_SERVER_COMMUNICATION_H_
#define HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTING_SERVER_COMMUNICATION_H_

struct Node;
struct IndexToAddress;
struct RoutingTable;
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
 * @brief Accept a connection from the listening port
 *
 * @param listenSocket The socket which is in listen mode
 * @param newSocketFd The socket obtained from a successful call to `accept`
 * @return 0 on success, 1 on error
 */
int acceptConnection(const int listenSocket, int* newSocketFd);

/**
 * @brief Send the routing tables to the nodes
 *
 * @param nodeArray The array of nodes (contains the tcp socket)
 * @param routingTableArray The array of routing tables
 * @param indexToAddress Object mapping indices to addresses
 * @return 0 on success, 1 on error
 */
int sendRoutingTables(const struct Node* const nodeArray,
                      const struct RoutingTableArray* const routingTableArray,
                      const struct IndexToAddress* const indexToAddress);

/**
 * @brief Translate routing table from index to address
 *
 * @param idxRoutingTable Original routing table using indices
 * @param indexToAddress Object mapping indices to addresses
 * @param addressRoutingTable New routing table using addresses
 * @return 0 on success, 1 on error
 */
int translateTableFromIdxToAddress(
    const struct RoutingTable* const idxRoutingTable,
    const struct IndexToAddress* const indexToAddress,
    struct RoutingTable* addressRoutingTable);

#endif  // HOME_EXAM_ROUTING_SERVER_INCLUDE_ROUTING_SERVER_COMMUNICATION_H_
