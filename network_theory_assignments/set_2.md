# Network theory set 2

## Assignment 1

Use the
[client and server code from INF1060](./client_server_example/).

Use them for your tests.

### Answer assignment 1

See [client_server_example](./client_server_example/).

## Assignment 2

Change server and client in such a way that you can give them a port number on
the command line.

### Answer assignment 2

Have instead made it possible to use a `config.txt` file.
See [client_server_example](./client_server_example/).

## Assignment 3

Change the client in such a way, that you can give it a server name and the
server port on the command line.

### Answer assignment 3

Have instead made it possible to use a `config.txt` file.
See [client_server_example](./client_server_example/).

## Assignment 4

Check the man pages of all functions that are used in the programs in
[Assignment 1](#assignment-1) - [Assignment 3](#assignment-3), and make all of
them safer, by checking their return code.

Handle the errors appropriately.

### Answer assignment 4

See [client_server_example](./client_server_example/).

## Assignment 5

Assume that you have a sender that wants to send data packets to a receiver.
The connection that is used is not lossless, so that some of the packets that
you send are lost.
Propose a method (a protocol) that sender and receiver can use to ensure that
all packets arrive.

**Hint:** The receiver must in one way or another confirm the reception of
packets.

### Answer assignment 5

The sender sends the package together with a packet number and a checksum.

The receiver checks the checksum to verify if the package has arrived
uncorrupted.
If the package is uncorrupted send the package number back together with a
checksum.

If the sender does not receive the package number from the receiver after `x`
amount of time, send again.

If the channel is very lossy one could even consider to send redundant packages.

## Assignment 6

Does your answer to [Assignment 5](#assignment-5) guarantee that all packets
arrive in the sending order?
If not, how can you add that?

### Answer assignment 6

A very resource demanding way to do this is to repeatedly send the same package
until a verification is received from the receiver.

## Assignment 7

Assume that you have to send a packet of 1000 bit length over a data channel
that is 2000 km long.
Assume that the signal travels approximately at the speed of light,
300.000 km/s.
How long does it take from the start of sending the packet to the end of
receiving the packet if...

**Hint**: You need time for putting the packet onto the channel, and time for
moving a bit through the channel.

### 7A

the bandwidth of the data channel is 10 Mbit/s

### 7B

the bandwidth of the data channel is 10 Gbit/s

### Answer assignment 7

We can use the following formula:

```text
time = (n_bits/bandwidth) + (distance/velocity)
```

#### Answer 7A

We get

```text
time = (10^3 bits/10^7 bits/s) + (2*10e^3 km/ 3*10e^5 km/s)
     = 10^-4s + 6.6...*10^-3s
```

#### Answer 7B

We get

```text
time = (10^3 bits/10^10 bits/s) + (2*10e^3 km/ 3*10e^5 km/s)
     = 10^-7s + 6.6...*10^-3s
```

## Assignment 8

How does the answer of [Assignment 7](#answer-assignment-7) fit with the
expression that the speed of the channel in example B is 1000 times bigger than
the speed of the channel in example A?

### Answer assignment 8

It's only the bandwidth that has increased, not the velocity in the channel.
This means that you can send 1000 times the amount of data, but not that it
will arrive 1000 times faster

## Assignment 9

Propose an application layer protocol on top of TCP for a distributed,
turn-based game between two players, such as chess (or GO, nine men's morris,
battleships, stratego, ...).

### Answer assignment 9

The game would essentially just need to transfer changes in a text based form.
In chess one could for example use
[algebraic notation](https://en.wikipedia.org/wiki/Algebraic_notation_(chess)),
or even simpler: Indicate just the movement of the pieces.

## Assignment 10

How would you write programs (client and server) that can be used to test the
bandwidth between two machines?
Explain what information your measurements gives you specifically.

**Hint:** Consider layer processing.

### Answer assignment 10

One can use UDP to send a large number of packages (to saturate the network),
but without flow control, congestion control, retransmission or reshuffling.

The bandwidth can then be measured from the (assuming that no packages are lost)
bits send divided by the time. That is, we can inverse the naïve formula:

```text
time = (n_bits/bandwidth) + (distance/velocity)
time - (n_bits/bandwidth) = (distance/velocity)
- (n_bits/bandwidth) = (distance/velocity) - time
(n_bits/bandwidth) = time - (distance/velocity)
(bandwidth/n_bits) = 1/(time - (distance/velocity))
bandwidth = n_bits/(time - (distance/velocity))
```

This is similar to how [iperf](https://iperf.fr/) measures the bandwidth.

One can further look for bottlenecks in the hardware between the client and
server (like the CPU, NIC, IO-bus), in the software (OS, drivers, other
processes, implementation of the server and the client) and in the network
(network components between the machines, network links between the component).
