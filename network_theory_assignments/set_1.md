# Network theory set 1

## Assignment 1

The applications running on your computer can only send messages
to applications running on another computer when the Internet
address of that computer is known. We prefer to use names instead
of these addresses, e.g. the name *safir.ifi.uio.no* instead of the
address *129.240.2.27*, but sometimes we need the address.

You can use the console programs `host` and `nslookup` to find names for
addresses and addresses for names. If they are not installed on
your computer, log in to *login.ifi.uio.no* to test the commands.

What interesting things do you find out when you try, for example,
`host ifi.uio.no` and `host login.ifi.uio.no`?

### Answer assignment 1

- `host` is a simple utility for performing DNS lookups.
  It is normally used to convert names to IP addresses and vice versa.
- `nslookup` is a program to query Internet domain name servers.
  Nslookup has two modes: interactive and non-interactive.
  Interactive mode allows the user to query name servers for information about
  various hosts and domains or to print a list of hosts in a domain.
  Non-interactive mode is used to print just the name and requested information
  for a host or domain.

```bash
$ host ifi.uio.no
ifi.uio.no has address 129.240.64.2
ifi.uio.no mail is handled by 10 smtp.uio.no.

$ nslookup ifi.uio.no
Server:         192.168.1.154
Address:        192.168.1.154#53

Non-authoritative answer:
Name:   ifi.uio.no
Address: 129.240.64.2
```

```bash
$ host login.ifi.uio.no
login.ifi.uio.no is an alias for rh8login.ifi.uio.no.
rh8login.ifi.uio.no has address 129.240.65.61
rh8login.ifi.uio.no has address 129.240.65.60
rh8login.ifi.uio.no has address 129.240.65.58
rh8login.ifi.uio.no mail is handled by 50 smtp.uio.no.

$ nslookup login.ifi.uio.no
Server:         192.168.1.154
Address:        192.168.1.154#53

Non-authoritative answer:
login.ifi.uio.no        canonical name = rh8login.ifi.uio.no.
Name:   rh8login.ifi.uio.no
Address: 129.240.65.58
Name:   rh8login.ifi.uio.no
Address: 129.240.65.61
Name:   rh8login.ifi.uio.no
Address: 129.240.65.60
```

```bash
$ host google.com
google.com has address 142.250.74.78
google.com has IPv6 address 2a00:1450:400f:802::200e
google.com mail is handled by 10 smtp.google.com.

$ nslookup google.com
Server:         192.168.1.154
Address:        192.168.1.154#53

Non-authoritative answer:
Name:   google.com
Address: 142.250.74.78
```

## Assignment 2

When you program distributed applications, your programs will
usually use the services of the transport layer. The layer
makes use of the lower layers, including the network layer.
The console program `traceroute` (`tracert` on windows) be used to
show you the path that messages from your computer to another
computers take. To do this, it lists the names and addresses of
the routers between the two end systems.

How many routers are used to forward packets to *www.ifi.uio.no*,
*www.uio.no*, *www.usit.no*, *www.ntnu.no*, *www.kth.se*, *www.fokus.gmd.de*,
*www.cmu.edu*?

Which other information does traceroute give you? (check the man pages)

### Answer assignment 2

- `traceroute` prints the route packets take to network host.
  Tracking the route one's packets follow (or finding the miscreant gateway
  that's discarding your packets) can be difficult.
  `traceroute` utilizes the IP protocol `time to live' field and attempts to
  elicit an ICMP TIME_EXCEEDED response from each gateway along the path to some
  host.

```bash
$ traceroute www.cmu.edu
traceroute to www.r53.cmu.edu (128.2.42.52), 64 hops max, 52 byte packets
 1  ***.***.***.*** (***.***.***.***) *.*** ms  *.*** ms  *.*** ms
 2  ***.***.***.*** (***.***.***.***) *.*** ms  *.*** ms  *.*** ms
 3  ***.***.***.*** (***.***.***.***) *.*** ms  *.*** ms  *.*** ms
 4  ***.***.***.*** (***.***.***.***) *.*** ms
    ***.***.***.*** (***.***.***.***) *.*** ms  *.*** ms
 5  ae5.cr1.fn3.as2116.net (195.0.241.200)  2.701 ms *
    ae5.cr1.oslosda310.as2116.net (195.0.241.202)  3.222 ms
 6  ae12.br1.fn3.as2116.net (195.0.244.234)  4.237 ms  2.433 ms
    ae11.br1.fn3.as2116.net (195.0.244.48)  2.353 ms
 7  nix-ix.core1.osl1.he.net (185.1.55.90)  2.504 ms  2.864 ms  2.824 ms
 8  * * 100ge0-35.core2.cph1.he.net (184.104.194.222)  15.196 ms
 9  ve951.core1.ewr5.he.net (184.104.196.98)  90.617 ms * *
