#ifndef HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
#define HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_

#include <stdio.h>  // for FILE

struct Node;
struct RoutingTable;

/**
 * @brief Assign a UDP socket to connectSocket for sending and receiving
 *
 * @param connectSocket Integer to be filled with the socket descriptor that
 *                      can send and receive
 * @param port The port to bind the socket to
 * @return 0 on success, 1 on error
 */
int getUDPSocket(int* const connectSocket, const int port);

/**
 * @brief Assign a connected TCP socket to connectSocket
 *
 * NOTE: The clientPort will be assigned when calling `connect`
 * NOTE: The serverAddress will always be localhost (i.e. 127.0.0.1)
 *
 * @param clientSocket The socket descriptor which can send and receive
 * @param basePort The port to bind the socket to
 * @return 0 on success, 1 on error
 */
int getTCPClientSocket(int* const clientSocket, const int basePort);

/**
 * @brief Send the edge information to the routing server
 *
 * @param node The node to be communicated
 * @return 0 on success, 1 on error
 */
int sendEdgeInformation(struct Node* const node);

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
 * @param basePort The port number of the server
 *                   (used to calculate forward port)
 * @param routingTable The routing table
 * @return 0 on success, 1 on error
 */
int receiveAndForwardPackets(const int udpSocketFd, const int ownAddress,
                             const int basePort,
                             const struct RoutingTable* const routingTable);

/**
 * @brief Prepare the packets and send them
 *
 * @param filepath Filepath to the text file to open
 * @param udpSocketFd The socket to use for communication
 * @param ownAddress The address of this node
 * @param basePort The port number of the server
 *                   (used to calculate forward port)
 * @param routingTable The routing table
 * @return 0 on success, 1 on error
 */
int prepareAndSendPackets(const char* filepath, const int udpSocketFd,
                          const int ownAddress, const int basePort,
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
                                       char** msg);

/**
 * @brief Free memory and close file descriptors for prepareAndSendPackets
 *
 * @param fp The file pointer
 * @param line The line
 * @param msg The message
 * @param packet The packet
 * @param quitMsg The message which triggers an exit
 * @param errorMsg The error message
 */
void cleanUpPrepareAndSendPackets(FILE* fp, char** line, char** msg,
                                  char** packet, char** quitMsg,
                                  const char* errorMsg);

/**
 * @brief Create a packet
 *
 * The packet has the following format:
 * - packet length (2 bytes in network byte order)
 * - destination address (2 bytes in network byte order)
 * - source address (2 bytes in network byte order)
 * - message (this a string that must always be 0-terminated)
 *
 * See also:
 * https://www.ibm.com/docs/en/zvm/7.3?topic=domains-network-byte-order-host-byte-order
 * https://commons.wikimedia.org/wiki/File:32bit-Endianess.svg#/media/File:32bit-Endianess.svg
 * https://en.wikipedia.org/wiki/Endianness
 *
 * @param length The length of the message
 * @param destination The destination of the message
 * @param source The source of the message
 * @param msg The message
 * @param packet The packet to be filled
 * @return 0 on success, 1 on error
 */
int createPacket(const int length, const unsigned short destination,
                 const unsigned short source, const char* const msg,
                 char** packet);

/**
 * @brief Send a UDP packet
 *
 * @param packet The packet to send
 * @param length The length of the packet
 * @param udpSocketFd The socket to use for communication
 * @param ownAddress The address of this node
 * @param ownAddress The address of the destination
 * @param basePort The port number of the server
 *                   (used to calculate the destination port)
 * @param routingTable The routing table used to find the next hop
 * @return 0 on success, 1 on error
 */
int sendUDPPacket(const char* const packet, const int length,
                  const int udpSocketFd, const unsigned short ownAddress,
                  const unsigned short destination, const int basePort,
                  const struct RoutingTable* const routingTable);

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
