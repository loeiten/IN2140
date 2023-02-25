# OS theory set 5

## Assignment 1

What types of IPC (locally on a machine) do we know in Unix/Linux?

### Answer assignment 1

## Assignment 2

Mention some differences and similarities between *pipes* and *mailboxes*.
When will you use *pipes* and when will you use *mailboxes*?

### Answer assignment 2

## Assignment 3

Why do we need separate functions to create areas of shared memory, instead of
just using `malloc` and passing the pointer to the area to another process?

### Answer assignment 3

## Assignment 4

Read about `mmap`.
Mention any differences between `mmap` and `shmget`, when can we use `mmap` to
communicate between processes?

### Answer assignment 4

## Assignment 5

Find out how to list the different signals your machine recognizes.
Explain some important signals.
Are there many signals you didn't know before?

### Answer assignment 5

## Assignment 6

What separates processes from threads in terms of IPC?
How can you implement "Inter thread communication"?

### Answer assignment 6

## Assignment 7

Write a program that takes input from the user and writes it back on screen.
However, before input is written back, it should be sent to a child process by
using `fork()`, which is responsible for printing.
You can decide how to send the message from one process to the other.

### Answer assignment 7

## Assignment 8

Change [Assignment 7](#assignment-7) so that it uses other forms for IPC.

Can you think of any special cases you need to take into account?
Is there any potential for something to go wrong?
Is there any potential to save capacity in some cases?

### Answer assignment 8
