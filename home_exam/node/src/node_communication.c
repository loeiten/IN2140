#include "../include/node_communication.h"

#include <arpa/inet.h>   // for htons, ntohs
#include <errno.h>       // for errno
#include <netinet/in.h>  // for sockaddr_in, INADDR_...
#include <stdio.h>       // for fprintf, stderr, ssi...
#include <stdlib.h>      // for EXIT_FAILURE, EXIT_S...
#include <string.h>      // for strerror, memcpy
#include <strings.h>     // for bzero
#include <sys/socket.h>  // for send, recv, socket
#include <unistd.h>

#include "../../utils/include/common.h"          // for Node
#include "../../utils/include/dynamic_memory.h"  // for allocateRoutingTable
// NOTE: We are not specifying the full path here
//       As a consequence we have to do the following
//       1. Use -I in the compilation to expand the include path of the compiler
//       2. If you use compile_commands.json for code completion,
//          compile errors, go-to definition etc. you need to create compile
//          command
//          You can do this with for example
//          bear --output ../build/compile_commands.json --append -- make -B
#include "print_lib/include/print_lib.h"  // for print_forwarded_pkt

int getUDPSocket(int* const connectSocket, const int basePort) {
  // A great introduction to socket programming can be found at:
  // https://users.cs.jmu.edu/bernstdh/web/common/lectures/summary_unix_udp.php
  (*connectSocket) =
      socket(AF_LOCAL,      // We are communicating locally
             SOCK_DGRAM,    // Datagrams (connection-less, unreliable
                            // messages of a fixed maximum length)
             IPPROTO_UDP);  // Use the UDP protocol
                            // (redundant because of SOCK_DGRAM)
  if ((*connectSocket) == -1) {
    // NOTE: fprintf(stderr, "%s\n", strerror(errno)) is similar to perror(NULL)
    fprintf(stderr, "Failed to create UPD socket.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  }

  // Create an address, which we can bind to the socket fd
  struct sockaddr_in addr;
  // For safety measure: Erase data by writing 0's to the memory location
  // https://stackoverflow.com/q/36086642/2786884
  bzero((void*)&addr, sizeof(addr));

  // Bind the socket to an address
  // NOTE: Only the server needs to bind to a port because a client needs a
  //       fixed port to send to.
  //       A client needn't bind() at all:
  //       an automatic bind() will take place on the first
  //       send()/sendto()/recv()/recvfrom() using a system-assigned local port
  //       number.
  //       https://stackoverflow.com/a/6194518/2786884
  // NOTE: In UDP you can connect before sending
  //       When a UDP socket is created, its local and remote addresses are
  //       unspecified. Datagrams can be sent immediately using sendto(2) or
  //       sendmsg(2) with a valid destination address as an argument. When
  //       connect(2) is called on the socket, the default destination
  //       address is set and datagrams can now be sent using send(2) or
  //       write(2) without specifying a destination address.
  //       https://linux.die.net/man/7/udp
  //       However, we know all the ports in advance
  // In this assignment the socket will be used to receive from and sending to
  // other nodes
  addr.sin_family = AF_LOCAL;  // We are still communicating locally
  addr.sin_addr.s_addr =
      INADDR_LOOPBACK;  // Only local addresses are accepted (INADDR_ANY would
                        // accept connection to any addresses)
  addr.sin_port = htons(basePort);  // The port in network byte order

  // Bind assigns the address specified by sockaddr_in to a socket
  // Traditionally, this operation is called "assigning a name to a socket".
  int success = bind((*connectSocket), (struct sockaddr*)&addr, sizeof(addr));
  if (success != 0) {
    fprintf(stderr, "Binding client socket to address failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  }

  // NOTE: We are not listening as UDP is connectionless
  return EXIT_SUCCESS;
}

int getTCPClientSocket(int* const clientSocket, const int serverPort) {
  // https://man7.org/linux/man-pages/man2/socket.2.html
  // Create the socket file descriptor for the server
  // This is an endpoint for the communication
  (*clientSocket) = socket(AF_LOCAL,      // We are communicating locally
                           SOCK_STREAM,   // Sequenced, reliable, two-way,
                                          // connection-based byte streams (TCP)
                           IPPROTO_TCP);  // Use the TCP protocol
                                          // (redundant because of SOCK_STREAM)
  if ((*clientSocket) == -1) {
    // NOTE: fprintf(stderr, "%s\n", strerror(errno)) is similar to perror(NULL)
    fprintf(stderr, "Failed to create socket for the client.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  }

  // Create an address, which we can bind to the socket fd
  struct sockaddr_in serverAddr;
  // For safety measure: Erase data by writing 0's to the memory location
  // https://stackoverflow.com/q/36086642/2786884
  bzero((void*)&serverAddr, sizeof(serverAddr));

  // Bind the socket to an address
  serverAddr.sin_family = AF_LOCAL;  // We are still communicating locally
  serverAddr.sin_addr.s_addr =
      INADDR_LOOPBACK;  // Equivalent to inet_addr("127.0.0.1")
  serverAddr.sin_port = htons(serverPort);  // The port in network byte order

  // NOTE: In TCP you can bind before connecting
  //       If not this is done automatically by the OS
  // https://idea.popcount.org/2014-04-03-bind-before-connect/

  // Initiate a connection on a socket
  int success = connect((*clientSocket), (struct sockaddr*)&serverAddr,
                        sizeof(serverAddr));
  if (success != 0) {
    fprintf(stderr, "Connection to server failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int sendEdgeInformation(struct Node* const node) {
  // Send own address
  ssize_t nBytes = sizeof(int);
  ssize_t bytesSent = send(node->tcpSocket, &(node->address), nBytes, 0);
  if (bytesSent != -1) {
    fprintf(stderr, "Sending node->address failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(stderr, "Sent less bytes than expected for node->address\n");
    return EXIT_FAILURE;
  }

  // Send the size of the array
  bytesSent = send(node->tcpSocket, &(node->nNeighbors), nBytes, 0);
  if (bytesSent == -1) {
    fprintf(stderr, "Sending node->nNeighbors failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(stderr, "Sent less bytes than expected for node->nNeighbors\n");
    return EXIT_FAILURE;
  }

  // Send the neighbor address array
  nBytes *= node->nNeighbors;
  bytesSent = send(node->tcpSocket, &(node->neighborAddresses), nBytes, 0);
  if (bytesSent == -1) {
    fprintf(stderr, "Sending node->neighborAddresses failed.\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(stderr,
            "Sent less bytes than expected for "
            "node->neighborAddresses\n");
    return EXIT_FAILURE;
  }

  // Send the neighbor weight array
  bytesSent = send(node->tcpSocket, &(node->edgeWeights), nBytes, 0);
  if (bytesSent == -1) {
    fprintf(stderr, "Sending node->edgeWeights failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != nBytes) {
    fprintf(stderr, "Sent less bytes than expected for node->edgeWeights\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int receiveRoutingTable(const int tcpRoutingServerSocketFd,
                        struct RoutingTable* routingTable) {
  // NOTE: recv()/send() are specific to socket descriptors, whereas
  //       read()/write() are universal functions working on all descriptors
  //       Both of them are blocking by default
  // Receive the number of rows
  int nRows;
  ssize_t nBytes = sizeof(int);
  ssize_t bytesReceived =
      recv(tcpRoutingServerSocketFd, &nRows, nBytes, MSG_WAITALL);
  if (bytesReceived == -1) {
    fprintf(stderr, "Receiving nRows failed.\nError %d: %s\n", errno,
            strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesReceived != nBytes) {
    fprintf(stderr, "Received less bytes than expected for nRows\n");
    return EXIT_FAILURE;
  }

  // Allocate memory to the routing table
  int success = allocateRoutingTable(routingTable, nRows, "node routing table");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Receive the table
  nBytes = nRows * sizeof(struct RoutingTableRows);
  bytesReceived = recv(tcpRoutingServerSocketFd,
                       &(routingTable->routingTableRows), nBytes, MSG_WAITALL);
  if (bytesReceived == -1) {
    fprintf(stderr,
            "Receiving routingTable->table routingTableRows\nError %d: %s\n",
            errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesReceived != nBytes) {
    fprintf(stderr, "Received less bytes than expected for routingTableRows\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int receiveAndForwardPackets(const int udpSocketFd, const int ownAddress,
                             const int serverPort,
                             const struct RoutingTable* const routingTable) {
  unsigned char* packet = NULL;
  ssize_t bytesReceived = recv(udpSocketFd, packet, MAX_MSG_LENGTH, 0);
  if (bytesReceived == -1) {
    fprintf(stderr, "Error when receiving package: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  short tmp;
  unsigned short destination;
  // NOTE: The packet order is given in the assignment
  memcpy(&tmp, &packet[2], sizeof(tmp));
  destination = ntohs(tmp);
  if (destination == ownAddress) {
    print_received_pkt(ownAddress, packet);
  } else {
    print_forwarded_pkt(ownAddress, packet);
    // Forward the packet
    // Find the next hop
    int nextHop = -1;
    for (int i = 0; i < routingTable->nRows; ++i) {
      if (routingTable->routingTableRows[i].destination == destination) {
        nextHop = routingTable->routingTableRows[i].nextHop;
        break;
      }
    }
    if (nextHop == -1) {
      fprintf(stderr,
              "Could not find the next hop of %d when the destination was %d\n",
              ownAddress, destination);
      return EXIT_FAILURE;
    }

    struct sockaddr_in destAddr;
    destAddr.sin_family = AF_LOCAL;              // We are communicating locally
    destAddr.sin_addr.s_addr = INADDR_LOOPBACK;  // The destination is local
    destAddr.sin_port = htons(destination + serverPort);  // This is the port

    // Update the source of the packet
    unsigned short source = htons(ownAddress);
    memcpy(&packet[4], &source, sizeof(source));

    // Forward the message
    memcpy(&tmp, &packet[0], sizeof(tmp));
    unsigned short length = ntohs(tmp);
    ssize_t bytesSent = sendto(udpSocketFd, packet, length, 0,
                               (struct sockaddr*)&destAddr, sizeof(destAddr));
    if (bytesSent == -1) {
      fprintf(stderr, "Forwarding from %d to %d failed.\nError %d: %s\n",
              ownAddress, destination, errno, strerror(errno));
      return EXIT_FAILURE;
    } else if (bytesSent != length) {
      fprintf(stderr, "Sent less bytes than expected from %d to %d\n",
              ownAddress, destination);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int prepareAndSendPackets(const int udpSocketFd, const int ownAddress,
                          const int serverPort,
                          const struct RoutingTable* const routingTable) {
  // Wait for all connections to open
  sleep(1);

  // Open data.txt
  FILE* fp = fopen("data.txt", "r");
  if (fp == NULL) {
    perror("Failed to open data.txt:");
    return EXIT_FAILURE;
  }

  char** line = NULL;
  char* msg = NULL;
  char* packet = NULL;
  size_t len = 0;

  // NOTE: getline is from POSIX.1-2008, not the C-standard, see
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getline.html
  // for specification
  ssize_t nBytes = 0;
  while (nBytes != EOF) {
    nBytes = getline(line, &len, fp);
    if (ferror(fp)) {
      perror("Failed to read data.txt");
      fclose(fp);
      return EXIT_FAILURE;
    } else if (feof(fp)) {
      break;
    }
    unsigned short length;
    unsigned short destination;
    unsigned short source = 1;
    int success =
        extractLengthDestinationAndMessage(*line, &length, &destination, &msg);
    if (success != EXIT_SUCCESS) {
      cleanUpPrepareAndSendPackets(
          fp, line, &msg, &packet,
          "Failed to extract length, destination and message\n");
      return EXIT_FAILURE;
    }
    if (strcmp(msg, "QUIT") == 0) {
      cleanUpPrepareAndSendPackets(fp, line, &msg, &packet, "");
      return EXIT_SUCCESS;
    }
    success = createPacket(length, destination, source, msg, &packet);
    if (success != EXIT_SUCCESS) {
      cleanUpPrepareAndSendPackets(fp, line, &msg, &packet,
                                   "Failed to create the packet\n");
      return EXIT_FAILURE;
    }

    success = sendUDPPacket(packet, length, udpSocketFd, ownAddress,
                            destination, serverPort, routingTable);
    if (success != EXIT_SUCCESS) {
      cleanUpPrepareAndSendPackets(fp, line, &msg, &packet,
                                   "Failed to create the packet\n");
      return EXIT_FAILURE;
    }
  }

  cleanUpPrepareAndSendPackets(fp, line, &msg, &packet, "");
  return EXIT_SUCCESS;
}

void cleanUpPrepareAndSendPackets(FILE* fp, char** line, char** msg,
                                  char** packet, const char* errorMsg) {
  fprintf(stderr, "%s\n", errorMsg);
  fclose(fp);
  if (*line != NULL) {
    free(*line);
    *line = NULL;
  }
  if (*msg != NULL) {
    free(*msg);
    *msg = NULL;
  }
  if (*packet != NULL) {
    free(*packet);
    *packet = NULL;
  }
}

int extractLengthDestinationAndMessage(const char* const line,
                                       unsigned short* const length,
                                       unsigned short* const destination,
                                       char** msg) {
  // Duplicate the line so that we can modify it
  char* lineCpy = strdup(line);
  if (lineCpy == NULL) {
    fprintf(stderr, "Could not duplicate the line '%s'\n", line);
    return EXIT_FAILURE;
  }

  // We will:
  // 1. Loop until the first space
  // 2. Replace the first space with \0
  // 3. Use the resulting string as a parameter in atoi
  // 4. Continue loop until the first \n
  // 5. Replace with \0
  // 6. Copy this to msg
  // 7. Calculate the message length

  // 1. Loop until the first space
  int lineLen = strlen(line);
  int spaceI = 0;
  for (; spaceI < lineLen; ++spaceI) {
    if (line[spaceI] == ' ') {
      break;
    }
  }
  if (spaceI == lineLen) {
    fprintf(stderr, "Found no spaces in the line '%s'\n", line);
    free(lineCpy);
    lineCpy = NULL;
    return EXIT_FAILURE;
  }

  // 2. Replace the first space with \0
  lineCpy[spaceI] = '\0';

  // 3. Use the resulting string as a parameter in atoi
  (*destination) = (unsigned short)atoi(lineCpy);

  // 4. Continue loop until the first \n
  int nI = spaceI;
  for (; nI < lineLen; ++nI) {
    if (line[nI] == '\n') {
      break;
    }
  }
  if (nI == lineLen) {
    fprintf(stderr, "Found no newlines in the line '%s'\n", line);
    free(lineCpy);
    lineCpy = NULL;
    return EXIT_FAILURE;
  }

  // 5. Replace with \0
  lineCpy[nI] = '\0';

  // 6. Copy this to msg
  (*msg) = strdup(&(lineCpy[spaceI]));
  if (*msg == NULL) {
    fprintf(stderr, "Could not duplicate the line '%s'\n", line);
    free(lineCpy);
    lineCpy = NULL;
    return EXIT_FAILURE;
  }

  // 7. Calculate the message length
  // 2 bytes for packet length
  // 2 bytes for destination address
  // 2 bytes for source address
  // +1 for terminating null character
  *length = 7 + strlen(*msg);

  return EXIT_SUCCESS;
}

int createPacket(const int length, const unsigned short destination,
                 const unsigned short source, const char* const msg,
                 char** packet) {
  // Allocate memory to the message
  *packet = (char*)malloc(length * sizeof(char));
  if ((*packet) == NULL) {
    perror("Could not allocate memory to the packet: ");
    return EXIT_FAILURE;
  }

  // Assign the length to the first 2 bytes of the packet
  unsigned short tmp = htons(length);
  memcpy(&(*packet[0]), &tmp, sizeof(tmp));

  // Assign the destination to the next 2 bytes
  tmp = htons(destination);
  memcpy(&(*packet[2]), &destination, sizeof(tmp));

  // Assign the source to the next 2 bytes
  tmp = htons(source);
  memcpy(&(*packet[4]), &tmp, sizeof(tmp));

  // Finally: Assign the message
  // +1 for the terminating null character
  memcpy(&(*packet[6]), &msg, (strlen(msg) + 1) * sizeof(char));

  return EXIT_SUCCESS;
}

int sendUDPPacket(const char* const packet, const int length,
                  const int udpSocketFd, const unsigned short ownAddress,
                  const unsigned short destination, const int serverPort,
                  const struct RoutingTable* const routingTable) {
  // Find next hop from the routing table
  int nextHop = -1;
  for (int row = 0; row < routingTable->nRows; ++row) {
    if (routingTable->routingTableRows[row].destination == destination) {
      nextHop = routingTable->routingTableRows[row].nextHop;
    }
  }
  if (nextHop == -1) {
    fprintf(stderr,
            "Couldn't find destination %d in the routing table of node "
            "%d",
            destination, ownAddress);
    return EXIT_FAILURE;
  }

  struct sockaddr_in destAddr;
  destAddr.sin_family = AF_LOCAL;              // We are communicating locally
  destAddr.sin_addr.s_addr = INADDR_LOOPBACK;  // The destination is local
  destAddr.sin_port = htons(serverPort + nextHop);  // This is the port

  ssize_t bytesSent = sendto(udpSocketFd, packet, length, 0,
                             (struct sockaddr*)&destAddr, sizeof(destAddr));
  if (bytesSent == -1) {
    fprintf(stderr, "Sending from %d to %d failed.\nError %d: %s\n", ownAddress,
            destination, errno, strerror(errno));
    return EXIT_FAILURE;
  } else if (bytesSent != length) {
    fprintf(stderr, "Sent less bytes than expected from %d to %d.\n",
            ownAddress, destination);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
