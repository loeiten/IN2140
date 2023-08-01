#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_

#define MAX_NEIGHBORS (10)

struct Router {
  unsigned char routerId;  // The routerId can change when we move during delete
  unsigned char flag;
  const char* producerModel;
  int neighbors[MAX_NEIGHBORS];  // Assignment specifies that a router can have
                                 // maximum 10 connections
};

// A note on constness:
// Printing should not be able to change any input
// When obtaining the binary string we should not be able to change the pointer
int printRouter(const struct Router* const routerArray, const unsigned int N,
                const int routerId);
void printNeighbors(const int* const neighbors);

int addLink(struct Router* const routerArray, const unsigned int N,
            const unsigned char fromRouter, const unsigned char toRouter);

int findRouterId(const struct Router* const routerArray, const unsigned int N,
                 const int routerId, int* const hitIdx);

int findFreeNeighbor(const struct Router* const router, int* const hitIdx);

int setFlag(struct Router* const routerArray, const unsigned int N,
            const int routerId, const int flag, const int value);

// NOTE: Instead of moving every element, this function copies the last element
//       of the array into the "hole" created by deletion
//       This can be beneficial if the array is big and one of the first
//       elements are being deleted
int deleteRouter(struct Router** routerArray, unsigned int* const N,
                 const int routerId);

int setModel(struct Router** routerArray, unsigned int const N,
             const int routerId, const char* const name);

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
