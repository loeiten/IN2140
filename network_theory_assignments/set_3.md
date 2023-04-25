# Network theory set 3

These questions are based on bonus questions.

## Assignment 1

Briefly describe the difference between a component based and a recursive
network architecture.

### Answer assignment 1

- Component based
   - Each node has components with specialized responsibilities
   - There will be one component for routing, one for packet loss, one for
     encryption etc.
- Recursive
   - We split the network recursively into smaller and smaller part
   - Each part is responsible for transfer from one network to another
   - Each part does not care how it's smaller parts function

## Assignment 2

What are the most important services TCP provides, both when it comes to
applications and when it comes to network?

### Answer assignment 2

- Connection-oriented communication, i.e. connection and disconnection
- Retransmission of lost packets (reliable transmission)
- Delivery of packages in the correct order
- Flow control: Do not overload the receiver
- Congestion control: Do not overload network components

## Assignment 3

What is bandwidth, throughput, "goodput" (experienced transmission speed for the
application)?

### Answer assignment 3

## Assignment 4

Describe the difference between flow control and congestion control in TCP.
What do we want to achieve with the two?

### Answer assignment 4

## Assignment 5

What five fields are used to uniquely identify a TCP-connection?

### Answer assignment 5

## Assignment 6

Explain the difference between propagation delay (latency), end-to-end delay and
round trip.

### Answer assignment 6

## Assignment 7

Mention one scenario where
[UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol) (User Datagram
Protocol) is superior to
[TCP](https://en.wikipedia.org/wiki/Transmission_Control_Protocol)
(Transmission Control Protocol)

### Answer assignment 7
