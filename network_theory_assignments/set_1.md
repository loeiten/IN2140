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

## Assignment 2

When you program distributed applications, your programs will
usually use the services of the transport layer. The layer
makes use of the lower layers, including the network layer,
The console program `traceroute` (`tracert` on windows) be used to
show you the path that messages from your computer to another
computers take. To do this, it lists the names and addresses of
the routers between the two end systems.

How many routers are used to forward packets to *www.ifi.uio.no*,
*www.uio.no*, *www.usit.no*, *www.ntnu.no*, *www.kth.se*, *www.fokus.gmd.de*,
*www.cmu.edu*?

Which other information does traceroute give you? (check the man pages)

### Answer assignment 2

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
