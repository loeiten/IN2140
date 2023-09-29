#include <assert.h>  // for assert
#include <libgen.h>  // for basename
#include <stdio.h>   // for fprintf, NULL, stderr
#include <stdlib.h>  // for EXIT_SUCCESS, EXIT_F...
#include <string.h>  // for strcmp

#include "../routing_server/include/receiver.h"  // for checkAllNodesReceived

void testCheckAllNodesReceived(void) {
#define N (1)
  struct ReceivedNode* receivedNodeArray = NULL;
  struct EdgeArray* nonValidEdgesArray = NULL;
  int success = checkAllNodesReceived(receivedNodeArray, nonValidEdgesArray, N);
  assert(success == EXIT_SUCCESS);
#undef N
}

void testCreateAdjacencyMatrix(void) {
#define N (1)
  struct ReceivedNode* receivedNodeArray = NULL;
  struct EdgeArray* nonValidEdgesArray = NULL;
  int** adjacencyMatrix = NULL;
  int success = createAdjacencyMatrix(receivedNodeArray, nonValidEdgesArray,
                                      &adjacencyMatrix, N);
  assert(success == EXIT_SUCCESS);
#undef N
}

int main(int argc, char** argv) {
  if (argc < 2) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
    fprintf(stderr, "Usage: ./%s test args\n1", basename(argv[0]));
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "checkAllNodesReceived") == 0) {
    testCheckAllNodesReceived();
  } else if (strcmp(argv[1], "createAdjacencyMatrix") == 0) {
    testCreateAdjacencyMatrix();
  } else {
    fprintf(stderr, "No test named %s in %s\n", argv[1], basename(argv[0]));
  }

  return EXIT_SUCCESS;
}
