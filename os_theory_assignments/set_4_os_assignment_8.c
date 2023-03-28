#include <stdio.h>      // for printf, NULL, snprintf
#include <stdlib.h>     // for free, malloc, EXIT_SUCCESS
#include <string.h>     // for strlen
#include <sys/fcntl.h>  // for open, O_CREAT, O_RDWR, O_TRUNC
#include <sys/stat.h>   // for S_IRWXU, S_IRWXG, S_IRWXO
#include <unistd.h>     // for close, fsync, lseek, read, write

int main() {
  const char* buf = "Hello, world!\n";
  const char* filename = "set_4_os_assignment_8.txt";

  // O_RDWR - open for read and write
  // O_CREAT - create the file
  // O_TRUNC - truncate the length to 0 if the file exists
  int fd =
      open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
  write(fd, (void*)buf, strlen(buf));
  fsync(fd);  // Ensure that the content is written (similar to fflush)

  // Allocate the string we will read
  const char* format = "%s";
  int length = snprintf(NULL, 0, format, buf);
  char* str = (char*)malloc((length + 1) * sizeof(char));

  // Reset the position (could also close and reopen)
  lseek(fd, 0, SEEK_SET);

  // Read and print
  read(fd, (void*)str, length);
  // Terminate the read data
  str[length] = '\0';

  printf("%s", str);

  // Clean up
  free(str);
  close(fd);

  return EXIT_SUCCESS;
}
