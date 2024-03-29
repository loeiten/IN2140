#include "../include/node_communication.h"

#include <arpa/inet.h>   // for htons, htonl, ntohs
#include <errno.h>       // for errno
#include <netinet/in.h>  // for sockaddr_in, in_addr
#include <stdio.h>       // for fprintf, stderr, NULL
#include <stdlib.h>      // for EXIT_FAILURE, free
#include <string.h>      // for strerror, strlen
#include <strings.h>     // for bzero
#include <sys/socket.h>  // for AF_INET, sendto, socket
#include <unistd.h>      // for sleep

#include "../../utils/include/common.h"          // for sendMessage, Node
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

int getUDPSocket(int* const connectSocket, const int port) {
  if ((port < MIN_PORT) || (port > MAX_PORT)) {
    fprintf(stderr, "port must be in the range [%d, %d], got %d\n", MIN_PORT,
            MAX_PORT, port);
  }
  // A great introduction to socket programming can be found at:
  // https://users.cs.jmu.edu/bernstdh/web/common/lectures/summary_unix_udp.php
  (*connectSocket) =
      socket(AF_INET,     // We are using IPv4 protocols (LOCAL is referring to
                          // UNIX domain sockets)
             SOCK_DGRAM,  // Datagrams (connection-less, unreliable
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
  addr.sin_family = AF_INET;  // We are still communicating locally
  addr.sin_addr.s_addr =
      htonl(INADDR_ANY);  // Only local addresses are accepted (INADDR_ANY would
                          // accept connection to any addresses)
  addr.sin_port = htons(port);  // The port in network byte order

  // Bind assigns the address specified by sockaddr_in to a socket
  // Traditionally, this operation is called "assigning a name to a socket".
  int success = bind((*connectSocket), (struct sockaddr*)&addr, sizeof(addr));
  if (success != 0) {
    fprintf(stderr,
            "Binding client socket=%d to port=%d failed.\nError %d: %s\n",
            *connectSocket, port, errno, strerror(errno));
    return EXIT_FAILURE;
  }

  // NOTE: We are not listening as UDP is connectionless
  return EXIT_SUCCESS;
}

int getTCPClientSocket(int* const clientSocket, const int basePort) {
  if ((basePort < MIN_PORT) || (basePort > MAX_PORT)) {
    fprintf(stderr, "basePort must be in the range [%d, %d], got %d\n",
            MIN_PORT, MAX_PORT, basePort);
  }
  // https://man7.org/linux/man-pages/man2/socket.2.html
  // Create the socket file descriptor for the server
  // This is an endpoint for the communication
  (*clientSocket) = socket(AF_INET,       // We are communicating using IPv4
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
  serverAddr.sin_family =
      AF_INET;  // Use IPv4 socket (LOCAL refers to UNIX domain sockets)
  serverAddr.sin_addr.s_addr =
      htonl(INADDR_LOOPBACK);  // Equivalent to inet_addr("127.0.0.1")
  serverAddr.sin_port = htons(basePort);  // The port in network byte order

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
  int success = sendMessage(node->tcpSocket, &(node->address), nBytes, 0);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Sending node->address failed.\n");
    return EXIT_FAILURE;
  }

  // Send the size of the array
  success = sendMessage(node->tcpSocket, &(node->nNeighbors), nBytes, 0);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Sending node->nNeighbors failed.\n");
    return EXIT_FAILURE;
  }

  // Send the neighbor address array
  nBytes *= node->nNeighbors;
  success = sendMessage(node->tcpSocket, node->neighborAddresses, nBytes, 0);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Sending node->neighborAddresses failed.\n");
    return EXIT_FAILURE;
  }

  // Send the neighbor weight array
  success = sendMessage(node->tcpSocket, node->edgeWeights, nBytes, 0);
  if (success != EXIT_SUCCESS) {
    fprintf(stderr, "Sending node->edgeWeights failed.\n");
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
  int success = receiveNBytesMessage(tcpRoutingServerSocketFd, &nRows, nBytes,
                                     MSG_WAITALL);
  if (success == EXIT_FAILURE) {
    fprintf(stderr, "Receiving nRows failed\n");
    return EXIT_FAILURE;
  }

  // Allocate memory to the routing table
  success = allocateRoutingTable(routingTable, nRows, "node routing table");
  if (success != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Special case if the node is an leaf node
  if (nRows == 0) {
    return EXIT_SUCCESS;
  }

  // Receive the table
  nBytes = nRows * sizeof(struct RoutingTableRow);
  success =
      receiveNBytesMessage(tcpRoutingServerSocketFd,
                           routingTable->routingTableRows, nBytes, MSG_WAITALL);
  if (success == EXIT_FAILURE) {
    fprintf(stderr, "Receiving routingTable->table failed\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int receiveAndForwardPackets(const int udpSocketFd, const int ownAddress,
                             const int basePort,
                             const struct RoutingTable* const routingTable) {
  unsigned char packet[MAX_MSG_LENGTH];

  while (1) {
    int success = receiveMessage(udpSocketFd, packet, 0);
    if (success != EXIT_SUCCESS) {
      fprintf(stderr, "Error when receiving packet\n");
      return EXIT_FAILURE;
    }

    short tmp;
    unsigned short destination;
    // NOTE: The packet order is given in the assignment
    memcpy(&tmp, &packet[2], sizeof(tmp));
    destination = ntohs(tmp);
    const char* msg = (char*)(&packet[6]);
    if ((destination == ownAddress) && (strcmp(msg, "QUIT") == 0)) {
      return EXIT_SUCCESS;
    }

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
        fprintf(
            stderr,
            "Could not find the next hop of %d when the destination was %d\n",
            ownAddress, destination);
        return EXIT_FAILURE;
      }

      struct sockaddr_in destAddr;
      // For safety measure: Erase data by writing 0's to the memory location
      // https://stackoverflow.com/q/36086642/2786884
      bzero((void*)&destAddr, sizeof(destAddr));
      destAddr.sin_family = AF_INET;  // We are communicating using IPv4
      destAddr.sin_addr.s_addr =
          htonl(INADDR_LOOPBACK);  // The destination is local
      destAddr.sin_port = htons(destination + basePort);  // This is the port

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
  }
  return EXIT_SUCCESS;
}

int prepareAndSendPackets(const char* filepath, const int udpSocketFd,
                          const int ownAddress, const int basePort,
                          const struct RoutingTable* const routingTable) {
  // Wait for all connections to open
  sleep(1);

  // Open data.txt
  FILE* fp = fopen(filepath, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open %s: %s\n", filepath, strerror(errno));
    return EXIT_FAILURE;
  }

  char* line = NULL;
  char* msg = NULL;
  char* packet = NULL;

  // NOTE: The \n as we read line by line
  char* quitMsg = NULL;
  int strLen = snprintf(NULL, 0, "%d QUIT\n", ownAddress);
  quitMsg = (char*)malloc((strLen + 1) * sizeof(char));
  if (quitMsg == NULL) {
    perror("Could not allocate memory to the quitMsg: ");
    fclose(fp);
    return EXIT_FAILURE;
  }
  int charWritten = snprintf(quitMsg, (strLen + 1), "%d QUIT\n", ownAddress);
  if ((charWritten < 0) || (charWritten > strLen)) {
    fprintf(stderr, "%s", "Failed to copy to the quitMsg string\n");
    free(quitMsg);
    fclose(fp);
    return EXIT_FAILURE;
  }

  size_t len = 0;
  ssize_t nBytes = 0;
  while (nBytes != EOF) {
    // NOTE: getline is from POSIX.1-2008, not the C-standard, see
    // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getline.html
    // for specification
    nBytes = getline(&line, &len, fp);
    if (ferror(fp)) {
      fprintf(stderr, "Failed to read %s: %s\n", filepath, strerror(errno));
      free(quitMsg);
      quitMsg = NULL;
      fclose(fp);
      return EXIT_FAILURE;
    } else if (feof(fp)) {
      break;
    }
    unsigned short length;
    unsigned short destination;
    unsigned short source = 1;
    int success =
        extractLengthDestinationAndMessage(line, &length, &destination, &msg);
    if (success != EXIT_SUCCESS) {
      cleanUpPrepareAndSendPackets(
          fp, &line, &msg, &packet, &quitMsg,
          "Failed to extract length, destination and message\n");
      return EXIT_FAILURE;
    }
    if (strcmp(line, quitMsg) == 0) {
      cleanUpPrepareAndSendPackets(fp, &line, &msg, &packet, &quitMsg, "");
      return EXIT_SUCCESS;
    }
    success = createPacket(length, destination, source, msg, &packet);
    if (success != EXIT_SUCCESS) {
      cleanUpPrepareAndSendPackets(fp, &line, &msg, &packet, &quitMsg,
                                   "Failed to create the packet\n");
      return EXIT_FAILURE;
    }
    print_pkt((unsigned char*)packet);

    success = sendUDPPacket(packet, length, udpSocketFd, ownAddress,
                            destination, basePort, routingTable);
    if (success != EXIT_SUCCESS) {
      cleanUpPrepareAndSendPackets(fp, &line, &msg, &packet, &quitMsg,
                                   "Failed to create the packet\n");
      return EXIT_FAILURE;
    }
  }

  cleanUpPrepareAndSendPackets(fp, &line, &msg, &packet, &quitMsg, "");
  return EXIT_SUCCESS;
}

void cleanUpPrepareAndSendPackets(FILE* fp, char** line, char** msg,
                                  char** packet, char** quitMsg,
                                  const char* errorMsg) {
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
  if (*quitMsg != NULL) {
    free(*quitMsg);
    *quitMsg = NULL;
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
  (*msg) = strdup(&(lineCpy[spaceI + 1]));
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
  memcpy(&((*packet)[0]), &tmp, sizeof(tmp));

  // Assign the destination to the next 2 bytes
  tmp = htons(destination);
  memcpy(&((*packet)[2]), &tmp, sizeof(tmp));

  // Assign the source to the next 2 bytes
  tmp = htons(source);
  memcpy(&((*packet)[4]), &tmp, sizeof(tmp));

  // Finally: Assign the message
  // +1 for the terminating null character
  memcpy(&((*packet)[6]), msg, (strlen(msg) + 1) * sizeof(char));

  return EXIT_SUCCESS;
}

int sendUDPPacket(const char* const packet, const int length,
                  const int udpSocketFd, const unsigned short ownAddress,
                  const unsigned short destination, const int basePort,
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
            "%d\n",
            destination, ownAddress);
    return EXIT_FAILURE;
  }

  struct sockaddr_in destAddr;
  // For safety measure: Erase data by writing 0's to the memory location
  // https://stackoverflow.com/q/36086642/2786884
  bzero((void*)&destAddr, sizeof(destAddr));
  destAddr.sin_family = AF_INET;  // We are communicating using IPv4
  destAddr.sin_addr.s_addr =
      htonl(INADDR_LOOPBACK);                     // The destination is local
  destAddr.sin_port = htons(basePort + nextHop);  // This is the port

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
