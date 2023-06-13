#include <stdio.h>   // for fprintf, perror, NULL, size_t, snprintf, stderr
#include <stdlib.h>  // for free, malloc, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strlen, strtok_r, strcmp

int getCommand(const char *commandStr, char **command, char ***args,
               size_t *nArgs) {
  size_t strLen;
  int charWritten;  // Number of chars not including terminating char
  char *token;
  char *savePtr;
  const char *format = "%s";

  // Copy the commandStr as strtok_r will alter the string
  strLen = snprintf(NULL, 0, format, commandStr);
  char *commandStrCpy = (char *)malloc((strLen + 1) * sizeof(char));
  if (commandStrCpy == NULL) {
    perror("Could not allocate memory to commandStrCpy: ");
    return EXIT_FAILURE;
  }
  charWritten = snprintf(commandStrCpy, (strLen + 1), "%s", commandStr);
  if ((charWritten < 0) || (charWritten > strLen)) {
    free(commandStrCpy);
    fprintf(stderr, "Failed to copy the commandStr\n");
    return EXIT_FAILURE;
  }

  // Count the number of arguments
  size_t nSpaces = 0;
  size_t i = 0;
  while (commandStr[i] != 0) {
    if (commandStr[i] == ' ') {
      ++nSpaces;
    }
    ++i;
  }

  // The first word is always the command
  // Find the first space
  // NOTE: strtok_r is the thread safe version of strtok
  token = strtok_r(commandStrCpy, " ", &savePtr);
  strLen = snprintf(NULL, 0, format, commandStr);
  if (strLen < 1) {
    free(commandStrCpy);
    fprintf(stderr, "Failed to get strLen of commandStr\n");
    return EXIT_FAILURE;
  }
  *command = (char *)malloc((strLen + 1) * sizeof(char));
  if (*command == NULL) {
    free(commandStrCpy);
    perror("Could not allocate memory to command: ");
    return EXIT_FAILURE;
  }
  charWritten = snprintf(*command, strLen + 1, "%s", token);
  if ((charWritten < 0) || (charWritten > strLen)) {
    free(commandStrCpy);
    free(*command);
    fprintf(stderr, "Failed to copy the command\n");
    return EXIT_FAILURE;
  }

  // Special case for set_model
  if (strcmp(*command, "setModel") == 0) {
    // We know that this function only has two arguments
    *nArgs = 2;

    *args = (char **)malloc(*nArgs * sizeof(char *));
    if (**args == NULL) {
      free(commandStrCpy);
      free(*command);
      perror("Could not allocate memory to args: ");
      return EXIT_FAILURE;
    }

    // The following word is the routerID
    token = strtok_r(NULL, " ", &savePtr);
    strLen = strlen(token);
    (*args)[0] = (char *)malloc((strLen + 1) * sizeof(char));
    if (args[0] == NULL) {
      free(commandStrCpy);
      free(*command);
      free(*args);
      perror("Could not allocate memory to args[0]: ");
      return EXIT_FAILURE;
    }
    charWritten = snprintf((*args)[0], strLen + 1, "%s", token);
    if ((charWritten < 0) || (charWritten > strLen)) {
      free(commandStrCpy);
      free(*command);
      free((*args)[0]);
      free(*args);
      fprintf(stderr, "Failed to copy to arg[0]\n");
      return EXIT_FAILURE;
    }

    // The rest of the string is the model name and can contain spaces
    // -2 for the spaces which are not captured in commandStr and command
    strLen = strlen(commandStr) - strlen(*command) - strLen - 2;
    (*args)[1] = (char *)malloc((strLen + 1) * sizeof(char));
    if (args[1] == NULL) {
      free(commandStrCpy);
      free(*command);
      free((*args)[0]);
      free((*args));
      perror("Could not allocate memory to args[1]: ");
      return EXIT_FAILURE;
    }
    charWritten = snprintf((*args)[1], strLen, "%s",
                           &commandStr[strlen(commandStr) - strLen]);
    if ((charWritten < 0) || (charWritten > strLen)) {
      free(commandStrCpy);
      free(*command);
      free((*args)[0]);
      free((*args)[1]);
      free((*args));
      fprintf(stderr, "Failed to copy to arg[1]\n");
      return EXIT_FAILURE;
    }

    // Free the copy
    free(commandStrCpy);
    return EXIT_SUCCESS;
  }

  // Allocate space for the args array
  *nArgs = nSpaces;
  *args = (char **)malloc(nSpaces * sizeof(char *));

  // Capture the args
  for (i = 0; i < *nArgs; ++i) {
    token = strtok_r(NULL, " ", &savePtr);
    strLen = strlen(token);
    (*args)[i] = (char *)malloc((strLen + 1) * sizeof(char));
    if (args[i] == NULL) {
      free(commandStrCpy);
      free(*command);
      for (size_t j = 0; j < i; ++j) {
        free((*args)[j]);
      }
      free(*args);
      perror("Could not allocate memory to args[i]: ");
      return EXIT_FAILURE;
    }
    charWritten = snprintf((*args)[i], strLen + 1, "%s", token);
    if ((charWritten < 0) || (charWritten > strLen)) {
      free(commandStrCpy);
      free(*command);
      for (size_t j = 0; j <= i; ++j) {
        free((*args)[j]);
      }
      free(*args);
      fprintf(stderr, "Failed to copy to arg[i]\n");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
