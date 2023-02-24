# OS theory set 2

## Assignment 1

Describe what happens when a user makes a keystroke on a terminal associated
with a multi-user system.
Does the system respond differently for preemptive or non-preemptive systems?
Why or why not?

### Answer assignment 1

- Preemptive system (where a process may be interrupted by a higher priority
  process)
   - The keystroke generates an "interrupt"
      - This gives more overhead, but faster response time
   - The OS handles the interrupt
   - The process continues
- Non-preemptive system
   - The keystroke is stored in a buffer
      - This gives less overhead, but may also give slower response time
   - The OS checks whether there are any new inputs from the keyboard and will
     potentially read from the buffer
- The keystroke is sent to the process of the user which in focus, but not
  necessarily the process currently running

## Assignment 2

Earlier versions of Windows used an essentially non-preemptive dispatching
technique that Microsoft calls "cooperative multitasking". With this technique,
each program is expected to voluntarily give up the CPU periodically to give
other processes a chance to execute.
Discuss this method.
What potential difficulties could this method cause?

### Answer assignment 2

- In [cooperative multitasking](https://en.wikipedia.org/wiki/Cooperative_multitasking)
  the OS never initiates a context switch from a running process to another
  process
   - Instead the process voluntarily yield control either
      - Periodically
      - When idle
      - When logically blocked
- Benefits:
   - The process can itself control how long it needs to run
   - Processes which needs shorter time can give up the CPU more often
   - Processes which needs longer time can run for longer
- Problems:
   - A poorly designed problem can consume all CPU time for itself
      - This can be done by performing expensive calculations or by busy waiting
        (i.e. the process repeatedly checks for a condition like a keyboard
        stroke)
      - This is unacceptable in a server setting
   - This gives less interactivity

## Assignment 3

Why is program relocation unnecessary when virtual memory is used?

### Answer assignment 3

FIXME: Probably a question for the memory part

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

1. It favors low numbers of `CPU_time/wall_clock_time`
   - A process which is I/O bound (i.e. having small CPU burst and long I/O
     times where the CPU is idle) is favored
   - Programs with such characteristics are typically found in system with a
     high number of transactions
   - Programs which are blocked and waiting for I/O will have the highest
     priority
1. We will get a situation where `CPU_time/wall_clock_time = CPU_time/CPU_time`
   - This number will be the same or all processes
   - Hence all processes will be prioritized equally
1. In scheduling one wants to achieve
   - Treat similar tasks in a similar way
      - Processes will be treated similarly with this algorithm
   - No process should wait forever
      - This condition can be hard to violate
         - One can thing about a situation where a long running process is
           interrupted by a new process which finishes quick and is followed by a
           process which finishes infinitesimally quicker and so on
         - At one point the `wall_clock_time` will be high enough for the long
           running program to interrupt the currently running program
   - Have short response times
      - This will have the highest possible response time as any newly launched
        process will have the highest priority
   - Maximize throughput
      - Since the `wall_clock_time` will increase, "old" jobs will eventually be
        completed
      - At the same time, it is possible to design a scheduler where "old" jobs
        are finished quicker
   - Minimize overhead
      - This may not be fulfilled to a high degree as there will be a lot of
        context switches for every newly launched process
   - Predictable access
      - This algorithm will have a high degree of predictability

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

See [os_set_2_assignment_5.c](./os_set_2_assignment_5.c)

## Assignment 6

In intruder programs, the following small snippet of code is often found in
several places:

```c
if (fork() != 0) exit(0);
```

What does this piece of code do?
Why is this used in illegal programs?

### Answer assignment 6

- Upon successful completion `fork()` returns a value of 0 to the child process
  and returns the PID of the child process to the parent process
   - Otherwise -1 is returned to the parent and no child process is created
- Hence this will kill the parent process and continues in a new process
- This is often used as it's harder to surveil a process based on PID

## Assignment 7

Explain how the function `fork()` and `execve()` works.

### Answer assignment 7

- `fork()`
   - Makes a copy of the running process
      - This is the child process
- `execve()`
   - Replaces one process with another

## Assignment 8

Explain what happens during a context switch.

### Answer assignment 8

1. Stop running process 1
1. Store the state (like registers and instruction pointer) of process 1
   - This is usually done on the stack or the
   [process control block](https://en.wikipedia.org/wiki/Process_control_block)
   (PCB)
1. Load the state of process 2 from either the stack or the PCB
1. Resume operation on program counter for process 2 (dispatch)

## Assignment 9

Explain the consequences of a user performing

```c
while(1) fork();
```

### Answer assignment 9

- This is called a fork bomb
- This will spawn a massive amount of child processes until the system have no
  more resources
- This will hinder new processes to start