10  * * *
11  * * *
12  * * *
13  216.56.50.86 (216.56.50.86)  445.698 ms  203.814 ms  117.548 ms
14  * * *
15  core0-pod-i-cyh-8500.gw.cmu.net(128.2.0.177)132.041 ms 220.728 ms 119.875 ms
16  pod-d-cyh-core0.gw.cmu.net (128.2.0.202)  151.932 ms  199.466 ms  120.321 ms
17  www-cmu-prod-vip.andrew.cmu.edu(128.2.42.52)177.214 ms 119.958 ms 194.422 ms
```

The `***.***.***.*** (***.***.***.***) *.*** ms  *.*** ms  *.*** ms` are
redacted, whereas `dd  * * *` don't send ICMP "time exceeded" messages or send
them with a ttl too small to reach us

## Assignment 3

Why are network functions organized in layers?

### Answer assignment 3

- It gives a logical separation of tasks
- Every layer utilized the services provided by the layer below
- This gives an abstraction such that one necessarily don't need to have deep
  knowledge about the layers below
- A network can use different link technologies
- A network can handle multiple protocols
- A network can be used by a variety of programs

## Assignment 4

### 4A

Which layers do we usually distinguish in the Internet?

### 4B

What are the tasks of each of these layers?

### Answer assignment 4

#### Answer 4A

```text
application          application
    |                    |
 transport            transport
    |                    |
 network    network   network
    |       |    |       |
  link       link      link
    |       |    |       |
physical _ /      \_ physical
```

#### Answer 4B

- Application: Communication between processes on different machines
- Transport: End-to-end communication between machines (TCP/UDP)
- Network: Routing protocol (IP)
- Link: Communication between neighbors in the network
- Physical: The medium connecting two neighbors together

## Assignment 5

### 5A

Which additional layers exist in the OSI model when compared to the
TCP/IP-model?

### 5B

What additional services do they offer?

### 5C

How are these services offered in the Internet (TCP/IP model)?

### Answer assignment 5

#### Answer 5A

The OSI model consist of a session layer after the transport layer and a
presentation layer after the session layer and before the application layer

#### Answer 5B

The session layer offers to sustain a session, whereas the presentation layer
translates data between machines. I.e. the presentation layer handles the:

- Character set
- Compression
- Encoding
- Encryption

#### Answer 5C

 These layers are often baked into the application layer in TCP/IP

## Assignment 6

How would you characterize the differences between circuit switching
and packet switching?

### Answer assignment 6

Packet switching means that each router decided the next link based on the
address and routing protocol.
Two packages heading the same way can take different paths through the network.
In package switching there is no guarantees about the capacity, but it gives a
better utilization of the network than circuit switching.

Circuit switching means that one establishes a logical direct connection between
two machines, which goes through a particular set of routers.
This connection is locked and reserves resources at all stages along the way
until the connection is closed.

Each packet that is sent contains an address and finds its way to its
destination by itself, with the help of routers along the way.

## Assignment 7

### 7A

Why are headers necessary in protocols?

### 7B

How are headers added by different layers of the protocol stack?
How are the headers removed when received by another end system?

### 7C

Why is it typical to add headers at the front of the packet?

### 7D

Some layers or protocols may also add trailers behind messages, why?

### 7E

Some protocols and layers purposefully do not have trailers, why might that be?

### Answer assignment 7

#### Answer 7A

It contains information about how the protocol should handle the data.
This can include destination address, protocol number, length, type etc.

#### Answer 7B

A layer adds headers to the data it received in the layer above and delivers
headers and data to the layer below.

The headers are removed before the data is delivered to the layer above.

#### Answer 7C

Headers are put in front of the data as this makes it easier to separate the
header from the data.

#### Answer 7D

Trailers are often used in lower layers to separate "frames"

#### Answer 7E

Having trails meaning that one have to iterate through the body before coming to
the trail, this means that extra processing which is resource consuming and can
be wasteful in time critical systems.
