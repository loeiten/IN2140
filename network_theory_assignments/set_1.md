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

## Assignment 4

### 4A

Which layers do we usually distinguish in the Internet?

### 4B

What are the tasks of each of these layers?

### Answer assignment 4

#### Answer 4A

#### Answer 4B

## Assignment 5

### 5A

Which additional layers exist in the OSI model when compared to the TCP/IP-model?

### 5B

What additional services do they offer?

### 5C

How are these services offered in the Internet (TCP/IP model)?

### Answer assignment 5

#### Answer 5A

#### Answer 5B

#### Answer 5C

## Assignment 6

How would you characterize the differences between circuit switching
and packet switching?

### Answer assignment 6

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

#### Answer 7B

#### Answer 7C

#### Answer 7D

#### Answer 7E
