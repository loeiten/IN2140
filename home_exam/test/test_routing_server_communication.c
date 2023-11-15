#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL
#include <stdlib.h>  // for EXIT_SUCCESS
#include <string.h>  // for strcmp
#include <unistd.h>  // for recv, send, MSG_WAITALL

#include "../routing_server/include/routing_server_communication.h"
#include "../utils/include/common.h"
#include "../utils/include/dynamic_memory.h"

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

void testPopulateNodeArray(const char* basePortStr) {
  // We're working with the following graph:
  //    7   22
  //  o - o - o
  // 101  15 42
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
  }
  assert(success == EXIT_SUCCESS);
  // NOTE: We don't know what order the nodes are sent to us, therefore we make
  //       a map in order to tie the ordering Assert node 0
  int indexMap[N];
  for (int i = 0; i < N; ++i) {
    if (nodeArray[i].address == 101) {
      indexMap[0] = i;
    } else if (nodeArray[i].address == 15) {
      indexMap[1] = i;
    } else if (nodeArray[i].address == 42) {
      indexMap[2] = i;
    } else {
      fprintf(stderr, "nodeArray[%d].address = %d was not expected", i,
              nodeArray[i].address);
      freeNodeArray(&nodeArray, N);
      return;
    }
  }
  // Assert address 101
  assert(nodeArray[indexMap[0]].tcpSocket != -1);
  assert(nodeArray[indexMap[0]].address == 101);
  assert(nodeArray[indexMap[0]].nNeighbors == 1);
  assert(nodeArray[indexMap[0]].neighborAddresses[0] == 15);
  assert(nodeArray[indexMap[0]].edgeWeights[0] == 7);
  // Assert address 15
  assert(nodeArray[indexMap[1]].tcpSocket != -1);
  assert(nodeArray[indexMap[1]].address == 15);
  assert(nodeArray[indexMap[1]].nNeighbors == 2);
  assert(nodeArray[indexMap[1]].neighborAddresses[0] == 101);
  assert(nodeArray[indexMap[1]].neighborAddresses[1] == 42);
  assert(nodeArray[indexMap[1]].edgeWeights[0] == 7);
  assert(nodeArray[indexMap[1]].edgeWeights[1] == 22);
  // Assert address 42
  assert(nodeArray[indexMap[2]].tcpSocket != -1);
  assert(nodeArray[indexMap[2]].address == 42);
  assert(nodeArray[indexMap[2]].nNeighbors == 1);
  assert(nodeArray[indexMap[2]].neighborAddresses[0] == 15);
  assert(nodeArray[indexMap[2]].edgeWeights[0] == 22);
  freeNodeArray(&nodeArray, N);
#undef N
  printf("NodeArray received successfully!\n");
}

void testSendRoutingTables(void) { assert(1 == 0); }

void testTranslateTableFromIdxToAddress(void) { assert(1 == 0); }

int main(int argc, char** argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n1", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "getTCPServerSocket") == 0) {
    testGetTCPServerSocket(argv[2]);
  } else if (strcmp(argv[1], "populateNodeArray") == 0) {
    testPopulateNodeArray(argv[2]);
  } else if (strcmp(argv[1], "sendRoutingTables") == 0) {
    testSendRoutingTables();
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
