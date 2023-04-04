#include <arpa/inet.h>   // for htons
#include <netinet/in.h>  // for sockaddr_in, INADDR_ANY, IPPROTO_TCP, in_addr
#include <stdio.h>       // for printf
#include <stdlib.h>      // for EXIT_SUCCESS
#include <strings.h>     // for bzero
#include <sys/socket.h>  // for accept, bind, listen, socket, AF_INET, SOCK_...
#include <unistd.h>      // for close, read, write

int main() {
  // Declaration of structures
  struct sockaddr_in serverAddr, clientAddr;
  int clientAddrLen;
  int requestSock, sock;
  char buf[14];

  // Create a request socket
  requestSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Create an address structure
  bzero((void *)&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(2009);

  // Bind the address to the socket
  bind(requestSock, (struct sockaddr *)&serverAddr, sizeof serverAddr);

  // Activate listening on the socket
  listen(requestSock, SOMAXCONN);

  // Accept a connection
  sock = accept(requestSock, (struct sockaddr *)&clientAddr, &clientAddrLen);

  // Read data from the connection, and write it out
  read(sock, buf, 13);
  buf[13] = '\0';
  printf("%s \n", buf);

  // Send data back to the connection
  write(sock, buf, 13);

  // Close the sockets
  close(sock);
  close(requestSock);

  return EXIT_SUCCESS;
}
