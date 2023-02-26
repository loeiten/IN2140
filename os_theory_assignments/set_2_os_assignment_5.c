#include <stdio.h>     // for printf, fclose, fopen, snprintf, NULL, ferror
#include <stdlib.h>    // for exit, malloc, EXIT_FAILURE, atoi, free, rand
#include <sys/wait.h>  // for wait
#include <unistd.h>    // for fork, sleep, pid_t, NULL

#define BUFFER_SIZE 8  // Max 8 bytes for a 64 bit number

int sumFromTo(const int from, const int to) {
  int curSum = 0;
  for (int i = from; i <= to; ++i) {
    curSum += i;
  }
  return curSum;
}

void storeNumber(const char* path, const int number) {
  FILE* fp = fopen(path, "w");
  if (fp == NULL) {
    printf("Couldn't open '%s' for writing\n", path);
    exit(EXIT_FAILURE);
  }

  // Create the string
  const char* format = "%d";
  int length = snprintf(NULL, 0, format, number);
  char* str = (char*)malloc((length + 1) * sizeof(char));
  snprintf(str, length + 1, format, number);

  // Write to file
  int success = fputs(str, fp);
  free(str);
  if (success == EOF) {
    fclose(fp);
    printf("Failed to write '%d' to '%s'\n", number, path);
    exit(EXIT_FAILURE);
  }

  // Close the file
  fclose(fp);
  return;
}

int readNumber(const char* path) {
  // Open file
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    printf("Couldn't open '%s' for reading\n", path);
    exit(EXIT_FAILURE);
  }

  // Read into the buffer
  char buffer[BUFFER_SIZE];
  int nRead = fread(buffer, sizeof(char), BUFFER_SIZE, fp);
  if ((nRead < 0) || ferror(fp)) {
    fclose(fp);
    printf("Failed to read from '%s'\n", path);
    exit(EXIT_FAILURE);
  }

  // Convert to integer
  int number = atoi(buffer);

  // Return the number
  fclose(fp);
  return number;
}

int main() {
  pid_t pid = 0;  // 0 is the PID for the parent process
  int offset = 0;

  for (int i = 0; i < 3; ++i) {
    if (pid == 0) {  // Only process 0 can spawn processes
      pid = fork();
      if (pid != 0) {  // Only children should increase the offset
        offset += 1000;
      }
    }
  }

  // NOTE: The parent process is the first condition, although the children
  //       processes will logically exit first
  if (pid == 0) {
    // Only the parent will do this

    // Wait for all processes to finish
    int status = 0;
    pid_t waitPID = 0;
    while (waitPID > 0) {
      waitPID = wait(&status);
      if (status != 0) {
        printf("Process '%d' exited with status '%d'\n", waitPID, status);
        exit(EXIT_FAILURE);
      }
    }

    // Read the numbers and sum them up
    int sum = 0;
    for (int proc = 1; proc <= 3; ++proc) {
      // Create the path
      const char* format = "proc_%d.txt";
      int length = snprintf(NULL, 0, format, proc);
      char* path = (char*)malloc((length + 1) * sizeof(char));
      snprintf(path, length + 1, format, proc);

      // Add the result
      sum += readNumber(path);
    }

    // Print the final sum
    printf("sum(1,3000)=%d\n", sum);
  } else {
    // Only the children will do this

    // Calculate the sum
    int number = sumFromTo(offset + 1, offset + 1000);

    // Get a random number between 0 and 5
    // The number generated will not be portable
    // https://stackoverflow.com/a/3503847/2786884
    unsigned int seed = 1337 + offset;
    srand(seed);  // Seed the generator
    int maxN = 5;
    int minN = 0;
    int randomN = rand_r(&seed) % ((maxN + 1) - minN) + minN;
    printf("Process %d going to sleep for %d seconds", pid, randomN);
    sleep(randomN);

    // Create the path based on the offset
    int proc = offset / 1000;
    const char* format = "proc_%d.txt";
    int length = snprintf(NULL, 0, format, proc);
    char* path = (char*)malloc((length + 1) * sizeof(char));
    snprintf(path, length + 1, format, proc);

    // Write to file
    storeNumber(path, number);
  }

  return EXIT_SUCCESS;
}
