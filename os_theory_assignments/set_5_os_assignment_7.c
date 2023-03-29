#include <libgen.h>    // for basename
#include <stdio.h>     // for perror, printf
#include <stdlib.h>    // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>    // for strlen, strcpy
#include <sys/ipc.h>   // for IPC_CREAT, IPC_NOWAIT, key_t
#include <sys/msg.h>   // for msgget, msgrcv, msgsnd, pid_t
#include <sys/wait.h>  // for wait
#include <unistd.h>    // for fork

#define MAXLEN 100

// This is the structure the arguments of msgsnd and msgrcv takes
struct msgBuf {
  long mType;
  char mText[MAXLEN];
};

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: ./%s STR\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Protect against too long messages
  int len = strlen(argv[1]);
  if (len > MAXLEN - 1) {
    printf("String too long\n");
    return EXIT_FAILURE;
  }

  struct msgBuf msg;
  int msgKey = 77;   // Which mailbox will we communicate through
  int msgType = 42;  // What "color" should the envelope have

  // Get the message queue
  // The queue will be created if not already created
  int msgQueueId = msgget((key_t)msgKey, IPC_CREAT | 0666);
  if (msgQueueId == -1) {
    perror("Failed to get the message queue id\n");
    return EXIT_FAILURE;
  }

  pid_t pid = fork();

  if (pid == 0) {  // Child
    // We let the child send the message
    // Create the message
    msg.mType = msgType;
    int strBufferLen = strlen(argv[1]) + 1;
    snprintf(msg.mText, strBufferLen, "%s", argv[1]);
    // Send the message
    int status = msgsnd(msgQueueId,            // The queue
                        (struct msgBuf*)&msg,  // The message struct
                        len + 1,  // The length to be send (includes \0)
                        0);       // Flag
    if (status == -1) {
      perror("Failed to send message\n");
      return EXIT_FAILURE;
    }
  } else {  // Parent
    // We let the parent read the message
    // Wait until the child has sent the message
    int status = 0;
    int waitPID = wait(&status);  // NOTE: wait is from POSIX1-2001
    if (status != 0) {
      printf("Process '%d' exited with status '%d'\n", waitPID, status);
      perror("wait failed");
      return EXIT_FAILURE;
    }

    status = msgrcv(msgQueueId,   // The queue
                    (void*)&msg,  // The message struct
                    MAXLEN,       // Length to be read
                    msgType,      // The color to read
                    IPC_NOWAIT);  // Do not block

    if (status == -1) {
      perror("Failed to receive message\n");
      return EXIT_FAILURE;
    }

    printf("%s from pid: %d\n", msg.mText, pid);
  }

  return EXIT_SUCCESS;
}
