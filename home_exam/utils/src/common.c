#include "../include/common.h"

#include <stdio.h>
#include <stdlib.h>

int getIndexFromAddress(const int address,
                        const struct IndexToAddress* const indexToAddress,
                        int* index) {
  *index = -1;
  for (int i = 0; i < indexToAddress->n; ++i) {
    if (address == indexToAddress->map[i]) {
      *index = i;
      return EXIT_SUCCESS;
    }
  }
  fprintf(stderr, "Could not find address %d in the indexToAddress map\n",
          address);
  return EXIT_FAILURE;
}
