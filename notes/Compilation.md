# Compilation

When compiling to an executable, a whole toolchain is involved:

## Preprocessing

Macros and includes are resolved.
It basically copies text from the `#include` or `#define` and paste them into
the appropriate place.

One could even have a file `endBrace.h` with the content `}`.
The preprocessor is then going to replace every instance of
`#include "<pathTo>/endBrace.h"` to `}`

The output of this step is a new `c` file with everything pasted from the
macros and includes.
The output is referred to as the **translation unit**.

To see the output of this stage, use

```sh
clang -E myFile.c
```

## Compilation to assembly

The preprocessed `c` file is translated to an assembly file (`.S`), which
includes the assembly code.
In order to execute the assembly it has to be translated to a binary
representation.

To see the output of this stage, use

```sh
clang -S myFile.c
```

## Assembler

The assembler takes the `.S` and produces an object (`.o`) file

## Linking

The linker takes all the object files and libraries and resolves references to
symbols defined in other translation units

## Ways to produce the executable

To produce the executable in one go:

```sh
clang main.c file1.c file2.c
```

Produce the object files and link them:

```sh
clang -c main.c
clang -c file1.c
clang -c file2.c
clang main.o file1.o file2.o
```

The latter is more modular, and can make compilation faster

## Sources

[Compiling multiple files (no linking)](https://www.youtube.com/watch?v=2YfM-HxQd_8)
[Inspect the output of compiling, assembling and linking](https://www.youtube.com/watch?v=N2y6csonII4)
[More details about compilation](https://www.youtube.com/watch?v=3tIqpEmWMLI)
[More details about linking](https://www.youtube.com/watch?v=H4s55GgAg0I)
