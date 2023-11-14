#include <arpa/inet.h>  // for ntohs
#include <assert.h>     // for assert
#include <libgen.h>     // for basename
#include <stdio.h>      // for fprintf, stderr
#include <stdlib.h>     // for free, EXIT_SUCCESS
#include <string.h>     // for strcmp, memcpy, strndup
#include <time.h>
#include <unistd.h>  // for close, sleep

#include "../node/include/node_communication.h"  // for createPacket, extrac...
#include "../utils/include/common.h"             // for sendMessage

void testGetUDPSocket(const char* basePortStr) {
  int connectSocket = -1;
  int basePort = atoi(basePortStr);
  int success = getUDPSocket(&connectSocket, basePort);
  if ((success != EXIT_SUCCESS) && (connectSocket != -1)) {
    close(connectSocket);
  }
  assert(success == EXIT_SUCCESS);
  assert(connectSocket > 0);
  close(connectSocket);
}

void testGetTCPClientSocket(const char* basePortStr) {
  // Wait until the server is set up
  sleep(1);

  int clientSocket = -1;
  int basePort = atoi(basePortStr);
  int success = getTCPClientSocket(&clientSocket, basePort);
  if ((success != EXIT_SUCCESS) && (clientSocket != -1)) {
    close(clientSocket);
  }
  assert(success == EXIT_SUCCESS);
  assert(clientSocket > 0);
  const char* message = "QUIT";
  // +1 for terminating /0 character
  success = sendMessage(clientSocket, (void*)message, strlen(message) + 1, 0);
  if ((success != EXIT_SUCCESS) && (clientSocket != -1)) {
    close(clientSocket);
  }
  assert(success == EXIT_SUCCESS);
  close(clientSocket);
}

void testSendEdgeInformation(const char* basePortStr, const char* addressStr) {
  // Wait until the server is set up
  sleep(1);

  // We're working with the following graph:
  //    7   22
  //  o - o - o
  // 101  15 42
  int basePort = atoi(basePortStr);
  int address = atoi(addressStr);

  struct Node node = {.tcpSocket = -1, .address = address};

  int clientSocket = -1;
  int success = getTCPClientSocket(&clientSocket, basePort);
  if ((success != EXIT_SUCCESS) && (clientSocket != -1)) {
    close(clientSocket);
  }
  assert(success == EXIT_SUCCESS);
  node.tcpSocket = clientSocket;
  // Declare variable here so that they are in scope
  int neighborAddresses[2];
  int edgeWeights[2];
  if (address == 101) {
    node.nNeighbors = 1;
    neighborAddresses[0] = 15;
    edgeWeights[0] = 7;
    node.neighborAddresses = neighborAddresses;
    node.edgeWeights = edgeWeights;
  } else if (address == 15) {
    node.nNeighbors = 2;
    neighborAddresses[0] = 101;
    neighborAddresses[1] = 42;
    edgeWeights[0] = 7;
    edgeWeights[1] = 22;
    node.neighborAddresses = neighborAddresses;
    node.edgeWeights = edgeWeights;
  } else if (address == 42) {
    node.nNeighbors = 1;
    neighborAddresses[0] = 15;
    edgeWeights[0] = 22;
    node.neighborAddresses = neighborAddresses;
    node.edgeWeights = edgeWeights;
  } else {
    fprintf(stderr, "address = %d was not expected", address);
    return;
  }
  success = sendEdgeInformation(&node);
  if ((success != EXIT_SUCCESS) && (clientSocket != -1)) {
    close(clientSocket);
  }
  assert(success == EXIT_SUCCESS);
  close(node.tcpSocket);
}

void testReceiveRoutingTable(void) { assert(1 == 0); }

void testReceiveAndForwardPackets(void) { assert(1 == 0); }

void testPrepareAndSendPackets(void) { assert(1 == 0); }

