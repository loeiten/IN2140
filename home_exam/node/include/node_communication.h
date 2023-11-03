#ifndef HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
#define HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_

// FIXME: These functions are untested
struct CommunicatedNode;
struct RoutingTable;

/**
 * @brief Assign a UDP socket to connectSocket for sending and receiving
 *
 * @param connectSocket The socket descriptor which can send and receive
 * @param connectPort The port to bind the socket to
 * @return 0 on success, 1 on error
 */
int getUDPSocket(int* const connectSocket, const int connectPort);

/**
 * @brief Assign a connected TCP socket to connectSocket
 *
 * NOTE: The clientPort will be assigned when calling `connect`
 * NOTE: The serverAddress will always be localhost (i.e. 127.0.0.1)
 *
 * @param clientSocket The socket descriptor which can send and receive
 * @param serverPort The port to bind the socket to
 * @return 0 on success, 1 on error
 */
int getTCPClientSocket(int* const clientSocket, const int serverPort);

/**
 * @brief Send the edge information to the routing server
 *
 * @param tcpRoutingServerSocketFd The connected TCP socket to use for
 *                                 communication
 * @param communicatedNode The node to be communicated
 * @return 0 on success, 1 on error
 */
int sendEdgeInformation(const int tcpRoutingServerSocketFd,
                        struct CommunicatedNode* const communicatedNode);

/**
 * @brief Receive the routing table
 *
 * @param tcpRoutingServerSocketFd The connected TCP socket to use for
 *                                 communication
 * @param routingTable The routing table to be filled
 * @return 0 on success, 1 on error
 */
int receiveRoutingTable(const int tcpRoutingServerSocketFd,
                        struct RoutingTable* routingTable);

/**
 * @brief Receive and potentially forward packets from other nodes
 *
 * @param udpSocketFd The socket to use for communication
 * @param ownAddress The address of this node
 * @param serverPort The port number of the server
 *                   (used to calculate forward port)
 * @param routingTable The routing table
 * @return 0 on success, 1 on error
 */
int receiveAndForwardPackets(const int udpSocketFd, const int ownAddress,
                             const int serverPort,
                             const struct RoutingTable* const routingTable);

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
