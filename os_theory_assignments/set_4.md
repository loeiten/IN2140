# OS theory set 4

## Assignment 1

Suppose that at a particular time the disk reads a sector from track 50.
The disk head is on its way from low to high numbers.
Following this request, the following requests are in the queue (each number
refers to which track the requested block is on, and the ordering shows the order
in which the requests arrived the system, i.e. 2 came first):

```text
99 55 43 78 4 11 89 67 1 98 45 88 60 30 77 2
```

### 1A

Show in a graphic figure how the disk head moves over the different grooves if
we use *First-Come-First serve* (FCFS) and SCAN respectively.

### 1B

Additionally, assume that:

1. moving the head takes 10 ms per track (constant time per track),
1. the disc rotates 7500 rounds per minute.
1. reading a block takes 1 ms per block.

Based on this information, calculate the time it takes for the entire queue of
requests to be executed if we assume an average (half revolution) rotation delay
per request for the FCFS and SCAN schedulers.

### Answer assignment 1

#### Answer 1A

FCFS:

`(50), 2, 77, 30, 60, 88, 45, 98, 1, 67, 89, 11, 4, 78, 43, 55, 99`

```text
     (50) -> |
| <-   2  <-
  ->  77  -> |
| <-  30  <-
  ->  60  -> 88 -> |
| <-         45 <-
  ->  98  -> |
| <-   1  <-
  ->  67  -> 89 -> |
| <-   4  <- 11 <-
  ->  78  -> |
| <-  43  <-
  ->  55  -> 99 -> |
```

SCAN:

`(50), 55, 60, 67, 77, 78, 88, 89, 98, 99, 45, 43, 30, 11, 4, 2`

```text
     (50) -> 55 -> 60 -> 67 -> 77 -> 78 -> 88 -> 89 -> 98 -> 99 |
| <-                            2 <-  4 <- 11 <- 30 <- 43 <- 45
```

#### Answer 1B

```text
Formula:
time = sum(d)*t_t +  r*((1/2)round*(1/rpms) + t_r)

Where:
d    - distance
t_t  - time to move between two neighboring tracks
r    - blocks to read
rpms - rounds per ms
t_r  - time to read 1 block

We have:
t_t  = 10 ms
r    = 15
rmps = 7 500 rpm / (60 s/m * 1000 ms/s) =  0.125 rpms
t_r  = 1 ms

Hence:
time = sum(d)*10 ms + 15*(0.5 rounds *8 ms/round + 1 ms) = sum(d)*10 ms + 75 ms

FCFS:
-----
sum(d) = (50-2) + (77-2) + (77-30) + (60-30) + (88-60) + (88-45) + (98-45) +
(98-1) + (67-1) + (89-67) + (89-11) + (11-4) + (78-4) + (78-43) + (55-43) +
(99-55) = 759

time = 759*10 ms + 75 ms = 7665 ms

SCAN:
-----
sum(d) = (55-50) + (60-55) + (67-60) + (77-67) + (78-77) + (88-78) + (89-88) +
(98-89) + (99-98) + (99-45) + (45-43) + (43-30) + (30-11) + (11-4) + (4-2) = 146

time = 146*10ms + 75 = 1535 ms
```

## Assignment 2

### 2A

Let's imagine a scenario from 1995:

You've noticed that loading programs from one of your floppy disks seems to take
longer than it used to.
A friend suggests copying the files from your disk, one by one, to a new floppy
disk.
You do this and find that the programs load much faster now. What happened?

### 2B

Do we experience the same problem today? Why/why not?

### 2C

Why do we *not* have this problem on USB sticks or SSDs?

### Answer assignment 2

#### Answer 2A

This happens due to
[fragmentation](https://en.wikipedia.org/wiki/File_system_fragmentation) of
the disk.
As the files becomes fragmented and scattered around on a disk it will take
longer time to load as the head needs to search.

As the fragmentation is drive specific the fragmentation will not be preserved
across a copy.

#### Answer 2B

We still experience the same problems today as fragmentation naturally happens
due to appending, truncation and deletion of files, and since the mechanical
drives still needs to search for the relevant bits of information across the
medium.

#### Answer 2C

We will not experience this problem on an USB stick or SSD as these technologies
does not include a rotating disk and a search head.
They are more comparable to how the memory works.

## Assignment 3

Explain why a `move` operation from one device to another requires manipulation
of the file itself, while a `move` operation from one place to another on the
same device only involves manipulation of the directory.

### Answer assignment 3

## Assignment 4

List a number of file types that you will assume are accessed sequentially.
Do the same for files that you would expect to require random access.

### Answer assignment 4

## Assignment 5

What are the advantages of partitioning a disk instead of using the entire disk
as a partition?

### Answer assignment 5

## Assignment 6

What is the purpose of the `open` and `close` operations?
What does the `open` and `close` operations do?

### Answer assignment 6

### Assignment 7

Create a program that opens the same file for writing from two different
processes using `fork()`.
What is happening? Why?

### Answer assignment 7

## Assignment 8

Create a program that writes the message *hello world* to a file using the
low-level I/O (unix filesystem interface) calls `open()` and `write()`.

### Answer assignment 8
