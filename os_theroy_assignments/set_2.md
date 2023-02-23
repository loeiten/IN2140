# OS theory set 2

## Assignment 1

Describe what happens when a user makes a keystroke on a terminal associated
with a multi-user system.
Does the system respond differently for preemptive or non-preemptive systems?
Why or why not?

### Answer assignment 1

## Assignment 2

Earlier versions of Windows used an essentially non-preemptive dispatching
technique that Microsoft calls "cooperative multitasking". With this technique,
each program is expected to voluntarily give up the CPU periodically to give
other processes a chance to execute.
Discuss this method.
What potential difficulties could this method cause?

### Answer assignment 2

## Assignment 3

Why is program relocation unnecessary when virtual memory is used?

### Answer assignment 3

## Assignment 4

Imagine a simple scheduling algorithm.
The priority for a process is calculated as the ratio between the CPU time used
by the process and the actual time that has passed.
The lower the number, the higher the priority.
Priorities are re-calculated every ten seconds.

1. What kind of jobs are favored by this type of algorithm?
1. If no I/O is performed, this algorithm will reduce to a round-robin
  algorithm. Explain why.
1. Discuss this algorithm in relation to the general purposes that one wants to
  achieve with scheduling.

### Answer assignment 4

## Assignment 5

We want to calculate 1+2+3+...+3000 in parallel.
Create a program that starts three child processes such that it

- calculates the sums 1-1000, 1001-2000 and 2001-3000 respectively.
- waits for a random number of seconds (between 0 and 5).
- writes the answer to each file.

The parent process must wait until all three children are finished, then it can
read the numbers on the three files and sum them.

Update the program so that the child processes return the intermediate values in
the status field to `wait()` instead of writing to file.
**NOTE**: the return value of a process is maximum 8 bits.

### Answer assignment 5

## Assignment 6

In intruder programs, the following small snippet of code is often found in
several places:

```c
if (fork() > 0) exit(0);
```

What does this piece of code do?
Why is this used in illegal programs?

### Answer assignment 6

## Assignment 7

Explain how the function `fork()` and `execve()` works.

### Answer assignment 7

## Assignment 8

Explain what happens during a context switch.

### Answer assignment 8

## Assignment 9

Explain the consequences of a user performing

```c
while(1) fork();
```

### Answer assignment 9
