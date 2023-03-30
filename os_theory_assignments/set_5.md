# OS theory set 5

## Assignment 1

What types of IPC (locally on a machine) do we know in Unix/Linux?

### Answer assignment 1

[Inter process communication](https://en.wikipedia.org/wiki/Inter-process_communication)
on Linux includes

- Mailbox (aka Message Queue)
- Shared Memory
- Signals
- Pipes
- Writing to file
- Sockets

## Assignment 2

Mention some differences and similarities between *pipes* and *mailboxes*.
When will you use *pipes* and when will you use *mailboxes*?

### Answer assignment 2

Similarities:

- Both are ordered after FIFO

Differences:

- A pipes is a byte stream whereas a mailbox has a structure
- Pipes are one-to-one, a mailbox can be read by several

When to use pipes:

- When you are dealing with text
- When there is no structure (other than the characters is the text)
- When there is only one receiver

When to use mailboxes:

- When you need to split the messages
- When you are handling more general data types/structures
- More than one can read from the mailbox, but every message is received by one
  process

## Assignment 3

Why do we need separate functions to create areas of shared memory, instead of
just using `malloc` and passing the pointer to the area to another process?

### Answer assignment 3

- A pointer refers to an address in the virtual memory of the process
- The process do not share memory, i.e. `0x123` in process A points to a
  different physical location than `0x123` in process B
- The solution is to have a system call to allocate a common, shared memory
  where `0x123` points to the same physical location

## Assignment 4

Read about `mmap`.
Mention any differences between `mmap` and `shmget`, when can we use `mmap` to
communicate between processes?

### Answer assignment 4

In computing, [`mmap(2)`](https://en.wikipedia.org/wiki/Mmap)
is a POSIX-compliant Unix system call that allocates memory or maps files or
devices into memory.

- `mmap`s are automatically deallocated when the last process exits
- `shm` must be deallocated by a process
- `shm` can survive without any running processes until shutdown
- `mmaps` survives until the last process has called `munmap`

## Assignment 5

Find out how to list the different signals your machine recognizes.
Explain some important signals.
Are there many signals you didn't know before?

### Answer assignment 5

```text
   Name      Default Action      Description
1  SIGHUP    terminate process   terminal line hangup
2  SIGINT    terminate process   interrupt program
3  SIGQUIT   create core image   quit program
4  SIGILL    create core image   illegal instruction
5  SIGTRAP   create core image   trace trap
6  SIGABRT   create core image   abort program (formerly SIGIOT)
7  SIGEMT    create core image   emulate instruction executed
8  SIGFPE    create core image   floating-point exception
9  SIGKILL   terminate process   kill program
10 SIGBUS    create core image   bus error
11 SIGSEGV   create core image   segmentation violation
12 SIGSYS    create core image   non-existent system call invoked
13 SIGPIPE   terminate process   write on a pipe with no reader
14 SIGALRM   terminate process   real-time timer expired
15 SIGTERM   terminate process   software termination signal
16 SIGURG    discard signal      urgent condition present on socket
17 SIGSTOP   stop process        stop (cannot be caught or ignored)
18 SIGTSTP   stop process        stop signal generated from keyboard
19 SIGCONT   discard signal      continue after stop
20 SIGCHLD   discard signal      child status has changed
21 SIGTTIN   stop process        background read attempted from control terminal
22 SIGTTOU   stop process        background write attempted to control terminal
23 SIGIO     discard signal      I/O is possible on a descriptor
24 SIGXCPU   terminate process   cpu time limit exceeded
25 SIGXFSZ   terminate process   file size limit exceeded
26 SIGVTALRM terminate process   virtual time alarm
27 SIGPROF   terminate process   profiling timer alarm
28 SIGWINCH  discard signal      Window size change
29 SIGINFO   discard signal      status request from keyboard
30 SIGUSR1   terminate process   User defined signal 1
31 SIGUSR2   terminate process   User defined signal 2
```

## Assignment 6

What separates processes from threads in terms of IPC?
How can you implement "Inter thread communication"?

### Answer assignment 6

Processes must send messages or create a shared memory using the kernel in order
to communicate
Threads can use the virtual memory of the process to communicate without the use
of kernel calls

## Assignment 7

Write a program that takes input from the user and writes it back on screen.
However, before input is written back, it should be sent to a child process by
using `fork()`, which is responsible for printing.
You can decide how to send the message from one process to the other.

### Answer assignment 7

See [set_5_os_assignment_7.c](set_5_os_assignment_7.c)

## Assignment 8

Change [Assignment 7](#assignment-7) so that it uses other forms for IPC.

Can you think of any special cases you need to take into account?
Is there any potential for something to go wrong?
Is there any potential to save capacity in some cases?

### Answer assignment 8

See [set_5_os_assignment_8.c](set_5_os_assignment_8.c)
