#include "../include/file_operations.h"

#include <stddef.h>
#include <stdio.h>   // for printf, FILE
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE

#define BUFFER_SIZE 4069

int copyFile(const char* src, const char* dst) {
  // Based on
  // https://stackoverflow.com/a/29079060/2786884
  char chars[BUFFER_SIZE];
  FILE* rStream;
  FILE* wStream;

  rStream = fopen(src, "r");
  if (rStream == NULL) {
    printf("Couldn't open %s", src);
    return EXIT_FAILURE;
  }
  wStream = fopen(dst, "w");
  if (wStream == NULL) {
    fclose(rStream);
    printf("Couldn't open %s", dst);
    return EXIT_FAILURE;
  }

  while (!feof(rStream)) {
    size_t bytes = fread(chars, sizeof(char), BUFFER_SIZE, rStream);
    if (bytes > 0) {
      fwrite(chars, sizeof(char), BUFFER_SIZE, wStream);
    }
  }
  fclose(rStream);
  fclose(wStream);

  return EXIT_SUCCESS;
}
