#include "../include/router.h"

#include <stdio.h>   // for fprintf, stderr, printf, size_t, NULL
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE, strtol

#include "../include/dynamic_memory.h"  // for
#include "../include/utils.h"           // for getBinaryString

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

int addLink(struct Router* const routerArray, const unsigned int N,
            const unsigned char fromRouter, const unsigned char toRouter) {
  // Find the router which we will update the neighbor with
  int routerIdx;
  int success = findRouterId(routerArray, N, fromRouter, &routerIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not add the link as the routerId was not found\n");
    return EXIT_FAILURE;
  }

  // Find the first free index
  int neighborIdx;
  success = findFreeNeighbor(&(routerArray[routerIdx]), &neighborIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Could not add the link as a free neighbor of routerId %d was "
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
  // NOTE: The current algorithm is an O(N) algorithm
  //       If there are a lot of searches it can be improved by inputting a
  //       sorted array, and use a binary search for O(log N)
  //       However, the best possible case would be to use a hash-map, in which
  //       The complexity would be O(1)
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
    fprintf(stderr, "Value can only be in range [0, 15] for flag 4\n");
    return EXIT_FAILURE;
  }

  // Find the correct index
  int hitIdx;
  int success = findRouterId(routerArray, N, routerId, &hitIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Could not set the flag as routerId %d was not found\n",
            routerId);
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
    // We must reverse the bits in the value as we are dealing with LSb 0
    // numbering
    int reversedValue = 0;
    // Loop through each bit from least to most significant bit
    for (int i = 0; i < 4; i++) {
      // Extract the i-th bit of num and append it to reversedNum
      reversedValue |= ((value >> i) & 1) << (3 - i);
    }
    // Mask any existing bits (do an OR with 0000 1111)
    routerArray[hitIdx].flag &= 0x0F;
    // Add the new bits
    routerArray[hitIdx].flag += reversedValue << 4;
  }

  return EXIT_SUCCESS;
}

int deleteRouter(struct Router** routerArray, unsigned int* const N,
                 const int routerId) {
  // Find the correct index
  int hitIdx;
  int success = findRouterId(*routerArray, *N, routerId, &hitIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Could not delete the router as routerId %d was not found\n",
            routerId);
    return EXIT_FAILURE;
  }

  // Delete all references
  for (int r = 0; r < *N; ++r) {
    for (int i = 0; i < MAX_NEIGHBORS; ++i) {
      if ((*routerArray)[r].neighbors[i] == -1) {
        break;
      }
      if ((*routerArray)[r].neighbors[i] == routerId) {
        if (((i == 0) && ((*routerArray)[r].neighbors[1] == -1)) ||
            (i == (MAX_NEIGHBORS - 1))) {
          // If this is the only or last of the neighbors:
          // Simply delete the reference
          (*routerArray)[r].neighbors[i] = -1;
        } else {
          // Else:
          // Copy the last neighbour to the "hole" and set the "hole" to -1
          int lastNeighbor;
          if ((*routerArray)[r].neighbors[MAX_NEIGHBORS - 1] == -1) {
            success = findFreeNeighbor(&((*routerArray)[r]), &lastNeighbor);
            if (success != EXIT_SUCCESS) {
              fprintf(stderr,
                      "Deletion of router with routerId %d failed as "
                      "findFreeNeighbor failed for router with routerId %d\n",
                      routerId, (int)(*routerArray)[r].routerId);
              return EXIT_FAILURE;
            }
          } else {
            // All the neighbors are filled
            lastNeighbor = MAX_NEIGHBORS;
          }

          // Decrement as findFreeNeighbor finds the first available neighbor
          --lastNeighbor;
          (*routerArray)[r].neighbors[i] =
              (*routerArray)[r].neighbors[lastNeighbor];
          (*routerArray)[r].neighbors[lastNeighbor] = -1;
        }
        break;
      }
    }
  }

  // Update N
  --(*N);

  // Free the dynamically created string
  free((void*)(*routerArray)[hitIdx].producerModel);
  (*routerArray)[hitIdx].producerModel = NULL;

  // Delete the router itself and fill the "hole" in the array
  if (!(((hitIdx == 0) && (*N == 1)) || (hitIdx == (*N)))) {
    // The hitIdx is not the only in the array, nor is it the last element
    // Hence, we must move the last element to the "hole"
    (*routerArray)[hitIdx] = (*routerArray)[(*N)];
  }
  // Reallocate the array
  // NOTE: Reallocate is not strictly necessary, and includes copying of
  //       elements
  //       When deleting, reallocation can be useful if a lot of element are
  //       being deleted
  struct Router* tmp =
      (struct Router*)realloc(*routerArray, (*N) * sizeof(struct Router));

  if (tmp == NULL) {
    perror("Could not reallocate routerArray during deletion: ");
    return EXIT_FAILURE;
  }

  // Assign router array to the temporary variable
  *routerArray = tmp;

  return EXIT_SUCCESS;
}

int setModel(struct Router** routerArray, unsigned int const N,
             const int routerId, const char* const name) {
  // Find the correct index
  int hitIdx;
  int success = findRouterId(*routerArray, N, routerId, &hitIdx);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr,
            "Could not set the model as the routerId %d was not found\n",
            routerId);
    return EXIT_FAILURE;
  }

  // Reallocate the string
  // Dynamically allocate the producerModels
  int length = snprintf(NULL, 0, "%s", name);
  char* producerModel = NULL;
  if (routerArray[hitIdx]->producerModel == NULL) {
    producerModel = malloc(length + 1);
    if (producerModel == NULL) {
      perror("Could not allocate memory to producerModel in the setModel: ");
      return EXIT_FAILURE;
    }
  } else {
    producerModel = realloc((void*)routerArray[hitIdx]->producerModel,
                            (length + 1) * sizeof(char));
    if (producerModel == NULL) {
      perror("Could not reallocate memory to producerModel in the setModel: ");
      return EXIT_FAILURE;
    }
  }
  snprintf(producerModel, length + 1, "%s", name);
  routerArray[hitIdx]->producerModel = producerModel;

  return EXIT_SUCCESS;
}
