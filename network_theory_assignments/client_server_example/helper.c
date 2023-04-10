#include "include/helper.h"

#include <stdio.h>   // for printf, fclose, fopen, getline, NULL, FILE, size_t
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS

int readLine(const char *path, char **line, int lineNo) {
  FILE *fp = fopen(path, "r");
  if (fp == NULL) {
    printf(
        "Failed to open %s. Please ensure that %s exists and contains the ip "
        "in the first line, and the port number in the second line.\n",
        path, path);
    return EXIT_FAILURE;
  }

  line = NULL;
  size_t len = 0;

  // NOTE: getline is from POSIX.1-2008, not the C-standard, see
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getline.html
  // for specification, and
  // https://github.com/NetBSD/src/blob/trunk/tools/compat/getline.c
  // for possible implementation
  // Read the header
  for (int i = 0; i <= lineNo; ++i) {
    // ssize_t is when the output can be negative
    ssize_t nBytes = getline(line, &len, fp);
    if (nBytes == -1) {
      printf("Failed to read line %d of %s\n", i, path);
      return EXIT_FAILURE;
    }
  }

  fclose(fp);
  return EXIT_SUCCESS;
}
