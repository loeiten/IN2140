#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL
#include <stdlib.h>  // for EXIT_SUCCESS
#include <string.h>  // for strcmp
#include <unistd.h>  // for recv, send, MSG_WAITALL

#include "../routing_server/include/routing_server_communication.h"
#include "../utils/include/common.h"
#include "../utils/include/dynamic_memory.h"
#include "include/test_tools.h"

void openTCP(const char* basePortStr) {
  int listenSocket = -1;
  int basePort = atoi(basePortStr);
  int success = getTCPServerSocket(&listenSocket, basePort);
  assert(success == EXIT_SUCCESS);
  assert(listenSocket != 1);

  // Accept a connection
  int newSocketFd = -1;
  success = acceptConnection(listenSocket, &newSocketFd);
  if (newSocketFd == -1) {
    close(listenSocket);
  }
  assert(success == EXIT_SUCCESS);
  assert(newSocketFd != 1);

  char msg[MAX_MSG_LENGTH];
  while (1) {
    // NOTE: Do NOT use MSG_WAITALL as we don't need to fill the whole buffer
    success = receiveMessage(newSocketFd, msg, 0);
    if (success != EXIT_SUCCESS) {
      close(listenSocket);
      close(newSocketFd);
    }
    assert(success == EXIT_SUCCESS);
    printf("Received: %s\n", msg);
    if (strcmp(msg, "QUIT") == 0) {
      return;
    }
  }
}

void testGetTCPServerSocket(const char* basePortStr) {
  int listenSocket = -1;
  int basePort = atoi(basePortStr);
  int success = getTCPServerSocket(&listenSocket, basePort);
  if ((success != EXIT_SUCCESS) && (listenSocket == -1)) {
    close(listenSocket);
  }
  assert(success == EXIT_SUCCESS);
  assert(listenSocket != -1);
  close(listenSocket);
}

void testPopulateNodeArrayAndSendRoutingTables(const char* basePortStr) {
  // NOTE: We combine the test of populateNodeArray and sendRoutingTables as
  //       the new sockets created during accept in the populateNodeArray is
  //       reused

  // We're working with the following graph:
  //    7   22
  //  o - o - o
  // 101  15 42

  // Test populateNodeArray
  // ===========================================================================
#define N (3)
  int listenSocket;
  int basePort = atoi(basePortStr);
  int success = getTCPServerSocket(&listenSocket, basePort);
  assert(success == EXIT_SUCCESS);
  if ((success != EXIT_SUCCESS) && (listenSocket == -1)) {
    close(listenSocket);
  }
  assert(success == EXIT_SUCCESS);
  struct Node* nodeArray = NULL;
  allocateNodeArray(&nodeArray, N, "testPopulateNodeArray");
  success = populateNodeArray(listenSocket, nodeArray, N);
  if (success != EXIT_SUCCESS) {
    freeNodeArray(&nodeArray, N);
    close(listenSocket);
  }
  assert(success == EXIT_SUCCESS);
  // NOTE: We don't know what order the nodes are sent to us, therefore we make
  //       a map in order to tie the ordering Assert node 0
  int indexMap[N];
  int addressMap[N];
  for (int i = 0; i < N; ++i) {
    if (nodeArray[i].address == 101) {
      addressMap[0] = i;
      indexMap[addressMap[0]] = 101;
    } else if (nodeArray[i].address == 15) {
      addressMap[1] = i;
      indexMap[addressMap[1]] = 15;
    } else if (nodeArray[i].address == 42) {
      addressMap[2] = i;
      indexMap[addressMap[2]] = 42;
    } else {
      fprintf(stderr, "nodeArray[%d].address = %d was not expected", i,
              nodeArray[i].address);
      freeNodeArray(&nodeArray, N);
      close(listenSocket);
      return;
    }
  }
  // Assert address 101
  assert(nodeArray[addressMap[0]].tcpSocket != -1);
  assert(nodeArray[addressMap[0]].address == 101);
  assert(nodeArray[addressMap[0]].nNeighbors == 1);
  assert(nodeArray[addressMap[0]].neighborAddresses[0] == 15);
  assert(nodeArray[addressMap[0]].edgeWeights[0] == 7);
  // Assert address 15
  assert(nodeArray[addressMap[1]].tcpSocket != -1);
  assert(nodeArray[addressMap[1]].address == 15);
  assert(nodeArray[addressMap[1]].nNeighbors == 2);
  assert(nodeArray[addressMap[1]].neighborAddresses[0] == 101);
  assert(nodeArray[addressMap[1]].neighborAddresses[1] == 42);
  assert(nodeArray[addressMap[1]].edgeWeights[0] == 7);
  assert(nodeArray[addressMap[1]].edgeWeights[1] == 22);
  // Assert address 42
  assert(nodeArray[addressMap[2]].tcpSocket != -1);
  assert(nodeArray[addressMap[2]].address == 42);
  assert(nodeArray[addressMap[2]].nNeighbors == 1);
  assert(nodeArray[addressMap[2]].neighborAddresses[0] == 15);
  assert(nodeArray[addressMap[2]].edgeWeights[0] == 22);
  printf("NodeArray received successfully!\n");
  // ===========================================================================

  // Test sendRoutingTables
  // ===========================================================================
  struct IndexToAddress indexToAddress;
  indexToAddress.n = N;
  indexToAddress.map = indexMap;
  // Manually create the routingTableArray
  struct RoutingTableArray routingTableArray;
  success = createIGraphRoutingTableArray(
      &routingTableArray, addressMap,
      "testPopulateNodeArrayAndSendRoutingTables");
  if (success != EXIT_SUCCESS) {
    freeNodeArray(&nodeArray, N);
    freeRoutingTableArray(&routingTableArray);
    close(listenSocket);
  }
  assert(success == EXIT_SUCCESS);

  success = sendRoutingTables(nodeArray, &routingTableArray, &indexToAddress);
  if (success != EXIT_SUCCESS) {
    freeNodeArray(&nodeArray, N);
    freeRoutingTableArray(&routingTableArray);
    close(listenSocket);
  }
  assert(success == EXIT_SUCCESS);
  freeNodeArray(&nodeArray, N);
  freeRoutingTableArray(&routingTableArray);
  printf("RoutingArray sent successfully!\n");
  close(listenSocket);
  // ===========================================================================
#undef N
}

