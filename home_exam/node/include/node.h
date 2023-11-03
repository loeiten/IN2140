#ifndef HOME_EXAM_NODE_INCLUDE_NODE_H_
#define HOME_EXAM_NODE_INCLUDE_NODE_H_

struct CommunicatedNode;
struct RoutingTable;

/**
 * @brief Free dynamically allocated memory and close all ports
 *
 * @param communicatedNode Structure describing the node and its neighbors
 * @param routingTable The routing table
 * @param tcpRoutingServerSocketFd Socket used for TCP communication
 * @param udpSocketFd Socket used for UDP communication
 * @param msg Message to be printed to stderr
 */
void cleanup(struct CommunicatedNode *communicatedNode,
             struct RoutingTable *routingTable,
             int *const tcpRoutingServerSocketFd, int *const udpSocketFd,
             const char *msg);

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_H_
