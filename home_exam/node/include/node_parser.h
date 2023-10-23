#ifndef HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_
#define HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_

struct CommunicatedNode;

int parseNodes(const char *const *const nodes, const int nNeighbors,
               struct CommunicatedNode *communicatedNode);

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_
