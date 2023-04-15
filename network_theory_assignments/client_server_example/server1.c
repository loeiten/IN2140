/*
 * Assignment:
 *
 * Communicate through a socket by running the binaries of `client1.c` and
 * `server1.c` on different machines.
 * In order to do this, you need to create a file `config.txt` on the form
 *
 * ```text
 * IPV4_address_to_the_server
 * port_to_use
 * ```
 *
 * If you do not have access to two different machines, it's possible to run
 * the two programs as different processes on the same machine.
 * In that case `config.txt` could point to localhost like this
 *
 * ```text
 * 127.0.0.1
 * 8080
 * ```
 *
 * See also explanation and examples in
 * https://www.geeksforgeeks.org/socket-programming-cc/
 * https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 */

#include <arpa/inet.h>   // for htons
#include <netdb.h>       // for getnameinfo, NI_MAXHOST, NI_MAXSERV, NI_...
#include <netinet/in.h>  // for sockaddr_in, INADDR_ANY, IPPROTO_TCP
#include <stdio.h>       // for printf, snprintf
#include <stdlib.h>      // for EXIT_FAILURE, atoi, free, EXIT_SUCCESS
#include <string.h>      // for strerror, strlen
#include <strings.h>     // for bzero
#include <sys/errno.h>   // for errno
#include <sys/socket.h>  // for accept, bind, listen, socket, AF_INET
#include <unistd.h>      // for close, read, write, NULL

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
  printf("Request socket bound on the server side to localhost:%d\n", port);

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
  char ipFromClientAddr[NI_MAXHOST];
  char portFromClientAddr[NI_MAXSERV];
  error = getnameinfo((struct sockaddr *)&clientAddr, clientAddrLen,
                      ipFromClientAddr, sizeof(ipFromClientAddr),
                      portFromClientAddr, sizeof(portFromClientAddr),
                      NI_NUMERICHOST | NI_NUMERICSERV);
  if (error != 0) {
    printf(
        "Could not resolve the server ip and port. It failed with error %d: "
        "%s\n",
        error, gai_strerror(error));
  } else {
    printf("Accepted connection from %s:%s.\n", ipFromClientAddr,
           portFromClientAddr);
  }

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
