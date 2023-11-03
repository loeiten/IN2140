#ifndef HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_
#define HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_

struct CommunicatedNode;

/**
 * @brief Parse nodes
 *
 * @param nodes On the form ["<neighborAddress>:<weight>", ...]
 * @param nNeighbors The number of neighbors
 * @param communicatedNode Structure describing the node and its neighbors
 * @return 0 in success, 1 on error
 */
int parseNodes(const char *const *const nodes, const int nNeighbors,
               struct CommunicatedNode *communicatedNode);

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_
