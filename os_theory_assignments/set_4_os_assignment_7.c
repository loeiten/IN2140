#include <stdio.h>     // for printf, fclose, fflush, fopen, ftell, FILE, fputs
#include <stdlib.h>    // for exit, EXIT_FAILURE, NULL, EXIT_SUCCESS
#include <sys/wait.h>  // for wait
#include <unistd.h>    // for fork, pid_t, NULL

void writeToFile(FILE* fp, const char* text) {
  int pos;
  printf("Address of fp=%p\n", (void*)fp);

  pos = ftell(fp);
  printf("Currently at %d bytes offset in the file\n", pos);
  fputs(text, fp);
  pos = ftell(fp);
  printf("Currently at %d bytes offset in the file\n", pos);
}

pid_t openAfterFork(void) {
  const char* filename = "set_4_os_assignment_7_open_after_fork.txt";

  pid_t pid = fork();
  FILE* fp0;
  FILE* fp;

  if (pid == 0) {  // Child
    fp0 = fopen(filename, "w");
    if (fp0 == NULL) {
      printf("pid %d couldn't open '%s' for writing\n", pid, filename);
      exit(EXIT_FAILURE);
    }
    printf("pid %d opened '%s' for writing\n", pid, filename);
    writeToFile(fp0, "Hello ");
  } else {  // Parent
    fp = fopen(filename, "w");
    if (fp == NULL) {
      printf("pid %d couldn't open '%s' for writing\n", pid, filename);
      exit(EXIT_FAILURE);
    }
    printf("pid %d opened '%s' for writing\n", pid, filename);
    // Avoid race
    int status = 0;
    int waitPID = wait(&status);  // NOTE: wait is from POSIX1-2001
    if (status != 0) {
      printf("Process '%d' exited with status '%d'\n", waitPID, status);
      exit(EXIT_FAILURE);
    }
    writeToFile(fp, "world\n");
  }

  // Close the files
  // NOTE: close closes the file descriptor of the process
  if (pid == 0) {  // Child
    fclose(fp0);
    printf("pid %d closed '%s'\n", pid, filename);
  } else {  // Parent
    fclose(fp);
    printf("pid %d closed '%s'\n", pid, filename);
  }

  return pid;
}

void openBeforeFork(void) {
  const char* filename = "set_4_os_assignment_7_open_before_fork.txt";

  FILE* fp = fopen(filename, "w");
  if (fp == NULL) {
    printf("Parent couldn't open '%s' for writing\n", filename);
    exit(EXIT_FAILURE);
  }
  printf("Parent opened '%s' for writing\n", filename);
  fflush(stdout);  // So that only one process writes the above

  pid_t pid = fork();
  if (pid == 0) {  // Child
    writeToFile(fp, "Hello ");
  } else {  // Parent
    // Avoid race
    int status = 0;
    int waitPID = wait(&status);  // NOTE: wait is from POSIX1-2001
    if (status != 0) {
      printf("Process '%d' exited with status '%d'\n", waitPID, status);
      exit(EXIT_FAILURE);
    }
    writeToFile(fp, "world\n");
  }

  if (pid != 0) {  // Parent
    fclose(fp);
    printf("Parent closed '%s'\n", filename);
  }
}

int main(void) {
  printf("Open file AFTER fork:\n---\n");
  fflush(stdout);  // So that only one process writes the above
  pid_t pid = openAfterFork();

  // Only the parent should execute the following
  if (pid != 0) {
    sleep(1);  // Sleep in order for the other function call to complete
    printf("\nOpen file BEFORE fork:\n---\n");
    fflush(stdout);  // So that only one process writes the above
    openBeforeFork();
  }

  return EXIT_SUCCESS;
}
