#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_

#define MAX_NEIGHBORS 10

struct Router {
  const unsigned char routerId;
  unsigned char flag;
  const char* producerModel;
  int neighbors[MAX_NEIGHBORS];  // Assignment specifies that a router can have
                                 // maximum 10 connections
};

// A note on constness:
// Printing should not be able to change any input
// When obtaining the binary string we should not be able to change the pointer
int printStruct(const struct Router* const routerArray, const unsigned int N,
                const int routerId);
void getBinaryString(const unsigned char c, char* const binaryStr);

int setNeighbor(const unsigned char fromRouter, const unsigned char toRouter,
                struct Router* const* const routerArray, unsigned int const N);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
