#include "../include/command.h"

#include <stddef.h>
#include <stdio.h>  // for printf, NULL, snprintf
#include <stdlib.h>
#include <string.h>

int getCommand(const char *commandStr, char *command, char **args,
               size_t *nArgs) {
  size_t strLen;
  char *token;
  const char *format = "%s";

  // Copy the commandStr as strtok will alter the string
  strLen = snprintf(NULL, 0, format, commandStr);
  char *commandStrCpy = (char *)malloc((strLen + 1) * sizeof(char));
  if (commandStrCpy == NULL) {
    perror("Could not allocate memory to commandStrCpy: ");
    return EXIT_FAILURE;
  }
  snprintf(commandStrCpy, strLen, "%s", commandStr);

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
  token = strtok(commandStrCpy, " ");
  strLen = snprintf(NULL, 0, format, commandStr);
  command = (char *)malloc((strLen + 1) * sizeof(char));
  if (command == NULL) {
    free(commandStrCpy);
    perror("Could not allocate memory to command: ");
    return EXIT_FAILURE;
  }
  snprintf(command, strLen, "%s", token);

  // Special case for set_model
  if (strcmp(command, "setModel") == 0) {
    // We know that this function only has two arguments
    *nArgs = 2;

    *args = (char *)malloc(*nArgs * sizeof(char));
    if (args == NULL) {
      free(commandStrCpy);
      free(command);
      perror("Could not allocate memory to args: ");
      return EXIT_FAILURE;
    }

    // The following word is the routerID
    token = strtok(NULL, " ");
    strLen = strlen(token);
    args[0] = (char *)malloc((strLen + 1) * sizeof(char));
    if (args[0] == NULL) {
      free(commandStrCpy);
      free(command);
      free(args);
      perror("Could not allocate memory to args[0]: ");
      return EXIT_FAILURE;
    }
    snprintf(args[0], strLen, "%s", token);

    // The rest of the string is the model name and can contain spaces
    // -2 for the spaces which are not captured in commandStr and command
    strLen = strlen(commandStr) - strlen(command) - strLen - 2;
    args[1] = (char *)malloc((strLen + 1) * sizeof(char));
    if (args[1] == NULL) {
      free(commandStrCpy);
      free(command);
      free(args[0]);
      free(args);
      perror("Could not allocate memory to args[1]: ");
      return EXIT_FAILURE;
    }
    snprintf(args[1], strLen, "%s", &commandStr[strlen(commandStr) - strLen]);

    // Free the copy
    free(commandStrCpy);
    return EXIT_SUCCESS;
  }

  // Allocate space for the args array
  *nArgs = nSpaces - 1;
  *args = (char *)malloc(nSpaces * sizeof(char));

  // Capture the args
  for (size_t i = 0; i < *nArgs; ++i) {
    token = strtok(NULL, " ");
    strLen = strlen(token);
    args[i] = (char *)malloc((strLen + 1) * sizeof(char));
    if (args[i] == NULL) {
      free(commandStrCpy);
      free(command);
      for (size_t j = 0; j < i; ++j) {
        free(args[j]);
      }
      free(args);
      perror("Could not allocate memory to args[i]: ");
      return EXIT_FAILURE;
    }
    snprintf(args[i], strLen, "%s", token);
  }

  return EXIT_SUCCESS;
}
