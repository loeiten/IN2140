#include "../include/router.h"  // for freeRouterArray

#include <libgen.h>  // for basename
#include <stddef.h>
#include <stdio.h>   // for fprintf, NULL, stderr, size_t
#include <stdlib.h>  // for free, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strcmp, strrchr

#include "../include/binary_file.h"  // for readBinaryFile
#include "../include/command.h"      // for getCommand

int printStruct(const struct Router* const routerArray, const unsigned int N,
                const int routerId) {
  int hitIdx = -1;
  for (size_t i = 0; i < N; ++i) {
    if (((int)routerArray[i].routerId) == routerId) {
      hitIdx = i;
    }
  }
  if (hitIdx == -1) {
    fprintf(stderr, "Could not find routerId %d in routerArray", routerId);
    return EXIT_FAILURE;
  }

  // 8 for the number of bits
  // +1 for the null terminator
  char binaryFlag[sizeof(unsigned char) * 8 + 1];
  getBinaryString(routerArray[hitIdx].flag, binaryFlag);

  // FIXME: Capture the neighbors
  const char* active = (binaryFlag[7] == '1') ? "Yes" : "No";
  const char* wireless = (binaryFlag[6] == '1') ? "Yes" : "No";
  const char* fiveGHz = (binaryFlag[5] == '1') ? "Yes" : "No";
  // Extract the modification number
  unsigned int bitLen = 4;
  char modificationNumberStr[bitLen + 1];
  // Reverse the order of bit in the binaryFlag
  for (size_t i = 0; i < bitLen; ++i) {
    // -1 as we start counting from 0
    modificationNumberStr[i] = binaryFlag[bitLen - i - 1];
  }
  modificationNumberStr[bitLen] = '\0';  // Null terminator
  long modificationNumber = strtol(modificationNumberStr, NULL, 2);
  printf(
      "\nName: %s\nRouterId: %d\nActive: %s\nWireless: %s\n5 GHz: "
      "%s\nModification number: %ld\nFlag (unsigned char): %c\nFlag (int): "
      "%d\nFlag (binary): %s\n",
      routerArray[hitIdx].producerModel, routerId, active, wireless, fiveGHz,
      modificationNumber, routerArray[hitIdx].flag,
      (int)routerArray[hitIdx].flag, binaryFlag);
  return EXIT_SUCCESS;
}

void getBinaryString(unsigned char c, char* const binaryStr) {
  // Number of bits in an unsigned char
  int numBits = sizeof(c) * 8;

  // Iterate over each bit from the most significant bit to the least
  // significant bit
  for (int i = numBits - 1; i >= 0; --i) {
    // Check if the i-th bit is set
    // First move the bits of c i places to the right
    // Then check the bitwise AND with ...0001
    unsigned char bit = (c >> i) & 1;

    // Store the bit as a character '0' or '1' in the binary string
    // -1 as we start counting from 0
    binaryStr[(numBits - i) - 1] = (bit == 1) ? '1' : '0';
  }

  // Null-terminate the binary string
  binaryStr[numBits] = '\0';
}
