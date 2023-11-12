#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL
#include <stdlib.h>  // for EXIT_SUCCESS
#include <string.h>  // for strcmp

void testGetUDPSocket(void) { assert(1 == 0); }

void testGetTCPClientSocket(void) { assert(1 == 0); }

void testSendEdgeInformation(void) { assert(1 == 0); }

void testReceiveRoutingTable(void) { assert(1 == 0); }

void testReceiveAndForwardPackets(void) { assert(1 == 0); }

void testPrepareAndSendPackets(void) { assert(1 == 0); }

void testExtractLengthDestinationAndMessage(void) { assert(1 == 0); }

void testCreatePacket(void) { assert(1 == 0); }

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
    testGetUDPSocket();
  } else if (strcmp(argv[1], "getTCPClientSocket") == 0) {
    testGetTCPClientSocket();
  } else if (strcmp(argv[1], "sendEdgeInformation") == 0) {
    testSendEdgeInformation();
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
