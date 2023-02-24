# OS theory set 3

## Assignment 1

- What kind of fragmentation will you be able to find in virtual memory?
- Is this a serious problem? Justify the answer.
- Discuss the relationship between fragmentation and page size.

### Answer assignment 1

## Assignment 2

Assume that we use a variant of the traditional circular queue routine ("round
robin") when distributing processor time to processes.
A process that has fully used up its time quantum is placed at the back of the
queue.
A process that uses half of the allocated time quantum is placed in the middle
of the queue, and a process that only uses a quarter of the allocated time
quantum is placed a quarter behind the queue from the start.
(We are talking about approximate numbers here.)

### 2A

What is the purpose of such a distribution?
Justify your answer briefly.

### 2B

Discuss the pros and cons of its implementation.

### 2C

Hod does the Linux CPU scheduler work?

### Answer assignment 2

#### Answer 2A

#### Answer 2B

#### Answer 2C

## Assignment 3

The designer of a new operating system to be used especially for real-time
applications has proposed the use of virtual memory management so that the
system can handle programs that are too large to fit into the limited memory
area sometimes offered by real-time systems.

What are the implications of this decision for the way virtual memory works?

### Answer assignment 3

## Assignment 4

Assume we have a small computer with space for only 4 pages in memory.
On this computer we are running a computer program that needs 8 pages of memory,
but fortunately does not need all the pages at the same time.
The computer must occasionally exchange pages to disk (page
requirements/swapping).
When running the program, the pages are referenced in this order:

```text
0172327103
```

where 0 is the first page.
At startup, the memory is completely empty.
The system uses the FIFO algorithm to determine which page is to go out when a
new page is to go in.

### 4A

How many page faults would occur assuming no pages were in memory at startup?

### 4B

How many page faults occur if the system uses LRU (Least Resently Used)?

### 4C

In theory, LRU seems like a nice algorithm.
Why is it so difficult to implement in practice?

### Answer assignment 4

#### Answer 4A

#### Answer 4B

#### Answer 4C

## Assignment 5

If an instruction takes `10` nanoseconds and a pagefault takes another `n`
nanoseconds - give a formula for effective instruction time if we have a
pagefault every k'th instruction.

### Answer assignment 5

## Assignment 6

A machine with 32-bit virtual addresses uses a two-level page table.
The virtual addresses are split into a 9-bit top level page table field, an
11-bit second-level page table and an offset.
How big are the pages in the system and how many pages are there in the system?
Does it matter if in another system we use 11 bits on the first level and 9 on
the second?

### Answer assignment 6

## Assignment 7

A machine has 48-bit virtual addresses and 32-bit physical addresses.
The page size is 8 KB.
How many pages can fit in the machine?

### Answer assignment 7

## Assignment 8

Take some time to get familiar with environment variables in Linux.

### Answer assignment 8

## Assignment 9

In Linux, you can get a lot of information about the system by looking at the
`/proc` file system.
Take some time to look at the kind of information you can find here.

### Answer assignment 9

## Assignment 10

Try the [self test about OSes](./self_test_OSes.md)
