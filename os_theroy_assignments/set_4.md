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

#### Answer 1B

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

#### Answer 2B

#### Answer 2C

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
