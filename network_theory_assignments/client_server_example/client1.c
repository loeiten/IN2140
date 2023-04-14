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

#include <arpa/inet.h>   // for inet_addr, htons
#include <netdb.h>       // for getnameinfo, NI_MAXHOST, NI_MAXSERV, NI_...
#include <netinet/in.h>  // for sockaddr_in, IPPROTO_TCP, in_addr
#include <stdio.h>       // for printf
#include <stdlib.h>      // for free, EXIT_FAILURE, EXIT_SUCCESS, atoi
#include <string.h>      // for strerror
#include <strings.h>     // for bzero
#include <sys/errno.h>   // for errno
#include <sys/socket.h>  // for connect, socket, AF_INET, SOCK_STREAM
#include <unistd.h>      // for close, read, write, NULL

#include "include/helper.h"  // for readLine

#define BUFF_LEN 1024

int main() {
  // Declaration of structures
  struct sockaddr_in serverAddr;
  char buff[BUFF_LEN];

  // Erase data by writing 0's to the memory location
  bzero((void *)&serverAddr, sizeof(serverAddr));
  bzero((void *)buff, sizeof(buff));

  // Obtain the ip and port
  char *ip = NULL;
  char *portStr = NULL;
  int success;
  success = readLine("config.txt", &ip, 0);
  if (success != EXIT_SUCCESS) {
    printf("Failed to read line\n");
    return EXIT_FAILURE;
  }
  success = readLine("config.txt", &portStr, 1);
  if (success != EXIT_SUCCESS) {
    printf("Failed to read line\n");
    return EXIT_FAILURE;
  }
  int port = atoi(portStr);
  free(portStr);

  // Create a socket, the integer returned can be thought about as a file handle
  int clientFd =
      socket(AF_INET,       // Use IPV4
             SOCK_STREAM,   // Communication type TCP (SOCK_DGRAM is UDP)
             IPPROTO_TCP);  // What IP protocol to use
  if (clientFd == -1) {
    printf("Failed to create socket.\nError %d: %s\n", errno, strerror(errno));
    return EXIT_FAILURE;
  }
  printf("Socket created on the client side\n");

  // Set the domain to internet
  serverAddr.sin_family = AF_INET;  // Use IPV4
  // Insert the IP address to the server
  serverAddr.sin_addr.s_addr = inet_addr(ip);  // Connect to this ip
  // Set the port number
  // The htons() function converts the unsigned short integer `hostshort` from
  // host byte order to network byte order.
  serverAddr.sin_port = htons(port);
  free(ip);

  // Connect
  int error =
      connect(clientFd, (struct sockaddr *)&serverAddr, sizeof serverAddr);
  if (error != 0) {
    printf("Connection failed.\nError %d: %s\n", errno, strerror(errno));
    return EXIT_FAILURE;
  }
  char ipFromServerAddr[NI_MAXHOST];
  char portFromServerAddr[NI_MAXSERV];
  error = getnameinfo((struct sockaddr *)&serverAddr, sizeof(serverAddr),
                      ipFromServerAddr, sizeof(ipFromServerAddr),
                      portFromServerAddr, sizeof(portFromServerAddr),
                      NI_NUMERICHOST | NI_NUMERICSERV);
  if (error != 0) {
    printf("Could not resolve the server ip and port. Error %d: %s\n", errno,
           strerror(errno));
  } else {
    printf("Socket connected from the client side to %s:%s.\n",
           ipFromServerAddr, portFromServerAddr);
  }

  // Send the data
  write(clientFd, "Hello, world!", BUFF_LEN);

  // Read data from the connection
  read(clientFd, buff, BUFF_LEN);
  printf("From server: %s\n", buff);

  // Close the socket
  close(clientFd);

  return EXIT_SUCCESS;
}
