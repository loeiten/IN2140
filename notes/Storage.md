# Storage

## Concepts

### Disk

A disk is a rotating storage medium that stores permanently, even when the power
is switched off.

A disk system consists of:

- Plates: The physical plates where the data is stored.
  A modern hard drive can have a dozen plates that can be read and written on
  both sides.
- Track: Unlike old vinyl plates (which also stored information), the track does
  not go into a spiral inward but is divided into many circles inward.
  There are many thousands of tracks on a hard drive.
- Disk head(s): A sensor/"needle" that moves over the hard drive to read or
  write information.
  The disk head is located at the very end of a mechanical arm.
- Sectors: The tracks are divided into sectors separated with unreadable,
  unmagnetic areas.
  The sector size is fixed for each hard drive.
- Cylinders: If you have multiple plates, the same groove on all plates will
  form a cylinder.

### Access time

Access time is the time it takes from when we ask to read something from disk
until we have had it read.
This time is roughly composed of
queuing time + rotation delay + reading time + transfer time.

### I/O

Means Input/Output

### Disk scheduling

Disk scheduling is about how we manage the mechanical parts of a disk to provide
the best possible service to those who need something from the disk.
There are many algorithms to choose from and it is not always easy to choose.

### Caching

Caching means storing a copy of data and moving it to a place where it is easier
to use.
Data from disk can be cached in memory and data from memory can be cached on the
CPU's internal cache.
Caching is an important means of increasing the efficiency of a system.
