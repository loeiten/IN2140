# Self test OSes

From <https://web.archive.org/web/20070102084427/http://www.ifi.uio.no/~inf1060/cgi-bin/fv.cgi?filename=Operativsystemer_1.txt>

## Question 1

The resident part of an operating system is

- [] the part of the OS that resides on the disk and is loaded when needed
- [] the part of the OS that keeps track of where the files are (reside)
- [x] the part that is in the RAM/internal memory
- [] the disk management system
- [] the network management system for "this" side of a network connection

## Question 2

An operating system ensures that processes do not destroy for each other by

- [] it ensures that there is never more than one process using a resource
- [x] access to shared resources must be done through calls to operating system
  functions
- [] it reviews the program code of the process before it starts up (high-level
  scheduling)
- [] it has backup functionality that can correct any problems that may occur
- [] only run one process at a time
- [] locks those resources the running process is not allowed to touch

Comments:

- More than one process can share a
 [resource](https://en.wikipedia.org/wiki/System_resource)

## Question 3

A scheduling strategy that does not throw a process out of the CPU before the
process signals that it is OK is called

- [x] non-preemptive
- [] preemptive
- [] resident
- [] non-resident
- [] polite
- [] non-polite

## Question 4

FIFO scheduling is

- [x] basically fair, but negative for jobs with a lot of I/O
- [] basically fair and especially positive for jobs with a lot of I/O
- [] unfair, starvation is possible, and especially negative for jobs with a lot
  of I/O
- [] unfair, starvation is possible, but positive for jobs with a lot of I/O

Comments:

- Fairness: Giving each process a fair share of the CPU
  Giving one process much more CPU time than an equivalent one is not fair
- FIFO is not preemptive

## Question 5

Shortest-job-first scheduling is

- [] positive for short jobs, and provides a consistent response time
- [x] positive for short jobs, but gives inconsistent response time
- [] avoids starvation, but gives inconsistent response time
- [] does not avoid starvation, but provides a consistent response time
- [] mostly always suitable
- [] almost always unsuitable

Comments:

- SJFS is not preemptive, thus avoids starvation
- Starvation is possible as new and shorted jobs may come

## Question 6

Round-robin scheduling is

- [] good in terms of resource utilization, but fundamentally unfair
- [x] fundamentally fair, but provides poor resource utilization
- [] positive for jobs with a lot of I/O, but gives poor resource utilization
- [] negative for jobs with a lot of I/O, but provides good resource utilization
- [] mostly always suitable
- [] almost always unsuitable

Comments:

- Each process gets a max time before being preempted
- Short time slices can improve utilization, but context switches cost

## Question 7

Multilevel queues are

- [] good in terms of resource utilization, but fundamentally unfair
- [x] fundamentally fair, but provides poor resource utilization
- [] positive for jobs with a lot of I/O, but gives poor resource utilization
- [] negative for jobs with a lot of I/O, but provides good resource utilization
- [] mostly always suitable but can lead to starvation
- [] almost always unsuitable, but suitable if starvation must be avoided

Comments:

- All processes are given highest priority and is let run for 1 quanta
- When finished, the quanta is doubled and the process is given 1 less priority
- Can leave to starvation if lower priority jobs always enter

## Question 8

The difference between disk management and file system is

- [] no difference at all
- [] filesystem is what deals with one disk, while disk management deals with
  multiple disks
- [x] disk management has to do with handling/reading/writing the disk, while
  the file system builds concepts such as files/folders/directories so that the
  underlying disk technology is hidden
- [] file system is something that the individual user has, and disk
  administration consists of managing the file system of many users
- [] ??? there is no such thing as disk management

## Question 9

Multi-user single-task operating systems are

- [] very common in today's operating systems
- [x] common in older systems, but on the way out
- [] pointless - if there are several users, it must be multi-tasking
- [] primarily used in cars and mobile phones
- [] appropriate in disk-less workstations
- [] a configuration option for most modern systems

## Question 10

Which of these statements are false?

- [] A real-time OS must have preemptive scheduling if it supports multi-tasking
- [] In a single-tasking single-user operating system, we don't need system
  protection
- [x] The file system in a modern OS is often based on the operating system's
  network functionality
- [] In a multi-tasking operating system, memory management is far more complex
  than in a single-tasking system
- [] Context switching is something the dispatcher does, and it is not part of
  the high-level scheduling

## Question 11

Virtual memory

- [] solves problems related to overlay technique, but neither process integrity
  nor fragmentation
- [] does not solve problems related to overlay technique, but in return process
  integrity and fragmentation are handled
- [x] solves problems related to overlay technique and process integrity but not
  fragmentation
- [] solves problems related to overlay technique, process integrity and
  fragmentation
- [] solves problems related to overlay technique and fragmentation, but not
  process integrity
- [] has nothing to do with either overlay technique, process integrity or
  fragmentation

## Question 12

Memory relocation

- [] solves problems related to overlay technique, but neither process integrity
  nor fragmentation
- [] does not solve problems related to overlay technique, but in return process
  integrity and fragmentation are handled
- [x] solves problems related to overlay technique and process integrity but not
  fragmentation
- [] solves problems related to overlay technique, process integrity and
  fragmentation
- [] solves problems related to overlay technique and fragmentation, but not
  processor integrity
- [] has nothing to do with either overlay technique, process integrity or
  fragmentation

## Question 13

The following pages should never be swapped out

- [] the page with process state information
- [] the program code of a process
- [] the part of the program code of a process that contains the code that the
  process is currently working on
- [] the security system in the operating system
- [x] the disk driver in the operating system
- [] the user table in the operating system

Comments:

- If the instructions on how to read is no longer available, then how would one
  read that part

## Question 14

The difference between a page and a frame is that

- [x] a frame is a segment of the internal memory of a machine, while a page is
  a part of the memory requirement of a process that fits exactly into a frame
- [] a page is a segment of the internal memory of a machine, while a frame is a
  part of the memory requirement of a process that fits exactly into a page
- [] there is no difference
- [] a frame is a segment of a disk, while a page is part of the memory
  requirements of a file
- [] a page is a segment of a disk, while a frame is part of the memory
  requirements of a file
