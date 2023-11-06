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

/**
 * @brief Prepare the packets and send them
 *
 * @param udpSocketFd The socket to use for communication
 * @param ownAddress The address of this node
 * @param serverPort The port number of the server
 *                   (used to calculate forward port)
 * @param routingTable The routing table
 * @return 0 on success, 1 on error
 */
int prepareAndSendPackets(const int udpSocketFd, const int ownAddress,
                          const int serverPort,
                          const struct RoutingTable* const routingTable);

/**
 * @brief Extract the length, destination and message from a line
 *
 * Example:
 * If the \p line reads
 *
 * \code{.unparsed}
 * 814 Good luck with the home exam\n
 * \endcode
 *
 * Then \p length is \a 35 :
 * - 2 bytes for the \p length
 * - 2 bytes for the \p destination
 * - 2 bytes for the \a source
 * - 29 bytes for <em>Good luck with the home exam\0</em>
 *
 * \p destination is \a 814 and \p message is
 * <em>Good luck with the home exam\0</em>
 *
 * @param line The line to extract from
 * @param length The length of the message
 * @param destination The destination of packet
 * @param msg The message
 * @return 0 on success, 1 on error
 */
int extractLengthDestinationAndMessage(const char* const line,
                                       unsigned short* const length,
                                       unsigned short* const destination,
                                       char* msg);

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
