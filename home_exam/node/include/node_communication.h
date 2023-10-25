#ifndef HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
#define HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_

enum connectionType { TCP, UDP };

int getConnectSocket(const int connectPort, int* const connectSocket,
                     enum connectionType connection);

#endif  // HOME_EXAM_NODE_INCLUDE_NODE_COMMUNICATION_H_
