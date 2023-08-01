#include "../include/file_operations.h"

#include <stddef.h>
#include <stdio.h>   // for printf, FILE
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_FAILURE

#define BUFFER_SIZE (65536)  // 2^16

int copyFile(const char* src, const char* dst) {
  // Based on
  // https://stackoverflow.com/a/29079060/2786884
  char chars[BUFFER_SIZE];
  FILE* rStream;
  FILE* wStream;

  rStream = fopen(src, "r");
  if (rStream == NULL) {
    printf("Couldn't open %s\n", src);
    return EXIT_FAILURE;
  }
  wStream = fopen(dst, "w");
  if (wStream == NULL) {
    fclose(rStream);
    printf("Couldn't open %s\n", dst);
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

int getNumberOfLines(const char* path, int* lines) {
  // Based on https://stackoverflow.com/a/70708991/2786884
  // NOTE: Could instead use getline
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    printf("Failed to open %s\n", path);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE];
  *lines = 0;

  int nRead = 1;  // Set to 1 to trigger the while loop
  while (nRead > 0) {
    nRead = fread(buffer, sizeof(char), BUFFER_SIZE, fp);
    for (int i = 0; i < nRead; ++i) {
      if (buffer[i] == '\n') {
        ++(*lines);
      }
    }
  }
  if (ferror(fp)) {
    fclose(fp);
    return EXIT_FAILURE;
  }

  fclose(fp);
  return EXIT_SUCCESS;
}
