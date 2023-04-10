#include <arpa/inet.h>   // for htons
#include <netinet/in.h>  // for sockaddr_in, INADDR_ANY, IPPROTO_TCP
#include <stdio.h>       // for printf
#include <stdlib.h>      // for atoi, free, EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>      // for strerror
#include <strings.h>     // for bzero
#include <sys/errno.h>   // for errno
#include <sys/socket.h>  // for accept, bind, listen, socket, AF_INET
#include <unistd.h>      // for close, read, write

#include "include/helper.h"  // for readLine

#define MSG_LEN 13

int main() {
  // Declaration of structures
  struct sockaddr_in serverAddr, clientAddr;
  socklen_t clientAddrLen;
  int requestSock, sock;
  char buf[MSG_LEN + 1];

  // Obtain the port
  char *portStr = NULL;
  int success;
  success = readLine("config.txt", &portStr, 1);
  if (success != EXIT_SUCCESS) {
    printf("Failed to read line\n");
    return EXIT_FAILURE;
  }

  // Create a request socket
  requestSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Create an address structure
  bzero((void *)&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  int port = atoi(portStr);
  free(portStr);
  serverAddr.sin_port = htons(port);

  // Bind the address to the socket
  int error =
      bind(requestSock, (struct sockaddr *)&serverAddr, sizeof serverAddr);
  if (error != 0) {
    printf("Connection failed: %s", strerror(errno));
  }

  // Activate listening on the socket
  listen(requestSock, SOMAXCONN);

  // Accept a connection
  sock = accept(requestSock, (struct sockaddr *)&clientAddr, &clientAddrLen);

  // Read data from the connection, and write it out
  read(sock, buf, MSG_LEN);
  buf[MSG_LEN] = '\0';
  printf("%s \n", buf);

  // Send data back to the connection
  write(sock, buf, MSG_LEN);

  // Close the sockets
  close(sock);
  close(requestSock);

  return EXIT_SUCCESS;
}
