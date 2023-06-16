#include <stddef.h>  // for NULL, size_t
#include <stdio.h>
#include <stdlib.h>  // for free
#include <string.h>

#include "../include/router.h"  // for Router

void freeRouterArray(struct Router** routerArray, size_t N) {
  if (*routerArray != NULL) {
    // Free the malloced strings
    for (size_t i = 0; i < N; ++i) {
      if ((*routerArray)[i].producerModel != NULL) {
        // Cast to void pointer since we are dealing with a const char *
        free((void*)(*routerArray)[i].producerModel);
        (*routerArray)[i].producerModel = NULL;
      }
    }
    free(*routerArray);
    *routerArray = NULL;
  }
  return;
}

void freeCommandStrCpy(char** commandStrCpy, const char* errorMsg) {
  free(*commandStrCpy);
  *commandStrCpy = NULL;
  if (strcmp(errorMsg, "") == 0) {
    return;
  }
  fprintf(stderr, "%s", errorMsg);
  return;
}
