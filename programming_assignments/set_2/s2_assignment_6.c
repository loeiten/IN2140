#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS

int main(int argc, char** argv) {
  // NOTE: We want to have the whole register in memory, if not, we should've
  //       traversed the file line by line
  // Read register
  // Read whom to remove
  // Remove remove from the register
  // Read whom to add
  // Add to the register
  // Store the register to a "dirty file"
  // Read the register from the "dirty file"
  // Print the register

  if (argc < 2) {
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s csvPath [removePath] [addPath]\n", basename(argv[0]));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
