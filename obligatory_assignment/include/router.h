#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_

struct Router {
  unsigned char routerId;
  unsigned char flag;
  const char* producerModel;
  int neighbors[10];  // Assignment specifies that a router can have
                      // maximum 10 connections
};

int printStruct(const struct Router* routerArray, unsigned int N, int routerId);
void getBinaryString(unsigned char c, char* binaryStr);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