void testTranslateTableFromIdxToAddress(void) {
  // We're working with the following graph:
  //    7   22
  //  o - o - o
  // 101  15 42
#define N (3)
  struct RoutingTableArray routingTableArray;
  const int addressMap[N] = {0, 1, 2};
  int indexMap[N] = {101, 15, 42};
  struct IndexToAddress indexToAddress;
  indexToAddress.n = N;
  indexToAddress.map = indexMap;
  int success = createIGraphRoutingTableArray(
      &routingTableArray, addressMap, "testTranslateTableFromIdxToAddress");
  assert(success == EXIT_SUCCESS);

  struct RoutingTable addressRoutingTable;
  addressRoutingTable.routingTableRows = NULL;

  // Address 101
  success =
      translateTableFromIdxToAddress(&(routingTableArray.routingTables[0]),
                                     &indexToAddress, &addressRoutingTable);
  assert(success == EXIT_SUCCESS);
  assert(addressRoutingTable.nRows == 2);
  assert(addressRoutingTable.routingTableRows[0].destination == 15);
  assert(addressRoutingTable.routingTableRows[0].nextHop == 15);
  assert(addressRoutingTable.routingTableRows[1].destination == 42);
  assert(addressRoutingTable.routingTableRows[1].nextHop == 15);

  // Address 15
  success =
      translateTableFromIdxToAddress(&(routingTableArray.routingTables[1]),
                                     &indexToAddress, &addressRoutingTable);
  assert(success == EXIT_SUCCESS);
  assert(addressRoutingTable.nRows == 2);
  assert(addressRoutingTable.routingTableRows[0].destination == 101);
  assert(addressRoutingTable.routingTableRows[0].nextHop == 101);
  assert(addressRoutingTable.routingTableRows[1].destination == 42);
  assert(addressRoutingTable.routingTableRows[1].nextHop == 42);

  // Address 42
  success =
      translateTableFromIdxToAddress(&(routingTableArray.routingTables[2]),
                                     &indexToAddress, &addressRoutingTable);
  assert(success == EXIT_SUCCESS);
  assert(addressRoutingTable.nRows == 2);
  assert(addressRoutingTable.routingTableRows[0].destination == 101);
  assert(addressRoutingTable.routingTableRows[0].nextHop == 15);
  assert(addressRoutingTable.routingTableRows[1].destination == 15);
  assert(addressRoutingTable.routingTableRows[1].nextHop == 15);

#undef N
  freeRoutingTableArray(&routingTableArray);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "getTCPServerSocket") == 0) {
    testGetTCPServerSocket(argv[2]);
  } else if (strcmp(argv[1], "populateNodeArrayAndSendRoutingTables") == 0) {
    testPopulateNodeArrayAndSendRoutingTables(argv[2]);
  } else if (strcmp(argv[1], "translateTableFromIdxToAddress") == 0) {
    testTranslateTableFromIdxToAddress();
  } else if (strcmp(argv[1], "openTCP") == 0) {
    openTCP(argv[2]);
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