void testExtractLengthDestinationAndMessage(void) {
  const char* line1 = "814 Good luck with the home exam\n";
  unsigned short length;
  unsigned short destination;
  char* msg = NULL;
  int success =
      extractLengthDestinationAndMessage(line1, &length, &destination, &msg);
  assert(success == EXIT_SUCCESS);
  assert(length == 35);
  assert(destination == 814);
  assert(strcmp(msg, "Good luck with the home exam") == 0);
  free(msg);
  msg = NULL;

  const char* line2 = "12 Foo bar baz\n";
  success =
      extractLengthDestinationAndMessage(line2, &length, &destination, &msg);
  assert(success == EXIT_SUCCESS);
  assert(length == 18);
  assert(destination == 12);
  assert(strcmp(msg, "Foo bar baz") == 0);
  free(msg);
  msg = NULL;
}

void testCreatePacket(void) {
  unsigned short length = 35;
  unsigned short destination = 814;
  unsigned short source = 1;
  const char* msg1 = "Good luck with the home exam";
  char* packet = NULL;
  int success = createPacket(length, destination, source, msg1, &packet);
  assert(success == EXIT_SUCCESS);
  unsigned short tmp;
  // Assert length
  memcpy(&tmp, &(packet[0]), sizeof(tmp));
  unsigned short readLength = ntohs(tmp);
  assert(length == readLength);
  // Assert destination
  memcpy(&tmp, &(packet[2]), sizeof(tmp));
  unsigned short readDestination = ntohs(tmp);
  assert(destination == readDestination);
  // Assert source
  memcpy(&tmp, &(packet[4]), sizeof(tmp));
  unsigned short readSource = ntohs(tmp);
  assert(source == readSource);
  // Assert message
  char* readMsg = NULL;
  readMsg = strndup(&(packet[6]), length - 6);
  assert(strcmp(msg1, readMsg) == 0);
  free(readMsg);
  readMsg = NULL;
  free(packet);
  packet = NULL;

  const char* msg2 = "Foo bar baz";
  length = 18;
  destination = 2;
  source = 1973;
  success = createPacket(length, destination, source, msg2, &packet);
  assert(success == EXIT_SUCCESS);
  // Assert length
  memcpy(&tmp, &(packet[0]), sizeof(tmp));
  readLength = ntohs(tmp);
  assert(length == readLength);
  // Assert destination
  memcpy(&tmp, &(packet[2]), sizeof(tmp));
  readDestination = ntohs(tmp);
  assert(destination == readDestination);
  // Assert source
  memcpy(&tmp, &(packet[4]), sizeof(tmp));
  readSource = ntohs(tmp);
  assert(source == readSource);
  // Assert message
  readMsg = strndup(&(packet[6]), length - 6);
  assert(strcmp(msg2, readMsg) == 0);
  free(readMsg);
  readMsg = NULL;
  free(packet);
  packet = NULL;
}

void testSendUDPPacket(void) { assert(1 == 0); }

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

  if (strcmp(argv[1], "getUDPSocket") == 0) {
    testGetUDPSocket(argv[2]);
  } else if (strcmp(argv[1], "getTCPClientSocket") == 0) {
    testGetTCPClientSocket(argv[2]);
  } else if (strcmp(argv[1], "sendEdgeInformation") == 0) {
    testSendEdgeInformation(argv[2], argv[3]);
  } else if (strcmp(argv[1], "receiveRoutingTable") == 0) {
    testReceiveRoutingTable();
  } else if (strcmp(argv[1], "receiveAndForwardPackets") == 0) {
    testReceiveAndForwardPackets();
  } else if (strcmp(argv[1], "prepareAndSendPackets") == 0) {
    testPrepareAndSendPackets();
  } else if (strcmp(argv[1], "extractLengthDestinationAndMessage") == 0) {
    testExtractLengthDestinationAndMessage();
  } else if (strcmp(argv[1], "createPacket") == 0) {
    testCreatePacket();
  } else if (strcmp(argv[1], "sendUDPPacket") == 0) {
    testSendUDPPacket();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
