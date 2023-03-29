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

When moving a file on the same device on a Unix-style system, one have to add
and remove the [inode](https://en.wikipedia.org/wiki/Inode) number in the
directory.

The `inode` description can be obtained by the `stat` system call.
For example `stat programming_assignments` (on macOS) returns

```text
# Device ID
16777225
# File serial number (inode number)
39679811
# File mode
## Directory
d
## First triad: What the owner can do (read, write, execute)
rwx
## Second triad: What the group members can do (read and execute)
r-x
## Third triad: What others can do (read and execute)
r-x
# Link count (how many hard links point to the file)
6
# User ID of file owner
michael
# Group ID
staff
# Device ID if file is a device file
0
# Size of the file in bytes
192
# Timestamp
## When the inode was modified
"Feb 13 13:28:48 2023"
## When the content was modified
"Feb 13 13:28:42 2023"
## When the last accessed time was
"Feb 13 13:28:42 2023"
## When the files was created
"Feb  9 20:39:46 2023"
# Preferred I/0 block size
4096
# Number of blocks allocated to this file
0
# User defined flags for file
0
# Name
programming_assignments
```

and `stat -s CMakeLists.txt` returns

```text
st_dev=16777225 st_ino=39679849 st_mode=0100644 st_nlink=1 st_uid=501 st_gid=20
st_rdev=0 st_size=72 st_atime=1676297876 st_mtime=1676291330 st_ctime=1676291330
st_birthtime=1675971648 st_blksize=4096 st_blocks=8 st_flags=0
```

See
[What is the meaning of each field with the output of `stat` on OSX?](https://stackoverflow.com/questions/54263830/what-is-the-meaning-of-each-field-with-the-output-of-stat-on-osx)
for more info.

When data is copied between devices, the data needs to be copied.
This requires the metadata about the file to be edited

## Assignment 4

List a number of file types that you will assume are accessed sequentially.
Do the same for files that you would expect to require random access.

### Answer assignment 4

Sequential access:

- Media files
   - Images
   - Sound
   - Video
- Text

Random access:

- Databases
- Compressed archives

## Assignment 5

What are the advantages of partitioning a disk instead of using the entire disk
as a partition?

### Answer assignment 5

- You can have several file systems without having several disks
- You will limit the amount of space needed to organize the file system

## Assignment 6

What is the purpose of the `open` and `close` operations?
What does the `open` and `close` operations do?

### Answer assignment 6

- `open`:
   - Check that the file exists
   - Mark that the file is used by a process
   - Make a file descriptor and return this
- `close`:
   - Remove the file descriptor and clean up the internal kernel structure
     associated with the file
   - Remove the mark of the file so that other processes can use it

### Assignment 7

Create a program that opens the same file for writing from two different
processes using `fork()`.
What is happening? Why?

### Answer assignment 7

See [set_4_assignment_7.c](set_4_assignment_7.c)

When the processes open the file individually:

- The processes will have different file table entries, so each process has its
  own file position for the file
- Thus the processes will overwrite each other
- (Note the `wait` statement to avoid racing)

When the file is opened prior to the fork:

- The processes will have the same file table entries, so both processes share
  the position in the file
- The file is written to sequentially
- (Note the `wait` statement to avoid racing)

In general though:

> So having multiple processes write to the same file is a recipe for disaster.

Furthermore in linux:

- `FILE *` refers to a struct
- `open` returns an `int` which refers to the
  [file descriptor](https://en.wikipedia.org/wiki/File_descriptor)
   - `0` is reserved to `stdin`
   - `1` is reserved to `stdout`
   - `2` is reserved to `stderr`
- Each process has a file descriptor table
   - This points to a global file table shared across all processes
      - The global file table contains
         - A reference count (how many times is this file referenced by other
          processes)
         - The position in the file
         - ...
      - The global file table points to the v-node table which tells about
         - File access
         - File type
         - File size
         - ...

## Assignment 8

Create a program that writes the message *hello world* to a file using the
low-level I/O (unix filesystem interface) calls `open()` and `write()`.

### Answer assignment 8

See [set_4_assignment_7.c](set_4_assignment_8.c)
