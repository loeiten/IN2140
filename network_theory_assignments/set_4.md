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
.
I.e. it's a process managing the rate of data transmission between two nodes in
a network in order not to overwhelm a slow receiver (in particular that the
receiver buffer must not overflow).

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

Is Selective Repeat always better than Go-Back-N?

### Answer assignment 2

#### Answer 2A

The [sliding window](https://en.wikipedia.org/wiki/Sliding_window_protocol)
protocol is a flow control algorithm where several packages can be in flight
(i.e. that they are sent but not acknowledged) simultaneously.

Both Go-Back-N and Selective Repeat are implementations of the Sliding Window
protocol, and they only differ in their retransmission.
Both of these have a `timeout` as with the Stop-N-Wait algorithm.

The packages are given a sequence number `SeqNo`.
The algorithm sets the size of the sequence to `max(SeqNo)` and a window
size `WS`, where `max(SeqNo) > WS`.

Both the sender side and the receiver side keep track of their own lower bound
`LB` and an upper bound `UB`, which have slightly different meaning for the two
sides.

On the sender side we have that:

- `LB` is the oldest `SeqNo` that is still unconfirmed
- `UB` is the next `SeqNo` to be sent
- If `UB==LB` then nothing is in flight and the sender is idle
- The sender can send packages as long as `(LB + WS) % max(SeqNo) < UB`
- When a package is sent, then `LB` is increased (modulo `max(SeqNo)`)
- When an `ACK` is received, then `UB` is increased (modulo `max(SeqNo)`)

On the receiver side we have that:

- `LB` is the lowest valid `SeqNo` that should be `ACK`ed
- `UB` is the highest valid `SeqNo` that should be `ACK`ed + 1
- If `UB==LB` then the buffer is full
- If `UB==(LB + WS) % max(SeqNo)` there is nothing to `ACK`
- When a package is received, then `LB` is increased (modulo `max(SeqNo)`)
- When an `ACK` is sent, then `UB` is increased (modulo `max(SeqNo)`)

#### Answer 2B

For [Go-Back-N](https://en.wikipedia.org/wiki/Go-Back-N_ARQ) the receiver only
accepts packages in order.
Packages not arriving in order are discarded and not acknowledged.
This means that the receiver only needs to buffer for one packet.
If the network usually looses more than one packet in a row, this fixes the
problem quickly.

If either a packet or `ACK` is lost, then all packages in that window will be
resent.

#### Answer 2C

For [Selective Repeat](https://en.wikipedia.org/wiki/Selective_Repeat_ARQ) the
receiver accepts packages out of order, but in window.
This has the advantages that it will lead to less overall traffic.
Also if the network usually looses only one packet in a row, this uses the
bandwidth better.

Unacknowledged packets in the window will be retransmitted after the `timeout`
has expired.

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
