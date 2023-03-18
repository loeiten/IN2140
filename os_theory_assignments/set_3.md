# OS theory set 3

## Assignment 1

- What kind of fragmentation will you be able to find in virtual memory?
- Is this a serious problem? Justify the answer.
- Discuss the relationship between fragmentation and page size.

### Answer assignment 1

[Internal fragmentation](https://en.wikipedia.org/wiki/Fragmentation_(computing)#Internal_fragmentation)
happens when the memory for a process is allocated, but the program doesn't
require the full amount of memory.

[External fragmentation](https://en.wikipedia.org/wiki/Fragmentation_(computing)#External_fragmentation)
arises when free memory is separated into small blocks and is interspersed by
allocated memory.

In virtual memory the memory is divided into equal-sized frames called
[pages](https://en.wikipedia.org/wiki/Page_(computer_memory)).
Some pages reside in the physical memory whereas others reside on a secondary
medium.
The virtual addresses are translated to physical addresses in the memory
management unit (MMU).

With paging, there will be no external fragmentation (as the pages need not be
consecutive).
There may be some internal fragmentation for the last page needed for the
process.
The larger the page size the larger the internal fragmentation.

Usually this does not pose a serious problem as long as the pagesize is set to
a sensible size depending on the work load. The smallest page size is usually 4
KiB (`2^12` bytes)

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

This distribution will give processes which are soon completed a higher
priority.
When the process is at its latest time quantum it will get progressively in the
front of the queue if there are any interrupts.

#### Answer 2B

This has the advantage that processes are completed faster, which means that
resources can be freed faster. Given that we know the number of processes in the
queue, insertion can be done in `O(1)` time as one can use indices.

At the same time it does not distinguish between long running process and short
running processes.

#### Answer 2C

The Linux CPU scheduler uses a algorithm called
[Completely Fair Scheduler (CFS)](https://en.wikipedia.org/wiki/Completely_Fair_Scheduler)
and uses a red/black tree sorted by CPU time.
This gives a low overhead on insertion and sorting, and the lookup for the
next process can be done in `O(log(N))` time.
There is one three per core.
There is one thread per core for migration of threads between the cores.

## Assignment 3

The designer of a new operating system to be used especially for real-time
applications has proposed the use of virtual memory management so that the
system can handle programs that are too large to fit into the limited memory
area sometimes offered by real-time systems.

What are the implications of this decision in relation to the way virtual memory
works?

### Answer assignment 3

As it takes much longer time to access from disk than from memory, it means that
the latency it takes for replacing a page between the memory and disk must be
tolerable for the application running.

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

A [page fault](https://en.wikipedia.org/wiki/Page_fault) is an exception that
the memory management unit (MMU) raises when a process accesses a memory page
without proper preparations.
Accessing the page requires a mapping to be added to the process's virtual
address space.
Besides, the actual page contents may need to be loaded from a backing store,
such as a disk.
The MMU detects the page fault, but the operating system's kernel handles the
exception by making the required page accessible in the physical memory or
denying an illegal memory access.

#### Answer 4A

Since there are no pages in memory at startup we would have the following:

- Accessing page `0` results in a PF, the FIFO queue `0`
- Accessing page `1` results in a PF, the FIFO queue `10`
- Accessing page `7` results in a PF, the FIFO queue `710`
- Accessing page `2` results in a PF, the FIFO queue `2710`
- Accessing page `3` results in a PF, the FIFO queue `3271`
- Accessing page `2` does not result in a PF, the FIFO queue `3271`
- Accessing page `7` does not result in a PF, the FIFO queue `3271`
- Accessing page `1` does not result in a PF, the FIFO queue `3271`
- Accessing page `0` results in a PF, the FIFO queue `0327`
- Accessing page `3` does not result in a PF, the FIFO queue `0327`

Hence we get 6 page faults

#### Answer 4B

If we use LRU we will get the following

- Accessing page `0` results in a PF, the LRU queue `0`
- Accessing page `1` results in a PF, the LRU queue `10`
- Accessing page `7` results in a PF, the LRU queue `710`
- Accessing page `2` results in a PF, the LRU queue `2710`
- Accessing page `3` results in a PF, the LRU queue `3271`
- Accessing page `2` does not result in a PF, the LRU queue `2371`
- Accessing page `7` does not result in a PF, the LRU queue `7231`
- Accessing page `1` does not result in a PF, the LRU queue `7213`
- Accessing page `0` results in a PF, the LRU queue `0721`
- Accessing page `3` results in a PF, the LRU queue `372`

Hence we get 7 page faults

#### Answer 4C

A usual implementation consists of a doubly linked list (DLL) and a hash-map
mapping the key to the pointer of node in the DLL.
In addition one must correctly implement:

1. The fill-up of the queue
1. The update policy if the element is present (move the node to the front)
1. The eviction policy (remove the element from the DLL and the hash-map)

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
