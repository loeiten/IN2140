#include "../include/node_communication.h"

#include <stdlib.h>  // for EXIT_SUCCESS

int getConnectSocket(const int connectPort, int* const connectSocket,
                     enum connectionType connection) {
  (void)connectPort;
  (void)connectSocket;
  (void)connection;
  /*
    // Abbreviations:
    // AF - Address family
    //      Address families are categorization of network addresses and
    protocols
    //      Examples: AF_INET (AddressFamily - InterNET) for IPv4
    //      Examples: AF_INET6 (AddressFamily - InterNET) for IPv6
    //      Examples: AF_LOCAL (AddressFamily - Local) for local communication
    //      See https://man7.org/linux/man-pages/man7/address_families.7.html
    // IN - Internet
    // SIN - Shorthand for sockaddr_in

    // https://man7.org/linux/man-pages/man2/socket.2.html
    // Create the socket file descriptor for the client
    // This is an endpoint for the communication
    int protocol;
    if(connection == UDP){
      protocol = IPPROTO_UDP;
    } else if (connection == TCP) {
      protocol = IPPROTO_TCP;
    } else {
      fprintf(stderr, "Unknown connectionType %d received\n", connection);
      return EXIT_FAILURE;
    }

    (*connectSocket) = socket(AF_LOCAL,      // We are communicating locally
                             // FIXME:
                             SOCK_STREAM,   // Sequenced, reliable, two-way,
                                            // connection-based byte streams
                             protocol);  // Use the UDP protocol
    if ((*connectSocket) == -1) {
      // NOTE: fprintf(stderr, "%s\n", strerror(errno)) is similar to
    perror(NULL) fprintf(stderr, "Failed to create socket for the client.\nError
    %d: %s\n", errno, strerror(errno)); return EXIT_FAILURE;
    }

    // Create an address, which we can bind to the socket fd
    struct sockaddr_in clientAddr;
    // For safety measure: Erase data by writing 0's to the memory location
    // https://stackoverflow.com/q/36086642/2786884
    bzero((void*)&clientAddr, sizeof(clientAddr));

    // Bind the socket to an address
    // When a process wants to receive new incoming packets or connections, it
    // should bind a socket to a local interface address using bind.
    // In this case, only one IP socket may be bound to any given local
    (address,
    // port) pair. When INADDR_ANY is specified in the bind call, the socket
    will
    // be bound to all local interfaces.
    clientAddr.sin_family = AF_LOCAL;  // We are still communicating locally
    // FIXME:
    clientAddr.sin_addr.s_addr = INADDR_ANY;  // All addresses are accepted
    clientAddr.sin_port = htons(connectPort);  // The port in network byte order

    // Bind assigns the address specified by sockaddr_in to a socket
    // Traditionally, this operation is called "assigning a name to a socket".
    int success =
        bind((*connectSocket), (struct sockaddr*)&clientAddr,
    sizeof(clientAddr)); if (success != 0) { fprintf(stderr, "Binding client
    socket to address failed.\nError %d: %s\n", errno, strerror(errno)); return
    EXIT_FAILURE;
    }

    // Activate connecting on the socket
    // It puts the client socket in a passive mode, where it waits for the
    client
    // to approach the client to make a connection
    success = connect((*connectSocket),
                     SOMAXCONN);  // Max length for queue pending connection to
                                  // the request socket
    if (success != 0) {
      fprintf(stderr, "Listening to the client socket failed.\nError %d: %s\n",
              errno, strerror(errno));
      return EXIT_FAILURE;
    }
  */
  return EXIT_SUCCESS;
}
