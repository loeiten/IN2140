#include <arpa/inet.h>   // for inet_addr, htons
#include <netinet/in.h>  // for sockaddr_in, IPPROTO_TCP, in_addr
#include <stdio.h>       // for printf
#include <stdlib.h>      // for EXIT_SUCCESS
#include <strings.h>     // for bzero
#include <sys/socket.h>  // for connect, socket, AF_INET, SOCK_STREAM
#include <unistd.h>      // for close, read, write

int main() {
  // Declaration of structures
  struct sockaddr_in serverAddr;
  int sock;
  char buf[14];

  // Create a socket
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Zero out the server address in the struct
  bzero((void *)&serverAddr, sizeof(serverAddr));

  // Set the domain to internet
  serverAddr.sin_family = AF_INET;

  // Insert the IP address to the server
  serverAddr.sin_addr.s_addr = inet_addr("xxx.xxx.xxx.xxx");

  // Sett the port number
  serverAddr.sin_port = htons(2009);

  // Connect
  connect(sock, (struct sockaddr *)&serverAddr, sizeof serverAddr);

  // Send the data
  write(sock, "Hello, world!", 13);

  // Read data from the connection
  read(sock, buf, 13);

  // Add a termination sign, and write it to the screen
  buf[13] = '\0';
  printf("%s \n", buf);

  // Close the socket
  close(sock);

  return EXIT_SUCCESS;
}
