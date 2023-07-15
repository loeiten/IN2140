#include "../include/router.h"

#include <stdio.h>   // for fprintf, stderr, printf, size_t, NULL
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE, strtol

#include "../include/utils.h"  // for getBinaryString

int printRouter(const struct Router* const routerArray, const unsigned int N,
                const int routerId) {
  int hitIdx;
  int success = findRouterId(routerArray, N, routerId, &hitIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not print router as routerId was not found");
    return EXIT_FAILURE;
  }

  // Capture the binary flag
  // 8 for the number of bits
  // +1 for the null terminator
  char binaryFlag[sizeof(unsigned char) * 8 + 1];
  getBinaryString(routerArray[hitIdx].flag, binaryFlag);

  const char* active = (binaryFlag[7] == '1') ? "Yes" : "No";
  const char* wireless = (binaryFlag[6] == '1') ? "Yes" : "No";
  const char* fiveGHz = (binaryFlag[5] == '1') ? "Yes" : "No";
  // Extract the modification number
  const unsigned int kBitLen = 4;
  // This is a fixed size array and not a variable size array (aka VLA) as the
  // size can be determined during compile time
  // Hence, there is no danger of stack-overflow for this variable
  char modificationNumberStr[kBitLen + 1];
  // Reverse the order of bit in the binaryFlag
  for (size_t i = 0; i < kBitLen; ++i) {
    // -1 as we start counting from 0
    modificationNumberStr[i] = binaryFlag[kBitLen - i - 1];
  }
  modificationNumberStr[kBitLen] = '\0';  // Null terminator
  long modificationNumber = strtol(modificationNumberStr, NULL, 2);

  // FIXME: Capture the neighbors

  printf(
      "\nName: %s\nRouterId: %d\nActive: %s\nWireless: %s\n5 GHz: "
      "%s\nModification number: %ld\nFlag: %s (%d)\n",
      routerArray[hitIdx].producerModel, routerId, active, wireless, fiveGHz,
      modificationNumber, binaryFlag, (int)routerArray[hitIdx].flag);
  printNeighbors(routerArray[hitIdx].neighbors);
  return EXIT_SUCCESS;
}

void printNeighbors(const int* const neighbors) {
  char* delim = "";
  printf("Neighbors: [");
  for (int i = 0; i < MAX_NEIGHBORS; ++i) {
    if (neighbors[i] == -1) {
      break;
    }
    printf("%s%d", delim, neighbors[i]);
    delim = ", ";
  }
  printf("]\n");
}

int setNeighbor(const unsigned char fromRouter, const unsigned char toRouter,
                struct Router* const* const routerArray, unsigned int const N) {
  // Find the router which we will update the neighbor with
  int routerIdx;
  int success = findRouterId(*routerArray, N, fromRouter, &routerIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Could not set the neighbor as the routerId was not found\n");
    return EXIT_FAILURE;
  }

  // Find the first free index
  int neighborIdx;
  success = findFreeNeighbor(&((*routerArray)[routerIdx]), &neighborIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Could not set the neighbor as a free neighbor of routerId %d was "
            "not found\n",
            fromRouter);
    return EXIT_FAILURE;
  }

  // Set the neighbor
  return EXIT_SUCCESS;
}

int findRouterId(const struct Router* const routerArray, const unsigned int N,
                 const int routerId, int* const hitIdx) {
  *hitIdx = -1;
  for (size_t i = 0; i < N; ++i) {
    if (((int)routerArray[i].routerId) == routerId) {
      *hitIdx = i;
      break;
    }
  }
  if (*hitIdx == -1) {
    fprintf(stderr, "Could not find routerId %d in routerArray", routerId);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int findFreeNeighbor(const struct Router* const router, int* const hitIdx) {
  *hitIdx = -1;
  for (size_t i = 0; i < MAX_NEIGHBORS; ++i) {
    if (router->neighbors[i] == -1) {
      *hitIdx = i;
      return EXIT_SUCCESS;
    }
  }
  return EXIT_FAILURE;
}
