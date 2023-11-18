#include "../include/common.h"

#include <errno.h>       // for errno
#include <stdio.h>       // for fprintf, stderr
#include <stdlib.h>      // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>      // for strerror
#include <sys/socket.h>  // for recv, ssize_t, send

int getIndexFromAddress(const int address,
                        const struct IndexToAddress* const indexToAddress,
                        int* index) {
  *index = -1;
  for (int i = 0; i < indexToAddress->n; ++i) {
    if (address == indexToAddress->map[i]) {
      *index = i;
      return EXIT_SUCCESS;
    }
  }
  fprintf(stderr, "Could not find address %d in the indexToAddress map\n",
          address);
  return EXIT_FAILURE;
}

int receiveNBytesMessage(const int sockFd, void* buf, const ssize_t nBytes,
                         const int flag) {
  ssize_t bytesReceived = recv(sockFd, buf, nBytes, flag);
  if (bytesReceived == -1) {
    fprintf(stderr, "Receiving failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesReceived != nBytes) {
    fprintf(stderr, "Received less bytes than expected\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int receiveMessage(const int sockFd, void* buf, const int flag) {
  ssize_t bytesReceived = recv(sockFd, buf, MAX_MSG_LENGTH, flag);
  if (bytesReceived == -1) {
    fprintf(stderr, "Receiving failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int sendMessage(const int sockFd, void* buf, const ssize_t nBytes,
                const int flag) {
  ssize_t bytesSent = send(sockFd, buf, nBytes, flag);
  if (bytesSent == -1) {
    fprintf(stderr, "Sending failed.\nError %d: %s\n", errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(stderr, "Sent less bytes than expected\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
