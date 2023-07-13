#include "../include/utils.h"

void getBinaryString(const unsigned char c, char* const binaryStr) {
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
