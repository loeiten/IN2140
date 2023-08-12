#include "../include/command.h"

#include <stdio.h>   // for perror, snprintf
#include <stdlib.h>  // for malloc, EXIT_FAILURE, EXIT_SU...
#include <string.h>  // for strlen, strtok_r, strcmp

#include "../include/dynamic_memory.h"  // for freeCommandStrCpy
#include "../include/router.h"          // for printRouter

int getCommand(const char *const commandStr, char **const command,
               char ***const args, size_t *const nArgs) {
  // Initialize memory to be allocated
  char *commandStrCpy = NULL;

  // Initialize helper variables
  const char *format = "%s";

  // Declare helper variables
  int strLen;
  int charWritten;  // Number of chars not including terminating char
  char *token;
  char *savePtr;

  // Copy the commandStr as strtok_r will alter the string
  strLen = snprintf(NULL, 0, format, commandStr);
  commandStrCpy = (char *)malloc((strLen + 1) * sizeof(char));
  if (commandStrCpy == NULL) {
    perror("Could not allocate memory to commandStrCpy: ");
    return EXIT_FAILURE;
  }
  charWritten = snprintf(commandStrCpy, (strLen + 1), format, commandStr);
  if ((charWritten < 0) || (charWritten > strLen)) {
    freeCommandStrCpy(&commandStrCpy, "Failed to copy the commandStr\n");
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
    freeCommandStrCpy(&commandStrCpy, "Failed to get strLen of commandStr\n");
    return EXIT_FAILURE;
  }
  *command = (char *)malloc((strLen + 1) * sizeof(char));
  if (*command == NULL) {
    freeCommandStrCpy(&commandStrCpy, "");
    perror("Could not allocate memory to command: ");
    return EXIT_FAILURE;
  }
  charWritten = snprintf(*command, strLen + 1, format, token);
  if ((charWritten < 0) || (charWritten > strLen)) {
    freeCommandStrCpy(&commandStrCpy, "Failed to copy the command\n");
    return EXIT_FAILURE;
  }

  // Special case for set_model
  if (strcmp(*command, "set_model") == 0) {
    // We know that this function only has two argument
    *nArgs = 2;

    *args = (char **)malloc(*nArgs * sizeof(char *));
    if (**args == NULL) {
      freeCommandStrCpy(&commandStrCpy, "");
      perror("Could not allocate memory to args: ");
      return EXIT_FAILURE;
    }

    // The following word is the routerID
    token = strtok_r(NULL, " ", &savePtr);
    strLen = strlen(token);
    (*args)[0] = (char *)malloc((strLen + 1) * sizeof(char));
    if ((*args)[0] == NULL) {
      freeCommandStrCpy(&commandStrCpy, "");
      perror("Could not allocate memory to args[0]: ");
      return EXIT_FAILURE;
    }
    charWritten = snprintf((*args)[0], strLen + 1, format, token);
    if ((charWritten < 0) || (charWritten > strLen)) {
      freeCommandStrCpy(&commandStrCpy, "Failed to copy to arg[0]\n");
      return EXIT_FAILURE;
    }

    // The rest of the string is the model name and can contain spaces
    // -2 for the spaces which are not captured in commandStr and command
    strLen = strlen(commandStr) - strlen(*command) - strLen - 2;
    (*args)[1] = (char *)malloc((strLen + 1) * sizeof(char));
    if ((*args)[1] == NULL) {
      freeCommandStrCpy(&commandStrCpy, "");
      perror("Could not allocate memory to args[1]: ");
      return EXIT_FAILURE;
    }
    charWritten = snprintf((*args)[1], strLen + 1, format,
                           &(commandStr[strlen(commandStr) - strLen]));
    if ((charWritten < 0) || (charWritten > strLen)) {
      freeCommandStrCpy(&commandStrCpy, "Failed to copy to arg[1]\n");
      return EXIT_FAILURE;
    }
    // Remove possible newline
    if ((*args)[1][strLen - 1] == '\n') {
      (*args)[1][strLen - 1] = '\0';
    }

    // Free the copy
    freeCommandStrCpy(&commandStrCpy, "");
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
    if ((*args)[i] == NULL) {
      freeCommandStrCpy(&commandStrCpy, "");
      perror("Could not allocate memory to args[i]: ");
      return EXIT_FAILURE;
    }
    charWritten = snprintf((*args)[i], strLen + 1, format, token);
    if ((charWritten < 0) || (charWritten > strLen)) {
      freeCommandStrCpy(&commandStrCpy, "Failed to copy to arg[i]\n");
      return EXIT_FAILURE;
    }
  }

  freeCommandStrCpy(&commandStrCpy, "");
  return EXIT_SUCCESS;
}

int runCommand(const char *const command, const char *const *const args,
               struct Router **routerArray, unsigned int *N) {
  int success;
  if (strcmp(command, "print") == 0) {
    const int routerId = atoi(args[0]);
    success = printRouter(*routerArray, *N, routerId);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
  } else if (strcmp(command, "set_flag") == 0) {
    const int routerId = atoi(args[0]);
    const int flag = atoi(args[1]);
    const int value = atoi(args[2]);
    success = setFlag(*routerArray, *N, routerId, flag, value);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
  } else if (strcmp(command, "set_model") == 0) {
    const int routerId = atoi(args[0]);
    const char *name = args[1];
    success = setModel(routerArray, *N, routerId, name);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
  } else if (strcmp(command, "add_link") == 0) {
    const unsigned char fromRouter = atoi(args[0]);
    const unsigned char toRouter = atoi(args[1]);
    success = addLink(*routerArray, *N, fromRouter, toRouter);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
  } else if (strcmp(command, "delete_router") == 0) {
    const int routerId = atoi(args[0]);
    success = deleteRouter(routerArray, N, routerId);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
  } else if (strcmp(command, "exist_route") == 0) {
    const unsigned char fromRouterId = atoi(args[0]);
    const unsigned char toRouterId = atoi(args[1]);
    int exists = -1;
    success = existsRoute(*routerArray, *N, fromRouterId, toRouterId, &exists);
    if (success != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    const char *existsString = (exists == 1) ? "exists" : "does not exist";
    printf("Route from router %u to router %u %s\n", fromRouterId, toRouterId,
           existsString);
  }

  return EXIT_SUCCESS;
}
