#include <libgen.h>    // for basename
#include <stdio.h>     // for perror, printf
#include <stdlib.h>    // for EXIT_FAILURE, NULL, EXIT_SUCCESS
#include <string.h>    // for strlen, memcpy
#include <sys/ipc.h>   // for IPC_CREAT, key_t
#include <sys/shm.h>   // for shmat, shmget, pid_t
#include <sys/wait.h>  // for wait
#include <unistd.h>    // for fork

#define SHMSZ (100)

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./%s STR\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Protect against too long messages
  int len = strlen(argv[1]);
  if (len > SHMSZ - 1) {
    printf("String too long\n");
    return EXIT_FAILURE;
  }

  int shmKey = 77;  // Which key to use for th shared memory

  // Create the segment
  // The segment will be created if not already created
  int shmId = shmget((key_t)shmKey, SHMSZ, IPC_CREAT | 0666);
  if (shmId == -1) {
    perror("Failed to get the shared memory id\n");
    return EXIT_FAILURE;
  }

  pid_t pid = fork();

  if (pid == 0) {  // Child
    // We let the child populate the shared memory area
    // Attach the segment to the data space
    char *shm =
        shmat(shmId,  // The Id
              NULL,   // The address (system choses a suitable address if NULL)
              0);     // Flags

    // shmat returns (void *) -1 on failure
    // For details see
    // https://stackoverflow.com/q/13306914/2786884
    if (shm == (void *)-1) {
      perror("Failed to attach to shared memory\n");
      return EXIT_FAILURE;
    }

    // Populate the shared memory area
    int strBufferLen = strlen(argv[1]) + 1;
    memcpy(shm,                           // Destination
           argv[1],                       // Source
           strBufferLen * sizeof(char));  // Number of bytes to copy
  } else {                                // Parent
    // We let the parent read from the shared memory area
    // Wait until the child has populated the area
    int status = 0;
    int waitPID = wait(&status);  // NOTE: wait is from POSIX1-2001
    if (status != 0) {
      printf("Process '%d' exited with status '%d'\n", waitPID, status);
      perror("wait failed");
      return EXIT_FAILURE;
    }

    // Attach the segment to the data space
    char *shm =
        shmat(shmId,  // The Id
              NULL,   // The address (system choses a suitable address if NULL)
              0);     // Flags

    // shmat returns (void *) -1 on failure
    // For details see
    // https://stackoverflow.com/q/13306914/2786884
    if (shm == (void *)-1) {
      perror("Failed to attach to shared memory\n");
      return EXIT_FAILURE;
    }

    char msg[SHMSZ];

    // Populate the shared memory area
    int strBufferLen = strlen(argv[1]) + 1;
    memcpy(msg,                           // Destination
           shm,                           // Source
           strBufferLen * sizeof(char));  // Number of bytes to copy

    printf("%s from pid: %d\n", msg, pid);
  }

  return EXIT_SUCCESS;
}
