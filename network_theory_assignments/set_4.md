# Network theory set 4

These questions are based an additional assignment set found in the
[Schedule](https://www.uio.no/studier/emner/matnat/ifi/IN2140/v19/timeplan/index.html#FOR)
.

## Assignment 1

Describe the Stop-N-Wait algorithm.
What is its pros and cons?

### Answer assignment 1

The [Stop-N-Wait algorithm](https://en.wikipedia.org/wiki/Stop-and-wait_ARQ)
is a [flow control algorithm](https://en.wikipedia.org/wiki/Flow_control_(data))
(i.e. a process managing the rate of data transmission between two nodes in a
network in order not to overwhelm a slow receiver).

The algorithm has two features:

1. If a packet gets lost, the algorithm will fix it
1. The algorithm will distinguish a packet loss and an `ACK` loss

It works as follows:

1. The sender starts a timer
1. The sender subsequently sends a packet with a single bit indicating the
   packet number
1. The sender then waits for an `ACK` from the receiver
1. If it receives an `ACK` with the correct bit it will:
    1. Restart the timer
    1. Send a new packet with the bit flipped
1. If it does not receive an `ACK` from the receiver within the time frame
    1. It will resend the previous packet

Pros:

- The algorithm is relatively simple

Cons:

- Only one frame can be sent at a time as the sender cannot transmit a new
  packet until it has received an `ACK`
- The sender needs to wait for the `ACK` after every frame in transit
- The connection is idle most of the time
- This gives poor throughput

## Assignment 2

### 2A

Describe Sliding Window

### 2B

Describe Go-Back-N

### 2C

Describe Selective Repeat

### 2D

Is Selective repeat always better than Go-Back-N?

### Answer assignment 2

#### Answer 2A

#### Answer 2B

#### Answer 2C

#### Answer 2D

## Assignment 3

Given a transmission speed of 100 Mbps, RTT on 0.5 ms and a packet size of 1000
bytes. How much of the capacity is utilized by Stop-N-Wait?

### Answer assignment 3

## Assignment 4

What defines the congestion window?

### Answer assignment 4

## Assignment 5

How does the congestion controller try to achieve the correct
bandwidth performance?

### Answer assignment 6

## Assignment 7

What is the purpose of DNS and how is it structured

### Answer assignment 7

## Assignment 8

What is the difference between recursive and iterative DNS queries?
What are the pros and cons of each?

### Answer assignment 8
