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

  printf(
      "Name: %s\nRouterId: %d\nActive: %s\nWireless: %s\n5 GHz: "
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

int setNeighbor(struct Router* const routerArray, const unsigned int N,
                const unsigned char fromRouter, const unsigned char toRouter) {
  // Find the router which we will update the neighbor with
  int routerIdx;
  int success = findRouterId(routerArray, N, fromRouter, &routerIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Could not set the neighbor as the routerId was not found\n");
    return EXIT_FAILURE;
  }

  // Find the first free index
  int neighborIdx;
  success = findFreeNeighbor(&(routerArray[routerIdx]), &neighborIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Could not set the neighbor as a free neighbor of routerId %d was "
            "not found\n",
            fromRouter);
    return EXIT_FAILURE;
  }

  // Set the neighbor
  routerArray[routerIdx].neighbors[neighborIdx] = (int)toRouter;
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
    fprintf(stderr, "Could not find routerId %d in routerArray\n", routerId);
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

int setFlag(struct Router* const routerArray, const unsigned int N,
            const int routerId, const int flag, const int value) {
  // Sanity checks
  if ((flag < 0) || (flag > 4) || (flag == 3)) {
    fprintf(stderr, "Flag can only be one of {0, 1, 2, 4}\n");
    return EXIT_FAILURE;
  }
  if ((flag < 3) && ((value < 0) || (value > 1))) {
    fprintf(stderr, "Value can only be {0, 1} for flag {0, 1, 2}\n");
    return EXIT_FAILURE;
  }
  if ((value < 0) || (value > 15)) {
    fprintf(stderr, "Value can only be in range [0, 15]\n");
    return EXIT_FAILURE;
  }

  // Find the correct index
  int hitIdx;
  int success = findRouterId(routerArray, N, routerId, &hitIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not set flag as routerId was not found");
    return EXIT_FAILURE;
  }

  // Modify the flag
  if (flag < 3) {
    if (value == 1) {
      // Set bit
      routerArray[hitIdx].flag |= 1 << flag;
    } else {
      // Clear bit
      routerArray[hitIdx].flag &= ~(1 << flag);
    }
  } else if (flag == 4) {
    routerArray[hitIdx].flag += value << 4;
  }

  return EXIT_SUCCESS;
}
