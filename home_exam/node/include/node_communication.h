#ifndef HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
#define HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_

// FIXME: These functions are untested

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

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
