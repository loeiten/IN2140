#ifndef HOME_EXAM_UTILS_INCLUDE_COMMON_H_
#define HOME_EXAM_UTILS_INCLUDE_COMMON_H_

#include <sys/socket.h>  // for ssize_t

#define MAX_MSG_LENGTH (1024)
// Non-privileged users are not allowed to bind to privileged ports (port
// numbers below 1024)
#define MIN_PORT (1024)
#define MAX_PORT (65500)

/**
 * @brief Struct representing the node
 */
struct Node {
  int tcpSocket;          /**< TCP socket used for communication */
  int address;            /**< The address of this node */
  int nNeighbors;         /**< The number of neighbors */
  int* neighborAddresses; /**< Addresses to the other neighbors */
  int* edgeWeights;       /**< Weight to the neighbors */
};

/**
 * @brief Elements in the routing table
 */
struct RoutingTableRows {
  int destination; /**< Destination of package */
  int nextHop;     /**< The next hop needed in order to reach the destination */
};

/**
 * @brief The routing table
 */
struct RoutingTable {
  int nRows;                                 /**< Number of elements */
  struct RoutingTableRows* routingTableRows; /**< Array of routingTables */
};

/**
 * @brief Map from index to node address
 */
struct IndexToAddress {
  int n;    /**< Number of elements */
  int* map; /**< The index to address map */
};

/**
 * @brief Struct representing an edge
 */
struct Edge {
  int lowNodeAddress;  /**< One of the nodes in the edge */
  int highNodeAddress; /**< The other node in the edge */
};

/**
 * @brief Struct representing an array of edges
 */
struct EdgeArray {
  struct Edge* array;          /**< The array */
  int firstAvailablePosition;  //**< Position of first non-used counter */
  int maxEdges;                /**< Number of edges */
};

/**
 * @brief Get the index from an address
 *
 * @param address The address to get the index from
 * @param indexToAddress The map of index to address
 * @param index The index corresponding to the address
 * @return 0 in success, 1 on error
 */
int getIndexFromAddress(const int address,
                        const struct IndexToAddress* const indexToAddress,
                        int* index);

/**
 * @brief Receive message from a socket which is nBytes long
 *
 * @param sockFd The socket to use for receiving
 * @param buf The buffer to store the data to
 * @param nBytes The number of bytes expected
 * @param flag The flags
 * @return 0 in success, 1 on error
 */
int receiveNBytesMessage(const int sockFd, void* buf, const ssize_t nBytes,
                         const int flag);

/**
 * @brief Receive message from a socket with unknown length
 *
 * @param sockFd The socket to use for receiving
 * @param buf The buffer to store the data to
 * @param flag The flags
 * @return 0 in success, 1 on error
 */
int receiveMessage(const int sockFd, void* buf, const int flag);

/**
 * @brief Send a message from a socket
 *
 * @param sockFd The socket to use for receiving
 * @param buf The buffer to store the data to
 * @param nBytes The number of bytes expected
 * @param flag The flags
 * @return 0 in success, 1 on error
 */
int sendMessage(const int sockFd, void* buf, const ssize_t nBytes,
                const int flag);

#endif  // HOME_EXAM_UTILS_INCLUDE_COMMON_H_
