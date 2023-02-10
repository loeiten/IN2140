#include "../include/string_helpers.h"

#include <ctype.h>   // for isdigit
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strlen

int getInt(const char* str, int* number) {
  int len;
  int i;

  len = strlen(str);
  i = 0;
  *number = 0;

  while ((i < len) && (isdigit(str[i]))) {
    *number *= 10;
    *number += str[i] - '0';
    ++i;
  }

  if (i != len) {
    *number = -1;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
