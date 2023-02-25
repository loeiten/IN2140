/*
 * Assignment:
 *
 * Write a program which takes `name`, `age` and `filename` as command line
 * arguments.
 * Make a cstring with the text
 *
 *     "Hello NAME, you are AGE years old."
 *
 * and store this in the file `filename`
 */

#include <libgen.h>  // for basename
#include <stdio.h>   // for printf, fclose, fopen, NULL, ferror, fputs, fread
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS, atoi, malloc

#define BUFFER_SIZE 1024

int writeFile(const char* name, const int age, const char* path) {
  // Open the file for writing
  FILE* fpw;
  fpw = fopen(path, "w");
  if (fpw == NULL) {
    printf("%s not found", path);
    return EXIT_FAILURE;
  }

  // Print to file
  // We concatenate the string in order to write everything at once
  // We do this as we have to convert the age to a string anyways, see
  // https://stackoverflow.com/a/32819876/2786884
  const char* format = "Hello %s, you are %d years old.\n";
  int length = snprintf(NULL, 0, format, name, age);
  char* str = (char*)malloc((length + 1) * sizeof(char));
  snprintf(str, length + 1, format, name, age);
  int success = fputs(str, fpw);
  free(str);
  if (success == EOF) {
    fclose(fpw);
    return EXIT_FAILURE;
  }

  // Close the file
  fclose(fpw);
  return EXIT_SUCCESS;
}

int readAndPrintFile(const char* path) {
  // Based on:
  // https://stackoverflow.com/a/3463793/2786884
  char buffer[BUFFER_SIZE];
  FILE* fpr = fopen(path, "r");
  if (fpr == NULL) {
    return EXIT_FAILURE;
  }
  int nRead = fread(buffer, sizeof(char), BUFFER_SIZE, fpr);
  while (nRead > 0) {
    fwrite(buffer, sizeof(char), nRead, stdout);
    nRead = fread(buffer, sizeof(char), BUFFER_SIZE, fpr);
  }
  if (ferror(fpr)) {
    fclose(fpr);
    return EXIT_FAILURE;
  }
  fclose(fpr);
  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
  if (argc != 4) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    printf("Usage: ./%s STR INT PATH\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Capture the name, age and path
  const char* name = argv[1];
  const int age = atoi(argv[2]);
  const char* path = argv[3];

  int exitFailure = writeFile(name, age, path);
  if (exitFailure) {
    printf("Failed writing to %s", path);
    return EXIT_FAILURE;
  }

  exitFailure = readAndPrintFile(path);
  if (exitFailure) {
    printf("Failed reading from %s", path);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
