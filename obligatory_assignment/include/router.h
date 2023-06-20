#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_

struct Router {
  const unsigned char routerId;
  unsigned char flag;
  const char* producerModel;
  int neighbors[10];  // Assignment specifies that a router can have
                      // maximum 10 connections
};

// A note on constness:
// Printing should not be able to change any input
// When obtaining the binary string we should not be able to change the pointer
int printStruct(const struct Router* const routerArray, const unsigned int N,
                const int routerId);
void getBinaryString(const unsigned char c, char* const binaryStr);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
