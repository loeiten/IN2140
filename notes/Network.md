# Network

## Concepts

### Internet

It is important that we are aware that the Internet (with large I) represents
one specific network.
Internet is a worldwide network that links a lot of computers together.
Through the internet we can visit websites that we are used to or send mail, and
much more.

### End system

An end system is a machine that is connected to a network and that does not
perform special tasks on behalf of the network.

### Protocol

Protocols are what defines the language we use to communicate between computers.
We need many protocols because there are many ways to use the network.

### Protocol stack

A protocol stack is a collection of protocols in different layers, each of which
provides specific properties of the transfer.

### The OSI-model

The OSI model is a theoretical model for how a network can be built into
different logical layers with its own protocols.
This is an important starting point when designing networks in reality.

### Physical/logic communication

Data physically moves through many layers and many cables (or other media), but
the application layers on each end have a logical direct communication between
themselves.
From their point of view, they talk directly to each other and do not care about
the physical communication that happens in between.

### Physical medium

The physical medium is the actual medium through which we transmit signals, e.g.
copper, fiber, coax, Cat5 LAN cable, radio, etc.

### Circuit switching

Circiut switching means that one establishes a logical direct connection between
two machines, which goes through a particular set of routers.
This connection is locked and reserves resources at all stages along the way
until the connection is closed.

### Packet switching

Each packet that is sent contains an address and finds its way to its
destination by itself, with the help of routers along the way.

### Socket

A socket is a virtual port used to create connections over networks.
The idea is that you "plug" the machines with a cable, and that the program must
know what socket the cable is plugged into.
We must have virtual sock because the computer does not have 1000 physical gates,
even if it can have 1000 active connections.

### Client

A client is a program on a machine that actively requests services from a server.
Often the client is the program the user looks at its screen.
For example, Outlook is an email client, which talks to an email server (where
the mail is sent to/from, something the user does not see directly).

### Server

A server is a program that waits for requests from clients and then responds to
them.
A web server accepts requests to send documents (web pages), and responds by
sending the documents.

### Server (machine)

What is often called a server in everyday speech is a machine that is used for
server tasks.
A server (machine) often runs many servers (programs).
It will (hopefully) be clear from the context what is meant.
For simplicity, one can often assume that the server is machine/program
together, as if the machine has no other tasks than running a single server.

### Endianness

Little endian:
Bytes are stored in memory with the least significant byte first (lowest memory
address).

Big endian:
Bytes are stored in memory with the most significant byte first (lowest memory
address).

Because different machines operate differently, but must be able to communicate
over networks, we have to define a standard network order, so that everyone
understands what is being sent and received.
Network order is big-endian, the opposite of what is most common in modern CPUs.
That's why we have to translate!

## Abbreviations

The following abbreviations are common when talking about networks

* NAP: Network Access Point
* ISP: Internet Service Provider
* TCP: Transfer Control Protocol
* IP: Internet Protocol
* HTTP: Hyper Text Transfer Protocol
* FTP: File Transfer Protocol
* PPP: Point-to-Point Protocol
* WWW: World Wide Web
* RFC: Request for Comments
* IETF: Internet Engineering Task Force
* SMTP: Simple Mail Transfer Protocol
* UDP: User Datagram Protocol
* OSI: Open Systems Interconnection
* ISO: International Organisation for Standarization
* XML: Extensible Markup Language
* XDR: External Data Representation
* RTP: Real-time Transfer Protocol
* ack: Acknowledgement
* [K/M/G]bps: [Kilo/Mega/Giga] bit per second. Note that it's *bit* not *byte*.
* ISDN: Integrated Services for Digital Network
* ADSL: Asynchronous Data Subscriber Line
* HFC: Hybrid Fiber Coax
* LAN: Local Area Networks
* IEEE: Institute of Electrical and Electronics Engineers
* TP: Twisted Pair
* WLAN: Wireless Local Area Network
* QoS: Quality of Service
* NTP: Network Time Protocol
* NBP: National Backbone Provider
* NCP: Network Control Protocol
* DNS: Domain Name System
* HTML: Hyper Text Markup Language
