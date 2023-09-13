#include "../include/route.h"

#include "print_lib.h"

void printEdges(const int *const distanceArray,
                const struct Route *const routeArray, const int n) {
  (void)distanceArray;
  (void)routeArray;
  (void)n;
  print_weighted_edge(0, 1, 2);
}
