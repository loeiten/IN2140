# Obligatory assignment

## The assignment

Let's imagine a scenario where a company wants a "better" system for managing
the routers they have in operation at two different buildings.
The department responsible for the network has a program that will be used to
store and process basic information about the routers.
The program should accept two files.
One file that describes the routers and links between them, and another file
with commands to execute.
We will see what these files look like in [File structures](#file-structures).

The program must be compiled with `Make`, and has to be started with two file
names as arguments.
The first file name must be the name of a file that contains data about routers
that can be read and processed by the program.
The second file contains one or more lines of commands to be executed.
For example, you can start in this way:

```bash
./router operation 50_routers_150_edges commands_50_routers.txt
```

You can assume that the program is executed with the correct number of arguments and valid files.

Together with this assignment we have the files given in [`assignment_files`](./assignment_files/).

| FILE                            | DESCRIPTION                                                      |
|---------------------------------|------------------------------------------------------------------|
| `5_routers_fully_connected`     | an information file for 5 routers                                |
| `5_routers_fully_connected.png` | a figure illustrating the topology in `5_routers_fully_connected`|
| `10_routers_10_edges`           | an information file for 10 routers                               |
| `10_routers_10_edges.png`       | a figure illustrating the topology in `10_routers_10_edges`      |
| `50_routers_150_edges`          | an information file for 50 routers                               |
| `50_routers_150_edges.png`      | a figure illustrating the topology in `50_routers_150_edges`     |
| `commands_10_routers.txt`       | a command file adapted to `10_routers_10_edges`                  |
| `commands_50_routers.txt`       | a command file adapted to `50_routers_150_edges`                 |

## File structures

In this section we specify what the two files that the program will read look
like.

### Information about routers

This file contains all the data that the program needs.
The file does not (exclusively) contain text data and thus cannot be inspected
in standard text editors such as Atom/Notepad etc.
This is intended to make it easier to write code for reading in numerical data,
as you do not have to think about translate between numbers in text form and
actual numbers in memory.

The line first in the file is an `int`, a 4-byte number, which tells us how many
routers the file has information about.
You should call this number `N`.
Information about each router is delimited by line breaks, i.e. each router is a
"line" of information.
A valid file without router data therefore contains a line with only the number
`0`.
We will only work with files that have router data.
It is important to note that this first line is a 4-byte `int` value, and NOT a
number in readable form!
The rest of the lines are maximum of 256 bytes long (this restriction must be
maintained when writing new data to the file).
See the end of the document for hints on handling binary data in files.

The next `N` lines contains representation of each routers on the following form

- Router-ID (unique) - `unsigned char`
- FLAG - `unsigned char`
- Length of producer/model string - `unsigned char`
- Producer/model - `char[]` (max length 253)

> Note: note that the fields Router-ID, Flag and Length can potentially have the
> value `\n`, which will terminate a call to `fgets()` if this is used to read
> the line from the file.

Each line in the file contains a total of 3 bytes of information about the
router, followed by the manufacturer/model.
Each line must be stored in a `struct`, which naturally must have the fields
`id`, `flag` and `model` (you can, of course, call them something else).
You should expand the `struct` as needed to solve the task.

In each `struct` there must be an `unsigned char`, which in the list above is
called FLAG, and it must represent various properties a router can have.

| Bit position | Property            | Explanation                                          |
|--------------|---------------------|------------------------------------------------------|
| 0            | Active              | If the router is active                              |
| 1            | Wireless            | If the router is wireless                            |
| 2            | 5 GHz               | Whether the router supports 5 GHz                    |
| 3            | Unused              | Not in use                                           |
| 4-7          | Modification number | Serial number which increases by one for each change |

After the `N` lines of information about the routers follow an unknown number of
lines with connections between the routers.
Each line contains two router IDs, and the lines are separated by line breaks.
The IDs are stored in binary format, and are of the `unsigned char` type.
A link between router R1 and R2 means that R1 must have a pointer to R2.
These links are one-way.
So it does not follow that `R2 -> R1` when `R1 -> R2`.
Note that `R2 -> R1` can also occur in the file so that you get a two-way
connection.

You can assume that each node has a maximum of 10 connections, and memory can be
allocated to 10 pointers for each router regardless of the actual number.
You can have an array of pointers in the `struct` with information about a
router.

All these links define a directed graph that you must traverse with a graph
search.

### Commands

This file contains a number of commands.
Each line contains a command.
The different commands have different formats.
Here follows a description of the commands:

- `print router_id`: Prints to `stdout` information about the router and the IDs
  of its neighbors, specifically only those neighbors that the router points to.
- `set_flag router_id flag value`: Sets the `flag` of a `router_id` to the
  `value` in the router with if router ID.
  The `flag` can take one of the values `{0, 1, 2, 4}`.
  The `values` can be in the range `[0, 15]`.
   - `flag == 0` sets the active bit to `value`.
   - `flag == 1` sets the wireless bit to `value`.
   - `flag == 2` sets 5GHz to `value`.
   - `flag == 3` is invalid, so please print an error message.
   - `flag == 4` sets the modification number to `value`.
   - For `flags` `0` through `2`, the `value` can only be `0` or `1`.
- `set_model router_id name`: Sets the `name` of the router with `router-id`.
- `add_link router_id router_id`: Add a link between two routers.
- `delete_router router_id`: Delete the router information.
  Note that all pointers to this router are also deleted.
- `exists_route router_id router_id`: Checks whether there is a route between
  two routers.
  You should do a graph search from the first router and see if you can get to
  the second router.
  One possibility, which is surprisingly simple with recursion, is to do a
  depth-first search.
  For simplicity, you do not need to write down the route you find, but feel
  free to do so if you have time and are absolutely sure that everything else is
  working as it should.

EVERYTHING in this file is readable with a text editor.
`router`, `flag` and `value` are all numbers in a readable form.
They must therefore be converted to the right data types using for example
`strtol`.

## Specifications

Here is some more specific information about what a user should be able to use
the program for, and how the program should work.
Roughly speaking, the program will be divided into three:

1. Read in the file and create the data structures you need.
1. Execute commands given in a command file.
1. Termination and write to file.

### Input and data-structures

This must happen as soon as the program starts.
The first argument of the command is the file which needs to be read and its
data must be stored in memory.
For each line in the file, space must be allocated to a `struct` (with
`malloc`), and the `struct` must be filled with data from the line.
A pointer to the `struct` is to be stored in a global, dynamically allocated
array.
This memory must be allocated with space for `N` (number of routers) routers.
When all the lines have been read in, given their own struct and their own entry
in the global array, the program proceeds to the command file.

### Termination

When the program has executed all the commands in the command file, all
allocated memory areas (allocated with `malloc`) must be released by calling
`free`, data must be written to the same file as it was read from (overwriting
the entire file), and the file must be closed.
It may be worthwhile to write to another file while you are developing the
program.

### Importance of the different functionality

Here is a list of the importance of the different parts of the assignment.
Use the list as a guide for what you should work on (and in what order).
This is with a view to what will be important for the home exam and what is
central to the assignment.
This will be used for correction.

1. Creating the array structure
1. Importing data from file
1. Correct insertion of data into the array
1. Good use of memory (`malloc` and `free`)
1. Print
1. Adding links
1. Deleting routers
1. Writing data to a file
1. Changing of name
1. Changing of the FLAG-char in the router struct
1. Check if there is a path between two routers

In other words:
Make sure that loading and creating the data structure works first, then make
sure that you write to the file correctly.
Watch out for memory usage throughout.
Only when this works should you start looking at command handling.

### Comments

We require you to comment on the following in your program:

- Call to `malloc`: Approximately how much memory is allocated, in bytes.
- Call to `free`: Where the memory was allocated.
- Function parameters: Types and briefly what they are.
- Function flow: Briefly what the function does unless it is obvious.

## Relevant man pages

These man pages contain information about functions that may be relevant for
solving this task.
Note that several of the man pages contain information about several functions
on one page, such as `malloc`/`calloc`/`realloc`.

- `malloc/calloc/realloc`
- `fgetc`
- `fread/fwrite`
- `fopen/fclose`
- `scanf/fscaf`
- `strcpy`
- `memcpy`
- `memmove`
- `atoi`
- `strtol`
- `isspace/isdigit/alnum` etc.
- `strdup`
- `strlen`
- `strtok`
- `printf/fprintf`

## Other hints

As the router data cannot be read as plain text, it is a good idea to have
another way of inspecting the file(s).
For this, there is, among other things, the terminal tool `xxd`.
This displays the file as hexadecimal numbers next to a textual representation.
You can then, for example, easily see that the first four bytes contain the
numerical value that says how many routers there are in the file.

Run `valgrind` (or use sanitizers or `leaks` on MacOs)!
