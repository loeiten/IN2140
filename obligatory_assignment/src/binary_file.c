#include "../include/binary_file.h"  // for Router

#include <errno.h>   // for errno
#include <libgen.h>  // for basename
#include <math.h>    // for errno
#include <stddef.h>
#include <stdio.h>   // for fclose, fprintf, ferror, fread, stderr, size_t
#include <stdlib.h>  // for free, EXIT_FAILURE, malloc, EXIT_SUCCESS
#include <string.h>  // for strerror

#include "../include/router.h"  // for Router

int readBinaryFile(const char* const binFile, struct Router** routerArray,
                   unsigned int* const N) {
  // Open the file
  FILE* fp;
  fp = fopen(binFile, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", binFile, strerror(errno));
    return EXIT_FAILURE;
  }

  // Initialize success
  int success;

  // Read the number of records
  size_t nBytes = 4;  // Given in the assignment
  size_t nItems = 1;
  int readBytes = fread(N, nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fclose(fp);
    fprintf(stderr, "Failed to read from %s: %s\n", binFile, strerror(errno));
    return EXIT_FAILURE;
  }

  // Malloc the routerArray
  *routerArray = (struct Router*)malloc(*N * sizeof(struct Router));
  if (*routerArray == NULL) {
    fclose(fp);
    perror("Could not allocate memory to routerArray: ");
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < *N; ++i) {
    // Read the newline
    unsigned char c;
    readBytes = fread(&c, sizeof(char), nItems, fp);
    if ((readBytes < 1) || ferror(fp)) {
      fclose(fp);
      fprintf(stderr, "Failed to read from %s: %s\n", binFile, strerror(errno));
      return EXIT_FAILURE;
    }
    if (c != '\n') {
      fclose(fp);
      fprintf(stderr, "Expected newline, but got '%c'\n", c);
      return EXIT_FAILURE;
    }

    success = readRouter(fp, &((*routerArray)[i]));
    if (success != EXIT_SUCCESS) {
      fclose(fp);
      fprintf(stderr, "Failed to read router %zu\n", i);
      return EXIT_FAILURE;
    }
  }

  // Read the neighbors
  size_t pairNumber = 0;
  success = readNeighbors(fp, routerArray, *N, &pairNumber);
  if (success != EXIT_SUCCESS) {
    fclose(fp);
    fprintf(stderr, "Failed to get neighbor from pair %zu\n", pairNumber);
    return EXIT_FAILURE;
  }

  fclose(fp);

  return EXIT_SUCCESS;
}

int readRouter(FILE* fp, struct Router* router) {
  // Initialize memory to be allocated
  char* producerModel = NULL;

  // Initialize helper variables
  size_t nBytes = 1;  // Given in the assignment
  size_t nItems = 1;

  // Initialize the neighbors to be -1
  for (size_t i = 0; i < MAX_NEIGHBORS; ++i) {
    router->neighbors[i] = -1;
  }

  // Read router id
  int readBytes = fread((void*)&(router->routerId), nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fprintf(stderr, "Failed to read routerId: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Read flag
  readBytes = fread(&(router->flag), nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fprintf(stderr, "Failed to read flag: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Read the length of the char
  unsigned char charLen;
  readBytes = fread(&charLen, nBytes, nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    fprintf(stderr, "Failed to read charLen: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Read the producerModel
  producerModel = (char*)malloc(charLen * sizeof(char));
  if (producerModel == NULL) {
    perror("Could not allocate memory to producerModel: ");
    return EXIT_FAILURE;
  }
  readBytes = fread(producerModel, (charLen - 1), nItems, fp);
  if ((readBytes < 1) || ferror(fp)) {
    // Free the producerModel here we do not indicate whether or not is has been
    // allocated outside this function
    free(producerModel);
    producerModel = NULL;
    fprintf(stderr, "Failed to read producerModel: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Null terminate
  producerModel[charLen - 1] = '\0';

  // Assign
  router->producerModel = producerModel;
  return EXIT_SUCCESS;
}

int readNeighbors(FILE* fp, struct Router* const* routerArray,
                  unsigned int const N, size_t* pairNumber) {
  unsigned char c;
  size_t nItems = 1;
  size_t nBytes = 1;  // Given in the assignment
  size_t readBytes = nItems;

  unsigned char fromRouter;
  unsigned char toRouter;

  while (readBytes >= nItems) {
    // Increment the pairNumber
    ++pairNumber;

    // Read the newline
    fread(&c, sizeof(char), nItems, fp);
    if (ferror(fp)) {
      fprintf(stderr, "Failed to read from file: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
    if (c != '\n') {
      fprintf(stderr, "Expected newline, but got '%c'\n", c);
      return EXIT_FAILURE;
    }

    // Read the from unsigned char
    fread(&fromRouter, nBytes, nItems, fp);
    if (ferror(fp)) {
      fprintf(stderr, "Failed to read the from router: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    // FIXME: Need a better way of finding EOF, readd readBytes above
    // Read the to unsigned char
    readBytes = fread(&toRouter, nBytes, nItems, fp);
    if (ferror(fp)) {
      fprintf(stderr, "Failed to read the to router: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    // FIXME: Implement setNeighbor
  }
}
