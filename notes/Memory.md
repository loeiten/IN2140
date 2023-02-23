# Memory

## Concepts

### Multi programming

Modern computers run up to many thousands of processes simultaneously, even
though we only have a limited number of processor cores at our disposal.
The management of many concurrent processes is a central aspect of operating
systems.

### Partitioning

The memory on the computer is divided into partitions.
Depending on which partitioning strategy you choose, the partitions may have
different sizes.

### Segmentation

Segmentation means that a continuous block of data is split up and placed in
different places in memory that are not necessarily connected.
This is done to make better use of memory.
As programmers, we usually do not have to deal with this, since this is
something that is done by the compiler.
Usually, code and data (static variables) are divided into different segments.
Segmentation also acts as a security mechanism, in that each segment has a set
of read, write, and execute accesses.

### Fragmentation

A fragmented memory is a memory that largely consists of small holes spread out,
as a result of poor or poorly adapted segmentation and allocation algorithms.
The consequences of fragmentation are often that space in memory becomes
unusable because there are small, isolated parts.

### Paging

Paging is an important concept in memory management that allows us to "cut"
parts of the memory out of RAM and move it to disk, so that we can load a new
program where the one we moved out was.
Paging algorithms choose which parts of RAM should be moved to disk and are
responsible for retrieving it if someone needs it again.
The processor translates virtual addresses into physical addresses by looking up
paging tables that the operating system sets up.
A separate hardware component called a *Translation Lookaside Buffer* (TLB) is
used to make lookups more efficient.

### Absolute/relative addressing

- *Absolute addressing*: Direct hardware addresses
- *Relative addresses*: Relate to one process's virtual address space.
