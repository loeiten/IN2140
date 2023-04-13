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

#define BUFF_LEN 1024

int main() {
  // Declaration of structures
  struct sockaddr_in serverAddr, clientAddr;
  socklen_t clientAddrLen;
  char buff[BUFF_LEN];

  // Erase data by writing 0's to the memory location
  bzero((void *)&serverAddr, sizeof(serverAddr));
  bzero((void *)buff, sizeof(buff));

  // Obtain the port
  char *portStr = NULL;
  int success;
  success = readLine("config.txt", &portStr, 1);
  if (success != EXIT_SUCCESS) {
    printf("Failed to read line\n");
    return EXIT_FAILURE;
  }
  int port = atoi(portStr);
  free(portStr);

  // Create a socket, the integer return a file handle
  int serverFd =
      socket(AF_INET,       // Use IPV4
             SOCK_STREAM,   // Communication type TCP (SOCK_DGRAM is UDP)
             IPPROTO_TCP);  // What IP protocol to use
  if (serverFd == -1) {
    printf("Failed to create socket.\nError %d: %s\n", errno, strerror(errno));
    return EXIT_FAILURE;
  }
  printf("Socket created on the server side\n");

  // Create an address structure
  serverAddr.sin_family = AF_INET;          // Use IPV4
  serverAddr.sin_addr.s_addr = INADDR_ANY;  // Localhost
  // The htons() function converts the unsigned short integer `hostshort` from
  // host byte order to network byte order.
  serverAddr.sin_port = htons(port);

  // Bind the socket to the address and port number
  int error = bind(serverFd, (struct sockaddr *)&serverAddr, sizeof serverAddr);
  if (error != 0) {
    printf("Binding socket failed.\nError %d: %s\n", errno, strerror(errno));
    return EXIT_FAILURE;
  }
  printf("Request socket bound to localhost, port %d on the server side\n",
         port);

  // Activate listening on the socket
  error = listen(serverFd,
                 SOMAXCONN);  // Max length fo queue pending connection to the
                              // request socket
  if (error != 0) {
    printf("Listening failed.\nError %d: %s\n", errno, strerror(errno));
    return EXIT_FAILURE;
  }
  printf("Socket is now listening on the server side\n");

  // Accept a connection
  // Extract first connection request from the queue and return a new file
  // descriptor referring to that socket
  int newSocketFd = accept(
      serverFd,
      (struct sockaddr
           *)&clientAddr,  // Will be filled with the address of the peer socket
      &clientAddrLen);
  if (serverFd == -1) {
    printf("Accepting request failed.\nError %d: %s\n", errno, strerror(errno));
    return EXIT_FAILURE;
  }
  printf("Accepted connection\n");

  // Read data from the fd, and write it out
  read(newSocketFd, buff, BUFF_LEN);
  printf("From client: %s\n", buff);

  // Reply to the client
  // Send data back to the connection through the fd
  bzero((void *)buff, sizeof(buff));
  char *msg = "Message received by the server\n";
  int strBufferLen = strlen(msg) + 1;
  snprintf(buff, strBufferLen, "%s", msg);
  printf("To client: %s\n", buff);
  write(newSocketFd, buff, BUFF_LEN);

  // Close the sockets
  close(serverFd);
  close(newSocketFd);

  return EXIT_SUCCESS;
}
