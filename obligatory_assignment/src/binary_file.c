#include "../include/binary_file.h"

#include <errno.h>     // for errno
#include <stdio.h>     // for fprintf, ferror, fclose, stderr, fread
#include <stdlib.h>    // for EXIT_FAILURE, EXIT_SUCCESS, free, malloc
#include <string.h>    // for strerror, strlen
#include <sys/stat.h>  // for stat, mkdir

#include "../include/router.h"  // for Router, addLink, MAX_NEIGHBORS

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
  if ((readBytes < nItems) || ferror(fp)) {
    fclose(fp);
    fprintf(stderr, "Failed to obtain the number of records from %s: %s\n",
            binFile, strerror(errno));
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
    if (readNewline(fp) != EXIT_SUCCESS) {
      fclose(fp);
      fprintf(stderr, "Failed to read from %s\n", binFile);
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
  success = readAndSetNeighbors(fp, *routerArray, *N, &pairNumber);
  if (success != EXIT_SUCCESS) {
    fclose(fp);
    fprintf(stderr, "Failed to get neighbor from pair %zu in %s\n", pairNumber,
            binFile);
    return EXIT_FAILURE;
  }

  fclose(fp);

  return EXIT_SUCCESS;
}

int readNewline(FILE* fp) {
  unsigned char c;
  size_t nItems = 1;
  int readBytes = fread(&c, sizeof(char), nItems, fp);
  if ((readBytes < nItems) || ferror(fp)) {
    fprintf(stderr, "Failed to read from file: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  if (c != '\n') {
    fprintf(stderr, "Expected newline, but got '%c'\n", c);
    return EXIT_FAILURE;
  }
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
  if ((readBytes < nItems) || ferror(fp)) {
    fprintf(stderr, "Failed to read routerId: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Read flag
  readBytes = fread(&(router->flag), nBytes, nItems, fp);
  if ((readBytes < nItems) || ferror(fp)) {
    fprintf(stderr, "Failed to read flag: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Read the length of the char
  unsigned char charLen;
  readBytes = fread(&charLen, nBytes, nItems, fp);
  if ((readBytes < nItems) || ferror(fp)) {
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
  if ((readBytes < nItems) || ferror(fp)) {
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

int readAndSetNeighbors(FILE* fp, struct Router* const routerArray,
                        const unsigned int N, size_t* pairNumber) {
  size_t nItems = 1;
  size_t nBytes = 1;  // Given in the assignment

  unsigned char fromRouter;
  unsigned char toRouter;

  while ((!feof(fp)) && (!ferror(fp))) {
    // Read the newline
    if (readNewline(fp) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    // Read the from unsigned char
    int readBytes = fread(&fromRouter, nBytes, nItems, fp);

    // We first get a end of stream *after* a failed read
    if (feof(fp)) {
      return EXIT_SUCCESS;
    }

    // Increment the pairNumber
    ++(*pairNumber);

    if ((readBytes < nItems) || ferror(fp)) {
      fprintf(stderr, "Failed to read the first neighbor: %s\n",
              strerror(errno));
      return EXIT_FAILURE;
    }

    // Read the to unsigned char
    readBytes = fread(&toRouter, nBytes, nItems, fp);
    if ((readBytes < nItems) || ferror(fp)) {
      fprintf(stderr, "Failed to read the second neighbor: %s\n",
              strerror(errno));
      return EXIT_FAILURE;
    }

    int success = addLink(routerArray, N, fromRouter, toRouter);
    if (success != EXIT_SUCCESS) {
      fprintf(stderr, "Failed to store the neighbor in the array\n");
      return EXIT_FAILURE;
    }
  }

  fprintf(stderr,
          "Reading neighbors finished before reaching a terminating \\n\n");
  return EXIT_FAILURE;
}

int writeBinaryFile(const char* const binFile,
                    const struct Router* const routerArray,
                    unsigned int const N) {
  // Get directories
  const char* directories = NULL;
  int success = getDirectories(binFile, &directories);
  if (success == EXIT_FAILURE) {
    fprintf(stderr, "Failed to obtain directories\n");
    if (directories != NULL) {
      free((void*)directories);
      directories = NULL;
      return EXIT_FAILURE;
    }
  }

  // Create the directories if it doesn't exist
  success = makeDirectories(directories);
  if (success != EXIT_SUCCESS) {
    free((void*)directories);
    directories = NULL;
    return EXIT_FAILURE;
  }

  // Free the directories
  free((void*)directories);
  directories = NULL;

  // Open the file
  FILE* fp;
  fp = fopen(binFile, "wb");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", binFile, strerror(errno));
    return EXIT_FAILURE;
  }

  // Write the number of records
  size_t nBytes = 4;  // Given in the assignment
  size_t nItems = 1;
  int writtenBytes = fread((void*)&N, nBytes, nItems, fp);
  if ((writtenBytes < nItems) || ferror(fp)) {
    fclose(fp);
    fprintf(stderr, "Failed write the number of records to %s: %s\n", binFile,
            strerror(errno));
    return EXIT_FAILURE;
  }

  // Write the router information
  for (size_t i = 0; i < N; ++i) {
    // Write the newline
    if (writeNewline(fp) != EXIT_SUCCESS) {
      fclose(fp);
      fprintf(stderr, "Failed to write to %s\n", binFile);
      return EXIT_FAILURE;
    }

    success = writeRouter(fp, routerArray[i]);
    if (success != EXIT_SUCCESS) {
      fclose(fp);
      fprintf(stderr, "Failed to write router %zu\n", i);
      return EXIT_FAILURE;
    }
  }

  // Write the neighbors
  size_t pairNumber = 0;
  success = writeNeighbors(fp, routerArray, N, &pairNumber);
  if (success != EXIT_SUCCESS) {
    fclose(fp);
    fprintf(stderr, "Failed to write neighbor from pair %zu in %s\n",
            pairNumber, binFile);
    return EXIT_FAILURE;
  }

  fclose(fp);

  return EXIT_SUCCESS;
}

int getDirectories(const char* const binFile, const char** directories) {
  return EXIT_SUCCESS;
}

int makeDirectories(const char* const directories) {
  struct stat st = {0};
  if (stat(directories, &st) == -1) {
    int success = mkdir(directories, 0700);
    if (success != 0) {
      fprintf(stderr, "Cannot make the directories %s: %s\n", directories,
              strerror(errno));
    }
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int writeNewline(FILE* fp) {
  unsigned char c = '\n';
  size_t nItems = 1;
  int writtenBytes = fwrite(&c, sizeof(char), nItems, fp);
  if ((writtenBytes < nItems) || ferror(fp)) {
    fprintf(stderr, "Failed write newline: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int writeRouter(FILE* fp, struct Router router) {
  // Initialize helper variables
  size_t nBytes = 1;  // Given in the assignment
  size_t nItems = 1;

  // Write router id
  int writtenBytes = fwrite(&(router.routerId), nBytes, nItems, fp);
  if ((writtenBytes < nItems) || ferror(fp)) {
    fprintf(stderr, "Failed to write routerId: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Read flag
  writtenBytes = fwrite(&(router.flag), nBytes, nItems, fp);
  if ((writtenBytes < nItems) || ferror(fp)) {
    fprintf(stderr, "Failed to write flag: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Read the producerModel
  unsigned char charLen = strlen(router.producerModel);
  writtenBytes = fwrite(router.producerModel, charLen, nItems, fp);
  if ((writtenBytes < nItems) || ferror(fp)) {
    fprintf(stderr, "Failed to write producerModel: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int writeNeighbors(FILE* fp, const struct Router* const routerArray,
                   const unsigned int N, size_t* pairNumber) {
  size_t nItems = 1;
  size_t nBytes = 1;  // Given in the assignment

  unsigned char fromRouter;
  unsigned char toRouter;

  // Loop over routers
  for (int r = 0; r < N; ++r) {
    fromRouter = routerArray[r].routerId;
    // Loop over neighbors
    for (int i = 0; i < MAX_NEIGHBORS; ++i) {
      if (routerArray[r].neighbors[i] == -1) {
        break;
      }
      toRouter = routerArray[r].neighbors[i];
      // Write the newline
      if (writeNewline(fp) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
      }

      // Write the first neighbor
      int writtenBytes = fwrite(&fromRouter, nBytes, nItems, fp);
      if ((writtenBytes < nItems) || ferror(fp)) {
        fprintf(stderr, "Failed to write the first neighbor: %s\n",
                strerror(errno));
        return EXIT_FAILURE;
      }

      // Read the to unsigned char
      writtenBytes = fwrite(&toRouter, nBytes, nItems, fp);
      if ((writtenBytes < nItems) || ferror(fp)) {
        fprintf(stderr, "Failed to read the second neighbor: %s\n",
                strerror(errno));
        return EXIT_FAILURE;
      }

      // Increment the pairNumber
      ++(*pairNumber);
    }
  }

  return EXIT_SUCCESS;
}
