#include "../include/node_parser.h"

#include <stdio.h>   // for fprintf, NULL, stderr, snprintf
#include <stdlib.h>  // for free, malloc, EXIT_FAILURE
#include <string.h>  // for strtok_r, strlen

#include "../../utils/include/common.h"  // for CommunicatedNode

int parseNodes(const char *const *const nodesStr, const int nNeighbors,
               struct CommunicatedNode *communicatedNode) {
  for (int i = 0; i < nNeighbors; ++i) {
    // Initialize memory to be allocated
    char *nodesStrCpy = NULL;

    // Initialize helper variables
    const char *format = "%s";

    // Declare helper variables
    int strLen;
    int charWritten;  // Number of chars not including terminating char
    char *token;
    char *savePtr;

    // Copy the commandStr as strtok_r will alter the string
    strLen = snprintf(NULL, 0, format, nodesStr[i]);
    nodesStrCpy = (char *)malloc((strLen + 1) * sizeof(char));
    if (nodesStrCpy == NULL) {
      perror("Could not allocate memory to nodesStrCpy: ");
      return EXIT_FAILURE;
    }
    charWritten = snprintf(nodesStrCpy, (strLen + 1), format, nodesStr[i]);
    if (charWritten != strLen) {
      fprintf(stderr, "Could not copy from nodesStr to nodesStrCpy\n");
      free(nodesStrCpy);
      nodesStrCpy = NULL;
      return EXIT_FAILURE;
    }

    // Find the :
    // NOTE: strtok_r is the thread safe version of strtok
    token = strtok_r(nodesStrCpy, ":", &savePtr);
    strLen = snprintf(NULL, 0, format, nodesStrCpy);
    if (strLen < 1) {
      fprintf(stderr, "Failed to get the len of NeighborAddress\n");
      free(nodesStrCpy);
      nodesStrCpy = NULL;
      return EXIT_FAILURE;
    }

    // Copy to neighborAddressStr
    char *neighborAddressStr = (char *)malloc((strLen + 1) * sizeof(char));
    if (neighborAddressStr == NULL) {
      fprintf(stderr, "Failed to allocate neighborAddressStr\n");
      free(nodesStrCpy);
      nodesStrCpy = NULL;
      return EXIT_FAILURE;
    }
    charWritten = snprintf(neighborAddressStr, strLen + 1, format, token);
    if (charWritten != strLen) {
      fprintf(stderr, "Could not copy from token to neighborAddressStr\n");
      free(nodesStrCpy);
      nodesStrCpy = NULL;
      free(neighborAddressStr);
      neighborAddressStr = NULL;
      return EXIT_FAILURE;
    }

    // Copy to weightStr
    token = strtok_r(NULL, " ", &savePtr);
    strLen = strlen(token);
    char *weightStr = (char *)malloc((strLen + 1) * sizeof(char));
    if (weightStr == NULL) {
      fprintf(stderr, "Failed to allocate weightStr\n");
      free(nodesStrCpy);
      nodesStrCpy = NULL;
      free(neighborAddressStr);
      neighborAddressStr = NULL;
      return EXIT_FAILURE;
    }
    charWritten = snprintf(weightStr, strLen + 1, format, token);
    if (charWritten != strLen) {
      fprintf(stderr, "Could not copy from token to weightStr\n");
      free(nodesStrCpy);
      nodesStrCpy = NULL;
      free(neighborAddressStr);
      neighborAddressStr = NULL;
      free(weightStr);
      weightStr = NULL;
      return EXIT_FAILURE;
    }

    // Convert to ints and store in communicatedNode
    communicatedNode->neighborAddresses[i] = atoi(neighborAddressStr);
    communicatedNode->edgeWeights[i] = atoi(weightStr);
  }

  return EXIT_SUCCESS;
}
