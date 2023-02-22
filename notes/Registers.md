# Registers

The registers depend on the architecture.
The `x86` instruction set architecture (named from the 8086 and 8088
microprocessors) is one such architecture.

## x86 names

Traditionally the 16-bit architecture had `L` and `H` for the low and high bits.
I.e. `AX` could be decomposed to `AL` and `AH`.
The 32-bit architecture added `E` for "extended", and the 64-bit architecture
added `R` for "register".

## Intended purposes

X registers:

- AX: Accumulator
- CX: Counter (for use with loops and strings)
- DX: Extend the precision of the accumulator (e.g. combine 32-bit EAX and EDX
  for 64-bit integer operations in 32-bit code)
- BX: Base index (for use with arrays)

Pointer registers:

- SP: Stack pointer for top address of the stack.
- BP: Stack base pointer for holding the address of the current stack frame.
- IP: Instruction pointer. Holds the program counter, the address of next
  instruction.

String registers:

- SI: Source index for string operations.
- DI: Destination index for string operations.

Segment registers:

- CS: Code
- DS: Data
- SS: Stack
- ES: Extra data
- FS: Extra data #2
- GS: Extra data #3

## C to 32-bit assembly

One could inspect the assembly code generated in [Godbolt](https://godbolt.org)
by selecting `x86-64 gcc 12.2` and add the `-m32` flag to get the 32-bit
assembly (IA32).

Further, one should uncheck the `Intel asm syntax` to get syntax more in line
with what shown in the course.

## Sources

[Register purpose](https://en.wikipedia.org/wiki/X86#Purpose)
[8086 basic register architecture](https://wahyu-ehs.medium.com/assembly-8086-basic-program-execution-registers-6ae244b61a33)
[x86 assembly guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)
[NASM registers](https://www.tutorialspoint.com/assembly_programming/assembly_registers.htm)
