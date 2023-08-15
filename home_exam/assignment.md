# Home exam: An emulation of routing and forwarding

## Introduction

An important part of networking is in an intermediate node to receive packets
and then forward them on an appropriate way towards the destination.
The idea in this home exam is to make a program that simulates routing and
forwarding in a network where you have to implement a small system that can
first set up routing tables and afterwards send messages from one node to all
other nodes.
For simplicity, we emulate computers using several processes running on the same
computer.

Your system must run on standard Linux machines, and you must use the standard
​`gcc` compiler installed on these machines.

Read carefully the section [​Points](#points) at the end of this file.
In the section, you can read what is important for points, you get a starting
recommendation, and also what is absolutely necessary to get any points at all.

## The task

For the sake of simplicity, the routing and forwarding are emulated by local
processes on a single machine and
[level 4 (transport layer) ports](https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers)
are used as unambiguous identifiers - addresses.

We want to emulate a network with `N` nodes, and we start one process for each
of these nodes.
Every node can only send packets to directly connected neighboring nodes in the
network.
To send to direct neighbors, we use
[UDP packets](https://en.wikipedia.org/wiki/User_Datagram_Protocol).

One of the `N` nodes (processes) is the sender.
This node has always the `OwnAddress==1` and is called "node 1" in this text.
Node 1 wants to send messages to several other nodes using the shortest path to
the destination.
Only node 1 starts the transmission of messages, and all other nodes should be
able to receive and forward packets.

The shortest path computation is computed by the central routing server - the
`​routing_server`​, which is not part of the forwarding network.
Instead, every node has a separate, direct TCP connection with ​`routing_server`​.
`routing_server` receives edge weights from all other nodes, computes Dijkstra's
algorithm and sends one dedicated routing table to each of the nodes.

Your goal is to collect the connectivity information on `​routing_server` from
each node, compute the shortest paths from node 1 to all the other nodes and
install the routes in the respective nodes' routing table.
Then, node 1 should send given messages through the emulated network using UDP
packets.

You should write 2 programs called `​node` and `routing_server`​, one for a node
and another one for the central server.

About the realism of this experiment:

- Using a central server to compute all routes inside a network is currently a
  hot topic, so it is realistic.
  These networks can get very large.
  Central route computation is not typical between networks that have different
  owners.
- Using a direct connection from every node in the network to the central server
  computing routes is a strong simplification.
  It is unrealistic.

## The emulated network

The nodes are arranged in a graph with weighted bi-directional as shown in
[graph1](./figures/graph1.pdf).
These edge weights represent the cost of sending a message over this edge.
The weights are the same in both directions.
Each node knows some of its direct neighbors in the graph, as well as the edge
weights to them.

## The routing server

The ​`routing_server` process starts first.
It takes as parameters a TCP port `P` and a number of nodes `N`.
The address of a node will be defined later in the section
[​The nodes](#the-nodes)​.
We give some more information about port `P` in the section
[​The base port](#the-base-port)​.

```text
Usage:​ routing_server <P> <N>
<P> the TCP port on which the server listens
<N> The number of nodes
```

The `​routing_server` is a server.
It waits for TCP connections from the `N` nodes, receives their data containing
their address and edge information.
You must design the protocol for this data transfer, but you should make sure
that the server has enough information to detect:

1. The address of the node that sends the data
1. The length of the data
1. The neighboring nodes
1. The weights.

Use comments to explain your protocol to us.

When the ​`routing_server` has received data from all `N` nodes, it first tests
that every edge between two nodes has been reported twice: once by each of the
two nodes, and that both nodes reported the same weight.
For edges where this is not the case, the ​`routing_server​` prints a warning and
ignores the edge.

After this check, the ​`routing_server` computes all shortest path routes from
node 1 to every other node.
For this, it uses Dijkstra's Shortest Path First algorithm, using the weights
provided by the nodes.
After computing the shortest paths from node 1 to every other node,
​`routing_server`​ calls the function
`void print_weighted_edge(short from_node, short to_node, int pathlen)` once for
every combination of 2 nodes in the network, including all cases where
`​from_node == to_node` (25 calls if there are 5 nodes).

The ​`pathlen` parameter is the path length (sum of all link weights) from node 1
to `​from_node`​, but only if `from_node`​ is on the shortest path from node 1 to
​`to_node`​.
Otherwise, ​`pathlen`​ is `-1`.

After that, the `​routing_server` sends a routing table to every node containing
pairs `<destination, next_hop>` without any weights.
`next_hop` is the address of the neighbor.
Only those `<destination, next_hop>`-pairs that can be used to reach a
destination from node 1 are sent to a node.
Some nodes may receive empty routing tables.
You design the protocol for this data transfer.
Use comments to explain your protocol to us.
The routing server should quit when it has sent one routing table to every
client.
You should make sure to close all sockets and release all memory.

## The nodes

The nodes take as parameters the base port `P`, their own address `A`
(a number 1-1023), and any number of edges to direct neighbors, where each edge
is given as a pair `<NeighborAddress:weight>` - two decimal numbers separated by
the character `:`.
We explain the idea of the base port `P` in the section
[​The base port](#the-base-port)​.

```text
Usage: ​node <Port> <OwnAddress> <NeighborAddress>:<weight> ...
<Port> Value between 1024 and 65535 - number of nodes
<OwnAddress> Node's address written as decimal number (1-1023) <NeighborAddress>:<weight>
       <NeighborAddress> - Neighbor's address
       <weight> - Link's weight to corresponding neighbor (an integer number)
```

The node with `OwnAddress==1` has a special role.
We compute the shortest paths from node 1 to all other nodes, and node 1 is the
only node that initiates sending of messages.
The `node` functions that are only used if the node's `OwnAddress` is `1` are
explained below in ​[The node with OwnAddress 1​](#the-node-with-ownaddress-1).

When the node `A` starts, it tries to bind a UDP socket on port `P+A`.
For example, if the `Port` parameter of node `A` has the value `P=6000` and the
node has the address `A=72`, then `A` should be creating a UDP socket with port
number `P+A=6072`.

Before attempting to bind the UDP socket to the port `P+A`, node `A` should
check that `P+A` is a valid port number.
This socket should be suitable for sending to other nodes and for receiving from
other nodes.
So, if `A` wants to send a packet to the node with id `B`, it has to send the
packet to port `P+B`.

If the node cannot bind the socket to the port, the following should happen:

- The node prints an error message to `stderr` and quits with error code `-1`
  (see: `man exit`).
- The user will stop the experiment and try again with a new value for `P`.

After bind has been performed, node `A` opens a TCP connection to the ​
`routing_server` and sends the edge information.
If the connection fails

- Node `A` quits with error code `-2`.

After the TCP connection has been established, node `A` waits for data from the
`​routing_server` containing its routing table.
When node `A` receives data from ​`routing_server`

- It may have to read several times to receives all bytes
- It creates an appropriate data structure to store its routing table
- It may close its connection to the `​routing_server`​ after receiving the
  complete routing table

After receiving a complete routing table, node `A` waits for packets from other
nodes.
Only node 1 behaves differently, see below.
When node A receives a packet from another node

- Memory is managed as appropriate for your code
- The node receives the entire packet.
  ​The packets​ below explains the format of the packet.
- After that, it checks whether that packet's destination address is its own
  address
- If yes:
   - It passes a pointer to the buffer containing the packet to the function

     ```c
     void print_received_pkt(short ownAddress, unsigned char* packet)
     ```

   - After this, it inspects the string contained in the message
   - If the message contains only the word `QUIT`, it closes all sockets,
     releases all remaining memory, and calls exit with value `0`
- Else
   - It passes a pointer to the buffer containing the packet to the function

     ```c
     void print_forwarded_pkt(short ownAddress, unsigned char* packet)
     ```

   - It uses the routing table provided by `routing_server` to find the next
     neighbor on the route towards the destination contained in the message.
     The message is not changed.

## The node with OwnAddress 1

This is a normal node that has a special role in this experiment.
It has the `OwnAddress==1`.

This node is the source of the shortest path tree and should read messages from
the file `data.txt`.
No other nodes should read from `data.txt`.
After receiving its routing table from the routing server, node 1 it waits for 1
second, doing nothing.
Then, it reads the human-readable file `data.txt` where every line contains one
destination address, followed by one space and then a string.
The string ends with the return character `\n`, which is not part of the message.
For example, in the line

```text
814 Good luck with the home exam
```

The string `Good luck with the home exam` ​is a message that is stored in a
packet for sending.
The following is also stored in the packet:

- The length (in this case `35`)
- The source address (in this case `1`)
- The destination address (in this case `814`).

The section [The packets](#the-packets) explains how packets must look.

The node 1 uses its routing table to find the address of the neighboring node
that is on the shortest path from node `1` to node `814`.
For example, the address of that neighboring node may be `17`.
Node 1 will then send the packet to the node 17.
It will do this by sending a UDP packet to the process with the UDP port that is
computed by adding base port (`6000`) and node address (`17`), that means it
will send the packet to UDP port `6017` on `localhost`.

The file `data.txt` will only contain strings that are shorter than `1000`
bytes, and it will be human-readable.

For every line:

- It creates a packet in dynamically allocated heap memory
- It fills in packet length, destination address, source address and the message
  (without address `D`)
- After that, it calls the function

  ```c
  void print_pkt( unsigned char* packet )
  ```

  with a pointer to the dynamically allocated buffer
- After that, it sends the packet to the next node on the route towards the
  destination `D`, using the routing table provided by `routing_server`
- Memory is released as appropriate for your code

## The packets

A packet is constructed in memory and sent as a single datagram over UDP.
It has the following format:

- packet length (2 bytes in network byte order)
- destination address (2 bytes in network byte order)
- source address (2 bytes in network byte order)
- message (this a string that must always be 0-terminated)

The packet length is given in bytes and includes the bytes for packet length,
addresses, and the message including the `\0`-character.

Our print functions expect a pointer that refers to memory that contains the
whole packet with exactly this structure.

## The base port

In the assignment, you are opening several sockets that can wait for events.
The TCP socket in `​routing_server` is used to wait for connections from nodes,
the UDP socket in each node is used to wait for packets from other nodes, as
well as for sending packets.

You can choose fixed ports for each process, but if somebody else is using the
same computer at the same time, you may not get the number you want.
Also, if your program crashed recently, you may not get the same port again
without waiting for several minutes.
It is therefore convenient if you can very quickly choose new ports for the
`routing_server`​ and all of your nodes.

Our idea is that every process should be started with a base port `P`.
For `​routing_server`​, this is the TCP port that it uses for listening to
connections.
All the nodes add their own address to the port number `P`, and use that number
for their UDP port.

For example, if the base port is `6017` and the node address is `44`, the node
will bind to the UDP port `6061`.

If the ​`bind()` function returns an error for in any of your processes, you can
simply stop the experiment, restart it with a new value for `P` (for example
`12752`), and all processes will choose a different port than in the previous
run.
For example, if you choose the new base port`12752`, the node with address`44`
will try to bind to the UDP port `12796`.
It might be useful to set the socket option`​SO_REUSEADDR`​ on your sockets before
you bind them.

## The rest

This assignment the a file `print_lib.c`, which implements the functions

```c
void print_weighted_edge(short from_node, short to_node, int pathlen)
void print_received_pkt(short ownAddress, unsigned char* packet)
void print_forwarded_pkt(short ownAddress, unsigned char* packet)
void print_pkt(unsigned char* packet)
```

You must include the header file `print_lib.h`, and compile `print_lib.c` and
link it with your own code.

The provided scripts start all processes in the correct order.

Don't forget to set the socket option `​SO_REUSEADDR`​ on the TCP server socket.
It helps in debugging.

UDP will never split packets.
When you send a packet with UDP, it arrives at the receiver completely or not at
all.
TCP is different.
When you send data with TCP, it can happen that the receiving TCP socket does
not read all of these data with a single `read()` or `recv()` function call.
It will probably not happen here, but you should check if the received number of
bytes is identical to the expected number of bytes.

Note that it is possible to solve the assignment without using `select` or
`threads` in any of the nodes.
We have chosen UDP for sending packets between nodes to make this possible.
A working TCP solution without `select` or `threads` would be extremely
difficult.

## Make

The submission must contain ​`Makefile`​ which will have at least these options
`make` ​- compiles both your programs resulting in executable binaries `node`
and `routing_server`
`make all` ​- does the same as make without any parameter
`make clean` ​- deletes the executables and any temporary files (eg. `*.o`)
`make run` ​- execute both of the scripts that we provide

The submission must also contain your code, our code and our scripts.

## Points

You will get full points if:

- `Makefile` works with `make`, `make run` and `make clean` as expected, and
- Output is identical to that of a flawless working solution
   - For the 2 example networks provided by us
   - For 2 additional example networks of similar size that we are not sharing,
     and
- Code runs on a linux machine with `valgrind` showing no memory leaks and no
  other memory errors, and
- Code implements the requested solution

You will get no points (failing the exam) if:

- Delivery cannot be extracted with either of the commands tar or zip on a linux
  machine, or
- Delivery does not contain `Makefile` and source code files, or
- Calling `make` does not compile the code on a linux machine, or
- Code does not run on a linux machine with any of the 2 example networks
  provided by us

Otherwise, we will check

- `Makefile`
- Files
   - Correct reading of messages from `data.txt`
   - Correct creation of packets from messages, including correct length and
     addresses
- Memory
   - Memory leaks
   - Correct handling of strings including `0`-termination (allocating
     ridiculously much space for a string is OK)
- TCP connections
   - Code for making TCP connections is correct
   - Waiting for several connections is correct
   - Mapping from socket to node address is correct in routing server, allowing
     it to answer to the right node
- UDP messages
   - Mapping from receiving socket to node address is correct in nodes
   - Mapping from node address to sending socket is correct in nodes
- Route computation and distribution
   - `routing_server` uses a suitable data structure for collecting neighbor
     information from all nodes
   - Shortest path routes from node 1 to all other nodes are computed according
     to Dijkstra's algorithm
   - Route computation is correct (even if the extracted routing tables are
     wrong)
   - A correct routing table is computed for every node (even if it is sent to
     the wrong node)
- Packet sending
   - Packets are routed correctly
   - Packets are implemented in the required format, including network byte
     order and 0-termination
- All functions are integrated correctly.

## Recommendation

It is strongly recommended that you develop your solution for Dijkstra's
algorithm and routing table creation first without the TCP server functionality.
You could read the graphs of the examples that we provide from file, or you
could fill them into hand-written data structures in your code.
If the complete program does not work, you could still get full points for
Dijkstra' algorithm if this is done correctly.

It is similarly recommended to implement nodes first with routing tables that
loaded from file or hand-written in the code.
You can then implement parts of the assignment such as reading of messages from
`data.txt`, UDP sending to a destination, receiving and forwarding of packets.
If done correctly, this could already give full points for "correct handling of
strings", "mapping from socket to node", "mapping from node to socket", and
"packets are in the required format".

If you deliver one program for Dijkstra's algorithm and one program for nodes,
but have not connected them, you will receive points for both parts.
