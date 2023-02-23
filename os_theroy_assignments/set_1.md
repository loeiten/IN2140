# OS theory set 1

## Assignment 1

Why do we have OSes?

Why do we need to understand them?

### Answer assignment 1

#### Why do we have OSes

- Can enable portability of the same application program on different hardware
   - Presents a "virtual machine" which is easier to use
- Manages resources
   - CPU time
   - Memory usage
- Manages devices
- Manages concurrency
- Usually adds a layer of security for the use of the underlying hardware

#### Why do we need to understand them

- Important in order to
   - Do things correctly
   - Do things efficient (division of labor between HW and SW)
   - Meet technical challenges

## Assignment 2

What are the main tasks of the BIOS and bootstrap?

Describe in simple terms what happens from the moment we turn on the computer
until the operating system is ready for use.

### Answer assignment 2

### What are the main tasks to the BIOS and the bootstrapping

- Basic Input/Output System (BIOS)
   - Store instructions for basic HW initialization and management
   - Initiates the bootstrap
- Bootstrap
   - Read the `boot` program from the `master boot record` on secondary storage
   - Run the `boot` program
         - Read root file system and locate file with OS kernel
         - Load kernel into memory
         - Run the kernel

### What happens from the moment we turn on the computer until the OS is ready

1. Gather HW information and setup the system
1. Load data from the boot sector
1. Execute boot program on a CPU
1. Load OS from disk
1. Run OS

## Assignment 3

What is user and kernel level? Why can't everything run at the kernel level?

### Answer assignment 3

#### What is the user level and the kernel level

- User level:
   - Protected mode
   - Not allowed to access HW or device drivers directly (only through APIs)
   - Access to assigned memory only
   - Limited instruction set
- Kernel level:
   - Real mode
   - Access to entire memory
   - All instructions can be executed
   - Bypasses security

#### Why cannot everything run at the kernel level

One could in principle make such an OS, however this would be a big security
risk, and the potential for memory mismanagement would be great

## Assignment 4

What happens when we call a system function (e.g. `read`) from the user level?

### Answer assignment 4

1. Parameters are pushed on the stack
1. The library code is called
1. The system call number is put in the register
1. The kernel is called through a TRAP
   - The kernel examines the system call number
   - The kernel finds the requested system call handler
   - The kernel executes the requested operation
1. Return to the library and clean up
   - Increase the instructor pointer
   - Remove parameters from the stack
1. Resume process

## Assignment 5

What are the advantages and disadvantages of monolithic cores vs micro-cores?
Find out how the most popular operating systems (Windows, Linux, MacOS) are
built in terms of kernel type.

### Answer assignment 5

#### Name the pros and cons for monolithic vs micro kernels

- Monolithic kernel
   - Pro
      - Runs fast (no boundaries to cross)
   - Cons
      - Complex
      - Easy to break
- Micro kernel
   - Pro
      - Kernels can be small
      - Easier to update
   - Cons
      - Lots of message passing

#### Find out what kernels types are used for Windows, Linux and MacOs

- Windows 11: Hybrid (Windows NT kernel [source](https://en.wikipedia.org/wiki/Windows_11))
- Ubuntu 22.10 : Monolithic (Linux kernel [source](https://en.wikipedia.org/wiki/Ubuntu))
- macOS Ventura: Hybrid (XNU kernel [source](https://en.wikipedia.org/wiki/MacOS_Ventura))

## Assignment 6

Interrupts are used for many different things, such as reading from the hard
drive, killing processes and communicating over networks.

Explain the following:

- What is an interrupt?
- What happens when an interrupt is created?

### Answer assignment 6

#### What is an "interrupt"

- Interrupt
   - An electronic signal that (usually) transfers the control to an interrupt
      handling routine
   - Caused by asynchronous events
      - Finished disk operations
      - Incoming network packets
      - Expired timers
- Exception
   - Cause by synchronous events (i.e. processor reach a predefined condition
      when running an instruction)
   - Trap - the exception handler can handle the exception
      - Overflow
      - System call
   - Fault - handler cannot correct
      - Division by 0
      - Wrong data format
- Abort - unrecoverable error
      - Hardware failure

#### What happens when an "interrupt" is created

1. Processes is running while the interrupt occur
1. Capture the state
1. Switch control and find the interrupt handler through the interrupt
    descriptor table (IDT)
1. Execute the interrupt handler
1. Restore the interrupted process
1. Continue execution
