#ifndef HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_
#define HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_

struct CommunicatedNode;

int parseNodes(const int argc, const char *const *const argv,
               struct CommunicatedNode *communicatedNode);

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_PARSER_H_
