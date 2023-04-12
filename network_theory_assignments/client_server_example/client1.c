#include <arpa/inet.h>   // for inet_addr, htons
#include <netinet/in.h>  // for sockaddr_in, IPPROTO_TCP, in_addr
#include <stdio.h>       // for printf
#include <stdlib.h>      // for free, EXIT_SUCCESS, atoi, EXIT_FAILURE
#include <string.h>      // for strerror
#include <strings.h>     // for bzero
#include <sys/errno.h>   // for errno
#include <sys/socket.h>  // for connect, socket, AF_INET, SOCK_STREAM
#include <unistd.h>      // for close, read, write

#include "include/helper.h"  // for readLine

#define MSG_LEN 13

int main() {
  // Declaration of structures
  struct sockaddr_in serverAddr;
  int sock;
  char buf[MSG_LEN + 1];

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

  // Create a socket
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Zero out the server address in the struct
  bzero((void *)&serverAddr, sizeof(serverAddr));

  // Set the domain to internet
  serverAddr.sin_family = AF_INET;

  // Insert the IP address to the server
  serverAddr.sin_addr.s_addr = inet_addr(ip);
  free(ip);

  // Set the port number
  int port = atoi(portStr);
  free(portStr);
  // The htons() function converts the unsigned short integer `hostshort` from
  // host byte order to network byte order.
  serverAddr.sin_port = htons(port);

  // Connect
  int error = connect(sock, (struct sockaddr *)&serverAddr, sizeof serverAddr);
  if (error != 0) {
    printf("Connection failed: %s", strerror(errno));
  }

  // Send the data
  write(sock, "Hello, world!", MSG_LEN);

  // Read data from the connection
  read(sock, buf, MSG_LEN);

  // Add a termination sign, and write it to the screen
  buf[MSG_LEN] = '\0';
  printf("%s\n", buf);

  // Close the socket
  close(sock);

  return EXIT_SUCCESS;
}
